/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003, 
 *                      2004, 2009  Henrik Sandklef 
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

#include "libxnee/xnee.h"
#include "libxnee/print_varargs.h"

const char*  XNEE_HOME_URL             = "http://www.gnu.org/software/xnee/";
const char*  XNEE_MAIL                 = "info-xnee@gnu.org";
const char*  XNEE_AUTHORS              = "Henrik Sandklef";
const char*  XNEE_BUG_MAIL             = "bug-xnee@gnu.org";
const char*  XNEE_DEVEL_MAIL           = "xnee-devel@gnu.org";
const char*  GNU_HOME_URL              = "http://www.gnu.org/";
const char*  FSF_HOME_URL              = "http://www.fsf.org/";
const char*  XNEE_PLUGIN_DIR           = "/usr/local/lib/xnee/plugins" ;
const char*  XNEE_RESOURCE_DIR         = "/usr/local/lib/xnee/resources" ;

const char*  XNEE_COMMENT_START_STR    = "#";
const char   XNEE_COMMENT_START_CHAR   = '#';

const char*  XNEE_EXEC_NO_PROG         = "xnee-exec-no-program";
const char*  XNEE_EXEC_MARK            = "EXEC";
const char*  XNEE_NEW_WINDOW_MARK      = "NEW-WINDOW";

const char*  XNEE_META_END	       = "META-END"  /* Marks the end of the META DATA in the recorded file*/ ;

const char*  XNEE_REPLAY_CALLBACK_NAME = "xnee_replay_dispatch";
const char*  XNEE_RECORD_CALLBACK_NAME = "xnee_record_dispatch";
const char*  XNEE_SYNC_FUNCTION_NAME   = "xnee_sync_fun";

const char*  EXT_REPLAY_CALLBACK_NAME = "xnee_replay_dispatch_external";
const char*  EXT_RECORD_CALLBACK_NAME = "xnee_record_dispatch_external";
const char*  EXT_SYNC_FUNCTION_NAME   = "xnee_sync_fun_external";

const char*  XNEE_FAKE_MOTION          = "fake-motion";
const char*  XNEE_FAKE_X_ARG           = "x";
const char*  XNEE_FAKE_Y_ARG           = "y";

const char*  XNEE_FAKE_BUTTON_PRESS    = "fake-button-press";
const char*  XNEE_FAKE_BUTTON_RELEASE  = "fake-button-release";
const char*  XNEE_FAKE_BUTTON          = "fake-button";
const char*  XNEE_FAKE_BUTTON_ARG      = "button";

const char*  XNEE_FAKE_KEY_PRESS       = "fake-key-press";
const char*  XNEE_FAKE_KEY_RELEASE     = "fake-key-release";
const char*  XNEE_FAKE_KEY             = "fake-key";
const char*  XNEE_FAKE_KEY_ARG         = "key";

const char*  XNEE_FAKE_MSEC_SLEEP      = "msec";

const char*  XNEE_TRUE_STRING          = "true";
const char*  XNEE_1_STRING             = "1";
const char*  XNEE_FALSE_STRING         = "false";
const char*  XNEE_0_STRING             = "0";

const char*  XNEE_XK_ALT_L             = "Alt_L";
const char*  XNEE_XK_ALT_R             = "Alt_R";
const char*  XNEE_XK_RETURN            = "XK_Return";
const char*  XNEE_XK_SPACE             = "XK_space";
const char*  XNEE_XK_DOWN              = "XK_Down";
const char*  XNEE_XK_UP                = "XK_Up";
const char*  XNEE_XK_LEFT              = "XK_Left";
const char*  XNEE_XK_RIGHT             = "XK_Right";



const char*  XNEE_SYMBOLIC_ALT      = "Alt";
const char*  XNEE_SYMBOLIC_CONTROL  = "Control";
const char*  XNEE_SYMBOLIC_SCROLL   = "Scroll";
const char*  XNEE_SYMBOLIC_CAPS     = "Caps";
const char*  XNEE_SYMBOLIC_SHIFT    = "Shift";

const char*  XNEE_REAL_ALT          = "Alt_L";
const char*  XNEE_REAL_CONTROL      = "Control_L";
const char*  XNEE_REAL_SCROLL       = "Scroll_L";
const char*  XNEE_REAL_CAPS         = "Caps_Lock";
const char*  XNEE_REAL_SHIFT        = "Shift_L";

const char* NULL_STRING             = "NULL";

const char* XNEE_EMPTY_STRING       = "" ;

