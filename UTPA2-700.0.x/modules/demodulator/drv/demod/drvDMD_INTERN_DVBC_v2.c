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
#include "drvDMD_INTERN_DVBC_v2.h"
#include "halDMD_INTERN_DVBC.h"
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

// share memory setting
typedef enum _DVBC_POOL_ID
{
    DVBC_POOL_ID_DMD0 = 0
} DVBC_POOL_ID;

typedef struct DLL_PACKED _DVBC_RESOURCE_PRIVATE
{
    DMD_DVBC_ResData sDMD_DVBC_ResData[DMD_DVBC_MAX_DEMOD_NUM];
} DVBC_RESOURCE_PRIVATE, *PDVBC_RESOURCE_PRIVATE;
// end share memory setting

DMD_DVBC_ResData *psDMD_DVBC_ResData;	// pointer to share memory

static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#define DMD_LOCK()      \
    do{                         \
        MS_ASSERT(MsOS_In_Interrupt() == FALSE); \
        if (_s32DMD_DVBC_Mutex == -1) return FALSE; \
        if (_u8DMD_DVBC_DbgLevel == DMD_DVBC_DBGLV_DEBUG) ULOGD("DEMOD","%s lock mutex\n", __FUNCTION__);\
        MsOS_ObtainMutex(_s32DMD_DVBC_Mutex, MSOS_WAIT_FOREVER);\
        }while(0)

#define DMD_UNLOCK()      \
    do{                         \
        MsOS_ReleaseMutex(_s32DMD_DVBC_Mutex);\
        if (_u8DMD_DVBC_DbgLevel == DMD_DVBC_DBGLV_DEBUG) ULOGD("DEMOD","%s unlock mutex\n", __FUNCTION__); \
        }while(0)
#if 0
MS_U8		 DVBT_TS_PHASE_EN =0;
MS_U8		 DVBT_TS_PHASE_NUM = 0; 
#endif

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


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
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

typedef MS_BOOL                   (*IOCTL_DVBC_Dual_Individual_Init)(DMD_DVBC_InitData_Transform*pDMD_DVBC_InitData, MS_U32 u32InitDataLen);
typedef MS_BOOL                   (*IOCTL_DVBC_Dual_Public_Init)(MS_U8 u8AGC_Tristate_Ctrl,MS_U8 u8Sar_Channel);
typedef MS_BOOL                   (*IOCTL_DVBC_Init)(DMD_DVBC_InitData_Transform *pDMD_DVBC_InitData, MS_U32 u32InitDataLen);
typedef MS_BOOL			    (*IOCTL_DVBC_Exit)(void);
typedef MS_BOOL                   (*IOCTL_DVBC_SetDbgLevel)(DMD_DVBC_DbgLv u8DbgLevel);
typedef const DMD_DVBC_Info* (*IOCTL_DVBC_GetInfo)(void);
typedef MS_BOOL 		    (*IOCTL_DVBC_GetLibVer)(const MSIF_Version **ppVersion);
typedef MS_BOOL			     (*IOCTL_DVBC_GetFWVer)(MS_U16 *ver);
typedef MS_BOOL                   (*IOCTL_DVBC_GetDSPReg)(MS_U16 u16Addr, MS_U8 *pu8Data);
typedef MS_BOOL			     (*IOCTL_DVBC_SetDSPReg)(MS_U16 u16Addr, MS_U8 pu8Data);
typedef MS_BOOL			     (*IOCTL_DVBC_GetReg)(MS_U16 u16Addr, MS_U8 *pu8Data);
typedef MS_BOOL			     (*IOCTL_DVBC_SetReg)(MS_U16 u16Addr, MS_U8 u8Data);
typedef MS_BOOL			     (*IOCTL_DVBC_SetSerialControl)(MS_BOOL bEnable);
typedef MS_BOOL                   (*IOCTL_DVBC_SetConfig_symbol_rate_list)(MS_U16 u16SymbolRate, DMD_DVBC_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num);
typedef MS_BOOL			     (*IOCTL_DVBC_SetActive)(MS_BOOL bEnable);
typedef MS_BOOL	          (*IOCTL_DVBC_GetLockWithRFPower)(DMD_DVBC_GETLOCK_TYPE eType, MS_U32 u32CurrRFPowerDbm, MS_U32 u32NoChannelRFPowerDbm, DMD_DVBC_LOCK_STATUS *eLockStatus);
//typedef MS_BOOL           (*IOCTL_DVBC_GetSignalStrengthWithRFPower)(MS_U16 *u16Strength);
//typedef MS_BOOL			      (*IOCTL_DVBC_GetSignalQualityWithRFPower)(MS_U16 *u16Quality);

typedef MS_BOOL			      (*IOCTL_DVBC_ActiveDmdSwitch)(MS_U8 demod_no);

typedef MS_BOOL                    (*IOCTL_DVBC_GetSNR)(MS_U16 *snr_reg);
//waiting add
typedef MS_BOOL                    (*IOCTL_DVBC_GetPostViterbiBer)(MS_U32 *BitErr_reg, MS_U16 *BitErrPeriod_reg);
typedef MS_BOOL                    (*IOCTL_DVBC_GetIFAGC)(MS_U8 *ifagc_reg, MS_U8 *ifagc_reg_lsb, MS_U16 *ifagc_err);

typedef MS_BOOL                    (*IOCTL_DVBC_GetPacketErr)(MS_U16 *pktErr);
typedef MS_BOOL                    (*IOCTL_DVBC_GetCellID)(MS_U16 *u16CellID);

typedef MS_BOOL                    (*IOCTL_DVBC_GetStatus)(DMD_DVBC_MODULATION_TYPE *pQAMMode, MS_U16 *u16SymbolRate, MS_U32 *config_Fc_reg, MS_U32 *Fc_over_Fs_reg, MS_U16 *Cfo_offset_reg);
//typedef MS_BOOL                     (*IOCTL_DVBC_GetStatus)(DMD_DVBC_MODULATION_TYPE *pQAMMode, MS_U16 *u16SymbolRate);
typedef MS_U32                       (*IOCTL_DVBC_SetPowerState)(EN_POWER_MODE u16PowerState);

#ifdef UFO_DEMOD_DVBC_GET_AGC_INFO
typedef MS_BOOL 										(*IOCTL_DVBC_GetAGCInfo)(MS_U8 u8dbg_mode, MS_U16* pu16Data);
#endif



typedef struct DVBC_INSTANT_PRIVATE
{
	IOCTL_DVBC_Dual_Individual_Init 	fpDVBC_Dual_Individual_Init;
	IOCTL_DVBC_Dual_Public_Init 		fpDVBC_Dual_Public_Init;
	IOCTL_DVBC_Init					fpDVBC_Init;
	IOCTL_DVBC_Exit					fpDVBC_Exit;
	IOCTL_DVBC_SetDbgLevel			fpDVBC_SetDbgLevel;
	IOCTL_DVBC_GetInfo				fpDVBC_GetInfo;
	IOCTL_DVBC_GetLibVer			fpDVBC_GetLibVer;
	IOCTL_DVBC_GetFWVer			fpDVBC_GetFWVer;
	IOCTL_DVBC_GetDSPReg			fpDVBC_GetDSPReg;
	IOCTL_DVBC_SetDSPReg			fpDVBC_SetDSPReg;
	IOCTL_DVBC_GetReg				fpDVBC_GetReg;
	IOCTL_DVBC_SetReg				fpDVBC_SetReg;
	IOCTL_DVBC_SetSerialControl           fpDVBCSetSerialControl;
	IOCTL_DVBC_SetConfig_symbol_rate_list fpDVBC_SetConfig_symbol_rate_list;
	IOCTL_DVBC_SetActive				fpDVBC_SetActive;
	IOCTL_DVBC_GetLockWithRFPower	fpDVBC_GetLockWithRFPower;
	//IOCTL_DVBC_GetSignalStrengthWithRFPower  fpDVBC_GetSignalStrengthWithRFPower;
	//IOCTL_DVBC_GetSignalQualityWithRFPower    fpDVBC_GetSignalQualityWithRFPower;
	IOCTL_DVBC_ActiveDmdSwitch                      fpDVBC_ActiveDmdSwitch;
	IOCTL_DVBC_GetSNR					     fpDVBC_GetSNR;
	IOCTL_DVBC_GetPostViterbiBer			     fpDVBC_GetPostViterbiBer;
	IOCTL_DVBC_GetPacketErr				     fpDVBC_GetPacketErr;
	IOCTL_DVBC_GetCellID				     fpDVBC_GetCellID;
	IOCTL_DVBC_GetStatus				     fpDVBC_GetStatus;
	IOCTL_DVBC_SetPowerState			     fpDVBC_SetPowerState;	
	
	//waiting add
	IOCTL_DVBC_GetIFAGC    fpDVBC_GetIFAGC;
	
	#ifdef UFO_DEMOD_DVBC_GET_AGC_INFO
	IOCTL_DVBC_GetAGCInfo fpDVBC_GetAGCInfo;
	#endif
	
	
} DVBC_INSTANT_PRIVATE;


MS_U8		 DVBC_TS_PHASE_EN =0;
MS_U8		 DVBC_TS_PHASE_NUM = 0; 
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if 1
static MSIF_Version _drv_dmd_dvbc_intern_version = {
    .MW = { DMD_DVBC_INTERN_VER, },
};
#else
static MSIF_Version _drv_dmd_dvbt_intern_version;
#endif

static const char pDMD_DVBC_MutexString[]={"Mutex DMD DVBC"};
//bryan temp mark static DMD_DVBC_InitData _sDMD_DVBC_InitData;
static DMD_DVBC_InitData_Transform _sDMD_DVBC_InitData;
static DMD_DVBC_DbgLv _u8DMD_DVBC_DbgLevel=DMD_DVBC_DBGLV_NONE;
static MS_S32 _s32DMD_DVBC_Mutex=-1;
static DMD_DVBC_Info sDMD_DVBC_Info;
static MS_U16 u16DMD_DVBC_AutoSymbol_Timeout = 10000, u16DMD_DVBC_FixSymbol_AutoQam_Timeout=2000, u16DMD_DVBC_FixSymbol_FixQam_Timeout=1000;
static MS_U32 u32DMD_DVBC_PrevScanTime=0, u32DMD_DVBC_ScanCount=0;

#if defined(CHIP_KAISER)||defined(CHIP_K6LITE)
//for dual demod setting

//demod NO0
static DMD_DVBC_InitData_Transform _sDMD_DVBC_InitData_dmd0;
//static DMD_DVBC_DbgLv _u8DMD_DVBC_DbgLevel=DMD_DVBC_DBGLV_NONE;
//static MS_S32 _s32DMD_DVBC_Mutex=-1;
static DMD_DVBC_Info sDMD_DVBC_Info_dmd0;
static MS_U16 u16DMD_DVBC_AutoSymbol_Timeout_dmd0 = 10000, u16DMD_DVBC_FixSymbol_AutoQam_Timeout_dmd0=2000, u16DMD_DVBC_FixSymbol_FixQam_Timeout_dmd0=1000;
static MS_U32 u32DMD_DVBC_PrevScanTime_dmd0=0, u32DMD_DVBC_ScanCount_dmd0=0;

//demod NO1
static DMD_DVBC_InitData_Transform _sDMD_DVBC_InitData_dmd1;
//static DMD_DVBC_DbgLv _u8DMD_DVBC_DbgLevel=DMD_DVBC_DBGLV_NONE;
//static MS_S32 _s32DMD_DVBC_Mutex=-1;
static DMD_DVBC_Info sDMD_DVBC_Info_dmd1;
static MS_U16 u16DMD_DVBC_AutoSymbol_Timeout_dmd1 = 10000, u16DMD_DVBC_FixSymbol_AutoQam_Timeout_dmd1=2000, u16DMD_DVBC_FixSymbol_FixQam_Timeout_dmd1=1000;
static MS_U32 u32DMD_DVBC_PrevScanTime_dmd1=0, u32DMD_DVBC_ScanCount_dmd1=0;

static MS_U8 drv_demod_swtich_status=0xff;  //
#endif
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define DMD_DBG(x)          (x)
#else
#define DMD_DBG(x)          //(x)
#endif

#if defined(CHIP_KAISER)||defined(CHIP_K6LITE)
MS_BOOL DMD_DVBC_Dual_Public_Init(MS_U8 u8AGC_Tristate_Ctrl,MS_U8 u8Sar_Channel)
{
    char pDMD_DVBC_MutexString[16];
   // MS_U32 u32IfFrequency = 36167L, u32FsFrequency = 45474L;
   // MS_U8 u8IQSwap=0, u8ADCIQMode = 0, u8PadSel = 0, bPGAEnable = 0, u8PGAGain = 5;
    MS_BOOL bRFAGCTristateEnable = 1;
    MS_BOOL bIFAGCTristateEnable = 0;

    if (_s32DMD_DVBC_Mutex != -1)
    {
        DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBC_Init more than once\n"));
        return FALSE;
    }
    
    if (NULL == strncpy(pDMD_DVBC_MutexString,"Mutex DMD DVBC",16))
    {
        DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBC_Init strcpy Fail\n"));
        return FALSE;    
    }
    _s32DMD_DVBC_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, pDMD_DVBC_MutexString, MSOS_PROCESS_SHARED);
    if ( _s32DMD_DVBC_Mutex == -1)
    {
        DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBC_Init Create Mutex Fail\n"));
        return FALSE;
    }

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBC_Init\n");
    }
    #endif

    if (u8Sar_Channel != 0xFF)
    {
    	//bryan temp mark 
      //  MDrv_SAR_Adc_Config(u8Sar_Channel, TRUE);
    }
    DMD_LOCK();
    //_u8DMD_DVBC_DbgLevel=DMD_DVBC_DBGLV_DEBUG;
    MDrv_SYS_DMD_VD_MBX_SetType(E_DMD_VD_MBX_TYPE_DVBC);
    HAL_DMD_RegInit();


     bRFAGCTristateEnable = (u8AGC_Tristate_Ctrl& (BIT_(0))) ? TRUE : FALSE; // RFAGC tristate control
     bIFAGCTristateEnable = (u8AGC_Tristate_Ctrl & (BIT_(4))) ? TRUE : FALSE; // IFAGC tristate control

    
    if (bIFAGCTristateEnable)
    {
        MDrv_SYS_SetAGCPadMux(E_SYS_DTV_AGC_PAD_SET_ALL_OFF);
    }
    else
    {
        MDrv_SYS_SetAGCPadMux(E_SYS_DTV_AGC_PAD_SET);
    }

    INTERN_DVBC_InitClkgen(bRFAGCTristateEnable);  //RF_AGC control no use in initclkgen
    DMD_UNLOCK();

    return TRUE;
}


MS_BOOL DMD_DVBC_Dual_Individual_Init(DMD_DVBC_InitData_Transform*pDMD_DVBC_InitData, MS_U32 u32InitDataLen)
{  
     DMD_LOCK();
     
    if ( sizeof(_sDMD_DVBC_InitData) == u32InitDataLen)
    {
        memcpy(&_sDMD_DVBC_InitData, pDMD_DVBC_InitData, u32InitDataLen);
    }
    else
    {
        DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBC_Init input data structure incorrect\n"));
        return FALSE;
    }

    u16DMD_DVBC_AutoSymbol_Timeout = 10000;
    u16DMD_DVBC_FixSymbol_AutoQam_Timeout=2000;
    u16DMD_DVBC_FixSymbol_FixQam_Timeout=2000;
    if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt != NULL)
    {
        if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[0]>=4) 
        {
            u16DMD_DVBC_AutoSymbol_Timeout = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[3]; 
            u16DMD_DVBC_AutoSymbol_Timeout =  (u16DMD_DVBC_AutoSymbol_Timeout<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[4];           
            if (u16DMD_DVBC_AutoSymbol_Timeout < 5000) u16DMD_DVBC_AutoSymbol_Timeout=5000;
            //ULOGD("DEMOD","u16DMD_DVBC_AutoSymbol_Timeout %d\n",u16DMD_DVBC_AutoSymbol_Timeout);
            
            u16DMD_DVBC_FixSymbol_AutoQam_Timeout = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[5];
            u16DMD_DVBC_FixSymbol_AutoQam_Timeout =  (u16DMD_DVBC_FixSymbol_AutoQam_Timeout<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[6];           
            if (u16DMD_DVBC_FixSymbol_AutoQam_Timeout < 2000) u16DMD_DVBC_FixSymbol_AutoQam_Timeout=2000;
            //ULOGD("DEMOD","u16DMD_DVBC_FixSymbol_AutoQam_Timeout %d\n",u16DMD_DVBC_FixSymbol_AutoQam_Timeout);            

            u16DMD_DVBC_FixSymbol_FixQam_Timeout = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[7];
            u16DMD_DVBC_FixSymbol_FixQam_Timeout =  (u16DMD_DVBC_FixSymbol_FixQam_Timeout<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[8];           
            if (u16DMD_DVBC_FixSymbol_FixQam_Timeout < 1000) u16DMD_DVBC_FixSymbol_FixQam_Timeout=1000;
            //ULOGD("DEMOD","u16DMD_DVBC_FixSymbol_FixQam_Timeout %d\n",u16DMD_DVBC_FixSymbol_FixQam_Timeout);            
        }
        else
        {
        }
    }
    else
    {
    }


    
    if (_sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitExt != NULL)
    {
        if (_sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitExt[0]>=1)
        {
            INTERN_DVBC_DMD51_Individual_Initialization(_sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitExt, _sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitSize);
        }
        else
        {
            ULOGD("DEMOD","u8DMD_DVBT_DSPRegInitExt Error\n");
        }
    }
    else
    {
        INTERN_DVBC_DMD51_Individual_Initialization(  NULL, 0);
    }

    INTERN_DVBC_Version(&sDMD_DVBC_Info.u16Version);
    DMD_UNLOCK();
    #ifdef MS_DEBUG
    ULOGD("DEMOD","firmware version: %x\n",sDMD_DVBC_Info.u16Version);
    #endif
    return TRUE;
}
#else
	MS_BOOL DMD_DVBC_Dual_Individual_Init(DMD_DVBC_InitData_Transform *pDMD_DVBC_InitData, MS_U32 u32InitDataLen)
	{
				ULOGD("DEMOD","Doesn't support DVBC_Dual_Individual_Init function!!!\n");
				return false;
	}

	MS_BOOL DMD_DVBC_Dual_Public_Init(MS_U8 u8AGC_Tristate_Ctrl,MS_U8 u8Sar_Channel)
	{
				ULOGD("DEMOD","Doesn't support DVBC_Dual_Public_Init function!!!\n");
				return false;
	}
#endif
MS_BOOL DMD_DVBC_Init(DMD_DVBC_InitData_Transform *pDMD_DVBC_InitData, MS_U32 u32InitDataLen)
{
    char pDMD_DVBC_MutexString[16];
//    MS_U32 u32IfFrequency = 36167L, u32FsFrequency = 45474L;
    MS_U32 u32IfFrequency =5000L, u32FsFrequency = 48000L;
    MS_U8 u8IQSwap = 0, u8ADCIQMode = 0, u8PadSel = 0, bPGAEnable = 0, u8PGAGain = 5;
    MS_BOOL bRFAGCTristateEnable = 1;
    MS_BOOL bIFAGCTristateEnable = 0;
	DMD_DVBC_ResData *pRes = psDMD_DVBC_ResData;


    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DMD_DVBC_Init\n"));
   
    if (_s32DMD_DVBC_Mutex != -1)
    {
        DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBC_Init more than once\n"));
        return FALSE;
    }

    if (NULL == strncpy(pDMD_DVBC_MutexString,"Mutex DMD DVBC",16))
    {
        DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBC_Init strcpy Fail\n"));
        return FALSE;
    }
    _s32DMD_DVBC_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, pDMD_DVBC_MutexString, MSOS_PROCESS_SHARED);
    if ( _s32DMD_DVBC_Mutex == -1)
    {
        DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBC_Init Create Mutex Fail\n"));
        return FALSE;
    }

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBC_Init\n");
    }
    #endif



	DMD_DBG(printf("[DEMOD][DMD_DVBC_INIT] _prev_u16PowerState is %d \n", _prev_u16PowerState));

	if(E_POWER_MECHANICAL==_prev_u16PowerState)
	{
		
		DMD_DBG(printf("memcpy(&(pRes->sDMD_DVBC_InitData_Transform) 1111 enter \n"));
		memcpy(&(pRes->sDMD_DVBC_InitData_Transform), pDMD_DVBC_InitData, sizeof(DMD_DVBC_InitData_Transform));
		DMD_DBG(printf("memcpy(&(pRes->sDMD_DVBC_InitData_Transform) 2222 exit \n"));

		DMD_DBG(printf("memcpy(&_sDMD_DVBC_InitData, pDMD_DVBC_InitData, u32InitDataLen) 3333 enter \n"));
		memcpy(&_sDMD_DVBC_InitData, pDMD_DVBC_InitData, sizeof(_sDMD_DVBC_InitData));
		DMD_DBG(printf("memcpy(&_sDMD_DVBC_InitData, pDMD_DVBC_InitData, u32InitDataLen) 4444 exit \n"));

	}
	else if(E_POWER_RESUME==_prev_u16PowerState)
	{
		DMD_DBG(printf("memcpy E_POWER_RESUME Enter  \n"));
		memcpy(&_sDMD_DVBC_InitData, &(pRes->sDMD_DVBC_InitData_Transform), sizeof(DMD_DVBC_InitData_Transform));
		DMD_DBG(printf("memcpy E_POWER_RESUME Exit  \n"));
	}
	else
	{
		ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c][DMD_DVBC_Init][check !!!]\n");
	}


	pRes->sDMD_DVBC_SHARE_MEMORY_InitData.b_IsInit_DVBCCreat = 1;
	ULOGD("DEMOD","[DMD_DVBC_Init] b_IsInit_DVBCCreat = %d;\n", pRes->sDMD_DVBC_SHARE_MEMORY_InitData.b_IsInit_DVBCCreat);
	


    if (_sDMD_DVBC_InitData.u8SarChannel != 0xFF)
    {
    //bryan temp mark
     //   MDrv_SAR_Adc_Config(_sDMD_DVBC_InitData.u8SarChannel, TRUE);
    }
    DMD_LOCK();
    //_u8DMD_DVBC_DbgLevel=DMD_DVBC_DBGLV_DEBUG;
    MDrv_SYS_DMD_VD_MBX_SetType(E_DMD_VD_MBX_TYPE_DVBC);
    HAL_DMD_RegInit();

    if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt != NULL)
    {
        if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[0]>=2)
        {
            bRFAGCTristateEnable = (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[3] & (BIT_(0))) ? TRUE : FALSE; // RFAGC tristate control
            bIFAGCTristateEnable = (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[3] & (BIT_(4))) ? TRUE : FALSE; // IFAGC tristate control
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

    if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt != NULL)
    {
        if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[0]>=3)
        {
            u32IfFrequency = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[4]; // IF frequency
            u32IfFrequency =  (u32IfFrequency<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[5]; // IF frequency
            u32IfFrequency =  (u32IfFrequency<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[6]; // IF frequency
            u32IfFrequency =  (u32IfFrequency<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[7]; // IF frequency
            u32FsFrequency = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[8]; // FS frequency
            u32FsFrequency =  (u32FsFrequency<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[9]; // FS frequency
            u32FsFrequency =  (u32FsFrequency<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[10]; // FS frequency
            u32FsFrequency =  (u32FsFrequency<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[11]; // FS frequency
            u8IQSwap = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[12]; // IQ Swap
			u8IQSwap = u8IQSwap;
            u8ADCIQMode = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[13]; // u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
            u8PadSel = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[14]; // u8PadSel : 0=Normal, 1=analog pad
            bPGAEnable = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[15]; // bPGAEnable : 0=disable, 1=enable
            u8PGAGain = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[16]; // u8PGAGain : default 5
        }
        else
        {

        }
    }
    else
    {

    }

    u16DMD_DVBC_AutoSymbol_Timeout = 10000;
    u16DMD_DVBC_FixSymbol_AutoQam_Timeout=2000;
    u16DMD_DVBC_FixSymbol_FixQam_Timeout=2000;
    if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt != NULL)
    {
        if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[0]>=4)
        {
            u16DMD_DVBC_AutoSymbol_Timeout = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[17];
            u16DMD_DVBC_AutoSymbol_Timeout =  (u16DMD_DVBC_AutoSymbol_Timeout<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[18];
            if (u16DMD_DVBC_AutoSymbol_Timeout < 5000) u16DMD_DVBC_AutoSymbol_Timeout=5000;
            //ULOGD("DEMOD","u16DMD_DVBC_AutoSymbol_Timeout %d\n",u16DMD_DVBC_AutoSymbol_Timeout);

            u16DMD_DVBC_FixSymbol_AutoQam_Timeout = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[19];
            u16DMD_DVBC_FixSymbol_AutoQam_Timeout =  (u16DMD_DVBC_FixSymbol_AutoQam_Timeout<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[20];
            if (u16DMD_DVBC_FixSymbol_AutoQam_Timeout < 2000) u16DMD_DVBC_FixSymbol_AutoQam_Timeout=2000;
            //ULOGD("DEMOD","u16DMD_DVBC_FixSymbol_AutoQam_Timeout %d\n",u16DMD_DVBC_FixSymbol_AutoQam_Timeout);

            u16DMD_DVBC_FixSymbol_FixQam_Timeout = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[21];
            u16DMD_DVBC_FixSymbol_FixQam_Timeout =  (u16DMD_DVBC_FixSymbol_FixQam_Timeout<<8)+_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[22];
            if (u16DMD_DVBC_FixSymbol_FixQam_Timeout < 1000) u16DMD_DVBC_FixSymbol_FixQam_Timeout=1000;
            //ULOGD("DEMOD","u16DMD_DVBC_FixSymbol_FixQam_Timeout %d\n",u16DMD_DVBC_FixSymbol_FixQam_Timeout);
        }
        else
        {
        }
    }
    else
    {
    }

    #ifdef MS_DEBUG
    ULOGD("DEMOD","u32IfFrequency %ld\n",u32IfFrequency);
    ULOGD("DEMOD","u32FsFrequency %ld\n",u32FsFrequency);
    ULOGD("DEMOD","u8IQSwap %d\n",u8IQSwap);
    #endif

    if (bIFAGCTristateEnable)
    {
        MDrv_SYS_SetAGCPadMux(E_SYS_DTV_AGC_PAD_SET_ALL_OFF);
    }
    else
    {
        MDrv_SYS_SetAGCPadMux(E_SYS_DTV_AGC_PAD_SET);
    }

	// oga
	 DVBC_TS_PHASE_EN = 0;
	 DVBC_TS_PHASE_NUM = 0; 	
   if (0)//(_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt != NULL)
   {
   	if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[0]>=5)	// version bigger than 5, apply TS phase solution
   	{
		 DVBC_TS_PHASE_EN = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[INDEX_C_TS_PHASE_EN];
		 DVBC_TS_PHASE_NUM = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[INDEX_C_TS_PHASE_NUM];   
		 ULOGD("DEMOD","##DVBC: TS check: bTsPhaseEn = %d, u16TsPhaseNum = %d\n",DVBC_TS_PHASE_EN,DVBC_TS_PHASE_NUM);
   	}
	else
	{
		ULOGD("DEMOD","##DVBC:TS Phase check !!, board version smaller than 4\n");
	}
   }
   else	// if init board define is NULL TS phase needs check.
   {
   	ULOGD("DEMOD","##DVBC: TS Phase check !!\n");
   }
	
    if(0)// (_sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitExt != NULL)
    {
        if (_sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitExt[0]>=1)
        {
            INTERN_DVBC_Power_On_Initialization(bRFAGCTristateEnable, u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain, _sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitExt, _sDMD_DVBC_InitData.u8DMD_DVBC_DSPRegInitSize);
        }
        else
        {
            ULOGD("DEMOD","u8DMD_DVBT_DSPRegInitExt Error\n");
        }
    }
    else
    {
	ULOGD("DEMOD","[DMD_DVBC_Init]INTERN_DVBC_Power_On_Initialization Enter \n");
	INTERN_DVBC_Power_On_Initialization(bRFAGCTristateEnable, u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain,  NULL, 0);
	ULOGD("DEMOD","[DMD_DVBC_Init]INTERN_DVBC_Power_On_Initialization exit \n");		
    }
    INTERN_DVBC_Version(&sDMD_DVBC_Info.u16Version);
	pRes->sDMD_DVBC_SHARE_MEMORY_PreviousData.u16Version = sDMD_DVBC_Info.u16Version;
	
    DMD_UNLOCK();
    #ifdef MS_DEBUG
    ULOGD("DEMOD","firmware version: %x\n",sDMD_DVBC_Info.u16Version);
    #endif
    return TRUE;
}

MS_BOOL DMD_DVBC_Exit(void)
{
	 DMD_DVBC_ResData *pRes = psDMD_DVBC_ResData;
	MS_BOOL bRet;

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBC_Exit\n");
    }
    #endif
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DMD_DVBC_Exit\n"));
    pRes->sDMD_DVBC_SHARE_MEMORY_InitData.b_IsInit_DVBCCreat = 0; 
	ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c] DVBCCreat = %d\n", pRes->sDMD_DVBC_SHARE_MEMORY_InitData.b_IsInit_DVBCCreat);
    DMD_LOCK();
    bRet = INTERN_DVBC_Exit();
    DMD_UNLOCK();
    MsOS_DeleteMutex(_s32DMD_DVBC_Mutex);
    _s32DMD_DVBC_Mutex= -1;
    return bRet;
}

MS_BOOL DMD_DVBC_SetDbgLevel(DMD_DVBC_DbgLv u8DbgLevel)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DMD_DVBC_SetDbgLevel\n"));

    DMD_LOCK();
    _u8DMD_DVBC_DbgLevel = u8DbgLevel;
    DMD_UNLOCK();
    return TRUE;
}

const DMD_DVBC_Info* DMD_DVBC_GetInfo(void)
{
#ifdef MS_DEBUG	
   MS_U8 state = 0;
#endif
	
    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBC_GetInfo\n");
    }
    #endif	
//for DBG	
#ifdef MS_DEBUG	
	MDrv_SYS_DMD_VD_MBX_ReadReg(FEC_REG_BASE + 0xE0, &state);
	ULOGD("DEMOD"," @INTERN_DVBC_GetLock FSM 0x%x\n",state);
        INTERN_DVBC_Show_AGC_Info();
	INTERN_DVBC_info();
#endif

    return (const DMD_DVBC_Info*)&sDMD_DVBC_Info;
}

MS_BOOL DMD_DVBC_GetLibVer(const MSIF_Version **ppVersion)
{
    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBC_GetLibVer\n");
    }
    #endif
    DMD_LOCK();
    if (!ppVersion)
    {
        return FALSE;
    }

    *ppVersion = &_drv_dmd_dvbc_intern_version;
    DMD_UNLOCK();
    return TRUE;
}

MS_BOOL DMD_DVBC_GetFWVer(MS_U16 *ver)
{

    MS_BOOL bRet;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DMD_DVBC_GetFWVer\n"));

    DMD_LOCK();

    bRet = INTERN_DVBC_Version(ver);
    //ULOGD("DEMOD","MDrv_DMD_DVBC_GetFWVer %x\n", *ver);
    DMD_UNLOCK();

    return bRet;

}

MS_BOOL DMD_DVBC_GetDSPReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
#if defined(CHIP_K1)||defined(CHIP_KENYA)
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=MDrv_SYS_DMD_VD_MBX_ReadDSPReg(u16Addr, pu8Data);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBC_GetDSPReg %x %x\n", u16Addr, *pu8Data);
    }
    #endif

    return bRet;
#else
  	ULOGD("DEMOD","Not Support function: MDrv_DMD_DVBC_GetDSPReg\n");
	return FALSE;
#endif
}

MS_BOOL DMD_DVBC_SetDSPReg(MS_U16 u16Addr, MS_U8 pu8Data)
{
#if defined(CHIP_K1)||defined(CHIP_KENYA)
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=MDrv_SYS_DMD_VD_MBX_WriteDSPReg(u16Addr, pu8Data);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBC_SetDSPReg %x %x\n", u16Addr, pu8Data);
    }
    #endif

    return bRet;
#else
  	ULOGD("DEMOD","Not Support function: MDrv_DMD_DVBC_SetDSPReg\n");
	return FALSE;
#endif
}


MS_BOOL DMD_DVBC_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DMD_DVBC_GetReg\n"));

    DMD_LOCK();
    bRet=MDrv_SYS_DMD_VD_MBX_ReadReg(u16Addr, pu8Data);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBC_GetReg %x %x\n", u16Addr, *pu8Data);
    }
    #endif
    return bRet;
}

MS_BOOL DMD_DVBC_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DMD_DVBC_SetReg\n"));

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBC_SetReg %x %x\n", u16Addr, u8Data);
    }
    #endif
    DMD_LOCK();
    bRet=MDrv_SYS_DMD_VD_MBX_WriteReg(u16Addr, u8Data);
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL DMD_DVBC_SetSerialControl(MS_BOOL bEnable)
{
    MS_BOOL bRet;
    MS_U8 u8TSClk;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DMD_DVBC_SetSerialControl\n"));

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBC_SetSerialControl %d\n", bEnable);
    }
    #endif

    DMD_LOCK();
    if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt != NULL)
    {
        if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[0]>=1)
        {
            u8TSClk = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[2];
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
    bRet=INTERN_DVBC_Serial_Control(bEnable, u8TSClk);
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL DMD_DVBC_SetConfig(MS_U16 u16SymbolRate, DMD_DVBC_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DMD_DVBC_SetConfig\n"));

		return MDrv_DMD_DVBC_SetConfig_symbol_rate_list(u16SymbolRate, eQamMode, u32IFFreq, bSpecInv, bSerialTS, NULL, 0);
	}

MS_BOOL DMD_DVBC_SetConfig_symbol_rate_list(MS_U16 u16SymbolRate, DMD_DVBC_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num)
{
	MS_BOOL bRet;
	MS_U8 u8TSClk;
	DMD_DVBC_ResData *pRes = psDMD_DVBC_ResData;

	ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c][_prev_u16PowerState = %d] DMD_DVBC_SetConfig_symbol_rate_list\n", _prev_u16PowerState);


	if(E_POWER_MECHANICAL==_prev_u16PowerState)
	{
		pRes->sDMD_DVBC_SHARE_MEMORY_PreviousData.u16SymbolRate = u16SymbolRate;
		pRes->sDMD_DVBC_SHARE_MEMORY_PreviousData.eQamMode = eQamMode;
		pRes->sDMD_DVBC_SHARE_MEMORY_PreviousData.u32IFFreq = u32IFFreq;
		pRes->sDMD_DVBC_SHARE_MEMORY_PreviousData.bSpecInv = bSpecInv;
		pRes->sDMD_DVBC_SHARE_MEMORY_PreviousData.bSerialTS = bSerialTS;

		ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c][E_POWER_MECHANICAL]DMD_DVBC_SetConfig_symbol_rate_list\n");
		ULOGD("DEMOD","SR = %d, QamMode = %d, IFFreq = %d, SpecInv = %d, SerialTS = %d\n",(int)u16SymbolRate, (int)eQamMode,(int) u32IFFreq, (int)bSpecInv, bSerialTS);
	}
	else if(E_POWER_RESUME==_prev_u16PowerState)
	{
		u16SymbolRate = pRes->sDMD_DVBC_SHARE_MEMORY_PreviousData.u16SymbolRate;
		eQamMode = pRes->sDMD_DVBC_SHARE_MEMORY_PreviousData.eQamMode;
		u32IFFreq = pRes->sDMD_DVBC_SHARE_MEMORY_PreviousData.u32IFFreq;
		bSpecInv = pRes->sDMD_DVBC_SHARE_MEMORY_PreviousData.bSpecInv;
		bSerialTS = pRes->sDMD_DVBC_SHARE_MEMORY_PreviousData.bSerialTS;
		ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c][E_POWER_RESUME]DMD_DVBC_SetConfig_symbol_rate_list\n");
		ULOGD("DEMOD","SR = %d, QamMode = %d, IFFreq = %d, SpecInv = %d, SerialTS = %d\n",(int)u16SymbolRate, (int)eQamMode, (int)u32IFFreq, (int)bSpecInv, (int)bSerialTS);
	}
	else
	{
			ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c][check!!]DMD_DVBC_SetConfig_symbol_rate_list\n");
	}

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBC_SetConfig %d %d %ld %d %d\n", u16SymbolRate, eQamMode, u32IFFreq, bSpecInv, bSerialTS);
    }
    #endif

		ULOGD("DEMOD","bryan check QAM mode=%d in V2\n",eQamMode);


    DMD_LOCK();
    if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt != NULL)
    {
        if (_sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[0]>=1)
        {
            u8TSClk = _sDMD_DVBC_InitData.u8DMD_DVBC_InitExt[2];
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
    bRet=INTERN_DVBC_Config(u16SymbolRate, eQamMode, u32IFFreq, bSpecInv, bSerialTS, u8TSClk, pu16_symbol_rate_list,u8_symbol_rate_list_num);
    sDMD_DVBC_Info.u16SymbolRate = u16SymbolRate;
    sDMD_DVBC_Info.eQamMode = eQamMode;
    sDMD_DVBC_Info.u32IFFreq = u32IFFreq;
    sDMD_DVBC_Info.bSpecInv = bSpecInv;
    sDMD_DVBC_Info.bSerialTS = bSerialTS;




	
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL DMD_DVBC_SetActive(MS_BOOL bEnable)
{
    MS_BOOL bRet;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DMD_DVBC_SetActive\n"));

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBC_SetActive %d\n", bEnable);
    }
    #endif

    DMD_LOCK();
    bRet=INTERN_DVBC_Active(bEnable);
    sDMD_DVBC_Info.u32ChkScanTimeStart = MsOS_GetSystemTime();
    u32DMD_DVBC_ScanCount=0;
    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        ULOGD("DEMOD","%s %ld\n", __FUNCTION__, sDMD_DVBC_Info.u32ChkScanTimeStart);
    }
    #endif
    DMD_UNLOCK();
    return bRet;
}



MS_BOOL DMD_DVBC_GetLockWithRFPower(DMD_DVBC_GETLOCK_TYPE eType, MS_U32 u32CurrRFPowerDbm, MS_U32 u32NoChannelRFPowerDbm, DMD_DVBC_LOCK_STATUS *eLockStatus)
{
    MS_U32 u32CurrScanTime=0;
    MS_BOOL bRet=FALSE;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DMD_DVBC_GetLockWithRFPower\n"));
    
    DMD_LOCK();
    if ( eType == DMD_DVBC_GETLOCK ) // for channel scan
    {
        if (INTERN_DVBC_GetLock(DMD_DVBC_GETLOCK_FEC_LOCK, 2000, -2000, 0))
        {
            *eLockStatus = DMD_DVBC_LOCK;
        }
        else
        {
            MS_U32 u32Timeout = 10000;//(sDMD_DVBC_Info.u16SymbolRate) ? 5000 : 15000;
            if (sDMD_DVBC_Info.u16SymbolRate == 0)
            {
                u32Timeout = u16DMD_DVBC_AutoSymbol_Timeout;
            }
            else
            {
                u32Timeout = (sDMD_DVBC_Info.eQamMode == DMD_DVBC_QAMAUTO) ? u16DMD_DVBC_FixSymbol_AutoQam_Timeout : u16DMD_DVBC_FixSymbol_FixQam_Timeout;
            }

            u32CurrScanTime=MsOS_GetSystemTime();
            if (u32CurrScanTime - sDMD_DVBC_Info.u32ChkScanTimeStart < u32Timeout)
            {
                if (u32DMD_DVBC_ScanCount==0)
                {
                    u32DMD_DVBC_PrevScanTime=u32CurrScanTime;
                    u32DMD_DVBC_ScanCount++;
                }

                if (_sDMD_DVBC_InitData.u8SarChannel != 0xFF)
                {
                //bryan temp mark
                 //   sDMD_DVBC_Info.u8SarValue=MDrv_SAR_Adc_GetValue(_sDMD_DVBC_InitData.u8SarChannel);
                }
                else
                {
                    sDMD_DVBC_Info.u8SarValue=0xFF;
                }

                /*bRet=HAL_DMD_GetRFLevel(&fCurrRFPowerDbm, fCurrRFPowerDbm, sDMD_DVBC_Info.u8SarValue,
                                                                            _sDMD_DVBC_InitData.pTuner_RfagcSsi, _sDMD_DVBC_InitData.u16Tuner_RfagcSsi_Size,
                                                                            _sDMD_DVBC_InitData.pTuner_IfagcSsi_HiRef, _sDMD_DVBC_InitData.u16Tuner_IfagcSsi_HiRef_Size,
                                                                            _sDMD_DVBC_InitData.pTuner_IfagcSsi_LoRef, _sDMD_DVBC_InitData.u16Tuner_IfagcSsi_LoRef_Size,
                                                                            _sDMD_DVBC_InitData.pTuner_IfagcErr_HiRef, _sDMD_DVBC_InitData.u16Tuner_IfagcErr_HiRef_Size,
                                                                            _sDMD_DVBC_InitData.pTuner_IfagcErr_LoRef, _sDMD_DVBC_InitData.u16Tuner_IfagcErr_LoRef_Size);
		*/
                //if (INTERN_DVBC_GetLock(DMD_DVBC_GETLOCK_NO_CHANNEL, u32CurrScanTime-u32DMD_DVBC_PrevScanTime))
                if (INTERN_DVBC_GetLock(DMD_DVBC_GETLOCK_NO_CHANNEL, u32CurrRFPowerDbm, u32NoChannelRFPowerDbm, u32CurrScanTime-u32DMD_DVBC_PrevScanTime))
                {
                    #ifdef MS_DEBUG
                    ULOGD("DEMOD","%s %ld UNLOCK:NO_CHANNEL\n", __FUNCTION__, u32CurrScanTime-sDMD_DVBC_Info.u32ChkScanTimeStart);
                    #endif
                    *eLockStatus = DMD_DVBC_UNLOCK;
                }
                else if (INTERN_DVBC_GetLock(DMD_DVBC_GETLOCK_ATV_DETECT, 2000, -2000, 0))
                {
                    #ifdef MS_DEBUG
                    ULOGD("DEMOD","%s %ld UNLOCK:ATV_DETECT\n", __FUNCTION__, u32CurrScanTime-sDMD_DVBC_Info.u32ChkScanTimeStart);
                    #endif
                    *eLockStatus = DMD_DVBC_UNLOCK;
                }
#ifdef NEW_TR_MODULE
                else if ((INTERN_DVBC_GetLock(DMD_DVBC_GETLOCK_TR_EVER_LOCK,  2000, -2000, 0) == FALSE) && ((u32CurrScanTime - sDMD_DVBC_Info.u32ChkScanTimeStart) > 500))
                
#else
                else if ((INTERN_DVBC_GetLock(DMD_DVBC_GETLOCK_TR_EVER_LOCK,  2000, -2000, 0) == FALSE) && ((u32CurrScanTime - sDMD_DVBC_Info.u32ChkScanTimeStart) > 5000))
#endif
                {
                    #ifdef MS_DEBUG
                    ULOGD("DEMOD","%s %ld UNLOCK:TR\n", __FUNCTION__, u32CurrScanTime-sDMD_DVBC_Info.u32ChkScanTimeStart);
                    #endif
                    *eLockStatus = DMD_DVBC_UNLOCK;
                }
                else
                {
                    *eLockStatus = DMD_DVBC_CHECKING;
                }
            }
            else
            {
                #ifdef MS_DEBUG
                ULOGD("DEMOD","%s %ld UNLOCK:TIMEOUT %ld\n", __FUNCTION__, MsOS_GetSystemTime(), u32Timeout);
                #endif
                *eLockStatus = DMD_DVBC_UNLOCK;
            }
            u32DMD_DVBC_PrevScanTime=u32CurrScanTime;
        }
        sDMD_DVBC_Info.eLockStatus = *eLockStatus;
        #ifdef MS_DEBUG
        if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
        {
            ULOGD("DEMOD","%s %ld %d\n", __FUNCTION__, MsOS_GetSystemTime(), sDMD_DVBC_Info.eLockStatus);
        }
        #endif
    }
    else
    {
        if (INTERN_DVBC_GetLock(eType, 2000, -2000, 0) == TRUE)
        {
            *eLockStatus = DMD_DVBC_LOCK;
        }
        else
        {
            *eLockStatus = DMD_DVBC_UNLOCK;
        }
    }
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBC_GetLock %x %x\n", eType, *eLockStatus);
    }
    #endif
    bRet=TRUE;
    
    
    #ifdef MS_DEBUG

	if(*eLockStatus ==DMD_DVBC_LOCK)
		ULOGD("DEMOD","check kernel driver DVBC locked!!\n");
	else
    ULOGD("DEMOD","check kernel driver DVBC unlock!!\n");
	#endif
    		
    return bRet;
}


/*
MS_BOOL DMD_DVBC_GetSignalStrength(MS_U16 *u16Strength)
{
    return MDrv_DMD_DVBC_GetSignalStrengthWithRFPower(u16Strength);
}

MS_BOOL DMD_DVBC_GetSignalStrengthWithRFPower(MS_U16 *u16Strength)
{
    MS_BOOL bRet;

    DMD_LOCK();
    if (_sDMD_DVBC_InitData.u8SarChannel != 0xFF)
    {
        sDMD_DVBC_Info.u8SarValue=MDrv_SAR_Adc_GetValue(_sDMD_DVBC_InitData.u8SarChannel);
    }
    else
    {
        sDMD_DVBC_Info.u8SarValue=0xFF;
    }
    bRet=INTERN_DVBC_GetSignalStrength(u16Strength, (const DMD_DVBC_InitData *)(&_sDMD_DVBC_InitData), sDMD_DVBC_Info.u8SarValue);
    sDMD_DVBC_Info.u16Strength=*u16Strength;
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBC_GetSignalStrength %d\n", *u16Strength);
    }
    #endif
    return bRet;
}

MS_BOOL DMD_DVBC_GetSignalQuality(MS_U16 *u16Quality)
{
    return MDrv_DMD_DVBC_GetSignalQualityWithRFPower(u16Quality);
}

MS_BOOL DMD_DVBC_GetSignalQualityWithRFPower(MS_U16 *u16Quality)
{
    MS_BOOL bRet;

    DMD_LOCK();
    if (_sDMD_DVBC_InitData.u8SarChannel != 0xFF)
    {
        sDMD_DVBC_Info.u8SarValue=MDrv_SAR_Adc_GetValue(_sDMD_DVBC_InitData.u8SarChannel);
    }
    else
    {
        sDMD_DVBC_Info.u8SarValue=0xFF;
    }
    bRet=INTERN_DVBC_GetSignalQuality(u16Quality, (const DMD_DVBC_InitData *)(&_sDMD_DVBC_InitData), sDMD_DVBC_Info.u8SarValue);
    sDMD_DVBC_Info.u16Quality=*u16Quality;
    DMD_UNLOCK();
    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        ULOGD("DEMOD","%s %d %d\n", __FUNCTION__, bRet, sDMD_DVBC_Info.u16Quality);
    }
    #endif

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBC_GetSignalQuality %d\n", *u16Quality);
    }
    #endif
    return bRet;
}
*/

#if defined(CHIP_KAISER)||defined(CHIP_K6LITE)
MS_BOOL DMD_DVBC_ActiveDmdSwitch(MS_U8 demod_no)
{
    MS_BOOL bRet=TRUE;
    DMD_LOCK();
    if((demod_no==0)  &&(drv_demod_swtich_status!=0))
    {
    	//save the demod 1 parameter back
    	_sDMD_DVBC_InitData_dmd1=_sDMD_DVBC_InitData;
	sDMD_DVBC_Info_dmd1=sDMD_DVBC_Info;
	u16DMD_DVBC_AutoSymbol_Timeout_dmd1=u16DMD_DVBC_AutoSymbol_Timeout;
	u16DMD_DVBC_FixSymbol_AutoQam_Timeout_dmd1=u16DMD_DVBC_FixSymbol_AutoQam_Timeout;
	u16DMD_DVBC_FixSymbol_FixQam_Timeout_dmd1=u16DMD_DVBC_FixSymbol_FixQam_Timeout;
	u32DMD_DVBC_PrevScanTime_dmd1=u32DMD_DVBC_PrevScanTime;
	u32DMD_DVBC_ScanCount_dmd1=u32DMD_DVBC_ScanCount;
    }
    else if((demod_no==1)  &&(drv_demod_swtich_status!=1))
    {
    	//save the demod 0 parameter back
    	_sDMD_DVBC_InitData_dmd0=_sDMD_DVBC_InitData;
	sDMD_DVBC_Info_dmd0=sDMD_DVBC_Info;
	u16DMD_DVBC_AutoSymbol_Timeout_dmd0=u16DMD_DVBC_AutoSymbol_Timeout;
	u16DMD_DVBC_FixSymbol_AutoQam_Timeout_dmd0=u16DMD_DVBC_FixSymbol_AutoQam_Timeout;
	u16DMD_DVBC_FixSymbol_FixQam_Timeout_dmd0=u16DMD_DVBC_FixSymbol_FixQam_Timeout;
	u32DMD_DVBC_PrevScanTime_dmd0=u32DMD_DVBC_PrevScanTime;
	u32DMD_DVBC_ScanCount_dmd0=u32DMD_DVBC_ScanCount;
    }

    
    bRet=INTERN_DVBC_ActiveDmdSwitch(demod_no);

    if((demod_no==0)  &&(drv_demod_swtich_status!=0))
    {
    	//insert the demod 0 parameter
    	_sDMD_DVBC_InitData=_sDMD_DVBC_InitData_dmd0;
	sDMD_DVBC_Info=sDMD_DVBC_Info_dmd0;
	u16DMD_DVBC_AutoSymbol_Timeout=u16DMD_DVBC_AutoSymbol_Timeout_dmd0;
	u16DMD_DVBC_FixSymbol_AutoQam_Timeout=u16DMD_DVBC_FixSymbol_AutoQam_Timeout_dmd0;
	u16DMD_DVBC_FixSymbol_FixQam_Timeout=u16DMD_DVBC_FixSymbol_FixQam_Timeout_dmd0;
	u32DMD_DVBC_PrevScanTime=u32DMD_DVBC_PrevScanTime_dmd0;
	u32DMD_DVBC_ScanCount=u32DMD_DVBC_ScanCount_dmd0;
    }
    else if((demod_no==1)  &&(drv_demod_swtich_status!=1))
    {
    	//insert the demod 1 parameter
    	_sDMD_DVBC_InitData=_sDMD_DVBC_InitData_dmd1;
	sDMD_DVBC_Info=sDMD_DVBC_Info_dmd1;
	u16DMD_DVBC_AutoSymbol_Timeout=u16DMD_DVBC_AutoSymbol_Timeout_dmd1;
	u16DMD_DVBC_FixSymbol_AutoQam_Timeout=u16DMD_DVBC_FixSymbol_AutoQam_Timeout_dmd1;
	u16DMD_DVBC_FixSymbol_FixQam_Timeout=u16DMD_DVBC_FixSymbol_FixQam_Timeout_dmd1;
	u32DMD_DVBC_PrevScanTime=u32DMD_DVBC_PrevScanTime_dmd1;
	u32DMD_DVBC_ScanCount=u32DMD_DVBC_ScanCount_dmd1;
    }

    drv_demod_swtich_status=demod_no;
    DMD_UNLOCK();
    return bRet;
}
#else
MS_BOOL DMD_DVBC_ActiveDmdSwitch(MS_U8 demod_no)
{
	ULOGD("DEMOD","Doesn't support DVBC_ActiveDmdSwitch function!!!\n");
	return false;
}
#endif

MS_BOOL DMD_DVBC_GetSNR(MS_U16 *snr_reg)
{
    MS_BOOL bRet;
    DMD_LOCK();
    bRet=INTERN_DVBC_GetSNR(snr_reg);
    DMD_UNLOCK();

    return bRet;
}

//waiting add
MS_BOOL DMD_DVBC_GetPostViterbiBer(MS_U32 *BitErr_reg, MS_U16 *BitErrPeriod_reg)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBC_GetPostViterbiBer(BitErr_reg, BitErrPeriod_reg);
    DMD_UNLOCK();

	  return bRet;
}


#ifdef UFO_DEMOD_DVBC_GET_AGC_INFO
MS_BOOL DMD_DVBC_GetAGCInfo(MS_U8 u8dbg_mode, MS_U16 *pu16Data)
{
    MS_BOOL bRet;    
    DMD_LOCK();
    bRet = INTERN_DVBC_AGC_Info(u8dbg_mode, pu16Data);
    DMD_UNLOCK();
    return bRet;
}
#endif

//waiting add
MS_BOOL DMD_DVBC_GetIFAGC(MS_U8 *ifagc_reg, MS_U8 *ifagc_reg_lsb, MS_U16 *ifagc_err)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBC_GetIFAGC(ifagc_reg, ifagc_reg_lsb, ifagc_err);
    DMD_UNLOCK();

	  return bRet;
}


MS_BOOL DMD_DVBC_GetPacketErr(MS_U16 *pktErr)
{
    MS_BOOL bRet;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DMD_DVBC_GetPacketErr\n"));

    DMD_LOCK();
    bRet=INTERN_DVBC_GetPacketErr(pktErr);
    DMD_UNLOCK();

	  return bRet;
}

MS_BOOL DMD_DVBC_GetCellID(MS_U16 *u16CellID)
{
    MS_BOOL bRet;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DMD_DVBC_GetCellID\n"));

    DMD_LOCK();
    bRet=INTERN_DVBC_Get_CELL_ID(u16CellID);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBC_GetCellID %x\n", *u16CellID);
    }
    #endif
    return bRet;
}


//bryan temp mark
#if(1)
MS_BOOL DMD_DVBC_GetStatus(DMD_DVBC_MODULATION_TYPE *pQAMMode, MS_U16 *u16SymbolRate, MS_U32 *config_Fc_reg, MS_U32 *Fc_over_Fs_reg, MS_U16 *Cfo_offset_reg)
{
    MS_BOOL bRet=TRUE;

    DMD_LOCK();
    INTERN_DVBC_GetCurrentModulationType(pQAMMode);
    INTERN_DVBC_GetCurrentSymbolRate(u16SymbolRate);
    INTERN_DVBC_Get_FreqOffset(config_Fc_reg, Fc_over_Fs_reg, Cfo_offset_reg, 8);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
        //ULOGD("DEMOD","MDrv_DMD_DVBC_GetStatus %d %d\n", *pQAMMode, *u16SymbolRate, *pFreqOff_reg);
    }
    #endif
    return bRet;
}
#else
MS_BOOL DMD_DVBC_GetStatus(DMD_DVBC_MODULATION_TYPE *pQAMMode, MS_U16 *u16SymbolRate)
{
    MS_BOOL bRet=TRUE;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DMD_DVBC_GetStatus\n"));

    DMD_LOCK();
    INTERN_DVBC_GetCurrentModulationType(pQAMMode);
    INTERN_DVBC_GetCurrentSymbolRate(u16SymbolRate);
   // INTERN_DVBC_Get_FreqOffset(pFreqOff,8);
    DMD_UNLOCK();
    //ULOGD("DEMOD","bryan get status QAM mode=%d in V2\n",*pQAMMode);
    #ifdef MS_DEBUG
    if (_u8DMD_DVBC_DbgLevel >= DMD_DVBC_DBGLV_INFO)
    {
     //   ULOGD("DEMOD","MDrv_DMD_DVBC_GetStatus %d %d %f\n", *pQAMMode, *u16SymbolRate, *pFreqOff);
    }
    #endif
    return bRet;
}
#endif

MS_U32 DMD_DVBC_SetPowerState(EN_POWER_MODE u16PowerState)
{

	MS_U32 u32Return = UTOPIA_STATUS_FAIL;
	DMD_DVBC_ResData *pRes = psDMD_DVBC_ResData;


    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DMD_DVBC_SetPowerState\n"));	
    
    u32Return = u32Return;

	if (u16PowerState == E_POWER_SUSPEND)
	{
		pRes->sDMD_DVBC_SHARE_MEMORY_InitData.b_DCOFF_IsDVBC = pRes->sDMD_DVBC_SHARE_MEMORY_InitData.b_IsInit_DVBCCreat;
		pRes->sDMD_DVBC_SHARE_MEMORY_InitData.eLastState  = u16PowerState;
		_prev_u16PowerState = u16PowerState;

		ULOGD("DEMOD","[E_POWER_SUSPEND] DVBCCreat =%d \n",pRes->sDMD_DVBC_SHARE_MEMORY_InitData.b_IsInit_DVBCCreat);
		ULOGD("DEMOD","[E_POWER_SUSPEND] b_DCOFF_IsDVBC =%d \n",pRes->sDMD_DVBC_SHARE_MEMORY_InitData.b_DCOFF_IsDVBC);
		if(pRes->sDMD_DVBC_SHARE_MEMORY_InitData.b_IsInit_DVBCCreat)
		{			
			DMD_DVBC_Exit();
		}
		else
		{			
		}

	    
	   
	    u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
	}
        else if (u16PowerState == E_POWER_RESUME)
        {
            if (_prev_u16PowerState == E_POWER_SUSPEND)
            {
			_prev_u16PowerState = u16PowerState;
			pRes->sDMD_DVBC_SHARE_MEMORY_InitData.eLastState  = u16PowerState;			
			ULOGD("DEMOD","[E_POWER_RESUME + E_POWER_SUSPEND] b_DCOFF_IsDVBC = %d \n",pRes->sDMD_DVBC_SHARE_MEMORY_InitData.b_DCOFF_IsDVBC);
			if(pRes->sDMD_DVBC_SHARE_MEMORY_InitData.b_DCOFF_IsDVBC)
			{
				
				DMD_DVBC_Init(&_sDMD_DVBC_InitData, sizeof(_sDMD_DVBC_InitData));
				DMD_DVBC_SetConfig_symbol_rate_list(1,2,3,0,0,NULL,0);
				DMD_DVBC_SetActive(1);				
			}
			else
			{				
			}

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
        ULOGD("DEMOD","\r\n ====== DVBC doesn't need to Suspend/Resume at Non-DVBC mode ====== \r\n");
        u32Return = UTOPIA_STATUS_FAIL; 
    }

    return u32Return;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_U32 DVBCOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCOpen\n"));	
    
    DVBC_INSTANT_PRIVATE *pDvbcPri= NULL;

    UtopiaInstanceCreate(sizeof(DVBC_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void*)&pDvbcPri);


	pDvbcPri->fpDVBC_Init=DMD_DVBC_Init;
	pDvbcPri->fpDVBC_Dual_Individual_Init=DMD_DVBC_Dual_Individual_Init;
	pDvbcPri->fpDVBC_Dual_Public_Init=DMD_DVBC_Dual_Public_Init;
	pDvbcPri->fpDVBC_Exit=DMD_DVBC_Exit;
	pDvbcPri->fpDVBC_SetDbgLevel=DMD_DVBC_SetDbgLevel;
  
	pDvbcPri->fpDVBC_GetInfo=DMD_DVBC_GetInfo;
	pDvbcPri->fpDVBC_GetLibVer=DMD_DVBC_GetLibVer;
	
	pDvbcPri->fpDVBC_GetFWVer=DMD_DVBC_GetFWVer;
	pDvbcPri->fpDVBC_GetDSPReg=DMD_DVBC_GetDSPReg;
	pDvbcPri->fpDVBC_SetDSPReg=DMD_DVBC_SetDSPReg;
	pDvbcPri->fpDVBC_GetReg=DMD_DVBC_GetReg;
	pDvbcPri->fpDVBC_SetReg=DMD_DVBC_SetReg;

	pDvbcPri->fpDVBCSetSerialControl=DMD_DVBC_SetSerialControl;
	pDvbcPri->fpDVBC_SetConfig_symbol_rate_list=DMD_DVBC_SetConfig_symbol_rate_list;
	pDvbcPri->fpDVBC_SetActive=DMD_DVBC_SetActive;

	pDvbcPri->fpDVBC_GetLockWithRFPower=DMD_DVBC_GetLockWithRFPower;

	//pDvbcPri->fpDVBC_GetSignalStrengthWithRFPower=DMD_DVBC_GetSignalStrengthWithRFPower;
	//pDvbcPri->fpDVBC_GetSignalQualityWithRFPower=DMD_DVBC_GetSignalQualityWithRFPower;
	
	pDvbcPri->fpDVBC_ActiveDmdSwitch=DMD_DVBC_ActiveDmdSwitch;

	pDvbcPri->fpDVBC_GetSNR=DMD_DVBC_GetSNR;
  pDvbcPri->fpDVBC_GetPostViterbiBer=DMD_DVBC_GetPostViterbiBer;

	pDvbcPri->fpDVBC_GetPacketErr=DMD_DVBC_GetPacketErr;

	pDvbcPri->fpDVBC_GetCellID=DMD_DVBC_GetCellID;
	//bryan temp mark pDvbcPri->fpDVBC_GetStatus=DMD_DVBC_GetStatus;
	pDvbcPri->fpDVBC_GetStatus=DMD_DVBC_GetStatus;
	
	pDvbcPri->fpDVBC_SetPowerState=DMD_DVBC_SetPowerState;
	
	//waiting add
	pDvbcPri->fpDVBC_GetIFAGC=DMD_DVBC_GetIFAGC;
	
	#ifdef UFO_DEMOD_DVBC_GET_AGC_INFO
	pDvbcPri->fpDVBC_GetAGCInfo=DMD_DVBC_GetAGCInfo;
	#endif

    //return TRUE;
    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 DVBCIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    //ULOGD("DEMOD","\n[drvDMD_INTERN_DVBC_v2.c][DVBC INFO] DVBC Ioctl \n");
    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    
    void* pResource = NULL;
    DVBC_RESOURCE_PRIVATE* psDVBCResPri = NULL;
    DVBC_INSTANT_PRIVATE* psDVBCInstPri = NULL;
    void* psDVBCInstPriVoid = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psDVBCInstPriVoid);
    psDVBCInstPri = (DVBC_INSTANT_PRIVATE*)psDVBCInstPriVoid;
    
    MS_BOOL bRet = FALSE;

    if (UtopiaResourceObtain(pModule, DVBC_POOL_ID_DMD0, &pResource) != 0)
    {
        DMD_DBG(ULOGD("DEMOD","UtopiaResourceObtainToInstant fail\n"));
	    return UTOPIA_STATUS_ERR_RESOURCE;
    }

    if(UtopiaResourceGetPrivate(pResource,(void*)&psDVBCResPri) != 0)
    {
        printf("UtopiaResourceGetPrivate fail\n");
        return UTOPIA_STATUS_FAIL;
    }


	psDMD_DVBC_ResData = ((PDVBC_RESOURCE_PRIVATE)psDVBCResPri)->sDMD_DVBC_ResData;

    switch (u32Cmd)
    {
#if defined(CHIP_KAISER)||defined(CHIP_K6LITE)

	case DMD_DVBC_DRV_CMD_Dual_Public_Init:
              DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - u8AGC_Tristate_Ctrl\n"));    
		bRet=psDVBCInstPri->fpDVBC_Dual_Public_Init(((PDVBC_Dual_Public_Init_PARAM)pArgs)->u8AGC_Tristate_Ctrl,
		((PDVBC_Dual_Public_Init_PARAM)pArgs)->u8Sar_Channel);

		((PDVBC_Dual_Public_Init_PARAM)pArgs)->ret=bRet;
	break;
       case DMD_DVBC_DRV_CMD_Dual_Individual_Init:
              DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - pDMD_DVBC_InitData\n"));        
        bRet=psDVBCInstPri->fpDVBC_Dual_Individual_Init(((PDVBC_Dual_Individual_Init_PARAM)pArgs)->pDMD_DVBC_InitData,
        ((PDVBC_Dual_Individual_Init_PARAM)pArgs)->u32InitDataLen);

        ((PDVBC_Dual_Individual_Init_PARAM)pArgs)->ret=bRet;
    	break;
    	#endif
    	
        case DMD_DVBC_DRV_CMD_Init:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - MDrv_DMD_DVBC_Init\n"));
 	     bRet = psDVBCInstPri->fpDVBC_Init( (((PDVBC_Init_PARAM)pArgs)->pDMD_DVBC_InitData), ((PDVBC_Init_PARAM)pArgs)->u32InitDataLen);
 	     ((PDVBC_Init_PARAM)pArgs)->ret=bRet;
            break;
        case DMD_DVBC_DRV_CMD_Exit:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - MDrv_DMD_DVBC_Exit\n"));
	     bRet= psDVBCInstPri->fpDVBC_Exit();
	     ((PDVBC_EXIT_PARAM_PARAM)pArgs)->ret=bRet;
            break;
        case DMD_DVBC_DRV_CMD_SetDbgLevel:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - MDrv_DMD_DVBC_SetDbgLevel\n"));            
	    bRet= psDVBCInstPri->fpDVBC_SetDbgLevel(((PDVBC_SetDbgLevel_PARAM)pArgs)->u8DbgLevel);	
	    ((PDVBC_SetDbgLevel_PARAM)pArgs)->ret=bRet;
            break;
        case DMD_DVBC_DRV_CMD_GetInfo:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - MDrv_DMD_DVBC_GetInfo\n"));            
	    ((PDVBC_GetInfo_PARAM)pArgs)->ret_info=psDVBCInstPri->fpDVBC_GetInfo();
            break;
        case DMD_DVBC_DRV_CMD_GetLibVer:
            bRet=psDVBCInstPri->fpDVBC_GetLibVer(((PDVBC_GetLibVer_PARAM)pArgs)->ppVersion);
            ((PDVBC_GetLibVer_PARAM)pArgs)->ret=bRet;
            break;
        case DMD_DVBC_DRV_CMD_GetFWVer:
              DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - MDrv_DMD_DVBC_GetFWVer\n"));            
		bRet=psDVBCInstPri->fpDVBC_GetFWVer(((PDVBC_GetFWVer_PARAM)pArgs)->ver);
	       ((PDVBC_GetFWVer_PARAM)pArgs)->ret=bRet;
            break;
        case DMD_DVBC_DRV_CMD_GetDSPReg:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - MDrv_DMD_DVBC_GetDSPReg\n"));                   
		bRet=psDVBCInstPri->fpDVBC_GetDSPReg(((PDVBC_GetDSPReg_PARAM)pArgs)->u16Addr,((PDVBC_GetDSPReg_PARAM)pArgs)->pu8Data);
		((PDVBC_GetDSPReg_PARAM)pArgs)->ret=bRet;
        break;

        case DMD_DVBC_DRV_CMD_SetDSPReg:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - MDrv_DMD_DVBC_SetDSPReg\n"));                   
		bRet=psDVBCInstPri->fpDVBC_SetDSPReg(((PDVBC_SetDSPReg_PARAM)pArgs)->u16Addr,((PDVBC_SetDSPReg_PARAM)pArgs)->pu8Data);
		((PDVBC_SetDSPReg_PARAM)pArgs)->ret=bRet;
        break;
        case DMD_DVBC_DRV_CMD_GetReg:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - MDrv_DMD_DVBC_GetReg\n"));            
		bRet=psDVBCInstPri->fpDVBC_GetReg(((PDVBC_GetReg_PARAM)pArgs)->u16Addr,((PDVBC_GetReg_PARAM)pArgs)->pu8Data);
		((PDVBC_GetReg_PARAM)pArgs)->ret=bRet;
            break;
        case DMD_DVBC_DRV_CMD_SetReg:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - MDrv_DMD_DVBC_SetReg\n"));            
		bRet=psDVBCInstPri->fpDVBC_SetReg(((PDVBC_SetReg_PARAM)pArgs)->u16Addr,((PDVBC_SetReg_PARAM)pArgs)->u8Data);
		((PDVBC_SetReg_PARAM)pArgs)->ret=bRet;
            break;
            
        case DMD_DVBC_DRV_CMD_SetSerialControl:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - MDrv_DMD_DVBC_SetSerialControl\n"));            
		bRet=psDVBCInstPri->fpDVBCSetSerialControl(((PDVBC_SetSerialControl_PARAM)pArgs)->bEnable);
		((PDVBC_SetSerialControl_PARAM)pArgs)->ret=bRet;
            break;

        case DMD_DVBC_DRV_CMD_SetSetConfig_symbol_rate_list:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - MDrv_DMD_DVBC_SetSetConfig_symbol_rate_list\n"));                        
		bRet=psDVBCInstPri->fpDVBC_SetConfig_symbol_rate_list(((PDVBC_SetConfig_Symbol_rate_list_PARAM)pArgs)->u16SymbolRate,\
		((PDVBC_SetConfig_Symbol_rate_list_PARAM)pArgs)->eQamMode,((PDVBC_SetConfig_Symbol_rate_list_PARAM)pArgs)->u32IFFreq,\
		((PDVBC_SetConfig_Symbol_rate_list_PARAM)pArgs)->bSpecInv,\
		((PDVBC_SetConfig_Symbol_rate_list_PARAM)pArgs)->bSerialTS,((PDVBC_SetConfig_Symbol_rate_list_PARAM)pArgs)->pu16_symbol_rate_list,\
		((PDVBC_SetConfig_Symbol_rate_list_PARAM)pArgs)->u8_symbol_rate_list_num
		);

		((PDVBC_SetConfig_Symbol_rate_list_PARAM)pArgs)->ret=bRet;
			
            break;
        case DMD_DVBC_DRV_CMD_SetActive:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - MDrv_DMD_DVBC_SetActive\n"));            
		bRet=psDVBCInstPri->fpDVBC_SetActive(((PDVBC_SetActive_PARAM)pArgs)->bEnable);
		((PDVBC_SetActive_PARAM)pArgs)->ret=bRet;
            break;

         
        case DMD_DVBC_DRV_CMD_GetLockWithRFPower:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - MDrv_DMD_DVBC_GetLockWithRFPower\n"));               

		//ULOGD("DEMOD","bryan check DVBC in V2 layer DMD_DVBC_DRV_CMD_GetLockWithRFPower\n");
		bRet=psDVBCInstPri->fpDVBC_GetLockWithRFPower(((PDVBC_GetLockWithRFPower)pArgs)->eType,\
		((PDVBC_GetLockWithRFPower)pArgs)->u32CurrRFPowerDbm,\
		((PDVBC_GetLockWithRFPower)pArgs)->u32NoChannelRFPowerDbm,\
		((PDVBC_GetLockWithRFPower)pArgs)->eLockStatus);
		((PDVBC_GetLockWithRFPower)pArgs)->ret=bRet;
			
            break;
/*
        case DMD_DVBC_DRV_CMD_GetSignalStrengthWithRFPower:
		bRet=psDVBCInstPri->fpDVBC_GetSignalStrengthWithRFPower(((PDVBC_GetSignalStrengthWithRFPower_PARAM)pArgs)->u16Strength);
		((PDVBC_GetSignalStrengthWithRFPower_PARAM)pArgs)->ret=bRet;
            break;

	case DMD_DVBC_DRV_CMD_GetSignalQualityWithRFPower:
		bRet=psDVBCInstPri->fpDVBC_GetSignalQualityWithRFPower((PDVBC_GetSignalStrengthWithRFPower_PARAM)pArgs->u16Strength);
		(PDVBC_GetSignalStrengthWithRFPower_PARAM)pArgs->ret=bRet;
	     break;
	#endif
*/
	
       case DMD_DVBC_DRV_CMD_ActiveDmdSwitch:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - MDrv_DMD_DVBC_ActiveDmdSwitch\n"));                 
		bRet=psDVBCInstPri->fpDVBC_ActiveDmdSwitch(((PDVBC_ActiveDmdSwitch_PARAM)pArgs)->demod_no);
		((PDVBC_ActiveDmdSwitch_PARAM)pArgs)->ret=bRet;
       	break;


       case DMD_DVBC_DRV_CMD_GetSNR:
       	bRet=psDVBCInstPri->fpDVBC_GetSNR(((PDVBC_GetSNR_PARAM)pArgs)->snr_reg);
       	((PDVBC_GetSNR_PARAM)pArgs)->ret=bRet;
       	break;

       // waiting add
       case DMD_DVBC_DRV_CMD_GetPostViterbiBer:
		bRet=psDVBCInstPri->fpDVBC_GetPostViterbiBer(((PDVBC_GetPostViterbiBer_PARAM)pArgs)->BitErr_reg,\
		((PDVBC_GetPostViterbiBer_PARAM)pArgs)->BitErrPeriod_reg);
		((PDVBC_GetPostViterbiBer_PARAM)pArgs)->ret=bRet;
           break;

	
       case DMD_DVBC_DRV_CMD_GetPacketErr:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - MDrv_DMD_DVBC_GetPacketErr\n"));                    
		bRet=psDVBCInstPri->fpDVBC_GetPacketErr(((PDVBC_GetPacketErr_PARAM)pArgs)->pktErr);
		((PDVBC_GetPacketErr_PARAM)pArgs)->ret=bRet;
           break;

	case DMD_DVBC_DRV_CMD_GetCellID:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - MDrv_DMD_DVBC_GetCellID\n"));                 
		bRet=psDVBCInstPri->fpDVBC_GetCellID(((PDVBC_GetCellID_PARAM)pArgs)->u16CellID);
		((PDVBC_GetCellID_PARAM)pArgs)->ret=bRet;
		break;
  
	case DMD_DVBC_DRV_CMD_GetStatus:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - MDrv_DMD_DVBC_GetStatus\n"));             
		bRet=psDVBCInstPri->fpDVBC_GetStatus(((PDVBC_GetStatus_PARAM)pArgs)->pQAMMode,((PDVBC_GetStatus_PARAM)pArgs)->u16SymbolRate,\
		((PDVBC_GetStatus_PARAM)pArgs)->config_Fc_reg,\
		((PDVBC_GetStatus_PARAM)pArgs)->Fc_over_Fs_reg,\
		((PDVBC_GetStatus_PARAM)pArgs)->Cfo_offset_reg);
		((PDVBC_GetStatus_PARAM)pArgs)->ret=bRet;
		break;	


	case DMD_DVBC_DRV_CMD_SetPowerState:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - MDrv_DMD_DVBC_SetPowerState\n"));            
		((PDVBC_SetPowerState_PARAM)pArgs)->ret_U32=psDVBCInstPri->fpDVBC_SetPowerState(((PDVBC_SetPowerState_PARAM)pArgs)->u16PowerState);

		break;
		
		
		// waiting add
       case DMD_DVBC_DRV_CMD_GetIFAGC:
       	DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - MDrv_DMD_DVBC_GetIFAGC\n"));
		bRet=psDVBCInstPri->fpDVBC_GetIFAGC(((PDVBC_GetIFAGC_PARAM)pArgs)->ifagc_reg,\
		((PDVBC_GetIFAGC_PARAM)pArgs)->ifagc_reg_lsb,\
		((PDVBC_GetIFAGC_PARAM)pArgs)->ifagc_err_reg);
		((PDVBC_GetIFAGC_PARAM)pArgs)->ret=bRet;
           break;
           
      #ifdef UFO_DEMOD_DVBC_GET_AGC_INFO     
      case DMD_DVBC_DRV_CMD_GetAGCInfo:
      DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCIoctl - MDrv_DMD_DVBC_GetAGCInfo\n"));
      bRet=psDVBCInstPri->fpDVBC_GetAGCInfo(((PDVBC_GetAGCInfo_PARAM)pArgs)->u8dbg_mode,\
		((PDVBC_GetAGCInfo_PARAM)pArgs)->pu16Data);
		((PDVBC_GetAGCInfo_PARAM)pArgs)->ret=bRet;
      break;
      #endif
		
         default:
            break;
    }
    
    //jway suggest UtopiaResourceRelease(pResource);
    UtopiaResourceRelease(pResource);
            
    return (bRet ? UTOPIA_STATUS_SUCCESS : UTOPIA_STATUS_FAIL);
}

MS_U32 DVBCClose(void* pInstance)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCClose\n"));

    UtopiaInstanceDelete(pInstance);

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 DVBCStr(MS_U32 u32PowerState, void* pModule)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    MS_U32 u32Ret = 0;
	
	void* pResource = NULL;
	DVBC_RESOURCE_PRIVATE* psDVBCResPri = NULL;	


    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCStr\n"));

    //UtopiaModuleGetSTRPrivate(pModule, (void**));

	if (UtopiaResourceObtain(pModule, DVBC_POOL_ID_DMD0, &pResource) != 0)
    {
        printf("DVBC - UtopiaResourceObtainToInstant fail\n");
	    return UTOPIA_STATUS_ERR_RESOURCE;
    }

    if(UtopiaResourceGetPrivate(pResource,(void*)&psDVBCResPri) != 0)
    {
        printf("DVBC - UtopiaResourceGetPrivate fail\n");
        return UTOPIA_STATUS_FAIL;
    }

    psDMD_DVBC_ResData = ((PDVBC_RESOURCE_PRIVATE)psDVBCResPri)->sDMD_DVBC_ResData;



    if (u32PowerState == E_POWER_SUSPEND)
    {
        /* Please Implement Module Suspend Flow Here. */
        u32Ret = DMD_DVBC_SetPowerState(E_POWER_SUSPEND);
        
        if(u32Ret == TRUE)
          u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
        else
          u32Return = UTOPIA_STATUS_FAIL;
        DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c][DVBCStr] SUSPEND !\n"));            
    }
    else if (u32PowerState == E_POWER_RESUME)
    {
        /* Please Implement Module Resume Flow Here. */
        u32Ret = DMD_DVBC_SetPowerState(E_POWER_RESUME);

        if(u32Ret == TRUE)        
          u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
        else
          u32Return = UTOPIA_STATUS_FAIL;
        DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c][DVBCStr] RESUME !\n"));               
    }
    else
    {
        u32Return = UTOPIA_STATUS_FAIL;
        DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c][DVBCStr] OTHERS !\n"));                     
    }

	   UtopiaResourceRelease(pResource);

    return u32Return;// for success
}

void DVBCRegisterToUtopia(void)
{
    // 1. deal with module
    
    void* pUtopiaModule = NULL;
//    UtopiaModuleCreate(MODULE_DVBT, 8, &pUtopiaModule);   
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c]DVBCRegisterToUtopia\n"));

    UtopiaModuleCreate(MODULE_DVBC, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)DVBCOpen, (FUtopiaClose)DVBCClose, (FUtopiaIOctl)DVBCIoctl);
 
   // Utopia2K STR
#if defined(MSOS_TYPE_LINUX_KERNEL)
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBC_v2.c][DVBCRegisterToUtopia] KERNEL DVBCStr!\n"));
    UtopiaModuleSetupSTRFunctionPtr(pUtopiaModule,(FUtopiaSTR)DVBCStr);
#endif

	// 20160107 oga. add share memory to control DC ON/OFF variable

    // 2. deal with resource
    void* psResource = NULL;
    // start func to add res, call once will create 2 access in resource. Also can declare BDMA_POOL_ID_BDMA1 for another channel depend on driver owner.
	UtopiaModuleAddResourceStart(pUtopiaModule, DVBC_POOL_ID_DMD0);
    // resource can alloc private for internal use, ex, BDMA_RESOURCE_PRIVATE
    UtopiaResourceCreate("dvbc0", sizeof(DVBC_RESOURCE_PRIVATE), &psResource);
    // func to reg res
    UtopiaResourceRegister(pUtopiaModule, psResource, DVBC_POOL_ID_DMD0);
    // end function to add res
	UtopiaModuleAddResourceEnd(pUtopiaModule, DVBC_POOL_ID_DMD0);


}


