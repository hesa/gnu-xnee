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



/**
 * Sets the callback function for Xnee as located in the plugin 
 * previously loaded with xnee_dlopen. The function tries to 
 * resolve the name <sym_name> in the plugin lib.
 * 
 * If the funciton could not resolve the name the prevoius
 * is still used. It is up to the user to take action if 
 * resolving the <sym_name> wasn't successfull.
 * 
 * @param xd          xnee's main structure
 * @param dest        where to store the function. 
 * @param sym_name    name of the function to find        
 * @return int        XNEE_OK if successful. 
 *                    XNEE_WRONG_PARAMS if type is wrong, 
 *                    XNEE_NO_MAIN_DATA if xd is null
 */
int
xnee_set_callback (xnee_data *xd,
		   callback_ptrptr, 
		   char *sym_name);

/* OLD VERSION
  xnee_set_callback (xnee_data *xd,
		   void (**dest) (XPointer , XRecordInterceptData *), 
		   char *sym_name);
*/


/**
 * Sets the synchronising callback function for Xnee as located 
 * in the plugin previously loaded with xnee_dlopen. The function 
 * tries to resolve the name <sym_name> in the plugin lib.
 * 
 * If the funciton could not resolve the name the prevoius
 * is still used. It is up to the user to take action if 
 * resolving the <sym_name> wasn't successfull.
 * 
 * @param xd          xnee's main structure
 * @param dest        where to store the function
 * @param sym_name    name of the function to find        
 * @return int        XNEE_OK if successful. 
 *                    XNEE_WRONG_PARAMS if type is wrong, 
 *                    XNEE_NO_MAIN_DATA if xd is null
 */
int
xnee_set_synchronize (xnee_data *xd,
		      synch_ptrptr dest, 
		      char *sym_name);

/* OLD VERSION
xnee_set_synchronize (xnee_data *xd,
		      void (**dest) (xnee_data *xd, int replayed_type, int replayed_nr), 
		      char *sym_name);
*/
