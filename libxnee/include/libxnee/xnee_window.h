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


typedef struct 
{
  int   rel_x ;
  int   rel_y ;
  int   x ;
  int   y ;
  int   border_w;
  int   border_h;
  int   width ;
  int   height ;
  int   window ;
  int   event ;
  int   parent ;
  char* name ;
} xnee_win_pos;


enum
  {
    XNEE_WINDOW_RECEIVED,
    XNEE_WINDOW_SESSION
  } xnee_window_types;


int
xnee_window_add_attribue_impl(xnee_data    *xd, 
			      XWindowAttributes *attributes,
			      Window        win,
			      int where);
int
xnee_window_add_impl(xnee_data    *xd, 
		     xnee_win_pos *xwp,
		     int where);

int
xnee_window_add_attribute_impl(xnee_data    *xd, 
			       XWindowAttributes *attributes,
			       Window win,
			       Window parent,
			       int where);

#define xnee_window_add_session(xd, xwp) \
   xnee_window_add_impl(xd, xwp, XNEE_WINDOW_SESSION)

#define xnee_window_add_received(xd, xwp) \
   xnee_window_add_impl(xd, xwp, XNEE_WINDOW_RECEIVED)

#define xnee_window_add_attribute_received(xd, xwp, win, par) \
   xnee_window_add_attribute_impl(xd, xwp, win, par, XNEE_WINDOW_RECEIVED)

#define xnee_window_add_attribute_session(xd, xwp, win, par) \
   xnee_window_add_attribute_impl(xd, xwp, win, par, XNEE_WINDOW_SESSION)

int
xnee_window_try_move(xnee_data *xd);
