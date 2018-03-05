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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    halJPD.c
/// @brief  JPD hal interface
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
#include "osalJPD.h"
#include "drvJPD.h"
#include "regJPD.h"
#include "halJPD.h"

#if USE_MJPEG_FUNCTION
#include "../vpu/regVPU.h" // For VPU
#include "../vpu/halVPU.h"
#endif

// MJPEG
#define USE_FW_HVD      1

#if USE_FW_HVD
#include "../hvd/fwHVD_if.h"
#define JPD_FW_VERSION  "JPD.FW.00130062"
MS_BOOL HAL_HVD_LoadCode(MS_U32 u32DestAddr, MS_U32 u32Size, MS_U32 u32BinAddr, MS_U8 u8FwSrcType);
#else
#include "fwJPD.h"
#endif

// For MVD Power On
void HAL_MVD_RegSetBase(MS_U32 u32Base);
void HAL_MVD_PowerCtrl(MS_BOOL bOn);
void HAL_MVD_SetSyncClk(MS_BOOL bEnable);
MS_BOOL HAL_MVD_RstHW(void);

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define HAL_JPD_ENABLE      1
#define HAL_JPD_DISABLE     0

#define HAL_JPD_MUTEX_SUPPORT   HAL_JPD_ENABLE
#define HAL_JPD_MIU_PROTECT     HAL_JPD_ENABLE


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define JPD_EVENT_ALL       0x7F

#define __HAL_JPD_DelayMs(x)      do { MS_U32 ticks=0; while((ticks++)>(x*10)); } while(0)

#define JPD_RIU_MAP u32JPDRiuBaseAdd  //obtain in init

#define JPD_DEBUG_HAL_MSG(format, args...)  do{if(_u8JPDHalDbgLevel & E_JPD_DEBUG_HAL_MSG) printf(format, ##args);}while(0)
#define JPD_DEBUG_HAL_ERR(format, args...)  do{if(_u8JPDHalDbgLevel & E_JPD_DEBUG_HAL_ERR) printf(format, ##args);}while(0)

#ifndef READ_BYTE
#define READ_BYTE(_reg)             (*(volatile MS_U8*)(_reg))
#define READ_WORD(_reg)             (*(volatile MS_U16*)(_reg))
#define READ_LONG(_reg)             (*(volatile MS_U32*)(_reg))
#define WRITE_BYTE(_reg, _val)      { (*((volatile MS_U8*)(_reg))) = (MS_U8)(_val); }
#define WRITE_WORD(_reg, _val)      { (*((volatile MS_U16*)(_reg))) = (MS_U16)(_val); }
#define WRITE_LONG(_reg, _val)      { (*((volatile MS_U32*)(_reg))) = (MS_U32)(_val); }
#endif

#define JPD_READ_BYTE(addr)         READ_BYTE( (JPD_RIU_MAP + (addr)) )
#define JPD_READ_WORD(addr)         READ_WORD( (JPD_RIU_MAP + (addr)) )
#define JPD_WRITE_BYTE(addr, val)   WRITE_BYTE( (JPD_RIU_MAP + (addr)), (val) )
#define JPD_WRITE_WORD(addr, val)   WRITE_WORD( (JPD_RIU_MAP + (addr)), (val) )

#define JPD_MACRO_START     do {
#define JPD_MACRO_END       } while (0)

#define __HAL_JPD_ReadByte( u32Reg )   JPD_READ_BYTE(((u32Reg) << 1) - ((u32Reg) & 1))

#define __HAL_JPD_Read2Byte( u32Reg )    (JPD_READ_WORD((u32Reg)<<1))

#define __HAL_JPD_Read4Byte( u32Reg )   ( (MS_U32)JPD_READ_WORD((u32Reg)<<1) | ((MS_U32)JPD_READ_WORD(((u32Reg)+2)<<1)<<16 ) )

#define __HAL_JPD_ReadBit( u32Reg, u8Mask )   (JPD_READ_BYTE(((u32Reg)<<1) - ((u32Reg) & 1)) & (u8Mask))

#define __HAL_JPD_WriteBit( u32Reg, bEnable, u8Mask ) \
    JPD_MACRO_START \
    JPD_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) , (bEnable) ? (JPD_READ_BYTE(  (((u32Reg) <<1) - ((u32Reg) & 1))  ) |  (u8Mask)) : \
                                (JPD_READ_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) ) & ~(u8Mask))); \
    JPD_MACRO_END

#define __HAL_JPD_WriteByte( u32Reg, u8Val ) \
    JPD_MACRO_START \
    JPD_WRITE_BYTE(((u32Reg) << 1) - ((u32Reg) & 1), (u8Val)); \
    JPD_MACRO_END

#define __HAL_JPD_Write2Byte( u32Reg, u16Val ) \
    JPD_MACRO_START \
    if ( ((u32Reg) & 0x01) ) \
    { \
        JPD_WRITE_BYTE(((u32Reg) << 1) - 1, (MS_U8)((u16Val))); \
        JPD_WRITE_BYTE(((u32Reg) + 1) << 1, (MS_U8)((u16Val) >> 8)); \
    } \
    else \
    { \
        JPD_WRITE_WORD( ((u32Reg)<<1) ,  u16Val); \
    } \
    JPD_MACRO_END

#define __HAL_JPD_Write4Byte( u32Reg, u32Val ) \
    JPD_MACRO_START \
    if ((u32Reg) & 0x01) \
    { \
        JPD_WRITE_BYTE( ((u32Reg) << 1) - 1 ,  (u32Val)); \
        JPD_WRITE_WORD( ((u32Reg) + 1)<<1 , ( (u32Val) >> 8)); \
        JPD_WRITE_BYTE( (((u32Reg) + 3) << 1) ,  ((u32Val) >> 24)); \
    } \
    else \
    { \
        JPD_WRITE_WORD( (u32Reg)<<1 , (u32Val)); \
        JPD_WRITE_WORD(  ((u32Reg) + 2)<<1 ,  ((u32Val) >> 16)); \
    } \
    JPD_MACRO_END

//JPD mutex
#if (HAL_JPD_MUTEX_SUPPORT == HAL_JPD_ENABLE)
static MS_S32 _s32JPDMutexID = -1;
MS_U8 strJPD[10] = "JPD_Mutex";

#define __HAL_JPD_MutexCreate() \
    JPD_MACRO_START \
    if( _s32JPDMutexID < 0 ) \
    { \
        _s32JPDMutexID = OSAL_JPD_MutexCreate( strJPD ); \
    } \
    JPD_MACRO_END
#define __HAL_JPD_MutexDelete() \
    JPD_MACRO_START \
    if( _s32JPDMutexID >= 0 ) \
    { \
        OSAL_JPD_MutexDelete(_s32JPDMutexID); \
        _s32JPDMutexID = -1; \
    } \
    JPD_MACRO_END
#define  __HAL_JPD_MutexEntry() \
    JPD_MACRO_START \
    if( _s32JPDMutexID >= 0 ) \
    { \
        if (!OSAL_JPD_MutexObtain(_s32JPDMutexID, OSAL_JPD_MUTEX_TIMEOUT)) \
        { \
            JPD_DEBUG_HAL_MSG("[HAL JPD][%s][%06d] Mutex taking timeout\n", __FUNCTION__, __LINE__); \
        } \
    } \
    JPD_MACRO_END
#define __HAL_JPD_MutexExit() \
    JPD_MACRO_START \
    if( _s32JPDMutexID >= 0 ) \
    { \
        OSAL_JPD_MutexRelease(_s32JPDMutexID); \
    } \
    JPD_MACRO_END
#else //HAL_JPD_MUTEX_SUPPORT
#define __HAL_JPD_MutexCreate()
#define __HAL_JPD_MutexDelete()
#define __HAL_JPD_MutexEntry()
#define __HAL_JPD_MutexExit()
#endif //HAL_JPD_MUTEX_SUPPORT

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_U32 u32JPDRiuBaseAdd = 0x0;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_U8 _pu8HalVer[] = JPD_HAL_VERSION;
static MS_U8 _pu8FwVer[] = JPD_FW_VERSION;
static MS_U8 _u8JPDHalDbgLevel = E_JPD_DEBUG_HAL_NONE;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _HAL_JPD_SetMIUProtectMask(MS_BOOL bEnable)
{
#if (HAL_JPD_MIU_PROTECT == HAL_JPD_ENABLE)
    __HAL_JPD_WriteBit(JPD_MIU0_RQ2_MASK+1, bEnable, JPD_MIU0_CLIENT_JPD);
    __HAL_JPD_WriteBit(JPD_MIU1_RQ2_MASK+1, bEnable, JPD_MIU1_CLIENT_JPD);

    JPD_DEBUG_HAL_MSG("_HAL_JPD_SetMIUProtectMask MIU0 : 0x%04X\n", __HAL_JPD_Read2Byte(JPD_MIU0_RQ2_MASK));
    JPD_DEBUG_HAL_MSG("_HAL_JPD_SetMIUProtectMask MIU1 : 0x%04X\n", __HAL_JPD_Read2Byte(JPD_MIU1_RQ2_MASK));
#endif
    return;
}

#if 0
MS_U8 _JPD_ReadByte(MS_U16 u16Reg)
{
    if ((u16Reg % 2) == 0) // low byte
    {
        return _LOWBYTE(_RIU_REG16(JPD_RIU_MAP+((u16Reg)*2)));
    }
    else // high byte
    {
        u16Reg -= 1;
        return _HIGHBYTE(_RIU_REG16(JPD_RIU_MAP+((u16Reg)*2)));
    }
}

void _JPD_WriteByte(MS_U16 u16Reg, MS_U8 u8Value)
{
    if ((u16Reg % 2) == 0) // low byte
    {
        _RIU_REG16(JPD_RIU_MAP+((u16Reg)*2))
            =  u8Value | (_RIU_REG16(JPD_RIU_MAP+((u16Reg)*2))&0xFF00);
    }
    else // high byte
    {
        u16Reg -= 1 ;
        _RIU_REG16(JPD_RIU_MAP+((u16Reg)*2))
            =  (u8Value<<8) | (_RIU_REG16(JPD_RIU_MAP+((u16Reg)*2))&0x00FF);
    }
}

MS_U16 _JPD_Read2Byte(MS_U16 u16Reg)
{
    MS_U16 u16RegData;

    if ((u16Reg % 2) == 0) // low byte
    {
       u16RegData = _RIU_REG16(JPD_RIU_MAP+((u16Reg)*2));
    }
    else // high byte
    {
       //0x2F05
        u16Reg = u16Reg - 1;
        u16RegData = _HIGHBYTE(_RIU_REG16(JPD_RIU_MAP+((u16Reg)*2)));
        u16Reg = u16Reg + 2;
        u16RegData |= _LOWBYTE(_RIU_REG16(JPD_RIU_MAP+((u16Reg)*2)));
    }

    return (u16RegData);
}

void _JPD_Write2Byte(MS_U16 u16Reg, MS_U16 u16Value)
{
    if ((u16Reg % 2) == 0) // low byte
    {
        _RIU_REG16(JPD_RIU_MAP + ((u16Reg)*2)) = u16Value;
    }
    else // high byte
    {
        _JPD_WriteByte(u16Reg, (MS_U8)(u16Value&0x00FF));
        _JPD_WriteByte(u16Reg+1, (MS_U8)(u16Value>>8));
    }
}

MS_U32 _JPD_Read4Byte(MS_U16 u16Reg)
{
    MS_U32 u32RegData = 0;

    if ((u16Reg % 2) == 0) // low byte
    {
        u32RegData |= _JPD_Read2Byte(u16Reg);
        u32RegData |= (_JPD_Read2Byte(u16Reg+2) << 16);
    }
    else // high byte
    {
        u32RegData |= _JPD_ReadByte(u16Reg);
        u32RegData |= (_JPD_Read2Byte(u16Reg+1) << 8);
        u32RegData |= (_JPD_ReadByte(u16Reg+3) << 24);
    }

    return (u32RegData);
}

void _JPD_Write4Byte(MS_U16 u16Reg, MS_U32 u32Value)
{
    if ((u16Reg % 2) == 0) // low byte
    {
        _JPD_Write2Byte(u16Reg, (MS_U16)u32Value);
        _JPD_Write2Byte(u16Reg+2, (MS_U16)(u32Value>>16));
    }
    else // high byte
    {
        _JPD_WriteByte(u16Reg, (MS_U8)(u32Value));
        _JPD_Write2Byte(u16Reg+1, (MS_U16)(u32Value>>8));
        _JPD_WriteByte(u16Reg+3, (MS_U8)(u32Value>>24));
    }
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void __HAL_JPD_Write4Byte(MS_U16 u16Reg, MS_U32 u32Value)
{
    _JPD_Write4Byte(u16Reg, u32Value);

    if(_u8JPDHalDbgLevel & E_JPD_DEBUG_HAL_REG)
    {
        MS_U32 read_value = _JPD_Read4Byte(u16Reg);
        if(u32Value != read_value)
            printf("Write Four Byte Failed!! Reg = 0x%04X, write_value = 0x%lX, read_value = 0x%lX\n", u16Reg, u32Value, read_value);
    }
//    WRITE_LONG(u16Reg, u32Value);
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U32 __HAL_JPD_Read4Byte(MS_U16 u16Reg)
{
    return _JPD_Read4Byte(u16Reg);
//    return READ_LONG(u16Reg);
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void __HAL_JPD_Write2Byte(MS_U16 u16Reg, MS_U16 u16Value)
{
    _JPD_Write2Byte(u16Reg, u16Value);

    if(_u8JPDHalDbgLevel & E_JPD_DEBUG_HAL_REG)
    {
        MS_U16 read_value = _JPD_Read2Byte(u16Reg);
        if(u16Value != read_value)
            printf("Write Two Byte Failed!! Reg = 0x%04X, write_value = 0x%04X, read_value = 0x%04X\n", u16Reg, u16Value, read_value);
    }
//    WRITE_WORD(u16Reg, u16Value);
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U16 __HAL_JPD_Read2Byte(MS_U16 u16Reg)
{
    return _JPD_Read2Byte(u16Reg);
//    return READ_WORD(u16Reg);
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void __HAL_JPD_WriteByte(MS_U16 u16Reg, MS_U8 u8Value)
{
    _JPD_WriteByte(u16Reg, u8Value);

    if(_u8JPDHalDbgLevel & E_JPD_DEBUG_HAL_REG)
    {
        MS_U8 read_value = _JPD_ReadByte(u16Reg);
        if(u8Value != read_value)
            printf("Write Two Byte Failed!! Reg = 0x%04X, write_value = 0x%04X, read_value = 0x%04X\n", u16Reg, u8Value, read_value);
    }
//    WRITE_BYTE(u16Reg, u8Value);
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U8 __HAL_JPD_ReadByte(MS_U16 u16Reg)
{
    return _JPD_ReadByte(u16Reg);
//    return READ_BYTE(u16Reg);
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void __HAL_JPD_WriteBit(MS_U16 u16Reg, MS_BOOL status, MS_U8 u8Bit)
{
    MS_U8 u8Tmp = __HAL_JPD_ReadByte(u16Reg);
    if (status)
        u8Tmp |= u8Bit;
    else
        u8Tmp &= (~u8Bit);
    __HAL_JPD_WriteByte(u16Reg, u8Tmp);
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U8 __HAL_JPD_ReadBit(MS_U16 u16Reg, MS_U8 u8Bit)
{
    MS_U8 u8Tmp = __HAL_JPD_ReadByte(u16Reg);
    return (u8Tmp & u8Bit);
}
#endif


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_SetMIU(MS_U8 u8Idx)
{
#if (ENABLE_TEST_18_miu_sel_128M==true) \
    ||(ENABLE_TEST_18_miu_sel_64M==true) \
    ||(ENABLE_TEST_18_miu_sel_32M==true)
    // This code is reserved for verification code TEST_18_miu_sel
    // The value of BK_JPD_MCONFIG+1 would be modified at JPEG_StartDecode
    // So we need to modify there too.
    __HAL_JPD_WriteBit(JPD_MIU1_MIU_SEL2+1, 1,JPD_MIU1_CLIENT_JPD);
//        __HAL_JPD_WriteBit(BK_JPD_MCONFIG+1, 0, JPD_MIU_SEL);     // 0 1
    #if (ENABLE_TEST_18_miu_sel_128M==true)
        __HAL_JPD_WriteBit(BK_JPD_MCONFIG+1, 1, JPD_MIU_SEL);     // 0 1 0 1
    #elif (ENABLE_TEST_18_miu_sel_64M==true)
        __HAL_JPD_WriteBit(BK_JPD_MCONFIG+1, 2, JPD_MIU_SEL);     // 0 1 0 1 0 1 0 1
    #else
        __HAL_JPD_WriteBit(BK_JPD_MCONFIG+1, 3, JPD_MIU_SEL);     //  0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1
    #endif
#else
    if(0 == __HAL_JPD_ReadBit(JPD_MIU1_MIU_SEL2+1, JPD_MIU1_CLIENT_JPD))
    {
        if(u8Idx)
        {
            __HAL_JPD_WriteBit(JPD_MIU0_MIU_SEL2+1, 1, JPD_MIU0_CLIENT_JPD);
            JPD_DEBUG_HAL_MSG("HAL_JPD_SetMIU : JPD_MIU0_MIU_SEL2 : 0x%04X\n", __HAL_JPD_Read2Byte(JPD_MIU0_MIU_SEL2));
        }
        else
        {
            __HAL_JPD_WriteBit(JPD_MIU0_MIU_SEL2+1, 0, JPD_MIU0_CLIENT_JPD);
            JPD_DEBUG_HAL_MSG("HAL_JPD_SetMIU : JPD_MIU0_MIU_SEL2 : 0x%04X\n", __HAL_JPD_Read2Byte(JPD_MIU0_MIU_SEL2));
        }
    }
    else
    {
        __HAL_JPD_WriteBit(BK_JPD_MCONFIG+1, 0, JPD_MIU_SEL);
        JPD_DEBUG_HAL_MSG("HAL_JPD_SetMIU : miu_hw_msel_en is active, BK_JPD_MCONFIG : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_MCONFIG));
    }

#endif
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_Set_S_Config(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_Set_S_Config : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_SCONFIG, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_Get_S_Config : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_SCONFIG));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U16 HAL_JPD_Get_S_Config(void)
{
    return __HAL_JPD_Read2Byte(BK_JPD_SCONFIG);
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_Set_M_Config(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_Set_M_Config : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_MCONFIG, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_Get_M_Config : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_MCONFIG));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U16 HAL_JPD_Get_M_Config(void)
{
    return __HAL_JPD_Read2Byte(BK_JPD_MCONFIG);
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_SetIntEn(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetIntEn : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_INTEN, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_GetIntEn : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_INTEN));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_SetMRCBufFloorLow(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetMRCBufFloorLow : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_RBUF_FLOOR_L, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_GetMRCBufFloorLow : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_RBUF_FLOOR_L));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_SetMRCBufFloorHigh(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetMRCBufFloorHigh : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_RBUF_FLOOR_H, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_GetMRCBufFloorHigh : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_RBUF_FLOOR_H));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_SetMRCBufCeilLow(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetMRCBufCeilLow : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_RBUF_CEIL_L, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_GetMRCBufCeilLow : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_RBUF_CEIL_L));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_SetMRCBufCeilHigh(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetMRCBufCeilHigh : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_RBUF_CEIL_H, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_GetMRCBufCeilHigh : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_RBUF_CEIL_H));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_SetRCSMAddrLow(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetRCSMAddrLow : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_RCSMADR_L, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_GetRCSMAddrLow : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_RCSMADR_L));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_SetRCSMAddrHigh(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetRCSMAddrHigh : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_RCSMADR_H, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_GetRCSMAddrHigh : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_RCSMADR_H));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_SetMWCBufStAddrLow(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetMWCBufStAddrLow : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_MWBF_SADR_L, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_GetMWCBufStAddrLow : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_MWBF_SADR_L));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_SetMWCBufStAddrHigh(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetMWCBufStAddrHigh : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_MWBF_SADR_H, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_GetMWCBufStAddrHigh : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_MWBF_SADR_H));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_SetPic_H(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetPic_H : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_PIC_H, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_GetPic_H : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_PIC_H));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_SetPic_V(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetPic_V : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_PIC_V, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_GetPic_V : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_PIC_V));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_SetEventFlag(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetEventFlag : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_EVENTFLAG, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_GetEventFlag : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_EVENTFLAG));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U16 HAL_JPD_GetEventFlag(void)
{
    return __HAL_JPD_Read2Byte(BK_JPD_EVENTFLAG);
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_SetROI_H(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetROI_H : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_ROI_H, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_GetROI_H : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_ROI_H));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_SetROI_V(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetROI_V : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_ROI_V, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_GetROI_V : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_ROI_V));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_SetROIWidth(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetROIWidth : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_ROI_WIDTH, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_GetROIWidth : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_ROI_WIDTH));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_SetROIHeight(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetROIHeight : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_ROI_HEIGHT, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_GetROIHeight : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_ROI_HEIGHT));
}

#if 0 // Unused function
/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U16 HAL_JPD_GetCurMadrLow(void)
{
    return __HAL_JPD_Read2Byte(BK_JPD_CUR_MADR_L);
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U16 HAL_JPD_GetCurMadrHigh(void)
{
    return __HAL_JPD_Read2Byte(BK_JPD_CUR_MADR_H);
}
#endif

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_SetClock(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetClock : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(JPD_CLOCK, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_GetClock : 0x%04X\n", __HAL_JPD_Read2Byte(JPD_CLOCK));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U16 HAL_JPD_GetClock(void)
{
    return __HAL_JPD_Read2Byte(JPD_CLOCK);
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_PowerOn(void)
{
    MS_U16 u16reg_val;
    //u16reg_val = MDrv_Read2Byte(_u16JPDClock_Addr);
    u16reg_val = __HAL_JPD_Read2Byte(JPD_CLOCK);
    u16reg_val = u16reg_val & 0xFFF2;
    u16reg_val |= 0x000C;  // set JPD disable [8] : 0, [3:2] : 11 192MHz
    //MDrv_Write2Byte(_u16JPDClock_Addr, reg_val);
    JPD_DEBUG_HAL_MSG("HAL_JPD_PowerOn : SetClock : 0x%04X\n", u16reg_val);
    __HAL_JPD_Write2Byte(JPD_CLOCK, u16reg_val);
    JPD_DEBUG_HAL_MSG("HAL_JPD_PowerOn : GetClock : 0x%04X\n", __HAL_JPD_Read2Byte(JPD_CLOCK));


    #if 0 // DEBUG
    printf ("   TSP STC : = 0x%lx\n", (MS_U32)(__HAL_JPD_Read2Byte(REG_TSP_STC_L) | (__HAL_JPD_Read2Byte(REG_TSP_STC_H) << 16))/90);
    #endif
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_PowerOff(void)
{
    MS_U16 u16reg_val;
    //u16reg_val = MDrv_Read2Byte(_u16JPDClock_Addr);
    u16reg_val = __HAL_JPD_Read2Byte(JPD_CLOCK);
    u16reg_val = u16reg_val & 0xFFF2;
    u16reg_val |= 0x000D;  // set JPD disable [8] : 1, [3:2] : 11 192MHz
    //MDrv_Write2Byte(_u16JPDClock_Addr, reg_val);
    JPD_DEBUG_HAL_MSG("HAL_JPD_PowerOff : SetClock : 0x%04X\n", u16reg_val);
    __HAL_JPD_Write2Byte(JPD_CLOCK, u16reg_val);
    JPD_DEBUG_HAL_MSG("HAL_JPD_PowerOff : GetClock : 0x%04X\n", __HAL_JPD_Read2Byte(JPD_CLOCK));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_InitRegBase(void)
{
    MS_U32 u32NonPMBankSize;
    if(!MDrv_MMIO_GetBASE(&u32JPDRiuBaseAdd, &u32NonPMBankSize, MS_MODULE_JPD))
    {
        JPD_DEBUG_HAL_ERR("Get RIUreg base Failed!!!\n");
    }
    else
    {
        JPD_DEBUG_HAL_MSG("RIUreg base = 0x%lX, length = %lu\n", u32JPDRiuBaseAdd, u32NonPMBankSize);
    }
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return none
/******************************************************************************/
void HAL_JPD_CreateMutex(void)
{
    __HAL_JPD_MutexCreate();
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return none
/******************************************************************************/
void HAL_JPD_DeleteMutex(void)
{
    __HAL_JPD_MutexDelete();
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_SetRSTIntv(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetRSTIntv : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_RSTINTV, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_GetRSTIntv : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_RSTINTV));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U16 HAL_JPD_GetCurVidx(void)
{
    return __HAL_JPD_Read2Byte(BK_JPD_CUR_VIDX);
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_Set_TID_Addr(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_Set_TID_Addr : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_TID_ADR, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_Get_TID_Addr : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_TID_ADR));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_Set_TID_Dat(MS_U16 u16Value)
{
    __HAL_JPD_Write2Byte(BK_JPD_TID_DAT, u16Value);
}

MS_U16 HAL_JPD_Get_TID_Dat(void)
{
    return __HAL_JPD_Read2Byte(BK_JPD_TID_DAT);
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return none
/******************************************************************************/
void HAL_JPD_SetDbgLevel(MS_U8 u8DbgLevel)
{
    _u8JPDHalDbgLevel = u8DbgLevel;
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return none
/******************************************************************************/
void HAL_JPD_GetLibVer(JPD_HAL_Version *pHalVer)
{
    pHalVer->pu8HalVer = _pu8HalVer;
    pHalVer->pu8FwVer = _pu8FwVer;
}

/******************************************************************************/
///Reset JPD
///@param value \b IN
///@param value \b OUT
///@return none
/******************************************************************************/
void HAL_JPD_Rst(void)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_Rst : start!!\n");
    _HAL_JPD_SetMIUProtectMask(TRUE);
    //mif reset is high active
    HAL_JPD_Set_M_Config(HAL_JPD_Get_M_Config() | JPD_MIF_RST);
    HAL_JPD_Set_M_Config(HAL_JPD_Get_M_Config() & ~JPD_MIF_RST);
    // reset event flag
    __HAL_JPD_Write2Byte(BK_JPD_EVENTFLAG, JPD_EVENT_ALL);
    // reset : low active
    __HAL_JPD_Write2Byte(BK_JPD_SCONFIG, 0);
    __HAL_JPD_Write2Byte(BK_JPD_SCONFIG, JPD_SWRST);
    __HAL_JPD_Write2Byte(BK_JPD_INTEN, JPD_EVENT_ALL);
    JPD_DEBUG_HAL_MSG("Get S-config : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_SCONFIG));
    JPD_DEBUG_HAL_MSG("Get M-config : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_MCONFIG));
    JPD_DEBUG_HAL_MSG("HAL Get INTEN : 0x%04X\n",  __HAL_JPD_Read2Byte(BK_JPD_INTEN));
    JPD_DEBUG_HAL_MSG("HAL Get EVENTFLAG : 0x%04X\n",  __HAL_JPD_Read2Byte(BK_JPD_EVENTFLAG));
    _HAL_JPD_SetMIUProtectMask(FALSE);
    JPD_DEBUG_HAL_MSG("HAL_JPD_Rst : end!!\n");
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return none
/******************************************************************************/
void HAL_JPD_SetMWBuffLineNum(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetMWBuffLineNum : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_MWBF_LINE_NUM, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_Get_MWBF_LINE_NUM : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_MWBF_LINE_NUM));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U32 HAL_JPD_GetCurMRCAddr(void)
{
    MS_U32 curMRCAddr = ((__HAL_JPD_Read2Byte(BK_JPD_CUR_MADR_H) << 16) | __HAL_JPD_Read2Byte(BK_JPD_CUR_MADR_L));

    return curMRCAddr;
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U16 HAL_JPD_GetCurRow(void)
{
    return __HAL_JPD_Read2Byte(BK_JPD_CUR_ROWP);
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U16 HAL_JPD_GetCurCol(void)
{
    return __HAL_JPD_Read2Byte(BK_JPD_CUR_CLNP);
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return none
/******************************************************************************/
void HAL_JPD_SetWriteProtect(MS_BOOL enable)
{
    __HAL_JPD_WriteBit(BK_JPD_MCONFIG+1, enable, JPD_MWC_WPEN);
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetWriteProtect : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_MCONFIG));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
void HAL_JPD_SetSpare(MS_U16 u16Value)
{
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetSpare : 0x%04X\n", u16Value);
    __HAL_JPD_Write2Byte(BK_JPD_SPARE, u16Value);
    JPD_DEBUG_HAL_MSG("HAL_JPD_SetSpare : 0x%04X\n", __HAL_JPD_Read2Byte(BK_JPD_SPARE));
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U16 HAL_JPD_GetSpare(void)
{
    return __HAL_JPD_Read2Byte(BK_JPD_SPARE);
}

#if USE_MJPEG_FUNCTION
/*================================ MJPEG =====================================*/

/******************************************************************************/
///Stop VPU
/******************************************************************************/
void HAL_JPD_StopVPU(void)
{
    //HAL_VPU_PowerCtrl(FALSE);
    HAL_VPU_DeInit();
}

/******************************************************************************/
///Reset VPU
///@return success or fail
/******************************************************************************/
MS_BOOL HAL_JPD_RstVPU(void)
{
#if 1
    HAL_VPU_InitRegBase(u32JPDRiuBaseAdd);

    return HAL_VPU_SwRst();
#else

    MS_U16  u16Timeout = 1000;

    #ifdef MSOS_TYPE_ECOS
    __HAL_JPD_WriteByte(REG_CKG_SVD, (MS_U8)CKG_SVD_216MHZ);
    #elif defined(MSOS_TYPE_LINUX)
    __HAL_JPD_WriteByte(REG_CKG_SVD, (MS_U8)CKG_SVD_216MHZ);
    #else // NON-OS
    __HAL_JPD_WriteByte(REG_CKG_SVD, (MS_U8)CKG_SVD_240MHZ);
    #endif
    // T2 SVD RESET
    __HAL_JPD_WriteByte(REG_SVD_RESET, REG_SVD_RESET_SWRST | REG_SVD_RESET_CPURST);
    __HAL_JPD_DelayMs(1);

    // Wait for reset done
    while(u16Timeout)
    {
        if((__HAL_JPD_ReadByte(REG_SVD_RESET) & (REG_SVD_RESET_SWRST_FIN|REG_SVD_RESET_CPURST_FIN))
           == (REG_SVD_RESET_SWRST_FIN|REG_SVD_RESET_CPURST_FIN))
        {
            break;
        }
        u16Timeout--;
    }

    if(!u16Timeout) JPD_DEBUG_HAL_ERR("    HAL_JPD_RstVPU timeout\n");

    return (u16Timeout>0) ? TRUE : FALSE;
#endif
}

/******************************************************************************/
///Release VPU
/******************************************************************************/
void HAL_JPD_ReleaseVPU(void)
{
#if 1
    HAL_VPU_SwRstRelse();
#else
    // Release REG_SVD_RESET_CPURST first
    __HAL_JPD_WriteBit(REG_SVD_RESET, FALSE, REG_SVD_RESET_CPURST);
    __HAL_JPD_DelayMs(1);

    // Release REG_SVD_RESET_SWRST
    __HAL_JPD_WriteBit(REG_SVD_RESET, FALSE, REG_SVD_RESET_SWRST);
    __HAL_JPD_DelayMs(1);
#endif
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_BOOL HAL_JPD_IsVPUOnMiu1(void)
{
    if (__HAL_JPD_ReadByte(0x12F4) & JPD_BIT(6))
        return TRUE;
    return FALSE;
}

/******************************************************************************/
///
///@param value \b IN
///@param value \b OUT
///@return status
/******************************************************************************/
MS_U32 HAL_JPD_GetMiuBase(void)
{
    if (HAL_JPD_IsVPUOnMiu1())
        return HAL_MIU1_BASE;

    return 0x00000000;
}

extern MS_BOOL gbEnableTurboFWMode;
/******************************************************************************/
///Load JPD Firmware for MJPEG
///@param u32Addr \b IN address to load firmware
///@return success or fail
/******************************************************************************/
MS_BOOL HAL_JPD_LoadCode(MS_PHYADDR u32Addr, MS_U32 u32Size, MS_PHYADDR u32BinAddr, MS_U8 u8FwSrcType)
{
    MS_BOOL bNeedReloadFW = TRUE;

    //Check whether need to reload fw or not
    if((TRUE == gbEnableTurboFWMode)
    && (FALSE == HAL_VPU_IsNeedReload(E_VPU_DECODER_MJPEG)))
    {
        bNeedReloadFW = FALSE;
    }

    if(TRUE == bNeedReloadFW)
    {
#if 0
        // VPU (available after T3)
#else
#if (USE_FW_HVD == 0)
        MS_U32 u32CodeSize;
#endif
//    MS_U8  u8BDMAFlag;
        MS_U32 u32MiuBase = 0xFFFFFFFF;

        //If we need to reload fw, need to reset vpu fw decoder type first.
        HAL_VPU_SetFWDecoder(E_VPU_DECODER_NONE);

        //if (u32Addr != 0)
        if ((u32Addr & 0x7FF) != 0)
        {
            //JPD_DEBUG_HAL_ERR ("   JPD Firmware load code address is not 0x00000000\n");
            JPD_DEBUG_HAL_ERR ("   JPD Firmware load code address must be 2048 byte alignment!\n");
            return FALSE;
        }
#if USE_FW_HVD
        JPD_DEBUG_HAL_MSG ("   JPD Firmware load code address = 0x%lx\n", u32Addr);
#else
        u32CodeSize = sizeof(u8FwJPD);
        JPD_DEBUG_HAL_MSG ("   JPD Firmware load code address = 0x%lx\n", u32Addr);
        JPD_DEBUG_HAL_MSG ("   JPD Firmware size = 0x%lx\n", u32CodeSize);
#endif

        #if 0 // Need to check if BDMA is ready or not
        // Use BDMA to copy firmware to correct address
        u8BDMAFlag = BDMACOPY_SRCMIU0 | BDMACOPY_ADDR_INC | BDMACOPY_CH1;
        // MIU judgement
        if (__HAL_JPD_ReadByte(0x12F4) & JPD_BIT(6))
        {
            u8BDMAFlag |= BDMACOPY_DSTMIU1;
            JPD_DEBUG_HAL_MSG ("   VPU on MIU 1\n");
        }
        else
        {
            u8BDMAFlag |= BDMACOPY_DSTMIU0;
            JPD_DEBUG_HAL_MSG ("   VPU on MIU 0\n");
        }
        MDrv_BDMA_COPY((MS_U32)&u8FwJPD[0], u32Addr, u32CodeSize, u8BDMAFlag);
        #else

        #if 0
        if (HAL_JPD_IsVPUOnMiu1())
        {
            JPD_DEBUG_HAL_MSG ("   VPU on MIU 1\n");
            u32Addr |= HAL_MIU1_BASE;
        }
        else
        {
            JPD_DEBUG_HAL_MSG ("   VPU on MIU 0\n");
        }
        #endif
        u32MiuBase = u32Addr & HAL_MIU1_BASE;
        if (u32MiuBase)
        {
            JPD_DEBUG_HAL_MSG ("   VPU on MIU 1\n");
        }
        else
        {
            JPD_DEBUG_HAL_MSG ("   VPU on MIU 0\n");
        }

        u32Addr = MS_PA2KSEG1((MS_U32)u32Addr); //PhysicalAddr to VirtualAddr
        u32BinAddr = MS_PA2KSEG1((MS_U32)u32BinAddr); //PhysicalAddr to VirtualAddr
        JPD_DEBUG_HAL_MSG ("   Load JPD Firmware to 0x%lx\n", u32Addr);

#if USE_FW_HVD
        if (HAL_HVD_LoadCode(u32Addr, u32Size, u32BinAddr, u8FwSrcType) == FALSE)
        {
            JPD_DEBUG_HAL_ERR ("\n    !!!Load Firmware Fail!!!\n\n");
            return FALSE;
        }

        // Set Codec Type for HVD Firmware
        {
            volatile HVD_ShareMem *pHVDShareMem = NULL;
            pHVDShareMem = (HVD_ShareMem *)(u32Addr + HVD_SHARE_MEM_ST_OFFSET);
            pHVDShareMem->u32CodecType = E_HVD_Codec_MJPEG;
        }
#else
        {
            MS_U32 ix;
            MS_U8 *pu8Fw = (MS_U8 *)(u32Addr);
            for (ix = 0; ix < u32CodeSize; ix++)
            {
                pu8Fw[ix] = u8FwJPD[ix];
            }
        }
#endif
        #if 0
        {
            MS_U32 ix;
            MS_U8 *pu8Fw = (MS_U8 *)(u32Addr);
            for (ix = 0; ix < u32CodeSize; ix++)
            {
                if (pu8Fw[ix] != u8FwJPD[ix])
                {
                    printf ("\n     !!! Firmware data copy ERROR !!!\n");
                    return FALSE;
                }
            }
        }
        #endif
        #endif
#endif
        //When complete loading fw, set vpu fw decoder type
        HAL_VPU_SetFWDecoder(E_VPU_DECODER_MJPEG);
    }

    return TRUE;
}

/******************************************************************************/
///Config VPU
///@param u32StAddr \b IN: CPU binary code base address in DRAM.
///@param u8dlend_en \b IN: endian
///     - 1, little endian
///     - 0, big endian
/******************************************************************************/
void HAL_JPD_ConfigVPU(MS_PHYADDR u32StAddr, MS_U8 u8dlend_en)
{
#if 1
    HAL_VPU_CPUSetting(u32StAddr);

    // MVD must be powered on to let vsync bypass to VPU
    HAL_MVD_RegSetBase(u32JPDRiuBaseAdd);
    //Set MVD Clock @ reg_CHIPTOP
    HAL_MVD_PowerCtrl(ENABLE);
    //Set MVD Clock aync
//    HAL_MVD_SetSyncClk(DISABLE);
    //MVD reset
//    HAL_MVD_RstHW();


    // Enable TSP for STC
    __HAL_JPD_Write2Byte(REG_TSP_CTRL, __HAL_JPD_Read2Byte(REG_TSP_CTRL) | REG_TSP_CPU_ENABLE | REG_TSP_SW_RSTZ);
    JPD_DEBUG_HAL_MSG("   Enable TSP for STC : [0x15F4] = 0x%x\n", __HAL_JPD_Read2Byte(REG_TSP_CTRL));

#else
    // get real MIU address
    u32StAddr = u32StAddr & JPD_MIU_ADDR_MASK;

    JPD_DEBUG_HAL_MSG ("   Set firmware address 0x%lx\n", u32StAddr);

    // code base address
    __HAL_JPD_Write4Byte(REG_SVD_CPU_SDR_BASE_L, u32StAddr >> 4);
    // data offset
    __HAL_JPD_Write4Byte(REG_SVD_MIU_OFFSET_H0, u32StAddr >> 11);

    __HAL_JPD_WriteByte(REG_SVD_LDEND_EN, (u8dlend_en & 0x01));
#endif
}


/******************************************************************************/
///Check VPU MailBox ready or not
///@return TRUE or FALSE
///    - TRUE, MailBox is free
///    - FALSE, MailBox is busy
///@param u8MBox \b IN: MailBox to check
///     - VPU_HI_MBOX0,
///     - VPU_HI_MBOX1,
///     - VPU_RISC_MBOX0,
///     - VPU_RISC_MBOX1,
/******************************************************************************/
MS_BOOL HAL_JPD_IsVPUMBoxReady(MS_U8 u8MBox)
{
    MS_BOOL bResult = FALSE;
    switch(u8MBox)
    {
        case VPU_HI_MBOX0:
            bResult = (__HAL_JPD_Read2Byte(VPU_REG_HI_MBOX_RDY) & VPU_REG_HI_MBOX0_RDY) ? FALSE : TRUE;
            break;
        case VPU_HI_MBOX1:
            bResult = (__HAL_JPD_Read2Byte(VPU_REG_HI_MBOX_RDY) & VPU_REG_HI_MBOX1_RDY) ? FALSE : TRUE;
            break;
        case VPU_RISC_MBOX0:
            bResult = (__HAL_JPD_Read2Byte(VPU_REG_RISC_MBOX_RDY) & VPU_REG_RISC_MBOX0_RDY) ? TRUE: FALSE;
            break;
        case VPU_RISC_MBOX1:
            bResult = (__HAL_JPD_Read2Byte(VPU_REG_RISC_MBOX_RDY) & VPU_REG_RISC_MBOX1_RDY) ? TRUE: FALSE;
            break;
        default:
            break;
    }
    return bResult;
}

/******************************************************************************/
///Read message from VPU
///@return TRUE or FALSE
///    - TRUE, success
///    - FALSE, failed
///@param u8MBox \b IN: MailBox to read
///    - VPU_RISC_MBOX0
///    - VPU_RISC_MBOX1
///@param u32Msg \b OUT: message read
/******************************************************************************/
MS_BOOL HAL_JPD_ReadVPUMBox(MS_U8 u8MBox, MS_U32 *u32Msg)
{
    MS_BOOL bResult = TRUE;
    switch(u8MBox)
    {
        case VPU_RISC_MBOX0:
            *u32Msg = ((MS_U32)(__HAL_JPD_Read2Byte(VPU_REG_RISC_MBOX0_H)) << 16) |
                      ((MS_U32)(__HAL_JPD_Read2Byte(VPU_REG_RISC_MBOX0_L)));
            break;
        case VPU_RISC_MBOX1:
            *u32Msg = ((MS_U32)(__HAL_JPD_Read2Byte(VPU_REG_RISC_MBOX1_H)) << 16) |
                      ((MS_U32)(__HAL_JPD_Read2Byte(VPU_REG_RISC_MBOX1_L)));
            //__HAL_JPD_WriteBit(VPU_REG_RISC_MBOX_CLR, FALSE, VPU_REG_RISC_MBOX1_CLR);
            break;
        default:
            bResult = FALSE;
            break;
    }
    return bResult;
}

///-----------------------------------------------------------------------------
/// Send message to VPU
/// @return TRUE or FALSE
///     - TRUE, Success
///     - FALSE, Failed
/// @param u8MBox \b IN: MailBox
///     - VPU_HI_MBOX0,
///     - VPU_HI_MBOX1,
///-----------------------------------------------------------------------------
MS_BOOL HAL_JPD_SendVPUMBox(MS_U8 u8MBox, MS_U32 u32Msg)
{
    MS_BOOL bResult = TRUE;
    switch(u8MBox)
    {
        case VPU_HI_MBOX0:
            __HAL_JPD_Write4Byte(VPU_REG_HI_MBOX0_L, u32Msg);
            __HAL_JPD_WriteBit(VPU_REG_HI_MBOX_SET, TRUE, VPU_REG_HI_MBOX0_SET);
            break;
        case VPU_HI_MBOX1:
            __HAL_JPD_Write4Byte(VPU_REG_HI_MBOX1_L, u32Msg);
            __HAL_JPD_WriteBit(VPU_REG_HI_MBOX_SET, TRUE, VPU_REG_HI_MBOX1_SET);
            break;
        default:
            bResult = FALSE;
            break;
    }
    return bResult;
}

///-----------------------------------------------------------------------------
/// Clear VPU RISC MBox
/// @return TRUE or FALSE
///     - TRUE, Success
///     - FALSE, Failed
/// @param u8MBox \b IN: MailBox
///     - VPU_RISC_MBOX0,
///     - VPU_RISC_MBOX1,
///-----------------------------------------------------------------------------
MS_BOOL HAL_JPD_ClearVPUMBox(MS_U8 u8MBox, MS_BOOL bFlag)
{
    MS_BOOL bResult = TRUE;
    switch(u8MBox)
    {
        case VPU_RISC_MBOX0:
            __HAL_JPD_WriteBit(VPU_REG_RISC_MBOX_CLR, bFlag, VPU_REG_RISC_MBOX0_CLR);
            break;
        case VPU_RISC_MBOX1:
            __HAL_JPD_WriteBit(VPU_REG_RISC_MBOX_CLR, bFlag, VPU_REG_RISC_MBOX1_CLR);
            break;
        default:
            bResult = FALSE;
            break;
    }
    return bResult;
}

MS_BOOL HAL_JPD_InitVPU(MS_BOOL bInMIU1)
{
    VPU_Init_Params VPUInitParams = { E_VPU_CLOCK_216MHZ, FALSE, -1, MSOS_WAIT_FOREVER, FALSE, E_VPU_DECODER_MJPEG};
    HAL_VPU_InitRegBase(u32JPDRiuBaseAdd);

    if(bInMIU1)
    {
        VPUInitParams.bInMIU1 = TRUE;
    }
    else
    {
        VPUInitParams.bInMIU1 = FALSE;
    }
    return HAL_VPU_Init(&VPUInitParams);
}

MJPEG_MSTAR_CHIP_ID HAL_JPD_Get_Chip_ID(void)
{
    return E_MJPEG_MSTAR_CHIP_T8;
}

#endif

