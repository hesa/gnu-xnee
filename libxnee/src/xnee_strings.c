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

const char*  XNEE_HOME_URL             = "http://www.gnu.org/software/xnee/";
const char*  XNEE_MAIL                 = "info-xnee@gnu.org";
const char*  XNEE_BUG_MAIL             = "bug-xnee@gnu.org";
const char*  XNEE_DEVEL_MAIL           = "xnee-devel@gnu.org";
const char*  GNU_HOME_URL              = "http://www.gnu.org/";
const char*  FSF_HOME_URL              = "http://www.fsf.org/";
const char*  XNEE_PLUGIN_DIR           = "/usr/local/lib/xnee/plugins" ;
const char*  XNEE_RESOURCE_DIR         = "/usr/local/lib/xnee/resources" ;

const char*  XNEE_COMMENT_START_STR    = "#";
const char   XNEE_COMMENT_START_CHAR   = '#';

const char*  XNEE_ERR_FILE             = "err-file";
const char*  XNEE_OUT_FILE             = "out-file";

const char*  XNEE_DISPLAY              = "display";
const char*  XNEE_FIRST_LAST           = "first-last";
const char*  XNEE_ALL_EVENTS           = "all-events";
const char*  XNEE_ALL_CLIENTS          = "all-clients";
const char*  XNEE_FUTURE_CLIENTS       = "future-clients";
const char*  XNEE_DIMENSION            = "Dimension";
const char*  XNEE_EVENT_MAX            = "events-to-record";
const char*  XNEE_DATA_MAX             = "data-to-record";
const char*  XNEE_TIME_MAX             = "seconds-to-record";
const char*  XNEE_LOOPS_LEFT           = "loops-left";
const char*  XNEE_STOP_KEY             = "stop-key";
const char*  XNEE_PAUSE_KEY            = "pause-key";
const char*  XNEE_RESUME_KEY           = "resume-key";
const char*  XNEE_INSERT_KEY           = "insert-key";
const char*  XNEE_EXEC_KEY             = "exec-key";
const char*  XNEE_EXEC_PROGRAM         = "exec-program";
const char*  XNEE_EXEC_NO_PROG         = "xnee-exec-no-program";
const char*  XNEE_EXEC_MARK            = "EXEC";
const char*  XNEE_EVERYTHING           = "everything";
const char*  XNEE_DELAY_TIME           = "time";
const char*  XNEE_SPEED_PERCENT        = "speed-percent";
const char*  XNEE_RECORDED_RESOLUTION  = "recorded-resolution";
const char*  XNEE_REPLAY_RESOLUTION    = "replay-resolution";
const char*  XNEE_ADJUST_RESOLUTION    = "resolution-adjustment";
const char*  XNEE_DISTRIBUTE           = "distribute";
const char*  XNEE_NO_EXPOSE            = "no-expose";
const char*  XNEE_NO_SYNC_MODE         = "no-sync";
const char*  XNEE_USE_SYNC             = "sync";
const char*  XNEE_SYNC_MODE            = "synchronised-replay";
const char*  XNEE_HUMAN_PRINTOUT       = "human-printout";
const char*  XNEE_LOOPS                = "loops";
const char*  XNEE_FORCE_REPLAY         = "force-replay";
const char*  XNEE_RESOURCE             = "resource";
const char*  XNEE_PLUGIN               = "plugin";
const char*  XNEE_ERROR_FD             = "stderr";
const char*  XNEE_VERBOSE              = "verbose";
const char*  XNEE_BUFFER_VERBOSE       = "buffer-verbose";
const char*  XNEE_MOUSE                = "mouse";
const char*  XNEE_KEYBOARD             = "keyboard";
const char*  XNEE_STORE_MOUSE_POS      = "store-mouse-position";
const char*  XNEE_FEEDBACK_XOSD        = "feedback-xosd";
const char*  XNEE_FEEDBACK_STDERR      = "feedback-stderr";
const char*  XNEE_FEEDBACK_NONE        = "feedback-none";

const char*  XNEE_REQUEST_STR          = "request-range";
const char*  XNEE_REPLIES_STR          = "reply-range";
const char*  XNEE_DEVICE_EVENT_STR     = "device-event-range";
const char*  XNEE_DELIVERED_EVENT_STR  = "delivered-event-range";
const char*  XNEE_ERROR_STR            = "error-range";
const char*  XNEE_REPLY_STR            = "reply-range";
const char*  XNEE_EXT_REQ_MAJ_STR      = "extension-request-major-range";
const char*  XNEE_EXT_REQ_MIN_STR      = "extension-request-minor-range";
const char*  XNEE_EXT_REP_MAJ_STR      = "extension-reply-major-range";
const char*  XNEE_EXT_REP_MIN_STR      = "extension-reply-minor-range";
const char*  XNEE_META_END	       = "META-END"  /* Marks the end of the META DATA in the recorded file*/ ;

const char*  XNEE_MAX_THRESHOLD        = "max-threshold";
const char*  XNEE_MIN_THRESHOLD        = "min-threshold";
const char*  XNEE_TOT_THRESHOLD        = "tot-threshold";

const char*  XNEE_REPLAY_CALLBACK_NAME = "xnee_replay_dispatch";
const char*  XNEE_RECORD_CALLBACK_NAME = "xnee_record_dispatch";
const char*  XNEE_SYNC_FUNCTION_NAME   = "xnee_sync_fun";


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
