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
#include "drvDMD_INTERN_DVBT2_v2.h"
#include "halDMD_INTERN_DVBT2.h"
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
//  Local Variables
//-------------------------------------------------------------------------------------------------
MS_BOOL bIsDVBT2 = FALSE;       // Usage for STR


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
typedef MS_BOOL                       (*IOCTL_DVBT2_Init)(DMD_DVBT2_InitData_Transform *pDMD_DVBT2_InitData, MS_U32 u32InitDataLen);
typedef MS_BOOL                       (*IOCTL_DVBT2_Exit)(void);
typedef MS_BOOL                       (*IOCTL_DVBT2_SetDbgLevel)(DMD_T2_DbgLv u8DbgLevel);
typedef DMD_DVBT2_Info*           (*IOCTL_DVBT2_GetInfo)(DMD_DVBT2_INFO_TYPE eInfoType);
typedef MS_BOOL                        (*IOCTL_DVBT2_GetLibVer)(const MSIF_Version **ppVersion);
typedef MS_BOOL                        (*IOCTL_DVBT2_GetFWVer)(MS_U16 *ver);
typedef MS_BOOL                        (*IOCTL_DVBT2_GetReg)(MS_U16 u16Addr, MS_U8 *pu8Data);
typedef MS_BOOL                        (*IOCTL_DVBT2_SetReg)(MS_U16 u16Addr, MS_U8 u8Data);
typedef MS_BOOL                        (*IOCTL_DVBT2_SetSerialControl)(MS_BOOL bEnable);
typedef MS_BOOL                        (*IOCTL_DVBT2_SetReset)(void);
typedef MS_BOOL                        (*IOCTL_DVBT2_SetConfig)(DMD_DVBT2_RF_CHANNEL_BANDWIDTH BW, MS_BOOL bSerialTS, MS_U8 u8PlpID);
typedef MS_BOOL                        (*IOCTL_DVBT2_SetActive)(MS_BOOL bEnable);
typedef MS_BOOL                        (*IOCTL_DVBT2_GetLock)(DMD_DVBT2_GETLOCK_TYPE eType, DMD_T2_LOCK_STATUS *eLockStatus);
//typedef MS_BOOL                        (*IOCTL_DVBT2_GetSignalStrength)(MS_U16 *u16Strength);
//typedef MS_BOOL                        (*IOCTL_DVBT2_GetSignalStrengthWithRFPower)(MS_U16 *u16Strength, float fRFPowerDbm);
//typedef MS_BOOL                        (*IOCTL_DVBT2_GetSignalQuality)(MS_U16 *u16Quality);
//typedef MS_BOOL                        (*IOCTL_DVBT2_GetSignalQualityWithRFPower)(MS_U16 *u16Quality, float fRFPowerDbm);
typedef MS_BOOL                        (*IOCTL_DVBT2_GetSNR)(MS_U16 *u16_snr100,  MS_U8 *snr_cali, MS_U8 *u8_gi);
typedef MS_BOOL                        (*IOCTL_DVBT2_GetPostLdpcBer)(MS_U32 *BitErr_reg, MS_U16 *BitErrPeriod_reg, MS_U16 *FecType);
typedef MS_BOOL                        (*IOCTL_DVBT2_GetPreLdpcBer)(MS_U32 *BitErr_reg, MS_U16 *BitErrPeriod_reg, MS_U16 *FecType);
typedef MS_BOOL                        (*IOCTL_DVBT2_GetPacketErr)(MS_U16 *pktErr);
typedef MS_BOOL                        (*IOCTL_DVBT2_GetL1Info)(MS_U16 *u16Info, DMD_DVBT2_SIGNAL_INFO eSignalType);
typedef MS_BOOL                        (*IOCTL_DVBT2_GetFreqOffset)(MS_U32 *CfoTd_reg, MS_U32 *CfoFd_reg, MS_U32 *Icfo_reg, MS_U8 *fft_reg);
//typedef MS_BOOL                        (*IOCTL_DVBT2_NORDIG_SSI_Table_Write)(DMD_T2_CONSTEL constel, DMD_T2_CODERATE code_rate, float write_value);
//typedef MS_BOOL                        (*IOCTL_DVBT2_NORDIG_SSI_Table_Read)(DMD_T2_CONSTEL constel, DMD_T2_CODERATE code_rate, float *read_value);
typedef MS_U32                           (*IOCTL_DVBT2_SetPowerState)(EN_POWER_MODE u16PowerState);
typedef MS_BOOL                         (*IOCTL_DVBT2_GetPlpBitMap)(MS_U8* u8PlpBitMap);
typedef MS_BOOL				    (*IOCTL_DVBT2_GetPlpGroupID)(MS_U8 u8PlpID, MS_U8* u8GroupID);
typedef MS_BOOL                         (*IOCTL_DVBT2_SetPlpID)(MS_U8 u8PlpID, MS_U8 u8GroupID);


typedef struct DVBT2_INSTANT_PRIVATE
{
	IOCTL_DVBT2_Init                           fpDVBT2_Init ;
	IOCTL_DVBT2_Exit                           fpDVBT2_Exit;
	IOCTL_DVBT2_SetDbgLevel                    fpDVBT2_SetDbgLevel;
	IOCTL_DVBT2_GetInfo                        fpDVBT2_GetInfo;
	IOCTL_DVBT2_GetLibVer                      fpDVBT2_GetLibVer;
	IOCTL_DVBT2_GetFWVer                       fpDVBT2_GetFWVer;
	IOCTL_DVBT2_GetReg                         fpDVBT2_GetReg;
	IOCTL_DVBT2_SetReg                         fpDVBT2_SetReg;
	IOCTL_DVBT2_SetSerialControl               fpDVBT2_SetSerialControl;
	IOCTL_DVBT2_SetReset                       fpDVBT2_SetReset;
	IOCTL_DVBT2_SetConfig                      fpDVBT2_SetConfig;
	IOCTL_DVBT2_SetActive                      fpDVBT2_SetActive;
	IOCTL_DVBT2_GetLock                        fpDVBT2_GetLock;
//	IOCTL_DVBT2_GetSignalStrength              fpDVBT2_GetSignalStrength;
//	IOCTL_DVBT2_GetSignalStrengthWithRFPower   fpDVBT2_GetSignalStrengthWithRFPower;
//	IOCTL_DVBT2_GetSignalQuality               fpDVBT2_GetSignalQuality;
//	IOCTL_DVBT2_GetSignalQualityWithRFPower    fpDVBT2_GetSignalQualityWithRFPower ;
	IOCTL_DVBT2_GetSNR                         fpDVBT2_GetSNR;
	IOCTL_DVBT2_GetPostLdpcBer                 fpDVBT2_GetPostLdpcBer;
	IOCTL_DVBT2_GetPreLdpcBer                  fpDVBT2_GetPreLdpcBer;
	IOCTL_DVBT2_GetPacketErr                   fpDVBT2_GetPacketErr;
	IOCTL_DVBT2_GetL1Info                      fpDVBT2_GetL1Info;
	IOCTL_DVBT2_GetFreqOffset                  fpDVBT2_GetFreqOffset;
//	IOCTL_DVBT2_NORDIG_SSI_Table_Write         fpDVBT2_NORDIG_SSI_Table_Write;
//	IOCTL_DVBT2_NORDIG_SSI_Table_Read          fpDVBT2_NORDIG_SSI_Table_Read;
	IOCTL_DVBT2_SetPowerState                  fpDVBT2_SetPowerState;
	IOCTL_DVBT2_GetPlpBitMap                   fpDVBT2_GetPlpBitMap;
	IOCTL_DVBT2_GetPlpGroupID              fpDVBT2_GetPlpGroupID;
	IOCTL_DVBT2_SetPlpID                       fpDVBT2_SetPlpID;
} DVBT2_INSTANT_PRIVATE;


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#define DMD_LOCK()      \
    do{                         \
        MS_ASSERT(MsOS_In_Interrupt() == FALSE); \
        if (_s32DMD_DVBT2_Mutex == -1) return FALSE; \
        if (_u8DMDDbgLevel == DMD_T2_DBGLV_DEBUG) printf("%s lock mutex\n", __FUNCTION__);\
        MsOS_ObtainMutex(_s32DMD_DVBT2_Mutex, MSOS_WAIT_FOREVER);\
        }while(0)

#define DMD_UNLOCK()      \
    do{                         \
        MsOS_ReleaseMutex(_s32DMD_DVBT2_Mutex);\
        if (_u8DMDDbgLevel == DMD_T2_DBGLV_DEBUG) printf("%s unlock mutex\n", __FUNCTION__); \
        }while(0)

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if 1
static MSIF_Version _drv_dmd_dvbt2_intern_version = {
    .MW = { DMD_DVBT2_INTERN_VER, },
};
#else
static MSIF_Version _drv_dmd_dvbt_intern_version;
#endif

static DMD_DVBT2_InitData_Transform _sDMD_DVBT2_InitData;
static DMD_T2_DbgLv _u8DMDDbgLevel=DMD_T2_DBGLV_NONE;
static MS_S32 _s32DMD_DVBT2_Mutex=-1;
static DMD_DVBT2_Info sDMD_DVBT2_Info;
//static MS_U16 u16DMD_DVBT2_P1_Timeout = 600, u16DMD_DVBT2_FEC_Timeout=6000;
static MS_U16 u16DMD_DVBT2_P1_Timeout = 1000, u16DMD_DVBT2_FEC_Timeout=6000;
static MS_U32 u32DMD_DVBT2_IfFrequency = 5000L, u32DMD_DVBT2_FsFrequency = 24000L;
//static MS_U8 u8DMD_DVBT2_IQSwap=0;
static DMD_DVBT2_RF_CHANNEL_BANDWIDTH eDMD_DVBT2_BandWidth=E_DMD_T2_RF_BAND_8MHz;
MS_U32  u32DMD_DVBT2_DRAM_START_ADDR;
MS_U32  u32DMD_DVBT2_EQ_START_ADDR;
MS_U32  u32DMD_DVBT2_TDI_START_ADDR;
MS_U32  u32DMD_DVBT2_DJB_START_ADDR;
MS_U32  u32DMD_DVBT2_FW_START_ADDR;
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define DMD_DBG(x)          (x)
#else
#define DMD_DBG(x)          //(x)
#endif


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL DMD_DVBT2_Init(DMD_DVBT2_InitData_Transform *pDMD_DVBT2_InitData, MS_U32 u32InitDataLen)
{
    char pDMD_DVBT2_MutexString[16];
    MS_U8 u8ADCIQMode = 0, u8PadSel = 0, bPGAEnable = 0, u8PGAGain = 5;
    MS_BOOL bRFAGCTristateEnable = 1;
    MS_BOOL bIFAGCTristateEnable = 0;

    bIsDVBT2 = TRUE;

    if (_s32DMD_DVBT2_Mutex != -1)
    {
        DMD_DBG(printf("MDrv_DMD_DVBT2_Init more than once\n"));
        return FALSE;
    }

    if (NULL == strncpy(pDMD_DVBT2_MutexString,"Mutex DMD DVBT2",16))
    {
        DMD_DBG(printf("MDrv_DMD_DVBT2_Init strcpy Fail\n"));
        return FALSE;
    }
    _s32DMD_DVBT2_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, pDMD_DVBT2_MutexString, MSOS_PROCESS_SHARED);
    if (_s32DMD_DVBT2_Mutex == -1)
    {
        DMD_DBG(printf("MDrv_DMD_DVBT2_Init Create Mutex Fail\n"));
        return FALSE;
    }
    //_u8DMDDbgLevel = DMD_DBGLV_DEBUG;
    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_T2_DBGLV_INFO)
    {
        printf("MDrv_DMD_DVBT2_Init\n");
    }
    #endif

    if ( sizeof(_sDMD_DVBT2_InitData) == u32InitDataLen)
    {
        memcpy(&_sDMD_DVBT2_InitData, pDMD_DVBT2_InitData, u32InitDataLen);
    }
    else
    {
        DMD_DBG(printf("MDrv_DMD_DVBT2_Init input data structure incorrect\n"));
        return FALSE;
    }

    if (_sDMD_DVBT2_InitData.u8SarChannel != 0xFF)
    {
//        MDrv_SAR_Adc_Config(_sDMD_DVBT2_InitData.u8SarChannel, TRUE);
    }

    DMD_LOCK();
    MDrv_SYS_DMD_VD_MBX_SetType(E_DMD_VD_MBX_TYPE_DVBT2);
    HAL_DMD_RegInit();

    if (_sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt != NULL)
    {
        if (_sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[0]>=2)
        {
            bRFAGCTristateEnable = (_sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[3] & (BIT_(0))) ? TRUE : FALSE; // RFAGC tristate control
            bIFAGCTristateEnable = (_sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[3] & (BIT_(4))) ? TRUE : FALSE; // IFAGC tristate control
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

    if (_sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt != NULL)
    {
        if (_sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[0]>=3)
        {
            u32DMD_DVBT2_IfFrequency = _sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[4]; // IF frequency
            u32DMD_DVBT2_IfFrequency =  (u32DMD_DVBT2_IfFrequency<<8)+_sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[5]; // IF frequency
            u32DMD_DVBT2_IfFrequency =  (u32DMD_DVBT2_IfFrequency<<8)+_sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[6]; // IF frequency
            u32DMD_DVBT2_IfFrequency =  (u32DMD_DVBT2_IfFrequency<<8)+_sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[7]; // IF frequency
            u32DMD_DVBT2_FsFrequency = _sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[8]; // FS frequency
            u32DMD_DVBT2_FsFrequency =  (u32DMD_DVBT2_FsFrequency<<8)+_sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[9]; // FS frequency
            u32DMD_DVBT2_FsFrequency =  (u32DMD_DVBT2_FsFrequency<<8)+_sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[10]; // FS frequency
            u32DMD_DVBT2_FsFrequency =  (u32DMD_DVBT2_FsFrequency<<8)+_sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[11]; // FS frequency
            //u8DMD_DVBT2_IQSwap = _sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[12]; // IQ Swap

            u8ADCIQMode = _sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[13]; // u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
            u8PadSel = _sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[14]; // u8PadSel : 0=Normal, 1=analog pad
            bPGAEnable = _sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[15]; // bPGAEnable : 0=disable, 1=enable
            u8PGAGain = _sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[16]; // u8PGAGain : default 5
        }
        else
        {

        }
    }
    else
    {

    }
    #ifdef MS_DEBUG
    printf("u32DMD_DVBT2_IfFrequency %ld\n",u32DMD_DVBT2_IfFrequency);
    printf("u32DMD_DVBT2_FsFrequency %ld\n",u32DMD_DVBT2_FsFrequency);
    #endif

    u16DMD_DVBT2_P1_Timeout = 1000;	//600;
    u16DMD_DVBT2_FEC_Timeout = 6000;
    if (_sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt != NULL)
    {
        if (_sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[0]>=4)
        {
            u16DMD_DVBT2_P1_Timeout = _sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[17]; // P1 timeout in ms
            u16DMD_DVBT2_P1_Timeout =  (u16DMD_DVBT2_P1_Timeout<<8)+_sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[18];
            if (u16DMD_DVBT2_P1_Timeout < 600) u16DMD_DVBT2_P1_Timeout=600;
            //printf("u16DMD_DVBT2_P1_Timeout %d\n",u16DMD_DVBT2_P1_Timeout);

            u16DMD_DVBT2_FEC_Timeout = _sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[19]; // FEC timeout in ms
            u16DMD_DVBT2_FEC_Timeout =  (u16DMD_DVBT2_FEC_Timeout<<8)+_sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[20];
            if (u16DMD_DVBT2_FEC_Timeout < 6000) u16DMD_DVBT2_FEC_Timeout=6000;
            //printf("u16DMD_DVBT2_FEC_Timeout %d\n",u16DMD_DVBT2_FEC_Timeout);
        }
        else
        {
        }
    }
    else
    {
    }

    //u32DMD_DVBT2_DRAM_START_ADDR = _sDMD_DVBT2_InitData.u32DramStartAddr;
    u32DMD_DVBT2_EQ_START_ADDR = _sDMD_DVBT2_InitData.u32EqStartAddr;   //0x14B23000;
    u32DMD_DVBT2_TDI_START_ADDR = _sDMD_DVBT2_InitData.u32TdiStartAddr; //0x14610000;
    u32DMD_DVBT2_DJB_START_ADDR = _sDMD_DVBT2_InitData.u32DjbStartAddr; //0x14BB9000;
    u32DMD_DVBT2_FW_START_ADDR = _sDMD_DVBT2_InitData.u32FwStartAddr;   //0x14608000;

    if (bIFAGCTristateEnable)
    {
        MDrv_SYS_SetAGCPadMux(E_SYS_DTV_AGC_PAD_SET_ALL_OFF);
    }
    else
    {
        MDrv_SYS_SetAGCPadMux(E_SYS_DTV_AGC_PAD_SET);
    }

    if (_sDMD_DVBT2_InitData.u8DMD_DVBT2_DSPRegInitExt != NULL)
    {
        if (_sDMD_DVBT2_InitData.u8DMD_DVBT2_DSPRegInitExt[0]>=1)
        {
            INTERN_DVBT2_Power_On_Initialization(bRFAGCTristateEnable, u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain, _sDMD_DVBT2_InitData.u8DMD_DVBT2_DSPRegInitExt, _sDMD_DVBT2_InitData.u8DMD_DVBT2_DSPRegInitSize);
        }
        else
        {
            printf("u8DMD_DVBT2_DSPRegInitExt Error\n");
        }
    }
    else
    {
        INTERN_DVBT2_Power_On_Initialization(bRFAGCTristateEnable, u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain,  NULL, 0);
    }

    INTERN_DVBT2_Version(&sDMD_DVBT2_Info.u16Version);
    DMD_UNLOCK();
    #ifdef MS_DEBUG
    printf("firmware version: %x\n",sDMD_DVBT2_Info.u16Version);
    #endif
    return TRUE;
}


MS_BOOL DMD_DVBT2_Exit(void)
{
    MS_BOOL bRet;

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_T2_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT2_Exit\n");
    }
    #endif
    bIsDVBT2 = FALSE;
    DMD_LOCK();
    bRet = INTERN_DVBT2_Exit();
    DMD_UNLOCK();
    MsOS_DeleteMutex(_s32DMD_DVBT2_Mutex);
    _s32DMD_DVBT2_Mutex= -1;
    return bRet;
}

MS_BOOL DMD_DVBT2_SetDbgLevel(DMD_T2_DbgLv u8DbgLevel)
{
    DMD_LOCK();
    _u8DMDDbgLevel = u8DbgLevel;
    DMD_UNLOCK();
    return TRUE;
}

DMD_DVBT2_Info* DMD_DVBT2_GetInfo(DMD_DVBT2_INFO_TYPE eInfoType)
{
    DMD_LOCK();
    switch (eInfoType)
    {
        case E_DMD_DVBT2_MODULATION_INFO:
            INTERN_DVBT2_Show_Modulation_info();
            break;
        case E_DMD_DVBT2_DEMOD_INFO:
            INTERN_DVBT2_Show_Demod_Info();
            break;
        case E_DMD_DVBT2_LOCK_INFO:
            INTERN_DVBT2_Show_Lock_Info();
            break;
        case E_DMD_DVBT2_PRESFO_INFO:
            INTERN_DVBT2_Show_PRESFO_Info();
            break;
        case E_DMD_DVBT2_LOCK_TIME_INFO:
            INTERN_DVBT2_Show_Lock_Time_Info();
            break;
        case E_DMD_DVBT2_BER_INFO:
            INTERN_DVBT2_Show_BER_Info();
            break;
        case E_DMD_DVBT2_AGC_INFO:
            INTERN_DVBT2_Show_AGC_Info();
            break;
        default:
            #ifdef MS_DEBUG
            printf("MDrv_DMD_DVBT2_GetInfo %d Error\n", eInfoType);
            #endif
            break;
    }
    DMD_UNLOCK();
    return &sDMD_DVBT2_Info;
}

MS_BOOL DMD_DVBT2_GetLibVer(const MSIF_Version **ppVersion)
{
    DMD_LOCK();
    if (!ppVersion)
    {
        return FALSE;
    }

    *ppVersion = &_drv_dmd_dvbt2_intern_version;
    DMD_UNLOCK();
    return TRUE;
}

MS_BOOL DMD_DVBT2_GetFWVer(MS_U16 *ver)
{

    MS_BOOL bRet;

    DMD_LOCK();

    bRet = INTERN_DVBT2_Version(ver);
    //printf("MDrv_DMD_DVBT2_GetFWVer %x\n",*ver);
    DMD_UNLOCK();

    return bRet;

}

MS_BOOL DMD_DVBT2_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=MDrv_SYS_DMD_VD_MBX_ReadReg(u16Addr, pu8Data);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_T2_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT2_GetReg %x %x\n", u16Addr, *pu8Data);
    }
    #endif

    return bRet;
}

MS_BOOL DMD_DVBT2_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet;

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_T2_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT2_SetReg %x %x\n", u16Addr, u8Data);
    }
    #endif

    DMD_LOCK();
    bRet=MDrv_SYS_DMD_VD_MBX_WriteReg(u16Addr, u8Data);
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL DMD_DVBT2_SetSerialControl(MS_BOOL bEnable)
{
    MS_BOOL bRet;
    MS_U8 u8TSClk;

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_T2_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT2_SetSerialControl %x\n", bEnable);
    }
    #endif

    DMD_LOCK();
    if (_sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt != NULL)
    {
        if (_sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[0]>=1)
        {
            u8TSClk = _sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[2]; // TS_CLK
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
    bRet=INTERN_DVBT2_Serial_Control(bEnable, u8TSClk);
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL DMD_DVBT2_SetReset(void)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet = INTERN_DVBT2_SoftReset();
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL DMD_DVBT2_SetConfig(DMD_DVBT2_RF_CHANNEL_BANDWIDTH BW, MS_BOOL bSerialTS, MS_U8 u8PlpID)
{
    //return MDrv_DMD_DVBT2_SetConfigHPLPSetIF(BW, bSerialTS, bPalBG, 0, u32DMD_DVBT2_IfFrequency, u32DMD_DVBT2_FsFrequency, u8DMD_DVBT2_IQSwap);
    MS_BOOL bRet;
    MS_U8 u8TSClk;

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_T2_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT2_SetConfig %d %d %d\n", BW, bSerialTS, u8PlpID);
    }
    #endif

    DMD_LOCK();
    if (_sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt != NULL)
    {
        if (_sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[0]>=1)
        {
            u8TSClk = _sDMD_DVBT2_InitData.u8DMD_DVBT2_InitExt[2]; // TS_CLK
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

    bRet=INTERN_DVBT2_Config(BW, bSerialTS, u8TSClk, u32DMD_DVBT2_IfFrequency, u8PlpID);
    eDMD_DVBT2_BandWidth=BW;
    DMD_UNLOCK();
    return bRet;
}


MS_BOOL DMD_DVBT2_SetActive(MS_BOOL bEnable)
{
    MS_BOOL bRet;

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_T2_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT2_SetActive %d\n", bEnable);
    }
    #endif

    DMD_LOCK();
    bRet=INTERN_DVBT2_Active(bEnable);
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL DMD_DVBT2_GetLock(DMD_DVBT2_GETLOCK_TYPE eType, DMD_T2_LOCK_STATUS *eLockStatus)
{
    MS_BOOL bRet=TRUE;
    DMD_LOCK();

    if ( eType == E_DMD_DVBT2_GETLOCK ) // for channel scan
    {
        *eLockStatus = INTERN_DVBT2_Lock(u16DMD_DVBT2_P1_Timeout, u16DMD_DVBT2_FEC_Timeout);
    }
    else
    {
        if (INTERN_DVBT2_GetLock(eType) == TRUE)
        {
            *eLockStatus = E_DMD_T2_LOCK;
        }
        else
        {
            *eLockStatus = E_DMD_T2_UNLOCK;
        }
    }
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_T2_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT2_GetLock %d\n", bRet);
    }
    #endif
    return bRet;
}

/*  implement in drv layer
MS_BOOL MDrv_DMD_DVBT2_GetSignalStrength(MS_U16 *u16Strength)
{
    return MDrv_DMD_DVBT2_GetSignalStrengthWithRFPower(u16Strength, 200.0f);
}
*/

#if 0
MS_BOOL DMD_DVBT2_GetSignalStrengthWithRFPower(MS_U16 *u16Strength, float fRFPowerDbm)
{
    MS_U8 u8SarValue;
    MS_BOOL bRet;

    DMD_LOCK();
    if (_sDMD_DVBT2_InitData.u8SarChannel != 0xFF)
    {
        u8SarValue=MDrv_SAR_Adc_GetValue(_sDMD_DVBT2_InitData.u8SarChannel);
    }
    else
    {
        u8SarValue=0xFF;
    }
    bRet=INTERN_DVBT2_GetSignalStrength(u16Strength, (const DMD_DVBT2_InitData *)(&_sDMD_DVBT2_InitData), u8SarValue, fRFPowerDbm);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_T2_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT2_GetSignalStrength %d\n", *u16Strength);
    }
    #endif
    return bRet;
}
#endif

/*   implement in drv layer
MS_BOOL DMD_DVBT2_GetSignalQuality(MS_U16 *u16Quality)
{
    return MDrv_DMD_DVBT2_GetSignalQualityWithRFPower(u16Quality, 200.0f);
}
*/

#if 0
MS_BOOL DMD_DVBT2_GetSignalQualityWithRFPower(MS_U16 *u16Quality, float fRFPowerDbm)
{
    MS_U8 u8SarValue=0;
    MS_BOOL bRet=0;

    DMD_LOCK();
    if (_sDMD_DVBT2_InitData.u8SarChannel != 0xFF)
    {
        u8SarValue=MDrv_SAR_Adc_GetValue(_sDMD_DVBT2_InitData.u8SarChannel);
    }
    else
    {
        u8SarValue=0xFF;
    }
    bRet=INTERN_DVBT2_GetSignalQuality(u16Quality, (const DMD_DVBT2_InitData *)(&_sDMD_DVBT2_InitData), u8SarValue, fRFPowerDbm);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_T2_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT2_GetSignalQuality %d\n", *u16Quality);
    }
    #endif
    return bRet;
}
#endif

#if 0
MS_BOOL DMD_DVBT2_GetSNR(float *fSNR)
{
    DMD_LOCK();
    *fSNR=INTERN_DVBT2_GetSNR();
    DMD_UNLOCK();

    return TRUE;
}

MS_BOOL DMD_DVBT2_GetPostLdpcBer(float *ber)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBT2_GetPostLdpcBer(ber);
    DMD_UNLOCK();

    return bRet;
}
#else
MS_BOOL DMD_DVBT2_GetSNR (MS_U16 *u16_snr100,  MS_U8 *snr_cali, MS_U8 *u8_gi)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBT2_GetSNR (u16_snr100,  snr_cali, u8_gi);
    DMD_UNLOCK();

    return TRUE;
}

MS_BOOL DMD_DVBT2_GetPostLdpcBer(MS_U32 *BitErr_reg, MS_U16 *BitErrPeriod_reg, MS_U16 *FecType)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBT2_GetPostLdpcBer(BitErr_reg, BitErrPeriod_reg, FecType);
    DMD_UNLOCK();

    return bRet;
}
#endif

MS_BOOL DMD_DVBT2_GetPreLdpcBer(MS_U32 *BitErr_reg, MS_U16 *BitErrPeriod_reg, MS_U16 *FecType)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBT2_GetPreLdpcBer(BitErr_reg, BitErrPeriod_reg, FecType);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL DMD_DVBT2_GetPacketErr(MS_U16 *pktErr)
{
    MS_BOOL bRet;
//    float   fBER;

    DMD_LOCK();
 //   INTERN_DVBT2_GetPostLdpcBer(&fBER);
    bRet=INTERN_DVBT2_GetPacketErr(pktErr);
//    if ((*pktErr ==1) && (fBER<= 0.000001)) // for no signal case, from Oga
//    {
//        *pktErr = 0x3FF;
//    }
    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_T2_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT2_GetPacketErr %d\n", *pktErr);
    }
    #endif
    DMD_UNLOCK();

    return bRet;
}



MS_BOOL DMD_DVBT2_GetL1Info(MS_U16 *u16Info, DMD_DVBT2_SIGNAL_INFO eSignalType)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBT2_Get_L1_Parameter(u16Info, eSignalType );
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL DMD_DVBT2_GetFreqOffset(MS_U32 *CfoTd_reg, MS_U32 *CfoFd_reg, MS_U32 *Icfo_reg, MS_U8 *fft_reg)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBT2_Get_FreqOffset(CfoTd_reg, CfoFd_reg, Icfo_reg, fft_reg);
    DMD_UNLOCK();

    return bRet;
}


#if 0
MS_BOOL DMD_DVBT2_NORDIG_SSI_Table_Write(DMD_T2_CONSTEL constel, DMD_T2_CODERATE code_rate, float write_value)
{
    return INTERN_DVBT2_NORDIG_SSI_Table_Write(constel, code_rate, write_value);
}

MS_BOOL DMD_DVBT2_NORDIG_SSI_Table_Read(DMD_T2_CONSTEL constel, DMD_T2_CODERATE code_rate, float *read_value)
{
    return INTERN_DVBT2_NORDIG_SSI_Table_Read(constel, code_rate, read_value);
}
#endif

MS_U32 DMD_DVBT2_SetPowerState(EN_POWER_MODE u16PowerState)
{
    static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    u32Return = u32Return;

    if(bIsDVBT2 == TRUE)
    {

	    if (u16PowerState == E_POWER_SUSPEND)
		{
          DMD_DVBT2_Exit();
	        _prev_u16PowerState = u16PowerState;
	        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
	    }
	    else if (u16PowerState == E_POWER_RESUME)
	    {
	        if (_prev_u16PowerState == E_POWER_SUSPEND)
	        {
	            DMD_DVBT2_Init(&_sDMD_DVBT2_InitData, sizeof(_sDMD_DVBT2_InitData));
	            _prev_u16PowerState = u16PowerState;
	            u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
	        }
	        else
	        {
	            printf("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
	            u32Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
	        }
	    }
	    else
	    {
        printf("[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        u32Return = FALSE;
	    }

    }
    else
    {
         ULOGD("DEMOD","\r\n ====== DVBT2 doesn't need to Suspend/Resume at Non-DVBT2 mode ====== \r\n");
        u32Return = FALSE;
    }
    return UTOPIA_STATUS_SUCCESS;
}


MS_BOOL DMD_DVBT2_GetPlpBitMap(MS_U8* u8PlpBitMap)
{
    return INTERN_DVBT2_GetPlpBitMap(u8PlpBitMap);
}

MS_BOOL DMD_DVBT2_GetPlpGroupID(MS_U8 u8PlpID, MS_U8* u8GroupID)
{
    return INTERN_DVBT2_GetPlpGroupID(u8PlpID, u8GroupID);
}

MS_BOOL DMD_DVBT2_SetPlpID(MS_U8 u8PlpID, MS_U8 u8GroupID)
{
    MS_BOOL bRet = FALSE;

    if (INTERN_DVBT2_GetLock(E_DMD_DVBT2_FEC_LOCK) == TRUE)
    {
        if (u8PlpID != 0xFF)
        {
            MS_U16 u16Retry = 0;
            MS_U8 u8GroupId = 0;
            MsOS_DelayTask(500);

            bRet = INTERN_DVBT2_GetPlpGroupID(u8PlpID, &u8GroupId);
            while ((bRet == FALSE) && (u16Retry < 60))
            {
                u16Retry++;
                printf("DoSet_DVBT2 get groupid retry %d \n", u16Retry);
                MsOS_DelayTask(100);
                bRet = INTERN_DVBT2_GetPlpGroupID(u8PlpID, &u8GroupId);
            }
            if (bRet == FALSE)
            {
                printf("DoSet_DVBT2() INTERN_DVBT2_GetPlpGroupID(%d) Error \n", u8PlpID);
                return FALSE;
            }

            bRet = INTERN_DVBT2_SetPlpGroupID(u8PlpID, u8GroupId);
            if (bRet == FALSE)
            {
                printf("DoSet_DVBT2() INTERN_DVBT2_SetPlpGroupID(%d,%d) Error", u8PlpID, u8GroupId);
                return FALSE;
            }
        }
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_U32 DVBT2Open(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    DVBT2_INSTANT_PRIVATE *pDvbt2Pri= NULL;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2_v2.c]DVBT2Open\n"));

// void *pDvbtPriVoid = NULL;

    UtopiaInstanceCreate(sizeof(DVBT2_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void*)&pDvbt2Pri);
// pDvbtPri = (DVBT_INSTANT_PRIVATE*)pDvbtPriVoid;


   pDvbt2Pri->fpDVBT2_Init                                  =DMD_DVBT2_Init;
   pDvbt2Pri->fpDVBT2_Exit                                  =DMD_DVBT2_Exit;
   pDvbt2Pri->fpDVBT2_SetDbgLevel                           =DMD_DVBT2_SetDbgLevel;
   pDvbt2Pri->fpDVBT2_GetInfo                               =DMD_DVBT2_GetInfo;
   pDvbt2Pri->fpDVBT2_GetLibVer                             =DMD_DVBT2_GetLibVer;
   pDvbt2Pri->fpDVBT2_GetFWVer                              =DMD_DVBT2_GetFWVer;
   pDvbt2Pri->fpDVBT2_GetReg                                =DMD_DVBT2_GetReg;
   pDvbt2Pri->fpDVBT2_SetReg                                =DMD_DVBT2_SetReg;
   pDvbt2Pri->fpDVBT2_SetSerialControl                      =DMD_DVBT2_SetSerialControl;
   pDvbt2Pri->fpDVBT2_SetReset                              =DMD_DVBT2_SetReset;
   pDvbt2Pri->fpDVBT2_SetConfig                             =DMD_DVBT2_SetConfig;
   pDvbt2Pri->fpDVBT2_SetActive                             =DMD_DVBT2_SetActive;
   pDvbt2Pri->fpDVBT2_GetLock                               =DMD_DVBT2_GetLock;
//   pDvbt2Pri->fpDVBT2_GetSignalStrengthWithRFPower          =DMD_DVBT2_GetSignalStrengthWithRFPower;
//   pDvbt2Pri->fpDVBT2_GetSignalQualityWithRFPower           =DMD_DVBT2_GetSignalQualityWithRFPower;
   pDvbt2Pri->fpDVBT2_GetSNR                                =DMD_DVBT2_GetSNR;
   pDvbt2Pri->fpDVBT2_GetPostLdpcBer                        =DMD_DVBT2_GetPostLdpcBer;
   pDvbt2Pri->fpDVBT2_GetPreLdpcBer                         =DMD_DVBT2_GetPreLdpcBer;
   pDvbt2Pri->fpDVBT2_GetPacketErr                          =DMD_DVBT2_GetPacketErr;
   pDvbt2Pri->fpDVBT2_GetL1Info                             =DMD_DVBT2_GetL1Info;
   pDvbt2Pri->fpDVBT2_GetFreqOffset                         =DMD_DVBT2_GetFreqOffset;
//   pDvbt2Pri->fpDVBT2_NORDIG_SSI_Table_Write                =DMD_DVBT2_NORDIG_SSI_Table_Write;
//   pDvbt2Pri->fpDVBT2_NORDIG_SSI_Table_Read                 =DMD_DVBT2_NORDIG_SSI_Table_Read;
   pDvbt2Pri->fpDVBT2_SetPowerState                         =DMD_DVBT2_SetPowerState;
   pDvbt2Pri->fpDVBT2_GetPlpBitMap                          =DMD_DVBT2_GetPlpBitMap;
   pDvbt2Pri->fpDVBT2_GetPlpGroupID			=DMD_DVBT2_GetPlpGroupID;
   pDvbt2Pri->fpDVBT2_SetPlpID                              =DMD_DVBT2_SetPlpID;

    //return TRUE;
    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 DVBT2Ioctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{

    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);

    //void* pResource = NULL;

    DVBT2_INSTANT_PRIVATE* psDVBT2InstPri = NULL;
    void* psDVBT2InstPriVoid = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psDVBT2InstPriVoid);
    psDVBT2InstPri = (DVBT2_INSTANT_PRIVATE*)psDVBT2InstPriVoid;

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
	case DMD_DVBT2_DRV_CMD_Init:
	DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2_v2.c]DVBT2Ioctl - MDrv_DMD_DVBT2_Init\n"));
		bRet = psDVBT2InstPri->fpDVBT2_Init( (((PDVBT2_INIT_PARAM)pArgs)->pDMD_DVBT2_InitData), ((PDVBT2_INIT_PARAM)pArgs)->u32InitDataLen);
	       ((PDVBT2_INIT_PARAM)pArgs)->ret=bRet;
	break;
       case DMD_DVBT2_DRV_CMD_Exit :
         bRet = psDVBT2InstPri->fpDVBT2_Exit();
         ((PDVBT2_EXIT_PARAM)pArgs)->ret=bRet;
       break;
       case DMD_DVBT2_DRV_CMD_SetDbgLevel:
	       bRet = psDVBT2InstPri->fpDVBT2_SetDbgLevel(((PDVBT2_SETDBGLEVEL_PARAM)pArgs)->u8DbgLevel);
	       ((PDVBT2_SETDBGLEVEL_PARAM)pArgs)->ret=bRet;
       break;
       case DMD_DVBT2_DRV_CMD_GetInfo:
        ((PDVBT2_GETINFO_PARAM)pArgs)->pInfo=psDVBT2InstPri->fpDVBT2_GetInfo(((PDVBT2_GETINFO_PARAM)pArgs)->eInfoType);
        bRet=true;
       break;
	case  DMD_DVBT2_DRV_CMD_GetLibVer:
		bRet=psDVBT2InstPri->fpDVBT2_GetLibVer(((PDVBT2_GETLIBVER_PARAM)pArgs)->ppVersion);
		((PDVBT2_GETLIBVER_PARAM)pArgs)->ret=bRet;
	break;
	case DMD_DVBT2_DRV_CMD_GetFWVer:
		bRet=psDVBT2InstPri->fpDVBT2_GetFWVer(((PDVBT2_GETFWVER_PARAM)pArgs)->ver);
		((PDVBT2_GETFWVER_PARAM)pArgs)->ret=bRet;
	break;
       case DMD_DVBT2_DRV_CMD_GetReg:
        bRet=psDVBT2InstPri ->fpDVBT2_GetReg(((PDVBT2_GETREG_PARAM)pArgs)->u16Addr,((PDVBT2_GETREG_PARAM)pArgs)->pu8Data);
        ((PDVBT2_GETREG_PARAM)pArgs)->ret=bRet;
       break;
       case DMD_DVBT2_DRV_CMD_SetReg:
        bRet=psDVBT2InstPri->fpDVBT2_SetReg(((PDVBT2_SETREG_PARAM)pArgs)->u16Addr,((PDVBT2_SETREG_PARAM)pArgs)->u8Data);
        ((PDVBT2_SETREG_PARAM)pArgs)->ret=bRet;
       break;
       case DMD_DVBT2_DRV_CMD_SetSerialControl:
        bRet=psDVBT2InstPri->fpDVBT2_SetSerialControl(((PDVBT2_SETSERIALCONTROL_PARAM)pArgs)->bEnable);
        ((PDVBT2_SETSERIALCONTROL_PARAM)pArgs)->ret=bRet;
       break;
       case DMD_DVBT2_DRV_CMD_SetReset:
        bRet=psDVBT2InstPri->fpDVBT2_SetReset();
       break;
       case DMD_DVBT2_DRV_CMD_SetConfig:
        bRet=psDVBT2InstPri->fpDVBT2_SetConfig(((PDVBT2_SETCONFIG_PARAM)pArgs)->BW,((PDVBT2_SETCONFIG_PARAM)pArgs)->bSerialTS,((PDVBT2_SETCONFIG_PARAM)pArgs)->u8PlpID);
        ((PDVBT2_SETCONFIG_PARAM)pArgs)->ret=bRet;
       break;
       case DMD_DVBT2_DRV_CMD_SetActive:
        bRet=psDVBT2InstPri->fpDVBT2_SetActive(((PDVBT2_SETACTIVE_PARAM)pArgs)->bEnable);
        ((PDVBT2_SETACTIVE_PARAM)pArgs)->ret=bRet;
       break;
       case DMD_DVBT2_DRV_CMD_GetLock:
        bRet=psDVBT2InstPri->fpDVBT2_GetLock(((PDVBT2_GETLOCK_PARAM)pArgs)->eType,((PDVBT2_GETLOCK_PARAM)pArgs)->eLockStatus);
        ((PDVBT2_GETLOCK_PARAM)pArgs)->ret=bRet;
       break;
       case DMD_DVBT2_DRV_CMD_GetSNR:
        bRet=psDVBT2InstPri->fpDVBT2_GetSNR(((PDVBT2_GETSNR_PARAM)pArgs)->u16_snr100, ((PDVBT2_GETSNR_PARAM)pArgs)->snr_cali, ((PDVBT2_GETSNR_PARAM)pArgs)->u8_gi);
        ((PDVBT2_GETSNR_PARAM)pArgs)->ret=bRet;
       break;
       case DMD_DVBT2_DRV_CMD_GetPostLdpcBer:
        bRet=psDVBT2InstPri->fpDVBT2_GetPostLdpcBer(((PDVBT2_GETPOSTLDPCBER_PARAM)pArgs)->BitErr_reg, ((PDVBT2_GETPOSTLDPCBER_PARAM)pArgs)->BitErrPeriod_reg, ((PDVBT2_GETPOSTLDPCBER_PARAM)pArgs)->FecType);
        ((PDVBT2_GETPOSTLDPCBER_PARAM)pArgs)->ret=bRet;
       break;
       case DMD_DVBT2_DRV_CMD_GetPreLdpcBer:
        bRet=psDVBT2InstPri->fpDVBT2_GetPreLdpcBer(((PDVBT2_GETPRELDPCBERPARAM)pArgs)->BitErr_reg, ((PDVBT2_GETPRELDPCBERPARAM)pArgs)->BitErrPeriod_reg, ((PDVBT2_GETPRELDPCBERPARAM)pArgs)->FecType);
        ((PDVBT2_GETPRELDPCBERPARAM)pArgs)->ret=bRet;
       break;
       case DMD_DVBT2_DRV_CMD_GetPacketErr:
        bRet=psDVBT2InstPri->fpDVBT2_GetPacketErr(((PDVBT2_GETPACKETERRPARAM)pArgs)->pktErr);
        ((PDVBT2_GETPACKETERRPARAM)pArgs)->ret=bRet;
       break;
       case DMD_DVBT2_DRV_CMD_GetL1Info:
        bRet=psDVBT2InstPri->fpDVBT2_GetL1Info(((PDVBT2_GETL1INFO_PARAM)pArgs)->u16Info,((PDVBT2_GETL1INFO_PARAM)pArgs)->eSignalType);
        ((PDVBT2_GETL1INFO_PARAM)pArgs)->ret=bRet;
       break;
       case DMD_DVBT2_DRV_CMD_GetFreqOffset:
        bRet=psDVBT2InstPri->fpDVBT2_GetFreqOffset(((PDVBT2_GETFREQOFFSET_PARAM)pArgs)->CfoTd_reg, ((PDVBT2_GETFREQOFFSET_PARAM)pArgs)->CfoFd_reg, ((PDVBT2_GETFREQOFFSET_PARAM)pArgs)->Icfo_reg, ((PDVBT2_GETFREQOFFSET_PARAM)pArgs)->fft_reg);
        ((PDVBT2_GETFREQOFFSET_PARAM)pArgs)->ret=bRet;
       break;
       case DMD_DVBT2_DRV_CMD_SetPowerState:
        ((PDVBT2_SETPOWERSTATE_PARAM)pArgs)->ret=psDVBT2InstPri->fpDVBT2_SetPowerState(((PDVBT2_SETPOWERSTATE_PARAM)pArgs)->u16PowerState);
        bRet=TRUE;
       break;
       case DMD_DVBT2_DRV_CMD_GetPlpBitMap:
        bRet=psDVBT2InstPri->fpDVBT2_GetPlpBitMap(((PDVBT2_GETPLPBITMAP_PARAM)pArgs)->u8PlpBitMap);
       ((PDVBT2_GETPLPBITMAP_PARAM)pArgs)->ret=bRet;
       break;
       case DMD_DVBT2_DRV_CMD_GetPlpGroupID:
        bRet=psDVBT2InstPri->fpDVBT2_GetPlpGroupID(((PDVBT2_GETPLPGROUPID_PARAM)pArgs)->u8PlpID,((PDVBT2_GETPLPGROUPID_PARAM)pArgs)->u8GroupID);
        ((PDVBT2_GETPLPGROUPID_PARAM)pArgs)->ret=bRet;
       break;
       case DMD_DVBT2_DRV_CMD_SetPlpID:
        bRet=psDVBT2InstPri->fpDVBT2_SetPlpID(((PDMD_DVBT2_SETPLPID_PARAM)pArgs)->u8PlpID,((PDMD_DVBT2_SETPLPID_PARAM)pArgs)->u8GroupID);
        ((PDMD_DVBT2_SETPLPID_PARAM)pArgs)->ret=bRet;
       break;
       default:
       break;
    }

    //jway suggest UtopiaResourceRelease(pResource);

    return (bRet ? UTOPIA_STATUS_SUCCESS : UTOPIA_STATUS_FAIL);
}

MS_U32 DVBT2Close(void* pInstance)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2_v2.c]DVBT2Close\n"));

    UtopiaInstanceDelete(pInstance);

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 DVBT2Str(MS_U32 u32PowerState, void* pModule)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    MS_U32 u32Ret = 0;

    //UtopiaModuleGetSTRPrivate(pModule, (void**));
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2_v2.c]DVBT2Str\n"));

printf("[YPDEBUG][drvDMD_INTERN_DVBT2_v2.c] DVBT2Str\n");

    if (u32PowerState == E_POWER_SUSPEND)
    {
        /* Please Implement Module Suspend Flow Here. */
        u32Ret = DMD_DVBT2_SetPowerState(E_POWER_SUSPEND);

        if(u32Ret == TRUE)
          u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
        else
          u32Return = UTOPIA_STATUS_FAIL;

        DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2_v2.c][DVBT2Str] SUSPEND !\n"));
    }
    else if (u32PowerState == E_POWER_RESUME)
    {
        /* Please Implement Module Resume Flow Here. */
        u32Ret = DMD_DVBT2_SetPowerState(E_POWER_RESUME);

        if(u32Ret == TRUE)
          u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
        else
          u32Return = UTOPIA_STATUS_FAIL;

        DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2_v2.c][DVBT2Str] RESUME !\n"));
    }
    else
    {
        u32Return = UTOPIA_STATUS_FAIL;
        DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2_v2.c][DVBT2Str] OTHERS !\n"));
    }

    return u32Return;// for success
}

void DVBT2RegisterToUtopia(void)
{
    // 1. deal with module

    void* pUtopiaModule = NULL;
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2_v2.c]DVBT2RegisterToUtopia\n"));
//    UtopiaModuleCreate(MODULE_DVBT, 8, &pUtopiaModule);   //bryan: why this was taken out
    UtopiaModuleCreate(MODULE_DVBT2, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)DVBT2Open, (FUtopiaClose)DVBT2Close, (FUtopiaIOctl)DVBT2Ioctl);

printf("[YPDEBUG][drvDMD_INTERN_DVBT2_v2.c] DVBT2RegisterToUtopia\n");
   // Utopia2K STR
#if defined(MSOS_TYPE_LINUX_KERNEL)
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2_v2.c][DVBT2RegisterToUtopia] KERNEL DVBT2Str!\n"));
    UtopiaModuleSetupSTRFunctionPtr(pUtopiaModule,(FUtopiaSTR)DVBT2Str);
#endif
}