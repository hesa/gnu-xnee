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


#include <X11/extensions/XTest.h>

#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"
#include "libxnee/xnee_sem.h"
#include "libxnee/xnee_buffer.h"
#include "libxnee/xnee_resolution.h"
#include "libxnee/xnee_dl.h"

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
  xnee_verbose((xd, "---> xnee_ungrab_stop_key\n"));
  xnee_ungrab_key ( xd, XNEE_GRAB_STOP);
  xnee_ungrab_key ( xd, XNEE_GRAB_PAUSE);
  xnee_ungrab_key ( xd, XNEE_GRAB_RESUME);
  
  xnee_verbose((xd, "<--- xnee_ungrab_stop_key\n"));
  return XNEE_OK;
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


  
  /* get the key+modifier from xd
   * corresponding to the mode given */
  switch (mode)
    {
    case XNEE_GRAB_STOP:
      xd->grab_keys->grab=XNEE_GRAB_SET;
      xd->grab_keys->stop_key=km.key;
      xd->grab_keys->stop_mod=km.modifier;
      xnee_verbose((xd, "----  xnee_grab_key STOP mode\n"));
      break;
    case XNEE_GRAB_PAUSE:
      xd->grab_keys->grab=XNEE_GRAB_SET;
      xd->grab_keys->pause_key=km.key;
      xd->grab_keys->pause_mod=km.modifier;
      xnee_verbose((xd, "----  xnee_grab_key PAUSE mode\n"));
      break;
    case XNEE_GRAB_RESUME:
      xd->grab_keys->grab=XNEE_GRAB_SET;
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

  xnee_set_verbose (xd);
  
  /* grab key + modifier */
  screen = DefaultScreen (xd->grab);
  window = RootWindow    (xd->grab, screen );
  xnee_verbose((xd, "grab     %d\n", xd->grab));
  xnee_verbose((xd, "stop key %d\n", km.key));
  xnee_verbose((xd, "stop mod %d\n", km.modifier));
  xnee_verbose((xd, "window   %d\n", window));
  xnee_verbose((xd, "screen   %d\n", screen));
  XGrabKey (xd->grab,  
	    km.key,            
	    km.modifier,
	    window,       
	    False,  
	    GrabModeSync,
	    GrabModeSync );
  xnee_verbose((xd, "<---- xnee_grab_stop_key\n"));

  xnee_unset_verbose (xd);
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
  free (xd->grab_keys);
  return XNEE_OK;
}
