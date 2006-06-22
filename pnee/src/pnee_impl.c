#include <pnee_impl.h>

xnee_data *xd;

PTHREAD_RETURN_VAL
pnee_start_recording(void *pnee_applet_in)
{
  int ret ;
  pnee_panel_applet *pa = (pnee_panel_applet *) pnee_applet_in;

  pnee_set_recording(pa);
  pnee_set_update_no_action(pa);

  /* HESA REMOVE THE LINE BELOW */
  xnee_print_grabbed_keys(pa->xd);

  xnee_set_events_recorded(pa->xd,0);
  fprintf(stderr, " ---> recording %d events\n", 
	  xnee_get_events_max(pa->xd));

  fprintf(stderr, " --- prepare \n");
  pnee_prepare_record();
  fprintf(stderr, " --- prepared \n"); 

  fprintf(stderr, " --- start record \n");
  ret=xnee_start(pa->xd);
  fprintf(stderr, " --- stopped record \n");
  
  pnee_set_rep_file (xnee_get_data_name(pa->xd));

  fprintf(stderr, " want to leave recording \n");
  pnee_show_states(pa);
  pnee_set_no_action(pa);
  fprintf(stderr, " <--- recording \n");
  pnee_show_states(pa);
  PTHREAD_RETURN;
}

PTHREAD_RETURN_VAL
pnee_start_replaying(void *pnee_applet_in)
{
  int ret;
  pnee_panel_applet *pa = (pnee_panel_applet *) pnee_applet_in;

  pnee_set_replaying(pa);
  pnee_set_update_no_action(pa);

  fprintf(stderr, " ---> replaying \n");
  fprintf(stderr, " ---> replaying %d events\n", 
	  xnee_get_events_max(pa->xd));

  pnee_prepare_replay();

  ret=xnee_start(pa->xd);
/*   xnee_close_down(xd);  */

  fprintf(stderr, " want to leave replaying \n");
  pnee_show_states(pa);
  pnee_set_no_action(pa);
  fprintf(stderr, " <--- replaying \n");
  pnee_show_states(pa);
  PTHREAD_RETURN;
}


PTHREAD_RETURN_VAL
pnee_progress_updater(void *pnee_applet_in)
{
  gint max ;
  gint curr;
  gdouble perc ;
  pnee_panel_applet *pa = (pnee_panel_applet *) pnee_applet_in;

  /*
  GtkWidget *wid;
  GdkWindow *win; 
  */

  while (1)
    {
      while ( pnee_is_replaying(pa) || pnee_is_recording(pa)  )
	{
	  if ( pnee_is_replaying(pa) )
	    {
	      pnee_set_update_replaying(pa);
	    }
	  else if ( pnee_is_recording(pa) )
	    {
	      pnee_set_update_recording(pa);
	    }
	  
	  max  = xnee_get_events_max(pa->xd);
	  if ( pnee_is_replaying(pa))
	    {
	      curr = xnee_get_events_replayed(pa->xd);
	    }
	  else
	    {
	      curr = xnee_get_events_recorded (pa->xd);
	    }
	  
	  perc = (gdouble) curr / (gdouble) max ;
	  
	  fprintf(stderr, " update: %d %d ===>  perc = %f \n", 
		  curr, max, perc);
	  
	  pnee_update_progress(pa, perc);
	  
	  usleep(1000*200);
	}
      pnee_set_update_no_action(pa);
      pnee_update_progress(pa, 0.0);
      /*
	wid = pnee_applet->progress;
	if (pnee_applet->progress)
	{
	win = gtk_widget_get_root_window (wid); 
	}
	if (win!=NULL)
	{
	gdk_window_process_updates (win, TRUE); 
	}
      */
      usleep(1000*1000);
    }
}

int
pnee_update_progress(pnee_panel_applet *pnee_applet_in, 
		     double perc)
{

  fprintf(stderr, "Updating to %f  on %u %u  ", 
	  perc, 
	  (unsigned int)pnee_applet_in, 
	  (unsigned int)pnee_applet_in->progress); fflush(stderr);

  if ( (pnee_applet_in!=0) && (pnee_applet_in->progress!=0) )
    {
      gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(pnee_applet_in->progress), 
				    perc);
      fprintf(stderr, "OK\n");fflush(stderr);
    }
  else
    {
      fprintf(stderr, "Update FAILED\n");fflush(stderr);
    }
  return 0;
}
















void
pnee_create_button(pnee_panel_applet *pnee_applet_in,
		   const gchar *tooltip_str, 
		   const gchar *id, 
		   GCallback    press_callback, 
		   pnee_button_type mode)
{
  GtkWidget *button;	
  GtkWidget *image;	
  GtkTooltips* tooltip;

  /* Button */
  button = gtk_button_new();
  fprintf (stderr, " button %d\n", (int)button);
  gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_HALF);
  gtk_container_set_border_width (GTK_CONTAINER (button), 0);

  /* Image */
  image = gtk_image_new_from_stock(id, pnee_applet->icon_size);
  tooltip= gtk_tooltips_new();
  gtk_widget_show(image);

  /* Tooltip */
  gtk_container_add(GTK_CONTAINER(button), image);
  gtk_tooltips_set_tip(tooltip, button, tooltip_str, NULL);

  /* callback */
  g_signal_connect(button, "button_press_event",
		   G_CALLBACK(cb_button_press), pnee_applet->applet);
  g_signal_connect(button, "clicked", 
		   G_CALLBACK(press_callback), 
		   pnee_applet);

  gtk_widget_show(button);

  if (mode == PNEE_BUTTON_RECORD )
    {
      fprintf(stderr, "Adding to rec\n");
      pnee_applet->rec_button=button;
      pnee_applet->rec_image=image;
      pnee_applet->rec_tooltip=tooltip;
    }
  else if (mode == PNEE_BUTTON_REPLAY )
    {
      fprintf(stderr, "Adding to replay\n");
      pnee_applet->rep_button=button;
      pnee_applet->rep_image=image;
      pnee_applet->rep_tooltip=tooltip;
    } 
  else if (mode == PNEE_BUTTON_STOP )
    {
      fprintf(stderr, "Adding to stop\n");
      pnee_applet->stop_button=button;
      pnee_applet->stop_image=image;
      pnee_applet->stop_tooltip=tooltip;
    } 
}


int
pnee_handle_xerr(Display *dpy, XErrorEvent *errevent)
{
  int ret ; 
  /*@out@*/ char tmp[100]="";

  if (errevent->type==0)
    {
       return XNEE_OK; 
    }
  else if (errevent->type==161)
    {
       return XNEE_OK; 
    }

  fprintf (stderr, "Error received:\n");
  fprintf (stderr, "\terror type       %u (0x%x)\n", 
	  errevent->type,errevent->type);
  fprintf (stderr, "\terror code       %u (0x%x)\n", 
	  errevent->error_code,errevent->error_code);
  fprintf (stderr, "\terror code major %u (0x%x)\n", 
	  errevent->request_code,errevent->request_code);
  fprintf (stderr, "\terror code minor %u (0x%x)\n", 
	  errevent->minor_code,errevent->minor_code);
  fprintf (stderr, "\tdisplay          %d (0x%x)\n", 
	  (int)dpy, (unsigned int)dpy);

  ret = XGetErrorText (dpy, 
		       (int) errevent->error_code, 
		       tmp, 
		       100);
  fprintf (stderr, " %s\n", (char *)tmp);
  fprintf (stderr, "This error can be ignored\n");

  usleep(1000*1000);

  return XNEE_OK;
}

int 
pnee_setup(pnee_panel_applet *pnee_panel_in)
{
  char *default_tmp_file;
  char *default_err_file;
  static int already_setup = 0;

  if (already_setup)
    {
      return 0;
    }
  already_setup = 1 ;

  /*  Get a new xnee_data structure  */
  xd = xnee_new_xnee_data();
  pnee_panel_in->xd = xd;

  default_err_file=pnee_get_default_err_name();

  (void) XSetErrorHandler (pnee_handle_xerr);  

  xnee_set_verbose(pnee_panel_in->xd);
  pnee_set_err_file (default_err_file); 
  xnee_open_files(pnee_panel_in->xd);
  xnee_unset_recall_window_pos (pnee_panel_in->xd);
  xnee_set_events_max (pnee_panel_in->xd, 200);
  xnee_set_data_max (pnee_panel_in->xd, -1);
  xnee_set_key (pnee_panel_in->xd, XNEE_GRAB_STOP, "F5");
  /*
    xnee_set_key (pnee_panel->xd, XNEE_GRAB_PAUSE, "p");
    xnee_set_key (pnee_panel->xd, XNEE_GRAB_RESUME, "r");
  */

    xnee_set_interval(pnee_panel_in->xd, 1);


  fs = create_filechooserdialog1();


  /* Set the program name */
  xnee_set_program_name (pnee_panel_in->xd, "pnee");

  /* Set the cli parameters */
  xnee_set_application_parameters (pnee_panel_in->xd, NULL);

  if (pnee_panel_in->pnee_pref==NULL)
    {
      pnee_panel_in->pnee_pref = 
	create_pnee_pref ();
    }

  default_tmp_file=pnee_get_default_filename();


  pnee_set_rec_file (default_tmp_file);
  pnee_set_rep_file (default_tmp_file);

  free (default_tmp_file);
  free (default_err_file);

  /* init threads   */
  fprintf(stderr, " ****** gdk_thread_init ******\n");
  gdk_threads_init ();
  gdk_threads_leave();


  return 0;
}

int
pnee_prepare_record(void)
{
  pnee_setup(pnee_applet);

  xnee_parse_range (pnee_applet->xd, XNEE_DEVICE_EVENT,  
		      "KeyPress-MotionNotify"); 
  xnee_set_recorder(pnee_applet->xd);
  xnee_prepare(pnee_applet->xd);

  return 0;
}

int
pnee_prepare_replay(void)
{
  gchar * my_file;
  GtkFileChooserButton *file_text ; 
  
  pnee_setup(pnee_applet);


  

  file_text = (GtkFileChooserButton*) lookup_widget(GTK_WIDGET(pnee_applet->pnee_pref),
						    "rep_choose_but");
  
  my_file = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(file_text)); 
      
  
  if (my_file!=NULL)
    {
      xnee_set_data_name (pnee_applet->xd, my_file);
    }
  
  xnee_set_replayer(pnee_applet->xd);
  xnee_unset_sync (pnee_applet->xd);
  xnee_prepare(pnee_applet->xd);

  return 0;
}







PTHREAD_RETURN_VAL
pnee_stop_pressed_impl(void *pnee_applet_in)
{
  gint ev_max;
  pnee_panel_applet *pa = (pnee_panel_applet *) pnee_applet_in;

  pnee_setup(pa);
  
  pnee_set_no_action(pa);

  fprintf(stderr, "stop pressed 0\n");

  ev_max = xnee_get_events_max(pa->xd);
  xnee_set_events_recorded (pa->xd, ev_max);

  pnee_set_no_action_no_thread(pa);
  pnee_set_update_no_action(pa);
  pnee_update_progress(pa, 0.0);

  PTHREAD_RETURN;
}

gboolean
cb_button_press(GtkWidget *w, GdkEventButton *event, GtkWidget *applet)
{
    if ((event->button == 3) || (event->button == 2))
    {
	gtk_propagate_event(applet, (GdkEvent *) event);

	return TRUE;
    }

    return FALSE;
}


void
register_stock_icons(pnee_panel_applet *pnee_applet_in, 
		     GtkIconFactory *factory)
{
    gint i;
    static gchar file_name[1024];
    const gchar *items[] = { "pnee-record.png", "pnee-replay.png" , "pnee-stop.png"};

    for (i = 0; i < sizeof(items) / sizeof(items[0]); ++i)
    {
	GdkPixbuf *pixbuf;
	GtkIconSet *icon_set;
        GError *error = NULL;
	
        sprintf(file_name, "%s/%s", PNEE_PIXDIR, items[i] );
	fprintf (stderr, "Trying to load file: %s\n", file_name);
	pixbuf = gdk_pixbuf_new_from_file(file_name, &error);
        if (pixbuf == NULL)
        {
	  fprintf(stderr, "no pixbuf...\n");
        }

	icon_set = gtk_icon_set_new_from_pixbuf(pixbuf);
	gtk_icon_factory_add(factory, items[i], icon_set);

	gtk_icon_set_unref(icon_set);
	g_object_unref(G_OBJECT(pixbuf));
    }
}


void
init_stock_icons(pnee_panel_applet *pnee_applet_in)
{
    GtkIconFactory *factory;

    factory = gtk_icon_factory_new();
    gtk_icon_factory_add_default(factory);

    register_stock_icons(pnee_applet, factory);

    pnee_applet->icon_size = gtk_icon_size_register("pnee_icon_size", 5, 5);

    g_object_unref(factory); 
}


void
display_about_dialog(BonoboUIComponent           *component,
		     gpointer                     user_data,
		     const char                  *cname)
{
  static GtkWidget *pnee_about = NULL ;

  if ( pnee_about == NULL)
    {
      pnee_about = create_pnee_about();
    }

  gtk_widget_show (pnee_about);


  return ;
}


void
display_properties_dialog(BonoboUIComponent           *component,
			  gpointer                     user_data,
			  const char                  *cname)
{
  if (pnee_applet->pnee_pref==NULL)
    {
      pnee_applet->pnee_pref = create_pnee_pref ();
    }

  gtk_widget_show (pnee_applet->pnee_pref);
  return ;
}


#ifdef PNEE_DEBUGGER
PTHREAD_RETURN_VAL
pnee_debugger(void *pnee_applet_in)
{
  pnee_panel_applet *pa = (pnee_panel_applet *) pnee_applet_in;

  while (1)
    {
      fprintf(stderr, "  debugger thread:: ");
      pnee_show_states(pa);
      fprintf(stderr, "  debugger thread:: ");
      usleep (1000*1000);
    }
  PTHREAD_RETURN;
}
#endif /* PNEE_DEBUGGER */


