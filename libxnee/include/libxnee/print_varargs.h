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

#ifndef  XNEE_PRINT_VARARGS_H
#define  XNEE_PRINT_VARARGS_H


#include "xnee.h"

/**
 * Verbose prints to specified file
 *
 * @param xd     xnee's main structure
 * @param str    formatted string 
 * @param ....   extra parameters to string
 * @return void  
 */
void
xnee_verbosef (xnee_data* xd, /*@null@*/ char * str,  ...)  __attribute__ ((format ( printf, 2 , 3))) ;

#ifdef HAVE_STDARG_H
void 
xnee_print_error (char * error, ...) __attribute__ (( format (printf, 1, 2)));
#else
void
xnee_print_error (char * error, ...)  __attribute__ (( format (printf, 1, 2)));
#endif /* HAVE_STDARG_H */



#ifdef NO_VERBOSE
#define xnee_verbose(a)
#else
#define xnee_verbose(a) xnee_verbosef a
#endif /* NO_VERBOSE */



#endif /*  XNEE_PRINT_VARARGS_H */
