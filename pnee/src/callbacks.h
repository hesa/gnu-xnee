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


#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>
#include <pnee_impl.h>


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
on_rec_file_button_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_rep_choose_but_file_activated       (GtkFileChooser  *filechooser,
                                        gpointer         user_data);

void
pnee_rep_pressed(void);

void 
pnee_rec_pressed(void);

void
pnee_stop_pressed(void);

void
pnee_set_interval (GtkToggleButton *togglebutton,
		   gpointer         user_data);

void
on_log_file_button_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_error_ok_clicked                    (GtkButton       *button,
                                        gpointer         user_data);

void
pnee_set_events_to_record (GtkToggleButton *togglebutton,
			   gpointer         user_data);

#endif /* CALLBACKS_H */

