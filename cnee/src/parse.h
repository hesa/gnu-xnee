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
 * Prints all flags/options that Xnee handles. Description of the flags is omitted.
 * @param fd      Where to print the flags
 * @return void  
 */
void 
xnee_flags (FILE *fd);


/**
 * Prints all flags/options (with description) that Xnee handles.
 * @param fd      Where to print the flags
 * @return void  
 */
void  
xnee_usage (FILE* fd);



/**
 * Parses the arguments in argv. 
 * @param xd       xnee's main structure
 * @param argc     number of args in argv
 * @param argv     list arguments 
 * @return void  
 */
void  
xnee_parse_args (xnee_data* xd, 
		 int argc, 
		 char **argv) ;



/**
 * Prints Xnee's options with description with man(nroff) syntax. This is
 * used when generating the man page for Xnee. 
 * <br>Doing like this we need not update the man page for every new option.
 * This is done automagically by the make system att build/compile time.
 * @param fd         Where to print 
 * @return void  
 */
void 
xnee_manpage (FILE *fd);
