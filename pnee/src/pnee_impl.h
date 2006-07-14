/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 2005, 2006 Henrik Sandklef 
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

#include <interface.h>
#include <pnee_types.h>

#define PTHREAD_RETURN_VAL void*
#define PTHREAD_RETURN return NULL


#include <callbacks.h>

extern GtkWidget* fs;
extern gchar *filename;
extern pnee_panel_applet *pnee_applet;


#define DEBUG_MARK()  fprintf(stderr, "  ---  %s:%d  %s()\n", __FILE__, __LINE__, __func__);
#define DEBUG_IN()   fprintf(stderr, "  ----> IN   %s:%d  %s()\n", __FILE__, __LINE__, __func__);
#define DEBUG_IN2()  fprintf(stderr, "  <---  IN   %s:%d  %s()\n", __FILE__, __LINE__, __func__);
#define DEBUG_OUT()  fprintf(stderr, "  ----> OUT  %s:%d  %s()\n", __FILE__, __LINE__, __func__);
#define DEBUG_OUT2() fprintf(stderr, "  <---- OUT  %s:%d  %s()\n", __FILE__, __LINE__, __func__);

#define _IN   gdk_threads_enter()
#define _OUT  gdk_threads_leave()



/*
#define _IN   DEBUG_IN(); gdk_threads_enter(); DEBUG_IN2()
#define _OUT  DEBUG_OUT(); gdk_threads_leave(); DEBUG_OUT2()
*/
#define pnee_set_no_action(p)            p->action_state=PNEE_ACTION_NONE 
#define pnee_set_no_action_no_thread(p)  p->action_state=PNEE_ACTION_NONE 
#define pnee_set_recording(p)            p->action_state=PNEE_ACTION_RECORD 
#define pnee_set_replaying(p)            p->action_state=PNEE_ACTION_REPLAY 
#define pnee_set_configure(p)            p->action_state=PNEE_ACTION_CONFIGURE 
#define pnee_is_recording(p)             (p->action_state==PNEE_ACTION_RECORD)
#define pnee_is_replaying(p)             (p->action_state==PNEE_ACTION_REPLAY) 
#define pnee_is_configure(p)             (p->action_state==PNEE_ACTION_CONFIGURE) 
#define pnee_is_ready(p)                 (p->action_state==PNEE_ACTION_NONE) 


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


void
create_delay_splash (void);

void
pnee_2xnee_update_all(void);

#endif /* PNEE_IMPL_H */
