#include "libxnee/xnee.h"
#include "libxnee/xnee_callback.h"




int main()
{
  xnee_data *xd = xnee_new_xnee_data();
  int i ;

  printf ("Set plugin\n");
  xnee_use_plugin(xd, "testcb.so", 1);

  printf ("Test rec callback\n");
  for (i=0;i<100000;i++)
    {
      xd->rec_callback((XPointer)xd, NULL);
    }

  printf ("Test rep callback\n");
  for (i=0;i<100000;i++)
    {
      xd->rep_callback((XPointer)xd, NULL);
    }


  return 0;
}

