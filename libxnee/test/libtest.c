/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *   Copyright (C) 2006, 2007, 2010
 *                        Henrik Sandklef 
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

#include "libxnee/xnee.h"
#include "libxnee/datastrings.h"
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
#include "libxnee/xnee_error.h"
#include "libxnee/xnee_display.h"
#include "libxnee/xnee_sem.h"
#include "libxnee/xnee_session.h"
#include "libxnee/xnee_fileop.h"
#include "libxnee/xnee_alloc.h"


int global_succ = 0;
int global_fail = 0;
xnee_data *xd ;

int test_datastrings()
{
  int nr;
  int test_succ=0;
  int test_fail=0;

  /*****************************************/
  printf ("\n Test 1: ");
  test_succ=0;
  test_fail=0;
  for (nr=1;xnee_print_request(nr)!=NULL;nr++)
    {
       char *rec_name = xnee_int2request(nr);
       int received   = xnee_request2int(rec_name);
       printf (".");
       if (received!=nr)
       {
          fprintf (stdout,"\t%.2d --> %20s --> %d \n",
                   nr,
                   rec_name,
                   received);
          test_fail++;
       }    
       else
       {
          test_succ++;
       }
    }


  printf ("\n Test 2: ");
  for (nr=2;xnee_print_event(nr)!=NULL;nr++)
  {
       char *rec_name = xnee_int2event(nr);
       int received   = xnee_event2int(rec_name);
       printf (".");
       if (received!=nr)
       {
          fprintf (stdout,"\t%.2d --> %20s --> %d \n",
                   nr,
                   rec_name,
                   received);
          test_fail++;
       }    
       else
       {
          test_succ++;
       }
  }
  
  printf ("\n Test 3: ");
  for (nr=2;xnee_int2error(nr)!=NULL;nr++)
  {
     char *rec_name = xnee_int2error(nr);
     int received   = xnee_error2int(rec_name);
     printf (".");
     if (received!=nr)
     {
        fprintf (stdout,"\t%.2d --> %20s --> %d \n",
                 nr,
                 rec_name,
                   received);
        test_fail++;
     }    
     else
     {
        test_succ++;
     }
  }
  global_succ+=test_succ;
  global_fail+=test_fail;

  return 0;
}

int test_xnee_data()
{
  int i ; 
  int ret;

  printf ("\nPreparing Test 4: ");
  /* Set the signal handler the libxnee's built in */ 
  (void) signal (SIGINT, signal_handler); 
  printf (".");
  

  /*  Get a new xnee_data structure  */
  xd = xnee_new_xnee_data();
  printf (".");

  /* Set the program name */
  xnee_set_program_name (xd, "libtest");
  printf (".");

  /* Set the cli parameters */
  xnee_set_application_parameters (xd, NULL);
  printf (".");


  if (xd!=NULL)
    {
      global_succ++;
    }
  else
    {
      fprintf (stdout, "failed to alloc xnee_data\n");
      global_fail++;
    }
  printf (".1");

  printf ("\n Test 4: ");
  
  for (i=0;i<10;i++)
  {
    printf ("  loop (%d)", i ); fflush(stdout);
/*     xnee_set_verbose(xd); */

    xnee_set_out_name (xd, "/tmp/libtest.xns") ; 

/*     xnee_parse_range (xd, XNEE_DEVICE_EVENT,   */
/* 		      "2-6");  */
    xnee_parse_range (xd, XNEE_DELIVERED_EVENT,  
		      "EnterNotify-MappingNotify"); 
       xnee_parse_range (xd, XNEE_REQUEST,  
		      "1-100"); 
    xnee_parse_range (xd, XNEE_ERROR,  
		      "1-17"); 
    
    xnee_parse_range (xd, XNEE_DELIVERED_EVENT, 
		      "ButtonPress-MotionNotify");
      /*
      xnee_set_key (xd, XNEE_GRAB_STOP, "s");
      xnee_set_key (xd, XNEE_GRAB_PAUSE, "p");
      xnee_set_key (xd, XNEE_GRAB_RESUME, "r");
    */

    xnee_set_events_max (xd, 50);
    xnee_set_data_max (xd, 0);

    xnee_set_recorder(xd); 
    /*    xnee_prepare(xd);*/

    fprintf(stderr, "Start xnee\n"); fflush(stderr);
    ret=xnee_start(xd);

    if (ret==0)
      {
	global_succ++;
       }
    else
      {
	 fprintf (stdout, "failed to start1 %d   %s)\n", 
		  ret,
		  xnee_get_err_description(ret));
	 global_fail++;
       }
     
    ret=xnee_renew_xnee_data(xd);
    if (ret==0)
      {
	global_succ++;
      }
    else
      {
	fprintf (stdout, "failed to renew xnee_data\n");
	global_fail++;
      }
  }
  
  printf ("\n Test 5: ");
  for (i=0;i<10;i++)
  {
    printf (".");

    xnee_set_out_name (xd, "/tmp/libtest.xns") ;
    xnee_parse_range (xd, XNEE_DELIVERED_EVENT,  
		      "EnterNotify-MappingNotify"); 
    xnee_parse_range (xd, XNEE_REQUEST,  
		      "1-100"); 
     /*
       xnee_parse_range (xd, XNEE_DEVICE_EVENT, 
       "ButtonPress-MotionNotify");
     */
    xnee_set_key (xd, XNEE_GRAB_STOP, "s");
    xnee_set_key (xd, XNEE_GRAB_PAUSE, "p");
    xnee_set_key (xd, XNEE_GRAB_RESUME, "r");

    xnee_set_recorder(xd);
    xnee_prepare(xd);
    xnee_set_events_max (xd, 0);
    xnee_set_data_max (xd, 10);
    ret=xnee_start(xd);

    if (ret==0)
      {
	global_succ++;
       }
    else
      {
	 fprintf (stdout, "failed to start2 %d   %s)\n", 
		  ret,
		  xnee_get_err_description(ret));
	 global_fail++;
       }
     
    ret=xnee_renew_xnee_data(xd);
    if (ret==0)
      {
	global_succ++;
      }
    else
      {
	fprintf (stdout, "failed to renew xnee_data\n");
	global_fail++;
      }
  }
  
  printf ("\n");
  return XNEE_OK;
}



int main()
{
  
  fprintf (stdout,"Starting test\n");

  test_datastrings();
  
  test_xnee_data();
  
#ifdef ETTA_SUTTIVA_NOLLA  
  fprintf (stdout,"Press return \n");
  getchar();

#endif

  /* hey, we are fin(n)ished .... cloe down */
  xnee_close_down(xd);

  fprintf (stdout, "\tSucesss:  %d\n", global_succ);
  fprintf (stdout, "\tFailures: %d\n", global_fail);


  /* Since we are here, we can exit gracefully */
  exit(XNEE_OK); 
}
