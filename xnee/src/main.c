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
#include "parse.h"

/*int xnee_set_info(xnee_data *xd);*/

int
cnee_check_protocol(xnee_data *xd);



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
  

  /*  Get a new xnee_data structure  */
  xd = xnee_new_xnee_data();


  /* Well .... parse the args */
  xnee_parse_args (xd, argc, argv);


  /* start up the action set during parsing the commnd line */
  ret = xnee_start(xd);
  if (ret==XNEE_NO_PROT_CHOOSEN)
  {
     xnee_print_error ("No protocol data choosen to record\n");
     xnee_print_error ("You can try out with xnee --all-events\n");
     exit(ret);
  }


  /* hey, we are fin(n)ished .... cloe down */
  xnee_close_down(xd);
  

  /* Since we are here, we can exit gracefully */
  exit(XNEE_OK); 
}







int
cnee_check_protocol(xnee_data *xd)
{
   int max;

   max = xnee_get_max_range(xd);

   
  /*
   * Check if default ranges was choosen
   *
   * everything option is to be  OBSOLETED
   */
  if ( xd->xnee_info->everything || xnee_is_all_events (xd)) 
  {
     if ( max > 0 )
     {
        xnee_print_error ("Error:\n%s, amibigous ranges\n", PACKAGE);
        xnee_print_error (" You can't use --all-events or --everything\n");
        xnee_print_error (" together with --event-ranges ..... \n");
        xnee_print_error (" For syntax description type %s --help\n",
                          PACKAGE);
        
        exit (XNEE_AMBIGOUS_CMD);
     }
     xnee_verbose((xd, "Choosing some of the default values as protocol\n"));
     xnee_record_select_default_protocol    (xd);
  }

  return (XNEE_OK);
}
