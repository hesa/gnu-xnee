/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003, 
 *                      2004, 2010, 2011  Henrik Sandklef 
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




#ifndef XNEE_XNEE_UTILS_H
#define XNEE_XNEE_UTILS_H
#include "xnee.h"

#ifdef LONG64
#define CARD32_PRINTF_FMT "%ld"
#define SIZE_T_PRINTF_FMT  "%d"
#else
#define CARD32_PRINTF_FMT "%ld"
#define SIZE_T_PRINTF_FMT "%d"
#endif
#define TIME_PRINTF_FMT  CARD32_PRINTF_FMT


/**
 * Removes XNEE_COMMENT_START from the argument
 * and removes unnecessary allocated memory
 * @param xd     xnee's main structure
 * @param str    string to clean up
 * @return int   1 on success
 */
int
xnee_rem_comment_start(xnee_data *xd, char *str) ;



int
xnee_write_settings_to_file (xnee_data *xd, FILE *fp);





/*
 *  INTERNAL USE
 *
 * Sleep for int seconds
 * and print some information to the user about the remaining time
 * 
 * Useful if a user wants to wait for a few seconds in order to iconize
 * the terminal window where record was started .... just an exanple
 *
 */
void xnee_delay (int , char *) ;


/**
 * Removes blanks from the argument
 * and removes unnecessary allocated memory
 * 
 * @param xd    xnee's main structure
 * @param str   string to clean up
 * @return int  1 on success
 */
int
xnee_strip(xnee_data *xd, char *str) ;





int
rem_all_blanks (char *array, size_t size);


int
rem_begin_blanks (char *array, int size);

int
rem_blanks (char *array, int size);






/**
 * Returns the clients (displays) id (resource )
 * @param dpy   display of the client
 * @return XID  client's X ID
 */
XID 
xnee_client_id (Display *dpy);

#define XNEE_ATOI_FUNCTION(func, xd, str, xnee_macro_in_ret_val) \
        { \
          int my_xnee_atio_macro_function_ret ; \
          if ( sscanf(str, "%d", &my_xnee_atio_macro_function_ret) == 1) \
             xnee_macro_in_ret_val = func(xd, my_xnee_atio_macro_function_ret); \
          else {\
                xnee_macro_in_ret_val = XNEE_WRONG_PARAMS ; \
               }\
        }
 




int
xnee_str2int(xnee_data *xd, char *str);

char **
xnee_str2strptr (char *tmp, int fill_option);

void
xnee_print_strptr(xnee_data *xd, char **strptr);

int
xnee_free_strptr(char **strptr);


int
xnee_boolstr2int(xnee_data *xd, char *str);

int 
xnee_record_from_data_display(xnee_data *xd);

Display *
xnee_get_display_for_recordcontext(xnee_data *xd);

int
xnee_get_xinput_event_base(Display *dpy);

int 
xnee_check_true(char *expr);

int 
xnee_check_false(char *expr);

int
xnee_is_screen_ok(xnee_data *xd, int screen);



#endif /*  XNEE_XNEE_UTILS_H */
