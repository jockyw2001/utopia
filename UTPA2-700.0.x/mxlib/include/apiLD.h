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

/*! \defgroup G_LD Local dimming interface
    \ingroup  G_PERIPHERAL
   
    \brief 
    
    Local dimming is introduced to achieve power saving and higher image contrast for LED backlight. 


    <b>Features</b>

    - Less backlight, less energy
    - Compensation restores image luminance
    - Average 10~30% saving
    - Dark region becomes even darker
    - Light leakage problem

    <b> LD Block Diagram: </b> \n
    \image html apiLD_pic.png

    <b> Operation Code Flow: </b> \n
    -# mboot init, local dimming ready
    -# kernel run algorithm
    -# pm51 read led data from sharememory
    -# pm51 send luminance data to led via spi
    \image html apiLD_pic2.png

    \defgroup LD_Init LD Init API
    \ingroup LD
    \defgroup LD_Debug LD Debug API
    \ingroup LD
    \defgroup LD_to_be_removed LD_to_be_removed (to be removed)
    \ingroup LD
*/

#ifndef _API_XC_LD_H_
#define _API_XC_LD_H_

//#include "MsCommon.h"
//#include "MsDevice.h"
#include "MsTypes.h"

#ifdef __cplusplus
extern "C" {
#endif


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define LD_LOG_DBG     1

// Debug related
#if LD_LOG_DBG
#define LD_DBG(_fmt, _args...)      {printf("LD:"_fmt, ##_args); }
#define LD_ASSERT(_fmt, _args...)  {printf("LD ASSERT: %s %d:"_fmt, __FILE__, __LINE__, ##_args); }
#else
#define LD_DBG(_fmt, _args...)      { }
#define LD_ASSERT(_fmt, _args...)  {printf("LD ASSERT: %s %d:"_fmt, __FILE__, __LINE__, ##_args); } 
#endif


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

/// Define PNL local dimming return type
typedef enum
{
    E_APILD_STATUS_SUCCESS = 0,
    E_APILD_STATUS_FAIL = 1,
    E_APILD_STATUS_NOT_SUPPORTED,
    E_APILD_STATUS_PARAMETER_ERROR,
    E_APILD_OBTAIN_MUTEX_FAIL,            ///< obtain mutex timeout when calling this function
    E_APILD_OBTAIN_MMIO_FAIL,            ///< obtain mutex timeout when calling this function
} E_LD_RETURN;

typedef enum
{
    E_LD_LED_EDGE_TB_TYPE = 0, // deprecated
    E_LD_LED_EDGE_LR_TYPE = 1, // deprecated
    E_LD_LED_DIRECT_TYPE  = 2,
    E_LD_LED_LOCAL_TYPE   = 3,  // new mode supporting 32x32 LED
    E_LD_LED_TYPE_NUM,
    E_LD_LED_TYPE_MAX = E_LD_LED_TYPE_NUM,
} E_LD_TYPE;

typedef struct _stLD_Init_Parameters
{
    E_LD_TYPE ePnlLDType;
    MS_U8* pu8RawData;
    MS_U16 u16DataSize;
    MS_U16 u16PnlWidth;
    MS_U16 u16PnlHeight;
    MS_U16 u16MemSize;
    MS_PHYADDR u32MemAdrress;
    MS_U32 u32Reserved;
} stLD_Init_Parameters, *pstLD_Init_Parameters;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//--------------


//-------------------------------------------------------------------------------------------------
/// init local dimming, set base registers, load table etc
/// @return MS_U16
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_LD_Init(pstLD_Init_Parameters pstLDArgs, MS_U16 u16DataLen);



//-------------------------------------------------------------------------------------------------
/// set debug level
/// @return MS_U16
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_LD_SetDbgLevel(MS_U16 u16DbgSwitch);


#ifdef __cplusplus
}
#endif

#endif
