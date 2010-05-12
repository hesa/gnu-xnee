/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003
 *                      2004, 2005, 2006, 2007, 2008
 *                      2009, 2010  Henrik Sandklef              
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
#include "libxnee/xnee_resolution.h"
#include "libxnee/xnee_fake.h"
#include "libxnee/xnee_km.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_resource.h"
#include "libxnee/xnee_time.h"
#include "libxnee/xnee_buffer.h"
#include "libxnee/xnee_threshold.h"
#include "libxnee/xnee_expr.h"
#include "libxnee/xnee_error.h"
#include "libxnee/xnee_session.h"
#include "libxnee/xnee_utils.h"
#include "libxnee/xnee_window.h"
#include "libxnee/xnee_range.h"

static int time_out_counter = 0;
static int diff_counter     = 0;

static int last_diff;
static int last_logread=1;


/*
 *  internal use only 
 */
Time
xnee_delta_time ( xnee_intercept_data * xindata)
{

  if ( xindata->newtime > xindata->oldtime ) 
    {
      return ( xindata->newtime - xindata->oldtime); 
    }
  else 
    {
      return 0 ;
    }
}





/**************************************************************
 *                                                            *
 * xnee_replay_synchronize                                    *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_replay_synchronize (xnee_data* xd)
{
  int diff;

  xnee_verbose((xd,"---> xnee_replay_synchronize sync=%d   FALSE=%d \n",
		xd->sync, XNEE_FALSE));
  if ( xd->sync == XNEE_FALSE ) 
    {
      xnee_verbose((xd, "Xnee in NO SYNC mode\n"));
      return XNEE_OK;
    }

  /*****
   * Check to see if we are in sync
   */
  xnee_verbose ((xd, "   synchronize: entering sync test loop\n"));

  while ( (0==0) )
    {
      /*
       * Handle all pending data from Xserver/RECORD 
       */
      diff = xnee_process_replies(xd);

      /* 
       * make sure that the cached values are valid
       */
      diff = xnee_update_buffer_cache(xd);
     
      /*
       * check the buffer limits ....
       */
      diff = xnee_check_buffer_limits(xd);

      /*
       * handle diff in the buffers .....
       */
      if (diff!=0)
	{
	  if ( (xd->meta_data.cached_min > xnee_get_min_threshold(xd))
	       &&
	       (xd->meta_data.cached_max < xnee_get_max_threshold(xd)))
	    break;

	  diff_counter++;
	  if (diff_counter >= xnee_get_tot_threshold(xd))
	    {
	      time_out_counter++;

	      /*
	       */
	      diff_counter=0;

	      /*
		hmmm don't about that know ..... yet! 
	       */
	      if (time_out_counter > MAX_SKIPPED_UNSYNC) 
		{
		  if (xnee_is_force_replay(xd) != 0 )
                  {
                     break;
                  }
		  (void)fprintf (stderr,
                                 "Can't synchronize anymore .... have to leave!  %d %d \n",
				 time_out_counter , MAX_SKIPPED_UNSYNC);
		  return XNEE_SYNCH_FAULT;
		}
	      else 
		{
		  last_diff = diff;
		  break;
		}
	    }
	  xnee_verbose ((xd, " ...diff => sleeping %d microsecs\n", 
			 XNEE_MISSING_DATA_DELAY ));
          /*@ ignore @*/
	  usleep (XNEE_MISSING_DATA_DELAY );
          /*@ end @*/
	  last_diff = diff;

	}
      else
	{
	  /* Thanks, Valia */ 
/* 	  if (diff_counter!=0) */
/* 	    { */
/* 	      xd->buf_verbose=True; */
/* 	      xnee_replay_printbuffer(xd);  */
/* 	      return XNEE_SYNCH_FAULT; */
/* 	    } */
	  diff_counter=0;
	  time_out_counter=0;
	  break;
	}
    }      
  
  xnee_replay_printbuffer(xd);
  xnee_verbose((xd,"<--- xnee_replay_synchronize \n"));
  return XNEE_OK;
}


/**************************************************************
 *                                                            *
 * xnee_replay_main_loop                                      *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_replay_main_loop(xnee_data *xd, int read_mode)
{
  static char tmp[256] ;
  
/*@reldef@*/
  xnee_intercept_data xindata ;

  int      logread = -1 ;
  int      replayable ;
  long int last_elapsed = 0;
  int      ret = XNEE_OK ; 
  char    *ret_str;
  Display *context_display;

  xindata.oldtime = 0 ;
  xindata.newtime = 0 ;

  xnee_reset_elapsed_time(xd);
  xnee_reset_fake(xd);

  context_display = xnee_get_display_for_recordcontext(xd);

  if ( xd->data_file == NULL)
    {
      xnee_verbose((xd, "Using stdin as file\n"));
      xd->data_file=stdin;
    }
  
  if ( (read_mode==XNEE_REPLAY_READ_META_DATA) || 
       (read_mode==XNEE_REPLAY_READ_ALL_DATA)
       )
    {
       while ( (logread != 0)  && ( xd->cont != 0 ) ) 
	{
	  ret_str = fgets(tmp, 256, xd->data_file);
	  
	  if ( ret_str == NULL)
          {
             ret = -1;
          }
	  else
          {
             ret = (int)strlen (ret_str);
          }

	  if ( ret == -1 )
	    {
	      return XNEE_OK;
	    }	
	  else if ( ret == 0 )
	    {
	      xnee_verbose((xd, "Empty line in data file\n"));
	    }
	  else 
	    {
	      ret = xnee_expression_handle_project(xd, ret_str);
/* 	      ret = xnee_expression_handle_session(xd, ret_str, &xindata); */


      
	      if ( (ret == XNEE_REPLAY_DATA) || 
		   (ret == XNEE_PRIMITIVE_DATA) || 
		   (ret == XNEE_SYNTAX_ERROR) )
		{
		  xnee_verbose((xd, 
				"We are finished reading settings"
				" etc from data file\n"));
		  
		  /* since NULL arg printing is done when in verbose mode */
		  xnee_record_print_record_range (xd, NULL);
		  xnee_print_xnee_settings       (xd, NULL); 
		  
		  xnee_verbose((xd, "REPLAY DATA coming up .... (%d) %s \n",
				ret, ret_str));

		  /* Make sure SYNTAX ERRROR is not forgotten */
		  if (ret==XNEE_SYNTAX_ERROR)
		    {
		      return ret;  
		      XNEE_RETURN_IF_ERR (ret);  
		    }
		  break ; 
		}
	    }
	}
/*        ret = xnee_set_ranges(xd); */
    }

/*   xnee_print_ranges(xd,stdout); */
  
  if ( 
      (read_mode==XNEE_REPLAY_READ_REPLAY_DATA) || 
      (read_mode==XNEE_REPLAY_READ_ALL_DATA))
    {
      ret = xnee_set_ranges(xd);
      XNEE_RETURN_IF_ERR (ret);


      /* REMOVE ME... after testing.
	 rep_prepare is done in xnee_prepare */
      /*    ret = xnee_rep_prepare(xd); */
      /*    if (ret!=XNEE_OK) */
      /*      { */
      /*        xnee_verbose((xd, "xnee_prepare failed (%d)....checking\n", ret)); */
      /*        xnee_verbose((xd, "xnee_prepare failed.... failure\n")); */
      /*        return ret; */
      /*      } */
      
      if ( xd->sync != 0 ) 
	{
           ret = xnee_setup_rep_recording(xd);
           XNEE_RETURN_IF_ERR(ret);
	}

      if ( xnee_is_verbose(xd) != 0)
	{
           ret = xnee_print_sys_info(xd , xd->out_file);
           XNEE_RETURN_IF_ERR(ret);
	}

/*       xnee_print_ranges(xd,stdout); */

      ret = xnee_expression_handle_session(xd, tmp, &xindata);

/*       if ( ret == XNEE_PRIMITIVE_DATA ) { printf ("return XNEE_OK\n"); ret = XNEE_OK; } */

/* 	  printf ("  ===== starting \n"); */
/* 	  xnee_print_list(); */
      /**
       * all META DATA setting up our sessions is read ...
       * go on replaying
       *
       * 
       *       Think of this as the main loop
       */
      while  ( (ret!=XNEE_SYNTAX_ERROR) && ( xd->cont != 0 ) ) 
	{
	  if (last_logread != 0)
	    {
	      /* 
	       * set value for hcoming time calculations 
	       * Now replay starts off
	       */
	      xnee_verbose((xd, " -->  xnee_get_elapsed_time\n"));
	      last_elapsed = xnee_get_elapsed_time(xd, XNEE_FROM_LAST_READ );
	      xnee_verbose((xd, " <--  xnee_get_elapsed_time\n"));
	    }


	  if ( ret == XNEE_META_DATA )
	    {
	      xnee_verbose((xd, "META DATA read ... should be "
			    "handled in the future... eg script ????\n"));
	    }
	  else if (ret!=0)
	    {
/* 	      printf (" CC : "); */
	      if (xd->first_read_time==0)
              {
                 /*@ ignore @*/
                 xd->first_read_time = xindata.newtime;
                 /*@ end @*/
              }
	      
	      
	
	      /* Interrupt variable set? */
	      if (xnee_get_interrupt_action(xd))
		{
		  xnee_verbose((xd, "interrupt (replay) variable was set (%d)\n",
				xnee_get_interrupt_action(xd)));
		  xnee_unset_interrupt_action(xd);
		  xnee_verbose((xd, "leaving loop  (%d)\n", 
				xnee_get_interrupt_action(xd)));
		  return XNEE_OK;
		}

/* 	      printf (" DD "); */

	      /*
	       *
	       * Do we have any grabbed key that have been pressed?
	       * 
	       */
	      if (xnee_check_key (xd)==XNEE_GRAB_DATA)
		{
		  ret = xnee_handle_rep_key(xd) ; 
		  if (ret == XNEE_GRAB_STOP)
		    {
		      xnee_verbose  ((xd," breaking async loop since STOP \n"));
		      return XNEE_OK;
		    }
		  else if (ret == XNEE_GRAB_RESUME)
		    {
                       if  ( (xd == NULL)
                             ||
                             (xd->control ==NULL)
                             ||
                             (xd->record_setup ==NULL))
                       {
                          return XNEE_RECORD_FAILURE;
                       }


		      xnee_verbose  ((xd," starting async loop since RESUME \n"));
		      ret = XRecordEnableContextAsync(context_display, 
                                                      xd->record_setup->rContext, 
                                                      xd->rec_callback, 
                                                      (XPointer) (xd) );
                      XNEE_RETURN_IF_ERR(ret);
		    }
		}
	      
/* 	      printf (" EE "); */

	      /*
	       *
	       * OK, all grabbed stuffed is handled, let's fake some events
	       *
	       */

	      switch (xindata.type)
		{
		case XNEE_EVENT:
		  /* if type == 0, break .... BTW, why is it 0?? */
		  if ( xindata.u.event.type == 0 ) { break ; }
		  /* is it a device event ? */
                   if ( ( xindata.u.event.type >= KeyPress ) 
		       && (xindata.u.event.type <= MotionNotify) )
		    {
		      if ( xindata.u.event.type == ButtonPress )
			{
			  xd->button_pressed++;
			}
		      else if ( xindata.u.event.type == ButtonRelease )
			{
			  xd->button_pressed--;
			}
		      if ( xindata.u.event.type == KeyPress )
			{
			  xd->key_pressed++;
			}
		      else if ( xindata.u.event.type == KeyRelease )
			{
			  xd->key_pressed--;
			}
		      
		      
		      /*		  if ( (xd->button_pressed==0) ||
			(xd->key_pressed==0) )
			{
		      */
		      ret = xnee_replay_synchronize (xd);
		      if (ret != XNEE_OK)
			{
			  xnee_verbose((xd, "xnee_replay_main_loop return %d\n", 
					ret));
			  return ret;
			}
		      /*		  */
		      /*		    }*/
		      xnee_verbose((xd," replay MAIN  new %lu   old %lu\n",xindata.newtime , xindata.oldtime ));
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
		  xnee_verbose((xd, "READ A REQUEST\n")); 
		  xnee_replay_buffer_handler ( xd, 
					       XNEE_REQUEST, 
					       xindata.u.request.type, 
					       XNEE_REPLAYED);
                  break;
		case XNEE_REPLY:
		  xnee_verbose((xd, "READ A REPLY\n")); 
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
	      xnee_verbose((xd, "Corrupt line ... skipped \n"));
	      break;
	    }
	  
	  
/* 	  (void)XSync(xd->control, False); */
	  xnee_verbose((xd, "Flushing after handled event\n"));
	  (void)XFlush(xd->control);
	  xnee_verbose((xd, "  <-- Flushed after handled event\n"));

	  ret_str = fgets(tmp, 256, xd->data_file);
	  if (ret_str == NULL)
	    {
 	      break; 
	    }

	  ret = xnee_expression_handle_session(xd, tmp, &xindata);
/* 	  printf ("  and again  (%d, %s)\n", ret, xnee_get_err_description(ret)); */
	  last_logread = 0;
	}
    }

  xd->cont     = 1;  
  last_logread = 1;
  time_out_counter = 0;
  diff_counter     = 0;


  return XNEE_OK;
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
  int ret;
  xnee_recordext_setup  *xrs       = xd->record_setup;
  Display *context_display;

      

  if  ( (xd == NULL)
        ||
        (xd->control ==NULL)
        ||
        (xd->record_setup ==NULL))
  {
     return XNEE_RECORD_FAILURE;
  }

  xnee_verbose((xd, "--->xnee_setup_rep_recording :)\n"));             
  nr_of_ranges=xnee_get_max_range(xd);

  if (xd->all_clients != 0)
    {
      xrs->xids[0] = XRecordAllClients;
    }
  else
    {
      xrs->xids[0] = XRecordFutureClients; 
    }
  xnee_verbose((xd, "\t  CreateContext   nr_of_ranges=%d\n", nr_of_ranges));  

  context_display = xnee_get_display_for_recordcontext(xd);

  xnee_verbose ((xd, "creating context .... on control = %d\n", (int)context_display));


  xrs->rContext = XRecordCreateContext(context_display, 
				       xrs->data_flags, 
				       xrs->xids, 1, 
				       xrs->range_array,nr_of_ranges );

  xnee_verbose((xd, "--- xnee_setup_rep_recording  freeing state \n"));             

  XRecordFreeState(xrs->rState); 

  xnee_verbose((xd, "--- xnee_setup_rep_recording  setting rstate \n"));             
  xd->record_setup->rState=NULL; 

  
  xnee_verbose((xd, "XSync()\n"));
  XSync  (xd->control, False); 
  xnee_verbose((xd, "XFlush()\n"));
  XFlush (xd->control); 
  XSynchronize(xd->data, True);  
  

  
  xnee_verbose((xd, "--- xnee_setup_rep_recording enabling async \n"));             

  /* Enable context for async interception */
  ret = XRecordEnableContextAsync (context_display, 
                                   xrs->rContext, 
                                   xnee_replay_dispatch, 
                                   (XPointer) (xd) /* closure passed to Dispatch */);
  if (ret==0)
    {
      xnee_verbose ((xd, "Could not start recording\n"));
      return XNEE_RECORD_FAILURE;
    }


  xnee_verbose((xd, "finished setting up record for replaying\n"));
  xnee_verbose((xd, "<---xnee_setup_rep_recording\n"));             
  
  return (XNEE_OK);
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
  static unsigned int last_record_window_pos_win = 0;
  static unsigned int last_record_window_pos_par = 0;
  
  XRecordDatum *xrec_data;
  int           type;
  xnee_data    *xd;
  int rec_window_pos = 0;
  XWindowAttributes window_attributes_return;
  int ret;

  if (data->data==NULL)
    {
      return;
    }

		      


  xrec_data  = (XRecordDatum *) (data->data) ;
  type       = (int) xrec_data->type ;
  xd         = (xnee_data*) (type_ref);

  switch(data->category)
    {
    case XRecordFromClient:
      XNEE_SYNC_DEBUG ( (stderr, "DISPATCH REQUEST %d \n",type  ) );
      xnee_verbose((xd, "GOT A REQUEST:         %d \n ", type));
/*       xnee_replay_buffer_handle (xd, XNEE_REQUEST, type, XNEE_RECEIVED); */
      xnee_replay_buffer_handler (xd, XNEE_REQUEST, type, XNEE_RECEIVED); 
/*       printf ("dispatch: %d (%d/%d)\n", XNEE_RECEIVED, XNEE_REQUEST,type); */

      break;
    case XRecordFromServer:
      if(type >  X_Reply )
	{
	  XNEE_SYNC_DEBUG ( (stderr, "DISPATCH EVENT   %d \n",type  ) );
	  xnee_verbose((xd, "GOT A EVENT:       %d \n", type));
	  
	  if ( type == ReparentNotify )
	    {
	      rec_window_pos = xnee_get_recall_window_pos(xd);
	      /* rec_window_pos interpretation:
	       *   0  used to sync
	       *   1  used to adjust window pos
	       *   2 both of the above 
		 *   * error
		 */
	      if ( rec_window_pos == 0 )
		{
		  /* only sync */
		  xnee_replay_buffer_handler (xd, XNEE_EVENT, type, XNEE_RECEIVED); 
		}
	      else if ( ( rec_window_pos == 1 ) || ( rec_window_pos == 2 ) )
		{
		  XGetWindowAttributes(xd->grab, 
				       xrec_data->event.u.reparent.window,
				       &window_attributes_return);

		  /* 
		   * Prevent the same window pos to be used more than once
		   */
		  if ( (last_record_window_pos_win != 
			xrec_data->event.u.reparent.window) ||
		       (last_record_window_pos_par != 
			xrec_data->event.u.reparent.parent) )
		    {
		      xnee_verbose((xd, "   xnee_replay adding window 0x%X\n",
				    (unsigned int)xrec_data->event.u.reparent.window));

		      xnee_window_add_attribute_received(xd, 
							 &window_attributes_return,
							 xrec_data->event.u.reparent.window,
							 xrec_data->event.u.reparent.parent);

		      ret = xnee_window_try_move(xd);
		      XNEE_RETURN_VOID_IF_ERR(ret);

		      last_record_window_pos_win = 
			xrec_data->event.u.reparent.window;
		      last_record_window_pos_par = 
			xrec_data->event.u.reparent.parent;
		    }
		}
	      
	      if ( rec_window_pos == 2 )
		{
		  xnee_replay_buffer_handler (xd, XNEE_EVENT, type, XNEE_RECEIVED); 
		}
	    }
	  else
	    {
	      xnee_replay_buffer_handler (xd, XNEE_EVENT, type, XNEE_RECEIVED); 
	    }
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
 * xnee_has_xtest_extension                                   *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_has_xtest_extension (xnee_data *xd) 
{
  int ok=1;
  xnee_testext_setup* xrs;
  
  if  ( (xd == NULL)
        ||
        (xd->control ==NULL)
        ||
        (xd->replay_setup ==NULL))
  {
     return XNEE_RECORD_FAILURE;
  }

  

  xrs=xd->replay_setup;
  if( XTestQueryExtension(xd->control,
                          &xrs->xtest_event_basep,
                          &xrs->xtest_error_basep,
                          &xrs->xtest_version_major,
                          &xrs->xtest_version_minor) == 0 )
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
xnee_replay_init          (xnee_data* xd)
{
  int i, j;
  xd->first_replayed_event=XNEE_TRUE;

  xnee_verbose((xd, "---> xnee_replay_init\n"));
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

  xd->first_read_time = 0;

  if ( xnee_no_rep_resolution(xd) == 0 )
  {
     int ret ; 
     
     ret = xnee_set_default_rep_resolution (xd);
     XNEE_RETURN_VOID_IF_ERR(ret);
  }
  
  xnee_verbose((xd, "<--- xnee_replay_init\n")); 
}

