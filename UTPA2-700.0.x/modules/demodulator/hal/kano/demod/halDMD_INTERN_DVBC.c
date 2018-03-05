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
#ifdef MSOS_TYPE_LINUX
#include <math.h>
#endif
#include "ULog.h"
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
//#include "apiPWS.h"

#include "MsTypes.h"
#include "drvBDMA.h"
//#include "drvIIC.h"
//#include "msAPI_Tuner.h"
//#include "msAPI_MIU.h"
//#include "BinInfo.h"
//#include "halVif.h"
#include "drvDMD_INTERN_DVBC.h"
#include "halDMD_INTERN_DVBC.h"
#include "halDMD_INTERN_common.h"
#ifdef SUPPORT_AP_BIN_IN_FLASH_2
#include "InfoBlock.h"
#endif
#include "drvMMIO.h"
//#include "TDAG4D01A_SSI_DVBT.c"
#include "drvDMD_VD_MBX.h"
#define TEST_EMBEDED_DEMOD 0
//U8 load_data_variable=1;
//-----------------------------------------------------------------------
#define BIN_ID_INTERN_DVBC_DEMOD BIN_ID_INTERN_DVBC

#define TDE_REG_BASE  0x2400UL
#define INNC_REG_BASE 0x9b00UL      // P2 = 1,  0x11b00 -> 0x1b00
#define EQE_REG_BASE  0x9a00UL			// P2 = 1,  0x11a00 -> 0x1a00
#define EQE2_REG_BASE 0x9c00UL		  // P2 = 1,  0x11c00 -> 0x1c00

#ifdef MS_DEBUG
#define DBG_INTERN_DVBC(x) x
#define DBG_GET_SIGNAL_DVBC(x)   x
#define DBG_INTERN_DVBC_TIME(x)  x
#define DBG_INTERN_DVBC_LOCK(x)  x
#define INTERN_DVBC_INTERNAL_DEBUG 0
#else
#define DBG_INTERN_DVBC(x) //x
#define DBG_GET_SIGNAL_DVBC(x)   //x
#define DBG_INTERN_DVBC_TIME(x)  //x
#define DBG_INTERN_DVBC_LOCK(x)  //x
#define INTERN_DVBC_INTERNAL_DEBUG 0
#endif
#define DBG_DUMP_LOAD_DSP_TIME 0


//#define SIGNAL_LEVEL_OFFSET     0.00f
//#define TAKEOVERPOINT           -60.0f
//#define TAKEOVERRANGE           0.5f
//#define LOG10_OFFSET            -0.21f
#define INTERN_DVBC_USE_SAR_3_ENABLE 0
#define INTERN_DVBT_GET_TIME msAPI_Timer_GetTime0()

#define TUNER_IF 		36167

#define TS_SER_C        0x00    //0: parallel 1:serial

#if (INTERN_DVBC_TS_SERIAL_INVERSION)
#define TS_INV_C        0x01
#else
#define TS_INV_C        0x00
#endif

#define DVBC_FS         45474   //24000
#define CFG_ZIF         0x00    //For ZIF ,FC=0
#define FC_H_C          ( (DVBC_FS-TUNER_IF)>0 ? (((DVBC_FS-TUNER_IF)>>8)&0xFF) : (((TUNER_IF-DVBC_FS)>>8)&0xFF) )
#define FC_L_C          ( (DVBC_FS-TUNER_IF)>0 ? (((DVBC_FS-TUNER_IF))&0xFF) : (((TUNER_IF-DVBC_FS))&0xFF) )
#define FS_H_C          ((DVBC_FS>>8)&0xFF)         // FS
#define FS_L_C          (DVBC_FS&0xFF)
#define AUTO_SCAN_C     0x00    // Auto Scan - 0:channel change, 1:auto-scan
#define IQ_SWAP_C       0x00
#define PAL_I_C         0x00    // PAL_I: 0: Non-Pal-I CCI, 1: Pal-I CCI (for UK)
// Bxko 6875, 6900, 7000, 6125, 4000, 6950
// Symbol Rate: 6875 = 0x1ADB
// Symbol Rate: 6900 = 0x1AF4
// Symbol Rate: 7000 = 0x1B58
// Symbol Rate: 4000 = 0x0FA0
// Symbol Rate: 6125 = 0x17ED
#define SR0_H           0x1A
#define SR0_L           0xF4	//6900
#define SR1_H           0x1B
#define SR1_L           0x58	//7000
#define SR2_H           0x17
#define SR2_L           0xED	//6125
#define SR3_H           0x0F
#define SR3_L           0xA0	//4000
#define SR4_H           0x1B
#define SR4_L           0x26	//6950
#define SR5_H           0x1A  //0xDB
#define SR5_L           0xDB  //0x1A	//6875
#define SR6_H           0x1C
#define SR6_L           0x20	//7200
#define SR7_H           0x1C
#define SR7_L           0x52	//7250
#define SR8_H           0x0B
#define SR8_L           0xB8	//3000
#define SR9_H           0x03
#define SR9_L           0xE8	//1000
#define SR10_H          0x07
#define SR10_L          0xD0	//2000
#define SR11_H          0x00
#define SR11_L          0x00	//0000


#define QAM             0x04 // QAM: 0:16, 1:32, 2:64, 3:128, 4:256

// SAR dependent
#define NO_SIGNAL_TH_A  0xA3
// Tuner dependent
#define NO_SIGNAL_TH_B_L  0xFF //0x00 , Gain
#define NO_SIGNAL_TH_B_H  0xFF //0xDD
#define NO_SIGNAL_TH_C_L  0xff //0x64 , Err
#define NO_SIGNAL_TH_C_H  0xff //0x00
#define DAGC1_REF               0x70
#define DAGC2_REF               0x30
#define AGC_REF_L               0x00
#define AGC_REF_H               0x06

#define INTERN_AUTO_SR_C  1
#define INTERN_AUTO_QAM_C 1

#define ATV_DET_EN        1

#if 0
MS_U8 INTERN_DVBC_DSPREG[] =
{   0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, AUTO_SCAN_C,          // 00h ~ 07h
    INTERN_AUTO_SR_C, INTERN_AUTO_QAM_C, ATV_DET_EN, NO_SIGNAL_TH_B_L, NO_SIGNAL_TH_B_H, NO_SIGNAL_TH_C_L, NO_SIGNAL_TH_C_H, 0x00, 			// 08h ~ 0fh
    0x00, CFG_ZIF, 0x00, FC_L_C, FC_H_C, FS_L_C, FS_H_C, SR0_L,        // 10h ~ 17h
    SR0_H, SR1_L, SR1_H, SR2_L, SR2_H, SR3_L, SR3_H, 0x00,          // 18h ~ 1fh
    0x00, QAM, IQ_SWAP_C, PAL_I_C, TS_SER_C, 0x00, TS_INV_C, 0x00,  // 20h ~27h
};
#else
MS_U8 INTERN_DVBC_DSPREG[] =
{
 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, AUTO_SCAN_C, INTERN_AUTO_SR_C, INTERN_AUTO_QAM_C, ATV_DET_EN, 0x00, 0x00, 0x01, 0x00, //00-0F
 0x00, 0x00, CFG_ZIF, FS_L_C, FS_H_C, 0x88, 0x13, FC_L_C, FC_H_C, SR0_L, SR0_H, SR1_L, SR1_H, SR2_L, SR2_H, SR3_L, 		//10-1F
 SR3_H, SR4_L, SR4_H, SR5_L, SR5_H, SR6_L, SR6_H, SR7_L, SR7_H, SR8_L, SR8_H, SR9_L, SR9_H, SR10_L, SR10_H, SR11_L, 					//20-2F
 SR11_H, 0x00, QAM, IQ_SWAP_C, PAL_I_C, TS_SER_C, 0x00, TS_INV_C, 0x00, 0x00, AGC_REF_L, AGC_REF_H, 0x90, 0xa0, 0x03, 0x05,						//30-3F
 0x05, 0x40, 0x04, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x7F, 0x00, NO_SIGNAL_TH_B_L, NO_SIGNAL_TH_B_H, NO_SIGNAL_TH_C_L,	//40-4F
 NO_SIGNAL_TH_C_H, 0x00, 0x00, 0x00, 0x00, 0x00, DAGC1_REF, DAGC2_REF, 0x73, 0x73, 0x73, 0x73, 0x73, 0x83, 0x83, 0x73,							//50-5F
 0x62, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                         									//60-6C
};
#endif
#define TS_SERIAL_OUTPUT_IF_CI_REMOVED 1 // _UTOPIA

//-----------------------------------------------------------------------
/****************************************************************
*Local Variables                                                                                              *
****************************************************************/

//static MS_BOOL TPSLock = 0;
static MS_U32 u32ChkScanTimeStartDVBC = 0;
static MS_U8 g_dvbc_lock = 0;

//Global Variables
S_CMDPKTREG gsCmdPacketDVBC;
//MS_U8 gCalIdacCh0, gCalIdacCh1;
static MS_BOOL bDMD_DVBC_NoChannelDetectedWithRFPower = FALSE;
static MS_U32 u32DMD_DVBC_NoChannelTimeAccWithRFPower = 0;
#ifdef INTERN_DVBC_LOAD_FW_FROM_CODE_MEMORY
MS_U8 INTERN_DVBC_table[] = {
    #include "fwDMD_INTERN_DVBC.dat"
};

#endif

MS_BOOL INTERN_DVBC_Show_Demod_Version(void);
// MS_BOOL INTERN_DVBC_GetPostViterbiBer(float *ber);
MS_BOOL INTERN_DVBC_GetPacketErr(MS_U16 *pktErr);
//MS_BOOL INTERN_DVBC_GetSNR(float *f_snr);
MS_BOOL INTERN_DVBC_Get_FreqOffset(MS_U32 *config_Fc_reg, MS_U32 *Fc_over_Fs_reg, MS_U16 *Cfo_offset_reg, MS_U8 u8BW);
MS_BOOL INTERN_DVBC_GetCurrentModulationType(DMD_DVBC_MODULATION_TYPE *pQAMMode);
MS_BOOL INTERN_DVBC_GetCurrentSymbolRate(MS_U16 *u16SymbolRate);
//MS_BOOL INTERN_DVBC_GetCurrentSymbolRateOffset(MS_U16 *pData);

#if (INTERN_DVBC_INTERNAL_DEBUG)
void INTERN_DVBC_info(void);
MS_BOOL INTERN_DVBC_Show_AGC_Info(void);
#endif

MS_U16 INTERN_DVBC_DSPReg_Init(const MS_U8 *u8DVBC_DSPReg,  MS_U8 u8Size)
{
    MS_U8   idx = 0, u8RegRead = 0, u8RegWrite = 0, u8Mask = 0;
    MS_U8 status = TRUE;
    MS_U16 u16DspAddr = 0;

    DBG_INTERN_DVBC(ULOGD("DEMOD","INTERN_DVBC_DSPReg_Init\n"));

    #if 0//def MS_DEBUG
    {
        MS_U8 u8buffer[256];
        ULOGD("DEMOD","INTERN_DVBC_DSPReg_Init Reset\n");
        for (idx = 0; idx<sizeof(INTERN_DVBC_DSPREG); idx++)
            MDrv_SYS_DMD_VD_MBX_WriteDSPReg(idx, 0);

        for (idx = 0; idx<sizeof(INTERN_DVBC_DSPREG); idx++)
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(idx, &(u8buffer[idx]));
        ULOGD("DEMOD","INTERN_DVBC_DSPReg_Init ReadBack, should be all 0\n");
        for (idx = 0; idx<sizeof(INTERN_DVBC_DSPREG); idx++)
            ULOGD("DEMOD","%x ", u8buffer[idx]);
        ULOGD("DEMOD","\n");

        ULOGD("DEMOD","INTERN_DVBC_DSPReg_Init Value\n");
        for (idx = 0; idx<sizeof(INTERN_DVBC_DSPREG); idx++)
            ULOGD("DEMOD","%x ", INTERN_DVBC_DSPREG[idx]);
        ULOGD("DEMOD","\n");
    }
    #endif

    for (idx = 0; idx<sizeof(INTERN_DVBC_DSPREG); idx++)
        status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(idx, INTERN_DVBC_DSPREG[idx]);

    // readback to confirm.
    #ifdef MS_DEBUG
    for (idx = 0; idx<sizeof(INTERN_DVBC_DSPREG); idx++)
    {
        status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(idx, &u8RegRead);
        if (u8RegRead != INTERN_DVBC_DSPREG[idx])
        {
            ULOGD("DEMOD","[Error]INTERN_DVBC_DSPReg_Init, idx=%d, drv_val=0x%x, firmware_val=0x%x\n",idx,INTERN_DVBC_DSPREG[idx],u8RegRead);
        }
    }
    #endif

    if (u8DVBC_DSPReg != NULL)
    {
        if (1 == u8DVBC_DSPReg[0])
        {
            u8DVBC_DSPReg+=2;
            for (idx = 0; idx<u8Size; idx++)
            {
                u16DspAddr = *u8DVBC_DSPReg;
                u8DVBC_DSPReg++;
                u16DspAddr = (u16DspAddr) + ((*u8DVBC_DSPReg)<<8);
                u8DVBC_DSPReg++;
                u8Mask = *u8DVBC_DSPReg;
                u8DVBC_DSPReg++;
                status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(u16DspAddr, &u8RegRead);
                u8RegWrite = (u8RegRead & (~u8Mask)) | ((*u8DVBC_DSPReg) & (u8Mask));
                u8DVBC_DSPReg++;
                DBG_INTERN_DVBC(ULOGD("DEMOD","DSP addr:%x mask:%x read:%x write:%x\n", u16DspAddr, u8Mask, u8RegRead, u8RegWrite));
                status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(u16DspAddr, u8RegWrite);
            }
        }
        else
        {
            ULOGD("DEMOD","FATAL: parameter version incorrect\n");
        }
    }

    #if 0//def MS_DEBUG
    {
        MS_U8 u8buffer[256];
        for (idx = 0; idx<sizeof(INTERN_DVBC_DSPREG); idx++)
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(idx, &(u8buffer[idx]));
        ULOGD("DEMOD","INTERN_DVBC_DSPReg_Init ReadBack\n");
        for (idx = 0; idx<sizeof(INTERN_DVBC_DSPREG); idx++)
            ULOGD("DEMOD","%x ", u8buffer[idx]);
        ULOGD("DEMOD","\n");
    }
    #endif

    #if 0//def MS_DEBUG
    {
        MS_U8 u8buffer[256];
        for (idx = 0; idx<128; idx++)
            status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x2380+idx, &(u8buffer[idx]));
        ULOGD("DEMOD","INTERN_DVBC_DSPReg_Init ReadReg 0x2000~0x207F\n");
        for (idx = 0; idx<128; idx++)
        {
            ULOGD("DEMOD","%x ", u8buffer[idx]);
            if ((idx & 0xF) == 0xF) ULOGD("DEMOD","\n");
        }
        ULOGD("DEMOD","\n");
    }
    #endif
    return status;
}

/***********************************************************************************
  Subject:    Command Packet Interface
  Function:   INTERN_DVBC_Cmd_Packet_Send
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
MS_BOOL INTERN_DVBC_Cmd_Packet_Send(S_CMDPKTREG* pCmdPacket, MS_U8 param_cnt)
{
    MS_U8   status = true, indx;
    MS_U8   reg_val, timeout = 0;
    return TRUE;
    // ==== Command Phase ===================
    DBG_INTERN_DVBC(ULOGD("DEMOD","--->INTERN_DVBC (cmd=0x%x)(0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,) \n",pCmdPacket->cmd_code,
            pCmdPacket->param[0],pCmdPacket->param[1],
            pCmdPacket->param[2],pCmdPacket->param[3],
            pCmdPacket->param[4],pCmdPacket->param[5] ));

    // wait _BIT_END clear
    do
    {
        reg_val = HAL_DMD_RIU_ReadByte(REG_CMD_CTRL);
        if((reg_val & _BIT_END) != _BIT_END)
        {
            break;
        }
        MsOS_DelayTask(5);
        if (timeout > 200)
        {
            ULOGD("DEMOD","---> INTERN_DVBC_Cmd_Packet_Send fail on 'wait _BIT_END clear' \n");
            return false;
        }
        timeout++;
    } while (1);

    // set cmd_3:0 and _BIT_START
    reg_val = HAL_DMD_RIU_ReadByte(REG_CMD_CTRL);
    reg_val = (reg_val & 0x0f)|(pCmdPacket->cmd_code << 4)|_BIT_START;
    HAL_DMD_RIU_WriteByte(REG_CMD_CTRL, reg_val);


    //DBG_INTERN_DVBT(ULOGD("DEMOD","demod_config: cmd_code = %bx\n", pCmdPacket->cmd_code));
    // wait _BIT_START clear
    do
    {
        reg_val = HAL_DMD_RIU_ReadByte(REG_CMD_CTRL);
        if((reg_val & _BIT_START) != _BIT_START)
        {
            break;
        }
        MsOS_DelayTask(10);
        if (timeout > 200)
        {
            ULOGD("DEMOD","---> INTERN_DVBC_Cmd_Packet_Send fail on 'wait _BIT_START clear' \n");
            return false;
        }
        timeout++;
    } while (1);

    // ==== Data Phase ======================

    HAL_DMD_RIU_WriteByte(REG_CMD_ADDR, 0x00);

    for (indx = 0; indx < param_cnt; indx++)
    {
        reg_val = HAL_DMD_RIU_ReadByte(REG_CMD_ADDR);
        //DBG_INTERN_DVBT(ULOGD("DEMOD","demod_config: param[%bd] = %bx\n", reg_val, pCmdPacket->param[indx]));

        // set param[indx] and _BIT_DRQ
        HAL_DMD_RIU_WriteByte(REG_CMD_DATA, pCmdPacket->param[indx]);
        reg_val = HAL_DMD_RIU_ReadByte(REG_DTA_CTRL);
        HAL_DMD_RIU_WriteByte(REG_DTA_CTRL, reg_val|_BIT_DRQ);

        // wait _BIT_DRQ clear
        do
        {
            reg_val = HAL_DMD_RIU_ReadByte(REG_DTA_CTRL);
            if ((reg_val & _BIT_DRQ) != _BIT_DRQ)
            {
                break;
            }
            MsOS_DelayTask(5);
            if (timeout > 200)
            {
                ULOGD("DEMOD","---> INTERN_DVBC_Cmd_Packet_Send fail on 'wait _BIT_DRQ clear' \n");
                return false;
            }
            timeout++;
        } while (1);
    }

    // ==== End Phase =======================

    // set _BIT_END to finish command
    reg_val = HAL_DMD_RIU_ReadByte(REG_CMD_CTRL);
    HAL_DMD_RIU_WriteByte(REG_CMD_CTRL, reg_val|_BIT_END);
    //MsOS_ReleaseMutex(_s32_Demod_DVBT_Mutex);
    return status;
}


/***********************************************************************************
  Subject:    Command Packet Interface
  Function:   INTERN_DVBT_Cmd_Packet_Exe_Check
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
MS_BOOL INTERN_DVBC_Cmd_Packet_Exe_Check(MS_BOOL* cmd_done)
{
    return TRUE;
}

/***********************************************************************************
  Subject:    SoftStop
  Function:   INTERN_DVBC_SoftStop
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/

MS_BOOL INTERN_DVBC_SoftStop ( void )
{
    #if 1
    MS_U16     u8WaitCnt=0;

    if (HAL_DMD_RIU_ReadByte(MBRegBase + 0x00))
    {
        ULOGD("DEMOD",">> MB Busy!\n");
        return FALSE;
    }

    HAL_DMD_RIU_WriteByte(MBRegBase + 0x00, 0xA5);                 // MB_CNTL set read mode

    HAL_DMD_RIU_WriteByte(0x103483, 0x02);                         // assert interrupt to VD MCU51
    HAL_DMD_RIU_WriteByte(0x103483, 0x00);                         // de-assert interrupt to VD MCU51

    while(HAL_DMD_RIU_ReadByte(MBRegBase + 0x00)!= 0x5A)           // wait MB_CNTL set done
    {
#if TEST_EMBEDED_DEMOD
        MsOS_DelayTask(1);  // << Ken 20090629
#endif
        if (u8WaitCnt++ >= 0x7FFF)
        {
            ULOGD("DEMOD",">> DVBC SoftStop Fail!\n");
            return FALSE;
        }
    }

    //HAL_DMD_RIU_WriteByte(0x103460, 0x01);                         // reset VD_MCU
    HAL_DMD_RIU_WriteByte(MBRegBase + 0x00, 0x00);                 // MB_CNTL clear
    #endif
    return TRUE;
}


/***********************************************************************************
  Subject:    Reset
  Function:   INTERN_DVBC_Reset
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
extern void HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake(void);
MS_BOOL INTERN_DVBC_Reset ( void )
{
    DBG_INTERN_DVBC(ULOGD("DEMOD"," @INTERN_DVBC_reset\n"));

    //DBG_INTERN_DVBC_TIME(ULOGD("DEMOD","INTERN_DVBC_Reset, t = %ld\n",MsOS_GetSystemTime()));

    // INTERN_DVBC_SoftStop();


    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00, 0x01);     // reset DMD_MCU
    //MDrv_SYS_DMD_VD_MBX_WriteReg(0x2002, 0x72);        // reset DVB-T
    MsOS_DelayTask(5);
    HAL_DMD_RIU_WriteByte(MBRegBase + 0x00 , 0x00);     // clear MB_CNTL
    // MDrv_SYS_DMD_VD_MBX_WriteReg(0x2002, 0x52);
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00, 0x00);
    MsOS_DelayTask(5);

    HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake();
    HAL_DMD_RIU_WriteByte(MBRegBase + 0x00 , 0x00);

    u32ChkScanTimeStartDVBC = MsOS_GetSystemTime();
    g_dvbc_lock = 0;

    return TRUE;
}

/***********************************************************************************
  Subject:    Exit
  Function:   INTERN_DVBC_Exit
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
MS_BOOL INTERN_DVBC_Exit ( void )
{

    return INTERN_DVBC_SoftStop();
}

/***********************************************************************************
  Subject:    Load DSP code to chip
  Function:   INTERN_DVBC_LoadDSPCode
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
static MS_BOOL INTERN_DVBC_LoadDSPCode(void)
{
    MS_U8  udata = 0x00;
    MS_U16 i;
    MS_U16 fail_cnt=0;

#if (DBG_DUMP_LOAD_DSP_TIME==1)
    MS_U32 u32Time;
#endif


#ifndef INTERN_DVBC_LOAD_FW_FROM_CODE_MEMORY
    BININFO BinInfo;
    MS_BOOL bResult;
    MS_U32 u32GEAddr;
    MS_U8 Data;
    MS_S8 op;
    MS_U32 srcaddr;
    MS_U32 len;
    MS_U32 SizeBy4K;
    MS_U16 u16Counter=0;
    MS_U8 *pU8Data;
#endif

#if 0
    if(HAL_DMD_RIU_ReadByte(0x101E3E))
    {
        ULOGD("DEMOD","Warring! Reg[0x101E3E]=%d\n", HAL_DMD_RIU_ReadByte(0x101E3E));
        return FALSE;
    }
#endif

  //  MDrv_Sys_DisableWatchDog();


    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00,  0x01);        // reset VD_MCU
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x01,  0x00);        // disable SRAM
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03,  0x50);        // enable "vdmcu51_if"
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03,  0x51);        // enable auto-increase
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x04,  0x00);        // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x05,  0x00);        // sram address high byte

    ////  Load code thru VDMCU_IF ////
    DBG_INTERN_DVBC(ULOGD("DEMOD",">Load Code.....\n"));
#ifdef INTERN_DVBC_LOAD_FW_FROM_CODE_MEMORY
    for ( i = 0; i < sizeof(INTERN_DVBC_table); i++)
    {
        HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x0C, INTERN_DVBC_table[i]); // write data to VD MCU 51 code sram
    }
#else
    BinInfo.B_ID = BIN_ID_INTERN_DVBC_DEMOD;
    msAPI_MIU_Get_BinInfo(&BinInfo, &bResult);
    if ( bResult != PASS )
    {
        return FALSE;
    }
    //ULOGD("DEMOD","\t DEMOD_MEM_ADR  =%08LX\n", ((DEMOD_MEM_ADR & MIU1) ? (DEMOD_MEM_ADR | MIU_INTERVAL) : (DEMOD_MEM_ADR)));

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
    //ULOGD("DEMOD","\t RRR: SizeBy4K=%d (L=%d)\n", SizeBy4K,BinInfo.B_Len);

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
        //ULOGD("DEMOD","\t i = %08X\n", i);
        //ULOGD("DEMOD","\t len = %08X\n", len);
        op = 1;
        u16Counter = 0 ;
        //ULOGD("DEMOD","\t (B=0x%x)(Src=0x%x)Data =",i,srcaddr);
        while(len--)
        {
            u16Counter ++ ;
            //ULOGD("DEMOD","file: %s, line: %d\n", __FILE__, __LINE__);
            //pU8Data = (MS_U8 *)(srcaddr|0x80000000);
            #if OBA2
            pU8Data = (MS_U8 *)(srcaddr);
            #else
            pU8Data = (MS_U8 *)(srcaddr|0x80000000);
            #endif
            Data  = *pU8Data;

            #if 0
            if(u16Counter < 0x100)
                ULOGD("DEMOD","0x%bx,", Data);
            #endif
            HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x0C, Data); // write data to VD MCU 51 code sram

            srcaddr += op;
        }
     //   ULOGD("DEMOD","\n\n\n");
    }

#if (DBG_DUMP_LOAD_DSP_TIME==1)
    ULOGD("DEMOD","------> INTERN_DVBC Load DSP Time:  (%lu)\n", msAPI_Timer_DiffTimeFromNow(u32Time)) ;
#endif

#endif

    ////  Content verification ////
    DBG_INTERN_DVBC(ULOGD("DEMOD",">Verify Code...\n"));

    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x04, 0x00);         // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x05, 0x00);         // sram address high byte

#ifdef INTERN_DVBC_LOAD_FW_FROM_CODE_MEMORY
    for ( i = 0; i < sizeof(INTERN_DVBC_table); i++)
    {
        udata = HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x10);    // read sram data
        if (udata != INTERN_DVBC_table[i])
        {
            ULOGD("DEMOD",">fail add = 0x%x\n", i);
            ULOGD("DEMOD",">code = 0x%x\n", INTERN_DVBC_table[i]);
            ULOGD("DEMOD",">data = 0x%x\n", udata);

            if (fail_cnt > 10)
            {
                ULOGD("DEMOD",">DVB-C DSP Loadcode fail!");
                return false;
            }
            fail_cnt++;
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
        //ULOGD("DEMOD","\t i = %08LX\n", i);
        //ULOGD("DEMOD","\t len = %08LX\n", len);
        op = 1;
        u16Counter = 0 ;
        //ULOGD("DEMOD","\t (B=0x%bx)(Src=0x%x)Data =",i,srcaddr);
        while(len--)
        {
            u16Counter ++ ;
            //ULOGD("DEMOD","file: %s, line: %d\n", __FILE__, __LINE__);
            //pU8Data = (MS_U8 *)(srcaddr|0x80000000);
            #if OBA2
            pU8Data = (MS_U8 *)(srcaddr);
            #else
            pU8Data = (MS_U8 *)(srcaddr|0x80000000);
            #endif
            Data  = *pU8Data;

            #if 0
            if(u16Counter < 0x100)
                ULOGD("DEMOD","0x%bx,", Data);
            #endif
            udata = HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x10);    // read sram data
            if (udata != Data)
            {
                ULOGD("DEMOD",">fail add = 0x%lx\n", (MS_U32)((i*0x1000)+(0x1000-len)));
                ULOGD("DEMOD",">code = 0x%x\n", Data);
                ULOGD("DEMOD",">data = 0x%x\n", udata);

                if (fail_cnt++ > 10)
                {
                    ULOGD("DEMOD",">DVB-C DSP Loadcode fail!");
                    return false;
                }
            }

            srcaddr += op;
        }
     //   ULOGD("DEMOD","\n\n\n");
    }
#endif

    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, 0x50);     // diable auto-increase
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, 0x00);     // disable "vdmcu51_if"
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x01, 0x01);     // enable SRAM
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00, 0x00);     // release VD_MCU

    DBG_INTERN_DVBC(ULOGD("DEMOD",">DSP Loadcode done."));
    //while(load_data_variable);
    #if 0
    INTERN_DVBC_Config(6875, 128, 36125, 0,1);
    INTERN_DVBC_Active(ENABLE);
    while(1);
    #endif
    HAL_DMD_RIU_WriteByte(0x101E3E, 0x04);     // DVBT = BIT1 -> 0x02

    return TRUE;
}

/***********************************************************************************
  Subject:    DVB-T CLKGEN initialized function
  Function:   INTERN_DVBC_Power_On_Initialization
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
void INTERN_DVBC_InitClkgen(MS_BOOL bRFAGCTristateEnable)
{
//		MS_U8 temp_val;
	HAL_DMD_RIU_WriteByte(0x103c0e, 0x00); //mux from DMD MCU to HK.
	HAL_DMD_RIU_WriteByte(0x101E39, 0x00); //mux from DMD MCU to HK.
	HAL_DMD_RIU_WriteByte(0x112003, (HAL_DMD_RIU_ReadByte(0x112003)&(~(BIT(5)))));      // Release Ana misc resest

// ============================================================== ADC SYNC FLOW START
// DMD_ANA_ADC_SYNC CLK_W
// [4:0] : reg_ckg_adcd 
//       [0]  : disable clock = 1'b1 
//       [1]  : invert clock
// `RIU_W((`RIUBASE_DMD_ANA_MISC>>1)+7'h68, 2'b01, 16'h0001);
// `RIU_W((`RIUBASE_DMD_ANA_MISC>>1)+7'h68, 2'b01, 16'h0001);
HAL_DMD_RIU_WriteByte(0x1128d0, 0x01);
// ============================================================== ADC SYNC FLOW END

    // CLK_DMDMCU clock setting
    // [0] disable clock
    // [1] invert clock
    // [4:2]
    //         000:170 MHz(MPLL_DIV_BUf)
    //         001:160MHz
    //         010:144MHz
    //         011:123MHz
    //         100:108MHz
    //         101:mem_clcok
    //         110:mem_clock div 2
    //         111:select XTAL
    HAL_DMD_RIU_WriteByte(0x10331f,0x00);
    HAL_DMD_RIU_WriteByte(0x10331e,0x10);

    // set parallet ts clock
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h00, 2'b11, 16'h0511);
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h00, 2'b11, 16'h0511);
    // wriu 0x103301 0x06
    // wriu 0x103300 0x19


    //HAL_DMD_RIU_WriteByte(0x103301,0x07);//0x060b,7.2M
    HAL_DMD_RIU_WriteByte(0x103301,0x07);
    HAL_DMD_RIU_WriteByte(0x103300,0x13);

    // enable atsc, DVBTC ts clock
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h04, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h04, 2'b11, 16'h0000);
    // wriu 0x103309 0x00
    // wriu 0x103308 0x00

    HAL_DMD_RIU_WriteByte(0x103309,0x00);
    HAL_DMD_RIU_WriteByte(0x103308,0x00);

    // enable dvbc adc clock
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h0a, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h0a, 2'b11, 16'h0000);
    // wriu 0x103315 0x00
    // wriu 0x103314 0x00

    HAL_DMD_RIU_WriteByte(0x103315,0x00);
    HAL_DMD_RIU_WriteByte(0x103314,0x00);

	// Reset TS divider
    HAL_DMD_RIU_WriteByte(0x103302,0x01);
    HAL_DMD_RIU_WriteByte(0x103302,0x00);
    
    // ==============================================================
// Start demod_0 CLKGEN setting ......
// ==============================================================
// ============================================================== ADC SYNC FLOW START
// ADC_SYNC CLK_R
// enable atsc_adcd_sync clock
// [3:0] : reg_ckg_atsc_adcd_sync
//         [0]  : disable clock 
//         [1]  : invert clock 
//         [3:2]: Select clock source
//                00:  clk_dmdadc_sync
//                01:  1'b0
//                10:  1'b0
//                11:  DFT_CLK  
// `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h05, 2'b11, 16'h0000);
// `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h05, 2'b11, 16'h0000);
HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);

// ADC_SYNC CLK_W / DMD_ANA_ADC_SYNC CLK_R
// enable dvbc adc clock
// [3:0]: reg_ckg_dvbtc_adc
//       [0]  : disable clock 
//       [1]  : invert clock = 1'b0
//       [3:2]: Select clock source => for demod clkgen clk_dvbtc_adc
//      	00:  clk_dmdadc
//      	01:  clk_dmdadc_div2
//      	10:  clk_dmdadc_div4
//      	11:  DFT_CLK  
// `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h0a, 2'b11, 16'h0000);
// `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h0a, 2'b11, 16'h0000);
HAL_DMD_RIU_WriteByte(0x103315, 0x00);
HAL_DMD_RIU_WriteByte(0x103314, 0x00);

// DMD_ANA_ADC_SYNC CLK_W
// [4:0] : reg_ckg_adcd
//       [0]  : disable clock = 1'b0 
//       [1]  : invert clock
// `RIU_W((`RIUBASE_DMD_ANA_MISC>>1)+7'h68, 2'b01, 16'h0000);
// `RIU_W((`RIUBASE_DMD_ANA_MISC>>1)+7'h68, 2'b01, 16'h0000);
HAL_DMD_RIU_WriteByte(0x1128d0, 0x00);
// ============================================================== ADC SYNC FLOW END

    HAL_DMD_RIU_WriteByte(0x152929,0x00);
    HAL_DMD_RIU_WriteByte(0x152928,0x04);
	
    HAL_DMD_RIU_WriteByte(0x152903,0x04);
    HAL_DMD_RIU_WriteByte(0x152902,0x04);

    HAL_DMD_RIU_WriteByte(0x152905,0x00);
    HAL_DMD_RIU_WriteByte(0x152904,0x00);

    HAL_DMD_RIU_WriteByte(0x152907,0x04);
    HAL_DMD_RIU_WriteByte(0x152906,0x00);

    HAL_DMD_RIU_WriteByte(0x111f0b,0x00);
    HAL_DMD_RIU_WriteByte(0x111f0a,0x00);


    HAL_DMD_RIU_WriteByte(0x111f23,0x08);
    HAL_DMD_RIU_WriteByte(0x111f22,0x44);

    HAL_DMD_RIU_WriteByte(0x111f3b,0x00);
    HAL_DMD_RIU_WriteByte(0x111f3a,0x00);	

    HAL_DMD_RIU_WriteByte(0x111f7f,0x00);
    HAL_DMD_RIU_WriteByte(0x111f7e,0x00);	

    HAL_DMD_RIU_WriteByte(0x111f71,0x00);
    HAL_DMD_RIU_WriteByte(0x111f70,0x00);

    HAL_DMD_RIU_WriteByte(0x111f73,0x00);
    HAL_DMD_RIU_WriteByte(0x111f72,0x00);	

    HAL_DMD_RIU_WriteByte(0x111f69,0x88);
    HAL_DMD_RIU_WriteByte(0x111f68,0x00);

    HAL_DMD_RIU_WriteByte(0x111f6b,0x01);
    HAL_DMD_RIU_WriteByte(0x111f6a,0x11);

    HAL_DMD_RIU_WriteByte(0x152923,0x00);
    HAL_DMD_RIU_WriteByte(0x152922,0x44);		

    HAL_DMD_RIU_WriteByte(0x111f25,0x04);
    HAL_DMD_RIU_WriteByte(0x111f24,0x00);

    HAL_DMD_RIU_WriteByte(0x15296d,0x00);
    HAL_DMD_RIU_WriteByte(0x15296c,0x81);

    HAL_DMD_RIU_WriteByte(0x152971,0x1c);
    HAL_DMD_RIU_WriteByte(0x152970,0xc1);
	
    HAL_DMD_RIU_WriteByte(0x152977,0x08);
    HAL_DMD_RIU_WriteByte(0x152976,0x08);	

    HAL_DMD_RIU_WriteByte(0x152981,0x00);
    HAL_DMD_RIU_WriteByte(0x152980,0x00);

    HAL_DMD_RIU_WriteByte(0x152983,0x00);
    HAL_DMD_RIU_WriteByte(0x152982,0x00);

    HAL_DMD_RIU_WriteByte(0x152985,0x00);
    HAL_DMD_RIU_WriteByte(0x152984,0x00);

    HAL_DMD_RIU_WriteByte(0x152987,0x00);
    HAL_DMD_RIU_WriteByte(0x152986,0x00);

    HAL_DMD_RIU_WriteByte(0x111feb,0x18);
    HAL_DMD_RIU_WriteByte(0x111fea,0x14);	

    HAL_DMD_RIU_WriteByte(0x111f74,0x10);

    HAL_DMD_RIU_WriteByte(0x111f77,0x01);

    HAL_DMD_RIU_WriteByte(0x111f79,0x41);
    HAL_DMD_RIU_WriteByte(0x111f78,0x10);	

    HAL_DMD_RIU_WriteByte(0x111fe0,0x08);

    HAL_DMD_RIU_WriteByte(0x111fe3,0x08);
    HAL_DMD_RIU_WriteByte(0x111fe2,0x10);	

    HAL_DMD_RIU_WriteByte(0x111ff0,0x08);

    HAL_DMD_RIU_WriteByte(0x111f31,0x00);

	HAL_DMD_RIU_WriteByte(0x101E39, 0x03); //mux from DMD MCU to HK.

	HAL_DMD_RIU_WriteByte(0x103c0e,0x01);	
}


/***********************************************************************************
  Subject:    Power on initialized function
  Function:   INTERN_DVBC_Power_On_Initialization
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/

MS_BOOL INTERN_DVBC_Power_On_Initialization(MS_BOOL bRFAGCTristateEnable, MS_U8 u8ADCIQMode, MS_U8 u8PadSel, MS_BOOL bPGAEnable, MS_U8 u8PGAGain, const MS_U8 *u8DMD_DVBC_DSPRegInitExt, MS_U8 u8DMD_DVBC_DSPRegInitSize)
{
    MS_U8            status = true;
    DBG_INTERN_DVBC(ULOGD("DEMOD","INTERN_DVBC_Power_On_Initialization\n"));

#if defined(PWS_ENABLE)
    Mapi_PWS_Stop_VDMCU();
#endif

    INTERN_DVBC_InitClkgen(bRFAGCTristateEnable);
    HAL_DMD_ADC_IQ_Switch(u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain);
    //// Firmware download //////////
    ULOGD("DEMOD","INTERN_DVBC Load DSP...\n");
    //MsOS_DelayTask(100);

    //if (HAL_DMD_RIU_ReadByte(0x101E3E) != 0x04) // DVBT = BIT1 -> 0x02
    {
        if (INTERN_DVBC_LoadDSPCode() == FALSE)
        {
            ULOGD("DEMOD","DVB-C Load DSP Code Fail\n");
            return FALSE;
        }
        else
        {
            ULOGD("DEMOD","DVB-C Load DSP Code OK\n");
        }
    }

    status &= INTERN_DVBC_Reset();

    status &= INTERN_DVBC_DSPReg_Init(u8DMD_DVBC_DSPRegInitExt, u8DMD_DVBC_DSPRegInitSize);

    return status;
}
/************************************************************************************************
  Subject:    Driving control
  Function:   INTERN_DVBC_Driving_Control
  Parmeter:   bInversionEnable : TRUE For High
  Return:      void
  Remark:
*************************************************************************************************/
void INTERN_DVBC_Driving_Control(MS_BOOL bEnable)
{
    MS_U8    u8Temp;

    u8Temp = HAL_DMD_RIU_ReadByte(0x101E10);

    if (bEnable)
    {
       u8Temp = u8Temp | 0x01; //bit0: clk, bit1~8:data , bit9: sync, bit10:valid
    }
    else
    {
       u8Temp = u8Temp & (~0x01);
    }

    DBG_INTERN_DVBC(ULOGD("DEMOD","---> INTERN_DVBC_Driving_Control(Bit0) = 0x%x \n",u8Temp));
    HAL_DMD_RIU_WriteByte(0x101E10, u8Temp);
}
/************************************************************************************************
  Subject:    Clk Inversion control
  Function:   INTERN_DVBC_Clk_Inversion_Control
  Parmeter:   bInversionEnable : TRUE For Inversion Action
  Return:      void
  Remark:
*************************************************************************************************/
void INTERN_DVBC_Clk_Inversion_Control(MS_BOOL bInversionEnable)
{
    MS_U8   u8Temp;

    u8Temp = HAL_DMD_RIU_ReadByte(0x103301);

    if (bInversionEnable)
    {
       u8Temp = u8Temp | 0x02; //bit 9: clk inv
    }
    else
    {
       u8Temp = u8Temp & (~0x02);
    }

    DBG_INTERN_DVBC(ULOGD("DEMOD","---> Inversion(Bit9) = 0x%x \n",u8Temp));
    HAL_DMD_RIU_WriteByte(0x103301, u8Temp);
}
/************************************************************************************************
  Subject:    Transport stream serial/parallel control
  Function:   INTERN_DVBC_Serial_Control
  Parmeter:   bEnable : TRUE For serial
  Return:     MS_BOOL :
  Remark:
*************************************************************************************************/
MS_BOOL INTERN_DVBC_Serial_Control(MS_BOOL bEnable, MS_U8 u8TSClk)
{
    MS_U8   status = true;
 return status;	

   
}

/************************************************************************************************
  Subject:    TS1 output control
  Function:   INTERN_DVBC_PAD_TS1_Enable
  Parmeter:   flag : TRUE For Turn on TS1, FALSE For Turn off TS1
  Return:     void
  Remark:
*************************************************************************************************/
void INTERN_DVBC_PAD_TS1_Enable(MS_BOOL flag)
{
    DBG_INTERN_DVBC(ULOGD("DEMOD"," @INTERN_DVBC_TS1_Enable... \n"));

    if(flag) // PAD_TS1 Enable TS CLK PAD
    {
        //ULOGD("DEMOD","=== TS1_Enable ===\n");
        //HAL_DMD_RIU_WriteByte(0x101EA5, HAL_DMD_RIU_ReadByte(0x101EA5)|0x10);   //For T3
        //HAL_DMD_RIU_WriteByte(0x101EAF, HAL_DMD_RIU_ReadByte(0x101EAF)|0x18);   //For T4
        //move to drvSYS HAL_DMD_RIU_WriteByte(0x101EAF, HAL_DMD_RIU_ReadByte(0x101EAF)|0x11);   //For T8
    }
    else // PAD_TS1 Disable TS CLK PAD
    {
        //ULOGD("DEMOD","=== TS1_Disable ===\n");
        //HAL_DMD_RIU_WriteByte(0x101EA5, HAL_DMD_RIU_ReadByte(0x101EA5)&0xEF);   //For T3
        //HAL_DMD_RIU_WriteByte(0x101EAF, HAL_DMD_RIU_ReadByte(0x101EAF)&0xE3);   //For T4
        //move to drvSYS HAL_DMD_RIU_WriteByte(0x101EAF, HAL_DMD_RIU_ReadByte(0x101EAF)&0xC0);   //For T8
    }
}

/************************************************************************************************
  Subject:    channel change config
  Function:   INTERN_DVBC_Config
  Parmeter:   BW: bandwidth
  Return:     MS_BOOL :
  Remark:
*************************************************************************************************/
MS_BOOL INTERN_DVBC_Config(MS_U16 u16SymbolRate, DMD_DVBC_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U8 u8TSClk, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num)
{

    MS_U8              status = true;
    MS_U8              reg_symrate_l, reg_symrate_h;
    //MS_U16             u16Fc = 0;
    // force
    // u16SymbolRate = 0;
    // eQamMode = DMD_DVBC_QAMAUTO;

    pu16_symbol_rate_list = pu16_symbol_rate_list;
    u8_symbol_rate_list_num = u8_symbol_rate_list_num;
 
    //DBG_INTERN_DVBC(ULOGD("DEMOD"," @INTERN_DVBC_config, SR=%d, QAM=%d, u32IFFreq=%ld, bSpecInv=%d, bSerialTS=%d, u8TSClk=%d\n",u16SymbolRate,eQamMode,u32IFFreq,bSpecInv,bSerialTS, u8TSClk));
    //DBG_INTERN_DVBC_TIME(ULOGD("DEMOD","INTERN_DVBC_Config, t = %ld\n",MsOS_GetSystemTime()));

    if (u8TSClk == 0xFF) u8TSClk=0x13;

/*
    switch(u32IFFreq)
    {
        case 36125:
        case 36167:
        case 36000:
        case 6000:
        case 4560:
            //u16Fc = DVBC_FS - u32IFFreq;
            DBG_INTERN_DVBC(ULOGD("DEMOD","Fc freq = %ld\n", DVBC_FS - u32IFFreq));
            break;
        case 44000:
        default:
            ULOGD("DEMOD","IF frequency not supported\n");
            status = false;
            break;
    }
*/

    reg_symrate_l = (MS_U8) (u16SymbolRate & 0xff);
    reg_symrate_h = (MS_U8) (u16SymbolRate >> 8);

    status &= INTERN_DVBC_Reset();

    if (eQamMode == DMD_DVBC_QAMAUTO)
    {
        DBG_INTERN_DVBC(ULOGD("DEMOD","DMD_DVBC_QAMAUTO\n"));
        status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBC_OP_AUTO_SCAN_QAM, 0x01);
        // give default value.
        status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBC_CFG_QAM, QAM);
    }
    else
    {
        DBG_INTERN_DVBC(ULOGD("DEMOD","DMD_DVBC_QAM %d\n", eQamMode));
        status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBC_OP_AUTO_SCAN_QAM, 0x00);
        status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBC_CFG_QAM, eQamMode);
    }
    // auto symbol rate enable/disable
    if (u16SymbolRate == 0)
    {
        status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBC_OP_AUTO_SCAN_SYM_RATE, 0x01);
    }
    else
    {
        status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBC_OP_AUTO_SCAN_SYM_RATE, 0x00);
        status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBC_CFG_BW0_L, reg_symrate_l);
        status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBC_CFG_BW0_H, reg_symrate_h);
    }
    // TS mode
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBC_CFG_TS_SERIAL, bSerialTS? 0x01:0x00);

    // IQ Swap
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBC_CFG_IQ_SWAP, bSpecInv? 0x01:0x00);

    // Fc
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBC_CFG_FC_L, (abs(DVBC_FS-u32IFFreq))&0xff);
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBC_CFG_FC_H, (abs((DVBC_FS-u32IFFreq))>>8)&0xff);
    // Lif
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBC_OP_LIF_EN, (u32IFFreq < 10000) ? 1 : 0);
    // Fif
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBC_CFG_FIF_L, (u32IFFreq)&0xff);
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBC_CFG_FIF_H, (u32IFFreq>>8)&0xff);

//// INTERN_DVBC system init: DVB-C //////////
//    gsCmdPacketDVBC.cmd_code = CMD_SYSTEM_INIT;

//    gsCmdPacketDVBC.param[0] = E_SYS_DVBC;
//    status &= INTERN_DVBC_Cmd_Packet_Send(&gsCmdPacketDVBC, 1);

#if (INTERN_DVBC_INTERNAL_DEBUG == 1)
    INTERN_DVBC_Show_Demod_Version();
#endif

    return status;
}
/************************************************************************************************
  Subject:    enable hw to lock channel
  Function:   INTERN_DVBC_Active
  Parmeter:   bEnable
  Return:     MS_BOOL
  Remark:
*************************************************************************************************/
MS_BOOL INTERN_DVBC_Active(MS_BOOL bEnable)
{
    MS_U8   status = true;

    DBG_INTERN_DVBC(ULOGD("DEMOD"," @INTERN_DVBC_active\n"));

    //// INTERN_DVBC Finite State Machine on/off //////////
    #if 0
    gsCmdPacketDVBC.cmd_code = CMD_FSM_CTRL;

    gsCmdPacketDVBC.param[0] = (MS_U8)bEnable;
    status &= INTERN_DVBC_Cmd_Packet_Send(&gsCmdPacketDVBC, 1);
    #else
    HAL_DMD_RIU_WriteByte(0x112600 + (0x0e)*2, 0x01);   // FSM_EN
    #endif

    bDMD_DVBC_NoChannelDetectedWithRFPower = FALSE;
    u32DMD_DVBC_NoChannelTimeAccWithRFPower = 0;
    return status;
}


MS_BOOL INTERN_DVBC_GetLock(DMD_DVBC_GETLOCK_TYPE eType, MS_U32 u32CurrRFPowerDbm, MS_U32 u32NoChannelRFPowerDbm, MS_U32 u32TimeInterval)
{
    MS_U16 u16Address = 0;
    MS_U8 cData = 0;
    MS_U8 cBitMask = 0;
    

    if (u32CurrRFPowerDbm < 1000)
    {
        if (eType == DMD_DVBC_GETLOCK_NO_CHANNEL)
        {
            MDrv_SYS_DMD_VD_MBX_ReadReg(FEC_REG_BASE + 0xE0, &cData);
            if (cData > 5)
            {
                bDMD_DVBC_NoChannelDetectedWithRFPower = FALSE;
                u32DMD_DVBC_NoChannelTimeAccWithRFPower = 0;
            }
            else
            {
                if ((u32CurrRFPowerDbm<u32NoChannelRFPowerDbm) && u32DMD_DVBC_NoChannelTimeAccWithRFPower<10000)
                {
                    u32DMD_DVBC_NoChannelTimeAccWithRFPower+=u32TimeInterval;
                }
                if (u32DMD_DVBC_NoChannelTimeAccWithRFPower>1500)
                {
                    bDMD_DVBC_NoChannelDetectedWithRFPower=1;
                    #ifdef MS_DEBUG
                    ULOGD("DEMOD","INTERN_DVBC_GetLock:DMD_DVBC_GETLOCK_NO_CHANNEL Detected Detected Detected!!\n");
                    #endif
                    return TRUE;
                }
            }
            #ifdef MS_DEBUG
            ULOGD("DEMOD","INTERN_DVBC_GetLock:DMD_DVBC_GETLOCK_NO_CHANNEL FSM:%d InputPower:%d Threshold:%d Interval:%ld TimeAcc:%ld NoChannelDetection:%d\n",cData, u32CurrRFPowerDbm, u32NoChannelRFPowerDbm, u32TimeInterval, u32DMD_DVBC_NoChannelTimeAccWithRFPower, bDMD_DVBC_NoChannelDetectedWithRFPower);
            #endif
        }
    }

    {
        switch( eType )
        {
            case DMD_DVBC_GETLOCK_FEC_LOCK:
                MDrv_SYS_DMD_VD_MBX_ReadReg(FEC_REG_BASE + 0xE0, &cData);
                #if (INTERN_DVBC_INTERNAL_DEBUG)
                INTERN_DVBC_info();
                #endif
                DBG_INTERN_DVBC(ULOGD("DEMOD"," @INTERN_DVBC_GetLock FSM 0x%x\n",cData));
                if (cData == 0x0C)
                {
                    if(g_dvbc_lock == 0)
                    {
                      g_dvbc_lock = 1;
                      DBG_INTERN_DVBC(ULOGD("DEMOD","[T12][DVBC]lock++++\n"));

                    }
                    return TRUE;
                }
                else
                {
                    if(g_dvbc_lock == 1)
                    {
                      g_dvbc_lock = 0;
                      DBG_INTERN_DVBC(ULOGD("DEMOD","[T12][DVBC]unlock----\n"));
                    }
                    return FALSE;
                }
                break;

            case DMD_DVBC_GETLOCK_PSYNC_LOCK:
                u16Address =  FEC_REG_BASE + 0x2C; //FEC: P-sync Lock,
                cBitMask = BIT(1);
                break;

            case DMD_DVBC_GETLOCK_DCR_LOCK:
                u16Address =  TDP_REG_BASE + 0x45; //DCR Lock,
                cBitMask = BIT(0);
                break;

            case DMD_DVBC_GETLOCK_AGC_LOCK:
                u16Address =  TDP_REG_BASE + 0x2F; //AGC Lock,
                cBitMask = BIT(0);
                break;

            case DMD_DVBC_GETLOCK_NO_CHANNEL:
                u16Address =  TOP_REG_BASE + 0xC3; //no channel,
                cBitMask = BIT(2)|BIT(3)|BIT(4);
                #ifdef MS_DEBUG
                {
                    MS_U8 reg_frz=0, FSM=0;
                    MS_U16 u16Timer=0;
                    MDrv_SYS_DMD_VD_MBX_ReadReg(FEC_REG_BASE + 0xE0, &FSM);
                    MDrv_SYS_DMD_VD_MBX_WriteReg(TDF_REG_BASE + 0x16, 0x03);
                    MDrv_SYS_DMD_VD_MBX_ReadReg(TDF_REG_BASE + 0x03, &reg_frz);
                    MDrv_SYS_DMD_VD_MBX_WriteReg(TDF_REG_BASE + 0x03, reg_frz | 0x80);
                    MDrv_SYS_DMD_VD_MBX_ReadReg(TDF_REG_BASE + 0x19, &cData);
                    MDrv_SYS_DMD_VD_MBX_WriteReg(TDF_REG_BASE + 0x03, reg_frz);
                    MDrv_SYS_DMD_VD_MBX_ReadReg(0x20DF, &reg_frz);
                    u16Timer=(u16Timer<<8)+reg_frz;
                    MDrv_SYS_DMD_VD_MBX_ReadReg(0x20DE, &reg_frz);
                    u16Timer=(u16Timer<<8)+reg_frz;
                    ULOGD("DEMOD","DMD_DVBC_GETLOCK_NO_CHANNEL %d %d %x\n",FSM,u16Timer,cData);
                }
                #endif
                break;

            case DMD_DVBC_GETLOCK_ATV_DETECT:
                u16Address =  TOP_REG_BASE + 0xC4; //ATV detection,
                cBitMask = BIT(1); // check atv
                break;

            case DMD_DVBC_GETLOCK_TR_LOCK:
                #if 0 // 20111108 temporarily solution
                u16Address =  INNC_REG_BASE + 0x0A*2 + 1; //TR lock indicator,
                cBitMask = BIT(4);
                break;
                #endif
            case DMD_DVBC_GETLOCK_TR_EVER_LOCK:
                u16Address =  TOP_REG_BASE + 0xC4; //TR lock indicator,
                cBitMask = BIT(4);
                break;

            default:
                return FALSE;
        }

        if (MDrv_SYS_DMD_VD_MBX_ReadReg(u16Address, &cData) == FALSE)
            return FALSE;

        if ((cData & cBitMask) != 0)
        {
            return TRUE;
        }

        return FALSE;
    }

    return FALSE;
}

/****************************************************************************
  Subject:    To get the Post viterbi BER
  Function:   INTERN_DVBC_GetPostViterbiBer
  Parmeter:  Quility
  Return:       E_RESULT_SUCCESS
                   E_RESULT_FAILURE =>Read I2C fail, INTERN_DVBC_VIT_STATUS_NG
  Remark:     For the Performance issue, here we just return the Post Value.(Not BER)
                   We will not read the Period, and have the "/256/8"
*****************************************************************************/
MS_BOOL INTERN_DVBC_GetPostViterbiBer(MS_U32 *BitErr_reg, MS_U16 *BitErrPeriod_reg)
{
    MS_BOOL           status = true;
    MS_U8             reg = 0, reg_frz = 0;
    //MS_U16            BitErrPeriod;
    //MS_U32            BitErr;
    //MS_U16            PktErr;

    /////////// Post-Viterbi BER /////////////

    // bank 1f 0x03 [1:0] reg_bit_err_num_freeze
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x03, &reg_frz);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(BACKEND_REG_BASE+0x03, reg_frz|0x03);

    // bank 1f 0x46 [7:0] reg_bit_err_sblprd_7_0
    //             0x47 [15:8] reg_bit_err_sblprd_15_8
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x47, &reg);
    *BitErrPeriod_reg = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x46, &reg);
    *BitErrPeriod_reg = (*BitErrPeriod_reg << 8)|reg;

    // bank 1f 0x6a [7:0] reg_bit_err_num_7_0
    //             0x6b [15:8] reg_bit_err_num_15_8
    // bank 1f 0x6c [7:0] reg_bit_err_num_23_16
    //             0x6d [15:8] reg_bit_err_num_31_24
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x6d, &reg);
    *BitErr_reg = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x6c, &reg);
    *BitErr_reg = (*BitErr_reg << 8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x6b, &reg);
    *BitErr_reg = (*BitErr_reg << 8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x6a, &reg);
    *BitErr_reg = (*BitErr_reg << 8)|reg;


    //INTERN_DVBC_GetPacketErr(&PktErr);

    // bank 1f 0x03 [1:0] reg_bit_err_num_freeze
    reg_frz=reg_frz&(~0x03);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(BACKEND_REG_BASE+0x03, reg_frz);
/*
    if (BitErrPeriod == 0 )    //protect 0
        BitErrPeriod = 1;

    if (BitErr <=0 )
        *ber = 0.5f / ((float)BitErrPeriod*128*188*8);
    else
        *ber = (float)BitErr / ((float)BitErrPeriod*128*188*8);

    DBG_GET_SIGNAL_DVBC(ULOGD("DEMOD","INTERN_DVBC PostVitBER = %8.3e \n ", *ber));
*/
    return status;
}

/****************************************************************************
  Subject:    To get the Packet error
  Function:   INTERN_DVBC_GetPacketErr
  Parmeter:   pktErr
  Return:     E_RESULT_SUCCESS
                   E_RESULT_FAILURE =>Read I2C fail, INTERN_DVBT_VIT_STATUS_NG
  Remark:     For the Performance issue, here we just return the Post Value.(Not BER)
                   We will not read the Period, and have the "/256/8"
*****************************************************************************/
MS_BOOL INTERN_DVBC_GetPacketErr(MS_U16 *pktErr)
{
    MS_BOOL          status = true;
    MS_U8            reg = 0, reg_frz = 0;
    MS_U16           PktErr;

    // bank 1f 0x03 [1:0] reg_bit_err_num_freeze
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x03, &reg_frz);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(BACKEND_REG_BASE+0x03, reg_frz|0x03);

    // bank 1f 0x66 [7:0] reg_uncrt_pkt_num_7_0
    //             0x67 [15:8] reg_uncrt_pkt_num_15_8
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x67, &reg);
    PktErr = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x66, &reg);
    PktErr = (PktErr << 8)|reg;

    // bank 1f 0x03 [1:0] reg_bit_err_num_freeze
    reg_frz=reg_frz&(~0x03);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(BACKEND_REG_BASE+0x03, reg_frz);

    DBG_GET_SIGNAL_DVBC(ULOGD("DEMOD","INTERN_DVBC PktErr = %d \n ", (int)PktErr));

    *pktErr = PktErr;

    return status;
}


/****************************************************************************
  Subject:    Read the signal to noise ratio (SNR)
  Function:   INTERN_DVBC_GetSNR
  Parmeter:   None
  Return:     -1 mean I2C fail, otherwise I2C success then return SNR value
  Remark:
*****************************************************************************/
MS_BOOL INTERN_DVBC_GetSNR(MS_U16 *snr_reg)
{
    MS_BOOL status = true;
    MS_U8 u8Data = 0, reg_frz = 0;
    // MS_U8 freeze = 0;
    //MS_U16 noisepower = 0;

    //if (TRUE == INTERN_DVBC_GetLock(DMD_DVBC_GETLOCK_PSYNC_LOCK, 200.0f, -200.0f, 0) )
    if (TRUE == INTERN_DVBC_GetLock(DMD_DVBC_GETLOCK_PSYNC_LOCK, 200*10, -200*10, 0) )
    {
        // bank 2c 0x3d [0] reg_bit_err_num_freeze
        status &= MDrv_SYS_DMD_VD_MBX_ReadReg(EQE_REG_BASE+0x3d, &reg_frz);
        status &= MDrv_SYS_DMD_VD_MBX_WriteReg(EQE_REG_BASE+0x3d, reg_frz|0x01);

        // read vk
        status &= MDrv_SYS_DMD_VD_MBX_ReadReg(EQE2_REG_BASE + 0x6f, &u8Data);
        //noisepower = u8Data;
        *snr_reg = u8Data;
        status &= MDrv_SYS_DMD_VD_MBX_ReadReg(EQE2_REG_BASE + 0x6e, &u8Data);
        //noisepower = (noisepower<<8)|u8Data;
        *snr_reg = ((*snr_reg)<<8)|u8Data;

        // bank 2c 0x3d [0] reg_bit_err_num_freeze
        reg_frz=reg_frz&(~0x01);
        status &= MDrv_SYS_DMD_VD_MBX_WriteReg(EQE_REG_BASE+0x3d, reg_frz);

        //if(noisepower == 0x0000)
        //    noisepower = 0x0001;
        if(*snr_reg == 0x0000)
            *snr_reg = 0x0001;
/*
#ifdef MSOS_TYPE_LINUX
        *f_snr = 10.0f*log10f(65536.0f/(float)noisepower);
#else
        *f_snr = 10.0f*Log10Approx(65536.0f/(float)noisepower);
#endif
*/
    }
    else
    {
        *snr_reg = 0;
    }
    return status;


}

MS_BOOL INTERN_DVBC_GetIFAGC(MS_U8 *ifagc_reg, MS_U8 *ifagc_reg_lsb, MS_U16 *ifagc_err)
{
	MS_BOOL status = true;
	
	status = HAL_DMD_IFAGC_RegRead(ifagc_reg, ifagc_reg_lsb, ifagc_err);
	
	return status;
}

//waiting mark
#if(0)
MS_BOOL INTERN_DVBC_GetSignalStrength(MS_U16 *strength, const DMD_DVBC_InitData *sDMD_DVBC_InitData, MS_U8 u8SarValue)
{
    MS_BOOL status = true;
    float   ch_power_db=0.0f, ch_power_db_rel=0.0f;
    DMD_DVBC_MODULATION_TYPE Qam_mode;

    DBG_INTERN_DVBC_TIME(ULOGD("DEMOD","INTERN_DVBC_GetSignalStrength, t=%ld, RF level=%f, Table=%lx\n",MsOS_GetSystemTime(), fRFPowerDbm, (MS_U32)(sDMD_DVBC_InitData->pTuner_RfagcSsi)));

    // if (INTERN_DVBC_Lock(COFDM_TPS_LOCK))
        //if (INTERN_DVBC_Lock(COFDM_AGC_LOCK))
        /* Actually, it's more reasonable, that signal level depended on cable input power level
        * thougth the signal isn't dvb-t signal.
        */
    // use pointer of IFAGC table to identify
    // case 1: RFAGC from SAR, IFAGC controlled by demod
    // case 2: RFAGC from tuner, ,IFAGC controlled by demod
    status=HAL_DMD_GetRFLevel(&ch_power_db, fRFPowerDbm, u8SarValue,
                                                                sDMD_DVBC_InitData->pTuner_RfagcSsi, sDMD_DVBC_InitData->u16Tuner_RfagcSsi_Size,
                                                                sDMD_DVBC_InitData->pTuner_IfagcSsi_HiRef, sDMD_DVBC_InitData->u16Tuner_IfagcSsi_HiRef_Size,
                                                                sDMD_DVBC_InitData->pTuner_IfagcSsi_LoRef, sDMD_DVBC_InitData->u16Tuner_IfagcSsi_LoRef_Size,
                                                                sDMD_DVBC_InitData->pTuner_IfagcErr_HiRef, sDMD_DVBC_InitData->u16Tuner_IfagcErr_HiRef_Size,
                                                                sDMD_DVBC_InitData->pTuner_IfagcErr_LoRef, sDMD_DVBC_InitData->u16Tuner_IfagcErr_LoRef_Size);

    status &= INTERN_DVBC_GetCurrentModulationType(&Qam_mode);

    if( (MS_U8)Qam_mode <= (MS_U8)DMD_DVBC_QAM256)
    {
        ch_power_db_rel = ch_power_db + intern_dvb_c_qam_ref[(MS_U8)Qam_mode];
    }
    else
    {
        ch_power_db_rel = -100.0f;
    }

    if(ch_power_db_rel <= -85.0f)
        {*strength = 0;}
    else if (ch_power_db_rel <= -80.0f)
        {*strength = (MS_U16)(0.0f + (ch_power_db_rel+85.0f)*10.0f/5.0f);}
    else if (ch_power_db_rel <= -75.0f)
        {*strength = (MS_U16)(10.0f + (ch_power_db_rel+80.0f)*20.0f/5.0f);}
    else if (ch_power_db_rel <= -70.0f)
        {*strength = (MS_U16)(30.0f + (ch_power_db_rel+75.0f)*30.0f/5.0f);}
    else if (ch_power_db_rel <= -65.0f)
        {*strength = (MS_U16)(60.0f + (ch_power_db_rel+70.0f)*10.0f/5.0f);}
    else if (ch_power_db_rel <= -55.0f)
        {*strength = (MS_U16)(70.0f + (ch_power_db_rel+65.0f)*20.0f/10.0f);}
    else if (ch_power_db_rel <= -45.0f)
        {*strength = (MS_U16)(90.0f + (ch_power_db_rel+55.0f)*10.0f/10.0f);}
    else
        {*strength = 100;}

    DBG_GET_SIGNAL_DVBC(ULOGD("DEMOD",">>> SSI_CH_PWR(dB) = %f , Score = %d<<<\n", ch_power_db, *strength));
    DBG_GET_SIGNAL_DVBC(ULOGD("DEMOD",">>> SSI = %d <<<\n", (int)*strength));

    return status;
}
#endif

/****************************************************************************
  Subject:    To get the DVT Signal quility
  Function:   INTERN_DVBC_GetSignalQuality
  Parmeter:  Quility
  Return:      E_RESULT_SUCCESS
                   E_RESULT_FAILURE
  Remark:    Here we have 4 level range
                  <1>.First Range => Quility =100  (You can define it by INTERN_DVBT_SIGNAL_BASE_100)
                  <2>.2th Range => 60 < Quality < 100 (You can define it by INTERN_DVBT_SIGNAL_BASE_60)
                  <3>.3th Range => 10 < Quality < 60  (You can define it by INTERN_DVBT_SIGNAL_BASE_10)
                  <4>.4th Range => Quality <10
*****************************************************************************/
//waiting mark
/*
MS_BOOL INTERN_DVBC_GetSignalQuality(MS_U16 *quality, const DMD_DVBC_InitData *sDMD_DVBC_InitData, MS_U8 u8SarValue)
{

    float       fber;
    float       log_ber;
    MS_BOOL status = true;
    DMD_DVBC_MODULATION_TYPE Qam_mode;
    float f_snr;

    fRFPowerDbm = fRFPowerDbm;
    status &= INTERN_DVBC_GetSNR(&f_snr);
    if (TRUE == INTERN_DVBC_GetLock(DMD_DVBC_GETLOCK_PSYNC_LOCK, 200.0f, -200.0f, 0))
    {
        if (INTERN_DVBC_GetPostViterbiBer(&fber) == FALSE)
        {
            DBG_INTERN_DVBC(ULOGD("DEMOD","\nGetPostViterbiBer Fail!"));
            return FALSE;
        }

        // log_ber = log10(fber)
        log_ber = (-1.0f)*Log10Approx(1.0f/fber); // Log10Approx() provide 1~2^32 input range only

        DBG_INTERN_DVBC(ULOGD("DEMOD","\nLog(BER) = %f",log_ber));
        status &= INTERN_DVBC_GetCurrentModulationType(&Qam_mode);
        if (Qam_mode == DMD_DVBC_QAM16)
        {
            if(log_ber  <= (-5.5f))
                *quality = 100;
            else if(log_ber  <= (-5.1f))
                *quality = (MS_U16)(90.0f + ((-5.1f)-log_ber)*10.0f/((-5.1f)-(-5.5f)));
            else if(log_ber  <= (-4.9f))
                *quality = (MS_U16)(80.0f + ((-4.9f)-log_ber)*10.0f/((-4.9f)-(-5.1f)));
            else if(log_ber  <= (-4.5f))
                *quality = (MS_U16)(70.0f + ((-4.5f)-log_ber)*10.0f/((-4.5f)-(-4.9f)));
            else if(log_ber  <= (-3.7f))
                *quality = (MS_U16)(60.0f + ((-3.7f)-log_ber)*10.0f/((-3.7f)-(-4.5f)));
            else if(log_ber  <= (-3.2f))
                *quality = (MS_U16)(50.0f + ((-3.2f)-log_ber)*10.0f/((-3.2f)-(-3.7f)));
            else if(log_ber  <= (-2.9f))
                *quality = (MS_U16)(40.0f + ((-2.9f)-log_ber)*10.0f/((-2.9f)-(-3.2f)));
            else if(log_ber  <= (-2.5f))
                *quality = (MS_U16)(30.0f + ((-2.5f)-log_ber)*10.0f/((-2.5f)-(-2.9f)));
            else if(log_ber  <= (-2.2f))
                *quality = (MS_U16)(20.0f + ((-2.2f)-log_ber)*10.0f/((-2.2f)-(-2.5f)));
            else if(log_ber  <= (-2.0f))
                *quality = (MS_U16)(0.0f + ((-2.0f)-log_ber)*10.0f/((-2.0f)-(-2.2f)));
            else
                *quality = 0;
        }
        else if (Qam_mode == DMD_DVBC_QAM32)
        {
            if(log_ber  <= (-5.0f))
                *quality = 100;
            else if(log_ber  <= (-4.7f))
                *quality = (MS_U16)(90.0f  + ((-4.7f)-log_ber)*10.0f/((-4.7f)-(-5.0f)));
            else if(log_ber  <= (-4.5f))
                *quality = (MS_U16)(80.0f  + ((-4.5f)-log_ber)*10.0f/((-4.5f)-(-4.7f)));
            else if(log_ber  <= (-3.8f))
                *quality = (MS_U16)(70.0f  + ((-3.8f)-log_ber)*10.0f/((-3.8f)-(-4.5f)));
            else if(log_ber  <= (-3.5f))
                *quality = (MS_U16)(60.0f  + ((-3.5f)-log_ber)*10.0f/((-3.5f)-(-3.8f)));
            else if(log_ber  <= (-3.0f))
                *quality = (MS_U16)(50.0f  + ((-3.0f)-log_ber)*10.0f/((-3.0f)-(-3.5f)));
            else if(log_ber  <= (-2.7f))
                *quality = (MS_U16)(40.0f  + ((-2.7f)-log_ber)*10.0f/((-2.7f)-(-3.0f)));
            else if(log_ber  <= (-2.4f))
                *quality = (MS_U16)(30.0f  + ((-2.4f)-log_ber)*10.0f/((-2.4f)-(-2.7f)));
            else if(log_ber  <= (-2.2f))
                *quality = (MS_U16)(20.0f  + ((-2.2f)-log_ber)*10.0f/((-2.2f)-(-2.4f)));
            else if(log_ber  <= (-2.0f))
                *quality = (MS_U16)(0.0f  + ((-2.0f)-log_ber)*10.0f/((-2.0f)-(-2.2f)));
            else
                *quality = 0;
        }
        else if (Qam_mode == DMD_DVBC_QAM64)
        {
            if(log_ber  <= (-5.4f))
                *quality = 100;
            else if(log_ber  <= (-5.1f))
                *quality = (MS_U16)(90.0f + ((-5.1f)-log_ber)*10.0f/((-5.1f)-(-5.4f)));
            else if(log_ber  <= (-4.9f))
                *quality = (MS_U16)(80.0f + ((-4.9f)-log_ber)*10.0f/((-4.9f)-(-5.1f)));
            else if(log_ber  <= (-4.3f))
                *quality = (MS_U16)(70.0f + ((-4.3f)-log_ber)*10.0f/((-4.3f)-(-4.9f)));
            else if(log_ber  <= (-3.7f))
                *quality = (MS_U16)(60.0f + ((-3.7f)-log_ber)*10.0f/((-3.7f)-(-4.3f)));
            else if(log_ber  <= (-3.2f))
                *quality = (MS_U16)(50.0f + ((-3.2f)-log_ber)*10.0f/((-3.2f)-(-3.7f)));
            else if(log_ber  <= (-2.9f))
                *quality = (MS_U16)(40.0f + ((-2.9f)-log_ber)*10.0f/((-2.9f)-(-3.2f)));
            else if(log_ber  <= (-2.4f))
                *quality = (MS_U16)(30.0f + ((-2.4f)-log_ber)*10.0f/((-2.4f)-(-2.9f)));
            else if(log_ber  <= (-2.2f))
                *quality = (MS_U16)(20.0f + ((-2.2f)-log_ber)*10.0f/((-2.2f)-(-2.4f)));
            else if(log_ber  <= (-2.05f))
                *quality = (MS_U16)(0.0f + ((-2.05f)-log_ber)*10.0f/((-2.05f)-(-2.2f)));
            else
                *quality = 0;
        }
        else if (Qam_mode == DMD_DVBC_QAM128)
        {
            if(log_ber  <= (-5.1f))
            *quality = 100;
            else if(log_ber  <= (-4.9f))
            *quality = (MS_U16)(90.0f + ((-4.9f)-log_ber)*10.0f/((-4.9f)-(-5.1f)));
            else if(log_ber  <= (-4.7f))
            *quality = (MS_U16)(80.0f + ((-4.7f)-log_ber)*10.0f/((-4.7f)-(-4.9f)));
            else if(log_ber  <= (-4.1f))
            *quality = (MS_U16)(70.0f + ((-4.1f)-log_ber)*10.0f/((-4.1f)-(-4.7f)));
            else if(log_ber  <= (-3.5f))
            *quality = (MS_U16)(60.0f + ((-3.5f)-log_ber)*10.0f/((-3.5f)-(-4.1f)));
            else if(log_ber  <= (-3.1f))
            *quality = (MS_U16)(50.0f + ((-3.1f)-log_ber)*10.0f/((-3.1f)-(-3.5f)));
            else if(log_ber  <= (-2.7f))
            *quality = (MS_U16)(40.0f + ((-2.7f)-log_ber)*10.0f/((-2.7f)-(-3.1f)));
            else if(log_ber  <= (-2.5f))
            *quality = (MS_U16)(30.0f + ((-2.5f)-log_ber)*10.0f/((-2.5f)-(-2.7f)));
            else if(log_ber  <= (-2.06f))
            *quality = (MS_U16)(20.0f + ((-2.06f)-log_ber)*10.0f/((-2.06f)-(-2.5f)));
        //else if(log_ber  <= (-2.05))
        else
        {
            if (f_snr >= 27.2f)
            *quality = 20;
            else if (f_snr >= 25.1f)
            *quality = (MS_U16)(0.0f + (f_snr - 25.1f)*20.0f/(27.2f-25.1f));
            else
            *quality = 0;
        }
        }
        else //256QAM
        {
            if(log_ber  <= (-4.8f))
                *quality = 100;
            else if(log_ber  <= (-4.6f))
                *quality = (MS_U16)(90.0f + ((-4.6f)-log_ber)*10.0f/((-4.6f)-(-4.8f)));
            else if(log_ber  <= (-4.4f))
                *quality = (MS_U16)(80.0f + ((-4.4f)-log_ber)*10.0f/((-4.4f)-(-4.6f)));
            else if(log_ber  <= (-4.0f))
                *quality = (MS_U16)(70.0f + ((-4.0f)-log_ber)*10.0f/((-4.0f)-(-4.4f)));
            else if(log_ber  <= (-3.5f))
                *quality = (MS_U16)(60.0f + ((-3.5f)-log_ber)*10.0f/((-3.5f)-(-4.0f)));
            else if(log_ber  <= (-3.1f))
                *quality = (MS_U16)(50.0f + ((-3.1f)-log_ber)*10.0f/((-3.1f)-(-3.5f)));
            else if(log_ber  <= (-2.7f))
                *quality = (MS_U16)(40.0f + ((-2.7f)-log_ber)*10.0f/((-2.7f)-(-3.1f)));
            else if(log_ber  <= (-2.4f))
                *quality = (MS_U16)(30.0f + ((-2.4f)-log_ber)*10.0f/((-2.4f)-(-2.7f)));
            else if(log_ber  <= (-2.06f))
                *quality = (MS_U16)(20.0f + ((-2.06f)-log_ber)*10.0f/((-2.06f)-(-2.4f)));
        //else if(log_ber  <= (-2.05))
        else
        {
            if (f_snr >= 29.6f)
                *quality = 20;
            else if (f_snr >= 27.3f)
                *quality = (MS_U16)(0.0f + (f_snr - 27.3f)*20.0f/(29.6f-27.3f));
            else
                *quality = 0;
        }
        }
    }
    else
    {
        *quality = 0;
    }

    //DBG_GET_SIGNAL_DVBC(ULOGD("DEMOD","SNR = %f, QAM = %d, code Rate = %d\n", cn_rec, tps_cnstl, tps_cr));
    DBG_GET_SIGNAL_DVBC(ULOGD("DEMOD","BER = %8.3e\n", fber));
    DBG_GET_SIGNAL_DVBC(ULOGD("DEMOD","Signal Quility = %d\n", *quality));
    return TRUE;
}
#endif
*/

/****************************************************************************
  Subject:    To get the Cell ID
  Function:   INTERN_DVBC_Get_CELL_ID
  Parmeter:   point to return parameter cell_id

  Return:     TRUE
              FALSE
  Remark:
*****************************************************************************/
MS_BOOL INTERN_DVBC_Get_CELL_ID(MS_U16 *cell_id)
{
  MS_BOOL status = true;
  MS_U8 value1 = 0;
  MS_U8 value2 = 0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE+0x2B, &value2); //TPS Cell ID [15:8]
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE+0x2A, &value1); //TPS Cell ID [7:0]

    *cell_id = ((MS_U16)value1<<8)|value2;
    return status;
}

/****************************************************************************
  Subject:    To get the DVBC Carrier Freq Offset
  Function:   INTERN_DVBC_Get_FreqOffset
  Parmeter:   Frequency offset (in KHz), bandwidth
  Return:     E_RESULT_SUCCESS
              E_RESULT_FAILURE
  Remark:
*****************************************************************************/
#if(1)
MS_BOOL INTERN_DVBC_Get_FreqOffset(MS_U32 *config_Fc_reg, MS_U32 *Fc_over_Fs_reg, MS_U16 *Cfo_offset_reg, MS_U8 u8BW)
{
    MS_U8       reg_frz = 0, reg = 0;
    MS_BOOL     status = TRUE;

    // no use.
    u8BW = u8BW;

    DBG_INTERN_DVBC(ULOGD("DEMOD","INTERN_DVBC_Get_FreqOffset\n"));

    // bank 2c 0x3d [0] reg_bit_err_num_freeze
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(EQE_REG_BASE+0x3d, &reg_frz);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(EQE_REG_BASE+0x3d, reg_frz|0x01);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(EQE2_REG_BASE + 0x75, &reg);
    *config_Fc_reg = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(EQE2_REG_BASE + 0x74, &reg);
    *config_Fc_reg = (*config_Fc_reg<<8)|reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(EQE2_REG_BASE + 0x73, &reg);
    *config_Fc_reg = (*config_Fc_reg<<8)|reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(EQE2_REG_BASE + 0x72, &reg);
    *config_Fc_reg = (*config_Fc_reg<<8)|reg;

    // bank 2c 0x3d [0] reg_bit_err_num_freeze
    reg_frz=reg_frz&(~0x01);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(EQE_REG_BASE+0x3d, reg_frz);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDF_REG_BASE + 0x5b, &reg);
    *Fc_over_Fs_reg = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDF_REG_BASE + 0x5a, &reg);
    *Fc_over_Fs_reg = (*Fc_over_Fs_reg<<8)|reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDF_REG_BASE + 0x59, &reg);
    *Fc_over_Fs_reg = (*Fc_over_Fs_reg<<8)|reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDF_REG_BASE + 0x58, &reg);
    *Fc_over_Fs_reg = (*Fc_over_Fs_reg<<8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_DVBC_CFG_FC_H, &reg);
    *Cfo_offset_reg = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_DVBC_CFG_FC_L, &reg);
    *Cfo_offset_reg = (*Cfo_offset_reg<<8)|reg;
    
    //waiting mark
    /*
    f_Fc = (float)Reg_Fc_over_Fs/134217728.0f * 45473.0f;

    FreqCfo_offset = (MS_S32)(RegCfo_offset<<4)/16;

    FreqCfo_offset = FreqCfo_offset/0x8000000/8.0f;

    status &= INTERN_DVBC_GetCurrentSymbolRate(&FreqB);

    FreqCfo_offset = FreqCfo_offset * FreqB - (f_Fc-(float)config_Fc);
    DBG_INTERN_DVBC_LOCK(ULOGD("DEMOD","[dvbc]Freq_Offset = %f KHz, Reg_offset = 0x%lx, Reg_Fc_over_Fs=0x%lx, SR = %d KS/s, Fc = %f %d\n",
                            FreqCfo_offset,RegCfo_offset,Reg_Fc_over_Fs,FreqB,f_Fc,config_Fc));

    *pFreqOff = FreqCfo_offset;
    */
    return status;
}
#endif


void INTERN_DVBC_Power_ON_OFF(MS_U8 bPowerOn)
{

    bPowerOn = bPowerOn;
}

MS_BOOL INTERN_DVBC_Power_Save(void)
{

    return TRUE;
}

/****************************************************************************
  Subject:    To get the current modulation type at the DVB-C Demod
  Function:   INTERN_DVBC_GetCurrentModulationType
  Parmeter:   pointer for return QAM type

  Return:     TRUE
              FALSE
  Remark:
*****************************************************************************/
MS_BOOL INTERN_DVBC_GetCurrentModulationType(DMD_DVBC_MODULATION_TYPE *pQAMMode)
{
    MS_U8 u8Data=0;

    DBG_INTERN_DVBC(ULOGD("DEMOD","INTERN_DVBC_GetCurrentModulationType\n"));


    MDrv_SYS_DMD_VD_MBX_ReadReg(0x9cc4, &u8Data);



  	//ULOGD("DEMOD","@@@@@@ 0x9cc4 pQAMMode = %d \n",u8Data&0x07);

    switch(u8Data&0x07)
    {
        case 0:
            *pQAMMode = DMD_DVBC_QAM16;
            DBG_INTERN_DVBC_LOCK(ULOGD("DEMOD","[dvbc]QAM=16\n"));
            return TRUE;
             break;
        case 1:
            *pQAMMode = DMD_DVBC_QAM32;
            DBG_INTERN_DVBC_LOCK(ULOGD("DEMOD","[dvbc]QAM=32\n"));
            return TRUE;
            break;
        case 2:
            *pQAMMode = DMD_DVBC_QAM64;
            DBG_INTERN_DVBC_LOCK(ULOGD("DEMOD","[dvbc]QAM=64\n"));
            return TRUE;
            break;
        case 3:
            *pQAMMode = DMD_DVBC_QAM128;
            DBG_INTERN_DVBC_LOCK(ULOGD("DEMOD","[dvbc]QAM=128\n"));
            return TRUE;
            break;
        case 4:
            *pQAMMode = DMD_DVBC_QAM256;
            DBG_INTERN_DVBC_LOCK(ULOGD("DEMOD","[dvbc]QAM=256\n"));
            return TRUE;
            break;
        default:
            *pQAMMode = DMD_DVBC_QAMAUTO;
            DBG_INTERN_DVBC_LOCK(ULOGD("DEMOD","[dvbc]QAM=invalid\n"));
            return FALSE;
    }
}

/****************************************************************************
  Subject:    To get the current symbol rate at the DVB-C Demod
  Function:   INTERN_DVBC_GetCurrentSymbolRate
  Parmeter:   pointer pData for return Symbolrate

  Return:     TRUE
              FALSE
  Remark:
*****************************************************************************/
MS_BOOL INTERN_DVBC_GetCurrentSymbolRate(MS_U16 *u16SymbolRate)
{
    MS_U8  tmp = 0;
    MS_U16 u16SymbolRateTmp = 0;
    
    DBG_INTERN_DVBC(ULOGD("DEMOD","INTERN_DVBC_GetCurrentSymbolRate\n"));


    // intp
    MDrv_SYS_DMD_VD_MBX_ReadReg(0x20d2, &tmp);
    u16SymbolRateTmp = tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(0x20d1, &tmp);
    u16SymbolRateTmp = (u16SymbolRateTmp<<8)|tmp;

    if (abs(u16SymbolRateTmp-6900)<2)
    {
        u16SymbolRateTmp=6900;
    }

    if (abs(u16SymbolRateTmp-6875)<2)
    {
        u16SymbolRateTmp=6875;
    }

    *u16SymbolRate = u16SymbolRateTmp;

    DBG_INTERN_DVBC_LOCK(ULOGD("DEMOD","[dvbc]SR=%d\n",*u16SymbolRate));


    return TRUE;
}


/****************************************************************************
  Subject:    To get the current symbol rate offset at the DVB-C Demod
  Function:   INTERN_DVBC_GetCurrentSymbolRate
  Parmeter:   pointer pData for return Symbolrate offset

  Return:     TRUE
              FALSE
  Remark:
*****************************************************************************/
//waiting mark
/*
MS_BOOL INTERN_DVBC_GetCurrentSymbolRateOffset(MS_U16 *pData)
{
    MS_U8   u8Data = 0, reg_frz = 0;
    MS_U32  u32Data = 0;
    // MS_S32  s32Data = 0;
    MS_BOOL status = TRUE;
    MS_U16  u16SymbolRate = 0;
    float   f_symb_offset = 0.0f;



    // bank 26 0x03 [7] reg_bit_err_num_freeze
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(INNC_REG_BASE+0x03, &reg_frz);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(INNC_REG_BASE+0x03, reg_frz|0x80);

    // sel, SFO debug output.
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(INNC_REG_BASE+0x2F, &u8Data);
    u32Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(INNC_REG_BASE+0x2E, &u8Data);
    u32Data = (u32Data<<8)|u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(INNC_REG_BASE+0x2D, &u8Data);
    u32Data = (u32Data<<8)|u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(INNC_REG_BASE+0x2C, &u8Data);
    u32Data = (u32Data<<8)|u8Data;

    // bank 26 0x03 [7] reg_bit_err_num_freeze
    reg_frz=reg_frz&(~0x80);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(INNC_REG_BASE+0x03, reg_frz);
    // s32Data = (MS_S32)(u32Data<<8);

    ULOGD("DEMOD","[dvbc]u32_symb_offset = 0x%x\n",(unsigned int)u32Data);

    status &= INTERN_DVBC_GetCurrentSymbolRate(&u16SymbolRate);

    // sfo = Reg*2^(-37)*FB/FS*1000000 (2^-28 * 1000000 = 0.003725)
//    f_symb_offset = (float)((MS_S32)u32Data) * (1000000.0f/powf(2.0f, 37.0f)) * (float)u16SymbolRate/(float)DVBC_FS;
    f_symb_offset = (float)((MS_S32)u32Data) * (0.000007276f) * (float)u16SymbolRate/(float)DVBC_FS;

    *pData = (MS_U16)(f_symb_offset + 0.5f);

    DBG_INTERN_DVBC_LOCK(ULOGD("DEMOD","[dvbc]sfo_offset = %d,%f\n",*pData, f_symb_offset));

    return status;
}
#endif
*/

MS_BOOL INTERN_DVBC_Version(MS_U16 *ver)
{

    MS_U8 status = true;
    MS_U8 tmp = 0;
    MS_U16 u16_INTERN_DVBC_Version;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0xC1, &tmp);
    u16_INTERN_DVBC_Version = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0xC2, &tmp);
    u16_INTERN_DVBC_Version = u16_INTERN_DVBC_Version<<8|tmp;
    *ver = u16_INTERN_DVBC_Version;

    return status;
}


MS_BOOL INTERN_DVBC_Show_Demod_Version(void)
{

    MS_BOOL status = true;
    MS_U16 u16_INTERN_DVBC_Version;

    status &= INTERN_DVBC_Version(&u16_INTERN_DVBC_Version);

    ULOGD("DEMOD","[DVBC]Version = %x\n",u16_INTERN_DVBC_Version);

    return status;
}



#if (INTERN_DVBC_INTERNAL_DEBUG)

MS_BOOL INTERN_DVBC_Show_AGC_Info(void)
{
    MS_U8 tmp = 0;
    MS_U8 agc_k = 0,agc_ref = 0,d1_k = 0,d1_ref = 0,d2_k = 0,d2_ref = 0;
    MS_U16 if_agc_gain = 0,d1_gain = 0,d2_gain = 0;
    MS_U16 if_agc_err = 0;
    MS_BOOL status = TRUE;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x11,&agc_k);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x13,&agc_ref);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0xB0,&d1_k);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0xB1,&d1_ref);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0xC0,&d2_k);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0xC1,&d2_ref);


    // select IF gain to read
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x22, &tmp);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDP_REG_BASE + 0x22, (tmp&0xF0)|0x03);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x25, &tmp);
    if_agc_gain = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x24, &tmp);
    if_agc_gain = (if_agc_gain<<8)|tmp;


    // select d1 gain to read.
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0xb6, &tmp);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDP_REG_BASE + 0xb6, (tmp&0xF0)|0x02);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0xb9, &tmp);
    d1_gain = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0xb8, &tmp);
    d1_gain = (d1_gain<<8)|tmp;

    // select d2 gain to read.
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0xc6, &tmp);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDP_REG_BASE + 0xc6, (tmp&0xF0)|0x02);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0xc9, &tmp);
    d2_gain = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0xc8, &tmp);
    d2_gain = (d2_gain<<8)|tmp;

    // select IF gain err to read
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x22, &tmp);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDP_REG_BASE + 0x22, (tmp&0xF0)|0x00);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x25, &tmp);
    if_agc_err = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x24, &tmp);
    if_agc_err = (if_agc_err<<8)|tmp;

    ULOGD("DEMOD","[dvbc]agc_k=0x%x, agc_ref=0x%x, d1_k=0x%x, d1_ref=0x%x, d2_k=0x%x, d2_ref=0x%x\n",
        agc_k,agc_ref,d1_k,d1_ref,d2_k,d2_ref);

    ULOGD("DEMOD","[dvbc]agc_g=0x%x, d1_g=0x%x, d2_g=0x%x, agc_err=0x%x\n",if_agc_gain,d1_gain,d2_gain,if_agc_err);

    return status;
}

void INTERN_DVBC_info(void)
{
    MS_U32 fb_fs = 0, fc_fs = 0, tr_error = 0, crv = 0, intp = 0;
    MS_U8 qam,tmp = 0;
    MS_U8 fft_u8 = 0;
    MS_U16 fft_u16bw = 0;
    MS_U16 version = 0,packetErr = 0,quality = 0,symb_rate = 0,symb_offset = 0;
    //float f_snr = 0,f_freq = 0;
    //DMD_DVBC_MODULATION_TYPE QAMMode = 0;
    MS_U16 f_start = 0,f_end = 0;
    MS_U8  s0_count = 0;
    MS_U8  sc4 = 0,sc3 = 0;
    MS_U8  kp0, kp1, kp2, kp3,kp4, fmax, era_th;
    MS_U16 aci_e0,aci_e1,aci_e2,aci_e3;
    MS_U16 count = 0;
    MS_U16 fb_i_1,fb_q_1;
    MS_U8  e0,e1,e2,e3;
    MS_S16 reg_freq;
    //float freq,mag;



    INTERN_DVBC_Version(&version);

    // fb_fs
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x53, &tmp);
    fb_fs = tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x52, &tmp);
    fb_fs = (fb_fs<<8)|tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x51, &tmp);
    fb_fs = (fb_fs<<8)|tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x50, &tmp);
    fb_fs = (fb_fs<<8)|tmp;
    // fc_fs
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x57, &tmp);
    fc_fs = tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x56, &tmp);
    fc_fs = (fc_fs<<8)|tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x55, &tmp);
    fc_fs = (fc_fs<<8)|tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x54, &tmp);
    fc_fs = (fc_fs<<8)|tmp;
    // crv
    MDrv_SYS_DMD_VD_MBX_ReadReg(EQE_REG_BASE + 0x43, &tmp);
    crv = tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(EQE_REG_BASE + 0x42, &tmp);
    crv = (crv<<8)|tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(EQE_REG_BASE + 0x41, &tmp);
    crv = (crv<<8)|tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(EQE_REG_BASE + 0x40, &tmp);
    crv = (crv<<8)|tmp;
    // tr_error
    MDrv_SYS_DMD_VD_MBX_ReadReg(INNC_REG_BASE + 0x4E, &tmp);
    tr_error = tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(INNC_REG_BASE + 0x4D, &tmp);
    tr_error = (tr_error<<8)|tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(INNC_REG_BASE + 0x4C, &tmp);
    tr_error = (tr_error<<8)|tmp;

    // intp
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0xD3, &tmp);
    intp = tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0xD2, &tmp);
    intp = (intp<<8)|tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0xD1, &tmp);
    intp = (intp<<8)|tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0xD0, &tmp);
    intp = (intp<<8)|tmp;

    //waiting mark
    // fft info
    // intp
    /*
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDE_REG_BASE + 0x93, &tmp);
    fft_u16bw = tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDE_REG_BASE + 0x92, &tmp);
    fft_u16bw = (fft_u16bw<<8)|tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDE_REG_BASE + 0x96, &tmp);
    fft_u8 = tmp;
    */

    MDrv_SYS_DMD_VD_MBX_ReadReg(EQE_REG_BASE + 0x02, &tmp);
    qam = tmp;

    MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0xE0, &tmp);
    f_start = tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0xE1, &tmp);
    f_start = (f_start<<8)|tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0xE2, &tmp);
    f_end = tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0xE3, &tmp);
    f_end = (f_end<<8)|tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0xE4, &tmp);
    s0_count = tmp;

    MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0xC3, &sc3);
    MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0xC4, &sc4);

    MDrv_SYS_DMD_VD_MBX_ReadReg(EQE_REG_BASE + 0x04, &kp0);
    MDrv_SYS_DMD_VD_MBX_ReadReg(EQE_REG_BASE + 0x05, &kp1);
    MDrv_SYS_DMD_VD_MBX_ReadReg(EQE_REG_BASE + 0x06, &kp2);
    MDrv_SYS_DMD_VD_MBX_ReadReg(EQE_REG_BASE + 0x07, &kp3);
    MDrv_SYS_DMD_VD_MBX_ReadReg(EQE_REG_BASE + 0x08, &kp4);
    MDrv_SYS_DMD_VD_MBX_ReadReg(EQE_REG_BASE + 0x0B, &fmax);
    MDrv_SYS_DMD_VD_MBX_ReadReg(EQE_REG_BASE + 0x49, &era_th);


    MDrv_SYS_DMD_VD_MBX_WriteReg(TDP_REG_BASE + 0x81, 0x00);

    count = 0x400;
    while(count--);

    MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x85, &tmp);
    aci_e0 = tmp&0x0f;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x84, &tmp);
    aci_e0 = aci_e0<<8|tmp;

    MDrv_SYS_DMD_VD_MBX_WriteReg(TDP_REG_BASE + 0x81, 0x01);

    count = 0x400;
    while(count--);


    MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x85, &tmp);
    aci_e1 = tmp&0x0f;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x84, &tmp);
    aci_e1 = aci_e1<<8|tmp;

    MDrv_SYS_DMD_VD_MBX_WriteReg(TDP_REG_BASE + 0x81, 0x02);

    count = 0x400;
    while(count--);

    MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x85, &tmp);
    aci_e2 = tmp&0x0f;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x84, &tmp);
    aci_e2 = aci_e2<<8|tmp;

    // read aci coef
    MDrv_SYS_DMD_VD_MBX_WriteReg(TDP_REG_BASE + 0x81, 0x03);

    count = 0x400;
    while(count--);

    MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x85, &tmp);
    aci_e3 = tmp&0x0f;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x84, &tmp);
    aci_e3 = aci_e3<<8|tmp;

    //waiting mark
    /*
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDE_REG_BASE + 0x13, &tmp);
    fb_i_1 = tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDE_REG_BASE + 0x12, &tmp);
    fb_i_1 = fb_i_1<<8|tmp;

    MDrv_SYS_DMD_VD_MBX_ReadReg(TDE_REG_BASE + 0x15, &tmp);
    fb_q_1 = tmp;
    MDrv_SYS_DMD_VD_MBX_ReadReg(TDE_REG_BASE + 0x14, &tmp);
    fb_q_1 = fb_q_1<<8|tmp;
    */

    MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0xE0, &e0);
    MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0xE1, &e1);
    MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0xE2, &e2);
    MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0xE3, &e3);

    //reg_freq = (MS_S16)((MS_U16)e1)<<8|e0;
    //freq = (float)reg_freq*45473.0/65536.0;
    //mag = (float)(((MS_U16)e3)<<8|e2)/65536.0;


    INTERN_DVBC_GetPacketErr(&packetErr);
    //INTERN_DVBC_GetSNR(&f_snr);
    INTERN_DVBC_Show_AGC_Info();
    //INTERN_DVBC_GetSignalQuality(&quality,NULL,0, 200.0f);
    //INTERN_DVBC_Get_FreqOffset(&f_freq,8);                        //GetStatus
    //INTERN_DVBC_GetCurrentSymbolRate(&symb_rate);                 //GetStatus
    //INTERN_DVBC_GetCurrentSymbolRateOffset(&symb_offset);
    //INTERN_DVBC_GetCurrentModulationType(&QAMMode);               //GetStatus
/*
    ULOGD("DEMOD","[MStar_1][1]0x%x,[2]0x%lx,[3]0x%lx,[4]0x%lx,[5]0x%lx,[6]0x%x,[7]%d\n",version,fb_fs,fc_fs,tr_error,crv,qam,packetErr);
    //ULOGD("DEMOD","[MStar_2][1]%f,[2]0x%lx,[3]%d,[4]%f,[5]%d,[6]%d,[7]%d\n",f_snr,intp,quality,f_freq,symb_rate,symb_offset,packetErr);
    ULOGD("DEMOD","[MStar_2][2]0x%lx\n",intp);
    ULOGD("DEMOD","[Mstar_3][1]0x%x,[2]0x%x,[3]0x%x,[4]0x%x,[5]%d,[6]0x%x,[7]0x%x\n",fft_u16bw,fft_u8,f_end,f_start,s0_count,sc3,sc4);
    ULOGD("DEMOD","[Mstar_4][1]0x%x,[2]0x%x,[3]0x%x,[4]0x%x,[5]0x%x,[6]0x%x,[7]0x%x\n",kp0,kp1,kp2,kp3,kp4,fmax,era_th);
    ULOGD("DEMOD","[Mstar_5][1]0x%x,[2]0x%x,[3]0x%x,[4]0x%x,[5]0x%x,[6]0x%x,[7]0x%x\n",aci_e0,aci_e1,aci_e2,aci_e3,fb_i_1,fb_q_1,era_th);
    //ULOGD("DEMOD","[Mstar_6][1]%f,[2]%f,[3]0x%x,[4]0x%x,[5]0x%x,[6]0x%x,[7]0x%x\n",freq,mag,aci_e2,aci_e3,fb_i_1,fb_q_1,era_th);
    ULOGD("DEMOD","[Mstar_6][3]0x%x,[4]0x%x,[5]0x%x,[6]0x%x,[7]0x%x\n",aci_e2,aci_e3,fb_i_1,fb_q_1,era_th);
*/
    return;

}


#endif

/***********************************************************************************
  Subject:    read register
  Function:   MDrv_1210_IIC_Bypass_Mode
  Parmeter:
  Return:
  Remark:
************************************************************************************/
//void MDrv_1210_IIC_Bypass_Mode(MS_BOOL enable)
//{
//    UNUSED(enable);
//    if (enable)
//        MDrv_SYS_DMD_VD_MBX_WriteReg(0x8010, 0x10);        // IIC by-pass mode on
//    else
//        MDrv_SYS_DMD_VD_MBX_WriteReg(0x8010, 0x00);        // IIC by-pass mode off
//}
