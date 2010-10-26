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


#ifndef XNEE_XNEE_SESSION_H
#define XNEE_XNEE_SESSION_H


int
xnee_prepare(xnee_data *xd);





/**
 * Returns the number of RecordRanges allocated so far
 * @param xd    xnee's main structure
 * @return int  number of RecordRanges allocated
 */
int
xnee_get_max_range (xnee_data *xd);




/**
 * Init Xnee structures
 * @param xd   xnee's main structure
 * @return int 1 if successfull, else 0 
 * \todo (return values need to be changed) 
 */
int
xnee_init(xnee_data* xd);




/**
 * Close down all open files and displays
 * @param xd     xnee's main structure
 * @return void  
 */
void
xnee_close_down(xnee_data *xd);



/**
 * Set variables so that Xnee can stop gracefully. 
 * @param xd    xnee's main structure
 * @return int  0 on success
 */
int 
xnee_stop_session( xnee_data* xd);



int 
xnee_process_count(int mode);

int 
xnee_process_replies(xnee_data *xd);


int
xnee_rep_prepare(xnee_data *xd);

int
handle_xerr(Display *dpy, XErrorEvent *errevent);



/* internal use */
void 
signal_handler(int sig)  __attribute__ ((noreturn));



int
xnee_err_handler(Display* dpy, XErrorEvent* ev);


int
xnee_more_to_record(xnee_data *xd);


/**
 * Zeros the sync data in xd.
 * @param xd     xnee's main structure
 * @return void  
 */
void
xnee_zero_sync_data (xnee_data* xd);

int 
xnee_set_x_server_version(xnee_data *xd);


#endif /* SESSION_*/
