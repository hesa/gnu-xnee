/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003 
 *                      2004, 2005, 2006, 2007 Henrik Sandklef
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
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_resource.h"
#include "libxnee/xnee_range.h"
#include "libxnee/xnee_session.h"
#include "libxnee/xnee_alloc.h"

#include "parse.h"
#include "cnee_strings.h"


#define CNEE_DEMO_DELAYED_START 3
xnee_option_t *cnee_options ;

static char *
cnee_get_default_filename()
{
  char *tmp_dir = NULL;
  char *user_name = NULL;

  const char *default_tmp_dir   = "/tmp/";
  const char *default_user_name = "user";
  const char *file_prefix = "cnee_";
  const char *file_suffix = ".xns";

  char *ret_str;
  int   size = 0 ; 

  /* set the dir */
  tmp_dir = getenv("TMPIR");
  if (tmp_dir==NULL)
    {
      tmp_dir= (char *) default_tmp_dir;
    }

  /* set the user name */
  user_name = getlogin();
  if (user_name==NULL)
    {
      user_name = (char *) default_user_name; 
    }
  
  size = strlen (tmp_dir) + 
    strlen (user_name)    + 
    strlen (file_prefix)  + 
    strlen (file_suffix) ;
  
  ret_str = (char*) malloc (size*sizeof(char));
  if (ret_str==NULL)
    {
      return NULL;
    }

  strncpy (ret_str, tmp_dir, size);
  strncat (ret_str, file_prefix, size - strlen(ret_str));
  strncat (ret_str, user_name, size - strlen(ret_str));
  strncat (ret_str, file_suffix, size - strlen(ret_str));
  return ret_str;
}



int
cnee_demonstration (xnee_data *xd)
{
  int ret;
  char *file; 

  /* Setup for recording */
  ret = xnee_set_recorder(xd);
  XNEE_RETURN_IF_ERR(ret);

  ret = xnee_parse_range (xd, XNEE_DEVICE_EVENT, 
			  "ButtonPress-MotionNotify");
  XNEE_RETURN_IF_ERR(ret);
  
  ret = xnee_parse_range (xd, XNEE_DEVICE_EVENT, 
			  "KeyPress-KeyRelease");
  XNEE_RETURN_IF_ERR(ret);
  
  file = cnee_get_default_filename();
  if (file==NULL)
    {
      return XNEE_FILE_NOT_FOUND;
    }
  
  ret = xnee_set_out_name(xd, file);
  XNEE_RETURN_IF_ERR(ret);

  ret = xnee_set_interval(xd, CNEE_DEMO_DELAYED_START);
  XNEE_RETURN_IF_ERR(ret);

  ret = xnee_set_events_max(xd, 200);
  XNEE_RETURN_IF_ERR(ret);

  /* Set the cli parameters */
  xnee_set_application_parameters (xd, NULL);
  XNEE_RETURN_IF_ERR(ret);

  ret = xnee_prepare(xd);
  if (ret==XNEE_OK)
    {
      fprintf (stderr, 
	       "Xnee will now start recording your "
	       "mouse and keyboard in %d seconds\n", 
	       CNEE_DEMO_DELAYED_START);

      /* start up the action set during parsing the commnd line */
      ret = xnee_start(xd);

    }

  if ( ret != XNEE_OK)
    {
      xnee_print_error (PACKAGE " failed to start\n");
      xnee_print_error ("   Error number:    %d\n", ret);
      xnee_print_error ("   Error string:    '%s'\n", 
			xnee_get_err_description(ret));
      xnee_print_error ("   Solution string: '%s'\n", 
			xnee_get_err_solution(ret));
      exit(ret);
    }
  else
    {
      fprintf (stderr, "Reording went well....\n");
    }

  ret = xnee_renew_xnee_data(xd);
  XNEE_RETURN_IF_ERR(ret);

  ret = xnee_set_data_name (xd, file);
  XNEE_RETURN_IF_ERR(ret);
  
  ret = xnee_set_replayer (xd);
  XNEE_RETURN_IF_ERR(ret);

  ret = xnee_prepare(xd); 
  if (ret==XNEE_OK)
    {
      fprintf (stderr, 
	       "Xnee will now replay your recorded "
	       "mouse and keyboard events in %d seconds\n", 
	       CNEE_DEMO_DELAYED_START);

      /* start up the action set during parsing the commnd line */
      ret = xnee_start(xd);
    }
  if ( ret != XNEE_OK)
    {
      xnee_print_error (PACKAGE " failed to start\n");
      xnee_print_error ("   Error number:    %d\n", ret);
      xnee_print_error ("   Error string:    '%s'\n", 
			xnee_get_err_description(ret));
      xnee_print_error ("   Solution string: '%s'\n", 
			xnee_get_err_solution(ret));
      exit(ret);
    }
  else
    {
      fprintf (stderr, "Replay went well....\n");
    }

  fprintf (stderr, "By the way, you can find the recorded file here:\n\t%s\n",
	   file);
  fprintf (stderr, "To replay that file again, type the following command\n");

  fprintf (stderr, 
	   "\t"
	   XNEE_CLI " --%s --%s %s\n", 
	   xnee_key2string(xd, xnee_options, XNEE_REPLAY_OPTION_KEY),
	   xnee_key2string(xd, cnee_options, CNEE_FILE_OPTION_KEY),
	   file);

  xnee_free (file);


  /* hey, we are fin(n)ished .... close down */
  xnee_close_down(xd);

  /* Since we are here, we can exit gracefully */
  exit(XNEE_OK); 
}
