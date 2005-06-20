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
 * Foundation, Inc., 51 Franklin Street, Boston,            
 * MA  02110-1301, USA.                                              
 ****/

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <X11/Xutil.h>


#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/xnee_keysym.h"
#include "libxnee/xnee_km.h"

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



int
xnee_token_to_km (xnee_data *xd,
		  int keycode,
		  const char *str,
		  xnee_key_code *kc);
KeyCode
xnee_keysym2keycode(xnee_data* xd, KeySym ks, char * str, xnee_key_code *kc);


KeyCode
xnee_str2keycode(xnee_data* xd, const char *str, xnee_key_code *kc)
{
  const char *tmp;
  if (xd->fake==NULL)
    return -1;

  tmp = xnee_symbolic_modifier2modifier(xd, str);
  if (tmp!=NULL)
    {
       str=tmp;
    }


  /* Xnee special syntax */
  if (strncmp(str, XNEE_XK_SPACE, strlen(XNEE_XK_SPACE))==0)
    {
      kc->kc = XKeysymToKeycode(xd->fake, XK_space);
    }
  else if (strncmp(str, XNEE_XK_RETURN, strlen(XNEE_XK_RETURN))==0)
    {
      kc->kc = XKeysymToKeycode(xd->fake, XK_Return);
    }
  else if (strncmp(str, XNEE_XK_ALT_L, strlen(XNEE_XK_ALT_L))==0)
    {
      kc->kc = XKeysymToKeycode(xd->fake, XK_Alt_L);
    }
  else if (strncmp(str, XNEE_XK_UP, strlen(XNEE_XK_UP))==0)
    {
      kc->kc = XKeysymToKeycode(xd->fake, XK_Up);
    }
  else if (strncmp(str, XNEE_XK_DOWN, strlen(XNEE_XK_DOWN))==0)
    {
      kc->kc = XKeysymToKeycode(xd->fake, XK_Down);
    }
  else if (strncmp(str, XNEE_XK_LEFT, strlen(XNEE_XK_LEFT))==0)
    {
      kc->kc = XKeysymToKeycode(xd->fake, XK_Left);
    }
  else if (strncmp(str, XNEE_XK_RIGHT, strlen(XNEE_XK_RIGHT))==0)
    {
      kc->kc = XKeysymToKeycode(xd->fake, XK_Right);
    }
  else  
    {
	printf ("keycode...%s\n", str);
      kc->kc = XKeysymToKeycode(xd->fake,XStringToKeysym(str));
      printf (" ..... kc=%d\n", kc->kc);
    }

  if (kc!=NULL)
  {
     xnee_token_to_km (xd, kc->kc, str, kc);
  }

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
		  const char *str,
		  xnee_key_code *kc)
{
  #define TOKEN_STRING_SIZE 20

  XEvent event;
  int size;
  int i ;
  int k ;
  int ret=-1;
  char string[TOKEN_STRING_SIZE];
  KeySym keysym;
  Display *dpy = xd->fake;

  xnee_verbose((xd, "Finding km for %s\n", str));

  memset (kc->mod_keycodes, 0, XNEE_NR_OF_MODIFIERS * sizeof(KeyCode));

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

      if (strncmp(str,string, TOKEN_STRING_SIZE)==0)
	{
	  KeySym ks ;
	  char *nm ;
	  xnee_verbose((xd, "  i=%d\n", i ));
	  xnee_verbose((xd, "  xd=%d\n", (int)xd ));
	  xnee_verbose((xd, "  map=%d\n", (int)xd->map ));
	  xnee_verbose((xd, "  max_keypermod=%d\n" ,xd->map->max_keypermod ));

	  k = (i-1)*xd->map->max_keypermod ;
	  xnee_verbose((xd, "  k=%d\n", k ));
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
  KeySym ks = 0 ;
  
  if (xd->fake==NULL)
    {
      return -1;
    }

  memset(kc,0,sizeof(xnee_key_code));
  buf[0]=token;
  buf[1]=0;

  switch( token)
    {
      /* XK_0 --- XK_9 is skipped and solved elsewhere */
      /* XK_A --- XK_Z is skipped and solved elsewhere */
      /* XK_a --- XK_z is skipped and solved elsewhere ... below */
    case ' ':
      ks=XK_space;
      break;
    case '!':
      ks=XK_exclam;
      break;
/*     case '!': */
/*       ks=XK_exclamdown; */
/*       break; */
    case '\"':
      ks=XK_quotedbl;
      break;
    case '#':
      ks=XK_numbersign;
      break;
    case '$':
      ks=XK_dollar;
      break;
    case '%':
      ks=XK_percent;
      break;
    case '&':
      ks=XK_ampersand;
      break;
    case '(':
      ks=XK_parenleft;
      break;
    case ')':
      ks=XK_parenright;
      break;
    case '+':
      ks=XK_plus;
      break;
    case ',':
      ks=XK_comma;
      break;
    case '-':
      ks=XK_minus;
      break;
    case '.':
      ks=XK_period;
      break;
    case '/':
      ks=XK_slash;
      break;
    case ':':
      ks=XK_colon;
      break;
    case ';':
      ks=XK_semicolon;
      break;
    case '<':
      ks=XK_less;
      break;
    case '=':
      ks=XK_equal;
      break;
    case '>':
      ks=XK_greater;
      break;
    case '?':
      ks=XK_question;
      break;
    case '@':
      ks=XK_at;
      break;
    case '*':
      ks=XK_asterisk;
      break;
    case '\\':
      ks=XK_backslash;
      break;
    case '[':
      ks=XK_bracketleft;
      break;
    case ']':
      ks=XK_bracketright;
      break;
    case '^':
      ks=XK_asciicircum;
      break;
    case '_':
      ks=XK_underscore;
      break;
    case '`':
      ks=XK_grave;
      break;
    case '´':
      ks=XK_quoteleft;
      break;
    case '\'':
       ks=XK_quoteright;
      break;
    case 27:
      ks=XK_apostrophe;
      break;
    case '{':
      ks=XK_braceleft;
      break;
    case '}':
      ks=XK_braceright;
      break;
    case '|':
      ks=XK_bar;
      break;
    case '\n':
      ks=XK_Return;
      break;
    case '\t':
       ks=XK_Tab;
       break;
    case '~':
      ks=XK_asciitilde;
      break;
    default:
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
    
  return XNEE_OK;
}



#ifdef KEYSYMS_TO_USE_LATER
    case -9:
      ks=XK_nobreakspace;
      break;
    case -11:
      /* FIX ME */
      ks=XK_cent;
      break;
    case -12:
      /* FIX ME */
      ks=XK_sterling;
      break;
    case -13:
      /* FIX ME */
      ks=XK_currency;
      break;
       

    case -14:
      /* FIX ME */
      ks=XK_yen;
      break;
    case -15:
      /* FIX ME */
      ks=XK_brokenbar;
      break;
    case -16:
      /* FIX ME */
      ks=XK_section;
      break;
    case -17:
      /* FIX ME */
      ks=XK_diaeresis;
      break;
    case -18:
      /* FIX ME */
      ks=XK_copyright;
      break;
    case -19:
      /* FIX ME */
      ks=XK_ordfeminine;
      break;
    case -20:
      /* FIX ME */
      ks=XK_guillemotleft;
      break;
    case -21:
      /* FIX ME */
      ks=XK_notsign;
      break;

    case 'p':
      /* FIX ME */
      ks=XK_hyphen;
      break;


    case -23:
      /* FIX ME */
      ks=XK_registered;
      break;
    case -24:
      /* FIX ME */
      ks=XK_macron;
      break;
    case -25:
      /* FIX ME */
      ks=XK_degree;
      break;
    case -26:
      /* FIX ME */
      ks=XK_plusminus;
      break;
    case -27:
      /* FIX ME */
      ks=XK_twosuperior;
      break;
    case -28:
      /* FIX ME */
      ks=XK_threesuperior;
      break;
    case -29:
      ks=XK_acute;
      break;
    case -30:
      /* FIX ME */
      ks=XK_mu;
      break;
    case -31:
      /* FIX ME */
      ks=XK_paragraph;
      break;
    case -32:
      /* FIX ME */
      ks=XK_periodcentered;
      break;
    case -33:
      /* FIX ME */
      ks=XK_cedilla;
      break;
    case -34:
      /* FIX ME */
      ks=XK_onesuperior;
      break;
    case -35:
      /* FIX ME */
      ks=XK_masculine;
      break;
    case -37:
      /* FIX ME */
      ks=XK_onequarter;
      break;
    case -38:
      /* FIX ME */
      ks=XK_onehalf;
      break;
    case -39:
      /* FIX ME */
      ks=XK_threequarters;
      break;
    case -40:
      /* FIX ME */
      ks=XK_questiondown;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Agrave;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Aacute;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Acircumflex;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Atilde;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Adiaeresis;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Aring;
      break;
    case 0:
      /* FIX ME */
      ks=XK_AE;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Ccedilla;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Egrave;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Eacute;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Ecircumflex;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Ediaeresis;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Igrave;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Iacute;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Icircumflex;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Idiaeresis;
      break;
    case 0:
      /* FIX ME */
      ks=XK_ETH;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Eth;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Ntilde;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Ograve;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Oacute;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Ocircumflex;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Otilde;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Odiaeresis;
      break;
    case 0:
      /* FIX ME */
      ks=XK_multiply;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Ooblique;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Oslash;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Ugrave;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Uacute;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Ucircumflex;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Udiaeresis;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Yacute;
      break;
    case 0:
      /* FIX ME */
      ks=XK_THORN;
      break;
    case 0:
      /* FIX ME */
      ks=XK_Thorn;
      break;
    case 0:
      /* FIX ME */
      ks=XK_ssharp;
      break;
    case 0:
      /* FIX ME */
      ks=XK_agrave;
      break;
    case 0:
      /* FIX ME */
      ks=XK_aacute;
      break;
    case 0:
      /* FIX ME */
      ks=XK_acircumflex;
      break;
    case 0:
      /* FIX ME */
      ks=XK_atilde;
      break;
    case 0:
      /* FIX ME */
      ks=XK_adiaeresis;
      break;
    case 0:
      /* FIX ME */
      ks=XK_aring;
      break;
    case 0:
      /* FIX ME */
      ks=XK_ae;
      break;
    case 0:
      /* FIX ME */
      ks=XK_ccedilla;
      break;
    case 0:
      /* FIX ME */
      ks=XK_egrave;
      break;
    case 0:
      /* FIX ME */
      ks=XK_eacute;
      break;
    case 0:
      /* FIX ME */
      ks=XK_ecircumflex;
      break;
    case 0:
      /* FIX ME */
      ks=XK_ediaeresis;
      break;
    case 0:
      /* FIX ME */
      ks=XK_igrave;
      break;
    case 0:
      /* FIX ME */
      ks=XK_iacute;
      break;
    case 0:
      /* FIX ME */
      ks=XK_icircumflex;
      break;
    case 0:
      /* FIX ME */
      ks=XK_idiaeresis;
      break;
    case 0:
      /* FIX ME */
      ks=XK_eth;
      break;
    case 0:
      /* FIX ME */
      ks=XK_ntilde;
      break;
    case 0:
      /* FIX ME */
      ks=XK_ograve;
      break;
    case 0:
      /* FIX ME */
      ks=XK_oacute;
      break;
    case 0:
      /* FIX ME */
      ks=XK_ocircumflex;
      break;
    case 0:
      /* FIX ME */
      ks=XK_otilde;
      break;
    case 0:
      /* FIX ME */
      ks=XK_odiaeresis;
      break;
    case 0:
      /* FIX ME */
      ks=XK_division;
      break;
    case 0:
      /* FIX ME */
      ks=XK_oslash;
      break;
    case 0:
      /* FIX ME */
      ks=XK_ooblique;
      break;
    case 0:
      /* FIX ME */
      ks=XK_ugrave;
      break;
    case 0:
      /* FIX ME */
      ks=XK_uacute;
      break;
    case 0:
      /* FIX ME */
      ks=XK_ucircumflex;
      break;
    case 0:
      /* FIX ME */
      ks=XK_udiaeresis;
      break;
    case 0:
      /* FIX ME */
      ks=XK_yacute;
      break;
    case 0:
      /* FIX ME */
      ks=XK_thorn;
      break;
    case 0:
      /* FIX ME */
      ks=XK_ydiaeresis;
      break;
    case 0:
      /* FIX ME */
      ks=XK_guillemotright;
      break;


#endif
