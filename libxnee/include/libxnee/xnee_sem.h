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
 * Foundation, Inc., 51 Franklin Street, Boston,            
 * MA  02110-1301, USA.                                              
 ****/

#include <libxnee/xnee.h>

#ifdef XNEE_USE_SEMAPHORES
#include <semaphore.h>


int 
xnee_sem_init(xnee_data *xd, sem_t *sem, int shared, int val);

int 
xnee_sem_wait(xnee_data *xd, sem_t *sem);

int 
xnee_sem_trywait(xnee_data *xd, sem_t *sem);

int 
xnee_sem_post(xnee_data *xd, sem_t *sem);

int 
xnee_sem_destroy(xnee_data *xd, sem_t *sem);

#else
#define xnee_sem_init(xd, sem, shared, val)
#define xnee_sem_wait(xd, sem)
#define xnee_sem_trywait(xd, sem)
#define xnee_sem_post(xd,  sem)
#define xnee_sem_destroy(xd, sem)
#endif /* XNEE_USE_SEMAPHORES*/

#define XNEE_BUFFER_SEM_INIT    
#define XNEE_BUFFER_SEM_GET      
#define XNEE_BUFFER_SEM_WAIT     
#define XNEE_BUFFER_SEM_POST    
#define XNEE_BUFFER_SEM_DESTROY 

