/*****  
 *       Xnee's Not an Event Emulator  
 *                                                                    
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003 Henrik Sandklef                    
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston,            
 * MA  02111-1307, USA.                                              
 ****/



#include <X11/extensions/XTest.h>

#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"
#include "libxnee/xnee_sem.h"
#include "libxnee/xnee_buffer.h"
#include "libxnee/xnee_resolution.h"
#include "libxnee/xnee_dl.h"




/**************************************************************
 *                                                            *
 * xnee_get_elapsed_time                                      *
 *                                                            *
 *                                                            *
 **************************************************************/
long 
xnee_get_elapsed_time(xnee_data *xd, char type )
{
  static  long last_read_msecec = 0;  /* from last call of this f'n */
  static  long last_read_sec = 0;
  static  long first_read_msecec = 0; /* from 1st time this f'n called */
  static  long first_read_sec = 0;
  long    last_sec; 
  long    last_msec; 
  long    diff_sec; 
  long    diff_msec; 
  long    time_offset_msec  = 0;

  struct timeval  cur_time;
  struct timezone zoneData;


  /* determine what value to get difference from */
  if ( type == XNEE_FROM_LAST_READ )
    {
      last_sec = last_read_sec;
      last_msec = last_read_msecec;
    }
  else
    {
      last_sec = first_read_sec;
      last_msec = first_read_msecec;
    }

  /* get current time */
  if(  gettimeofday( &cur_time, &zoneData) == 0 )
    {
      /* get difference between now and last */
      diff_sec = cur_time.tv_sec - last_sec;
      diff_msec = cur_time.tv_usec - last_msec;

      /* convert diff to usec */	
      time_offset_msec = (diff_sec * XNEE_USEC_PER_SEC) + diff_msec;


      /* if first time through - save values and return XNEE_OK */
      if ( last_sec == 0  )
	{
	  last_read_msecec = cur_time.tv_usec;
	  last_read_sec = cur_time.tv_sec;
	  first_read_msecec = cur_time.tv_usec;
	  first_read_sec = cur_time.tv_sec;
/* 	  xnee_verbose ((xd, "1st elapsed time:%c _sec _msec: %d %d timeoffeset = 0\n",  */
/* 		       type, cur_time.tv_sec, cur_time.tv_usec )); */
	  return XNEE_OK;
	}

      /* update last_read values with the new time */
      if ( type == XNEE_FROM_LAST_READ )
	{
	  last_read_msecec = cur_time.tv_usec;
	  last_read_sec = cur_time.tv_sec;
    	}
/*       xnee_verbose ((xd, "elapsed time type:%c current time _sec _msec: %d %d time_offset_msec: %d\n",  */
/* 		   type, cur_time.tv_sec, cur_time.tv_usec, time_offset_msec )); */
      return time_offset_msec;
    }

  /* Shouldn't reach this point.... silent the compiler */
  return 0;
}





/**************************************************************
 *                                                            *
 * xnee_calc_sleep_amount                                     *
 *                                                            *
 *                                                            *
 **************************************************************/
long 
xnee_calc_sleep_amount(xnee_data *xd, 
		       long last_diff, 
		       long first_diff, 
		       long record_last_diff, 
		       long recordFirst_diff )
{
  long sleep_amt, out_of_wack_amt =0;
  float tmp;

  

/*   xnee_verbose ((xd, "xnee_calc_sleep_amount last_diff: %lu first_diff: %lu record_last_diff: %lu recordFirst_diff: %lu\n",  */
/* 		last_diff, first_diff, record_last_diff, recordFirst_diff )); */

  /* determine where we are from first read  either too fast or too slow */
  /* find amount that we are too fast or too slow */
  out_of_wack_amt = recordFirst_diff - first_diff;

  if ( out_of_wack_amt > 0 ) /* too fast - we should slow down a bit */
    {
      if ( record_last_diff > last_diff )  /* recorded wait more than we have waited so far */
	{
	  /* if the amount we are out of wack is more than the recorded wait */
	  /* then sleep the full recorded difference */
	  if ( out_of_wack_amt > record_last_diff )  
	    {
	      sleep_amt = record_last_diff;
	    }
	  else /* otherwise sleep the amount out of wack */
	    {
	      sleep_amt = out_of_wack_amt;
	    }
	}
      else  /* we have already waited the recorded amount of time */
	{
	  /* but since we are still too fast - sleep a percentage of recorded wait time */
	  tmp = (float)record_last_diff * XNEE_TOO_FAST_ADJ_PERCENTAGE/100;
	  sleep_amt = ( long )tmp;
	}
    }
  else if ( out_of_wack_amt < 0 ) /* too slow or right on time */ 
    {
      if ( record_last_diff > last_diff )  /* recorded wait more than we have waited so far */
	{
	  sleep_amt = record_last_diff - last_diff; /* amount of wait left */
	}
      else  /* we have already waited the recorded amount of time */
	{
	  sleep_amt = 0;
	}
    }

/*   xnee_verbose (( xd, "xnee_calc_sleep_amount: %d\n", sleep_amt )); */
  return ( sleep_amt );
}




