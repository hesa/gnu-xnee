#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>

#ifdef HAVE_STDARG_H
#include <stdarg.h>
#else
#include <varargs.h> 
#endif

#include "libxnee/feedback.h"

static int need_init=1;


typedef int (* feedback_fptr) (char*); 
static feedback_fptr feedback_fp ; 

#ifdef  HAVE_XOSD
#include "xosd.h"
int  osd_feedback (char *str);
void osd_printerror(void);
int  init_osd();
#endif
int 
err_feedback (char *str);

static int 
feedback_init()
{
  if (need_init)
    {
      need_init=0;
      feedback_fp = err_feedback;
    }
  return XNEE_OK;
}

#ifdef HAVE_STDARG_H
int 
feedback(char *str, ... )
{
   va_list ap;
   char buf[200];
   int conv;

   va_start(ap, str);
   feedback_init();
   conv = vsnprintf ((char*) buf, 
              200, 
              str, 
              ap );
   va_end(ap);
   buf[conv]='\0';
   feedback_fp (buf);
   osd_feedback (buf);
   return XNEE_OK;
}
#else
#errror do implement this ... please
#endif


int 
err_feedback (char *str)
{
   fprintf (stderr, "%s\n", str);
   fflush (stderr);
   return XNEE_OK;
}



#ifdef  HAVE_XOSD

static xosd *osd;

int 
osd_feedback (char *str );
void
osd_printerror(void)
{
  fprintf (stderr, "ERROR: %s\n", xosd_error);
}
int 
init_osd()
{
  printf ("--->init xosd\n");
  osd = xosd_create (2);


  printf ("--- init xosd\n");
   if (!osd)
     {
       osd_printerror ();
       return 1;
     }
   
  printf ("--- init xosd\n");
   if (0 != xosd_set_shadow_offset (osd, 2))
     {
       osd_printerror ();
    }
   if (0 != xosd_set_shadow_colour (osd, "blue1"))
     {
       osd_printerror ();
     }
   if (0 != xosd_set_outline_offset (osd, 1))
     {
       osd_printerror ();
     }
   if (0 != xosd_set_colour (osd, "yellow"))
     {
       osd_printerror ();
     }
  printf ("--- init xosd\n");
   if (0 != xosd_set_outline_colour (osd, "blue4"))
     {
       osd_printerror ();
     }
   if (0 != xosd_set_font (osd, (char *) osd_default_font))
     {
       osd_printerror ();
    }
  printf ("--- init xosd\n");
   if (0 != xosd_set_timeout (osd, FEEDBACK_TIMEOUT))
     {
       osd_printerror ();
     }
  printf ("<---init xosd\n");
  return XNEE_OK;
}


int 
osd_feedback (char *str)
{
  printf ("--->display xosd\n");
  sleep (1);
  xosd_display (osd, 0, XOSD_string, str);
  printf ("<---display xosd\n");
  return XNEE_OK;
}


#endif
