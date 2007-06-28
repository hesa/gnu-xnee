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

#ifndef XNEE_TIME_H
#define XNEE_TIME_H




int
xnee_reset_elapsed_time(xnee_data *xd);


/**
 * Returns the elapsed time from either the 1st read or the last call to this f'n
 * 
 * @param xd         xnee's main structure
 * @param type       char (f or l ) represents type of elapsed time 
 * @return long      elapsed time. 
 */
long 
xnee_get_elapsed_time(xnee_data *xd, char type );



/**
 * Returns the calculated sleep amount - attempts to keep timing the same as when recorded 
 * 
 * @param xd         		xnee's main structure
 * @param last_diff   		amount of time elapsed since last read 
 * @param first_diff  		amount of time elapsed since 1st read (ie how long playback has been running) 
 * @param record_last_diff      recorded amount of time between last recorded entry and current entry
 * @param record_first_diff      recorded amount of time between start of recording and current entry 
 * @return long                 amount of time to sleep
 */
long 
xnee_calc_sleep_amount(xnee_data *xd, 
		       unsigned long last_diff, 
		       unsigned long first_diff, 
		       unsigned long record_last_diff, 
		       unsigned long record_first_diff );


#define xnee_is_normal_speed(xd)  (xd->speed_percent==100)
#define xnee_get_speed(xd)  xd->speed_percent

long 
xnee_calc_sleep_amount_percent(xnee_data *xd, 
			       Time last_diff, 
			       Time first_diff, 
			       Time record_last_diff, 
			       Time recordFirst_diff );


long 
xnee_calc_sleep_amount_slow(xnee_data *xd, 
			    unsigned long  last_diff, 
			    unsigned long first_diff, 
			    unsigned long record_last_diff, 
			    unsigned long recordFirst_diff );

long 
xnee_calc_sleep_amount_fast(xnee_data *xd, 
			    unsigned long last_diff, 
			    unsigned long first_diff, 
			    unsigned long record_last_diff, 
			    unsigned long recordFirst_diff );


#endif /* XNEE_TIME_H */
