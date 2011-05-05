/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003, 
 *                      2004, 2009, 2010, 2011 Henrik Sandklef 
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
    {
      return 1;
    }
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








char **
xnee_str2strptr (char *tmp, int fill_option)
{
  int size=0; 
  int str_size=0 ; 
  char **ret ; 
  char *blank_ptr1;
  char *str;
  ret = NULL;

  while (*tmp)
    {
      /* rem leading blanks */
      while(tmp[0]==' ') 
	{
	  if (tmp[0]==' ')
	    {
	      tmp++;
	    }
	}

      /* Find next blank (or newline) */
      blank_ptr1 = strstr(tmp, " ");
      if (blank_ptr1==NULL)
	{
	  blank_ptr1 = strstr(tmp, "\n");
	}

      ret= (char**) realloc(ret,(size+2)*sizeof(char*));

      if (ret == NULL )
	{
	  break;
	}

      if (blank_ptr1 != NULL)
	{
	  str_size = strlen(tmp) - strlen(blank_ptr1) + 1 ;
	}
      else
	{
	  str_size = strlen(tmp) + 1;
	}

      if ( (size==0) && (fill_option==XNEE_CLI_SYNTAX) )
	{
	  str =  (char*) calloc(str_size, sizeof(char)+2);
	  strcpy (str, "--");
	  strncat(str, tmp, str_size-1);
	}
      else
	{
	  str =  (char*) calloc(str_size, sizeof(char));
	  strncpy(str, tmp, str_size-1);
	}

      ret[size]   = str;
      ret[size+1] = NULL; 
      size++;

      tmp = blank_ptr1;
      if (tmp==NULL)
	{
	  break;
	}
      tmp++;
    }

  return ret;
}


void
xnee_print_strptr(xnee_data *xd, char **strptr)
{
  int i = 0 ; 

  if ( (xd==NULL) || ( xd->verbose == 0 ) )
    {
      return;
    }

  if (strptr==NULL)
    {
      return;
    }

  fprintf (xd->err_file, "Option: '%s'\n", strptr[0]);

  for (i=1; strptr[i] != NULL ; i++)
    {
      if ( strptr[i] != NULL ) 
	{
	  fprintf (xd->err_file, "\targument: '%s'\n", strptr[i]);
	}
      else break;
    }
}

int
xnee_free_strptr(char **strptr)
{
  int i = 0 ; 

  if (strptr==NULL)
    {
      return XNEE_OK;
    }

  for (i=0; strptr[i] != NULL ; i++)
    {
      if ( strptr[i] != NULL ) 
	{
	  free(strptr[i]);
	}
      else 
	{
	  break;
	}
    }
  free(strptr);

  return XNEE_OK;
}

int
xnee_boolstr2int(xnee_data *xd, char *str)
{
  int ret;

  XNEE_VERBOSE_ENTER_FUNCTION();
  if (str==NULL)
    {
      ret = XNEE_BOOL_IMPLICIT_TRUE;
    }
  else if (xnee_check_true(str))
    {
      ret = XNEE_BOOL_EXPLICIT_TRUE;
    }
  else if (xnee_check_false(str))
    {
      ret = XNEE_BOOL_EXPLICIT_FALSE;
    }
  else
    {
      ret = XNEE_BOOL_ERROR;
    }
  XNEE_VERBOSE_LEAVE_FUNCTION();
  return ret;
}


int
xnee_str2int(xnee_data *xd, char *str)
{
  int ret=XNEE_OK;

  xnee_verbose((xd, " xnee_str2int\n"));  

  if (str==NULL)
    {
      /* Set return value to error */
      ret = INT_MAX;
    }
  else
    {
      /* scan the string */
      if ( !sscanf(str, "%d", &ret) == 1 )
	{
	  /* If scan failed, set return value to error */
	  ret = INT_MAX;
	}
    }
  return ret;
}


int 
xnee_record_from_data_display(xnee_data *xd)
{
  int ret_val = 0;


  /* fprintf (stderr,"X info:   %s %d %d %d\n", */
  /* 	  xd->x_vendor_name,  */
  /* 	  xd->x_version_major,  */
  /* 	  xd->x_version_minor,  */
  /* 	  xd->x_version_minor_sub); */

  if ( (xd != NULL) && (xd->x_vendor_name != NULL ) )
    {
      

      /*
       *  Xorg
       *
       *    Fedora 13 (X.org 1.8.2), 14 (X.org 1.9.1) calls the server "Fedora project" 
       *          (thanks to William Bader)
       */
      if (strstr(xd->x_vendor_name, "X.Org") || strstr(xd->x_vendor_name, "Fedora Project"))
	{
	  /*
	   *  Version 1
	   */
	  if ( xd->x_version_major == 1 )
	    {
	      /*
	       *  versions 
	       */
	      if ( ( xd->x_version_minor >= 3 ) &&
		   ( xd->x_version_minor <= 10 ) )
		{

		  /*
		   *  Once the XLIB fix is in place
		   *  this should be enabled 
		   *
		  if ( ( xd->x_version_minor == 7 ) &&
		       ( xd->x_version_minor_sub > 6 ))
		    {
		    ;
		    }
		  else
		    {
		  */
		      /*
		    }
		      */
		  ret_val=1;
		}
	      else
		{
		  ret_val=0;
		}
	    }
	  else if ( xd->x_version_major == 6 )
	    {
	      if ( xd->x_version_minor == 9 )
	        {
		  /*
		   * NoMachine (nxserver-3.4.0-8) 
		   *    returns "The X.Org Foundation" 6.9.0
		   */
		  ret_val = 1;
		}
	      else
		{
		  fprintf(stderr, "               WARNING\n");
		  fprintf(stderr, "You seem to be running NoMachine X server\n");
		  fprintf(stderr, "with a X server version unkown to " PACKAGE " \n");
		  fprintf(stderr, "We will assume that it works similar to nxserver-3.4.0-8\n");
		  ret_val = 1;
		}
	    }
	  else if ( xd->x_version_major == 7 )
	    {
	      ret_val = 1;
	    }
	}
      else if (strstr(xd->x_vendor_name, "Sun Microsystems") )
	{
	  
	  if ( ( xd->x_version_major == 1 ) &&
	       ( xd->x_version_minor >= 3 ) )
	    {
	      ret_val = 1;
	    }
	}
      /* N900 */
      else if (strstr(xd->x_vendor_name, "Nokia") )
	{
	  if ( ( xd->x_version_major == 1 ) &&
	       ( xd->x_version_minor >= 1 ) )
	    {
	      ret_val = 1;
	    }
	}
      else
	{
	  ;
	}
    }
  xnee_verbose((xd, " -- xnee_record_from_data_display()  using the following X serv data\n"));
  xnee_verbose((xd, "    using the following X serv data\n"));
  xnee_verbose((xd,   "X info:   %s %d %d %d\n",
		xd->x_vendor_name, 
		xd->x_version_major, 
		xd->x_version_minor, 
		xd->x_version_minor_sub));
  xnee_verbose((xd, "<-- %d \n", ret_val));

  /* fprintf (stderr, "Return %d\n", ret_val); */

  return ret_val ;
}



Display *
xnee_get_display_for_recordcontext(xnee_data *xd)
{
  Display *context_display;

  if (xd==NULL)
    {
      return NULL;
    }

  if ( xnee_record_from_data_display(xd))
    {
      /*
       * From X.org 1.6.0 to ????
       * 
       *   there seem to be something strange about 
       *   the XRecordCreateContext call
       *   which causes the XRecordEnableContextAsync 
       *   to fail ... ugly fix, but it works
       *
       */
      fprintf(stderr, "Workaround: Creating context on data display instead of control \n");
      fprintf(stderr, "            You can ignore this message\n");
      context_display = xd->data ;
    }
  else
    {
      context_display = xd->control ;
    }
  return context_display;
}


int
xnee_is_screen_ok(xnee_data *xd, int screen)
{
  int ret;

  ret = 0 ;
  if ( (screen >= 0 )  && 
       (screen < 100))
    {
      ret = 1;
    }
  else 
    {
      if ( (xd != NULL) && (xd->x_vendor_name != NULL ) )
	{
	  
	  if (strstr(xd->x_vendor_name, "Nokia") )
	    {
	      if ( ( xd->x_version_major == 1 ) &&
		   ( xd->x_version_minor >= 1 ) )
		{
		  ret = 1;
		}
	    }
	}
    }
  
  return ret ;
}



int 
xnee_check_true(char *expr)
{
   return  (int) ( (strncmp(expr,XNEE_TRUE_STRING,strlen(XNEE_TRUE_STRING))==0)
                   ||
                   (strncmp(expr,XNEE_1_STRING,strlen(XNEE_1_STRING))==0) );
}

int 
xnee_check_false(char *expr)
{
   return  (int)( (strncmp(expr,XNEE_FALSE_STRING,strlen(XNEE_FALSE_STRING))==0)
                  ||
                  (strncmp(expr,XNEE_0_STRING,strlen(XNEE_0_STRING))==0) );
}
