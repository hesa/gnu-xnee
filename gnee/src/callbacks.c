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
GtkWidget* fs;
gchar *filename;
#define  CHOOSE_XNEE_PROJECT_FILE 1
#define  CHOOSE_XNEE_SESSION_FILE 2
#define  FILE_TO_SAVE             1
#define  FILE_TO_OPEN             2
static int          file_choosen = 0;
static int          file_action  = 0;




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
/*     xnee_data          *xd; */

    source_list      = lookup_widget(GTK_WIDGET(user_data),
                                     "include_list");
    destination_list = lookup_widget(GTK_WIDGET(user_data),
                                     "exclude_list");
    combo_label      = lookup_widget(GTK_WIDGET(user_data),
                                     "combo_label1");

/*     xd = (xnee_data*) lookup_widget(GTK_WIDGET(user_data), "xd"); */

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
  if (fs==NULL)
    fs = create_fileselection1 ();
  file_choosen = CHOOSE_XNEE_PROJECT_FILE;
  file_action  = FILE_TO_SAVE;
}


void
on_open1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  
  if (fs==NULL)
    fs = create_fileselection1 ();
  file_choosen = CHOOSE_XNEE_PROJECT_FILE;
  file_action  = FILE_TO_OPEN;

  gtk_widget_show (fs);
  printf ("open  ... OK!\n");
}


void
on_save1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  if (ext_xd != NULL)
    {
      FILE *file;
      char *fname = xnee_get_rc_name(ext_xd);

      if (fname == NULL)
	{
	  printf ("Implement me... no file to save\n");
	}
      else
	{
	  file = fopen (fname, "w");
	  xnee_write_settings_to_file (ext_xd, file) ;
	  fclose (file);
	}
    }
}


void
on_save_as1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  if (fs==NULL)
    fs = create_fileselection1 ();

  file_choosen = CHOOSE_XNEE_PROJECT_FILE;
  file_action  = FILE_TO_SAVE;

  gtk_widget_show (fs);
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
  printf ("on_about1\n");

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
	  gnee_xnee_start_recording(ext_xd);
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
	  gnee_xnee_start_replaying(ext_xd);
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
	xnee_set_replay_speed (ext_xd, speed); 
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
	xnee_set_interval (ext_xd, delay); 
      else
	xnee_set_interval (ext_xd, 0); 
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
    GtkToggleButton *togglebutton=NULL;

    delay = gtk_spin_button_get_value_as_int(spinbutton);
    
    if (ext_xd != NULL)
      {
	if (use_delay)
	  {
	    printf ("Setting interval ... to %d\n", delay);
	    xnee_set_interval (ext_xd, delay);
	  }
	else
	  {
	    printf ("Setting interval NOT  ... to %d\n", delay);
	    xnee_set_interval (ext_xd, 0);
	  }
      }
}

void
on_verbose_logging_checkbox_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  gint speed = 0 ; 
  
  
  if (ext_xd != NULL)
    {
      if (gtk_toggle_button_get_active(togglebutton))
	xnee_set_verbose (ext_xd); 
      else
	xnee_unset_verbose (ext_xd); 
    }
}




void
on_spinbutton5_change_value            (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  printf ("5:: %d \n", gtk_spin_button_get_value_as_int(spinbutton));
  if (ext_xd != NULL)
    {
      xnee_set_events_max (ext_xd, 
			    gtk_spin_button_get_value_as_int(spinbutton)); 
    }
}


void
on_spinbutton4_change_value            (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  printf ("4:: %d \n", gtk_spin_button_get_value_as_int(spinbutton));
  if (ext_xd != NULL)
    {
      xnee_set_data_max (ext_xd, 
			  gtk_spin_button_get_value_as_int(spinbutton)); 
    }

}


void
on_spinbutton6_change_value            (GtkSpinButton   *spinbutton,
                                        gpointer         user_data)
{
  printf ("6:: %d \n", gtk_spin_button_get_value_as_int(spinbutton));
  if (ext_xd != NULL)
    {
      xnee_set_time_max (ext_xd, 
			     gtk_spin_button_get_value_as_int(spinbutton)); 
    }

}


void
on_ok_button1_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
  FILE *file;

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
	  xnee_set_rc_byname (ext_xd, filename);
	  file = fopen (filename, "w");
	  xnee_write_settings_to_file (ext_xd, file) ;
	  fclose (file);
	  file_choosen = 0;
	  file_action  = 0;
	}
      else if ( file_action  == FILE_TO_OPEN)
	{
	  int ret;
	  printf (" OPEN\n");
	  file = fopen (filename, "rw");
	  
	  ret = xnee_set_rc_byname (ext_xd, filename);
	  g_print (" ret=%d\n", ret);

	  if ( xnee_get_rc_file (ext_xd) != NULL) 
	    {
	      xnee_add_resource (ext_xd );
	    }
	  else
	    g_print (" Could not open project file %s\n", filename);


	  fclose (file);
	  file_choosen = 0;
	  file_action  = 0;
	  
	  gnee_set_xd_settings();

	}
      else 
	{
	  printf (" DEFAULT\n");
	}
      break;
    case CHOOSE_XNEE_SESSION_FILE:
      printf ("session file: %s\n", filename);
      xnee_set_out_byname (ext_xd, filename); 
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
  if (fs==NULL)
    fs = create_fileselection1 ();
  file_choosen = CHOOSE_XNEE_PROJECT_FILE;
  file_action  = FILE_TO_OPEN;

  printf ("open button\n");

  gtk_widget_show (fs);
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
  if (fs==NULL)
    fs = create_fileselection1 ();
  file_choosen = CHOOSE_XNEE_SESSION_FILE;

  gtk_widget_show (fs);
}


void
on_save_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
  if (ext_xd != NULL)
    {
      FILE *file;
      char *fname = xnee_get_rc_name(ext_xd);

      if (fname == NULL)
	{
	  printf ("Implement me... no file to save\n");
	}
      else
	{
	  file = fopen (fname, "w");
	  xnee_write_settings_to_file (ext_xd, file) ;
	  fclose (file);
	}
    }
}

