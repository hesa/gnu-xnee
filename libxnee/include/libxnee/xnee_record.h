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



#ifndef XNEE_RECORD_H
#define XNEE_RECORD_H


#include "xnee.h"




/* Standard includes */
#include <stdio.h>

/* X11 includes */
#include <X11/Xlib.h>
#include <X11/Xmd.h>
#include <X11/Xproto.h>
#include <X11/Xos.h>
#include <X11/Xlibint.h>  /* Warning, there be dragons here.... */

/* 
 *  X11 extensions includes 
 */
#include <X11/extensions/record.h> 

/* 
 * Xnee includes 
 */
#include "xnee.h"





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
void
xnee_record_handle_event ( xnee_data *xd, XRecordInterceptData *xrecintd);




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
 * Selects some default ranges of data to record
 *
 * @param xd     xnee's main structure
 * @return void  
 */
void 
xnee_record_select_default_protocol   (xnee_data *xd) ;




/**
 * Adds a range of data to record. 
 * <br><br>
 * Example range string: 1,2-4,6-9,12,10<br>
 * Calls xnee_add_range to do the job of allocating
 *
 * @param xd         xnee's main structure
 * @param data_type  type of X11 data (event, request ...) 
 * @param range_str  comma spearated integer or integer "-" integer
 * @return int       returns 0 in success
 */
int
xnee_add_range_str (xnee_data *xd, int data_type, char *range_str);




/**
 * Add X11 data to record. Allocates a Record structure if needed.
 * <br><br>
 * YOU HAVE TO DEALLOC THE RANGES ALLOCATED BY THIS FUNCIOTN
 * or you can use xnee_free_xnee_data
 *
 * @param xd     xnee's main structure
 * @param type   type of protocol data (i.e event, request, reply or error) 
 * @param first  first value of range (or only value)  
 * @param end    end value of range (if not 0)
 * @return int   returns 0 in success
 */
int
xnee_add_range (xnee_data *xd, int type , int first, int end);




/**
 * Parses a range string  and call.
 * Calls xnee_add_range_str to do the job of allocating
 * <br><br>
 * Example: xnee_parse_range (xd, XNEE_EVENT, "1-4,7,13");
 * <br><br>
 * NOTE:  YOU HAVE TO DEALLOC THE RANGES ALLOCATED BY THIS FUNCIOTN
 * or you can use xnee_free_xnee_data
 *
 * @param xd     xnee's main structure
 * @param type   type of protocol data (i.e event, request, reply or error) 
 * @param range  string with ranges
 * @return int   0 on success
 */
int
xnee_parse_range (xnee_data *xd, int type , char *range);



/**
 * Sets up Record extension with some nice values
 * @param xd   xnee's main structure
 * @return int 0 on success. XNEE_NO_PROT_CHOOSEN if no data was choosen to record.
 */
int
xnee_setup_recordext (xnee_data* xd);



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
 * Sets a XRecordRange's values to 0 (zero)
 * <br><br>
 * This uses memset instead of going through the structure and set each value to 0.
 *
 * @param range  the XRecordRange to set to 0
 * @return void  
 */
void 
xnee_null_range ( XRecordRange *range);






/*
 *  This function parses arguments and sets values that corresponds 
 *  to the options
 *  
 *   OBSOLETE 
void 
xnee_record_parse_args        (xnee_data*, int, char ** ) ;
*/



/*
 *
 * OBSOLETED by xnee_free_xnee_data 
int
xnee_record_close_down (xnee_data *);
*/


#endif   /* XNEE_RECORD_H */

