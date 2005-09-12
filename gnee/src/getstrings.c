/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 2002, 2003, 2004, 2005 Henrik Sandklef
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
#include "libxnee/datastrings.h"


int
print_em_all_sucker (void)
{
  int i ;
  char *tmp_str   ;
  int   tmp_int   ;
  int   start_val ;

  /****
   *  This is the structure to use
   */
  struct data_description *dd;


  /****
   *  To get for example request data 
   *  you use this function
   *
   *  I figure you should use this function
   *  when filling data to the "add ranges" window
   */
  dd = xnee_get_request_names();


  /****
   *  Ok... how to get all requests
   */
  for (i=0; dd[i].data_nr !=-1 ; i++)
    {
      /****
       *  print 'em 
       */
      printf (" %.3d\t%25s\t\"%s\" -- ",
	      dd[i].data_nr,
	      dd[i].data_name,
	      dd[i].data_descr);
      
      /****
       *  use the number of the request to ...
       */
      start_val = dd[i].data_nr;
      printf ("%d ", start_val);

      /****
       *  ... get the string representation and use this string
       *
       *  I figure you should use this function
       *  when filling data name into the "add ranges" window
       */
      tmp_str = xnee_int2request(start_val);
      printf (" --> %s", tmp_str);
      
      /****
       *  ... get the integer representation again
       */
      tmp_int = xnee_request2int(tmp_str);
      printf (" --> %d\n", tmp_int);

      /****
       *  ... which really should be the start_value
       */
      if (start_val != tmp_int)
	{
	  printf ("Failure when retrieving: %d %d\n", start_val, tmp_int);
	  exit(1);
	}
    }
  return 0;
}


int main()
{


  printf ("Request:\n");
  print_em_all_sucker();
		      
  return 0;
}
