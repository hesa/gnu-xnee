/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003 Henrik Sandklef                    
 *                                                                   
 * This program is free software; you can redistribute it and/or     
 * modify it under the terms of the GNU General Public License       
 * as published by the Free Software Foundation; either version 2    
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston,            
 * MA  02111-1307, USA.                                              
 ****/
#ifndef XNEE_SETGET_H
#define XNEE_SETGET_H

#include "libxnee/xnee.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"


int
xnee_set_display_name (xnee_data *xd, char *disp);

char*
xnee_get_display_name (xnee_data *xd);

Display *
xnee_get_control_display(xnee_data *xd);

Display *
xnee_get_data_display(xnee_data *xd);

Display *
xnee_get_grab_display(xnee_data *xd);


int
xnee_set_out_file (xnee_data *xd, FILE* out);

FILE*
xnee_get_out_file (xnee_data *xd);

int
xnee_set_out_name (xnee_data *xd, char* out_name);
 
char *
xnee_get_out_name (xnee_data *xd);


int
xnee_set_out_byname (xnee_data *xd, char* out_name);



int
xnee_set_err_file (xnee_data *xd, FILE* err);

FILE*
xnee_get_err_file (xnee_data *xd);



int
xnee_set_err_name (xnee_data *xd, char* err_name);
 
char *
xnee_get_err_name (xnee_data *xd);

int
xnee_set_err_byname (xnee_data *xd, char *err_name);



int
xnee_set_rc_file (xnee_data *xd, FILE* rc);

FILE*
xnee_get_rc_file (xnee_data *xd);


int
xnee_set_rc_name (xnee_data *xd, char* rc_name);
 
char *
xnee_get_rc_name (xnee_data *xd);

int
xnee_set_rc_byname (xnee_data *xd, char *rc_name);



int
xnee_set_data_file (xnee_data *xd, FILE* data_file);

FILE*
xnee_get_data_file (xnee_data *xd);

int
xnee_set_data_name (xnee_data *xd, char* data);

char *
xnee_get_data_name (xnee_data *xd);



int
xnee_set_data_name_byname (xnee_data *xd, char* data_name);
 


 
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
xnee_is_all_clients (xnee_data *xd);
 


 
int
xnee_set_sync (xnee_data *xd);

int
xnee_unset_sync (xnee_data *xd);
 
int
xnee_get_sync (xnee_data *xd);

 
int
xnee_set_recorder (xnee_data *xd);

int
xnee_set_replayer (xnee_data *xd);
 
int
xnee_set_distributor (xnee_data *xd);
 
int
xnee_set_retyper (xnee_data *xd);
 
int
xnee_get_mode (xnee_data *xd);

int
xnee_set_mode (xnee_data *xd, int mode);

int
xnee_is_recorder (xnee_data *xd);

int
xnee_is_replayer (xnee_data *xd);

int
xnee_is_distributor (xnee_data *xd);

int
xnee_is_retyper (xnee_data *xd);


 


int
xnee_set_km (xnee_data *xd, int mode, char* km );

char *
xnee_get_km (xnee_data *xd, int mode);

int
xnee_set_stop_km (xnee_data *xd, char* stop_km);

char *
xnee_get_stop_km (xnee_data *xd);

int
xnee_set_pause_km (xnee_data *xd, char *pause_km);

char *
xnee_get_pause_km (xnee_data *xd);

int
xnee_set_resume_km (xnee_data *xd, char *resume_km);

char*
xnee_get_resume_km (xnee_data *xd);

int
xnee_set_insert_km (xnee_data *xd, char  *insert_km);

char*
xnee_get_insert_km (xnee_data *xd);

int
xnee_set_exec_km (xnee_data *xd, char *exec_km);

char*
xnee_get_exec_km (xnee_data *xd);
 



 
int
xnee_set_sync (xnee_data *xd);

int
xnee_get_sync (xnee_data *xd);

int
xnee_is_sync (xnee_data *xd);





 
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
xnee_set_all_events (xnee_data *xd);

int 
xnee_is_all_events (xnee_data *xd);

int 
xnee_get_interval (xnee_data *xd);


int 
xnee_set_human_printout (xnee_data *xd);

int
xnee_set_replay_speed_str (xnee_data *xd, char *speed_str);

int
xnee_set_replay_speed (xnee_data *xd, int speed);

int
xnee_get_replay_speed(xnee_data *xd);




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
xnee_get_time_left (xnee_data *xd);

#define xnee_inc_events_recorded(xd)   xd->xnee_info->events_recorded++;
#define xnee_dec_events_recorded(xd)   xd->xnee_info->events_recorded--;
#define xnee_zero_events_recorded(xd)   xd->xnee_info->events_recorded=0;

#define xnee_inc_data_recorded(xd)   xd->xnee_info->data_recorded++;
#define xnee_dec_data_recorded(xd)   xd->xnee_info->data_recorded--;
#define xnee_zero_data_recorded(xd)   xd->xnee_info->data_recorded=0;

#define xnee_inc_time_recorded(xd)   xd->xnee_info->time_recorded++;
#define xnee_dec_time_recorded(xd)   xd->xnee_info->time_recorded--;
#define xnee_zero_time_recorded(xd)   xd->xnee_info->time_recorded=0;


#endif /* XNEE_SETGET_H */ 
