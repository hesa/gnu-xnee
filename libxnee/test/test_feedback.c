#include "test_setget.h"
#include "libxnee/xnee.h"
#include "libxnee/feedback.h"

int fails;
int total;
int verbose;

int 
test_setfeedback(xnee_data *xd)
{
  int ret ;
  const char *ptr;
  static char *xosd_test_font = "-bitstream-courier 10 pitch-*-*-*-*-*-*-*-*-*-*-*-*";
  /*" * -*-terminus-medium-r-*-*-*-120-*-*-*-*-*-*"; */
  /*
    no
  */

  ret = xnee_set_no_feedback(xd);
  XNEE_TEST_ASSERT(ret, 0, "xnee_set_no_feedback");

  ret = feedback(xd, "   ** Testing no feedback\n");
  XNEE_TEST_ASSERT(ret, 0, "feedback");

  ret = xnee_is_no_feedback(xd);
  XNEE_TEST_ASSERT(ret, 1, "xnee_is_no_feedback");


  /*
    stderr 
  */

  ret = xnee_set_stderr_feedback(xd);
  XNEE_TEST_ASSERT(ret, 0, "xnee_set_stderr_feedback");

  ret = feedback(xd, "   ** Testing stderr feedback\n");
  XNEE_TEST_ASSERT(ret, 0, "feedback");

  ret = xnee_is_stderr_feedback(xd);
  XNEE_TEST_ASSERT(ret, 1, "xnee_is_stderr_feedback");

  
  /*
    xosd
  */

  ret = xnee_set_xosd_feedback(xd);
  XNEE_TEST_ASSERT(ret, 0, "xnee_set_xosd_feedback");

  ret = feedback(xd, "   ** Testing xosd feedback\n");
  XNEE_TEST_ASSERT(ret, 0, "feedback");

  ret = xnee_is_xosd_feedback(xd);
  XNEE_TEST_ASSERT(ret, 1, "xnee_is_xosd_feedback");

  ptr = xnee_get_xosd_font_impl(xd);
  XNEE_TEST_ASSERT_DIFF(ptr, NULL, "xnee_get_xosd_font_impl");
  
  ret = xnee_set_xosd_font_impl(xd, xosd_test_font);
  XNEE_TEST_ASSERT(ret, 0, "xnee_set_xosd_font_impl");
  
  ptr = xnee_get_xosd_font_impl(xd);
  XNEE_TEST_ASSERT_DIFF(ptr, NULL, "xnee_get_xosd_font_impl");
  

  ret = feedback(xd, "   ** Testing xosd feedback again\n");
  XNEE_TEST_ASSERT(ret, 0, "feedback");

  usleep(2000*1000);

}

int main()
{
  xnee_data *xdl;

  fails = 0;
  total = 0;
  

  printf (" feedback tester\n");

  xdl = xnee_new_xnee_data();

  xnee_set_program_name (xdl, "feedbacker");

  xnee_set_application_parameters (xdl, NULL);

  test_setfeedback(xdl);

  fprintf (stdout, "\tSucesss:  %d\n", total);
  fprintf (stdout, "\tFailures: %d\n", fails);

  xnee_close_down(xdl);

  
      
  return 0;


}
