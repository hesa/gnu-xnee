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



void
on_spinbutton3_change_value            (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

void
on_wait_checkbox_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_wait_spinbutton_change_value        (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

void
on_wait_button_value_change            (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_wait_spinbutton_value_change        (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_verbose_logging_checkbox_toggled    (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_spinbutton5_change_value            (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

void
on_spinbutton4_change_value            (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

void
on_spinbutton6_change_value            (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

void
on_ok_button1_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_open_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_cancel_button1_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_sfile_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_save_button_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_about_ok_button_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_stop_m_combo_changed                (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_stop_k_combo_changed                (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_pause_m_combo_changed               (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_resume_m_combo_changed              (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_mark_m_combo_changed                (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_exec_m_combo_changed                (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_pause_k_combo_changed               (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_resume_k_combo_changed              (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_mark_k_combo_changed                (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_exec_k_combo_changed                (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_err_cont_button_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_err_quit_button_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_cancelbutton1_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_okbutton1_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_rep_menu_open_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_sfile_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_open_project_file2_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_set_session_file1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_checkbutton10_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_rec_file_sel_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_rep_disp_cb_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_rep_disp_text_changed               (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_button11_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
on_max_thresh_sb_value_changed         (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

void
on_min_thresh_sb_value_changed         (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

void
on_tot_thresh_sb_value_changed         (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

void
on_spinbutton11_change_value           (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

void
on_spinbutton12_change_value           (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

void
on_spinbutton13_change_value           (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

void
on_spinbutton7_change_value            (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);

void
on_speed_spin_change_value             (GtkSpinButton   *spinbutton,
                                        gpointer         user_data);


void
on_rec_disp_text_changed               (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_skip_sync_cb_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_force_rep_cb_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_okbutton2_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_speed_toggle_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton1_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radiobutton2_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_xnee_format_rb_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_human_format_rb_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_gnee_window_destroy                 (GtkObject       *object,
                                        gpointer         user_data);

void
on_stderr_fb_b_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_no_fb_b_toggled                     (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_xosb_fb_b_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_xosd_fb_b_toggled                   (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

gboolean
on_replay_settings_box_realize         (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

gboolean
on_replay_settings_box_visi            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_replay_settings_box_map             (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_record_settings_box_map             (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_button5_clicked                     (GtkButton       *button,
                                        gpointer         user_data);

void
on_about_ok_button_clicked             (GtkButton       *button,
                                        gpointer         user_data);
