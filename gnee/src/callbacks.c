#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "recordables.h"
#include "gnee_xnee.h"

#include <stdio.h>
#include <libxnee/xnee.h>

int get_type(GtkWidget* combo_label)
{
    int   type;
    gchar *selection;

    selection = gtk_editable_get_chars
        (GTK_EDITABLE(combo_label), 0, -1);

    if (selection != 0)
    {
        if (strcmp(selection, "Events") == 0)
        {
            type = XNEE_DELIVERED_EVENT;
            g_print("type is XNEE_DELIVERED_EVENT\n");
        }
        else if (strcmp(selection, "Replies") == 0)
        {
            type = XNEE_REPLY;
            g_print("type is XNEE_REPLY\n");
        }
        else if (strcmp(selection, "Requests") == 0)
        {
            type = XNEE_REQUEST;
            g_print("type is XNEE_REQUEST\n");
        }
        else if (strcmp(selection, "Errors") == 0)
        {
            type = XNEE_ERROR;
            g_print("type is XNEE_ERROR\n");
        }
    }

    g_free(selection);

    return type;
}

void
on_record_include_button_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget          *source_list;
    GtkWidget          *destination_list;
    GtkWidget          *combo_label;
    xnee_data          *xd;

    source_list      = lookup_widget(GTK_WIDGET(user_data),
                                     "exclude_list");
    destination_list = lookup_widget(GTK_WIDGET(user_data),
                                     "include_list");
    combo_label      = lookup_widget(GTK_WIDGET(user_data),
                                     "combo_label1");
    xd = (xnee_data*) lookup_widget(GTK_WIDGET(user_data), "xd");

    gnee_recordables_exclude(GTK_TREE_VIEW(source_list), 
                             GTK_TREE_VIEW(destination_list),
                             xd,
                             get_type(combo_label));
}


void
on_record_exclude_button_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget          *source_list;
    GtkWidget          *destination_list;
    GtkWidget          *combo_label;
    xnee_data          *xd;

    source_list      = lookup_widget(GTK_WIDGET(user_data),
                                     "include_list");
    destination_list = lookup_widget(GTK_WIDGET(user_data),
                                     "exclude_list");
    combo_label      = lookup_widget(GTK_WIDGET(user_data),
                                     "combo_label1");

    xd = (xnee_data*) lookup_widget(GTK_WIDGET(user_data), "xd");

    gnee_recordables_include(GTK_TREE_VIEW(source_list), 
                             GTK_TREE_VIEW(destination_list),
                             xd,
                             get_type(combo_label));
}


void
on_exclude_list_row_activated          (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{
    GtkWidget          *source_list;
    GtkWidget          *destination_list;
    GtkWidget          *combo_label;
    xnee_data          *xd;

    source_list      = lookup_widget(GTK_WIDGET(user_data),
                                     "exclude_list");
    destination_list = lookup_widget(GTK_WIDGET(user_data),
                                     "include_list");
    combo_label      = lookup_widget(GTK_WIDGET(user_data),
                                     "combo_label1");

    xd = (xnee_data*) lookup_widget(GTK_WIDGET(user_data), "xd");

    gnee_recordables_include(GTK_TREE_VIEW(source_list), 
                             GTK_TREE_VIEW(destination_list),
                             xd,
                             get_type(combo_label));
}


void
on_include_list_row_activated          (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{
    GtkWidget          *source_list;
    GtkWidget          *destination_list;
    GtkWidget          *combo_label;
    xnee_data          *xd;

    source_list      = lookup_widget(GTK_WIDGET(user_data),
                                     "include_list");
    destination_list = lookup_widget(GTK_WIDGET(user_data),
                                     "exclude_list");
    combo_label      = lookup_widget(GTK_WIDGET(user_data),
                                     "combo_label1");

    xd = (xnee_data*) lookup_widget(GTK_WIDGET(user_data), "xd");

    gnee_recordables_exclude(GTK_TREE_VIEW(source_list), 
                             GTK_TREE_VIEW(destination_list),
                             xd,
                             get_type(combo_label));
}

void
on_combo_label1_changed                (GtkEditable     *editable,
                                        gpointer         user_data)
{
    GtkWidget          *exclude_list;
    GtkWidget          *include_list;

    GtkTreeStore       *exclude_store = 0;
    GtkTreeStore       *include_store = 0;

    gchar              *selection;
    GtkWidget          *combo_label;

    exclude_list = lookup_widget(GTK_WIDGET(user_data),
                                 "exclude_list");
    include_list = lookup_widget(GTK_WIDGET(user_data),
                                 "include_list");

    combo_label = lookup_widget(GTK_WIDGET(user_data),
                                "combo_label1");
    selection = gtk_editable_get_chars
        (GTK_EDITABLE(combo_label), 0, -1);

    if (selection != 0)
    {
        if (strcmp(selection, "Events") == 0)
        {
            exclude_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), "exclude_event_store"));
            include_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), "include_event_store"));
            
        }
        else if (strcmp(selection, "Replies") == 0)
        {
            exclude_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), "exclude_reply_store"));
            include_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), "include_reply_store"));
            
        }
        else if (strcmp(selection, "Requests") == 0)
        {
            exclude_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), "exclude_request_store"));
            include_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), "include_request_store"));
            
        }
        else if (strcmp(selection, "Errors") == 0)
        {
            exclude_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), "exclude_error_store"));
            include_store = GTK_TREE_STORE
                (lookup_widget(GTK_WIDGET(user_data), "include_error_store"));
            
        }
        
        if (exclude_store != NULL && include_store != NULL)
        {
            gtk_tree_view_set_model(GTK_TREE_VIEW(exclude_list),
                                    GTK_TREE_MODEL(exclude_store));
            gtk_tree_view_set_model(GTK_TREE_VIEW(include_list),
                                    GTK_TREE_MODEL(include_store));
        }
        
        g_free(selection);
    }
}

void
on_gnee_window_hide                    (GtkWidget       *widget,
                                        gpointer         user_data)
{

    int         ret;
    xnee_data   *xd;
    GtkWidget   *window;

    while (gtk_events_pending ())
        gtk_main_iteration ();

    ret    = -1;
    window = lookup_widget(user_data, "gnee_window");
    
    if (window != NULL)
    {
        xd = (xnee_data*) g_object_get_data(window, "xd");
        if (xd != NULL)
        {
            ret = gnee_xnee_start_recording(xd);
        }
    }

    gtk_widget_show_all(GTK_WIDGET(user_data));

/*     int          ret; */
/*     gboolean     valid; */


/*     GtkTreeStore *include_store; */
/*     GtkTreeIter   iterator; */

/*     GtkWidget* window = lookup_widget(user_data, "gnee_window"); */
/*     if (window != NULL) */
/*     { */
/*         gtk_widget_hide(window); */
/*         g_print("hiding gnee window\n"); */
/*     } */
/*     else */
/*     { */
/*         g_print("Unable to locate gnee_window\n"); */
/*     } */

/*     g_print("on_gnee_window_hide\n"); */
/*     while (gtk_events_pending ()) */
/*         gtk_main_iteration (); */

}


void
on_new1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_open1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_save1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_save_as1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_cut1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_copy1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_paste1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_delete1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_log_window_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{

}


void
on_record                              (GtkButton       *button,
                                        gpointer         user_data)
{
    on_gnee_window_hide(button, user_data);
    /*     GtkWidget* window = lookup_widget(user_data, "gnee_window"); */
    /*     if (window != NULL) */
    /*     { */
    /*         gtk_widget_hide(window); */
    /*         g_print("hiding gnee window\n"); */
    /*     } */
    /*     else */
    /*     { */
    /*         g_print("Unable to locate gnee_window\n"); */
    /*     } */
}


void
on_replay                              (GtkButton       *button,
                                        gpointer         user_data)
{

}

