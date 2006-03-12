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


#include <stdio.h>
#include <unistd.h>
#include <string.h>
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

static int needs_init=1;
static xnee_options_t xnee_options_impl;
xnee_options_t *xnee_options;

xnee_options_t*
xnee_init_strings(xnee_data *xd);


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
 * xnee_add_resource_syntax                                   *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_add_resource_syntax_impl2(xnee_data *xd, char *tmp, int syntax_type)
{
  int ret=1;  
  char *range;
  int len=0;
  int val;

  len=strlen(tmp);
  
  if ( tmp == NULL)
    {
      return -1;
    }

  if (!strncmp("#",tmp,1))  /* # META data */
    {
      /*       xnee_handle_resource_meta (xd, tmp); */
      if (xnee_handle_meta_data (xd, tmp+1)!=-1) 
	{ 
	  xnee_verbose((xd, "  xnee_handle_meta_data: handling #   return 1\n"));
	}
      else
	{
	  xnee_verbose((xd, "  xnee_handle_meta_data: handling #\n"));
	}
    }
  if (syntax_type==XNEE_CLI_SYNTAX)
    {
      while (tmp[0]=='-')
	{
	  tmp++;
	}
    }

  rem_all_blanks (tmp, len);
  len=strlen(tmp);
  if (len==0)
    {
      return -1;
    }
  
  if (!strncmp(XNEE_DISPLAY,tmp,strlen(XNEE_DISPLAY)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_set_display_name (xd, range) ;
    }
  else if (!strncmp(XNEE_FIRST_LAST,tmp,strlen(XNEE_FIRST_LAST)))
    {
      xd->xnee_info.first_last = True;
    }
  else if (!strncmp(XNEE_NO_SYNC_MODE,tmp,strlen(XNEE_NO_SYNC_MODE)))
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
	  exit(XNEE_BAD_RESOLUTION );
	}
      xnee_verbose ((xd, "recored resolution= %dx%d\n", 
		     xnee_get_rec_resolution_x(xd),
		     xnee_get_rec_resolution_y(xd)));
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
      xnee_set_events_max(xd, atoi(range));
    }
  else if (!strncmp(XNEE_LOOPS,tmp,strlen(XNEE_LOOPS))) 
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_set_events_max(xd, atoi(range));
    }
  else if (!strncmp(XNEE_DATA_MAX,tmp,strlen(XNEE_DATA_MAX))) 
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_set_data_max(xd, atoi(range));
    }
  else if (!strncmp(XNEE_EVENT_MAX,tmp,strlen(XNEE_EVENT_MAX))) 
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_set_events_max(xd, atoi(range));
    }
  else if (!strncmp(XNEE_TIME_MAX,tmp,strlen(XNEE_TIME_MAX))) 
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_set_time_max(xd, atoi(range));
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
  else if (!strncmp(XNEE_STORE_MOUSE_POS,tmp,strlen(XNEE_STORE_MOUSE_POS)))
    {
      xnee_set_store_mouse_pos (xd);
    }
  else if (!strncmp(XNEE_OUT_FILE,tmp,3)) 
    {
      xnee_verbose((xd, "file \n" ));
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_verbose((xd, "file \"%s\"\n", range));
      xnee_set_out_name (xd, range);
    }
  else if (!strncmp(XNEE_ERR_FILE,tmp,8))  
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_set_err_name (xd, range);
    }
  else if (!strncmp(XNEE_DISTRIBUTE,tmp,strlen(XNEE_DISTRIBUTE)))
    {
      range=strstr (tmp, " ");
      range += 1 ;
      if (strlen(range)>0)
	{
	  xnee_add_display_list ( xd, range);
	}
    }
  else if (!strncmp(XNEE_RECALL_WINDOW_POS,tmp,strlen(XNEE_RECALL_WINDOW_POS)))
    {
      range=strstr (tmp, " ");
      range += 1 ;
      if ( sscanf (range, "%d", &val) == 1)
	{
	  if (strlen(range)>0)
	    {
	      xnee_set_new_window_pos_value ( xd, range);
	    }
	}
      else
	{
	  return XNEE_SYNTAX_ERROR;
	}
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
  else 
    {
      xnee_verbose((xd,"Corrupt line: \"%s\"\n", tmp)); 
      ret = XNEE_SYNTAX_ERROR;
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

#define TMP_BUF_SIZE 256
  static char tmp[TMP_BUF_SIZE] ;
  int read_more  = 1 ;
  
  strncpy(tmp,"",TMP_BUF_SIZE);
  

  while (read_more!=0)
    {
      if ( fgets(tmp, 256, xd->rc_file) == NULL)
	return XNEE_OK;
      /*
       * Hey, I __know__ we'll keep the char array....
       * as long as we need... 
       */
      /*       read_more=xnee_add_resource_syntax(xd, tmp); */
      read_more=xnee_expression_handle_project(xd, tmp); 

      xnee_verbose((xd,"  adding : \"%s\" \t-----------------returned %d\n", 
		    tmp, read_more));
      if (read_more==XNEE_SYNTAX_ERROR)
	return read_more;

    }
  return read_more;
}


char *
xnee_get_project_name(xnee_data *xd){
  if (xd->xrm.project_name!=NULL)
    return xd->xrm.project_name;
  else
    return "none";
}

char *
xnee_get_project_descr(xnee_data *xd){
  if (xd->xrm.project_descr!=NULL)
    return xd->xrm.project_descr;
  else
    return "none";
}

char *
xnee_get_creat_date(xnee_data *xd)
{
  time_t rawtime;
  struct tm * timeinfo;
#define XNEE_DATE_BUF_SIZE 100
  static char buf[XNEE_DATE_BUF_SIZE];

  if (xd->xrm.creat_date!=NULL)  
    {
      return xd->xrm.creat_date;
    }
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  snprintf(buf, XNEE_DATE_BUF_SIZE,
	   "%.4d-%.2d-%.2d",
	   timeinfo->tm_year + 1900 , 
	   timeinfo->tm_mon + 1 , 
	   timeinfo->tm_mday  );
  
  return buf;
}

char *
xnee_get_creat_program(xnee_data *xd)
{
  if (xd->xrm.creat_prog!=NULL)  
    return xd->xrm.creat_prog;
  else
    return xnee_get_program_name(xd);
}

char *
xnee_get_creat_prog_vers(xnee_data *xd)
{
  if (xd->xrm.creat_prog_vers!=NULL)  
    return xd->xrm.creat_prog_vers;
  else
    return VERSION;
}

char *
xnee_get_last_date(xnee_data *xd)
{
  if (xd->xrm.last_date!=NULL)  
    return xd->xrm.last_date;
  else
    return "none";
}

char *
xnee_get_last_program(xnee_data *xd)
{
  if (xd->xrm.last_prog!=NULL)  
    return xd->xrm.last_prog;
  else
    return "none";
}

char *
xnee_get_last_prog_vers(xnee_data *xd){
  if (xd->xrm.last_prog_vers!=NULL)  
    return xd->xrm.last_prog_vers;
  else
    return "none";
}

char *
xnee_get_author_name(xnee_data *xd){
  if (xd->xrm.author_name!=NULL)  
    return xd->xrm.author_name;
  else
    return "none";
}

char *
xnee_get_author_email(xnee_data *xd){
  if (xd->xrm.author_email!=NULL)  
    return xd->xrm.author_email;
  else
    return "none";
}


int
xnee_set_project_name(xnee_data *xd, char *str){
  XNEE_FREE_IF_NOT_NULL(xd->xrm.project_name);
  xd->xrm.project_name=strdup(str);
  return XNEE_OK;
}

int
xnee_set_project_descr(xnee_data *xd, char *str){
  xd->xrm.project_descr=strdup(str);
  return XNEE_OK;
}

int
xnee_set_creat_date(xnee_data *xd, char *str){
  XNEE_FREE_IF_NOT_NULL(xd->xrm.creat_date);
  xd->xrm.creat_date=strdup(str);
  return XNEE_OK;
}

int
xnee_set_creat_program(xnee_data *xd, char *str){
  XNEE_FREE_IF_NOT_NULL(xd->xrm.creat_prog_vers);
  xd->xrm.creat_prog=strdup(str);
  return XNEE_OK;
}

int
xnee_set_creat_prog_vers(xnee_data *xd, char *str){
  xd->xrm.creat_prog_vers=strdup(str);
  return XNEE_OK;
}

int
xnee_set_last_date(xnee_data *xd, char *str)
{
  XNEE_FREE_IF_NOT_NULL(xd->xrm.last_date);
  xd->xrm.last_date=strdup(str);
  return XNEE_OK;
}

int
xnee_set_last_program(xnee_data *xd, char *str){
  xd->xrm.last_prog=strdup(str);
  return XNEE_OK;
}
int
xnee_set_last_prog_vers(xnee_data *xd, char *str)
{
  xd->xrm.last_prog_vers=strdup(str);
  return XNEE_OK;
}
int
xnee_set_author_name(xnee_data *xd, char *str){
  xd->xrm.author_name=strdup(str);
  return XNEE_OK;
}

int
xnee_set_author_email(xnee_data *xd, char *str){
  xd->xrm.author_email=strdup(str);
  return XNEE_OK;
}


int
xnee_is_resource_option(xnee_data *xd, const char *str, int syntax_type)
{
  /*   printf ("'%s'     %s:%d  (%s)  str=%s (%lu)\n", */
  /* 	  str, __FILE__, __LINE__, __func__, str, str); */

  xnee_is_option(xd, xnee_options, str, syntax_type);
}

int
xnee_is_option(xnee_data *xd, 
	       xnee_options_t *options, 
	       const char *str, 
	       int syntax_type)
{
  int i =0;
  char *tmp;

  if (str==NULL)
    {
      return XNEE_SYNTAX_ERROR;
    }
  
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

  for (i=0;i<options->nr_of_options;i++)
    {
      /*       printf ("\t opt=%d %s %s %s\n", i,  
	       tmp, 
	       options->options[i]->option, 
	       options->options[i]->short_option); 
      */
      if (xnee_check(tmp,
		     options->options[i]->option,
		     options->options[i]->short_option))
	{
	  
	  return 1;
	}
    }
  return 0;
}



xnee_options_t*
xnee_init_strings(xnee_data *xd)
{
  if (needs_init==0)
    {
      return xnee_options;
    }

  xnee_options = &xnee_options_impl;

  xnee_add_strings(xd,
		   xnee_options,
		   XNEE_ERR_FILE,
		   "e",
		   "<file>",
		   "Redirect all Xnee verbose output file_name"
		   "(default is stderr)", 
		   XNEE_GENERAL_OPTION
		   );
    
  xnee_add_strings(xd, xnee_options, XNEE_OUT_FILE,
		   "o",
		   "<file>",
		   "Redirect all Xnee data to file_name"
		   "(default is stdout)", 
		   XNEE_GENERAL_OPTION
		   );
    
  xnee_add_strings(xd, xnee_options, XNEE_DISPLAY,
		   "d",
		   "<displayname>",
		   "X server to contact (default is localhost)",
		   XNEE_GENERAL_OPTION
		   );
    
  xnee_add_strings(xd, xnee_options, XNEE_DIMENSION,
		   NULL,
		   "<displayname>",
		   "Dimension when recorded",
		   XNEE_INTERNAL_OPTION
		   );
    
  xnee_add_strings(xd, xnee_options, XNEE_LOOPS,
		   NULL,
		   "<nr>",
		   "Nr of events to record (obsolete)",
		   XNEE_OBSOLETE_OPTION
		   );
    
  xnee_add_strings(xd, xnee_options,
		   XNEE_FIRST_LAST,
		   NULL,
		   NULL,
		   "Print only first and last of multiple successive" 
		   "MotionEvent",		  
		   XNEE_RECORD_OPTION
		   );
    
  xnee_add_strings(xd, xnee_options,
		   "all-events",
		   NULL,
		   NULL,
		   "Intercept all events",
		   XNEE_RECORD_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_ALL_EVENTS,
		   NULL,
		   NULL,
		   "Record all client's data (default)", 
		   XNEE_RECORD_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_FUTURE_CLIENTS,
		   NULL,
		   NULL,
		   "Record future client's data", 
		   XNEE_GENERAL_OPTION
		   );
      
  xnee_add_strings(xd, xnee_options,
		   XNEE_EVENT_MAX,
		   "etr",
		   "<n>",
		   "Intercept n numbers of events ( n<0 means forever)",
		   XNEE_RECORD_OPTION
		   );
    
  xnee_add_strings(xd, xnee_options,
		   XNEE_DATA_MAX,
		   "dtr",
		   "<n>",
		   "Intercept n numbers of data ( n<0 means forever)", 
		   XNEE_GENERAL_OPTION
		   );
    
  xnee_add_strings(xd, xnee_options,
		   XNEE_TIME_MAX,
		   "str", 
		   "<n>",
		   "Intercept for n seconds ( n<0 means forever)"	,
		   XNEE_RECORD_OPTION
		   );
    
  xnee_add_strings(xd, xnee_options,
		   XNEE_STOP_KEY,
		   "sk",
		   "<key>",
		   "When pressing key Xnee exits", 
		   XNEE_GRAB_OPTION
		   );
    
  xnee_add_strings(xd, xnee_options,
		   XNEE_PAUSE_KEY,
		   "pk",
		   "<key>",
		   "When pressing key Xnee pauses its current action", 
		   XNEE_GRAB_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_RESUME_KEY,
		   "rk",
		   "<key>",
		   "When pressing key Xnee resumes its paused action", 
		   XNEE_GRAB_OPTION
		   );
    
  xnee_add_strings(xd, xnee_options,
		   XNEE_INSERT_KEY,
		   "ik",
		   "<key>", 
		   "When pressing key Xnee inserts a META mark"
		   "in the session file" , 
		   XNEE_GRAB_OPTION
		   );
    
  xnee_add_strings(xd, xnee_options,
		   XNEE_EXEC_KEY,
		   "ek",
		   "<key>",
		   "When pressing modifier mod and key key Xnee inserts "
		   "an exec mark into the session file" ,
		   XNEE_GRAB_OPTION
		   );
    
  xnee_add_strings(xd, xnee_options,
		   XNEE_EXEC_PROGRAM,
		   "ep",
		   "<programname>", 
		   "Program to start when pressing the grabbed exec key",
		   XNEE_GRAB_OPTION
		   );


  xnee_add_strings(xd, xnee_options,
		   XNEE_DELAY_TIME,
		   "t",
		   "<secs>",
		   "Delay start of application for <secs> seconds. Used to "
		   "prevent recording of KeyRelease when starting Xnee from"
		   "terminal",
		   XNEE_RECORD_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_SPEED_PERCENT,
		   "sp",
		   "<speed>",
		   "Set replaying speed percentage is set to speed",
		   XNEE_REPLAY_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_RECORDED_RESOLUTION,
		   NULL,
		   "<res>", 
		   "Resolution to use when recording is set to res",
		   XNEE_RECORD_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_REPLAY_RESOLUTION,
		   NULL,
		   "<res>", 
		   "Resolution to use when replaying is set to res",
		   XNEE_REPLAY_OPTION
		   );
    
  xnee_add_strings(xd, xnee_options,
		   XNEE_ADJUST_RESOLUTION,
		   NULL, 
		   NULL, 
		   "Use resolution adjustment",
		   XNEE_REPLAY_OPTION
		   );
    
  xnee_add_strings(xd, xnee_options,
		   XNEE_DISTRIBUTE,
		   "di",
		   "<LIST>",
		   "Distribute recorded or replayed events to LIST where"
		   "LIST is comma separated list of displays",
		   XNEE_GENERAL_OPTION
		   );
    
  xnee_add_strings(xd, xnee_options, XNEE_SYNC_MODE,
		   NULL,
		   NULL,
		   "Synchronise during replay",
		   XNEE_REPLAY_OPTION
		   );
    
  xnee_add_strings(xd, xnee_options,
		   XNEE_NO_SYNC_MODE,
		   "ns", 
		   NULL,
		   "Don't use synchornisation during replay",
		   XNEE_REPLAY_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_ALL_CLIENTS,
		   "ac", 
		   NULL,
		   "Record all client's data (default)",
		   XNEE_GENERAL_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_FUTURE_CLIENTS,
		   "fc", 
		   NULL,
		   "Record future client's data (default)",
		   XNEE_GENERAL_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_RECALL_WINDOW_POS,
		   "rcp",
		   NULL,
		   "Recall the recorded window position to be used"
		   "during replay",
		   XNEE_REPLAY_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_HUMAN_PRINTOUT,
		   "hp", 
		   NULL,
		   "Prints human readable",
		   XNEE_RECORD_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_FORCE_REPLAY,
		   "fp", 
		   NULL,
		   "Keep replaying even if we are out of sync"
		   ".... dangerous",   
		   XNEE_REPLAY_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_PLUGIN,
		   "p",
		   "<name", 
		   "Use the plugin name",
		   XNEE_GENERAL_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_VERBOSE,
		   "v", 
		   NULL,
		   "Enable verbose printout",
		   XNEE_GENERAL_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_BUFFER_VERBOSE,
		   "bv", 
		   NULL,
		   "Enable verbose printout of replay buffer",
		   XNEE_REPLAY_OPTION
		   );
  
  xnee_add_strings(xd, xnee_options,
		   XNEE_KEYBOARD,
		   NULL, 
		   NULL, 
		   "Record the keyboard",
		   XNEE_RECORD_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_STORE_MOUSE_POS,
		   "smp", 
		   NULL, 
		   "Store the initial position of the mouse",
		   XNEE_RECORD_OPTION
		   );
  
  xnee_add_strings(xd, xnee_options,
		   XNEE_FEEDBACK_XOSD,
		   "fx",  
		   NULL, 
		   "Use xosd to feedback",
		   XNEE_GENERAL_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_FEEDBACK_STDERR,
		   "fs", 
		   NULL, 
		   "Use stderr to feedback",
		   XNEE_GENERAL_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_FEEDBACK_NONE,
		   "fn", 
		   NULL, 
		   "Dont' use feedback",
		   XNEE_GENERAL_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_REQUEST_STR,
		   "reqra",
		   "<X_LIST>", 
		   "Set request range to X_LIST",
		   XNEE_RECORD_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_REPLIES_STR,
		   "repra",
		   "<X_LIST>", 
		   "Set reply range to X_LIST",
		   XNEE_RECORD_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_DEVICE_EVENT_STR,
		   "devera",
		   "<X_LIST>", 
		   "Set device event range to X_LIST",
		   XNEE_GENERAL_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_DELIVERED_EVENT_STR,
		   "devra",
		   "<X_LIST>",
		   "Set delivered event range to X_LIST", 
		   XNEE_RECORD_OPTION
		   );
  
  xnee_add_strings(xd, xnee_options,
		   XNEE_ERROR_STR,
		   "erra",
		   "<X_LIST>", 
		   "Set error range to X_LIST",
		   XNEE_RECORD_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_REPLIES_STR,
		   "repra",
		   "<X_LIST>", 
		   "Set reply range to X_LIST",
		   XNEE_RECORD_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_EXT_REQ_MAJ_STR,
		   "erqmar",
		   "<X_LIST>",
		   "Set extension request major range to X_LIST",
		   XNEE_RECORD_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_EXT_REQ_MIN_STR,
		   "erqmir",
		   "<X_LIST>", 
		   "Set extension request minor range to X_LIST", 
		   XNEE_RECORD_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_EXT_REP_MAJ_STR,
		   "erpmar",
		   "<X_LIST>", 
		   "Set extension reply major range to X_LIST", 
		   XNEE_RECORD_OPTION
		   );

  xnee_add_strings(xd, xnee_options,
		   XNEE_EXT_REP_MIN_STR,
		   "erpmir",
		   "<X_LIST>", 
		   "Set extension reply minor range to X_LIST", 
		   XNEE_RECORD_OPTION
		   );


  xnee_add_strings(xd, xnee_options,
		   XNEE_MAX_THRESHOLD,
		   "mat",
		   "<nr>", 
		   "Set the maximum threshold for sync to nr",
		   XNEE_REPLAY_OPTION
		   );
    
  xnee_add_strings(xd, xnee_options,
		   XNEE_MIN_THRESHOLD,
		   "mit",
		   "<nr>", 
		   "Set the minimum threshold for sync to tnr",
		   XNEE_REPLAY_OPTION
		   );
    
  xnee_add_strings(xd, xnee_options,
		   XNEE_TOT_THRESHOLD,
		   "tot",
		   "<nr>", 
		   "Set the total threshold for sync to nr",
		   XNEE_REPLAY_OPTION
		   );
    
  xnee_add_strings(xd, xnee_options,
		   XNEE_REPLAY_OFFSET,
		   "ro",
		   "<x,y>",
		   "Set the replay offset to (x,y)",
		   XNEE_REPLAY_OPTION
		   );
    
  needs_init=0;
  return xnee_options;
}
 


int
xnee_add_strings (xnee_data  *xd, 
		  xnee_options_t  *options,		  
		  const char *option, 
		  const char *short_option, 
		  char       *args,
		  char       *option_descr,
		  int         type)
{

  if ( (option==NULL) || (option_descr==NULL) )
    {
      xnee_verbose((xd, "Option or description to add is NULL\n"));
      return XNEE_SYNTAX_ERROR;
    }

  if (options->nr_of_options==0)
    {
      options->options = calloc(1, sizeof(xnee_option_t*));
    }
  else
    {
      options->options = realloc(options->options, 
				 (options->nr_of_options+1)*sizeof(xnee_option_t*));
    }
  
  if (options!=NULL)
    {
      options->options[options->nr_of_options] = 
	(xnee_option_t *)
	calloc(1, sizeof(xnee_option_t));
      options->nr_of_options++;
    }
  else
    {
      return XNEE_MEMORY_FAULT;
    }
  options->options[options->nr_of_options-1]->type   = type; 
  options->options[options->nr_of_options-1]->option = (char*)option;
  options->options[options->nr_of_options-1]->short_option = (char*)short_option; 
  options->options[options->nr_of_options-1]->description = option_descr; 

  if (args!=NULL)
    {
      options->options[options->nr_of_options-1]->args   = args;
    }
  if ( (options->options[options->nr_of_options-1]->option==NULL) ||
       (options->options[options->nr_of_options-1]->description==NULL) )
    {
      return XNEE_MEMORY_FAULT;
    }

  return XNEE_OK;
}

int
xnee_find_option_entry (xnee_data  *xd, 
			const char *option, 
			xnee_options_t  *options)
{

  int i ; 

  if (options==NULL)
    {
      return XNEE_MEMORY_FAULT;
    }

  for (i=0;i<options->nr_of_options;i++)
    {
      if (!strncmp(option,  
		   options->options[i]->option,
		   strlen(options->options[i]->option)))
	{
	  /*	  printf ("** Found option %s at %d in option array  (%s)\n", 
		  option, i, options->options[i]->option);
	  */
	  return i;
	}
    }
  return -1;
}

int 
xnee_parse_check_opts(xnee_data *xd,         
		      const char *opt, 
		      const char *short_opt, 
		      char *user_str)
{
  int ret ;
  int user_len;
  int opt_len;
  int len;

  XNEE_VERBOSE_ENTER_FUNCTION();


  xnee_verbose((xd, "%s: '%s' '%s' '%s' = %d\n",
		__func__, 
		opt,
		short_opt,
		user_str,
		ret));

  user_len = strlen(user_str);
  opt_len = strlen(opt);

  if ( opt_len > user_len)
    {
      len = user_len;
    }
  else
    {
      len = opt_len;
    }
  ret = (!strncmp(opt,user_str,len)) &&
    (user_len == opt_len);
  xnee_verbose((xd, "%s: ret(1) = %d\n", __func__, ret));

  user_len = strlen(user_str);
  opt_len = strlen(short_opt);

  if ( opt_len > user_len)
    {
      len = user_len;
    }
  else
    {
      len = opt_len;
    }

  ret = ret || 
    ( (strncmp(short_opt,user_str,len)==0)
      &&
      (user_len == opt_len));

  xnee_verbose((xd, "%s: ret(2) = %d\n", __func__, ret));

  
  
  XNEE_VERBOSE_LEAVE_FUNCTION();
  return ret;
}


int 
xnee_parse_check_impl(xnee_data *xd, 
		      xnee_options_t *options,
		      const char *opt, 
		      char *user_str)
{
  int pos; 
  int ret;
  char *short_opt;

  pos = xnee_find_option_entry(xd, opt, options);
  if (pos<0)
    {
      short_opt=NULL;
    }
  else
    {
      short_opt = options->options[pos]->short_option;
    }

  if (short_opt==NULL)
    {
      short_opt = (char *)opt;
    }
  ret = xnee_parse_check_opts(xd, opt, short_opt, user_str);
  
  return ret;
}

