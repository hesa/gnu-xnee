/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *        Copyright (C) 1999, 2000, 2001, 2002, 2003 
 *                      2004, 2005, 2006 Henrik Sandklef
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

#include "cnee_printer.h"

extern xnee_options_t *cnee_options;
extern xnee_options_t *xnee_options;

static char *explain[] = {
  "file_name" , "Name of a file (e.g /tmp/user1_session)" ,
  "display_name" , "Name of a display (e.g 192.168.1.12:0" ,
  "secs" , "Seconds (e.g 10)" ,
  "n" , "Integer number (e.g 100)" ,
  "LIST" , "Comma separated list of display (e.g 192.168.1.12:0,129.12.23.0:0) ",
  "X_LIST" , "Comma separated list of integers (e.g 12-23,35,45-56) " ,
  NULL 
};


static char *examples[] = {
  "" XNEE_CLI " --record --events-to-record 1000 -devera 2-6 -o /tmp/xnee.xns -e /tmp/xnee.log -v ", 
  "Writes 1000 data to file /tmp/xnee.xns and dumps the verbose printout to /tmp/xnee.log",
  "" XNEE_CLI " --replay -f /tmp/xnee.xns -v -e /tmp/xnee.log --no-sync",
  "Read data from /tmp/xnee.xns, replay it and verbose print to file /tmp/xnee.log",
  "For more examples, read the Xnee manual",
  NULL 
};


static char *description[] = {
  "  " XNEE_CLI " (part of the " PACKAGE " project) can record and replay an X session. " XNEE_CLI " also has the ability to distribute events to multiple displays.\n",
  NULL 
};



/**************************************************************
 *                                                            *
 * xnee_flags                                                 *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_flags (FILE *fd)
{
  int i ;
  printf ("%s    %s:%d\n",
	  __func__, __FILE__, __LINE__);
  for (i = 0; i<xnee_options->nr_of_options; i++) 
    {
      fprintf (fd, "--%s\n",xnee_options->options[i]->option );
    }
  for (i = 0; i<cnee_options->nr_of_options; i++) 
    {
      fprintf (fd, "--%s\n",cnee_options->options[i]->option );
    }
}

static void
xnee_usage_printer(FILE *fd, int tmp_size, char *pre, char *str)
{
  #define USAGE_PRINT_LINE_LEN 256
  char line_buf[USAGE_PRINT_LINE_LEN];

  char *tmp = str;
  int   len = strlen(str);
  int   ret ;
  int   size = tmp_size;

  if (tmp_size>USAGE_PRINT_LINE_LEN)
    {
      size = USAGE_PRINT_LINE_LEN;
    }
  

  while (len>0)
    {
      ret = snprintf (line_buf, size, str);
      printf ("%s %s\n", pre, line_buf);
/*       printf ("** '%s'    '%s'\n", line_buf, str); */
/*       printf (" len =%d   ret=%d   str'%s'\n", */
/* 	      len, ret, str); */
      len = len -  size; 
      str += size - 1;
    }


}

static void
xnee_usage_sub(FILE *fd, xnee_options_t *options, int type)
{
  int i ; 
  char *descr;
  char *option_long;
  char *option_short;
  char *option_arg;
  char *option_descr;
  char *empty_str="";
  char *dash_str=",-";
  #define USAGE_DESCR_SIZE 256
  char line_buf[USAGE_DESCR_SIZE];

  #define SHORT_PREFIX(a) (a?dash_str:empty_str)
  #define EMPTY_IF_NULL(a) (a?a:empty_str)

  for (i = 0; i<options->nr_of_options; i++) 
    {
      if ( (options->options[i]->type==type)
	   ||
	   (type==XNEE_ANY_OPTION) )
	{
	  option_long  = options->options[i]->option ;
	  option_short = options->options[i]->short_option ;
	  option_arg   = options->options[i]->args ;
	  option_descr = options->options[i]->description ;
	  fprintf (fd, "\t--%s%s%s %s\n",
		   EMPTY_IF_NULL(option_long),
		   SHORT_PREFIX(option_short),
		   EMPTY_IF_NULL(option_short),
		   EMPTY_IF_NULL(option_arg)
		   );
	  snprintf (line_buf, USAGE_DESCR_SIZE,
		    "%s\n\n",EMPTY_IF_NULL(option_descr) );
	  xnee_usage_printer(fd, 65, "\t  ", line_buf);
	}
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
  int i ;


  fprintf (fd ,"USAGE: %s [OPTIONS]\n", XNEE_CLI );
  fprintf (fd ,"\n");
  fprintf (fd ,"DESCRIPTION\n");
  
  for (cpp = description; *cpp; cpp++) 
    {
      fprintf (fd, "%s\n", *cpp);
    }

  fprintf (fd ,"GENERAL OPTIONS\n\n");
  xnee_usage_sub(fd, xnee_options, XNEE_GENERAL_OPTION);

  fprintf (fd ,"RECORD OPTIONS\n\n");
  xnee_usage_sub(fd, xnee_options, XNEE_RECORD_OPTION);

  fprintf (fd ,"REPLAY OPTIONS\n\n");
  xnee_usage_sub(fd, xnee_options, XNEE_REPLAY_OPTION);

  fprintf (fd ,"CNEE OPTIONS\n\n");
  xnee_usage_sub(fd, cnee_options, XNEE_ANY_OPTION);




#ifdef EINAR
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
	}
    }
#endif /* EINAR */
  fprintf (fd, "\n  Report bugs to %s\n", XNEE_BUG_MAIL);
  
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

  fprintf (fd ,".\\\" Copyright Henrik Sandklef 2002, 2003, 2004, 2005, 2006\n");
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

#ifdef EINAR

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
	}
    }

#endif /* EINAR */
  fprintf (fd ,".SH \"AUTHOR\" \n");
  fprintf (fd ,"Henrik Sandklef.\n");

  fprintf (fd ,".SH \"REPORTING BUGS\"\n");
  fprintf (fd ,"Report bugs in the program to %s. \n", XNEE_BUG_MAIL);

  fprintf (fd ,".SH \"COPYRIGHT\"\n");
  fprintf (fd ,"Copyright (C) 2002 Henrik Sandklef.\n");
  fprintf (fd ,"This  is  free  software;  see the source for copying conditions. ");
  fprintf (fd ,"There is NO warranty;");
  fprintf (fd ,"not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");

  fprintf (fd ,".SH \"SEE ALSO\"\n"); 
  fprintf (fd ,".BR xnee(info), xev (1)\n");

  fprintf (fd ,".SH \"VERSION\"\n");
  fprintf (fd ,PACKAGE " man page, version " VERSION "\n");

  fprintf (fd ,".SH \"NOTES\"\n");
  fprintf (fd ,"This page describes\n");
  fprintf (fd ,".B Xnee.\n");
  fprintf (fd ,"Mail corrections and additions to %s\n", XNEE_BUG_MAIL );
}

/**************************************************************
 *                                                            *
 * xnee_infopage                                              *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_infopage (FILE *fd)
{
  char **cpp;
  char *command;
  char *descr;

  fprintf (fd ,"\\input texinfo  @c -*- Texinfo -*-\n");
  /* Thanks Peter for you remark on %% */
  fprintf (fd ,"@c %%**start of header\n");
  fprintf (fd ,"@setfilename "XNEE_CLI".info\n");
  fprintf (fd ,"@set EDITION %s\n", VERSION);
  fprintf (fd ,"@set VERSION %s\n", VERSION);
  fprintf (fd ,"@settitle " XNEE_CLI " Manual \n");
  fprintf (fd ,"@setchapternewpage off\n");
  fprintf (fd ,"@c %%**end o\n");



  fprintf (fd, "@ifinfo\n");
  fprintf (fd, "@node Top, Options,, (dir)\n");
  fprintf (fd, "@top \n");
/*   fprintf (fd, "@include xnee_copying\n"); */
  fprintf (fd, "@end ifinfo\n");

#ifdef EINAR
  for (cpp = description; *cpp; cpp++) 
    {
      fprintf (fd, "%s\n", *cpp);
    }
  fprintf (fd, "Current version of " PACKAGE " is " VERSION "\n");



  fprintf (fd, "@menu\n");
  fprintf (fd, "* Options::        Options\n");
  fprintf (fd, "* Examples::       Examples\n");
  fprintf (fd, "* Bugs::           Bugs\n");
  fprintf (fd, "* See also::       See also\n");
  fprintf (fd, "@end menu\n");

  fprintf (fd, "@contents\n");
  fprintf (fd, "\n");
  fprintf (fd ,"Copyright (C) 2002, 2003, 2004, 2005, 2006 Henrik Sandklef.\n");
  fprintf (fd, "@*\n");
  fprintf (fd, "@*\n");
  fprintf (fd ,"This  is  free  software;  see the source for copying conditions. ");
  fprintf (fd ,"There is NO warranty;");
  fprintf (fd ,"not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");


  fprintf (fd, "@node Options, Examples, top, top \n");

  for (cpp = help; *cpp; cpp++) 
    {
      fprintf (fd, "@code{  %s}\n", *cpp++);
      fprintf (fd, "%s\n\n", *cpp);
    }

  for (cpp = explain; *cpp; cpp++) 
    {
      fprintf (fd, "%s\n", *cpp++);
      fprintf (fd, "\t%s\n", *cpp);
    }


  fprintf (fd, "@node Examples, Bugs, Options, top \n");
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
	}
    }
  fprintf (fd ,"\n");
#endif /* EINAR */
  fprintf (fd, "@node Bugs, See also, Examples,top \n");
  fprintf (fd, "If you encounter a bug, report it to ");
  fprintf (fd, "@indent @ @ @ @email{xnee-bug@@gnu.org}\n@ \n");
  fprintf (fd, "@indent or go to the Xnee site:\n");
  fprintf (fd, "@indent @ @ @ @url{http://svannah.gnu.org}\n@\n");
  fprintf (fd, "@indent Before reporting the bug, make sure it is not already reported\n");

  fprintf (fd, "@node See also, (dir), Bugs,top \n");
  fprintf (fd ,"There are some other interesting programs out there\n@*\n@*\n");
  fprintf (fd ,"Look at xev (1)\n@*\n");
  fprintf (fd ,"Also make sure to read the Xnee manual or info page\n");


  fprintf (fd, "@bye\n");
  return;
}
