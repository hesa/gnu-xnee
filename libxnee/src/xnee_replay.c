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
#include "libxnee/xnee_fake.h"
#include "libxnee/xnee_km.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_resource.h"
#include "libxnee/xnee_time.h"
#include "libxnee/xnee_buffer.h"
#include "libxnee/xnee_threshold.h"
#include "libxnee/xnee_expr.h"



void 
xnee_replay_m_delay (unsigned long usecs)
{
  usleep ( usecs ) ;
}



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
  static int time_out_counter=0;
  static int diff_counter=0;

  xnee_verbose((xd,"---> xnee_replay_synchronize \n"));;
  if ( xd->sync == False ) 
    {
      xnee_verbose((xd, "Xnee in NO SYNC mode\n"));
      return XNEE_OK;
    }
   
  /*****
   * Check to see if we are in sync
   */
  xnee_verbose ((xd, "   synchronize: entering sync test loop\n"));

  while (1)
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
	  static int last_diff;
	  
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
		  if (xnee_is_force_replay(xd))
		    break;
		  fprintf (stderr,
			   "Can't synchronize anymore .... have to leave!\n");
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
	  usleep (XNEE_MISSING_DATA_DELAY );
	  last_diff = diff;

	}
      else
	{
	  if (diff_counter!=0)
	    {
	      xd->buf_verbose=True;
	      xnee_replay_printbuffer(xd); 
	      return XNEE_SYNCH_FAULT;
	      /*	      exit(0);*/
	    }
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
 * xnee_replay_read_protocol                                  *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_replay_read_protocol (xnee_data* xd, xnee_intercept_data * xindata) 
{
  char tmp[256] ;
  int eofile = 0 ;
  char *ret;

  strcpy(tmp,"");
  if ( xd->data_file == NULL)
    {
      xnee_verbose((xd, "Using stdin as file\n"));
      xd->data_file=stdin;
    }
  ret = fgets(tmp, 256, xd->data_file);

  if (ret == NULL )
    {
      return 0;
    }
  
  if ( tmp == NULL) 
    {
      return XNEE_OK;
    }
  eofile = xnee_expression_handle_session(xd,
					  tmp,
					  xindata);

  xnee_verbose((xd,"replay data handled (or perhaps not) '%s'\n", tmp)); 
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
  char *range;

  xnee_verbose((xd, "xnee_handle_meta_data     (xd, \"%s\");\n", str));

  xnee_rem_comment_start (xd, str);

  xnee_verbose((xd, "xnee_handle_meta_data (2) (xd, \"%s\");\n", str));

  rem_all_blanks (str, strlen(str));

  xnee_verbose((xd, "xnee_handle_meta_data (3) (xd, \"%s\");\n", str));

  if (strncmp (str, XNEE_ALL_EVENTS, strlen (XNEE_ALL_EVENTS)) == 0  )
    {
      xnee_handle_meta_data_sub1 (&xd->xnee_info.all_events, str,  strlen(XNEE_ALL_EVENTS) );
    }
  else if (strncmp (str, XNEE_EVERYTHING, strlen (XNEE_EVERYTHING)) == 0  )
    {
      xnee_handle_meta_data_sub1 (&xd->xnee_info.everything, str,  strlen(XNEE_EVERYTHING) );
    }
  else if (strncmp (str, XNEE_LOOPS_LEFT, strlen (XNEE_LOOPS_LEFT)) == 0  )
    {
      xnee_handle_meta_data_sub1 (&xd->xnee_info.events_max, str,  strlen(XNEE_LOOPS_LEFT) );
    }
  else if (strncmp (str, XNEE_EVENT_MAX, strlen (XNEE_EVENT_MAX)) == 0  )
    {
      xnee_handle_meta_data_sub1 (&xd->xnee_info.events_max, str,  strlen(XNEE_EVENT_MAX) );
    }
  else if (strncmp (str, XNEE_DATA_MAX, strlen (XNEE_DATA_MAX)) == 0  )
    {
      xnee_handle_meta_data_sub1 (&xd->xnee_info.data_max, str,  strlen(XNEE_DATA_MAX) );
    }
  else if (strncmp (str, XNEE_TIME_MAX, strlen (XNEE_TIME_MAX)) == 0  )
    {
      xnee_handle_meta_data_sub1 (&xd->xnee_info.time_max, str,  strlen(XNEE_TIME_MAX) );
    }
  else if (strncmp (str, XNEE_NO_EXPOSE, strlen (XNEE_NO_EXPOSE)) == 0  )
    {
      xnee_handle_meta_data_sub1 (&xd->xnee_info.no_expose, str,  strlen(XNEE_NO_EXPOSE) );
    }
  else if (strncmp (str, XNEE_NO_EXPOSE, strlen (XNEE_NO_EXPOSE)) == 0  )
    {
      xnee_handle_meta_data_sub1 (&xd->xnee_info.no_expose, str,  strlen(XNEE_NO_EXPOSE) );
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
  else if (!strncmp(XNEE_DIMENSION,str,strlen(XNEE_DIMENSION)))
    {
      range=strstr (str, ":");
      range += 1 ;
      if ( xnee_set_rec_resolution (xd, range))
	{
	  xnee_verbose ((xd, "failed to set recorded resolution\n"));
	  xnee_close_down(xd);
	  exit(XNEE_BAD_RESOLUTION );
	}
      xnee_verbose ((xd, "recored resolution= %dx%d\n", 
		     xnee_get_rec_resolution_x(xd),
		     xnee_get_rec_resolution_y(xd)));
    }
  else 
    {
      xnee_verbose((xd, " Skipping :\"%s\"\n", str));
      ; /* TODO .... */
      return -1;
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
int
xnee_replay_main_loop(xnee_data *xd)
{
  xnee_intercept_data xindata ;
  int logread = 1 ;
  int replayable ;
  static int last_logread=1;
  int last_elapsed = 0;
  int ret = XNEE_OK ; 
/*   static int in_pause_mode = 0 ;  */

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
      else if ( logread == XNEE_REPLAY_DATA )
	{
	  /* since NULL arg printing is done when in verbose mode */
	  xnee_record_print_record_range (xd, NULL);
	  xnee_print_xnee_settings       (xd, NULL); 
	  
	  if (xd->sync)
	    {
	      xnee_setup_rep_recording(xd);
	    }
	  if (xnee_is_verbose(xd))
	    xnee_print_sys_info(xd , xd->out_file);
	  xnee_verbose((xd, "REPLAY DATA coming up .... \n"));
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
      if (last_logread)
	{
	  /* 
	   * set value for forthcoming time calculations 
	   * Now replay starts off
	   */
	  last_elapsed = xnee_get_elapsed_time(xd, XNEE_FROM_LAST_READ );
	}
      if ( logread == XNEE_META_DATA )
	{
	  xnee_verbose((xd, "META DATA read ... should be handled in the future... eg script ????\n"));
	}
      else if (logread)
	{
	  if (xd->first_read_time==0)
            xd->first_read_time = xindata.newtime;


	  if (xnee_check_km (xd)==XNEE_GRAB_DATA)
	    {
	      ret = xnee_handle_rep_km(xd) ; 
	      if (ret == XNEE_GRAB_STOP)
		{
		  xnee_verbose  ((xd," breaking async loop since STOP \n"));
		  return XNEE_OK;
		}
	      else if (ret == XNEE_GRAB_RESUME)
		{
		  xnee_verbose  ((xd," starting async loop since RESUME \n"));
		  XRecordEnableContextAsync(xd->data, 
					    xd->record_setup->rContext, 
					    xd->rec_callback, 
					    (XPointer) (xd) );
		}
	    }
	  
	  switch (xindata.type)
	    {
	    case XNEE_EVENT:
	      
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
	  xnee_verbose((xd, "Corrupt line ... skipped \n"));
	  break;
	}


      XSync(xd->control, False);
      xnee_verbose((xd, "Flushing after handled event\n"));
      XFlush(xd->control);
      xnee_verbose((xd, "  <-- Flushed after handled event\n"));
      last_logread = 0;
      logread = xnee_replay_read_protocol(xd, &xindata);
    }
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
  xnee_recordext_setup  *xrs       = xd->record_setup;

  xnee_verbose((xd, "--->xnee_setup_rep_recording :)\n"));             
  nr_of_ranges=xnee_get_max_range(xd);

  xnee_verbose((xd, "--- xnee_setup_rep_recording \n"));             
  if (xd->all_clients)
    {
      xrs->xids[0] = XRecordAllClients;
    }
  else
    {
      xrs->xids[0] = XRecordFutureClients; 
    }
  xnee_verbose((xd, "\t  CreateContext   nr_of_ranges=%d\n", nr_of_ranges));  

  xnee_verbose ((xd, "creating context .... on control = %d\n", xd->control));
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

  xnee_verbose((xd, "--- xnee_setup_rep_recording setting xids \n"));             

  xrs->xids[0] = xnee_client_id (xd->control);
  xrs->xids[1] = xnee_client_id (xd->data);

  xnee_verbose((xd, "--- xnee_setup_rep_recording unregistring \n"));             

  XRecordUnregisterClients( xd->control, 
			    xrs->rContext,
			    xrs->xids,
			    2);

  xnee_verbose((xd, "--- xnee_setup_rep_recording getting context \n"));             
  xnee_verbose((xd, "--- \t %d\n", xd->control));             
  xnee_verbose((xd, "--- \t %d\n", xrs));             
  xnee_verbose((xd, "--- \t %d\n", xrs->rContext));             
  xnee_verbose((xd, "--- \t %d\n", xrs->rState));             

  if(!XRecordGetContext(xd->control, xrs->rContext, (XRecordState **) xrs->rState))
    {
      xnee_print_error ("\n Couldn't get the context information for Display %d\n", (int) xd->control) ;
      exit(XNEE_BAD_CONTEXT);
    }
  
  xnee_verbose((xd, "\t  GetContext      0x%lx (%d clients intercepted))\n", 
		xrs->rContext, (int) ( (xrs->rState) - (xrs->nclients) ) ));   
  
  xnee_verbose((xd, "--- xnee_setup_rep_recording  freeing state \n"));             

  XRecordFreeState(xrs->rState); 

  xnee_verbose((xd, "--- xnee_setup_rep_recording  setting rstate \n"));             
  xd->record_setup->rState=NULL; 

  /*
  xnee_verbose((xd, "XSync()\n"));
  XSync  (xd->control, False); 
  xnee_verbose((xd, "XFlush()\n"));
  XFlush (xd->control); 
  XSynchronize(xd->data, True);  
  */

  
  xnee_verbose((xd, "--- xnee_setup_rep_recording enabling async \n"));             

  /* Enable context for async interception */
  XRecordEnableContextAsync (xd->data, 
			     xrs->rContext, 
			     xnee_replay_dispatch, 
			     (XPointer) (xd) /* closure passed to Dispatch */);
  xnee_verbose((xd, " 1.3\n"));             
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
xnee_replay_init          (xnee_data* xd)
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
  if (xnee_no_rep_resolution(xd))
    xnee_set_default_rep_resolution (xd);
}

