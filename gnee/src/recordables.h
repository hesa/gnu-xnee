#ifndef  MODELS__H
#define  MODELS__H

#include <gtk/gtk.h>
#include <libxnee/xnee.h>

void 
gnee_recordables_create(GtkWidget* gnee_window);

void
gnee_recordables_include(GtkTreeView* source_list,
                         GtkTreeView* destination_list,
                         xnee_data* xd,
                         int type);

void
gnee_recordables_exclude(GtkTreeView* source_list,
                         GtkTreeView* destination_list,
                         xnee_data* xd,
                         int type);

#endif //MODELS__H
