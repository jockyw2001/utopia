
#ifndef H_NSK_DBG
#define H_NSK_DBG

#define NSK_DEBUG_LEVEL    0
#define NSK_DEBUG_RAW(x) { printf x ; }

#define PRINT_ERROR     3
#define PRINT_INFO      4
#define PRINT_TRACE     7

MS_U32 debug_level;

#define NSK_PRINT(A)  do { if(debug_level >= PRINT_TRACE)     \
                      {                       \
                            NSK_DEBUG_RAW(A); \
                      } } while (0)

#define NSK_ERROR(A)  do { if(debug_level >= PRINT_ERROR)     \
                      {                       \
                            NSK_DEBUG_RAW(("ERROR: ")); NSK_DEBUG_RAW(A); NSK_DEBUG_RAW((" at line %d file %s\n", __LINE__, __FILE__)); \
                      } } while (0)

#define NSK_INFO(A)  do { if(debug_level >= PRINT_INFO)     \
                      {                       \
                            NSK_DEBUG_RAW(("------ %s(): ", __FUNCTION__)); NSK_DEBUG_RAW(A); \
                      } } while (0)


#define NSK_TRACE(A)  do { if(debug_level >= PRINT_TRACE)     \
                      {                       \
                            NSK_DEBUG_RAW(("------ %s(): ", __FUNCTION__)); NSK_DEBUG_RAW(A); \
                      } } while (0)

#if 0
#if NSK_DEBUG_LEVEL == 0
#define NSK_PRINT(A)
#define NSK_ERROR(A)
#define NSK_TRACE(A)
#elif NSK_DEBUG_LEVEL == 1
#define NSK_PRINT(A)  NSK_DEBUG_RAW(A)
#define NSK_ERROR(A)  { NSK_DEBUG_RAW(("ERROR: ")); NSK_DEBUG_RAW(A); NSK_DEBUG_RAW((" at line %d file %s\n", __LINE__, __FILE__)); }
#define NSK_TRACE(A)
#else
#define NSK_PRINT(A)  NSK_DEBUG_RAW(A)
#define NSK_ERROR(A)  { NSK_DEBUG_RAW(("ERROR: ")); NSK_DEBUG_RAW(A); NSK_DEBUG_RAW((" at line %d file %s\n", __LINE__, __FILE__)); }
#define NSK_TRACE(A)  { NSK_DEBUG_RAW(("------ %s(): ", __FUNCTION__)); NSK_DEBUG_RAW(A); }
#endif

#define DEBUG_LEVEL     0

#define debug(lvl, x...)    do { if (lvl <= DEBUG_LEVEL) { printf(x); } } while (0)
#endif



#endif // H_NSK_DBG

