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


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <dlfcn.h>

#include <sys/utsname.h>

#include <X11/Xlibint.h>
#include <X11/Xproto.h>
#include <X11/Xlib.h>

#include <X11/extensions/record.h> 
#include <X11/extensions/XTest.h> 

#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/xnee_dl.h"
#include "libxnee/xnee_sem.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/datastrings.h"



/* internal prototypes */
int
get_modifier_sub(xnee_data *xd,  char *mod_str);




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
      max_val=XNEE_MAX (xd->xnee_info->data_ranges[i], max_val);
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
  xd->data     = xnee_open_display (xd);
  xd->control  = xnee_open_display (xd);
  xd->fake     = xnee_open_display (xd);
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
  
  xnee_verbose((xd, "Closing file=%s fd=%d\n", file_name, file));
  if ( file_name != NULL) 
    {
      free (file_name);
    }
  if ( file != 0) 
    {
      fclose (file);
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
  

  xnee_free_file (xd, xd->out_name,  xd->out_file);
  xnee_free_file (xd, xd->err_name,  xd->err_file);
  xnee_free_file (xd, xd->data_name, xd->data_file);
  xnee_free_file (xd, xd->rc_name,   xd->rc_file);

  XNEE_DEBUG ( (stderr ," --> xnee_close_down() at 0.6 \n"  ));
  xnee_verbose((xd, "Freeeing data "));
  xnee_free_xnee_data(xd);
}



/**************************************************************
 *                                                            *
 * xnee_version                                               *
 *                                                            *
 *                                                            *
 **************************************************************/
void
xnee_version(xnee_data* xd)
{
  fprintf (stderr, "%s ",PACKAGE);
  fprintf (stderr, "%s\n",VERSION);
  fprintf (stderr, "Copyright (C) 2000-2003 Henrik Sandklef (%s)\n", XNEE_MAIL);
  fprintf (stderr, "%s and all its included programs come with ", PACKAGE);
  fprintf (stderr, "NO WARRANTY,\nto the extent permitted by law.\n" );
  fprintf (stderr, "This is free software, and you may redistribute\n");
  fprintf (stderr, "copies of it under terms of GNU Genreal Public License.\n");
  fprintf (stderr, "Xnee: Xnee's not an Event Emulator %s \n", XNEE_HOME_URL);
  fprintf (stderr, "GPL:  Gnu General Public License   %s\n",  GNU_HOME_URL);
  fprintf (stderr, "FSF:  Free Software Foundation     %s\n",  FSF_HOME_URL);
}


/**************************************************************
 *                                                            *
 * xnee_init                                                  *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_init(xnee_data* xd, char *name)
{
  if (xd == NULL) 
    {
      return XNEE_MEMORY_FAULT;
    }
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
  xd->mode          = XNEE_RECORDER   ; 

  xd->data_file     = stdin  ;
  xd->out_file      = stdout ;
  xd->err_file      = stderr ;
  xd->rc_file       = NULL   ;
  xd->buffer_file   = stderr ;

  xd->data_name     = NULL  ;
  xd->out_name      = NULL  ;
  xd->err_name      = NULL  ;
  xd->rc_name       = NULL  ;

  xd->display       = getenv ("DISPLAY");
  xd->distr_list    = NULL   ;
  xd->distr_list_size = 0   ;
  xd->cont          = True ;
  xd->force_replay  = False;

  xd->buf_sem = (sem_t *) malloc (sizeof(sem_t));
  xnee_sem_init (xd, xd->buf_sem, 0, 1);


  xd->button_pressed=0;
  /*  Not done until needed
      xnee_init_names();
  */

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
  return XNEE_OK;
}


int
get_modifier_from_mapping_sub(Display *display, char *mod_str) 
{
  XModifierKeymap *mod_keymap;
  KeyCode *keycode_ptr;
  KeySym my_keysym;
  int i, j, max_keys;
  int ret=0;
  static char *mod_list[]={"ShiftMask", "LockMask","ControlMask", 
		  "Mod1Mask", "Mod2Mask", "Mod3Mask", "Mod4Mask", "Mod5Mask" };

  mod_keymap = XGetModifierMapping(display);
  max_keys = mod_keymap->max_keypermod;
  keycode_ptr = mod_keymap->modifiermap;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < max_keys; j++) {
      my_keysym = XKeycodeToKeysym(display, *keycode_ptr, 0);

      if (XKeysymToString(my_keysym)!=NULL)
	{
	  if (xnee_check (mod_str, XKeysymToString(my_keysym),XKeysymToString(my_keysym)))
	    {
	      ret |= get_modifier_sub(NULL, mod_list[i]);
	      return ret;
	    }
	  else
	    {
	      ;
	    }
	}
      else
	{
	  ;
	}
      keycode_ptr++;
    }
  }
  return ret;
}

int
get_modifier_from_mapping(Display *display, char *mod_str) 
{
  /*
    XModifierKeymap *mod_keymap;
    KeyCode *keycode_ptr;
    KeySym my_keysym;
    int i, j, max_keys;
  */
  int ret;
    
  if (xnee_check(mod_str, "Alt", "Alt"))
    {
      ret = get_modifier_from_mapping_sub(display, "Alt_L");
      ret |= get_modifier_from_mapping_sub(display, "Alt_R");
    }
  else if (xnee_check(mod_str, "Shift", "Shift"))
    {
      ret = get_modifier_from_mapping_sub(display, "Shift_L");
      ret |= get_modifier_from_mapping_sub(display, "Shift_R");
    }
  else if (xnee_check(mod_str, "Control", "Control"))
    {
      ret = get_modifier_from_mapping_sub(display, "Control_L");
      ret |= get_modifier_from_mapping_sub(display, "Control_R");
    }
  else if (xnee_check(mod_str, "Scroll_Lock", "Scroll"))
    {
      ret = get_modifier_from_mapping_sub(display, "Scroll_Lock");
    }
  else if (xnee_check(mod_str, "Caps_Lock", "Caps"))
    {
      ret = get_modifier_from_mapping_sub(display, "Caps_Lock");
    }
  else
    {
      ret = get_modifier_from_mapping_sub(display, mod_str);
    }
  return ret;
}


int
get_modifier_sub(xnee_data *xd,  char *mod_str) 
{
  int ret=0;

  if( xnee_check (mod_str, "ShiftMask", "ShiftMask"  ) )
    {
      ret = ShiftMask;
    }
  else if( xnee_check (mod_str, "LockMask", "LockMask"  ) )
    {
      ret = LockMask;
    }
  else if( xnee_check (mod_str, "ControlMask", "ControlMask"  ) )
    {
      ret = ControlMask;
    }
  else if( xnee_check (mod_str, "Mod1Mask", "Mod1Mask"  ) )
    {
      ret = Mod1Mask;
    }
  else if( xnee_check (mod_str, "Mod2Mask", "Mod2Mask"  ) )
    {
      ret = Mod2Mask;
    }
  else if( xnee_check (mod_str, "Mod3Mask", "Mod3Mask"  ) )
    {
      ret = Mod3Mask;
    }
  else if( xnee_check (mod_str, "Mod4Mask", "Mod4Mask"  ) )
    {
      ret = Mod4Mask;
    }
  else if( xnee_check (mod_str, "Mod5Mask", "Mod5Mask"  ) )
    {
      ret = Mod5Mask;
    }
  else if( xnee_check (mod_str, "AnyModifier", "any"  ) )
    {
      ret = AnyModifier;
    }
  else if( xnee_check (mod_str, "none", "0"  ) )
    {
      ret = 0;
    }
  else 
    {
      /* prevent recursive disaster ... */
      if (xd==NULL)
	{
	  ret = 0;
	}
      else
	{
	  ret = get_modifier_from_mapping( xd->grab, mod_str);
	}
    }
  return ret;
}

int 
is_last (xnee_data *xd,  char *mod_strs) 
{
  char *tmp;
  int i ;
  int len = strlen (mod_strs);
  tmp=mod_strs;

  for (i=0;  (i<len) ; i++)
    {
      if (tmp[i]=='+')
	{
	  return XNEE_OK;
	}
    }
  return 1;
}

int
xkm_rem_blanks (char *array)
{
  int i=0;
  int j=0;
  int idx=-1;
  int size=strlen(array);

  for (i=0; i<size;i++)
    {
      idx++;
      if ( (array[idx]==' ') ||  (array[idx]=='\t') ||  (array[idx]=='\n') )
	{
	  for (j=idx;j<size-1;j++)
	    {
	      array[j]=array[j+1];
	    }
	  array[j]='\0';
	  idx--;
	}
    }
  return XNEE_OK;
}

int
get_modifier(xnee_data *xd,  char *mod_strs) 
{
  int ret=0;
  /*  int try_ret;*/
  char *tmp;
  char mod_head[20]="";
  char mod_tail[40]="";
  int i;
  /*  int last=0;*/
  int len=strlen(mod_strs);
  int found=0;
  static int _level=0;
  int level=_level;
  xkm_rem_blanks(mod_strs);

  _level++;
  
  xnee_verbose((xd, "%d ---> get_modifier:\"%s\"\n", level, mod_strs));
  tmp=mod_strs;
  
  if (!is_last(xd, tmp)==1)
    {
      xnee_verbose((xd, "%d --- get_modifier:\n", level));
      for (i=0;  (i<len) ; i++)
	{
	  xnee_verbose((xd, "%d --- get_modifier 2:\n", level));
	  if (tmp[i]=='+')
	    {
	      
	      xnee_verbose((xd, "%d --- get_modifier 3:\n", level));
	      found=1;
	      strncpy (mod_head, tmp, i); 
	      tmp+=(sizeof(char)*i+1);
	      
	      strcpy (mod_tail, tmp); 
	      ret |= get_modifier_sub (xd, mod_head);
	      ret |= get_modifier (xd, mod_tail);
	      xnee_verbose((xd, "%d ---- get_modifier FOUND \"%s\"   ret=%d\n", level, tmp, ret));
	      break;
	    }
	}
    }
  else
    {
      ret |= get_modifier_sub (xd, tmp);
      xnee_verbose((xd, "%d ---- get_modifier LAST :\"%s\"  ret=%d\n", level, tmp, ret));
      
    }
 /*	  XKM_TAB(level);
	  xnee_verbose((xd, "\t %d head:\"%s\"   tail:\"%s\"\n", level, mod_head, mod_tail));
  */
  
  xnee_verbose((xd, "%d <--- get_modifier:\"%s\" --- ret=%d\n", level, mod_strs, ret));
  return ret;
}



/**************************************************************
 *                                                            *
 * get_modifier                                               *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xget_modifier(xnee_data *xd, char *mod_str) 
{
  int ret=-1;

  if( xnee_check (mod_str, "Shift", "Shift"  ) )
    {
      ret = ShiftMask;
    }
  else if( xnee_check (mod_str, "LockMask", "Lock"  ) )
    {
      ret = LockMask;
    }
  else if( xnee_check (mod_str, "Control", "ctrl"  ) )
    {
      ret = ControlMask;
    }
  else if( xnee_check (mod_str, "Mod1Mask", "m1m"  ) )
    {
      ret = Mod1Mask;
    }
  else if( xnee_check (mod_str, "Mod2Mask", "m2m"  ) )
    {
      ret = Mod2Mask;
    }
  else if( xnee_check (mod_str, "Mod3Mask", "m3m"  ) )
    {
      ret = Mod3Mask;
    }
  else if( xnee_check (mod_str, "Mod4Mask", "m4m"  ) )
    {
      ret = Mod4Mask;
    }
  else if( xnee_check (mod_str, "Mod5Mask", "m5m"  ) )
    {
      ret = Mod5Mask;
    }
  else if( xnee_check (mod_str, "AnyModifier", "any"  ) )
    {
      ret = AnyModifier;
    }
  else if( xnee_check (mod_str, "none", "0"  ) )
    {
      ret = 0;
    }
  else 
    {
      ret = -1;
    }
  xnee_verbose((xd, "get_modifier\tmodifier=%d (%s)\n", ret, mod_str));
  return ret;
}



/**************************************************************
 *                                                            *
 * xnee_grab_key                                              *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_grab_key (xnee_data* xd, int mode, char *mod_key)
{
  int window;
  int screen;
  xnee_km_tuple km;

  xnee_verbose((xd, "----> xnee_grab_key\n"));

  xnee_verbose((xd, "----  xnee_grab_key mod_key=%s\n", mod_key));
  xnee_get_km_tuple (xd, &km, mod_key);
  xnee_verbose((xd, "----  xnee_grab_key mod=%d\n", km.modifier));
  xnee_verbose((xd, "----  xnee_grab_key key=%d\n", km.key));


  /* get the key+modifier from xd
   * corresponding to the mode given */
  switch (mode)
    {
    case XNEE_GRAB_STOP:
      xd->grab_keys->stop_key=km.key;
      xd->grab_keys->stop_mod=km.modifier;
      xnee_verbose((xd, "----  xnee_grab_key STOP mode\n"));
      break;
    case XNEE_GRAB_PAUSE:
      xd->grab_keys->pause_key=km.key;
      xd->grab_keys->pause_mod=km.modifier;
      xnee_verbose((xd, "----  xnee_grab_key PAUSE mode\n"));
      break;
    case XNEE_GRAB_RESUME:
      xd->grab_keys->resume_key=km.key;
      xd->grab_keys->resume_mod=km.modifier;
      xnee_verbose((xd, "----  xnee_grab_key RESUME mode\n"));
      break;
    default:
      xnee_print_error ("Unknown grab mode\n");
      return XNEE_UNKNOWN_GRAB_MODE;
    }

  /* make sure we have a display to grab on*/
  if (xd->grab==NULL)
    {
      xd->grab = XOpenDisplay (NULL);
      if (xd->grab==NULL)
	{
	  xnee_verbose((xd, "could not open display for grab...\n"));
	  return XNEE_NOT_OPEN_DISPLAY;
	}
    }
  
  /* grab key + modifier */
  screen = DefaultScreen (xd->grab);
  window = RootWindow    (xd->grab, screen );
  xnee_verbose((xd, "window   %d\n", window));
  xnee_verbose((xd, "screen   %d\n", screen));
  xnee_verbose((xd, "data     %d\n", xd->grab));
  xnee_verbose((xd, "stop key %d\n", km.key));
  xnee_verbose((xd, "stop mod %d\n", km.modifier));
  XGrabKey (xd->grab,  
	    km.key,            
	    km.modifier,
	    window,       
	    False,  
	    GrabModeSync,
	    GrabModeSync );
  xnee_verbose((xd, "<---- xnee_grab_stop_key\n"));
  return XNEE_OK;
}



int
xnee_get_grab_mode (xnee_data *xd, int key, int modifier)
{
  


  if ( (key==xd->grab_keys->stop_key) && 
       (modifier==xd->grab_keys->stop_mod) )
    {
      xnee_verbose ((xd, "xnee_get_grab_mode: STOP \n"));
      return XNEE_GRAB_STOP;
    }
  else if ( (key==xd->grab_keys->pause_key) && 
	    (modifier==xd->grab_keys->pause_mod) )
    {
      xnee_verbose ((xd, "xnee_get_grab_mode: PAUSE \n"));
      return XNEE_GRAB_PAUSE;
    }
  else if ( (key==xd->grab_keys->resume_key) && 
	    (modifier==xd->grab_keys->resume_mod) )
    {
      xnee_verbose ((xd, "xnee_get_grab_mode: RESUME \n"));
      return XNEE_GRAB_PAUSE;
    }
  else 
   {
 return XNEE_GRAB_UNKOWN;
   }
}


int
xnee_check_km(xnee_data *xd)
{
  XEvent ev;
  if ( (xd->grab!=NULL)
       &&
       ( xd->grab_keys->grab))
    {
      


      /*
       * Has the user pressed STOP (mod + key) 
       */
      XFlush (xd->grab);
      XAllowEvents (xd->grab,
		    AsyncKeyboard,
		    CurrentTime);
      XFlush (xd->grab);
      

      if ( !XCheckMaskEvent ( xd->grab, 0xffffffff , &ev) == False)
	{
	  XEvent my_event ;
	  int    tmp_code;
	  int    tmp_modifier;
	  int    mode;


	  XNextEvent (xd->grab, &my_event);
	  if (my_event.xkey.send_event==1) 
	    { 
	      xnee_verbose ((xd , "send_event==true\n")); 
	    }
	  tmp_modifier=my_event.xkey.state;
	  tmp_code=my_event.xkey.keycode;
	  xnee_verbose ((xd, "key      = %d\n", tmp_modifier));
	  xnee_verbose ((xd, "modifier = %d\n", tmp_code));
	  mode=xnee_get_grab_mode (xd, tmp_code, tmp_modifier);
	  xnee_verbose ((xd, "mode     = %d\n", mode));
	  
	  
	  xnee_verbose((xd, "\n\n\tUSER PUSHED MOD + KEY ... leaving\n\n\n"));
	  /* Since we've got called we have data .. we don't care about it. 
	     Only freeeing the mem */
	  
	  return XNEE_GRAB_DATA;
	}
      else
	{
	  return XNEE_NO_GRAB_DATA;
	}
      
    }
  else
    {
      return XNEE_NO_GRAB_DATA;
    }
  return XNEE_NO_GRAB_DATA;
}



int 
xnee_ungrab_key (xnee_data* xd, int mode)
{
  int key      = 0 ;
  int modifier = 0 ;
  int window;
  int screen;
    
  switch (mode)
    {
    case XNEE_GRAB_STOP:
      key      = xd->grab_keys->stop_key;
      modifier = xd->grab_keys->stop_mod;
      xd->grab_keys->stop_key=0;
      xd->grab_keys->stop_mod=0;
      xnee_verbose((xd, "----  xnee_grab_key STOP mode\n"));
      break;
    case XNEE_GRAB_PAUSE:
      key      = xd->grab_keys->pause_key;
      modifier = xd->grab_keys->pause_mod;
      xd->grab_keys->pause_key=0;
      xd->grab_keys->pause_mod=0;
      xnee_verbose((xd, "----  xnee_grab_key PAUSE mode\n"));
      break;
    case XNEE_GRAB_RESUME:
      key      = xd->grab_keys->resume_key;
      modifier = xd->grab_keys->resume_mod;
      xd->grab_keys->resume_key=0;
      xd->grab_keys->resume_mod=0;
      xnee_verbose((xd, "----  xnee_grab_key RESUME mode\n"));
      break;
    default:
      xnee_print_error ("Unknown grab mode\n");
      return XNEE_UNKNOWN_GRAB_MODE;
    }

  if ( xd->grab != 0 )
    {
      screen = DefaultScreen (xd->grab);
      xnee_verbose((xd, "---  xnee_ungrab_stop_key\n"));
      window = RootWindow(xd->grab, screen );
      xnee_verbose((xd, "---  xnee_ungrab_stop_key\n"));
      xnee_verbose((xd, "window   %d\n", window));
      xnee_verbose((xd, "screen   %d\n", screen));
      xnee_verbose((xd, "data     %d\n", xd->grab));
      xnee_verbose((xd, "stop key %d\n", key));
      xnee_verbose((xd, "stop mod %d\n", modifier));
      XUngrabKey (xd->grab,  
		  key,            
		  modifier,
		  window);
      xd->grab_keys->grab=1;
    }
}

/**************************************************************
 *                                                            *
 * xnee_ungrab_keys                                           *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_ungrab_keys (xnee_data* xd)
{
  xnee_verbose((xd, "---> xnee_ungrab_stop_key\n"));
  xnee_ungrab_key ( xd, XNEE_GRAB_STOP);
  xnee_ungrab_key ( xd, XNEE_GRAB_PAUSE);
  xnee_ungrab_key ( xd, XNEE_GRAB_RESUME);
  
  xnee_verbose((xd, "<--- xnee_ungrab_stop_key\n"));
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
  xnee_verbose((xd, " ---> xnee_stop_session\n" ));
  xnee_ungrab_keys(xd);
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
    case SIGINT:
      fprintf  (stderr,  "sighandler SIGINT (%d)\n", sig);
      exit (sig);
    default:
      fprintf (stderr, "signal_handler error .... unxpected signal (%d)\n .... leaving", sig);
      exit (sig);
    }
}




/**************************************************************
 *                                                            *
 * xnee_new_grab_keys                                         *
 *                                                            *
 *                                                            *
 **************************************************************/
xnee_grab_keys  *
xnee_new_grab_keys()
{

  xnee_grab_keys* xgk = 
    (xnee_grab_keys*) malloc (sizeof (xnee_grab_keys));
  /*  memset (xgk, 0, sizeof (xnee_grab_keys)); */

  if (xgk==NULL)
    return NULL;
  xgk->stop_key     = 0 ;
  xgk->stop_mod     = 0 ;

  xgk->pause_key    = 0 ;
  xgk->pause_mod    = 0 ;

  xgk->resume_key   = 0 ;
  xgk->resume_mod   = 0 ;

  xgk->grab = 0;

  return xgk;
}


/**************************************************************
 *                                                            *
 * xnee_free_grab_keys                                        *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_free_grab_keys(xnee_data *xd)
{
  free (xd->grab_keys);
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
  if (max>0)
    {
      for ( i=0 ; i<max ; i++ )
	{
	  XNEE_DEBUG ( (stderr, "   freeing range at %d since max=%d\n", i, max));
	  free (xd->record_setup->range_array[i]);
	}
    }
  XRecordDisableContext (xd->control, xd->record_setup->rContext) ; 
  XRecordFreeContext (xd->control, xd->record_setup->rContext) ; 

  free (xd->record_setup->xids);
  free (xd->record_setup->rState);
  free (xd->record_setup);
  XNEE_DEBUG ( (stderr ," <--xnee_free_xnee_data()\n"  ));
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

  xd->xnee_info     = (xnee_record_init_data*)  malloc (sizeof (xnee_record_init_data)) ;
  memset (xd->xnee_info, 0, sizeof(xnee_record_init_data));

  xd->replay_setup  = (xnee_testext_setup*)     malloc (sizeof (xnee_testext_setup)) ;
  memset (xd->replay_setup, 0, sizeof(xnee_testext_setup));

  xd->record_setup  = xnee_new_recordext_setup(); 

  xd->grab_keys     = xnee_new_grab_keys();
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

  xnee_free_recordext_setup ( xd);
  xnee_free_grab_keys(xd);
  free (xd->xnee_info);
  free (xd->replay_setup );
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
      xnee_verbose((xd, "Adding Display - allocating memory for the first time\n"));
      xd->distr_list = (Display **) calloc (1,sizeof (Display));
    }
  else 
    {
      xnee_verbose((xd, "Adding Display - allocating memory for the %d time\n", xd->distr_list_size));
      xd->distr_list = (Display **) realloc (xd->distr_list, 
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
  
  xd->distr_list[xd->distr_list_size]=dpy;
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
      xd->distr_list = (Display **) calloc (1,sizeof (Display));
    }
  else 
    {
      xd->distr_list = (Display **) realloc (xd->distr_list, 
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
  
  xd->distr_list[xd->distr_list_size]=dpy;
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
  return 1;
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
  

  xd->plugin_handle = (void*) xnee_dlopen (xd, lib_name, RTLD_LAZY );
  if (!xd->plugin_handle) {
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
 * xnee_add_resource_syntax                                   *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_add_resource_syntax(xnee_data *xd, char *tmp)
{
  int ret=1;  
  char *range;
  int len=0;
  
  len=strlen(tmp);


  if ( tmp == NULL)
    {
      return -1;
    }
  rem_all_blanks (tmp, len);
  len=strlen(tmp);
  if (len==0)
    {
      return -1;
    }
  xnee_verbose((xd, "   adding (2) \"%s\"  \n", tmp));
  
  if (!strncmp("#",tmp,1))  /* # META data */
    {
      ; /* for now ..... do nothing */
    }
  else if (!strncmp(XNEE_DISPLAY,tmp,strlen(XNEE_DISPLAY)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_set_display_name (xd, range) ;
    }
  else if (!strncmp(XNEE_FIRST_LAST,tmp,strlen(XNEE_FIRST_LAST)))
    {
      xd->xnee_info->first_last = True;
    }
  else if (!strncmp(XNEE_NO_SYNC,tmp,strlen(XNEE_NO_SYNC)))
    {
      xd->sync = False;
    }
  else if (!strncmp(XNEE_FORCE_REPLAY,tmp,strlen(XNEE_FORCE_REPLAY)))
    {
      xd->force_replay = True;
    }
  else if (!strncmp(XNEE_ALL_CLIENTS,tmp,strlen(XNEE_ALL_CLIENTS)))
    {
      xd->all_clients = True;
    }
  else if (!strncmp(XNEE_VERBOSE,tmp,strlen(XNEE_VERBOSE)))  
    {
      xd->verbose = True;
    }
  else if (!strncmp(XNEE_BUFFER_VERBOSE,tmp,strlen(XNEE_BUFFER_VERBOSE)))  
    {
      xd->buf_verbose = True;
    }
  else if (!strncmp(XNEE_LOOPS,tmp,strlen(XNEE_LOOPS))) 
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xd->xnee_info->loops_left = atoi(range);
    }
  else if (!strncmp(XNEE_STOP_KEY,tmp,strlen(XNEE_STOP_KEY)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_verbose((xd, "Stop key :\"%s\"\n", range ));
      xnee_grab_key (xd, XNEE_GRAB_STOP, range);
      xnee_verbose((xd, "Stop key :\"%s\"\n", range ));
    }
  else if (!strncmp(XNEE_PAUSE_KEY,tmp,strlen(XNEE_PAUSE_KEY)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_verbose((xd, "Pause key :\"%s\"\n", range ));
      xnee_grab_key (xd, XNEE_GRAB_PAUSE, range);
      xnee_verbose((xd, "Pause key :\"%s\"\n", range ));
    }
  else if (!strncmp(XNEE_RESUME_KEY,tmp,strlen(XNEE_RESUME_KEY)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_verbose((xd, "Resume key :\"%s\"\n", range ));
      xnee_grab_key (xd, XNEE_GRAB_RESUME, range);
      xnee_verbose((xd, "Resume key :\"%s\"\n", range ));
    }
  else if (!strncmp(XNEE_KEYBOARD,tmp,strlen(XNEE_KEYBOARD))) 
    {	 
      xnee_parse_range (xd, XNEE_DEVICE_EVENT, 
			"KeyPress-KeyRelease");
      
    }
  else if (!strncmp(XNEE_MOUSE,tmp,strlen(XNEE_MOUSE)))
    {
      xnee_parse_range (xd, XNEE_DEVICE_EVENT, 
			"ButtonPress-MotionNotify");
    }
  else if (!strncmp(XNEE_OUT_FILE,tmp,3)) 
    {
      xnee_verbose((xd, "file \n" ));
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_verbose((xd, "file \"%s\"\n", range));
      xnee_set_out_byname (xd, range);
    }
  else if (!strncmp(XNEE_ERR_FILE,tmp,8))  
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_set_err_byname (xd, range);
    }
  else if (!strncmp(XNEE_DISTRIBUTE,tmp,strlen(XNEE_DISTRIBUTE)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_add_display_list ( xd, range);
    }
  else if (!strncmp(XNEE_DEVICE_EVENT_STR,tmp,strlen(XNEE_DEVICE_EVENT_STR)))
    {
      range=strstr (tmp, ":");
      range ++ ;
      xnee_parse_range ( xd, XNEE_DEVICE_EVENT, range);
    }
  else if (!strncmp(XNEE_DELIVERED_EVENT_STR,
		    tmp,strlen(XNEE_DELIVERED_EVENT_STR)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_DELIVERED_EVENT, range);
    }
  else if (!strncmp(XNEE_ERROR_STR,tmp,strlen(XNEE_ERROR_STR))) 
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_ERROR, range);
    }
  else if (!strncmp(XNEE_REQUEST_STR,tmp,strlen(XNEE_REQUEST_STR)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_REQUEST, range);
    }
  else if (!strncmp(XNEE_REPLY_STR,tmp,strlen(XNEE_REPLY_STR)))  
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_REPLY, range);
    }
  else if (!strncmp(XNEE_EXT_REQ_MAJ_STR,tmp,strlen(XNEE_EXT_REQ_MAJ_STR)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_EXT_REQUEST_MAJOR, range);
    }
  else if (!strncmp(XNEE_EXT_REQ_MIN_STR,tmp,strlen(XNEE_EXT_REQ_MIN_STR)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_EXT_REQUEST_MINOR, range);
    }
  else if (!strncmp(XNEE_EXT_REP_MAJ_STR,tmp,strlen(XNEE_EXT_REP_MAJ_STR))) 
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_EXT_REPLY_MAJOR, range);
    }
  else if (!strncmp(XNEE_EXT_REP_MIN_STR,tmp,strlen(XNEE_EXT_REP_MIN_STR))) 
    {
      range=strstr (tmp, ":");
      range ++ ;
      xnee_parse_range ( xd, XNEE_EXT_REPLY_MINOR, range);
    }
  /* 
   * TO KEEP COMPATIBILITY WITH OLD 
   * - SCRIPTS USING XNEE
   * - XNEE PLUGINS
   * THE OLD SYNTAX IS ALLOWED 
   * ... UNTIL ???  
   */
  /* START OF OBSOLETE PARSING */
  else if (!strncmp(XNEE_OBSOLETE_FIRST_LAST,tmp,strlen(XNEE_OBSOLETE_FIRST_LAST)))
    {
      xd->xnee_info->first_last = True;
    }
  else if (!strncmp(XNEE_OBSOLETE_NO_SYNC,tmp,strlen(XNEE_OBSOLETE_NO_SYNC)))  
    {
      xd->sync = False;
    }
  else if (!strncmp(XNEE_OBSOLETE_ALL_CLIENTS,tmp,strlen(XNEE_OBSOLETE_ALL_CLIENTS)))
    {
      xd->all_clients = True;
    }
  else if (!strncmp(XNEE_OBSOLETE_STOP_KEY,tmp,strlen(XNEE_OBSOLETE_STOP_KEY)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_verbose((xd, "==================== Stop key :\"%s\"\n", range ));
      xnee_grab_key (xd, XNEE_GRAB_STOP, range);
      xnee_verbose((xd, "==================== Stop key :\"%s\"\n", range ));
    }
  else if (!strncmp(XNEE_OBSOLETE_ERROR_STR,tmp,strlen(XNEE_OBSOLETE_ERROR_STR)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      if ( xnee_use_plugin(xd, range) != 0 )
	{
	xnee_print_error ("Unable to open plugin file (%s)\n", range);
	}
    }
  else if (!strncmp(XNEE_OBSOLETE_DEVICE_EVENT_STR,tmp,
		    strlen(XNEE_OBSOLETE_DEVICE_EVENT_STR)))
    {
      range=strstr (tmp, ":");
      range ++ ;
      xnee_parse_range ( xd, XNEE_DEVICE_EVENT, range);
    }
  else if (!strncmp(XNEE_OBSOLETE_DELIVERED_EVENT_STR,tmp,
		    strlen(XNEE_OBSOLETE_DELIVERED_EVENT_STR))) 
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_DELIVERED_EVENT, range);
    }
  else if (!strncmp(XNEE_OBSOLETE_ERROR_STR,tmp,strlen(XNEE_OBSOLETE_ERROR_STR)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_ERROR, range);
    }
  else if (!strncmp(XNEE_OBSOLETE_REQUEST_STR,tmp,strlen(XNEE_OBSOLETE_REQUEST_STR)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_REQUEST, range);
    }
  else if (!strncmp(XNEE_OBSOLETE_REPLY_STR,tmp,strlen(XNEE_OBSOLETE_REPLY_STR))) 
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_REPLY, range);
    }
  else if (!strncmp(XNEE_OBSOLETE_EXT_REQ_MAJ_STR,tmp,
		    strlen(XNEE_OBSOLETE_EXT_REQ_MAJ_STR)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_EXT_REQUEST_MAJOR, range);
    }
  else if (!strncmp(XNEE_OBSOLETE_EXT_REQ_MIN_STR,tmp,
		    strlen(XNEE_OBSOLETE_EXT_REQ_MIN_STR)))
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_EXT_REQUEST_MINOR, range);
    }
  else if (!strncmp(XNEE_OBSOLETE_EXT_REP_MAJ_STR,tmp,
		    strlen(XNEE_OBSOLETE_EXT_REP_MAJ_STR))) 
    {
      range=strstr (tmp, ":");
      range += 1 ;
      xnee_parse_range ( xd, XNEE_EXT_REPLY_MAJOR, range);
    }
  else if (!strncmp(XNEE_OBSOLETE_EXT_REP_MIN_STR,tmp,
		    strlen(XNEE_OBSOLETE_EXT_REP_MIN_STR))) 
    {
      range=strstr (tmp, ":");
      range ++ ;
      xnee_parse_range ( xd, XNEE_EXT_REPLY_MINOR, range);
    }
  /* END OF OBSOLETE PARSING */
  else 
    {
      xnee_verbose((xd,"Corrupt line: \"%s\"\n", tmp)); 
      ret=0;
    }      
  return ret;
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
 * rem_blanks                                                 *
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
 * xnee_get_km_tuple                                          *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_get_km_tuple (xnee_data     *xd, 
		   xnee_km_tuple *km, 
		   char          *mod_and_key)
{
  char key_buf[20];
  char mod_buf[20];
  int i=0;
  int idx=-1;
  int len=strlen(mod_and_key);
  xnee_verbose((xd, " ---> xnee_get_km_tuple_key %s \n", mod_and_key));
  strcpy (mod_buf, mod_and_key);
  rem_all_blanks (mod_buf, strlen(mod_and_key));

  /* make sure we have a display to grab on */
  if ( xd->grab == NULL)
    {
      xd->grab = XOpenDisplay (xd->display);
      if ( xd->grab == NULL)
	{
	  xnee_print_error ("Could not open display for grabbing\n");
	  return (XNEE_NOT_OPEN_DISPLAY);
	}
    }
  

  for (i=0;i<len;i++)
    {
      if (mod_and_key[i] == '\0')
	;
      else if (mod_and_key[i] == ',')
	idx=0;
      if (idx == 0 )
	{
	  mod_buf[i]='\0';
	  idx++;
	}
      else if (idx < 0 )
	{
	  mod_buf[i]=mod_and_key[i];
	}
      else
	{
	  key_buf[idx++ - 1]=mod_and_key[i];
	  key_buf[idx-1]='\0';
	}
    }

  xnee_verbose((xd, " ---  xnee_get_km_tuple_key mod=\"%s\"   key=\"%s\"\n", mod_buf, key_buf));
  /* Is it a modifier in alias form (e.g Control, Shift....)
     or is it a number */
  i=get_modifier(xd, mod_buf);
  if ( i != -1 )
    {
      km->modifier=i;
    }
  else if ( sscanf ( mod_buf, "%d", km->modifier) < 0 )
    {
      xnee_print_error ("SYNTAX ERROR\n\tCouldn't convert "
			" string \"%s\" to an int\n",mod_buf);
      return ( XNEE_SYNTAX_ERROR);
    }

  /* Is it a Key in letter form (e.g a,b)
     or is it a number */
  if ( sscanf ( key_buf, "%d", &km->key) <= 0 )
    {
      if (strlen(key_buf)!=1)
	{
	  xnee_print_error ("SYNTAX ERROR\n\tCouldn't convert "
			    "string \"%s\" to an int.\n",key_buf);
	  return ( XNEE_SYNTAX_ERROR);
	}
      else
	{
	  /* is it a letter */
	  km->key = XStringToKeysym (key_buf)  ; 
	  if ( km->key == 0 )
	    {
	      xnee_print_error ("SYNTAX ERROR\n\tCouldn't convert "
				"string \"%s\" to an int\n",key_buf);
	      return ( XNEE_SYNTAX_ERROR);
	    }
	  /* OK, it's a integer... just use it! */
	  km->key = XKeysymToKeycode (xd->grab, km->key);
	}
    } 
  xnee_verbose((xd, " <--- xnee_get_km_tuple_key %s \n", mod_and_key));
  return XNEE_OK;
}




/**************************************************************
 *                                                            *
 * xnee_add_resource                                            *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_add_resource(xnee_data *xd)
{

  char tmp[256] ;
  int read_more  = 1 ;
  
  strcpy(tmp,"");


  while (read_more!=0)
    {
      if ( fgets(tmp, 256, xd->rc_file) == NULL)
	return XNEE_OK;
      /*
       * Hey, I know we'll keep the char array....
       * as long as we need... next call aint deep 
       */
      xnee_verbose((xd,"  adding : \"%s\"\n", tmp));
      read_more=xnee_add_resource_syntax(xd, tmp);
    }
  return read_more;
}






/**************************************************************
 *                                                            *
 * xnee_get_record_config                                     *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_get_record_config (xnee_data* xd, xnee_intercept_data * xindata) 
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




/**************************************************************
 *                                                            *
 * xnee_get_elapsed_time                                      *
 *                                                            *
 *                                                            *
 **************************************************************/
long 
xnee_get_elapsed_time(xnee_data *xd, char type )
{
  static  long last_read_msecec = 0;  /* from last call of this f'n */
  static  long last_read_sec = 0;
  static  long first_read_msecec = 0; /* from 1st time this f'n called */
  static  long first_read_sec = 0;
  long    last_sec; 
  long    last_msec; 
  long    diff_sec; 
  long    diff_msec; 
  long    time_offset_msec  = 0;

  struct timeval  cur_time;
  struct timezone zoneData;


  /* determine what value to get difference from */
  if ( type == XNEE_FROM_LAST_READ )
    {
      last_sec = last_read_sec;
      last_msec = last_read_msecec;
    }
  else
    {
      last_sec = first_read_sec;
      last_msec = first_read_msecec;
    }

  /* get current time */
  if(  gettimeofday( &cur_time, &zoneData) == 0 )
    {
      /* get difference between now and last */
      diff_sec = cur_time.tv_sec - last_sec;
      diff_msec = cur_time.tv_usec - last_msec;

      /* convert diff to usec */	
      time_offset_msec = (diff_sec * XNEE_USEC_PER_SEC) + diff_msec;


      /* if first time through - save values and return XNEE_OK */
      if ( last_sec == 0  )
	{
	  last_read_msecec = cur_time.tv_usec;
	  last_read_sec = cur_time.tv_sec;
	  first_read_msecec = cur_time.tv_usec;
	  first_read_sec = cur_time.tv_sec;
/* 	  xnee_verbose ((xd, "1st elapsed time:%c _sec _msec: %d %d timeoffeset = 0\n",  */
/* 		       type, cur_time.tv_sec, cur_time.tv_usec )); */
	  return XNEE_OK;
	}

      /* update last_read values with the new time */
      if ( type == XNEE_FROM_LAST_READ )
	{
	  last_read_msecec = cur_time.tv_usec;
	  last_read_sec = cur_time.tv_sec;
    	}
/*       xnee_verbose ((xd, "elapsed time type:%c current time _sec _msec: %d %d time_offset_msec: %d\n",  */
/* 		   type, cur_time.tv_sec, cur_time.tv_usec, time_offset_msec )); */
      return time_offset_msec;
    }

  /* Shouldn't reach this point.... silent the compiler */
  return 0;
}

/**************************************************************
 *                                                            *
 * xnee_calc_sleep_amount                                     *
 *                                                            *
 *                                                            *
 **************************************************************/
long 
xnee_calc_sleep_amount(xnee_data *xd, long last_diff, long first_diff, long record_last_diff, long recordFirst_diff )
{
  long sleep_amt, out_of_wack_amt =0;
  float tmp;

/*   xnee_verbose ((xd, "xnee_calc_sleep_amount last_diff: %lu first_diff: %lu record_last_diff: %lu recordFirst_diff: %lu\n",  */
/* 		last_diff, first_diff, record_last_diff, recordFirst_diff )); */

  /* determine where we are from first read  either too fast or too slow */
  /* find amount that we are too fast or too slow */
  out_of_wack_amt = recordFirst_diff - first_diff;

  if ( out_of_wack_amt > 0 ) /* too fast - we should slow down a bit */
    {
      if ( record_last_diff > last_diff )  /* recorded wait more than we have waited so far */
	{
	  /* if the amount we are out of wack is more than the recorded wait */
	  /* then sleep the full recorded difference */
	  if ( out_of_wack_amt > record_last_diff )  
	    {
	      sleep_amt = record_last_diff;
	    }
	  else /* otherwise sleep the amount out of wack */
	    {
	      sleep_amt = out_of_wack_amt;
	    }
	}
      else  /* we have already waited the recorded amount of time */
	{
	  /* but since we are still too fast - sleep a percentage of recorded wait time */
	  tmp = (float)record_last_diff * XNEE_TOO_FAST_ADJ_PERCENTAGE/100;
	  sleep_amt = ( long )tmp;
	}
    }
  else if ( out_of_wack_amt < 0 ) /* too slow or right on time */ 
    {
      if ( record_last_diff > last_diff )  /* recorded wait more than we have waited so far */
	{
	  sleep_amt = record_last_diff - last_diff; /* amount of wait left */
	}
      else  /* we have already waited the recorded amount of time */
	{
	  sleep_amt = 0;
	}
    }

/*   xnee_verbose (( xd, "xnee_calc_sleep_amount: %d\n", sleep_amt )); */
  return ( sleep_amt );
}








/**************************************************************
 *                                                            *
 * xnee_set_callback                                          *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_set_callback (xnee_data *xd, 
		   callback_ptrptr dest, 
		   char *sym_name)
{
  char *error;
  callback_ptr saved;
  xnee_verbose ((xd, "\nTrying to set \"%s\" as callback\n", sym_name));

  saved = *dest;

  if (xd==NULL)
    {
      return XNEE_NO_MAIN_DATA;
    }
  else
    {
      *dest = (callback_ptr) xnee_dlsym(xd, 
					 xd->plugin_handle,
					 sym_name);
      if ((error = (char*)xnee_dlerror(xd)) != NULL)  
	{
	  xnee_verbose ((xd, "Failed to set \"%s\" from plugin\n", sym_name));
	  *dest = saved ;
	  fputs(error, stderr);
	  return (XNEE_PLUGIN_FILE_ERROR);
	}
      else
	{
	  xnee_verbose ((xd, "Function set  \"%s\" OK \n", sym_name));
	}
    }
  return XNEE_OK;
}


/**************************************************************
 *                                                            *
 * xnee_set_synchronize                                       *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_set_synchronize (xnee_data *xd,
		      synch_ptrptr dest, 
		      char *sym_name)
{

  char *error;
  synch_ptr saved;
  xnee_verbose ((xd, "\nTrying to set \"%s\" as callback\n", sym_name));

  saved = *dest;

  if (xd==NULL)
    {
      return XNEE_NO_MAIN_DATA;
    }
  else
    {
      *dest = (synch_ptr) xnee_dlsym(xd, 
				     xd->plugin_handle, 
				     sym_name);
      
      if ((error = (char*)xnee_dlerror(xd)) != NULL)  
	{
	  xnee_verbose ((xd, "Failed to set \"%s\" from plugin\n", sym_name));
	  *dest = saved ;
	  fputs(error, stderr);
	  return (XNEE_PLUGIN_FILE_ERROR);
	}
      else
	{
	  xnee_verbose ((xd, "Function set  \"%s\" OK \n", sym_name));
	}
    }
  return XNEE_OK;
}



int 
xnee_process_count(xnee_data *xd, int mode)
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
xnee_handle_km(xnee_data *xd)
{
  printf ("key + modifier received\n");
  return ;
}


int 
xnee_process_replies(xnee_data *xd)
{

  int last_count;
  int count; 
  last_count=xnee_process_count (xd, XNEE_PROCESS_GET);
  xnee_process_count (xd, XNEE_PROCESS_RESET);

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
      count=xnee_process_count (xd, XNEE_PROCESS_GET);
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
  xnee_process_count (xd, XNEE_PROCESS_RESET);
  return XNEE_OK;
}


int
xnee_set_autorepeat (xnee_data *xd)
{
  int i ;
  int j;

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


  XAutoRepeatOff(xd->fake);
  return xd->kbd_orig.global_auto_repeat;
}


int
xnee_reset_autorepeat (xnee_data *xd)
{  
  xnee_verbose((xd,"Resetting autorepeat on (%d) to: ",
		xd->fake));
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
  /* make sure the resetting of autorepeat is handled
     before we vlose down the diaplay */
  XFlush (xd->fake);
  
  
}

