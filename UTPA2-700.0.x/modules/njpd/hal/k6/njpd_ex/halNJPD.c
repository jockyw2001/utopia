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
// Copyright (c) 2008-2011 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    halNJPD.c
/// @brief  NJPD hal interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "halCHIP.h"
// Internal Definition
//#include "regCHIP.h"
#include "drvMMIO.h" //get RIU base
#include "osalNJPD.h"
#include "drvNJPD.h"
#include "regNJPD.h"
#include "halNJPD.h"
#ifdef CONFIG_MSTAR_CLKM
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/clkm.h>
#else
#include "drvCLKM.h"
#endif
#endif


#include "drvSYS.h"

// MJPEG
#define USE_FW_HVD      1

#if USE_FW_HVD
//#include "../hvd_ex/fwHVD_if.h"
#define NJPD_FW_VERSION  "NJPD.FW.00130062"
#else
#include "fwNJPD.h"
#endif

// For MVD Power On
void HAL_MVD_RegSetBase(MS_U32 u32Base);
void HAL_MVD_PowerCtrl(MS_BOOL bOn);
void HAL_MVD_SetSyncClk(MS_BOOL bEnable);
MS_BOOL HAL_MVD_RstHW(void);

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define HAL_NJPD_ENABLE      1
#define HAL_NJPD_DISABLE     0

#define HAL_NJPD_MUTEX_SUPPORT   HAL_NJPD_ENABLE
#define HAL_NJPD_MIU_PROTECT     HAL_NJPD_ENABLE
#define MEASURE_NJPD_MIU_CLIENT_EFFICIENCY       FALSE//TRUE

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define __HAL_NJPD_DelayMs(x)      do { MS_U32 ticks=0; while((ticks++)>(x*10)); } while(0)

#define NJPD_RIU_MAP u32NJPDRiuBaseAdd  //obtain in init

#ifndef ANDROID
#define NJPD_DEBUG_HAL_MSG(format, args...)  do{if(_u8NJPDHalDbgLevel & E_NJPD_DEBUG_HAL_MSG) printf(format, ##args);}while(0)
#define NJPD_DEBUG_HAL_ERR(format, args...)  do{if(_u8NJPDHalDbgLevel & E_NJPD_DEBUG_HAL_ERR) printf(format, ##args);}while(0)
#else
#define LOG_TAG "HAL_JPD"
#include <cutils/log.h>
#ifndef LOGD
#define NJPD_DEBUG_HAL_MSG(format, args...)  do{if(_u8NJPDHalDbgLevel & E_NJPD_DEBUG_HAL_MSG) ALOGI(format, ##args);}while(0)
#define NJPD_DEBUG_HAL_ERR(format, args...)  do{if(_u8NJPDHalDbgLevel & E_NJPD_DEBUG_HAL_ERR) ALOGE(format, ##args);}while(0)
#else
#define NJPD_DEBUG_HAL_MSG(format, args...)  do{if(_u8NJPDHalDbgLevel & E_NJPD_DEBUG_HAL_MSG) LOGI(format, ##args);}while(0)
#define NJPD_DEBUG_HAL_ERR(format, args...)  do{if(_u8NJPDHalDbgLevel & E_NJPD_DEBUG_HAL_ERR) LOGE(format, ##args);}while(0)
#endif

#endif

#ifndef READ_BYTE
#define READ_BYTE(_reg)             (*(volatile MS_U8*)(_reg))
#define READ_WORD(_reg)             (*(volatile MS_U16*)(_reg))
#define READ_LONG(_reg)             (*(volatile MS_U32*)(_reg))
#define WRITE_BYTE(_reg, _val)      { (*((volatile MS_U8*)(_reg))) = (MS_U8)(_val); }
#define WRITE_WORD(_reg, _val)      { (*((volatile MS_U16*)(_reg))) = (MS_U16)(_val); }
#define WRITE_LONG(_reg, _val)      { (*((volatile MS_U32*)(_reg))) = (MS_U32)(_val); }
#endif

#define NJPD_READ_BYTE(addr)         READ_BYTE( (NJPD_RIU_MAP + (addr)) )
#define NJPD_READ_WORD(addr)         READ_WORD( (NJPD_RIU_MAP + (addr)) )
#define NJPD_WRITE_BYTE(addr, val)   WRITE_BYTE( (NJPD_RIU_MAP + (addr)), (val) )
#define NJPD_WRITE_WORD(addr, val)   WRITE_WORD( (NJPD_RIU_MAP + (addr)), (val) )

#define NJPD_MACRO_START     do {
#define NJPD_MACRO_END       } while (0)

#define __HAL_NJPD_ReadByte( u32Reg )   NJPD_READ_BYTE(((u32Reg) << 1) - ((u32Reg) & 1))

#define __HAL_NJPD_Read2Byte( u32Reg )    (NJPD_READ_WORD((u32Reg)<<1))

#define __HAL_NJPD_Read4Byte( u32Reg )   ( (MS_U32)NJPD_READ_WORD((u32Reg)<<1) | ((MS_U32)NJPD_READ_WORD(((u32Reg)+2)<<1)<<16 ) )

#define __HAL_NJPD_ReadBit( u32Reg, u8Mask )   (NJPD_READ_BYTE(((u32Reg)<<1) - ((u32Reg) & 1)) & (u8Mask))

#define __HAL_NJPD_WriteBit( u32Reg, bEnable, u8Mask ) \
    NJPD_MACRO_START \
    NJPD_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) , (bEnable) ? (NJPD_READ_BYTE(  (((u32Reg) <<1) - ((u32Reg) & 1))  ) |  (u8Mask)) : \
                                (NJPD_READ_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) ) & ~(u8Mask))); \
    NJPD_MACRO_END

#define __HAL_NJPD_WriteByte( u32Reg, u8Val ) \
    NJPD_MACRO_START \
    NJPD_WRITE_BYTE(((u32Reg) << 1) - ((u32Reg) & 1), (u8Val)); \
    NJPD_MACRO_END

#define __HAL_NJPD_Write2Byte( u32Reg, u16Val ) \
    NJPD_MACRO_START \
    if ( ((u32Reg) & 0x01) ) \
    { \
        NJPD_WRITE_BYTE(((u32Reg) << 1) - 1, (MS_U8)((u16Val))); \
        NJPD_WRITE_BYTE(((u32Reg) + 1) << 1, (MS_U8)((u16Val) >> 8)); \
    } \
    else \
    { \
        NJPD_WRITE_WORD( ((u32Reg)<<1) ,  u16Val); \
    } \
    NJPD_MACRO_END

#define __HAL_NJPD_Write4Byte( u32Reg, u32Val ) \
    NJPD_MACRO_START \
    if ((u32Reg) & 0x01) \
    { \
        NJPD_WRITE_BYTE( ((u32Reg) << 1) - 1 ,  (u32Val)); \
        NJPD_WRITE_WORD( ((u32Reg) + 1)<<1 , ( (u32Val) >> 8)); \
        NJPD_WRITE_BYTE( (((u32Reg) + 3) << 1) ,  ((u32Val) >> 24)); \
    } \
    else \
    { \
        NJPD_WRITE_WORD( (u32Reg)<<1 , (u32Val)); \
        NJPD_WRITE_WORD(  ((u32Reg) + 2)<<1 ,  ((u32Val) >> 16)); \
    } \
    NJPD_MACRO_END

//NJPD mutex
#if (HAL_NJPD_MUTEX_SUPPORT == HAL_NJPD_ENABLE)
static MS_S32 _s32NJPDMutexID = -1;
MS_U8 strNJPD[10] = "NJPD_Mutex";

#define __HAL_NJPD_MutexCreate() \
    NJPD_MACRO_START \
    if( _s32NJPDMutexID < 0 ) \
    { \
        _s32NJPDMutexID = OSAL_NJPD_MutexCreate( strNJPD ); \
    } \
    NJPD_MACRO_END
#define __HAL_NJPD_MutexDelete() \
    NJPD_MACRO_START \
    if( _s32NJPDMutexID >= 0 ) \
    { \
        OSAL_NJPD_MutexDelete(_s32NJPDMutexID); \
        _s32NJPDMutexID = -1; \
    } \
    NJPD_MACRO_END
#define  __HAL_NJPD_MutexEntry() \
    NJPD_MACRO_START \
    if( _s32NJPDMutexID >= 0 ) \
    { \
        if (!OSAL_NJPD_MutexObtain(_s32NJPDMutexID, OSAL_NJPD_MUTEX_TIMEOUT)) \
        { \
            NJPD_DEBUG_HAL_MSG("[HAL NJPD][%s][%06d] Mutex taking timeout\n", __FUNCTION__, __LINE__); \
        } \
    } \
    NJPD_MACRO_END
#define __HAL_NJPD_MutexExit() \
    NJPD_MACRO_START \
    if( _s32NJPDMutexID >= 0 ) \
    { \
        OSAL_NJPD_MutexRelease(_s32NJPDMutexID); \
    } \
    NJPD_MACRO_END
#else //HAL_NJPD_MUTEX_SUPPORT
#define __HAL_NJPD_MutexCreate()
#define __HAL_NJPD_MutexDelete()
#define __HAL_NJPD_MutexEntry()
#define __HAL_NJPD_MutexExit()
#endif //HAL_NJPD_MUTEX_SUPPORT

#define MAX_MJPEG_DEC_NUM 2

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_VIRT u32NJPDRiuBaseAdd = 0x0;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_U8 _pu8HalVer[] = NJPD_HAL_VERSION;
static MS_U8 _pu8FwVer[] = NJPD_FW_VERSION;
static MS_U8 _u8NJPDHalDbgLevel = E_NJPD_DEBUG_HAL_NONE;


static JPD_Number eNJPDNum = E_NJPD_NJPD1;
static NJPD_VerificationMode eVerificationMode = E_NJPD00_NONE;

#if MEASURE_NJPD_MIU_CLIENT_EFFICIENCY
static MS_U8 bJPEGFirstTime=0;
#endif
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _HAL_NJPD_SetMIUProtectMask(MS_BOOL bEnable)
{
#if (HAL_NJPD_MIU_PROTECT == HAL_NJPD_ENABLE)
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_WriteBit(NJPD_MIU0_RQ2_MASK, bEnable, NJPD0_MIU0_CLIENT_NJPD);
        __HAL_NJPD_WriteBit(NJPD_MIU1_RQ2_MASK, bEnable, NJPD0_MIU1_CLIENT_NJPD);

        NJPD_DEBUG_HAL_MSG("%s MIU0 : 0x%04x\n", __FUNCTION__, __HAL_NJPD_Read2Byte(NJPD_MIU0_RQ2_MASK));
        NJPD_DEBUG_HAL_MSG("%s MIU1 : 0x%04x\n", __FUNCTION__, __HAL_NJPD_Read2Byte(NJPD_MIU1_RQ2_MASK));
    }
    else
    {
        __HAL_NJPD_WriteBit(NJPD_MIU0_RQ2_MASK, bEnable, NJPD0_MIU0_CLIENT_NJPD);
        __HAL_NJPD_WriteBit(NJPD_MIU1_RQ2_MASK, bEnable, NJPD0_MIU1_CLIENT_NJPD);

        NJPD_DEBUG_HAL_MSG("%s MIU0 : 0x%04x\n", __FUNCTION__, __HAL_NJPD_Read2Byte(NJPD_MIU0_RQ2_MASK));
        NJPD_DEBUG_HAL_MSG("%s MIU1 : 0x%04x\n", __FUNCTION__, __HAL_NJPD_Read2Byte(NJPD_MIU1_RQ2_MASK));
    }
#endif
    return;
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_NJPD_SetMIU(MS_U8 u8Idx)
{
#if 1
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        NJPD_DEBUG_HAL_MSG("NJPD0, ");
        __HAL_NJPD_WriteBit(NJPD0_MIU1_MIU_SEL2, 1, NJPD0_MIU1_CLIENT_NJPD);

        MS_U16 u16Reg = (MS_U16)u8Idx;
        u16Reg = u16Reg << NJPD_MIU_SEL_SHIFT;
        u16Reg = u16Reg | (__HAL_NJPD_Read2Byte(BK_NJPD1_IBUF_READ_LENGTH) & ~(NJPD_MIU_SEL|NJPD_MIU_SEL_1));
        __HAL_NJPD_Write2Byte(BK_NJPD1_IBUF_READ_LENGTH, u16Reg);
    }
    else
    {
        // Note: there is no NJPD1 in k6
        NJPD_DEBUG_HAL_MSG("NJPD1, ");
        __HAL_NJPD_WriteBit(NJPD0_MIU1_MIU_SEL2, 1, NJPD0_MIU1_CLIENT_NJPD);

        MS_U16 u16Reg = (MS_U16)u8Idx;
        u16Reg = u16Reg << NJPD_MIU_SEL_SHIFT;
        u16Reg = u16Reg | (__HAL_NJPD_Read2Byte(BK_NJPD2_IBUF_READ_LENGTH) & ~(NJPD_MIU_SEL|NJPD_MIU_SEL_1));
        __HAL_NJPD_Write2Byte(BK_NJPD2_IBUF_READ_LENGTH, u16Reg);
    }

#else
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        NJPD_DEBUG_HAL_MSG("NJPD0, ");
        __HAL_NJPD_WriteBit(NJPD0_MIU1_MIU_SEL2, 0, NJPD0_MIU1_CLIENT_NJPD);
        if(u8Idx)
        {
            NJPD_DEBUG_HAL_MSG("MIU 1\n");
            __HAL_NJPD_WriteBit(NJPD0_MIU0_MIU_SEL2, 1, NJPD0_MIU0_CLIENT_NJPD);
        }
        else
        {
            NJPD_DEBUG_HAL_MSG("MIU 0\n");
            __HAL_NJPD_WriteBit(NJPD0_MIU0_MIU_SEL2, 0, NJPD0_MIU0_CLIENT_NJPD);
        }
    }
    else
    {
        NJPD_DEBUG_HAL_MSG("NJPD1, ");
        __HAL_NJPD_WriteBit(NJPD0_MIU1_MIU_SEL2, 0, NJPD0_MIU1_CLIENT_NJPD);
        if(u8Idx)
        {
            NJPD_DEBUG_HAL_MSG("MIU 1\n");
            __HAL_NJPD_WriteBit(NJPD0_MIU0_MIU_SEL2, 1, NJPD0_MIU0_CLIENT_NJPD);
        }
        else
        {
            NJPD_DEBUG_HAL_MSG("MIU 0\n");
            __HAL_NJPD_WriteBit(NJPD0_MIU0_MIU_SEL2, 0, NJPD0_MIU0_CLIENT_NJPD);
        }
    }
#endif
}

void HAL_NJPD_JPDSelect(MS_BOOL bNJPD)
{
    if(bNJPD)
    {
        __HAL_NJPD_WriteBit(NJPD_MIU_GROUP2_I64, 0, NJPD_MIU0_CLIENT_NJPD_CS2);

        if(eNJPDNum == E_NJPD_NJPD1)
        {
            __HAL_NJPD_WriteBit(BK_NJPD1_TOP_MARB_PORT_ENABLE, 1, NJPD_TOP_MARB_P0_ENABLE);
            __HAL_NJPD_WriteBit(BK_NJPD1_TOP_MARB_PORT_ENABLE, 1, NJPD_TOP_MARB_P1_ENABLE);
            __HAL_NJPD_WriteBit(BK_NJPD1_TOP_MARB_PORT_ENABLE, 0, NJPD_TOP_MARB_P2_ENABLE);
        }
        else
        {
            __HAL_NJPD_WriteBit(BK_NJPD2_TOP_MARB_PORT_ENABLE, 1, NJPD_TOP_MARB_P0_ENABLE);
            __HAL_NJPD_WriteBit(BK_NJPD2_TOP_MARB_PORT_ENABLE, 1, NJPD_TOP_MARB_P1_ENABLE);
            __HAL_NJPD_WriteBit(BK_NJPD2_TOP_MARB_PORT_ENABLE, 0, NJPD_TOP_MARB_P2_ENABLE);
        }
    }
    else
    {
        __HAL_NJPD_WriteBit(NJPD_MIU_GROUP2_I64, 1, NJPD_MIU0_CLIENT_NJPD_CS2);

        if(eNJPDNum == E_NJPD_NJPD1)
        {
            __HAL_NJPD_WriteBit(BK_NJPD1_TOP_MARB_PORT_ENABLE, 0, NJPD_TOP_MARB_P0_ENABLE);
            __HAL_NJPD_WriteBit(BK_NJPD1_TOP_MARB_PORT_ENABLE, 0, NJPD_TOP_MARB_P1_ENABLE);
            __HAL_NJPD_WriteBit(BK_NJPD1_TOP_MARB_PORT_ENABLE, 1, NJPD_TOP_MARB_P2_ENABLE);
        }
        else
        {
            __HAL_NJPD_WriteBit(BK_NJPD2_TOP_MARB_PORT_ENABLE, 0, NJPD_TOP_MARB_P0_ENABLE);
            __HAL_NJPD_WriteBit(BK_NJPD2_TOP_MARB_PORT_ENABLE, 0, NJPD_TOP_MARB_P1_ENABLE);
            __HAL_NJPD_WriteBit(BK_NJPD2_TOP_MARB_PORT_ENABLE, 1, NJPD_TOP_MARB_P2_ENABLE);
        }
    }
#if 1
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_WriteBit(BK_NJPD1_TOP_MARB_PORT_ENABLE, 1, NJPD_TOP_MARB_P0_W_BYPASS_ENABLE);
        __HAL_NJPD_WriteBit(BK_NJPD1_TOP_MARB_PORT_ENABLE, 1, NJPD_TOP_MARB_P1_W_BYPASS_ENABLE);
        __HAL_NJPD_WriteBit(BK_NJPD1_TOP_MARB_PORT_ENABLE, 1, NJPD_TOP_MARB_P2_W_BYPASS_ENABLE);
        __HAL_NJPD_WriteBit(BK_NJPD1_TOP_MARB_PORT_ENABLE, 1, NJPD_TOP_MARB_P0_R_BYPASS_ENABLE);
        __HAL_NJPD_WriteBit(BK_NJPD1_TOP_MARB_PORT_ENABLE+1, 1, NJPD_TOP_MARB_P1_R_BYPASS_ENABLE);
        __HAL_NJPD_WriteBit(BK_NJPD1_TOP_MARB_PORT_ENABLE+1, 1, NJPD_TOP_MARB_P2_R_BYPASS_ENABLE);
    }
    else
    {
        __HAL_NJPD_WriteBit(BK_NJPD2_TOP_MARB_PORT_ENABLE, 1, NJPD_TOP_MARB_P0_W_BYPASS_ENABLE);
        __HAL_NJPD_WriteBit(BK_NJPD2_TOP_MARB_PORT_ENABLE, 1, NJPD_TOP_MARB_P1_W_BYPASS_ENABLE);
        __HAL_NJPD_WriteBit(BK_NJPD2_TOP_MARB_PORT_ENABLE, 1, NJPD_TOP_MARB_P2_W_BYPASS_ENABLE);
        __HAL_NJPD_WriteBit(BK_NJPD2_TOP_MARB_PORT_ENABLE, 1, NJPD_TOP_MARB_P0_R_BYPASS_ENABLE);
        __HAL_NJPD_WriteBit(BK_NJPD2_TOP_MARB_PORT_ENABLE+1, 1, NJPD_TOP_MARB_P1_R_BYPASS_ENABLE);
        __HAL_NJPD_WriteBit(BK_NJPD2_TOP_MARB_PORT_ENABLE+1, 1, NJPD_TOP_MARB_P2_R_BYPASS_ENABLE);
    }
#endif
}

void HAL_NJPD_Set_GlobalSetting00(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_GLOBAL_SETTING00, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_Get_GlobalSetting00 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_GLOBAL_SETTING00));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_GLOBAL_SETTING00, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_Get_GlobalSetting00 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_GLOBAL_SETTING00));
    }
}

MS_U16 HAL_NJPD_Get_GlobalSetting00(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_GLOBAL_SETTING00);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_GLOBAL_SETTING00);
    }
}

void HAL_NJPD_Set_GlobalSetting01(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_GLOBAL_SETTING01, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_Get_GlobalSetting01 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_GLOBAL_SETTING01));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_GLOBAL_SETTING01, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_Get_GlobalSetting01 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_GLOBAL_SETTING01));
    }
}

MS_U16 HAL_NJPD_Get_GlobalSetting01(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_GLOBAL_SETTING01);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_GLOBAL_SETTING01);
    }
}

void HAL_NJPD_Set_GlobalSetting02(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_GLOBAL_SETTING02, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_Get_GlobalSetting02 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_GLOBAL_SETTING02));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_GLOBAL_SETTING02, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_Get_GlobalSetting02 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_GLOBAL_SETTING02));
    }
}

MS_U16 HAL_NJPD_Get_GlobalSetting02(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_GLOBAL_SETTING02);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_GLOBAL_SETTING02);
    }
}


void HAL_NJPD_SetMRCBuf0_StartLow(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MIU_READ_BUFFER0_START_ADDR_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetMRCBuf0_StartLow : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_READ_BUFFER0_START_ADDR_L));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MIU_READ_BUFFER0_START_ADDR_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetMRCBuf0_StartLow : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_READ_BUFFER0_START_ADDR_L));
    }
}

void HAL_NJPD_SetMRCBuf0_StartHigh(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MIU_READ_BUFFER0_START_ADDR_H, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetMRCBuf0_StartHigh : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_READ_BUFFER0_START_ADDR_H));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MIU_READ_BUFFER0_START_ADDR_H, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetMRCBuf0_StartHigh : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_READ_BUFFER0_START_ADDR_H));
    }
}

void HAL_NJPD_SetMRCBuf0_EndLow(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MIU_READ_BUFFER0_END_ADDR_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetMRCBuf0_EndLow : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_READ_BUFFER0_END_ADDR_L));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MIU_READ_BUFFER0_END_ADDR_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetMRCBuf0_EndLow : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_READ_BUFFER0_END_ADDR_L));
    }
}

void HAL_NJPD_SetMRCBuf0_EndHigh(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MIU_READ_BUFFER0_END_ADDR_H, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetMRCBuf0_EndHigh : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_READ_BUFFER0_END_ADDR_H));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MIU_READ_BUFFER0_END_ADDR_H, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetMRCBuf0_EndHigh : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_READ_BUFFER0_END_ADDR_H));
    }
}

void HAL_NJPD_SetMRCBuf1_StartLow(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MIU_READ_BUFFER1_START_ADDR_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetMRCBuf1_StartLow : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_READ_BUFFER1_START_ADDR_L));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MIU_READ_BUFFER1_START_ADDR_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetMRCBuf1_StartLow : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_READ_BUFFER1_START_ADDR_L));
    }
}

void HAL_NJPD_SetMRCBuf1_StartHigh(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MIU_READ_BUFFER1_START_ADDR_H, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetMRCBuf1_StartHigh : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_READ_BUFFER1_START_ADDR_H));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MIU_READ_BUFFER1_START_ADDR_H, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetMRCBuf1_StartHigh : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_READ_BUFFER1_START_ADDR_H));
    }
}

void HAL_NJPD_SetMRCBuf1_EndLow(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MIU_READ_BUFFER1_END_ADDR_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetMRCBuf1_EndLow : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_READ_BUFFER1_END_ADDR_L));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MIU_READ_BUFFER1_END_ADDR_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetMRCBuf1_EndLow : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_READ_BUFFER1_END_ADDR_L));
    }
}

void HAL_NJPD_SetMRCBuf1_EndHigh(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MIU_READ_BUFFER1_END_ADDR_H, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetMRCBuf1_EndHigh : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_READ_BUFFER1_END_ADDR_H));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MIU_READ_BUFFER1_END_ADDR_H, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetMRCBuf1_EndHigh : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_READ_BUFFER1_END_ADDR_H));
    }
}


void HAL_NJPD_SetMRCStart_Low(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MIU_READ_START_ADDR_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetMRCStart_Low : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_READ_START_ADDR_L));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MIU_READ_START_ADDR_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetMRCStart_Low : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_READ_START_ADDR_L));
    }
}

void HAL_NJPD_SetMRCStart_High(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MIU_READ_START_ADDR_H, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetMRCStart_High : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_READ_START_ADDR_H));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MIU_READ_START_ADDR_H, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetMRCStart_High : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_READ_START_ADDR_H));
    }
}


void HAL_NJPD_SetMWCBuf_StartLow(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MIU_WRITE_START_ADDR_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetMWCBuf_StartLow : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_WRITE_START_ADDR_L));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MIU_WRITE_START_ADDR_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetMWCBuf_StartLow : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_WRITE_START_ADDR_L));
    }
}

void HAL_NJPD_SetMWCBuf_StartHigh(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MIU_WRITE_START_ADDR_H, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetMWCBuf_StartHigh : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_WRITE_START_ADDR_H));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MIU_WRITE_START_ADDR_H, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetMWCBuf_StartHigh : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_WRITE_START_ADDR_H));
    }
}

MS_U16 HAL_NJPD_GetMWCBuf_StartLow(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_WRITE_START_ADDR_L);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_WRITE_START_ADDR_L);
    }
}

MS_U16 HAL_NJPD_GetMWCBuf_StartHigh(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_WRITE_START_ADDR_H);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_WRITE_START_ADDR_H);
    }
}


MS_U16 HAL_NJPD_GetMWCBuf_WritePtrLow(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_WRITE_POINTER_ADDR_L);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_WRITE_POINTER_ADDR_L);
    }
}

MS_U16 HAL_NJPD_GetMWCBuf_WritePtrHigh(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_WRITE_POINTER_ADDR_H);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_WRITE_POINTER_ADDR_H);
    }
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_NJPD_SetPic_H(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_IMG_HSIZE, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetPic_H : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_IMG_HSIZE));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_IMG_HSIZE, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetPic_H : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_IMG_HSIZE));
    }
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_NJPD_SetPic_V(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_IMG_VSIZE, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetPic_V : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_IMG_VSIZE));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_IMG_VSIZE, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetPic_V : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_IMG_VSIZE));
    }
}

void HAL_NJPD_ClearEventFlag(MS_U16 u16Value)
{
    // Write 1 clear
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_IRQ_CLEAR, u16Value);
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_IRQ_CLEAR, u16Value);
    }
}
void HAL_NJPD_ForceEventFlag(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_IRQ_FORCE, u16Value);
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_IRQ_FORCE, u16Value);
    }
}
void HAL_NJPD_MaskEventFlag(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_IRQ_MASK, u16Value);
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_IRQ_MASK, u16Value);
    }
}
MS_U16 HAL_NJPD_GetEventFlag(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
#if MEASURE_NJPD_MIU_CLIENT_EFFICIENCY
        MsOS_DelayTask(1);
        printf("bank: 0x1012, offset: 0x0e, value=0x%x(%d), efficiency=%d%%\n", __HAL_NJPD_Read2Byte(BK_MIU0_GENERAL(0x0e)),
         __HAL_NJPD_Read2Byte(BK_MIU0_GENERAL(0x0e)), __HAL_NJPD_Read2Byte(BK_MIU0_GENERAL(0x0e))*100/1024);
#endif
        return __HAL_NJPD_Read2Byte(BK_NJPD1_IRQ_FINAL_S);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_IRQ_FINAL_S);
    }
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_NJPD_SetROI_H(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_ROI_H_START, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetROI_H : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_ROI_H_START));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_ROI_H_START, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetROI_H : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_ROI_H_START));
    }
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_NJPD_SetROI_V(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_ROI_V_START, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetROI_V : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_ROI_V_START));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_ROI_V_START, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetROI_V : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_ROI_V_START));
    }
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_NJPD_SetROIWidth(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_ROI_H_SIZE, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetROIWidth : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_ROI_H_SIZE));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_ROI_H_SIZE, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetROIWidth : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_ROI_H_SIZE));
    }
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_NJPD_SetROIHeight(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_ROI_V_SIZE, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetROIHeight : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_ROI_V_SIZE));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_ROI_V_SIZE, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetROIHeight : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_ROI_V_SIZE));
    }
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_NJPD_SetClock(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    __HAL_NJPD_Write2Byte(NJPD_CLOCK, u16Value);
    NJPD_DEBUG_HAL_MSG("HAL_NJPD_GetClock : 0x%04x\n", __HAL_NJPD_Read2Byte(NJPD_CLOCK));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_NJPD_SetClockGate(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_GATED_CLOCK_CTRL, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetClockGate : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_GATED_CLOCK_CTRL));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_GATED_CLOCK_CTRL, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetClockGate : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_GATED_CLOCK_CTRL));
    }
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U16 HAL_NJPD_GetClock(void)
{
    return __HAL_NJPD_Read2Byte(NJPD_CLOCK);
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_NJPD_PowerOn()
{
    MS_U16 u16reg_val;
    u16reg_val = HAL_NJPD_GetClock();
    u16reg_val = u16reg_val & 0xFFF2;
    u16reg_val |= 0x0000;  // set NJPD disable [0] : 0, [3:2] : 00 288MHz
    NJPD_DEBUG_HAL_MSG("%s : SetClock : 0x%04x\n", __FUNCTION__ , u16reg_val);

#ifdef CONFIG_MSTAR_SRAMPD
// reference to k6_register_assignment.xls
//1712H	0	1	1	1	1	0	0	0	0	codec_block	0	codec_gp
// reference to k6_block_top_reg.xls
// h11	h10	31	0	reg_codec_sram_sd_en	31	0	32	h00	rw
// bit [18] : jpd
    NJPD_DEBUG_HAL_MSG("NJPD sram on\n");
    *((volatile MS_U32 *)(MS_VIRT)(NJPD_RIU_MAP + 0x71200*2 + 0x10*2*2)) &= ~(1UL << 18);
    MsOS_DelayTask(1);
#endif

#ifdef CONFIG_MSTAR_CLKM
    MS_S32 handle = Drv_Clkm_Get_Handle("g_clk_njpd");
    Drv_Clkm_Set_Clk_Source(handle , "720p60fps_mode");
#else
    HAL_NJPD_SetClock(u16reg_val);
#endif
    NJPD_DEBUG_HAL_MSG("%s : GetClock : 0x%04x\n", __FUNCTION__ , HAL_NJPD_GetClock());
    HAL_NJPD_JPDSelect(TRUE);
#if 0
    // set the security range for test
    __HAL_NJPD_Write2Byte(NJPD_NONPM_SECURE_20, 0x0000);
    __HAL_NJPD_Write2Byte(NJPD_NONPM_SECURE_21, 0x0000);
    __HAL_NJPD_Write2Byte(NJPD_NONPM_SECURE_22, 0x0000);
    __HAL_NJPD_Write2Byte(NJPD_NONPM_SECURE_23, 0x0000);
    __HAL_NJPD_Write2Byte(NJPD_NONPM_SECURE_24, 0x0000);
    __HAL_NJPD_Write2Byte(NJPD_NONPM_SECURE_25, 0x0000);
    __HAL_NJPD_Write2Byte(NJPD_NONPM_SECURE_26, 0x0000);
    __HAL_NJPD_Write2Byte(NJPD_NONPM_SECURE_27, 0x0000);
    __HAL_NJPD_Write2Byte(NJPD_NONPM_SECURE_28, 0x0000);
    __HAL_NJPD_Write2Byte(NJPD_NONPM_SECURE_29, 0x0000);
    __HAL_NJPD_Write2Byte(NJPD_NONPM_SECURE_2A, 0x0000);
    __HAL_NJPD_Write2Byte(NJPD_NONPM_SECURE_2B, 0x0000);
    __HAL_NJPD_Write2Byte(NJPD_NONPM_SECURE_2C, 0x0000);
    __HAL_NJPD_Write2Byte(NJPD_NONPM_SECURE_2D, 0x0000);
    __HAL_NJPD_Write2Byte(NJPD_NONPM_SECURE_2E, 0x0000);
    __HAL_NJPD_Write2Byte(NJPD_NONPM_SECURE_2F, 0x0000);
#endif
#if 0   // test code to make sure the MCM function disabled
#define JPD_MCM_BASE               0x71200 //0x71F20
#define BK_MCM_GENERAL(x)       (JPD_MCM_BASE+NJPD_OFFSET(x))

//    MS_U8 u8i;
//    for(u8i=0; u8i<0x16; u8i++)
//    {
//        __HAL_JPD_Write2Byte(BK_MCM_GENERAL(u8i), 0);
//    }

//    __HAL_NJPD_Write2Byte(BK_MCM_GENERAL(0x01), 0x0000);
//    printf("MCM 0x%lx\n", 0x00);
//    __HAL_NJPD_Write2Byte(BK_MCM_GENERAL(0x01), 0xf200);
//    printf("MCM 0x%lx\n", 0xf2);

//    __HAL_NJPD_Write2Byte(BK_MCM_GENERAL(0x01), 0xf000);
//    printf("MCM 0x%lx\n", 0xf0);

    __HAL_NJPD_Write2Byte(BK_MCM_GENERAL(0x01), 0x1200);
    printf("MCM 0x%lx\n", 0x12);
#endif


#if MEASURE_NJPD_MIU_CLIENT_EFFICIENCY
    if(bJPEGFirstTime==0)
    {
       printf("bank: 0x1012, offset: 0x15, value=0x%x\n", __HAL_NJPD_Read2Byte(BK_MIU0_GENERAL(0x15)));
       __HAL_NJPD_Write2Byte(BK_MIU0_GENERAL(0x15), __HAL_NJPD_Read2Byte(BK_MIU0_GENERAL(0x15)) & ~0x80);
       printf("bank: 0x1012, offset: 0x15, value=0x%x\n", __HAL_NJPD_Read2Byte(BK_MIU0_GENERAL(0x15)));
       printf("bank: 0x1012, offset: 0x0d, value=0x%x\n", __HAL_NJPD_Read2Byte(BK_MIU0_GENERAL(0x0d)));
       __HAL_NJPD_Write2Byte(BK_MIU0_GENERAL(0x0d), 0x2734);
       printf("bank: 0x1012, offset: 0x0d, value=0x%x\n", __HAL_NJPD_Read2Byte(BK_MIU0_GENERAL(0x0d)));
       __HAL_NJPD_Write2Byte(BK_MIU0_GENERAL(0x0d), 0x2735);
       printf("bank: 0x1012, offset: 0x0d, value=0x%x\n", __HAL_NJPD_Read2Byte(BK_MIU0_GENERAL(0x0d)));

       bJPEGFirstTime=1;
    }
#endif
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_NJPD_PowerOff(void)
{
    MS_U16 u16reg_val;
    u16reg_val = HAL_NJPD_GetClock();
    u16reg_val = u16reg_val & 0xFFF2;
    u16reg_val |= 0x0001;  // set NJPD disable [8] : 1, [3:2] : 00 288MHz
    NJPD_DEBUG_HAL_MSG("%s : SetClock : 0x%04x\n", __FUNCTION__ , u16reg_val);
#ifdef CONFIG_MSTAR_CLKM
    MS_S32 handle = Drv_Clkm_Get_Handle("g_clk_njpd");
    Drv_Clkm_Clk_Gate_Disable(handle);
#else
    HAL_NJPD_SetClock(u16reg_val);
#endif


#ifdef CONFIG_MSTAR_SRAMPD
// reference to k6_register_assignment.xls
//1712H	0	1	1	1	1	0	0	0	0	codec_block	0	codec_gp
// reference to k6_block_top_reg.xls
// h11	h10	31	0	reg_codec_sram_sd_en	31	0	32	h00	rw
// bit [18] : jpd
    NJPD_DEBUG_HAL_MSG("NJPD sram off\n");
    *((volatile MS_U32 *)(MS_VIRT)(NJPD_RIU_MAP + 0x71200*2 + 0x10*2*2)) |= (1UL << 18);
    MsOS_DelayTask(1);
#endif
    NJPD_DEBUG_HAL_MSG("%s : GetClock : 0x%04x\n", __FUNCTION__ , HAL_NJPD_GetClock());
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_NJPD_InitRegBase(void)
{
    MS_PHY u32NonPMBankSize;
    if(!MDrv_MMIO_GetBASE(&u32NJPDRiuBaseAdd, &u32NonPMBankSize, MS_MODULE_JPD))
    {
        NJPD_DEBUG_HAL_ERR("Get RIUreg base Failed!!!\n");
    }
    else
    {
        NJPD_DEBUG_HAL_MSG("RIUreg base = 0x%tx, length = %tx\n", (ptrdiff_t)u32NJPDRiuBaseAdd, (ptrdiff_t)u32NonPMBankSize);
    }
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return none
/******************************************************************************/
void HAL_NJPD_CreateMutex(void)
{
    __HAL_NJPD_MutexCreate();
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return none
/******************************************************************************/
void HAL_NJPD_DeleteMutex(void)
{
    __HAL_NJPD_MutexDelete();
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_NJPD_SetRSTIntv(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_RESTART_INTERVAL, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetRSTIntv : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_RESTART_INTERVAL));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_RESTART_INTERVAL, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetRSTIntv : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_RESTART_INTERVAL));
    }
}


/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return none
/******************************************************************************/
void HAL_NJPD_SetDbgLevel(MS_U8 u8DbgLevel)
{
    _u8NJPDHalDbgLevel = u8DbgLevel;
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return none
/******************************************************************************/
void HAL_NJPD_GetLibVer(NJPD_HAL_Version *pHalVer)
{
    pHalVer->pu8HalVer = _pu8HalVer;
    pHalVer->pu8FwVer = _pu8FwVer;
}

/******************************************************************************/
///Reset NJPD
///@param value \b IN
///@param value \b OUT
///@return none
/******************************************************************************/
void HAL_NJPD_Rst(void)
{
    NJPD_DEBUG_HAL_MSG("%s : start!!\n", __FUNCTION__);
    _HAL_NJPD_SetMIUProtectMask(TRUE);

    // reset event flag
    HAL_NJPD_ClearEventFlag(NJPD_EVENT_ALL);
    // reset : low active

#if ENABLE_TEST_NJPD_17_Obuf_Output_Format
    HAL_NJPD_SetOutputFormat(TRUE, E_NJPD_OUTPUT_YC_SWAP);
//    HAL_NJPD_SetOutputFormat(TRUE, E_NJPD_OUTPUT_UV_SWAP);
//    HAL_NJPD_SetOutputFormat(TRUE, E_NJPD_OUTPUT_UV_7BIT);
//    HAL_NJPD_SetOutputFormat(TRUE, E_NJPD_OUTPUT_UV_MSB);
#else
    if(HAL_NJPD_GetVerificationMode()==E_NJPD17_OBUF_OUTPUT_FORMAT_YC_SWAP)
    {
        HAL_NJPD_SetOutputFormat(TRUE, E_NJPD_OUTPUT_YC_SWAP);
    }
    else if(HAL_NJPD_GetVerificationMode()==E_NJPD17_OBUF_OUTPUT_FORMAT_UV_SWAP)
    {
        HAL_NJPD_SetOutputFormat(TRUE, E_NJPD_OUTPUT_UV_SWAP);
    }
    else if(HAL_NJPD_GetVerificationMode()==E_NJPD17_OBUF_OUTPUT_FORMAT_UV_7BIT)
    {
        HAL_NJPD_SetOutputFormat(TRUE, E_NJPD_OUTPUT_UV_7BIT);
    }
    else if(HAL_NJPD_GetVerificationMode()==E_NJPD17_OBUF_OUTPUT_FORMAT_UV_MSB)
    {
        HAL_NJPD_SetOutputFormat(TRUE, E_NJPD_OUTPUT_UV_MSB);
    }
    else
    {
        HAL_NJPD_SetOutputFormat(TRUE, E_NJPD_OUTPUT_ORIGINAL);
    }
#endif

#if ENABLE_NJPD_DEBUG_MSG
    HAL_NJPD_Set_GlobalSetting02(0xFC | NJPD_TBC_MODE | NJPD_LITTLE_ENDIAN | NJPD_REMOVE_0xFF00 |
         NJPD_REMOVE_0xFFFF |  NJPD_HUFF_DATA_LOSS_ERROR);
#else
    if(HAL_NJPD_GetVerificationMode()==E_NJPD01_TABLE_READ_WRITE)
    {
        HAL_NJPD_Set_GlobalSetting02(0xFC | NJPD_TBC_MODE | NJPD_LITTLE_ENDIAN | NJPD_REMOVE_0xFF00 |
             NJPD_REMOVE_0xFFFF |  NJPD_HUFF_DATA_LOSS_ERROR);
    }
    else
    {
        HAL_NJPD_Set_GlobalSetting02(NJPD_TBC_MODE | NJPD_LITTLE_ENDIAN | NJPD_REMOVE_0xFF00 |
             NJPD_REMOVE_0xFFFF |  NJPD_HUFF_DATA_LOSS_ERROR);
    }
#endif
    HAL_NJPD_Set_GlobalSetting00(HAL_NJPD_Get_GlobalSetting00() | NJPD_SWRST);
#if 0
HAL_NJPD_MaskEventFlag(0xffff);

#else
    HAL_NJPD_MaskEventFlag(~(NJPD_EVENT_DECODE_DONE | NJPD_EVENT_MINICODE_ERR |
            NJPD_EVENT_INV_SCAN_ERR | NJPD_EVENT_RES_MARKER_ERR | NJPD_EVENT_RMID_ERR |
            NJPD_EVENT_END_IMAGE_ERR | NJPD_EVENT_MRC0_EMPTY | NJPD_EVENT_MRC1_EMPTY |
            NJPD_EVENT_WRITE_PROTECT | NJPD_EVENT_DATA_LOSS_ERR |
            NJPD_EVENT_HUFF_TABLE_ERR));  // set 0 to turn on the irq
#endif

    NJPD_DEBUG_HAL_MSG("Get BK_NJPD_GLOBAL_SETTING00 : 0x%04x\n", HAL_NJPD_Get_GlobalSetting00());
    NJPD_DEBUG_HAL_MSG("Get BK_NJPD_GLOBAL_SETTING01 : 0x%04x\n", HAL_NJPD_Get_GlobalSetting01());
    NJPD_DEBUG_HAL_MSG("Get BK_NJPD_GLOBAL_SETTING02 : 0x%04x\n", HAL_NJPD_Get_GlobalSetting02());
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        NJPD_DEBUG_HAL_MSG("HAL Get INTEN : 0x%04x\n",  __HAL_NJPD_Read2Byte(BK_NJPD1_IRQ_MASK));
        NJPD_DEBUG_HAL_MSG("HAL Get EVENTFLAG : 0x%04x\n",  __HAL_NJPD_Read2Byte(BK_NJPD1_IRQ_FINAL_S));
    }
    else
    {
        NJPD_DEBUG_HAL_MSG("HAL Get INTEN : 0x%04x\n",  __HAL_NJPD_Read2Byte(BK_NJPD2_IRQ_MASK));
        NJPD_DEBUG_HAL_MSG("HAL Get EVENTFLAG : 0x%04x\n",  __HAL_NJPD_Read2Byte(BK_NJPD2_IRQ_FINAL_S));
    }

// Edison@20120620 by Arong
// 1. Add the fucntion of auto last buffer when FFD9 is detected (reg_spare00[0])
// 2. Fix last buffer error (reg_spare00[1])
//// 3. write burst_error (reg_spare00[3])                // Edison/Einstein doesnt have this bit
//// 4. Fix read-burst error (reg_spare00[4])             // Edison/Einstein doesnt have this bit
// 5. Coding error (reg_spare00[5])
// 6. read-burst error (reg_spare00[6])

// Kano@20150326 by Ethan Chang
// 7. let IP to MIU's last signel would trigger down when ready_ack(reg_spare00[7]=1    // When Kano BWP function enable would affect the result

// Nike&Einstein@20130412 by Tony Tseng
// 9. ECO new error concealment(reg_spare00[9])      // Nike have this bit

    HAL_NJPD_SetSpare00(0x2FB);

#if 0
    MS_U16 u16Value = ((HAL_NJPD_Get_MARB07() & ~NJPD_JPD_MARB_BURST_SPLIT) | 0x0002<<12);
    printf("MARB07=0x%x\n", u16Value);
    HAL_NJPD_Set_MARB07(u16Value);
#endif
    _HAL_NJPD_SetMIUProtectMask(FALSE);

#if ENABLE_TEST_NJPD_18_Ibuf_Burst_Length_Test
    HAL_NJPD_SetIBufReadLength(0x15, 0x1a);
#else
    if(HAL_NJPD_GetVerificationMode()==E_NJPD18_IBUF_BURST_LENGTH)
    {
        HAL_NJPD_SetIBufReadLength(0x15, 0x1a);
    }
    else
    {
        if(HAL_NJPD_IsNeedToPatch(E_NJPD_EAGLE_SW_PATCH))
        {
            NJPD_DEBUG_HAL_MSG("EAGLE_SW_PATCH start!!!!!!\n");
            HAL_NJPD_SetIBufReadLength(0x1f, 0x1f);
            HAL_NJPD_SetMRBurstThd(0x10);
            MS_U16 u16Value = (HAL_NJPD_Get_MARB07() | 0xc01f);
            NJPD_DEBUG_HAL_MSG("MARB07=0x%x\n", u16Value);
            HAL_NJPD_Set_MARB07(u16Value);
            NJPD_DEBUG_HAL_MSG("EAGLE_SW_PATCH end!!!!!!\n");
        }
        else
        {
            HAL_NJPD_SetIBufReadLength(0x10, 0x1f);
        }
    }
#endif

#if ENABLE_NJPD_DEBUG_MSG
    //HAL_NJPD_SetCRCReadMode();
    HAL_NJPD_SetCRCWriteMode();
#else
    if(HAL_NJPD_GetVerificationMode()==E_NJPD01_TABLE_READ_WRITE)
    {
        HAL_NJPD_SetCRCWriteMode();
    }
    else
    {
        HAL_NJPD_SetCRCReadMode();
    }
#endif
    NJPD_DEBUG_HAL_MSG("%s : end!!\n", __FUNCTION__);
}

void HAL_NJPD_SetMWBuffLineNum(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : NOT IMPLEMENTED!!!\n", __FUNCTION__);
    UNUSED(u16Value);
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U32 HAL_NJPD_GetCurMRCAddr(void)
{
    MS_U32 curMRCAddr;
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        curMRCAddr = ((__HAL_NJPD_Read2Byte(BK_NJPD1_MIU_READ_POINTER_ADDR_H) << 16) | __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_READ_POINTER_ADDR_L));
    }
    else
    {
        curMRCAddr = ((__HAL_NJPD_Read2Byte(BK_NJPD2_MIU_READ_POINTER_ADDR_H) << 16) | __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_READ_POINTER_ADDR_L));
    }

    return curMRCAddr;
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U16 HAL_NJPD_GetCurRow(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_ROW_IDEX);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_ROW_IDEX);
    }
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U16 HAL_NJPD_GetCurCol(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_COLUMN_IDEX);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_COLUMN_IDEX);
    }
}

void HAL_NJPD_SetWriteProtect(MS_BOOL enable)
{
    NJPD_DEBUG_HAL_MSG("%s : NOT IMPLEMENTED!!!\n", __FUNCTION__);
    UNUSED(enable);
}

void HAL_NJPD_SetAutoProtect(MS_BOOL enable)
{
    MS_U16 u16RegValue;
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        u16RegValue = __HAL_NJPD_Read2Byte(BK_NJPD1_MARB_LBOUND_0_H);
        u16RegValue &= 0x1FFF;
        u16RegValue |= enable<<13;
        __HAL_NJPD_Write2Byte(BK_NJPD1_MARB_LBOUND_0_H, u16RegValue);
        NJPD_DEBUG_HAL_MSG("%s : 0x%04X\n", __FUNCTION__, __HAL_NJPD_Read2Byte(BK_NJPD1_MARB_LBOUND_0_H));
    }
    else
    {
        u16RegValue = __HAL_NJPD_Read2Byte(BK_NJPD2_MARB_LBOUND_0_H);
        u16RegValue &= 0x1FFF;
        u16RegValue |= enable<<13;
        __HAL_NJPD_Write2Byte(BK_NJPD2_MARB_LBOUND_0_H, u16RegValue);
        NJPD_DEBUG_HAL_MSG("%s : 0x%04X\n", __FUNCTION__, __HAL_NJPD_Read2Byte(BK_NJPD2_MARB_LBOUND_0_H));
    }
}


void HAL_NJPD_Set_MARB06(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04X\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MARB_SETTING_06, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_Get_MARB06 : 0x%04X\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MARB_SETTING_06));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MARB_SETTING_06, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_Get_MARB06 : 0x%04X\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MARB_SETTING_06));
    }
}

MS_U16 HAL_NJPD_Get_MARB06(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_MARB_SETTING_06);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_MARB_SETTING_06);
    }
}

void HAL_NJPD_Set_MARB07(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04X\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MARB_SETTING_07, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_Get_MARB07 : 0x%04X\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MARB_SETTING_07));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MARB_SETTING_07, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_Get_MARB07 : 0x%04X\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MARB_SETTING_07));
    }
}

MS_U16 HAL_NJPD_Get_MARB07(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_MARB_SETTING_07);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_MARB_SETTING_07);
    }
}


void HAL_NJPD_SetWPENUBound_0_L(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04X\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MARB_UBOUND_0_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetWPENUBound_0_L : 0x%04X\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MARB_UBOUND_0_L));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MARB_UBOUND_0_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetWPENUBound_0_L : 0x%04X\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MARB_UBOUND_0_L));
    }
}
void HAL_NJPD_SetWPENUBound_0_H(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04X\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MARB_UBOUND_0_H, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetWPENUBound_0_H : 0x%04X\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MARB_UBOUND_0_H));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MARB_UBOUND_0_H, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetWPENUBound_0_H : 0x%04X\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MARB_UBOUND_0_H));
    }
}
void HAL_NJPD_SetWPENLBound_0_L(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04X\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MARB_LBOUND_0_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetWPENLBound_0_L : 0x%04X\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MARB_LBOUND_0_L));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MARB_LBOUND_0_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetWPENLBound_0_L : 0x%04X\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MARB_LBOUND_0_L));
    }
}
void HAL_NJPD_SetWPENLBound_0_H(MS_U16 u16Value)
{
    MS_U16 u16RegValue;
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        u16RegValue = __HAL_NJPD_Read2Byte(BK_NJPD1_MARB_LBOUND_0_H);
        u16RegValue &= 0xE000;
        u16Value &= 0x1FFF;
        u16RegValue |= u16Value;
        NJPD_DEBUG_HAL_MSG("%s : 0x%04X\n", __FUNCTION__, u16RegValue);
        __HAL_NJPD_Write2Byte(BK_NJPD1_MARB_LBOUND_0_H, u16RegValue);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetWPENLBound_0_H : 0x%04X\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MARB_LBOUND_0_H));
    }
    else
    {
        u16RegValue = __HAL_NJPD_Read2Byte(BK_NJPD2_MARB_LBOUND_0_H);
        u16RegValue &= 0xE000;
        u16Value &= 0x1FFF;
        u16RegValue |= u16Value;
        NJPD_DEBUG_HAL_MSG("%s : 0x%04X\n", __FUNCTION__, u16RegValue);
        __HAL_NJPD_Write2Byte(BK_NJPD2_MARB_LBOUND_0_H, u16RegValue);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetWPENLBound_0_H : 0x%04X\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MARB_LBOUND_0_H));
    }
}

void HAL_NJPD_SetSpare00(MS_U16 u16Value)
{
//  [0]: hw auto detect ffd9 => cannot be used in svld mode, unless the "last buffer" cannot work, do not enable this
//  [1]: input buffer bug => always turn on
//  [2]: marb bug => always turn on (not used in 2011/12/28 ECO version)
//  [3]: bypass marb => 2011/12/28 ECO version

     NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_SPARE00, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetSpare00 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_SPARE00));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_SPARE00, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetSpare00 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_SPARE00));
    }
}
MS_U16 HAL_NJPD_GetSpare00(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_SPARE00);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_SPARE00);
    }
}

void HAL_NJPD_SetSpare01(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_SPARE01, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetSpare01 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_SPARE01));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_SPARE01, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetSpare01 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_SPARE01));
    }
}
MS_U16 HAL_NJPD_GetSpare01(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_SPARE01);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_SPARE01);
    }
}

void HAL_NJPD_SetSpare02(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_SPARE02, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetSpare02 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_SPARE02));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_SPARE02, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetSpare02 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_SPARE02));
    }
}
MS_U16 HAL_NJPD_GetSpare02(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_SPARE02);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_SPARE02);
    }
}

void HAL_NJPD_SetSpare03(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_SPARE03, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetSpare03 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_SPARE03));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_SPARE03, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetSpare03 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_SPARE03));
    }
}
MS_U16 HAL_NJPD_GetSpare03(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_SPARE03);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_SPARE03);
    }
}

void HAL_NJPD_SetSpare04(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_SPARE04, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetSpare04 : 0x%04x\n",  __HAL_NJPD_Read2Byte(BK_NJPD1_SPARE04));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_SPARE04, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetSpare04 : 0x%04x\n",  __HAL_NJPD_Read2Byte(BK_NJPD2_SPARE04));
    }
}
MS_U16 HAL_NJPD_GetSpare04(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_SPARE04);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_SPARE04);
    }
}

void HAL_NJPD_SetSpare05(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_SPARE05, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetSpare05 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_SPARE05));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_SPARE05, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetSpare05 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_SPARE05));
    }
}
MS_U16 HAL_NJPD_GetSpare05(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_SPARE05);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_SPARE05);
    }
}

void HAL_NJPD_SetSpare06(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_SPARE06, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetSpare06 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_SPARE06));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_SPARE06, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetSpare06 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_SPARE06));
    }
}
MS_U16 HAL_NJPD_GetSpare06(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_SPARE06);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_SPARE06);
    }
}

void HAL_NJPD_SetSpare07(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_SPARE07, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetSpare07 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_SPARE07));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_SPARE07, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetSpare07 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_SPARE07));
    }
}
MS_U16 HAL_NJPD_GetSpare07(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_SPARE07);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_SPARE07);
    }
}

void HAL_NJPD_SetWriteOneClearReg(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_WRITE_ONE_CLEAR, u16Value);
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_WRITE_ONE_CLEAR, u16Value);
    }
}

void HAL_NJPD_SetWriteOneClearReg_2(MS_U16 u16Value)
{
    // for convience, not to print any message in this function
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_WRITE_ONE_CLEAR, u16Value);
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_WRITE_ONE_CLEAR, u16Value);
    }
}

MS_U16 HAL_NJPD_GetWriteOneClearReg(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_WRITE_ONE_CLEAR);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_WRITE_ONE_CLEAR);
    }
}

void HAL_NJPD_SetHTableStart_Low(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MIU_HTABLE_START_ADDR_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetHTableStart_Low : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_HTABLE_START_ADDR_L));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MIU_HTABLE_START_ADDR_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetHTableStart_Low : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_HTABLE_START_ADDR_L));
    }
}

void HAL_NJPD_SetHTableStart_High(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MIU_HTABLE_START_ADDR_H, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetHTableStart_High : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_HTABLE_START_ADDR_H));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MIU_HTABLE_START_ADDR_H, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetHTableStart_High : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_HTABLE_START_ADDR_H));
    }
}

void HAL_NJPD_SetQTableStart_Low(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MIU_QTABLE_START_ADDR_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetQTableStart_Low : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_QTABLE_START_ADDR_L));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MIU_QTABLE_START_ADDR_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetQTableStart_Low : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_QTABLE_START_ADDR_L));
    }
}

void HAL_NJPD_SetQTableStart_High(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MIU_QTABLE_START_ADDR_H, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetQTableStart_High : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_QTABLE_START_ADDR_H));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MIU_QTABLE_START_ADDR_H, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetQTableStart_High : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_QTABLE_START_ADDR_H));
    }
}

void HAL_NJPD_SetGTableStart_Low(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MIU_GTABLE_START_ADDR_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetGTableStart_Low : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_GTABLE_START_ADDR_L));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MIU_GTABLE_START_ADDR_L, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetGTableStart_Low : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_GTABLE_START_ADDR_L));
    }
}

void HAL_NJPD_SetGTableStart_High(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MIU_GTABLE_START_ADDR_H, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetGTableStart_High : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_GTABLE_START_ADDR_H));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MIU_GTABLE_START_ADDR_H, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetGTableStart_High : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_GTABLE_START_ADDR_H));
    }
}

void HAL_NJPD_SetHTableSize(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    u16Value &=0x00FF;
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_MIU_HTABLE_SIZE, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetHTableSize : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_HTABLE_SIZE));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_MIU_HTABLE_SIZE, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetHTableSize : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_HTABLE_SIZE));
    }
}

MS_U16 HAL_NJPD_GetHTableSize()
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_HTABLE_SIZE);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_HTABLE_SIZE);
    }
}
void HAL_NJPD_SetRIUInterface(MS_U16 u16Value)
{
//    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_TBC, u16Value);
//        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetRIUInterface : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_TBC));
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_TBC, u16Value);
//        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetRIUInterface : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_TBC));
    }
}

MS_U16 HAL_NJPD_GetRIUInterface(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_TBC);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_TBC);
    }
}

MS_U16 HAL_NJPD_TBCReadData_L()
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_TBC_RDATA_L);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_TBC_RDATA_L);
    }
}

MS_U16 HAL_NJPD_TBCReadData_H()
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD1_TBC_RDATA_H);
    }
    else
    {
        return __HAL_NJPD_Read2Byte(BK_NJPD2_TBC_RDATA_H);
    }
}

void HAL_NJPD_SetIBufReadLength(MS_U8 u8Min, MS_U8 u8Max)
{
    MS_U16 u16Value;
    u16Value = (u8Min<<5) + u8Max;
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        u16Value = (u16Value | (__HAL_NJPD_Read2Byte(BK_NJPD1_IBUF_READ_LENGTH) & 0xfc00));
        __HAL_NJPD_Write2Byte(BK_NJPD1_IBUF_READ_LENGTH, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetIBufReadLength : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_IBUF_READ_LENGTH));
    }
    else
    {
        u16Value = (u16Value | (__HAL_NJPD_Read2Byte(BK_NJPD2_IBUF_READ_LENGTH) & 0xfc00));
        __HAL_NJPD_Write2Byte(BK_NJPD2_IBUF_READ_LENGTH, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetIBufReadLength : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_IBUF_READ_LENGTH));
    }
}


void HAL_NJPD_SetMRBurstThd(MS_U16 u16Value)
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, u16Value);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        u16Value = (u16Value | (__HAL_NJPD_Read2Byte(BK_NJPD1_MARB_SETTING_04) & 0xffe0));
        __HAL_NJPD_Write2Byte(BK_NJPD1_MARB_SETTING_04, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD1_GetMARB_SETTING_04 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MARB_SETTING_04));
    }
    else
    {
        u16Value = (u16Value | (__HAL_NJPD_Read2Byte(BK_NJPD2_MARB_SETTING_04) & 0xffe0));
        __HAL_NJPD_Write2Byte(BK_NJPD2_MARB_SETTING_04, u16Value);
        NJPD_DEBUG_HAL_MSG("HAL_NJPD2_GetMARB_SETTING_04 : 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MARB_SETTING_04));
    }
}



void HAL_NJPD_SetCRCReadMode()
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, 0x200);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_CRC_MODE, 0x200);
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_CRC_MODE, 0x200);
    }
}

void HAL_NJPD_SetCRCWriteMode()
{
    NJPD_DEBUG_HAL_MSG("%s : 0x%04x\n", __FUNCTION__, 0x300);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        __HAL_NJPD_Write2Byte(BK_NJPD1_CRC_MODE, 0x300);
    }
    else
    {
        __HAL_NJPD_Write2Byte(BK_NJPD2_CRC_MODE, 0x300);
    }
}

void HAL_NJPD_ResetMarb(void)
{
    NJPD_DEBUG_HAL_MSG("%s\n", __FUNCTION__);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        NJPD_DEBUG_HAL_MSG("set BK_NJPD1_MARB_RESET to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD1_MARB_RESET) & 0xFE);
        __HAL_NJPD_WriteByte(BK_NJPD1_MARB_RESET, __HAL_NJPD_ReadByte(BK_NJPD1_MARB_RESET) & 0xFE);
        NJPD_DEBUG_HAL_MSG("set BK_NJPD1_MARB_RESET to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD1_MARB_RESET) | 0x01);
        __HAL_NJPD_WriteByte(BK_NJPD1_MARB_RESET, __HAL_NJPD_ReadByte(BK_NJPD1_MARB_RESET) | 0x01);
        NJPD_DEBUG_HAL_MSG("set BK_NJPD1_MARB_RESET to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD1_MARB_RESET) & 0xFD);
        __HAL_NJPD_WriteByte(BK_NJPD1_MARB_RESET, __HAL_NJPD_ReadByte(BK_NJPD1_MARB_RESET) & 0xFD);
        NJPD_DEBUG_HAL_MSG("set BK_NJPD1_MARB_RESET to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD1_MARB_RESET) | 0x02);
        __HAL_NJPD_WriteByte(BK_NJPD1_MARB_RESET, __HAL_NJPD_ReadByte(BK_NJPD1_MARB_RESET) | 0x02);
//        __HAL_NJPD_WriteByte(BK_NJPD1_MARB_RESET, __HAL_NJPD_ReadByte(BK_NJPD1_MARB_RESET) | 0x12);
//        NJPD_DEBUG_HAL_MSG("!!set BK_NJPD1_MARB_RESET to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD1_MARB_RESET));
    }
    else
    {
        NJPD_DEBUG_HAL_MSG("set BK_NJPD2_MARB_RESET to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD2_MARB_RESET) & 0xFE);
        __HAL_NJPD_WriteByte(BK_NJPD2_MARB_RESET, __HAL_NJPD_ReadByte(BK_NJPD2_MARB_RESET) & 0xFE);
        NJPD_DEBUG_HAL_MSG("set BK_NJPD2_MARB_RESET to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD2_MARB_RESET) | 0x01);
        __HAL_NJPD_WriteByte(BK_NJPD2_MARB_RESET, __HAL_NJPD_ReadByte(BK_NJPD2_MARB_RESET) | 0x01);
        NJPD_DEBUG_HAL_MSG("set BK_NJPD2_MARB_RESET to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD2_MARB_RESET) & 0xFD);
        __HAL_NJPD_WriteByte(BK_NJPD2_MARB_RESET, __HAL_NJPD_ReadByte(BK_NJPD2_MARB_RESET) & 0xFD);
        NJPD_DEBUG_HAL_MSG("set BK_NJPD2_MARB_RESET to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD2_MARB_RESET) | 0x02);
        __HAL_NJPD_WriteByte(BK_NJPD2_MARB_RESET, __HAL_NJPD_ReadByte(BK_NJPD2_MARB_RESET) | 0x02);
//        __HAL_NJPD_WriteByte(BK_NJPD2_MARB_RESET, __HAL_NJPD_ReadByte(BK_NJPD2_MARB_RESET) | 0x12);
//        NJPD_DEBUG_HAL_MSG("!!set BK_NJPD2_MARB_RESET to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD2_MARB_RESET));
    }

}

MS_U8 HAL_NJPD_GetHandshakeCnt()
{
    MS_U8 u8Cnt;
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        // reg_jpd_handshake_cnt
        u8Cnt = (MS_U8)__HAL_NJPD_ReadByte(BK_NJPD1_HANDSHAKE_CNT);
    }
    else
    {
        // reg_jpd_handshake_cnt
        u8Cnt = (MS_U8)__HAL_NJPD_ReadByte(BK_NJPD2_HANDSHAKE_CNT);
    }
    return u8Cnt;
}

void HAL_NJPD_Handshake_SWRowCountMode(MS_U8 u8Cnt)
{
    NJPD_DEBUG_HAL_MSG("%s() with Count=%d\n", __FUNCTION__, u8Cnt);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        // reg_jpd_handshake_sw_mode = 1
        NJPD_DEBUG_HAL_MSG("set BK_NJPD1_HANDSHAKE to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD1_HANDSHAKE) | 0x04);
        __HAL_NJPD_WriteByte(BK_NJPD1_HANDSHAKE, __HAL_NJPD_ReadByte(BK_NJPD1_HANDSHAKE) | 0x04);
        // reg_jpd_handshake_en =1
        NJPD_DEBUG_HAL_MSG("set BK_NJPD1_HANDSHAKE to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD1_HANDSHAKE) | 0x02);
        __HAL_NJPD_WriteByte(BK_NJPD1_HANDSHAKE, __HAL_NJPD_ReadByte(BK_NJPD1_HANDSHAKE) | 0x02);
        // reg_jpd_sw_mb_row_cnt
        NJPD_DEBUG_HAL_MSG("set BK_NJPD1_SW_MB_ROW_CNT to 0x%02x\n", u8Cnt);
        __HAL_NJPD_WriteByte(BK_NJPD1_SW_MB_ROW_CNT, u8Cnt);

        // reg_jpd_handshake_sw_woc
        HAL_NJPD_SetWriteOneClearReg(NJPD_HANDSHAKE_SW_WOC);
    }
    else
    {
        // reg_jpd_handshake_sw_mode = 1
        NJPD_DEBUG_HAL_MSG("set BK_NJPD2_HANDSHAKE to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD2_HANDSHAKE) | 0x04);
        __HAL_NJPD_WriteByte(BK_NJPD2_HANDSHAKE, __HAL_NJPD_ReadByte(BK_NJPD2_HANDSHAKE) | 0x04);
        // reg_jpd_handshake_en =1
        NJPD_DEBUG_HAL_MSG("set BK_NJPD2_HANDSHAKE to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD2_HANDSHAKE) | 0x02);
        __HAL_NJPD_WriteByte(BK_NJPD2_HANDSHAKE, __HAL_NJPD_ReadByte(BK_NJPD2_HANDSHAKE) | 0x02);
        // reg_jpd_sw_mb_row_cnt
        NJPD_DEBUG_HAL_MSG("set BK_NJPD2_SW_MB_ROW_CNT to 0x%02x\n", u8Cnt);
        __HAL_NJPD_WriteByte(BK_NJPD2_SW_MB_ROW_CNT, u8Cnt);

        // reg_jpd_handshake_sw_woc
        HAL_NJPD_SetWriteOneClearReg(NJPD_HANDSHAKE_SW_WOC);
    }
}

void HAL_NJPD_Handshake_HWRowCountMode()
{
    NJPD_DEBUG_HAL_MSG("%s()\n", __FUNCTION__);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        // reset reg_jpd_handshake_cnt_rst
        NJPD_DEBUG_HAL_MSG("set BK_NJPD1_HANDSHAKE to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD1_HANDSHAKE) & 0xFE);
        __HAL_NJPD_WriteByte(BK_NJPD1_HANDSHAKE, __HAL_NJPD_ReadByte(BK_NJPD1_HANDSHAKE) & 0xFE);
        NJPD_DEBUG_HAL_MSG("set BK_NJPD1_HANDSHAKE to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD1_HANDSHAKE) | 0x01);
        __HAL_NJPD_WriteByte(BK_NJPD1_HANDSHAKE, __HAL_NJPD_ReadByte(BK_NJPD1_HANDSHAKE) | 0x01);

        // reg_jpd_handshake_sw_mode = 0
        NJPD_DEBUG_HAL_MSG("set BK_NJPD1_HANDSHAKE to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD1_HANDSHAKE) & 0xFB);
        __HAL_NJPD_WriteByte(BK_NJPD1_HANDSHAKE, __HAL_NJPD_ReadByte(BK_NJPD1_HANDSHAKE) & 0xFB);

        // reg_jpd_handshake_en =1
        NJPD_DEBUG_HAL_MSG("set BK_NJPD1_HANDSHAKE to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD1_HANDSHAKE) | 0x02);
        __HAL_NJPD_WriteByte(BK_NJPD1_HANDSHAKE, __HAL_NJPD_ReadByte(BK_NJPD1_HANDSHAKE) | 0x02);
    }
    else
    {
        // reset reg_jpd_handshake_cnt_rst
        NJPD_DEBUG_HAL_MSG("set BK_NJPD2_HANDSHAKE to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD2_HANDSHAKE) & 0xFE);
        __HAL_NJPD_WriteByte(BK_NJPD2_HANDSHAKE, __HAL_NJPD_ReadByte(BK_NJPD2_HANDSHAKE) & 0xFE);
        NJPD_DEBUG_HAL_MSG("set BK_NJPD2_HANDSHAKE to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD2_HANDSHAKE) | 0x01);
        __HAL_NJPD_WriteByte(BK_NJPD2_HANDSHAKE, __HAL_NJPD_ReadByte(BK_NJPD2_HANDSHAKE) | 0x01);

        // reg_jpd_handshake_sw_mode = 0
        NJPD_DEBUG_HAL_MSG("set BK_NJPD2_HANDSHAKE to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD2_HANDSHAKE) & 0xFB);
        __HAL_NJPD_WriteByte(BK_NJPD2_HANDSHAKE, __HAL_NJPD_ReadByte(BK_NJPD2_HANDSHAKE) & 0xFB);

        // reg_jpd_handshake_en =1
        NJPD_DEBUG_HAL_MSG("set BK_NJPD2_HANDSHAKE to 0x%02x\n", __HAL_NJPD_ReadByte(BK_NJPD2_HANDSHAKE) | 0x02);
        __HAL_NJPD_WriteByte(BK_NJPD2_HANDSHAKE, __HAL_NJPD_ReadByte(BK_NJPD2_HANDSHAKE) | 0x02);
    }
}


void HAL_NJPD_SetOutputFormat(MS_BOOL bRst, NJPD_OutputFormat eOutputFormat)
{
    switch(eOutputFormat)
    {
        case E_NJPD_OUTPUT_ORIGINAL:
            if(bRst)
            {
                HAL_NJPD_Set_GlobalSetting00(0);
                HAL_NJPD_ResetMarb();
                HAL_NJPD_Set_GlobalSetting01(HAL_NJPD_Get_GlobalSetting01()& NJPD_GTABLE_RST);
            }
            else
            {
                HAL_NJPD_Set_GlobalSetting00(HAL_NJPD_Get_GlobalSetting00()&0xf3ff);
                HAL_NJPD_Set_GlobalSetting01(HAL_NJPD_Get_GlobalSetting01()&0xfcff);
            }
            break;
        case E_NJPD_OUTPUT_YC_SWAP:
            if(bRst)
            {
                HAL_NJPD_Set_GlobalSetting00(NJPD_YC_SWAP);
                HAL_NJPD_Set_GlobalSetting01(HAL_NJPD_Get_GlobalSetting01()& NJPD_GTABLE_RST);
            }
            else
            {
                HAL_NJPD_Set_GlobalSetting00((HAL_NJPD_Get_GlobalSetting00()&0xf3ff) | NJPD_YC_SWAP);
                HAL_NJPD_Set_GlobalSetting01(HAL_NJPD_Get_GlobalSetting01()&0xfcff);
            }
            break;
        case E_NJPD_OUTPUT_UV_SWAP:
            if(bRst)
            {
                HAL_NJPD_Set_GlobalSetting00(NJPD_UV_SWAP);
                HAL_NJPD_Set_GlobalSetting01(HAL_NJPD_Get_GlobalSetting01()& NJPD_GTABLE_RST);
            }
            else
            {
                HAL_NJPD_Set_GlobalSetting00((HAL_NJPD_Get_GlobalSetting00()&0xf3ff) | NJPD_UV_SWAP);
                HAL_NJPD_Set_GlobalSetting01(HAL_NJPD_Get_GlobalSetting01()&0xfcff);
            }
            break;
        case E_NJPD_OUTPUT_UV_7BIT:
            if(bRst)
            {
                HAL_NJPD_Set_GlobalSetting00(0);
//                HAL_NJPD_Set_GlobalSetting01(NJPD_UV_7BIT);
                HAL_NJPD_Set_GlobalSetting01((HAL_NJPD_Get_GlobalSetting01()& NJPD_GTABLE_RST)|NJPD_UV_7BIT);
            }
            else
            {
                HAL_NJPD_Set_GlobalSetting00(HAL_NJPD_Get_GlobalSetting00()&0xf3ff);
                HAL_NJPD_Set_GlobalSetting01((HAL_NJPD_Get_GlobalSetting01()&0xfcff) | NJPD_UV_7BIT);
            }

            break;
        case E_NJPD_OUTPUT_UV_MSB:
            if(bRst)
            {
                HAL_NJPD_Set_GlobalSetting00(0);
//                HAL_NJPD_Set_GlobalSetting01(NJPD_UV_7BIT | NJPD_UV_MSB);
                HAL_NJPD_Set_GlobalSetting01((HAL_NJPD_Get_GlobalSetting01()& NJPD_GTABLE_RST)|(NJPD_UV_7BIT | NJPD_UV_MSB));
            }
            else
            {
                HAL_NJPD_Set_GlobalSetting00(HAL_NJPD_Get_GlobalSetting00()&0xf3ff);
                HAL_NJPD_Set_GlobalSetting01((HAL_NJPD_Get_GlobalSetting01()&0xfcff) | NJPD_UV_7BIT | NJPD_UV_MSB);
            }
            break;
    }
}

void HAL_NJPD_SetMTLBMode(MS_U8 u8MTLB)
{
    NJPD_DEBUG_HAL_MSG("%s()\n", __FUNCTION__);
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        // set reg_jpd_miu_tlb
        NJPD_DEBUG_HAL_MSG("set BK_NJPD1_MIU_TLB to 0x%02x\n", u8MTLB);
        __HAL_NJPD_WriteByte(BK_NJPD1_MIU_TLB, u8MTLB);

    }
    else
    {
        // set reg_jpd_miu_tlb
        NJPD_DEBUG_HAL_MSG("set BK_NJPD2_MIU_TLB to 0x%02x\n", u8MTLB);
        __HAL_NJPD_WriteByte(BK_NJPD2_MIU_TLB, u8MTLB);
    }
}

void HAL_NJPD_SetVerificationMode(NJPD_VerificationMode VerificationMode)
{
    NJPD_DEBUG_HAL_MSG("Set Verification Mode to %x\n", VerificationMode);
    eVerificationMode = VerificationMode;
}

NJPD_VerificationMode HAL_NJPD_GetVerificationMode(void)
{
    return eVerificationMode;
}

void HAL_NJPD_Debug(void)
{
    if(eNJPDNum == E_NJPD_NJPD1)
    {
        NJPD_DEBUG_HAL_MSG("read BK_NJPD1_MARB_CRC_RESULT_0: 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MARB_CRC_RESULT_0));
        NJPD_DEBUG_HAL_MSG("read BK_NJPD1_MARB_CRC_RESULT_1: 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MARB_CRC_RESULT_1));
        NJPD_DEBUG_HAL_MSG("read BK_NJPD1_MARB_CRC_RESULT_2: 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MARB_CRC_RESULT_2));
        NJPD_DEBUG_HAL_MSG("read BK_NJPD1_MARB_CRC_RESULT_3: 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MARB_CRC_RESULT_3));
        NJPD_DEBUG_HAL_MSG("read BK_NJPD1_GLOBAL_SETTING00: 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_GLOBAL_SETTING00));
        NJPD_DEBUG_HAL_MSG("read BK_NJPD1_GLOBAL_SETTING01: 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_GLOBAL_SETTING01));
        NJPD_DEBUG_HAL_MSG("read BK_NJPD1_GLOBAL_SETTING02: 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_GLOBAL_SETTING02));
        NJPD_DEBUG_HAL_MSG("read BK_NJPD1_MIU_READ_STATUS = 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_READ_STATUS));
        NJPD_DEBUG_HAL_MSG("read BK_NJPD1_MIU_READ_POINTER_ADDR_L: 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_READ_POINTER_ADDR_L));
        NJPD_DEBUG_HAL_MSG("read BK_NJPD1_MIU_READ_POINTER_ADDR_H: 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD1_MIU_READ_POINTER_ADDR_H));

        MS_U16 u16i;
        for(u16i=0; u16i<=0x7b; u16i++)
        {
            __HAL_NJPD_Write2Byte(BK_NJPD1_DEBUG_BUS_SELECT, u16i);
            NJPD_DEBUG_HAL_MSG("BK_NJPD1_DEBUG_BUS[H:L] = [0x%02x][0x%04x:0x%04x]\n", u16i,
            __HAL_NJPD_Read2Byte(BK_NJPD1_DEBUG_BUS_H),
            __HAL_NJPD_Read2Byte(BK_NJPD1_DEBUG_BUS_L));
        }
        __HAL_NJPD_Write2Byte(BK_NJPD1_DEBUG_BUS_SELECT, 0xFF);
        NJPD_DEBUG_HAL_MSG("BK_NJPD1_DEBUG_BUS[H:L] = [0x%02x][0x%04x:0x%04x]\n", 0xFF,
        __HAL_NJPD_Read2Byte(BK_NJPD1_DEBUG_BUS_H),
        __HAL_NJPD_Read2Byte(BK_NJPD1_DEBUG_BUS_L));

        NJPD_DEBUG_HAL_MSG("=======================================================\n");
        NJPD_DEBUG_HAL_MSG("NJPD  | 00/08 01/09 02/0A 03/0B 04/0C 05/0D 06/0E 07/0F\n");
        NJPD_DEBUG_HAL_MSG("=======================================================\n");
        for(u16i=0; u16i<0x80; u16i+=8)
        {
            NJPD_DEBUG_HAL_MSG("%02x    | %04x  %04x  %04x  %04x  %04x  %04x  %04x  %04x \n",u16i,
            __HAL_NJPD_Read2Byte(BK_NJPD1_GENERAL(u16i)),
            __HAL_NJPD_Read2Byte(BK_NJPD1_GENERAL(u16i+1)),
            __HAL_NJPD_Read2Byte(BK_NJPD1_GENERAL(u16i+2)),
            __HAL_NJPD_Read2Byte(BK_NJPD1_GENERAL(u16i+3)),
            __HAL_NJPD_Read2Byte(BK_NJPD1_GENERAL(u16i+4)),
            __HAL_NJPD_Read2Byte(BK_NJPD1_GENERAL(u16i+5)),
            __HAL_NJPD_Read2Byte(BK_NJPD1_GENERAL(u16i+6)),
            __HAL_NJPD_Read2Byte(BK_NJPD1_GENERAL(u16i+7))
            );
        }
        NJPD_DEBUG_HAL_MSG("=======================================================\n");
    }
    else
    {
        NJPD_DEBUG_HAL_MSG("read BK_NJPD2_MARB_CRC_RESULT_0: 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MARB_CRC_RESULT_0));
        NJPD_DEBUG_HAL_MSG("read BK_NJPD2_MARB_CRC_RESULT_1: 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MARB_CRC_RESULT_1));
        NJPD_DEBUG_HAL_MSG("read BK_NJPD2_MARB_CRC_RESULT_2: 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MARB_CRC_RESULT_2));
        NJPD_DEBUG_HAL_MSG("read BK_NJPD2_MARB_CRC_RESULT_3: 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MARB_CRC_RESULT_3));
        NJPD_DEBUG_HAL_MSG("read BK_NJPD2_GLOBAL_SETTING00: 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_GLOBAL_SETTING00));
        NJPD_DEBUG_HAL_MSG("read BK_NJPD2_GLOBAL_SETTING01: 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_GLOBAL_SETTING01));
        NJPD_DEBUG_HAL_MSG("read BK_NJPD2_GLOBAL_SETTING02: 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_GLOBAL_SETTING02));
        NJPD_DEBUG_HAL_MSG("read BK_NJPD2_MIU_READ_STATUS = 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_READ_STATUS));
        NJPD_DEBUG_HAL_MSG("read BK_NJPD2_MIU_READ_POINTER_ADDR_L: 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_READ_POINTER_ADDR_L));
        NJPD_DEBUG_HAL_MSG("read BK_NJPD2_MIU_READ_POINTER_ADDR_H: 0x%04x\n", __HAL_NJPD_Read2Byte(BK_NJPD2_MIU_READ_POINTER_ADDR_H));

        MS_U16 u16i;
        for(u16i=0; u16i<=0x7b; u16i++)
        {
            __HAL_NJPD_Write2Byte(BK_NJPD2_DEBUG_BUS_SELECT, u16i);
            NJPD_DEBUG_HAL_MSG("BK_NJPD2_DEBUG_BUS[H:L] = [0x%02x][0x%04x:0x%04x]\n", u16i,
            __HAL_NJPD_Read2Byte(BK_NJPD2_DEBUG_BUS_H),
            __HAL_NJPD_Read2Byte(BK_NJPD2_DEBUG_BUS_L));
        }
        __HAL_NJPD_Write2Byte(BK_NJPD2_DEBUG_BUS_SELECT, 0xFF);
        NJPD_DEBUG_HAL_MSG("BK_NJPD2_DEBUG_BUS[H:L] = [0x%02x][0x%04x:0x%04x]\n", 0xFF,
        __HAL_NJPD_Read2Byte(BK_NJPD2_DEBUG_BUS_H),
        __HAL_NJPD_Read2Byte(BK_NJPD2_DEBUG_BUS_L));

        NJPD_DEBUG_HAL_MSG("=======================================================\n");
        NJPD_DEBUG_HAL_MSG("NJPD  | 00/08 01/09 02/0A 03/0B 04/0C 05/0D 06/0E 07/0F\n");
        NJPD_DEBUG_HAL_MSG("=======================================================\n");
        for(u16i=0; u16i<0x80; u16i+=8)
        {
            NJPD_DEBUG_HAL_MSG("%02x    | %04x  %04x  %04x  %04x  %04x  %04x  %04x  %04x \n",u16i,
            __HAL_NJPD_Read2Byte(BK_NJPD2_GENERAL(u16i)),
            __HAL_NJPD_Read2Byte(BK_NJPD2_GENERAL(u16i+1)),
            __HAL_NJPD_Read2Byte(BK_NJPD2_GENERAL(u16i+2)),
            __HAL_NJPD_Read2Byte(BK_NJPD2_GENERAL(u16i+3)),
            __HAL_NJPD_Read2Byte(BK_NJPD2_GENERAL(u16i+4)),
            __HAL_NJPD_Read2Byte(BK_NJPD2_GENERAL(u16i+5)),
            __HAL_NJPD_Read2Byte(BK_NJPD2_GENERAL(u16i+6)),
            __HAL_NJPD_Read2Byte(BK_NJPD2_GENERAL(u16i+7))
            );
        }
        NJPD_DEBUG_HAL_MSG("=======================================================\n");
    }
        MS_U8 u8i;

        NJPD_DEBUG_HAL_MSG("=======================================================\n");
        NJPD_DEBUG_HAL_MSG("MIU0  | 00/08 01/09 02/0A 03/0B 04/0C 05/0D 06/0E 07/0F\n");
        NJPD_DEBUG_HAL_MSG("=======================================================\n");
        for(u8i=0; u8i<0x80; u8i+=8)
        {
            NJPD_DEBUG_HAL_MSG("%02x    | %04x  %04x  %04x  %04x  %04x  %04x  %04x  %04x \n",u8i,
            __HAL_NJPD_Read2Byte(BK_MIU0_GENERAL(u8i)),
            __HAL_NJPD_Read2Byte(BK_MIU0_GENERAL(u8i+1)),
            __HAL_NJPD_Read2Byte(BK_MIU0_GENERAL(u8i+2)),
            __HAL_NJPD_Read2Byte(BK_MIU0_GENERAL(u8i+3)),
            __HAL_NJPD_Read2Byte(BK_MIU0_GENERAL(u8i+4)),
            __HAL_NJPD_Read2Byte(BK_MIU0_GENERAL(u8i+5)),
            __HAL_NJPD_Read2Byte(BK_MIU0_GENERAL(u8i+6)),
            __HAL_NJPD_Read2Byte(BK_MIU0_GENERAL(u8i+7))
            );
        }
        NJPD_DEBUG_HAL_MSG("=======================================================\n");

        NJPD_DEBUG_HAL_MSG("=======================================================\n");
        NJPD_DEBUG_HAL_MSG("MIU1  | 00/08 01/09 02/0A 03/0B 04/0C 05/0D 06/0E 07/0F\n");
        NJPD_DEBUG_HAL_MSG("=======================================================\n");
        for(u8i=0; u8i<0x80; u8i+=8)
        {
            NJPD_DEBUG_HAL_MSG("%02x    | %04x  %04x  %04x  %04x  %04x  %04x  %04x  %04x \n",u8i,
            __HAL_NJPD_Read2Byte(BK_MIU1_GENERAL(u8i)),
            __HAL_NJPD_Read2Byte(BK_MIU1_GENERAL(u8i+1)),
            __HAL_NJPD_Read2Byte(BK_MIU1_GENERAL(u8i+2)),
            __HAL_NJPD_Read2Byte(BK_MIU1_GENERAL(u8i+3)),
            __HAL_NJPD_Read2Byte(BK_MIU1_GENERAL(u8i+4)),
            __HAL_NJPD_Read2Byte(BK_MIU1_GENERAL(u8i+5)),
            __HAL_NJPD_Read2Byte(BK_MIU1_GENERAL(u8i+6)),
            __HAL_NJPD_Read2Byte(BK_MIU1_GENERAL(u8i+7))
            );
        }
        NJPD_DEBUG_HAL_MSG("=======================================================\n");


        NJPD_DEBUG_HAL_MSG("=======================================================\n");
        NJPD_DEBUG_HAL_MSG("1608  | 00/08 01/09 02/0A 03/0B 04/0C 05/0D 06/0E 07/0F\n");
        NJPD_DEBUG_HAL_MSG("=======================================================\n");
        for(u8i=0; u8i<0x80; u8i+=8)
        {
            NJPD_DEBUG_HAL_MSG("%02x    | %04x  %04x  %04x  %04x  %04x  %04x  %04x  %04x \n",u8i,
            __HAL_NJPD_Read2Byte(BK_1608_GENERAL(u8i)),
            __HAL_NJPD_Read2Byte(BK_1608_GENERAL(u8i+1)),
            __HAL_NJPD_Read2Byte(BK_1608_GENERAL(u8i+2)),
            __HAL_NJPD_Read2Byte(BK_1608_GENERAL(u8i+3)),
            __HAL_NJPD_Read2Byte(BK_1608_GENERAL(u8i+4)),
            __HAL_NJPD_Read2Byte(BK_1608_GENERAL(u8i+5)),
            __HAL_NJPD_Read2Byte(BK_1608_GENERAL(u8i+6)),
            __HAL_NJPD_Read2Byte(BK_1608_GENERAL(u8i+7))
            );
        }
        NJPD_DEBUG_HAL_MSG("=======================================================\n");

        NJPD_DEBUG_HAL_MSG("=======================================================\n");
        NJPD_DEBUG_HAL_MSG("160F  | 00/08 01/09 02/0A 03/0B 04/0C 05/0D 06/0E 07/0F\n");
        NJPD_DEBUG_HAL_MSG("=======================================================\n");
        for(u8i=0; u8i<0x80; u8i+=8)
        {
            NJPD_DEBUG_HAL_MSG("%02x    | %04x  %04x  %04x  %04x  %04x  %04x  %04x  %04x \n",u8i,
            __HAL_NJPD_Read2Byte(BK_160F_GENERAL(u8i)),
            __HAL_NJPD_Read2Byte(BK_160F_GENERAL(u8i+1)),
            __HAL_NJPD_Read2Byte(BK_160F_GENERAL(u8i+2)),
            __HAL_NJPD_Read2Byte(BK_160F_GENERAL(u8i+3)),
            __HAL_NJPD_Read2Byte(BK_160F_GENERAL(u8i+4)),
            __HAL_NJPD_Read2Byte(BK_160F_GENERAL(u8i+5)),
            __HAL_NJPD_Read2Byte(BK_160F_GENERAL(u8i+6)),
            __HAL_NJPD_Read2Byte(BK_160F_GENERAL(u8i+7))
            );
        }
        NJPD_DEBUG_HAL_MSG("=======================================================\n");


        NJPD_DEBUG_HAL_MSG("=======================================================\n");
        NJPD_DEBUG_HAL_MSG("1615  | 00/08 01/09 02/0A 03/0B 04/0C 05/0D 06/0E 07/0F\n");
        NJPD_DEBUG_HAL_MSG("=======================================================\n");
        for(u8i=0; u8i<0x80; u8i+=8)
        {
            NJPD_DEBUG_HAL_MSG("%02x    | %04x  %04x  %04x  %04x  %04x  %04x  %04x  %04x \n",u8i,
            __HAL_NJPD_Read2Byte(BK_1615_GENERAL(u8i)),
            __HAL_NJPD_Read2Byte(BK_1615_GENERAL(u8i+1)),
            __HAL_NJPD_Read2Byte(BK_1615_GENERAL(u8i+2)),
            __HAL_NJPD_Read2Byte(BK_1615_GENERAL(u8i+3)),
            __HAL_NJPD_Read2Byte(BK_1615_GENERAL(u8i+4)),
            __HAL_NJPD_Read2Byte(BK_1615_GENERAL(u8i+5)),
            __HAL_NJPD_Read2Byte(BK_1615_GENERAL(u8i+6)),
            __HAL_NJPD_Read2Byte(BK_1615_GENERAL(u8i+7))
            );
        }
        NJPD_DEBUG_HAL_MSG("=======================================================\n");


}

void HAL_NJPD_SetNJPDInstance(JPD_Number JPDNum)
{
    eNJPDNum = JPDNum;
}

MS_BOOL HAL_NJPD_IsNeedToPatch(NJPD_PATCH_INDEX eIndex)
{
    switch(eIndex)
    {
        case E_NJPD_MIU_LAST_Z_PATCH:
            return FALSE;
            break;
        case E_NJPD_EAGLE_SW_PATCH:
            return FALSE;
            break;
        default:
            break;
    }
    return FALSE;
}

MS_U8 HAL_NJPD_JPDCount(void)
{
    return 1;
}
