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

#include <X11/Xproto.h>
#include <X11/Xlibint.h>
#include <X11/Xlib.h>

#include <X11/extensions/record.h> 
#include <X11/extensions/XTest.h> 

#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/xnee_dl.h"
#include "libxnee/xnee_sem.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_resolution.h"
#include "libxnee/xnee_grab.h"
#include "libxnee/xnee_km.h"
#include "libxnee/datastrings.h"


extern xnee_options_t  *xnee_options;

#ifndef XNEE_RESOURCE_H
#define XNEE_RESOURCE_H

#define XNEE_RES_PROJECT                 "ProjectName"
#define XNEE_RES_DESCRIPTION             "ProjectDescription"
#define XNEE_RES_CREAT_DATE              "ProjectCreationDate"
#define XNEE_RES_CREAT_PROGRAM           "ProjectCreationProgram"
#define XNEE_RES_CREAT_PROG_VER          "ProjectCreationProgVersion"
#define XNEE_RES_LASTCHANGE_DATE         "ProjectLastChangeDate"
#define XNEE_RES_LASTCHANGE_PROGRAM      "ProjectLastChangeProgram"
#define XNEE_RES_LASTCHANGE_PROG_VER     "ProjectLastChangeProgVersion"
#define XNEE_RES_CURRENT_CHANGE_DATE     "ProjectCurrentChangeDate"
#define XNEE_RES_CURRENT_CHANGE_PROGRAM  "ProjectCurrentChangeProgram"
#define XNEE_RES_CURRENT_CHANGE_PROG_VER "ProjectCurrentChangeProgVersion"

#define XNEE_RES_AUTHOR_NAME              "Author"
#define XNEE_RES_AUTHOR_EMAIL             "Email"

#define XNEE_NO_SYNTAX  0
#define XNEE_CLI_SYNTAX 1
#define XNEE_XNS_SYNTAX 2
#define XNEE_CLI_PREFIX "--"

#define XNEE_RESOURCE_CHECK(a,b) strncmp(a,b,strlen(a))
#define XNEE_REMOVE_BEGINING_BLANKS(val) \
  do  { val++ ;} while( (val[0] == ' ') || (val[0] == '\t') ); 
#define XNEE_REMOVE_TRAILING_CRAP(a) \
  do { a[strlen(a)-1]='\0'; } \
  while ( (a[strlen(a)-1] == ' ') || (a[strlen(a)-1] == '\n') ) ;


int
xnee_init_xnee_resource_meta(xnee_resource_meta* xrm);

int 
xnee_new_xnee_resource_meta(xnee_resource_meta* xrm);

int
xnee_free_xnee_resource_meta(xnee_resource_meta* xrm);



/**
 * Opens the resource file as specified in xd. Reads it and calls xnee_add_resource_syntax where applicable.
 * @param xd    xnee's main structure
 * @return int  0 means OK.
 */
int
xnee_add_resource(xnee_data *xd);


/**
 * Takes a Xnee setting in form of a string (using Xnee Session/Resource file format)
 * @param xd  
 * @param tmp  
 * @return int  1 on success, 0 on failure.
 * \todo (return values need to be changed) 
 */
#define xnee_add_resource_syntax(xd, tmp_str)  xnee_add_resource_syntax_impl(xd, tmp_str, XNEE_XNS_SYNTAX) 

/**
 * Takes a Xnee setting in form of a string as used by program (gnee, cnee)
 * @param xd  
 * @param tmp  
 * @return int  1 on success, 0 on failure.
 * \todo (return values need to be changed) 
 */
#define xnee_add_cli_syntax(xd, tmp_str)  xnee_add_resource_syntax_impl(xd, tmp_str, XNEE_CLI_SYNTAX) 

/**
 * Takes a Xnee setting in form of a string 
 * @param xd  
 * @param tmp  
 * @return int  1 on success, 0 on failure.
 * \todo (return values need to be changed) 
 */
int
xnee_add_resource_syntax_impl(xnee_data *xd, char *str, int syntax_type);

int
xnee_set_project_name(xnee_data *xd, char* project_name_str);

int
xnee_set_project_descr(xnee_data *xd, char* project_descr_str);
int
xnee_set_creat_date(xnee_data *xd, char* creat_date_str);
int
xnee_set_creat_program(xnee_data *xd, char* creat_prog_str);
int
xnee_set_creat_prog_vers(xnee_data *xd, char* creat_prog_date_str);
int
xnee_set_last_date(xnee_data *xd, char* last_date_str);
int
xnee_set_last_program(xnee_data *xd, char* last_prog_str);
int
xnee_set_last_prog_vers(xnee_data *xd, char* last_prog_date_str);
int
xnee_set_author_name(xnee_data *xd, char* author_name_str);
int
xnee_set_author_email(xnee_data *xd, char* author_email_str);
int
xnee_set_char(xnee_data *xd, char* char_str);


char *
xnee_get_project_name(xnee_data *xd);

char *
xnee_get_project_descr(xnee_data *xd);

char *
xnee_get_creat_date(xnee_data *xd);

char *
xnee_get_creat_program(xnee_data *xd);

char *
xnee_get_creat_prog_date(xnee_data *xd);

char *
xnee_get_last_date(xnee_data *xd);

char *
xnee_get_last_program(xnee_data *xd);

char *
xnee_get_last_prog_vers(xnee_data *xd);

char *
xnee_get_author_name(xnee_data *xd);

char *
xnee_get_author_email(xnee_data *xd);

char *
xnee_get_char(xnee_data *xd);

char *
xnee_get_project_name_str(xnee_data *xd);

char *
xnee_get_project_descr_str(xnee_data *xd);

char *
xnee_get_creat_date_str(xnee_data *xd);

char *
xnee_get_creat_prog_vers(xnee_data *xd);

char *
xnee_get_last_date_str(xnee_data *xd);

char *
xnee_get_last_prog_str(xnee_data *xd);

char *
xnee_get_last_prog_date_str(xnee_data *xd);

char *
xnee_get_author_name_str(xnee_data *xd);

char *
xnee_get_author_email_str(xnee_data *xd);

char *
xnee_get_char_str(xnee_data *xd);

int
xnee_handle_resource_meta (xnee_data *xd, char *meta_str);

int
xnee_is_option(xnee_data *xd, 
	       xnee_options_t *options, 
	       const char *str, 
	       int syntax_type);

int
xnee_is_resource_option(xnee_data *xd, const char *str, int syntax_type);


int
xnee_add_strings (xnee_data  *xd, 
		  xnee_options_t  *options,
		  const char *long_option, 
		  const char *short_option, 
		  char       *args,
		  char       *option_descr,
		  int         type);

int
xnee_find_option_entry (xnee_data  *xd, const char *option, xnee_options_t  *options);

#define xnee_parse_check(xd, str, opt) \
    xnee_parse_check_impl(xd, xnee_options, str, opt) 

int 
xnee_parse_check_impl(xnee_data *xd, 
		      xnee_options_t *options,
		      const char *str, 
		      char *opt);


#endif  /* XNEE_RESOURCE_H */
