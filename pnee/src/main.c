/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 2005, 2006 Henrik Sandklef 
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


#include <pnee_impl.h>
#include <callbacks.h>

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

volatile pnee_panel_applet pnee_applet_impl;
pnee_panel_applet *pnee_applet;

void 
pnee_panel_applet_create (pnee_panel_applet *pnee_applet_in);




static const char Context_menu_xml [] =
   "<popup name=\"button3\">\n"
/*   "   <menuitem name=\"Record Item\" "
   "             verb=\"Record\" "
   "           _label=\"_Record session...\"\n"
   "          pixtype=\"stock\" "
   "          pixname=\"gtk-media-record\"/>\n"
   "   <menuitem name=\"Replay Item\" "
   "             verb=\"Replay\" "
   "           _label=\"_Replay session...\"\n"
   "          pixtype=\"stock\" "
   "          pixname=\"gtk-media-play\"/>\n"*/
   "   <menuitem name=\"Properties Item\" "
   "             verb=\"Properties\" "
   "           _label=\"_Preferences...\"\n"
   "          pixtype=\"stock\" "
   "          pixname=\"gtk-properties\"/>\n"
   "   <menuitem name=\"About Item\" "
   "             verb=\"About\" "
   "           _label=\"_About...\"\n"
   "          pixtype=\"stock\" "
   "          pixname=\"gnome-stock-about\"/>\n"
   "</popup>\n";




#ifndef FAKED_MAIN
static const BonoboUIVerb myexample_menu_verbs [] = 
  {
    /*    
	  BONOBO_UI_VERB ("Record",      pnee_start_recording),
	  BONOBO_UI_VERB ("Replay",      pnee_start_replaying),
    */
    BONOBO_UI_VERB ("Properties",  display_properties_dialog),
    BONOBO_UI_VERB ("About",       display_about_dialog),
    BONOBO_UI_VERB_END
  };
#endif



#ifndef FAKED_MAIN
static gboolean
pnee_applet_fill (PanelApplet *applet,
		  const gchar *iid,
		  gpointer data)
{
  pnee_applet_impl.action_state = PNEE_ACTION_NONE;
  pnee_applet_impl.update_state = PNEE_UPDATE_NONE;
  pnee_applet = (pnee_panel_applet*) &pnee_applet_impl;  
  
  if (strcmp (iid, "OAFIID:pnee") != 0)
    {
      return FALSE;
    }

  pnee_applet->applet = applet;
  pnee_panel_applet_create (pnee_applet);
  
  return TRUE;
}
#endif


void 
pnee_panel_applet_create (pnee_panel_applet *pnee_applet_in)
{
/*   GtkTooltips *tooltips; */
  AtkObject *atk_obj;
  sem_init(&pnee_applet->action_mutex, 0, 10);
  sem_init(&pnee_applet->update_mutex, 0, 1);

  fprintf(stderr, "Using dir: '%s' \n", PNEE_PIXDIR);
  add_pixmap_directory (PNEE_PIXDIR);

  pnee_setup(pnee_applet_in);
    
  create_delay_splash();

#ifdef PNEE_DEBUGGER
  pthread_create (&pnee_applet->xnee_debugger, 
		  NULL, 
		  pnee_debugger, 
		  pnee_applet);
#endif /* PNEE_DEBUGGER */

    init_stock_icons(pnee_applet);

    panel_applet_set_flags (pnee_applet->applet, 
			    PANEL_APPLET_EXPAND_MINOR);

    pnee_applet->container = gtk_vbox_new (FALSE,0);
    gtk_container_add (GTK_CONTAINER (pnee_applet->applet), 
		       pnee_applet->container);

/*     tooltips = gtk_tooltips_new(); */

/*     gtk_tooltips_set_tip(tooltips, GTK_WIDGET(pnee_applet->applet), _("GNU Xnee applet"), NULL); */

    atk_obj = gtk_widget_get_accessible (GTK_WIDGET (pnee_applet->applet));
    if (GTK_IS_ACCESSIBLE (atk_obj))
	   atk_object_set_name (atk_obj, _("GNU Xnee panel"));

  
    gtk_widget_show_all (GTK_WIDGET (pnee_applet->applet));
    
    panel_applet_setup_menu (PANEL_APPLET (pnee_applet->applet),
			     Context_menu_xml,
			     myexample_menu_verbs,
			     pnee_applet->applet);
    
/*     pnee_applet->tooltips = tooltips; */

    /* Create the visual stuff */
    pnee_applet->progress = gtk_progress_bar_new ();
    gtk_progress_bar_set_orientation(GTK_PROGRESS_BAR(pnee_applet->progress),
				     GTK_PROGRESS_LEFT_TO_RIGHT);
    gtk_widget_set_size_request(pnee_applet->progress, 10, 5);

    gtk_container_set_border_width (GTK_CONTAINER (pnee_applet->container), 0);
    pnee_applet->button_box = gtk_hbox_new(FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (pnee_applet->button_box), 0);

    pnee_create_button(pnee_applet,
		       "Record" , 
		       "pnee-record.png" , 
		       pnee_rec_pressed  ,
		       PNEE_BUTTON_RECORD);

    pnee_create_button(pnee_applet,
		       "Play" , 
		       "pnee-replay.png" , 
		       pnee_rep_pressed  ,
		       PNEE_BUTTON_REPLAY);

    pnee_create_button(pnee_applet,
		       "Stop" , 
		       "pnee-stop.png" , 
		       pnee_stop_pressed  ,
		       PNEE_BUTTON_STOP);

/*       pnee_applet->rec_image = gtk_image_new_from_file("pnee-record.png"); */

/*     pnee_applet->rep_image = gtk_image_new_from_file("/usr/share/pixmaps/pnee-replay.png"); */

    gtk_box_pack_start (GTK_BOX (pnee_applet->button_box), 
			pnee_applet->rec_button, TRUE, TRUE, 0);

    gtk_box_pack_start (GTK_BOX (pnee_applet->button_box), 
			pnee_applet->stop_button, TRUE, TRUE, 0);

    gtk_box_pack_start (GTK_BOX (pnee_applet->button_box), 
			pnee_applet->rep_button, TRUE, TRUE, 0);

    gtk_box_pack_start (GTK_BOX (pnee_applet->container), 
			pnee_applet->button_box, TRUE, TRUE, 0);

    gtk_box_pack_start (GTK_BOX (pnee_applet->container),   
			  pnee_applet->progress, TRUE, TRUE, 0); 

    gtk_widget_show_all (pnee_applet->container);

    pthread_create (&pnee_applet->xnee_updater, 
		    NULL, 
		    pnee_progress_updater,
		    pnee_applet);

    return;
}



#ifdef FAKED_MAIN
int
read_cmd()
{
  
}

int
main (int argc, char *argv[])
{

  gtk_set_locale ();
  gtk_init (&argc, &argv);


/*   add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps"); */
  add_pixmap_directory (PNEE_PIXDIR);
  fprintf(stderr, "Using dir: '%s' \n", PNEE_PIXDIR);
  pnee_setup();

  if (pnee_panel->pnee_pref==NULL)
    {
      pnee_panel->pnee_pref = create_pnee_panel->pnee_pref ();
    }
  gtk_widget_show (pnee_panel->pnee_pref);
  /*
   * The following code was added by Glade to create one of each component
   * (except popup menus), just so that you see something after building
   * the project. Delete any components that you don't want shown initially.
   */
  gtk_main (); 
  return 0;
}

#else
PANEL_APPLET_BONOBO_FACTORY ("OAFIID:pnee_Factory",
                             PANEL_TYPE_APPLET,
                             "The Xnee Applet",
                             "0",
                             pnee_applet_fill,
                             NULL);
#endif
