/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 2005, 2006, 2007 Henrik Sandklef 
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


#ifndef PNEE_TYPES_H
#define PNEE_TYPES_H

typedef enum _pnee_actions
  {
    PNEE_ACTION_NONE,
    PNEE_ACTION_RECORD,
    PNEE_ACTION_REPLAY,
    PNEE_ACTION_CONFIGURE
  } pnee_actions;


typedef enum _pnee_update_actions
  {
    PNEE_UPDATE_NONE,
    PNEE_UPDATE_RECORD,
    PNEE_UPDATE_REPLAY
  } pnee_update_actions;

typedef enum _pnee_button_type
  {
    PNEE_BUTTON_RECORD,
    PNEE_BUTTON_REPLAY,
    PNEE_BUTTON_STOP
  } pnee_button_type;

typedef struct
{
  PanelApplet *applet;
  GtkWidget *button_box;	
  GtkWidget *label;	
  GtkWidget *container;
  GtkTooltips *tooltips;

  GtkWidget *rec_button;
  GtkWidget *rep_button;
  GtkWidget *stop_button;

  GtkWidget *rec_image;
  GtkWidget *rep_image;
  GtkWidget *stop_image;

  GtkTooltips* rec_tooltip;
  GtkTooltips* rep_tooltip;
  GtkTooltips* stop_tooltip;
  GtkWidget *progress;

  GtkIconSize icon_size;

  pthread_t  xnee_thread;
  pthread_t  xnee_updater;

  sem_t      action_mutex;
  sem_t      update_mutex;
  GtkWidget *pnee_pref;

  volatile pnee_actions action_state ;
  volatile pnee_update_actions update_state ;

  GtkWidget *delay_progr;
  
  unsigned char reset;

  xnee_data *xd ;

/* #define FAKED_MAIN */
#define PNEE_DEBUGGER 
#ifdef PNEE_DEBUGGER
  pthread_t xnee_debugger;
#endif /* PNEE_DEBUGGER */
} pnee_panel_applet;





#endif /*  PNEE_TYPES_H */
