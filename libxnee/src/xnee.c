/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *  Copyright (C) 1999-2007, 2010-2011, 2014 Henrik Sandklef 
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


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <dlfcn.h>

#include <sys/utsname.h>


#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/xnee_dl.h"
#include "libxnee/xnee_sem.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_fake.h"
#include "libxnee/datastrings.h"
#include "libxnee/xnee_grab.h"
#include "libxnee/xnee_km.h"
#include "libxnee/xnee_resolution.h"
#include "libxnee/xnee_resource.h"
#include "libxnee/xnee_callback.h"
#include "libxnee/xnee_range.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_session.h"
#include "libxnee/xnee_error.h"
#include "libxnee/xnee_display.h"
#include "libxnee/xnee_utils.h"
#include "libxnee/xnee_alloc.h"




/**************************************************************
 *                                                            *
 * xnee_check                                                 *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_check ( const char *arg, const char *long_arg , const char *short_arg ) 
{
  const char *tmp_long  = long_arg;
  const char *tmp_short = short_arg;
  
  if (tmp_short==NULL)
    {
      tmp_short = tmp_long;
    }

  /* returns 1 if arg was equal to any of long_arg or short_arg, 
     else it returns 0*/
   return (int) ( (strcmp ( arg, tmp_long)  == 0 ) || 
                  (strcmp ( arg, tmp_short) == 0 ) ) ; 
}




int
xnee_start(xnee_data *xd)
{
   int ret ;
   int i   ;
   int data_to_record = 0 ;

   if (xd==NULL)
     {
       return XNEE_NO_MAIN_DATA;
     }

   /* If just checking syntax,
      return OK ..... failure should have been 
      handled by normal parsing ..... */
   if (xnee_is_syntax_checker(xd))
     {
       return XNEE_OK;
     }

   ret = xnee_prepare(xd);
   XNEE_RETURN_IF_ERR(ret);

   xd->in_use = 1;

   /* grab all keys that have been specified */
   ret = xnee_grab_all_keys (xd);
   if (ret != XNEE_OK)
     {
       xnee_verbose((xd, "grab failure \n"));
       return ret;
     }

   if (xnee_get_interval (xd) != 0)
   {
     xnee_delay (xnee_get_interval (xd), "xnee:" );
   }
   

   /*
    * are we recording or are we replaying
    */
   if ( xnee_is_recorder(xd) != 0)  
     {
       /* Make sure we arerecording something */
       for (i=XNEE_EVENT; i<XNEE_NR_OF_TYPES; i++)
	 {
	   data_to_record += xnee_get_nr_of_data (i);
	 }
       if ( data_to_record == 0 )
	 {
	   return XNEE_NO_PROT_CHOOSEN;
	 }
       
       /* 
	* Print settings 
	* if verbose mode that is 
	*/
       xnee_print_xnee_settings       (xd, NULL); 
       xnee_record_print_record_range (xd, NULL);
       
       /*
	* Do we have XRecord extension on the display
	*
	*/ 
       if ( xnee_has_record_extension(xd) != XNEE_OK)
	 {
	   xnee_verbose((xd, "Can't find Record extension\n"));
	   xnee_verbose((xd, "Look in the README file included"));
	   xnee_verbose((xd, "in Xnee how to enable it\n"));
	   return(XNEE_NO_REC_EXT);
	 }
       ret = xnee_setup_recording(xd);
       XNEE_RETURN_IF_ERR (ret);
       
       ret = xnee_print_sys_info(xd, xnee_get_out_file (xd));
       XNEE_RETURN_IF_ERR (ret);
       
       xnee_print_xnee_settings (xd, xnee_get_out_file (xd)) ;
       xnee_record_print_record_range (xd, xnee_get_out_file (xd)) ;
       
       
       xnee_zero_events_recorded(xd);
       xnee_zero_data_recorded(xd);
       xnee_zero_time_recorded(xd);
       
       /*
        * At last. Time to enter the main loop
        *
        */
       if (xnee_more_to_record(xd)!=0)
	 {
	   xnee_verbose((xd, "Entering main loop( recorder)\n"));
	   ret = xnee_record_async(xd);
	   
	   if ( (ret != XNEE_OK) &&  (ret != XNEE_OK_LEAVE) )
	     {
	       return ret;
	     }
	 }
     }
   else if ( xnee_is_replayer(xd) != 0 )
     {
       xnee_verbose((xd, " (replayer)\n"));
       /*
	* Do we have XTest extension on the display
	*
	*/ 
       if ( xnee_has_xtest_extension(xd) == 0)
	 {
	   return(XNEE_NO_TEST_EXT);
	 }
       
       /*
	* Do we have XRecord extension on the display
	*
	*/ 
       if ( xnee_has_record_extension(xd) != XNEE_OK)
	 {
	   xnee_verbose((xd, "I can't find Record extension\n"));
	   xnee_verbose((xd, "Look in the README file how to enable it\n"));
	   xnee_verbose((xd, "However, I continue without doing syncing\n"));
	   /*	  xd->sync=False;*/
	   ret = xnee_unset_sync (xd);
	   XNEE_RETURN_IF_ERR (ret);
	 }
       
       /*@ignore@*/
       XTestGrabControl (xnee_get_control_display(xd), True);
       XTestGrabControl (xnee_get_data_display(xd), True);
       /*@end@*/

       /*       ret = xnee_rep_prepare(xd); */
       /*       XNEE_RETURN_IF_ERR (ret); */
       
       xnee_record_print_record_range (xd, stderr) ;
       
       /*
       * At last. Time to enter the main loop
       * ... wait to set up recording until all META data from file is read 
       * Thanks: Janice Waddick 
       */
      
      xnee_verbose((xd, "Entering main loop (replayer)\n"));
      ret = xnee_replay_main_loop(xd, XNEE_REPLAY_READ_REPLAY_DATA);
      xnee_verbose((xd,"finished replaying\n"));
      if (ret != XNEE_OK)
	{
	  return ret;
	}
      

    }
  else if ( xnee_is_retyper(xd) != 0 )
    {
      if ( xnee_type_file(xd) != 0 )
	{
	  xnee_print_error ("Unable to open retype file\n");
	}
      xnee_verbose((xd,"finished reptyping\n"));
    }
  else
    {
      return XNEE_MODE_NOT_SET;
    }
   xnee_verbose((xd, "xnee_start : ungrab -----> \n"));
   ret = xnee_ungrab_keys (xd);
   XNEE_RETURN_IF_ERR (ret);

   xnee_verbose((xd, "xnee_start : ungrab <---- \n"));
   ret = xnee_reset_autorepeat (xd); 
   XNEE_RETURN_IF_ERR (ret);
   
   ret = xnee_renew_xnee_data(xd);
   XNEE_RETURN_IF_ERR (ret);

   return (XNEE_OK);
}


