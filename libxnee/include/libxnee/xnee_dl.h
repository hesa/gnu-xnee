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

#include <dlfcn.h>
#include <libxnee/xnee.h>
#include <libxnee/print.h>

#ifndef XNEE_DL_H
#define XNEE_DL_H

void*
xnee_dlopen (xnee_data *xd, char *dl, int flag);

void*
xnee_dlsym (xnee_data *xd, void* handle, char *sym);

int 
xnee_dlclose (xnee_data *xd, void* handle);

const char* 
xnee_dlerror (xnee_data *xd);


#endif /* XNEE_DL_H */
