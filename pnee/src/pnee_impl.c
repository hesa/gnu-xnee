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


#include <pnee_impl.h>
#include <interface.h>
#include <support.h>
#include <callbacks.h>
#include "pnee_types.h"

xnee_data *xd;
extern pnee_panel_applet *pnee_applet;

#define PNEE_DELAY_RECORD 0
#define PNEE_DELAY_REPLAY 1

GtkWidget *my_delay;
GtkWidget *my_del_progr_win;
GtkWidget *my_delay_progr;
GtkWidget *my_delay_label;


static int 
pnee_handle_err(xnee_data *xd, int error)
{
  const char *err;
  const char *descr;

  if ( error != XNEE_OK_LEAVE )
    {
      err   = xnee_get_err_description(error);
      descr = xnee_get_err_solution(error);
      
      fprintf (stderr, "Error number: %d\n", error);
      fprintf (stderr, "  Error:      %s\n", err);
      fprintf (stderr, "  Solution:   %s\n", descr);
    }


  fprintf(stderr, " \n\n\n\n\n\n *** ERROR *** \n\n\n\n");
  return XNEE_OK;
}

GtkWidget *
create_delay_splash_impl (void)
{

  _IN;
  my_delay = gtk_window_new (GTK_WINDOW_POPUP);
  gtk_container_set_border_width (GTK_CONTAINER (my_delay), 3);
  gtk_window_set_title (GTK_WINDOW (my_delay), "pnee delay");
  gtk_window_set_position (GTK_WINDOW (my_delay), GTK_WIN_POS_CENTER);
  _OUT;

  _IN;
  my_del_progr_win = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (my_del_progr_win);
  gtk_container_add (GTK_CONTAINER (my_delay), my_del_progr_win);
  _OUT;

  _IN;
  my_delay_label = gtk_label_new ("Pnee - a GNU Xnee Applet");
  gtk_widget_show (my_delay_label);
  gtk_box_pack_start (GTK_BOX (my_del_progr_win), my_delay_label, FALSE,
		      FALSE, 0);
  /*   gtk_label_set_text (GTK_LABEL (my_delay_progr), "Delayd start of pnee"); */
  _OUT;

  _IN;
  my_delay_progr = gtk_progress_bar_new ();
  gtk_widget_show (my_delay_progr);
  gtk_box_pack_start (GTK_BOX (my_del_progr_win), my_delay_progr, FALSE,
		      FALSE, 0);
  gtk_progress_bar_set_text (GTK_PROGRESS_BAR (my_delay_progr),
			     "Delayd start of pnee");
  _OUT;


  /* Store pointers to all widgets, for use by lookup_widget(). */
  /*
     GLADE_HOOKUP_OBJECT_NO_REF (my_delay, my_delay, "delay");
     GLADE_HOOKUP_OBJECT (my_delay, my_del_progr_win, "del_progr_win");
     GLADE_HOOKUP_OBJECT (my_delay, my_delay_progr, "delay_progr");
   */
  return my_delay;
}

void
create_delay_splash (void)
{
  if (my_delay == NULL)
    {
      create_delay_splash_impl ();
    }
}


void
delay_start (int mode)
{
  gdouble perc;
  char buf[100];
  int delay_time ;
  int delay_time_start;

  pnee_set_interval(NULL, pnee_applet->pnee_pref);

  delay_time = xnee_get_interval (xd) ;
  delay_time_start = delay_time;

  fprintf(stderr, " ============================================================ delay: %d\n", 
	  delay_time);

  fprintf (stderr, "  ****** %s:%d       delay %d  \n", __FILE__, __LINE__, xnee_get_interval(pnee_applet->xd));
  create_delay_splash ();
  _IN;
  gtk_widget_show_all (my_delay);
  _OUT;

  while (delay_time != 0)
    {
      if (!pnee_is_recording (pnee_applet) &&
	  !pnee_is_replaying (pnee_applet))
	{
	  fprintf (stderr, "Action interrupted ...., leaving start\n");
	  _IN;
	  gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (my_delay_progr),
					 0.0);
	  _OUT;

  fprintf (stderr, "  ****** %s:%d       delay %d  \n", __FILE__, __LINE__, xnee_get_interval(pnee_applet->xd));
	  return;
	}

      perc = (gdouble) delay_time / (gdouble) delay_time_start;
      if (mode == PNEE_DELAY_RECORD)
	{
	  sprintf (buf, "Starting to record in %d seconds", delay_time);
	}
      else
	{
	  sprintf (buf, "Starting to replay in %d seconds", delay_time);
	}



      fprintf (stderr, "  delay_start->\n");
      sem_wait (&pnee_applet->update_mutex);
      fprintf (stderr, "  delay_start -- %u\n", (int) my_delay_progr);
      _IN;
      gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (my_delay_progr), perc);
      gtk_progress_bar_set_text (GTK_PROGRESS_BAR (my_delay_progr), buf);
      _OUT;
      fprintf (stderr, "  delay_start -- \n");
      sem_post (&pnee_applet->update_mutex);
      fprintf (stderr, "  <-delay_start\n");

      usleep (1000 * 1000);
      delay_time--;
    }

  _IN;
  gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (my_delay_progr), 0.0);
  _OUT;

  _IN;
  gtk_widget_hide_all (my_delay);
  _OUT;
  fprintf (stderr, "  ****** %s:%d       delay %d  \n", __FILE__, __LINE__, xnee_get_interval(pnee_applet->xd));
  xnee_set_interval(xd, 0);
  return;
}




PTHREAD_RETURN_VAL
pnee_start_recording (void *pnee_applet_in)
{
  int ret;

  pnee_panel_applet *pa = (pnee_panel_applet *) pnee_applet_in;

  create_delay_splash ();

  DEBUG_MARK();
  
  pnee_set_recording (pa);
  pnee_set_update_no_action (pa);

  xnee_set_events_recorded(pa->xd,0);
  pnee_prepare_record ();

  fprintf(stderr, "\n\n\n ********** RECORDING SETTINGS ******** \n\n\n");
  xnee_print_ranges(pa->xd, stderr);
  xnee_print_xnee_settings(pa->xd, stderr);

  pnee_set_interval(NULL, pa->pnee_pref);
  delay_start (PNEE_DELAY_RECORD);
  
  fprintf(stderr, "\n\n\n ********** START RECORDING ******** \n\n\n");
  ret = xnee_start (pa->xd);
  if ( ret != XNEE_OK)
    {
      pnee_handle_err(xd, ret);
    }
  fprintf(stderr, "\n\n\n ********** STOPED RECORDING  (%d) ******** \n\n\n", ret);
  DEBUG_MARK();

  pnee_set_no_action (pa);
  PTHREAD_RETURN;
}

PTHREAD_RETURN_VAL
pnee_start_replaying (void *pnee_applet_in)
{
  int ret;
  int delay;

  pnee_panel_applet *pa = (pnee_panel_applet *) pnee_applet_in;

  create_delay_splash ();


  pnee_set_replaying (pa);
  pnee_set_update_no_action (pa);

  pnee_prepare_replay ();

  delay_start (PNEE_DELAY_REPLAY);

  ret = xnee_start (pa->xd);

  pnee_set_no_action (pa);

  PTHREAD_RETURN;
}


PTHREAD_RETURN_VAL
pnee_progress_updater (void *pnee_applet_in)
{
  gint max;
  gint curr;
  gdouble perc;
  pnee_panel_applet *pa = (pnee_panel_applet *) pnee_applet_in;

  /*
     GtkWidget *wid;
     GdkWindow *win; 
   */

  while (1)
    {
      while (pnee_is_replaying (pa) || pnee_is_recording (pa))
	{
	  if (pnee_is_replaying (pa))
	    {
	      pnee_set_update_replaying (pa);
	    }
	  else if (pnee_is_recording (pa))
	    {
	      pnee_set_update_recording (pa);
	    }

	  max = xnee_get_events_max (pa->xd);
	  if (pnee_is_replaying (pa))
	    {
	      curr = xnee_get_events_replayed (pa->xd);
	    }
	  else
	    {
	      curr = xnee_get_events_recorded (pa->xd);
	    }

	  perc = (gdouble) curr / (gdouble) max;

	  pnee_update_progress (pa, perc);
	  usleep (1000 * 200);
	}

      pnee_set_update_no_action (pa);
      pnee_update_progress (pa, 0.0);
      usleep (1000 * 1000);
    }
}

int
pnee_update_progress (pnee_panel_applet * pnee_applet_in, double perc)
{
  if ((pnee_applet_in != 0) && (pnee_applet_in->progress != 0))
    {
      sem_wait (&pnee_applet->update_mutex);
      _IN;
      gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR
				     (pnee_applet_in->progress), perc);
      _OUT;
      sem_post (&pnee_applet->update_mutex);
    }
  return 0;
}
















void
pnee_create_button (pnee_panel_applet * pnee_applet_in,
		    const gchar * tooltip_str,
		    const gchar * id,
		    GCallback press_callback, pnee_button_type mode)
{
  GtkWidget *button;
  GtkWidget *image;
  GtkTooltips *tooltip;

  /* Button */
  button = gtk_button_new ();
  _IN;
  gtk_button_set_relief (GTK_BUTTON (button), GTK_RELIEF_HALF);
  gtk_container_set_border_width (GTK_CONTAINER (button), 0);
  _OUT;

  /* Image */
  _IN;
  image = gtk_image_new_from_stock (id, pnee_applet->icon_size);
  tooltip = gtk_tooltips_new ();
  gtk_widget_show (image);
  _OUT;

  /* Tooltip */
  _IN;
  gtk_container_add (GTK_CONTAINER (button), image);
  gtk_tooltips_set_tip (tooltip, button, tooltip_str, NULL);
  _OUT;

  /* callback */
  g_signal_connect (button, "button_press_event",
		    G_CALLBACK (cb_button_press), pnee_applet->applet);
  g_signal_connect (button, "clicked",
		    G_CALLBACK (press_callback), pnee_applet);

  _IN;
  gtk_widget_show (button);
  _OUT;

  if (mode == PNEE_BUTTON_RECORD)
    {
      pnee_applet->rec_button = button;
      pnee_applet->rec_image = image;
      pnee_applet->rec_tooltip = tooltip;
    }
  else if (mode == PNEE_BUTTON_REPLAY)
    {
      pnee_applet->rep_button = button;
      pnee_applet->rep_image = image;
      pnee_applet->rep_tooltip = tooltip;
    }
  else if (mode == PNEE_BUTTON_STOP)
    {
      pnee_applet->stop_button = button;
      pnee_applet->stop_image = image;
      pnee_applet->stop_tooltip = tooltip;
    }
}


int
pnee_handle_xerr (Display * dpy, XErrorEvent * errevent)
{
  int ret;
  /*@out@ */ char tmp[100] = "";

  if (errevent->type == 0)
    {
      return XNEE_OK;
    }
  else if (errevent->type == 161)
    {
      return XNEE_OK;
    }

  fprintf (stderr, "Error received:\n");
  fprintf (stderr, "\terror type       %u (0x%x)\n",
	   errevent->type, errevent->type);
  fprintf (stderr, "\terror code       %u (0x%x)\n",
	   errevent->error_code, errevent->error_code);
  fprintf (stderr, "\terror code major %u (0x%x)\n",
	   errevent->request_code, errevent->request_code);
  fprintf (stderr, "\terror code minor %u (0x%x)\n",
	   errevent->minor_code, errevent->minor_code);
  fprintf (stderr, "\tdisplay          %d (0x%x)\n",
	   (int) dpy, (unsigned int) dpy);

  ret = XGetErrorText (dpy, (int) errevent->error_code, tmp, 100);
  fprintf (stderr, " %s\n", (char *) tmp);
  fprintf (stderr, "This error can be ignored\n");

  usleep (1000 * 1000);

  return XNEE_OK;
}


int
pnee_xnee_init (pnee_panel_applet * pnee_panel_in)
{

  char *default_tmp_file;
  char *default_err_file;

  default_tmp_file = pnee_get_default_filename ();
  default_err_file = pnee_get_default_err_name ();

  /*  Get a new xnee_data structure  */
  xd = xnee_new_xnee_data ();
  pnee_panel_in->xd = xd;

  (void) XSetErrorHandler (pnee_handle_xerr);

  xnee_set_verbose (pnee_panel_in->xd);
  pnee_set_err_file (default_err_file);
  xnee_open_files (pnee_panel_in->xd);
  xnee_unset_recall_window_pos (pnee_panel_in->xd);
  xnee_set_events_max (pnee_panel_in->xd, 600);
  xnee_set_data_max (pnee_panel_in->xd, -1);
  xnee_set_key (pnee_panel_in->xd, XNEE_GRAB_STOP, "F5");


  /* Set the program name */
  xnee_set_program_name (pnee_panel_in->xd, "pnee");

  /* Set the cli parameters */
  xnee_set_application_parameters (pnee_panel_in->xd, NULL);

  pnee_set_rec_file (default_tmp_file);

  pnee_set_rep_file(default_tmp_file);

  free (default_tmp_file);
  free (default_err_file);

  fprintf (stderr, "  ****** %s:%d       delay %d  \n", __FILE__, __LINE__, xnee_get_interval(pnee_applet->xd));
  pnee_set_interval(NULL, pnee_panel_in->pnee_pref);
  fprintf (stderr, "  ****** %s:%d       delay %d  \n", __FILE__, __LINE__, xnee_get_interval(pnee_applet->xd));

  return XNEE_OK;
}




int
pnee_setup (pnee_panel_applet * pnee_panel_in)
{
  static int already_setup = 0;

  if (already_setup)
    {
      return 0;
    }
  already_setup = 1;

  fs = create_filechooserdialog1 ();


  if (pnee_panel_in->pnee_pref == NULL)
    {
      pnee_panel_in->pnee_pref = create_pnee_pref ();
    }

  pnee_xnee_init (pnee_panel_in);

  /* init threads   */
  gdk_threads_init ();
  gdk_threads_leave ();

  return 0;
}

int
pnee_prepare_record (void)
{
  pnee_setup (pnee_applet);
  
  pnee_2xnee_update_all();

  xnee_parse_range (pnee_applet->xd, XNEE_DEVICE_EVENT,
		    "KeyPress-MotionNotify");
  xnee_set_recorder (pnee_applet->xd);
  pnee_set_interval(NULL, pnee_applet->pnee_pref);

  return 0;
}

int
pnee_prepare_replay (void)
{
/*   gchar *my_file; */

  pnee_setup (pnee_applet);

  pnee_2xnee_update_all();

  xnee_set_replayer (pnee_applet->xd);
  xnee_unset_sync (pnee_applet->xd);
  pnee_set_interval(NULL, pnee_applet->pnee_pref);

  return 0;
}







PTHREAD_RETURN_VAL
pnee_stop_pressed_impl (void *pnee_applet_in)
{
/*   gint ev_max; */
  pnee_panel_applet *pa = (pnee_panel_applet *) pnee_applet_in;

  pnee_setup (pa);

  /* 
   * Make sure we are in action, 
   * otherwise we will prevent next cation (only once though)
   */
  if (pnee_is_replaying (pa) || pnee_is_recording (pa))
    {
      xnee_set_interrupt_action (xd);
    }

  pnee_set_no_action (pa);
  pnee_set_no_action_no_thread (pa);
  pnee_set_update_no_action (pa);

  _IN;
  gtk_widget_hide_all (my_delay);
  _OUT
;
  PTHREAD_RETURN;
}

gboolean
cb_button_press (GtkWidget * w, GdkEventButton * event, GtkWidget * applet)
{
  if ((event->button == 3) || (event->button == 2))
    {
      gtk_propagate_event (applet, (GdkEvent *) event);
      return TRUE;
    }

  return FALSE;
}


void
register_stock_icons (pnee_panel_applet * pnee_applet_in,
		      GtkIconFactory * factory)
{
  gint i;
  static gchar file_name[1024];
  const gchar *items[] =
    { "pnee-record.png", "pnee-replay.png", "pnee-stop.png" };

  for (i = 0; i < sizeof (items) / sizeof (items[0]); ++i)
    {
      GdkPixbuf *pixbuf;
      GtkIconSet *icon_set;
      GError *error = NULL;

      sprintf (file_name, "%s/%s", PNEE_PIXDIR, items[i]);
      pixbuf = gdk_pixbuf_new_from_file (file_name, &error);

      icon_set = gtk_icon_set_new_from_pixbuf (pixbuf);
      gtk_icon_factory_add (factory, items[i], icon_set);

      gtk_icon_set_unref (icon_set);
      g_object_unref (G_OBJECT (pixbuf));
    }
}


void
init_stock_icons (pnee_panel_applet * pnee_applet_in)
{
  GtkIconFactory *factory;

  factory = gtk_icon_factory_new ();
  gtk_icon_factory_add_default (factory);

  register_stock_icons (pnee_applet, factory);

  pnee_applet->icon_size = gtk_icon_size_register ("pnee_icon_size", 5, 5);

  g_object_unref (factory);
}


void
display_about_dialog (BonoboUIComponent * component,
		      gpointer user_data, const char *cname)
{
  static GtkWidget *pnee_about = NULL;

  /*
   * Prevent about to showwhile in action
   */
  if (pnee_is_replaying (pnee_applet) || pnee_is_recording (pnee_applet))
    {
      fprintf (stderr, "************** about blocked ");
      return;
    }

  if (pnee_about == NULL)
    {
      pnee_about = create_pnee_about ();
    }

  gtk_widget_show (pnee_about);


  return;
}


void
display_properties_dialog (BonoboUIComponent * component,
			   gpointer user_data, const char *cname)
{
  /*
   * Prevent properties to showwhile in action
   */
  if (pnee_is_replaying (pnee_applet) || pnee_is_recording (pnee_applet))
    {
      fprintf (stderr, "*****************  properties blocked ");
      return;
    }

  if (pnee_applet->pnee_pref == NULL)
    {
      pnee_applet->pnee_pref = create_pnee_pref ();
    }
  gtk_widget_show (pnee_applet->pnee_pref);
  return;
}

void
pnee_2xnee_update_err_name(void)
{
  GtkFileChooserButton *file_text;
  char *my_file ;

  file_text =
    (GtkFileChooserButton *)
    lookup_widget (GTK_WIDGET (pnee_applet->pnee_pref), "rep_choose_but");
  
  my_file = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (file_text));
  if (my_file != NULL)
    {
      xnee_set_data_name (pnee_applet->xd, my_file);
    }

}

void
pnee_2xnee_update_out_name(void)
{
  GtkFileChooserButton *file_text;
  char *my_file ;

  file_text =
    (GtkFileChooserButton *)
    lookup_widget (GTK_WIDGET (pnee_applet->pnee_pref), "rec_chooser_but");
  
  my_file = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (file_text));
  if (my_file != NULL)
    {
      xnee_set_out_name (pnee_applet->xd, my_file);
    }

}

void
pnee_2xnee_update_stop_key(void)
{
  GtkEntry *entry;
  char * my_key = NULL;

  entry =  (GtkEntry *)
	lookup_widget (GTK_WIDGET (pnee_applet->pnee_pref), "entry5");
  if ( entry != NULL)
    {
      my_key = GCHAR_TO_CHAR(gtk_entry_get_text (GTK_ENTRY(entry)));
    }
  if (my_key!=NULL)
    {
      xnee_set_key (xd, XNEE_GRAB_STOP, my_key);
    }
}

void
pnee_2xnee_update_all(void)
{


  if (xnee_is_replayer(pnee_applet->xd))
    {
      /* 
       *    update err file name 
       */
      pnee_2xnee_update_err_name();
    }
  else if (xnee_is_recorder(pnee_applet->xd))
    {
      /* 
       *    update out file name 
       */
      pnee_2xnee_update_err_name();
    }

  

  /*
   *   update stop key 
   */
  pnee_2xnee_update_stop_key();

}

#ifdef PNEE_DEBUGGER
PTHREAD_RETURN_VAL
pnee_debugger (void *pnee_applet_in)
{
  pnee_panel_applet *pa = (pnee_panel_applet *) pnee_applet_in;

  while (1)
    {
      fprintf (stderr, "  debugger thread:: ");
      pnee_show_states (pa);
      usleep (1000 * 1000);
    }
  PTHREAD_RETURN;
}
#endif /* PNEE_DEBUGGER */
