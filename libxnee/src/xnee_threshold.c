/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003 Henrik Sandklef                    
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
#include "libxnee/xnee_resolution.h"
#include "libxnee/xnee_threshold.h"


static int 
xnee_set_threshold (int thresh, int *threshold)
{
  *threshold = thresh ;
  return XNEE_OK;
}

static int 
xnee_set_threshold_str (char *thresh_str, int *threshold)
{
  int ret;
  
  if (thresh_str == NULL)
  {
    return XNEE_BAD_THRESHOLD;
  }

  ret = sscanf(thresh_str, "%d",
	       threshold); 
  
  if ( ret == 1 ) 
    {
      return XNEE_OK;
    }
  return XNEE_BAD_THRESHOLD;
}



int
xnee_set_max_threshold_str (xnee_data *xd, char *thresh_str)
{
  return xnee_set_threshold_str (thresh_str, &xd->meta_data.sum_max_threshold);
} 

int
xnee_set_min_threshold_str (xnee_data *xd, char *thresh_str)
{
  return xnee_set_threshold_str (thresh_str, &xd->meta_data.sum_min_threshold);
} 

int
xnee_set_tot_threshold_str (xnee_data *xd, char *thresh_str)
{
  return xnee_set_threshold_str (thresh_str, 
				 &xd->meta_data.tot_diff_threshold);
} 


int
xnee_set_max_threshold (xnee_data *xd, int thresh)
{
    return xnee_set_threshold (thresh, &xd->meta_data.sum_max_threshold);
} 

int
xnee_set_min_threshold (xnee_data *xd, int thresh)
{
    return xnee_set_threshold (thresh, &xd->meta_data.sum_min_threshold);
} 

int
xnee_set_tot_threshold (xnee_data *xd, int thresh)
{
  return xnee_set_threshold (thresh, &xd->meta_data.tot_diff_threshold);
} 

int
xnee_get_max_threshold (xnee_data *xd)
{
  return xd->meta_data.sum_max_threshold;
}

int
xnee_get_min_threshold (xnee_data *xd)
{
  return xd->meta_data.sum_min_threshold;
}

int
xnee_get_tot_threshold (xnee_data *xd)
{
  return xd->meta_data.tot_diff_threshold;
}


