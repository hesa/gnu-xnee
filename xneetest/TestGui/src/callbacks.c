#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"


void
on_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  printf ("on_button1_clicked\n");

}


gboolean
on_label4_focus                        (GtkWidget       *widget,
                                        GtkDirectionType  direction,
                                        gpointer         user_data)
{
  printf ("label4_focus\n");
  return FALSE;
}


void
on_label4_move_cursor                  (GtkLabel        *label,
                                        GtkMovementStep  step,
                                        gint             count,
                                        gboolean         extend_selection,
                                        gpointer         user_data)
{
  printf ("label4_move_cursor\n");

}


void
on_new1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  printf ("on_new1_activate\n");
}


void
on_open1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  printf ("on_open1_activate\n");

}


void
on_save1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  printf ("on_save1_activate\n");

}


void
on_save_as1_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  printf ("on_save_as1_activate\n");

}


void
on_quit1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  printf ("on_quit1_activate\n");

}


void
on_cut1_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  printf ("on_cut1_activate\n");

}


void
on_copy1_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  printf ("on_copy1_activate\n");

}


void
on_paste1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  printf ("on_paste1_activate\n");

}


void
on_delete1_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  printf ("on_delete1_activate\n");

}


void
on_about1_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
  printf ("on_about1_activate\n");

}


void
on_radiobutton2_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  printf ("on_radiobutton2_toggled\n");

}


void
on_radiobutton3_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  printf ("on_radiobutton3_toggled\n");

}


void
on_radiobutton4_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  printf ("on_radiobutton4_toggled\n");

}


void
on_checkbutton2_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  printf ("on_checkbutton2_toggled\n");

}


void
on_checkbutton3_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  printf ("on_checkbutton3_toggled\n");

}


void
on_togglebutton1_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  printf ("on_togglebutton1_toggled\n");

}


void
on_togglebutton2_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  printf ("on_togglebutton2_toggled\n");

}


void
on_togglebutton3_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  printf ("on_togglebutton3_toggled\n");

}


void
on_spinbutton1_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{
  printf ("on_spinbutton1_changed\n");
}


void
on_combobox1_changed                   (GtkComboBox     *combobox,
                                        gpointer         user_data)
{
  printf ("on_combobox1_changed\n");

}


void
on_entry1_changed                      (GtkEditable     *editable,
                                        gpointer         user_data)
{
  printf ("on_entry1_changed\n");

}


void
on_entry1_editing_done                 (GtkCellEditable *celleditable,
                                        gpointer         user_data)
{
  printf ("on_entry1_editing_done\n");

}



void
on_calendar1_next_month                (GtkCalendar     *calendar,
                                        gpointer         user_data)
{
  printf ("on_calendar1_next_month\n");

}


void
on_checkbutton1_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
  printf ("on_checkbutton1_toggled\n");

}


void
on_button2_pressed                     (GtkButton       *button,
                                        gpointer         user_data)
{
  printf ("on_button2_pressed\n");

}


void
on_button2_released                    (GtkButton       *button,
                                        gpointer         user_data)
{
  printf ("on_button2_released\n");

}


void
on_calendar1_prev_year                 (GtkCalendar     *calendar,
                                        gpointer         user_data)
{
  printf ("on_calendar1_prev_year\n");

}


void
on_calendar1_prev_month                (GtkCalendar     *calendar,
                                        gpointer         user_data)
{
  printf ("on_calendar1_prev_month\n");

}


void
on_button3_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  printf ("on_button3_clicked\n");

}


void
on_calendar1_day_selected              (GtkCalendar     *calendar,
                                        gpointer         user_data)
{
  printf ("on_calendar1_day_selected\n");
}

