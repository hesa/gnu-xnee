/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003,
 *                      2010  Henrik Sandklef     
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


#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"
#include "libxnee/xnee_sem.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_resolution.h"
#include "libxnee/xnee_time.h"
#include "libxnee/xnee_fake.h"
#include "libxnee/xnee_keysym.h"
#include "libxnee/xnee_display.h"
#include "libxnee/xnee_xinput.h"
#include "libxnee/xnee_utils.h"


static int loop_nr = 0;

#ifdef XNEE_XINPUT_SUPPORT
static int xnee_xi_last_know_pointer_pos[]={0,0};
#endif /* XNEE_XINPUT_SUPPORT */

int
xnee_reset_fake( xnee_data *xd)
{
    xnee_verbose((xd, "reset fake\n"));
    loop_nr = 0 ;
    return XNEE_OK;
}

static void
xnee_fake_sleep(unsigned long period)
{ 
  static unsigned long collected_time=0;

#define SLEEP_THRESH 10
  if (period>SLEEP_THRESH)
    {
      usleep (period*1000); 
    }
  else
    {
      collected_time+=period;
      if (collected_time>SLEEP_THRESH)
	{
	  usleep(collected_time*1000);
	  collected_time=0;
	}
    }
}




static unsigned long
xnee_replay_event_handler_sleep_amt(xnee_data* xd, 
				    xnee_intercept_data* xindata, 
				    long int last_elapsed,
				    int      only_update_time)
{
  unsigned long last_diff = 0;
  unsigned long first_diff = 0 ; 
  unsigned long record_last_diff = 0 ; 
  unsigned long sleep_amt = 0; 
  unsigned long record_first_diff =0 ;
  int speed ; 
  Time saved_time = 0 ; /* used to restore time of last replayable event */


  /* Get the recorded time difference. 
     Should be from previous read 
     X server time recorded in file*/
  saved_time = xindata->oldtime ;

  if ( only_update_time )
    {
      xindata->oldtime = saved_time; 
      return 0;
    }



  /* Synchronise the time */
  xnee_verbose((xd,"ev handler  new %lu   old %lu\n",xindata->newtime , xindata->oldtime ));
  record_last_diff = xnee_delta_time(xindata) ; 

  record_first_diff = ( xindata->newtime - xd->first_read_time ) ;


  /* get the actual difference from last read - 
     we may have had to wait/sleep */
  last_diff = xnee_get_elapsed_time(xd, XNEE_FROM_LAST_READ );
  
  /* get the actual elapsed time from the start of the read */
  first_diff = xnee_get_elapsed_time(xd, XNEE_FROM_FIRST_READ );
  


  if (loop_nr==0)
    {
      record_last_diff = 0 ;
      record_first_diff = 0 ;
      loop_nr=1;
    }
  else if (loop_nr==1)
    {
      record_last_diff = 0 ;
      record_first_diff = 0 ;
      loop_nr=2;
    }

  /* if the first event is also the 1st entry from recorded file 
     - reset time - should seldom happen */
  if ( (xd->first_replayed_event==XNEE_TRUE) && (last_elapsed == 0 )) 
    {
      xd->first_replayed_event=XNEE_FALSE;
      xnee_verbose ((xd, 
		     "\txd->first_replayed_event==%d  ----> dtime1=10 ; \n", XNEE_TRUE
		     ));
      record_last_diff = 10 ; 
    }
  speed = xnee_get_replay_speed(xd); 
  
  if (speed==100)
    {
      sleep_amt = 
	xnee_calc_sleep_amount( xd, 
				last_diff, 
				first_diff, 
				record_last_diff, 
				record_first_diff ) ; 
    } 
  else  if (speed>100)
    {
      sleep_amt = 
	xnee_calc_sleep_amount_fast( xd, 
				     last_diff, 
				     first_diff, 
				     record_last_diff, 
				     record_first_diff ) ; 
    }
  else  
    {
      sleep_amt = 
	xnee_calc_sleep_amount_slow( xd, 
				     last_diff, 
				     first_diff, 
				     record_last_diff, 
				     record_first_diff ) ; 
    }

  /*  
    printf ("xnee_calc_sleep_amount : %lu : (%lu) : %lu :  %lu ===>  %d\n",     
	    last_diff, 
	    first_diff,  
	    record_last_diff, 
	    record_first_diff, 
	    sleep_amt);
    fflush(stdout);
  */

  return sleep_amt;
}


/**************************************************************
 *                                                            *
 * xnee_replay_event_handler                                  *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_replay_event_handler( xnee_data* xd, 
			   xnee_intercept_data* xindata, 
			   long int last_elapsed)
{
  int           return_value=0;
  unsigned long sleep_amt = 0; 

  int screen;
  int x ;
  int y ; 

  xnee_verbose((xd, "---  xnee_replay_event_handler fake=%p\n ", (void*)xd->fake));
  XTestGrabControl (xd->fake, True); 
  xnee_verbose((xd, "---  xnee_replay_event_handler 0\n "));
  XFlush(xd->fake);                 

  xnee_verbose((xd, "---  xnee_replay_event_handler 1\n "));

  xnee_verbose((xd, "---  xnee_replay_event_handler switch on %d  (assuming %d or %d )\n ", 
		xindata->u.event.type, 
		KeyPress, 
		KeyRelease));
  
#ifdef XNEE_XINPUT_SUPPORT
  if ( xnee_is_forced_core_device_events(xd))
   {
      if ( xindata->type == XNEE_PROTO_XINPUT_EVENT_MASTER )
	{
	  xnee_verbose ((xd, "ignore master event %d\n", xindata->type));
	}
      else
	{
#endif /* XNEE_XINPUT_SUPPORT*/
	  /* If we use the last args to the XTestFakexxx functions
	   * it is harder to synchronize .... 
	   * XNEE_FAKE_SLEEP is a macro for usleep 
	   */
	  switch (xindata->u.event.type)
	    {
	    case KeyPress:
	      sleep_amt = xnee_replay_event_handler_sleep_amt(xd, 
							      xindata, 
							      last_elapsed,
							      0);
	      xnee_inc_events_replayed(xd);      
	      xnee_fake_key_event (xd, xindata->u.event.keycode, XNEE_PRESS, sleep_amt );
	      break;
	    case KeyRelease:
	      sleep_amt = xnee_replay_event_handler_sleep_amt(xd, 
							      xindata, 
							      last_elapsed,
							      0);
	      xnee_inc_events_replayed(xd);      
	      xnee_fake_key_event (xd, xindata->u.event.keycode, XNEE_RELEASE, sleep_amt);
	      break;
	    case ButtonPress:
	      sleep_amt = xnee_replay_event_handler_sleep_amt(xd, 
							      xindata, 
							      last_elapsed,
							      0);
	      xnee_inc_events_replayed(xd);      
	      xnee_fake_button_event(xd, xindata->u.event.button, XNEE_PRESS, sleep_amt);
	      break;
	    case ButtonRelease:
	      sleep_amt = xnee_replay_event_handler_sleep_amt(xd, 
							      xindata, 
							      last_elapsed,
							      0);
	      xnee_inc_events_replayed(xd);      
	      xnee_fake_button_event(xd, xindata->u.event.button, XNEE_RELEASE, sleep_amt);
	      break;
	    case MotionNotify:
	      xnee_inc_events_replayed(xd);      
	      screen = xindata->u.event.screen_nr ; 
	      x      = (int) xindata->u.event.x ; 
	      y      = (int) xindata->u.event.y ; 


	      if (xnee_is_screen_ok(xd, screen))
		{
		  /* xnee_set_verbose(xd); */
		  sleep_amt = xnee_replay_event_handler_sleep_amt(xd, 
								  xindata, 
								  last_elapsed,
								  0);
		  /*xnee_unset_verbose(xd); */
		  xnee_fake_motion_event (xd,
					  screen,
					  x, 
					  y, 
					  sleep_amt);
		}
	      else 
		{
		  ;
		}
	      break;
	      
	    default:                  /*  Do nothing  */
	      xnee_verbose((xd, " Did NOT replay %d  returning XNEE_NOT_REPLAYABLE \n", xindata->u.event.type));
	      /* restore time of last replayable event */
	      xnee_replay_event_handler_sleep_amt(xd, 
						  xindata, 
						  last_elapsed,
						  1);
	      return_value= XNEE_NOT_REPLAYABLE;
	      break;
	    }
#ifdef XNEE_XINPUT_SUPPORT
	}
    }
  else
    {
      if ( xindata->type == XNEE_PROTO_XINPUT_EVENT_SLAVE )
	{
	  sleep_amt = xnee_replay_event_handler_sleep_amt(xd, 
							  xindata, 
							  last_elapsed,
							  0);

	  if ( ( xindata->u.xievent.type >= KeyPress ) 
	       && (xindata->u.xievent.type <= MotionNotify) )
	    {
	      int screen = xindata->u.event.screen_nr ; 
	      int x      = (int) xindata->u.xievent.x ; 
	      int y      = (int) xindata->u.xievent.y ; 
	      int devid  = (int) xindata->u.xievent.deviceid ; 
	      
	      if ( xindata->u.xievent.type == MotionNotify) 
		{
		  xnee_fake_xi_motion_event (xd,
					     screen,
					     x, 
					     y, 
					     sleep_amt,
					     devid);
		}
	      else if (xindata->u.xievent.type == ButtonPress) 
		{
		  xnee_inc_events_replayed(xd);      
		  xnee_fake_xi_button_event(xd, xindata->u.event.button, 
					    XNEE_PRESS, sleep_amt, devid);
		}
	      else if (xindata->u.xievent.type == ButtonRelease) 
		{
		  xnee_inc_events_replayed(xd);      
		  xnee_fake_xi_button_event(xd, xindata->u.event.button, 
					    XNEE_RELEASE, sleep_amt, devid);
		}
	      else if (xindata->u.xievent.type == KeyPress) 
		{
		  xnee_inc_events_replayed(xd);      
		  xnee_fake_xi_key_event (xd, xindata->u.xievent.keycode, XNEE_PRESS, sleep_amt, devid);
		}
	      else if (xindata->u.xievent.type == KeyRelease) 
		{
		  xnee_inc_events_replayed(xd);      
		  xnee_fake_xi_key_event (xd, xindata->u.xievent.keycode, XNEE_RELEASE, sleep_amt, devid);
		}
	    }
	}
      else
	{
	  xnee_verbose((xd, 
			"Not replaying master device events\n"));
	}
    }
#endif /* XNEE_XINPUT_SUPPORT*/
  
  if (return_value==0)
    {
      ;
    }
  xnee_verbose((xd, "<--- xnee_replay_event_handler returning after handling of %d \n", 
		xindata->u.event.type ));
  
  

  return return_value ;
}
  
  



/**************************************************************
 *                                                            *
 * xnee_fake_key_event_impl                                   *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_fake_key_event_impl  (xnee_data* xd, int keycode, Bool bo, int dtime, int deviceid)
{
  int i=0;

  int size= xd->distr_list_size;

#ifdef XNEE_XINPUT_SUPPORT
	  XDevice *xdevice;
#endif /* XNEE_XINPUT_SUPPORT*/

  if (!xnee_is_recorder (xd))
    {
      if (xnee_is_swinput_playback(xd))
	{
	  fprintf (stdout, "fake swinput\n");
	  fprintf (stderr, "fake swinput\n");
	}
      else if (deviceid != 0 )
	{
#ifdef XNEE_XINPUT_SUPPORT
	 
	  xdevice = xnee_get_xinput_device(xd, deviceid);

	  xnee_fake_sleep (dtime); 
	  

	  XTestFakeDeviceKeyEvent(xd->fake, 
				  xdevice, 
				  keycode, 
				  bo, 
				  NULL,
				  0,
				  1);

#endif /* XNEE_XINPUT_SUPPORT*/
	}
      else
	{
	  xnee_fake_sleep (dtime);
	  xnee_verbose((xd, "XTestFakeKeyEvent (%p, %d, %d, %d ))\n",
			(void*) xd->fake, 
			(int) keycode, 
			(int) bo, 
			(int) dtime));
	  XTestFakeKeyEvent (xd->fake, keycode, bo, CurrentTime);
	  XFlush(xd->fake);
      
	  for (i=0; i<size ; i++)
	    {
	      XTestGrabControl (xd->distr_list[i].dpy, True); 
	      xnee_verbose((xd, "XTestFakeKeyEvent (%p, %d, %d, %d )) **\n",
			    (void*) xd->distr_list[i].dpy, 
			    (int) keycode, 
			    (int) bo, 
			    (int) dtime));
	      XTestFakeKeyEvent (xd->distr_list[i].dpy, keycode, bo, dtime);
	      XFlush (xd->distr_list[i].dpy);
	    }
	}
    }
  xnee_verbose((xd,"\n\n\n"));
  return (XNEE_OK);
}



/**************************************************************
 *                                                            *
 * xnee_fake_key_mod_event                                        *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_fake_key_mod_event (xnee_data* xd, xnee_script_s *xss, Bool bo, int dtime)
{
  int i=0;
  int size= xd->distr_list_size;
  int mods=0;


  if (!xnee_is_recorder (xd))
    {

      for (mods=0;(mods<8)&&(xss->kc.mod_keycodes[mods]!=0);mods++)
	{
	  xnee_fake_sleep (dtime);

	  xnee_fake_key_event (xd,
			       xss->kc.mod_keycodes[mods], 
			       bo, 
			       0);
	}
      xnee_fake_sleep (dtime);
      xnee_verbose((xd, "XTestFakeKeyEvent (%p, %d, %d, %d ))\n",
		    (void*) xd->fake, 
		    (int) xss->kc.kc, 
		    (int) bo, 
		    (int) dtime));
      XTestFakeKeyEvent (xd->fake, xss->kc.kc, bo, CurrentTime);
      XFlush(xd->fake);
    }

  for (i=0; i<size ; i++)
    {
      XTestGrabControl (xd->distr_list[i].dpy, True); 
      for (mods=0;(mods<8)&&(xss->kc.mod_keycodes[mods]!=0);mods++)
	{
	  xnee_verbose((xd, "XTestFakeKeyEvent modifier \n"));
	  xnee_fake_key_event (xd,
			       xss->kc.mod_keycodes[mods], 
			       True, 
			       0);
	}
      xnee_verbose((xd, "XTestFakeKeyEvent (%p, %d, %d, %d )) **\n",
		    (void*) xd->distr_list[i].dpy, 
		    (int) xss->kc.kc, 
		    (int) bo, 
		    (int) dtime));
      XTestFakeKeyEvent (xd->distr_list[i].dpy, xss->kc.kc, bo, dtime);
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
xnee_fake_button_event_impl (xnee_data* xd, 
			     int button, 
			     Bool bo , 
			     int dtime,
			     int deviceid)
{
  int i=0;
  int size= xd->distr_list_size;
  
#ifdef XNEE_XINPUT_SUPPORT
	  XDevice *xdevice;
#endif /* XNEE_XINPUT_SUPPORT*/

  if (!xnee_is_recorder (xd))
    {
      if (xnee_is_swinput_playback(xd))
	{
	  fprintf (stdout, "fake swinput\n");
	  fprintf (stderr, "fake swinput\n");
	}
      else if (deviceid != 0 )
	{
#ifdef XNEE_XINPUT_SUPPORT
	  XDevice *xdevice;
	 
	  xdevice = xnee_get_xinput_device(xd, deviceid);

	  xnee_fake_sleep (dtime); 
	  XTestFakeDeviceButtonEvent(xd->fake, 
				     xdevice, 
				     button, 
				     bo, 
				     xnee_xi_last_know_pointer_pos,
				     2,
				     1);
#endif /* XNEE_XINPUT_SUPPORT*/
	}
      else
	{
	  xnee_fake_sleep (dtime); 
	  xnee_verbose((xd, "XTestFakeButtonEvent (%p, %d, %d, %d)) \n",
			(void*) xd->fake, 
			(int) button, 
			(int) bo, 
			(int) dtime));
	  XTestFakeButtonEvent (xd->fake, button, bo, 0);
	}  
      
      for (i=0; i<size ; i++)
	{
	  XTestGrabControl (xd->distr_list[i].dpy, True); 
	  xnee_verbose((xd, "XTestFakeButtonEvent (%p, %d, %d, %d))  **\n",
			(void*) xd->distr_list[i].dpy, 
			(int) button, 
			(int) bo, 
			(int) dtime));
	  XTestFakeButtonEvent (xd->distr_list[i].dpy, button, bo, dtime);
	  XFlush (xd->distr_list[i].dpy);
	}
      XFlush(xd->fake);
      
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
xnee_fake_motion_event_impl (xnee_data* xd,
			     int screen, 
			     int x, 
			     int y, 
			     unsigned long dtime,
			     int deviceid)
{
  int i=0;
  int size= xd->distr_list_size;
  int new_x;
  int new_y;
#ifdef XNEE_XINPUT_SUPPORT
  int axes[2];
  XDevice *xdevice;
#endif /* XNEE_XINPUT_SUPPORT*/

  xnee_verbose((xd, "---> xnee_fake_motion_event\n"));
  xnee_verbose((xd, "---  delay = %d\n", (int)dtime));

  if (!xnee_is_recorder (xd))
    {

      new_x = xnee_resolution_newx(xd,x) + xd->res_info.x_offset;
      new_y = xnee_resolution_newy(xd,y) + xd->res_info.y_offset;
      
      if (xnee_is_swinput_playback(xd))
	{
	  fprintf (stdout, "fake swinput\n");
	  fprintf (stderr, "fake swinput\n");
	}
      else if (deviceid == 0 )
	{
	  xnee_fake_sleep (dtime);
	  xnee_verbose((xd, "XTestFakeMotionEvent (%p, %d, %d, %d, %d))\n",
			(void*) xd->fake, 
			(int) screen, 
			(int) new_x,
			(int) new_y,
			0));
	  XTestFakeMotionEvent(xd->fake, 
			       screen, 
			       new_x, 
			       new_y, 
			       0);
	}
      else
	{

#ifdef XNEE_XINPUT_SUPPORT
	  axes[0] = new_x;
	  axes[1] = new_y;
	  xnee_verbose((xd, "---  xi find device\n"));
	 
	  xdevice = xnee_get_xinput_device(xd, deviceid);

	  xnee_verbose((xd, "---  xi devices found? \n"));

	  if (xdevice==NULL)
	    {
	      fprintf(stderr, 
		      "Couldn't find device for device id: %d\n",
		      deviceid);
	      return XNEE_XINPUT_EXTENSION_FAILURE;
	    }
	  
	  xnee_fake_sleep (dtime);

	  xnee_verbose((xd, "XTestFakeDeviceMotionEvent (%d, %d, %d, {%d, %d}, %d, %d))\n",
			(int) xd->fake, 
			(int) xdevice,
			(int) False,
			(int) new_x,
			(int) new_y,
			2,0));

	  xnee_xi_last_know_pointer_pos[0]=axes[0];
	  xnee_xi_last_know_pointer_pos[1]=axes[1];
	  XTestFakeDeviceMotionEvent(xd->fake, 
				     xdevice,
				     False,
				     0,
				     axes,
				     2,
				     1);

#endif /* XNEE_XINPUT_SUPPORT*/
	}
      
      for (i=0; i<size ; i++)
	{
	  /*       XTestGrabControl (xd->distr_list[i].dpy, True);  */
	  
	  xnee_verbose((xd, "XTestFakeMotionEvent (%p, %d, %d, %d, %d))  **\n",
			(void*) xd->distr_list[i].dpy, 
			(int) 0, 
			(int) x,
			(int) y,
			10));
	  XTestFakeMotionEvent(xd->distr_list[i].dpy, 
			       0, 
			       x, 
			       y, 
			       CurrentTime);
	  XFlush (xd->distr_list[i].dpy);
	  
	}
      
      
      XFlush(xd->fake);
    }

  
  
  xnee_verbose((xd, " <------- xnee_fake_motion_event\n"));
  return (XNEE_OK);
}

/**************************************************************
 *                                                            *
 * xnee_fake_relative_motion_event                            *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_fake_relative_motion_event (xnee_data* xd,
				 int x, 
				 int y, 
				 unsigned long dtime)
{
  int i=0;
  int size= xd->distr_list_size;

  xnee_verbose((xd, "---> xnee_fake_relative_motion_event\n"));
  if (!xnee_is_recorder (xd))
    {
      xnee_fake_sleep (dtime);
      xnee_verbose((xd, "XTestFakeRelativeMotionEvent (%p, %d, %d, %d))\n",
		    (void*) xd->fake, 
		    (int) x,
		    (int) y,
		    0));
      XTestFakeRelativeMotionEvent(xd->fake, 
			   x, 
			   y, 
			   0);
      XFlush(xd->fake);
    }
  
  for (i=0; i<size ; i++)
    {
      XTestGrabControl (xd->distr_list[i].dpy, True); 

      xnee_verbose((xd, "XTestFakeRelativeMotionEvent (%p, %d, %d, %d))  **\n",
		   (void*) xd->distr_list[i].dpy, 
		   (int) x,
		   (int) y,
		    10));

      XTestFakeRelativeMotionEvent(xd->distr_list[i].dpy, 
			   x, 
			   y, 
			   CurrentTime);
      XFlush (xd->distr_list[i].dpy);
      
    }
  xnee_verbose((xd, " <------- xnee_fake_relative_motion_event\n"));
  return (XNEE_OK);
}

int 
xnee_type_file(xnee_data *xd)
{
  char tmp[256]="" ;
  int i;

  
  xnee_script_s xss;

  xnee_verbose ((xd,"---> xnee_type_file\n"));

  xnee_setup_display (xd);
  xnee_replay_init (xd);   

  xnee_verbose ((xd,"--- xnee_type_file\n"));

  if (!xnee_has_xtest_extension(xd))
    {
      exit(XNEE_NO_TEST_EXT);
    }
  
  xnee_verbose ((xd,"--- xnee_type_file\n"));

  while (fgets(tmp, 256, xd->rt_file)!=NULL)
    {
      xnee_verbose ((xd,"  xnee_type_file loop read size=" SIZE_T_PRINTF_FMT " \"%s\"\n", 
		     strlen(tmp),tmp));
      
      for ( i=0 ; (size_t)i<strlen(tmp) ; i++ )
	{
	  xnee_char2keycode(xd, tmp[i], &xss.kc); 
	  
	  
	  xnee_verbose ((xd, "retyping key %c keycode %d\n", 
			 tmp[i],xss.kc.kc));
	  
	  xnee_fake_key_mod_event (xd, &xss, XNEE_PRESS, 0);

 	  usleep ( 1000 * xnee_get_retype_press_delay(xd)); 

	  xnee_fake_key_mod_event (xd, &xss, XNEE_RELEASE, 0);
 	  usleep ( 1000 * xnee_get_retype_release_delay(xd)); 
	  
	}
    }
  xnee_verbose ((xd,"<--- xnee_type_file\n"));
  return (XNEE_OK);
}
