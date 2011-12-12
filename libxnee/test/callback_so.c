#include <stdio.h>
#include "libxnee/xnee.h"
#include "libxnee/xnee_session.h"

void 
xnee_record_dispatch_external (XPointer xpoint_xnee_data,
		      XRecordInterceptData *data )
{
  static int ctr=0;
  if (++ctr%1000==0) 
    {
      printf(" %d ", ctr);
      fflush(stdout);
    }

  if (  (data==NULL) || (!data->data) )
    {
      xnee_process_count(XNEE_PROCESS_RESET);
      return;
    } 

  XRecordFreeData(data);
  return;
}



void 
xnee_replay_dispatch_external (XPointer type_ref, XRecordInterceptData *data)
{
  static int ctr=0;
  if (++ctr%1000==0) 
    {
      printf(" %d ", ctr);
      fflush(stdout);
    }

  if (  (data==NULL) || (!data->data) )
    {
      return;
    }
  XRecordFreeData(data);
  return;
}
