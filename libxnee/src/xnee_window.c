/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 2006 Henrik Sandklef                    
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
 * Foundation, Inc., 51 Franklin Street, Boston,            
 * MA  02110-1301, USA.                                              
 ****/

#include <X11/Xlib.h>

#include "libxnee/xnee.h"
#include "libxnee/xnee_window.h"
#include "libxnee/print.h"

/*
 *  In this file the following two words are frequent enough
 *  to let their usage be explained:
 *     session   - information read from session file
 *     received  - information as received during replay from RECORD
 */ 

#define XNEE_WINDOW_BUFFER_SIZE 10
static int received_index = 0;
static int session_index  = 0;

xnee_win_pos received_windows[XNEE_WINDOW_BUFFER_SIZE];
xnee_win_pos session_windows[XNEE_WINDOW_BUFFER_SIZE];

static int
xnee_window_print_lists(void)
{
  int i ; 

  fprintf (stderr,"Session\n");
  for (i=0; ( i<session_index ) && (i<XNEE_WINDOW_BUFFER_SIZE ) ;i++)
    {
      fprintf (stderr,"  %d %d %d\n",
	       session_windows[i].window,
	       session_windows[i].x,
	       session_windows[i].y);
    }
  fprintf (stderr,"Received\n");
  for (i=0;( i<received_index ) && (i<XNEE_WINDOW_BUFFER_SIZE ) ;i++)
    {
      fprintf (stderr,"  %d %d %d\n",
	      received_windows[i].window,
	      received_windows[i].x,
	      received_windows[i].y);
    }
}


int
xnee_window_add_attribute_impl(xnee_data    *xd, 
			       XWindowAttributes *attributes,
			       Window win,
			       int where)
{
  xnee_win_pos  xwp;

  xwp.x      = attributes->x;
  xwp.y      = attributes->y;
  xwp.width  = attributes->width;
  xwp.height = attributes->height;
  xwp.window = win;
  
  return xnee_window_add_impl(xd, &xwp, where); 
}


int
xnee_window_add_impl(xnee_data    *xd, 
		     xnee_win_pos *xwp,
		     int where)
{
  xnee_win_pos *ptr;
  int index ;

  xnee_verbose((xd, "---> xnee_window_add_impl\n"));

  switch ( where )
   {
   case XNEE_WINDOW_RECEIVED:
      index = received_index;
      ptr = &received_windows[received_index];
      break;
   case XNEE_WINDOW_SESSION:
      index = session_index;
      ptr = &session_windows[session_index];
      break;
   default:
     return XNEE_WINDOW_POS_ADJ_ERROR;
   }


  if ( index >= XNEE_WINDOW_BUFFER_SIZE )
    {
      xnee_verbose ((xd, "<--- xnee_window_add_impl ... error\n"));
      return XNEE_WINDOW_POS_ADJ_ERROR;
    }

  ptr->x      = xwp->x;
  ptr->y      = xwp->y;
  ptr->height = xwp->height;
  ptr->width  = xwp->width;
  ptr->window = xwp->window;
  
  if ( where == XNEE_WINDOW_RECEIVED )
    {
      received_index++;
    }
  else
    {
      session_index++;
    }

  xnee_verbose((xd, "<--- xnee_window_add_impl\n"));
  return XNEE_OK;
}


int
xnee_window_add_read(xnee_data    *xd, 
		     xnee_win_pos *xwp)
{
  xnee_win_pos *ptr;

  xnee_verbose((xd, "---> xnee_window_add_read\n"));

  if ( session_index >= XNEE_WINDOW_BUFFER_SIZE )
    {
      xnee_verbose((xd, "<--- xnee_window_add_read ... error\n"));
      return XNEE_WINDOW_POS_ADJ_ERROR;
    }

  
  ptr = &session_windows[session_index];

  ptr->x      = xwp->x;
  ptr->y      = xwp->y;
  ptr->height = xwp->height;
  ptr->width  = xwp->width;
  
  session_index++;

  xnee_verbose((xd, "<--- xnee_window_add_read\n"));
  return XNEE_OK;
}


int
xnee_window_try_move(xnee_data *xd)
{
  int ret = XNEE_OK;
  xnee_win_pos *sess_ptr;
  xnee_win_pos *rec_ptr;


  if (xnee_is_verbose(xd))
    {
      xnee_window_print_lists();
    }

  /*
   * If we haven't read (from file) and received (from RECORD)
   * then there we lack info on how to move the new window...
   * return....
   */
  if ( (session_index==0) || (received_index==0) )
    {
      return XNEE_OK;
    }

  /*
   * OK, we have read info on saved window and we have
   * a window to apply that info on, let's do it.
   */
  sess_ptr =  &session_windows[session_index-1];
  rec_ptr  =  &received_windows[received_index-1];

  if ( rec_ptr->window != 0 )
    {
      xnee_verbose((xd, " XMoveWindow(%d,%d,%d,%d)\n", 
		    xd->control,
		    rec_ptr->window,
		    sess_ptr->x,
		    sess_ptr->y));
      XMoveWindow(xd->control,
		  rec_ptr->window,
		  sess_ptr->x,
		  sess_ptr->y);

      session_index--;
      received_index--;
    }
  else
    {
      return XNEE_WINDOW_POS_ADJ_ERROR;
    }
  return ret;
}


