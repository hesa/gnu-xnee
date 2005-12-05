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

/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <gtk/gtk.h>

#include "interface.h"
#include "support.h"
#include "recordables.h"

#include <libxnee/xnee.h>
#include <libxnee/xnee_alloc.h>
#include <libxnee/xnee_session.h>
#include <gnee_xnee.h>

#define GNEE "gnee"

xnee_data   *ext_xd;
gnee_xnee   *ext_gx;
GtkWidget   *ext_gnee_window;

const char  *version_string = "--version" ;

int
main (int argc, char *argv[])
{
    GtkWidget          *gnee_window;

    GtkWidget          *exclude_list;
    GtkTreeStore       *exclude_store;
/*     GtkTreeIter         exclude_iterator;  */
    GtkCellRenderer    *exclude_list_renderer;
    GtkTreeViewColumn  *exclude_list_column;

    GtkWidget          *include_list;
    GtkTreeStore       *include_store;
/*     GtkTreeIter         include_iterator;  */
    GtkCellRenderer    *include_list_renderer;
    GtkTreeViewColumn  *include_list_column;

    GList              *combo_list = NULL;
    GtkWidget          *event_group_combo;
    char *default_tmp_file;
    int i ;

    xnee_data   *xd;
    gnee_xnee    gx; 

    ext_gx = &gx;
    gx_init_gx(ext_gx);


#ifdef ENABLE_NLS
    bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);
#endif

    gtk_set_locale ();
    gtk_init (&argc, &argv);


    add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");

    /*
     * The following code was added by Glade to create one of each component
     * (except popup menus), just so that you see something after building
     * the project. Delete any components that you don't want shown initially.
     */
    gnee_window = create_gnee_window ();
    ext_gnee_window = gnee_window;

/*     gnee_settings = gnee_settings_new(); */

    gnee_recordables_create(gnee_window);


    exclude_list  = lookup_widget(gnee_window, "exclude_list");
/*     exclude_store = gnee_settings->record_settings->events->exclude_store; */
    exclude_store = gtk_tree_store_new(1, G_TYPE_STRING);

    exclude_list_renderer = gtk_cell_renderer_text_new ();
    exclude_list_column   = gtk_tree_view_column_new_with_attributes
        ("Exclude when recording", 
         exclude_list_renderer, 
         "text", 
         0, 
         NULL);

    gtk_tree_view_append_column(GTK_TREE_VIEW (exclude_list), 
                                exclude_list_column);

    gtk_tree_view_set_model(GTK_TREE_VIEW(exclude_list),
                            GTK_TREE_MODEL(exclude_store));


    include_list  = lookup_widget(gnee_window, "include_list");
/*  include_store = gnee_settings->record_settings->events->exclude_store; */
    include_store = gtk_tree_store_new(1, G_TYPE_STRING);

    include_list_renderer = gtk_cell_renderer_text_new ();
    include_list_column   = gtk_tree_view_column_new_with_attributes
        ("Include when recording", 
         include_list_renderer, 
         "text", 
         0, 
         NULL);

    gtk_tree_view_append_column(GTK_TREE_VIEW(include_list), 
                                include_list_column);

    gtk_tree_view_set_model(GTK_TREE_VIEW(include_list),
                            GTK_TREE_MODEL(include_store));

    combo_list = g_list_append(combo_list, _("Events"));
    combo_list = g_list_append(combo_list, _("Replies"));
    combo_list = g_list_append(combo_list, _("Requests"));
    combo_list = g_list_append(combo_list, _("Errors"));

    event_group_combo = lookup_widget(gnee_window, "event_group_combo");
    gtk_combo_set_popdown_strings(GTK_COMBO(event_group_combo),
                                  combo_list);

    gtk_widget_show (gnee_window);


    
    xd = xnee_new_xnee_data();
    ext_xd = xd;

    /* Set the program name */
    xnee_set_program_name (xd, XNEE_GUI);

    /* Set the cli parameters */
    xnee_set_application_parameters (xd, argv);

    /* scan through the args .... */
    for (i=0;i<argc;i++)
      {
	if (strncmp(argv[i],version_string, strlen(version_string))==0)
	  {
	    fprintf (stderr, "%s %s, part of project ", 
		     GNEE,
		     VERSION,
		     PACKAGE);
	    xnee_version(xd);
	    exit(XNEE_OK);
	  }
      }

    /* Set the signal handler the libxnee's built in */ 
    (void) signal (SIGINT, signal_handler);

    /* Set the error handler the libxnee's built in */ 
    (void) XSetErrorHandler (handle_xerr);  
  
    /* set gnee default settings */
    gx_set_events_max (xd, 100);
    gnee_set_events_max (100);

    gx_set_data_max (xd, -1);
    gnee_set_data_max (-1);

    gx_set_time_max (xd, -1);
    gnee_set_time_max (-1);

    gnee_set_sync();
    
    gx_set_xosd_feedback();
    gnee_set_xosd_feedback();

    gx_set_max_threshold(xd, 20) ;
    gx_set_min_threshold(xd, -20) ;
    gx_set_tot_threshold(xd, 20) ;
    
    
    default_tmp_file=gx_get_default_filename();
    gnee_set_rec_file (default_tmp_file);
    gnee_set_rep_file (default_tmp_file);
    free (default_tmp_file);

    g_object_set_data(G_OBJECT(gnee_window), "xd", xd);

    gx_create_km_lists();
    
    gtk_main();

    return 0;
}
