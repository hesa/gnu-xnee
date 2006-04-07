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

xnee_data *xd;

int main(int argc, char **argv)
{
  fails = 0 ;
  total = 0;
  verbose = 0;

  if (argc>1) verbose = 1;

  printf ("Starting tests\n");

  /*  Get a new xnee_data structure  */
  xd = (xnee_data *)xnee_new_xnee_data();

  /* Set the program name */
  xnee_set_program_name (xd, "libtest");

  /* Set the cli parameters */
  xnee_set_application_parameters (xd, NULL);

  test_all();

  fprintf (stdout, "\tSucesss:  %d\n", total);
  fprintf (stdout, "\tFailures: %d\n", fails);

  xnee_close_down(xd);

  return 0;
}
