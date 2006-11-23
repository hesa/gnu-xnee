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


#ifndef XNEE_X11_FILES_H
#define XNEE_X11_FILES_H

/* Ugly hack to aid various OS */
#define NEED_REPLIES
#define NEED_EVENTS 
#include <X11/Xproto.h>

/* Ugly hack to aid AIX */
#include <X11/Xlibint.h>  /* Warning, there be dragons here.... */
#ifndef XLIBINT_H
#define XLIBINT_H
#endif /* XLIBINT_H */

#include <X11/Xlib.h>
#include <X11/Xmd.h>
#include <X11/Xos.h>

#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <X11/Xutil.h>

#include <X11/extensions/record.h> 
#include <X11/extensions/XTest.h>


#endif /* XNEE_X11_FILES_H */


