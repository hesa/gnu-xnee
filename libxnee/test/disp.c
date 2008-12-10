#include "test_setget.h"
#include "libxnee/xnee.h"



int fails;
int total;
int verbose;

int main()
{
  xnee_data *xdl;
  int ret ;
  Display *dpy;

  fails = 0;
  total = 0;
  

  printf (" display tester\n");


  xdl = xnee_new_xnee_data();

  xnee_set_program_name (xdl, "disp-tester");

  xnee_set_application_parameters (xdl, NULL);

  xnee_add_display_list(xdl, ":0");
  total++;
  xnee_add_display_list(xdl, ":0"); 
  total++;
  xnee_add_display_list(xdl, ":0,:0"); 
  total++;

  ret = xnee_add_display_str(":0", xdl);
  XNEE_TEST_ASSERT(ret, 0, "xnee_add_display_str");

  dpy = XOpenDisplay(NULL);

  ret = xnee_add_display(dpy, xdl);
  XNEE_TEST_ASSERT(ret, 0, "xnee_add_display");

  ret = xnee_print_distr_list (xdl, NULL);
  XNEE_TEST_ASSERT(ret, 0, "xnee_print_distr_list");
  

  fprintf (stdout, "\n");
  fprintf (stdout, "\tSucesss:  %d\n", total);
  fprintf (stdout, "\tFailures: %d\n", fails);

  xnee_close_down(xdl);

  

  return 0;
}


