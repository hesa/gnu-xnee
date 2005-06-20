/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004 Henrik Sandklef 
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

#include "libxnee/xnee.h"
#include "libxnee/xnee_internal.h"
#include "libxnee/xnee_error.h"
#include "libxnee/print_varargs.h"
#include "libxnee/xnee_range.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_resource.h"
#include "libxnee/xnee_settings.h"
#include "libxnee/xnee_utils.h"



/**************************************************************
 *                                                            *
 * xnee_write_settings_to_file                                *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_write_settings_to_file (xnee_data *xd, FILE *fp) 
{
   int ret;
   
   ret = xnee_set_ranges(xd);
   XNEE_RETURN_IF_ERR(ret); 

   xnee_print_xnee_resource_settings (xd, fp) ;
   xnee_print_xnee_settings (xd, fp) ;
   xnee_print_ranges (xd, fp);
   return XNEE_OK;
}




/**************************************************************
 *                                                            *
 * xnee_delay                                                 *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_delay (int secs, char *mess) 
{
  int i;
  int j;
  int bytes_written = 0 ;
  int last_written  = 0 ;
  size_t len = strlen (mess);
  unsigned int sleep_int = 1 ; 
  unsigned int ret;

  for ( i=secs ; i>-1 ; i-- ) 
    {
      if (len>0)
	{
           (void) fflush(stderr);
           last_written= bytes_written  ;
           bytes_written = fprintf (stderr,
				    "\r%s delayed start, time left: "
				    "%d seconds left", mess, i );
	   (void) fflush(stderr);
           for ( j=0 ; j < ( last_written -bytes_written) ; j++ ) 
	    {
               fprintf (stderr," \b");
	    }
	   (void) fflush(stderr);
	}
      ret = sleep (sleep_int);
      if (ret!=0)
      {
         sleep_int = sleep_int + ret;
      }
      else
      {
         sleep_int = 1 ;
      }
    }


  fprintf (stderr,"\r");
  for ( j=0 ; j < bytes_written ; j++ ) 
    {
      fprintf (stderr, " ");
    }
  /* We can typecast these to void safely, since this is not
   * in any way an obstacle if it fails.... */
  (void) fprintf (stderr, "\r");
  (void) fflush(stderr);
}





/**************************************************************
 *                                                            *
 * xnee_strip                                                 *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_strip(xnee_data *xd, char *str) 
{
  int i;
  int j;
  int blanks=0;
  size_t len  ;
  len = strlen (str);

  xnee_verbose ((xd," --> xnee_strip \"%s\"\n", str));
  /*
   * how many blanks do we have  */
  for (i=0 ; i<=(int)len ; i++)
    {
      if  (str[i]==' ' ) 
	{
	  blanks++;
	  for (j=i ; j<=(int)len ; j++)
	    {
	      str[j]=str[j+1];
	      len--;
	    }
	  i--;
	}
    }
  str[len-blanks]='\0';
  xnee_verbose ((xd," <-- xnee_strip \"%s\"\n", str));
  return 1;
} 




/**************************************************************
 *                                                            *
 * xnee_rem_comment_start                                     *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_rem_comment_start(xnee_data *xd, char *str) 
{
  int  i ;
  int  j ;
  int  comms=0;
  size_t len  ;

  if (str==NULL)
    return 1;

  xnee_verbose ((xd," --> xnee_rem_comment_start \"%s\"\n", str));


  len = strlen (str);
  
  /*
   * how many comments do we have  */
  for (i=0 ; i<=(int)len ; i++)
    {
      if ( str[i]==XNEE_COMMENT_START_CHAR )
	{
	  comms++;
	  for (j=i ; j<=(int)len ; j++)
	    {
	      str[j]=str[j+1];
	    }
	  i--;
	}
    }

  str[len-comms]='\0';
  xnee_verbose ((xd," <-- xnee_rem_comment_start \"%s\"\n", str));
  return 0;
} 






/**************************************************************
 *                                                            *
 * rem_blanks                                                 *
 *                                                            *
 *                                                            *
 **************************************************************/
int
rem_blanks (char *array, int size)
{
  int i=0;
  int j=0;
  for (i=0;i<size;i++)
    {
      if ( (array[i]==' ') ||  (array[i]=='\t') ||  (array[i]=='\n') )
	{
	  for (j=i;j<size-1;j++)
	    {
	      array[j]=array[j+1];
	    }
	  array[j]='\0';
	}
    }
  return XNEE_OK;
}


/**************************************************************
 *                                                            *
 * rem_begin_blanks                                           *
 *                                                            *
 *                                                            *
 **************************************************************/
int
rem_begin_blanks (char *array, int size)
{
  int i=0;
  int j=0;
  for (i=0;i<size;i++)
    {
      if ( (array[i]==' ') ||  (array[i]=='\t') ||  (array[i]=='\n') )
	{
	  for (j=i;j<size-1;j++)
	    {
	      array[j]=array[j+1];
	    }
	  i--;
	  array[j]='\0';
	}
      else
	{
	  return 0;
	}
    }
  return XNEE_OK;
}


/**************************************************************
 *                                                            *
 * rem_all_blanks                                             *
 *                                                            *
 *                                                            *
 **************************************************************/
int
rem_all_blanks (char *array, size_t size)
{
  int i=0;
  int j=0;
  int zeros=0;


  for (i=0;i<(int)size;i++)
    {
      if ( (array[i]==' ') ||  (array[i]=='\t') ||  (array[i]=='\n') )
	{
	  zeros++;
	  for (j=i;j<(int)size;j++)
	    {
	      array[j]=array[j+1];
	    }
	  i--;
	  array[j-zeros]='\0';
	}
    }
  return XNEE_OK;
}





/**************************************************************
 *                                                            *
 * xnee_client_id                                             *
 *                                                            *
 *                                                            *
 **************************************************************/
XID 
xnee_client_id (Display *dpy) 
{
  return dpy->resource_base ;
}








