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
extern  GtkWidget   *ext_gnee_window;




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

  if (fs==NULL)
    fs = create_fileselection1 ();
  file_choosen = CHOOSE_XNEE_SESSION_FILE;
  file_action=FILE_TO_OPEN;

  gtk_widget_show (fs);
}

int
write_session_file()
{
  if (fs==NULL)
    fs = create_fileselection1 ();
  file_choosen = CHOOSE_XNEE_SESSION_FILE;
  file_action=FILE_TO_SAVE;

  gtk_widget_show (fs);
}

int
read_project_file()
{
  if (fs==NULL)
    fs = create_fileselection1 ();
  file_choosen = CHOOSE_XNEE_PROJECT_FILE;
  file_action=FILE_TO_OPEN;

  gtk_widget_show (fs);
}

int
write_project_file()
{
  if (fs==NULL)
    fs = create_fileselection1 ();
  file_choosen = CHOOSE_XNEE_PROJECT_FILE;
  file_action=FILE_TO_SAVE;

  gtk_widget_show (fs);
}


int get_type(GtkWidget* combo_label)
{
    int   type;
    gchar *selection;

    selection = gtk_editable_get_chars
        (GTK_EDITABLE(combo_label), 0, -1);

    if (selection != 0)
    {
        if (strcmp(selection, "Events") == 0)
        {
            type = XNEE_EVENT;
            g_print("type is XNEE_DELIVERED_EVENT\n");
        }
        else if (strcmp(selection, "Replies") == 0)
        {
            type = XNEE_REPLY;
            g_print("type is XNEE_REPLY\n");
        }
        else if (strcmp(selection, "Requests") == 0)
        {
            type = XNEE_REQUEST;
            g_print("type is XNEE_REQUEST\n");
        }
        else if (strcmp(selection, "Errors") == 0)
        {
            type = XNEE_ERROR;
            g_print("type is XNEE_ERROR\n");
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

    source_list      = lookup_widget(GTK_WIDGET(user_data),
                                     "exclude_list");
    destination_list = lookup_widget(GTK_WIDGET(user_data),
                                     "include_list");
    combo_label      = lookup_widget(GTK_WIDGET(user_data),
                                     "combo_label1");

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

    source_list      = lookup_widget(GTK_WIDGET(user_data),
                                     "include_list");
    destination_list = lookup_widget(GTK_WIDGET(user_data),
                                     "exclude_list");
    combo_label      = lookup_widget(GTK_WIDGET(user_data),
                                     "combo_label1");

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

    source_list      = lookup_widget(GTK_WIDGET(user_data),
                                     "exclude_list");
    destination_list = lookup_widget(GTK_WIDGET(user_data),
                                     "include_list");
    combo_label      = lookup_widget(GTK_WIDGET(user_data),
                                     "combo_label1");

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

    source_list      = lookup_widget(GTK_WIDGET(user_data),
                                     "include_list");
    destination_list = lookup_widget(GTK_WIDGET(user_data),
                                     "exclude_list");
    combo_label      = lookup_widget(GTK_WIDGET(user_data),
                                     "combo_label1");

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
  
  source_list      = lookup_widget(GTK_WIDGET(user_data),
				   "include_list");
  destination_list = lookup_widget(GTK_WIDGET(user_data),
				   "exclude_list");
  combo_label      = lookup_widget(GTK_WIDGET(user_data),
				   "combo_label1");
  
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

    exclude_list = lookup_widget(GTK_WIDGET(user_data),
                                 "exclude_list");
    include_list = lookup_widget(GTK_WIDGET(user_data),
                                 "include_list");

    combo_label = lookup_widget(GTK_WIDGET(user_data),
                                "combo_label1");
    selection = gtk_editable_get_chars
        (GTK_EDITABLE(combo_label), 0, -1);

    if (selection != 0)
    {
        if (strcmp(selection, "Events") == 0)
        {
            exclude_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), "exclude_event_store"));
            include_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), "include_event_store"));
            
        }
        else if (strcmp(selection, "Replies") == 0)
        {
            exclude_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), "exclude_reply_store"));
            include_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), "include_reply_store"));
            
        }
        else if (strcmp(selection, "Requests") == 0)
        {
            exclude_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), "exclude_request_store"));
            include_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), "include_request_store"));
            
        }
        else if (strcmp(selection, "Errors") == 0)
        {
            exclude_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), "exclude_error_store"));
            include_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), "include_error_store"));
            
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
  write_project_file();
}


void
on_open1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  read_project_file();
}


void
on_save1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  gx_save();
}


void
on_save_as1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  write_project_file();
}


void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  printf ("quit1\n");

}


void
on_cut1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  printf ("cut1\n");

}


void
on_copy1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  printf ("on_copy1\n");

}


void
on_paste1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  printf ("on_paste1\n");

}


void
on_delete1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  printf ("on_delete1\n");
}


void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  GtkWidget          *about_box;

  printf ("on_about1_activate 1\n");
  about_box      = lookup_widget(GTK_WIDGET(user_data),
				 "about_window");

  printf ("on_about1_activate 1\n");
  if (about_box==NULL)
    about_box = create_about_window();
  printf ("on_about1_activate 1\n");

  gtk_widget_show (about_box);
}


void
on_log_window_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  printf ("on_log11\n");
}


void
on_record                              (GtkButton       *button,
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
	  gx_set_variable_data(ext_xd, ext_gx);
	  
	  gx_start_recording(ext_xd);
        }
      
    }

    gtk_widget_show_all(window);
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
	  gx_set_variable_data(ext_xd, ext_gx);

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
	gx_set_speed (ext_xd, speed); 
      }
}


void
on_wait_spinbutton_value_changed       (GtkEditable     *editable,
                                        gpointer         user_data)
{
  printf (" 4444444\n");
  
}


void
on_wait_checkbox_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  gint speed = 0 ; 
  
  use_delay = gtk_toggle_button_get_active(togglebutton);

  if (ext_xd != NULL)
    {
      if (use_delay)
	{
	  printf ("using delay... setting to %d\n", delay);
	  gx_set_interval (ext_xd, delay); 
	}
      else
	{
	  printf ("NOT using delay... setting to %d\n", 0);
	  gx_set_interval (ext_xd, 0); 
	}
    }
}


void
on_wait_button_value_change            (GtkEditable     *editable,
                                        gpointer         user_data)
{
  printf ("on_wb\n");
}


void
on_wait_spinbutton_value_change        (GtkEditable     *editable,
                                        gpointer         user_data)
{

}


void
on_wait_spinbutto_change_value         (GtkEditable     *editable,
                                        gpointer         user_data)
{

}


void
on_wait_spinbutton_change_value        (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
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
  if (ext_xd != NULL)
    {
      gx_set_events_max(ext_xd,gtk_spin_button_get_value_as_int(spinbutton)); 
    }
}


void
on_spinbutton4_change_value            (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  if (ext_xd != NULL)
    {
      gx_set_data_max(ext_xd,gtk_spin_button_get_value_as_int(spinbutton)); 
    }

}


void
on_spinbutton6_change_value            (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
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

  if (filename!=NULL)
    free (filename);

  filename = strdup(gtk_file_selection_get_filename(GTK_FILE_SELECTION(fs)));
  printf ("filename = %s\n", filename);

  switch (file_choosen)
    {
    case CHOOSE_XNEE_PROJECT_FILE:
      printf ("project file: %s\n", filename);
      if ( file_action  == FILE_TO_SAVE )
	{
	  printf (" SAVE\n");
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
	  printf (" OPEN\n");
	  file = fopen (filename, "rw");
	  
	  ret = gx_set_rc_byname (ext_xd, filename);
	  g_print (" ret=%d\n", ret);

	  if ( gx_get_rc_file (ext_xd) != NULL) 
	    {
	      gx_add_resource (ext_xd );
	    }
	  else
	    g_print (" Could not open project file %s\n", filename);


	  fclose (file);
	  file_choosen = 0;
	  file_action  = 0;

	  gx_set_xd_settings();
	  
	  gx_set_xd_settings();
	}
      else 
	{
	  printf (" DEFAULT\n");
	}
      break;
    case CHOOSE_XNEE_SESSION_FILE:
      printf ("session file: %s\n", filename);
      if ( file_action  == FILE_TO_SAVE )
	{
	  file_text = (GtkEntry*) lookup_widget(GTK_WIDGET(ext_gnee_window),
						    "rec_file_text");
	  
	  gtk_entry_set_text (file_text, 
			      filename);
	  xnee_set_out_byname (ext_xd, filename);
	}
      else if ( file_action  == FILE_TO_OPEN)
	{
	  printf (" read from it\n");

	  file_text = (GtkEntry*) lookup_widget(GTK_WIDGET(ext_gnee_window),
						    "rep_file_text");
	  
	  gtk_entry_set_text (file_text, 
			      filename);
	  
	  xnee_set_data_name_byname(ext_xd, filename);
	}
      
      file_choosen=0;
      break;
    case 0:
    default:
      printf ("undef\n");
      break;
    }
  gtk_widget_hide(fs);
}


void
on_open_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  read_project_file();
}


void
on_cancel_button1_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_hide(fs);
}


void
on_sfile_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
}


void
on_save_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  if (ext_xd != NULL)
    {
      FILE *file;
      char *fname = gx_get_rc_name(ext_xd);

      if (fname == NULL)
	{
	  printf ("Implement me... no file to save\n");
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

  printf ("on_about1 \n" );
  about_box      = lookup_widget(GTK_WIDGET(user_data),
				 "about_window");
  
  printf ("on_about1 %d \n", (int) about_box);

  if (about_box==NULL)
    about_box = create_about_window();

  gtk_widget_hide (about_box);
  
}



static void 
combo_change_helper(gpointer gw, 
		    char *combo_name, 
		    int mod_or_key, 
		    int grab_action)
{
  gchar *k_selection;
  gchar *m_selection;
  GtkWidget          *m_combo;
  GtkWidget          *k_combo;
  char  m_combo_name[50] ;
  char  k_combo_name[50] ;


  if ( (m_combo==NULL) || (m_combo==0) )
    return ;

  
  
  switch (grab_action)
    {
    case XNEE_GRAB_STOP:
      GNEE_DEBUG((" stop \n"));
      strcpy (m_combo_name, "stop_m_combo");
      strcpy (k_combo_name, "stop_k_combo");
      break;
    case XNEE_GRAB_PAUSE:
      GNEE_DEBUG((" pause \n"));
      strcpy (m_combo_name, "pause_m_combo");
      strcpy (k_combo_name, "pause_k_combo");
      break;
    case XNEE_GRAB_RESUME:
      GNEE_DEBUG((" resume \n"));
      strcpy (m_combo_name, "resume_m_combo");
      strcpy (k_combo_name, "resume_k_combo");
      break;
    case XNEE_GRAB_INSERT:
      GNEE_DEBUG((" insert \n"));
      strcpy (m_combo_name, "mark_m_combo");
      strcpy (k_combo_name, "mark_k_combo");
      break;
    case XNEE_GRAB_EXEC:
      GNEE_DEBUG((" exec \n"));
      strcpy (m_combo_name, "exec_m_combo");
      strcpy (k_combo_name, "exec_k_combo");
      break;
    default:
      strcpy (m_combo_name, "");
      strcpy (k_combo_name, "");
    }

  if (strlen (m_combo_name) != 0)
    {
      k_combo      = lookup_widget(GTK_WIDGET(gw),
				   k_combo_name);
      m_combo      = lookup_widget(GTK_WIDGET(gw),
				   m_combo_name);

      m_selection = gtk_editable_get_chars
	(GTK_EDITABLE(m_combo), 0, -1);
      k_selection = gtk_editable_get_chars
	(GTK_EDITABLE(k_combo), 0, -1);
    }
      
  
  if (strlen(k_selection)!=0)
    {
      if (strcmp (k_selection, "None")==0)
	{
	  ;
	}
      else if (strcmp (m_selection, "None")==0)
	{
	  ;
	}
      else
	{
	  char mod_key[120];
	  strcpy (mod_key, m_selection);
	  strcat (mod_key, ",");
	  strcat (mod_key, k_selection);

	  GNEE_DEBUG(("combo_change_helper: mod/key=%d   type=%.2d  %s \n", 
		      mod_or_key,
		      grab_action,
		      mod_key));
	  
	  
 	  gx_set_km (ext_xd, grab_action, mod_key); 
 	  GNEE_DEBUG(("   ----> %s\n", 
		  gx_get_km (ext_xd, grab_action))); 
	}
    }
}

void
on_stop_m_combo_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
  combo_change_helper(user_data, 
		      "stop_m_combo", 
		      XNEE_GRAB_MODIFIER, 
		      XNEE_GRAB_STOP );  
}


void
on_stop_k_combo_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
  combo_change_helper(user_data, 
		      "stop_k_combo", 
		      XNEE_GRAB_KEY, 
		      XNEE_GRAB_STOP );  
}




void
on_pause_m_combo_changed               (GtkEditable     *editable,
                                        gpointer         user_data)
{
  combo_change_helper(user_data, 
		      "pause_m_combo", 
		      XNEE_GRAB_MODIFIER, 
		      XNEE_GRAB_PAUSE );  

}


void
on_resume_m_combo_changed              (GtkEditable     *editable,
                                        gpointer         user_data)
{
  combo_change_helper(user_data, 
		      "resume_m_combo", 
		      XNEE_GRAB_MODIFIER, 
		      XNEE_GRAB_RESUME );  

}


void
on_mark_m_combo_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
  combo_change_helper(user_data, 
		      "mark_m_combo", 
		      XNEE_GRAB_MODIFIER, 
		      XNEE_GRAB_INSERT );  

}


void
on_exec_m_combo_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
  combo_change_helper(user_data, 
		      "exec_m_combo", 
		      XNEE_GRAB_MODIFIER, 
		      XNEE_GRAB_EXEC );  

}


void
on_pause_k_combo_changed               (GtkEditable     *editable,
                                        gpointer         user_data)
{
  combo_change_helper(user_data, 
		      "pause_k_combo", 
		      XNEE_GRAB_KEY, 
		      XNEE_GRAB_PAUSE );  

}


void
on_resume_k_combo_changed              (GtkEditable     *editable,
                                        gpointer         user_data)
{
  combo_change_helper(user_data, 
		      "resume_k_combo", 
		      XNEE_GRAB_KEY, 
		      XNEE_GRAB_RESUME );  

}


void
on_mark_k_combo_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
  combo_change_helper(user_data, 
		      "mark_k_combo", 
		      XNEE_GRAB_KEY, 
		      XNEE_GRAB_INSERT );  

}


void
on_exec_k_combo_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
  combo_change_helper(user_data, 
		      "exec_k_combo", 
		      XNEE_GRAB_KEY, 
		      XNEE_GRAB_EXEC );  
}


void
on_err_cont_button_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
}


void
on_err_quit_button_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  
}


void
on_cancelbutton1_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
  err_continue_clicked=1;
  err_quit_clicked=0;
  gx_undisplay_errror();
}


void
on_okbutton1_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
}


void
on_rep_menu_open_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  read_session_file();
}




void
on_open_project_file2_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  read_project_file();
}


void
on_set_session_file1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  write_session_file();
}


void
on_checkbutton10_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if (gtk_toggle_button_get_active(togglebutton))
    {
      printf ("use rec disp\n");
      gx_set_using_rec_display(ext_gx);
    }
  else
    {
      printf ("NOT use rec disp\n");
      gx_unset_using_rec_display(ext_gx);
    }
}



void
on_rec_file_sel_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  write_session_file();
}


void
on_rep_disp_cb_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if (gtk_toggle_button_get_active(togglebutton))
    {
      printf ("use rep disp\n");
      gx_set_using_rep_display(ext_gx);
    }
  else
    {
      printf ("NOT use rec disp\n");
      gx_unset_using_rep_display(ext_gx);
    }
}


void
on_rep_disp_text_changed               (GtkEditable     *editable,
                                        gpointer         user_data)
{

}


void
on_button11_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
  read_session_file();
}


void
on_max_thresh_sb_value_changed         (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  if (ext_xd != NULL)
    {
      gx_set_min_threshold(ext_xd,
			   gtk_spin_button_get_value_as_int(spinbutton)); 
    }
}


void
on_min_thresh_sb_value_changed         (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
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
  if (ext_xd != NULL)
    {
      gx_set_events_max(ext_xd,gtk_spin_button_get_value_as_int(spinbutton)); 
    }
}


void
on_spinbutton12_change_value           (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  if (ext_xd != NULL)
    {
      gx_set_data_max(ext_xd,gtk_spin_button_get_value_as_int(spinbutton)); 
    }
}


void
on_spinbutton13_change_value           (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  if (ext_xd != NULL)
    {
      gx_set_time_max(ext_xd,gtk_spin_button_get_value_as_int(spinbutton)); 
    }
}


void
on_spinbutton7_change_value            (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{

}


void
on_speed_spin_change_value             (GtkSpinButton   *spinbutton,
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
on_rec_disp_text_changed               (GtkEditable     *editable,
                                        gpointer         user_data)
{
}


void
on_skip_sync_cb_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if (ext_xd != NULL)
    {
      if (gtk_toggle_button_get_active(togglebutton))
	{
	  printf ("sync\n");
	  gx_set_sync (ext_xd); 
	}
      else
	{
	  printf ("unsync\n");
	  gx_unset_sync (ext_xd); 
	}
    }
}



void
on_force_rep_cb_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if (ext_xd != NULL)
    {
      if (gtk_toggle_button_get_active(togglebutton))
	{
	  printf ("forced mode\n");
	  gx_set_force_replay (ext_xd); 
	}
      else
	{
	  printf ("no more forced mode\n");
	  gx_unset_force_replay (ext_xd); 
	}
    }
}


void
on_okbutton2_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
  err_continue_clicked=0;
  err_quit_clicked=1;
  gx_undisplay_errror();
}


void
on_speed_toggle_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
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
  if (ext_xd != NULL)
    {
      if (gtk_toggle_button_get_active(togglebutton))
	{
	  printf ("xnee formatre\n");
	  gx_set_xnee_printout();
	}
    }
}


void
on_human_format_rb_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  if (ext_xd != NULL)
    {
      if (gtk_toggle_button_get_active(togglebutton))
	{
	  printf ("humane\n");
	  gx_set_human_printout();
	}
    }
}

