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


#include <X11/extensions/XTest.h>

#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"
#include "libxnee/xnee_sem.h"
#include "libxnee/xnee_resolution.h"
#include "libxnee/xnee_time.h"



/*
 * internal use only
 */
void 
xnee_correct_time(unsigned long *myTime) 
{
  if (*myTime < 2) 
    {
      *myTime = 2;
    }
}





int
xnee_fake(xnee_data *xd, xnee_intercept_data* xindata)
{
  return XNEE_OK;
}


/**************************************************************
 *                                                            *
 * xnee_replay_event_handler                                  *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_replay_event_handler( xnee_data* xd, xnee_intercept_data* xindata, int last_elapsed)
{
  int           return_value=0;
  unsigned long last_diff ;
  unsigned long first_diff; 
  unsigned long record_last_diff; 
  unsigned long sleep_amt; 
  unsigned long record_first_diff =0 ;
  int screen;
  int x ;
  int y ; 

  Time saved_time = 0 ; /* used to restore time of last replayable event */
  

  xnee_verbose((xd, "---> xnee_replay_event_handler \n "));
  xnee_verbose((xd, "---  xnee_replay_event_handler fake=%d\n ", xd->fake));
  XTestGrabControl (xd->fake, True); 
  xnee_verbose((xd, "---  xnee_replay_event_handler 0\n "));
  XFlush(xd->fake);                 

  xnee_verbose((xd, "---  xnee_replay_event_handler 1\n "));
  /* Get the recorded time difference. Should be from previous read 
     X server time recorded in file*/
  /* need to mult by 1000 to get microseconds */
  saved_time = xindata->oldtime ;
  record_last_diff = xnee_delta_time(xindata) * 1000; /* Synchronise the time */
  record_first_diff = ( xindata->newtime - xd->first_read_time ) * 1000;

  /* get the actual difference from last read - we may have had to wait/sleep */
  last_diff = xnee_get_elapsed_time(xd, XNEE_FROM_LAST_READ );

  /* get the actual elapsed time from the start of the read */
  first_diff = xnee_get_elapsed_time(xd, XNEE_FROM_FIRST_READ );


  xnee_verbose((xd, "---  xnee_replay_event_handler \n "));


  /* if the first event is also the 1st entry from recorded file 
     - reset time - should seldom happen */
  if (xd->first_replayed_event==1 && last_elapsed == 0 ) 
    {
      xd->first_replayed_event=0;
      xnee_verbose ((xd, 
		     "\txd->first_replayed_event==1  ----> dtime1=10 ; \n"));
      record_last_diff = 10 * 1000; 
    }

  sleep_amt = xnee_calc_sleep_amount( xd, last_diff, first_diff, 
				      record_last_diff, record_first_diff )  
    / 1000 ; 

  xnee_verbose((xd, "---  xnee_replay_event_handler \n "));


  xnee_verbose ((xd, "switching type: %d sleep_amt: %d\n", 
		 xindata->u.event.type, sleep_amt ));
  fflush( NULL );
  xnee_verbose((xd, "---> xnee_replay_event_handler switching type%d\n", 
		xindata->u.event.type));
  
  /* If we use the last args to the XTestFakexxx functions
   * it is harder to synchronize .... 
   * XNEE_FAKE_SLEEP is a macro for usleep 
   */
  switch (xindata->u.event.type)
    {
    case KeyPress:
      xnee_fake_key_event (xd, xindata->u.event.keycode, True, CurrentTime );
      break;
    case KeyRelease:
      xnee_fake_key_event  (xd, xindata->u.event.keycode, False, CurrentTime);
      break;
    case ButtonPress:
      xnee_fake_button_event (xd, xindata->u.event.button, True, CurrentTime);
      break;
    case ButtonRelease:
      xnee_fake_button_event (xd, xindata->u.event.button, False, CurrentTime);
      break;
    case MotionNotify:
      screen = xindata->u.event.screen_nr ; 
      x = (int) xindata->u.event.x ; 
      y = (int) xindata->u.event.y ; 
      xnee_fake_motion_event (xd,
			      0,
			      x, 
			      y, 
			      CurrentTime);
      break;

    default:                  /*  Do nothing  */
      xnee_verbose((xd, " Did NOT replay %d  returning XNEE_NOT_REPLAYABLE \n", xindata->u.event.type));
      /* restore time of last replayable event */
      xindata->oldtime = saved_time;
      return_value= XNEE_NOT_REPLAYABLE;
      break;
    }

  if (return_value==0)
    {
      ;
    }
  xnee_verbose((xd, "<--- xnee_replay_event_handler returning after handling of \n", xindata->u.event.type ));

  return return_value ;
}







/**************************************************************
 *                                                            *
 * xnee_fake_key_event                                        *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_fake_key_event  (xnee_data* xd, int keycode, Bool bo, int dtime)
{
  int i=0;
  int size= xd->distr_list_size;
  
  xnee_verbose((xd, "XTestFakeKeyEvent (%d, %d, %d, %d ))\n",
		(int) xd->fake, 
		(int) keycode, 
		(int) bo, 
		(int) dtime));
  XTestFakeKeyEvent (xd->fake, keycode, bo, CurrentTime);
  XNEE_FAKE_SLEEP ( dtime);  

  for (i=0; i<size ; i++)
    {
      XTestGrabControl (xd->distr_list[i].dpy, True); 
      xnee_verbose((xd, "XTestFakeKeyEvent (%d, %d, %d, %d )) **\n",
		    (int) xd->distr_list[i].dpy, 
		    (int) keycode, 
		    (int) bo, 
		    (int) dtime));
      XTestFakeKeyEvent (xd->distr_list[i].dpy, keycode, bo, dtime);
      XFlush (xd->distr_list[i].dpy);
    }
  
  xnee_verbose((xd,"\n\n\n"));
  return (XNEE_OK);
}





/**************************************************************
 *                                                            *
 * xnee_fake_button_event                                     *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_fake_button_event (xnee_data* xd, int button, Bool bo , int dtime)
{
  int i=0;
  int size= xd->distr_list_size;
  
  XNEE_FAKE_SLEEP ( dtime );   
  xnee_verbose((xd, "XTestFakeButtonEvent (%d, %d, %d, %d)) \n",
		(int) xd->fake, 
		(int) button, 
		(int) bo, 
		(int) dtime));
  XTestFakeButtonEvent (xd->fake, button, True, 0);
  
  for (i=0; i<size ; i++)
    {
      XTestGrabControl (xd->distr_list[i].dpy, True); 
      xnee_verbose((xd, "XTestFakeButtonEvent (%d, %d, %d, %d))  **\n",
		   (int) xd->distr_list[i].dpy, 
		   (int) button, 
		   (int) bo, 
		   (int) dtime));
      XTestFakeButtonEvent (xd->distr_list[i].dpy, button, bo, dtime);
      XFlush (xd->distr_list[i].dpy);
    }
  return (XNEE_OK);
}




/**************************************************************
 *                                                            *
 * xnee_fake_motion_event                                     *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_fake_motion_event (xnee_data* xd,
			int screen, 
			int x, 
			int y, 
			int dtime)
{
  int i=0;
  int size= xd->distr_list_size;
  
  x = xnee_resolution_newx(xd,x);
  y = xnee_resolution_newx(xd,y);
  
  xnee_correct_time((unsigned long *)&dtime);
  xnee_verbose((xd, "XTestFakeMotionEvent (%d, %d, %d, %d, %lu))\n",
		(int) xd->fake, 
		(int) screen, 
		(int) x,
		(int) y,
		0));
  XTestFakeMotionEvent(xd->fake, 
		       screen, 
		       x, 
		       y, 
		       0);
  XNEE_FAKE_SLEEP ( dtime );  
  
  xnee_verbose((xd, "---> xnee_fake_motion_event\n"));
  
  for (i=0; i<size ; i++)
    {
      XTestGrabControl (xd->distr_list[i].dpy, True); 

      xnee_verbose((xd, "XTestFakeMotionEvent (%d, %d, %d, %d, %lu))  **\n",
		   (int) xd->distr_list[i].dpy, 
		   (int) screen, 
		   (int) x,
		   (int) y,
		    10));
      XTestFakeMotionEvent(xd->distr_list[i].dpy, 
			   screen, 
			   x, 
			   y, 
			   CurrentTime);
      XFlush (xd->distr_list[i].dpy);
      
    }
  xnee_verbose((xd, " <------- xnee_fake_motion_event\n"));
  return (XNEE_OK);
}




