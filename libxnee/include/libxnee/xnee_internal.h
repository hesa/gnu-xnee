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


#ifndef XNEE_XNEE_INTERNAL_H
#define XNEE_XNEE_INTERNAL_H


#include <stdio.h>
#include <unistd.h>

#include <signal.h>

#include "libxnee/xnee_settings.h"



#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDARG_H
#include <stdarg.h>
#else
#include <varargs.h> 
#endif


/* needed for the RECORD extension */
#ifndef NEED_EVENTS
#define NEED_EVENTS
#endif  /* NEED_EVENTS */

#ifndef NEED_REPLIES
#define NEED_REPLIES
#endif  /* NEED_REPLIES */



#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif


/* If not using GNU C skip: __attribute__ */
#ifndef __GNUC__
#  define  __attribute__(x)  /*NOTHING AT ALL*/
#endif

#define XNEE_FREE(a)               (void)xnee_free(a);   
#define XNEE_FCLOSE(a)             (void)fclose(a); 

#define XNEE_FREE_AND_NULL(a)      XNEE_FREE(a); a=NULL; 
#define XNEE_FCLOSE_AND_NULL(a)    XNEE_FCLOSE(a); a=NULL; 

#define XNEE_FREE_IF_NOT_NULL(a)   if (a!=NULL) { XNEE_FREE_AND_NULL(a);   }
#define XNEE_FCLOSE_IF_NOT_NULL(a) if (a!=NULL) { XNEE_FCLOSE_AND_NULL(a); }
#define XNEE_CLOSE_DISPLAY_IF_NOT_NULL(a) if(a!=NULL) { XCloseDisplay(a); a=NULL; }



/* ** Do NOT edit ** */
enum _xnee_data_types {
  XNEE_EVENT  = 0,
  XNEE_REQUEST  ,
  XNEE_REPLY             ,
  XNEE_ERROR             ,
  XNEE_DELIVERED_EVENT   ,
  XNEE_DEVICE_EVENT      ,
  XNEE_EXT_REPLY_MAJOR   ,
  XNEE_EXT_REPLY_MINOR   ,
  XNEE_EXT_REQUEST_MAJOR ,
  XNEE_EXT_REQUEST_MINOR ,
  XNEE_NR_OF_TYPES       ,
  XNEE_META_DATA         ,
  XNEE_REPLAY_DATA       ,
  XNEE_SETTINGS_DATA     ,
  XNEE_MARK_DATA         ,
  XNEE_ACTION_DATA       ,
  XNEE_PRIMITIVE_DATA    ,
  XNEE_PROJECT_INFORMATION_DATA,
  XNEE_NO_DATA          
} xnee_data_types ;

/* ** Do NOT edit ** */
enum _xnee_mode {
  XNEE_NOMODE    = 0,
  XNEE_REPLAYER     ,
  XNEE_RECORDER     ,
  XNEE_RETYPER      ,        
  XNEE_DISTRIBUTOR          
} xnee_mode ;




/* this macro might be dangerous 
   .. don't use when incrementing variables (e.g i++) */
#define XNEE_MAX(x, y)   ((x)>(y)?(x):(y))

/* this macro might be dangerous 
   .. don't use when decrementing variables (e.g i++) */
#define XNEE_MIN(x, y)   ((x)>(y)?(y):(x))

/* this macro might be dangerous 
   .. don't use when incrementing variables (e.g i++) */
#define XNEE_ABS(x)   ((x)>0?(x):(0-(x)))

#ifdef XNEE_WANT_DEBUG
#define XNEE_DEBUG(arg) fprintf arg 
#else
#define XNEE_DEBUG(arg) 
#endif

#ifdef XNEE_WANT_SYNC_DEBUG
#define XNEE_SYNC_DEBUG(arg) fprintf arg 
#else
#define XNEE_SYNC_DEBUG(arg) 
#endif


/* AIX's XTestFakeKeyEvent has a bug with the last argument 
 * requires 0 - so we use usleep instead 
 *  
 *       OBSOLETE 
 *
 */
#ifdef  AIX
#define XTEST_ERR_SLEEP(per)  usleep (per*1000)
#else
#define XTEST_ERR_SLEEP(per)  
#endif
/* EO OBSOLETE */



#define XNEE_FAKE_SLEEP(per)  { if (per>1000)usleep (per*1000); }

#define XNEE_RETURN_IF_ERR(ret_val) \
   if (ret_val!=XNEE_OK)            \
   {                                                                    \
      const char *xnee_macro_tmp ;                                      \
      (void) fprintf (stderr, "Xnee error\n");                          \
      xnee_macro_tmp = xnee_get_err_description (ret);                  \
      (void) fprintf (stderr, "Description: %s\n", xnee_macro_tmp );               \
      xnee_macro_tmp = xnee_get_err_solution (ret) ;                    \
      (void) fprintf (stderr, "Solution:    %s\n", xnee_macro_tmp);     \
      return (ret);                                                     \
   }

#define XNEE_RETURN_NULL_IF_ERR(ret_val) \
   if (ret_val != XNEE_OK)               \
   {                                     \
      return (NULL);                     \
   }

#define XNEE_RETURN_VOID_IF_ERR(ret_val) \
   if (ret_val != XNEE_OK)               \
   {                                     \
      return;                            \
   }

/*
 * Resolution states
 *
 */
enum xnee_resolution_states
  {
    XNEE_RESOLUTION_UNSET  = -1,
    XNEE_RESOLUTION_USED   =  0,
    XNEE_RESOLUTION_UNUSED =  1
  } _xnee_resolution_states;

/* 
 * Grab modes/actions
 */
enum xnee_grab_modes 
  {
    XNEE_GRAB_STOP    = 0,
    XNEE_GRAB_PAUSE   ,
    XNEE_GRAB_RESUME  ,
    XNEE_GRAB_INSERT  ,
    XNEE_GRAB_EXEC    ,
    XNEE_GRAB_LAST,
    XNEE_GRAB_NODATA = 0,
    XNEE_GRAB_SET    = 1,
    XNEE_GRAB_UNKOWN  = 15
  } _xnee_grab_modes;

/*
 *  continue_process commnd enum
 */
enum cont_proc_commands
  {
    XNEE_PROCESS_RESET = 0 ,
    XNEE_PROCESS_INC       ,
    XNEE_PROCESS_DEC       ,
    XNEE_PROCESS_GET
  } _cont_proc_commands;

typedef void (*callback_ptr)( XPointer, XRecordInterceptData *); 
typedef callback_ptr *callback_ptrptr;                                         

typedef void (*synch_ptr)( XPointer, XRecordInterceptData *); 
typedef synch_ptr *synch_ptrptr;                                         

typedef int (*print_fptr)  ( const char*, ... ); 
typedef int (*fprint_fptr) (FILE*,  const char*, ... ); 
typedef int (*vfprint_fptr) (FILE*,  const char*, va_list ap ); 


#endif /*  XNEE_XNEE_INTERNAL_H */
