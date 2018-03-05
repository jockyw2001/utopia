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
/// file    drvAVD.c
/// @brief  AVD Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#include <stdio.h>
#include <math.h>
#endif
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"

// Internal Definition
//#include "regCHIP.h"
//#include "regAVD.h"
//#include "mapi_tuner.h"
#include "drvSYS.h"
#include "drvDMD_VD_MBX.h"
#include "drvDMD_INTERN_DVBT_v2.h"
#include "halDMD_INTERN_DVBT.h"
#include "halDMD_INTERN_common.h"
#include "drvSAR.h"  // for Utopia2
#include "utopia.h"
#include "utopia_dapi.h"
#include "../../utopia_core/utopia_driver_id.h"
#include "ULog.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#define DMD_LOCK()      \
    do{                         \
        MS_ASSERT(MsOS_In_Interrupt() == FALSE); \
        if (_s32DMD_DVBT_Mutex == -1) return FALSE; \
        if (_u8DMDDbgLevel == DMD_DBGLV_DEBUG) ULOGD("DEMOD","%s lock mutex\n", __FUNCTION__);\
        MsOS_ObtainMutex(_s32DMD_DVBT_Mutex, MSOS_WAIT_FOREVER);\
        }while(0)

#define DMD_UNLOCK()      \
    do{                         \
        MsOS_ReleaseMutex(_s32DMD_DVBT_Mutex);\
        if (_u8DMDDbgLevel == DMD_DBGLV_DEBUG) ULOGD("DEMOD","%s unlock mutex\n", __FUNCTION__); \
        }while(0)

#if 0
MS_U8		 DVBT_TS_PHASE_EN =0;
MS_U8		 DVBT_TS_PHASE_NUM = 0;
#endif

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
MS_BOOL bIsDVBT = FALSE;       // Usage for STR

// YP mark
#if 0

#if 1
static MSIF_Version _drv_dmd_dvbt_intern_version = {
    .MW = { DMD_DVBT_INTERN_VER, },
};
#else
static MSIF_Version _drv_dmd_dvbt_intern_version;
#endif

static DMD_DVBT_InitData _sDMD_DVBT_InitData;
static DMD_DbgLv _u8DMDDbgLevel=DMD_DBGLV_NONE;
static MS_S32 _s32DMD_DVBT_Mutex=-1;
static DMD_DVBT_Info sDMD_DVBT_Info;
static MS_U16 u16DMD_DVBT_TPS_Timeout = 1500, u16DMD_DVBT_FEC_Timeout=6000;
static MS_U32 u32DMD_DVBT_IfFrequency = 36167L, u32DMD_DVBT_FsFrequency = 45474L;
static MS_U8 u8DMD_DVBT_IQSwap=0;
static DMD_RF_CHANNEL_BANDWIDTH eDMD_DVBT_BandWidth=E_DMD_RF_CH_BAND_8MHz;
#endif
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define DMD_DBG(x)          (x)
#else
#define DMD_DBG(x)          // (x)
#endif
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
typedef MS_BOOL                   (*IOCTL_DVBT_Init)(DMD_DVBT_InitData_Transform*, MS_U32);
typedef MS_BOOL                   (*IOCTL_DVBT_Exit)(void);
typedef MS_BOOL                   (*IOCTL_DVBT_SetDbgLevel)(DMD_DbgLv);
//typedef DMD_DVBT_Info*       (*IOCTL_DVBT_GetInfo)(DMD_DVBT_INFO_TYPE);
typedef MS_BOOL                   (*IOCTL_DVBT_GetLibVer)(const MSIF_Version **);
typedef MS_BOOL                   (*IOCTL_DVBT_GetFWVer)(MS_U16 *);
typedef MS_BOOL                   (*IOCTL_DVBT_GetReg)(MS_U16, MS_U8 *);
typedef MS_BOOL                   (*IOCTL_DVBT_SetReg)(MS_U16, MS_U8);
typedef MS_BOOL                   (*IOCTL_DVBT_SetSerialControl)(MS_BOOL);
typedef MS_BOOL                   (*IOCTL_DVBT_SetConfig)(DMD_RF_CHANNEL_BANDWIDTH, MS_BOOL, MS_BOOL);
typedef MS_BOOL                   (*IOCTL_DVBT_SetConfigHPLP)(DMD_RF_CHANNEL_BANDWIDTH, MS_BOOL, MS_BOOL, MS_BOOL);
typedef MS_BOOL                   (*IOCTL_DVBT_SetConfigHPLPSetIF)(DMD_RF_CHANNEL_BANDWIDTH, MS_BOOL, MS_BOOL, MS_BOOL, MS_U32, MS_U32, MS_U8);
typedef MS_BOOL                   (*IOCTL_DVBT_SetActive)(MS_BOOL);
typedef MS_BOOL                   (*IOCTL_DVBT_GetLock)(DMD_DVBT_GETLOCK_TYPE, DMD_LOCK_STATUS *);
//typedef MS_BOOL                   (*IOCTL_DVBT_GetSignalStrength)(MS_U16 *);
//typedef MS_BOOL                   (*IOCTL_DVBT_GetSignalStrengthWithRFPower)(MS_U16 *, float);
//typedef MS_BOOL                   (*IOCTL_DVBT_GetSignalQuality)(MS_U16 *);
//typedef MS_BOOL                   (*IOCTL_DVBT_GetSignalQualityWithRFPower)(MS_U16 *, float);
//arthur
typedef MS_BOOL                   (*IOCTL_DVBT_GetSNR)(MS_U32 *);
typedef MS_BOOL                   (*IOCTL_DVBT_GetPostViterbiBer)(MS_U16 *,MS_U32 *,MS_U16 *);
typedef MS_BOOL                    (*IOCTL_DVBT_GetIFAGC)(MS_U8 *ifagc_reg, MS_U8 *ifagc_reg_lsb, MS_U16 *ifagc_err);

//typedef MS_BOOL                   (*IOCTL_DVBT_GetPreViterbiBer)(float *);
typedef MS_BOOL                   (*IOCTL_DVBT_GetPacketErr)(MS_U16 *);
typedef MS_BOOL                   (*IOCTL_DVBT_GetTPSInfo)(MS_U16 *);
typedef MS_BOOL                   (*IOCTL_DVBT_GetCellID)(MS_U16 *);
//typedef MS_BOOL                   (*IOCTL_DVBT_GetFreqOffset)(float *);
//typedef MS_BOOL                   (*IOCTL_DVBT_NORDIGSSITableWrite)(DMD_CONSTEL, DMD_CODERATE, float);
//typedef MS_BOOL                   (*IOCTL_DVBT_NORDIGSSITableRead)(DMD_CONSTEL, DMD_CODERATE, float*);
typedef MS_U32                     (*IOCTL_DVBT_SetPowerState)(EN_POWER_MODE);

typedef struct DVBT_INSTANT_PRIVATE
{
    IOCTL_DVBT_Init                 fpDVBTInit;
    IOCTL_DVBT_Exit                  fpDVBTExit;
    IOCTL_DVBT_SetDbgLevel     fpDVBTSetDbgLevel;
    //IOCTL_DVBT_GetInfo              fpDVBTGetInfo;
    IOCTL_DVBT_GetLibVer               fpDVBTGetLibVer;
    IOCTL_DVBT_GetFWVer                fpDVBTGetFWVer;
    IOCTL_DVBT_GetReg               fpDVBTGetReg;
    IOCTL_DVBT_SetReg                fpDVBTSetReg;
    IOCTL_DVBT_SetSerialControl      fpDVBTSetSerialControl;
  //  IOCTL_DVBT_SetConfig                fpDVBTSetConfig;
  //  IOCTL_DVBT_SetConfigHPLP         fpDVBTSetConfigHPLP;
    IOCTL_DVBT_SetConfigHPLPSetIF    fpDVBTSetConfigHPLPSetIF;
    IOCTL_DVBT_SetActive    fpDVBTSetActive;
    IOCTL_DVBT_GetLock    fpDVBTGetLock;
//    IOCTL_DVBT_GetSignalStrength    fpDVBTGetSignalStrength;
//    IOCTL_DVBT_GetSignalStrengthWithRFPower     fpDVBTGetSignalStrengthWithRFPower;
//    IOCTL_DVBT_GetSignalQuality    fpDVBTGetSignalQuality;
//    IOCTL_DVBT_GetSignalQualityWithRFPower    fpDVBTGetSignalQualityWithRFPower;
//arthur
    IOCTL_DVBT_GetIFAGC    fpDVBT_GetIFAGC;
    IOCTL_DVBT_GetSNR    fpDVBTGetSNR;
    IOCTL_DVBT_GetPostViterbiBer    fpDVBTGetPostViterbiBer;
//    IOCTL_DVBT_GetPreViterbiBer    fpDVBTGetPreViterbiBer;
    IOCTL_DVBT_GetPacketErr    fpDVBTGetPacketErr;
    IOCTL_DVBT_GetTPSInfo    fpDVBTGetTPSInfo;
    IOCTL_DVBT_GetCellID    fpDVBTGetCellID;
//    IOCTL_DVBT_GetFreqOffset    fpDVBTGetFreqOffset;
//    IOCTL_DVBT_NORDIGSSITableWrite    fpDVBTNORDIGSSITableWrite;
//    IOCTL_DVBT_NORDIGSSITableRead    fpDVBTNORDIGSSITableRead;
    IOCTL_DVBT_SetPowerState        fpDVBTSetPowerState;
} DVBT_INSTANT_PRIVATE;


/***********************************
original driver code define
************************************/
#define DMD_LOCK()      \
    do{                         \
        MS_ASSERT(MsOS_In_Interrupt() == FALSE); \
        if (_s32DMD_DVBT_Mutex == -1) return FALSE; \
        if (_u8DMDDbgLevel == DMD_DBGLV_DEBUG) ULOGD("DEMOD","%s lock mutex\n", __FUNCTION__);\
        MsOS_ObtainMutex(_s32DMD_DVBT_Mutex, MSOS_WAIT_FOREVER);\
        }while(0)

#define DMD_UNLOCK()      \
    do{                         \
        MsOS_ReleaseMutex(_s32DMD_DVBT_Mutex);\
        if (_u8DMDDbgLevel == DMD_DBGLV_DEBUG) ULOGD("DEMOD","%s unlock mutex\n", __FUNCTION__); \
        }while(0)

MS_U8		 DVBT_TS_PHASE_EN =0;
MS_U8		 DVBT_TS_PHASE_NUM = 0;
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if 1
static MSIF_Version _drv_dmd_dvbt_intern_version = {
    .MW = { DMD_DVBT_INTERN_VER, },
};
#else
static MSIF_Version _drv_dmd_dvbt_intern_version;
#endif

//bryan temp mark
#if(0)
static DMD_DVBT_InitData _sDMD_DVBT_InitData;
#else
static DMD_DVBT_InitData_Transform _sDMD_DVBT_InitData;
#endif
static DMD_DbgLv _u8DMDDbgLevel=DMD_DBGLV_NONE;
static MS_S32 _s32DMD_DVBT_Mutex=-1;
static DMD_DVBT_Info sDMD_DVBT_Info;
static MS_U16 u16DMD_DVBT_TPS_Timeout = 1500, u16DMD_DVBT_FEC_Timeout=6000;
static MS_U32 u32DMD_DVBT_IfFrequency = 36167L, u32DMD_DVBT_FsFrequency = 45474L;
static MS_U8 u8DMD_DVBT_IQSwap=0;
static DMD_RF_CHANNEL_BANDWIDTH eDMD_DVBT_BandWidth=E_DMD_RF_CH_BAND_8MHz;
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#if(0)
#ifdef MS_DEBUG
#define DMD_DBG(x)          (x)
#else
#define DMD_DBG(x)          //(x)
#endif
#endif


/* bryan temp mark*/
#if(0)
MS_BOOL DMD_DVBT_Init(DMD_DVBT_InitData *pDMD_DVBT_InitData, MS_U32 u32InitDataLen)
{
    char pDMD_DVBT_MutexString[16];
    MS_U8 u8ADCIQMode = 0, u8PadSel = 0, bPGAEnable = 0, u8PGAGain = 5;
    MS_BOOL bRFAGCTristateEnable = 1;
    MS_BOOL bIFAGCTristateEnable = 0;

    if (_s32DMD_DVBT_Mutex != -1)
    {
        DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBT_Init more than once\n"));
        return FALSE;
    }

    if (NULL == strncpy(pDMD_DVBT_MutexString,"Mutex DMD DVBT",16))
    {
        DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBT_Init strcpy Fail\n"));
        return FALSE;
    }
    _s32DMD_DVBT_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, pDMD_DVBT_MutexString, MSOS_PROCESS_SHARED);
    if (_s32DMD_DVBT_Mutex == -1)
    {
        DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBT_Init Create Mutex Fail\n"));
        return FALSE;
    }
    //_u8DMDDbgLevel = DMD_DBGLV_DEBUG;
    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBT_Init\n");
    }
    #endif

    if ( sizeof(_sDMD_DVBT_InitData) == u32InitDataLen)
    {
        memcpy(&_sDMD_DVBT_InitData, pDMD_DVBT_InitData, u32InitDataLen);
    }
    else
    {
        DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBT_Init input data structure incorrect\n"));
        return FALSE;
    }

    if (_sDMD_DVBT_InitData.u8SarChannel != 0xFF)
    {
        MDrv_SAR_Adc_Config(_sDMD_DVBT_InitData.u8SarChannel, TRUE);
    }

    DMD_LOCK();
    MDrv_SYS_DMD_VD_MBX_SetType(E_DMD_VD_MBX_TYPE_DVBT);
    HAL_DMD_RegInit();

    if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt != NULL)
    {
        if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=2)
        {
            bRFAGCTristateEnable = (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[3] & (BIT_(0))) ? TRUE : FALSE; // RFAGC tristate control
            bIFAGCTristateEnable = (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[3] & (BIT_(4))) ? TRUE : FALSE; // IFAGC tristate control
        }
        else
        {
            bRFAGCTristateEnable = 1;
            bIFAGCTristateEnable = 0;
        }
    }
    else
    {
        bRFAGCTristateEnable = 1;
        bIFAGCTristateEnable = 0;
    }

    if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt != NULL)
    {
        if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=3)
        {
            u32DMD_DVBT_IfFrequency = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[4]; // IF frequency
            u32DMD_DVBT_IfFrequency =  (u32DMD_DVBT_IfFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[5]; // IF frequency
            u32DMD_DVBT_IfFrequency =  (u32DMD_DVBT_IfFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[6]; // IF frequency
            u32DMD_DVBT_IfFrequency =  (u32DMD_DVBT_IfFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[7]; // IF frequency
            u32DMD_DVBT_FsFrequency = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[8]; // FS frequency
            u32DMD_DVBT_FsFrequency =  (u32DMD_DVBT_FsFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[9]; // FS frequency
            u32DMD_DVBT_FsFrequency =  (u32DMD_DVBT_FsFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[10]; // FS frequency
            u32DMD_DVBT_FsFrequency =  (u32DMD_DVBT_FsFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[11]; // FS frequency
            u8DMD_DVBT_IQSwap = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[12]; // IQ Swap

            u8ADCIQMode = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[13]; // u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
            u8PadSel = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[14]; // u8PadSel : 0=Normal, 1=analog pad
            bPGAEnable = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[15]; // bPGAEnable : 0=disable, 1=enable
            u8PGAGain = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[16]; // u8PGAGain : default 5
        }
        else
        {

        }
    }
    else
    {

    }
    #ifdef MS_DEBUG
    ULOGD("DEMOD","u32DMD_DVBT_IfFrequency %ld\n",u32DMD_DVBT_IfFrequency);
    ULOGD("DEMOD","u32DMD_DVBT_FsFrequency %ld\n",u32DMD_DVBT_FsFrequency);
    ULOGD("DEMOD","u8DMD_DVBT_IQSwap %d\n",u8DMD_DVBT_IQSwap);
    #endif

    u16DMD_DVBT_TPS_Timeout = 1500;
    u16DMD_DVBT_FEC_Timeout = 6000;
    if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt != NULL)
    {
        if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=4)
        {
            u16DMD_DVBT_TPS_Timeout = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[17]; // TPS timeout in ms
            u16DMD_DVBT_TPS_Timeout =  (u16DMD_DVBT_TPS_Timeout<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[18];
            if (u16DMD_DVBT_TPS_Timeout < 700) u16DMD_DVBT_TPS_Timeout=700;
            //ULOGD("DEMOD","u16DMD_DVBT_TPS_Timeout %d\n",u16DMD_DVBT_TPS_Timeout);

            u16DMD_DVBT_FEC_Timeout = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[19]; // FEC timeout in ms
            u16DMD_DVBT_FEC_Timeout =  (u16DMD_DVBT_FEC_Timeout<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[20];
            if (u16DMD_DVBT_FEC_Timeout < 2500) u16DMD_DVBT_FEC_Timeout=2500;
            //ULOGD("DEMOD","u16DMD_DVBT_FEC_Timeout %d\n",u16DMD_DVBT_FEC_Timeout);
        }
        else
        {
        }
    }
    else
    {
    }

    if (bIFAGCTristateEnable)
    {
        MDrv_SYS_SetAGCPadMux(E_SYS_DTV_AGC_PAD_SET_ALL_OFF);
    }
    else
    {
        MDrv_SYS_SetAGCPadMux(E_SYS_DTV_AGC_PAD_SET);
    }


	// oga
	DVBT_TS_PHASE_EN =0;
	DVBT_TS_PHASE_NUM = 0;
   if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt != NULL)
   {
/*
   	if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=5)	// version bigger than 5, apply TS phase solution
   	{
		 DVBT_TS_PHASE_EN = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[INDEX_T_TS_PHASE_EN];
		 DVBT_TS_PHASE_NUM = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[INDEX_T_TS_PHASE_NUM];
		 ULOGD("DEMOD","##DVBT:TS check: bTsPhaseEn = %d, u16TsPhaseNum = %d\n",DVBT_TS_PHASE_EN,DVBT_TS_PHASE_NUM);
   	}
	else
	{
		ULOGD("DEMOD","##DVBT:TS Phase check !!, board version smaller than 4\n");
	}
*/
 }
   else	// if init board define is NULL TS phase needs check.
   {
   	ULOGD("DEMOD","##DVBT:TS Phase check !!\n");
   }



    if (_sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitExt != NULL)
    {
        if (_sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitExt[0]>=1)
        {
            INTERN_DVBT_Power_On_Initialization(bRFAGCTristateEnable, u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain, _sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitExt, _sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitSize);
        }
        else
        {
            ULOGD("DEMOD","u8DMD_DVBT_DSPRegInitExt Error\n");
        }
    }
    else
    {
        INTERN_DVBT_Power_On_Initialization(bRFAGCTristateEnable, u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain,  NULL, 0);
    }

    INTERN_DVBT_Version(&sDMD_DVBT_Info.u16Version);
    DMD_UNLOCK();
    #ifdef MS_DEBUG
    ULOGD("DEMOD","firmware version: %x\n",sDMD_DVBT_Info.u16Version);
    #endif
    return TRUE;
}
#else
MS_BOOL DMD_DVBT_Init(DMD_DVBT_InitData_Transform *pDMD_DVBT_InitData, MS_U32 u32InitDataLen)
{
    char pDMD_DVBT_MutexString[16];
    MS_U8 u8ADCIQMode = 0, u8PadSel = 0, bPGAEnable = 0, u8PGAGain = 5;
    MS_BOOL bRFAGCTristateEnable = 1;
    MS_BOOL bIFAGCTristateEnable = 0;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DMD_DVBT_Init\n"));

    bIsDVBT = TRUE;

    if (_s32DMD_DVBT_Mutex != -1)
    {
        DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBT_Init more than once\n"));
        return FALSE;
    }

    if (NULL == strncpy(pDMD_DVBT_MutexString,"Mutex DMD DVBT",16))
    {
        DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBT_Init strcpy Fail\n"));
        return FALSE;
    }
    _s32DMD_DVBT_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, pDMD_DVBT_MutexString, MSOS_PROCESS_SHARED);
    if (_s32DMD_DVBT_Mutex == -1)
    {
        DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBT_Init Create Mutex Fail\n"));
        return FALSE;
    }
    //_u8DMDDbgLevel = DMD_DBGLV_DEBUG;
    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBT_Init\n");
    }
    #endif
		u32InitDataLen=sizeof(_sDMD_DVBT_InitData);
    //if ( sizeof(_sDMD_DVBT_InitData) == u32InitDataLen)
    //bryan temp test
    if(1)
    {
        memcpy(&_sDMD_DVBT_InitData, pDMD_DVBT_InitData, u32InitDataLen);
    }
    else
    {
        DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBT_Init input data structure incorrect\n"));
        return FALSE;
    }

    if (_sDMD_DVBT_InitData.u8SarChannel != 0xFF)
    {
      //bryan temp mark
      //  MDrv_SAR_Adc_Config(_sDMD_DVBT_InitData.u8SarChannel, TRUE);
    }

    DMD_LOCK();
    MDrv_SYS_DMD_VD_MBX_SetType(E_DMD_VD_MBX_TYPE_DVBT);
    HAL_DMD_RegInit();

    if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt != NULL)
    {
        if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=2)
        {
            bRFAGCTristateEnable = (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[3] & (BIT_(0))) ? TRUE : FALSE; // RFAGC tristate control
            bIFAGCTristateEnable = (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[3] & (BIT_(4))) ? TRUE : FALSE; // IFAGC tristate control
        }
        else
        {
            bRFAGCTristateEnable = 1;
            bIFAGCTristateEnable = 0;
        }
    }
    else
    {
        bRFAGCTristateEnable = 1;
        bIFAGCTristateEnable = 0;
    }

    if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt != NULL)
    {
        if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=3)
        {
            u32DMD_DVBT_IfFrequency = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[4]; // IF frequency
            u32DMD_DVBT_IfFrequency =  (u32DMD_DVBT_IfFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[5]; // IF frequency
            u32DMD_DVBT_IfFrequency =  (u32DMD_DVBT_IfFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[6]; // IF frequency
            u32DMD_DVBT_IfFrequency =  (u32DMD_DVBT_IfFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[7]; // IF frequency
            u32DMD_DVBT_FsFrequency = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[8]; // FS frequency
            u32DMD_DVBT_FsFrequency =  (u32DMD_DVBT_FsFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[9]; // FS frequency
            u32DMD_DVBT_FsFrequency =  (u32DMD_DVBT_FsFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[10]; // FS frequency
            u32DMD_DVBT_FsFrequency =  (u32DMD_DVBT_FsFrequency<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[11]; // FS frequency
            u8DMD_DVBT_IQSwap = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[12]; // IQ Swap

            u8ADCIQMode = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[13]; // u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
            u8PadSel = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[14]; // u8PadSel : 0=Normal, 1=analog pad
            bPGAEnable = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[15]; // bPGAEnable : 0=disable, 1=enable
            u8PGAGain = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[16]; // u8PGAGain : default 5
        }
        else
        {

        }
    }
    else
    {

    }
    #ifdef MS_DEBUG
    ULOGD("DEMOD","u32DMD_DVBT_IfFrequency %ld\n",u32DMD_DVBT_IfFrequency);
    ULOGD("DEMOD","u32DMD_DVBT_FsFrequency %ld\n",u32DMD_DVBT_FsFrequency);
    ULOGD("DEMOD","u8DMD_DVBT_IQSwap %d\n",u8DMD_DVBT_IQSwap);
    #endif

    u16DMD_DVBT_TPS_Timeout = 1500;
    u16DMD_DVBT_FEC_Timeout = 6000;
    if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt != NULL)
    {
        if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=4)
        {
            u16DMD_DVBT_TPS_Timeout = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[17]; // TPS timeout in ms
            u16DMD_DVBT_TPS_Timeout =  (u16DMD_DVBT_TPS_Timeout<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[18];
            if (u16DMD_DVBT_TPS_Timeout < 700) u16DMD_DVBT_TPS_Timeout=700;
            //ULOGD("DEMOD","u16DMD_DVBT_TPS_Timeout %d\n",u16DMD_DVBT_TPS_Timeout);

            u16DMD_DVBT_FEC_Timeout = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[19]; // FEC timeout in ms
            u16DMD_DVBT_FEC_Timeout =  (u16DMD_DVBT_FEC_Timeout<<8)+_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[20];
            if (u16DMD_DVBT_FEC_Timeout < 2500) u16DMD_DVBT_FEC_Timeout=2500;
            //ULOGD("DEMOD","u16DMD_DVBT_FEC_Timeout %d\n",u16DMD_DVBT_FEC_Timeout);
        }
        else
        {
        }
    }
    else
    {
    }

    if (bIFAGCTristateEnable)
    {
        MDrv_SYS_SetAGCPadMux(E_SYS_DTV_AGC_PAD_SET_ALL_OFF);
    }
    else
    {
        MDrv_SYS_SetAGCPadMux(E_SYS_DTV_AGC_PAD_SET);
    }


	// oga
	DVBT_TS_PHASE_EN =0;
	DVBT_TS_PHASE_NUM = 0;
   if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt != NULL)
   {
/*
   	if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=5)	// version bigger than 5, apply TS phase solution
   	{
		 DVBT_TS_PHASE_EN = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[INDEX_T_TS_PHASE_EN];
		 DVBT_TS_PHASE_NUM = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[INDEX_T_TS_PHASE_NUM];
		 ULOGD("DEMOD","##DVBT:TS check: bTsPhaseEn = %d, u16TsPhaseNum = %d\n",DVBT_TS_PHASE_EN,DVBT_TS_PHASE_NUM);
   	}
	else
	{
		ULOGD("DEMOD","##DVBT:TS Phase check !!, board version smaller than 4\n");
	}
*/
 }
   else	// if init board define is NULL TS phase needs check.
   {
   	ULOGD("DEMOD","##DVBT:TS Phase check !!\n");
   }



    if (_sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitExt != NULL)
    {
        if (_sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitExt[0]>=1)
        {
            INTERN_DVBT_Power_On_Initialization(bRFAGCTristateEnable, u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain, _sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitExt, _sDMD_DVBT_InitData.u8DMD_DVBT_DSPRegInitSize);
        }
        else
        {
            ULOGD("DEMOD","u8DMD_DVBT_DSPRegInitExt Error\n");
        }
    }
    else
    {
        INTERN_DVBT_Power_On_Initialization(bRFAGCTristateEnable, u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain,  NULL, 0);
    }

    INTERN_DVBT_Version(&sDMD_DVBT_Info.u16Version);
    DMD_UNLOCK();
    #ifdef MS_DEBUG
    ULOGD("DEMOD","firmware version: %x\n",sDMD_DVBT_Info.u16Version);
    #endif
    return TRUE;
}
#endif

MS_BOOL DMD_DVBT_Exit(void)
{
    MS_BOOL bRet;

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBT_Exit\n");
    }
    #endif

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DMD_DVBT_Exit\n"));
    bIsDVBT = FALSE;

    DMD_LOCK();
    bRet = INTERN_DVBT_Exit();
    DMD_UNLOCK();
    MsOS_DeleteMutex(_s32DMD_DVBT_Mutex);
    _s32DMD_DVBT_Mutex= -1;
    return bRet;
}

MS_BOOL DMD_DVBT_SetDbgLevel(DMD_DbgLv u8DbgLevel)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DMD_DVBT_SetDbgLevel\n"));

    DMD_LOCK();
    _u8DMDDbgLevel = u8DbgLevel;
    DMD_UNLOCK();
    return TRUE;
}

/*bryan temp mark*/
#if(0)
DMD_DVBT_Info* DMD_DVBT_GetInfo(DMD_DVBT_INFO_TYPE eInfoType)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT_GetInfo\n"));

    DMD_LOCK();
    switch (eInfoType)
    {
        case E_DMD_DVBT_MODULATION_INFO:
            INTERN_DVBT_Show_Modulation_info();
            break;
        case E_DMD_DVBT_DEMOD_INFO:
            INTERN_DVBT_Show_Demod_Info();
            break;
        case E_DMD_DVBT_LOCK_INFO:
            INTERN_DVBT_Show_Lock_Info();
            break;
        case E_DMD_DVBT_PRESFO_INFO:
            INTERN_DVBT_Show_PRESFO_Info();
            break;
        case E_DMD_DVBT_LOCK_TIME_INFO:
            INTERN_DVBT_Show_Lock_Time_Info();
            break;
        case E_DMD_DVBT_BER_INFO:
            INTERN_DVBT_Show_BER_Info();
            break;
        case E_DMD_DVBT_AGC_INFO:
            INTERN_DVBT_Show_AGC_Info();
            break;
        default:
            #ifdef MS_DEBUG
            ULOGD("DEMOD","MDrv_DMD_DVBT_GetInfo %d Error\n", eInfoType);
            #endif
            break;
    }
    DMD_UNLOCK();
    return &sDMD_DVBT_Info;
}
#endif


MS_BOOL DMD_DVBT_GetLibVer(const MSIF_Version **ppVersion)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DMD_DVBT_GetLibVer\n"));

    DMD_LOCK();
    if (!ppVersion)
    {
        return FALSE;
    }

    *ppVersion = &_drv_dmd_dvbt_intern_version;
    DMD_UNLOCK();
    return TRUE;
}

MS_BOOL DMD_DVBTGetFWVer(MS_U16 *ver)
{

    MS_BOOL bRet;
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DMD_DVBTGetFWVer\n"));

    DMD_LOCK();

    bRet = INTERN_DVBT_Version(ver);
    //ULOGD("DEMOD","MDrv_DMD_DVBT_GetFWVer %x\n",*ver);
    DMD_UNLOCK();

    return bRet;

}

MS_BOOL DMD_DVBT_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DMD_DVBT_GetReg\n"));

    DMD_LOCK();
    bRet=MDrv_SYS_DMD_VD_MBX_ReadReg(u16Addr, pu8Data);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBT_GetReg %x %x\n", u16Addr, *pu8Data);
    }
    #endif

    return bRet;
}

MS_BOOL DMD_DVBT_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DMD_DVBT_SetReg\n"));

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBT_SetReg %x %x\n", u16Addr, u8Data);
    }
    #endif

    DMD_LOCK();
    bRet=MDrv_SYS_DMD_VD_MBX_WriteReg(u16Addr, u8Data);
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL DMD_DVBT_SetSerialControl(MS_BOOL bEnable)
{
    MS_BOOL bRet;
    MS_U8 u8TSClk;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DMD_DVBT_SetSerialControl\n"));

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBT_SetSerialControl %x\n", bEnable);
    }
    #endif

    DMD_LOCK();
    if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt != NULL)
    {
        if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=1)
        {
            u8TSClk = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[2]; // TS_CLK
        }
        else
        {
            u8TSClk = 0xFF; // parallel mode: 0x0513 => ts_clk=288/(2*(0x16+1))=6.26MHz //@@++--
        }
    }
    else
    {
        u8TSClk = 0xFF; // parallel mode: 0x0513 => ts_clk=288/(2*(0x16+1))=6.26MHz //@@++--
    }
    bRet=INTERN_DVBT_Serial_Control(bEnable, u8TSClk);
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL DMD_DVBT_SetConfig(DMD_RF_CHANNEL_BANDWIDTH BW, MS_BOOL bSerialTS, MS_BOOL bPalBG)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DMD_DVBT_SetConfig\n"));

    return MDrv_DMD_DVBT_SetConfigHPLPSetIF(BW, bSerialTS, bPalBG, 0, u32DMD_DVBT_IfFrequency, u32DMD_DVBT_FsFrequency, u8DMD_DVBT_IQSwap);
}


MS_BOOL DMD_DVBT_SetConfigHPLP(DMD_RF_CHANNEL_BANDWIDTH BW, MS_BOOL bSerialTS, MS_BOOL bPalBG, MS_BOOL bLPSel)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DMD_DVBT_SetConfigHPLP\n"));

    return MDrv_DMD_DVBT_SetConfigHPLPSetIF(BW, bSerialTS, bPalBG, bLPSel, u32DMD_DVBT_IfFrequency, u32DMD_DVBT_FsFrequency, u8DMD_DVBT_IQSwap);
}


MS_BOOL DMD_DVBT_SetConfigHPLPSetIF(DMD_RF_CHANNEL_BANDWIDTH BW, MS_BOOL bSerialTS, MS_BOOL bPalBG, MS_BOOL bLPSel, MS_U32 u32IFFreq, MS_U32 u32FSFreq, MS_U8 u8IQSwap)
{
    MS_BOOL bRet;
    MS_U8 u8TSClk;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DMD_DVBT_SetConfigHPLPSetIF\n"));

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBT_SetConfigHPLPSetIF %d %d %d %d %ld %ld %d\n", BW, bSerialTS, bPalBG, bLPSel, u32IFFreq, u32FSFreq, u8IQSwap);
    }
    #endif

    DMD_LOCK();
    if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt != NULL)
    {
        if (_sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[0]>=1)
        {
            u8TSClk = _sDMD_DVBT_InitData.u8DMD_DVBT_InitExt[2]; // TS_CLK
        }
        else
        {
            u8TSClk = 0xFF; // parallel mode: 0x0513 => ts_clk=288/(2*(0x16+1))=6.26MHz //@@++--
        }
    }
    else
    {
        u8TSClk = 0xFF; // parallel mode: 0x0513 => ts_clk=288/(2*(0x16+1))=6.26MHz //@@++--
    }

    bRet=INTERN_DVBT_Config(BW, bSerialTS, bPalBG, bLPSel, u8TSClk, u32IFFreq, u32FSFreq, u8IQSwap);
    eDMD_DVBT_BandWidth=BW;
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL DMD_DVBT_SetActive(MS_BOOL bEnable)
{
    MS_BOOL bRet;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DMD_DVBT_SetActive\n"));

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBT_SetActive %d\n", bEnable);
    }
    #endif

    DMD_LOCK();
    bRet=INTERN_DVBT_Active(bEnable);
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL DMD_DVBT_GetLock(DMD_DVBT_GETLOCK_TYPE eType, DMD_LOCK_STATUS *eLockStatus)
{
    MS_BOOL bRet=TRUE;
    DMD_LOCK();

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DMD_DVBT_GetLock\n"));

    if ( eType == E_DMD_DMD_DVBT_GETLOCK ) // for channel scan
    {
        *eLockStatus = INTERN_DVBT_Lock(u16DMD_DVBT_TPS_Timeout, u16DMD_DVBT_FEC_Timeout);
    }
    else
    {
        if (INTERN_DVBT_GetLock(eType) == TRUE)
        {
            *eLockStatus = E_DMD_LOCK;
        }
        else
        {
            *eLockStatus = E_DMD_UNLOCK;
        }
    }
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBT_GetLock %d\n", bRet);
    }
    #endif
    return bRet;
}


/*bryan temp mark*/
#if(0)
MS_BOOL DMD_DVBT_GetSignalStrengthWithRFPower(MS_U16 *u16Strength, float fRFPowerDbm)
{
    MS_U8 u8SarValue;
    MS_BOOL bRet;

    DMD_LOCK();
    if (_sDMD_DVBT_InitData.u8SarChannel != 0xFF)
    {
        u8SarValue=MDrv_SAR_Adc_GetValue(_sDMD_DVBT_InitData.u8SarChannel);
    }
    else
    {
        u8SarValue=0xFF;
    }
    bRet=INTERN_DVBT_GetSignalStrength(u16Strength, (const DMD_DVBT_InitData *)(&_sDMD_DVBT_InitData), u8SarValue, fRFPowerDbm);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBT_GetSignalStrength %d\n", *u16Strength);
    }
    #endif
    return bRet;
}
#endif

#if(0)
MS_BOOL DMD_DVBT_GetSignalQualityWithRFPower(MS_U16 *u16Quality, float fRFPowerDbm)
{
    MS_U8 u8SarValue=0;
    MS_BOOL bRet=0;

    DMD_LOCK();
    if (_sDMD_DVBT_InitData.u8SarChannel != 0xFF)
    {
        u8SarValue=MDrv_SAR_Adc_GetValue(_sDMD_DVBT_InitData.u8SarChannel);
    }
    else
    {
        u8SarValue=0xFF;
    }
    bRet=INTERN_DVBT_GetSignalQuality(u16Quality, (const DMD_DVBT_InitData *)(&_sDMD_DVBT_InitData), u8SarValue, fRFPowerDbm);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBT_GetSignalQuality %d\n", *u16Quality);
    }
    #endif
    return bRet;
}
#endif

//arthur
MS_BOOL DMD_DVBT_GetIFAGC(MS_U8 *ifagc_reg, MS_U8 *ifagc_reg_lsb, MS_U16 *ifagc_err)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBT_GetIFAGC(ifagc_reg, ifagc_reg_lsb, ifagc_err);
    DMD_UNLOCK();

	  return bRet;
}

MS_BOOL DMD_DVBT_GetSNR(MS_U32 *noise_power_reg)
{
    DMD_LOCK();
    INTERN_DVBT_GetSNR(noise_power_reg);
    DMD_UNLOCK();

    return TRUE;
}

MS_BOOL DMD_DVBT_GetPostViterbiBer(MS_U16 *BitErrPeriod_reg, MS_U32 *BitErr_reg, MS_U16 *PktErr_reg)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet = INTERN_DVBT_GetPostViterbiBer(BitErrPeriod_reg, BitErr_reg, PktErr_reg);
    DMD_UNLOCK();

    return bRet;
}
#if(0)
MS_BOOL DMD_DVBT_GetPreViterbiBer(float *ber)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBT_GetPreViterbiBer(ber);
    DMD_UNLOCK();

    return bRet;
}
#endif

MS_BOOL DMD_DVBT_GetPacketErr(MS_U16 *pktErr)
{
    MS_BOOL bRet;
   // float   fBER;
   DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DMD_DVBT_GetPacketErr\n"));

    DMD_LOCK();
    //bryan temp mark
    //INTERN_DVBT_GetPostViterbiBer(&fBER);
    bRet=INTERN_DVBT_GetPacketErr(pktErr);

    //bryan temp mark
    //if ((*pktErr ==1) && (fBER<= 0.000001)) // for no signal case, from Oga
    if ((*pktErr ==1) )
    {
        *pktErr = 0x3FF;
    }
    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBT_GetPacketErr %d\n", *pktErr);
    }
    #endif
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL DMD_DVBT_GetTPSInfo(MS_U16 *u16Info)
{
    MS_BOOL bRet;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DMD_DVBT_GetTPSInfo\n"));

    DMD_LOCK();
    bRet=INTERN_DVBT_Get_TPS_Info(u16Info);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL DMD_DVBT_GetCellID(MS_U16 *u16CellID)
{
    MS_BOOL bRet;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DMD_DVBT_GetCellID\n"));

    DMD_LOCK();
    bRet=INTERN_DVBT_Get_CELL_ID(u16CellID);
    DMD_UNLOCK();
    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBT_GetCellID %d\n", *u16CellID);
    }
    #endif
    return bRet;
}

/*bryan temp mark*/
#if(0)
MS_BOOL DMD_DVBT_GetFreqOffset(float *pFreqOff)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8BW=8;

    DMD_LOCK();
    switch (eDMD_DVBT_BandWidth)
    {
        case E_DMD_RF_CH_BAND_6MHz:
            u8BW=6;
            break;

        case E_DMD_RF_CH_BAND_7MHz:
            u8BW=7;
            break;

        case E_DMD_RF_CH_BAND_8MHz:
        default:
            u8BW=8;
            break;
    }
    bRet=INTERN_DVBT_Get_FreqOffset(pFreqOff, u8BW);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_DBGLV_DEBUG)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBT_GetStatus %d %f\n", u8BW, *pFreqOff);
    }
    #endif
    return bRet;
}
#endif

/*bryan temp mark*/
#if(0)
MS_BOOL DMD_DVBT_NORDIG_SSI_Table_Write(DMD_CONSTEL constel, DMD_CODERATE code_rate, float write_value)
{
    return INTERN_DVBT_NORDIG_SSI_Table_Write(constel, code_rate, write_value);
}

MS_BOOL DMD_DVBT_NORDIG_SSI_Table_Read(DMD_CONSTEL constel, DMD_CODERATE code_rate, float *read_value)
{
    return INTERN_DVBT_NORDIG_SSI_Table_Read(constel, code_rate, read_value);
}
#endif

MS_U32 DMD_DVBT_SetPowerState(EN_POWER_MODE u16PowerState)
{
    static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DMD_DVBT_SetPowerState\n"));

    u32Return = u32Return;

    if(bIsDVBT == TRUE)
    {
        if (u16PowerState == E_POWER_SUSPEND)
    	{
    //        MDrv_DMD_DVBT_Exit();
            DMD_DVBT_Exit();
            _prev_u16PowerState = u16PowerState;
            u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
        }
        else if (u16PowerState == E_POWER_RESUME)
        {
            if (_prev_u16PowerState == E_POWER_SUSPEND)
            {
                DMD_DVBT_Init(&_sDMD_DVBT_InitData, sizeof(_sDMD_DVBT_InitData));
                _prev_u16PowerState = u16PowerState;
                u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
            }
            else
            {
                ULOGD("DEMOD","[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
                u32Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
            }
        }
        else
        {
        	ULOGD("DEMOD","[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        	u32Return = FALSE;
        }
    }
    else
    {
        ULOGD("DEMOD","\r\n ====== DVBT doesn't need to Suspend/Resume at Non-DVBT mode ====== \r\n");
        u32Return = FALSE;
    }
//    return UTOPIA_STATUS_SUCCESS;
    return u32Return;
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_U32 DVBTOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{

    DVBT_INSTANT_PRIVATE *pDvbtPri= NULL;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DVBTOpen\n"));

// void *pDvbtPriVoid = NULL;

    UtopiaInstanceCreate(sizeof(DVBT_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void*)&pDvbtPri);
// pDvbtPri = (DVBT_INSTANT_PRIVATE*)pDvbtPriVoid;



    pDvbtPri->fpDVBTInit = DMD_DVBT_Init;
    pDvbtPri->fpDVBTExit = DMD_DVBT_Exit;
    pDvbtPri->fpDVBTSetDbgLevel = DMD_DVBT_SetDbgLevel;
/*bryan temp mark*/
    #if(0)
    pDvbtPri->fpDVBTGetInfo = DMD_DVBT_GetInfo;
    #endif

    pDvbtPri->fpDVBTGetLibVer = DMD_DVBT_GetLibVer;
    pDvbtPri->fpDVBTGetFWVer = DMD_DVBTGetFWVer;
    pDvbtPri->fpDVBTGetReg = DMD_DVBT_GetReg;
    pDvbtPri->fpDVBTSetReg = DMD_DVBT_SetReg;
    pDvbtPri->fpDVBTSetSerialControl = DMD_DVBT_SetSerialControl;
    //pDvbtPri->fpDVBTSetConfig = DMD_DVBT_SetConfig;
    //pDvbtPri->fpDVBTSetConfigHPLP = MDrv_DMD_DVBT_SetConfigHPLP;
    pDvbtPri->fpDVBTSetConfigHPLPSetIF = DMD_DVBT_SetConfigHPLPSetIF;
    pDvbtPri->fpDVBTSetActive = DMD_DVBT_SetActive;
    pDvbtPri->fpDVBTGetLock = DMD_DVBT_GetLock;
    //pDvbtPri->fpDVBTGetSignalStrength = MDrv_DMD_DVBT_GetSignalStrength;
    /*bryan temp mark*/
    #if(0)
    pDvbtPri->fpDVBTGetSignalStrengthWithRFPower = DMD_DVBT_GetSignalStrengthWithRFPower;
    #endif

    //pDvbtPri->fpDVBTGetSignalQuality = MDrv_DMD_DVBT_GetSignalQuality;
    #if(0)
    pDvbtPri->fpDVBTGetSignalQualityWithRFPower = DMD_DVBT_GetSignalQualityWithRFPower;
    #endif

    //arthur
    pDvbtPri->fpDVBTGetSNR = DMD_DVBT_GetSNR;
    pDvbtPri->fpDVBTGetPostViterbiBer =DMD_DVBT_GetPostViterbiBer;
    pDvbtPri->fpDVBT_GetIFAGC=DMD_DVBT_GetIFAGC;

	  #if 0
    pDvbtPri->fpDVBTGetPreViterbiBer = DMD_DVBT_GetPreViterbiBer;
    #endif

    pDvbtPri->fpDVBTGetPacketErr = DMD_DVBT_GetPacketErr;
    pDvbtPri->fpDVBTGetTPSInfo = DMD_DVBT_GetTPSInfo;
    pDvbtPri->fpDVBTGetCellID = DMD_DVBT_GetCellID;

    #if(0)
    pDvbtPri->fpDVBTGetFreqOffset = DMD_DVBT_GetFreqOffset;
    #endif

    #if(0)
    pDvbtPri->fpDVBTNORDIGSSITableWrite = DMD_DVBT_NORDIG_SSI_Table_Write;
    pDvbtPri->fpDVBTNORDIGSSITableRead = DMD_DVBT_NORDIG_SSI_Table_Read;
    #endif

    pDvbtPri->fpDVBTSetPowerState = DMD_DVBT_SetPowerState;

    //return TRUE;
    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 DVBTIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{

    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);

    //void* pResource = NULL;

    DVBT_INSTANT_PRIVATE* psDVBTInstPri = NULL;
    void* psDVBTInstPriVoid = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psDVBTInstPriVoid);
    psDVBTInstPri = (DVBT_INSTANT_PRIVATE*)psDVBTInstPriVoid;

    MS_BOOL bRet = FALSE;

#if 0
    if (UtopiaResourceObtain(pModule, DVBT_POOL_ID_DMD0, &pResource) != 0)
    {
        DMD_DBG(ULOGD("DEMOD","UtopiaResourceObtainToInstant fail\n"));
	    return UTOPIA_STATUS_ERR_RESOURCE;
    }

    psDMD_DVBT_ResData = ((PDVBT_RESOURCE_PRIVATE)pResource)->sDMD_DVBT_ResData;
#endif



    switch (u32Cmd)
    {
        case DMD_DVBT_DRV_CMD_Init:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DVBTIoctl - MDrv_DMD_DVBT_Init\n"));
	     //bryan test
	     #if(0)
            bRet = psDVBTInstPri->fpDVBTInit( &(((PDVBT_INIT_PARAM)pArgs)->DMD_DVBT_InitData), ((PDVBT_INIT_PARAM)pArgs)->u32InitDataLen);
            #else
            bRet = psDVBTInstPri->fpDVBTInit( (((PDVBT_INIT_PARAM)pArgs)->DMD_DVBT_InitData), ((PDVBT_INIT_PARAM)pArgs)->u32InitDataLen);
            #endif
	     ((PDVBT_INIT_PARAM)pArgs)->ret=bRet;
            break;
        case DMD_DVBT_DRV_CMD_Exit:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DVBTIoctl - MDrv_DMD_DVBT_Exit\n"));
            bRet = psDVBTInstPri->fpDVBTExit();
            ((PDVBT_EXIT_PARAM)pArgs)->ret=bRet;
            break;
        case DMD_DVBT_DRV_CMD_SetDbgLeve:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DVBTIoctl - MDrv_DMD_DVBT_SetDbgLevel\n"));
            bRet = psDVBTInstPri->fpDVBTSetDbgLevel(((PDVBT_SETDBG_LEVEL_PARAM)pArgs)->u8DbgLevel);
	     ((PDVBT_SETDBG_LEVEL_PARAM)pArgs)->ret=bRet;
            break;
        /*bryan temp mark*/
        #if(0)
        case DMD_DVBT_DRV_CMD_GetInfo:
            DMD_DBG(ULOGD("DEMOD","DVBTIoctl - MDrv_DMD_DVBT_GetInfo\n"));
            ((PDVBT_GETINFO_PARAM)pArgs)->pInfo = psDVBTInstPri->fpDVBTGetInfo(((PDVBT_GETINFO_PARAM)pArgs)->eInfoType);
            bRet = TRUE;
	    ((PDVBT_GETINFO_PARAM)pArgs)->ret=bRet;
            break;
         #endif
        case DMD_DVBT_DRV_CMD_GetLibVer:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DVBTIoctl - MDrv_DMD_DVBT_GetLibVer\n"));
            bRet = psDVBTInstPri->fpDVBTGetLibVer(((PDVBT_GETLIBVER_PARAM)pArgs)->ppVersion);
	     ((PDVBT_GETLIBVER_PARAM)pArgs)->ret=bRet;
            break;
        case DMD_DVBT_DRV_CMD_GetFWVer:
//	     ULOGD("DEMOD","bryan debug check demod GetFWVer DVBT!!\n");
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DVBTIoctl - MDrv_DMD_DVBTGetFWVer\n"));
            bRet = psDVBTInstPri->fpDVBTGetFWVer(((PDVBT_GETFWVER_PARAM)pArgs)->ver);
	    ((PDVBT_GETFWVER_PARAM)pArgs)->ret=bRet;
            break;
        case DMD_DVBT_DRV_CMD_GetReg:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DVBTIoctl - MDrv_DMD_DVBT_GetReg\n"));
            bRet = psDVBTInstPri->fpDVBTGetReg( ((PDVBT_GETREG_PARAM)pArgs)->u16Addr, ((PDVBT_GETREG_PARAM)pArgs)->pu8Data);
	     ((PDVBT_GETREG_PARAM)pArgs)->ret=bRet;
            break;
        case DMD_DVBT_DRV_CMD_SetReg:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DVBTIoctl - MDrv_DMD_DVBT_SetReg\n"));
            bRet = psDVBTInstPri->fpDVBTSetReg( ((PDVBT_SETREG_PARAM)pArgs)->u16Addr, ((PDVBT_SETREG_PARAM)pArgs)->u8Data);
	     ((PDVBT_SETREG_PARAM)pArgs)->ret=bRet;
            break;
        case DMD_DVBT_DRV_CMD_SetSerialControl:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DVBTIoctl - MDrv_DMD_DVBT_SetSerialControl\n"));
            bRet = psDVBTInstPri->fpDVBTSetSerialControl( ((PDVBT_SetSerialControl_PARAM)pArgs)->bEnable);
	     ((PDVBT_SetSerialControl_PARAM)pArgs)->ret=bRet;
            break;
/*
        case DMD_DVBT_DRV_CMD_SetConfig:
            DMD_DBG(ULOGD("DEMOD","DVBTIoctl - MDrv_DMD_DVBT_SetConfig\n"));
            u32Ret = psDVBTInstPri->fpDVBTSetConfig( ((PDVBT_SetConfig_PARAM)pArgs)->BW, ((PDVBT_SetConfig_PARAM)pArgs)->bSerialTS,  ((PDVBT_SetConfig_PARAM)pArgs)->bPalBG);
            break;
        case DMD_DVBT_DRV_CMD_SetConfigHPLP:
            DMD_DBG(ULOGD("DEMOD","DVBTIoctl - MDrv_DMD_DVBT_SetConfigHPLP\n"));
            u32Ret = psDVBTInstPri->fpDVBTSetConfigHPLP(((PDVBT_SetConfigHPLP_PARAM)pArgs)->BW, ((PDVBT_SetConfigHPLP_PARAM)pArgs)->bSerialTS,  ((PDVBT_SetConfigHPLP_PARAM)pArgs)->bPalBG,  ((PDVBT_SetConfigHPLP_PARAM)pArgs)->bLPSel);
           break;
 */
        case DMD_DVBT_DRV_CMD_SetConfigHPLPSetIF:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DVBTIoctl - MDrv_DMD_DVBT_SetConfigHPLPSetIF\n"));
            bRet = psDVBTInstPri->fpDVBTSetConfigHPLPSetIF(((PDVBT_SetConfigHPLPSetIF_PARAM)pArgs)->BW, ((PDVBT_SetConfigHPLPSetIF_PARAM)pArgs)->bSerialTS,  ((PDVBT_SetConfigHPLPSetIF_PARAM)pArgs)->bPalBG,  ((PDVBT_SetConfigHPLPSetIF_PARAM)pArgs)->bLPSel, ((PDVBT_SetConfigHPLPSetIF_PARAM)pArgs)->u32IFFreq,  ((PDVBT_SetConfigHPLPSetIF_PARAM)pArgs)->u32FSFreq,  ((PDVBT_SetConfigHPLPSetIF_PARAM)pArgs)->u8IQSwap);
	    ((PDVBT_SetConfigHPLPSetIF_PARAM)pArgs)->ret=bRet;
            break;
        case DMD_DVBT_DRV_CMD_SetActive:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DVBTIoctl - MDrv_DMD_DVBT_SetActive\n"));
            bRet = psDVBTInstPri->fpDVBTSetActive(((PDVBT_SetActive_PARAM)pArgs)->bEnable);
	    ((PDVBT_SetActive_PARAM)pArgs)->ret=bRet;
            break;
        case DMD_DVBT_DRV_CMD_GetLock:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DVBTIoctl - MDrv_DMD_DVBT_GetLock\n"));
            bRet = psDVBTInstPri->fpDVBTGetLock(((PDVBT_GetLock_PARAM)pArgs)->eType, ((PDVBT_GetLock_PARAM)pArgs)->eLockStatus);
	    ((PDVBT_GetLock_PARAM)pArgs)->ret=bRet;
            break;
/*
        case DMD_DVBT_DRV_CMD_GetSignalStrength:
            DMD_DBG(ULOGD("DEMOD","DVBTIoctl - MDrv_DMD_DVBT_GetSignalStrength\n"));
            u32Ret = psDVBTInstPri->fpDVBTGetSignalStrength(((PDVBT_GetSignalStrength_PARAM)pArgs)->u16Strength);
            break;
*/

/*
        case DMD_DVBT_DRV_CMD_GetSignalStrengthWithRFPower:
            DMD_DBG(ULOGD("DEMOD","DVBTIoctl - MDrv_DMD_DVBT_GetSignalStrengthWithRFPower\n"));
            bRet = psDVBTInstPri->fpDVBTGetSignalStrengthWithRFPower(((PDVBT_GetSignalStrengthWithRFPower_PARAM)pArgs)->u16Strength, ((PDVBT_GetSignalStrengthWithRFPower_PARAM)pArgs)->fRFPowerDbm);
	     ((PDVBT_GetSignalStrengthWithRFPower_PARAM)pArgs)->ret=bRet;
            break;
*/

/*
        case DMD_DVBT_DRV_CMD_GetSignalQuality:
            DMD_DBG(ULOGD("DEMOD","DVBTIoctl - MDrv_DMD_DVBT_GetSignalQuality\n"));
            u32Ret = psDVBTInstPri->fpDVBTGetSignalQuality(((PDVBT_GetSignalQuality_PARAM)pArgs)->u16Quality);
            break;
*/

/*
        case DMD_DVBT_DRV_CMD_GetSignalQualityWithRFPower:
            DMD_DBG(ULOGD("DEMOD","DVBTIoctl - MDrv_DMD_DVBT_GetSignalQualityWithRFPower\n"));
            bRet = psDVBTInstPri->fpDVBTGetSignalQualityWithRFPower(((PDVBT_GetSignalQualityWithRFPower_PARAM)pArgs)->u16Quality, ((PDVBT_GetSignalQualityWithRFPower_PARAM)pArgs)->fRFPowerDbm);
	     ((PDVBT_GetSignalQualityWithRFPower_PARAM)pArgs)->ret=bRet;
            break;
*/
         /* bryan temp mark*/
         // arthur
        case DMD_DVBT_DRV_CMD_GetSNR:
            DMD_DBG(ULOGD("DEMOD","DVBTIoctl - MDrv_DMD_DVBT_GetSNR\n"));
            bRet = psDVBTInstPri->fpDVBTGetSNR(((PDVBT_GetSNR_PARAM)pArgs)->noise_power_reg);
	          ((PDVBT_GetSNR_PARAM)pArgs)->ret=bRet;
            break;

       case DMD_DVBT_DRV_CMD_GetIFAGC:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DVBTIoctl - MDrv_DMD_DVBT_GetIFAGC\n"));
		        bRet=psDVBTInstPri->fpDVBT_GetIFAGC(((PDVBT_GetIFAGC_PARAM)pArgs)->ifagc_reg,\
		        ((PDVBT_GetIFAGC_PARAM)pArgs)->ifagc_reg_lsb,\
		        ((PDVBT_GetIFAGC_PARAM)pArgs)->ifagc_err_reg);
		        ((PDVBT_GetIFAGC_PARAM)pArgs)->ret=bRet;
            break;

         /*bryan temp mark */

        case DMD_DVBT_DRV_CMD_GetPostViterbiBer:
            DMD_DBG(ULOGD("DEMOD","DVBTIoctl - MDrv_DMD_DVBT_GetPostViterbiBer\n"));
            bRet = psDVBTInstPri->fpDVBTGetPostViterbiBer(((PDVBT_GetPostViterbiBer_PARAM)pArgs)->BitErrPeriod_reg, ((PDVBT_GetPostViterbiBer_PARAM)pArgs)->BitErr_reg, ((PDVBT_GetPostViterbiBer_PARAM)pArgs)->PktErr_reg);
	          ((PDVBT_GetPostViterbiBer_PARAM)pArgs)->ret=bRet;
            break;

	/*bryan temp mark */
        #if(0)
        case DMD_DVBT_DRV_CMD_GetPreViterbiBer:
            DMD_DBG(ULOGD("DEMOD","DVBTIoctl - MDrv_DMD_DVBT_GetPreViterbiBer\n"));
            bRet = psDVBTInstPri->fpDVBTGetPreViterbiBer(((PDVBT_GetPreViterbiBer_PARAM)pArgs)->ber);
	      ((PDVBT_GetPreViterbiBer_PARAM)pArgs)->ret=bRet;
            break;
         #endif

            case DMD_DVBT_DRV_CMD_GetPacketErr:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DVBTIoctl - MDrv_DMD_DVBT_GetPacketErr\n"));
            bRet = psDVBTInstPri->fpDVBTGetPacketErr(((PDVBT_GetPacketErr_PARAM)pArgs)->pktErr);
	     ((PDVBT_GetPacketErr_PARAM)pArgs)->ret=bRet;
            break;
            case DMD_DVBT_DRV_CMD_GetTPSInfo:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DVBToctl - MDrv_DMD_DVBT_GetTPSInfo\n"));
            bRet = psDVBTInstPri->fpDVBTGetTPSInfo(((PDVBT_GetTPSInfo_PARAM)pArgs)->u16Info);
	     ((PDVBT_GetTPSInfo_PARAM)pArgs)->ret=bRet;
            break;
            case DMD_DVBT_DRV_CMD_GetCellID:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DVBTIoctl - MDrv_DMD_DVBT_GetCellID\n"));
            bRet = psDVBTInstPri->fpDVBTGetCellID(((PDVBT_GetCellID_PARAM)pArgs)->u16CellID);
	      ((PDVBT_GetCellID_PARAM)pArgs)->ret=bRet;
            break;

        /*bryan temp mark*/
        #if(0)
        case DMD_DVBT_DRV_CMD_GetFreqOffset:
            DMD_DBG(ULOGD("DEMOD","DVBTIoctl - MDrv_DMD_DVBT_GetFreqOffset\n"));
            bRet = psDVBTInstPri->fpDVBTGetFreqOffset(((PDVBT_GetFreqOffset_PARAM)pArgs)->pFreqOff);
	      ((PDVBT_GetFreqOffset_PARAM)pArgs)->ret=bRet;
            break;
         #endif
        /*bryan temp mark*/
        #if(0)
        case DMD_DVBT_DRV_CMD_NORDIGSSITableWrite:
            DMD_DBG(ULOGD("DEMOD","DVBTIoctl - MDrv_DMD_DVBT_NORDIG_SSI_Table_Write\n"));
           bRet = psDVBTInstPri->fpDVBTNORDIGSSITableWrite(((PDVBT_NORDIGSSITableWrite_PARAM)pArgs)->constel, ((PDVBT_NORDIGSSITableWrite_PARAM)pArgs)->code_rate, ((PDVBT_NORDIGSSITableWrite_PARAM)pArgs)->write_value);
            ((PDVBT_NORDIGSSITableWrite_PARAM)pArgs)->ret=bRet;
           break;
        case DMD_DVBT_DRV_CMD_NORDIGSSITableRead:
            DMD_DBG(ULOGD("DEMOD","DVBTIoctl - MDrv_DMD_DVBT_NORDIG_SSI_Table_Read\n"));
           bRet = psDVBTInstPri->fpDVBTNORDIGSSITableRead(((PDVBT_NORDIGSSITableRead_PARAM)pArgs)->constel, ((PDVBT_NORDIGSSITableRead_PARAM)pArgs)->code_rate, ((PDVBT_NORDIGSSITableRead_PARAM)pArgs)->read_value);
           ((PDVBT_NORDIGSSITableRead_PARAM)pArgs)->ret=bRet;
           break;
        #endif

        case DMD_DVBT_DRV_CMD_SetPowerState:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DVBTIoctl - MDrv_DMD_DVBT_SetPowerState\n"));
            bRet = psDVBTInstPri->fpDVBTSetPowerState(((PDVBT_SetPowerState_PARAM)pArgs)->u16PowerState);
	     ((PDVBT_SetPowerState_PARAM)pArgs)->ret=bRet;
            break;
         default:
            break;
    }

    //jway suggest UtopiaResourceRelease(pResource);

    return (bRet ? UTOPIA_STATUS_SUCCESS : UTOPIA_STATUS_FAIL);
}

MS_U32 DVBTClose(void* pInstance)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DVBTClose\n"));

    UtopiaInstanceDelete(pInstance);

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 DVBTStr(MS_U32 u32PowerState, void* pModule)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    MS_U32 u32Ret = 0;

    //UtopiaModuleGetSTRPrivate(pModule, (void**));
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DVBTStr\n"));

    if (u32PowerState == E_POWER_SUSPEND)
    {
        /* Please Implement Module Suspend Flow Here. */
        u32Ret = DMD_DVBT_SetPowerState(E_POWER_SUSPEND);

        if(u32Ret == TRUE)
          u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
        else
          u32Return = UTOPIA_STATUS_FAIL;

        DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c][DVBTStr] SUSPEND !\n"));
    }
    else if (u32PowerState == E_POWER_RESUME)
    {
        /* Please Implement Module Resume Flow Here. */
        u32Ret = DMD_DVBT_SetPowerState(E_POWER_RESUME);

        if(u32Ret == TRUE)
          u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
        else
          u32Return = UTOPIA_STATUS_FAIL;

        DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c][DVBTStr] RESUME !\n"));
    }
    else
    {
        u32Return = UTOPIA_STATUS_FAIL;
        DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c][DVBTStr] OTHERS !\n"));
    }

    return u32Return;// for success
}

void DVBTRegisterToUtopia(void)
{
    // 1. deal with module

    void* pUtopiaModule = NULL;
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c]DVBTRegisterToUtopia\n"));
//    UtopiaModuleCreate(MODULE_DVBT, 8, &pUtopiaModule);   //bryan: why this was taken out
    UtopiaModuleCreate(MODULE_DVBT, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)DVBTOpen, (FUtopiaClose)DVBTClose, (FUtopiaIOctl)DVBTIoctl);

   // Utopia2K STR
#if defined(MSOS_TYPE_LINUX_KERNEL)
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT_v2.c][DVBTRegisterToUtopia] KERNEL DVBTStr!\n"));
    UtopiaModuleSetupSTRFunctionPtr(pUtopiaModule,(FUtopiaSTR)DVBTStr);
#endif
}


