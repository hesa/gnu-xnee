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


#ifndef XNEE_FEEDBACK_H
#define XNEE_FEEDBACK_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#ifdef HAVE_STDARG_H
#include <stdarg.h>
#else
#include <varargs.h> 
#endif


#include "libxnee/xnee.h"

#define FEEDBACK_DELAY     5
#define FEEDBACK_TIMEOUT    3

#define XNEE_FEEDBACK_INS_MARK_STR "Xnee inserted mark in log file"
#define XNEE_FEEDBACK_STOP_STR     "Xnee stopped"
#define XNEE_FEEDBACK_START_STR    "Xnee started"
#define XNEE_FEEDBACK_PAUSE_STR    "Xnee paused"
#define XNEE_FEEDBACK_RESUME_STR   "Xnee resumed"

#define XNEE_FEEDBACK_INS_MARK     feedback_fp(XNEE_FEEDBACK_INS_MARK_STR); 
#define XNEE_FEEDBACK_STOP         feedback_fp(XNEE_FEEDBACK_STOP_STR);     
#define XNEE_FEEDBACK_START        feedback_fp(XNEE_FEEDBACK_START_STR);    
#define XNEE_FEEDBACK_PAUSE        feedback_fp(XNEE_FEEDBACK_PAUSE_STR);    
#define XNEE_FEEDBACK_RESUME       feedback_fp(XNEE_FEEDBACK_RESUME_STR);   


enum feedback_types 
{
  XNEE_NO_FEEDBACK      = 0 ,
  XNEE_STDERR_FEEDBACK      ,
  XNEE_XOSD_FEEDBACK        ,
  XNEE_UNDEFINED_FEEDBACK   
} ;


#ifdef HAVE_STDARG_H
int  feedback(xnee_data *xd, char *str, ... )  __attribute__ ((format ( printf, 2 , 3)));
#else
int  feedback(xnee_data *xd, va_dcl valist)
  
#endif


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



int 
xnee_get_requested_feedback(xnee_data *xd);

#define STDERR_REQ(xd) 
#define XOSD_REQ(xd)   
#define NO_REQ(xd)     

#define xnee_fb_no_requested(xd)     (XNEE_NO_FEEDBACK     == xnee_get_requested_feedback(xd) )
#define xnee_fb_stderr_requested(xd) (XNEE_STDERR_FEEDBACK == xnee_get_requested_feedback(xd) )
#define xnee_fb_xosd_requested(xd)   (XNEE_XOSD_FEEDBACK   == xnee_get_requested_feedback(xd) )

int 
xnee_set_xosd_feedback(xnee_data *xd);

int 
xnee_set_no_feedback(xnee_data *xd);

int 
xnee_set_stderr_feedback(xnee_data *xd);

int 
xnee_is_xosd_feedback(xnee_data *xd);

int 
xnee_is_stderr_feedback(xnee_data *xd);

int 
xnee_is_no_feedback(xnee_data *xd);

int 
xnee_feedback_close(xnee_data *xd);

int 
xnee_feedback( xnee_data *, char *str, ... );


#endif /* XNEE_FEEDBACK_H */

