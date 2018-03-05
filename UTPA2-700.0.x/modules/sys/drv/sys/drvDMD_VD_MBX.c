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
/// file    drvSYS.c
/// @brief  System Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsVersion.h"
#include "regCHIP.h"
#include "halCHIP.h"
#include "drvMMIO.h"
#include "drvDMD_VD_MBX.h"
#include "halDMD_VD_MBX.h"
#include "ULog.h"



//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define DMD_VD_MBX_LOCK()      \
    do{                         \
        MS_ASSERT(MsOS_In_Interrupt() == FALSE); \
        ULOGD(TAG_DMD_VD_MBX,"%s lock\n", __FUNCTION__);\
        MsOS_ObtainMutex(_s32_DMD_VD_MBX_Mutex, MSOS_WAIT_FOREVER);\
        }while(0)

#define DMD_VD_MBX_UNLOCK()      \
    do{                         \
        MsOS_ReleaseMutex(_s32_DMD_VD_MBX_Mutex);\
        ULOGD(TAG_DMD_VD_MBX,"%s unlock\n", __FUNCTION__); \
        }while(0)
#else
#define DMD_VD_MBX_LOCK()      \
    do{                         \
        MS_ASSERT(MsOS_In_Interrupt() == FALSE); \
        MsOS_ObtainMutex(_s32_DMD_VD_MBX_Mutex, MSOS_WAIT_FOREVER);\
        }while(0)

#define DMD_VD_MBX_UNLOCK()      \
    do{                         \
        MsOS_ReleaseMutex(_s32_DMD_VD_MBX_Mutex);\
        }while(0)
#endif

#define TAG_DMD_VD_MBX "DMD_VD_MBX"
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if (DMD_VD_MBX_CHIP_VERSION >= DMD_VD_MBX_CHIP_T3)
static MS_S32 _s32_DMD_VD_MBX_Mutex=-1;
#endif
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// System initialzation
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
#if (DMD_VD_MBX_CHIP_VERSION >= DMD_VD_MBX_CHIP_T3)
MS_BOOL MDrv_SYS_DMD_VD_MBX_Init(void)
{
    if (_s32_DMD_VD_MBX_Mutex == -1)
    {
        _s32_DMD_VD_MBX_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex DMD_VD_MBX", MSOS_PROCESS_SHARED);
        MS_ASSERT(_s32_DMD_VD_MBX_Mutex >= 0);
        #ifdef MS_DEBUG
        ULOGD(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_Init %lx\n",_s32_DMD_VD_MBX_Mutex);
        #endif
    }
    else
    {
        ULOGE(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_Init more than twice\n");
        return FALSE;
    }

    if (_s32_DMD_VD_MBX_Mutex >= 0)
    {
        return HAL_SYS_DMD_VD_MBX_Init();
    }
    else
    {
        #ifdef MS_DEBUG
        ULOGE(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_Init Fail\n");
        #endif
        return FALSE;
    }
}

void MDrv_SYS_DMD_VD_MBX_SetType(DMD_VD_MBX_Type eType)
{
    DMD_VD_MBX_LOCK();
    switch (eType)
    {
        case E_DMD_VD_MBX_TYPE_ATV:
            HAL_SYS_DMD_VD_MBX_SetType(BIN_FOR_ATV);
            break;
        case E_DMD_VD_MBX_TYPE_DVBT:
            HAL_SYS_DMD_VD_MBX_SetType(BIN_FOR_DVBT);
            break;
        case E_DMD_VD_MBX_TYPE_DVBC:
            HAL_SYS_DMD_VD_MBX_SetType(BIN_FOR_DVBC);
            break;
        case E_DMD_VD_MBX_TYPE_ATSC:
            HAL_SYS_DMD_VD_MBX_SetType(BIN_FOR_ATSC);
            break;
            
        case E_DMD_VD_MBX_TYPE_DTMB: 
        	  HAL_SYS_DMD_VD_MBX_SetType(BIN_FOR_DTMB);
            break;   
            
        case E_DMD_VD_MBX_TYPE_ISDBT:
            HAL_SYS_DMD_VD_MBX_SetType(BIN_FOR_ISDBT);
            break;
        case E_DMD_VD_MBX_TYPE_DVBS:

            HAL_SYS_DMD_VD_MBX_SetType(BIN_FOR_DVBS);

            break;

        case E_DMD_VD_MBX_TYPE_DVBT2:

            HAL_SYS_DMD_VD_MBX_SetType(BIN_FOR_DVBT2);

            break;

        default:
            #ifdef MS_DEBUG
            ULOGE(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_SetType Fail\n");
            #endif
            break;
    }
    DMD_VD_MBX_UNLOCK();
}

MS_BOOL MDrv_SYS_DMD_VD_MBX_ReadReg(MS_U32 u32Reg, MS_U8 *u8Value)
{
    MS_BOOL bResult = FALSE;
    DMD_VD_MBX_LOCK();
    u32Reg &= 0xFFFF;
    switch (HAL_SYS_DMD_VD_MBX_GetType())
    {
        case BIN_FOR_ATV:
            bResult = HAL_SYS_DMD_VD_MBX_ATV_ReadByte(u32Reg, u8Value);
            break;
        case BIN_FOR_DVBT:
        case BIN_FOR_DVBC:
        case BIN_FOR_DVBS:

		case BIN_FOR_DVBT2:

            bResult = HAL_SYS_DMD_VD_MBX_DVB_ReadByte(u32Reg, u8Value);
            break;
        case BIN_FOR_ATSC:
        case BIN_FOR_DTMB:
        case BIN_FOR_ISDBT:
            *u8Value = HAL_SYS_DMD_VD_MBX_ATSC_ReadByte(u32Reg);
            bResult = TRUE;
            break;
        default:
            #ifdef MS_DEBUG
            ULOGE(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_ReadReg>> Incorrect firmware code!!\n");
            #endif
            *u8Value = 0;
            bResult = FALSE;
            break;
    }
    #ifdef MS_DEBUG
    ULOGD(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_ReadReg %lx %x\n",u32Reg,*u8Value);
    #endif
    DMD_VD_MBX_UNLOCK();
    return bResult;
}

MS_BOOL MDrv_SYS_DMD_VD_MBX_ReadDSPReg(MS_U32 u32Reg, MS_U8 *u8Value)
{
    MS_BOOL bResult;
    DMD_VD_MBX_LOCK();
    u32Reg &= 0xFFFF;
    if (HAL_SYS_DMD_VD_MBX_GetType() & (BIN_FOR_DVBT|BIN_FOR_DVBC|BIN_FOR_DVBT2|BIN_FOR_DVBS))

    {
        bResult = HAL_SYS_DMD_VD_MBX_DVB_ReadDspReg(u32Reg, u8Value);
    }
    else
    {
        #ifdef MS_DEBUG
        ULOGE(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_ReadDSPReg>> Incorrect firmware code!!\n");
        #endif
        bResult = FALSE;
    }
    #ifdef MS_DEBUG
    ULOGD(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_ReadDSPReg %lx %x\n",u32Reg,*u8Value);
    #endif
    DMD_VD_MBX_UNLOCK();
    return bResult;
}

#if (DMD_VD_MBX_CHIP_VERSION >= DMD_VD_MBX_CHIP_T12_T13)
MS_BOOL MDrv_SYS_DMD_VD_MBX_DBG_ReadReg(MS_U32 u32Reg, MS_U8 *u8Value)
{
    MS_BOOL bResult;
    DMD_VD_MBX_LOCK();
    u32Reg &= 0xFFFF;
    if (HAL_SYS_DMD_VD_MBX_GetType() & (BIN_FOR_DVBT|BIN_FOR_DVBC|BIN_FOR_DVBT2|BIN_FOR_DVBS))

    {
        bResult = HAL_SYS_DMD_VD_MBX_DVB_DBG_ReadReg(u32Reg, u8Value);
    }
    else
    {
        #ifdef MS_DEBUG
        ULOGE(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_DBG_ReadReg>> Incorrect firmware code!!\n");
        #endif
        bResult = FALSE;
    }
    #ifdef MS_DEBUG
    ULOGD(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_DBG_ReadReg %lx %x\n",u32Reg,*u8Value);
    #endif
    DMD_VD_MBX_UNLOCK();
    return bResult;
}
#else
MS_BOOL MDrv_SYS_DMD_VD_MBX_DBG_ReadReg(MS_U32 u32Reg, MS_U8 *u8Value)
{
    u32Reg = u32Reg;
    *u8Value = *u8Value;

    return TRUE;
}
#endif

MS_BOOL MDrv_SYS_DMD_VD_MBX_WriteReg(MS_U32 u32Reg, MS_U8 u8Value)
{
    MS_BOOL bResult = FALSE;
    DMD_VD_MBX_LOCK();
    u32Reg &= 0xFFFF;
    #ifdef MS_DEBUG
    ULOGD(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_WriteReg %lx %x\n",u32Reg,u8Value);
    #endif
    switch (HAL_SYS_DMD_VD_MBX_GetType())
    {
        case BIN_FOR_ATV:
            bResult = HAL_SYS_DMD_VD_MBX_ATV_WriteByte(u32Reg, u8Value);
            break;
        case BIN_FOR_DVBT:
        case BIN_FOR_DVBC:
        case BIN_FOR_DVBS:	

		case BIN_FOR_DVBT2:

            bResult = HAL_SYS_DMD_VD_MBX_DVB_WriteByte(u32Reg, u8Value);
            break;
        case BIN_FOR_ATSC:
        case BIN_FOR_DTMB:
        case BIN_FOR_ISDBT:
            HAL_SYS_DMD_VD_MBX_ATSC_WriteByte(u32Reg, u8Value);
            bResult = TRUE;
            break;
        default:
            #ifdef MS_DEBUG
            ULOGE(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_WriteReg>> Incorrect firmware code!!\n");
            #endif
            DMD_VD_MBX_UNLOCK();
            bResult = FALSE;
            break;
    }
    DMD_VD_MBX_UNLOCK();
    return bResult;
}

MS_BOOL MDrv_SYS_DMD_VD_MBX_WriteDSPReg(MS_U32 u32Reg, MS_U8 u8Value)
{
    MS_BOOL bResult;
    DMD_VD_MBX_LOCK();
    u32Reg &= 0xFFFF;
    #ifdef MS_DEBUG
    ULOGD(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_WriteDSPReg %lx %x\n",u32Reg,u8Value);
    #endif
    if (HAL_SYS_DMD_VD_MBX_GetType()& (BIN_FOR_DVBT|BIN_FOR_DVBC|BIN_FOR_DVBT2|BIN_FOR_DVBS))

    {
        bResult = HAL_SYS_DMD_VD_MBX_DVB_WriteDspReg(u32Reg, u8Value);
    }
    else
    {
        #ifdef MS_DEBUG
        ULOGE(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_WriteDSPReg>> Incorrect firmware code!!\n");
        #endif
        bResult = FALSE;
    }
    DMD_VD_MBX_UNLOCK();
    return bResult;
}

#if (DMD_VD_MBX_CHIP_VERSION >= DMD_VD_MBX_CHIP_T12_T13)
MS_BOOL MDrv_SYS_DMD_VD_MBX_DBG_WriteReg(MS_U32 u32Reg, MS_U8 u8Value)
{
    MS_BOOL bResult;
    DMD_VD_MBX_LOCK();
    u32Reg &= 0xFFFF;
    #ifdef MS_DEBUG
    ULOGD(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_DBG_WriteReg %lx %x\n",u32Reg,u8Value);
    #endif
    if (HAL_SYS_DMD_VD_MBX_GetType()& (BIN_FOR_DVBT|BIN_FOR_DVBC|BIN_FOR_DVBT2|BIN_FOR_DVBS))

    {
        bResult = HAL_SYS_DMD_VD_MBX_DVB_DBG_WriteReg(u32Reg, u8Value);
    }
    else
    {
        #ifdef MS_DEBUG
        ULOGE(TAG_DMD_VD_MBX,"MDrv_SYS_DMD_VD_MBX_DBG_WriteReg>> Incorrect firmware code!!\n");
        #endif
        bResult = FALSE;
    }
    DMD_VD_MBX_UNLOCK();
    return bResult;
}
#else
MS_BOOL MDrv_SYS_DMD_VD_MBX_DBG_WriteReg(MS_U32 u32Reg, MS_U8 u8Value)
{
    u32Reg = u32Reg;
    u8Value = u8Value;

    return TRUE;
}
#endif
#else
MS_BOOL MDrv_SYS_DMD_VD_MBX_Init(void)
{
    return FALSE;
}
void MDrv_SYS_DMD_VD_MBX_SetType(DMD_VD_MBX_Type eType)
{
    return;
}
MS_BOOL MDrv_SYS_DMD_VD_MBX_ReadReg(MS_U32 u32Reg, MS_U8 *u8Value)
{
    *u8Value = 0;
    return FALSE;
}
MS_BOOL MDrv_SYS_DMD_VD_MBX_ReadDSPReg(MS_U32 u32Reg, MS_U8 *u8Value)
{
    return FALSE;
}
MS_BOOL MDrv_SYS_DMD_VD_MBX_DBG_ReadReg(MS_U32 u32Reg, MS_U8 *u8Value)
{
    return FALSE;
}
MS_BOOL MDrv_SYS_DMD_VD_MBX_WriteReg(MS_U32 u32Reg, MS_U8 u8Value)
{
    return FALSE;
}
MS_BOOL MDrv_SYS_DMD_VD_MBX_WriteDSPReg(MS_U32 u32Reg, MS_U8 u8Value)
{
    return FALSE;
}
MS_BOOL MDrv_SYS_DMD_VD_MBX_DBG_WriteReg(MS_U32 u32Reg, MS_U8 u8Value)
{
    return FALSE;
}
#endif
