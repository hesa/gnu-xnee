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

#include "cnee.h"
#include "cnee_fake.h"
#include "cnee_strings.h"
#include "libxnee/xnee_resource.h"



static int 
cnee_fake_string(xnee_data *xd, char *my_string)
{
  xnee_script_s xss ;
  int i ;
  int len ; 

  len = (int)strlen(my_string);

  xnee_verbose((xd, "Faking: '%s'  len=%d\n", my_string));

  for (i=0;i<len;i++)
    {
      xnee_char2keycode(xd, my_string[i], &xss.kc); 
      xnee_fake_key_mod_event (xd, &xss, XNEE_PRESS,  CurrentTime);
      usleep (1000*10);
      xnee_fake_key_mod_event (xd, &xss, XNEE_RELEASE,  CurrentTime);
    }
  usleep(1000*200);
  
  /*
  xnee_char2keycode(xd, my_string[0], &xss.kc); 
  xnee_fake_key_mod_event (xd, &xss, XNEE_PRESS,  CurrentTime);
  usleep (1000*100);
  xnee_fake_key_mod_event (xd, &xss, XNEE_RELEASE,  CurrentTime);
  */

  return XNEE_OK;
}


static int
xnee_type_help_sub (xnee_data *xd, xnee_option_t *opts, int type)
{
  int ret;
  int i ;
  char *command;
  char *descr;
  char *option_long;
  char *option_short;
  char *option_arg;
  char *option_descr;
  char *empty_str="";

  #define HELP_STR_SIZE 500
  char my_string[HELP_STR_SIZE];

  for (i = 0; xnee_options[i].key!=XNEE_LAST_OPTION; i++) 
    {
      if (xnee_options[i].type==type)
	{
	  option_long  = xnee_options[i].option ;
	  option_short = xnee_options[i].short_option ;
	  option_arg   = xnee_options[i].args ;
	  option_descr = xnee_options[i].description ;
	  snprintf (my_string, HELP_STR_SIZE, "%s,-%s %s\n",
		    (option_long ? option_long  : empty_str),
		    (option_short? option_short : empty_str),
		    (option_arg  ? option_arg   : empty_str)
		    );
	  ret = cnee_fake_string(xd, my_string);
	}
    }
  return ret;
}


int
xnee_type_help (xnee_data *xd)
{
  char **cpp;

  xnee_verbose ((xd,"---> xnee_type_help\n"));
  xnee_setup_display (xd);
  xnee_replay_init (xd);   
  xnee_set_autorepeat (xd);
  if (!xnee_has_xtest_extension(xd))
    {
      exit(XNEE_NO_TEST_EXT);
    }


  cnee_fake_string(xd ,"USAGE: " XNEE_CLI " [OPTIONS]\n");
  cnee_fake_string(xd ,"\n");
  cnee_fake_string(xd ,"DESCRIPTION\n");
  for (cpp = description; *cpp; cpp++) 
    {
      cnee_fake_string(xd, *cpp);
      cnee_fake_string(xd, "\n\n");
    }

  cnee_fake_string(xd, "\nGENERAL OPTIONS\n\n");
  xnee_type_help_sub(xd, xnee_options, XNEE_GENERAL_OPTION);

  cnee_fake_string(xd, "\nRECORD OPTIONS\n\n");
  xnee_type_help_sub(xd, xnee_options, XNEE_RECORD_OPTION);

  cnee_fake_string(xd, "\nREPLAY OPTIONS\n\n");
  xnee_type_help_sub(xd, xnee_options, XNEE_REPLAY_OPTION);

  cnee_fake_string(xd, "\nCNEE OPTIONS\n\n");
  xnee_type_help_sub(xd, xnee_options, XNEE_ANY_OPTION);

  cnee_fake_string(xd, "\nCOMMENTS\n\n");
  for (cpp = explain; *cpp; cpp++) 
    {
      cnee_fake_string(xd, *cpp);
      cnee_fake_string(xd, "\n\n");
    }

  cnee_fake_string (xd ,"\nEXAMPLES\n\n");
  for (cpp = examples; *cpp; cpp++) 
    {
      cnee_fake_string(xd, *cpp);
      cnee_fake_string(xd, "\n\n");
    }

  cnee_fake_string(xd, "\nAUTHORS\n\t");
  cnee_fake_string(xd, (char*)XNEE_AUTHORS);
  cnee_fake_string(xd, "\n\n\nREPORTING BUGS\n\tReport bugs to ");
  cnee_fake_string(xd, (char*)XNEE_BUG_MAIL);
  cnee_fake_string(xd, "\n\n");

 xnee_reset_autorepeat (xd);
 xnee_verbose ((xd,"<--- xnee_type_help\n"));

 xnee_close_down(xd);
 exit(XNEE_OK_LEAVE);
}
  
