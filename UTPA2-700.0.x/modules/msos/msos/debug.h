#ifndef __H_DEBUG__
#define __H_DEBUG__

#ifndef EXTRA_FMT
# define EXTRA_FMT "%s: "
#endif
#ifndef EXTRA_ARG
# define EXTRA_ARG ,__FUNCTION__
#endif


//extern int lib_lowprintf(FAR const char *format, ...);
extern int lib_lowprintf(const char *format, ...);
#  define lldbg(format, arg...) \
   lib_lowprintf(EXTRA_FMT format EXTRA_ARG, ##arg)

   //lib_lowprintf(EXTRA_FMT format EXTRA_ARG, ##arg)
#endif
