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


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <dlfcn.h>

#include <sys/utsname.h>

#include <X11/Xlibint.h>
#include <X11/Xproto.h>
#include <X11/Xlib.h>

#include <X11/extensions/record.h> 
#include <X11/extensions/XTest.h> 

#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/xnee_dl.h"
#include "libxnee/xnee_sem.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_resolution.h"
#include "libxnee/xnee_grab.h"
#include "libxnee/xnee_km.h"
#include "libxnee/datastrings.h"


/**************************************************************
 *                                                            *
 * xnee_add_resource_syntax                                   *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_add_resource_syntax(xnee_data *xd, char *tmp)
{
  int ret=1;  
  char *range;
  int len=0;
  
  len=strlen(tmp);


  if ( tmp == NULL)
    {
      return -1;
    }
  rem_all_blanks (tmp, len);
  len=strlen(tmp);
  if (len==0)
    {
      return -1;
    }
  xnee_verbose((xd, "   adding (2) \"%s\"  \n", tmp));
  
  if (!strncmp("#",tmp,1))  /* # META data */
    {
      ; /* for now ..... do nothing */
    }
  else if (!strncmp(XNEE_DISPLAY,tmp,strlen(XNEE_DISPLAY)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_set_display_name (xd, range) ;
    }
  else if (!strncmp(XNEE_FIRST_LAST,tmp,strlen(XNEE_FIRST_LAST)))
    {
      xd->xnee_info->first_last = True;
    }
  else if (!strncmp(XNEE_NO_SYNC,tmp,strlen(XNEE_NO_SYNC)))
    {
      xd->sync = False;
    }
  else if (!strncmp(XNEE_FORCE_REPLAY,tmp,strlen(XNEE_FORCE_REPLAY)))
    {
      xd->force_replay = True;
    }
  else if (!strncmp(XNEE_ALL_CLIENTS,tmp,strlen(XNEE_ALL_CLIENTS)))
    {
      xd->all_clients = True;
    }
  else if (!strncmp(XNEE_DIMENSION,tmp,strlen(XNEE_DIMENSION)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      if ( xnee_set_rec_resolution (xd, range))
	{
	  xnee_verbose ((xd, "failed to set recorded resolution\n"));
	  xnee_close_down(xd);
	  exit(XNEE_BAD_RESOLTION );
	}
      xnee_verbose ((xd, "recored resolution= %dx%d\n", 
		     xnee_get_rec_resolution_x(xd),
		     xnee_get_rec_resolution_y(xd)));
      exit(0);
      
    }
  else if (!strncmp(XNEE_VERBOSE,tmp,strlen(XNEE_VERBOSE)))  
    {
      xd->verbose = True;
    }
  else if (!strncmp(XNEE_BUFFER_VERBOSE,tmp,strlen(XNEE_BUFFER_VERBOSE)))  
    {
      xd->buf_verbose = True;
    }
  else if (!strncmp(XNEE_LOOPS,tmp,strlen(XNEE_LOOPS))) 
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xd->xnee_info->loops_left = atoi(range);
    }
  else if (!strncmp(XNEE_STOP_KEY,tmp,strlen(XNEE_STOP_KEY)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_verbose((xd, "Stop key :\"%s\"\n", range ));
      xnee_grab_key (xd, XNEE_GRAB_STOP, range);
      xnee_verbose((xd, "Stop key :\"%s\"\n", range ));
    }
  else if (!strncmp(XNEE_PAUSE_KEY,tmp,strlen(XNEE_PAUSE_KEY)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_verbose((xd, "Pause key :\"%s\"\n", range ));
      xnee_grab_key (xd, XNEE_GRAB_PAUSE, range);
      xnee_verbose((xd, "Pause key :\"%s\"\n", range ));
    }
  else if (!strncmp(XNEE_RESUME_KEY,tmp,strlen(XNEE_RESUME_KEY)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_verbose((xd, "Resume key :\"%s\"\n", range ));
      xnee_grab_key (xd, XNEE_GRAB_RESUME, range);
      xnee_verbose((xd, "Resume key :\"%s\"\n", range ));
    }
  else if (!strncmp(XNEE_KEYBOARD,tmp,strlen(XNEE_KEYBOARD))) 
    {	 
      xnee_parse_range (xd, XNEE_DEVICE_EVENT, 
			"KeyPress-KeyRelease");
      
    }
  else if (!strncmp(XNEE_MOUSE,tmp,strlen(XNEE_MOUSE)))
    {
      xnee_parse_range (xd, XNEE_DEVICE_EVENT, 
			"ButtonPress-MotionNotify");
    }
  else if (!strncmp(XNEE_OUT_FILE,tmp,3)) 
    {
      xnee_verbose((xd, "file \n" ));
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_verbose((xd, "file \"%s\"\n", range));
      xnee_set_out_byname (xd, range);
    }
  else if (!strncmp(XNEE_ERR_FILE,tmp,8))  
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_set_err_byname (xd, range);
    }
  else if (!strncmp(XNEE_DISTRIBUTE,tmp,strlen(XNEE_DISTRIBUTE)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_add_display_list ( xd, range);
    }
  else if (!strncmp(XNEE_DEVICE_EVENT_STR,tmp,strlen(XNEE_DEVICE_EVENT_STR)))
    {
      range=strstr (tmp, ":");
      range ++ ;
      xnee_parse_range ( xd, XNEE_DEVICE_EVENT, range);
    }
  else if (!strncmp(XNEE_DELIVERED_EVENT_STR,
		    tmp,strlen(XNEE_DELIVERED_EVENT_STR)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_DELIVERED_EVENT, range);
    }
  else if (!strncmp(XNEE_ERROR_STR,tmp,strlen(XNEE_ERROR_STR))) 
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_ERROR, range);
    }
  else if (!strncmp(XNEE_REQUEST_STR,tmp,strlen(XNEE_REQUEST_STR)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_REQUEST, range);
    }
  else if (!strncmp(XNEE_REPLY_STR,tmp,strlen(XNEE_REPLY_STR)))  
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_REPLY, range);
    }
  else if (!strncmp(XNEE_EXT_REQ_MAJ_STR,tmp,strlen(XNEE_EXT_REQ_MAJ_STR)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_EXT_REQUEST_MAJOR, range);
    }
  else if (!strncmp(XNEE_EXT_REQ_MIN_STR,tmp,strlen(XNEE_EXT_REQ_MIN_STR)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_EXT_REQUEST_MINOR, range);
    }
  else if (!strncmp(XNEE_EXT_REP_MAJ_STR,tmp,strlen(XNEE_EXT_REP_MAJ_STR))) 
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_EXT_REPLY_MAJOR, range);
    }
  else if (!strncmp(XNEE_EXT_REP_MIN_STR,tmp,strlen(XNEE_EXT_REP_MIN_STR))) 
    {
      range=strstr (tmp, ":");
      range ++ ;
      xnee_parse_range ( xd, XNEE_EXT_REPLY_MINOR, range);
    }
  /* 
   * TO KEEP COMPATIBILITY WITH OLD 
   * - SCRIPTS USING XNEE
   * - XNEE PLUGINS
   * THE OLD SYNTAX IS ALLOWED 
   * ... UNTIL ???  
   */
  /* START OF OBSOLETE PARSING */
  else if (!strncmp(XNEE_OBSOLETE_FIRST_LAST,tmp,strlen(XNEE_OBSOLETE_FIRST_LAST)))
    {
      xd->xnee_info->first_last = True;
    }
  else if (!strncmp(XNEE_OBSOLETE_NO_SYNC,tmp,strlen(XNEE_OBSOLETE_NO_SYNC)))  
    {
      xd->sync = False;
    }
  else if (!strncmp(XNEE_OBSOLETE_ALL_CLIENTS,tmp,strlen(XNEE_OBSOLETE_ALL_CLIENTS)))
    {
      xd->all_clients = True;
    }
  else if (!strncmp(XNEE_OBSOLETE_STOP_KEY,tmp,strlen(XNEE_OBSOLETE_STOP_KEY)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_verbose((xd, "==================== Stop key :\"%s\"\n", range ));
      xnee_grab_key (xd, XNEE_GRAB_STOP, range);
      xnee_verbose((xd, "==================== Stop key :\"%s\"\n", range ));
    }
  else if (!strncmp(XNEE_OBSOLETE_ERROR_STR,tmp,strlen(XNEE_OBSOLETE_ERROR_STR)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      if ( xnee_use_plugin(xd, range) != 0 )
	{
	xnee_print_error ("Unable to open plugin file (%s)\n", range);
	}
    }
  else if (!strncmp(XNEE_OBSOLETE_DEVICE_EVENT_STR,tmp,
		    strlen(XNEE_OBSOLETE_DEVICE_EVENT_STR)))
    {
      range=strstr (tmp, ":");
      range ++ ;
      xnee_parse_range ( xd, XNEE_DEVICE_EVENT, range);
    }
  else if (!strncmp(XNEE_OBSOLETE_DELIVERED_EVENT_STR,tmp,
		    strlen(XNEE_OBSOLETE_DELIVERED_EVENT_STR))) 
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_DELIVERED_EVENT, range);
    }
  else if (!strncmp(XNEE_OBSOLETE_ERROR_STR,tmp,strlen(XNEE_OBSOLETE_ERROR_STR)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_ERROR, range);
    }
  else if (!strncmp(XNEE_OBSOLETE_REQUEST_STR,tmp,strlen(XNEE_OBSOLETE_REQUEST_STR)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_REQUEST, range);
    }
  else if (!strncmp(XNEE_OBSOLETE_REPLY_STR,tmp,strlen(XNEE_OBSOLETE_REPLY_STR))) 
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_REPLY, range);
    }
  else if (!strncmp(XNEE_OBSOLETE_EXT_REQ_MAJ_STR,tmp,
		    strlen(XNEE_OBSOLETE_EXT_REQ_MAJ_STR)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_EXT_REQUEST_MAJOR, range);
    }
  else if (!strncmp(XNEE_OBSOLETE_EXT_REQ_MIN_STR,tmp,
		    strlen(XNEE_OBSOLETE_EXT_REQ_MIN_STR)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_EXT_REQUEST_MINOR, range);
    }
  else if (!strncmp(XNEE_OBSOLETE_EXT_REP_MAJ_STR,tmp,
		    strlen(XNEE_OBSOLETE_EXT_REP_MAJ_STR))) 
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_EXT_REPLY_MAJOR, range);
    }
  else if (!strncmp(XNEE_OBSOLETE_EXT_REP_MIN_STR,tmp,
		    strlen(XNEE_OBSOLETE_EXT_REP_MIN_STR))) 
    {
      range=strstr (tmp, ":");
      range ++ ;
      xnee_parse_range ( xd, XNEE_EXT_REPLY_MINOR, range);
    }
  /* END OF OBSOLETE PARSING */
  else 
    {
      xnee_verbose((xd,"Corrupt line: \"%s\"\n", tmp)); 
      ret=0;
    }      
  return ret;
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

  char tmp[256] ;
  int read_more  = 1 ;
  
  strcpy(tmp,"");


  while (read_more!=0)
    {
      if ( fgets(tmp, 256, xd->rc_file) == NULL)
	return XNEE_OK;
      /*
       * Hey, I know we'll keep the char array....
       * as long as we need... next call aint deep 
       */
      xnee_verbose((xd,"  adding : \"%s\"\n", tmp));
      read_more=xnee_add_resource_syntax(xd, tmp);
    }
  return read_more;
}



