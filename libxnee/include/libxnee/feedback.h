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


int 
feedback( xnee_data *, char *str, ... );




