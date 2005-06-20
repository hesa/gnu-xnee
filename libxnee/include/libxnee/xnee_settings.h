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
 * Foundation, Inc., 51 Franklin Street, Boston,            
 * MA  02110-1301, USA.                                              
 ****/


#ifndef XNEE_XNEE_SETTINGS_H
#define XNEE_XNEE_SETTINGS_H


#define SYNCHRONIZATION_EVENT (CreateNotify | MapRequest | ConfigureRequest | DestroyNotify | MapNotify | ConfigureNotify | UnmapNotify )

#define XNEE_NR_OF_MODIFIERS    8

#define XNEE_HIGHEST_DATA_NR    256
#define XNEE_REPLAY_BUFFER_SIZE XNEE_HIGHEST_DATA_NR 
#define XNEE_SAVED_LINES_BYTES  1024
#define XNEE_RANGE_STRING_SIZE  512 
#define DATA_NAME_SIZE_MAX      32



extern const int XNEE_MAX_DELAY ;

extern const int XNEE_PRESS ;
extern const int XNEE_RELEASE ;


extern const int xnee_delay_timeout ;
extern const int XNEE_DELAY ;
extern const int XNEE_MISSING_DATA_DELAY ;

extern const int XNEE_MAX_SYNCH ;
extern const int XNEE_MOTION_DELAY ;

extern const int XNEE_BUFFER_MAX     ;
extern const int XNEE_BUFFER_MIN     ;
extern const int XNEE_BUFFER_SUM_MAX ;
extern const int XNEE_BUFFER_SUM_MIN ;
extern const int XNEE_BUFFER_TOT_MAX ;

extern const int MAX_NOT_IN_SYNC ;
extern const int MAX_OUT_OF_SYNC ;      /* number of data allowed to be out of sync */
extern const int MAX_UNSYNC_LOOPS ;    /* number check-loops when out of sync 
				      before exit  */

extern const int MAX_SKIPPED_UNSYNC ; /* number of times to ignore unsync state  */
extern const int XNEE_NOT_REPLAYABLE ;


extern const int XNEE_DEFAULT_MAX_THRESHOLD ;
extern const int XNEE_DEFAULT_MIN_THRESHOLD ;
extern const int XNEE_DEFAULT_TOT_THRESHOLD ;

extern const int XNEE_USEC_PER_SEC ;
extern const int XNEE_MSEC_PER_SEC ;

extern const char XNEE_FROM_LAST_READ ;
extern const char XNEE_FROM_FIRST_READ ;

extern const int XNEE_TOO_FAST_ADJ_PERCENTAGE	     ;

extern const int XNEE_DELAY_RECORDING ;

extern const int XNEE_RECEIVED ;
extern const int XNEE_REPLAYED ;

extern const int XNEE_TRUE ;
extern const int XNEE_FALSE ;

extern const int XNEE_GRAB_MODIFIER ;
extern const int XNEE_GRAB_KEY ;

extern const int XNEE_REPLAY_READ_META_DATA ;
extern const int XNEE_REPLAY_READ_REPLAY_DATA ;
extern const int XNEE_REPLAY_READ_ALL_DATA ;

extern const int REPLAY_CALLBACK ;
extern const int RECORD_CALLBACK ;
extern const int SYNC_CALLBACK ;

extern const int XNEE_NR_OF_GRABBED_MODIFIER ;


#endif /*  XNEE_XNEE_SETTINGS_H */

