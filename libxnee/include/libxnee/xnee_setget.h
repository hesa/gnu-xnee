/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003 
 *                      2004, 2005, 2006, 2007, 2008
 *                      2009, 2010, 2011  Henrik Sandklef                    
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
#ifndef XNEE_SETGET_H
#define XNEE_SETGET_H

#include "libxnee/xnee.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"


#define xnee_set_events_replayed(xd,n)   xd->xnee_info.replayed_events=n;
#define xnee_inc_events_replayed(xd)     xd->xnee_info.replayed_events++;
#define xnee_get_events_replayed(xd)     xd->xnee_info.replayed_events;

/** 
 * 
 * 
 * @param xd    xnee's main structure
 * @param disp  name of the X display to set
 * 
 * This functions is called to set the display name of the 
 * main display for Xnee.
 *
 * @return      0 on success
 */
int
xnee_set_display_name (xnee_data *xd, const char *disp);

/** 
 * 
 * 
 * @param xd    xnee's main structure
 * 
 * This functions is called to get the display name of the
 * display used in Xnee.
 *
 * @return      display
 */
char*
xnee_get_display_name (xnee_data *xd);


/** 
 * 
 * 
 * @param xd    xnee's main structure
 * 
 * This functions is called to get the display of the 
 * control display for Xnee.
 *
 * @return 
 */
Display *
xnee_get_control_display(xnee_data *xd);

/** 
 * 
 * 
 * @param xd    xnee's main structure
 * 
 * This functions is called to get the display of the 
 * data display for Xnee.
 *
 * @return 
 */
Display *
xnee_get_data_display(xnee_data *xd);

/** 
 * 
 * 
 * @param xd    xnee's main structure
 * 
 * This functions is called to get the display of the 
 * grab display for Xnee.
 *
 * @return      display 
 */
Display *
xnee_get_grab_display(xnee_data *xd);


/** 
 * 
 * 
 * @param xd    xnee's main structure
 * @param out   file descriptor to use for printout
 * 
 * @return 
 */
int
xnee_set_out_file (xnee_data *xd, FILE* out);


/** 
 * 
 * 
 * @param xd    xnee's main structure
 * 
 * @return      file descriptor to use for printout
 */
FILE*
xnee_get_out_file (xnee_data *xd);

/** 
 * 
 * 
 * @param xd         xnee's main structure
 * @param out_name   file name to use for printout
 * 
 * @return 
 */
int
xnee_set_out_name (xnee_data *xd, char* out_name);
 
/** 
 * 
 * 
 * @param xd         xnee's main structure
 * 
 * @return           file name to use for printout
 */
char *
xnee_get_out_name (xnee_data *xd);


/** 
 * 
 * 
 * @param xd    xnee's main structure
 * @param rt    file descriptor to use for retype
 * 
 * @return 
 */
int
xnee_set_rt_file (xnee_data *xd, FILE* rt);

/** 
 * 
 * 
 * @param xd    xnee's main structure
 * 
 * @return      file descriptor to use for retype
 */
FILE*
xnee_get_rt_file (xnee_data *xd);

/** 
 * 
 * 
 * @param xd         xnee's main structure
 * 
 * @return           file name to use for retype
 */
int
xnee_set_rt_name (xnee_data *xd, char* rt_name);
 
/** 
 * 
 * 
 * @param xd         xnee's main structure
 * 
 * @return           file name to use for retype
 */
char *
xnee_get_rt_name (xnee_data *xd);


/** 
 * 
 * 
 * @param xd        xnee's main structure
 * @param err       file decriptor to use for error prints
 * 
 * @return          0 on success
 */
int
xnee_set_err_file (xnee_data *xd, FILE* err);

/** 
 * 
 * 
 * @param xd    xnee's main structure
 * 
 * @return      file descriptor to use for error prints
 */
FILE*
xnee_get_err_file (xnee_data *xd);


/** 
 * 
 * 
 * @param xd         xnee's main structure
 * @param out_name   file name to use for error prints
 * 
 * @return           0 on success
 */
int
xnee_set_err_name (xnee_data *xd, char* err_name);
 

char *
xnee_get_err_name (xnee_data *xd);


int
xnee_set_rc_file (xnee_data *xd, FILE* rc);

FILE*
xnee_get_rc_file (xnee_data *xd);


int
xnee_set_rc_name (xnee_data *xd, const char* rc_name);
 
int
xnee_set_rc_byname (xnee_data *xd, const char* rc_name);
 
 
char *
xnee_get_rc_name (xnee_data *xd);



int
xnee_set_data_file (xnee_data *xd, FILE* data_file);

FILE*
xnee_get_data_file (xnee_data *xd);

int
xnee_set_data_name (xnee_data *xd, const char* data);

char *
xnee_get_data_name (xnee_data *xd);


 
int
xnee_set_verbose (xnee_data *xd);
 
int
xnee_unset_verbose (xnee_data *xd);
 
int
xnee_get_verbose (xnee_data *xd);

 
int
xnee_is_verbose (xnee_data *xd);

 
int
xnee_set_buf_verbose (xnee_data *xd);

int
xnee_unset_buf_verbose (xnee_data *xd);
 
int
xnee_get_buf_verbose (xnee_data *xd);

 


int
xnee_set_all_clients (xnee_data *xd);


int
xnee_unset_all_clients (xnee_data *xd);

int
xnee_get_all_clients (xnee_data *xd);

int
xnee_is_future_clients_str(xnee_data *xd);

int
xnee_is_all_clients (xnee_data *xd);
 


 
int
xnee_set_recorder (xnee_data *xd);

int
xnee_set_replayer (xnee_data *xd);
 
int
xnee_set_distributor (xnee_data *xd);
 
int
xnee_set_syntax_checker (xnee_data *xd);
 
int
xnee_set_retyper (xnee_data *xd);
 
unsigned char
xnee_get_mode (xnee_data *xd);

int
xnee_set_mode (xnee_data *xd, unsigned char mode);

int
xnee_is_recorder (xnee_data *xd);

int
xnee_is_replayer (xnee_data *xd);

int
xnee_is_syntax_checker (xnee_data *xd);
 
int
xnee_is_distributor (xnee_data *xd);

int
xnee_is_retyper (xnee_data *xd);


 

char*
xnee_get_exec_prog (xnee_data *xd);

int
xnee_set_key (xnee_data *xd, int mode, const char* key );

char *
xnee_get_key (xnee_data *xd, int mode);

char *
xnee_get_key_str (xnee_data *xd, int mode);

char*
xnee_get_extra_str (xnee_data *xd, int idx);

int
xnee_set_key_str (xnee_data *xd, int idx, const char *str);

int
xnee_set_extra_str (xnee_data *xd, int idx, const char *str);

int
xnee_set_exec_prog (xnee_data *xd, const char *prog);


#define xnee_get_stop_key(xd)        xnee_get_key_str(xd, XNEE_GRAB_STOP)
#define xnee_get_pause_key(xd)       xnee_get_key_str(xd, XNEE_GRAB_PAUSE)
#define xnee_get_resume_key(xd)      xnee_get_key_str(xd, XNEE_GRAB_RESUME)
#define xnee_get_insert_key(xd)      xnee_get_key_str(xd, XNEE_GRAB_INSERT)
#define xnee_get_exec_key(xd)        xnee_get_key_str(xd, XNEE_GRAB_EXEC)

#define xnee_set_stop_key(xd, key)        xnee_set_key_str(xd, XNEE_GRAB_STOP, key)
#define xnee_set_pause_key(xd, key)       xnee_set_key_str(xd, XNEE_GRAB_PAUSE, key)
#define xnee_set_resume_key(xd, key)      xnee_set_key_str(xd, XNEE_GRAB_RESUME, key)
#define xnee_set_insert_key(xd, key)      xnee_set_key_str(xd, XNEE_GRAB_INSERT, key)
#define xnee_set_exec_key(xd, key)        xnee_set_key_str(xd, XNEE_GRAB_EXEC, key)

 
int
xnee_set_sync (xnee_data *xd);

int
xnee_get_sync (xnee_data *xd);

int
xnee_is_sync (xnee_data *xd);

int
xnee_unset_sync (xnee_data *xd);
 




 
int
xnee_set_force_replay (xnee_data *xd);
 
int
xnee_unset_force_replay (xnee_data *xd);
 
int
xnee_get_force_replay (xnee_data *xd);

int
xnee_is_force_replay (xnee_data *xd);


 

int
xnee_set_cont (xnee_data *xd);

int
xnee_get_cont (xnee_data *xd);

int
xnee_unset_cont (xnee_data *xd);

int
xnee_is_cont (xnee_data *xd);



 
 
int
set_first_replayed_event (xnee_data *xd);

int
get_first_replayed_event (xnee_data *xd);

int
is_first_replayed_event (xnee_data *xd);


int 
xnee_set_first_last (xnee_data *xd);

int 
xnee_unset_first_last (xnee_data *xd);

int 
xnee_get_first_last (xnee_data *xd);

int 
xnee_is_first_last (xnee_data *xd);





int 
xnee_set_no_expose (xnee_data *xd);

int 
xnee_unset_no_expose (xnee_data *xd);

int 
xnee_get_no_expose (xnee_data *xd);

int 
xnee_is_no_expose (xnee_data *xd);


int 
xnee_set_interval (xnee_data *xd, int interval);

int 
xnee_get_interval (xnee_data *xd);


int 
xnee_set_human_printout (xnee_data *xd);

int 
xnee_set_xnee_printout (xnee_data *xd);

#define xnee_is_human_printout(xd) (xd->rec_callback == xnee_human_dispatch)
#define xnee_is_xnee_printout(xd)  (xd->rec_callback == xnee_record_dispatch)
#define xnee_is_autorepeat(xd)     (xd->autorepeat_saved!=0)


int
xnee_set_replay_speed_str (xnee_data *xd, const char *speed_str);

int
xnee_set_replay_speed (xnee_data *xd, int speed);

int
xnee_get_replay_speed(xnee_data *xd);

int 
xnee_set_replay_offset_str (xnee_data *xd, char *str);

int 
xnee_set_replay_offset_x(xnee_data *xd, int offset);

int 
xnee_set_replay_offset_y(xnee_data *xd, int offset);

int 
xnee_get_replay_offset_x(xnee_data *xd);

int 
xnee_get_replay_offset_y(xnee_data *xd);




int 
xnee_set_events_max_str (xnee_data *xd, char *str);

int 
xnee_set_events_max (xnee_data *xd, int loops);

int 
xnee_get_events_max (xnee_data *xd);

int 
xnee_get_events_left (xnee_data *xd);

int 
xnee_set_data_max (xnee_data *xd, int loops);

int 
xnee_get_data_max (xnee_data *xd);

int 
xnee_get_data_left (xnee_data *xd);

int 
xnee_set_time_max (xnee_data *xd, int time);

int 
xnee_get_time_max (xnee_data *xd);

int 
xnee_get_time_left (xnee_data *xd);

#define xnee_set_events_recorded(xd,e)  xd->xnee_info.events_recorded=e;
#define xnee_get_events_recorded(xd)    xd->xnee_info.events_recorded;
#define xnee_inc_events_recorded(xd)    xd->xnee_info.events_recorded++;
#define xnee_dec_events_recorded(xd)    xd->xnee_info.events_recorded--;
#define xnee_zero_events_recorded(xd)   xd->xnee_info.events_recorded=0;

#define xnee_inc_data_recorded(xd)   xd->xnee_info.data_recorded++;
#define xnee_dec_data_recorded(xd)   xd->xnee_info.data_recorded--;
#define xnee_zero_data_recorded(xd)   xd->xnee_info.data_recorded=0;

#define xnee_inc_time_recorded(xd)   xd->xnee_info.time_recorded++;
#define xnee_dec_time_recorded(xd)   xd->xnee_info.time_recorded--;
#define xnee_zero_time_recorded(xd)   xd->xnee_info.time_recorded=0;

int
xnee_set_store_mouse_pos(xnee_data *xd);

int
xnee_unset_store_mouse_pos(xnee_data *xd);

Bool
xnee_is_store_mouse_pos(xnee_data *xd);

int
xnee_set_program_name(xnee_data *xd, const char* name);

char *
xnee_get_program_name(xnee_data *xd);

int
xnee_get_new_project(xnee_data *xd);

int
xnee_set_new_project(xnee_data *xd);

int
xnee_unset_new_project(xnee_data *xd);


int
xnee_set_autorepeat (xnee_data *xd);

int
xnee_reset_autorepeat (xnee_data *xd);


int
xnee_set_application_parameters(xnee_data *xd, char **argv);

char **
xnee_get_application_parameters(xnee_data *xd);

int
xnee_set_new_window_pos (xnee_data *xd);

int
xnee_get_new_window_pos (xnee_data *xd);

int
xnee_get_new_window_pos_value (xnee_data *xd);

int
xnee_unset_new_window_pos (xnee_data *xd);

int
xnee_get_recall_window_pos (xnee_data *xd);

int
xnee_set_recall_window_pos (xnee_data *xd);

int
xnee_unset_recall_window_pos (xnee_data *xd);

char *
xnee_get_project_name(xnee_data *xd);

char *
xnee_get_project_descr(xnee_data *xd);

char *
xnee_get_creat_date(xnee_data *xd);

char *
xnee_get_creat_program(xnee_data *xd);

char *
xnee_get_creat_prog_date(xnee_data *xd);

char *
xnee_get_last_date(xnee_data *xd);

char *
xnee_get_last_program(xnee_data *xd);

char *
xnee_get_last_prog_vers(xnee_data *xd);

char *
xnee_get_author_name(xnee_data *xd);

char *
xnee_get_author_email(xnee_data *xd);

char *
xnee_get_char(xnee_data *xd);

char *
xnee_get_project_name_str(xnee_data *xd);

char *
xnee_get_project_descr_str(xnee_data *xd);

char *
xnee_get_creat_date_str(xnee_data *xd);

char *
xnee_get_creat_prog_vers(xnee_data *xd);

char *
xnee_get_last_date_str(xnee_data *xd);

char *
xnee_get_last_prog_str(xnee_data *xd);

char *
xnee_get_last_prog_date_str(xnee_data *xd);

char *
xnee_get_author_name_str(xnee_data *xd);

char *
xnee_get_author_email_str(xnee_data *xd);

char *
xnee_get_char_str(xnee_data *xd);

int
xnee_set_all_clients_str(xnee_data *xd, char *str);

int
xnee_set_future_clients_str(xnee_data *xd, char *str);

int
xnee_set_future_clients(xnee_data *xd);

int
xnee_unset_future_clients(xnee_data *xd);

int 
xnee_set_data_max_str (xnee_data *xd, char *str);

int 
xnee_set_time_max_str (xnee_data *xd, char *str);

int 
xnee_set_sync_mode_str (xnee_data *xd, char *str);

int 
xnee_unset_sync_mode_str (xnee_data *xd, char *str);

int 
xnee_set_sync_mode(xnee_data *xd);

int 
xnee_get_sync_mode(xnee_data *xd);


int 
xnee_set_unsync_mode(xnee_data *xd);

int 
xnee_get_unsync_mode(xnee_data *xd);

int 
xnee_unset_sync_mode(xnee_data *xd);

int
xnee_set_buf_verbose(xnee_data *xd);

int
xnee_unset_buf_verbose(xnee_data *xd);

const char *
xnee_get_xosd_font(xnee_data *xd);

int
xnee_set_xosd_font(xnee_data *xd, char *font_str);

int
xnee_set_max_threshold_str (xnee_data *xd, char *thresh_str);

int
xnee_set_min_threshold_str (xnee_data *xd, char *thresh_str);

int
xnee_set_tot_threshold_str (xnee_data *xd, char *thresh_str);

int
xnee_set_max_threshold (xnee_data *xd, int thresh);

int
xnee_set_min_threshold (xnee_data *xd, int thresh);

int
xnee_set_tot_threshold (xnee_data *xd, int thresh);


int
xnee_set_rec_resolution_y (xnee_data *xd, int res);

int
xnee_set_rec_resolution_x (xnee_data *xd, int res);

int
xnee_set_rep_resolution_y (xnee_data *xd, int res);

int
xnee_set_rep_resolution_x (xnee_data *xd, int res);



int
xnee_get_max_threshold (xnee_data *xd);

int
xnee_get_min_threshold (xnee_data *xd);

int
xnee_get_tot_threshold (xnee_data *xd);

int
xnee_set_rec_resolution (xnee_data *xd, char *res_str);
 
int
xnee_get_rec_resolution_x (xnee_data *xd);

int
xnee_get_rec_resolution_y (xnee_data *xd);

int
xnee_set_rep_resolution (xnee_data *xd, char *res_str);

int
xnee_get_rep_resolution_x (xnee_data *xd);

int
xnee_get_rep_resolution_y (xnee_data *xd);
 
#define xnee_set_interrupt_error(xd, err) \
   (xd->xnee_info.interrupt=err)

#define xnee_set_interrupt_action(xd)  \
   (xd->xnee_info.interrupt=XNEE_OK_LEAVE)

#define xnee_unset_interrupt_action(xd) \
   (xd->xnee_info.interrupt=XNEE_OK)

#define xnee_get_interrupt_action(xd) \
   (xd->xnee_info.interrupt)

#define xnee_is_interrupt_action(xd) \
   (xd->xnee_info.interrupt!=XNEE_OK)

xnee_data*
xnee_get_xnee_data (void);

int
xnee_set_xnee_data (xnee_data *xd);

int
xnee_set_keep_autorepeat (xnee_data *xd);

int
xnee_set_retype_press_delay(xnee_data *xd, unsigned int delay);

int
xnee_set_retype_release_delay(xnee_data *xd, unsigned int delay);

#define xnee_get_retype_press_delay(xd) \
  (xd->retype.key_press_delay)

#define xnee_get_retype_release_delay(xd) \
  (xd->retype.key_release_delay)


int
xnee_set_project_file(xnee_data *xd, char *name);

int 
xnee_set_resolution_str (xnee_data *xd, char *str);

int
xnee_set_plugin_name(xnee_data *xd, char *str, unsigned char mode);

int 
xnee_set_xosd_feedback(xnee_data *xd);

int
xnee_set_stderr_feedback(xnee_data *xd);

int 
xnee_set_no_feedback(xnee_data *xd);

int
xnee_has_xinput2(xnee_data *xd);


int
xnee_set_no_reparent_recording(xnee_data *xd);

int
xnee_unset_no_reparent_recording(xnee_data *xd);

int
xnee_is_no_reparent_recording(xnee_data *xd);


int
xnee_set_replay_backend(xnee_data *xd, int replay_backend);

int
xnee_set_replay_backend_name(xnee_data *xd, char *replay_backend);

#define xnee_is_swinput_playback(xd) (xd->replay_backend == XNEE_REPLAY_SWINPUT)

int
xnee_set_forced_core_device_events(xnee_data *xd);

int
xnee_unset_forced_core_device_events(xnee_data *xd);

int
xnee_is_forced_core_device_events(xnee_data *xd);

#endif /* XNEE_SETGET_H */ 
