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
 * Foundation, Inc., 51 Franklin Street, Boston,            
 * MA  02110-1301, USA.                                              
 ****/


#include <stdio.h>

#include <X11/X.h>
#include <X11/Xproto.h>
#include <libxnee/datastrings.h>
#include "libxnee/xnee.h"





struct data_description event_field[]=
{
   {KeyPress,"KeyPress", "Pressing a key"}, 
   {KeyRelease,"KeyRelease", "Releasing a key"}, 
   {ButtonPress,"ButtonPress", "Pressing a button"}, 
   {ButtonRelease,"ButtonRelease", "Releasing a button"}, 
   {MotionNotify,"MotionNotify", "Moving the pointer"}, 
   {EnterNotify,"EnterNotify", ""}, 
   {LeaveNotify,"LeaveNotify", ""}, 
   {FocusIn,"FocusIn", ""}, 
   {FocusOut,"FocusOut", ""}, 
   {KeymapNotify,"KeymapNotify", ""}, 
   {Expose,"Expose", ""}, 
   {GraphicsExpose,"GraphicsExpose", ""}, 
   {NoExpose,"NoExpose", ""}, 
   {VisibilityNotify,"VisibilityNotify", ""}, 
   {CreateNotify,"CreateNotify", ""}, 
   {DestroyNotify,"DestroyNotify", ""}, 
   {UnmapNotify,"UnmapNotify", ""}, 
   {MapNotify,"MapNotify", ""}, 
   {MapRequest,"MapRequest", ""}, 
   {ReparentNotify,"ReparentNotify", ""}, 
   {ConfigureNotify,"ConfigureNotify", ""}, 
   {ConfigureRequest,"ConfigureRequest", ""}, 
   {GravityNotify,"GravityNotify", ""}, 
   {ResizeRequest,"ResizeRequest", ""}, 
   {CirculateNotify,"CirculateNotify", ""}, 
   {CirculateRequest,"CirculateRequest", ""}, 
   {PropertyNotify,"PropertyNotify", ""}, 
   {SelectionClear,"SelectionClear", ""}, 
   {SelectionRequest,"SelectionRequest", ""}, 
   {SelectionNotify,"SelectionNotify", ""}, 
   {ColormapNotify,"ColormapNotify", ""}, 
   {ClientMessage,"ClientMessage", ""}, 
   {MappingNotify,"MappingNotify", ""}, 
   {LASTEvent,"LASTEvent", ""},
   {-1,NULL, ""}
} ;

struct data_description request_field[]=
{
   {X_CreateWindow,"X_CreateWindow", "Test description"},
   {X_ChangeWindowAttributes,"X_ChangeWindowAttributes", ""},
   {X_GetWindowAttributes,"X_GetWindowAttributes", ""},
   {X_DestroyWindow,"X_DestroyWindow", ""},
   {X_DestroySubwindows,"X_DestroySubwindows", ""},
   {X_ChangeSaveSet,"X_ChangeSaveSet", ""},
   {X_ReparentWindow,"X_ReparentWindow", ""},
   {X_MapWindow,"X_MapWindow", ""},
   {X_MapSubwindows,"X_MapSubwindows", ""},
   {X_UnmapWindow,"X_UnmapWindow", ""},
   {X_UnmapSubwindows,"X_UnmapSubwindows", ""},
   {X_ConfigureWindow,"X_ConfigureWindow", ""},
   {X_CirculateWindow,"X_CirculateWindow", ""},
   {X_GetGeometry,"X_GetGeometry", ""},
   {X_QueryTree,"X_QueryTree", ""},
   {X_InternAtom,"X_InternAtom", ""},
   {X_GetAtomName,"X_GetAtomName", ""},
   {X_ChangeProperty,"X_ChangeProperty", ""},
   {X_DeleteProperty,"X_DeleteProperty", ""},
   {X_GetProperty,"X_GetProperty", ""},
   {X_ListProperties,"X_ListProperties", ""},
   {X_SetSelectionOwner,"X_SetSelectionOwner", ""},
   {X_GetSelectionOwner,"X_GetSelectionOwner", ""},
   {X_ConvertSelection,"X_ConvertSelection", ""},
   {X_SendEvent,"X_SendEvent", ""},
   {X_GrabPointer,"X_GrabPointer", ""},
   {X_UngrabPointer,"X_UngrabPointer", ""},
   {X_GrabButton,"X_GrabButton", ""},
   {X_UngrabButton,"X_UngrabButton", ""},
   {X_ChangeActivePointerGrab,"X_ChangeActivePointerGrab", ""},
   {X_GrabKeyboard,"X_GrabKeyboard", ""},
   {X_UngrabKeyboard,"X_UngrabKeyboard", ""},
   {X_GrabKey,"X_GrabKey", ""},
   {X_UngrabKey,"X_UngrabKey", ""},
   {X_AllowEvents,"X_AllowEvents", ""},
   {X_GrabServer,"X_GrabServer", ""},
   {X_UngrabServer,"X_UngrabServer", ""},
   {X_QueryPointer,"X_QueryPointer", ""},
   {X_GetMotionEvents,"X_GetMotionEvents", ""},
   {X_TranslateCoords,"X_TranslateCoords", ""},
   {X_WarpPointer,"X_WarpPointer", ""},
   {X_SetInputFocus,"X_SetInputFocus", ""},
   {X_GetInputFocus,"X_GetInputFocus", ""},
   {X_QueryKeymap,"X_QueryKeymap", ""},
   {X_OpenFont,"X_OpenFont", ""},
   {X_CloseFont,"X_CloseFont", ""},
   {X_QueryFont,"X_QueryFont", ""},
   {X_QueryTextExtents,"X_QueryTextExtents", ""},
   {X_ListFonts,"X_ListFonts", ""},
   {X_ListFontsWithInfo	,"X_ListFontsWithInfo", ""},
   {X_SetFontPath,"X_SetFontPath", ""},
   {X_GetFontPath,"X_GetFontPath", ""},
   {X_CreatePixmap,"X_CreatePixmap", ""},
   {X_FreePixmap,"X_FreePixmap", ""},
   {X_CreateGC,"X_CreateGC", ""},
   {X_ChangeGC,"X_ChangeGC", ""},
   {X_CopyGC,"X_CopyGC", ""},
   {X_SetDashes,"X_SetDashes", ""},
   {X_SetClipRectangles,"X_SetClipRectangles", ""},
   {X_FreeGC,"X_FreeGC", ""},
   {X_ClearArea,"X_ClearArea", ""},
   {X_CopyArea,"X_CopyArea", ""},
   {X_CopyPlane,"X_CopyPlane", ""},
   {X_PolyPoint,"X_PolyPoint", ""},
   {X_PolyLine,"X_PolyLine", ""},
   {X_PolySegment,"X_PolySegment", ""},
   {X_PolyRectangle,"X_PolyRectangle", ""},
   {X_PolyArc,"X_PolyArc", ""},
   {X_FillPoly,"X_FillPoly", ""},
   {X_PolyFillRectangle,"X_PolyFillRectangle", ""},
   {X_PolyFillArc,"X_PolyFillArc", ""},
   {X_PutImage,"X_PutImage", ""},
   {X_GetImage,"X_GetImage", ""},
   {X_PolyText8,"X_PolyText8", ""},
   {X_PolyText16,"X_PolyText16", ""},
   {X_ImageText8,"X_ImageText8", ""},
   {X_ImageText16,"X_ImageText16", ""},
   {X_CreateColormap,"X_CreateColormap", ""},
   {X_FreeColormap,"X_FreeColormap", ""},
   {X_CopyColormapAndFree,"X_CopyColormapAndFree", ""},
   {X_InstallColormap,"X_InstallColormap", ""},
   {X_UninstallColormap,"X_UninstallColormap", ""},
   {X_ListInstalledColormaps,"X_ListInstalledColormaps", ""},
   {X_AllocColor,"X_AllocColor", ""},
   {X_AllocNamedColor,"X_AllocNamedColor", ""},
   {X_AllocColorCells,"X_AllocColorCells", ""},
   {X_AllocColorPlanes,"X_AllocColorPlanes", ""},
   {X_FreeColors,"X_FreeColors", ""},
   {X_StoreColors,"X_StoreColors", ""},
   {X_StoreNamedColor,"X_StoreNamedColor", ""},
   {X_QueryColors,"X_QueryColors", ""},
   {X_LookupColor,"X_LookupColor", ""},
   {X_CreateCursor,"X_CreateCursor", ""},
   {X_CreateGlyphCursor,"X_CreateGlyphCursor", ""},
   {X_FreeCursor,"X_FreeCursor", ""},
   {X_RecolorCursor,"X_RecolorCursor", ""},
   {X_QueryBestSize,"X_QueryBestSize", ""},
   {X_QueryExtension,"X_QueryExtension", ""},
   {X_ListExtensions,"X_ListExtensions", ""},
   {X_ChangeKeyboardMapping,"X_ChangeKeyboardMapping", ""},
   {X_GetKeyboardMapping,"X_GetKeyboardMapping", ""},
   {X_ChangeKeyboardControl,"X_ChangeKeyboardControl", ""},
   {X_GetKeyboardControl,"X_GetKeyboardControl", ""},
   {X_Bell,"X_Bell", ""},
   {X_ChangePointerControl,"X_ChangePointerControl", ""},
   {X_GetPointerControl,"X_GetPointerControl", ""},
   {X_SetScreenSaver,"X_SetScreenSaver", ""},
   {X_GetScreenSaver,"X_GetScreenSaver", ""},
   {X_ChangeHosts,"X_ChangeHosts", ""},
   {X_ListHosts,"X_ListHosts", ""},
   {X_SetAccessControl,"X_SetAccessControl", ""},
   {X_SetCloseDownMode,"X_SetCloseDownMode", ""},
   {X_KillClient,"X_KillClient", ""},
   {X_RotateProperties	,"X_RotateProperties", ""},
   {X_ForceScreenSaver	,"X_ForceScreenSaver", ""},
   {X_SetPointerMapping,"X_SetPointerMapping", ""},
   {X_GetPointerMapping,"X_GetPointerMapping", ""},
   {X_SetModifierMapping	,"X_SetModifierMapping", ""},
   {X_GetModifierMapping	,"X_GetModifierMapping", ""},
   {X_NoOperation,"X_NoOperation", ""},
   {-1,NULL, NULL}
};

struct data_description error_field[]=
{
   {Success,"Success", ""}, 
   {BadRequest,"BadRequest", ""}, 
   {BadValue,"BadValue", ""}, 
   {BadWindow,"BadWindow", ""}, 
   {BadPixmap,"BadPixmap", ""}, 
   {BadAtom,"BadAtom", ""}, 
   {BadCursor,"BadCursor", ""}, 
   {BadFont,"BadFont", ""}, 
   {BadMatch,"BadMatch", ""}, 
   {BadDrawable,"BadDrawable", ""}, 
   {BadAccess,"BadAccess", ""}, 
   {BadAlloc,"BadAlloc", ""}, 
   {BadColor,"BadColor", ""}, 
   {BadGC,"BadGC", ""}, 
   {BadIDChoice,"BadIDChoice", ""}, 
   {BadName,"BadName", ""}, 
   {BadLength,"BadLength", ""}, 
   {BadImplementation,"BadImplementation", ""}, 
   {-1,NULL, NULL}
};

struct data_description reply_field[]=
{
   {sz_xGenericReply,"sz_xGenericReply", ""}, 
   {-1,NULL, NULL}
};


int
xnee_event2int(char *ev)
{
  int i ; 

  for (i=0;event_field[i].data_nr!=-1;i++)
    {
       if (strncmp(event_field[i].data_name, 
                   ev, 
                   strlen(event_field[i].data_name))==0)
       {
          return event_field[i].data_nr;
       }
    }
  return -1;
}

int
xnee_request2int(char *req)
{
  int i ; 

  for (i=0;request_field[i].data_nr!=-1;i++)
  {
     int len = XNEE_MAX (strlen(request_field[i].data_name), strlen(req));

     if (strncmp(request_field[i].data_name, req, len)==0)
     {
        return request_field[i].data_nr;
     }
  }
  return -1;
}


int
xnee_error2int(char *ev)
{
   int i ; 
   
  for (i=0;error_field[i].data_nr!=-1;i++)
  {
     if (strncmp(error_field[i].data_name, 
                 ev, 
                 strlen(error_field[i].data_name))==0)
     {
        return error_field[i].data_nr;
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
   int i ;
   for (i=0;event_field[i].data_nr!=-1;i++)
   {
      if (event_field[i].data_nr == ev)
      {
         return event_field[i].data_name;
      }
   }
   return NULL;
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
       int i ;
       for (i=0;error_field[i].data_nr!=-1;i++)
       {
          if (error_field[i].data_nr == err)
          {
             return error_field[i].data_name;
          }
       }
    }
  return NULL;
}



/*
 * 
 * Prints the name of the error that corresponds to the argument ev.  
 * Printout is made on stdout.  
 *
 */
char * 
xnee_print_request (int req)
{
   int i ;
   for (i=0;request_field[i].data_nr!=-1;i++)
   {
      if (request_field[i].data_nr == req)
      {
         return request_field[i].data_name;
      }
   }
   return NULL;
}


/*
 * Prints the name of the request that corresponds to the argument req.  
 */
char *
xnee_int2request(int req)
{
  int i ;
  if ( (req <= 0) || (req>X_NoOperation) )
    {
      return NULL;
    }
  else 
    {
       for (i=0;request_field[i].data_nr!=-1;i++)
       {
          if (request_field[i].data_nr == req)
          {
	    return request_field[i].data_name;
          }
       }
    }
   return NULL;
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
   int i;
   if ( (ev <= 1) || (ev>LASTEvent) )
   {
      return NULL;
   }
   else 
   {
      for (i=0;event_field[i].data_nr!=-1;i++)
      {
         if (event_field[i].data_nr == ev)
         {
            return event_field[i].data_name;
         }
      }
   }
   return NULL;
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
   int i ; 
   for (i=0;error_field[i].data_nr!=-1;i++)
   {
      if (error_field[i].data_nr == err)
      {
         return error_field[i].data_name;
      }
   }
   return NULL;
}





/**
 * Get the integer value from X11 data dat
 */
int
xnee_data2int(int type, char *dat)
{
  if (type==XNEE_DEVICE_EVENT)
    return xnee_event2int(dat);
  else  if (type==XNEE_EVENT)
    return xnee_event2int(dat);
  else  if (type==XNEE_DELIVERED_EVENT)
    return xnee_event2int(dat);
  else if (type==XNEE_REQUEST)
    return xnee_request2int(dat);
  else if (type==XNEE_ERROR)
    return xnee_error2int(dat);
  else if (type==XNEE_REPLY)
    return -1;
  else 
    return -1;
}

/**
 * Get the integer value from X11 data dat
 */
char*
xnee_int2data(int type, int dat)
{
  if (type==XNEE_DEVICE_EVENT)
    return xnee_int2event(dat);
  else  if (type==XNEE_EVENT)
    return xnee_int2event(dat);
  else  if (type==XNEE_DELIVERED_EVENT)
    return xnee_int2event(dat);
  else if (type==XNEE_REQUEST)
    return xnee_int2request(dat);
  else if (type==XNEE_ERROR)
    return xnee_int2error(dat);
  else if (type==XNEE_REPLY)
    return NULL;
  else 
    return NULL;
}

/**
 * Get the integer value from X11 data dat
 */
int
xnee_data2int_special(int *type, char *dat)
{
  int i ; 
  int ret;

  for (i=0;i<XNEE_NR_OF_TYPES;i++)
    {
      if (i==XNEE_EVENT)
      {
	;
      }
      else 
	{
	  ret = xnee_data2int(i, dat);
	  if (ret != -1)
	    {
	      if ( (i==XNEE_DEVICE_EVENT) && (ret > MotionNotify))
		;
	      else if ( (i==XNEE_DELIVERED_EVENT) && (ret <= MotionNotify))
		;
	      else
		{
		  *type = i;
		  return ret;
		}
	    }
	}
    }
  return -1;
}






struct data_description*
xnee_get_event_names()
{
  return event_field;
}


struct data_description*
xnee_get_error_names()
{
  return error_field;
}


struct data_description*
xnee_get_request_names()
{
  return request_field;
}

struct data_description*
xnee_get_reply_names()
{
  return reply_field;
}



