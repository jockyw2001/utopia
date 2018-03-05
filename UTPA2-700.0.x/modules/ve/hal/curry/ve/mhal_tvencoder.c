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
// (!!¡?MStar Confidential Information!!L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
/// @file  mhal_tvencoder.c
/// @brief TV encoder Module
/// @author MStar Semiconductor Inc.
///
////////////////////////////////////////////////////////////////////////////////
#ifndef _MHALTVENCODER_C
#define _MHALTVENCODER_C

////////////////////////////////////////////////////////////////////////////////
// Include List
////////////////////////////////////////////////////////////////////////////////
#include "ve_Analog_Reg.h"
#include "ve_hwreg_utility2.h"

#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drvTVEncoder.h"
#include "mhal_tvencoder_tbl.h"
#include "mhal_tvencoder.h"
#include "drvMMIO.h"
#include "drvSYS.h"
#include "drvMIU.h"
#include "halCHIP.h"
#include "drvIPAUTH.h"

#ifdef CONFIG_MSTAR_CLKM
    #include "drvCLKM.h"
#endif

//For MV
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "mdrv_macrovision_tbl.h"

//For DCS
#include "mdrv_dcs_tbl.h"

#define VE_DOLBY_VERIFIER_STACK_SIZE        4096
#define VE_DOLBY_VERIFIER_DELAY             20
#define VE_DOLBY_VERIFIER_HASH_INFO_LENGTH  18
#define VE_DOLBY_VERIFIER_BUF_SIZE          1024
#define VE_DOLBY_VERIFIER_TIMER             10000//ms


#define TTX_PACKET_SIZE 48
#define TTX_CHECK_DONE_MAX_COUNT 2

#define MAX_COUNT_OF_TABLE          2000
#define U32_END_OF_TABLE_VALUE      0xffffff

////////////////////////////////////////////////////////
//Local Variable
////////////////////////////////////////////////////////
MS_VIRT _VE_RIU_BASE;

#if 0
//For Macrovision OTP checking
static MS_VIRT virtPMRIUBaseAddress = 0;
static MS_U32 u32PMBankSize = 0;
#endif

static MS_S32 _s32VE_MIUWriteAddressAdjustment = 0;
static MS_S32 _s32VE_MIUReadAddressAdjustment = 0;
static MS_BOOL bIs_DIEnabled = FALSE;
static MS_BOOL _gbUseCusTable = FALSE;
static MS_U8 _gpu8CusTable[MS_VE_VIDEOSYS_NUM][VE_CUSTBL_MAX_SIZE];
static MS_U8 _u8VE_MIUSel = 0;

typedef enum
{
    EN_VE_DOLBY_VERIFIER_PREPARE         = 0x00,
    EN_VE_DOLBY_VERIFIER_ONGOING         = 0x01,
    EN_VE_DOLBY_VERIFIER_DONE            = 0x02,
}EN_VE_DOLBY_VERIFIER_STATUS;

static MS_PHY _phyDolbyVerifierBufAddr = 0;
static MS_U8* _pu8DolbyVerifierBufVA = 0;
static EN_VE_DOLBY_VERIFIER_STATUS _enVE_DolbyVerifierStatus = EN_VE_DOLBY_VERIFIER_PREPARE;
#ifndef MSOS_TYPE_OPTEE
static MS_U8 _VE_DolbyVerifier_Stack[VE_DOLBY_VERIFIER_STACK_SIZE];
static MS_S32 _s32VE_DolbyVerifier_TaskId = -1;
#endif
static MS_U8 _u8MV_N00 = 0x36;//Default MS_VE_MV_NTSC_TYPE1

static MS_BOOL _bMV_Enable = FALSE;
static MS_VE_MV_TYPE _eMvType = MS_VE_MV_PAL_START;
static MS_BOOL _bMV_BitControl_Set = FALSE;
static MS_U8 _u8MV_BitControl_N0 = 0x00;

//Each item value can not be same
typedef enum
{
    E_VE_DUMPTABTYPE_MACROVISION = MACROVISION_TABTYPE_MACROVISION,
    E_VE_DUMPTABTYPE_DCS_CVBS = E_DCS_TABTYPE_CVBS,
    E_VE_DUMPTABTYPE_DCS_COMPONENT = E_DCS_TABTYPE_COMPONENT,
} E_VE_DUMP_TAB_TYPE;

////////////////////////////////////////////////////////
//MACRO
////////////////////////////////////////////////////////

#if 0
#define PM_RIU_READ_BYTE(addr)         ( READ_BYTE( virtPMRIUBaseAddress + (addr) ) )
#define PM_MDrv_ReadByte( u32Reg )   PM_RIU_READ_BYTE(((u32Reg) << 1) - ((u32Reg) & 1))
#endif

#define VE_DBG(msg) //msg

//for CVBS ouptout display centered in DE mode
//1 unit = 4 pixels in U4
#define PIXELS_PER_UNIT    (4)

////////////////////////////////////////////////////////
//Functions
////////////////////////////////////////////////////////
// Put this function here because hwreg_utility2 only for hal.
void Hal_VE_init_riu_base(MS_VIRT virtRiu_Base)
{
    _VE_RIU_BASE = virtRiu_Base;
}

void MsWriteVEReg(MS_U16 u16addr, MS_U16 u16Data)
{
    MDrv_Write2Byte(u16addr, u16Data);
}

void Hal_VE_WriteRegTbl ( const MS_U8 *pRegTable )
{
    MS_U32 u32Index = 0; // register index
    MS_U16 u16Count = MAX_COUNT_OF_TABLE;
    do
    {
        u32Index = ((pRegTable[0] << 16) + (pRegTable[1] << 8) + pRegTable[2]);
        if (u32Index == U32_END_OF_TABLE_VALUE) // check end of table
        {
            break;
        }

        u32Index &= U32_END_OF_TABLE_VALUE;

        MDrv_WriteByte( u32Index, pRegTable[3] );
        pRegTable += 4;

    } while (--u16Count > 0);
}

/******************************************************************************/
/*                     VE Function                                            */
/* ****************************************************************************/
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_set_color_convert()
/// @brief \b Function \b Description : Set the color convert for different
///                                     video system and DAC type
///
/// @param <IN>        \b VideoSys : The output video system
/// @param <IN>        \b u8DACType : The output DAC type
/// @param <OUT>       \b None :
/// @param <RET>       \b None :
/// @param <GLOBAL>    \b None :
////////////////////////////////////////////////////////////////////////////////
void Hal_VE_set_color_convert(MS_VE_VIDEOSYS videosys, MS_U8 u8dactype)
{

    if(videosys == MS_VE_NTSC || videosys == MS_VE_NTSC_J || videosys == MS_VE_NTSC_443)
    { // NTSC
        switch(u8dactype)
        {
        case VE_OUT_CVBS_YCC:
            break;

        case VE_OUT_CVBS_YCbCr:
            MDrv_WriteByte(L_BK_VE_ENC(0x41), 0x40);
            MDrv_WriteByte(L_BK_VE_ENC(0x42), 0x00);
            MDrv_WriteByte(L_BK_VE_ENC(0x43), 0x35);
            MDrv_WriteByte(L_BK_VE_ENC(0x44), 0x25);
            MDrv_WriteByte(L_BK_VE_ENC(0x45), 0x19);
            MDrv_WriteByte(L_BK_VE_ENC(0x46), 0x4B);
            break;

        case VE_OUT_CVBS_RGB:
            MDrv_WriteByte(L_BK_VE_ENC(0x41), 0x40);
            MDrv_WriteByte(L_BK_VE_ENC(0x42), 0x00);
            MDrv_WriteByte(L_BK_VE_ENC(0x43), 0x47);
            MDrv_WriteByte(L_BK_VE_ENC(0x44), 0x24);
            MDrv_WriteByte(L_BK_VE_ENC(0x45), 0x18);
            MDrv_WriteByte(L_BK_VE_ENC(0x46), 0x7E);

            //finetune for ScartRGB color bar test
            MDrv_WriteByte(L_BK_VE_ENC(0x04), 0x00);
            MDrv_WriteByte(H_BK_VE_ENC(0x04), 0x50);
            MDrv_WriteByte(L_BK_VE_ENC(0x27), 0x00);
            MDrv_WriteByte(L_BK_VE_ENC(0x2A), 0x46);
            MDrv_WriteByte(H_BK_VE_ENC(0x2A), 0x62);
            break;
        }
    }
    else
    { // PAL
        switch(u8dactype)
        {
        case VE_OUT_CVBS_YCC:
            break;

        case VE_OUT_CVBS_YCbCr:
            MDrv_WriteByte(L_BK_VE_ENC(0x41), 0x40);
            MDrv_WriteByte(L_BK_VE_ENC(0x42), 0x00);
            MDrv_WriteByte(L_BK_VE_ENC(0x43), 0x35);
            MDrv_WriteByte(L_BK_VE_ENC(0x44), 0x25);
            MDrv_WriteByte(L_BK_VE_ENC(0x45), 0x19);
            MDrv_WriteByte(L_BK_VE_ENC(0x46), 0x4B);
            break;

        case VE_OUT_CVBS_RGB:
            MDrv_WriteByte(L_BK_VE_ENC(0x41), 0x40);
            MDrv_WriteByte(L_BK_VE_ENC(0x42), 0x00);
            MDrv_WriteByte(L_BK_VE_ENC(0x43), 0x47);
            MDrv_WriteByte(L_BK_VE_ENC(0x44), 0x24);
            MDrv_WriteByte(L_BK_VE_ENC(0x45), 0x18);
            MDrv_WriteByte(L_BK_VE_ENC(0x46), 0x7E);

            //finetune for ScartRGB color bar test
            MDrv_WriteByte(L_BK_VE_ENC(0x04), 0x00);
            MDrv_WriteByte(H_BK_VE_ENC(0x04), 0x50);
            MDrv_WriteByte(L_BK_VE_ENC(0x27), 0x00);
            MDrv_WriteByte(L_BK_VE_ENC(0x2A), 0x46);
            MDrv_WriteByte(H_BK_VE_ENC(0x2A), 0x62);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_set_source_color_convert()
/// @brief \b Function \b Description : Set the color convert for different input source
///
/// @param <IN>        \b u8val : The value of register setting
/// @param <IN>        \b u8Mask: Mask bit
////////////////////////////////////////////////////////////////////////////////
void Hal_VE_set_source_color_convert(MS_U8 u8val, MS_U8 u8Mask)
{
    MDrv_WriteByteMask(L_BK_VE_SRC(0x0A), u8val, u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_set_444to422_mode()
/// @brief \b Function \b Description : Set the 444 to 422 mode setting
///
/// @param <IN>        \b u8val : The value of register setting
/// @param <IN>        \b u8Mask: Mask bit
////////////////////////////////////////////////////////////////////////////////
void Hal_VE_set_444to422_mode(MS_U8 u8val, MS_U8 u8Mask)
{
    MDrv_WriteByteMask(H_BK_VE_SRC(0x0A), u8val, u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_SelMIU(MS_U8)
/// @brief \b Function \b Description : select VE read/write memory in which MIU
///
/// @param <IN>        \b u8MIU_Seltype : select which MIU
/// @param <OUT>       \b None :
/// @param <RET>       \b None :
/// @param <GLOBAL>    \b None :
////////////////////////////////////////////////////////////////////////////////
//#define VE_MIU_GROUP1          (REG_MIU_BASE+0xF3)
void Hal_VE_SelMIU(MS_U8 u8MIU_Seltype)
{
    //Kano VE MIU is for TTX only
    if (u8MIU_Seltype == E_CHIP_MIU_3)
    {
        //MDrv_MIU_SelMIU(MIU_CLIENT_VE_W,MIU_SELTYPE_MIU3);
        MDrv_MIU_SelMIU(MIU_CLIENT_VE_R,MIU_SELTYPE_MIU3);
    }
    else if (u8MIU_Seltype == E_CHIP_MIU_2)
    {
        //MDrv_MIU_SelMIU(MIU_CLIENT_VE_W,MIU_SELTYPE_MIU2);
        MDrv_MIU_SelMIU(MIU_CLIENT_VE_R,MIU_SELTYPE_MIU2);
    }
    else if (u8MIU_Seltype == E_CHIP_MIU_1)
    {
        //MDrv_MIU_SelMIU(MIU_CLIENT_VE_W,MIU_SELTYPE_MIU1);
        MDrv_MIU_SelMIU(MIU_CLIENT_VE_R,MIU_SELTYPE_MIU1);
    }
    else
    {
        //MDrv_MIU_SelMIU(MIU_CLIENT_VE_W,MIU_SELTYPE_MIU0);
        MDrv_MIU_SelMIU(MIU_CLIENT_VE_R,MIU_SELTYPE_MIU0);
    }

    _u8VE_MIUSel = u8MIU_Seltype;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_SetMemAddr()
/// @brief \b Function \b Description : set VE read/write memory address
///
/// @param <IN>        \b phyRDAddress : Pointer of VE Memory Read address
/// @param <IN>        \b phyWRAddress : Pointer of VE Memory Write address
/// @param <OUT>       \b None :
/// @param <RET>       \b None :
/// @param <GLOBAL>    \b None :
////////////////////////////////////////////////////////////////////////////////
void Hal_VE_SetMemAddr(MS_PHY *pphyRDAddress, MS_PHY *pphyWRAddress)
{
    MS_PHY phyMIUAddress;
    MS_BOOL bOrgFlag = g_bVeDisableRegWrite;

    g_bVeDisableRegWrite = FALSE;

    if(pphyWRAddress != NULL)
    {
        phyMIUAddress = *pphyWRAddress / BYTE_PER_WORD;

        //Shift wirte address to centralize display according to write adjustment
        if(_s32VE_MIUWriteAddressAdjustment >= 0)
        {
            phyMIUAddress += (MS_PHY)_s32VE_MIUWriteAddressAdjustment;
        }
        else
        {
            phyMIUAddress -= (MS_PHY)(-1*_s32VE_MIUWriteAddressAdjustment);
        }

        MDrv_Write2Byte(L_BK_VE_SRC(0x7B), (MS_U16)(phyMIUAddress >> 0));
        MDrv_Write2Byte(L_BK_VE_SRC(0x7C), (MS_U16)(phyMIUAddress >> 16));
    }

    if(bOrgFlag == TRUE)
    {
        //wait Vsync
        g_bVeDisableRegWrite = TRUE;
        printf("Wait V Sync!!\n");
        MsOS_DelayTask(80);
        g_bVeDisableRegWrite = FALSE;
    }

    if(pphyRDAddress != NULL)
    {
        phyMIUAddress = *pphyRDAddress / BYTE_PER_WORD;

        //Shift read address to centralize display according to read adjustment
        if(_s32VE_MIUReadAddressAdjustment >= 0)
        {
            phyMIUAddress += (MS_PHY)_s32VE_MIUReadAddressAdjustment;
        }
        else
        {
            phyMIUAddress -= (MS_PHY)(-1*_s32VE_MIUReadAddressAdjustment);
        }

        MDrv_Write2Byte(L_BK_VE_SRC(0x01), (MS_U16)(phyMIUAddress >> 0));
        MDrv_Write2Byte(L_BK_VE_SRC(0x02), (MS_U16)(phyMIUAddress >> 16));
    }
    g_bVeDisableRegWrite = bOrgFlag;
}

static MS_BOOL _Hal_VE_get_reg_table_value(const MS_U8 *pu8RegTable, MS_U32 u32RegAddr, MS_U8* u8RegValue)
{
    MS_U32 u32Index = 0; // register index
    MS_U16 u16MaxTableSize = 2000;
    MS_BOOL bRet = FALSE;

    if(NULL == u8RegValue)
    {
        VE_DBG(printf("NULL pointer parameter!\n"));
        return FALSE;
    }

    if(NULL == pu8RegTable)
    {
        VE_DBG(printf("NULL table!\n"));
        return FALSE;
    }

    do
    {
        u32Index = ((pu8RegTable[0] << 16) + (pu8RegTable[1] << 8) + pu8RegTable[2]);

        if (u32Index == 0xFFFFFF) // check end of table
        {
            break;
        }

        u32Index &= 0xFFFFFF;

        // Found
        if(u32Index == u32RegAddr)
        {
            *u8RegValue = pu8RegTable[3];
            bRet = TRUE;
            break;
        }

        pu8RegTable += 4;

    } while (--u16MaxTableSize > 0);

    return bRet;
}

// Restore TTX settings to original
static void _Hal_VE_restore_orig_ttx_setting(void)
{
    MS_U16 u16TtxRange_OddStart = 0;
    MS_U16 u16TtxRange_OddEnd = 0;
    MS_U16 u16TtxRange_EvenStart = 0;
    MS_U16 u16TtxRange_EvenEnd = 0;
    MS_U32 u32TtxActiveLines = 0;
    MS_U8 u8RegValue_L = 0, u8RegValue_H = 0;
    MS_U8* pu8RegTable ;//= VE_OUT_VIDEOSTD_TBL[Hal_VE_Get_Output_Video_Std()].pVBI_TBL;

    if(MS_VE_PAL == Hal_VE_Get_Output_Video_Std())
    {
        pu8RegTable = VE_OUT_VIDEOSTD_TBL[MS_VE_PAL].pVBI_TBL;
    }
    else // NTSC
    {
        pu8RegTable = VE_OUT_VIDEOSTD_TBL[MS_VE_NTSC].pVBI_TBL;
    }

    // Odd start
    if(_Hal_VE_get_reg_table_value(pu8RegTable, L_BK_VE_ENC(0x58), &u8RegValue_L) && _Hal_VE_get_reg_table_value(pu8RegTable, H_BK_VE_ENC(0x58), &u8RegValue_H))
    {
        u16TtxRange_OddStart = u8RegValue_L + (u8RegValue_H << 8);
    }
    else
    {
        if(MS_VE_PAL == Hal_VE_Get_Output_Video_Std())
        {
            u16TtxRange_OddStart = 7;
        }
        else
        {
            u16TtxRange_OddStart = 10;
        }
    }

    // Odd end
    if(_Hal_VE_get_reg_table_value(pu8RegTable, L_BK_VE_ENC(0x59), &u8RegValue_L) && _Hal_VE_get_reg_table_value(pu8RegTable, H_BK_VE_ENC(0x59), &u8RegValue_H))
    {
        u16TtxRange_OddEnd = u8RegValue_L + (u8RegValue_H << 8);
    }
    else
    {
        if(MS_VE_PAL == Hal_VE_Get_Output_Video_Std())
        {
            u16TtxRange_OddEnd = 22;
        }
        else
        {
            u16TtxRange_OddEnd = 19;
        }
    }

    // Even start
    if(_Hal_VE_get_reg_table_value(pu8RegTable, L_BK_VE_ENC(0x5A), &u8RegValue_L) && _Hal_VE_get_reg_table_value(pu8RegTable, H_BK_VE_ENC(0x5A), &u8RegValue_H))
    {
        u16TtxRange_EvenStart = u8RegValue_L + (u8RegValue_H << 8);
    }
    else
    {
        if(MS_VE_PAL == Hal_VE_Get_Output_Video_Std())
        {
            u16TtxRange_EvenStart = 319;
        }
        else
        {
            u16TtxRange_EvenStart = 273;
        }
    }

    // Even end
    if(_Hal_VE_get_reg_table_value(pu8RegTable, L_BK_VE_ENC(0x5B), &u8RegValue_L) && _Hal_VE_get_reg_table_value(pu8RegTable, H_BK_VE_ENC(0x5B), &u8RegValue_H))
    {
        u16TtxRange_EvenEnd = u8RegValue_L + (u8RegValue_H << 8);
    }
    else
    {
        if(MS_VE_PAL == Hal_VE_Get_Output_Video_Std())
        {
            u16TtxRange_EvenEnd = 335;
        }
        else
        {
            u16TtxRange_EvenEnd = 282;
        }
    }

    // VBI TTX active lines
    if(_Hal_VE_get_reg_table_value(pu8RegTable, L_BK_VE_ENC(0x3C), &u8RegValue_L) && _Hal_VE_get_reg_table_value(pu8RegTable, H_BK_VE_ENC(0x3C), &u8RegValue_H))
    {
        u32TtxActiveLines = u8RegValue_L + (u8RegValue_H << 8);

        if(_Hal_VE_get_reg_table_value(pu8RegTable, L_BK_VE_ENC(0x3D), &u8RegValue_L) && _Hal_VE_get_reg_table_value(pu8RegTable, H_BK_VE_ENC(0x3D), &u8RegValue_H))
        {
            u32TtxActiveLines |= (u8RegValue_L + (u8RegValue_H << 8)) << 16;
        }
        else
        {
            u32TtxActiveLines = 0x3FFFF;
        }
    }
    else
    {
        u32TtxActiveLines = 0x3FFFF;
    }

    // Restore VBI TTX range
    MDrv_Write2Byte(L_BK_VE_ENC(0x58), u16TtxRange_OddStart);
    MDrv_Write2Byte(L_BK_VE_ENC(0x59), u16TtxRange_OddEnd);
    MDrv_Write2Byte(L_BK_VE_ENC(0x5A), u16TtxRange_EvenStart);
    MDrv_Write2Byte(L_BK_VE_ENC(0x5B), u16TtxRange_EvenEnd);

    // Restore VBI TTX active lines
    MDrv_Write2Byte(L_BK_VE_ENC(0x3C), u32TtxActiveLines);
    MDrv_Write2Byte(L_BK_VE_ENC(0x3D), u32TtxActiveLines >> 16);
}

#ifndef MSOS_TYPE_OPTEE
static void _Hal_VE_dolby_verifier_task(void)
{
    MS_U32 u32SystemTime = 0;
    MS_U8 au8AuthInfo[VE_DOLBY_VERIFIER_HASH_INFO_LENGTH] = {0};
    int i;

    u32SystemTime = MsOS_GetSystemTime();

    while(_enVE_DolbyVerifierStatus == EN_VE_DOLBY_VERIFIER_ONGOING)
    {
        MS_U8 u8Count = 0;

        Hal_VE_Clear_ttxReadDoneStatus();

        memset((void*)au8AuthInfo, 0x0, VE_DOLBY_VERIFIER_HASH_INFO_LENGTH);
        MDrv_AUTH_GetHashInfo(au8AuthInfo);
        memset((void*)_pu8DolbyVerifierBufVA, 0x0, TTX_PACKET_SIZE);

        *(_pu8DolbyVerifierBufVA+6) = 0x36; // Packet header
        *(_pu8DolbyVerifierBufVA+7) = 0x97;
        *(_pu8DolbyVerifierBufVA+8) = 0x00;
        *(_pu8DolbyVerifierBufVA+9) = 0x01; // Packet index

        for(i=0; i<VE_DOLBY_VERIFIER_HASH_INFO_LENGTH; i++)
        {
            *(_pu8DolbyVerifierBufVA+(i+10)) = au8AuthInfo[i];
        }

        MsOS_FlushMemory(); // Flush L2 cache

        //Enable next read
        if (MDrv_Read2Byte(L_BK_VE_SRC(0x43))&0x0002)
        {
            MDrv_Write2Byte(L_BK_VE_SRC(0x43),MDrv_Read2Byte(L_BK_VE_SRC(0x43))&0xfffd);
        }
        else
        {
            MDrv_Write2Byte(L_BK_VE_SRC(0x43),MDrv_Read2Byte(L_BK_VE_SRC(0x43))|0x0002);
        }

        MsOS_DelayTask(VE_DOLBY_VERIFIER_DELAY);

        while(!Hal_VE_Get_ttxReadDoneStatus())
        {
            if(u8Count >= TTX_CHECK_DONE_MAX_COUNT)
            {
                VE_DBG(printf("Waiting for TTX done timeout\n"));
                break;
            }

            MsOS_DelayTask(VE_DOLBY_VERIFIER_DELAY);
            u8Count++;
        }

        if(MsOS_Timer_DiffTimeFromNow(u32SystemTime) >= VE_DOLBY_VERIFIER_TIMER)
        {
            _enVE_DolbyVerifierStatus = EN_VE_DOLBY_VERIFIER_DONE;
            MDrv_WriteByteMask(L_BK_VE_ENC(0x2E), 0x00, BIT(3));
            _Hal_VE_restore_orig_ttx_setting();

            VE_DBG(printf("Exit TTXDolby_Verifier\n"));
        }
    }
}
#endif
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_init()
/// @brief \b Function \b Description : Initiate VE
///
/// @param <IN>        \b None :
/// @param <OUT>       \b None :
/// @param <RET>       \b None :
/// @param <GLOBAL>    \b None :
////////////////////////////////////////////////////////////////////////////////
void Hal_VE_init(void)
{
    MS_U8 au8AuthInfo[VE_DOLBY_VERIFIER_HASH_INFO_LENGTH] = {0};
    int i;

    MDrv_WriteByte(L_BK_CHIPTOP(0x24), 0x00);   // clock of ve & vedac
    MDrv_WriteByte(H_BK_CHIPTOP(0x24), 0x08);   // clock of ve & vedac

    MDrv_WriteByteMask(L_BK_VE_ENC(0x03),0x00,0x10);

    // read FIFO control
    MDrv_WriteByte(L_BK_VE_SRC(0x32),0X30);
    MDrv_WriteByte(L_BK_VE_SRC(0x37),0X0b);
    MDrv_WriteByte(H_BK_VE_SRC(0x37),0X0c);

    // initial TTX VBI
    MDrv_Write2Byte(L_BK_VE_SRC(0x06), 0x00);
    MDrv_Write2ByteMask(L_BK_VE_SRC(0x43), 0x00B9, 0x01FD); // Set VBI TTX mode
    MDrv_WriteByteMask(L_BK_VE_SRC(0x46), BIT(1), BIT(1)|BIT(0)); // Set reg_vbi_en_md
    MDrv_WriteByteMask(L_BK_VE_SRC(0x35), BIT(7), BIT(7)); // Set vbi_rd_mpd
    MDrv_Write2ByteMask(L_BK_VE_SRC(0x08), 0x0000, BIT(10)); // Disable TTX interrupt mask
    MDrv_WriteByte(L_BK_VE_ENC(0x07), 0x00);
    MDrv_WriteByteMask(L_BK_VE_SRC(0x00), BIT(3), BIT(3)); // Enable VBI

    //test for VE output stable temp solution,2007/07/05
    MDrv_WriteByte(L_BK_VE_SRC(0x44),0xf0);
    MDrv_WriteByteMask(L_BK_VE_SRC(0x46),0xd0,0xf0);

    MDrv_WriteRegBit(H_BK_VE_SRC(0x7E), 1, BIT(0)); //enable black boundary

    MDrv_Write2Byte(L_BK_VE_SRC(0x11), 0x0080); //default VE Hardware debug mode VE ToDo
    MDrv_WriteRegBit(L_BK_VE_SRC(0x5A), DISABLE, BIT(0));// disable scaler in

    MDrv_WriteByte(L_BK_VE_SRC(0x27),0xB5);

    //tvs & tve field need sync at fbl mode
    MDrv_WriteByteMask(L_BK_VE_SRC(0x0B),0x08,BIT(3));

    //sync delay pipe from write clock to read clock domain (add 2 pipe flip-flop)
    MDrv_WriteByteMask(L_BK_VE_SRC(0x7E),0x02,BIT(1));

    // enable new FRC control mode
    MDrv_WriteRegBit(L_BK_VE_SRC(0x5C), BIT(3), BIT(3));
    // enable VE scaling mode
    MDrv_WriteRegBit(L_BK_VE_SRC(0x5C), BIT(2), BIT(2));

    //Change default VE Video/GOP layer switch value to be FrameColor->Video->osd->osd1
    MDrv_WriteByteMask(L_BK_VE_SRC(0x5f),0x1,BIT(0)|BIT(1)|BIT(2));

    //Jitter adjust
    MApi_XC_WriteByte(0x103116, 0x54);
    MApi_XC_WriteByte(0x103117, 0x54);

    // Enable upsample filter
    // reg_en_hb_lpf_y_x2, bit12
    // reg_en_rgb_x4_lpf, bit13
    MDrv_Write2Byte(L_BK_VE_ENC(0x02), 0x3000);

    //Init cus table
    memset(&_gpu8CusTable, 0xff, sizeof(_gpu8CusTable));

#ifndef CONFIG_MBOOT
    if(_phyDolbyVerifierBufAddr != 0)
    {
        MS_BOOL bPreVEDisableRegWrite = g_bVeDisableRegWrite;

        g_bVeDisableRegWrite = FALSE; // Tempararily enable VE register write for TTX setting

        VE_DBG(printf("Set Dolby single part name control, buf addr=0x%lx\n", _phyDolbyVerifierBufAddr));

        _phyDolbyVerifierBufAddr = ( ((_phyDolbyVerifierBufAddr) + ((VE_MIU_WIDTH)-1)) & ~((VE_MIU_WIDTH)-1) ); //alignment

        if(1 == _u8VE_MIUSel)
        {
            _pu8DolbyVerifierBufVA = (MS_U8*)MsOS_PA2KSEG1(_phyDolbyVerifierBufAddr + HAL_MIU1_BASE);
        }
        else
        {
            _pu8DolbyVerifierBufVA = (MS_U8*)MsOS_PA2KSEG1(_phyDolbyVerifierBufAddr);
        }

        if(_pu8DolbyVerifierBufVA == NULL)
        {
            #define MPOOL_MAPPING_ALIGNED_SIZE		0x1000

            if(1 == _u8VE_MIUSel)
            {

                if (!MsOS_MPool_Mapping(1, (_phyDolbyVerifierBufAddr & 0xFFFFF000) ,  MPOOL_MAPPING_ALIGNED_SIZE , 1))
                {
                    printf("---%s:%d error, map fail\n", __FUNCTION__, __LINE__);
                    return;
                }

                _pu8DolbyVerifierBufVA = (MS_U8*)MsOS_PA2KSEG1(_phyDolbyVerifierBufAddr + HAL_MIU1_BASE);
            }
            else
            {

                if (!MsOS_MPool_Mapping(0, (_phyDolbyVerifierBufAddr & 0xFFFFF000),  MPOOL_MAPPING_ALIGNED_SIZE , 1))
                {
                    printf("---%s:%d error, map fail\n", __FUNCTION__, __LINE__);
                    return;
                }

                _pu8DolbyVerifierBufVA = (MS_U8*)MsOS_PA2KSEG1(_phyDolbyVerifierBufAddr);
            }

        }

        memset((void*)_pu8DolbyVerifierBufVA, 0x0, TTX_PACKET_SIZE);
#ifndef MSOS_TYPE_OPTEE
        MDrv_AUTH_GetHashInfo(au8AuthInfo);
#endif
        *(_pu8DolbyVerifierBufVA+6) = 0x36; // Packet header
        *(_pu8DolbyVerifierBufVA+7) = 0x97;
        *(_pu8DolbyVerifierBufVA+8) = 0x00;
        *(_pu8DolbyVerifierBufVA+9) = 0x01; // Packet index

        for(i=0; i<VE_DOLBY_VERIFIER_HASH_INFO_LENGTH; i++)
        {
            *(_pu8DolbyVerifierBufVA+(i+10)) = au8AuthInfo[i];
        }

        if(MS_VE_PAL == Hal_VE_Get_Output_Video_Std())
        {
            Hal_VE_SetVbiTtxRange(7, 22, 319, 335); // From VBI setting table
            Hal_VE_Set_VbiTtxActiveLinesBitmap(0x2000); // Open line 20
        }
        else // NTSC
        {
            Hal_VE_SetVbiTtxRange(10, 19, 273, 282); // From VBI setting table
            Hal_VE_Set_VbiTtxActiveLinesBitmap(0x0200); // Open line 19
        }

        Hal_VE_Set_ttx_Buffer(((MS_VIRT)(_phyDolbyVerifierBufAddr)), TTX_PACKET_SIZE);
        MDrv_WriteByteMask(L_BK_VE_ENC(0x2E), BIT(7)|BIT(6)|BIT(3), BIT(7)|BIT(6)|BIT(3)); //Set VBI Mode to enable TTx
#ifndef MSOS_TYPE_OPTEE
        _enVE_DolbyVerifierStatus = EN_VE_DOLBY_VERIFIER_ONGOING;

        // Init task for Dolby verifier
        if (_s32VE_DolbyVerifier_TaskId == -1)
        {
            _s32VE_DolbyVerifier_TaskId = MsOS_CreateTask((TaskEntry)_Hal_VE_dolby_verifier_task,
                                                          (MS_VIRT)NULL,
                                                          E_TASK_PRI_MEDIUM,
                                                          TRUE,
                                                          (void *)_VE_DolbyVerifier_Stack,
                                                          VE_DOLBY_VERIFIER_STACK_SIZE,
                                                          "VE DOLBY");
            if(_s32VE_DolbyVerifier_TaskId == -1)
            {
                printf("Create Dolby single part name task failed!!\n");
            }
        }
#endif
        g_bVeDisableRegWrite = bPreVEDisableRegWrite;
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_CCIR_PAD_Sel()
/// @brief \b Function \b Description : Initiate PAD for VE CCIR output
///
/// @param <IN>        \b None :
/// @param <OUT>       \b None :
/// @param <RET>       \b None :
/// @param <GLOBAL>    \b None :
////////////////////////////////////////////////////////////////////////////////
void Hal_VE_CCIR_PAD_Sel(void)
{
    //Designer recommend to assign PAD config in MBOOT, since it need config only one time
    //So below code is used to test CCIR when MBOOT not ready for this

	//In K2, defaultly set CCIR Mode1 as output mode
    MDrv_WriteByte(L_BK_CHIP(0x02), 0x00);  // reg_ts1_mode [6:4]
    MDrv_WriteByte(L_BK_CHIP(0x09), 0x00);  // i2cm1
    MDrv_WriteByte(H_BK_CHIP(0x50), 0x00);  // reg_allpad_in
    MDrv_WriteByte(L_BK_CHIP(0x08), 0x10);  // CCIR output mode1
    MDrv_WriteByte(L_BK_CHIP(0x1f), 0x03);  // [1:0] 1:from disp_misc, 3:from ve //[3]:bit swap
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_Get_Output_Video_Std()
/// @brief \b Function \b Description : Get the output video standard of video
///                                     encoder
///
/// @param <IN>        \b None :
/// @param <OUT>       \b None :
/// @param <RET>       \b VideoSystem : the video standard
/// @param <GLOBAL>    \b None :
////////////////////////////////////////////////////////////////////////////////
MS_VE_VIDEOSYS Hal_VE_Get_Output_Video_Std(void)
{
    if(MDrv_Read2Byte(L_BK_VE_SRC(0x42)) == 0x1e0)
    {
        //n
        return MS_VE_NTSC;
    }
    else
    {
        //p
        return MS_VE_PAL;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_set_output_video_std()
/// @brief \b Function \b Description : Set the output video standard of video
///                                     encoder
///
/// @param <IN>        \b VideoSystem : the video standard
/// @param <OUT>       \b None :
/// @param <RET>       \b MS_BOOL : TRUE : supported and success,
///                                 FALSE: unsupported or unsuccess
/// @param <GLOBAL>    \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL Hal_VE_set_output_video_std(MS_VE_VIDEOSYS VideoSystem)
{
    MS_BOOL bRet = FALSE;
    PMS_VE_Out_VideoSYS pVideoSysTbl = NULL;

    if(VideoSystem >= MS_VE_VIDEOSYS_NUM)
    {
        bRet = FALSE;
        return bRet;
    }
    else
    {
        bRet = TRUE;
        pVideoSysTbl = &VE_OUT_VIDEOSTD_TBL[VideoSystem];
    }

    if(bRet)
    {
        //return FALSE if chip does not support selected output type
        if(pVideoSysTbl->pVE_TBL == NULL)
        {
            return FALSE;
        }

        Hal_VE_WriteRegTbl(pVideoSysTbl->pVE_TBL);
        Hal_VE_WriteRegTbl(pVideoSysTbl->pVE_Coef_TBL);
        Hal_VE_WriteRegTbl(pVideoSysTbl->pVBI_TBL);

        MDrv_WriteRegBit(L_BK_VE_ENC(0x03), pVideoSysTbl->bvtotal_525, BIT(3)); // vtotal
        MDrv_WriteRegBit(L_BK_VE_ENC(0x06), pVideoSysTbl->bPALSwitch, BIT(0));  // Palswitch

        if(_gbUseCusTable)
        {
            MS_U8* u8tbl = _gpu8CusTable[VideoSystem];
            Hal_VE_WriteRegTbl(u8tbl);
        }

        MDrv_WriteRegBit(L_BK_VE_SRC(0x00), TRUE, BIT(5)); // load register
        MDrv_WriteRegBit(L_BK_VE_SRC(0x00), FALSE, BIT(5));

        if(_enVE_DolbyVerifierStatus == EN_VE_DOLBY_VERIFIER_ONGOING)
        {
            if(VideoSystem <= MS_VE_PAL_M) // 480i
            {
                MDrv_Write2Byte(L_BK_VE_ENC(0x3C), 0x0200); // Open line 19
                MDrv_Write2Byte(L_BK_VE_ENC(0x3D), 0);
            }
            else // 576i
            {
                MDrv_Write2Byte(L_BK_VE_ENC(0x3C), 0x2000); // Open line 20
                MDrv_Write2Byte(L_BK_VE_ENC(0x3D), 0);
            }
        }
    }

    /* trun on/off SECAM enable bit */
    if (VideoSystem != MS_VE_SECAM) {
        /* NOT SECAM */
        MDrv_WriteRegBit(L_BK_VE_ENC_EX(0x10), FALSE, BIT(0));
    } else {
        /* SECAM */
        MDrv_WriteRegBit(L_BK_VE_ENC_EX(0x10), TRUE, BIT(0));
    }

    //TVE software reset
    Hal_VE_sofeware_reset(ENABLE); // software reset
    Hal_VE_sofeware_reset(DISABLE); // software reset

    return bRet;
}

//------------------------------------------------------------------------------
/// Load customized setting table for VE
/// User can load customized table in initialize function, then enable it.
/// Driver will apply customized table setting when MDrv_VE_SetOutputVideoStd
/// Note: Users set value which want to be changed only.
///
/// @param  -VideoSystem \b IN: the video standard
/// @param  -pTbl \b IN: pointer to the table
/// @param  -u16size \b IN: size of table
/// @return TRUE: supported and success,  FALSE: unsupported or unsuccess
//------------------------------------------------------------------------------
MS_BOOL Hal_VE_SetCusTable(MS_VE_VIDEOSYS VideoSystem, MS_U8* pu8Tbl, MS_U16 u16size)
{
    if (pu8Tbl == NULL)
    {
        VE_DBG(printf("[%s] pTbl error\n.", __FUNCTION__));
        return FALSE;
    }

    if (u16size > VE_CUSTBL_MAX_SIZE)
    {
        VE_DBG(printf("[%s] pTbl size error\n.", __FUNCTION__));
        return FALSE;
    }

    memcpy(_gpu8CusTable[VideoSystem],pu8Tbl,u16size);

    return TRUE;
}

//------------------------------------------------------------------------------
/// Enable/Disable customized table
///
/// @param  -pTbl \b IN: pointer to the table
//------------------------------------------------------------------------------
void Hal_VE_EnableCusTable(MS_BOOL bEnable)
{
    _gbUseCusTable = bEnable;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_set_wss_data()
/// @brief \b Function \b Description : Set the WSS data of video encoder
///
/// @param <IN>        \b bEn       : Enable, Disable
/// @param <IN>        \b u8WSSData : The WSS data
/// @param <OUT>       \b None :
/// @param <RET>       \b None :
/// @param <GLOBAL>    \b None :
////////////////////////////////////////////////////////////////////////////////
void Hal_VE_set_wss_data(MS_BOOL ben, MS_U16 u16wssdata)
{
    if(ben)
    {
        MDrv_Write2Byte(L_BK_VE_ENC(0x3B), (u16wssdata & 0x3FFF));

        if(MDrv_Read2Byte(L_BK_VE_SRC(0x42)) == 0x1e0)    //NTSC & PAL-M
        {
            printf("[%s][%d] ERROR: this function only for 625i System! \n", __FUNCTION__, __LINE__);
            return;
        }
        else    //PAL
        {
            MDrv_Write2Byte(L_BK_VE_ENC(0x3B), (u16wssdata & 0x3FFF));
            MDrv_Write2Byte(L_BK_VE_ENC(0x56), 0x0017);
            MDrv_Write2Byte(L_BK_VE_ENC(0x57), 0x0017);
            MDrv_Write2Byte(L_BK_VE_ENC(0x6C), 0x0000);
            MDrv_Write2Byte(L_BK_VE_ENC(0x6D), 0x0000);
        }
        MDrv_WriteByteMask(L_BK_VE_ENC(0x2E), BIT(7)|BIT(2), BIT(7)|BIT(2));
    }
    else
    {
        MDrv_WriteByteMask(L_BK_VE_ENC(0x2E), 0x00, BIT(2));
    }
}

MS_U16 Hal_VE_get_wss_data(void)
{
  return (MDrv_Read2Byte(L_BK_VE_ENC(0x3B)) & 0x3FFF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_set_fix_color_out()
/// @brief \b Function \b Description : Set VE fix color out
///
/// @param <IN>        \b pInfo : the information of fix color out
/// @param <OUT>       \b None :
/// @param <RET>       \b None :
/// @param <GLOBAL>    \b None :
////////////////////////////////////////////////////////////////////////////////
void Hal_VE_set_fix_color_out(MS_BOOL ben, MS_U8 u8color_y, MS_U8 u8color_cb, MS_U8 u8color_cr)
{
    if(ben)
    {
        MDrv_WriteByteMask(L_BK_VE_SRC(0x38), u8color_y << 7, 0x7F80);
        MDrv_Write2Byte(L_BK_VE_SRC(0x39), ((u8color_cb<<8) | u8color_cr));
    }

    MDrv_WriteRegBit(L_BK_VE_SRC(0x38), ben, BIT(6));
}

#if 0
//------------------------------------------------------------------------------
/// Adjust contrast
/// @param  -u8Constrast \b IN: adjusting value for contrast, range:0~FFh
/// @return MS_MTSTATUS
//------------------------------------------------------------------------------

void MDrv_VE_Adjust_Contrast(MS_U8 u8Constrast)
{
    MDrv_WriteByte(L_BK_VE_ENC(0x04), u8Constrast);
}


//------------------------------------------------------------------------------
/// Adjust Hue
/// @param  -u8Hue \b IN: adjusting value for Hue, range:0~FFh
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_Adjust_Hue(MS_U8 u8Hue)
{
    MDrv_WriteByte(H_BK_VE_ENC(0x05), u8Hue);
}


//------------------------------------------------------------------------------
/// Adjust saturation
///
/// @param  -u8Saturation \b IN: adjusting value for saturation, range:0~FFh
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_Adjust_Saturation(MS_U8 u8USaturation, MS_U8 u8VSaturation)
{
    MDrv_WriteByte(L_BK_VE_ENC(0x2A), u8USaturation);
    MDrv_WriteByte(H_BK_VE_ENC(0x2A), u8VSaturation);
}


//------------------------------------------------------------------------------
/// Adjust brightness
///
/// @param  -u8brightness \b IN: adjusting value for brightness, range:0~FFh
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_Adjust_Brightness(MS_U8 u8brightness)
{
    MDrv_WriteByte(H_BK_VE_ENC(0x04), u8brightness);
}


//------------------------------------------------------------------------------
/// VE power trun on
///
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_PowerOn(void)
{
    MDrv_Power_Set_HwClock(E_HWCLK_VEIN_ONOFF, POWER_ON);
    MDrv_Power_Set_HwClock(E_HWCLK_VE_ONOFF, POWER_ON);
    MDrv_Power_Set_HwClock(E_HWCLK_VEVE_ONOFF, POWER_ON);
    MDrv_WriteByteMask(L_BK_DAC(0x00), 0x8F, 0x8F);
}


//------------------------------------------------------------------------------
/// VE power trun off
///
/// @return none
//------------------------------------------------------------------------------
void MDrv_VE_PowerOff(void)
{
    MDrv_Power_Set_HwClock(E_HWCLK_VEIN_ONOFF, POWER_DOWN);
    MDrv_Power_Set_HwClock(E_HWCLK_VE_ONOFF, POWER_DOWN);
    MDrv_Power_Set_HwClock(E_HWCLK_VEVE_ONOFF, POWER_DOWN);
    MDrv_WriteByteMask(L_BK_DAC(0x00), 0x00, 0x8F);
}
#endif


void Hal_VE_set_capture_window(MS_U16 u16hstart, MS_U16 u16hend, MS_U16 u16vstart, MS_U16 u16vend)
{
    MDrv_Write2Byte(L_BK_VE_SRC(0x12), u16hstart);
    MDrv_Write2Byte(L_BK_VE_SRC(0x13), u16hend);
    MDrv_Write2Byte(L_BK_VE_SRC(0x14), u16vstart);
    MDrv_Write2Byte(L_BK_VE_SRC(0x15), u16vend);
}

void Hal_VE_set_hsync_inverse(MS_BOOL bEn)
{
    MDrv_WriteRegBit(L_BK_VE_SRC(0x41), bEn, BIT(0));
}

void Hal_VE_set_cvbs_buffer_out(MS_U16 u16chnl, MS_U16 u16mux, MS_U16 u16clamp, MS_U16 u16test)
{
    MS_U8 u8Adc38L = u16chnl & 0xFF;
    MS_U8 u8Adc39L = u16mux & 0xFF;
    MS_U8 u8Adc39H = (u16mux >> 8);
    MS_U8 u8Adc3AL = u16clamp & 0xFF;
    MS_U8 u8Adc3BH = u16test & 0xFF;


    MDrv_WriteByte(REG_ADC_ATOP_38_L, u8Adc38L);
    MDrv_WriteByte(REG_ADC_ATOP_39_L, u8Adc39L);
    MDrv_WriteByte(REG_ADC_ATOP_39_H, u8Adc39H);
    MDrv_WriteByte(REG_ADC_ATOP_3A_L, u8Adc3AL);
    MDrv_WriteByte(REG_ADC_ATOP_3B_H, u8Adc3BH);
}

void Hal_VE_set_frc(MS_BOOL bfulldrop, MS_U32 u32fullnum, MS_U32 u32emptynum, MS_BOOL binterlace, MS_BOOL b3FrameMode)
{
    MDrv_Write4Byte(L_BK_VE_SRC(0x21),  u32fullnum);

    if(bfulldrop)
        MDrv_WriteRegBit(L_BK_VE_SRC(0x20), 0, BIT(0)); // enable Full drop
    else
        MDrv_WriteRegBit(L_BK_VE_SRC(0x20), 1, BIT(0)); // disable Full drop

    MDrv_Write4Byte(L_BK_VE_SRC(0x30),  u32emptynum);

    //U4 Always use normal mode (disable de-interlace)
    MDrv_WriteByteMask(L_BK_VE_SRC(0x35), 0x00, BIT(3)|BIT(2));
}

void Hal_VE_set_field_size(MS_U16 u16FieldSize)
{
    MDrv_Write2Byte(L_BK_VE_SRC(0x45), u16FieldSize);
}

void Hal_VE_set_field_inverse(MS_BOOL ben)
{
    if(!MDrv_ReadRegBit(L_BK_VE_SRC(0x10), BIT(4)))
    {
        MDrv_WriteRegBit(L_BK_VE_SRC(0x10), ben, BIT(1));
    }
    else
    {
        //CCIR ignore field inverse
        MDrv_WriteRegBit(L_BK_VE_SRC(0x10), 0, BIT(1));
    }
}

void Hal_VE_set_ccir656_out_pal(MS_BOOL bpal)
{
    if(bpal)
    {
        MDrv_WriteRegBit(L_BK_VE_SRC(0x40), 1, BIT(0));
        Hal_VE_WriteRegTbl(tVE_CCIROUT_PAL_TBL);
    }
    else
    {
        MDrv_WriteRegBit(L_BK_VE_SRC(0x40), 0, BIT(0));
        Hal_VE_WriteRegTbl(tVE_CCIROUT_NTSC_TBL);
    }
}

void Hal_VE_sofeware_reset(MS_BOOL ben)
{
    MDrv_WriteRegBit(L_BK_VE_SRC(0x00), ben, BIT(4));
}

void Hal_VE_set_vbi(MS_BOOL ben)
{
    MDrv_WriteRegBit(L_BK_VE_SRC(0x00), ben, BIT(3)); // load register,but not affect bit3(VBI output)

}

void Hal_VE_set_reg_load(MS_BOOL ben)
{
    MDrv_WriteRegBit(L_BK_VE_SRC(0x00), ben, BIT(5));
}

void Hal_VE_set_ctrl(MS_U16 u16enval)
{
    MDrv_WriteByteMask(L_BK_VE_SRC(0x00), u16enval, BIT(7)|BIT(6)|BIT(5)|BIT(4)|BIT(2)|BIT(1));
}

void Hal_VE_set_h_scaling(MS_BOOL ben, MS_U16 u16ratio)
{
     MDrv_Write2Byte(L_BK_VE_SRC(0x16), u16ratio);
     if(ben)
        MDrv_WriteRegBit(H_BK_VE_SRC(0x00), 0, BIT(0));
     else
        MDrv_WriteRegBit(H_BK_VE_SRC(0x00), 1, BIT(0));
}

void Hal_VE_set_h_upscaling(MS_BOOL ben, MS_U32 u32ratio)
{
    // set h upscale ratio
    MDrv_Write2Byte(L_BK_VE_DISC(0x66), (u32ratio >> 16));
    MDrv_Write2Byte(L_BK_VE_DISC(0x65), (u32ratio & 0xFFFF));

    // enable/disable upscale
    if(ben) {
        MDrv_WriteRegBit(L_BK_VE_SRC(0x5C), BIT(2), BIT(2));
    } else {
        MDrv_WriteRegBit(L_BK_VE_SRC(0x5C), 0x0, BIT(2));
    }
}

void Hal_VE_set_v_scaling(MS_BOOL ben, MS_U16 u16ratio)
{
    MDrv_Write2Byte(L_BK_VE_SRC(0x17), u16ratio);
    if(ben)
        MDrv_WriteRegBit(H_BK_VE_SRC(0x00), 0, BIT(1));
    else
        MDrv_WriteRegBit(H_BK_VE_SRC(0x00), 1, BIT(1));
}

void Hal_VE_set_v_upscaling(MS_BOOL ben, MS_U32 u32ratio)
{
    // set h upscale ratio
    if (bIs_DIEnabled) {
        u32ratio = u32ratio << 1;  // scale down 1/2x if DI is enabled
    }
    MDrv_Write2Byte(L_BK_VE_DISC(0x62), (u32ratio >> 16));
    MDrv_Write2Byte(L_BK_VE_DISC(0x61), (u32ratio & 0xFFFF));

    // enable/disable upscale
    if(ben) {
        MDrv_WriteRegBit(L_BK_VE_SRC(0x5C), BIT(2), BIT(2));
    } else {
        MDrv_WriteRegBit(L_BK_VE_SRC(0x5C), 0x0, BIT(2));
    }
}

void Hal_VE_set_v_scaling_Traditional(MS_BOOL ben, MS_U16 u16ratio)
{
    Hal_VE_set_v_scaling(ben, u16ratio);
}

void Hal_VE_set_out_sel(MS_U8 u8out_sel)
{
    MDrv_WriteByteMask(L_BK_VE_ENC(0x40), u8out_sel, BIT(1)|BIT(0));

}

void Hal_VE_set_source_sync_inv(MS_U8 u8val, MS_U8 u8Mask)
{
    MDrv_WriteByteMask(L_BK_VE_SRC(0x41), u8val, u8Mask);
}

void Hal_VE_set_sog(MS_BOOL ben)
{
    MDrv_WriteRegBit(L_BK_VE_SRC(0x7E), ben, BIT(4));
}

void Hal_VE_set_mux(MS_U8 u8clk, MS_U8 u8data)
{
    //MDrv_WriteByte(L_BK_IPMUX(0x02), u8data); // input select --- no define in U4/K1
    MDrv_WriteByte(L_BK_CHIPTOP(0x25), u8clk);  //idclk
}

//------------------------------------------------------------------------------
/// Enable DI - enable de-interlace function for DNR2VE path if necessary (interlaced input)
///
/// @param  bEnable: enable DI or disable DI
/// @return VE_Result
//------------------------------------------------------------------------------
static VE_Result Hal_VE_EnableDI(MS_BOOL bEnable)
{
    MS_BOOL bIsDNR2VE = FALSE;
    if (((BIT(0) | BIT(1)) & MDrv_ReadByte(L_BK_VE_SRC(0x0B))) == (BIT(0) | BIT(1))) {
        bIsDNR2VE = TRUE;
    }
    printf("[%s][%d] bEnable = %d, bIsDNR2VE = %d\n", __FUNCTION__, __LINE__,
            bEnable, bIsDNR2VE);
    if((bEnable) && (bIsDNR2VE)) {
        // change DRAM format to Y8C8M4
        Hal_VE_Set_DRAM_Format(MS_VE_DRAM_FORMAT_Y8C8M4);
        // set write field size
        //field size formula: filed_size= width * line_per_field/ Pixel_per_pack
        //Pixel_per_pack = MIU_access_unit / bits_per_pixel

        if ( MDrv_Read2Byte( L_BK_VE_SRC(0x42) ) == 0x1E0/*NTSC*/ )
        {
            //NTSC
            // 720* 240 / (64 / 20) = 69120 = 0xE100
            Hal_VE_set_field_size(0xE100);
        }
        else
        {
            //PAL
            //After K2 U00, field size extend to 17 bits
            // 720* 288 / (64 / 20) = 69120 = 0x10E00
            Hal_VE_set_field_size(0x0E00);
            MDrv_WriteRegBit(H_BK_VE_SRC(0x46), BIT(0), BIT(0));
        }

        Hal_VE_Set_Pack_Num_Per_Line(0xEF); // 720 - 1 in Y8C8M4
        // set MIU control mode for DI - read two field
        MDrv_WriteRegBit(L_BK_VE_SRC(0x5C), BIT(1), BIT(1));
        // enable MADi
        MDrv_WriteRegBit(L_BK_VE_DISC(0x58), BIT(0), BIT(0));
        // set 422 to 444 conversion method
        MDrv_WriteByteMask(L_BK_VE_DISC(0x60), 0x00, BIT(1)|BIT(0));
        bIs_DIEnabled = TRUE;
    } else {
        // change DRAM format to Y8C8
        Hal_VE_Set_DRAM_Format(MS_VE_DRAM_FORMAT_Y8C8);
        // set write field size
        Hal_VE_set_field_size((MDrv_Read2Byte(L_BK_VE_SRC(0x42)) == 0x1E0/*NTSC*/)?
                0xA8C0: 0xCA80);
        Hal_VE_Set_Pack_Num_Per_Line(0xB3); // 720 - 1 in Y8C8
        // set MIU control mode for DI - read one field
        MDrv_WriteRegBit(L_BK_VE_SRC(0x5C), 0x0, BIT(1));
        // disable MADi
        MDrv_WriteRegBit(L_BK_VE_DISC(0x58), 0x0, BIT(0));
        // set 422 to 444 conversion method to default value
        //MDrv_WriteByteMask(L_BK_VE_DISC(0x60), BIT(1) | BIT(0), BIT(1)|BIT(0));
        bIs_DIEnabled = FALSE;
    }
    return E_VE_OK;
}

void Hal_VE_set_inputsource(PMS_VE_InputSrc_Info pInputSrcInfo)
{
    MS_U8 u8Clk_Mux = 0, u8Data_Mux = 0;

    switch(pInputSrcInfo->eInputSrcType)
    {
        case MS_VE_SRC_DTV:
        case MS_VE_SRC_SUB:
        case MS_VE_SRC_DTV_FROM_MVOP:
        case MS_VE_SRC_DSUB:
        case MS_VE_SRC_COMP:
        case MS_VE_SRC_ATV:
        case MS_VE_SRC_CVBS0:
        case MS_VE_SRC_CVBS1:
        case MS_VE_SRC_CVBS2:
        case MS_VE_SRC_CVBS3:
        case MS_VE_SRC_SVIDEO:
        case MS_VE_SRC_HDMI_A:
        case MS_VE_SRC_HDMI_B:
        case MS_VE_SRC_HDMI_C:
        case MS_VE_SRC_DI:
        case MS_VE_SRC_DNR:
        case MS_VE_SRC_BT656:
        default:
            // Not support in Kano
        case MS_VE_SRC_SCALER: //OP2
            u8Clk_Mux  = CKG_VE_IN_ODCLK;
            MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x7C), BIT(1)|BIT(0), BIT(1)|BIT(0)); // Set SC1_top to TVE path
            MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x7C), 0x80, 0xF0); // Enable SW delay and set delay to 0, patch for redundant design
            break;
    }

    Hal_VE_set_mux(u8Clk_Mux, u8Data_Mux);
}

void Hal_VE_set_rgb_in(MS_BOOL ben)
{
    MDrv_WriteRegBit(L_BK_VE_SRC(0x10), ben, BIT(6));
}

void Hal_VE_set_ccir656_in(MS_BOOL ben)
{
    if(ben)
        MDrv_WriteByteMask(L_BK_VE_SRC(0x10), BIT(4), BIT(5)|BIT(4)); // video source isCCIR656
    else
        MDrv_WriteByteMask(L_BK_VE_SRC(0x10), 0, BIT(5)|BIT(4)); // video source is YCbCr
}

void Hal_VE_set_source_interlace(MS_BOOL ben)
{
    if(ben)
        MDrv_WriteRegBit(L_BK_VE_SRC(0x10), 1, BIT(0));
    else
        MDrv_WriteRegBit(L_BK_VE_SRC(0x10), 0, BIT(0));

    Hal_VE_EnableDI(ben);
}

void Hal_VE_set_clk_on_off(MS_BOOL ben)
{
#ifdef CONFIG_MSTAR_CLKM
    MS_S32 s32Handle;
    if (ben)
    {
        //clock of ve
        s32Handle = Drv_Clkm_Get_Handle("g_clk_ve");
        Drv_Clkm_Set_Clk_Source(s32Handle,"g_clk_ve enable");

        //clock of vedac
        s32Handle = Drv_Clkm_Get_Handle("g_clk_vedac");
        Drv_Clkm_Set_Clk_Source(s32Handle,"g_clk_vedac enable");
    }
    else
    {
        //clock of ve
        s32Handle = Drv_Clkm_Get_Handle("g_clk_ve");
        Drv_Clkm_Clk_Gate_Disable(s32Handle);
        VE_DBG(printf("g_clk_ve Disable clock.\n"));

        //clock of vedac
        s32Handle = Drv_Clkm_Get_Handle("g_clk_vedac");
        Drv_Clkm_Clk_Gate_Disable(s32Handle);
        VE_DBG(printf("g_clk_vedac Disable clock.\n"));
    }
#else
    if(ben)
    {
        MDrv_WriteByteMask(L_BK_CHIPTOP(0x24), 0,BIT(0));   // clock of ve enable
        MDrv_WriteByteMask(H_BK_CHIPTOP(0x24), 0, BIT(0));   //  clock of vedac enable
    }
    else
    {
        MDrv_WriteByteMask(L_BK_CHIPTOP(0x24), 1,BIT(0));   // clock of ve enable
        MDrv_WriteByteMask(H_BK_CHIPTOP(0x24), 1, BIT(0));   //  clock of vedac enable
    }
#endif

    MDrv_WriteByteMask(L_BK_CHIPTOP(0x25), (ben==TRUE)?0:BIT(0),BIT(0));   //clock of vein enable/disable
}

void Hal_VE_set_ve_on_off(MS_BOOL ben)
{
    MDrv_WriteRegBit(L_BK_VE_SRC(0x00), ben, BIT(0));
}

void Hal_VE_set_blackscreen(MS_BOOL ben)
{
    MDrv_Write2Byte(L_BK_VE_SRC(0x39), 0x8080); //Set VE YPbPr black - CbCr
    MDrv_WriteByte(H_BK_VE_SRC(0x38), 0x00); //Set VE YPbPr black-Y
    //Note: Y value under 16 might truncate to 0, so 0x10 is equal to 0x0
    MDrv_WriteRegBit(L_BK_VE_SRC(0x38),ben, BIT(6)); // Turn on black
}

MS_BOOL Hal_VE_is_blackscreen_enabled(void)
{
    // Only check enable bit.
    if ( MDrv_ReadByte( L_BK_VE_SRC(0x38) ) & BIT(6) )
    {
        if ( MDrv_Read2Byte( L_BK_VE_SRC(0x39) ) == 0x8080 ) // black
            return TRUE;
        else
            return FALSE;
    }
    else
    {
        return FALSE;
    }
}
//------------------------------------------------------------------------------
/// VE restart TVE to read data from TT buffer
/// @return none
//------------------------------------------------------------------------------
void Hal_VE_EnableTtx(MS_BOOL bEnable)
{
    if(_enVE_DolbyVerifierStatus == EN_VE_DOLBY_VERIFIER_ONGOING)
    {
        _enVE_DolbyVerifierStatus = EN_VE_DOLBY_VERIFIER_DONE;
        MDrv_WriteByteMask(L_BK_VE_ENC(0x2E), 0x00, BIT(3));
        _Hal_VE_restore_orig_ttx_setting();
    }

    //trig transition
    if (bEnable)
    {
        //Set VBI Mode to enable TTx
        MDrv_WriteByteMask(L_BK_VE_ENC(0x2E), BIT(7)|BIT(6)|BIT(3), BIT(7)|BIT(6)|BIT(3));

        //Enable next read
        if (MDrv_Read2Byte(L_BK_VE_SRC(0x43))&0x0002)
        {
            MDrv_Write2Byte(L_BK_VE_SRC(0x43),MDrv_Read2Byte(L_BK_VE_SRC(0x43))&0xfffd);
        }
        else
        {
            MDrv_Write2Byte(L_BK_VE_SRC(0x43),MDrv_Read2Byte(L_BK_VE_SRC(0x43))|0x0002);
        }
    }
    else
    {
        MDrv_WriteByteMask(L_BK_VE_ENC(0x2E), 0x00, BIT(3));
    }
}

//------------------------------------------------------------------------------
/// VE set TT buffer address
/// @return none
//------------------------------------------------------------------------------
void Hal_VE_Set_ttx_Buffer(MS_PHY phyStartAddr, MS_U32 u32Size)
{
    MS_PHY phyVeTtxAddr = 0;
    MS_U32 u32TtxDataSize = 0;
    MS_PHY phyStartOffset;
    MS_U8 u8MiuSel = 0;

    if(_enVE_DolbyVerifierStatus == EN_VE_DOLBY_VERIFIER_ONGOING)
    {
        _enVE_DolbyVerifierStatus = EN_VE_DOLBY_VERIFIER_DONE;
        MDrv_WriteByteMask(L_BK_VE_ENC(0x2E), 0x00, BIT(3));
        _Hal_VE_restore_orig_ttx_setting();
    }

    if(_enVE_DolbyVerifierStatus == EN_VE_DOLBY_VERIFIER_DONE)
    {
        _phy_to_miu_offset(u8MiuSel, phyStartOffset, phyStartAddr);
        Hal_VE_SelMIU(u8MiuSel);
    }

    phyVeTtxAddr = phyStartAddr / BYTE_PER_WORD;
    u32TtxDataSize = (u32Size / BYTE_PER_WORD) - 1;
    MDrv_Write4Byte(L_BK_VE_SRC(0x03), phyVeTtxAddr);
    MDrv_Write4Byte(L_BK_VE_SRC(0x05), u32TtxDataSize);
}

//------------------------------------------------------------------------------
/// VE clear TT buffer read done status
/// @return none
//------------------------------------------------------------------------------
void Hal_VE_Clear_ttxReadDoneStatus(void)
{
    MDrv_Write2Byte(L_BK_VE_SRC(0x09),MDrv_Read2Byte(L_BK_VE_SRC(0x09))|0x0400);
    MDrv_Write2Byte(L_BK_VE_SRC(0x09),MDrv_Read2Byte(L_BK_VE_SRC(0x09))&0xfbff);
}

//------------------------------------------------------------------------------
/// VE TT buffer read done status
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
MS_BOOL Hal_VE_Get_ttxReadDoneStatus(void)
{
    MS_U8 u8RegVal;
    //Drv_TVEncoder_Read(BK_VE_SRC(REG_TVE_IRQ_MASK), &u16RegVal);
    u8RegVal = MDrv_ReadByte(L_BK_VE_SRC(0x08));
    if (u8RegVal & BIT(2))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//------------------------------------------------------------------------------
/// VE Set VBI TT active line per field
/// @return none
//------------------------------------------------------------------------------
void Hal_VE_Set_VbiTtxActiveLines(MS_U8 u8LinePerField)
{
    MS_U8 i;

    if(_enVE_DolbyVerifierStatus == EN_VE_DOLBY_VERIFIER_ONGOING)
    {
        _enVE_DolbyVerifierStatus = EN_VE_DOLBY_VERIFIER_DONE;
        MDrv_WriteByteMask(L_BK_VE_ENC(0x2E), 0x00, BIT(3));
        _Hal_VE_restore_orig_ttx_setting();
    }

    MDrv_Write2Byte(L_BK_VE_ENC(0x3c),0xffff);
    //MDrv_Write2Byte(L_BK_VE_ENC(0x3d),0xffff);

    for(i=u8LinePerField;i<16;i++)
    {
        MDrv_Write2Byte(L_BK_VE_ENC(0x3c),MDrv_Read2Byte(L_BK_VE_ENC(0x3c))<<1);
    }

    if(u8LinePerField>16)
    {
        MDrv_Write2Byte(L_BK_VE_ENC(0x3d), 0x01);
    }
    else
    {
        MDrv_Write2Byte(L_BK_VE_ENC(0x3d), 0x00);
    }

}

//------------------------------------------------------------------------------
/// VE Set VBI ttx active line
/// @param <IN>\b odd_start: odd page start line
/// @param <IN>\b odd_end: odd page end line
/// @param <IN>\b even_start: even page start line
/// @param <IN>\b even_end: even page end line
/// @return none
//------------------------------------------------------------------------------
void Hal_VE_SetVbiTtxRange(MS_U16 odd_start, MS_U16 odd_end,
                         MS_U16 even_start, MS_U16 enen_end)
{
    if(_enVE_DolbyVerifierStatus == EN_VE_DOLBY_VERIFIER_ONGOING)
    {
        _enVE_DolbyVerifierStatus = EN_VE_DOLBY_VERIFIER_DONE;
        MDrv_WriteByteMask(L_BK_VE_ENC(0x2E), 0x00, BIT(3));
        _Hal_VE_restore_orig_ttx_setting();
    }

    MDrv_Write2Byte(L_BK_VE_ENC(0x58), odd_start);
    MDrv_Write2Byte(L_BK_VE_ENC(0x59), odd_end);
    MDrv_Write2Byte(L_BK_VE_ENC(0x5A), even_start);
    MDrv_Write2Byte(L_BK_VE_ENC(0x5B), enen_end);
}

//------------------------------------------------------------------------------
/// VE Set VBI TT active line by the given bitmap
/// @param <IN>\b u32Bitmap: a bitmap that defines whick physical lines the teletext lines are to be inserted.
/// @return none
//------------------------------------------------------------------------------
void Hal_VE_Set_VbiTtxActiveLinesBitmap(MS_U32 u32Bitmap)
{
    if(_enVE_DolbyVerifierStatus == EN_VE_DOLBY_VERIFIER_ONGOING)
    {
        _enVE_DolbyVerifierStatus = EN_VE_DOLBY_VERIFIER_DONE;
        MDrv_WriteByteMask(L_BK_VE_ENC(0x2E), 0x00, BIT(3));
        _Hal_VE_restore_orig_ttx_setting();
    }

    MDrv_Write2Byte(L_BK_VE_ENC(0x3C), u32Bitmap);
    MDrv_Write2Byte(L_BK_VE_ENC(0x3D), u32Bitmap >> 16);
}

//------------------------------------------------------------------------------
/// VE Set VE display window offset
/// @return none
//------------------------------------------------------------------------------
void Hal_VE_Set_winodw_offset(MS_PHY phyOffsetAddr)
{
    //change target offsetaddr from w_addr to r_addr (for CVBS output centered in DE mode)
    //Hal_VE_SetMemAddr(&u32offsetaddr,NULL);

    //r_addr set as a pinpoint and it is a constant address
    //w_addr could be offset to present different view of TVE output
    Hal_VE_SetMemAddr(NULL, &phyOffsetAddr);
}

MS_U32 Hal_VE_ConvertARGB2YUV(MS_U32 u32aRGB)
{
    UNUSED(u32aRGB);

    //do nothing
    return u32aRGB;
}

//------------------------------------------------------------------------------
/// Set VE Frame Color
/// @return none
//------------------------------------------------------------------------------
VE_Result Hal_VE_SetFrameColor(MS_U32 u32aRGB)
{
    UNUSED(u32aRGB);

    //do nothing
    return E_VE_NOT_SUPPORT;
}

//------------------------------------------------------------------------------
/// VE Set VE output with OSD
/// @return none
//------------------------------------------------------------------------------
void Hal_VE_Set_OSD(MS_BOOL bEnable)
{
    // Moved to GOP driver
    return;
}

//------------------------------------------------------------------------------
/// Set VE OSD Layer
/// @return none
//------------------------------------------------------------------------------
VE_Result Hal_VE_Set_OSDLayer(MS_VE_OSD_LAYER_SEL eVideoOSDLayer)
{
    MDrv_WriteByteMask(L_BK_VE_SRC(0x5F),eVideoOSDLayer,0x07);
    return E_VE_OK;
}

//------------------------------------------------------------------------------
/// Get VE OSD Layer
/// @return none
//------------------------------------------------------------------------------
MS_VE_OSD_LAYER_SEL Hal_VE_Get_OSDLayer(void)
{
    return (MS_VE_OSD_LAYER_SEL)(MDrv_Read2Byte(L_BK_VE_SRC(0x5F)) & 0x0007);
}

//------------------------------------------------------------------------------
/// Set VE Video Alpha
/// @return none
//------------------------------------------------------------------------------
VE_Result Hal_VE_Set_VideoAlpha(MS_U8 u8Val)
{
    UNUSED(u8Val);

    //do nothing
    return E_VE_NOT_SUPPORT;
}

//------------------------------------------------------------------------------
/// Get VE Video Alpha
/// @return none
//------------------------------------------------------------------------------
VE_Result Hal_VE_Get_VideoAlpha(MS_U8 *pu8Val)
{
    UNUSED(pu8Val);

    //do nothing
    return E_VE_NOT_SUPPORT;
}

//------------------------------------------------------------------------------
/// VE Set VBI CC
/// @return none
//------------------------------------------------------------------------------
void Hal_VE_EnableCcSw(MS_BOOL bEnable)
{
    if (bEnable)
    {
        MDrv_Write2Byte(L_BK_VE_ENC(0x2E), (MDrv_Read2Byte(L_BK_VE_ENC(0x2E))|0x0081));
        MDrv_Write2Byte(L_BK_VE_SRC(0x46), (MDrv_Read2Byte(L_BK_VE_SRC(0x46))|0x0004));
    }
    else
    {
        MDrv_Write2Byte(L_BK_VE_ENC(0x2E), (MDrv_Read2Byte(L_BK_VE_ENC(0x2E))&0xfffe));
    }
}

//------------------------------------------------------------------------------
/// VE Set VBI CC active line
/// @return none
//------------------------------------------------------------------------------
void Hal_VE_SetCcRange(MS_U16 odd_start, MS_U16 odd_end, MS_U16 even_start, MS_U16 enen_end)
{
    MDrv_Write2Byte(L_BK_VE_ENC(0x4E), odd_start);
    MDrv_Write2Byte(L_BK_VE_ENC(0x4F), odd_end);
    MDrv_Write2Byte(L_BK_VE_ENC(0x50), even_start);
    MDrv_Write2Byte(L_BK_VE_ENC(0x51), enen_end);
}

//------------------------------------------------------------------------------
/// VE Set VBI CC data
/// @return none
//------------------------------------------------------------------------------
void Hal_VE_SendCcData(MS_BOOL bIsOdd, MS_U16 data)    // true: odd, false: even
{
    if (bIsOdd)
    {
        MDrv_Write2Byte(L_BK_VE_ENC(0x2B), data);
    }
    else
    {
        MDrv_Write2Byte(L_BK_VE_ENC(0x2C), data);
    }
}

//------------------------------------------------------------------------------
/// VE Enable Test pattern
/// @return none
//------------------------------------------------------------------------------
void Hal_VE_Set_TestPattern(MS_BOOL ben)
{
    MDrv_WriteRegBit(H_BK_VE_SRC(0x00),ben, BIT(2)); // Turn on black
}

//------------------------------------------------------------------------------
/// VE write register
/// @return none
//------------------------------------------------------------------------------
void Hal_VE_W2BYTE_MSK(MS_U32 u32Reg, MS_U16 u16Val, MS_U16 u16Mask)
{
    W2BYTEMSK(u32Reg, u16Val, u16Mask);
}

//------------------------------------------------------------------------------
/// VE write register
/// @return MS_U16 register value
//------------------------------------------------------------------------------
MS_U16 Hal_VE_R2BYTE_MSK(MS_U32 u32Reg, MS_U16 u16Mask)
{
    return R2BYTEMSK(u32Reg, u16Mask);
}

//-------------------------------------------------------------------------------------------------
//  Local Definition for DumpTbl
//-------------------------------------------------------------------------------------------------
#define VE_TAB_MAX_SIZE  2048

#define VE_ASSERT(_cnd, _fmt, _args...)                                                                            \
                                    if (!(_cnd)) {                                                                  \
                                        printf("VE ASSERT: %s %d:"_fmt, __FILE__, __LINE__, ##_args);              \
                                        while(1);                                                                   \
                                    }

static void _Hal_VE_DumpRegTab(MS_U8 *pVeTab, MS_U8 u8TableType)
{
    MS_U32 u32tabIdx = 0;
    MS_U16 u16timeout = 0x3FFF;
    MS_U32 u32Addr;
    MS_U16 u16Mask;
    MS_U16 u16Value;

    VE_DBG(printf("tab: gen \n"));

    if (pVeTab == NULL)
    {
        VE_ASSERT(0, "[%s] VeTable error \n.", __FUNCTION__);
        return;
    }

    while (--u16timeout)
    {
        u32Addr = ((pVeTab[u32tabIdx]<<8) + pVeTab[(u32tabIdx +1)]) & 0xFFFF;
        u16Mask  = pVeTab[(u32tabIdx +2)] & 0xFF;
        u16Value = pVeTab[(u32tabIdx +3)] & 0xFF;

        if (u32Addr == REG_TABLE_END) // check end of table
            break;

        switch((E_VE_DUMP_TAB_TYPE)u8TableType)
        {
            case E_VE_DUMPTABTYPE_MACROVISION:
                break;

            case E_VE_DUMPTABTYPE_DCS_CVBS:
            case E_VE_DUMPTABTYPE_DCS_COMPONENT:
                if(pVeTab[(u32tabIdx +4)] == E_DCSST_IS_ACTIVITIVE_KEYBYTE)
                {
                    u16Value = *(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL + pVeTab[(u32tabIdx +5)]);
                }
                break;

            default:
                VE_ASSERT(0, "[%s] GetTable Type :unknown Tab Type \n.", __FUNCTION__);
                return ;
        }

        VE_DBG(printf("[addr=%04lx, msk=%02x, val=%02x] \n", u32Addr, u16Mask, u16Value));

        u32Addr|=0x100000;  //Convert to NonPM address

        if (u32Addr & 0x1)
        {
            u32Addr --;
            W2BYTE_TAB(u32Addr, (u16Value << 8), (u16Mask << 8));
        }
        else
        {
            W2BYTE_TAB(u32Addr, u16Value, u16Mask);
        }

        switch((E_VE_DUMP_TAB_TYPE)u8TableType)
        {
            case E_VE_DUMPTABTYPE_MACROVISION:
                u32tabIdx = u32tabIdx + 4;
                break;

            case E_VE_DUMPTABTYPE_DCS_CVBS:
            case E_VE_DUMPTABTYPE_DCS_COMPONENT:
                u32tabIdx = u32tabIdx + REG_ADDR_SIZE+REG_MASK_SIZE+DCS_DCSTBL_EXTEND_SIZE+1;
                break;

            //default:
            //    VE_ASSERT(0, "[%s] GetTable Type :unknown Tab Type \n.", __FUNCTION__);
            //    return ;
        }
    }

    if (u16timeout==0)
    {
        VE_ASSERT(0, "[%s] fails: timeout \n.", __FUNCTION__);
    }
}

static void _Hal_VE_GetTabSize(MS_U8 *pVeTab, MS_U16 u16Tabletype, MS_U32 *pu32VeTabsize)
{
    MS_U8 u8Colslen = 0;
    MS_U16 u16timeout = 0x3FFF;
    MS_U16 u16Addr;

    if (pVeTab == NULL)
    {
        VE_ASSERT(0, "[%s] VeTable error \n.", __FUNCTION__);
        return;
    }

    switch((E_VE_DUMP_TAB_TYPE)u16Tabletype)
    {
        case E_VE_DUMPTABTYPE_MACROVISION:
            u8Colslen = 4;
            break;

        case E_VE_DUMPTABTYPE_DCS_CVBS:
        case E_VE_DUMPTABTYPE_DCS_COMPONENT:
            u8Colslen = REG_ADDR_SIZE+REG_MASK_SIZE+DCS_DCSTBL_EXTEND_SIZE+1;
            break;

        default:
            VE_ASSERT(0, "[%s] GetTable Size :unknown Tab Size \n.", __FUNCTION__);
            return ;
    }

    while (--u16timeout)
    {
        u16Addr = (pVeTab[*pu32VeTabsize]<<8) + pVeTab[(*pu32VeTabsize +1)];
        if (u16Addr == REG_TABLE_END) // check end of table
        {
            *pu32VeTabsize = *pu32VeTabsize + u8Colslen;
            break;
        }
        *pu32VeTabsize = *pu32VeTabsize + u8Colslen;
    }

    if (u16timeout==0)
    {
        VE_ASSERT(0, "[%s] fails: timeout \n.", __FUNCTION__);
    }

    VE_DBG(printf("<<*pu32VeTabsize= %ld>>\n", *pu32VeTabsize));

    if(*pu32VeTabsize > VE_TAB_MAX_SIZE)
    {
        VE_ASSERT(0, "[%s] Tab size too large than VE_TAB_MAX_SIZE = %d \n.", __FUNCTION__, VE_TAB_MAX_SIZE);
    }
}


void Hal_VE_DumpTable(MS_U8 *pVeTable, MS_U8 u8TableType)
{
    //MS_U8 u8VeTab[VE_TAB_MAX_SIZE];
    MS_U32 u32VeTabsize = 0;

    _Hal_VE_GetTabSize(pVeTable, u8TableType, &u32VeTabsize);
    //memcpy(u8VeTab, pVeTable, u32VeTabsize);

    VE_DBG(printf("[%s][%d] DAC u8DACtype = %d\n", __FUNCTION__, __LINE__, u8TableType));

    switch((E_VE_DUMP_TAB_TYPE)u8TableType)
    {
        case E_VE_DUMPTABTYPE_MACROVISION:
        case E_VE_DUMPTABTYPE_DCS_CVBS:
        case E_VE_DUMPTABTYPE_DCS_COMPONENT:
            _Hal_VE_DumpRegTab(pVeTable, u8TableType);
            break;

        default:
            VE_DBG(printf( "GetTable:unknown Tab type> \n"));
            break;
    }

}

#if 0
//Check OTP of Macrovision
static MS_BOOL _Hal_VE_IsOTPEnable(void)
{

    if(!MDrv_MMIO_GetBASE( &virtPMRIUBaseAddress, &u32PMBankSize, MS_MODULE_PM ))
    {
        printf("[%s][%d] _Hal_VE_IsOTPEnable Get PM BASE failure \n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    VE_DBG(printf("[%s][%d] virtPMRIUBaseAddress = 0x%lx , u32PMBankSize = 0x%lx \n", __FUNCTION__, __LINE__, virtPMRIUBaseAddress, u32PMBankSize));
    VE_DBG(printf("[%s][%d] H_BK_PMOTP(0x03) = 0x%x \n", __FUNCTION__, __LINE__, PM_MDrv_ReadByte(H_BK_PMOTP(0x03))));

    //-----------------------------------------
    ///read register 0x113A_03_Bit[12:10]
    ///000:      MacroVisison can be enabled.
    ///Others:   MacroVision need to be disabled.
    //-----------------------------------------
    if(((PM_MDrv_ReadByte(H_BK_PMOTP(0x03)) & 0x1C) == 0x0))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif

static VE_Result _Hal_VE_IsMVValid(MS_VE_MV_TYPE eMvType)
{
    //Check if output standard and Macrovision are matched
    if(MDrv_Read2Byte(L_BK_VE_SRC(0x42)) == 0x1e0)
    {
        //NTSC Mode
        if(eMvType >= MS_VE_MV_PAL_START)
        {
            return E_VE_FAIL;
        }
    }
    else
    {
        //PAL Mode
        if(eMvType < MS_VE_MV_PAL_START)
        {
            return E_VE_FAIL;
        }
    }

    return E_VE_OK;
}

static void _Hal_VE_MV_OnOff(InterruptNum eIntNum)
{
    UNUSED(eIntNum);

    // clear INT status
    MDrv_WriteByteMask(H_BK_VE_SRC(0x09),0x02,0x02);
    MDrv_WriteByteMask(H_BK_VE_SRC(0x09),0x00,0x02);

    {
        if(_bMV_Enable)
        {
            if(_bMV_BitControl_Set)
            {
                MDrv_WriteByteMask(L_BK_VE_ENC(0x78),_u8MV_BitControl_N0,0xFF);    //N0
            }
            else
            {
                if((_eMvType == MS_VE_MV_NTSC_TYPE1)||(_eMvType == MS_VE_MV_PAL_TYPE1)
                    ||(_eMvType == MS_VE_MV_PAL_TYPE2)||(_eMvType == MS_VE_MV_PAL_TYPE3))
                {
                    MDrv_WriteByteMask(L_BK_VE_ENC(0x78),0x36,0xFF);
                }
                else
                {
                    MDrv_WriteByteMask(L_BK_VE_ENC(0x78),0x3E,0xFF);
                }
            }
        }
        else
        {
            MDrv_WriteByteMask(L_BK_VE_ENC(0x78),0x00,0xFF); //Disable Macrovision
            //MDrv_WriteByteMask(L_BK_VE_ENC(0x2F),0x00,0x01); //Disable ECO Bit
            MDrv_WriteByteMask(L_BK_VE_ENC(0x7E),0x00,0x01); //Disable BURST Enable

            //For Sync Step issue
            MDrv_WriteByteMask(H_BK_VE_ENC(0x7D),0x00,0x01);

            //For Y-Clamping
            MDrv_WriteByteMask(H_BK_VE_ENC(0x2D),0x53,0x53);
            MDrv_WriteByteMask(L_BK_VE_ENC(0x2D),0x20,0x70);
        }
    }

    MsOS_DisableInterrupt(E_INT_IRQ_VE);
}

VE_Result Hal_VE_SetMV(MS_BOOL bEnable, MS_VE_MV_TYPE eMvType)
{
    VE_Result eRet;
    static MS_BOOL bIsCBAttach =FALSE;

    eRet = _Hal_VE_IsMVValid(eMvType);
    if(!(eRet == E_VE_OK))
    {
        VE_DBG(printf("[%s][%d] ERROR ret = %d \n", __FUNCTION__, __LINE__, eRet));
        return eRet;
    }

    _eMvType = eMvType;
    _bMV_BitControl_Set = FALSE;

    if(!bEnable)
    {
        _bMV_Enable = FALSE;
    }
    else
    {
        MACROVISION_TAB_INFO mTbl;

        _bMV_Enable = TRUE;

        switch(eMvType)
        {
            default:
            case MS_VE_MV_NTSC_TYPE1:
                mTbl = MACROVISIONMAP_Main[MACROVISION_TYPE_MACROVISION_NTSC_TYPE1];
                VE_DBG(printf("[%s][%d] Macrovision case NTSC TYPE1!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_MV_NTSC_TYPE2:
                mTbl = MACROVISIONMAP_Main[MACROVISION_TYPE_MACROVISION_NTSC_TYPE2];
                VE_DBG(printf("[%s][%d] Macrovision case NTSC TYPE2!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_MV_NTSC_TYPE3:
                mTbl = MACROVISIONMAP_Main[MACROVISION_TYPE_MACROVISION_NTSC_TYPE3];
                VE_DBG(printf("[%s][%d] Macrovision case NTSC TYPE3!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_MV_NTSC_TEST_N01:
                mTbl = MACROVISIONMAP_Main[MACROVISION_TYPE_MACROVISION_NTSC_TEST_N01];
                VE_DBG(printf("[%s][%d] Macrovision case NTSC TEST_N01!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_MV_NTSC_TEST_N02:
                mTbl = MACROVISIONMAP_Main[MACROVISION_TYPE_MACROVISION_NTSC_TEST_N02];
                VE_DBG(printf("[%s][%d] Macrovision case NTSC TEST_N02!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_MV_NTSC_TYPE2_TTX:
                mTbl = MACROVISIONMAP_Main[MACROVISION_TYPE_MACROVISION_NTSC_TYPE2_TTX];
                VE_DBG(printf("[%s][%d] Macrovision case NTSC TYPE2 TTX!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_MV_NTSC_TEST_N01_TTX:
                mTbl = MACROVISIONMAP_Main[MACROVISION_TYPE_MACROVISION_NTSC_TEST_N01_TTX];
                VE_DBG(printf("[%s][%d] Macrovision case NTSC TEST_N01_TTX!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_MV_PAL_TYPE1:
            case MS_VE_MV_PAL_TYPE2:
            case MS_VE_MV_PAL_TYPE3:
                mTbl = MACROVISIONMAP_Main[MACROVISION_TYPE_MACROVISION_PAL_TYPE1_2_3];
                VE_DBG(printf("[%s][%d] Macrovision case PAL 1/2/3!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_MV_PAL_TEST_P01:
                mTbl = MACROVISIONMAP_Main[MACROVISION_TYPE_MACROVISION_PAL_TEST_P01];
                VE_DBG(printf("[%s][%d] Macrovision case TEST P01!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_MV_PAL_TEST_P02:
                mTbl = MACROVISIONMAP_Main[MACROVISION_TYPE_MACROVISION_PAL_TEST_P02];
                VE_DBG(printf("[%s][%d] Macrovision case TEST P02!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_MV_PAL_TYPE2_TTX:
                mTbl = MACROVISIONMAP_Main[MACROVISION_TYPE_MACROVISION_PAL_TYPE1_2_3_TTX];
                VE_DBG(printf("[%s][%d] Macrovision case PAL TYPE2 TTX!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_MV_PAL_TEST_P01_TTX:
                mTbl = MACROVISIONMAP_Main[MACROVISION_TYPE_MACROVISION_PAL_TEST_P01_TTX];
                VE_DBG(printf("[%s][%d] Macrovision case TEST P01 TTX!!\n", __FUNCTION__, __LINE__));
                break;
        }

        Hal_VE_DumpTable(mTbl.pVeMACROVISIONTab, mTbl.eMACROVISIONtype);

        //Set MV_TYPE (0:NTSC, 1:PAL)
        if(eMvType >= MS_VE_MV_PAL_START)
        {
            MDrv_WriteByteMask(H_BK_VE_ENC(0x78),0x80,0x80);
        }
        else
        {
            MDrv_WriteByteMask(H_BK_VE_ENC(0x78),0x00,0x80);
        }

        //Sync Step issue
        MDrv_WriteByteMask(H_BK_VE_ENC(0x7D),0x01,0x01);

        //BP pulse width to small ECO bits //Kenya U02 ECO 0x103E_30[1]
        MDrv_WriteByteMask(L_BK_VE_ENC(0x30),0x01,0xFF);

        //For Y-Clamping
        MDrv_WriteByteMask(H_BK_VE_ENC(0x2D),0x33,0x33);
        MDrv_WriteByteMask(L_BK_VE_ENC(0x2D),0x70,0x70);
    }

    MDrv_WriteByteMask(H_BK_VE_SRC(0x08),0xFD,0xFF);

    MDrv_WriteByteMask(H_BK_VE_SRC(0x09),0x02,0x02);
    MDrv_WriteByteMask(H_BK_VE_SRC(0x09),0x00,0x02);

    //Attach interrupt
    if(!bIsCBAttach)
    {
        bIsCBAttach = TRUE;
        MsOS_AttachInterrupt(E_INT_IRQ_VE, (InterruptCb)_Hal_VE_MV_OnOff);
    }

    MsOS_EnableInterrupt(E_INT_IRQ_VE);

    return E_VE_OK;
}

VE_Result Hal_VE_SetMV_BitControl(MS_BOOL bEnable, MS_VE_MV_BitControl MV_BitControl_Data)
{
    VE_Result eRet = E_VE_OK;

    if(!(eRet == E_VE_OK))
    {
        VE_DBG(printf("[%s][%d] ERROR ret = %d \n", __FUNCTION__, __LINE__, eRet));
        return eRet;
    }

    _bMV_BitControl_Set = TRUE;

    if(!bEnable)
    {
        _bMV_Enable = FALSE;
    }
    else
    {
        MS_U16 u16Nx_Data = 0;
        MS_VE_MV_BitControl VE_MV_BitControl_Data;
        memcpy(&VE_MV_BitControl_Data, &MV_BitControl_Data, sizeof(VE_MV_BitControl_Data));

        _bMV_Enable = TRUE;

        //N0 and N0 expanded setting
        u16Nx_Data = ((VE_MV_BitControl_Data.MV_ByteInfo.u8CPC[0]&0x0F)<<4)|(VE_MV_BitControl_Data.MV_ByteInfo.u8CPC[1]&0x0F);
        _u8MV_BitControl_N0 = u16Nx_Data&0xFF;    //N0
        u16Nx_Data = (u16Nx_Data/(2*2*2))%2;
        MDrv_WriteByteMask(L_BK_VE_ENC(0x7E),u16Nx_Data,0x01);    //N0 expanded setting

        //N1
        u16Nx_Data = ((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[0]&0x03)<<4)|(VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[1]&0x0F);
        MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x01),u16Nx_Data,0x3F);

        //N2
        u16Nx_Data = ((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[2]&0x07)<<3)|((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[3]&0x0E)>>1);
        MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x01),(u16Nx_Data&0x3)<<6,0xC0);
        MDrv_WriteByteMask(H_BK_VE_ENC_EX(0x01),(u16Nx_Data&0x3C)>>2,0x0F);

        //N3
        u16Nx_Data = ((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[3]&0x01)<<5)|((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[4]&0x0F)<<1)
                                                                          |((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[5]&0x08)>>3);
        MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x02),u16Nx_Data,0x3F);

        //N22
        u16Nx_Data = (VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[5]&0x04)>>2;
        MDrv_WriteByteMask(H_BK_VE_ENC(0x7D),u16Nx_Data<<1,0x02);

        //N4
        u16Nx_Data = ((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[5]&0x03)<<4)|(VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[6]&0x0F);
        MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x02),(u16Nx_Data&0x3)<<6,0xC0);
        MDrv_WriteByteMask(H_BK_VE_ENC_EX(0x02),(u16Nx_Data&0x3C)>>2,0x0F);

        //N5
        u16Nx_Data = (VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[7]&0x0E)>>1;
        MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x03),u16Nx_Data,0x07);

        //N6
        u16Nx_Data = ((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[7]&0x01)<<2)|((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[8]&0x0C)>>2);
        MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x03),u16Nx_Data<<3,0x38);

        //N7
        u16Nx_Data = VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[8]&0x03;
        MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x03),u16Nx_Data<<6,0xC0);

        //N8
        MDrv_WriteByteMask(L_BK_VE_ENC(0x78),0x00,0x00);
        u16Nx_Data = ((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[9]&0x0F)<<2)|((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[10]&0x0C)>>2);
        MDrv_WriteByteMask(H_BK_VE_ENC(0x78),u16Nx_Data,0x3F);

        //N9
        u16Nx_Data = ((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[10]&0x03)<<4)|(VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[11]&0x0F);
        MDrv_WriteByteMask(L_BK_VE_ENC(0x79),u16Nx_Data,0x3F);

        //N10
        u16Nx_Data = ((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[12]&0x0F)<<2)|((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[13]&0x0C)>>2);
        MDrv_WriteByteMask(L_BK_VE_ENC(0x79),(u16Nx_Data&0x3)<<6,0xC0);
        MDrv_WriteByteMask(H_BK_VE_ENC(0x79),(u16Nx_Data&0x3C)>>2,0x0F);

        //N11
        u16Nx_Data = ((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[13]&0x03)<<13)|((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[14]&0x0F)<<9)
                      |((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[15]&0x0F)<<5)|((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[16]&0x0F)<<1)
                      |((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[17]&0x08)>>3);
        MDrv_WriteByteMask(L_BK_VE_ENC(0x7A),u16Nx_Data&0xFF,0xFF);
        MDrv_WriteByteMask(H_BK_VE_ENC(0x7A),(u16Nx_Data&0x7F00)>>8,0x7F);

        //N12
        u16Nx_Data = ((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[17]&0x07)<<12)|((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[18]&0x0F)<<8)
                      |((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[19]&0x0F)<<4)|(VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[20]&0x0F);
        MDrv_WriteByteMask(L_BK_VE_ENC(0x7B),u16Nx_Data&0xFF,0xFF);
        MDrv_WriteByteMask(H_BK_VE_ENC(0x7B),(u16Nx_Data&0x7F00)>>8,0x7F);

        //N13
        u16Nx_Data = ((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[21]&0x0F)<<4)|(VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[22]&0x0F);
        MDrv_WriteByteMask(L_BK_VE_ENC(0x7C),u16Nx_Data,0xFF);

        //N14
        u16Nx_Data = ((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[23]&0x0F)<<4)|(VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[24]&0x0F);
        MDrv_WriteByteMask(H_BK_VE_ENC(0x7C),u16Nx_Data,0xFF);

        //N15
        u16Nx_Data = ((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[25]&0x0F)<<4)|(VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[26]&0x0F);
        MDrv_WriteByteMask(L_BK_VE_ENC(0x7D),u16Nx_Data,0xFF);

        //N16
        u16Nx_Data = (VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[27]&0x08)>>3; //N16
        //if(N16==1) val = 0x3E01[7:0] - 15
        //else          val = 0x3E01[7:0]
        u16Nx_Data = MDrv_ReadByte(L_BK_VE_ENC(0x01)) - ((u16Nx_Data==0x01)?15:0);  //ADV_BURST_ST
        MDrv_WriteByteMask(L_BK_VE_ENC(0x7F),u16Nx_Data,0xFF);

        //N17~19
        MS_U16 u16N17 = 0;
        MS_U16 u16N18 = 0;
        MS_U16 u16N19 = 0;
        u16N17 = ((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[27]&0x07)<<1)|((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[28]&0x08)>>3);  //N17
        u16N18 = ((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[28]&0x07)<<1)|((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[29]&0x08)>>3);  //N18
        u16N19 = ((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[29]&0x07)<<1)|((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[30]&0x08)>>3);  //N19
        u16Nx_Data = MDrv_ReadByte(L_BK_VE_ENC(0x7F)) + 4*u16N17 + 4*u16N18 + 8*u16N19 -1;    //ADV_BURST_ST + 4xN17 + 4xN18 + 8xN19 - 1
        MDrv_WriteByteMask(H_BK_VE_ENC(0x7F),u16Nx_Data,0xFF);
        //if(N17 == 0) val =0;
        //else            val = 4xN17
        u16Nx_Data = (u16N17==0x00)?0:(4*u16N17);
        MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x00),u16Nx_Data,0x7F);
        //if(N18 == 0) val =0;
        //else            val = 4xN18
        u16Nx_Data = (u16N18==0x00)?0:(4*u16N18);
        MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x00),(u16Nx_Data&0x01)<<7,0x80);
        MDrv_WriteByteMask(H_BK_VE_ENC_EX(0x00),(u16Nx_Data&0xFFFE)>>1,0x3F);

        //N20
        u16Nx_Data = (VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[30]&0x07);
        u16Nx_Data = ((u16Nx_Data&0x1)<<2)|(((u16Nx_Data&0x2)>>1)<<1)|((u16Nx_Data&0x4)>>2);
        MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x04),u16Nx_Data,0x07);

        //N21
        u16Nx_Data = ((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[31]&0x0F)<<6)|((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[32]&0x0F)<<2)
                      |((VE_MV_BitControl_Data.MV_ByteInfo.u8CPS[0]&0x0C)>>2);
        u16Nx_Data = ((u16Nx_Data&0x1)<<9)|(((u16Nx_Data>>1)&0x1)<<8)|(((u16Nx_Data>>2)&0x1)<<7)|(((u16Nx_Data>>3)&0x1)<<6)
                     |(((u16Nx_Data>>4)&0x1)<<5)|(((u16Nx_Data>>5)&0x1)<<4)|(((u16Nx_Data>>6)&0x1)<<3)|(((u16Nx_Data>>7)&0x1)<<2)
                     |(((u16Nx_Data>>8)&0x1)<<1)|((u16Nx_Data>>9)&0x1);
        MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x04),(u16Nx_Data&0x1F)<<3,0xF8);
        MDrv_WriteByteMask(H_BK_VE_ENC_EX(0x04),(u16Nx_Data&0xFFE0)>>5,0x1F);

        //Set MV_TYPE (0:NTSC, 1:PAL)
        if(Hal_VE_Get_Output_Video_Std() == MS_VE_NTSC)
        {
            MDrv_WriteByteMask(H_BK_VE_ENC(0x78),0x00,0x80);
        }
        else if(Hal_VE_Get_Output_Video_Std() == MS_VE_PAL)
        {
            MDrv_WriteByteMask(H_BK_VE_ENC(0x78),0x80,0x80);
        }

        //Enable ECO-Bit
        //MDrv_WriteByteMask(L_BK_VE_ENC(0x2F),0x01,0x01);

        //Sync Step issue
        MDrv_WriteByteMask(H_BK_VE_ENC(0x7D),0x01,0x01);

        //BP pulse width to small ECO bits
        MDrv_WriteByteMask(L_BK_VE_ENC(0x30),0x01,0xFF);

        //For Y-Clamping
        MDrv_WriteByteMask(H_BK_VE_ENC(0x2D),0x53,0x53);
        MDrv_WriteByteMask(L_BK_VE_ENC(0x2D),0x70,0x70);
    }

    MDrv_WriteByteMask(H_BK_VE_SRC(0x08),0xFD,0xFF);

    MDrv_WriteByteMask(H_BK_VE_SRC(0x09),0x02,0x02);
    MDrv_WriteByteMask(H_BK_VE_SRC(0x09),0x00,0x02);

    //Attach interrupt
    MsOS_AttachInterrupt(E_INT_IRQ_VE, (InterruptCb)_Hal_VE_MV_OnOff);
    MsOS_EnableInterrupt(E_INT_IRQ_VE);

    return E_VE_OK;
}

VE_Result Hal_VE_DCS_SetType(MS_BOOL bEnable, MS_VE_DCS_TYPE eDCSType)
{
    MS_BOOL eRet = FALSE;
    MS_U8 u8DCSKEYTBL_Index = 0;
    // coverity[unsigned_compare]
    if((eDCSType >= MS_VE_DCS_TYPE_MIN) && (eDCSType <= MS_VE_DCS_TYPE_MAX))
    {
        if(((eDCSType >= MS_VE_DCS_CVBS_MIN) && (eDCSType <= MS_VE_DCS_CVBS_MAX)) ||
             ((eDCSType >= MS_VE_DCS_COMPONENT_MIN) && (eDCSType <= MS_VE_DCS_COMPONENT_MAX)))
        {
            eRet = TRUE;
        }
    }

    if(!eRet)
    {
        VE_DBG(printf("[%s][%d] DCS Type not Support\n", __FUNCTION__, __LINE__));
        return E_VE_NOT_SUPPORT;
    }

    if(!bEnable)
    {
        u8DCSKEYTBL_Index = E_DCS_STATE_ON_TO_TRANSITIONOFF * E_DCSST_ACTIVITIVE_KEYBYTE_NUM;
        if((eDCSType >= MS_VE_DCS_CVBS_MIN) && (eDCSType <= MS_VE_DCS_CVBS_MAX))
        {
            MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x74),0x01,0x09); //Disable CVBS DCS and DCS Activitive Key Write Enable

            MDrv_WriteByte(L_BK_VE_ENC_EX(0x70),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_0));
            MDrv_WriteByte(H_BK_VE_ENC_EX(0x70),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_1));
            MDrv_WriteByte(L_BK_VE_ENC_EX(0x71),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_2));
            MDrv_WriteByte(H_BK_VE_ENC_EX(0x71),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_3));
            MDrv_WriteByte(L_BK_VE_ENC_EX(0x72),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_4));
            MDrv_WriteByte(H_BK_VE_ENC_EX(0x72),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_5));
            MDrv_WriteByte(L_BK_VE_ENC_EX(0x73),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_6));
            MDrv_WriteByte(H_BK_VE_ENC_EX(0x73),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_7));

            MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x74),0x00,0x01); //DCS Activitive Key Write Disable
            MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x74),0x01,0x01); //DCS Activitive Key Write Enable

            u8DCSKEYTBL_Index = E_DCS_STATE_TRANSITIONOFF_TO_OFF * E_DCSST_ACTIVITIVE_KEYBYTE_NUM;

            MDrv_WriteByte(L_BK_VE_ENC_EX(0x70),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_0));
            MDrv_WriteByte(H_BK_VE_ENC_EX(0x70),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_1));
            MDrv_WriteByte(L_BK_VE_ENC_EX(0x71),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_2));
            MDrv_WriteByte(H_BK_VE_ENC_EX(0x71),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_3));
            MDrv_WriteByte(L_BK_VE_ENC_EX(0x72),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_4));
            MDrv_WriteByte(H_BK_VE_ENC_EX(0x72),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_5));
            MDrv_WriteByte(L_BK_VE_ENC_EX(0x73),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_6));
            MDrv_WriteByte(H_BK_VE_ENC_EX(0x73),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_7));

            MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x74),0x00,0x01); //DCS Activitive Key Write Disable
            MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x74),0x01,0x01); //DCS Activitive Key Write Enable
        }
        else if((eDCSType >= MS_VE_DCS_COMPONENT_MIN) && (eDCSType <= MS_VE_DCS_COMPONENT_MAX))
        {
            MDrv_WriteByte(L_BK_COMPONENT(0x00),0x01); //change sub bank 1
            MDrv_WriteByte(H_BK_COMPONENT(0x00),0x00); //change sub bank 1
            MDrv_WriteByteMask(L_BK_COMPONENT(0x74),0x01,0x21); //Disable Component DCS and DCS Activitive Key Write Enable

            MDrv_WriteByte(L_BK_COMPONENT(0x70),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_0));
            MDrv_WriteByte(H_BK_COMPONENT(0x70),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_1));
            MDrv_WriteByte(L_BK_COMPONENT(0x71),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_2));
            MDrv_WriteByte(H_BK_COMPONENT(0x71),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_3));
            MDrv_WriteByte(L_BK_COMPONENT(0x72),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_4));
            MDrv_WriteByte(H_BK_COMPONENT(0x72),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_5));
            MDrv_WriteByte(L_BK_COMPONENT(0x73),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_6));
            MDrv_WriteByte(H_BK_COMPONENT(0x73),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_7));

            MDrv_WriteByteMask(L_BK_COMPONENT(0x74),0x00,0x01); //DCS Activitive Key Write Disable
            MDrv_WriteByteMask(L_BK_COMPONENT(0x74),0x01,0x01); //DCS Activitive Key Write Disable

            u8DCSKEYTBL_Index = E_DCS_STATE_TRANSITIONOFF_TO_OFF * E_DCSST_ACTIVITIVE_KEYBYTE_NUM;

            MDrv_WriteByte(L_BK_COMPONENT(0x70),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_0));
            MDrv_WriteByte(H_BK_COMPONENT(0x70),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_1));
            MDrv_WriteByte(L_BK_COMPONENT(0x71),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_2));
            MDrv_WriteByte(H_BK_COMPONENT(0x71),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_3));
            MDrv_WriteByte(L_BK_COMPONENT(0x72),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_4));
            MDrv_WriteByte(H_BK_COMPONENT(0x72),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_5));
            MDrv_WriteByte(L_BK_COMPONENT(0x73),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_6));
            MDrv_WriteByte(H_BK_COMPONENT(0x73),*(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL+u8DCSKEYTBL_Index+E_DCSST_ACTIVITIVE_KEYBYTE_7));

            MDrv_WriteByteMask(L_BK_COMPONENT(0x74),0x00,0x01); //DCS Activitive Key Write Disable
            MDrv_WriteByteMask(L_BK_COMPONENT(0x74),0x01,0x01); //DCS Activitive Key Write Disable
        }
    }
    else
    {
        DCS_TAB_INFO DCSTbl;

        switch(eDCSType)
        {
            //default: When switching on "eDCSType", the value of "eDCSType" must be in one of the following intervals: {[0,1], [16,31]}.
            case MS_VE_DCS_CVBS_NTSC:
                DCSTbl = DCSMAP_Main[E_DCS_TABLE_DCS_CVBS_NTSC];
                VE_DBG(printf("[%s][%d] DCS case DCS_ACP_CVBS_NTSC!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_DCS_CVBS_PAL:
                DCSTbl = DCSMAP_Main[E_DCS_TABLE_DCS_CVBS_PAL];
                VE_DBG(printf("[%s][%d] DCS case DCS_ACP_CVBS_PAL!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_DCS_COMPONENT_480I:
                DCSTbl = DCSMAP_Main[E_DCS_TABLE_DCS_COMPONENT_480I];
                VE_DBG(printf("[%s][%d] DCS case DCS_ACP_COMPONENT_480I!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_DCS_COMPONENT_480P:
                DCSTbl = DCSMAP_Main[E_DCS_TABLE_DCS_COMPONENT_480P];
                VE_DBG(printf("[%s][%d] DCS case DCS_ACP_COMPONENT_480P!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_DCS_COMPONENT_576I:
                DCSTbl = DCSMAP_Main[E_DCS_TABLE_DCS_COMPONENT_576I];
                VE_DBG(printf("[%s][%d] DCS case DCS_ACP_COMPONENT_576I!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_DCS_COMPONENT_576P:
                DCSTbl = DCSMAP_Main[E_DCS_TABLE_DCS_COMPONENT_576P];
                VE_DBG(printf("[%s][%d] DCS case DCS_ACP_COMPONENT_576P!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_DCS_COMPONENT_720P_24:
                DCSTbl = DCSMAP_Main[E_DCS_TABLE_DCS_COMPONENT_720P_24];
                VE_DBG(printf("[%s][%d] DCS case DCS_ACP_Component_720P_24!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_DCS_COMPONENT_720P_25:
                DCSTbl = DCSMAP_Main[E_DCS_TABLE_DCS_COMPONENT_720P_25];
                VE_DBG(printf("[%s][%d] DCS case DCS_ACP_COMPONENT_720P_25!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_DCS_COMPONENT_720P_30:
                DCSTbl = DCSMAP_Main[E_DCS_TABLE_DCS_COMPONENT_720P_30];
                VE_DBG(printf("[%s][%d] DCS case DCS_ACP_COMPONENT_720P_30!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_DCS_COMPONENT_720P_50:
                DCSTbl = DCSMAP_Main[E_DCS_TABLE_DCS_COMPONENT_720P_50];
                VE_DBG(printf("[%s][%d] DCS case DCS_ACP_COMPONENT_720P_50!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_DCS_COMPONENT_720P_60:
                DCSTbl = DCSMAP_Main[E_DCS_TABLE_DCS_COMPONENT_720P_60];
                VE_DBG(printf("[%s][%d] DCS case DCS_ACP_COMPONENT_720P_60!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_DCS_COMPONENT_1080I_50:
                DCSTbl = DCSMAP_Main[E_DCS_TABLE_DCS_COMPONENT_1080I_50];
                VE_DBG(printf("[%s][%d] DCS case DCS_ACP_COMPONENT_1080I_50!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_DCS_COMPONENT_1080I_60:
                DCSTbl = DCSMAP_Main[E_DCS_TABLE_DCS_COMPONENT_1080I_60];
                VE_DBG(printf("[%s][%d] DCS case DCS_ACP_COMPONENT_1080I_60!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_DCS_COMPONENT_1080P_24:
                DCSTbl = DCSMAP_Main[E_DCS_TABLE_DCS_COMPONENT_1080P_24];
                VE_DBG(printf("[%s][%d] DCS case DCS_ACP_COMPONENT_1080P_24!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_DCS_COMPONENT_1080P_25:
                DCSTbl = DCSMAP_Main[E_DCS_TABLE_DCS_COMPONENT_1080P_25];
                VE_DBG(printf("[%s][%d] DCS case DCS_ACP_COMPONENT_1080P_25!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_DCS_COMPONENT_1080P_30:
                DCSTbl = DCSMAP_Main[E_DCS_TABLE_DCS_COMPONENT_1080P_30];
                VE_DBG(printf("[%s][%d] DCS case DCS_ACP_COMPONENT_1080P_30!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_DCS_COMPONENT_1080P_50:
                DCSTbl = DCSMAP_Main[E_DCS_TABLE_DCS_COMPONENT_1080P_50];
                VE_DBG(printf("[%s][%d] DCS case DCS_ACP_COMPONENT_1080P_50!!\n", __FUNCTION__, __LINE__));
                break;
            case MS_VE_DCS_COMPONENT_1080P_60:
                DCSTbl = DCSMAP_Main[E_DCS_TABLE_DCS_COMPONENT_1080P_60];
                VE_DBG(printf("[%s][%d] DCS case DCS_ACP_COMPONENT_1080P_60!!\n", __FUNCTION__, __LINE__));
                break;
            default:
                DCSTbl = DCSMAP_Main[E_DCS_TABLE_DCS_CVBS_NTSC];
                VE_DBG(printf("[%s][%d] Can't find DSC type. use DCS case DCS_ACP_CVBS_NTSC by default!!\n", __FUNCTION__, __LINE__));
                break;
        }

        Hal_VE_DumpTable(DCSTbl.pVeDCSTab, DCSTbl.eDCStype);
    }

    if((eDCSType >= MS_VE_DCS_CVBS_MIN) && (eDCSType <= MS_VE_DCS_CVBS_MAX))
    {
        if((MDrv_ReadByte(L_BK_VE_ENC_EX(0x74))&BIT(7))>>7)//DCS Lock Status
        {
            eRet = FALSE;
        }
    }
    else if((eDCSType >= MS_VE_DCS_COMPONENT_MIN) && (eDCSType <= MS_VE_DCS_COMPONENT_MAX))
    {
        MDrv_WriteByte(L_BK_COMPONENT(0x00),0x01); //change sub bank 1
        MDrv_WriteByte(H_BK_COMPONENT(0x00),0x00); //change sub bank 1
        if((MDrv_ReadByte(L_BK_COMPONENT(0x74))&BIT(6))>>6)//DCS Lock Status
        {
            eRet = FALSE;
        }
    }

    if(!eRet)
    {
        VE_DBG(printf("[%s][%d] DCS Status is Lock\n", __FUNCTION__, __LINE__));
        return E_VE_FAIL;
    }

    return E_VE_OK;
}

VE_Result Hal_VE_DCS_SetActivationKey(MS_U8 *pu8ActivationKeyTbl, MS_U8 u8ActivationKeyTblSize)
{
    if(u8ActivationKeyTblSize != (E_DCS_STATE_TRANSITION_TYPE_NUM * E_DCSST_ACTIVITIVE_KEYBYTE_NUM))
    {
        VE_DBG(printf("[%s][%d] DCS Activation Key Size ERROR, Key Size is not equal to %d \n", __FUNCTION__, __LINE__, E_DCS_STATE_TRANSITION_TYPE_NUM * E_DCSST_ACTIVITIVE_KEYBYTE_NUM));
        return E_VE_FAIL;
    }

    if(pu8ActivationKeyTbl ==NULL)
    {
        VE_DBG(printf("[%s][%d] DCS Activation Key ERROR, Key is 'NULL' \n", __FUNCTION__, __LINE__));
        return E_VE_FAIL;
    }

    memcpy(DCSKEYTBL_STATE_TRANSITION_ACTIVATION_KEYTBL, pu8ActivationKeyTbl, u8ActivationKeyTblSize);

    return E_VE_OK;
}

//for Hal_VE_DisableRegWrite_GetCaps
#define DISABLE_REG_WRITE_CAP  TRUE;

MS_BOOL Hal_VE_DisableRegWrite_GetCaps(void)
{
    return DISABLE_REG_WRITE_CAP;
}

// show internal color bar
// TRUE: enable color bar
// FALSE: disable color bar
void Hal_VE_ShowColorBar(MS_BOOL bEnable)
{
    if (bEnable) {
        //enable color bar
        MDrv_WriteRegBit(L_BK_VE_ENC(0x03), 1, BIT(4));
    } else {
        //disable color bar
        MDrv_WriteRegBit(L_BK_VE_ENC(0x03), 0, BIT(4));
    }
}

VE_Result Hal_VE_AdjustPositionBase(MS_S32 s32WAddrAdjustment, MS_S32 s32RAddrAdjustment)
{
    _s32VE_MIUWriteAddressAdjustment = s32WAddrAdjustment / PIXELS_PER_UNIT;
    _s32VE_MIUReadAddressAdjustment = s32RAddrAdjustment / PIXELS_PER_UNIT;

    return E_VE_OK;
}

VE_Result HAL_VE_SetFrameLock(MS_U32 u32IDclk, MS_U32 u32ODclk, MS_U32 u32InitPll, MS_BOOL bEnable)
{
    // Do nothing
    UNUSED(u32IDclk);
    UNUSED(u32ODclk);
    UNUSED(u32InitPll);
    UNUSED(bEnable);
    return E_VE_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_GetCaps()
/// @brief \b Function \b Description : return chip capability
///
/// @param <IN>        \b pointer to MS_VE_Cap structure
////////////////////////////////////////////////////////////////////////////////
VE_Result Hal_VE_GetCaps(MS_VE_Cap *cap)
{
    cap->bSupport_UpScale = FALSE;
    cap->bSupport_CropMode = FALSE;
    cap->bSupport_DolbyVerifier = TRUE;

    return E_VE_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_Get_DRAM_Format()
/// @brief \b Function \b Description : return DRAM format
///
/// @param <IN>        \b none
/// @return one of MS_VE_DRAM_FORMET
////////////////////////////////////////////////////////////////////////////////
MS_VE_DRAM_FORMAT Hal_VE_Get_DRAM_Format(void)
{
    if(!MDrv_ReadRegBit(L_BK_VE_SRC(0x5C), BIT(0))) {
        return MS_VE_DRAM_FORMAT_Y8C8;
    } else {
        return MS_VE_DRAM_FORMAT_Y8C8M4;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_Set_DRAM_Format()
/// @brief \b Function \b Description : set DRAM format
///
/// @param <IN>        \b one of MS_VE_DRAM_FORMAT
/// @return none
////////////////////////////////////////////////////////////////////////////////
void Hal_VE_Set_DRAM_Format(MS_VE_DRAM_FORMAT fmt)
{
    if(fmt == MS_VE_DRAM_FORMAT_Y8C8) {
        MDrv_WriteRegBit(L_BK_VE_SRC(0x5C), 0x0, BIT(0));
    } else if (fmt == MS_VE_DRAM_FORMAT_Y8C8M4) {
        MDrv_WriteRegBit(L_BK_VE_SRC(0x5C), BIT(0), BIT(0));
    } else {
        VE_DBG(printf("MS_VE_DRAM_FORMAT not supported: %d,"
                    "use Y8C8\n", (int)fmt));
        MDrv_WriteRegBit(L_BK_VE_SRC(0x5C), 0, BIT(0));
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_Get_Pack_Num_Per_Line()
/// @brief \b Function \b Description : return DRAM Pack number per line
///
/// @param <IN>        \b none
/// @return pack num per line
////////////////////////////////////////////////////////////////////////////////
MS_U8 Hal_VE_Get_Pack_Num_Per_Line(void)
{
    return MDrv_ReadByte(L_BK_VE_SRC(0x5D));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_Set_Pack_Num_Per_Line()
/// @brief \b Function \b Description : Set Pack number per line
///
/// @param <IN>        \b none
/// @return one of MS_VE_DRAM_FORMET
////////////////////////////////////////////////////////////////////////////////
void Hal_VE_Set_Pack_Num_Per_Line(MS_U8 PackNumPerLine)
{
    MDrv_WriteByte(L_BK_VE_SRC(0x5D), PackNumPerLine);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_Get_Field_Line_Number()
/// @brief \b Function \b Description : return field line number
///
/// @param <IN>        \b none
/// @return field line number
////////////////////////////////////////////////////////////////////////////////
MS_U16 Hal_VE_Get_Field_Line_Number(void)
{
    return MDrv_Read2Byte(L_BK_VE_SRC(0x5E));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_Get_Field_Line_Number()
/// @brief \b Function \b Description : return field line number
///
/// @param <IN>        \b none
/// @return field line number
////////////////////////////////////////////////////////////////////////////////
void Hal_VE_Set_Field_Line_Number(MS_U16 FieldLineNumber)
{
    MDrv_Write2Byte(L_BK_VE_SRC(0x5E), FieldLineNumber);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_Set_Crop_OSD_Offest()
/// @brief \b Function \b Description : setup crop for osd offset
///
/// @param <IN>        \b hstart
/// @param <IN>        \b vstart
/// @return none
////////////////////////////////////////////////////////////////////////////////
void Hal_VE_Set_Crop_OSD_Offset(MS_U16 hstart, MS_U16 vstart)
{
    MDrv_Write2Byte(L_BK_VE_SRC(0x61), hstart);
    MDrv_Write2Byte(L_BK_VE_SRC(0x62), vstart);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_Set_VScale_Output_Line_Number()
/// @brief \b Function \b Description : Set VE vertical output line number in scale mode
///
/// @param <IN>        \b line
/// @return none
////////////////////////////////////////////////////////////////////////////////
void Hal_VE_Set_VScale_Output_Line_Number(MS_U16 line)
{
    MDrv_Write2Byte(L_BK_VE_DISC(0x68), line);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_Set_HScale_Output_Line_Number()
/// @brief \b Function \b Description : Set VE Horizontal output line number in scale mode
///
/// @param <IN>        \b line
/// @return none
////////////////////////////////////////////////////////////////////////////////
void Hal_VE_Set_HScale_Output_Line_Number(MS_U16 line)
{
    MDrv_Write2Byte(L_BK_VE_DISC(0x69), line);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_Adjust_FrameStart()
/// @brief \b Function \b Description : the frame start is used to adjust output video
///                                     (in pixel)
///
/// @param <IN>        \b pixel_offset
/// @return VE_Result
////////////////////////////////////////////////////////////////////////////////
VE_Result Hal_VE_Adjust_FrameStart(MS_S16 pixel_offset)
{
    MS_U16 reg_av_st = 0;

    reg_av_st = MDrv_Read2Byte(L_BK_VE_ENC(0x25));
    // double the offset since the value of reg_hf_st and reg_av_st are
    // duplicated in HW register
    reg_av_st += pixel_offset * 2;

    MDrv_Write2Byte(L_BK_VE_ENC(0x2F), reg_av_st);  // set the offset into reg_hf_st
    return E_VE_OK;
}

#define MS_VE_CRC6_START_BIT    (14)
static void _Hal_VE_CalcCRC6(MS_U32 *u32wssdata)
{
    // initialization, bit5-bit0 = b'111111
    MS_U32 u32TempWssData = *u32wssdata;
    MS_U8 u8Crc = 0x3F, u8CrcTemp = 0x3F;
    MS_U8 u8Index = 0;
    MS_U8 u8LowestBit, u8Bit5, u8Bit4 = 0x0;

    VE_DBG(printf("[%s][%d] u32TempWssData = 0x%05lx \n", __FUNCTION__, __LINE__, u32TempWssData));
    /* for each bit, do operations as below:
     * (1) for last 4 bits, shift right 1 bit
     * (2) for the first 2 bits, do xor operation base on different algorithm
     */
    for(u8Index = 0; u8Index < MS_VE_CRC6_START_BIT; u8Index++)
    {
        u8LowestBit = (( u32TempWssData >> u8Index) & 0x1);
        u8CrcTemp = (u8Crc >> 1) & 0x0F;            // for last 4 CRC bits (b3-b0), temp CRC = CRC shift right 1 bit
        u8Bit5 = u8LowestBit ^ ( u8Crc & BIT(0) );  // for CRC bit5, let lowest bit of WSS to do xor with lowest CRC bit
        u8CrcTemp = u8CrcTemp | ((u8Bit5) << 5);
        u8Bit4 = (u8CrcTemp >> 5) ^ (u8Crc >> 5);   // for CRC bit4, let bit5 of CRC to do xor with bit 5 of tempCRC
        u8CrcTemp = u8CrcTemp | ((u8Bit4) << 4);
        u8Crc = u8CrcTemp;                          // re-store CRC
        VE_DBG(printf("[%s][%d] u8Crc = 0x%x \n", __FUNCTION__, __LINE__, u8Crc));
    }

    // reorder the result of WSS data, b19-b0
    u32TempWssData = (u32TempWssData & 0x3FFF) | (u8Crc << MS_VE_CRC6_START_BIT);
    *u32wssdata = u32TempWssData;
    VE_DBG(printf("[%s][%d] u32WssData = 0x%05lx \n", __FUNCTION__, __LINE__, u32TempWssData));
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_set_wss525_data()
/// @brief \b Function \b Description : Set the WSS data of video encoder for 525i System
///
/// @param <IN>        \b bEn       : Enable, Disable
/// @param <IN>        \b u8WSSData : The WSS data
/// @param <OUT>       \b None :
/// @param <RET>       \b None :
/// @param <GLOBAL>    \b None :
////////////////////////////////////////////////////////////////////////////////
VE_Result Hal_VE_set_wss525_data(MS_BOOL ben, MS_U32 u32wssdata)
{
    MS_U16 u16NtscWssInfo1 = 0, u16NtscWssInfo2 = 0;

    if(ben)
    {
        if(MDrv_Read2Byte(L_BK_VE_SRC(0x42)) == 0x1e0)    //NTSC & PAL-M
        {
            /*
             * The WSS consists of 2 bits of star code(generated by HW),
             * 14 bits of data (b13 - b0) and 6 bits of CRC (b19:b14).
             * b19:b0 -> info2, info1
             * b19:b0 -> 0000, 0000 0000 0000 0000
             *
             * Cyclic Redundancy Check (CRC):
             * The Generator polynomial G(X) of the CRC is G(X) = X6 + X + 1,
             * in which, with an initial state of all 1's
             */
            _Hal_VE_CalcCRC6(&u32wssdata);

            u16NtscWssInfo2 = (MS_U16)( (u32wssdata >>16) & 0x000F );
            u16NtscWssInfo1 = (MS_U16)( u32wssdata & 0xFFFF);
            MDrv_Write2Byte(L_BK_VE_ENC(0x6F), (u16NtscWssInfo2));
            MDrv_Write2Byte(L_BK_VE_ENC(0x6E), (u16NtscWssInfo1));
            MDrv_Write2Byte(L_BK_VE_ENC(0x71), (u16NtscWssInfo2));
            MDrv_Write2Byte(L_BK_VE_ENC(0x70), (u16NtscWssInfo1));
            MDrv_Write2Byte(L_BK_VE_ENC(0x56), 0x0014);
            MDrv_Write2Byte(L_BK_VE_ENC(0x57), 0x0014);
            MDrv_Write2Byte(L_BK_VE_ENC(0x6C), 0x011B);
            MDrv_Write2Byte(L_BK_VE_ENC(0x6D), 0x011B);
        }
        else    //PAL
        {
            printf("[%s][%d] ERROR: this function only for 525i System! \n", __FUNCTION__, __LINE__);
            return E_VE_NOT_SUPPORT;
        }
        MDrv_WriteByteMask(L_BK_VE_ENC(0x2E), BIT(7)|BIT(2), BIT(7)|BIT(2));
    }
    else
    {
        MDrv_WriteByteMask(L_BK_VE_ENC(0x2E), 0x00, BIT(2));
    }

    return E_VE_OK;
}

MS_U32 Hal_VE_get_wss525_data(void)
{
    MS_U32 u32Wss525Data = 0;

    u32Wss525Data = (MS_U32)(MDrv_Read2Byte(L_BK_VE_ENC(0x6F)) & 0xF) << 16;
    u32Wss525Data = u32Wss525Data | (MS_U32)(MDrv_Read2Byte(L_BK_VE_ENC(0x6E)) & 0xFFFF);

    return ( u32Wss525Data );
}

void Hal_VE_OnOffWSS(MS_BOOL ben)
{
    if(ben)
    {
        MDrv_WriteByteMask(L_BK_VE_ENC(0x2E), BIT(2), BIT(2));
    }
    else
    {
        MDrv_WriteByteMask(L_BK_VE_ENC(0x2E), 0, BIT(2));
    }
}
MS_BOOL Hal_VE_GetWSSStatus(void)
{
    if(MDrv_ReadRegBit(L_BK_VE_ENC(0x2E), BIT(2)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
void Hal_VE_OnOffMV(MS_BOOL ben)
{
    if(ben)
    {
        MDrv_WriteByte(L_BK_VE_ENC(0x78), _u8MV_N00);
    }
    else
    {
        _u8MV_N00 = MDrv_ReadByte(L_BK_VE_ENC(0x78));
        MDrv_WriteByte(L_BK_VE_ENC(0x78), 0x00);
    }
}
MS_BOOL Hal_VE_GetMVStatus(void)
{
    if(MDrv_ReadByte(L_BK_VE_ENC(0x78)) != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
void Hal_VE_OnOffDCS(MS_BOOL ben)
{
    if(ben)
    {
        MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x74), BIT(3), BIT(3));
    }
    else
    {
        MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x74), 0, BIT(3));
    }
}
MS_BOOL Hal_VE_GetDCSStatus(void)
{
    if(MDrv_ReadRegBit(L_BK_VE_ENC_EX(0x74), BIT(3)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
//-------------------------------------------------------------------------------------------------
/// @brief \b Function \b Name: Hal_VE_set_dolby_verifier_addr
/// @brief \b Function \b Description : Set frame buffer address for dolby verifier
/// @param <IN> \b phyAddr : physical address of the frame buffer
///
/// @return none
//-------------------------------------------------------------------------------------------------
void Hal_VE_set_dolby_verifier_addr(MS_PHY phyAddr)
{
    _phyDolbyVerifierBufAddr = phyAddr;
    return;
}

//-------------------------------------------------------------------------------------------------
/// @brief \b Function \b Name: Hal_VE_get_dolby_verifier_buf_size
/// @brief \b Function \b Description : Get needed frame buffer size for dolby verifier
/// @param <IN>        \b none
///
/// @return buffer size
//-------------------------------------------------------------------------------------------------
MS_U32 Hal_VE_get_dolby_verifier_buf_size(void)
{
    return VE_DOLBY_VERIFIER_BUF_SIZE;
}

VE_Result Hal_VE_ReloadSetting(void)
{

	#ifndef MBOOT

	//OS_DELAY_TASK(300);
	Hal_VE_sofeware_reset(ENABLE); // software reset
	Hal_VE_set_reg_load(ENABLE);// load register,but not affect bit3(VBI output)

	//MsOS_DelayTask(5) ; // delay 5 ms
	OS_DELAY_TASK(30);

	#endif

	Hal_VE_set_reg_load(DISABLE);// load register,but not affect bit3(VBI output)
	OS_DELAY_TASK(5);
	Hal_VE_sofeware_reset(DISABLE); // software reset

	return E_VE_OK;
}

#endif
