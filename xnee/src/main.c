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
  int max;
  xnee_data *xd ;


  
  
  (void) signal (SIGINT, signal_handler);

   /*
    *  
    */
  xd = xnee_new_xnee_data();

  /*
   * Init the structs. Use PACKAGE as a name for the program
   */
  xnee_init(xd, PACKAGE);
  
  /*
   * Init Recording variables
   * Since those are used when recording and replaying we 
   */
  xnee_record_init (xd);
  
  /*
   * Well .... parse the args
   *
   */
  xnee_parse_args (xd, argc, argv);

  /*
   * The value max will indicate how many ranges was specified
   * on the command line 
   */
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
	  xnee_print_error (" You can't use --all_events or --everything\n");
	  xnee_print_error (" together with --event_ranges ..... \n");
	  xnee_print_error (" For syntax description type %s --help\n",
			    PACKAGE);

	  exit (XNEE_AMBIGOUS_CMD);
	}
      xnee_verbose((xd, "Choosing some of the default values as protocol\n"));
      xnee_record_select_default_protocol    (xd);
    }


  /* 
   * Print settings 
   * only done if verbose mode  
   */
  xnee_print_distr_list(xd, NULL);
  
  xnee_record_print_record_range (xd, NULL);
  
  ret=xnee_setup_recordext (xd);

  if ( xnee_is_recorder(xd) )
    {
      if (ret==XNEE_NO_PROT_CHOOSEN)
	{
	  xnee_print_error ("No protocol data choosen to record\n");
	  xnee_print_error ("You can try out with xnee --all_events\n");
	  exit(ret);
	}
    }
  
  
  /* 
   * Test Displays and Extensions  
   *
   */
  xnee_setup_display (xd);
  


  /*
   * If no recording client, init xnee_sync 
   *
   */
  if ( ! xnee_is_recorder(xd) )
    {
      xnee_replay_init (xd, argv[0]);   
    }
  
  
  /*
   * Save repeat mode so we can reset it after we are done
   *
   */
  xnee_set_autorepeat (xd);
  


  /*
   * are we recording or are we replaying
   */
  if (xnee_is_recorder(xd)) 
    {
      
      /* 
       * Print settings 
       * if verbose mode that is 
       */
      xnee_record_print_record_range (xd, NULL);
      xnee_print_xnee_settings       (xd, NULL); 


      /*
       * Do we have XRecord extension on the display
       *
       */ 
      if (!xnee_has_record_extension(xd))
	{
	  xnee_verbose((xd, "Can't find Record extension\n"));
	  xnee_verbose((xd, "Look in the README file included in Xnee how to enable it\n"));
	  exit(XNEE_NO_REC_EXT);
	}
      xnee_setup_recording(xd);

      xnee_print_sys_info(xd, xnee_get_out_file (xd));
      xnee_print_xnee_settings (xd, xnee_get_out_file (xd)) ;
      xnee_record_print_record_range (xd, xnee_get_out_file (xd)) ;

      /*
       * At last. Time to enter the main loop
       *
       */
      if (xnee_get_loops_left(xd)!=0)
	{
	  xnee_verbose ((xd, "Entering main loop( recorder)\n"));

	  xnee_record_async(xd);
	}
    }
  else 
    {
      xnee_verbose((xd, " (replayer)\n"));
      /*
       * Do we have XTest extension on the display
       *
       */ 
      if (!xnee_has_xtest_extension(xd))
	{
	  exit(XNEE_NO_TEST_EXT);
	}
      
      /*
       * Do we have XRecord extension on the display
       *
       */ 
      if (!xnee_has_record_extension(xd))
	{
	  xnee_verbose((xd, "I can't find Record extension\n"));
	  xnee_verbose((xd, "Look in the README file how to enable it\n"));
	  xnee_verbose((xd, "However, I continue without doing syncing\n"));
	  /*	  xd->sync=False;*/
	  xnee_unset_sync (xd);
	}

      XTestGrabControl (xnee_get_control_display(xd), True);
      XTestGrabControl (xnee_get_data_display(xd), True);

      /*
       * At last. Time to enter the main loop
       * ... wait to set up recording until all META data from file is read 
       * Thanks: Janice Waddick 
       */
      xnee_verbose((xd, "Entering main loop (replayer)\n"));
      xnee_replay_main_loop(xd);
    }
  
  /*
   * Close everything down .... free memory, tell X server we are leaving ...
  if ( xd->recorder || xd->sync )
    {
      xnee_record_close_down(xd);
    }
   */

  xnee_reset_autorepeat (xd);

  xnee_close_down(xd);
  

  exit(XNEE_OK); 
}

