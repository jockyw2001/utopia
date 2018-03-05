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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
// Internal Definition
#include "regCHIP.h"
#include "drvAUDIO.h"
#include "regAUDIO.h"
#include "halAUDIO.h"
#include "halMAD2.h"
#include "halMAD.h"
#include "halAUR2.h"
#include "ddr_config.h"
#include "r2_shm_comm.h"
#include "decR2_shm.h"
#if ASND_R2_SUPPORT
#include "sndR2_shm.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define DBG_MAD2(msg) //msg
#define AUD_ERRMSG(msg)  MS_CRITICAL_MSG(msg)
#define AUD_DBGMSG(msg)  MS_DEBUG_MSG(msg)

#ifdef CONFIG_MBOOT //mboot Speed up
    #define HALMAD2_PRINT(fmt, args...)    //printf("     " fmt, ## args)
    #define HALMAD2_ERROR(fmt, args...)    printf("     " fmt, ## args)
#else
    #define HALMAD2_PRINT(fmt, args...)    printf("      " fmt, ## args)
    #define HALMAD2_ERROR(fmt, args...)    printf("      " fmt, ## args)
#endif

#define HALMAD2_CHECK_SHM_INIT \
    do { \
        if (g_AudioVars2 == NULL) \
        { \
            HALMAD2_ERROR("%s() : Warning! g_AudioVars2 should not be NULL !!!\n", __FUNCTION__); \
            HALMAD2_ERROR("%s() : Perform SHM Init here !!!\n", __FUNCTION__); \
            if (HAL_AUDIO_InitialVars() == FALSE) \
            {\
                MS_ASSERT(0);\
            }\
        } \
    } while(0)

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define H2BYTE(value)             ((MS_U8)((value) / 0x10000))
#define HIBYTE(value)             ((MS_U8)((value >>8) & 0x0000FF))
#define LOBYTE(value)             ((MS_U8)(value&0x0000FF))

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern MS_U8  g_Dsp2CodeType;

extern AUDIO_SHARED_VARS2 * g_AudioVars2;

extern MS_S32  _s32AUDIOMutexIDMA;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

static MS_U16   CH1_setting_tmp;
static MS_U16   CH2_setting_tmp;
static MS_U16   CH3_setting_tmp;
static MS_U16   CH4_setting_tmp;
static MS_U16   CH5_setting_tmp;
static MS_U16   CH6_setting_tmp;
static MS_U16   CH7_setting_tmp;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------



///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO_MAD2 SYSETM Relational Hal Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_SetMemInfo()
/// @brief \b Function \b Description:  This routine is used to set the SE-DSP memory information
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD2_SetMemInfo(void)
{
    MS_U8 MIUBank = 0;
    MS_U32 SE_DSP_DDR_ADDR = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE);
    MS_U32 DEC_R2_ADDR = HAL_AUDIO_GetDspMadBaseAddr(DSP_ADV); // Get ADEC R2 Start address
#if ASND_R2_SUPPORT
    MS_U32 SND_R2_ADDR = DEC_R2_ADDR + ADEC__R2_DDR_SIZE; // Get SND-R2 Start address
#endif
    MS_U32 SE_DSP_DDR_ADDR_LINE_BASE = SE_DSP_DDR_ADDR >> BYTES_IN_MIU_LINE_LOG2;
    MS_U32 SE_DSP_DDR_ADDR_ICACHE = SE_DSP_DDR_ADDR >> MAD_MEMOFFSET_LOG2;

    HALMAD2_PRINT("HAL_MAD2_SetMemInfo : SE_DSP_DDR_ADDR = 0x%08X, DEC_R2_ADDR = 0x%08X\n", SE_DSP_DDR_ADDR, DEC_R2_ADDR);

    // Set MIU BANK Info
    MIUBank = g_AudioVars2->g_DSPMadMIUBank[DSP_ADV];
    HAL_AUDIO_WriteMaskByte((REG_RIU_MAIL_00 + 1), 0x01, MIUBank);

    // switch the MAD_BASE_ADDR control to MCU
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0001, 0x0000);

    HAL_MAD2_SetMcuCmd(0x00);
    HAL_AUDIO_WriteMaskReg(REG_SE_AUD_DTRL, 0x04FF, 0x0400);//0x2DE0                //reset AUD register

    // Set  MAD BASE (for paser)
    // For ES 1/2
    HAL_AUDIO_WriteMaskByte((REG_DEC_MAD_OFFSET_BASE_L + 1), 0xFF, (MS_U8)(SE_DSP_DDR_ADDR_LINE_BASE & 0xFF)); // [7:0]
    HAL_AUDIO_WriteReg(REG_DEC_MAD_OFFSET_BASE_H, (MS_U16)((SE_DSP_DDR_ADDR_LINE_BASE >> 8) & 0xFFFF)); // [23:8]
    HAL_AUDIO_WriteReg(REG_DEC_MAD_OFFSET_BASE_EXT, (MS_U16)((SE_DSP_DDR_ADDR_LINE_BASE >> 24) & 0x0F)); // [27:24]

    // For ES3
    HAL_AUDIO_WriteMaskByte((REG_SE_MAD_OFFSET_BASE_L + 1), 0xFF, (MS_U8)(SE_DSP_DDR_ADDR_LINE_BASE & 0xFF)); // [7:0]
    HAL_AUDIO_WriteReg(REG_SE_MAD_OFFSET_BASE_H, (MS_U16)((SE_DSP_DDR_ADDR_LINE_BASE >> 8) & 0xFFFF)); // [23:8]
    HAL_AUDIO_WriteReg(REG_SE_MAD_OFFSET_BASE_EXT, (MS_U16)((SE_DSP_DDR_ADDR_LINE_BASE >> 24) & 0x0F)); // [27:24]

    HAL_AUDIO_WriteMaskReg(REG_SE_AUD_DTRL, 0xFFFF, 0x0400);

    // Config as ES1 (DTV)
    HAL_AUDIO_WriteMaskReg(REG_DEC_MCFG, 0x00FF, 0x0002);
    HAL_AUDIO_WriteReg(REG_DEC_MBASE_H, ((OFFSET_ES1_DRAM_ADDR>>BYTES_IN_MIU_LINE_LOG2) >> 8));        // Unit : Line
    HAL_AUDIO_WriteMaskReg(REG_DEC_MSIZE_H, 0xFF00, (ES1_DRAM_SIZE>>BYTES_IN_MIU_LINE_LOG2)-1);          // Unit : Line

    // Config as ES2 (DTV)
    HAL_AUDIO_WriteMaskReg(REG_DEC_MCFG, 0x00FF, 0x0003);
    HAL_AUDIO_WriteReg(REG_DEC_MBASE_H, ((OFFSET_ES2_DRAM_ADDR>>BYTES_IN_MIU_LINE_LOG2) >> 8));        // Unit : Line
    HAL_AUDIO_WriteMaskReg(REG_DEC_MSIZE_H, 0xFF00, (ES2_DRAM_SIZE>>BYTES_IN_MIU_LINE_LOG2)-1);          // Unit : Line


    // ES3 config (SE-DSP)
    HAL_AUDIO_WriteMaskReg(REG_SE_MCFG, 0x00FF, 0x0002);
    HAL_AUDIO_WriteReg(REG_SE_MBASE_H, ((OFFSET_ES3_DRAM_ADDR>>BYTES_IN_MIU_LINE_LOG2) >> 8));        // Unit : Line
    HAL_AUDIO_WriteMaskReg(REG_SE_MSIZE_H, 0xFF00, (ES3_DRAM_SIZE>>BYTES_IN_MIU_LINE_LOG2)-1);          // Unit : Line

    // ES4 config (SE-DSP)
    HAL_AUDIO_WriteMaskReg(REG_SE_MCFG, 0x00FF, 0x0003);
    HAL_AUDIO_WriteReg(REG_SE_MBASE_H, ((OFFSET_ES4_DRAM_ADDR>>BYTES_IN_MIU_LINE_LOG2) >> 8));        // Unit : Line
    HAL_AUDIO_WriteMaskReg(REG_SE_MSIZE_H, 0xFF00, (ES4_DRAM_SIZE>>BYTES_IN_MIU_LINE_LOG2)-1);          // Unit : Line

    // Sound Effect DMA
    // ICACHE BASE
    HAL_AUDIO_WriteReg(REG_SE_DSP_ICACHE_BASE_L, (MS_U16)(SE_DSP_DDR_ADDR_ICACHE & 0xFFFF));
    HAL_AUDIO_WriteMaskByte(REG_SE_BDMA_CFG, 0x0F, (MS_U8)((SE_DSP_DDR_ADDR_ICACHE >> 16) & 0x0F));

    // Reset MAD module
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0040, 0x0040);
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0080, 0x0080);
    //AUDIO_DELAY1MS(5);

    // Set MAD module
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0080, 0x0000);
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0040, 0x0000);

  // Reset MAD & ReEnable MIU Request for SND-DSP
    HAL_MAD2_RSTMAD_DisEn_MIUREQ();

  /********************************************************
  *     DEC-R2 Meminfo Setting
  ********************************************************/
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_SYNTH_EXPANDER, REG_DECR2_CTRL_BANK_SW_BIT_MASK, 0x00);        //BANK 1630 control by MCU
    HAL_DEC_R2_EnableR2(FALSE);
    HALMAD2_PRINT(" ======> Audio DEC_R2 Base : %x\n", DEC_R2_ADDR);
    HAL_AUR2_WriteReg(REG_DECR2_ICMEM2_BASE_LO, (MS_U16)(DEC_R2_ADDR & 0xFFFF));
    HAL_AUR2_WriteReg(REG_DECR2_ICMEM2_BASE_HI, (MS_U16)((DEC_R2_ADDR >> 16) & 0xFFFF));
    HAL_AUR2_WriteReg(REG_DECR2_DCMEM_BASE_LO, (MS_U16)(DEC_R2_ADDR & 0xFFFF));
    HAL_AUR2_WriteReg(REG_DECR2_DCMEM_BASE_HI, (MS_U16)((DEC_R2_ADDR >> 16) & 0xFFFF));

    HAL_AUR2_WriteReg(REG_DECR2_DQMEM_BASE_LO, 0x0000);
    HAL_AUR2_WriteReg(REG_DECR2_DQMEM_BASE_HI, 0xFFFF);
    HAL_AUR2_WriteReg(REG_DECR2_DQMEM_SIZE_MASK_LO, 0x0000);
    HAL_AUR2_WriteReg(REG_DECR2_DQMEM_SIZE_MASK_HI, 0xFFFF);

    HAL_AUR2_WriteReg(REG_DECR2_IO1_MAPPING_BASE_HI, 0x9000);
    HAL_AUR2_WriteReg(REG_DECR2_IO2_MAPPING_BASE_HI, 0xB000);
    HAL_AUR2_WriteReg(REG_DECR2_MEM_CTRL, 0x0017);
    HAL_AUR2_WriteReg(REG_DECR2_SWITCH_CTRL, 0x7000);                     // Exception Vector switch to 0x0000_01x0

#if ASND_R2_SUPPORT
  /********************************************************
  *     SND-R2 Meminfo Setting
  ********************************************************/
    HAL_AUDIO_WriteMaskByte(REG_AUDIO_SYNTH_EXPANDER, REG_SNDR2_CTRL_BANK_SW_BIT_MASK, 0x00);        //BANK 1129 control by MCU
    HAL_SND_R2_EnableR2(FALSE);
    HALMAD2_PRINT(" ======> Audio SND_R2 Base : %08x\n", SND_R2_ADDR);
    HAL_AUR2_WriteReg(REG_SNDR2_ICMEM2_BASE_LO, (MS_U16)(SND_R2_ADDR & 0xFFFF));
    HAL_AUR2_WriteReg(REG_SNDR2_ICMEM2_BASE_HI, (MS_U16)((SND_R2_ADDR >> 16) & 0xFFFF));
    HAL_AUR2_WriteReg(REG_SNDR2_DCMEM_BASE_LO, (MS_U16)(SND_R2_ADDR & 0xFFFF));
    HAL_AUR2_WriteReg(REG_SNDR2_DCMEM_BASE_HI, (MS_U16)((SND_R2_ADDR >> 16) & 0xFFFF));

    HAL_AUR2_WriteReg(REG_SNDR2_DQMEM_BASE_LO, 0x0000);
    HAL_AUR2_WriteReg(REG_SNDR2_DQMEM_BASE_HI, 0xFFFF);
    HAL_AUR2_WriteReg(REG_SNDR2_DQMEM_SIZE_MASK_LO, 0x0000);
    HAL_AUR2_WriteReg(REG_SNDR2_DQMEM_SIZE_MASK_HI, 0xFFFF);

    HAL_AUR2_WriteReg(REG_SNDR2_IO1_MAPPING_BASE_HI, 0x9000);
    HAL_AUR2_WriteReg(REG_SNDR2_IO2_MAPPING_BASE_HI, 0xB000);
    HAL_AUR2_WriteReg(REG_SNDR2_IO3_MAPPING_BASE_HI, 0xB000);
    HAL_AUR2_WriteReg(REG_SNDR2_MEM_CTRL, 0x0017);
    HAL_AUR2_WriteReg(REG_SNDR2_SWITCH_CTRL, 0x7000);                     // Exception Vector switch to 0x0000_01x0
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_SetDecCmd()
/// @brief \b Function \b Description: This routine is to set  the DVB2 decoder command.
/// @param <IN>        \b u8DecCmd    :    0--STOP
///                                    1--PLAY
///                                    1--RESYNC
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD2_SetDecCmd(MS_U16 u16DecCmd)
{
    MS_U8 DecCmd = 0;
    DBG_MAD2(printf("HAL_MAD2_SetDecCmd:(%x)\r\n", u16DecCmd));

    if (u16DecCmd & 0x0100)
    {
        if (IS_AUDIO_DSP_CODE_TYPE_SIF(HAL_AUDIO_GetDsp2CodeType()))
        {
            // SIF command
            HAL_AUDIO_WriteMaskByte(REG_SE_DECODE_CMD, 0x1F, (MS_U8)(u16DecCmd&0x01F) );
        }
    }
    else
    {
        DecCmd = (MS_U8)(u16DecCmd&0x01F);

        if (DecCmd == AU_DVB2_DECCMD_PLAY)
        {
            // AD start
            HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x10, AU_DEC_R2_CMD_PLAY_AD);
        }
        else if (DecCmd == AU_DVB2_DECCMD_STOP)
        {
            // AD stop
            HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x10, AU_DEC_R2_CMD_STOP);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_SetDecCmd2()
/// @brief \b Function \b Description: This routine is to set  the DVB2 decoder command.
/// @param <IN>        \b u8DecCmd    :    0--STOP
///                                    1--PLAY
///                                    1--RESYNC
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD2_SetDecCmd2(MS_U16 u16DecCmd)
{
    HAL_MAD_SetDSP2DecCmd((AU_DVB_DECCMD)u16DecCmd);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_GetDecCmd()
/// @brief \b Function \b Description: This routine is to get  the DVB1 decode command.
/// @param <IN>        \b u8DecCmd    :
///                                    0--STOP
///                                    1--PLAY
///                                    1--RESYNC
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_MAD2_GetDecCmd(void)
{
    MS_U8 dec2cmd_status;

    dec2cmd_status = HAL_AUDIO_AbsReadByte(REG_R2_DECODE2_CMD);
    dec2cmd_status = dec2cmd_status & 0x10;

    return(dec2cmd_status);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_SetFreeRun()
/// @brief \b Function \b Description: This routine is to set  the DVB2 decoder in free run or AV-sync mode.
/// @param <IN>        \b u8FreeRun    :    0--normal AV sync
///                                    1--free-run mode
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD2_SetFreeRun(MS_U8 u8FreeRun)
{
    DBG_MAD2(printf("HAL_MAD2_SetFreeRun:(%x)\r\n",u8FreeRun));
    if( u8FreeRun >= 2 )
    {
        DBG_MAD2(printf("Invalid mode\r\n"));
    }

    HAL_AUDIO_WriteMaskByte(REG_SE_DECODE_CMD, 0x20, u8FreeRun<<5 );
}

#if 0  //replace by  HAL_MAD2_SetPIOCmd( ) and  HAL_MAD2_TriggerPIO8( )
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_Pio8()
/// @brief \b Function \b Description: This routine send a PIO8 interrupt to DSP with a command on 0x2D23.
/// @param <IN>        \b u16Cmd    : 0xE0, for MHEG5 file protocol
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD2_Pio8(MS_U16 u16Cmd)
{
    MS_U8 tmp = 0;
    // Enter MCU/DSP hand-shake
    HAL_AUDIO_SeWriteByte(0x2D23, u16Cmd);

    // just toggle
    tmp = HAL_AUDIO_SeReadByte(0x2D00);
    tmp ^= 0x20;
    HAL_AUDIO_SeWriteMaskByte(0x2D00, 0x20, tmp);

    tmp ^= 0x20;
    HAL_AUDIO_SeWriteMaskByte(0x2D00, 0x20, tmp);
}
#endif
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_SetPIOCmd()
/// @brief \b Function \b Description: This routine set a command on 0x2D23 for PIO8 interrupt(to DSP).
/// @param <IN>        \b u16Cmd    : 0xE0, for MHEG5 file protocol
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD2_SetPIOCmd(MS_U8 Cmd)
{
    if(Cmd == 0xE0)
    {   // 2nd decoder PIO_MHEG5 isr
        HAL_AUDIO_WriteMaskReg(REG_MB_SE_PIO_ID, 0xFF00, 0xE000);
    }
    else
    {   // reload command etc.
        HAL_AUDIO_WriteMaskReg(REG_MB_SE_PIO_ID, 0xFF00, (MS_U16)Cmd<<8);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_Read_DSP_sram()
/// @brief \b Function \b Description: This routine is used to Read DSP internal sram value by IDMA
/// @param <IN>        \b dsp_addr    : DSP internal sram address
/// @param <IN>        \b dm    :    0--DSP_MEM_TYPE_PM
///                                1--DSP_MEM_TYPE_DM
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b value :    MS_U32 sram value
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD2_Read_DSP_sram(MS_U16 dsp_addr,AUDIO_DSP_MEMORY_TYPE dsp_memory_type)
{
    return 0;  //secure boot: MCU can't use IDMA
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_Write_DSP_sram()
/// @brief \b Function \b Description: This routine is used to Write DSP internal sram value by IDMA
/// @param <IN>        \b dsp_addr    : DSP internal sram address
/// @param <IN>        \b value     : data want to write
/// @param <IN>        \b dm        :    0-- write to DSP_MEM_TYPE_PM
///                                        1-- write to DSP_MEM_TYPE_DM
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD2_Write_DSP_sram(MS_U16 dsp_addr, MS_U32 value, AUDIO_DSP_MEMORY_TYPE dsp_memory_type)
{
    return TRUE;   //secure boot: MCU can't use IDMA
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_DisEn_MIUREQ()
/// @brief \b Function \b Description: This routine is to reset DVB2 MIU request command.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD2_DisEn_MIUREQ(void)
{
    // Disable MIU Request
    HAL_AUDIO_WriteMaskReg(REG_SE_AUD_DTRL, 0x0040, 0x0040 );
    AUDIO_DELAY1MS(1);
    // Enable MIU Request
    HAL_AUDIO_WriteMaskReg(REG_SE_AUD_DTRL, 0x0040, 0x0000 );
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_DisMiuReq()
/// @brief \b Function \b Description: This routine is to reset DVB2 MIU request command.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD2_DisMiuReq(void)
{
    // Disable MIU Request
    HAL_AUDIO_WriteMaskReg(REG_SE_AUD_DTRL, 0x0040, 0x0040 );        // disable
    HAL_AUDIO_WriteMaskReg(REG_SE_AUD_DTRL, 0x0080, 0x0080 );          // reset MAD module
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_RSTMAD_DisEn_MIUREQ()
/// @brief \b Function \b Description: This routine is to reset DVB2 MIU request command.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD2_RSTMAD_DisEn_MIUREQ(void)
{
    // Disable MIU Request
    HAL_AUDIO_WriteMaskReg(REG_SE_AUD_DTRL, 0x0040, 0x0040 );        // disable
    // Reset MAD
    HAL_AUDIO_WriteMaskReg(REG_SE_AUD_DTRL, 0x0080, 0x0080 );          // reset MAD module
    AUDIO_DELAY1MS(1);
    // Set MAD
    HAL_AUDIO_WriteMaskReg(REG_SE_AUD_DTRL, 0x0080, 0x0000 );
    // Enable MIU Request
    HAL_AUDIO_WriteMaskReg(REG_SE_AUD_DTRL, 0x0040, 0x0000 );
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_Backup_pathreg()
/// @brief \b Function \b Description: This routine used to backup path register
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD2_Backup_pathreg(void)
{
    CH1_setting_tmp = HAL_AUDIO_SeReadReg(0x2C64);
    CH2_setting_tmp = HAL_AUDIO_SeReadReg(0x2C66);
    CH3_setting_tmp = HAL_AUDIO_SeReadReg(0x2C68);
    CH4_setting_tmp = HAL_AUDIO_SeReadReg(0x2C6A);
    CH5_setting_tmp = HAL_AUDIO_SeReadReg(0x2C76);
    CH6_setting_tmp = HAL_AUDIO_SeReadReg(0x2C78);
    CH7_setting_tmp = HAL_AUDIO_SeReadReg(0x2C7A);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_Restore_pathreg()
/// @brief \b Function \b Description: This routine used to restore path register
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD2_Restore_pathreg(void)
{
    HAL_AUDIO_SeWriteReg(0x2C64, CH1_setting_tmp);
    HAL_AUDIO_SeWriteReg(0x2C66, CH2_setting_tmp);
    HAL_AUDIO_SeWriteReg(0x2C68, CH3_setting_tmp);
    HAL_AUDIO_SeWriteReg(0x2C6A, CH4_setting_tmp);
    HAL_AUDIO_SeWriteReg(0x2C76, CH5_setting_tmp);
    HAL_AUDIO_SeWriteReg(0x2C78, CH6_setting_tmp);
    HAL_AUDIO_SeWriteReg(0x2C7A, CH7_setting_tmp);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_EnableChIRQ()
/// @brief \b Function \b Description: This routine used to disable ch irq
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD2_EnableChIRQ(MS_BOOL bEnable)
{
    if(bEnable)
    {
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_IRQ_CONTROL1, 0xF0, 0x00);     // Enable CH1 ~ CH4 IRQ
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_IRQ_CONTROL1_2, 0x10, 0x00);     // Enable CH5 IRQ
    }
    else
    {
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_IRQ_CONTROL1, 0xF0, 0xF0);     // Mask CH1 ~ CH4 IRQ
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_IRQ_CONTROL1_2, 0x10, 0x10);     // Mask CH5 IRQ
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_SetDspIDMA()
/// @brief \b Function \b Description:  This function is used to set DSP IDMA.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD2_SetDspIDMA(void)
{
    HAL_AUDIO_WriteMaskByte(REG_SE_IDMA_CTRL0,0x00FF, 0x0083);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_TriggerPIO8()
/// @brief \b Function \b Description:  This function is used to trigger PIO8 init.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD2_TriggerPIO8(void)
{
    HAL_AUDIO_WriteMaskByte(REG_SE_IDMA_CTRL0, 0x0020, 0x00A0);
    AUDIO_DELAY1MS(2);
    HAL_AUDIO_WriteMaskByte(REG_SE_IDMA_CTRL0, 0x0020, 0x0080);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_GetReloadCodeAck()
/// @brief \b Function \b Description: This routine is to report DSP reload ACK cmd.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_MAD2_GetReloadCodeAck(void)
{
    return(HAL_AUDIO_ReadByte(REG_MB_SE_ACK2));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_GetLoadCodeAck()
/// @brief \b Function \b Description: This routine is to report DSP reload ACK cmd.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_MAD2_GetLoadCodeAck(void)
{
    return(HAL_AUDIO_ReadByte(REG_MB_SE_ACK1));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_SetMcuCmd()
/// @brief \b Function \b Description: This routine is to write MCU cmd.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD2_SetMcuCmd(MS_U8 cmd)
{
    if( cmd == 0xF3 )
    {
        HAL_AUR2_WriteMaskReg(REG_DECR2_SYSTEM_START, 0xFFFF, 0x00F3);
        HAL_AUR2_WriteMaskReg(REG_SNDR2_SYSTEM_START, 0xFFFF, 0x00F3);
    }

    HAL_AUDIO_WriteMaskReg(REG_MB_SE_CMD1, 0xFF00, (MS_U16)cmd<<8);

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_ReLoadCode()
/// @brief \b Function \b Description: This routine is used to reload the DVB2 algorithm dsp code.
/// @param u8Type      \b algorithm type
/// @return MS_BOOL    \b reload code success or not
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD2_ReLoadCode(AUDIO_DSP_CODE_TYPE dspCodeType)
{
    MS_BOOL ret_status = FALSE;
    MS_BOOL loadcode_status = FALSE;
    MS_U32  time_out = 0;

    if(HAL_AUDIO_GetDsp2CodeType() == dspCodeType)           // if type is the same, don't reload again
    {
        printf("Audio DSP2 code is same(%x), no need to reload\n", dspCodeType);
        return TRUE;
    }

    HAL_MAD2_SetDecCmd(0x100);                      // Stop

    HAL_MAD2_DisEn_MIUREQ();

    DBG_MAD2(printf("type=%x\r\n",dspCodeType));

    HAL_AUDIO_SetDsp2CodeType(dspCodeType, ret_status);

    MsOS_EnableInterrupt(E_INT_FIQ_DEC_DSP2UP);

    switch(dspCodeType)
    {
        case AU_ATVENC_BTSC:
            HAL_AUR2_WriteByte(REG_R2_ASNDDSP_TYPE, asnd_dsp_type_btscencode);
            loadcode_status = TRUE;
            break;

        case AU_ATVENC_FMTx:
            HAL_AUR2_WriteByte(REG_R2_ASNDDSP_TYPE, asnd_dsp_type_fmtx);
            loadcode_status = TRUE;
            break;

        case AU_DVB2_ENCODE_DTSE:
            HAL_AUR2_WriteByte(REG_R2_ASNDDSP_TYPE, asnd_dsp_type_dtst);
            loadcode_status = TRUE;
            break;

        case AU_DVB2_ENCODE_MS10_DDE:
            HAL_AUR2_WriteByte(REG_R2_ASNDDSP_TYPE, asnd_dsp_type_MS10dde);
            loadcode_status = TRUE;
            break;

        case AU_DVB2_ENCODE_DDE:
            HAL_AUR2_WriteByte(REG_R2_ASNDDSP_TYPE, asnd_dsp_type_dde);
            loadcode_status = TRUE;
            break;

        default:
            HALMAD2_ERROR("Not correct DSP type\n");
            loadcode_status = FALSE;
            break;
    }

    if(loadcode_status == TRUE)
    {
        while(time_out++<500)
        {
            if(HAL_AUR2_ReadReg(REG_DECR2_DSPRELOAD_DONE_ACK) == 0x5678)
                break;
            AUDIO_DELAY1MS(1);
        }

        if(time_out>=500)
        {
            DBG_MAD2("DSP2 Re-Active\n");
        }
        else
        {
            DBG_MAD2("audio DSP_SE ReloadCode success..\n");
        }
    }

    return ret_status;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_RebootDsp(void)
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD2_RebootDsp(void)
{

    MS_U8 u8DspCodeType;
    AU_DVB_DECCMD deccmd_status;
    MS_U8 u8Dsp2CodeType=HAL_AUDIO_GetDsp2CodeType();

    HAL_MAD2_Backup_pathreg();

    AUD_ERRMSG(printf("\n*** MAD Auto-Recovery SE DSP*** \n"));
    AUD_ERRMSG(printf("*** g_Dsp2CodeType: 0x%x \n", g_Dsp2CodeType));

    // Reset MAD module
    HAL_MAD2_RSTMAD_DisEn_MIUREQ();
    AUDIO_DELAY1MS(2);

    AUD_DBGMSG(printf("*** Load code and reset SE DSP \n"));
    HAL_MAD2_EnableChIRQ(FALSE);
    HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_CHANNEL, FALSE, NULL);  // Audio SIF channel enable setting -> disable
    u8DspCodeType=(MS_U8)(HAL_AUDIO_GetDspCodeType());
    deccmd_status = HAL_MAD_GetDecCmd(DVB_Audio_Decoder1);

    HAL_MAD2_SetDecCmd(0x100); // CathyNeedRefine
    HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_STOP);

    HAL_MAD2_SetMemInfo();

    // load DSP2 code
    HAL_AUDSP_DspLoadCode(u8Dsp2CodeType);                // Se
    HAL_AUDSP_DspLoadCode(AU_SND_EFFECT);   // AdvSe

    HAL_AUDIO_SeSystemLoadCode();                                            // system segment

    HAL_MAD2_EnableChIRQ(TRUE);
    HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_CHANNEL, TRUE, NULL);  // Audio SIF channel enable setting -> enable
    AUDIO_DELAY1MS(500);


    HAL_MAD_LoadCode((AUDIO_DSP_CODE_TYPE)u8DspCodeType);

    HAL_MAD_SetDecCmd(deccmd_status);

    HAL_MAD2_Restore_pathreg();

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_XPCM_setParam()
/// @brief \b Function \b Description: This routine configures necessary parameters when playing XPCM data
/// @param audioType   \b type: 1--lpcm
///                             2--MS-ADPCM
///                             17--IMA-ADPCM
/// @param Channels    \b channel num: 0-- mono
///                                    1-- stereo
/// @param sampleRate  \b sample rate: 8, 11.025, 16, 22.05, 44.1, 48 (KHz)
/// @param bitsPerSample \b bit per sample:  8--8 bits
///                                         16--16 bits
/// @param blockSize \b block size
/// @param samplePerBlock \b samples per block
/// @return MS_U8    \b TRUE--parameters are correct
///                     FALSE--parameters are incorrect
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD2_XPCM_setParam (XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate,
                                                    MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
{
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_TYPE, ADEC2, audioType, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_CHANNELS, ADEC2, channels, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_SAMPLERATES, ADEC2, sampleRate, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_BITS_PER_SAMPLE, ADEC2, bitsPerSample, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_BLOCK_SIZE, ADEC2, blockSize, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_SAMPLE_PER_BLOCK, ADEC2, samplePerBlock, 0 );

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_XPCM2_CheckIntStatus()
/// @brief \b Function \b Description:
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b value :    MS_U32 sram value for interrupt status (No DSP decode XPCM return 0)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_MAD2_XPCM2_CheckIntStatus(void)
{
    return 0;  // no dsp in kano
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_Read_DSP_Xbox()
/// @brief \b Function \b Description: This routine is used to Read DSP internal Xbox value
/// @param <IN>        \b dsp_addr    : DSP internal Xbox address
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b value :    MS_U32 Xbox value
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD2_Read_DSP_Xbox(MS_U16 dsp_addr)
{
    MS_U16 dat[2];
    MS_U32 value=0;

    if(HAL_AUDIO_GetDSPalive() == 0)
    {
        return 0;
    }

    HALMAD2_CHECK_SHM_INIT;

    OS_OBTAIN_MUTEX(_s32AUDIOMutexIDMA, MSOS_WAIT_FOREVER);

    // check IDMA busy or not
    if(HAL_AUDSP_Check_XBox_TransData_Ready() == FALSE)
    {
        //printf("ERROR: XBox trans data Busy \n");

        OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

        return 0;
    }

    HAL_AUDIO_WriteReg( REG_DSP_XBOX_ADDRESS, (dsp_addr&0x01FF));
    HAL_AUDIO_WriteMaskByte(REG_DSP_XBOX_MISC, 0x07, 0x04);  //bit[2]:read

    if (HAL_AUDSP_Check_XBox_TransData_Ready() == FALSE)
    {
        //printf("ERROR: read data  from XBox time out \n");

        OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

        return 0;
    }

    dat[1] = HAL_AUDIO_ReadReg(REG_DSP_XBOX_RD_DATA_H);  //[23:8]
    dat[0] = HAL_AUDIO_ReadReg(REG_DSP_XBOX_RD_DATA_L);  //[7:0]

    value = ((dat[1] << 8) | dat[0]);

    OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

    return value;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD2_Write_DSP_Xbo()
/// @brief \b Function \b Description: This routine is used to Write DSP internal Xbox value
/// @param <IN>        \b dsp_addr    : DSP internal Xbox address
/// @param <IN>        \b value     : data want to write
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD2_Write_DSP_Xbox(MS_U16 dsp_addr, MS_U32 value)
{
    MS_U16 dat[2];

    if(HAL_AUDIO_GetDSPalive() == 0)
        return 0;

    HALMAD2_CHECK_SHM_INIT;

    OS_OBTAIN_MUTEX(_s32AUDIOMutexIDMA, MSOS_WAIT_FOREVER);

    if (HAL_AUDSP_Check_XBox_TransData_Ready() == FALSE)
    {
        //printf("SE DSP XBox Busy \n");

        OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

        return FALSE;
    }

    dat[1] = ((MS_U16)((value >>8) & 0xFFFF)); //[23:8]
    dat[0] = ((MS_U16)(value & 0x00FF)); //[7:0]

    HAL_AUDIO_WriteReg( REG_DSP_XBOX_ADDRESS, (dsp_addr&0x01FF));

    HAL_AUDIO_WriteReg( REG_DSP_XBOX_WR_DATA_H, dat[1]);   //[23:8]
    HAL_AUDIO_WriteReg( REG_DSP_XBOX_WR_DATA_L, dat[0]);   //[7:0]

    HAL_AUDIO_WriteMaskByte(REG_DSP_XBOX_MISC, 0x07, 0x02);  //bit[1]:write

    if (HAL_AUDSP_Check_XBox_TransData_Ready() == FALSE)
    {
        //printf("ERROR: SE DSP XBox write data time out2 \n");

        OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

        return FALSE;
    }

    OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

    return TRUE;
}