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


#include "libxnee/print_varargs.h"
#include "libxnee/print.h"
#include "libxnee/datastrings.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"
#include "libxnee/xnee_km.h"
#include "libxnee/xnee_setget.h"


/* internal prototypes */
int
get_modifier_sub(xnee_data *xd,  char *mod_str);


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

  xnee_verbose ((xd, " ---> is_last\n"));
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
		    SyncKeyboard,
		    CurrentTime);
      XFlush (xd->grab);
      

      if ( XCheckMaskEvent ( xd->grab, 0xffffffff , &ev) == True)
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

	  if (my_event.type == KeyPress )
	    xnee_verbose ((xd , "##### KeyPress\n")); 
	  else if (my_event.type == KeyRelease )
	    xnee_verbose ((xd , "#### KeyRelease\n")); 

	  tmp_modifier=my_event.xkey.state;
	  tmp_code=my_event.xkey.keycode;
	  xnee_verbose ((xd, "key      = %d\n", tmp_modifier));
	  xnee_verbose ((xd, "modifier = %d\n", tmp_code));
	  mode=xnee_get_grab_mode (xd, tmp_code, tmp_modifier);
	  xnee_verbose ((xd, "mode     = %d\n", mode));
	  
	  
	  xnee_verbose((xd, "\n\n\tUSER PUSHED MOD + KEY ... leaving\n\n\n"));
	  if ( ( tmp_modifier == xd->grab_keys->stop_mod ) &&
	       ( tmp_code == xd->grab_keys->stop_key ) )
	    {
	      xd->grab_keys->grabbed_action=XNEE_GRAB_STOP;
	    }
	  else if ( ( tmp_modifier == xd->grab_keys->pause_mod ) &&
	       ( tmp_code == xd->grab_keys->pause_key ) )
	    {
	      xd->grab_keys->grabbed_action=XNEE_GRAB_PAUSE;
	    }
	  else if ( ( tmp_modifier == xd->grab_keys->resume_mod ) &&
	       ( tmp_code == xd->grab_keys->resume_key ) )
	    {
	      xd->grab_keys->grabbed_action=XNEE_GRAB_RESUME;
	    }
	  else if ( ( tmp_modifier == xd->grab_keys->insert_mod ) &&
	       ( tmp_code == xd->grab_keys->insert_key ) )
	    {
	      xd->grab_keys->grabbed_action=XNEE_GRAB_INSERT;
	    }
	  else 
	    {
	      xd->grab_keys->grabbed_action=XNEE_GRAB_NODATA;
	    }
	  return XNEE_GRAB_DATA;
	}
      else
	{
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
xnee_handle_km(xnee_data *xd)
{
  int ret=XNEE_OK;
  xnee_verbose ((xd, " ---> xnee_handle_km\n"));

  if (xd->grab_keys->grabbed_action==XNEE_GRAB_STOP)
    {
      xnee_verbose ((xd, " ---  xnee_handle_km: STOP \n"));
      ret=XNEE_GRAB_STOP;
      xnee_verbose ((xd, " <--- xnee_handle_km\n"));
    }
  else if (xd->grab_keys->grabbed_action==XNEE_GRAB_PAUSE)
    {
      xnee_verbose ((xd, " ---  xnee_handle_km: PAUSE \n"));
      xnee_unsetup_recording (xd);
      ret=XNEE_GRAB_PAUSE;
    }
  else if (xd->grab_keys->grabbed_action==XNEE_GRAB_RESUME)
    {
      xnee_verbose ((xd, " ---  xnee_handle_km: RESUME \n"));
      xnee_setup_recording (xd);
      ret=XNEE_GRAB_RESUME;
    }
  else if (xd->grab_keys->grabbed_action==XNEE_GRAB_INSERT)
    {
      time_t rawtime;
      struct tm * timeinfo;

      xnee_verbose ((xd, " ---  xnee_handle_km: MARK \n"));
      time ( &rawtime );
      timeinfo = localtime ( &rawtime );
      fprintf (xd->out_file, "#XNEE INSERT MARK:%.4d-%.2d-%.2d:%.2d.%.2d.%.2d\n",
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
      xnee_verbose ((xd, " ---  xnee_handle_km: UNKNOWN \n"));
    }  

  xnee_verbose ((xd, " <--- xnee_handle_km\n"));
  return ret;
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
  else if ( sscanf ( mod_buf, "%d", &km->modifier) < 0 )
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





