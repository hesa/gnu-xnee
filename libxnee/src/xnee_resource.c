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
    XNEE_REMOVE_BEGINING_BLANKS(value);
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

  if (!strncmp("#",tmp,1))  /* # META data */
    {
/*       xnee_handle_resource_meta (xd, tmp); */
      if (xnee_handle_meta_data (xd, tmp+1)!=-1) 
	{ printf ("RETURN\n"); return 1; }
      else
	printf ("continue..with: %s\n", ++tmp);
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
      printf ("   time before %s   %s  : %d\n", 
	      tmp,
	      range,
	      atoi(range));
      xnee_set_time_max(xd, atoi(range));
      printf ("   time after: %d\n", atoi(range));
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
  else 
    {
      xnee_verbose((xd,"Corrupt line: \"%s\"\n", tmp)); 
      ret=-1;
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

  static char tmp[256] ;
  int read_more  = 1 ;
  
  strcpy(tmp,"");
  

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
  static char buf[20];
  if (xd->xrm.creat_date!=NULL)  
    return xd->xrm.creat_date;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  sprintf(buf, "%.4d-%.2d-%.2d",
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
