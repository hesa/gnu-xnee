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
#ifndef XNEE_RESOLUTION_H
#define XNEE_RESOLUTION_H

#include "libxnee/xnee.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"


int
xnee_resolution_init (xnee_data *xd);
 
int
xnee_set_rec_resolution (xnee_data *xd, char *str);
 
int
xnee_get_rec_resolution_x (xnee_data *xd);

int
xnee_get_rec_resolution_y (xnee_data *xd);

int
xnee_set_rep_resolution (xnee_data *xd, char *str);
 
int
xnee_get_rep_resolution_x (xnee_data *xd);
 
int
xnee_get_rep_resolution_y (xnee_data *xd);

int 
xnee_str_to_res(char *res_str, xnee_res *xr);

int 
xnee_res_cmp(xnee_res *xr1, xnee_res *xr2);

int 
xnee_resolution_differs (xnee_data *xd);

int 
xnee_resolution_newx (xnee_data *xd, int xval);

int 
xnee_resolution_newy (xnee_data *xd, int yval);

int
xnee_set_default_rep_resolution (xnee_data *xd);

int
xnee_no_rep_resolution (xnee_data *xd);

int 
xnee_is_resolution_used (xnee_data *xd);

int 
xnee_set_resolution_used (xnee_data *xd);

int 
xnee_unset_resolution_used (xnee_data *xd);

int 
xnee_get_resolution_used (xnee_data *xd);

#endif 

