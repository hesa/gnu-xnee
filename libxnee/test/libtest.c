#include <stdio.h>

#include "libxnee/xnee.h"
#include "libxnee/datastrings.h"

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
  if (xd!=NULL)
    {
      global_succ++;
    }
  else
    {
      fprintf (stdout, "failed to alloc xnee_data\n");
      global_fail++;
    }

  
  for (i=0;i<10;i++)
  {
    xnee_set_verbose(xd);
    xnee_set_recorder(xd);
    ret=xnee_start(xd);
    exit(0);
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
  
  
  /* start up the action set during parsing the commnd line */
  ret = xnee_start(xd);
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
}



int main()
{


  fprintf (stdout,"Testing:\n");


  fprintf (stdout,"\tdatastrings\n");
  test_datastrings();
  
  fprintf (stdout,"\txnee_data\n");
  test_xnee_data();
  
  
  fprintf (stdout,"Press return \n");
  getchar();

  printf ("Sucesss:  %d\n", global_succ);
  printf ("Failures: %d\n", global_fail);


  /* hey, we are fin(n)ished .... cloe down */
  xnee_close_down(xd);

  /* Since we are here, we can exit gracefully */
  exit(XNEE_OK); 
   
}
