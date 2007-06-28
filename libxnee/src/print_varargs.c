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

#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/print_varargs.h"

#ifdef HAVE_STDARG_H
#include <stdarg.h>
#else
#include <varargs.h> 
#endif



/**************************************************************
 *                                                            *
 * xnee_verbose                                               *
 *                                                            *
 *                                                            *
 **************************************************************/
#ifdef HAVE_STDARG_H
void
xnee_verbosef (xnee_data *xd, /*@null@*/ char * msg,  ...)
{
  
  va_list ap;

  if (xd==NULL)
  {
     return;
  }

  if ( (xd->verbose!=0) && (xd->err_file!=NULL) )
    {
      va_start(ap, msg);
      (void)xd->verbose_fp ( xd->err_file, msg, ap );
      fflush(xd->err_file);
    }
}

#else
void
xnee_verbosef (xnee_data *xd, valist)
  va_dcl
{
  char *fmt;
  va_list ap;
  
  if (xd==NULL)
  {
     return;
  }

  if ( (xd->verbose!=0) && (xd->err_file!=NULL) )
    {
      va_start(argp);
      fmt = va_arg(argp, char *);
      xd->verbose_fp ( xd->err_file, msg, ap );
    }
}
#endif




/*
 * Print an error message
 */
#ifdef HAVE_STDARG_H
void
xnee_print_error (char * error, ...)
{
  va_list ap;
  va_start(ap, error);
  (void)vprintf ( error, ap );
  va_end(ap);
}
#else
void
xnee_print_error (char * error, ...)
{
  char *fmt;
  va_list ap;
  va_start(ap);
  fmt = va_arg(argp, char *);
  vprintf ( error, ap );
  va_end(ap);
}
#endif


