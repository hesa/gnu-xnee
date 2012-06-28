/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *   Copyright (C) 2010, 2011, 2012     Henrik Sandklef 
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

#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/xnee_range.h"
#include "libxnee/xnee_xinput.h"

#define XNEE_XINPUT_PRINT_MASTER_OR_SLAVE(xd, devid, fd)	\
  if ( ! xd->xi_data.xi_devices[devid].is_slave)		\
    fprintf (fd, "%d", XNEE_PROTO_XINPUT_EVENT_MASTER);		\
  else								\
    fprintf (fd, "%d", XNEE_PROTO_XINPUT_EVENT_SLAVE);

#define XNEE_XINPUT_IS_SLAVE(xd, devid)     	\
  xd->xi_data.xi_devices[devid].is_slave


#ifdef XNEE_XINPUT_SUPPORT

int
xnee_init_xinput(xnee_data *xd)
{
  if ( xd==NULL )
    {
      return -1;
    }
  xd->xi_data.forced_core_replay = 0;
  xd->xi_data.recording_enabled  = 1;
  
  return XNEE_OK;
}

int
xnee_init_xinput_devices(xnee_data *xd)
{

  int major = XI_2_Major;
  int minor = XI_2_Minor;
  int nr_of_devices;

  int i;
  XIDeviceInfo *xi_info, *dev;
  static int already_done = 0;

  if ( (xd==NULL) || (xd->control==NULL) )
    {
      return -1;
    }

  if (already_done != 0)
    {
      return XNEE_OK;
    }

  if (XIQueryVersion(xd->control, &major, &minor) != Success ||
      (major * 1000 + minor) < (XI_2_Major * 1000 + XI_2_Minor))
    {
      if (major  < XI_2_Major )
	{
	  fprintf(stderr, "XI2 not supported\n");
	  return EXIT_FAILURE;
	}
      else
	{
	  fprintf(stderr, "XI2 may not be supported (minor: %d/%d)\n", 
		  minor, XI_2_Minor);
	}
    }

  xi_info = XIQueryDevice(xd->control, XIAllDevices, &nr_of_devices);
      
  for(i = 0; i < nr_of_devices; i++)
    {
      dev = &xi_info[i];
	
      xd->xi_data.xi_devices[dev->deviceid].name     = strdup(dev->name);
      xd->xi_data.xi_devices[dev->deviceid].deviceid = dev->deviceid;
      xd->xi_data.xi_devices[dev->deviceid].device   = NULL ;

      if (dev->use == XIMasterPointer || dev->use == XIMasterKeyboard)
	{
	  xd->xi_data.xi_devices[dev->deviceid].is_slave = 0;
	}
      else
	{
	  xd->xi_data.xi_devices[dev->deviceid].is_slave = 1;
	}
    }
  xd->xi_data.nr_of_xi_devices   = nr_of_devices;

  XIFreeDeviceInfo(xi_info);
  
  already_done = 1 ;

  return 0;
}

XDevice *
xnee_get_xinput_device(xnee_data *xd, int deviceid)
{
  int count;
  int i;
  int dev_pointer;
  XDeviceInfo *dev_info;
  XDevice *xdevice = NULL ;
  
  xnee_verbose((xd, "--->  xnee_get_xinput_device\n"));

  if ( xd->xi_data.xi_devices[deviceid].device == NULL )
    {
      if (!(dev_info = XListInputDevices(xd->fake, &count)) || !count)
	{
	  fprintf(stderr, "Cannot list input devices\n");
	  return NULL;
	}
      xnee_verbose((xd, "---  xnee_get_xinput_device  count=%d\n", count));

      dev_pointer = 0;
      for (i = 0; i < count; i++) 
	{
	  xnee_verbose((xd, "pointer: %d %d (%d)  [%d %d ]\n", 
			i, 
			(int)dev_info[i].id, 
			(int)deviceid,
			(int)xd->xi_data.xi_devices[i].deviceid,
			(int)xd->xi_data.nr_of_xi_devices));

	  if ( dev_info[i].id  == deviceid) 
	    {
	      dev_pointer = i; 
	      xdevice = XOpenDevice(xd->fake, dev_info[dev_pointer].id);
	      xd->xi_data.xi_devices[deviceid].device=xdevice;
	      break;
	    }
	}

      XFreeDeviceList(dev_info);
    }

  xnee_verbose((xd, "<---  xnee_get_xinput_device\n"));
  
  /* we may return NULL, so make sure to check */
  return xd->xi_data.xi_devices[deviceid].device;
}

int
xnee_get_xinput_event_base(Display *dpy)
{
  int ret;
  int ext_op;
  static int ext_fi_ev = 0 ;
  int ext_fi_er;

  if ( ext_fi_ev == 0)
    {
      
      if ( dpy == NULL )
	{
	  dpy = XOpenDisplay(NULL);
	}
      if ( dpy == NULL )
	{
	  return -1;
	}
      ret = XQueryExtension(dpy, 
			    XNEE_XINPUT_EXTENSION_NAME, 
			    &ext_op, 
			    &ext_fi_ev, 
			    &ext_fi_er);
      if ( ! ret )
	{
	  return -1;
	}
    }
  return ext_fi_ev;
}


int
xnee_has_xinput(xnee_data *xd)
{
  if (xd==NULL)
    {
      return -1;
    }
  
  if ( xd->xi_data.xinput_event_base <= 0 )
    {
      return 0;
    }

  return 1;
}


int 
xnee_handle_xinput_event(xnee_data * xd, 
			 int  event_type, 
			 XRecordDatum *xrec_data,
			 Time server_time)
{
  static saved_xinput_event sxe;
  static int valuator_counter = 0;
  int event_base;
  FILE *out ;

  if  (xd == NULL ) 
    {
      return XNEE_NO_MAIN_DATA;
    }


  if (xnee_xinput_disabled(xd))
    {
      return XNEE_OK;
    }

  out = xd->out_file;
  
   /* printf ("\nXI HE base=%d   event_type=%d  last:%d  \n",xd->xi_data.xinput_event_base, event_type, sxe.type);    */

  event_base = xd->xi_data.xinput_event_base;

  /*
   * Are we using Xinput
   *  AND
   * the event is a xinput device event
   */
  if ( ( event_base > 0 )          &&
       ( event_type > event_base ) &&
       ( event_type <= (event_base + 6) ))
    {
      /* printf ("XI XI :: "); */

      int ordinary_event_nr ;
      deviceKeyButtonPointer *e;
      e = (deviceKeyButtonPointer *) &xrec_data->event ;
      
      xnee_verbose((xd, "handle xi:: dev-id%d type:%d  ", 
		    e->deviceid,
		    event_type));
      ordinary_event_nr = event_type - event_base + 1;
      sxe.type         =  ordinary_event_nr ;
      
      /* printf ("%d \n", ordinary_event_nr); */

      /* printf ("GOT %d | detail: %d  saved=%d\n", ordinary_event_nr, xrec_data->event.u.u.detail, sxe.type); */
      
      /* MotionEvent */
      if ( ordinary_event_nr == MotionNotify)
	{
	  sxe.x        = xrec_data->event.u.keyButtonPointer.rootX;
	  sxe.y        = xrec_data->event.u.keyButtonPointer.rootY;
	  sxe.time     = xrec_data->event.u.keyButtonPointer.time;

	  /*
	  printf ("XI2 Motion (%d/%d): root: %dx%d  XI:%dx%d | xy:%dx%d  axes:%d/%d  %d %d %d %d %d %d \n", 
		  ((XDeviceMotionEvent*)&xrec_data->event)->type,
		  ((XDeviceMotionEvent*)&xrec_data->event)->deviceid,
		  xrec_data->event.u.keyButtonPointer.rootX,
		  xrec_data->event.u.keyButtonPointer.rootY,
		  ((XDeviceMotionEvent*)&xrec_data->event)->x_root,
		  ((XDeviceMotionEvent*)&xrec_data->event)->y_root,
		  ((XDeviceMotionEvent*)&xrec_data->event)->x,
		  ((XDeviceMotionEvent*)&xrec_data->event)->y,
		  ((XDeviceMotionEvent*)&xrec_data->event)->first_axis,
		  ((XDeviceMotionEvent*)&xrec_data->event)->axes_count,
		  ((XDeviceMotionEvent*)&xrec_data->event)->axis_data[0],
		  ((XDeviceMotionEvent*)&xrec_data->event)->axis_data[1],
		  ((XDeviceMotionEvent*)&xrec_data->event)->axis_data[2],
		  ((XDeviceMotionEvent*)&xrec_data->event)->axis_data[3],
		  ((XDeviceMotionEvent*)&xrec_data->event)->axis_data[4],
		  ((XDeviceMotionEvent*)&xrec_data->event)->axis_data[5]

		  );
	  */

	  /* 
	   * Rest of the data is sent in an Devicevaluator event, 
	   * store what we have and continue
	   */
	}
      
      /* ButtonPress / ButtonRelease */
      else if ( ( ordinary_event_nr ==  ButtonPress) || ( ordinary_event_nr ==  ButtonRelease) )
	{
	  XNEE_XINPUT_PRINT_MASTER_OR_SLAVE(xd, e->deviceid, out);
	  fprintf (out, ",%u,0,0,%d,0,0,%lu,%d,%s\n",
		   sxe.type,
		   xrec_data->event.u.u.detail, 
		   xrec_data->event.u.keyButtonPointer.time,
		   /* sxe.time, */
		   e->deviceid,
		   xd->xi_data.xi_devices[e->deviceid].name);

	}
      else if ( ( ordinary_event_nr ==  KeyPress) || ( ordinary_event_nr ==  KeyRelease) )
	{
	  XNEE_XINPUT_PRINT_MASTER_OR_SLAVE(xd, e->deviceid, out);
	  fprintf (out, ",%u,0,0,0,%d,0,%lu,%d,%s\n",
		   ordinary_event_nr,
		   e->detail ,
		   server_time,
		   /* sxe.time, */
		   e->deviceid,
		   xd->xi_data.xi_devices[e->deviceid].name);
	}
    }
  else if ( event_type == event_base)
    {
      /* printf (" -:: SAME ::- "); */

      /*
       *
       *  DaviceValuator event
       *
       */
      deviceValuator *e;
      e = (deviceValuator *) &xrec_data->event ;


      valuator_counter++;

      /*	printf ("deviceValuator: %d %d (%d),  %d, %d (%d %d %d %d %d %d)    sxe.type=%d\n", 
		e->type, e->deviceid, 	sxe.deviceid,		e->num_valuators, 
		e->first_valuator, 	e->valuator0, 
		e->valuator1, e->valuator2, e->valuator3, e->valuator4, e->valuator5,	sxe.type
		);		*/		
      if ( valuator_counter < e->num_valuators )
	{
	  xnee_verbose((xd, "  valuator not printed %d / %d \n",
			valuator_counter , e->num_valuators ));
/* 	  return XNEE_OK; */
	}
      valuator_counter = 0 ;



      if ( sxe.type == MotionNotify )
	{
	  if (XNEE_XINPUT_IS_SLAVE(xd, e->deviceid))
	    {
	      XNEE_XINPUT_PRINT_MASTER_OR_SLAVE(xd, e->deviceid, out);
	      fprintf (out, ",%d,%d,%d,0,0,0,%lu,%d,'%s'\n",
		       sxe.type,
		       /* (int)e->valuator0,   */
		       /* (int)e->valuator1,   */
		       sxe.x,
		       sxe.y,
		       server_time,
		       /* sxe.time, */
		       e->deviceid,
		       xd->xi_data.xi_devices[e->deviceid].name);
	    }
	}
      else
	{
	  fprintf (stderr, 
		   "WARNING: Enough valuators,"
		   "but non motion/button event (%d) so not printing\n", sxe.type);      
	  return -1;
	}
      sxe.x = 0;
      sxe.y = 0;
      sxe.button = 0 ;
    }
  else
    {
      fprintf (stderr, 
	       "WARNING: Enough valuators ... Xnee believes this to not be a valid XI event\n");      
      return -1;
    }
  return XNEE_OK;
}

int 
xnee_handle_xinput_event_human(xnee_data * xd, 
			       int event_type, 
			       XRecordDatum *xrec_data,
			       Time server_time)
{
  static saved_xinput_event sxe;
  FILE *out ;

  if  (xd == NULL ) 
    {
      return XNEE_NO_MAIN_DATA;
    }


  if (xnee_xinput_disabled(xd))
    {
      return XNEE_OK;
    }

  out = xd->out_file;
  
  /*
   * Are we using Xinput
   *  AND
   * the event is a xinput device event
   */
  if ( ( xd->xi_data.xinput_event_base > 0 )          &&
       ( event_type > xd->xi_data.xinput_event_base ) &&
       ( event_type <= (xd->xi_data.xinput_event_base + 6) ))
    {
      int ordinary_event_nr ;
      deviceKeyButtonPointer *e;
      e = (deviceKeyButtonPointer *) &xrec_data->event ;
      
      
      ordinary_event_nr = event_type - xd->xi_data.xinput_event_base + 1;
      sxe.type         =  ordinary_event_nr ;
      
      /* MotionEvent */
      if ( ordinary_event_nr == MotionNotify)
	{
	  sxe.x        = xrec_data->event.u.keyButtonPointer.rootX;
	  sxe.y        = xrec_data->event.u.keyButtonPointer.rootY;
	  sxe.time     = xrec_data->event.u.keyButtonPointer.time;
	  
	  /* 
	   * Rest of the data is sent in an Devicevaluator event, 
	   * store what we have and continue
	   */
	}
      
      /* ButtonPress / ButtonRelease */
      else if ( ( ordinary_event_nr ==  ButtonPress) || ( ordinary_event_nr ==  ButtonRelease) )
	{
	  XNEE_XINPUT_PRINT_MASTER_OR_SLAVE(xd, e->deviceid, out);
	}
      else if ( ( ordinary_event_nr ==  KeyPress) || ( ordinary_event_nr ==  KeyRelease) )
	{
	  XNEE_XINPUT_PRINT_MASTER_OR_SLAVE(xd, e->deviceid, out);
	  fprintf (out, ",%u,0,0,0,%d,0,%lu,%d,%s\n",
		   ordinary_event_nr,
		   e->detail ,
		   sxe.time,
		   e->deviceid,
		   xd->xi_data.xi_devices[e->deviceid].name);
	}
    }
  else if ( event_type == xd->xi_data.xinput_event_base)
    {
      
      /*
       *
       *  DaviceValuator event
       *
       */
      deviceValuator *e;
      e = (deviceValuator *) &xrec_data->event ;
      
      /*	printf ("deviceValuator: %d %d (%d),  %d, %d (%d %d %d %d %d %d)    sxe.type=%d\n", 
		e->type, e->deviceid, 	sxe.deviceid,		e->num_valuators, 
		e->first_valuator, 	e->valuator0, 
		e->valuator1, e->valuator2, e->valuator3, e->valuator4, e->valuator5,	sxe.type
		);		*/		
      if ( sxe.type == MotionNotify )
	{
	  
	  XNEE_XINPUT_PRINT_MASTER_OR_SLAVE(xd, e->deviceid, out);
	  (void)xd->data_fp (xd->out_file,
			     ",XInput Event=%s  Number=%d ",
			     xd->xi_data.xi_devices[e->deviceid].name,
			     sxe.type);
	  (void)xd->data_fp (xd->out_file,"\n");

	  sxe.x = 0;
	  sxe.y = 0;
	  sxe.button = 0 ;
	}
    }
  else
    {
      return -1;
    }
  return XNEE_OK;
}




int 
xnee_xinput_add_devices(xnee_data *xd)
{
  int xinput_ev_base;
#define XI_BUF_SIZE 100
  char buf[XI_BUF_SIZE+1];

  if ( xd->xi_data.xinput_event_base == 0 )
    {
      return XNEE_OK;
    }

  xnee_init_xinput_devices(xd);

  xinput_ev_base = xnee_get_xinput_event_base(NULL);

  if ( xinput_ev_base == -1 )
    {
      fprintf(xd->err_file, "Failed to initialize xinput implementation\n");
      fprintf(xd->err_file, "Will not use XInput event\n");
    }

  if ( xnee_xinput_keyboard_requested(xd) && 
       xnee_xinput_mouse_requested(xd) )
    {
      snprintf(buf, XI_BUF_SIZE, "%d-%d", 
	      xinput_ev_base,
	      xinput_ev_base+5);
    }
  else if ( xnee_xinput_mouse_requested(xd))
    {
      snprintf(buf, XI_BUF_SIZE, "%d,%d-%d", 
	      xinput_ev_base,
	      xinput_ev_base+3,
	      xinput_ev_base+5);
    }
  else if ( xnee_xinput_keyboard_requested(xd))
    {
      snprintf(buf, XI_BUF_SIZE, "%d-%d", 
	      xinput_ev_base+1,
	      xinput_ev_base+2);
    }

  xnee_parse_range (xd, 
		    XNEE_DEVICE_EVENT, 
		    buf);

  return XNEE_OK;
}
	  


#else /*  XNEE_XINPUT_SUPPORT */

int
xnee_init_xinput(xnee_data *xd)
{
  return XNEE_OK;
}


int
xnee_init_xinput_devices(xnee_data *xd)
{
  return XNEE_OK;
}

int
xnee_get_xinput_event_base(Display *dpy)
{
  return XNEE_OK;
}


int
xnee_has_xinput(xnee_data *xd)
{
  return -1;
}


int 
xnee_handle_xinput_event(xnee_data * xd, 
			 int event_type, 
			 XRecordDatum *xrec_data,
			 Time server_time)
{
  return XNEE_OK;
}


int 
xnee_xinput_add_devices(xnee_data *xd)
{
  return XNEE_OK;
}
	  
int
xnee_handle_xinput_event_human(xnee_data * xd, 
			       int event_type, 
			       XRecordDatum *xrec_data,
			       Time server_time)
{
  return XNEE_OK;
}

#endif /*  XNEE_XINPUT_SUPPORT */

