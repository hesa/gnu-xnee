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



/* 
 * internal use only 
 */
/**************************************************************
 *                                                            *
 * xnee_replay_buffer_status                                  *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_replay_buffer_status (xnee_data* xd, int data_type, int nr);
int
xnee_replay_buffer_max_diff (xnee_data* xd, int type);
int
xnee_replay_buffer_min_diff (xnee_data* xd, int type);
int
xnee_replay_buffer_tot_diff (xnee_data* xd, int type);

void 
xnee_replay_m_delay (unsigned long usecs)
{
  usleep ( usecs ) ;
}



/*
 *  internal use only 
 */
unsigned long 
xnee_delta_time ( xnee_intercept_data * xindata)
{
  unsigned int new ; 
  unsigned int old ; 
  new = xindata->newtime; 
  old = xindata->oldtime; 
  
  if ( new > old ) 
    {
      return (new-old); 
    }
  else 
    {
      return 0 ;
    }
}


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
      xnee_verbose ((xd, "\txd->first_replayed_event==1  ----> dtime1=10 ; \n"));
      record_last_diff = 10 * 1000; 
    }

  sleep_amt = xnee_calc_sleep_amount( xd, last_diff, first_diff, 
				      record_last_diff, record_first_diff )  / 1000 ; 

  xnee_verbose((xd, "---  xnee_replay_event_handler \n "));


  xnee_verbose ((xd, "switching type: %d sleep_amt: %d\n", xindata->u.event.type, sleep_amt ));
  fflush( NULL );
  xnee_verbose((xd, "---> xnee_replay_event_handler switching type%d\n", xindata->u.event.type));
  
  /* AIX's XTestFakeKeyEvent has a bug with the last argument 
   *   requires 0 - so we use usleep instead 
   *  
   * XTEST_ERR_SLEEP is a macro for usleep on AIX
   */
  switch (xindata->u.event.type)
    {
    case KeyPress:
      xnee_verbose((xd, "XTestFakeKeyEvent (%d, %d, %d, %d ))\n",
		   (int) xd->fake, 
		   (int) xindata->u.event.keycode, 
		   (int) True, 
		   (int) sleep_amt));
      XTEST_ERR_SLEEP ( sleep_amt );  
      XTestFakeKeyEvent   (xd->fake, xindata->u.event.keycode, True, sleep_amt);
      xnee_fake_key_event (xd, xindata->u.event.keycode, True, CurrentTime   );
      break;
    case KeyRelease:
      xnee_verbose((xd, "XTestFakeKeyEvent (%d, %d, %d, %d ))\n",
		   (int) xd->fake, 
		   (int) xindata->u.event.keycode, 
		   (int) False, 
		   (int) sleep_amt));
      XTEST_ERR_SLEEP ( sleep_amt );  
      XTestFakeKeyEvent (xd->fake, xindata->u.event.keycode, False, sleep_amt);
      xnee_fake_key_event  (xd, xindata->u.event.keycode, False, CurrentTime);
      break;
    case ButtonPress:
      xnee_verbose((xd, "XTestFakeButtonEvent (%d, %d, %d, %d))\n",
		   (int) xd->fake, 
		   (int) xindata->u.event.button, 
		   (int) True, 
		   (int) sleep_amt));
      XTEST_ERR_SLEEP ( sleep_amt );   
      XTestFakeButtonEvent (xd->fake, xindata->u.event.button, True, sleep_amt);
      xnee_fake_button_event (xd, xindata->u.event.button, True, CurrentTime);
      printf ("\n\n\tPRESS\n\n\n");
      break;
    case ButtonRelease:
      xnee_verbose((xd, "XTestFakeButtonEvent (%d, %d, %d, %d))\n",
		   (int) xd->fake, 
		   (int) xindata->u.event.button, 
		   (int) False, 
		   (int) sleep_amt));
      XTEST_ERR_SLEEP ( sleep_amt );  
      XTestFakeButtonEvent (xd->fake, xindata->u.event.button, False, sleep_amt);
      xnee_fake_button_event (xd, xindata->u.event.button, False, CurrentTime);
      break;
    case MotionNotify:
      screen = xindata->u.event.screen_nr ; 
      x = (int) xindata->u.event.x ; 
      y = (int) xindata->u.event.y ; 
      
      xnee_correct_time(&sleep_amt);
      xnee_verbose((xd, "XTestFakeMotionEvent (%d, %d, %d, %d, %lu))\n",
		   (int) xd->fake, 
		   (int) screen, 
		   (int) x,
		   (int) y,
		   sleep_amt));
      XTestFakeMotionEvent(xd->fake, 
			   screen, 
			   x, 
			   y, 
			   sleep_amt);
      xnee_fake_motion_event (xd,
			      0,
			      x, 
			      y, 
			      CurrentTime);
      XTEST_ERR_SLEEP ( sleep_amt );  
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
 * xnee_replay_synchronize                                    *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_replay_synchronize (xnee_data* xd)
{
  int type;
  int i ; 
  int diff;
  int count = 0;
  static int time_outs;

  xnee_verbose((xd,"---> xnee_replay_synchronize \n"));;
  if ( xd->sync == False ) 
    {
      xnee_verbose((xd, "Xnee in NO SYNC mode\n"));
      return ;
    }
   
  /*
   *
   * Check to see if we are in sync
   *
   */
  xnee_verbose ((xd, "   synchronize: entering sync test loop\n"));

  printf ("=========================================================\n");
  while (1)
    {
      /*
       * Handle all pending data from X/RECORD 
       */
      diff = xnee_process_replies(xd);

      /* 
       * make sure that the cached values are valid
       */
      diff=xnee_update_buffer_cache(xd);
     
      /*
       * check the buffer limits ....
       */
      diff=xnee_check_buffer_limits(xd);
      if (diff!=0)
	{
	  printf ("\t\tdiff = %d  count=%03d  button %d\n", 
		  diff , count++, xd->button_pressed) ; 
	  if (xd->button_pressed)
	    exit(0);
	}
      else
	break;
    }      
  
  xnee_replay_printbuffer(xd);
  xnee_verbose((xd,"<--- xnee_replay_synchronize \n"));
}



/**************************************************************
 *                                                            *
 * xnee_replay_buffer_handler                                 *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_replay_buffer_handler (xnee_data* xd, 
			    int data_type, 
			    int data_nr,
			    Bool rec_or_rep)
{
  xnee_verbose((xd,"---> xnee_replay_buffer_handler "));
  if (rec_or_rep==XNEE_RECEIVED) 
    xnee_verbose((xd," XNEE_RECEIVED \n"));
  else
    xnee_verbose((xd," XNEE_REPLAYED \n"));

  /*
   * protect the buffer by waiting for the buffer semaphore
   */
  xnee_sem_wait (xd, xd->buf_sem);


  /*
   * From where did we get the data?
   * 
   * If rec_or_rep equals:
   *    XNEE_RECEIVED     from the Xserver .... decrement
   *    XNEE_REPLAYED     from file ..... increment 
   *
   */
  if (rec_or_rep==XNEE_RECEIVED) 
    {
      if (!(  
	    ( data_type == XNEE_EVENT ) && 
	    ( data_nr   <= ButtonRelease ) ))
	{
	  if (data_nr==161)
	    {
	      fprintf (stderr, 
		       "161 received:This is workaround for a bug in Xnee\n");
	      
	      /*
	       * release the protecting semaphore the buffer
	       */
	      xnee_sem_post (xd, xd->buf_sem);
	      return;
	    }

	  /* 
	   * If we get a data such that its amount of 
	   * unsynced data (xd->data_buffer[data_type][data_nr]) 
	   * is the same as the cached maximum value for 
	   * the buffer we can't assure that the cached
	   * maximum value is valid, since we are going to 
	   * decrement the value. Set the cache to -1
	   */
	  if (xd->meta_data.cached_max == 
	      xd->data_buffer[data_type][data_nr])
	    {
	      /* don't get it .... please the above read again */
	      xd->meta_data.cached_max=-1;
	    }
	  
	  /*
	   * decrement the data buffer
	   * decrement the total buffer
	   */
	  xd->data_buffer[data_type][data_nr]--;
	  xd->meta_data.total_diff--;
	  xd->meta_data.sum_min--;

	  /*
	   * If the value for this data is smaller than the cached 
	   * minimum value. Set this value in the cache 
	   */
	  if ( xd->data_buffer[data_type][data_nr] < 
	       xd->meta_data.cached_min)
	    {
	      xd->meta_data.cached_min = 
		xd->data_buffer[data_type][data_nr] ;
	    }
	  
	  
	  XNEE_SYNC_DEBUG ( (stdout, "--------       [%d][%d]    %d   \n", 
			     data_type, nr, nr_of_data))  ;
	  xnee_verbose ((xd, "   buffer_handler  %d %d  to %d\n" ,
			data_type,data_nr, xd->data_buffer[data_type][data_nr]));
	}
    }
  else 
    {

      /* 
       * If we get a data such that its amount of unsynced data 
       * (xd->data_buffer[data_type][data_nr]) is the same as the 
       * cached minimum value for the buffer we can't assure that the cached
       * minimum value is valid, sice we are going to decrement the value. 
       * Set the cache to +1
       */
      if (xd->meta_data.cached_min == 
	  xd->data_buffer[data_type][data_nr])
	{
	  xd->meta_data.cached_min=+1;
	}
      
      /*
       * increment the data  buffer
       * increment the total buffer
       */
      xd->data_buffer[data_type][data_nr]++;
      xd->meta_data.total_diff++;
      xd->meta_data.sum_max++;
      
      /*
       * If the value for this data is bigger than the cached maximum value. 
       * Set this value in the cache 
       */
      if ( xd->data_buffer[data_type][data_nr] > 
	   xd->meta_data.cached_max)
	{
	  xd->meta_data.cached_max = 
	    xd->data_buffer[data_type][data_nr] ;
	}
      
      xnee_verbose ((xd, "   buffer_handler incremented   %d %d  to %d \n", 
		     data_type, data_nr, 
		     xd->data_buffer[data_type][data_nr] ));
    }
  
  /*
   * release the protecting semaphore the buffer
   */
  xnee_sem_post (xd, xd->buf_sem);
  
  xnee_replay_printbuffer(xd);
  xnee_verbose((xd,"<--- xnee_replay_buffer_handler \n"));;
}




/**************************************************************
 *                                                            *
 * xnee_replay_read_protocol                                  *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_replay_read_protocol (xnee_data* xd, xnee_intercept_data * xindata) 
{
  char tmp[256] ;
  int eofile = 0 ;


  strcpy(tmp,"");
  if ( xd->data_file == NULL)
    {
      xnee_verbose((xd, "Using stdin as file\n"));
      xd->data_file=stdin;
    }
  fgets(tmp, 256, xd->data_file);

  if ( tmp == NULL) 
    {
      return XNEE_OK;
    }
  if (!strncmp("0",tmp,1))  /* EVENT */
    {    
      xindata->oldtime = xindata->newtime ;
      eofile = sscanf(tmp, "%d,%d,%d,%d,%d,%d,%d,%lu",
		      &xindata->type, 
		      &xindata->u.event.type, 
		      &xindata->u.event.x,
		      &xindata->u.event.y, 
		      &xindata->u.event.button,
		      &xindata->u.event.keycode, 
		      &xindata->u.event.screen_nr,
		      &xindata->newtime);
      if (eofile < 8)    /* NUM ARGS */
	{
	  (void)xnee_print_error("Error in file %s \n", xd->data_file);
	  eofile = 0 ;
	}   
    }   
  else if (!strncmp("1",tmp,1))  /* REQUEST */
    {
      xindata->oldtime = xindata->newtime ;
      eofile = sscanf(tmp, "%d,%d,%lu",
		      &xindata->type, 
		      &xindata->u.request.type,
		      &xindata->newtime);
      if (eofile < 3)
	{
	  (void)xnee_print_error("Error in file %s \n", xd->data_file);
	  eofile = 0;
	}   
    }
  else if (!strncmp("2",tmp,1)) /* REPLY */
    {
      xindata->oldtime = xindata->newtime ;
      eofile = sscanf(tmp, "%d,%d,%lu",
		      &xindata->type, 
		      &xindata->u.reply.type,
		      &xindata->newtime);
      if (eofile < 3)
	{
	  (void)xnee_print_error("Error in file %s \n", xd->data_file);
	  eofile = 0;
	}   
    }
  else if (!strncmp("3",tmp,1))  /* ERROR */
    {
      xindata->oldtime = xindata->newtime ;
      eofile = sscanf(tmp, "%d,%d,%lu",
		      &xindata->type, 
		      &xindata->u.error.type,
		      &xindata->newtime);
      if (eofile < 3)
	{
	  (void)xnee_print_error("Error in file %s \n", xd->data_file);
	  eofile = 0;
	} 
    }  
  else if (!strncmp("#",tmp,1))  /* # META data */
    {
      xnee_handle_meta_data(xd, tmp);
      eofile=XNEE_META_DATA;
    }
  else 
    {
      xnee_verbose((xd,"Corrupt line: \"%s\"\n", tmp)); 
      eofile=0;
   }      

  return eofile;
}





/**************************************************************
 *                                                            *
 * xnee_handle_meta_data_sub1                                 *
 *                                                            *
 *                                                            *
 **************************************************************/
void
xnee_handle_meta_data_sub1(int *dest, char *src_str, int comp_str_size)
{
  int val;
  

  if ( ! sscanf ( src_str + comp_str_size + 1 , "%d", &val)  ) 
    {
      xnee_print_error ("Could not read value for argument (for \"%s\" from file)\n", src_str);
    }
  else
    {
      *dest = val;
    }
}





/**************************************************************
 *                                                            *
 * xnee_handle_meta_data                                      *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_handle_meta_data(xnee_data* xd, char * str)
{

  xnee_verbose((xd, "xnee_handle_meta_data     (xd, \"%s\");\n", str));

  xnee_rem_comment_start (xd, str);

  xnee_verbose((xd, "xnee_handle_meta_data (2) (xd, \"%s\");\n", str));

  rem_all_blanks (str, strlen(str));

  xnee_verbose((xd, "xnee_handle_meta_data (3) (xd, \"%s\");\n", str));

  if (strncmp (str, XNEE_ALL_EVENTS, strlen (XNEE_ALL_EVENTS)) == 0  )
    {
      xnee_handle_meta_data_sub1 (&xd->xnee_info->all_events, str,  strlen(XNEE_ALL_EVENTS) );
    }
  else if (strncmp (str, XNEE_EVERYTHING, strlen (XNEE_EVERYTHING)) == 0  )
    {
      xnee_handle_meta_data_sub1 (&xd->xnee_info->everything, str,  strlen(XNEE_EVERYTHING) );
    }
  else if (strncmp (str, XNEE_LOOPS_LEFT, strlen (XNEE_LOOPS_LEFT)) == 0  )
    {
      xnee_handle_meta_data_sub1 (&xd->xnee_info->loops_left, str,  strlen(XNEE_LOOPS_LEFT) );
    }
  else if (strncmp (str, XNEE_NO_EXPOSE, strlen (XNEE_NO_EXPOSE)) == 0  )
    {
      xnee_handle_meta_data_sub1 (&xd->xnee_info->no_expose, str,  strlen(XNEE_NO_EXPOSE) );
    }
  else if (strncmp (str, XNEE_NO_EXPOSE, strlen (XNEE_NO_EXPOSE)) == 0  )
    {
      xnee_handle_meta_data_sub1 (&xd->xnee_info->no_expose, str,  strlen(XNEE_NO_EXPOSE) );
    }
  else if ( 
	   (strncmp (str, XNEE_REQUEST_STR, strlen (XNEE_REQUEST_STR)) == 0  )
	   || 
	   (strncmp (str, XNEE_DELIVERED_EVENT_STR, strlen (XNEE_DELIVERED_EVENT_STR)) == 0  )
	   || 
	   (strncmp (str, XNEE_ERROR_STR, strlen (XNEE_ERROR_STR)) == 0  )
	   || 
	   (strncmp (str, XNEE_REPLY_STR, strlen (XNEE_REPLY_STR)) == 0  )
	   || 
	   (strncmp (str, XNEE_EXT_REQ_MAJ_STR, strlen (XNEE_EXT_REQ_MAJ_STR)) == 0  )
	   || 
	   (strncmp (str, XNEE_EXT_REQ_MIN_STR, strlen (XNEE_EXT_REQ_MIN_STR)) == 0  )
	   || 
	   (strncmp (str, XNEE_EXT_REP_MAJ_STR, strlen (XNEE_EXT_REP_MAJ_STR)) == 0  )
	   || 
	   (strncmp (str, XNEE_EXT_REP_MIN_STR, strlen (XNEE_EXT_REP_MIN_STR)) == 0  )
	   || 
	   (strncmp (str, XNEE_STOP_KEY, strlen (XNEE_STOP_KEY)) == 0  )
	   )
    {
      xnee_add_resource_syntax(xd, str);
    }
  else if (strncmp (str, XNEE_DEVICE_EVENT_STR, strlen (XNEE_DEVICE_EVENT_STR)) == 0 )
    {
      xnee_verbose((xd, " Skipping :\"%s\"\n", str));
    }
  else if ( strncmp (str, XNEE_META_END, strlen (XNEE_META_END)) == 0  )
    {
      /* once the meta end line is read - temp stop processing file until XRecord Async callback set up*/
      return XNEE_FALSE;
    }
  else 
    {
      xnee_verbose((xd, " Skipping :\"%s\"\n", str));
      ; /* TODO .... */
    }

  /*
  xnee_print_xnee_settings (xd, xd->err) ;
  xnee_record_print_record_range (xd, xd->err) ;
  fprintf (stderr, "Press enter to continue:\n");
  getchar();
  */
  return XNEE_TRUE;
}



int
print_time()
{
  struct timeval tv;
  gettimeofday(&tv, NULL); 
  printf ("%lu:%lu\n", tv.tv_sec, tv.tv_usec);
  return XNEE_OK;
}


/**************************************************************
 *                                                            *
 * xnee_replay_main_loop                                      *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_replay_main_loop(xnee_data *xd)
{
  xnee_intercept_data xindata ;
  int logread = 1 ;
  int replayable ;
  static int last_logread=1;
  int last_elapsed = 0;
  XEvent ev;
  xindata.oldtime = xindata.newtime ;

  /* First of all ... read up the META DATA 
   * so we know our settings */
  while (logread && xd->cont) 
    {
      logread = xnee_replay_read_protocol(xd, &xindata);

      if (!logread)
	{
	  fprintf (stderr, "Could not read data from file\n");
	  xnee_close_down (xd);
	}	
      else if ( logread != XNEE_META_DATA )
	{
	  xnee_record_print_record_range (xd, NULL);
	  xnee_print_xnee_settings       (xd, NULL); 
	  
	  if (xd->sync)
	    {
	      xnee_setup_rep_recording(xd);
	    }
	  xnee_print_sys_info(xd , xd->out_file);
	  xnee_verbose((xd, "META DATA finished .... \n"));
	  break ; 
	}
    }
	 



  
  /**
   * all META DATA setting up our sessions is read ...
   * go on replaying
   *
   * 
   *       Think of this as the main loop
   */
  while (logread && xd->cont) 
    {
      static int hesa=0;
      if (!last_logread==XNEE_META_DATA)
	{
	  /* 
	   * set value for forthcoming time calculations 
	   * Now replay starts off
	   */
	  last_elapsed = xnee_get_elapsed_time(xd, XNEE_FROM_LAST_READ );
	}
      logread = xnee_replay_read_protocol(xd, &xindata);

      if ( logread == XNEE_META_DATA )
	{
	  xnee_verbose((xd, "META DATA read ... should be handled in the future... eg script ????\n"));
	}
      else if (logread)
	{
	  
	  if (xnee_check_km (xd)==XNEE_GRAB_DATA)
	    {
	      printf ("\n\nsomeone grabbed us\n\n");
	    }
	    
	  switch (xindata.type)
	    {
	    case XNEE_EVENT:

	      if ( ( xindata.u.event.type >= KeyPress ) 
		   && (xindata.u.event.type <= MotionNotify) )
		{
		  
		  xnee_replay_synchronize (xd);

		  if ( xindata.u.event.type == ButtonPress )
		    {
		      xd->button_pressed++;
		    }
		  else if ( xindata.u.event.type == ButtonRelease )
		    {
		      xd->button_pressed--;
		    }
		  if (xd->button_pressed==0)
		    {
		      xnee_replay_synchronize (xd);
		    }
		  replayable = 
		    xnee_replay_event_handler(xd, 
					      &xindata, last_elapsed);
		  
		}
	      else
		{
		  XNEE_SYNC_DEBUG ( (stderr, 
				     "SYNC     EVENT   %d\n", 
				     xindata.u.event.type ) );
		  xnee_replay_buffer_handler ( 
					      xd, 
					      XNEE_EVENT, 
					      xindata.u.event.type, 
					      XNEE_REPLAYED);
		}
	      break;
	    case XNEE_REQUEST:
	      xnee_verbose((xd, "READ A REQUEST, %d\n", xindata)); 
	      xnee_replay_buffer_handler ( xd, 
					   XNEE_REQUEST, 
					   xindata.u.request.type, 
					   XNEE_REPLAYED);
	    case XNEE_REPLY:
	      xnee_verbose((xd, "READ A REPLY, %d\n", xindata)); 
	      xnee_replay_buffer_handler ( 
					  xd, 
					  XNEE_REPLY, 
					  xindata.u.reply.type, 
					  XNEE_REPLAYED);
	      break;
	    default:
	      xnee_verbose((xd, 
			    "xnee_replay_MainReplayLoop: Unknown type \n"));
	      break;
	    }
	}
      else 
	{
	  break;
	}


      XSync(xd->control, False);
      xnee_verbose((xd, "Flushing after handled event\n"));
      XFlush(xd->control);
      xnee_verbose((xd, "  <-- Flushed after handled event\n"));
      last_logread = logread;
    }
}





/**************************************************************
 *                                                            *
 * xnee_setup_rep_recording                                   *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_setup_rep_recording(xnee_data *xd)
{
  int nr_of_ranges=0;
  xnee_recordext_setup  *xrs       = xd->record_setup;

  xnee_verbose((xd, "--->xnee_setup_rep_recording\n"));             
  nr_of_ranges=xnee_get_max_range(xd);

  if (xd->all_clients)
    {
      xrs->xids[0] = XRecordAllClients;
    }
  else
    {
      xrs->xids[0] = XRecordFutureClients; 
    }

  xnee_verbose((xd, "\t  CreateContext   nr_of_ranges=%d\n", nr_of_ranges));  

  xrs->rContext = XRecordCreateContext(xd->control, 
				       xrs->data_flags, 
				       xrs->xids, 1, 
				       xrs->range_array,nr_of_ranges );

  xnee_verbose((xd, "\t  CreateContext   0x%lx\n", xrs->rContext));  
  XRecordRegisterClients   (xd->control,
			    xrs->rContext,
			    xrs->data_flags, 
			    xrs->xids,1,
			    xrs->range_array,nr_of_ranges);    

  xrs->xids[0] = xnee_client_id (xd->control);
  xrs->xids[1] = xnee_client_id (xd->data);

  XRecordUnregisterClients( xd->control, 
			    xrs->rContext,
			    xrs->xids,
			    2);

  if(!XRecordGetContext(xd->control, xrs->rContext, (XRecordState **) xrs->rState))
  {
    xnee_print_error ("\n Couldn't get the context information for Display %d\n", (int) xd->control) ;
    exit(1);
  }
  
  xnee_verbose((xd, "\t  GetContext      0x%lx (%d clients intercepted))\n", 
		xrs->rContext, (int) ( (xrs->rState) - (xrs->nclients) ) ));   
  
  XRecordFreeState(xrs->rState); 
  xd->record_setup->rState=NULL; 

  /*
  xnee_verbose((xd, "XSync()\n"));
  XSync  (xd->control, False); 
  xnee_verbose((xd, "XFlush()\n"));
  XFlush (xd->control); 
  XSynchronize(xd->data, True);  
  */

  
  if (xd->xnee_info->interval != 0)
    {
      xnee_delay (xd->xnee_info->interval, "" );
    }


  /* Enable context for async interception */
  XRecordEnableContextAsync (xd->data, 
			     xrs->rContext, 
			     xnee_replay_dispatch, 
			     (XPointer) (xd) /* closure passed to Dispatch */);
  xnee_verbose((xd, " 1.3\n"));             
  xnee_verbose((xd, "finished setting up record for replaying\n"));
  xnee_verbose((xd, "<---xnee_setup_rep_recording\n"));             
  
  return (0);
}






/**************************************************************
 *                                                            *
 * xnee_replay_dispatch                                       *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_replay_dispatch (XPointer type_ref, XRecordInterceptData *data)
{
  XRecordDatum *xrec_data;
  int           type;
  xnee_data    *xd;
  
  if (!data->data)
    {
      return;
    }

  xrec_data  = (XRecordDatum *) (data->data) ;
  type       = xrec_data->type ;
  xd         = (xnee_data*) (type_ref);

  switch(data->category)
    {
    case XRecordFromClient:
      XNEE_SYNC_DEBUG ( (stderr, "DISPATCH REQUEST %d \n",type  ) );
      xnee_verbose((xd, "GOT A REQUEST:         %d \n ", type));
/*       xnee_replay_buffer_handle (xd, XNEE_REQUEST, type, XNEE_RECEIVED); */
      xnee_replay_buffer_handler (xd, XNEE_REQUEST, type, XNEE_RECEIVED); 

      break;
    case XRecordFromServer:
      if(type >  X_Reply )
	{
	  XNEE_SYNC_DEBUG ( (stderr, "DISPATCH EVENT   %d \n",type  ) );
	  xnee_verbose((xd, "GOT A EVENT:       %d \n", type));
/* 	  xnee_replay_buffer_handle (xd, XNEE_EVENT, type, XNEE_RECEIVED); */
 	  xnee_replay_buffer_handler (xd, XNEE_EVENT, type, XNEE_RECEIVED); 
	}
      else 
	{
	  XNEE_SYNC_DEBUG ( (stderr, "DISPATCH type=%d  REPLY \n",type  ) );
	  xnee_verbose((xd, "GOT A REPLY:       %d \n", type));
/* 	  xnee_replay_buffer_handle (xd, XNEE_REPLY, type, XNEE_RECEIVED); */
 	  xnee_replay_buffer_handler (xd, XNEE_REPLY, type, XNEE_RECEIVED); 
	}
      break;   
    case XRecordClientStarted:
      xnee_verbose((xd,  "ClientStarted \n"));
      break;
    case XRecordClientDied:
      xnee_verbose((xd,  "ClientDied \n"));
      break;
    case XRecordEndOfData:
      xnee_verbose((xd,  "EndOfData \n"));
      break; 
    default:
      xnee_print_error( "Case: Default reached in Dispatch (...) \n");
      break;  
    } 
  XRecordFreeData(data);
}






/**************************************************************
 *                                                            *
 * xnee_replay_buffer_status                                  *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_replay_buffer_status (xnee_data* xd, int data_type, int nr)
{
  return xd->data_buffer[data_type][nr];
}



/**************************************************************
 *                                                            *
 * xnee_replay_buffer_max_diff                                *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_replay_buffer_max_diff (xnee_data* xd, int type)
{
  int max_val=0;
  int i ; 

  for (i=0;i<XNEE_REPLAY_BUFFER_SIZE;i++)
    {
      max_val = XNEE_MAX ( xd->data_buffer[type][i], max_val);
    }
  return max_val;
}


/**************************************************************
 *                                                            *
 * xnee_replay_buffer_min_diff                                *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_replay_buffer_min_diff (xnee_data* xd, int type)
{
  int min_val=0;
  int i ; 

  for (i=0;i<XNEE_REPLAY_BUFFER_SIZE;i++)
    {
      min_val = XNEE_MIN ( xd->data_buffer[type][i], min_val);
    }
  return min_val;
}


/**************************************************************
 *                                                            *
 * xnee_replay_buffer_tot_diff                                *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_replay_buffer_tot_diff (xnee_data* xd, int type)
{
  int i;
  int tot_val=0;

  for (i=0;i<XNEE_REPLAY_BUFFER_SIZE;i++)
    {
      tot_val += XNEE_ABS(xd->data_buffer[type][i]);
    }
  return tot_val;
}






/**************************************************************
 *                                                            *
 *  OBSOLETE ..... all is done is synchron... function        *
 *                                                            *
 *                                                            *
 * xnee_replay_buffer_handle                                  *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_replay_buffer_handle (xnee_data* xd, int data_type, int nr, Bool rec_or_rep)
{


  /*
   *  int i,j;
   */
  int nr_of_data=0;


  /*
   * protect the buffer by waiting for the buffer semaphore
   */
  xnee_sem_wait (xd, xd->buf_sem);

  /*
   * From where did we get the data?
   * 
   * If rec_or_rep equals XNEE_RECEIVED  from the Xserver .... decrement
   * If not (thats is if XNEE_REPLAYED ) from file ..... increment 
   *
   */
  if (rec_or_rep==XNEE_RECEIVED) 
    {
      if (!    (  
		( data_type==XNEE_EVENT ) && 
		( nr <= ButtonRelease ) ))
	{
	  xd->data_buffer[data_type][nr]--;
	  xnee_verbose ((xd, 
			 "xnee_replay_buffer_handle decremented %d %d to %d\n",
			 data_type,nr,xd->data_buffer[data_type][nr] ));
	}
    }
  else 
    {
      if (! (  
	     ( data_type==XNEE_EVENT ) && 
	     ( nr <= ButtonRelease) ))
	{
	  xd->data_buffer[data_type][nr]++;
	  xnee_verbose ((xd, 
			 "xnee_replay_buffer_handle incremented %d %d to %d\n",
			 data_type, nr, xd->data_buffer[data_type][nr] ));
	}
    }


  /*
   * release the protecting semaphore the buffer
   */
  xnee_sem_post (xd, xd->buf_sem);
  return nr_of_data;
}








/**************************************************************
 *                                                            *
 * xnee_has_xtest_extension                                   *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_has_xtest_extension (xnee_data *xd) 
{
  int ok=1;
  xnee_testext_setup* xrs=xd->replay_setup;
  if( !XTestQueryExtension(xd->control,
			   &xrs->xtest_event_basep,
			   &xrs->xtest_error_basep,
			   &xrs->xtest_version_major,
			   &xrs->xtest_version_minor) )
    {
      xnee_print_error ("XTest extension missing\n");
      ok=0;
    }
  xnee_verbose ((xd, "\t  XTest-\n\t  Release         %d.%d\n", 
		xrs->xtest_version_major,
		xrs->xtest_version_minor));
  return (ok);

}






/**************************************************************
 *                                                            *
 * xnee_replay_init                                           *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_replay_init          (xnee_data* xd, char * name)
{
  int i, j;
  xd->first_replayed_event=1;
  for ( i=0 ; i<4 ; i++) 
    {
      for ( j=0 ; j<XNEE_REPLAY_BUFFER_SIZE ; j++) 
	{
	  xd->data_buffer[i][j]=0;
	  /*	  
		 xnee_verbose((
		 xd, 
		 "xnee_replay_init %d x %d  =%d\n", 
		 i , j, xd->data_buffer[i][j]));
	   */
	}
      xd->meta_data.cached_max=0;
      xd->meta_data.cached_min=0;
    }
  xd->meta_data.total_diff=0;
  xd->meta_data.sum_max=0;
  xd->meta_data.sum_min=0;
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
  for (i=0; i<size ; i++)
    {

      XTestGrabControl (xd->distr_list[i], True); 
      xnee_verbose((xd, "XTestFakeKeyEvent (%d, %d, %d, %d )) **\n",
		   (int) xd->distr_list[i], 
		   (int) keycode, 
		   (int) bo, 
		   (int) dtime));
      XTestFakeKeyEvent (xd->distr_list[i], keycode, bo, dtime);
      XFlush (xd->distr_list[i]);
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
  for (i=0; i<size ; i++)
    {
      XTestGrabControl (xd->distr_list[i], True); 
      xnee_verbose((xd, "XTestFakeButtonEvent (%d, %d, %d, %d))  **\n",
		   (int) xd->distr_list[i], 
		   (int) button, 
		   (int) bo, 
		   (int) dtime));
      XTestFakeButtonEvent (xd->distr_list[i], button, bo, dtime);
      XFlush (xd->distr_list[i]);
      
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
  xnee_verbose((xd, "---> xnee_fake_motion_event\n"));
  for (i=0; i<size ; i++)
    {
      XTestGrabControl (xd->distr_list[i], True); 

      xnee_verbose((xd, "XTestFakeMotionEvent (%d, %d, %d, %d, %lu))  **\n",
		   (int) xd->distr_list[i], 
		   (int) 0, 
		   (int) x,
		   (int) y,
		   10));
      XTestFakeMotionEvent(xd->distr_list[i], 
			   0, 
			   x, 
			   y, 
			   CurrentTime);
      XFlush (xd->distr_list[i]);
      
    }
  xnee_verbose((xd, " <------- xnee_fake_motion_event\n"));
  return (XNEE_OK);
}






/**************************************************************
 *                                                            *
 * xnee_zero_sync_data                                        *
 *                                                            *
 *                                                            *
 **************************************************************/
void
xnee_zero_sync_data (xnee_data* xd)
{
  ;
}




int 
xnee_update_buffer_cache(xnee_data *xd)
{
  int type ; 
  int counter ; 
  
  
  /*
   * protect the buffer by waiting for the buffer semaphore
   */
  xnee_sem_wait (xd, xd->buf_sem);
  
  for (counter=0 ; counter < MAX_UNSYNC_LOOPS; counter++)
    {
      int diff=0;
      xnee_verbose((xd, "-----------------------------------------\n"));
      for ( type=0 ; type< 2 ; type++) 
	{
	  int real_min=0;
	  int real_max=0;
	  int total_diff=0;
	  
	  xnee_verbose((xd, "     synchronize: checking type:%d\n", type));
	  
	  real_min=xd->meta_data.cached_min;
	  real_max=xd->meta_data.cached_max;
	  
	  
	  /* The cached values might be invalid. 
	   * If so, get fresh value and replace them */
	  if (real_min==1)
	    {
	      xnee_replay_buffer_min_diff (xd,type);
	      real_min = xnee_replay_buffer_min_diff (xd,type);
	      xnee_verbose ((xd, 
			     " cached min value invalid. refreshing to %d   old: %d\n", 
			     real_min,
			     xd->meta_data.cached_min));
	      xd->meta_data.cached_min=real_min;
	    }
	  
	  
	  if (real_max==-1)
	    {
	      xnee_replay_buffer_max_diff (xd,type);
	      real_max = xnee_replay_buffer_max_diff (xd,type);
	      xnee_verbose ((xd, "  cached max value invalid. refreshing to %d   old: %d\n", 
			     real_max,
			     xd->meta_data.cached_max));
	      xd->meta_data.cached_max=real_max;
	    }
	    

	}
    }
  /*
   * release the protecting semaphore the buffer
   */
  xnee_sem_post (xd, xd->buf_sem);
  
  return XNEE_OK;
}




int
xnee_check_buffer_limits (xnee_data *xd)
{
  int cached_min = 0 ;
  int cached_max = 0 ;
  int sum_max    = 0 ;
  int sum_min    = 0 ;
  int tot_diff   = 0 ;
  int diff       = 0 ;

  
  cached_max = xd->meta_data.cached_max;
  cached_min = xd->meta_data.cached_min;
  sum_max    = xd->meta_data.sum_max;
  sum_min    = xd->meta_data.sum_min;
  tot_diff   = xd->meta_data.total_diff;


  xnee_verbose ((xd, "---> xnee_check_buffer_limits  %d\n", 
		 xd->button_pressed));

  if (xd->button_pressed!=0)
    {
      return diff; exit(0);
    }
  xnee_verbose ((xd, "---  %02d %02d %02d %02d %02d \n",
		 cached_max ,
		 cached_min ,
		 sum_max    ,
		 sum_min    ,
		 tot_diff   ));
  

  if ( cached_max > XNEE_BUFFER_MAX) 
    {
      xnee_verbose ((xd, "cached_max %d > XNEE_BUFFER_MAX %d\n",
		     cached_max , XNEE_BUFFER_MAX));
      xnee_replay_printbuffer(xd); 
      diff=1;
    }
  else if ( cached_min < XNEE_BUFFER_MIN) 
    {
      xnee_verbose ((xd, "cached_min %d < XNEE_BUFFER_MIN %d\n",
		     cached_min , XNEE_BUFFER_MIN));
      xnee_replay_printbuffer(xd); 
      diff=1;
    }
  else if ( sum_max > XNEE_BUFFER_SUM_MAX )
    {
      xnee_verbose ((xd, "sum_max %d > XNEE_BUFFER_SUM_MAX %d\n",
		     sum_max , XNEE_BUFFER_SUM_MAX));
      xnee_replay_printbuffer(xd); 
      diff=1;
    }
  else if ( sum_min < XNEE_BUFFER_SUM_MIN )
    {
      xnee_verbose ((xd, "sum_min %d < XNEE_BUFFER_SUM_MIN %d\n",
		     sum_min , XNEE_BUFFER_SUM_MIN));
      xnee_replay_printbuffer(xd); 
      diff=1;
    }
  else if ( tot_diff > XNEE_BUFFER_TOT_MAX )
    {
      xnee_verbose ((xd, "tot_diff %d > XNEE_BUFFER_TOT_MAX %d\n",
		     tot_diff , XNEE_BUFFER_TOT_MAX));
      xnee_replay_printbuffer(xd); 
      diff=1;
    }

  xnee_verbose ((xd, "<--- xnee_check_buffer_limits diff=%d\n", diff));

  usleep (XNEE_MISSING_DATA_DELAY);
  return diff;
}




  /* else
    {
      time_outs -= 1;
      if (time_outs<=0)
	time_outs = 0;
  }
*/

  /*  xnee_verbose((xd," --- xnee_replay_synchronize: sleeping %d\n", 
      XNEE_MISSING_DATA_DELAY));;
      usleep (XNEE_MISSING_DATA_DELAY);
      xnee_verbose((xd," --- xnee_replay_synchronize: .... awoke \n"));;
      xnee_process_replies(xd); 
  */
  /* 
   * Check to see if we are totally out of sync 
   */

  /*if (counter == (MAX_UNSYNC_LOOPS))
    {
    time_outs++;
    if (time_outs>=MAX_SKIPPED_UNSYNC)
    {
    printf ("Error: Xnee can't synchronize anymore. Assuming replay error\n");
    printf ("   timeouts: %d\n", time_outs);
    printf ("   counter : %d\n", counter);
    if (!xd->force_replay)
    {
    xnee_close_down (xd);
    exit (XNEE_SYNCH_FAULT);
    }
    }
    else if (time_outs >= (MAX_SKIPPED_UNSYNC - 10))
    {
    int delay=XNEE_MISSING_DATA_DELAY;
    xnee_process_replies (xd);
    xnee_verbose((xd," --- xnee_replay_synchronize: .... awoke \n"));
    
    if (!xd->force_replay)
    {
    xnee_verbose ((xd, "xnee: increasing timeouts\n"));
    if ( time_outs > ( MAX_SKIPPED_UNSYNC - 2 ))
    {
    delay = XNEE_MISSING_DATA_DELAY * 1000 ;
    }
    if ( time_outs > ( MAX_SKIPPED_UNSYNC - 3 ))
    {
    delay = XNEE_MISSING_DATA_DELAY * 100 ;
    }
    else if ( time_outs > ( MAX_SKIPPED_UNSYNC - 5 ))
    {
    delay = XNEE_MISSING_DATA_DELAY * 100 ;
    }
    }
    xnee_verbose((xd," --- xnee_replay_synchronize: sleeping %d\n", 
    delay));
    usleep (delay);
    }
    }    
  */
