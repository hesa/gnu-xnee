/*****  
 *       Xnee's Not an Event Emulator  
 *                                                                    
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003 Henrik Sandklef   
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


#ifndef XNEE_GRAB_H
#define XNEE_GRAB_H

int 
xnee_ungrab_key (xnee_data* xd, int mode);

int 
xnee_ungrab_keys (xnee_data* xd);

xnee_grab_keys*
xnee_new_grab_keys(xnee_data *xd);

/*@null@*/ 
int
xnee_free_grab_keys(/*@null@*/ xnee_grab_keys *grab_keys);

int
xnee_grab_keys_init(xnee_data *xd);

int 
xnee_grab_all_keys (xnee_data* xd);


enum 
  {
    XNEE_GRAB_NONE_PRESSED=0,
    XNEE_GRAB_NONE_IN_USE,
    XNEE_GRAB_SOME_IN_USE,
    XNEE_GRAB_ALL_IN_USE,
    XNEE_GRAB_DO_SAVE,
    XNEE_GRAB_DO_PRINT,
    XNEE_GRAB_DONT_PRINT,
    XNEE_OVERRIDE_FALSE,
    XNEE_OVERRIDE_TRUE,
    XNEE_OVERRIDE_GET,
    XNEE_GRAB_BUFFER_SAVE,
    XNEE_GRAB_BUFFER_PRINT,
    XNEE_GRAB_BUFFER_RESET,
    XNEE_GRAB_KM_PRESS,
    XNEE_GRAB_KM_RELEASE,
    XNEE_GRAB_MOUSE
  } xnee_grab_modifiers_control ;
  
int
xnee_save_or_print(xnee_data *xd, KeyCode kc, int mode);

int 
xnee_grab_handle_buffer(xnee_data *xd, char *str, int mode);

int
xnee_get_grab_mode ( xnee_data *xd, int key);
 

void
xnee_print_grabbed_keys(xnee_data *xd);

#endif /* XNEE_GRAB_H */


