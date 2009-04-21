#include "libxnee/xnee.h"
#include "libxnee/xnee_callback.h"




int main()
{
  xnee_data *xd = xnee_new_xnee_data();

  xnee_set_verbose(xd);
  xnee_use_plugin(xd, "testcb.so");

  xd->rec_callback(xd, NULL);

}

