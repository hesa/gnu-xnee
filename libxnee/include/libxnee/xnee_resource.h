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


#ifndef XNEE_RESOURCE_H
#define XNEE_RESOURCE_H
/**
 * Opens the resource file as specified in xd. Reads it and calls xnee_add_resource_syntax where applicable.
 * @param xd    xnee's main structure
 * @return int  0 means OK.
 */
int
xnee_add_resource(xnee_data *xd);

/**
 * Takes a Xnee setting in form of a string (with the resource format).
 * @param xd  
 * @param tmp  
 * @return int  1 on success, 0 on failure.
 * \todo (return values need to be changed) 
 */
int
xnee_add_resource_syntax(xnee_data *xd, char *tmp);





#endif  /* XNEE_RESOURCE_H */
