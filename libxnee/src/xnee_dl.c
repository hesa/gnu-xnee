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


/**************************************************************
 *                                                            *
 * xnee_dlopen                                                *
 *                                                            *
 *                                                            *
 **************************************************************/
void *
xnee_dlopen (xnee_data *xd, char *dl, int flag)
{
  xnee_verbose ((xd, " ---> xnee_dlopen\n"));
  xnee_verbose ((xd, " <--- xnee_dlopen\n"));
  return dlopen ((const char*)dl, flag);
}


/**************************************************************
 *                                                            *
 * xnee_dlsym                                                 *
 *                                                            *
 *                                                            *
 **************************************************************/
void *
xnee_dlsym (xnee_data *xd, void* handle, char *sym)
{
  xnee_verbose ((xd, " ---> xnee_dlsym\n"));
  xnee_verbose ((xd, " <--- xnee_dlsym\n"));
  return dlsym (handle, sym);
}


/**************************************************************
 *                                                            *
 * xnee_dlclose                                               *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_dlclose (xnee_data *xd, void* handle)
{
  xnee_verbose ((xd, " ---> xnee_dlclose\n"));
  xnee_verbose ((xd, " <--- xnee_dlclose\n"));
  return dlclose (handle);
}


/**************************************************************
 *                                                            *
 * xnee_dlerror                                               *
 *                                                            *
 *                                                            *
 **************************************************************/
char* 
xnee_dlerror (xnee_data *xd)
{
  xnee_verbose ((xd, " ---> xnee_dlerror\n"));
  xnee_verbose ((xd, " <--- xnee_dlerror\n"));
  return dlerror();
}


