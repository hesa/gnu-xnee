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



#include "libxnee/xnee.h"
#include "libxnee/datastrings.h"
#include "libxnee/print.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"
#include "libxnee/xnee_km.h"
#include "libxnee/xnee_fake.h"
#include "libxnee/xnee_grab.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/feedback.h"
#include "libxnee/xnee_range.h"


#ifdef DEBUG_DISPATCHER
/**************************************************************
 *                                                            *
 * xnee_record_dispatch2                                      *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_record_dispatch2 (XPointer xpoint_xnee_data,
		       XRecordInterceptData *data )
{

  if ( (!data->data) || (data==NULL) )
    {
      XRecordFreeData(data);
      XNEE_DEBUG ( (stderr ," <-- xnee_human_dispatch()  \n"  ));
      xnee_process_count(XNEE_PROCESS_RESET);
      return;
    } 

  /*increment the process reply counter*/
  xnee_process_count(XNEE_PROCESS_INC);
  xpoint_xnee_data=0;
  printf( "."); 

  fflush (stdout);
  XRecordFreeData(data);
  return;
}
#endif



/**************************************************************
 *                                                            *
 * xnee_record_handle_event                                   *
 *                                                            *
 *                                                            *
 **************************************************************/
void
xnee_record_handle_event ( xnee_data *xd, XRecordInterceptData *xrecintd)
{
  XRecordDatum *xrec_data  = (XRecordDatum *) (xrecintd->data) ;
  int           event_type = xrec_data->type ;
  FILE *out = xd->out_file;
  KeyCode kc;
  int do_print = 1;

  XNEE_DEBUG ( (stderr ," -->xnee_record_handle_event()  \n"  ));
  if ((xd->xnee_info.first_last) && (xd->xnee_info.last_motion > 0) && (event_type==MotionNotify)) 
    {
      XNEE_DEBUG ( (stderr ," -- xnee_record_handle_event() at 0  \n"  ));
      xd->xnee_info.x           = xrec_data->event.u.keyButtonPointer.rootX ;
      xd->xnee_info.y           = xrec_data->event.u.keyButtonPointer.rootY ;
      xd->xnee_info.server_time = xrecintd->server_time ;
    }
  else 
    { 
      XNEE_DEBUG ( (stderr ," -- xnee_record_handle_event() at 1  \n"  ));
      if ( (xd->xnee_info.first_last) && 
	   (xd->xnee_info.last_motion) &&
	   ((event_type==KeyPress)    || 
	    (event_type==KeyRelease)  ||
	    (event_type==ButtonPress) || 
	    (event_type==ButtonRelease  ))
	   ) 
	{
	  XNEE_DEBUG ( (stderr ," -- xnee_record_handle_event() at 2  \n"  ));
	  fprintf (out,"0,%d,%d,%d,0,0,0,%lu\n",
		   6,	       
		   xd->xnee_info.x,
		   xd->xnee_info.y,
		   xd->xnee_info.server_time);
	}
      else 
	{
	  XNEE_DEBUG ( (stderr ," -- xnee_record_handle_event() at 3 (%d)  \n" ,event_type ));

	  switch(event_type)
	    {	  
	    case KeyPress:
	      kc = xrec_data->event.u.u.detail;

	      do_print = xnee_save_or_print(xd, kc, XNEE_GRAB_KM_PRESS);
	      if (do_print==XNEE_GRAB_DO_PRINT)
		{
		  fprintf (out,"0,%d,0,0,0,%d,0,%lu\n",
			   event_type,
			   kc,
			   xrecintd->server_time
			   );
		  xnee_fake_key_event  (xd, kc, True, CurrentTime );
		}
	      else if (do_print==XNEE_GRAB_DO_SAVE)
		{
		  char buf[64];
		  sprintf (buf, "0,%d,0,0,0,%d,0,%lu\n",
			    event_type,
			    kc,
			   xrecintd->server_time);
		  xnee_grab_handle_buffer (xd, buf, XNEE_GRAB_BUFFER_SAVE);
		}
	      break;
	    case KeyRelease:
	      kc = xrec_data->event.u.u.detail;
	      do_print = xnee_save_or_print(xd, kc, XNEE_GRAB_KM_RELEASE);

	      if (do_print==XNEE_GRAB_DO_PRINT)
		{
		  fprintf (out,"0,%d,0,0,0,%d,0,%lu\n",
			   event_type,
			   xrec_data->event.u.u.detail,
			   xrecintd->server_time
			   );
		  xnee_fake_key_event  (xd,xrec_data->event.u.u.detail , False, CurrentTime);
		}
	      else if (do_print==XNEE_GRAB_DO_SAVE)
		{
		  char buf[64];
		  sprintf (buf, "0,%d,0,0,0,%d,0,%lu\n",
			    event_type,
			    kc,
			   xrecintd->server_time);
		  xnee_grab_handle_buffer (xd, buf, XNEE_GRAB_BUFFER_SAVE);

		}
	      break;
	    case ButtonPress:
	      fprintf (out,"0,%d,0,0,%d,0,0,%lu\n",
		       event_type,
		       xrec_data->event.u.u.detail,
		       xrecintd->server_time
		       );
	      xnee_fake_button_event (xd,xrec_data->event.u.u.detail , True, CurrentTime);
	      break;
	    case ButtonRelease:
	      fprintf (out,"0,%d,0,0,%d,0,0,%lu\n",
		       event_type,
		       xrec_data->event.u.u.detail,
		       xrecintd->server_time
		       );
	      xnee_fake_button_event (xd, xrec_data->event.u.u.detail, False, CurrentTime);
	      break;
	    case MotionNotify:  
	      xnee_save_or_print(xd, kc, XNEE_GRAB_MOUSE);
	      fprintf (out,"0,%d,%d,%d,0,0,0,%lu\n",
		       event_type,
		       xrec_data->event.u.keyButtonPointer.rootX,
		       xrec_data->event.u.keyButtonPointer.rootY,
		       xrecintd->server_time
		       );
	      xd->xnee_info.last_motion = True ;
	      xnee_fake_motion_event (xd,
				      0, 
				      xrec_data->event.u.keyButtonPointer.rootX, 
				      xrec_data->event.u.keyButtonPointer.rootY, 
				      CurrentTime);
	      
	      break;
	    case CreateNotify:
	      fprintf (out,"0,%d,0,0,0,0,0,%lu\n",
		       event_type,
		       xrecintd->server_time
		       );
	      break;
	    case DestroyNotify:
	      fprintf (out,"0,%d,0,0,0,0,0,%lu\n", 
		       event_type,
		       xrecintd->server_time
		       );
	      break;
	    default:
	      XNEE_DEBUG ( (stderr ," -- xnee_record_handle_event() at 6  \n"  ));
	      fprintf (out,"0,%d,0,0,0,0,0,%lu\n", 
		       event_type,
		       xrecintd->server_time
		       );
	      break;
	    }
	}
      XNEE_DEBUG ( (stderr ," -- xnee_record_handle_event() at 7  \n"  ));

    }
  fflush (out);
  if (((event_type==KeyPress) || (event_type==KeyRelease)
       || (event_type==ButtonPress) || (event_type==ButtonRelease)) 
      && (xd->xnee_info.last_motion) )
    xd->xnee_info.last_motion=0 ;
  XNEE_DEBUG ( (stderr ," <-- xnee_record_handle_event()  \n"  ));
}





/**************************************************************
 *                                                            *
 * xnee_record_dispatch                                       *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_record_dispatch(XPointer xpoint_xnee_data,
			  XRecordInterceptData *data )
{
  XRecordDatum        *xrec_data  ;
  xnee_data           *xd ;
  int                  data_type = 0;  

  XNEE_DEBUG ( (stderr ," --> xnee_record_dispatch()  \n"  ));
  
  if ( (!data->data) || (data==NULL) )
    {
      XRecordFreeData(data);
      XNEE_DEBUG ( (stderr ," <-- xnee_record_dispatch()  \n"  ));
      xnee_process_count(XNEE_PROCESS_RESET);
      return;
    } 
  else
    {
      /*increment the process reply counter*/
      xnee_process_count(XNEE_PROCESS_INC);
    }
  
  /* fix by Ton van Vliet */
  xd = (xnee_data *) (xpoint_xnee_data) ;
  xrec_data = (XRecordDatum *) (data->data) ;



  if ( xnee_more_to_record(xd)==0 ) 
    {
      XRecordFreeData(data);
      return ;
    } 
  else
    {
      xnee_inc_data_recorded(xd);
    }

 
  /* if NOT XRECORDEndOffData or XRECORDClientDied  set data_type */  
  if(data->data_len) 
    {
      data_type = xrec_data->type;
    }
  
  
  switch(data->category)
    {
    case XRecordFromClient:
      xnee_record_print_request (xd, data) ;
      break;
    case XRecordFromServer:
      if(data_type >  X_Reply )
	{
	  xnee_inc_events_recorded(xd);
	  xnee_record_handle_event (xd, data) ;
	}
      else
	{ 
	  xnee_record_print_reply (xd, data) ;
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
  XNEE_DEBUG ( (stderr ," <-- xnee_record_dispatch()  \n"  ));
} 
 




/**************************************************************
 *                                                            *
 * xnee_human_dispatch                                        *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_human_dispatch(XPointer xpoint_xnee_data,
		    XRecordInterceptData *data )
{
  XRecordDatum        *xrec_data  ;
  xnee_data           *xd ;
  int                  data_type = 0;  
  
  XNEE_DEBUG ( (stderr ," --> xnee_human_dispatch()  \n"  ));
  
  if ( (!data->data) || (data==NULL) )
    {
      XRecordFreeData(data);
      XNEE_DEBUG ( (stderr ," <-- xnee_human_dispatch()  \n"  ));
      xnee_process_count(XNEE_PROCESS_RESET);
      return;
    } 
  else
    {
      /*increment the process reply counter*/
      xnee_process_count(XNEE_PROCESS_INC);
    }
  
  /* fix by Ton van Vliet */
  xd = (xnee_data *) (xpoint_xnee_data) ;
  xrec_data = (XRecordDatum *) (data->data) ;
  
  if ( xnee_more_to_record(xd)==0 ) 
    {
      XRecordFreeData(data);
      return ;
    } 
  else
    {
      xnee_inc_data_recorded(xd);
    }

 
  /* if NOT XRECORDEndOffData or XRECORDClientDied  set data_type */  
  if(data->data_len) 
    {
      data_type = xrec_data->type;
    }

  


  switch(data->category)
    {
    case XRecordFromClient:
      xnee_human_print_request (xd, data) ;
      break;
    case XRecordFromServer:
      if(data_type >  X_Reply )
	{
	  xnee_inc_events_recorded(xd);
	  xnee_human_print_event (xd, data) ;
	}
      else
	{ 
	  xnee_human_print_reply (xd, data) ;
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
  XNEE_DEBUG ( (stderr ," <-- xnee_human_dispatch()  \n"  ));
  XRecordFreeData(data);
} 
 





/**************************************************************
 *                                                            *
 * xnee_record_init                                           *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_record_init (xnee_data *xd) 
{
  int i=0;
  XNEE_DEBUG ( (stderr ," --> xnee_record_init()  \n"  ));
  xd->xnee_info.first_last      = False  ; /* Intercept all MotionNotify, else only first and last */

  xd->xnee_info.last_motion     = False  ; /* Used to record only first and last Motion in a row*/
  xd->xnee_info.events_recorded = 0    ; /* Default 100 Datum */
  xd->xnee_info.store_mouse_pos = 0;
   
  xd->xnee_info.data_recorded   = 0    ; /* Default 500 Datum*/
   
  xd->xnee_info.time_recorded   = 0      ; /* Default 1*/
   
  xd->xnee_info.events_max      = 100    ; /* Default 100 Datum */
   
  xd->xnee_info.data_max        = 500    ; /* Default 500 Datum*/
   
  xd->xnee_info.time_max        = 1     ; /* Default 1*/
   
  xd->xnee_info.x               = 0      ; /* Need not to be set, anyhow last MotionNotify x=0*/
  xd->xnee_info.y               = 0      ; /* Need not to be set, anyhow last MotionNotify x=0*/
  xd->xnee_info.server_time     = 0      ; /* Need not to be set, anyhow last .... */  

  xd->xnee_info.interval        = 0      ;
  xd->xnee_info.size            = 0      ;
  xd->xnee_info.size            = 0      ;

  for ( i=0 ; i< XNEE_NR_OF_TYPES ; i++) 
    {
      xd->xnee_info.data_ranges[i] = 0 ;       /* Count how many data specified */
    }
  /*
    for (i=0 ; i<XNEE_NR_OF_TYPES ; i++)
    {
    xd->rec_range->next[0]=0;
    }
    xd->rec_range->next_record_range=0;
  */
  XNEE_DEBUG ( (stderr ," <-- xnee_record_init()  \n"  ));
}










/**************************************************************
 *                                                            *
 * xnee_setup_recordext                                       *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_setup_recordext (xnee_data *xd)
{
  int ret=XNEE_OK;

  xnee_verbose((xd, " ---> xnee_setup_recordext\n"));
  xd->record_setup->data_flags = XRecordFromServerTime 
    | XRecordFromClientTime  
    | XRecordFromClientSequence; 
  ret = xnee_get_max_range(xd) ;
  if (ret==0)
    {
      xnee_verbose((xd, " --- xnee_setup_recordext Nothing to record \n"));
      ret = XNEE_NO_PROT_CHOOSEN;
    } 
  else
    {
      ret = XNEE_OK;
    }

  xnee_verbose((xd, " <--- xnee_setup_recordext %d\n", ret));
  return (ret);
}



/**************************************************************
 *                                                            *
 * xnee_has_record_extension                                  *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_has_record_extension(xnee_data *xd)
{
  int ok=1;
  XSynchronize(xd->control,True);
  if(!XRecordQueryVersion(xd->control, &xd->record_setup->major_return, &xd->record_setup->minor_return) )
    {
      xnee_print_error ("Record extension missing\n");
      ok=0;
    }
  xnee_verbose ((xd, "\t  XRecord-\n\t  Release         %d.%d\n", 
		xd->record_setup->major_return, 
		xd->record_setup->minor_return));  
  return (ok);
}




/**************************************************************
 *                                                            *
 * xnee_setup_recording                                       *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_setup_recording(xnee_data *xd)
{
  int nr_of_ranges=0;
  nr_of_ranges=xnee_get_max_range(xd);
  xnee_verbose((xd, "--->xnee_setup_recording\n"));

  XNEE_DEBUG ( (stderr ," --> xnee_setup_recording()  \n"  ));

  XSynchronize(xd->control, True); /* so we get errors at convenient times */
  XSetErrorHandler(handle_xerr);

  if (xd->all_clients)
    {
      xd->record_setup->xids[0] = XRecordAllClients; 
    }
  else
    {
      xd->record_setup->xids[0] = XRecordFutureClients; 
    }
  
  xd->record_setup->rContext = 
    XRecordCreateContext(xd->control, 
			 xd->record_setup->data_flags, 
			 xd->record_setup->xids,1, 
			 xd->record_setup->range_array, nr_of_ranges);
  
  /*
    XRecordRegisterClients   (xd->control,
			    xd->record_setup->rContext,
			    xd->record_setup->data_flags, 
			    xd->record_setup->xids,1,
			    xd->record_setup->range_array,nr_of_ranges);    
  */
  xd->record_setup->xids[0] = xnee_client_id (xd->control);
  xd->record_setup->xids[1] = xnee_client_id (xd->data);
  
  /* 
   *Remove our clients displays from recording ...
   */
  /*
    XRecordUnregisterClients( xd->control, 
			    xd->record_setup->rContext,
			    xd->record_setup->xids,2);
  

  if(!XRecordGetContext(xd->control, xd->record_setup->rContext, (XRecordState **) xd->record_setup->rState))
  {
    xnee_print_error ("\n Couldn't get the context information for Display %d\n", (int) xd->control) ;
    exit(1);
  }
  */
/*
  XRecordFreeState(xd->record_setup->rState); 
  xd->record_setup->rState=NULL; 
*/

  xnee_verbose((xd, "\t  GetContext      0x%lx (%d clients intercepted))\n", 
		xd->record_setup->rContext, 
		(int) ( (xd->record_setup->rState) - (xd->record_setup->nclients) )));   
  
  /* Enable context for async interception 
  XSynchronize(xd->data, True);  
  */

  xnee_verbose((xd, "<---xnee_setup_recording\n"));
  XNEE_DEBUG ( (stderr ," <-- xnee_setup_recording()  \n"  ));
  return (0);
}




/**************************************************************
 *                                                            *
 * xnee_unsetup_recording                                     *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_unsetup_recording(xnee_data *xd)
{

  xnee_verbose((xd, "---> xnee_unsetup_recording\n"));

  xnee_verbose((xd, "---  disabling context \n"));
  XRecordDisableContext(xd->control, xd->record_setup->rContext);

  xnee_verbose((xd, "---  freeing context \n"));
  XRecordFreeContext(xd->control, xd->record_setup->rContext);

  xnee_verbose((xd, "<--- xnee_unsetup_recording\n"));
  return (XNEE_OK);
}




/**************************************************************
 *                                                            *
 * xnee_record_loop                                           *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_record_loop(xnee_data *xd)
{
  xnee_verbose((xd, " ---> xnee_record_loop()\n"));
  
  /* 
   * In case the key pressed to invoke Xnee is not released
   * we wait 1/2 of a second and hopefully it is. If not
   * the user is holding it pressed for "TOO" long.
   */
  usleep ( XNEE_DELAY_RECORDING );

  XRecordEnableContext(xd->data, 
		       xd->record_setup->rContext, 
		       xd->rec_callback, 
		       (XPointer) (xd) /* closure passed to Dispatch */);
      
  xnee_verbose((xd, " <--- xnee_record_loop()\n"));
  /*  while (1) 
    {
      XRecordProcessReplies (xd->data); 
    }
  */
  return (0);
}


/**************************************************************
 *                                                            *
 * xnee_record_async                                          *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_record_async(xnee_data *xd)
{
  xnee_verbose((xd, " ---> xnee_record_async()\n"));
  
  /* 
   * In case the key pressed to invoke Xnee is not released
   * we wait 1/2 of a second and hopefully it is. If not
   * the user is holding it pressed for "TOO" long.
   */
  usleep ( XNEE_DELAY_RECORDING );

  if (xnee_is_store_mouse_pos(xd))
    {
      xnee_store_mouse_pos (xd);
    }


  XRecordEnableContextAsync(xd->data, 
			    xd->record_setup->rContext, 
			    xd->rec_callback, 
			    (XPointer) (xd) /* closure passed to Dispatch */);
  
  while (1) 
    {
      int ret;
      /* has the user pressed a modifier+key */
      if (xnee_check_key (xd)==XNEE_GRAB_DATA)
	{
	  ret = xnee_handle_rec_key(xd) ; 
	  if (ret == XNEE_GRAB_STOP)
	    {
	      xnee_verbose  ((xd," breaking async loop since STOP \n"));
	      break;
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

      /* handle data in the RECORD buffer */
      xnee_process_replies(xd);
      
      if ( xnee_more_to_record(xd) == 0 ) 
	{
	  xnee_verbose  ((xd," closing down while loop in async loop\n"));
	  break ; 
	}

      /* sleep for a little while 
       * there might be someone wanting the CPU
       */  
      usleep (100*100);
    }

/*   XRecordDisableContext(xd->control,  */
/* 			xd->record_setup->rContext); */
/*   XRecordFreeContext(xd->control,  */
/* 			xd->record_setup->rContext); */
  xnee_stop_session(xd);

  xnee_verbose((xd, " <--- xnee_record_async()\n"));
  return (XNEE_OK);
}









/*
 *
 * Name:      xnee_record_close_down
 *
 * Arguments: xnee_data
 *
 *
 * Description: Deallocs recording stuff
 *              This function is obsoleted by the function xnee_free_xnee_data
 * 
 * 
 int
 xnee_record_close_down (xnee_data *xd)
 {
 int i=0;
 int   max=get_max_range(xd);
 xnee_verbose((xd, "Freeing ranges\n"));
 for ( i=0 ; i<max ; i++ )
 {
 free ( xd->record_setup->range_array[i]);
 }
 free (xd->record_setup->range_array);
 return (0);
 }
*/


