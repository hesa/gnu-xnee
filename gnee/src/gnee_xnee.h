#ifndef  GNEE_XNEE__H
#define  GNEE_XNEE__H

#include <config.h>
#include <libxnee/xnee.h>

#define USE_GNEE_DEBUG

#ifdef USE_GNEE_DEBUG
#define GNEE_DEBUG(a) printf a
#else
#define GNEE_DEBUG(a) 
#endif


#define GNEE_OK 0

typedef struct 
{
  int use_rec_display ; 
  int use_rep_display ; 
} gnee_xnee ;

#define gx_move_event(gw, ev)  \
      move_recordable(gw, ev, "exclude_event_store","include_event_store")

#define gx_move_reply(gw, rep)  \
      move_recordable(gw, rep, "exclude_reply_store","include_reply_store")

#define gx_move_request(gw, req)  \
      move_recordable(gw, req, \
      "exclude_request_store","include_request_store")

#define gx_move_error(gw, err)  \
      move_recordable(gw, err, "exclude_error_store","include_error_store")

#define gx_set_replay_speed(xd,s)        xnee_set_replay_speed(xd,s)
#define gx_set_interval(xd,i)            xnee_set_interval(xd,i)      
#define gx_set_verbose(xd)               xnee_set_verbose(xd)
#define gx_unset_verbose(xd)             xnee_unset_verbose(xd)
#define gx_set_rc_byname(xd,f)           xnee_set_rc_byname(xd,f)
#define gx_get_rc_file(xd)               xnee_get_rc_file(xd)
#define gx_add_resource(xd)              xnee_add_resource (xd)
#define gx_set_out_byname(xd,f)          xnee_set_out_byname(xd,f)
#define gx_get_rc_name(xd)               xnee_get_rc_name(xd)
#define gx_write_settings_to_file(xd,f)  xnee_write_settings_to_file(xd,f) 
#define gx_set_km(xd,km,mode)            xnee_set_km(xd,km,mode) 
#define gx_get_km(ext_xd,mode)           xnee_get_km(ext_xd,mode)

#define  gx_set_events_max(ext_xd,loops) xnee_set_events_max(ext_xd, loops)
#define  gx_get_events_max(ext_xd)       xnee_get_events_max(ext_xd)
#define  gx_get_events_left(ext_xd)      xnee_get_events_left(ext_xd)
#define  gx_set_data_max(ext_xd,loops)   xnee_set_data_max(ext_xd, loops)
#define  gx_get_data_max(ext_xd)         xnee_get_data_max(ext_xd)
#define  gx_get_data_left(ext_xd)        xnee_get_data_left(ext_xd)
#define  gx_set_time_max(ext_xd,time)    xnee_set_time_max(ext_xd,time)
#define  gx_get_time_left(ext_xd)        xnee_get_time_left(ext_xd)



#define  gx_set_max_threshold(xd,thresh) xnee_set_max_threshold(xd,thresh)
#define  gx_set_min_threshold(xd,thresh) xnee_set_min_threshold(xd,thresh)
#define  gx_set_tot_threshold(xd,thresh) xnee_set_tot_threshold(xd,thresh)

#define gx_set_using_rec_display(gx)   gx->use_rec_display=1
#define gx_unset_using_rec_display(gx) gx->use_rec_display=0
#define gx_set_using_rep_display(gx)   gx->use_rep_display=1
#define gx_unset_using_rep_display(gx) gx->use_rep_display=0

#define gx_set_sync(ext_xd)            xnee_set_sync(ext_xd)
#define gx_unset_sync(ext_xd)          xnee_unset_sync(ext_xd)
#define gx_set_force_replay(ext_xd)    xnee_set_force_replay(ext_xd)
#define gx_unset_force_replay(ext_xd)  xnee_unset_force_replay(ext_xd)


void 
gx_save(void);

int
gx_init_value(xnee_data* xd);

int
gx_start_recording(xnee_data* xd);

int
gx_stop_recording(xnee_data* xd);

int
gx_start_replaying(xnee_data* xd);

int
gx_stop_replaying(xnee_data* xd);





void 
gx_display_errror(int err_nr, char *err_str);

void 
gx_undisplay_errror(void);

#endif //GNEE_XNEE__H
