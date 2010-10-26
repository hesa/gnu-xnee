/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003,
 *                      2004, 2005, 2006, 2007, 2008,
 *                      2009, 2010  Henrik Sandklef
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
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <dlfcn.h>

#include <sys/utsname.h>


#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/xnee_dl.h"
#include "libxnee/xnee_sem.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_resolution.h"
#include "libxnee/xnee_resource.h"
#include "libxnee/xnee_grab.h"
#include "libxnee/xnee_km.h"
#include "libxnee/xnee_range.h"
#include "libxnee/datastrings.h"
#include "libxnee/xnee_expr.h"
#include "libxnee/xnee_alloc.h"
#include "libxnee/xnee_utils.h"
#include "libxnee/xnee_session.h"
#include "libxnee/xnee_display.h"
#include "libxnee/print.h"

#define  INC_ARGS_USED(a, str) if(str!=NULL) a=1; else a=0;


int
xnee_option2id(xnee_data *xd, 
 	       xnee_option_t *options, 
	       const char *str, 
	       int syntax_type);


static xnee_option_t xnee_options_impl[] = 
  {
    /* 
     * Key 
     * long option
     * short option
     * arg description
     * description
     * type of option (record/replay,general ... 
     */
    {  
      XNEE_ERR_FILE_KEY,
      "err-file",
      "e",
      "<file>",
      "Redirect all Xnee verbose output file_name"
      "(default is stderr)", 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },
    {
      XNEE_OUT_FILE_KEY,
      "out-file",
      "o",
      "<file>",
      "Redirect all Xnee data to file_name"
      "(default is stdout)", 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },    
    {
      XNEE_DISPLAY_KEY,
      "display",
      "d",
      "<displayname>",
      "X server to contact (default is localhost)",
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },    
    { 
      XNEE_DIMENSION_KEY,
      "Dimension",
      NULL,
      "<displayname>",
      "Dimension when recorded",
      XNEE_INTERNAL_OPTION,
      XNEE_OPTION_VISIBLE
    },
    { 
      XNEE_LOOPS_KEY,
      "loops-left",
      NULL,
      "<nr>",
      "Nr of events to record (obsolete)",
      XNEE_OBSOLETE_OPTION,
      XNEE_OPTION_VISIBLE
    },
    {
      XNEE_FIRST_LAST_KEY,
      "first-last",
      NULL,
      NULL,
      "Print only first and last of multiple successive" 
      " MotionEvent",		  
      XNEE_RECORD_OPTION,
      XNEE_OPTION_VISIBLE
    },
    {
      XNEE_ALL_EVENTS_KEY,
      "all-events",
      NULL,
      NULL,
      "Intercept all events",
      XNEE_RECORD_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_ALL_CLIENTS_KEY,
      "all-clients",
      "ac", 
      NULL,
      "Record all client's data (default)",
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },
    {
      XNEE_FUTURE_CLIENTS_KEY,
      "future-clients",
      NULL,
      NULL,
      "Record future client's data", 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },
    {
      XNEE_EVENT_MAX_KEY,
      "events-to-record",
      "etr",
      "<n>",
      "Intercept n numbers of events ( n<0 means forever)",
      XNEE_RECORD_OPTION,
      XNEE_OPTION_VISIBLE
    },    
    {
      XNEE_DATA_MAX_KEY,
      "data-to-record",
      "dtr",
      "<n>",
      "Intercept n numbers of data ( n<0 means forever)", 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },
    
    {
      XNEE_TIME_MAX_KEY,
      "seconds-to-record",
      "str", 
      "<n>",
      "Intercept for n seconds ( n<0 means forever)"	,
      XNEE_RECORD_OPTION,
      XNEE_OPTION_VISIBLE
    },
    
    {
      XNEE_STOP_KEY_KEY,
      "stop-key",
      "sk",
      "<key>",
      "When pressing key Xnee exits", 
      XNEE_GRAB_OPTION,
      XNEE_OPTION_VISIBLE
    },
    
    {
      XNEE_PAUSE_KEY_KEY,
      "pause-key",
      "pk",
      "<key>",
      "When pressing key Xnee pauses its current action", 
      XNEE_GRAB_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_RESUME_KEY_KEY,
      "resume-key",
      "rk",
      "<key>",
      "When pressing key Xnee resumes its paused action", 
      XNEE_GRAB_OPTION,
      XNEE_OPTION_VISIBLE
    },
    
    {
      XNEE_INSERT_KEY_KEY,
      "insert-key",
      "ik",
      "<key>", 
      "When pressing key Xnee inserts a META mark"
      "in the session file" , 
      XNEE_GRAB_OPTION,
      XNEE_OPTION_VISIBLE
    },
    
    {
      XNEE_EXEC_KEY_KEY,
      "exec-key",
      "ek",
      "<key>",
      "When pressing modifier mod and key key Xnee inserts "
      "an exec mark into the session file" ,
      XNEE_GRAB_OPTION,
      XNEE_OPTION_VISIBLE
    },
    
    {
      XNEE_EXEC_PROGRAM_KEY,
      "exec-program-key",
      "ep",
      "<programname>", 
      "Program to start when pressing the grabbed exec key",
      XNEE_GRAB_OPTION,
      XNEE_OPTION_VISIBLE
    },


    {
      XNEE_DELAY_TIME_KEY,
      "time",
      "t",
      "<secs>",
      "Delay start of application for <secs> seconds. Can be "
      "used to prevent recording of KeyRelease when starting "
      "Xnee from terminal",
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_SPEED_PERCENT_KEY,
      "speed-percent",
      "sp",
      "<speed>",
      "Set replaying speed percentage is set to speed",
      XNEE_REPLAY_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_RECORDED_RESOLUTION_KEY,
      "recorded-resolution",
      NULL,
      "<res>", 
      "Resolution to use when recording is set to res",
      XNEE_RECORD_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_REPLAY_RESOLUTION_KEY,
      "replay-resolution",
      NULL,
      "<res>", 
      "Resolution to use when replaying is set to res",
      XNEE_REPLAY_OPTION,
      XNEE_OPTION_VISIBLE
    },
    
    {
      XNEE_ADJUST_RESOLUTION_KEY,
      "resolution-adjustment",
      NULL, 
      NULL, 
      "Use resolution adjustment",
      XNEE_REPLAY_OPTION,
      XNEE_OPTION_VISIBLE
    },
    
    { 
      XNEE_RECORD_OPTION_KEY,
      "record",
      "rec",
      NULL, 
      "Set recording mode" , 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    { 
      XNEE_REPLAY_OPTION_KEY,
      "replay",
      "rep",
      NULL, 
      "Set replaying mode" , 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_DISTRIBUTE_KEY,
      "distribute",
      "di",
      "<LIST>",
      "Distribute recorded or replayed events to LIST where"
      " LIST is comma separated list of displays",
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },
    
    { 
      XNEE_SYNC_MODE_KEY,
      "synchronised-replay",
      NULL,
      NULL,
      "Synchronise during replay",
      XNEE_REPLAY_OPTION,
      XNEE_OPTION_VISIBLE
    },
    
    {
      XNEE_NO_SYNC_MODE_KEY,
      "no-synchronise",
      "ns", 
      NULL,
      "Don't use synchornisation during replay",
      XNEE_REPLAY_OPTION,
      XNEE_OPTION_VISIBLE
    },
    {
      XNEE_FUTURE_CLIENTS_KEY,
      "future-clients",
      "fc", 
      NULL,
      "Record future client's data (default)",
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_RECALL_WINDOW_POS_KEY,
      "recall-window-position",
      "rwp",
      NULL,
      "Recall the recorded window position to be used"
      " during replay",
      XNEE_REPLAY_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_HUMAN_PRINTOUT_KEY,
      "human-printout",
      "hp", 
      NULL,
      "Prints human readable",
      XNEE_RECORD_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_FORCE_REPLAY_KEY,
      "force-replay",
      "fp", 
      NULL,
      "Keep replaying even if we are out of sync"
      "  .... dangerous",   
      XNEE_REPLAY_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_PLUGIN_KEY,
      "plugin",
      "p",
      "<name", 
      "Use the plugin name",
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_VERBOSE_KEY,
      "verbose",
      "v", 
      NULL,
      "Enable verbose printout",
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_BUFFER_VERBOSE_KEY,
      "buffer-verbose",
      "bv", 
      NULL,
      "Enable verbose printout of replay buffer",
      XNEE_REPLAY_OPTION,
      XNEE_OPTION_VISIBLE
    },
  
    {
      XNEE_STORE_MOUSE_POS_KEY,
      "store-mouse-position",
      "smp", 
      NULL, 
      "Store the initial position of the mouse",
      XNEE_RECORD_OPTION,
      XNEE_OPTION_VISIBLE
    },
  
    {
      XNEE_FEEDBACK_XOSD_KEY,
      "feedback-xosd",
      "fx",  
      NULL, 
      "Use xosd to feedback",
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_FEEDBACK_STDERR_KEY,
      "feedback-stderr",
      "fs", 
      NULL, 
      "Use stderr to feedback",
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_FEEDBACK_NONE_KEY,
      "feedback-none",
      "fn", 
      NULL, 
      "Dont' use feedback",
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_REQUEST_STR_KEY,
      "request-range",
      "reqra",
      "<X_LIST>", 
      "Set request range to X_LIST",
      XNEE_RECORD_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_REPLY_STR_KEY,
      "reply-range",
      "repra",
      "<X_LIST>", 
      "Set reply range to X_LIST",
      XNEE_RECORD_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_DEVICE_EVENT_STR_KEY,
      "device-event-range",
      "devera",
      "<X_LIST>", 
      "Set device event range to X_LIST",
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_DELIVERED_EVENT_STR_KEY,
      "delivered-event-range",
      "devra",
      "<X_LIST>",
      "Set delivered event range to X_LIST", 
      XNEE_RECORD_OPTION,
      XNEE_OPTION_VISIBLE
    },
  
    {
      XNEE_ERROR_STR_KEY,
      "error-range",
      "erra",
      "<X_LIST>", 
      "Set error range to X_LIST",
      XNEE_RECORD_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_EXT_REQ_MAJ_STR_KEY,
      "extension-request-major-range",
      "erqmar",
      "<X_LIST>",
      "Set extension request major range to X_LIST",
      XNEE_RECORD_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_EXT_REQ_MIN_STR_KEY,
      "extension-request-minor-range",
      "erqmir",
      "<X_LIST>", 
      "Set extension request minor range to X_LIST", 
      XNEE_RECORD_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_EXT_REP_MAJ_STR_KEY,
      "extension-reply-major-range",
      "erpmar",
      "<X_LIST>", 
      "Set extension reply major range to X_LIST", 
      XNEE_RECORD_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_EXT_REP_MIN_STR_KEY,
      "extension-reply-minor-range",
      "erpmir",
      "<X_LIST>", 
      "Set extension reply minor range to X_LIST", 
      XNEE_RECORD_OPTION,
      XNEE_OPTION_VISIBLE
    },


    {
      XNEE_MAX_THRESHOLD_KEY,
      "max-threshold",
      "mat",
      "<nr>", 
      "Set the maximum threshold for sync to nr",
      XNEE_REPLAY_OPTION,
      XNEE_OPTION_VISIBLE
    },
    
    {
      XNEE_MIN_THRESHOLD_KEY,
      "min-threshold",
      "mit",
      "<nr>", 
      "Set the minimum threshold for sync to tnr",
      XNEE_REPLAY_OPTION,
      XNEE_OPTION_VISIBLE
    },
    
    {
      XNEE_TOT_THRESHOLD_KEY,
      "tot-threshold",
      "tot",
      "<nr>", 
      "Set the total threshold for sync to nr",
      XNEE_REPLAY_OPTION,
      XNEE_OPTION_VISIBLE
    },
    
    {
      XNEE_REPLAY_OFFSET_KEY,
      "replay-offset",
      "ro",
      "<x,y>",
      "Set the replay offset to (x,y)",
      XNEE_REPLAY_OPTION,
      XNEE_OPTION_VISIBLE
    },
    {
      XNEE_KEEP_AUTOREPEAT,
      "keep-autorepeat",
      "ka",
      NULL,
      "Keep autorepeat during record/replay",
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_RETYPE_PRESS_DELAY,
      "retype-press-delay",
      "rpd",
      NULL,
      "Delay (in milli seconds) after faked key press when retryping file ",
      XNEE_RETYPE_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_RETYPE_RELEASE_DELAY,
      "retype-release-delay",
      "rrd",
      NULL,
      "Delay (in milli seconds) after faked key release when retryping file ",
      XNEE_RETYPE_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_NO_REPARENT_RECORD,
      "no-reparent-recording",
      "nrr",
      NULL,
      "Prevent explicit recording of ReparentNotify event",
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },


    {
      XNEE_LAST_OPTION,
      NULL,
      NULL,
      NULL,
      NULL,
      XNEE_LAST_OPTION,
      XNEE_LAST_OPTION
    }
  } ;

xnee_option_t *xnee_options = xnee_options_impl;




int
xnee_free_xnee_resource_meta(xnee_resource_meta* xrm)
{
  xrm->new_project = 0;
  XNEE_FREE_AND_NULL (xrm->project_name);
  XNEE_FREE_AND_NULL (xrm->project_descr);
  XNEE_FREE_AND_NULL (xrm->creat_date);
  XNEE_FREE_AND_NULL (xrm->creat_prog);
  XNEE_FREE_AND_NULL (xrm->creat_prog_vers);
  XNEE_FREE_AND_NULL (xrm->last_date);
  XNEE_FREE_AND_NULL (xrm->last_prog);
  XNEE_FREE_AND_NULL (xrm->last_prog_vers);
  XNEE_FREE_AND_NULL (xrm->author_name);
  XNEE_FREE_AND_NULL (xrm->author_email);
  return XNEE_OK;
}

		  


/**************************************************************
 *                                                            *
 * xnee_handle_resource_meta                                  *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_handle_resource_meta (xnee_data *xd, char *meta_str)
{
  char *tmp = meta_str;
  char *value;
  int len;

  len = strlen (tmp);
  if ( len <= 2)
    {
      xnee_verbose ((xd,"skipping meta data: \"%s\" \n", tmp));
      return XNEE_OK;
    }
  else
    tmp++;

  if (len>0)
    XNEE_REMOVE_BEGINING_BLANKS(tmp);

  value=strstr (tmp, ":");

  if (value==NULL)
    return XNEE_OK;

  value += 1 ;  
  
  if (strlen(value)>2) 
    {
      XNEE_REMOVE_BEGINING_BLANKS(value);
    }
  XNEE_REMOVE_TRAILING_CRAP(value);

  

  if (!XNEE_RESOURCE_CHECK(XNEE_RES_PROJECT,tmp))
    {
      xnee_verbose ((xd, 
		     "adding to xd: " XNEE_RES_PROJECT "=\"%s\" \n", 
		     value));
      xnee_set_project_name(xd, value);
    }
  else if (!XNEE_RESOURCE_CHECK(XNEE_RES_DESCRIPTION,tmp))
    {
      xnee_verbose ((xd, 
		     "adding to xd: " XNEE_RES_DESCRIPTION "=\"%s\" \n", 
		     value));
      xnee_set_project_descr(xd, value);
    }
  else if (!XNEE_RESOURCE_CHECK(XNEE_RES_CREAT_DATE,tmp))
    {
      xnee_verbose ((xd, 
		     "adding to xd: " XNEE_RES_CREAT_DATE"=\"%s\" \n", 
		     value));
      xnee_set_creat_date(xd, value);
    }
  else if (!XNEE_RESOURCE_CHECK(XNEE_RES_CREAT_PROGRAM,tmp))
    {
      xnee_verbose ((xd, 
		     "adding to xd: " XNEE_RES_CREAT_PROGRAM"=\"%s\" \n", 
		     value));
      xnee_set_creat_program(xd, value);
    }
  else if (!XNEE_RESOURCE_CHECK(XNEE_RES_CREAT_PROG_VER,tmp))
    {
      xnee_verbose ((xd, 
		     "adding to xd: " XNEE_RES_CREAT_PROG_VER "=\"%s\" \n", 
		     value));
      xnee_set_creat_prog_vers(xd, value);
    }
  else if (!XNEE_RESOURCE_CHECK(XNEE_RES_LASTCHANGE_DATE,tmp))
    {
      xnee_verbose ((xd, 
		     "adding to xd: " XNEE_RES_LASTCHANGE_DATE "=\"%s\" \n", 
		     value));
      xnee_set_last_date(xd, value);
    }
  else if (!XNEE_RESOURCE_CHECK(XNEE_RES_LASTCHANGE_PROGRAM,tmp))
    {
      xnee_verbose ((xd, 
		     "adding to xd: "XNEE_RES_LASTCHANGE_PROGRAM "=\"%s\" \n",
		     value));
      xnee_set_last_program(xd, value);
    }
  else if (!XNEE_RESOURCE_CHECK(XNEE_RES_LASTCHANGE_PROG_VER,tmp))
    {
      xnee_verbose ((xd, 
		     "adding to xd: " XNEE_RES_LASTCHANGE_PROG_VER"=\"%s\" \n",
		     value));
      xnee_set_last_prog_vers(xd, value);
    }
  else if (!XNEE_RESOURCE_CHECK(XNEE_RES_AUTHOR_NAME,tmp))
    {
      xnee_verbose ((xd, 
		     "adding to xd: " XNEE_RES_AUTHOR_NAME "=\"%s\" \n", 
		     value));
      xnee_set_author_name(xd, value);
    }
  else if (!XNEE_RESOURCE_CHECK(XNEE_RES_AUTHOR_EMAIL,tmp))
    {
      xnee_verbose ((xd, 
		     "adding to xd: " XNEE_RES_AUTHOR_EMAIL"=\"%s\" \n", 
		     value));
      xnee_set_author_email(xd, value);
    }
  else 
    {
      xnee_verbose ((xd, 
		     "Could not add to xd: \"%s\" \n", 
		     value));
    }
  return XNEE_OK;
}



/**************************************************************
 *                                                            *
 * xnee_add_resource                                            *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_add_resource(xnee_data *xd)
{

#define TMP_BUF_SIZE 256
  static char tmp[TMP_BUF_SIZE] ;
  int read_more  = 1 ;
  int i ;
  int len;
  int ret;

  strncpy(tmp,"",TMP_BUF_SIZE);
  

  while (read_more!=0)
    {

      if ( fgets(tmp, 256, xd->rc_file) == NULL)
	{
	  return XNEE_OK;
	}

      if ( tmp == NULL)
	{
	  return XNEE_SYNTAX_ERROR;
	}

      /* remove trailing blanks and newlines|tab... */
      len = strlen(tmp);
      i=len;
      while ( (i>0) && 
	      ( tmp[i] != ' ' ) && 
	      ( tmp[i] != '\t' ) && 
	      ( tmp[i] != '\n' )  ) i--;
      tmp[i]='\0';

      /*
       * Hey, I __know__ we'll keep the char array....
       * as long as we need... 
       */
      /*       read_more=xnee_add_resource_syntax(xd, tmp); */
      ret=xnee_expression_handle_project(xd, tmp); 


      if ( (ret==XNEE_BLANK_LINE) ||
	   (ret==XNEE_META_DATA) ||
	   (ret==XNEE_SETTINGS_DATA) )
	{
	  ret = XNEE_OK;
	}

      if (ret != XNEE_OK)
	{
	  read_more = 0;
	}

      xnee_verbose((xd,"  adding : \"%s\" \t-----------------returned %d\n", 
		    tmp, read_more));

      if (ret==XNEE_SYNTAX_ERROR)
	{
	  return ret;
	}
    }

  return read_more;
}




int
xnee_key2id(xnee_data      *xd, 
	    xnee_option_t  *options, 
	    int             key)
{
  int i ;

  xnee_verbose((xd, "xnee_key2id key=%d\n", key));
  
  for (i=0;options[i].key!=XNEE_LAST_OPTION;i++)
    {
      if ( options[i].key == key )
	{
	  return i;
	}
    }  
  return XNEE_OPTION_NOT_FOUND; 
}


int
xnee_is_resource_option(xnee_data *xd, const char *str, int syntax_type)
{
  int id ;

  /*   printf ("'%s'     %s:%d  (%s)  str=%s (%lu)\n", */
  /* 	  str, __FILE__, __LINE__, __func__, str, str); */

  id = xnee_option2id(xd, xnee_options, str, syntax_type);

  if (id==XNEE_OPTION_NOT_FOUND)
    {
      return 0;
    }
  return id;
}


const char *
xnee_key2string(xnee_data      *xd, 
		xnee_option_t  *options, 
		int             key)
{
  int id ;

  id = xnee_key2id(xd, options, key);

  xnee_verbose((xd, "Looking for %d ===> %d \n",
		key, 
		id));

  if ( id == XNEE_OPTION_NOT_FOUND )
    {
      return XNEE_EMPTY_STRING;
    }
  return options[id].option;
}


int
xnee_option2id(xnee_data *xd, 
 	       xnee_option_t *options, 
	       const char *str, 
	       int syntax_type)
{
  int i =0;
  char *tmp;


  if (str==NULL)
    {
      return XNEE_SYNTAX_ERROR;
    }
  
  xnee_verbose((xd, "xnee_option2id '%s'\n",
		str));


  tmp=(char*)str;
  if (syntax_type==XNEE_CLI_SYNTAX)
    {
      if (tmp[0]=='-')
	{
	  tmp++;
	}
      if (tmp[0]=='-')
	{
	  tmp++;
	}
    }

  for (i=0;options[i].key!=XNEE_LAST_OPTION;i++)
    {
      /*             printf ("\t opt=%d %s %s %s\n", 
		     i,  
		     tmp, 
		     options[i].option, 
		     options[i].short_option); 
      */     
      if (xnee_check(tmp,
		     options[i].option,
		     options[i].short_option))
	{
	  return i;
	}
    }
  return XNEE_OPTION_NOT_FOUND; 
}


int
xnee_option2key(xnee_data *xd, 
		const char *str)
{
  int id ;

  id = xnee_option2id(xd,  xnee_options, 
		      str, XNEE_XNS_SYNTAX);

  if ( id == XNEE_OPTION_NOT_FOUND )
    {
      return XNEE_NO_OPTION_KEY;
    }
  return xnee_options[id].key;
}




int
xnee_find_option_entry_impl (xnee_data     *xd, 
			     xnee_option_t *options,
			     const char    *option, 
			     int            syntax_type)
{
  int i ; 
  int long_match  = 0;
  int short_match = 0;

  XNEE_VERBOSE_ENTER_FUNCTION();

  XNEE_VERBOSE_IN_FUNCTION(option);

  if (options==NULL)
    {
      xnee_verbose((xd, "\toption == NULL\n"));
      return XNEE_MEMORY_FAULT;
    }

/*   printf ("[option: %s] ", option);  */

  if (syntax_type==XNEE_CLI_SYNTAX)
    /* || (syntax_type==XNEE_XNS_SYNTAX) )*/
    {
      if (option[0]=='-')
	{
	  option++;
	}
      else
	{
	  return XNEE_OPTION_NOT_FOUND;
	}
      if (option[0]=='-')
	{
	  option++;
	}
    }

/*   printf (" -- [option: %s] 2222 ", option);  */

  for (i=0;options[i].key!=XNEE_LAST_OPTION;i++)
    {
      /*   printf ("\tCompare '%s' '%s'\n", 
 		    options[i].option, 
 		    option); */
      xnee_verbose((xd, "\tCompare '%s'  '%s'\n",
		    options[i].option,
		    option));

      long_match = (!strncmp(option,  
			     options[i].option,
			     strlen(options[i].option))) &&
	(strlen(options[i].option)==strlen(option));

      if ( options[i].short_option != NULL )
	{
	  short_match = (!strncmp(option,  
				  options[i].short_option,
				  strlen(options[i].short_option))) &&
	    (strlen(options[i].short_option)==strlen(option));
	}
      if (long_match || short_match )
	{
	  xnee_verbose((xd,"** Found key: %d   for option %s "
			"at %d in option array  (%s)\n", 
			options[i].key, option, i, options[i].option));
	  return i;
	}
    }

  XNEE_VERBOSE_LEAVE_FUNCTION();
  return XNEE_OPTION_NOT_FOUND;
}



int
xnee_parse_option_impl(xnee_data *xd, char **opt_and_args, int *args_used, int syntax_mode)
{
  int ret = XNEE_OK;
  int key; 
  int entry = XNEE_OPTION_NOT_FOUND; 
  int opt_int;

  if (opt_and_args==NULL)
    {
      return -1;
    }

  if (syntax_mode == XNEE_CLI_SYNTAX)
    {
      entry = xnee_find_cli_option_entry(xd, 
					 xnee_options,
					 opt_and_args[0]);
    }
  else if (syntax_mode == XNEE_XNS_SYNTAX)
    {
/*    xnee_set_verbose(xd);  */
      entry = xnee_find_resource_option_entry(xd, 
					      xnee_options,
					      opt_and_args[0]);
/*    xnee_unset_verbose(xd);  */
    }
  else
    {
      fprintf (stderr, "Type of parse unspecified.... internal error. Report this\n");
    }
  
  if ( entry == XNEE_OPTION_NOT_FOUND )
    {
      return -1 ;
    }

  key = xnee_options[entry].key;

  xnee_verbose((xd, "Found xns entry for '%s' '%s' at position: %d\n", 
		EMPTY_IF_NULL(opt_and_args[0]),
		EMPTY_IF_NULL(opt_and_args[1]), 
		entry));

  xnee_verbose((xd, "\tlong option: '%s'\n", 
		EMPTY_IF_NULL(xnee_options[entry].option)));
  xnee_verbose((xd, "\tshort option:'%s'\n", 
		EMPTY_IF_NULL(xnee_options[entry].short_option)));
  

  /*
    #define verbose_option(a)  \
    xnee_verbose((xd, "%s:%d %s Handling: '%s' \n", \
    __FILE__ , __LINE__, __func__, a ));
  */
#define verbose_option(a)  \
       xnee_verbose((xd,"%s:%d-->%s() Handling: '%s' \n", \
		     __FILE__ , __LINE__, __func__, a ));
  
  ret = XNEE_SYNTAX_ERROR;

  switch (key)
    {
    case XNEE_NO_OPTION_KEY:
      ret = XNEE_SYNTAX_ERROR;
      break;
      
    case XNEE_ERR_FILE_KEY:
      verbose_option("XNEE_ERR_FILE_KEY");
      ret = xnee_set_err_name (xd, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_OUT_FILE_KEY:
      verbose_option("XNEE_OUT_FILE_KEY");
      ret = xnee_set_out_name (xd, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_DISPLAY_KEY:
      verbose_option("XNEE_DISPLAY_KEY");
      ret = xnee_set_display_name (xd, opt_and_args[1]) ;
      *args_used = 1;
      break;

    case XNEE_FIRST_LAST_KEY:
      verbose_option("XNEE_FIRST_LAST_KEY");
      ret = xnee_set_first_last(xd);
      *args_used = 0;
/*       ret = xnee_set_first_list_str(xd, opt_and_args[1]); */
/*       *args_used = 1; */
      break;

    case XNEE_ALL_EVENTS_KEY:
      verbose_option("XNEE_ALL_EVENTS_KEY");
      ret = xnee_set_all_clients(xd);
      INC_ARGS_USED(*args_used, opt_and_args[1]);
      break;

    case XNEE_ALL_CLIENTS_KEY:
      verbose_option("XNEE_ALL_CLIENTS_KEY");
      ret = xnee_set_all_clients(xd);
      break;

    case XNEE_FUTURE_CLIENTS_KEY:
      verbose_option("XNEE_FUTURE_CLIENTS_KEY");
      ret = xnee_set_future_clients_str(xd, opt_and_args[1]);
      INC_ARGS_USED(*args_used, opt_and_args[1]);
      break;

    case XNEE_DIMENSION_KEY:
      verbose_option("XNEE_DIMENSION_KEY");
      ret = xnee_set_rec_resolution (xd, opt_and_args[1]); 
      *args_used = 1;
      break;

    case XNEE_EVENT_MAX_KEY:
      verbose_option("XNEE_EVENT_MAX_KEY");
      ret = xnee_set_events_max_str (xd, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_DATA_MAX_KEY:
      verbose_option("XNEE_DATA_MAX_KEY");
      ret = xnee_set_data_max_str (xd, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_TIME_MAX_KEY:
      verbose_option("XNEE_TIME_MAX_KEY");
      ret = xnee_set_time_max_str (xd, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_STOP_KEY_KEY:
      verbose_option("XNEE_STOP_KEY_KEY");
      ret = xnee_set_key (xd, XNEE_GRAB_STOP, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_PAUSE_KEY_KEY:
      verbose_option("XNEE_PAUSE_KEY_KEY");
      ret = xnee_set_key (xd, XNEE_GRAB_PAUSE, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_RESUME_KEY_KEY:
      verbose_option("XNEE_RESUME_KEY_KEY");
      ret = xnee_set_key (xd, XNEE_GRAB_RESUME, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_INSERT_KEY_KEY:
      verbose_option("XNEE_INSERT_KEY_KEY");
      ret = xnee_set_key (xd, XNEE_GRAB_INSERT, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_EXEC_KEY_KEY:
      verbose_option("XNEE_EXEC_KEY_KEY");
      ret = xnee_set_key (xd, XNEE_GRAB_EXEC, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_EXEC_PROGRAM_KEY:
      verbose_option("XNEE_EXEC_PROGRAM_KEY");
      ret = xnee_set_exec_prog (xd, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_DELAY_TIME_KEY:
      verbose_option("XNEE_DELAY_TIME_KEY");
      XNEE_ATOI_FUNCTION(xnee_set_interval, xd, opt_and_args[1], ret);
      *args_used = 1;
      break;

    case XNEE_SPEED_PERCENT_KEY:
      verbose_option("XNEE_SPEED_PERCENT_KEY");
      ret = xnee_set_replay_speed_str (xd, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_RECORDED_RESOLUTION_KEY:
      verbose_option("XNEE_RECORDED_RESOLUTION_KEY");
      ret = xnee_set_rec_resolution (xd, opt_and_args[1]); 
      *args_used = 1;
      break;

    case XNEE_REPLAY_RESOLUTION_KEY:
      verbose_option("XNEE_REPLAY_RESOLUTION_KEY");
      ret = xnee_set_rep_resolution (xd, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_ADJUST_RESOLUTION_KEY:
      verbose_option("XNEE_ADJUST_RESOLUTION_KEY");
      ret = xnee_set_resolution_str (xd, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_DISTRIBUTE_KEY:
      verbose_option("XNEE_DISTRIBUTE_KEY");
      ret = xnee_add_display_list (xd, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_RECORD_OPTION_KEY:            
      verbose_option("XNEE_RECORD_OPTION_KEY");
      ret = xnee_set_recorder(xd);
      break;

    case XNEE_REPLAY_OPTION_KEY:            
      verbose_option("XNEE_REPLAY_OPTION_KEY");
      ret = xnee_set_replayer(xd);
      break;

    case XNEE_NO_SYNC_MODE_KEY:
      verbose_option("XNEE_NO_SYNC_MODE_KEY");
      ret = xnee_unset_sync_mode (xd); 
/*       ret = xnee_unset_sync_mode_str (xd, opt_and_args[1]); */
/*       INC_ARGS_USED(*args_used, opt_and_args[1]); */
      break;

    case XNEE_SYNC_MODE_KEY:
      verbose_option("XNEE_SYNC_MODE_KEY");
      ret = xnee_set_sync_mode (xd); 
/*       ret = xnee_set_sync_mode_str (xd, opt_and_args[1]); */
/*       INC_ARGS_USED(*args_used, opt_and_args[1]); */
      break;

    case XNEE_RECALL_WINDOW_POS_KEY:
      verbose_option("XNEE_RECALL_WINDOW_POS_KEY");
      ret = xnee_set_recall_window_pos(xd);
      *args_used = 0;
      break;

    case XNEE_REPLAY_OFFSET_KEY:
      verbose_option("XNEE_REPLAY_OFFSET_KEY");
      ret = xnee_set_replay_offset_str (xd, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_KEEP_AUTOREPEAT:
      verbose_option("XNEE_KEEP_AUTOREPEAT");
      ret = xnee_set_keep_autorepeat (xd);
      *args_used = 0;
      break;

    case XNEE_HUMAN_PRINTOUT_KEY:
      verbose_option("XNEE_HUMAN_PRINTOUT_KEY");
      ret = xnee_set_human_printout(xd);
      *args_used = 0;
      break;

    case XNEE_LOOPS_KEY:
      verbose_option("XNEE_LOOPS_KEY");
      ret = xnee_set_events_max_str (xd, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_FORCE_REPLAY_KEY:
      verbose_option("XNEE_FORCE_REPLAY_KEY");
      ret = xnee_set_force_replay(xd);
      *args_used = 0;
      break;

    case XNEE_RESOURCE_KEY:
      verbose_option("XNEE_RESOURCE_KEY");
      ret = xnee_set_project_name(xd, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_PLUGIN_KEY:
      verbose_option("XNEE_PLUGIN_KEY");
      ret = xnee_set_plugin_name(xd, opt_and_args[1], 1);
      *args_used = 1;
      break;

    case XNEE_ERROR_FD_KEY:
      verbose_option("XNEE_ERROR_FD_KEY");
      ret = xnee_set_err_name (xd, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_VERBOSE_KEY:
      verbose_option("XNEE_VERBOSE_KEY");
      ret = xnee_set_verbose(xd);
      *args_used = 0;
      break;

    case XNEE_BUFFER_VERBOSE_KEY:
      verbose_option("XNEE_BUFFER_VERBOSE_KEY");
      ret = xnee_set_buf_verbose(xd);
      *args_used = 0;
      break;

    case XNEE_STORE_MOUSE_POS_KEY:
      verbose_option("XNEE_STORE_MOUSE_POS_KEY");
      ret = xnee_set_store_mouse_pos(xd);
      *args_used = 0;
      break;

    case XNEE_XOSD_FONT_KEY:
      verbose_option("XNEE_XOSD_FONT_KEY");
      ret = xnee_set_xosd_font(xd, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_FEEDBACK_XOSD_KEY:
      verbose_option("XNEE_FEEDBACK_XOSD_KEY");
      ret = xnee_set_xosd_feedback(xd);
      *args_used = 0;
      break;

    case XNEE_FEEDBACK_STDERR_KEY:
      verbose_option("XNEE_FEEDBACK_STDERR_KEY");
      ret = xnee_set_stderr_feedback(xd);
      *args_used = 0;
      break;

    case XNEE_FEEDBACK_NONE_KEY:
      verbose_option("XNEE_FEEDBACK_NONE_KEY");
      ret = xnee_set_no_feedback(xd);
      *args_used = 1;
      break;

    case XNEE_MAX_THRESHOLD_KEY:
      verbose_option("XNEE_MAX_THRESHOLD_KEY");
      ret = xnee_set_max_threshold_str(xd, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_MIN_THRESHOLD_KEY:
      verbose_option("XNEE_MIN_THRESHOLD_KEY");
      ret = xnee_set_min_threshold_str(xd, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_TOT_THRESHOLD_KEY:
      verbose_option("XNEE_TOT_THRESHOLD_KEY");
      ret = xnee_set_tot_threshold_str(xd, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_REQUEST_STR_KEY:
      verbose_option("XNEE_REQUEST_STR_KEY");
      ret = xnee_parse_range(xd, XNEE_REQUEST, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_REPLY_STR_KEY:
      verbose_option("XNEE_REPLY_STR_KEY");
      ret = xnee_parse_range(xd, XNEE_REPLY, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_DEVICE_EVENT_STR_KEY:
      verbose_option("XNEE_DEVICE_EVENT_STR_KEY");
      ret = xnee_parse_range(xd, XNEE_DEVICE_EVENT, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_DELIVERED_EVENT_STR_KEY:
      verbose_option("XNEE_DELIVERED_EVENT_STR_KEY");
      ret = xnee_parse_range(xd, XNEE_DELIVERED_EVENT, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_ERROR_STR_KEY:
      verbose_option("XNEE_ERROR_STR_KEY");
      ret = xnee_parse_range(xd, XNEE_ERROR, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_EXT_REQ_MAJ_STR_KEY:
      verbose_option("XNEE_EXT_REQ_MAJ_STR_KEY");
      ret = xnee_parse_range ( xd, XNEE_EXT_REQUEST_MAJOR, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_EXT_REQ_MIN_STR_KEY:
      verbose_option("XNEE_EXT_REQ_MIN_STR_KEY");
      ret = xnee_parse_range ( xd, XNEE_EXT_REQUEST_MINOR, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_EXT_REP_MAJ_STR_KEY:
      verbose_option("XNEE_EXT_REP_MAJ_STR_KEY");
      ret = xnee_parse_range ( xd, XNEE_EXT_REPLY_MAJOR, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_EXT_REP_MIN_STR_KEY:
      verbose_option("XNEE_EXT_REP_MIN_STR_KEY");
      ret = xnee_parse_range ( xd, XNEE_EXT_REPLY_MINOR, opt_and_args[1]);
      *args_used = 1;
      break;

    case XNEE_RETYPE_PRESS_DELAY:
      verbose_option("XNEE_RETYPE_PRESS_DELAY");
      opt_int = xnee_str2int(xd, opt_and_args[1]);
      ret = xnee_set_retype_press_delay ( xd, opt_int); 
      *args_used = 1;
      break;

    case XNEE_RETYPE_RELEASE_DELAY:
      verbose_option("XNEE_RETYPE_RELEASE_DELAY");
      opt_int = xnee_str2int(xd, opt_and_args[1]);
      ret = xnee_set_retype_release_delay ( xd, opt_int); 
      *args_used = 1;
      break;


    case XNEE_NO_REPARENT_RECORD:
      verbose_option("XNEE_NO_REPARENT_RECORD");
      ret = xnee_set_no_reparent_recording (xd); 
      *args_used = 0;
      break;





      /*
	case XNEE_REPLAY_CALLBACK_NAME_KEY:
	verbose_option("XNEE_REPLAY_CALLBACK_NAME_KEY");
	*args_used = 1;
	break;

	case XNEE_RECORD_CALLBACK_NAME_KEY:
	verbose_option("XNEE_RECORD_CALLBACK_NAME_KEY");
	*args_used = 1;
	break;

	case XNEE_SYNC_FUNCTION_NAME_KEY:
	verbose_option("XNEE_SYNC_FUNCTION_NAME_KEY");
	*args_used = 1;
	break;

      */

      /* ********** Below you'll find ********** 
       *
       *            non-valid options 
       * 
       * ****************************************/

    case XNEE_EXEC_MARK_KEY:
      verbose_option("XNEE_EXEC_MARK_KEY");
      *args_used = 1;
      fprintf (stderr, "%s can not be used as a command line option\n",
	       XNEE_EXEC_MARK);
      ret = XNEE_SYNTAX_ERROR;
      break;

    case XNEE_NEW_WINDOW_MARK_KEY:
      verbose_option("XNEE_NEW_WINDOW_MARK_KEY");
      fprintf (stderr, "%s can not be used as a command line option\n",
	       XNEE_NEW_WINDOW_MARK);
      ret = XNEE_SYNTAX_ERROR;
      *args_used = 1;
      break;

    case XNEE_EVERYTHING_KEY:
      verbose_option("XNEE_EVERYTHING_KEY");
      fprintf (stderr, "'everyting' is an obsoleted option\n");
      ret = XNEE_SYNTAX_ERROR;
      *args_used = 1;
      break;

    case XNEE_NO_EXPOSE_KEY:
      verbose_option("XNEE_NO_EXPOSE_KEY");
      fprintf (stderr, "\no-expose' is an obsoleted option\n");
      ret = XNEE_SYNTAX_ERROR;
      *args_used = 1;
      break;


      /*    case XNEE_USE_SYNC_KEY:
	    verbose_option("XNEE_USE_SYNC_KEY");
	    fprintf (stderr, "\invalid key' is an obsoleted option\n");
	    ret = XNEE_SYNTAX_ERROR;
	    *args_used = 1;
	    break;
      */

    default:
      ret = XNEE_SYNTAX_ERROR;
      break;
    }



  return ret;
}
