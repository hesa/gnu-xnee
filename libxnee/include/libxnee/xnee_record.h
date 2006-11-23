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



#ifndef XNEE_RECORD_H
#define XNEE_RECORD_H


/* Standard includes */
#include <stdio.h>


/* 
 * Xnee includes 
 */
#include "libxnee/xnee.h"







/**
 * Prints X events data according to the Xnee protocol\/format
 * prints all events to the logfile as specified in in 
 * the xnee_data struct.
 * If the event is \"replayable\" we call fakeXX functions
 * <br><br>By all events we mean all but Expose/NoExpose 
 * ... there are a lot of them
 *
 * @param xd        xnee's main structure
 * @param xrecintd  pointer to data as sent by the Xserver
 * @return void  
 */
int
xnee_record_handle_event ( xnee_data *xd, /*@null@*/ XRecordInterceptData *xrecintd);




/**
 * RECORD callback procedure for  Xnee
 * @param xpoint_xnee_data   pointer to xnee's main structure
 * @param data               pointer to data as sent by the Xserver
 * @return void              
 *
 */
void 
xnee_record_dispatch(XPointer xpoint_xnee_data,
		     XRecordInterceptData *data );

/**
 * RECORD callback procedure for Xnee 
 * This callback calls function that prints in human format
 * 
 * @param xpoint_xnee_data   pointer to xnee's main structure
 * @param data               pointer to data as sent by the Xserver
 * @return void  
 */
void 
xnee_human_dispatch(XPointer xpoint_xnee_data,
		    XRecordInterceptData *data );



/**
 * Inits the structures used when recording.
 * @param xd     xnee's main structure
 * @return void  
 */
void 
xnee_record_init              (xnee_data* xd) ; 





/**
 * Sets up Record extension with some nice values
 * @param xd   xnee's main structure
 * @return int 0 on success. XNEE_NO_PROT_CHOOSEN if no data was choosen to record.
 */
int
xnee_setup_recordext (xnee_data* xd);

/**
 * Closes down Record extension. After this a call to xnee_setup_recordext is possible
 * to restart recording.
 * @param xd   xnee's main structure
 * @return int 0 on success. XNEE_NO_PROT_CHOOSEN if no data was choosen to record.
 */
int
xnee_unsetup_recordext (xnee_data* xd);



/**
 * Tests if Record extension is readu to be used
 * @param xd    xnee's main structure
 * @return int  1 if present, o if not
 * \todo (return values need to be changed) 
 */
int 
xnee_has_record_extension(xnee_data *xd);




/**
 * Given a xnee_data struct this function sets up
 * all data and sets the dispatch method to Record extension.
 *
 * @param xd    xnee's main structure
 * @return int  0 in success
 */
int
xnee_setup_recording(xnee_data *xd);


/** 
 * 
 * 
 * @param xd xnee's main structure
 * 
 * @return   0 if succuessful
 */
int
xnee_unsetup_recording(xnee_data *xd);

/**
 * Waits for data from X server 
 * When data is received the dispatch 
 * method is called
 * 
 *
 * @param xd    xnee's main structure
 * @return int  0 on success
 */
int 
xnee_record_loop(xnee_data *xd);


/** 
 * 
 * 
 * @param xd xnee's main structure
 * 
 * @return   0 on succeess
 */
int 
xnee_record_async(xnee_data *xd);





#endif   /* XNEE_RECORD_H */

