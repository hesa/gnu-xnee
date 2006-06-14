#include <string.h>

#include <gtk/gtklabel.h>
#include <gtk/gtkbox.h>
#include <gtk/gtkimage.h>
#include <gtk/gtkimage.h>

/* #define FAKED_MAIN */
#include <panel-applet.h>

#include "xnee_panel.h"
#include "libxnee/xnee.h"
#include "libxnee/xnee_alloc.h"
#include "libxnee/print_varargs.h"
#include "libxnee/print.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_range.h"
#include "libxnee/xnee_session.h"

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"


xnee_data *xd ;
GtkWidget *pnee_pref;
extern GtkWidget* fs;


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

  xnee_set_verbose(xd);
  XNEE_VERBOSE_ENTER_FUNCTION();

  fs = create_filechooserdialog1();


  xnee_set_key (xd, XNEE_GRAB_STOP, "F1");

  /* Set the program name */
  xnee_set_program_name (xd, "pnee");

  /* Set the cli parameters */
  xnee_set_application_parameters (xd, NULL);

  if (pnee_pref==NULL)
    {
      pnee_pref = create_pnee_pref ();
    }

  default_tmp_file=pnee_get_default_filename();

  default_err_file=pnee_get_default_err_name();
  pnee_set_err_file (default_err_file);

  pnee_set_rec_file (default_tmp_file);
  pnee_set_rep_file (default_tmp_file);

  free (default_tmp_file);
  free (default_err_file);
  
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
  xnee_set_events_max (xd, 200);
  xnee_set_data_max (xd, 200);
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

int
pnee_start_recording()
{
  int ret ;

  pnee_prepare_record();

  XNEE_VERBOSE_ENTER_FUNCTION();

  ret=xnee_start(xd);
  
  pnee_set_rep_file (xnee_get_data_name(xd));

  XNEE_VERBOSE_LEAVE_FUNCTION();
  return 0;
}

int
pnee_start_replaying()
{
  int ret;
  XNEE_VERBOSE_ENTER_FUNCTION();
  pnee_prepare_replay();
  ret=xnee_start(xd);
/*   xnee_close_down(xd);  */
  XNEE_VERBOSE_LEAVE_FUNCTION();
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


#ifndef FAKED_MAIN
static gboolean
myexample_applet_fill (PanelApplet *applet,
		       const gchar *iid,
		       gpointer data)
{
  GtkWidget *label;
  
  XNEE_VERBOSE_ENTER_FUNCTION();
  if (strcmp (iid, "OAFIID:pnee") != 0)
    {
      return FALSE;
    }
  
  label = gtk_label_new ("Xnee");
  gtk_container_add (GTK_CONTAINER (applet), label);
  
  gtk_widget_show_all (GTK_WIDGET (applet));
  
  panel_applet_setup_menu (PANEL_APPLET (applet),
			   Context_menu_xml,
			   myexample_menu_verbs,
			   applet);
  
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
                             myexample_applet_fill,
                             NULL);
#endif
