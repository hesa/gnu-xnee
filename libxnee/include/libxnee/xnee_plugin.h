/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004,
 *                      2008, 2009 Henrik Sandklef 
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


#ifndef XNEE_XNEE_PLUGIN_H
#define XNEE_XNEE_PLUGIN_H

/**
 * Opens the plugin file (pl_name). Xnee will use the functions in this file instead of the built in
 * @param xd       xnee's main structure
 * @param pl_name  name of the plugin file
 * @param mode     external (1) or internal (0) plugin
 * @return int     0 means OK.
 */
int
xnee_use_plugin(xnee_data *xd, char *pl_name, unsigned char mode);


/**
 * Closes the previoulsy opened plugin file (pl_name). 
 * Xnee will use the defaults functions as callbacks again
 * @param xd       xnee's main structure
 * @param pl_name  name of the plugin file
 * @return int     0 means OK.
 */
int
xnee_unuse_plugin(xnee_data *xd);






#endif /* XNEE_XNEE_PLUGIN_H */


