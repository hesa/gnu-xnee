/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004 Henrik Sandklef
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


#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"



int
xnee_set_display_name (xnee_data *xd, char *disp)
{
  if (disp==NULL)
    return XNEE_OK;


  xd->display=strdup(disp);

  if (xd->display==NULL)
    {
      return XNEE_MEMORY_FAULT;
    }
  return XNEE_OK;
}

char*
xnee_get_display_name (xnee_data *xd)
{
  return xd->display;
}


Display *
xnee_get_control_display(xnee_data *xd)
{
  return xd->control;
}

Display *
xnee_get_data_display(xnee_data *xd)
{
  return xd->data;
}

Display *
xnee_get_grab_display(xnee_data *xd)
{
  return xd->grab;
}


int
xnee_set_out_file (xnee_data *xd, FILE* out)
{
  XNEE_FCLOSE_IF_NOT_NULL(xd->out_file);
  xd->out_file=out;
  return XNEE_OK;
}

FILE*
xnee_get_out_file (xnee_data *xd)
{
  return xd->out_file;
}

int
xnee_set_out_name (xnee_data *xd, char* out_name)
{
  XNEE_FREE_IF_NOT_NULL(xd->out_name);
  xd->out_name=strdup(out_name);
  if (xd->out_name==NULL)
    {
      return XNEE_MEMORY_FAULT;
    }
  return XNEE_OK;
}
 
char *
xnee_get_out_name (xnee_data *xd)
{
  return xd->out_name;
}


int
xnee_set_rt_file (xnee_data *xd, FILE* rt)
{
  XNEE_FCLOSE_IF_NOT_NULL(xd->rt_file);
  xd->rt_file=rt;
  return XNEE_OK;
}

FILE*
xnee_get_rt_file (xnee_data *xd)
{
  return xd->rt_file;
}

int
xnee_set_rt_name (xnee_data *xd, char* rt_name)
{
  XNEE_FREE_IF_NOT_NULL(xd->rt_name);
  xd->rt_name=strdup(rt_name);
  if (xd->rt_name==NULL)
    {
      return XNEE_MEMORY_FAULT;
    }
  return XNEE_OK;
}
 
char *
xnee_get_rt_name (xnee_data *xd)
{
  return xd->rt_name;
}


xnee_set_err_file (xnee_data *xd, FILE* err)
{
  XNEE_FCLOSE_IF_NOT_NULL(xd->out_file);
  xd->err_file=err;
  return XNEE_OK;
}

FILE*
xnee_get_err_file (xnee_data *xd)
{
  return xd->err_file;
}



int
xnee_set_err_name (xnee_data *xd, char* err_name)
{
  XNEE_FREE_IF_NOT_NULL(xd->err_name);
  xd->err_name=strdup(err_name);

  if (xd->err_name==NULL)
    {
      return XNEE_MEMORY_FAULT;
    }
  return XNEE_OK;
}
 
char *
xnee_get_err_name (xnee_data *xd)
{
  return xd->err_name;
}



int
xnee_set_rc_file (xnee_data *xd, FILE* rc)
{
  XNEE_FCLOSE_IF_NOT_NULL(xd->rc_file);
  xd->rc_file=rc;
  return XNEE_OK;
}


FILE*
xnee_get_rc_file (xnee_data *xd)
{
  return xd->rc_file;
}


int
xnee_set_rc_name (xnee_data *xd, char* rc_name)
{
  XNEE_FREE_IF_NOT_NULL(xd->rc_name);
  xd->rc_name=strdup(rc_name);
  if (xd->rc_name==NULL)
    {
      return XNEE_MEMORY_FAULT;
    }
  return XNEE_OK;
}
 
char *
xnee_get_rc_name (xnee_data *xd)
{
  return xd->rc_name;
}


int
xnee_set_rc_byname (xnee_data *xd, char *rc_name)
{
  if (rc_name!=NULL)
    {
      xnee_set_rc_name (xd, rc_name);
    }
  else
    {
      return XNEE_OK;
    }

  XNEE_FCLOSE_IF_NOT_NULL(xd->rc_file);
  xd->rc_file = fopen (xd->rc_name,"r");

  if (xd->rc_file ==NULL)
    {
      XNEE_FREE_IF_NOT_NULL (xd->rc_name);
      return XNEE_FILE_NOT_FOUND;
    }

  return XNEE_OK;
}



int
xnee_set_data_file (xnee_data *xd, FILE* data_file)
{
  XNEE_FCLOSE_IF_NOT_NULL(xd->data_file);
  xd->data_file=data_file;
  return XNEE_OK;
}

FILE*
xnee_get_data_file (xnee_data *xd)
{
  return xd->data_file;
}

int
xnee_set_data_name (xnee_data *xd, char* data)
{
  XNEE_FREE_IF_NOT_NULL(xd->data_name);
  xd->data_name=strdup(data);
  return XNEE_OK;
}

char *
xnee_get_data_name (xnee_data *xd)
{
  return xd->data_name;
}





 
int
xnee_set_verbose (xnee_data *xd)
{
  xd->verbose=XNEE_TRUE;
  return XNEE_OK;
}
 
int
xnee_unset_verbose (xnee_data *xd)
{
  xd->verbose=XNEE_FALSE;
  return XNEE_OK;
}
 
int
xnee_get_verbose (xnee_data *xd)
{
  return xd->verbose;
}

 
int
xnee_is_verbose (xnee_data *xd)
{
  return (xd->verbose==XNEE_TRUE);
}

 
int
xnee_set_buf_verbose (xnee_data *xd)
{
  xd->buf_verbose=XNEE_TRUE;
  return XNEE_OK;
}

int
xnee_unset_buf_verbose (xnee_data *xd)
{
  xd->buf_verbose=XNEE_FALSE;
  return XNEE_OK;
}
 
int
xnee_get_buf_verbose (xnee_data *xd)
{
  return xd->buf_verbose;
}

 


int
xnee_set_all_clients (xnee_data *xd)
{
  xd->all_clients=XNEE_TRUE;
  return XNEE_OK;
}

int
xnee_unset_all_clients (xnee_data *xd)
{
  xd->all_clients=XNEE_FALSE;
  return XNEE_OK;
}

int
xnee_get_all_clients (xnee_data *xd)
{
  return xd->all_clients;
}


int
xnee_is_all_clients (xnee_data *xd)
{
  return (xd->all_clients==XNEE_TRUE);
}
 


 
int
xnee_set_sync (xnee_data *xd)
{
  xnee_verbose((xd, "xnee_set_sync\n"));
  xd->sync=XNEE_TRUE;
  return XNEE_OK;
}

int
xnee_unset_sync (xnee_data *xd)
{
  xnee_verbose((xd, "xnee_unset_sync\n"));
  xd->sync=XNEE_FALSE;
  return XNEE_OK;
}
 
int
xnee_get_sync (xnee_data *xd)
{
  return xd->sync;
}


int
xnee_is_sync (xnee_data *xd)
{
  return (xd->sync==XNEE_TRUE);
}

 
int
xnee_set_recorder (xnee_data *xd)
{
  xd->mode=XNEE_RECORDER;
  return XNEE_OK;
}

int
xnee_set_replayer (xnee_data *xd)
{
  xd->mode=XNEE_REPLAYER;
  return XNEE_OK;
}
 
int
xnee_set_distributor (xnee_data *xd)
{
  xd->mode=XNEE_REPLAYER;
  return XNEE_OK;
}
 

int
xnee_set_retyper (xnee_data *xd)
{
  xd->mode=XNEE_RETYPER;
  return XNEE_OK;
}
 

int
xnee_get_mode (xnee_data *xd)
{
  return xd->mode;
}

int
xnee_set_mode (xnee_data *xd, int mode)
{
  xd->mode=mode;
  return XNEE_OK;
}

int
xnee_is_recorder (xnee_data *xd)
{
  return (xd->mode==XNEE_RECORDER);
}

int
xnee_is_replayer (xnee_data *xd)
{
  return (xd->mode==XNEE_REPLAYER);
}

int
xnee_is_distributor (xnee_data *xd)
{
  return (xd->mode==XNEE_DISTRIBUTOR);
}

int
xnee_is_retyper (xnee_data *xd)
{
  return (xd->mode==XNEE_RETYPER);
}



int
xnee_set_cont (xnee_data *xd)
{
  xd->cont=XNEE_TRUE;
  return XNEE_OK;
}

int
xnee_get_cont (xnee_data *xd)
{
  return xd->cont;
}

int
xnee_unset_cont (xnee_data *xd)
{
  xd->cont=XNEE_FALSE;
  return XNEE_OK;
}

int
xnee_is_cont (xnee_data *xd)
{
  xd->cont=XNEE_FALSE;
  return XNEE_OK;
}



int
xnee_set_km (xnee_data *xd, int mode, char* km)
{
    switch (mode)
    {
    case XNEE_GRAB_STOP:
      xnee_set_stop_km(xd, km);
      break;
    case XNEE_GRAB_PAUSE:
      xnee_set_pause_km(xd, km);
      break;
    case XNEE_GRAB_RESUME:
      xnee_set_resume_km(xd, km);
      break;
    case XNEE_GRAB_INSERT:
      xnee_set_insert_km(xd, km);
      break;
    case XNEE_GRAB_EXEC:
      xnee_set_exec_km(xd, km);
      break;
    default:
      xnee_print_error ("Unknown grab mode\n");
      return XNEE_UNKNOWN_GRAB_MODE;
    }
      return XNEE_OK;
}

char*
xnee_get_km (xnee_data *xd, int mode)
{
    switch (mode)
    {
    case XNEE_GRAB_STOP:
      xnee_get_stop_km(xd);
      break;
    case XNEE_GRAB_PAUSE:
      xnee_get_pause_km(xd);
      break;
    case XNEE_GRAB_RESUME:
      xnee_get_resume_km(xd);
      break;
    case XNEE_GRAB_INSERT:
      xnee_get_insert_km(xd);
      break;
    case XNEE_GRAB_EXEC:
      xnee_get_exec_km(xd);
      break;
    default:
      xnee_print_error ("Unknown grab mode\n");
      return NULL;
    }
    return XNEE_OK;
}

int
xnee_set_stop_km (xnee_data *xd, char* stop_km)
{
  XNEE_FREE_IF_NOT_NULL(xd->grab_keys->stop_str);
  xd->grab_keys->stop_str=strdup(stop_km);
  return XNEE_OK;
}

char *
xnee_get_stop_km (xnee_data *xd)
{
  return xd->grab_keys->stop_str;
}

 


int
xnee_set_pause_km (xnee_data *xd, char *pause_km)
{
  XNEE_FREE_IF_NOT_NULL(xd->grab_keys->pause_str);
  xd->grab_keys->pause_str=strdup(pause_km);
  return XNEE_OK;
}

char *
xnee_get_pause_km (xnee_data *xd)
{
  return xd->grab_keys->pause_str;
}


int
xnee_set_resume_km (xnee_data *xd, char* resume_km)
{
  XNEE_FREE_IF_NOT_NULL(xd->grab_keys->resume_str);
  xd->grab_keys->resume_str=strdup(resume_km);
  return XNEE_OK;
}

char*
xnee_get_resume_km (xnee_data *xd)
{
  return xd->grab_keys->resume_str;
}


int
xnee_set_insert_km (xnee_data *xd, char* insert_km)
{
  XNEE_FREE_IF_NOT_NULL(xd->grab_keys->insert_str);
  xd->grab_keys->insert_str=strdup(insert_km);
  return XNEE_OK;
}

char*
xnee_get_insert_km (xnee_data *xd)
{
  return xd->grab_keys->insert_str;
}

int
xnee_set_exec_km (xnee_data *xd, char *exec_km)
{
  XNEE_FREE_IF_NOT_NULL(xd->grab_keys->exec_str);
  xd->grab_keys->exec_str=strdup(exec_km);
  return XNEE_OK;
}

int
xnee_set_exec_prog (xnee_data *xd, char *prog)
{
  XNEE_FREE_IF_NOT_NULL(xd->grab_keys->exec_prog);
  xd->grab_keys->exec_prog=strdup(prog);
  return XNEE_OK;
}

char*
xnee_get_exec_km (xnee_data *xd)
{
  return xd->grab_keys->exec_str;
}

char*
xnee_get_exec_prog (xnee_data *xd)
{
  return xd->grab_keys->exec_prog;
}

 



 
int
xnee_set_force_replay (xnee_data *xd)
{
  xd->force_replay=XNEE_TRUE;
  return XNEE_OK;
}
 
int
xnee_unset_force_replay (xnee_data *xd)
{
  xd->force_replay=XNEE_FALSE;
  return XNEE_OK;
}
 
int
xnee_get_force_replay (xnee_data *xd)
{
  return (xd->force_replay==XNEE_TRUE);
}

 


int
xnee_is_force_replay (xnee_data *xd)
{
  return (xd->force_replay==XNEE_TRUE);
}

 
int
set_first_replayed_event (xnee_data *xd)
{
  xd->first_replayed_event=XNEE_TRUE;
  return 0;
}

int
get_first_replayed_event (xnee_data *xd)
{
  return (xd->first_replayed_event==XNEE_TRUE);
}

int
is_first_replayed_event (xnee_data *xd)
{
  return xd->first_replayed_event;
}




int 
xnee_set_first_last (xnee_data *xd)
{
  xd->xnee_info.first_last = XNEE_TRUE;
  return XNEE_OK;
}


int 
xnee_get_first_last (xnee_data *xd){
  return xd->xnee_info.first_last;
}

int 
xnee_is_first_last (xnee_data *xd){
  return (xd->xnee_info.first_last==XNEE_TRUE);
}


int 
xnee_unset_first_last (xnee_data *xd)
{
  xd->xnee_info.first_last = XNEE_FALSE;
  return XNEE_OK;
}









int 
xnee_set_no_expose (xnee_data *xd)
{
  xd->xnee_info.no_expose = XNEE_TRUE;
  return XNEE_OK;
}


int 
xnee_get_no_expose (xnee_data *xd){
  return xd->xnee_info.no_expose;
}

int 
xnee_is_no_expose (xnee_data *xd){
  return (xd->xnee_info.no_expose==XNEE_TRUE);
}


int 
xnee_unset_no_expose (xnee_data *xd)
{
  xd->xnee_info.no_expose = XNEE_FALSE;
  return XNEE_OK;
}



int 
xnee_set_events_max (xnee_data *xd, int loops)
{
  xd->xnee_info.events_max = loops;
  return XNEE_OK;
}


int 
xnee_get_events_max (xnee_data *xd)
{
  return xd->xnee_info.events_max;
}


int 
xnee_get_events_left (xnee_data *xd)
{
  return (xd->xnee_info.events_max - xd->xnee_info.events_recorded);
}


int 
xnee_set_data_max (xnee_data *xd, int loops)
{
  xd->xnee_info.data_max = loops;
  return XNEE_OK;
}


int 
xnee_get_data_max (xnee_data *xd)
{
  return xd->xnee_info.data_max;
}


int 
xnee_get_data_left (xnee_data *xd){
  return (xd->xnee_info.data_max - xd->xnee_info.data_recorded);
}


int 
xnee_set_time_max (xnee_data *xd, int time)
{
  xd->xnee_info.time_max = time;
  return XNEE_OK;
}

int 
xnee_get_time_max (xnee_data *xd)
{
  return xd->xnee_info.time_max ;
}


int 
xnee_get_time_left (xnee_data *xd)
{
  return (xd->xnee_info.time_max - xd->xnee_info.time_recorded);
}



int 
xnee_set_interval (xnee_data *xd, int interval)
{
  printf ("set interval = %d\n", interval); 
  xd->xnee_info.interval = interval ;
  return XNEE_OK;
}

int 
xnee_get_interval (xnee_data *xd)
{
  return xd->xnee_info.interval;
}


int 
xnee_set_human_printout (xnee_data *xd)
{
  xd->rec_callback = xnee_human_dispatch;
  return XNEE_OK;
}

int 
xnee_set_xnee_printout (xnee_data *xd)
{
  xd->rec_callback = xnee_record_dispatch;
  return XNEE_OK;
}




int 
xnee_set_all_events (xnee_data *xd)
{
  xd->xnee_info.all_events = True;
  return XNEE_OK;
}

int 
xnee_is_all_events (xnee_data *xd)
{
  return (xd->xnee_info.all_events == XNEE_TRUE);
}


int
xnee_set_replay_speed_str (xnee_data *xd, char *speed_str)
{
  int speed;
  int ret  ; 
  ret = sscanf(speed_str, "%d", &speed);
  
  if (ret == 1)
    {
      xnee_verbose ((xd, "Setting replay speed = %d (%s)\n", 
		     speed, speed_str));
      xnee_set_replay_speed (xd, speed);
      return XNEE_OK;
    }
  else
    {
      xnee_verbose ((xd, "Failed to set replay speed\n"));
      return XNEE_BAD_SPEED;
    }
  return XNEE_OK;
}

int
xnee_set_replay_speed (xnee_data *xd, int speed)
{
  xnee_verbose ((xd, "xnee_set_replay_speed = %d \n", speed));
  xd->speed_percent=speed;
  return XNEE_OK;
}


int
xnee_get_replay_speed(xnee_data *xd)
{
  return xd->speed_percent;
}

