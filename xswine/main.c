/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 2010 Henrik Sandklef      
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


#include <stdio.h>
#include <time.h>

#define XSWINE_OK 0 
typedef unsigned long Time;

FILE *swmouse0;
FILE *swmouse1;

typedef struct 
{
  int  xnee_type;
  int  type;
  int  abs_x;
  int  abs_y;
  int  button ;    
  int  keycode ;   
  int  screen_nr;
  Time current_time;
  int  deviceid;
  char name[100];
} xswine_replay_data;

static int 
xswine_handle_line(char *line)
{
  xswine_replay_data xrd;
  static Time        previous_time;
  Time        time_diff;
  int ret;
  FILE *fp;

  switch (line[0])
    {
    case '#':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
      /* discard */
      break;
    case '7':
      ret = sscanf(line, "%d,%d,%d,%d,%d,%d,%d,%lu,%d,'%s'",
		   &xrd.xnee_type, 
		   &xrd.type, 
		   &xrd.abs_x,
		   &xrd.abs_y, 
		   &xrd.button,
		   &xrd.keycode, 
		   &xrd.screen_nr,
		   &xrd.current_time,
		   &xrd.deviceid,
		   xrd.name);

      if (xrd.deviceid==9)
	{
	  fp = swmouse0;
	}
      else
	{
	  fp = swmouse1;
	}

      if ( previous_time )
	{
	  time_diff = xrd.current_time - previous_time ;
	  /* fprintf (stderr," diff %d\n", time_diff); */
	  usleep(time_diff*1000);
	}
      previous_time = xrd.current_time;


      /* printf ("Replay %dx%d on %d to %d\n", xrd.abs_x, xrd.abs_y, xrd.deviceid, fp);  */

      ret = fprintf(fp, "a %d %d\n", xrd.abs_x, xrd.abs_y);
      /* fprintf(stdout, "a %d %d    ret=%d\n", xrd.abs_x, xrd.abs_y, ret); */
      fflush(fp);
      break;
    default:
      /* discard */
      break;
    }
  return XSWINE_OK;
}

static int
xswine_read_input(FILE *fp)
{
  char buf[256];
  int cont = 1;
  char* ret_str ;

  while (cont )
    {
      ret_str = fgets(buf, 256, stdin);
      if (ret_str == NULL) { cont = 0; }
      xswine_handle_line(buf);
    }
}


int 
main(int argc, char **argv)
{
  FILE *fp;
  int ret;

  swmouse0 = fopen ("/dev/swmouse0", "a+");
  swmouse1 = fopen ("/dev/swmouse1", "a+");

  if (argc>2)
    {
      fp = fopen(argv[1],"r");
      if (fp==NULL)
	{
	  fprintf(stderr, "Failed to open file: '%s'\n", argv[1]);
	  return(1);
	}
    }
  else
    {
      fp = stdin;
    }
  ret = xswine_read_input(fp);
  return ret;
}
