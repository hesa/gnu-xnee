/*****  
 *       Xnee's Not an Event Emulator  
 *                                                                    
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *  Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004 Henrik Sandklef                    
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

#include <stdio.h>

#include "libxnee/xnee_dl.h"
#include "libxnee/feedback.h"

#ifdef HAVE_STDARG_H
#include <stdarg.h>
#else
#include <varargs.h> 
#endif

enum feedback_types 
{
  XNEE_NO_FEEDBACK      = 0 ,
  XNEE_STDERR_FEEDBACK      ,
  XNEE_XOSD_FEEDBACK        ,
  XNEE_UNDEFINED_FEEDBACK   
} ;





/*********************************************
 *
 *
 *  XOSD - X11 on screen display support 
 *
 *
 *
 *********************************************/
#ifndef  XOSD_string
#define  XOSD_string 1
#endif /* XOSD_string*/



/* xosd fun/lib pointers */
static void   *xosd_lib = NULL ; 
static void   *osd;
static int     feedback_used;
static void* (*xosd_print)(void*, int, int, char *) = NULL ; 


static int
xnee_setup_xosd(xnee_data *xd)
{
  void* (*xosd_create)(int) = NULL ; 
  void* (*xosd_int)(void*, int) = NULL ; 
  void* (*xosd_char)(void*, char*) = NULL ; 
  int ret;


  xosd_lib = xnee_dlopen (xd, "libxosd.so", RTLD_LAZY);
  if (xosd_lib==NULL) 
    { 
      feedback_used=0; 
      return XNEE_XOSD_FAILURE ; 
    }
  
  xosd_create = xnee_dlsym(xd, xosd_lib, "xosd_create");
  if (xosd_create==NULL) { feedback_used=0; return XNEE_XOSD_FAILURE ; }
  osd = xosd_create(2);
  if (osd==NULL) 
    { 
      feedback_used=0; 
      xnee_dlclose(xd, xosd_lib);
      return XNEE_XOSD_FAILURE ; 
    }
  

  xosd_int = xnee_dlsym(xd, xosd_lib, "xosd_set_shadow_offset");
  if (xosd_int==NULL) 
    { 
      feedback_used=0; 
      free(osd);
      xnee_dlclose(xd, xosd_lib);
      return XNEE_XOSD_FAILURE ; 
    }
  xosd_int(osd, 2);

  
  xosd_int = xnee_dlsym(xd, xosd_lib, "xosd_set_timeout");
  if (xosd_int==NULL) 
    { 
      feedback_used=0; 
      free(osd);
      xnee_dlclose(xd, xosd_lib);
      return XNEE_XOSD_FAILURE ; 
    }
  xosd_int(osd, FEEDBACK_TIMEOUT);

  
  xosd_char = xnee_dlsym(xd, xosd_lib, "xosd_set_font");
  if (xosd_char==NULL) 
    { 
      feedback_used=0; 
      free(osd);
      xnee_dlclose(xd, xosd_lib);
      return XNEE_XOSD_FAILURE ; 
    }
  xosd_char(osd, "-adobe-helvetica-bold-r-normal-*-*-320-*-*-p-*-iso8859-1");

  xosd_char = xnee_dlsym(xd, xosd_lib, "xosd_set_colour");
  if (xosd_char==NULL) 
    { 
      feedback_used=0; 
      free(osd);
      xnee_dlclose(xd, xosd_lib);
      return XNEE_XOSD_FAILURE ; 
    }
  xosd_char(osd, "yellow");


  xosd_print= xnee_dlsym(xd, xosd_lib, "xosd_display");
  if (xosd_print==NULL) 
    { 
      feedback_used=0; 
      free(osd);
      xnee_dlclose(xd, xosd_lib);
      return XNEE_XOSD_FAILURE ; 
    }
  
  return XNEE_OK;
}

static int
xnee_setup_feedback(xnee_data *xd)
{
  if (xnee_setup_xosd(xd)==XNEE_OK)
    feedback_used = XNEE_XOSD_FEEDBACK;
  else
    feedback_used = XNEE_STDERR_FEEDBACK;

  return feedback_used;
}

int static 
feedback_init(xnee_data *xd, char *str)
{
  static int need_init = 1 ;

  if (need_init == 1)
    {
      xnee_setup_feedback(xd);
      need_init = 0 ;
    }
  return XNEE_OK;
}


#ifdef HAVE_STDARG_H
int 
feedback(xnee_data *xd, char *str, ... )
{
  
  va_list ap;
  static char buf[200];
  int conv;
  int ret;
#else
int 
feedback(xnee_data *xd, valist)
  va_dcl
{
  char *str;
  va_list ap;
  int conv;
  int ret;
  va_start(argp);
  str = va_arg(argp, char *);
#endif

  conv = vsnprintf ((char*) buf, 
		    200, 
		    str, 
		    ap );
  if (conv>=200)
    conv=199;
  buf[conv]='\0';
  
  feedback_init(xd, NULL); 
  if (feedback_used==XNEE_XOSD_FEEDBACK)
    {
      
      xosd_print(osd, 0, XOSD_string, buf); 
      ret = XNEE_OK;
    }
  else if (feedback_used==XNEE_STDERR_FEEDBACK)
    {
      fprintf (stderr,buf);
      ret = XNEE_OK;
    }
  else
    {
      ret = XNEE_FEEDBACK_FAILURE;
    }

  va_start(ap, str);
  va_end(ap);
  return ret;
}


int 
xnee_set_stderr_feedback(xnee_data *xd)
{
  feedback_used=XNEE_STDERR_FEEDBACK;
  
  if (xosd_lib!=NULL)
    xnee_dlclose(xd, xosd_lib);

  if (osd!=NULL)
    free(osd);
  
  return XNEE_OK;
}

int 
xnee_set_xosd_feedback(xnee_data *xd)
{
  int ret ;

  ret = xnee_setup_xosd(xd);
  if (ret == XNEE_OK)
    {
      feedback_used = XNEE_XOSD_FEEDBACK;
    }
  else
    {
      feedback_used = XNEE_STDERR_FEEDBACK;
    }
  return ret;
}


int 
xnee_is_xosd_feedback(xnee_data *xd)
{
  return (feedback_used==XNEE_XOSD_FEEDBACK);
}

int 
xnee_is_stderr_feedback(xnee_data *xd)
{
  return (feedback_used==XNEE_STDERR_FEEDBACK);
}



