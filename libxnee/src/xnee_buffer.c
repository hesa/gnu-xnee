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



/**************************************************************
 *                                                            *
 * xnee_replay_buffer_status                                  *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_replay_buffer_status (xnee_data* xd, int data_type, int nr)
{
  return xd->data_buffer[data_type][nr];
}



/**************************************************************
 *                                                            *
 * xnee_replay_buffer_max_diff                                *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_replay_buffer_max_diff (xnee_data* xd, int type)
{
  int max_val=0;
  int i ; 

  for (i=0;i<XNEE_REPLAY_BUFFER_SIZE;i++)
    {
      max_val = XNEE_MAX ( xd->data_buffer[type][i], max_val);
    }
  return max_val;
}


/**************************************************************
 *                                                            *
 * xnee_replay_buffer_min_diff                                *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_replay_buffer_min_diff (xnee_data* xd, int type)
{
  int min_val=0;
  int i ; 

  for (i=0;i<XNEE_REPLAY_BUFFER_SIZE;i++)
    {
      min_val = XNEE_MIN ( xd->data_buffer[type][i], min_val);
    }
  return min_val;
}


/**************************************************************
 *                                                            *
 * xnee_replay_buffer_tot_diff                                *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_replay_buffer_tot_diff (xnee_data* xd, int type)
{
  int i;
  int tot_val=0;

  for (i=0;i<XNEE_REPLAY_BUFFER_SIZE;i++)
    {
      tot_val += XNEE_ABS(xd->data_buffer[type][i]);
    }
  return tot_val;
}






/**************************************************************
 *                                                            *
 *  OBSOLETE ..... all is done is synchron... function        *
 *                                                            *
 *                                                            *
 * xnee_replay_buffer_handle                                  *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_replay_buffer_handle (xnee_data* xd, int data_type, int nr, Bool rec_or_rep)
{


  /*
   *  int i,j;
   */
  int nr_of_data=0;


  /*
   * protect the buffer by waiting for the buffer semaphore
   */
  xnee_sem_wait (xd, xd->buf_sem);

  /*
   * From where did we get the data?
   * 
   * If rec_or_rep equals XNEE_RECEIVED  from the Xserver .... decrement
   * If not (thats is if XNEE_REPLAYED ) from file ..... increment 
   *
   */
  if (rec_or_rep==XNEE_RECEIVED) 
    {
      if (!    (  
		( data_type==XNEE_EVENT ) && 
		( nr <= ButtonRelease ) ))
	{
	  xd->data_buffer[data_type][nr]--;
	  xnee_verbose ((xd, 
			 "xnee_replay_buffer_handle decremented %d %d to %d\n",
			 data_type,nr,xd->data_buffer[data_type][nr] ));
	}
    }
  else 
    {
      if (! (  
	     ( data_type==XNEE_EVENT ) && 
	     ( nr <= ButtonRelease) ))
	{
	  xd->data_buffer[data_type][nr]++;
	  xnee_verbose ((xd, 
			 "xnee_replay_buffer_handle incremented %d %d to %d\n",
			 data_type, nr, xd->data_buffer[data_type][nr] ));
	}
    }


  /*
   * release the protecting semaphore the buffer
   */
  xnee_sem_post (xd, xd->buf_sem);
  return nr_of_data;
}





/**************************************************************
 *                                                            *
 * xnee_replay_buffer_handler                                 *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_replay_buffer_handler (xnee_data* xd, 
			    int data_type, 
			    int data_nr,
			    Bool rec_or_rep)
{
  xnee_verbose((xd,"---> xnee_replay_buffer_handler "));
  if (rec_or_rep==XNEE_RECEIVED) 
    xnee_verbose((xd," XNEE_RECEIVED \n"));
  else
    xnee_verbose((xd," XNEE_REPLAYED \n"));

  /*
   * protect the buffer by waiting for the buffer semaphore
   */
  xnee_sem_wait (xd, xd->buf_sem);


  /*
   * From where did we get the data?
   * 
   * If rec_or_rep equals:
   *    XNEE_RECEIVED     from the Xserver .... decrement
   *    XNEE_REPLAYED     from file ..... increment 
   *
   */
  if (rec_or_rep==XNEE_RECEIVED) 
    {
      if (!(  
	    ( data_type == XNEE_EVENT ) && 
	    ( data_nr   <= ButtonRelease ) ))
	{
	  if (data_nr==161)
	    {
	      fprintf (stderr, 
		       "161 received:This is workaround for a bug in Xnee\n");
	      
	      /*
	       * release the protecting semaphore the buffer
	       */
	      xnee_sem_post (xd, xd->buf_sem);
	      return;
	    }

	  /* 
	   * If we get a data such that its amount of 
	   * unsynced data (xd->data_buffer[data_type][data_nr]) 
	   * is the same as the cached maximum value for 
	   * the buffer we can't assure that the cached
	   * maximum value is valid, since we are going to 
	   * decrement the value. Set the cache to -1
	   */
	  if (xd->meta_data.cached_max == 
	      xd->data_buffer[data_type][data_nr])
	    {
	      /* don't get it .... please read the above read again */
	      xd->meta_data.cached_max=-1;
	    }


	  
	  /*
	   * decrement the data buffer
	   * decrement the total buffer
	   */
	  xd->data_buffer[data_type][data_nr]--;
	  xd->meta_data.total_diff--;
	  xd->meta_data.sum_min--;
	  xd->meta_data.sum_max--;
      
	  /*
	   * If the value for this data is smaller than the cached 
	   * minimum value. Set this value in the cache 
	   */
	  if ( xd->data_buffer[data_type][data_nr] < 
	       xd->meta_data.cached_min)
	    {
	      xd->meta_data.cached_min = 
		xd->data_buffer[data_type][data_nr] ;
	    }
	  
	  
	  XNEE_SYNC_DEBUG ( (stdout, "--------       [%d][%d]    %d   \n", 
			     data_type, nr, nr_of_data))  ;
	  xnee_verbose ((xd, "   buffer_handler  %d %d  to %d\n" ,
			data_type,data_nr, xd->data_buffer[data_type][data_nr]));
	}
    }
  else 
    {

      /* 
       * If we get a data such that its amount of unsynced data 
       * (xd->data_buffer[data_type][data_nr]) is the same as the 
       * cached minimum value for the buffer we can't assure that the cached
       * minimum value is valid, sice we are going to decrement the value. 
       * Set the cache to +1
       */
      if (xd->meta_data.cached_min == 
	  xd->data_buffer[data_type][data_nr])
	{
	  xd->meta_data.cached_min=+1;
	}
      
      /*
       * increment the data  buffer
       * increment the total buffer
       */
      xd->data_buffer[data_type][data_nr]++;
      xd->meta_data.total_diff++;
      xd->meta_data.sum_max++;
      xd->meta_data.sum_min++;
      
      /*
       * If the value for this data is bigger than the cached maximum value. 
       * Set this value in the cache 
       */
      if ( xd->data_buffer[data_type][data_nr] > 
	   xd->meta_data.cached_max)
	{
	  xd->meta_data.cached_max = 
	    xd->data_buffer[data_type][data_nr] ;
	}
      
      xnee_verbose ((xd, "   buffer_handler incremented   %d %d  to %d \n", 
		     data_type, data_nr, 
		     xd->data_buffer[data_type][data_nr] ));
    }
  
  /*
   * release the protecting semaphore the buffer
   */
  xnee_sem_post (xd, xd->buf_sem);
  
  xnee_replay_printbuffer(xd);
  xnee_verbose((xd,"<--- xnee_replay_buffer_handler \n"));;
}



int 
xnee_update_buffer_cache(xnee_data *xd)
{
  int type ; 
  int counter ; 
  
  
  /*
   * protect the buffer by waiting for the buffer semaphore
   */
  xnee_sem_wait (xd, xd->buf_sem);
  
  for (counter=0 ; counter < MAX_UNSYNC_LOOPS; counter++)
    {
      for ( type=0 ; type< 2 ; type++) 
	{
	  int real_min=0;
	  int real_max=0;
	  
	  real_min=xd->meta_data.cached_min;
	  real_max=xd->meta_data.cached_max;
	  
	  
	  /* The cached values might be invalid. 
	   * If so, get fresh value and replace them */
	  if (real_min==1)
	    {
	      xnee_replay_buffer_min_diff (xd,type);
	      real_min = xnee_replay_buffer_min_diff (xd,type);
	      xnee_verbose ((xd, 
			     " cached min value invalid. refreshing to %d   old: %d\n", 
			     real_min,
			     xd->meta_data.cached_min));
	      xd->meta_data.cached_min=real_min;
	    }
	  
	  
	  if (real_max==-1)
	    {
	      xnee_replay_buffer_max_diff (xd,type);
	      real_max = xnee_replay_buffer_max_diff (xd,type);
	      xnee_verbose ((xd, "  cached max value invalid. refreshing to %d   old: %d\n", 
			     real_max,
			     xd->meta_data.cached_max));
	      xd->meta_data.cached_max=real_max;
	    }
	    

	}
    }
  /*
   * release the protecting semaphore the buffer
   */
  xnee_sem_post (xd, xd->buf_sem);
  
  return XNEE_OK;
}




int 
xnee_hard_update_buffer_cache(xnee_data *xd)
{
  int type ; 
  int counter ; 
  int real_min=0;
  int real_max=0;
  
  
  /*
   * protect the buffer by waiting for the buffer semaphore
   */
  xnee_sem_wait (xd, xd->buf_sem);
  
  for (counter=0 ; counter < MAX_UNSYNC_LOOPS; counter++)
    {
      for ( type=0 ; type< 2 ; type++) 
	{
	  
	  real_min = xnee_replay_buffer_min_diff (xd,type);
	  if (real_min<xd->meta_data.cached_min)
	    xd->meta_data.cached_min=real_min;

	  real_max = xnee_replay_buffer_max_diff (xd,type);
	  if (real_max>xd->meta_data.cached_max)
	    xd->meta_data.cached_max=real_max;
	}
    }
  /*
   * release the protecting semaphore the buffer
   */
  xnee_sem_post (xd, xd->buf_sem);
  
  return XNEE_OK;
}




int
xnee_check_buffer_limits (xnee_data *xd)
{
  int cached_min = 0 ;
  int cached_max = 0 ;
  int sum_max    = 0 ;
  int sum_min    = 0 ;
  int tot_diff   = 0 ;
  int diff       = 0 ;


  /* HESA HESA HESA TO REMOVE 
  xnee_hard_update_buffer_cache (xd);
  */
  xnee_hard_update_buffer_cache (xd);


  cached_max = xd->meta_data.cached_max;
  cached_min = xd->meta_data.cached_min;
  sum_max    = xd->meta_data.sum_max;
  sum_min    = xd->meta_data.sum_min;
  tot_diff   = xd->meta_data.total_diff;
  xnee_verbose ((xd, "---> xnee_check_buffer_limits  button %d   key %d\n", 
		 xd->button_pressed,
		 xd->key_pressed
		 ));

  xnee_verbose ((xd, "---  %02d %02d %02d %02d %02d \n",
		 cached_max ,
		 cached_min ,
		 sum_max    ,
		 sum_min    ,
		 tot_diff   ));

  if ( cached_max > XNEE_BUFFER_MAX) 
    {
      xnee_verbose ((xd, "cached_max %d > XNEE_BUFFER_MAX %d\n",
		     cached_max , XNEE_BUFFER_MAX));
      xnee_replay_printbuffer(xd); 
      diff=cached_max;
    }
  else if ( cached_min < XNEE_BUFFER_MIN) 
    {
      xnee_verbose ((xd, "cached_min %d < XNEE_BUFFER_MIN %d\n",
		     cached_min , XNEE_BUFFER_MIN));
      xnee_replay_printbuffer(xd); 
      diff=cached_min;
    }
  /*
  else if ( sum_max > XNEE_BUFFER_SUM_MAX )
    {
      xnee_verbose ((xd, "sum_max %d > XNEE_BUFFER_SUM_MAX %d\n",
		     sum_max , XNEE_BUFFER_SUM_MAX));
      xnee_replay_printbuffer(xd); 
      diff=sum_max;
    }
  else if ( sum_min < XNEE_BUFFER_SUM_MIN )
    {
      xnee_verbose ((xd, "sum_min %d < XNEE_BUFFER_SUM_MIN %d\n",
		     sum_min , XNEE_BUFFER_SUM_MIN));
      xnee_replay_printbuffer(xd); 
      diff=sum_min;
      }*/
  else if ( tot_diff > XNEE_BUFFER_TOT_MAX )
    {
      xnee_verbose ((xd, "tot_diff %d > XNEE_BUFFER_TOT_MAX %d\n",
		     tot_diff , XNEE_BUFFER_TOT_MAX));
      xnee_replay_printbuffer(xd); 
      diff=tot_diff;
    }
  else
    {
      xnee_verbose ((xd, "no diff  %d %d\n",
		     tot_diff , XNEE_BUFFER_TOT_MAX));
      xnee_replay_printbuffer(xd); 
      diff=0;
    }
  
  xnee_verbose ((xd, "<--- xnee_check_buffer_limits diff=%d\n", diff));
  return diff;
}


