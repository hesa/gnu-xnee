/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003 Henrik Sandklef          
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston,            
 * MA  02111-1307, USA.                                              
 ****/

#include <gtk/gtk.h>

#include <pthread.h>


#include "gnee_xnee.h"
#include "interface.h"
#include <libxnee/print.h>
#include <libxnee/xnee_record.h>
#include <libxnee/xnee_replay.h>
#include <libxnee/xnee_setget.h>
#include <libxnee/datastrings.h>
#include <libxnee/xnee_range.h>


static gnee_xnee gx ; 
static pthread_t action_thread;


extern  xnee_data   *ext_xd;
extern  gnee_xnee   *ext_gx;
extern  GtkWidget   *ext_gnee_window;
static GtkWidget *err_win=NULL;


void gx_init_gx(gnee_xnee *gx) 
{
  gx->use_rec_display = 0 ; 
  gx->use_rep_display = 0 ; 
}

int     gx_is_using_rec_display(gnee_xnee *gx)  { return gx->use_rec_display; }
int     gx_is_using_rep_display(gnee_xnee *gx)  { return gx->use_rep_display; }



int 
gx_set_record_display(xnee_data *xd, gnee_xnee *gx)
{
  GtkWidget   *rec_disp_text;

  if (gx_is_using_rec_display(gx))
    {
      rec_disp_text = (GtkWidget*) 
	lookup_widget (ext_gnee_window, "rec_disp_text");
      
      if (rec_disp_text==NULL)
	{
	  ;
	}
      else
	{
	  printf ("new display to record from is: %s\n", 
		  gtk_entry_get_text((GtkEntry*)rec_disp_text));
	  xnee_set_display_name(ext_xd, 
				(char*)
				gtk_entry_get_text((GtkEntry*)rec_disp_text));
	}
    }
  return GNEE_OK; 
}

int 
gx_set_replay_display(xnee_data *xd, gnee_xnee *gx)
{
  GtkWidget   *rep_disp_text;
  
  if (gx_is_using_rep_display(gx))
    {
      rep_disp_text = (GtkWidget*) 
	lookup_widget (ext_gnee_window, "rep_disp_text");
      
      if (rep_disp_text==NULL)
	{
	  ;
	}
      else
	{
	  printf ("new display to replay to is: %s\n", 
		  gtk_entry_get_text((GtkEntry*)rep_disp_text));
	  xnee_set_display_name(ext_xd, 
				(char*)
				gtk_entry_get_text((GtkEntry*)rep_disp_text));
	}
    }
  return GNEE_OK; 
}

int 
gx_set_variable_data(xnee_data *xd, gnee_xnee *gx)
{
  gx_set_replay_display(xd, gx);
  gx_set_record_display(xd, gx);
}


void
gnee_set_events_max(int val)
{
  GtkSpinButton   *spinbutton;
  
  spinbutton = (GtkSpinButton *) 
    lookup_widget (ext_gnee_window, "spinbutton11");
  gtk_spin_button_set_value(spinbutton, val); 
}

void
gnee_set_data_max(int val)
{
  GtkSpinButton   *spinbutton;
  
  spinbutton = (GtkSpinButton *)
    lookup_widget (ext_gnee_window, "spinbutton12");
  gtk_spin_button_set_value(spinbutton, val); 
}


void
gnee_set_time_max(int val)
{
  GtkSpinButton   *spinbutton;
  
  spinbutton = (GtkSpinButton *)
    lookup_widget (ext_gnee_window, "spinbutton13");
  gtk_spin_button_set_value(spinbutton, val); 
}





void
gx_save(void)
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
gnee_set_ranges(int type)
{
  int   i    = 0 ;
  int   len  = 0 ;
  int  *data = NULL ; 
  char *name = NULL ;

  len  = xnee_get_nr_of_data(type);
  data = xnee_get_data (type);
  
  GNEE_DEBUG(("Setting ranges from project file \n"));
  GNEE_DEBUG(("  type    %d  \n", type));
  GNEE_DEBUG(("  len     %d  \n", len));

  for (i=0;i<len;i++)
    {
      name = (char*) xnee_int2data(type, data[i]);
      GNEE_DEBUG(("  ... trying %s \n", name));

      if ( (type==XNEE_DEVICE_EVENT)
	   || (type==XNEE_DELIVERED_EVENT)
	   || (type==XNEE_EVENT))
	{
	  GNEE_DEBUG(("Adding from proj file: \t%.3d (%s)\n", 
		      data[i], name ));
	  gx_move_event(ext_gnee_window, name);
	}
      else if ( type==XNEE_ERROR)
	{
	  GNEE_DEBUG(("\t%.3d (%s)\n", 
		      data[i], name ));
	  gx_move_error(ext_gnee_window, name);
	}
      else if ( type==XNEE_REPLY)
	{
	  GNEE_DEBUG(("\t%.3d (%s)\n", 
		      data[i], name ));
	  gx_move_reply(ext_gnee_window, name);
	}
      else if ( type==XNEE_REQUEST)
	{
	  GNEE_DEBUG(("\t%.3d (%s)\n", 
		      data[i], name ));
	  gx_move_request (ext_gnee_window, name);
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

  if (ext_gnee_window != NULL)
    {
      printf ("Aint got no window\n");
    }
  

  val = xnee_get_events_max(ext_xd);
  gnee_set_events_max(val);

  val = xnee_get_data_max(ext_xd);
  gnee_set_data_max(val);

  val = xnee_get_time_max(ext_xd);
  gnee_set_time_max(val);

  for (i=1;i<XNEE_NR_OF_TYPES;i++)
    {
      gnee_set_ranges(i);
    }
}


static int
gx_init_xnee (xnee_data *xd)
{

  static int need_init = 1;

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

int
gx_start_recording(xnee_data* xd)
{
  int ret;
  static int ev_save = 0;
  static int dat_save = 0;
  static int tim_save = 0;

  gx_set_variable_data(xd, ext_gx);

  xnee_set_recorder (xd);

  ret = xnee_start(xd);
  if (ret != XNEE_OK) 
    {
      gtk_widget_show_all(ext_gnee_window);
      gx_display_errror(ret, "failed to start recording.. bad grab?");
      printf ("\n\nfailed to start gnee... cause: %d\n", ret);
    }
  
  return 0;
}

int
gx_stop_recording(xnee_data* xd)
{
  return 0;
}

int
gx_start_replaying(xnee_data* xd)
{

  gx_init_xnee(xd);
  xnee_set_replayer(xd);

  xnee_start(xd);
  return 0;
}

int
gx_stop_replaying(xnee_data* xd)
{
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
  "None", "a", "b", "c" , "d" , "e" ,
  "f", "g", "h", "i" , "j", "k", "l", "m",
  "n" , "o" , "p", "q", "r", "s", "t", "u", 
  "v", "x", "y", "z", 
  "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", 
  NULL };


void
create_km_list(GtkWidget *m_combo,GtkWidget *k_combo)
{
  int i = 0 ; 
  GList *m_items = NULL;
  GList *k_items = NULL;

  GNEE_DEBUG(("Add modifiers\n"));
  i=0;
  while (gx_modifiers[i]!=NULL)
    {
      GNEE_DEBUG(("Adding modifier: %s\n", gx_modifiers[i]));
      m_items = g_list_append (m_items, gx_modifiers[i]);
      i++;
    }
  GNEE_DEBUG(("Adding modifiers to popdown\n"));
  gtk_combo_set_popdown_strings (GTK_COMBO (m_combo), m_items);

  
  
  GNEE_DEBUG(("Add keys\n"));
  i=0;
  while (gx_keys[i]!=NULL)
    {
      GNEE_DEBUG(("Adding key: %s\n", gx_keys[i]));
      k_items = g_list_append (k_items, gx_keys[i]);
      i++;
    }
  gtk_combo_set_popdown_strings (GTK_COMBO (k_combo), k_items);
}


void
create_km_lists_sub(GtkWidget* gnee_window, char *mod_c, char *key_c)
{
  GtkWidget *m_entry;
  GtkWidget *k_entry;

  m_entry = (GtkWidget *) lookup_widget (ext_gnee_window, mod_c);
  k_entry = (GtkWidget *) lookup_widget (ext_gnee_window, key_c);
  create_km_list(m_entry,k_entry);
}

void
gx_create_km_lists(GtkWidget* gnee_window)
{
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
  create_km_lists_sub ( gnee_window, "combo1", "combo2");
  create_km_lists_sub ( gnee_window, "combo3", "combo7");
  create_km_lists_sub ( gnee_window, "combo4", "combo8");
  create_km_lists_sub ( gnee_window, "combo5", "combo9");
  create_km_lists_sub ( gnee_window, "combo6", "combo10");
}



void 
gx_undisplay_errror()
{
  gtk_widget_hide(err_win);
}

void 
gx_display_errror(int err_nr, char *err_str)
{
  GtkLabel   *my_text;
  gchar buf[4];

  if (err_win == NULL)
    {
      err_win = create_dialog1();
    }

  my_text = (GtkLabel *) 
    lookup_widget (err_win, "descr_label");
  
  gtk_label_set_text(my_text,err_str);

  my_text = (GtkLabel *) 
    lookup_widget (err_win, "err_nr_label");
  sprintf (buf, "%d", err_nr);
  gtk_label_set_text(my_text,buf);
  
  gtk_widget_show (err_win);
}
