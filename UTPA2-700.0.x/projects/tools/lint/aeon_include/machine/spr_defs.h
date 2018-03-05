#ifndef __SPR_DEFS_H
#define __SPR_DEFS_H

#if 1 //def __aeon__
#define MAX_GRPS (32)
#define MAX_SPRS_PER_GRP_BITS (11)
#define MAX_SPRS_PER_GRP (1 << MAX_SPRS_PER_GRP_BITS)
#define MAX_SPRS (0x10000)
#define SPR_GRP_OFF(spr)    ((spr) & (MAX_SPRS_PER_GRP - 1))
#define SPR_GRP(spr)        ((spr)&(-MAX_SPRS_PER_GRP))

/* Base addresses for the groups */
#define SPRGROUP_SYS	(0<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_DMMU	(1<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_IMMU	(2<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_DC	(3<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_IC	(4<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_MAC	(5<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_D	(6<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_PC	(7<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_PM	(8<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_PIC	(9<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_TT	(10<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_ACCEL	(11<< MAX_SPRS_PER_GRP_BITS)

#define SPR_GROUP_SYS   SPRGROUP_SYS
#define SPR_GROUP_PM    SPRGROUP_PM
#define SPR_GROUP_PIC   SPRGROUP_PIC
#define SPR_GROUP_TT    SPRGROUP_TT

/* System control and status group */
#define SPR_VR		(SPRGROUP_SYS + 0)
#define SPR_UPR		(SPRGROUP_SYS + 1)
#define SPR_CPUCFGR	(SPRGROUP_SYS + 2)
#define SPR_DMMUCFGR	(SPRGROUP_SYS + 3)
#define SPR_IMMUCFGR	(SPRGROUP_SYS + 4)
#define SPR_DCCFGR	(SPRGROUP_SYS + 5)
#define SPR_ICCFGR	(SPRGROUP_SYS + 6)
#define SPR_DCFGR	(SPRGROUP_SYS + 7)
#define SPR_PCCFGR	(SPRGROUP_SYS + 8)
#define SPR_NPC         (SPRGROUP_SYS + 16)  /* CZ 21/06/01 */
#define SPR_DRETURN	(SPRGROUP_SYS + 16)  /* CZ 21/06/01 */
#define SPR_SR		(SPRGROUP_SYS + 17)  /* CZ 21/06/01 */
#define SPR_PPC         (SPRGROUP_SYS + 18)  /* CZ 21/06/01 */
#define SPR_EPCR_BASE	(SPRGROUP_SYS + 32)  /* CZ 21/06/01 */
#define SPR_EPCR_LAST	(SPRGROUP_SYS + 47)  /* CZ 21/06/01 */
#define SPR_EEAR_BASE	(SPRGROUP_SYS + 48)
#define SPR_EEAR_LAST	(SPRGROUP_SYS + 63)
#define SPR_ESR_BASE	(SPRGROUP_SYS + 64)
#define SPR_ESR_LAST	(SPRGROUP_SYS + 79)
#define SPR_GPR(x)	(SPRGROUP_SYS + 0x400 + (x))

#define SPR_SYS_SR			SPR_GRP_OFF(SPR_SR)
#define SPR_SYS_ESR_BASE	SPR_GRP_OFF(SPR_ESR_BASE)
#define SPR_SYS_ESR_LAST	SPR_GRP_OFF(SPR_ESR_LAST)

/* Data MMU group */
#define SPR_DMMUCR	(SPRGROUP_DMMU + 0)
#define SPR_DTLBMR_BASE(WAY)	(SPRGROUP_DMMU + 0x200 + (WAY) * 0x100)
#define SPR_DTLBMR_LAST(WAY)	(SPRGROUP_DMMU + 0x27f + (WAY) * 0x100)
#define SPR_DTLBTR_BASE(WAY)	(SPRGROUP_DMMU + 0x280 + (WAY) * 0x100)
#define SPR_DTLBTR_LAST(WAY)	(SPRGROUP_DMMU + 0x2ff + (WAY) * 0x100)

/* Instruction MMU group */
#define SPR_IMMUCR	(SPRGROUP_IMMU + 0)
#define SPR_ITLBMR_BASE(WAY)	(SPRGROUP_IMMU + 0x200 + (WAY) * 0x100)
#define SPR_ITLBMR_LAST(WAY)	(SPRGROUP_IMMU + 0x27f + (WAY) * 0x100)
#define SPR_ITLBTR_BASE(WAY)	(SPRGROUP_IMMU + 0x280 + (WAY) * 0x100)
#define SPR_ITLBTR_LAST(WAY)	(SPRGROUP_IMMU + 0x2ff + (WAY) * 0x100)

/* Data cache group */
#define SPR_DCCR	(SPRGROUP_DC + 0)
#define SPR_DCBPR	(SPRGROUP_DC + 1)
#define SPR_DCBFR	(SPRGROUP_DC + 2)
#define SPR_DCBIR	(SPRGROUP_DC + 3)
#define SPR_DCBWR	(SPRGROUP_DC + 4)
#define SPR_DCBLR	(SPRGROUP_DC + 5)
#define SPR_DCR_BASE(WAY)	(SPRGROUP_DC + 0x200 + (WAY) * 0x200)
#define SPR_DCR_LAST(WAY)	(SPRGROUP_DC + 0x3ff + (WAY) * 0x200)

/* Instruction cache group */
#define SPR_ICCR	(SPRGROUP_IC + 0)
#define SPR_ICBPR	(SPRGROUP_IC + 1)
#define SPR_ICBIR	(SPRGROUP_IC + 2)
#define SPR_ICBLR	(SPRGROUP_IC + 3)
#define SPR_ICR_BASE(WAY)	(SPRGROUP_IC + 0x200 + (WAY) * 0x200)
#define SPR_ICR_LAST(WAY)	(SPRGROUP_IC + 0x3ff + (WAY) * 0x200)

/* MAC group */
#define SPR_MACLO 	  (SPRGROUP_MAC + 1)
#define SPR_MACHI 	  (SPRGROUP_MAC + 2)
#define SPR_MACHI2    (SPRGROUP_MAC + 3)

#define SPR_AMULHI	  (SPRGROUP_MAC + 8)
#define SPR_AMACLO0	  (SPRGROUP_MAC + 9)
#define SPR_AMACHI0	  (SPRGROUP_MAC + 10)
#define SPR_AMACLO1	  (SPRGROUP_MAC + 11)
#define SPR_AMACHI1	  (SPRGROUP_MAC + 12)
#define SPR_AMACCFG   (SPRGROUP_MAC + 13)
#define SPR_AMACSTATUS  (SPRGROUP_MAC + 14)

#define SPR_AMACGUARD0 (SPRGROUP_MAC + 5)
#define SPR_AMACGUARD1 (SPRGROUP_MAC + 6)

#define SPR_AMACQ0					(SPRGROUP_MAC + 16)
#define SPR_AMACQ0_ADDROFS			(SPRGROUP_MAC + 17)
#define SPR_AMACQ0_STRIDE			(SPRGROUP_MAC + 18)
#define SPR_AMACQ0_STRIDE2			(SPRGROUP_MAC + 19)
#define SPR_AMACQ0_ADDROFS_STRIDE	(SPRGROUP_MAC + 20)

#define SPR_AMACQ1					(SPRGROUP_MAC + 24)
#define SPR_AMACQ1_ADDROFS			(SPRGROUP_MAC + 25)
#define SPR_AMACQ1_STRIDE			(SPRGROUP_MAC + 26)
#define SPR_AMACQ1_STRIDE2			(SPRGROUP_MAC + 27)
#define SPR_AMACQ1_ADDROFS_STRIDE	(SPRGROUP_MAC + 28)

#define AMACQSTRIDE_BITS	8
#define MK_QADDR_STRIDE(addr_offset, stride, stride2)    \
    ( ((unsigned)(addr_offset)<<(AMACQSTRIDE_BITS+AMACQSTRIDE_BITS)) |  \
      (((unsigned)(stride2) & ((1<<AMACQSTRIDE_BITS)-1)) << AMACQSTRIDE_BITS) | \
      ((unsigned)(stride) & ((1<<AMACQSTRIDE_BITS)-1)) )

typedef struct
{
	union
	{
		struct
		{
#if defined(HAS_EXECUTION) && defined(WORDS_BIGENDIAN)
			unsigned addr_offset:32-2*AMACQSTRIDE_BITS;
			int stride2:AMACQSTRIDE_BITS;	// need to multiply by sizeof(item)
			int stride:AMACQSTRIDE_BITS;	// need to multiply by sizeof(item)
#else
			int stride:AMACQSTRIDE_BITS;
			int stride2:AMACQSTRIDE_BITS;
			unsigned addr_offset:32-2*AMACQSTRIDE_BITS;
#endif
		};
		unsigned v;
	};
} qaddr_stride_t;

typedef enum
{
    amac_round_to_infinity = 0,
    amac_round_away_from_zero = 1
} amac_round_mode_e;

typedef enum
{
    amac_saturate_dont_care = 0,
    amac_saturate_to_64bit_before_shift = 0,
    amac_saturate_to_64bit_after_shift = 1,
    amac_saturate_to_32bit_after_shift = 2,
    amac_saturate_to_16bit_after_shift = 3
} amac_saturate_mode_e;

typedef struct
{
	union
	{
		struct
		{
#if defined(HAS_EXECUTION) && defined(WORDS_BIGENDIAN)
            unsigned __resv4:13;
            unsigned a1_round_mode:1;
            unsigned a1_saturate_mode:2;
            unsigned __resv3:5;
            unsigned a0_round_mode:1;
            unsigned a0_saturate_mode:2;
            unsigned __resv2:5;
            unsigned fmode:1;
            unsigned __resv1:1;
			unsigned enabled:1;
#else
			unsigned enabled:1;
            unsigned __resv1:1;
            unsigned fmode:1;
            unsigned __resv2:5;
            unsigned a0_saturate_mode:2;
            unsigned a0_round_mode:1;
            unsigned __resv3:5;
            unsigned a1_saturate_mode:2;
            unsigned a1_round_mode:1;
            unsigned __resv4:13;
#endif
		};
		unsigned v;
	};
} amac_cfg_t;

typedef struct
{
	union
	{
		struct
		{
#if defined(HAS_EXECUTION) && defined(WORDS_BIGENDIAN)
            unsigned __resv1:31;
			unsigned ov:1;
#else
			unsigned ov:1;
            unsigned __resv1:31;
#endif
		};
		unsigned v;
	};
} amac_status_t;

/* Debug group */
#define SPR_DVR(N)	(SPRGROUP_D + (N))
#define SPR_DCR(N)	(SPRGROUP_D + 8 + (N))
#define SPR_DMR1	(SPRGROUP_D + 16)
#define SPR_DMR2	(SPRGROUP_D + 17)
#define SPR_DWCR0	(SPRGROUP_D + 18)
#define SPR_DWCR1	(SPRGROUP_D + 19)
#define SPR_DSR		(SPRGROUP_D + 20)
#define SPR_DRR		(SPRGROUP_D + 21)
#define SPR_DREASON	(SPRGROUP_D + 22)

/* Performance counters group */
#define SPR_PCCR(N)	(SPRGROUP_PC + (N))
#define SPR_PCMR(N)	(SPRGROUP_PC + 8 + (N))

/* Power management group */
#define SPR_PMR (SPRGROUP_PM + 0)

#define SPR_PM_PMR  SPR_GRP_OFF(SPR_PMR)

/* PIC group */
#define SPR_PICMR (SPRGROUP_PIC + 0)
#define SPR_PICPR (SPRGROUP_PIC + 1)
#define SPR_PICSR (SPRGROUP_PIC + 2)

/* Tick Timer group */
#define SPR_TTMR (SPRGROUP_TT + 0)
#define SPR_TTCR (SPRGROUP_TT + 1)
#define SPR_TTUR (SPRGROUP_TT + 31)

#define SPR_TT_TTMR	    SPR_GRP_OFF(SPR_TTMR)
#define SPR_TT_TTCR		SPR_GRP_OFF(SPR_TTCR)

/* Accelerator group */
#define SPR_ACCEL_FFT		(SPRGROUP_ACCEL + 0)
#define SPR_ACCEL_HW_LOOP_COUNTER0	(SPRGROUP_ACCEL + 8)
#define SPR_ACCEL_HW_LOOP_COUNTER1	(SPRGROUP_ACCEL + 9)
#define SPR_ACCEL_HW_LOOP_COUNTER2	(SPRGROUP_ACCEL + 10)
#define SPR_ACCEL_HW_LOOP_COUNTER3	(SPRGROUP_ACCEL + 11)

#define FFT_ADDR_MODE_1_1       0
#define FFT_ADDR_MODE_2_2       1

typedef struct 
{
	union
	{
		struct
		{
			unsigned busy:1;
			unsigned __resv1:31;
		};
		unsigned v;
	};
} fft_status_t;

/*
 * Bit definitions for the Version Register
 *
 */
#define SPR_VR_VER	0xffff0000  /* Processor version */
#define SPR_VR_CFG	0x00ff0000  /* Processor configuration */
#define SPR_VR_REV	0x0000003f  /* Processor revision */

/*
 * Bit definitions for the Unit Present Register
 *
 */
#define SPR_UPR_UP	0x00000001  /* UPR present */
#define SPR_UPR_DCP	0x00000002  /* Data cache present */
#define SPR_UPR_ICP	0x00000004  /* Instruction cache present */
#define SPR_UPR_DMP	0x00000008  /* Data MMU present */
#define SPR_UPR_IMP	0x00000010  /* Instruction MMU present */
#define SPR_UPR_MP  0x00000020  /* MAC Present */
#define SPR_UPR_DUP	0x00000040  /* Debug unit present */
#define SPR_UPR_PCUP	0x00000080  /* Performance counters unit present */
#define SPR_UPR_PMP	0x00000100  /* Power management present */
#define SPR_UPR_PICP	0x00000200  /* PIC present */
#define SPR_UPR_TTP	0x00000400  /* Tick timer present */
#define SPR_UPR_CUST	0xff000000  /* Custom units */

/*
 * Bit definitions for the Supervision Register
 *
 */
#define SPR_SR_CID      0xf0000000  /* Context ID */
#define SPR_SR_SUMRA    0x00010000  /* Supervisor SPR read access */
#define SPR_SR_FO       0x00008000  /* Fixed one */
#define SPR_SR_EPH      0x00004000  /* Exception Prefix High */
#define SPR_SR_DSX      0x00002000  /* Delay Slot Exception */
#define SPR_SR_OVE      0x00001000  /* Overflow flag Exception */
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
 * Bit definitions for the Data MMU Control Register
 *
 */
#define SPR_DMMUCR_P2S	0x0000003e  /* Level 2 Page Size */
#define SPR_DMMUCR_P1S	0x000007c0  /* Level 1 Page Size */
#define SPR_DMMUCR_VADDR_WIDTH	0x0000f800  /* Virtual ADDR Width */
#define SPR_DMMUCR_PADDR_WIDTH	0x000f0000  /* Physical ADDR Width */

/*
 * Bit definitions for the Instruction MMU Control Register
 *
 */
#define SPR_IMMUCR_P2S	0x0000003e  /* Level 2 Page Size */
#define SPR_IMMUCR_P1S	0x000007c0  /* Level 1 Page Size */
#define SPR_IMMUCR_VADDR_WIDTH	0x0000f800  /* Virtual ADDR Width */
#define SPR_IMMUCR_PADDR_WIDTH	0x000f0000  /* Physical ADDR Width */

/*
 * Bit definitions for the Data TLB Match Register
 *
 */
#define SPR_DTLBMR_V	0x00000001  /* Valid */
#define SPR_DTLBMR_PL1	0x00000002  /* Page Level 1 (if 0 then PL2) */
#define SPR_DTLBMR_CID	0x0000003c  /* Context ID */
#define SPR_DTLBMR_LRU	0x000000c0  /* Least Recently Used */
#define SPR_DTLBMR_VPN	0xfffff000  /* Virtual Page Number */

/*
 * Bit definitions for the Data TLB Translate Register
 *
 */
#define SPR_DTLBTR_CC	0x00000001  /* Cache Coherency */
#define SPR_DTLBTR_CI	0x00000002  /* Cache Inhibit */
#define SPR_DTLBTR_WBC	0x00000004  /* Write-Back Cache */
#define SPR_DTLBTR_WOM	0x00000008  /* Weakly-Ordered Memory */
#define SPR_DTLBTR_A	0x00000010  /* Accessed */
#define SPR_DTLBTR_D	0x00000020  /* Dirty */
#define SPR_DTLBTR_URE	0x00000040  /* User Read Enable */
#define SPR_DTLBTR_UWE	0x00000080  /* User Write Enable */
#define SPR_DTLBTR_SRE	0x00000100  /* Supervisor Read Enable */
#define SPR_DTLBTR_SWE	0x00000200  /* Supervisor Write Enable */
#define SPR_DTLBTR_PPN	0xfffff000  /* Physical Page Number */

/*
 * Bit definitions for the Instruction TLB Match Register
 *
 */
#define SPR_ITLBMR_V	0x00000001  /* Valid */
#define SPR_ITLBMR_PL1	0x00000002  /* Page Level 1 (if 0 then PL2) */
#define SPR_ITLBMR_CID	0x0000003c  /* Context ID */
#define SPR_ITLBMR_LRU	0x000000c0  /* Least Recently Used */
#define SPR_ITLBMR_VPN	0xfffff000  /* Virtual Page Number */

/*
 * Bit definitions for the Instruction TLB Translate Register
 *
 */
#define SPR_ITLBTR_CC	0x00000001  /* Cache Coherency */
#define SPR_ITLBTR_CI	0x00000002  /* Cache Inhibit */
#define SPR_ITLBTR_WBC	0x00000004  /* Write-Back Cache */
#define SPR_ITLBTR_WOM	0x00000008  /* Weakly-Ordered Memory */
#define SPR_ITLBTR_A	0x00000010  /* Accessed */
#define SPR_ITLBTR_D	0x00000020  /* Dirty */
#define SPR_ITLBTR_SXE	0x00000040  /* User Read Enable */
#define SPR_ITLBTR_UXE	0x00000080  /* User Write Enable */
#define SPR_ITLBTR_PPN	0xfffff000  /* Physical Page Number */

/*
 * Bit definitions for Data Cache Control register
 *
 */
#define SPR_DCCR_EW	0x000000ff  /* Enable ways */

/*
 * Bit definitions for Insn Cache Control register
 *
 */
#define SPR_ICCR_EW	0x000000ff  /* Enable ways */

/*
 * Bit definitions for Debug Control register
 *
 */
#if 1
#define SPR_DCFGR_NDP	0x00000007
#define SPR_DCFGR_WPCI	0x00000008
#else
#define SPR_DCFGR_NDP   0x0000000f
#define SPR_DCFGR_WPCI  0x00000010
#endif


/*
 * Bit definitions for Data Cache Configuration Register
 *
 */

#define SPR_DCCFGR_NCW		0x00000007
#define SPR_DCCFGR_NCS		0x00000078
#define SPR_DCCFGR_CBS		0x00000080
#define SPR_DCCFGR_CWS		0x00000100
#define SPR_DCCFGR_CCRI		0x00000200
#define SPR_DCCFGR_CBIRI	0x00000400
#define SPR_DCCFGR_CBPRI	0x00000800
#define SPR_DCCFGR_CBLRI	0x00001000
#define SPR_DCCFGR_CBFRI	0x00002000
#define SPR_DCCFGR_CBWBRI	0x00004000

/*
 * Bit definitions for Instruction Cache Configuration Register
 *
 */
#define SPR_ICCFGR_NCW		0x00000007
#define SPR_ICCFGR_NCS		0x00000078
#define SPR_ICCFGR_CBS		0x00000080
#define SPR_ICCFGR_CCRI		0x00000200
#define SPR_ICCFGR_CBIRI	0x00000400
#define SPR_ICCFGR_CBPRI	0x00000800
#define SPR_ICCFGR_CBLRI	0x00001000

/*
 * Bit definitions for Data MMU Configuration Register
 *
 */

#define SPR_DMMUCFGR_NTW	0x00000003
#define SPR_DMMUCFGR_NTS	0x0000001C
#define SPR_DMMUCFGR_NAE	0x000000E0
#define SPR_DMMUCFGR_CRI	0x00000100
#define SPR_DMMUCFGR_PRI        0x00000200
#define SPR_DMMUCFGR_TEIRI	0x00000400
#define SPR_DMMUCFGR_HTR	0x00000800

/*
 * Bit definitions for Instruction MMU Configuration Register
 *
 */

#define SPR_IMMUCFGR_NTW	0x00000003
#define SPR_IMMUCFGR_NTS	0x0000001C
#define SPR_IMMUCFGR_NAE	0x000000E0
#define SPR_IMMUCFGR_CRI	0x00000100
#define SPR_IMMUCFGR_PRI	0x00000200
#define SPR_IMMUCFGR_TEIRI	0x00000400
#define SPR_IMMUCFGR_HTR	0x00000800

/*
 * Bit definitions for Debug Control registers
 *
 */
#define SPR_DCR_DP	0x00000001  /* DVR/DCR present */
#define SPR_DCR_CC	0x0000000e  /* Compare condition */
#define SPR_DCR_SC	0x00000010  /* Signed compare */
#define SPR_DCR_CT	0x000000e0  /* Compare to */

/* Bit results with SPR_DCR_CC mask */
#define SPR_DCR_CC_MASKED 0x00000000
#define SPR_DCR_CC_EQUAL  0x00000002
#define SPR_DCR_CC_LESS   0x00000004
#define SPR_DCR_CC_LESSE  0x00000006
#define SPR_DCR_CC_GREAT  0x00000008
#define SPR_DCR_CC_GREATE 0x0000000a
#define SPR_DCR_CC_NEQUAL 0x0000000c

/* Bit results with SPR_DCR_CT mask */
#define SPR_DCR_CT_DISABLED 0x00000000
#define SPR_DCR_CT_IFEA     0x00000020
#define SPR_DCR_CT_LEA      0x00000040
#define SPR_DCR_CT_SEA      0x00000060
#define SPR_DCR_CT_LD       0x00000080
#define SPR_DCR_CT_SD       0x000000a0
#define SPR_DCR_CT_LSEA     0x000000c0
#define SPR_DCR_CT_LSD		0x000000e0
/* SPR_DCR_CT_LSD doesn't seem to be implemented anywhere in or1ksim. 2004-1-30 HP */

/*
 * Bit definitions for Debug Mode 1 register
 *
 */
#define SPR_DMR1_CW0	0x00000003  /* Chain watchpoint 0 */
#define SPR_DMR1_CW1	0x0000000c  /* Chain watchpoint 1 */
#define SPR_DMR1_CW2	0x00000030  /* Chain watchpoint 2 */
#define SPR_DMR1_CW3	0x000000c0  /* Chain watchpoint 3 */
#define SPR_DMR1_CW4	0x00000300  /* Chain watchpoint 4 */
#define SPR_DMR1_CW5	0x00000c00  /* Chain watchpoint 5 */
#define SPR_DMR1_CW6	0x00003000  /* Chain watchpoint 6 */
#define SPR_DMR1_CW7	0x0000c000  /* Chain watchpoint 7 */
#define SPR_DMR1_CW8	0x00030000  /* Chain watchpoint 8 */
#define SPR_DMR1_CW9	0x000c0000  /* Chain watchpoint 9 */
#define SPR_DMR1_CW10	0x00300000  /* Chain watchpoint 10 */
#define SPR_DMR1_ST	0x00400000  /* Single-step trace*/
#define SPR_DMR1_BT	0x00800000  /* Branch trace */
#define SPR_DMR1_DXFW	0x01000000  /* Disable external force watchpoint */

/*
 * Bit definitions for Debug Mode 2 register
 *
 */
#define SPR_DMR2_WCE0	0x00000001  /* Watchpoint counter 0 enable */
#define SPR_DMR2_WCE1	0x00000002  /* Watchpoint counter 1 enable */
#define SPR_DMR2_AWTC	0x00000ffc  /* Assign watchpoints to counters */
#define SPR_DMR2_WGB	0x003ff000  /* Watchpoints generating breakpoint */
#define SPR_DMR2_WBS	0xffc00000  /* Watchpoints generated breakpoint */

/*
 * Bit definitions for Debug watchpoint counter registers
 *
 */
#define SPR_DWCR_COUNT	0x0000ffff  /* Count */
#define SPR_DWCR_MATCH	0xffff0000  /* Match */

/*
 * Bit definitions for Debug stop register
 *
 */
#define SPR_DSR_RSTE	0x00000001  /* Reset exception */
#define SPR_DSR_BUSEE	0x00000002  /* Bus error exception */
#define SPR_DSR_DPFE	0x00000004  /* Data Page Fault exception */
#define SPR_DSR_IPFE	0x00000008  /* Insn Page Fault exception */
#define SPR_DSR_TTE	0x00000010  /* Tick Timer exception */
#define SPR_DSR_AE	0x00000020  /* Alignment exception */
#define SPR_DSR_IIE	0x00000040  /* Illegal Instruction exception */
#define SPR_DSR_IE	0x00000080  /* Interrupt exception */
#define SPR_DSR_DME	0x00000100  /* DTLB miss exception */
#define SPR_DSR_IME	0x00000200  /* ITLB miss exception */
#define SPR_DSR_RE	0x00000400  /* Range exception */
#define SPR_DSR_SCE	0x00000800  /* System call exception */
#define SPR_DSR_FPE     0x00001000  /* Floating Point Exception */
#define SPR_DSR_TE	0x00002000  /* Trap exception */

/*
 * Bit definitions for Debug reason register
 *
 */
#define SPR_DRR_RSTE	0x00000001  /* Reset exception */
#define SPR_DRR_BUSEE	0x00000002  /* Bus error exception */
#define SPR_DRR_DPFE	0x00000004  /* Data Page Fault exception */
#define SPR_DRR_IPFE	0x00000008  /* Insn Page Fault exception */
#define SPR_DRR_TTE	0x00000010  /* Tick Timer exception */
#define SPR_DRR_AE	0x00000020  /* Alignment exception */
#define SPR_DRR_IIE	0x00000040  /* Illegal Instruction exception */
#define SPR_DRR_IE	0x00000080  /* Interrupt exception */
#define SPR_DRR_DME	0x00000100  /* DTLB miss exception */
#define SPR_DRR_IME	0x00000200  /* ITLB miss exception */
#define SPR_DRR_RE	0x00000400  /* Range exception */
#define SPR_DRR_SCE	0x00000800  /* System call exception */
#define SPR_DRR_FPE     0x00001000  /* Floating Point Exception */
#define SPR_DRR_TE	0x00002000  /* Trap exception */

/*
 * Bit definitions for Performance counters mode registers
 *
 */
#define SPR_PCMR_CP	0x00000001  /* Counter present */
#define SPR_PCMR_UMRA	0x00000002  /* User mode read access */
#define SPR_PCMR_CISM	0x00000004  /* Count in supervisor mode */
#define SPR_PCMR_CIUM	0x00000008  /* Count in user mode */
#define SPR_PCMR_LA	0x00000010  /* Load access event */
#define SPR_PCMR_SA	0x00000020  /* Store access event */
#define SPR_PCMR_IF	0x00000040  /* Instruction fetch event*/
#define SPR_PCMR_DCM	0x00000080  /* Data cache miss event */
#define SPR_PCMR_ICM	0x00000100  /* Insn cache miss event */
#define SPR_PCMR_IFS	0x00000200  /* Insn fetch stall event */
#define SPR_PCMR_LSUS	0x00000400  /* LSU stall event */
#define SPR_PCMR_BS	0x00000800  /* Branch stall event */
#define SPR_PCMR_DTLBM	0x00001000  /* DTLB miss event */
#define SPR_PCMR_ITLBM	0x00002000  /* ITLB miss event */
#define SPR_PCMR_DDS	0x00004000  /* Data dependency stall event */
#define SPR_PCMR_WPE	0x03ff8000  /* Watchpoint events */

/* 
 * Bit definitions for the Power management register
 *
 */
#define SPR_PMR_SDF	0x0000000f  /* Slow down factor */
#define SPR_PMR_DME	0x00000010  /* Doze mode enable */
#define SPR_PMR_SME	0x00000020  /* Sleep mode enable */
#define SPR_PMR_DCGE	0x00000040  /* Dynamic clock gating enable */
#define SPR_PMR_SUME	0x00000080  /* Suspend mode enable */

/*
 * Bit definitions for PICMR
 *
 */
#define SPR_PICMR_IUM	0xfffffffc  /* Interrupt unmask */

/*
 * Bit definitions for PICPR
 *
 */
#define SPR_PICPR_IPRIO	0xfffffffc  /* Interrupt priority */

/*
 * Bit definitions for PICSR
 *
 */
#define SPR_PICSR_IS	0xffffffff  /* Interrupt status */

/*
 * Bit definitions for Tick Timer Control Register
 *
 */
#define SPR_TTCR_PERIOD	0x0fffffff  /* Time Period */
#define SPR_TTMR_PERIOD	SPR_TTCR_PERIOD
#define SPR_TTMR_IP	0x10000000  /* Interrupt Pending */
#define SPR_TTMR_IE	0x20000000  /* Interrupt Enable */
#define SPR_TTMR_RT	0x40000000  /* Restart tick */
#define SPR_TTMR_SR     0x80000000  /* Single run */
#define SPR_TTMR_CR     0xc0000000  /* Continuous run */
#define SPR_TTMR_M      0xc0000000  /* Tick mode */

#define SPR_TTUR_ICTM   0x00000001  /* Instruction Counting Timer Mode */

/*
 * l.nop constants
 *
 */
#define NOP_NOP         0x0000      /* Normal nop instruction */
#define NOP_EXIT        0x0001      /* End of simulation */
#define NOP_REPORT      0x0002      /* Simple report */
#define NOP_PRINTF      0x0003      /* Simprintf instruction */
#define NOP_CNT_RESET   0x0005	    /* Reset statistics counters */
#define NOP_SIM_SYS_CALL 0x0006     /* Simulator syscall emulation */
#define NOP_REPORT_FIRST 0x0400     /* Report with number */
#define NOP_REPORT_LAST 0x03ff      /* Report with number */


#ifndef HAZARD
#define HAZARD(x)       x
#endif

#define L_FF1(x)                                            \
({  int _res_, _x_ = x; __asm__ volatile ("l.ff1\t%0,%1" : "=r"(_res_) : "r"(_x_)); _res_; })

#define L_FL1(x)                                            \
({  int _res_, _x_ = x; __asm__ volatile ("l.fl1\t%0,%1" : "=r"(_res_) : "r"(_x_)); _res_; })

/* Move from architecture special register (SPR) */
#define __l_mfspr__(result, spr_grp, spr_reg)               \
    __asm__ volatile ("l.mfspr\t%0,%1,%2" : "=r"(result) : "r"(spr_reg), "K"(spr_grp));
#define __l_mfspr1__(result, spr)                           \
    __asm__ volatile ("l.mfspr\t%0,r0,%1" : "=r"(result) : "K"(spr))
#define __l_mtspr__(spr_grp, spr_reg, val)                  \
    __asm__ volatile ("l.mtspr\t%0,%1,%2" : : "r"(spr_reg), "r"(val), "K"(spr_grp));
#define __l_mtspr1__(spr,val)                               \
    __asm__ volatile ("l.mtspr\tr0,%0,%1" : : "r"(val), "K"(spr))

#define L_MFSPR(_spr_group_5bit_shifted_by_11bit_, _spr_register_11bit_const_) \
({  int _res_; __l_mfspr__(_res_, _spr_group_5bit_shifted_by_11bit_, _spr_register_11bit_const_); _res_; })

#define L_MFSPR1(_spr_register_16bit_const_)                \
({  int _res_; __l_mfspr1__(_res_, _spr_register_16bit_const_); _res_; })

/* Move data to architecture special registers (SPR) */
#define L_MTSPR(_spr_group_5bit_shifted_by_11bit_, _spr_register_11bit_const_, _val_) \
    __l_mtspr__(_spr_group_5bit_shifted_by_11bit_, _spr_register_11bit_const_, _val_)

#define L_MTSPR1(_spr_register_16bit_const_, _val_)         \
    __l_mtspr1__(_spr_register_16bit_const_, _val_)

/* Enhancement to existing multiply (keep full 64-bit) */
#define __l_mul__(lo, a,b)                  \
   { int _a_ = (a), _b_ = (b); __asm__ volatile ("l.mul\t%0,%1,%2" : "=r"(lo) : "r"(_a_), "r"(_b_)); }
#define __l_muli__(lo, a,b)                 \
   { int _a_ = (a); __asm__ volatile ("l.muli\t%0,%1,%2" : "=r"(lo) : "r"(_a_), "I"(b)); }
#define __l_mulu__(lo, a,b)                 \
   { unsigned _a_ = (a), _b_ = (b); __asm__ volatile ("l.mulu\t%0,%1,%2" : "=r"(lo) : "r"(_a_), "r"(_b_)); }

/* signed 32x32 to signed 64-bit multiply */
#define L_MULL(a,b)                         \
({  int _lo_; __l_mul__(_lo_, a,b);         \
    int _hi_; __l_mfspr1__(_hi_, SPR_MACHI2); \
    ((long long)_hi_<<32)|(unsigned)_lo_; })

#define L_MULL_LO(a,b)                      \
({  int _lo_; __l_mul__(_lo_, a,b); _lo_; })

#define L_MULLI(a,b)                        \
({  int _lo_; __l_muli__(_lo_, a,b);        \
    int _hi_; __l_mfspr1__(_hi_, SPR_MACHI2); \
    ((long long)_hi_<<32)|(unsigned)_lo_; })

#define L_MULLI_LO(a,b)                     \
({  int _lo_; __l_muli__(_lo_, a,b); _lo_; })

#define L_MULLU(a,b)                        \
({  unsigned _lo_; __l_mulu__(_lo_, a,b);   \
    unsigned _hi_; __l_mfspr1__(_hi_, SPR_MACHI2); \
    ((unsigned long long)_hi_<<32)|(unsigned)_lo_; })

#define L_MULLU_LO(a,b)                     \
({  int _lo_; __l_mulu__(_lo_, a,b); _lo_; })


#define __l_amull__(lo, a,b)                \
   { int _a_ = (a), _b_ = (b); __asm__ volatile ("l.amull\t%0,%1,%2" : "=r"(lo) : "r"(_a_), "r"(_b_)); }
#define __l_amullu__(lo, a,b)                \
   { int _a_ = (a), _b_ = (b); __asm__ volatile ("l.amullu\t%0,%1,%2" : "=r"(lo) : "r"(_a_), "r"(_b_)); }

/* Pipelined version of signed 32x32 multiply to 64-bit */
#define L_AMULL(a,b)                        \
({  int _lo_; __l_amull__(_lo_, a,b);       \
    int _hi_; __l_mfspr1__(_hi_, SPR_AMULHI); \
    ((long long)_hi_<<32)|(unsigned)_lo_; })

#define L_AMULL_LO(a,b)                     \
({  int _lo_; __l_amull__(_lo_, a,b); _lo_; })

/* Pipelined version of unsigned 32x32 multiply to 64-bit  */
#define L_AMULLU(a,b)                       \
({  unsigned _lo_; __l_amullu__(_lo_, a,b); \
    unsigned _hi_; __l_mfspr1__(_hi_, SPR_AMULHI); \
    ((unsigned long long)_hi_<<32)|(unsigned)_lo_; })

#define L_AMULLU_LO(a,b)                    \
({  unsigned _lo_; __l_amullu__(_lo_, a,b); _lo_; })


#define AMAC0       0
#define AMAC1       1
#define AMAC_Q0     0
#define AMAC_Q1     1

#define __l_amul__(m, a,b)                      \
   { int _a_ = (a), _b_ = (b); __asm__ volatile ("l.amul\ta" #m ",%0,%1" : : "r"(_a_), "r"(_b_)); }
#define __l_amac__(m, a,b)                      \
   { int _a_ = (a), _b_ = (b); __asm__ volatile ("l.amac\ta" #m ",%0,%1" : : "r"(_a_), "r"(_b_)); }
#define __l_amsb__(m, a,b)                      \
   { int _a_ = (a), _b_ = (b); __asm__ volatile ("l.amsb\ta" #m ",%0,%1" : : "r"(_a_), "r"(_b_)); }
#define __l_amulq__(m, a,q)                     \
   { int _a_ = (a); __asm__ volatile ("l.amulq\ta" #m ",%0,q" #q : : "r"(_a_)); }
#define __l_amacq__(m, a,q)                     \
   { int _a_ = (a); __asm__ volatile ("l.amacq\ta" #m ",%0,q" #q : : "r"(_a_)); }
#define __l_amsbq__(m, a,q)                     \
   { int _a_ = (a); __asm__ volatile ("l.amsbq\ta" #m ",%0,q" #q : : "r"(_a_)); }

#define __l_amulu__(m, a,b)                      \
   { unsigned _a_ = (a), _b_ = (b); __asm__ volatile ("l.amulu\ta" #m ",%0,%1" : : "r"(_a_), "r"(_b_)); }
#define __l_amacu__(m, a,b)                      \
   { unsigned _a_ = (a), _b_ = (b); __asm__ volatile ("l.amacu\ta" #m ",%0,%1" : : "r"(_a_), "r"(_b_)); }
#define __l_amsbu__(m, a,b)                      \
   { unsigned _a_ = (a), _b_ = (b); __asm__ volatile ("l.amsbu\ta" #m ",%0,%1" : : "r"(_a_), "r"(_b_)); }
#define __l_amuluq__(m, a,q)                     \
   { unsigned _a_ = (a); __asm__ volatile ("l.amulqu\ta" #m ",%0,q" #q : : "r"(_a_)); }
#define __l_amacuq__(m, a,q)                     \
   { unsigned _a_ = (a); __asm__ volatile ("l.amacqu\ta" #m ",%0,q" #q : : "r"(_a_)); }
#define __l_amsbuq__(m, a,q)                     \
   { unsigned _a_ = (a); __asm__ volatile ("l.amsbqu\ta" #m ",%0,q" #q : : "r"(_a_)); }

#define __l_amul_rs__(lo, m,a,b, bits)          \
   { int _a_ = (a), _b_ = (b); __asm__ volatile ("l.amul_rs\t%0,a" #m ",%1,%2,%3" : "=r"(lo) : "r"(_a_), "r"(_b_), "K"(bits)); }
#define __l_amac_rs__(lo, m,a,b, bits)          \
   { int _a_ = (a), _b_ = (b); __asm__ volatile ("l.amac_rs\t%0,a" #m ",%1,%2,%3" : "=r"(lo) : "r"(_a_), "r"(_b_), "K"(bits)); }
#define __l_amsb_rs__(lo, m,a,b, bits)          \
   { int _a_ = (a), _b_ = (b); __asm__ volatile ("l.amsb_rs\t%0,a" #m ",%1,%2,%3" : "=r"(lo) : "r"(_a_), "r"(_b_), "K"(bits)); }
#define __l_amulq_rs__(lo, m,a,q, bits)         \
   { int _a_ = (a); __asm__ volatile ("l.amulq_rs\t%0,a" #m ",%1,q" #q ",%2": "=r"(lo) : "r"(_a_), "K"(bits)); }
#define __l_amacq_rs__(lo, m,a,q, bits)         \
   { int _a_ = (a); __asm__ volatile ("l.amacq_rs\t%0,a" #m ",%1,q" #q ",%2": "=r"(lo) : "r"(_a_), "K"(bits)); }
#define __l_amsbq_rs__(lo, m,a,q, bits)         \
   { int _a_ = (a); __asm__ volatile ("l.amsbq_rs\t%0,a" #m ",%1,q" #q ",%2": "=r"(lo) : "r"(_a_), "K"(bits)); }

#define __l_amacr_lo__(m, lo, mode, bits)       \
    __asm__ volatile ("l.amacr\t%0,a" #m ",%1,%2\t# CLR|RND|SHT|SAT" : "=r"(lo) : "K"(mode), "K"(bits))
#define __l_mfspr_amachi__(m, hi)               \
    __asm__ volatile ("l.mfspr\t%0,r0,%1\t# amac\ta" #m "hi" : "=r"(hi) : "K"(SPR_AMACHI0+2*m))
#define __l_mfspr_amacguard__(m, g)            \
    __asm__ volatile ("l.mfspr\t%0,r0,%1\t# amac\ta" #m "g" : "=r"(g) : "K"(SPR_AMACGUARD0+m))

/* Pipelined version of signed 32x32 mutiply to a 72-bit MAC */
#define L_AMUL(m, a,b)  __l_amul__(m, a,b)
#define L_AMULU(m, a,b) __l_amulu__(m, a,b)
#define L_AMUL_RS_LO(m, a,b, bits)  ({ int _lo_; __l_amul_rs__(_lo_, m,a,b, bits); _lo_; })

/* Pipelined version of signed 32x32 mutiply accumulated to a 72-bit MAC */
#define L_AMAC(m, a,b)  __l_amac__(m, a,b)
#define L_AMACU(m, a,b) __l_amacu__(m, a,b)
#define L_AMAC_RS_LO(m, a,b, bits)  ({ int _lo_; __l_amac_rs__(_lo_, m,a,b, bits); _lo_; })

/* Pipelined version of signed 32x32 mutiply substracted from a 72-bit MAC */
#define L_AMSB(m, a,b)  __l_amsb__(m, a,b)
#define L_AMSBU(m, a,b) __l_amsbu__(m, a,b)
#define L_AMSB_RS_LO(m, a,b, bits)  ({ int _lo_; __l_amsb_rs__(_lo_, m,a,b, bits); _lo_; })

/* Pipelined version of signed 32x32 register * amacq mutiply to a 72-bit MAC */
#define L_AMULQ(m, a,q)   __l_amulq__(m, a,q)
#define L_AMULUQ(m, a,q)  __l_amuluq__(m, a,q)
#define L_AMULQ_RS_LO(m, a,q, bits)  ({ int _lo_; __l_amulq_rs__(_lo_, m,a,q, bits); _lo_; })

/* Pipelined version of signed 32x32 register * amacq mutiply accumulated to a 72-bit MAC */
#define L_AMACQ(m, a,q)   __l_amacq__(m, a,q)
#define L_AMACUQ(m, a,q)  __l_amacuq__(m, a,q)
#define L_AMACQ_RS_LO(m, a,q, bits)  ({ int _lo_; __l_amacq_rs__(_lo_, m,a,q, bits); _lo_; })

/* Pipelined version of signed 32x32 register * amacq mutiply substracted from a 72-bit MAC */
#define L_AMSBQ(m, a,q)   __l_amsbq__(m, a,q)
#define L_AMSBUQ(m, a,q)  __l_amsbuq__(m, a,q)
#define L_AMSBQ_RS_LO(m, a,q, bits)  ({ int _lo_; __l_amsbq_rs__(_lo_, m,a,q, bits); _lo_; })

/* Pipelined version of setting a 72-bit MAC */
#define L_AMACW(m, hi, lo)                  \
   { int _hi_ = (hi), _lo_ = (lo); __asm__ volatile ("l.amacw\ta" #m ",%0,%1" : : "r"(_hi_), "r"(_lo_)); }
#define L_AMACW0(m)                         \
   { __asm__ volatile ("l.amacw\ta" #m ",r0,r0"); }

/* Pipelined version of reading a 72-bit MAC, optional round, shift, saturate, and clear (in that order)*/
#define AMACR_CLR   8   // clear
#define AMACR_RND   4   // round
#define AMACR_SAT   2   // saturate
#define AMACR_SHR   1   // shift

#if 0 // doesn't work because SPR_AMACHI/LO isn't updated when !AMACR_CLR, or cleared when AMACR_CLR.  In both case, _hi_ isn't what we want
#define L_AMACR(m, mode, bits)              \
({  int _lo_; __l_amacr_lo__(m, _lo_, mode, bits); \
    int _hi_; __l_mfspr_amachi__(m, _hi_);  \
    ((long long)_hi_<<32)|(unsigned)_lo_; })
#endif // 0

#define L_AMACR_LO(m, mode, bits)           \
({  int _lo_; __l_amacr_lo__(m, _lo_, mode, bits); _lo_; })

#define L_MFSPR_AMACHI(m)                   \
({  int _hi_; __l_mfspr_amachi__(m, _hi_); _hi; })

#define L_MFSPR_GUARD(m)                    \
({  int _g_; __l_mfspr_amacguard__(m, _g_); _g; })


/* Double-load */
#define __l_ldww__(ldww, q, mask, r, addr, offset)    \
    __asm__ volatile (ldww "\t%0,%1(%2),q" #q ",%3" : "=r"(r) : "I"(offset), "r"(addr), "K"(mask));

#define L_LDWW(q, mask, addr, offset)       \
({  int _r_; __l_ldww__("l.ldww", q, mask, _r_, addr, offset); _r_; })

#define L_LDWWX(q, mask, addr, offset)      \
({  int _r_; __l_ldww__("l.ldwwx", q, mask, _r_, addr, offset); _r_; })

#define L_LDWW2(q, mask, addr, offset)      \
({  int _r_; __l_ldww__("l.ldww2", q, mask, _r_, addr, offset); _r_; })

#define L_LDWW2X(q, mask, addr, offset)     \
({  int _r_; __l_ldww__("l.ldww2x", q, mask, _r_, addr, offset); _r_; })


/* AMAC combo */
#if 0 // doesn't work because SPR_AMACHI/LO isn't updated when !AMACR_CLR, or cleared when AMACR_CLR.  In both case, _hi_ isn't what we want
#define L_AMAC_p(m, a1,b1, mode, bits)      \
({  __l_amul__(m, a1,b1);                   \
    HAZARD(__asm__ volatile ("l.nop"));     \
    int _lo_; __l_amacr_lo__(m, _lo_, mode, bits); \
    int _hi_; __l_mfspr_amachi__(m, _hi_);  \
    ((long long)_hi_<<32)|(unsigned)_lo_; })

#define L_AMACQ_p(m, a1, q, mode, bits)     \
({  __l_amulq__(m, a1,q);                   \
    HAZARD(__asm__ volatile ("l.nop"));     \
    int _lo_; __l_amacr_lo__(m, _lo_, mode, bits); \
    int _hi_; __l_mfspr_amachi__(m, _hi_);  \
    ((long long)_hi_<<32)|(unsigned)_lo_; })

#define L_AMAC_pp(m, a1,b1, a2,b2, mode, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amac__(m, a2,b2);                   \
    HAZARD(__asm__ volatile ("l.nop"));     \
    int _lo_; __l_amacr_lo__(m, _lo_, mode, bits); \
    int _hi_; __l_mfspr_amachi__(m, _hi_);  \
    ((long long)_hi_<<32)|(unsigned)_lo_; })
#endif // 0

#define L_AMAC_p_LO(m, a1,b1, mode, bits)   \
({  __l_amul__(m, a1,b1);                   \
    HAZARD(__asm__ volatile ("l.nop"));     \
    int _lo_; __l_amacr_lo__(m, _lo_, mode, bits); _lo_; })

#define L_AMACQ_p_LO(m, a1, q, mode, bits)  \
({  __l_amulq__(m, a1,q);                   \
    HAZARD(__asm__ volatile ("l.nop"));     \
    int _lo_; __l_amacr_lo__(m, _lo_, mode, bits); _lo_; })

#define L_AMAC_pp_LO(m, a1,b1, a2,b2, mode, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amac__(m, a2,b2);                   \
    HAZARD(__asm__ volatile ("l.nop"));     \
    int _lo_; __l_amacr_lo__(m, _lo_, mode, bits); _lo_; })

#define L_AMACU_pp_LO(m, a1,b1, a2,b2, mode, bits) \
({  __l_amulu__(m, a1,b1);                  \
    __l_amacu__(m, a2,b2);                  \
    HAZARD(__asm__ volatile ("l.nop"));     \
    unsigned _lo_; __l_amacr_lo__(m, _lo_, mode, bits); _lo_; })

#define L_AMAC_pq_LO(m, a1,b1, a2,q2, mode, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amacq__(m, a2,q2);                  \
    HAZARD(__asm__ volatile ("l.nop"));     \
    int _lo_; __l_amacr_lo__(m, _lo_, mode, bits); _lo_; })

#define L_AMAC_pm_LO(m, a1,b1, a2,b2, mode, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amsb__(m, a2,b2);                   \
    HAZARD(__asm__ volatile ("l.nop"));     \
    int _lo_; __l_amacr_lo__(m, _lo_, mode, bits); _lo_; })

#define L_AMAC_ppp_LO(m, a1,b1, a2,b2, a3,b3, mode, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amac__(m, a2,b2);                   \
    __l_amac__(m, a3,b3);                   \
    HAZARD(__asm__ volatile ("l.nop"));     \
    int _lo_; __l_amacr_lo__(m, _lo_, mode, bits); _lo_; })

#define L_AMAC_ppm_LO(m, a1,b1, a2,b2, a3,b3, mode, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amac__(m, a2,b2);                   \
    __l_amsb__(m, a3,b3);                   \
    HAZARD(__asm__ volatile ("l.nop"));     \
    int _lo_; __l_amacr_lo__(m, _lo_, mode, bits); _lo_; })

#define L_AMAC_pmm_LO(m, a1,b1, a2,b2, a3,b3, mode, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amsb__(m, a2,b2);                   \
    __l_amsb__(m, a3,b3);                   \
    HAZARD(__asm__ volatile ("l.nop"));     \
    int _lo_; __l_amacr_lo__(m, _lo_, mode, bits); _lo_; })

#define L_AMAC_pppp_LO(m, a1,b1, a2,b2, a3,b3, a4,b4, mode, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amac__(m, a2,b2);                   \
    __l_amac__(m, a3,b3);                   \
    __l_amac__(m, a4,b4);                   \
    HAZARD(__asm__ volatile ("l.nop"));     \
    int _lo_; __l_amacr_lo__(m, _lo_, mode, bits); _lo_; })

#define L_AMAC_ppmm_LO(m, a1,b1, a2,b2, a3,b3, a4,b4, mode, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amac__(m, a2,b2);                   \
    __l_amsb__(m, a3,b3);                   \
    __l_amsb__(m, a4,b4);                   \
    HAZARD(__asm__ volatile ("l.nop"));     \
    int _lo_; __l_amacr_lo__(m, _lo_, mode, bits); _lo_; })

#define L_AMAC_pmpm_LO(m, a1,b1, a2,b2, a3,b3, a4,b4, mode, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amsb__(m, a2,b2);                   \
    __l_amac__(m, a3,b3);                   \
    __l_amsb__(m, a4,b4);                   \
    HAZARD(__asm__ volatile ("l.nop"));     \
    int _lo_; __l_amacr_lo__(m, _lo_, mode, bits); _lo_; })

#define L_AMAC_pmmp_LO(m, a1,b1, a2,b2, a3,b3, a4,b4, mode, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amsb__(m, a2,b2);                   \
    __l_amsb__(m, a3,b3);                   \
    __l_amac__(m, a4,b4);                   \
    HAZARD(__asm__ volatile ("l.nop"));     \
    int _lo_; __l_amacr_lo__(m, _lo_, mode, bits); _lo_; })

#define L_AMAC_ppppp_LO(m, a1,b1, a2,b2, a3,b3, a4,b4, a5,b5, mode, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amac__(m, a2,b2);                   \
    __l_amac__(m, a3,b3);                   \
    __l_amac__(m, a4,b4);                   \
    __l_amac__(m, a5,b5);                   \
    HAZARD(__asm__ volatile ("l.nop"));     \
    int _lo_; __l_amacr_lo__(m, _lo_, mode, bits); _lo_; })

#define L_AMAC_pppmm_LO(m, a1,b1, a2,b2, a3,b3, a4,b4, a5,b5, mode, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amac__(m, a2,b2);                   \
    __l_amac__(m, a3,b3);                   \
    __l_amsb__(m, a4,b4);                   \
    __l_amsb__(m, a5,b5);                   \
    HAZARD(__asm__ volatile ("l.nop"));     \
    int _lo_; __l_amacr_lo__(m, _lo_, mode, bits); _lo_; })

#define L_AMAC_pppppppppp_LO(m, a1,b1, a2,b2, a3,b3, a4,b4, a5,b5, a6,b6, a7,b7, a8,b8, a9,b9, a10,b10, mode, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amac__(m, a2,b2);                   \
    __l_amac__(m, a3,b3);                   \
    __l_amac__(m, a4,b4);                   \
    __l_amac__(m, a5,b5);                   \
    __l_amac__(m, a6,b6);                   \
    __l_amac__(m, a7,b7);                   \
    __l_amac__(m, a8,b8);                   \
    __l_amac__(m, a9,b9);                   \
    __l_amac__(m, a10,b10);                 \
    HAZARD(__asm__ volatile ("l.nop"));     \
    int _lo_; __l_amacr_lo__(m, _lo_, mode, bits); _lo_; })

/* AMAC combo: 1 instruction short, always shift-rounding */
#if 0 // doesn't work because SPR_AMACHI/LO isn't updated when !AMACR_CLR, or cleared when AMACR_CLR.  In both case, _hi_ isn't what we want
#define L_AMAC_RS_p(m, a1,b1, bits)         \
({  int _lo_; __l_amul_rs__(_lo_, m,a1,b1, bits); \
    int _hi_; __l_mfspr_amachi__(m, _hi_);  \
    ((long long)_hi_<<32)|(unsigned)_lo_; })

#define L_AMACQ_RS_p(m, a1, q, bits)        \
({  int _lo_; __l_amulq_rs__(_lo_, m,a1,q, bits); \
    int _hi_; __l_mfspr_amachi__(m, _hi_);  \
    ((long long)_hi_<<32)|(unsigned)_lo_; })

#define L_AMAC_RS_pp(m, a1,b1, a2,b2, bits) \
({  __l_amul__(m, a1,b1);                   \
    int _lo_; __l_amac_rs__(_lo_, m,a2,b2, bits); \
    int _hi_; __l_mfspr_amachi__(m, _hi_);  \
    ((long long)_hi_<<32)|(unsigned)_lo_; })
#endif // 0

#define L_AMAC_RS_p_LO(m, a1,b1, bits)      \
({  int _lo_; __l_amul_rs__(_lo_, m,a1,b1, bits); _lo_; })

#define L_AMACQ_RS_p_LO(m, a1, q, bits)     \
({  int _lo_; __l_amulq_rs__(_lo_, m,a1,q, bits); _lo_; })

#define L_AMAC_RS_pp_LO(m, a1,b1, a2,b2, bits) \
({  __l_amul__(m, a1,b1);                   \
    int _lo_; __l_amac_rs__(_lo_, m,a2,b2, bits); _lo_; })

#define L_AMAC_RS_pq_LO(m, a1,b1, a2,q2, bits) \
({  __l_amul__(m, a1,b1);                   \
    int _lo_; __l_amacq_rs__(_lo_, m,a2,q2, bits); _lo_; })

#define L_AMAC_RS_qq_LO(m, a1,q1, a2,q2, bits) \
({  __l_amulq__(m, a1,q1);                  \
    int _lo_; __l_amacq_rs__(_lo_, m,a2,q2, bits); _lo_; })

#define L_AMAC_RS_pm_LO(m, a1,b1, a2,b2, bits) \
({  __l_amul__(m, a1,b1);                   \
    int _lo_; __l_amsb_rs__(_lo_, m,a2,b2, bits); _lo_; })

#define L_AMAC_RS_qn_LO(m, a1,q1, a2,q2, bits) \
({  __l_amulq__(m, a1,q1);                   \
    int _lo_; __l_amsbq_rs__(_lo_, m,a2,q2, bits); _lo_; })

#define L_AMAC_RS_ppp_LO(m, a1,b1, a2,b2, a3,b3, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amac__(m, a2,b2);                   \
    int _lo_; __l_amac_rs__(_lo_, m,a3,b3, bits); _lo_; })

#define L_AMAC_RS_ppm_LO(m, a1,b1, a2,b2, a3,b3, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amac__(m, a2,b2);                   \
    int _lo_; __l_amsb_rs__(_lo_, m,a3,b3, bits); _lo_; })

#define L_AMAC_RS_pmm_LO(m, a1,b1, a2,b2, a3,b3, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amsb__(m, a2,b2);                   \
    int _lo_; __l_amsb_rs__(_lo_, m,a3,b3, bits); _lo_; })

#define L_AMAC_RS_pppp_LO(m, a1,b1, a2,b2, a3,b3, a4,b4, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amac__(m, a2,b2);                   \
    __l_amac__(m, a3,b3);                   \
    int _lo_; __l_amac_rs__(_lo_, m,a4,b4, bits); _lo_; })

#define L_AMAC_RS_ppmm_LO(m, a1,b1, a2,b2, a3,b3, a4,b4, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amac__(m, a2,b2);                   \
    __l_amsb__(m, a3,b3);                   \
    int _lo_; __l_amsb_rs__(_lo_, m,a4,b4, bits); _lo_; })

#define L_AMAC_RS_pmpm_LO(m, a1,b1, a2,b2, a3,b3, a4,b4, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amsb__(m, a2,b2);                   \
    __l_amac__(m, a3,b3);                   \
    int _lo_; __l_amsb_rs__(_lo_, m,a4,b4, bits); _lo_; })

#define L_AMAC_RS_pmmp_LO(m, a1,b1, a2,b2, a3,b3, a4,b4, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amsb__(m, a2,b2);                   \
    __l_amsb__(m, a3,b3);                   \
    int _lo_; __l_amac_rs__(_lo_, m,a4,b4, bits); _lo_; })

#define L_AMAC_RS_ppppp_LO(m, a1,b1, a2,b2, a3,b3, a4,b4, a5,b5, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amac__(m, a2,b2);                   \
    __l_amac__(m, a3,b3);                   \
    __l_amac__(m, a4,b4);                   \
    int _lo_; __l_amac_rs__(_lo_, m,a5,b5, bits); _lo_; })

#define L_AMAC_RS_pppmm_LO(m, a1,b1, a2,b2, a3,b3, a4,b4, a5,b5, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amac__(m, a2,b2);                   \
    __l_amac__(m, a3,b3);                   \
    __l_amsb__(m, a4,b4);                   \
    int _lo_; __l_amsb_rs__(_lo_, m,a5,b5, bits); _lo_; })

#define L_AMAC_RS_pppppp_LO(m, a1,b1, a2,b2, a3,b3, a4,b4, a5,b5, a6,b6, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amac__(m, a2,b2);                   \
    __l_amac__(m, a3,b3);                   \
    __l_amac__(m, a4,b4);                   \
    __l_amac__(m, a5,b5);                   \
    int _lo_; __l_amac_rs__(_lo_, m,a6,b6, bits); _lo_; })

#define L_AMAC_RS_pppppppp_LO(m, a1,b1, a2,b2, a3,b3, a4,b4, a5,b5, a6,b6, a7,b7, a8,b8, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amac__(m, a2,b2);                   \
    __l_amac__(m, a3,b3);                   \
    __l_amac__(m, a4,b4);                   \
    __l_amac__(m, a5,b5);                   \
    __l_amac__(m, a6,b6);                   \
    __l_amac__(m, a7,b7);                   \
    int _lo_; __l_amac_rs__(_lo_, m,a8,b8, bits); _lo_; })

#define L_AMAC_RS_pppppppppp_LO(m, a1,b1, a2,b2, a3,b3, a4,b4, a5,b5, a6,b6, a7,b7, a8,b8, a9,b9, a10,b10, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amac__(m, a2,b2);                   \
    __l_amac__(m, a3,b3);                   \
    __l_amac__(m, a4,b4);                   \
    __l_amac__(m, a5,b5);                   \
    __l_amac__(m, a6,b6);                   \
    __l_amac__(m, a7,b7);                   \
    __l_amac__(m, a8,b8);                   \
    __l_amac__(m, a9,b9);                   \
    int _lo_; __l_amac_rs__(_lo_, m,a10,b10, bits); _lo_; })

#define L_AMAC_RS_ppppppppmmmmmmmm_LO(m, a1,b1, a2,b2, a3,b3, a4,b4, a5,b5, a6,b6, a7,b7, a8,b8, \
                            a9,b9, a10,b10, a11,b11, a12,b12, a13,b13, a14,b14, a15,b15, a16,b16, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amac__(m, a2,b2);                   \
    __l_amac__(m, a3,b3);                   \
    __l_amac__(m, a4,b4);                   \
    __l_amac__(m, a5,b5);                   \
    __l_amac__(m, a6,b6);                   \
    __l_amac__(m, a7,b7);                   \
    __l_amac__(m, a8,b8);                   \
    __l_amsb__(m, a9,b9);                   \
    __l_amsb__(m, a10,b10);                 \
    __l_amsb__(m, a11,b11);                 \
    __l_amsb__(m, a12,b12);                 \
    __l_amsb__(m, a13,b13);                 \
    __l_amsb__(m, a14,b14);                 \
    __l_amsb__(m, a15,b15);                 \
    int _lo_; __l_amsb_rs__(_lo_, m,a16,b16, bits); _lo_; })

#define L_AMAC_RS_pppppppppppppppp_LO(m, a1,b1, a2,b2, a3,b3, a4,b4, a5,b5, a6,b6, a7,b7, a8,b8, \
                            a9,b9, a10,b10, a11,b11, a12,b12, a13,b13, a14,b14, a15,b15, a16,b16, bits) \
({  __l_amul__(m, a1,b1);                   \
    __l_amac__(m, a2,b2);                   \
    __l_amac__(m, a3,b3);                   \
    __l_amac__(m, a4,b4);                   \
    __l_amac__(m, a5,b5);                   \
    __l_amac__(m, a6,b6);                   \
    __l_amac__(m, a7,b7);                   \
    __l_amac__(m, a8,b8);                   \
    __l_amac__(m, a9,b9);                   \
    __l_amac__(m, a10,b10);                 \
    __l_amac__(m, a11,b11);                 \
    __l_amac__(m, a12,b12);                 \
    __l_amac__(m, a13,b13);                 \
    __l_amac__(m, a14,b14);                 \
    __l_amac__(m, a15,b15);                 \
    int _lo_; __l_amac_rs__(_lo_, m,a16,b16, bits); _lo_; })

#define __l_divl__(lo, a,b, bits)           \
   { int _a_ = (a), _b_ = (b); __asm__ volatile ("l.divl\t%0,%1,%2,%3" : "=r"(lo) : "r"(_a_), "r"(_b_), "K"(bits)); }
#define __l_divlu__(lo, a,b, bits)          \
   { unsigned _a_ = (a), _b_ = (b); __asm__ volatile ("l.divlu\t%0,%1,%2,%3" : "=r"(lo) : "r"(_a_), "r"(_b_), "K"(bits)); }

/* Left-shift before divide */
#define L_DIVL(a,b, bits)  ({ int _lo_; __l_divl__(_lo_, a,b, bits); _lo_; })
#define L_DIVLU(a,b, bits)  ({ unsigned _lo_; __l_divlu__(_lo_, a,b, bits); _lo_; })

/* Arithmetic-Shift-Right & Clip to 16-bit */

#define __SRA_CLIP16(shift_right, t)                                    \
({  int _u_, _t_ = t;                                                   \
    __asm__("l.srai     %0,%0,%1"     : "+r"(_t_)  : "K"(shift_right)); \
    __asm__("l.sfgesi   %0,-32768"    :          : "r"(_t_));           \
    __asm__("l.addi     %0,r0,-32768" : "=r"(_u_)  :       );           \
    __asm__("l.cmov     %0,%0,%1"     : "+r"(_t_)  : "r"(_u_));         \
    __asm__("l.sflesi   %0,32767"     :          : "r"(_t_));           \
    __asm__("l.addi     %0,r0,32767"  : "=r"(_u_)  :       );           \
    __asm__("l.cmov     %0,%0,%1"     : "+r"(_t_)  : "r"(_u_));         \
    _t_; })

#define __CLIP16(t)                                                     \
({  int _u_, _t_ = t;                                                   \
    __asm__("l.sfgesi   %0,-32768"    :          : "r"(_t_));           \
    __asm__("l.addi     %0,r0,-32768" : "=r"(_u_)  :       );           \
    __asm__("l.cmov     %0,%0,%1"     : "+r"(_t_)  : "r"(_u_));         \
    __asm__("l.sflesi   %0,32767"     :          : "r"(_t_));           \
    __asm__("l.addi     %0,r0,32767"  : "=r"(_u_)  :       );           \
    __asm__("l.cmov     %0,%0,%1"     : "+r"(_t_)  : "r"(_u_));         \
    _t_; })

#define __SRA_CLIP16_ex(shift_right, t)     \
({  int _result_, _t_ = t;                  \
    __asm__ volatile ("l.srari\t%0,%1,%2" : "=r"(_result_) : "r"(_t_), "K"(shift_right) ); \
    __asm__ volatile ("l.sat\t%0,%1,16" : "=r"(_result_) : "r"(_result_) ); \
    _result_;})

#define __CLIP16_ex(t)      \
({  int _result_, _t_ = t;  \
    __asm__ volatile ("l.sat\t%0,%1,16" : "=r"(_result_) : "r"(_t_) ); \
    _result_;})

/* Extract, Deposit, and Saturate */

#define L_EXT(v, width, bitpos)             \
({  int _result_, _v_ = v, _b_ = bitpos;    \
    __asm__ volatile ("l.ext\t%0,%1,%2,%3" : "=r"(_result_) : "r"(_v_), "K"(width), "r"(_b_)); \
    _result_;})

#define L_EXTI(v, width, bitpos)            \
({  int _result_, _v_ = v;                  \
    __asm__ volatile ("l.exti\t%0,%1,%2,%3" : "=r"(_result_) : "r"(_v_), "K"(width), "K"(bitpos)); \
    _result_;})

#define L_EXTU(v, width, bitpos)            \
({  int _result_, _v_ = v, _b_ = bitpos;    \
    __asm__ volatile ("l.extu\t%0,%1,%2,%3" : "=r"(_result_) : "r"(_v_), "K"(width), "r"(_b_)); \
    _result_;})

#define L_EXTUI(v, width, bitpos)           \
({  int _result_, _v_ = v;                  \
    __asm__ volatile ("l.extui\t%0,%1,%2,%3" : "=r"(_result_) : "r"(_v_), "K"(width), "K"(bitpos)); \
    _result_;})

#define L_EXTR(v, width, bitpos)            \
({  int _result_, _v_ = v, _width_ = width, _b_ = bitpos;    \
    __asm__ volatile ("l.extr\t%0,%1,%2,%3" : "=r"(_result_) : "r"(_v_), "r"(_width_), "r"(_b_)); \
    _result_;})

#define L_EXTRI(v, width, bitpos)           \
({  int _result_, _v_ = v, _width_ = width; \
    __asm__ volatile ("l.extri\t%0,%1,%2,%3" : "=r"(_result_) : "r"(_v_), "r"(_width_), "K"(bitpos)); \
    _result_;})

#define L_EXTRU(v, width, bitpos)           \
({  int _result_, _v_ = v, _width_ = width, _b_ = bitpos; \
    __asm__ volatile ("l.extru\t%0,%1,%2,%3" : "=r"(_result_) : "r"(_v_), "r"(_width_), "r"(_b_)); \
    _result_;})

#define L_EXTRUI(v, width, bitpos)          \
({  int _result_, _v_ = v, _width_ = width; \
    __asm__ volatile ("l.extrui\t%0,%1,%2,%3" : "=r"(_result_) : "r"(_v_), "r"(_width_), "K"(bitpos)); \
    _result_;})

#define L_DEP(d, v, width, bitpos)          \
({  int _v_ = v, _b_ = bitpos;              \
    __asm__ volatile ("l.dep\t%0,%1,%2,%3" : "+r"(d) : "r"(_v_), "K"(width), "r"(_b_)); \
    d;})

#define L_DEPI(v, width, bitpos)            \
({  int _v_ = v;                            \
    __asm__ volatile ("l.depi\t%0,%1,%2,%3" : "+r"(d) : "r"(_v_), "K"(width), "K"(bitpos)); \
    d;})

#define L_DEPR(d, v, width, bitpos)         \
({  int _v_ = v, _width_ = width, _b_ = bitpos; \
    __asm__ volatile ("l.depr\t%0,%1,%2,%3" : "+r"(d) : "r"(_v_), "r"(_width_), "r"(_b_)); \
    d;})

#define L_DEPRI(v, width, bitpos)           \
({  int _v_ = v, _width_ = width;           \
    __asm__ volatile ("l.depri\t%0,%1,%2,%3" : "+r"(d) : "r"(_v_), "r"(_width_), "K"(bitpos)); \
    d;})

#define L_DEPOV(d)                          \
({  __asm__ volatile ("l.depov\t%0" : "+r"(d) : ); \
    d;})

#define L_SAT(v, width)                     \
({  int _result_, _v_ = v;                  \
    __asm__ volatile ("l.sat\t%0,%1,%2" : "=r"(_result_) : "r"(_v_), "K"(width)); \
    _result_;})

#define L_SATU(v, width)                    \
({  int _result_, _v_ = v;                  \
    __asm__ volatile ("l.satu\t%0,%1,%2" : "=r"(_result_) : "r"(_v_), "K"(width)); \
    _result_;})

#define L_SATSU(v, width)                   \
({  int _result_, _v_ = v;                  \
    __asm__ volatile ("l.satsu\t%0,%1,%2" : "=r"(_result_) : "r"(_v_), "K"(width)); \
    _result_;})

#define L_SATUS(v, width)                   \
({  int _result_, _v_ = v;                  \
    __asm__ volatile ("l.satus\t%0,%1,%2" : "=r"(_result_) : "r"(_v_), "K"(width)); \
    _result_;})

#define L_SATDEP(d, v, width, bitpos)       \
({  int _v_ = v, _b_ = bitpos;    \
    __asm__ volatile ("l.satdep\t%0,%1,%2,%3" : "+r"(d) : "r"(_v_), "K"(width), "r"(_b_)); \
    d;})

#define L_SATDEPI(d, v, width, bitpos)      \
({  int _v_ = v;                  \
    __asm__ volatile ("l.satdepi\t%0,%1,%2,%3" : "+r"(d) : "r"(_v_), "K"(width), "K"(bitpos)); \
    d;})

#define L_SATDEPU(d, v, width, bitpos)      \
({  int _v_ = v, _b_ = bitpos;    \
    __asm__ volatile ("l.satdepu\t%0,%1,%2,%3" : "+r"(d) : "r"(_v_), "K"(width), "r"(_b_)); \
    d;})

#define L_SATDEPUI(d, v, width, bitpos)     \
({  int _v_ = v;                  \
    __asm__ volatile ("l.satdepui\t%0,%1,%2,%3" : "+r"(d) : "r"(_v_), "K"(width), "K"(bitpos)); \
    d;})

/* FFT */
#define __l_fft__(fft, n, re,im, mode)      \
   { void *_re_ = (void*)(re), *_im_ = (void*)(im); __asm__ volatile (fft "\t%0,%1,%2,%3" :  : "K"(n), "r"(_re_), "r"(_im_), "K"(mode)); }

#define L_FFT(n, re,im, mode)       __l_fft__("l.fft", n, re,im, mode)
#define L_FFTR(n, re,im, mode)      __l_fft__("l.fftr", n, re,im, mode)
#define L_IFFT(n, re,im, mode)      __l_fft__("l.ifft", n, re,im, mode)
#define L_IFFTR(n, re,im, mode)     __l_fft__("l.ifftr", n, re,im, mode)
#define L_WAIT_FOR_FFT  while(L_MFSPR1(SPR_ACCEL_FFT) != 0);

/* ABS/COUNT1/BYTESWAP/MAX/MIN */
#define L_ABS(a)                            \
({  int _res_, _a_ = a;                     \
    __asm__ volatile ("l.abs\t%0,%1" : "=r"(_res_) : "r"(_a_)); \
    _res_;})

#define L_COUNT1(a)                         \
({  int _res_, _a_ = a;                     \
    __asm__ volatile ("l.count1\t%0,%1" : "=r"(_res_) : "r"(_a_)); \
    _res_;})

#define L_BYTESWAP(a)                       \
({  int _res_, _a_ = a;                     \
    __asm__ volatile ("l.byteswap\t%0,%1" : "=r"(_res_) : "r"(_a_)); \
    _res_;})

#define L_BITSWAP(a)                        \
({  int _res_, _a_ = a;                     \
    __asm__ volatile ("l.bitswap\t%0,%1" : "=r"(_res_) : "r"(_a_)); \
    _res_;})

#define L_MAX(a,b)                          \
({  int _res_, _a_ = a, _b_ = b;            \
    __asm__ volatile ("l.max\t%0,%1,%2" : "=r"(_res_) : "r"(_a_), "r"(_b_)); \
    _res_;})

#define L_MAXU(a,b)                         \
({  int _res_, _a_ = a, _b_ = b;            \
    __asm__ volatile ("l.maxu\t%0,%1,%2" : "=r"(_res_) : "r"(_a_), "r"(_b_)); \
    _res_;})

#define L_MIN(a,b)                          \
({  int _res_, _a_ = a, _b_ = b;            \
    __asm__ volatile ("l.min\t%0,%1,%2" : "=r"(_res_) : "r"(_a_), "r"(_b_)); \
    _res_;})

#define L_MINU(a,b)                         \
({  int _res_, _a_ = a, _b_ = b;            \
    __asm__ volatile ("l.minu\t%0,%1,%2" : "=r"(_res_) : "r"(_a_), "r"(_b_)); \
    _res_;})

#define L_MAXI(a,b)                         \
({  int _res_, _a_ = a;                     \
    __asm__ volatile ("l.maxi\t%0,%1,%2" : "=r"(_res_) : "r"(_a_), "I"(b)); \
    _res_;})

#define L_MAXUI(a,b)                        \
({  int _res_, _a_ = a;                     \
    __asm__ volatile ("l.maxui\t%0,%1,%2" : "=r"(_res_) : "r"(_a_), "I"(b)); \
    _res_;})

#define L_MINI(a,b)                         \
({  int _res_, _a_ = a;                     \
    __asm__ volatile ("l.mini\t%0,%1,%2" : "=r"(_res_) : "r"(_a_), "I"(b)); \
    _res_;})

#define L_MINUI(a,b)                        \
({  int _res_, _a_ = a;                     \
    __asm__ volatile ("l.minui\t%0,%1,%2" : "=r"(_res_) : "r"(_a_), "I"(b)); \
    _res_;})

/* Bi-directional shift arithmetic/logical */
#define L_BSA(a,b)                          \
({  int _res_, _a_ = a, _b_ = b;            \
    __asm__ volatile ("l.bsa\t%0,%1,%2" : "=r"(_res_) : "r"(_a_), "r"(_b_)); \
    _res_;})

#define L_BSL(a,b)                          \
({  int _res_, _a_ = a, _b_ = b;            \
    __asm__ volatile ("l.bsl\t%0,%1,%2" : "=r"(_res_) : "r"(_a_), "r"(_b_)); \
    _res_;})

/* Double-shift */
#define L_DSL(a,b,cnt)                      \
({  int _a_ = a, _b_ = b, _cnt_ = cnt;      \
    __asm__ volatile ("l.dsl\t%0,%1,%2" : "+r"(_a_) : "r"(_b_), "r"(_cnt_)); \
    _a_;})

#define L_DSR(a,b,cnt)                      \
({  int _a_ = a, _b_ = b, _cnt_ = cnt;      \
    __asm__ volatile ("l.dsr\t%0,%1,%2" : "+r"(_a_) : "r"(_b_), "r"(_cnt_)); \
    _a_;})

#define L_DSLI(a,b,cnt)                     \
({  int _a_ = a, _b_ = b;                   \
    __asm__ volatile ("l.dsl\t%0,%1,%2" : "+r"(_a_) : "r"(_b_), "K"(cnt)); \
    _a_;})

#define L_DSRI(a,b,cnt)                     \
({  int _a_ = a, _b_ = b;                   \
    __asm__ volatile ("l.dsr\t%0,%1,%2" : "+r"(_a_) : "r"(_b_), "K"(cnt)); \
    _a_;})

#endif /* __aeon__ */

#endif /* __SPR_DEFS_H */
