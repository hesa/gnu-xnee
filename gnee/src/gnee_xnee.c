#include "gnee_xnee.h"

#include <libxnee/print.h>
#include <libxnee/xnee_record.h>
#include <libxnee/xnee_replay.h>
#include <libxnee/xnee_setget.h>

int
gnee_xnee_start_recording(xnee_data* xd)
{
    // FIXME:  Can we do this here, or must we do it in the beginning?
    //xnee_init(xd);
    //xnee_record_init(xd);


/*     include_store = GTK_TREE_STORE */
/*         (lookup_widget(GTK_WIDGET(user_data), "include_event_store")); */
    
/*     valid = gtk_tree_model_get_iter_first */
/*         (GTK_TREE_MODEL(include_store), &iterator); */

/*     while (valid) */
/*     { */
/*         gchar* name; */
/*         gtk_tree_model_get(GTK_TREE_MODEL(include_store), &iterator,  */
/*                            0, &name, -1); */
/*         valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(include_store), &iterator); */

/*         g_print("will record '%s'\n", name); */
/*         ret = xnee_add_range_str(xd, XNEE_DELIVERED_EVENT, name); */

/*         g_free(name); */
/*     } */


    //xnee_set_verbose(xd);

    //ret = xnee_add_range_str(xd, XNEE_DELIVERED_EVENT, "KeyPress");
    //ret = xnee_add_range_str(xd, XNEE_DELIVERED_EVENT, "KeyRelease");
    //ret = xnee_add_range_str(xd, XNEE_DELIVERED_EVENT, "ButtonPress");
    //ret = xnee_add_range_str(xd, XNEE_DELIVERED_EVENT, "ButtonRelease");
    //xnee_add_range_str(xd, XNEE_DELIVERED_EVENT, "MotionNotify");

    xnee_set_loops_left (xd, 10);
    xnee_set_out_byname (xd, "gnee.out");

    xnee_setup_recordext(xd);

    xnee_set_recorder(xd);

    xnee_setup_display(xd);

    xnee_setup_recording(xd);

    //xnee_record_print_record_range(xd, "foo");

    //xnee_record_loop(xd);
    xnee_record_async(xd);

    xnee_close_down(xd);

    return 0;
}

int
gnee_xnee_stop_recording(xnee_data* xd)
{
    return 0;
}

int
gnee_xnee_start_replaying(xnee_data* xd)
{
    return 0;
}

int
gnee_xnee_stop_replaying(xnee_data* xd)
{
    return 0;
}
