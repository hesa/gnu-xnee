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






/* Standard includes */
#include <stdio.h>

/* X11 includes */
#include <X11/Xlib.h>
#include <X11/Xmd.h>
#include <X11/Xproto.h>
#include <X11/Xos.h>
#include <X11/Xlibint.h>  /* Warning, there be dragons here.... */

/* 
 *  X11 extensions includes 
 */
#include <X11/extensions/record.h> 

#include "libxnee/xnee.h"
#include "libxnee/print.h"
#include "libxnee/xnee_dl.h"
#include "libxnee/xnee_sem.h"
#include "libxnee/xnee_setget.h"
#include "libxnee/xnee_resolution.h"
#include "libxnee/xnee_resource.h"
#include "libxnee/xnee_grab.h"
#include "libxnee/xnee_km.h"
#include "libxnee/datastrings.h"
#include "libxnee/xnee_range.h"


static struct xnee_ranges  myxrs           ;
static int    need_init          =  1      ;
struct xnee_ranges *xrs          = &myxrs  ;



/**************************************************************
 *                                                            *
 * xnee_null_range                                            *
 *                                                            *
 *                                                            *
 **************************************************************/
void 
xnee_null_range ( XRecordRange *range)
{
  memset (range, 0, sizeof(XRecordRange));
  /* There seems to be something spooky about setting 
     errors to first=last=0
     This solves the X server crashes that has been around for a while */
  /*
  range->errors.first = BadCursor;
  range->errors.last  = BadCursor;
  */
}

static int
xnee_bsort(int *numbers, int size)
{
  int i;
  int j;
  int temp;

  for (i = (size - 1); i >= 0; i--)
  {
    for (j = 1; j <= i; j++)
    {
       if (numbers[j-1] > numbers[j])
       {
          temp = numbers[j-1];
          numbers[j-1] = numbers[j];
          numbers[j] = temp;
       }
    }
  }
  return XNEE_OK;
}

static int 
xnee_bsort_all()
{
   int i ; 
   for (i=0;i<XNEE_NR_OF_TYPES;i++)
      xnee_bsort (xrs->type[i].data, xrs->type[i].index);
  return XNEE_OK;
}


static int 
xnee_init_list(struct xnee_range *xr, int size)
{
   xr->index = 0;
   xr->size  = size;
   xr->data = (int*) malloc (xr->size*sizeof(int));
   return XNEE_OK;
}

int 
xnee_free_lists()
{
   int i ; 
   for (i=0;i<XNEE_NR_OF_TYPES;i++)
   {
      free(xrs->type[i].data);
   }
   need_init = 1;
   return XNEE_OK;
}

static int 
xnee_init_lists()
{
   int i ; 
   xrs->alloc_size = 5;
   
   for (i=0;i<XNEE_NR_OF_TYPES;i++)
   {
      xnee_init_list (&xrs->type[i], xrs->alloc_size);
   }
   need_init = 0 ;
   return XNEE_OK;
}


int 
xnee_refresh_ranges(xnee_data *xd)
{
  xnee_verbose((xd, "-->xnee_refresh_ranges\n"));
  xnee_free_lists();
  xnee_init_lists();
  xrs->nr_of_data = 0;
  xnee_verbose((xd, "<--xnee_refresh_ranges\n"));
  return XNEE_OK;
}

int 
xnee_print_list(void)
{
   int i ; 
   int j ; 
   
   
   for (i=0;i<XNEE_NR_OF_TYPES;i++)
   {
      printf ("%d:[", i);
      for (j=0; j<xrs->type[i].index ;j++)
      {
         printf ("%.3d", xrs->type[i].data[j]);

         if (j!=xrs->type[i].index-1)
            printf (",");
      }
      printf ("]\n");
   }
   printf ("\n");
   return XNEE_OK;
}


int 
xnee_add_to_list2(int type, int ev)
{
   int i ;
   struct xnee_range *xrp;

   
   if (type==XNEE_EVENT)
     {
       if ((ev>=KeyPress)&&(ev<=MotionNotify))
	 type=XNEE_DEVICE_EVENT;
       else
	 type=XNEE_DELIVERED_EVENT;
     }
   xrp = &xrs->type[type];

   if (need_init==1)
      xnee_init_lists();
      
   for (i=0;i<xrp->index;i++)
      if (xrp->data[i]==ev)
      {
         return 0;
      }

   if (xrp->index >=(xrp->size-1) )
   {
      xrp->size+=xrp->size;
      
      xrp->data = (int*) realloc (xrp->data, 
                                  xrp->size*sizeof(int));
      if (xrp->data==NULL)
      { 
         printf ("  PANIC in xnee_add_to_list2 ....\n") ; 
	 fflush(stdout); 
	 exit(0); 
      }
   }
   xrp->data[xrp->index++]=ev;
   return XNEE_OK;
}

/**************************************************************
 *                                                            *
 * xnee_add_range_str                                         *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_add_range_str (xnee_data *xd, int type, char *range)
{
  char start_str[XNEE_RANGE_STRING_SIZE] ;
  char *second;
  int start=0;
  int stop=0;
  int str_len=0;
  int ret=0;
  int i ; 
  xnee_verbose((xd, " --> add_range_str (%d, %d, %s)\n", xd,type, range));
  str_len=strspn(range, "1234567890-");

  if ( str_len > XNEE_RANGE_STRING_SIZE )
    {
      fprintf (stderr, "The string you wanted to add is too long\n");
      fprintf (stderr, "Ranges can't be longer then %d characters\n",
	       XNEE_RANGE_STRING_SIZE);
      return -1;
    }

  if (str_len==0)
    {
      str_len=
	strspn(range, 
	       "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_");
	  strncpy(start_str,range,str_len);
	  start_str[str_len]='\0';
	  start=xnee_data2int (type, start_str);
	  if (start==-1)
	    {
	      fprintf(stderr, 
		      "Could not convert \"%s\" to an integer\nleaving", 
		      start_str); 
	      return -1;
	    }
    }
  else
    {
      strncpy(start_str,range,str_len);
      start_str[str_len]='\0';
      sscanf(start_str,"%d",&start);
    }
  xnee_verbose((xd, 
		" --  add_range_str first string=\"%s\" (%d)\n", 
		start_str, start));
  
  
  second=strchr ( range, '-' ) ;
  if (second) 
    {
      int ret;
      second++;
      ret=sscanf(second,"%d",&stop);
      if (ret==0)
	{
	  stop=xnee_data2int (type, second);
	  if (stop==-1)
	    {
	      fprintf(stderr, 
		      "Could not convert \"%s\" to an integer\nleaving", 
		      second); 
	      return -1;
	    }
	}
      xnee_verbose((xd, " --  add_range_str second string=\"%s\"   ret=%d\n", 
                    second, ret));
    }

  xnee_verbose((xd, " --  add_range_str 1 %d %d \n" , 
		start, stop));
  if ( (start>=0) && (stop>=0) )
    {
      if ( (start!=stop) && (start<stop) )
	{
	  for (i=start;i<=stop;i++)
	    {
	      ret = xnee_add_to_list2(type, i);
	    }
	}
      else if (start!=0)
	{
	  xnee_verbose((xd, " --  add_range_str 4\n" ));
	  ret = xnee_add_to_list2(type, start);
	}
    }
  /* ret=xnee_add_range (xd, type, start, stop); */
  
  
  xnee_verbose((xd, " <-- add_range_str (%d, %d, %s)\n", xd,type, range));
  return (ret);
}



/**************************************************************
 *                                                            *
 * xnee_add_range                                             *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_add_range (xnee_data* xd, 
		int type, 
		int start, 
		int stop)
{

  /*  
      int this_types_index=0;
  */

  XRecordRange *range;
  int max_index=0;
  int alloc_nr=0;

  XRecordRange *r_range; 
  xnee_verbose((xd, "---> xnee_add_range %d %d %d\n", type, start, stop));
  
  XNEE_DEBUG ( (stderr ," --> xnee_add_range()  \n"  ));
  /* increment the counter to ensure we allocate enough memory */
  alloc_nr=xd->xnee_info.data_ranges[type]  ;
  max_index=xnee_get_max_range(xd) - 1 ;
  
  

  xnee_verbose((xd, "  MAX %d ALLOC %d\n", max_index, alloc_nr));
  



  if (alloc_nr>max_index)
  {
     xnee_verbose((xd, " -- Allocating a new range of size %d\n", 
                   alloc_nr+1));
     
     if ( alloc_nr == 0 ) 
     {
        xd->record_setup->range_array = 
           (XRecordRange**) Xcalloc (1, sizeof(XRecordRange*));
     }
     else 
     {
        xd->record_setup->range_array = 
           (XRecordRange**) Xrealloc (xd->record_setup->range_array,
                                      (alloc_nr+1)*sizeof(XRecordRange*));
     }
     r_range =  XRecordAllocRange();
     
     xnee_null_range (r_range);
     xd->record_setup->range_array[alloc_nr] = r_range; 
  }
  
  

  /* is it single value */
  if (stop==0) 
    {
      stop=start;
    }
  xnee_verbose((xd, 
                "Adding %d range %d - %d at range %d\n", 
                type, start, stop, alloc_nr));

  range = xd->record_setup->range_array[alloc_nr];

  if ( type == XNEE_DELIVERED_EVENT ) 
    {
      range->delivered_events.first = start;
      range->delivered_events.last = stop;

      /* Workaround for problem with crashing X server*/
      xd->xnee_info.data_ranges[XNEE_ERROR]++;
      range->errors.first = BadCursor;
      range->errors.last = BadCursor;
    }
  else if ( type == XNEE_REQUEST ) 
    {
      range->core_requests.first = start;
      range->core_requests.last = stop;
    }
  else if ( type == XNEE_ERROR ) 
    {
      range->errors.first = start;
      range->errors.last = stop;

      /* Workaround for problem with crashing X server*/
      range->delivered_events.first =33 ;
      range->delivered_events.last = 33;
      xd->xnee_info.data_ranges[XNEE_DELIVERED_EVENT]++;
    }
  else if ( type == XNEE_REPLY ) 
    {
      range->core_replies.first = start;
      range->core_replies.last = stop;

    }
  else if ( type == XNEE_EXT_REQUEST_MAJOR ) 
    {
      range->ext_requests.ext_major.first = start;
      range->ext_requests.ext_major.last = stop;
    }
  else if ( type == XNEE_EXT_REQUEST_MINOR ) 
    {
      range->ext_requests.ext_minor.first = start;
      range->ext_requests.ext_minor.last = stop;
    }
  else if ( type == XNEE_EXT_REPLY_MAJOR ) 
    {
      range->ext_replies.ext_major.first = start;
      range->ext_replies.ext_major.last = stop;
    }
  else if ( type == XNEE_EXT_REPLY_MINOR ) 
    {
      range->ext_replies.ext_minor.first = start;
      range->ext_replies.ext_minor.last = stop;
    }
  else if ( type == XNEE_DEVICE_EVENT ) 
    {
      range->device_events.first = start;
      range->device_events.last = stop;
    }

 
  /*
   * Used when debugging 
   * print_data_range_count (xnee_info, rec_range); 
   *
   */
  xd->xnee_info.data_ranges[type]++;
  xnee_verbose((xd, "<--- xnee_add_range\n"));
  return (0);
  
}


/**************************************************************
 *                                                            *
 * xnee_parse_range                                           *
 *                                                            *
 *                                                            *
 **************************************************************/
int
xnee_parse_range (xnee_data *xd,int type, char *range)
{
  char buf[DATA_NAME_SIZE_MAX];
  int next;
  int len;
  int ret=0;
  int range_len=strlen(range);

  xnee_verbose ((xd,"int arg=%d\n", xd));
  xnee_verbose ((xd, "nt arg=%d\n", type));
  xnee_verbose ((xd, "string arg=%s\n", range));

  xnee_verbose((xd, "--> parse_range (%u, %d, %s)\n", (int) xd, type, range));

  while ( 1 ) {
    next=strspn (range, "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_");
    if (next==0) break; 
    strncpy(buf,range,next);
    buf[next]='\0';
    len =strlen(range);
    range+=next+1;
    range_len=range_len - next - 1;
    xnee_verbose((xd, " -- calling xnee_add_range_str (%d, %d, %s) \n" , xd, type, buf));

    ret=xnee_add_range_str (xd, type, buf);
    if (range_len<=0) break;
    if (ret!=0) return (ret);
  }

  xnee_verbose((xd, "<-- parse_range()\n"));
  return (0);  
}





int 
xnee_get_nr_of_data (int type)
{
   if (xrs->type==NULL)
      return -1;
   else
      return xrs->type[type].index;
}

int *
xnee_get_data (int type)
{
   if (xrs->type==NULL)
      return NULL;
   else
      return xrs->type[type].data;
}



int 
xnee_set_ranges(xnee_data *xd)
{
   int i ; 
   int j ; 


   xnee_bsort_all();


   for (j=0; j<XNEE_NR_OF_TYPES ;j++)
   {
      int first = -1;
      int last  = -1;
      int this  ;
      for (i=0; i<xrs->type[j].index ;i++)
	{
	  this = xrs->type[j].data[i] ;
	  
	  if (first == -1)
	    {
	      first=this;
	      last=this;
	    }
	  else if ( this == last + 1 )
	    {
	      last = this;
	    }
	  else
	    {
	      xnee_add_range (xd, j, 
			      first,
			      last);
	      
	      first=this;
	      last=this;
	    }
	}
      if (first!=-1)
	{
	  if (last!=-1)
	    {
	      xnee_add_range (xd, j, 
			      first,
			      last);
	    }
	  else
	    {
	      xnee_add_range (xd, j, 
			      first,
			      first);
	    }
	}
   } 
   return XNEE_OK;
}



int 
xnee_rem_from_list(int type, int ev)
{
   int i ; 
   int j ; 

   struct xnee_range *xrp;

   if (need_init==1)
      return 0;
   xrp = &xrs->type[type];

   xnee_print_list();
    

   for (i=0;i<xrp->index;i++)
   {
      if ( xrp->data[i] == ev )
      {
         for (j=i;j<xrp->index;j++)
         {
            if (j==xrp->index-1)
            {
               xrp->data[j]=0;
            }
            else
            {
               xrp->data[j]=xrp->data[j+1];
            }
         }
         xrp->index--;
      }
   }

   return XNEE_OK;
}


int*
xnee_get_list(int type)
{
   if (need_init==1)
      return NULL;

   if (type==0)
   {
      return xrs->type[type].data;
   }
   return XNEE_OK;
}



int
xnee_rem_data_from_range_str (xnee_data *xd, 
                              int type, 
                              char *name)
{
   int rem_data ; 
   int my_type;

   if (type == -1)
     {
       rem_data = xnee_data2int_special(&my_type, name);
       type = my_type ; 
     }
   else
     {
       rem_data = xnee_data2int(type, name);
     }

   if (rem_data == -1)
   {
      return -1;
   }
   
   xnee_verbose ((xd, "xnee range removing : %d of type %d\n", 
		  rem_data, type ));
   xnee_rem_from_list(type, rem_data);


   return XNEE_OK;
}




