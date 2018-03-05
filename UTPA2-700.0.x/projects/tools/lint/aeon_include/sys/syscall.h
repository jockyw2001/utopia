#define SYS_exit	1
#define SYS_fork	2
#define SYS_read	3
#define SYS_write	4
#define SYS_open	5
#define SYS_close	6
#define SYS_wait4	7
#define SYS_creat	8
#define SYS_link	9
#define SYS_unlink	10
#define SYS_execv	11
#define SYS_chdir	12
#define SYS_mknod	14
#define SYS_chmod	15
#define SYS_chown	16
#define SYS_lseek	19
#define SYS_getpid	20
#define SYS_isatty	21
#define SYS_fstat	22
#define SYS_time	23
#define SYS_times	24


#define SYS_ARG		25

#define NOP_AEONSIM_BREAK               0x6600  // void break();
#define NOP_AEONSIM_SET_ARGV_BLOCK      0x6601  // void __set_argv_block(void *p, int size);
#define NOP_AEONSIM_ARGC                0x6602  // int __argc()
#define NOP_AEONSIM_ARGV                0x6603  // char **__argv()
#define NOP_AEONSIM_STDIN               0x6604  // FILE *stdin()
#define NOP_AEONSIM_STDOUT              0x6605  // FILE *stdout()
#define NOP_AEONSIM_STDERR              0x6606  // FILE *stderr()

#define NOP_AEONSIM_JUMP_TO_ZERO        0x6607  // void __jump_to_zero()
#define NOP_AEONSIM_UNHANDLED_EXCEPTION 0x6608  // void __unhandled_expcetion(int handler_addr)
#define NOP_AEONSIM_TIMER0_START        0x6950  // void __timer0_start()
#define NOP_AEONSIM_TIMER0_STOP         0x6960  // void __timer0_stop()
#define NOP_AEONSIM_TIMER0_CANCEL       0x6970  // void __timer0_cancel()
#define NOP_AEONSIM_COUNTER0_INCREMENT  0x6980  // void __counter0_increment()

