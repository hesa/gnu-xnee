#ifndef PNEE_IMPL_H
#define PNEE_IMPL_H
#include <string.h>

#include <gdk/gdk.h>
#include <gnome.h>
#include <gtk/gtk.h>
#include <gtk/gtklabel.h>
#include <gtk/gtkbox.h>
#include <gtk/gtkimage.h>
#include <gtk/gtkimage.h>


#include <pthread.h>


#include <panel-applet.h>

#include "libxnee/xnee.h"
#include "libxnee/xnee_alloc.h"
#include "libxnee/print_varargs.h"
#include "libxnee/print.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_grab.h"
#include "libxnee/xnee_range.h"
#include "libxnee/xnee_session.h"
#include "libxnee/xnee_fileop.h"

#include <callbacks.h>
#include <interface.h>

#define PTHREAD_RETURN_VAL void*
#define PTHREAD_RETURN return NULL


typedef enum _pnee_actions
  {
    PNEE_ACTION_NONE,
    PNEE_ACTION_RECORD,
    PNEE_ACTION_REPLAY
  } pnee_actions;


typedef enum _pnee_update_actions
  {
    PNEE_UPDATE_NONE,
    PNEE_UPDATE_RECORD,
    PNEE_UPDATE_REPLAY
  } pnee_update_actions;

typedef enum _pnee_button_type
  {
    PNEE_BUTTON_RECORD,
    PNEE_BUTTON_REPLAY,
    PNEE_BUTTON_STOP
  } pnee_button_type;

typedef struct
{
  PanelApplet *applet;
  GtkWidget *button_box;	
  GtkWidget *label;	
  GtkWidget *container;
  GtkTooltips *tooltips;

  GtkWidget *rec_button;
  GtkWidget *rep_button;
  GtkWidget *stop_button;

  GtkWidget *rec_image;
  GtkWidget *rep_image;
  GtkWidget *stop_image;

  GtkTooltips* rec_tooltip;
  GtkTooltips* rep_tooltip;
  GtkTooltips* stop_tooltip;
  GtkWidget *progress;

  GtkIconSize icon_size;

  pthread_t  xnee_thread;
  pthread_t  xnee_updater;

  sem_t      action_mutex;
  GtkWidget *pnee_pref;

  volatile pnee_actions action_state ;
  volatile pnee_update_actions update_state ;


  xnee_data *xd ;

/* #define FAKED_MAIN */
#define PNEE_DEBUGGER
#ifdef PNEE_DEBUGGER
  pthread_t xnee_debugger;
#endif /* PNEE_DEBUGGER */
} pnee_panel_applet;

extern GtkWidget* fs;
extern gchar *filename;
extern pnee_panel_applet *pnee_applet;


#define THREAD_WAIT_READY   { while (action_state!=PNEE_ACTION_NONE) { usleep (1000*50); fprintf(stderr, "twr "); }
#define RETURN_IF_THREAD_BUSY   { if (action_state!=PNEE_ACTION_NONE) { fprintf(stderr, "\n\n ********** thread busy (%d) , leaving\n\n", action_state); return ; } }

#define DEBUG_MARK()  fprintf(stderr, "  ---  %s:%d  %s()\n", __FILE__, __LINE__, __func__);

#define _IN  fprintf (stderr, "********** Taking sem (at %d)\n", __LINE__); fflush(stderr); gdk_threads_enter(); fprintf (stderr, "**********  sem taken (at %d)\n", __LINE__); fflush(stderr) 
#define _OUT  fprintf (stderr, "********** Releasing sem\n"); fflush(stderr); gdk_threads_leave()

/*
#define pnee_set_no_action()  _IN; DEBUG_MARK(); action_state=PNEE_ACTION_NONE   ;   DEBUG_MARK();  _OUT
#define pnee_set_no_action_no_thread() _IN; action_state=PNEE_ACTION_NONE ; _OUT
#define pnee_set_recording() _IN; action_state=PNEE_ACTION_RECORD ;    _OUT
#define pnee_set_replaying() _IN; action_state=PNEE_ACTION_REPLAY ;    _OUT
*/
#define pnee_set_no_action(p)  DEBUG_MARK(); p->action_state=PNEE_ACTION_NONE 
#define pnee_set_no_action_no_thread(p) p->action_state=PNEE_ACTION_NONE 
#define pnee_set_recording(p)  p->action_state=PNEE_ACTION_RECORD 
#define pnee_set_replaying(p)  p->action_state=PNEE_ACTION_REPLAY 
#define pnee_is_recording(p)   (p->action_state==PNEE_ACTION_RECORD)
#define pnee_is_replaying(p)   (p->action_state==PNEE_ACTION_REPLAY) 
#define pnee_is_ready(p)       (p->action_state==PNEE_ACTION_NONE) 


#define pnee_set_update_no_action(p) p->update_state=PNEE_UPDATE_NONE 
#define pnee_set_update_recording(p) p->update_state=PNEE_UPDATE_RECORD 
#define pnee_set_update_replaying(p) p->update_state=PNEE_UPDATE_REPLAY 

#define GCHAR_TO_CHAR(a) ((char *) (a))

#define pnee_show_states(p)       \
     fprintf(stderr,       \
	     " pnee status : %d %d  threads: %x %x \n", \
	     p->action_state, \
	     p->update_state, \
	     (int)p->xnee_thread, \
	     (int)p->xnee_updater);

#ifdef PNEE_DEBUGGER
PTHREAD_RETURN_VAL
pnee_debugger(void *pnee_applet_in);
#endif



void
pnee_create_button(pnee_panel_applet *pnee_applet_in,
		   const gchar *tooltip_str, 
		   const gchar *id, 
		   GCallback    press_callback, 
		   pnee_button_type mode);

int
pnee_handle_xerr(Display *dpy, XErrorEvent *errevent);


int
pnee_prepare_record(void);


int
pnee_prepare_replay(void);


void
display_properties_dialog(BonoboUIComponent           *component,
			  gpointer                     user_data,
			  const char                  *cname);

void
display_about_dialog(BonoboUIComponent           *component,
		     gpointer                     user_data,
		     const char                  *cname);

void
init_stock_icons(pnee_panel_applet *pnee_applet_in);

void
register_stock_icons(pnee_panel_applet *pnee_applet_in, 
		     GtkIconFactory *factory);

gboolean
cb_button_press(GtkWidget *w, GdkEventButton *event, GtkWidget *applet);


PTHREAD_RETURN_VAL
pnee_stop_pressed_impl(void *pnee_applet);



PTHREAD_RETURN_VAL
pnee_start_recording(void *pnee_applet);

PTHREAD_RETURN_VAL
pnee_start_replaying(void *pnee_applet);

PTHREAD_RETURN_VAL
pnee_progress_updater(void *pnee_applet);


int
pnee_update_progress(pnee_panel_applet *pnee_applet, 
                     double perc);

 

PTHREAD_RETURN_VAL
pnee_progress_updater(void *pnee_applet);

int
pnee_update_progress(pnee_panel_applet *pnee_applet, 
                     double perc);

int 
pnee_setup(pnee_panel_applet *pnee_applet);

#endif /* PNEE_IMPL_H */
