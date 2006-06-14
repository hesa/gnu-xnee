#include <gtk/gtk.h>


void 
pnee_set_rec_file(char *filename);

void 
pnee_set_rep_file(char *filename);

void 
pnee_set_err_file(char *filename);

char *
pnee_get_default_filename();

char *
pnee_get_default_err_name();

void
on_setting_ok_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_verbose_check_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_display_check_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_delay_check_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_speed_check_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_feedback_button_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_faked_rec_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_faked_rep_clicked                   (GtkButton       *button,
                                        gpointer         user_data);


void
on_spinbutton2_change_value            (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

void
on_spinbutton3_value_changed           (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

void
on_spinbutton1_value_changed           (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

void
on_rec_file_button_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_rep_file_button_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_f_cancel_button_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_f_ok_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_f_cancel_button_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_f_ok_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_entry2_editing_done                 (GtkEditable     *editable,
                                        gpointer         user_data);
void
on_entry3_editing_done                 (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry4_changed                      (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_verbose_f_sel_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_use_display_ok_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_entry1_changed                      (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry5_changed                      (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_f_c_button_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_f_open_button_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_rec_chooser_but_file_activated      (GtkFileChooser  *filechooser,
                                        gpointer         user_data);

void
on_rep_choose_but_file_activated       (GtkFileChooser  *filechooser,
                                        gpointer         user_data);

void
on_rec_file_button_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_rep_choose_but_file_activated       (GtkFileChooser  *filechooser,
                                        gpointer         user_data);
