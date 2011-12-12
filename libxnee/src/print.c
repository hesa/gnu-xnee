/*
  Xnee's Not an Event Emulator enables recording and replaying of X 
  protocol data
  Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 
                2007, 2008, 2009, 2010, 2011 Henrik Sandklef

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 3
  of the License, or any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Boston, MA  02110-1301, USA.
*/



#include "libxnee/xnee.h"
#include "libxnee/print_varargs.h"
#include "libxnee/print.h"
#include "libxnee/datastrings.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"
#include "libxnee/xnee_resource.h"
#include "libxnee/xnee_threshold.h"
#include "libxnee/feedback.h"
#include "libxnee/xnee_error.h"
#include "libxnee/xnee_session.h"
#include "libxnee/xnee_xinput.h"


#define XNEE_HP_SEP " "
#define XNEE_HP_EQUALS "="
#define XNEE_HP_CONTENT_BEGIN " { "
#define XNEE_HP_CONTENT_END " } "


/*
 * 
 * Prints the number and name of the XEvent  
 *
 */
int
xnee_print_event_info (xnee_data *xd) 
{
  int i = 2 ; 
  char *tmp = NULL; 

  fprintf (xd->out_file,"  X11 Event\n\n");
  fprintf (xd->out_file,"number\tname\n");
  fprintf (xd->out_file,"======\t====\n");
  
  tmp = xnee_print_event(i) ; 
  while (tmp!=NULL)
    {
      if (tmp!=NULL)
	{
	  fprintf (xd->out_file,"%.2d\t%s\n",i,tmp);
	}
      i++;
      tmp = xnee_print_event(i) ; 
    }
  return XNEE_OK;
}


/*
 * 
 * Prints the number and name of the X11 error
 *
 */
int
xnee_print_error_info (xnee_data *xd) 
{
  int i = 0 ; 
  char *tmp = NULL; 
  fprintf (xd->out_file,"  X11 Error\n\n");
  fprintf (xd->out_file,"number\tname\n");
  fprintf (xd->out_file,"======\t====\n");
  tmp = xnee_print_error_code(i) ; 
  while (tmp!=NULL)
    {
      if (tmp!=NULL)
	{
	  fprintf (xd->out_file,"%.2d\t%s\n",i,tmp);
	}
      i++;
      tmp = xnee_print_error_code(i) ; 
    }

  return XNEE_OK;
}


/*
 * 
 * Prints the number and name of the X11 error
 *
 */
int
xnee_print_request_info (xnee_data *xd) 
{
  int i=1 ; 
  char *tmp = NULL ; 

  fprintf (xd->out_file,"  X11 Request\n\n");
  fprintf (xd->out_file,"number\tname\n");
  fprintf (xd->out_file,"======\t====\n");
  tmp = xnee_print_request(i); 
  while (tmp!=NULL)
    {
      if (tmp!=NULL)
	{
	  fprintf (xd->out_file,"%.2d\t%s\n",i,tmp);
	}
      i++;
      tmp = xnee_print_request(i); 
    }

  return XNEE_OK;
}

/*
 * 
 * Prints the number and name of the X11 reply
 *
 */
int
xnee_print_reply_info (xnee_data *xd) 
{
  (void) fprintf (xd->out_file,"  X11 Reply\n\n");
  return XNEE_OK;
}

/*
 * 
 * Prints the number and name of the X11 data
 *
 */
int
xnee_print_data_info (xnee_data *xd) 
{
  int ret ; 

  ret = xnee_print_event_info (xd) ;
  XNEE_RETURN_IF_ERR(ret);

  ret = xnee_print_error_info (xd) ;
  XNEE_RETURN_IF_ERR(ret);

  ret = xnee_print_request_info (xd) ;
  XNEE_RETURN_IF_ERR(ret);

  ret = xnee_print_reply_info (xd) ;
  XNEE_RETURN_IF_ERR(ret);
   
  return XNEE_OK;
}





  
/*
 * Print character representation of event if verbose mode is on.  
 */
void 
xnee_verbose_event (xnee_data * xd, int ev ) {
  if ( xd->verbose != 0 ) 
    {
      (void)xnee_print_event (ev);
    }
}




/*
 *
 * xnee_record_print_request.  
 *
 */
void 
xnee_record_print_request (xnee_data *xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data  ;
  int           req_type    ;
  xResourceReq  *req;

  if ( xrecintd == NULL)
    {
      return;
    }

  xrec_data =  (XRecordDatum *) xrecintd->data ;
  req_type  =  (int)            xrec_data->type ;
  req       =  (xResourceReq *) &xrec_data->req ;
  (void) xd->data_fp (xd->out_file,"1,%d,%lu,%lu,%lu,%lu\n", 
		      req_type, 
		      req->reqType,
		      req->length,
		      req->id,
		      xrecintd->server_time);
}



/*
 *
 * xnee_human_print_request.  
 *
 */
void 
xnee_human_print_request (xnee_data *xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data;
  int           request_type ;
  char  *request_name ;
  char  *name=" not defined " ;

  if ( xrecintd == NULL)
    {
      return;
    }


  xrec_data  = (XRecordDatum *) (xrecintd->data) ;

  request_type = (int)xrec_data->type ;

  request_name = xnee_print_request(request_type);
    if ( request_name == NULL )
      {
	request_name = name;
      }

  (void)xd->data_fp (xd->out_file,"Request" XNEE_HP_EQUALS "%s" XNEE_HP_SEP "Number" XNEE_HP_EQUALS "%d", 
		     request_name, request_type);
  
  if ( 1 )
    {
      (void)xd->data_fp (xd->out_file,XNEE_HP_CONTENT_BEGIN); 
      xnee_human_print_request_verbose (xd,xrecintd );
      (void)xd->data_fp (xd->out_file,XNEE_HP_CONTENT_END); 
    }

  (void)xd->data_fp (xd->out_file,"\n");
}


/*
 *
 * xnee_human_print_request.  
 *
 */
void 
xnee_human_print_request_verbose (xnee_data *xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data;
  int           req_type ;
  xCreateWindowReq *req_create_ptr;

  xrec_data = (XRecordDatum *) (xrecintd->data) ;
  req_type  = (int) xrec_data->type ;
  
  switch (req_type)
    {
    case X_CreateWindow:
      req_create_ptr = (xCreateWindowReq *) (xrecintd->data) ;
      (void)xd->data_fp (xd->out_file,
			 XNEE_HP_SEP "reqType" XNEE_HP_EQUALS "%c"
			 XNEE_HP_SEP "depth"   XNEE_HP_EQUALS "%d"
			 XNEE_HP_SEP "length"  XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "wid"     XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "parent"  XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "width"   XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "height"  XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "borderWidth" XNEE_HP_EQUALS "%lu"
#if defined(__cplusplus) || defined(c_plusplus)
			 XNEE_HP_SEP "c_class" XNEE_HP_EQUALS "%lu"
#else
			 XNEE_HP_SEP "class"   XNEE_HP_EQUALS "%lu"
#endif
			 XNEE_HP_SEP "visual"  XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "mask"    XNEE_HP_EQUALS "%lu",
			 req_create_ptr->reqType, 
			 req_create_ptr->depth, 
			 req_create_ptr->length, 
			 req_create_ptr->wid, 
			 req_create_ptr->parent, 
			 req_create_ptr->width, 
			 req_create_ptr->height, 
			 req_create_ptr->borderWidth, 
#if defined(__cplusplus) || defined(c_plusplus)
			 req_create_ptr->c_class, 
#else
			 req_create_ptr->class, 
#endif
			 req_create_ptr->visual, 
			 req_create_ptr->mask);
     break;
    case sz_xReparentWindowReq:
      (void)xd->data_fp (xd->out_file,
			 XNEE_HP_SEP "length" XNEE_HP_EQUALS "%lu  RE ",
			 xrec_data->req.length);
     break;
    default:
      (void)xd->data_fp (xd->out_file, " NOT IMPLEMENTED REQUEST ");
      break;
    }
}

/*
 *
 * xnee_human_print_event.  
 *
 */
void 
xnee_human_print_event (xnee_data *xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data  ;
  int           event_type ;
  

  char  *event_name ;
  char  *name=" not defined " ;
  xrec_data  = (XRecordDatum *) (xrecintd->data) ;
  event_type = (int)xrec_data->type ;

  event_name = xnee_print_event(event_type);
    if ( event_name == NULL )
      {
	event_name = name;
      }

  (void)xd->data_fp (xd->out_file,"Event" XNEE_HP_EQUALS "%s" XNEE_HP_SEP "Number" XNEE_HP_EQUALS "%d", 
		     event_name, event_type);
  
  xnee_handle_xinput_event_human(xd, 
				 event_type, 
				 xrec_data,
				 xrecintd->server_time);

  if ( 1 )
    {
      (void)xd->data_fp (xd->out_file,XNEE_HP_CONTENT_BEGIN); 
      xnee_human_print_event_verbose (xd,xrecintd );
      (void)xd->data_fp (xd->out_file,XNEE_HP_CONTENT_END); 
    }

  (void)xd->data_fp (xd->out_file,"\n");
}


/*
 *
 * xnee_human_print_event.  
 *
 */
void 
xnee_human_print_event_verbose (xnee_data *xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data  ;
  int           event_type ;

  xrec_data  = (XRecordDatum *) (xrecintd->data) ;
  event_type = (int)xrec_data->type ;


  (void)xd->data_fp (xd->out_file, "{ ");
  switch (event_type)
    {
    case KeyPress:
    case KeyRelease:
    case ButtonPress:
    case ButtonRelease:
    case MotionNotify:
      (void)xd->data_fp (xd->out_file,
			  "root" XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "event" XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "child" XNEE_HP_EQUALS "%lu", 
			 xrec_data->event.u.keyButtonPointer.root,
			 xrec_data->event.u.keyButtonPointer.event,
			 xrec_data->event.u.keyButtonPointer.child
			 );
      (void)xd->data_fp (xd->out_file,
			 XNEE_HP_SEP "rootX" XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "rootY" XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "eventX" XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "eventY" XNEE_HP_EQUALS "%lu", 
			 xrec_data->event.u.keyButtonPointer.rootX,
			 xrec_data->event.u.keyButtonPointer.rootY,
			 xrec_data->event.u.keyButtonPointer.eventX,
			 xrec_data->event.u.keyButtonPointer.eventY
			 );
      (void)xd->data_fp (xd->out_file,
			 XNEE_HP_SEP "state" XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "sameScreen" XNEE_HP_EQUALS "%d", 
			 xrec_data->event.u.keyButtonPointer.state,
			 xrec_data->event.u.keyButtonPointer.sameScreen			 );
      break;
    case EnterNotify:
    case LeaveNotify:
      (void)xd->data_fp (xd->out_file,
			  "root" XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "event" XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "child" XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "rootX" XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "rootY" XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "eventX" XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "eventY" XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "state" XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "mode" XNEE_HP_EQUALS "%d", 
			 xrec_data->event.u.enterLeave.root,
			 xrec_data->event.u.enterLeave.event,
			 xrec_data->event.u.enterLeave.child,
			 xrec_data->event.u.enterLeave.rootX,
			 xrec_data->event.u.enterLeave.rootY,
			 xrec_data->event.u.enterLeave.eventX,
			 xrec_data->event.u.enterLeave.eventY,
			 xrec_data->event.u.enterLeave.state,
			 xrec_data->event.u.enterLeave.mode);
      break;

    case FocusIn:
    case FocusOut:
      (void)xd->data_fp (xd->out_file,
			  "window" XNEE_HP_EQUALS "%lu"
			 XNEE_HP_SEP "mode" XNEE_HP_EQUALS "%lu", 
			 xrec_data->event.u.focus.window,
			 xrec_data->event.u.focus.mode);
      break;
    case Expose:
      (void)xd->data_fp (xd->out_file,
			  "window" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "x" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "y" XNEE_HP_EQUALS "%d"  
			 XNEE_HP_SEP "width" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "height" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "count" XNEE_HP_EQUALS "%d", 
			 xrec_data->event.u.expose.window,
			 xrec_data->event.u.expose.x,
			 xrec_data->event.u.expose.y,
			 xrec_data->event.u.expose.width,
			 xrec_data->event.u.expose.height,
			 xrec_data->event.u.expose.count);
      break;
    case GraphicsExpose:
      (void)xd->data_fp (xd->out_file,
			  "drawable" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "x" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "y" XNEE_HP_EQUALS "%d"  
			 XNEE_HP_SEP "width" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "height" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "minorEvent" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "majorEvent" XNEE_HP_EQUALS "%d"
			 XNEE_HP_SEP "count" XNEE_HP_EQUALS "%d", 
			 xrec_data->event.u.graphicsExposure.drawable,
			 xrec_data->event.u.graphicsExposure.x,
			 xrec_data->event.u.graphicsExposure.y,
			 xrec_data->event.u.graphicsExposure.width,
			 xrec_data->event.u.graphicsExposure.height,
			 xrec_data->event.u.graphicsExposure.minorEvent,
			 xrec_data->event.u.graphicsExposure.majorEvent,
			 xrec_data->event.u.graphicsExposure.count);
      break;
    case NoExpose:
      (void)xd->data_fp (xd->out_file,
			  "drawable" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "minorEvent" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "majorEvent" XNEE_HP_EQUALS "%d" , 
			 xrec_data->event.u.noExposure.drawable,
			 xrec_data->event.u.noExposure.minorEvent,
			 xrec_data->event.u.noExposure.majorEvent);
      break;
    case VisibilityNotify:
      (void)xd->data_fp (xd->out_file,
			  "window" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "state" XNEE_HP_EQUALS "%d" , 
			 xrec_data->event.u.visibility.window,
			 xrec_data->event.u.visibility.state);
      break;
    case CreateNotify:
      (void)xd->data_fp (xd->out_file,
			  "parent" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "window" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "x" XNEE_HP_EQUALS "%d"  
			 XNEE_HP_SEP "y" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "width" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "height" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "borderWidth" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "override" XNEE_HP_EQUALS "%d" ,
			 xrec_data->event.u.createNotify.parent,
			 xrec_data->event.u.createNotify.window,
			 xrec_data->event.u.createNotify.x,
			 xrec_data->event.u.createNotify.y,
			 xrec_data->event.u.createNotify.width,
			 xrec_data->event.u.createNotify.height,
			 xrec_data->event.u.createNotify.borderWidth,
			 xrec_data->event.u.createNotify.override);
      break;
    case DestroyNotify:
      (void)xd->data_fp (xd->out_file,
			  "event" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "window" XNEE_HP_EQUALS "%lu" ,
			 xrec_data->event.u.destroyNotify.event,
			 xrec_data->event.u.destroyNotify.window);
      break;
    case UnmapNotify:
      (void)xd->data_fp (xd->out_file,
			  "event" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "window" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "fromConfigure" XNEE_HP_EQUALS "%d" ,
			 xrec_data->event.u.unmapNotify.event,
			 xrec_data->event.u.unmapNotify.window,
			 xrec_data->event.u.unmapNotify.fromConfigure);
      break;
    case MapNotify:
      (void)xd->data_fp (xd->out_file,
			  "event" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "window" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "override" XNEE_HP_EQUALS "%d" ,
			 xrec_data->event.u.mapNotify.event,
			 xrec_data->event.u.mapNotify.window,
			 xrec_data->event.u.mapNotify.override);
      break;
    case MapRequest:
      (void)xd->data_fp (xd->out_file,
			  "parent" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "window" XNEE_HP_EQUALS "%lu",
			 xrec_data->event.u.mapRequest.parent,
			 xrec_data->event.u.mapRequest.window);
      break;
    case ReparentNotify:
      (void)xd->data_fp (xd->out_file,
			  "event" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "window" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "parent" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "x" XNEE_HP_EQUALS "%d"  
			 XNEE_HP_SEP "y" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "override" XNEE_HP_EQUALS "%d" ,
			 xrec_data->event.u.reparent.event,
			 xrec_data->event.u.reparent.window,
			 xrec_data->event.u.reparent.parent,
			 xrec_data->event.u.reparent.x,
			 xrec_data->event.u.reparent.y,
			 xrec_data->event.u.reparent.override);
      break;
    case ConfigureNotify:
      (void)xd->data_fp (xd->out_file,
			  "event" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "window" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "aboveSibling" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "parent" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "x" XNEE_HP_EQUALS "%d"  
			 XNEE_HP_SEP "y" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "width" XNEE_HP_EQUALS "%d"  
			 XNEE_HP_SEP "height" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "borderWidth" XNEE_HP_EQUALS "%d"  
			 XNEE_HP_SEP "override" XNEE_HP_EQUALS "%d" ,
			 xrec_data->event.u.configureNotify.event,
			 xrec_data->event.u.configureNotify.window,
			 xrec_data->event.u.configureNotify.aboveSibling,
			 xrec_data->event.u.configureNotify.x,
			 xrec_data->event.u.configureNotify.y,
			 xrec_data->event.u.configureNotify.width,
			 xrec_data->event.u.configureNotify.height,
			 xrec_data->event.u.configureNotify.borderWidth,
			 xrec_data->event.u.configureNotify.override);
      break;
    case ConfigureRequest:
      (void)xd->data_fp (xd->out_file,
			  "parent" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "window" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "sibling" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "x" XNEE_HP_EQUALS "%d"  
			 XNEE_HP_SEP "y" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "width" XNEE_HP_EQUALS "%d"  
			 XNEE_HP_SEP "height" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "borderWidth" XNEE_HP_EQUALS "%d"  
			 XNEE_HP_SEP "valueMask" XNEE_HP_EQUALS "%d" ,
			 xrec_data->event.u.configureRequest.parent,
			 xrec_data->event.u.configureRequest.window,
			 xrec_data->event.u.configureRequest.sibling,
			 xrec_data->event.u.configureRequest.x,
			 xrec_data->event.u.configureRequest.y,
			 xrec_data->event.u.configureRequest.width,
			 xrec_data->event.u.configureRequest.height,
			 xrec_data->event.u.configureRequest.borderWidth,
			 xrec_data->event.u.configureRequest.valueMask);
      break;

    case GravityNotify:
      (void)xd->data_fp (xd->out_file,
			 XNEE_HP_SEP "event" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "window" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "x" XNEE_HP_EQUALS "%d"  
			 XNEE_HP_SEP "y" XNEE_HP_EQUALS "%d" ,
			 xrec_data->event.u.gravity.event,
			 xrec_data->event.u.gravity.window,
			 xrec_data->event.u.gravity.x,
			 xrec_data->event.u.gravity.y);
      break;

    case ResizeRequest:
      (void)xd->data_fp (xd->out_file,
			 XNEE_HP_SEP "window" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "width" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "height" XNEE_HP_EQUALS "%d" ,
			 xrec_data->event.u.resizeRequest.window,
			 xrec_data->event.u.resizeRequest.width,
			 xrec_data->event.u.resizeRequest.height);
      break;

    case CirculateNotify:
    case CirculateRequest:
      (void)xd->data_fp (xd->out_file,
			 XNEE_HP_SEP "event" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "window" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "parent" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "place" XNEE_HP_EQUALS "%lu" ,
			 xrec_data->event.u.circulate.event,
			 xrec_data->event.u.circulate.window,
			 xrec_data->event.u.circulate.parent,
			 xrec_data->event.u.circulate.place);
      break;

    case PropertyNotify:
      (void)xd->data_fp (xd->out_file,
			 XNEE_HP_SEP "window" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "atom" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "time" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "state" XNEE_HP_EQUALS "%d" ,
			 xrec_data->event.u.property.window,
			 xrec_data->event.u.property.atom,
			 xrec_data->event.u.property.time,
			 xrec_data->event.u.property.state);
      break;

    case SelectionClear:
      (void)xd->data_fp (xd->out_file,
			 XNEE_HP_SEP "time" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "window" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "atom" XNEE_HP_EQUALS "%d" ,
			 xrec_data->event.u.selectionClear.time,
			 xrec_data->event.u.selectionClear.window,
			 xrec_data->event.u.selectionClear.atom);
      break;

    case SelectionRequest:
      (void)xd->data_fp (xd->out_file,
			 XNEE_HP_SEP "time" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "owner" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "requestor" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "selection" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "target" XNEE_HP_EQUALS "%d" ,
			 XNEE_HP_SEP "property" XNEE_HP_EQUALS "%d" ,
			 xrec_data->event.u.selectionRequest.time,
			 xrec_data->event.u.selectionRequest.owner,
			 xrec_data->event.u.selectionRequest.requestor,
			 xrec_data->event.u.selectionRequest.selection,
			 xrec_data->event.u.selectionRequest.target,
			 xrec_data->event.u.selectionRequest.property);
      break;

    case SelectionNotify:
      (void)xd->data_fp (xd->out_file,
			 XNEE_HP_SEP "time" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "requestor" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "selection" XNEE_HP_EQUALS "%d" 
			 XNEE_HP_SEP "target" XNEE_HP_EQUALS "%d" ,
			 XNEE_HP_SEP "property" XNEE_HP_EQUALS "%d" ,
			 xrec_data->event.u.selectionNotify.time,
			 xrec_data->event.u.selectionNotify.requestor,
			 xrec_data->event.u.selectionNotify.selection,
			 xrec_data->event.u.selectionNotify.target,
			 xrec_data->event.u.selectionNotify.property);
      break;

    case ColormapNotify:
      (void)xd->data_fp (xd->out_file,
			 XNEE_HP_SEP "window" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "colormap" XNEE_HP_EQUALS "%lu" 
#if defined(__cplusplus) || defined(c_plusplus)
			 XNEE_HP_SEP "c_new" XNEE_HP_EQUALS "%d" 
#else
			 XNEE_HP_SEP "new" XNEE_HP_EQUALS "%d" 
#endif
			 XNEE_HP_SEP "state" XNEE_HP_EQUALS "%d" ,
			 xrec_data->event.u.colormap.window,
			 xrec_data->event.u.colormap.colormap,
#if defined(__cplusplus) || defined(c_plusplus)
			 xrec_data->event.u.colormap.c_new,
#else
			 xrec_data->event.u.colormap.new,
#endif
			 xrec_data->event.u.colormap.state);
      break;

    case ClientMessage:
      (void)xd->data_fp (xd->out_file,
			 XNEE_HP_SEP "window" XNEE_HP_EQUALS "%lu" ,
			 xrec_data->event.u.clientMessage.window );
      /*
      (void)xd->data_fp (xd->out_file, "bytes" XNEE_HP_EQUALS " {");
      for (i=0;i<20;i++)
	{
	  (void)xd->data_fp (xd->out_file, "%c ", 
			     xrec_data->event.u.clientMessage.u.b.bytes[i]);

	}
      (void)xd->data_fp (xd->out_file, "} ");
      */
      break;

    case MappingNotify:
      (void)xd->data_fp (xd->out_file,
			 XNEE_HP_SEP "request" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "firstKeyCode" XNEE_HP_EQUALS "%lu" 
			 XNEE_HP_SEP "count" XNEE_HP_EQUALS "%d" ,
			 xrec_data->event.u.mappingNotify.request,
			 xrec_data->event.u.mappingNotify.firstKeyCode,
			 xrec_data->event.u.mappingNotify.count);
      break;

    default:
      (void)xd->data_fp (xd->out_file," NOT IMPLEMENTED ");
      break;
    }
  (void)xd->data_fp (xd->out_file, " }");

}



/*
 *
 *   xnee_record_print_reply.  
 *
 */
void xnee_record_print_reply (xnee_data* xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data ;
  int           rep_type   ;

  xrec_data = (XRecordDatum *) (xrecintd->data) ;
  rep_type  = (int)xrec_data->type ;
  
  (void)xd->data_fp (xd->out_file,"2,%d,%lu\n", rep_type,xrecintd->server_time);
}


/*
 *
 *   xnee_human_print_reply.  
 *
 */
void xnee_human_print_reply (xnee_data* xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data  ;
  int           rep_type   ;

  xrec_data = (XRecordDatum *) (xrecintd->data) ;
  rep_type  = (int)xrec_data->type ;
  
  (void)xd->data_fp (xd->out_file,"Reply       %d,%lu\n", rep_type,xrecintd->server_time);
}




/*
 *
 *   xnee_record_print_error.  
 *
 */
void xnee_record_print_error (xnee_data *xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data ;
  int           err_type  ;
  
  xrec_data = (XRecordDatum *) (xrecintd->data) ;
  err_type  = (int)xrec_data->type ;
  
  (void)xd->data_fp (xd->out_file,"3,%d,%lu\n", err_type,xrecintd->server_time);
}

/*
 *
 *   xnee_human_print_error.  
 *
 */
void xnee_human_print_error (xnee_data *xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data ;
  int           err_type  ;
  
  xrec_data = (XRecordDatum *) (xrecintd->data) ;
  err_type = (int) xrec_data->type ;
  
  (void)xd->data_fp (xd->out_file,"Error %s\n", xnee_print_error_code(err_type));
}


#ifdef USE_OBSOLETE
/*
 *
 *    xnee_record_print_record_range.  
 *
 */
void 
old_xnee_record_print_record_range (xnee_data *xd, FILE* out) 
{
  int i=0;
  int max=xnee_get_max_range(xd);
    
  if (out == NULL)
    {
      if ( !xd->verbose ) 
	{
	  return ;
	}
      out=xd->err_file;
    }

  fprintf (out, " -------------> MAX = %d <---------- \n", max);

  fprintf (out, "\n\n- Record Information -\n\n");
  fprintf (out, "   Variable              Value\n");
  fprintf (out, "---------------------------------\n");
  fprintf (out, "   data_flags            %d\n", xd->record_setup->data_flags);
  fprintf (out, "   rState                %d\n", (int)xd->record_setup->rState);
  fprintf (out, "   xids[0]               %ld\n", xd->record_setup->xids[0]);
  fprintf (out, "   xids[1]               %ld\n", xd->record_setup->xids[1]);

  fprintf (out, "\n\n      - Record Ranges -\n\n");
  for ( i=0 ; i<max ; i++ )
    {
      fprintf (out, "RecordRange[%d]\n", i);
      fprintf (out, "    Variable              Value\n");
      fprintf (out, "---------------------------------\n");
      fprintf (out, " %s           %d-%d\n", 
	       XNEE_REQUEST_STR,
	       xd->record_setup->range_array[i]->core_requests.first,
	       xd->record_setup->range_array[i]->core_requests.last       );

      fprintf (out, " %s            %d-%d\n", 
	       XNEE_REPLIES_STR,
	       xd->record_setup->range_array[i]->core_replies.first
	       , xd->record_setup->range_array[i]->core_replies.last       );

      fprintf (out, " %s  %d-%d\n", 
	       XNEE_EXT_REQ_MAJ_STR,
	       xd->record_setup->range_array[i]->ext_requests.ext_major.first,
	       xd->record_setup->range_array[i]->ext_requests.ext_major.last        );

      fprintf (out, " %s  %d-%d\n", 
	       XNEE_EXT_REQ_MIN_STR,
	       xd->record_setup->range_array[i]->ext_requests.ext_minor.first,
	       xd->record_setup->range_array[i]->ext_requests.ext_minor.last        );

      fprintf (out, " %s   %d-%d\n", 
	       XNEE_EXT_REP_MAJ_STR,
	       xd->record_setup->range_array[i]->ext_replies.ext_major.first,
	       xd->record_setup->range_array[i]->ext_replies.ext_major.last        );

      fprintf (out, " %s   %d-%d\n", 
	       XNEE_EXT_REP_MIN_STR,
	       xd->record_setup->range_array[i]->ext_replies.ext_minor.first,
	       xd->record_setup->range_array[i]->ext_replies.ext_minor.last        );

      fprintf (out, " %s        %d-%d\n", 
	       XNEE_DELIVERED_EVENT_STR,
	       xd->record_setup->range_array[i]->delivered_events.first,
	       xd->record_setup->range_array[i]->delivered_events.last      );

      fprintf (out, " %s           %d-%d\n", 
	       XNEE_DEVICE_EVENT_STR,
	       xd->record_setup->range_array[i]->device_events.first,
	       xd->record_setup->range_array[i]->device_events.last        );

      fprintf (out, " %s                  %d-%d\n", 
	       XNEE_ERROR_STR,
	       xd->record_setup->range_array[i]->errors.first,
	       xd->record_setup->range_array[i]->errors.last      );
    }
}
#endif



/*
 *
 *    xnee_record_print_sys_info.  
 * 
 */
int
xnee_print_sys_info(xnee_data *xd, FILE *out)
{
  struct utsname uname_pointer;
  time_t rawtime;
  time_t ret_time;
  struct tm * timeinfo;
  int    ret ;


  if ( (xd==NULL) || (xd->data==NULL) )
    {
      return XNEE_NO_MAIN_DATA;
    }
  if (xd->record_setup==NULL)
    {
      return XNEE_NO_RECORD_DATA;
    }

  


  ret_time = time ( &rawtime );
  if ( ret_time != rawtime)
    {
      return XNEE_DATE_FAILURE;
    }

  timeinfo = localtime ( &rawtime );
  if (timeinfo == NULL)
    {
      return XNEE_DATE_FAILURE;
    }
  
  ret = uname(&uname_pointer);
  if (ret == -1)
    {
      return XNEE_DATE_FAILURE;
    }




  if (( xd->out_file == NULL ) && (xd->mode==XNEE_RECORDER))
    {
      return XNEE_OK;
    }

  if (out == NULL)
    {
      if ( xd->verbose == 0 ) 
	{
	  return XNEE_OK;
	}
      if ( xd->out_file == NULL )
	out=xd->err_file;
      
    }



  fprintf (out, "####################################\n");
  fprintf (out, "#         System information       #\n");
  fprintf (out, "####################################\n");
  
  fprintf (out, "# Date:                   %.4d:%.2d:%.2d \n",
	   timeinfo->tm_year + 1900 , 
	   timeinfo->tm_mon + 1 , 
	   timeinfo->tm_mday  );
  fprintf (out, "# Time:                   %.2d:%.2d:%.2d \n",
	   timeinfo->tm_hour, 
	   timeinfo->tm_min , 
	   timeinfo->tm_sec  );
  if ( xd->program_name!=NULL)
    {
      fprintf (out, "# Xnee program:           %s \n", xd->program_name);
    }
  else
    {
      fprintf (out, "# Xnee program:           <missing info> \n");
    }
  fprintf (out, "# Xnee version:           %s \n", VERSION);
  fprintf (out, "# Xnee home:              %s \n", XNEE_HOME_URL);
  fprintf (out, "# Xnee info mailing list: %s \n", XNEE_MAIL);
  fprintf (out, "# Xnee bug mailing list:  %s \n", XNEE_BUG_MAIL);
  fprintf (out, "# X version:              %d \n", ProtocolVersion(xd->data));
  fprintf (out, "# X revision:             %d \n", ProtocolRevision(xd->data));
  fprintf (out, "# X vendor:               %s \n", ServerVendor (xd->data));
  fprintf (out, "# X vendor release:       %d \n", VendorRelease (xd->data));
  fprintf (out, "# Record version major:   %d\n", 
	   xd->record_setup->major_return);
  fprintf (out, "# Record version minor:   %d\n", 
	   xd->record_setup->minor_return);
  fprintf (out, "# OS name:                %s \n", uname_pointer.sysname);
  fprintf (out, "# OS Release:             %s \n", uname_pointer.release);
  fprintf (out, "# OS Version:             %s \n", uname_pointer.version);
  fprintf (out, "# Machine:                %s \n", uname_pointer.machine);
  fprintf (out, "# Nodename:               %s \n", uname_pointer.nodename);
  fprintf (out, "# Display name:           %s\n", xd->display);
  fprintf (out, "# Dimension:              %dx%d\n",
	   DisplayWidth (xd->data, 0), 
	   DisplayHeight (xd->data, 0));
  return XNEE_OK;
}


/*
 *
 * xnee_print_distr_list.  
 *
 */
int 
xnee_print_distr_list (xnee_data* xd, /*@null@*/ FILE *out)
{
  int i=0;

  if (xd==NULL) 
    {
      return XNEE_NO_MAIN_DATA;
    }

  if (xd->distr_list==NULL)
    {
      return XNEE_OK;
    }


  if (out == NULL)
    {
      if ( xd->verbose == 0 ) 
	{
	  return 0;
	}
      out=xd->err_file;
    }
  for (i=0 ; i<(int)xd->distr_list_size ; i++)
    {
      fprintf (out, "Distribution display[%d]=%p\n", 
	       i, (void*)xd->distr_list[i].dpy);
    }
  return (XNEE_OK);
}


int
xnee_record_print_record_range (xnee_data *xd, /*@null@*/ FILE* out) 
{
  int max=0;
  int i ;

  if ( (xd==NULL) || 
       (xd->record_setup==NULL) ||
       (xd->record_setup->range_array==NULL) ||
       (xd->record_setup->xids==NULL) )
    {
      return XNEE_NO_MAIN_DATA; 
    }

  if (out == NULL)
    {
      if ( xd->verbose == 0 ) 
	{
	  return XNEE_OK;
	}
      out=xd->err_file;
    }

  max=xnee_get_max_range(xd);


  fprintf (out, "#########################################\n");
  fprintf (out, "#          Record settings              #\n");
  fprintf (out, "#########################################\n");
  fprintf (out, "#   data_flags          %d\n", 
	   xd->record_setup->data_flags);
  fprintf (out, "#   rState              %p\n", 
	   (void*)xd->record_setup->rState);
  fprintf (out, "#   xids[0]             %d\n", 
	   (int) xd->record_setup->xids[0]);
  fprintf (out, "#   xids[1]             %d\n", 
	   (int) xd->record_setup->xids[1]);
  fprintf (out, "# Number of Ranges      %d\n", 
	   (int) max);


  for ( i=0 ; i<max ; i++ )
    {
      fprintf (out, "# RecordRange[%d]\n", i);
      fprintf (out, "%s            %u-%u\n", 
	       xnee_xns_key2string(XNEE_REQUEST_STR_KEY),
	       xd->record_setup->range_array[i]->core_requests.first,
	       xd->record_setup->range_array[i]->core_requests.last       );

      fprintf (out, "%s                   %u-%u \n", 
	       xnee_xns_key2string(XNEE_REPLY_STR_KEY),
	       xd->record_setup->range_array[i]->core_replies.first,
	       xd->record_setup->range_array[i]->core_replies.last       );

      fprintf (out, "%s  %u-%u \n", 
	       xnee_xns_key2string(XNEE_EXT_REQ_MAJ_STR_KEY),
	       xd->record_setup->range_array[i]->ext_requests.ext_major.first,
	       xd->record_setup->range_array[i]->ext_requests.ext_major.last);
      
      fprintf (out, "%s  %u-%u \n", 
	       xnee_xns_key2string(XNEE_EXT_REQ_MIN_STR_KEY),
	       xd->record_setup->range_array[i]->ext_requests.ext_minor.first,
	       xd->record_setup->range_array[i]->ext_requests.ext_minor.last);

      fprintf (out, "%s   %u-%u\n", 
	       xnee_xns_key2string(XNEE_EXT_REP_MAJ_STR_KEY),
	       xd->record_setup->range_array[i]->ext_replies.ext_major.first,
	       xd->record_setup->range_array[i]->ext_replies.ext_major.last);

      fprintf (out, "%s   %u-%u \n", 
	       xnee_xns_key2string(XNEE_EXT_REP_MIN_STR_KEY),
	       xd->record_setup->range_array[i]->ext_replies.ext_minor.first,
	       xd->record_setup->range_array[i]->ext_replies.ext_minor.last );

      fprintf (out, "%s         %u-%u \n", 
	       xnee_xns_key2string(XNEE_DELIVERED_EVENT_STR_KEY),
	       xd->record_setup->range_array[i]->delivered_events.first,
	       xd->record_setup->range_array[i]->delivered_events.last  );

      fprintf (out, "%s            %u-%u \n", 
	       xnee_xns_key2string(XNEE_DEVICE_EVENT_STR_KEY),
	       xd->record_setup->range_array[i]->device_events.first,
	       xd->record_setup->range_array[i]->device_events.last    );

      fprintf (out, "%s                   %u-%u \n", 
	       xnee_xns_key2string(XNEE_ERROR_STR_KEY),
	       xd->record_setup->range_array[i]->errors.first,
	       xd->record_setup->range_array[i]->errors.last      );
    }


  return XNEE_OK;
}



/*
 *
 *
 *
 */
int
xnee_print_ranges (xnee_data *xd, FILE *fp)
{
  int max=0;
  int i ;
  

  if ( (xd==NULL) || 
       (xd->record_setup==NULL) ||
       (xd->record_setup->range_array==NULL) )
    {
      return XNEE_NO_MAIN_DATA;
    }


  max=xnee_get_max_range(xd);

  fprintf (fp, "#########################################\n");
  fprintf (fp, "#          Recording ranges             #\n");
  fprintf (fp, "#########################################\n");
  for ( i=0 ; i<max ; i++ )
    {
  
      fprintf (fp, "\n# Range nr:%d\n", i);

      if ( ( (int)xd->record_setup->range_array[i]->core_requests.first != 0)
	   &&
	   ( (int)xd->record_setup->range_array[i]->core_requests.last != 0))
	{
	  fprintf (fp,  "%s %u-%u\n", 
		   xnee_xns_key2string(XNEE_REQUEST_STR_KEY),
                   xd->record_setup->range_array[i]->core_requests.first,
                   xd->record_setup->range_array[i]->core_requests.last       );
	}
      if ( ( (int)xd->record_setup->range_array[i]->core_replies.first != 0)
	   &&
	   ( (int)xd->record_setup->range_array[i]->core_replies.last != 0))
	{
	  fprintf (fp,   "%s                   %u-%u \n",
		   xnee_xns_key2string(XNEE_REPLY_STR_KEY),
		   xd->record_setup->range_array[i]->core_replies.first,
		   xd->record_setup->range_array[i]->core_replies.last       );
	}
      if ( ( (int)xd->record_setup->range_array[i]->ext_requests.ext_major.first != 0)
	   &&
	   ( (int)xd->record_setup->range_array[i]->ext_requests.ext_major.last != 0))
	{
	  fprintf (fp, "%s  %u-%u \n", 
		   xnee_xns_key2string(XNEE_EXT_REQ_MAJ_STR_KEY),
		   xd->record_setup->range_array[i]->ext_requests.ext_major.first,
		   xd->record_setup->range_array[i]->ext_requests.ext_major.last);
	}
      if ( ( (int)xd->record_setup->range_array[i]->ext_requests.ext_minor.first != 0)
	   &&
	   ( (int)xd->record_setup->range_array[i]->ext_requests.ext_minor.last != 0))
	{
	  fprintf (fp, "%s  %u-%u \n", 
		   xnee_xns_key2string(XNEE_EXT_REQ_MIN_STR_KEY),
		   xd->record_setup->range_array[i]->ext_requests.ext_minor.first,
		   xd->record_setup->range_array[i]->ext_requests.ext_minor.last        );
	}
      
      if ( ( (int)xd->record_setup->range_array[i]->ext_replies.ext_major.first != 0)
	   &&
	   ( (int)xd->record_setup->range_array[i]->ext_replies.ext_major.last != 0))
	{
	  fprintf (fp,  "%s   %u-%u\n", 
		   xnee_xns_key2string(XNEE_EXT_REP_MAJ_STR_KEY),
		   xd->record_setup->range_array[i]->ext_replies.ext_major.first,
		   xd->record_setup->range_array[i]->ext_replies.ext_major.last        );
	}
      if ( ( (int)xd->record_setup->range_array[i]->ext_replies.ext_minor.first != 0)
	   &&
	   ( (int)xd->record_setup->range_array[i]->ext_replies.ext_minor.last != 0))
	{
	  
	  fprintf (fp,  "%s   %u-%u \n", 
		   xnee_xns_key2string(XNEE_EXT_REP_MIN_STR_KEY),
		   xd->record_setup->range_array[i]->ext_replies.ext_minor.first,
		   xd->record_setup->range_array[i]->ext_replies.ext_minor.last);
	}
      
      if ( ( (int)xd->record_setup->range_array[i]->delivered_events.first != 0)
	   &&
	   ( (int)xd->record_setup->range_array[i]->delivered_events.last != 0))
	{
	  fprintf (fp, "%s         %u-%u \n", 
		   xnee_xns_key2string(XNEE_DELIVERED_EVENT_STR_KEY),
		   xd->record_setup->range_array[i]->delivered_events.first,
		   xd->record_setup->range_array[i]->delivered_events.last      );
	  
	}
      if ( ( (int)xd->record_setup->range_array[i]->device_events.first != 0)
	   &&
	   ( (int)xd->record_setup->range_array[i]->device_events.last != 0))
	{
	  fprintf (fp, "%s            %u-%u \n", 
		   xnee_xns_key2string(XNEE_DEVICE_EVENT_STR_KEY),
		   xd->record_setup->range_array[i]->device_events.first,
		   xd->record_setup->range_array[i]->device_events.last        );
	}
      if ( ( (int) xd->record_setup->range_array[i]->errors.first != 0)
	   &&
	   ( (int) xd->record_setup->range_array[i]->errors.last != 0))
	{
	  fprintf (fp,  "%s                   %u-%u \n", 
		   xnee_xns_key2string(XNEE_ERROR_STR_KEY),
		   xd->record_setup->range_array[i]->errors.first,
		   xd->record_setup->range_array[i]->errors.last      );
	}
      
    }
  return XNEE_OK;
}

/*
 *
 *    xnee_print_xnee_resource_settings.  
 * 
 */
int
xnee_print_xnee_resource_settings (xnee_data* xd, FILE* out) 
{
  time_t rawtime;
  time_t ret_time;
  struct tm * timeinfo;





  if ( (xd==NULL) || (xd->data==NULL) )
    {
      return XNEE_NO_MAIN_DATA;
    }
  if (xd->record_setup==NULL)
    {
      return XNEE_NO_RECORD_DATA;
    }

  


  ret_time = time ( &rawtime );
  if ( ret_time != rawtime)
    {
      return XNEE_DATE_FAILURE;
    }

  timeinfo = localtime ( &rawtime );
  if (timeinfo == NULL)
    {
      return XNEE_DATE_FAILURE;
    }
  
  if (out == NULL)
    {
      if ( xd->verbose == 0 ) 
	{
	  return XNEE_OK;
	}
      out=xd->err_file;
    }



  fprintf (out,  "###############################################\n");
  fprintf (out,  "#                                             #\n");
  fprintf (out,  "#            Xnee Project file                #\n");
  fprintf (out,  "#                                             #\n");
  fprintf (out,  "#                                             #\n");
  fprintf (out,  "#  This file is automagically created by      #\n");
  fprintf (out,  "#  libxnee.... edit if you know what you do   #\n");
  fprintf (out,  "#                                             #\n");
  fprintf (out,  "###############################################\n");
  fprintf (out,  "\n");
  fprintf (out,  "\n");
  fprintf (out,  "\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Project info                          #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  XNEE_RES_PROJECT":  \t\t\t%s\n",
	   xnee_get_project_name(xd));
  fprintf (out,  XNEE_RES_DESCRIPTION":\t\t%s\n",
	   xnee_get_project_descr(xd));

  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#     Creation info                          #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  XNEE_RES_CREAT_DATE":    \t%s\n",
	   xnee_get_creat_date(xd));
  fprintf (out,  XNEE_RES_CREAT_PROGRAM": \t%s\n",
	   xnee_get_creat_program(xd));
  fprintf (out,  XNEE_RES_CREAT_PROG_VER":  \t%s\n",
	   xnee_get_creat_prog_vers(xd));


  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#    Last change info                        #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  XNEE_RES_LASTCHANGE_DATE":  \t%s\n",
	   xnee_get_last_date(xd));
  fprintf (out,  XNEE_RES_LASTCHANGE_PROGRAM":\t%s\n",
	   xnee_get_last_program(xd));
  fprintf (out,  XNEE_RES_LASTCHANGE_PROG_VER":\t%s\n",
	   xnee_get_last_prog_vers(xd));


  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Current info                          #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  XNEE_RES_CURRENT_CHANGE_DATE":  \t%.4d-%.2d-%.2d \n",
	   timeinfo->tm_year + 1900 , 
	   timeinfo->tm_mon + 1 , 
	   timeinfo->tm_mday  );
  fprintf (out,  XNEE_RES_CURRENT_CHANGE_PROGRAM":     \t%s\n",
	   PACKAGE);
  fprintf (out,  XNEE_RES_CURRENT_CHANGE_PROG_VER":  \t%s\n",
	   VERSION);
  return XNEE_OK;
}


/*
 *
 *    xnee_print_xnee_settings.  
 * 
 */
int
xnee_print_xnee_settings (xnee_data* xd, /*@null@*/ FILE* out) 
{
  int ret;
  int i ;
   
  if (out == NULL)
    {
      if ( xd->verbose == 0 ) 
	{
	  return XNEE_OK;
	}
      out=xd->err_file;
    }
  
  if ( (xd==NULL) || 
       (xd->grab_keys==NULL) ||
       (xd->grab_keys->action_keys==NULL) )
    {
      return XNEE_NO_MAIN_DATA;
    }
  fprintf (out,  "\n");

  if ( xd->app_args!= NULL)
    {
      /* Xnee application argument */
      fprintf (out,  "\n\n");
      fprintf (out,  "##############################################\n");
      fprintf (out,  "#      Xnee application arguments            #\n");
      fprintf (out,  "##############################################\n");
      fprintf (out,  "#  ");
      for (i=0;xd->app_args[i]!=NULL;i++)
	{
	  if ( xd->app_args[i] != NULL)
	    {
	      fprintf (out,  "%s ", xd->app_args[i]);
	    }
	}
      fprintf (out,  "\n");
    }
  
  /* DISPLAY */
  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Displays                              #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "# display %s\n",
	   (xd->display==NULL) ? "NULL"  : xd->display);
  fprintf (out,  "# %s\n" , xnee_xns_key2string(XNEE_DISTRIBUTE_KEY));
  ret = xnee_print_distr_list(xd, out);
  XNEE_RETURN_IF_ERR(ret);


  /* FILES */
  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Files                                 #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,   "# %s %s\n",
	   xnee_xns_key2string(XNEE_OUT_FILE_KEY),
	   (xd->out_name==NULL) ? "stdout" : xd->out_name );
  fprintf (out,   "# %s %s\n",
	   xnee_xns_key2string(XNEE_ERR_FILE_KEY),
	   (xd->err_name==NULL) ? "stderr" : xd->err_name );
  
  /* GRAB */
  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Key Grabs                             #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "# %s         %s\n",
	   xnee_xns_key2string(XNEE_STOP_KEY_KEY),  
	   (xd->grab_keys->action_keys[XNEE_GRAB_STOP].str) ? xd->grab_keys->action_keys[XNEE_GRAB_STOP].str: "0" );
  fprintf (out,   "# %s        %s\n",
	   xnee_xns_key2string(XNEE_PAUSE_KEY_KEY),
	   (xd->grab_keys->action_keys[XNEE_GRAB_PAUSE].str) ? xd->grab_keys->action_keys[XNEE_GRAB_PAUSE].str: "0" );
  fprintf (out,   "# %s       %s\n",
	   xnee_xns_key2string(XNEE_RESUME_KEY_KEY),
	   (xd->grab_keys->action_keys[XNEE_GRAB_RESUME].str) ? xd->grab_keys->action_keys[XNEE_GRAB_RESUME].str: "0" );
  fprintf (out,   "# %s       %s\n",
	   xnee_xns_key2string(XNEE_INSERT_KEY_KEY),
	   (xd->grab_keys->action_keys[XNEE_GRAB_INSERT].str) ? xd->grab_keys->action_keys[XNEE_GRAB_INSERT].str: "0" );
  fprintf (out,   "# %s         %s\n",
	   xnee_xns_key2string(XNEE_EXEC_KEY_KEY),
	   (xd->grab_keys->action_keys[XNEE_GRAB_EXEC].str) ? xd->grab_keys->action_keys[XNEE_GRAB_EXEC].str: "0" );
  fprintf (out,   "# %s         %s\n",
	   xnee_xns_key2string(XNEE_EXEC_PROGRAM_KEY),
	   (xd->grab_keys->action_keys[XNEE_GRAB_EXEC].extra_str) 
	   ? xd->grab_keys->action_keys[XNEE_GRAB_EXEC].extra_str: 
	   XNEE_EXEC_NO_PROG );
  
  /* Limits */
  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Recording limits etc                  #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "\n");
  fprintf (out,  "%s        %d\n",
	   xnee_xns_key2string(XNEE_EVENT_MAX_KEY),xnee_get_events_max(xd) );
  fprintf (out,  "%s          %d\n",
	   xnee_xns_key2string(XNEE_DATA_MAX_KEY),xnee_get_data_max(xd) );
  fprintf (out,  "%s       %d\n",
	   xnee_xns_key2string(XNEE_TIME_MAX_KEY),xnee_get_time_left(xd) );
  if ( xd->xnee_info.first_last == 0 )
    {
      fprintf (out,  "# ");
    }
  fprintf (out,  "first-last\n"); 

  fprintf (out,  "\n# Record  all (including current) clients or only future ones\n");
  if (xnee_get_all_clients(xd)==0)
    {
      fprintf (out,  "# ");
    }
  fprintf (out,  "%s\n",
           xnee_xns_key2string(XNEE_ALL_CLIENTS_KEY));
  if (xnee_get_all_clients(xd)!=0)
    {
      fprintf (out,  "# ");
    }
  fprintf (out,  "%s\n",
           xnee_xns_key2string(XNEE_FUTURE_CLIENTS_KEY));

  fprintf (out,  "\n# Store the starting mouse position \n");
  if (xnee_is_store_mouse_pos(xd)==0)
    {
      fprintf (out,  "# ");
    }
  fprintf (out,  "%s\n",
	   xnee_xns_key2string(XNEE_STORE_MOUSE_POS_KEY));
  
  
  /* Resolution */
  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Resolution                            #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "\n# Resolution\n");
  fprintf (out,  "#" "%s  %dx%d\n",
	   xnee_xns_key2string(XNEE_RECORDED_RESOLUTION_KEY),
	   xnee_get_rec_resolution_x(xd),
	   xnee_get_rec_resolution_y(xd)  ); 
  fprintf (out,  "#" "%s  %dx%d\n",
	   xnee_xns_key2string(XNEE_REPLAY_RESOLUTION_KEY),
	   xnee_get_rep_resolution_x(xd),
	   xnee_get_rep_resolution_y(xd)  ); 
  fprintf (out,  "#" "%s  %d\n",
	   xnee_xns_key2string(XNEE_ADJUST_RESOLUTION_KEY),
	   xnee_get_resolution_used (xd));


  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Speed                                 #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "\n# Speed\n");
  fprintf (out,  "#" "%s  %d\n",
	   xnee_xns_key2string(XNEE_SPEED_PERCENT_KEY),
	   xnee_get_replay_speed(xd)); 
  

  /* Limits */
  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Replaying limits etc                  #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "\n%s %d \n", 
           xnee_xns_key2string(XNEE_MAX_THRESHOLD_KEY),
           xnee_get_max_threshold(xd));
  fprintf (out,  "%s %d \n", 
           xnee_xns_key2string(XNEE_MIN_THRESHOLD_KEY),
           xnee_get_min_threshold(xd));
  fprintf (out,  "%s %d \n", 
           xnee_xns_key2string(XNEE_TOT_THRESHOLD_KEY),
           xnee_get_tot_threshold(xd));
  

  /* Feedback */
  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Feedback                              #\n");
  fprintf (out,  "##############################################\n");
  if (!xnee_fb_no_requested(xd) )
    {
      fprintf (out, "#");
    }
  fprintf (out, "%s\n", xnee_xns_key2string(XNEE_FEEDBACK_NONE_KEY));

  if (!xnee_fb_stderr_requested(xd))
    {
      fprintf (out, "#");
    }
  fprintf (out, "%s\n", xnee_xns_key2string(XNEE_FEEDBACK_STDERR_KEY));

  if (!xnee_fb_xosd_requested(xd))
    {
      fprintf (out, "#");
    }
  fprintf (out, "%s\n", xnee_xns_key2string(XNEE_FEEDBACK_XOSD_KEY));

  fprintf (out, "%s %s\n", 
	   xnee_xns_key2string(XNEE_XOSD_FONT_KEY),
	   EMPTY_IF_NULL((char*)xnee_get_xosd_font(xd)));


  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Various                               #\n");
  fprintf (out,  "##############################################\n");
  /* Plugin */
  fprintf (out,  "\n# Plugin file \n");
  if ( (xd->plugin_name                      != NULL) &&  
       (xnee_xns_key2string(XNEE_PLUGIN_KEY) != NULL)   )
    {
      fprintf (out,  "# ");
      fprintf (out,  "%s       %s\n",
	       xnee_xns_key2string(XNEE_PLUGIN_KEY),
	       xd->plugin_name ); 
    }      

  
  /* Modes */
  fprintf (out,  "\n# Modes (currently not used)\n");
  fprintf (out,  "# ");
  fprintf (out,    "%s\n",
	   xnee_xns_key2string(XNEE_SYNC_MODE_KEY)); 

  /* Replay offset */
  fprintf (out,  "\n# Replay offset\n");
  fprintf (out,    "#%s %dx%d\n",
	   xnee_xns_key2string(XNEE_REPLAY_OFFSET_KEY),
	   xnee_get_replay_offset_x(xd),
	   xnee_get_replay_offset_y(xd)); 

  fprintf (out,  "\n# Human printout of X11 data (instead of Xnee format)\n");
  if (!xnee_is_human_printout(xd))
    {
      fprintf (out,  "# ");
    }      
  fprintf (out,  "%s  \n",
	   xnee_xns_key2string(XNEE_HUMAN_PRINTOUT_KEY)); 

  
  fprintf (out,  "\n# Delay before starting record/replay\n");
  fprintf (out,  "# %s %d\n", 
	   xnee_xns_key2string(XNEE_DELAY_TIME_KEY),
	   xnee_get_interval(xd) ); 

  fprintf (out,  "\n# No recording of ReparentNotify\n");
  if ( ! xnee_is_no_reparent_recording(xd) )
    {
      fprintf (out,  "# "); 
    }
  fprintf (out,  "%s \n", 
	   xnee_xns_key2string(XNEE_NO_REPARENT_RECORD)    ); 

  /* Various */
  fprintf (out,  "\n# Various\n");
  return XNEE_OK;
}


/*
 *   xnee_print_data_range_count
 *
 *
 * 
 */
int 
xnee_print_data_range_count (xnee_data * xd)
{
  int i=0;
  xnee_verbose((xd, "Data ranges\n-----------\n"));

  for ( i=0 ; i< XNEE_NR_OF_TYPES ; i++) 
    {
      xnee_verbose((xd, "PROT_DATA %d=%d\n", i,xd->xnee_info.data_ranges[i]));
    }
  xnee_verbose((xd, "MAX = %d\n", xnee_get_max_range(xd)));
  return (0);
}



/**************************************************************
 *                                                            *
 * xnee_replay_printbuffer                                    *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_replay_printbuffer_impl (xnee_data *xd )
{
  int i;
  fprint_fptr  fp   = xd->buffer_verbose_fp ;
  FILE        *file = xd->buffer_file       ;
  /*  int **       array= (int **)xd->data_buffer;*/

  if (xd->buf_verbose!=0) 
    {
      (void)fp (file,"\n --- replay buffer ---\n");
      /*
	fp (file,"\t- means from Xserver\n\t+ means from file  \n");
	fp (file,"#     event  request  reply    error \n");
	fp (file,"-------------------------------------\n");
      */
      for (i=0;i<XNEE_REPLAY_BUFFER_SIZE;i++)
	{
	  if (xd->data_buffer[XNEE_EVENT][i] ||
 	      xd->data_buffer[XNEE_REQUEST][i] ||
 	      xd->data_buffer[XNEE_REPLY][i] ||
 	      xd->data_buffer[XNEE_ERROR][i] )
	    fp (file, "%.3d    %.3d     %.3d      %.3d    %.3d\n", i, 
		xd->data_buffer[XNEE_EVENT][i],
		xd->data_buffer[XNEE_REQUEST][i],
		xd->data_buffer[XNEE_REPLY][i],
		xd->data_buffer[XNEE_ERROR][i]
		);
	}
      fp (file,"cached: max=%02d  min=%02d total=%02d  s_max=%02d s_min=%02d\n", 
	  xd->meta_data.cached_max,
	  xd->meta_data.cached_min,
	  xd->meta_data.total_diff,
	  xd->meta_data.sum_max,
	  xd->meta_data.sum_min
	  );
      fp (file,"\n");
    }
}



/*
 *
 *   
 *
 */
void 
xnee_store_mouse_pos (xnee_data* xd )
{
  int rx;
  int ry;
  int wx;
  int wy;
  Window root;
  Window child;
  unsigned int mask;

  XQueryPointer(xd->data,
		RootWindow(xd->data,0),
		&root,
		&child,
		&rx,
		&ry,
		&wx,
		&wy,
		&mask);
  
  xd->data_fp (xd->out_file, 
	       "%s " 
	       "%s=%d " 
	       "%s=%d \n" ,
	       XNEE_FAKE_MOTION,
	       XNEE_FAKE_X_ARG, rx, 
	       XNEE_FAKE_Y_ARG, ry );
}



/**************************************************************
 *                                                            *
 * xnee_version                                               *
 *                                                            *
 *                                                            *
 **************************************************************/
void
xnee_version(xnee_data* xd)
{
  xnee_verbose ((xd, " --> xnee_version\n"));
  fprintf (stderr, "%s %s\n",PACKAGE, VERSION);
  fprintf (stderr, "Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010 Henrik Sandklef \n");
  fprintf (stderr, "%s and all its included programs come with ", PACKAGE);
  fprintf (stderr, "NO WARRANTY,\nto the extent permitted by law.\n" );
  fprintf (stderr, "This is free software, and you may redistribute\n");
  fprintf (stderr, "copies of it under terms of GNU General Public License.\n");
  /*
  fprintf (stderr, "For questions: \n\t%s  \n", XNEE_MAIL);
  fprintf (stderr, "Xnee, Xnee's not an Event Emulator \n\t%s \n", XNEE_HOME_URL);
  fprintf (stderr, "GPL, GNU General Public License \n\t%s\n",  GNU_HOME_URL);
  */
  xnee_verbose ((xd, " <-- xnee_version\n"));
}



int
xnee_print_xnee_data(xnee_data *xd)
{

  fprintf (stderr, "xnee_data\n");
  fprintf (stderr, "  xd:          %p\n", (void*)xd);
  fprintf (stderr, "  xd->data:    %p\n", (void*)xd->data);
  fprintf (stderr, "  xd->control: %p\n", (void*)xd->control);
  fprintf (stderr, "  xd->fake:    %p\n", (void*)xd->fake);
  fprintf (stderr, "  xd->grab:    %p\n", (void*)xd->grab);
  return XNEE_OK;
}



int
xnee_print_data_str(xnee_data *xd, char *tmp, int type)
{
  char *data;
  int ret = XNEE_OK;
  int data_nr;

  XNEE_VERBOSE_ENTER_FUNCTION();

  if ( (tmp[0] >= '0') && (tmp[0] <= '9') )
    {
      /* We have data as a number */
      if (sscanf(tmp, "%d", &data_nr) != 1 )
	{
	  ret = XNEE_WRONG_PARAMS;
	}
      else
	{
	  data = (char*) xnee_int2data (type, data_nr);
	  if ( data != NULL)
	    {
	      fprintf (stdout, "%s\n", data);
	    }
	  else
	    {
	      ret = XNEE_WRONG_PARAMS;
	    }
	}
    }
  else
    {
      /* We have data as a string */
      data = tmp;
      data_nr = xnee_data2int(type, tmp);
      if ( data_nr == -1)
	{
	  ret = XNEE_WRONG_PARAMS;
	}
      else
	{
	  fprintf (stdout, "%d\n", data_nr);
	}
    }
  XNEE_VERBOSE_LEAVE_FUNCTION();

  return ret;
}
