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



#include "libxnee/print_varargs.h"
#include "libxnee/print.h"
#include "libxnee/datastrings.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"
#include "libxnee/xnee_resource.h"



/**************************************************************
 *                                                            *
 * xnee_print_error_code                                      *
 *                                                            *
 *                                                            *
 **************************************************************/
/*char *
xnee_print_error_code22(int status) 
{
  return ( err_codes[status]);
}
*/






/*
 * 
 * Prints the number and name of the XEvent  
 *
 */
int
xnee_print_event_info (xnee_data *xd) 
{
  int i ; 

  fprintf (xd->out_file,"  X11 Event\n\n");
  fprintf (xd->out_file,"number\tname\n");
  fprintf (xd->out_file,"======\t====\n");
  for (i=2;i<LASTEvent;i++)
    {
      fprintf (xd->out_file,"%.2d\t%s\n",i,xnee_print_event(i));
    }
  return XNEE_OK;
}


/*
 * 
 * Prints the number and name of the X11 error
 *
 */
void
xnee_print_error_info (xnee_data *xd) 
{
  int i ; 

  fprintf (xd->out_file,"  X11 Error\n\n");
  fprintf (xd->out_file,"number\tname\n");
  fprintf (xd->out_file,"======\t====\n");
  for (i=0;i<BadImplementation;i++)
    {
      fprintf (xd->out_file,"%.2d\t%s\n",i,xnee_print_error_code(i));
    }
}


/*
 * 
 * Prints the number and name of the X11 error
 *
 */
void
xnee_print_request_info (xnee_data *xd) 
{
  int i ; 

  fprintf (xd->out_file,"  X11 Request\n\n");
  fprintf (xd->out_file,"number\tname\n");
  fprintf (xd->out_file,"======\t====\n");
  for (i=1;i<X_NoOperation;i++)
    {
      fprintf (xd->out_file,"%.2d\t%s\n",i,xnee_print_request(i));
    }
}

/*
 * 
 * Prints the number and name of the X11 reply
 *
 */
void
xnee_print_reply_info (xnee_data *xd) 
{
  fprintf (xd->out_file,"  X11 Reply\n\n");
}

/*
 * 
 * Prints the number and name of the X11 data
 *
 */
void
xnee_print_data_info (xnee_data *xd) 
{
  xnee_print_event_info (xd) ;
  xnee_print_error_info (xd) ;
  xnee_print_request_info (xd) ;
  xnee_print_reply_info (xd) ;
}





  
/*
 * Print character representation of event if verbose mode is on.  
 */
void 
xnee_verbose_event (xnee_data * xd, int ev ) {
  if ( xd->verbose ) 
    {
      xnee_print_event (ev);
    }
}




/*
 *
 * xnee_record_print_request.  
 *
 */
void 
xnee_record_print_request (xnee_data *xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data  = (XRecordDatum *) (xrecintd->data) ;
  int           req_type = xrec_data->type ;
  
  xd->data_fp (xd->out_file,"1,%d,%lu\n", req_type, xrecintd->server_time);
}



/*
 *
 * xnee_human_print_request.  
 *
 */
void 
xnee_human_print_request (xnee_data *xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data  = (XRecordDatum *) (xrecintd->data) ;
  int           req_type = xrec_data->type ;
  
  xd->data_fp (xd->out_file,"Request   %.3d\t%s\n", 
	   req_type, 
	   xnee_print_request(req_type));
}

/*
 *
 * xnee_human_print_event.  
 *
 */
void 
xnee_human_print_event (xnee_data *xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data  = (XRecordDatum *) (xrecintd->data) ;
  int           event_type = xrec_data->type ;

  xd->data_fp (xd->out_file,"Event     %.3d\t%s\n", 
	    event_type,
	   xnee_print_event(event_type));
}



/*
 *
 *   xnee_record_print_reply.  
 *
 */
void xnee_record_print_reply (xnee_data* xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data  = (XRecordDatum *) (xrecintd->data) ;
  int           rep_type = xrec_data->type ;
  
  xd->data_fp (xd->out_file,"2,%d,%lu\n", rep_type,xrecintd->server_time);
}


/*
 *
 *   xnee_human_print_reply.  
 *
 */
void xnee_human_print_reply (xnee_data* xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data  = (XRecordDatum *) (xrecintd->data) ;
  int           rep_type = xrec_data->type ;
  
  xd->data_fp (xd->out_file,"Reply       %d,%lu\n", rep_type,xrecintd->server_time);
}




/*
 *
 *   xnee_record_print_error.  
 *
 */
void xnee_record_print_error (xnee_data *xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data  = (XRecordDatum *) (xrecintd->data) ;
  int           err_type = xrec_data->type ;
  
  xd->data_fp (xd->out_file,"3,%d,%lu\n", err_type,xrecintd->server_time);
}

/*
 *
 *   xnee_human_print_error.  
 *
 */
void xnee_human_print_error (xnee_data *xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data  = (XRecordDatum *) (xrecintd->data) ;
  int           err_type = xrec_data->type ;
  
  xd->data_fp (xd->out_file,"Error %s\n", xnee_print_error_code(err_type));
}



/*
 *
 *    xnee_record_print_record_range.  
 *
 */
void 
old_xnee_record_print_record_range (xnee_data *xd, FILE* out) 
{
  int i=0;
  int max=xnee_get_max_range(xd);
    
  if (out == NULL)
    {
      if ( !xd->verbose ) 
	{
	  return ;
	}
      out=xd->err_file;
    }

  fprintf (out, " -------------> MAX = %d <---------- \n", max);

  fprintf (out, "\n\n- Record Information -\n\n");
  fprintf (out, "   Variable              Value\n");
  fprintf (out, "---------------------------------\n");
  fprintf (out, "   data_flags            %d\n", xd->record_setup->data_flags);
  fprintf (out, "   rState                %d\n", (int)xd->record_setup->rState);
  fprintf (out, "   xids[0]               %ld\n", xd->record_setup->xids[0]);
  fprintf (out, "   xids[1]               %ld\n", xd->record_setup->xids[1]);

  fprintf (out, "\n\n      - Record Ranges -\n\n");
  for ( i=0 ; i<max ; i++ )
    {
      fprintf (out, "RecordRange[%d]\n", i);
      fprintf (out, "    Variable              Value\n");
      fprintf (out, "---------------------------------\n");
      fprintf (out, " %s           %d-%d\n", 
	       XNEE_REQUEST_STR,
	       xd->record_setup->range_array[i]->core_requests.first,
	       xd->record_setup->range_array[i]->core_requests.last       );

      fprintf (out, " %s            %d-%d\n", 
	       XNEE_REPLIES_STR,
	       xd->record_setup->range_array[i]->core_replies.first
	       , xd->record_setup->range_array[i]->core_replies.last       );

      fprintf (out, " %s  %d-%d\n", 
	       XNEE_EXT_REQ_MAJ_STR,
	       xd->record_setup->range_array[i]->ext_requests.ext_major.first,
	       xd->record_setup->range_array[i]->ext_requests.ext_major.last        );

      fprintf (out, " %s  %d-%d\n", 
	       XNEE_EXT_REQ_MIN_STR,
	       xd->record_setup->range_array[i]->ext_requests.ext_minor.first,
	       xd->record_setup->range_array[i]->ext_requests.ext_minor.last        );

      fprintf (out, " %s   %d-%d\n", 
	       XNEE_EXT_REP_MAJ_STR,
	       xd->record_setup->range_array[i]->ext_replies.ext_major.first,
	       xd->record_setup->range_array[i]->ext_replies.ext_major.last        );

      fprintf (out, " %s   %d-%d\n", 
	       XNEE_EXT_REP_MIN_STR,
	       xd->record_setup->range_array[i]->ext_replies.ext_minor.first,
	       xd->record_setup->range_array[i]->ext_replies.ext_minor.last        );

      fprintf (out, " %s        %d-%d\n", 
	       XNEE_DELIVERED_EVENT_STR,
	       xd->record_setup->range_array[i]->delivered_events.first,
	       xd->record_setup->range_array[i]->delivered_events.last      );

      fprintf (out, " %s           %d-%d\n", 
	       XNEE_DEVICE_EVENT_STR,
	       xd->record_setup->range_array[i]->device_events.first,
	       xd->record_setup->range_array[i]->device_events.last        );

      fprintf (out, " %s                  %d-%d\n", 
	       XNEE_ERROR_STR,
	       xd->record_setup->range_array[i]->errors.first,
	       xd->record_setup->range_array[i]->errors.last      );
    }
}




/*
 *
 *    xnee_record_print_sys_info.  
 * 
 */
int
xnee_print_sys_info(xnee_data *xd, FILE *out)
{
  struct utsname uname_pointer;
  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  
  uname(&uname_pointer);




  if (( xd->out_file == NULL ) && (xd->mode==XNEE_RECORDER))
    return 0;

  if (out == NULL)
    {
      if ( !xd->verbose ) 
	{
	  return 0;
	}
      if ( xd->out_file == NULL )
	out=xd->err_file;
      
    }

  fprintf (out, "####################################\n");
  fprintf (out, "#         System information       #\n");
  fprintf (out, "####################################\n");
  
  fprintf (out, "# Date:                   %.4d:%.2d:%.2d \n",
	  timeinfo->tm_year + 1900 , 
	   timeinfo->tm_mon + 1 , 
	   timeinfo->tm_mday  );
  fprintf (out, "# Time:                   %.2d:%.2d:%.2d \n",
	  timeinfo->tm_hour, 
	   timeinfo->tm_min , 
	   timeinfo->tm_sec  );
  fprintf (out, "# Xnee version:           %s \n", VERSION);
  fprintf (out, "# Xnee home:              %s \n", XNEE_HOME_URL);
  fprintf (out, "# Xnee info mailing list: %s \n", XNEE_MAIL);
  fprintf (out, "# Xnee bug mailing list:  %s \n", XNEE_BUG_MAIL);
  fprintf (out, "# X version:              %d \n", ProtocolVersion(xd->data));
  fprintf (out, "# X revision:             %d \n", ProtocolRevision(xd->data));
  fprintf (out, "# X vendor:               %s \n", ServerVendor (xd->data));
  fprintf (out, "# X vendor release:       %d \n", VendorRelease (xd->data));
  fprintf (out, "# Record version major:   %d\n", 
	   xd->record_setup->major_return);
  fprintf (out, "# Record version minor:   %d\n", 
	   xd->record_setup->minor_return);
  fprintf (out, "# OS name:                %s \n", uname_pointer.sysname);
  fprintf (out, "# OS Release:             %s \n", uname_pointer.release);
  fprintf (out, "# OS Version:             %s \n", uname_pointer.version);
  fprintf (out, "# Machine:                %s \n", uname_pointer.machine);
  fprintf (out, "# Nodename:               %s \n", uname_pointer.nodename);
  fprintf (out, "# Display name:           %s\n", xd->display);
  fprintf (out, "# Dimension:              %dx%d\n",
	   DisplayWidth (xd->data, 0), 
	   DisplayHeight (xd->data, 0));
  return 0;
}


/*
 *
 * xnee_print_distr_list.  
 *
 */
int 
xnee_print_distr_list (xnee_data* xd, FILE *out)
{
  int i=0;
  if (out == NULL)
    {
      if ( !xd->verbose ) 
	{
	  return 0;
	}
      out=xd->err_file;
    }
  for (i=0;i<xd->distr_list_size;i++)
    {
      fprintf (out, "Distribution display[%d]=%d\n", 
	       i, (int)xd->distr_list[i].dpy);
    }
  return (XNEE_OK);
}


void 
xnee_record_print_record_range (xnee_data *xd, FILE* out) 
{
  int max=0;
  int i ;
  if (out == NULL)
    {
      if ( !xd->verbose ) 
	{
	  return ;
	}
      out=xd->err_file;
    }
  max=xnee_get_max_range(xd);


  fprintf (out, "####################################\n");
  fprintf (out, "#          Record settings         #\n");
  fprintf (out, "####################################\n");
  fprintf (out, "#   data_flags          %d\n", 
	   xd->record_setup->data_flags);
  fprintf (out, "#   rState              %d\n", 
	   (int) xd->record_setup->rState);
  fprintf (out, "#   xids[0]             %d\n", 
	   (int) xd->record_setup->xids[0]);
  fprintf (out, "#   xids[1]             %d\n", 
	   (int) xd->record_setup->xids[1]);
  fprintf (out, "# Number of Ranges      %d\n", 
	   (int) max);


  for ( i=0 ; i<max ; i++ )
    {
      fprintf (out, "# RecordRange[%d]\n", i);
      fprintf (out, XNEE_REQUEST_STR "           %d-%d\n", 
	       xd->record_setup->range_array[i]->core_requests.first,
	       xd->record_setup->range_array[i]->core_requests.last       );

      fprintf (out, XNEE_REPLY_STR  "                   %d-%d \n", 
	       xd->record_setup->range_array[i]->core_replies.first,
	       xd->record_setup->range_array[i]->core_replies.last       );

      fprintf (out, XNEE_EXT_REQ_MAJ_STR "  %d-%d \n", 
	       xd->record_setup->range_array[i]->ext_requests.ext_major.first,
	       xd->record_setup->range_array[i]->ext_requests.ext_major.last);
      
      fprintf (out, XNEE_EXT_REQ_MIN_STR "  %d-%d \n", 
	       xd->record_setup->range_array[i]->ext_requests.ext_minor.first,
	       xd->record_setup->range_array[i]->ext_requests.ext_minor.last);

      fprintf (out, XNEE_EXT_REP_MAJ_STR "   %d-%d\n", 
	       xd->record_setup->range_array[i]->ext_replies.ext_major.first,
	       xd->record_setup->range_array[i]->ext_replies.ext_major.last);

      fprintf (out, XNEE_EXT_REP_MIN_STR "   %d-%d \n", 
	       xd->record_setup->range_array[i]->ext_replies.ext_minor.first,
	       xd->record_setup->range_array[i]->ext_replies.ext_minor.last );

      fprintf (out, XNEE_DELIVERED_EVENT_STR"         %d-%d \n", 
	       xd->record_setup->range_array[i]->delivered_events.first,
	       xd->record_setup->range_array[i]->delivered_events.last  );

      fprintf (out, XNEE_DEVICE_EVENT_STR "            %d-%d \n", 
	       xd->record_setup->range_array[i]->device_events.first,
	       xd->record_setup->range_array[i]->device_events.last    );

      fprintf (out, XNEE_ERROR_STR "                   %d-%d \n", 
	       xd->record_setup->range_array[i]->errors.first,
	       xd->record_setup->range_array[i]->errors.last      );
    }


  return ;
}



/*
 *
 *
 *
 */
void
xnee_print_ranges (xnee_data *xd, FILE *fp)
{
  int max=0;
  int i ;
  
  max=xnee_get_max_range(xd);

  for ( i=0 ; i<max ; i++ )
    {
      if ( ( xd->record_setup->range_array[i]->core_requests.first!=0)
	   &&
	   ( xd->record_setup->range_array[i]->core_requests.last!=0))
	{
	  fprintf (fp, " "XNEE_REQUEST_STR ": %d-%d\n", 
		 xd->record_setup->range_array[i]->core_requests.first,
		 xd->record_setup->range_array[i]->core_requests.last       );
	}
      if ( ( xd->record_setup->range_array[i]->core_replies.first!=0)
	   &&
	   ( xd->record_setup->range_array[i]->core_replies.last!=0))
	{
	  fprintf (fp, "   "  XNEE_REPLY_STR  ":                   %d-%d \n",
		   xd->record_setup->range_array[i]->core_replies.first,
		   xd->record_setup->range_array[i]->core_replies.last       );
	}
      if ( ( xd->record_setup->range_array[i]->ext_requests.ext_major.first!=0)
	   &&
	   ( xd->record_setup->range_array[i]->ext_requests.ext_major.last!=0))
	{
	  fprintf (fp, "   " XNEE_EXT_REQ_MAJ_STR ":  %d-%d \n", 
		   xd->record_setup->range_array[i]->ext_requests.ext_major.first,
		   xd->record_setup->range_array[i]->ext_requests.ext_major.last);
	}
      if ( ( xd->record_setup->range_array[i]->ext_requests.ext_minor.first!=0)
	   &&
	   ( xd->record_setup->range_array[i]->ext_requests.ext_minor.last!=0))
	{
	  fprintf (fp, "   " XNEE_EXT_REQ_MIN_STR ":  %d-%d \n", 
		   xd->record_setup->range_array[i]->ext_requests.ext_minor.first,
		   xd->record_setup->range_array[i]->ext_requests.ext_minor.last        );
	}
      
      if ( ( xd->record_setup->range_array[i]->ext_replies.ext_major.first!=0)
	   &&
	   ( xd->record_setup->range_array[i]->ext_replies.ext_major.last!=0))
	{
	  fprintf (fp, "   " XNEE_EXT_REP_MAJ_STR ":   %d-%d\n", 
		   xd->record_setup->range_array[i]->ext_replies.ext_major.first,
		   xd->record_setup->range_array[i]->ext_replies.ext_major.last        );
	}
      if ( ( xd->record_setup->range_array[i]->ext_replies.ext_minor.first!=0)
	   &&
	   ( xd->record_setup->range_array[i]->ext_replies.ext_minor.last!=0))
	{
	  
	  fprintf (fp, "   " XNEE_EXT_REP_MIN_STR ":   %d-%d \n", 
		   xd->record_setup->range_array[i]->ext_replies.ext_minor.first,
		   xd->record_setup->range_array[i]->ext_replies.ext_minor.last);
	}
      
      if ( ( xd->record_setup->range_array[i]->delivered_events.first!=0)
	   &&
	   ( xd->record_setup->range_array[i]->delivered_events.last!=0))
	{
	  fprintf (fp, "   " XNEE_DELIVERED_EVENT_STR":         %d-%d \n", 
		   xd->record_setup->range_array[i]->delivered_events.first,
		   xd->record_setup->range_array[i]->delivered_events.last      );
	  
	}
      if ( ( xd->record_setup->range_array[i]->device_events.first!=0)
	   &&
	   ( xd->record_setup->range_array[i]->device_events.last!=0))
	{
	  fprintf (fp, "   " XNEE_DEVICE_EVENT_STR ":            %d-%d \n", 
		   xd->record_setup->range_array[i]->device_events.first,
		   xd->record_setup->range_array[i]->device_events.last        );
	}
      if ( ( xd->record_setup->range_array[i]->errors.first!=0)
	   &&
	   ( xd->record_setup->range_array[i]->errors.last!=0))
	{
	  fprintf (fp, "   " XNEE_ERROR_STR ":                   %d-%d \n", 
		   xd->record_setup->range_array[i]->errors.first,
		   xd->record_setup->range_array[i]->errors.last      );
	}
      
    }
}

/*
 *
 *    xnee_print_xnee_resource_settings.  
 * 
 */
void 
xnee_print_xnee_resource_settings (xnee_data* xd, FILE* out) 
{
  time_t rawtime;
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  if (out == NULL)
    {
      if ( !xd->verbose ) 
	{
	  return ;
	}
	out=xd->err_file;
    }

  fprintf (out,  "##########################################\n");
  fprintf (out,  "#                                        #\n");
  fprintf (out,  "#           Xnee resource file           #\n");
  fprintf (out,  "#                                        #\n");
  fprintf (out,  "##########################################\n");
  fprintf (out,  "#                                        #\n");
  fprintf (out,  "# This file is automagically created by  #\n");
  fprintf (out,  "# libxnee.                               #\n");
  fprintf (out,  "# ... edit if you know what you do       #\n");
  fprintf (out,  "#                                        #\n");
  fprintf (out,  "##########################################\n");
  fprintf (out,  "#\n");
  fprintf (out,  "#\n");
  fprintf (out,  "#\n");
  fprintf (out,  "#########################################\n");
  fprintf (out,  "#      Project info     #\n");
  fprintf (out,  "#########################################\n");
  fprintf (out,  "# "XNEE_RES_PROJECT":\t%s\n",
	   xnee_get_project_name(xd));
  fprintf (out,  "# "XNEE_RES_DESCRIPTION":\t%s\n",
	   xnee_get_project_descr(xd));
  fprintf (out,  "#\n");
  fprintf (out,  "#\n");

  fprintf (out,  "#########################################\n");
  fprintf (out,  "#     Creation info     #\n");
  fprintf (out,  "#########################################\n");
  fprintf (out,  "# "XNEE_RES_CREAT_DATE":  \t%s\n",
	   xnee_get_creat_date(xd));
  fprintf (out,  "# "XNEE_RES_CREAT_PROGRAM":\t%s\n",
	   xnee_get_creat_program(xd));
  fprintf (out,  "# "XNEE_RES_CREAT_PROG_VER":\t%s\n",
	   xnee_get_creat_prog_vers(xd));


  fprintf (out,  "#\n");
  fprintf (out,  "#\n");
  fprintf (out,  "#########################################\n");
  fprintf (out,  "#    Last change info   #\n");
  fprintf (out,  "#########################################\n");
  fprintf (out,  "# "XNEE_RES_LASTCHANGE_DATE":\t%s\n",
	   xnee_get_creat_date(xd));
  fprintf (out,  "# "XNEE_RES_LASTCHANGE_PROGRAM":\t%s\n",
	   xnee_get_creat_program(xd));
  fprintf (out,  "# "XNEE_RES_LASTCHANGE_PROG_VER":%s\n",
	   xnee_get_last_prog_vers(xd));


  fprintf (out,  "#\n");
  fprintf (out,  "#\n");
  fprintf (out,  "#########################################\n");
  fprintf (out,  "#      Current info     #\n");
  fprintf (out,  "#########################################\n");
  fprintf (out,  "# CurrentDate:  \t%.4d-%.2d-%.2d \n",
	   timeinfo->tm_year + 1900 , 
	   timeinfo->tm_mon + 1 , 
	   timeinfo->tm_mday  );
  fprintf (out,  "# CurrentProgram:     \t%s\n",
	   PACKAGE);
  fprintf (out,  "# CurrentProgVersion:  \t%s\n",
	   VERSION);
}




/*
 *
 *    xnee_print_xnee_settings.  
 * 
 */
void 
xnee_print_xnee_settings (xnee_data* xd, FILE* out) 
{
  if (out == NULL)
    {
      if ( !xd->verbose ) 
	{
	  return ;
	}
	out=xd->err_file;
    }
  
  
  fprintf (out,  "#\n");
  fprintf (out,  "#\n");
  fprintf (out,  "#\n");
  fprintf (out,  "display %s\n",
	   (xd->display==NULL) ? "NULL"  : xd->display);
  fprintf (out,  XNEE_OUT_FILE "  %s\n",
	   (xd->out_name==NULL) ? "stdout" : xd->out_name );
  fprintf (out,  XNEE_ERR_FILE " %s\n",
	   (xd->err_name==NULL) ? "stderr" : xd->err_name );
  
  /* ****************************** */
  
  fprintf (out,  "%s         %s\n",
	   XNEE_STOP_KEY,
	   xd->grab_keys->stop_str);
  fprintf (out,   "%s        %d,%d\n",
	   XNEE_PAUSE_KEY,
	   xd->grab_keys->pause_mod,
	   xd->grab_keys->pause_key );
  fprintf (out,   "%s       %d,%d\n",
	   XNEE_RESUME_KEY,
	   xd->grab_keys->resume_mod,
	   xd->grab_keys->resume_key );

  fprintf (out,  "%s       %d\n",
	   XNEE_ALL_EVENTS,xd->xnee_info->all_events );
  fprintf (out,  "everything       %d\n",
	   xd->xnee_info->everything );

  fprintf (out,  "%s       %d\n",
	   XNEE_EVENT_MAX,xnee_get_events_left(xd) );
  fprintf (out,  "%s       %d\n",
	   XNEE_DATA_MAX,xnee_get_data_max(xd) );
  fprintf (out,  "%s       %d\n",
	   XNEE_TIME_MAX,xnee_get_time_left(xd) );

  fprintf (out,  "%s        %d\n",
	   XNEE_NO_EXPOSE,xd->xnee_info->no_expose );
  fprintf (out,  "last-motion      %d\n",
	   xd->xnee_info->last_motion ); 
  fprintf (out,  "first-last       %d\n",
	   xd->xnee_info->first_last ); 
}


/*
 *   xnee_print_data_range_count
 *
 *
 * 
 */
int 
xnee_print_data_range_count (xnee_data * xd)
{
  int i=0;
  xnee_verbose((xd, "Data ranges\n-----------\n"));

  for ( i=0 ; i< XNEE_NR_OF_TYPES ; i++) 
    {
      xnee_verbose((xd, "PROT_DATA %d=%d\n", i,xd->xnee_info->data_ranges[i]));
    }
  xnee_verbose((xd, "MAX = %d\n", xnee_get_max_range(xd)));
  return (0);
}



/**************************************************************
 *                                                            *
 * xnee_replay_printbuffer                                    *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_replay_printbuffer_impl (xnee_data *xd )
{
  int i;
  fprint_fptr  fp   = xd->buffer_verbose_fp ;
  FILE        *file = xd->buffer_file       ;
  /*  int **       array= (int **)xd->data_buffer;*/

  if (xd->buf_verbose) 
    {
      fp (file,"\n --- replay buffer ---\n");
      /*
	fp (file,"\t- means from Xserver\n\t+ means from file  \n");
	fp (file,"#     event  request  reply    error \n");
	fp (file,"-------------------------------------\n");
      */
      for (i=0;i<XNEE_REPLAY_BUFFER_SIZE;i++)
	{
	  if (xd->data_buffer[XNEE_EVENT][i] ||
 	      xd->data_buffer[XNEE_REQUEST][i] ||
 	      xd->data_buffer[XNEE_REPLY][i] ||
 	      xd->data_buffer[XNEE_ERROR][i] )
	    fp (file, "%.3d    %.3d     %.3d      %.3d    %.3d\n", i, 
		     xd->data_buffer[XNEE_EVENT][i],
		     xd->data_buffer[XNEE_REQUEST][i],
		     xd->data_buffer[XNEE_REPLY][i],
		     xd->data_buffer[XNEE_ERROR][i]
		     );
	}
      fp (file,"cached: max=%02d  min=%02d total=%02d  s_max=%02d s_min=%02d\n", 
	  xd->meta_data.cached_max,
	  xd->meta_data.cached_min,
	  xd->meta_data.total_diff,
	  xd->meta_data.sum_max,
	  xd->meta_data.sum_min
	  );
      fp (file,"\n");
    }
}


/**************************************************************
 *                                                            *
 * xnee_version                                               *
 *                                                            *
 *                                                            *
 **************************************************************/
void
xnee_version(xnee_data* xd)
{
  xnee_verbose ((xd, " --> xnee_version\n"));
  fprintf (stderr, "%s ",PACKAGE);
  fprintf (stderr, "%s\n",VERSION);
  fprintf (stderr, "Copyright (C) 2000-2003 Henrik Sandklef (%s)\n", XNEE_MAIL);
  fprintf (stderr, "%s and all its included programs come with ", PACKAGE);
  fprintf (stderr, "NO WARRANTY,\nto the extent permitted by law.\n" );
  fprintf (stderr, "This is free software, and you may redistribute\n");
  fprintf (stderr, "copies of it under terms of GNU Genreal Public License.\n");
  fprintf (stderr, "Xnee: Xnee's not an Event Emulator %s \n", XNEE_HOME_URL);
  fprintf (stderr, "GPL:  Gnu General Public License   %s\n",  GNU_HOME_URL);
  fprintf (stderr, "FSF:  Free Software Foundation     %s\n",  FSF_HOME_URL);
  xnee_verbose ((xd, " <-- xnee_version\n"));
}

