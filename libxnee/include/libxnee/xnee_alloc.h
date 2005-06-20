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


#ifndef XNEE_XNEE_ALLOC_H
#define XNEE_XNEE_ALLOC_H



/**
 * Allocates a new xnee_data structure. 
 * To free the memory, xnee_free_xnee_data can be used.
 * @return xnee_data * NULL if alloc failed
 */
/*@null@*/
xnee_data*
xnee_new_xnee_data(void);



/**
 * Fres xnee_data structure. 
 * @param xd   data to free 
 * @return int  0 on success
 */
int 
xnee_free_xnee_data(xnee_data* xd );



/**
 * Allocates a new xnee_recordext_setup.
 * To free the memory allocated, use xnee_free_recordext_setup
 * @return xnee_recordext_setup *  NULL if alloc failed
 */
/*@null@*/ 
xnee_recordext_setup*
xnee_new_recordext_setup(void);


/**
 * Frees xnee_free_recordext_setup structure
 * @param xd      xnee's main structure
 * @return int    0 on success
 */
/*@null@*/ 
int 
xnee_free_recordext_setup( xnee_data* xd);


void
xnee_free_replay_setup( /*@only@*/ /*@null@*/ xnee_testext_setup* xrs);

xnee_testext_setup*
xnee_new_replay_setup(void);



int
xnee_renew_xnee_data(xnee_data *xd);


int
xnee_free( /*@only@*/ /*@out@*/ /*@null@*/ void *mem);

#define XNEE_CHECK_ALLOCED_MEMORY(mem) \
   {                                            \
      if (mem==NULL)                                 \
      {                                                 \
         XNEE_PRINT_ERROR_IF_NOT_OK(XNEE_MEMORY_FAULT); \
         return (NULL);                                 \
      }                                                 \
   }

#define XNEE_CHECK_ALLOCED_MEMORY_INT(mem) \
   {                                            \
      if (mem==NULL)                                 \
      {                                                 \
         XNEE_PRINT_ERROR_IF_NOT_OK(XNEE_MEMORY_FAULT); \
         return (XNEE_MEMORY_FAULT);                                 \
      }                                                 \
   }




#endif /* XNEE_XNEE_ALLOC */
