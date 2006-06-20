#include <string.h>

#include <gnome.h>
#include <gtk/gtk.h>
#include <gtk/gtklabel.h>
#include <gtk/gtkbox.h>
#include <gtk/gtkimage.h>
#include <gtk/gtkimage.h>


#include <pthread.h>

/* #define FAKED_MAIN */
#include <panel-applet.h>

#include "libxnee/xnee.h"
#include "libxnee/xnee_alloc.h"
#include "libxnee/print_varargs.h"
#include "libxnee/print.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_range.h"
#include "libxnee/xnee_session.h"
#include "libxnee/xnee_fileop.h"

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

typedef enum _pnee_actions
  {
    PNEE_ACTION_NONE,
    PNEE_ACTION_RECORD,
    PNEE_ACTION_REPLAY
  } pnee_actions;


typedef enum _pnee_update_actions
  {
    PNEE_UPDATE_NONE,
    PNEE_UPDATE_RECORD,
    PNEE_UPDATE_REPLAY
  } pnee_update_actions;


void
pnee_start_recording(void *);

int
pnee_start_replaying(void);

typedef struct
{
  PanelApplet *applet;
  GtkWidget *button_box;	
  GtkWidget *label;	
  GtkWidget *container;
  GtkTooltips *tooltips;

  GtkWidget *rec_button;
  GtkWidget *rep_button;
  GtkWidget *stop_button;

  GtkWidget *rec_image;
  GtkWidget *rep_image;
  GtkWidget *stop_image;

  GtkTooltips* rec_tooltip;
  GtkTooltips* rep_tooltip;
  GtkTooltips* stop_tooltip;
  GtkWidget *progress;

  GtkIconSize icon_size;

  pthread_t xnee_thread;
  pthread_t xnee_updater;

#define PNEE_DEBUGGER
#ifdef PNEE_DEBUGGER
  pthread_t xnee_debugger;
#endif /* PNEE_DEBUGGER */
} pnee_panel_applet;

pnee_panel_applet pnee_applet_impl;
pnee_panel_applet *pnee_applet;
xnee_data *xd ;
GtkWidget *pnee_pref;
extern GtkWidget* fs;

#define _IN  gdk_threads_enter()
#define _OUT gdk_threads_leave()

#define pnee_set_no_action() _IN; action_state=PNEE_ACTION_NONE ; _OUT
#define pnee_set_recording() _IN; action_state=PNEE_ACTION_RECORD ; _OUT
#define pnee_set_replaying() _IN; action_state=PNEE_ACTION_REPLAY ; _OUT
#define pnee_is_recording() (action_state==PNEE_ACTION_RECORD) 
#define pnee_is_replaying() (action_state==PNEE_ACTION_REPLAY)
pnee_actions action_state = PNEE_ACTION_NONE;


#define pnee_set_update_no_action() update_state=PNEE_UPDATE_NONE 
#define pnee_set_update_recording() update_state=PNEE_UPDATE_RECORD 
#define pnee_set_update_replaying() update_state=PNEE_UPDATE_REPLAY 
pnee_update_actions update_state = PNEE_UPDATE_NONE;



extern gchar *filename;


static const char Context_menu_xml [] =
   "<popup name=\"button3\">\n"
   "   <menuitem name=\"Record Item\" "
   "             verb=\"Record\" "
   "           _label=\"_Record session...\"\n"
   "          pixtype=\"stock\" "
   "          pixname=\"gtk-media-record\"/>\n"
   "   <menuitem name=\"Replay Item\" "
   "             verb=\"Replay\" "
   "           _label=\"_Replay session...\"\n"
   "          pixtype=\"stock\" "
   "          pixname=\"gtk-media-play\"/>\n"
   "   <menuitem name=\"Properties Item\" "
   "             verb=\"Properties\" "
   "           _label=\"_Preferences...\"\n"
   "          pixtype=\"stock\" "
   "          pixname=\"gtk-properties\"/>\n"
   "   <menuitem name=\"About Item\" "
   "             verb=\"About\" "
   "           _label=\"_About...\"\n"
   "          pixtype=\"stock\" "
   "          pixname=\"gnome-stock-about\"/>\n"
   "</popup>\n";

#define pnee_show_states()       fprintf(stderr, " pnee : %d %d \n", action_state, update_state);


#ifdef PNEE_DEBUGGER
void
pnee_debugger(void *data)
{
  while (1)
    {
      pnee_show_states();
      usleep (1000*1000);
    }
}
#endif /* PNEE_DEBUGGER */

void 
pnee_progress_updater(void *pnee_applet)
{
  gint max ;
  gint curr;
  gdouble perc ;

  if ( pnee_is_replaying() )
    {
      pnee_set_update_replaying();
    }
  else if ( pnee_is_recording() )
    {
      pnee_set_update_recording();
    }
  
  if ( !pnee_is_replaying() && !pnee_is_recording() )
    {
      pnee_set_update_no_action()  ;
      return;
    }

  while ( pnee_is_replaying() || pnee_is_recording()  )
    {
      max  = xnee_get_events_max(xd);
      if ( pnee_is_replaying() )
	{
	  curr = xnee_get_events_replayed(xd);
	}
      else
	{
	  curr = xnee_get_events_recorded (xd);
	}
      
      perc = (gdouble) curr / (gdouble) max ;
      
      fprintf(stderr, " update: %d %d ===>  perc = %f \n", 
	      curr, max, perc);
      
      pnee_update_progress(pnee_applet, perc);
      
      usleep(1000*500);
    }

  pnee_set_update_no_action();
  pnee_update_progress(pnee_applet, 1.0);
  fprintf(stderr, "***  updater thread is fini!\n");
}

void
pnee_stop_pressed(GtkWidget *w, pnee_panel_applet * pnee_applet)
{
  int ev_max = 0;
  
  pnee_set_no_action();
  
  pnee_update_progress(pnee_applet, 0,0);
      
  fprintf(stderr, "stop pressed\n");
  fprintf(stderr, "stop pressed  xd=%d\n", xd);

  ev_max = xnee_get_events_max(xd);
  xnee_set_events_recorded (xd, ev_max);
}

void
pnee_rec_pressed(GtkWidget *w, pnee_panel_applet * pnee_applet)
{
  pnee_update_progress(pnee_applet, 0.0);
  pthread_create (&pnee_applet->xnee_thread, 
		  NULL, 
		  pnee_start_recording, 
		  NULL);

  pthread_create (&pnee_applet->xnee_updater, 
		  NULL, 
		  pnee_progress_updater,
		  pnee_applet);

}

void
pnee_rep_pressed(GtkWidget *w, pnee_panel_applet * pnee_applet)
{
    pthread_create (&pnee_applet->xnee_thread, 
		    NULL, 
		    pnee_start_replaying, 
		    NULL);
  pthread_create (&pnee_applet->xnee_updater, 
		  NULL, 
		  pnee_progress_updater,
		  pnee_applet);
}

gboolean
cb_button_press(GtkWidget *w, GdkEventButton *event, GtkWidget *applet)
{
    if ((event->button == 3) || (event->button == 2))
    {
	gtk_propagate_event(applet, (GdkEvent *) event);

	return TRUE;
    }

    return FALSE;
}


static void
register_stock_icons(pnee_panel_applet *pnee_applet, 
		     GtkIconFactory *factory)
{
    gint i;
    static gchar file_name[1024];
    const gchar *items[] = { "pnee-record.png", "pnee-replay.png" , "pnee-stop.png"};

    for (i = 0; i < sizeof(items) / sizeof(items[0]); ++i)
    {
	GdkPixbuf *pixbuf;
	GtkIconSet *icon_set;
        GError *error = NULL;

        sprintf(file_name, "%s/%s%s", PNEE_PIXDIR, items[i] );
	fprintf (stderr, "Trying to load file: %s\n", file_name);
	pixbuf = gdk_pixbuf_new_from_file(file_name, &error);
        if (pixbuf == NULL)
        {
	  fprintf(stderr, "no pixbuf...\n");
        }

	icon_set = gtk_icon_set_new_from_pixbuf(pixbuf);
	gtk_icon_factory_add(factory, items[i], icon_set);

	gtk_icon_set_unref(icon_set);
	g_object_unref(G_OBJECT(pixbuf));
    }
}


static void
init_stock_icons(pnee_panel_applet *pnee_applet)
{
    GtkIconFactory *factory;

    factory = gtk_icon_factory_new();
    gtk_icon_factory_add_default(factory);

    register_stock_icons(pnee_applet, factory);

    pnee_applet->icon_size = gtk_icon_size_register("pnee_icon_size", 5, 5);

    g_object_unref(factory); 
}




void
pnee_create_button(pnee_panel_applet *pnee_applet,
		   const gchar *tooltip_str, 
		   const gchar *id, 
		   GCallback    press_callback, 
		   int mode)
{
  GtkWidget *button;	
  GtkWidget *image;	
  GtkTooltips* tooltip;

  /* Button */
  button = gtk_button_new();
  fprintf (stderr, " button %d\n", button);
  gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_HALF);
  gtk_container_set_border_width (GTK_CONTAINER (button), 0);

  /* Image */
  image = gtk_image_new_from_stock(id, pnee_applet->icon_size);
  tooltip= gtk_tooltips_new();
  gtk_widget_show(image);

  /* Tooltip */
  gtk_container_add(GTK_CONTAINER(button), image);
  gtk_tooltips_set_tip(tooltip, button, tooltip_str, NULL);

  /* callback */
  g_signal_connect(button, "button_press_event",
		   G_CALLBACK(cb_button_press), pnee_applet->applet);
  g_signal_connect(button, "clicked", 
		   G_CALLBACK(press_callback), 
		   pnee_applet);

  gtk_widget_show(button);

  if (mode == 0 )
    {
      fprintf(stderr, "Adding to rec\n");
      pnee_applet->rec_button=button;
      pnee_applet->rec_image=image;
      pnee_applet->rec_tooltip=tooltip;
    }
  else if (mode == 1 )
    {
      fprintf(stderr, "Adding to replay\n");
      pnee_applet->rep_button=button;
      pnee_applet->rep_image=image;
      pnee_applet->rep_tooltip=tooltip;
    } 
  else if (mode == 2 )
    {
      fprintf(stderr, "Adding to stop\n");
      pnee_applet->stop_button=button;
      pnee_applet->stop_image=image;
      pnee_applet->stop_tooltip=tooltip;
    } 
}


int 
pnee_setup()
{
  char *default_tmp_file;
  char *default_err_file;
  static int already_setup = 0;

  if (already_setup)
    {
      return 0;
      XNEE_VERBOSE_LEAVE_FUNCTION();
    }
  already_setup = 1 ;

  /*  Get a new xnee_data structure  */
  xd = xnee_new_xnee_data();

  default_err_file=pnee_get_default_err_name();
  xnee_set_verbose(xd);
  pnee_set_err_file (default_err_file); 
  xnee_open_files(xd);
  xnee_unset_recall_window_pos (xd);
  xnee_set_events_max (xd, 200);
  xnee_set_data_max (xd, -1);

  XNEE_VERBOSE_ENTER_FUNCTION();

  fs = create_filechooserdialog1();

  xnee_set_key (xd, XNEE_GRAB_STOP, "a");

  /* Set the program name */
  xnee_set_program_name (xd, "pnee");

  /* Set the cli parameters */
  xnee_set_application_parameters (xd, NULL);

  if (pnee_pref==NULL)
    {
      pnee_pref = create_pnee_pref ();
    }

  default_tmp_file=pnee_get_default_filename();


  pnee_set_rec_file (default_tmp_file);
  pnee_set_rep_file (default_tmp_file);

  free (default_tmp_file);
  free (default_err_file);

  /* init threads 
  fprintf(stderr, " ****** g_thread_init ******\n");
  g_thread_init (NULL);
  */
  fprintf(stderr, " ****** gdk_thread_init ******\n");
  gdk_threads_init ();

  XNEE_VERBOSE_LEAVE_FUNCTION();
  return 0;
}

int
pnee_prepare_record()
{
  pnee_setup();

  XNEE_VERBOSE_ENTER_FUNCTION();

  xnee_parse_range (xd, XNEE_DEVICE_EVENT,  
		      "KeyPress-MotionNotify"); 
  xnee_set_recorder(xd);
  xnee_prepare(xd);

  XNEE_VERBOSE_LEAVE_FUNCTION();
  return 0;
}

int
pnee_prepare_replay()
{
  gchar * my_file;
  GtkFileChooserButton *file_text ; 
  
  pnee_setup();

  XNEE_VERBOSE_ENTER_FUNCTION();

  

  file_text = (GtkFileChooserButton*) lookup_widget(GTK_WIDGET(pnee_pref),
						    "rep_choose_but");
  
  my_file = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(file_text)); 
      
  
  if (my_file!=NULL)
    {
      xnee_set_data_name (xd, my_file);
    }
  
  xnee_set_replayer(xd);
  xnee_unset_sync (xd);
  xnee_prepare(xd);

  XNEE_VERBOSE_LEAVE_FUNCTION();
  return 0;
}


int
display_about_dialog(void)
{
  static GtkWidget *pnee_about = NULL ;
  XNEE_VERBOSE_ENTER_FUNCTION();

  if ( pnee_about == NULL)
    {
      pnee_about = create_pnee_about();
    }

  gtk_widget_show (pnee_about);


  XNEE_VERBOSE_LEAVE_FUNCTION();
  return 0;
}

void
pnee_start_recording(void *data)
{
  int ret ;

  pnee_set_recording();
  pnee_set_update_no_action();

  xnee_set_events_recorded(xd,0);
  fprintf(stderr, " ---> recording %d events\n", 
	  xnee_get_events_max(xd));

  fprintf(stderr, " --- prepare \n");
  pnee_prepare_record();
  fprintf(stderr, " --- prepared \n"); 

  XNEE_VERBOSE_ENTER_FUNCTION();

  fprintf(stderr, " --- start record \n");
  ret=xnee_start(xd);
  fprintf(stderr, " --- stopped record \n");
  
  pnee_set_rep_file (xnee_get_data_name(xd));

  XNEE_VERBOSE_LEAVE_FUNCTION();

  fprintf(stderr, " want to leave recording \n");
  pnee_show_states();
  pnee_set_no_action();
  fprintf(stderr, " <--- recording \n");
  pnee_show_states();
  return 0;
}

int
pnee_start_replaying(void)
{
  int ret;

  pnee_set_replaying();
  pnee_set_update_no_action();

  fprintf(stderr, " ---> replaying \n");
  fprintf(stderr, " ---> replaying %d events\n", 
	  xnee_get_events_max(xd));

  XNEE_VERBOSE_ENTER_FUNCTION();
  pnee_prepare_replay();

  ret=xnee_start(xd);
/*   xnee_close_down(xd);  */
  XNEE_VERBOSE_LEAVE_FUNCTION();
  fprintf(stderr, " want to leave replaying \n");
  pnee_show_states();
  pnee_set_no_action();
  fprintf(stderr, " <--- replaying \n");
  pnee_show_states();
  return 0;
}

int
display_properties_dialog()
{
  XNEE_VERBOSE_ENTER_FUNCTION();
  if (pnee_pref==NULL)
    {
      pnee_pref = create_pnee_pref ();
    }

  gtk_widget_show (pnee_pref);
  XNEE_VERBOSE_LEAVE_FUNCTION();
  return 0;
}

#ifndef FAKED_MAIN
static const BonoboUIVerb myexample_menu_verbs [] = {
        BONOBO_UI_VERB ("Record", pnee_start_recording),
        BONOBO_UI_VERB ("Replay", pnee_start_replaying),
        BONOBO_UI_VERB ("Properties", display_properties_dialog),
        BONOBO_UI_VERB ("About", display_about_dialog),
        BONOBO_UI_VERB_END
};
#endif

static gboolean
on_button_press (GtkWidget      *event_box, 
		 GdkEventButton *event,
		 gpointer        data)
{
  static int window_shown;
  static GtkWidget *window, *box, *image, *label;
  /* Don't react to anything other than the left mouse button;
     return FALSE so the event is passed to the default handler */


  XNEE_VERBOSE_ENTER_FUNCTION();

  if (event->button != 1)
    {
      return FALSE;
    }
  
  if (!window_shown) 
    {
      window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
      box = GTK_BOX (gtk_vbox_new (TRUE, 12));
      gtk_container_add (GTK_CONTAINER (window), box);
      
      image = GTK_IMAGE (gtk_image_new_from_file ("/usr/share/pixmaps/mypicture.png"));
      gtk_box_pack_start (GTK_BOX (box), image, TRUE, TRUE, 12);
      
      label = gtk_label_new ("Hello World");
      gtk_box_pack_start (GTK_BOX (box), label, TRUE, TRUE, 12);
      
      gtk_widget_show_all (window);
    }
  else
    {
      gtk_widget_hide (GTK_WIDGET (window));
    }
  
  window_shown = !window_shown;

  XNEE_VERBOSE_LEAVE_FUNCTION();
  return TRUE;
}

void
pnee_update_progress(pnee_panel_applet *pnee_applet, 
		     double perc)
{
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(pnee_applet->progress), 
				perc);
}


void 
pnee_panel_applet_create (pnee_panel_applet *pnee_applet)
{
    GtkTooltips *tooltips;
    GtkIconInfo *icon_info;
    AtkObject *atk_obj;

    pnee_setup();

#ifdef PNEE_DEBUGGER
  pthread_create (&pnee_applet->xnee_debugger, 
		  NULL, 
		  pnee_debugger, 
		  NULL);
#endif /* PNEE_DEBUGGER */

    

    init_stock_icons(pnee_applet);

    panel_applet_set_flags (pnee_applet->applet, 
			    PANEL_APPLET_EXPAND_MINOR);

    panel_applet_set_background_widget(pnee_applet->applet,
                                       GTK_WIDGET(pnee_applet->applet));

/*     pnee_applet->container = gtk_alignment_new (0.5, 0.5, 0, 0); */
    pnee_applet->container = gtk_vbox_new (FALSE,0);
    gtk_container_add (GTK_CONTAINER (pnee_applet->applet), 
		       pnee_applet->container);

    tooltips = gtk_tooltips_new();

    gtk_tooltips_set_tip(tooltips, GTK_WIDGET(pnee_applet->applet), _("GNU Xnee applet"), NULL);

    atk_obj = gtk_widget_get_accessible (GTK_WIDGET (pnee_applet->applet));
    if (GTK_IS_ACCESSIBLE (atk_obj))
	   atk_object_set_name (atk_obj, _("GNU Xnee panel"));

/*     pnee_applet->size = panel_applet_get_size (pnee_applet->applet); */

/*     pnee_applet->orient = panel_applet_get_orient (pnee_applet->applet); */
    
  
/*    label = gtk_label_new ("Xnee");
    gtk_container_add (GTK_CONTAINER (applet), label); 
*/
  
    gtk_widget_show_all (GTK_WIDGET (pnee_applet->applet));
    
    panel_applet_setup_menu (PANEL_APPLET (pnee_applet->applet),
			     Context_menu_xml,
			     myexample_menu_verbs,
			     pnee_applet->applet);
    
    /* 
    
    panel_applet_setup_menu_from_file (pnee_applet->applet,
                                       DATADIR,
			               "GNOME_GWeatherApplet.xml",
                                       NULL,
			               weather_applet_menu_verbs,
			               pnee_applet);
    
      if (panel_applet_get_locked_down (pnee_applet->applet)) {
      BonoboUIComponent *popup_component;
      
      popup_component = panel_applet_get_popup_component (pnee_applet->applet);
      
      bonobo_ui_component_set_prop (popup_component,
				    "/commands/Props",
				    "hidden", "1",
				    NULL);
    }
    */

    pnee_applet->tooltips = tooltips;

    /* Create the visual stuff */
    pnee_applet->progress = gtk_progress_bar_new ();
    gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(pnee_applet->progress),
				     GTK_PROGRESS_LEFT_TO_RIGHT);
    gtk_widget_set_size_request(pnee_applet->progress, 10, 5);

/*     pnee_applet->label =  gtk_label_new ("."); */

    gtk_container_set_border_width (GTK_CONTAINER (pnee_applet->container), 0);
    pnee_applet->button_box = gtk_hbox_new(FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (pnee_applet->button_box), 0);

    pnee_create_button(pnee_applet,
		       "Start recording" , 
		       "pnee-record.png" , 
		       pnee_rec_pressed  ,
		       0);

    pnee_create_button(pnee_applet,
		       "Start replaying" , 
		       "pnee-replay.png" , 
		       pnee_rep_pressed  ,
		       1);

    pnee_create_button(pnee_applet,
		       "Stop" , 
		       "pnee-stop.png" , 
		       pnee_stop_pressed  ,
		       2);

/*       pnee_applet->rec_image = gtk_image_new_from_file("pnee-record.png"); */

/*     pnee_applet->rep_image = gtk_image_new_from_file("/usr/share/pixmaps/pnee-replay.png"); */

    gtk_box_pack_start (GTK_BOX (pnee_applet->button_box), 
			pnee_applet->rec_button, TRUE, TRUE, 0);

    gtk_box_pack_start (GTK_BOX (pnee_applet->button_box), 
			pnee_applet->stop_button, TRUE, TRUE, 0);

    gtk_box_pack_start (GTK_BOX (pnee_applet->button_box), 
			pnee_applet->rep_button, TRUE, TRUE, 0);

    gtk_box_pack_start (GTK_BOX (pnee_applet->container), 
			pnee_applet->button_box, TRUE, TRUE, 0);

    gtk_box_pack_start (GTK_BOX (pnee_applet->container),   
			  pnee_applet->progress, TRUE, TRUE, 0); 

    /*
    gtk_widget_show (pnee_applet->progress);
    gtk_box_pack_start (GTK_BOX (pnee_applet->box), 
			pnee_applet->progress, FALSE, FALSE, 0);
			gtk_progress_bar_set_text (GTK_PROGRESS_BAR (pnee_applet->progress), 
			       "Xnee Event Recorder and Replayer");
    */
    /*
    gtk_progress_bar_set_ellipsize (GTK_PROGRESS_BAR (pnee_applet->progress), 
				    PANGO_ELLIPSIZE_MIDDLE);
    */
    gtk_widget_show_all (pnee_applet->applet);

    return;
}



#ifndef FAKED_MAIN
static gboolean
pnee_applet_fill (PanelApplet *applet,
		  const gchar *iid,
		  gpointer data)
{
  GtkWidget *label;
  pnee_applet = &pnee_applet_impl;  
  
  XNEE_VERBOSE_ENTER_FUNCTION();
  if (strcmp (iid, "OAFIID:pnee") != 0)
    {
      return FALSE;
    }

  pnee_applet->applet = applet;
  pnee_panel_applet_create (pnee_applet);
  
  XNEE_VERBOSE_LEAVE_FUNCTION();
  return TRUE;
}
#endif

#ifdef FAKED_MAIN
int
read_cmd()
{
  
}

int
main (int argc, char *argv[])
{

  gtk_set_locale ();
  gtk_init (&argc, &argv);


  add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");

  pnee_setup();


  if (pnee_pref==NULL)
    {
      pnee_pref = create_pnee_pref ();
    }
  gtk_widget_show (pnee_pref);
  /*
   * The following code was added by Glade to create one of each component
   * (except popup menus), just so that you see something after building
   * the project. Delete any components that you don't want shown initially.
   */
  gtk_main (); 
  return 0;
}

#else
PANEL_APPLET_BONOBO_FACTORY ("OAFIID:pnee_Factory",
                             PANEL_TYPE_APPLET,
                             "The Xnee Applet",
                             "0",
                             pnee_applet_fill,
                             NULL);
#endif
