/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003 Henrik Sandklef                    
 *                                                                   
 * This program is free software; you can redistribute it and/or     
 * modify it under the terms of the GNU General Public License       
 * as published by the Free Software Foundation; either version 2    
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston,            
 * MA  02111-1307, USA.                                              
 ****/


#include <stdio.h>

#include <X11/X.h>
#include <X11/Xproto.h>

#include <libxnee/datastrings.h>
#include "libxnee/xnee.h"




typedef char * DATA_STRING ;

static  DATA_STRING event_names[38];
static  DATA_STRING request_names[140];
static  DATA_STRING error_names[18];


/*********  INTERNAL USE ***************/
int
xnee_add_to_list(DATA_STRING*, int pos, char *str);

void 
xnee_init_event_names();

void 
xnee_init_request_names();

void 
xnee_init_error_names();

void 
xnee_init_names();
/*********  END OF INTERNAL USE ***************/


int
xnee_event2int(char *ev)
{
  int i ; 
  for (i=2;event_names[i]!=NULL;i++)
    {
      if (strncmp(event_names[i], ev, strlen(event_names[i]))==0)
	{
	  return i;
	}
    }
  return -1;
}

int
xnee_request2int(char *req)
{
  int i ; 

  for (i=1;i<=X_NoOperation;i++)
    {
      if (request_names[i]==NULL)
	{
	  ;
	}
      else
	{
	  int len = XNEE_MAX (strlen(request_names[i]), strlen(req));
	  if (strncmp(request_names[i], req, len)==0)
	    {
	      return i;
	    }
	}
    }
  return -1;
}


int
xnee_error2int(char *ev)
{
  int i ; 

  for (i=2;error_names[i]!=NULL;i++)
    {
      if (strncmp(error_names[i], ev, strlen(error_names[i]))==0)
	{
	  return i;
	}
    }
  return -1;
}





/*
 * 
 * Prints the name of the XEvent that corresponds to the argument ev.  
 * Printout is made on stdout.  
 *
 */
char *
xnee_print_event (int ev) 
{
  return event_names[ev];
}



/*
 * 
 * Prints the name of the error that corresponds to the argument ev.  
 * Printout is made on stdout.  
 *
 */
char * 
xnee_print_error_code ( int err)
{
  if ( (err >= FirstExtensionError)  && ( err <= LastExtensionError ))
    {
      return " FirstExtensionError =< error >= LastExtensionError";
    }
  else
    {
      return error_names[err];
    }
}



/*
 * 
 * Prints the name of the error that corresponds to the argument ev.  
 * Printout is made on stdout.  
 *
 */
char * 
xnee_print_request ( int err)
{
  return request_names[err];
}


/*
 * 
 * Prints the name of the request that corresponds to the argument req.  
 * Printout is made on stdout.  
 *
 */
char *
xnee_int2request(int req)
{
  if ( (req <= 0) || (req>X_NoOperation) )
    {
      return NULL;
    }
  else 
    {
      return request_names[req];
    }
}






/*
 * 
 * Prints the name of the event that corresponds to the argument ev.  
 * Printout is made on stdout.  
 *
 */
char *
xnee_int2event(int ev)
{
  if ( (ev <= 1) || (ev>LASTEvent) )
    {
      return NULL;
    }
  else 
    {
      return event_names[ev];
    }
}



/*
 * 
 * Prints the name of the error that corresponds to the argument err.  
 * Printout is made on stdout.  
 *
 */
char *
xnee_int2error(int err)
{
  return error_names[err];
}





/**
 * Get the integer value from X11 data dat
 */
int
xnee_data2int(int type, char *dat)
{
  if (type==XNEE_DEVICE_EVENT)
      return xnee_event2int(dat);
  else  if (type==XNEE_DELIVERED_EVENT)
      return xnee_event2int(dat);
  else if (type==XNEE_REQUEST)
    return xnee_request2int(dat);
  else if (type==XNEE_ERROR)
    return xnee_error2int(dat);
  else if (type==XNEE_REPLY)
    return 0;
  else 
    return 0;
}







int
xnee_add_to_list(char **arr, int pos, char *str)
{
  if (str!=NULL)
    {
      /*      arr[pos]=(char*) calloc(strlen(str)+2, sizeof(char));*/
      arr[pos]=strdup(str);
    }
  else
    {
      arr[pos]=NULL;
    }
}


void 
xnee_init_event_names()
{
  xnee_add_to_list(event_names, 0, "0");
  xnee_add_to_list(event_names, 1, "1");
  xnee_add_to_list(event_names,KeyPress,"KeyPress"); 
  xnee_add_to_list(event_names,KeyRelease,"KeyRelease"); 
  xnee_add_to_list(event_names,ButtonPress,"ButtonPress"); 
  xnee_add_to_list(event_names,ButtonRelease,"ButtonRelease"); 
  xnee_add_to_list(event_names,MotionNotify,"MotionNotify"); 
  xnee_add_to_list(event_names,EnterNotify,"EnterNotify"); 
  xnee_add_to_list(event_names,LeaveNotify,"LeaveNotify"); 
  xnee_add_to_list(event_names,FocusIn,"FocusIn"); 
  xnee_add_to_list(event_names,FocusOut,"FocusOut"); 
  xnee_add_to_list(event_names,KeymapNotify,"KeymapNotify"); 
  xnee_add_to_list(event_names,Expose,"Expose"); 
  xnee_add_to_list(event_names,GraphicsExpose,"GraphicsExpose"); 
  xnee_add_to_list(event_names,NoExpose,"NoExpose"); 
  xnee_add_to_list(event_names,VisibilityNotify,"VisibilityNotify"); 
  xnee_add_to_list(event_names,CreateNotify,"CreateNotify"); 
  xnee_add_to_list(event_names,DestroyNotify,"DestroyNotify"); 
  xnee_add_to_list(event_names,UnmapNotify,"UnmapNotify"); 
  xnee_add_to_list(event_names,MapNotify,"MapNotify"); 
  xnee_add_to_list(event_names,MapRequest,"MapRequest"); 
  xnee_add_to_list(event_names,ReparentNotify,"ReparentNotify"); 
  xnee_add_to_list(event_names,ConfigureNotify,"ConfigureNotify"); 
  xnee_add_to_list(event_names,ConfigureRequest,"ConfigureRequest"); 
  xnee_add_to_list(event_names,GravityNotify,"GravityNotify"); 
  xnee_add_to_list(event_names,ResizeRequest,"ResizeRequest"); 
  xnee_add_to_list(event_names,CirculateNotify,"CirculateNotify"); 
  xnee_add_to_list(event_names,CirculateRequest,"CirculateRequest"); 
  xnee_add_to_list(event_names,PropertyNotify,"PropertyNotify"); 
  xnee_add_to_list(event_names,SelectionClear,"SelectionClear"); 
  xnee_add_to_list(event_names,SelectionRequest,"SelectionRequest"); 
  xnee_add_to_list(event_names,SelectionNotify,"SelectionNotify"); 
  xnee_add_to_list(event_names,ColormapNotify,"ColormapNotify"); 
  xnee_add_to_list(event_names,ClientMessage,"ClientMessage"); 
  xnee_add_to_list(event_names,MappingNotify,"MappingNotify"); 
  xnee_add_to_list(event_names,LASTEvent,"LASTEvent"); 
}

void 
xnee_init_request_names()
{
  xnee_add_to_list(request_names,X_CreateWindow,"X_CreateWindow");
  xnee_add_to_list(request_names,X_ChangeWindowAttributes,"X_ChangeWindowAttributes");
  xnee_add_to_list(request_names,X_GetWindowAttributes,"X_GetWindowAttributes");
  xnee_add_to_list(request_names,X_DestroyWindow,"X_DestroyWindow");
  xnee_add_to_list(request_names,X_DestroySubwindows,"X_DestroySubwindows");
  xnee_add_to_list(request_names,X_ChangeSaveSet,"X_ChangeSaveSet");
  xnee_add_to_list(request_names,X_ReparentWindow,"X_ReparentWindow");
  xnee_add_to_list(request_names,X_MapWindow,"X_MapWindow");
  xnee_add_to_list(request_names,X_MapSubwindows,"X_MapSubwindows");
  xnee_add_to_list(request_names,X_UnmapWindow,"X_UnmapWindow");
  xnee_add_to_list(request_names,X_UnmapSubwindows,"X_UnmapSubwindows");
  xnee_add_to_list(request_names,X_ConfigureWindow,"X_ConfigureWindow");
  xnee_add_to_list(request_names,X_CirculateWindow,"X_CirculateWindow");
  xnee_add_to_list(request_names,X_GetGeometry,"X_GetGeometry");
  xnee_add_to_list(request_names,X_QueryTree,"X_QueryTree");
  xnee_add_to_list(request_names,X_InternAtom,"X_InternAtom");
  xnee_add_to_list(request_names,X_GetAtomName,"X_GetAtomName");
  xnee_add_to_list(request_names,X_ChangeProperty,"X_ChangeProperty");
  xnee_add_to_list(request_names,X_DeleteProperty,"X_DeleteProperty");
  xnee_add_to_list(request_names,X_GetProperty,"X_GetProperty");
  xnee_add_to_list(request_names,X_ListProperties,"X_ListProperties");
  xnee_add_to_list(request_names,X_SetSelectionOwner,"X_SetSelectionOwner");
  xnee_add_to_list(request_names,X_GetSelectionOwner,"X_GetSelectionOwner");
  xnee_add_to_list(request_names,X_ConvertSelection,"X_ConvertSelection");
  xnee_add_to_list(request_names,X_SendEvent,"X_SendEvent");
  xnee_add_to_list(request_names,X_GrabPointer,"X_GrabPointer");
  xnee_add_to_list(request_names,X_UngrabPointer,"X_UngrabPointer");
  xnee_add_to_list(request_names,X_GrabButton,"X_GrabButton");
  xnee_add_to_list(request_names,X_UngrabButton,"X_UngrabButton");
  xnee_add_to_list(request_names,X_ChangeActivePointerGrab,"X_ChangeActivePointerGrab");
  xnee_add_to_list(request_names,X_GrabKeyboard,"X_GrabKeyboard");
  xnee_add_to_list(request_names,X_UngrabKeyboard,"X_UngrabKeyboard");
  xnee_add_to_list(request_names,X_GrabKey,"X_GrabKey");
  xnee_add_to_list(request_names,X_UngrabKey,"X_UngrabKey");
  xnee_add_to_list(request_names,X_AllowEvents,"X_AllowEvents");
  xnee_add_to_list(request_names,X_GrabServer,"X_GrabServer");
  xnee_add_to_list(request_names,X_UngrabServer,"X_UngrabServer");
  xnee_add_to_list(request_names,X_QueryPointer,"X_QueryPointer");
  xnee_add_to_list(request_names,X_GetMotionEvents,"X_GetMotionEvents");
  xnee_add_to_list(request_names,X_TranslateCoords,"X_TranslateCoords");
  xnee_add_to_list(request_names,X_WarpPointer,"X_WarpPointer");
  xnee_add_to_list(request_names,X_SetInputFocus,"X_SetInputFocus");
  xnee_add_to_list(request_names,X_GetInputFocus,"X_GetInputFocus");
  xnee_add_to_list(request_names,X_QueryKeymap,"X_QueryKeymap");
  xnee_add_to_list(request_names,X_OpenFont,"X_OpenFont");
  xnee_add_to_list(request_names,X_CloseFont,"X_CloseFont");
  xnee_add_to_list(request_names,X_QueryFont,"X_QueryFont");
  xnee_add_to_list(request_names,X_QueryTextExtents,"X_QueryTextExtents");
  xnee_add_to_list(request_names,X_ListFonts,"X_ListFonts");
  xnee_add_to_list(request_names,X_ListFontsWithInfo	,"X_ListFontsWithInfo");
  xnee_add_to_list(request_names,X_SetFontPath,"X_SetFontPath");
  xnee_add_to_list(request_names,X_GetFontPath,"X_GetFontPath");
  xnee_add_to_list(request_names,X_CreatePixmap,"X_CreatePixmap");
  xnee_add_to_list(request_names,X_FreePixmap,"X_FreePixmap");
  xnee_add_to_list(request_names,X_CreateGC,"X_CreateGC");
  xnee_add_to_list(request_names,X_ChangeGC,"X_ChangeGC");
  xnee_add_to_list(request_names,X_CopyGC,"X_CopyGC");
  xnee_add_to_list(request_names,X_SetDashes,"X_SetDashes");
  xnee_add_to_list(request_names,X_SetClipRectangles,"X_SetClipRectangles");
  xnee_add_to_list(request_names,X_FreeGC,"X_FreeGC");
  xnee_add_to_list(request_names,X_ClearArea,"X_ClearArea");
  xnee_add_to_list(request_names,X_CopyArea,"X_CopyArea");
  xnee_add_to_list(request_names,X_CopyPlane,"X_CopyPlane");
  xnee_add_to_list(request_names,X_PolyPoint,"X_PolyPoint");
  xnee_add_to_list(request_names,X_PolyLine,"X_PolyLine");
  xnee_add_to_list(request_names,X_PolySegment,"X_PolySegment");
  xnee_add_to_list(request_names,X_PolyRectangle,"X_PolyRectangle");
  xnee_add_to_list(request_names,X_PolyArc,"X_PolyArc");
  xnee_add_to_list(request_names,X_FillPoly,"X_FillPoly");
  xnee_add_to_list(request_names,X_PolyFillRectangle,"X_PolyFillRectangle");
  xnee_add_to_list(request_names,X_PolyFillArc,"X_PolyFillArc");
  xnee_add_to_list(request_names,X_PutImage,"X_PutImage");
  xnee_add_to_list(request_names,X_GetImage,"X_GetImage");
  xnee_add_to_list(request_names,X_PolyText8,"X_PolyText8");
  xnee_add_to_list(request_names,X_PolyText16,"X_PolyText16");
  xnee_add_to_list(request_names,X_ImageText8,"X_ImageText8");
  xnee_add_to_list(request_names,X_ImageText16,"X_ImageText16");
  xnee_add_to_list(request_names,X_CreateColormap,"X_CreateColormap");
  xnee_add_to_list(request_names,X_FreeColormap,"X_FreeColormap");
  xnee_add_to_list(request_names,X_CopyColormapAndFree,"X_CopyColormapAndFree");
  xnee_add_to_list(request_names,X_InstallColormap,"X_InstallColormap");
  xnee_add_to_list(request_names,X_UninstallColormap,"X_UninstallColormap");
  xnee_add_to_list(request_names,X_ListInstalledColormaps,"X_ListInstalledColormaps");
  xnee_add_to_list(request_names,X_AllocColor,"X_AllocColor");
  xnee_add_to_list(request_names,X_AllocNamedColor,"X_AllocNamedColor");
  xnee_add_to_list(request_names,X_AllocColorCells,"X_AllocColorCells");
  xnee_add_to_list(request_names,X_AllocColorPlanes,"X_AllocColorPlanes");
  xnee_add_to_list(request_names,X_FreeColors,"X_FreeColors");
  xnee_add_to_list(request_names,X_StoreColors,"X_StoreColors");
  xnee_add_to_list(request_names,X_StoreNamedColor,"X_StoreNamedColor");
  xnee_add_to_list(request_names,X_QueryColors,"X_QueryColors");
  xnee_add_to_list(request_names,X_LookupColor,"X_LookupColor");
  xnee_add_to_list(request_names,X_CreateCursor,"X_CreateCursor");
  xnee_add_to_list(request_names,X_CreateGlyphCursor,"X_CreateGlyphCursor");
  xnee_add_to_list(request_names,X_FreeCursor,"X_FreeCursor");
  xnee_add_to_list(request_names,X_RecolorCursor,"X_RecolorCursor");
  xnee_add_to_list(request_names,X_QueryBestSize,"X_QueryBestSize");
  xnee_add_to_list(request_names,X_QueryExtension,"X_QueryExtension");
  xnee_add_to_list(request_names,X_ListExtensions,"X_ListExtensions");
  xnee_add_to_list(request_names,X_ChangeKeyboardMapping,"X_ChangeKeyboardMapping");
  xnee_add_to_list(request_names,X_GetKeyboardMapping,"X_GetKeyboardMapping");
  xnee_add_to_list(request_names,X_ChangeKeyboardControl,"X_ChangeKeyboardControl");
  xnee_add_to_list(request_names,X_GetKeyboardControl,"X_GetKeyboardControl");
  xnee_add_to_list(request_names,X_Bell,"X_Bell");
  xnee_add_to_list(request_names,X_ChangePointerControl,"X_ChangePointerControl");
  xnee_add_to_list(request_names,X_GetPointerControl,"X_GetPointerControl");
  xnee_add_to_list(request_names,X_SetScreenSaver,"X_SetScreenSaver");
  xnee_add_to_list(request_names,X_GetScreenSaver,"X_GetScreenSaver");
  xnee_add_to_list(request_names,X_ChangeHosts,"X_ChangeHosts");
  xnee_add_to_list(request_names,X_ListHosts,"X_ListHosts");
  xnee_add_to_list(request_names,X_SetAccessControl,"X_SetAccessControl");
  xnee_add_to_list(request_names,X_SetCloseDownMode,"X_SetCloseDownMode");
  xnee_add_to_list(request_names,X_KillClient,"X_KillClient");
  xnee_add_to_list(request_names,X_RotateProperties	,"X_RotateProperties");
  xnee_add_to_list(request_names,X_ForceScreenSaver	,"X_ForceScreenSaver");
  xnee_add_to_list(request_names,X_SetPointerMapping,"X_SetPointerMapping");
  xnee_add_to_list(request_names,X_GetPointerMapping,"X_GetPointerMapping");
  xnee_add_to_list(request_names,X_SetModifierMapping	,"X_SetModifierMapping");
  xnee_add_to_list(request_names,X_GetModifierMapping	,"X_GetModifierMapping");
  xnee_add_to_list(request_names,X_NoOperation,"X_NoOperation");
  xnee_add_to_list(request_names,X_NoOperation+1,NULL); 
}

void 
xnee_init_error_names()
{
  xnee_add_to_list(error_names,Success,"Success"); 
  xnee_add_to_list(error_names,BadRequest,"BadRequest"); 
  xnee_add_to_list(error_names,BadValue,"BadValue"); 
  xnee_add_to_list(error_names,BadWindow,"BadWindow"); 
  xnee_add_to_list(error_names,BadPixmap,"BadPixmap"); 
  xnee_add_to_list(error_names,BadAtom,"BadAtom"); 
  xnee_add_to_list(error_names,BadCursor,"BadCursor"); 
  xnee_add_to_list(error_names,BadFont,"BadFont"); 
  xnee_add_to_list(error_names,BadMatch,"BadMatch"); 
  xnee_add_to_list(error_names,BadDrawable,"BadDrawable"); 
  xnee_add_to_list(error_names,BadAccess,"BadAccess"); 
  xnee_add_to_list(error_names,BadAlloc,"BadAlloc"); 
  xnee_add_to_list(error_names,BadColor,"BadColor"); 
  xnee_add_to_list(error_names,BadGC,"BadGC"); 
  xnee_add_to_list(error_names,BadIDChoice,"BadIDChoice"); 
  xnee_add_to_list(error_names,BadName,"BadName"); 
  xnee_add_to_list(error_names,BadLength,"BadLength"); 
  xnee_add_to_list(error_names,BadImplementation,"BadImplementation"); 
  xnee_add_to_list(error_names,BadImplementation+1,NULL); 
}

void 
xnee_init_names()
{
  xnee_init_event_names();
  xnee_init_request_names();
  xnee_init_error_names();
}



#ifdef DEBUG_NAMING_STUFF
int main ()
{
  int nr;
  char tmp[32];
  xnee_init_names();


  nr = xnee_event2int("MappingNotify");
  fprintf (stdout, "MappingNotify=%d \n", nr);

  strcpy(tmp,xnee_int2event(nr));
  fprintf (stdout, "MappingNotify=%s \n", tmp);


  /*****************************************/
  for (nr=14;nr<20;nr++)
    {
      fprintf (stdout,"%.2d\t%s\n",nr,xnee_print_request(nr));
    }


  /*****************************************/

  nr = xnee_request2int("X_AllowEvents");
  fprintf (stdout, "X_AllowEvents=%d \n", nr);

  strcpy(tmp,xnee_int2event(nr));
  fprintf (stdout, "X_AllowEvents=%s \n", tmp);

}
#endif /* DEBUG_NAMING_STUFF */
