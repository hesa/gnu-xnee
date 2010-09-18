#include <stdio.h>
#include <time.h>

#define XSWINE_OK 0 
typedef unsigned long Time;

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
  int ret;
  switch (line[0])
    {
    case '#':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
      /* discard */
      break;
    case '6':
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
      /* printf ("Replay for device %s:  %dx%d on %d\n", xrd.name, xrd.abs_x, xrd.abs_y, xrd.deviceid); */
      
      break;
    case '7':
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

  printf ("reading... from %u\n", fp);
  while (cont )
    {
      ret_str = fgets(buf, 256, stdin);
      if (ret_str == NULL) { cont = 0; }
      xswine_handle_line(buf);
    }
  printf ("reading done...\n");
}


int 
main(int argc, char **argv)
{
  FILE *fp;
  int ret;

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
