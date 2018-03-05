/*
 * time.h
 * 
 * Struct and function declarations for dealing with time.
 */

#ifndef _TIME_H_
#define _TIME_H_

#include "_ansi.h"
#include <sys/reent.h>

#ifndef NULL
#define	NULL	0
#endif

/* Get _CLOCKS_PER_SEC_ */
#include <machine/time.h>

#ifndef _CLOCKS_PER_SEC_
#define _CLOCKS_PER_SEC_ 1000
#endif

#define CLOCKS_PER_SEC _CLOCKS_PER_SEC_
#define CLK_TCK CLOCKS_PER_SEC
#define __need_size_t
#include <stddef.h>

#include <sys/types.h>

_BEGIN_STD_C

struct tm
{
  int	tm_sec;
  int	tm_min;
  int	tm_hour;
  int	tm_mday;
  int	tm_mon;
  int	tm_year;
  int	tm_wday;
  int	tm_yday;
  int	tm_isdst;
};

clock_t	   _EXFUN(clock,    (void));
double	   _EXFUN(difftime, (time_t _time2, time_t _time1));
time_t	   _EXFUN(mktime,   (struct tm *_timeptr));
time_t	   _EXFUN(time,     (time_t *_timer));
#ifndef _REENT_ONLY
char	  *_EXFUN(asctime,  (const struct tm *_tblock));
char	  *_EXFUN(ctime,    (const time_t *_time));
struct tm *_EXFUN(gmtime,   (const time_t *_timer));
struct tm *_EXFUN(localtime,(const time_t *_timer));
#endif
size_t	   _EXFUN(strftime, (char *_s, size_t _maxsize, const char *_fmt, const struct tm *_t));

char	  *_EXFUN(asctime_r,	(const struct tm *, char *));
char	  *_EXFUN(ctime_r,	(const time_t *, char *));
struct tm *_EXFUN(gmtime_r,	(const time_t *, struct tm *));
struct tm *_EXFUN(localtime_r,	(const time_t *, struct tm *));

#if defined(AEONSIM) || defined(XGEN)
#include <sys/syscall.h>

#define AEONSIM_MAX_TIMER   16
#define AEONSIM_ALL_TIMER   -1
#define XGEN_MAX_TIMER      AEONSIM_MAX_TIMER
#define XGEN_ALL_TIMER      AEONSIM_ALL_TIMER
#define AEONSIM_MAX_COUNTER   16
#define AEONSIM_ALL_COUNTER   -1
#define XGEN_MAX_COUNTER      AEONSIM_MAX_COUNTER
#define XGEN_ALL_COUNTER      AEONSIM_ALL_COUNTER

void __timer_reset(int timer_id);
void __timer_start(int timer_id);
void __timer_stop(int timer_id);
void __timer_cancel(int timer_id);
void __timer_report(int timer_id);
int __timer_started(int timer_id);
int __timer_n_start(int timer_id);

long long  __timer_last_cycles(int timer_id);       // number of cycles elapse
long long  __timer_last_cycles_due_to_i_miss(int timer_id);       // number of cycles elapse due to i-cache miss
long long  __timer_last_cycles_due_to_d_miss(int timer_id);       // number of cycles elapse due to i-cache miss
long long  __timer_last_instructions(int timer_id); // number of instruction executed
long long  __timer_last_data_prefetch(int timer_id);// data prefetch in byte
long long  __timer_last_data_read(int timer_id);    // data read in byte
long long  __timer_last_data_write(int timer_id);   // data write in byte
long long  __timer_last_instruction_fetch(int timer_id);   // instruction fetch in byte
long long  __timer_last_bus_prefetch(int timer_id);// data prefetch in byte which miss cache/scratch and involve memroy bus
long long  __timer_last_bus_read(int timer_id);    // data read in byte which miss cache/scratch and involve memroy bus
long long  __timer_last_bus_write(int timer_id);   // data write in byte which miss cache/scratch and involve memroy bus
long long  __timer_last_bus_fetch(int timer_id);   // instruction fetch in byte which miss cache/scratch and involve memroy bus
long long  __timer_last_qbus_read(int timer_id);   // data read in byte from QMEM
long long  __timer_last_qbus_write(int timer_id);  // data write in byte to QMEM
long long  __timer_last_qbus_fetch(int timer_id);  // instruction fetch in byte from QMEM

long long  __timer_total_cycles(int timer_id);
long long  __timer_total_cycles_due_to_i_miss(int timer_id);
long long  __timer_total_cycles_due_to_d_miss(int timer_id);
long long  __timer_total_instructions(int timer_id);
long long  __timer_total_data_prefetch(int timer_id);
long long  __timer_total_data_read(int timer_id);
long long  __timer_total_data_write(int timer_id);
long long  __timer_total_instruction_fetch(int timer_id);
long long  __timer_total_bus_prefetch(int timer_id);
long long  __timer_total_bus_read(int timer_id);
long long  __timer_total_bus_write(int timer_id);
long long  __timer_total_bus_fetch(int timer_id);
long long  __timer_total_qbus_read(int timer_id);
long long  __timer_total_qbus_write(int timer_id);
long long  __timer_total_qbus_fetch(int timer_id);

long long  __timer_total_microsecond(int timer_id);
void __timer_add_last(int dest_timer_id, int src_timer_id);

void __counter_reset(int timer_id);
void __counter_increment(int timer_id, unsigned divisor);
void __counter_report(int timer_id);
long long __counter_value(int timer_id);

// __timer_start/stop short-hand
#ifdef AEONSIM
#define __timer0_start()    __asm__ __volatile__ ("l.nop %0\t# void __timer0_start()" : : "K"(NOP_AEONSIM_TIMER0_START+0));
#define __timer1_start()    __asm__ __volatile__ ("l.nop %0\t# void __timer1_start()" : : "K"(NOP_AEONSIM_TIMER0_START+1));
#define __timer2_start()    __asm__ __volatile__ ("l.nop %0\t# void __timer2_start()" : : "K"(NOP_AEONSIM_TIMER0_START+2));
#define __timer3_start()    __asm__ __volatile__ ("l.nop %0\t# void __timer3_start()" : : "K"(NOP_AEONSIM_TIMER0_START+3));
#define __timer4_start()    __asm__ __volatile__ ("l.nop %0\t# void __timer4_start()" : : "K"(NOP_AEONSIM_TIMER0_START+4));
#define __timer5_start()    __asm__ __volatile__ ("l.nop %0\t# void __timer5_start()" : : "K"(NOP_AEONSIM_TIMER0_START+5));
#define __timer6_start()    __asm__ __volatile__ ("l.nop %0\t# void __timer6_start()" : : "K"(NOP_AEONSIM_TIMER0_START+6));
#define __timer7_start()    __asm__ __volatile__ ("l.nop %0\t# void __timer7_start()" : : "K"(NOP_AEONSIM_TIMER0_START+7));
#define __timer8_start()    __asm__ __volatile__ ("l.nop %0\t# void __timer8_start()" : : "K"(NOP_AEONSIM_TIMER0_START+8));
#define __timer9_start()    __asm__ __volatile__ ("l.nop %0\t# void __timer9_start()" : : "K"(NOP_AEONSIM_TIMER0_START+9));
#define __timer10_start()   __asm__ __volatile__ ("l.nop %0\t# void __timer10_start()" : : "K"(NOP_AEONSIM_TIMER0_START+10));
#define __timer11_start()   __asm__ __volatile__ ("l.nop %0\t# void __timer11_start()" : : "K"(NOP_AEONSIM_TIMER0_START+11));
#define __timer12_start()   __asm__ __volatile__ ("l.nop %0\t# void __timer12_start()" : : "K"(NOP_AEONSIM_TIMER0_START+12));
#define __timer13_start()   __asm__ __volatile__ ("l.nop %0\t# void __timer13_start()" : : "K"(NOP_AEONSIM_TIMER0_START+13));
#define __timer14_start()   __asm__ __volatile__ ("l.nop %0\t# void __timer14_start()" : : "K"(NOP_AEONSIM_TIMER0_START+14));
#define __timer15_start()   __asm__ __volatile__ ("l.nop %0\t# void __timer15_start()" : : "K"(NOP_AEONSIM_TIMER0_START+15));

#define __timer0_stop()     __asm__ __volatile__ ("l.nop %0\t# void __timer0_stop()" : : "K"(NOP_AEONSIM_TIMER0_STOP+0));
#define __timer1_stop()     __asm__ __volatile__ ("l.nop %0\t# void __timer1_stop()" : : "K"(NOP_AEONSIM_TIMER0_STOP+1));
#define __timer2_stop()     __asm__ __volatile__ ("l.nop %0\t# void __timer2_stop()" : : "K"(NOP_AEONSIM_TIMER0_STOP+2));
#define __timer3_stop()     __asm__ __volatile__ ("l.nop %0\t# void __timer3_stop()" : : "K"(NOP_AEONSIM_TIMER0_STOP+3));
#define __timer4_stop()     __asm__ __volatile__ ("l.nop %0\t# void __timer4_stop()" : : "K"(NOP_AEONSIM_TIMER0_STOP+4));
#define __timer5_stop()     __asm__ __volatile__ ("l.nop %0\t# void __timer5_stop()" : : "K"(NOP_AEONSIM_TIMER0_STOP+5));
#define __timer6_stop()     __asm__ __volatile__ ("l.nop %0\t# void __timer6_stop()" : : "K"(NOP_AEONSIM_TIMER0_STOP+6));
#define __timer7_stop()     __asm__ __volatile__ ("l.nop %0\t# void __timer7_stop()" : : "K"(NOP_AEONSIM_TIMER0_STOP+7));
#define __timer8_stop()     __asm__ __volatile__ ("l.nop %0\t# void __timer8_stop()" : : "K"(NOP_AEONSIM_TIMER0_STOP+8));
#define __timer9_stop()     __asm__ __volatile__ ("l.nop %0\t# void __timer9_stop()" : : "K"(NOP_AEONSIM_TIMER0_STOP+9));
#define __timer10_stop()    __asm__ __volatile__ ("l.nop %0\t# void __timer10_stop()" : : "K"(NOP_AEONSIM_TIMER0_STOP+10));
#define __timer11_stop()    __asm__ __volatile__ ("l.nop %0\t# void __timer11_stop()" : : "K"(NOP_AEONSIM_TIMER0_STOP+11));
#define __timer12_stop()    __asm__ __volatile__ ("l.nop %0\t# void __timer12_stop()" : : "K"(NOP_AEONSIM_TIMER0_STOP+12));
#define __timer13_stop()    __asm__ __volatile__ ("l.nop %0\t# void __timer13_stop()" : : "K"(NOP_AEONSIM_TIMER0_STOP+13));
#define __timer14_stop()    __asm__ __volatile__ ("l.nop %0\t# void __timer14_stop()" : : "K"(NOP_AEONSIM_TIMER0_STOP+14));
#define __timer15_stop()    __asm__ __volatile__ ("l.nop %0\t# void __timer15_stop()" : : "K"(NOP_AEONSIM_TIMER0_STOP+15));

#define __timer0_cancel()     __asm__ __volatile__ ("l.nop %0\t# void __timer0_cancel()" : : "K"(NOP_AEONSIM_TIMER0_CANCEL+0));
#define __timer1_cancel()     __asm__ __volatile__ ("l.nop %0\t# void __timer1_cancel()" : : "K"(NOP_AEONSIM_TIMER0_CANCEL+1));
#define __timer2_cancel()     __asm__ __volatile__ ("l.nop %0\t# void __timer2_cancel()" : : "K"(NOP_AEONSIM_TIMER0_CANCEL+2));
#define __timer3_cancel()     __asm__ __volatile__ ("l.nop %0\t# void __timer3_cancel()" : : "K"(NOP_AEONSIM_TIMER0_CANCEL+3));
#define __timer4_cancel()     __asm__ __volatile__ ("l.nop %0\t# void __timer4_cancel()" : : "K"(NOP_AEONSIM_TIMER0_CANCEL+4));
#define __timer5_cancel()     __asm__ __volatile__ ("l.nop %0\t# void __timer5_cancel()" : : "K"(NOP_AEONSIM_TIMER0_CANCEL+5));
#define __timer6_cancel()     __asm__ __volatile__ ("l.nop %0\t# void __timer6_cancel()" : : "K"(NOP_AEONSIM_TIMER0_CANCEL+6));
#define __timer7_cancel()     __asm__ __volatile__ ("l.nop %0\t# void __timer7_cancel()" : : "K"(NOP_AEONSIM_TIMER0_CANCEL+7));
#define __timer8_cancel()     __asm__ __volatile__ ("l.nop %0\t# void __timer8_cancel()" : : "K"(NOP_AEONSIM_TIMER0_CANCEL+8));
#define __timer9_cancel()     __asm__ __volatile__ ("l.nop %0\t# void __timer9_cancel()" : : "K"(NOP_AEONSIM_TIMER0_CANCEL+9));
#define __timer10_cancel()    __asm__ __volatile__ ("l.nop %0\t# void __timer10_cancel()" : : "K"(NOP_AEONSIM_TIMER0_CANCEL+10));
#define __timer11_cancel()    __asm__ __volatile__ ("l.nop %0\t# void __timer11_cancel()" : : "K"(NOP_AEONSIM_TIMER0_CANCEL+11));
#define __timer12_cancel()    __asm__ __volatile__ ("l.nop %0\t# void __timer12_cancel()" : : "K"(NOP_AEONSIM_TIMER0_CANCEL+12));
#define __timer13_cancel()    __asm__ __volatile__ ("l.nop %0\t# void __timer13_cancel()" : : "K"(NOP_AEONSIM_TIMER0_CANCEL+13));
#define __timer14_cancel()    __asm__ __volatile__ ("l.nop %0\t# void __timer14_cancel()" : : "K"(NOP_AEONSIM_TIMER0_CANCEL+14));
#define __timer15_cancel()    __asm__ __volatile__ ("l.nop %0\t# void __timer15_cancel()" : : "K"(NOP_AEONSIM_TIMER0_CANCEL+15));

// __counter_increment short-hand
#define __counter0_increment()     __asm__ __volatile__ ("l.nop %0\t# void __counter0_increment()" : : "K"(NOP_AEONSIM_COUNTER0_INCREMENT+0));
#define __counter1_increment()     __asm__ __volatile__ ("l.nop %0\t# void __counter1_increment()" : : "K"(NOP_AEONSIM_COUNTER0_INCREMENT+1));
#define __counter2_increment()     __asm__ __volatile__ ("l.nop %0\t# void __counter2_increment()" : : "K"(NOP_AEONSIM_COUNTER0_INCREMENT+2));
#define __counter3_increment()     __asm__ __volatile__ ("l.nop %0\t# void __counter3_increment()" : : "K"(NOP_AEONSIM_COUNTER0_INCREMENT+3));
#define __counter4_increment()     __asm__ __volatile__ ("l.nop %0\t# void __counter4_increment()" : : "K"(NOP_AEONSIM_COUNTER0_INCREMENT+4));
#define __counter5_increment()     __asm__ __volatile__ ("l.nop %0\t# void __counter5_increment()" : : "K"(NOP_AEONSIM_COUNTER0_INCREMENT+5));
#define __counter6_increment()     __asm__ __volatile__ ("l.nop %0\t# void __counter6_increment()" : : "K"(NOP_AEONSIM_COUNTER0_INCREMENT+6));
#define __counter7_increment()     __asm__ __volatile__ ("l.nop %0\t# void __counter7_increment()" : : "K"(NOP_AEONSIM_COUNTER0_INCREMENT+7));
#define __counter8_increment()     __asm__ __volatile__ ("l.nop %0\t# void __counter8_increment()" : : "K"(NOP_AEONSIM_COUNTER0_INCREMENT+8));
#define __counter9_increment()     __asm__ __volatile__ ("l.nop %0\t# void __counter9_increment()" : : "K"(NOP_AEONSIM_COUNTER0_INCREMENT+9));
#define __counter10_increment()    __asm__ __volatile__ ("l.nop %0\t# void __counter10_increment()" : : "K"(NOP_AEONSIM_COUNTER0_INCREMENT+10));
#define __counter11_increment()    __asm__ __volatile__ ("l.nop %0\t# void __counter11_increment()" : : "K"(NOP_AEONSIM_COUNTER0_INCREMENT+11));
#define __counter12_increment()    __asm__ __volatile__ ("l.nop %0\t# void __counter12_increment()" : : "K"(NOP_AEONSIM_COUNTER0_INCREMENT+12));
#define __counter13_increment()    __asm__ __volatile__ ("l.nop %0\t# void __counter13_increment()" : : "K"(NOP_AEONSIM_COUNTER0_INCREMENT+13));
#define __counter14_increment()    __asm__ __volatile__ ("l.nop %0\t# void __counter14_increment()" : : "K"(NOP_AEONSIM_COUNTER0_INCREMENT+14));
#define __counter15_increment()    __asm__ __volatile__ ("l.nop %0\t# void __counter15_increment()" : : "K"(NOP_AEONSIM_COUNTER0_INCREMENT+15));

#else
//!AEONSIM
void __timer0_start();
void __timer1_start();
void __timer2_start();
void __timer3_start();
void __timer4_start();
void __timer5_start();
void __timer6_start();
void __timer7_start();
void __timer8_start();
void __timer9_start();
void __timer10_start();
void __timer11_start();
void __timer12_start();
void __timer13_start();
void __timer14_start();
void __timer15_start();

void __timer0_stop();
void __timer1_stop();
void __timer2_stop();
void __timer3_stop();
void __timer4_stop();
void __timer5_stop();
void __timer6_stop();
void __timer7_stop();
void __timer8_stop();
void __timer9_stop();
void __timer10_stop();
void __timer11_stop();
void __timer12_stop();
void __timer13_stop();
void __timer14_stop();
void __timer15_stop();

void __timer0_cancel();
void __timer1_cancel();
void __timer2_cancel();
void __timer3_cancel();
void __timer4_cancel();
void __timer5_cancel();
void __timer6_cancel();
void __timer7_cancel();
void __timer8_cancel();
void __timer9_cancel();
void __timer10_cancel();
void __timer11_cancel();
void __timer12_cancel();
void __timer13_cancel();
void __timer14_cancel();
void __timer15_cancel();

void __counter0_increment();
void __counter1_increment();
void __counter2_increment();
void __counter3_increment();
void __counter4_increment();
void __counter5_increment();
void __counter6_increment();
void __counter7_increment();
void __counter8_increment();
void __counter9_increment();
void __counter10_increment();
void __counter11_increment();
void __counter12_increment();
void __counter13_increment();
void __counter14_increment();
void __counter15_increment();

#endif /* AEONSIM */
#endif /* AEONSIM || XGEN */

_END_STD_C

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __STRICT_ANSI__
char      *_EXFUN(strptime,     (const char *, const char *, struct tm *));
_VOID      _EXFUN(tzset,	(_VOID));
_VOID      _EXFUN(_tzset_r,	(struct _reent *));

typedef struct __tzrule_struct
{
  char ch;
  int m;
  int n;
  int d;
  int s;
  time_t change;
  long offset; /* Match type of _timezone. */
} __tzrule_type;

typedef struct __tzinfo_struct
{
  int __tznorth;
  int __tzyear;
  __tzrule_type __tzrule[2];
} __tzinfo_type;

__tzinfo_type *_EXFUN (__gettzinfo, (_VOID));

/* getdate functions */

#ifdef HAVE_GETDATE
#ifndef _REENT_ONLY
#define getdate_err (*__getdate_err())
int *_EXFUN(__getdate_err,(_VOID));

struct tm *	_EXFUN(getdate, (const char *));
/* getdate_err is set to one of the following values to indicate the error.
     1  the DATEMSK environment variable is null or undefined,
     2  the template file cannot be opened for reading,
     3  failed to get file status information,
     4  the template file is not a regular file,
     5  an error is encountered while reading the template file,
     6  memory allication failed (not enough memory available),
     7  there is no line in the template that matches the input,
     8  invalid input specification  */
#endif /* !_REENT_ONLY */

/* getdate_r returns the error code as above */
int		_EXFUN(getdate_r, (const char *, struct tm *));
#endif /* HAVE_GETDATE */

/* defines for the opengroup specifications Derived from Issue 1 of the SVID.  */
extern __IMPORT long _timezone;
extern __IMPORT int _daylight;
extern __IMPORT char *_tzname[2];

/* POSIX defines the external tzname being defined in time.h */
#ifndef tzname
#define tzname _tzname
#endif
#endif /* !__STRICT_ANSI__ */

#ifdef __cplusplus
}
#endif

#include <sys/features.h>

#ifdef __CYGWIN__
#include <cygwin/time.h>
#endif /*__CYGWIN__*/

#if defined(_POSIX_TIMERS)

#include <signal.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Clocks, P1003.1b-1993, p. 263 */

int _EXFUN(clock_settime, (clockid_t clock_id, const struct timespec *tp));
int _EXFUN(clock_gettime, (clockid_t clock_id, struct timespec *tp));
int _EXFUN(clock_getres,  (clockid_t clock_id, struct timespec *res));

/* Create a Per-Process Timer, P1003.1b-1993, p. 264 */

int _EXFUN(timer_create,
  (clockid_t clock_id, struct sigevent *evp, timer_t *timerid));

/* Delete a Per_process Timer, P1003.1b-1993, p. 266 */

int _EXFUN(timer_delete, (timer_t timerid));

/* Per-Process Timers, P1003.1b-1993, p. 267 */

int _EXFUN(timer_settime,
  (timer_t timerid, int flags, const struct itimerspec *value,
   struct itimerspec *ovalue));
int _EXFUN(timer_gettime, (timer_t timerid, struct itimerspec *value));
int _EXFUN(timer_getoverrun, (timer_t timerid));

/* High Resolution Sleep, P1003.1b-1993, p. 269 */

int _EXFUN(nanosleep, (const struct timespec  *rqtp, struct timespec *rmtp));

#ifdef __cplusplus
}
#endif
#endif /* _POSIX_TIMERS */

#ifdef __cplusplus
extern "C" {
#endif

/* CPU-time Clock Attributes, P1003.4b/D8, p. 54 */

/* values for the clock enable attribute */

#define CLOCK_ENABLED  1  /* clock is enabled, i.e. counting execution time */
#define CLOCK_DISABLED 0  /* clock is disabled */

/* values for the pthread cputime_clock_allowed attribute */

#define CLOCK_ALLOWED    1 /* If a thread is created with this value a */
                           /*   CPU-time clock attached to that thread */
                           /*   shall be accessible. */
#define CLOCK_DISALLOWED 0 /* If a thread is created with this value, the */
                           /*   thread shall not have a CPU-time clock */
                           /*   accessible. */

/* Manifest Constants, P1003.1b-1993, p. 262 */

#define CLOCK_REALTIME (clockid_t)1

/* Flag indicating time is "absolute" with respect to the clock
   associated with a time.  */

#define TIMER_ABSTIME	4

/* Manifest Constants, P1003.4b/D8, p. 55 */

#if defined(_POSIX_CPUTIME)

/* When used in a clock or timer function call, this is interpreted as
   the identifier of the CPU_time clock associated with the PROCESS
   making the function call.  */

#define CLOCK_PROCESS_CPUTIME (clockid_t)2

#endif

#if defined(_POSIX_THREAD_CPUTIME)

/*  When used in a clock or timer function call, this is interpreted as
    the identifier of the CPU_time clock associated with the THREAD
    making the function call.  */

#define CLOCK_THREAD_CPUTIME (clockid_t)3

#endif

#if defined(_POSIX_CPUTIME)

/* Accessing a Process CPU-time CLock, P1003.4b/D8, p. 55 */

int _EXFUN(clock_getcpuclockid, (pid_t pid, clockid_t *clock_id));

#endif /* _POSIX_CPUTIME */

#if defined(_POSIX_CPUTIME) || defined(_POSIX_THREAD_CPUTIME)

/* CPU-time Clock Attribute Access, P1003.4b/D8, p. 56 */

int _EXFUN(clock_setenable_attr, (clockid_t clock_id, int attr));
int _EXFUN(clock_getenable_attr, (clockid_t clock_id, int *attr));

#endif /* _POSIX_CPUTIME or _POSIX_THREAD_CPUTIME */

#ifdef __cplusplus
}
#endif

#endif /* _TIME_H_ */

