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

#include "cnee_fake.h"

extern xnee_options_t *cnee_options;
extern xnee_options_t *xnee_options;


int
xnee_type_help (xnee_data *xd)
{
  int i ;
  xnee_script_s xss ;
  char *command;
  char *descr;
  char *option_long;
  char *option_short;
  char *option_arg;
  char *option_descr;
  char *empty_str="";

  #define HELP_STR_SIZE 500

  char my_string[HELP_STR_SIZE];

  xnee_verbose ((xd,"---> xnee_type_help\n"));
  xnee_setup_display (xd);
  xnee_replay_init (xd);   
  xnee_set_autorepeat (xd);
  if (!xnee_has_xtest_extension(xd))
    {
      exit(XNEE_NO_TEST_EXT);
    }

  for (i = 0; i<xnee_options->nr_of_options; i++) 
    {
      if (xnee_options->options[i]->type==XNEE_GENERAL_OPTION)
	{
	  option_long  = xnee_options->options[i]->option ;
	  option_short = xnee_options->options[i]->short_option ;
	  option_arg   = xnee_options->options[i]->args ;
	  option_descr = xnee_options->options[i]->description ;
	  snprintf (my_string, HELP_STR_SIZE, "%s,-%s %s\n",
		    (option_long ? option_long  : empty_str),
		    (option_short? option_short : empty_str),
		    (option_arg  ? option_arg   : empty_str)
		    );
	  snprintf (my_string, HELP_STR_SIZE, 
		    "\t%s\n\n",cnee_options->options[i]->option );
	}
    }
#ifdef EINAR
  for (cpp = help; *cpp; cpp+=1) 
    {
      xnee_verbose ((xd,"string to fake %s\n", *cpp));
      strncpy(my_string,*cpp,HELP_STR_SIZE);

      for (i=0;(size_t)i<strlen(my_string);i++)
	{
	  xnee_char2keycode(xd, my_string[i], &xss.kc); 
	  xnee_fake_key_mod_event (xd, &xss, XNEE_PRESS,  CurrentTime);
	  usleep (1000*10);
	  xnee_fake_key_mod_event (xd, &xss, XNEE_RELEASE,  CurrentTime);
	}
      usleep(1000*200);

      xnee_char2keycode(xd, my_string[0], &xss.kc); 
      xnee_fake_key_mod_event (xd, &xss, XNEE_PRESS,  CurrentTime);
      usleep (1000*100);
      xnee_fake_key_mod_event (xd, &xss, XNEE_RELEASE,  CurrentTime);
    } 
 xnee_reset_autorepeat (xd);
 xnee_verbose ((xd,"<--- xnee_type_help\n"));
#endif /* EINAR */ 
 xnee_close_down(xd);
 exit(XNEE_OK);
}
  
