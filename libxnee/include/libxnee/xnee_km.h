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

#define XNEE_SYMBOLIC_ALT      "Alt"
#define XNEE_SYMBOLIC_CONTROL  "Control"
#define XNEE_SYMBOLIC_SCROLL   "Scroll"
#define XNEE_SYMBOLIC_CAPS     "Caps"
#define XNEE_SYMBOLIC_SHIFT    "Shift"

#define XNEE_REAL_ALT      "Alt_L"
#define XNEE_REAL_CONTROL  "Control_L"
#define XNEE_REAL_SCROLL   "Scroll_L"
#define XNEE_REAL_CAPS     "Caps_Lock"
#define XNEE_REAL_SHIFT    "Shift_L"


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
