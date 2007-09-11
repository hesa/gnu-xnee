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
#include "libxnee/xnee_error.h"
#include "libxnee/xnee_alloc.h"

#ifdef HAVE_STDARG_H
#include <stdarg.h>
#else
#include <varargs.h> 
#endif



static char *latest_error = NULL ;

char error_descr_solut[128][256] = 
  {
    /* XNEE_OK */            
    "OK", 
    "No need to do anything",

    /* XNEE_MEMORY_FAULT */   
    "Memory failure",
    "This is probably because you are out of memory",

    /* XNEE_FILE_NOT_FOUND */ 
    "Xnee can't find or create the file you are lookig for ",
    "Check the path and the permissions",

    /* XNEE_TIMED_OUT */      
    "Timeout expired",
    "The synchronisation timeout expired",

    /* XNEE_USER_INTR */      
    "User interrupt",
    "This is an internal 'signal'",

    /* XNEE_SYNCH_FAULT */    
    "Synchronisation filure",
    "For more information on this error, please read the manual(s)",

    /* XNEE_WRONG_PARAMS */   
    "Wrong paramters",
    "You have given invalid input to Xnee ",

    /* XNEE_NO_REC_EXT */     
    "Record extension missing on the display. Please add appropriate option to your X Server configuration",
    "Enable the RECORD extension. Read the manual on how to do this",

    /* XNEE_NO_TEST_EXT */    
    "XTest extension missing on the display",
    "Enable the XTest extension. Read the mnual on how to do this",

    /* XNEE_NO_PROT_CHOOSEN */
    "No protocol choosen",
    "Make sure you have set the Xnee application to record some data",

    /* XNEE_NOT_OPEN_DISPLAY */
    "Could not open the display",
    "Make sure you have given a correct display name and that you "
    "have permissions to open it",

    /* XNEE_AMBIGOUS_CMD */   
    "Ambigious command",
    "Make sure that the command/actions set aren't ambigous",

    /* XNEE_OUT_OF_SYNC */    
    "Can't synchronise any longer",
    "Read the synchronisation part of the Xnee manual",

    /* XNEE_NOT_SYNCING */    
    "Xnee is not using synchronisation",
    "internal 'signal'",

    /* XNEE_NO_PLUGIN_FILE */ 
    "Xnee can't find the plugin file",
    "Make sure that the path and permission are correct",

    /* XNEE_PLUGIN_FILE_ERROR */
    "Xnee can't use the plugin file",
    "Make sure that the plugin file is valid",

    /* XNEE_NO_PROJECT_FILE */
    "Xnee can't find the project file specified",
    "Make sure that the path and permissions are valid",

    /* XNEE_NO_MAIN_DATA */   
    "xnee_data structure missing",
    "This is a major error, please report the error "
    "with all the information you have: project file, session file, comand"
    "line options etc",

    /* XNEE_NO_RECORD_DATA */   
    "record_setup structure missing",
    "This is a major error, please report the error "
    "with all the information you have: project file, session file, comand"
    "line options etc",

    /* XNEE_NO_REPLAY_DATA */   
    "record_replay structure missing",
    "This is a major error, please report the error "
    "with all the information you have: project file, session file, comand"
    "line options etc",

    /* XNEE_SYNTAX_ERROR */    
    "Xnee syntax error",
    "Make sure all Xnee expressions are valid",

    /* XNEE_UNKNOWN_GRAB_MODE */
    "Requested grab mode is unknown",
    "Internal error, please report it",

    /* XNEE_NO_GRAB_DATA */   
    "No grab data",
    "Not used anymore. If this is reported to you, "
    "report it to the Xnee project",

    /* XNEE_GRAB_DATA */      
    "Grab data found",
    "Internal state, please report this as an error if you receive it",

    /* XNEE_BAD_GRAB_DATA */  
    "Bad grab data",
    "You have requested to grab the same key more than once or one already grabbed",

    /* XNEE_BAD_SESSION_FILE */   
    "Bad session file",
    "Make sure that the path and permission to the session file are correct",

    /* XNEE_BAD_SPEED */      
    "Invalid replay speed",
    "Make sure that the replay speed is valid ",

    /* XNEE_BAD_RESOLUTION */ 
    "Invalid screen resolution",
    "Make sure that the resolution is valid ",

    /* XNEE_BAD_THRESHOLD */  
    "Invalid threshold(s)",
    "Make sure that the threshold(s) is/are valid ",

    /* XNEE_BAD_CONTEXT */
    "Bad context",
    "Internal state, report if you receive this",

    /* XNEE_BLANK_LINE */
    "Blank line",
    "",

    /* XNEE_XOSD_FAILURE */
    "xosd failure",
    "Could not load xosd library",

    /* XNEE_FEEDBACK_FAILURE */
    "Feedback failure",
    "Could not give feedback",

    /* XNEE_MODE_NOT_SET */
    "No mode set",
    "You have to specify a mode (record/replay/retype)",

    /* XNEE_GRAB_MEM_FAILURE */
    "Grab memory failure",
    "The memory used to save lines during grabs is exceeded. Report it",

    /* XNEE_RECORD_FAILURE */
    "Record memory failure",
    "Xnee failed due to bad data received from RECORD extension",

    /* XNEE_DATE_FAILURE */
    "Date failure",
    "Xnee failed to get or handle the date from the OS",

    /* XNEE_SCREEN_MISSING */
    "Screen nr could not be retrieved",
    "Xnee could not find the screen of the recorded event. Report the problem",

    /* XNEE_RANGE_FAILURE */
    "Fatal error while adding data to range of recorded data",
    "Xnee could not add data to record. Report the problem",

    /* XNEE_BAD_OFFSET */
    "Fatal error while setting replay offset",
    "Xnee could not adjust replay offsets. Report the problem",

    /* XNEE_WINDOW_POS_ADJ_ERROR */
    "Fatal error while adjusting window position",
    "Xnee could not adjust window position. Report the problem",

    /* XNEE_MISSING_ARG */
    "Missing arguments when trying to parse",
    "Xnee misses one or more arguments. Make sure your command line or file is correct",

    /* XNEE_OK_LEAVE */            
    "OK, Xnee will quit as soon as possible", 
    "No need to do anything",

    /* XNEE_GRAB_CONFUSION */            
    "Xnee have reached a point where it is confused over the grabs done",
    "Make sure you do not grab something already grabbed. If you are sure about this, report the problem (include recorded session file and verbose printout file)",

    /* XNEE_PROJECT_SYNTAX_ERROR */            
    "Syntax error in Xnee project file",
    "Check the file",

   /* XNEE_LAST_ERROR */
    "Last error",
    "Internal state, report if you receive this",

  };


const char *
xnee_get_err_description (int error)
{
  if ( (error<XNEE_OK) || (error>XNEE_LAST_ERROR))
    {
      return NULL;
    }
  return error_descr_solut[error*2];
}

const char *
xnee_get_err_solution (int error)
{
  if ( (error<XNEE_OK) || (error>XNEE_LAST_ERROR))
    {
      return NULL;
    }
  return error_descr_solut[error*2+1];
}


char *
xnee_get_err_string (void)
{
  return latest_error;
}

void
xnee_free_err_name (void)
{
  XNEE_FREE_IF_NOT_NULL(latest_error);
  return ;
}


#ifdef HAVE_STDARG_H
char *
xnee_set_err_string (char *new_error, ...)
{
#define ERROR_STRING_SIZE 100
  va_list ap;

  char buf[ERROR_STRING_SIZE];
  va_start(ap, new_error);

  vsnprintf (buf,ERROR_STRING_SIZE,
	     new_error, ap );
  latest_error = strdup(new_error);
  return latest_error;
}
#else
char *
xnee_set_err_string (char *new_error, ...);
{
  char *fmt;
  char buf[100];
  va_list ap;

  va_start(ap);
  fmt = va_arg(argp, char *);

  vsnprintf ( buf, ERROR_STRING_SIZE,
	    new_error, ap );
  latest_error = strdup(new_error);
  return latest_error;
}
#endif
