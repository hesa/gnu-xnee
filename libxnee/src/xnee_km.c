/*****  
 *       Xnee's Not an Event Emulator  
 *                                                                    
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 * Copyright (C) 1999, 2000, 2001, 2002, 
 *               2003, 2004, 2005, 2006, 2007 Henrik Sandklef 
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


#include "libxnee/print_varargs.h"
#include "libxnee/print.h"
#include "libxnee/datastrings.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"
#include "libxnee/xnee_km.h"
#include "libxnee/feedback.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_grab.h"
#include "libxnee/xnee_grab.h"
#include "libxnee/feedback.h"


int xkm_rem_blanks (char *array);


xnee_keymask
get_modifier_sub(xnee_data *xd,  char *mod_str) ;

const char *
xnee_symbolic_modifier2modifier (xnee_data *xd, const char *mod_str)
{
   xnee_verbose((xd, "--- xnee_symbolic_modifier2modifier\n"));
   if (xnee_check(mod_str, XNEE_SYMBOLIC_ALT, XNEE_SYMBOLIC_ALT))
    {
      return XNEE_REAL_ALT;
    }
  else if (xnee_check(mod_str, XNEE_SYMBOLIC_SHIFT, XNEE_SYMBOLIC_SHIFT))
    {
      return XNEE_REAL_SHIFT;
    }
  else if (xnee_check(mod_str, XNEE_SYMBOLIC_CONTROL, XNEE_SYMBOLIC_CONTROL))
    {
      return XNEE_REAL_CONTROL;
    }
  else if (xnee_check(mod_str, XNEE_SYMBOLIC_CAPS, XNEE_SYMBOLIC_CAPS))
    {
      return XNEE_REAL_CAPS;
    }
  else if (xnee_check(mod_str, XNEE_SYMBOLIC_SCROLL, XNEE_SYMBOLIC_SCROLL))
    {
      return XNEE_REAL_SCROLL;
    }
  else 
    return NULL;
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
get_modifier_from_mapping_sub(Display *display, char *mod_str) 
{
  XModifierKeymap *mod_keymap;
  KeyCode *keycode_ptr;
  KeySym my_keysym;
  int i, j, max_keys;
  int ret=0;

  static char *mod_list[]={
    "ShiftMask", "LockMask","ControlMask", 
    "Mod1Mask",  "Mod2Mask", "Mod3Mask", 
    "Mod4Mask", "Mod5Mask" };

  mod_keymap = XGetModifierMapping(display);
  max_keys = mod_keymap->max_keypermod;
  keycode_ptr = mod_keymap->modifiermap;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < max_keys; j++) {
      my_keysym = XKeycodeToKeysym(display, *keycode_ptr, 0);

      if (XKeysymToString(my_keysym)!=NULL)
	{
	  if (xnee_check (mod_str, 
			  XKeysymToString(my_keysym),
			  XKeysymToString(my_keysym)))
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



xnee_keymask
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
xnee_check_key(xnee_data *xd)
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
		    SyncKeyboard,
		    CurrentTime);
      XFlush (xd->grab);

      if ( XCheckMaskEvent ( xd->grab, 0xffffffff , &ev) == True)
	{
	  int    tmp_code;
	  int    mode;
	  int i ;

	  if (ev.xkey.send_event==1) 
	    { 
	      xnee_verbose ((xd , "send_event==true\n")); 
	    }

	  if (ev.type == KeyPress )
	    {
	      XEvent my_event ;
	      xnee_verbose ((xd , "##### KeyPress\n")); 
	      XNextEvent (xd->grab, &my_event);
	    }
	  else if (ev.type == KeyRelease )
	    {
	      xnee_verbose ((xd , "#### KeyRelease\n")); 
	      return XNEE_GRAB_NODATA;
	    }

	  tmp_code=ev.xkey.keycode;

	  

	  xnee_verbose ((xd, "key = %d\n", tmp_code));
	  mode = xnee_get_grab_mode (xd, tmp_code);

	  xnee_verbose ((xd, "mode     = %d\n", mode));
	  
	  for (i=XNEE_GRAB_STOP;i<XNEE_GRAB_LAST;i++)
	    {
	      xnee_verbose((xd, "     compare %d == %d",tmp_code, xd->grab_keys->action_keys[i].key));
	      xnee_verbose((xd, "     str:    %s",xd->grab_keys->action_keys[i].str));
	      xnee_verbose((xd, "     code:   %d",tmp_code));
	      if ( tmp_code == xd->grab_keys->action_keys[i].key )
		{
		  xd->grab_keys->grabbed_action = i ; 
		  xnee_verbose((xd, "\n\n\tUSER PUSHED KEY ... i=%d\n\n\n",i));
		  return XNEE_GRAB_DATA ; 
		}	    
	    }
	  return XNEE_GRAB_NODATA;
	}
    }
  else
    {
      return XNEE_GRAB_NODATA;
    }
  return XNEE_GRAB_NODATA;
}


int 
xnee_handle_rec_key(xnee_data *xd)
{

  int    ret=XNEE_OK;
  static int exec_ctr = 0; /* Keeps track of nr of exec grabs */
  char *exec_prog ;
  char *tmp_ptr;
  int len;
  int exec_ret;

  xnee_verbose ((xd, " ---> xnee_handle_rec_km\n"));

  switch ( xd->grab_keys->grabbed_action )
    {
    case XNEE_GRAB_STOP :
       feedback (xd, "Xnee stop received");
      break;

    case XNEE_GRAB_PAUSE :
      feedback (xd, "Xnee pause received");
      break;

    case XNEE_GRAB_RESUME :
      feedback (xd, "Xnee resume received");
      break;

    case XNEE_GRAB_INSERT :
      feedback (xd, "Xnee insert received");
      break;

    case XNEE_GRAB_EXEC :
      exec_ctr++;
      feedback (xd, "Xnee exec received");
      break;

    default:
      ret=XNEE_NO_GRAB_DATA;
      break;
      
    }


  if (xd->grab_keys->grabbed_action==XNEE_GRAB_STOP)
    {
      xnee_verbose ((xd, " ---  xnee_handle_rec_km: STOP \n"));
      xnee_verbose ((xd, " <--- xnee_handle_rec_km\n"));
    }
  else if (xd->grab_keys->grabbed_action==XNEE_GRAB_PAUSE)
    {
      xnee_verbose ((xd, " ---  xnee_handle_rec_km: PAUSE \n"));
      feedback (xd, "Xnee pause received");
      xnee_unsetup_recording (xd);
      ret=XNEE_GRAB_PAUSE;
    }
  else if (xd->grab_keys->grabbed_action==XNEE_GRAB_RESUME)
    {
      xnee_verbose ((xd, " ---  xnee_handle_rec_km: RESUME \n"));
      feedback (xd, "Xnee resume received");
      xnee_setup_recording (xd);
      ret=XNEE_GRAB_RESUME;
    }
  else if (xd->grab_keys->grabbed_action==XNEE_GRAB_EXEC)
    {
      xnee_verbose ((xd, " ---  xnee_handle_rec_km: EXEC (nr %d)\n", exec_ctr));
      
      tmp_ptr =  xnee_get_exec_prog(xd);
      if ( tmp_ptr == NULL ) 
	{
	  ret=XNEE_NO_GRAB_DATA;
	  xnee_verbose((xd, "Xnee exec received... couldn't find valid exec program\n"));
	  return ret;
	}
      
      len = strlen (tmp_ptr);
      len = len + 10 ; 
      
      exec_prog = (char*) calloc(len, sizeof(char));
      
      sprintf(exec_prog, "%s %d &", xnee_get_exec_prog(xd), exec_ctr);
      
      xnee_verbose ((xd, " ---  xnee_handle_rec_km: EXEC \n"));
      feedback (xd, "Xnee exec received");
      
      if (exec_prog==NULL)
	{
	  fprintf (xd->out_file, "%s   \n",XNEE_EXEC_MARK);
	}
      else
	{
	  fprintf (xd->out_file, "%s    %s\n", XNEE_EXEC_MARK, exec_prog);
	  exec_ret= system (exec_prog );
	  
	  if ( exec_ret != 0 )
	    {
	      fprintf(stderr, "Failed to launch: %s\n", "xterm");
	    }
	  free(exec_prog);
	  
	  ret=XNEE_GRAB_EXEC;
	}
    }
  else if (xd->grab_keys->grabbed_action==XNEE_GRAB_INSERT)
    {
      time_t rawtime;
      struct tm * timeinfo;
      
      feedback (xd, "Xnee inserting mark in log file");
      xnee_verbose ((xd, " ---  xnee_handle_rec_km: MARK \n"));
      time ( &rawtime );
      timeinfo = localtime ( &rawtime );
      fprintf (xd->out_file, "M MARK:%.4d-%.2d-%.2d:%.2d.%.2d.%.2d\n",
	       timeinfo->tm_year + 1900 , 
	       timeinfo->tm_mon + 1 , 
	       timeinfo->tm_mday  ,
	       timeinfo->tm_hour, 
	       timeinfo->tm_min , 
	       timeinfo->tm_sec  );
      ret=XNEE_GRAB_INSERT;
    }
  else 
    {
      xnee_verbose ((xd, " ---  xnee_handle_rec_km: UNKNOWN \n"));
    }  
  
  xnee_verbose ((xd, " <--- xnee_handle_rec_km\n"));
  return ret;
}


int 
xnee_handle_rep_key(xnee_data *xd)
{
  int ret=XNEE_OK;
  int exec_ret;
  xnee_verbose ((xd, " ---> xnee_handle_rep_km\n"));

  if (xd->grab_keys->grabbed_action==XNEE_GRAB_STOP)
    {
      xnee_verbose ((xd, " ---  xnee_handle_rep_km: STOP \n"));
      xnee_verbose ((xd, " <--- xnee_handle_rep_km\n"));
      feedback (xd, "Xnee stop received");
      ret=XNEE_GRAB_STOP;
    }
  else if (xd->grab_keys->grabbed_action==XNEE_GRAB_PAUSE)
    {
      xnee_verbose ((xd, " ---  xnee_handle_rep_km: PAUSE \n"));
      feedback (xd, "Xnee pause received");
      fprintf (stderr, "pause during replay is not implemented yet\n");
      ret=XNEE_GRAB_PAUSE;
    }
  else if (xd->grab_keys->grabbed_action==XNEE_GRAB_RESUME)
    {
      xnee_verbose ((xd, " ---  xnee_handle_rep_km: RESUME \n"));
      feedback (xd, "Xnee resume received");
      fprintf (stderr, "resume during replay is not implemented yet\n");
      ret=XNEE_GRAB_RESUME;
    }
  else if (xd->grab_keys->grabbed_action==XNEE_GRAB_EXEC)
    {
      xnee_verbose ((xd, " ---  xnee_handle_rep_km: EXEC \n"));
      feedback (xd, "Xnee exec received");
      exec_ret = system ("xterm&");

      if ( exec_ret != 0 )
	{
	  fprintf(stderr, "Failed to launch: %s\n", "xterm");
	}
      ret=XNEE_GRAB_EXEC;
    }
  else if (xd->grab_keys->grabbed_action==XNEE_GRAB_INSERT)
    {
      feedback (xd, "Xnee inserting mark in log file");
      xnee_verbose ((xd, " ---  xnee_handle_rep_km: MARK \n"));
      fprintf (stderr, "insert during replay is not implemented yet\n");
      ret=XNEE_GRAB_INSERT;
    }
  else 
    {
      xnee_verbose ((xd, " ---  xnee_handle_rep_km: UNKNOWN \n"));
    }  

  xnee_verbose ((xd, " <--- xnee_handle_rep_km\n"));
  return ret;
}


/**************************************************************
 *                                                            *
 * xnee_get_key                                               *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_get_action_key (xnee_data     *xd, 
		     xnee_action_key *ak, 
		     char          *key)
{
  KeySym ks;

  if (key==NULL)
    {
      ak->key=0;
    }

  xnee_verbose((xd, " ---> xnee_get_action_key %s \n", key));

  ks = XStringToKeysym (key)  ; 

  if ( ks == 0 )
    {
      xnee_print_error ("SYNTAX ERROR\n\tCouldn't convert "
			"string \"%s\" to an int\n",key);
      return ( XNEE_SYNTAX_ERROR);
    }
  xnee_verbose((xd, "  --- xnee_get_action_key %s gives us %d \n", 
		key, (int)ks));

  /* OK, it's a integer... just use it! */
  ak->key = XKeysymToKeycode (xd->grab, ks);
  
  xnee_verbose((xd, " <--- xnee_get_action_key %s gives us %d \n", key, ak->key));
  
  return XNEE_OK;
}


static int
xnee_km_check_not_same(xnee_data *xd, char *str, int start)
{
  int ret  = 0 ;
  char *tmp_ptr = NULL ; 


  if (start!=XNEE_GRAB_UNKOWN)
    {
      if (start == XNEE_GRAB_STOP)
	{
	  return  XNEE_OK;
	}
      else if ( (start>0) && (start<XNEE_GRAB_LAST))
	{
	  tmp_ptr = xd->grab_keys->action_keys[start].str;
	}
      else
	{
	  return XNEE_OK;
	}

      if (str==NULL)
	{
	  return  XNEE_OK + xnee_km_check_not_same( xd, str, start + 1); 
	}

      if (tmp_ptr!=NULL)
	{
	  if (strcmp ( str, tmp_ptr)==0)
	    {
	      ret = 1;
	    }
	}
      xnee_verbose((xd, "---  xnee_km_check_not_same %s %s %d ret =  %d\n",
		str, tmp_ptr, start, ret ));

      return ret + xnee_km_check_not_same( xd, str, start + 1); 
    }
  else 
    {
      xnee_verbose((xd, "---  xnee_km_check_not_same %s %s %d ret =  zero\n",
		str, tmp_ptr, start ));

      return 0 ;
    }

}

int
xnee_key_check(xnee_data *xd)
{
  int ret ; 
  xnee_verbose((xd, " ---> xnee_km_check\n"));

  ret = xnee_km_check_not_same(xd, 
			       xd->grab_keys->action_keys[XNEE_GRAB_STOP].str, 
			       XNEE_GRAB_STOP);
  
  xnee_verbose((xd, "  --- xnee_km_check %d \n", ret));

  xnee_verbose((xd, " <--- xnee_km_check\n"));
  return ret;
}



