#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>
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
xnee_set_xosd_feedback(xnee_data *xd);

int 
xnee_set_stderr_feedback(xnee_data *xd);

int 
xnee_is_xosd_feedback(xnee_data *xd);

int 
xnee_is_stderr_feedback(xnee_data *xd);

int 
xnee_feedback_close(xnee_data *xd);

int 
xnee_feedback( xnee_data *, char *str, ... );




