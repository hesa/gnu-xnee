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


#include "libxnee/xnee.h"
#include "libxnee/xnee_record.h"
#include "libxnee/xnee_replay.h"
#include "parse.h"
#include "libxnee/print.h"



static char *help[] = {
  "--help, -h                     ", "Print this message", 
  "--display,d  displayname       ", "X server to contact (default is localhost)", 
  "--flags, -flags                ", "Prints all flags/options xnee accepts",
  "--file, -f <file_name>         ", "Read data from file file_name (default is stdin)", 
  "--out, -o <file_name>          ", "Redirect all Xnee data to file_name (default is stdout)", 
  "--err, -e <file_name>          ", "Redirect all Xnee verbose output file_name (default is stderr)", 
  "--resource ,-r <file_name>     ", "Use resource file file_name",
  "--plugin ,-p <name     >       ", "Use the plugin name name",
  "--all-events, -ae              ", "Intercept all events",
  "--no-expose, -ne               ", "Do not intercept (print) Expose NoExpose",
  "--first-last, -fl              ", "Print only first and last of multiple successive MotionEvent ",
  "--everything                   ", "Intercept all X11 Protocol .... beware, your computer may crash !",
  "--loops <n>, -l                ", "Intercept n numbers of data ( n<0 means forever)",
  "--k-log                        ", "Intercept 1000     (one thousand) data",
  "--10k-log                      ", "Intercept 10 000   (ten thousand) data",
  "--100k-log                     ", "Intercept 100 000  (one hundred thousand) data",
  "--m-log                        ", "Intercept 1000 000 (one million) data",
  "--verbose, -v                  ", "Verbose printout",
  "--buffer-verbose, -bv          ", "Verbose printout of replay buffer",
  "--time, -t  <secs>             ", "Delay start of application for <secs> seconds. Used to prevent recording of KeyRelease when starting Xnee from terminal",
  "--binary, -b                   ", "Obsolete option, don't use this",
  "--text, -t                     ", "Store data output in text format (default)",
  "--version, -V                  ", "Print product information",
  "--all-clients, -ac             ", "Record all client's data (default)" , 
  "--future-clients, -fc          ", "Record future client's data" , 
  "--human-printouts, -hp         ", "Prints human readable" , 
  "--record, -rec                 ", "Set recording mode (default)" , 
  "--replay, -rep                 ", "Set replaying mode" , 
  "--stop-key mod,key, -sk        ", "When pressing modifier mod and key key Xnee exits" , 
  "--print-settings, -ps          ", "Prints Xnee settings and waits (for <ENTER>)", 
  "--print-event-names, -pens     ", "Prints X11 event number and name ", 
  "--print-event-name, -pen <ev>  ", "Prints X11 event number or name coresponding to ev", 
  "--print-error-names, -pern     ", "Prints X11 error number and name ", 
  "--print-reply-names, -pren     ", "Prints X11 reply number and name ", 
  "--print-request-names, -prns   ", "Prints X11 request number and name ", 
  "--print-request-name, -prn <req> ", "Prints X11 request number or name  coresponding to req", 
  "--print-data-names, -pdn       ", "Prints X11 data number and name ", 
  "--manpage                      ", "Prints Xnee help text in format as used when generating man page", 
  "--distribute, -di <LIST>       ", "Distribute recorded or replayed events to LIST where LIST is comma separated list of displays",
  "--device-event-range, -devera  <X_LIST> ", "Set device event range to X_LIST", 
  "--delivered-event-range, -devra    <X_LIST>", "Set delivered event range to X_LIST", 
  "--error-range, -erra    <X_LIST>", "Set error range to X_LIST", 
  "--request-range, -reqra <X_LIST>","Set request range to X_LIST", 
  "--reply-range, -repra   <X_LIST>","Set reply range to X_LIST", 
  "--extension-request-major-range, -erqmar  <X_LIST>  ","Set extension request major range to X_LIST", 
  "--extension-request-mainor-range, -erqmir  <X_LIST> ", "Set extension request minor range to X_LIST", 
  "--extension-reply-major-range, -erpmar  <X_LIST>    ","Set extension reply major range to X_LIST", 
  "--extension-reply-mainor-range, -erpmir  <X_LIST>   ","Set extension reply minor range to X_LIST",
  "--force-replay, -fp             ", "Keep replaying even if we are out of sync .... dangerous",
  NULL 
};

static char *explain[] = {
  "file_name" , "Name of a file (e.g /tmp/user1_session.xnee)" ,
  "display_name" , "Name of a display (e.g 192.168.1.12:0" ,
  "secs" , "Seconds (e.g 10)" ,
  "n" , "Integer number (e.g 100)" ,
  "LIST" , "Comma separated list of display (e.g 192.168.1.12:0,129.12.23.0:0) ",
  "X_LIST" , "Comma separated list of integers (e.g 12-23,35,45-56) " ,
  NULL 
};


static char *examples[] = {
  "" PACKAGE "  --k_log -devera 2-6 -o /tmp/xnee.rec -e /tmp/xnee.log -v ", 
  "Writes 1000 data to file /tmp/xnee.rec and dumps the verbose printout to /tmp/xnee.log",
  "" PACKAGE " -rep -f /tmp/xnee.rec -v -e /tmp/xnee.log --no-sync",
  "Read data from /tmp/xnee.rec, replay it and verbose print to file /tmp/xnee.log",
  "For more examples, look at the EXAMPLES file",
  NULL 
};


static char *description[] = {
  "  " PACKAGE " can record and replay an X session." PACKAGE " also has the ability to ",
  "  distribute events to multiple displays.\n",
  NULL 
};





static char *obsolete_help[] = {
  "--help, -h                     ", "Print this message", 
  "--display,d  displayname       ", "X server to contact (default is localhost)", 
  "--flags, -flags                ", "Prints all flags/options xnee accepts",
  "--file, -f <file_name>         ", "Read data from file file_name (default is stdin)", 
  "--out, -o <file_name>          ", "Redirect all Xnee data to file_name (default is stdout)", 
  "--err, -e <file_name>          ", "Redirect all Xnee verbose output file_name (default is stderr)", 
  "--resource ,-p <file_name>       ", "Use resource file file_name",
  "--all_events, -ae              ", "Intercept all events",
  "--no-expose, -ne               ", "Do not intercept (print) Expose NoExpose",
  "--first_last, -fl              ", "Print only first and last of multiple successive MotionEvent ",
  "--everything                   ", "Intercept all X11 Protocol .... beware, your computer may crash !",
  "--loops <n>, -l                ", "Intercept n numbers of data ( n<0 means forever)",
  "--k_log                        ", "Intercept 1000     (one thousand) data",
  "--10k_log                      ", "Intercept 10 000   (ten thousand) data",
  "--100k_log                     ", "Intercept 100 000  (one hundred thousand) data",
  "--m_log                        ", "Intercept 1000 000 (one million) data",
  "--verbose, -v                  ", "Verbose printout",
  "--time, -t  <secs>             ", "Delay start of application for <secs> seconds. Used to prevent recording of KeyRelease when starting Xnee from terminal",
  "--binary, -b                   ", "Obsolete option, don't use this",
  "--text, -t                     ", "Store data output in text format (default)",
  "--version, -V                  ", "Print product information",
  "--all_clients, -ac             ", "Record all client's data (default)" , 
  "--future_clients, -fc          ", "Record future client's data" , 
  "--human_printouts, -hp         ", "Prints human readable" , 
  "--record, -rec                 ", "Set recording mode (default)" , 
  "--replay, -rep                 ", "Set replaying mode" , 
  "--stop_key mod,key, -sk        ", "When pressing modifier mod and key key Xnee exits" , 
  "--print_settings, -ps          ", "Prints Xnee settings and waits (for <ENTER>)", 
  "--manpage                      ", "Prints Xnee help text in format as used when generating man page", 
  "--distribute, -di <LIST>       ", "Distribute recorded or replayed events to LIST where LIST is comma separated list of displays",
  "--device_event_range, -devera  <X_LIST> ", "Set device event range to X_LIST", 
  "--delivered_event_range, -devra    <X_LIST>", "Set delivered event range to X_LIST", 
  "--error_range, -erra    <X_LIST>", "Set error range to X_LIST", 
  "--request_range, -reqra <X_LIST>","Set request range to X_LIST", 
  "--reply_range, -repra   <X_LIST>","Set reply range to X_LIST", 
  "--extension_request_major_range, -erqmar  <X_LIST>  ","Set extension request major range to X_LIST", 
  "--extension_request_mainor_range, -erqmir  <X_LIST> ", "Set extension request minor range to X_LIST", 
  "--extension_reply_major_range, -erpmar  <X_LIST>    ","Set extension reply major range to X_LIST", 
  "--extension_reply_mainor_range, -erpmir  <X_LIST>   ","Set extension reply minor range to X_LIST",
  NULL 
};


/**************************************************************
 *                                                            *
 * xnee_parse_args                                            *
 *                                                            *
 *                                                            *
 **************************************************************/
void  
xnee_parse_args (xnee_data* xd , int argc, char **argv )
{                                         

  int i ;
  int ret=0;
 
  for (i = 1; i < argc; i++) 
    {
      xnee_verbose ((xd, "##########  xnee_parse_args: %d / %d  \"%s\"\n", 
		    i, argc, argv[i]));
      if( xnee_check (argv[i], "--display", "-d"  ) )
	{
	  if (++i >= argc) 
	    {
	      xnee_usage(stderr);
	      xnee_close_down(xd);
	      exit(XNEE_WRONG_PARAMS);
	    }
	  xd->display = argv[i];
	  xnee_verbose((xd, "display=%s\n", xd->display ));
	  continue;
	}
      else if(xnee_check(argv[i], "--err", "-e" )) 
	{
	  if (++i >= argc)
	    {
	      xnee_usage(stderr);
	      xnee_close_down(xd);
	      exit(XNEE_WRONG_PARAMS);
	    }
	  /*
	   *	  xd->std_err = True ;
	   */
	  xd->err_name = (char *) malloc ( strlen(argv[i])+1 );
	  xd->err_name = argv[i];
	  xd->err = fopen (argv[i],"w");
	  continue; 
	}
      else if(xnee_check(argv[i], "--resource", "-r" )) 
	{
	  if (++i >= argc)
	    {
	      xnee_usage(stderr);
	      xnee_close_down(xd);
	      exit(XNEE_WRONG_PARAMS);
	    }
	  xd->sf_name = argv[i];
	  xd->sf = fopen (argv[i],"r");
	  if ( xd->sf == NULL) 
	    {
	      xnee_verbose((xd, "Could not open resource file %s\n", xd->sf_name));
	      xd->sf_name = (char *) malloc ( strlen(argv[i]) + strlen (XNEE_RESOURCE_DIR) + 2 );
	      strcpy ( xd->sf_name , XNEE_RESOURCE_DIR);
	      strcat ( xd->sf_name , "/");
	      strcat ( xd->sf_name , argv[i]);
	      xnee_verbose((xd, "\ttryingresource file %s\n", xd->sf_name));
	      xd->sf = fopen (xd->sf_name,"r");
	    }
	  if ( xd->sf != NULL) 
	    {
	      xnee_add_resource (xd );
	    }
	  else
	    {
	      xnee_print_error ("Unable to open resource file\n");
	    }
	}
      else if(xnee_check(argv[i], "--plugin", "-p" )) 
	{
	  if (++i >= argc)
	    {
	      xnee_usage(stderr);
	      xnee_close_down(xd);
	      exit(XNEE_WRONG_PARAMS);
	    }
	  if ( xnee_use_plugin(xd, argv[i]) != 0 )
	    {
	      xnee_print_error ("Unable to open plugin file (%s)\n", argv[i]);
	    }
	}
      else if(xnee_check(argv[i], "--all-clients", "-ac" )) 
	{
	  xd->all_clients = True;
	}
      else if(xnee_check(argv[i], "--future-clients", "-fc" )) 
	{
	  xd->all_clients = False;
	  continue; 
	}
      else if(xnee_check(argv[i], "--record", "-rec" )) 
	{
	  xd->recorder = True;
	  continue; 
	}
      else if(xnee_check(argv[i], "--no-sync", "-ns" )) 
	{
	  xd->sync = False;
	  continue; 
	}
      else if(xnee_check(argv[i], "--force-replay", "-fp" )) 
	{
	  xd->force_replay = True;
	  continue; 
	}
      else if(xnee_check(argv[i], "--replay", "-rep" )) 
	{
	  xd->recorder = False;
	  continue; 
	}
      else if(xnee_check(argv[i], "--everything", "--everything")) 
	{
	  xd->xnee_info->everything = True;
	  continue;
	}
      else if(xnee_check(argv[i], "--all-events", "-ae" )) 
	{
	  xd->xnee_info->all_events = True;
	  continue;
	}
      else if(xnee_check(argv[i], "--k-log", "--k-log")) 
	{
	  xd->xnee_info->loops_left = 1000;
	  continue;
	}
      else if(xnee_check(argv[i], "--10k-log", "--10k-log")) 
	{
	  xd->xnee_info->loops_left = 10000;
	  continue;
	}
      else if(xnee_check(argv[i], "--100k-log", "--100k-log")) 
	{
	  xd->xnee_info->loops_left = 100000;
	  continue;
	}
      else if(xnee_check(argv[i], "--print-settings", "-ps")) 
	{
	  xnee_print_xnee_settings (xd, xd->err) ;
	  xnee_record_print_record_range (xd, xd->err) ;
	  fprintf (stderr, "Press enter to continue:\n");
	  getchar();
	  continue;
	}
      else if(xnee_check(argv[i], "--m-log", "--m-log")) 
	{
	  xd->xnee_info->loops_left = 1000000;
	  continue;
	}
      else if(xnee_check(argv[i], "--first-last", "-fl")) 
	{
	  xd->xnee_info->first_last = True;
	  continue;
	}
      else if(xnee_check(argv[i], "--no-expose", "-ne")) 
	{
	  xd->xnee_info->no_expose = True;
	  continue;
	}
      else if(xnee_check(argv[i], "--out", "-o" )) 
	{
	  if (++i >= argc)
	    {
	      xnee_usage(stderr);
	      xnee_close_down(xd);
	      exit(XNEE_WRONG_PARAMS);
	    }
	  /*	
	   *  xd->std_out = True ;
	   */
	  xd->out_name = (char *) malloc ( strlen(argv[i])+1 );
	  xd->out_name = argv[i];
	  xd->out = fopen (argv[i],"w");
	  continue; 
	}
      else if(xnee_check(argv[i], "--loops", "-l")) 
	{
	  xd->xnee_info->loops_left = atoi(argv[++i]) ;
	}
      else if(xnee_check(argv[i], "--stop-key", "-sk")) 
	{
	  xnee_add_stop_key (xd, argv[++i]);
	}
      else if(xnee_check(argv[i], "--time", "-t")) 
	{
	  xd->xnee_info->interval = atoi(argv[++i]) ;
	  if (xd->xnee_info->interval > XNEE_MAX_DELAY ) 
	    {
	      fprintf (stderr, "Interval is more than %d seconds\n", XNEE_MAX_DELAY);
	      xnee_usage(stderr); 
	      xnee_close_down(xd);
	      exit(XNEE_WRONG_PARAMS);
	    }
	  else if ( xd->xnee_info->interval <= 0 ) 
	    {
	      fprintf (stderr, "Interval is less than or equal to 0\n");
	      xnee_usage(stderr); 
	      xnee_close_down(xd);
	      exit (XNEE_WRONG_PARAMS);
	    }
	  /*	  xnee_verbose((xd, "Setting up timer ---> \n"));
	  setup_timer(xd, xd->xnee_info->interval);
	  xnee_verbose((xd, "Setting up timer <--- \n"));
	  */
	  continue;
	}
      else if(xnee_check(argv[i], "--binary", "-b")) 
	{
	  /* xnee_info->no_expose = True; */
	  fprintf (stderr, "********* OBSOLETE OPTION **************\n\n");
	  fprintf (stderr, "*** Binary printout will not be      *** \n");
	  fprintf (stderr, "*** implemented                      *** \n");
	}
      else if(xnee_check(argv[i], "--manpage", "--manpage")) 
	{
	  xnee_manpage (stdout);
	  exit(XNEE_OK);
	}
      else if(xnee_check(argv[i], "--help", "-h")) 
	{
	  xnee_usage (stdout);
	  exit(XNEE_OK);
	}
      else if(xnee_check(argv[i], "--flags", "-flags")) 
	{
	  xnee_flags (stdout);
	  exit(XNEE_OK);
	}
      else if(xnee_check(argv[i], "--human-printout", "-hp")) 
	{
	  xd->rec_callback = xnee_human_dispatch;
	  continue;
	}
      else if(xnee_check(argv[i], "--print-event-names", "-pens")) 
	{
	  int ret =
	    xnee_print_event_info (xd) ;
	  exit(ret);
	}
      else if(xnee_check(argv[i], "--print-event-name", "-pen")) 
	{
	  char *ev;
	  int ev_nr;
	  if ( (argv[i+1][0] >= '0') && (argv[i+1][0] <= '9') )
	    {
	      ev_nr=atoi(argv[i+1]);
	      ev = (char*) xnee_int2event (ev_nr);
	      if (ev==NULL)
		{
		  exit (XNEE_WRONG_PARAMS);
		}
	      else
		{
		  fprintf (stdout, "%s\n", ev);
		}
	    }
	  else
	    {
	      ev = argv[i+1]; 
	      ev_nr = xnee_event2int (ev);
	      if ( ev_nr == -1)
		{
		  exit (XNEE_WRONG_PARAMS);
		}
	      else
		{
		  fprintf (stdout, "%d\n", ev_nr);
		}
	    }
	  exit(XNEE_OK);
	}
      else if(xnee_check(argv[i], "--print-error-name", "-pern")) 
	{
	  xnee_print_error_info (xd) ;
	  exit(XNEE_OK);
	}
      else if(xnee_check(argv[i], "--print-request-names", "-prns")) 
	{
	  xnee_print_request_info (xd) ;
	  exit(XNEE_OK);
	}
      else if(xnee_check(argv[i], "--print-request-name", "-prn")) 
	{
	  char *req;
	  int req_nr;
	  if ( (argv[i+1][0] >= '0') && (argv[i+1][0] <= '9') )
	    {
	      req_nr=atoi(argv[i+1]);
	      req = (char*)  xnee_int2request (req_nr);
	      if (req==NULL)
		{
		  exit (XNEE_WRONG_PARAMS);
		}
	      else
		{
		  fprintf (stdout, "%s\n", req);
		}
	    }
	  else
	    {
	      req = argv[i+1]; 
	      req_nr = xnee_request2int (req);
	      if ( req_nr == -1)
		{
		  exit (XNEE_WRONG_PARAMS);
		}
	      else
		{
		  fprintf (stdout, "%d\n", req_nr);
		}
	    }
	  exit(XNEE_OK);
	}
      else if(xnee_check(argv[i], "--print-reply-name", "-pren")) 
	{
	  xnee_print_reply_info (xd) ;
	  exit(XNEE_OK);
	}
      else if(xnee_check(argv[i], "--print-data-name", "-pdn")) 
	{
	  xnee_print_data_info (xd) ;
	  exit(XNEE_OK);
	}
      else if(xnee_check(argv[i], "--verbose", "-v")) 
	{
	  xd->verbose = True;
	  continue;
	}
      else if(xnee_check(argv[i], "--buffer-verbose", "-bv")) 
	{
	  xd->buf_verbose = True;
	  continue;
	}
      else if(xnee_check(argv[i], "--delivered-event-range", "-devra")) 
	{
	  ret=xnee_parse_range (xd, XNEE_DELIVERED_EVENT, argv[++i]);
	  continue;
	}
      else if(xnee_check(argv[i], "--device-event-range", "-devera")) 
	{
	  ret=xnee_parse_range (xd, XNEE_DEVICE_EVENT, argv[++i]);
	  continue;
	}
      else if(xnee_check(argv[i], "--error-range", "-erra")) 
	{
	  ret=xnee_parse_range (xd, XNEE_ERROR, argv[++i]);
	  continue;
	}
      else if(xnee_check(argv[i], "--request-range", "-reqra")) 
	{
	  ret=xnee_parse_range (xd, XNEE_REQUEST, argv[++i]);
	  continue;
	}
      else if(xnee_check(argv[i], "--reply-range", "-repra")) 
	{
	  ret=xnee_parse_range (xd, XNEE_REPLY, argv[++i]);
	  continue;
	}
      else if(xnee_check(argv[i], "--extension-request-major-range", "-erqmar")) 
	{
	  ret=xnee_parse_range (xd, XNEE_EXT_REQUEST_MAJOR, argv[++i]);
	  continue;
	}
      else if(xnee_check(argv[i], "--extension-request-manor-range", "-erqmir")) 
	{
	  ret=xnee_parse_range (xd, XNEE_EXT_REQUEST_MINOR, argv[++i]);
	  continue;
	}
      else if(xnee_check(argv[i], "--extension-reply-major-range", "-erpmar")) 
	{
	  ret=xnee_parse_range (xd, XNEE_EXT_REPLY_MAJOR, argv[++i]);
	  continue;
	}
      else if(xnee_check(argv[i], "--extension-reply-mainor-range", "-erpmir")) 
	{
	  ret=xnee_parse_range (xd, XNEE_EXT_REPLY_MINOR, argv[++i]);
	  continue;
	}
      else if(xnee_check(argv[i], "--version", "-V")) 
	{
	  xnee_version(xd);
	  exit(0);
	}
      else if(xnee_check(argv[i], "--distribute", "-di")) 
	{
	  xnee_verbose((xd, "--distribute found \"%s\"\n", argv[i+1]));
	  xnee_add_display_list ( xd, argv[++i]);
	  xnee_verbose((xd, "--distribute found \"%s\" finished \n", argv[i+1]));
	}
      else if ( xnee_check (argv[i], "--file", "-f"))
        {
          if (++i >= argc)
	    {
	      xnee_usage(stderr);
	      xnee_close_down(xd);
	      exit(XNEE_FILE_NOT_FOUND);
	    }
          xd->data_file = fopen (argv[i],"rw");
	  xnee_verbose((xd, "Reading replay data from %s (%d))\n", 
			argv[i], xd->data_file));
          continue;
        }
      else if(xnee_check(argv[i], "--all-clients", "-ac" )) 
	{
	  xd->all_clients = True;
	  continue; 
	}
      else if(xnee_check(argv[i], "--future-clients", "-fc" )) 
	{
	  xd->all_clients = False;
	  continue; 
	}
      else if(xnee_check(argv[i], "--record", "-rec" )) 
	{
	  xd->recorder = True;
	  continue; 
	}
      else if(xnee_check(argv[i], "--no-sync", "-ns" )) 
	{
	  xd->sync = False;
	  continue; 
	}
      else if(xnee_check(argv[i], "--replay", "-rep" )) 
	{
	  xd->recorder = False;
	  continue; 
	}
      else if(xnee_check(argv[i], "--everything", "--everything")) 
	{
	  xd->xnee_info->everything = True;
	  continue;
	}
      /* 
       * TO KEEP COMPATIBILITY WITH OLD 
       * - SCRIPTS USING XNEE
       * - XNEE PLUGINS
       * THE OLD SYNTAX IS ALLOWED 
       * ... UNTIL ???  
       */
      /* START OF OBSOLETE PARSING */
      else if(xnee_check(argv[i], "--all_events", "-ae" )) 
	{
	  xnee_print_obsolete_mess ( ("OBSOLETE: --all_events\nUSE:--all-events\n"));
	  xd->xnee_info->all_events = True;
	  continue;
	}
      else if(xnee_check(argv[i], "--k_log", "--k_log")) 
	{
	  xnee_print_obsolete_mess ( ("OBSOLETE: --k_log\nUSE:--k_log\n"));
	  xd->xnee_info->loops_left = 1000;
	  continue;
	}
      else if(xnee_check(argv[i], "--10k_log", "--10k_log")) 
	{
	  xnee_print_obsolete_mess ( ("OBSOLETE: --10k_log\nUSE:--10k-log\n"));
	  xd->xnee_info->loops_left = 10000;
	  continue;
	}
      else if(xnee_check(argv[i], "--100k_log", "--100k_log")) 
	{
	  xnee_print_obsolete_mess ( ("OBSOLETE: --100k_log\nUSE:--100k-log\n"));
	  xd->xnee_info->loops_left = 100000;
	  continue;
	}
      else if(xnee_check(argv[i], "--print_settings", "-ps")) 
	{
	  xnee_print_obsolete_mess ( ("OBSOLETE: --print_settings\nUSE:--print-settings\n"));
	  xnee_print_xnee_settings (xd, xd->err) ;
	  xnee_record_print_record_range (xd, xd->err) ;
	  fprintf (stderr, "Press enter to continue:\n");
	  getchar();
	  continue;
	}
      else if(xnee_check(argv[i], "--m_log", "--m_log")) 
	{
	  xnee_print_obsolete_mess ( ("OBSOLETE: --m_log\nUSE:--m-log\n"));
	  xd->xnee_info->loops_left = 1000000;
	  continue;
	}
      else if(xnee_check(argv[i], "--first_last", "-fl")) 
	{
	  xnee_print_obsolete_mess ( ("OBSOLETE: --first_last\nUSE:--first-last\n"));
	  xd->xnee_info->first_last = True;
	  continue;
	}
      else if(xnee_check(argv[i], "--no_expose", "-ne")) 
	{
	  xnee_print_obsolete_mess ( ("OBSOLETE: --no_expose\nUSE:--no-expose\n"));
	  xd->xnee_info->no_expose = True;
	  continue;
	}
      else if(xnee_check(argv[i], "--stop_key", "-sk")) 
	{
	  xnee_print_obsolete_mess ( ("OBSOLETE: --stop_key\nUSE:--stop-key\n"));
	  xnee_add_stop_key (xd, argv[++i]);
	}
      else if(xnee_check(argv[i], "--human_printout", "-hp")) 
	{
	  xnee_print_obsolete_mess ( ("OBSOLETE: --human_printout\nUSE:--human-printout\n"));
	  xd->rec_callback = xnee_human_dispatch;
	  continue;
	}
      else if(xnee_check(argv[i], "--delivered_event_range", "-devra")) 
	{
	  xnee_print_obsolete_mess ( ("OBSOLETE: --delivered_event_range\nUSE:--delivered-event-range\n"));
	  ret=xnee_parse_range (xd, XNEE_DELIVERED_EVENT, argv[++i]);
	  continue;
	}
      else if(xnee_check(argv[i], "--device_event_range", "-devera")) 
	{
	  xnee_print_obsolete_mess ( ("OBSOLETE: --device_event_range\nUSE:--device-event-range\n"));
	  ret=xnee_parse_range (xd, XNEE_DEVICE_EVENT, argv[++i]);
	  continue;
	}
      else if(xnee_check(argv[i], "--error_range", "-erra")) 
	{
	  xnee_print_obsolete_mess ( ("OBSOLETE: --error_range\nUSE:--error-range\n"));
	  ret=xnee_parse_range (xd, XNEE_ERROR, argv[++i]);
	  continue;
	}
      else if(xnee_check(argv[i], "--request_range", "-reqra")) 
	{
	  xnee_print_obsolete_mess ( ("OBSOLETE: --request_range\nUSE:--request-range\n"));
	  ret=xnee_parse_range (xd, XNEE_REQUEST, argv[++i]);
	  continue;
	}
      else if(xnee_check(argv[i], "--reply_range", "-repra")) 
	{
	  xnee_print_obsolete_mess ( ("OBSOLETE: --reply_range\nUSE:--reply-range\n"));
	  ret=xnee_parse_range (xd, XNEE_REPLY, argv[++i]);
	  continue;
	}
      else if(xnee_check(argv[i], "--extension_request_major_range", "-erqmar")) 
	{
	  xnee_print_obsolete_mess ( ("OBSOLETE: --extension_request_major_range\nUSE:--extension-request-major-range\n"));
	  ret=xnee_parse_range (xd, XNEE_EXT_REQUEST_MAJOR, argv[++i]);
	  continue;
	}
      else if(xnee_check(argv[i], "--extension_request_minor_range", "-erqmir")) 
	{
	  xnee_print_obsolete_mess ( ("OBSOLETE: --extension_request_minor_range\nUSE:--extension-request-minor-range\n"));
	  ret=xnee_parse_range (xd, XNEE_EXT_REQUEST_MINOR, argv[++i]);
	  continue;
	}
      else if(xnee_check(argv[i], "--extension_reply_major_range", "-erpmar")) 
	{
	  xnee_print_obsolete_mess ( ("OBSOLETE: --extension_reply_major_range\nUSE:--extension-reply-major-range\n"));
	  ret=xnee_parse_range (xd, XNEE_EXT_REPLY_MAJOR, argv[++i]);
	  continue;
	}
      else if(xnee_check(argv[i], "--extension_reply_minor_range", "-erpmir")) 
	{
	  xnee_print_obsolete_mess ( ("OBSOLETE: --extension_reply_minor_range\nUSE:--extension-reply-minor-range\n"));
	  ret=xnee_parse_range (xd, XNEE_EXT_REPLY_MINOR, argv[++i]);
	  continue;
	}
      /* END OF OBSOLETE PARSING */
      else 
	{
	  fprintf (stderr, "Could not parse \"%s\"\n", argv[i]);
	  xnee_usage(stderr);
	  exit(XNEE_WRONG_PARAMS);
	}
      if (ret!=0) 
	{
	  xnee_print_error ("Could not parse %s\n", argv[i]);
	  exit (ret);
	}
    }
  return;
}




/**************************************************************
 *                                                            *
 * xnee_flags                                                 *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_flags (FILE *fd)
{
  char **cpp;
  for (cpp = help; *cpp; cpp+=2) 
    {
      fprintf (fd, "%s\n", *cpp);
    }
}

/**************************************************************
 *                                                            *
 * xnee_usage                                                 *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_usage (FILE *fd)
{
  char **cpp;
  char *command;
  char *descr;

  fprintf (fd ,"USAGE: %s [OPTIONS]\n", PACKAGE );
  fprintf (fd ,"\n");
  fprintf (fd ,"DESCRIPTION\n");
  
  for (cpp = description; *cpp; cpp++) 
    {
      fprintf (fd, "%s\n", *cpp);
    }

  fprintf (fd ,"OPTIONS\n");
  fprintf (fd ,"\n");
  for (cpp = help; *cpp; cpp++) 
    {
      fprintf (fd, "  %s\n", *cpp++);
      fprintf (fd, "\t%s\n\n", *cpp);
    }

  fprintf (fd ,"COMMENTS\n");
  for (cpp = explain; *cpp; cpp++) 
    {
      fprintf (fd, "  %s\n", *cpp++);
      fprintf (fd, "\t%s\n", *cpp);
    }
  fprintf (fd ,"\n");

  fprintf (fd ,"EXAMPLES\n");
  fprintf (fd ,"\n");
  for (cpp = examples; *cpp; cpp++) 
    {
      command=*cpp++;
      descr=*cpp;
      if (command != NULL )
	{
	  if (descr != NULL )
	    fprintf (fd, "  %s\n", command);
	  else
	    {
	      fprintf (fd, "  %s\n", command);
	      break;
	    }
	  
	  if (descr!=NULL)
	      fprintf (fd, "\t%s\n\n", *cpp);
	  else 
	    ;
	}
      else 
	;
    }
  fprintf (fd, "\n  Report bugs to %s\n", XNEE_MAIL);

}


/**************************************************************
 *                                                            *
 * xnee_manpage                                               *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_manpage (FILE *fd)
{
  char **cpp;
  char *command;
  char *descr;

  fprintf (fd ,".\\\" Copyright Henrik Sandklef 2002.\n");
  fprintf (fd ,".\\\"   \n");
  fprintf (fd ,".\\\" This file may be copied under the conditions described \n");
  fprintf (fd ,".\\\" in the LDP GENERAL PUBLIC LICENSE, Version 1, September 1998 \n");
  fprintf (fd ,".\\\" that should have been distributed together with this file. \n");
  fprintf (fd ,".\\\" \n");
  fprintf (fd ,".TH \"XNEE\" 1 2002 \"" PACKAGE"\" \n");
  fprintf (fd ,".SH \"NAME\" \n");
  fprintf (fd ,"xnee record, replays or distributes X11 data \n");
  fprintf (fd ,".SH \"SYNOPSIS\" \n");
  fprintf (fd ,".BI \"xnee [\" options \"]  \n");
  fprintf (fd ,".sp \n");
  fprintf (fd ,".SH \"DESCRIPTION\" \n");
  fprintf (fd ,"The program \n");
  fprintf (fd ,".B Xnee \n");
  fprintf (fd ,"can record and replay an X session. Xnee also has the ability to distribute events to multiple displays. \n");
  fprintf (fd ,".B Xnee  \n");
  fprintf (fd ,"gets copies of X protocol data from the X server. These are either printed to file ( \n");
  fprintf (fd ,".I record \n");
  fprintf (fd ,"mode) or replayed and synchronised ( \n");
  fprintf (fd ,".I replay  \n");
  fprintf (fd ,"mode). \n");
  fprintf (fd ,"During record and replay Xnee can distribute the record/replayed events to multiple displays. \n");
  fprintf (fd ,".SH \"OPTIONS\" \n");

  for (cpp = help; *cpp; cpp++) 
    {
      fprintf (fd, ".TP\n.B \"%s\"\n", *cpp++);
      fprintf (fd, "%s\n", *cpp);
    }

  fprintf (fd ,".SH \"COMMENTS\"\n");
  for (cpp = explain; *cpp; cpp++) 
    {
      fprintf (fd, ".TP\n.B \"%s\"\n", *cpp++);
      fprintf (fd, "%s\n", *cpp);
    }
  fprintf (fd ,"\n");


  fprintf (fd, ".SH \"EXAMPLES\"\n");
  for (cpp = examples; *cpp; cpp++) 
    {
      command=*cpp++;
      descr=*cpp;
      if (command != NULL )
	{
	  if (descr != NULL )
	    fprintf (fd, ".TP\n.B %s\n", command);
	  else
	    {
	      fprintf (fd, ".TP\n.B  %s\n", command);
	      break;
	    }
	  
	  if (descr!=NULL)
	      fprintf (fd, "\n%s\n", *cpp);
	  else 
	    ;
	}
      else 
	;
    }

  fprintf (fd ,".SH \"AUTHOR\" \n");
  fprintf (fd ,"Henrik Sandklef.\n");

  fprintf (fd ,".SH \"REPORTING BUGS\"\n");
  fprintf (fd ,"Report bugs in the program to "XNEE_BUG_MAIL". \n");

  fprintf (fd ,".SH \"COPYRIGHT\"\n");
  fprintf (fd ,"Copyright (C) 1999,2000,2001,2002,2003 Henrik Sandklef.\n");
  fprintf (fd ,"This  is  free  software;  see the source for copying conditions. ");
  fprintf (fd ,"There is NO warranty;");
  fprintf (fd ,"not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");

  fprintf (fd ,".SH \"SEE ALSO\"\n"); 
  fprintf (fd ,".BR xev (1)\n");

  fprintf (fd ,".SH \"VERSION\"\n");
  fprintf (fd ,PACKAGE " man page, version " VERSION "\n");

  fprintf (fd ,".SH \"NOTES\"\n");
  fprintf (fd ,"This page describes\n");
  fprintf (fd ,".B Xnee.\n");
  fprintf (fd ,"Mail corrections and additions to XNEE_DEV_MAIL.\n");
}


