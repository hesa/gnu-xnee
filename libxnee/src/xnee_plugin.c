/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004,
 *                      2008, 2009 Henrik Sandklef 
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
#include "libxnee/xnee_dl.h"
#include "libxnee/xnee_alloc.h"
#include "libxnee/xnee_session.h"
#include "libxnee/xnee_plugin.h"
#include "libxnee/xnee_callback.h"

/**************************************************************
 *                                                            *
 * xnee_use_plugin                                            *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_use_plugin(xnee_data *xd, char *pl_name, unsigned char mode)
{
  int ret=0;
  #define LIB_NAME_STR_SIZE 100
  char lib_name[LIB_NAME_STR_SIZE];


  const char *rec_cb ;
  const char *rep_cb ;
  const char *sync_cb ;

  if (mode==0)
    {
      rec_cb  = XNEE_RECORD_CALLBACK_NAME;
      rep_cb  = XNEE_REPLAY_CALLBACK_NAME;
      sync_cb = XNEE_SYNC_FUNCTION_NAME;
    }
  else
    {
      rec_cb  = EXT_RECORD_CALLBACK_NAME;
      rep_cb  = EXT_REPLAY_CALLBACK_NAME;
      sync_cb = EXT_SYNC_FUNCTION_NAME;
    }

  xnee_verbose ((xd, "Using plugin file: %s\n", pl_name));
  strncpy (lib_name, "lib", LIB_NAME_STR_SIZE);
  strncat (lib_name, pl_name, LIB_NAME_STR_SIZE - strlen(lib_name) );

  if (xd->plugin_name!=NULL)
    {
      ret = XNEE_FREE_AND_NULL(xd->plugin_name);
      XNEE_RETURN_IF_ERR(ret);
    }
  /*@ignore@*/
  xd->plugin_name=strdup(pl_name);
  /*@end@*/
  xd->plugin_handle = (void*) xnee_dlopen (xd, lib_name, RTLD_LAZY );
  if (!xd->plugin_handle) 
    {
      (void)fputs (dlerror(), stderr);
      xnee_close_down(xd);
      exit(XNEE_PLUGIN_FILE_ERROR);
    }
  
  xnee_verbose ((xd, "We've got plugin file handle %d\n", (int)xd->plugin_handle));

  ret = xnee_set_callback (xd, 
			   &xd->rec_callback,
			   rec_cb);

  if ( ret != XNEE_OK )
    {
      /*
      xnee_close_down(xd);
      exit(XNEE_PLUGIN_FILE_ERROR);
      */
    }
  
  ret = xnee_set_callback (xd, 
			   &xd->rep_callback,
			   rep_cb);
  if ( ret != XNEE_OK )
    {
      /*      
	      xnee_close_down(xd);
	      exit(XNEE_PLUGIN_FILE_ERROR);
      */
    }

#ifdef USE_DYNAMIC_SYNC_FUN   
  ret = xnee_set_callback (xd, 
			   &xd->sync_fun,
			   XNEE_SYNC_FUNCTION_NAME);
  if ( ret != XNEE_OK )
    {
      /*      
      xnee_close_down(xd);
      exit(XNEE_PLUGIN_FILE_ERROR);
      */
    }
#endif   /* USE_DYNAMIC_SYNC_FUN    */

   return XNEE_OK;
}


/**************************************************************
 *                                                            *
 * xnee_unuse_plugin                                          *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_unuse_plugin(xnee_data *xd)
{
  int ret;
  if ( xd == NULL)
    {
      return (XNEE_MEMORY_FAULT);
    }
  XNEE_FREE_IF_NOT_NULL(xd->plugin_name);

  if (xd->plugin_handle != NULL)
    {
      ret = xnee_dlclose (xd, xd->plugin_handle);
      XNEE_RETURN_IF_ERR(ret);
      xd->plugin_handle = NULL;
    }

  xd->rec_callback     = xnee_record_dispatch ;
  xd->rep_callback     = xnee_replay_dispatch ;
  return XNEE_OK;
}


