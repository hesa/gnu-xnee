#include <gtk/gtk.h>


void
on_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_label4_focus                        (GtkWidget       *widget,
                                        GtkDirectionType  direction,
                                        gpointer         user_data);

void
on_label4_move_cursor                  (GtkLabel        *label,
                                        GtkMovementStep  step,
                                        gint             count,
                                        gboolean         extend_selection,
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
on_radiobutton2_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton3_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton4_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton2_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_checkbutton3_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_togglebutton1_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_togglebutton2_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_togglebutton3_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_spinbutton1_changed                 (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_combobox1_changed                   (GtkComboBox     *combobox,
                                        gpointer         user_data);

void
on_entry1_changed                      (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry1_editing_done                 (GtkCellEditable *celleditable,
                                        gpointer         user_data);

void
on_calendar1_prev_year                 (GtkCalendar     *calendar,
                                        gpointer         user_data);

void
on_calendar1_next_month                (GtkCalendar     *calendar,
                                        gpointer         user_data);

void
on_checkbutton1_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_button2_pressed                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_button2_released                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_calendar1_prev_year                 (GtkCalendar     *calendar,
                                        gpointer         user_data);

void
on_calendar1_prev_month                (GtkCalendar     *calendar,
                                        gpointer         user_data);

void
on_button3_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_calendar1_day_selected              (GtkCalendar     *calendar,
                                        gpointer         user_data);

void
on_button4_activate                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_button4_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_button5_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_button6_clicked                     (GtkButton       *button,
                                        gpointer         user_data);
