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

#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_resolution.h"
#include "libxnee/xnee_resource.h"
#include "libxnee/xnee_grab.h"
#include "libxnee/xnee_km.h"
#include "libxnee/xnee_range.h"
#include "libxnee/xnee_expr.h"
#include "libxnee/datastrings.h"


/*************************************************************
 * internal functions *
 *************************************************************/
static int
xnee_expression_handle_replay(xnee_data *xd, 
			      char *tmp,
			      xnee_intercept_data * xindata);

static int
xnee_expression_handle_settings(xnee_data *xd, char *tmp);

static int
xnee_expression_handle_meta(xnee_data *xd, char *tmp);

static int
xnee_expression_handle_action(xnee_data *xd, char *tmp);

static int
xnee_expression_handle_mark(xnee_data *xd, char *tmp);


/*************************************************************
 *    public api 
 *************************************************************/

int
xnee_expression_handle_session(xnee_data *xd, 
			       char *tmp,
			       xnee_intercept_data *xindata)
{
  int do_continue = 0;

  /* Do we have a string at all */
  if ( tmp == NULL)
    {
      /* if not, return -1 */
      return -1;
    }

  /* Is it a replayable event*/
  do_continue = 
    xnee_expression_handle_replay(xd, 
				  tmp,
				  xindata);
  if (do_continue==XNEE_REPLAY_DATA) { return (XNEE_REPLAY_DATA); }

  /* ok, we have some other data */
  do_continue = xnee_expression_handle_project(xd, 
					       tmp);
  return (do_continue);
}


int
xnee_expression_handle_project(xnee_data *xd, char *tmp)
{
  int do_continue = 0;
  int len ;

  /* Do we have a string at all */
  if ( tmp == NULL)
    {
      /* if not, return -1 */
      return -1;
    }

  len = strlen (tmp);

  /* OK the string passed the first tests */
  rem_begin_blanks (tmp, len);
  len=strlen(tmp);
  if (len==0)
    {
      return -1;
    }
  

  /* Is it a meta string */
  do_continue = xnee_expression_handle_meta(xd, tmp);
  if (do_continue==XNEE_OK) { return (do_continue); }
  
  /* Is it a setting expression*/
  do_continue = xnee_expression_handle_settings(xd, tmp);
  if (do_continue==XNEE_SETTINGS_DATA) { return (do_continue); }
  
  /* Is it an action string */
  do_continue = xnee_expression_handle_action(xd, tmp);
  if (do_continue==XNEE_ACTION_DATA) { return (do_continue); }
  
  /* Is it a mark string */
  do_continue = xnee_expression_handle_mark(xd, tmp);
  if (do_continue==XNEE_MARK_DATA) { return (do_continue); }
  
  return (XNEE_SYNTAX_ERROR);
}



/*************************************************************
 * internal functions *
 *************************************************************/
static int
xnee_expression_handle_replay(xnee_data *xd, 
			      char *tmp,
			      xnee_intercept_data * xindata)
{
  int ret = 0;

  if (!strncmp("0",tmp,1))  /* EVENT */
    {    
      xindata->oldtime = xindata->newtime ;
      ret = sscanf(tmp, "%d,%d,%d,%d,%d,%d,%d,%lu",
		      &xindata->type, 
		      &xindata->u.event.type, 
		      &xindata->u.event.x,
		      &xindata->u.event.y, 
		      &xindata->u.event.button,
		      &xindata->u.event.keycode, 
		      &xindata->u.event.screen_nr,
		      &xindata->newtime);
      if (ret < 8)    /* NUM ARGS */
	{
	  (void)xnee_print_error("Error in file %s \n", xd->data_name);
	  ret = 0 ;
	}   
    }   
  else if (!strncmp("1",tmp,1))  /* REQUEST */
    {
      xindata->oldtime = xindata->newtime ;
      ret = sscanf(tmp, "%d,%d,%lu",
		      &xindata->type, 
		      &xindata->u.request.type,
		      &xindata->newtime);
      if (ret < 3)
	{
	  (void)xnee_print_error("Error in file %s \n", xd->data_name);
	  ret = 0;
	}   
    }
  else if (!strncmp("2",tmp,1)) /* REPLY */
    {
      xindata->oldtime = xindata->newtime ;
      ret = sscanf(tmp, "%d,%d,%lu",
		      &xindata->type, 
		      &xindata->u.reply.type,
		      &xindata->newtime);
      if (ret < 3)
	{
	  (void)xnee_print_error("Error in file %s \n", xd->data_name);
	  ret = 0;
	}   
    }
  else if (!strncmp("3",tmp,1))  /* ERROR */
    {
      xindata->oldtime = xindata->newtime ;
      ret = sscanf(tmp, "%d,%d,%lu",
		      &xindata->type, 
		      &xindata->u.error.type,
		      &xindata->newtime);
      if (ret < 3)
	{
	  (void)xnee_print_error("Error in file %s \n", xd->data_name);
	  ret = 0;
	} 
    }  
  else 
    {
      ret=-1;
    }      
  if (ret>0)
    ret = XNEE_REPLAY_DATA;

  return ret;
}

static int
xnee_expression_handle_settings(xnee_data *xd, char *tmp)
{
  int ret=XNEE_SETTINGS_DATA;  
  char *range_tmp;
  char *range;
  char range_buf[100];
  int len=0;
  
  len=strlen(tmp);


#define XNEE_CHECK_SESS_EXPR(arg,str) ( (!strncmp(arg,str,strlen(arg))) &&  ( strlen(arg)==strlen(str) ))
  
  
  /* OK the string passed the first tests */
  range_tmp=strstr (tmp, " ");

  if (range_tmp!=NULL) 
    {
      strcpy(range_buf, range_tmp);

      len=strlen(range_buf);

      rem_begin_blanks (range_buf, len);

      len=strlen(range_buf);
      do 
	{
	  range_buf[len-1]='\0';
	} while ( 
		 (range_buf[len-1]=='\n')
		 || 
		 (range_buf[len-1]=='\r')
		 );
	   
	
      range = &range_buf[0];
      if (len==0)
	{
	  range = NULL;
	}

    }
  if (!strncmp(XNEE_DISPLAY,tmp,strlen(XNEE_DISPLAY)))
    {
      xnee_set_display_name (xd, range) ;
    }
  else if (!strncmp(XNEE_FIRST_LAST,tmp,strlen(XNEE_FIRST_LAST)))
    {
      if (strncmp(range,"0",1)==0)
	xnee_unset_first_last(xd);
      else
	xnee_set_first_last(xd);
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
      if ( xnee_set_rec_resolution (xd, range))
	{
	  xnee_verbose ((xd, "failed to set recorded resolution\n"));
	  xnee_close_down(xd);
	  exit(XNEE_BAD_RESOLUTION );
	}
      xnee_verbose ((xd, "recored resolution= %dx%d\n", 
		     xnee_get_rec_resolution_x(xd),
		     xnee_get_rec_resolution_y(xd)));
    }
  else if (!strncmp(XNEE_VERBOSE,tmp,strlen(XNEE_VERBOSE)))  
    {
      if (strncmp(range,"0",1)==0)
	xnee_set_verbose(xd);
      else
	xnee_unset_verbose(xd);
    }
  else if (!strncmp(XNEE_BUFFER_VERBOSE,tmp,strlen(XNEE_BUFFER_VERBOSE)))  
    {
      xd->buf_verbose = True;
    }
  else if (!strncmp(XNEE_LOOPS,tmp,strlen(XNEE_LOOPS))) 
    {
      xnee_set_events_max(xd, atoi(range));
    }
  else if (!strncmp(XNEE_LOOPS,tmp,strlen(XNEE_LOOPS))) 
    {
      xnee_set_events_max(xd, atoi(range));
    }
  else if (!strncmp(XNEE_DATA_MAX,tmp,strlen(XNEE_DATA_MAX))) 
    {
      xnee_set_data_max(xd, atoi(range));
    }
  else if (!strncmp(XNEE_EVENT_MAX,tmp,strlen(XNEE_EVENT_MAX))) 
    {
      xnee_set_events_max(xd, atoi(range));
    }
  else if (!strncmp(XNEE_TIME_MAX,tmp,strlen(XNEE_TIME_MAX))) 
    {
      xnee_set_time_max(xd, atoi(range));
    }
  else if (!strncmp(XNEE_STOP_KEY,tmp,strlen(XNEE_STOP_KEY)))
    {
      if ( (range==NULL) || (strlen(range)==0) )
	ret = XNEE_SYNTAX_ERROR;
      else
	{
	  xnee_grab_key (xd, XNEE_GRAB_STOP, range);
	}
    }
  else if (!strncmp(XNEE_PAUSE_KEY,tmp,strlen(XNEE_PAUSE_KEY)))
    {
      if ( (range==NULL) || (strlen(range)==0) )
	ret = XNEE_SYNTAX_ERROR;
      else
	{
	  xnee_grab_key (xd, XNEE_GRAB_PAUSE, range);
	}
    }
  else if (!strncmp(XNEE_RESUME_KEY,tmp,strlen(XNEE_RESUME_KEY)))
    {
      if ( (range==NULL) || (strlen(range)==0) )
	ret = XNEE_SYNTAX_ERROR;
      else
	{
      xnee_verbose((xd, "Resume key :\"%s\"\n", range ));
      xnee_grab_key (xd, XNEE_GRAB_RESUME, range);
      xnee_verbose((xd, "Resume key :\"%s\"\n", range ));
	}
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
  else if (!strncmp(XNEE_OUT_FILE,tmp,strlen(XNEE_OUT_FILE))) 
    {
      xnee_set_out_byname (xd, range);
    }
  else if (XNEE_CHECK_SESS_EXPR(XNEE_ERR_FILE,tmp))
    {
      printf ("ERROR %s\n", tmp); sleep (10);
      xnee_set_err_byname (xd, range);
    }
  else if (!strncmp(XNEE_DISTRIBUTE,tmp,strlen(XNEE_DISTRIBUTE)))
    {
      xnee_add_display_list ( xd, range);
    }
  else if (!strncmp(XNEE_DEVICE_EVENT_STR,tmp,strlen(XNEE_DEVICE_EVENT_STR)))
    {
      xnee_parse_range ( xd, XNEE_DEVICE_EVENT, range);
    }
  else if (!strncmp(XNEE_DELIVERED_EVENT_STR,
		    tmp,strlen(XNEE_DELIVERED_EVENT_STR)))
    {
      xnee_parse_range ( xd, XNEE_DELIVERED_EVENT, range);
    }
  else if (!strncmp(XNEE_ERROR_STR,tmp,strlen(XNEE_ERROR_STR))) 
    {
      xnee_parse_range ( xd, XNEE_ERROR, range);
    }
  else if (!strncmp(XNEE_REQUEST_STR,tmp,strlen(XNEE_REQUEST_STR)))
    {
      xnee_parse_range ( xd, XNEE_REQUEST, range);
    }
  else if (!strncmp(XNEE_REPLY_STR,tmp,strlen(XNEE_REPLY_STR)))  
    {
      xnee_parse_range ( xd, XNEE_REPLY, range);
    }
  else if (!strncmp(XNEE_EXT_REQ_MAJ_STR,tmp,strlen(XNEE_EXT_REQ_MAJ_STR)))
    {
      xnee_parse_range ( xd, XNEE_EXT_REQUEST_MAJOR, range);
    }
  else if (!strncmp(XNEE_EXT_REQ_MIN_STR,tmp,strlen(XNEE_EXT_REQ_MIN_STR)))
    {
      xnee_parse_range ( xd, XNEE_EXT_REQUEST_MINOR, range);
    }
  else if (!strncmp(XNEE_EXT_REP_MAJ_STR,tmp,strlen(XNEE_EXT_REP_MAJ_STR))) 
    {
      xnee_parse_range ( xd, XNEE_EXT_REPLY_MAJOR, range);
    }
  else if (!strncmp(XNEE_EXT_REP_MIN_STR,tmp,strlen(XNEE_EXT_REP_MIN_STR))) 
    {
      xnee_parse_range ( xd, XNEE_EXT_REPLY_MINOR, range);
    }
  else 
    {
      ret=-1;
    }      


  return ret;
}


static int
xnee_expression_handle_meta(xnee_data *xd, char *tmp)
{
  if (!strncmp("#",tmp,1))  /* # META data */
    {
      /*
	xnee_handle_resource_meta (xd, tmp); 
	if (xnee_handle_meta_data (xd, tmp+1)!=-1) 
	{ printf ("RETURN\n"); return 1; }
	else
	printf ("continue..with: %s\n", ++tmp);
      */
      xnee_verbose ((xd, "meta data read: %s\n", tmp));
      return XNEE_META_DATA;
    }
  return -1;
}

static int
xnee_expression_handle_action(xnee_data *xd, char *tmp)
{
      xnee_verbose ((xd, "handling action: %s\n", tmp));
      return -1;
}

static int
xnee_expression_handle_mark(xnee_data *xd, char *tmp)
{
      xnee_verbose ((xd, "handling action: %s\n", tmp));
      return -1;
}




