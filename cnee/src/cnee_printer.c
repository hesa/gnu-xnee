/*****
 *       Xnee's Not an Event Emulator                                
 *                                                                   
 * Xnee enables recording and replaying of X protocol data           
 *                                                                   
 *  Copyright (C) 1999-2007, 2009, 2011, 2014 Henrik Sandklef
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

#include "cnee.h"
#include "cnee_printer.h"
#include "cnee_strings.h"
#include "libxnee/xnee_resource.h"

#define CNEE_TEXT_LENGTH 65
#define CNEE_PRINT_INFO 1
#define CNEE_PRINT_HELP 2
#define CNEE_PRINT_MAN  3

#define xnee_manpage_sub(fd, opt, type)  \
    xnee_print_sub_impl(fd, opt, type, CNEE_PRINT_MAN)

#define xnee_infopage_sub(fd, opt, type) \
    xnee_print_sub_impl(fd, opt, type, CNEE_PRINT_INFO)

#define xnee_usage_sub(fd, opt, type)    \
    xnee_print_sub_impl(fd, opt, type, CNEE_PRINT_HELP)



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
/*   printf ("%s    %s:%d\n", */
/* 	  __func__, __FILE__, __LINE__); */

  for (i = 0; xnee_options[i].key != XNEE_LAST_OPTION; i++) 
    {
      fprintf (fd, "--%s\n",xnee_options[i].option );
    }
  for (i = 0; cnee_options[i].key!=XNEE_LAST_OPTION; i++) 
    {
      fprintf (fd, "--%s\n",cnee_options[i].option );
    }
}

static void
xnee_usage_printer(FILE *fd, int tmp_size, char *pre, char *str)
{
  #define USAGE_PRINT_LINE_LEN 256
  char line_buf[USAGE_PRINT_LINE_LEN];

  int   len = strlen(str);
  int   size = tmp_size;

  if (tmp_size>USAGE_PRINT_LINE_LEN)
    {
      size = USAGE_PRINT_LINE_LEN;
    }
  

  while (len>0)
    {
      snprintf (line_buf, 
		size,
		"%s",
		(const char*)str);
      
      fprintf (fd, "    %s %s\n", pre, line_buf);
      len = len -  size; 
      str += size - 1;
    }


}


void 
xnee_print_sub_impl (FILE *fd, xnee_option_t *options, int type, int format )
{
  int i ; 
  char *option_long;
  char *option_short;
  char *option_arg;
  char *option_descr;
  char *dash_str=",-";
  #define USAGE_DESCR_SIZE 256
  char line_buf[USAGE_DESCR_SIZE];

  #define SHORT_PREFIX(a) (a?dash_str:XNEE_EMPTY_STRING)

  for (i = 0; options[i].key != XNEE_LAST_OPTION; i++) 
    {
      if ( (options[i].type==type)
	   ||
	   (type==XNEE_ANY_OPTION) )
	{
	  option_long  = options[i].option ;
	  option_short = options[i].short_option ;
	  option_arg   = options[i].args ;
	  option_descr = options[i].description ;

	  snprintf (line_buf, USAGE_DESCR_SIZE,
		    "%s\n\n",EMPTY_IF_NULL(option_descr) );

	  if (format == CNEE_PRINT_INFO)
	    {
	      fprintf (fd, "@code{--%s%s %s %s}\n",
		       EMPTY_IF_NULL(option_long),
		       SHORT_PREFIX(option_short),
		       EMPTY_IF_NULL(option_short),
		       EMPTY_IF_NULL(option_arg)
		       );
	      fprintf(fd, "%s", line_buf);
	    }
	  else if (format == CNEE_PRINT_HELP)
	    {
	      fprintf (fd, "\t--%s%s%s %s\n",
		       EMPTY_IF_NULL(option_long),
		       SHORT_PREFIX(option_short),
		       EMPTY_IF_NULL(option_short),
		       EMPTY_IF_NULL(option_arg)
		       );
	      snprintf (line_buf, USAGE_DESCR_SIZE,
			"%s\n",EMPTY_IF_NULL(option_descr) );
	      xnee_usage_printer(fd, CNEE_TEXT_LENGTH, "\t", line_buf);
	    }
	  else if (format == CNEE_PRINT_MAN)
	    {
	      fprintf (fd, ".TP\n.B \"--%s%s %s %s\"",
		       EMPTY_IF_NULL(option_long),
		       SHORT_PREFIX(option_short),
		       EMPTY_IF_NULL(option_short),
		       EMPTY_IF_NULL(option_arg)
		       );
	      fprintf (fd, "\n%s\n",EMPTY_IF_NULL(option_descr) );
	    }

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
  char *descr;

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

  fprintf (fd ,"RETYPE OPTIONS\n\n");
  xnee_usage_sub(fd, xnee_options, XNEE_RETYPE_OPTION);

  fprintf (fd ,"GRAB OPTIONS\n\n");
  xnee_usage_sub(fd, xnee_options, XNEE_GRAB_OPTION);

  fprintf (fd ,"CNEE OPTIONS\n\n");
  xnee_usage_sub(fd, cnee_options, XNEE_ANY_OPTION);

  fprintf (fd ,"COMMENTS\n");
  for (cpp = explain; *cpp; cpp++) 
    {
      xnee_usage_printer(fd, CNEE_TEXT_LENGTH, "\t", *cpp++);
      xnee_usage_printer(fd, CNEE_TEXT_LENGTH, "\t   ", *cpp);
/*       fprintf (fd, "\t%s\n", *cpp++); */
/*       fprintf (fd, "\t   %s\n", *cpp); */
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
	    {
	      xnee_usage_printer(fd, CNEE_TEXT_LENGTH, "\t", command);
	    }
	  else
	    {
	      xnee_usage_printer(fd, CNEE_TEXT_LENGTH, "\t", command);
	      break;
	    }
	  
	  if (descr!=NULL)
	    {
	      xnee_usage_printer(fd, CNEE_TEXT_LENGTH, "\t  ", *cpp);
	      fprintf (fd, "\n\n");
	    }
	}
    }
  fprintf (fd, "\nAUTHORS\n");
  fprintf (fd, "\t%s\n\n", XNEE_AUTHORS);

  fprintf (fd, "\nREPORTING BUGS\n");
  fprintf (fd, "\tReport bugs to %s\n\n", XNEE_BUG_MAIL);
  
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

  fprintf (fd ,".\\\" Copyright Henrik Sandklef 2002, 2003, 2004, 2005, 2006, 2007\n");
  fprintf (fd ,".\\\"   \n");
  fprintf (fd, ".\\\" This file is part of GNU Xnee.\n");
  fprintf (fd, ".\\\"\n");
  fprintf (fd, ".\\\" GNU Xnee is free software; you can redistribute it and/or modify\n");
  fprintf (fd, ".\\\" it under the terms of the GNU General Public License as published by\n");
  fprintf (fd, ".\\\" the Free Software Foundation; either version 3, or (at your option)\n");
  fprintf (fd, ".\\\" any later version.\n");
  fprintf (fd, ".\\\"\n");
  fprintf (fd, ".\\\" GNU Xnee is distributed in the hope that it will be useful,\n");
  fprintf (fd, ".\\\" but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
  fprintf (fd, ".\\\" MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
  fprintf (fd, ".\\\" GNU General Public License for more details.\n");
  fprintf (fd, ".\\\"\n");
  fprintf (fd, ".\\\" You should have received a copy of the GNU General Public License\n");
  fprintf (fd, ".\\\" along with GNU Emacs; see the file COPYING.  If not, write to the\n");
  fprintf (fd, ".\\\" Free Software Foundation, Inc., 59 Temple Place - Suite 330,\n");
  fprintf (fd, ".\\\" Boston, MA 02111-1307, USA.\n");
  fprintf (fd, ".\\\"\n");
  fprintf (fd ,".\\\" \n");
  fprintf (fd ,".TH \"" XNEE_CLI_UC "\" 1 2007 \"" PACKAGE"\" \n");
  fprintf (fd ,".SH \"NAME\" \n");
  fprintf (fd ,"%s record, replays or distributes X11 data \n", XNEE_CLI);
  fprintf (fd ,".SH \"SYNOPSIS\" \n");
  fprintf (fd ,".BI \"%s [\" options \"]  \n", XNEE_CLI);
  fprintf (fd ,".sp \n");
  fprintf (fd ,".SH \"DESCRIPTION\" \n");
  fprintf (fd ,"The program \n");
  fprintf (fd ,".B %s \n", XNEE_CLI);
  fprintf (fd ,"can record and replay an X session. %s also has the ability to distribute events to multiple displays. \n", XNEE_CLI);
  fprintf (fd ,".B %s  \n", XNEE_CLI);
  fprintf (fd ,"gets copies of X protocol data from the X server. These are either printed to file ( \n");
  fprintf (fd ,".I record \n");
  fprintf (fd ,"mode) or replayed and synchronised ( \n");
  fprintf (fd ,".I replay  \n");
  fprintf (fd ,"mode). \n");
  fprintf (fd ,"During record and replay %s can distribute the record/replayed events to multiple displays. \n", XNEE_CLI);

  fprintf (fd ,".SH \"GENERAL OPTIONS\" \n");
  xnee_manpage_sub(fd, xnee_options, XNEE_GENERAL_OPTION);

  fprintf (fd ,".SH \"RECORD OPTIONS\" \n");
  xnee_manpage_sub(fd, xnee_options, XNEE_RECORD_OPTION);
 
  fprintf (fd ,".SH \"REPLAY OPTIONS\" \n");
  xnee_manpage_sub(fd, xnee_options, XNEE_REPLAY_OPTION);

  fprintf (fd ,".SH \"CNEE OPTIONS\" \n");
  xnee_manpage_sub(fd, cnee_options, XNEE_ANY_OPTION);


  /*
    for (cpp = help; *cpp; cpp++) 
    {
    fprintf (fd, ".TP\n.B \"%s\"\n", *cpp++);
    fprintf (fd, "%s\n", *cpp);
    }
  */

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
  

  fprintf (fd ,".SH \"AUTHOR\" \n");
  fprintf (fd ,"Henrik Sandklef.\n");

  fprintf (fd ,".SH \"REPORTING BUGS\"\n");
  fprintf (fd ,"Report bugs in the program to %s. \n", XNEE_BUG_MAIL);

  fprintf (fd ,".SH \"COPYRIGHT\"\n");
  fprintf (fd ,"Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007  Henrik Sandklef.\n");
  fprintf (fd ,"This  is  free  software;  see the source for copying conditions. ");
  fprintf (fd ,"There is NO warranty;");
  fprintf (fd ,"not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");

  fprintf (fd ,".SH \"SEE ALSO\"\n"); 
  fprintf (fd ,
	   ".BR %s(info), %s(info), xev (1)\n", 
	   XNEE_CLI,
	   PACKAGE);

  fprintf (fd ,".SH \"VERSION\"\n");
  fprintf (fd ,PACKAGE " man page, version " VERSION "\n");

  fprintf (fd ,".SH \"NOTES\"\n");
  fprintf (fd ,"This page describes\n");
  fprintf (fd ,".B %s.\n", XNEE_CLI);
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
  fprintf (fd ,"Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007 Henrik Sandklef.\n");
  fprintf (fd, "@*\n");
  fprintf (fd, "@*\n");
  fprintf (fd ,"This  is  free  software;  see the source for copying conditions. ");
  fprintf (fd ,"There is NO warranty;");
  fprintf (fd ,"not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");


  fprintf (fd, "@node Options, Examples, top, top \n");

  xnee_infopage_sub(fd, xnee_options, XNEE_GENERAL_OPTION);
  xnee_infopage_sub(fd, xnee_options, XNEE_RECORD_OPTION);
  xnee_infopage_sub(fd, xnee_options, XNEE_REPLAY_OPTION);
  xnee_infopage_sub(fd, cnee_options, XNEE_ANY_OPTION);


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
