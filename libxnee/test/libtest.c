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
  char tmp[32];

  /*****************************************/
  test_succ=0;
  test_fail=0;
  fprintf (stderr, "\t\trequests\n");
  for (nr=1;xnee_print_request(nr)!=NULL;nr++)
    {
       char *rec_name = xnee_int2request(nr);
       int received   = xnee_request2int(rec_name);
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


  fprintf (stderr, "\t\tevents\n");
  for (nr=2;xnee_print_event(nr)!=NULL;nr++)
  {
       char *rec_name = xnee_int2event(nr);
       int received   = xnee_event2int(rec_name);
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
  
  fprintf (stderr, "\t\terrors\n");
  for (nr=2;xnee_int2error(nr)!=NULL;nr++)
  {
     char *rec_name = xnee_int2error(nr);
     int received   = xnee_error2int(rec_name);
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

  /* Set the signal handler the libxnee's built in */ 
  (void) signal (SIGINT, signal_handler); 
  

  /*  Get a new xnee_data structure  */
  xd = xnee_new_xnee_data();

  /* Set the program name */
  xnee_set_program_name (xd, "libtest");

  /* Set the cli parameters */
  xnee_set_application_parameters (xd, NULL);

  if (xd!=NULL)
    {
      global_succ++;
    }
  else
    {
      fprintf (stdout, "failed to alloc xnee_data\n");
      global_fail++;
    }

  for (i=0;i<1;i++)
  {
    xnee_set_verbose(xd);

    xnee_set_out_name (xd, "/tmp/libtest.xns") ; 
    xnee_unset_verbose(xd);
    xnee_parse_range (xd, XNEE_DELIVERED_EVENT,  
		      "EnterNotify-MappingNotify"); 
    xnee_parse_range (xd, XNEE_REQUEST,  
		      "1-100"); 
    xnee_parse_range (xd, XNEE_ERROR,  
		      "1-17"); 
    /*
      xnee_parse_range (xd, XNEE_DEVICE_EVENT, 
      "ButtonPress-MotionNotify");
      xnee_set_key (xd, XNEE_GRAB_STOP, "s");
      xnee_set_key (xd, XNEE_GRAB_PAUSE, "p");
      xnee_set_key (xd, XNEE_GRAB_RESUME, "r");
    */

    xnee_set_events_max (xd, 10);
    xnee_set_data_max (xd, 10);

    xnee_set_recorder(xd);
    xnee_prepare(xd);
    ret=xnee_start(xd);

    if (ret==0)
      {
	global_succ++;
       }
    else
      {
	 fprintf (stdout, "failed to start %d   %s)\n", 
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
  
  for (i=0;i<3;i++)
  {
    xnee_set_verbose(xd);

    xnee_set_out_name (xd, "/tmp/libtest.xns") ;
    xnee_unset_verbose(xd);
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
    xnee_set_events_max (xd, 10);
    xnee_set_data_max (xd, 10);

    xnee_set_recorder(xd);
    xnee_prepare(xd);
    ret=xnee_start(xd);

    if (ret==0)
      {
	global_succ++;
       }
    else
      {
	 fprintf (stdout, "failed to start %d   %s)\n", 
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
  
  
}



int main()
{
  
  fprintf (stdout,"Testing:\n");

  fprintf (stdout,"\tdatastrings\n");
  test_datastrings();
  
  fprintf (stdout,"\txnee_data\n");
  test_xnee_data();
  
  fprintf (stdout, "Sucesss:  %d\n", global_succ);
  fprintf (stdout, "Failures: %d\n", global_fail);

#ifdef ETTA_SUTTIVA_NOLLA  
  fprintf (stdout,"Press return \n");
  getchar();

#endif

  /* hey, we are fin(n)ished .... cloe down */
  xnee_close_down(xd);

  /* Since we are here, we can exit gracefully */
  exit(XNEE_OK); 
   
}
