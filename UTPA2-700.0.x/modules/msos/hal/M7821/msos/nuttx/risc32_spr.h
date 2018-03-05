#ifndef __RISC32_SPR_H__
#define __RISC32_SPR_H__

#if defined (__arm__)
#define set_cpsr(value) asm volatile ("msr cpsr, %0": :"r"(value))
#define get_cpsr() \
    ({  \
        unsigned long value; \
        asm volatile ("mrs\t\t%0,cpsr": "=r"(value) : ); \
        value; \
     })
#define mem_barrier(value) asm volatile ("mcr p15, 0, %0, c7, c10, 4": :"r"(value))

#elif defined (__mips__)

#define mtsr(value) asm volatile (" mtc0 %0, $12" : : "r" (value))
#define mfsr() \
    ({ \
        unsigned long value; \
        asm volatile ("mfc0\t\t%0,$12" : "=r" (value) : ); \
        value; \
    })

#define mtcause(value) asm volatile (" mtc0 %0, $13" : : "r" (value))
#define mfcause() \
    ({ \
        unsigned long value; \
        asm volatile ("mfc0\t\t%0,$13" : "=r" (value) : ); \
        value; \
    })

#define mtepc(value) asm volatile (" mtc0 %0, $14" : : "r" (value))
#define mfepc() \
    ({ \
        unsigned long value; \
        asm volatile ("mfc0\t\t%0,$14" : "=r" (value) : ); \
        value; \
    })

#elif defined (__aarch64__)
#else
/*
 * macros for SPR access
 *
 */
#define mtspr(spr, value) \
    __asm__ __volatile__ ("l.mtspr\t\t%0,%1,0" : : "r" (spr), "r" (value))

#define mfspr(spr) \
    ({ \
        unsigned long value; \
        __asm__ __volatile__ ("l.mfspr\t\t%0,%1,0" : "=r" (value) : "r" (spr) : "memory"); \
        value; \
    })
#endif

#define MAX_GRPS                (32)
#define MAX_SPRS_PER_GRP_BITS   (11)
#define MAX_SPRS_PER_GRP        (1 << MAX_SPRS_PER_GRP_BITS)
#define MAX_SPRS                (0x10000)

/* Base addresses for the groups */
#define SPRGROUP_SYS            (0 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_DMMU           (1 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_IMMU           (2 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_DC             (3 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_IC             (4 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_MAC            (5 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_D              (6 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_PC             (7 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_PM             (8 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_PIC            (9 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_TT             (10<< MAX_SPRS_PER_GRP_BITS)

/* System control and status group */
#define SPR_VR                  (SPRGROUP_SYS + 0)
#define SPR_UPR                 (SPRGROUP_SYS + 1)
#define SPR_CPUCFGR             (SPRGROUP_SYS + 2)
#define SPR_DMMUCFGR            (SPRGROUP_SYS + 3)
#define SPR_IMMUCFGR            (SPRGROUP_SYS + 4)
#define SPR_DCCFGR              (SPRGROUP_SYS + 5)
#define SPR_ICCFGR              (SPRGROUP_SYS + 6)
#define SPR_DCFGR               (SPRGROUP_SYS + 7)
#define SPR_PCCFGR              (SPRGROUP_SYS + 8)
#define SPR_NPC                 (SPRGROUP_SYS + 16)
#define SPR_SR                  (SPRGROUP_SYS + 17)
#define SPR_PPC                 (SPRGROUP_SYS + 18)
#define SPR_EPCR_BASE           (SPRGROUP_SYS + 32)
#define SPR_EPCR_LAST           (SPRGROUP_SYS + 47)
#define SPR_EEAR_BASE           (SPRGROUP_SYS + 48)
#define SPR_EEAR_LAST           (SPRGROUP_SYS + 63)
#define SPR_ESR_BASE            (SPRGROUP_SYS + 64)
#define SPR_ESR_LAST            (SPRGROUP_SYS + 79)

/* Data cache group */
#define SPR_DCCR                (SPRGROUP_DC + 0)
#define SPR_DCBPR               (SPRGROUP_DC + 1)
#define SPR_DCBFR               (SPRGROUP_DC + 2)
#define SPR_DCBIR               (SPRGROUP_DC + 3)
#define SPR_DCBWR               (SPRGROUP_DC + 4)
#define SPR_DCBLR               (SPRGROUP_DC + 5)
#define SPR_DCR_BASE(WAY)       (SPRGROUP_DC + 0x200 + (WAY) * 0x200)
#define SPR_DCR_LAST(WAY)       (SPRGROUP_DC + 0x3ff + (WAY) * 0x200)

/* Instruction cache group */
#define SPR_ICCR                (SPRGROUP_IC + 0)
#define SPR_ICBPR               (SPRGROUP_IC + 1)
#define SPR_ICBIR               (SPRGROUP_IC + 2)
#define SPR_ICBLR               (SPRGROUP_IC + 3)
#define SPR_ICR_BASE(WAY)       (SPRGROUP_IC + 0x200 + (WAY) * 0x200)
#define SPR_ICR_LAST(WAY)       (SPRGROUP_IC + 0x3ff + (WAY) * 0x200)

/* MAC group */
#define SPR_MACLO               (SPRGROUP_MAC + 1)
#define SPR_MACHI               (SPRGROUP_MAC + 2)
#define SPR_MACHI2              (SPRGROUP_MAC + 3)

/* PIC group */
#define SPR_PICMR               (SPRGROUP_PIC + 0)
#define SPR_PICPR               (SPRGROUP_PIC + 1)
#define SPR_PICSR               (SPRGROUP_PIC + 2)

/* Tick Timer group */
#define SPR_TTMR                (SPRGROUP_TT + 0)
#define SPR_TTCR                (SPRGROUP_TT + 1)

/*
 * Bit definitions for the Supervision Register
 *
 */
#define SPR_SR_CID      0xf0000000  /* Context ID */
#define SPR_SR_SUMRA    0x00010000  /* Supervisor SPR read access */
#define SPR_SR_FO       0x00008000  /* Fixed one */
#define SPR_SR_EPH      0x00004000  /* Exception Prefix High */
#define SPR_SR_DSX      0x00002000  /* Delay Slot Exception */
#if 1//M10/M12 R2
#define SPR_SR_TDE      0x00001000  /* Trap Disable Exception */
#else
#define SPR_SR_OVE      0x00001000  /* Overflow flag Exception */
#endif
#define SPR_SR_OV       0x00000800  /* Overflow flag */
#define SPR_SR_CY       0x00000400  /* Carry flag */
#define SPR_SR_F        0x00000200  /* Condition Flag */
#define SPR_SR_CE       0x00000100  /* CID Enable */
#define SPR_SR_LEE      0x00000080  /* Little Endian Enable */
#define SPR_SR_IME      0x00000040  /* Instruction MMU Enable */
#define SPR_SR_DME      0x00000020  /* Data MMU Enable */
#define SPR_SR_ICE      0x00000010  /* Instruction Cache Enable */
#define SPR_SR_DCE      0x00000008  /* Data Cache Enable */
#define SPR_SR_IEE      0x00000004  /* Interrupt Exception Enable */
#define SPR_SR_TEE      0x00000002  /* Tick timer Exception Enable */
#define SPR_SR_SM       0x00000001  /* Supervisor Mode */

/*
 * Bit definitions for Data Cache Control register
 *
 */
#define SPR_DCCR_EW 0x000000ff  /* Enable ways */

/*
 * Bit definitions for Insn Cache Control register
 *
 */
#define SPR_ICCR_EW 0x000000ff  /* Enable ways */

/*
 * Bit definitions for PICMR
 *
 */
#define SPR_PICMR_IUM   0xfffffffc  /* Interrupt unmask */

/*
 * Bit definitions for PICPR
 *
 */
#define SPR_PICPR_IPRIO 0xfffffffc  /* Interrupt priority */

/*
 * Bit definitions for PICSR
 *
 */
#define SPR_PICSR_IS    0xffffffff  /* Interrupt status */

/*
 * Bit definitions for Tick Timer Control Register
 *
 */
#define SPR_TTCR_PERIOD 0x0fffffff  /* Time Period */
#define SPR_TTMR_PERIOD SPR_TTCR_PERIOD
#define SPR_TTMR_IP     0x10000000  /* Interrupt Pending */
#define SPR_TTMR_IE     0x20000000  /* Interrupt Enable */
#define SPR_TTMR_RT     0x40000000  /* Restart tick */
#define SPR_TTMR_SR     0x80000000  /* Single run */
#define SPR_TTMR_CR     0xc0000000  /* Continuous run */
#define SPR_TTMR_M      0xc0000000  /* Tick mode */

#endif /* __RISC32_SPR_H__ */
