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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
/// @file INTERN_DVBT.c
/// @brief INTERN_DVBT DVBT
/// @author MStar Semiconductor, Inc.
//
////////////////////////////////////////////////////////////////////////////////

#define _INTERN_DVBT_C_
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"

#include "MsTypes.h"
#include "drvBDMA.h"
#include "drvIIC.h"
#include "msAPI_Tuner.h"
#include "msAPI_MIU.h"
#include "BinInfo.h"
#include "halVif.h"

#include "INTERN_DVBT.h"
#ifdef SUPPORT_AP_BIN_IN_FLASH_2
#include "InfoBlock.h"
#endif

#include "apiPWS.h"

#include "drvMMIO.h"
#include "drvAVD.h"

#define TEST_EMBEDED_DEMOD 0
//U8 load_data_variable=1;
//-----------------------------------------------------------------------
#define BIN_ID_INTERN_DVBT_DEMOD BIN_ID_INTERN_DVBT

#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 ) //T3

//  #define INTERN_DVBT_LOAD_FW_FROM_CODE_MEMORY

	#define MBRegBase   0x110500//0x1033C0
	#define VDMcuBase   0x103400
	#define DMDMcuBase  0x103460

#elif ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD ) //T4

	#define INTERN_DVBT_LOAD_FW_FROM_CODE_MEMORY

	#define MBRegBase   0x112600 //Demod MailBox //mick
	#define VDMcuBase   0x103400 //DmdMCU51 (40-4F)
	#define DMDMcuBase  0x103480

#endif

#define DBG_INTERN_DVBT(x) //x
#define DBG_GET_SIGNAL(x)
#define DBG_DUMP_LOAD_DSP_TIME 0

//-----------------------------------------------------------------------
/****************************************************************
*Local Variables                                                *
****************************************************************/
//static U8 *RIUBase=NULL;
//static U32 RIUSize=0;
static BOOL FECLock=0;

//Global Variables
S_CMDPKTREG gsCmdPacket;
//U8 gCalIdacCh0, gCalIdacCh1;

#ifdef INTERN_DVBT_LOAD_FW_FROM_CODE_MEMORY
code U8 INTERN_DVBT_table[] = {

	#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 ) //T3
    	#include "INTERN_DVBT_demod.dat"
	#elif ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD ) //T4
    	#include "INTERN_DVBT_demod_T4.dat"
    #endif
};

#endif

#if 0
static void INTERN_DVBT_InitRegBase(void)
{
    MDrv_MMIO_GetBASE((MS_U32*)(void *)RIUBase, &RIUSize, MS_MODULE_MIU);
    printf("address:%lx %lx\n",(MS_U32)RIUBase, RIUSize);
}
#endif
/***********************************************************************************
  Subject:    Delay
  Function:   INTERN_DVBT_Delayms
  Parmeter:   delay_ms : millisecs for delay
  Return:     NONE
  Remark:
************************************************************************************/
static void    INTERN_DVBT_Delayms(U32 delay_ms)
{
    MsOS_DelayTask(delay_ms);
}
//-----------------------------------------------------------------------
BOOLEAN INTERN_DVBT_ReadReg(U16 u16Addr, U8 *pu8Data)
{
    U16     u8WaitCnt=0;

//   UNUSED(pu8Data);
    if (MDrv_ReadByte(MBRegBase + 0x00))
    {
        printf(">> MB Busy!\n");
        return FALSE;
    }

    MDrv_WriteByte(MBRegBase + 0x02, (U8)(u16Addr >> 8));   // ADDR_H
    MDrv_WriteByte(MBRegBase + 0x01, (U8)u16Addr);          // ADDR_L
    MDrv_WriteByte(MBRegBase + 0x00, 0x01);                 // MB_CNTL set read mode

    MDrv_WriteByte(DMDMcuBase + 0x03, 0x02);                // assert interrupt to DMD MCU51 //mick
    MDrv_WriteByte(DMDMcuBase + 0x03, 0x00);                // de-assert interrupt to DMD MCU51

    while(MDrv_ReadByte(MBRegBase + 0x00)!= 0xFF)           // wait MB_CNTL set done
    {
#if TEST_EMBEDED_DEMOD
        INTERN_DVBT_Delayms(1);  // << Ken 20090629
#endif
        if (u8WaitCnt++ >= 0xFF)
        {
            printf(">> DVBT ReadReg Fail!\n");
            return FALSE;
        }
    }

    *pu8Data = MDrv_ReadByte(MBRegBase + 0x03);             // REG_DATA get
    MDrv_WriteByte(MBRegBase + 0x00, 0x00);                 // MB_CNTL clear

    return TRUE;
}

//-----------------------------------------------------------------------
BOOLEAN INTERN_DVBT_WriteReg(U32 u16Addr, U8 u8Data)
{
    U16     u8WaitCnt=0;

    MDrv_WriteByte(MBRegBase + 0x02, (U8)(u16Addr >> 8));   // ADDR_H
    MDrv_WriteByte(MBRegBase + 0x01, (U8)u16Addr);          // ADDR_L
    MDrv_WriteByte(MBRegBase + 0x03, u8Data);               // REG_DATA
    MDrv_WriteByte(MBRegBase + 0x00, 0x02);                 // MB_CNTL set write mode

    MDrv_WriteByte(DMDMcuBase + 0x03, 0x02);                // assert interrupt to VD MCU51 //mick
    MDrv_WriteByte(DMDMcuBase + 0x03, 0x00);                // de-assert interrupt to VD MCU51

    while(MDrv_ReadByte(MBRegBase + 0x00) != 0xFF)          // wait done flag
    {
#if TEST_EMBEDED_DEMOD
        INTERN_DVBT_Delayms(1);   // <<< Ken 20090629
#endif
        if (u8WaitCnt++ >= 0xFF)
        {
            printf(">> DVBT WriteReg Fail!\n");
            return FALSE;
        }
    }

    MDrv_WriteByte(MBRegBase + 0x00, 0x00);                 // MB_CNTL clear

    return TRUE;
}

/***********************************************************************************
  Subject:    Command Packet Interface
  Function:   INTERN_DVBT_Cmd_Packet_Send
  Parmeter:
  Return:     BOOLEAN
  Remark:
************************************************************************************/
BOOLEAN INTERN_DVBT_Cmd_Packet_Send(S_CMDPKTREG* pCmdPacket, U8 param_cnt)
{
    U8             status = true, indx;
    U8             reg_val, timeout = 0;

    // ==== Command Phase ===================
    DBG_INTERN_DVBT(printf("--->INTERN_DVBT (cmd=0x%x)(0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,) \n",pCmdPacket->cmd_code,
            pCmdPacket->param[0],pCmdPacket->param[1],
            pCmdPacket->param[2],pCmdPacket->param[3],
            pCmdPacket->param[4],pCmdPacket->param[5] ));

    // wait _BIT_END clear
    do
    {
#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 ) //T3
        status &= INTERN_DVBT_ReadReg(REG_CMD_CTRL, &reg_val);
#elif ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD ) //T4
        reg_val = MDrv_ReadByte(MBRegBase + 0x1C);
#endif
        if((reg_val & _BIT_END) != _BIT_END)
        {
            break;
        }
        MsOS_DelayTask(10);
        if (timeout++ > 200)
        {
            printf("---> INTERN_DVBT_Cmd_Packet_Send fail on 'wait _BIT_END clear' \n");
            return false;
        }
    } while (1);

    // set cmd_3:0 and _BIT_START
#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 ) //T3
    status &= INTERN_DVBT_ReadReg(REG_CMD_CTRL, &reg_val);
    reg_val = (reg_val & 0x0f)|(pCmdPacket->cmd_code << 4)|_BIT_START;
    status &= INTERN_DVBT_WriteReg(REG_CMD_CTRL, reg_val);
#elif ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD ) //T4
    reg_val = MDrv_ReadByte(MBRegBase + 0x1C);
    reg_val = (reg_val & 0x0f)|(pCmdPacket->cmd_code << 4)|_BIT_START;
    MDrv_WriteByte(MBRegBase + 0x1C, reg_val);
#endif

    //DBG_INTERN_DVBT(printf("demod_config: cmd_code = %bx\n", pCmdPacket->cmd_code));
    // wait _BIT_START clear
    do
    {
#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 ) //T3
        status &= INTERN_DVBT_ReadReg(REG_CMD_CTRL, &reg_val);
#elif ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD ) //T4
        reg_val = MDrv_ReadByte(MBRegBase + 0x1C);
#endif
        if((reg_val & _BIT_START) != _BIT_START)
        {
            break;
        }
        MsOS_DelayTask(1);
        if (timeout++ > 200)
        {
            printf("---> INTERN_DVBT_Cmd_Packet_Send fail on 'wait _BIT_START clear' \n");
            return false;
        }
    } while (1);

    // ==== Data Phase ======================

#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 ) //T3
    status &= INTERN_DVBT_WriteReg(REG_CMD_ADDR, 0x00);
#elif ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD ) //T4
    MDrv_WriteByte(MBRegBase + 0x1E, 0x00);
#endif

    for (indx = 0; indx < param_cnt; indx++)
    {
#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 ) //T3
        status &= INTERN_DVBT_ReadReg(REG_CMD_ADDR, &reg_val);
#elif ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD ) //T4
        reg_val = MDrv_ReadByte(MBRegBase + 0x1E);
#endif
        //DBG_INTERN_DVBT(printf("demod_config: param[%bd] = %bx\n", reg_val, pCmdPacket->param[indx]));

        // set param[indx] and _BIT_DRQ
#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 ) //T3
        status &= INTERN_DVBT_WriteReg(REG_CMD_DATA, pCmdPacket->param[indx]);
        status &= INTERN_DVBT_ReadReg(REG_DTA_CTRL, &reg_val);
        status &= INTERN_DVBT_WriteReg(REG_DTA_CTRL, reg_val|_BIT_DRQ);

#elif ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD ) //T4
        MDrv_WriteByte(MBRegBase + 0x1F, pCmdPacket->param[indx]);
        reg_val = MDrv_ReadByte(MBRegBase + 0x1D);
        MDrv_WriteByte(MBRegBase + 0x1D, reg_val|_BIT_DRQ);
#endif
        // wait _BIT_DRQ clear
        do
        {
#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 ) //T3
            status &= INTERN_DVBT_ReadReg(REG_DTA_CTRL, &reg_val);
#elif ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD ) //T4
            reg_val = MDrv_ReadByte(MBRegBase + 0x1D);
#endif
            if ((reg_val & _BIT_DRQ) != _BIT_DRQ)
            {
                break;
            }
            MsOS_DelayTask(1);
            if (timeout++ > 200)
            {
                printf("---> INTERN_DVBT_Cmd_Packet_Send fail on 'wait _BIT_DRQ clear' \n");
                return false;
            }
        } while (1);
    }

    // ==== End Phase =======================

    // set _BIT_END to finish command
#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 ) //T3
     status &= INTERN_DVBT_ReadReg(REG_CMD_CTRL, &reg_val);
     status &= INTERN_DVBT_WriteReg(REG_CMD_CTRL, reg_val|_BIT_END);

#elif ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD ) //T4
     reg_val = MDrv_ReadByte(MBRegBase + 0x1C);
     MDrv_WriteByte(MBRegBase + 0x1C, reg_val|_BIT_END);
#endif
    return status;
}


/***********************************************************************************
  Subject:    Command Packet Interface
  Function:   INTERN_DVBT_Cmd_Packet_Exe_Check
  Parmeter:
  Return:     BOOLEAN
  Remark:
************************************************************************************/
BOOLEAN INTERN_DVBT_Cmd_Packet_Exe_Check(BOOL* cmd_done)
{
    U8             status = true;
    U8             reg_val;
    U16            timeout = 0;

    // wait _BIT_END clear
    do {
        status &= INTERN_DVBT_ReadReg(REG_CMD_CTRL, &reg_val);

        if((reg_val & _BIT_END) != _BIT_END)
            break;

        if (timeout++ > 200)
        {
            *cmd_done = false;
            return status;
        }
        INTERN_DVBT_Delayms(10);
    } while (1);

    DBG_INTERN_DVBT(printf("Exe duration: %d\n", (int)timeout));

    *cmd_done = true;
    return status;
}

/***********************************************************************************
  Subject:    SoftStop
  Function:   INTERN_DVBT_SoftStop
  Parmeter:
  Return:     BOOLEAN
  Remark:
************************************************************************************/
#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 ) //T3
#else
BOOLEAN INTERN_DVBT_SoftStop ( void )
{
#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 ) //T3
    U16     u8WaitCnt=0;

    if (MDrv_ReadByte(MBRegBase + 0x00))
    {
        printf(">> MB Busy!\n");
        return FALSE;
    }

    MDrv_WriteByte(MBRegBase + 0x00, 0xA5);                 // MB_CNTL set read mode

    MDrv_WriteByte(DMDMcuBase + 0x03, 0x02);                // assert interrupt to VD MCU51
    MDrv_WriteByte(DMDMcuBase + 0x03, 0x00);                // de-assert interrupt to VD MCU51

    while(MDrv_ReadByte(MBRegBase + 0x00)!= 0x5A)           // wait MB_CNTL set done
    {
#if TEST_EMBEDED_DEMOD
        INTERN_DVBT_Delayms(1);  // << Ken 20090629
#endif
        if (u8WaitCnt++ >= 0xFF)
        {
            printf(">> DVBT SoftStop Fail!\n");
            return FALSE;
        }
    }

    //MDrv_WriteByte(0x103460, 0x01);                         // reset VD_MCU
    MDrv_WriteByte(MBRegBase + 0x00, 0x00);                 // MB_CNTL clear

#endif
    return TRUE;
}
#endif

/***********************************************************************************
  Subject:    Reset
  Function:   INTERN_DVBT_Reset
  Parmeter:
  Return:     BOOLEAN
  Remark:
************************************************************************************/
BOOLEAN INTERN_DVBT_Reset ( void )
{
    DBG_INTERN_DVBT(printf(" @INTERN_DVBT_reset\n"));

#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 ) //T3
#else
    INTERN_DVBT_SoftStop();
#endif

    MDrv_WriteByte(DMDMcuBase + 0x00, 0x01);     // reset VD_MCU
//    INTERN_DVBT_WriteReg(0x2002, 0x72);        // reset DVB-T
    INTERN_DVBT_Delayms(5);
//    INTERN_DVBT_WriteReg(0x2002, 0x52);
    MDrv_WriteByte(DMDMcuBase + 0x00, 0x00);
    INTERN_DVBT_Delayms(5);
    FECLock = FALSE;
    return TRUE;
}

/***********************************************************************************
  Subject:    Exit
  Function:   INTERN_DVBT_Exit
  Parmeter:
  Return:     BOOLEAN
  Remark:
************************************************************************************/
BOOLEAN INTERN_DVBT_Exit ( void )
{
#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 ) //T3
//    Mapi_PWS_Stop_VDMCU();
#else
    INTERN_DVBT_SoftStop();


    //diable clk gen
    //MDrv_WriteByte(0x103314, 0x01);   // reg_ckg_dvbtc_adc@0x0a[3:0] : ADC_CLK
    //MDrv_WriteByte(0x103315, 0x01);   // reg_ckg_dvbtc_innc@0x0a[11:8]

    MDrv_WriteByte(0x10330a, 0x01);   // reg_ckg_atsc_adcd_sync@0x05[3:0] : ADCCLK
    MDrv_WriteByte(0x10330b, 0x00);

    MDrv_WriteByte(0x10330c, 0x01);   // reg_ckg_dvbtc_inner1x@0x06[3:0] : MPLLDIV10/4=21.5MHz
    MDrv_WriteByte(0x10330d, 0x01);   // reg_ckg_dvbtc_inner2x@0x06[11:8]: MPLLDIV10/2=43.2MHz

    MDrv_WriteByte(0x10330e, 0x01);   // reg_ckg_dvbtc_inner4x@0x07[3:0] : MPLLDIV10=86.4MHz
    MDrv_WriteByte(0x10330f, 0x00);

    MDrv_WriteByte(0x103310, 0x01);   // reg_ckg_dvbtc_outer1x@0x08[3:0] : MPLLDIV10/2=43.2MHz
    MDrv_WriteByte(0x103311, 0x01);   // reg_ckg_dvbtc_outer2x@0x08[11:8]: MPLLDIV10=86.4MHz

    MDrv_WriteByte(0x103312, 0x01);   // dvbt_t:0x0000, dvb_c: 0x0004
    MDrv_WriteByte(0x103313, 0x00);

    MDrv_WriteByte(0x103314, 0x01);   // reg_ckg_dvbtc_adc@0x0a[3:0] : ADC_CLK
    MDrv_WriteByte(0x103315, 0x01);   // reg_ckg_dvbtc_innc@0x0a[11:8]

    MDrv_WriteByte(0x103316, 0x01);   // reg_ckg_dvbtc_eq8x@0x0b[3:0] : MPLLDIV3/2=144MHz
    MDrv_WriteByte(0x103317, 0x01);   // reg_ckg_dvbtc_eq@0x0b[11:8] : MPLLDIV3/16=18MHz

    MDrv_WriteByte(0x103318, 0x11);   // reg_ckg_dvbtc_sram0~3@0x0c[13:0]
    MDrv_WriteByte(0x103319, 0x11);

    MDrv_WriteByte(0x103308, 0x01);   // parallel mode:0x0001 / serial mode: 0x0401
    MDrv_WriteByte(0x103309, 0x05);   // reg_ckg_dvbtc_ts@0x04

//#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 ) //T3
//    MDrv_WriteByte(0x101E3E, 0x00);   // DVBT = BIT1 -> 0x02
//#endif

    if(MDrv_Read2Byte(0x001ECC) == 0x000F)   // T3
    {
        MDrv_WriteByte(0x101E3E, 0x00);   // DVBT = BIT1 clear
    }

#endif

    return TRUE;
}

/***********************************************************************************
  Subject:    Load DSP code to chip
  Function:   INTERN_DVBT_LoadDSPCode
  Parmeter:
  Return:     BOOLEAN
  Remark:
************************************************************************************/
static BOOLEAN INTERN_DVBT_LoadDSPCode(void)
{
    U8  udata = 0x00;
    U16 i;
    U16 fail_cnt=0;

#if (DBG_DUMP_LOAD_DSP_TIME==1)
    U32 u32Time;
#endif

#ifndef INTERN_DVBT_LOAD_FW_FROM_CODE_MEMORY
    BININFO BinInfo;
    BOOLEAN bResult;
    U32 u32GEAddr;
    U8 Data;
    S8 op;
    U32 srcaddr;
    U32 len;
    U32 SizeBy4K;
    U16 u16Counter=0;
    U8 *pU8Data;
#endif

#if 0
    if(MDrv_ReadByte(0x101E3E))
    {
        printf("Warring! Reg[0x101E3E]=%d\n", MDrv_ReadByte(0x101E3E));
        return FALSE;
    }
#endif

    MDrv_Sys_DisableWatchDog();

    MDrv_WriteByte(DMDMcuBase + 0x00,  0x01);        // reset VD_MCU
    MDrv_WriteByte(DMDMcuBase + 0x01,  0x00);        // disable SRAM
    MDrv_WriteByte(DMDMcuBase + 0x03,  0x50);        // enable "vdmcu51_if"
    MDrv_WriteByte(DMDMcuBase + 0x03,  0x51);        // enable auto-increase
    MDrv_WriteByte(DMDMcuBase + 0x04,  0x00);        // sram address low byte
    MDrv_WriteByte(DMDMcuBase + 0x05,  0x00);        // sram address high byte

    ////  Load code thru VDMCU_IF ////
    DBG_INTERN_DVBT(printf(">Load Code...\n"));
#ifdef INTERN_DVBT_LOAD_FW_FROM_CODE_MEMORY
    for ( i = 0; i < sizeof(INTERN_DVBT_table); i++)
    {
        MDrv_WriteByte(DMDMcuBase + 0x0C, INTERN_DVBT_table[i]); // write data to VD MCU 51 code sram
    }
#else
    BinInfo.B_ID = BIN_ID_INTERN_DVBT_DEMOD;
    msAPI_MIU_Get_BinInfo(&BinInfo, &bResult);
    if ( bResult != PASS )
    {
        return FALSE;
    }
    //printf("\t DEMOD_MEM_ADR  =%08LX\n", ((DEMOD_MEM_ADR & MIU1) ? (DEMOD_MEM_ADR | MIU_INTERVAL) : (DEMOD_MEM_ADR)));

#ifdef SUPPORT_AP_BIN_IN_FLASH_2
    InfoBlock_Flash_2_Checking_Start(&BinInfo);
#endif

#if OBA2
    MApi_BDMA_CopyFromResource(BinInfo.B_FAddr, _PA2VA((DEMOD_MEM_MEMORY_TYPE & MIU1) ? (DEMOD_MEM_ADR | MIU_INTERVAL) : (DEMOD_MEM_ADR)), MemAlign(BinInfo.B_Len, 8));
#else
    msAPI_MIU_Copy(BinInfo.B_FAddr, ((DEMOD_MEM_MEMORY_TYPE & MIU1) ? (DEMOD_MEM_ADR | MIU_INTERVAL) : (DEMOD_MEM_ADR)), MemAlign(BinInfo.B_Len, 8), MIU_FLASH2SDRAM);
#endif

#ifdef SUPPORT_AP_BIN_IN_FLASH_2
    InfoBlock_Flash_2_Checking_End(&BinInfo);
#endif

    //W1BaseAddr = MDrv_Sys_GetXdataWindow1Base();
    SizeBy4K=BinInfo.B_Len/0x1000;
    //printf("\t RRR: SizeBy4K=%d (L=%d)\n", SizeBy4K,BinInfo.B_Len);

#if (DBG_DUMP_LOAD_DSP_TIME==1)
    u32Time = msAPI_Timer_GetTime0();
#endif

    u32GEAddr = _PA2VA(((DEMOD_MEM_MEMORY_TYPE & MIU1) ? (DEMOD_MEM_ADR | MIU_INTERVAL) : (DEMOD_MEM_ADR)));

    for (i=0;i<=SizeBy4K;i++)
    {
        if(i==SizeBy4K)
            len=BinInfo.B_Len%0x1000;
        else
            len=0x1000;

        srcaddr = u32GEAddr+(0x1000*i);
        //printf("\t i = %08X\n", i);
        //printf("\t len = %08X\n", len);
        op = 1;
        u16Counter = 0 ;
        //printf("\t (B=0x%x)(Src=0x%x)Data =",i,srcaddr);
        while(len--)
        {
            u16Counter ++ ;
            //printf("file: %s, line: %d\n", __FILE__, __LINE__);
            //pU8Data = (U8 *)(srcaddr|0x80000000);
            #if OBA2
            pU8Data = (U8 *)(srcaddr);
            #else
            pU8Data = (U8 *)(srcaddr|0x80000000);
            #endif
            Data  = *pU8Data;

            #if 0
            if(u16Counter < 0x100)
                printf("0x%bx,", Data);
            #endif
            MDrv_WriteByte(DMDMcuBase + 0x0C, Data); // write data to VD MCU 51 code sram

            srcaddr += op;
        }
     //   printf("\n\n\n");
    }

#if (DBG_DUMP_LOAD_DSP_TIME==1)
    printf("------> INTERN_DVBT Load DSP Time:  (%lu)\n", msAPI_Timer_DiffTimeFromNow(u32Time)) ;
#endif

#endif

    ////  Content verification ////
    DBG_INTERN_DVBT(printf(">Verify Code...\n"));

    MDrv_WriteByte(DMDMcuBase + 0x04, 0x00);         // sram address low byte
    MDrv_WriteByte(DMDMcuBase + 0x05, 0x00);         // sram address high byte

#ifdef INTERN_DVBT_LOAD_FW_FROM_CODE_MEMORY
    for ( i = 0; i < sizeof(INTERN_DVBT_table); i++)
    {
        udata = MDrv_ReadByte(DMDMcuBase + 0x10);    // read sram data
        if (udata != INTERN_DVBT_table[i])
        {
            printf(">fail add = 0x%x\n", i);
            printf(">code = 0x%x\n", INTERN_DVBT_table[i]);
            printf(">data = 0x%x\n", udata);

            if (fail_cnt++ > 10)
            {
                printf(">DVB-T DSP Loadcode fail!");
                return false;
            }
        }
    }
#else
    for (i=0;i<=SizeBy4K;i++)
    {
        if(i==SizeBy4K)
            len=BinInfo.B_Len%0x1000;
        else
            len=0x1000;

        srcaddr = u32GEAddr+(0x1000*i);
        //printf("\t i = %08LX\n", i);
        //printf("\t len = %08LX\n", len);
        op = 1;
        u16Counter = 0 ;
        //printf("\t (B=0x%bx)(Src=0x%x)Data =",i,srcaddr);
        while(len--)
        {
            u16Counter ++ ;
            //printf("file: %s, line: %d\n", __FILE__, __LINE__);
            //pU8Data = (U8 *)(srcaddr|0x80000000);
            #if OBA2
            pU8Data = (U8 *)(srcaddr);
            #else
            pU8Data = (U8 *)(srcaddr|0x80000000);
            #endif
            Data  = *pU8Data;

            #if 0
            if(u16Counter < 0x100)
                printf("0x%bx,", Data);
            #endif
            udata = MDrv_ReadByte(DMDMcuBase + 0x10);    // read sram data
            if (udata != Data)
            {
                printf(">fail add = 0x%lx\n", (U32)((i*0x1000)+(0x1000-len)));
                printf(">code = 0x%x\n", Data);
                printf(">data = 0x%x\n", udata);

                if (fail_cnt++ > 10)
                {
                    printf(">DVB-T DSP Loadcode fail!");
                    return false;
                }
            }

            srcaddr += op;
        }
     //   printf("\n\n\n");
    }
#endif

    MDrv_WriteByte(DMDMcuBase + 0x03, 0x50);     // diable auto-increase
    MDrv_WriteByte(DMDMcuBase + 0x03, 0x00);     // disable "vdmcu51_if"
    MDrv_WriteByte(DMDMcuBase + 0x01, 0x01);     // enable SRAM
    MDrv_WriteByte(DMDMcuBase + 0x00, 0x00);     // release VD_MCU

    DBG_INTERN_DVBT(printf(">DSP Loadcode done."));
    //while(load_data_variable);

    if(MDrv_Read2Byte(0x001ECC) == 0x000F)    // chip_id == T3
    {
        MDrv_WriteByte(0x101E3E, 0x02);   // DVBT = BIT1 -> 0x02
    }

    return TRUE;
}

/***********************************************************************************
  Subject:    DVB-T CLKGEN initialized function
  Function:   INTERN_DVBT_Power_On_Initialization
  Parmeter:
  Return:     BOOLEAN
  Remark:
************************************************************************************/
void INTERN_DVBT_InitClkgen(void)
{
#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 ) //T3
    //<<CLK_GEN1_DVB_t >>
    if ( MDrv_ReadByte( 0x001ecf) == 0x00 )
       MDrv_WriteByte(0x100b42, 0x10);   	// reg_ckg_vdmcu@0x21[4:0]        (bank_CLKGEN0) //108MHz
    else  //after t3_u02
       MDrv_WriteByte(0x100b42, 0x0D); 		// reg_ckg_vdmcu@0x21[4:0]        (bank_CLKGEN0) //108MHz

    MDrv_WriteByte(0x100b43, 0x01);   		// reg_ckg_vd200@0x21[11:8]

    MDrv_WriteByte(0x100b44, 0x00);   		// enable mail-box clock          (bank_CLKGEN0)
    MDrv_WriteByte(0x100b45, 0x00);   		// enable mail-box clock

    MDrv_WriteByte(0x103314, 0x00);   		// reg_ckg_dvbtc_adc@0x0a[3:0] : ADC_CLK
    MDrv_WriteByte(0x103315, 0x01);   		// reg_ckg_dvbtc_innc@0x0a[11:8]

    MDrv_WriteByte(0x10330a, 0x00);   		// reg_ckg_atsc_adcd_sync@0x05[3:0] : ADCCLK
    MDrv_WriteByte(0x10330b, 0x00);

    MDrv_WriteByte(0x10330c, 0x00);   		// reg_ckg_dvbtc_inner1x@0x06[3:0] : MPLLDIV10/4=21.5MHz
    MDrv_WriteByte(0x10330d, 0x00);   		// reg_ckg_dvbtc_inner2x@0x06[11:8]: MPLLDIV10/2=43.2MHz

    MDrv_WriteByte(0x10330e, 0x00);   		// reg_ckg_dvbtc_inner4x@0x07[3:0] : MPLLDIV10=86.4MHz
    MDrv_WriteByte(0x10330f, 0x00);

    MDrv_WriteByte(0x103310, 0x00);   		// reg_ckg_dvbtc_outer1x@0x08[3:0] : MPLLDIV10/2=43.2MHz
    MDrv_WriteByte(0x103311, 0x00);   		// reg_ckg_dvbtc_outer2x@0x08[11:8]: MPLLDIV10=86.4MHz

    MDrv_WriteByte(0x103312, 0x00);   		// dvbt_t:0x0000, dvb_c: 0x0004
    MDrv_WriteByte(0x103313, 0x00);

    MDrv_WriteByte(0x103314, 0x00);   		// reg_ckg_dvbtc_adc@0x0a[3:0] : ADC_CLK
    MDrv_WriteByte(0x103315, 0x00);   		// reg_ckg_dvbtc_innc@0x0a[11:8]

    MDrv_WriteByte(0x103316, 0x00);   		// reg_ckg_dvbtc_eq8x@0x0b[3:0] : MPLLDIV3/2=144MHz
    MDrv_WriteByte(0x103317, 0x00);   		// reg_ckg_dvbtc_eq@0x0b[11:8] : MPLLDIV3/16=18MHz

    MDrv_WriteByte(0x103318, 0x00);   		// reg_ckg_dvbtc_sram0~3@0x0c[13:0]
    MDrv_WriteByte(0x103319, 0x00);

#if 0 //parallel
    MDrv_WriteByte(0x103308, 0x01);   		// parallel mode:0x0001 / serial mode: 0x0401
    MDrv_WriteByte(0x103309, 0x00);   		// reg_ckg_dvbtc_ts@0x04

    MDrv_WriteByte(0x103300, 0x11);   		// parallel mode: 0x0511 /serial mode 0x0400

	#if(INTERN_DVBT_TS_PARALLEL_INVERSION == 0)
    	MDrv_WriteByte(0x103301, 0x05); 	// reg_ckg_dvbtmk_ts_out_mode@0x00
	#else
    	MDrv_WriteByte(0x103301, 0x07); 	// reg_ckg_dvbtmk_ts_out_mode@0x00
	#endif

#else // serial
    MDrv_WriteByte(0x103308, 0x01);   		// parallel mode:0x0001 / serial mode: 0x0401
    MDrv_WriteByte(0x103309, 0x04);   		// reg_ckg_dvbtc_ts@0x04

    MDrv_WriteByte(0x103300, 0x00);   		// parallel mode: 0x0511 /serial mode 0x0400
	#if(INTERN_DVBT_TS_SERIAL_INVERSION == 0)
    	MDrv_WriteByte(0x103301, 0x04);  	// reg_ckg_dvbtmk_ts_out_mode@0x00
	#else
    	MDrv_WriteByte(0x103301, 0x06);  	// reg_ckg_dvbtmk_ts_out_mode@0x00
	#endif
#endif
    //MDrv_WriteByte(0x101e22, 0x02);   	// internal TS connect to TS0:0x0002; to TS1:0x0200
    //MDrv_WriteByte(0x101e23, 0x00);   	// reg_ts0~1_mux@0x11         (bank_CHIP)

    //MDrv_WriteByte(0x100b50, 0x08);   	// internal TS0:0x0108;   internal TS1:0x0801
    //MDrv_WriteByte(0x100b51, 0x08);   	// reg_ckg_ts0~1@0x28         (bank_CLKGEN0) << Ken 20090629

    MDrv_WriteByte(0x101e04, 0x00);   		// reg_if_agc_pad_oen@0x02    (bank_CHIP)
    MDrv_WriteByte(0x101e05, 0x00);

//----------------------------------------------------------------------------------------
#elif ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD ) //T4
    //<<CLK_GEN1_DVB_t >>

    MDrv_WriteByte(0x10331e, 0x10);   		// reg_ckg_dmdmcu@0x21[4:0]        (bank_CLKGEN0) //108MHz

    MDrv_WriteByte(0x103480, 0x00);			// release dmdmcu_rst

    MDrv_WriteByte(0x103314, 0x00);   		// reg_ckg_dvbtc_adc@0x0a[3:0] : ADC_CLK
    MDrv_WriteByte(0x103315, 0x01);   		// reg_ckg_dvbtc_innc@0x0a[11:8]

    MDrv_WriteByte(0x10330a, 0x00);   		// reg_ckg_atsc_adcd_sync@0x05[3:0] : ADCCLK
    MDrv_WriteByte(0x10330b, 0x00);

    MDrv_WriteByte(0x10330c, 0x00);   		// reg_ckg_dvbtc_inner1x@0x06[3:0] : MPLLDIV10/4=21.5MHz
    MDrv_WriteByte(0x10330d, 0x00);   		// reg_ckg_dvbtc_inner2x@0x06[11:8]: MPLLDIV10/2=43.2MHz

    MDrv_WriteByte(0x10330e, 0x00);   		// reg_ckg_dvbtc_inner4x@0x07[3:0] : MPLLDIV10=86.4MHz
    MDrv_WriteByte(0x10330f, 0x00);

    MDrv_WriteByte(0x103310, 0x00);   		// reg_ckg_dvbtc_outer1x@0x08[3:0] : MPLLDIV10/2=43.2MHz
    MDrv_WriteByte(0x103311, 0x00);   		// reg_ckg_dvbtc_outer2x@0x08[11:8]: MPLLDIV10=86.4MHz

    MDrv_WriteByte(0x103312, 0x00);   		// dvbt_t:0x0000, dvb_c: 0x0004
    MDrv_WriteByte(0x103313, 0x00);

    MDrv_WriteByte(0x103314, 0x00);   		// reg_ckg_dvbtc_adc@0x0a[3:0] : ADC_CLK
    MDrv_WriteByte(0x103315, 0x00);   		// reg_ckg_dvbtc_innc@0x0a[11:8]

    MDrv_WriteByte(0x103316, 0x00);   		// reg_ckg_dvbtc_eq8x@0x0b[3:0] : MPLLDIV3/2=144MHz
    MDrv_WriteByte(0x103317, 0x00);   		// reg_ckg_dvbtc_eq@0x0b[11:8] : MPLLDIV3/16=18MHz

    MDrv_WriteByte(0x103318, 0x00);   		// reg_ckg_dvbtc_sram0~3@0x0c[13:0]
    MDrv_WriteByte(0x103319, 0x00);

#if 0 //parallel
    MDrv_WriteByte(0x103308, 0x01);   		// parallel mode:0x0001 / serial mode: 0x0401
    MDrv_WriteByte(0x103309, 0x00);   		// reg_ckg_dvbtc_ts@0x04

    MDrv_WriteByte(0x103300, 0x11);   		// parallel mode: 0x0511 /serial mode 0x0400

	#if(INTERN_DVBT_TS_PARALLEL_INVERSION == 0)
    	MDrv_WriteByte(0x103301, 0x05);   	// reg_ckg_dvbtmk_ts_out_mode@0x00
	#else
    	MDrv_WriteByte(0x103301, 0x07);   	// reg_ckg_dvbtmk_ts_out_mode@0x00
	#endif
#else // serial
    MDrv_WriteByte(0x103308, 0x01);   		// parallel mode:0x0001 / serial mode: 0x0401
    MDrv_WriteByte(0x103309, 0x04);   		// reg_ckg_dvbtc_ts@0x04

    MDrv_WriteByte(0x103300, 0x00);   		// parallel mode: 0x0511 /serial mode 0x0400
	#if(INTERN_DVBT_TS_SERIAL_INVERSION == 0)
    	MDrv_WriteByte(0x103301, 0x04); 	// reg_ckg_dvbtmk_ts_out_mode@0x00
	#else
    	MDrv_WriteByte(0x103301, 0x06); 	// reg_ckg_dvbtmk_ts_out_mode@0x00
	#endif
#endif
// !!!
    //MDrv_WriteByte(0x101e22, 0x02);   	// internal TS connect to TS0:0x0002; to TS1:0x0200
    //MDrv_WriteByte(0x101e23, 0x00);   	// reg_ts0~1_mux@0x11         (bank_CHIP)

    //MDrv_WriteByte(0x100b50, 0x08);   	// internal TS0:0x0108;   internal TS1:0x0801
    //MDrv_WriteByte(0x100b51, 0x01);   	// reg_ckg_ts0~1@0x28         (bank_CLKGEN0) << Ken 20090629
// !!!
    MDrv_WriteByte(0x101e04, 0x00);   		// reg_if_agc_pad_oen@0x02    (bank_CHIP)
    MDrv_WriteByte(0x101e05, 0x00);

//----------------------------------------
//For T4 Add
//At Chip Top
	MDrv_WriteByte(0x101e9e, MDrv_ReadByte(0x101e9e) & 0x3F);   // Bit6,7 Clean=0
//    MDrv_WriteByte(0x101e9e, 0x00);   	// reg_agcgctrl
//    MDrv_WriteByte(0x101e9f, 0x00);

	MDrv_WriteByte(0x101e05, MDrv_ReadByte(0x101e05) & 0xEF);   // Bit12 Clean=0
//	MDrv_WriteByte(0x101e04, 0x00);   		// reg_if_agc_pad_oen
//	MDrv_WriteByte(0x101e05, 0x00);

	MDrv_WriteByte(0x101eA1, MDrv_ReadByte(0x101eA1) & 0x7F);   // Bit15 Clean=0
//	MDrv_WriteByte(0x101eA0, 0x00);   		// reg_allpad_in
//	MDrv_WriteByte(0x101eA1, 0x00);
// ---------------------------------------
	MDrv_WriteByte(0x112002, 0x52);			// wreg vdbank_DEMOD_0+0x01 0052
	MDrv_WriteByte(0x112003, 0x00);			// Release Ana misc resest

	MDrv_WriteByte(0x112060, 0x00);			// ADC sign bit
	MDrv_WriteByte(0x112061, 0x00);

	MDrv_WriteByte(0x112064, 0x00);			// ADC I channel offset // wreg vdbank_DEMOD_0+0x32 0c00
	MDrv_WriteByte(0x112065, 0x0c);

	MDrv_WriteByte(0x112066, 0x00);			// ADC Q channel offset
	MDrv_WriteByte(0x112067, 0x0c);
// ---------------------------------------
// Enable DVBT demod (MailBox=0x1020)
	MDrv_WriteByte(0x112818, 0x03);			// Set enable ADC clock [ANA_PWDN_ADCI]
	MDrv_WriteByte(0x112819, 0x00);			// wreg vdbank_DMD_ANA_MISC+0x0c 0x0000
	INTERN_DVBT_Delayms(2);

	MDrv_WriteByte(0x11286a, 0x86);			// wreg vdbank_DMD_ANA_MISC+0x35 0x1e04
	MDrv_WriteByte(0x11286b, 0x1e);			// [ANA_MPLL_ICTRL]
	INTERN_DVBT_Delayms(2);

// ---------------------------------------------------------------------
// DVBT , DVBC Clock Setting
// ---------------------------------------------------------------------
	MDrv_WriteByte(0x11286a, 0x06);			// Reset MPLL
	MDrv_WriteByte(0x11286b, 0x1e);			// wreg vdbank_DMD_ANA_MISC+0x35 0x1e04
	INTERN_DVBT_Delayms(2);

	MDrv_WriteByte(0x11286a, 0x06);			// Disable MPLL reset
	MDrv_WriteByte(0x11286b, 0x06);			// wreg vdbank_DMD_ANA_MISC+0x35 0x0604
	INTERN_DVBT_Delayms(2);

	MDrv_WriteByte(0x112866, 0x02);			// Set MPLL_LOOP_DIV_FIRST and SECOND
	MDrv_WriteByte(0x112867, 0x09);			// [ANA_MPLL_LOOP_DIV_FIRST]

	MDrv_WriteByte(0x112860, 0x00);			// Set MPLL_ADC_DIV_SEL [ANA_MPLL_CLK_DP_PD]
	MDrv_WriteByte(0x112861, 0x13);			// wreg vdbank_DMD_ANA_MISC+0x30 0x1300

	MDrv_WriteByte(0x112802, 0x40);			// Set IMUXS QMUXS [ANA_ADC_PWDNI]
	MDrv_WriteByte(0x112803, 0x04);			// wreg vdbank_DMD_ANA_MISC+0x01 0x0440

	MDrv_WriteByte(0x112818, 0x00);			// Set enable ADC clock [ANA_PWDN_ADCI]
	MDrv_WriteByte(0x112819, 0x00);			// wreg vdbank_DMD_ANA_MISC+0x0c 0x0000

	MDrv_WriteByte(0x112840, 0x86);			// Disable PWDN_REF [ANA_VCLP]
	MDrv_WriteByte(0x112841, 0x1e);			// wreg vdbank_DMD_ANA_MISC+0x20 0x0000
#endif
}

/***********************************************************************************
  Subject:    Power on initialized function
  Function:   INTERN_DVBT_Power_On_Initialization
  Parmeter:
  Return:     BOOLEAN
  Remark:
************************************************************************************/

BOOLEAN INTERN_DVBT_Power_On_Initialization ( void )
{
    U8            status = true;
    //U8            cal_done;

    //INTERN_DVBT_InitRegBase();
#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 ) //T3
	printf("INTERN_DVBT_Power_On_Initialization call stop vdmcu\n");
  #if ENABLE_PWS
    Mapi_PWS_Stop_VDMCU();
  #endif
#endif

    INTERN_DVBT_InitClkgen();

    //// Firmware download //////////
    DBG_INTERN_DVBT(printf("INTERN_DVBT Load DSP...\n"));
    //MsOS_DelayTask(100);

#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7 ) //T3
    if (MDrv_ReadByte(0x101E3E) != 0x02) // DVBT = BIT1 -> 0x02
    {
        if (INTERN_DVBT_LoadDSPCode() == FALSE)
        {
            printf("DVB-T Load DSP Code Fail\n");
            return FALSE;
        }
        else
        {
            printf("DVB-T Load DSP Code OK\n");
        }
    }
#elif ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD ) //T4

    extern BOOLEAN devCOFDM_WriteReg(U16 RegAddr, U16 RegData);

	//reset VIF setting to avoid affect demod
    devCOFDM_WriteReg(0x2200, 0xFF);
    devCOFDM_WriteReg(0x2204, 0x00);

    if (INTERN_DVBT_LoadDSPCode() == FALSE)
    {
        printf("DVB-T Load DSP Code Fail\n");
        return FALSE;
    }
    else
    {
        printf("DVB-T Load DSP Code OK\n");
    }
#endif

#if 0
    //// MCU Reset //////////
    DBG_INTERN_DVBT(printf("INTERN_DVBT Reset...\n"));
    if (INTERN_DVBT_Reset() == FALSE)
    {
        DBG_INTERN_DVBT(printf("Fail\n"));
        return FALSE;
    }
    else
    {
        DBG_INTERN_DVBT(printf("OK\n"));
    }
#endif

//    INTERN_DVBT_ReadReg(0x8000, &gu8ChipRevId);
//    DBG_INTERN_DVBT(printf("DVB-T RevID:%bx\n", gu8ChipRevId));

    //// DAC calibration //////////
//    DBG_INTERN_DVBT(printf("INTERN_DVBT Cal DAC...\n"));
//
//    gsCmdPacket.cmd_code = CMD_DAC_CALI;
//    gsCmdPacket.param[dac_op_code] = DAC_RUN_CALI;
//    status &= INTERN_DVBT_Cmd_Packet_Send(&gsCmdPacket, 1);
//    status &= INTERN_DVBT_Cmd_Packet_Exe_Check(&cal_done);
//    if (cal_done == true)
//    {
//        DBG_INTERN_DVBT(printf("OK\n"));
//        status &= INTERN_DVBT_ReadReg(0x81A8, &gCalIdacCh0);
//        status &= INTERN_DVBT_ReadReg(0x81A9, &gCalIdacCh1);
//    }
//    else
//    {
//        gCalIdacCh0 = gCalIdacCh1 = 0;
//        DBG_INTERN_DVBT(printf("FAIL\n"));
//    }

    return status;
}

/************************************************************************************************
  Subject:    Driving control
  Function:   INTERN_DVBT_Driving_Control
  Parmeter:   bInversionEnable : TRUE For High
  Return:      void
  Remark:
*************************************************************************************************/
void INTERN_DVBT_Driving_Control(BOOLEAN bEnable)
{
    U8    u8Temp;

    u8Temp = MDrv_ReadByte(0x101E10);

    if (bEnable)
    {
       u8Temp = u8Temp | 0x01; //bit0: clk, bit1~8:data , bit9: sync, bit10:valid
    }
    else
    {
       u8Temp = u8Temp & (~0x01);
    }

    DBG_INTERN_DVBT(printf("---> INTERN_DVBT_Driving_Control(Bit0) = 0x%bx \n",u8Temp));
    MDrv_WriteByte(0x101E10, u8Temp);
}
/************************************************************************************************
  Subject:    Clk Inversion control
  Function:   INTERN_DVBT_Clk_Inversion_Control
  Parmeter:   bInversionEnable : TRUE For Inversion Action
  Return:      void
  Remark:
*************************************************************************************************/
void INTERN_DVBT_Clk_Inversion_Control(BOOLEAN bInversionEnable)
{
    U8    u8Temp;

    u8Temp = MDrv_ReadByte(0x103301);

    if (bInversionEnable)
    {
       u8Temp = u8Temp | 0x02; //bit 9: clk inv
    }
    else
    {
       u8Temp = u8Temp & (~0x02);
    }

    DBG_INTERN_DVBT(printf("---> Inversion(Bit9) = 0x%bx \n",u8Temp));
    MDrv_WriteByte(0x103301, u8Temp);
}
/************************************************************************************************
  Subject:    Transport stream serial/parallel control
  Function:   INTERN_DVBT_Serial_Control
  Parmeter:   bEnable : TRUE For serial
  Return:     BOOLEAN :
  Remark:
*************************************************************************************************/
BOOLEAN INTERN_DVBT_Serial_Control(BOOLEAN bEnable)
{
    U8            status = true;

    DBG_INTERN_DVBT(printf(" @INTERN_DVBT_ts... \n"));

    if (bEnable)    //Serial mode for TS pad
    {
        // serial
        MDrv_WriteByte(0x103308, 0x01);   // serial mode: 0x0401
        MDrv_WriteByte(0x103309, 0x04);   // reg_ckg_dvbtc_ts@0x04

        MDrv_WriteByte(0x103300, 0x00);   // serial mode 0x0400
#if(INTERN_DVBT_TS_SERIAL_INVERSION == 0)
        MDrv_WriteByte(0x103301, 0x04);   // reg_ckg_dvbtmk_ts_out_mode@0x00
#else
        MDrv_WriteByte(0x103301, 0x06);   // reg_ckg_dvbtmk_ts_out_mode@0x00
#endif
        MDrv_WriteByte(0x101EA5, MDrv_ReadByte(0x101EA5)&0xEF);   // PAD_TS1 is used as output

        //// INTERN_DVBT TS Control: Serial //////////
        gsCmdPacket.cmd_code = CMD_TS_CTRL;

        gsCmdPacket.param[0] = TS_SERIAL;
#if(INTERN_DVBT_TS_SERIAL_INVERSION == 0)
        gsCmdPacket.param[1] = 0;//TS_CLK_NO_INV;
#else
        gsCmdPacket.param[1] = 1;//TS_CLK_INVERSE;
#endif
        status &= INTERN_DVBT_Cmd_Packet_Send(&gsCmdPacket, 2);
    }
    else
    {
        //parallel
        MDrv_WriteByte(0x103308, 0x01);   // parallel mode:0x0001
        MDrv_WriteByte(0x103309, 0x00);   // reg_ckg_dvbtc_ts@0x04

        //MDrv_WriteByte(0x103300, 0x11);   // parallel mode: 0x0511 => ts_clk=288/(2*(0x11+1))=8MHz
        MDrv_WriteByte(0x103300, 0x13);   // parallel mode: 0x0513 => ts_clk=288/(2*(0x13+1))=7.2MHz
#if(INTERN_DVBT_TS_PARALLEL_INVERSION == 0)
        MDrv_WriteByte(0x103301, 0x05);   // reg_ckg_dvbtmk_ts_out_mode@0x00
#else
        MDrv_WriteByte(0x103301, 0x07);   // reg_ckg_dvbtmk_ts_out_mode@0x00
#endif
        MDrv_WriteByte(0x101EA5, MDrv_ReadByte(0x101EA5)|0x10);   // PAD_TS1 is used as output

        //// INTERN_DVBT TS Control: Parallel //////////
        gsCmdPacket.cmd_code = CMD_TS_CTRL;

        gsCmdPacket.param[0] = TS_PARALLEL;
#if(INTERN_DVBT_TS_PARALLEL_INVERSION == 0)
        gsCmdPacket.param[1] = 0;//TS_CLK_NO_INV;
#else
        gsCmdPacket.param[1] = 1;//TS_CLK_INVERSE;
#endif
        status &= INTERN_DVBT_Cmd_Packet_Send(&gsCmdPacket, 2);
    }

    DBG_INTERN_DVBT(printf("---> Inversion(Bit5) = 0x%bx \n",gsCmdPacket.param[1] ));

    INTERN_DVBT_Driving_Control(INTERN_DVBT_DTV_DRIVING_LEVEL);
    return status;
}


/************************************************************************************************
  Subject:    channel change config
  Function:   INTERN_DVBT_Config
  Parmeter:   BW: bandwidth
  Return:     BOOLEAN :
  Remark:
*************************************************************************************************/
BOOLEAN INTERN_DVBT_Config ( RF_CHANNEL_BANDWIDTH BW, BOOLEAN bSerialTS, BOOLEAN bPalBG)
{
    U16             fs, fc;
    U8              bandwidth;
    U8              status = true;

    DBG_INTERN_DVBT(printf(" @INTERN_DVBT_config\n"));

    fs = 45473;
    fc = fs - 36167;

    switch(BW)
    {
        /*    no provided
        case E_RF_CH_BAND_5MHz:
            bandwidth = 0;
            break;
        */
        case E_RF_CH_BAND_6MHz:
            bandwidth = 1;
            break;
        case E_RF_CH_BAND_7MHz:
            bandwidth = 2;
            break;
        case E_RF_CH_BAND_8MHz:
        default:
            bandwidth = 3;
            break;
    }

    status &= INTERN_DVBT_Reset();

//// INTERN_DVBT system init: DVB-T //////////
    gsCmdPacket.cmd_code = CMD_SYSTEM_INIT;

    gsCmdPacket.param[0] = E_SYS_DVBT;
    status &= INTERN_DVBT_Cmd_Packet_Send(&gsCmdPacket, 1);

//// DVB-T configurate ///////////////////
    gsCmdPacket.cmd_code = CMD_DVBT_CONFIG;

    gsCmdPacket.param[p_opmode_rfagc_en]        = 0;
    gsCmdPacket.param[p_opmode_humdet_en]       = 0;
    gsCmdPacket.param[p_opmode_dcr_en]          = 1;
    gsCmdPacket.param[p_opmode_iqb_en]          = 0;
    gsCmdPacket.param[p_opmode_auto_iq_swap]    = 1;
    gsCmdPacket.param[p_opmode_auto_fsa_left]   = 1;
    gsCmdPacket.param[p_opmode_auto_rfmax]      = 0;
    gsCmdPacket.param[p_opmode_mode_forced]     = 0;
    gsCmdPacket.param[p_opmode_cp_forced]       = 0;
    gsCmdPacket.param[pc_config_rssi]           = 0;
    gsCmdPacket.param[pc_config_zif]            = 0;
    gsCmdPacket.param[pc_config_fc_l]           = (U8)(fc%256); //0x5a;
    gsCmdPacket.param[pc_config_fc_h]           = (U8)(fc/256); //0x24;
    gsCmdPacket.param[pc_config_fs_l]           = (U8)(fs%256); //0xa1;
    gsCmdPacket.param[pc_config_fs_h]           = (U8)(fs/256); //0xb1;
    gsCmdPacket.param[pc_config_bw]             = bandwidth;
    gsCmdPacket.param[pc_config_fsa_left]       = 0;
    gsCmdPacket.param[pc_config_rfmax]          = 1;
    gsCmdPacket.param[pc_config_lp_sel]         = 0;
    gsCmdPacket.param[pc_config_cp]             = 3;
    gsCmdPacket.param[pc_config_mode]           = 1;
    gsCmdPacket.param[pc_config_iq_swap]        = 0; //mick iq swap
    gsCmdPacket.param[pc_config_atv_system]     = bPalBG? 0:1;
    gsCmdPacket.param[pc_config_serial_ts]      = bSerialTS;
    gsCmdPacket.param[pc_config_data_swap]      = 0;
    gsCmdPacket.param[pc_config_icfo_range]     = 1;

    if (bSerialTS)
    {
#if(INTERN_DVBT_TS_SERIAL_INVERSION == 0)
        gsCmdPacket.param[pc_config_ts_out_inv] = false;
#else
        gsCmdPacket.param[pc_config_ts_out_inv] = true;
#endif
    }
    else
    {
#if(INTERN_DVBT_TS_PARALLEL_INVERSION == 0)
        gsCmdPacket.param[pc_config_ts_out_inv] = false;
#else
        gsCmdPacket.param[pc_config_ts_out_inv] = true;
#endif
    }
    status &= INTERN_DVBT_Cmd_Packet_Send(&gsCmdPacket, DVBT_PARAM_LEN);

#if (FRONTEND_TUNER_TYPE == LG_TDTC_G311D_TUNER)
    gsCmdPacket.cmd_code = CMD_TUNED_VALUE;
    gsCmdPacket.param[agc_ref_small] = 168;
    gsCmdPacket.param[agc_ref_large] = 248;
    gsCmdPacket.param[agc_ref_aci]   = 150;
    gsCmdPacket.param[ripple_switch_th_l] = 0xCD; //461 (0.90)
    gsCmdPacket.param[ripple_switch_th_h] = 0x01;
    status &= INTERN_DVBT_Cmd_Packet_Send(&gsCmdPacket, TUNED_PARAM_MAX_NUM);
#endif

    if(bSerialTS)
    {
        // serial
        MDrv_WriteByte(0x103308, 0x01);   // parallel mode:0x0001 / serial mode: 0x0401
        MDrv_WriteByte(0x103309, 0x04);   // reg_ckg_dvbtc_ts@0x04

        MDrv_WriteByte(0x103300, 0x00);   // parallel mode: 0x0511 /serial mode 0x0400
    #if(INTERN_DVBT_TS_SERIAL_INVERSION == 0)
        MDrv_WriteByte(0x103301, 0x04);   // reg_ckg_dvbtmk_ts_out_mode@0x00
    #else
        MDrv_WriteByte(0x103301, 0x06);   // reg_ckg_dvbtmk_ts_out_mode@0x00
    #endif
    }
    else
    {
        //parallel
        MDrv_WriteByte(0x103308, 0x01);   // parallel mode:0x0001 / serial mode: 0x0401
        MDrv_WriteByte(0x103309, 0x00);   // reg_ckg_dvbtc_ts@0x04

        //MDrv_WriteByte(0x103300, 0x11);   // parallel mode: 0x0511 => ts_clk=288/(2*(0x11+1))=8MHz
        MDrv_WriteByte(0x103300, 0x13);   // parallel mode: 0x0513 => ts_clk=288/(2*(0x13+1))=7.2MHz
    #if(INTERN_DVBT_TS_PARALLEL_INVERSION == 0)
        MDrv_WriteByte(0x103301, 0x05);   // reg_ckg_dvbtmk_ts_out_mode@0x00
    #else
        MDrv_WriteByte(0x103301, 0x07);   // reg_ckg_dvbtmk_ts_out_mode@0x00
    #endif
    }
    return status;
}
/************************************************************************************************
  Subject:    enable hw to lock channel
  Function:   INTERN_DVBT_Active
  Parmeter:   bEnable
  Return:     BOOLEAN
  Remark:
*************************************************************************************************/
BOOLEAN INTERN_DVBT_Active(BOOLEAN bEnable)
{
    U8            status = true;

    DBG_INTERN_DVBT(printf(" @INTERN_DVBT_active\n"));

    //// INTERN_DVBT Finite State Machine on/off //////////
    gsCmdPacket.cmd_code = CMD_FSM_CTRL;

    gsCmdPacket.param[0] = (U8)bEnable;
    status &= INTERN_DVBT_Cmd_Packet_Send(&gsCmdPacket, 1);

    return status;
}
/************************************************************************************************
  Subject:    Return lock status
  Function:   INTERN_DVBT_Lock
  Parmeter:   eStatus :
  Return:     BOOLEAN
  Remark:
*************************************************************************************************/
BOOLEAN INTERN_DVBT_Lock( COFDM_LOCK_STATUS eStatus )
{
    U16 u16Address;
    BYTE cData;
    BYTE cBitMask;

    static U32      u32FecLockTime;

    switch( eStatus )
    {
    case COFDM_FEC_LOCK:
        INTERN_DVBT_ReadReg(0x23E0, &cData);
        if (cData == 0x0B)
        {
            u32FecLockTime = MsOS_GetSystemTime();
            FECLock = TRUE;
            //printf("dmd_lock\n");

            return TRUE;
        }
        else
        {
            if (FECLock == TRUE)
            {
                if ( ( MsOS_GetSystemTime() - u32FecLockTime ) < 3000 )
                    return TRUE;
            }
            FECLock = FALSE;
            //printf("dmd_unlock\n");
            return FALSE;      // continuously un-lock
        }
        break;

    case COFDM_PSYNC_LOCK:
        u16Address =  0x232C; //FEC: P-sync Lock,
        cBitMask = BIT1;
        break;

    case COFDM_TPS_LOCK:
        u16Address =  0x2222; //TPS Lock,
        cBitMask = BIT1;
        break;

    case COFDM_DCR_LOCK:
        u16Address =  0x2145; //DCR Lock,
        cBitMask = BIT0;
        break;

    case COFDM_AGC_LOCK:
        u16Address =  0x212F; //AGC Lock,
        cBitMask = BIT0;
        break;

    case COFDM_MODE_DET:
        u16Address =  0x24CF; //Mode CP Detect,
        cBitMask = BIT4;
        break;

    default:
        return FALSE;
    }

    if (INTERN_DVBT_ReadReg(u16Address, &cData) == FALSE)
        return FALSE;

    if ((cData & cBitMask) == cBitMask)
    {
        return TRUE;
    }

    return FALSE;

}

/****************************************************************************
  Subject:    To get the Post viterbi BER
  Function:   INTERN_DVBT_GetPostViterbiBer
  Parmeter:  Quility
  Return:       E_RESULT_SUCCESS
                   E_RESULT_FAILURE =>Read I2C fail, INTERN_DVBT_VIT_STATUS_NG
  Remark:     For the Performance issue, here we just return the Post Value.(Not BER)
                   We will not read the Period, and have the "/256/8"
*****************************************************************************/
BOOLEAN INTERN_DVBT_GetPostViterbiBer(float *ber)
{
    U8            status = true;
    U8            reg, reg_frz;
    U16            BitErrPeriod;
    U32            BitErr;
    U16            PktErr;

    /////////// Post-Viterbi BER /////////////

    // bank 7 0x32 [7] reg_bit_err_num_freeze
    status &= INTERN_DVBT_ReadReg(0x2332, &reg_frz);
    status &= INTERN_DVBT_WriteReg(0x2332, reg_frz|0x80);

    // bank 7 0x30 [7:0] reg_bit_err_sblprd_7_0
    //               [15:8] reg_bit_err_sblprd_15_8
    status &= INTERN_DVBT_ReadReg(0x2331, &reg);
    BitErrPeriod = reg;

    status &= INTERN_DVBT_ReadReg(0x2330, &reg);
    BitErrPeriod = (BitErrPeriod << 8)|reg;

    // bank 7 0x3a [7:0] reg_bit_err_num_7_0
    //               [15:8] reg_bit_err_num_15_8
    // bank 7 0x3c [7:0] reg_bit_err_num_23_16
    //               [15:8] reg_bit_err_num_31_24

    status &= INTERN_DVBT_ReadReg(0x233D, &reg);
    BitErr = reg;

    status &= INTERN_DVBT_ReadReg(0x233C, &reg);
    BitErr = (BitErr << 8)|reg;

    status &= INTERN_DVBT_ReadReg(0x233B, &reg);
    BitErr = (BitErr << 8)|reg;

    status &= INTERN_DVBT_ReadReg(0x233A, &reg);
    BitErr = (BitErr << 8)|reg;

    // bank 7 0x3e [7:0] reg_uncrt_pkt_num_7_0
    //               [15:8] reg_uncrt_pkt_num_15_8
    status &= INTERN_DVBT_ReadReg(0x233F, &reg);
    PktErr = reg;

    status &= INTERN_DVBT_ReadReg(0x233E, &reg);
    PktErr = (PktErr << 8)|reg;

    // bank 7 0x32 [7] reg_bit_err_num_freeze
    status &= INTERN_DVBT_WriteReg(0x2332, reg_frz);

    if (BitErrPeriod == 0 )    //protect 0
        BitErrPeriod = 1;

    if (BitErr <=0 )
        *ber = 0.5 / ((float)BitErrPeriod*128*188*8);
    else
        *ber = (float)BitErr / ((float)BitErrPeriod*128*188*8);


   DBG_GET_SIGNAL(printf("INTERN_DVBT PostVitBER = %8.3e \n ", *ber));
   DBG_GET_SIGNAL(printf("INTERN_DVBT PktErr = %d \n ", (int)PktErr));


    return status;
}

/****************************************************************************
  Subject:    Function providing approx. result of Log10(X)
  Function:   Log10Approx
  Parmeter:   Operand X in float
  Return:     Approx. value of Log10(X) in float
  Remark:      Ouput range from 0.0, 0.3 to 9.6 (input 1 to 2^32)
*****************************************************************************/
float Log10Approx(float flt_x)
{
    U32       u32_temp = 1;
    U8        indx = 0;

    do {
        u32_temp = u32_temp << 1;
        if (flt_x < (float)u32_temp)
            break;
    }while (++indx < 32);

    // 10*log10(X) ~= 0.3*N, when X ~= 2^N
    return (float)0.3 * indx;
}

/****************************************************************************
  Subject:    Read the signal to noise ratio (SNR)
  Function:   INTERN_DVBT_GetSNR
  Parmeter:   None
  Return:     -1 mean I2C fail, otherwise I2C success then return SNR value
  Remark:
*****************************************************************************/
float INTERN_DVBT_GetSNR (void)
{
    U8            status = true;
    U8            reg, reg_frz;
    U32           noise_power;
    float         snr;

    // bank 6 0xfe [0] reg_fdp_freeze
    status &= INTERN_DVBT_ReadReg(0x22fe, &reg_frz);
    status &= INTERN_DVBT_WriteReg(0x22fe, reg_frz|0x01);

    // bank 6 0xff [0] reg_fdp_load
    status &= INTERN_DVBT_ReadReg(0x22ff, &reg);

    // bank 6 0x4a [26:0] reg_snr_accu <27,1>
    status &= INTERN_DVBT_ReadReg(0x224d, &reg);
    noise_power = reg & 0x07;

    status &= INTERN_DVBT_ReadReg(0x224c, &reg);
    noise_power = (noise_power << 8)|reg;

    status &= INTERN_DVBT_ReadReg(0x224b, &reg);
    noise_power = (noise_power << 8)|reg;

    status &= INTERN_DVBT_ReadReg(0x224a, &reg);
    noise_power = (noise_power << 8)|reg;

    noise_power = noise_power/2;

    // bank 6 0x26 [5:4] reg_transmission_mode
    status &= INTERN_DVBT_ReadReg(0x2226, &reg);

    // bank 6 0xfe [0] reg_fdp_freeze
    status &= INTERN_DVBT_WriteReg(0x22fe, reg_frz);

    if ((reg&0x03)==0x00)     //2K
    {
        if (noise_power<1512)
            snr = 0;
        else
            snr = 10*Log10Approx((float)noise_power/1512);
    }
    //else if ((reg&0x03)==0x01)//8K
    else
    {
        if (noise_power<6048)
            snr = 0;
        else
            snr = 10*Log10Approx((float)noise_power/6048);
    }
    /* ignore 4K
    else                       //4K
    {
      if (noise_power<3024)
        snr = 0;
      else
        snr = 10*Log10Approx(noise_power/3024);
    }
    */

    if (status == true)
        return snr;
    else
        return -1;

}

FUNCTION_RESULT INTERN_DVBT_GetSignalStrength(WORD *strength)
{
    *strength = 80;
    return E_RESULT_SUCCESS;
}

/****************************************************************************
  Subject:    To get the DVT Signal quility
  Function:   INTERN_DVBT_GetSignalQuality
  Parmeter:  Quility
  Return:      E_RESULT_SUCCESS
                   E_RESULT_FAILURE
  Remark:    Here we have 4 level range
                  <1>.First Range => Quility =100  (You can define it by INTERN_DVBT_SIGNAL_BASE_100)
                  <2>.2th Range => 60 < Quality < 100 (You can define it by INTERN_DVBT_SIGNAL_BASE_60)
                  <3>.3th Range => 10 < Quality < 60  (You can define it by INTERN_DVBT_SIGNAL_BASE_10)
                  <4>.4th Range => Quality <10
*****************************************************************************/
FUNCTION_RESULT INTERN_DVBT_GetSignalQuality(WORD *quality)
{
    float         fber;
    float         log_ber;

    if (TRUE == INTERN_DVBT_Lock(COFDM_PSYNC_LOCK) )
    {
        if (INTERN_DVBT_GetPostViterbiBer(&fber) == FALSE)
        {
            DBG_INTERN_DVBT(printf("GetPostViterbiBer Fail!\n"));
            return E_RESULT_FAILURE;
        }

        log_ber = (-1)*Log10Approx(1/fber);    // Log10Approx() provide 1~2^32 input range only

        DBG_GET_SIGNAL(printf("Log(BER) = %f\n",log_ber));

        if ( log_ber <= (-7.0) )            // PostVit BER < 1e-7
        {
            *quality = 100;
        }
        else  if ( log_ber < (-3.7) )        // PostVit BER < 2e-4
        {
            *quality = 60 + (((-3.7) - log_ber) / ((-3.7)-(-7.0)) * (100-60));
        }
        else  if ( log_ber < (-2.7) )        // PostVit BER < 2e-3
        {
            *quality = 10 + (((-2.7) - log_ber) / ((-2.7)-(-3.7)) * (60-10));
        }
        else
        {
            *quality = 10;
        }

    }
    else
    {
        *quality = 0;
    }

    DBG_GET_SIGNAL(printf("Signal Quility = %d\n", (int)*quality));

    return E_RESULT_SUCCESS;

}


/****************************************************************************
  Subject:    To get the DVBT Carrier Freq Offset
  Function:   INTERN_DVBT_Get_FreqOffset
  Parmeter:   Frequency offset (in KHz), bandwidth
  Return:     E_RESULT_SUCCESS
              E_RESULT_FAILURE
  Remark:
*****************************************************************************/
FUNCTION_RESULT INTERN_DVBT_Get_FreqOffset(float *pFreqOff, U8 u8BW)
{
    float         N, FreqB;
    float         FreqCfoTd, FreqCfoFd, FreqIcfo;
    U32           RegCfoTd, RegCfoFd, RegIcfo;
    U8            reg_frz, reg;
    U8            status;

    FreqB = (float)u8BW * 8 / 7;

    status = INTERN_DVBT_ReadReg(0x2104+1, &reg_frz);

    status &= INTERN_DVBT_WriteReg(0x2104+1, reg_frz|0x80);

    status &= INTERN_DVBT_ReadReg(0x24c6+2, &reg);
    RegCfoTd = reg;

    status &= INTERN_DVBT_ReadReg(0x24c6+1, &reg);
    RegCfoTd = (RegCfoTd << 8)|reg;

    status &= INTERN_DVBT_ReadReg(0x24c6, &reg);
    RegCfoTd = (RegCfoTd << 8)|reg;

    FreqCfoTd = (float)RegCfoTd;

    if (RegCfoTd & 0x800000)
        FreqCfoTd = FreqCfoTd - (float)0x1000000;

    FreqCfoTd = FreqCfoTd * FreqB * 0.00011642;

    status &= INTERN_DVBT_WriteReg(0x2104+1, reg_frz&(~0x80));

    status &= INTERN_DVBT_ReadReg(0x22fe, &reg_frz);
    status &= INTERN_DVBT_WriteReg(0x22fe, reg_frz|0x01);

    //status &= INTERN_DVBT_WriteReg(0x22fe+1, 0x01);
    status &= INTERN_DVBT_WriteReg(0x22ff, 0x01);

    status &= INTERN_DVBT_ReadReg(0x2230+3, &reg);
    RegCfoFd = reg;

    status &= INTERN_DVBT_ReadReg(0x2230+2, &reg);
    RegCfoFd = (RegCfoFd << 8)|reg;

    status &= INTERN_DVBT_ReadReg(0x2230+1, &reg);
    RegCfoFd = (RegCfoFd << 8)|reg;

    FreqCfoFd = (float)RegCfoFd;

    if (RegCfoFd & 0x800000)
        FreqCfoFd = FreqCfoFd - (float)0x1000000;

    FreqCfoFd = FreqCfoFd * FreqB * 0.00011642;

    status &= INTERN_DVBT_ReadReg(0x2218+1, &reg);
    RegIcfo = reg & 0x07;

    status &= INTERN_DVBT_ReadReg(0x2218, &reg);
    RegIcfo = (RegIcfo << 8)|reg;

    FreqIcfo = (float)RegIcfo;

    if (RegIcfo & 0x400)
        FreqIcfo = FreqIcfo - (float)0x800;

    status &= INTERN_DVBT_ReadReg(0x2226, &reg);
    reg = reg & 0x30;

    switch (reg)
    {
        case 0x00:  N = 2048;  break;
        case 0x20:  N = 4096;  break;
        case 0x10:
        default:    N = 8192;  break;
    }

    FreqIcfo = FreqIcfo * FreqB / N * 1000;         //unit: kHz
    status &= INTERN_DVBT_WriteReg(0x22fe, reg_frz&(~0x01));
    //status &= INTERN_DVBT_WriteReg(0x22fe+1, 0x01);
    status &= INTERN_DVBT_WriteReg(0x22ff, 0x01);
    *pFreqOff = FreqIcfo + (FreqCfoFd + FreqCfoTd)/1000;

    // DBG_GET_SIGNAL(printf("FCFO = %f\n", FreqCfoFd));
    // DBG_GET_SIGNAL(printf("TCFO = %f\n", FreqCfoTd));
    // DBG_GET_SIGNAL(printf("ICFO = %f\n", FreqIcfo));
    DBG_GET_SIGNAL(printf("CFOE = %f\n", *pFreqOff));

    if (status == TRUE)
        return E_RESULT_SUCCESS;
    else
        return E_RESULT_FAILURE;
}


void INTERN_DVBT_Power_ON_OFF(U8 bPowerOn)
{

    bPowerOn = bPowerOn;
}

BOOLEAN INTERN_DVBT_Power_Save(void)
{

    return TRUE;
}

/****************************************************************************
  Subject:    To get the DVBT constellation parameter
  Function:   INTERN_DVBT_Get_TPS_Parameter_Const
  Parmeter:   point to return parameter(0: QPSK, 1:16QAM, 2:64QAM)
  Return:     TRUE
              FALSE
  Remark:     The TPS parameters will be available after TPS lock
*****************************************************************************/
BOOLEAN INTERN_DVBT_Get_TPS_Parameter_Const( WORD * TPS_parameter)
{
    U8 tps_param;

    if ( INTERN_DVBT_ReadReg(0x2224, &tps_param) == FALSE )
        return FALSE;

    *TPS_parameter = tps_param & (BIT0|BIT1) ;
    return TRUE;
}

/***********************************************************************************
  Subject:    read register
  Function:   MDrv_1210_IIC_Bypass_Mode
  Parmeter:
  Return:
  Remark:
************************************************************************************/
//void MDrv_1210_IIC_Bypass_Mode(BOOLEAN enable)
//{
//    UNUSED(enable);
//    if (enable)
//        INTERN_DVBT_WriteReg(0x8010, 0x10);        // IIC by-pass mode on
//    else
//        INTERN_DVBT_WriteReg(0x8010, 0x00);        // IIC by-pass mode off
//}
