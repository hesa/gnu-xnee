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
 * Foundation, Inc., 51 Franklin Street, Boston,            
 * MA  02110-1301, USA.                                              
 ****/


#ifndef XNEE_XNEE_DISPLAY_H
#define XNEE_XNEE_DISPLAY_H

#include "libxnee/xnee.h"



int 
xnee_setup_display (xnee_data *xd);


/**
 *  Set xnee_data->display to the value as 
 *  specified in environment variable DISPLAY
 * 
 * @param xd    xnee's main structure
 * @return void  
 */
void 
xnee_set_default_display (xnee_data *xd);





/**
 * open a display for the host as specified in 
 * xnee_program_data->program
 * @param xd           xnee's main structure
 * @return Display *   NULL if the display could not be opened
 */
Display * 
xnee_open_display (xnee_data *xd);




/**
 * Adds a display to ditribution list
 * @param xd    xnee's main structure
 * @param disp  Display to add
 * @return int  0 on success
 */
int 
xnee_add_display_list (xnee_data* xd, char * disp);





/**
 * Adds a display to ditribution list
 * @param str  string representation of a display
 * @param xd   xnee's main structure
 * @return int XNEE_OK on success, XNEE_NOT_OPEN_DISPLAY if failure
 */
int 
xnee_add_display_str (char *str , xnee_data* xd);




/**
 * Adds a display to ditribution list
 * @param dpy    Display to add
 * @param xd     xnee's main structure
 * @return int   XNEE_OK on success, XNEE_NOT_OPEN_DISPLAY if failure
 */
int 
xnee_add_display (Display * dpy, xnee_data* xd);



#endif /*   XNEE_XNEE_DISPLAY_H  */

