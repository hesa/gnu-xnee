#ifndef  GNEE_XNEE__H
#define  GNEE_XNEE__H

#include <config.h>
#include <libxnee/xnee.h>

int
gnee_xnee_start_recording(xnee_data* xd);

int
gnee_xnee_stop_recording(xnee_data* xd);

int
gnee_xnee_start_replaying(xnee_data* xd);

int
gnee_xnee_stop_replaying(xnee_data* xd);

#endif //GNEE_XNEE__H
