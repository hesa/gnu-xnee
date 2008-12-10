#include <stdio.h>

#include "libxnee/xnee.h"
#include "libxnee/xnee_resource.h"

extern int orig;
extern int ret;
extern int new;

extern char *orig_str;
extern char *ret_str;
extern char *new_str;

extern FILE *orig_fd;
extern FILE *ret_fd;
extern FILE *new_fd;

extern xnee_data *xdl;

extern int fails;
extern int total;
extern int verbose ; 

#define PRINT_FUN   printf ("   %s:%d     %s()\n", __FILE__, __LINE__ , __func__); 

#define XNEE_PRINT_ERROR(err) \
      fprintf (stderr, "Error number: %d\n", err);                                \
      fprintf (stderr, "  Error:      %s\n", xnee_get_err_description(err));     \
      fprintf (stderr, "  Solution:   %s\n", xnee_get_err_solution(err)); \

#define ON_FAIL(fun, line) \
   total++; \
   fails++ ; \
   printf("Test of function '%s()' failed  (%s:%d)   %d/%d\n", fun, __FILE__, line, fails, total );  

   #define ON_OK(fun, line)   if(verbose)printf("Test of function '%s()' OK    line:%d   %d/%d  \n", fun, line, fails, total); total++;

#define XNEE_TEST_ASSERT(ret_val, comp_val, fun_str) \
   { \
     if ( ret_val != comp_val ) \
       {                    \
           ON_FAIL(fun_str, __LINE__); \
           printf ("   %d != %d\n", ret_val, comp_val); \
           PRINT_FUN;\
           exit(1); \
       }\
     else \
      { \
           ON_OK(fun_str, __LINE__); \
       } \
   }

#define XNEE_TEST_ASSERT_DIFF(ret_val, comp_val, fun_str) \
   { \
     if ( ret_val == comp_val ) \
       {                    \
          ON_FAIL(fun_str, __LINE__);\
          printf ("   %d == %d\n", ret_val, comp_val); \
          PRINT_FUN \
           exit(1); \
       }\
     else \
        ON_OK(fun_str, __LINE__); \
   }

#define XNEE_TEST_ASSERT_STR(ret_str, comp_str, fun_str) \
   { \
     if ( strcmp(ret_str, comp_str)!=0) \
       {                    \
          ON_FAIL(fun_str, __LINE__);\
          printf ("   %s != %s\n", ret_str, comp_str); \
          PRINT_FUN \
          exit(1); \
       }\
     else \
        ON_OK(fun_str, __LINE__);\
   }

#define XNEE_TEST_ASSERT_DIFF_STR(ret_str, comp_str, fun_str) \
   { \
     if ( strcmp(ret_str, comp_str )==0) \
       {                    \
          ON_FAIL(fun_str, __LINE__);\
          printf ("   %s == %s\n", ret_str, comp_str); fflush(stdout);\
          PRINT_FUN \
          exit(1); \
       }\
     else \
        ON_OK(fun_str, __LINE__);\
   }


