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


#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <X11/Xutil.h>

#include <X11/extensions/XTest.h>

#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"
#include "libxnee/xnee_sem.h"
#include "libxnee/xnee_buffer.h"
#include "libxnee/xnee_resolution.h"
#include "libxnee/xnee_dl.h"
#include "libxnee/xnee_km.h"
#include "libxnee/xnee_grab.h"

static int add_ctr=0;

int 
xnee_ungrab_key (xnee_data* xd, int mode)
{
  int window;
  int screen;
  int i ;

  xnee_verbose((xd, "--->  xnee_ungrab_key\n"));
  
  if (xd->grab_keys==NULL)
    {
      xnee_verbose((xd, " ---  xnee_ungrab_key the data seem to have been freed already\n"));
      return XNEE_OK;
    }

  xnee_verbose((xd, " ---  xnee_ungrab_key we have a grab struct allocated\n"));
/*   XNEE_FREE_IF_NOT_NULL(xd->grab_keys->action_keys[mode].str); */


  if (xd->grab_keys->action_keys[mode].key==0) 
    {
      xnee_verbose((xd, "---  xnee_ungrab_key key==0\n"));

      printf (" (((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((( \n");

      return XNEE_OK;
    }

  if ( xd->grab != 0 )
    {
      xnee_verbose((xd, "---  xnee_ungrab_key get screen\n"));
      screen = DefaultScreen (xd->grab);
      xnee_verbose((xd, "---  xnee_ungrab_key get window\n"));
      window = RootWindow(xd->grab, screen );
      xnee_verbose((xd, "---  xnee_ungrab_key\n"));
      xnee_verbose((xd, "window   %d\n", window));
      xnee_verbose((xd, "screen   %d\n", screen));
      xnee_verbose((xd, "data     %d\n", xd->grab));
      xnee_verbose((xd, "stop key %d\n", xd->grab_keys->action_keys[mode].key));
      xnee_verbose((xd, "stop mod %d\n", AnyModifier));
      XUngrabKey (xd->grab,  
		  xd->grab_keys->action_keys[mode].key,            
		  AnyModifier,
		  window);
      xd->grab_keys->grab=1;
      xd->grab_keys->action_keys[mode].key=0;
    }
  return XNEE_OK;
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
  xnee_verbose((xd, "---> xnee_ungrab_keys\n")); 

  xnee_ungrab_key ( xd, XNEE_GRAB_STOP);
  xnee_ungrab_key ( xd, XNEE_GRAB_PAUSE);
  xnee_ungrab_key ( xd, XNEE_GRAB_RESUME);
  xnee_ungrab_key ( xd, XNEE_GRAB_INSERT);
  xnee_ungrab_key ( xd, XNEE_GRAB_EXEC);
  
  xnee_verbose((xd, "<--- xnee_ungrab_keys\n"));
  return XNEE_OK;
}



/**************************************************************
 *                                                            *
 * xnee_grab_key                                              *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_grab_key (xnee_data* xd, int mode, char *key)
{
  int window;
  int screen;
  xnee_action_key ak;
  int i ;

  xnee_verbose((xd, "----> xnee_grab_key\n"));
  if (key==NULL)
    {
      return XNEE_BAD_GRAB_DATA;
    }

  xnee_get_key (xd, &ak, key);
  xnee_verbose((xd, "----  xnee_grab_key key=%s\n", key));
  xnee_verbose((xd, "----  xnee_grab_key key=%d\n", ak.key));

  if (ak.key==0) 
    return XNEE_BAD_GRAB_DATA;

  if ( (mode>=XNEE_GRAB_STOP) && (mode<XNEE_GRAB_LAST))
    {
      xd->grab_keys->action_keys[mode].key=ak.key;
    }
  else
    {
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
  xnee_verbose((xd, "grab     %d\n", xd->grab));
  xnee_verbose((xd, "stop key %d\n", ak.key));
  xnee_verbose((xd, "stop mod %d\n", AnyModifier));
  xnee_verbose((xd, "window   %d\n", window));
  xnee_verbose((xd, "screen   %d\n", screen));

  XGrabKey (xd->grab,  
	    ak.key,            
	    AnyModifier,
	    window,       
	    False,  
	    GrabModeSync,
	    GrabModeSync );

  xnee_verbose((xd, "<---- xnee_grab_stop_key\n"));

  return XNEE_OK;
}



int
xnee_get_grab_mode (xnee_data *xd, int key)
{
  int i ;

  for (i=XNEE_GRAB_STOP;i<XNEE_GRAB_LAST;i++)
    {
      if ( key == xd->grab_keys->action_keys[i].key) 
	{
	  return i;
	}
    }
  
 return XNEE_GRAB_UNKOWN;
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
  int i;
  xnee_grab_keys* xgk = 
    (xnee_grab_keys*) malloc (sizeof (xnee_grab_keys));
  /*  memset (xgk, 0, sizeof (xnee_grab_keys)); */

  if (xgk==NULL)
    return NULL;

  xgk->grab = XNEE_GRAB_NODATA;
  xgk->grabbed_action = XNEE_GRAB_NODATA;

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
  xnee_verbose((xd, "--->  xnee_free_grab\n"));
  XNEE_FREE_IF_NOT_NULL (xd->grab_keys);
  if (xd->grab_keys==NULL)
    {
      xnee_verbose((xd, "---  xnee_free_grab  .... yes, we are NULL\n"));
      return XNEE_OK;
    }
  xnee_verbose((xd, "<---  xnee_free_grab\n"));
  return XNEE_OK;
}




/**************************************************************
 *                                                            *
 * xnee_grab_all_keys                                         *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_grab_all_keys (xnee_data* xd)
{
  int window;
  int screen;
  int ret;
  int i ; 
  xnee_action_key ak;

  xnee_verbose((xd, "----> xnee_grab_all_keys\n"));
  /* check validity on the k+m combinations */
  ret = xnee_key_check(xd);
  if (ret != XNEE_OK)
    {
      xnee_verbose((xd, "----  xnee_grab_all_keys: "
		    "trying to grab same key + modifier   return=%d  (ok=%d)\n", ret, XNEE_OK));
      return XNEE_BAD_GRAB_DATA;
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
  screen = DefaultScreen (xd->grab);
  window = RootWindow    (xd->grab, screen );
  
  xnee_verbose((xd, "----  xnee_grab_all_keys\n"));
  
  for (i=XNEE_GRAB_STOP;i<XNEE_GRAB_LAST;i++)
    {
      xd->grab_keys->action_keys[i].key=0;

      if (xd->grab_keys->action_keys[i].str!=NULL)
	xnee_get_action_key (xd, &ak, xd->grab_keys->action_keys[i].str);

      xnee_verbose((xd, "----  xnee_grab_all_keys %d (%s)\n",ak.key,xd->grab_keys->action_keys[i].str ));
      
      if (ak.key!=0) 
	{
	  xd->grab_keys->action_keys[i].key = ak.key;
	  xnee_verbose((xd, "----  xnee_grab_all_keys %d\n",i));
	  xd->grab_keys->grab=XNEE_GRAB_SET;

	  XGrabKey (xd->grab,  
		    xd->grab_keys->action_keys[i].key,            
		    AnyModifier,
		    window,       
		    True,  
		    GrabModeSync,
		    GrabModeSync );
	}
      
    }

  xnee_verbose((xd, "<----  xnee_grab_all_keys \n"));
  
  return XNEE_OK;
}



int
xnee_grab_keys_init(xnee_data *xd)
{
  int i ;
  xd->grab_keys->grab           = 0 ; 
  xd->grab_keys->grabbed_action = 0 ; 

  for (i=XNEE_GRAB_STOP;i<XNEE_GRAB_LAST;i++)
    {
      xd->grab_keys->action_keys[i].key=0;
      xd->grab_keys->action_keys[i].str=NULL;
      xd->grab_keys->action_keys[i].extra_str=NULL;
    }
  return XNEE_OK;
}






static int grab_mode_used = 0 ;

static KeyCode mod_kcs[8];
static int     mod_idx = 0 ;

static int     current_modifier_state     = 0 ;
static int     last_modifier_state     = 0 ;
static int     saved_line_override = 0 ;
static int     non_used_mod        = 0 ;

#define XNEE_GRAB_SET_OVERRIDE(xd)   xnee_grab_handle_override(xd, XNEE_OVERRIDE_TRUE) ; \
        xnee_grab_handle_buffer(xd , NULL , XNEE_GRAB_BUFFER_PRINT)

#define XNEE_GRAB_UNSET_OVERRIDE(xd)  xnee_grab_handle_override(xd, XNEE_OVERRIDE_FALSE)
#define XNEE_GRAB_GET_OVERRIDE(xd)    xnee_grab_handle_override(xd, XNEE_OVERRIDE_GET)
#define XNEE_GRAB_IS_OVERRIDE(xd)     \
  ( XNEE_OVERRIDE_TRUE == xnee_grab_handle_override(xd, XNEE_OVERRIDE_GET) )





#ifdef USE_OBSOLETE
static int 
xnee_mod_in_use_sub (xnee_km_tuple *km)
{
  int i ;
  int j ;
  int found = 0;
  int nr_of_found = 0;
  int ret ;

  for (i=0;i<km->kcs_idx;i++)
    {
      for (j=0;j<mod_idx;j++)
	{
	  if ( 
	      (km->kcs[i][0]==mod_kcs[j])
	      ||
	      (km->kcs[i][1]==mod_kcs[j])
	      )
	    {
	      found = 1;
	    }
	}
      if (found)
	{
	  found = 0;
	  nr_of_found++;
	}
    }

  /* No modiifiers pressed at all */
  if ( mod_idx == 0 )
    {
      ret = XNEE_GRAB_NONE_PRESSED ;
    }

  /* Modifier pressed that is not grabbed by xnee  */
  else if ( ( mod_idx > nr_of_found ) && (mod_idx>0) )
    { 
      ret = XNEE_GRAB_NONE_IN_USE ;
    }

  /* All modifier pressed that is grabbed by xnee  */
  else if ( (nr_of_found==km->kcs_idx) && 
	    ((km->kcs_idx)==mod_idx) && 
	    (nr_of_found!=0) )
    {
      ret = XNEE_GRAB_ALL_IN_USE ;
    }

  /* Some modifier pressed that is grabbed by xnee  */
  else if ( nr_of_found!=0 )
    {
      ret = XNEE_GRAB_SOME_IN_USE ;
    }
  else
    {
      printf ("\n\n\nWARNING HESA \n\n\n");
      ret = XNEE_GRAB_SOME_IN_USE ;
      printf ("   SUB: %d %d %d : RETURN %d \n", nr_of_found, km->kcs_idx, mod_idx, ret);    
    }

/*   printf ("   SUB: %d %d %d : RETURN %d \n", nr_of_found, km->kcs_idx, mod_idx, ret);     */

  return ret;
}


static void
xnee_mod_in_use (xnee_grab_keys *xgk)
{
  last_modifier_state = current_modifier_state;

  current_modifier_state = XNEE_GRAB_NONE_PRESSED ;

  current_modifier_state = XNEE_MAX(current_modifier_state,
			     xnee_mod_in_use_sub (&xgk->stop));

  current_modifier_state = XNEE_MAX(current_modifier_state,
			     xnee_mod_in_use_sub (&xgk->pause));

  current_modifier_state = XNEE_MAX(current_modifier_state,
			     xnee_mod_in_use_sub (&xgk->resume));

  current_modifier_state = XNEE_MAX(current_modifier_state,
			     xnee_mod_in_use_sub (&xgk->insert));

  current_modifier_state = XNEE_MAX(current_modifier_state,
			     xnee_mod_in_use_sub (&xgk->exec));

  return ;
}
#endif 

static int
xnee_key_in_use (xnee_grab_keys *xgk, KeyCode kc)
{
  int key_used = 0;
  int i ;

  for (i=XNEE_GRAB_STOP;i<XNEE_GRAB_LAST;i++)
    {
      if (xgk->action_keys[i].key==kc)
	return 1;
    }
  return 0;
}



static int 
xnee_add_mod (KeyCode kc)
{
  int i ;


  if (mod_idx<8)
    {
      mod_kcs[mod_idx++]=kc;

      return XNEE_OK;
    }
  else
    {
      return XNEE_BAD_GRAB_DATA;
    }
}

static int 
xnee_rem_mod (KeyCode kc)
{
  int i ;
  int found = 0;
  int pos ; 

  for (i=0;i<8;i++)
    {

      if (mod_kcs[i]==0)
	{
	  break;
	}
      
      if (mod_kcs[i]==kc)
	{
	  found=1;
	  pos = i;
	  break;
	}
    }
  if (found)
    {
      mod_idx--;
      i = XNEE_OK;
      for (i=pos;i<8;i++)
	{
	  if ( i == 7 )
	    {
	      mod_kcs[i]=0;
	    }
	  else
	    {
	      mod_kcs[i]=mod_kcs[i+1];
	    }
	}
    }
  else
    {
      i= XNEE_BAD_GRAB_DATA;
    }
  
  return i;
}




int 
xnee_grab_handle_buffer(xnee_data *xd, char *str, int mode)
{
  static char saved_buf[XNEE_SAVED_LINES_BYTES];

  xnee_verbose((xd, "---> xnee_grab_handle_buffer %d \n", mode));
  if (mode==XNEE_GRAB_BUFFER_SAVE)
    {
      if ( (saved_buf==NULL) || (str==NULL) 
	   ||
	   (strlen(saved_buf) > (XNEE_SAVED_LINES_BYTES - 128) )
	   )
	{
	  xnee_verbose((xd, "<--- xnee_grab_handle_buffer %d \n", XNEE_GRAB_MEM_FAILURE));
	  return XNEE_GRAB_MEM_FAILURE;
	}
      strcat (saved_buf, str);
      xnee_verbose((xd, "---  xnee_grab_handle_buffer saved '%s'\n", str));
      
    }
  else if (mode==XNEE_GRAB_BUFFER_PRINT)
    {
      if (strlen(saved_buf)>0)
	{
	  fprintf (xd->out_file,"%s", saved_buf);
	}
      saved_buf[0]='\0';
    }
  else if (mode==XNEE_GRAB_BUFFER_RESET)
    {
      fprintf (stderr, "grab mode buffer reset\n");
      saved_buf[0]='\0';
    }
  xnee_verbose((xd, "<--- xnee_grab_handle_buffer %d \n", XNEE_OK));
  return XNEE_OK;
}

static int
xnee_grab_handle_override(xnee_data *xd, int mode )
{
  static int current_grab_mode_override = XNEE_OVERRIDE_FALSE ;

  xnee_verbose((xd, "---> xnee_handle_grab_override %d \n", mode));

  switch (mode)
    {
    case XNEE_OVERRIDE_TRUE:
      xnee_grab_handle_buffer(xd, NULL, XNEE_GRAB_BUFFER_PRINT);
      current_grab_mode_override = mode ;
      break;
    case XNEE_OVERRIDE_FALSE:
      current_grab_mode_override = mode ;
      break;
    case XNEE_OVERRIDE_GET:
      break ;
    default:
      xnee_verbose((xd, " --- ERROR: default mode reached in switch\n" ));
      break;
    }
  xnee_verbose((xd, "<--- xnee_handle_grab_override %d \n", current_grab_mode_override));
  return current_grab_mode_override;
}


static int
xnee_handle_grab_mouse(xnee_data *xd)
{
  int ret ; 

  xnee_verbose((xd, "--->  xnee_handle_grab_mouse %d %d \n",
		current_modifier_state,
		last_modifier_state));

  if ( (current_modifier_state==XNEE_GRAB_NONE_PRESSED)
       ||
       (last_modifier_state==XNEE_GRAB_NONE_IN_USE) 
       ||
       (current_modifier_state==XNEE_GRAB_NONE_IN_USE) )
    {
      ret = XNEE_GRAB_DO_PRINT;
    }
  else
    {
      XNEE_GRAB_SET_OVERRIDE(xd);
    }

  ret = XNEE_GRAB_DO_PRINT ; 
  xnee_verbose((xd, "<--- xnee_handle_grab_mouse %d\n", ret));
}

static int
xnee_handle_grab_modifier(xnee_data *xd, KeyCode kc, int mode)
{
  xnee_grab_handle_buffer(xd, NULL, XNEE_GRAB_BUFFER_PRINT);
  return XNEE_GRAB_DO_PRINT;
}


#ifdef USE_OBSOLETE
static int
xnee_handle_grab_modifier(xnee_data *xd, KeyCode kc, int mode)
{
  int ret ;

  xnee_verbose((xd, "---> xnee_handle_grab_modifier %d %d\n", kc, mode));
  if (mode==XNEE_GRAB_KM_PRESS)
    {
      /* add this modifier to the buffer */
      xnee_add_mod(kc);
    }
  else
    {
      /* remove this modifier from the buffer */
      xnee_rem_mod(kc);
    }

  /* Update the global state */
  xnee_mod_in_use(xd->grab_keys);
  

  if ( XNEE_GRAB_IS_OVERRIDE(xd) )
    {
      xnee_grab_handle_buffer(xd, NULL, XNEE_GRAB_BUFFER_PRINT);
      ret = XNEE_GRAB_DO_PRINT ; 

      if ( current_modifier_state == XNEE_GRAB_NONE_PRESSED )
	XNEE_GRAB_UNSET_OVERRIDE(xd);
      
    }
  else if ( current_modifier_state == XNEE_GRAB_NONE_PRESSED )
    {
      if ( grab_mode_used == 0 )
	{
	  xnee_grab_handle_buffer(xd , NULL , XNEE_GRAB_BUFFER_PRINT);
	  ret = XNEE_GRAB_DO_PRINT ; 
	}
      else if ( grab_mode_used == 1 )
	{
	  xnee_grab_handle_buffer(xd , NULL , XNEE_GRAB_BUFFER_RESET);
	  ret = XNEE_GRAB_DONT_PRINT ; 
	}
      else
	{
	  xnee_grab_handle_buffer(xd , NULL , XNEE_GRAB_BUFFER_PRINT);
	  ret = XNEE_GRAB_DO_PRINT ; 
	}
      grab_mode_used=0;
      XNEE_GRAB_UNSET_OVERRIDE(xd);
  
    }
  else if ( current_modifier_state == XNEE_GRAB_NONE_IN_USE )
    {
      xnee_grab_handle_buffer(xd , NULL , XNEE_GRAB_BUFFER_PRINT);
      XNEE_GRAB_SET_OVERRIDE(xd);
      ret = XNEE_GRAB_DO_PRINT ; 
    }
  else if ( current_modifier_state == XNEE_GRAB_SOME_IN_USE )
    {
      if (grab_mode_used == 2 )
	{
	  ret = XNEE_GRAB_DO_PRINT ; 
	}
      else
	{
	  ret = XNEE_GRAB_DO_SAVE ; 
	}
    }
  else if ( current_modifier_state == XNEE_GRAB_ALL_IN_USE )
    {
      ret = XNEE_GRAB_DO_SAVE ; 
    }
  
  xnee_verbose((xd, "<--- xnee_handle_grab_modifier %d \n", ret));
  return ret;
}
#endif

static int
xnee_handle_grab_key(xnee_data *xd, KeyCode kc, int mode)
{
  int ret ;

  xnee_verbose((xd, "---> xnee_handle_grab_key %d %d\n", kc, mode));
  

  if ( XNEE_GRAB_IS_OVERRIDE(xd) )
    {
      xnee_grab_handle_buffer(xd, NULL, XNEE_GRAB_BUFFER_PRINT);
      ret = XNEE_GRAB_DO_PRINT ; 
    }
  else if ( current_modifier_state == XNEE_GRAB_NONE_PRESSED )
    {
      ret = XNEE_GRAB_DO_PRINT ; 
    }
  else if ( current_modifier_state == XNEE_GRAB_NONE_IN_USE )
    {
      ret = XNEE_GRAB_DO_PRINT ; 
    }
  else if ( current_modifier_state == XNEE_GRAB_SOME_IN_USE )
    {
      
      ret = XNEE_GRAB_DO_SAVE ; 
    }
  else if ( current_modifier_state == XNEE_GRAB_ALL_IN_USE )
    {
      if ( xnee_key_in_use ( xd->grab_keys , kc))
	{
	  fprintf (stderr, PACKAGE " ("  VERSION 
		   ") discarded a key %s (keycode=%d)\n",
		   mode==XNEE_GRAB_KM_PRESS?"press":"release",
		   kc);
	  ret = XNEE_GRAB_DONT_PRINT ; 
	  if (grab_mode_used==0)
	    grab_mode_used = 1 ;
	}
      else
	{
	  xnee_grab_handle_buffer(xd , NULL , XNEE_GRAB_BUFFER_PRINT);
	  grab_mode_used = 2 ;
	  ret = XNEE_GRAB_DO_PRINT ; 
	}
    }
  
  xnee_verbose((xd, "<--- xnee_handle_grab_key %d \n", ret));
  return ret;
}

int
xnee_save_or_print(xnee_data *xd, KeyCode kc, int mode)
{
  int ret; 
  int is_modifier ;
  KeySym ks ;
  xnee_verbose((xd, "---> xnee_save_or_print %d %d\n", kc, mode));
  ks = XKeycodeToKeysym(xd->control, kc, 0);
  

  if ( mode == XNEE_GRAB_MOUSE )
      {
	xnee_verbose((xd, "--- xnee_save_or_print treat a mouse\n"));
	ret = xnee_handle_grab_mouse(xd);
      }
    else if (IsModifierKey (ks))
      {
	xnee_verbose((xd, "--- xnee_save_or_print treat a modifier\n"));
	ret = xnee_handle_grab_modifier(xd, kc, mode);
      }
    else
      {
	xnee_verbose((xd, "--- xnee_save_or_print treat an ordinary key\n")); 
	ret = xnee_handle_grab_key(xd, kc, mode); 
    }

/*   if ( ret == XNEE_GRAB_DO_SAVE ) */
/*     printf (" DO SAVE\n"); */
/*   else if ( ret == XNEE_GRAB_DO_PRINT ) */
/*     printf (" DO PRINT\n"); */
/*   else if ( ret == XNEE_GRAB_DONT_PRINT ) */
/*     printf (" DONT PRINT\n"); */

   xnee_verbose((xd, "<--- xnee_save_or_print %d\n", ret));
  return ret;
}

