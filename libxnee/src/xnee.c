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


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <dlfcn.h>

#include <sys/utsname.h>

#include <X11/Xproto.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>

#include <X11/extensions/record.h> 
#include <X11/extensions/XTest.h> 

#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/xnee_dl.h"
#include "libxnee/xnee_sem.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/datastrings.h"
#include "libxnee/xnee_grab.h"
#include "libxnee/xnee_km.h"
#include "libxnee/xnee_resolution.h"
#include "libxnee/xnee_resource.h"
#include "libxnee/xnee_callback.h"
#include "libxnee/xnee_range.h"
#include "libxnee/xnee_setget.h"



xnee_data *xd_global;
int masks[] =
  {
    0,
    ShiftMask,
    ControlMask,
    LockMask,
    Mod1Mask,
    Mod2Mask,
    Mod3Mask,
    Mod4Mask,
    Mod5Mask,
    -1
  } ;

/**************************************************************
 *                                                            *
 * xnee_get_max_range                                         *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_write_settings_to_file (xnee_data *xd, FILE *fp) 
{
   xnee_set_ranges(xd);
   xnee_print_xnee_resource_settings (xd, fp) ;
   xnee_print_xnee_settings (xd, fp) ;
   xnee_print_ranges (xd, fp);
   return XNEE_OK;
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
  for ( i=0 ; i<XNEE_NR_OF_TYPES ; i++)
    {
      max_val=XNEE_MAX (xd->xnee_info.data_ranges[i], max_val);
    }
  xnee_verbose((xd, "Returning max_val=%d\n", max_val));
  /* Returns the number of RecordRanges allocated so far */
  return max_val;
}




/**************************************************************
 *                                                            *
 * xnee_setup_display                                         *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_setup_display (xnee_data *xd)
{
  if (xd->data==NULL)
    xd->data     = xnee_open_display (xd);
  if (xd->data==NULL)
     return XNEE_NOT_OPEN_DISPLAY;
  
  if (xd->control==NULL)
    xd->control  = xnee_open_display (xd);
  if (xd->control==NULL)
     return XNEE_NOT_OPEN_DISPLAY;

  if (xd->fake==NULL)
    xd->fake     = xnee_open_display (xd);
  if (xd->fake==NULL)
     return XNEE_NOT_OPEN_DISPLAY;

  xnee_verbose((xd, "display data    %d\n" , (int) xd->data));
  xnee_verbose((xd, "display control %d\n" , (int) xd->control));
  xnee_verbose((xd, "display fake    %d\n" , (int) xd->fake));
  /* return XNEE_OK on success, 1  indicats error opening the displays */
  if ( ( xd->data != NULL ) && (xd->control) && (xd->fake) )
    return XNEE_OK;
  else 
    return 1;
}



/**************************************************************
 *                                                            *
 * xnee_delay                                                 *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_delay (int secs, char *mess) 
{
  int i;
  int j;
  int bytes_written = 0 ;
  int last_written  = 0 ;
  int len = strlen (mess);

  for ( i=secs ; i>-1 ; i-- ) 
    {
      if (len>0)
	{
	  fflush(stdout);
	  last_written= bytes_written  ;
	  bytes_written = fprintf (stdout,"\r%s delayed start, time left: %d seconds left", mess, i );
	  for ( j=0 ; j < ( last_written -bytes_written) ; j++ ) 
	    {
	      fprintf (stdout," \b");
	    }
	}
      sleep (1);
    }
  printf ("\r");
  for ( j=0 ; j < bytes_written ; j++ ) 
    {
      printf (" ");
    }
  printf ("\r");
  fflush(stdout);
}





/**************************************************************
 *                                                            *
 * xnee_check                                                 *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_check ( char *arg, char *long_arg , char *short_arg ) 
{
  /* returns 1 if arg was equal to any of long_arg or short_arg, 
     else it returns 0*/
  return ( ! strcmp ( arg, long_arg) || ( ! strcmp ( arg, short_arg) )) ; 
}



/**************************************************************
 *                                                            *
 * xnee_check_begin                                           *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_check_begin ( char *arg, char *long_arg , char *short_arg ) 
{
  int long_len = strlen (long_arg);
  int short_len = strlen (short_arg);

  if ( strncmp ( arg, long_arg, long_len) == 0)
    return XNEE_TRUE;
  if ( strncmp ( arg, short_arg, short_len) == 0)
    return XNEE_TRUE;
  
  return XNEE_FALSE;
}





/**************************************************************
 *                                                            *
 * xnee_set_default_display                                   *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_set_default_display (xnee_data *xd)
{
  xd->display = getenv ("DISPLAY");
}



/**************************************************************
 *                                                            *
 * xnee_open_display                                          *
 *                                                            *
 *                                                            *
 **************************************************************/
Display * 
xnee_open_display(xnee_data* xd)
{
  Display *dpy ;

  xnee_verbose((xd,  "Open display %s \n", xd->display ));
  
  dpy = XOpenDisplay (xd->display);
  if (!dpy) 
    {
      (void) xnee_print_error ("%s: unable to open display \"%s\"\n",
			       PACKAGE, 
			       XDisplayName(xd->display));
      return dpy;
    }
  xnee_verbose((xd, "Display %s = %d\n", xd->display, (int) dpy));
  return dpy;
}



int  
xnee_free_file (xnee_data *xd, char *file_name, FILE* file)
{
  if ( file_name != NULL) 
    {
      xnee_verbose((xd, "Closing file=%s fd=%d\n", 
		    file_name, 
		    (int)file));
      
      if ( ( file_name != NULL) && ( strlen(file_name)!=0))
	{
	  XNEE_FREE (file_name); 
	  if ( file != 0) 
	    {
	      XNEE_FCLOSE (file); 
	    }
	}
    }
  return XNEE_OK;
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
  XNEE_DEBUG ( (stderr ," --> xnee_close_down() at 0 \n"  ));

  xnee_verbose((xd, "Freeing context "));

  if (xd->grab_keys==NULL)
      xnee_verbose((xd, " ---  xnee_close_down the grab data seem to have been freed already\n"));
  else
      xnee_verbose((xd, " ---  xnee_close_down the grab exists.....\n"));
    
  xnee_verbose((xd, "xnee_close_down : ungrab -----> \n"));
  xnee_ungrab_keys (xd);
  xnee_verbose((xd, "xnee_close_down : ungrab <---- \n"));
  xnee_reset_autorepeat (xd_global);

  if (xd->plugin_handle!=NULL)
    {
      xnee_verbose((xd, "Closing plugin lib "));
      xnee_dlclose(xd, xd->plugin_handle);
    }

  if (xd->buf_sem!=0)
    {
      xnee_verbose((xd, "Destroying buffer semaphore "));
      sem_destroy (xd->buf_sem);
    }


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
      /*      XCloseDisplay ( xd->control );*/
    }

  if ( xd->fake!=NULL)  
    {
      XNEE_DEBUG ( (stderr ," --> xnee_close_down() at 0.3 \n"  ));
      xnee_verbose((xd, "Closing displays control=%d \n", (int) xd->fake));
      XNEE_DEBUG ( (stderr ," --> xnee_close_down() at 0.3.1 \n"  ));
      /*      XCloseDisplay ( xd->control );*/
    }

  XNEE_DEBUG ( (stderr ," --> xnee_close_down() at 0.4 \n"  )); 
  if ( xd->data!=NULL)  
    {
      xnee_verbose((xd, "Closing displays data=%d \n", (int) xd->data));
      /*      XCloseDisplay ( xd->data );*/
    }
  
  XNEE_DEBUG ( (stderr ," --> xnee_close_down() at 0.5 \n"  ));

  xnee_verbose((xd, "closing fds\n"));

  xnee_verbose((xd ," --  xnee_close_down() free data_file \n"  ));
  xnee_free_file (xd, xd->data_name, xd->data_file);

  xnee_verbose((xd ," --  xnee_close_down() free rc_file \n"  ));
  xnee_free_file (xd, xd->rc_name,   xd->rc_file);

  if (xd->err_file!=stdout)
    {
      xnee_verbose((xd ," --  xnee_close_down() free out_file \n"  ));
      xnee_free_file (xd, xd->out_name,  xd->out_file);
    }

  if (xd->rt_file!=stdout)
    {
      xnee_verbose((xd ," --  xnee_close_down() free rt_file \n"  ));
      xnee_free_file (xd, xd->rt_name,  xd->rt_file);
    }

  if (xd->err_file!=stderr)
    {
      xnee_verbose((xd ," --  xnee_close_down() free err_file\n"  ));
      xnee_free_file (xd, xd->err_name,  xd->err_file); 
    }

  xnee_verbose((xd, "finished closing fds\n"));


  XNEE_DEBUG ( (stderr ," --> xnee_close_down() at 0.6 \n"  ));
  xnee_verbose((xd, "Freeeing data "));
  xnee_free_xnee_data(xd);

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
  if (xd == NULL) 
    {
      return XNEE_MEMORY_FAULT;
    }

  xd_global = xd ; 


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

  xd->data_file     = stdin  ;
  xd->out_file      = stdout ;
  xd->err_file      = stderr ;
  xd->rc_file       = NULL   ;
  xd->rt_file       = NULL   ;
  xd->buffer_file   = stderr ;

  xd->data_name     = NULL  ;
  xd->out_name      = NULL  ;
  xd->err_name      = NULL  ;
  xd->rc_name       = NULL  ;
  xd->rt_name       = NULL  ;

  xd->display       = getenv ("DISPLAY");
  xd->distr_list    = NULL  ;
  xd->distr_list_size = 0   ;
  xd->cont          = True ;
  xd->force_replay  = False;
  xd->first_read_time =0;
  
  xd->speed_percent = 100 ; 
  xd->buf_sem = (sem_t *) malloc (sizeof(sem_t));
  xnee_sem_init (xd, xd->buf_sem, 0, 1);

  xd->autorepeat_saved = 0 ;

  xd->button_pressed=0;
  /*  Not done until needed
      xnee_init_names();
  */

  
  xnee_set_resolution_used (xd);
  xnee_resolution_init (xd);
  xnee_grab_keys_init(xd);


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


  /* Init Recording variables
   * Since those are used when recording and replaying. */
  xnee_record_init (xd);
  
  

  xnee_verbose((xd, "<--- xnee_init\n"));
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
  char tmp[100];
  
  printf ("xnee - X11 error handler \n");
  printf ("   type       %d \n",  ev->type);
  printf ("   serial     %lu \n", ev->serial);
  printf ("   error code %d \n", ev->error_code);
  printf ("   major code %d \n", ev->request_code);
  printf ("   minor code %d \n", ev->minor_code);
  
  XGetErrorText (dpy, ev->error_code, tmp, len);
  printf ("   Message: %s\n", tmp);
  return ev->type;
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
  xnee_verbose((xd, " ---> xnee_stop_session\n" ));
  xnee_verbose((xd, "xnee_stop_session : ungrab -----> \n"));
  xnee_ungrab_keys (xd);
  xnee_verbose((xd, "xnee_stop_session : ungrab <---- \n"));
  xnee_reset_autorepeat (xd);
  xnee_verbose((xd, " <--- xnee_stop_session\n" ));
  return (0);
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
  switch (sig)
    {
    case SIGTERM:
      fprintf  (stderr,  "sighandler SIGTERM (%d)\n", sig);
      xnee_ungrab_keys(xd_global);
      xnee_reset_autorepeat (xd_global);
      exit (sig);
    case SIGINT:
      fprintf  (stderr,  "sighandler SIGINT (%d)\n", sig);
      xnee_ungrab_keys(xd_global);
      xnee_reset_autorepeat (xd_global);
      exit (sig);
    default:
      fprintf (stderr, 
	       "signal_handler error. Unxpected signal (%d)\n .... leaving",
	       sig);
      exit (sig);
    }
}





/**************************************************************
 *                                                            *
 * xnee_new_recordext_setup                                   *
 *                                                            *
 *                                                            *
 **************************************************************/
xnee_recordext_setup*
xnee_new_recordext_setup()
{
  xnee_recordext_setup* xrs = (xnee_recordext_setup*) malloc (sizeof (xnee_recordext_setup));
  memset (xrs, 0, sizeof (xnee_recordext_setup));

  xrs->xids     = (XRecordClientSpec*) calloc (2, sizeof(XRecordClientSpec)); 
  xrs->rContext = 0;

  xrs->rState   = (XRecordState*) malloc (sizeof(XRecordState)); 
  memset (xrs->rState, 0, sizeof (XRecordState));
  /* 
   * Record ranges are allocated using seperate functions
   */
  return xrs;
}



/**************************************************************
 *                                                            *
 * xnee_free_recordext_setup                                  *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_free_recordext_setup(xnee_data* xd)
{
  int i;
  int max=xnee_get_max_range(xd);

  XNEE_DEBUG ( (stderr ," -->xnee_free_recordext_data()  \n"  ));
  if (xd->control!=NULL)
  {
    if ( xd->record_setup->rContext != 0)
      {
	xnee_unsetup_recording(xd);
      }
  }


  if (max>0)
    {
      for ( i=0 ; i<max ; i++ )
	{
	  XNEE_DEBUG ( (stderr, "   freeing range at %d since max=%d\n", i, max));
	  free (xd->record_setup->range_array[i]);
	}
    }

  free (xd->record_setup->xids);
  free (xd->record_setup->rState);
  free (xd->record_setup);
  XNEE_DEBUG ( (stderr ," <--xnee_free_xnee_data()\n"  ));
  return XNEE_OK;
}


int
xnee_new_dyn_data(xnee_data *xd)
{
   xnee_verbose((xd, "---> xnee_new_dyn_data\n"));

/*    xnee_verbose((xd, " --- xnee_new_dyn_data: xnee_info\n")); */
/*    xd->xnee_info     =   */
/*      (xnee_record_init_data*)  malloc (sizeof (xnee_record_init_data)) ;  */
/*    memset (xd->xnee_info, 0, sizeof(xnee_record_init_data));  */

   xnee_verbose((xd, " --- xnee_new_dyn_data: replay_setup\n"));
   xd->replay_setup  = 
      (xnee_testext_setup*)     malloc (sizeof (xnee_testext_setup)) ;
   memset (xd->replay_setup, 0, sizeof(xnee_testext_setup));
   
   xnee_verbose((xd, " --- xnee_new_dyn_data: record_setup\n"));
   xd->record_setup  = xnee_new_recordext_setup(); 
   
   xnee_verbose((xd, " --- xnee_new_dyn_data: grab_keys\n"));
   xd->grab_keys     = xnee_new_grab_keys();
   xnee_verbose((xd, "<--- xnee_new_dyn_data\n"));

  return XNEE_OK;
}


int 
xnee_reset_xnee_info(xnee_data *xd)
{
  int i ; 
  for (i=0;i<XNEE_NR_OF_TYPES;i++)
    {
      xd->xnee_info.data_ranges[i]=0;
    }
  return XNEE_OK;
}


int
xnee_free_dyn_data(xnee_data *xd)
{
   xnee_verbose((xd, "---> xnee_free_dyn_data\n"));

/*    xnee_verbose((xd, " --- xnee_free_dyn_data: refreshing ranges\n")); */

   if (xnee_is_replayer(xd))
     xnee_refresh_ranges(xd); 

   xnee_verbose((xd, " --- xnee_free_dyn_data: record_ext\n"));
   xnee_free_recordext_setup ( xd);

   xnee_verbose((xd, " --- xnee_free_dyn_data: grab_keys\n"));
   xnee_free_grab_keys(xd);

   xnee_verbose((xd, " --- xnee_free_dyn_data: resource_meta\n"));
   xnee_free_xnee_resource_meta(&xd->xrm);

   xnee_verbose((xd, " --- xnee_free_dyn_data: replay_setup\n"));
   free ( xd->replay_setup);

/*    xnee_verbose((xd, " --- xnee_free_dyn_data: xnee_info\n"));  */
/*    free (xd->xnee_info);  */
   xnee_reset_xnee_info(xd);

   xnee_verbose((xd, "<--- xnee_free_dyn_data\n"));
   return XNEE_OK;
}




/**************************************************************
 *                                                            *
 * xnee_new_xnee_data                                         *
 *                                                            *
 *                                                            *
 **************************************************************/
xnee_data* 
xnee_new_xnee_data()
{

  xnee_data* xd  = (xnee_data*) malloc (sizeof (xnee_data));
  if (xd==NULL)
    {
      xnee_print_error ("Could not allocate memory ........\n");
      exit (XNEE_MEMORY_FAULT);
    }
  memset (xd, 0, sizeof(xnee_data));

  xnee_new_dyn_data(xd);

  xnee_init(xd);

  return xd;
}


/**************************************************************
 *                                                            *
 * xnee_free_xnee_data                                        *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_free_xnee_data(xnee_data* xd)
{
  xnee_free_dyn_data(xd);
  
  free (xd->buf_sem);
  free (xd);
  return XNEE_OK;
}


/**************************************************************
 *                                                            *
 * xnee_client_id                                             *
 *                                                            *
 *                                                            *
 **************************************************************/
XID 
xnee_client_id (Display *dpy) 
{
  return dpy->resource_base ;
}




/**************************************************************
 *                                                            *
 * xnee_add_display_list                                      *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_add_display_list ( xnee_data* xd, char * disp)
{
  char buf[10];
  int next;
  int len;
  int ret=0;
  int disp_len=strlen(disp);
  xnee_verbose((xd, "xnee_add_display (%s, ... )\n", disp));
  while ( 1 ) {
    next=strcspn (disp, ",");
    if (next==0) break; 
    strncpy(buf,disp,next);
    buf[next]='\0';
    len =strlen(disp);
    disp+=next+1;
    disp_len=disp_len - next - 1;
    xnee_verbose((xd,  " ------------------------- (%s) %d    %d     %d\n", buf, len, disp_len, disp_len));
    ret=xnee_add_display_str (buf,xd);
    xnee_verbose((xd,  " ------------------------- are we doing fine???\n"));
    if (ret!=0) 
      {
	xnee_verbose((xd, "Could not add all display for distribution ... returning\n"));
	return (ret);
      }
    if (disp_len<=0) 
      {
	xnee_verbose((xd, "disp_len = %d\n", disp_len));
	break;
      }
  }
  xnee_verbose((xd, "xnee_add_display_str ... finished\n"));
  return (0);  
}





/**************************************************************
 *                                                            *
 * xnee_add_display_str                                       *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_add_display_str (char * disp_str, xnee_data* xd)
{
  Display *dpy;
  int xtest_event_basep;
  int xtest_error_basep;
  int xtest_version_major;
  int xtest_version_minor;
  
  xnee_verbose((xd, "Adding Display \"%s\" to distribution list\n", disp_str));
  
  xnee_verbose((xd, "Adding Display - allocating memory \n", disp_str));
  if (xd->distr_list_size==0) 
    {
      xnee_verbose((xd, 
		    "Adding Display - allocating memory for the first time\n"
		    ));
      xd->distr_list = (xnee_distr *) calloc (1,sizeof (xnee_distr));
    }
  else 
    {
      xnee_verbose((xd, 
		    "Adding Display - allocating memory for the %d time\n", 
		    xd->distr_list_size));
      xd->distr_list = (xnee_distr *) realloc (xd->distr_list, 
						xd->distr_list_size);
    }
  
  
  xnee_verbose((xd, "Adding Display  - opening display\n"));
  dpy = XOpenDisplay (disp_str);
  if (dpy==NULL)
    {
      xnee_print_error ("Could not open display %s\n", disp_str);
      exit ( XNEE_NOT_OPEN_DISPLAY );
    }
  xnee_verbose((xd, "Adding Display  - opening display gave us %d\n", dpy));
  
  xnee_verbose((xd, "Adding Display  - trying to grab control\n"));
  XTestGrabControl (dpy, True);

  if (!dpy) 
    {
      (void) xnee_print_error ("Unable to open display \"%s\"\n",
			       disp_str);
      return (XNEE_NOT_OPEN_DISPLAY);
    }
  xnee_verbose((xd, "Adding Display  - does the display/host have XTest\n"));
  if( !XTestQueryExtension(dpy,
			   &xtest_event_basep,
			   &xtest_error_basep,
			   &xtest_version_major,
			   &xtest_version_minor) )
    {
      xnee_print_error ("XTest extension missing on display %s \n", disp_str);
      return (XNEE_NO_TEST_EXT);
    }
  xnee_verbose ((xd, "  XTest  Release on \"%s\"         %d.%d\n", 
		disp_str,
		xtest_version_major,
		xtest_version_minor));
  
  xnee_verbose ((xd, "adding to distr list at pos %d\n", 
		 xd->distr_list_size));
  xnee_verbose ((xd, " -- resolution to 0\n"));
  xd->distr_list[xd->distr_list_size].is_used=0;
  
  xnee_verbose ((xd, "\t width %d \n"));
  xd->distr_list[xd->distr_list_size].res.x_res=DisplayWidth  (dpy, 0);
  xnee_verbose ((xd, "\t height\n"));
  xd->distr_list[xd->distr_list_size].res.y_res=DisplayHeight (dpy, 0);
  xnee_verbose ((xd, "\t dpy \n"));
  xd->distr_list[xd->distr_list_size].dpy=dpy;


  xnee_verbose ((xd, "cheking if resolution differs\n"));
  if (xnee_res_cmp(&xd->distr_list[xd->distr_list_size].res,
		   &xd->res_info.record)==0)
    {
      xnee_verbose ((xd, " -- resolution differs\n"));
      xd->distr_list[xd->distr_list_size].is_used=1;
    }
  xd->distr_list_size++;
  
  return (XNEE_OK);
}




/**************************************************************
 *                                                            *
 * xnee_add_display                                           *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_add_display (Display *dpy, xnee_data* xd)
{
  int xtest_event_basep;
  int xtest_error_basep;
  int xtest_version_major;
  int xtest_version_minor;

  xnee_verbose((xd, "Adding Display \"%d\" to distribution list\n", dpy));
  if (xd->distr_list_size==0) 
    {
      xd->distr_list = (xnee_distr *) calloc (1,sizeof (Display));
    }
  else 
    {
      xd->distr_list = (xnee_distr *) realloc (xd->distr_list, 
					       xd->distr_list_size);
    }
  XTestGrabControl (dpy, True);

  if( !XTestQueryExtension(dpy,
			   &xtest_event_basep,
			   &xtest_error_basep,
			   &xtest_version_major,
			   &xtest_version_minor) )
    {
      xnee_print_error ("XTest extension missing on display %d \n", dpy);
      return (XNEE_NO_TEST_EXT);
    }
  xnee_verbose ((xd, "  XTest  Release on \"%d\"         %d.%d\n", 
		dpy,
		xtest_version_major,
		xtest_version_minor));
  
  xd->distr_list[xd->distr_list_size].dpy=dpy;
  xd->distr_list_size++;
  
  return (XNEE_OK);
}



/**************************************************************
 *                                                            *
 * xnee_strip                                                 *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_strip(xnee_data *xd, char *str) 
{
  int i,j, blanks=0, len  ;
  len = strlen (str);

  xnee_verbose ((xd," --> xnee_strip \"%s\"\n", str));
  /*
   * how many blanks do we have  */
  for (i=0 ; i<=len ; i++)
    {
      if  (str[i]==' ' ) 
	{
	  blanks++;
	  for (j=i ; j<=len ; j++)
	    {
	      str[j]=str[j+1];
	      len--;
	    }
	  i--;
	}
    }
  str[len-blanks]='\0';
  xnee_verbose ((xd," <-- xnee_strip \"%s\"\n", str));
  return 1;
} 





/**************************************************************
 *                                                            *
 * xnee_rem_comment_start                                     *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_rem_comment_start(xnee_data *xd, char *str) 
{
  int i,j, comms=0, len  ;

  if (str==NULL)
    return 1;

  xnee_verbose ((xd," --> xnee_rem_comment_start \"%s\"\n", str));


  len = strlen (str);
  
  /*
   * how many comments do we have  */
  for (i=0 ; i<=len ; i++)
    {
      if ( str[i]==XNEE_COMMENT_START_CHAR )
	{
	  comms++;
	  for (j=i ; j<=len ; j++)
	    {
	      str[j]=str[j+1];
	    }
	  i--;
	}
    }

  str[len-comms]='\0';
  xnee_verbose ((xd," <-- xnee_rem_comment_start \"%s\"\n", str));
  return 0;
} 





/**************************************************************
 *                                                            *
 * xnee_use_plugin                                            *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_use_plugin(xnee_data *xd, char *pl_name)
{
  int ret=0;
  char lib_name[100];

  xnee_verbose ((xd, "Using plugin file: %s\n", pl_name));
  strcpy (lib_name, "lib");
  strcat (lib_name, pl_name);

  xd->plugin_name=strdup(pl_name);
  xd->plugin_handle = (void*) xnee_dlopen (xd, lib_name, RTLD_LAZY );
  if (!xd->plugin_handle) 
    {
      fputs (dlerror(), stderr);
      xnee_close_down(xd);
      exit(XNEE_PLUGIN_FILE_ERROR);
    }
  
  xnee_verbose ((xd, "We've got plugin file handle %d\n", xd->plugin_handle));

  ret = xnee_set_callback (xd, 
			   &xd->rec_callback,
			   XNEE_RECORD_CALLBACK_NAME);
  if ( ret != XNEE_OK )
    {
      /*
      xnee_close_down(xd);
      exit(XNEE_PLUGIN_FILE_ERROR);
      */
    }
  
  ret = xnee_set_callback (xd, 
			   &xd->rep_callback,
			   XNEE_REPLAY_CALLBACK_NAME);
  if ( ret != XNEE_OK )
    {
      /*      
	      xnee_close_down(xd);
	      exit(XNEE_PLUGIN_FILE_ERROR);
      */
    }
  
   
   
  ret = xnee_set_callback (xd, 
			   &xd->sync_fun,
			   XNEE_SYNC_FUNCTION_NAME);
  if ( ret != XNEE_OK )
    {
      /*      
      xnee_close_down(xd);
      exit(XNEE_PLUGIN_FILE_ERROR);
      */
    }
  
   return XNEE_OK;
}

/**************************************************************
 *                                                            *
 * xnee_unuse_plugin                                            *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_unuse_plugin(xnee_data *xd)
{
  XNEE_FREE_IF_NOT_NULL(xd->plugin_name);
  xnee_dlclose (xd, xd->plugin_handle);
  xd->plugin_handle = NULL;

  xd->rec_callback     = xnee_record_dispatch ;
  xd->rep_callback     = xnee_replay_dispatch ;
  return XNEE_OK;
}






/**************************************************************
 *                                                            *
 * rem_blanks                                                 *
 *                                                            *
 *                                                            *
 **************************************************************/
int
rem_blanks (char *array, int size)
{
  int i=0;
  int j=0;
  for (i=0;i<size;i++)
    {
      if ( (array[i]==' ') ||  (array[i]=='\t') ||  (array[i]=='\n') )
	{
	  for (j=i;j<size-1;j++)
	    {
	      array[j]=array[j+1];
	    }
	  array[j]='\0';
	}
    }
  return XNEE_OK;
}


/**************************************************************
 *                                                            *
 * rem_begin_blanks                                           *
 *                                                            *
 *                                                            *
 **************************************************************/
int
rem_begin_blanks (char *array, int size)
{
  int i=0;
  int j=0;
  for (i=0;i<size;i++)
    {
      if ( (array[i]==' ') ||  (array[i]=='\t') ||  (array[i]=='\n') )
	{
	  for (j=i;j<size-1;j++)
	    {
	      array[j]=array[j+1];
	    }
	  i--;
	  array[j]='\0';
	}
      else
	{
	  return 0;
	}
    }
  return XNEE_OK;
}


/**************************************************************
 *                                                            *
 * rem_all_blanks                                             *
 *                                                            *
 *                                                            *
 **************************************************************/
int
rem_all_blanks (char *array, int size)
{
  int i=0;
  int j=0;
  int zeros=0;


  for (i=0;i<size;i++)
    {
      if ( (array[i]==' ') ||  (array[i]=='\t') ||  (array[i]=='\n') )
	{
	  zeros++;
	  for (j=i;j<size;j++)
	    {
	      array[j]=array[j+1];
	    }
	  i--;
	  array[j-zeros]='\0';
	}
    }
  return XNEE_OK;
}







/**************************************************************
 *                                                            *
 * xnee_get_record_config                                     *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_get_record_config (xnee_data* xd) 
{
  char tmp[256] ;
  int processing_meta_data = XNEE_TRUE;

  strcpy(tmp,"");
  if ( xd->data_file == NULL)
    {
      xnee_verbose ((xd, "Using stdin as file\n"));
      xd->data_file=stdin;
    }
  while ( processing_meta_data )
    {
      fgets(tmp, 256, xd->data_file);
      xnee_verbose ((xd, "tmp: >%s", tmp));
      if ( tmp == NULL) 
	{
	  return XNEE_OK;
	}
      if (tmp[0] == '#' )  /* # META data */
	{
	  processing_meta_data = xnee_handle_meta_data(xd, tmp);
	}
      else 
	{
	  (void)xnee_print_error("Unexpected data read in file %s data: %s\n", xd->data_file, tmp);
	}  
    }
  return 1;
}
/**************************************************************
 *                                                            *
 * xnee_check_inSync                                          *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_check_inSync (xnee_data *xd )
{
  int i;
  
  for (i=0;i<XNEE_REPLAY_BUFFER_SIZE;i++)
    {
      /* For all request and events  
	 check if there are any expected ones we haven't received */
      if ( xd->data_buffer[XNEE_REQUEST][i] > 0 || xd->data_buffer[XNEE_EVENT][i] > 0 ) 
	{
	  xnee_verbose ((xd,"xnee_check_inSync -> NOT IN SYNC waiting on type: %d events: %d requests: %d\n", 
			i,  xd->data_buffer[XNEE_EVENT][i], xd->data_buffer[XNEE_REQUEST][i]));
	  
	  /* sometime on AIX we get invalid events - haven't figured out why yet */
	  if ( xd->data_buffer[XNEE_EVENT][i] > 0 && i > LASTEvent )
	    {
	      xnee_verbose ((xd,"xnee_check_inSync -> Not waiting on the weird event: %d > LASTEvent(35)%d\n", i ));
	    }
	  else
	    {
	      return XNEE_FALSE; 
	    }
	}
    }
  xnee_verbose ((xd,"xnee_check_inSync -> IN SYNC\n"));
  return XNEE_TRUE;
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
  last_count=xnee_process_count (XNEE_PROCESS_GET);
  xnee_process_count (XNEE_PROCESS_RESET);

  /* it is more important to handle all data
     in the data display than to check for 
     grabbed key+modifier */
  while ( 1 )
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
  xnee_process_count (XNEE_PROCESS_RESET);
  return XNEE_OK;
}


int
xnee_set_autorepeat (xnee_data *xd)
{
  int i ;
  int j;

  if (xd->autorepeat_saved==1)
    {
      return XNEE_OK;
    }
  

  XGetKeyboardControl (xd->fake, &xd->kbd_orig);
  
  xnee_verbose ((xd," key_click_percent  %d \n", 
		 xd->kbd_orig.key_click_percent));
  xnee_verbose ((xd," bell_percent       %d\n", 
		 xd->kbd_orig.bell_percent));
  xnee_verbose ((xd," bell_pitch         %d\n", 
		 xd->kbd_orig.bell_pitch));
  xnee_verbose ((xd," bell_duration      %d\n", 
		 xd->kbd_orig.bell_duration));
  xnee_verbose ((xd," led_mask           %d\n",  
		 xd->kbd_orig.led_mask));
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
  XAutoRepeatOff(xd->fake);
  xd->autorepeat_saved=1;  
  return xd->kbd_orig.global_auto_repeat;
}


int
xnee_reset_autorepeat (xnee_data *xd)
{  
  if (xd->autorepeat_saved==0)
    {
      return XNEE_OK;
    }

  xnee_verbose((xd,"Resetting autorepeat on (%d) to: ",
		xd->fake));

  if (!xd->fake)
    return XNEE_OK;

  if (xd->kbd_orig.global_auto_repeat==AutoRepeatModeOn)
    {
      xnee_verbose((xd,"AutoRepeatModeOn\n"));
      XAutoRepeatOn(xd->fake);
    }
  else
    {
      xnee_verbose((xd,"AutoRepeatModeOff\n"));
      XAutoRepeatOff(xd->fake);
    }
  /* Make sure the resetting of autorepeat is handled
     before we close down the display */
  XFlush (xd->fake);
  xd->autorepeat_saved=0;  
  return XNEE_OK;
}


KeyCode
xnee_str2keycode(xnee_data* xd, char *str, xnee_key_code *kc)
{
  if (xd->fake==NULL)
    return -1;
  kc->kc = XKeysymToKeycode(xd->fake,XStringToKeysym(str));
  if (kc!=NULL)
    xnee_token_to_km (xd,kc->kc, str,kc);
  return kc->kc;
}


KeyCode
xnee_keysym2keycode(xnee_data* xd, KeySym ks, char * str, xnee_key_code *kc)
{
  if (xd->fake==NULL)
    return -1;

  kc->kc = XKeysymToKeycode(xd->fake, ks);
  xnee_token_to_km (xd,kc->kc,str,kc);
  return kc->kc;
}


int
xnee_token_to_km (xnee_data *xd,
		  int keycode,
		  char *str,
		  xnee_key_code *kc)
{
  XEvent event;
  int size;
  int i ;
  int k ;
  int ret=-1;
  char string[20];
  KeySym keysym;
  Display *dpy = xd->fake;
  
  for (i=0;masks[i]!=-1;i++)
    {
      event.xkey.type    = KeyPress;
      event.xkey.display = dpy ;
      event.xkey.time    = CurrentTime;
      event.xkey.x       = event.xkey.y = 0;
      event.xkey.x_root  = event.xkey.y_root = 0;
      event.xkey.state   = masks[i];
      event.xkey.keycode = keycode;
      
      size = XLookupString ((XKeyEvent *) &event, string, 20, &keysym, 0);
      string [size] = 0;

      if (strcmp(str,string)==0)
	{
	  KeySym ks ;
	  char *nm ;
	  k = (i-1)*xd->map->max_keypermod ;
	  ks = XKeycodeToKeysym(dpy,
				xd->map->modifiermap[k],
				0);
	  nm = XKeysymToString(ks);
	  
	  
	  
	  kc->mod_keycodes[0] = xd->map->modifiermap[k];
	  ret = XNEE_OK ;
	  break;
       }
   }

  return ret;
}





KeyCode
xnee_char2keycode (xnee_data *xd, char token, xnee_key_code *kc)
{
  char buf[2];
  if (xd->fake==NULL)
    return -1;

  KeySym ks = 0 ;
  
  memset(kc,0,sizeof(xnee_key_code));
  buf[0]=token;
  buf[1]=0;

  switch( token)
    {
    case ' ':
      ks = XK_space ;
    case '$':
      break;
    case '%':
      break;
    case '&':
      ks = XK_ampersand ;
      break;
    case '-':
      ks = XK_minus ;
      break;
    case '\n':
      ks = XK_Return ;
    case '\0':
      break;
    case '(':
      break;
    case ')':
      break;
    case '/':
      ks = XK_slash;
      break;
    case '\\':
      ks = XK_backslash;
      break;
    case ',':
      break;
    case '.':
      break;
    case '#':
      break;
    case '"':
      break;
    case '?':
      break;
    case '@':
      break;
    case '!':
      break;
    default:
      /*    a-z, 0-9   */
      ks = 0 ;
      break;
    }

  if ( ks != 0)
    {
      xnee_keysym2keycode(xd, ks, buf, kc);
    }
  else
    {
      xnee_str2keycode(xd, buf, kc);
    }
    
  if ((token >= 'A') && (token <='Z'))
    /*     kc->shift_press=1; */
    ;

  return XNEE_OK;
}


static int
xnee_open_files(xnee_data *xd)
{
  char *file_name;

  xnee_verbose((xd, "---> xnee_open_files\n"));
  file_name = xnee_get_err_name(xd);
  if (file_name!=NULL)
    {
      xnee_verbose((xd, "---  xnee_open_files: handling err\n"));
      if (!xnee_check (file_name, "stderr", "STDERR"))
	{
	  xnee_verbose((xd, "---  xnee_open_files: opening err: %s\n", 
			xd->err_name));
	  XNEE_FCLOSE_IF_NOT_NULL(xd->err_file); 
	  xd->err_file = fopen (xd->err_name,"w");
	  if (xd->err_file==NULL)
	    return XNEE_FILE_NOT_FOUND;
	}
    }

  if (xnee_is_recorder(xd))
    {
      xnee_verbose((xd, "---  xnee_open_files: is recorder\n"));
      file_name = xnee_get_out_name(xd);
      if (file_name!=NULL)
	{
	  xnee_verbose((xd, "---  xnee_open_files: handling out\n"));
	  if (!xnee_check (file_name, "stdout", "STDOUT"))
	    {
	      xnee_verbose((xd, "---  xnee_open_files: opening out: %s\n", 
			    xd->out_name));
	      XNEE_FCLOSE_IF_NOT_NULL(xd->out_file); 
	      xd->out_file = fopen (xd->out_name,"w");
	      if (xd->out_file==NULL)
		return XNEE_FILE_NOT_FOUND;
	    }
	}
    }
  else if (xnee_is_replayer(xd))
    {
      xnee_verbose((xd, "---  xnee_open_files: is replayer\n"));
      file_name = xnee_get_data_name(xd);
      if (file_name!=NULL)
	{
	  xnee_verbose((xd, "---  xnee_open_files: handling data (in)\n"));
	  if (!xnee_check (file_name, "stdin", "STDIN"))
	    {
	      xnee_verbose((xd, "---  xnee_open_files: opening data: %s\n", 
			    xd->data_name));
	      XNEE_FCLOSE_IF_NOT_NULL(xd->data_file); 
	      xd->data_file = fopen (xd->data_name,"r");
	      if (xd->data_file==NULL)
		return XNEE_FILE_NOT_FOUND;
	    }
	}
    }
  else if (xnee_is_retyper(xd))
    {
      xnee_verbose((xd, "---  xnee_open_files: is retyper\n"));
      file_name = xnee_get_rt_name(xd);
      if (file_name!=NULL)
	{
	  xnee_verbose((xd, "---  xnee_open_files: handling retype (in)\n"));
	  if (!xnee_check (file_name, "stdin", "STDIN"))
	    {
	      xnee_verbose((xd, "---  xnee_open_files: opening retype: %s\n", 
			    xd->rt_name));
	      XNEE_FCLOSE_IF_NOT_NULL(xd->rt_file); 
	      xd->rt_file = fopen (xd->rt_name,"r");
	      if (xd->rt_file==NULL)
		return XNEE_FILE_NOT_FOUND;
	    }
	}
    }
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
  xnee_print_distr_list(xd, NULL);

  xnee_set_ranges(xd);

  ret=xnee_setup_recordext (xd);
  
  if ( xnee_is_recorder(xd) )
  {
    if (ret==XNEE_NO_PROT_CHOOSEN)
      {
	xnee_verbose((xd, "<-- xnee_rep_prepare returning %d\n", ret));
        return ret;
      }
  }

  if ( xnee_is_replayer(xd) )
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
  
  xnee_verbose((xd," building modifier map on %d\n", xd->fake)); 
  xd->map = XGetModifierMapping(xd->fake);

  
  /*
   * If no recording client, init xnee_sync 
   *
   */
  if ( ! xnee_is_recorder(xd) )
    {
      xnee_replay_init (xd);   
    }
  
  /*
   * Save repeat mode so we can reset it after we are done
   *
   */
  xnee_set_autorepeat (xd);
  
  xnee_verbose((xd, "<-- xnee_rep_prepare returning %d\n", XNEE_OK));
  return XNEE_OK;
}

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



int
xnee_start(xnee_data *xd)
{
   int ret ;

#ifdef XNEE_OBSOLETE
   ret = xnee_prepare(xd);
   if (ret!=XNEE_OK)
     {
       xnee_verbose((xd, "xnee_prepare failed (%d)....checking\n", ret));
       if ( xnee_is_recorder(xd) )
	 {
	   if (ret==XNEE_NO_PROT_CHOOSEN)
	     {
	       xnee_verbose((xd, "xnee_prepare failed.... failure %d\n", ret));
	       return ret;
	     }
	   xnee_verbose((xd, "xnee_prepare failed.... it was not OK\n"));
	   return ret;
	 }
      else
      {
         xnee_verbose((xd, "xnee_prepare failed.... failure nr %d\n", ret));
         return ret;
      }
   }
#endif
   /* grab all keys that have been specified */
   ret = xnee_grab_all_keys (xd);
   if (ret != XNEE_OK)
     {
       xnee_verbose((xd, "grab failure \n"));
       return ret;
     }
  

   if (xnee_get_interval (xd) != 0)
   {
     xnee_delay (xnee_get_interval (xd), "xnee:" );
   }
   
   /*
    * are we recording or are we replaying
    */
   if (xnee_is_recorder(xd)) 
   {
     
      /* 
       * Print settings 
       * if verbose mode that is 
       */
      xnee_print_xnee_settings       (xd, NULL); 
      xnee_record_print_record_range (xd, NULL);
      
      /*
       * Do we have XRecord extension on the display
       *
       */ 
       if (!xnee_has_record_extension(xd))
       {
          xnee_verbose((xd, "Can't find Record extension\n"));
          xnee_verbose((xd, "Look in the README file included"));
          xnee_verbose((xd, "in Xnee how to enable it\n"));
          exit(XNEE_NO_REC_EXT);
       }
       xnee_setup_recording(xd);
       
       xnee_print_sys_info(xd, xnee_get_out_file (xd));
       xnee_print_xnee_settings (xd, xnee_get_out_file (xd)) ;
       xnee_record_print_record_range (xd, xnee_get_out_file (xd)) ;
       

       xnee_zero_events_recorded(xd);
       xnee_zero_data_recorded(xd);
       xnee_zero_time_recorded(xd);


       /*
        * At last. Time to enter the main loop
        *
        */
       if (xnee_more_to_record(xd)!=0)
       {
	 xnee_verbose((xd, "Entering main loop( recorder)\n"));
	 ret = xnee_record_async(xd);
	 if (ret != XNEE_OK)
	   return ret;
       }
    }
  else if (xnee_is_replayer(xd))
  {
      xnee_verbose((xd, " (replayer)\n"));
      /*
       * Do we have XTest extension on the display
       *
       */ 
      if (!xnee_has_xtest_extension(xd))
	{
	  exit(XNEE_NO_TEST_EXT);
	}
      
      /*
       * Do we have XRecord extension on the display
       *
       */ 
      if (!xnee_has_record_extension(xd))
	{
	  xnee_verbose((xd, "I can't find Record extension\n"));
	  xnee_verbose((xd, "Look in the README file how to enable it\n"));
	  xnee_verbose((xd, "However, I continue without doing syncing\n"));
	  /*	  xd->sync=False;*/
	  xnee_unset_sync (xd);
	}

      XTestGrabControl (xnee_get_control_display(xd), True);
      XTestGrabControl (xnee_get_data_display(xd), True);

      /*
       * At last. Time to enter the main loop
       * ... wait to set up recording until all META data from file is read 
       * Thanks: Janice Waddick 
       */
      xnee_verbose((xd, "Entering main loop (replayer)\n"));
      ret = xnee_replay_main_loop(xd, XNEE_REPLAY_READ_REPLAY_DATA);
      if (ret != XNEE_OK)
	{
	  ;
	}
    }
  else if (xnee_is_retyper(xd))
    {
      if ( xnee_type_file(xd) != 0 )
	{
	  xnee_print_error ("Unable to open retype file\n");
	}
      xnee_verbose((xd,"finished replaying\n"));
    }
  else
    {
      return XNEE_MODE_NOT_SET;
    }
  /*
   * Close everything down .... free memory, tell X server we are leaving ...
  if ( xd->recorder || xd->sync )
    {
      xnee_record_close_down(xd);
    }
   */
   
   xnee_verbose((xd, "xnee_start : ungrab -----> \n"));
   xnee_ungrab_keys (xd);
   xnee_verbose((xd, "xnee_start : ungrab <---- \n"));
   xnee_reset_autorepeat (xd); 
   
   xnee_renew_xnee_data(xd);
   return (XNEE_OK);
}



int 
xnee_renew_xnee_data(xnee_data *xd)
{
   xnee_free_dyn_data(xd);
   xnee_new_dyn_data(xd);
   return (XNEE_OK);
}


int 
xnee_refresh_xnee_data(xnee_data *xd)
{
   xnee_free_dyn_data(xd);
   xnee_new_dyn_data(xd);
   return (XNEE_OK);
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

  return ( evs && dats && tims );
}

int 
xnee_check_true(char *expr)
{
  return  ( (strncmp(expr,XNEE_TRUE_STRING,strlen(XNEE_TRUE_STRING))==0)
	    ||
	    (strncmp(expr,XNEE_1_STRING,strlen(XNEE_1_STRING))==0) );
}

int 
xnee_check_false(char *expr)
{
  return  ( (strncmp(expr,XNEE_FALSE_STRING,strlen(XNEE_FALSE_STRING))==0)
	    ||
	    (strncmp(expr,XNEE_0_STRING,strlen(XNEE_0_STRING))==0) );
}

