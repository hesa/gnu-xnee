#ifndef  MODELS__H
#define  MODELS__H

#include <gtk/gtk.h>
#include <config.h>
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

void
gnee_move_recordable(GtkWidget* gnee_window,
		     char* data_name,
		     char* exclude_store_name,
		     char* include_store_name,
		     int   type, 
		     int   delete);


void
gnee_recordable2xd(GtkWidget* gnee_window,
		   char* include_store_name,
		   int   type);

void
gnee_remove_all_recordables();

#endif /* MODELS__H */
