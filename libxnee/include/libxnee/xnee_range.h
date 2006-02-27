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



#ifndef XNEE_RANGE_H
#define XNEE_RANGE_H


#include "xnee.h"

struct xnee_range
{
   int *data;
   int index;
   int size;
};


struct xnee_ranges
{
   struct xnee_range type[XNEE_NR_OF_TYPES];
   int nr_of_data;
   int alloc_size;
} ;

/* Standard includes */
#include <stdio.h>

/* X11 includes */
#include <X11/Xmd.h>
#include <X11/Xproto.h>
#include <X11/Xos.h>
#include <X11/Xlibint.h>  /* Warning, there be dragons here.... */
#include <X11/Xlib.h>

#include "libxnee/xnee.h"



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
 * Sets a XRecordRange's values to 0 (zero)
 * <br><br>
 * This uses memset instead of going through the structure and set each value to 0.
 *
 * @param range  the XRecordRange to set to 0
 * @return void  
 */
void 
xnee_null_range ( XRecordRange *range);

int 
xnee_get_nr_of_data (int type);

int *
xnee_get_data (int type);

int 
xnee_print_list(void);

int 
xnee_set_ranges(xnee_data *xd);

int*
xnee_get_list(int type);

int 
xnee_rem_from_list(int type, int ev);


int
xnee_rem_data_from_range_str (xnee_data *xd, 
                              int type  , 
                              char *str) ;

int 
xnee_refresh_ranges(xnee_data *xd);

int 
xnee_free_ranges(xnee_data *xd);

int 
xnee_init_ranges(void);

int 
xnee_is_type_nr_set(xnee_data *xd, int type, int nr);

#endif 
