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
#include "libxnee/xnee_resource.h"
#include "libxnee/xnee_setget.h"

/* #include "cnee_fake.h" */
/* #include "cnee_strings.h" */

#include "cnee_printer.h"
#include "parse.h"

static xnee_option_t cnee_options_impl[] = 
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
      CNEE_FILE_OPTION_KEY,
      "file",
      "f",
      "<file_name>", 
      "Read data from file file_name (default is stdin)", 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE,
    },
    { 
      CNEE_HELP_OPTION_KEY,
      "help",
      "h",
      NULL,
      "Print this message", 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },
    { 
      CNEE_FLAGS_OPTION_KEY,
      "flags",
      NULL,
      NULL, 
      "Prints all flags/options xnee accepts",
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },
    
    { 
      CNEE_PROJECT_OPTION_KEY,
      "project",
      "pr",
      "<file_name>", 
      "Use project file file_name",
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },
    { 
      CNEE_VERSION_OPTION_KEY,
      "version",
      "V",
      NULL,
      "Print product information",
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },
    { 
      CNEE_RECORD_OPTION_KEY,
      "record",
      "rec",
      NULL, 
      "Set recording mode" , 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },
    { 
      CNEE_REPLAY_OPTION_KEY,
      "replay",
      "rep",
      NULL, 
      "Set replaying mode" , 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    { 
      CNEE_RETYPE_FILE_OPTION_KEY,
      "retype",
      NULL,
      "<file>", 
      "Types (faking user) the contents of the specified file" , 
      XNEE_MISC_OPTION,
      XNEE_OPTION_VISIBLE
    },

    { 
      CNEE_WRITE_SETTINGS_OPTION_KEY,
      "write-settings file",
      NULL,
      NULL,
      "Writes settings to a resource file",
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },


    { 
      CNEE_PRINT_SETTINGS_OPTION_KEY,
      "print-settings",
      "ps",
      NULL, 
      "Prints Xnee settings and waits (for <ENTER>)", 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    { 
      CNEE_PRINT_E_NAMES_OPTION_KEY,
      "print-event-names",
      "pens",
      NULL,
      "Prints X11 event number and name ", 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    { 
      CNEE_PRINT_E_NAME_OPTION_KEY,
      "print-event-name",
      "pen",
      "<ev>", 
      "Prints X11 event number or name coresponding to ev", 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    { 
      CNEE_PRINT_ERR_NAMES_OPTION_KEY,
      "print-error-names",
      "perns",
      NULL,
      "Prints X11 error number and name ", 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    { 
      CNEE_PRINT_ERR_NAME_OPTION_KEY,
      "print-error-name",
      "pern",
      "<er>",
      "Prints X11 error number or name coresponding to er ", 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    { 
      CNEE_PRINT_REP_NAMES_OPTION_KEY,
      "print-reply-names",
      "pren",
      NULL,
      "Prints X11 reply number and name ", 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    { 
      CNEE_PRINT_REQ_NAMES_OPTION_KEY,
      "print-request-names",
      "prns",
      NULL,
      "Prints X11 request number and name ", 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },
    { 
      CNEE_PRINT_REQ_NAME_OPTION_KEY,
      "print-request-name",
      "prns",
      "<req>", 
      "Prints X11 request number or name  coresponding to req", 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    {
      XNEE_KEYBOARD_OPTION_KEY,
      "keyboard",
      NULL, 
      NULL, 
      "Record the keyboard",
      XNEE_RECORD_OPTION,
      XNEE_OPTION_VISIBLE,
    },
    { 
      XNEE_MOUSE_OPTION_KEY,
      "mouse",
      NULL, 
      NULL,
      "Record mouse events", 
      XNEE_RECORD_OPTION,
      XNEE_OPTION_VISIBLE
    },
    { 
      CNEE_PRINT_DATA_NAMES_OPTION_KEY,
      "print-data-names",
      "pdn",
      NULL, 
      "Prints X11 data number and name ", 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },
    { 
      CNEE_GEN_MANPAGE_OPTION_KEY,
      "manpage",
      NULL,
      NULL,
      "Prints Xnee help text in format as used when generating man page", 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },
    { 
      CNEE_GEN_TEXIPAGE_OPTION_KEY,
      "texipage",
      NULL,
      NULL,
      "Prints Xnee help text in format as used when generating info page", 
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    { 
      CNEE_TYPE_HELP_OPTION_KEY,
      "type-help",
      NULL,
      NULL,
      "Type this help message using faked keys (used to test xnee itself)",
      XNEE_GENERAL_OPTION,
      XNEE_OPTION_VISIBLE
    },

    { 
      CNEE_DEMONSTRATION_OPTION_KEY,
      "demonstration",
      "demo",
      NULL,
      "Let Xnee take you on a demonstration ride",
      XNEE_GENERAL_OPTION
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

  };


xnee_option_t *cnee_options = &cnee_options_impl[0];
static int need_init = 1;

#define xnee_cli_key2string(key) \
   xnee_key2string(xd, cnee_options, key)



int
xnee_parse_cnee_option(xnee_data *xd, char **opt_and_args, int *args_used)
{
  int ret = XNEE_OK;
  int entry ; 
  int key; 


  /* By default we set used nr of arguments to 0 */
  *args_used = 0;


  entry = xnee_find_cli_option_entry(xd, 
				     cnee_options,
				     opt_and_args[0]);

  if ( entry == XNEE_OPTION_NOT_FOUND )
    {
      return -1;
    }


  key = cnee_options[entry].key;

  xnee_verbose((xd,
		"Found cli entry for '%s' at position: %d\n", 
		opt_and_args[0], entry));
  xnee_verbose((xd,
		"\tlong option: '%s'\n", 
		EMPTY_IF_NULL(cnee_options[entry].option)));
  xnee_verbose((xd,
		"\tshort option:'%s'\n", 
		EMPTY_IF_NULL(cnee_options[entry].short_option)));
  xnee_verbose((xd,  
		"\tlong option: '%s'\n", cnee_options[entry].option));
  xnee_verbose((xd,  
		"\tshort option:'%s'\n", cnee_options[entry].short_option));
      

  #define verbose_option(a)  \
       xnee_verbose((xd,"%s:%d-->%s() Handling: '%s' \n", \
		     __FILE__ , __LINE__, __func__, a ));

  switch (key)
    {
    case XNEE_NO_OPTION_KEY:
      ret = XNEE_SYNTAX_ERROR;
      break;
            
    case CNEE_FILE_OPTION_KEY:
      verbose_option("CNEE_FILE_OPTION_KEY");
      ret = xnee_set_data_name (xd,opt_and_args[1] );
      *args_used = 1;
      break;
      
    case CNEE_HELP_OPTION_KEY:              
      verbose_option("CNEE_HELP_OPTION_KEY");
      xnee_usage (stdout);
      ret = XNEE_OK_LEAVE;
      break;

    case CNEE_FLAGS_OPTION_KEY:             
      verbose_option("CNEE_FLAGS_OPTION_KEY");
      xnee_flags(stdout);
      ret = XNEE_OK_LEAVE;
      break;

    case CNEE_PROJECT_OPTION_KEY:           
      verbose_option("CNEE_PROJECT_OPTION_KEY");
      ret = xnee_set_project_file(xd, opt_and_args[1]);
      *args_used = 1;
      break;

    case CNEE_VERSION_OPTION_KEY:           
      verbose_option("CNEE_VERSION_OPTION_KEY");
      xnee_version(xd);
      return XNEE_OK_LEAVE;
      break;

    case CNEE_RECORD_OPTION_KEY:            
      verbose_option("CNEE_RECORD_OPTION_KEY");
      ret = xnee_set_recorder(xd);
      break;

    case CNEE_REPLAY_OPTION_KEY:            
      verbose_option("CNEE_REPLAY_OPTION_KEY");
      ret = xnee_set_replayer(xd);
      break;

    case CNEE_TYPE_HELP_OPTION_KEY:         
      verbose_option("CNEE_TYPE_HELP_OPTION_KEY");
/*       xnee_set_retyper(xd); */
/*       ret =  xnee_set_rt_name (xd, opt_buf) ; */
      break;

    case CNEE_GEN_MANPAGE_OPTION_KEY:       
      xnee_manpage (stdout);
      verbose_option("CNEE_GEN_MANPAGE_OPTION_KEY");
      return XNEE_OK_LEAVE;
      break;

    case CNEE_GEN_TEXIPAGE_OPTION_KEY:      
      verbose_option("CNEE_GEN_TEXIPAGE_OPTION_KEY");
      xnee_infopage (stdout);
      return XNEE_OK_LEAVE;
      break;

    case CNEE_WRITE_SETTINGS_OPTION_KEY:    
      verbose_option("CNEE_WRITE_SETTINGS_OPTION_KEY");
      break;

    case CNEE_PRINT_SETTINGS_OPTION_KEY:    
      verbose_option("CNEE_PRINT_SETTINGS_OPTION_KEY");
      break;

    case CNEE_PRINT_E_NAMES_OPTION_KEY:     
      verbose_option("CNEE_PRINT_E_NAMES_OPTION_KEY");
      xnee_print_event_info(xd);
      ret = XNEE_OK_LEAVE;
      break;

    case CNEE_PRINT_E_NAME_OPTION_KEY:      
      verbose_option("CNEE_PRINT_E_NAME_OPTION_KEY");
      xnee_print_data_str(xd, opt_and_args[1], XNEE_EVENT);
      ret = XNEE_OK_LEAVE;
      *args_used = 1;
      break;

    case CNEE_PRINT_ERR_NAMES_OPTION_KEY:   
      verbose_option("CNEE_PRINT_ERR_NAMES_OPTION_KEY");
      break;

    case CNEE_PRINT_ERR_NAME_OPTION_KEY:    
      verbose_option("CNEE_PRINT_ERR_NAME_OPTION_KEY");
      xnee_print_data_str(xd, opt_and_args[1], XNEE_ERROR);
      ret = XNEE_OK_LEAVE;
      break;

    case CNEE_PRINT_REP_NAMES_OPTION_KEY:   
      verbose_option("CNEE_PRINT_REP_NAMES_OPTION_KEY");
      xnee_print_reply_info(xd);
      ret = XNEE_OK_LEAVE;
      break;

    case CNEE_PRINT_REQ_NAMES_OPTION_KEY:   
      verbose_option("CNEE_PRINT_REQ_NAMES_OPTION_KEY");
      xnee_print_request_info(xd);
      ret = XNEE_OK_LEAVE;
      break;

    case CNEE_PRINT_REQ_NAME_OPTION_KEY:    
      verbose_option("CNEE_PRINT_REQ_NAME_OPTION_KEY");
      xnee_print_data_str(xd, opt_and_args[1], XNEE_REQUEST);
      ret = XNEE_OK_LEAVE;
      break;

    case CNEE_PRINT_DATA_NAMES_OPTION_KEY:  
      verbose_option("CNEE_PRINT_DATA_NAMES_OPTION_KEY");
      break;

    case CNEE_RETYPE_FILE_OPTION_KEY:       
      verbose_option("CNEE_RETYPE_FILE_OPTION_KEY");
      break;

    case CNEE_REMOVE_EVENT_OPTION_KEY:      
      verbose_option("CNEE_REMOVE_EVENT_OPTION_KEY");
      break;

    case CNEE_DEMONSTRATION_OPTION_KEY:     
      verbose_option("CNEE_DEMONSTRATION_OPTION_KEY");
      cnee_demonstration (xd);
      ret = XNEE_OK_LEAVE;
      break;

    case XNEE_KEYBOARD_OPTION_KEY:          
      verbose_option("XNEE_KEYBOARD_OPTION_KEY");
      ret=xnee_parse_range (xd, XNEE_DEVICE_EVENT, 
			    "KeyPress-KeyRelease");
      break;

    case XNEE_MOUSE_OPTION_KEY: 
      verbose_option("XNEE_MOUSE_OPTION_KEY");
      ret=xnee_parse_range (xd, XNEE_DEVICE_EVENT, 
			    "ButtonPress-MotionNotify");
      break;

    default:
      ret = XNEE_NO_OPTION_KEY;
      
    }

  return ret;
}




/**************************************************************
 *                                                            *
 * xnee_parse_args                                            *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_parse_args (xnee_data* xd , int argc, char **argv )
{                                         

  int i ;
  int ret;
  const char *descr;
  const char *err;
  int entry ;
  int args_used;

  char *last_opt = NULL;

  for (i=1; i < argc; i++) 
    {
      ret = xnee_parse_cnee_option(xd, &argv[i], &args_used);
      if ( ret == XNEE_OK)
	{
	  i = i + args_used ;
	  continue;
	}
      else if ( ret == XNEE_OK_LEAVE)
	{
	  return ret;
	}

      ret = xnee_parse_xns_option(xd, &argv[i], &args_used);
      if ( ret == XNEE_OK )
	{
	  i = i + args_used;
	  continue;
	}
      else if ( ret == XNEE_OK_LEAVE)
	{
	  return ret;
	}
      else
	{
	  break;
	}
      

    }
      
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

  return ret;
}




