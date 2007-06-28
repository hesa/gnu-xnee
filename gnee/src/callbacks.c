/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 2002, 2003, 2004, 2005 Henrik Sandklef
 *                                                                   
 * This program is free software; you can redistribute it and/or     
 * modify it under the terms of the GNU General Public License       
 * as published by the Free Software Foundation; either version 3    
 * of the License, or any later version.                             
 *                                                                   
 *                                                                   
 * This program is distributed in the hope that it will be useful,   
 * but WITHOUT ANY WARRANTY; without even the implied warranty of    
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     
 * GNU General Public License for more details.                      
 *                                                                   
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software       
 * Foundation, Inc., 51 Franklin Street, Boston,            
 * MA  02110-1301, USA.                                              
 ****/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "recordables.h"
#include "gnee_xnee.h"

#include <stdio.h>
#include <libxnee/xnee.h>
#include <libxnee/xnee_setget.h>



static int          use_delay = 0;
static gint         delay = 0 ; 
extern xnee_data   *ext_xd;
extern gnee_xnee   *ext_gx;
extern GtkWidget   *ext_gnee_window;

static int x_offset   = 0;
static int y_offset   = 0;
static int use_offset = 0;

const char *events_string   = "Events";
const char *replies_string  = "Replies";
const char *requests_string = "Requests";
const char *errors_string   = "Errors";
const char *exclude_list_string = "exclude_list";
const char *include_list_string = "include_list";
const char *combo_label1_string = "combo_label1";

const char *stop_k_combo_string   = "stop_k_combo";
const char *pause_k_combo_string  = "pause_k_combo";
const char *resume_k_combo_string = "resume_k_combo";
const char *mark_k_combo_string   = "mark_k_combo";
const char *exec_k_combo_string   = "exec_k_combo";

const char *stop_m_combo_string   = "stop_m_combo";
const char *pause_m_combo_string  = "pause_m_combo";
const char *resume_m_combo_string = "resume_m_combo";
const char *mark_m_combo_string   = "mark_m_combo";
const char *exec_m_combo_string   = "exec_m_combo";

const char *exclude_event_store_string = "exclude_event_store";
const char *include_event_store_string = "include_event_store";

const char *exclude_reply_store_string = "exclude_reply_store";
const char *include_reply_store_string = "include_reply_store";

const char *exclude_request_store_string = "exclude_request_store";
const char *include_request_store_string = "include_request_store";

const char *exclude_error_store_string = "exclude_error_store";
const char *include_error_store_string = "include_error_store";

const char *rec_file_text_string = "rec_file_text"; 
const char *rep_file_text_string = "rep_file_text"; 


GtkWidget* fs;
gchar *filename;
#define  CHOOSE_XNEE_PROJECT_FILE 1
#define  CHOOSE_XNEE_SESSION_FILE 2
#define  FILE_TO_SAVE             1
#define  FILE_TO_OPEN             2
static int          file_choosen = 0;
static int          file_action  = 0;


int err_continue_clicked=0;
int err_quit_clicked=0;



int
read_session_file()
{
  GNEE_DEBUG(("read_session_file()\n"));
  if (fs==NULL)
    {
      fs = create_fileselection1 ();
    }
  file_choosen = CHOOSE_XNEE_SESSION_FILE;
  file_action=FILE_TO_OPEN;

  gtk_widget_show (fs);
  return GNEE_OK;
}

int
write_session_file()
{
  GNEE_DEBUG(("write_session_file()\n"));
  if (fs==NULL)
    {
      fs = create_fileselection1 ();
    }
  file_choosen = CHOOSE_XNEE_SESSION_FILE;
  file_action=FILE_TO_SAVE;

  gtk_widget_show (fs);
  return GNEE_OK;
}

int
read_project_file()
{
  GNEE_DEBUG(("read_project_file()\n"));
  if (fs==NULL)
    {
      fs = create_fileselection1 ();
    }
  file_choosen = CHOOSE_XNEE_PROJECT_FILE;
  file_action=FILE_TO_OPEN;

  gtk_widget_show (fs);
  return GNEE_OK;
}

int
write_project_file()
{
  GNEE_DEBUG(("write_project_file()\n"));
  if (fs==NULL)
    {
      fs = create_fileselection1 ();
    }
  file_choosen = CHOOSE_XNEE_PROJECT_FILE;
  file_action=FILE_TO_SAVE;

  gtk_widget_show (fs);
  return GNEE_OK;
}


int 
get_type(GtkWidget* combo_label)
{
  int   type = 0 ;
  gchar *selection;
  
  GNEE_DEBUG(("int get_type(GtkWidget* combo_label)\n"));
  selection = gtk_editable_get_chars
    (GTK_EDITABLE(combo_label), 0, -1);
  
  if (selection != 0)
    {
      if (strncmp(selection, 
		  events_string, 
		  strlen(events_string) ) == 0)
        {
	  type = XNEE_EVENT;
        }
      else if (strncmp(selection, 
		       replies_string, 
		       strlen(replies_string)) == 0)
        {
	  type = XNEE_REPLY;
        }
      else if (strncmp(selection, 
		       requests_string, 
		       strlen(requests_string)) == 0)
        {
	  type = XNEE_REQUEST;
        }
      else if (strncmp(selection, 
		       errors_string, 
		       strlen(errors_string) ) == 0)
        {
	  type = XNEE_ERROR;
        }
    }
  
  g_free(selection);
  
  return type;
}

void
on_record_include_button_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget          *source_list;
    GtkWidget          *destination_list;
    GtkWidget          *combo_label;

    GNEE_DEBUG(("on_record_include_button_clicked\n"));
    source_list      = lookup_widget(GTK_WIDGET(user_data),
                                     exclude_list_string);
    destination_list = lookup_widget(GTK_WIDGET(user_data),
                                     include_list_string);
    combo_label      = lookup_widget(GTK_WIDGET(user_data),
                                     combo_label1_string);
    
    gnee_recordables_exclude(GTK_TREE_VIEW(source_list), 
                             GTK_TREE_VIEW(destination_list),
                             ext_xd,
                             get_type(combo_label));
}


void
on_record_exclude_button_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget          *source_list;
    GtkWidget          *destination_list;
    GtkWidget          *combo_label;

    GNEE_DEBUG(("on_record_exclude_button_clicked\n"));
    source_list      = lookup_widget(GTK_WIDGET(user_data),
                                     include_list_string);
    destination_list = lookup_widget(GTK_WIDGET(user_data),
                                     exclude_list_string);
    combo_label      = lookup_widget(GTK_WIDGET(user_data),
                                     combo_label1_string);

    gnee_recordables_include(GTK_TREE_VIEW(source_list), 
                             GTK_TREE_VIEW(destination_list),
                             ext_xd,
                             get_type(combo_label));
}


void
on_exclude_list_row_activated          (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{
    GtkWidget          *source_list;
    GtkWidget          *destination_list;
    GtkWidget          *combo_label;

    GNEE_DEBUG(("on_exclude_list_row_activated\n"));
    source_list      = lookup_widget(GTK_WIDGET(user_data),
                                     exclude_list_string);
    destination_list = lookup_widget(GTK_WIDGET(user_data),
                                     include_list_string);
    combo_label      = lookup_widget(GTK_WIDGET(user_data),
                                     combo_label1_string);

    gnee_recordables_include(GTK_TREE_VIEW(source_list), 
                             GTK_TREE_VIEW(destination_list),
                             ext_xd,
                             get_type(combo_label));
}


void
on_include_list_row_activated          (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{
    GtkWidget          *source_list;
    GtkWidget          *destination_list;
    GtkWidget          *combo_label;

    GNEE_DEBUG(("on_include_list_row_activated\n"));
    source_list      = lookup_widget(GTK_WIDGET(user_data),
                                     include_list_string);
    destination_list = lookup_widget(GTK_WIDGET(user_data),
                                     exclude_list_string);
    combo_label      = lookup_widget(GTK_WIDGET(user_data),
                                     combo_label1_string);

    gnee_recordables_exclude(GTK_TREE_VIEW(source_list), 
                             GTK_TREE_VIEW(destination_list),
                             ext_xd,
                             get_type(combo_label));
}

void
fake_on_include_list_row_activated (gpointer user_data, int type)
{
  GtkWidget          *source_list;
  GtkWidget          *destination_list;
  GtkWidget          *combo_label;
  
    GNEE_DEBUG(("fake_on_include_list_row_activated\n"));
  source_list      = lookup_widget(GTK_WIDGET(user_data),
				   include_list_string);
  destination_list = lookup_widget(GTK_WIDGET(user_data),
				   exclude_list_string);
  combo_label      = lookup_widget(GTK_WIDGET(user_data),
				   combo_label1_string);
  
}

void
on_combo_label1_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
    GtkWidget          *exclude_list;
    GtkWidget          *include_list;

    GtkTreeStore       *exclude_store = 0;
    GtkTreeStore       *include_store = 0;

    gchar              *selection;
    GtkWidget          *combo_label;

    GNEE_DEBUG(("on_combo_label1_changed\n"));
    exclude_list = lookup_widget(GTK_WIDGET(user_data),
                                 exclude_list_string);
    include_list = lookup_widget(GTK_WIDGET(user_data),
                                 include_list_string);

    combo_label = lookup_widget(GTK_WIDGET(user_data),
                                combo_label1_string);
    selection = gtk_editable_get_chars
      (GTK_EDITABLE(combo_label), 0, -1);

    if (selection != 0)
    {
        if (strcmp(selection, events_string) == 0)
        {
            exclude_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), exclude_event_store_string));
            include_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), include_event_store_string));
            
        }
        else if (strcmp(selection, replies_string) == 0)
        {
            exclude_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), exclude_reply_store_string));
            include_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), include_reply_store_string));
            
        }
        else if (strcmp(selection, requests_string) == 0)
        {
            exclude_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), exclude_request_store_string));
            include_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), include_request_store_string));
            
        }
        else if (strcmp(selection, errors_string) == 0)
        {
            exclude_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), exclude_error_store_string));
            include_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), include_error_store_string));
            
        }
        
        if (exclude_store != NULL && include_store != NULL)
        {
            gtk_tree_view_set_model(GTK_TREE_VIEW(exclude_list),
                                    GTK_TREE_MODEL(exclude_store));
            gtk_tree_view_set_model(GTK_TREE_VIEW(include_list),
                                    GTK_TREE_MODEL(include_store));
        }
        
        g_free(selection);
    }
}

void
on_new1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_new1_activate\n"));
  write_project_file();
}


void
on_open1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_open1_activate\n"));
  read_project_file();
}


void
on_save1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_save1_activate\n"));
  gx_save();
}


void
on_save_as1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_save_as1\n"));
  write_project_file();
}


void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_quit1_activate\n"));
  gnee_close_down();
}


void
on_cut1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_cut1_activate\n"));
}


void
on_copy1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_copy1_activate\n"));
}


void
on_paste1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_paste1_activate\n"));
}


void
on_delete1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_delete1_activate\n"));
}


void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget          *about_box;
  GtkWidget          *about_label_1;
  char buf[256];
  GNEE_DEBUG(("on_about1_activate\n"));

  about_box      = lookup_widget(GTK_WIDGET(user_data),
				 "gnee_about");

  if (about_box == NULL)
    {
      about_box = create_gnee_about();
    }
  
  if (about_box != NULL)
    {
      about_box = create_gnee_about();
    }
  


  about_label_1 = lookup_widget(GTK_WIDGET(about_box),
				"gnee_label");
  if (about_label_1!=NULL)
    {
      sprintf(buf,"<b>%s %s</b>\n\n",
	      XNEE_GUI, 
	      VERSION);
      strcat(buf, "is the graphical frontend to " PACKAGE "\n\n");
      strcat(buf, PACKAGE " is Free Software released under GPL version 3\n");

      gtk_label_set_markup((GtkLabel*)about_label_1, (gchar*)buf);
    }

  if (about_box!=NULL)
    {
      gtk_widget_show (about_box);
    }
}


void
on_log_window_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_log_window\n"));
}


void
on_record                              (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget   *window;
    char         *str;
    GtkEditable  *disp_edit; 
    
    
    window = lookup_widget(user_data, "gnee_window");

    if (window != NULL)
      {
	gtk_widget_hide(window); 
	while (gtk_events_pending ())
	  {
	    gtk_main_iteration ();
	  }
	
	if (ext_xd != NULL)
	  {
	    gnee_recordable2xd(ext_gnee_window,  
			       "include_event_store" , 
			       XNEE_EVENT); 
	    
	    gx_start_recording(ext_xd);
	  }
	
      }
    gtk_widget_show_all(window); 
    fflush(stdout);
}


void
on_replay                              (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget   *window;

    window = lookup_widget(user_data, "gnee_window");
    
    if (window != NULL)
    {
      gtk_widget_hide(window);
      while (gtk_events_pending ())
	{
	  gtk_main_iteration ();
	}
      
      if (ext_xd != NULL)
        {
	  /* This usleep is to prevent the 
	   * window iconification to disturbe
	   * replay.....
	   */
	  usleep(50*1000);

	  gx_start_replaying(ext_xd);
        }
    }

    gtk_widget_show_all(window);
}


void
on_spinbutton3_change_value            (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
    gint speed = 0 ; 

    speed = gtk_spin_button_get_value_as_int(spinbutton);
    
    if (ext_xd != NULL)
      {
	gx_set_speed (speed); 
      }
}


void
on_wait_spinbutton_value_changed       (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_wait_spinbutton\n"));
}


void
on_wait_checkbox_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  gint speed = 0 ; 
  GNEE_DEBUG(("on_wait_checkbox\n"));
  
  use_delay = gtk_toggle_button_get_active(togglebutton);

  if (ext_xd != NULL)
    {
      if (use_delay)
	{
	  gx_set_interval (ext_xd, delay); 
	}
      else
	{
	  gx_set_interval (ext_xd, 0); 
	}
    }
}


void
on_wait_button_value_change            (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_wait_button\n"));
}


void
on_wait_spinbutton_value_change        (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_wait_spinbutton\n"));
  
}


void
on_wait_spinbutto_change_value         (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_wait_spinbutto\n"));
  
}


void
on_wait_spinbutton_change_value        (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_wait_spinbutton\n"));
  delay = gtk_spin_button_get_value_as_int(spinbutton);
    
  if (ext_xd != NULL)
    {
      if (use_delay)
	{
	  gx_set_interval (ext_xd, delay);
	}
      else
	{
	  gx_set_interval (ext_xd, 0);
	}
    }
}

void
on_verbose_logging_checkbox_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GtkWidget   *window;

  GNEE_DEBUG(("on_verbose_logging_checkbox_toggled\n"));
  if (ext_xd != NULL)
    {
      if (gtk_toggle_button_get_active(togglebutton))
	{
	  gx_set_verbose(ext_xd); 
	}
      else
	{
	  gx_unset_verbose(ext_xd); 
	}
    }

}




void
on_spinbutton5_change_value            (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_spinbutton5_change\n"));
  if (ext_xd != NULL)
    {
      gx_set_events_max(ext_xd,gtk_spin_button_get_value_as_int(spinbutton)); 
    }
}


void
on_spinbutton4_change_value            (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_spinbutton4_change\n"));
  if (ext_xd != NULL)
    {
      gx_set_data_max(ext_xd,gtk_spin_button_get_value_as_int(spinbutton)); 
    }

}


void
on_spinbutton6_change_value            (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_spinbutton6_change\n"));
  if (ext_xd != NULL)
    {
      gx_set_time_max(ext_xd,gtk_spin_button_get_value_as_int(spinbutton)); 
    }

}



void
on_ok_button1_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  FILE *file;
  GtkEntry *file_text ; 

  GNEE_DEBUG(("on_ok_button1_clicked\n"));
  if (filename!=NULL)
    {
      free (filename);
    }

  filename = strdup(gtk_file_selection_get_filename(GTK_FILE_SELECTION(fs)));

  switch (file_choosen)
    {
    case CHOOSE_XNEE_PROJECT_FILE:
      if ( file_action  == FILE_TO_SAVE )
	{
	  gx_set_rc_byname (ext_xd, filename);
	  file = fopen (filename, "w");
	  gx_write_settings_to_file (ext_xd, file) ;
	  fclose (file);
	  file_choosen = 0;
	  file_action  = 0;
	}
      else if ( file_action  == FILE_TO_OPEN)
	{
	  int ret;
	  file = fopen (filename, "rw");
	  xnee_refresh_ranges(ext_xd);
	  gnee_remove_all_recordables();
	  ret = gx_set_rc_byname (ext_xd, filename);

	  if ( gx_get_rc_file (ext_xd) != NULL) 
	    {
 	      gx_add_resource (ext_xd ); 
	    }
	  else
	  {
	      g_print (" Could not open project file %s\n", filename);
	  }

          
	  fclose (file);
	  file_choosen = 0;
	  file_action  = 0;

          /* Make the GUI reflect all settings as read from file */
 	  gx_set_xd_settings(); 
	}
      else 
	{
	  ;
	}
      break;
    case CHOOSE_XNEE_SESSION_FILE:
      if ( file_action  == FILE_TO_SAVE )
	{
	  file_text = (GtkEntry*) lookup_widget(GTK_WIDGET(ext_gnee_window),
						    rec_file_text_string);
	  
	  gtk_entry_set_text (file_text, 
			      filename);
	  xnee_set_out_name (ext_xd, filename);
	}
      else if ( file_action  == FILE_TO_OPEN)
	{

	  file_text = (GtkEntry*) lookup_widget(GTK_WIDGET(ext_gnee_window),
						    rep_file_text_string);
	  
	  gtk_entry_set_text (file_text, 
			      filename);
	  
	  xnee_set_data_name(ext_xd, filename);

          /* Make the GUI reflect all settings as read from file */
 	  gx_set_xd_settings(); 


	}
      
      file_choosen=0;
      break;
    case 0:
    default:
      break;
    }
  gtk_widget_hide(fs);
}


void
on_open_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_open_button\n"));
  read_project_file();
}


void
on_cancel_button1_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_cancel_button1\n"));
  gtk_widget_hide(fs);
}


void
on_sfile_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_sfile_activate\n"));
}


void
on_save_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_save_button\n"));
  if (ext_xd != NULL)
    {
      FILE *file;
      char *fname = gx_get_rc_name(ext_xd);

      if (fname == NULL)
	{
	  write_project_file();
	}
      else
	{
	  file = fopen (fname, "w");
	  gx_write_settings_to_file (ext_xd, file) ;
	  fclose (file);
	}
    }
}


void
on_about_ok_button_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget          *about_box;

  GNEE_DEBUG(("on_about_ok\n"));
  about_box      = lookup_widget(GTK_WIDGET(user_data),
				 "gnee_about");
  

  if (about_box==NULL)
    about_box = create_gnee_about();

  gtk_widget_hide (about_box);
  
}



static void 
combo_change_helper(gpointer gw, 
		    const char *combo_name, 
		    int mod_or_key, 
		    int grab_action)
{
  gchar *k_selection = NULL;
  GtkWidget          *k_combo = NULL;
  #define MOD_KEY_SIZE    128
  #define COMBO_NAME_SIZE 50

  char mod_key[MOD_KEY_SIZE];
  char k_combo_name[COMBO_NAME_SIZE] ;

  GNEE_DEBUG(("combo_change_helper\n"));



  GNEE_DEBUG((" combo .............. \n"));
  
  switch (grab_action)
    {
    case XNEE_GRAB_STOP:
      GNEE_DEBUG((" stop \n"));
      strncpy (k_combo_name, stop_k_combo_string, COMBO_NAME_SIZE);
      break;
    case XNEE_GRAB_PAUSE:
      GNEE_DEBUG((" pause \n"));
      strncpy (k_combo_name, pause_k_combo_string, COMBO_NAME_SIZE);
      break;
    case XNEE_GRAB_RESUME:
      GNEE_DEBUG((" resume \n"));
      strncpy (k_combo_name, resume_k_combo_string, COMBO_NAME_SIZE);
      break;
    case XNEE_GRAB_INSERT:
      GNEE_DEBUG((" insert \n"));
      strncpy (k_combo_name, mark_k_combo_string, COMBO_NAME_SIZE);
      break;
    case XNEE_GRAB_EXEC:
      GNEE_DEBUG((" exec \n"));
      strncpy (k_combo_name, exec_k_combo_string, COMBO_NAME_SIZE);
      break;
    default:
      GNEE_DEBUG(("NO action choosen\n"));
      strncpy (k_combo_name, "", COMBO_NAME_SIZE);
    }

  if (strlen (k_combo_name) != 0)
    {
      k_combo      = lookup_widget(GTK_WIDGET(gw),
				   k_combo_name);
      k_selection = gtk_editable_get_chars
	(GTK_EDITABLE(k_combo), 0, -1);
    }
      
  
  if (strlen(k_selection)!=0)
    {
      if (strcmp (k_selection, "None")==0)
	{
	  GNEE_DEBUG((" combo no 1 \n"));
	}
      else
	{
	  strncpy (mod_key, k_selection, MOD_KEY_SIZE);

	  GNEE_DEBUG(("combo_change_helper: mod/key=%d   type=%.2d  %s \n", 
		      mod_or_key,
		      grab_action,
		      mod_key));
	  
	  
 	  gx_set_key (ext_xd, grab_action, mod_key); 
	}
    }
}

void
on_stop_m_combo_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_stop_m_combo_changed\n"));
  combo_change_helper(user_data, 
		      stop_m_combo_string, 
		      XNEE_GRAB_MODIFIER, 
		      XNEE_GRAB_STOP );  
}


void
on_stop_k_combo_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_stop_k\n"));
  combo_change_helper(user_data, 
		      stop_k_combo_string, 
		      XNEE_GRAB_KEY, 		      XNEE_GRAB_STOP );  
}




void
on_pause_m_combo_changed               (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_pause_m\n"));
  combo_change_helper(user_data, 
		      pause_m_combo_string, 
		      XNEE_GRAB_MODIFIER, 
		      XNEE_GRAB_PAUSE );  

}


void
on_resume_m_combo_changed              (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_resume_m\n"));
  combo_change_helper(user_data, 
		      resume_m_combo_string, 
		      XNEE_GRAB_MODIFIER, 
		      XNEE_GRAB_RESUME );  

}


void
on_mark_m_combo_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_mark_m\n"));
  combo_change_helper(user_data, 
		      mark_m_combo_string, 
		      XNEE_GRAB_MODIFIER, 
		      XNEE_GRAB_INSERT );  

}


void
on_exec_m_combo_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_exec_m\n"));
  combo_change_helper(user_data, 
		      exec_m_combo_string, 
		      XNEE_GRAB_MODIFIER, 
		      XNEE_GRAB_EXEC );  

}


void
on_pause_k_combo_changed               (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_pause_k\n"));
  combo_change_helper(user_data, 
		      pause_k_combo_string, 
		      XNEE_GRAB_KEY, 
		      XNEE_GRAB_PAUSE );  

}


void
on_resume_k_combo_changed              (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_resume_k\n"));
  combo_change_helper(user_data, 
		      resume_k_combo_string, 
		      XNEE_GRAB_KEY, 
		      XNEE_GRAB_RESUME );  

}


void
on_mark_k_combo_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_mark_k\n"));
  combo_change_helper(user_data, 
		      mark_k_combo_string, 
		      XNEE_GRAB_KEY, 
		      XNEE_GRAB_INSERT );  

}


void
on_exec_k_combo_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_exec_k\n"));
  combo_change_helper(user_data, 
		      exec_k_combo_string, 
		      XNEE_GRAB_KEY, 
		      XNEE_GRAB_EXEC );  
}


void
on_err_cont_button_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_err_cont\n"));
}


void
on_err_quit_button_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_err_quit\n"));
}


void
on_cancelbutton1_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_cancelbutton1_clicked\n"));
  err_continue_clicked=1;
  err_quit_clicked=0;
  gx_undisplay_errror();
}


void
on_okbutton1_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_okbutton1_clicked\n"));
}


void
on_rep_menu_open_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    GNEE_DEBUG(("on_rep_menu\n"));
  read_session_file();
}




void
on_open_project_file2_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_open_project\n"));
  read_project_file();
}


void
on_set_session_file1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_set_session\n"));
  write_session_file();
}


void
on_checkbutton10_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_checkbutton10_toggled\n"));

  if (gtk_toggle_button_get_active(togglebutton))
    {
      gx_set_using_rec_display(ext_gx);
    }
  else
    {
      gx_unset_using_rec_display(ext_gx);
    }
}



void
on_rec_file_sel_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_rec_file\n"));
  write_session_file();
}


void
on_rep_disp_cb_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_rep_disp\n"));
  if (gtk_toggle_button_get_active(togglebutton))
    {
      gx_set_using_rep_display(ext_gx);
    }
  else
    {
      gx_unset_using_rep_display(ext_gx);
    }
}


void
on_rep_disp_text_changed               (GtkEditable     *editable,
                                        gpointer         user_data)
{
  
  GNEE_DEBUG(("on_rep_disp\n"));
}


void
on_button11_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_button11_clicked\n"));
  read_session_file();
}


void
on_max_thresh_sb_value_changed         (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_max_thresh\n"));
  if (ext_xd != NULL)
    {
      gx_set_max_threshold(ext_xd,
			   gtk_spin_button_get_value_as_int(spinbutton)); 
    }
}


void
on_min_thresh_sb_value_changed         (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_min_thresh\n"));
  if (ext_xd != NULL)
    {
      gx_set_min_threshold(ext_xd,
			   gtk_spin_button_get_value_as_int(spinbutton)); 
    }
}


void
on_tot_thresh_sb_value_changed         (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_tot_thresh\n"));
  if (ext_xd != NULL)
    {
      gx_set_tot_threshold(ext_xd,
			   gtk_spin_button_get_value_as_int(spinbutton)); 
    }
}


void
on_spinbutton11_change_value           (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_spinbutton11_change\n"));
  if (ext_xd != NULL)
    {
      gx_set_events_max(ext_xd,gtk_spin_button_get_value_as_int(spinbutton)); 
    }
}


void
on_spinbutton12_change_value           (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_spinbutton12_change\n"));
  if (ext_xd != NULL)
    {
      gx_set_data_max(ext_xd,gtk_spin_button_get_value_as_int(spinbutton)); 
    }
}


void
on_spinbutton13_change_value           (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_spinbutton13_change\n"));
  if (ext_xd != NULL)
    {
      gx_set_time_max(ext_xd,gtk_spin_button_get_value_as_int(spinbutton)); 
    }
}


void
on_spinbutton7_change_value            (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_spinbutton7_change\n"));
  
}


void
on_speed_spin_change_value             (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  gint speed = 0 ; 

  GNEE_DEBUG(("on_speed_spin\n"));
  speed = gtk_spin_button_get_value_as_int(spinbutton);
  
  if (ext_xd != NULL)
  {
      gx_set_speed (speed); 
  }
}




void
on_rec_disp_text_changed               (GtkEditable     *editable,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_rec_disp\n"));
}


void
on_skip_sync_cb_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_skip_sync "));
  if (ext_xd != NULL)
    {
      if (gtk_toggle_button_get_active(togglebutton))
      {
	  GNEE_DEBUG(("  SYNC ON"));
	  gx_set_sync (ext_xd); 
	}
      else
	{
	  GNEE_DEBUG(("  SYNC OFF "));
	  gx_unset_sync (ext_xd); 
	}
    }
  GNEE_DEBUG(("     xd says=%d\n", 
	      xnee_get_sync(ext_xd)));
}



void
on_force_rep_cb_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_force_rep\n"));
  if (ext_xd != NULL)
    {
      if (gtk_toggle_button_get_active(togglebutton))
	{
	  gx_set_force_replay (ext_xd); 
	}
      else
	{
	  gx_unset_force_replay (ext_xd); 
	}
    }
}


void
on_okbutton2_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_okbutton2_clicked\n"));
  err_continue_clicked=0;
  err_quit_clicked=1;
  gx_undisplay_errror();
}


void
on_speed_toggle_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_speed_toggle\n"));
  if (ext_xd != NULL)
    {
      if (gtk_toggle_button_get_active(togglebutton))
	{
	  gx_set_use_speed(1); 
	}
      else
	{
	  gx_set_use_speed(0); 
	}
    }

}


void
on_radiobutton1_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_radiobutton1_toggled\n"));
  if (ext_xd != NULL)
    {
      if (gtk_toggle_button_get_active(togglebutton))
	{
	  gx_set_all_clients(); 
	}
    }
}


void
on_radiobutton2_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_radiobutton2_toggled\n"));
  if (ext_xd != NULL)
    {
      if (gtk_toggle_button_get_active(togglebutton))
	{
	  gx_unset_all_clients(); 
	}
    }
}


void
on_xnee_format_rb_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_xnee_format\n"));
  if (ext_xd != NULL)
    {
      if (gtk_toggle_button_get_active(togglebutton))
	{
	  gx_set_xnee_printout();
	}
    }
}


void
on_human_format_rb_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_human_format\n"));
  if (ext_xd != NULL)
    {
      if (gtk_toggle_button_get_active(togglebutton))
	{
	  gx_set_human_printout();
	}
    }
}


void
on_gnee_window_destroy                 (GtkObject       *object,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_gnee_window\n"));
  gnee_close_down();
}


void
on_stderr_fb_b_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_stderr_fb\n"));
  if (ext_xd != NULL)
    {
      if (gtk_toggle_button_get_active(togglebutton))
	{
	  gx_set_stderr_feedback();
	}
    }
}


void
on_no_fb_b_toggled                     (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_no_fb\n"));
  if (ext_xd != NULL)
    {
      if (gtk_toggle_button_get_active(togglebutton))
	{
	  gx_set_no_feedback();
	}
    }
}



void
on_xosd_fb_b_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("on_xosd_fb\n"));
  if (ext_xd != NULL)
    {
      if (gtk_toggle_button_get_active(togglebutton))
	{
	  gx_set_xosd_feedback();
	}
    }

}


void
on_replay_settings_box_map             (GtkWidget       *widget,
                                        gpointer         user_data)
{
/*   printf ("map\n"); */
  ;
}


void
on_record_settings_box_map             (GtkWidget       *widget,
                                        gpointer         user_data)
{
  ;
}


void
on_button5_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget   *window;
    
  if (user_data)
    {
      window = lookup_widget(user_data, "gnee_about");
    }

  if (window)
    {
      gtk_widget_hide (window);
    }
}



void
on_new_win_check_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  int use_recall;
  GNEE_DEBUG(("on_new_win_checkbox\n"));
  
  use_recall = gtk_toggle_button_get_active(togglebutton);

  if (ext_xd != NULL)
    {
      if (use_recall)
	{
	  gx_set_recall_window_pos(ext_xd); 
	}
      else
	{
	  gx_unset_recall_window_pos(ext_xd); 
	}
    }
}



void
on_x_offs_spin_changed                 (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  x_offset = gtk_spin_button_get_value_as_int(spinbutton);
  
  if (ext_xd != NULL)
    {
      if (use_offset)
	{
	  gx_set_replay_offset_x (ext_xd, x_offset);
	}
    }
}


void
on_y_offs_spin_changed                 (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  y_offset = gtk_spin_button_get_value_as_int(spinbutton);
  
  if (ext_xd != NULL)
    {
      if (use_offset)
	{
	  gx_set_replay_offset_y (ext_xd, y_offset);
	}
    }
}


void
on_offset_toggle_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  GNEE_DEBUG(("%s\n", __func__));
  
  use_offset = gtk_toggle_button_get_active(togglebutton);

  if (ext_xd != NULL)
    {
      if (use_offset)
	{
	  gx_set_replay_offset_x(ext_xd, x_offset); 	
	  gx_set_replay_offset_y(ext_xd, y_offset); 
	}
      else
	{
	  gx_set_replay_offset_x(ext_xd, 0);
	  gx_set_replay_offset_y(ext_xd, 0); 
	}
    }
}

