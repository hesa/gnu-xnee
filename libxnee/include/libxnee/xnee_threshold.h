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


#include <X11/extensions/XTest.h>

#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"
#include "libxnee/xnee_sem.h"
#include "libxnee/xnee_resolution.h"


int
xnee_set_max_threshold_str (xnee_data *xd, char *thresh_str);

int
xnee_set_min_threshold_str (xnee_data *xd, char *thresh_str);

int
xnee_set_tot_threshold_str (xnee_data *xd, char *thresh_str);

int
xnee_set_max_threshold (xnee_data *xd, int thresh);

int
xnee_set_min_threshold (xnee_data *xd, int thresh);

int
xnee_set_tot_threshold (xnee_data *xd, int thresh);





int
xnee_get_max_threshold (xnee_data *xd);

int
xnee_get_min_threshold (xnee_data *xd);

int
xnee_get_tot_threshold (xnee_data *xd);


