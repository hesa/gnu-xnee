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


#ifndef CNEE_PARSE_H
#define CNEE_PARSE_H


#include "libxnee/xnee.h"


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
int
xnee_parse_args (xnee_data* xd, 
		 int argc, 
		 char **argv) ;



/**
 * Prints cnee's options with description with man(nroff) syntax. This is
 * used when generating the man page for cnee. 
 * <br>Doing like this we need not update the man page for every new option.
 * This is done automagically by the make system att build/compile time.
 * @param fd         Where to print 
 * @return void  
 */
void 
xnee_manpage (FILE *fd);

/**
 * Prints Xnee's options with description with info syntax. This is
 * used when generating the info page for cnee. 
 * @param fd         Where to print 
 * @return void  
 */
void 
xnee_infopage (FILE *fd);


enum {
  CNEE_FILE_OPTION_KEY = 1          ,
  CNEE_HELP_OPTION_KEY              ,
  CNEE_FLAGS_OPTION_KEY             ,
  CNEE_PROJECT_OPTION_KEY           ,
  CNEE_VERSION_OPTION_KEY           ,
  CNEE_TYPE_HELP_OPTION_KEY         ,
  CNEE_GEN_MANPAGE_OPTION_KEY       ,
  CNEE_GEN_TEXIPAGE_OPTION_KEY      ,
  CNEE_WRITE_SETTINGS_OPTION_KEY    ,
  CNEE_PRINT_SETTINGS_OPTION_KEY    ,
  CNEE_PRINT_E_NAMES_OPTION_KEY     ,
  CNEE_PRINT_E_NAME_OPTION_KEY      ,
  CNEE_PRINT_ERR_NAMES_OPTION_KEY   ,
  CNEE_PRINT_ERR_NAME_OPTION_KEY    ,
  CNEE_PRINT_REP_NAMES_OPTION_KEY   ,
  CNEE_PRINT_REQ_NAMES_OPTION_KEY   ,
  CNEE_PRINT_REQ_NAME_OPTION_KEY    ,
  CNEE_PRINT_DATA_NAMES_OPTION_KEY  ,
  CNEE_RETYPE_FILE_OPTION_KEY       ,
  CNEE_REMOVE_EVENT_OPTION_KEY      ,
  CNEE_DEMONSTRATION_OPTION_KEY     ,
  XNEE_KEYBOARD_OPTION_KEY          ,
  XNEE_MOUSE_OPTION_KEY             ,
  CNEE_SYNTAX_CHECKER_KEY           
 } ;


#endif /* CNEE_PARSE_H */
