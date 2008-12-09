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
  xEvent               ev_impl;
  int req_type ;
  int ev_type ;
  int i ;

  XRecordInterceptData *xrecintd  = &xrecintd_impl;
  XRecordDatum         *xrec_data = &xrec_data_impl;
  xResourceReq         *req       = &req_impl;
  xEvent               *ev        = &ev_impl;

  char *tmp_str;

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
  xnee_record_print_request(xd, NULL);   
  total++;
  xnee_human_print_request(xd, NULL);   
  total++;
  

  /* NOTE: bogus values */
  #define SET_BOGUES_REQ_VALUES(r)   \
  memset(xrecintd, 0, sizeof(XRecordInterceptData)); \
  memset(xrec_data, 0, sizeof(XRecordDatum)); \
  req_type     = r ; \
  req->reqType = 1 ; \
  req->length  = 1 ; \
  req->id      = 4 ; \
  xrecintd->server_time = 200; \
  xrec_data->req    = *(xReq*)&req;\
  xrec_data->type   = req_type;\
  xrecintd->data    = xrec_data;


 for (i=1;i<128;i++)
    {
      SET_BOGUES_REQ_VALUES(i);
      xnee_record_print_request(xd, xrecintd);  
      total++;

      SET_BOGUES_REQ_VALUES(i);
      xnee_human_print_request(xd, xrecintd);  
      total++;

      SET_BOGUES_REQ_VALUES(i);
      xnee_human_print_request_verbose(xd, xrecintd);  
      total++;
    }
  
  printf ("==================================================\n");

  #define SET_BOGUES_EV_VALUES(e)   \
  memset(xrecintd, 0, sizeof(XRecordInterceptData));  \
  memset(xrec_data, 0, sizeof(XRecordDatum));  \
  ev_type      = e ;  \
  ev->u.u.type = 0 ;   \
  ev->u.keyButtonPointer.state = 0 ;   \
  xrec_data->event  = *(xEvent*)&ev;  \
  xrec_data->type   = (unsigned char) ev_type ; \
  xrecintd->data    = xrec_data;  

  for (i=2;i<35;i++)
    {
      SET_BOGUES_EV_VALUES(i);
      xnee_human_print_event(xd, xrecintd);  
      total++;

      SET_BOGUES_EV_VALUES(i);
      xnee_human_print_event_verbose(xd, xrecintd);  
      total++;
    }
  
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


  fprintf (stdout, "\n");
  fprintf (stdout, "\tSucesss:  %d\n", total);
  fprintf (stdout, "\tFailures: %d\n", fails);

  xnee_close_down(xdl);

  
      
  return 0;


}

