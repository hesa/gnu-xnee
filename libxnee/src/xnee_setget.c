/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003, 
 *                      2004, 2009, 2010  Henrik Sandklef
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


#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_session.h"
#include "libxnee/xnee_utils.h"
#include "libxnee/xnee_alloc.h"
#include "libxnee/xnee_resource.h"
#include "libxnee/xnee_plugin.h"
#include "libxnee/feedback.h"
#include "libxnee/xnee_range.h"



static xnee_data *xd_saved;

int
xnee_set_display_name (xnee_data *xd, const char *disp)
{

  if (disp==NULL)
    {
      return XNEE_OK;
    }

  XNEE_FREE_IF_NOT_NULL(xd->display);
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
  if ( xd->out_file != xd->saved_out_file )
    {
      XNEE_FCLOSE_IF_NOT_NULL(xd->out_file);
    }
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


int
xnee_set_err_file (xnee_data *xd, FILE* err)
{
  if ( xd->err_file != xd->saved_err_file )
    {
      XNEE_FCLOSE_IF_NOT_NULL(xd->out_file);
    }
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
xnee_set_rc_name (xnee_data *xd, const char* rc_name)
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
xnee_set_rc_byname (xnee_data *xd, const char *rc_name)
{
  struct stat buf;

  if (rc_name!=NULL)
    {
      xnee_set_rc_name (xd, rc_name);
    }
  else
    {
      return XNEE_OK;
    }

  if (stat(rc_name, &buf) == ENOENT)
    {
      xnee_set_new_project(xd);
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
xnee_set_data_name (xnee_data *xd, const char* data)
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
   if (xd==NULL)
   {
      return (XNEE_NO_MAIN_DATA);
   }
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
xnee_is_future_clients (xnee_data *xd)
{
  return !xd->all_clients;
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
xnee_set_syntax_checker (xnee_data *xd)
{
  xd->mode=XNEE_SYNTAX_CHECKER;
  return XNEE_OK;
}
 
int
xnee_set_retyper (xnee_data *xd)
{
  xd->mode=XNEE_RETYPER;
  return XNEE_OK;
}
 

unsigned char 
xnee_get_mode (xnee_data *xd)
{
  return xd->mode;
}

int
xnee_set_mode (xnee_data *xd, unsigned char mode)
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
xnee_is_syntax_checker (xnee_data *xd)
{
  return (xd->mode==XNEE_SYNTAX_CHECKER);
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
xnee_set_key (xnee_data *xd, int mode, const char* km)
{
  xnee_verbose((xd, "---> xnee_set_key\n"));

  if (km==NULL)
    {
      return XNEE_UNKNOWN_GRAB_MODE;
    }

  switch (mode)
    {
    case XNEE_GRAB_STOP:
      xnee_verbose((xd, " --- xnee_set_key stop to %s\n", km));
      xnee_set_stop_key(xd, km);
      break;
    case XNEE_GRAB_PAUSE:
      xnee_verbose((xd, " --- xnee_set_key pause to %s\n", km));
      xnee_set_pause_key(xd, km);
      break;
    case XNEE_GRAB_RESUME:
      xnee_verbose((xd, " --- xnee_set_key resume to %s\n", km));
      xnee_set_resume_key(xd, km);
      break;
    case XNEE_GRAB_INSERT:
      xnee_verbose((xd, " --- xnee_set_key insert to %s\n", km));
      xnee_set_insert_key(xd, km);
      break;
    case XNEE_GRAB_EXEC:
      xnee_verbose((xd, " --- xnee_set_key exec to %s\n", km));
      xnee_set_exec_key(xd, km);
      break;
    default:
      xnee_print_error ("Unknown grab mode\n");
      return XNEE_UNKNOWN_GRAB_MODE;
    }
  xnee_verbose((xd, "<--- xnee_set_key\n"));
  return XNEE_OK;
}

char*
xnee_get_key (xnee_data *xd, int mode)
{
  char *ret;
  xnee_verbose((xd, "---> xnee_get_key %d\n", mode));
    switch (mode)
    {
    case XNEE_GRAB_STOP:
      xnee_verbose((xd, " --- xnee_get_key stop \n"));
      ret = xnee_get_stop_key(xd);
      break;
    case XNEE_GRAB_PAUSE:
      xnee_verbose((xd, " --- xnee_get_key pause \n"));
      ret = xnee_get_pause_key(xd);
      break;
    case XNEE_GRAB_RESUME:
      xnee_verbose((xd, " --- xnee_get_key resume \n"));
      ret = xnee_get_resume_key(xd);
      break;
    case XNEE_GRAB_INSERT:
      xnee_verbose((xd, " --- xnee_get_key insert \n"));
      ret = xnee_get_insert_key(xd);
      break;
    case XNEE_GRAB_EXEC:
      xnee_verbose((xd, " --- xnee_get_key exec \n"));
      ret = xnee_get_exec_key(xd);
      break;
    default:
      xnee_print_error ("Unknown grab mode\n");
      return NULL;
    }
    xnee_verbose((xd, "<--- xnee_get_key '%s'\n", ret));
    return ret;
}




int
xnee_set_extra_str (xnee_data *xd, int idx, const char *str)
{
  if ( (idx<0) && (idx>XNEE_GRAB_LAST) )
    {
      return XNEE_BAD_GRAB_DATA;
    }
  if (str==NULL)
    {
      return XNEE_SYNTAX_ERROR;
    }
  if ( ( xd != NULL ) && 
       ( xd->grab_keys!= NULL) )
    {
      XNEE_FREE_IF_NOT_NULL(xd->grab_keys->action_keys[idx].extra_str);
      xd->grab_keys->action_keys[idx].extra_str = strdup(str);
    }
  else
    {
      return XNEE_BAD_GRAB_DATA;
    }
  return XNEE_OK;
}

int
xnee_set_key_str (xnee_data *xd, int idx, const char *str)
{
  if ( (idx<0) && (idx>XNEE_GRAB_LAST) )
    return XNEE_BAD_GRAB_DATA;

  xnee_verbose((xd, "xnee_set_key_str (xd, %d, %s)\n", idx, str));

  XNEE_FREE_IF_NOT_NULL(xd->grab_keys->action_keys[idx].str);
  xd->grab_keys->action_keys[idx].str = strdup(str);
  return XNEE_OK;
}

char*
xnee_get_extra_str (xnee_data *xd, int idx)
{
  if ( (idx<0) && (idx>XNEE_GRAB_LAST) )
    return NULL;
  return xd->grab_keys->action_keys[idx].extra_str;
}

char*
xnee_get_key_str (xnee_data *xd, int idx)
{
  if ( (idx<0) && (idx>XNEE_GRAB_LAST) )
    return NULL;

  return xd->grab_keys->action_keys[idx].str;
}

char*
xnee_get_exec_prog (xnee_data *xd)
{
  return xd->grab_keys->action_keys[XNEE_GRAB_EXEC].extra_str;
}


int
xnee_set_exec_prog (xnee_data *xd, const char *prog)
{
  int ret ; 
  ret = xnee_set_extra_str (xd, XNEE_GRAB_EXEC, prog);
  return ret;
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
xnee_set_replay_speed_str (xnee_data *xd, const char *speed_str)
{
  int speed;
  int ret  ; 

  if (speed_str==NULL)
    {
      return XNEE_SYNTAX_ERROR;
    }

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


int
xnee_set_store_mouse_pos(xnee_data *xd)
{
  xd->xnee_info.store_mouse_pos = True ; 
  return XNEE_OK;
}

int
xnee_unset_store_mouse_pos(xnee_data *xd)
{
  xd->xnee_info.store_mouse_pos = False ; 
  return XNEE_OK;
}

Bool
xnee_is_store_mouse_pos(xnee_data *xd)
{
  return (xd->xnee_info.store_mouse_pos); 
}


int
xnee_set_program_name(xnee_data *xd, const char* name)
{
  XNEE_FREE_IF_NOT_NULL(xd->program_name);

  xd->program_name = strdup(name);
  if (xd->program_name==NULL)
    {
      return XNEE_MEMORY_FAULT;
    }
  return XNEE_OK;
}


char *
xnee_get_program_name(xnee_data *xd)
{
  return (xd->program_name);
}


int
xnee_get_new_project(xnee_data *xd)
{
  return (xd->xrm.new_project);
}

int
xnee_set_new_project(xnee_data *xd)
{
      xd->xrm.new_project = 1;
      return XNEE_OK;
}

int
xnee_unset_new_project(xnee_data *xd)
{
      xd->xrm.new_project = 0;
      return XNEE_OK;
}

int
xnee_set_application_parameters(xnee_data *xd, char **argv)
{
  xd->app_args=argv;
  return XNEE_OK;
}

char **
xnee_get_application_parameters(xnee_data *xd)
{
  return xd->app_args;
}


int
xnee_set_autorepeat (xnee_data *xd)
{
  if (xd->autorepeat_saved==1)
    {
      return XNEE_OK;
    }
  
  if (xd->keep_autorepeat!=0)
    {
      return XNEE_OK;
    }

  if ( (xd==NULL) || (xd->fake==NULL) )
    {
      return (XNEE_MEMORY_FAULT);
    }

  /*@ ignore @*/
  XGetKeyboardControl (xd->fake, &xd->kbd_orig);
  /*@ end @*/
  
  xnee_verbose ((xd," key_click_percent  %d \n", 
		 xd->kbd_orig.key_click_percent));
  xnee_verbose ((xd," bell_percent       %d\n", 
		 xd->kbd_orig.bell_percent));
  xnee_verbose ((xd," bell_pitch         %d\n", 
		 xd->kbd_orig.bell_pitch));
  xnee_verbose ((xd," bell_duration      %d\n", 
		 xd->kbd_orig.bell_duration));
  xnee_verbose ((xd," led_mask           %d\n",  
		 (int)xd->kbd_orig.led_mask));
  xnee_verbose ((xd," global_auto_repeat %d\n", 
		 xd->kbd_orig.global_auto_repeat));

  xnee_verbose((xd,"Auto repeat:\n"));
  /*
  for (i=0;i<32;i++)
    {
      xnee_verbose((xd,"Key\t"));
      for (j=1;j<=8;j++)
	xnee_verbose((xd,"%03d ", (i*8)+j )); 
      xnee_verbose((xd,"\nValue\t"));
      for (j=1;j<=8;j++)
        {
          xnee_verbose((xd,"  %d ", xd->kbd_orig.auto_repeats[i] && j));
        }
      xnee_verbose((xd,"\n\n"));
    }
  */
  /*@ ignore @*/
  XAutoRepeatOff(xd->fake);
  /*@ end @*/
  xd->autorepeat_saved=1;  

  return XNEE_OK;
}


int
xnee_reset_autorepeat (xnee_data *xd)
{  
  if (xd->autorepeat_saved==0)
    {
      return XNEE_OK;
    }

  if (xd->keep_autorepeat!=0)
    {
      return XNEE_OK;
    }

  xnee_verbose((xd,"Resetting autorepeat on (%d) to: ",
		(xd->fake==NULL)?0:(int)xd->fake));

  if (!xd->fake)
    return XNEE_OK;

  if (xd->kbd_orig.global_auto_repeat==AutoRepeatModeOn)
    {
      xnee_verbose((xd,"AutoRepeatModeOn\n"));
      /*@ignore@*/
      XAutoRepeatOn(xd->fake);
      /*@end@*/
    }
  else
    {
      xnee_verbose((xd,"AutoRepeatModeOff\n"));
      /*@ignore@*/
      XAutoRepeatOff(xd->fake);
      /*@end@*/
    }
  /* Make sure the resetting of autorepeat is handled
     before we close down the display */
  /*@ignore@*/
  XFlush (xd->fake);
  /*@end@*/
  xd->autorepeat_saved=0;  
  return XNEE_OK;
}


int 
xnee_set_replay_offset_str (xnee_data *xd, char *str)
{
  int x;
  int y;
  int ret;
  xnee_verbose((xd, "replay offset str: %s\n", str));

  if (str==NULL)
    {
      return XNEE_WRONG_PARAMS;
    }

  ret = sscanf(str, "%d,%d",&x,&y);
  
  if (ret != 2 )
    {
      xnee_verbose((xd, "replay offset failed: %d\n", ret));
      exit(ret);
    }

  xd->res_info.x_offset = x;
  xd->res_info.y_offset = y;

  xnee_verbose((xd, "replay offset OK: %dx%d\n", x,y));
  return XNEE_OK;
}

int 
xnee_set_replay_offset_x(xnee_data *xd, int offset)
{
  xd->res_info.x_offset = offset;
  return XNEE_OK;
}

int 
xnee_set_replay_offset_y(xnee_data *xd, int offset)
{
  xd->res_info.y_offset = offset;
  return XNEE_OK;
}

int 
xnee_get_replay_offset_x(xnee_data *xd)
{
  return xd->res_info.x_offset ;
}

int 
xnee_get_replay_offset_y(xnee_data *xd)
{
  return xd->res_info.y_offset ;
}

int
xnee_get_new_window_pos (xnee_data *xd)
{
  xnee_verbose((xd, "xnee_set_new_window_pos()\n"));
  return xd->xnee_info.store_window_pos;
}

int
xnee_set_new_window_pos (xnee_data *xd)
{
  xnee_verbose((xd, "xnee_set_new_window_pos()\n"));
  xd->xnee_info.store_window_pos=1;
  return XNEE_OK;
}

int
xnee_set_new_window_pos_value (xnee_data *xd, int val)
{
  xnee_verbose((xd, "xnee_set_new_window_pos(%d)\n", val));
  xd->xnee_info.store_window_pos=val;
  return XNEE_OK;
}

int
xnee_get_new_window_pos_value (xnee_data *xd)
{
  return xd->xnee_info.store_window_pos;
}

int
xnee_unset_new_window_pos (xnee_data *xd)
{
  xnee_verbose((xd, "xnee_unset_new_window_pos()\n"));
  xd->xnee_info.store_window_pos=0;
  return XNEE_OK;
}


int
xnee_get_recall_window_pos (xnee_data *xd)
{
  xnee_verbose((xd, "xnee_get_recall_window_pos()\n"));
  return xd->recall_recorded_win_pos;
}


int
xnee_set_recall_window_pos (xnee_data *xd)
{
  xnee_verbose((xd, "xnee_set_recall_window_pos()\n"));
  xd->recall_recorded_win_pos=1;
  return xnee_parse_range (xd, XNEE_DELIVERED_EVENT, "ReparentNotify");
}


int
xnee_unset_recall_window_pos (xnee_data *xd)
{
  xnee_verbose((xd, "xnee_unset_recall_window_pos()\n"));
  xd->recall_recorded_win_pos=0;
  return XNEE_OK;
}


int
xnee_set_project_file(xnee_data *xd, char *name)
{
  #define XNEE_PARSE_BUF 200
  char buf [XNEE_PARSE_BUF]; 
  int ret;

  if (name == NULL)
    {
      ret = XNEE_WRONG_PARAMS;
    }

  ret = xnee_set_rc_byname (xd, name);
  if ( ret != XNEE_OK) 
    {
      xnee_verbose((xd, "Could not open project file %s\n", name));
	      
      if ( (strlen(name) + strlen (XNEE_RESOURCE_DIR) + 2 ) > 200)
	{
	  xnee_verbose ((xd, "ERROR: Filename too big\n"));
	  xnee_verbose ((xd, "... leaving.\n"));
	  xnee_close_down(xd);
	  exit(XNEE_WRONG_PARAMS);
	}
      strncpy ( buf , XNEE_RESOURCE_DIR, XNEE_PARSE_BUF );
      strncat ( buf ,  "/", XNEE_PARSE_BUF - strlen(buf));
      strncat ( buf , name, XNEE_PARSE_BUF - strlen(buf));
      xnee_verbose((xd, "\ttryingresource file %s\n", buf));
      ret = xnee_set_rc_name (xd, buf);
    }
  if ( xnee_get_rc_file (xd) != NULL) 
    {
      ret = xnee_add_resource (xd);
	      
      if (ret!=XNEE_OK)
	{
	  xnee_verbose ((xd, "project file read: return value %d\n", 
			 ret));
	  if (ret == XNEE_SYNTAX_ERROR)
	    {
	      char *tmp_str;
	      xnee_verbose ((xd, "project file read: SYNTAX ERROR\n"));
	      tmp_str = xnee_get_err_string();
	      if (tmp_str!=NULL)
		{
		  fprintf (stderr,"%s", tmp_str);
		}
	      XNEE_FREE_IF_NOT_NULL(tmp_str);
	      return ret;
	    }
	}
    }
  else
    {
      xnee_print_error ("Unable to open resource file\n");
      xnee_verbose ((xd, "Could not open resource file\n"));
      xnee_verbose ((xd, "... leaving\n"));
      ret = XNEE_WRONG_PARAMS;
    }
  return ret;
}



char *
xnee_get_project_name(xnee_data *xd)
{
  if (xd->xrm.project_name!=NULL)
    {
      return xd->xrm.project_name;
    }
  else
    {
      return (char*)XNEE_EMPTY_STRING;
    }
}

char *
xnee_get_project_descr(xnee_data *xd){
  if (xd->xrm.project_descr!=NULL)
    {
      return xd->xrm.project_descr;
    }
  else
    {
      return (char*)XNEE_EMPTY_STRING;
    }
}

char *
xnee_get_creat_date(xnee_data *xd)
{
  time_t rawtime;
  struct tm * timeinfo;
#define XNEE_DATE_BUF_SIZE 100
  static char buf[XNEE_DATE_BUF_SIZE];

  if (xd->xrm.creat_date!=NULL)  
    {
      return xd->xrm.creat_date;
    }
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  snprintf(buf, XNEE_DATE_BUF_SIZE,
	   "%.4d-%.2d-%.2d",
	   timeinfo->tm_year + 1900 , 
	   timeinfo->tm_mon + 1 , 
	   timeinfo->tm_mday  );
  
  return buf;
}

char *
xnee_get_creat_program(xnee_data *xd)
{
  if (xd->xrm.creat_prog!=NULL)  
    return xd->xrm.creat_prog;
  else
    return xnee_get_program_name(xd);
}

char *
xnee_get_creat_prog_vers(xnee_data *xd)
{
  if (xd->xrm.creat_prog_vers!=NULL)  
    return xd->xrm.creat_prog_vers;
  else
    return VERSION;
}

char *
xnee_get_last_date(xnee_data *xd)
{
  if (xd->xrm.last_date!=NULL)  
    return xd->xrm.last_date;
  else
    return "none";
}

char *
xnee_get_last_program(xnee_data *xd)
{
  if (xd->xrm.last_prog!=NULL)  
    return xd->xrm.last_prog;
  else
    return "none";
}

char *
xnee_get_last_prog_vers(xnee_data *xd){
  if (xd->xrm.last_prog_vers!=NULL)  
    return xd->xrm.last_prog_vers;
  else
    return "none";
}

char *
xnee_get_author_name(xnee_data *xd){
  if (xd->xrm.author_name!=NULL)  
    return xd->xrm.author_name;
  else
    return "none";
}

char *
xnee_get_author_email(xnee_data *xd){
  if (xd->xrm.author_email!=NULL)  
    return xd->xrm.author_email;
  else
    return "none";
}


int
xnee_set_project_name(xnee_data *xd, char *str)
{
  if (str==NULL)
    {
      return XNEE_NO_PROJECT_FILE;
    }

  XNEE_FREE_IF_NOT_NULL(xd->xrm.project_name);
  xd->xrm.project_name=strdup(str);
  return XNEE_OK;
}

int
xnee_set_plugin_name(xnee_data *xd, char *str, unsigned char mode)
{
  if (str==NULL)
    {
      return XNEE_NO_PLUGIN_FILE;
    }

  return xnee_use_plugin(xd, str, mode);
}



int
xnee_set_project_descr(xnee_data *xd, char *str){
  xd->xrm.project_descr=strdup(str);
  return XNEE_OK;
}

int
xnee_set_creat_date(xnee_data *xd, char *str){
  XNEE_FREE_IF_NOT_NULL(xd->xrm.creat_date);
  xd->xrm.creat_date=strdup(str);
  return XNEE_OK;
}

int
xnee_set_creat_program(xnee_data *xd, char *str){
  XNEE_FREE_IF_NOT_NULL(xd->xrm.creat_prog_vers);
  xd->xrm.creat_prog=strdup(str);
  return XNEE_OK;
}

int
xnee_set_creat_prog_vers(xnee_data *xd, char *str){
  xd->xrm.creat_prog_vers=strdup(str);
  return XNEE_OK;
}

int
xnee_set_last_date(xnee_data *xd, char *str)
{
  XNEE_FREE_IF_NOT_NULL(xd->xrm.last_date);
  xd->xrm.last_date=strdup(str);
  return XNEE_OK;
}

int
xnee_set_last_program(xnee_data *xd, char *str){
  xd->xrm.last_prog=strdup(str);
  return XNEE_OK;
}
int
xnee_set_last_prog_vers(xnee_data *xd, char *str)
{
  xd->xrm.last_prog_vers=strdup(str);
  return XNEE_OK;
}
int
xnee_set_author_name(xnee_data *xd, char *str){
  xd->xrm.author_name=strdup(str);
  return XNEE_OK;
}

int
xnee_set_author_email(xnee_data *xd, char *str){
  xd->xrm.author_email=strdup(str);
  return XNEE_OK;
}

#ifdef USE_OBSOLETE
int
xnee_set_first_list_str2(xnee_data *xd, char *str)
{
  int ret = XNEE_OK;  
  
  if ( str != NULL)
    {
      ret = xnee_boolstr2int(xd, str);
    }
  if (ret == XNEE_BOOL_ERROR)
    {
      ret = XNEE_SYNTAX_ERROR;
    }
  else
    {
      if (ret)
	{
	  ret = xnee_set_first_last(xd);
	}
      else
	{
	  ret = xnee_unset_first_last(xd);
	}
    }
  return ret;
}
#endif

int
xnee_set_all_clients_str(xnee_data *xd, char *str)
{
  int ret = XNEE_OK;

  ret = xnee_boolstr2int(xd, str);
  if (ret == XNEE_BOOL_ERROR)
    {
      ret = XNEE_SYNTAX_ERROR;
    }
  else
    {
      if (ret)
	{
	  ret = xnee_set_all_clients(xd);
	}
      else
	{
	  ret = xnee_unset_all_clients(xd);
	}
    }
  
  return ret;
}


int
xnee_set_future_clients_str(xnee_data *xd, char *str)
{
  int ret = XNEE_OK;

  ret = xnee_boolstr2int(xd, str);
  if (ret == XNEE_BOOL_ERROR)
    {
      ret = XNEE_SYNTAX_ERROR;
    }
  else
    {
      if (ret)
	{
	  ret = xnee_set_future_clients(xd);
	}
      else
	{
	  ret = xnee_unset_future_clients(xd);
	}
    }
  return ret;
}

int
xnee_set_future_clients(xnee_data *xd)
{
  /* Setting future clients is the same as unsetting 
     all_clients*/
  return xnee_unset_all_clients(xd);
}

int
xnee_unset_future_clients(xnee_data *xd)
{
  /* Unsetting future clients is the same as setting
     all_clients*/
  return xnee_set_all_clients(xd);
}



int 
xnee_set_events_max_str (xnee_data *xd, char *str)
{
  int ret;
  
  ret = xnee_str2int (xd, str);

  if ( ret == INT_MAX )
    {
      ret = XNEE_SYNTAX_ERROR ;
    }
  else
    {
      ret = xnee_set_events_max(xd, ret);
    }
  return ret;
}

int 
xnee_set_data_max_str (xnee_data *xd, char *str)
{
  int ret;
  
  ret = xnee_str2int (xd, str);

  if ( ret == INT_MAX )
    {
      ret = XNEE_SYNTAX_ERROR ;
    }
  else
    {
      ret = xnee_set_data_max(xd, ret);
    }
  return ret;
}

int 
xnee_set_time_max_str (xnee_data *xd, char *str)
{
  int ret;
  
  ret = xnee_str2int (xd, str);

  if ( ret == INT_MAX )
    {
      ret = XNEE_SYNTAX_ERROR ;
    }
  else
    {
      ret = xnee_set_time_max(xd, ret);
    }
  return ret;
}

int 
xnee_set_resolution_str (xnee_data *xd, char *str)
{
  int ret = XNEE_OK;

  ret = xnee_boolstr2int(xd, str);
  if (ret == XNEE_BOOL_ERROR)
    {
      ret = XNEE_SYNTAX_ERROR;
    }
  else
    {
      if (ret)
	{
	  ret = xnee_set_resolution_used(xd);
	}
      else
	{
	  ret = xnee_unset_resolution_used(xd);
	}
    }
  return ret;
}


int 
xnee_set_sync_mode_str (xnee_data *xd, char *str)
{
  int ret = XNEE_OK;

  ret = xnee_boolstr2int(xd, str);
  if (ret == XNEE_BOOL_ERROR)
    {
      ret = XNEE_SYNTAX_ERROR;
    }
  else
    {
      if (ret)
	{
	  ret = xnee_set_sync_mode(xd);
	}
      else
	{
	  ret = xnee_unset_sync_mode(xd);
	}
    }
  return ret;
}

int 
xnee_unset_sync_mode_str (xnee_data *xd, char *str)
{
  int ret = XNEE_OK;

  ret = xnee_boolstr2int(xd, str);
  if (ret == XNEE_BOOL_ERROR)
    {
      ret = XNEE_SYNTAX_ERROR;
    }
  else
    {
      if (ret)
	{
	  ret = xnee_unset_sync_mode(xd);
	}
      else
	{
	  ret = xnee_set_sync_mode(xd);
	}
    }
  return ret;
}


int 
xnee_set_sync_mode(xnee_data *xd)
{
  xd->sync = True;
  return XNEE_OK;
}

int 
xnee_get_sync_mode(xnee_data *xd)
{
  return xd->sync ;
}

int 
xnee_unset_sync_mode(xnee_data *xd)
{
  xnee_verbose((xd, "Setting no sync\n"));
  xd->sync = False;
  return XNEE_OK;
}

int 
xnee_set_unsync_mode(xnee_data *xd)
{
  xnee_verbose((xd, "Setting no sync\n"));
  xd->sync = False;
  return XNEE_OK;
}

int 
xnee_get_unsync_mode(xnee_data *xd)
{
  return xd->sync ;
}

const char *
xnee_get_xosd_font(xnee_data *xd)
{
  return xnee_get_xosd_font_impl(xd);
}

int
xnee_set_xosd_font(xnee_data *xd, char *font_str)
{
  return xnee_set_xosd_font_impl(xd, font_str);
}


int
xnee_set_rec_resolution (xnee_data *xd, char *res_str)
{
  return xnee_str_to_res (res_str, &xd->res_info.record);
}
 
int
xnee_get_rec_resolution_x (xnee_data *xd)
{
  return xd->res_info.record.x_res;
}

int
xnee_get_rec_resolution_y (xnee_data *xd)
{
  return xd->res_info.record.y_res;
}

int
xnee_set_rec_resolution_y (xnee_data *xd, int res)
{
  xd->res_info.record.y_res = res;
  return XNEE_OK;
}

int
xnee_set_rec_resolution_x (xnee_data *xd, int res)
{
  xd->res_info.record.x_res = res;
  return XNEE_OK;
}


int
xnee_set_rep_resolution_y (xnee_data *xd, int res)
{
  xd->res_info.replay.y_res = res;
  return XNEE_OK;
}

int
xnee_set_rep_resolution_x (xnee_data *xd, int res)
{
  xd->res_info.replay.x_res = res;
  return XNEE_OK;
}


int
xnee_set_rep_resolution (xnee_data *xd, char *res_str)
{
  int ret ;
  ret = xnee_str_to_res (res_str, &xd->res_info.replay);

  return ret;
} 

int
xnee_get_rep_resolution_x (xnee_data *xd)
{
  return xd->res_info.replay.x_res;
}

int
xnee_get_rep_resolution_y (xnee_data *xd)
{
  return xd->res_info.replay.y_res;
}
 
xnee_data*
xnee_get_xnee_data (void)
{
  return xd_saved;
}

int
xnee_set_xnee_data (xnee_data *xd)
{
  xd_saved = xd;
  return XNEE_OK;
}
 
int
xnee_set_keep_autorepeat (xnee_data *xd)
{
  xd->keep_autorepeat = 1;
  return XNEE_OK;
}
 
int
xnee_set_retype_press_delay(xnee_data *xd, unsigned int delay)
{
  xd->retype.key_press_delay   = delay;
  return XNEE_OK;
}

int
xnee_set_retype_release_delay(xnee_data *xd, unsigned int delay)
{
  xd->retype.key_release_delay = delay;
  return XNEE_OK;
}


int
xnee_set_no_reparent_recording(xnee_data *xd)
{
  xd->no_reparent_recording = 1;
  return XNEE_OK;
}

int
xnee_unset_no_reparent_recording(xnee_data *xd)
{
  xd->no_reparent_recording = 0;
  return XNEE_OK;
}

int
xnee_is_no_reparent_recording(xnee_data *xd)
{
  return xd->no_reparent_recording ;
}


