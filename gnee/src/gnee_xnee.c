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


#include <pthread.h>


#include "gnee_xnee.h"



#include <libxnee/print.h>
#include <libxnee/xnee_record.h>
#include <libxnee/xnee_replay.h>
#include <libxnee/xnee_setget.h>



static pthread_t action_thread;


static int
gnee_xnee_init_xnee (xnee_data *xd)
{

  static int need_init = 1;

  if (xd==NULL)
    return 1;

  if (! need_init)
    return 0;

  
  (void) signal (SIGINT, signal_handler);
    
  xd = xnee_new_xnee_data();

}

int
gnee_xnee_start_recording(xnee_data* xd)
{
  int ret;

  
  gnee_xnee_init_xnee (xd);

  printf ("recording soon.......\n");

  /* This should be done elsewhere .... just to test */
/*   xnee_set_verbose(xd); */
  xnee_parse_range (xd, XNEE_DEVICE_EVENT, "2-6");
  xnee_parse_range (xd, XNEE_DELIVERED_EVENT, "11-13");
  xnee_set_loops_left (xd, 100);
  xnee_set_recorder (xd);
  xnee_set_out_byname (xd, "hesa.xlr"); 
  /* EO This should be done elsewhere .... just to test */

  sleep (2);
  printf ("recording now.......\n");

  xnee_start(xd);
  
  printf ("recording finished.......\n");
  return 0;
}

int
gnee_xnee_stop_recording(xnee_data* xd)
{
  
  xnee_renew_xnee_data(xd);
  
  return 0;
}

int
gnee_xnee_start_replaying(xnee_data* xd)
{

  xnee_renew_xnee_data(xd);
  printf ("starting to replay soon ....\n");
  xnee_set_data_name_byname (xd, "hesa.xlr"); 
  xnee_set_replayer (xd);
  sleep (2);
  printf ("starting to replay now ....\n");
/*   xnee_set_verbose(xd);  */
  xnee_start(xd);
  return 0;
}

int
gnee_xnee_stop_replaying(xnee_data* xd)
{
  return 0;
}
