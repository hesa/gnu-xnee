/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *  Copyright (C) 1999, 2000, 2001, 2002, 2003, 
 *                2004, 2008, 2009, 2010, 2011  Henrik Sandklef 
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


#include <stdio.h>
#include <string.h>

#include "libxnee/xnee.h"
#include "libxnee/xnee_error.h"
#include "libxnee/xnee_display.h"
#include "libxnee/print_varargs.h"
#include "libxnee/xnee_resolution.h"
#include "libxnee/xnee_display.h"
#include "libxnee/xnee_xinput.h"

/**************************************************************
 *                                                            *
 * xnee_setup_display                                         *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_setup_display (xnee_data *xd)
{
  int ret;

  xnee_verbose((xd, "-- xnee_setup_display - data display\n"));
  if (xd->data!=NULL)
    {
      /*
      xnee_verbose((xd, "-- xnee_setup_display - close data display\n"));
      XFlush(xd->data);
      XCloseDisplay(xd->data);*/
    }
  else 
    {
      xnee_verbose((xd, "-- xnee_setup_display - open data display\n"));
      xd->data     = xnee_open_display (xd);
    }
  if (xd->data==NULL)
    {
      return XNEE_NOT_OPEN_DISPLAY;
    }
  

  
  xnee_verbose((xd, "-- xnee_setup_display - control display\n"));
  if (xd->control!=NULL)
    {
      XCloseDisplay(xd->control);
    }
  xd->control  = xnee_open_display (xd);
  if (xd->control==NULL)
    {
      return XNEE_NOT_OPEN_DISPLAY;
    }
  
  

  xnee_verbose((xd, "-- xnee_setup_display - fake display\n"));
  if (xd->fake!=NULL)
    {
      XCloseDisplay(xd->fake);
    }
  xd->fake     = xnee_open_display (xd);
  if (xd->fake==NULL)
    {
      return XNEE_NOT_OPEN_DISPLAY;
    }

  xnee_verbose((xd, "-- xnee_setup_display - modifier map\n"));

  xnee_verbose((xd, "Freeing modifier mapping memory %p ", (void*)xd->map));
  XFreeModifiermap(xd->map);

  xnee_verbose((xd," building modifier map on %p\n", (void*)xd->fake)); 
  xd->map = XGetModifierMapping(xd->fake);


  xnee_verbose((xd, "display data    %p\n" , (void*) xd->data));
  xnee_verbose((xd, "display control %p\n" , (void*) xd->control));
  xnee_verbose((xd, "display fake    %p\n" , (void*) xd->fake));

  xnee_verbose((xd, "-- xnee_setup_display - resolution\n"));
  /* 
   * resolution */
  ret = xnee_set_default_rec_resolution (xd);

  if ( ret != XNEE_OK )
    {
      return XNEE_NOT_OPEN_DISPLAY;
    }


#ifdef  XNEE_XINPUT_SUPPORT 
  ret = xnee_get_xinput_event_base(xd->control);
  if ( ret > 0 )
    {
      xd->xi_data.xinput_event_base = ret;
      
      ret = xnee_init_xinput_devices(xd);
      if (ret != XNEE_OK )
	{
	  fprintf(stderr, "Failed finding X Input extension devices\n");
	  xd->xi_data.xinput_event_base = -1;
	}
    }
#endif /* XNEE_XINPUT_SUPPORT */


  /* return XNEE_OK on success, 1  indicats error opening the displays */
  if ( ( xd->data != NULL ) && (xd->control != NULL) && (xd->fake != NULL) )
    {
      xnee_verbose((xd, "-- xnee_setup_display - return OK\n"));
      return XNEE_OK;
    }
  else 
    {
      xnee_verbose((xd, "-- xnee_setup_display - return 1 ... which is bad\n"));
      return 1;
    }




}






/**************************************************************
 *                                                            *
 * xnee_set_default_display                                   *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_set_default_display (xnee_data *xd)
{
  char *tmp;
  tmp = getenv ((const char*)"DISPLAY");
  if ( tmp != NULL )
    {
      xd->display = strdup(tmp);
    }
  else
    {
      xd->display = NULL;
    }
  return ;
}



/**************************************************************
 *                                                            *
 * xnee_open_display                                          *
 *                                                            *
 *                                                            *
 **************************************************************/
Display * 
xnee_open_display(xnee_data* xd)
{
  Display *dpy ;

/*@null@*/
  const char *tmp;

  tmp = (const char*) ((xd->display!=NULL)?
		       (xd->display):
		       NULL_STRING);
  
  if ( (tmp!=NULL) && (strlen(tmp)==0) )
    {
      tmp=NULL;
    }
  
  xnee_verbose((xd,  "Open display %s \n", 
		tmp?tmp:"" ));
  
  dpy = XOpenDisplay (tmp);
  xnee_verbose((xd,  "Open display %s \n", 
		tmp?tmp:"" ));
  
  if (!dpy) 
    {
      (void) xnee_print_error ("%s: unable to open display \"%s\"\n",
			       PACKAGE, 
			       XDisplayName(tmp));
      return dpy;
    }
  
  xnee_verbose((xd, "Display %s = %p\n", 
		tmp?tmp:"", (void*) dpy));
  return dpy;
}





/**************************************************************
 *                                                            *
 * xnee_add_display_list                                      *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_add_display_list ( xnee_data* xd, char * disp)
{
  char buf[256];
  size_t next;
  size_t len;
  int ret=0;
  int disp_len;


  if (disp==NULL)
    {
      return XNEE_WRONG_PARAMS;
    }

  disp_len = strlen(disp);


  xnee_verbose((xd, "xnee_add_display (xd, %s, )\n", disp));

  while ( (1!=0) ) {

    next=strcspn (disp, ",");
    if (next==0) break; 
    strncpy(buf,disp,next);
    buf[next]='\0';
    len =strlen(disp);
    disp+=next+1;
    disp_len=disp_len - next - 1;
    ret=xnee_add_display_str (buf,xd);
    xnee_verbose((xd,  " ------------------------- are we doing fine??? ret=%d\n", ret));

    if (ret!=0) 
      {
	xnee_verbose((xd, "Could not add all display for distribution ... returning\n"));
	return (ret);
      }

    if (disp_len <= 0) 
      {
	xnee_verbose((xd, "disp_len = %d\n", disp_len));
	break;
      }

  }
  xnee_verbose((xd, "xnee_add_display_str ... finished\n"));
  return XNEE_OK;  
}





/**************************************************************
 *                                                            *
 * xnee_add_display_str                                       *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_add_display_str (char * disp_str, xnee_data* xd)
{
  Display *dpy;
  int xtest_event_basep = 0;
  int xtest_error_basep = 0;
  int xtest_version_major = 0;
  int xtest_version_minor = 0;
  
  xnee_verbose((xd, "Adding Display \"%s\" to distribution list\n", disp_str));
  
  if (xd->distr_list_size==0) 
    {
      xnee_verbose((xd, 
		    "Adding Display - allocating memory for the first time\n"
		    ));
      xd->distr_list = (xnee_distr *) calloc (1,sizeof (xnee_distr));
    }
  else 
    {
      xnee_distr *tmp = xd->distr_list;
      xd->distr_list = (xnee_distr *) realloc (xd->distr_list, 
					       xd->distr_list_size);
      if (xd->distr_list==NULL)
	{
	  /*@ignore@*/
	  xd->distr_list = tmp;
	  /*@end@*/
	  tmp = NULL;
	  return XNEE_MEMORY_FAULT;
	}
    }
  
  
  xnee_verbose((xd, "Adding Display  - opening display\n"));
  dpy = XOpenDisplay (disp_str);
  if (dpy==NULL)
    {
      xnee_print_error ("Could not open display %s\n", disp_str);
      return ( XNEE_NOT_OPEN_DISPLAY );
    }
  xnee_verbose((xd, "Adding Display  - opening display gave us %p\n", (void*) dpy));
  
  xnee_verbose((xd, "Adding Display  - trying to grab control\n"));
  /*@ignore@*/
  XTestGrabControl (dpy, True);
  /*@end@*/

  if (!dpy) 
    {
      (void) xnee_print_error ("Unable to open display \"%s\"\n",
			       disp_str);
      return (XNEE_NOT_OPEN_DISPLAY);
    }
  xnee_verbose((xd, "Adding Display  - does the display/host have XTest\n"));
  if( XTestQueryExtension(dpy,
			  &xtest_event_basep,
			  &xtest_error_basep,
			  &xtest_version_major,
			  &xtest_version_minor) == 0)
    {
      xnee_print_error ("XTest extension missing on display %s \n", disp_str);
      return (XNEE_NO_TEST_EXT);
    }
  xnee_verbose ((xd, "  XTest  Release on \"%s\"         %d.%d\n", 
		disp_str,
		xtest_version_major,
		xtest_version_minor));
  
  xnee_verbose ((xd, " -- resolution to 0\n"));

  if  ( (xd!=NULL) && 
	(xd->distr_list != NULL ) )
    {
      xd->distr_list[xd->distr_list_size].is_used=0;
      
      xd->distr_list[xd->distr_list_size].res.x_res=DisplayWidth  (dpy, 0);
      xd->distr_list[xd->distr_list_size].res.y_res=DisplayHeight (dpy, 0);
      xd->distr_list[xd->distr_list_size].dpy=dpy;
      
      
      xnee_verbose ((xd, "cheking if resolution differs\n"));
      if (xnee_res_cmp(&xd->distr_list[xd->distr_list_size].res,
		       &xd->res_info.record)==0)
	{
	  xnee_verbose ((xd, " -- resolution differs\n"));
	  xd->distr_list[xd->distr_list_size].is_used=1;
	}
      xd->distr_list_size++;
    }
  else
    {
      return (XNEE_MEMORY_FAULT);
    }

  return (XNEE_OK);
}




/**************************************************************
 *                                                            *
 * xnee_add_display                                           *
 *                                                            *
 *                                                            *
 **************************************************************/
int 
xnee_add_display (Display *dpy, xnee_data* xd)
{
  int xtest_event_basep   = 0;
  int xtest_error_basep   = 0;
  int xtest_version_major = 0;
  int xtest_version_minor = 0;

  xnee_verbose((xd, "Adding Display \"%p\" to distribution list\n", (void*) dpy));
  if (xd->distr_list_size==0) 
    {
      xd->distr_list = (xnee_distr *) calloc (1,sizeof (Display));
    }
  else 
    {
      xnee_distr *tmp = xd->distr_list;
      xd->distr_list = (xnee_distr *) realloc (xd->distr_list, 
					       xd->distr_list_size);
      if (xd->distr_list==NULL)
	{
	  /*@ignore@*/
	  xd->distr_list = tmp;
	  /*@end@*/
	  tmp = NULL;
	  return XNEE_MEMORY_FAULT;
	}
    }
  /*@ignore@*/
  XTestGrabControl (dpy, True);
  /*@end@*/

  if( XTestQueryExtension(dpy,
			  &xtest_event_basep,
			  &xtest_error_basep,
			  &xtest_version_major,
			  &xtest_version_minor) == 0)
    {
       xnee_print_error ("XTest extension missing on display %p \n", (void*)dpy);
       return (XNEE_NO_TEST_EXT);
    }
  xnee_verbose ((xd, "  XTest  Release on \"%p\"         %d.%d\n", 
		 (void*)dpy,
		 xtest_version_major,
		 xtest_version_minor));
  
  if ( (xd != NULL) && (xd->distr_list != NULL) )
    {
      xd->distr_list[xd->distr_list_size].dpy=dpy;
      xd->distr_list_size++;
    }
  else
    {
      return (XNEE_MEMORY_FAULT);
    }
  return (XNEE_OK);
}

