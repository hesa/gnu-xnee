/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "interface.h"
#include "support.h"
#include "recordables.h"

#include <libxnee/xnee.h>

int
main (int argc, char *argv[])
{
    GtkWidget          *gnee_window;

    GtkWidget          *exclude_list;
    GtkTreeStore       *exclude_store;
    GtkTreeIter         exclude_iterator; 
    GtkCellRenderer    *exclude_list_renderer;
    GtkTreeViewColumn  *exclude_list_column;

    GtkWidget          *include_list;
    GtkTreeStore       *include_store;
    GtkTreeIter         include_iterator; 
    GtkCellRenderer    *include_list_renderer;
    GtkTreeViewColumn  *include_list_column;

    GList              *combo_list = NULL;
    GtkWidget          *event_group_combo;

    xnee_data   *xd;

    //gnee_settings      *gnee_settings;

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

    //gnee_settings = gnee_settings_new();

    gnee_recordables_create(gnee_window);


    exclude_list  = lookup_widget(gnee_window, "exclude_list");
    //exclude_store = gnee_settings->record_settings->events->exclude_store;
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
    //include_store = gnee_settings->record_settings->events->exclude_store;
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
    xnee_init(xd);
    xnee_record_init(xd);
    
    g_object_set_data(G_OBJECT(gnee_window), "xd", xd);

    gtk_main();

    return 0;
}
