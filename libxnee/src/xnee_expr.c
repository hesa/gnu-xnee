/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 * Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004 Henrik Sandklef 
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
#include "libxnee/xnee_threshold.h"
#include "libxnee/xnee_error.h"
#include "libxnee/xnee_keysym.h"
#include "libxnee/xnee_fake.h"






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
xnee_expression_handle_comment(xnee_data *xd, char *tmp);

static int
xnee_expression_handle_action(xnee_data *xd, char *tmp);

static int
xnee_expression_handle_mark(xnee_data *xd, char *tmp);

static int
xnee_expression_handle_prim(xnee_data *xd, char *tmp);

static int
xnee_expression_handle_projinfo(xnee_data *xd, char *tmp);

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

  /* Is it a replayable event*/
  do_continue = 
    xnee_expression_handle_prim(xd, 
				tmp);
  if (do_continue==XNEE_PRIMITIVE_DATA) { return (XNEE_PRIMITIVE_DATA); }




  /* Is it a meta string */
  do_continue = xnee_expression_handle_comment(xd, tmp);
  if (do_continue==XNEE_META_DATA) { return (do_continue); }
  
  /* Is it a setting expression*/
  do_continue = xnee_expression_handle_settings(xd, tmp);
  if (do_continue==XNEE_SETTINGS_DATA) { return (do_continue); }
  
  /* Is it an action string */
  do_continue = xnee_expression_handle_action(xd, tmp);
  if (do_continue==XNEE_ACTION_DATA) { return (do_continue); }

  /* Is it a mark string */
  do_continue = xnee_expression_handle_mark(xd, tmp);
  if (do_continue==XNEE_MARK_DATA) { return (do_continue); }
  
  /* Is it a mark string */
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
      /* if not, return -1 .... */
      return -1;
    }
  len = strlen (tmp);

  /* OK the string passed the first tests */
  rem_begin_blanks (tmp, len);
  len=strlen(tmp);
  if (len==0)
    {
      return XNEE_BLANK_LINE;
    }
  

  /* Is it a meta string */
  do_continue = xnee_expression_handle_comment(xd, tmp);
  if (do_continue==XNEE_META_DATA) { return (do_continue); }
  
  /* Is it a setting expression*/
  do_continue = xnee_expression_handle_settings(xd, tmp);
  if (do_continue==XNEE_SETTINGS_DATA) { return (do_continue); }
  
  /* Is it an action string */
  do_continue = xnee_expression_handle_action(xd, tmp);
  if (do_continue==XNEE_ACTION_DATA) { return (do_continue); }

  /* Is it a mark string */
  do_continue = xnee_expression_handle_mark(xd, tmp);
  if (do_continue==XNEE_MARK_DATA) { return (do_continue); }
  
  /* Is it a project information string */
  do_continue = xnee_expression_handle_projinfo(xd, tmp);
  if (do_continue==XNEE_PROJECT_INFORMATION_DATA) { return (do_continue); }
  

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
  char *range_tmp = NULL;
  char *range = NULL;
  char range_buf[100];
  int len=0;
  
  xnee_verbose ((xd, "handling settings: %s\n", tmp));
  len=strlen(tmp);

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
      ret = xnee_set_display_name (xd, range) ;
    }
  else if (!strncmp(XNEE_FIRST_LAST,tmp,strlen(XNEE_FIRST_LAST)))
    {
      if (xnee_check_true(range))
	ret = xnee_set_first_last(xd);
      else if (xnee_check_false(range))
	ret = xnee_unset_first_last(xd);
      else
	{
	  xnee_verbose((xd, "First last mode is invalid ... bailing out\n"));
	  xnee_set_err_string("Could not parse %s", tmp);
	  return XNEE_SYNTAX_ERROR;
	}
    }
  else if (!strncmp(XNEE_SYNC_MODE,tmp,strlen(XNEE_SYNC_MODE)))
    {
      if (xnee_check_true(range))
	  xd->sync = True;
      else if (xnee_check_false(range))
	  xd->sync = False;
      else
	{
	  xnee_verbose((xd, "Sync is invalid ... bailing out\n"));
	  xnee_set_err_string("Could not parse %s", tmp);
	  return XNEE_SYNTAX_ERROR;
	}
      ret = XNEE_OK;
    }
  else if (!strncmp(XNEE_HUMAN_PRINTOUT,tmp,strlen(XNEE_HUMAN_PRINTOUT)))
    {
      if (xnee_check_true(range))
	xnee_set_human_printout (xd);
      else if (xnee_check_false(range))
	xnee_set_xnee_printout (xd);
      else
	{
	  xnee_verbose((xd, "Printout mode is invalid ... bailing out\n"));
	  xnee_set_err_string("Could not parse %s", tmp);
	  return XNEE_SYNTAX_ERROR;
	}
      ret = XNEE_OK;
    }
  else if (!strncmp(XNEE_FORCE_REPLAY,tmp,strlen(XNEE_FORCE_REPLAY)))
    {
      if (xnee_check_true(range))
	xd->force_replay = True;
      else if (xnee_check_false(range))
	xd->force_replay = False;
      else
	{
	  xnee_verbose((xd, "Forced replay mode is invalid ... bailing out\n"));
	  xnee_set_err_string("Could not parse %s", tmp);
	  return XNEE_SYNTAX_ERROR;
	}
      ret = XNEE_OK;
    }
  else if (!strncmp(XNEE_ALL_CLIENTS,tmp,strlen(XNEE_ALL_CLIENTS)))
    {
       if ( range==NULL)
          xd->all_clients = True;
       else if (xnee_check_true(range))
          xd->all_clients = True;
       else if (xnee_check_false(range))
          xd->all_clients = False;
       else
       {
	  xnee_verbose((xd, "All clients mode is invalid ... bailing out\n"));
	  xnee_set_err_string("Could not parse %s", tmp);
	  return XNEE_SYNTAX_ERROR;
       }
      ret = XNEE_OK;
    }
  else if (!strncmp(XNEE_FUTURE_CLIENTS,tmp,strlen(XNEE_FUTURE_CLIENTS)))
    {
       if ( range==NULL)
          xd->all_clients = False;
       else if (xnee_check_true(range))
          xd->all_clients = False;
       else if (xnee_check_false(range))
          xd->all_clients = True;
       else
       {
	  xnee_verbose((xd, "Future clients mode is invalid ... bailing out\n"));
	  xnee_set_err_string("Could not parse %s", tmp);
	  return XNEE_SYNTAX_ERROR;
       }
      ret = XNEE_OK;
    }
  else if (!strncmp(XNEE_DIMENSION,tmp,strlen(XNEE_DIMENSION)))
    {
      ret = xnee_set_rec_resolution (xd, range); 
      if ( ret != XNEE_OK)
	{
	  ret = XNEE_BAD_RESOLUTION;
	}
    }
  else if (!strncmp(XNEE_VERBOSE,tmp,strlen(XNEE_VERBOSE)))  
    {
      if (xnee_check_true(range))
	ret = xnee_set_verbose(xd);
      else if (xnee_check_false(range))
	ret = xnee_unset_verbose(xd);
      else
	{
	  xnee_verbose((xd, "Verbose mode is invalid ... bailing out\n"));
	  xnee_set_err_string("Could not parse %s", tmp);
	  return XNEE_SYNTAX_ERROR;
	}
    }
  else if (!strncmp(XNEE_BUFFER_VERBOSE,tmp,strlen(XNEE_BUFFER_VERBOSE)))  
    {
      if (xnee_check_true(range))
	ret = xnee_set_buf_verbose(xd);
      else if (xnee_check_false(range))
	ret = xnee_unset_buf_verbose(xd);
      else
	{
	  xnee_verbose((xd, "Buffer Verbose mode is invalid ... bailing out\n"));
	  xnee_set_err_string("Could not parse %s", tmp);
	  return XNEE_SYNTAX_ERROR;
	}
    }
  else if (!strncmp(XNEE_DELAY_TIME,tmp,strlen(XNEE_DELAY_TIME))) 
    {
      ret = xnee_set_interval(xd, atoi(range));
    }
  else if (!strncmp(XNEE_ADJUST_RESOLUTION,tmp,strlen(XNEE_ADJUST_RESOLUTION))) 
    {
      if (xnee_check_true(range))
	ret = xnee_set_resolution_used (xd);
      else if (xnee_check_false(range))
	ret = xnee_unset_resolution_used (xd);
      else
	{
	  xnee_verbose((xd, "Resolution mode is invalid ... bailing out\n"));
	  xnee_set_err_string("Could not parse %s", tmp);
	  return XNEE_SYNTAX_ERROR;
	}
    }
  else if (!strncmp(XNEE_REPLAY_RESOLUTION,tmp,strlen(XNEE_REPLAY_RESOLUTION)))
    {
      ret = xnee_set_rep_resolution (xd, range);
    }
  else if (!strncmp(XNEE_RECORDED_RESOLUTION,tmp,strlen(XNEE_RECORDED_RESOLUTION)))
    {
      ret = xnee_set_rec_resolution (xd, range);
    }
  else if (!strncmp(XNEE_LOOPS,tmp,strlen(XNEE_LOOPS))) 
    {
      fprintf (stderr, "Obsolete option: '%s'\n", tmp);
      ret = xnee_set_events_max(xd, atoi(range));
    }
  else if (!strncmp(XNEE_DATA_MAX,tmp,strlen(XNEE_DATA_MAX))) 
    {
      ret = xnee_set_data_max(xd, atoi(range));
    }
  else if (!strncmp(XNEE_EVENT_MAX,tmp,strlen(XNEE_EVENT_MAX))) 
    {
      ret = xnee_set_events_max(xd, atoi(range));
    }
  else if (!strncmp(XNEE_TIME_MAX,tmp,strlen(XNEE_TIME_MAX))) 
    {
      ret = xnee_set_time_max(xd, atoi(range));
    }
  else if (!strncmp(XNEE_SPEED_PERCENT,tmp,strlen(XNEE_SPEED_PERCENT))) 
    {
      ret = xnee_set_replay_speed_str (xd, range);
    }
  else if (!strncmp(XNEE_STOP_KEY,tmp,strlen(XNEE_STOP_KEY)))
    {
      /* If there is a '0', we should not read it */
      if (!xnee_check_false(range))
	{
	  ret = xnee_set_key (xd, XNEE_GRAB_STOP, range);
	}
      else
	{
	  ret = XNEE_OK;
	}
    }
  else if (!strncmp(XNEE_PAUSE_KEY,tmp,strlen(XNEE_PAUSE_KEY)))
    {
      /* If there is a '0', we should not read it */
      if (!xnee_check_false(range))
	ret = xnee_set_key (xd, XNEE_GRAB_PAUSE, range);
      else
	{
	  ret = XNEE_OK;
	}
    }
  else if (!strncmp(XNEE_RESUME_KEY,tmp,strlen(XNEE_RESUME_KEY)))
    {
      if (!xnee_check_false(range))
	  ret = xnee_set_key (xd, XNEE_GRAB_RESUME, range);
      else
	{
	  ret = XNEE_OK;
	}
    }
  else if (!strncmp(XNEE_INSERT_KEY,tmp,strlen(XNEE_INSERT_KEY)))
    {
      if (!xnee_check_false(range))
	  ret = xnee_set_key (xd, XNEE_GRAB_INSERT, range);
      else
	{
	  ret = XNEE_OK;
	}
    }
  else if (!strncmp(XNEE_EXEC_KEY,tmp,strlen(XNEE_EXEC_KEY)))
    {
      if (!xnee_check_false(range))
	ret = xnee_set_key (xd, XNEE_GRAB_EXEC, range);
      else
	{
	  ret = XNEE_OK;
	}
    }
  else if (!strncmp(XNEE_EXEC_PROGRAM,tmp,strlen(XNEE_EXEC_PROGRAM)))
    {
      if ( (strncmp(XNEE_EXEC_NO_PROG, range, strlen(XNEE_EXEC_NO_PROG))==0))
	{
	  xnee_verbose((xd, "found no program to exec\n"));
	}
      else
	{
	  xnee_verbose((xd, "found program to exec: %s\n", range));
 	  ret = xnee_set_exec_prog (xd, range); 
	}
    }

  /* These are specific to xnee 
   *
   *
   *  Don't use
   *
   */
   else if (!strncmp(XNEE_STORE_MOUSE_POS,tmp,strlen(XNEE_STORE_MOUSE_POS)))  
     {	  
       xnee_set_store_mouse_pos(xd);
     } 
/*   else if (!strncmp(XNEE_KEYBOARD,tmp,strlen(XNEE_KEYBOARD)))  */
/*     {	  */
/*       ret = xnee_parse_range (xd, XNEE_DEVICE_EVENT,  */
/* 			"KeyPress-KeyRelease"); */
      
/*     } */
/*   else if (!strncmp(XNEE_MOUSE,tmp,strlen(XNEE_MOUSE))) */
/*     { */
/*       ret = xnee_parse_range (xd, XNEE_DEVICE_EVENT,  */
/* 			"ButtonPress-MotionNotify"); */
/*     } */
  else if (!strncmp(XNEE_PLUGIN,tmp,strlen(XNEE_PLUGIN))) 
    {
      /* If there is a '0', we should not read it */
      if (!xnee_check_false(range))
	ret = xnee_use_plugin (xd, range);
      else
	ret = XNEE_OK;
    }
  else if (!strncmp(XNEE_OUT_FILE,tmp,strlen(XNEE_OUT_FILE))) 
    {
      ret = xnee_set_out_name (xd, range);
    }
  else if (!strncmp(XNEE_ERR_FILE,tmp,strlen(XNEE_ERR_FILE))) 
    {
      ret = xnee_set_err_name (xd, range);
    }
  else if (!strncmp(XNEE_DISTRIBUTE,tmp,strlen(XNEE_DISTRIBUTE)))
    {
       if ( (range==NULL ) || (strlen(range)==0) )
       {
          ret = XNEE_OK ;
       }
       else
       {
          ret = xnee_add_display_list ( xd, range);
       }
    }
  else if (!strncmp(XNEE_DEVICE_EVENT_STR,tmp,strlen(XNEE_DEVICE_EVENT_STR)))
    {
      ret = xnee_parse_range ( xd, XNEE_DEVICE_EVENT, range);
    }
  else if (!strncmp(XNEE_DELIVERED_EVENT_STR,
		    tmp,strlen(XNEE_DELIVERED_EVENT_STR)))
    {
      ret = xnee_parse_range ( xd, XNEE_DELIVERED_EVENT, range);
    }
  else if (!strncmp(XNEE_ERROR_STR,tmp,strlen(XNEE_ERROR_STR))) 
    {
      ret = xnee_parse_range ( xd, XNEE_ERROR, range);
    }
  else if (!strncmp(XNEE_REQUEST_STR,tmp,strlen(XNEE_REQUEST_STR)))
    {
      ret = xnee_parse_range ( xd, XNEE_REQUEST, range);
    }
  else if (!strncmp(XNEE_REPLY_STR,tmp,strlen(XNEE_REPLY_STR)))  
    {
      ret = xnee_parse_range ( xd, XNEE_REPLY, range);
    }
  else if (!strncmp(XNEE_EXT_REQ_MAJ_STR,tmp,strlen(XNEE_EXT_REQ_MAJ_STR)))
    {
      ret = xnee_parse_range ( xd, XNEE_EXT_REQUEST_MAJOR, range);
    }
  else if (!strncmp(XNEE_EXT_REQ_MIN_STR,tmp,strlen(XNEE_EXT_REQ_MIN_STR)))
    {
      ret = xnee_parse_range ( xd, XNEE_EXT_REQUEST_MINOR, range);
    }
  else if (!strncmp(XNEE_EXT_REP_MAJ_STR,tmp,strlen(XNEE_EXT_REP_MAJ_STR))) 
    {
      ret = xnee_parse_range ( xd, XNEE_EXT_REPLY_MAJOR, range);
    }
  else if (!strncmp(XNEE_EXT_REP_MIN_STR,tmp,strlen(XNEE_EXT_REP_MIN_STR))) 
    {
      ret = xnee_parse_range ( xd, XNEE_EXT_REPLY_MINOR, range);
    }
  else if (!strncmp(XNEE_MAX_THRESHOLD,tmp,strlen(XNEE_MAX_THRESHOLD))) 
    {
      ret = xnee_set_max_threshold (xd,atoi(range));
    }
  else if (!strncmp(XNEE_MIN_THRESHOLD,tmp,strlen(XNEE_MIN_THRESHOLD))) 
    {
      ret = xnee_set_min_threshold (xd,atoi(range));
    }
  else if (!strncmp(XNEE_TOT_THRESHOLD,tmp,strlen(XNEE_TOT_THRESHOLD))) 
    {
      ret = xnee_set_tot_threshold (xd,atoi(range));
    }
  else 
    {
      ret=-1;
    }      
  if (ret==XNEE_OK)
    ret = XNEE_SETTINGS_DATA;

  return ret;
}


static int
xnee_expression_handle_comment(xnee_data *xd, char *tmp)
{
  int i ;
  int len ;

  len = strlen(tmp);

  xnee_verbose ((xd, "handling comment: %s\n", tmp));
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
  if (strncmp(XNEE_EXEC_MARK,tmp,strlen(XNEE_EXEC_MARK))==0)
  {
    char *exec_prog ;

    exec_prog = xnee_get_exec_prog(xd);
    if (exec_prog != NULL)
      {
	system (exec_prog);
	return XNEE_ACTION_DATA;
      }

    exec_prog = getenv("XNEE_EXEC_PROGRAM");
    if (exec_prog != NULL)
      {
	system (exec_prog);
	return XNEE_ACTION_DATA;
      }
    system("xterm -fg yellow -bg blue");
  }
  return -1;
}

static int
xnee_expression_handle_mark(xnee_data *xd, char *tmp)
{
      xnee_verbose ((xd, "handling mark: %s\n", tmp));
      return -1;
}



static char *
xnee_expr_get_proj_value (char *var_and_val)
{
  char *tmp;

  tmp = strstr(var_and_val, ":");
  if (tmp==NULL)
    return NULL;
  tmp++;

  while ( (tmp!=NULL) && ( (*tmp==' ') || (*tmp=='\t') )) 
    {
      tmp++;
    }
  
  if (tmp==NULL)
    return NULL;

  if (tmp==NULL)
    return NULL;

  if (tmp[strlen(tmp)-1]=='\n')
    tmp[strlen(tmp)-1]='\0';

  return tmp;
}



static int
xnee_expression_handle_projinfo(xnee_data *xd, char *tmp)
{
#define XNEE_IS_STR_SAME(a,b)			\
  ( strncmp(a,b, strlen(a))==0)


      xnee_verbose ((xd, "handling project: %s\n", tmp));

      /*
       *  creation 
       */
      if (XNEE_IS_STR_SAME(XNEE_RES_CREAT_PROGRAM,tmp))
	{
	  xnee_set_creat_program(xd, xnee_expr_get_proj_value(tmp));
	  return XNEE_PROJECT_INFORMATION_DATA;
	}
      else if (XNEE_IS_STR_SAME(XNEE_RES_CREAT_PROG_VER,tmp))
	{
	  xnee_set_creat_prog_vers(xd, xnee_expr_get_proj_value(tmp));
	  return XNEE_PROJECT_INFORMATION_DATA;
	}
      else if (XNEE_IS_STR_SAME(XNEE_RES_CREAT_DATE,tmp)  )
	{
	  xnee_set_creat_date(xd, xnee_expr_get_proj_value(tmp));
	  return XNEE_PROJECT_INFORMATION_DATA;
	}
      /*
       *  last change
       */
      else if (XNEE_IS_STR_SAME(XNEE_RES_LASTCHANGE_PROGRAM,tmp))
	{
	  xnee_set_last_program(xd, xnee_expr_get_proj_value(tmp));
	  return XNEE_PROJECT_INFORMATION_DATA;
	}
      else if (XNEE_IS_STR_SAME(XNEE_RES_LASTCHANGE_PROG_VER,tmp))
	{
	  xnee_set_last_prog_vers(xd, xnee_expr_get_proj_value(tmp));
	  return XNEE_PROJECT_INFORMATION_DATA;
	}
      else if (XNEE_IS_STR_SAME(XNEE_RES_LASTCHANGE_DATE,tmp)  )
	{
	  xnee_set_last_date(xd, xnee_expr_get_proj_value(tmp));
	  return XNEE_PROJECT_INFORMATION_DATA;
	}
      else if (strncmp("P",tmp,1)==0)  /* Action data */
	{
	  return XNEE_PROJECT_INFORMATION_DATA;
	}

      return XNEE_OK;
}



#define CHECK_EQUALITY(a,b) ( (a!=NULL) && (b!=NULL) && (strlen(a)==strlen(b)) && (strcmp(a,b)==0) )
#define BUF_SIZE 100

static int
xnee_expression_handle_prim_sub(xnee_data *xd, char *arg, xnee_script_s *xss)
{
  char var[BUF_SIZE];
  char val[BUF_SIZE];
  char *str = arg;
  char *tmpp = NULL;
  char *valp = NULL;
  int len       = 0 ;
  int i ; 

  if (arg == NULL)
    return XNEE_SYNTAX_ERROR;

  while (str!=NULL)
    {

      /* skip leading blanks */
      while ( str[0] == ' ' ) str++ ;
      

      /****************
       * Find varaiable 
       */

      /* find '=' */
      tmpp = strstr(str, "=");

      /* no '=' found, bail out */
      if (tmpp==NULL)
	return XNEE_SYNTAX_ERROR;

      /* copy everything from str upto '=' into var */  
      len = strlen(str)-strlen(tmpp);
      strncpy (var,str,len);
      var[len]='\0';
      /* remove trailing blanks ... */
      i=0;
      while ( (i<len) && ( var[i] != ' ' ) && ( var[i] != '\t' ) ) i++;
      var[i]='\0';
      
      /* variable is found. Great, move on to value */

      /****************
       * Find value 
       */
      str = strstr(str,"=");
      /* no '=' found, bail out */
      if (tmpp==NULL)
	return XNEE_SYNTAX_ERROR;
      str++;
      /* skip leading blanks */
      while ( str[0] == ' ' ) str++ ;

      /* copy everything from str upto '=' into var */  
      len = strlen(str);
      strcpy (val,str);
      val[len]='\0';
      /* remove trailing blanks ... */
      i=0;
      while ( (i<len) && ( val[i] != ' ' ) && ( val[i] != '\t' ) && ( val[i] != '\n' )  ) i++;
      val[i]='\0';

      /* x=12 etc */
      if (strncmp(var,XNEE_FAKE_X_ARG,strlen(XNEE_FAKE_X_ARG))==0)
	{
	  valp = &val[0];
	  xss->x_rel=0;
	  if ( (val[0]=='+') )
	    {
	      xss->x_rel=1;
	      valp++;
	    }
	  else if ( (val[0]=='-') )
	    {
	      xss->x_rel=1;
	    }
	  xss->valid = sscanf (valp, "%d", &xss->x);
	}
      /* y=12 etc */
      else if (strncmp(var,XNEE_FAKE_Y_ARG,strlen(XNEE_FAKE_Y_ARG))==0)
	{
	  valp = &val[0];
	  xss->y_rel=0;
	  if ( (val[0]=='+') )
	    {
	      xss->y_rel=1;
	      valp++;
	    }
	  else if ( (val[0]=='-') )
	    {
	      xss->y_rel=1;
	    }
	  xss->valid = sscanf (valp, "%d", &xss->y);
	}
      /* key=a key=shift etc */
      else if (strncmp(var,XNEE_FAKE_KEY_ARG,strlen(XNEE_FAKE_KEY_ARG))==0)
	{
 	  if (strlen(val)==1) 
	    {
	      valp = &val[0];
	      valp++;

	      if (val[0]=='\0')
		val[0]=' ';
	      
	      xss->valid = sscanf (val, "%d", &xss->key);
	      xss->key = xnee_char2keycode (xd,val[0],&xss->kc);
	    }
	  else
	    {
 	      xss->valid = 1; 
	      xss->key = xnee_str2keycode (xd,val,&xss->kc); 
	    }
	}
      /* button=12 etc */
      else if (strncmp(var,
		       XNEE_FAKE_BUTTON_ARG,
		       strlen(XNEE_FAKE_BUTTON_ARG))==0)
	{
	  valp = &val[0];
	  xss->valid = sscanf (valp, "%d", &xss->button);
	}
      else 
	{
	  fprintf (stderr, " keyword '%s' not yet handled\n", var);
	}

      /* skip leading blanks */
      
      if (!xss->valid)
	{
	  return XNEE_SYNTAX_ERROR ;
	}

      str=strstr(str," ");
    }
  return XNEE_OK;
}

static int
xnee_expression_handle_prim(xnee_data *xd, char *str)
{
  int ret= 0 ;  
  char *prim_args;
  char buf[32];
  int   prim_len = 0 ; 
  xnee_script_s xss ;
  
  xnee_verbose ((xd, "handling primitive: %s\n", str));
  prim_args = strstr(str," ");

  if (prim_args==NULL)
  {
    return 0;
  }

  prim_len  = strlen(str) - strlen(prim_args);
  strncpy(buf, str, prim_len);
  buf[prim_len]='\0';
  prim_args++;

  xnee_expression_handle_prim_sub(xd, prim_args, &xss);

  if (CHECK_EQUALITY(buf, XNEE_FAKE_MOTION))
    {
      if (xss.x_rel)
	{
	  xnee_fake_relative_motion_event (xd, xss.x, xss.y, 0);
	}
      else
	{
	  xnee_fake_motion_event (xd, 0, xss.x, xss.y, 0);
	}
      ret = XNEE_PRIMITIVE_DATA ;
    }
  else if (CHECK_EQUALITY(buf, XNEE_FAKE_BUTTON_PRESS))
    {
      xnee_fake_button_event (xd, xss.button, XNEE_PRESS, 0);
      ret = XNEE_PRIMITIVE_DATA ;
    }
  else if (CHECK_EQUALITY(buf, XNEE_FAKE_BUTTON_RELEASE))
    {
      xnee_fake_button_event (xd, xss.button, XNEE_RELEASE, 0);
      ret = XNEE_PRIMITIVE_DATA ;
    }
  else if (CHECK_EQUALITY(buf, XNEE_FAKE_BUTTON))
    {
      xnee_fake_button_event (xd, xss.button, XNEE_PRESS, 0);
      usleep (100);
      xnee_fake_button_event (xd, xss.button, XNEE_RELEASE, 0);
      ret = XNEE_PRIMITIVE_DATA ;
    }
  else if (CHECK_EQUALITY(buf, XNEE_FAKE_KEY_PRESS))
    {
      xnee_fake_key_mod_event (xd, &xss, XNEE_PRESS, 0);
      ret = XNEE_PRIMITIVE_DATA ;
    }
  else if (CHECK_EQUALITY(buf, XNEE_FAKE_KEY_RELEASE))
    {
      xnee_fake_key_mod_event (xd, &xss, XNEE_RELEASE, 0);
      ret = XNEE_PRIMITIVE_DATA ;
    }
  else if (CHECK_EQUALITY(buf, XNEE_FAKE_KEY))
    {
      xnee_fake_key_mod_event (xd, &xss, XNEE_PRESS, 0);
      xnee_fake_key_mod_event (xd, &xss, XNEE_RELEASE, 0);
      ret = XNEE_PRIMITIVE_DATA ;
    }
  else
  {
     xnee_verbose((xd, "  else branch reached in xnee_expression_handle_prim\n"));  
  }


  return ret;
}


