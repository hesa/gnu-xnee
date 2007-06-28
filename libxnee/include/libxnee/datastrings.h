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



#include <stdio.h>

#ifndef XNEE_DATASTRINGS_H 
#define XNEE_DATASTRINGS_H 

void 
xnee_init_names(void);


/**
 * Get the integer value from X11 data dat
 * @param dat
 * @param type
 * @return int
 */
int
xnee_data2int(int type, char *dat);


/**
 * Get the integer value from X11 data dat
 * @param dat
 * @param type type is saved here
 * @return int
 */
int
xnee_data2int_special(int *type, char *dat);



/**
 * Get the integer value from event ev
 * @param ev
 * @return int
 */
int
xnee_event2int(char *ev);

/**
 * Get the integer value from request req
 * @param req
 * @return int
 */
int
xnee_request2int(char *req);

/**
 * Get the integer value from error err
 * @param err
 * @return int
 */
int
xnee_error2int(char *err);


/**
 * Prints a string representation of the error
 * @param err  
 * @return char *  
 */
char *
xnee_print_error_code ( int err);



/*
 * Prints the name of the XEvent that corresponds to the argument ev
 * Printout is made on stdout 
 */
char *
xnee_print_event (int); 


/*
 * Prints the name of the Request that corresponds to the argument ev
 * Printout is made on stdout 
 */
char * 
xnee_print_request ( int req);



char *
xnee_int2event(int ev);

char *
xnee_int2request(int req);

char *
xnee_int2error(int err);


struct data_description*
xnee_get_event_names(void);

struct data_description*
xnee_get_error_names(void);

struct data_description*
xnee_get_request_names(void);

struct data_description*
xnee_get_reply_names(void);

char*
xnee_int2data(int type, int dat);


#endif /*  XNEE_DATASTRINGS_H  */
