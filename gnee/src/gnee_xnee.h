#ifndef  GNEE_XNEE__H
#define  GNEE_XNEE__H

#include "config.h"
#include <recordables.h>
#include <libxnee/xnee.h>
#include <libxnee/print.h>
#include <libxnee/xnee_record.h>
#include <libxnee/xnee_replay.h>
#include <libxnee/xnee_setget.h>
#include <libxnee/datastrings.h>
#include <libxnee/xnee_range.h>
#include <libxnee/xnee_threshold.h>
#include <libxnee/xnee_setget.h>
#include <libxnee/xnee_resource.h>

/* #define USE_GNEE_DEBUG */

#ifdef USE_GNEE_DEBUG
#define GNEE_DEBUG(a) printf a ; 
#else
#define GNEE_DEBUG(a) 
#endif


#define GNEE_OK 0

typedef struct 
{
  int use_rec_display ; 
  int use_rep_display ; 

  int use_speed ;
  int speed ; 

  

} gnee_xnee ;


int
gnee_set_cb(char * cb_name, int on_or_off);

void
gnee_set_sb(char *sb_name, int val);

int 
gx_set_replay_display(xnee_data *xd, gnee_xnee *gx);

int 
gx_set_record_display(xnee_data *xd, gnee_xnee *gx);

int 
gx_set_variable_data(xnee_data *xd, gnee_xnee *gx);

#define gx_set_xosd_feedback()   xnee_set_xosd_feedback(ext_xd) 
#define gx_set_no_feedback()     xnee_set_no_feedback(ext_xd) 
#define gx_set_stderr_feedback() xnee_set_stderr_feedback(ext_xd) 


#define gnee_set_record_display()  gnee_set_various_display(0)
#define gnee_set_replay_display()  gnee_set_various_display(1)


#define gnee_set_verbose()   gnee_set_verbosity(1)
#define gnee_unset_verbose() gnee_set_verbosity(0)

#define gnee_set_sync()      gnee_set_cb("skip_sync_cb",1)
#define gnee_unset_sync()    gnee_set_cb("skip_sync_cb",0)

#define gnee_set_force()     gnee_set_cb("force_rep_cb",1)
#define gnee_unset_force()   gnee_set_cb("force_rep_cb",0)
#define gx_set_all_clients()       xnee_set_all_clients(ext_xd)
#define gx_unset_all_clients()     xnee_unset_all_clients(ext_xd)

#define gx_set_human_printout()    xnee_set_human_printout(ext_xd)
#define gx_set_xnee_printout()     xnee_set_human_printout(ext_xd)

#define gx_add_event(gw, ev)  \
      gnee_move_recordable(gw, ev, \
		      "exclude_event_store","include_event_store", \
		      XNEE_EVENT, XNEE_FALSE)

#define gx_del_event(gw, ev)  \
      gnee_move_recordable(gw, ev, \
		      "include_event_store", "exclude_event_store",\
		      XNEE_EVENT, XNEE_TRUE)


#define gx_add_reply(gw, rep)  \
      gnee_move_recordable(gw, rep, \
		      "exclude_reply_store","include_reply_store", \
		      XNEE_REPLY, XNEE_FALSE)

#define gx_del_reply(gw, rep)  \
      gnee_move_recordable(gw, rep, \
		      "exclude_reply_store","include_reply_store", \
		      XNEE_REPLY, XNEE_TRUE)

#define gx_add_request(gw, req)  \
      gnee_move_recordable(gw, req, \
	              "exclude_request_store","include_request_store", \
		      XNEE_REQUEST, XNEE_FALSE)

#define gx_del_request(gw, req)  \
      gnee_move_recordable(gw, req, \
	              "exclude_request_store","include_request_store", \
		      XNEE_REQUEST, XNEE_TRUE)

#define gx_add_error(gw, err)  \
      gnee_move_recordable(gw, err, \
      "exclude_error_store","include_error_store" ,\
      XNEE_ERROR, XNEE_FALSE)

#define gx_del_error(gw, err)  \
      gnee_move_recordable(gw, err, \
      "exclude_error_store","include_error_store" ,\
      XNEE_ERROR, XNEE_TRUE)



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

#define gx_set_events_max(ext_xd,loops) xnee_set_events_max(ext_xd, loops);
#define gx_get_events_max(ext_xd)       xnee_get_events_max(ext_xd)
#define gx_get_events_left(ext_xd)      xnee_get_events_left(ext_xd)

#define gx_set_data_max(ext_xd,loops)   xnee_set_data_max(ext_xd, loops);
#define gx_get_data_max(ext_xd)         xnee_get_data_max(ext_xd)
#define gx_get_data_left(ext_xd)        xnee_get_data_left(ext_xd)

#define gx_set_time_max(ext_xd,time)    xnee_set_time_max(ext_xd,time);
#define gx_get_time_max(ext_xd)         xnee_get_time_max(ext_xd)
#define gx_get_time_left(ext_xd)        xnee_get_time_left(ext_xd)



#define gx_set_max_threshold(xd,thresh) xnee_set_max_threshold(xd,thresh)
#define gx_set_min_threshold(xd,thresh) xnee_set_min_threshold(xd,thresh)
#define gx_set_tot_threshold(xd,thresh) xnee_set_tot_threshold(xd,thresh)

#define gnee_get_max_threshold(xd) xnee_get_max_threshold(xd)
#define gnee_get_min_threshold(xd) xnee_get_min_threshold(xd)
#define gnee_get_tot_threshold(xd) xnee_get_tot_threshold(xd)

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

void 
gx_init_gx(gnee_xnee *gx);

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
gx_display_errror(int err_nr);

void 
gx_undisplay_errror(void);

void
gnee_set_events_max(int val);

void
gnee_set_data_max(int val);

void
gnee_set_time_max(int val);

void
gnee_set_max_thresh(int val);

void
gnee_set_min_thresh(int val);

void
gnee_set_tot_thresh(int val);

void 
gnee_set_rec_file(char *filename);

void 
gnee_set_rep_file(char *filename);

char *
gx_get_default_filename();

void
gx_create_km_lists();

void
gnee_close_down();

int 
gx_set_speed (int val);

void 
gx_set_xd_settings();

int 
gx_set_use_speed (int val) ;




#endif /* GNEE_XNEE__H */
