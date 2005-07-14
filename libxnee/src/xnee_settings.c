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



#include "libxnee/xnee.h"




const int XNEE_MAX_DELAY = 3600;


const int XNEE_PRESS = True;
const int XNEE_RELEASE = False;


const int xnee_delay_timeout = 5000 ;
const int XNEE_DELAY = 10;
const int XNEE_MISSING_DATA_DELAY = (10*5);

const int XNEE_MAX_SYNCH = 100 /* buffer size */;
const int XNEE_MOTION_DELAY = 21 /* default value for delay during synch */;

const int MAX_NOT_IN_SYNC = 2 ;
const int MAX_OUT_OF_SYNC = 2 ;      /* number of data allowed to be out of sync */
const int MAX_UNSYNC_LOOPS = 10 ;    /* number check-loops when out of sync 
				      before exit  */
const int MAX_SKIPPED_UNSYNC = 10  ; /* number of times to ignore unsync state  */
const int XNEE_NOT_REPLAYABLE = 13 ;

const int XNEE_DEFAULT_MAX_THRESHOLD = 20;
const int XNEE_DEFAULT_MIN_THRESHOLD = 20;
const int XNEE_DEFAULT_TOT_THRESHOLD = 40;

const int XNEE_USEC_PER_SEC = 1000000;
const int XNEE_MSEC_PER_SEC = 1000;

const char XNEE_FROM_LAST_READ = 'l';
const char XNEE_FROM_FIRST_READ = 'f';

const int XNEE_TOO_FAST_ADJ_PERCENTAGE = 10;

const int XNEE_DELAY_RECORDING = 1000*500;

const int XNEE_RECEIVED = 1;
const int XNEE_REPLAYED = 2   ;

const int XNEE_TRUE = 1;
const int XNEE_FALSE = 0;

const int XNEE_GRAB_MODIFIER = 1;
const int XNEE_GRAB_KEY = 2;

const int XNEE_REPLAY_READ_META_DATA = 10;
const int XNEE_REPLAY_READ_REPLAY_DATA = 20;
const int XNEE_REPLAY_READ_ALL_DATA = 30;

const int REPLAY_CALLBACK = 1;
const int RECORD_CALLBACK = 2;
const int SYNC_CALLBACK = 3;

const int XNEE_NR_OF_GRABBED_MODIFIER = 8;
