/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003 
 *                      2004, 2005, 2006 Henrik Sandklef
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


#include "libxnee/xnee.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_resolution.h"
#include "libxnee/xnee_resource.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_km.h"
#include "libxnee/xnee_grab.h"
#include "libxnee/xnee_fake.h"
#include "libxnee/xnee_replay.h"
#include "libxnee/datastrings.h"
#include "libxnee/xnee_threshold.h"
#include "libxnee/xnee_strings.h"
#include "libxnee/print.h"
#include "libxnee/xnee_range.h"
#include "libxnee/xnee_error.h"
#include "libxnee/xnee_keysym.h"
#include "libxnee/feedback.h"
#include "libxnee/xnee_session.h"
#include "libxnee/xnee_plugin.h"
#include "libxnee/xnee_display.h"
#include "libxnee/xnee_utils.h"

#include "cnee_fake.h"
#include "cnee_strings.h"
#include "cnee_printer.h"

#include "parse.h"

enum 
  {
    CNEE_NO_OPTION,
    CNEE_RESOURCE_OPTION,
    CNEE_CNEE_OPTION,
  } cnee_option_type;

enum 
  {
    XNEE_OPTION_IS_OPTION,
    XNEE_OPTION_IS_ARG,
    XNEE_OPTION_FLUSH
  } xnee_option_parser_command;

static xnee_options_t   cnee_options_impl;
xnee_options_t  *cnee_options;
static int need_init = 1;


int
xnee_parse_wrapper(xnee_data *xd, char *option, int type)
{
  int ret;
#define XNEE_REMOVE_LEADING_CLI_DASHES(a) \
  { if (a[0]=='-') a++; if (a[0]=='-') a++; }

  XNEE_REMOVE_LEADING_CLI_DASHES(option);

  if ( type == XNEE_RESOURCE_OPTION )
    {
      ret = xnee_expression_handle_project(xd, option);
      if ( (ret!=XNEE_OK) && (ret!=XNEE_SETTINGS_DATA) )
	{
	  return (ret);
	}
    }
  else if ( type == CNEE_CNEE_OPTION )
    {
      ret = xnee_cnee_parse_args(xd, option);

      if (ret!=XNEE_OK)
	{
	  printf ("XNEE_USAGE (cnee)");
	  /* 	  xnee_close_down(xd); */
	  /* 	  exit(ret); */
	}
    }
  else
    {
      printf ("** Panic in parser wrapper '%s' ....\n", option);
    }
  return ret;
}

int
xnee_option_parse(xnee_data *xd, char *opt, int is_option, int type)
{
  static int option_type = 0;
  int ret = XNEE_OK; 

#define XNEE_OPTION_PARSE_BUF 200
#define xnee_add_opt(o) { strcat(buf,o); strcat(buf," "); }
  
  static char buf [XNEE_OPTION_PARSE_BUF] =""; 

  if (is_option==XNEE_OPTION_IS_OPTION)
    {
/*       printf (" IS OPTION\n"); */
      if ( strlen(buf)!=0 )
	{ 
	  ret = xnee_parse_wrapper(xd, buf, option_type);
	  buf[0]='\0'; 
	} 
      option_type = type ;
      xnee_add_opt(opt);
    }
  else if (is_option==XNEE_OPTION_IS_ARG)
    {
/*       printf (" IS ARG\n"); */
      xnee_add_opt(opt);
      /*       printf ("\tAdding: '%s'\n", buf); */
    }
  else if (is_option==XNEE_OPTION_FLUSH)
    {
/*       printf (" IS FLUSH\n"); */
      ret = xnee_parse_wrapper(xd, buf, option_type);
      buf[0]='\0'; 
    }

  return XNEE_OK;
}

#define cnee_parse_check(xd, str, opt) \
    xnee_parse_check_impl(xd, cnee_options, str, opt)


int
xnee_cnee_parse_args(xnee_data *xd, char *tmp)
{
#define RANGE_BUF_SIZE 100
  int ret=XNEE_SETTINGS_DATA;  
  char *range_tmp = NULL;
  char *range = NULL;
  char range_buf[RANGE_BUF_SIZE];
  char opt_buf[RANGE_BUF_SIZE];
  char *opt;
  int len=0;
#define XNEE_PARSE_BUF 200
  char buf [XNEE_PARSE_BUF] =""; 


  /* OK the string passed the first tests */
  range_tmp=strstr (tmp, " ");

  if (range_tmp!=NULL) 
    {
      strncpy(range_buf, range_tmp, RANGE_BUF_SIZE );
      
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

  if (range!=NULL)
    {
      len=strlen(tmp) - strlen(range) ;
      strncpy (opt_buf, tmp, len);
      opt_buf[len-1]='\0';
    }
  else
    {
      strncpy (opt_buf, tmp, strlen(tmp));
      opt_buf[strlen(tmp)-1]='\0';
    }


  if (cnee_parse_check(xd, CNEE_FLAGS_OPTION_LONG,opt_buf))
    {
      xnee_usage(stdout);
      xnee_close_down(xd);
      exit(XNEE_OK);
    }
  else if(cnee_parse_check(xd,  CNEE_PROJECT_OPTION_LONG, opt_buf )) 
    {
      ret = xnee_set_rc_byname (xd, opt_buf);
      if ( ret != XNEE_OK) 
	{
	  xnee_verbose((xd, "Could not open project file %s\n", opt_buf));
	      
	  if ( (strlen(opt_buf) + strlen (XNEE_RESOURCE_DIR) + 2 ) > 200)
	    {
	      return XNEE_FILE_NOT_FOUND;
	    }
	  strncpy ( buf , XNEE_RESOURCE_DIR, XNEE_PARSE_BUF );
	  strncat ( buf ,  "/", XNEE_PARSE_BUF - strlen(buf));
	  strncat ( buf , opt_buf, XNEE_PARSE_BUF - strlen(buf));
	  xnee_verbose((xd, "\ttryingresource file %s\n", buf));
	  ret = xnee_set_rc_name (xd, buf);
	}
	  
      if ( xnee_get_rc_file (xd) != NULL) 
	{
	  ret = xnee_add_resource (xd );
	      
	  if (ret!=XNEE_OK)
	    {
	      xnee_verbose ((xd, "project file read: return value %d\n", 
			     ret));
	      if (ret == XNEE_SYNTAX_ERROR)
		{
		  char *tmp_str;
		  xnee_verbose ((xd, "project file read: SYNTAX ERROR\n"));
		  tmp_str = xnee_get_err_string();
		  fprintf (stderr,"%s", tmp_str);
		  XNEE_FREE_IF_NOT_NULL(tmp_str);
		}
	    }
	}
      else
	{
	  return (XNEE_FILE_NOT_FOUND);
	}
    }
  else if(cnee_parse_check(xd,  "retype-file", opt_buf )) 
    {
      xnee_set_retyper(xd);
      ret =  xnee_set_rt_name (xd, opt_buf) ;
    }
  else if(cnee_parse_check(xd,  "type-help", opt_buf )) 
    {
      ret = xnee_type_help(xd);
    }
  else if(cnee_parse_check(xd,  "record", opt_buf )) 
    {
      ret = xnee_set_recorder (xd);
    }
  else if(cnee_parse_check(xd,  "no-sync", opt_buf )) 
    {
      ret = xnee_unset_sync(xd);
    }
  else if(cnee_parse_check(xd,  "replay", opt_buf )) 
    {
      ret = xnee_set_replayer (xd);
    }
  else if(cnee_parse_check(xd,  "all-events", opt_buf )) 
    {
      ret = xnee_add_range (xd, XNEE_DEVICE_EVENT, 2,6);
      ret = ret + xnee_add_range (xd, XNEE_DELIVERED_EVENT, 7,LASTEvent);
    }
  else if(cnee_parse_check(xd,  "print-settings", opt_buf)) 
    {
      xnee_print_xnee_settings (xd, xnee_get_err_file(xd)) ;
      xnee_record_print_record_range (xd, xnee_get_err_file(xd)) ;
      fprintf (stderr, "Press enter to continue:\n");
      getchar();
    }
  else if(cnee_parse_check(xd,  "mouse", opt_buf)) 
    {
      ret=xnee_parse_range (xd, XNEE_DEVICE_EVENT, 
			    "ButtonPress-MotionNotify");
    }
  else if(cnee_parse_check(xd,  "keyboard", opt_buf)) 
    {
      ret=xnee_parse_range (xd, XNEE_DEVICE_EVENT, 
			    "KeyPress-KeyRelease");
    }
  else if(cnee_parse_check(xd,  "manpage", opt_buf)) 
    {
      xnee_manpage (stdout);
      exit(XNEE_OK);
    }
  else if(cnee_parse_check(xd,  "infopage", opt_buf)) 
    {
      xnee_infopage (stdout);
      exit(XNEE_OK);
    }
  else if(cnee_parse_check(xd,  "help", opt_buf)) 
    {
      xnee_usage (stdout);
      exit(XNEE_OK);
    }
  else if(cnee_parse_check(xd,  "flags", opt_buf)) 
    {
      xnee_flags (stdout);
      exit(XNEE_OK);
    }
  else if(cnee_parse_check(xd,  "remove-event", opt_buf)) 
    {
      fprintf (stderr, "Currently obsoleted function\n");
      exit(XNEE_WRONG_PARAMS);


      /* This is not a public option, but used to test libxnee
       * However, it does nothing harmful so we keep it here
       * and don't "#ifdef" it away since it may come in use
       *
       * And why not present it to the user.?
       * I figure a 'normal user' will be confused by this option
       */

/*
      ret=xnee_rem_data_from_range_str (xd, 
					XNEE_DEVICE_EVENT, 
					argv[++i]) ;
*/
    }
      
  else if(cnee_parse_check(xd,  "version", opt_buf)) 
    {
      fprintf (stderr, XNEE_CLI " part of " );
      xnee_version(xd);
      exit(XNEE_OK);
    }
  else if ( cnee_parse_check (xd,  "file", opt_buf))
    {
      ret= xnee_set_data_name (xd, opt_buf);
      if (ret==XNEE_OK)
	{
	  xnee_verbose((xd, "Reading replay data from %s \n", 
			xnee_get_data_name (xd)));
	}
    }
  else if ( cnee_parse_check (xd,  "write-settings", "-ws"))
    {
      if (xnee_check (opt_buf, "stdout", "STDOUT"))
	{
	  xnee_write_settings_to_file (xd, stdout);
	}
      else if (xnee_check (opt_buf, "stderr", "STDERR"))
	{
	  xnee_write_settings_to_file (xd, stderr);
	}
      else
	{
	  FILE *file;
	  file = fopen (opt_buf,"w");
	  xnee_write_settings_to_file (xd, 
				       file);
	  fclose(file);
	}
      exit(XNEE_OK);
    }
  else 
    {
      fprintf (stderr, "Could not parse \"%s\"\n", opt_buf);
      xnee_usage(stderr);
      exit(XNEE_WRONG_PARAMS);
    }
  if (ret!=0) 
    {
      xnee_print_error ("Could not parse %s\n", opt_buf);
      exit (ret);
    }
  return ret;
}




xnee_option_t**
cnee_init_options(xnee_data *xd)
{
  if (need_init==0)
    {
      return XNEE_OK;
    }

  cnee_options = &cnee_options_impl;

  cnee_options = &cnee_options_impl;

  xnee_add_strings(xd, 
		   cnee_options,
		   "file",
		   "f", 
		   "<file_name>", 
		   "Read data from file file_name (default is stdin)", 
		   XNEE_GENERAL_OPTION);

  xnee_add_strings(xd, cnee_options,
		   "help",
		   "h", 
		   NULL,
		   "Print this message", 
		   XNEE_GENERAL_OPTION);

  xnee_add_strings(xd, cnee_options,
		   "flags",
		   NULL,
		   NULL, 
		   "Prints all flags/options xnee accepts",
		   XNEE_GENERAL_OPTION);

  xnee_add_strings(xd, cnee_options,
		   CNEE_PROJECT_OPTION_LONG,
		   CNEE_PROJECT_OPTION_SHORT,
		   "<file_name>", 
		   "Use project file file_name",
		   XNEE_GENERAL_OPTION);


  xnee_add_strings(xd, cnee_options,
		   "version",
		   "V", 
		   NULL,
		   "Print product information",
		   XNEE_GENERAL_OPTION);

  xnee_add_strings(xd, cnee_options,
		   "record",
		   "re",
		   NULL, 
		   "Set recording mode" , 
		   XNEE_GENERAL_OPTION);

  xnee_add_strings(xd, cnee_options,
		   "replay",
		   "rep",
		   NULL, 
		   "Set replaying mode" , 
		   XNEE_GENERAL_OPTION);

  xnee_add_strings(xd, cnee_options,
		   CNEE_RETYPE_FILE_OPTION_LONG,
		   NULL,
		   "<file>", 
		   "Types (faking user) the contents of the specified file" , 
		   XNEE_MISC_OPTION);

  xnee_add_strings(xd, cnee_options,
		   "write-settings file",
		   NULL,
		   NULL,
		   "Writes settings to a resource file",
		   XNEE_GENERAL_OPTION);


  xnee_add_strings(xd, cnee_options,
		   "print-settings",
		   "ps",
		   NULL, 
		   "Prints Xnee settings and waits (for <ENTER>)", 
		   XNEE_GENERAL_OPTION);

  xnee_add_strings(xd, cnee_options,
		   "print-event-names",
		   "pens", 
		   NULL,
		   "Prints X11 event number and name ", 
		   XNEE_GENERAL_OPTION);

  xnee_add_strings(xd, cnee_options,
		   "print-event-name",
		   "pen",
		   "<ev>", 
		   "Prints X11 event number or name coresponding to ev", 
		   XNEE_GENERAL_OPTION);

  xnee_add_strings(xd, cnee_options,
		   "print-error-names",
		   "perns", 
		   NULL,
		   "Prints X11 error number and name ", 
		   XNEE_GENERAL_OPTION);

  xnee_add_strings(xd, cnee_options,
		   "print-error-name",
		   "pern",
		   "<er>",
		   "Prints X11 error number or name coresponding to er ", 
		   XNEE_GENERAL_OPTION);

  xnee_add_strings(xd, cnee_options,
		   "print-reply-names",
		   "pren", 
		   NULL,
		   "Prints X11 reply number and name ", 
		   XNEE_GENERAL_OPTION);

  xnee_add_strings(xd, cnee_options,
		   "print-request-names",
		   "prns", 
		   NULL,
		   "Prints X11 request number and name ", 
		   XNEE_GENERAL_OPTION);

  xnee_add_strings(xd, cnee_options,
		   XNEE_MOUSE,
		   NULL, 
		   NULL,
		   "Record mouse events", 
		   XNEE_RECORD_OPTION);

  xnee_add_strings(xd, cnee_options,
		   XNEE_KEYBOARD,
		   NULL, 
		   NULL,
		   "Record keyboard events", 
		   XNEE_RECORD_OPTION);

  xnee_add_strings(xd, cnee_options,
		   "print-request-name",
		   "prn",
		   "<req>", 
		   "Prints X11 request number or name  coresponding to req", 
		   XNEE_GENERAL_OPTION);

  xnee_add_strings(xd, cnee_options,
		   "print-data-names",
		   "pdn",
		   NULL, 
		   "Prints X11 data number and name ", 
		   XNEE_GENERAL_OPTION);

  xnee_add_strings(xd, cnee_options,
		   "manpage",
		   NULL,
		   NULL,
		   "Prints Xnee help text in format as used when generating man page", 
		   XNEE_GENERAL_OPTION);


  xnee_add_strings(xd, cnee_options,
		   "infopage",
		   NULL,
		   NULL,
		   "Prints Xnee help text in format as used when generating info page", 
		   XNEE_GENERAL_OPTION);


  xnee_add_strings(xd, cnee_options,
		   "type-help",
		   NULL,
		   NULL,
		   "Type this help message using faked keys (used to test xnee itself)",
		   XNEE_GENERAL_OPTION);
}


/**************************************************************
 *                                                            *
 * xnee_parse_args                                            *
 *                                                            *
 *                                                            *
 **************************************************************/
void  
xnee_parse_args (xnee_data* xd , int argc, char **argv )
{                                         

  int i ;
  int ret=0;
  cnee_init_options(xd);
  const char *descr;
  const char *err;

  for (i = 1; i < argc; i++) 
    {
      printf ("** parse '%s'\n", argv[i]);
      /* Check if ordinary Xnee Resource Syntax option */
      if ( xnee_is_resource_option(xd, argv[i], XNEE_CLI_SYNTAX) )
	{
	  ret = xnee_option_parse(xd, 
				  argv[i], 
				  XNEE_OPTION_IS_OPTION , 
				  CNEE_RESOURCE_OPTION );	  
	  printf ("** XNEE OPTION '%s'\n", argv [i]);
	}
      else if ( xnee_is_option(xd, cnee_options, argv[i], XNEE_CLI_SYNTAX) )
	{
	  ret = xnee_option_parse(xd, 
				  argv[i], 
				  XNEE_OPTION_IS_OPTION, 
				  CNEE_CNEE_OPTION );	  
	  printf ("** CNEE OPTION '%s'  ret=%d\n", argv [i], ret);
	  
	}
      else
	{
	  printf ("** 3: '%s' \n", argv[i]);
	  ret = xnee_option_parse(xd, 
				  argv[i], 
				  XNEE_OPTION_IS_ARG, 
				  CNEE_NO_OPTION);	  
	}

      if (ret != XNEE_OK)
	{
	  break;
	}
    }

  ret = xnee_option_parse(xd, 
			  NULL, 
			  XNEE_OPTION_FLUSH, 
			  CNEE_NO_OPTION);	  
  


  if (ret != XNEE_OK)
    {
      err = xnee_get_err_description(ret);
      descr = xnee_get_err_solution(ret);
      
      fprintf (stderr, "Error number: %d\n", ret);
      fprintf (stderr, "  Error:      %s\n", err);
      fprintf (stderr, "  Solution:   %s\n", descr);
      
      xnee_close_down(xd);
      exit(XNEE_WRONG_PARAMS);
    }
}




