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
#include "libxnee/xnee_fileop.h"
#include "libxnee/xnee_error.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_alloc.h"
#include "libxnee/print_varargs.h"


int  
xnee_free_file (xnee_data *xd, /*@null@*/ char *file_name, /*@null@*/ FILE* file)
{
  xnee_verbose((xd, "Closing file=%s fd=%d\n", 
		file_name, 
		(int)file));
  

   /*  @owned@  */ /* char *tmp = file_name ; */
   if ( file_name != NULL) 
   {
      /* Tell splint we REALLY want to cast FILE* to an int 
         .... it won't believe us else */
      /*@access FILE@*/
       XNEE_FREE_IF_NOT_NULL (file_name); 
    }

   if (file!=NULL)
     {
       XNEE_FCLOSE_IF_NOT_NULL ( file ) ;
     }

  return XNEE_OK;
}


int
xnee_open_err_file(xnee_data *xd)
{
  char *file_name;

  file_name = xnee_get_err_name(xd);
  if (file_name!=NULL)
    {
      xnee_verbose((xd, "---  xnee_open_files: handling err\n"));
      if ((xnee_check (file_name, "stderr", "STDERR") == XNEE_OK) &&
	  ((xnee_check (file_name, "-", "-") == XNEE_OK) ) )
	{
	  xnee_verbose((xd, "---  xnee_open_files: opening err: %s\n", 
			xd->err_name));
	  if ( xd->err_file != xd->saved_err_file )
	    {
	      XNEE_FCLOSE_IF_NOT_NULL(xd->err_file); 
	    }
	  xd->err_file = fopen (xd->err_name,"w");
	  if (xd->err_file==NULL)
	    {
	      return XNEE_FILE_NOT_FOUND;
	    }
	  xnee_verbose((xd, "---  xnee_open_files: opening err: %s  OK\n", 
			xd->err_name));
	}
    }
  return XNEE_OK;
}


int
xnee_open_files(xnee_data *xd)
{
  char *file_name;

  xnee_verbose((xd, "---> xnee_open_files\n"));

  xnee_open_err_file(xd);

  if ( (xnee_is_recorder(xd) != 0) )
    {
      xnee_verbose((xd, "---  xnee_open_files: is recorder\n"));
      file_name = xnee_get_out_name(xd);
      if (file_name!=NULL)
	{
	  xnee_verbose((xd, "---  xnee_open_files: handling out\n"));
	  if ( xnee_check (file_name, "stdout", "STDOUT") == 0 )
	    {
	      xnee_verbose((xd, "---  xnee_open_files: opening out: %s\n", 
			    xd->out_name));
	      if ( xd->out_file != xd->saved_out_file )
		{
		  XNEE_FCLOSE_IF_NOT_NULL(xd->out_file); 
		}
	      xd->out_file = fopen (xd->out_name,"w");
	      if (xd->out_file==NULL)
		return XNEE_FILE_NOT_FOUND;
	    }
	}
    }
  else if ( xnee_is_replayer(xd) != 0 )
    {
      xnee_verbose((xd, "---  xnee_open_files: is replayer\n"));
      file_name = xnee_get_data_name(xd);
      if (file_name!=NULL)
	{
	  xnee_verbose((xd, "---  xnee_open_files: handling data (in)\n"));

	  if ( xnee_check (file_name, "stdin", "STDIN") == 0 )
	    {
	      xnee_verbose((xd, "---  xnee_open_files: opening data: %s\n", 
			    xd->data_name));
	      XNEE_FCLOSE_IF_NOT_NULL(xd->data_file); 
	      xd->data_file = fopen (xd->data_name,"r");
	      if (xd->data_file==NULL)
		return XNEE_FILE_NOT_FOUND;
	    }
	}
    }
  else if ( xnee_is_retyper(xd) != 0 )
    {
      xnee_verbose((xd, "---  xnee_open_files: is retyper\n"));
      file_name = xnee_get_rt_name(xd);
      xnee_verbose((xd, "---  xnee_open_files: retype file %s\n", file_name));

      if (file_name!=NULL)
	{
	  xnee_verbose((xd, "---  xnee_open_files: handling retype (in)\n"));
	  if ( xnee_check (file_name, "stdin", "STDIN") == 0)
	    {
	      xnee_verbose((xd, "---  xnee_open_files: opening retype: %s\n", 
			    xd->rt_name));
	      XNEE_FCLOSE_IF_NOT_NULL(xd->rt_file); 
	      xd->rt_file = fopen (xd->rt_name,"r");
	      if (xd->rt_file==NULL)
		return XNEE_FILE_NOT_FOUND;
	    }
	}
    }
  return XNEE_OK;
}


