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


#include "libxnee/xnee.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"



int
xnee_set_display_name (xnee_data *xd, char *disp)
{
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
xnee_set_out_byname (xnee_data *xd, char* out_name)
{
  xnee_set_out_name (xd, out_name);
  xd->out_file = fopen (xd->out_name,"w");
  return XNEE_OK;
}



int
xnee_set_err_file (xnee_data *xd, FILE* err)
{
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
xnee_set_err_byname (xnee_data *xd, char *err_name)
{
  xnee_set_err_name (xd, err_name);
  xd->err_file = fopen (xd->err_name,"w");
  return XNEE_OK;
}



int
xnee_set_rc_file (xnee_data *xd, FILE* rc)
{
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
  xnee_set_rc_name (xd, rc_name);
  xd->rc_file = fopen (xd->rc_name,"r");
  if (xd->rc_file ==NULL)
    return XNEE_FILE_NOT_FOUND;
  return XNEE_OK;
}



int
xnee_set_data_file (xnee_data *xd, FILE* data_file)
{
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
  xd->data_name=strdup(data);
  if (xd->data_name==NULL)
    {
      return XNEE_MEMORY_FAULT;
    }
  return XNEE_OK;
}

char *
xnee_get_data_name (xnee_data *xd)
{
  return xd->data_name;
}



int
xnee_set_data_name_byname (xnee_data *xd, char* data_name)
{
  xnee_set_data_name (xd, data_name);
  xd->data_file = fopen (data_name,"r");
  if (xd->data_file == NULL)
    return XNEE_FILE_NOT_FOUND;
  return XNEE_OK;
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
  xd->sync=XNEE_TRUE;
  return XNEE_OK;
}

int
xnee_unset_sync (xnee_data *xd)
{
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
is_xnee_replayer (xnee_data *xd)
{
  return (xd->mode==XNEE_REPLAYER);
}

int
xnee_is_distributor (xnee_data *xd)
{
  return (xd->mode==XNEE_DISTRIBUTOR);
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
xnee_set_stop_key (xnee_data *xd, int stop_key)
{
  xd->grab_keys->stop_key=stop_key;
  return XNEE_OK;
}

int
xnee_get_stop_key (xnee_data *xd)
{
  return xd->grab_keys->stop_key;
}

 
int
xnee_set_stop_mod (xnee_data *xd, int stop_mod)
{
  xd->grab_keys->stop_mod=stop_mod;
  return XNEE_OK;
}

int
xnee_get_stop_mod (xnee_data *xd)
{
  return xd->grab_keys->stop_mod;
}

 




int
xnee_set_pause_key (xnee_data *xd, int pause_key)
{
  xd->grab_keys->pause_key=pause_key;
  return XNEE_OK;
}

int
xnee_get_pause_key (xnee_data *xd)
{
  return xd->grab_keys->pause_key;
}

 
int
xnee_set_pause_mod (xnee_data *xd, int pause_mod)
{
  xd->grab_keys->pause_mod=pause_mod;
  return XNEE_OK;
}

int
xnee_get_pause_mod (xnee_data *xd)
{
  return xd->grab_keys->pause_mod;
}

 




int
xnee_set_resume_key (xnee_data *xd, int resume_key)
{
  xd->grab_keys->resume_key=resume_key;
  return XNEE_OK;
}

int
xnee_get_resume_key (xnee_data *xd)
{
  return xd->grab_keys->resume_key;
}

 
int
xnee_set_resume_mod (xnee_data *xd, int resume_mod)
{
  xd->grab_keys->resume_mod=resume_mod;
  return XNEE_OK;
}

int
xnee_get_resume_mod (xnee_data *xd)
{
  return xd->grab_keys->resume_mod;
}

 





 
int
xnee_set_force_replay (xnee_data *xd)
{
  xd->force_replay=XNEE_TRUE;
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
  xd->xnee_info->first_last = XNEE_TRUE;
  return XNEE_OK;
}


int 
xnee_get_first_last (xnee_data *xd){
  return xd->xnee_info->first_last;
}

int 
xnee_is_first_last (xnee_data *xd){
  return (xd->xnee_info->first_last==XNEE_TRUE);
}


int 
xnee_unset_first_last (xnee_data *xd)
{
  xd->xnee_info->first_last = XNEE_FALSE;
  return XNEE_OK;
}









int 
xnee_set_no_expose (xnee_data *xd)
{
  xd->xnee_info->no_expose = XNEE_TRUE;
  return XNEE_OK;
}


int 
xnee_get_no_expose (xnee_data *xd){
  return xd->xnee_info->no_expose;
}

int 
xnee_is_no_expose (xnee_data *xd){
  return (xd->xnee_info->no_expose==XNEE_TRUE);
}


int 
xnee_unset_no_expose (xnee_data *xd)
{
  xd->xnee_info->no_expose = XNEE_FALSE;
  return XNEE_OK;
}



int 
xnee_set_loops_left (xnee_data *xd, int loops)
{
  xd->xnee_info->loops_left = loops;
  return XNEE_OK;
}


int 
xnee_get_loops_left (xnee_data *xd){
  return xd->xnee_info->loops_left;
}


int 
xnee_set_interval (xnee_data *xd, int interval)
{
  xd->xnee_info->interval = interval ;
  return XNEE_OK;
}

int 
xnee_get_interval (xnee_data *xd)
{
  return xd->xnee_info->interval;
}


int 
xnee_set_human_printout (xnee_data *xd)
{
  xd->rec_callback = xnee_human_dispatch;
  return XNEE_OK;
}




int 
xnee_set_all_events (xnee_data *xd)
{
  xd->xnee_info->all_events = True;
  return XNEE_OK;
}

int 
xnee_is_all_events (xnee_data *xd)
{
  return (xd->xnee_info->all_events == XNEE_TRUE);
}
