#include "recordables.h"

#define GNEE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)


static char* events[][2] = {
    { "KeyPress", "Event description" },
    { "KeyRelease", "Event description" },
    { "ButtonPress", "Event description" },
    { "ButtonRelease", "Event description" },
    { "MotionNotify", "Event description" },
    { "EnterNotify", "Event description" },
    { "LeaveNotify", "Event description" },
    { "FocusIn", "Event description" },
    { "FocusOut", "Event description" },
    { "KeymapNotify", "Event description" },
    { "Expose", "Event description" },
    { "GraphicsExpose", "Event description" },
    { "NoExpose", "Event description" },
    { "VisibilityNotify", "Event description" },
    { "CreateNotify", "Event description" },
    { "DestroyNotify", "Event description" },
    { "UnmapNotify", "Event description" },
    { "MapNotify", "Event description" },
    { "MapRequest", "Event description" },
    { "ReparentNotify", "Event description" },
    { "ConfigureNotify", "Event description" },
    { "ConfigureRequest", "Event description" },
    { "GravityNotify", "Event description" },
    { "ResizeRequest", "Event description" },
    { "CirculateNotify", "Event description" },
    { "CirculateRequest", "Event description" },
    { "PropertyNotify", "Event description" },
    { "SelectionClear", "Event description" },
    { "SelectionRequest", "Event description" },
    { "SelectionNotify", "Event description" },
    { "ColormapNotify", "Event description" },
    { "ClientMessage", "Event description" },
    { "MappingNotify", "Event description" },
    NULL
};

static char* replies[][2] = {
    { "X11 Reply", "Reply description" },
    NULL
};

static char* requests[][2] = {
    { "CreateWindow", "Request description" },
    { "ChangeWindowAttributes", "Request description" },
    { "GetWindowAttributes", "Request description" },
    { "DestroyWindow", "Request description" },
    { "DestroySubwindows", "Request description" },
    { "ChangeSaveSet", "Request description" },
    { "ReparentWindow", "Request description" },
    { "MapWindow", "Request description" },
    { "MapSubwindows", "Request description" },
    { "UnmapWindow", "Request description" },
    { "UnmapSubwindows", "Request description" },
    { "ConfigureWindow", "Request description" },
    { "CirculateWindow", "Request description" },
    { "GetGeometry", "Request description" },
    { "QueryList", "Request description" },
    { "InternAtom", "Request description" },
    { "GetAtomName", "Request description" },
    { "ChangeProperty", "Request description" },
    { "DeleteProperty", "Request description" },
    { "GetProperty", "Request description" },
    { "ListProperties", "Request description" },
    { "SetSelectionOwner", "Request description" },
    { "GetSelectionOwner", "Request description" },
    { "ConvertSelection", "Request description" },
    { "SendEvent", "Request description" },
    { "GrabPointer", "Request description" },
    { "UngrabPointer", "Request description" },
    { "GrabButton", "Request description" },
    { "UngrabButton", "Request description" },
    { "ChangeActivePointerGrab", "Request description" },
    { "GrabKeyboard", "Request description" },
    { "UngrabKeyboard", "Request description" },
    { "GrabKey", "Request description" },
    { "UngrabKey", "Request description" },
    { "AllowEvents", "Request description" },
    { "GrabServer", "Request description" },
    { "UngrabServer", "Request description" },
    { "QueryPointer", "Request description" },
    { "GetMotionEvents", "Request description" },
    { "TranslateCoords", "Request description" },
    { "WarpPointer", "Request description" },
    { "SetInputFocus", "Request description" },
    { "GetInputFocus", "Request description" },
    { "QueryKeymap", "Request description" },
    { "OpenFont", "Request description" },
    { "CloseFont", "Request description" },
    { "QueryFont", "Request description" },
    { "QueryTextExtents", "Request description" },
    { "ListFonts", "Request description" },
    { "ListFontsWithInfo", "Request description" },
    { "SetFontPath", "Request description" },
    { "GetFontPath", "Request description" },
    { "CreatePixmap", "Request description" },
    { "FreePixmap", "Request description" },
    { "CreateGC", "Request description" },
    { "ChangeGC", "Request description" },
    { "CopyGC", "Request description" },
    { "SetDashes", "Request description" },
    { "SetClipRectangles", "Request description" },
    { "FreeGC", "Request description" },
    { "ClearArea", "Request description" },
    { "CopyArea", "Request description" },
    { "CopyPlane", "Request description" },
    { "PolyPoint", "Request description" },
    { "PolyLine", "Request description" },
    { "PolySegment", "Request description" },
    { "PolyRectangle", "Request description" },
    { "PolyArc", "Request description" },
    { "FillPoly", "Request description" },
    { "PolyFillRectangle", "Request description" },
    { "PolyFillArc", "Request description" },
    { "PutImage", "Request description" },
    { "GetImage", "Request description" },
    { "PolyText8", "Request description" },
    { "PolyText16", "Request description" },
    { "ImageText8", "Request description" },
    { "ImageText16", "Request description" },
    { "CreateColormap", "Request description" },
    { "FreeColormap", "Request description" },
    { "CopyColormapAndFree", "Request description" },
    { "InstallColormap", "Request description" },
    { "UninstallColormap", "Request description" },
    { "ListInstalledColormaps", "Request description" },
    { "AllocColor", "Request description" },
    { "AllocNamedColor", "Request description" },
    { "AllocColorCells", "Request description" },
    { "AllocColorPlanes", "Request description" },
    { "FreeColors", "Request description" },
    { "StoreColors", "Request description" },
    { "StoreNamedColor", "Request description" },
    { "QueryColors", "Request description" },
    { "LookupColor", "Request description" },
    { "CreateCursor", "Request description" },
    { "CreateGlyphCursor", "Request description" },
    { "FreeCursor", "Request description" },
    { "RecolorCursor", "Request description" },
    { "QueryBestSize", "Request description" },
    { "QueryExtension", "Request description" },
    { "ListExtensions", "Request description" },
    { "ChangeKeyboardMapping", "Request description" },
    { "GetKeyboardMapping", "Request description" },
    { "ChangeKeyboardControl", "Request description" },
    { "GetKeyboardControl", "Request description" },
    { "Bell", "Request description" },
    { "ChangePointerControl", "Request description" },
    { "GetPointerControl", "Request description" },
    { "SetScreenSaver", "Request description" },
    { "GetScreenSaver", "Request description" },
    { "ChangeHosts", "Request description" },
    { "ListHosts", "Request description" },
    { "SetAccessControl", "Request description" },
    { "SetCloseDownMode", "Request description" },
    { "KillClient", "Request description" },
    { "RotateProperties", "Request description" },
    { "ForceScreenSaver", "Request description" },
    { "SetPointerMapping", "Request description" },
    { "GetPointerMapping", "Request description" },
    { "SetModifierMapping", "Request description" },
    { "GetModifierMapping", "Request description" },
    NULL
};
    

static char* errors[][2] = {
    { "Success", "Error description" },
    { "BadRequest", "Error description" },
    { "BadValue", "Error description" },
    { "BadWindow", "Error description" },
    { "BadPixmap", "Error description" },
    { "BadAtom", "Error description" },
    { "BadCursor", "Error description" },
    { "BadFont", "Error description" },
    { "BadMatch", "Error description" },
    { "BadDrawable", "Error description" },
    { "BadAccess", "Error description" },
    { "BadAlloc", "Error description" },
    { "BadColor", "Error description" },
    { "BadGC", "Error description" },
    { "BadIDChoice", "Error description" },
    { "BadName", "Error description" },
    { "BadLength", "Error description" },
    NULL
};


void
move_between_lists(GtkTreeView* source_list, 
                   GtkTreeView* destination_list,
                   xnee_data* xd,
                   int type,
                   gboolean include);

void
create_recordable_list(GtkWidget* gnee_window,
                       char* names[][2], 
                       char* exclude_store_name,
                       char* include_store_name);

void
gnee_recordables_create(GtkWidget* gnee_window)
{
    create_recordable_list
        (gnee_window, events, "exclude_event_store", "include_event_store");
    create_recordable_list
        (gnee_window, errors, "exclude_error_store", "include_error_store");
    create_recordable_list
        (gnee_window, replies, "exclude_reply_store", "include_reply_store");
    create_recordable_list
        (gnee_window, requests, "exclude_request_store", "include_request_store");
}

void
gnee_recordables_include(GtkTreeView* source_list,
                         GtkTreeView* destination_list,
                         xnee_data* xd,
                         int type)
{
    move_between_lists(source_list, destination_list, xd, type, TRUE);
}

void
gnee_recordables_exclude(GtkTreeView* source_list,
                         GtkTreeView* destination_list,
                         xnee_data* xd,
                         int type)
{
    move_between_lists(source_list, destination_list, xd, type, FALSE);
}

void
move_between_lists(GtkTreeView* source_list, 
                   GtkTreeView* destination_list,
                   xnee_data* xd,
                   int type,
                   gboolean include)
{
    GtkTreeSelection   *selection;
    char               *selection_value;
    gboolean            selected;
    GtkTreeModel       *model;
    GtkTreeIter         iterator;
    GtkTreeStore       *store;
    gint                new_order;
    
    selection = gtk_tree_view_get_selection(source_list);
    
    selected = gtk_tree_selection_get_selected(selection,
                                               &model,
                                               &iterator);
    
    if (selected)
    {
        gtk_tree_model_get(model, &iterator,
                           0, &selection_value, 
                           -1);
        
        store = GTK_TREE_STORE(model);
        gtk_tree_store_remove(store, &iterator);
        
        store = GTK_TREE_STORE(gtk_tree_view_get_model
                               (destination_list));
        gtk_tree_store_append(store, &iterator, NULL);
        gtk_tree_store_set(store, &iterator, 0, selection_value, -1);

        if (xd == NULL)
        {
            g_print("xd is null\n");
        }

        if (include)
        {
            g_print("will include '%s'\n", selection_value);
             xnee_add_range_str
                 (xd, type, selection_value);
        }
        else
        {
            g_print("will exclude '%s'\n", selection_value);
            xnee_rem_data_from_range_str
                (xd, type, selection_value);            
        }

        g_free(selection_value);
        
    }

}

void
create_recordable_list(GtkWidget* gnee_window,
                       char* names[][2], 
                       char* exclude_store_name,
                       char* include_store_name)
{

    GtkTreeStore       *store;
    GtkTreeIter         iterator; 
    int                 i;

    store = gtk_tree_store_new (1, G_TYPE_STRING);

    i = 0;
    while (names[i][0] != NULL)
    {
        gtk_tree_store_append(store, &iterator, NULL);
        gtk_tree_store_set(store, &iterator, 0, names[i][0], -1);
        i++;
    }

    GNEE_HOOKUP_OBJECT(gnee_window, store, exclude_store_name);

    store = gtk_tree_store_new (1, G_TYPE_STRING);

    GNEE_HOOKUP_OBJECT(gnee_window, store, include_store_name);

}
