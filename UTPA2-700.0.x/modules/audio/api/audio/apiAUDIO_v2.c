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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   apiAUDIO_v2.c
/// @brief  Audio 1.0 API wrapper for Audio 2.0
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "utopia.h"
#include "apiAUDIO_private.h"
#include "apiAUDIO_v2.h"
#include "drvMBX.h"

#include "utopia_dapi.h"
//#include "util_symbol.h"
#include "MsTypes.h"
#include "MsOS.h"
#include "MsCommon.h"
#include "MsVersion.h"

#include "../drv/audio/internal/drvAUDIO_internal.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/module.h>
#else
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#endif
//#include <linux/kernel.h>


//------------------------------------------------------------------------------
// Local Variable
//------------------------------------------------------------------------------
#ifdef UTOPIA_20
static void*                        _pInstantAudio    = NULL;
static void*                        _pAttributeAudio  = NULL;
#endif

//-------------------------------------------------------------------------------------------------
//  Extern Variables
//-------------------------------------------------------------------------------------------------
extern AUDIO_SHARED_VARS2 * g_AudioVars2;

//------------------------------------------------------------------------------
// Internal implementation
//------------------------------------------------------------------------------

#ifdef UTOPIA_20
static MS_BOOL _AUDIO_IOCtrlOpen(void)
{

    if(_pInstantAudio == NULL)
    {
        #ifdef  AUDIO_UTOPIA2K
        if(UtopiaOpen(MODULE_AUDIO|KERNEL_MODE, &_pInstantAudio, 0, _pAttributeAudio) !=  UTOPIA_STATUS_SUCCESS)
        #else
        if(UtopiaOpen(MODULE_AUDIO, &_pInstantAudio, 0, _pAttributeAudio) !=  UTOPIA_STATUS_SUCCESS)
        #endif
        {
            ULOGE("AUDIO", "[MAPI AUDIO][%06d] Open Audio Lib fail\n", __LINE__);
            return FALSE;
        }
    }

    return TRUE;
}
#endif

//============================================================
// AUDIO_SYSTEM RELATIONAL API FUNCTION
//============================================================

void MApi_AUDIO_Initialize(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Audio Init fail\n", __LINE__);
        return;
    }

    // add for Kdrv MBX export function
    #if 0//def MSOS_TYPE_LINUX_KERNEL
    MBX_CPU_ID eHKCPU;
    MS_U32 u32TimeoutMillSecs = 10000;
    eHKCPU = E_MBX_CPU_MIPS;
    if ( E_MBX_SUCCESS != MApi_MBX_Init(eHKCPU, E_MBX_ROLE_HK, u32TimeoutMillSecs))
    {
        return FALSE;
    }
    MApi_MBX_Enable(TRUE);
    #endif

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Initialize, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Audio Init fail\n", __LINE__);
        return;
    }

    return;

#else

    return _MApi_AUDIO_Initialize();

#endif
}


void MApi_AUDIO_SetPowerOn(MS_BOOL bFlag)
{

#ifdef UTOPIA_20

    MS_U32 u32Param = ((MS_U32)bFlag);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Audio Set Power on  fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetPowerOn, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Audio Set Power on fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetPowerOn(bFlag);

#endif
}

void MApi_AUDIO_SetDspBaseAddr(MS_U8 u8Index, MS_U32 u32Bin_Base_Address, MS_U32 u32Mad_Base_Buffer_Adr)
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_DSPBASEADDR Status;
    Status.u8Index = u8Index;
    Status.u32Bin_Base_Address = u32Bin_Base_Address;
    Status.u32Mad_Base_Buffer_Adr = u32Mad_Base_Buffer_Adr;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set DSP Base Address  fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDSPBaseAddr, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set DSP Base Address fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MDrv_AUDIO_SetDspBaseAddr(u8Index, u32Bin_Base_Address, u32Mad_Base_Buffer_Adr);

#endif
}

MS_U32 MApi_AUDIO_GetDspMadBaseAddr(MS_U8 u8Index)
{

#ifdef UTOPIA_20
    AUDIO_ST_GET_DSPBASEADDR Status;
    Status.u8Index = u8Index;
    Status.u32Addr = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get  DSP Base Address  fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetDSPBaseAddr, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get DSP Base Address fail\n", __LINE__);
        //return;
    }
    return Status.u32Addr;

#else

    return _MDrv_AUDIO_GetDspMadBaseAddr(u8Index);

#endif
}

void MApi_AUDIO_WriteDecMailBox(MS_U8 u8ParamNum, MS_U16 u16Data)
{

#ifdef UTOPIA_20
    AUDIO_ST_WRITE_DECMAILBOX Status;
    Status.u8ParamNum = u8ParamNum;
    Status.u16Data = u16Data;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Write Dec Mail Box  fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_WriteDecMailBox, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Write Dec Mail Box fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MDrv_AUDIO_WriteDecMailBox(u8ParamNum, u16Data);

#endif
}

void MApi_AUDIO_TriggerSifPLL(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Trigger SIF PLL fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_TriggerSIFPLL, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Trigger SIF PLL fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MDrv_AUDIO_TriggerSifPLL();

#endif
}


/////*******************  PATCH GROUP  ********************************////
void MDrv_AUDIO_SetDspBaseAddr(MS_U8 u8Index, MS_PHY u32Bin_Base_Address, MS_PHY u32Mad_Base_Buffer_Adr)
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_DSPBASEADDR Status;
    Status.u8Index = u8Index;
    Status.u32Bin_Base_Address = u32Bin_Base_Address;
    Status.u32Mad_Base_Buffer_Adr = u32Mad_Base_Buffer_Adr;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set DSP Base Address  fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDSPBaseAddr, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set DSP Base Address fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MDrv_AUDIO_SetDspBaseAddr(u8Index, u32Bin_Base_Address, u32Mad_Base_Buffer_Adr);

#endif
}

MS_PHY MDrv_AUDIO_GetDspMadBaseAddr(MS_U8 u8Index)
{

#ifdef UTOPIA_20
    AUDIO_ST_GET_DSPBASEADDR Status;
    Status.u8Index = u8Index;
    Status.u32Addr = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get  DSP Base Address  fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetDSPBaseAddr, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get DSP Base Address fail\n", __LINE__);
        //return;
    }
    return Status.u32Addr;

#else

    return _MDrv_AUDIO_GetDspMadBaseAddr(u8Index);

#endif
}

void MDrv_AUDIO_WriteDecMailBox(MS_U8 u8ParamNum, MS_U16 u16Data)
{

#ifdef UTOPIA_20
    AUDIO_ST_WRITE_DECMAILBOX Status;
    Status.u8ParamNum = u8ParamNum;
    Status.u16Data = u16Data;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Write Dec Mail Box  fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_WriteDecMailBox, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Write Dec Mail Box fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MDrv_AUDIO_WriteDecMailBox(u8ParamNum, u16Data);

#endif
}

void MDrv_AUDIO_TriggerSifPLL(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Trigger SIF PLL fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_TriggerSIFPLL, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Trigger SIF PLL fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MDrv_AUDIO_TriggerSifPLL();

#endif
}
/////*******************  PATCH GROUP End   **************************////

void MApi_Audio_Monitor(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Monitor fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Monitor, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Monitor fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_Audio_Monitor();

#endif
}

void MApi_AUDIO_SetSystemInfo(AUDIO_INIT_INFO *pau_info)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set System Info fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetSystemInfo, (void*)pau_info) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set System Info fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetSystemInfo(pau_info);

#endif
}

/*void MApi_AUDIO_ASF_Data_To_DDR(MS_BOOL bEnable)
{

#ifdef UTOPIA_20
    MS_U32 u32Param = ((MS_U32)bEnable);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ASF Data to DDR fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ASF_Data_To_DDR, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ASF Data to DDR fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_ASF_Data_To_DDR(bEnable);

#endif
}


void MApi_AUDIO_R2_DEBUG_MSG(MS_BOOL bEnable)
{

#ifdef UTOPIA_20
    MS_U32 u32Param = ((MS_U32)bEnable);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] R2 Debug MSG fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_R2_DEBUG_MSG, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] R2 Debug MSG fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_R2_DEBUG_MSG(bEnable);

#endif
}

void MApi_AUDIO_R2_DDR_LATENCY_MSG(MS_BOOL bEnable)
{

#ifdef UTOPIA_20
    MS_U32 u32Param = ((MS_U32)bEnable);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] R2 DDR Latency MSG fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_R2_DDR_LETENCY_MSG, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] R2 DDR Latency MSG fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_R2_DDR_LATENCY_MSG(bEnable);

#endif
}*/

void MApi_AUDIO_SetOutConnectivity(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set out connectivity fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetOutConnectivity, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set out connectivity fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetOutConnectivity();

#endif
}

void MApi_AUDIO_SetPathInfo(AUDIO_PATH_INFO *path_info)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Path Info fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetPathInfo, (void*)path_info) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Path Info fai\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetPathInfo(path_info);

#endif
}

void MApi_AUDIO_GetPathInfo(AUDIO_PATH_INFO *path_info)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Path Info fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetPathInfo, (void*)path_info) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Path Info fai\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_GetPathInfo(path_info);

#endif
}

void MApi_AUDIO_SetOutputInfo(AUDIO_OUT_INFO *pout_info)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Output Info fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetOutputInfo, (void*)pout_info) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Output Info fai\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetOutputInfo(pout_info);

#endif
}

void MApi_AUDIO_SetSourceInfo(AUDIO_SOURCE_INFO_TYPE eSourceType)
{

#ifdef UTOPIA_20

    MS_U32 u32Param = ((MS_U32)eSourceType);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Source Info fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetSourceInfo, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Source Info fai\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetSourceInfo(eSourceType);

#endif
}

void MApi_AUDIO_InputSwitch(AUDIO_INPUT_TYPE enSource, AUDIO_SWITCH_GROUP enGroup)
{

#ifdef UTOPIA_20
    AUDIO_ST_INPUTSWITCH Status;
    Status.eSource = enSource;
    Status.eGroup = enGroup;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Input Switch fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_InputSwitch, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Input Switch fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_InputSwitch(enSource, enGroup);

#endif
}

/*void MApi_AUDIO_SetInputScale(int Step)
{

#ifdef UTOPIA_20

    MS_U32 u32Param = ((MS_U32)Step);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Input Scale fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Set_Input_Scale, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Input Scale fai\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetInputScale(Step);

#endif
}

void MApi_AUDIO_SetScartScale(int Step)
{

#ifdef UTOPIA_20

    MS_U32 u32Param = ((MS_U32)Step);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Scart Scale fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Set_Scart_Scale, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Scart Scale fai\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetScartScale(Step);

#endif
}

MS_BOOL MApi_AUDIO_Audyssey_VXT_Set_THRESH_LEVEL_TABLE(int *thresh_level_table)
{

#ifdef UTOPIA_20
    AUDIO_ST_AUDYSSEY_VXT_SET_THRESH_LEVEL_TABLE Status;
    Status.bStatus = FALSE;
    Status.pthresh_level_table = thresh_level_table;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Audyssey vxt set thresh level table fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_AUDYSSEY_VXT_SET_THRESH_LEVEL_TABLE, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Audyssey vxt set thresh level table fail\n", __LINE__);
        //return;
    }

    return Status.bStatus;

#else

    return _MApi_AUDIO_Audyssey_VXT_Set_THRESH_LEVEL_TABLE(thresh_level_table);

#endif
}

void  MApi_AUDIO_SPDIF_SetChannelStatus(SPDIF_CS_MODE_TYPE eType, SPDIF_CS_MODE_VALUE eValue)
{


#ifdef UTOPIA_20
    AUDIO_ST_SPDIF_CHANNELSTATUS Status;
    Status.bSet = TRUE;
    Status.eType = eType;
    Status.eValue = eValue;
    Status.u32Ret = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SPDIF Set Channel Status fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SPDIF_CHANNELSTATUS, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SPDIF Set Channel Status fail\n", __LINE__);
        //return;
    }

    return;

#else

    return _MApi_AUDIO_SPDIF_SetChannelStatus(eType, eValue);

#endif
}

MS_U16  MApi_AUDIO_SPDIF_GetChannelStatus(SPDIF_CS_MODE_TYPE eType)
{

#ifdef UTOPIA_20
    AUDIO_ST_SPDIF_CHANNELSTATUS Status;
    Status.bSet = FALSE;
    Status.eType = eType;
    Status.eValue = 0;
    Status.u32Ret = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SPDIF Set Channel Status fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SPDIF_CHANNELSTATUS, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SPDIF Set Channel Status fail\n", __LINE__);
        //return;
    }

    return Status.u32Ret;

#else

    return _MApi_AUDIO_SPDIF_GetChannelStatus(eType);

#endif
}*/


MS_BOOL MApi_AUDIO_SetDataCaptureSource(AUDIO_DEVICE_TYPE eID, AUDIO_CAPTURE_SOURCE_TYPE eSource)
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_DATACAPTURESOURCE Status;
    Status.bRet = 0;
    Status.eID = eID;
    Status.eSource = eSource;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Data Capture Source fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDataCaptureSource, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Data Capture Source fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_SetDataCaptureSource(eID, eSource);

#endif
}

void MApi_AUDIO_SetOutputSourceInfo(OUTPUT_SOURCE_INFO *pSourceInfo)
{

#ifdef UTOPIA_20
    AUDIO_ST_OUTPUT_SOURCE Status;
    Status.bSet = TRUE;
    Status.stOutputSourceInfo = pSourceInfo;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Output Source Info fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_OutputSourceInfo, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Output Source Info fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetOutputSourceInfo(pSourceInfo);

#endif
}

void  MApi_AUDIO_GetOutputSourceInfo(OUTPUT_SOURCE_INFO *pSourceInfo)
{

#ifdef UTOPIA_20
    AUDIO_ST_OUTPUT_SOURCE Status;
    Status.bSet = FALSE;
    Status.stOutputSourceInfo = pSourceInfo;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Output Source Info fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_OutputSourceInfo, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Output Source Info fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_GetOutputSourceInfo(pSourceInfo);

#endif
}

MS_U16 MApi_AUDIO_ReadMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum)
{

#ifdef UTOPIA_20
    AUDIO_ST_READ_MAILBOX Status;
    Status.u16Ret = 0;
    Status.bDspType = bDspType;
    Status.u8ParamNum = u8ParamNum;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Read MailBox fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ReadMailBox, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Read MailBox fail\n", __LINE__);
        //return;
    }

    return Status.u16Ret;

#else

    return _MApi_AUDIO_ReadMailBox(bDspType, u8ParamNum);

#endif
}

void MApi_AUDIO_WriteMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum, MS_U16 u16Data)
{

#ifdef UTOPIA_20
    AUDIO_ST_WRITE_MAILBOX Status;
    Status.bDspType = bDspType;
    Status.u8ParamNum = u8ParamNum;
    Status.u16Data = u16Data;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Write MailBox fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_WriteMailBox, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Write MailBox fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_WriteMailBox(bDspType, u8ParamNum, u16Data);

#endif
}

void MApi_AUDIO_FwTriggerDSP(MS_U16 u16Cmd)
{

#ifdef UTOPIA_20
    MS_U32 u32Param = ((MS_U32)u16Cmd);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Fw Trigger DSP fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_FwTriggerDSP, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Fw Trigger DSP fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_FwTriggerDSP(u16Cmd);

#endif
}

void MApi_AUDIO_SetFwStatus(MS_BOOL bBootOnDDR)
{

#ifdef UTOPIA_20
    MS_U32 u32Param = ((MS_U32)bBootOnDDR);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Fw Status fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetFwStatus, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Fw Status fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetFwStatus(bBootOnDDR);

#endif
}

void MApi_AUDIO_ExitAudioSystem(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Exit Auido system fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ExitAudioSystem, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Exit Auido system fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_ExitAudioSystem();

#endif
}

void MApi_AUDIO_RebootDsp(MS_BOOL bDspType)
{

#ifdef UTOPIA_20
    MS_U32 u32Param = ((MS_U32)bDspType);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Reboot DSP fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_RebootDSP, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Reboot DSP fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_RebootDsp(bDspType);

#endif
}

void MApi_AUDIO_SendIntrupt(MS_BOOL bDspType,MS_U8 u8Cmd)
{

#ifdef UTOPIA_20
    AUDIO_ST_SEND_INTERRUPT Status;
    Status.bDspType = bDspType;
    Status.u8Cmd = u8Cmd;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Send Intrupt fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SendIntrupt, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Send Intrupt fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SendIntrupt(bDspType, u8Cmd);

#endif
}

void MApi_AUDIO_I2S_SetMode(AUDIO_I2S_MODE_TYPE u8Mode, AUDIO_I2S_MODE_VALUE  u8Val)
{

#ifdef UTOPIA_20
    AUDIO_ST_I2S_SETMODE Status;
    Status.eMode = u8Mode;
    Status.eVal = u8Val;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] I2S SetMode fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_I2S_SetMode, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] I2S SetMode fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_I2S_SetMode(u8Mode, u8Val);

#endif
}

void MApi_AUDIO_WritePreInitTable(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Write PreInit Table fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_WritePreInitTable, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Write PreInit Table fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_WritePreInitTable();

#endif
}

void MApi_AUDIO_EnaEarphone_LowPower_Stage(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable EarPhone LowPower Stage fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_EnaEarphone_LowPower_Stage, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable EarPhone LowPower Stage fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_EnaEarphone_LowPower_Stage();

#endif
}

void MApi_AUDIO_EnaEarphone_HighDriving_Stage(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable EarPhone HighDriving Stage fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_EnaEarphone_HighDriving_Stage, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable EarPhone HighDriving Stage fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_EnaEarphone_HighDriving_Stage();

#endif
}

//=============================================================
// AUDIO_SOUND RELATIONAL API FUNCTION
//=============================================================
MS_BOOL MApi_SND_ProcessEnable(Sound_ENABLE_Type Type, MS_BOOL enable)
{

#ifdef UTOPIA_20
    AUDIO_ST_SND_PROCESSENABLE Status;
    Status.bRet = 0;
    Status.eType = Type;
    Status.bEnable = enable;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SND Process Enable fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_SND_ProcessEnable, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SND Process Enable fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_SND_ProcessEnable(Type, enable);

#endif
}

MS_BOOL MApi_SND_SetParam1( Sound_SET_PARAM_Type Type, MS_U16 param1, MS_U16 param2)
{

#ifdef UTOPIA_20
    AUDIO_ST_SND_SETPARAM1 Status;
    Status.bRet = 0;
    Status.eType = Type;
    Status.u16Param1 = param1;
    Status.u16Param2 = param2;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SND Set Param1 fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_SND_SetParam1, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SND Set Param1 fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_SND_SetParam1(Type, param1, param2);

#endif
}

MS_U16 MApi_SND_GetParam1( Sound_GET_PARAM_Type Type, MS_U16 param1)
{

#ifdef UTOPIA_20
    AUDIO_ST_SND_GETPARAM1 Status;
    Status.u16Ret = 0;
    Status.eType = Type;
    Status.u16Param1 = param1;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SND Get Param1 fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_SND_GetParam1, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SND Get Param1 fail\n", __LINE__);
        //return;
    }
    return Status.u16Ret;

#else

    return _MApi_SND_GetParam1(Type, param1);

#endif
}

MS_BOOL MApi_AUDIO_SOUND_SetParam(Sound_SET_Type Type, MS_U32 *p_param )
{

#ifdef UTOPIA_20
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Type;
    Status.pParam = p_param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Sound Set Param fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Sound Set Param fail\n", __LINE__);
        //return;
    }
    return Status.bStatus;

#else

    return _MApi_AUDIO_SOUND_SetParam(Type, p_param);

#endif
}

void MApi_AUDIO_SetAbsoluteVolume(MS_U8 u8Path, MS_U8 u8Vol1, MS_U8 u8Vol2)
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_ABSOLUTEVOLUME Status;
    Status.u8Path = u8Path;
    Status.u8Vol1 = u8Vol1;
    Status.u8Vol2 = u8Vol2;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Absolute Volume fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetAbsoluteVolume, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Absolute Volume fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetAbsoluteVolume(u8Path, u8Vol1, u8Vol2);

#endif
}

void MApi_AUDIO_SetPreScale(MS_U8 u8Path, MS_U8 u8PreScale )
{

#ifdef UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = u8Path;
	u32Param[1] = u8PreScale;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetPreScale;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set PreScale fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set PreScale fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetPreScale(u8Path, u8PreScale);

#endif
}

void MApi_AUDIO_SetMute(MS_U8 u8Path, MS_BOOL EnMute )
{

#ifdef UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = u8Path;
    u32Param[1] = EnMute;

    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetMute;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Mute fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Mute fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetMute(u8Path, EnMute);

#endif
}

void MApi_AUDIO_SourceConnect(AUDIO_OUTPORT_SOURCE_TYPE eConnectSourceType, MS_BOOL bConnect )
{

#ifdef UTOPIA_20
    AUDIO_ST_SOURCECONNECT Status;
    Status.eConnectSourceType = eConnectSourceType;
    Status.bConnect = bConnect;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Source connect fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SourceConnect, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Source connect fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SourceConnect(eConnectSourceType, bConnect);

#endif
}

void MApi_AUDIO_SetBalance(MS_U8 u8Balance)
{

#ifdef UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = u8Balance;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetBalance;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Balance fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Balance fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetBalance(u8Balance);

#endif
}

void MApi_AUDIO_EnableEQ(MS_BOOL EnEQ)
{

#ifdef UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = EnEQ;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_EnableEQ;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable EQ fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable EQ fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_EnableEQ(EnEQ);

#endif
}

void MApi_AUDIO_EnableTone(MS_BOOL EnTone)
{

#ifdef UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = EnTone;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_EnableTone;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable Tone fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable Tone fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_EnableTone(EnTone);

#endif
}

void MApi_AUDIO_EnableAutoVolume(MS_BOOL EnAVC)
{

#ifdef UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = EnAVC;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_EnableAutoVolume;
   	Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable Auto Volume fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable Auto Volume fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_EnableAutoVolume(EnAVC);

#endif
}

void MApi_AUDIO_EnableSurround(MS_BOOL EnSurround)
{

#ifdef UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = EnSurround;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_EnableSurround;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable Surround fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable Surround fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_EnableSurround(EnSurround);

#endif
}

void MApi_AUDIO_EnableBalance(MS_BOOL EnBalance)
{

#ifdef UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = EnBalance;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_EnableBalance;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable Balance fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable Balance fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_EnableBalance(EnBalance);

#endif
}

void MApi_AUDIO_SetDynamicBass(MS_U8 u8DynamicBass )
{

#ifdef UTOPIA_20
    MS_U32 u32Param = ((MS_U32)u8DynamicBass);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Dynamic Bass fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDynamicBass, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Dynamic Bass fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetDynamicBass(u8DynamicBass);

#endif
}

void MApi_AUDIO_SetBass(MS_U8 u8Bass)
{

#ifdef UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = u8Bass;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetBass;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Bass fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Bass fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetBass(u8Bass);

#endif
}

void MApi_AUDIO_AbsoluteBass(MS_U8 u8Bass)
{

#ifdef UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = u8Bass;

    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_AbsoluteBass;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Absolute Bass fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Absolute Bass fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_AbsoluteBass(u8Bass);

#endif
}

void MApi_AUDIO_SetTreble(MS_U8 u8Treble)
{

#ifdef UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = u8Treble;

    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetTreble;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Treble fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Treble fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetTreble(u8Treble);

#endif
}

void MApi_AUDIO_AbsoluteTreble(MS_U8 u8Treble )
{

#ifdef UTOPIA_20
    MS_U32 u32Param = ((MS_U32)u8Treble);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Absolute Treble fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_AbsoluteTreble, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Absolute Treble fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_AbsoluteTreble(u8Treble);

#endif
}

void MApi_AUDIO_SetEq(MS_U8 u8band, MS_U8 u8level)
{

#ifdef UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = u8band;
    u32Param[1] = u8level;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_SetEq;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set EQ fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set EQ fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetEq(u8band, u8level);

#endif
}

void MApi_AUDIO_SetEq_7band(MS_U8 u8band, MS_U8 u8level)
{

#ifdef UTOPIA_20
    AUDIO_ST_SETEQ_7BAND Status;
    Status.u8Band = u8band;
    Status.u8Level = u8level;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set EQ 7band fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetEq_7band, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set EQ 7band fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetEq_7band(u8band, u8level);

#endif
}

void MApi_AUDIO_SetAvcThreshold(MS_U8 Level)
{

#ifdef UTOPIA_20

    MS_U32 u32Param = ((MS_U8)Level);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Avc Threshold fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetAvcThreshold, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Avc Threshold fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetAvcThreshold(Level);

#endif
}

void MApi_AUDIO_SetAvcMode(MS_U8 AvcMode)
{

#ifdef UTOPIA_20

    MS_U32 u32Param = ((MS_U8)AvcMode);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Avc Mode fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetAvcMode, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Avc Mode fail\n", __LINE__);
        return;
    }
    return;
#else

    return _MApi_AUDIO_SetAvcMode(AvcMode);

#endif
}

void MApi_AUDIO_SetAvcAT(MS_U8 AvcAT)
{

#ifdef UTOPIA_20

    MS_U32 u32Param = ((MS_U8)AvcAT);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Avc AT fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetAvcAT, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Avc AT fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetAvcAT(AvcAT);

#endif
}

void MApi_AUDIO_SetAvcRT(MS_U8 AvcRT)
{

#ifdef UTOPIA_20

    MS_U32 u32Param = ((MS_U8)AvcRT);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Avc RT fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetAvcRT, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Avc RT fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetAvcRT(AvcRT);

#endif
}

void MApi_AUDIO_SetBufferProcess(MS_U8 DelayTime)
{

#ifdef UTOPIA_20
    MS_U32 u32Param = ((MS_U32)DelayTime);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Buffer Process fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetBufferProcess, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Buffer Process fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetBufferProcess(DelayTime);

#endif
}

void MApi_AUDIO_SetSurroundXA(MS_U8 mode)
{

#ifdef UTOPIA_20

    MS_U32 u32Param = ((MS_U8)mode);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Surround XA fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetSurroundXA, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Surround XA fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetSurroundXA(mode);

#endif
}

void MApi_AUDIO_SetSurroundXB(MS_U8 mode )
{

#ifdef UTOPIA_20

    MS_U32 u32Param = ((MS_U8)mode);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Surround XB fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetSurroundXB, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Surround XB fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetSurroundXB(mode);

#endif
}

void MApi_AUDIO_SetSurroundXK( MS_U8 mode )
{

#ifdef UTOPIA_20

    MS_U32 u32Param = ((MS_U8)mode);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Surround XK fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetSurroundXK, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Surround XK fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetSurroundXK(mode);

#endif
}

void MApi_AUDIO_SetSurroundLPFGain(MS_U8 mode)
{

#ifdef UTOPIA_20

    MS_U32 u32Param = ((MS_U8)mode);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Surround LPF Gain fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetSurroundLPFGain, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Surround LPF Gain fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetSurroundLPFGain(mode);

#endif
}

MS_U8 MApi_AUDIO_ConvertVolumeUnit(MS_U8 UiVolume)
{

#ifdef UTOPIA_20
    AUDIO_ST_CONVERT_VOLUMEUNIT Status;
    Status.u8Ret = 0;
    Status.u8Uivolume = UiVolume;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Convert Volume Unit fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ConvertVolumeUnit, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Convert Volume Unit fail\n", __LINE__);
        //return;
    }

    return Status.u8Ret;

#else

    return _MApi_AUDIO_ConvertVolumeUnit(UiVolume);

#endif
}

void MApi_AUDIO_SetPEQCoef(AUDIO_PEQ_COEF *peq_coef)
{
#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set PEQ Coef fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetPEQCoef, (void*)peq_coef) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set PEQ Coef fai\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetPEQCoef(peq_coef);

#endif
}

void MApi_AUDIO_EnablePEQ(MS_BOOL Enpeq)
{

#ifdef UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = Enpeq;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_EnablePEQ;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable PEQ fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable PEQ fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_EnablePEQ(Enpeq);

#endif
}

MS_BOOL MApi_AUDIO_EnableDcRemove(MS_BOOL EnDcRemove)
{

#ifdef UTOPIA_20
    AUDIO_ST_ENABLE_DCREMOVE Status;
    Status.bRet = 0;
    Status.bEndcremove = EnDcRemove;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable DC Remove fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_EnableDcRemove, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable DC Remove fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_EnableDcRemove(EnDcRemove);

#endif
}
//============================================================
// AUDIO_DTV RELATIONAL API FUNCTION
//============================================================
MS_BOOL MApi_AUDIO_ReleaseDecodeSystem(AUDIO_DEC_ID DecId)
{

#ifdef UTOPIA_20
    AUDIO_ST_RELEASE_DECODESYSTEM Status;
    Status.bStatus = FALSE;
    Status.DecId = DecId;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Release Decode System fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ReleaseDecodeSystem, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Release Decode System fail\n", __LINE__);
        //return;
    }
    return (MS_BOOL)Status.bStatus;

#else

    return _MApi_AUDIO_ReleaseDecodeSystem(DecId);

#endif
}

MS_BOOL MApi_AUDIO_SetDecodeSystem(AUDIO_DEC_ID DecId,  AudioDecStatus_t * p_AudioDecStatus)
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_DECODESYSTEM Status;
    Status.bStatus = FALSE;
    Status.DecId = DecId;
    Status.p_AudioDecStatus = p_AudioDecStatus;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Decode System fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDecodeSystem, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Decode System fail\n", __LINE__);
        //return;
    }
    return Status.bStatus;

#else

    return _MApi_AUDIO_SetDecodeSystem(DecId, p_AudioDecStatus);

#endif
}

MS_BOOL MApi_AUDIO_GetDecodeSystem(AUDIO_DEC_ID DecId,  AudioDecStatus_t * p_AudioDecStatus)
{

#ifdef UTOPIA_20
    AUDIO_ST_GET_DECODESYSTEM Status;
    Status.bStatus = FALSE;
    Status.DecId = DecId;
    Status.p_AudioDecStatus = p_AudioDecStatus;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Decode System fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetDecodeSystem, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Decode System fail\n", __LINE__);
        //return;
    }
    return Status.bStatus;

#else

    return _MApi_AUDIO_GetDecodeSystem(DecId, p_AudioDecStatus);

#endif
}

MS_BOOL MApi_AUDIO_SetDecodeCmd(AUDIO_DEC_ID DecId, En_DVB_decCmdType enDecComamnd)
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_DECCMD Status;
    Status.bRet = 0;
    Status.eDecId = DecId;
    Status.eDecComamnd = enDecComamnd;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Decode Cmd fail\n", __LINE__);
        //return;
    }

	if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDecodeCmd, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Decode Cmd fail\n", __LINE__);
        //return;
    }
    return Status.bRet;

#else

    return _MApi_AUDIO_SetDecodeCmd(DecId, enDecComamnd);

#endif
}

AUDIO_DEC_ID MApi_AUDIO_OpenDecodeSystem(AudioDecStatus_t * p_AudioDecStatus)
{

#ifdef UTOPIA_20
    AUDIO_ST_OPEN_DECODESYSTEM Status;
    Status.DecId = AU_DEC_INVALID;
    Status.p_AudioDecStatus = p_AudioDecStatus;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Open Decode System fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_OpenDecodeSystem, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Open Decode System fail\n", __LINE__);
        //return;
    }

    return Status.DecId;

#else

    return _MApi_AUDIO_OpenDecodeSystem(p_AudioDecStatus);

#endif
}

MS_BOOL MApi_AUDIO_SetSystem(En_DVB_decSystemType enDecSystem)
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_SYSTEM Status;
    Status.bRet = 0;
    Status.eDecSystem = enDecSystem;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set System fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetSystem, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set System fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_SetSystem(enDecSystem);

#endif
}

MS_BOOL MApi_AUDIO_GetDecSysSupportStatus(En_DVB_decSystemType enDecSystem)
{

#ifdef UTOPIA_20
    AUDIO_ST_GET_DECSYSSUPPORTSTATUS Status;
    Status.bRet = 0;
    Status.eDecSystem = enDecSystem;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Dec Sys Support Status fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetDecSysSupportStatus, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Dec Sys Support Status fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_GetDecSysSupportStatus(enDecSystem);

#endif
}

MS_BOOL MApi_AUDIO_IsMadLock(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Is Mad Lock fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_IsMadLock, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Is Mad Lock fail\n", __LINE__);
        //return;
    }
    return ((MS_BOOL)u32param[1]);

#else

    return _MApi_AUDIO_IsMadLock();

#endif
}

En_DVB_decCmdType MApi_AUDIO_GetDecStatus(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Dec Status fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetDecStatus, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Dec Status fail\n", __LINE__);
        //return;
    }
    return ((En_DVB_decCmdType)u32param[1]);

#else

    return _MApi_AUDIO_GetDecStatus();

#endif
}

AU_DVB_DECCMD MApi_AUDIO_GetCommand(En_DVB_AudioDecoder AdoDecoderNo)
{

#ifdef UTOPIA_20
    AUDIO_ST_DECODER_COMMAND Status;
    Status.bSet = FALSE;
    Status.eDecCmd = AU_DVB_DECCMD_STOP;
    Status.eAdoDecoderNo = AdoDecoderNo;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Command fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Decode_Command, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Command fail\n", __LINE__);
        //return;
    }
    return Status.eDecCmd;

#else

    return _MApi_AUDIO_GetCommand(AdoDecoderNo);

#endif
}

void MApi_AUDIO_SetCommand(En_DVB_decCmdType enDecComamnd)
{

#ifdef UTOPIA_20
    AUDIO_ST_DECODER_COMMAND Status;
    Status.bSet = TRUE;
    Status.eDecCmd = AU_DVB_DECCMD_STOP;
    Status.enCommandType = enDecComamnd;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Command fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Decode_Command, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Command fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetCommand(enDecComamnd);

#endif
}

MS_BOOL MApi_AUDIO_GetMAD_LOCK(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get MAD Lock fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetMAD_Lock, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get MAD Lock fail\n", __LINE__);
        //return;
    }
    return ((MS_BOOL)u32param[1]);;

#else

    return _MApi_AUDIO_GetMAD_LOCK();

#endif
}

void MApi_AUDIO_SetADOutputMode(MS_U8 out_mode)
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_AD Status;
    Status.eType = AD_OUTPUT_MODE;
    Status.u8Data = out_mode;
    Status.u8Data2 = 0;
    Status.bMute = FALSE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set AD Output Mode fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SETAD, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set AD Output Mode fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetADOutputMode(out_mode);

#endif
}

void MApi_AUDIO_SetADAbsoluteVolume(MS_U8 AbsVolume)
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_AD Status;
    Status.eType = AD_ABSOLUTE_VOLUME;
    Status.u8Data = AbsVolume;
    Status.u8Data2 = 0;
    Status.bMute = FALSE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set AD Absolute Volume fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SETAD, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set AD Absolute Volume fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetADAbsoluteVolume(AbsVolume);

#endif
}

/*void MApi_AUDIO_SetMainVolumeForAD (MS_U8 u8Vol1, MS_U8 u8Vol2)
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_AD Status;
    Status.eType = AD_SET_MAIN_VOL;
    Status.u8Data = u8Vol1;
    Status.u8Data2 = u8Vol2;
    Status.bMute = FALSE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Main Volume For AD fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SETAD, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Main Volume For AD fail\n", __LINE__);
        //return;
    }
    return;

#else

    return _MApi_AUDIO_SetMainVolumeForAD(u8Vol1, u8Vol2);

#endif
}*/

void MApi_AUDIO_ADSetMute(MS_BOOL EnMute)
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_AD Status;
    Status.eType = AD_SET_MUTE;
    Status.u8Data = 0;
    Status.u8Data2 = 0;
    Status.bMute = EnMute;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set AD Mute fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SETAD, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set AD Mute fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_ADSetMute(EnMute);

#endif
}

//============================================================
// AUDIO_PVR RELATED FUNCTIONS
//============================================================
void MApi_AUDIO_SetEncInit(AU_DVB_ENCBR BitRate, MS_U8 u8EncFrameNum)
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_ENCINIT Status;
    Status.eBitRate = BitRate;
    Status.u8EncFrameNum = u8EncFrameNum;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Enc Init fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetEncInit, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Enc Init fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetEncInit(BitRate, u8EncFrameNum);

#endif
}

void MApi_AUDIO_SetEncCommand(AU_DVB_ENCCMD enEncComamnd)
{

#ifdef UTOPIA_20

    MS_U32 u32Param = ((MS_U32)enEncComamnd);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Enc Command fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetEncCommand, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Enc Command fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetEncCommand(enEncComamnd);

#endif
}

void MApi_AUDIO_GetEncodeFrameInfo(AU_DVB_ENC_FRAME_INFO *EncFrameInfo)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Enc Frame Info fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetEncodeFrameInfo, (void*)EncFrameInfo) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Enc Frame Info fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_GetEncodeFrameInfo(EncFrameInfo);

#endif
}

MS_BOOL MApi_AUDIO_GetEncodeDoneFlag(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Encode Done Flag fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetEncodeDoneFlag, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Encode Done Flag fail\n", __LINE__);
        //return;
    }
    return ((MS_BOOL)u32param[1]);;

#else

    return _MApi_AUDIO_GetEncodeDoneFlag();

#endif
}

//============================================================
// AUDIO_ATV RELATIONAL API FUNCTION
//============================================================
void MApi_AUDIO_SIF_SetStandard(AUDIOSTANDARD_TYPE standard_type)
{

#ifdef UTOPIA_20

    MS_U32 u32Param = ((MS_U32)standard_type);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Set Standard fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_SetStandard, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Set Standard fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SIF_SetStandard(standard_type);

#endif
}

MS_U8 MApi_AUDIO_SIF_GetSoundMode(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Get Sound Mode fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_GetSoundMode, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Get Sound Mode fail\n", __LINE__);
        //return;
    }
    return ((MS_BOOL)u32param[1]);

#else

    return _MApi_AUDIO_SIF_GetSoundMode();

#endif
}

MS_BOOL MApi_AUDIO_SIF_SetSoundMode(MS_U8 u8SifSoundMode)
{

#ifdef UTOPIA_20
    AUDIO_ST_SIF_SET_SOUNDMODE Status;
    Status.bRet = 0;
    Status.u8SifSoundMode = u8SifSoundMode;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Set Sound Mode fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_SetSoundMode, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Set Sound Mode fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_SIF_SetSoundMode(u8SifSoundMode);

#endif
}

MS_BOOL MApi_AUDIO_SIF_GetAudioStatus(AUDIOSTATUS * eAudioStatus)
{

#ifdef UTOPIA_20

    AUDIO_ST_SIF_GET_AUDIO_STATUS Status;

    Status.bStatus = FALSE;
    Status.p_eAudioStatus = eAudioStatus;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Get Audio Status fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_GetAudioStatus, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Get Audio Status fail\n", __LINE__);
        //return;
    }

    return Status.bStatus;

#else

    return _MApi_AUDIO_SIF_GetAudioStatus(eAudioStatus);

#endif
}

void MApi_AUDIO_SIF_StartAutoStandardDetection(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Start Auto standard Detection fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_StartAutoStandardDetection, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Start Auto standard Detection fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SIF_StartAutoStandardDetection();

#endif
}

AUDIOSTANDARD_TYPE MApi_AUDIO_SIF_GetResultOfAutoStandardDetection(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Get Result of Auto Standard Detection fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_GetResultOfAutoStandardDetection, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Get Result of Auto Standard Detection fail\n", __LINE__);
        //return;
    }
    return ((AUDIOSTANDARD_TYPE)u32param[1]);

#else

    return _MApi_AUDIO_SIF_GetResultOfAutoStandardDetection();

#endif
}

AUDIOSTANDARD_TYPE MApi_AUDIO_SIF_ConvertToBasicAudioStandard(AUDIOSTANDARD_TYPE eStandard)
{

#ifdef UTOPIA_20
    AUDIO_ST_SIF_SIF_CONVERTTOBASICAUDIOSTANDARD Status;
    Status.eRet = 0;
    Status.eStandard = eStandard;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Convert To Basic Audio Standard fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_ConvertToBasicAudioStandard, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Convert To Basic Audio Standard fail\n", __LINE__);
        //return;
    }

    return Status.eRet;

#else

    return _MApi_AUDIO_SIF_ConvertToBasicAudioStandard(eStandard);

#endif
}

void MApi_AUDIO_SIF_SetThreshold(THR_TBL_TYPE *ThrTbl)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Set Threshold fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_SetThreshold, (THR_TBL_TYPE *)ThrTbl) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Set Threshold fail\n", __LINE__);
        //return;
    }
    return;

#else

    return _MApi_AUDIO_SIF_SetThreshold(ThrTbl);

#endif
}

void MApi_AUDIO_SIF_SetPrescale(MS_U8 gain_type, MS_S32 db_value)
{

#ifdef UTOPIA_20
    AUDIO_ST_SIF_SETPRESCALE Status;
    Status.u8GainType = gain_type;
    Status.s32DbValue = db_value;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Set Prescale fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_SetPrescale, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Set Prescale fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SIF_SetPrescale(gain_type, db_value);

#endif
}

MS_BOOL MApi_AUDIO_SIF_IsPALType(AUDIO_SIF_PAL_TYPE pal_type)
{

#ifdef UTOPIA_20
    AUDIO_ST_SIF_SIF_ISPALTYPE Status;
    Status.bRet = 0;
    Status.ePalType = pal_type;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Is PAL Type fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_IsPALType, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Is PAL Type fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_SIF_IsPALType(pal_type);

#endif
}

void MApi_AUDIO_SIF_SetPALType(AUDIO_SIF_PAL_TYPE pal_type)
{

#ifdef UTOPIA_20

    MS_U32 u32Param = ((MS_U32)pal_type);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Set PAL Type fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_SetPALType, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Set PAL Type fail\n", __LINE__);
        //return;
    }
    return;

#else

    return _MApi_AUDIO_SIF_SetPALType(pal_type);

#endif
}

void MApi_AUDIO_SIF_SendCmd(En_AUD_SIF_CmdType enAudSifCommand, MS_U8 comm_arg1, MS_U8 comm_arg2)
{

#ifdef UTOPIA_20
    AUDIO_ST_SIF_SENDCMD Status;
    Status.eAudSifCommand = enAudSifCommand;
    Status.u8CommArg1 = comm_arg1;
    Status.u8CommArg2 = comm_arg2;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Send Cmd fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_SendCmd, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Send Cmd fail\n", __LINE__);
        //return;
    }
    return;

#else

    return _MApi_AUDIO_SIF_SendCmd(enAudSifCommand, comm_arg1, comm_arg2);

#endif
}

void MApi_AUDIO_SIF_Shift(En_AUD_VIF_Type type)
{

#ifdef UTOPIA_20

    MS_U32 u32Param = ((MS_U32)type);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Shift fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SIF_Shift, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SIF Shift fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SIF_Shift(type);

#endif
}

//============================================================
// AUDIO_FM_RADIO RELATIONAL API FUNCTION
//============================================================
#if 1
MS_U8 MApi_AUDIO_FM_RADIO_GetSoundMode(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] FM Radio Get Sound Mode fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_FM_RADIO_GetSoundMode, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] FM Radio Get Sound Mode fail\n", __LINE__);
        //return;
    }
    return ((MS_U8)u32param[1]);

#else

    return _MApi_AUDIO_FM_RADIO_GetSoundMode();

#endif
}

void MApi_AUDIO_FM_RADIO_SetSoundMode(MS_U8 u8FMRadioSoundMode)
{

#ifdef UTOPIA_20
    MS_U32 u32param = ((MS_U32)u8FMRadioSoundMode);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] FM Radio Set Sound Mode fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_FM_RADIO_SetSoundMode, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] FM Radio Set Sound Mode fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_FM_RADIO_SetSoundMode(u8FMRadioSoundMode);

#endif
}

void MApi_AUDIO_FM_RADIO_DeEmphassisOption(MS_BOOL Is_Europe)
{

#ifdef UTOPIA_20
    MS_U32 u32param = ((MS_U32)Is_Europe);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] FM Radio DeEmphassisOption fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_FM_RADIO_DeEmphassisOption, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] FM Radio DeEmphassisOption fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_FM_RADIO_DeEmphassisOption(Is_Europe);

#endif
}

MS_S8 MApi_AUDIO_FM_RADIO_GET_DC_AMP(void)
{

#ifdef UTOPIA_20
    MS_S32 s32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] FM Radio Get DC AMP fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_FM_RADIO_GetDC_Amp, (void*)s32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] FM Radio Get DC AMP fail\n", __LINE__);
        //return;
    }
    return ((MS_S8)s32param[1]);

#else

    return _MApi_AUDIO_FM_RADIO_GET_DC_AMP();

#endif
}

MS_U8 MApi_AUDIO_FM_RADIO_GET_NSR(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] FM Radio Get NSR fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_FM_RADIO_Get_NSR, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] FM Radio Get NSR fail\n", __LINE__);
        //return;
    }
    return ((MS_U8)u32param[1]);

#else

    return _MApi_AUDIO_FM_RADIO_GET_NSR();

#endif
}

void MApi_AUDIO_FM_RADIO_RESET(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] FM Radio Reset fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_FM_RADIO_Reset, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] FM Radio Reset fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_FM_RADIO_RESET();

#endif
}
#endif
//=============================================================
// AUDIO_SPDIF RELATIONAL API FUNCTION
//=============================================================
void MApi_AUDIO_SPDIF_HWEN(MS_BOOL spdif_en)
{

#ifdef UTOPIA_20
    AUDIO_ST_SPDIF_OP Status;
    Status.eType = SPDIF_HWEN;
    Status.spdif_hwen = spdif_en;
    Status.mute_en = FALSE;
    Status.spdif_mode = SPDIF_OUT_NONE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SPDIF HWEN fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SPDIF_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SPDIF HWEN fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SPDIF_HWEN(spdif_en);

#endif
}

void MApi_AUDIO_SPDIF_SetMute(MS_BOOL mute_en)
{

#ifdef UTOPIA_20
    AUDIO_ST_SPDIF_OP Status;
    Status.eType = SPDIF_SETMUTE;
    Status.spdif_hwen = FALSE;
    Status.mute_en = mute_en;
    Status.spdif_mode = SPDIF_OUT_NONE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SPDIF Set Mute fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SPDIF_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SPDIF Set Mute fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SPDIF_SetMute(mute_en);

#endif
}

void MApi_AUDIO_SPDIF_SetMode(MS_U8 spdif_mode)
{

#ifdef UTOPIA_20
    AUDIO_ST_SPDIF_OP Status;
    Status.eType = SPDIF_SETMODE;
    Status.spdif_hwen = FALSE;
    Status.mute_en = FALSE;
    Status.spdif_mode = spdif_mode;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SPDIF Set Mode fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SPDIF_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SPDIF Set Mode fail\n", __LINE__);
        return;
    }

    return;

#else

    return _MApi_AUDIO_SPDIF_SetMode(spdif_mode);

#endif
}

MS_U8 MApi_AUDIO_SPDIF_GetMode(void)
{

#ifdef UTOPIA_20
    AUDIO_ST_SPDIF_OP Status;
    Status.eType = SPDIF_GETMODE;
    Status.spdif_hwen = FALSE;
    Status.mute_en = FALSE;
    Status.spdif_mode = SPDIF_OUT_NONE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SPDIF Get Mode fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SPDIF_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SPDIF Get Mode fail\n", __LINE__);
        //return;
    }
    return Status.spdif_mode;

#else

    return _MApi_AUDIO_SPDIF_GetMode();

#endif
}

void MApi_AUDIO_SPDIF_SetSCMS(MS_U8 C_bit_en, MS_U8 L_bit_en)
{

#ifdef UTOPIA_20
    AUDIO_ST_SPDIF_SCMS Status;
    Status.bSetSCMS = TRUE;
    Status.C_bit_en = C_bit_en;
    Status.L_bit_en = L_bit_en;
    Status.Ret_bit = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SPDIF Set SCMS fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SPDIF_SCMS, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SPDIF Set SCMS fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SPDIF_SetSCMS(C_bit_en, L_bit_en);

#endif
}

MS_U8 MApi_AUDIO_SPDIF_GetSCMS(void)
{

#ifdef UTOPIA_20
    AUDIO_ST_SPDIF_SCMS Status;
    Status.bSetSCMS = FALSE;
    Status.C_bit_en = 0;
    Status.L_bit_en = 0;
    Status.Ret_bit = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SPDIF Get SCMS fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SPDIF_SCMS, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SPDIF Get SCMS fail\n", __LINE__);
        //return;
    }
    return Status.Ret_bit;

#else

    return _MApi_AUDIO_SPDIF_GetSCMS();

#endif
}

void MApi_Audio_SPDIF_Monitor(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SPDIF Monitor fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SPDIF_Monitor, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SPDIF Monitor fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_Audio_SPDIF_Monitor();

#endif
}

MS_BOOL MApi_AUDIO_SPDIF_ChannelStatus_CTRL(AUDIO_SPDIF_CS_TYPE cs_mode, AUDIO_SPDIF_CS_TYPE_STATUS status)
{

#ifdef UTOPIA_20
    AUDIO_ST_SPDIF_CHANNELSTATUS_CTRL Status;
    Status.bRet = 0;
    Status.eCsMode = cs_mode;
    Status.eStatus = status;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SPDIF Channel Status CTRL fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SPDIF_ChannelStatus_CTRL, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SPDIF Channel Status CTRL fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_SPDIF_ChannelStatus_CTRL(cs_mode, status);

#endif
}

MS_BOOL MApi_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus)
{

#ifdef UTOPIA_20
    AUDIO_ST_DIGITALOUT_SETCHANNELSTATUS Status;
    Status.bStatus = FALSE;
    Status.ePath = ePath;
    Status.stChannelStatus = stChannelStatus;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] DigitalOut Set Channel Status fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_DigitalOut_SetChannelStatus, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] DigitalOut Set Channel Status fail\n", __LINE__);
        //return;
    }
    return Status.bStatus;

#else

    return _MApi_AUDIO_DigitalOut_SetChannelStatus(ePath, stChannelStatus);

#endif
}

MS_BOOL MApi_AUDIO_DigitalOut_SetDeviceCapability(DIGITAL_OUTPUT_TYPE eoutType, AUDIO_FORMAT_CODE ecodecType, Digital_Out_Device_Capability_t *p_codecCapability)
{

#ifdef UTOPIA_20
    AUDIO_ST_DIGITALOUT_SETDEVICECAP Status;
    Status.bStatus = FALSE;
    Status.eoutType = eoutType;
    Status.ecodecType = ecodecType;
    Status.p_codecCapability = p_codecCapability;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] DigitalOut Set Device Capability fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_DigitalOut_SetDeviceCapability, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] DigitalOut Set DeviceCapability fail\n", __LINE__);
        //return;
    }
    return Status.bStatus;

#else
    return _MApi_AUDIO_DigitalOut_SetDeviceCapability(eoutType, ecodecType, p_codecCapability);
#endif
}

// unused
/*void MApi_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus)
{

#ifdef UTOPIA_20
    if (_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] DigitalOut Get Channel Status fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_DigitalOut_GetChannelStatus, (void*)stChannelStatus) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] DigitalOut Get Channel Status fail\n", __LINE__);
        //return;
    }
#else
    return _MApi_AUDIO_DigitalOut_GetChannelStatus(ePath, stChannelStatus);
#endif
}*/

//=============================================================
// AUDIO_HDMI RELATIONAL API FUNCTION
//=============================================================
void MApi_AUDIO_HDMI_Tx_SetMute(MS_BOOL mute_en)
{

#ifdef UTOPIA_20
    MS_U32 u32param = ((MS_U32)mute_en);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI Tx Set Mute fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_Tx_SetMute, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI Tx Set MuteL fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_HDMI_Tx_SetMute(mute_en);

#endif
}

void MApi_AUDIO_HDMI_Tx_GetStatus(MS_BOOL *onOff, AUDIO_FS_TYPE *hdmi_SmpFreq, HDMI_TX_OUTPUT_TYPE *outType )
{

#ifdef UTOPIA_20

    AUDIO_HDMI_TX_GETSTATUS Status;

    Status.onOff = FALSE;
    Status.hdmi_smpFreq = AUDIO_FS_NONE;
    Status.outType = HDMI_OUT_PCM;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI TX Get Status fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_Tx_GetStatus, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI TX Get Status fail\n", __LINE__);
        return;
    }
    *onOff = Status.onOff;
    *hdmi_SmpFreq = Status.hdmi_smpFreq;
    *outType = Status.outType;

    return;

#else

    return _MApi_AUDIO_HDMI_Tx_GetStatus(onOff, hdmi_SmpFreq, outType);

#endif
}

MS_U8 MApi_AUDIO_HDMI_Monitor(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI Monitor fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_Monitor, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI Monitor fail\n", __LINE__);
        //return;
    }
    return ((MS_U8)u32param[1]);

#else

    return _MApi_AUDIO_HDMI_Monitor();

#endif
}

MS_BOOL MApi_AUDIO_HDMI_GetNonpcmFlag(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI Get NonPCM Flag fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_GetNonpcmFlag, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI Get NonPCM Flag fail\n", __LINE__);
        //return;
    }
    return ((MS_BOOL)u32param[1]);

#else

    return _MApi_AUDIO_HDMI_GetNonpcmFlag();

#endif
}

void MApi_AUDIO_HDMI_SetNonpcm(MS_U8 nonPCM_en)     //[Q] should be replace by MApi_AUDIO_HDMI_RX_SetNonpcm
{

#ifdef UTOPIA_20
    MS_U32 u32param = ((MS_U32)nonPCM_en);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI Set NonPCM fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_SetNonpcm, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI Set NonPCM fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_HDMI_SetNonpcm(nonPCM_en);

#endif
}

AUDIO_HDMI_RX_TYPE MApi_AUDIO_HDMI_RX_SetNonpcm(MS_U8 nonPCM_en)
{

#ifdef UTOPIA_20
    AUDIO_ST_HDMI_NONPCM_OP Status;
    Status.bSet = TRUE;
    Status.nonPCM_en = nonPCM_en;
    Status.eRet = HDMI_RX_PCM;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI RX Set Non PCM fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_Nonpcm_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI RX Set Non PCM fail\n", __LINE__);
        //return;
    }
    return Status.eRet;

#else

    return _MApi_AUDIO_HDMI_RX_SetNonpcm(nonPCM_en);

#endif
}

AUDIO_HDMI_RX_TYPE MApi_AUDIO_HDMI_RX_GetNonPCM(void)
{

#ifdef UTOPIA_20
    AUDIO_ST_HDMI_NONPCM_OP Status;
    Status.bSet = FALSE;
    Status.nonPCM_en = 0;
    Status.eRet = HDMI_RX_PCM;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI RX Get Non PCM fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_Nonpcm_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI RX Get Non PCM fail\n", __LINE__);
        //return;
    }
    return Status.eRet;

#else

    return _MApi_AUDIO_HDMI_RX_GetNonPCM();

#endif
}

void MApi_AUDIO_DTV_HDMI_CFG(MS_U8 ctrl)
{

#ifdef UTOPIA_20
    MS_U32 u32param = ((MS_U32)ctrl);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] DTV HDMI CFG fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_DTV_HDMI_CFG, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] DTV HDMI CFG fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_DTV_HDMI_CFG(ctrl);

#endif
}

MS_U16 MApi_AUDIO_HDMI_GetSynthFreq(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI Get Synth Freq fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_GetSynthFreq, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI Get Synth Freq fail\n", __LINE__);
        //return;
    }
    return ((MS_U16)u32param[1]);

#else

    return _MApi_AUDIO_HDMI_GetSynthFreq();

#endif
}

void MApi_AUDIO_HDMI_SetDownSample(MS_U8 ratio)
{

#ifdef UTOPIA_20
    MS_U32 u32param = ((MS_U32)ratio);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI Set Down Sample fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_SetDownSample, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI Set Down Sample fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_HDMI_SetDownSample(ratio);

#endif
}

void MApi_AUDIO_HDMI_TX_SetMode(HDMI_TX_OUTPUT_TYPE outType)
{

#ifdef UTOPIA_20
    MS_U32 u32param = ((MS_U32)outType);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI TX Set Mode fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_TX_SetMode, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI TX Set Mode fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_HDMI_TX_SetMode(outType);

#endif
}

MS_BOOL MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus(ST_HDMI_AUDIO_STATUS *p_hdmiAudioSts)
{

#ifdef UTOPIA_20

    AUDIO_ST_RX_HDMI_AUDIO_STATUS Status;

    Status.bStatus = FALSE;
    Status.pHdmi_status = p_hdmiAudioSts;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI RX Get HDMI Audio Status fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_HDMI_RX_GetHdmiInAudioStatus, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] HDMI RX Get HDMI Audio Status fail\n", __LINE__);
        //return;
    }
    return Status.bStatus;

#else

    return _MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus(p_hdmiAudioSts);

#endif

}
//=============================================================
// AUDIO_ADVANCE_SOUND RELATIONAL API FUNCTION
//=============================================================
MS_BOOL MApi_AUDIO_ADVSOUND_ProcessEnable(ADVSND_TYPE type)
{

#ifdef UTOPIA_20
    AUDIO_ST_ADVSOUND_PROCESSENABLE Status;
    Status.bRet = 0;
    Status.eType = type;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ADVSOUND Precess Enable fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADVSOUND_ProcessEnable, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ADVSOUND Precess Enable fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_ADVSOUND_ProcessEnable(type);

#endif
}

MS_BOOL MApi_AUDIO_ADVSOUND_SubProcessEnable(ADVFUNC proc, MS_BOOL enable)
{

#ifdef UTOPIA_20
    AUDIO_ST_ADVSOUND_SUBPROCESSENABLE Status;
    Status.bRet = 0;
    Status.eProc = proc;
    Status.bEnable = enable;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ADVSOUND Sub Process Enable fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADVSOUND_SubProcessEnable, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ADVSOUND Sub Precess Enable fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_ADVSOUND_SubProcessEnable(proc, enable);

#endif
}

MS_BOOL MApi_AUDIO_ADVSOUND_SetParam(ADVSND_PARAM param, MS_U16 u16value1, MS_U16 u16value2)
{

#ifdef UTOPIA_20
    AUDIO_ST_ADVSOUND_SETPARAM Status;
    Status.bRet = 0;
    Status.eParam = param;
    Status.u16Value1 = u16value1;
    Status.u16Value2 = u16value2;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ADVSOUND Set Param fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADVSOUND_SetParam, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ADVSOUND Set Param fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_ADVSOUND_SetParam(param, u16value1, u16value2);

#endif
}

MS_BOOL MApi_AUDIO_ADVSND_SetParam(ADVSND_PARAM param, MS_U32 u32value1, MS_U16 u16value2)
{

#ifdef UTOPIA_20
    AUDIO_ST_ADVSND_SETPARAM Status;
    Status.bRet = 0;
    Status.eParam = param;
    Status.u32Value1 = u32value1;
    Status.u16Value2 = u16value2;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ADVSND Set Param fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADVSND_SetParam, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ADVSND Set Param fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_ADVSND_SetParam(param, u32value1, u16value2);

#endif
}

/*MS_U32 MApi_AUDIO_ADVSOUND_GetParam(ADVSND_GET_PARAM param)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0};
    u32param[1] = ((MS_U32)param);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ADVSOUND Get Param fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADVSOUND_GetParam, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ADVSOUND Get Param fail\n", __LINE__);
        //return;
    }
    return (MS_U32)u32param[0];

#else

    return _MApi_AUDIO_ADVSOUND_GetParam(param);

#endif
}

MS_BOOL MApi_AUDIO_ADVSOUND_SetASFParam(ADVSND_PARAM param, MS_S16 s16value1, MS_S16 s16value2)
{

#ifdef UTOPIA_20
    AUDIO_ST_ADVSND_SETASFPARAM Status;
    Status.bRet = 0;
    Status.eParam = param;
    Status.s16Value1 = s16value1;
    Status.s16Value2 = s16value2;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ADVSOUND Set ASF Param fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADVSOUND_SET_ASF_PARAM, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ADVSOUND Set ASF Param fail\n", __LINE__);
        //return;
    }
    return Status.bRet;
#else

    return _MApi_AUDIO_ADVSOUND_SetASFParam(param, s16value1, s16value2);

#endif
}

MS_U32 MApi_AUDIO_ADVSOUND_GetASFParam(ADVSND_GET_PARAM param)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0};
    u32param[1] = ((MS_U32)param);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ADVSOUND Get ASF Param fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADVSOUND_GET_ASF_PARAM, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ADVSOUND Get ASF Param fail\n", __LINE__);
        //return;
    }
    return (MS_U32)u32param[0];

#else

    return _MApi_AUDIO_ADVSOUND_GetASFParam(param);

#endif
}*/

MS_U32 MApi_AUDIO_ADVSOUND_GetInfo(AdvSound_GET_Type Type)
{

#ifdef UTOPIA_20
    AUDIO_ST_SRS_ADVSOUND_GETINFO Status;
    Status.u32Ret = 0;
    Status.eType = Type;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ADVSOUND Get Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADVSOUND_GetInfo, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ADVSOUND Get Info fail\n", __LINE__);
        //return;
    }

    return Status.u32Ret;

#else

    return _MApi_AUDIO_ADVSOUND_GetInfo(Type);

#endif
}

void MApi_DBXTV_SetMode(EN_DBXTV_TotSonMode totsonmode, EN_DBXTV_TotVolMode totvolmode, EN_DBXTV_TotSurMode totsurmode,MS_U32 enable)
{

#ifdef UTOPIA_20
    AUDIO_ST_DBXTV_SETMODE Status;
    Status.eTotSonMode = totsonmode;
    Status.eTotVolMode = totvolmode;
    Status.eTotSurMode = totsurmode;
    Status.u32Enable = enable;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] DBXTV Set Mode fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_DBXTV_SetMode, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] DBXTV Set Mode fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_DBXTV_SetMode(totsonmode, totvolmode, totsurmode, enable);

#endif
}

void MApi_AUDIO_SeInit(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Se Init fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SeInit, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Se Init fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SeInit();

#endif
}

void MApi_AUDIO_SetAdvSndSys(En_DVB_advsndType u8AdvSurrMod)
{

#ifdef UTOPIA_20
    AUDIO_ST_SETADVSNDSYS Status;
    Status.eType = u8AdvSurrMod;

    if (_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Adv Snd Sys fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetAdvSndSys, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Adv Snd Sys fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetAdvSndSys(u8AdvSurrMod);

#endif
}

/*void MApi_AUDIO_ADVSND_Set_Sound_Param(AdvSound_SET_Type Type, MS_U32 *pParam) // New api for cover below functions
{

#ifdef UTOPIA_20
    AUDIO_ST_ADV_SOUND_SETPARAM Status;
    Status.eType = Type;
    Status.pParam = pParam;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set VDS fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ADV_SOUND_SET, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set VDS fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_ADVSND_Set_Sound_Param(Type, pParam);

#endif

}*/

void MApi_AUDIO_SetVDS(MS_U8 u8enVDS)
{

#ifdef UTOPIA_20

    MS_U32 u32param = ((MS_U32)u8enVDS);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set VDS fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetVDS, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set VDS fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetVDS(u8enVDS);

#endif
}

void MApi_AUDIO_SetVSPK(MS_U8 u8enVSPK)
{

#ifdef UTOPIA_20

    MS_U32 u32param = ((MS_U32)u8enVSPK);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set VSPK fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetVSPK, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set VSPK fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetVSPK(u8enVSPK);

#endif
}

void MApi_AUDIO_SetSRS(MS_U8 u8enSRS)
{

#ifdef UTOPIA_20

    MS_U32 u32param = ((MS_U32)u8enSRS);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set SRS fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetSRS, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set SRS fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetSRS(u8enSRS);

#endif
}

void MApi_AUDIO_SetBBE(MS_U8 u8enBBE , MS_U8 u8BBEMode)
{

#ifdef UTOPIA_20
    AUDIO_ST_SETBBE Status;
    Status.u8EnBBE = u8enBBE;
    Status.u8BBEMode = u8BBEMode;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set BBE fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetBBE, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set BBE fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetBBE(u8enBBE, u8BBEMode);

#endif
}

void MApi_AUDIO_VSPK_WMod(MS_U8 u8VSPK_WMode)
{

#ifdef UTOPIA_20

    MS_U32 u32param = ((MS_U32)u8VSPK_WMode);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] VSPK WMod fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_VSPK_WMod, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] VSPK WMod fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_VSPK_WMod(u8VSPK_WMode);

#endif
}

void MApi_AUDIO_VSPK_SMod(MS_U8 u8VSPK_SMode)
{

#ifdef UTOPIA_20

    MS_U32 u32param = ((MS_U32)u8VSPK_SMode);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] VSPK SMod fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_VSPK_SMod, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] VSPK SMod fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_VSPK_SMod(u8VSPK_SMode);

#endif
}

void MApi_AUDIO_SRS_DC(MS_U8 u8SRSenDC)
{

#ifdef UTOPIA_20

    MS_U32 u32param = ((MS_U32)u8SRSenDC);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SRS DC fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SRS_DC, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SRS DC fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SRS_DC(u8SRSenDC);

#endif
}

void MApi_AUDIO_SRS_TruBass(MS_U8 u8SRSenTruBass)
{

#ifdef UTOPIA_20

    MS_U32 u32param = ((MS_U32)u8SRSenTruBass);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SRS TruBass fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SRS_TruBass, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SRS TruBass fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SRS_TruBass(u8SRSenTruBass);

#endif
}

void  MApi_AUDIO_SRS_SetTsxtPara(MS_U8 u8mode, MS_U8 u8value)
{

#ifdef UTOPIA_20
    AUDIO_ST_SRS_SETTSXTPARA Status;
    Status.u8Mode = u8mode;
	Status.u8Value = u8value;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SRS SetTsxtPara fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SRSTsxtPara, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SRS SetTsxtPara fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SRS_SetTsxtPara(u8mode, u8value);

#endif
}

void MApi_AUDIO_SetSRSTSHD(MS_U8 u8enTSHD)
{

#ifdef UTOPIA_20

    MS_U32 u32param = ((MS_U32)u8enTSHD);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set SRS TSHD fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetSRSTSHD, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set SRS TSHD fail\n", __LINE__);
        return;
    }
    return;

    #else

    return _MApi_AUDIO_SetSRSTSHD(u8enTSHD);

#endif
}

void  MApi_AUDIO_TSHD_TruBass(MS_U8 u8TSHDenTruBass)
{
#ifdef UTOPIA_20
    MS_U32 u32param = ((MS_U32)u8TSHDenTruBass);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] TSHD TreBass fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_TSHD_TruBass, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] TSHD TreBass fail\n", __LINE__);
        return;
    }
    return;

#else
    _MApi_AUDIO_TSHD_TruBass(u8TSHDenTruBass);
#endif

}

void  MApi_AUDIO_TSHD_Definition(MS_U8 u8TSHDenDefinition)
{

#ifdef UTOPIA_20

    MS_U32 u32param = ((MS_U32)u8TSHDenDefinition);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] TSHD Definition fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_TSHD_Definition, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] TSHD Definition fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_TSHD_Definition(u8TSHDenDefinition);

#endif
}

void  MApi_AUDIO_TSHD_Clarity(MS_U8 u8TSHDenClarity)
{

#ifdef UTOPIA_20

    MS_U32 u32param = ((MS_U32)u8TSHDenClarity);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] TSHD Clarity fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_TSHD_Clarity, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] TSHD Clarity fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_TSHD_Clarity(u8TSHDenClarity);

#endif
}

void  MApi_AUDIO_SRS_SetTshdPara(MS_U8 u8mode, MS_U8 u8value)
{

#ifdef UTOPIA_20
    AUDIO_ST_SRS_SETTSHDPARA Status;
    Status.u8Mode = u8mode;
    Status.u8Value = u8value;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SRS SetTshdPara fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SRSSetTshdPara, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] SRS SetTshdPara fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SRS_SetTshdPara(u8mode, u8value);

#endif
}

void MApi_AUDIO_COPY_Parameter(ADVSND_PARAM type, void* Parameter_ptr, MS_U32 size)
{

#ifdef UTOPIA_20
    AUDIO_ST_COPY_PARAMETER Parameter;
    void *pParam = NULL;
    MS_U32 type_size, parameter_size, total_size;

    parameter_size = size;
    #if defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
    type_size = strlen(type)+1;
    #else
    type_size = sizeof(ADVSND_TYPE);
    #endif
    total_size = size+type_size;

    pParam = malloc(total_size);
    if (pParam != NULL)
    {
        memcpy(pParam, Parameter_ptr, parameter_size);
        #if defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
        memcpy((void *)(((MS_U8 *)pParam)+parameter_size), (void *)type, type_size);
        #else
        memcpy((void *)(((MS_U8 *)pParam)+parameter_size), (void *)&type, type_size);
        #endif

        Parameter.Parameter_ptr = pParam;
        Parameter.parameter_size = parameter_size;
        Parameter.type_size = type_size;
    }
    else
    {
        return;
    }

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] COPY Parameter fail\n", __LINE__);
        goto exit;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Copy_Parameter, (void*)&Parameter) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] COPY Parameter fail\n", __LINE__);
        goto exit;
    }
exit:
    if (pParam != NULL)
    {
        free(pParam);
    }
    return;

#else

    return _MApi_AUDIO_COPY_Parameter(type, Parameter_ptr, size);

#endif
}

//=============================================================
// AUDIO_KTV RELATIONAL API FUNCTION
//=============================================================
MS_BOOL MApi_AUDIO_SetKTVInfo(AUDIO_KTV_MODEL modelType, AUDIO_KTV_PARAMTYPE paramType, MS_U32 param1, MS_U32 param2)
{

#ifdef UTOPIA_20
    AUDIO_ST_KTV_OP Status;
    Status.eType = KTV_SET_INFO;
    Status.modelType = modelType;
    Status.paramType = AUD_KTV_ParamType1;
    Status.param1 = param1;
    Status.param2 = param2;
    Status.bRet = FALSE;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set KTV Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_KTV_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set KTV Info fail\n", __LINE__);
        //return;
    }
    return Status.bRet;

#else

    return _MApi_AUDIO_SetKTVInfo(modelType, paramType, param1, param2);

#endif
}

MS_U32 MApi_AUDIO_GetKTVInfo(AUDIO_KTV_MODEL modelType, AUDIO_KTV_INFOTYPE infoType)
{

#ifdef UTOPIA_20
    AUDIO_ST_KTV_OP Status;
    Status.eType = KTV_GET_INFO;
    Status.modelType = modelType;
    Status.infoType = infoType;
    Status.u32Ret = 0;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set KTV Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_KTV_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set KTV Info fail\n", __LINE__);
        //return;
    }
    return Status.u32Ret;

#else

    return _MApi_AUDIO_GetKTVInfo(modelType, infoType);

#endif
}

void MApi_AUDIO_SetMixModeVolume(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_U8 u8Vol1, MS_U8 u8Vol2)
{

#ifdef UTOPIA_20
    AUDIO_ST_KTV_OP Status;
    Status.eType = KTV_SET_MIX_VOL;
    Status.eSourceType = eSourceType;
    Status.VolType = VolType;
    Status.u8Vol1 = u8Vol1;
    Status.u8Vol2 = u8Vol2;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Mix Mode Volume fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_KTV_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Mix Mode Volume fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetMixModeVolume(eSourceType, VolType, u8Vol1, u8Vol2);

#endif
}

void MApi_AUDIO_SetMixModeMute(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_BOOL EnMute)
{

#ifdef UTOPIA_20
    AUDIO_ST_KTV_OP Status;
    Status.eType = KTV_SET_MIX_MUTE;
    Status.eSourceType = eSourceType;
    Status.VolType = VolType;
    Status.EnMute = EnMute;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Mix Mode Mute fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_KTV_OP, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Mix Mode Mute fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetMixModeMute(eSourceType, VolType, EnMute);

#endif
}

void MApi_AUDIO_PlayMenuSound(void)
{

#ifdef UTOPIA_20
    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Play Menu Sound fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PlayMenuSound, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Play Menu Sound fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_PlayMenuSound();

#endif
}

//=============================================================
// AUDIO_MM RELATIONAL API FUNCTION
//=============================================================
MS_BOOL MApi_AUDIO_SetCommAudioInfo( Audio_COMM_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_COMMAUDIOINFO Status;
    Status.bRet = 0;
    Status.eInfoType = infoType;
    Status.Param1 = param1;
    Status.Param2 = param2;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Comm Audio Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetCommAudioInfo, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Comm Audio Info fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_SetCommAudioInfo(infoType, param1, param2);

#endif
}

#if 0
MS_BOOL MApi_ADEC_SetDecodeParam( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_U32* Param )
{
#ifdef UTOPIA_20
    AUDIO_ST_SETDECODEPARAM Status;
    Status.DecID = DecId;
    Status.paramType = paramType;
    Status.Param = Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Decode Param fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDecodeParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Decode Param fail\n", __LINE__);
        //return;
    }
    return Status.bStatus;

#else

    return _MApi_ADEC_SetDecodeParam(DecId, paramType, Param);

#endif

}
#endif

MS_BOOL MApi_AUDIO_SetMpegInfo( Audio_MPEG_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_MPEGINFO Status;
    Status.bRet = 0;
    Status.eInfoType = infoType;
    Status.Param1 = param1;
    Status.Param2 = param2;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Mpeg Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetMpegInfo, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Mpeg Info fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_SetMpegInfo(infoType, param1, param2);

#endif
}

MS_BOOL MApi_AUDIO_SetAC3Info(Audio_AC3_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_AC3INFO Status;
    Status.bRet = 0;
    Status.eInfoType = infoType;
    Status.Param1 = param1;
    Status.Param2 = param2;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set AC3 Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetAC3Info, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set AC3 Info fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_SetAC3Info(infoType, param1, param2);

#endif
}

MS_BOOL MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_AC3PINFO Status;
    Status.bRet = 0;
    Status.eInfoType = infoType;
    Status.Param1 = param1;
    Status.Param2 = param2;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set AC3P Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetAC3PInfo, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set AC3P Info fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_SetAC3PInfo(infoType, param1, param2);

#endif
}

MS_BOOL MApi_AUDIO_SetAACInfo( Audio_AAC_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_AACINFO Status;
    Status.bRet = 0;
    Status.eInfoType = infoType;
    Status.Param1 = param1;
    Status.Param2 = param2;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set AAC Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetAACInfo, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set AAC Info fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_SetAACInfo(infoType, param1, param2);

#endif
}

MS_BOOL MApi_AUDIO_SetWmaInfo(Audio_WMA_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_WMAINFO Status;
    Status.bRet = 0;
    Status.eInfoType = infoType;
    Status.Param1 = param1;
    Status.Param2 = param2;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set WMA Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetWMAInfo, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set WMA Info fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_SetWmaInfo(infoType, param1, param2);

#endif
}

MS_BOOL MApi_AUDIO_SetDTSCommonCtrl( Audio_DTS_infoType infoType, Audio_DTS_CtrlType ctrl_type )
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_DTSCOMMONCTRL Status;
    Status.bRet = 0;
    Status.eInfoType = infoType;
    Status.eCtrlType = ctrl_type;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set DTS Common Ctrl fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetDTSCommonCtrl, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set DTS Common Ctrl fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_SetDTSCommonCtrl(infoType, ctrl_type);

#endif
}

MS_U64 MApi_AUDIO_GetCommAudioInfo( Audio_COMM_infoType infoType )
{

#ifdef UTOPIA_20
    AUDIO_ST_GET_COMMAUDIOINFO Status;
    Status.u64Ret = 0;
    Status.eInfoType = infoType;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Comm Audio Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetCommAudioInfo, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Comm Audio Info fail\n", __LINE__);
        //return;
    }

    return Status.u64Ret;

#else

    return _MApi_AUDIO_GetCommAudioInfo(infoType);

#endif
}

MS_U32 MApi_AUDIO_GetMpegInfo( Audio_MPEG_infoType infoType )
{

#ifdef UTOPIA_20
    AUDIO_ST_GET_MPEGINFO Status;
    Status.u32Ret = 0;
    Status.eInfoType = infoType;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Mpeg Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetMpegInfo, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Mpeg Info fail\n", __LINE__);
        //return;
    }

    return Status.u32Ret;

#else

    return _MApi_AUDIO_GetMpegInfo(infoType);

#endif
}

MS_U32 MApi_AUDIO_GetAC3Info(Audio_AC3_infoType infoType )
{

#ifdef UTOPIA_20
    AUDIO_ST_GET_AC3INFO Status;
    Status.u32Ret = 0;
    Status.eInfoType = infoType;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get AC3 Info fail\n", __LINE__);
        //return;
    }

	if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetAC3Info, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get AC3 Info fail\n", __LINE__);
        //return;
    }

    return Status.u32Ret;

#else

    return _MApi_AUDIO_GetAC3Info(infoType);

#endif
}

MS_U32 MApi_AUDIO_GetAC3PInfo( Audio_AC3P_infoType infoType )
{

#ifdef UTOPIA_20
    AUDIO_ST_GET_AC3PINFO Status;
    Status.u32Ret = 0;
    Status.eInfoType = infoType;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get AC3P Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetAC3PInfo, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get AC3P Info fail\n", __LINE__);
        //return;
    }

    return Status.u32Ret;

#else

    return _MApi_AUDIO_GetAC3PInfo(infoType);

#endif
}

MS_U32 MApi_AUDIO_GetAACInfo( Audio_AAC_infoType infoType )
{

#ifdef UTOPIA_20
    AUDIO_ST_GET_AACINFO Status;
    Status.u32Ret = 0;
    Status.eInfoType = infoType;


    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get AAC Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetAACInfo, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get AAC Info fail\n", __LINE__);
        //return;
    }

    return Status.u32Ret;

#else

    return _MApi_AUDIO_GetAACInfo(infoType);

#endif
}

MS_U32 MApi_AUDIO_GetWmaInfo( Audio_WMA_infoType infoType )
{

#ifdef UTOPIA_20
    AUDIO_ST_GET_WMAINFO Status;
    Status.u32Ret = 0;
    Status.eInfoType = infoType;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Wma Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetWmaInfo, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Wma Info fail\n", __LINE__);
        //return;
    }

    return Status.u32Ret;

#else

    return _MApi_AUDIO_GetWmaInfo(infoType);

#endif
}

MS_U32 MApi_AUDIO_GetDTSInfo( Audio_DTS_infoType infoType )
{

#ifdef UTOPIA_20
    AUDIO_ST_GET_DTSINFO Status;
    Status.u32Ret = 0;
    Status.eInfoType = infoType;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get DTS Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetDTSInfo, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get DTS Info fail\n", __LINE__);
        //return;
    }

    return Status.u32Ret;

#else

    return _MApi_AUDIO_GetDTSInfo(infoType);

#endif
}

MS_U8 MApi_AUDIO_XPCM_Param(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate, MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
{

#ifdef UTOPIA_20
    AUDIO_ST_XPCMPARAM Status;
    Status.u8Ret = 0;
    Status.eAudioType = audioType;
    Status.u8Channels = channels;
    Status.u16SampleRate = sampleRate;
    Status.u8BitsPerSample = bitsPerSample;
    Status.u16BlockSize = blockSize;
    Status.u16SamplePerBlock = samplePerBlock;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] XPCM Param fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_XPCM_Param, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] XPCM Param fail\n", __LINE__);
        //return;
    }

    return Status.u8Ret;

#else

    return _MApi_AUDIO_XPCM_Param(audioType, channels, sampleRate, bitsPerSample, blockSize, samplePerBlock);

#endif
}

void MApi_AUDIO_XPCM2_Param(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate, MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_XPCMPARAM Status;
    Status.eAudioType = audioType;
    Status.u8Channels = channels;
    Status.u16SampleRate = sampleRate;
    Status.u8BitsPerSample = bitsPerSample;
    Status.u16BlockSize = blockSize;
    Status.u16SamplePerBlock = samplePerBlock;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] XPCM2 Param fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_XPCM2_Param, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] XPCM2 Param fail\n", __LINE__);
        //return;
    }
    return;

#else

    return _MApi_AUDIO_XPCM2_Param(audioType, channels, sampleRate, bitsPerSample, blockSize, samplePerBlock);

#endif
}

MS_U16 MApi_AUDIO_XPCM2_CheckIntStatus(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] XPCM2 Check Int Status fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_XPCM2_CheckIntStatus, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] XPCM2 Check Int Status fail\n", __LINE__);
        //return;
    }
    return (MS_U16)u32param[1];

#else

    return _MApi_AUDIO_XPCM2_CheckIntStatus();

#endif
}

MS_U8 MApi_AUDIO_RA8_Param(MS_U16 mNumCodecs, MS_U16 mSamples, MS_U16 mSampleRate, MS_U16* Channels, MS_U16* Regions, MS_U16* cplStart, MS_U16* cplQbits, MS_U16* FrameSize)
{

#ifdef UTOPIA_20
    AUDIO_ST_RA8_Param Status;
    Status.u8Status = FALSE;
    Status.mNumCodecs = mNumCodecs;
    Status.mSamples = mSamples;
    Status.mSampleRate= mSampleRate;
    Status.pChannels = Channels;
    Status.pRegions = Regions;
    Status.pcplStart = cplStart;
    Status.pcplQbits = cplQbits;
    Status.pFrameSize = FrameSize;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] RA8 Param fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_RA8_Param, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] RA8 Param fail\n", __LINE__);
        //return;
    }
    return Status.u8Status;

#else

    return _MApi_AUDIO_RA8_Param(mNumCodecs, mSamples, mSampleRate, Channels, Regions, cplStart, cplQbits,  FrameSize);

#endif
}

void MApi_AUDIO_Init(En_DVB_decSystemType enDecSystem)
{

#ifdef UTOPIA_20
    MS_U32 u32param =((MS_U32)enDecSystem);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Init fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Init, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Init fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_Init(enDecSystem);

#endif
}

void MApi_AUDIO_StartDecode(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Start Decode fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_StartDecode, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Start Decode fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_StartDecode();

#endif
}

void MApi_AUDIO_StartBrowse(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Start Broswe fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_StartBroswe, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Start Broswe fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_StartBrowse();

#endif
}

void MApi_AUDIO_StopDecode(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Stop Decode fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_StopDecode, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Stop Decode fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_StopDecode();

#endif
}

void MApi_AUDIO_PauseDecode(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Pause Decode fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PauseDecode, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Pause Decode fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_PauseDecode();

#endif
}

MS_U8 MApi_AUDIO_CheckPlayDone(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Check Play Done fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_CheckPlayDone, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Check Play Done fail\n", __LINE__);
        //return;
    }
    return (MS_U8)u32param[1];

#else

    return _MApi_AUDIO_CheckPlayDone();

#endif
}

MS_U16 MApi_AUDIO_GetResidualBufferSize(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Residual Buffer Size fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetResidualBufferSize, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Residual Buffer Size fail\n", __LINE__);
        //return;
    }
    return (MS_U16)u32param[1];

#else

    return _MApi_AUDIO_GetResidualBufferSize();

#endif
}

MS_U16 MApi_AUDIO_GetPCMBufferSize(MS_U16 u16BitRate)
{

#ifdef UTOPIA_20
    AUDIO_ST_GET_PCMBUFFERSIZE Status;
    Status.u16Ret = 0;
	Status.u16BitRate = u16BitRate;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get PCM Buffer Size fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetPCMBufferSize, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get PCM Buffer Size fail\n", __LINE__);
        //return;
    }

    return Status.u16Ret;

#else

    return _MApi_AUDIO_GetPCMBufferSize(u16BitRate);

#endif
}

MS_U16 MApi_AUDIO_GetPCMBufferSize2(MS_U16 u16BitRate,MS_U16 u16SampleRate)
{

#ifdef UTOPIA_20
    AUDIO_ST_GET_PCMBUFFERSIZE2 Status;
    Status.u16Ret = 0;
	Status.u16BitRate = u16BitRate;
	Status.u16SampleRate = u16SampleRate;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get PCM Buffer Size2 fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetPCMBufferSize2, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get PCM Buffer Size2 fail\n", __LINE__);
        //return;
    }

    return Status.u16Ret;

#else

    return _MApi_AUDIO_GetPCMBufferSize2(u16BitRate, u16SampleRate);

#endif
}

MS_U32 MApi_AUDIO_GetCurrentFrameNumber(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Current Frame Number fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetCurrentFrameNumber, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Current Frame Number fail\n", __LINE__);
        //return;
    }
    return (MS_U32)u32param[1];

#else

    return _MApi_AUDIO_GetCurrentFrameNumber();

#endif
}

MS_U16 MApi_AUDIO_GetSampleRate(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Sample Rate fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetSampleRates, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Sample Rate fail\n", __LINE__);
        //return;
    }
    return (MS_U16)u32param[1];

#else

    return _MApi_AUDIO_GetSampleRate();

#endif
}

MS_U32 MApi_AUDIO_GetBitRate(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Bit Rate fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetBitRates, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Bit Rate fail\n", __LINE__);
        //return;
    }
    return (MS_U32)u32param[1];

#else

    return _MApi_AUDIO_GetBitRate();

#endif
}

Audio_MP3_LAYER MApi_AUDIO_GetLayer(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Layer fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetLayer, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Layer fail\n", __LINE__);
        //return;
    }
    return (Audio_MP3_LAYER)u32param[1];

#else

    return _MApi_AUDIO_GetLayer();

#endif
}

MS_U8 MApi_AUDIO_CheckInputRequest(MS_PHY *pPHYWrtAddr, MS_U32 *pU32WrtBytes)
{

#ifdef UTOPIA_20
    AUDIO_ST_CHECK_INPUT_REQ Status;
    Status.u8Status = FALSE;
    Status.pPHYWrtAddr = pPHYWrtAddr;
    Status.pU32WrtBytes = pU32WrtBytes;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Check Input Request fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_CheckInputRequest, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Check Input Request fail\n", __LINE__);
        //return;
    }

    return Status.u8Status;

#else

    return _MApi_AUDIO_CheckInputRequest(pPHYWrtAddr, pU32WrtBytes);

#endif
}

void MApi_AUDIO_SetInput(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Input fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetInput, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Input fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetInput();

#endif
}

void MApi_AUDIO_SetSampleRateIndex(MS_U16 u16Index)
{

#ifdef UTOPIA_20
    MS_U32 u32param = ((MS_U32)u16Index);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Sample Rate Index fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetSampleRateIndex, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Sample Rate Index fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetSampleRateIndex(u16Index);

#endif
}

void MApi_AUDIO_SetXPCMParam(XPCM_TYPE audioType, MS_U8 channels, MS_U16 sampleRate, MS_U8 bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
{
#ifdef UTOPIA_20
    AUDIO_ST_SET_XPCMPARAM Status;
    Status.eAudioType = audioType;
    Status.u8Channels = channels;
    Status.u16SampleRate = sampleRate;
    Status.u8BitsPerSample = bitsPerSample;
    Status.u16BlockSize = blockSize;
    Status.u16SamplePerBlock = samplePerBlock;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set XPCM Param fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetXPCMParam, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set XPCM Param fail\n", __LINE__);
        //return;
    }
    return;

#else

    return _MApi_AUDIO_SetXPCMParam(audioType, channels, sampleRate, bitsPerSample, blockSize, samplePerBlock);

#endif
}

void MApi_AUDIO_FileEndNotification(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] File End Notification fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_FileEndNotification, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] File End Notification fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_FileEndNotification();

#endif
}

void MApi_AUDIO_FileEndDataHandle(MS_U32 u32DataLeft)
{

#ifdef UTOPIA_20
    MS_U32 u32param = ((MS_U32)u32DataLeft);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] File End Data Handle fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_FileEndDataHandle, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] File End Data Handle fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_FileEndDataHandle(u32DataLeft);

#endif
}

MS_U32 MApi_AUDIO_GetPlayTick(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Play Tick fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetPlayTick, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Play Tick fail\n", __LINE__);
        //return;
    }
    return u32param[1];

#else

    return _MApi_AUDIO_GetPlayTick();

#endif
}

MS_U16 MApi_AUDIO_GetEsMEMCnt(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Es MEM Cnt fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetEsMEMCnt, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Es MEM Cnt fail\n", __LINE__);
        //return;
    }
    return (MS_U16)u32param[1];

#else

    return _MApi_AUDIO_GetEsMEMCnt();

#endif
}

void MApi_AUDIO_SetASFParm(WMA_ASF_PARMTYPE parm_type, MS_U32 value)
{

#ifdef UTOPIA_20
	AUDIO_ST_SET_ASFPARM Status;
	Status.eParmType = parm_type;
    Status.u32Value = value;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set ASF Parm fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetASFParm, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set ASF Parm fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_SetASFParm(parm_type, value);

#endif
}

void MApi_AUDIO_MM_SetInput (En_DVB_AudioDecoder AUDDecoderNo, MS_U8 u8IntTag)
{

#ifdef UTOPIA_20
	AUDIO_ST_MM_SETINPUT Status;
	Status.eAudDecoderNo = AUDDecoderNo;
    Status.u8IntTag = u8IntTag;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] MM Set Input fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_MM_SetInput, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] MM Set Input fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_MM_SetInput(AUDDecoderNo, u8IntTag);

#endif
}

MS_U32 MApi_AUDIO_MM_CheckPlayDone (En_DVB_AudioDecoder AUDDecoderNo)
{

#ifdef UTOPIA_20
	AUDIO_ST_MM_CHECKPLAYDONE Status;
	Status.u32Ret = 0;
	Status.eAudDecoderNo = AUDDecoderNo;


    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] MM Check Play Done fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_MM_CheckPlayDone, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] MM Check Play Done fail\n", __LINE__);
        //return;
    }

    return Status.u32Ret;

#else

    return _MApi_AUDIO_MM_CheckPlayDone(AUDDecoderNo);

#endif
}

MS_U8 MApi_AUDIO_MM_CheckInputRequest(En_DVB_AudioDecoder AUDDecoderNo, MS_PHY *pPHYWrtAddr, MS_U32 *pU32WrtBytes)
{

#ifdef UTOPIA_20
    AUDIO_ST_MM_CHECK_INPUT_REQ Status;
    Status.u8Status = FALSE;
    Status.AUDDecoderNo = AUDDecoderNo;
    Status.pPHYWrtAddr = pPHYWrtAddr;
    Status.pU32WrtBytes = pU32WrtBytes;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] MM Check Input Request fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_MM_CheckInputRequest, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] MM Check Input Request fail\n", __LINE__);
        //return;
    }
    return Status.u8Status;

#else

    return _MApi_AUDIO_MM_CheckInputRequest(AUDDecoderNo, pPHYWrtAddr, pU32WrtBytes);

#endif
}

void MApi_AUDIO_DmaReader_Init(SAMPLE_RATE sampleRate)
{

#ifdef UTOPIA_20
    MS_U32 u32param = ((MS_U32)sampleRate);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Dma Reader Init fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_DmaReader_Init, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Dma Reader Init fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_DmaReader_Init(sampleRate);

#endif
}

void MApi_AUDIO_DmaReader_AllInput_Init(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Dma Reader All Input Init fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_DmaReader_AllInput_Init, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Dma Reader All Input Init fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_DmaReader_AllInput_Init();

#endif
}

MS_BOOL MApi_AUDIO_DmaReader_WritePCM(void* buffer, MS_U32 bytes)
{

#ifdef UTOPIA_20
    AUDIO_ST_DMAREADER_WRITEPCM Status;
    Status.bStatus = FALSE;
    Status.pBuffer = buffer;
    Status.bytes = bytes;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Dma Reader WritePCM fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_DmaReader_WritePCM, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Dma Reader WritePCM fail\n", __LINE__);
        //return;
    }
    return Status.bStatus;

#else

    return _MApi_AUDIO_DmaReader_WritePCM(buffer, bytes);

#endif
}

void MApi_AUDIO_DmaWriter_Init(void)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Dma Writer Init fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_DmaWrite_Init, 0) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Dma Writer Init fail\n", __LINE__);
        return;
    }
    return ;

#else

    return _MApi_AUDIO_DmaWriter_Init();

#endif
}

void MApi_AUDIO_USBPCM_Enable(MS_BOOL bEnable)
{

#ifdef UTOPIA_20
    MS_U32 u32param = ((MS_U32)bEnable);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] USBPCM Enable fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_USBPCM_Enable, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] USBPCM Enable fail\n", __LINE__);
        return;
    }
    return ;

#else

    return _MApi_AUDIO_USBPCM_Enable(bEnable);

#endif
}

void MApi_AUDIO_USBPCM_SetFlag(MS_BOOL bEnable)
{

#ifdef UTOPIA_20
    MS_U32 u32param = ((MS_U32)bEnable);

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] USBPCM SetFlag fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_USBPCM_SetFlag, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] USBPCM SetFlag fail\n", __LINE__);
        return;
    }
    return ;

#else

    return _MApi_AUDIO_USBPCM_SetFlag(bEnable);

#endif
}

MS_BOOL MApi_AUDIO_USBPCM_GetFlag(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] USBPCM GetFlag fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_USBPCM_GetFlag, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] USBPCM GetFlag fail\n", __LINE__);
        //return;
    }
    return (MS_BOOL)u32param[1];

#else

    return _MApi_AUDIO_USBPCM_GetFlag();

#endif
}

void MApi_AUDIO_USBPCM_GetMemInfo(AUDIO_UPLOAD_INFO *uploadInfo)
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] USBPCM Get Mem Info fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_USBPCM_GetMemInfo, (void*)uploadInfo) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] USBPCM Get Mem Info fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_USBPCM_GetMemInfo(uploadInfo);

#endif
}

MS_BOOL MApi_AUDIO_PCMCapture_Init(const AUDIO_DEVICE_TYPE eID, const AUDIO_CAPTURE_SOURCE_TYPE eSource)
{

#ifdef UTOPIA_20
    AUDIO_ST_PCMCAPTURE_INIT Status;
    Status.bStatus = FALSE;
    Status.eID = eID;
    Status.eSource = eSource;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Capture Init fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PCMCapture_Init, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Capture Init fail\n", __LINE__);
        //return;
    }
    return Status.bStatus;

#else

    return _MApi_AUDIO_PCMCapture_Init(eID, eSource);

#endif
}

MS_BOOL MApi_AUDIO_PCMCapture_Start(const AUDIO_DEVICE_TYPE eID)
{

#ifdef UTOPIA_20
    AUDIO_ST_PCMCAPTURE_INIT Status;
    Status.bStatus = FALSE;
    Status.eID = eID;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Capture Start fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PCMCapture_Start, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Capture Start fail\n", __LINE__);
        //return;
    }
    return Status.bStatus;

#else

    return _MApi_AUDIO_PCMCapture_Start(eID);

#endif
}

MS_BOOL MApi_AUDIO_PCMCapture_Stop(const AUDIO_DEVICE_TYPE eID)
{

#ifdef UTOPIA_20
    AUDIO_ST_PCMCAPTURE_INIT Status;
    Status.bStatus = FALSE;
    Status.eID = eID;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Capture Stop fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PCMCapture_Stop, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Capture Stop fail\n", __LINE__);
        //return;
    }
    return Status.bStatus;

#else

    return _MApi_AUDIO_PCMCapture_Stop(eID);

#endif
}

MS_BOOL MApi_AUDIO_PCMCapture_Read(const AUDIO_DEVICE_TYPE eID, void *buffer, const MS_U32 bytes)
{

#ifdef UTOPIA_20
    AUDIO_ST_PCMCAPTURE_READ Status;
    Status.bStatus = FALSE;
    Status.eID = eID;
    Status.pBuffer = buffer;
    Status.bytes = bytes;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Capture Read fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PCMCapture_Read, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Capture Read fail\n", __LINE__);
        //return;
    }
    return Status.bStatus;

#else

    return _MApi_AUDIO_PCMCapture_Read(eID, buffer, bytes);

#endif
}

MS_BOOL MApi_AUDIO_VoIP_Config(AUDIO_VoIP_CONFIG_TYPE configType, MS_U32 param1, MS_U32 param2)
{

#ifdef UTOPIA_20
    AUDIO_ST_VOIP_CONFIG Status;
    Status.bRet = 0;
    Status.eConfigType = configType;
    Status.u32Param1 = param1;
    Status.u32Param2 = param2;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] VoIP Config fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_VoIP_Config, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] VoIP Config fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_VoIP_Config(configType, param1, param2);

#endif
}

MS_BOOL MApi_AUDIO_ALSA_Check(void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ALSA Check fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ALSA_Check, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ALSA Check fail\n", __LINE__);
        //return;
    }
    return (MS_BOOL)u32param[1];

#else

    return _MApi_AUDIO_ALSA_Check();

#endif
}

MS_BOOL MApi_AUDIO_ALSA_Enable(MS_BOOL bEnable)
{

#ifdef UTOPIA_20
    AUDIO_ST_ALSA_ENABLE Status;
    Status.bRet = 0;
    Status.bEnable = bEnable;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ALSA Enable fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_ALSA_Enable, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] ALSA Enable fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_ALSA_Enable(bEnable);

#endif
}

MS_BOOL MApi_AUDIO_UNI_CheckDecodeDone(MS_PHY *pPHYWrtAddr, MS_U32 *pU32WrtBytes)
{

#ifdef UTOPIA_20
    AUDIO_ST_UNI_CHECK_DECODEDONE Status;
    Status.bStatus = FALSE;
    Status.pPHYWrtAddr = pPHYWrtAddr;
    Status.pU32WrtBytes = pU32WrtBytes;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] UNI Check Decode Done fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_UNI_CheckDecodeDone, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] UNI Check Decode Done fail\n", __LINE__);
        //return;
    }

    return Status.bStatus;

#else

    return _MApi_AUDIO_UNI_CheckDecodeDone(pPHYWrtAddr, pU32WrtBytes);

#endif
}

void MApi_AUDIO_UNI_SetOutput (MS_U32 PCMOutCnt)
{

#ifdef UTOPIA_20
    MS_U32 u32param = PCMOutCnt;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] UNI Set Output fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_UNI_SetOutput, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] UNI Set Output fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_UNI_SetOutput(PCMOutCnt);

#endif
}

void MApi_AUDIO_UNI_Set_PCMInputWriteAddr (MS_U32 PCMIn_Wptr)
{

#ifdef UTOPIA_20
    MS_U32 u32param = PCMIn_Wptr;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] UNI Set PCM Input Write Addr fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_UNI_Set_PCMInputWriteAddr, (void*)&u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] UNI Set PCM Input Write Addr fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_UNI_Set_PCMInputWriteAddr(PCMIn_Wptr);

#endif
}

MS_U32 MApi_AUDIO_UNI_Get_OutPCMLevel (void)
{

#ifdef UTOPIA_20
    MS_U32 u32param[2] = {0, 0};

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] UNI Get Out PCM Level fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_UNI_Get_OutPCMLevel, (void*)u32param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] UNI Get Out PCM Level fail\n", __LINE__);
        //return;
    }
    return (MS_BOOL)u32param[1];

#else

    return _MApi_AUDIO_UNI_Get_OutPCMLevel();

#endif
}

void MApi_AUDIO_RingTask(audioRingTask* auRingTask)        // this function is used for auto ring address
{

#ifdef UTOPIA_20

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Ring Task fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_RingTask, (void*)auRingTask) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Ring Task fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_RingTask(auRingTask);

#endif
}

void MApi_AUDIO_Ring_DataTransfer(audioRingTask* auRingTask, audioRingTask* auRingTask_1)
{

#ifdef UTOPIA_20
    AUDIO_ST_RING_DATA_TRANS st_auRingTask;
    st_auRingTask.pauRingTask = auRingTask;
    st_auRingTask.pauRingTask_1 = auRingTask_1;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Ring Task Data Transfer fail\n", __LINE__);
        return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Ring_DataTransfer, (void*)&st_auRingTask) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Ring Task Data Transfer fail\n", __LINE__);
        return;
    }
    return;

#else

    return _MApi_AUDIO_Ring_DataTransfer(auRingTask, auRingTask_1);

#endif
}

MS_BOOL MApi_AUDIO_MM2_initAesInfo( AUDIO_DEC_ID dec_id )
{

#ifdef UTOPIA_20
    AUDIO_ST_MM2_INITAESINFO Status;
    Status.bRet = 0;
    Status.eDecId = dec_id;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] MM2 Init Aes Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_MM2_InitAesInfo, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] MM2 Init Aes Info fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_MM2_initAesInfo(dec_id);

#endif
}

MS_BOOL MApi_AUDIO_MM2_checkAesInfo( AUDIO_DEC_ID  dec_id, AES_INFO *aes_info )
{

#ifdef UTOPIA_20
    AUDIO_ST_MM2_CHECKAESINFO Status;
    Status.dec_id = dec_id;
    Status.paes_info = aes_info;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] MM2 Check Aes Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_MM2_CheckAesInfo, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] MM2 Check Aes Info fail\n", __LINE__);
        //return;
    }

    return TRUE;

#else

    return _MApi_AUDIO_MM2_checkAesInfo(dec_id, aes_info);

#endif
}

MS_BOOL MApi_AUDIO_MM2_inputAesFinished( AUDIO_DEC_ID dec_id, MS_U32 es_size, MS_BOOL ptsExist, MS_U64 pts )
{

#ifdef UTOPIA_20
    AUDIO_ST_MM2_INPUTAES_FINISH Status;
    Status.bStatus = FALSE;
    Status.dec_id = dec_id;
    Status.es_size = es_size;
    Status.ptsExist = ptsExist;
    Status.pts = pts;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] MM2 Input Aes Finished fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_MM2_InputAesFinished, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] MM2 Input Aes Finished fail\n", __LINE__);
        //return;
    }
    return Status.bStatus;

#else

    return _MApi_AUDIO_MM2_inputAesFinished(dec_id, es_size, ptsExist, pts);

#endif
}

MS_BOOL MApi_AUDIO_SetAudioParam2( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_VIRT Param )
{

#ifdef UTOPIA_20
    AUDIO_ST_SET_AUDIOPARAM2 Status;
    Status.bRet = 0;
    Status.eDecId = DecId;
    Status.eParamType = paramType;
    Status.Param = Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Audio Param2 fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_SetAudioParam2, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Set Audio Param2 fail\n", __LINE__);
        //return;
    }

    return Status.bRet;

#else

    return _MApi_AUDIO_SetAudioParam2(DecId, paramType, Param );

#endif
}

MS_BOOL MApi_AUDIO_GetAudioInfo2(AUDIO_DEC_ID DecId, Audio_InfoType infoType, void * pInfo )
{

#ifdef UTOPIA_20
    AUDIO_ST_GETAUDIOINFO2 Status;
    Status.bStatus = FALSE;
    Status.DecId = DecId;
    Status.infoType = infoType;
    Status.pInfo = pInfo;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Audio Info2 fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetAudioInfo2, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Audio Info2 fail\n", __LINE__);
        //return;
    }
    return Status.bStatus;

#else

    return _MApi_AUDIO_GetAudioInfo2(DecId, infoType, pInfo);

#endif
}

MS_PHY MApi_AUDIO_GetDDRInfo(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo)
{

#ifdef UTOPIA_20
    AUDIO_ST_GET_DDRINFO Status;
    Status.Ret = 0;
    Status.eDecId = DecId;
    Status.eDdrInfo = DDRInfo;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get DDR Info fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetDDRInfo, &Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get DDR Info fail\n", __LINE__);
        //return;
    }

    return Status.Ret;

#else

    return _MApi_AUDIO_GetDDRInfo(DecId, DDRInfo);

#endif
}

MS_U32 MApi_AUDIO_GetCaps(AUDIO_DEC_ID DecId, MS_U32 *pCaps)
{

#ifdef UTOPIA_20
    AUDIO_ST_GETCAPS Status;
    Status.u32Ret = 0;
    Status.eDecId= DecId;
    Status.pu32Caps= pCaps;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Caps fail\n", __LINE__);
        //return;
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_GetCaps, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Get Caps fail\n", __LINE__);
        //return;
    }
    return Status.u32Ret;

#else

    return _MApi_AUDIO_GetCaps(DecId, pCaps);

#endif
}

MS_S32 MApi_AUDIO_PCM_Open(void *pData)
{

#ifdef UTOPIA_20
    AUDIO_ST_PCM_OPEN Status;
    Status.s32Ret = 0;
    Status.pData = pData;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Open fail\n", __LINE__);
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PcmOpen, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Open fail\n", __LINE__);
    }
    return Status.s32Ret;

#else

    return _MApi_AUDIO_PCM_Open(pData);

#endif
}

MS_S32 MApi_AUDIO_PCM_Close(MS_S32 s32DeviceId)
{

#ifdef UTOPIA_20
    AUDIO_ST_PCM_CLOSE Status;
    Status.s32Ret = 0;
    Status.s32DeviceId = s32DeviceId;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Close fail\n", __LINE__);
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PcmClose, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Close fail\n", __LINE__);
    }
    return Status.s32Ret;

#else

    return _MApi_AUDIO_PCM_Close(s32DeviceId);

#endif
}

MS_S32 MApi_AUDIO_PCM_Start(MS_S32 s32DeviceId)
{

#ifdef UTOPIA_20
    AUDIO_ST_PCM_START Status;
    Status.s32Ret = 0;
    Status.s32DeviceId = s32DeviceId;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Start fail\n", __LINE__);
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PcmStart, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Start fail\n", __LINE__);
    }
    return Status.s32Ret;

#else

    return _MApi_AUDIO_PCM_Start(s32DeviceId);

#endif
}

MS_S32 MApi_AUDIO_PCM_Stop(MS_S32 s32DeviceId)
{

#ifdef UTOPIA_20
    AUDIO_ST_PCM_STOP Status;
    Status.s32Ret = 0;
    Status.s32DeviceId = s32DeviceId;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Stop fail\n", __LINE__);
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PcmStop, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Stop fail\n", __LINE__);
    }
    return Status.s32Ret;

#else

    return _MApi_AUDIO_PCM_Stop(s32DeviceId);

#endif
}

MS_S32 MApi_AUDIO_PCM_Set(MS_S32 s32DeviceId, MS_U32 u32Cmd, const void *pData)
{

#ifdef UTOPIA_20
    AUDIO_ST_PCM_SET Status;
    Status.s32Ret = 0;
    Status.s32DeviceId = s32DeviceId;
    Status.u32Cmd = u32Cmd;
    Status.pData = pData;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Set fail\n", __LINE__);
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PcmSet, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Set fail\n", __LINE__);
    }
    return Status.s32Ret;

#else

    return _MApi_AUDIO_PCM_Set(s32DeviceId, u32Cmd, pData);

#endif
}

MS_S32 MApi_AUDIO_PCM_Get(MS_S32 s32DeviceId, MS_U32 u32Cmd, void *pData)
{

#ifdef UTOPIA_20
    AUDIO_ST_PCM_GET Status;
    Status.s32Ret = 0;
    Status.s32DeviceId = s32DeviceId;
    Status.u32Cmd = u32Cmd;
    Status.pData = pData;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Get fail\n", __LINE__);
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PcmGet, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Get fail\n", __LINE__);
    }
    return Status.s32Ret;

#else

    return _MApi_AUDIO_PCM_Get(s32DeviceId, u32Cmd, pData);

#endif
}

MS_U32 MApi_AUDIO_PCM_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size)
{

#ifdef UTOPIA_20
    AUDIO_ST_PCM_READ Status;
    Status.u32Ret = 0;
    Status.s32DeviceId = s32DeviceId;
    Status.u32Size = u32Size;
    Status.pBuf = pBuf;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Read fail\n", __LINE__);
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PcmRead, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Read fail\n", __LINE__);
    }
    return Status.u32Ret;

#else

    return _MApi_AUDIO_PCM_Read(s32DeviceId, pBuf, u32Size);

#endif
}

MS_U32 MApi_AUDIO_PCM_Write(MS_S32 s32DeviceId, const void *pBuf, MS_U32 u32Size)
{

#ifdef UTOPIA_20
    AUDIO_ST_PCM_WRITE Status;
    Status.u32Ret = 0;
    Status.s32DeviceId = s32DeviceId;
    Status.u32Size = u32Size;
    Status.pBuf = pBuf;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Write fail\n", __LINE__);
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PcmWrite, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Write fail\n", __LINE__);
    }
    return Status.u32Ret;

#else

    return _MApi_AUDIO_PCM_Write(s32DeviceId, pBuf, u32Size);

#endif
}

MS_S32 MApi_AUDIO_PCM_Flush(MS_S32 s32DeviceId)
{

#ifdef UTOPIA_20
    AUDIO_ST_PCM_FLUSH Status;
    Status.s32Ret = 0;
    Status.s32DeviceId = s32DeviceId;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Flush fail\n", __LINE__);
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_PcmFlush, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] PCM Flush fail\n", __LINE__);
    }
    return Status.s32Ret;

#else

    return _MApi_AUDIO_PCM_Flush(s32DeviceId);

#endif
}

MS_S32 MApi_AUDIO_ENCODER_Open(const char *pName)
{

#ifdef UTOPIA_20
    AUDIO_ST_ENCODER_OPEN Status;
    Status.s32Ret = 0;
    Status.pName = pName;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Encoder Open fail\n", __LINE__);
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_EncoderOpen, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Encoder Open fail\n", __LINE__);
    }
    return Status.s32Ret;

#else

    return _MApi_AUDIO_ENCODER_Open(pName);

#endif
}

MS_S32 MApi_AUDIO_ENCODER_Close(MS_S32 s32DeviceId)
{

#ifdef UTOPIA_20
    AUDIO_ST_ENCODER_CLOSE Status;
    Status.s32Ret = 0;
    Status.s32DeviceId = s32DeviceId;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Encoder Close fail\n", __LINE__);
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_EncoderClose, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Encoder Close fail\n", __LINE__);
    }
    return Status.s32Ret;

#else

    return _MApi_AUDIO_ENCODER_Close(s32DeviceId);

#endif
}

MS_S32 MApi_AUDIO_ENCODER_Start(MS_S32 s32DeviceId)
{

#ifdef UTOPIA_20
    AUDIO_ST_ENCODER_START Status;
    Status.s32Ret = 0;
    Status.s32DeviceId = s32DeviceId;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Encoder Start fail\n", __LINE__);
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_EncoderStart, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Encoder Start fail\n", __LINE__);
    }
    return Status.s32Ret;

#else

    return _MApi_AUDIO_ENCODER_Start(s32DeviceId);

#endif
}

MS_S32 MApi_AUDIO_ENCODER_Stop(MS_S32 s32DeviceId)
{

#ifdef UTOPIA_20
    AUDIO_ST_ENCODER_STOP Status;
    Status.s32Ret = 0;
    Status.s32DeviceId = s32DeviceId;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Encoder Stop fail\n", __LINE__);
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_EncoderStop, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Encoder Stop fail\n", __LINE__);
    }
    return Status.s32Ret;

#else

    return _MApi_AUDIO_ENCODER_Stop(s32DeviceId);

#endif
}

MS_S32 MApi_AUDIO_ENCODER_Ioctl(MS_S32 s32DeviceId, const char *pCmd, void *pData)
{

#ifdef UTOPIA_20
    AUDIO_ST_ENCODER_IOCTL Status;
    Status.s32Ret = 0;
    Status.s32DeviceId = s32DeviceId;
    Status.pCmd = pCmd;
    Status.pData = pData;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Encoder Get fail\n", __LINE__);
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_EncoderIoctl, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Encoder Get fail\n", __LINE__);
    }
    return Status.s32Ret;

#else

    return _MApi_AUDIO_ENCODER_Ioctl(s32DeviceId, pCmd, pData);

#endif
}

MS_U32 MApi_AUDIO_ENCODER_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size)
{

#ifdef UTOPIA_20
    AUDIO_ST_ENCODER_READ Status;
    Status.u32Ret = 0;
    Status.s32DeviceId = s32DeviceId;
    Status.u32Size = u32Size;
    Status.pBuf = pBuf;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Encoder Read fail\n", __LINE__);
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_EncoderRead, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Encoder Read fail\n", __LINE__);
    }
    return Status.u32Ret;

#else

    return _MApi_AUDIO_ENCODER_Read(s32DeviceId, pBuf, u32Size);

#endif
}

MS_S32 MApi_AUDIO_ENCODER_Flush(MS_S32 s32DeviceId)
{

#ifdef UTOPIA_20
    AUDIO_ST_ENCODER_FLUSH Status;
    Status.s32Ret = 0;
    Status.s32DeviceId = s32DeviceId;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Encoder Flush fail\n", __LINE__);
    }

    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_EncoderFlush, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Encoder Flush fail\n", __LINE__);
    }
    return Status.s32Ret;

#else

    return _MApi_AUDIO_ENCODER_Flush(s32DeviceId);

#endif
}

#if 0
////     TEST   Block   ////
#define TEST_GOP1 0
#define TEST_GOP2 0
#define TEST_GOP3 0
#define TEST_GOP4 0
#define TEST_GOP5 0
#define TEST_GOP7 0
#define TEST_GOP8 0
#define TEST_GOP9 0
#define TEST_GOP10 0
#define TEST_GOP11 0

int VT_Count = 0;

void MApi_AUDIO_EnableAutoVolume(MS_BOOL EnAVC)
{
    //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] %d\n", __FUNCTION__, __LINE__);

#ifdef UTOPIA_20
    MS_U32 u32Param[4] = {0,0,0,0};
    u32Param[0] = EnAVC;
    AUDIO_ST_SOUND_SETPARAM Status;
    Status.bStatus = FALSE;
    Status.eType = Sound_SET_Type_EnableAutoVolume;
    Status.pParam = (MS_U32*)u32Param;

    if(_AUDIO_IOCtrlOpen() == FALSE)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable Auto Volume fail\n", __LINE__);
        return;
    }

    #if 1
    if(VT_Count < 5)
    {
        if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_Sound_SetParam, (void*)&Status) != UTOPIA_STATUS_SUCCESS)
        {
            ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable Auto Volume fail\n", __LINE__);
            return;
        }
        VT_Count++;

    }
    else
    {

        // Test block
        VT_Count++;
        // Test block end

        #if (TEST_GOP1 == 1)        //audio system
        MS_U16 u16Data = 0;
        MApi_AUDIO_SetPowerOn(TRUE);
        u16Data = MApi_AUDIO_ReadMailBox(0, 5);
        MApi_AUDIO_WriteMailBox(0, 5, u16Data);
        MApi_AUDIO_FwTriggerDSP(0);
        MApi_AUDIO_SetFwStatus(TRUE);
        MApi_AUDIO_I2S_SetMode(AUDIO_I2S_SOURCE_CH, 0);
        MApi_AUDIO_EnaEarphone_LowPower_Stage();
        MApi_AUDIO_EnaEarphone_HighDriving_Stage();
        MApi_AUDIO_SendIntrupt(0, 0);
        MApi_AUDIO_RebootDsp(DSP_SE);
        MApi_AUDIO_ExitAudioSystem();
        MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_MPEG);
        #endif

        #if (TEST_GOP2 == 1)        //audio sound
        MS_U16 u16Data = 0;
        MS_U32 u32Data = 0;
        u32Data = u16Data = MApi_SND_GetParam1(Sound_GET_PARAM_EQ, 0);
        MApi_AUDIO_SOUND_SetParam(Sound_GET_PARAM_EQ, &u32Data);
        MApi_AUDIO_SetPreScale(0, 0x6F);
        MApi_AUDIO_SourceConnect(E_CONNECT_KTV, TRUE);
        MApi_AUDIO_SetDynamicBass(0x0C);
        MApi_AUDIO_AbsoluteBass(0);
        MApi_AUDIO_AbsoluteTreble(0);
        MApi_AUDIO_ConvertVolumeUnit(100);
        AUDIO_PEQ_COEF eq_coef;
        eq_coef.band = 0;
        eq_coef.sfs = 0;
        eq_coef.scale = 0;
        eq_coef.a0 = 0;
        eq_coef.a1 = 0;
        eq_coef.a2 = 0;
        eq_coef.b1 = 0;
        eq_coef.b2 = 0;
        eq_coef.type = AUDIO_COEF_PEQ;
        eq_coef.enable = FALSE;
        eq_coef.precision = AUDIO_SINGLE_PRECISION;
        MApi_AUDIO_SetPEQCoef(&eq_coef);
        MApi_AUDIO_EnablePEQ(FALSE);
        MApi_AUDIO_EnableDcRemove(TRUE);
        #endif

        #if (TEST_GOP3 == 1)        //audio DTV
        MApi_AUDIO_GetMAD_LOCK();
        MApi_AUDIO_ReleaseDecodeSystem(AU_DEC_MAX);
        AudioDecStatus_t AudioDecStatus;
        MApi_AUDIO_GetDecodeSystem(AU_DEC_ID1, &AudioDecStatus);
        MApi_AUDIO_SetDecodeSystem(AU_DEC_ID1, &AudioDecStatus);
        MApi_AUDIO_SetDecodeCmd(AU_DEC_ID1, MSAPI_AUD_DVB_DECCMD_PLAY);
        MApi_AUDIO_GetDecSysSupportStatus(MSAPI_AUD_DVB_MPEG);
        MApi_AUDIO_GetDecStatus();
        MApi_AUDIO_GetCommand(DVB_Audio_Decoder1);
        #endif

        #if (TEST_GOP4 == 1)        //audio pvr
        MApi_AUDIO_SetEncInit(AU_DVB_ENCBR_192K, 2);
        MApi_AUDIO_SetEncCommand(AU_DVB_ENCCMD_STOP);
        AU_DVB_ENC_FRAME_INFO EncFrameInfo;
        MApi_AUDIO_GetEncodeFrameInfo(&EncFrameInfo);
        MApi_AUDIO_GetEncodeDoneFlag();
        #endif

        #if (TEST_GOP5 == 1)        // audio atv
        MApi_AUDIO_SIF_StartAutoStandardDetection();
        MApi_AUDIO_SIF_GetResultOfAutoStandardDetection();
        #endif

        #if (TEST_GOP7 == 1)        // audio SPDIF
        MApi_AUDIO_SPDIF_SetMute(FALSE);
        MApi_AUDIO_SPDIF_GetMode();
        Digital_Out_Channel_Status_t stChannelStatus;
        MApi_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stChannelStatus);
        #endif

        #if (TEST_GOP8 == 1)        //audio hdmi
        MS_BOOL onOff = FALSE;
        AUDIO_FS_TYPE hdmi_smpFreq = 0;
        HDMI_TX_OUTPUT_TYPE outType = 1;
        MApi_AUDIO_HDMI_Tx_GetStatus(&onOff, &hdmi_smpFreq, &outType);
        MApi_AUDIO_HDMI_Monitor();
        MApi_AUDIO_HDMI_SetNonpcm(FALSE);
        MApi_AUDIO_DTV_HDMI_CFG(1);
        MApi_AUDIO_HDMI_GetSynthFreq();
        MApi_AUDIO_HDMI_SetDownSample(0);
        ST_HDMI_AUDIO_STATUS hdmiAudioSts;
        MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus(&hdmiAudioSts);
        #endif

        #if (TEST_GOP9 == 1)        // audio advance
        MApi_AUDIO_SeInit();
        MApi_AUDIO_ADVSOUND_SetParam(0, 0, 0);
        MApi_AUDIO_ADVSND_SetParam(0, 0, 0);
        MApi_AUDIO_ADVSOUND_GetInfo(0);
        MApi_DBXTV_SetMode(0, 0, 0, 0);
        MApi_AUDIO_SetAdvSndSys(MSAPI_AUD_ADVSND_BBE);
        MApi_AUDIO_SetVDS(0);
        MApi_AUDIO_SetVSPK(0);
        MApi_AUDIO_SetSRS(0);
        MApi_AUDIO_SetBBE(0, 1);
        MApi_AUDIO_VSPK_WMod(1);
        MApi_AUDIO_VSPK_SMod(1);
        MApi_AUDIO_SRS_DC(1);
        MApi_AUDIO_SRS_TruBass(1);
        MApi_AUDIO_SRS_SetTsxtPara(0, 0);
        MApi_AUDIO_SetSRSTSHD(1);
        MApi_AUDIO_TSHD_Definition(0);
        MApi_AUDIO_SRS_SetTshdPara(0, 0);
        #endif

        #if (TEST_GOP10 == 1)       //audio dtv
        MApi_AUDIO_GetKTVInfo (0, 0);
        MApi_AUDIO_SetKTVInfo (0, 0, 0, 2);
        MApi_AUDIO_SetMixModeVolume(0, 0, 0, 0);
        MApi_AUDIO_SetMixModeMute(0, 0, TRUE);
        MApi_AUDIO_PlayMenuSound();
        #endif

        #if (TEST_GOP11 == 1)      //audio MM
        MApi_AUDIO_Init(MSAPI_AUD_DVB_MPEG);
        MApi_AUDIO_SetAACInfo(Audio_AAC_error_count, 0, 0);
        MApi_AUDIO_GetMpegInfo(Audio_MPEG_infoType_DecStatus);
        MApi_AUDIO_GetAC3Info(Audio_AC3_infoType_DecStatus);
        MApi_AUDIO_GetAACInfo(Audio_AAC_error_count);
        MApi_AUDIO_GetWmaInfo(Audio_WMA_infoType_DecStatus);
        MApi_AUDIO_GetDTSInfo(Audio_DTS_infoType_DecStatus);
        MApi_AUDIO_XPCM_Param(LPCM, 0, 0, 0, 0, 0);
        MApi_AUDIO_XPCM2_CheckIntStatus();
        MApi_AUDIO_StartBrowse();
        MApi_AUDIO_PauseDecode();
        MApi_AUDIO_CheckPlayDone();
        MApi_AUDIO_GetResidualBufferSize();
        MApi_AUDIO_GetPCMBufferSize(100);
        MApi_AUDIO_GetPCMBufferSize2(100,100);
        MApi_AUDIO_GetCurrentFrameNumber();
        MApi_AUDIO_GetSampleRate();
        MApi_AUDIO_GetBitRate();
        MApi_AUDIO_GetLayer();
        MApi_AUDIO_SetSampleRateIndex(1);
        MApi_AUDIO_FileEndNotification();
        MApi_AUDIO_GetEsMEMCnt();
        MApi_AUDIO_MM_SetInput (0, 0);
        MApi_AUDIO_MM_CheckPlayDone (0);
        MS_U32 u32WrtAddr = 0x1000;
        MS_U32 u32WrtByte = 10;
        MApi_AUDIO_MM_CheckInputRequest(0, &u32WrtAddr, &u32WrtByte);
        MApi_AUDIO_DmaReader_Init(0);
        MApi_AUDIO_DmaReader_AllInput_Init();
        MS_U32 buff = 0;
        //MApi_AUDIO_DmaReader_WritePCM(&buff, u32WrtByte);     // core dump cause by buffer address
        MApi_AUDIO_DmaWriter_Init();
        MApi_AUDIO_USBPCM_Enable(FALSE);
        MApi_AUDIO_USBPCM_SetFlag(FALSE);
        MApi_AUDIO_USBPCM_GetFlag();
        AUDIO_UPLOAD_INFO uploadInfo;
        MApi_AUDIO_USBPCM_GetMemInfo(&uploadInfo);
        MApi_AUDIO_PCMCapture_Init(0, 0);
        MApi_AUDIO_PCMCapture_Start(0);
        MApi_AUDIO_PCMCapture_Stop(0);
        MApi_AUDIO_PCMCapture_Read(0, &buff, u32WrtByte);
        MApi_AUDIO_VoIP_Config(0, 0, 0);
        MApi_AUDIO_ALSA_Check();
        MApi_AUDIO_ALSA_Enable(FALSE);
        MApi_AUDIO_UNI_CheckDecodeDone(&u32WrtAddr, &u32WrtByte);
        MApi_AUDIO_UNI_SetOutput(0);
        MApi_AUDIO_UNI_Set_PCMInputWriteAddr (0);
        MApi_AUDIO_UNI_Get_OutPCMLevel();
        audioRingTask auRingTask, auRingTask_1;
        MApi_AUDIO_RingTask(&auRingTask) ;
        MApi_AUDIO_Ring_DataTransfer(&auRingTask, &auRingTask_1);
        MApi_AUDIO_SetAudioParam2(0, 0, 0);
        MApi_AUDIO_GetAudioInfo2(0, 0, &buff);
        MApi_AUDIO_GetDDRInfo(0, 0);
        #endif
    }
    #else
    if (UtopiaIoctl(_pInstantAudio, MApi_CMD_AUDIO_EnableAutoVolume, (void*)&u32Param) != UTOPIA_STATUS_SUCCESS)
    {
        ULOGE("AUDIO", "[MAPI AUDIO][%06d] Enable Auto Volume fail\n", __LINE__);
        return;
    }
    #endif
    //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] Return 0\n", __FUNCTION__);
    return;

#else   //utopia 1.0
    if(VT_Count < 5)
    {
        VT_Count++;
            return _MApi_AUDIO_EnableAutoVolume(EnAVC);
    }
    else
    {

        #if (TEST_GOP1 == 1)        //audio system
        MS_U16 u16Data = 0;
        MApi_AUDIO_SetPowerOn(TRUE);
        u16Data = MApi_AUDIO_ReadMailBox(0, 5);
        MApi_AUDIO_WriteMailBox(0, 5, u16Data);
        MApi_AUDIO_FwTriggerDSP(0);
        MApi_AUDIO_SetFwStatus(TRUE);
        MApi_AUDIO_I2S_SetMode(AUDIO_I2S_SOURCE_CH, 0);
        MApi_AUDIO_EnaEarphone_LowPower_Stage();
        MApi_AUDIO_EnaEarphone_HighDriving_Stage();
        MApi_AUDIO_SendIntrupt(0, 0);
        MApi_AUDIO_RebootDsp(DSP_SE);
        MApi_AUDIO_ExitAudioSystem();
        MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_MPEG);
        #endif

        #if (TEST_GOP2 == 1)        //audio sound
        MS_U16 u16Data = 0;
        MS_U32 u32Data = 0;

        u32Data = u16Data = MApi_SND_GetParam1(Sound_GET_PARAM_EQ, 0);
        MApi_AUDIO_SOUND_SetParam(Sound_GET_PARAM_EQ, &u32Data);
        MApi_AUDIO_SetPreScale(0, 0x6F);
        MApi_AUDIO_SourceConnect(E_CONNECT_KTV, TRUE);
        MApi_AUDIO_SetDynamicBass(0x0C);
        MApi_AUDIO_AbsoluteBass(0);
        MApi_AUDIO_AbsoluteTreble(0);
        MApi_AUDIO_ConvertVolumeUnit(100);

        AUDIO_PEQ_COEF eq_coef;
        eq_coef.band = 0;
        eq_coef.sfs = 0;
        eq_coef.scale = 0;
        eq_coef.a0 = 0;
        eq_coef.a1 = 0;
        eq_coef.a2 = 0;
        eq_coef.b1 = 0;
        eq_coef.b2 = 0;
        eq_coef.type = AUDIO_COEF_PEQ;
        eq_coef.enable = FALSE;
        eq_coef.precision = AUDIO_SINGLE_PRECISION;
        MApi_AUDIO_SetPEQCoef(&eq_coef);
        MApi_AUDIO_EnablePEQ(FALSE);
        MApi_AUDIO_EnableDcRemove(TRUE);
        #endif

        #if (TEST_GOP3 == 1)        //audio DTV
        MApi_AUDIO_GetMAD_LOCK();
        MApi_AUDIO_ReleaseDecodeSystem(AU_DEC_MAX);
        AudioDecStatus_t AudioDecStatus;
        MApi_AUDIO_GetDecodeSystem(AU_DEC_ID1, &AudioDecStatus);
        MApi_AUDIO_SetDecodeSystem(AU_DEC_ID1, &AudioDecStatus);
        MApi_AUDIO_SetDecodeCmd(AU_DEC_ID1, MSAPI_AUD_DVB_DECCMD_PLAY);
        MApi_AUDIO_GetDecSysSupportStatus(MSAPI_AUD_DVB_MPEG);
        MApi_AUDIO_GetDecStatus();
        MApi_AUDIO_GetCommand(DVB_Audio_Decoder1);
        #endif

        #if (TEST_GOP4 == 1)        //audio pvr
        MApi_AUDIO_SetEncInit(AU_DVB_ENCBR_192K, 2);
        MApi_AUDIO_SetEncCommand(AU_DVB_ENCCMD_STOP);
        AU_DVB_ENC_FRAME_INFO EncFrameInfo;
        MApi_AUDIO_GetEncodeFrameInfo(&EncFrameInfo);
        MApi_AUDIO_GetEncodeDoneFlag();
        #endif

        #if (TEST_GOP5 == 1)        // audio atv
        MApi_AUDIO_SIF_StartAutoStandardDetection();
        MApi_AUDIO_SIF_GetResultOfAutoStandardDetection();
        #endif

        #if (TEST_GOP7 == 1)        // audio SPDIF
        MApi_AUDIO_SPDIF_SetMute(FALSE);
        MApi_AUDIO_SPDIF_GetMode();
        Digital_Out_Channel_Status_t stChannelStatus;
        MApi_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stChannelStatus);
        #endif

        #if (TEST_GOP8 == 1)        //audio hdmi
        MS_BOOL onOff = FALSE;
        AUDIO_FS_TYPE hdmi_smpFreq = 0;
        HDMI_TX_OUTPUT_TYPE outType = 1;
        MApi_AUDIO_HDMI_Tx_GetStatus(&onOff, &hdmi_smpFreq, &outType);
        MApi_AUDIO_HDMI_Monitor();
        MApi_AUDIO_HDMI_SetNonpcm(FALSE);
        MApi_AUDIO_DTV_HDMI_CFG(1);
        MApi_AUDIO_HDMI_GetSynthFreq();
        MApi_AUDIO_HDMI_SetDownSample(0);
        ST_HDMI_AUDIO_STATUS hdmiAudioSts;
        MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus(&hdmiAudioSts);
        #endif

        #if (TEST_GOP9 == 1)        // audio advance
        MApi_AUDIO_SeInit();
        MApi_AUDIO_ADVSOUND_SetParam(0, 0, 0);
        MApi_AUDIO_ADVSND_SetParam(0, 0, 0);
        MApi_AUDIO_ADVSOUND_GetInfo(0);
        MApi_DBXTV_SetMode(0, 0, 0, 0);
        MApi_AUDIO_SetAdvSndSys(MSAPI_AUD_ADVSND_BBE);
        MApi_AUDIO_SetVDS(0);
        MApi_AUDIO_SetVSPK(0);
        MApi_AUDIO_SetSRS(0);
        MApi_AUDIO_SetBBE(0, 1);
        MApi_AUDIO_VSPK_WMod(1);
        MApi_AUDIO_VSPK_SMod(1);
        MApi_AUDIO_SRS_DC(1);
        MApi_AUDIO_SRS_TruBass(1);
        MApi_AUDIO_SRS_SetTsxtPara(0, 0);
        MApi_AUDIO_SetSRSTSHD(1);
        MApi_AUDIO_TSHD_Definition(0);
        MApi_AUDIO_SRS_SetTshdPara(0, 0);
        #endif

        #if (TEST_GOP10 == 1)       //audio dtv
        MApi_AUDIO_GetKTVInfo (0, 0);
        MApi_AUDIO_SetKTVInfo (0, 0, 0, 2);
        MApi_AUDIO_SetMixModeVolume(0, 0, 0, 0);
        MApi_AUDIO_SetMixModeMute(0, 0, TRUE);
        MApi_AUDIO_PlayMenuSound();
        #endif

        #if (TEST_GOP11 == 1)      //audio MM
        MApi_AUDIO_Init(MSAPI_AUD_DVB_MPEG);
        MApi_AUDIO_SetAACInfo(Audio_AAC_error_count, 0, 0);
        MApi_AUDIO_GetMpegInfo(Audio_MPEG_infoType_DecStatus);
        MApi_AUDIO_GetAC3Info(Audio_AC3_infoType_DecStatus);
        MApi_AUDIO_GetAACInfo(Audio_AAC_error_count);
        MApi_AUDIO_GetWmaInfo(Audio_WMA_infoType_DecStatus);
        MApi_AUDIO_GetDTSInfo(Audio_DTS_infoType_DecStatus);
        MApi_AUDIO_XPCM_Param(LPCM, 0, 0, 0, 0, 0);
        MApi_AUDIO_XPCM2_Param(LPCM, 0, 0, 0, 0, 0);
        MApi_AUDIO_XPCM2_CheckIntStatus();
        MApi_AUDIO_StartBrowse();
        MApi_AUDIO_PauseDecode();
        MApi_AUDIO_CheckPlayDone();
        MApi_AUDIO_GetResidualBufferSize();
        MApi_AUDIO_GetPCMBufferSize(100);
        MApi_AUDIO_GetPCMBufferSize2(100,100);
        MApi_AUDIO_GetCurrentFrameNumber();
        MApi_AUDIO_GetSampleRate();
        MApi_AUDIO_GetBitRate();
        MApi_AUDIO_GetLayer();
        MApi_AUDIO_SetSampleRateIndex(1);
        MApi_AUDIO_FileEndNotification();
        MApi_AUDIO_GetEsMEMCnt();
        MApi_AUDIO_MM_SetInput (0, 0);
        MApi_AUDIO_MM_CheckPlayDone (0);
        MS_U32 u32WrtAddr = 0x1000;
        MS_U32 u32WrtByte = 10;
        MApi_AUDIO_MM_CheckInputRequest(0, &u32WrtAddr, &u32WrtByte);
        MApi_AUDIO_DmaReader_Init(0);
        MApi_AUDIO_DmaReader_AllInput_Init();
        MS_U32 buff = 0;
        //MApi_AUDIO_DmaReader_WritePCM(&buff, u32WrtByte);     // core dump cause by buffer address
        MApi_AUDIO_DmaWriter_Init();
        MApi_AUDIO_USBPCM_Enable(FALSE);
        MApi_AUDIO_USBPCM_SetFlag(FALSE);
        MApi_AUDIO_USBPCM_GetFlag();
        AUDIO_UPLOAD_INFO uploadInfo;
        MApi_AUDIO_USBPCM_GetMemInfo(&uploadInfo);
        MApi_AUDIO_PCMCapture_Init(0, 0);
        MApi_AUDIO_PCMCapture_Start(0);
        MApi_AUDIO_PCMCapture_Stop(0);
        MApi_AUDIO_PCMCapture_Read(0, &buff, u32WrtByte);
        MApi_AUDIO_VoIP_Config(0, 0, 0);
        MApi_AUDIO_ALSA_Check();
        MApi_AUDIO_ALSA_Enable(FALSE);
        MApi_AUDIO_UNI_CheckDecodeDone(&u32WrtAddr, &u32WrtByte);
        MApi_AUDIO_UNI_SetOutput(0);
        MApi_AUDIO_UNI_Set_PCMInputWriteAddr (0);
        MApi_AUDIO_UNI_Get_OutPCMLevel();
        audioRingTask auRingTask, auRingTask_1;
        MApi_AUDIO_RingTask(&auRingTask) ;
        MApi_AUDIO_Ring_DataTransfer(&auRingTask, &auRingTask_1);
        MApi_AUDIO_SetAudioParam2(0, 0, 0);
        MApi_AUDIO_GetAudioInfo2(0, 0, &buff);
        MApi_AUDIO_GetDDRInfo(0, 0);
        #endif
    }
#endif
}
#endif
