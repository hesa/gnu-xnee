#include <stdio.h>
#include "libxnee/xnee.h"

void 
xnee_record_dispatch (XPointer xpoint_xnee_data,
		      XRecordInterceptData *data )
{
  printf(" dummy callback\n");

  if ( (!data->data) || (data==NULL) )
    {
      XRecordFreeData(data);
      xnee_process_count(XNEE_PROCESS_RESET);
      return;
    } 

  printf(" dummy callback\n");
  fflush (stdout);
  XRecordFreeData(data);
  return;
}



void 
xnee_replay_dispatch (XPointer type_ref, XRecordInterceptData *data)
{
  
  XRecordDatum *xrec_data;
  int           type;
  xnee_data    *xd;
  int rec_window_pos = 0;
  XWindowAttributes window_attributes_return;
  int ret;

  printf(" dummy callback\n");

  if (data->data==NULL)
    {
      return;
    }
  XRecordFreeData(data);

}
