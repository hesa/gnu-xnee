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
#include "libxnee/xnee_error.h"
#include "libxnee/xnee_grab.h"
#include "libxnee/xnee_alloc.h"
#include "libxnee/xnee_range.h"
#include "libxnee/xnee_utils.h"
#include "libxnee/xnee_session.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_resource.h"
#include "libxnee/xnee_fileop.h"
#include "libxnee/print_varargs.h"


int xnee_free_dyn_data(xnee_data *xd);


/**************************************************************
 *                                                            *
 * xnee_new_recordext_setup                                   *
 *                                                            *
 *                                                            *
 **************************************************************/
/*@null@*/ 
xnee_recordext_setup*
xnee_new_recordext_setup()
{
   xnee_recordext_setup* xrs ;
   xrs = (xnee_recordext_setup*) malloc (sizeof (xnee_recordext_setup));
   XNEE_CHECK_ALLOCED_MEMORY(xrs);

   memset (xrs, 0, sizeof (xnee_recordext_setup));

   if (xrs->xids != NULL) 
     { 
       XNEE_FREE_IF_NOT_NULL(xrs->xids);
     }
   /*@ignore@*/ /* we can ignore splint warning since we know we have freed it */
   xrs->xids     = (XRecordClientSpec*) calloc (2, sizeof(XRecordClientSpec)); 
  /*@end@*/
  XNEE_CHECK_ALLOCED_MEMORY(xrs->xids);

  xrs->rContext = 0;

  if (xrs->rState!=NULL) 
  {
     XNEE_FREE_IF_NOT_NULL (xrs->rState);
  }
  /*@ignore@*/ /* we can ignore splint warning since we know we have freed it */
  xrs->rState   = (XRecordState*) malloc (sizeof(XRecordState)); 
  /*@end@*/
  XNEE_CHECK_ALLOCED_MEMORY(xrs->rState);
  memset (xrs->rState, 0, sizeof (XRecordState));

  /* 
   * Record ranges are allocated using seperate functions
   */
  return xrs;
}



/**************************************************************
 *                                                            *
 * xnee_free_recordext_setup                                  *
 *                                                            *
 *                                                            *
 **************************************************************/
/*@null@*/ 
int 
xnee_free_recordext_setup(xnee_data* xd)
{
  int i;
  int max ; 
  int ret ; 
  /*@only@*/ /*@out@*/ /*@null@*/ 
  xnee_recordext_setup *record_setup = NULL ;


  XNEE_DEBUG ((stderr ," -->xnee_free_recordext_data()  \n"  ));
  
  if ( (xd!=NULL) && (xd->record_setup!=NULL))
  {
     record_setup = xd->record_setup;
     
     max = xnee_get_max_range(xd);
     
     ret = xnee_unsetup_recording(xd);
     if (ret!=XNEE_OK)
       {
	 xnee_print_error ("Could not unsetup recording (%d)\n", ret);
	 return ret;
       }
     
     if (max>0)
     {
        for ( i=0 ; i<max ; i++ )
        {
           XNEE_DEBUG ( (stderr, "   freeing range at %d since max=%d\n", i, max));
           /*@ignore@*/
           if ((record_setup != NULL) && 
               (record_setup->range_array != NULL ) &&
               (record_setup->range_array[i] != NULL ))
           {
	     ret = XNEE_FREE_AND_NULL (record_setup->range_array[i]);
	      if (ret!=XNEE_OK)
		{
		  xnee_print_error ("Could not free memory at  "
				    "record_setup->range_array[%d]   (%d)\n", 
				    i, ret);
		  return ret;
		}
           }
	   /*@end@*/
        }
     }
     XNEE_FREE_AND_NULL (record_setup->range_array);
  }


  /*@ignore@*/
  if (record_setup!=NULL)
  {
    XNEE_FREE_IF_NOT_NULL (record_setup->xids);
    XNEE_FREE_IF_NOT_NULL (record_setup->rState);
  }
  XNEE_FREE_IF_NOT_NULL (xd->record_setup);

  XNEE_DEBUG ( (stderr ," <--xnee_free_recordext_setup\n"  ));
  return XNEE_OK;
  /*@end@*/
}


void
xnee_free_replay_setup( /*@only@*/ /*@null@*/ xnee_testext_setup* xrs)
{
  XNEE_FREE_IF_NOT_NULL (xrs);
  return ;
}

/*@null@*/
xnee_testext_setup*
xnee_new_replay_setup(void)
{
   xnee_testext_setup* xrs = (xnee_testext_setup*)     malloc (sizeof (xnee_testext_setup)) ;
   XNEE_CHECK_ALLOCED_MEMORY(xrs);
   memset (xrs, 0, sizeof(xnee_testext_setup));
   return xrs;
}

static int
xnee_new_dyn_data(xnee_data *xd)
{
   if (xd==NULL)
   {
      return XNEE_MEMORY_FAULT;
   }

   xnee_verbose((xd, "---> xnee_new_dyn_data\n"));

   /* 
    * Freeing old memory 
    */
   xnee_verbose((xd, "---  xnee_new_dyn_data: freeing old memory (if found)\n"));
   xnee_free_dyn_data(xd);

   
   /* 
    * Getting new memory 
    */
   xd->replay_setup = xnee_new_replay_setup();
   XNEE_CHECK_ALLOCED_MEMORY_INT(xd->replay_setup);
   
   xd->record_setup  = xnee_new_recordext_setup(); 
   XNEE_CHECK_ALLOCED_MEMORY_INT(xd->record_setup);
   
   xd->grab_keys     = xnee_new_grab_keys();
   XNEE_CHECK_ALLOCED_MEMORY_INT(xd->grab_keys);
   
   xnee_grab_keys_init(xd);
   
   return XNEE_OK;
}


static int 
xnee_reset_xnee_info(/*@null@*/ xnee_data *xd)
{
  int i ; 
  if (xd==NULL)
  {
     return XNEE_NO_MAIN_DATA;
  }
  /*   probably not neeeded
       check if remove hesa hesa HESA HESA
       xnee_free_ranges(xd);  
  */

  return XNEE_OK;
}


int
xnee_free_dyn_data(xnee_data *xd)
{
   int ret;
   int i ; 
   xnee_verbose((xd, "---> xnee_free_dyn_data\n"));

   if ( xd->grab_keys && xd->grab_keys->action_keys)
   for (i=0;i<XNEE_GRAB_LAST;i++)
     {
       xnee_verbose((xd, " --- xnee_free_dyn_data: grab key str %d\n", i)); 
       XNEE_FREE_IF_NOT_NULL(xd->grab_keys->action_keys[i].str);
     }

   xnee_verbose((xd, " --- xnee_free_dyn_data: program name\n")); 
   XNEE_FREE_IF_NOT_NULL(xd->program_name);

   xnee_verbose((xd, " --- xnee_free_dyn_data: program name\n")); 
   XNEE_FREE_IF_NOT_NULL(xd->program_name);

   xnee_verbose((xd, " --- xnee_free_dyn_data: refreshing ranges\n")); 
   ret = xnee_free_ranges(xd); 
   XNEE_RETURN_IF_ERR(ret);

   xnee_verbose((xd, " --- xnee_free_dyn_data: replay_setup\n"));
   xnee_free_replay_setup(xd->replay_setup);
   xd->replay_setup = NULL;

   xnee_verbose((xd, " --- xnee_free_dyn_data: record_ext\n"));
   ret = xnee_free_recordext_setup (xd);
   XNEE_RETURN_IF_ERR(ret);

   xnee_verbose((xd, " --- xnee_free_dyn_data: grab_keys\n"));
   ret = xnee_free_grab_keys(xd->grab_keys);
   XNEE_RETURN_IF_ERR(ret);
   xd->grab_keys=NULL;

   xnee_verbose((xd, " --- xnee_free_dyn_data: resource_meta\n"));
   ret = xnee_free_xnee_resource_meta(&xd->xrm);
   XNEE_RETURN_IF_ERR(ret);

   xnee_verbose((xd, " --- xnee_free_dyn_data: xnee_info\n")); 
   ret = xnee_reset_xnee_info(xd);
   XNEE_RETURN_IF_ERR(ret);

   xnee_verbose((xd, "<--- xnee_free_dyn_data\n"));
   return XNEE_OK;
}




/**************************************************************
 *                                                            *
 * xnee_new_xnee_data                                         *
 *                                                            *
 *                                                            *
 **************************************************************/
/*@null@*/
xnee_data* 
xnee_new_xnee_data()
{
   int ret ;
   xnee_data* xd  = (xnee_data*) malloc (sizeof (xnee_data));

   if (xd==NULL)
   {
      xnee_print_error ("Could not allocate memory ........\n");
      return NULL;
   }
   memset (xd, 0, sizeof(xnee_data));
   
   ret = xnee_new_dyn_data(xd);
   XNEE_RETURN_NULL_IF_ERR(ret);
  
   ret = xnee_init(xd);
   XNEE_RETURN_NULL_IF_ERR(ret);

   return xd;
}


/**************************************************************
 *                                                            *
 * xnee_free_xnee_data                                        *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_free_xnee_data(xnee_data* xd)
{
  int ret ; 

  ret = xnee_free_dyn_data(xd);
  XNEE_RETURN_IF_ERR(ret);
  
#ifdef XNEE_USE_SEMAPHORES
  free (xd->buf_sem);
#endif 
  free (xd);
  return XNEE_OK;
}



int 
xnee_renew_xnee_data(xnee_data *xd)
{
   int ret ; 

   ret = xnee_free_dyn_data(xd);
   XNEE_RETURN_IF_ERR (ret);

   ret = xnee_new_dyn_data(xd);
   XNEE_RETURN_IF_ERR (ret);


   /*
    * Reset counters...
    */
   xd->first_read_time = 0;
   
   xd->meta_data.sum_max    = 0;
   xd->meta_data.sum_min    = 0;
   
   xd->meta_data.total_diff = 0;
   xd->meta_data.cached_max = 0;
   xd->meta_data.cached_min = 0;
  
  return (XNEE_OK);
}

int
xnee_free( /*@only@*/  /*@out@*/ /*@null@*/ void *mem)
{
   if (mem==NULL)
   {
      return XNEE_MEMORY_FAULT;
   }
   free(mem);
   return XNEE_OK;
}
