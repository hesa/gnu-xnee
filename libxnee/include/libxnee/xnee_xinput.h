/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 2010 Henrik Sandklef                    
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




#ifndef XNEE_XINPUT_H
#define XNEE_XINPUT_H

#include "libxnee/xnee.h"
#ifdef   XNEE_XINPUT_SUPPORT
#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h> 
#include <X11/extensions/XIproto.h>
#endif /*  XNEE_XINPUT_SUPPORT */


#define xnee_disable_xinput(xd)  \
  xd->xi_data.xinput_event_base      = 0 ; \
  xd->xi_data.recording_enabled      = 0 ; \
  xd->xi_data.xinput_record_mouse    = 0;                   \
  xd->xi_data.xinput_record_keyboard = 0;

#define xnee_xinput_disabled(xd)\
  (xd->xi_data.recording_enabled != 1)

#define xnee_xinput_request_mouse(xd)     \
  xd->xi_data.xinput_record_mouse=1;
#define xnee_xinput_request_keyboard(xd)  \
  xd->xi_data.xinput_record_keyboard=1;

#define xnee_xinput_mouse_requested(xd)   \
  (xd->xi_data.xinput_record_mouse==1)
#define xnee_xinput_keyboard_requested(xd)\
  (xd->xi_data.xinput_record_keyboard==1)


int
xnee_init_xinput(xnee_data *xd);

int
xnee_init_xinput_devices(xnee_data *xd);

int
xnee_get_xinput_event_base(Display *dpy);

int
xnee_has_xinput(xnee_data *xd);

int 
xnee_handle_xinput_event(xnee_data * xd, 
			 int event_type, 
			 XRecordDatum *xrec_data,
			 Time server_time);


int 
xnee_handle_xinput_event_human(xnee_data * xd, 
			       int event_type, 
			       XRecordDatum *xrec_data,
			       Time server_time);



XDevice * 
xnee_get_xinput_device(xnee_data *xd, int deviceid);

#endif /* XNEE_XINPUT_H */
