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


#include <X11/extensions/XTest.h>

#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"
#include "libxnee/xnee_sem.h"
#include "libxnee/xnee_resolution.h"

#ifndef XNEE_XNEE_BUFFER_H
#define XNEE_XNEE_BUFFER_H

int
xnee_replay_buffer_max_diff (xnee_data* xd, int type);

int
xnee_replay_buffer_min_diff (xnee_data* xd, int type);

int
xnee_replay_buffer_tot_diff (xnee_data* xd, int type);

void 
xnee_replay_buffer_handler (xnee_data* xd, 
			    int data_type, 
			    int data_nr,
			    Bool rec_or_rep);


int 
xnee_hard_update_buffer_cache(xnee_data *xd);



int
xnee_check_buffer_limits (xnee_data *xd);

/**
 * Prints the synchronisation status of the X11 data given by data_type and nr
 * 
 * @param xd         xnee's main structure
 * @param data_type  X11 data type (e.g event)
 * @param nr         nr of the data type (e.g 23)
 * @return int       0 means in sync. 
 */
int
xnee_replay_buffer_status (xnee_data* xd, int data_type, int nr);



int 
xnee_update_buffer_cache(xnee_data *xd);



#endif /* XNEE_XNEE_BUFFER_H */
