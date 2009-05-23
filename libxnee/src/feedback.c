/*****  
 *       Xnee's Not an Event Emulator  
 *                                                                    
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *  Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004 
 *                2009 Henrik Sandklef                    
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

#include <stdio.h>

#include "libxnee/xnee.h"
#include "libxnee/xnee_dl.h"
#include "libxnee/feedback.h"
#include "libxnee/xnee_setget.h"
#ifdef HAVE_STDARG_H
#include <stdarg.h>
#else
#include <varargs.h> 
#endif



int  xnee_xosd_close(xnee_data *xd);


/* xosd fun/lib pointers */
static void   *xosd_lib = NULL ; 
static void   *osd;
static int     feedback_used      = XNEE_UNDEFINED_FEEDBACK;
static int     feedback_requested = XNEE_XOSD_FEEDBACK;


 
typedef  void* (*xosd_create_fun) (int)           ; 
typedef  void* (*xosd_int_fun)    (void*, int)    ; 
typedef  void* (*xosd_char_fun)   (void*, char*)  ; 
typedef  void* (*xosd_print_fun) (void*, int, int, char *) ; 

typedef  xosd_create_fun *xosd_create_fun_ptr;
typedef  xosd_int_fun    *xosd_int_fun_ptr;
typedef  xosd_char_fun   *xosd_char_fun_ptr;
typedef  xosd_print_fun  *xosd_print_fun_ptr;

static xosd_print_fun xosd_print ;
static char *xosd_font ;
static char *xosd_default_font = "-*-terminus-medium-r-*-*-*-320-*-*-*-*-*-*";

static xosd_create_fun  xosd_create = NULL ; 
static xosd_int_fun     xosd_int    = NULL ; 
static xosd_char_fun    xosd_char   = NULL ; 


const char *
xnee_get_xosd_font_impl(xnee_data *xd)
{
  xnee_verbose((xd, "xnee_get_xosd_font_impl\n"));
  return xosd_font;
}

int
xnee_set_xosd_font_impl(xnee_data *xd, char *font_str)
{
  if ( (font_str==NULL) || strlen(font_str)<2)
    {
      return XNEE_XOSD_FAILURE;
    }
  xnee_verbose((xd, "xnee_set_xosd_font_impl\n"));
  XNEE_FREE_IF_NOT_NULL(xosd_font);
  xosd_font = strdup(font_str);
  if ( xosd_font == NULL)
    {
      return XNEE_XOSD_FAILURE;
    }

  return XNEE_OK;
}

static int
xnee_setup_xosd(xnee_data *xd)
{
  xosd_create_fun_ptr xosd_create_p ;
  xosd_int_fun_ptr    xosd_int_p    ;
  xosd_char_fun_ptr   xosd_char_p   ;
  xosd_print_fun_ptr  xosd_print_p  ;

  xosd_lib = xnee_dlopen (xd, "libxosd.so", RTLD_LAZY);
  if (xosd_lib==NULL) 
    { 
      feedback_used=XNEE_UNDEFINED_FEEDBACK; 
      return XNEE_XOSD_FAILURE ; 
    }
  
  
  xosd_create_p = &xosd_create;
  *(void**)(xosd_create_p) = xnee_dlsym(xd, xosd_lib, "xosd_create");
  xosd_create = (xosd_create_fun) *xosd_create_p;
  if (xosd_create==NULL) 
    { 
      feedback_used=XNEE_UNDEFINED_FEEDBACK; 
      return XNEE_XOSD_FAILURE ; 
    }
  osd = xosd_create(2);
  if (osd==NULL) 
    { 
      feedback_used=XNEE_UNDEFINED_FEEDBACK; 
      xnee_dlclose(xd, xosd_lib);
      return XNEE_XOSD_FAILURE ; 
    }
  
  xosd_int_p = &xosd_int;
  *(void**)(xosd_int_p) = xnee_dlsym(xd, xosd_lib, "xosd_set_shadow_offset");
  if (xosd_int==NULL) 
    { 
      feedback_used=XNEE_UNDEFINED_FEEDBACK; 
      free(osd);
      xnee_dlclose(xd, xosd_lib);
      return XNEE_XOSD_FAILURE ; 
    }
  xosd_int(osd, 2);

  
  *(void**)xosd_int_p =  xnee_dlsym(xd, xosd_lib, "xosd_set_timeout");
  xosd_int =  (xosd_int_fun) *xosd_int_p;
  if (xosd_int==NULL) 
    { 
      feedback_used=XNEE_UNDEFINED_FEEDBACK; 
      free(osd);
      xnee_dlclose(xd, xosd_lib);
      return XNEE_XOSD_FAILURE ; 
    }
  xosd_int(osd, FEEDBACK_TIMEOUT);

  xosd_char_p = &xosd_char;
  *(void**)xosd_char_p = xnee_dlsym(xd, xosd_lib, "xosd_set_font");
  if (xosd_char==NULL) 
    { 
      feedback_used=XNEE_UNDEFINED_FEEDBACK; 
      free(osd);
      xnee_dlclose(xd, xosd_lib);
      return XNEE_XOSD_FAILURE ; 
    }

  xnee_set_xosd_font(xd, xosd_default_font);
  xosd_char(osd, xosd_font);


  *(void**)xosd_char_p = xnee_dlsym(xd, xosd_lib, "xosd_set_colour");
  xosd_char = (xosd_char_fun) *xosd_char_p;
  if (xosd_char==NULL) 
    { 
      feedback_used=XNEE_UNDEFINED_FEEDBACK; 
      free(osd);
      xnee_dlclose(xd, xosd_lib);
      return XNEE_XOSD_FAILURE ; 
    }
  xosd_char(osd, "yellow");

  xosd_print_p = &xosd_print;
  *(void**)xosd_print_p = xnee_dlsym(xd, xosd_lib, "xosd_display");
  if (xosd_print==NULL) 
    { 
      feedback_used=XNEE_UNDEFINED_FEEDBACK; 
      free(osd);
      xnee_dlclose(xd, xosd_lib);
      return XNEE_XOSD_FAILURE ; 
    }
  
  return XNEE_OK;
}

static int
feedback_init(xnee_data *xd)
{
  int ret ;

  /* this should not be the case */
  if ( feedback_requested==XNEE_UNDEFINED_FEEDBACK )
    {
      feedback_used = XNEE_NO_FEEDBACK ; 
      return XNEE_FEEDBACK_FAILURE;
    }

  /* this happends when we already have init the requested feedback */
  if ( feedback_used==feedback_requested )
    {
      return XNEE_OK;
    }
  

  /* OK a new feedback is requested */

  /* anything to close ? */

  /* is the current XOSD? .. then close it */
  if ( feedback_used==XNEE_XOSD_FEEDBACK )
    {
       xnee_xosd_close(xd);
    }
  else if ( feedback_used==XNEE_STDERR_FEEDBACK )
    {
      ; 
    }

  if ( feedback_requested==XNEE_XOSD_FEEDBACK )
    {
      ret = xnee_setup_xosd(xd);
      if (ret == XNEE_OK) 
	{
	  feedback_used = XNEE_XOSD_FEEDBACK;
	}
    }
  else if (feedback_requested==XNEE_STDERR_FEEDBACK )
    {
      ret = XNEE_OK;
      feedback_used = XNEE_STDERR_FEEDBACK;
    } 
  else if (feedback_requested==XNEE_NO_FEEDBACK )
    {
      ret = XNEE_OK;
      feedback_used = XNEE_NO_FEEDBACK;
    } 
  else 
    {
      ret = XNEE_OK;
      feedback_used = XNEE_UNDEFINED_FEEDBACK;
    } 
  return ret;
}

int 
xnee_xosd_close(xnee_data *xd)
{
   xnee_verbose((xd, "xnee_xosd_close\n"));
   XNEE_FREE_IF_NOT_NULL(osd);
   if (xosd_lib!=NULL)
   {
      xnee_dlclose(xd, xosd_lib);
   }
   XNEE_FREE_IF_NOT_NULL(xosd_font);
   return XNEE_OK;
}

int 
xnee_feedback_close(xnee_data *xd)
{
  xnee_xosd_close(xd);
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
feedback(xnee_data *xd, char *str, va_dcl  valist)
{
  char *str;
  va_list ap;
  int conv;
  int ret;
  va_start(argp);
  str = va_arg(argp, char *);
#endif
  if (str==NULL)
     return XNEE_SYNTAX_ERROR;
  
  conv = vsnprintf (buf, 200, str, ap );
  if (conv>=200)
    conv=199;
  buf[conv]='\0';
  
  feedback_init(xd); 
  if (feedback_used==XNEE_XOSD_FEEDBACK)
    {
      xosd_print(osd, 0, XOSD_string, buf); 
      ret = XNEE_OK;
    }
  else if (feedback_used==XNEE_STDERR_FEEDBACK)
    {
      fprintf (stderr,"%s", buf);
      ret = XNEE_OK;
    }
  else if (feedback_used==XNEE_NO_FEEDBACK)
    {
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
   xnee_verbose((xd, "xnee_set_stderr_feedback\n"));
   feedback_requested=XNEE_STDERR_FEEDBACK;
   return XNEE_OK;
}

int 
xnee_get_requested_feedback(xnee_data *xd)
{
   xnee_verbose((xd, "xnee_get_requested_feedback\n"));

   return feedback_requested;
}



int 
xnee_set_no_feedback(xnee_data *xd)
{
   xnee_verbose((xd, "xnee_set_no_stderr_feedback\n"));
   feedback_requested=XNEE_NO_FEEDBACK;
   return XNEE_OK;
}

int 
xnee_set_xosd_feedback(xnee_data *xd)
{
   xnee_verbose((xd, "xnee_set_xosd_feedback\n"));
   feedback_requested=XNEE_XOSD_FEEDBACK;
   return XNEE_OK;
}


int 
xnee_is_xosd_feedback(xnee_data *xd)
{
  xnee_verbose((xd, "xnee_is_xosd_feedback used=%d\n", feedback_used));
   return (feedback_used==XNEE_XOSD_FEEDBACK);
}


int 
xnee_is_stderr_feedback(xnee_data *xd)
{
   xnee_verbose((xd, "xnee_is_stderr_feedback  used=%d\n", feedback_used));
  return (feedback_used==XNEE_STDERR_FEEDBACK);
}

int 
xnee_is_no_feedback(xnee_data *xd)
{
   xnee_verbose((xd, "xnee_is_no_feedback used=%d\n", feedback_used));
   return (feedback_used==XNEE_NO_FEEDBACK);
}



