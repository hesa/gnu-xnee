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

#ifndef XNEE_XNEE_ERROR_H
#define XNEE_XNEE_ERROR_H

#include <libxnee/xnee.h>


#define XNEE_PRINT_ERROR_IF_NOT_OK(ret)           \
   {                                            \
      if (ret != XNEE_OK)                                         \
      {                                                           \
         fprintf (stderr, "%s\n", xnee_get_err_description(ret)); \
      } \
   }


const char *
xnee_get_err_description (int error) ;

const char *
xnee_get_err_solution (int error);

char *
xnee_get_err_string (void);

char *
xnee_set_err_string (char *new_error, ...) __attribute__ ((format (printf, 1 ,2)));

void
xnee_free_err_string (void);


#endif /* XNEE_XNEE_ERROR_H */
