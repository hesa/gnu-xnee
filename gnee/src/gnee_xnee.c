/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *   Copyright (C) 2003, 2004 Henrik Sandklef and Jon-Erling Dahl
 *                                                                   
 * This program is free software; you can redistribute it and/or     
 * modify it under the terms of the GNU General Public License       
 * as published by the Free Software Foundation; either version 2    
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

#include <gtk/gtk.h>

#include <pthread.h>
#include <support.h>
#include <string.h>


#include "gnee_xnee.h"
#include "interface.h"
#include <libxnee/print.h>
#include <libxnee/xnee_record.h>
#include <libxnee/xnee_replay.h>
#include <libxnee/xnee_setget.h>
#include <libxnee/datastrings.h>
#include <libxnee/xnee_range.h>
#include <libxnee/xnee_threshold.h>
#include <libxnee/xnee_setget.h>
#include <libxnee/xnee_error.h>
#include <libxnee/xnee_session.h>
#include <libxnee/xnee_alloc.h>


/* static gnee_xnee gx ;  */
/* static pthread_t action_thread; */


extern  xnee_data   *ext_xd;
extern  gnee_xnee   *ext_gx;
extern  GtkWidget   *ext_gnee_window;
static  GtkWidget   *err_win=NULL;





void 
gx_init_gx(gnee_xnee *gx) 
{
  GNEE_DEBUG(("gx_init_gx\n"));
  gx->use_rec_display = 0 ; 
  gx->use_rep_display = 0 ; 

  gx->use_speed = 0 ;
  gx->speed = 100 ;
}

static int 
gx_is_using_rec_display(gnee_xnee *gx)  { return (gx->use_rec_display!=0); }

static int 
gx_is_using_rep_display(gnee_xnee *gx)  { return (gx->use_rep_display!=0); }

static int 
gx_is_using_speed(gnee_xnee *gx)        { return (gx->use_speed!=0);}





int
gnee_set_cb(char * cb_name, int on_or_off)
{
  GtkToggleButton *togglebutton;

  togglebutton = (GtkToggleButton*)lookup_widget (ext_gnee_window, 
						  cb_name);
  if (togglebutton!=NULL)
    {
      gtk_toggle_button_set_active(togglebutton, on_or_off);
    }
  return GNEE_OK;
}

void
gnee_set_sb(char *sb_name, int val)
{
  GtkSpinButton   *spinbutton;
  GNEE_DEBUG(("gnee_set_sb\n"));
  spinbutton = (GtkSpinButton *) 
    lookup_widget (ext_gnee_window, sb_name);

  if (spinbutton!=NULL)
    {
      gtk_spin_button_set_value(spinbutton, val); 
    }
}





static int 
gnee_set_interval()
{
  GtkSpinButton *spin=NULL;
  GtkToggleButton *togglebutton;

  GNEE_DEBUG(("gnee_set_interval\n"));
  spin = (GtkSpinButton*)lookup_widget (ext_gnee_window, 
					  "wait_spinbutton");
  togglebutton = (GtkToggleButton*)lookup_widget (ext_gnee_window, 
					  "wait_checkbox");

  if (spin!=NULL)
    {
      gtk_spin_button_set_value(spin,
				xnee_get_interval(ext_xd));
      
    }
  if (togglebutton!=NULL)
    {
      gtk_toggle_button_set_active(togglebutton, 1);
    }

  

  return GNEE_OK;
}

int
gnee_set_verbosity(int on_or_off)
{
  GNEE_DEBUG(("gnee_set_verbosity\n"));
  return gnee_set_cb ("verbose_logging_checkbox", on_or_off);
}

int
gnee_set_no_feedback()
{
  GNEE_DEBUG(("gnee_set_no\n"));
  gnee_set_cb ("no_fb_b", 1);
}

int
gnee_set_xosd_feedback()
{
  GNEE_DEBUG(("gnee_set_xosd\n"));
  gnee_set_cb ("xosd_fb_b", 1);
}

static int
gnee_set_stderr_feedback()
{
  GNEE_DEBUG(("gnee_set_stderr\n"));
  gnee_set_cb ("stderr_fb_b", 1);
}

static int 
gnee_set_various_display(int use_recording_display)
{
  GtkWidget   *disp_text;
  char        *disp_to; 
 
  GNEE_DEBUG(("gnee_set_various\n"));
  if (use_recording_display)
    {
      disp_text = (GtkWidget*) 
	lookup_widget (ext_gnee_window, "rec_disp_text");
    }
  else
    {
      disp_text = (GtkWidget*) 
	lookup_widget (ext_gnee_window, "rep_disp_text");
    }

  if (disp_text==NULL)
    {
      ;
    }
  else
    {
      disp_to = xnee_get_display_name(ext_xd);

      gtk_entry_set_text((GtkEntry*)disp_text,
			 disp_to);
    }
  return GNEE_OK; 
}


int 
gx_set_record_display(xnee_data *xd, gnee_xnee *gx)
{
  GtkWidget   *rec_disp_text;
  
  GNEE_DEBUG(("gx_set_record\n"));

  if (gx_is_using_rec_display(gx))
    {
      rec_disp_text = (GtkWidget*) 
	lookup_widget (ext_gnee_window, "rec_disp_text");
      
      if (rec_disp_text==NULL)
	{
	  xnee_set_display_name(ext_xd, "");
	}
      else
	{
	  xnee_verbose((xd, "set record display to %s\n", 
			gtk_entry_get_text((GtkEntry*)rec_disp_text)));
	  xnee_set_display_name(ext_xd, 
				(char*)
				gtk_entry_get_text((GtkEntry*)rec_disp_text));
	}
    }
  else
    {
      xnee_set_display_name(ext_xd, "");
    }
  return GNEE_OK; 
}

int 
gx_set_replay_display(xnee_data *xd, gnee_xnee *gx)
{
  GtkWidget   *rep_disp_text;
  
  GNEE_DEBUG(("gx_set_replay\n"));

  if (gx_is_using_rep_display(gx))
    {
      rep_disp_text = (GtkWidget*) 
	lookup_widget (ext_gnee_window, "rep_disp_text");
      
      if (rep_disp_text==NULL)
	{
	  xnee_set_display_name(ext_xd, "");
	}
      else
	{
	  xnee_verbose((xd, "set replay display to %s\n", 
			gtk_entry_get_text((GtkEntry*)rep_disp_text)));
	  xnee_set_display_name(ext_xd, 
				(char*)
				gtk_entry_get_text((GtkEntry*)rep_disp_text));
	}
    }
  else
    {
      xnee_set_display_name(ext_xd, "");
    }
  return GNEE_OK; 
}


int 
gx_set_variable_data(xnee_data *xd, gnee_xnee *gx)
{
  GNEE_DEBUG(("gx_set_variable\n"));

  /*
   * Set display (record or replay)
   */
  if (xnee_is_recorder(xd))
    {
      gx_set_record_display(xd, gx);
    }
  else if (xnee_is_replayer(xd))
    {
      gx_set_replay_display(xd, gx);
    }

  GNEE_DEBUG(( "Speed settings : %d (%d)   using=%d\n",
	       xnee_get_replay_speed(xd),
	       ext_gx->speed,
	       ext_gx->use_speed));


  return GNEE_OK;
}




void
gnee_set_events_max(int val)
{
  GtkSpinButton   *spinbutton;
  GNEE_DEBUG(("gnee_set_events\n"));
  
  spinbutton = (GtkSpinButton *) 
    lookup_widget (ext_gnee_window, "spinbutton11");
  gtk_spin_button_set_value(spinbutton, val); 
}

void
gnee_set_max_thresh(int val)
{
  GNEE_DEBUG(("gnee_set_max\n"));
  gnee_set_sb("max_thresh_sb", val);
}

void
gnee_set_min_thresh(int val)
{
  GNEE_DEBUG(("thresh(int val\n"));
  gnee_set_sb("min_thresh_sb", val);
}

void
gnee_set_tot_thresh(int val)
{
  GNEE_DEBUG(("gnee_set_tot\n"));
  gnee_set_sb("tot_thresh_sb", val);
}

void
gnee_set_data_max(int val)
{
  GtkSpinButton   *spinbutton;
  GNEE_DEBUG(("gnee_set_data\n"));
  
  spinbutton = (GtkSpinButton *)
    lookup_widget (ext_gnee_window, "spinbutton12");
  gtk_spin_button_set_value(spinbutton, val); 
}


void
gnee_set_time_max(int val)
{
  GtkSpinButton   *spinbutton;
  
  GNEE_DEBUG(("gnee_set_time\n"));
  spinbutton = (GtkSpinButton *)
    lookup_widget (ext_gnee_window, "spinbutton13");
  gtk_spin_button_set_value(spinbutton, val); 
}





void
gx_save(void)
{
  GNEE_DEBUG(("gx_save(void\n"));
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
gnee_set_ranges(int type)
{
  int   i    = 0 ;
  int   len  = 0 ;
  int  *data = NULL ; 
  char *name = NULL ;

  GNEE_DEBUG(("gnee_set_ranges\n"));

  len  = xnee_get_nr_of_data(type);
  data = xnee_get_data (type);
  
  GNEE_DEBUG(("Setting ranges from project file \n"));

  for (i=0;i<len;i++)
    {
      name = (char*) xnee_int2data(type, data[i]);

      if ( (type==XNEE_DEVICE_EVENT)
	   || (type==XNEE_DELIVERED_EVENT)
	   || (type==XNEE_EVENT))
	{
	  GNEE_DEBUG(("Adding from proj file: \t%.3d (%s)\n", 
		      data[i], name ));
	  gx_add_event(ext_gnee_window, name);
	}
      else if ( type==XNEE_ERROR)
	{
	  GNEE_DEBUG(("\t%.3d (%s)\n", 
		      data[i], name ));
	  gx_add_error(ext_gnee_window, name);
	}
      else if ( type==XNEE_REPLY)
	{
	  GNEE_DEBUG(("\t%.3d (%s)\n", 
		      data[i], name ));
	  gx_add_reply(ext_gnee_window, name);
	}
      else if ( type==XNEE_REQUEST)
	{
	  GNEE_DEBUG(("\t%.3d (%s)\n", 
		      data[i], name ));
	  gx_add_request (ext_gnee_window, name);
	}
      else
	;
    }
}




void 
gx_set_xd_settings()
{
  int val;
  int i ;

  GNEE_DEBUG(("gx_set_xd\n"));

  if (ext_gnee_window == NULL)
    {
      fprintf (stderr, "Could not find a gnee_window in\n");
    }
  
  val = xnee_get_events_max(ext_xd);
  gnee_set_events_max(val);

  val = xnee_get_data_max(ext_xd);
  gnee_set_data_max(val);

  val = xnee_get_time_max(ext_xd);
  gnee_set_time_max(val);


  val = xnee_get_max_threshold(ext_xd); 
  gnee_set_max_thresh(val);  

  val = xnee_get_min_threshold(ext_xd);
  gnee_set_min_thresh(val);

  val = xnee_get_tot_threshold(ext_xd);
  gnee_set_tot_thresh(val);

  for (i=1;i<XNEE_NR_OF_TYPES;i++)
    {
      gnee_set_ranges(i);
    }

  gnee_set_record_display();
  gnee_set_replay_display();
  
  if (xnee_is_verbose(ext_xd))
  {
      gnee_set_verbose();
  }
  else
  {
      gnee_unset_verbose();
  }

  gnee_set_interval();

  if (xnee_is_sync(ext_xd))
  {
      gnee_set_sync();
  }
  else
  {
      gnee_unset_sync();
  }

  if (xnee_is_force_replay(ext_xd))
  {
    gnee_set_force();
  }
  else
  {
    gnee_unset_force();
  }
}


static int
gx_init_xnee (xnee_data *xd)
{

  static int need_init = 1;

  GNEE_DEBUG(("gx_init_xnee\n"));

  if (xd==NULL)
    {
      return 1;
    }

  if (need_init!=1)
    {
      return 0;
    }

  
  (void) signal (SIGINT, signal_handler);
  
  xd = xnee_new_xnee_data();
  need_init=0;
  return 0;
}


void
gnee_reset_gnee()
{
  GNEE_DEBUG(("gnee_reset_gnee\n"));
}


int
gx_start_recording(xnee_data* xd)
{
  int ret;
  GNEE_DEBUG(("gx_start_recording\n"));
  
  GNEE_DEBUG(("Starting recorder\n"));
  xnee_set_recorder (xd);

  GNEE_DEBUG(("setting variable data\n"));
  ret = gx_set_variable_data(xd, ext_gx);
  GNEE_IF_ERROR_RETURN(ret,ext_gnee_window);

  ret = xnee_prepare(xd);
  GNEE_IF_ERROR_RETURN(ret,ext_gnee_window);

  GNEE_DEBUG(("start recording\n"));

  ret = xnee_start(xd);
  GNEE_IF_ERROR_RETURN(ret,ext_gnee_window);

  GNEE_DEBUG((" recorder stopped\n"));
 
  fflush(stdout);
 return 0;
}

int
gx_stop_recording(xnee_data* xd)
{
  xnee_verbose((xd, "stop recording\n"));
  return 0;
}

int
gx_start_replaying(xnee_data* xd)
{
  int ret ; 

  GNEE_DEBUG(("gx_start_replaying\n"));

  GNEE_DEBUG(("Starting replayer\n"));
  xnee_set_replayer(xd);

  GNEE_DEBUG(("setting variable data\n"));
  ret = gx_set_variable_data(xd, ext_gx);
  GNEE_IF_ERROR_RETURN(ret,ext_gnee_window);

  ret = xnee_prepare(xd);
  GNEE_IF_ERROR_RETURN(ret,ext_gnee_window);

  GNEE_DEBUG(("start replaying\n"));

  ret = xnee_start(xd);
  GNEE_IF_ERROR_RETURN(ret,ext_gnee_window);

  GNEE_DEBUG((" replayer stopped\n"));
  return 0;
}

int
gx_stop_replaying(xnee_data* xd)
{
  GNEE_DEBUG(("gx_stop_replaying\n"));
  xnee_verbose((xd, "stop replaying\n"));
  return 0;
}


static char *gx_modifiers[] = {
  "None", 
  "Any", 
  "Control", 
  "Shift", 
  "Alt",
  "Control+Shift", 
  "Control+Alt",
  "Shift+Alt", 
  "Control+Shift+Alt", 
  NULL };

static char *gx_keys[] = {
  "None", 
  "space", 
  "F1", "F2",  "F3",  "F4", 
  "F5", "F6",  "F7",  "F8", 
  "F9", "F10",  "F11",  "F12", 
  "a", "b", "c", "d", "e",
  "f", "g", "h", "i", "j", 
  "k", "l", "m", "n", "o", 
  "p", "q", "r", "s", "t", 
  "u", "v", "x", "y", "z", 
  "0", "1", "2", "3", "4", 
  "5", "6", "7", "8", "9", 
  NULL };


static void
create_km_list(GtkWidget *k_combo)
{
  int i = 0 ; 
/*   GList *m_items = NULL; */
  GList *k_items = NULL;

  GNEE_DEBUG(("create_km_list\n"));
/*   GNEE_DEBUG(("Add modifiers\n")); */
  i=0;
/*   while (gx_modifiers[i]!=NULL) */
/*     { */
/*       GNEE_DEBUG(("Adding modifier: %s\n", gx_modifiers[i])); */
/*       m_items = g_list_append (m_items, gx_modifiers[i]); */
/*       i++; */
/*     } */
/*   GNEE_DEBUG(("Adding modifiers to popdown\n")); */
/*   gtk_combo_set_popdown_strings (GTK_COMBO (m_combo), m_items); */

  
  
/*   GNEE_DEBUG(("Add keys\n")); */
  i=0;
  while (gx_keys[i]!=NULL)
    {
/*       GNEE_DEBUG(("Adding key: %s\n", gx_keys[i])); */
      k_items = g_list_append (k_items, gx_keys[i]);
      i++;
    }
  gtk_combo_set_popdown_strings (GTK_COMBO (k_combo), k_items);
}


static void
create_km_lists_sub(char *key_c)
{
/*   GtkWidget *m_entry; */
  GtkWidget *k_entry;

  GNEE_DEBUG(("create_km_lists\n"));
/*   m_entry = (GtkWidget *) lookup_widget (ext_gnee_window, mod_c); */
  k_entry = (GtkWidget *) lookup_widget (ext_gnee_window, key_c);
/*   create_km_list(m_entry,k_entry); */
  create_km_list(k_entry);
}

void
gx_create_km_lists()
{
  GNEE_DEBUG(("gx_create_km\n"));
  /****************************************************
   * combo#     
     1       stop   modifier
     2       stop   key
     3       pause  modifier
     4       resume modifier
     5       mark   modifier
     6       exec   modifier
     7       pause  key
     8       resume key 
     9       mark   key
     10      exec   key
  ******************************************************/
  create_km_lists_sub ("combo2");
  create_km_lists_sub ("combo7");
  create_km_lists_sub ("combo8");
  create_km_lists_sub ("combo9");
  create_km_lists_sub ("combo10");
}



void 
gx_undisplay_errror()
{
  GNEE_DEBUG(("gx_undisplay_errror\n"));
  gtk_widget_hide(err_win);
}

void 
gx_display_errror(int err_nr)
{
  GtkLabel   *my_text;
#define GNEE_BUF_SIZE 8
  gchar buf[GNEE_BUF_SIZE];
  const char *str_ptr = NULL;
  int i ;

  GNEE_DEBUG(("create_km_list\n"));
  if (err_win == NULL)
    {
      err_win = create_dialog1();
    }

  /* number field */
  my_text = (GtkLabel *) 
    lookup_widget (err_win, "err_nr_label");
  snprintf (buf, GNEE_BUF_SIZE, "%d", err_nr);
  gtk_label_set_text(my_text,buf);

  /* description field */
  my_text = (GtkLabel *) 
    lookup_widget (err_win, "descr_label");
  if (my_text!=NULL)
    {
      str_ptr = xnee_get_err_description(err_nr);
      if (str_ptr!=NULL)
	{
	  gtk_label_set_text(my_text,str_ptr);
	}
    }
  
  /* description field */
  my_text = (GtkLabel *) 
    lookup_widget (err_win, "solu_label");
  if (my_text!=NULL)
    {
      str_ptr = xnee_get_err_solution(err_nr);
      if (str_ptr!=NULL)
	{
	  gtk_label_set_text(my_text,str_ptr);
	}
    }

  gtk_widget_show (err_win);


  for (i=XNEE_OK;i<=XNEE_LAST_ERROR;i++)
    {
      printf ("%.2d\n\t%s\n\t%s\n",
	      i,
	      xnee_get_err_description(i),
	      xnee_get_err_solution(i)) ;
    }

}




int 
gx_set_use_speed (int val) 
{
  GNEE_DEBUG(("gx_set_use_speed (%d)\n", val));

  ext_gx->use_speed = val;

  if (val==0)
    {
	gx_set_replay_speed(ext_xd, 100);
	;
    }
  else
    {
      GtkSpinButton   *spinbutton;
      int val ; 

      spinbutton = (GtkSpinButton *) 
	  lookup_widget (ext_gnee_window, "speed_spin");
      val = gtk_spin_button_get_value_as_int(spinbutton); 
      GNEE_DEBUG(("gx_set_replay_speed (%d\n", val));
      gx_set_replay_speed(ext_xd, val);
    }
  return GNEE_OK;
}

int 
gx_set_speed (int val) 
{
    GNEE_DEBUG(("gx_set_speed to %d\n", val));
    if ( ext_gx->use_speed != 0 )
    {
	gx_set_replay_speed(ext_xd, val);
	ext_gx->speed = val ; 
    }
    else
    {
	gx_set_replay_speed(ext_xd, 100);
    }
    return GNEE_OK;
}


void 
gnee_set_rec_file(char *filename)
{
  GtkEntry *file_text ; 
  GNEE_DEBUG(("gnee_set_rec\n"));
  
  file_text = (GtkEntry*) lookup_widget(GTK_WIDGET(ext_gnee_window),
					"rec_file_text");
  
  gtk_entry_set_text (file_text, 
		      filename);
  xnee_set_out_name (ext_xd, filename);
}

void 
gnee_set_rep_file(char *filename)
{
  GtkEntry *file_text ; 
  
  GNEE_DEBUG(("gnee_set_rep\n"));
  file_text = (GtkEntry*) lookup_widget(GTK_WIDGET(ext_gnee_window),
					"rep_file_text");
  
  gtk_entry_set_text (file_text, 
		      filename);

  xnee_set_data_name (ext_xd, filename);
}


void
gnee_close_down()
{
  GNEE_DEBUG(("gnee_close_down\n"));
  xnee_close_down(ext_xd);
  exit(GNEE_OK);
}


char *
gx_get_default_filename()
{
  char *tmp_dir = NULL;
  char *user_name = NULL;

  const char *default_tmp_dir   = "/tmp/";
  const char *default_user_name = "user";
  const char *file_prefix = "gnee_";
  const char *file_suffix = ".xns";

  char *ret_str;
  int   size = 0 ; 

  GNEE_DEBUG(("gx_get_default\n"));

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

