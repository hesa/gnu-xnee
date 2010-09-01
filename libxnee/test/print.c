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

#include "test_setget.h"
#include "libxnee/xnee.h"



int fails;
int total;
int verbose;


int 
test_distributionfuns(xnee_data *xd)
{
  int ret;
  char disp_buf[256];

  /*
  xnee_add_display_list(xd, ":0");
  total++;
  xnee_add_display_list(xd, ":0"); 
  total++;
  xnee_add_display_list(xd, ":0,:0"); 
  total++;
  */
  xnee_add_display_list(xd, getenv("DISPLAY"));
  total++;
  xnee_add_display_list(xd, getenv("DISPLAY")); 
  total++;
  strcpy(disp_buf, getenv("DISPLAY"));
  strcat(disp_buf, ",");
  strcat(disp_buf, getenv("DISPLAY"));
  strcat(disp_buf, ",");
  strcat(disp_buf, getenv("DISPLAY"));
  xnee_add_display_list(xd, disp_buf); 
  total++;

  ret = xnee_print_distr_list (xd, NULL);
  XNEE_TEST_ASSERT(ret, 0, "xnee_print_distr_list");
  
  ret = xnee_print_distr_list (xd, xd->out_file);
  XNEE_TEST_ASSERT(ret, 0, "xnee_print_distr_list");
  
  
  return 0;
}

int 
test_rangefuns(xnee_data *xd)
{
  int ret;

  xnee_record_print_record_range (xd, NULL) ;
  total++;

  xnee_add_range(xd, XNEE_DELIVERED_EVENT, 2, 4);
  xnee_add_range(xd, XNEE_DEVICE_EVENT, 7, 14);
  xnee_add_range(xd, XNEE_REQUEST, 2, 4);
  xnee_add_range(xd, XNEE_REPLY, 2, 4);
  xnee_add_range(xd, XNEE_ERROR, 1, 1);

  xnee_prepare(xd);

  xnee_record_print_record_range (xd, NULL) ;
  total++;

  ret = xnee_print_ranges (xd, xd->out_file) ;
  XNEE_TEST_ASSERT(ret, 0, "xnee_record_print_ranges");

  ret = xnee_print_data_range_count (xd);
  XNEE_TEST_ASSERT(ret, 0, "xnee_print_data_range_count");

  xnee_replay_printbuffer_impl (xd );
  total++;

  xnee_version(xd);
  total++;

  ret = xnee_print_xnee_data(xd);
  XNEE_TEST_ASSERT(ret, 0, "xnee_print_xnee_data");

  ret = xnee_print_xnee_resource_settings (xd, xd->out_file ) ;
  XNEE_TEST_ASSERT(ret, 0, "xnee_print_xnee_resource_settings");
  
  xnee_store_mouse_pos (xd );
  total++;

  ret = xnee_print_xnee_settings(xd, xd->out_file);
  XNEE_TEST_ASSERT(ret, 0, "xnee_rec_print_xnee_settings");

  ret = xnee_print_data_str(xd, "MotionNotify", XNEE_EVENT);
  XNEE_TEST_ASSERT(ret, 0, "xnee_print_data_str");

  return 0;
}

int 
test_printfuns(xnee_data *xd)
{
  int ret ;
  XRecordInterceptData xrecintd_impl;
  XRecordDatum         xrec_data_impl;
  xResourceReq         req_impl;
  xEvent               ev_impl;
  xGenericReply        rep_impl;
  xError               err_impl;
  int req_type ;
  int ev_type ;
  int rep_type ;
  int i ;

  XRecordInterceptData *xrecintd  = &xrecintd_impl;
  XRecordDatum         *xrec_data = &xrec_data_impl;
  xResourceReq         *req       = &req_impl;
  xEvent               *ev        = &ev_impl;
  xGenericReply        *rep       = &rep_impl;
  xError               *err       = &err_impl;

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
  xrecintd->data    = (unsigned char*)xrec_data;


 for (i=1;i<128;i++)
    {
      printf("  request: %d\n", i);
      
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
  xrecintd->data    = (unsigned char*) xrec_data;  

  for (i=2;i<35;i++)
    {
      SET_BOGUES_EV_VALUES(i);
      xnee_human_print_event(xd, xrecintd);  
      total++;

      SET_BOGUES_EV_VALUES(i);
      xnee_human_print_event_verbose(xd, xrecintd);  
      total++;
    }


  memset(xrecintd, 0, sizeof(XRecordInterceptData));  
  memset(xrec_data, 0, sizeof(XRecordDatum));  
  rep_type = 0;
  rep->type = X_Reply;
  xrec_data->reply = *(xGenericReply*)&req ;
  xrecintd->data = (unsigned char*) xrec_data;

  xnee_human_print_reply (xd, xrecintd );
  total++;

  xnee_record_print_reply (xd, xrecintd );
  total++;

  err->type = X_Error;
  xrec_data->error = *(xError*)&err;
  xrecintd->data   = (unsigned char*) xrec_data;
  xnee_human_print_error (xd, xrecintd );
  total++;

  xnee_record_print_error (xd, xrecintd );
  total++;

  return 0;
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

  test_rangefuns(xdl);

  test_distributionfuns(xdl);

  xnee_set_verbose(xdl);
  xnee_verbose((xdl, "Just a simple string, printed from test code\n"));
  xnee_verbose((NULL, "Just a simple string, but with xnee_data arg set to NULL\n"));
  xnee_print_error("Simple test string in test code\n");
  xnee_unset_verbose(xdl);

  fprintf (stdout, "\n");
  fprintf (stdout, "\tSucesss:  %d\n", total);
  fprintf (stdout, "\tFailures: %d\n", fails);

  xnee_close_down(xdl);

  
      
  return 0;
}

