/*****  
 *       Xnee's Not an Event Emulator  
 *                                                                    
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003 Henrik Sandklef                    
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


#include <signal.h>

#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/xnee_dl.h"
#include "libxnee/xnee_sem.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_fake.h"
#include "libxnee/datastrings.h"
#include "libxnee/xnee_grab.h"
#include "libxnee/xnee_km.h"
#include "libxnee/xnee_resolution.h"
#include "libxnee/xnee_resource.h"
#include "libxnee/xnee_callback.h"
#include "libxnee/xnee_range.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_error.h"
#include "libxnee/xnee_display.h"
#include "libxnee/xnee_sem.h"
#include "libxnee/xnee_session.h"
#include "libxnee/xnee_fileop.h"
#include "libxnee/xnee_alloc.h"

xnee_data *xd_global;


int
xnee_prepare(xnee_data *xd)
{
   int ret;

   xnee_verbose((xd, "--> xnee_prepare\n"));
   ret = xnee_open_files(xd);
   if (ret == XNEE_OK)
     {
       ret = xnee_rep_prepare(xd);
     }
   xnee_verbose((xd, "<-- xnee_prepare returning %d\n", ret));
   return ret;
}



/**************************************************************
 *                                                            *
 * xnee_get_max_range                                         *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_get_max_range (xnee_data *xd) 
{
  int i=0;
  int max_val=0;

  if (!xd->record_setup->active)
    {
      max_val = 0;
    }
  else
    {
      for ( i=0 ; i<XNEE_NR_OF_TYPES ; i++)
	{
	  max_val=XNEE_MAX (xd->xnee_info.data_ranges[i], max_val);
	}
      xnee_verbose((xd, "Returning max_val=%d\n", max_val));
      /* Returns the number of RecordRanges allocated so far */
    }

  return max_val;
}




/**************************************************************
 *                                                            *
 * xnee_close_down                                            *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_close_down(xnee_data* xd) 
{
   int ret ;

   if (xd==NULL)
   {
      return;
   }

  XNEE_DEBUG ( (stderr ," --> xnee_close_down() at 0 \n"  ));

  xnee_verbose((xd, "Freeing context "));

  xnee_verbose((xd, "xnee_close_down : ungrab -----> \n"));
  ret = xnee_ungrab_keys (xd);
  xnee_verbose((xd, "xnee_close_down : ungrab <---- %d\n", ret));
  xnee_verbose((xd, "xnee_close_down : xnee_reset_autorepeat ----> \n"));
  ret = xnee_reset_autorepeat (xd_global);
  xnee_verbose((xd, "xnee_close_down : xnee_reset_autorepeat <---- %d\n", ret));

  if (xd->plugin_handle!=NULL)
    {
      xnee_verbose((xd, "Closing plugin lib "));
      ret = xnee_dlclose(xd, xd->plugin_handle);
      xnee_verbose((xd, "Closing plugin lib <<--- %d\n", ret));
    }

#ifdef XNEE_USE_SEMAPHORES
  if (xd->buf_sem!=0)
    {
      xnee_verbose((xd, "Destroying buffer semaphore "));
      ret = sem_destroy(xd->buf_sem);
      xnee_verbose((xd, "Destroying buffer semaphore <--- %d\n ", ret));
    }
#endif

  xnee_verbose((xd, "Freeing modifier mapping memory %d ", xd->map));
  XFreeModifiermap(xd->map);

  xnee_verbose((xd, "Closing displays on host "));
  if ( xd->display==NULL ) 
    {
      XNEE_DEBUG ( (stderr ," --> xnee_close_down() at 0.1 \n"  ));
      xnee_verbose((xd, "localhost"));
    }
  else 
    {      
      XNEE_DEBUG ( (stderr ," --> xnee_close_down() at 0.2 \n"  ));
      xnee_verbose((xd, "%s\n", xd->display));
    }

  if ( xd->control!=NULL)  
    {
      XNEE_DEBUG ( (stderr ," --> xnee_close_down() at 0.3 \n"  ));
      xnee_verbose((xd, "Closing displays control=%d \n", (int) xd->control));
      XNEE_DEBUG ( (stderr ," --> xnee_close_down() at 0.3.1 \n"  ));
      XCloseDisplay ( xd->control );
    }

  if ( xd->fake!=NULL)  
    {
      XNEE_DEBUG ( (stderr ," --> xnee_close_down() at 0.3 \n"  ));
      xnee_verbose((xd, "Closing displays control=%d \n", (int) xd->fake));
      XNEE_DEBUG ( (stderr ," --> xnee_close_down() at 0.3.1 \n"  ));
      XCloseDisplay ( xd->fake );
    }

  XNEE_DEBUG ( (stderr ," --> xnee_close_down() at 0.4 \n"  )); 
  if ( xd->data!=NULL)  
    {
/*       printf ("close data in %s   ---> %d\n", __func__, xd->data); */
      xnee_verbose((xd, "Closing displays data=%d \n", (int) xd->data));
      XCloseDisplay ( xd->data );
    }
  
  XNEE_DEBUG ( (stderr ," --> xnee_close_down() at 0.41 \n"  )); 
  if ( xd->grab!=NULL)  
    {
      xnee_verbose((xd, "Closing displays data=%d \n", (int) xd->grab));
      XCloseDisplay ( xd->grab );
    }
  
  XNEE_DEBUG ( (stderr ," --> xnee_close_down() at 0.5 \n"  ));

  xnee_verbose((xd, "closing fds\n"));

  xnee_verbose((xd ," --  xnee_close_down() free data_file \n"  ));
  ret = xnee_free_file (xd, xd->data_name, xd->data_file);
  xd->data_file=NULL;
  xnee_verbose((xd ," --  xnee_close_down() free data_file <-- %d\n", ret));

  xnee_verbose((xd ," --  xnee_close_down() free rc_file \n"  ));
  ret = xnee_free_file (xd, xd->rc_name,   xd->rc_file);
  xd->rc_file=NULL;
  xnee_verbose((xd ," --  xnee_close_down() free rc_file <-- %d\n", ret));


  xnee_verbose((xd ," --  xnee_close_down() free rt_file \n"  ));
  ret = xnee_free_file (xd, xd->rt_name,  xd->rt_file);
  xd->rt_file=NULL;
  xnee_verbose((xd ," --  xnee_close_down() free rt_file <-- %d\n", ret));
  
  xnee_verbose((xd ," --  xnee_close_down() free err_file\n"  ));

  ret = xnee_free_file (xd, xd->err_name,  xd->err_file); 
  xd->err_file=NULL;

  ret = xnee_free_file (xd, xd->out_name,  xd->out_file);
  xd->out_file=NULL;

  ret = xnee_free_xnee_data(xd);
  xd=NULL;
}



/**************************************************************
 *                                                            *
 * xnee_init                                                  *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_init(xnee_data* xd)
{
  int ret ; 
  int i ; 
  char *tmp;
  
  if (xd == NULL) 
    {
      return XNEE_MEMORY_FAULT;
    }

  xd_global = xd ; 


  xd->app_args = NULL;

  xd->data     = NULL;
  xd->control  = NULL;
  xd->fake     = NULL;

  /*  xd->human_print = False  ; */
  
  xd->plugin_handle    = NULL;
  xd->rec_callback     = xnee_record_dispatch ;
  xd->rep_callback     = xnee_replay_dispatch ;
  xd->buffer_verbose_fp= fprintf ;
  xd->verbose_fp       = vfprintf ;
  xd->data_fp          = fprintf ;
  /* TODO   xd->sync_fun     = xnee_replay_dispatch ; */

  xd->verbose       = False  ; 
  xd->buf_verbose   = False  ; 
  xd->all_clients   = True   ; 
  xd->sync          = True   ; 
  xd->mode          = XNEE_NOMODE   ; 

  xd->xnee_info.store_window_pos = 0;
  
  /*@access FILE@*/
  xd->data_file     = (FILE*) stdin  ;
  /*@access FILE@*/
  xd->out_file      = (FILE*) stdout ;
  xd->saved_out_file= (FILE*) stdout ;
  /*@access FILE@*/
  xd->err_file      = (FILE*) stderr ;
  xd->saved_err_file= (FILE*) stderr ;
  /*@access FILE@*/
  xd->rc_file       = NULL   ;
  /*@access FILE@*/
  xd->rt_file       = (FILE*) stdin  ;
  /*@access FILE@*/
  xd->buffer_file   = (FILE*) stderr ;

  xd->data_name     = NULL  ;
  xd->out_name      = NULL  ;
  xd->err_name      = NULL  ;
  xd->rc_name       = NULL  ;
  xd->rt_name       = NULL  ;

  tmp = getenv ((const char*)"DISPLAY");
  if ( tmp != NULL )
    {
      xd->display = strdup(tmp);
    }
  else
    {
      xd->display = NULL;
    }
  xd->distr_list    = NULL  ;
  xd->distr_list_size = 0   ;
  xd->cont          = True ;
  xd->force_replay  = False;
  xnee_unset_recall_window_pos (xd);
  xd->first_read_time =0;
  
  xd->speed_percent = 100 ; 
#ifdef XNEE_USE_SEMAPHORES
  xd->buf_sem = (sem_t *) malloc (sizeof(sem_t));
  xnee_sem_init (xd, xd->buf_sem, 0, 1);
#endif
  xd->autorepeat_saved = 0 ;

  xd->button_pressed=0;
  /*  Not done until needed
      xnee_init_names();
  */

  for (i=0;i<XNEE_NR_OF_TYPES;i++)
    {
      xd->xnee_info.data_ranges[i]=0;
    }

  xnee_new_grab_keys(xd);

  xd->in_use = 1;

  /* 
   * resolution */
  xnee_set_default_rec_resolution (xd);
  
  /* 
   * meta data */
  xd->meta_data.sum_max    = 0;
  xd->meta_data.sum_min    = 0;

  xd->meta_data.total_diff = 0;
  xd->meta_data.cached_max = 0;
  xd->meta_data.cached_min = 0;

  xd->meta_data.sum_max_threshold  = XNEE_DEFAULT_MAX_THRESHOLD;
  xd->meta_data.sum_min_threshold  = XNEE_DEFAULT_MIN_THRESHOLD;
  xd->meta_data.tot_diff_threshold = XNEE_DEFAULT_TOT_THRESHOLD;


  xd->res_info.x_offset = 0;
  xd->res_info.y_offset = 0;

  /* Init Recording variables
   * Since those are used when recording and replaying. */
  xnee_record_init (xd);
  
  ret = xnee_resolution_init (xd);
  XNEE_RETURN_IF_ERR(ret);

  /* Set the signal handler the libxnee's built in */ 
  (void) signal (SIGINT, signal_handler);
  
  /* Set the error handler the libxnee's built in */ 
  (void) XSetErrorHandler (handle_xerr); 

  xnee_verbose((xd, "<--- xnee_init\n"));
  return XNEE_OK;

}


/**************************************************************
 *                                                            *
 * xnee_stop_session                                          *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_stop_session( xnee_data* xd)
{
   int ret ;
   xnee_verbose((xd, " ---> xnee_stop_session\n" ));
   xnee_verbose((xd, "xnee_stop_session : ungrab -----> \n"));
   xnee_verbose((xd, "xnee_stop_session : ungrab <---- \n"));
   ret = xnee_reset_autorepeat (xd);
   xnee_verbose((xd, " <--- xnee_stop_session %d\n", ret ));
  return (0);
}




int 
xnee_process_count(int mode)
{
  static int continue_process = 0;
  
  /* if we are to use make sure 
     xd != NULL */

  if (mode == XNEE_PROCESS_RESET )
    continue_process=0;
  else if (mode == XNEE_PROCESS_INC )
    continue_process++;
  else if (mode == XNEE_PROCESS_DEC )
    continue_process--;
  /* we need not care about XNEE_PROCESS_GET */

  return continue_process;
}



int 
xnee_process_replies(xnee_data *xd)
{

  int last_count;
  int count; 

  if ( (xd==NULL) || (xd->data==NULL) )
    {
      return (XNEE_MEMORY_FAULT);
    }

  last_count=xnee_process_count (XNEE_PROCESS_GET);
  (void) xnee_process_count (XNEE_PROCESS_RESET);

  /* it is more important to handle all data
     in the data display than to check for 
     grabbed key+modifier */
  while ( (1!=0) )
    {
      XRecordProcessReplies (xd->data); 
      /* XX reasons to stop calling RECORD to check buffer
       *    1) no data in buffer
       *    2) no data in buffer
       *    3) we have received XX data .. it is time to check if 
       *       user has presed any modifier+key
       *    4) number of data to record exceeded
       */
      count=xnee_process_count (XNEE_PROCESS_GET);
      if ( 
	  ( count <= 0) 
	  || 
	  ( last_count==count)
	  || 
	  ( count > 20 )
	  )
	{
	  break;
	}
      last_count=count;
    }
  (void)xnee_process_count (XNEE_PROCESS_RESET);
  return XNEE_OK;
}



int
xnee_rep_prepare(xnee_data *xd)
{
  int ret ; 

  xnee_verbose((xd, "--> xnee_rep_prepare\n"));
  /* 
   * Print settings 
   * only done if verbose mode  
   */
  ret = xnee_print_distr_list(xd, NULL);
  XNEE_RETURN_IF_ERR (ret);

/*   ret = xnee_set_ranges(xd); */
/*   XNEE_RETURN_IF_ERR (ret); */


  xnee_set_events_replayed(xd,0);

  if ( xnee_is_replayer(xd) != 0 )
  {
    xnee_verbose((xd, "Entering main loop (replayer) to read META data \n"));
    ret = xnee_replay_main_loop(xd, XNEE_REPLAY_READ_META_DATA);
  }

  /* 
   * Test Displays and Extensions  
   *
   */

  ret = xnee_setup_display (xd);
  if (ret!=XNEE_OK)
    {
      xnee_verbose((xd, "<-- xnee_rep_prepare returning %d\n", ret));
      return ret;
    }
  
  ret = xnee_setup_recordext (xd);
  if ( xnee_is_recorder(xd) != 0)
  {
    if (ret==XNEE_NO_PROT_CHOOSEN)
      {
	xnee_verbose((xd, "<-- xnee_rep_prepare returning %d\n", ret));
        return ret;
      }
  }

  /*
   * If no recording client, init xnee_sync 
   *
   */
  if ( xnee_is_recorder(xd) == 0 )
    {
      xnee_replay_init (xd);   
    }
  
  /*
   * Save repeat mode so we can reset it after we are done
   *
   */
  ret = xnee_set_autorepeat (xd);
  XNEE_RETURN_IF_ERR (ret);

/*   ret = xnee_refresh_ranges(xd); */
/*   XNEE_RETURN_IF_ERR (ret); */

  xnee_verbose((xd, "<-- xnee_rep_prepare returning %d\n", XNEE_OK));
  return XNEE_OK;
}


int
handle_xerr(Display *dpy, XErrorEvent *errevent)
{
  int ret ; 
  /*@out@*/ char tmp[100]="";

/*   fprintf (stderr, "Error received:  %d\n",  */
/* 	   errevent->error_code); */

  /*  if (errevent->type==0)
    {
       return XNEE_OK; 
    }
  else if (errevent->type==161)
    {
       return XNEE_OK; 
    }
  */

  fprintf (stderr, "Error received:\n");
  fprintf (stderr, "\terror type       %u (0x%x)\n", 
	  errevent->type,errevent->type);
  fprintf (stderr, "\terror code       %u (0x%x)\n", 
	  errevent->error_code,errevent->error_code);
  fprintf (stderr, "\terror code major %u (0x%x)\n", 
	  errevent->request_code,errevent->request_code);
  fprintf (stderr, "\terror code minor %u (0x%x)\n", 
	  errevent->minor_code,errevent->minor_code);
  fprintf (stderr, "\tdisplay          %d (0x%x)\n", 
	  (int)dpy, (unsigned int)dpy);

  ret = XGetErrorText (dpy, 
		       (int) errevent->error_code, 
		       tmp, 
		       100);
  fprintf (stderr, " %s\n", (char *)tmp);
  fprintf (stderr, "This error can be ignored\n");
  return XNEE_OK;
}



/**************************************************************
 *                                                            *
 * xnee_err_handler                                           *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_err_handler(Display* dpy, XErrorEvent* ev)
{
  int len=100;
  int ret ; 

  /*@out@*/ char tmp[100]="";

  printf ("xnee - X11 error handler \n");
  printf ("   type       %d \n",  ev->type);
  printf ("   serial     %lu \n", ev->serial);
  printf ("   error code %u \n", ev->error_code);
  printf ("   major code %u \n", ev->request_code);
  printf ("   minor code %u \n", ev->minor_code);
  
  ret = XGetErrorText (dpy, 
		       (int) ev->error_code, 
		       tmp, 
		       len);
  if (ret!=0)
  {
     fprintf (stderr, "Could not look up error string for error %d %lu %c %c %c \n",    
              ev->type, ev->serial, ev->error_code, ev->request_code, ev->minor_code);
  }

  printf ("   Message: %s\n", tmp);
  return ev->type;
}

/**************************************************************
 *                                                            *
 * signal_handler                                             *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
signal_handler(int sig) 
{
   int ret; 
  switch (sig)
    {
    case SIGTERM:
      fprintf  (stderr,  "sighandler SIGTERM (%d)\n", sig);
      ret = xnee_ungrab_keys(xd_global);
      XNEE_PRINT_ERROR_IF_NOT_OK(ret);
      
      ret = xnee_reset_autorepeat (xd_global);
      XNEE_PRINT_ERROR_IF_NOT_OK(ret);
      
      exit (sig);

    case SIGINT:
      fprintf  (stderr,  "sighandler SIGINT (%d)\n", sig);
      ret = xnee_ungrab_keys(xd_global);
      XNEE_PRINT_ERROR_IF_NOT_OK(ret);
      
      ret = xnee_reset_autorepeat (xd_global);
      XNEE_PRINT_ERROR_IF_NOT_OK(ret);
      
      exit (sig);
    default:
      fprintf (stderr, 
	       "signal_handler error. Unxpected signal (%d)\n .... leaving",
	       sig);
      exit (sig);
    }
}

int
xnee_more_to_record(xnee_data *xd)
{
  int evs;
  int dats;
  int tims;

  evs = xnee_get_events_left(xd);
  if (evs<0) evs = 1;

  dats = xnee_get_data_left(xd);
  if (dats<0) dats = 1;

  tims = xnee_get_time_left(xd);
  if (tims<0) tims = 1;

  return (int) ( (evs!=0) && (dats!=0) && (tims!=0) );
}


