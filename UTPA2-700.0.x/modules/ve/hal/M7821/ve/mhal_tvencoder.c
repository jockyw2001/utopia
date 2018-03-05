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
// (!!¡ÓMStar Confidential Information!!L) by the recipient.
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
#include "drvMIU.h"
#include "halCHIP.h"
#ifdef CONFIG_MSTAR_CLKM
#include "drvCLKM.h"
#endif

#define VE_DBG(msg) //msg
#define MAX_COUNT_OF_TABLE          2000
#define U32_END_OF_TABLE_VALUE      0xffffff
MS_VIRT _VE_RIU_BASE;

#define _SC_PK_L_(bank, addr)   (((MS_U16)(bank) << 8) | (MS_U16)((addr)*2))
#define _SC_PK_H_(bank, addr)   (((MS_U16)(bank) << 8) | (MS_U16)((addr)*2+1))

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
            MDrv_WriteByte(L_BK_VE_ENC(0x43), 0x5A);
            MDrv_WriteByte(L_BK_VE_ENC(0x46), 0x80);
            break;

        case VE_OUT_CVBS_RGB:
            MDrv_WriteByte(L_BK_VE_ENC(0x41), 0x95);
            MDrv_WriteByte(L_BK_VE_ENC(0x42), 0x4B);
            MDrv_WriteByte(L_BK_VE_ENC(0x43), 0x90);
            MDrv_WriteByte(L_BK_VE_ENC(0x44), 0x49);
            MDrv_WriteByte(L_BK_VE_ENC(0x45), 0x32);
            MDrv_Write2Byte(L_BK_VE_ENC(0x46), 0x102);
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
            MDrv_WriteByte(L_BK_VE_ENC(0x43), 0x55);
            MDrv_WriteByte(L_BK_VE_ENC(0x46), 0x78);
            break;

        case VE_OUT_CVBS_RGB:
            MDrv_WriteByte(L_BK_VE_ENC(0x41), 0x8E);
            MDrv_WriteByte(L_BK_VE_ENC(0x42), 0x4B);
            MDrv_WriteByte(L_BK_VE_ENC(0x43), 0x88);
            MDrv_WriteByte(L_BK_VE_ENC(0x44), 0x45);
            MDrv_WriteByte(L_BK_VE_ENC(0x45), 0x2F);
            MDrv_WriteByte(L_BK_VE_ENC(0x46), 0xF3);
            break;
        }
    }
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
    // for mahnattan MIU setting base on
    //REG_SC_BK7F_11_L for group select MIU or by IP
    //REG_SC_BK7F_10_L for IP select
    if (u8MIU_Seltype == E_CHIP_MIU_3)
    {
        MDrv_MIU_SelMIU(MIU_CLIENT_VE_W,MIU_SELTYPE_MIU3);
        MDrv_MIU_SelMIU(MIU_CLIENT_VE_R,MIU_SELTYPE_MIU3);
    }
    else if (u8MIU_Seltype == E_CHIP_MIU_2)
    {
        MDrv_MIU_SelMIU(MIU_CLIENT_VE_W,MIU_SELTYPE_MIU2);
        MDrv_MIU_SelMIU(MIU_CLIENT_VE_R,MIU_SELTYPE_MIU2);
    }
    else if (u8MIU_Seltype == E_CHIP_MIU_1)
    {
        MDrv_MIU_SelMIU(MIU_CLIENT_VE_W,MIU_SELTYPE_MIU1);
        MDrv_MIU_SelMIU(MIU_CLIENT_VE_R,MIU_SELTYPE_MIU1);
    }
    else
    {
        MDrv_MIU_SelMIU(MIU_CLIENT_VE_W,MIU_SELTYPE_MIU0);
        MDrv_MIU_SelMIU(MIU_CLIENT_VE_R,MIU_SELTYPE_MIU0);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_SetMemAddr()
/// @brief \b Function \b Description : set VE read/write memory address
///
/// @param <IN>        \b pu32RDAddress : Pointer of VE Memory Read address
/// @param <IN>        \b pu32WRAddress : Pointer of VE Memory Write address
/// @param <OUT>       \b None :
/// @param <RET>       \b None :
/// @param <GLOBAL>    \b None :
////////////////////////////////////////////////////////////////////////////////
void Hal_VE_SetMemAddr(MS_PHY *phyRDAddress, MS_PHY *phyWRAddress)
{
    MS_PHY phyMIUAddress;
    if(phyRDAddress != NULL)
    {
        // { reg_miu_radr_h, reg_miu_radr_l }
        phyMIUAddress = *phyRDAddress/BYTE_PER_WORD;
        MDrv_Write2Byte(L_BK_VE_SRC(0x01), (MS_U16)(phyMIUAddress >> 0));
        MDrv_Write2Byte(L_BK_VE_SRC(0x02), (MS_U16)(phyMIUAddress >> 16));
    }
    if(phyWRAddress != NULL)
    {
        // { reg_miu_wadr_h, reg_miu_wadr_l }
        phyMIUAddress = *phyWRAddress/BYTE_PER_WORD;
        MDrv_Write2Byte(L_BK_VE_SRC(0x7B), (MS_U16)(phyMIUAddress >> 0));
        MDrv_Write2Byte(L_BK_VE_SRC(0x7C), (MS_U16)(phyMIUAddress >> 16));
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: Hal_VE_init()
/// @brief \b Function \b Description : Initiate VE
///
/// @param <IN>        \b u32MIUAddress : Memory address of VE
/// @param <OUT>       \b None :
/// @param <RET>       \b None :
/// @param <GLOBAL>    \b None :
////////////////////////////////////////////////////////////////////////////////
void Hal_VE_init(void)
{
    MS_U16 u16temp;
    //MDrv_WriteByte(H_BK_CHIPTOP(0x19), 0x80);
    MDrv_WriteByteMask(L_BK_CLKGEN0(0x24), 0x00, 0x0f);   // clock of ve
    MDrv_WriteByteMask(H_BK_CLKGEN0(0x24), 0x08, 0x0f);   // clock of vedac
    MDrv_WriteByteMask(L_BK_VE_ENC(0x03),0x00,0x10);

    // read FIFO control
    MDrv_Write2Byte(L_BK_VE_SRC(0x24),0x2a00);
    MDrv_Write2Byte(L_BK_VE_SRC(0x25),0x00ff);
    MDrv_Write2Byte(L_BK_VE_SRC(0x26),0x107f);
    MDrv_Write2Byte(L_BK_VE_SRC(0x27),0x007f);
    MDrv_Write2Byte(L_BK_VE_SRC(0x32),0x002A);
    MDrv_Write2Byte(L_BK_VE_SRC(0x36),0x003f);
    MDrv_Write2Byte(L_BK_VE_SRC(0x37),0x7F36);
    MDrv_Write2Byte(L_BK_VE_SRC(0x38),0x003f);
    MDrv_Write2Byte(L_BK_VE_SRC(0x6D),0x00ff);

    // initial TTX VBI
    MDrv_Write2Byte(L_BK_VE_SRC(0x06),0);
    MDrv_Write2Byte(L_BK_VE_SRC(0x43),(MDrv_Read2Byte(L_BK_VE_SRC(0x43))&0xff00)|0x0039);
    u16temp = (MDrv_Read2Byte(L_BK_VE_SRC(0x08))&0x00ff)|0xfb00;
    MDrv_Write2Byte(L_BK_VE_SRC(0x08), u16temp);
    MDrv_Write2Byte(L_BK_VE_SRC(0x46),(MDrv_Read2Byte(L_BK_VE_SRC(0x46))&0xff00)|0x0003);
    MDrv_WriteByte(L_BK_VE_ENC(0x07), 0x00);
    MDrv_Write2Byte(L_BK_VE_SRC(0x00),MDrv_Read2Byte(L_BK_VE_SRC(0x00))|0x0008);

    //test for VE output stable temp solution,2007/07/05
    MDrv_WriteByte(L_BK_VE_SRC(0x44),0xf0);
    // 256-bit miu bus : active pixel count / 16
    //  (45 = 720/16)
    MDrv_WriteByte(H_BK_VE_SRC(0x44),0x2D);
    MDrv_WriteByteMask(L_BK_VE_SRC(0x46),0xd0,0xf0);

    MDrv_WriteRegBit(H_BK_VE_SRC(0x7E), 1, BIT(0)); //enable black boundary

    MDrv_Write2Byte(L_BK_VE_SRC(0x11), 0x0080); //default VE Hardware debug mode VE ToDo
    MDrv_WriteRegBit(L_BK_VE_SRC(0x5A), DISABLE, BIT(0));// disable scaler in

    //sync delay pipe from write clock to read clock domain (add 2 pipe flip-flop)
    MDrv_WriteByteMask(L_BK_VE_SRC(0x7E),0x02,BIT(1));
    MDrv_Write2ByteMask(L_BK_VE_SRC(0x7D),BIT(14),BIT(14)); // mode for trigger TVE
    MDrv_Write2ByteMask(L_BK_VE_ENC(0x3F),BIT(10),BIT(10)); // always do sync with TVE source
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
    MS_BOOL bRet;
    PMS_VE_Out_VideoSYS pVideoSysTbl = NULL;

    if(VideoSystem >= MS_VE_VIDEOSYS_NUM)
    {
        bRet = FALSE;
    }
    else
    {
        bRet = TRUE;
        if(VideoSystem == MS_VE_PAL)
            pVideoSysTbl = &VE_OUT_VIDEOSTD_TBL[MS_VE_PAL];
        else
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

        MDrv_WriteRegBit(L_BK_VE_SRC(0x00), 1, BIT(5)); // load register
        MDrv_WriteRegBit(L_BK_VE_SRC(0x00), 0, BIT(5));
    }

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
    // not supported
    return TRUE;
}

//------------------------------------------------------------------------------
/// Enable/Disable customized table
///
/// @param  -pTbl \b IN: pointer to the table
//------------------------------------------------------------------------------
void Hal_VE_EnableCusTable(MS_BOOL bEnable)
{
    // not supported
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

        MDrv_Write2Byte(L_BK_VE_ENC(0x56), 0x0017);
        MDrv_Write2Byte(L_BK_VE_ENC(0x57), 0x0017);
        MDrv_Write2Byte(L_BK_VE_ENC(0x6C), 0x0000);
        MDrv_Write2Byte(L_BK_VE_ENC(0x6D), 0x0000);

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
    MDrv_Power_Set_HwClock(E_HWCLK_VEDAC_ONOFF, POWER_ON);
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
    MDrv_Power_Set_HwClock(E_HWCLK_VEDAC_ONOFF, POWER_DOWN);
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
    UNUSED(u16chnl);
    UNUSED(u16mux);
    UNUSED(u16clamp);
    UNUSED(u16test);
// The register below has been changed from what is was, and it may cause component output no signal(ADC39[13])
// So comment it temporarily
/*
    MS_U8 u8Adc38L = u16chnl & 0xFF;
    MS_U8 u8Adc39L = u16mux & 0xFF;
    MS_U8 u8Adc39H = (u16mux >> 8);
    MS_U8 u8Adc3AL = u16clamp & 0xFF;
    MS_U8 u8Adc3BH = u16test & 0xFF;


    MDrv_WriteByte(L_BK_ADC_ATOP(0x38), u8Adc38L);
    MDrv_WriteByte(L_BK_ADC_ATOP(0x39), u8Adc39L);
    MDrv_WriteByte(H_BK_ADC_ATOP(0x39), u8Adc39H);
    MDrv_WriteByte(L_BK_ADC_ATOP(0x3A), u8Adc3AL);
    MDrv_WriteByte(H_BK_ADC_ATOP(0x3B), u8Adc3BH);*/
}

void Hal_VE_set_frc(MS_BOOL bfulldrop, MS_U32 u32fullnum, MS_U32 u32emptynum, MS_BOOL binterlace, MS_BOOL b3FrameMode)
{
    MDrv_Write4Byte(L_BK_VE_SRC(0x21),  u32fullnum);

    if(bfulldrop)
        MDrv_WriteRegBit(L_BK_VE_SRC(0x20), 0, BIT(0)); // enable Full drop
    else
        MDrv_WriteRegBit(L_BK_VE_SRC(0x20), 1, BIT(0)); // disable Full drop

    MDrv_Write4Byte(L_BK_VE_SRC(0x30),  u32emptynum);

	// T3 Always use normal mode (disable de-interlace)
    if(binterlace)
	{
        MDrv_WriteByteMask(L_BK_VE_SRC(0x35), 0x00, BIT(3)|BIT(2));
        //MDrv_WriteByteMask(L_BK_VE_SRC(0x46), 0xd0, 0xf0);
    }
    else
    {
        //new mode for VE only for progressive.
        if(b3FrameMode)
        {
            MDrv_WriteByteMask(L_BK_VE_SRC(0x35), BIT(3), BIT(3)|BIT(2));
        }
        else
        {
            MDrv_WriteByteMask(L_BK_VE_SRC(0x35), BIT(3)|BIT(2), BIT(3)|BIT(2));
        }
        //MDrv_WriteByteMask(L_BK_VE_SRC(0x46), 0x00, 0xf0);
    }

}

void Hal_VE_set_field_size(MS_U16 u16FieldSize)
{
    MDrv_Write2Byte(L_BK_VE_SRC(0x45), u16FieldSize);
}

void Hal_VE_set_field_inverse(MS_BOOL ben)
{
    MDrv_WriteRegBit(L_BK_VE_SRC(0x10), !ben, BIT(1));
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
    MDrv_WriteByteMask(L_BK_VE_SRC(0x00), u16enval, BIT(7)|BIT(5)|BIT(4)|BIT(2)|BIT(1));
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
    MDrv_Write2Byte(L_BK_VE_SRC(0x63), (u32ratio >> 16));
    MDrv_Write2Byte(L_BK_VE_SRC(0x62), (u32ratio & 0xFFFF));

    // enable/disable upscale
    if(ben) {
        MDrv_WriteRegBit(H_BK_VE_SRC(0x61), BIT(0), BIT(0));
    } else {
        MDrv_WriteRegBit(H_BK_VE_SRC(0x61), 0x0, BIT(0));
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
    // not supported
}

void Hal_VE_set_v_scaling_Traditional(MS_BOOL ben, MS_U16 u16ratio)
{
    //u16ratio -=7;//experience value to complement the receiver's overscan
    Hal_VE_set_v_scaling(ben, u16ratio);
}

void Hal_VE_set_v_initfactor(MS_BOOL binterlace, MS_U32 u32V_Ratio)
{
    MS_U16 u16init_factor = 0;

    if(u32V_Ratio != 0)
    {
        u16init_factor = ((0x800*512)/u32V_Ratio) -512; //(0x800/reg(0x103b17) -1)/2x1024 <=2047
    }

    if(u16init_factor >= 2047)
    {
        u16init_factor = 2047;
    }

    if(binterlace)
    {
        MDrv_Write2ByteMask(L_BK_VE_SRC(0x19),u16init_factor,0x7FF);
        MDrv_Write2ByteMask(L_BK_VE_SRC(0x19),BIT(15),BIT(15));
    }
    else
    {
        MDrv_Write2Byte(L_BK_VE_SRC(0x19),0x8000);
    }
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


void Hal_VE_set_inputsource(PMS_VE_InputSrc_Info pInputSrcInfo)
{
    MS_U8 u8Clk_Mux = 0, u8Data_Mux = 0;
    switch(pInputSrcInfo->eInputSrcType)
    {
        case MS_VE_SRC_DTV:
            #if (ENABLE_VE_SUBTITLE)
            u8Data_Mux = VE_IPMUX_SC_IP1;
            #else
            u8Data_Mux = VE_IPMUX_MVOP;
            #endif
            u8Clk_Mux = CKG_VE_IN_DFT_LIVE; //CKG_VE_IN_CLK_MPEG0;

            break;

        case MS_VE_SRC_MAIN:
            u8Data_Mux = VE_IPMUX_SC_IP1;
            u8Clk_Mux  = CKG_VE_IN_1;
            break;

        case MS_VE_SRC_SUB:
            u8Data_Mux = VE_IPMUX_SC_SUBIP;
            u8Clk_Mux  = CKG_VE_IN_CLK_MPEG0;
            break;

        case MS_VE_SRC_DTV_FROM_MVOP:
            u8Data_Mux = VE_IPMUX_MVOP;
            u8Clk_Mux = CKG_VE_IN_CLK_MPEG0;
            break;

        case MS_VE_SRC_DSUB:
        #if (ENABLE_VE_SUBTITLE)
            u8Data_Mux = VE_IPMUX_SC_IP1;
            u8Clk_Mux  = CKG_VE_IN_1;
        #else
            u8Data_Mux = VE_IPMUX_ADC_A;
            u8Clk_Mux  = CKG_VE_IN_CLK_ADC;
        #endif
            break;

        case MS_VE_SRC_COMP:
        #if (ENABLE_VE_SUBTITLE)
            u8Data_Mux = VE_IPMUX_SC_IP1;
            u8Clk_Mux  = CKG_VE_IN_1;
        #else
            u8Data_Mux = VE_IPMUX_ADC_A;
            u8Clk_Mux  = CKG_VE_IN_CLK_ADC;
        #endif
            break;

        case MS_VE_SRC_ATV:
        case MS_VE_SRC_CVBS0:
        case MS_VE_SRC_CVBS1:
        case MS_VE_SRC_CVBS2:
        case MS_VE_SRC_CVBS3:
        case MS_VE_SRC_SVIDEO:
            u8Data_Mux = VE_IPMUX_VD;
            u8Clk_Mux  = CKG_VE_IN_CLK_VD;
            break;

        case MS_VE_SRC_HDMI_A:
        case MS_VE_SRC_HDMI_B:
        case MS_VE_SRC_HDMI_C:
        #if (ENABLE_VE_SUBTITLE)
            u8Clk_Mux  = CKG_VE_IN_1;
            u8Data_Mux = VE_IPMUX_SC_IP1;
        #else
            u8Clk_Mux  = CKG_VE_IN_CLK_DVI;
            u8Data_Mux = VE_IPMUX_HDMI_DVI;
        #endif
            break;

        case MS_VE_SRC_SCALER:
            u8Clk_Mux  = CKG_VE_IN_0_;
            u8Data_Mux = VE_IPMUX_CAPTURE;
            Hal_VE_set_rgb_in(ENABLE); // enable RGB in
            break;
        default:
            u8Clk_Mux  = 0;
            u8Data_Mux = 0;
            break;
    }
    if(pInputSrcInfo->eInputSrcType == MS_VE_SRC_SUB)
    {
        switch(pInputSrcInfo->eInputSrcOfMixedSrc)
        {
            case MS_VE_SRC_DTV:
                u8Clk_Mux = CKG_VE_IN_CLK_MPEG0;
                break;
            case MS_VE_SRC_DTV_FROM_MVOP:
                u8Clk_Mux = CKG_VE_IN_CLK_MPEG0;
                break;
            case MS_VE_SRC_DSUB:
                u8Clk_Mux  = CKG_VE_IN_CLK_ADC;
                break;
            case MS_VE_SRC_COMP:
                u8Clk_Mux  = CKG_VE_IN_CLK_ADC;
                break;
            case MS_VE_SRC_ATV:
            case MS_VE_SRC_CVBS0:
            case MS_VE_SRC_CVBS1:
            case MS_VE_SRC_CVBS2:
            case MS_VE_SRC_CVBS3:
            case MS_VE_SRC_SVIDEO:
                u8Data_Mux = VE_IPMUX_VD;
                u8Clk_Mux  = CKG_VE_IN_CLK_VD;
                break;
            case MS_VE_SRC_HDMI_A:
            case MS_VE_SRC_HDMI_B:
            case MS_VE_SRC_HDMI_C:
                u8Clk_Mux  = CKG_VE_IN_CLK_DVI;
                break;
            case MS_VE_SRC_SCALER:
                u8Clk_Mux  = CKG_VE_IN_0_;
                break;
            default:
                break;
        }
    }
    else if(pInputSrcInfo->eInputSrcType == MS_VE_SRC_MAIN)
    {
        switch(pInputSrcInfo->eInputSrcOfMixedSrc)
        {
            case MS_VE_SRC_ATV:
            case MS_VE_SRC_CVBS0:
            case MS_VE_SRC_CVBS1:
            case MS_VE_SRC_CVBS2:
            case MS_VE_SRC_CVBS3:
            case MS_VE_SRC_SVIDEO:
                u8Data_Mux = VE_IPMUX_VD;
                u8Clk_Mux  = CKG_VE_IN_CLK_VD;
                break;
            default:
                break;
        }
    }
    if(u8Data_Mux == VE_IPMUX_VD)
    {
        //Special for VD sources
        MDrv_WriteRegBit(L_BK_VE_SRC(0x10), BIT(3) , BIT(3));
    }
    Hal_VE_set_mux(u8Clk_Mux, u8Data_Mux);
    /*
    if(MS_VE_SRC_SCALER == pInputSrcInfo->eInputSrcType)
    {
        // Only for source is Scaler OP2
        MDrv_WriteRegBit(L_BK_VE_SRC(0x7E), 1 , BIT(3));
    }
    else
    {
        MDrv_WriteRegBit(L_BK_VE_SRC(0x7E), 0 , BIT(3));
    }*/
}

void Hal_VE_set_mux(MS_U8 u8clk, MS_U8 u8data)
{
    MDrv_WriteByteMask(REG_IPMUX_02_L, u8data,0xF); // input select
    //MDrv_WriteByte(REG_CKG_VE_IN, u8clk); // idclk
    MDrv_WriteByte(L_BK_CLKGEN0(0x25), u8clk);  // idclk

    MDrv_WriteRegBit(L_BK_CLKGEN0(0x5a), BIT(4), BIT(4));  // CLK_VE_IN setting user mode

    MDrv_WriteByteMask(REG_SC_BK02_2C_L, BIT(3)|BIT(2),BIT(3)|BIT(2)); // enable IP2VE
}


void Hal_VE_set_rgb_in(MS_BOOL ben)
{
    if(ben)
        MDrv_WriteByteMask(L_BK_VE_SRC(0x10), (BIT(6)|BIT(5)), (BIT(6)|BIT(5)));
    else
        MDrv_WriteByteMask(L_BK_VE_SRC(0x10), 0, BIT(6)|BIT(5));
}

void Hal_VE_set_ccir656_in(MS_BOOL ben)
{
    if(ben)
        MDrv_WriteByteMask(L_BK_VE_SRC(0x10), BIT(4), BIT(4)); // video source isCCIR656
    else
        MDrv_WriteByteMask(L_BK_VE_SRC(0x10), 0, BIT(4)); // video source is YCbCr
}

void Hal_VE_set_source_interlace(MS_BOOL ben)
{
    #ifndef VE_SOURCE_AFTER_DI
    if(ben)
        MDrv_WriteRegBit(L_BK_VE_SRC(0x10), 1, BIT(0));
    else
        MDrv_WriteRegBit(L_BK_VE_SRC(0x10), 0, BIT(0));
    #else
    MDrv_WriteRegBit(L_BK_VE_SRC(0x10), 0, BIT(0)); // For monaco, always capture after DI, so source always P mode
    #endif
}

void Hal_VE_set_clk_on_off(MS_BOOL ben)
{
    //for central control power down
#ifdef CONFIG_MSTAR_CLKM

    MDrv_WriteByteMask(L_BK_CLKGEN0(0x25), !ben, 0x01);   // clock of vein Ena/disable

  #ifdef MSOS_TYPE_LINUX_KERNEL
    //kernel mode
    if(ben)
    {
        MS_S32 handle1 = 0, handle2= 0;
        handle1 = get_handle("g_clk_ve");
        set_clk_source(handle1,"clk_ve");

        handle2 = get_handle("g_clk_vedac_digital");
        set_clk_source(handle2,"clk_vedac");
    }
    else
    {
        MS_S32 handle1 = 0, handle2= 0;
        handle1 = get_handle("g_clk_ve");
        clk_gate_disable(handle1);

        handle2 = get_handle("g_clk_vedac_digital");
        clk_gate_disable(handle2);
    }
  #else
    //user mode
    if(ben)
    {
        MS_S32 handle1 = 0, handle2= 0;
        handle1 = Drv_Clkm_Get_Handle("g_clk_ve");
        Drv_Clkm_Set_Clk_Source(handle1,"clk_ve");

        handle2 = Drv_Clkm_Get_Handle("g_clk_vedac_digital");
        Drv_Clkm_Set_Clk_Source(handle2,"clk_vedac");
    }
    else
    {
        MS_S32 handle1 = 0, handle2= 0;
        handle1 = Drv_Clkm_Get_Handle("g_clk_ve");
        Drv_Clkm_Clk_Gate_Disable(handle1);

        handle2 = Drv_Clkm_Get_Handle("g_clk_vedac_digital");
        Drv_Clkm_Clk_Gate_Disable(handle2);
    }
  #endif

#else
    MDrv_WriteByteMask(L_BK_CLKGEN0(0x25), !ben, 0x01);   // clock of vein Ena/disable
    MDrv_WriteByteMask(L_BK_CLKGEN0(0x24), !ben, 0x01);   // clock of ve Ena/disable
    MDrv_WriteByteMask(H_BK_CLKGEN0(0x24), !ben, 0x01);   // clock of vedac Ena/disable
#endif
}

void Hal_VE_set_ve_on_off(MS_BOOL ben)
{
    MDrv_WriteRegBit(L_BK_VE_SRC(0x00), ben, BIT(0));
}

void Hal_VE_set_blackscreen(MS_BOOL ben)
{
    MDrv_Write2Byte(L_BK_VE_SRC(0x39), 0x8080); //Set VE YPbPr black
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
    //trig transition
    if (bEnable)
    {
       if (MDrv_Read2Byte(L_BK_VE_SRC(0x43))&0x0002)

        {
            MDrv_Write2Byte(L_BK_VE_SRC(0x43),MDrv_Read2Byte(L_BK_VE_SRC(0x43))&0xfffd);
        }
        else
        {
            MDrv_Write2Byte(L_BK_VE_SRC(0x43),MDrv_Read2Byte(L_BK_VE_SRC(0x43))|0x0002);
        }
    }
}

//------------------------------------------------------------------------------
/// VE set TT buffer address
/// @return none
//------------------------------------------------------------------------------
void Hal_VE_Set_ttx_Buffer(MS_PHY u32StartAddr, MS_U32 u32Size)
{
    MS_PHY u32VETTX_Addr,u32TTXdatasize;
    u32VETTX_Addr = u32StartAddr /BYTE_PER_WORD;
    u32TTXdatasize = (u32Size/BYTE_PER_WORD)-1;
    MDrv_Write4Byte(L_BK_VE_SRC(0x03),u32VETTX_Addr);
    MDrv_Write4Byte(L_BK_VE_SRC(0x05),u32TTXdatasize);
    /*Drv_TVEncoder_Write(BK_VE_SRC(REG_TVE_VBI_ADDR_L), (MS_U16)u32StartAddr);
    Drv_TVEncoder_Write(BK_VE_SRC(REG_TVE_VBI_ADDR_H), u32StartAddr>>16);
    Drv_TVEncoder_Write(BK_VE_SRC(REG_TVE_VBI_MEMSIZE_L), (MS_U16)u32Size);
    Drv_TVEncoder_Write(BK_VE_SRC(REG_TVE_VBI_MEMSIZE_H), u32Size>>16);*/
    //Drv_TVEncoder_WriteBit(BK_VE_SRC(0x17), TRUE, BIT1); //stop mode
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
    // not implemented
}

//------------------------------------------------------------------------------
/// VE Set VBI TT active line by the given bitmap
/// @param <IN>\b u32Bitmap: a bitmap that defines whick physical lines the teletext lines are to be inserted.
/// @return none
//------------------------------------------------------------------------------
void Hal_VE_Set_VbiTtxActiveLinesBitmap(MS_U32 u32Bitmap)
{
    // not implemented
}

//------------------------------------------------------------------------------
/// VE Set VE display window offset
/// @return none
//------------------------------------------------------------------------------
void Hal_VE_Set_winodw_offset(MS_PHY phyoffsetaddr)
{
    Hal_VE_SetMemAddr(NULL, &phyoffsetaddr);
}

MS_U32 Hal_VE_ConvertARGB2YUV(MS_U32 u32aRGB)
{
    MS_S16 s16R, s16G, s16B, s16Y, s16U, s16V;
    MS_U32 u32YUV = 0;

    s16R = (u32aRGB>>16) & 0xFF;
    s16G = (u32aRGB>>8) & 0xFF;
    s16B = (u32aRGB>>0) & 0xFF;

    //////////////////////////////////////////////////
    //RGB to YUV Formula
    //Y = R * .299 + G * .587 + B * .114;           //
    //U = R * -.169 + G * -.331 + B * .500 + 128.;  //
    //V = R * .500 + G * -.419 + B * -.081 + 128.; //
    //////////////////////////////////////////////////

    s16Y = (s16R*299/1000) + (s16G*587/1000) + (s16B*114/1000);
    s16U = -(s16R*169/1000) - (s16G*331/1000) + (s16B*500/1000) + 128;
    s16V = (s16R*500/1000) - (s16G*419/1000) - (s16B*81/1000) + 128;

    if(s16Y > 0xFF)
    {
        s16Y = 0xFF;
    }
    if(s16U > 0xFF)
    {
        s16U = 0xFF;
    }
    if(s16V > 0xFF)
    {
        s16V = 0xFF;
    }

    u32YUV = (s16Y<<16) + (s16U<<8) + (s16V<<0);

    return u32YUV;
}

//------------------------------------------------------------------------------
/// Set VE Frame Color
/// @return none
//------------------------------------------------------------------------------
VE_Result Hal_VE_SetFrameColor(MS_U32 u32aRGB)
{
    MS_U32 u32YUV;

    u32YUV = Hal_VE_ConvertARGB2YUV(u32aRGB);

    MDrv_WriteByteMask(H_BK_VE_ENC_EX(0x3C),(u32YUV >> 16),0xFF);///< V
    MDrv_WriteByteMask(L_BK_VE_ENC_EX(0x3D),(u32YUV >> 8),0xFF); ///< U
    MDrv_WriteByteMask(H_BK_VE_ENC_EX(0x3D),(u32YUV >> 0),0xFF);///< V

    return E_VE_OK;
}

//------------------------------------------------------------------------------
/// VE Set VE output with OSD
/// @return none
//------------------------------------------------------------------------------
void Hal_VE_Set_OSD(MS_BOOL bEnable)
{
    MDrv_WriteByteMask(L_BK_VE_SRC(0x55),bEnable,BIT(0));
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
    MS_U16 u16PostVal = 0;
    MS_U16 u16PreVal = u8Val;

    u16PostVal = (u16PreVal * (BIT(6) -1)) / (BIT(8)-1);

    MDrv_WriteByteMask(H_BK_VE_SRC(0x5F),(MS_U8)u16PostVal,0x3F);

    return E_VE_OK;
}

//------------------------------------------------------------------------------
/// Get VE Video Alpha
/// @return none
//------------------------------------------------------------------------------
VE_Result Hal_VE_Get_VideoAlpha(MS_U8 *pu8Val)
{
    MS_U16 u16PostVal = 0;
    MS_U16 u16PreVal = (MS_U16)MDrv_Read2Byte(H_BK_VE_SRC(0x5F) & 0x3F00);

    u16PostVal = (u16PreVal * (BIT(8) -1)) / (BIT(6)-1);

    *pu8Val = (MS_U8)u16PostVal;

    return E_VE_OK;
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

void Hal_VE_DumpTable(MS_U8 *pVeTable, MS_U8 u8TableType)
{
    //do nothing on this chip
}

VE_Result Hal_VE_SetMV(MS_BOOL bEnable, MS_VE_MV_TYPE eMvType)
{
    //do nothing on this chip
    return E_VE_NOT_SUPPORT;
}

VE_Result Hal_VE_SetMV_BitControl(MS_BOOL bEnable, MS_VE_MV_BitControl MV_BitControl_Data)
{
    //do nothing on this chip
    return E_VE_NOT_SUPPORT;
}

VE_Result Hal_VE_DCS_SetType(MS_BOOL bEnable, MS_VE_DCS_TYPE eDCSType)
{
    //do nothing on this chip
    UNUSED(bEnable);
    UNUSED(eDCSType);
    return E_VE_NOT_SUPPORT;
}

VE_Result Hal_VE_DCS_SetActivationKey(MS_U8 *pu8ActivationKeyTbl, MS_U8 u8ActivationKeyTblSize)
{
    //do nothing on this chip
    UNUSED(pu8ActivationKeyTbl);
    UNUSED(u8ActivationKeyTblSize);
    return E_VE_NOT_SUPPORT;
}

//for Hal_VE_DisableRegWrite_GetCaps
#define DISABLE_REG_WRITE_CAP  FALSE;

MS_BOOL Hal_VE_DisableRegWrite_GetCaps(void)
{
    return DISABLE_REG_WRITE_CAP;
}

// show internal color bar
// TRUE: enable color bar
// FALSE: disable color bar
void Hal_VE_ShowColorBar(MS_BOOL bEnable)
{
    // not implemented
}

VE_Result Hal_VE_AdjustPositionBase(MS_S32 s32WAddrAdjustment, MS_S32 s32RAddrAdjustment)
{
    //do nothing on this chip
    return E_VE_NOT_SUPPORT;
}

VE_Result HAL_VE_SetFrameLock(MS_U32 u32IDclk, MS_U32 u32ODclk, MS_U32 u32InitPll, MS_BOOL bEnable)
{
    //do nothing on this chip
    return E_VE_NOT_SUPPORT;
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
    cap->bSupport_DolbyVerifier = FALSE;

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
    // not supported
    return MS_VE_DRAM_FORMAT_Y8C8;
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
    // not supported
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
    // not supported
    return 0;
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
    // not supported
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
    // not supported
    return 0;
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
    // not supported
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
    // not supported
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
    // not supported
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
    // not supported
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
    // not supported
    return E_VE_NOT_SUPPORT;
}

MS_BOOL Hal_VE_ISRConfig(MS_VE_INT_TYPE eIntType)
{
    switch(eIntType)
    {
        case MS_VE_INT_TVS_VSYNC:
            MDrv_WriteRegBit(L_BK_VE_SRC(0x46), BIT(2) , BIT(2));
            MDrv_WriteRegBit(L_BK_VE_SRC(0x7E), 0 , BIT(1));
            break;
        case MS_VE_INT_TVE_FIELD:
            MDrv_WriteRegBit(L_BK_VE_SRC(0x46), 0 , BIT(2));
            break;
        case MS_VE_INT_TVE_VSYNC_TVE_FIELD:
            MDrv_WriteRegBit(L_BK_VE_SRC(0x46), BIT(2) , BIT(2));
            MDrv_WriteRegBit(L_BK_VE_SRC(0x7E), BIT(1) , BIT(1));
            break;
        default:
            return FALSE;
    }
    return TRUE;
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
    // not supported
}
MS_BOOL Hal_VE_GetWSSStatus(void)
{
    // not supported
    return TRUE;
}
void Hal_VE_OnOffMV(MS_BOOL ben)
{
    // not supported
}
MS_BOOL Hal_VE_GetMVStatus(void)
{
    // not supported
    return TRUE;
}
void Hal_VE_OnOffDCS(MS_BOOL ben)
{
    // not supported
}
MS_BOOL Hal_VE_GetDCSStatus(void)
{
    // not supported
    return TRUE;
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
    // Not support
    return 0;
}

void Hal_VE_Set_PreHSD(MS_VE_INPUT_SRC_TYPE eInputSrcType,MS_U16 *u16In_Hsize,MS_U16 *u16In_Vsize)
{
    MS_BOOL bEnable = FALSE;

    // for Manhattan no di2VE , just IP2VE, need do go through 2P to 1P engine
    if(*u16In_Hsize> 1920) // bigger size do pre_HSD
    {
        bEnable = TRUE; // do average(1/2)
    }
    else
    {
        bEnable = FALSE;
    }

    // set polirity
    MApi_XC_W2BYTEMSK(_SC_PK_L_(0x0D,0x09), BIT(0), BIT(0));

    if(bEnable)
    {
        *u16In_Hsize /= 2;
        MApi_XC_W2BYTEMSK(_SC_PK_L_(0x0D,0x22), BIT(15)|BIT(8)|BIT(11), BIT(15)|BIT(8)|BIT(11));
        MApi_XC_W2BYTE(_SC_PK_L_(0x0D,0x20),*u16In_Hsize);
    }
    else // just do 2P to 1P
    {
        MApi_XC_W2BYTEMSK(_SC_PK_L_(0x0D,0x22), BIT(15), BIT(15)|BIT(8)|BIT(11));
        MApi_XC_W2BYTE(_SC_PK_L_(0x0D,0x20),*u16In_Hsize /2);
    }
}

void Hal_VE_set_crop_window(MS_U16 u16hstart, MS_U16 u16hend, MS_U16 u16vstart, MS_U16 u16vend)
{
    MDrv_Write2Byte(L_BK_VE_SRC(0x56), u16hstart);
    MDrv_Write2Byte(L_BK_VE_SRC(0x58), u16hend);
    MDrv_Write2Byte(L_BK_VE_SRC(0x57), u16vstart);
    MDrv_Write2Byte(L_BK_VE_SRC(0x59), u16vend);
    /*1 if HSU enable :
     (horizontal crop end) - (horizontal crop start) + 1 should be equal hsu output size (0x103B65[9:0])

      2 if HSU disable :
     (horizontal crop end) - (horizontal crop start) + 1 should be equal hsu input size (0x103B64[9:0])
    */
    // no us HSU
    MDrv_Write2Byte(L_BK_VE_SRC(0x64), u16hend-u16hstart+1);
}

VE_Result Hal_VE_ReloadSetting(void)
{
    // not supported
    return E_VE_NOT_SUPPORT;
}

#endif

