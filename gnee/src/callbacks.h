#include <gtk/gtk.h>


void
on_record_include_button_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_record_exclude_button_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_exclude_list_row_activated          (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data);

void
on_include_list_row_activated          (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data);

void
on_combo_label1_changed                (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_gnee_window_hide                    (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_new1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_open1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_save1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_save_as1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_cut1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_copy1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_paste1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_delete1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_log_window_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_record                              (GtkButton       *button,
                                        gpointer         user_data);

void
on_replay                              (GtkButton       *button,
                                        gpointer         user_data);

