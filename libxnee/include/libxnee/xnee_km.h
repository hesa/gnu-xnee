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

#ifndef XNEE_KM_H
#define XNEE_KM_H

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
xnee_get_grab_mode ( xnee_data *xd, int key, int modifier);

int
xnee_check_km(xnee_data *xd);



int
xnee_get_km_tuple (xnee_data     *xd, 
		   xnee_km_tuple *km, 
		   char          *mod_and_key);




#endif /* XNEE_KM_H */