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

#include <semaphore.h>

#include <libxnee/xnee.h>
#include <libxnee/print.h>
#include <libxnee/xnee_sem.h>



/**************************************************************
 *                                                            *
 * xnee_sem_init                                              *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_sem_init(xnee_data *xd, sem_t *sem, int shared, int val)
{ 
  int ret = 0;
  ret = sem_init (sem, shared, val);
  return ret;
}

/**************************************************************
 *                                                            *
 * xnee_sem_wait                                              *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_sem_wait(xnee_data *xd, sem_t *sem)
{ 
  int ret = 0;
  xnee_verbose ((xd, "   --> WAITING for the sem\n"));
  ret = sem_wait (sem);
  xnee_verbose ((xd, "   <-- WAITING for the sem\n"));
  return ret;
}


/**************************************************************
 *                                                            *
 * xnee_sem_trywait                                           *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_sem_trywait(xnee_data *xd, sem_t *sem)
{ 
  int ret = 0;
  ret = sem_trywait (sem);
  return ret;
}

/**************************************************************
 *                                                            *
 * xnee_sem_post                                              *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_sem_post(xnee_data *xd, sem_t *sem)
{ 
  int ret = 0;
  ret = sem_post (sem);
  return ret;
}

/**************************************************************
 *                                                            *
 * xnee_sem_destroy                                           *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_sem_destroy(xnee_data *xd, sem_t *sem)
{ 
  int ret = 0;
  ret = sem_destroy (sem);
  return ret;
}

