#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <X11/Xlib.h>


enum 
{
  X   = 0,
  Y   = 2, 
  ALL = 4,
  ROOT= 8,
  WIN = 16
} xy_mask;

int xg_check( char *arg, char *long_arg , char *short_arg ) 
{
  /* returns 1 if arg was equal to any of long_arg or short_arg, 
     else it returns 0*/
  return ( ! strcmp ( arg, long_arg) || ( ! strcmp ( arg, short_arg) )) ; 
}

#define MOUSE_POS_ROOT_X_LONG    "--mouse-position-root-x"
#define MOUSE_POS_ROOT_X_SHORT   "-mprx"
#define MOUSE_POS_ROOT_Y_LONG    "--mouse-position-root-y"
#define MOUSE_POS_ROOT_Y_SHORT   "-mpry"
#define MOUSE_POS_ROOT_ALL_LONG  "--mouse-position-root-all"
#define MOUSE_POS_ROOT_ALL_SHORT "-mpra"

#define MOUSE_POS_WIN_X_LONG    "--mouse-position-win-x"
#define MOUSE_POS_WIN_X_SHORT   "-mpwx"
#define MOUSE_POS_WIN_Y_LONG    "--mouse-position-win-y"
#define MOUSE_POS_WIN_Y_SHORT   "-mpwy"
#define MOUSE_POS_WIN_ALL_LONG  "--mouse-position-win-all"
#define MOUSE_POS_WIN_ALL_SHORT "-mpwa"

#define RESOLUTION_X_LONG   "--resolution-x"
#define RESOLUTION_Y_LONG   "--resolution-y"
#define RESOLUTION_XY_LONG  "--resolution-xy"
#define RESOLUTION_X_SHORT  "-rx"
#define RESOLUTION_Y_SHORT  "-ry"
#define RESOLUTION_XY_SHORT "-rxy"
  
#define DISPLAY_LONG        "--display"
#define DISPLAY_SHORT       "-d"
#define SCREEN_LONG         "--screen"
#define SCREEN_SHORT        "-s"

int 
get_pointer_pos(Display *dpy, int all, int root_or_win);
int 
get_resolution_pos(Display *dpy, int all);

static int screen = 0;

int 
main(int argc, char **argv)
{
  Display *dpy;
  int i;
  

  dpy = XOpenDisplay(NULL);
  for (i=1;i<argc;i++)
    {
      if (xg_check (argv[i],
		    MOUSE_POS_ROOT_X_LONG , 
		    MOUSE_POS_ROOT_X_SHORT))
	get_pointer(dpy, X, ROOT);
      else if (xg_check (argv[i], 
			 MOUSE_POS_ROOT_Y_LONG, 
			 MOUSE_POS_ROOT_Y_SHORT))
	get_pointer(dpy, Y, ROOT);
      else if (xg_check (argv[i], 
			 MOUSE_POS_ROOT_ALL_LONG, 
			 MOUSE_POS_ROOT_ALL_SHORT))
	get_pointer(dpy, ALL, ROOT);
      else if (xg_check (argv[i],
		    MOUSE_POS_WIN_X_LONG , 
		    MOUSE_POS_WIN_X_SHORT))
	get_pointer(dpy, X, WIN);
      else if (xg_check (argv[i], 
			 MOUSE_POS_WIN_Y_LONG, 
			 MOUSE_POS_WIN_Y_SHORT))
	get_pointer(dpy, Y, WIN);
      else if (xg_check (argv[i], 
			 MOUSE_POS_WIN_ALL_LONG, 
			 MOUSE_POS_WIN_ALL_SHORT))
	get_pointer(dpy, ALL, WIN);
      else if (xg_check (argv[i], 
			 RESOLUTION_X_LONG, 
			 RESOLUTION_X_SHORT))
	get_resolution (dpy, X );
      else if (xg_check (argv[i], 
			 RESOLUTION_Y_LONG, 
			 RESOLUTION_Y_SHORT))
	get_resolution (dpy, Y );
      else if (xg_check (argv[i], 
			 RESOLUTION_XY_LONG, 
			 RESOLUTION_XY_SHORT))
	get_resolution (dpy, ALL );
      else if (xg_check (argv[i], 
			 DISPLAY_LONG, 
			 DISPLAY_SHORT))
	dpy = XOpenDisplay (argv[i]);
      else if (xg_check (argv[i], 
			 SCREEN_LONG, 
			 SCREEN_SHORT))
	if (! sscanf (argv[i+1], "%d", &screen))
	  fprintf (stderr,
		   "Could not use argument '%s'\n", argv[++i]);
	else
	  i++;
      else
	printf ("Error in parser ... \"%s\"\n", argv[i]);
    }
  fflush(stdout);
  XCloseDisplay(dpy);
  exit(0);
}

int 
get_pointer(Display *dpy, int all, int root_or_win)
{
  int rx;
  int ry;
  int wx;
  int wy;
  Window root;
  Window child;
  unsigned int mask;
  Bool bo;

  bo = XQueryPointer(dpy,
		     RootWindow(dpy,0),
		     &root,
		     &child,
		     &rx,
		     &ry,
		     &wx,
		     &wy,
		     &mask);

  if (root_or_win==ROOT)
    {
      if (all==X)
	printf ("%d\n", rx);
      else if (all==Y)
	printf ("%d\n", ry);
      else if (all==ALL)
	printf ("%dx%d\n", rx,ry);
    }
  else if (root_or_win==WIN)
    {
      if (all==X)
	printf ("%d\n", wx);
      else if (all==Y)
	printf ("%d\n", wy);
      else if (all==ALL)
	printf ("%dx%d\n", wx,wy);
    }
}


int 
get_resolution(Display *dpy, int all)
{
  int my_screen = 0 ;
  if (screen!=0)
    my_screen=screen;
  else
    screen = DefaultScreen(dpy);

  if (all==X)
    printf ("%d\n", DisplayWidth(dpy,screen));
  else if (all==Y)
    printf ("%d\n", DisplayHeight(dpy,screen));
  else if (all==ALL)
    printf ("%dx%d\n", DisplayWidth(dpy,screen),DisplayHeight(dpy,screen));
}

