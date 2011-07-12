/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 * Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 
 *               2005, 2006, 2007, 2009, 2010, 2011
 *               Henrik Sandklef 
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
#include "libxnee/feedback.h"
#include "libxnee/xnee_utils.h"
#include "libxnee/xnee_plugin.h"
#include "libxnee/xnee_display.h"
#include "libxnee/xnee_window.h"





/*************************************************************
 * internal functions *
 *************************************************************/
static int
xnee_expression_handle_replay(xnee_data *xd, 
			      char *tmp,
			      xnee_intercept_data * xindata);

static int
xnee_expression_handle_settings(xnee_data *xd, char *tmp, int synt_mode);

static int
xnee_expression_handle_comment(xnee_data *xd, char *tmp);

static int
xnee_expression_handle_action(xnee_data *xd, char *tmp);

static int
xnee_expression_handle_mark(xnee_data *xd, char *tmp);

static int
xnee_expression_handle_prim(xnee_data *xd, char *tmp, xnee_intercept_data * xindata);

static int
xnee_expression_handle_projinfo(xnee_data *xd, char *tmp);

static int
xnee_expression_handle_newwindow(xnee_data *xd, char *tmp);

static int
xnee_is_replayable(xnee_data *xd, char *tmp);


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

  /* Is it a replayable script primitive */
  do_continue = 
    xnee_expression_handle_prim(xd, 
				tmp,
				xindata);
  if (do_continue==XNEE_PRIMITIVE_DATA) { return (XNEE_PRIMITIVE_DATA); }

  /* Is it a meta string */
  do_continue = xnee_expression_handle_comment(xd, tmp);
  if (do_continue==XNEE_META_DATA) { return (do_continue); }
  
  /* Is it a setting expression */
  do_continue = xnee_expression_handle_settings(xd, tmp, XNEE_CLI_SYNTAX);
  if (do_continue==XNEE_SETTINGS_DATA) { return (do_continue); }
  
  /* Is it an action string */
  do_continue = xnee_expression_handle_action(xd, tmp);
  if (do_continue==XNEE_ACTION_DATA) { return (do_continue); }

  /* Is it a mark string */
  do_continue = xnee_expression_handle_mark(xd, tmp);
  if (do_continue==XNEE_MARK_DATA) { return (do_continue); }
  
   /* Is it a new window */
  do_continue = xnee_expression_handle_newwindow(xd, tmp);
  if (do_continue==XNEE_NEW_WINDOW_DATA) { return (do_continue); }
  

  /* is it? ..., continue anyway */
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
  
  /* Is it a replayable data */
  do_continue = xnee_is_replayable(xd, tmp);
  if (do_continue) { return (XNEE_REPLAY_DATA); }

  /* Is it a script data */
  do_continue = xnee_is_script(xd, tmp);
  if (do_continue) { return (XNEE_PRIMITIVE_DATA); }

  /* Is it a meta string */
  do_continue = xnee_expression_handle_comment(xd, tmp);
  if (do_continue==XNEE_META_DATA) { return (do_continue); }

  /* Is it a setting expression */
  do_continue = xnee_expression_handle_settings(xd, tmp, XNEE_XNS_SYNTAX);
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
#ifdef XNEE_XINPUT_SUPPORT
  unsigned long tmp_time;  
#endif /* XNEE_XINPUT_SUPPORT */

  if (!strncmp("0",tmp,1))  /* EVENT */
    { 
      int last_type;
      int last_x;
      int last_y;

#ifdef XNEE_XINPUT_SUPPORT
      tmp_time = xindata->newtime;
#else
      xindata->oldtime = xindata->newtime ;
#endif /* XNEE_XINPUT_SUPPORT */
      last_type=xindata->u.event.type;
      last_x=xindata->u.event.x;
      last_y=xindata->u.event.y;

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
	  (void)xnee_print_error("Error in file %s: %s \n", xd->data_name, tmp);
	  ret = 0 ;
	}   
#ifdef XNEE_XINPUT_SUPPORT
      
      /* If the recorded sessions file has been recorded
	 with XI enabled, we get one extra Motion event
	 which will ¤% up the calc sleep funs: 
	 If so: discard it */
      if (xnee_is_screen_ok(xd, xindata->u.event.screen_nr))
	{
	  xindata->oldtime = tmp_time ;
	}
      else
	{
	  xindata->newtime = tmp_time ;
	}
#endif /* XNEE_XINPUT_SUPPORT */
      
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
	  (void)xnee_print_error("Error in file %s: %s \n", xd->data_name, tmp);
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
	  (void)xnee_print_error("Error in file %s: %s \n", xd->data_name, tmp);
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
	  (void)xnee_print_error("Error in file %s: %s \n", xd->data_name, tmp);
	  ret = 0;
	} 
    }
#ifdef XNEE_XINPUT_SUPPORT
  else if (!strncmp("6",tmp,1))    /* XInput Device (master) */
    {
      if ( ! xnee_is_forced_core_device_events(xd))
	{
	  xindata->oldtime = xindata->newtime ;
	  ret = sscanf(tmp, "%d,%d,%d,%d,%d,%d,%d,%lu,%d,%s",
		       &xindata->type, 
		       &xindata->u.xievent.type, 
		       &xindata->u.xievent.x,
		       &xindata->u.xievent.y, 
		       &xindata->u.xievent.button,
		       &xindata->u.xievent.keycode, 
		       &xindata->u.xievent.screen_nr,
		       &xindata->newtime,
		       &xindata->u.xievent.deviceid,
		       xindata->u.xievent.name);
	  if (ret < 10)
	    {
	      (void)xnee_print_error("Error in file %s: %s \n", xd->data_name, tmp);
	      ret = 0;
	    } 
	}
    }
  else if (!strncmp("7",tmp,1))  /* XInput Device (slave) */
    {
      if ( ! xnee_is_forced_core_device_events(xd))
	{
	  xindata->oldtime = xindata->newtime ;
	  ret = sscanf(tmp, "%d,%d,%d,%d,%d,%d,%d,%lu,%d,%s",
		       &xindata->type, 
		       &xindata->u.xievent.type, 
		       &xindata->u.xievent.x,
		       &xindata->u.xievent.y, 
		       &xindata->u.xievent.button,
		       &xindata->u.xievent.keycode, 
		       &xindata->u.xievent.screen_nr,
		       &xindata->newtime,
		       &xindata->u.xievent.deviceid,
		       xindata->u.xievent.name);
	  if (ret < 10)
	    {
	      (void)xnee_print_error("Error in file %s: %s \n", xd->data_name, tmp);
	      ret = 0;
	    } 
	}
    }  
#endif /* XNEE_XINPUT_SUPPORT */
  else 
    {
      ret=-1;
    }      
  if (ret>0)
    {
      ret = XNEE_REPLAY_DATA;
    }

  return ret;
}


static int
xnee_expression_handle_settings(xnee_data *xd, char *tmp, int synt_mode)
{
  #define RANGE_BUF_SIZE 100
  int ret=XNEE_SETTINGS_DATA;  
  char **ret_strptr;
  char *my_tmp;
  int args_used ;

  if (tmp==NULL)
    {
      xnee_verbose ((xd, "handling settings: NULL returning\n"));
      return XNEE_WRONG_PARAMS;
    }

  xnee_verbose ((xd, "handling settings: '%s'\n", tmp));


  if ( xnee_is_replayable(xd, tmp) == XNEE_REPLAY_DATA) 
    { 
      return (XNEE_REPLAY_DATA); 
    }

  my_tmp = tmp;

  ret_strptr = xnee_str2strptr(my_tmp, synt_mode);


  xnee_print_strptr(xd, ret_strptr);

  ret = xnee_parse_xns_option(xd, ret_strptr, &args_used) ;

  /*  option_key = xnee_option2key(xd, ret_strptr);
      xnee_free_strptr(ret_strptr);
      if (option_key==0)
      {
      ret = XNEE_OK;
      }
  */ 

  if (ret==XNEE_OK)
    {
      ret = XNEE_SETTINGS_DATA;
    }

  return ret;
}


static int
xnee_expression_handle_comment(xnee_data *xd, char *tmp)
{
  xnee_verbose ((xd, "handling comment: \"%s\"\n", tmp));
  if (!strncmp("#",tmp,1))  /* # META data */
    {
      xnee_verbose ((xd, "comment data read: %s\n", tmp));
      return XNEE_META_DATA;
    }
  return -1;
}

static int
xnee_expression_handle_action(xnee_data *xd, char *tmp)
{
/*   static int exec_counter = 0 ; */
  char *exec_prog ;
  int ret;

  xnee_verbose ((xd, "handling action: %s\n", tmp));

  if (strncmp(XNEE_EXEC_MARK,tmp,strlen(XNEE_EXEC_MARK))==0)
  {

    xnee_verbose ((xd, "** handling action: %s==%s\n", tmp, XNEE_EXEC_MARK));


    exec_prog = xnee_get_exec_prog(xd);
    if (exec_prog != NULL)
      {
	ret = system (exec_prog);
	return XNEE_ACTION_DATA;
      }

    exec_prog = getenv("XNEE_EXEC_PROGRAM");
    if (exec_prog != NULL)
      {
	ret = system (exec_prog);
	return XNEE_ACTION_DATA;
      }
  }
  return -1;
}

static int
xnee_expression_handle_mark(xnee_data *xd, char *tmp)
{
      xnee_verbose ((xd, "handling mark: %s\n", tmp));
      return -1;
}

static int
xnee_expression_handle_newwindow(xnee_data *xd, char *tmp)
{

  xnee_win_pos xwp;
  int ret;

  int x ;
  int y ;
  int override ;
  char buf[256];


  xnee_verbose ((xd, "---> xnee_expression_handle_newwindow: %s\n", tmp));


  if (strncmp(XNEE_NEW_WINDOW_MARK,tmp,strlen(XNEE_NEW_WINDOW_MARK))==0)
    {
      tmp = tmp+strlen(XNEE_NEW_WINDOW_MARK);
      
      sscanf (tmp,  ":%d,%d:%d,%d,%d,%d,%d,%d:%dx%d+%d+%d:%d,%d:%s\n", 
	      &xwp.x,
	      &xwp.y,
	      &xwp.event,
	      &xwp.window,
	      &xwp.parent,
	      &x,
	      &y,
	      &override,
	      &xwp.rel_x,
	      &xwp.rel_y,
	      &xwp.width,
	      &xwp.height,
	      &xwp.border_h,
	      &xwp.border_w,
	      &buf[0]);

      xwp.name=strdup(buf);

      ret = xnee_window_add_session(xd, &xwp);
      XNEE_RETURN_IF_ERR(ret);

      ret = xnee_window_try_move(xd);
      XNEE_RETURN_IF_ERR(ret);
      
      xnee_verbose ((xd, "<---  : xnee_expression_handle_newwindow\n"));

      return XNEE_NEW_WINDOW_DATA;
    }      

  xnee_verbose ((xd, "<--- xnee_expression_handle_newwindow: %s\n", tmp));
  return -1;
}


static char *
xnee_expr_get_proj_value (char *var_and_val)
{
  char *tmp;

  tmp = strstr(var_and_val, ":");
  if (tmp==NULL)
    {  
      return NULL;
    }
  tmp++;

  while ( (tmp!=NULL) && ( (*tmp==' ') || (*tmp=='\t') )) 
    {
      tmp++;
    }
  
  if (tmp==NULL)
    {
      return NULL;
    }

  if (tmp==NULL)
    {
      return NULL;
    }

  if (tmp[strlen(tmp)-1]=='\n')
    {
      tmp[strlen(tmp)-1]='\0';
    }

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
    {
      return XNEE_SYNTAX_ERROR;
    }

  xss->msecs =  0;  


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
	{
	  return XNEE_SYNTAX_ERROR;
	}

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
	{
	  return XNEE_SYNTAX_ERROR;
	}



      str++;
      /* skip leading blanks */
      while ( str[0] == ' ' ) str++ ;

      /* copy everything from str upto '=' into var */  
      len = strlen(str);
      strncpy (val, str, BUF_SIZE - strlen(val));
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
		{
		  val[0]=' ';
		}

	      xss->key = xnee_char2keycode (xd,val[0],&xss->kc);
	      if ( xss->kc.kc != 0 )
		{
		  xss->valid = 1 ; 
		}
	      else
		{
		  xss->valid = 0 ; 
		}
	    }
	  else 
	    {
	      xss->key = xnee_str2keycode (xd,val,&xss->kc); 
 	      xss->valid = 1; 
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
      /* msecs=12 etc */
      else if (strncmp(var,
		       XNEE_FAKE_MSEC_SLEEP,
		       strlen(XNEE_FAKE_MSEC_SLEEP))==0)
	{
	  valp = &val[0];
	  xss->valid = sscanf (valp, "%d", &xss->msecs);
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
xnee_expression_handle_prim(xnee_data *xd, char *str, xnee_intercept_data * xindata)
{
  int ret= 0 ;  
  char *prim_args;
  char buf[256];
  int   prim_len = 0 ; 
  xnee_script_s xss ;
  
  xindata->type         = 0;
  xindata->u.event.type = 0;

  xnee_verbose ((xd, "handling primitive: %s\n", str));

  memset(&xss, 0, sizeof(xnee_script_s));

  if (str == NULL)
    {
      return 0;
    }

  /* If NEW-WINDOW string, return */
  if (strncmp(str,
	      XNEE_NEW_WINDOW_MARK, 
	      strlen(XNEE_NEW_WINDOW_MARK)) == 0 )
    {
      return 0;
    }

  /* Skip the leading blanks */
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
	  xnee_fake_relative_motion_event (xd, xss.x, xss.y, xss.msecs);
	}
      else
	{
	  xnee_fake_motion_event (xd, 0, xss.x, xss.y, xss.msecs);
	}
      ret = XNEE_PRIMITIVE_DATA ;
    }
  else if (CHECK_EQUALITY(buf, XNEE_FAKE_BUTTON_PRESS))
    {
      xnee_fake_button_event (xd, xss.button, XNEE_PRESS, xss.msecs);
      ret = XNEE_PRIMITIVE_DATA ;
    }
  else if (CHECK_EQUALITY(buf, XNEE_FAKE_BUTTON_RELEASE))
    {
      xnee_fake_button_event (xd, xss.button, XNEE_RELEASE, xss.msecs);
      ret = XNEE_PRIMITIVE_DATA ;
    }
  else if (CHECK_EQUALITY(buf, XNEE_FAKE_BUTTON))
    {
      xnee_fake_button_event (xd, xss.button, XNEE_PRESS, xss.msecs);
      usleep (100);
      xnee_fake_button_event (xd, xss.button, XNEE_RELEASE, xss.msecs);
      ret = XNEE_PRIMITIVE_DATA ;
    }
  else if (CHECK_EQUALITY(buf, XNEE_FAKE_KEY_PRESS))
    {
      xnee_fake_key_mod_event (xd, &xss, XNEE_PRESS, xss.msecs);
      ret = XNEE_PRIMITIVE_DATA ;
    }
  else if (CHECK_EQUALITY(buf, XNEE_FAKE_KEY_RELEASE))
    {
      xnee_fake_key_mod_event (xd, &xss, XNEE_RELEASE, xss.msecs);
      ret = XNEE_PRIMITIVE_DATA ;
    }
  else if (CHECK_EQUALITY(buf, XNEE_FAKE_KEY))
    {
      xnee_fake_key_mod_event (xd, &xss, XNEE_PRESS,   xss.msecs); 
      xnee_fake_key_mod_event (xd, &xss, XNEE_RELEASE, xss.msecs); 
      ret = XNEE_PRIMITIVE_DATA ;
    }
  else
  {
     xnee_verbose((xd, "  else branch reached in xnee_expression_handle_prim\n"));  
  }


  return ret;
}


static int
xnee_is_replayable(xnee_data *xd, char *tmp)
{
  char first_c ; 

  first_c = tmp[0];

  xnee_verbose((xd, " xnee_is_replayable\n"));  

  if ( ( first_c >= '0' ) && ( first_c<='3' ) )
    {
      return 1;
    }
  return 0;
}


int
xnee_is_script(xnee_data *xd, char *tmp)
{

  xnee_verbose((xd, " xnee_is_replayable\n"));  

  if ( strncmp(tmp, "xnee-", strlen("xnee-")) == 0 )
    {
      return 1;
    }
  return 0;
}
