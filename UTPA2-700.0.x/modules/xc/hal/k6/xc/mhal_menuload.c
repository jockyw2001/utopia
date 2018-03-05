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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
//==============================================================================
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "xc_hwreg_utility2.h"
#include "xc_Analog_Reg.h"
#include "drvXC_IOPort.h"
#include "xc_hwreg_utility2.h"
#include "hwreg_sc.h"

#include "apiXC.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_Adc.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "apiXC_Auto.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#endif
#include "XC_private.h"
// Registers
#include "mhal_menuload.h"

#include "mdrv_sc_dynamicscaling.h"

MS_BOOL Hal_XC_MLoad_GetCaps(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(psXCInstPri->u32DeviceID == E_XC_DEVICE0)
    {
        return TRUE;
    }
    else // SC1 not supporting Menuload
    {
        return FALSE;
    }
}

MS_U16 Hal_XC_MLoad_get_status(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    return ((SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK1F_02_L) & 0x8000) >>15);
}

void Hal_XC_MLoad_set_on_off(void *pInstance, MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(bEn)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_02_L, 0x8000, 0x8000);
    else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_02_L, 0x0000, 0x8000);
}

void Hal_XC_MLoad_set_len(void *pInstance, MS_U16 u16Len)//req len
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    u16Len &= 0x0F;
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_02_L, u16Len, 0x7FF);
}

void Hal_XC_MLoad_set_depth(void *pInstance, MS_U16 u16depth)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK1F_01_L, u16depth);
}

void Hal_XC_MLoad_set_miusel(void *pInstance, MS_U8 u8MIUSel)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if (u8MIUSel == 0)
    {
       SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_1E_L, 0x0000, 0x0003);
    }
    else if (u8MIUSel == 1)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_1E_L, 0x0001, 0x0003);
    }
}

MS_U8 Hal_XC_MLoad_get_miusel(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    return ((SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK1F_10_L) & 0x2000)>>13);
}

void Hal_XC_MLoad_set_base_addr(void *pInstance, MS_PHY u32addr)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    u32addr /= MS_MLOAD_MEM_BASE_UNIT;

    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK1F_03_L, (MS_U16)(u32addr & 0xFFFF));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_04_L, (MS_U16)((u32addr & 0x3FF0000)>>16), 0x03FF);
}

void Hal_XC_MLoad_Set_riu(void *pInstance, MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if (bEn)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_10_L, 0x1000, 0x1000);
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_10_L, 0x0000, 0x1000);
    }
}

void Hal_XC_MLoad_set_trigger_timing(void *pInstance, MS_U16 u16sel)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    u16sel = (u16sel & 0x0003)<<12;
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_19_L, u16sel, 0x3000);
}

void Hal_XC_MLoad_set_opm_lock(void *pInstance, MS_U16 u16sel)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    u16sel = (u16sel & 0x0003)<<8;
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_19_L, u16sel, 0x0300);
}

void Hal_XC_MLoad_set_trigger_delay(void *pInstance, MS_U16 u16delay)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_22_L, u16delay, 0x0FFF);
}

//___|T_________________........__|T____ VSync
//__________|T__________________         ATP(refer the size befor memory to cal the pip sub and main length)
//_________________|T___________         Disp

//Generate TRAIN_TRIG_P from delayed line of Vsync(Setting the delay line for Auto tune area)
//Generate DISP_TRIG_P from delayed line of Vsync(Setting the delay line for Display area)
void Hal_XC_MLoad_set_trig_p(void *pInstance, MS_U16 u16train, MS_U16 u16disp)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1A_L, u16train, 0x0FFF);
    //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1B_L, u16disp,  0x0FFF);
}

//Get the delay line for Auto tune area
//Get the delay line for Display area
MS_BOOL Hal_XC_MLoad_get_trig_p(void *pInstance, MS_U16 *pu16Train, MS_U16 *pu16Disp)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    *pu16Train = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1A_L, 0x0FFF);
    *pu16Disp = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK20_1B_L, 0x0FFF);
    return TRUE;
}

void Hal_XC_MLoad_set_riu_cs(void *pInstance, MS_BOOL bEn)
{
    if(bEn)
    {
        MDrv_WriteByteMask(0x100104, 0x10, 0x10);
    }
    else
    {
        MDrv_WriteByteMask(0x100104, 0x00, 0x10);
    }
}

MS_BOOL Hal_XC_MLG_GetCaps(void *pInstance)
{
    return FALSE;
}

MS_U16 Hal_XC_MLG_get_status(void *pInstance)
{
    return TRUE;
}

void Hal_XC_MLoad_Enable_64BITS_COMMAND(void *pInstance,MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_U16 u16sel=0;
    if(bEn)
    {//enable 64bits command : bk1f_70[0]=1
        u16sel=1;
    }
    else
    {//disable 64bits command : bk1f_70[0]=0
        u16sel=0;
    }
    u16sel = u16sel & 0x0001;
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK1F_70_L, u16sel, 0x0001);
}

void Hal_XC_MLoad_Enable_64BITS_SPREAD_MODE(void *pInstance,MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_U16 u16sel=0;
    if(bEn)
    {//enable 64BITS_SPREAD_MODE : bk1f_70[15]=1
        u16sel=1;
    }
    else
    {//disable 64BITS_SPREAD_MODE : bk1f_70[15]=1
        u16sel=0;
    }
    u16sel = (u16sel & 0x0001)<<15;
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK1F_70_L, u16sel, 0x8000);
}

void Hal_XC_MLoad_Set_64Bits_MIU_Bus_Sel(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_U16 u16sel = 0x00;

    if( MS_MLOAD_MEM_BASE_UNIT == 8 )
    {
        u16sel = 0x00;
    }
    else if( MS_MLOAD_MEM_BASE_UNIT == 16 )
    {
        u16sel = 0x01;
    }
    else if( MS_MLOAD_MEM_BASE_UNIT == 32 )
    {
        u16sel = 0x11;
    }
    else
    {
        printf("MIU Bus not support !!!!!!!!!!!!!!!!!\n");
        u16sel = 0x00;
    }

    u16sel = (u16sel & 0x0003)<<14;
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK1F_13_L, u16sel, 0xC000);
}

void Hal_XC_MLoad_Command_Format_initial(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if ENABLE_64BITS_COMMAND
    Hal_XC_MLoad_Enable_64BITS_COMMAND(pInstance,TRUE);
    if(ENABLE_64BITS_SPREAD_MODE)
    {
        Hal_XC_MLoad_Enable_64BITS_SPREAD_MODE(pInstance,TRUE);
    }
    //select MIU Bus : 00: 64bit, 01:128bit, 11:256bit
    Hal_XC_MLoad_Set_64Bits_MIU_Bus_Sel(pInstance);
#else
    Hal_XC_MLoad_set_miu_bus_sel(pInstance, MS_MLOAD_MIU_BUS_SEL);
#endif
}

MS_U64 Hal_XC_MLoad_Gen_64bits_spreadMode(void *pInstance,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_U64 u64CmdTemp = 0;
    MS_U8  u8AddrTemp = 0;
    MS_U16 u16BankTemp = 0;
    MS_U16 u16DataTemp = 0;
    MS_U16 u16MaskTemp = 0;

    u16MaskTemp = 0xFFFF;
    u16DataTemp = (SC_R2BYTE(psXCInstPri->u32DeviceID,u32Addr) & ~u16Mask) | (u16Data & u16Mask);

    u8AddrTemp= (u32Addr & 0xFF) >> 1;
    u16BankTemp= 0x1300 | ((u32Addr >> 8) & 0xFF);

    u64CmdTemp|= (MS_U64)u16DataTemp;
    u64CmdTemp|= ((MS_U64)u8AddrTemp<<16);
    u64CmdTemp|= ((MS_U64)u16BankTemp<<23);
    u64CmdTemp|= ((MS_U64)u16MaskTemp<<48);
    return u64CmdTemp;
}

MS_BOOL Hal_XC_MLoad_parsing_64bits_spreadMode_NonXC(void *pInstance,MS_U64 u64Cmd, MS_U32 *u32Addr, MS_U16 *u16Data)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_U8  u8AddrTemp = 0;
    MS_U16 u16BankTemp = 0;

    *u16Data = (MS_U16)(0xFFFF&(MS_U16)u64Cmd);
    u8AddrTemp= (MS_U8)((u64Cmd>>16 & 0x7F) << 1);
    u16BankTemp= (MS_U16)((u64Cmd >> 23) & 0xFFFF);
    *u32Addr = (MS_U32)(u8AddrTemp|u16BankTemp<<8);

    return TRUE;
}

MS_U64 Hal_XC_MLoad_Gen_64bits_spreadMode_NonXC(void *pInstance,MS_U32 u32Bank,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_U64 u64CmdTemp = 0;

    u64CmdTemp|= (MS_U64)u16Data;
    u64CmdTemp|= ((MS_U64) ((u32Addr<<16) >>1));
    u64CmdTemp|= ((MS_U64)u32Bank<<23);
    u64CmdTemp|= ((MS_U64)u16Mask<<48);

    return u64CmdTemp;
}

MS_U64 Hal_XC_MLoad_Gen_64bits_subBankMode(void *pInstance,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_MLoad_Data_64Bits_SubBank data_SBank_Mode;
    data_SBank_Mode.u32NoUse = 0x0;
    data_SBank_Mode.u8Addr = (u32Addr & 0xFF) >> 1;
    data_SBank_Mode.u8Bank = (u32Addr >> 8) & 0xFF;

    if( u16Mask == 0xFFFF )
    {
        data_SBank_Mode.u16Data = u16Data;
    }
    else
    {
        data_SBank_Mode.u16Data = (SC_R2BYTE(psXCInstPri->u32DeviceID,u32Addr) & ~u16Mask) | (u16Data & u16Mask);
    }
    return data_SBank_Mode.u64Cmd;
}

MS_BOOL Hal_XC_MLoad_parsing_32bits_subBankMode(void *pInstance, MS_U32 u32MloadData, MS_U32 *pu32Addr, MS_U16 *pu16Data)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_MLoad_Data data;
    data.u32Cmd = u32MloadData;
    *pu32Addr = (((MS_U32)(data.u8Addr))<<1) + ((((MS_U32)(data.u8Bank)) - _XC_Device_Offset[psXCInstPri->u32DeviceID])<<8);
    *pu16Data = data.u16Data;

    return TRUE;
}

MS_U32 Hal_XC_MLoad_Gen_32bits_subBankMode(void *pInstance,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_MLoad_Data data;
    data.u8Addr = (u32Addr & 0xFF) >> 1;
    data.u8Bank = ((u32Addr >> 8) & 0xFF) + _XC_Device_Offset[psXCInstPri->u32DeviceID];

    if( u16Mask == 0xFFFF )
    {
        data.u16Data = u16Data;
    }
    else
    {
        data.u16Data = (SC_R2BYTE(psXCInstPri->u32DeviceID, u32Addr) & ~u16Mask) | (u16Data & u16Mask);
    }
    return data.u32Cmd;
}

MS_U16 Hal_XC_MLoad_Get_Depth(void *pInstance, MS_U16 u16CmdCnt)
{
    MS_U16 result = u16CmdCnt;
    MS_U16 u16CmdLength = 0;
    MS_U16 u16CmdNum = 0;
    if(ENABLE_64BITS_COMMAND)
    {
        u16CmdLength = 8;//64 bits command = 8 bytes
        u16CmdNum = (MS_MLOAD_MEM_BASE_UNIT / u16CmdLength)/2; //256 bit only 128 bit have cmds, so cmds num is 2
        if((u16CmdCnt%u16CmdNum)!=0)
        {
           printf("KickOff: Commands are not full!!\n");
        }
        result = u16CmdCnt/u16CmdNum;
    }
    return result;
}

void Hal_XC_MLoad_set_trigger_sync(void *pInstance, MLoad_Trigger_Sync eTriggerSync)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    switch(eTriggerSync)
    {
        case MLOAD_TRIGGER_BY_IP_MAIN_SYNC:
        {//trigger by IP_Main Vsync
           SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_1D_L, 0x2000, 0x7000);
        }
        break;
        case MLOAD_TRIGGER_BY_IP_SUB_SYNC:
        {//trigger by IP_Sub Vsync
           SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_1D_L, 0x3000, 0x7000);
        }
        break;
        default:
        {
            //default: trigger by OP Vsync
            #if 1
            if( SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_1D_L, 0x7000)!=0x0000)
            {
                //HW patch: menuload (triggered by ip vsync) can not stop
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_1D_L, 0x1000, 0x7000);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_02_L, 0x0000, 0x8000);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_02_L, 0x8000, 0x8000);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_02_L, 0x0000, 0x8000);
            }
            #endif

            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_1D_L, 0x0000, 0x7000);
        }
        break;
    }
}

void Hal_SC_ControlMloadTrig(void *pInstance)
{
    UNUSED(pInstance);
}

void Hal_XC_MLoad_set_BitMask(void *pInstance,MS_BOOL enable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(enable)
    {
        //disable bit mask inverse (means: mask 0xFE can write bit 1111 1110)
        //(otherwise mask 0xFE will write bit 0000 0001)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_07_L, 0x0000, 0x2000);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_07_L, 0x8000, 0x8000);
    }else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_07_L, 0x0000, 0x8000);
    }
}

