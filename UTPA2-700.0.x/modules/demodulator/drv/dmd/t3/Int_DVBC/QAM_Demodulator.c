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

#include "Board.h"
#include "QAM_Demodulator.h"
#include "GPIO.h"
#include "MsTypes.h"
#include "drvBDMA.h"
//#include "SWI2C.h"
#include"drvIIC.h"
#include "drvUartDebug.h" //For OTHER_TUNER_DEBUG
#include "msAPI_CI.h"
#include "msAPI_Timer.h"
#include "apiXC.h"
#include "apiXC_Adc.h"

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"

#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
#include "MApp_GlobalFunction.h"
#include "INTERN_DVBC.c"
#include "drvVIF.h"
#endif

#define QAM_DBG(x)  //x
#if ENABLE_AUTOTEST
extern BOOLEAN g_bAutobuildDebug;
#endif
extern U16 u16ScanDtvChNum;

void devQAM_Init(void)
{
    // hard-wire reset pin
    Demodulator_OFF();
    MsOS_DelayTask(20);
    Demodulator_ON();
    MsOS_DelayTask(30);

#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    INTERN_DVBC_Power_On_Initialization();
#endif

    devDigitalTuner_Init();
}

void devQAM_ControlPowerOnOff(BOOLEAN bPowerOn)
{

#if (FRONTEND_IF_DEMODE_TYPE == MSTAR_VIF || FRONTEND_IF_DEMODE_TYPE == MSTAR_VIF_MSB1210 || FRONTEND_IF_DEMODE_TYPE == MSTAR_INTERN_VIF)
    if (bPowerOn == ENABLE) // DTV mode
    {
        EXT_RF_AGC_OFF();
        printf("KKK: (Temp)EXT_RF_AGC_OFF...for DTV\n");
    }
    else
    {
        EXT_RF_AGC_ON();
        printf("KKK: (Temp)EXT_RF_AGC_On...for ATV\n");
    }
#endif

#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    INTERN_DVBC_Power_ON_OFF(bPowerOn);
#endif
}

FUNCTION_RESULT devQAM_PowerSave(void)
{
#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    if (INTERN_DVBC_Power_Save() == TRUE)
        return E_RESULT_SUCCESS;
    else
        return E_RESULT_FAILURE;
#else
    return E_RESULT_SUCCESS;
#endif
}

void devQAM_TunerProgramming(U8 *cTunerData)
{
    *cTunerData=*cTunerData;
}

#if DTV_SCAN_AUTO_FINE_TUNE_ENABLE
BOOLEAN devQAM_GetFrequencyOffset(float *pFreqOff, RF_CHANNEL_BANDWIDTH u8BW)
{
    *pFreqOff = 0;  //Temp Solution;;
    u8BW = u8BW; //To avoid compiler warning
    return 1;
}
BOOLEAN devQAM_SetAfcEnable(BOOLEAN bEnable)
{
    bEnable = bEnable; //To avoid compiler warning
    return 1;
}
#endif

void devQAM_SetFrequency(DWORD dwFrequency, U8 u8QamMode, U16 u16SymbolRate, BOOLEAN bSpecInv)
{
    //dwFrequency = 562000;                   // << Ken 20090629
    //eBandWidth = E_RF_CH_BAND_8MHz;         // << Ken 20090629

    //printf("====dwFrequency = %lu \n", dwFrequency/1000);
    //printf("====eBandWidth = %bx\n", eBandWidth);
    #if ENABLE_AUTOTEST
    if(g_bAutobuildDebug && (dwFrequency/1000 == 850))
    {
        printf("31_DTV_Tuning_End_%d\n", u16ScanDtvChNum);
    }
    #endif

    #if (ENABLE_AUTOTEST || ENABLE_BOOTTIME)
      #if (ENABLE_BOOTTIME==DISABLE)
        if (g_bAutobuildDebug == TRUE)
      #endif
        {
            gU32TmpTime = msAPI_Timer_DiffTimeFromNow(gU32BootTime);
            printf("[boot time]QAM set RF = %ld\n", gU32TmpTime);
        }
    #endif

    devDigitalTuner_SetFreq((double)(dwFrequency/1000.0), E_RF_CH_BAND_8MHz);

#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    //MsOS_DelayTask(10); //waiting for tuner write

    #if ENABLE_CI
    if ( msAPI_CI_CardDetect())
        INTERN_DVBC_Config (u16SymbolRate, u8QamMode, 36167, bSpecInv, FALSE);
    else
    #endif
    INTERN_DVBC_Config (u16SymbolRate, u8QamMode, 36167, bSpecInv, TRUE);

    INTERN_DVBC_Active(ENABLE);
#else
    UNUSED(dwFrequency);
    UNUSED(u8QamMode);
    UNUSED(u16SymbolRate);
    UNUSED(bSpecInv);
#endif

}

BOOLEAN devQAM_GetLockStatus(QAM_LOCK_STATUS eStatus)
{
#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    return INTERN_DVBC_Lock(eStatus);
#else
    UNUSED(eStatus);
    return TRUE;
#endif
}

BYTE devQAM_GetSignalToNoiseRatio(void)
{
#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    return (BYTE)INTERN_DVBC_GetSNR();
#else
    return 1;
#endif
}

void devQAM_PassThroughI2C(BOOLEAN bEnable)
{
    bEnable=bEnable;
}

void devQAM_SwitchTransportSteamInferface(QAM_TS_INTERFACE interface)
{
    BOOLEAN bEnable;

    if ( interface == E_QAM_SERIAL_INTERFACE )
    {
        bEnable = TRUE;
    }
    else
    {
        bEnable = FALSE;
    }
#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    INTERN_DVBC_Serial_Control(bEnable);
#endif

}

FUNCTION_RESULT devQAM_GetSignal_Vit( DWORD *vit )
{
#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    float ber;
    *vit = *vit;
    if (INTERN_DVBC_GetPostViterbiBer(&ber)==TRUE)
       return E_RESULT_SUCCESS;
    else
        return E_RESULT_FAILURE;
#else
    *vit = *vit;
    return E_RESULT_FAILURE;
#endif
}

FUNCTION_RESULT devQAM_GetSignalStrength(WORD *strength)
{
#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    return INTERN_DVBC_GetSignalStrength(strength);
#else
    *strength = 80;
    return E_RESULT_SUCCESS;
#endif

}


FUNCTION_RESULT devQAM_GetSignalQuality(WORD *quality)
{
#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    return INTERN_DVBC_GetSignalQuality(quality);
#else
    *quality = 80;
    return E_RESULT_SUCCESS;
#endif
}

BOOLEAN devQAM_PassThroughI2C_ReadBytes(U8 u8SlaveID, U8 u8AddrNum, U8* paddr, U16 u16size, U8* pu8data)
{
    UNUSED(u8SlaveID);
    UNUSED(u8AddrNum);
    UNUSED(paddr);
    UNUSED(u16size);
    UNUSED(pu8data);
    return TRUE;
}

BOOLEAN devQAM_PassThroughI2C_WriteBytes(U8 u8SlaveID, U8 u8AddrNum, U8* paddr, U16 u16size, U8* pu8data)
{
    UNUSED(u8SlaveID);
    UNUSED(u8AddrNum);
    UNUSED(paddr);
    UNUSED(u16size);
    UNUSED(pu8data);
    return TRUE;
}
BOOLEAN devQAM_GetSpecInv(void)
{
#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    return TRUE;
#endif
    return TRUE;
}
