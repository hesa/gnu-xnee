/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003 
 *                      2009, 2010  Henrik Sandklef
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
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_resource.h"
#include "libxnee/xnee_session.h"
#include "parse.h"


static void 
cnee_handle_err(int error)
{
  const char *err;
  const char *descr;

  if ( error != XNEE_OK_LEAVE )
    {
      err   = xnee_get_err_description(error);
      descr = xnee_get_err_solution(error);
      
      fprintf (stderr, "Error number: %d\n", error);
      fprintf (stderr, "  Error:      %s\n", err);
      fprintf (stderr, "  Solution:   %s\n", descr);
    }
}



/**************************************************************
 *                                                            *
 * main                                                       *
 *                                                            *
 *                                                            *
 **************************************************************/
int main(int argc,char *argv[])
{
  int ret;
  xnee_data *xd ;


  /*  Get a new xnee_data structure  */
  xd = xnee_new_xnee_data();
  if (xd==NULL)
  {
     return (XNEE_NO_MAIN_DATA);
  }

  /* Set the program name */
  ret = xnee_set_program_name (xd, XNEE_CLI);
  if ( ret != XNEE_OK)
    {
      cnee_handle_err(ret);
    }

  /* Well .... parse the args */
  ret = xnee_parse_args (xd, argc, argv);
  if ( ret != XNEE_OK)
    {
      if (ret == XNEE_OK_LEAVE )
	{
	  exit (XNEE_OK);
	}
      else if ( ret == -1 )
	{
	  ret = XNEE_WRONG_PARAMS;
	}
      cnee_handle_err(ret);
    }

  /* Set the cli parameters */
  ret = xnee_set_application_parameters (xd, argv);
  if ( ret != XNEE_OK)
    {
      cnee_handle_err(ret);
    }

  if (ret==XNEE_OK)
    {
      /* start up the action set during parsing the commnd line */
      ret = xnee_start(xd);
    }

  if ( ret != XNEE_OK)
  {
    cnee_handle_err(ret);
  }

  /* hey, we are fin(n)ished .... close down */
  xnee_close_down(xd);
  
  /* Since we are here, we can exit gracefully */
  exit(ret); 
}






