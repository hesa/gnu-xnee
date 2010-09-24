/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003 
 *                      2010 Henrik Sandklef                    
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



#ifndef XNEE_FAKE_H
#define XNEE_FAKE_H


#include "xnee.h"
#include "xnee_record.h"

int
xnee_reset_fake( xnee_data *xd);


#define xnee_fake_motion_event(xd, scr, x, y, dtime) xnee_fake_motion_event_impl(xd, scr, x, y, dtime, 0)

#define xnee_fake_xi_motion_event(xd, scr, x, y, dtime, dev) xnee_fake_motion_event_impl(xd, scr, x, y, dtime, dev)

#define xnee_fake_button_event(xd, button, bo , dtime)              xnee_fake_button_event_impl (xd, button, bo , dtime, 0) 

#define xnee_fake_xi_button_event(xd, button, bo , dtime, deviceid) xnee_fake_button_event_impl (xd, button, bo , dtime, deviceid) 


/**
 * Sends a faked moton event to the display in the distribution list as found in xd.
 * @param xd       xnee's main structure
 * @param screen   the screen to send it to
 * @param x        x coordinate
 * @param y        y coordinate
 * @param dtime    how long before the faking shall take place
 * @param deviceid Id of the device (XInput extension)
 * @return int     XNEE_OK on success.
 */
int 
xnee_fake_motion_event_impl (xnee_data* xd,
			     int screen, 
			     int x, 
			     int y, 
			     unsigned long dtime,
			     int deviceid);



/**
 * Sends a faked button event to the display in the distribution list as found in xd.
 * @param xd       xnee's main structure
 * @param button   the button to fake
 * @param bo       press or release
 * @param dtime    how long before the faking shall take place
 * @param deviceid Id of the device (XInput extension)
 * @return int     XNEE_OK on success.
 */
int
xnee_fake_button_event_impl (xnee_data* xd, 
			     int button, 
			     Bool bo , 
			     int dtime,
			     int deviceid);



/**
 * Sends a faked key event to the display in the distribution list as found in xd.
 * @param xd       xnee's main structure
 * @param keycode  keycode of the event to be faked
 * @param bo       is it a press or a release
 * @param dtime    how long before the faking shall take place
 * @return int     XNEE_OK on success.
 */
int 
xnee_fake_key_event  (xnee_data* xd, int keycode, Bool bo, int dtime);


/**
 * Replays the event (if possible) and calls xnee_fake_** 
 * functions for distribution. <br>
 * If it wasn't poosible to replay the event XNEE_NOT_REPLAYABLE is returned.
 * 
 * @param xd   xnee's main structure
 * @param xid  the data to replay
 * @param last_elapsed 
 * @return 0   on success, XNEE_NOT_REPLAYABLE if failure
 *  
 */
int 
xnee_replay_event_handler (xnee_data *xd,  xnee_intercept_data* xid , long int last_elapsed);  



int
xnee_fake_relative_motion_event (xnee_data* xd,
				 int x, 
				 int y, 
				 unsigned long dtime);


int 
xnee_type_file(xnee_data *xd );

int
xnee_fake_key_mod_event (xnee_data* xd, xnee_script_s *xss, Bool bo, int dtime);


#endif /* XNEE_FAKE_H */
