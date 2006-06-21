#include <string.h>

#include <gdk/gdk.h>
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

typedef enum _pnee_button_type
  {
    PNEE_BUTTON_RECORD,
    PNEE_BUTTON_REPLAY,
    PNEE_BUTTON_STOP
  } pnee_button_type;

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


static volatile pnee_actions action_state = PNEE_ACTION_NONE;
static volatile pnee_update_actions update_state = PNEE_UPDATE_NONE;


#define DEBUG_MARK()  fprintf(stderr, "  ---  %s:%d  %s()\n", __FILE__, __LINE__, __func__);

#define _IN  fprintf (stderr, "********** Taking sem (at %d)\n", __LINE__); fflush(stderr); gdk_threads_enter(); fprintf (stderr, "**********  sem taken (at %d)\n", __LINE__); fflush(stderr) 
#define _OUT  fprintf (stderr, "********** Releasing sem\n"); fflush(stderr); gdk_threads_leave()

/*
#define pnee_set_no_action()  _IN; DEBUG_MARK(); action_state=PNEE_ACTION_NONE   ;   DEBUG_MARK();  _OUT
#define pnee_set_no_action_no_thread() _IN; action_state=PNEE_ACTION_NONE ; _OUT
#define pnee_set_recording() _IN; action_state=PNEE_ACTION_RECORD ;    _OUT
#define pnee_set_replaying() _IN; action_state=PNEE_ACTION_REPLAY ;    _OUT
*/
#define pnee_set_no_action()  DEBUG_MARK(); action_state=PNEE_ACTION_NONE 
#define pnee_set_no_action_no_thread() action_state=PNEE_ACTION_NONE 
#define pnee_set_recording()  action_state=PNEE_ACTION_RECORD 
#define pnee_set_replaying()  action_state=PNEE_ACTION_REPLAY 
#define pnee_is_recording()  (action_state==PNEE_ACTION_RECORD)
#define pnee_is_replaying()  (action_state==PNEE_ACTION_REPLAY) 
#define pnee_is_ready()      (action_state==PNEE_ACTION_NONE) 


#define pnee_set_update_no_action() update_state=PNEE_UPDATE_NONE 
#define pnee_set_update_recording() update_state=PNEE_UPDATE_RECORD 
#define pnee_set_update_replaying() update_state=PNEE_UPDATE_REPLAY 

#define PTHREAD_RETURN_VAL void*
#define PTHREAD_RETURN return NULL

#define THREAD_WAIT_READY   { while (action_state!=PNEE_ACTION_NONE) { usleep (1000*50); fprintf(stderr, "twr "); }
#define RETURN_IF_THREAD_BUSY   { if (action_state!=PNEE_ACTION_NONE) { fprintf(stderr, "\n\n ********** thread busy (%d) , leaving\n\n", action_state); return ; } }
 
 
/*
 */
extern gchar *filename;
 
sem_t action_mutex;
 
PTHREAD_RETURN_VAL
pnee_start_recording(void *pnee_applet);
 
PTHREAD_RETURN_VAL
pnee_start_replaying(void *pnee_applet);

PTHREAD_RETURN_VAL
pnee_progress_updater(void *pnee_applet);

int
pnee_update_progress(pnee_panel_applet *pnee_applet, 
                     double perc);

int 
pnee_setup(void);


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

#define pnee_show_states()       \
     fprintf(stderr,       \
	     " pnee status : %d %d  threads: %x %x \n", \
	     action_state, \
	     update_state, \
	     (int)pnee_applet_impl.xnee_thread, \
	     (int)pnee_applet_impl.xnee_updater);


#ifdef PNEE_DEBUGGER
PTHREAD_RETURN_VAL
pnee_debugger(void *data)
{
  while (1)
    {
      fprintf(stderr, "  debugger thread:: ");
      pnee_show_states();
      usleep (1000*1000);
    }
  PTHREAD_RETURN;
}
#endif /* PNEE_DEBUGGER */

PTHREAD_RETURN_VAL
pnee_progress_updater(void *pnee_applet_in)
{
  gint max ;
  gint curr;
  gdouble perc ;
  GtkWidget *wid;
  GdkWindow *win; 

  while (1)
    {
      while ( pnee_is_replaying() || pnee_is_recording()  )
	{
	  if ( pnee_is_replaying() )
	    {
	      pnee_set_update_replaying();
	    }
	  else if ( pnee_is_recording() )
	    {
	      pnee_set_update_recording();
	    }
	  
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
	  
	  usleep(1000*200);
	}
      pnee_set_update_no_action();

      pnee_update_progress(pnee_applet, 0.0);
      wid = pnee_applet->progress;
      if (pnee_applet->progress)
	{
	  win = gtk_widget_get_root_window (wid); 
	}
      if (win!=NULL)
	{
/* 	  gdk_window_process_updates (win, TRUE); */
	}
      usleep(1000*1000);
    }

  PTHREAD_RETURN;
}


PTHREAD_RETURN_VAL
pnee_stop_pressed_impl(void *pnee_applet)
{
  gint ev_max;

  XNEE_VERBOSE_ENTER_FUNCTION();
  
  pnee_setup();
  
  pnee_set_no_action();

  fprintf(stderr, "stop pressed 0\n");
      
  fprintf(stderr, "stop pressed\n");
  fprintf(stderr, "stop pressed  xd=%d\n", (int)xd);

  ev_max = xnee_get_events_max(xd);
  xnee_set_events_recorded (xd, ev_max);

  pnee_update_progress(pnee_applet, 0.0);
  XNEE_VERBOSE_LEAVE_FUNCTION();
  PTHREAD_RETURN;
}

void
pnee_stop_pressed(void)
{
  pthread_create (&pnee_applet->xnee_thread, 
		  NULL, 
		  pnee_stop_pressed_impl, 
		  NULL);
  return;
}


void 
pnee_rec_pressed(void)
{

  sem_wait (&action_mutex);
  fprintf(stderr, " get mutex !!!!!!!!!!!!!!!!!!!!!!!!\n");
  
  if (!pnee_is_ready())
    {
      
      fprintf(stderr, " Skipping create !!!!!!!!!!!!!!!!!!!!!!!!!\n");
      sem_post (&action_mutex);
      return;
    }

  pnee_set_recording();
  sem_post (&action_mutex);

  fprintf(stderr, " Creating record thread !!!!!!!!!!!!!!!!!!!!!!!!\n");


  pnee_update_progress(pnee_applet, 0.0);
  pthread_create (&pnee_applet->xnee_thread, 
		  NULL, 
		  pnee_start_recording, 
		  NULL);

  return;
}

void
pnee_rep_pressed(void)
{

  sem_wait (&action_mutex);
  fprintf(stderr, " get mutex !!!!!!!!!!!!!!!!!!!!!!!!\n");
  
  if (!pnee_is_ready())
    {
      
      fprintf(stderr, " Skipping create !!!!!!!!!!!!!!!!!!!!!!!!!\n");
      sem_post (&action_mutex);
      return;
    }

  pnee_set_replaying();
  sem_post (&action_mutex);

  fprintf(stderr, " Creating replay thread !!!!!!!!!!!!!!!!!!!!!!!!\n");
  pnee_update_progress(pnee_applet, 0.0);
  pthread_create (&pnee_applet->xnee_thread, 
		  NULL, 
		  pnee_start_replaying, 
		  pnee_applet);
  return;
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
register_stock_icons(pnee_panel_applet *pnee_applet_in, 
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
	
        sprintf(file_name, "%s/%s", PNEE_PIXDIR, items[i] );
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
init_stock_icons(pnee_panel_applet *pnee_applet_in)
{
    GtkIconFactory *factory;

    factory = gtk_icon_factory_new();
    gtk_icon_factory_add_default(factory);

    register_stock_icons(pnee_applet, factory);

    pnee_applet->icon_size = gtk_icon_size_register("pnee_icon_size", 5, 5);

    g_object_unref(factory); 
}




void
pnee_create_button(pnee_panel_applet *pnee_applet_in,
		   const gchar *tooltip_str, 
		   const gchar *id, 
		   GCallback    press_callback, 
		   pnee_button_type mode)
{
  GtkWidget *button;	
  GtkWidget *image;	
  GtkTooltips* tooltip;

  /* Button */
  button = gtk_button_new();
  fprintf (stderr, " button %d\n", (int)button);
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

  if (mode == PNEE_BUTTON_RECORD )
    {
      fprintf(stderr, "Adding to rec\n");
      pnee_applet->rec_button=button;
      pnee_applet->rec_image=image;
      pnee_applet->rec_tooltip=tooltip;
    }
  else if (mode == PNEE_BUTTON_REPLAY )
    {
      fprintf(stderr, "Adding to replay\n");
      pnee_applet->rep_button=button;
      pnee_applet->rep_image=image;
      pnee_applet->rep_tooltip=tooltip;
    } 
  else if (mode == PNEE_BUTTON_STOP )
    {
      fprintf(stderr, "Adding to stop\n");
      pnee_applet->stop_button=button;
      pnee_applet->stop_image=image;
      pnee_applet->stop_tooltip=tooltip;
    } 
}


int
pnee_handle_xerr(Display *dpy, XErrorEvent *errevent)
{
  int ret ; 
  /*@out@*/ char tmp[100]="";

  if (errevent->type==0)
    {
       return XNEE_OK; 
    }
  else if (errevent->type==161)
    {
       return XNEE_OK; 
    }

  fprintf (stderr, "Error received:\n");
  fprintf (stderr, "\terror type       %u (0x%x)\n", 
	  errevent->type,errevent->type);
  fprintf (stderr, "\terror code       %u (0x%x)\n", 
	  errevent->error_code,errevent->error_code);
  fprintf (stderr, "\terror code major %u (0x%x)\n", 
	  errevent->request_code,errevent->request_code);
  fprintf (stderr, "\terror code minor %u (0x%x)\n", 
	  errevent->minor_code,errevent->minor_code);
  fprintf (stderr, "\tdisplay          %d (0x%x)\n", 
	  (int)dpy, (unsigned int)dpy);

  ret = XGetErrorText (dpy, 
		       (int) errevent->error_code, 
		       tmp, 
		       100);
  fprintf (stderr, " %s\n", (char *)tmp);
  fprintf (stderr, "This error can be ignored\n");

  usleep(1000*1000);

  return XNEE_OK;
}

int 
pnee_setup(void)
{
  char *default_tmp_file;
  char *default_err_file;
  static int already_setup = 0;

  XNEE_VERBOSE_ENTER_FUNCTION();

  if (already_setup)
    {
      XNEE_VERBOSE_IN_FUNCTION("already_setup");
      XNEE_VERBOSE_LEAVE_FUNCTION();
      return 0;
    }
  already_setup = 1 ;

  /*  Get a new xnee_data structure  */
  xd = xnee_new_xnee_data();

  default_err_file=pnee_get_default_err_name();

  (void) XSetErrorHandler (pnee_handle_xerr);  

  xnee_set_verbose(xd);
  pnee_set_err_file (default_err_file); 
  xnee_open_files(xd);
  xnee_unset_recall_window_pos (xd);
  xnee_set_events_max (xd, 200);
  xnee_set_data_max (xd, -1);
  xnee_set_key (xd, XNEE_GRAB_STOP, "F5");
  xnee_set_key (xd, XNEE_GRAB_PAUSE, "p");
  xnee_set_key (xd, XNEE_GRAB_RESUME, "r");
  xnee_set_interval(xd, 1);

  XNEE_VERBOSE_ENTER_FUNCTION();

  fs = create_filechooserdialog1();


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

  /* init threads   */
  fprintf(stderr, " ****** gdk_thread_init ******\n");
  gdk_threads_init ();
  gdk_threads_leave();


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


void
display_about_dialog(BonoboUIComponent           *component,
		     gpointer                     user_data,
		     const char                  *cname)
{
  static GtkWidget *pnee_about = NULL ;
  XNEE_VERBOSE_ENTER_FUNCTION();

  if ( pnee_about == NULL)
    {
      pnee_about = create_pnee_about();
    }

  gtk_widget_show (pnee_about);


  XNEE_VERBOSE_LEAVE_FUNCTION();
  return ;
}

PTHREAD_RETURN_VAL
pnee_start_recording(void *data)
{
  int ret ;

  pnee_set_recording();
  pnee_set_update_no_action();

  /* HESA REMOVE THE LINE BELOW */
  xnee_print_grabbed_keys(xd);

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
  PTHREAD_RETURN;
}

PTHREAD_RETURN_VAL
pnee_start_replaying(void *pnee_applet)
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
  PTHREAD_RETURN;
}

void
display_properties_dialog(BonoboUIComponent           *component,
			  gpointer                     user_data,
			  const char                  *cname)
{
  XNEE_VERBOSE_ENTER_FUNCTION();
  if (pnee_pref==NULL)
    {
      pnee_pref = create_pnee_pref ();
    }

  gtk_widget_show (pnee_pref);
  XNEE_VERBOSE_LEAVE_FUNCTION();
  return ;
}

#ifndef FAKED_MAIN
static const BonoboUIVerb myexample_menu_verbs [] = 
  {
    /*    BONOBO_UI_VERB ("Record",      pnee_start_recording),
    BONOBO_UI_VERB ("Replay",      pnee_start_replaying),
    */
    BONOBO_UI_VERB ("Properties",  display_properties_dialog),
    BONOBO_UI_VERB ("About",       display_about_dialog),
    BONOBO_UI_VERB_END
  };
#endif


int
pnee_update_progress(pnee_panel_applet *pnee_applet_in, 
		     double perc)
{
  /*
  fprintf(stderr, "Updating to %f  on %u %u  ", 
	  perc, 
	  pnee_applet, 
	  pnee_applet->progress); fflush(stderr);
  */
  if ( (pnee_applet!=0) && (pnee_applet->progress!=0) )
    {
      gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(pnee_applet->progress), 
				    perc);
/*       fprintf(stderr, "OK\n");fflush(stderr); */
    }
  else
    {
      fprintf(stderr, "Update FAILED\n");fflush(stderr);
    }
  return 0;
}


void 
pnee_panel_applet_create (pnee_panel_applet *pnee_applet_in)
{
  GtkTooltips *tooltips;
  AtkObject *atk_obj;
  sem_init(&action_mutex, 0, 1);

  pnee_setup();
    
#ifdef PNEE_DEBUGGER
  pthread_create (&pnee_applet->xnee_debugger, 
		  NULL, 
		  pnee_debugger, 
		  NULL);
#endif /* PNEE_DEBUGGER */

    pthread_create (&pnee_applet->xnee_updater, 
		    NULL, 
		    pnee_progress_updater,
		    pnee_applet);
    

    init_stock_icons(pnee_applet);

    panel_applet_set_flags (pnee_applet->applet, 
			    PANEL_APPLET_EXPAND_MINOR);

    pnee_applet->container = gtk_vbox_new (FALSE,0);
    gtk_container_add (GTK_CONTAINER (pnee_applet->applet), 
		       pnee_applet->container);

    tooltips = gtk_tooltips_new();

    gtk_tooltips_set_tip(tooltips, GTK_WIDGET(pnee_applet->applet), _("GNU Xnee applet"), NULL);

    atk_obj = gtk_widget_get_accessible (GTK_WIDGET (pnee_applet->applet));
    if (GTK_IS_ACCESSIBLE (atk_obj))
	   atk_object_set_name (atk_obj, _("GNU Xnee panel"));

  
    gtk_widget_show_all (GTK_WIDGET (pnee_applet->applet));
    
    panel_applet_setup_menu (PANEL_APPLET (pnee_applet->applet),
			     Context_menu_xml,
			     myexample_menu_verbs,
			     pnee_applet->applet);
    
    pnee_applet->tooltips = tooltips;

    /* Create the visual stuff */
    pnee_applet->progress = gtk_progress_bar_new ();
    gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(pnee_applet->progress),
				     GTK_PROGRESS_LEFT_TO_RIGHT);
    gtk_widget_set_size_request(pnee_applet->progress, 10, 5);

    gtk_container_set_border_width (GTK_CONTAINER (pnee_applet->container), 0);
    pnee_applet->button_box = gtk_hbox_new(FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (pnee_applet->button_box), 0);

    pnee_create_button(pnee_applet,
		       "Start recording" , 
		       "pnee-record.png" , 
		       pnee_rec_pressed  ,
		       PNEE_BUTTON_RECORD);

    pnee_create_button(pnee_applet,
		       "Start replaying" , 
		       "pnee-replay.png" , 
		       pnee_rep_pressed  ,
		       PNEE_BUTTON_REPLAY);

    pnee_create_button(pnee_applet,
		       "Stop" , 
		       "pnee-stop.png" , 
		       pnee_stop_pressed  ,
		       PNEE_BUTTON_STOP);

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

    gtk_widget_show_all (pnee_applet->container);

    return;
}



#ifndef FAKED_MAIN
static gboolean
pnee_applet_fill (PanelApplet *applet,
		  const gchar *iid,
		  gpointer data)
{
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
