//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all 
// or part of MStar Software is expressly prohibited, unless prior written 
// permission has been granted by MStar. 
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.  
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software. 
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s 
//    confidential information in strictest confidence and not disclose to any
//    third party.  
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.  
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or 
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.  
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-20012 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// file:   regCMDQ.h
// Description: CMDQ Register Definition
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CMDQ_REG_MCU_H_
#define _CMDQ_REG_MCU_H_


//--------------------------------------------------------------------------------------------------
//  Abbreviation
//--------------------------------------------------------------------------------------------------
// Addr                             Address
// Buf                              Buffer
// Clr                              Clear
// CmdQ                             Command queue
// Cnt                              Count
// Ctrl                             Control
// Flt                              Filter
// Hw                               Hardware
// Int                              Interrupt
// Len                              Length
// Ovfw                             Overflow
// Pkt                              Packet
// Rec                              Record
// Recv                             Receive
// Rmn                              Remain
// Reg                              Register
// Req                              Request
// Rst                              Reset
// Scmb                             Scramble
// Sec                              Section
// Stat                             Status
// Sw                               Software
// Ts                               Transport Stream


//--------------------------------------------------------------------------------------------------
//  Global Definition
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Compliation Option
//--------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#define REG_CMDQCTRL_BASE         0x46A00UL //0x123500 * 2  -> 0x246A00 (0xBF246A00)


typedef struct _REG32
{
    volatile MS_U16                L;
    volatile MS_U16                empty_L;
    volatile MS_U16                H;
    volatile MS_U16                empty_H;
} REG32;


typedef struct _REG_CMDQCtrl
{
    REG32            CMDQ_Enable;                      		//0x01 & 0x00
    #define CMDQ_CMDQ_EN                  0x00010000UL   	//0x01 bit:0 1:enable
    #define CMDQ_CMDQ_DISEN               0x00000000UL   	//0x01 bit:0 1:enable
    REG32            CMDQ_Trig_Mode;                   		//0x03 & 0x02
    #define CMDQ_DMA_TRIG_EN              0x00000000UL   	//0x02 bit:0 cmdq enable
    #define CMDQ_CMD_BUF_DIRECT_MODE      0x00000000UL   	//0x02 bit:1 0:direct
    #define CMDQ_CMD_INCREAMENT_MODE      0x00000002UL   	//0x02 bit:1 1:increament
    #define CMDQ_RING_BUFFER_MODE         0x00000004UL		//0x02 bit:4 ring buffer mode
    #define CMDQ_DMA_TRIG                 0x00010000UL		//0x03 bit:0 
    #define CMDQ_MOV_CMD_PTR              0x00020000UL		//0x03 bit:1  cmdq start run
	#define CMDQ_READ_TRIG                0x01000000UL		//0x03 bit:8  read trig bit
	#define CMDQ_RST_CMD_ST_PTR_TRIG      0x00040000UL		//0x03 bit:2increament reset start
    REG32            CMDQ_Cmd_St_Ptr;                  		//0x05 & 0x04 start pointer
    REG32            CMDQ_Cmd_End_Ptr;                 		//0x07 & 0x06 End pointer
	REG32            CMDQ_Sw_Wr_Mi_Wadr;               		//0x09 & 0x08 write pointer
	REG32            CMDQ_Rd_Mi_Radr;                  		//0x0B & 0x0A read pointer
	REG32            CMDQ_Buf_Radr_Wadr;               		//0x0D & 0x0C write & read buffer
	REG32            CMDQ_0F_0E;                       		//0x0F & 0x0E  non-use
    REG32            CMDQ_Length_ReadMode;             		//0x11 & 0x10
	#define CMDQ_MIU_SELECT_MIU1          0x00000002UL		//0x10 bit:2 miu_select
    #define CMDQ_REQ_LEN_MASK             0x000d0000UL		//0x11 bit:0~4
    #define CMDQ_REQ_TH_MASK              0x09000000UL		//0x11 bit:8~11 -> not used
    #define REQ_DMA_BURST_MODE            0x10000000UL		//0x11 bit:12
    REG32            CMDQ_1f_12[7];                    		//0x1f ~ 0x12
    REG32            CMDQ_Mask_Setting;                		//0x21 & 0x20
    #define CMDQ_MASK_BIT                 0x00000080UL		//0x20 bit:7=1
    REG32            CMDQ_27_22[3];                    		//0x27 ~ 0x22
    REG32            CMDQ_Tout_Base_Amount;            		//0x29 ~ 0x28
	#define TOUT_DO_NOT_JUMP              0xFF7F0000UL		//0x29 bit7: reg_tout_jump_cmd_en
    REG32            CMDQ_Cmd_Mode_Enable;             		//0x2b & 0x2a
    REG32            CMDQ_One_Step_Trig;               		//0x2d & 0x2c
    REG32            CMDQ_Cmd_Replace_Dat2_3;          		//0x2f & 0x2e
    REG32            CMDQ_En_Clk_Miu;                  		//0x31 & 0x30
    #define CMDQ_SOFT_RSTZ                0x00000000UL		//0x31 bit:0
    REG32            CMDQ_Debug_Sel;                   		//0x33 & 0x32
    REG32            CMDQ_3f_34[6];                    		//0x3f ~ 0x34
    REG32            CMDQ_Dma_State_Rb_Cmd;            		//0x41 & 0x40
    REG32            CMDQ_Cmd_Sel_Decode_State;        		//0x43 & 0x42
    #define CMDQ_CRASH_15_0_BIT           0x00000000UL		//0x43 bit:3 command[15:0]
    #define CMDQ_CRASH_31_16_BIT          0x00010000UL		//0x43 bit:3 command[31:16]
    #define CMDQ_CRASH_55_32_BIT          0x00020000UL		//0x43 bit:3 command[55:32]
    #define CMDQ_CRASH_63_56_BIT          0x00030000UL		//0x43 bit:3 command[63:56]
    REG32            CMDQ_Raw_Irq_Final_Irq;           		//0x45 & 0x44
    #define CMDQ_READ_DEC_DONE            0x00000004UL		//0x44 bit:3 read cmdq done
    #define CMDQ_READ_SOFT_INTER          0x000000f0UL		//0x44 bit:4~7 read interrupt
    REG32            CMDQ_Irq_Mask_Irq_Force;          		//0x47 & 0x46
    REG32            CMDQ_Sw_Trig_Cap_Sel_Irq_Clr;     		//0x49 & 0x48
    #define CMDQ_DEC_DONE_CLR             0x00000004UL		//0x48 bit:3 clear cmdq done
    #define CMDQ_SOFT_INTER_CLR           0x000000f0UL		//0x48 bit:4~7 clear interrupt
    REG32            CMDQ_Poll_Ratio_Wait_Time;        		//0x4b & 0x4a
    #define CMDQ_POLL_WAIT_TIME_MASK      0x0000ffffUL		//0x4a bit:0~15
    #define CMDQ_POLL_TIMER_RATIO_MASK    0x00ff0000UL		//0x4b bit:0~7
    REG32            CMDQ_Cmd_Ptr_Vld;                 		//0x4d & 0x4c
    REG32            CMDQ_75_4e[20];                   		//0x75 ~ 0x4e
	REG32            CMDQ_Hw_dummy;                    		//0x77 & 0x76
    REG32            CMDQ_dummy;                       		//0x79 & 0x78
    #define CMDQ__DUMMY_WRITE_ZERO        0x00000000UL		//0x78 bit:0~7
    
    
}REG_CMDQCtrl;

#endif // #ifndef _CMDQ_REG_MCU_H_
