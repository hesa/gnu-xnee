#ifndef  GNEE_XNEE__H
#define  GNEE_XNEE__H

#include <config.h>
#include <libxnee/xnee.h>



#define move_event(gw, ev)  \
      move_recordable(gw, ev, "exclude_event_store","include_event_store")

#define move_reply(gw, rep)  \
      move_recordable(gw, rep, "exclude_reply_store","include_reply_store")

#define move_request(gw, req)  \
      move_recordable(gw, req, "exclude_request_store","include_request_store")

#define move_error(gw, err)  \
      move_recordable(gw, err, "exclude_error_store","include_error_store")



int
gnee_xnee_init_value(xnee_data* xd);

int
gnee_xnee_start_recording(xnee_data* xd);

int
gnee_xnee_stop_recording(xnee_data* xd);

int
gnee_xnee_start_replaying(xnee_data* xd);

int
gnee_xnee_stop_replaying(xnee_data* xd);

#endif //GNEE_XNEE__H
