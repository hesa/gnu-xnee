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

#include <X11/Xlibint.h>
#include <X11/Xproto.h>
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


#ifndef XNEE_RESOURCE_H
#define XNEE_RESOURCE_H

#define XNEE_RES_PROJECT             "Project"

#define XNEE_RES_DESCRIPTION         "Description"
#define XNEE_RES_CREAT_DATE          "CreationDate"
#define XNEE_RES_CREAT_PROGRAM       "CreationProgram"
#define XNEE_RES_CREAT_PROG_VER      "CreationProgVersion"
#define XNEE_RES_LASTCHANGE_DATE     "LastChangeDate"
#define XNEE_RES_LASTCHANGE_PROGRAM  "LastChangeProgram"
#define XNEE_RES_LASTCHANGE_PROG_VER "LastChangeProgVersion"

#define XNEE_RES_AUTHOR_NAME      "Author"
#define XNEE_RES_AUTHOR_EMAIL     "Email"

 

#define XNEE_RESOURCE_CHECK(a,b) strncmp(a,b,strlen(a))
#define XNEE_REMOVE_BEGINING_BLANKS(val) \
  while( (val[0] == ' ') || (val[0] == '\t') )val++;
#define XNEE_REMOVE_TRAILING_CRAP(a) \
  while( (a[strlen(a)-1] == ' ') || (a[strlen(a)-1] == '\n') ) \
  { a[strlen(a)-1]='\0'; a--; }

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
 * Takes a Xnee setting in form of a string (with the resource format).
 * @param xd  
 * @param tmp  
 * @return int  1 on success, 0 on failure.
 * \todo (return values need to be changed) 
 */
int
xnee_add_resource_syntax(xnee_data *xd, char *tmp);





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

#endif  /* XNEE_RESOURCE_H */
