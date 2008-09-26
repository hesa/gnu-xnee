#include "test_setget.h"
#include "libxnee/xnee.h"

int fails;
int total;
int verbose;

int orig;
int ret;
int new;

char *orig_str;
char *ret_str;
char *new_str;

FILE *orig_fd;
FILE *ret_fd;
FILE *new_fd;


int main(int argc, char **argv)
{
  fails = 0 ;
  total = 0;
  verbose = 0;
xnee_data *xdl;

  if (argc>1) verbose = 1;

  printf ("Starting tests :) :) \n");

  /*  Get a new xnee_data structure  */
  /* call to xnee_init */

   
  xdl = xnee_new_xnee_data();
/*   xnee_init(xdl); */

  /* Set the program name */
  xnee_set_program_name (xdl, "libtest");

  /* Set the cli parameters */
  xnee_set_application_parameters (xdl, NULL);
  xnee_set_recorder (xdl);
  test_all(xdl);

  fprintf (stdout, "\tSucesss:  %d\n", total);
  fprintf (stdout, "\tFailures: %d\n", fails);

  xnee_close_down(xdl);

  return 0;
}
