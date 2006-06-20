/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

GtkWidget*
create_pnee_pref (void)
{
  GtkWidget *pnee_pref;
  GtkWidget *main_notebook;
  GtkWidget *vbox1;
  GtkWidget *general_frame;
  GtkWidget *alignment5;
  GtkWidget *table1;
  GtkWidget *verbose_check;
  GtkWidget *hbox9;
  GtkWidget *delay_check;
  GtkObject *spinbutton2_adj;
  GtkWidget *spinbutton2;
  GtkWidget *general_label;
  GtkWidget *record_frame;
  GtkWidget *alignment4;
  GtkWidget *vbox4;
  GtkWidget *hbox4;
  GtkWidget *rec_file_label;
  GtkWidget *entry3;
  GtkWidget *rec_file_button;
  GtkWidget *hbox11;
  GtkWidget *hbox10;
  GtkWidget *nr_events_label;
  GtkObject *spinbutton3_adj;
  GtkWidget *spinbutton3;
  GtkWidget *hbox11b;
  GtkWidget *grab_label;
  GtkWidget *entry5;
  GtkWidget *record_label;
  GtkWidget *replay_frame;
  GtkWidget *alignment2;
  GtkWidget *vbox3;
  GtkWidget *hbox3;
  GtkWidget *repl_file_label;
  GtkWidget *rep_choose_but;
  GtkWidget *hbox5;
  GtkWidget *speed_check;
  GtkObject *spinbutton1_adj;
  GtkWidget *spinbutton1;
  GtkWidget *replay_label;
  GtkWidget *setting_ok;
  GtkWidget *settings_label;
  GtkWidget *vbox5;
  GtkWidget *faked_rec;
  GtkWidget *faked_rep;
  GtkWidget *about_label;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();

  pnee_pref = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (pnee_pref), _("Pnee - Preferences"));

  main_notebook = gtk_notebook_new ();
  gtk_widget_show (main_notebook);
  gtk_container_add (GTK_CONTAINER (pnee_pref), main_notebook);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (main_notebook), vbox1);

  general_frame = gtk_frame_new (NULL);
  gtk_widget_show (general_frame);
  gtk_box_pack_start (GTK_BOX (vbox1), general_frame, TRUE, TRUE, 0);
  gtk_frame_set_shadow_type (GTK_FRAME (general_frame), GTK_SHADOW_NONE);

  alignment5 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment5);
  gtk_container_add (GTK_CONTAINER (general_frame), alignment5);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment5), 0, 0, 12, 0);

  table1 = gtk_table_new (2, 3, FALSE);
  gtk_widget_show (table1);
  gtk_container_add (GTK_CONTAINER (alignment5), table1);

  verbose_check = gtk_check_button_new_with_mnemonic (_("verbose logging"));
  gtk_widget_show (verbose_check);
  gtk_table_attach (GTK_TABLE (table1), verbose_check, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  hbox9 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox9);
  gtk_table_attach (GTK_TABLE (table1), hbox9, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);

  delay_check = gtk_check_button_new_with_mnemonic (_("Delay"));
  gtk_widget_show (delay_check);
  gtk_box_pack_start (GTK_BOX (hbox9), delay_check, FALSE, FALSE, 0);

  spinbutton2_adj = gtk_adjustment_new (3, 0, 10000, 1, 10, 10);
  spinbutton2 = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton2_adj), 1, 0);
  gtk_widget_show (spinbutton2);
  gtk_box_pack_start (GTK_BOX (hbox9), spinbutton2, TRUE, TRUE, 0);
  gtk_tooltips_set_tip (tooltips, spinbutton2, _("Delay start of record or replay. (seconds)"), NULL);

  general_label = gtk_label_new (_("<b>General</b>"));
  gtk_widget_show (general_label);
  gtk_frame_set_label_widget (GTK_FRAME (general_frame), general_label);
  gtk_label_set_use_markup (GTK_LABEL (general_label), TRUE);

  record_frame = gtk_frame_new (NULL);
  gtk_widget_show (record_frame);
  gtk_box_pack_start (GTK_BOX (vbox1), record_frame, TRUE, TRUE, 0);
  gtk_frame_set_shadow_type (GTK_FRAME (record_frame), GTK_SHADOW_NONE);

  alignment4 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment4);
  gtk_container_add (GTK_CONTAINER (record_frame), alignment4);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment4), 0, 0, 12, 0);

  vbox4 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox4);
  gtk_container_add (GTK_CONTAINER (alignment4), vbox4);

  hbox4 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox4);
  gtk_box_pack_start (GTK_BOX (vbox4), hbox4, TRUE, TRUE, 0);

  rec_file_label = gtk_label_new (_("Record to file"));
  gtk_widget_show (rec_file_label);
  gtk_box_pack_start (GTK_BOX (hbox4), rec_file_label, FALSE, FALSE, 0);

  entry3 = gtk_entry_new ();
  gtk_widget_show (entry3);
  gtk_box_pack_start (GTK_BOX (hbox4), entry3, TRUE, TRUE, 0);

  rec_file_button = gtk_button_new_with_mnemonic (_("..."));
  gtk_widget_show (rec_file_button);
  gtk_box_pack_start (GTK_BOX (hbox4), rec_file_button, FALSE, FALSE, 0);

  hbox11 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox11);
  gtk_box_pack_start (GTK_BOX (vbox4), hbox11, TRUE, TRUE, 0);

  hbox10 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox10);
  gtk_box_pack_start (GTK_BOX (hbox11), hbox10, TRUE, TRUE, 0);

  nr_events_label = gtk_label_new (_("Events to record"));
  gtk_widget_show (nr_events_label);
  gtk_box_pack_start (GTK_BOX (hbox10), nr_events_label, FALSE, FALSE, 0);

  spinbutton3_adj = gtk_adjustment_new (200, -1, 10000, 1, 10, 10);
  spinbutton3 = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton3_adj), 1, 0);
  gtk_widget_show (spinbutton3);
  gtk_box_pack_start (GTK_BOX (hbox10), spinbutton3, TRUE, TRUE, 0);
  gtk_tooltips_set_tip (tooltips, spinbutton3, _("Number of events to record (-1 for ever)."), NULL);

  hbox11b = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox11b);
  gtk_box_pack_start (GTK_BOX (vbox4), hbox11b, TRUE, TRUE, 0);

  grab_label = gtk_label_new (_("Stop key"));
  gtk_widget_show (grab_label);
  gtk_box_pack_start (GTK_BOX (hbox11b), grab_label, FALSE, FALSE, 0);

  entry5 = gtk_entry_new ();
  gtk_widget_show (entry5);
  gtk_box_pack_start (GTK_BOX (hbox11b), entry5, TRUE, TRUE, 0);
  gtk_widget_set_size_request (entry5, 4, -1);
  gtk_tooltips_set_tip (tooltips, entry5, _("Grab a key to stop recording"), NULL);
  gtk_entry_set_max_length (GTK_ENTRY (entry5), 5);
  gtk_entry_set_text (GTK_ENTRY (entry5), _("F1"));

  record_label = gtk_label_new (_("<b>Record</b>"));
  gtk_widget_show (record_label);
  gtk_frame_set_label_widget (GTK_FRAME (record_frame), record_label);
  gtk_label_set_use_markup (GTK_LABEL (record_label), TRUE);

  replay_frame = gtk_frame_new (NULL);
  gtk_widget_show (replay_frame);
  gtk_box_pack_start (GTK_BOX (vbox1), replay_frame, TRUE, TRUE, 0);
  gtk_frame_set_shadow_type (GTK_FRAME (replay_frame), GTK_SHADOW_NONE);

  alignment2 = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (alignment2);
  gtk_container_add (GTK_CONTAINER (replay_frame), alignment2);
  gtk_alignment_set_padding (GTK_ALIGNMENT (alignment2), 0, 0, 12, 0);

  vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox3);
  gtk_container_add (GTK_CONTAINER (alignment2), vbox3);

  hbox3 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox3);
  gtk_box_pack_start (GTK_BOX (vbox3), hbox3, TRUE, TRUE, 0);

  repl_file_label = gtk_label_new (_("Replay from file"));
  gtk_widget_show (repl_file_label);
  gtk_box_pack_start (GTK_BOX (hbox3), repl_file_label, FALSE, FALSE, 0);

  rep_choose_but = gtk_file_chooser_button_new (_("Select A File"), GTK_FILE_CHOOSER_ACTION_OPEN);
  gtk_widget_show (rep_choose_but);
  gtk_box_pack_start (GTK_BOX (hbox3), rep_choose_but, TRUE, TRUE, 0);
  g_object_set (rep_choose_but,
                "local-only", FALSE,
                NULL);

  hbox5 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox5);
  gtk_box_pack_start (GTK_BOX (vbox3), hbox5, TRUE, TRUE, 0);

  speed_check = gtk_check_button_new_with_mnemonic (_("Replay speed %"));
  gtk_widget_show (speed_check);
  gtk_box_pack_start (GTK_BOX (hbox5), speed_check, FALSE, FALSE, 0);

  spinbutton1_adj = gtk_adjustment_new (100, 1, 10000, 1, 10, 10);
  spinbutton1 = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton1_adj), 1, 0);
  gtk_widget_show (spinbutton1);
  gtk_box_pack_start (GTK_BOX (hbox5), spinbutton1, TRUE, TRUE, 0);
  gtk_tooltips_set_tip (tooltips, spinbutton1, _("Set a different speed"), NULL);

  replay_label = gtk_label_new (_("<b>Replay</b>"));
  gtk_widget_show (replay_label);
  gtk_frame_set_label_widget (GTK_FRAME (replay_frame), replay_label);
  gtk_label_set_use_markup (GTK_LABEL (replay_label), TRUE);

  setting_ok = gtk_button_new_with_mnemonic (_("OK"));
  gtk_widget_show (setting_ok);
  gtk_box_pack_start (GTK_BOX (vbox1), setting_ok, FALSE, FALSE, 0);

  settings_label = gtk_label_new (_("Settings"));
  gtk_widget_show (settings_label);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (main_notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (main_notebook), 0), settings_label);

  vbox5 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox5);
  gtk_container_add (GTK_CONTAINER (main_notebook), vbox5);

  faked_rec = gtk_button_new_with_mnemonic (_("Record"));
  gtk_widget_show (faked_rec);
  gtk_box_pack_start (GTK_BOX (vbox5), faked_rec, FALSE, FALSE, 0);

  faked_rep = gtk_button_new_with_mnemonic (_("Replay"));
  gtk_widget_show (faked_rep);
  gtk_box_pack_start (GTK_BOX (vbox5), faked_rep, FALSE, FALSE, 0);

  about_label = gtk_label_new (_("About"));
  gtk_widget_show (about_label);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (main_notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (main_notebook), 1), about_label);

  g_signal_connect_swapped ((gpointer) verbose_check, "toggled",
                            G_CALLBACK (on_verbose_check_toggled),
                            GTK_OBJECT (verbose_check));
  g_signal_connect_swapped ((gpointer) delay_check, "toggled",
                            G_CALLBACK (on_delay_check_toggled),
                            GTK_OBJECT (delay_check));
  g_signal_connect_swapped ((gpointer) spinbutton2, "value_changed",
                            G_CALLBACK (on_spinbutton2_change_value),
                            GTK_OBJECT (spinbutton2));
  g_signal_connect_swapped ((gpointer) rec_file_button, "clicked",
                            G_CALLBACK (on_rec_file_button_clicked),
                            GTK_OBJECT (rec_file_button));
  g_signal_connect_swapped ((gpointer) spinbutton3, "value_changed",
                            G_CALLBACK (on_spinbutton3_value_changed),
                            GTK_OBJECT (spinbutton3));
  g_signal_connect_swapped ((gpointer) entry5, "changed",
                            G_CALLBACK (on_entry5_changed),
                            GTK_OBJECT (entry5));
  g_signal_connect_swapped ((gpointer) rep_choose_but, "file_activated",
                            G_CALLBACK (on_rep_choose_but_file_activated),
                            GTK_OBJECT (rep_choose_but));
  g_signal_connect_swapped ((gpointer) rep_choose_but, "selection_changed",
                            G_CALLBACK (on_rep_choose_but_file_activated),
                            GTK_OBJECT (rep_choose_but));
  g_signal_connect_swapped ((gpointer) speed_check, "toggled",
                            G_CALLBACK (on_speed_check_toggled),
                            GTK_OBJECT (speed_check));
  g_signal_connect_swapped ((gpointer) spinbutton1, "value_changed",
                            G_CALLBACK (on_spinbutton1_value_changed),
                            GTK_OBJECT (spinbutton1));
  g_signal_connect_swapped ((gpointer) setting_ok, "clicked",
                            G_CALLBACK (on_setting_ok_clicked),
                            GTK_OBJECT (pnee_pref));
  g_signal_connect_swapped ((gpointer) faked_rec, "clicked",
                            G_CALLBACK (on_faked_rec_clicked),
                            GTK_OBJECT (faked_rec));
  g_signal_connect_swapped ((gpointer) faked_rep, "clicked",
                            G_CALLBACK (on_faked_rep_clicked),
                            GTK_OBJECT (faked_rep));

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (pnee_pref, pnee_pref, "pnee_pref");
  GLADE_HOOKUP_OBJECT (pnee_pref, main_notebook, "main_notebook");
  GLADE_HOOKUP_OBJECT (pnee_pref, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (pnee_pref, general_frame, "general_frame");
  GLADE_HOOKUP_OBJECT (pnee_pref, alignment5, "alignment5");
  GLADE_HOOKUP_OBJECT (pnee_pref, table1, "table1");
  GLADE_HOOKUP_OBJECT (pnee_pref, verbose_check, "verbose_check");
  GLADE_HOOKUP_OBJECT (pnee_pref, hbox9, "hbox9");
  GLADE_HOOKUP_OBJECT (pnee_pref, delay_check, "delay_check");
  GLADE_HOOKUP_OBJECT (pnee_pref, spinbutton2, "spinbutton2");
  GLADE_HOOKUP_OBJECT (pnee_pref, general_label, "general_label");
  GLADE_HOOKUP_OBJECT (pnee_pref, record_frame, "record_frame");
  GLADE_HOOKUP_OBJECT (pnee_pref, alignment4, "alignment4");
  GLADE_HOOKUP_OBJECT (pnee_pref, vbox4, "vbox4");
  GLADE_HOOKUP_OBJECT (pnee_pref, hbox4, "hbox4");
  GLADE_HOOKUP_OBJECT (pnee_pref, rec_file_label, "rec_file_label");
  GLADE_HOOKUP_OBJECT (pnee_pref, entry3, "entry3");
  GLADE_HOOKUP_OBJECT (pnee_pref, rec_file_button, "rec_file_button");
  GLADE_HOOKUP_OBJECT (pnee_pref, hbox11, "hbox11");
  GLADE_HOOKUP_OBJECT (pnee_pref, hbox10, "hbox10");
  GLADE_HOOKUP_OBJECT (pnee_pref, nr_events_label, "nr_events_label");
  GLADE_HOOKUP_OBJECT (pnee_pref, spinbutton3, "spinbutton3");
  GLADE_HOOKUP_OBJECT (pnee_pref, hbox11b, "hbox11b");
  GLADE_HOOKUP_OBJECT (pnee_pref, grab_label, "grab_label");
  GLADE_HOOKUP_OBJECT (pnee_pref, entry5, "entry5");
  GLADE_HOOKUP_OBJECT (pnee_pref, record_label, "record_label");
  GLADE_HOOKUP_OBJECT (pnee_pref, replay_frame, "replay_frame");
  GLADE_HOOKUP_OBJECT (pnee_pref, alignment2, "alignment2");
  GLADE_HOOKUP_OBJECT (pnee_pref, vbox3, "vbox3");
  GLADE_HOOKUP_OBJECT (pnee_pref, hbox3, "hbox3");
  GLADE_HOOKUP_OBJECT (pnee_pref, repl_file_label, "repl_file_label");
  GLADE_HOOKUP_OBJECT (pnee_pref, rep_choose_but, "rep_choose_but");
  GLADE_HOOKUP_OBJECT (pnee_pref, hbox5, "hbox5");
  GLADE_HOOKUP_OBJECT (pnee_pref, speed_check, "speed_check");
  GLADE_HOOKUP_OBJECT (pnee_pref, spinbutton1, "spinbutton1");
  GLADE_HOOKUP_OBJECT (pnee_pref, replay_label, "replay_label");
  GLADE_HOOKUP_OBJECT (pnee_pref, setting_ok, "setting_ok");
  GLADE_HOOKUP_OBJECT (pnee_pref, settings_label, "settings_label");
  GLADE_HOOKUP_OBJECT (pnee_pref, vbox5, "vbox5");
  GLADE_HOOKUP_OBJECT (pnee_pref, faked_rec, "faked_rec");
  GLADE_HOOKUP_OBJECT (pnee_pref, faked_rep, "faked_rep");
  GLADE_HOOKUP_OBJECT (pnee_pref, about_label, "about_label");
  GLADE_HOOKUP_OBJECT_NO_REF (pnee_pref, tooltips, "tooltips");

  return pnee_pref;
}

GtkWidget*
create_filechooserdialog1 (void)
{
  GtkWidget *filechooserdialog1;
  GtkWidget *dialog_vbox1;
  GtkWidget *dialog_action_area1;
  GtkWidget *f_c_button;
  GtkWidget *f_open_button;

  filechooserdialog1 = gtk_file_chooser_dialog_new ("", NULL, GTK_FILE_CHOOSER_ACTION_SAVE, NULL);
  g_object_set (filechooserdialog1,
                "local-only", FALSE,
                "show-hidden", TRUE,
                NULL);
  gtk_window_set_type_hint (GTK_WINDOW (filechooserdialog1), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox1 = GTK_DIALOG (filechooserdialog1)->vbox;
  gtk_widget_show (dialog_vbox1);

  dialog_action_area1 = GTK_DIALOG (filechooserdialog1)->action_area;
  gtk_widget_show (dialog_action_area1);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);

  f_c_button = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (f_c_button);
  gtk_dialog_add_action_widget (GTK_DIALOG (filechooserdialog1), f_c_button, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (f_c_button, GTK_CAN_DEFAULT);

  f_open_button = gtk_button_new_from_stock ("gtk-open");
  gtk_widget_show (f_open_button);
  gtk_dialog_add_action_widget (GTK_DIALOG (filechooserdialog1), f_open_button, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (f_open_button, GTK_CAN_DEFAULT);

  g_signal_connect_swapped ((gpointer) f_c_button, "clicked",
                            G_CALLBACK (on_f_c_button_clicked),
                            GTK_OBJECT (f_c_button));
  g_signal_connect_swapped ((gpointer) f_open_button, "clicked",
                            G_CALLBACK (on_f_open_button_clicked),
                            GTK_OBJECT (f_open_button));

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (filechooserdialog1, filechooserdialog1, "filechooserdialog1");
  GLADE_HOOKUP_OBJECT_NO_REF (filechooserdialog1, dialog_vbox1, "dialog_vbox1");
  GLADE_HOOKUP_OBJECT_NO_REF (filechooserdialog1, dialog_action_area1, "dialog_action_area1");
  GLADE_HOOKUP_OBJECT (filechooserdialog1, f_c_button, "f_c_button");
  GLADE_HOOKUP_OBJECT (filechooserdialog1, f_open_button, "f_open_button");

  gtk_widget_grab_default (f_open_button);
  return filechooserdialog1;
}

GtkWidget*
create_pnee_about (void)
{
  GtkWidget *pnee_about;
  const gchar *authors[] = {
    "Henrik Sandklef <hesa@gnu.org>",
    NULL
  };
  const gchar *documenters[] = {
    "None, ",
    "please contact hesa@gnu.org if you want to contribute",
    NULL
  };
  const gchar *artists[] = {
    "None, ",
    "please contact hesa@gnu.org if you want to contribute",
    NULL
  };
  /* TRANSLATORS: Replace this string with your names, one name per line. */
  gchar *translators = "None, \nplease contact hesa@gnu.org if you want to contribute";

  pnee_about = gtk_about_dialog_new ();
  gtk_about_dialog_set_version (GTK_ABOUT_DIALOG (pnee_about), VERSION);
  gtk_about_dialog_set_name (GTK_ABOUT_DIALOG (pnee_about), _("Pnee"));
  gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG (pnee_about), _("\302\251 2005, 2006 Henrik Sandklef <hesa@gnu.org>"));
  gtk_about_dialog_set_comments (GTK_ABOUT_DIALOG (pnee_about), _("Pnee is a GNU Xnee Applet  "));
  gtk_about_dialog_set_license (GTK_ABOUT_DIALOG (pnee_about), _("\t\t    GNU GENERAL PUBLIC LICENSE\n\t\t       Version 2, June 1991\n\n Copyright (C) 1989, 1991 Free Software Foundation, Inc.\n 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA\n Everyone is permitted to copy and distribute verbatim copies\n of this license document, but changing it is not allowed.\n\n\t\t\t    Preamble\n\n  The licenses for most software are designed to take away your\nfreedom to share and change it.  By contrast, the GNU General Public\nLicense is intended to guarantee your freedom to share and change free\nsoftware--to make sure the software is free for all its users.  This\nGeneral Public License applies to most of the Free Software\nFoundation's software and to any other program whose authors commit to\nusing it.  (Some other Free Software Foundation software is covered by\nthe GNU Lesser General Public License instead.)  You can apply it to\nyour programs, too.\n\n  When we speak of free software, we are referring to freedom, not\nprice.  Our General Public Licenses are designed to make sure that you\nhave the freedom to distribute copies of free software (and charge for\nthis service if you wish), that you receive source code or can get it\nif you want it, that you can change the software or use pieces of it\nin new free programs; and that you know you can do these things.\n\n  To protect your rights, we need to make restrictions that forbid\nanyone to deny you these rights or to ask you to surrender the rights.\nThese restrictions translate to certain responsibilities for you if you\ndistribute copies of the software, or if you modify it.\n\n  For example, if you distribute copies of such a program, whether\ngratis or for a fee, you must give the recipients all the rights that\nyou have.  You must make sure that they, too, receive or can get the\nsource code.  And you must show them these terms so they know their\nrights.\n\n  We protect your rights with two steps: (1) copyright the software, and\n(2) offer you this license which gives you legal permission to copy,\ndistribute and/or modify the software.\n\n  Also, for each author's protection and ours, we want to make certain\nthat everyone understands that there is no warranty for this free\nsoftware.  If the software is modified by someone else and passed on, we\nwant its recipients to know that what they have is not the original, so\nthat any problems introduced by others will not reflect on the original\nauthors' reputations.\n\n  Finally, any free program is threatened constantly by software\npatents.  We wish to avoid the danger that redistributors of a free\nprogram will individually obtain patent licenses, in effect making the\nprogram proprietary.  To prevent this, we have made it clear that any\npatent must be licensed for everyone's free use or not licensed at all.\n\n  The precise terms and conditions for copying, distribution and\nmodification follow.\n\n\t\t    GNU GENERAL PUBLIC LICENSE\n   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION\n\n  0. This License applies to any program or other work which contains\na notice placed by the copyright holder saying it may be distributed\nunder the terms of this General Public License.  The \"Program\", below,\nrefers to any such program or work, and a \"work based on the Program\"\nmeans either the Program or any derivative work under copyright law:\nthat is to say, a work containing the Program or a portion of it,\neither verbatim or with modifications and/or translated into another\nlanguage.  (Hereinafter, translation is included without limitation in\nthe term \"modification\".)  Each licensee is addressed as \"you\".\n\nActivities other than copying, distribution and modification are not\ncovered by this License; they are outside its scope.  The act of\nrunning the Program is not restricted, and the output from the Program\nis covered only if its contents constitute a work based on the\nProgram (independent of having been made by running the Program).\nWhether that is true depends on what the Program does.\n\n  1. You may copy and distribute verbatim copies of the Program's\nsource code as you receive it, in any medium, provided that you\nconspicuously and appropriately publish on each copy an appropriate\ncopyright notice and disclaimer of warranty; keep intact all the\nnotices that refer to this License and to the absence of any warranty;\nand give any other recipients of the Program a copy of this License\nalong with the Program.\n\nYou may charge a fee for the physical act of transferring a copy, and\nyou may at your option offer warranty protection in exchange for a fee.\n\n  2. You may modify your copy or copies of the Program or any portion\nof it, thus forming a work based on the Program, and copy and\ndistribute such modifications or work under the terms of Section 1\nabove, provided that you also meet all of these conditions:\n\n    a) You must cause the modified files to carry prominent notices\n    stating that you changed the files and the date of any change.\n\n    b) You must cause any work that you distribute or publish, that in\n    whole or in part contains or is derived from the Program or any\n    part thereof, to be licensed as a whole at no charge to all third\n    parties under the terms of this License.\n\n    c) If the modified program normally reads commands interactively\n    when run, you must cause it, when started running for such\n    interactive use in the most ordinary way, to print or display an\n    announcement including an appropriate copyright notice and a\n    notice that there is no warranty (or else, saying that you provide\n    a warranty) and that users may redistribute the program under\n    these conditions, and telling the user how to view a copy of this\n    License.  (Exception: if the Program itself is interactive but\n    does not normally print such an announcement, your work based on\n    the Program is not required to print an announcement.)\n\nThese requirements apply to the modified work as a whole.  If\nidentifiable sections of that work are not derived from the Program,\nand can be reasonably considered independent and separate works in\nthemselves, then this License, and its terms, do not apply to those\nsections when you distribute them as separate works.  But when you\ndistribute the same sections as part of a whole which is a work based\non the Program, the distribution of the whole must be on the terms of\nthis License, whose permissions for other licensees extend to the\nentire whole, and thus to each and every part regardless of who wrote it.\n\nThus, it is not the intent of this section to claim rights or contest\nyour rights to work written entirely by you; rather, the intent is to\nexercise the right to control the distribution of derivative or\ncollective works based on the Program.\n\nIn addition, mere aggregation of another work not based on the Program\nwith the Program (or with a work based on the Program) on a volume of\na storage or distribution medium does not bring the other work under\nthe scope of this License.\n\n  3. You may copy and distribute the Program (or a work based on it,\nunder Section 2) in object code or executable form under the terms of\nSections 1 and 2 above provided that you also do one of the following:\n\n    a) Accompany it with the complete corresponding machine-readable\n    source code, which must be distributed under the terms of Sections\n    1 and 2 above on a medium customarily used for software interchange; or,\n\n    b) Accompany it with a written offer, valid for at least three\n    years, to give any third party, for a charge no more than your\n    cost of physically performing source distribution, a complete\n    machine-readable copy of the corresponding source code, to be\n    distributed under the terms of Sections 1 and 2 above on a medium\n    customarily used for software interchange; or,\n\n    c) Accompany it with the information you received as to the offer\n    to distribute corresponding source code.  (This alternative is\n    allowed only for noncommercial distribution and only if you\n    received the program in object code or executable form with such\n    an offer, in accord with Subsection b above.)\n\nThe source code for a work means the preferred form of the work for\nmaking modifications to it.  For an executable work, complete source\ncode means all the source code for all modules it contains, plus any\nassociated interface definition files, plus the scripts used to\ncontrol compilation and installation of the executable.  However, as a\nspecial exception, the source code distributed need not include\nanything that is normally distributed (in either source or binary\nform) with the major components (compiler, kernel, and so on) of the\noperating system on which the executable runs, unless that component\nitself accompanies the executable.\n\nIf distribution of executable or object code is made by offering\naccess to copy from a designated place, then offering equivalent\naccess to copy the source code from the same place counts as\ndistribution of the source code, even though third parties are not\ncompelled to copy the source along with the object code.\n\n  4. You may not copy, modify, sublicense, or distribute the Program\nexcept as expressly provided under this License.  Any attempt\notherwise to copy, modify, sublicense or distribute the Program is\nvoid, and will automatically terminate your rights under this License.\nHowever, parties who have received copies, or rights, from you under\nthis License will not have their licenses terminated so long as such\nparties remain in full compliance.\n\n  5. You are not required to accept this License, since you have not\nsigned it.  However, nothing else grants you permission to modify or\ndistribute the Program or its derivative works.  These actions are\nprohibited by law if you do not accept this License.  Therefore, by\nmodifying or distributing the Program (or any work based on the\nProgram), you indicate your acceptance of this License to do so, and\nall its terms and conditions for copying, distributing or modifying\nthe Program or works based on it.\n\n  6. Each time you redistribute the Program (or any work based on the\nProgram), the recipient automatically receives a license from the\noriginal licensor to copy, distribute or modify the Program subject to\nthese terms and conditions.  You may not impose any further\nrestrictions on the recipients' exercise of the rights granted herein.\nYou are not responsible for enforcing compliance by third parties to\nthis License.\n\n  7. If, as a consequence of a court judgment or allegation of patent\ninfringement or for any other reason (not limited to patent issues),\nconditions are imposed on you (whether by court order, agreement or\notherwise) that contradict the conditions of this License, they do not\nexcuse you from the conditions of this License.  If you cannot\ndistribute so as to satisfy simultaneously your obligations under this\nLicense and any other pertinent obligations, then as a consequence you\nmay not distribute the Program at all.  For example, if a patent\nlicense would not permit royalty-free redistribution of the Program by\nall those who receive copies directly or indirectly through you, then\nthe only way you could satisfy both it and this License would be to\nrefrain entirely from distribution of the Program.\n\nIf any portion of this section is held invalid or unenforceable under\nany particular circumstance, the balance of the section is intended to\napply and the section as a whole is intended to apply in other\ncircumstances.\n\nIt is not the purpose of this section to induce you to infringe any\npatents or other property right claims or to contest validity of any\nsuch claims; this section has the sole purpose of protecting the\nintegrity of the free software distribution system, which is\nimplemented by public license practices.  Many people have made\ngenerous contributions to the wide range of software distributed\nthrough that system in reliance on consistent application of that\nsystem; it is up to the author/donor to decide if he or she is willing\nto distribute software through any other system and a licensee cannot\nimpose that choice.\n\nThis section is intended to make thoroughly clear what is believed to\nbe a consequence of the rest of this License.\n\n  8. If the distribution and/or use of the Program is restricted in\ncertain countries either by patents or by copyrighted interfaces, the\noriginal copyright holder who places the Program under this License\nmay add an explicit geographical distribution limitation excluding\nthose countries, so that distribution is permitted only in or among\ncountries not thus excluded.  In such case, this License incorporates\nthe limitation as if written in the body of this License.\n\n  9. The Free Software Foundation may publish revised and/or new versions\nof the General Public License from time to time.  Such new versions will\nbe similar in spirit to the present version, but may differ in detail to\naddress new problems or concerns.\n\nEach version is given a distinguishing version number.  If the Program\nspecifies a version number of this License which applies to it and \"any\nlater version\", you have the option of following the terms and conditions\neither of that version or of any later version published by the Free\nSoftware Foundation.  If the Program does not specify a version number of\nthis License, you may choose any version ever published by the Free Software\nFoundation.\n\n  10. If you wish to incorporate parts of the Program into other free\nprograms whose distribution conditions are different, write to the author\nto ask for permission.  For software which is copyrighted by the Free\nSoftware Foundation, write to the Free Software Foundation; we sometimes\nmake exceptions for this.  Our decision will be guided by the two goals\nof preserving the free status of all derivatives of our free software and\nof promoting the sharing and reuse of software generally.\n\n\t\t\t    NO WARRANTY\n\n  11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY\nFOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN\nOTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES\nPROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED\nOR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF\nMERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS\nTO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE\nPROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,\nREPAIR OR CORRECTION.\n\n  12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING\nWILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR\nREDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES,\nINCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING\nOUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED\nTO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY\nYOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER\nPROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE\nPOSSIBILITY OF SUCH DAMAGES.\n\n\t\t     END OF TERMS AND CONDITIONS\n\n\t    How to Apply These Terms to Your New Programs\n\n  If you develop a new program, and you want it to be of the greatest\npossible use to the public, the best way to achieve this is to make it\nfree software which everyone can redistribute and change under these terms.\n\n  To do so, attach the following notices to the program.  It is safest\nto attach them to the start of each source file to most effectively\nconvey the exclusion of warranty; and each file should have at least\nthe \"copyright\" line and a pointer to where the full notice is found.\n\n    <one line to give the program's name and a brief idea of what it does.>\n    Copyright (C) <year>  <name of author>\n\n    This program is free software; you can redistribute it and/or modify\n    it under the terms of the GNU General Public License as published by\n    the Free Software Foundation; either version 2 of the License, or\n    (at your option) any later version.\n\n    This program is distributed in the hope that it will be useful,\n    but WITHOUT ANY WARRANTY; without even the implied warranty of\n    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n    GNU General Public License for more details.\n\n    You should have received a copy of the GNU General Public License\n    along with this program; if not, write to the Free Software\n    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA\n\n\nAlso add information on how to contact you by electronic and paper mail.\n\nIf the program is interactive, make it output a short notice like this\nwhen it starts in an interactive mode:\n\n    Gnomovision version 69, Copyright (C) year name of author\n    Gnomovision comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n    This is free software, and you are welcome to redistribute it\n    under certain conditions; type `show c' for details.\n\nThe hypothetical commands `show w' and `show c' should show the appropriate\nparts of the General Public License.  Of course, the commands you use may\nbe called something other than `show w' and `show c'; they could even be\nmouse-clicks or menu items--whatever suits your program.\n\nYou should also get your employer (if you work as a programmer) or your\nschool, if any, to sign a \"copyright disclaimer\" for the program, if\nnecessary.  Here is a sample; alter the names:\n\n  Yoyodyne, Inc., hereby disclaims all copyright interest in the program\n  `Gnomovision' (which makes passes at compilers) written by James Hacker.\n\n  <signature of Ty Coon>, 1 April 1989\n  Ty Coon, President of Vice\n\nThis General Public License does not permit incorporating your program into\nproprietary programs.  If your program is a subroutine library, you may\nconsider it more useful to permit linking proprietary applications with the\nlibrary.  If this is what you want to do, use the GNU Lesser General\nPublic License instead of this License.\n"));
  gtk_about_dialog_set_website (GTK_ABOUT_DIALOG (pnee_about), "http://www.gnu.org/software/xnee");
  gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG (pnee_about), authors);
  gtk_about_dialog_set_documenters (GTK_ABOUT_DIALOG (pnee_about), documenters);
  gtk_about_dialog_set_artists (GTK_ABOUT_DIALOG (pnee_about), artists);
  gtk_about_dialog_set_translator_credits (GTK_ABOUT_DIALOG (pnee_about), translators);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (pnee_about, pnee_about, "pnee_about");

  return pnee_about;
}

GtkWidget*
create_pnee_panel (void)
{
  GtkWidget *pnee_panel;
  GtkWidget *pnee_panel_box;
  GtkWidget *pnee_panel_img;
  GtkWidget *pnee_progress;

  pnee_panel = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (pnee_panel), "window1");

  pnee_panel_box = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (pnee_panel_box);
  gtk_container_add (GTK_CONTAINER (pnee_panel), pnee_panel_box);

  pnee_panel_img = gtk_image_new_from_stock ("gtk-media-record", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (pnee_panel_img);
  gtk_box_pack_start (GTK_BOX (pnee_panel_box), pnee_panel_img, TRUE, TRUE, 0);

  pnee_progress = gtk_progress_bar_new ();
  gtk_widget_show (pnee_progress);
  gtk_box_pack_start (GTK_BOX (pnee_panel_box), pnee_progress, FALSE, FALSE, 0);
  gtk_progress_bar_set_text (GTK_PROGRESS_BAR (pnee_progress), "Xnee Event Recorder and Replayer");
  gtk_progress_bar_set_ellipsize (GTK_PROGRESS_BAR (pnee_progress), PANGO_ELLIPSIZE_MIDDLE);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (pnee_panel, pnee_panel, "pnee_panel");
  GLADE_HOOKUP_OBJECT (pnee_panel, pnee_panel_box, "pnee_panel_box");
  GLADE_HOOKUP_OBJECT (pnee_panel, pnee_panel_img, "pnee_panel_img");
  GLADE_HOOKUP_OBJECT (pnee_panel, pnee_progress, "pnee_progress");

  return pnee_panel;
}

