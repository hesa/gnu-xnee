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


#ifndef XNEE_XNEE_H
#define XNEE_XNEE_H

/* needed for the RECORD extension */
#define NEED_EVENTS
#define NEED_REPLIES



#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#ifdef HAVE_STDARG_H
#include <stdarg.h>
#else
#include <varargs.h> 
#endif

#include <sys/utsname.h>

#include <X11/Xlibint.h>
#include <X11/Xproto.h>
#include <X11/Xlib.h>

#include <X11/extensions/record.h> 


#define XNEE_MAX_DELAY       3600

/* macros for replay buffer */
#define XNEE_HIGHEST_DATA_NR  256
#define XNEE_REPLAY_BUFFER_SIZE ( XNEE_HIGHEST_DATA_NR  )


#define XNEE_FREE(a)           free(a);   a=NULL; 
#define XNEE_FCLOSE(a)         fclose(a); a=NULL; 
#define XNEE_FREE_IF_NULL(a)   if (a!=NULL) XNEE_FREE(a)
#define XNEE_FCLOSE_IF_NULL(a) if(a!=0) XNEE_FCLOSE(a)

#define XNEE_RANGE_STRING_SIZE 512

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

#define xnee_delay_timeout 5000 
#define XNEE_DELAY 10
#define XNEE_MISSING_DATA_DELAY (10*5)

/* this macro might be dangerous 
   .. don't use when incrementing variables (e.g i++) */
#define XNEE_MAX(x, y)   ((x)>(y)?(x):(y))

/* this macro might be dangerous 
   .. don't use when decrementing variables (e.g i++) */
#define XNEE_MIN(x, y)   ((x)>(y)?(y):(x))

/* this macro might be dangerous 
   .. don't use when incrementing variables (e.g i++) */
#define XNEE_ABS(x)   ((x)>0?(x):(0-(x)))



#define XNEE_FREE_AND_NULL(x) { free(x); x=NULL;}

#define XNEE_MAX_SYNCH     100 /* buffer size */
#define XNEE_MOTION_DELAY   21 /* default value for delay during synch */

#define XNEE_BUFFER_MAX      6  /* */
#define XNEE_BUFFER_MIN      -6 /* */
#define XNEE_BUFFER_SUM_MAX  20 /* */
#define XNEE_BUFFER_SUM_MIN -2 /* */
#define XNEE_BUFFER_TOT_MAX  20 /* */

#define MAX_OUT_OF_SYNC      2 /* number of data allowed to be out of sync */
#define MAX_NOT_IN_SYNC      2
#define MAX_OUT_OF_SYNC      2 /* number of data allowed to be out of sync */
#define MAX_UNSYNC_LOOPS    10 /* number check-loops when out of sync 
				  before exit  */
#define MAX_SKIPPED_UNSYNC  10 /* number of times to ignore unsync state  */
#define XNEE_NOT_REPLAYABLE 13


#define XNEE_USEC_PER_SEC        1000000
#define XNEE_MSEC_PER_SEC        1000
#define XNEE_FROM_LAST_READ 	'l'
#define XNEE_FROM_FIRST_READ 	'f'
#define XNEE_TOO_FAST_ADJ_PERCENTAGE		10


#define XNEE_DELAY_RECORDING (1000*500)

#define XNEE_RECEIVED   1
#define XNEE_REPLAYED   2   

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

#define XNEE_TRUE  1
#define XNEE_FALSE 0

/* 
 * Return values
 */
enum return_values 
{ 
  XNEE_OK =           0  ,
  XNEE_MEMORY_FAULT      ,
  XNEE_FILE_NOT_FOUND    ,
  XNEE_TIMED_OUT         ,
  XNEE_USER_INTR         ,
  XNEE_SYNCH_FAULT       ,
  XNEE_WRONG_PARAMS      ,
  XNEE_NO_REC_EXT        ,
  XNEE_NO_TEST_EXT       ,
  XNEE_NO_PROT_CHOOSEN   ,
  XNEE_NOT_OPEN_DISPLAY  ,
  XNEE_AMBIGOUS_CMD      ,
  XNEE_OUT_OF_SYNC       ,
  XNEE_NOT_SYNCING       ,
  XNEE_NO_PLUGIN_FILE    ,
  XNEE_PLUGIN_FILE_ERROR ,
  XNEE_NO_RESOURCE_FILE  ,
  XNEE_NO_MAIN_DATA      ,
  XNEE_SYNTAX_ERROR      , 
  XNEE_UNKNOWN_GRAB_MODE ,
  XNEE_NO_GRAB_DATA      ,
  XNEE_GRAB_DATA         ,
  XNEE_BAD_GRAB_DATA     ,
  XNEE_BAD_LOG_FILE      ,
  XNEE_BAD_SPEED         ,
  XNEE_BAD_RESOLUTION    ,
  XNEE_BAD_THRESHOLD     ,
  XNEE_BAD_CONTEXT
} _return_values;
  

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
    XNEE_GRAB_NODATA = 0,
    XNEE_GRAB_SET    = 1,
    XNEE_GRAB_STOP    ,
    XNEE_GRAB_PAUSE   ,
    XNEE_GRAB_RESUME  ,
    XNEE_GRAB_INSERT  ,
    XNEE_GRAB_EXEC    ,
    XNEE_GRAB_UNKOWN  = 15
  } _xnee_grab_modes;

#define XNEE_GRAB_MODIFIER 1
#define XNEE_GRAB_KEY      2


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

#define XNEE_HOME_URL     "http://www.gnu.org/software/xnee/"
#define XNEE_MAIL         "info-xnee@gnu.org"
#define XNEE_BUG_MAIL     "bug-xnee@gnu.org"
#define XNEE_DEVEL_MAIL   "xnee-devel@gnu.org"
#define GNU_HOME_URL      "http://www.gnu.org/"
#define FSF_HOME_URL      "http://www.fsf.org/"
#define XNEE_PLUGIN_DIR   "/usr/local/lib/xnee/plugins" 
#define XNEE_RESOURCE_DIR "/usr/local/lib/xnee/resources" 

#define DATA_NAME_SIZE_MAX 32

#define SYNCHRONIZATION_EVENT (CreateNotify | MapRequest | ConfigureRequest | DestroyNotify | MapNotify | ConfigureNotify | UnmapNotify )

#define XNEE_COMMENT_START_STR  "#"
#define XNEE_COMMENT_START_CHAR '#'

#define XNEE_ERR_FILE              "err-file"
#define XNEE_OUT_FILE              "out-file"

#define XNEE_DISPLAY              "display"
#define XNEE_FIRST_LAST           "first-last"
#define XNEE_ALL_EVENTS           "all-events"
#define XNEE_ALL_CLIENTS          "all-clients"
#define XNEE_DIMENSION            "Dimension"
#define XNEE_EVENT_MAX            "events-to-record"
#define XNEE_DATA_MAX             "data-to-record"
#define XNEE_TIME_MAX             "time-left-to-record"
#define XNEE_LOOPS_LEFT           "loops-left"
#define XNEE_STOP_KEY             "stop-key"
#define XNEE_PAUSE_KEY            "pause-key"
#define XNEE_RESUME_KEY           "resume-key"
#define XNEE_INSERT_KEY           "insert-key"
#define XNEE_EXEC_KEY             "exec-key"
#define XNEE_EVERYTHING           "everything"
#define XNEE_DISTRIBUTE           "distribute"
#define XNEE_NO_EXPOSE            "no-expose"
#define XNEE_NO_SYNC              "no-sync"
#define XNEE_LOOPS                "loops"
#define XNEE_FORCE_REPLAY         "force-replay"
#define XNEE_RESOURCE             "resource"
#define XNEE_PLUGIN               "plugin"
#define XNEE_ERROR_FD             "stderr"
#define XNEE_VERBOSE              "verbose"
#define XNEE_BUFFER_VERBOSE       "buffer-verbose"
#define XNEE_MOUSE                "mouse"
#define XNEE_KEYBOARD             "keyboard"

#define XNEE_REQUEST_STR          "request-range"
#define XNEE_REPLIES_STR          "reply-range"
#define XNEE_DEVICE_EVENT_STR     "device-event-range"
#define XNEE_DELIVERED_EVENT_STR  "delivered-event-range"
#define XNEE_ERROR_STR            "error-range"
#define XNEE_REPLY_STR            "reply-range"
#define XNEE_EXT_REQ_MAJ_STR      "extension-request-major-range"
#define XNEE_EXT_REQ_MIN_STR      "extension-request-minor-range"
#define XNEE_EXT_REP_MAJ_STR      "extension-reply-major-range"
#define XNEE_EXT_REP_MIN_STR      "extension-reply-minor-range"
#define XNEE_META_END		  "META-END"  /* Marks the end of the META DATA in the recorded file*/ 

#define XNEE_REPLAY_CALLBACK_NAME "xnee_replay_dispatch"
#define XNEE_RECORD_CALLBACK_NAME "xnee_record_dispatch"
#define XNEE_SYNC_FUNCTION_NAME   "xnee_sync_fun"

#define REPLAY_CALLBACK 1
#define RECORD_CALLBACK 2
#define SYNC_CALLBACK   3

/* 
 * TO KEEP COMPATIBILITY WITH OLD 
 * - SCRIPTS USING XNEE
 * - XNEE PLUGINS
 * THE OLD SYNTAX IS ALLOWED 
 * ... UNTIL ???  
 */
/* START OF OBSOLETE  */
#define XNEE_OBSOLETE_FIRST_LAST           "first_last"
#define XNEE_OBSOLETE_ALL_EVENTS           "all_events"
#define XNEE_OBSOLETE_ALL_CLIENTS          "all_clients"
#define XNEE_OBSOLETE_LOOPS_LEFT           "loops_left"
#define XNEE_OBSOLETE_STOP_KEY             "stop_key"
#define XNEE_OBSOLETE_EVERYTHING           "everything"
#define XNEE_OBSOLETE_NO_EXPOSE            "no_expose"
#define XNEE_OBSOLETE_NO_SYNC              "no_sync"
#define XNEE_OBSOLETE_ERR_FILE             "err_file"
#define XNEE_OBSOLETE_OUT_FILE             "out_file"

#define XNEE_OBSOLETE_REQUEST_STR          "core-requests"
#define XNEE_OBSOLETE_DEVICE_EVENT_STR     "device_events"
#define XNEE_OBSOLETE_DELIVERED_EVENT_STR  "delivered_events"
#define XNEE_OBSOLETE_ERROR_STR            "errors"
#define XNEE_OBSOLETE_REPLY_STR            "core_replies"
#define XNEE_OBSOLETE_EXT_REQ_MAJ_STR      "ext_requests.ext_major"
#define XNEE_OBSOLETE_EXT_REQ_MIN_STR      "ext_requests.ext_minor"
#define XNEE_OBSOLETE_EXT_REP_MAJ_STR      "ext_replies.ext_major"
#define XNEE_OBSOLETE_EXT_REP_MIN_STR      "ext_replies.ext_minor"

/* END OF OBSOLETE */


/**
 * String representation of Xnee's error codes
 *
 *
static char *err_codes[] = { 
  "Exited without error", 
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "Memory fault",                 
  "File not found",               
  "Time out reached",             
  "Signal interrupted"            
  "Synchronisation failure",      
  "Wrong parameters",             
  "X11 Record extension missing", 
  "X11 Test extension missing"    
  "No protocol choosen"           
  "Could not open Display"        
  "Ambigious command"             
  "Out of sync"                   
  "Not syncing"                   
  "Could not find resource file"  
};
*/

/** 
 * \brief simply an X event. 
 *
 */
typedef struct {
  int type;    /*!< Simply an X error. Put in a struct if things are to be added later on  */
}xnee_error;  

/** 
 * \brief simply an X event. 
 *
 */
typedef struct {
  int type;      /*!<  Simply an X reply. Put in a struct if things are to be added later on  */
}xnee_reply;

/** 
 * \brief simply an X event. 
 *
 */
typedef struct {
  int type;      /*!<  Simply an X requets. Put in a struct if things are to be added later on  */
}xnee_request;

/** 
 * \brief simply an X event. 
 *
 */
typedef struct {
  int type ;      /*!< type of Xevent (e.g MotionNotify)*/
  int x, y ;      /*!< x, y coordinates. These are only used when type is MotionXXX*/
  int button ;    /*!< x, y coordinates. These are only used when type is Button*/
  int keycode ;   /*!< x, y coordinates. These are only used when type is Key*/
  int screen_nr ; /*!< The screen on which the event occured */
}xnee_event;


struct data_description 
{
   int   data_nr;
   char *data_name;
   char *data_descr;
};



typedef struct 
{
  char * project_name ;
  char * project_descr;
  char * creat_date;
  char * creat_prog;
  char * creat_prog_vers;
  char * last_date;
  char * last_prog;
  char * last_prog_vers;
  char * author_name;
  char * author_email;
} xnee_resource_meta ; 


/*! \brief Holds information about Record Extension setup
 *
 */
typedef struct
{
  int	 xtest_version_major ;  /*!< Major version number of XTest */
  int    xtest_version_minor ;  /*!< Minor version number of XTest */
  int	 xtest_error_basep   ;  /*!< First error number for this extension*/
  int    xtest_event_basep   ;  /*!< First event number for this extension*/
} xnee_testext_setup;




/*! \brief Holds a integer tuple describing key + modifier
 *
 */
typedef struct
{
  int	 key ;             /*!< key */
  int    modifier ;        /*!< modifier key  */
} xnee_km_tuple;




/*! \brief Holds a information about the sync state
 *
 */
typedef struct
{
  int	 max   ;       /*!< max positive diff  */
  int    min   ;       /*!< max negative diff  */
  int    total ;       /*!< total diff  */
} xnee_diff;



/*! \brief Holds information about keycodes needed to fake a letter press
 *
 */
typedef struct
{
  KeyCode kc ;       /*!< key to fake */ 
  int shift_press ;  /*!< is a SHIT press needed */ 
  int alt_press   ;  /*!< is a ALT press needed */ 
  int alt_gr_press;  /*!< is a ALT GRAPH press needed */ 
  int ctrl_press  ;  /*!< is a CTRL press needed */ 
 
} xnee_key_code;




/*! \brief Resolution of X server
 *
 */
typedef struct
{
  int	 x_res ;  /*!< Xserver resoluton:   x */
  int    y_res ;  /*!< Xserver resoluton:   y */
} xnee_res;


/*! \brief Data needed for distribution 
 *
 */
typedef struct
{
  xnee_res res      ;  /*!< resolution when replaying */
  int      is_used  ;  /*!< flag to say if we should 
			 convert resolution at all */
  Display  *dpy     ; 
} xnee_distr;


/*! \brief Resolution of X server
 *
 */
typedef struct
{
  xnee_res record ;  /*!< resolution when recorded   */
  xnee_res replay ;  /*!< resolution when replaying */
  int  is_used    ;  /*!< flag to say if we should convert resolution at all */
} xnee_resolution_info;



/*! \brief time scale settings for Xnee
 *
 */
typedef struct
{
  int  percent    ;  /*!< percentage of the original time (0-10000)  */
  int  is_used    ;  /*!< flag to say if we should scale time at all */
} xnee_timescale_info;




/*! \brief holds replay delay information.
 *  Xnee chooses at replay/fake-time one of these to 
 *  use as delay.
 */
typedef struct
{
  Time f_delay ;  /*!< time to wait before next event is faked */
  Time s_delay  ; /*!< time to sleep before next event is faked */
} xnee_delay_time;



/** 
 * Used for holding data of what we've received 
 *
 *  u          
 *  type       Type of event/request/error/reply... eg MotionNotify 
 *  newtime    
 *  oldtime                 
 *
 */
typedef struct {
  
  union {
    xnee_event   event ;
    xnee_request request ;
    xnee_reply   reply ;
    xnee_error   error ; 
  } u ;  /*!< What have we got ... event, request, reply or error  */
  int type ;     /*!< Type of event/request/error/reply... eg MotionNotify  */
  Time newtime ; /*!< Time when data (u.type) occured  */
  Time oldtime ; /*!< Remember when the last data occured*/
} xnee_intercept_data;



/* *
 * this typedef should, according to XRecord,  
 * specification be defined in <X11/extensions/record.h>
 * ... can't find .... errrh?
 *
 */
typedef union { 
  unsigned char    type ;
  xEvent           event ;
  xResourceReq     req   ;
  xGenericReply    reply ;
  xError           error ;
  xConnSetupPrefix setup;
} XRecordDatum ;


                                                                      
typedef void (*callback_ptr)( XPointer, XRecordInterceptData *); 
typedef callback_ptr *callback_ptrptr;                                         

typedef void (*synch_ptr)( XPointer, XRecordInterceptData *); 
typedef synch_ptr *synch_ptrptr;                                         

typedef int (*print_fptr)  ( const char*, ... ); 
typedef int (*fprint_fptr) (FILE*,  const char*, ... ); 
typedef int (*vfprint_fptr) (FILE*,  const char*, va_list ap ); 



/**
 * Holds information about the xnee_record session
 */
typedef struct
{
  Bool            first_last     ;  /*!< when true, only first and last motion events are printed */
  Bool            last_motion    ;  /*!< was the last event a motion event */
  
  unsigned long   server_time    ;  /*!< when the X11 data did occur       /     */
  int             x              ;  /*!< last MotionNotify RootX-value      /   */
  int             y              ;  /*!< last MotionNotify RootY-value       / */
  int             events_recorded;  /*!< .. to Intercept                     / */
  int             data_recorded  ;  /*!< .. to Intercept                     / */
  int             time_recorded  ;  /*!< .. to Intercept                     / */
  int             events_max     ;  /*!< .. to Intercept                     / */
  int             data_max       ;  /*!< .. to Intercept                     / */
  int             time_max       ;  /*!< .. to Intercept                     / */
  unsigned int    interval       ;  /*!< how many seconds to record         /   */
  unsigned int    size           ;  /*!< max size of file                  /     */

  Bool	all_events       ; /*!< Intercept all XEvents */
  Bool	everything       ; /*!< Intercept everything */
  Bool  no_expose        ; /*!< when true, no Expose or NoExpose will be printed out */

  int data_ranges[XNEE_NR_OF_TYPES] ;  /*!< Count how many data ranges specified */

} xnee_record_init_data ; 
/**
 * Holds information about Record Extension setup
 */
typedef struct
{

  int     grab         ;    /*!< true if any key+mod is grabbed */
  int     grabbed_action ;  /*!< set to the action when grabbed */

  int     stop_key     ;    /*!< key used to stop Xnee */
  int     stop_mod     ;    /*!< modifier used to stop Xnee */
  char   *stop_str     ;     /*!< string representation of the key+modifier */

  int     pause_key    ;    /*!< key used to pause Xnee */
  int     pause_mod    ;    /*!< modifier used to pause Xnee */
  char   *pause_str    ;     /*!< string representation of the key+modifier */

  int     resume_key   ;    /*!< key used to resume Xnee */
  int     resume_mod   ;    /*!< modifier used to resume Xnee */
  char   *resume_str   ;   /*!< string representation of the key+modifier */

  int     insert_key   ;    /*!< key used to insert a mark in Xnee's log */
  int     insert_mod   ;    /*!< modifier used to insert a mark in Xnee's log */
  char   *insert_str   ;    /*!< string representation of the key+modifier */

  int     exec_key   ;       /*!< key used to exec a program */
  int     exec_mod   ;       /*!< modifier used to exec a program */
  char   *exec_str   ;      /*!< string representation of the key+modifier */

} xnee_grab_keys;





/**
 * Holds information about Record Extension setup
 */
typedef struct
{
  XID		      id ;
  XRecordClientSpec * xids; 
  XRecordContext      rContext;
  XRecordState 	    * rState; 
  XRecordRange     ** range_array ; 
  /*  XRecordRange 	    * range;*/

  int 	data_flags;
  int	major_return, minor_return, nclients; 
} xnee_recordext_setup;



struct buffer_meta_data
{
  int sum_max  ;      
  /*!< sum of the maximum in the buffer. */

  int sum_min  ;      
  /*!< sum of the minimum values in the buffer. */

  int total_diff ; 
  /*!< the total sum of positive values in the buffer  */
  
  int cached_max  ;      
  /*!< a cached value of the maximum value in the buffer. 
    (-1 for unknown state) */
  
  int cached_min  ;      
  /*!< a cached value of the minimum value in the buffer. 
    (-1 for unknown state) */


  /* thresholds used during synch */
  int sum_max_threshold;
  int sum_min_threshold;
  int tot_diff_threshold;
  
} ;


/**
 * Holds most information about the Xnee session. 
 *
 */
typedef struct
{

  char    *out_name    ;    /*!< name of output file (e.g stdout, /tmp/xnee.log*/
  char    *err_name    ;    /*!< name of error file  (e.g stdout, /tmp/xnee.log*/
  char    *rc_name     ;    /*!< name of resource file (e.g netscape.xns, /tmp/xterm.xns*/
  char    *data_name   ;    /*!< name of data file (e.g */

  FILE    *data_file   ;    /*!< data input file descriptor */
  FILE    *out_file    ;    /*!< output file descriptor */
  FILE    *err_file    ;    /*!< error file descriptor */
  FILE    *rc_file     ;    /*!< resource file descriptor */
  FILE    *buffer_file ;    /*!< verbose buffer printout file descriptor */

  Bool     verbose     ;    /*!< true if verbose mode */
  Bool     buf_verbose ;    /*!< true if verbose mode for buffer printouts */
  Bool     all_clients ;    /*!< True if recording all clients 
			        (else Xneee recods only future clients) */
  Bool     sync        ;    /*!< True if Record used when replaying */
  Bool     mode        ;    /*!< Xnee's current mode (RECORDER/REPLAY...)  */

  void *plugin_handle  ;        /*!< Handle for the plugin file */
  callback_ptr rec_callback ;   /*!< recording callback function  */
  callback_ptr rep_callback ;   /*!< replaying callback function  */
  callback_ptr sync_fun     ;   /*!< synchronisation function     */

  /* OLD VERSION
    void (* rec_callback ) (XPointer , XRecordInterceptData *); 
    void (* rep_callback ) (XPointer , XRecordInterceptData *); 
    void (* sync_fun     ) (XPointer , XRecordInterceptData *); 
  */

  fprint_fptr buffer_verbose_fp; /*!< pointer to buffer verbose fun */
  vfprint_fptr verbose_fp;        /*!< pointer to verbose fun */
  fprint_fptr data_fp   ;        /*!< pointer to xnee protcol print fun */

  char    *display     ;    /*!< char representation of the Display */
  Display *data        ;    /*!< used for sending recored data between Xnee and Xserver*/
  Display *control     ;    /*!< used for sending info between Xnee and Xserver  */
  Display *fake        ;    /*!< used for faking events  */
  Display *grab        ;    /*!< used for holding the grabbed key/modifier */
  int first_replayed_event; /*!< True if the event to replay is the first one. 
                                 Needed to set the start time of the first event to 0 */
  int     cont         ;    /*!< A simple flag telling Xnee wether to keep 
			         recording/replaying or top quit. */
  xnee_distr *distr_list ;  /*!< array of displays to distribute events to */
  int     distr_list_size ; /*!< size of array of displays to distribute events to */
  sem_t   *buf_sem     ;    /*!< semaphore to protect the replay buffer */
  long first_read_time ;    /*!< server time of the first read from recorded file */
  int     force_replay ;    /*!< Keep replaying even if we are out of sync .... dangerous */
  XKeyboardState kbd_orig;  /*!< User keyboard stare before Xnee */
  int     glob_autorepeat ; /*!< Current global autorepeat state */

  xnee_record_init_data    *xnee_info ; 
  xnee_recordext_setup     *record_setup;
  xnee_testext_setup       *replay_setup;
  
  int                      data_buffer[4][XNEE_REPLAY_BUFFER_SIZE];
  struct buffer_meta_data  meta_data;
  int                      speed_percent;
  xnee_grab_keys           *grab_keys;

  int     button_pressed ;
  int     key_pressed ;
  
  xnee_resolution_info   res_info; 
  xnee_resource_meta     xrm;
} xnee_data ; 







/**
 * Returns the number of RecordRanges allocated so far
 * @param xd    xnee's main structure
 * @return int  number of RecordRanges allocated
 */
int
xnee_get_max_range (xnee_data *xd);





/* internal use */
void 
signal_handler(int sig) ;


/**
 * Opens the displays as specified in the xnee_data struct
 * @param xd    xnee's main structure
 * @return int  0 on success, all other values indicats error opening the displays
 */
int 
xnee_setup_display(xnee_data* xd);



/**
 * Returns the clients (displays) id (resource )
 * @param dpy   display of the client
 * @return XID  client's X ID
 */
XID 
xnee_client_id (Display *dpy);



/**
 * Adds a display to ditribution list
 * @param xd    xnee's main structure
 * @param disp  Display to add
 * @return int  0 on success
 */
int 
xnee_add_display_list (xnee_data* xd, char * disp);





/**
 * Adds a display to ditribution list
 * @param str  string representation of a display
 * @param xd   xnee's main structure
 * @return int XNEE_OK on success, XNEE_NOT_OPEN_DISPLAY if failure
 */
int 
xnee_add_display_str (char *str , xnee_data* xd);




/**
 * Adds a display to ditribution list
 * @param dpy    Display to add
 * @param xd     xnee's main structure
 * @return int   XNEE_OK on success, XNEE_NOT_OPEN_DISPLAY if failure
 */
int 
xnee_add_display (Display * dpy, xnee_data* xd);








/**
 * Init Xnee structures
 * @param xd   xnee's main structure
 * @return int 1 if successfull, else 0 
 * \todo (return values need to be changed) 
 */
int
xnee_init(xnee_data* xd);




/*
 *  INTERNAL USE
 *
 * Sleep for int seconds
 * and print some information to the user about the remaining time
 * 
 * Useful if a user wants to wait for a few seconds in order to iconize
 * the terminal window where record was started .... just an exanple
 *
 */
void xnee_delay (int , char *) ;



/**
 * Checks if the first argument is equals to any of the other two arguments  
 * @param arg       string to compare  
 * @param long_arg  with this one  
 * @param short_arg and with this one  
 * @return int      1 if arg was equal to any of long_arg or short_arg, else it returns 0  
 */
int 
xnee_check ( char *arg, char *long_arg , char *short_arg );




/**
 * open a display for the host as specified in 
 * xnee_program_data->program
 * @param xd           xnee's main structure
 * @return Display *   NULL if the display could not be opened
 */
Display * 
xnee_open_display (xnee_data *xd);




/**
 * Close down all open files and displays
 * @param xd     xnee's main structure
 * @return void  
 */
void
xnee_close_down(xnee_data *xd);




/**
 *  Set xnee_data->display to the value as 
 *  specified in environment variable DISPLAY
 * 
 * @param xd    xnee's main structure
 * @return void  
 */
void 
xnee_set_default_display (xnee_data *xd);



/**
 * Allocates a new xnee_data structure. 
 * To free the memory, xnee_free_xnee_data can be used.
 * @return xnee_data * NULL if alloc failed
 */
xnee_data*
xnee_new_xnee_data();



/**
 * Fres xnee_data structure. 
 * @param xd   data to free 
 * @return int  0 on success
 */
int 
xnee_free_xnee_data(xnee_data* xd );



/**
 * Allocates a new xnee_recordext_setup.
 * To free the memory allocated, use xnee_free_recordext_setup
 * @return xnee_recordext_setup *  NULL if alloc failed
 */
xnee_recordext_setup*
xnee_new_recordext_setup();


/**
 * Frees xnee_free_recordext_setup structure
 * @param xd      xnee's main structure
 * @return int    0 on success
 */
int 
xnee_free_recordext_setup(xnee_data* xd);


/**
 * Zeros the sync data in xd.
 * @param xd     xnee's main structure
 * @return void  
 */
void
xnee_zero_sync_data (xnee_data* xd);


/**
 * Opens the plugin file (pl_name). Xnee will use the function in this file instead of the built in
 * @param xd       xnee's main structure
 * @param pl_name  name of the plugin file
 * @return int     0 means OK.
 */
int
xnee_use_plugin(xnee_data *xd, char *pl_name);


/**
 * Removes XNEE_COMMENT_START from the argument
 * and removes unnecessary allocated memory
 * @param xd     xnee's main structure
 * @param str    string to clean up
 * @return int   1 on success
 */
int
xnee_rem_comment_start(xnee_data *xd, char *str) ;



/**
 * Removes blanks from the argument
 * and removes unnecessary allocated memory
 * 
 * @param xd    xnee's main structure
 * @param str   string to clean up
 * @return int  1 on success
 */
int
xnee_strip(xnee_data *xd, char *str) ;


/**
 * Set variables so that Xnee can stop gracefully. 
 * @param xd    xnee's main structure
 * @return int  0 on success
 */
int 
xnee_stop_session( xnee_data* xd);





#define xnee_print_obsolete_mess(s) fprintf (stderr, s)




int
rem_all_blanks (char *array, int size);


int
rem_begin_blanks (char *array, int size);

int
rem_blanks (char *array, int size);



int 
xnee_process_count(int mode);

int 
xnee_handle_km(xnee_data *xd);


int 
xnee_process_replies(xnee_data *xd);


int
xnee_set_autorepeat (xnee_data *xd);

int
xnee_reset_autorepeat (xnee_data *xd);

KeyCode
xnee_str2keycode(xnee_data* xd, char *str );

KeyCode
xnee_char2keycode (xnee_data *xd, char token, xnee_key_code *kc);

int
xnee_write_settings_to_file (xnee_data *xd, FILE *fp);

int
xnee_renew_xnee_data(xnee_data *xd);

int
xnee_start(xnee_data *xd);

int
xnee_more_to_record(xnee_data *xd);

int
xnee_err_handler(Display* dpy, XErrorEvent* ev);

#endif /*   XNEE_XNEE_H */

