/*
Xnee's Not an Event Emulator enables recording and replaying of X protocol data
Copyright (C) 1999, 2000, 2001, 2002, 2003 Henrik Sandklef

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/



#include "xnee_record.h"
#include "xnee_replay.h"
#include "print_varargs.h"

#ifndef XNEE_PRINT_H
#define XNEE_PRINT_H

/**
 * Prints number and name of X11 events
 *
 * @param  xd     xnee's main structure
 * @return int    XNEE_OK on success. 
 */
int
xnee_print_event_info (xnee_data *xd);


/**
 * Prints number and name of X11 errors
 *
 * @param xd     xnee's main structure
 * @return void  
 */
void
xnee_print_error_info (xnee_data *xd);


/**
 * Prints number and name of X11 replies
 *
 * @param xd     xnee's main structure
 * @return void  
 */
void
xnee_print_reply_info (xnee_data *xd);


/**
 * Prints number and name of X11 requests
 *
 * @param xd     xnee's main structure
 * @return void  
 */
void
xnee_print_request_info (xnee_data *xd);


/**
 * Prints number and name of X11 data
 *
 * @param xd     xnee's main structure
 * @return void  
 */
void
xnee_print_data_info (xnee_data *xd);


/**
 * Verbose prints to specified file
 *
 * @param xd     xnee's main structure
 * @param str    formatted string 
 * @param ....   extra parameters to string
 * @return void  
 */
void
xnee_verbosef (xnee_data* xd, char * str, ...);



int
xnee_print_sys_info(xnee_data *, FILE *);




/*
 * Print character representation of event if verbose mode is on
 */
void 
xnee_verbose_event (xnee_data *, int ev ) ;


/*
 * Print an error message
 */
void
xnee_print_error (char *, ...);






/*
 *
 * Name:      xnee_record_print_request
 *
 * Arguments: xnee_data
 *            XRecordInterceptData
 *
 *
 * Description: Prints X requests according to the Xnee protocol/format
 *
 */
void 
xnee_record_print_request (xnee_data *xd, XRecordInterceptData *xrecintd );


/*
 *
 * Name:      xnee_human_print_request
 *
 * Arguments: xnee_data
 *            XRecordInterceptData
 *
 *
 * Description: Prints X requests so humans can read
 *
 */
void 
xnee_human_print_request (xnee_data *xd, XRecordInterceptData *xrecintd );


/*
 *
 * Name:      xnee_human_print_reply
 *
 * Arguments: xnee_data
 *            XRecordInterceptData
 *
 *
 * Description: Prints X replies so humans can read 
 *
 */
void xnee_human_print_reply (xnee_data* xd, XRecordInterceptData *xrecintd );


/*
 *
 * Name:      xnee_record_print_reply
 *
 * Arguments: xnee_data
 *            XRecordInterceptData
 *
 *
 * Description: Prints X replies  according to the Xnee protocol/format
 *
 */
void xnee_record_print_reply (xnee_data* xd, XRecordInterceptData *xrecintd );



/*
 *
 * Name:      xnee_record_print_error
 *
 * Arguments: xnee_data
 *            XRecordInterceptData
 *
 *
 * Description: Prints X errors according to the Xnee protocol/format
 *
 */
void xnee_record_print_error (xnee_data *xd, XRecordInterceptData *xrecintd );


/*
 *
 * Name:      xnee_human_print_error
 *
 * Arguments: xnee_data
 *            XRecordInterceptData
 *
 *
 * Description: Prints X errors in a human format
 *
 */
void xnee_human_print_error (xnee_data *xd, XRecordInterceptData *xrecintd );

/*
 *
 * Name:      xnee_human_print_event
 *
 * Arguments: xnee_data
 *            XRecordInterceptData
 *
 *
 * Description: Prints X events in a human format
 *
 */
void xnee_human_print_event (xnee_data *xd, XRecordInterceptData *xrecintd );



/*
 *
 * Name:      xnee_record_print_record_range
 *
 * Arguments: xnee_data* , FILE*
 *
 *
 * Description: Prints the Record range in a(n almost) user friendly format
 *              File descriptors to be used are specified in the xnee_data struct
 *
 */
void xnee_record_print_record_range (xnee_data *xd, FILE*) ;


/*
 *
 * Name:      xnee_record_print_sys_info
 *
 * Arguments: xnee_data
 *
 *
 * Description: Prints the X and xnee states and settings
 *              The format of the file will be recognised by xnee 
 *              when replaying, in order to use the same settings
 *              and thereby enable synchronisation again
 * 
 * 
 */
int
xnee_rec_print_sys_info(xnee_data *xd);




/*
 *
 * Name:      xnee_print_xnee_settings
 *
 * Arguments: xnee_data
 *
 *
 * Description: Prints some init stuff ......
 *              This function is obsoleted by the new printout function xnee_rec_print_sys_info
 * 
 * 
 */
void 
xnee_print_xnee_settings (xnee_data* xd, FILE *) ;


/*
 *   Name:          xnee_print_distribution_list
 *
 *   Arguments:
 *     xnee_data *, FILE *
 *
 *   Description:   Prints the list of displays to distribute events to
 *
 *                  
 */ 
int 
xnee_print_distr_list (xnee_data*, FILE *);





/*
 *   Name:        xnee_print_data_range_count
 *
 * Arguments: xnee_data*
 *
 * 
 */
int 
xnee_print_data_range_count (xnee_data * xd);


#ifdef NO_BUF_VERBOSE
#define xnee_replay_printbuffer(a) 
#else
#define xnee_replay_printbuffer(a) xnee_replay_printbuffer_impl(a) 
#endif


/**
 * If xnee_data->verbose is set this functions prints the synchronisation buffers.
 *
 * @param xd       xnee's main structure
 * @return void  
 */
void  
xnee_replay_printbuffer_impl (xnee_data *xd);


/**
 * Print version information etc
 * 
 * @param xd      xnee's main structure
 * @return void  
 */
void
xnee_version(xnee_data *xd);


void
xnee_print_ranges (xnee_data *xd, FILE *fp);

#endif /* XNEE_PRINT_H */
