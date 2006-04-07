/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003 
 *                      2004, 2005, 2006 Henrik Sandklef
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
#include "cnee.h"



char *explain[] = {
  "file_name" , "Name of a file (e.g /tmp/user1_session)" ,
  "display_name" , "Name of a display (e.g 192.168.1.12:0" ,
  "secs" , "Seconds (e.g 10)" ,
  "n" , "Integer number (e.g 100)" ,
  "LIST" , "Comma separated list of display (e.g 192.168.1.12:0,129.12.23.0:0) ",
  "X_LIST" , "Comma separated list of integers (e.g 12-23,35,45-56) " ,
  NULL 
};


char *examples[] = {
  ""XNEE_CLI "--record --events-to-record 1000 -devera 2-6 -o /tmp/xnee.xns -e /tmp/xnee.log -v", 
  "Writes 1000 data to file /tmp/xnee.xns and dumps the verbose printout to /tmp/xnee.log",
  ""XNEE_CLI " --replay -f /tmp/xnee.xns -v -e /tmp/xnee.log --no-sync",
  "Read data from /tmp/xnee.xns, replay it and verbose print to file /tmp/xnee.log",
  "For more examples, read the Xnee manual",
  NULL 
};


char *description[] = {
  "  " XNEE_CLI " (part of the " PACKAGE " project) can record and replay an X session. " XNEE_CLI " also has the ability to distribute events to multiple displays.\n",
  NULL 
};







