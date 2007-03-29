#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#include "libxnee/xnee.h"
#include "libxnee/print_varargs.h"
#include "libxnee/print.h"
#include "libxnee/xnee_setget.h"

#include "pnee_impl.h"

extern xnee_data *xd;
extern GtkWidget *pnee_pref;

static int use_delay = 1 ;
static int use_display = 0 ;
static int use_speed = 0 ;


GtkWidget* fs = NULL ;
gchar     *global_filename;
gchar     *display_string ;
extern xnee_data *xd ;

enum {
  PNEE_FILE_NONE   ,
  PNEE_FILE_RECORD ,
  PNEE_FILE_REPLAY ,
  PNEE_FILE_VERBOSE
} pnee_files;


int        file_choosen = PNEE_FILE_NONE;
extern GtkWidget *my_err_dial;
  

char *
pnee_get_default_filename(void)
{
  char *tmp_dir = NULL;
  char *user_name = NULL;

  const char *default_tmp_dir   = "/tmp/";
  const char *default_user_name = "user";
  const char *file_prefix = "pnee_";
  const char *file_suffix = ".xns";

  char *ret_str;
  int   size = 0 ; 

  XNEE_VERBOSE_ENTER_FUNCTION();

  /* set the dir */
  tmp_dir = getenv("TMPIR");
  if (tmp_dir==NULL)
    {
      tmp_dir= (char *) default_tmp_dir;
    }

  /* set the user name */
  user_name = getlogin();
  if (user_name==NULL)
    {
      user_name= getenv("USER");
    }
  if (user_name==NULL)
    {
      user_name = (char *) default_user_name; 
    }
  
  size = strlen (tmp_dir) + 
    strlen (user_name)    + 
    strlen (file_prefix)  + 
    strlen (file_suffix) ;
  
  ret_str = (char*) malloc (size*sizeof(char));
  if (ret_str==NULL)
    {
      return NULL;
    }

  strncpy (ret_str, tmp_dir, size);
  strncat (ret_str, file_prefix, size - strlen(ret_str));
  strncat (ret_str, user_name, size - strlen(ret_str));
  strncat (ret_str, file_suffix, size - strlen(ret_str));
  return ret_str;
}


char *
pnee_get_default_err_name(void)
{
  char *tmp_dir = NULL;
  char *user_name = NULL;

  const char *default_tmp_dir   = "/tmp/";
  const char *default_user_name = "user";
  const char *file_prefix = "pnee_";
  const char *file_suffix = ".err";

  char *ret_str;
  int   size = 0 ; 

  XNEE_VERBOSE_ENTER_FUNCTION();

  /* set the dir */
  tmp_dir = getenv("TMPIR");
  if (tmp_dir==NULL)
    {
      tmp_dir= (char *) default_tmp_dir;
    }

  /* set the user name */
  user_name = getlogin();
  if (user_name==NULL)
    {
      user_name= getenv("USER");
    }
  if (user_name==NULL)
    {
      user_name = (char *) default_user_name; 
    }
  
  size = strlen (tmp_dir) + 
    strlen (user_name)    + 
    strlen (file_prefix)  + 
    strlen (file_suffix) ;
  
  ret_str = (char*) malloc (size*sizeof(char));
  if (ret_str==NULL)
    {
      return NULL;
    }

  strncpy (ret_str, tmp_dir, size);
  strncat (ret_str, file_prefix, size - strlen(ret_str));
  strncat (ret_str, user_name, size - strlen(ret_str));
  strncat (ret_str, file_suffix, size - strlen(ret_str));
  return ret_str;
}


void 
pnee_set_rec_file(char *filename)
{
  GtkEntry *file_text ; 

  XNEE_VERBOSE_ENTER_FUNCTION();
  
  file_text = (GtkEntry*) lookup_widget(GTK_WIDGET(pnee_applet->pnee_pref),
					"entry3");
  xnee_verbose((xd, " setting filename to gui: %s\n", filename));
  gtk_entry_set_text (file_text, 
		      filename);

  xnee_verbose((xd, " setting filename in xd:  %s\n", filename));
  xnee_set_out_name (xd, filename);

  XNEE_VERBOSE_LEAVE_FUNCTION();
}

void 
pnee_set_rep_file(char *filename)
{
  GtkFileChooserButton *file_text ; 
  XNEE_VERBOSE_ENTER_FUNCTION();


  if (filename!=NULL)
    {
      _IN;
      file_text = (GtkFileChooserButton*) lookup_widget(GTK_WIDGET(pnee_applet->pnee_pref),
							"rep_choose_but");
      _OUT;
      _IN;
      gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(file_text),  
				    filename); 
      _OUT;
      
      xnee_verbose((xd, " setting date name to:%s\n", filename));
      xnee_set_data_name (xd, filename);
    }

  XNEE_VERBOSE_LEAVE_FUNCTION();
}



void 
pnee_set_err_file(char *filename)
{
  GtkFileChooserButton *file_text ; 


  XNEE_VERBOSE_ENTER_FUNCTION();

  file_text = (GtkEntry*) lookup_widget(GTK_WIDGET(pnee_applet->pnee_pref),
					"log_entry");

  if (file_text!=NULL)
    {
      xnee_verbose((xd, " setting filename to gui: %s\n", filename));
      gtk_entry_set_text (file_text, 
			  filename);

      xnee_verbose((xd, " setting (err) filename in xd:  %s\n", filename));
      xnee_set_err_name (xd, filename); 
    }
  else
    {
      xnee_verbose((xd, "Failed to get text entry for ERROR\n"));
    }

  XNEE_VERBOSE_LEAVE_FUNCTION();
}



void
on_setting_ok_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  XNEE_VERBOSE_ENTER_FUNCTION();
  gtk_widget_hide (pnee_applet->pnee_pref);
  XNEE_VERBOSE_LEAVE_FUNCTION();
}


void
on_verbose_check_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  XNEE_VERBOSE_ENTER_FUNCTION();
  if (gtk_toggle_button_get_active(togglebutton))
    {
      xnee_set_verbose(xd);
      XNEE_VERBOSE_IN_FUNCTION("Setting verbose");
    }
  else
    {
      XNEE_VERBOSE_IN_FUNCTION("Unsetting verbose");
      
      xnee_unset_verbose(xd);
    }
  XNEE_VERBOSE_LEAVE_FUNCTION();
}


void
on_display_check_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  XNEE_VERBOSE_ENTER_FUNCTION();

  if (gtk_toggle_button_get_active(togglebutton))
    {
      xnee_set_display_name(xd, display_string);
      XNEE_VERBOSE_IN_FUNCTION("Setting display");
      use_display = 1 ; 
    }
  else
    {
      xnee_set_display_name(xd, NULL);
      XNEE_VERBOSE_IN_FUNCTION("Unsetting display (usiong NULL)");
      use_display = 0 ; 
    }

  XNEE_VERBOSE_LEAVE_FUNCTION();
}




void
pnee_set_interval (GtkToggleButton *togglebutton,
		   gpointer         user_data)
{
  gint delay = 0 ; 
  GtkWidget   *window=NULL;

  XNEE_VERBOSE_ENTER_FUNCTION();
  if ( use_delay )
    {
      if (user_data)
	{
	  window = lookup_widget(user_data, "spinbutton2");
	}
      if (window!=NULL)
	{
	  delay = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(window));
	}
    }
  else
    {
      delay = 0 ;
    }

  xnee_set_interval (xd, (int)delay) ;
  xnee_verbose((xd, " -- : setting delay %d\n", delay));
  xnee_verbose((xd, "=================== Setting delay=%d\n", delay));
  XNEE_VERBOSE_LEAVE_FUNCTION();
  return ;
}


void
pnee_set_events_to_record (GtkToggleButton *togglebutton,
			   gpointer         user_data)
{
  gint nr = 0 ; 
  GtkWidget   *window=NULL;

  XNEE_VERBOSE_ENTER_FUNCTION();
  if (user_data)
    {
      window = lookup_widget(user_data, "spinbutton3");
    }
  if (window)
    {
      nr = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(window));
      xnee_set_events_max (xd, nr) ;
      xnee_verbose((xd, " -- : setting events to record: %d\n", nr));
    }
  XNEE_VERBOSE_LEAVE_FUNCTION();
  return ;
}

void
pnee_set_speed (GtkToggleButton *togglebutton,
		gpointer         user_data)
{
  gint speed = 0 ; 
  GtkWidget *window=NULL;

  XNEE_VERBOSE_ENTER_FUNCTION();
  if (user_data)
    {
      window = lookup_widget(user_data, "spinbutton1");
    }
  if (window)
    {
      speed = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(window));
      xnee_set_replay_speed (xd, speed) ;
      xnee_verbose((xd, " -- : setting replay speed: %d\n", speed));
    }
  XNEE_VERBOSE_LEAVE_FUNCTION();
  return ;
}


void
on_delay_check_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  XNEE_VERBOSE_ENTER_FUNCTION();

  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton)))
    {
      use_delay = 1;
      pnee_set_interval(NULL, user_data);
    }
  else
    {
      xnee_verbose((xd, " --on_delay_check_toggled: setting default 0\n"));
      use_delay = 0;
      xnee_verbose((xd, "*****  346 delay=%d", 0));
      xnee_set_interval (xd, 0) ;
    }

  XNEE_VERBOSE_LEAVE_FUNCTION();
  return;
}

void
on_speed_check_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  XNEE_VERBOSE_ENTER_FUNCTION();

  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton)))
    {
      use_speed = 1;
      pnee_set_speed(togglebutton, user_data);
    }
  else
    {
      xnee_verbose((xd, " --on_delay_check_toggled: setting default speed 100 \n"));
      use_speed = 0;
      xnee_set_replay_speed (xd, 100) ;
    }

  XNEE_VERBOSE_LEAVE_FUNCTION();
}


void
on_feedback_button_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  XNEE_VERBOSE_ENTER_FUNCTION();
  XNEE_VERBOSE_LEAVE_FUNCTION();

}


void
on_faked_rec_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
  XNEE_VERBOSE_ENTER_FUNCTION();
  pnee_rec_pressed();
  XNEE_VERBOSE_LEAVE_FUNCTION();
}


void
on_faked_rep_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
  XNEE_VERBOSE_ENTER_FUNCTION();
  pnee_rep_pressed();
  XNEE_VERBOSE_LEAVE_FUNCTION();
}


void
on_spinbutton2_change_value            (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  XNEE_VERBOSE_ENTER_FUNCTION();
  pnee_set_interval(NULL, user_data);
  XNEE_VERBOSE_LEAVE_FUNCTION();

}


void
on_spinbutton3_value_changed           (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  XNEE_VERBOSE_ENTER_FUNCTION();
  pnee_set_events_to_record(NULL, user_data);
  XNEE_VERBOSE_LEAVE_FUNCTION();
}


void
on_spinbutton1_value_changed           (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  XNEE_VERBOSE_ENTER_FUNCTION();
  pnee_set_speed(NULL, user_data);
  XNEE_VERBOSE_LEAVE_FUNCTION();
}


void
on_rec_file_button_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  XNEE_VERBOSE_ENTER_FUNCTION();
  gtk_file_chooser_set_action (GTK_FILE_CHOOSER(fs),GTK_FILE_CHOOSER_ACTION_SAVE); 
  gtk_widget_show (fs);
  file_choosen=PNEE_FILE_RECORD;
  XNEE_VERBOSE_LEAVE_FUNCTION();
}


void
on_rep_file_button_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  XNEE_VERBOSE_ENTER_FUNCTION();
  gtk_file_chooser_set_action (GTK_FILE_CHOOSER(fs),GTK_FILE_CHOOSER_ACTION_OPEN); 
  gtk_widget_show (fs);
  file_choosen=PNEE_FILE_REPLAY;
  XNEE_VERBOSE_LEAVE_FUNCTION();
}


void
on_f_cancel_button_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  XNEE_VERBOSE_ENTER_FUNCTION();
  gtk_widget_hide (fs);
  XNEE_VERBOSE_LEAVE_FUNCTION();
  file_choosen=PNEE_FILE_NONE;
}


void
on_f_ok_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  XNEE_VERBOSE_ENTER_FUNCTION();  
  gtk_widget_hide (fs);

  global_filename = strdup(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fs)));
  if ( file_choosen == PNEE_FILE_RECORD )
    {
      XNEE_VERBOSE_IN_FUNCTION("got rec file ");
      xnee_verbose((xd, "rec file: %s\n", GCHAR_TO_CHAR(global_filename)));
      xnee_set_out_name (xd, GCHAR_TO_CHAR(global_filename));
      pnee_set_rec_file (GCHAR_TO_CHAR(global_filename));
    }
  else if ( file_choosen == PNEE_FILE_REPLAY )
    {
      XNEE_VERBOSE_IN_FUNCTION("got rep file  ");
      xnee_set_data_name(xd, GCHAR_TO_CHAR(global_filename));
      pnee_set_rep_file (GCHAR_TO_CHAR(global_filename));
    }
  else if ( file_choosen == PNEE_FILE_VERBOSE )
    {
      XNEE_VERBOSE_IN_FUNCTION("got verbose file ");
      xnee_set_err_name(xd, GCHAR_TO_CHAR(global_filename));

      pnee_set_err_file (GCHAR_TO_CHAR(global_filename));
      xnee_open_err_file(xd);
    }
  else
    {
      XNEE_VERBOSE_IN_FUNCTION("que?? ");
    }

  XNEE_VERBOSE_LEAVE_FUNCTION();
}


void
on_entry2_editing_done                 (GtkEditable     *editable,
                                        gpointer         user_data)
{
  /* replay */
  XNEE_VERBOSE_ENTER_FUNCTION();

  global_filename = GCHAR_TO_CHAR(gtk_entry_get_text (GTK_ENTRY(user_data)));
  xnee_verbose((xd, "replay file changed to: %s\n", global_filename));
  xnee_set_data_name (xd, global_filename);

  XNEE_VERBOSE_LEAVE_FUNCTION();

}


void
on_entry3_editing_done                 (GtkEditable     *editable,
                                        gpointer         user_data)
{
  /* record */
  XNEE_VERBOSE_ENTER_FUNCTION();

  global_filename = GCHAR_TO_CHAR(gtk_entry_get_text (GTK_ENTRY(user_data)));
  xnee_verbose((xd, "record file changed to: %s\n", global_filename));
  xnee_set_out_name (xd, global_filename);

  XNEE_VERBOSE_LEAVE_FUNCTION();

}


void
on_entry4_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
  XNEE_VERBOSE_ENTER_FUNCTION();
  display_string = GCHAR_TO_CHAR(gtk_entry_get_text (GTK_ENTRY(user_data)));
  xnee_verbose((xd, "display: %s\n", display_string));

  xnee_set_display_name(xd, display_string);

  XNEE_VERBOSE_LEAVE_FUNCTION();
}


void
on_verbose_f_sel_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  XNEE_VERBOSE_ENTER_FUNCTION();
  file_choosen=PNEE_FILE_VERBOSE;
  gtk_widget_show (fs);
  XNEE_VERBOSE_LEAVE_FUNCTION();
}


void
on_use_display_ok_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_entry1_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
  char * my_string;
  XNEE_VERBOSE_ENTER_FUNCTION();

  my_string = GCHAR_TO_CHAR(gtk_entry_get_text (GTK_ENTRY(user_data)));
  xnee_verbose((xd, "verbose file: %s\n", my_string));

  xnee_set_err_name(xd, my_string);

  XNEE_VERBOSE_LEAVE_FUNCTION();

}


void
on_entry5_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
  char * my_key;
  XNEE_VERBOSE_ENTER_FUNCTION();
  my_key = GCHAR_TO_CHAR(gtk_entry_get_text (GTK_ENTRY(user_data)));
  xnee_verbose((xd, "stop file: %s\n", my_key));

  xnee_set_key (xd, XNEE_GRAB_STOP, my_key);

  XNEE_VERBOSE_LEAVE_FUNCTION();

}


void
on_f_c_button_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  XNEE_VERBOSE_ENTER_FUNCTION();
  gtk_widget_hide (fs);
  XNEE_VERBOSE_LEAVE_FUNCTION();
}


void
on_f_open_button_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  gchar * my_file;
  XNEE_VERBOSE_ENTER_FUNCTION();

  gtk_widget_hide (fs);
  my_file = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(fs));

  XNEE_VERBOSE_IN_FUNCTION(my_file);

  on_f_ok_button_clicked(NULL,NULL);

  XNEE_VERBOSE_LEAVE_FUNCTION();
}


void
on_rec_chooser_but_file_activated      (GtkFileChooser  *filechooser,
                                        gpointer         user_data)
{
  XNEE_VERBOSE_ENTER_FUNCTION();
  gtk_widget_hide (fs);
  XNEE_VERBOSE_LEAVE_FUNCTION();
}


void
on_rep_choose_but_file_activated       (GtkFileChooser  *filechooser,
                                        gpointer         user_data)
{
  gchar * my_file;
  XNEE_VERBOSE_ENTER_FUNCTION();

  my_file = gtk_file_chooser_get_filename (user_data);
  
  if (my_file!=NULL)
    {
      xnee_set_data_name (xd, my_file);
    }

  XNEE_VERBOSE_LEAVE_FUNCTION();
}



void
pnee_stop_pressed(void)
{
  pthread_create (&pnee_applet->xnee_thread, 
		  NULL, 
		  pnee_stop_pressed_impl, 
		  pnee_applet);
  return;
}



void 
pnee_rec_pressed(void)
{

  sem_wait (&pnee_applet->action_mutex);
  
  if (!pnee_is_ready(pnee_applet))
    {
      
      sem_post (&pnee_applet->action_mutex);
      return;
    }


  pnee_set_recording(pnee_applet);

  pnee_2xnee_update_all();
  
  sem_post (&pnee_applet->action_mutex);

  pthread_create (&pnee_applet->xnee_thread, 
		  NULL, 
		  pnee_start_recording, 
		  pnee_applet);
  return;
}

void
pnee_rep_pressed(void)
{
  sem_wait (&pnee_applet->action_mutex);
  
  if (!pnee_is_ready(pnee_applet))
    {
      
      sem_post (&pnee_applet->action_mutex);
      return;
    }

  pnee_set_replaying(pnee_applet);

  pnee_2xnee_update_all();
  
  sem_post (&pnee_applet->action_mutex);

  pthread_create (&pnee_applet->xnee_thread, 
		  NULL, 
		  pnee_start_replaying, 
		  pnee_applet);
  return;
}


void
on_log_file_button_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  XNEE_VERBOSE_ENTER_FUNCTION();

  gtk_file_chooser_set_action (GTK_FILE_CHOOSER(fs),
			       GTK_FILE_CHOOSER_ACTION_SAVE); 
  gtk_widget_show (fs);
  file_choosen=PNEE_FILE_VERBOSE;

  XNEE_VERBOSE_LEAVE_FUNCTION();
}


void
on_error_ok_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_hide (my_err_dial);
}

