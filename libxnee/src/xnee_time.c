/*****  
 *       Xnee's Not an Event Emulator  
 *                                                                    
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003
 *                      2011  Henrik Sandklef                    
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
#include "libxnee/xnee_time.h"
#include "libxnee/xnee_buffer.h"
#include "libxnee/xnee_resolution.h"
#include "libxnee/xnee_dl.h"




/**************************************************************
 *                                                            *
 * xnee_get_elapsed_time                                      *
 *                                                            *
 *                                                            *
 **************************************************************/
static  unsigned long last_read_msecec  = 0;  
static  unsigned long last_read_sec     = 0;
static  unsigned long first_read_msecec = 0; 
static  unsigned long first_read_sec    = 0;


int
xnee_reset_elapsed_time(xnee_data *xd)
{
    
    xnee_verbose ((xd, " --- xnee_reset_elapsed_time \n"));

    last_read_msecec       = 0;  
    last_read_sec     = 0;
    first_read_msecec = 0; 
    first_read_sec    = 0;
    return XNEE_OK;
}


long 
xnee_get_elapsed_time(xnee_data *xd, char type )
{
  long    last_sec; 
  long    last_msec; 
  long    diff_sec; 
  long    diff_msec; 
  long    time_offset_msec  = 0;

  struct timeval  cur_time;
  struct timezone zoneData;

  xnee_verbose ((xd, " --- xnee_get_elapsed_time\n"));


  /* determine what value to get difference from */
  if ( type == XNEE_FROM_LAST_READ )
    {
      last_sec  = last_read_sec;
      last_msec = last_read_msecec;
    }
  else
    {
      last_sec  = first_read_sec;
      last_msec = first_read_msecec;
    }

  /* get current time */
  if(  gettimeofday( &cur_time, &zoneData) == 0 )
    {

      /* get difference between now and last */
      diff_sec = cur_time.tv_sec - last_sec;
      diff_msec = (cur_time.tv_usec/1000) - last_msec;

      /* convert diff to msec */	
      time_offset_msec = (diff_sec * XNEE_MSEC_PER_SEC) + diff_msec;


      /* if first time through - save values and return XNEE_OK */
      if ( last_sec == 0  )
	{
	  last_read_msecec  = cur_time.tv_usec/1000;
	  last_read_sec     = cur_time.tv_sec;
	  first_read_msecec = cur_time.tv_usec/1000;
	  first_read_sec    = cur_time.tv_sec;
/* 	  xnee_verbose ((xd, "1st elapsed time:%c _sec _msec: %d %d timeoffeset = 0\n",  */
/* 		       type, cur_time.tv_sec, cur_time.tv_usec )); */
	  return XNEE_OK;
	}

      /* update last_read values with the new time */
      if ( type == XNEE_FROM_LAST_READ )
	{
	  last_read_msecec = cur_time.tv_usec/1000;
	  last_read_sec    = cur_time.tv_sec;
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
		       unsigned long last_diff, 
		       unsigned long first_diff, 
		       unsigned long record_last_diff, 
		       unsigned long recordFirst_diff )
{
  /* Time sleep_amt; */
  unsigned long out_of_wack_amt = 0;
  int           out_of_wack     = 0;
  float tmp;
  float start_diff_percent ;
  float compensation_factor ;
  float sleep_amt ; 
  static unsigned long stored_recordFirst_diff;

  xnee_verbose ((xd, "xnee_calc_sleep_amount last_diff: %lu first_diff: %lu record_last_diff:" 
		 "%lu recordFirst_diff: %lu\n",  
		 last_diff, first_diff, record_last_diff, recordFirst_diff )); 

  if ( stored_recordFirst_diff != 0 )
    {
      start_diff_percent = ((double)stored_recordFirst_diff / (double) first_diff);
      if ( start_diff_percent < 0.99 ) 
	{
	  if ( start_diff_percent < 0.70 ) 
	    {
	      compensation_factor=0.5;      
	    }
	  else if ( start_diff_percent < 0.80 ) 
	    {
	      compensation_factor=0.8;      
	    }
	  else
	    {
	      compensation_factor=0.9;      
	    }
	}
      else if ( start_diff_percent > 1.01 ) 
	{
	  if ( start_diff_percent > 1.3 ) 
	    {
	      compensation_factor=1.5;      
	    }
	  else if ( start_diff_percent > 1.2 ) 
	    {
	      compensation_factor=1.2;      
	    }
	  else
	    {
	      compensation_factor=1.1;      
	    }
	}
      else 
	{
	  compensation_factor=1.0;      
	}
    }
  else
    {
      compensation_factor=1.0;      
    }
  /* compensation_factor=1.0;       */
  
  sleep_amt= (record_last_diff*compensation_factor);

  if (sleep_amt > 5.0) 
    {
      sleep_amt = sleep_amt - 1.0;
    }

  stored_recordFirst_diff = recordFirst_diff;
  return ( sleep_amt );      

  /* printf ("\n\t%d  compensation: %f \n\n", record_last_diff, compensation_factor); */
  return record_last_diff*compensation_factor;

  /* determine where we are from first read  
   * either too fast or too slow */
  if (recordFirst_diff > first_diff)
    {
      out_of_wack=1;
    }
  else if (recordFirst_diff < first_diff)
    {
      out_of_wack=-1;
    }
  /* else we keep =0 */


  
  if ( out_of_wack==1 ) 
    {

      xnee_verbose (( xd, "xnee_calc_sleep_amount: too fast\n" )); 
      /* too fast - we should slow down a bit */
      if ( record_last_diff > last_diff ) 
	{
	  xnee_verbose (( xd, "xnee_calc_sleep_amount: too fast 1\n" )); 
	  /* recorded wait more than we have waited so far */

	  /* find amount that we are too fast or too slow */
	  out_of_wack_amt = recordFirst_diff - first_diff;
	  
	  /* if the amount we are out of wack is more than the recorded wait */
	  /* then sleep the full recorded difference */
	  if ( out_of_wack_amt > record_last_diff )  
	    {
	      xnee_verbose (( xd, "xnee_calc_sleep_amount: 1.1\n" )); 
	      //	      sleep_amt = record_last_diff;
	      sleep_amt = out_of_wack_amt ;
	    }
	  else 
	    {
	      xnee_verbose (( xd, "xnee_calc_sleep_amount: 1.2\n" )); 
	      /* otherwise sleep the amount out of wack */
	      sleep_amt = out_of_wack_amt;
	    }
	}
      else 
	{
	  xnee_verbose (( xd, "xnee_calc_sleep_amount: too fast 2 \n" )); 
	  /* we have already waited the recorded amount of time */
	  /* but since we are still too fast - 
	     sleep a percentage of recorded wait time */
	  tmp = (float)record_last_diff * XNEE_TOO_FAST_ADJ_PERCENTAGE/100;
	  sleep_amt = ( long ) tmp;
	}
    }
  else if ( out_of_wack == -1 ) 
    {
      xnee_verbose (( xd, "xnee_calc_sleep_amount: too slown" )); 
      if ( record_last_diff > last_diff ) 
	{
	  /* recorded wait more than we have waited so far */
	  /* amount of wait left */
	  sleep_amt = (record_last_diff - last_diff) / 8 ; 
	}
      else
	{
	  /* we have already waited the recorded amount of time */
	  sleep_amt = 0;
	}
    }
  else
    {
      xnee_verbose (( xd, "xnee_calc_sleep_amount: on time\n" )); 
      sleep_amt=2;
    }

  xnee_verbose (( xd, "xnee_calc_sleep_amount: %d\n", (int)sleep_amt )); 
  return ( sleep_amt );
}




/**************************************************************
 *                                                            *
 * xnee_calc_sleep_amount                                     *
 *                                                            *
 *                                                            *
 **************************************************************/
long 
xnee_calc_sleep_amount_slow(xnee_data *xd, 
			unsigned long  last_diff, 
			unsigned long first_diff, 
			unsigned long record_last_diff, 
			unsigned long recordFirst_diff )
{
  Time sleep_amt;
  unsigned long out_of_wack_amt = 0;
  int           out_of_wack     = 0;
  float tmp;

  
  recordFirst_diff = 
    recordFirst_diff * xnee_get_speed(xd) / 100;
  record_last_diff = 
    record_last_diff * xnee_get_speed(xd) / 100;
  
  xnee_verbose ((xd, "xnee_calc_sleep_amount_slow last_diff: %lu first_diff: %lu record_last_diff: %lu recordFirst_diff: %lu\n",  
		 last_diff, first_diff, record_last_diff, recordFirst_diff )); 
/*   printf ("xnee_calc_sleep_amount last_diff: %lu first_diff: %lu record_last_diff: %lu recordFirst_diff: %lu\t",    */
/*  	  last_diff, first_diff, record_last_diff, recordFirst_diff );   */
  

  /* determine where we are from first read  
   * either too fast or too slow */
  if (recordFirst_diff > first_diff)
    {
      out_of_wack=1;
    }
  else if (recordFirst_diff < first_diff)
    {
      out_of_wack=-1;
    }
  /* else we keep =0 */


  
  if ( out_of_wack==1 ) 
    {
      /* too fast - we should slow down a bit */
      if ( record_last_diff > last_diff ) 
	{
	  /* recorded wait more than we have waited so far */

	  /* find amount that we are too fast or too slow */
	  out_of_wack_amt = recordFirst_diff - first_diff;
	  
	  /* if the amount we are out of wack is more than the recorded wait */
	  /* then sleep the full recorded difference */
	  if ( out_of_wack_amt > record_last_diff )  
	    {
	      sleep_amt = record_last_diff;
	    }
	  else 
	    {
	      /* otherwise sleep the amount out of wack */
	      sleep_amt = out_of_wack_amt;
	    }
	}
      else 
	{
	  /* we have already waited the recorded amount of time */
	  /* but since we are still too fast - 
	     sleep a percentage of recorded wait time */
	  tmp = (float)record_last_diff * XNEE_TOO_FAST_ADJ_PERCENTAGE/100;
	  sleep_amt = ( long ) tmp;
	}
    }
  else if ( out_of_wack == -1 ) 
    {
      if ( record_last_diff > last_diff ) 
	{
	  /* recorded wait more than we have waited so far */
	  /* amount of wait left */
	    sleep_amt = (record_last_diff - last_diff) / 8  ; 
	}
      else
	{
	  /* we have already waited the recorded amount of time */
	    sleep_amt = 0;
	}
    }
  else
    {
      sleep_amt=0;
    }

/*   printf ("xnee_calc_sleep_amount: %d\n", (int)sleep_amt);  */
  xnee_verbose (( xd, "xnee_calc_sleep_amount: %d\n", (int)sleep_amt )); 

  return ( sleep_amt );

}





/**************************************************************
 *                                                            *
 * xnee_calc_sleep_amount                                     *
 *                                                            *
 *                                                            *
 **************************************************************/
long 
xnee_calc_sleep_amount_fast(xnee_data *xd, 
			    unsigned long last_diff, 
			    unsigned long first_diff, 
			    unsigned long record_last_diff, 
			    unsigned long recordFirst_diff )
{
  Time sleep_amt;
  unsigned long out_of_wack_amt = 0;
  int           out_of_wack     = 0;
  
  recordFirst_diff = 
    recordFirst_diff * xnee_get_speed(xd) / 100;
  record_last_diff = 
    record_last_diff * xnee_get_speed(xd) / 100;
  

  /* determine where we are from first read  
   * either too fast or too slow */
  if (recordFirst_diff > first_diff)
    {
      out_of_wack=1;
    }
  else if (recordFirst_diff < first_diff)
    {
      out_of_wack=-1;
    }
  /* else we keep =0 */


  
  if ( out_of_wack==1 ) 
    {
      /* too fast - we should slow down a bit */
      if ( record_last_diff > last_diff ) 
	{
	  /* recorded wait more than we have waited so far */
	  
	  /* find amount that we are too fast or too slow */
	  out_of_wack_amt = recordFirst_diff - first_diff;
	  
	  /* if the amount we are out of wack is more than the 
	   * recorded wait then sleep the full recorded difference */
	  if ( out_of_wack_amt > record_last_diff )  
	    {
	      sleep_amt = record_last_diff ;
	    }
	  else 
	    {
	      /* otherwise sleep the amount out of wack */
	      sleep_amt = out_of_wack_amt;
	    }
	}
      else 
	{
	  /* we have already waited the recorded amount of time */
	  /* but since we are still too fast - 
	     sleep a percentage of recorded wait time */
	  sleep_amt = record_last_diff ;
	}
    }
  else if ( out_of_wack == -1 ) 
    {

      
      /* we have already waited the recorded amount of time */
      sleep_amt = 10;
    }
  else
    {
      /* right on time */ 
      sleep_amt=0;
    }
    xnee_verbose (( xd, "xnee_calc_sleep_amount: %d\n", (int)sleep_amt )); 
    
  return ( sleep_amt );
}




