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


#ifndef XNEE_XNEE_KEYSYM_H
#define XNEE_XNEE_KEYSYM_H

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>

KeyCode
xnee_str2keycode(xnee_data* xd, const char *str, xnee_key_code *kc);

KeyCode
xnee_char2keycode (xnee_data *xd, char token, xnee_key_code *kc);

#endif /* XNEE_XNEE_KEYSYM_H */
