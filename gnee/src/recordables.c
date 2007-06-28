/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 2002, 2003, 2004, 2005 Henrik Sandklef
 *                                                                   
 * This program is free software; you can redistribute it and/or     
 * modify it under the terms of the GNU General Public License       
 * as published by the Free Software Foundation; either version 3    
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

#include "recordables.h"
#include "support.h"
#include "gnee_xnee.h"
#include "libxnee/xnee.h"
#include "libxnee/datastrings.h"

#define GNEE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

extern xnee_data   *ext_xd;
extern GtkWidget   *ext_gnee_window;



void
move_between_lists(GtkTreeView* source_list, 
                   GtkTreeView* destination_list,
                   xnee_data* xd,
                   int type,
                   gboolean include);

void
create_recordable_list(GtkWidget* gnee_window,
                       struct data_description*,
                       char* exclude_store_name,
                       char* include_store_name);

void
gnee_recordables_create(GtkWidget* gnee_window)
{
  create_recordable_list
    (gnee_window, xnee_get_event_names(), 
     "exclude_event_store", "include_event_store");

  create_recordable_list 
    (gnee_window, xnee_get_error_names(), 
     "exclude_error_store", "include_error_store"); 

  create_recordable_list 
    (gnee_window, xnee_get_reply_names(), 
     "exclude_reply_store", "include_reply_store"); 

  create_recordable_list 
    (gnee_window, xnee_get_request_names(), 
     "exclude_request_store", "include_request_store"); 
}

void
gnee_recordables_include(GtkTreeView* source_list,
                         GtkTreeView* destination_list,
                         xnee_data* xd,
                         int type)
{
  move_between_lists(source_list, destination_list, xd, type, TRUE);
}

void
gnee_recordables_exclude(GtkTreeView* source_list,
                         GtkTreeView* destination_list,
                         xnee_data* xd,
                         int type)
{
    move_between_lists(source_list, destination_list, xd, type, FALSE);
}

void
move_between_lists(GtkTreeView* source_list, 
                   GtkTreeView* destination_list,
                   xnee_data* xd,
                   int type,
                   gboolean include)
{
    GtkTreeSelection   *selection;
    char               *selection_value;
    gboolean            selected;
    GtkTreeModel       *model;
    GtkTreeIter         iterator;
    GtkTreeStore       *store;
/*     gint                new_order; */
    
    selection = gtk_tree_view_get_selection(source_list);
    
    selected = gtk_tree_selection_get_selected(selection,
                                               &model,
                                               &iterator);
    if (selected)
    {
        gtk_tree_model_get(model, &iterator,
                           0, &selection_value, 
                           -1);
        
        store = GTK_TREE_STORE(model);
        gtk_tree_store_remove(store, &iterator);
        
        store = GTK_TREE_STORE(gtk_tree_view_get_model
                               (destination_list));
        gtk_tree_store_append(store, &iterator, NULL);
        gtk_tree_store_set(store, &iterator, 0, selection_value, -1);

        if (xd == NULL)
        {
            GNEE_DEBUG(("xd is null\n"));
        }

        if (include)
        {
	  GNEE_DEBUG(("will include '%s' %d \n", selection_value, type));
	  xnee_add_range_str
	    (xd, type, selection_value);
        }
        else
        {
            GNEE_DEBUG(("will exclude '%s' %d \n", selection_value, type));
            xnee_rem_data_from_range_str
                (xd, -1, selection_value);            
        }

        g_free(selection_value);
        
    }

}

void
create_recordable_list(GtkWidget* gnee_window,
                       struct data_description* data_names,
                       char* exclude_store_name,
		       char* include_store_name)
{

    GtkTreeStore       *store;
    GtkTreeIter         iterator; 
    int                 i;

    store = gtk_tree_store_new (1, G_TYPE_STRING);

    i = 0;
    while (data_names[i].data_nr != -1)
    {
        gtk_tree_store_append(store, &iterator, NULL);
        gtk_tree_store_set(store, &iterator, 0, data_names[i].data_name, -1);
        i++;
    }

    GNEE_HOOKUP_OBJECT(gnee_window, store, exclude_store_name);

    store = gtk_tree_store_new (1, G_TYPE_STRING);

    GNEE_HOOKUP_OBJECT(gnee_window, store, include_store_name);
}





void
gnee_move_recordable(GtkWidget* gnee_window,
		char* data_name,
		char* exclude_store_name,
		char* include_store_name,
		int   type, 
		int   delete)
{
  GtkTreeStore       *excl_store;
  GtkTreeStore       *incl_store;
  GtkTreeIter         iter; 
  GtkTreeIter         incl_iter; 
  
  int valid;
  int incl_valid;
  gint row_count=0;


  excl_store = 
    g_object_get_data (G_OBJECT(gnee_window), exclude_store_name);
  
  valid = gtk_tree_model_get_iter_first (GTK_TREE_MODEL(excl_store), &iter);
  
  incl_store = 
    g_object_get_data (G_OBJECT(gnee_window), include_store_name);
  
  incl_valid = 
    gtk_tree_model_get_iter_first (GTK_TREE_MODEL(incl_store), &incl_iter);
  
  
  while (valid)
    {
      gchar *str_data;
      gchar *str_data2;
      
      gtk_tree_model_get (GTK_TREE_MODEL(excl_store), &iter, 
			  0, &str_data,
			  -1);
      
      GNEE_DEBUG(("      comparing '%s' '%s'\n", 
		  data_name,
		  str_data));
      if (strncmp(str_data, data_name, strlen(str_data))==0)
	{
	  gtk_tree_store_remove(GTK_TREE_STORE(excl_store), &iter);
	  GNEE_DEBUG(("   EXCL move_recordable %s %s;\n", 
		      data_name,
		      str_data));
  
	  
	  while (valid)
	    {
	      valid = 
		gtk_tree_model_iter_next (GTK_TREE_MODEL(incl_store), 
					  &incl_iter);

	    }
	  if (delete)
	    {
	      gtk_tree_model_get (GTK_TREE_MODEL(incl_store), 
				  &incl_iter, 
				  0, &str_data2,
				  -1);
	      
	      GNEE_DEBUG(("   INCL move_recordable %s \n", str_data2)); 
	    }
	  gtk_tree_store_append(GTK_TREE_STORE(incl_store), 
				&incl_iter, 
				NULL);
	  gtk_tree_store_set(GTK_TREE_STORE(incl_store), 
			     &incl_iter, 
			     0, str_data, 
			     -1);
	  if (delete)
	    {
	      xnee_rem_data_from_range_str (ext_xd, type, str_data); 
	    }
	  else
	    {
	      xnee_add_range_str (ext_xd, type, str_data);
	    }
	}
      
      g_free (str_data);
      
      row_count++;
      valid = gtk_tree_model_iter_next (GTK_TREE_MODEL(excl_store), &iter);
    } 
}


void
gnee_recordable2xd(GtkWidget* gnee_window,
		   char* include_store_name,
		   int   type)
{
  GtkTreeStore       *incl_store;
  GtkTreeIter         incl_iter; 
  
  int valid = 1 ;
  int incl_valid = 0;
  gint row_count=0;

  xnee_verbose((ext_xd,"    read_recordable (,,..);\n")); 
  
  incl_store = 
    g_object_get_data (G_OBJECT(gnee_window), include_store_name);
  
  incl_valid = 
    gtk_tree_model_get_iter_first (GTK_TREE_MODEL(incl_store), &incl_iter);
  
  
  while (valid && incl_store && incl_valid)
    {
      gchar *str_data;
      
      gtk_tree_model_get (GTK_TREE_MODEL(incl_store), 
			  &incl_iter, 
			  0, &str_data,
			  -1);
      xnee_verbose((ext_xd,"Found a string: %s\n", str_data));
      xnee_add_range_str (ext_xd, type, str_data); 
      g_free (str_data);
      
      row_count++;
      valid = gtk_tree_model_iter_next (GTK_TREE_MODEL(incl_store), &incl_iter);
    } 
}



void
gnee_remove_type(GtkWidget* gnee_window,
		 char* exclude_store_name,
		 char* include_store_name,
		 int   type)
{
  GtkTreeStore       *excl_store;
  GtkTreeStore       *incl_store;
  GtkTreeIter         iter; 
  GtkTreeIter         incl_iter; 
  
  int valid;
  int incl_valid;
  gint row_count=0;


  excl_store = 
    g_object_get_data (G_OBJECT(gnee_window), exclude_store_name);
  
  incl_store = 
    g_object_get_data (G_OBJECT(gnee_window), include_store_name);
  
  incl_valid = 
    gtk_tree_model_get_iter_first (GTK_TREE_MODEL(incl_store), &incl_iter);
  
  
  valid = gtk_tree_model_get_iter_first (GTK_TREE_MODEL(excl_store), &iter);
  
  while (valid)
    {
      gchar *str_data;
      gchar *str_data2;
      
      gtk_tree_model_get (GTK_TREE_MODEL(excl_store), &iter, 
			  0, &str_data,
			  -1);
      
      GNEE_DEBUG(("      REMOVING '%s' '%s'\n", str_data));
      gtk_tree_store_remove(GTK_TREE_STORE(excl_store), &iter);


      /* find last pos in resulting tree */
      incl_valid = 
	gtk_tree_model_get_iter_first (GTK_TREE_MODEL(incl_store), &incl_iter);
      while (valid)
	{
	  valid = 
	    gtk_tree_model_iter_next (GTK_TREE_MODEL(incl_store), 
				      &incl_iter);
	  
	}
      gtk_tree_model_get (GTK_TREE_MODEL(incl_store), 
			  &incl_iter, 
			  0, &str_data2,
			  -1);
      
      GNEE_DEBUG(("   INCL move_recordable %s \n", str_data2)); 

      gtk_tree_store_append(GTK_TREE_STORE(incl_store), 
			    &incl_iter, 
			    NULL);
      gtk_tree_store_set(GTK_TREE_STORE(incl_store), 
			 &incl_iter, 
			     0, str_data, 
			     -1);
      g_free (str_data);
      g_free (str_data2);

      row_count++;
      valid = gtk_tree_model_get_iter_first (GTK_TREE_MODEL(excl_store), &iter);
    } 


}


void
gnee_remove_all_recordables()
{
  GNEE_DEBUG(("Removing events **************** \n"));
  gnee_remove_type(ext_gnee_window,
		   "include_event_store",
		   "exclude_event_store",
		   XNEE_EVENT);
  xnee_verbose((ext_xd, "removing request\n"));
  gnee_remove_type(ext_gnee_window,
		   "include_request_store",
		   "exclude_request_store",
		   XNEE_REQUEST);

  xnee_verbose((ext_xd, "removing error\n"));
  gnee_remove_type(ext_gnee_window,
		   "include_error_store",
		   "exclude_error_store",
		   XNEE_ERROR);

  xnee_verbose((ext_xd, "removing reply\n"));
  gnee_remove_type(ext_gnee_window,
		   "include_reply_store",
		   "exclude_reply_store",
		   XNEE_REPLY);
}





