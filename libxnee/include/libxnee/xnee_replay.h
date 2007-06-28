/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003 Henrik Sandklef                    
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



#ifndef XNEE_REPLAY_H
#define XNEE_REPLAY_H


#include "xnee.h"
#include "xnee_record.h"




/* internal use only */
unsigned long int 
xnee_delta_time( xnee_intercept_data *);


/**
 * Read the Xnee Manual and Xnee developer Manual for a more detailed 
 * description on synchronisation. 
 *
 * @param xd             xnee's main structure
 * @return void  
 */
int
xnee_replay_synchronize (xnee_data *xd) ;



#ifdef USE_OBSOLETE
/**
 * Reads from file (or stdin) an X11 protocol data and places it in xid.
 * If no X11 data was found it calls xnee_handle_meta_data to handle the data.
 * 
 * @param xd    xnee's main structure
 * @param xid   X11 data read from file is stored here
 * @return int  number of data read. 0 means failure or no more data
 *  
 */
int   
xnee_replay_read_protocol (xnee_data *xd, xnee_intercept_data *xid);
#endif



/**
 * Handles all meta data read from file. <br>
 * The meta data is used to setup Xnee when replaying.
 *
 * @param xd     xnee's main structure
 * @param str    the meta data found in the file
 * @return int 
 */
int
xnee_handle_meta_data(xnee_data* xd ,char* str );



/**
 * Reads data from file (or stdin) until the file reaches an end or the user presses
 * the stop modifier+key.
 * 
 * @param xd           xnee's main structure
 * @param read_mode    Read meta data or read replay data
 * @return void  
 */
int
xnee_replay_main_loop ( xnee_data *xd, int read_mode);





/**
 * Sets up Record extension with nice values
 *
 * @param xd   xnee's main structure
 * @return int 0 on success.
 */
int
xnee_setup_rep_recording(xnee_data *xd);





/**
 * Callback function for Record extension when an X11 data is ready to be picked up the 
 * recording program (yes, that's Xnee). It can also be called indirectly by Xnee when
 * Xnee calls XRecordProcessReplies. 
 * <br>
 * If data was recievied it is handled by the synchronise function. If no data was ready
 * to be read (as is usually the case when calling XRecordProcessReplies (which in turn
 * calls this function)) nothing is done.
 * 
 * @param xd      pointer to xnee's main structure
 * @param xp      pointer to data as sent by the Xserver
 * @return void  
 */
void  
xnee_replay_dispatch (XPointer xd, 
		      XRecordInterceptData  *xp);









/**
 * Stores data in buffers. 
 * The synchronisation and callback functions makes use of this function to store
 * data that are out of sync.
 *
 * @param xd           xnee's main structure
 * @param data_type    X11 data type (e.g event)
 * @param nr           X11 data nr (e.g 23)
 * @param rec_or_rep   Is the data received or replayed  
 * @return int  
 */
int
xnee_replay_buffer_handle (xnee_data* xd, int data_type, int nr, Bool rec_or_rep);





/**
 * Verifies that XTest extension is present
 *
 * @param xd    xnee's main structure
 * @return int  0 if not. 1 if XTest is present. 
 * \todo (return values need to be changed) 
 */
int
xnee_has_xtest_extension (xnee_data *xd) ;





/**
 * Inits the replaying data in xnee_data
 * @param xd       xnee's main structure
 * @return void  
 */
void  
xnee_replay_init (xnee_data *xd);






/* OBSOLETED */
void  
xnee_m_delay (int);



/**
 * Parses the arguments given in argv as given by the resource file syntax.<br>
 * Read the manual for a description of the syntax.
 * @param xd     xnee's main structure
 * @param argc   number of 'strings' in argv
 * @param argv   array of strings containing directives to Xnee
 * @return void  
 */
void  
xnee_replay_parse_args(xnee_data *xd, int argc, char **argv);


/**
 * A callback function that only prints the data retreived from the Xserver
 *
 * @param xp   pointer to data as sent by the Xserver
 * @param xd   xnee's main structure
 * @return void  
 */
void  
xnee_replay_dispatch2 (XPointer xp, 
		       XRecordInterceptData  *xd);

/*
 *  
 * Set the record range 
 * This is used when recording data to  synchronize the replay
 *
 *  OBSOLETE .... USED BEFORE WHEN TESTING
 *
 *
void  
xnee_replay_select_protocol (XRecordRange *) ;
*/





#endif   /* XNEE_REPLAY_H */

