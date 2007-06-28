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

#ifndef XNEE_KM_H
#define XNEE_KM_H

const char *
xnee_symbolic_modifier2modifier (xnee_data *xd, const char *mod_str);


/**
 * Grabs the modifier and key as specified on xd. 
 * These keys are mapped to call xnee_stop_session
 * @param xd      xnee's main structure
 * @param mode    action mode (stop, pause, resume)
 * @param mod_key string representation of modifier + key
 * @return int   XNEE_OK on success
 */
int 
xnee_grab_key (xnee_data* xd , int mode, char *mod_key);


int
xnee_check_key(xnee_data *xd);



int
xnee_get_action_key (xnee_data        *xd, 
		     xnee_action_key  *xk, 
		     char             *mod_and_key);

int 
xnee_handle_rec_key(xnee_data *xd);

int 
xnee_handle_rep_key(xnee_data *xd);


int
xnee_key_check(xnee_data *xd);


#endif /* XNEE_KM_H */
