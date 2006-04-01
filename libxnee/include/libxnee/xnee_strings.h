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

#ifndef XNEE_XNEE_STRINGS_H
#define XNEE_XNEE_STRINGS_H

#include "libxnee/xnee.h"

extern const char*  XNEE_EMPTY_STRING         ;

extern const char*  XNEE_COMMENT_START_STR    ;
extern const char   XNEE_COMMENT_START_CHAR   ;

extern const char*  XNEE_HOME_URL             ;
extern const char*  XNEE_MAIL                 ;
extern const char*  XNEE_AUTHORS              ;
extern const char*  XNEE_BUG_MAIL             ;
extern const char*  XNEE_DEVEL_MAIL           ;
extern const char*  GNU_HOME_URL              ;
extern const char*  FSF_HOME_URL              ;
extern const char*  XNEE_PLUGIN_DIR           ;
extern const char*  XNEE_RESOURCE_DIR         ;


extern const char*  XNEE_ERR_FILE             ;
extern const char*  XNEE_OUT_FILE             ;

extern const char*  XNEE_DISPLAY              ;
extern const char*  XNEE_FIRST_LAST           ;
extern const char*  XNEE_ALL_EVENTS           ;
extern const char*  XNEE_ALL_CLIENTS          ;
extern const char*  XNEE_FUTURE_CLIENTS       ;
extern const char*  XNEE_DIMENSION            ;
extern const char*  XNEE_EVENT_MAX            ;
extern const char*  XNEE_DATA_MAX             ;
extern const char*  XNEE_TIME_MAX             ;
extern const char*  XNEE_LOOPS_LEFT           ;
extern const char*  XNEE_STOP_KEY             ;
extern const char*  XNEE_PAUSE_KEY            ;
extern const char*  XNEE_RESUME_KEY           ;
extern const char*  XNEE_INSERT_KEY           ;
extern const char*  XNEE_EXEC_KEY             ;
extern const char*  XNEE_EXEC_PROGRAM         ;
extern const char*  XNEE_EXEC_NO_PROG         ;
extern const char*  XNEE_EXEC_MARK            ;
extern const char*  XNEE_NEW_WINDOW_MARK      ;
extern const char*  XNEE_NEW_WINDOW           ;
extern const char*  XNEE_EVERYTHING           ;
extern const char*  XNEE_DELAY_TIME           ;
extern const char*  XNEE_SPEED_PERCENT        ;
extern const char*  XNEE_RECORDED_RESOLUTION  ;
extern const char*  XNEE_REPLAY_RESOLUTION    ;
extern const char*  XNEE_ADJUST_RESOLUTION    ;
extern const char*  XNEE_DISTRIBUTE           ;
extern const char*  XNEE_NO_EXPOSE            ;
extern const char*  XNEE_NO_SYNC_MODE         ;
extern const char*  XNEE_REPLAY_OFFSET        ;
extern const char*  XNEE_USE_SYNC             ;
extern const char*  XNEE_SYNC_MODE            ;
extern const char*  XNEE_RECALL_WINDOW_POS    ;
extern const char*  XNEE_HUMAN_PRINTOUT       ;
extern const char*  XNEE_LOOPS                ;
extern const char*  XNEE_FORCE_REPLAY         ;
extern const char*  XNEE_RESOURCE             ;
extern const char*  XNEE_PLUGIN               ;
extern const char*  XNEE_ERROR_FD             ;
extern const char*  XNEE_VERBOSE              ;
extern const char*  XNEE_BUFFER_VERBOSE       ;
extern const char*  XNEE_MOUSE                ;
extern const char*  XNEE_KEYBOARD             ;
extern const char*  XNEE_STORE_MOUSE_POS      ;
extern const char*  XNEE_FEEDBACK_XOSD        ;
extern const char*  XNEE_XOSD_FONT            ;
extern const char*  XNEE_FEEDBACK_STDERR      ;
extern const char*  XNEE_FEEDBACK_NONE        ;

extern const char*  XNEE_REQUEST_STR          ;
extern const char*  XNEE_REPLIES_STR          ;
extern const char*  XNEE_DEVICE_EVENT_STR     ;
extern const char*  XNEE_DELIVERED_EVENT_STR  ;
extern const char*  XNEE_ERROR_STR            ;
extern const char*  XNEE_REPLY_STR            ;
extern const char*  XNEE_EXT_REQ_MAJ_STR      ;
extern const char*  XNEE_EXT_REQ_MIN_STR      ;
extern const char*  XNEE_EXT_REP_MAJ_STR      ;
extern const char*  XNEE_EXT_REP_MIN_STR      ;
extern const char*  XNEE_META_END             ;

extern const char*  XNEE_MAX_THRESHOLD        ;
extern const char*  XNEE_MIN_THRESHOLD        ;
extern const char*  XNEE_TOT_THRESHOLD        ;

extern const char*  XNEE_REPLAY_CALLBACK_NAME ;
extern const char*  XNEE_RECORD_CALLBACK_NAME ;
extern const char*  XNEE_SYNC_FUNCTION_NAME   ;


extern const char*  XNEE_FAKE_MOTION          ;
extern const char*  XNEE_FAKE_X_ARG           ;
extern const char*  XNEE_FAKE_Y_ARG           ;

extern const char*  XNEE_FAKE_BUTTON_PRESS    ;
extern const char*  XNEE_FAKE_BUTTON_RELEASE  ;
extern const char*  XNEE_FAKE_BUTTON          ;
extern const char*  XNEE_FAKE_BUTTON_ARG      ;

extern const char*  XNEE_FAKE_KEY_PRESS       ;
extern const char*  XNEE_FAKE_KEY_RELEASE     ;
extern const char*  XNEE_FAKE_KEY             ;
extern const char*  XNEE_FAKE_KEY_ARG         ;

extern const char*  XNEE_FAKE_MSEC_SLEEP      ;

extern const char*  XNEE_TRUE_STRING          ;
extern const char*  XNEE_1_STRING             ;
extern const char*  XNEE_FALSE_STRING         ;
extern const char*  XNEE_0_STRING             ;

extern const char*  XNEE_XK_ALT_L             ;
extern const char*  XNEE_XK_ALT_R             ;
extern const char*  XNEE_XK_RETURN            ;
extern const char*  XNEE_XK_SPACE             ;

extern const char*  XNEE_XK_UP                ;
extern const char*  XNEE_XK_DOWN              ;
extern const char*  XNEE_XK_LEFT              ;
extern const char*  XNEE_XK_RIGHT             ;





extern const char*  XNEE_SYMBOLIC_ALT     ;
extern const char*  XNEE_SYMBOLIC_CONTROL ;
extern const char*  XNEE_SYMBOLIC_SCROLL  ;
extern const char*  XNEE_SYMBOLIC_CAPS    ;
extern const char*  XNEE_SYMBOLIC_SHIFT   ;

extern const char*  XNEE_REAL_ALT         ;
extern const char*  XNEE_REAL_CONTROL     ;
extern const char*  XNEE_REAL_SCROLL      ;
extern const char*  XNEE_REAL_CAPS        ;
extern const char*  XNEE_REAL_SHIFT       ;

extern const char*  NULL_STRING           ;

#define EMPTY_IF_NULL(a) (a?a:XNEE_EMPTY_STRING)

#endif /* XNEE_XNEE_STRINGS_H */
