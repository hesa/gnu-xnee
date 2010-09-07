/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *   Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 
 *                 2006, 2007, 2008, 2009, 2010
 *                        Henrik Sandklef 
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
#include "libxnee/xnee_xinput.h"

#ifdef XNEE_XINPUT_SUPPORT


#define XNEE_XINPUT_PRINT_MASTER_OR_SLAVE(xd, devid, fd)	\
  if ( ! xd->xi_data.xi_devices[devid].is_slave)		\
    fprintf (fd, "%d", XNEE_PROTO_XINPUT_EVENT_MASTER);		\
  else								\
    fprintf (fd, "%d", XNEE_PROTO_XINPUT_EVENT_SLAVE);



int
xnee_init_xinput_devices(xnee_data *xd)
{

  int major = XI_2_Major;
  int minor = XI_2_Minor;
  int nr_of_devices;

  int i;
  XIDeviceInfo *xi_info, *dev;

  if ( (xd==NULL) || (xd->control==NULL) )
    {
      return -1;
    }

  if (XIQueryVersion(xd->control, &major, &minor) != Success ||
      (major * 1000 + minor) < (XI_2_Major * 1000 + XI_2_Minor))
    {
      fprintf(stderr, "XI2 not supported.\n");
      return EXIT_FAILURE;
    }

  xi_info = XIQueryDevice(xd->control, XIAllDevices, &nr_of_devices);
      
  for(i = 0; i < nr_of_devices; i++)
    {
      dev = &xi_info[i];
	
      xd->xi_data.xi_devices[dev->deviceid].name     = strdup(dev->name);
      xd->xi_data.xi_devices[dev->deviceid].deviceid = dev->deviceid;

      if (dev->use == XIMasterPointer || dev->use == XIMasterKeyboard)
	{
	  xd->xi_data.xi_devices[dev->deviceid].is_slave = 0;
	}
      else
	{
	  xd->xi_data.xi_devices[dev->deviceid].is_slave = 1;
	}
    }

  XIFreeDeviceInfo(xi_info);
  
  return 0;
}

int
xnee_get_xinput_event_base(Display *dpy)
{
  int ret;
  int ext_op;
  int ext_fi_ev;
  int ext_fi_er;
  
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
			 int event_type, 
			 XRecordDatum *xrec_data)
{
  static saved_xinput_event sxe;
  FILE *out ;
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
      
      /* printf ("GOT %d | detail: %d  saved=%d\n", ordinary_event_nr, xrec_data->event.u.u.detail, sxe.type); */
      
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
	  fprintf (out, ",%u,0,0,%d,0,0,%lu,%d,%s\n",
		   ordinary_event_nr,
		   e->detail ,
		   sxe.time,
		   e->deviceid,
		   xd->xi_data.xi_devices[e->deviceid].name);
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
	  if ( e->num_valuators != 2 )
	    {
	      printf ("Number of valuators was faulty :(\n");
	      /*
	       * HESA HESA HESA
	       * exit is good when developing
	       * since it forces an exit on error ..... 
	       *  FIX LATER
	       */
	      exit(1);
	    }
	  
	  XNEE_XINPUT_PRINT_MASTER_OR_SLAVE(xd, e->deviceid, out);
	  fprintf (out, ",%d,%d,%d,0,0,0,%lu,%d,'%s'\n",
		   sxe.type,
		   e->valuator0, 
		   e->valuator1, 
		   sxe.time,
		   e->deviceid,
		   xd->xi_data.xi_devices[e->deviceid].name);
	  
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
  int ret ; 
  char buf[10];

  if ( xd->xi_data.xinput_event_base == XNEE_XINPUT_DISABLED )
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

  if ( xnee_xinput_keyboard_requested(xd) || 
       xnee_xinput_mouse_requested(xd) )
    {
      sprintf(buf, "%d-%d", 
	      xinput_ev_base,
	      xinput_ev_base+5);
    }
  else if ( xnee_xinput_mouse_requested(xd))
    {
      sprintf(buf, "%d,%d-%d", 
	      xinput_ev_base,
	      xinput_ev_base+3,
	      xinput_ev_base+5);
    }
  else if ( xnee_xinput_keyboard_requested(xd))
    {
      sprintf(buf, "%d-%d", 
	      xinput_ev_base,
	      xinput_ev_base+2);
    }

  ret=xnee_parse_range (xd, 
			XNEE_DEVICE_EVENT, 
			buf);
  

  return XNEE_OK;
}
	  

int 
xnee_activate_xinput(xnee_data *xd);


#endif /*  XNEE_XINPUT_SUPPORT */

