/*****  
 *       Xnee's Not an Event Emulator  
 *                                                                    
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 * Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004 Henrik Sandklef 
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




#ifdef USE_OBSOLETE
xnee_keymask
get_modifier_sub(xnee_data *xd,  char *mod_str, xnee_km_tuple *km);



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
	      ret |= get_modifier_sub(NULL, mod_list[i], NULL);
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
get_modifier_sub(xnee_data *xd,  char *mod_str, xnee_km_tuple *km) 
{
  int ret=0;

  if ( (xd!=NULL) && (km!=NULL))
    {
      xnee_mod2kc(xd, mod_str, km);
    }

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
#endif



#ifdef USE_OBSOLETE
static int 
is_last (xnee_data *xd,  char *mod_strs) 
{
  char *tmp;
  int i ;
  int len = strlen (mod_strs);
  tmp=mod_strs;

  xnee_verbose ((xd, " ---> is_last ('%s' %d)\n", mod_strs, len));

  for (i=0;  (i<len) ; i++)
    {
      if (tmp[i]=='+')
	{
	  return XNEE_OK;
	}
    }
  xnee_verbose ((xd, " <--- is_last\n"));
  return 1;
}

xnee_keymask
get_modifier(xnee_data *xd,  char *mod_strs, xnee_km_tuple *km) 
{
  #define MODIFIER_BUF_SIZE 50
  int ret=0;
  /*  int try_ret;*/
  char *tmp;
  char mod_head[MODIFIER_BUF_SIZE]="";
  char mod_tail[MODIFIER_BUF_SIZE]="";
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
	      
	      xnee_verbose((xd, "%d --- get_modifier 3: '%s'\n", level, tmp));
	      found=1;
	      strncpy (mod_head, tmp, i); 
	      tmp+=(sizeof(char)*i+1);

	      xnee_verbose((xd, "%d --- get_modifier 4: '%s'\n", level, mod_head));
	      strncpy (mod_tail, MODIFIER_BUF_SIZE - strlen(mod_tail), tmp); 

	      xnee_verbose((xd, "%d --- get_modifier 5: '%s'\n", level, mod_tail));
	      ret |= get_modifier_sub (xd, mod_head, km);

	      ret |= get_modifier (xd, mod_tail, km);
	      xnee_verbose((xd, "%d ---- get_modifier FOUND \"%s\"   ret=%d\n", level, tmp, ret));
	      break;
	    }
	}
    }
  else
    {
      ret |= get_modifier_sub (xd, tmp, km);
      xnee_verbose((xd, "%d ---- get_modifier LAST :\"%s\"  ret=%d\n", level, tmp, ret));
      
    }
 /*	  XKM_TAB(level);
	  xnee_verbose((xd, "\t %d head:\"%s\"   tail:\"%s\"\n", level, mod_head, mod_tail));
  */
  
  xnee_verbose((xd, "%d <--- get_modifier:\"%s\" --- ret=%d\n", level, mod_strs, ret));
  return ret;
}

#endif


/**************************************************************
 *                                                            *
 * get_modifier                                               *
 *                                                            *
 *                                                            *
 **************************************************************/
/* 
 *  OBSOLETE 
 *
 *
xnee_keymask
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

*/

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
	  XEvent my_event ;
	  int    tmp_code;
	  int    mode;
	  int i ;

	  XNextEvent (xd->grab, &my_event);
	  if (my_event.xkey.send_event==1) 
	    { 
	      xnee_verbose ((xd , "send_event==true\n")); 
	    }

	  if (my_event.type == KeyPress )
	    xnee_verbose ((xd , "##### KeyPress\n")); 
	  else if (my_event.type == KeyRelease )
	    xnee_verbose ((xd , "#### KeyRelease\n")); 

	  tmp_code=my_event.xkey.keycode;

	  xnee_verbose ((xd, "key = %d\n", tmp_code));
	  mode = xnee_get_grab_mode (xd, tmp_code);

	  xnee_verbose ((xd, "mode     = %d\n", mode));
	  
	  for (i=XNEE_GRAB_STOP;i<XNEE_GRAB_LAST;i++)
	    {
	      xnee_verbose((xd, "     compare %d == %d",tmp_code, xd->grab_keys->action_keys[i].key));
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
  int ret=XNEE_OK;
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
      char *exec_prog;
      xnee_verbose ((xd, " ---  xnee_handle_rec_km: EXEC \n"));
      feedback (xd, "Xnee exec received");
      exec_prog = xnee_get_exec_prog(xd);
      if (exec_prog==NULL)
	fprintf (xd->out_file, "%s   \n",XNEE_EXEC_MARK);
      else
	{
	  fprintf (xd->out_file, "%s    %s\n", XNEE_EXEC_MARK, exec_prog);
	  system ( exec_prog );
	}


      ret=XNEE_GRAB_EXEC;
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
      system ("xterm&");
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



