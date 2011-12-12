/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *   Copyright (C) 2006, 2007, 2010, 2011
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
#include "libxnee/xnee_session.h"
#include "libxnee/xnee_setget.h"

#define DUMMY_FILE_NO ((FILE*)0)
#define DUMMY_INT ((int)177)

int i ;

#define BEGIN_LOOP for(i=0;i<1000;i++) { 
#define END_LOOP }

int test_all(xnee_data *xd) 
{
  /*
    Doing what's done in xnee_start
    .... but not actually calling xnee_start
  ret = xnee_init(xd);
  XNEE_RETURN_IF_ERR(ret);
   */


  ret = xnee_prepare(xd);
  XNEE_RETURN_IF_ERR(ret);



  /* Testing:
   *
   *           xnee_set_display_name
   *
   */
  BEGIN_LOOP
  xnee_set_display_name (xd, "einar");
  orig_str = xnee_get_display_name(xd);
  
  ret      = xnee_set_display_name (xd, "dumm_str"); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_display_name"); 
  
  new_str  = xnee_get_display_name(xd);
  XNEE_TEST_ASSERT_STR(new_str, "dumm_str","xnee_set_display_name");

  END_LOOP


#ifdef USE_LATER



  /* Testing:
   *
   *           xnee_set_out_file
   *
   */
    xnee_set_out_file(xd, 0); 
  orig_fd  = xnee_get_out_file(xd);
  
  ret      = xnee_set_out_file (xd, DUMMY_FILE_NO); 
  
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_out_file"); 
  
  new_fd   = xnee_get_out_file(xd);
  XNEE_TEST_ASSERT(new_fd , DUMMY_FILE_NO,"xnee_set_out_file"); 
  XNEE_TEST_ASSERT_DIFF(orig_fd , new_fd ,"xnee_set_out_file");

  ret      = xnee_set_out_file (xd, DUMMY_FILE_NO);
  XNEE_TEST_ASSERT(new_fd , orig_fd , "xnee_set_out_file");

  /* Testing:
   *
   *           xnee_set_out_name
   *
   */
  orig_str = xnee_get_out_name(xd);
  
  ret      = xnee_set_out_name (xd, "dc_str"); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_out_name"); 
  
  new_str  = xnee_get_out_name(xd);
  XNEE_TEST_ASSERT_STR(new_str, "dc_str","xnee_set_out_name"); /* ADDITIONAL TEST */ 
  XNEE_TEST_ASSERT_DIFF_STR(orig_str, new_str,"xnee_set_out_name");
  ret      = xnee_set_out_name (xd, "dc_str");
  XNEE_TEST_ASSERT_STR(new_str, orig_str, "xnee_set_out_name");

  /* Testing:
   *
   *           xnee_set_rt_file
   *
   */
  orig_fd  = xnee_get_rt_file(xd);
  
  ret      = xnee_set_rt_file (xd, DUMMY_FILE_NO); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_rt_file"); 
  
  new_fd   = xnee_get_rt_file(xd);
  XNEE_TEST_ASSERT(new_fd , DUMMY_FILE_NO,"xnee_set_rt_file"); /* ADDITIONAL TEST */ 
  XNEE_TEST_ASSERT_DIFF(orig_fd , new_fd ,"xnee_set_rt_file");
  ret      = xnee_set_rt_file (xd, DUMMY_FILE_NO);
  XNEE_TEST_ASSERT(new_fd , orig_fd , "xnee_set_rt_file");

  /* Testing:
   *
   *           xnee_set_rt_name
   *
   */
  orig_str = xnee_get_rt_name(xd);
  
  ret      = xnee_set_rt_name (xd, "dc_str"); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_rt_name"); 
  
  new_str  = xnee_get_rt_name(xd);
  XNEE_TEST_ASSERT_STR(new_str, "dc_str","xnee_set_rt_name"); /* ADDITIONAL TEST */ 
  XNEE_TEST_ASSERT_DIFF_STR(orig_str, new_str,"xnee_set_rt_name");
  ret      = xnee_set_rt_name (xd, "dc_str");
  XNEE_TEST_ASSERT_STR(new_str, orig_str, "xnee_set_rt_name");

  /* Testing:
   *
   *           xnee_set_err_file
   *
   */
  orig_fd  = xnee_get_err_file(xd);
  
  ret      = xnee_set_err_file (xd, DUMMY_FILE_NO); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_err_file"); 
  
  new_fd   = xnee_get_err_file(xd);
  XNEE_TEST_ASSERT(new_fd , DUMMY_FILE_NO,"xnee_set_err_file"); /* ADDITIONAL TEST */ 
  XNEE_TEST_ASSERT_DIFF(orig_fd , new_fd ,"xnee_set_err_file");
  ret      = xnee_set_err_file (xd, DUMMY_FILE_NO);
  XNEE_TEST_ASSERT(new_fd , orig_fd , "xnee_set_err_file");

  /* Testing:
   *
   *           xnee_set_err_name
   *
   */
  orig_str = xnee_get_err_name(xd);
  
  ret      = xnee_set_err_name (xd, "dc_str"); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_err_name"); 
  
  new_str  = xnee_get_err_name(xd);
  XNEE_TEST_ASSERT_STR(new_str, "dc_str","xnee_set_err_name"); /* ADDITIONAL TEST */ 
  XNEE_TEST_ASSERT_DIFF_STR(orig_str, new_str,"xnee_set_err_name");
  ret      = xnee_set_err_name (xd, "dc_str");
  XNEE_TEST_ASSERT_STR(new_str, orig_str, "xnee_set_err_name");

  /* Testing:
   *
   *           xnee_set_rc_file
   *
   */
  orig_fd  = xnee_get_rc_file(xd);
  
  ret      = xnee_set_rc_file (xd, DUMMY_FILE_NO); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_rc_file"); 
  
  new_fd   = xnee_get_rc_file(xd);
  XNEE_TEST_ASSERT(new_fd , DUMMY_FILE_NO,"xnee_set_rc_file"); /* ADDITIONAL TEST */ 
  XNEE_TEST_ASSERT_DIFF(orig_fd , new_fd ,"xnee_set_rc_file");
  ret      = xnee_set_rc_file (xd, DUMMY_FILE_NO);
  XNEE_TEST_ASSERT(new_fd , orig_fd , "xnee_set_rc_file");

  /* Testing:
   *
   *           xnee_set_rc_name
   *
   */
  orig_str = xnee_get_rc_name(xd);
  
  ret      = xnee_set_rc_name (xd, "dumm_str"); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_rc_name"); 
  
  new_str  = xnee_get_rc_name(xd);
  XNEE_TEST_ASSERT_STR(new_str, "dumm_str","xnee_set_rc_name"); /* ADDITIONAL TEST */ 
  XNEE_TEST_ASSERT_DIFF_STR(orig_str, new_str,"xnee_set_rc_name");
  ret      = xnee_set_rc_name (xd, "dumm_str");
  XNEE_TEST_ASSERT_STR(new_str, orig_str, "xnee_set_rc_name");

  /* Testing:
   *
   *           xnee_set_data_file
   *
   */
  orig_fd  = xnee_get_data_file(xd);
  
  ret      = xnee_set_data_file (xd, DUMMY_FILE_NO); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_data_file"); 
  
  new_fd   = xnee_get_data_file(xd);
  XNEE_TEST_ASSERT(new_fd , DUMMY_FILE_NO,"xnee_set_data_file"); /* ADDITIONAL TEST */ 
  XNEE_TEST_ASSERT_DIFF(orig_fd , new_fd ,"xnee_set_data_file");
  ret      = xnee_set_data_file (xd, DUMMY_FILE_NO);
  XNEE_TEST_ASSERT(new_fd , orig_fd , "xnee_set_data_file");

  /* Testing:
   *
   *           xnee_set_data_name
   *
   */
  orig_str = xnee_get_data_name(xd);
  
  ret      = xnee_set_data_name (xd, "dumm_str"); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_data_name"); 
  
  new_str  = xnee_get_data_name(xd);
  XNEE_TEST_ASSERT_STR(new_str, "dumm_str","xnee_set_data_name"); /* ADDITIONAL TEST */ 
  XNEE_TEST_ASSERT_DIFF_STR(orig_str, new_str,"xnee_set_data_name");

  ret      = xnee_set_data_name (xd, "dumm_str");
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_data_name"); 
  XNEE_TEST_ASSERT_STR(new_str, orig_str, "xnee_set_data_name");


#endif /* USE_LATER */

  /* Testing:
   *
   *           xnee_set_verbose
   *
   */
  BEGIN_LOOP
  xnee_unset_verbose(xd);
  orig     = xnee_get_verbose(xd);
  
  ret      = xnee_set_verbose (xd); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_verbose"); 
  
  new        = xnee_get_verbose(xd);
  XNEE_TEST_ASSERT_DIFF(orig    , new      ,"xnee_set_verbose");
  ret      = xnee_unset_verbose (xd);
  new        = xnee_get_verbose(xd);
  XNEE_TEST_ASSERT(new , orig    , "xnee_set_verbose");
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_verbose"); 
  END_LOOP


  /* Testing:
   *
   *           xnee_set_buf_verbose
   *
   */
    BEGIN_LOOP
  xnee_unset_buf_verbose (xd); 
  orig     = xnee_get_buf_verbose(xd);
  
  ret      = xnee_set_buf_verbose (xd); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_buf_verbose"); 
  
  new        = xnee_get_buf_verbose(xd);
  XNEE_TEST_ASSERT_DIFF(orig    , new      ,"xnee_set_buf_verbose");

  ret      = xnee_unset_buf_verbose (xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_buf_verbose"); 
  new        = xnee_get_buf_verbose(xd);
  XNEE_TEST_ASSERT(new      , orig    , "xnee_set_buf_verbose");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_all_clients
   *
   */
    BEGIN_LOOP
  xnee_set_all_clients(xd);
  orig     = xnee_get_all_clients(xd);
  
  ret      = xnee_unset_all_clients (xd); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_all_clients"); 
  
  new        = xnee_get_all_clients(xd);
  XNEE_TEST_ASSERT_DIFF(orig    , new      ,"xnee_set_all_clients");

  ret      = xnee_set_all_clients (xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_all_clients"); 
  new        = xnee_get_all_clients(xd);
  XNEE_TEST_ASSERT(new      , orig    , "xnee_set_all_clients");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_sync
   *
   */
    BEGIN_LOOP
  xnee_set_sync(xd);
  orig     = xnee_get_sync(xd);
  
  ret      = xnee_unset_sync (xd); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_sync"); 
  
  new        = xnee_get_sync(xd);
  XNEE_TEST_ASSERT_DIFF(orig    , new      ,"xnee_set_sync");

  ret      = xnee_set_sync (xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_sync"); 
  new        = xnee_get_sync(xd);
  XNEE_TEST_ASSERT(new      , orig    , "xnee_set_sync");
  END_LOOP


  /* Testing:
   *
   *           xnee_set_mode
   *
   */
    BEGIN_LOOP
  xnee_set_mode(xd, 0);
  orig     = xnee_get_mode(xd);
  
  ret      = xnee_set_mode (xd, DUMMY_INT); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_mode"); 
  
  new        = xnee_get_mode(xd);

  XNEE_TEST_ASSERT(new          , DUMMY_INT,"xnee_set_mode"); /* ADDITIONAL TEST */ 
  XNEE_TEST_ASSERT_DIFF(orig    , new      ,"xnee_set_mode");

  ret      = xnee_set_mode (xd, 0);
  new        = xnee_get_mode(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_mode"); 
  XNEE_TEST_ASSERT(new      , orig    , "xnee_set_mode");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_replayer
   *
   */
    BEGIN_LOOP
  xnee_set_replayer(xd);
  orig     = xnee_is_replayer(xd);
  
  ret      = xnee_set_recorder (xd); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_replayer"); 
  
  new        = xnee_is_replayer(xd);
  XNEE_TEST_ASSERT_DIFF(orig    , new      ,"xnee_set_replayer");

  ret       = xnee_set_replayer (xd);
  new        = xnee_is_replayer(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_replayer"); 
  XNEE_TEST_ASSERT(new      , orig    , "xnee_set_replayer");
  END_LOOP


  /* Testing:
   *
   *           xnee_set_recorder
   *
   */
    BEGIN_LOOP
  xnee_set_recorder(xd);
  orig     = xnee_is_recorder(xd);
  
  ret      = xnee_set_replayer (xd); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_recorder"); 
  
  new        = xnee_is_recorder(xd);
  XNEE_TEST_ASSERT_DIFF(orig    , new      ,"xnee_set_recorder");

  ret      = xnee_set_recorder (xd);
  new        = xnee_is_recorder(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_recorder"); 
  XNEE_TEST_ASSERT(new      , orig    , "xnee_set_recorder");
  END_LOOP


  /* Testing:
   *
   *           xnee_set_cont
   *
   */
    BEGIN_LOOP
  xnee_set_cont(xd);
  orig     = xnee_get_cont(xd);
  
  ret      = xnee_unset_cont (xd); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_cont"); 
  
  new        = xnee_get_cont(xd);
  XNEE_TEST_ASSERT_DIFF(orig    , new      ,"xnee_set_cont");

  ret        = xnee_set_cont (xd);
  new        = xnee_get_cont(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_mode"); 
  XNEE_TEST_ASSERT(new      , orig    , "xnee_set_cont");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_exec_prog
   *
   */
    BEGIN_LOOP
  xnee_set_exec_prog(xd, "str1");
  orig_str = xnee_get_exec_prog(xd);
  
  ret      = xnee_set_exec_prog (xd, "dumm_str"); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_exec_prog"); 
  XNEE_TEST_ASSERT_STR(orig_str, "dumm_str","xnee_set_exec_prog"); 
  END_LOOP

  /* Testing:
   *
   *           xnee_set_force_replay
   *
   */
    BEGIN_LOOP
  xnee_set_force_replay(xd);
  orig     = xnee_get_force_replay(xd);
  
  ret      = xnee_unset_force_replay (xd); 
  new      = xnee_get_force_replay(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_force_replay"); 
  XNEE_TEST_ASSERT_DIFF(orig, new, "xnee_set_force_replay"); 
  
  ret      = xnee_set_force_replay(xd);
  new      = xnee_get_force_replay(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_force_replay"); 
  XNEE_TEST_ASSERT(new      , orig    , "xnee_set_force_replay");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_first_last
   *
   */
    BEGIN_LOOP
  xnee_set_first_last(xd);
  orig     = xnee_get_first_last(xd);
  
  ret      = xnee_unset_first_last (xd); 
  new      = xnee_get_first_last(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_first_last"); 
  XNEE_TEST_ASSERT_DIFF(orig    , new      ,"xnee_set_first_last");
  
  ret      = xnee_set_first_last (xd);
  new      = xnee_get_first_last(xd);
  XNEE_TEST_ASSERT(orig, new      ,"xnee_set_first_last");
  XNEE_TEST_ASSERT(new      , orig    , "xnee_set_first_last");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_events_max
   *
   */
    BEGIN_LOOP
  xnee_set_events_max (xd, 0); 
  orig     = xnee_get_events_max(xd);
  
  ret      = xnee_set_events_max (xd, DUMMY_INT); 
  new      = xnee_get_events_max(xd);
  XNEE_TEST_ASSERT_DIFF(new, orig, "xnee_set_events_max"); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_events_max"); 
  
  ret      = xnee_set_events_max (xd, 0);
  new        = xnee_get_events_max(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_events_max"); 
  XNEE_TEST_ASSERT(new      , orig    , "xnee_set_events_max");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_data_max
   *
   */
    BEGIN_LOOP
  xnee_set_data_max (xd, 0); 
  orig     = xnee_get_data_max(xd);
  
  ret      = xnee_set_data_max (xd, DUMMY_INT); 
  new      = xnee_get_data_max(xd);
  XNEE_TEST_ASSERT_DIFF(new, orig, "xnee_set_data_max"); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_data_max"); 
  
  ret      = xnee_set_data_max (xd, 0);
  new        = xnee_get_data_max(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_data_max"); 
  XNEE_TEST_ASSERT(new      , orig    , "xnee_set_data_max");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_time_max
   *
   */
    BEGIN_LOOP
  xnee_set_time_max (xd, 0); 
  orig     = xnee_get_time_max(xd);
  
  ret      = xnee_set_time_max (xd, DUMMY_INT); 
  new      = xnee_get_time_max(xd);
  XNEE_TEST_ASSERT_DIFF(new, orig, "xnee_set_time_max"); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_time_max"); 
  
  ret      = xnee_set_time_max (xd, 0);
  new        = xnee_get_time_max(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_time_max"); 
  XNEE_TEST_ASSERT(new      , orig    , "xnee_set_time_max");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_interval
   *
   */
    BEGIN_LOOP
  xnee_set_interval (xd, 0); 
  orig     = xnee_get_interval(xd);
  
  ret      = xnee_set_interval (xd, DUMMY_INT); 
  new      = xnee_get_interval(xd);
  XNEE_TEST_ASSERT_DIFF(new, orig, "xnee_set_interval"); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_interval"); 
  
  ret      = xnee_set_interval (xd, 0);
  new        = xnee_get_interval(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_interval"); 
  XNEE_TEST_ASSERT(new      , orig    , "xnee_set_interval_max");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_human_printout
   *
   */


    BEGIN_LOOP
    ;
  xnee_set_recorder(xd);
  xnee_init(xd);
  xnee_set_recorder(xd);
  xnee_set_human_printout(xd);
  orig     = xnee_is_human_printout(xd);
  ret      = xnee_set_xnee_printout(xd); 
  new      = xnee_is_human_printout(xd);

  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_human_printout"); 
  XNEE_TEST_ASSERT_DIFF(orig, new  ,"xnee_set_human_printout");
  
  ret      = xnee_set_human_printout(xd);
  new      = xnee_is_human_printout(xd);
  XNEE_TEST_ASSERT(orig    , new      ,"xnee_set_human_printout");
  XNEE_TEST_ASSERT(new      , orig    , "xnee_set_human_printout");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_xnee_printout
   *
   */
    BEGIN_LOOP
  xnee_set_xnee_printout(xd);
  orig     = xnee_is_xnee_printout(xd);
  
  ret      = xnee_set_human_printout(xd); 
  new     = xnee_is_xnee_printout(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_xnee_printout"); 
  XNEE_TEST_ASSERT_DIFF(orig, new  ,"xnee_set_xnee_printout");
  
  ret      = xnee_set_xnee_printout(xd);
  new      = xnee_is_xnee_printout(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_xnee_printout"); 
  XNEE_TEST_ASSERT(new      , orig    , "xnee_set_xnee_printout");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_replay_speed_str
   *
   */
    BEGIN_LOOP
  xnee_set_replay_speed(xd, 120);
  orig = xnee_get_replay_speed(xd);
  
  ret      = xnee_set_replay_speed_str (xd, "456"); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_replay_speed_str"); 
  
  new  = xnee_get_replay_speed(xd);
  XNEE_TEST_ASSERT(new, 456,"xnee_set_replay_speed_str"); 
  XNEE_TEST_ASSERT_DIFF(orig, new,"xnee_set_replay_speed_str");

  ret      = xnee_set_replay_speed_str (xd, "120");
  new      = xnee_get_replay_speed(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_replay_speed_str"); 
  XNEE_TEST_ASSERT(new, orig, "xnee_set_replay_speed_str");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_replay_speed
   *
   */
    BEGIN_LOOP
  xnee_set_replay_speed(xd, 120 );
  orig     = xnee_get_replay_speed(xd);
  
  ret      = xnee_set_replay_speed (xd, DUMMY_INT); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_replay_speed"); 
  
  new        = xnee_get_replay_speed(xd);END_LOOP

  XNEE_TEST_ASSERT(new      , DUMMY_INT,"xnee_set_replay_speed"); /* ADDITIONAL TEST */ 
  BEGIN_LOOP

  ret      = xnee_set_replay_speed (xd, 120);
  new      = xnee_get_replay_speed(xd);
  XNEE_TEST_ASSERT(orig    , new      ,"xnee_set_replay_speed");
  XNEE_TEST_ASSERT(new      , orig    , "xnee_set_replay_speed");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_store_mouse_pos
   *
   */
    BEGIN_LOOP
  xnee_set_store_mouse_pos(xd); 
  orig     = xnee_is_store_mouse_pos(xd);
  
  ret      = xnee_unset_store_mouse_pos(xd); 
  new      = xnee_is_store_mouse_pos(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_store_mouse_pos"); 
  XNEE_TEST_ASSERT_DIFF(orig    , new      ,"xnee_set_store_mouse_pos");
  
  ret      = xnee_set_store_mouse_pos(xd);
  new      = xnee_is_store_mouse_pos(xd);
  XNEE_TEST_ASSERT(orig    , new      ,"xnee_set_store_mouse_pos");
  XNEE_TEST_ASSERT(new      , orig    , "xnee_set_store_mouse_pos");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_program_name
   *
   */
    BEGIN_LOOP
  ret      = xnee_set_program_name (xd, "dumm_str"); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_program_name"); 
  
  new_str  = xnee_get_program_name(xd);
  XNEE_TEST_ASSERT_STR(new_str, "dumm_str","xnee_set_program_name"); 

  ret      = xnee_set_program_name (xd, "hej");
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_program_name"); 

  END_LOOP

  /* Testing:
   *
   *           xnee_set_replay_offset_str
   *
   */
    BEGIN_LOOP
  {
    int orig_x;
    int orig_y;
    int new_x;
    int new_y;

    xnee_set_replay_offset_str (xd, "34,12"); 
    orig_x = xnee_get_replay_offset_x(xd);
    orig_y = xnee_get_replay_offset_y(xd);
    XNEE_TEST_ASSERT(orig_x, 34,"xnee_set_replay_offset_str_x"); 
    XNEE_TEST_ASSERT(orig_y, 12,"xnee_set_replay_offset_str_x"); 
  
    ret      = xnee_set_replay_offset_str (xd, "12,34"); 
    new_x = xnee_get_replay_offset_x(xd);
    new_y = xnee_get_replay_offset_y(xd);
    XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_replay_offset_str"); 
    XNEE_TEST_ASSERT_DIFF(orig_x, new_x,"xnee_set_replay_offset_str");
    XNEE_TEST_ASSERT_DIFF(orig_y, new_y,"xnee_set_replay_offset_str");
    
    ret      = xnee_set_replay_offset_str (xd, "34,12"); 
    new_x  = xnee_get_replay_offset_x(xd);
    new_y  = xnee_get_replay_offset_y(xd);
    XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_replay_offset_str"); 
    XNEE_TEST_ASSERT(new_x, orig_x,"xnee_set_replay_offset_str_x"); 
    XNEE_TEST_ASSERT(new_y, orig_y,"xnee_set_replay_offset_str_y"); 
  }    

  END_LOOP

  /* Testing:
   *
   *           xnee_set_new_window_pos_value
   *
   */
    BEGIN_LOOP
    xnee_set_new_window_pos_value (xd, DUMMY_INT-1); 
  orig     = xnee_get_new_window_pos_value(xd);
  
  ret      = xnee_set_new_window_pos_value (xd, DUMMY_INT); 
  new        = xnee_get_new_window_pos_value(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_new_window_pos_value"); 
  XNEE_TEST_ASSERT_DIFF(orig    , new      ,"xnee_set_new_window_pos_value");
  
  ret      = xnee_set_new_window_pos_value (xd, DUMMY_INT-1); 
  new        = xnee_get_new_window_pos_value(xd);
  XNEE_TEST_ASSERT(orig    , new      ,"xnee_set_new_window_pos_value");
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_new_window_pos_value"); 

  END_LOOP

  /* Testing:
   *
   *           xnee_set_recall_window_pos
   *
   */
    BEGIN_LOOP
  xnee_set_recall_window_pos(xd);
  orig  = xnee_get_recall_window_pos(xd);
  
  ret   = xnee_unset_recall_window_pos (xd); 
  new  = xnee_get_recall_window_pos(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_recall_window_pos"); 
  XNEE_TEST_ASSERT_DIFF(orig    , new      ,"xnee_set_recall_window_pos");
  
  ret      = xnee_set_recall_window_pos(xd);
  new      = xnee_get_recall_window_pos (xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_recall_window_pos"); 
  XNEE_TEST_ASSERT(new      , orig    , "xnee_set_recall_window_pos");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_project_name
   *
   */
    BEGIN_LOOP
  ret      = xnee_set_project_name(xd, "cdtrs"); 
  orig_str = xnee_get_project_name(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_project_name"); 
  XNEE_TEST_ASSERT_DIFF_STR(orig_str, "cdtrs","xnee_set_project_name");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_project_descr
   *
   */
    BEGIN_LOOP
  ret      = xnee_set_project_descr(xd, "cdtrs"); 
  orig_str = xnee_get_project_descr(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_project_descr"); 
  XNEE_TEST_ASSERT_DIFF_STR(orig_str, "cdtrs","xnee_set_project_descr");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_creat_date
   *
   */
    BEGIN_LOOP
  ret      = xnee_set_creat_date(xd, "cdtrs"); 
  orig_str = xnee_get_creat_date(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_creat_date"); 
  XNEE_TEST_ASSERT_DIFF_STR(orig_str, "cdtrs","xnee_set_creat_date");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_creat_program
   *
   */
    BEGIN_LOOP
  ret      = xnee_set_creat_program(xd, "cdtrs"); 
  orig_str = xnee_get_creat_program(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_creat_program"); 
  XNEE_TEST_ASSERT_DIFF_STR(orig_str, "cdtrs","xnee_set_creat_program");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_creat_prog_vers
   *
   */
    BEGIN_LOOP
  ret      = xnee_set_creat_prog_vers(xd, "cdtrs"); 
  orig_str = xnee_get_creat_prog_vers(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_creat_prog_vers"); 
  XNEE_TEST_ASSERT_DIFF_STR(orig_str, "cdtrs","xnee_set_creat_prog_vers");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_last_date
   *
   */
    BEGIN_LOOP
  ret      = xnee_set_last_date(xd, "cdtrs"); 
  orig_str = xnee_get_last_date(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_last_date"); 
  XNEE_TEST_ASSERT_DIFF_STR(orig_str, "cdtrs","xnee_set_last_date");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_last_program
   *
   */
    BEGIN_LOOP
  ret      = xnee_set_last_program(xd, "cdtrs"); 
  orig_str = xnee_get_last_program(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_last_program"); 
  XNEE_TEST_ASSERT_DIFF_STR(orig_str, "cdtrs","xnee_set_last_program");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_last_prog_vers
   *
   */
    BEGIN_LOOP
  ret      = xnee_set_last_prog_vers(xd, "cdtrs"); 
  orig_str = xnee_get_last_prog_vers(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_last_prog_vers"); 
  XNEE_TEST_ASSERT_DIFF_STR(orig_str, "cdtrs","xnee_set_last_prog_vers");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_author_name
   *
   */
    BEGIN_LOOP
  ret      = xnee_set_author_name(xd, "cdtrs"); 
  orig_str = xnee_get_author_name(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_author_name"); 
  XNEE_TEST_ASSERT_DIFF_STR(orig_str, "cdtrs","xnee_set_author_name");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_author_email
   *
   */
    BEGIN_LOOP
  ret      = xnee_set_author_email(xd, "cdtrs"); 
  orig_str = xnee_get_author_email(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_author_email"); 
  XNEE_TEST_ASSERT_DIFF_STR(orig_str, "cdtrs","xnee_set_author_email");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_all_clients_str
   *
   */
    BEGIN_LOOP
  xnee_set_all_clients(xd);
  orig = xnee_get_all_clients(xd);
  
  ret      = xnee_set_all_clients_str(xd, "false"); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_all_clients_str"); 
  
  new  = xnee_get_all_clients(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_all_clients_str"); 
  XNEE_TEST_ASSERT(new, 0 ,"xnee_set_all_clients_str"); 
  XNEE_TEST_ASSERT_DIFF(orig, new,"xnee_set_all_clients_str");

  ret  = xnee_set_all_clients_str(xd, "true");
  new  = xnee_get_all_clients(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_all_clients_str"); 
  XNEE_TEST_ASSERT(new, orig, "xnee_set_all_clients_str");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_future_clients_str
   *
   */
    BEGIN_LOOP
  xnee_set_future_clients(xd);
  orig = xnee_is_future_clients(xd);
  
  ret      = xnee_set_future_clients_str(xd, "false"); 
  new = xnee_is_future_clients(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_future_clients_str"); 
  
  new  = xnee_is_future_clients(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_future_clients_str"); 
  XNEE_TEST_ASSERT(new, 0,"xnee_set_future_clients_str"); 
  XNEE_TEST_ASSERT_DIFF(orig, new, "xnee_set_future_clients_str");

  ret      = xnee_set_future_clients_str(xd, "true");
  new = xnee_is_future_clients(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_future_clients_str"); 
  XNEE_TEST_ASSERT(new, 1,"xnee_set_future_clients_str"); 
  XNEE_TEST_ASSERT(orig, new, "xnee_set_future_clients_str");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_events_max_str
   *
   */
    BEGIN_LOOP
  xnee_set_events_max(xd, 10);
  orig = xnee_get_events_max(xd);
  
  ret      = xnee_set_events_max_str (xd, "12345"); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_events_max_str"); 
  
  new  = xnee_get_events_max(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_events_max_str"); 
  XNEE_TEST_ASSERT(new, 12345,"xnee_set_events_max_str");
  XNEE_TEST_ASSERT_DIFF(orig, new,"xnee_set_events_max_str");

  ret      = xnee_set_events_max_str (xd, "10");
  new  = xnee_get_events_max(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_events_max_str"); 
  XNEE_TEST_ASSERT(new, orig, "xnee_set_events_max_str");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_data_max_str
   *
   */
    BEGIN_LOOP
  xnee_set_data_max(xd, 12);
  orig = xnee_get_data_max(xd);
  
  ret      = xnee_set_data_max_str (xd, "34"); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_data_max_str"); 
  
  new  = xnee_get_data_max(xd);
  XNEE_TEST_ASSERT(new, 34,"xnee_set_data_max_str"); 
  XNEE_TEST_ASSERT_DIFF(orig, new,"xnee_set_data_max_str");

  ret      = xnee_set_data_max_str (xd, "12");
  new  = xnee_get_data_max(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_data_max_str"); 
  XNEE_TEST_ASSERT(new, orig, "xnee_set_data_max_str");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_time_max_str
   *
   */
    BEGIN_LOOP
  xnee_set_time_max(xd, 23);
  orig = xnee_get_time_max(xd);
  
  ret      = xnee_set_time_max_str (xd, "45"); 
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_time_max_str"); 
  
  new  = xnee_get_time_max(xd);
  XNEE_TEST_ASSERT(new, 45,"xnee_set_time_max_str"); 
  XNEE_TEST_ASSERT_DIFF(orig, new,"xnee_set_time_max_str");

  ret      = xnee_set_time_max_str (xd, "23");
  new  = xnee_get_time_max(xd);
  XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_time_max_str"); 
  XNEE_TEST_ASSERT(new, orig, "xnee_set_time_max_str");
  END_LOOP

  /* Testing:
   *
   *           xnee_set_rep_resolution
   *
   */
    BEGIN_LOOP
  {
    int orig_x;
    int orig_y;
    int new_x;
    int new_y;
    
    xnee_set_rep_resolution_x(xd, 1280);
    xnee_set_rep_resolution_y(xd, 800);

    orig_x = xnee_get_rep_resolution_x(xd);
    orig_y = xnee_get_rep_resolution_y(xd);
  
    ret      = xnee_set_rep_resolution_x (xd, 123); 
    XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_rep_resolution"); 
    ret      = xnee_set_rep_resolution_y (xd, 456); 
    XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_rep_resolution"); 
  
    new_x  = xnee_get_rep_resolution_x(xd);
    new_y  = xnee_get_rep_resolution_y(xd);

    XNEE_TEST_ASSERT(new_x, 123,"xnee_set_rep_resolution"); 
    XNEE_TEST_ASSERT_DIFF(orig_x, new_x,"xnee_set_rep_resolution");

    XNEE_TEST_ASSERT(new_y, 456,"xnee_set_rep_resolution"); 
    XNEE_TEST_ASSERT_DIFF(orig_y, new_y,"xnee_set_rep_resolution");

    ret      = xnee_set_rep_resolution_x (xd, 1280); 
    XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_rep_resolution"); 
    ret      = xnee_set_rep_resolution_y (xd, 800); 
    XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_rep_resolution"); 
  
    new_x  = xnee_get_rep_resolution_x(xd);
    new_y  = xnee_get_rep_resolution_y(xd);
    XNEE_TEST_ASSERT(new_x, orig_x, "xnee_set_rep_resolution_str");
    XNEE_TEST_ASSERT(new_y, orig_y, "xnee_set_rep_resolution_str");
  }
  END_LOOP

  /* Testing:
   *
   *           xnee_set_rec_resolution
   *
   */
    BEGIN_LOOP
  {
    int orig_x;
    int orig_y;
    int new_x;
    int new_y;
    
    xnee_set_rec_resolution_x(xd, 1280);
    xnee_set_rec_resolution_y(xd, 800);

    orig_x = xnee_get_rec_resolution_x(xd);
    orig_y = xnee_get_rec_resolution_y(xd);
  
    ret      = xnee_set_rec_resolution_x (xd, 123); 
    XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_rec_resolution"); 
    ret      = xnee_set_rec_resolution_y (xd, 456); 
    XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_rec_resolution"); 
  
    new_x  = xnee_get_rec_resolution_x(xd);
    new_y  = xnee_get_rec_resolution_y(xd);

    XNEE_TEST_ASSERT(new_x, 123,"xnee_set_rec_resolution"); 
    XNEE_TEST_ASSERT_DIFF(orig_x, new_x,"xnee_set_rec_resolution");

    XNEE_TEST_ASSERT(new_y, 456,"xnee_set_rec_resolution"); 
    XNEE_TEST_ASSERT_DIFF(orig_y, new_y,"xnee_set_rec_resolution");

    ret      = xnee_set_rec_resolution_x (xd, 1280); 
    XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_rec_resolution"); 
    ret      = xnee_set_rec_resolution_y (xd, 800); 
    XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_rec_resolution"); 
  
    new_x  = xnee_get_rec_resolution_x(xd);
    new_y  = xnee_get_rec_resolution_y(xd);
    XNEE_TEST_ASSERT(new_x, orig_x, "xnee_set_rec_resolution_str");
    XNEE_TEST_ASSERT(new_y, orig_y, "xnee_set_rec_resolution_str");
  }
  END_LOOP

    /* Testing:
     *
     *           xnee_set_sync_mode_str
     *
     */
    BEGIN_LOOP
    orig = xnee_get_sync_mode(xd);
  
    ret  = xnee_set_sync_mode_str (xd, "false"); 
    XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_sync_mode_str"); 
  
    new  = xnee_get_sync_mode(xd);
    XNEE_TEST_ASSERT(new, 0,"xnee_set_sync_mode_str"); 
    XNEE_TEST_ASSERT_DIFF(new, 1,"xnee_set_sync_mode_str");

    ret      = xnee_set_sync_mode_str (xd, "true");
    new  = xnee_get_sync_mode(xd);
    XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_sync_mode_str"); 
    XNEE_TEST_ASSERT(new, orig, "xnee_set_sync_mode_str");
    END_LOOP

    /* Testing:
     *
     *           xnee_set_sync_mode
     *
     */
    BEGIN_LOOP
    xnee_set_sync_mode(xd);
    orig  = xnee_get_sync_mode(xd);
  
    ret   = xnee_unset_sync_mode(xd); 
    new        = xnee_get_sync_mode(xd);
    XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_sync_mode"); 
    XNEE_TEST_ASSERT_DIFF(orig    , new      ,"xnee_set_sync_mode");

    ret      = xnee_set_sync_mode(xd);
    new      = xnee_get_sync_mode(xd);
    XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_sync_mode"); 
    XNEE_TEST_ASSERT(new      , orig    , "xnee_set_sync_mode");
    END_LOOP

    /* Testing:
     *
     *           xnee_set_xosd_font
     *
     */
    BEGIN_LOOP
    ret      = xnee_set_xosd_font(xd, "apa"); 
    XNEE_TEST_ASSERT(ret, XNEE_OK, "xnee_set_xosd_font"); 
    orig_str = (char*)xnee_get_xosd_font(xd);
    XNEE_TEST_ASSERT_STR(orig_str, "apa","xnee_set_xosd_font"); 
    END_LOOP

    return 0;  
} 
