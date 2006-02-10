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

#include "libxnee/xnee.h"
#include "libxnee/xnee_window.h"
#include "libxnee/print.h"


#define XNEE_WINDOW_BUFFER_SIZE 10
static int received_index = 0;
static int session_index  = 0;

xnee_win_pos received_windows[XNEE_WINDOW_BUFFER_SIZE];
xnee_win_pos session_windows[XNEE_WINDOW_BUFFER_SIZE];

int
xnee_window_add_received(xnee_data *xd, 
			 xnee_win_pos *xwp)
{
  xnee_win_pos *ptr;

  printf ("xnee_window_add_received \n");
  xnee_verbose ((xd, "---> xnee_window_add_received\n"));
  if ( received_index >= XNEE_WINDOW_BUFFER_SIZE )
    {
      xnee_verbose ((xd, "<--- xnee_window_add_received   errror\n"));
      return XNEE_WINDOW_POS_ADJ_ERROR;
    }

  ptr = &received_windows[received_index];

  ptr->x      = xwp->x;
  ptr->y      = xwp->y;
  ptr->height = xwp->height;
  ptr->width  = xwp->width;
 

  received_index++;

  printf ( "<--- xnee_window_add_received  %dx%d\n", ptr->x, ptr->y);
  xnee_verbose ((xd, "<--- xnee_window_add_received  %dx%d\n", ptr->x, ptr->y));
  return XNEE_OK;
}


int
xnee_window_add_read(xnee_data    *xd, 
		     xnee_win_pos *xwp)
{
  xnee_win_pos *ptr;

  xnee_verbose ((xd, "---> xnee_window_add_read\n"));

  printf ("xnee_window_add_read: ");
  printf ("  %dx%d+%d+%d\n",
	  xwp->x,xwp->y,xwp->width,xwp->height);

  if ( session_index >= XNEE_WINDOW_BUFFER_SIZE )
    {
      xnee_verbose ((xd, "<--- xnee_window_add_read ... error\n"));
      return XNEE_WINDOW_POS_ADJ_ERROR;
    }

  
  ptr = &session_windows[session_index];

  ptr->x      = xwp->x;
  ptr->y      = xwp->y;
  ptr->height = xwp->height;
  ptr->width  = xwp->width;
  
  session_index++;

  xnee_verbose ((xd, "<--- xnee_window_add_read\n"));
  return XNEE_OK;
}


int
xnee_window_try_move(xnee_data *xd)
{
  ;
}


