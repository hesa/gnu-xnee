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
#include <libxnee/print.h>
#include <libxnee/xnee_record.h>
#include <libxnee/xnee_replay.h>
#include <libxnee/xnee_setget.h>
#include <libxnee/datastrings.h>
#include <libxnee/xnee_range.h>



static pthread_t action_thread;


extern  xnee_data   *ext_xd;
extern  GtkWidget   *ext_gnee_window;

void
gnee_set_events_max(int val)
{
  GtkSpinButton   *spinbutton;
  
  spinbutton = (GtkSpinButton *) 
    lookup_widget (ext_gnee_window, "spinbutton5");
  gtk_spin_button_set_value(spinbutton, val); 
}

void
gnee_set_data_max(int val)
{
  GtkSpinButton   *spinbutton;
  
  spinbutton = (GtkSpinButton *)
    lookup_widget (ext_gnee_window, "spinbutton4");
  gtk_spin_button_set_value(spinbutton, val); 
}


void
gnee_set_time_max(int val)
{
  GtkSpinButton   *spinbutton;
  
  spinbutton = (GtkSpinButton *)
    lookup_widget (ext_gnee_window, "spinbutton6");
  gtk_spin_button_set_value(spinbutton, val); 
}



void
gnee_set_ranges(int type)
{
  int   i    = 0 ;
  int   len  = 0 ;
  int  *data = NULL ; 
  char *name = NULL ;

  printf ("setting Walker Texas ranger\n");

  
  len  = xnee_get_nr_of_data(type);
  data = xnee_get_data (type);
  g_print("Type: %d \n", type);
  g_print("  XNEE_EVENT          = %d \n", XNEE_EVENT);
  g_print("  XNEE_REQUEST        = %d \n", XNEE_REQUEST);
  g_print("  XNEE_REPLY          = %d \n", XNEE_REPLY);
  g_print("  XNEE_ERROR          = %d \n", XNEE_ERROR);
  g_print("  XNEE_DELIVEREDEVENT = %d \n", XNEE_DELIVERED_EVENT);
  g_print("  XNEE_DEVICE_EVENT   = %d \n", XNEE_DEVICE_EVENT);
  for (i=0;i<len;i++)
    {
      name = (char*) xnee_int2data(type, data[i]);
      g_print ("\t%.3d (%s)\n", 
	       data[i], name );
      if ( (type==XNEE_DEVICE_EVENT)
	   || (type==XNEE_DELIVERED_EVENT)
	   || (type==XNEE_EVENT))
	move_event (ext_gnee_window, name);
      else if ( type==XNEE_ERROR)
	move_error (ext_gnee_window, name);
      else if ( type==XNEE_REPLY)
	move_reply (ext_gnee_window, name);
      else if ( type==XNEE_REQUEST)
	move_request (ext_gnee_window, name);
      else
	;
    }
  g_print ("\n");


}




void 
gnee_set_xd_settings()
{
  int val;
  int i ;

  printf ("setting gnee from xd\n");


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
gnee_xnee_init_xnee (xnee_data *xd)
{

  static int need_init = 1;

  if (xd==NULL)
    {
      printf (" 1\n");
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
gnee_xnee_start_recording(xnee_data* xd)
{
  int ret;
  static int ev_save = 0;
  static int dat_save = 0;
  static int tim_save = 0;
  
  xnee_zero_time_recorded(xd);
  xnee_zero_data_recorded(xd);
  xnee_zero_events_recorded(xd);

  xnee_set_recorder (xd);

  xnee_start(xd);
  
  return 0;
}

int
gnee_xnee_stop_recording(xnee_data* xd)
{
  return 0;
}

int
gnee_xnee_start_replaying(xnee_data* xd)
{
  gnee_xnee_init_xnee (xd);
  xnee_set_replayer (xd);

  xnee_set_data_name_byname (xd, "hesa.xlr"); 
  xnee_unset_sync (xd);
  xnee_start(xd);
  return 0;
}

int
gnee_xnee_stop_replaying(xnee_data* xd)
{
  return 0;
}


int
gnee_xnee_init_value(xnee_data* xd)
{
  ;
}

