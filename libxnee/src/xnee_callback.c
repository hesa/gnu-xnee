/*****  
 *       Xnee's Not an Event Emulator  
 *                                                                    
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003 
 *                      2009 Henrik Sandklef                    
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
#include "libxnee/print.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"
#include "libxnee/xnee_sem.h"
#include "libxnee/xnee_buffer.h"
#include "libxnee/xnee_resolution.h"
#include "libxnee/xnee_dl.h"
#include "libxnee/xnee_callback.h"


/**************************************************************
 *                                                            *
 * xnee_set_callback                                          *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_set_callback (xnee_data *xd, 
		   callback_ptrptr dest, 
		   const char *sym_name)
{
  const char *error;
  callback_ptr saved;
  xnee_verbose ((xd, "\nTrying to set \"%s\" as callback\n", sym_name));

  if (xd==NULL)
    {
      return XNEE_NO_MAIN_DATA;
    }
  else
    {
      saved = *dest;

      *(void**)(dest) =  xnee_dlsym(xd, 
				    xd->plugin_handle,
				    sym_name);
      error = xnee_dlerror(xd) ;
      if ( error != NULL)  
	{
	  xnee_verbose ((xd, "Failed to set \"%s\" from plugin\n", sym_name));
	  *dest = saved ;
	  fputs(error, stderr);
	  return (XNEE_PLUGIN_FILE_ERROR);
	}
      else
	{
	  xnee_verbose ((xd, "Function set  \"%s\" OK \n", sym_name));
	}
    }
  return XNEE_OK;
}


/**************************************************************
 *                                                            *
 * xnee_set_synchronize                                       *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_set_synchronize (xnee_data *xd,
		      synch_ptrptr dest, 
		      const char *sym_name)
{

   const char *error;
  synch_ptr saved;
  xnee_verbose ((xd, "\nTrying to set \"%s\" as callback\n", sym_name));

  saved = *dest;

  if (xd==NULL)
    {
      return XNEE_NO_MAIN_DATA;
    }
  else
    {
      dest = (synch_ptrptr) xnee_dlsym(xd, 
				       xd->plugin_handle, 
				       (const char*)sym_name);
      
      if ((error = xnee_dlerror(xd)) != NULL)  
	{
	  xnee_verbose ((xd, "Failed to set \"%s\" from plugin\n", sym_name));
	  *dest = saved ;
	  fputs(error, stderr);
	  return (XNEE_PLUGIN_FILE_ERROR);
	}
      else
	{
	  xnee_verbose ((xd, "Function set  \"%s\" OK \n", sym_name));
	}
    }
  return XNEE_OK;
}
