/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 2006, 2007, 2008, 2009, 2010 
 *                      Henrik Sandklef                    
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
#include "libxnee/xnee_window.h"
#include "libxnee/print_varargs.h"
#include "libxnee/print.h"
#include "libxnee/xnee_alloc.h"
#include "libxnee/xnee_setget.h"

/*
 *  In this file the following two words are frequent enough
 *  to let their usage be explained:
 *     session   - information read from session file
 *     received  - information as received during replay from RECORD
 */ 

#define XNEE_DEBUG_WINDOW_CODE 1
#ifdef  XNEE_DEBUG_WINDOW_CODE
#define XNEE_WINDOW_DEBUG(a) printf a
#else
#define XNEE_WINDOW_DEBUG(a) 
#endif

#define MAX_NR_OF_MOVES     10
#define XNEE_WINDOW_BUFFER_SIZE 10
static int received_index = 0;
static int session_index  = 0;

xnee_win_pos received_windows[XNEE_WINDOW_BUFFER_SIZE];
xnee_win_pos session_windows[XNEE_WINDOW_BUFFER_SIZE];

static int
xnee_window_print_lists(void)
{
  int i ; 

  fprintf (stderr,"----  List of stored window --- \n");
  fprintf (stderr,"|\n");
  fprintf (stderr,"| -- Session -- \n");
  for (i=0; ( i<session_index ) && (i<XNEE_WINDOW_BUFFER_SIZE ) ;i++)
    {
      fprintf (stderr,"| %d  0x%X %d %d '%s'\n",
	       i,
	       session_windows[i].window,
	       session_windows[i].x,
	       session_windows[i].y,
	       session_windows[i].name?session_windows[i].name:"NO NAME");
    }
  fprintf (stderr,"|\n");
  fprintf (stderr,"| -- Received --\n");
  for (i=0;( i<received_index ) && (i<XNEE_WINDOW_BUFFER_SIZE ) ;i++)
    {
      fprintf (stderr,"| %d 0x%X %d %d '%s'\n",
	       i,
	       received_windows[i].window,
	       received_windows[i].x,
	       received_windows[i].y,
	       received_windows[i].name?received_windows[i].name:"NO NAME");
    }
  fprintf (stderr,"--- End of list of windows ....\n");
  fprintf (stderr,"\n");

  return XNEE_OK;
}


int
xnee_window_add_attribute_impl(xnee_data    *xd, 
			       XWindowAttributes *attributes,
			       Window win,
			       Window parent,
			       int where)
{
  xnee_win_pos  xwp;
  char *win_name;

  xwp.x      = attributes->x;
  xwp.y      = attributes->y;
  xwp.width  = attributes->width;
  xwp.height = attributes->height;
  xwp.window = win;
  xwp.parent = parent;

  if (where == XNEE_WINDOW_RECEIVED)
    {
      XFlush(xd->grab);

      if (!XFetchName(xd->grab, win, &win_name)) 
	{ /* Get window name if any */
	  xnee_verbose((xd," window has has no name\n"));
	  win_name=NULL;
	} 
      else if (win_name) 
	{
	  xnee_verbose((xd," window has has name '%s'\n", win_name));
	} 
      xwp.name = win_name;
    }
  else
    { 
      xwp.name = NULL;
    }
  
  return xnee_window_add_impl(xd, &xwp, where); 
}


int
xnee_window_add_impl(xnee_data    *xd, 
		     xnee_win_pos *xwp,
		     int where)
{
  xnee_win_pos *ptr;
  int index ;

  xnee_verbose((xd, "---> xnee_window_add_impl\n"));

  switch ( where )
   {
   case XNEE_WINDOW_RECEIVED:
      index = received_index;
      ptr = &received_windows[received_index];
      break;
   case XNEE_WINDOW_SESSION:
      index = session_index;
      ptr = &session_windows[session_index];
      break;
   default:
     return XNEE_WINDOW_POS_ADJ_ERROR;
   }


  if ( index >= XNEE_WINDOW_BUFFER_SIZE )
    {
      xnee_verbose ((xd, "<--- xnee_window_add_impl ... error\n"));
      return XNEE_WINDOW_POS_ADJ_ERROR;
    }

  
  memcpy(ptr,
	 xwp,
	 sizeof(xnee_win_pos));
  
  if ( where == XNEE_WINDOW_RECEIVED )
    {
      received_index++;
    }
  else
    {
      session_index++;
    }

  xnee_verbose((xd, "<--- xnee_window_add_impl\n"));
  return XNEE_OK;
}


#ifdef CRAP
int
xnee_window_add_read(xnee_data    *xd, 
		     xnee_win_pos *xwp)
{
  xnee_win_pos *ptr;

  xnee_verbose((xd, "---> xnee_window_add_read\n"));

  if ( session_index >= XNEE_WINDOW_BUFFER_SIZE )
    {
      xnee_verbose((xd, "<--- xnee_window_add_read ... error\n"));
      return XNEE_WINDOW_POS_ADJ_ERROR;
    }

  
  ptr = &session_windows[session_index];

  ptr->rel_x  = xwp->rel_x;
  ptr->rel_y  = xwp->rel_y;
  ptr->x      = xwp->x;
  ptr->y      = xwp->y;
  ptr->height = xwp->height;
  ptr->width  = xwp->width;
  ptr->window = xwp->window;
  
  if ( where == XNEE_WINDOW_RECEIVED )
    {
      received_index++;
    }
  else
    {
      session_index++;
    }

  xnee_verbose((xd, "<--- xnee_window_add_impl\n"));
  return XNEE_OK;
}
#endif /* CRAP?  */ 


int
xnee_window_add_read(xnee_data    *xd, 
		     xnee_win_pos *xwp)
{
  xnee_win_pos *ptr;

  xnee_verbose((xd, "---> xnee_window_add_read\n"));

  if ( session_index >= XNEE_WINDOW_BUFFER_SIZE )
    {
      xnee_verbose((xd, "<--- xnee_window_add_read ... error\n"));
      return XNEE_WINDOW_POS_ADJ_ERROR;
    }

  
  ptr = &session_windows[session_index];

  ptr->x      = xwp->x;
  ptr->y      = xwp->y;
  ptr->height = xwp->height;
  ptr->width  = xwp->width;
  
  session_index++;

  xnee_verbose((xd, "<--- xnee_window_add_read\n"));
  return XNEE_OK;
}


static int
xnee_window_remove_window(xnee_data *xd, int rec_idx, int ses_idx)
{
  int i ; 

  xnee_verbose((xd,"xnee_window_remove_window recorded:%d    session:%d\n",
		rec_idx, ses_idx));

  for (i=rec_idx; i<XNEE_WINDOW_BUFFER_SIZE;i++)
    {
      /*      XNEE_WINDOW_DEBUG(("Rem rece pos %d (%s,%d) and putting pos %d (%s) \n",
	      i, received_windows[i].name,received_windows[i].window,
	      i+1, received_windows[i+1].name));
      */
      XNEE_FREE_IF_NOT_NULL(received_windows[i].name);
      memcpy(&received_windows[i],
	     &received_windows[i+1],
	     sizeof(xnee_win_pos));
      if (received_windows[i+1].window==0)
	{
	  break;
	}

    }
  
  for (i=ses_idx; i<XNEE_WINDOW_BUFFER_SIZE;i++)
    {
      /*      XNEE_WINDOW_DEBUG(("Rem sess pos %d (%s,%d) and putting pos %d (%s) \n",
	      i, session_windows[i].name,session_windows[i].window,
	      i+1, session_windows[i+1].name));
      */
      if (session_windows[i].name!=NULL)
	{
	  XFree(session_windows[i].name);
	}
      memcpy(&session_windows[i],
	     &session_windows[i+1],
	     sizeof(xnee_win_pos));
      if (session_windows[i+1].window==0)
	{
	  break;
	}
      
    }
  session_index--;
  received_index--;
      
  return XNEE_OK;
}




int
xnee_window_try_move(xnee_data *xd)
{
  int ret = XNEE_OK;
  xnee_win_pos *sess_ptr;
  xnee_win_pos *rec_ptr;

  int pos_x;
  int pos_y;
  int rx ;
  int ry ;
  int diff_x=0;
  int diff_y=0;
  int nr_of_moves = 0;
  XWindowAttributes win_attributes;
  Window child;

  xnee_verbose((xd, "Trying to move window %d %d \n",
	  session_index,
	  received_index));

  if (xnee_is_verbose(xd))
    {
      xnee_window_print_lists();
    }

  /*
   * If we haven't read (from file) and received (from RECORD)
   * then there we lack info on how to move the new window...
   * return....
   */
  if ( (session_index==0) || (received_index==0) )
    {
      return XNEE_OK;
    }

  XNEE_VERBOSE_MARK();

  xnee_verbose((xd, "Trying to move window 1\n"));
  /*
   * OK, we have read info on saved window and we have
   * a window to apply that info on, let's do it.
   */
  sess_ptr =  &session_windows[session_index-1];
  rec_ptr  =  &received_windows[received_index-1];

  
  if ( rec_ptr->window == 0 )
    {
      return XNEE_WINDOW_POS_ADJ_ERROR;
    }
  
  XNEE_VERBOSE_MARK();
  XGetWindowAttributes(xd->grab, 
		       rec_ptr->window,
		       &win_attributes);
  
  XTranslateCoordinates (xd->grab, 
			 rec_ptr->window, 
			 win_attributes.root, 
			 win_attributes.border_width,
			 win_attributes.border_width,
			 &rx, 
			 &ry, 
			 &child);


  if ( (sess_ptr->x==rx) && (sess_ptr->y==ry) )
    {
      XNEE_WINDOW_DEBUG(("Windows are already loacted correctly :)   %d %d    %d %d\n",
	      sess_ptr->x, 
	      rx, 
	      sess_ptr->y, 
	      ry));
      xnee_window_remove_window(xd, received_index-1, session_index-1);
      return XNEE_OK;
    }

  xnee_verbose((xd, "XMoveWindow(%p,0x%X, %u,%u)\n",
		(void*)xd->grab,
		rec_ptr->window,
		(unsigned int)sess_ptr->x ,
		(unsigned int)sess_ptr->y ));
  XNEE_WINDOW_DEBUG(("XMoveWindow(%p,0x%X, %u,%u)\n",
		(void*)xd->grab,
		rec_ptr->window,
		(unsigned int)sess_ptr->x ,
		(unsigned int)sess_ptr->y ));
  XMoveWindow(xd->grab,
	      rec_ptr->window,
	      sess_ptr->x ,
	      sess_ptr->y );
  
  XFlush(xd->grab);
  usleep(50*1000);
  
  XGetWindowAttributes(xd->grab, 
		       rec_ptr->window,
		       &win_attributes);
  
  XTranslateCoordinates (xd->grab, 
			 rec_ptr->window, 
			 win_attributes.root, 
			 win_attributes.border_width,
			 win_attributes.border_width,
			 &rx, 
			 &ry, 
			 &child);
  
  
  while ( nr_of_moves<MAX_NR_OF_MOVES)
    {
      

      /*       requested   - actual - frame */
      XNEE_VERBOSE_MARK();

      diff_x = sess_ptr->x - rx ;
      diff_y = sess_ptr->y - ry ;

      XNEE_WINDOW_DEBUG(("Requested   Actual   diff    attrib   window=0x%X  (%d try)\n", rec_ptr->window, nr_of_moves));
      XNEE_WINDOW_DEBUG(("   %.3d      %.3d      %.2d       %d\n",
			 sess_ptr->x , rx , diff_x, win_attributes.x));
      XNEE_WINDOW_DEBUG(("   %.3d      %.3d      %.2d       %d\n",
			 sess_ptr->y , ry , diff_y, win_attributes.y));
      XNEE_WINDOW_DEBUG(("\n"));
      
      if  ( !diff_x && !diff_y )
	{


	  xnee_window_remove_window(xd, received_index-1, session_index-1);
	  xnee_verbose((xd,"leaving ..... all  (%d %d) is ok\n", diff_x, diff_y));
	  break;
	}

      pos_x = sess_ptr->x + diff_x ;
      pos_y = sess_ptr->y + diff_y ;

      xnee_verbose((xd, "XMoveWindow(%p,0x%X, %u,%u)\n",
		    (void*)xd->grab,
		    rec_ptr->window,
		    (unsigned int)pos_x ,
		    (unsigned int)pos_y ));
      XNEE_WINDOW_DEBUG(("XMoveWindow(%p,0x%X, %u,%u)\n",
		    (void*)xd->grab,
		    rec_ptr->window,
		    (unsigned int)pos_x ,
		    (unsigned int)pos_y ));
      XMoveWindow(xd->grab, rec_ptr->window, pos_x, pos_y);

      XFlush(xd->grab);
      usleep(50*1000);
      XGetWindowAttributes(xd->grab, 
			   rec_ptr->window,
			   &win_attributes);
      
      XTranslateCoordinates (xd->grab, 
			     rec_ptr->window, 
			     win_attributes.root, 
			     win_attributes.border_width,
			     win_attributes.border_width,
			     &rx, 
			     &ry, 
			     &child);
      nr_of_moves++;
    }

  xnee_verbose((xd, "<--- xnee_window_try_move:   moved %d times\n", nr_of_moves));
  return ret;
}


