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
 * Foundation, Inc., 51 Franklin Street, Boston,            
 * MA  02110-1301, USA.                                              
 ****/


#ifndef XNEE_XNEE_H
#define XNEE_XNEE_H

#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#include <sys/utsname.h>


#include "libxnee/x11_files.h"

#include "libxnee/xnee_internal.h"
#include "libxnee/xnee_strings.h"
#include "libxnee/xnee_error.h"

#define  XNEE_CLI       "cnee"
#define  XNEE_CLI_UC    "CNEE"
  
typedef int xnee_keymask;

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
    XNEE_NO_PROJECT_FILE   ,
    XNEE_NO_MAIN_DATA      ,
    XNEE_NO_RECORD_DATA    ,
    XNEE_NO_REPLAY_DATA    ,
    XNEE_SYNTAX_ERROR      , 
    XNEE_UNKNOWN_GRAB_MODE ,
    XNEE_NO_GRAB_DATA      ,
    XNEE_GRAB_DATA         ,
    XNEE_BAD_GRAB_DATA     ,
    XNEE_BAD_LOG_FILE      ,
    XNEE_BAD_SPEED         ,
    XNEE_BAD_RESOLUTION    ,
    XNEE_BAD_THRESHOLD     ,
    XNEE_BAD_CONTEXT       ,
    XNEE_BLANK_LINE        ,
    XNEE_XOSD_FAILURE      ,
    XNEE_FEEDBACK_FAILURE  ,
    XNEE_MODE_NOT_SET      ,
    XNEE_GRAB_MEM_FAILURE  ,
    XNEE_RECORD_FAILURE    ,
    XNEE_DATE_FAILURE      ,
    XNEE_SCREEN_MISSING    ,
    XNEE_RANGE_FAILURE     ,
    XNEE_BAD_OFFSET        ,
    XNEE_WINDOW_POS_ADJ_ERROR     ,
    XNEE_MISSING_ARG       ,
    XNEE_OK_LEAVE          ,
    XNEE_GRAB_CONFUSION    ,
    XNEE_PROJECT_SYNTAX_ERROR  ,
    XNEE_LAST_ERROR
  } _return_values;


enum bool_string_values
  {
    XNEE_BOOL_EXPLICIT_FALSE=0,
    XNEE_BOOL_IMPLICIT_TRUE,
    XNEE_BOOL_EXPLICIT_TRUE,
    XNEE_BOOL_ERROR
  } _bool_string_values;

  

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

enum
  {
    XNEE_ANY_OPTION,
    XNEE_GENERAL_OPTION,
    XNEE_RECORD_OPTION,
    XNEE_REPLAY_OPTION,
    XNEE_SYNC_OPTION,
    XNEE_GRAB_OPTION,
    XNEE_MISC_OPTION,
    XNEE_INTERNAL_OPTION,
    XNEE_OBSOLETE_OPTION
  } xnee_option_type;

typedef struct
{
  int   key;
  char *option; 
  char *short_option; 
  char *args; 
  char *description; 
  int   type;
  int   visible;
} xnee_option_t;

/*
typedef struct
{
  xnee_option_t   **options;
  int             nr_of_options;
} xnee_options_t;
*/

typedef struct 
{
  Bool  new_project;   
  char *project_name ;
  char *project_descr;
  char *creat_date;
  char *creat_prog;
  char *creat_prog_vers;
  char *last_date;
  char *last_prog;
  char *last_prog_vers;
  char *author_name;
  char *author_email;
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




/*! \brief Holds a 
 *
 */
typedef struct
{
  KeyCode         key ;         /*!< key */
  char           *str;          /*!<  string representation of the key */
  char           *extra_str;
} xnee_action_key;




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

} old_xnee_key_code;


/*! \brief Holds information about keycodes needed to fake a letter press
 *
 */
typedef struct
{
  KeyCode kc ;       /*!< key to fake */ 
  KeyCode mod_keycodes[XNEE_NR_OF_MODIFIERS];
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
  int  x_offset ;    /*!< Xserver offset for X */
  int  y_offset ;    /*!< Xserver offset for Y */
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




/** 
 * Used for holding scripting data 
 *
 * x             mouse position's X coord  
 * y             mouse position's Y coord
 * x_rel         X coord is relative 
 * y_rel         Y coord is relative  
 * button        nr of button to fake
 * button_state  XNEE_PRESS or XNEE_RELEASE 
 * key           nr of key to fake
 * key_state     XNEE_PRESS or XNEE_RELEASE 
 * valid         1 if whole struct is valid, else 0 
 * msecs         Nr of msecs to sleep before fakeing
 * kc            keycode of the key to fake
 *
 */
typedef struct 
{
  /* Mouse */
  int x ; 
  int y ; 
  int x_rel  ; 
  int y_rel  ; 
  int button ; 
  int button_state ; 

  /* Keyboard */
  int key    ; 
  int key_state    ;

  int valid ; 
  int msecs ; 

  xnee_key_code kc;

} xnee_script_s ;


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


                                                                      



/**
 * Holds information about the xnee_record session
 */
typedef struct
{
  Bool            first_last     ;  /*!< when true, only first and last motion events are printed */
  Bool            last_motion    ;  /*!< was the last event a motion event */
  int               store_mouse_pos;  /*!< shall we save the mouse position before starting recording */
  Bool            store_window_pos;  /*!< shall we store every new window position   0=don't, 1=only for window pos, 2=window pos and user resuested recording*/
  unsigned long   server_time    ;  /*!< when the X11 data did occur          */
  int             x              ;  /*!< last MotionNotify RootX-value        */
  int             y              ;  /*!< last MotionNotify RootY-value        */
  int             events_recorded;  /*!< .. to Intercept                      */
  int             data_recorded  ;  /*!< .. to Intercept                      */
  int             time_recorded  ;  /*!< .. to Intercept                      */
  int             events_max     ;  /*!< .. to Intercept                      */
  int             data_max       ;  /*!< .. to Intercept                      */
  int             time_max       ;  /*!< .. to Intercept                      */
  unsigned int    interval       ;  /*!< how many seconds to record           */
  unsigned int    size           ;  /*!< max size of file                     */

  int             replayed_events;

  int data_ranges[XNEE_NR_OF_TYPES] ;  /*!< Count how many data ranges specified */

  int             interrupt ;

} xnee_record_init_data ; 




/**
 * 
 */
typedef struct
{

  int     grab         ;   /*!< true if any key     is grabbed */
  int     grabbed_action ; /*!< set to the action when grabbed */

  xnee_action_key action_keys[XNEE_GRAB_LAST];

} xnee_grab_keys;





/**
 * Holds information about Record Extension setup
 */
typedef struct
{
  /*@null@*/ /*@only@*/   XRecordClientSpec * xids; 
  /*@null@*/ /*@only@*/   XRecordState      * rState; 
  /*@null@*/ /*@only@*/   XRecordRange     ** range_array ; 

  XID		      id ;
  XRecordContext      rContext;
  int 	data_flags;
  int	major_return;
  int   minor_return ;
  int   nclients; 
  int   active;
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
  /*@null@*/ char    *program_name;    /*!< name of the program currently using libxnee */
  /*@null@*/ char    *out_name    ;    /*!< name of output file (e.g stdout, /tmp/xnee.log*/
  /*@null@*/ char    *err_name    ;    /*!< name of error file  (e.g stdout, /tmp/xnee.log*/
  /*@null@*/ char    *rc_name     ;    /*!< name of resource file (e.g netscape.xns, /tmp/xterm.xns*/
  /*@null@*/ char    *data_name   ;    /*!< name of data file (e.g */
  /*@null@*/ char    *rt_name     ;    /*!< name of retype file (e.g stdout, /home/user/myfile.txt */

  /*@null@*/ /*@dependent@*/FILE    *data_file   ;    /*!< data input file descriptor */
  /*@null@*/ /*@dependent@*/FILE    *out_file    ;    /*!< output file descriptor */
  /*@null@*/ /*@dependent@*/ FILE    *err_file    ;    /*!< error file descriptor */
  /*@null@*/ /*@dependent@*/FILE    *rc_file     ;    /*!< resource file descriptor */
  /*@null@*/ /*@dependent@*/FILE    *rt_file     ;    /*!< retype file descriptor */
  /*@null@*/ /*@dependent@*/FILE    *buffer_file ;    /*!< verbose buffer printout file descriptor */

  FILE    *saved_out_file    ;    /*!< output file descriptor */
  FILE    *saved_err_file    ;    /*!< error file descriptor */

  char   **app_args    ;

  Bool     verbose     ;    /*!< true if verbose mode */
  Bool     buf_verbose ;    /*!< true if verbose mode for buffer printouts */
  Bool     all_clients ;    /*!< True if recording all clients 
			      (else Xneee recods only future clients) */
  Bool     sync        ;    /*!< True if Record used when replaying */
  unsigned char  mode  ;    /*!< Xnee's current mode (RECORDER/REPLAY/SYNTAX_CHECK...)  */

  /*@null@*/ void *plugin_handle  ;        /*!< Handle for the plugin file */
  /*@null@*/ char *plugin_name    ;        /*!< Name of the plugin file */
  callback_ptr rec_callback ;   /*!< recording callback function  */
  callback_ptr rep_callback ;   /*!< replaying callback function  */
  callback_ptr sync_fun     ;   /*!< synchronisation function     */

  
  fprint_fptr buffer_verbose_fp; /*!< pointer to buffer verbose fun */
  vfprint_fptr verbose_fp;        /*!< pointer to verbose fun */
  fprint_fptr data_fp   ;        /*!< pointer to xnee protcol print fun */
   
  /*@null@*/ /*@observer@*/
  /*@null@*/ char    * display    ;    /*!< char representation of the Display */
  /*@null@*/ Display *data        ;    /*!< used for sending recored data between Xnee and Xserver*/
  /*@null@*/ Display *control     ;    /*!< used for sending info between Xnee and Xserver  */
  /*@null@*/ Display *fake        ;    /*!< used for faking events  */
  /*@null@*/ Display *grab        ;    /*!< used for holding the grabbed key/modifier */
  int first_replayed_event;  /*!< True if the event to replay is the first one. 
			       Needed to set the start time of the first event to 0 */
  int recall_recorded_win_pos; /*!< True if Xnee (during replay) shall use the recorded. 
			            window position*/
  int     cont         ;     /*!< A simple flag telling Xnee wether to keep 
			       recording/replaying or to quit. */
  /*@null@*/ xnee_distr *distr_list ;  /*!< array of displays to distribute events to */
  size_t     distr_list_size ; /*!< size of array of displays to distribute events to */
  /*@null@*/    /*@reldef@*/ 
#ifdef XNEE_USE_SEMAPHORES
  sem_t   *buf_sem     ;    /*!< semaphore to protect the replay buffer */
#endif /* XNEE_USE_SEMAPHORES */
  long first_read_time ;    /*!< server time of the first read from recorded file */
  int     force_replay ;    /*!< Keep replaying even if we are out of sync .... dangerous */
   
  XKeyboardState kbd_orig;  /*!< User keyboard state before Xnee messes is up */
  int     autorepeat_saved; /*!< Flag indicating if we have a stored keyboard state */
   
  /*@only@*/ /*@null@*/ 
  xnee_record_init_data    xnee_info ; 

  /*@only@*/ /*@null@*/ 
  xnee_recordext_setup     *record_setup;

  /*@only@*/ /*@null@*/ 
  xnee_testext_setup       *replay_setup;
   
  int                      data_buffer[4][XNEE_REPLAY_BUFFER_SIZE];
  struct buffer_meta_data  meta_data;
  int                      speed_percent;
  /*@only@*/ /*@null@*/ 
  xnee_grab_keys           *grab_keys;

  int     button_pressed ;
  int     key_pressed ;
  
  xnee_resolution_info   res_info; 
  xnee_resource_meta     xrm;

  XModifierKeymap *map ;
  
  int              in_use;

} xnee_data ; 






/**
 * Checks if the first argument is equals to any of the other two arguments  
 * @param arg       string to compare  
 * @param long_arg  with this one  
 * @param short_arg and with this one  
 * @return int      1 if arg was equal to any of long_arg or short_arg, else it returns 0  
 */
int 
xnee_check ( const char *arg, const char *long_arg , const char *short_arg );






#define xnee_print_obsolete_mess(s) fprintf (stderr, s)


int
xnee_start(xnee_data *xd);

int 
xnee_check_true(char *expr);

int 
xnee_check_false(char *expr);



/**
 * Allocates a new xnee_data structure. 
 * To free the memory, xnee_free_xnee_data can be used.
 * @return xnee_data * NULL if alloc failed
 */
/*@null@   The implementation of this function is located in xnee_alloc.c*/

xnee_data*
xnee_new_xnee_data(void);



#define DEBUG_XNEE_DATA                                        
#ifdef DEBUG_XNEE_DATA                                        
void                                                          
xnee_debug_xnee_data(xnee_data *xd, char *str)                ;
#endif


#endif /*   XNEE_XNEE_H */

