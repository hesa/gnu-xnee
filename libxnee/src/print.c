/*
  Xnee's Not an Event Emulator enables recording and replaying of X protocol data
  Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 
                2007 Henrik Sandklef

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 3
  of the License, or any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Boston, MA  02110-1301, USA.
*/



#include "libxnee/xnee.h"
#include "libxnee/print_varargs.h"
#include "libxnee/print.h"
#include "libxnee/datastrings.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"
#include "libxnee/xnee_resource.h"
#include "libxnee/xnee_threshold.h"
#include "libxnee/feedback.h"
#include "libxnee/xnee_error.h"
#include "libxnee/xnee_session.h"




/*
 * 
 * Prints the number and name of the XEvent  
 *
 */
int
xnee_print_event_info (xnee_data *xd) 
{
  int i = 2 ; 
  char *tmp = NULL; 

  fprintf (xd->out_file,"  X11 Event\n\n");
  fprintf (xd->out_file,"number\tname\n");
  fprintf (xd->out_file,"======\t====\n");
  
  tmp = xnee_print_event(i) ; 
  while (tmp!=NULL)
    {
      if (tmp!=NULL)
	{
	  fprintf (xd->out_file,"%.2d\t%s\n",i,tmp);
	}
      i++;
      tmp = xnee_print_event(i) ; 
    }
  return XNEE_OK;
}


/*
 * 
 * Prints the number and name of the X11 error
 *
 */
int
xnee_print_error_info (xnee_data *xd) 
{
  int i = 0 ; 
  char *tmp = NULL; 
  fprintf (xd->out_file,"  X11 Error\n\n");
  fprintf (xd->out_file,"number\tname\n");
  fprintf (xd->out_file,"======\t====\n");
  tmp = xnee_print_error_code(i) ; 
  while (tmp!=NULL)
    {
      if (tmp!=NULL)
	{
	  fprintf (xd->out_file,"%.2d\t%s\n",i,tmp);
	}
      i++;
      tmp = xnee_print_error_code(i) ; 
    }

  return XNEE_OK;
}


/*
 * 
 * Prints the number and name of the X11 error
 *
 */
int
xnee_print_request_info (xnee_data *xd) 
{
  int i=1 ; 
  char *tmp = NULL ; 

  fprintf (xd->out_file,"  X11 Request\n\n");
  fprintf (xd->out_file,"number\tname\n");
  fprintf (xd->out_file,"======\t====\n");
  tmp = xnee_print_request(i); 
  while (tmp!=NULL)
    {
      if (tmp!=NULL)
	{
	  fprintf (xd->out_file,"%.2d\t%s\n",i,tmp);
	}
      i++;
      tmp = xnee_print_request(i); 
    }

  return XNEE_OK;
}

/*
 * 
 * Prints the number and name of the X11 reply
 *
 */
int
xnee_print_reply_info (xnee_data *xd) 
{
  (void) fprintf (xd->out_file,"  X11 Reply\n\n");
  return XNEE_OK;
}

/*
 * 
 * Prints the number and name of the X11 data
 *
 */
int
xnee_print_data_info (xnee_data *xd) 
{
  int ret ; 

  ret = xnee_print_event_info (xd) ;
  XNEE_RETURN_IF_ERR(ret);

  ret = xnee_print_error_info (xd) ;
  XNEE_RETURN_IF_ERR(ret);

  ret = xnee_print_request_info (xd) ;
  XNEE_RETURN_IF_ERR(ret);

  ret = xnee_print_reply_info (xd) ;
  XNEE_RETURN_IF_ERR(ret);
   
  return XNEE_OK;
}





  
/*
 * Print character representation of event if verbose mode is on.  
 */
void 
xnee_verbose_event (xnee_data * xd, int ev ) {
  if ( xd->verbose != 0 ) 
    {
      (void)xnee_print_event (ev);
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
  XRecordDatum *xrec_data  ;
  int           req_type    ;
  xResourceReq  *req;

  if ( xrecintd == NULL)
    {
      return;
    }

  xrec_data =  (XRecordDatum *) xrecintd->data ;
  req_type  =  (int)            xrec_data->type ;
  req       =  (xResourceReq *) &xrec_data->req ;
  (void) xd->data_fp (xd->out_file,"1,%d,%lu,%lu,%lu,%lu\n", 
		      req_type, 
		      req->reqType,
		      req->length,
		      req->id,
		      xrecintd->server_time);
}



/*
 *
 * xnee_human_print_request.  
 *
 */
void 
xnee_human_print_request (xnee_data *xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data;
  int           req_type ;

  xrec_data = (XRecordDatum *) (xrecintd->data) ;
  req_type  = (int) xrec_data->type ;
  
  (void)xd->data_fp (xd->out_file,"Request   %20s\t%.3d\t%lu\n", 
		     xnee_print_request(req_type),
		     req_type,
		     xrec_data->req.id    );
}

/*
 *
 * xnee_human_print_event.  
 *
 */
void 
xnee_human_print_event (xnee_data *xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data  ;
  int           event_type ;

  xrec_data  = (XRecordDatum *) (xrecintd->data) ;
  event_type = (int)xrec_data->type ;

  switch (event_type)
    {
    case CreateNotify:
      (void)xd->data_fp (xd->out_file,"Event     %20s\t%.3d\t%lu\n", 
			 xnee_print_event(event_type), 
			 event_type,
			 xrec_data->event.u.createNotify.window);
      break;
    default:
      (void)xd->data_fp (xd->out_file,"Event     %20s\t%.3d\n", 
			 xnee_print_event(event_type), event_type);
      break;
    }
}



/*
 *
 *   xnee_record_print_reply.  
 *
 */
void xnee_record_print_reply (xnee_data* xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data ;
  int           rep_type   ;

  xrec_data = (XRecordDatum *) (xrecintd->data) ;
  rep_type  = (int)xrec_data->type ;
  
  (void)xd->data_fp (xd->out_file,"2,%d,%lu\n", rep_type,xrecintd->server_time);
}


/*
 *
 *   xnee_human_print_reply.  
 *
 */
void xnee_human_print_reply (xnee_data* xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data  ;
  int           rep_type   ;

  xrec_data = (XRecordDatum *) (xrecintd->data) ;
  rep_type  = (int)xrec_data->type ;
  
  (void)xd->data_fp (xd->out_file,"Reply       %d,%lu\n", rep_type,xrecintd->server_time);
}




/*
 *
 *   xnee_record_print_error.  
 *
 */
void xnee_record_print_error (xnee_data *xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data ;
  int           err_type  ;
  
  xrec_data = (XRecordDatum *) (xrecintd->data) ;
  err_type  = (int)xrec_data->type ;
  
  (void)xd->data_fp (xd->out_file,"3,%d,%lu\n", err_type,xrecintd->server_time);
}

/*
 *
 *   xnee_human_print_error.  
 *
 */
void xnee_human_print_error (xnee_data *xd, XRecordInterceptData *xrecintd )
{
  XRecordDatum *xrec_data ;
  int           err_type  ;
  
  xrec_data = (XRecordDatum *) (xrecintd->data) ;
  err_type = (int) xrec_data->type ;
  
  (void)xd->data_fp (xd->out_file,"Error %s\n", xnee_print_error_code(err_type));
}


#ifdef USE_OBSOLETE
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
#endif



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
  time_t ret_time;
  struct tm * timeinfo;
  int    ret ;


  if ( (xd==NULL) || (xd->data==NULL) )
    {
      return XNEE_NO_MAIN_DATA;
    }
  if (xd->record_setup==NULL)
    {
      return XNEE_NO_RECORD_DATA;
    }

  


  ret_time = time ( &rawtime );
  if ( ret_time != rawtime)
    {
      return XNEE_DATE_FAILURE;
    }

  timeinfo = localtime ( &rawtime );
  if (timeinfo == NULL)
    {
      return XNEE_DATE_FAILURE;
    }
  
  ret = uname(&uname_pointer);
  if (ret == -1)
    {
      return XNEE_DATE_FAILURE;
    }




  if (( xd->out_file == NULL ) && (xd->mode==XNEE_RECORDER))
    {
      return XNEE_OK;
    }

  if (out == NULL)
    {
      if ( xd->verbose == 0 ) 
	{
	  return XNEE_OK;
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
  if ( xd->program_name!=NULL)
    {
      fprintf (out, "# Xnee program:           %s \n", xd->program_name);
    }
  else
    {
      fprintf (out, "# Xnee program:           <missing info> \n");
    }
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
  return XNEE_OK;
}


/*
 *
 * xnee_print_distr_list.  
 *
 */
int 
xnee_print_distr_list (xnee_data* xd, /*@null@*/ FILE *out)
{
  int i=0;

  if (xd==NULL) 
    {
      return XNEE_NO_MAIN_DATA;
    }

  if (xd->distr_list==NULL)
    {
      return XNEE_OK;
    }


  if (out == NULL)
    {
      if ( xd->verbose == 0 ) 
	{
	  return 0;
	}
      out=xd->err_file;
    }
  for (i=0 ; i<(int)xd->distr_list_size ; i++)
    {
      fprintf (out, "Distribution display[%d]=%d\n", 
	       i, (int)xd->distr_list[i].dpy);
    }
  return (XNEE_OK);
}


int
xnee_record_print_record_range (xnee_data *xd, /*@null@*/ FILE* out) 
{
  int max=0;
  int i ;


  if ( (xd==NULL) || 
       (xd->record_setup==NULL) ||
       (xd->record_setup->range_array==NULL) ||
       (xd->record_setup->xids==NULL) )
    {
      return XNEE_NO_MAIN_DATA; 
    }

  if (out == NULL)
    {
      if ( xd->verbose == 0 ) 
	{
	  return XNEE_OK;
	}
      out=xd->err_file;
    }
  max=xnee_get_max_range(xd);


  fprintf (out, "#########################################\n");
  fprintf (out, "#          Record settings              #\n");
  fprintf (out, "#########################################\n");
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
      fprintf (out, "%s            %u-%u\n", 
	       xnee_xns_key2string(XNEE_REQUEST_STR_KEY),
	       xd->record_setup->range_array[i]->core_requests.first,
	       xd->record_setup->range_array[i]->core_requests.last       );

      fprintf (out, "%s                   %u-%u \n", 
	       xnee_xns_key2string(XNEE_REPLY_STR_KEY),
	       xd->record_setup->range_array[i]->core_replies.first,
	       xd->record_setup->range_array[i]->core_replies.last       );

      fprintf (out, "%s  %u-%u \n", 
	       xnee_xns_key2string(XNEE_EXT_REQ_MAJ_STR_KEY),
	       xd->record_setup->range_array[i]->ext_requests.ext_major.first,
	       xd->record_setup->range_array[i]->ext_requests.ext_major.last);
      
      fprintf (out, "%s  %u-%u \n", 
	       xnee_xns_key2string(XNEE_EXT_REQ_MIN_STR_KEY),
	       xd->record_setup->range_array[i]->ext_requests.ext_minor.first,
	       xd->record_setup->range_array[i]->ext_requests.ext_minor.last);

      fprintf (out, "%s   %u-%u\n", 
	       xnee_xns_key2string(XNEE_EXT_REP_MAJ_STR_KEY),
	       xd->record_setup->range_array[i]->ext_replies.ext_major.first,
	       xd->record_setup->range_array[i]->ext_replies.ext_major.last);

      fprintf (out, "%s   %u-%u \n", 
	       xnee_xns_key2string(XNEE_EXT_REP_MIN_STR_KEY),
	       xd->record_setup->range_array[i]->ext_replies.ext_minor.first,
	       xd->record_setup->range_array[i]->ext_replies.ext_minor.last );

      fprintf (out, "%s         %u-%u \n", 
	       xnee_xns_key2string(XNEE_DELIVERED_EVENT_STR_KEY),
	       xd->record_setup->range_array[i]->delivered_events.first,
	       xd->record_setup->range_array[i]->delivered_events.last  );

      fprintf (out, "%s            %u-%u \n", 
	       xnee_xns_key2string(XNEE_DEVICE_EVENT_STR_KEY),
	       xd->record_setup->range_array[i]->device_events.first,
	       xd->record_setup->range_array[i]->device_events.last    );

      fprintf (out, "%s                   %u-%u \n", 
	       xnee_xns_key2string(XNEE_ERROR_STR_KEY),
	       xd->record_setup->range_array[i]->errors.first,
	       xd->record_setup->range_array[i]->errors.last      );
    }


  return XNEE_OK;
}



/*
 *
 *
 *
 */
int
xnee_print_ranges (xnee_data *xd, FILE *fp)
{
  int max=0;
  int i ;
  

  if ( (xd==NULL) || 
       (xd->record_setup==NULL) ||
       (xd->record_setup->range_array==NULL) )
    {
      return XNEE_NO_MAIN_DATA;
    }



  max=xnee_get_max_range(xd);

  fprintf (fp, "#########################################\n");
  fprintf (fp, "#          Recording ranges             #\n");
  fprintf (fp, "#########################################\n");
  for ( i=0 ; i<max ; i++ )
    {
  
      fprintf (fp, "\n# Range nr:%d\n", i);

      if ( ( (int)xd->record_setup->range_array[i]->core_requests.first != 0)
	   &&
	   ( (int)xd->record_setup->range_array[i]->core_requests.last != 0))
	{
	  fprintf (fp,  "%s %u-%u\n", 
		   xnee_xns_key2string(XNEE_REQUEST_STR_KEY),
                   xd->record_setup->range_array[i]->core_requests.first,
                   xd->record_setup->range_array[i]->core_requests.last       );
	}
      if ( ( (int)xd->record_setup->range_array[i]->core_replies.first != 0)
	   &&
	   ( (int)xd->record_setup->range_array[i]->core_replies.last != 0))
	{
	  fprintf (fp,   "%s                   %u-%u \n",
		   xnee_xns_key2string(XNEE_REPLY_STR_KEY),
		   xd->record_setup->range_array[i]->core_replies.first,
		   xd->record_setup->range_array[i]->core_replies.last       );
	}
      if ( ( (int)xd->record_setup->range_array[i]->ext_requests.ext_major.first != 0)
	   &&
	   ( (int)xd->record_setup->range_array[i]->ext_requests.ext_major.last != 0))
	{
	  fprintf (fp, "%s  %u-%u \n", 
		   xnee_xns_key2string(XNEE_EXT_REQ_MAJ_STR_KEY),
		   xd->record_setup->range_array[i]->ext_requests.ext_major.first,
		   xd->record_setup->range_array[i]->ext_requests.ext_major.last);
	}
      if ( ( (int)xd->record_setup->range_array[i]->ext_requests.ext_minor.first != 0)
	   &&
	   ( (int)xd->record_setup->range_array[i]->ext_requests.ext_minor.last != 0))
	{
	  fprintf (fp, "%s  %u-%u \n", 
		   xnee_xns_key2string(XNEE_EXT_REQ_MIN_STR_KEY),
		   xd->record_setup->range_array[i]->ext_requests.ext_minor.first,
		   xd->record_setup->range_array[i]->ext_requests.ext_minor.last        );
	}
      
      if ( ( (int)xd->record_setup->range_array[i]->ext_replies.ext_major.first != 0)
	   &&
	   ( (int)xd->record_setup->range_array[i]->ext_replies.ext_major.last != 0))
	{
	  fprintf (fp,  "%s   %u-%u\n", 
		   xnee_xns_key2string(XNEE_EXT_REP_MAJ_STR_KEY),
		   xd->record_setup->range_array[i]->ext_replies.ext_major.first,
		   xd->record_setup->range_array[i]->ext_replies.ext_major.last        );
	}
      if ( ( (int)xd->record_setup->range_array[i]->ext_replies.ext_minor.first != 0)
	   &&
	   ( (int)xd->record_setup->range_array[i]->ext_replies.ext_minor.last != 0))
	{
	  
	  fprintf (fp,  "%s   %u-%u \n", 
		   xnee_xns_key2string(XNEE_EXT_REP_MIN_STR_KEY),
		   xd->record_setup->range_array[i]->ext_replies.ext_minor.first,
		   xd->record_setup->range_array[i]->ext_replies.ext_minor.last);
	}
      
      if ( ( (int)xd->record_setup->range_array[i]->delivered_events.first != 0)
	   &&
	   ( (int)xd->record_setup->range_array[i]->delivered_events.last != 0))
	{
	  fprintf (fp, "%s         %u-%u \n", 
		   xnee_xns_key2string(XNEE_DELIVERED_EVENT_STR_KEY),
		   xd->record_setup->range_array[i]->delivered_events.first,
		   xd->record_setup->range_array[i]->delivered_events.last      );
	  
	}
      if ( ( (int)xd->record_setup->range_array[i]->device_events.first != 0)
	   &&
	   ( (int)xd->record_setup->range_array[i]->device_events.last != 0))
	{
	  fprintf (fp, "%s            %u-%u \n", 
		   xnee_xns_key2string(XNEE_DEVICE_EVENT_STR_KEY),
		   xd->record_setup->range_array[i]->device_events.first,
		   xd->record_setup->range_array[i]->device_events.last        );
	}
      if ( ( (int) xd->record_setup->range_array[i]->errors.first != 0)
	   &&
	   ( (int) xd->record_setup->range_array[i]->errors.last != 0))
	{
	  fprintf (fp,  "%s                   %u-%u \n", 
		   xnee_xns_key2string(XNEE_ERROR_STR_KEY),
		   xd->record_setup->range_array[i]->errors.first,
		   xd->record_setup->range_array[i]->errors.last      );
	}
      
    }
  return XNEE_OK;
}

/*
 *
 *    xnee_print_xnee_resource_settings.  
 * 
 */
int
xnee_print_xnee_resource_settings (xnee_data* xd, FILE* out) 
{
  time_t rawtime;
  time_t ret_time;
  struct tm * timeinfo;





  if ( (xd==NULL) || (xd->data==NULL) )
    {
      return XNEE_NO_MAIN_DATA;
    }
  if (xd->record_setup==NULL)
    {
      return XNEE_NO_RECORD_DATA;
    }

  


  ret_time = time ( &rawtime );
  if ( ret_time != rawtime)
    {
      return XNEE_DATE_FAILURE;
    }

  timeinfo = localtime ( &rawtime );
  if (timeinfo == NULL)
    {
      return XNEE_DATE_FAILURE;
    }
  
  if (out == NULL)
    {
      if ( xd->verbose == 0 ) 
	{
	  return XNEE_OK;
	}
      out=xd->err_file;
    }



  fprintf (out,  "###############################################\n");
  fprintf (out,  "#                                             #\n");
  fprintf (out,  "#            Xnee Project file                #\n");
  fprintf (out,  "#                                             #\n");
  fprintf (out,  "#                                             #\n");
  fprintf (out,  "#  This file is automagically created by      #\n");
  fprintf (out,  "#  libxnee.... edit if you know what you do   #\n");
  fprintf (out,  "#                                             #\n");
  fprintf (out,  "###############################################\n");
  fprintf (out,  "\n");
  fprintf (out,  "\n");
  fprintf (out,  "\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Project info                          #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  XNEE_RES_PROJECT":  \t\t\t%s\n",
	   xnee_get_project_name(xd));
  fprintf (out,  XNEE_RES_DESCRIPTION":\t\t%s\n",
	   xnee_get_project_descr(xd));

  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#     Creation info                          #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  XNEE_RES_CREAT_DATE":    \t%s\n",
	   xnee_get_creat_date(xd));
  fprintf (out,  XNEE_RES_CREAT_PROGRAM": \t%s\n",
	   xnee_get_creat_program(xd));
  fprintf (out,  XNEE_RES_CREAT_PROG_VER":  \t%s\n",
	   xnee_get_creat_prog_vers(xd));


  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#    Last change info                        #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  XNEE_RES_LASTCHANGE_DATE":  \t%s\n",
	   xnee_get_last_date(xd));
  fprintf (out,  XNEE_RES_LASTCHANGE_PROGRAM":\t%s\n",
	   xnee_get_last_program(xd));
  fprintf (out,  XNEE_RES_LASTCHANGE_PROG_VER":\t%s\n",
	   xnee_get_last_prog_vers(xd));


  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Current info                          #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  XNEE_RES_CURRENT_CHANGE_DATE":  \t%.4d-%.2d-%.2d \n",
	   timeinfo->tm_year + 1900 , 
	   timeinfo->tm_mon + 1 , 
	   timeinfo->tm_mday  );
  fprintf (out,  XNEE_RES_CURRENT_CHANGE_PROGRAM":     \t%s\n",
	   PACKAGE);
  fprintf (out,  XNEE_RES_CURRENT_CHANGE_PROG_VER":  \t%s\n",
	   VERSION);
  return XNEE_OK;
}


/*
 *
 *    xnee_print_xnee_settings.  
 * 
 */
int
xnee_print_xnee_settings (xnee_data* xd, /*@null@*/ FILE* out) 
{
  int ret;
  int i ;
   
  if (out == NULL)
    {
      if ( xd->verbose == 0 ) 
	{
	  return XNEE_OK;
	}
      out=xd->err_file;
    }
  
  if ( (xd==NULL) || 
       (xd->grab_keys==NULL) ||
       (xd->grab_keys->action_keys==NULL) )
    {
      return XNEE_NO_MAIN_DATA;
    }
  fprintf (out,  "\n");

  if ( xd->app_args!= NULL)
    {
      /* Xnee application argument */
      fprintf (out,  "\n\n");
      fprintf (out,  "##############################################\n");
      fprintf (out,  "#      Xnee application arguments            #\n");
      fprintf (out,  "##############################################\n");
      fprintf (out,  "#  ");
      for (i=0;xd->app_args[i]!=NULL;i++)
	{
	  if ( xd->app_args[i] != NULL)
	    {
	      fprintf (out,  "%s ", xd->app_args[i]);
	    }
	}
      fprintf (out,  "\n");
    }
  
  /* DISPLAY */
  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Displays                              #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "# display %s\n",
	   (xd->display==NULL) ? "NULL"  : xd->display);
  fprintf (out,  "# %s\n" , xnee_xns_key2string(XNEE_DISTRIBUTE_KEY));
  ret = xnee_print_distr_list(xd, out);
  XNEE_RETURN_IF_ERR(ret);


  /* FILES */
  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Files                                 #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,   "# %s %s\n",
	   xnee_xns_key2string(XNEE_OUT_FILE_KEY),
	   (xd->out_name==NULL) ? "stdout" : xd->out_name );
  fprintf (out,   "# %s %s\n",
	   xnee_xns_key2string(XNEE_ERR_FILE_KEY),
	   (xd->err_name==NULL) ? "stderr" : xd->err_name );
  
  /* GRAB */
  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Key Grabs                             #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "# %s         %s\n",
	   xnee_xns_key2string(XNEE_STOP_KEY_KEY),  
	   (xd->grab_keys->action_keys[XNEE_GRAB_STOP].str) ? xd->grab_keys->action_keys[XNEE_GRAB_STOP].str: "0" );
  fprintf (out,   "# %s        %s\n",
	   xnee_xns_key2string(XNEE_PAUSE_KEY_KEY),
	   (xd->grab_keys->action_keys[XNEE_GRAB_PAUSE].str) ? xd->grab_keys->action_keys[XNEE_GRAB_PAUSE].str: "0" );
  fprintf (out,   "# %s       %s\n",
	   xnee_xns_key2string(XNEE_RESUME_KEY_KEY),
	   (xd->grab_keys->action_keys[XNEE_GRAB_RESUME].str) ? xd->grab_keys->action_keys[XNEE_GRAB_RESUME].str: "0" );
  fprintf (out,   "# %s       %s\n",
	   xnee_xns_key2string(XNEE_INSERT_KEY_KEY),
	   (xd->grab_keys->action_keys[XNEE_GRAB_INSERT].str) ? xd->grab_keys->action_keys[XNEE_GRAB_INSERT].str: "0" );
  fprintf (out,   "# %s         %s\n",
	   xnee_xns_key2string(XNEE_EXEC_KEY_KEY),
	   (xd->grab_keys->action_keys[XNEE_GRAB_EXEC].str) ? xd->grab_keys->action_keys[XNEE_GRAB_EXEC].str: "0" );
  fprintf (out,   "# %s         %s\n",
	   xnee_xns_key2string(XNEE_EXEC_PROGRAM_KEY),
	   (xd->grab_keys->action_keys[XNEE_GRAB_EXEC].extra_str) 
	   ? xd->grab_keys->action_keys[XNEE_GRAB_EXEC].extra_str: 
	   XNEE_EXEC_NO_PROG );
  
  /* Limits */
  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Recording limits etc                  #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "\n");
  fprintf (out,  "%s        %d\n",
	   xnee_xns_key2string(XNEE_EVENT_MAX_KEY),xnee_get_events_max(xd) );
  fprintf (out,  "%s          %d\n",
	   xnee_xns_key2string(XNEE_DATA_MAX_KEY),xnee_get_data_max(xd) );
  fprintf (out,  "%s       %d\n",
	   xnee_xns_key2string(XNEE_TIME_MAX_KEY),xnee_get_time_left(xd) );
  fprintf (out,  "first-last       %d\n",
	   xd->xnee_info.first_last ); 

  fprintf (out,  "\n# Record  all (including current) clients or only future ones\n");
  if (xnee_get_all_clients(xd)==0)
    {
      fprintf (out,  "# ");
    }
  fprintf (out,  "%s\n",
           xnee_xns_key2string(XNEE_ALL_CLIENTS_KEY));
  if (xnee_get_all_clients(xd)!=0)
    {
      fprintf (out,  "# ");
    }
  fprintf (out,  "%s\n",
           xnee_xns_key2string(XNEE_FUTURE_CLIENTS_KEY));

  fprintf (out,  "\n# Store the starting mouse position \n");
  if (xnee_is_store_mouse_pos(xd)==0)
    {
      fprintf (out,  "# ");
    }
  fprintf (out,  "%s\n",
	   xnee_xns_key2string(XNEE_STORE_MOUSE_POS_KEY));
  
  
  /* Resolution */
  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Resolution                            #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "\n# Resolution\n");
  fprintf (out,  "#" "%s  %dx%d\n",
	   xnee_xns_key2string(XNEE_RECORDED_RESOLUTION_KEY),
	   xnee_get_rec_resolution_x(xd),
	   xnee_get_rec_resolution_y(xd)  ); 
  fprintf (out,  "#" "%s  %dx%d\n",
	   xnee_xns_key2string(XNEE_REPLAY_RESOLUTION_KEY),
	   xnee_get_rep_resolution_x(xd),
	   xnee_get_rep_resolution_y(xd)  ); 
  fprintf (out,  "#" "%s  %d\n",
	   xnee_xns_key2string(XNEE_ADJUST_RESOLUTION_KEY),
	   xnee_get_resolution_used (xd));


  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Speed                                 #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "\n# Speed\n");
  fprintf (out,  "#" "%s  %d\n",
	   xnee_xns_key2string(XNEE_SPEED_PERCENT_KEY),
	   xnee_get_replay_speed(xd)); 
  

  /* Limits */
  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Replaying limits etc                  #\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "\n%s %d \n", 
           xnee_xns_key2string(XNEE_MAX_THRESHOLD_KEY),
           xnee_get_max_threshold(xd));
  fprintf (out,  "%s %d \n", 
           xnee_xns_key2string(XNEE_MIN_THRESHOLD_KEY),
           xnee_get_min_threshold(xd));
  fprintf (out,  "%s %d \n", 
           xnee_xns_key2string(XNEE_TOT_THRESHOLD_KEY),
           xnee_get_tot_threshold(xd));
  

  /* Feedback */
  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Feedback                              #\n");
  fprintf (out,  "##############################################\n");
  if (!xnee_fb_no_requested(xd) )
    {
      fprintf (out, "#");
    }
  fprintf (out, "%s\n", xnee_xns_key2string(XNEE_FEEDBACK_NONE_KEY));

  if (!xnee_fb_stderr_requested(xd))
    {
      fprintf (out, "#");
    }
  fprintf (out, "%s\n", xnee_xns_key2string(XNEE_FEEDBACK_STDERR_KEY));

  if (!xnee_fb_xosd_requested(xd))
    {
      fprintf (out, "#");
    }
  fprintf (out, "%s\n", xnee_xns_key2string(XNEE_FEEDBACK_XOSD_KEY));

  fprintf (out, "%s %s\n", 
	   xnee_xns_key2string(XNEE_XOSD_FONT_KEY),
	   EMPTY_IF_NULL((char*)xnee_get_xosd_font(xd)));


  fprintf (out,  "\n\n");
  fprintf (out,  "##############################################\n");
  fprintf (out,  "#      Various                               #\n");
  fprintf (out,  "##############################################\n");
  /* Plugin */
  fprintf (out,  "\n# Plugin file \n");
  if (xd->plugin_name==NULL)
    {
      fprintf (out,  "# ");
    }      
  fprintf (out,  "%s       %s\n",
	   xnee_xns_key2string(XNEE_PLUGIN_KEY),
	   xd->plugin_name ); 
  
  /* Modes */
  fprintf (out,  "\n# Modes (currently not used)\n");
  fprintf (out,    "#%s       %d\n",
	   xnee_xns_key2string(XNEE_SYNC_MODE_KEY),
	   xd->sync ); 

  /* Replay offset */
  fprintf (out,  "\n# Replay offset\n");
  fprintf (out,    "#%s %dx%d\n",
	   xnee_xns_key2string(XNEE_REPLAY_OFFSET_KEY),
	   xnee_get_replay_offset_x(xd),
	   xnee_get_replay_offset_y(xd)); 

  fprintf (out,  "\n# Human printout of X11 data (instead of Xnee format)\n");
  if (!xnee_is_human_printout(xd))
    {
      fprintf (out,  "# ");
    }      
  fprintf (out,  "%s  \n",
	   xnee_xns_key2string(XNEE_HUMAN_PRINTOUT_KEY)); 

  
  fprintf (out,  "\n# Delay before starting record/replay\n");
  fprintf (out,  "# %s %d\n", 
	   xnee_xns_key2string(XNEE_DELAY_TIME_KEY),
	   xnee_get_interval(xd) ); 

  

  /* Various */
  fprintf (out,  "\n# Various\n");
  return XNEE_OK;
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
      xnee_verbose((xd, "PROT_DATA %d=%d\n", i,xd->xnee_info.data_ranges[i]));
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

  if (xd->buf_verbose!=0) 
    {
      (void)fp (file,"\n --- replay buffer ---\n");
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



/*
 *
 *   
 *
 */
void 
xnee_store_mouse_pos (xnee_data* xd )
{
  int rx;
  int ry;
  int wx;
  int wy;
  Window root;
  Window child;
  unsigned int mask;
  Bool bo;

  bo = XQueryPointer(xd->data,
		     RootWindow(xd->data,0),
		     &root,
		     &child,
		     &rx,
		     &ry,
		     &wx,
		     &wy,
		     &mask);
  
  xd->data_fp (xd->out_file, 
	       "%s " 
	       "%s=%d " 
	       "%s=%d \n" ,
	       XNEE_FAKE_MOTION,
	       XNEE_FAKE_X_ARG, rx, 
	       XNEE_FAKE_Y_ARG, ry );
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
  fprintf (stderr, "%s %s\n",PACKAGE, VERSION);
  fprintf (stderr, "Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007 Henrik Sandklef \n");
  fprintf (stderr, "%s and all its included programs come with ", PACKAGE);
  fprintf (stderr, "NO WARRANTY,\nto the extent permitted by law.\n" );
  fprintf (stderr, "This is free software, and you may redistribute\n");
  fprintf (stderr, "copies of it under terms of GNU General Public License.\n");
  /*
  fprintf (stderr, "For questions: \n\t%s  \n", XNEE_MAIL);
  fprintf (stderr, "Xnee, Xnee's not an Event Emulator \n\t%s \n", XNEE_HOME_URL);
  fprintf (stderr, "GPL, GNU General Public License \n\t%s\n",  GNU_HOME_URL);
  */
  xnee_verbose ((xd, " <-- xnee_version\n"));
}



int
xnee_print_xnee_data(xnee_data *xd)
{

  fprintf (stderr, "xnee_data\n");
  fprintf (stderr, "  xd:          %d\n", xd);
  fprintf (stderr, "  xd->data:    %d\n", xd->data);
  fprintf (stderr, "  xd->control: %d\n", xd->control);
  fprintf (stderr, "  xd->fake:    %d\n", xd->fake);
  fprintf (stderr, "  xd->grab:    %d\n", xd->grab);
  
}



int
xnee_print_data_str(xnee_data *xd, char *tmp, int type)
{
  char *data;
  int ret = XNEE_OK;
  int data_nr;

  XNEE_VERBOSE_ENTER_FUNCTION();

  if ( (tmp[0] >= '0') && (tmp[0] <= '9') )
    {
      /* We have data as a number */
      if (sscanf(tmp, "%d", &data_nr) != 1 )
	{
	  ret = XNEE_WRONG_PARAMS;
	}
      else
	{
	  data = (char*) xnee_int2data (type, data_nr);
	  if ( data != NULL)
	    {
	      fprintf (stdout, "%s\n", data);
	    }
	  else
	    {
	      ret = XNEE_WRONG_PARAMS;
	    }
	}
    }
  else
    {
      /* We have data as a string */
      data = tmp;
      data_nr = xnee_data2int(type, tmp);
      if ( data_nr == -1)
	{
	  ret = XNEE_WRONG_PARAMS;
	}
      else
	{
	  fprintf (stdout, "%d\n", data_nr);
	}
    }
  XNEE_VERBOSE_LEAVE_FUNCTION();

  return ret;
}
