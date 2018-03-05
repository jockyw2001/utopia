#ifndef	_MACHTIME_H_
#define	_MACHTIME_H_

#if defined(__rtems__)
#define _CLOCKS_PER_SEC_  sysconf(_SC_CLK_TCK)
#elif defined(__arm__) || defined(__thumb__)
#define _CLOCKS_PER_SEC_ 100
#elif defined(__aeon__)
#define _CLOCKS_PER_SEC_ 1000000
#if defined(AEONSIM) || defined(XGEN)
typedef long long cycle_t;
cycle_t cycle();
#endif /* AEONSIM || XGEN */
#endif

#endif	/* _MACHTIME_H_ */

