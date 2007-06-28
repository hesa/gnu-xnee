/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *    Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004 Henrik Sandklef
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

/* This file contains function handling different resolution 
   during reaply than when recording ... */

#include "libxnee/xnee.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"
#include "libxnee/print.h"
#include "libxnee/xnee_resolution.h"

int
xnee_set_default_rep_resolution (xnee_data *xd)
{
  if (xd->data != NULL)
    {
      xd->res_info.replay.x_res = DisplayWidth  (xd->data, 0);
      xd->res_info.replay.y_res = DisplayHeight (xd->data, 0);
      return XNEE_OK;
    }
  return -1 ;
}
 
int
xnee_set_default_rec_resolution (xnee_data *xd)
{
  if ( (xd!=NULL) &&
       (xd->data != NULL) )
    {
      xd->res_info.record.x_res = DisplayWidth  (xd->data, 0);
      xd->res_info.record.y_res = DisplayHeight (xd->data, 0);
      return XNEE_OK;
    }
  return -1 ;
}
 

int
xnee_no_rep_resolution (xnee_data *xd)
{
  return ( (xd->res_info.replay.x_res != 1) && 
	   ( xd->res_info.replay.y_res != 1));
}
 
 

int 
xnee_str_to_res(char *res_str, xnee_res *xr)
{
  int ret;

  if (res_str == NULL)
    {
      return XNEE_BAD_RESOLUTION;
    }

  if ( ! (  ( xr->x_res == 1 ) && ( xr->y_res == 1 )))
    {
      return XNEE_OK;
    }
  
  ret = sscanf(res_str, "%dx%d",
	       &xr->x_res, 
	       &xr->y_res); 

  if ( ret == 2 ) 
    {
      return XNEE_OK;
    }
  else if (xnee_check ("VGA", res_str, res_str))
    {
      xr->x_res = 800 ;
      xr->y_res = 640 ;
      return XNEE_OK;
    }
  else if (xnee_check ("SVGA", res_str, res_str))
    {
      xr->x_res = 1024 ;
      xr->y_res = 768 ;
      return XNEE_OK;
    }
  else if (xnee_check ("XGA", res_str, res_str))
    {
      xr->x_res = 1280 ;
      xr->y_res = 1024 ;
      return XNEE_OK;
    }
  else if (xnee_check ("UXGA", res_str, res_str))
    {
      xr->x_res = 1600 ;
      xr->y_res = 1440 ;
      return XNEE_OK;
    }
  else
    {
      ;
    }
  return XNEE_BAD_RESOLUTION;
}


int 
xnee_res_cmp(xnee_res *xr1, xnee_res *xr2)
{
  int ret;
  ret =  ( ( xr1->x_res == xr2->x_res)
	   && 
	   ( xr1->y_res == xr2->y_res) );
  /* 
     1   means same
     0   means diff 
  */
  return ret;
}


int 
xnee_resolution_differs (xnee_data *xd)
{
  if (xnee_res_cmp(&xd->res_info.record,&xd->res_info.replay)==0)
    {
      /* diff */
      return 1;
    }
  else
    {
      /* no diff */
      return 0;
    }
}


int 
xnee_resolution_newx (xnee_data *xd, int xval)
{
  static int diff = XNEE_RESOLUTION_UNSET ; 

  if (diff==XNEE_RESOLUTION_UNSET)  
    {
      if (xnee_is_resolution_used(xd))
	{
	  if (xnee_resolution_differs(xd))
	    {
	      diff = XNEE_RESOLUTION_USED;
	    }
	  else
	    {
	      diff = XNEE_RESOLUTION_UNUSED;
	    }
	}
      else
	{
	  diff = XNEE_RESOLUTION_UNUSED;
	}
    }
  if (diff==XNEE_RESOLUTION_USED)
    {
      return ( ( xval * xnee_get_rep_resolution_x (xd) ) 
	       / xnee_get_rec_resolution_x (xd));
    }

  return xval;
}

int 
xnee_resolution_newy (xnee_data *xd, int yval)
{
  static int diff = XNEE_RESOLUTION_UNSET ;

  if (diff==XNEE_RESOLUTION_UNSET)
    {
      if (xnee_is_resolution_used(xd))
	{
	  if (xnee_resolution_differs(xd))
	    {
	      diff = XNEE_RESOLUTION_USED;
	    }
	  else
	    {
	      diff = XNEE_RESOLUTION_UNUSED;
	    }
	}
      else
	{
	  diff = XNEE_RESOLUTION_UNUSED;
	}
    }
  if (diff==XNEE_RESOLUTION_USED)
    {
      return ( ( yval * xnee_get_rep_resolution_y (xd) ) 
	       / xnee_get_rec_resolution_y (xd));
    }
  return yval;
}

int
xnee_is_resolution_used (xnee_data *xd)
{
   if (xd==NULL)
   {
      return XNEE_MEMORY_FAULT;
   }
  return xd->res_info.is_used==XNEE_RESOLUTION_USED;  
}

int 
xnee_set_resolution_used (xnee_data *xd)
{
   if (xd==NULL)
   {
      return XNEE_MEMORY_FAULT;
   }
  xd->res_info.is_used=XNEE_RESOLUTION_USED;  
  return XNEE_OK;
}

int 
xnee_unset_resolution_used (xnee_data *xd)
{
   if (xd==NULL)
   {
      return XNEE_MEMORY_FAULT;
   }
  xd->res_info.is_used=XNEE_RESOLUTION_UNUSED;  
  return XNEE_OK;
}

int 
xnee_get_resolution_used (xnee_data *xd)
{
   if (xd==NULL)
   {
      return XNEE_MEMORY_FAULT;
   }
  return xd->res_info.is_used;  
}

int
xnee_resolution_init (xnee_data *xd)
{
   xd->res_info.record.x_res=1; 
   xd->res_info.record.y_res=1; 

   xd->res_info.replay.x_res=1; 
   xd->res_info.replay.y_res=1; 

/*   xnee_set_resolution_used(xd); */
  return XNEE_OK;
}
 


