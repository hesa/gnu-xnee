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


#include "libxnee/print.h"
#include "libxnee/xnee.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_fake.h"
#include "libxnee/xnee_error.h"
#include "parse.h"



/**************************************************************
 *                                                            *
 * main                                                       *
 *                                                            *
 *                                                            *
 **************************************************************/
int main(int argc,char *argv[])
{
  int ret;
  xnee_data *xd ;
  
  /* Set the signal handler the libxnee's built in */ 
  (void) signal (SIGINT, signal_handler);
  
  /* Set the error handler the libxnee's built in */ 
  (void) XSetErrorHandler (handle_xerr);  
  
  /*  Get a new xnee_data structure  */
  xd = xnee_new_xnee_data();
  if (xd==NULL)
  {
     return (XNEE_NO_MAIN_DATA);
  }

  /* Set the program name */
  xnee_set_program_name (xd, XNEE_CLI);

  /* Well .... parse the args */
  xnee_parse_args (xd, argc, argv);


  ret = xnee_prepare(xd);
  if (ret==XNEE_OK)
    {
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


  /* hey, we are fin(n)ished .... close down */
  xnee_close_down(xd);
  

  /* Since we are here, we can exit gracefully */
  exit(XNEE_OK); 
}






