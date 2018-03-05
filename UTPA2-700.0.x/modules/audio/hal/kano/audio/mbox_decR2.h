#define MBOX_RESERVERD      0xFFFFFFFF

#define RIU_RD_REG8(reg_addr)           (*((volatile unsigned char *)(reg_addr)))
#define RIU_WR_REG8(reg_addr, reg_val)  *((volatile unsigned char *)(reg_addr)) = reg_val
#define RIU_RD_REG16(reg_addr)          (*((volatile unsigned short *)(reg_addr)))
#define RIU_WR_REG16(reg_addr, reg_val) *((volatile unsigned short *)(reg_addr)) = reg_val
#define RIU_RD_REG32(reg_addr)          (*((volatile unsigned int *)(reg_addr)))
#define RIU_WR_REG32(reg_addr, reg_val) *((volatile unsigned int *)(reg_addr)) = reg_val


#define DSP_RD_REG24(reg_addr)          ((*((volatile unsigned int *)(reg_addr)))&0xFFFFFF)
#define DSP_WR_REG24(reg_addr, reg_val)  *((volatile unsigned int *)(reg_addr)) = (reg_val&0xFFFFFF)

#define REG_SET_BIT( regAddr, bitMask )  *((volatile unsigned short *)regAddr) = *((volatile unsigned short *)regAddr)|bitMask
#define REG_CLR_BIT( regAddr, bitMask )  *((volatile unsigned short *)regAddr) = *((volatile unsigned short *)regAddr)&~bitMask
#define REG_TST_BIT( regAddr, bitMask )  (*((volatile unsigned short *)regAddr)&bitMask)
#define REG_TOGGLE_BIT( regAddr, bitMask )  *((volatile unsigned short *)regAddr) = *((volatile unsigned short *)regAddr)^bitMask

#define REG24_SET_BIT( regAddr, bitMask )  *((volatile unsigned int *)regAddr) = *((volatile unsigned int *)regAddr)|bitMask
#define REG24_CLR_BIT( regAddr, bitMask )  *((volatile unsigned int *)regAddr) = *((volatile unsigned int *)regAddr)&~bitMask
#define REG24_TST_BIT( regAddr, bitMask )  (*((volatile unsigned int *)regAddr)&bitMask)
#define REG24_TOGGLE_BIT( regAddr, bitMask )  *((volatile unsigned int *)regAddr) = *((volatile unsigned int *)regAddr)^bitMask

/***************************************************************
*   Mailbox  : Dec_R2 to MCU 
*   Mailbox  : MCU to Dec_R2
****************************************************************/
#ifdef __aeon__
#define MBOX_M2D_0380       0xB0000000
#define MBOX_M2D_0382       0xB0000002
#define MBOX_M2D_0384       0xB0000004
#define MBOX_M2D_0386       0xB0000006
#define MBOX_M2D_0388       0xB0000008
#define MBOX_M2D_038A       0xB000000A
#define MBOX_M2D_038C       0xB000000C
#define MBOX_M2D_038E       0xB000000E
#define MBOX_M2D_0390       0xB0000010
#define MBOX_M2D_0392       0xB0000012
#define MBOX_M2D_0394       0xB0000014
#define MBOX_M2D_0396       0xB0000016
#define MBOX_M2D_0398       0xB0000018
#define MBOX_M2D_039A       0xB000001A
#define MBOX_M2D_039C       0xB000001C
#define MBOX_M2D_039E       0xB000001E

#define MBOX_D2M_03A0       0xB0000020
#define MBOX_D2M_03A2       0xB0000022
#define MBOX_D2M_03A4       0xB0000024
#define MBOX_D2M_03A6       0xB0000026
#define MBOX_D2M_03A8       0xB0000028
#define MBOX_D2M_03AA       0xB000002A
#define MBOX_D2M_03AC       0xB000002C
#define MBOX_D2M_03AE       0xB000002E
#define MBOX_D2M_03B0       0xB0000030
#define MBOX_D2M_03B2       0xB0000032
#define MBOX_D2M_03B4       0xB0000034
#define MBOX_D2M_03B6       0xB0000036
#define MBOX_D2M_03B8       0xB0000038
#define MBOX_D2M_03BA       0xB000003A
#define MBOX_D2M_03BC       0xB000003C
#define MBOX_D2M_03BE       0xB000003E

/***************************************************************
*   Mailbox  : snd_R2 to dec_R2
*   Mailbox  : dec_R2 to snd_R2
****************************************************************/
#define MBOX_A2D_IO_0       0xB0000C00
#define MBOX_A2D_IO_1       0xB0000C04
#define MBOX_A2D_IO_2       0xB0000C08
#define MBOX_A2D_IO_3       0xB0000C0C


/***************************************************************
*   Mailbox  : dec_R2 to ASND DSP 
*   Mailbox  : ASND DSP to dec_R2
****************************************************************/
#define MBOX_S2D_IO_0       0xB0000800
#define MBOX_S2D_IO_1       0xB0000804
#define MBOX_S2D_IO_2       0xB0000808
#define MBOX_S2D_IO_3       0xB000080C
#define MBOX_S2D_IO_4       0xB0000810
#define MBOX_S2D_IO_5       0xB0000814
#define MBOX_S2D_IO_6       0xB0000818
#define MBOX_S2D_IO_7       0xB000081C
#define MBOX_S2D_IO_8       0xB0000820
#define MBOX_S2D_IO_9       0xB0000824
#define MBOX_S2D_IO_A       0xB0000828
#define MBOX_S2D_IO_B       0xB000082C
#define MBOX_S2D_IO_C       0xB0000830
#define MBOX_S2D_IO_D       0xB0000834
#define MBOX_S2D_IO_E       0xB0000838
#define MBOX_S2D_IO_F       0xB000083C

#define MBOX_D2S_IO_0       0xB0000840
#define MBOX_D2S_IO_1       0xB0000844
#define MBOX_D2S_IO_2       0xB0000848
#define MBOX_D2S_IO_3       0xB000084C
#define MBOX_D2S_IO_4       0xB0000850
#define MBOX_D2S_IO_5       0xB0000854
#define MBOX_D2S_IO_6       0xB0000858
#define MBOX_D2S_IO_7       0xB000085C
#define MBOX_D2S_IO_8       0xB0000860
#define MBOX_D2S_IO_9       0xB0000864
#define MBOX_D2S_IO_A       0xB0000868
#define MBOX_D2S_IO_B       0xB000086C
#define MBOX_D2S_IO_C       0xB0000870
#define MBOX_D2S_IO_D       0xB0000874
#define MBOX_D2S_IO_E       0xB0000878
#define MBOX_D2S_IO_F       0xB000087C

#define MBOX_INT_MASK       0xB0000FFC

/***************************************************************
*   Mailbox  : Aeon_Register (R2_1129 or R2_1630)   //RW
     If your IC is Monaco/Clipper/Miami/Keres or later ICs, 
     R2 can aceess these regiseters.
     After set 112CB2 [1:0] = 0x3 , R2 can access its BANK register.
For Example At R2_1129:
MBOX_R2Reg_00  = R2_112900
MBOX_R2Reg_C8 = R2_1129C8
Wishbone mapping: 0xB0004000 + C8*2 = 0xB0004190
****************************************************************/
#define MBOX_R2Reg_82       0xB0004104
#define MBOX_R2Reg_84       0xB0004108

#else

/***************************************************************
*   Mailbox  : Aeon(R2) to MCU 
*   Mailbox  : MCU to Aeon(R2) 
****************************************************************/
#define MBOX_M2D_0380       ((unsigned int)&mb_m2d[0])
#define MBOX_M2D_0382       ((unsigned int)&mb_m2d[1])
#define MBOX_M2D_0384       ((unsigned int)&mb_m2d[2])
#define MBOX_M2D_0386       ((unsigned int)&mb_m2d[3])
#define MBOX_M2D_0388       ((unsigned int)&mb_m2d[4])
#define MBOX_M2D_038A       ((unsigned int)&mb_m2d[5])
#define MBOX_M2D_038C       ((unsigned int)&mb_m2d[6])
#define MBOX_M2D_038E       ((unsigned int)&mb_m2d[7])
#define MBOX_M2D_0390       ((unsigned int)&mb_m2d[8])
#define MBOX_M2D_0392       ((unsigned int)&mb_m2d[9])
#define MBOX_M2D_0394       ((unsigned int)&mb_m2d[10]) 
#define MBOX_M2D_0396       ((unsigned int)&mb_m2d[11]) 
#define MBOX_M2D_0398       ((unsigned int)&mb_m2d[12]) 
#define MBOX_M2D_039A       ((unsigned int)&mb_m2d[13]) 
#define MBOX_M2D_039C       ((unsigned int)&mb_m2d[14]) 
#define MBOX_M2D_039E       ((unsigned int)&mb_m2d[15]) 

#define MBOX_D2M_03A0       ((unsigned int)&mb_d2m[0]) 
#define MBOX_D2M_03A2       ((unsigned int)&mb_d2m[1]) 
#define MBOX_D2M_03A4       ((unsigned int)&mb_d2m[2]) 
#define MBOX_D2M_03A6       ((unsigned int)&mb_d2m[3]) 
#define MBOX_D2M_03A8       ((unsigned int)&mb_d2m[4]) 
#define MBOX_D2M_03AA       ((unsigned int)&mb_d2m[5]) 
#define MBOX_D2M_03AC       ((unsigned int)&mb_d2m[6]) 
#define MBOX_D2M_03AE       ((unsigned int)&mb_d2m[7]) 
#define MBOX_D2M_03B0       ((unsigned int)&mb_d2m[8]) 
#define MBOX_D2M_03B2       ((unsigned int)&mb_d2m[9]) 
#define MBOX_D2M_03B4       ((unsigned int)&mb_d2m[10]) 
#define MBOX_D2M_03B6       ((unsigned int)&mb_d2m[11]) 
#define MBOX_D2M_03B8       ((unsigned int)&mb_d2m[12]) 
#define MBOX_D2M_03BA       ((unsigned int)&mb_d2m[13]) 
#define MBOX_D2M_03BC       ((unsigned int)&mb_d2m[14]) 
#define MBOX_D2M_03BE       ((unsigned int)&mb_d2m[15]) 

/***************************************************************
*   Mailbox  : snd_R2 to dec_R2
*   Mailbox  : dec_R2 to snd_R2
****************************************************************/
#define MBOX_A2D_IO_0       ((unsigned int)&mb_a2d[0]) 
#define MBOX_A2D_IO_1       ((unsigned int)&mb_a2d[1]) 
#define MBOX_A2D_IO_2       ((unsigned int)&mb_a2d[2]) 
#define MBOX_A2D_IO_3       ((unsigned int)&mb_a2d[3]) 

/***************************************************************
*   Mailbox  : Aeon(R2) to ASND DSP 
*   Mailbox  : ASND DSP to Aeon(R2) 
****************************************************************/
#define MBOX_S2D_IO_0       ((unsigned int)&mb_s2d[0]) 
#define MBOX_S2D_IO_1       ((unsigned int)&mb_s2d[1]) 
#define MBOX_S2D_IO_2       ((unsigned int)&mb_s2d[2]) 
#define MBOX_S2D_IO_3       ((unsigned int)&mb_s2d[3]) 
#define MBOX_S2D_IO_4       ((unsigned int)&mb_s2d[4]) 
#define MBOX_S2D_IO_5       ((unsigned int)&mb_s2d[5]) 
#define MBOX_S2D_IO_6       ((unsigned int)&mb_s2d[6]) 
#define MBOX_S2D_IO_7       ((unsigned int)&mb_s2d[7]) 
#define MBOX_S2D_IO_8       ((unsigned int)&mb_s2d[8]) 
#define MBOX_S2D_IO_9       ((unsigned int)&mb_s2d[9]) 
#define MBOX_S2D_IO_A       ((unsigned int)&mb_s2d[10])
#define MBOX_S2D_IO_B       ((unsigned int)&mb_s2d[11])
#define MBOX_S2D_IO_C       ((unsigned int)&mb_s2d[12])
#define MBOX_S2D_IO_D       ((unsigned int)&mb_s2d[13])
#define MBOX_S2D_IO_E       ((unsigned int)&mb_s2d[14])
#define MBOX_S2D_IO_F       ((unsigned int)&mb_s2d[15])

#define MBOX_D2S_IO_0       ((unsigned int)&mb_d2s[0]) 
#define MBOX_D2S_IO_1       ((unsigned int)&mb_d2s[1]) 
#define MBOX_D2S_IO_2       ((unsigned int)&mb_d2s[2]) 
#define MBOX_D2S_IO_3       ((unsigned int)&mb_d2s[3]) 
#define MBOX_D2S_IO_4       ((unsigned int)&mb_d2s[4]) 
#define MBOX_D2S_IO_5       ((unsigned int)&mb_d2s[5]) 
#define MBOX_D2S_IO_6       ((unsigned int)&mb_d2s[6]) 
#define MBOX_D2S_IO_7       ((unsigned int)&mb_d2s[7]) 
#define MBOX_D2S_IO_8       ((unsigned int)&mb_d2s[8]) 
#define MBOX_D2S_IO_9       ((unsigned int)&mb_d2s[9]) 
#define MBOX_D2S_IO_A       ((unsigned int)&mb_d2s[10])
#define MBOX_D2S_IO_B       ((unsigned int)&mb_d2s[11])
#define MBOX_D2S_IO_C       ((unsigned int)&mb_d2s[12])
#define MBOX_D2S_IO_D       ((unsigned int)&mb_d2s[13])
#define MBOX_D2S_IO_E       ((unsigned int)&mb_d2s[14])
#define MBOX_D2S_IO_F       ((unsigned int)&mb_d2s[15])

#define MBOX_INT_MASK       ((unsigned int)&mb_int_mask)

#define MBOX_R2Reg_82       ((unsigned int)&mb_r2reg[0])
#define MBOX_R2Reg_84       ((unsigned int)&mb_r2reg[1])

#endif
