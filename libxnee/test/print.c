#include "test_setget.h"
#include "libxnee/xnee.h"


int fails;
int total;
int verbose;

int 
test_printfuns(xnee_data *xd)
{
  int ret ;
  XRecordInterceptData xrecintd_impl;
  XRecordDatum         xrec_data_impl;
  xResourceReq         req_impl;
  int req_type ;

  XRecordInterceptData *xrecintd  = &xrecintd_impl;
  XRecordDatum         *xrec_data = &xrec_data_impl;
  xResourceReq         *req       = &req_impl;
  xReq                 tmp_req;
  xReq                *tmp_req_p;

  ret = xnee_print_event_info(xd);
  XNEE_TEST_ASSERT(ret, 0, "xnee_print_event_info");

  ret = xnee_print_error_info(xd);
  XNEE_TEST_ASSERT(ret, 0, "xnee_print_error_info");

  ret = xnee_print_request_info(xd);
  XNEE_TEST_ASSERT(ret, 0, "xnee_print_request_info");

  ret = xnee_print_reply_info(xd);
  XNEE_TEST_ASSERT(ret, 0, "xnee_print_reply_info");

  ret = xnee_print_data_info(xd);
  XNEE_TEST_ASSERT(ret, 0, "xnee_print_data_info");


  /* just make sure it links :) */
  xnee_verbose_event(xd, 18);
  total++;
  
  /* make sure it accepts NULL without crashing the app */
/*   xnee_record_print_request(xd, NULL);   */
  total++;

/*   memset(&xrecintd, 0, sizeof(XRecordInterceptData)); */
/*   memset(&xrec_data, 0, sizeof(XRecordDatum)); */
  /* NOTE: bogus values */
  req_type     = 1 ;
  req->reqType = 2 ;
  req->length  = 3 ;
  req->id      = 4 ;
  xrecintd->server_time = 200;

  tmp_req_p  = (xReq*)req;
  tmp_req    = *tmp_req_p;
  xrec_data->req    = tmp_req;
  xrec_data->type   = req_type;
  xrecintd->data    = (unsigned char) xrec_data;

  printf ("%d,%lu,%lu,%lu,%lu\n", 
		      req_type, 
		      req->reqType,
		      req->length,
		      req->id,
		      xrecintd->server_time);
  

  xnee_record_print_request(xd, xrecintd);  
  total++;

  
}

int main()
{
  xnee_data *xdl;

  fails = 0;
  total = 0;
  

  printf (" print functions tester\n");

  xdl = xnee_new_xnee_data();

  xnee_set_program_name (xdl, "print-tester");

  xnee_set_application_parameters (xdl, NULL);

  test_printfuns(xdl);

  fprintf (stdout, "\tSucesss:  %d\n", total);
  fprintf (stdout, "\tFailures: %d\n", fails);

  xnee_close_down(xdl);

  
      
  return 0;


}

