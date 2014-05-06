/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *  Copyright (C) 1999-2012, 2014  Henrik Sandklef      
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

#include <unistd.h>

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
#include "libxnee/xnee_error.h"
#include "libxnee/xnee_session.h"
#include "libxnee/xnee_utils.h"
#include "libxnee/xnee_strings.h"
#include "libxnee/xnee_display.h"
#include "libxnee/xnee_xinput.h"



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

  if ( (data==NULL) || (!data->data) )
    {
      XRecordFreeData(data);
      XNEE_DEBUG ( (stderr ," <-- xnee_human_dispatch()  \n"  ));
      xnee_process_count(XNEE_PROCESS_RESET);
      return;
    } 

  /*increment the process reply counter*/
  xnee_process_count(XNEE_PROCESS_INC);
  xpoint_xnee_data=0;

  fflush (stdout);
  XRecordFreeData(data);
  return;
}
#endif


int
xnee_get_screen_nr(xnee_data *xd, Display *dpy, Window recorded_root);

/**************************************************************
 *                                                            *
 * xnee_get_screen_nr                                         *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_get_screen_nr(xnee_data *xd, Display *dpy, Window recorded_root)
{
  int nr_screens  ;
  int this_screen ;
  int i            ; 
  int keep_looping  ; 
  Window  disp_root     ;
  
  nr_screens = ScreenCount(dpy);
  keep_looping = 1 ;
  this_screen  = -1 ; 
  i = 0 ;
  
  do 
    {
      Screen *scr;
      scr = ScreenOfDisplay(dpy,i);
      disp_root = RootWindowOfScreen(scr);

      /* printf ("Loop: i=%d scr=%d dr=%d   rr=%d\n",  */
      /* 	      i, scr, disp_root, recorded_root); */

      if ( disp_root == recorded_root )
	{
	  keep_looping = 0 ;
	  this_screen  = i; 
	}
      i++;
    } while ( (i<nr_screens) && ( keep_looping ) );
  
  /* printf ("display %d   nr of screens %d      this:%d\n", */
  /* 	  dpy, nr_screens, this_screen); */

  /* If we didn't find a screen, bail out */
  if ( this_screen == -1)
    {
      xnee_verbose((xd, "Could not find a screen. (xnee_display:get_screen_nr()\n"));
      return -1 ;
    }
  
  return this_screen;
}


static int 
xnee_record_handle_event_printer(xnee_data * xd, 
				 int event_type, 
				 XRecordDatum *xrec_data, 
				 XRecordInterceptData *xrecintd)
{
  static unsigned int last_record_window_pos_win=0;
  static unsigned int last_record_window_pos_par=0;

  KeyCode kc;
  XWindowAttributes window_attributes_return;
  int rx;
  int ry;
  Window dummy_window;
  FILE *out ;
  int ret ; 
  unsigned int screen ;
  char *win_name;
  int do_print = 1;
  int event_base;
  
  out = xd->out_file;

  XNEE_DEBUG ( (stderr ," -- xnee_record_handle_event() at 3 (%d)  \n" ,event_type ));
 

  /*
   * If XI is in use, don't print device events as core event
   *   only print as XI event
   * 
   * event_base must be set (>0)
   * event_type must be within event_base and event_base+5
   *
   */
  event_base = xd->xi_data.xinput_event_base ;
  if  ( ( event_base       >  0 ) && 
	( event_base       <= event_type ) && 
	( (event_base + 6) >  event_type ) ) 
    {
      ret = xnee_handle_xinput_event(xd, 
				     event_type, 
				     xrec_data,
				     xrecintd->server_time);
      
      /* printf ("XI <----  %d \n", ret); */
    }
  else
    {
      /* printf ("CORE:: %d (%d)\n", event_type, event_base); */
      switch(event_type)
	{	  
	case KeyPress:
	  if  ( event_base > 0 ) { break; }
	  kc = xrec_data->event.u.u.detail;
	  
	  do_print = xnee_save_or_print(xd, kc, XNEE_GRAB_KM_PRESS);
	  
	  if (do_print==XNEE_GRAB_DO_PRINT)
	    {
	      fprintf (out,"0,%u,0,0,0,%d,0,%lu\n",
		       event_type,
		       kc,
		       xrecintd->server_time
		       );
	      ret = xnee_fake_key_event  (xd, kc, True, CurrentTime );
	      XNEE_RETURN_IF_ERR (ret);
	      
	    }
	  else if (do_print==XNEE_GRAB_DO_SAVE)
	    {
	      char buf[64];
	      ret = snprintf (buf, 64, "0,%u,0,0,0,%d,0,%lu\n",
			      event_type,
			      kc,
			      xrecintd->server_time);
	      if (ret<3)
		{
		  return   XNEE_MEMORY_FAULT    ;
		}
	      ret = xnee_grab_handle_buffer (xd, buf, XNEE_GRAB_BUFFER_SAVE);
	      XNEE_RETURN_IF_ERR(ret);
	    }
	  else if (do_print==XNEE_GRAB_DONT_PRINT)
	    {
	      return XNEE_GRAB_CONFUSION;
	    }
	  break;
	case KeyRelease:
	  if  ( event_base > 0 ) { break; }
	  kc = xrec_data->event.u.u.detail;
	  do_print = xnee_save_or_print(xd, kc, XNEE_GRAB_KM_RELEASE);
	  
	  if (do_print==XNEE_GRAB_DO_PRINT)
	    {
	      fprintf (out,"0,%u,0,0,0,%d,0,%lu\n",
		       event_type,
		       xrec_data->event.u.u.detail,
		       xrecintd->server_time
		       );
	      ret = xnee_fake_key_event  (xd, (int) xrec_data->event.u.u.detail , False, CurrentTime);
	      XNEE_RETURN_IF_ERR(ret);
	    }
	  else if (do_print==XNEE_GRAB_DO_SAVE)
	    {
	      char buf[64];
	      ret = snprintf (buf, 64, "0,%u,0,0,0,%d,0,%lu\n",
			      event_type,
			      kc,
			      xrecintd->server_time);
	      if (ret<3)
		{
		  return   XNEE_MEMORY_FAULT    ;
		}
	      ret = xnee_grab_handle_buffer (xd, buf, XNEE_GRAB_BUFFER_SAVE);
	      XNEE_RETURN_IF_ERR(ret);
	      
	    }
	  break;
	case ButtonPress:
	  if  ( event_base > 0 ) { break; }
	  fprintf (out,"0,%u,0,0,%d,0,0,%lu\n",
		   event_type,
		   xrec_data->event.u.u.detail,
		   xrecintd->server_time
		   );
	  ret = xnee_fake_button_event(xd, (int)xrec_data->event.u.u.detail , True, CurrentTime);
	  XNEE_RETURN_IF_ERR(ret);
	  break;
	case ButtonRelease:
	  if  ( event_base > 0 ) { break; }
	  fprintf (out,"0,%u,0,0,%d,0,0,%lu\n",
		   event_type,
		   xrec_data->event.u.u.detail,
		   xrecintd->server_time
		   );
	  ret = xnee_fake_button_event(xd,  (int)xrec_data->event.u.u.detail, False, CurrentTime);
	  XNEE_RETURN_IF_ERR(ret);
	  
	  break;
	case MotionNotify:  
	  if  ( event_base > 0 ) { break; }
	  kc = 0;
	  ret = xnee_get_screen_nr(xd, 
				   xd->data,
				   xrec_data->event.u.keyButtonPointer.root);
	  if (0)
	    {
	      /* Discarding core event that seems to have come from XI */
	      /* ... anyhow, it's recorded as an XI event  */
	      ret = XNEE_OK;
	    }
	  else
	    {
	      screen = ret;
	      do_print = xnee_save_or_print(xd, kc, XNEE_GRAB_MOUSE);
	      fprintf (out,"0,%u,%d,%d,0,0,%u,%lu\n",
		       event_type,
		       xrec_data->event.u.keyButtonPointer.rootX,
		       xrec_data->event.u.keyButtonPointer.rootY,
		       screen,
		       xrecintd->server_time
		       );
	      
	      
	      xd->xnee_info.last_motion = True ;
	      ret = xnee_fake_motion_event (xd,
					    screen, 
					    xrec_data->event.u.keyButtonPointer.rootX, 
					    xrec_data->event.u.keyButtonPointer.rootY, 
					    CurrentTime);
	      XNEE_RETURN_IF_ERR(ret);
	    }
	  
	  break;
	case CreateNotify:
	  fprintf (out,"0,%u,0,0,0,0,0," TIME_PRINTF_FMT "\n",
		   event_type,
		   xrecintd->server_time
		   );
	  break;
	case DestroyNotify:
	  fprintf (out,"0,%u,0,0,0,0,0,%lu\n", 
		   event_type,
		   xrecintd->server_time
		   );
	  break;
	case ReparentNotify:
	  
	  if ( ! xnee_is_no_reparent_recording(xd))
	    {
	      fprintf (out,"0,%u,0,0,0,0,0,%lu\n", 
		       event_type,
		       xrecintd->server_time );
	    } 
	  XGetWindowAttributes(xd->grab, 
			       xrec_data->event.u.reparent.window,
			       &window_attributes_return);
	  
	  XTranslateCoordinates (xd->grab, 
				 xrec_data->event.u.reparent.window, 
				 window_attributes_return.root, 
				 -window_attributes_return.border_width,
				 -window_attributes_return.border_width,
				 &rx, 
				 &ry, 
				 &dummy_window);
	  
	  /* 
	   * Prevent the same window pos to be printed more than once 
	   */
	  if ( (last_record_window_pos_win != 
		xrec_data->event.u.reparent.window) ||
	       (last_record_window_pos_par != 
		xrec_data->event.u.reparent.parent) )
	    {
	      
	      XFlush(xd->grab);
	      if (!XFetchName(xd->grab, xrec_data->event.u.reparent.window, &win_name)) 
		{ /* Get window name if any */
		  xnee_verbose((xd," window has has no name\n"));
		  win_name=NULL;
		} 
	      else if (win_name) 
		{
		  xnee_verbose((xd," window has has name '%s'\n", win_name));
		}
	      fprintf (out, 
		       "%s:%d,%d:%u,%u,%u,%u,%u,%d:%dx%d+%d+%d:%d,%d:%s\n", 
		       XNEE_NEW_WINDOW_MARK,
		       rx,
		       ry,
		       (unsigned int) xrec_data->event.u.reparent.event,
		       (unsigned int) xrec_data->event.u.reparent.window,
		       (unsigned int) xrec_data->event.u.reparent.parent,
		       (unsigned int) xrec_data->event.u.reparent.x,
		       (unsigned int) xrec_data->event.u.reparent.y,
		       xrec_data->event.u.reparent.override,
		       window_attributes_return.x,
		       window_attributes_return.y,
		       window_attributes_return.width,
		       window_attributes_return.height,
		       window_attributes_return.border_width,
		       window_attributes_return.border_width,
		       win_name?win_name:""
		       );
	      if (win_name) 
		{
		  xnee_verbose((xd," freeing window name\n"));
		  XFree(win_name);
		} 
	      last_record_window_pos_win = 
		xrec_data->event.u.reparent.window;
	      last_record_window_pos_par = 
		xrec_data->event.u.reparent.parent;
	    }
	  break;
	default:
	  {
	    fprintf (out,"0,%u,0,0,0,0,0,%lu\n", 
		     event_type,
		     xrecintd->server_time);
	  }
	} 
    }
  return XNEE_OK;
}
  
  /**************************************************************
 *                                                            *
 * xnee_record_handle_event                                   *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_record_handle_event ( xnee_data *xd, /*@null@*/ XRecordInterceptData *xrecintd)
{
  XRecordDatum *xrec_data ;
  unsigned int  event_type ;
  FILE *out ;

   if ( xrecintd==NULL )
   {
      return XNEE_RECORD_FAILURE;
   }
   
  xrec_data  = (XRecordDatum *) (xrecintd->data) ;
  event_type = (unsigned int)xrec_data->type ;
  out = xd->out_file;

  XNEE_DEBUG ( (stderr ," -->xnee_record_handle_event()  \n"  ));
  if ( (xd->xnee_info.first_last!=0) && 
       (xd->xnee_info.last_motion > 0) && 
       (event_type==MotionNotify)) 
    {
      XNEE_DEBUG ( (stderr ," -- xnee_record_handle_event() at 0  \n"  ));
      xd->xnee_info.x           = xrec_data->event.u.keyButtonPointer.rootX ;
      xd->xnee_info.y           = xrec_data->event.u.keyButtonPointer.rootY ;

      /*@ignore@*/ 
      xd->xnee_info.server_time = xrecintd->server_time ;
      /*@end@*/
    }
  else 
    { 
      XNEE_DEBUG ( (stderr ," -- xnee_record_handle_event() at 1  \n"  ));
      if ( (xd->xnee_info.first_last!=0) && 
	   (xd->xnee_info.last_motion!=0) &&
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
	  
	  xnee_record_handle_event_printer(xd,
					   event_type, 
					   xrec_data, 
					   xrecintd);
	}
      else 
	{
	  xnee_record_handle_event_printer(xd,
					   event_type, 
					   xrec_data, 
					   xrecintd);
	}
      XNEE_DEBUG ( (stderr ," -- xnee_record_handle_event() at 7  \n"  ));
      
    }
  (void)fflush (out);
  if (((event_type==KeyPress) || (event_type==KeyRelease)
       || (event_type==ButtonPress) || (event_type==ButtonRelease)) 
      && (xd->xnee_info.last_motion!=0) )
    xd->xnee_info.last_motion=0 ;
  XNEE_DEBUG ( (stderr ," <-- xnee_record_handle_event()  \n"  ));
  
  return XNEE_OK;
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
  XRecordDatum *xrec_data  ;
  xnee_data    *xd ;
  int           data_type = 0;  
  int           ret;

  XNEE_DEBUG ( (stderr ," --> xnee_record_dispatch()  \n"  ));

  if ( (data==NULL) || (!data->data) )
    {
      XNEE_DEBUG ( (stderr ," <-- xnee_record_dispatch()  \n"  ));
      (void)xnee_process_count(XNEE_PROCESS_RESET);
      return;
    } 
  else
    {
      /*increment the process reply counter*/
       (void)xnee_process_count(XNEE_PROCESS_INC);
    }
  

  /* fix by Ton van Vliet */
  xd = (xnee_data *) (xpoint_xnee_data) ;

  if (!xd->in_use)
    {
      return;
    }

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
  if(data->data_len!=0) 
    {
       data_type = (int) xrec_data->type;
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
	  ret = xnee_record_handle_event (xd, data) ;
	  /* 
	   * Nothing will happen if we get XNEE_OK and set
	   *   the interrupt var to that value.
	   * If ret!=XNEE_OK, we must set it!!! 
	   */ 
	  xnee_set_interrupt_error(xd, ret);
          XNEE_SILENTLY_RETURN_VOID_IF_ERR (ret);
	}
      else
	{ 
           xnee_record_print_reply (xd, data) ;
	}
      break;   
    case XRecordClientStarted:
      xnee_verbose((xd,  "ClientStarted \n"));
      fprintf(stderr,  "ClientStarted %lu %p \n", 
	      data->id_base, 
	      data->data);
      break;
    case XRecordClientDied:
      xnee_verbose((xd,  "ClientDied \n"));
      fprintf(stderr,  "ClientDied \n");
      break;
    case XRecordEndOfData:
      xnee_verbose((xd,  "EndOfData \n"));
      fprintf(stderr,  "EndOfData \n");
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
  
  if ( (data==NULL) || (!data->data) )
    {
      XRecordFreeData(data);
      XNEE_DEBUG ( (stderr ," <-- xnee_human_dispatch()  \n"  ));
      (void)xnee_process_count(XNEE_PROCESS_RESET);
      return;
    } 
  else
    {
      /*increment the process reply counter*/
       (void)xnee_process_count(XNEE_PROCESS_INC);
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
  if(data->data_len!=0) 
    {
       data_type = (int) xrec_data->type;
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
  xd->xnee_info.events_recorded = 0    ; 
  xd->xnee_info.store_mouse_pos = 0;
   
  xd->xnee_info.data_recorded   = 0    ; 
   
  xd->xnee_info.time_recorded   = 0    ;
   
  xd->xnee_info.events_max      = -1    ; 
   
  xd->xnee_info.data_max        = -1    ; 
   
  xd->xnee_info.time_max        = -1    ; 
   
  xd->xnee_info.x               = 0      ; /* Need not to be set, anyhow last MotionNotify x=0*/
  xd->xnee_info.y               = 0      ; /* Need not to be set, anyhow last MotionNotify x=0*/
  xd->xnee_info.server_time     = 0      ; /* Need not to be set, anyhow last .... */  

  xd->xnee_info.interval        = 0      ;
  xd->xnee_info.size            = 0      ;
  xd->xnee_info.size            = 0      ;

  xd->xnee_info.replayed_events = 0      ;
  xd->xnee_info.interrupt       = 0;

  xnee_set_override_display(xd,XNEE_OVERRIDE_DISPLAY_NONE);

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

  if ( (xd == NULL) ||
       (xd->record_setup == NULL))
  {
     return XNEE_RECORD_FAILURE;
  }

  xnee_verbose((xd, " ---> xnee_setup_recordext\n"));

  xd->record_setup->data_flags = XRecordFromServerTime 
    | XRecordFromClientTime  
    | XRecordFromClientSequence; 

  if ( xnee_is_recorder(xd)==False)
    {
      return XNEE_OK;
    }
  
#ifdef  XNEE_XINPUT_SUPPORT
     xnee_xinput_add_devices(xd);
     xnee_record_print_record_range (xd, xnee_get_out_file (xd)) ;
#endif /*  XNEE_XINPUT_SUPPORT */



  
  ret = xnee_parse_range (xd, XNEE_DELIVERED_EVENT, "ReparentNotify");
  XNEE_RETURN_IF_ERR (ret);

  ret = xnee_set_ranges(xd);
  XNEE_RETURN_IF_ERR (ret);

/*   xnee_record_print_record_range(xd, stderr); */
  
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
  int ok=XNEE_OK;

  if  ( (xd == NULL)
        ||
        (xd->control == NULL))
  {
    fprintf(stderr, "cannot look for RECORD extension withou xnee_data or control display %p %p \n", 
	    (void*)xd, (void*)xd->control);
    return XNEE_RECORD_FAILURE;
  }

  if( XRecordQueryVersion(xd->control, 
                          &xd->record_setup->major_return, 
                          &xd->record_setup->minor_return) == 0 )
    {
      xnee_print_error ("Record extension missing."
			" This is not an error in " 
			PACKAGE "\n");
      ok=XNEE_NO_REC_EXT;
    }
  else
  {
     xnee_verbose ((xd, "\t  XRecord-\n\t  Release         %d.%d\n", 
                    xd->record_setup->major_return, 
                    xd->record_setup->minor_return));  
  }
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
  Display *context_display;
  nr_of_ranges=xnee_get_max_range(xd);
  xnee_verbose((xd, "--->xnee_setup_recording\n"));

  if ( (xd == NULL)
       ||
       (xd->control == NULL)
       ||
       (xd->record_setup == NULL))
  {
     return XNEE_RECORD_FAILURE;
  }


  XNEE_DEBUG ( (stderr ," --> xnee_setup_recording()  \n"  ));

  (void)XSynchronize(xd->control, True); 

  if (xd->all_clients!=0)
    {
      xd->record_setup->xids[0] = XRecordAllClients; 
    }
  else
    {
      xd->record_setup->xids[0] = XRecordFutureClients; 
    }

  context_display = xnee_get_display_for_recordcontext(xd);

  xd->record_setup->rContext = 
    XRecordCreateContext(context_display, 
			 xd->record_setup->data_flags, 
			 xd->record_setup->xids,1, 
			 xd->record_setup->range_array, 
			 nr_of_ranges);
  XFlush(context_display);

  XFlush(xd->control);
  XFlush(xd->data);
  XSync(xd->control, True);
  XSync(xd->data, True);

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
  Display *context_display;

   if (xd==NULL)
     {
       return XNEE_RECORD_FAILURE;
     }
   if ( (xd->control == NULL) || 
        (xd->record_setup == NULL))
     {
       return XNEE_OK; 
     }



  context_display = xnee_get_display_for_recordcontext(xd);

  xnee_verbose((xd, "---> xnee_unsetup_recording\n"));

  if (xd->record_setup->rContext != 0)
    {
      xnee_verbose((xd, "---  disabling context %p on %p \n", 
		    (void*)xd->record_setup->rContext, 
		    (void*)context_display));

      (void)XRecordDisableContext(xd->control, 
				  xd->record_setup->rContext);

      xnee_verbose((xd, "---  freeing context \n"));
      (void)XRecordFreeContext(xd->control, 
			       xd->record_setup->rContext);

      xd->record_setup->rContext = 0;
    }


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
   int ret ; 
   Display *context_display;

   if ( (xd==NULL)
        || 
        (xd->data==NULL)
        || 
        (xd->record_setup==NULL))
     {
       return XNEE_RECORD_FAILURE;
     }
   
   
   xnee_verbose((xd, " ---> xnee_record_loop()\n"));
  

  context_display = xnee_get_display_for_recordcontext(xd);

   /* 
    * In case the key pressed to invoke Xnee is not released
    * we wait 1/2 of a second and hopefully it is. If not
    * the user is holding it pressed for "TOO" long.
    */
   /*@ ignore @*/
   (void) usleep ( XNEE_DELAY_RECORDING );
   /*@ end @*/
   
   ret = XRecordEnableContextAsync(context_display, 
				   xd->record_setup->rContext, 
				   xd->rec_callback, 
				   (XPointer) (xd) /* closure passed to Dispatch */);
   
   XNEE_RETURN_IF_ERR(ret);
   
   xnee_verbose((xd, " <--- xnee_record_loop()\n"));
   /*  while (1) 
       {
       XRecordProcessReplies (xd->data); 
       }
   */
   return XNEE_OK;
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
   int ret ;
   Display *context_display;

   if ( (xd==NULL)
        || 
        (xd->data==NULL)
        || 
        (xd->record_setup==NULL))
     {
       return XNEE_RECORD_FAILURE;
     }
   
  xnee_verbose((xd, " ---> xnee_record_async() \n"));
  
  /* 
   * In case the key pressed to invoke Xnee is not released
   * we wait 1/2 of a second and hopefully it is. If not
   * the user is holding it pressed for "TOO" long.
   */
  usleep ( XNEE_DELAY_RECORDING );

  if ( xnee_is_store_mouse_pos(xd) != 0  )
    {
      xnee_store_mouse_pos (xd);
    }


  context_display = xnee_get_display_for_recordcontext(xd);
  

  xnee_verbose((xd, " --- xnee_record_async() enable context\n"));

  ret = XRecordEnableContextAsync(context_display, 
			     xd->record_setup->rContext, 
			     xd->rec_callback, 
			     (XPointer) (xd) /* closure passed to Dispatch */);


  if (ret==0)
  {
     return (XNEE_RECORD_FAILURE);
  }
  

/*   XNEE_RETURN_IF_ERR(ret); */


  for (;;) 
    {
      xnee_verbose((xd, " --- xnee_record_async() loop\n"));

      /* Interrupt variable set? */
      if (xnee_is_interrupt_action(xd))
	{
	  ret = xnee_get_interrupt_action(xd);
	  xnee_verbose((xd, "interrupt (record) variable was set (%d)\n",
			ret));
	  
	  xnee_unset_interrupt_action(xd);
	  xnee_verbose((xd, "leaving loop  (%d)\n", 
			ret));

	  break;
	}

      xnee_verbose((xd, " --- xnee_record_async() loop 2 \n"));

      /* has the user pressed a modifier+key */
      if (xnee_check_key (xd)==XNEE_GRAB_DATA)
	{

	  ret = xnee_handle_rec_key(xd) ; 
	  if (ret == XNEE_GRAB_STOP)
	    {
	      xnee_verbose  ((xd," breaking async loop since STOP \n"));
	      xd->in_use=0;
	      break;
	    }
	  else if (ret == XNEE_GRAB_RESUME)
	    {
	      xnee_verbose  ((xd," starting async loop since RESUME \n"));
	      ret = XRecordEnableContextAsync(context_display, 
                                              xd->record_setup->rContext, 
                                              xd->rec_callback, 
                                              (XPointer) (xd) );
	      xnee_verbose  ((xd," starting async loop since RESUME finished \n"));
              if(ret!=1)
		{
		  return XNEE_RECORD_FAILURE;
		}
	    }
	}


      xnee_verbose((xd, " --- xnee_record_async() loop 3\n"));

      /* handle data in the RECORD buffer */
      ret = xnee_process_replies(xd);
      XNEE_RETURN_IF_ERR(ret);

      if ( xnee_more_to_record(xd) == 0 ) 
	{
	  xnee_verbose  ((xd," closing down while loop in async loop\n"));
	  ret=XNEE_OK;
	  break ; 
	}

      /* sleep for a little while 
       * there might be someone wanting the CPU
       */  
      usleep (100*100);
    }

  if (ret == XNEE_OK)
    {
      ret = xnee_stop_session(xd);
    }
  else
    {
      xnee_stop_session(xd);
    }

  xnee_verbose((xd, " <--- xnee_record_async()\n"));
  return (ret);
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


