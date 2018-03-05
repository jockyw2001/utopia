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
/// file    drvDMD_INTERN_DVBS.c
/// @brief  DVBS Driver Interface
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
#include "drvDMD_INTERN_DVBS_v2.h"
#include "halDMD_INTERN_DVBS.h"
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
        if (_s32DMD_DVBS_Mutex == -1) return FALSE; \
        if (_u8DMD_DVBS_DbgLevel == DMD_DVBS_DBGLV_DEBUG) printf("%s lock mutex\n", __FUNCTION__);\
        MsOS_ObtainMutex(_s32DMD_DVBS_Mutex, MSOS_WAIT_FOREVER);\
        }while(0)

#define DMD_UNLOCK()      \
    do{                         \
        MsOS_ReleaseMutex(_s32DMD_DVBS_Mutex);\
        if (_u8DMD_DVBS_DbgLevel == DMD_DVBS_DBGLV_DEBUG) printf("%s unlock mutex\n", __FUNCTION__); \
        }while(0)

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
MS_BOOL bIsDVBS = FALSE;       // Usage for STR 
#if 1
static MSIF_Version _drv_dmd_dvbs_intern_version = {
    .MW = { DMD_DVBS_INTERN_VER, },
};
#else
static MSIF_Version _drv_dmd_dvbs_intern_version;
#endif



static const char pDMD_DVBS_MutexString[]={"Mutex DMD DVBS"};
static DMD_DVBS_InitData_Transform _sDMD_DVBS_InitData;
DMD_DVBS_DbgLv _u8DMD_DVBS_DbgLevel=DMD_DVBS_DBGLV_NONE;
static MS_S32 _s32DMD_DVBS_Mutex=-1;
DMD_DVBS_Info sDMD_DVBS_Info;
static MS_U16 u16DMD_DVBS_AutoSymbol_Timeout = 10000, u16DMD_DVBS_FixSymbol_AutoQam_Timeout=2000, u16DMD_DVBS_FixSymbol_FixQam_Timeout=1000;
//static MS_U32 u32DMD_DVBS_PrevScanTime=0;
static MS_U32 u32DMD_DVBS_ScanCount=0;

#ifdef UFO_DEMOD_DVBS_SUPPORT_DMD_INT
fpIntCallBack fpCB = NULL;
#define DMD_EVT_MASK          0x0F
#define DMD_EVT_MCU_INT       0x01
#define DMD_EVT_TASK_STACK_SIZE 4096
static MS_U8  u8StackBuffer[DMD_EVT_TASK_STACK_SIZE];
static MS_S32 _s32DmdEventTaskId = -1;
static MS_S32 _s32DmdEventId = -1;
static MS_U32 u32Events;
extern fpIntCallBack fpCB;
#endif

//Debug Only-

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
//#define MS_DEBUG 1
#ifdef MS_DEBUG
#define DMD_DBG(x)          (x)
#else
#define DMD_DBG(x)          //(x)
#endif
#ifdef MS_DEBUG
#define DBG_INTERN_DVBS(x) x
#define DBG_GET_SIGNAL_DVBS(x)   x
#define DBG_INTERN_DVBS_TIME(x)  x
#define DBG_INTERN_DVBS_LOCK(x)  x
#define INTERN_DVBS_INTERNAL_DEBUG  1
#else
#define DBG_INTERN_DVBS(x)          //x
#define DBG_GET_SIGNAL_DVBS(x)      //x
#define DBG_INTERN_DVBS_TIME(x)     //x
#define DBG_INTERN_DVBS_LOCK(x)     //x
#define INTERN_DVBS_INTERNAL_DEBUG  0
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
#define DVBS2FEC_REG_BASE            0x3D00  
#define DVBS2OPPRO_REG_BASE         0x3E00 
#define INTERN_DVBS_DEMOD_WAIT_TIMEOUT      (6000)
#endif
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

typedef MS_BOOL                   (*IOCTL_DVBS_Init)(DMD_DVBS_InitData_Transform *pDMD_DVBS_InitData, MS_U32 u32InitDataLen);
typedef MS_BOOL			    (*IOCTL_DVBS_Exit)(void);
typedef MS_BOOL                   (*IOCTL_DVBS_SetDbgLevel)(DMD_DVBS_DbgLv u8DbgLevel);
typedef const DMD_DVBS_Info* (*IOCTL_DVBS_GetInfo)(void);
typedef MS_BOOL 		    (*IOCTL_DVBS_GetLibVer)(const MSIF_Version **ppVersion);
typedef MS_BOOL			     (*IOCTL_DVBS_GetFWVer)(MS_U16 *ver);
typedef MS_BOOL                   (*IOCTL_DVBS_GetDSPReg)(MS_U16 u16Addr, MS_U8 *pu8Data);
typedef MS_BOOL			     (*IOCTL_DVBS_SetDSPReg)(MS_U16 u16Addr, MS_U8 pu8Data);
typedef MS_BOOL			     (*IOCTL_DVBS_GetReg)(MS_U16 u16Addr, MS_U8 *pu8Data);
typedef MS_BOOL			     (*IOCTL_DVBS_SetReg)(MS_U16 u16Addr, MS_U8 u8Data);
typedef MS_BOOL			     (*IOCTL_DVBS_SetSerialControl)(MS_BOOL bEnable);
typedef MS_BOOL                   (*IOCTL_DVBS_BlindScan_Config_symbol_rate_list)(MS_U32 u32SymbolRate, DMD_DVBS_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num);
typedef MS_BOOL                   (*IOCTL_DVBS_SetConfig_symbol_rate_list)(MS_U32 u32SymbolRate, DMD_DVBS_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num);
typedef MS_BOOL			     (*IOCTL_DVBS_SetActive)(MS_BOOL bEnable);
typedef MS_BOOL	          (*IOCTL_DVBS_GetLockWithRFPower)(DMD_DVBS_GETLOCK_TYPE eType, DMD_DVBS_LOCK_STATUS *eLockStatus, MS_U32 fCurrRFPowerDbm, MS_U32 fNoChannelRFPowerDbm);
typedef MS_BOOL           (*IOCTL_DVBS_GetSignalStrengthWithRFPower)(MS_U16 fRFPowerDbm, DMD_DVBS_DEMOD_TYPE *pDemodType, MS_U8  *u8_DVBS2_CurrentCodeRateLocal,  MS_U8   *u8_DVBS2_CurrentConstellationLocal);
//typedef MS_BOOL			      (*IOCTL_DVBS_GetSignalQualityWithRFPower)(MS_U16 *u16Quality);

typedef MS_BOOL			      (*IOCTL_DVBS_ActiveDmdSwitch)(MS_U8 demod_no);

typedef MS_BOOL                    (*IOCTL_DVBS_GetSNR)(MS_U32 *u32NDA_SNR_A, MS_U32 *u32NDA_SNR_AB);

typedef MS_BOOL                    (*IOCTL_DVBS_GetPostViterbiBer)(MS_U32 *BitErr_reg, MS_U16 *BitErrPeriod_reg);
typedef MS_BOOL                    (*IOCTL_DVBS_GetIFAGC)(MS_U8 *ifagc_reg, MS_U8 *ifagc_reg_lsb, MS_U16 *ifagc_err);

typedef MS_BOOL                    (*IOCTL_DVBS_GetPacketErr)(MS_U16 *pktErr);
typedef MS_BOOL                    (*IOCTL_DVBS_GetCellID)(MS_U16 *u16CellID);

typedef MS_BOOL                    (*IOCTL_DVBS_GetStatus)(DMD_DVBS_MODULATION_TYPE *pQAMMode, MS_U32 *u32SymbolRate, MS_S16  *s16CFO);
//typedef MS_BOOL                     (*IOCTL_DVBS_GetStatus)(DMD_DVBS_MODULATION_TYPE *pQAMMode, MS_U16 *u16SymbolRate);
typedef MS_U32                       (*IOCTL_DVBS_SetPowerState)(EN_POWER_MODE u16PowerState);
typedef MS_BOOL                       (*IOCTL_DVBS_Get_FreqOffset)(MS_S16 *s16CFO);
typedef MS_BOOL (*IOCTL_DVBS_BlindScan_Start)(MS_U16 u16StartFreq, MS_U16 u16EndFreq);
typedef MS_BOOL (*IOCTL_DVBS_BlindScan_NextFreq)(MS_BOOL* bBlindScanEnd);
typedef MS_BOOL (*IOCTL_DVBS_BlindScan_Cancel)(void);
typedef MS_BOOL (*IOCTL_DVBS_BlindScan_End)(void);
typedef MS_BOOL (*IOCTL_DVBS_BlindScan_GetChannel)(MS_U16 u16ReadStart, MS_U16* u16TPNum, HAL_DEMOD_MS_FE_CARRIER_PARAM *pTable);
typedef MS_BOOL (*IOCTL_DVBS_BlindScan_GetCurrentFreq)(MS_U32 *u32CurrentFeq);
typedef MS_BOOL (*IOCTL_DVBS_BlindScan_WaitCurFreqFinished)(MS_U8* u8Progress,MS_U8 *u8FindNum, MS_U8 *substate_reg, MS_U32  *u32Data, MS_U16 *symbolrate_reg, MS_U16 *CFO_reg);
typedef MS_BOOL (*IOCTL_DVBS_BlindScan_GetTunerFreq)(MS_U16 *u16TunerCenterFreq, MS_U16 *u16TunerCutOffFreq);
typedef MS_BOOL (*IOCTL_DVBS_DiSEqC_Init)(void);
typedef MS_BOOL (*IOCTL_DVBS_DiSEqC_SetLNBOut)(MS_BOOL bLow);
typedef MS_BOOL (*IOCTL_DVBS_DiSEqC_GetLNBOut)(MS_BOOL* bLNBOutLow);
typedef MS_BOOL (*IOCTL_DVBS_DiSEqC_Set22kOnOff)(MS_BOOL b22kOn);
typedef MS_BOOL (*IOCTL_DVBS_DiSEqC_Get22kOnOff)(MS_BOOL* b22kOn);
typedef MS_BOOL (*IOCTL_DVBS_DiSEqC_SendCmd)(MS_U8* pCmd,MS_U8 u8CmdSize);
typedef MS_BOOL (*IOCTL_DVBS_DiSEqC_SetTxToneMode)(MS_BOOL bTxTone22kOff);
typedef MS_BOOL (*IOCTL_DVBS_DiSEqC_SetTone)(MS_BOOL bTone1);
typedef MS_BOOL (*IOCTL_DVBS_UnicableAGCCheckPower)(MS_BOOL pbAGCCheckPower);
typedef MS_BOOL (*IOCTL_DVBS_GetTsDivNum)(MS_U32 *u32SymbolRate, MS_U8* system_type_reg, MS_U8 *code_rate_idx, MS_U8 *fec_type_idx, MS_U8 *pilot_flag, MS_U32 *u32temp, MS_U8 *code_rate_reg);
typedef MS_BOOL (*IOCTL_DVBS_GetCurrentDemodCodeRate)(DMD_DVBS_CODE_RATE_TYPE *pCodeRate);
typedef MS_BOOL (*IOCTL_DVBS_GetTunrSignalLevel_PWR)(MS_U16 *u16Data);
#ifdef UFO_SUPPORT_VCM
//Terry add
typedef MS_BOOL (*IOCTL_DVBS_Set_IS_ID)(MS_U8 u8ISID);
typedef MS_BOOL (*IOCTL_DVBS_Set_Default_IS_ID)(MS_U8 *u8ISID, MS_U8 *u8ISID_table);
typedef MS_BOOL (*IOCTL_DVBS_Get_IS_ID_INFO)(MS_U8 *u8ISID, MS_U8 *u8ISID_table);
typedef MS_BOOL (*IOCTL_DVBS_VCM_INIT)(DMD_DVBS_VCM_OPT u8VCM_OPT, MS_U8 u8IS_ID, MS_U32 u32DMD_DVBS2_DJB_START_ADDR);
typedef MS_BOOL (*IOCTL_DVBS_VCM_CHECK)(void);
typedef MS_BOOL (*IOCTL_DVBS_VCM_ENABLED)(MS_U8 u8VCM_ENABLED);
typedef MS_BOOL (*IOCTL_DVBS_VCM_MODE)(DMD_DVBS_VCM_OPT u8VCM_OPT);
#endif

#ifdef UFO_DEMOD_DVBS_SUPPORT_DMD_INT
typedef MS_BOOL (*IOCTL_DVBS_INT_MODE)(fpIntCallBack fpCBReg);
#endif

#ifdef UFO_DEMOD_GetParam_NEW_FLOW
typedef MS_BOOL (*IOCTL_DVBS_GetParam)(PDVBS_GET_INFO pParam);
#endif

#ifdef UFO_DEMOD_GET_AGC_INFO
typedef MS_BOOL (*IOCTL_DVBS_GetAGC_INFO)(MS_U8 u8dbg_mode, MS_U16 *pu16Data);
#endif

typedef struct DVBS_INSTANT_PRIVATE
{
	IOCTL_DVBS_Init					fpDVBS_Init;
	IOCTL_DVBS_Exit					fpDVBS_Exit;
	IOCTL_DVBS_SetDbgLevel			fpDVBS_SetDbgLevel;
	IOCTL_DVBS_GetInfo				fpDVBS_GetInfo;
	IOCTL_DVBS_GetLibVer			fpDVBS_GetLibVer;
	IOCTL_DVBS_GetFWVer			fpDVBS_GetFWVer;
	IOCTL_DVBS_GetDSPReg			fpDVBS_GetDSPReg;
	IOCTL_DVBS_SetDSPReg			fpDVBS_SetDSPReg;
	IOCTL_DVBS_GetReg				fpDVBS_GetReg;
	IOCTL_DVBS_SetReg				fpDVBS_SetReg;
	IOCTL_DVBS_SetSerialControl           fpDVBSSetSerialControl;
	IOCTL_DVBS_BlindScan_Config_symbol_rate_list fpDVBS_BlindScan_Config_symbol_rate_list;
	IOCTL_DVBS_SetConfig_symbol_rate_list fpDVBS_SetConfig_symbol_rate_list;
	IOCTL_DVBS_SetActive				fpDVBS_SetActive;
	IOCTL_DVBS_GetLockWithRFPower	fpDVBS_GetLockWithRFPower;
	IOCTL_DVBS_GetSignalStrengthWithRFPower  fpDVBS_GetSignalStrengthWithRFPower;
	//IOCTL_DVBS_GetSignalQualityWithRFPower    fpDVBS_GetSignalQualityWithRFPower;
	IOCTL_DVBS_ActiveDmdSwitch                      fpDVBS_ActiveDmdSwitch;
	IOCTL_DVBS_GetSNR					     fpDVBS_GetSNR;
	IOCTL_DVBS_GetPostViterbiBer			     fpDVBS_GetPostViterbiBer;
	IOCTL_DVBS_GetPacketErr				     fpDVBS_GetPacketErr;
	IOCTL_DVBS_GetCellID				     fpDVBS_GetCellID;
	IOCTL_DVBS_GetStatus				     fpDVBS_GetStatus;
	IOCTL_DVBS_SetPowerState			     fpDVBS_SetPowerState;	
	IOCTL_DVBS_Get_FreqOffset                         fpDVBS_Get_FreqOffset;
	IOCTL_DVBS_BlindScan_Start                        fpDVBS_BlindScan_Start;
	IOCTL_DVBS_BlindScan_NextFreq                  fpDVBS_BlindScan_NextFreq;
	IOCTL_DVBS_BlindScan_Cancel                     fpDVBS_BlindScan_Cancel;
       IOCTL_DVBS_BlindScan_End                          fpDVBS_BlindScan_End;
       IOCTL_DVBS_BlindScan_GetChannel              fpDVBS_BlindScan_GetChannel;
       IOCTL_DVBS_BlindScan_GetCurrentFreq        fpDVBS_BlindScan_GetCurrentFreq;
       IOCTL_DVBS_BlindScan_WaitCurFreqFinished fpDVBS_BlindScan_WaitCurFreqFinished;
       IOCTL_DVBS_BlindScan_GetTunerFreq           fpDVBS_BlindScan_GetTunerFreq;
       IOCTL_DVBS_DiSEqC_Init                             fpDVBS_DiSEqC_Init;
       IOCTL_DVBS_DiSEqC_SetLNBOut                  fpDVBS_DiSEqC_SetLNBOut;
       IOCTL_DVBS_DiSEqC_GetLNBOut                  fpDVBS_DiSEqC_GetLNBOut;
       IOCTL_DVBS_DiSEqC_Set22kOnOff               fpDVBS_DiSEqC_Set22kOnOff;
       IOCTL_DVBS_DiSEqC_Get22kOnOff               fpDVBS_DiSEqC_Get22kOnOff;
       IOCTL_DVBS_DiSEqC_SendCmd                   fpDVBS_DiSEqC_SendCmd;
       IOCTL_DVBS_DiSEqC_SetTxToneMode          fpDVBS_DiSEqC_SetTxToneMode;
       IOCTL_DVBS_DiSEqC_SetTone                     fpDVBS_DiSEqC_SetTone;
       IOCTL_DVBS_UnicableAGCCheckPower         fpDVBS_UnicableAGCCheckPower;

	//Leo add
	IOCTL_DVBS_GetTsDivNum	                     fpDVBS_GetTsDivNum;
	IOCTL_DVBS_GetIFAGC    fpDVBS_GetIFAGC;
	IOCTL_DVBS_GetCurrentDemodCodeRate fpDVBS_GetCurrentDemodCodeRate;
	IOCTL_DVBS_GetTunrSignalLevel_PWR fpDVBS_GetTunrSignalLevel_PWR;
#ifdef UFO_SUPPORT_VCM
        //Terry add
        IOCTL_DVBS_Set_IS_ID fpDVBS_Set_IS_ID;    
        IOCTL_DVBS_Set_Default_IS_ID fpDVBS_Set_Default_IS_ID;    
        IOCTL_DVBS_Get_IS_ID_INFO fpDVBS_Get_IS_ID_INFO;
        IOCTL_DVBS_VCM_INIT fpDVBS_VCM_INIT;
        IOCTL_DVBS_VCM_CHECK fpDVBS_VCM_CHECK;
        IOCTL_DVBS_VCM_ENABLED fpDVBS_VCM_ENABLED;        
        IOCTL_DVBS_VCM_MODE fpDVBS_VCM_MODE;
#endif

#ifdef UFO_DEMOD_DVBS_SUPPORT_DMD_INT
        IOCTL_DVBS_INT_MODE fpDVBS_INT_MODE;
#endif

#ifdef UFO_DEMOD_GetParam_NEW_FLOW
        IOCTL_DVBS_GetParam fpDVBS_GetParam;
#endif

#ifdef UFO_DEMOD_GET_AGC_INFO
        IOCTL_DVBS_GetAGC_INFO fpDVBS_GetAGC_INFO;
#endif
} DVBS_INSTANT_PRIVATE;


#ifdef UFO_DEMOD_DVBS_SUPPORT_DMD_INT
static void _mdrv_dmd_dvbs_event_task(MS_U32 argc, void *argv)
{
   MS_U8 u8IntType;
  do
  {
     MsOS_WaitEvent(_s32DmdEventId, DMD_EVT_MCU_INT,&u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);
     INTERN_DVBS_Demod_Interrupt_Monitor(&u8IntType);
     //if(_sDMD_DVBS_InitData.fpCB != NULL)

     if(u8IntType == 1)
        MDrv_DMD_DVBS2_TS_DivNum_Calculation();

     if(fpCB!=NULL)
     {
       //_sDMD_DVBS_InitData.fpCB(u8IntType);
       fpCB(u8IntType);
     }     
  }while(1);
}

static void _mdrv_dmd_dvbs_cb(InterruptNum irq)
{
    MsOS_ClearEvent(_s32DmdEventId, DMD_EVT_MASK);
    MsOS_SetEvent(_s32DmdEventId, DMD_EVT_MCU_INT);
    MsOS_EnableInterrupt(E_INT_FIQ_DMDMCU2HK);
}
#endif


MS_U8		 DVBS_TS_PHASE_EN =0;
MS_U8		 DVBS_TS_PHASE_NUM = 0; 
#ifdef MSOS_TYPE_LINUX_KERNEL
#if 1
static float _LogApproxTableX[80] =
{ 1.00, 1.30, 1.69, 2.20, 2.86, 3.71, 4.83, 6.27, 8.16, 10.60, 13.79,
  17.92, 23.30, 30.29, 39.37, 51.19, 66.54, 86.50, 112.46, 146.19,
  190.05, 247.06, 321.18, 417.54, 542.80, 705.64, 917.33, 1192.53,
  1550.29, 2015.38, 2620.00, 3405.99, 4427.79, 5756.13, 7482.97,
  9727.86, 12646.22, 16440.08, 21372.11, 27783.74, 36118.86,
  46954.52, 61040.88, 79353.15, 103159.09, 134106.82, 174338.86,
  226640.52, 294632.68, 383022.48, 497929.22, 647307.99, 841500.39, 1093950.50,
  1422135.65, 1848776.35, 2403409.25, 3124432.03, 4061761.64, 5280290.13,
  6864377.17, 8923690.32, 11600797.42, 15081036.65, 19605347.64, 25486951.94,
  33133037.52, 43072948.77, 55994833.40, 72793283.42, 94631268.45,
  123020648.99, 159926843.68, 207904896.79, 270276365.82, 351359275.57,
  456767058.24, 593797175.72, 771936328.43, 1003517226.96
};

static float _LogApproxTableY[80] =
{ 0.00, 0.11, 0.23, 0.34, 0.46, 0.57, 0.68, 0.80, 0.91, 1.03, 1.14, 1.25,
  1.37, 1.48, 1.60, 1.71, 1.82, 1.94, 2.05, 2.16, 2.28, 2.39, 2.51, 2.62,
  2.73, 2.85, 2.96, 3.08, 3.19, 3.30, 3.42, 3.53, 3.65, 3.76, 3.87, 3.99,
  4.10, 4.22, 4.33, 4.44, 4.56, 4.67, 4.79, 4.90, 5.01, 5.13, 5.24, 5.36,
  5.47, 5.58, 5.70, 5.81, 5.93, 6.04, 6.15, 6.27, 6.04, 6.15, 6.27, 6.38,
  6.49, 6.61, 6.72, 6.84, 6.95, 7.06, 7.18, 7.29, 7.41, 7.52, 7.63, 7.75,
  7.86, 7.98, 8.09, 8.20, 8.32, 8.43, 8.55, 8.66
};

static float Log10Approx(float flt_x)
{
    MS_U8  indx = 0;

    do {
        if (flt_x < _LogApproxTableX[indx])
            break;
        indx++;
    }while (indx < 79);   //stop at indx = 80

    return _LogApproxTableY[indx];
}
#else
static float Log10Approx(float flt_x)
{
    MS_U32       u32_temp = 1;
    MS_U8        indx = 0;

    do {
        u32_temp = u32_temp << 1;
        if (flt_x < (float)u32_temp)
            break;
    }while (++indx < 32);

    // 10*log10(X) ~= 0.3*N, when X ~= 2^N
    return (float)0.3 * indx;
}
#endif
#endif


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL DMD_DVBS_Init(DMD_DVBS_InitData_Transform *pDMD_DVBS_InitData, MS_U32 u32InitDataLen)
{
    char pDMD_DVBS_MutexString[16];
    MS_U32 u32IfFrequency = 0L, u32FsFrequency = 96000L;
    MS_U8 u8IQSwap=0, u8ADCIQMode = 0, u8PadSel = 0, bPGAEnable = 0, u8PGAGain = 5;
    MS_BOOL bRFAGCTristateEnable = 1;
    MS_BOOL bIFAGCTristateEnable = 0;
    bIsDVBS = TRUE;

    if (_s32DMD_DVBS_Mutex != -1)
    {
        DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBS_Init more than once\n"));
        return FALSE;
    }

    if (NULL == strncpy(pDMD_DVBS_MutexString,"Mutex DMD DVBS",16))
    {
        DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBS_Init strcpy Fail\n"));
        return FALSE;
    }
    _s32DMD_DVBS_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, pDMD_DVBS_MutexString, MSOS_PROCESS_SHARED);
    if ( _s32DMD_DVBS_Mutex == -1)
    {
        DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBS_Init Create Mutex Fail\n"));
        return FALSE;
    }

    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBS_Init\n");
    }
    #endif

    if ( sizeof(_sDMD_DVBS_InitData) == u32InitDataLen)
    {
        memcpy(&_sDMD_DVBS_InitData, pDMD_DVBS_InitData, u32InitDataLen);
    }
    else
    {
        DMD_DBG(ULOGD("DEMOD","MDrv_DMD_DVBS_Init input data structure incorrect\n"));
        return FALSE;
    }

    if (_sDMD_DVBS_InitData.u8SarChannel != 0xFF)
    {
        //MDrv_SAR_Adc_Config(_sDMD_DVBS_InitData.u8SarChannel, TRUE);
    }
    DMD_LOCK();
    //_u8DMD_DVBS_DbgLevel=DMD_DVBS_DBGLV_DEBUG;
    MDrv_SYS_DMD_VD_MBX_SetType(E_DMD_VD_MBX_TYPE_DVBS);
    HAL_DMD_RegInit();

    if (_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt != NULL)
    {
        if (_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[0]>=2)
        {
            bRFAGCTristateEnable = (_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[3] & (BIT_(0))) ? TRUE : FALSE; // RFAGC tristate control
            bIFAGCTristateEnable = (_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[3] & (BIT_(4))) ? TRUE : FALSE; // IFAGC tristate control
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

    if (_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt != NULL)
    {
        if (_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[0]>=3)
        {
            u32IfFrequency = _sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[4]; // IF frequency
            u32IfFrequency =  (u32IfFrequency<<8)+_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[5]; // IF frequency
            u32IfFrequency =  (u32IfFrequency<<8)+_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[6]; // IF frequency
            u32IfFrequency =  (u32IfFrequency<<8)+_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[7]; // IF frequency
            u32FsFrequency = _sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[8]; // FS frequency
            u32FsFrequency =  (u32FsFrequency<<8)+_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[9]; // FS frequency
            u32FsFrequency =  (u32FsFrequency<<8)+_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[10]; // FS frequency
            u32FsFrequency =  (u32FsFrequency<<8)+_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[11]; // FS frequency
            u8IQSwap = _sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[12]; // IQ Swap
            u8ADCIQMode = _sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[13]; // u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
            u8PadSel = _sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[14]; // u8PadSel : 0=Normal, 1=analog pad
            bPGAEnable = _sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[15]; // bPGAEnable : 0=disable, 1=enable
            u8PGAGain = _sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[16]; // u8PGAGain : default 5
        }
        else
        {

        }
    }
    else
    {

    }

    u16DMD_DVBS_AutoSymbol_Timeout = 10000;
    u16DMD_DVBS_FixSymbol_AutoQam_Timeout=2000;
    u16DMD_DVBS_FixSymbol_FixQam_Timeout=2000;
    if (_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt != NULL)
    {
        if (_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[0]>=4)
        {
            u16DMD_DVBS_AutoSymbol_Timeout = _sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[17];
            u16DMD_DVBS_AutoSymbol_Timeout =  (u16DMD_DVBS_AutoSymbol_Timeout<<8)+_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[18];
            if (u16DMD_DVBS_AutoSymbol_Timeout < 5000) u16DMD_DVBS_AutoSymbol_Timeout=5000;
            //printf("u16DMD_DVBS_AutoSymbol_Timeout %d\n",u16DMD_DVBS_AutoSymbol_Timeout);

            u16DMD_DVBS_FixSymbol_AutoQam_Timeout = _sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[19];
            u16DMD_DVBS_FixSymbol_AutoQam_Timeout =  (u16DMD_DVBS_FixSymbol_AutoQam_Timeout<<8)+_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[20];
            if (u16DMD_DVBS_FixSymbol_AutoQam_Timeout < 2000) u16DMD_DVBS_FixSymbol_AutoQam_Timeout=2000;
            //printf("u16DMD_DVBS_FixSymbol_AutoQam_Timeout %d\n",u16DMD_DVBS_FixSymbol_AutoQam_Timeout);

            u16DMD_DVBS_FixSymbol_FixQam_Timeout = _sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[21];
            u16DMD_DVBS_FixSymbol_FixQam_Timeout =  (u16DMD_DVBS_FixSymbol_FixQam_Timeout<<8)+_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[22];
            if (u16DMD_DVBS_FixSymbol_FixQam_Timeout < 1000) u16DMD_DVBS_FixSymbol_FixQam_Timeout=1000;
            //printf("u16DMD_DVBS_FixSymbol_FixQam_Timeout %d\n",u16DMD_DVBS_FixSymbol_FixQam_Timeout);
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

    if (_sDMD_DVBS_InitData.u8DMD_DVBS_DSPRegInitExt != NULL)
    {
        if (_sDMD_DVBS_InitData.u8DMD_DVBS_DSPRegInitExt[0]>=1)
        {
            INTERN_DVBS_Power_On_Initialization(bRFAGCTristateEnable, u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain, _sDMD_DVBS_InitData.u8DMD_DVBS_DSPRegInitExt, _sDMD_DVBS_InitData.u8DMD_DVBS_DSPRegInitSize);
        }
        else
        {
            ULOGD("DEMOD","u8DMD_DVBS_DSPRegInitExt Error\n");
        }
    }
    else
    {
        INTERN_DVBS_Power_On_Initialization(bRFAGCTristateEnable, u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain,  NULL, 0);
    }
#ifdef UFO_DEMOD_DVBS_SUPPORT_DMD_INT
    if (_s32DmdEventId < 0)
    {
        _s32DmdEventId = MsOS_CreateEventGroup("DMD_INTERN_DVBS_Event");
        if (_s32DmdEventId > 0)
            ULOGD("DEMOD","[%s][%d] Event create ok\n",__FUNCTION__,__LINE__);
        else
            ULOGD("DEMOD","[%s][%d] create failed \n",__FUNCTION__,__LINE__);
    }

    MsOS_AttachInterrupt(E_INT_FIQ_DMDMCU2HK, _mdrv_dmd_dvbs_cb);
    MsOS_DisableInterrupt(E_INT_FIQ_DMDMCU2HK);

    if(_s32DmdEventTaskId < 0)
    {
       _s32DmdEventTaskId = MsOS_CreateTask(_mdrv_dmd_dvbs_event_task,
                                            0,
                                            E_TASK_PRI_HIGHEST,
                                            TRUE,
                                            u8StackBuffer,
                                            DMD_EVT_TASK_STACK_SIZE,
                                            "DMD_INTERN_DVBS_EVT_TASK");
        if (_s32DmdEventTaskId > 0)
            ULOGD("DEMOD","[%s][%d] Event task create ok\n",__FUNCTION__,__LINE__);
        else
            ULOGD("DEMOD","[%s][%d] create task failed \n",__FUNCTION__,__LINE__);
    }
#endif    
    
    INTERN_DVBS_Version(&sDMD_DVBS_Info.u16Version);
    
    DMD_UNLOCK();
    #ifdef MS_DEBUG
    ULOGD("DEMOD","firmware version: %x\n",sDMD_DVBS_Info.u16Version);
    #endif
       
    return TRUE;
}

MS_BOOL DMD_DVBS_Exit(void)
{
    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBS_Exit\n");
    }
    #endif
    bIsDVBS = FALSE;
    DMD_LOCK();
    INTERN_DVBS_Exit();
    DMD_UNLOCK();
    MsOS_DeleteMutex(_s32DMD_DVBS_Mutex);
    _s32DMD_DVBS_Mutex= -1;
    return TRUE;
}

MS_BOOL DMD_DVBS_SetDbgLevel(DMD_DVBS_DbgLv u8DbgLevel)
{
    DMD_LOCK();
    _u8DMD_DVBS_DbgLevel = u8DbgLevel;
    DMD_UNLOCK();
    return TRUE;
}

const DMD_DVBS_Info* DMD_DVBS_GetInfo(void)
{
    MS_U8 state = 0;

    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBS_GetInfo\n");
    }
    #endif
//for DBG
#if 1
	MDrv_SYS_DMD_VD_MBX_ReadReg(FEC_REG_BASE + 0xE0, &state);
	ULOGD("DEMOD"," @INTERN_DVBS_GetLock FSM 0x%x\n",state);
        INTERN_DVBS_Show_AGC_Info();
	INTERN_DVBS_info();
#endif

    return (const DMD_DVBS_Info*)&sDMD_DVBS_Info;
}

MS_BOOL DMD_DVBS_GetLibVer(const MSIF_Version **ppVersion)
{
    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBS_GetLibVer\n");
    }
    #endif
    DMD_LOCK();
    if (!ppVersion)
    {
        return FALSE;
    }

    *ppVersion = &_drv_dmd_dvbs_intern_version;
    DMD_UNLOCK();
    return TRUE;
}

MS_BOOL DMD_DVBS_GetFWVer(MS_U16 *ver)
{

    MS_BOOL bRet;

    DMD_LOCK();

    bRet = INTERN_DVBS_Version(ver);
    //printf("MDrv_DMD_DVBS_GetFWVer %x\n", *ver);
    DMD_UNLOCK();

    return bRet;

}

MS_BOOL DMD_DVBS_GetDSPReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
#ifdef CHIP_K1
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=MDrv_SYS_DMD_VD_MBX_ReadDSPReg(u16Addr, pu8Data);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBS_GetDSPReg %x %x\n", u16Addr, *pu8Data);
    }
    #endif

    return bRet;
#else
  	ULOGD("DEMOD","Not Support function: MDrv_DMD_DVBS_GetDSPReg\n");
	return FALSE;
#endif
}

MS_BOOL DMD_DVBS_SetDSPReg(MS_U16 u16Addr, MS_U8 pu8Data)
{
#ifdef CHIP_K1
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=MDrv_SYS_DMD_VD_MBX_WriteDSPReg(u16Addr, pu8Data);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBS_SetDSPReg %x %x\n", u16Addr, pu8Data);
    }
    #endif

    return bRet;
#else
  	ULOGD("DEMOD","Not Support function: MDrv_DMD_DVBS_SetDSPReg\n");
	return FALSE;
#endif
}


MS_BOOL DMD_DVBS_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=MDrv_SYS_DMD_VD_MBX_ReadReg(u16Addr, pu8Data);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBS_GetReg %x %x\n", u16Addr, *pu8Data);
    }
    #endif
    return bRet;
}

MS_BOOL DMD_DVBS_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet;

    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBS_SetReg %x %x\n", u16Addr, u8Data);
    }
    #endif
    DMD_LOCK();
    bRet=MDrv_SYS_DMD_VD_MBX_WriteReg(u16Addr, u8Data);
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL DMD_DVBS_SetSerialControl(MS_BOOL bEnable)
{
    MS_BOOL bRet;
    MS_U8 u8TSClk;

    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBS_SetSerialControl %d\n", bEnable);
    }
    #endif

    DMD_LOCK();
    if (_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt != NULL)
    {
        if (_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[0]>=1)
        {
            u8TSClk = _sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[2];
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
    bRet=INTERN_DVBS_Serial_Control(bEnable, u8TSClk);
    DMD_UNLOCK();
    return bRet;
}

#ifdef UFO_DEMOD_BLINDSCAN_NEW_FLOW
MS_BOOL DMD_DVBS_BlindScan_Config_symbol_rate_list(MS_U32 u32SymbolRate, DMD_DVBS_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num)
{
    MS_BOOL bRet;
    MS_U8 u8TSClk;
    #ifdef MSOS_TYPE_LINUX_KERNEL
    MS_U32          u32CurrentSR;
    #endif
    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBS_BlindScanConfig %ld %d %ld %d %d\n", u32SymbolRate, eQamMode, u32IFFreq, bSpecInv, bSerialTS);
    }
    #endif

    DMD_LOCK();
    if (_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt != NULL)
    {
        if (_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[0]>=1)
        {
            u8TSClk = _sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[2];
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
    bRet=INTERN_DVBS_BlindScan_Config(u32SymbolRate, eQamMode, u32IFFreq, bSpecInv, bSerialTS, u8TSClk, pu16_symbol_rate_list,u8_symbol_rate_list_num);
    sDMD_DVBS_Info.u32SymbolRate = u32SymbolRate;
    sDMD_DVBS_Info.eQamMode = eQamMode;
    sDMD_DVBS_Info.u32IFFreq = u32IFFreq;
    sDMD_DVBS_Info.bSpecInv = bSpecInv;
    sDMD_DVBS_Info.bSerialTS = bSerialTS;
    DMD_UNLOCK();
    return bRet;
}
#endif

MS_BOOL DMD_DVBS_SetConfig_symbol_rate_list(MS_U32 u32SymbolRate, DMD_DVBS_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num)
{
    MS_BOOL bRet;
    MS_U8 u8TSClk;
    #ifdef MSOS_TYPE_LINUX_KERNEL
    MS_U32          u32CurrentSR;
    #endif	
    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBS_SetConfig %ld %d %ld %d %d\n", u32SymbolRate, eQamMode, u32IFFreq, bSpecInv, bSerialTS);
    }
    #endif

    DMD_LOCK();
    if (_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt != NULL)
    {
        if (_sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[0]>=1)
        {
            u8TSClk = _sDMD_DVBS_InitData.u8DMD_DVBS_InitExt[2];
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
    bRet=INTERN_DVBS_Config(u32SymbolRate, eQamMode, u32IFFreq, bSpecInv, bSerialTS, u8TSClk, pu16_symbol_rate_list,u8_symbol_rate_list_num);
    sDMD_DVBS_Info.u32SymbolRate = u32SymbolRate;
    sDMD_DVBS_Info.eQamMode = eQamMode;
    sDMD_DVBS_Info.u32IFFreq = u32IFFreq;
    sDMD_DVBS_Info.bSpecInv = bSpecInv;
    sDMD_DVBS_Info.bSerialTS = bSerialTS;
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL DMD_DVBS_SetActive(MS_BOOL bEnable)
{
    MS_BOOL bRet;

    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBS_SetActive %d\n", bEnable);
    }
    #endif

    DMD_LOCK();
    bRet=INTERN_DVBS_Active(bEnable);
    sDMD_DVBS_Info.u32ChkScanTimeStart = MsOS_GetSystemTime();
    u32DMD_DVBS_ScanCount=0;
    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        ULOGD("DEMOD","%s %ld\n", __FUNCTION__, sDMD_DVBS_Info.u32ChkScanTimeStart);
    }
    #endif
#ifdef UFO_DEMOD_DVBS_SUPPORT_DMD_INT
    MsOS_EnableInterrupt(E_INT_FIQ_DMDMCU2HK);
#endif    
    DMD_UNLOCK();
    return bRet;
}

#ifdef UFO_DEMOD_GetParam_NEW_FLOW
MS_BOOL DMD_DVBS_GetParam(PDVBS_GET_INFO pParam)
{
   DMD_DVBS_CODE_RATE_TYPE coderate;
   MS_U8 rolloff;
   DMD_DVBS_MODULATION_TYPE modulationmode;
   DMD_DVBS_DEMOD_TYPE DemodType;
   HAL_DEMOD_EN_SAT_IQ_MODE SAT_IQ_MODE;
   
   MS_S16 freqoff;
   MS_BOOL bRet = TRUE;

    DMD_LOCK();
    bRet &= INTERN_DVBS_GetCurrentDemodCodeRate(&coderate);
    bRet &= INTERN_DVBS_GetRollOff(&rolloff);
    bRet &= INTERN_DVBS_GetCurrentModulationType(&modulationmode);
    bRet &= INTERN_DVBS_GetCurrentDemodType(&DemodType); //get the DemodType from hal
    bRet &= INTERN_DVBS_Get_FreqOffset(&freqoff);
    bRet &= INTERN_DVBS_Get_IQ_MODE(&SAT_IQ_MODE);
    DMD_UNLOCK();

    if( DemodType == DMD_SAT_DVBS)
        DemodType = HAL_DEMOD_SAT_DVBS;
    else
        DemodType = HAL_DEMOD_SAT_DVBS2;
    
    pParam->eCodeRate = coderate;
    pParam->eRollOff = rolloff;
    pParam->eConstellation = modulationmode;
    pParam->eDemodType = DemodType;
    pParam->eFreqoff = (float)freqoff;
    pParam->eIQ_Mode = SAT_IQ_MODE;

   return bRet;
}
#endif

#ifdef UFO_DEMOD_GET_AGC_INFO
MS_BOOL DMD_DVBS_GetAGCInfo(MS_U8 u8dbg_mode, MS_U16 *pu16Data)
{
    MS_BOOL bRet;    
    DMD_LOCK();
    bRet = INTERN_DVBS_AGC_Info(u8dbg_mode, pu16Data);
    DMD_UNLOCK();
    return bRet;
}
#endif

MS_BOOL DMD_DVBS_GetLockWithRFPower(DMD_DVBS_GETLOCK_TYPE eType, DMD_DVBS_LOCK_STATUS *eLockStatus, MS_U32 fCurrRFPowerDbm, MS_U32 fNoChannelRFPowerDbm)
{
    //MS_U32 u32CurrScanTime=0;
    MS_BOOL bRet=FALSE;
    DMD_LOCK();
    /*	
    if ( eType == DMD_DVBS_GETLOCK ) // for channel scan
    {
        if (INTERN_DVBS_GetLock(DMD_DVBS_GETLOCK, 200, -200, 0))
        {
            *eLockStatus = DMD_DVBS_LOCK;
        }
        else
        {
            MS_U32 u32Timeout = 7000;//(sDMD_DVBS_Info.u32SymbolRate) ? 5000 : 15000;

            u32CurrScanTime=MsOS_GetSystemTime();
            if (u32CurrScanTime - sDMD_DVBS_Info.u32ChkScanTimeStart < u32Timeout)
            {
                if (u32DMD_DVBS_ScanCount==0)
                {
                    u32DMD_DVBS_PrevScanTime=u32CurrScanTime;
                    u32DMD_DVBS_ScanCount++;
                }

                if (_sDMD_DVBS_InitData.u8SarChannel != 0xFF)
                {
                    //sDMD_DVBS_Info.u8SarValue=MDrv_SAR_Adc_GetValue(_sDMD_DVBS_InitData.u8SarChannel);
                }
                else
                {
                    sDMD_DVBS_Info.u8SarValue=0xFF;
                }

                bRet=HAL_DMD_GetRFLevel(&fCurrRFPowerDbm, fCurrRFPowerDbm, sDMD_DVBS_Info.u8SarValue,
                                                                            _sDMD_DVBS_InitData.pTuner_RfagcSsi, _sDMD_DVBS_InitData.u16Tuner_RfagcSsi_Size,
                                                                            _sDMD_DVBS_InitData.pTuner_IfagcSsi_HiRef, _sDMD_DVBS_InitData.u16Tuner_IfagcSsi_HiRef_Size,
                                                                            _sDMD_DVBS_InitData.pTuner_IfagcSsi_LoRef, _sDMD_DVBS_InitData.u16Tuner_IfagcSsi_LoRef_Size,
                                                                            _sDMD_DVBS_InitData.pTuner_IfagcErr_HiRef, _sDMD_DVBS_InitData.u16Tuner_IfagcErr_HiRef_Size,
                                                                            _sDMD_DVBS_InitData.pTuner_IfagcErr_LoRef, _sDMD_DVBS_InitData.u16Tuner_IfagcErr_LoRef_Size);

                if (INTERN_DVBS_GetLock(DMD_DVBS_GETLOCK_NO_CHANNEL, fCurrRFPowerDbm, fNoChannelRFPowerDbm, u32CurrScanTime-u32DMD_DVBS_PrevScanTime))
                {
                    #ifdef MS_DEBUG
                    ULOGD("DEMOD","%s %ld UNLOCK:NO_CHANNEL\n", __FUNCTION__, u32CurrScanTime-sDMD_DVBS_Info.u32ChkScanTimeStart);
                    #endif
                    *eLockStatus = DMD_DVBS_UNLOCK;
                }
#ifdef NEW_TR_MODULE
                else if ((INTERN_DVBS_GetLock(DMD_DVBS_GETLOCK_TR_EVER_LOCK, 200, -200, 0) == FALSE) && ((u32CurrScanTime - sDMD_DVBS_Info.u32ChkScanTimeStart) > 3000))
#else
                else if ((INTERN_DVBS_GetLock(DMD_DVBS_GETLOCK_TR_EVER_LOCK, 200, -200, 0) == FALSE) && ((u32CurrScanTime - sDMD_DVBS_Info.u32ChkScanTimeStart) > 5000))
#endif
                {
                    #ifdef MS_DEBUG
                    ULOGD("DEMOD","%s %ld UNLOCK:TR\n", __FUNCTION__, u32CurrScanTime-sDMD_DVBS_Info.u32ChkScanTimeStart);
                    #endif
                    *eLockStatus = DMD_DVBS_UNLOCK;
                }
                else
                {
                    *eLockStatus = DMD_DVBS_CHECKING;
                }
            }
            else
            {
                #ifdef MS_DEBUG
                ULOGD("DEMOD","%s %ld UNLOCK:TIMEOUT %ld\n", __FUNCTION__, MsOS_GetSystemTime(), u32Timeout);
                #endif
                *eLockStatus = DMD_DVBS_UNLOCK;
            }
            u32DMD_DVBS_PrevScanTime=u32CurrScanTime;
        }
        sDMD_DVBS_Info.eLockStatus = *eLockStatus;
        #ifdef MS_DEBUG
        if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
        {
            ULOGD("DEMOD","%s %ld %d\n", __FUNCTION__, MsOS_GetSystemTime(), sDMD_DVBS_Info.eLockStatus);
        }
        #endif
    }
    else
    {
    */
        if (INTERN_DVBS_GetLock(eType, 200, -200, 0) == TRUE)
        {
            *eLockStatus = DMD_DVBS_LOCK;
        }
        else
        {
            *eLockStatus = DMD_DVBS_UNLOCK;
        }
    //}
    sDMD_DVBS_Info.eLockStatus = *eLockStatus;
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBS_GetLock %x %x\n", eType, *eLockStatus);
    }
    #endif
    bRet=TRUE;
    return bRet;
}

MS_BOOL DMD_DVBS_GetSignalStrengthWithRFPower(MS_U16 fRFPowerDbm, DMD_DVBS_DEMOD_TYPE *pDemodType, MS_U8  *u8_DVBS2_CurrentCodeRateLocal,  MS_U8   *u8_DVBS2_CurrentConstellationLocal)
{
    MS_BOOL bRet;

    DMD_LOCK();
    /*	
    if (_sDMD_DVBS_InitData.u8SarChannel != 0xFF)
    {
        sDMD_DVBS_Info.u8SarValue=MDrv_SAR_Adc_GetValue(_sDMD_DVBS_InitData.u8SarChannel);
    }
    else
    {
        sDMD_DVBS_Info.u8SarValue=0xFF;
    }
    */
    sDMD_DVBS_Info.u8SarValue=0xFF;
    bRet=INTERN_DVBS_GetSignalStrength(fRFPowerDbm, pDemodType, u8_DVBS2_CurrentCodeRateLocal, u8_DVBS2_CurrentConstellationLocal);
    //sDMD_DVBS_Info.u16Strength=*u16Strength;
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        //printf("MDrv_DMD_DVBS_GetSignalStrength %d\n", *u16Strength);
    }
    #endif
    return bRet;
}

#if (0)
MS_BOOL DMD_DVBS_GetSignalQualityWithRFPower(MS_U16 *u16Quality, float fRFPowerDbm)
{
    MS_BOOL bRet;

    DMD_LOCK();
    if (_sDMD_DVBS_InitData.u8SarChannel != 0xFF)
    {
        sDMD_DVBS_Info.u8SarValue=MDrv_SAR_Adc_GetValue(_sDMD_DVBS_InitData.u8SarChannel);
    }
    else
    {
        sDMD_DVBS_Info.u8SarValue=0xFF;
    }
    #ifndef MSOS_TYPE_LINUX_KERNEL
    bRet=INTERN_DVBS_GetSignalQuality(u16Quality, (const DMD_DVBS_InitData *)(&_sDMD_DVBS_InitData), sDMD_DVBS_Info.u8SarValue, fRFPowerDbm);
    #else    
    {
	float       fber = 0.0;
    //float       log_ber;
    MS_BOOL     status = TRUE;
    float       f_snr = 0.0, ber_sqi = 0.0, cn_rel = 0.0;
    //MS_U8       u8Data =0;
    DMD_DVBS_CODE_RATE_TYPE       _u8_DVBS2_CurrentCodeRateLocal ;
    MS_U16     bchpkt_error,BCH_Eflag2_Window;
    //fRFPowerDbm = fRFPowerDbm;
    float snr_poly =0.0;
    float Fixed_SNR =0.0;
    double eFlag_PER=0.0;
    if (u8DemodLockFlag == 1)	
   // if (TRUE == INTERN_DVBS_GetLock(DMD_DVBS_GETLOCK, 200.0f, -200.0f, 0))
    {
        if(_bDemodType)  //S2
        {

           MDrv_DMD_DVBS_GetSNR(&f_snr);
           snr_poly = 0.005261367463671*pow(f_snr, 3)-0.116517828301214*pow(f_snr, 2)+0.744836970505452*pow(f_snr, 1)-0.86727609780167;
           Fixed_SNR = f_snr + snr_poly;

           if (Fixed_SNR < 17.0)
              Fixed_SNR = Fixed_SNR;
           else if ((Fixed_SNR < 20.0) && (Fixed_SNR >= 17.0))
              Fixed_SNR = Fixed_SNR - 0.8;
           else if ((Fixed_SNR < 22.5) && (Fixed_SNR >= 20.0))
              Fixed_SNR = Fixed_SNR - 2.0;
           else if ((Fixed_SNR < 27.0) && (Fixed_SNR >= 22.5))
              Fixed_SNR = Fixed_SNR - 3.0;
           else if ((Fixed_SNR < 29.0) && (Fixed_SNR >= 27.0))
              Fixed_SNR = Fixed_SNR - 3.5;
           else if (Fixed_SNR >= 29.0)
              Fixed_SNR = Fixed_SNR - 3.0;


           if (Fixed_SNR < 1.0)
              Fixed_SNR = 1.0;
           if (Fixed_SNR > 30.0)
              Fixed_SNR = 30.0;

            //BCH EFLAG2_Window,  window size 0x2000
            BCH_Eflag2_Window=0x2000;
            MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2FEC_REG_BASE + 0x25*2 + 1, (BCH_Eflag2_Window>>8));
            MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2FEC_REG_BASE + 0x25*2 , (BCH_Eflag2_Window&0xff));
            INTERN_DVBS_GetPacketErr(&bchpkt_error);
            eFlag_PER = (float)(bchpkt_error)/(float)(BCH_Eflag2_Window);
            if(eFlag_PER>0)
              fber = 0.089267531133002*pow(eFlag_PER, 2) + 0.019640560289510*eFlag_PER + 0.0000001;
            else
              fber = 0;

#ifdef MSOS_TYPE_LINUX
                    //log_ber = ( - 1) *log10f(1 / fber);
                    if (fber > 1.0E-1)
                        ber_sqi = (log10f(1.0f/fber))*20.0f + 8.0f;
                    else if(fber > 8.5E-7)
                        ber_sqi = (log10f(1.0f/fber))*20.0f - 30.0f;
                    else
                        ber_sqi = 100.0;
#else
                    //log_ber = ( - 1) *Log10Approx(1 / fber);
                    if (fber > 1.0E-1)
                        ber_sqi = (Log10Approx(1.0f/fber))*20.0f + 8.0f;
                    else if(fber > 8.5E-7)
                        ber_sqi = (Log10Approx(1.0f/fber))*20.0f - 30.0f;
                    else
                        ber_sqi = 100.0;

#endif

            *u16Quality = Fixed_SNR/30*ber_sqi;
            DBG_INTERN_DVBS(printf(" Fixed_SNR %f\n",Fixed_SNR));
            DBG_INTERN_DVBS(printf(" BCH_Eflag2_Window %d\n",BCH_Eflag2_Window));
            DBG_INTERN_DVBS(printf(" eFlag_PER [%f]\n fber [%8.3e]\n ber_sqi [%f]\n",eFlag_PER,fber,ber_sqi));
        }
        else  //S
        {
            if (MDrv_DMD_DVBS_GetPostViterbiBer(&fber) == FALSE)//ViterbiBer
            {
                DBG_INTERN_DVBS(printf("\nGetPostViterbiBer Fail!"));
                bRet= FALSE;
            }
            _fPostBer=fber;


            if (status==FALSE)
            {
                DBG_INTERN_DVBS(printf("MSB131X_DTV_GetSignalQuality GetPostViterbiBer Fail!\n"));
                bRet= FALSE;
            }
            float fDVBS_SQI_CNref[]=
            {   //0,    1,    2,    3,    4
                4.2,   5.9,  6,  6.9,  7.5,//QPSK
            };

            INTERN_DVBS_GetCurrentDemodCodeRate(&_u8_DVBS2_CurrentCodeRateLocal);
#if 0
#ifdef MSOS_TYPE_LINUX
            log_ber = ( - 1.0f) *log10f(1.0f / fber);           //BY modify
#else
            log_ber = ( - 1.0f) *Log10Approx(1.0f / fber);      //BY modify
#endif
            DBG_INTERN_DVBS(printf("\nLog(BER) = %f\n",log_ber));
#endif
            if (fber > 2.5E-2)
                ber_sqi = 0.0;
            else if(fber > 8.5E-7)
#ifdef MSOS_TYPE_LINUX
                ber_sqi = (log10f(1.0f/fber))*20.0f - 32.0f; //40.0f;
#else
                ber_sqi = (Log10Approx(1.0f/fber))*20.0f - 32.0f;//40.0f;
#endif
            else
                ber_sqi = 100.0;

            status &= MDrv_DMD_DVBS_GetSNR(&f_snr);
            DBG_GET_SIGNAL_DVBS(printf("INTERN_DVBS GetSNR = %d \n", (int)f_snr));

            cn_rel = f_snr - fDVBS_SQI_CNref[_u8_DVBS2_CurrentCodeRateLocal];

            DBG_INTERN_DVBS(printf(" fber = %f\n",fber));
            DBG_INTERN_DVBS(printf(" f_snr = %f\n",f_snr));
            DBG_INTERN_DVBS(printf(" cn_nordig_s1 = %f\n",fDVBS_SQI_CNref[_u8_DVBS2_CurrentCodeRateLocal]));
            DBG_INTERN_DVBS(printf(" cn_rel = %f\n",cn_rel));
            DBG_INTERN_DVBS(printf(" ber_sqi = %f\n",ber_sqi));

            if (cn_rel < -7.0f)
            {
                *u16Quality = 0;
            }
            else if (cn_rel < 3.0)
            {
                *u16Quality = (MS_U16)(ber_sqi*((cn_rel - 3.0)/10.0 + 1.0));
            }
            else
            {
                *u16Quality = (MS_U16)ber_sqi;
            }


        }
            //INTERN_DVBS_GetTunrSignalLevel_PWR();//For Debug.
            DBG_INTERN_DVBS(printf(">>>>>Signal Quility(SQI) = %d\n", *quality));
            bRet= TRUE;
    }
    else
    {
        *u16Quality = 0;
    }
     bRet= TRUE;
    	}
    #endif
    //printf("@@--%s--u16Quality = %d\n", __func__, *u16Quality);
    sDMD_DVBS_Info.u16Quality=*u16Quality;
    DMD_UNLOCK();
    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        printf("%s %d %d\n", __FUNCTION__, bRet, sDMD_DVBS_Info.u16Quality);
    }
    #endif

    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        printf("MDrv_DMD_DVBS_GetSignalQuality %d\n", *u16Quality);
    }
    #endif
    return bRet;
}
#endif
MS_BOOL DMD_DVBS_ActiveDmdSwitch(MS_U8 demod_no)
{
	ULOGD("DEMOD","Doesn't support DVBS_ActiveDmdSwitch function!!!\n");
	return false;
}

MS_BOOL DMD_DVBS_GetSNR(MS_U32 *u32NDA_SNR_A, MS_U32 *u32NDA_SNR_AB)
{
    MS_BOOL bRet;
    DMD_LOCK();
    bRet=INTERN_DVBS_GetSNR(u32NDA_SNR_A, u32NDA_SNR_AB);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL DMD_DVBS_GetPostViterbiBer(MS_U32 *BitErr_reg, MS_U16 *BitErrPeriod_reg)
{ 
    MS_BOOL bRet;
    DMD_LOCK();
    bRet=INTERN_DVBS_GetPostViterbiBer(BitErr_reg, BitErrPeriod_reg);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL DMD_DVBS_GetIFAGC(MS_U8 *ifagc_reg, MS_U8 *ifagc_reg_lsb, MS_U16 *ifagc_err)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBS_GetIFAGC(ifagc_reg, ifagc_reg_lsb, ifagc_err);
    DMD_UNLOCK();

	  return bRet;
}

MS_BOOL DMD_DVBS_GetPacketErr(MS_U16 *pktErr)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBS_GetPacketErr(pktErr);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL DMD_DVBS_GetCellID(MS_U16 *u16CellID)
{
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBS_Get_CELL_ID(u16CellID);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBS_GetCellID %x\n", *u16CellID);
    }
    #endif
    return bRet;
}

MS_BOOL DMD_DVBS_GetStatus(DMD_DVBS_MODULATION_TYPE *pQAMMode, MS_U32 *u32SymbolRate, MS_S16  *s16CFO)
{
    MS_BOOL bRet=TRUE;

    DMD_LOCK();
    bRet &= INTERN_DVBS_GetCurrentModulationType(pQAMMode);
    bRet &= INTERN_DVBS_GetCurrentSymbolRate(u32SymbolRate);
    bRet &= INTERN_DVBS_Get_FreqOffset(s16CFO);
    DMD_UNLOCK();

    #ifdef MS_DEBUG
    if (_u8DMD_DVBS_DbgLevel >= DMD_DVBS_DBGLV_INFO)
    {
        ULOGD("DEMOD","MDrv_DMD_DVBS_GetStatus %d %ld \n", *pQAMMode, *u32SymbolRate);
    }
    #endif
    return bRet;
}

MS_U32 DMD_DVBS_SetPowerState(EN_POWER_MODE u16PowerState)
{
    static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    if(bIsDVBS == TRUE)
    {
    if (u16PowerState == E_POWER_SUSPEND)
	{
        DMD_DVBS_Exit();
        _prev_u16PowerState = u16PowerState;
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (_prev_u16PowerState == E_POWER_SUSPEND)
        {
            DMD_DVBS_Init(&_sDMD_DVBS_InitData, sizeof(_sDMD_DVBS_InitData));
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
         ULOGD("DEMOD","\r\n ====== DVBS doesn't need to Suspend/Resume at Non-DVBS mode ====== \r\n");
        u32Return = FALSE;
    }
    return UTOPIA_STATUS_SUCCESS;
}
/*
MS_U32 MDrv_DMD_DVBS_Demod_Restart(MS_U32 u32SymbolRate, MS_U32 u32Frequency)
{
	MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBS_Demod_Restart(u32SymbolRate, u32Frequency);
	sDMD_DVBS_Info.u32ChkScanTimeStart = MsOS_GetSystemTime();
    DMD_UNLOCK();

    return bRet;
}
*/

MS_BOOL DMD_DVBS_Get_FreqOffset(MS_S16 *s16CFO)
{
	MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBS_Get_FreqOffset(s16CFO);
    DMD_UNLOCK();

    return bRet;
}
//------------------------------------------------------------------------------
MS_BOOL DMD_DVBS_BlindScan_Start(MS_U16 u16StartFreq, MS_U16 u16EndFreq)
{
	MS_BOOL bRet=TRUE;

    DMD_LOCK();
    bRet=INTERN_DVBS_BlindScan_Start(u16StartFreq, u16EndFreq);
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL DMD_DVBS_BlindScan_NextFreq(MS_BOOL* bBlindScanEnd)
{
	MS_BOOL bRet=TRUE;

    DMD_LOCK();
    bRet=INTERN_DVBS_BlindScan_NextFreq(bBlindScanEnd);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL DMD_DVBS_BlindScan_Cancel(void)
{
	MS_BOOL bRet=TRUE;

    DMD_LOCK();
    bRet=INTERN_DVBS_BlindScan_Cancel();
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL DMD_DVBS_BlindScan_End(void)
{
	MS_BOOL bRet=TRUE;

    DMD_LOCK();
    bRet=INTERN_DVBS_BlindScan_End();
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL DMD_DVBS_BlindScan_GetChannel(MS_U16 u16ReadStart, MS_U16* u16TPNum, HAL_DEMOD_MS_FE_CARRIER_PARAM *pTable)
{
    return INTERN_DVBS_BlindScan_GetChannel(u16ReadStart, u16TPNum, pTable);
}

MS_BOOL DMD_DVBS_BlindScan_GetCurrentFreq(MS_U32 *u32CurrentFeq)
{
    return INTERN_DVBS_BlindScan_GetCurrentFreq(u32CurrentFeq);
}

MS_BOOL DMD_DVBS_BlindScan_WaitCurFreqFinished(MS_U8* u8Progress,MS_U8 *u8FindNum, MS_U8 *substate_reg, MS_U32  *u32Data, MS_U16 *symbolrate_reg, MS_U16 *CFO_reg)
{
    return INTERN_DVBS_BlindScan_WaitCurFreqFinished(u8Progress, u8FindNum,substate_reg,u32Data,symbolrate_reg,CFO_reg);
}

MS_BOOL DMD_DVBS_BlindScan_GetTunerFreq(MS_U16 *u16TunerCenterFreq, MS_U16 *u16TunerCutOffFreq)
{
    return INTERN_DVBS_BlindScan_GetTunerFreq(u16TunerCenterFreq, u16TunerCutOffFreq);
}
//------------------------------------------------------------------------------
MS_BOOL DMD_DVBS_DiSEqC_Init(void)
{
	MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBS_DiSEqC_Init();
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL DMD_DVBS_DiSEqC_SetLNBOut(MS_BOOL bLow)
{
	MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBS_DiSEqC_SetLNBOut(bLow);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL DMD_DVBS_DiSEqC_GetLNBOut(MS_BOOL* bLNBOutLow)
{
	MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBS_DiSEqC_GetLNBOut(bLNBOutLow);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL DMD_DVBS_DiSEqC_Set22kOnOff(MS_BOOL b22kOn)
{
	MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBS_DiSEqC_Set22kOnOff(b22kOn);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL DMD_DVBS_DiSEqC_Get22kOnOff(MS_BOOL* b22kOn)
{
	MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBS_DiSEqC_Get22kOnOff(b22kOn);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL DMD_DVBS_DiSEqC_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize)
{
	MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBS_DiSEqC_SendCmd(pCmd, u8CmdSize);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL DMD_DVBS_DiSEqC_SetTxToneMode(MS_BOOL bTxTone22kOff)
{
	MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBS_DiSEqC_SetTxToneMode(bTxTone22kOff);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL DMD_DVBS_DiSEqC_SetTone(MS_BOOL bTone1)
{
	MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBS_DiSEqC_SetTone(bTone1);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL DMD_DVBS_UnicableAGCCheckPower(MS_BOOL pbAGCCheckPower)
{
	MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBS_UnicableAGCCheckPower(pbAGCCheckPower);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL DMD_DVBS_GetTsDivNum(MS_U32 *u32SymbolRate, MS_U8* system_type_reg, MS_U8 *code_rate_idx, MS_U8 *fec_type_idx, MS_U8 *pilot_flag, MS_U32 *u32temp, MS_U8 *code_rate_reg)
{
    MS_BOOL bRet;
    bRet=INTERN_DVBS_GetTsDivNum(u32SymbolRate, system_type_reg, code_rate_idx, fec_type_idx, pilot_flag, u32temp, code_rate_reg);
    return bRet;
}

MS_BOOL DMD_DVBS_GetCurrentDemodCodeRate(DMD_DVBS_CODE_RATE_TYPE *pCodeRate)
{

    MS_BOOL bRet;

    DMD_LOCK();

    bRet = INTERN_DVBS_GetCurrentDemodCodeRate(pCodeRate);
    //printf("MDrv_DMD_DVBS_GetFWVer %x\n", *ver);
    DMD_UNLOCK();

    return bRet;

}

MS_BOOL DMD_DVBS_GetTunrSignalLevel_PWR(MS_U16 *u16Data)
{
    MS_BOOL bRet;
    bRet=INTERN_DVBS_GetTunrSignalLevel_PWR(u16Data);
    return bRet;
}
#ifdef UFO_SUPPORT_VCM
MS_BOOL DMD_DVBS_Set_IS_ID(MS_U8 u8IS_ID)
{
    MS_BOOL bret;
    bret = INTERN_DVBS2_Set_IS_ID(u8IS_ID);
    return bret;
}

MS_BOOL DMD_DVBS_Set_Default_IS_ID(MS_U8 *u8IS_ID, MS_U8 *u8ISID_table)
{
    MS_BOOL bret;
    bret = INTERN_DVBS2_Set_Default_IS_ID(u8IS_ID, u8ISID_table);
    return bret;
}


MS_BOOL DMD_DVBS_Get_IS_ID_INFO(MS_U8 *u8IS_ID, MS_U8 *u8ISID_table)
{
    MS_BOOL bret;
    bret = INTERN_DVBS2_Get_IS_ID_INFO(u8IS_ID, u8ISID_table);
    return bret;
}

MS_BOOL DMD_DVBS_VCM_INIT(DMD_DVBS_VCM_OPT u8VCM_OPT, MS_U8 u8IS_ID, MS_U32 u32DMD_DVBS2_DJB_START_ADDR)
{
    MS_BOOL bret = TRUE;
    bret &= INTERN_DVBS2_VCM_INIT(u8VCM_OPT, u8IS_ID, u32DMD_DVBS2_DJB_START_ADDR);
    #ifdef MS_DEBUG
    ULOGD("DEMOD","firmware version: %x\n",sDMD_DVBS_Info.u16Version);
    #endif
    
    return bret;
}

MS_BOOL DMD_DVBS_VCM_CHECK(void)
{
    MS_BOOL bret;
    bret = INTERN_DVBS2_VCM_CHECK();
    return bret;
}

MS_BOOL DMD_DVBS_VCM_ENABLED(MS_U8 u8VCM_ENABLED)
{
   MS_BOOL bret;
   bret = INTERN_DVBS2_VCM_ENABLED(u8VCM_ENABLED);
   return bret;
}

MS_BOOL DMD_DVBS_VCM_MODE(DMD_DVBS_VCM_OPT u8VCM_OPT)
{
   MS_BOOL bret;
   bret = INTERN_DVBS2_VCM_MODE(u8VCM_OPT);
   return bret;
}

#endif

#ifdef UFO_DEMOD_DVBS_SUPPORT_DMD_INT
MS_BOOL DMD_DVBS_INT_MODE(fpIntCallBack fpCBReg)
{
    fpCB = fpCBReg;
    return TRUE;
}
#endif 

MS_U32 DVBSOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSOpen\n"));	
    
    DVBS_INSTANT_PRIVATE *pDVBSPri= NULL;

    UtopiaInstanceCreate(sizeof(DVBS_INSTANT_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void*)&pDVBSPri);


	pDVBSPri->fpDVBS_Init=DMD_DVBS_Init;
	pDVBSPri->fpDVBS_Exit=DMD_DVBS_Exit;
	pDVBSPri->fpDVBS_SetDbgLevel=DMD_DVBS_SetDbgLevel;
  
	pDVBSPri->fpDVBS_GetInfo=DMD_DVBS_GetInfo;
	pDVBSPri->fpDVBS_GetLibVer=DMD_DVBS_GetLibVer;
	
	pDVBSPri->fpDVBS_GetFWVer=DMD_DVBS_GetFWVer;
	pDVBSPri->fpDVBS_GetDSPReg=DMD_DVBS_GetDSPReg;
	pDVBSPri->fpDVBS_SetDSPReg=DMD_DVBS_SetDSPReg;
	pDVBSPri->fpDVBS_GetReg=DMD_DVBS_GetReg;
	pDVBSPri->fpDVBS_SetReg=DMD_DVBS_SetReg;

	pDVBSPri->fpDVBSSetSerialControl=DMD_DVBS_SetSerialControl;
	#ifdef UFO_DEMOD_BLINDSCAN_NEW_FLOW
	pDVBSPri->fpDVBS_BlindScan_Config_symbol_rate_list=DMD_DVBS_BlindScan_Config_symbol_rate_list;
       #endif
	pDVBSPri->fpDVBS_SetConfig_symbol_rate_list=DMD_DVBS_SetConfig_symbol_rate_list;
	pDVBSPri->fpDVBS_SetActive=DMD_DVBS_SetActive;

	pDVBSPri->fpDVBS_GetLockWithRFPower=DMD_DVBS_GetLockWithRFPower;

	pDVBSPri->fpDVBS_GetSignalStrengthWithRFPower=DMD_DVBS_GetSignalStrengthWithRFPower;
	//pDVBSPri->fpDVBS_GetSignalQualityWithRFPower=DMD_DVBS_GetSignalQualityWithRFPower;
	
	pDVBSPri->fpDVBS_ActiveDmdSwitch=DMD_DVBS_ActiveDmdSwitch;

	pDVBSPri->fpDVBS_GetSNR=DMD_DVBS_GetSNR;
  pDVBSPri->fpDVBS_GetPostViterbiBer=DMD_DVBS_GetPostViterbiBer;

	pDVBSPri->fpDVBS_GetPacketErr=DMD_DVBS_GetPacketErr;

	pDVBSPri->fpDVBS_GetCellID=DMD_DVBS_GetCellID;
	//pDVBSPri->fpDVBS_GetStatus=DMD_DVBS_GetStatus;
	pDVBSPri->fpDVBS_GetStatus=DMD_DVBS_GetStatus;
	
	pDVBSPri->fpDVBS_SetPowerState=DMD_DVBS_SetPowerState;
	pDVBSPri->fpDVBS_Get_FreqOffset=DMD_DVBS_Get_FreqOffset;
       pDVBSPri->fpDVBS_BlindScan_Start=DMD_DVBS_BlindScan_Start;
	pDVBSPri->fpDVBS_BlindScan_NextFreq=DMD_DVBS_BlindScan_NextFreq;
	pDVBSPri->fpDVBS_BlindScan_Cancel=DMD_DVBS_BlindScan_Cancel;
	pDVBSPri->fpDVBS_BlindScan_End=DMD_DVBS_BlindScan_End;
	pDVBSPri->fpDVBS_BlindScan_GetChannel=DMD_DVBS_BlindScan_GetChannel;
	pDVBSPri->fpDVBS_BlindScan_GetCurrentFreq=DMD_DVBS_BlindScan_GetCurrentFreq;
	pDVBSPri->fpDVBS_BlindScan_WaitCurFreqFinished=DMD_DVBS_BlindScan_WaitCurFreqFinished;
	pDVBSPri->fpDVBS_BlindScan_GetTunerFreq=DMD_DVBS_BlindScan_GetTunerFreq;
	pDVBSPri->fpDVBS_DiSEqC_Init=DMD_DVBS_DiSEqC_Init;
	pDVBSPri->fpDVBS_DiSEqC_SetLNBOut=DMD_DVBS_DiSEqC_SetLNBOut;
	pDVBSPri->fpDVBS_DiSEqC_GetLNBOut=DMD_DVBS_DiSEqC_GetLNBOut;
	pDVBSPri->fpDVBS_DiSEqC_Set22kOnOff=DMD_DVBS_DiSEqC_Set22kOnOff;
	pDVBSPri->fpDVBS_DiSEqC_Get22kOnOff=DMD_DVBS_DiSEqC_Get22kOnOff;
	pDVBSPri->fpDVBS_DiSEqC_SendCmd=DMD_DVBS_DiSEqC_SendCmd;
	pDVBSPri->fpDVBS_DiSEqC_SetTxToneMode=DMD_DVBS_DiSEqC_SetTxToneMode;
	pDVBSPri->fpDVBS_DiSEqC_SetTone=DMD_DVBS_DiSEqC_SetTone;
	pDVBSPri->fpDVBS_UnicableAGCCheckPower=DMD_DVBS_UnicableAGCCheckPower;
	pDVBSPri->fpDVBS_GetTsDivNum=DMD_DVBS_GetTsDivNum;
	pDVBSPri->fpDVBS_GetIFAGC=DMD_DVBS_GetIFAGC;
	pDVBSPri->fpDVBS_GetCurrentDemodCodeRate=DMD_DVBS_GetCurrentDemodCodeRate;
       pDVBSPri->fpDVBS_GetTunrSignalLevel_PWR=DMD_DVBS_GetTunrSignalLevel_PWR;
#ifdef UFO_SUPPORT_VCM
       pDVBSPri->fpDVBS_Set_IS_ID = DMD_DVBS_Set_IS_ID;
       pDVBSPri->fpDVBS_Set_Default_IS_ID = DMD_DVBS_Set_Default_IS_ID;
       pDVBSPri->fpDVBS_Get_IS_ID_INFO = DMD_DVBS_Get_IS_ID_INFO;
	pDVBSPri->fpDVBS_VCM_INIT = DMD_DVBS_VCM_INIT;   
       pDVBSPri->fpDVBS_VCM_CHECK = DMD_DVBS_VCM_CHECK;
       pDVBSPri->fpDVBS_VCM_ENABLED = DMD_DVBS_VCM_ENABLED; 
       pDVBSPri->fpDVBS_VCM_MODE = DMD_DVBS_VCM_MODE;
#endif

#ifdef UFO_DEMOD_DVBS_SUPPORT_DMD_INT
       pDVBSPri->fpDVBS_INT_MODE = DMD_DVBS_INT_MODE;
#endif       

#ifdef UFO_DEMOD_GetParam_NEW_FLOW
        pDVBSPri->fpDVBS_GetParam = DMD_DVBS_GetParam;
#endif

#ifdef UFO_DEMOD_GET_AGC_INFO
        pDVBSPri->fpDVBS_GetAGC_INFO = DMD_DVBS_GetAGCInfo;
#endif

    //return TRUE;
    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 DVBSIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    //ULOGD("DEMOD","\n[drvDMD_INTERN_DVBS_v2.c][DVBS INFO] DVBS Ioctl \n");
    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    
    //void* pResource = NULL;
    
    DVBS_INSTANT_PRIVATE* psDVBSInstPri = NULL;
    void* psDVBSInstPriVoid = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psDVBSInstPriVoid);
    psDVBSInstPri = (DVBS_INSTANT_PRIVATE*)psDVBSInstPriVoid;
    
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
        case DMD_DVBS_DRV_CMD_Init:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_Init\n"));
 	     bRet = psDVBSInstPri->fpDVBS_Init( (((PDVBS_Init_PARAM)pArgs)->pDMD_DVBS_InitData), ((PDVBS_Init_PARAM)pArgs)->u32InitDataLen);
 	     ((PDVBS_Init_PARAM)pArgs)->ret=bRet;
            break;
        case DMD_DVBS_DRV_CMD_Exit:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_Exit\n"));
	     bRet= psDVBSInstPri->fpDVBS_Exit();
	     ((PDVBS_EXIT_PARAM_PARAM)pArgs)->ret=bRet;
            break;
        case DMD_DVBS_DRV_CMD_SetDbgLevel:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_SetDbgLevel\n"));            
	    bRet= psDVBSInstPri->fpDVBS_SetDbgLevel(((PDVBS_SetDbgLevel_PARAM)pArgs)->u8DbgLevel);	
	    ((PDVBS_SetDbgLevel_PARAM)pArgs)->ret=bRet;
            break;
        case DMD_DVBS_DRV_CMD_GetInfo:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_GetInfo\n"));            
	    ((PDVBS_GetInfo_PARAM)pArgs)->ret_info=psDVBSInstPri->fpDVBS_GetInfo();
            break;
        case DMD_DVBS_DRV_CMD_GetLibVer:
            bRet=psDVBSInstPri->fpDVBS_GetLibVer(((PDVBS_GetLibVer_PARAM)pArgs)->ppVersion);
            ((PDVBS_GetLibVer_PARAM)pArgs)->ret=bRet;
            break;
        case DMD_DVBS_DRV_CMD_GetFWVer:
              DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_GetFWVer\n"));            
		bRet=psDVBSInstPri->fpDVBS_GetFWVer(((PDVBS_GetFWVer_PARAM)pArgs)->ver);
	       ((PDVBS_GetFWVer_PARAM)pArgs)->ret=bRet;
            break;
        case DMD_DVBS_DRV_CMD_GetDSPReg:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_GetDSPReg\n"));                   
		bRet=psDVBSInstPri->fpDVBS_GetDSPReg(((PDVBS_GetDSPReg_PARAM)pArgs)->u16Addr,((PDVBS_GetDSPReg_PARAM)pArgs)->pu8Data);
		((PDVBS_GetDSPReg_PARAM)pArgs)->ret=bRet;
        break;

        case DMD_DVBS_DRV_CMD_SetDSPReg:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_SetDSPReg\n"));                   
		bRet=psDVBSInstPri->fpDVBS_SetDSPReg(((PDVBS_SetDSPReg_PARAM)pArgs)->u16Addr,((PDVBS_SetDSPReg_PARAM)pArgs)->pu8Data);
		((PDVBS_SetDSPReg_PARAM)pArgs)->ret=bRet;
        break;
        case DMD_DVBS_DRV_CMD_GetReg:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_GetReg\n"));            
		bRet=psDVBSInstPri->fpDVBS_GetReg(((PDVBS_GetReg_PARAM)pArgs)->u16Addr,((PDVBS_GetReg_PARAM)pArgs)->pu8Data);
		((PDVBS_GetReg_PARAM)pArgs)->ret=bRet;
            break;
        case DMD_DVBS_DRV_CMD_SetReg:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_SetReg\n"));            
		bRet=psDVBSInstPri->fpDVBS_SetReg(((PDVBS_SetReg_PARAM)pArgs)->u16Addr,((PDVBS_SetReg_PARAM)pArgs)->u8Data);
		((PDVBS_SetReg_PARAM)pArgs)->ret=bRet;
            break;
            
        case DMD_DVBS_DRV_CMD_SetSerialControl:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_SetSerialControl\n"));            
		bRet=psDVBSInstPri->fpDVBSSetSerialControl(((PDVBS_SetSerialControl_PARAM)pArgs)->bEnable);
		((PDVBS_SetSerialControl_PARAM)pArgs)->ret=bRet;
            break;

#ifdef UFO_DEMOD_BLINDSCAN_NEW_FLOW
        case DMD_DVBS_DRV_CMD_BlindScan_Config_symbol_rate_list:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_BlindScan_Config_symbol_rate_list\n"));                        
		bRet=psDVBSInstPri->fpDVBS_BlindScan_Config_symbol_rate_list(((PDVBS_BlindScan_Config_Symbol_rate_list_PARAM)pArgs)->u16SymbolRate,\
		((PDVBS_BlindScan_Config_Symbol_rate_list_PARAM)pArgs)->eQamMode,((PDVBS_BlindScan_Config_Symbol_rate_list_PARAM)pArgs)->u32IFFreq,\
		((PDVBS_BlindScan_Config_Symbol_rate_list_PARAM)pArgs)->bSpecInv,\
		((PDVBS_BlindScan_Config_Symbol_rate_list_PARAM)pArgs)->bSerialTS,((PDVBS_BlindScan_Config_Symbol_rate_list_PARAM)pArgs)->pu16_symbol_rate_list,\
		((PDVBS_BlindScan_Config_Symbol_rate_list_PARAM)pArgs)->u8_symbol_rate_list_num
		);

		((PDVBS_BlindScan_Config_Symbol_rate_list_PARAM)pArgs)->ret=bRet;
			
            break;
#endif			
        case DMD_DVBS_DRV_CMD_SetConfig_symbol_rate_list:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_SetSetConfig_symbol_rate_list\n"));                        
		bRet=psDVBSInstPri->fpDVBS_SetConfig_symbol_rate_list(((PDVBS_SetConfig_Symbol_rate_list_PARAM)pArgs)->u16SymbolRate,\
		((PDVBS_SetConfig_Symbol_rate_list_PARAM)pArgs)->eQamMode,((PDVBS_SetConfig_Symbol_rate_list_PARAM)pArgs)->u32IFFreq,\
		((PDVBS_SetConfig_Symbol_rate_list_PARAM)pArgs)->bSpecInv,\
		((PDVBS_SetConfig_Symbol_rate_list_PARAM)pArgs)->bSerialTS,((PDVBS_SetConfig_Symbol_rate_list_PARAM)pArgs)->pu16_symbol_rate_list,\
		((PDVBS_SetConfig_Symbol_rate_list_PARAM)pArgs)->u8_symbol_rate_list_num
		);

		((PDVBS_SetConfig_Symbol_rate_list_PARAM)pArgs)->ret=bRet;
			
            break;
			
        case DMD_DVBS_DRV_CMD_SetActive:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_SetActive\n"));            
		bRet=psDVBSInstPri->fpDVBS_SetActive(((PDVBS_SetActive_PARAM)pArgs)->bEnable);
		((PDVBS_SetActive_PARAM)pArgs)->ret=bRet;
            break;

         
        case DMD_DVBS_DRV_CMD_GetLockWithRFPower:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_GetLockWithRFPower\n"));               

		//ULOGD("DEMOD","bryan check DVBS in V2 layer DMD_DVBS_DRV_CMD_GetLockWithRFPower\n");
		bRet=psDVBSInstPri->fpDVBS_GetLockWithRFPower(((PDVBS_GetLockWithRFPower)pArgs)->eType,\
		((PDVBS_GetLockWithRFPower)pArgs)->eLockStatus,\
		((PDVBS_GetLockWithRFPower)pArgs)->u32CurrRFPowerDbm,\
		((PDVBS_GetLockWithRFPower)pArgs)->u32NoChannelRFPowerDbm);
		((PDVBS_GetLockWithRFPower)pArgs)->ret=bRet;
			
            break;

        case DMD_DVBS_DRV_CMD_GetSignalStrengthWithRFPower:
		bRet=psDVBSInstPri->fpDVBS_GetSignalStrengthWithRFPower(((PDVBS_GetSignalStrengthWithRFPower_PARAM)pArgs)->fRFPowerDbm, ((PDVBS_GetSignalStrengthWithRFPower_PARAM)pArgs)->pDemodType, ((PDVBS_GetSignalStrengthWithRFPower_PARAM)pArgs)->u8_DVBS2_CurrentCodeRateLocal, ((PDVBS_GetSignalStrengthWithRFPower_PARAM)pArgs)->u8_DVBS2_CurrentConstellationLocal);
		((PDVBS_GetSignalStrengthWithRFPower_PARAM)pArgs)->ret=bRet;
            break;
/*
	case DMD_DVBS_DRV_CMD_GetSignalQualityWithRFPower:
		bRet=psDVBSInstPri->fpDVBS_GetSignalQualityWithRFPower((PDVBS_GetSignalQualityWithRFPower_PARAM)pArgs->u16Quality);
		(PDVBS_GetSignalQualityWithRFPower_PARAM)pArgs->ret=bRet;
	     break;
	#endif
*/
	
       case DMD_DVBS_DRV_CMD_ActiveDmdSwitch:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_ActiveDmdSwitch\n"));                 
		bRet=psDVBSInstPri->fpDVBS_ActiveDmdSwitch(((PDVBS_ActiveDmdSwitch_PARAM)pArgs)->demod_no);
		((PDVBS_ActiveDmdSwitch_PARAM)pArgs)->ret=bRet;
       	break;


       case DMD_DVBS_DRV_CMD_GetSNR:
       	bRet=psDVBSInstPri->fpDVBS_GetSNR(((PDVBS_GetSNR_PARAM)pArgs)->u32NDA_SNR_A,((PDVBS_GetSNR_PARAM)pArgs)->u32NDA_SNR_AB);
       	((PDVBS_GetSNR_PARAM)pArgs)->ret=bRet;
       	break;

    
       case DMD_DVBS_DRV_CMD_GetPostViterbiBer:
		bRet=psDVBSInstPri->fpDVBS_GetPostViterbiBer(((PDVBS_GetPostViterbiBer_PARAM)pArgs)->BitErr_reg,\
		((PDVBS_GetPostViterbiBer_PARAM)pArgs)->BitErrPeriod_reg);
		((PDVBS_GetPostViterbiBer_PARAM)pArgs)->ret=bRet;
           break;

	
       case DMD_DVBS_DRV_CMD_GetPacketErr:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_GetPacketErr\n"));                    
		bRet=psDVBSInstPri->fpDVBS_GetPacketErr(((PDVBS_GetPacketErr_PARAM)pArgs)->pktErr);
		((PDVBS_GetPacketErr_PARAM)pArgs)->ret=bRet;
           break;

	case DMD_DVBS_DRV_CMD_GetCellID:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_GetCellID\n"));                 
		bRet=psDVBSInstPri->fpDVBS_GetCellID(((PDVBS_GetCellID_PARAM)pArgs)->u16CellID);
		((PDVBS_GetCellID_PARAM)pArgs)->ret=bRet;
		break;
  
	case DMD_DVBS_DRV_CMD_GetStatus:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_GetStatus\n"));             
		bRet=psDVBSInstPri->fpDVBS_GetStatus(((PDVBS_GetStatus_PARAM)pArgs)->pQAMMode,((PDVBS_GetStatus_PARAM)pArgs)->u32SymbolRate,\
		((PDVBS_GetStatus_PARAM)pArgs)->s16CFO);
		((PDVBS_GetStatus_PARAM)pArgs)->ret=bRet;
		break;	


	case DMD_DVBS_DRV_CMD_SetPowerState:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_SetPowerState\n"));            
		((PDVBS_SetPowerState_PARAM)pArgs)->ret_U32=psDVBSInstPri->fpDVBS_SetPowerState(((PDVBS_SetPowerState_PARAM)pArgs)->u16PowerState);

		break;

	case DMD_DVBS_DRV_CMD_Get_FreqOffset:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_Get_FreqOffset\n"));            
		bRet=psDVBSInstPri->fpDVBS_Get_FreqOffset(((PDVBS_Get_FreqOffset_PARAM)pArgs)->s16CFO);
		((PDVBS_Get_FreqOffset_PARAM)pArgs)->bRet=bRet;

		break;

	case DMD_DVBS_DRV_CMD_BlindScan_Start:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_BlindScan_Start\n"));            
		bRet=psDVBSInstPri->fpDVBS_BlindScan_Start(((PDVBS_BlindScan_Start_PARAM)pArgs)->u16StartFreq,((PDVBS_BlindScan_Start_PARAM)pArgs)->u16EndFreq);
		((PDVBS_BlindScan_Start_PARAM)pArgs)->bRet=bRet;

		break;

	case DMD_DVBS_DRV_CMD_BlindScan_NextFreq:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_BlindScan_NextFreq\n"));            
		bRet=psDVBSInstPri->fpDVBS_BlindScan_NextFreq(((PDVBS_BlindScan_NextFreq_PARAM)pArgs)->bBlindScanEnd);
		((PDVBS_BlindScan_NextFreq_PARAM)pArgs)->bRet=bRet;

		break;

	case DMD_DVBS_DRV_CMD_BlindScan_Cancel:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_BlindScan_Cancel\n"));            
		bRet=psDVBSInstPri->fpDVBS_BlindScan_Cancel();
		((PDVBS_BlindScan_Cancel_PARAM)pArgs)->bRet=bRet;

		break;	

	case DMD_DVBS_DRV_CMD_BlindScan_End:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_BlindScan_End\n"));            
		bRet=psDVBSInstPri->fpDVBS_BlindScan_End();
		((PDVBS_BlindScan_End_PARAM)pArgs)->bRet=bRet;

		break;

	case DMD_DVBS_DRV_CMD_BlindScan_GetChannel:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_BlindScan_GetChannel\n"));            
		bRet=psDVBSInstPri->fpDVBS_BlindScan_GetChannel(((PDVBS_BlindScan_GetChannel_PARAM)pArgs)->u16ReadStart,((PDVBS_BlindScan_GetChannel_PARAM)pArgs)->u16TPNum,((PDVBS_BlindScan_GetChannel_PARAM)pArgs)->pTable);
		((PDVBS_BlindScan_GetChannel_PARAM)pArgs)->bRet=bRet;

		break;

	case DMD_DVBS_DRV_CMD_BlindScan_GetCurrentFreq:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_BlindScan_GetCurrentFreq\n"));            
		bRet=psDVBSInstPri->fpDVBS_BlindScan_GetCurrentFreq(((PDVBS_BlindScan_GetCurrentFreq_PARAM)pArgs)->u32CurrentFreq);
		((PDVBS_BlindScan_GetCurrentFreq_PARAM)pArgs)->bRet=bRet;

		break;

	case DMD_DVBS_DRV_CMD_BlindScan_WaitCurFreqFinished:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_BlindScan_WaitCurFreqFinished\n"));            
		bRet=psDVBSInstPri->fpDVBS_BlindScan_WaitCurFreqFinished(((PDVBS_BlindScan_WaitCurFreqFinished_PARAM)pArgs)->u8Progress,((PDVBS_BlindScan_WaitCurFreqFinished_PARAM)pArgs)->u8FindNum,((PDVBS_BlindScan_WaitCurFreqFinished_PARAM)pArgs)->substate_reg,((PDVBS_BlindScan_WaitCurFreqFinished_PARAM)pArgs)->u32Data,((PDVBS_BlindScan_WaitCurFreqFinished_PARAM)pArgs)->symbolrate_reg,((PDVBS_BlindScan_WaitCurFreqFinished_PARAM)pArgs)->CFO_reg);
		((PDVBS_BlindScan_WaitCurFreqFinished_PARAM)pArgs)->bRet=bRet;

		break;

	case DMD_DVBS_DRV_CMD_BlindScan_GetTunerFreq:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_BlindScan_GetTunerFreq\n"));            
		bRet=psDVBSInstPri->fpDVBS_BlindScan_GetTunerFreq(((PDVBS_BlindScan_GetTunerFreq_PARAM)pArgs)->u16TunerCenterFreq,((PDVBS_BlindScan_GetTunerFreq_PARAM)pArgs)->u16TunerCutOffFreq);
		((PDVBS_BlindScan_GetTunerFreq_PARAM)pArgs)->bRet=bRet;

		break;

	case DMD_DVBS_DRV_CMD_DiSEqC_Init:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_DiSEqC_Init\n"));            
		bRet=psDVBSInstPri->fpDVBS_DiSEqC_Init();
		((PDVBS_DiSEqC_Init_PARAM)pArgs)->bRet=bRet;

		break;

	case DMD_DVBS_DRV_CMD_DiSEqC_SetLNBOut:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_DiSEqC_SetLNBOut\n"));            
		bRet=psDVBSInstPri->fpDVBS_DiSEqC_SetLNBOut(((PDVBS_DiSEqC_SetLNBOut_PARAM)pArgs)->bLow);
		((PDVBS_DiSEqC_SetLNBOut_PARAM)pArgs)->bRet=bRet;

		break;

	case DMD_DVBS_DRV_CMD_DiSEqC_GetLNBOut:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_DiSEqC_GetLNBOut\n"));            
		bRet=psDVBSInstPri->fpDVBS_DiSEqC_GetLNBOut(((PDVBS_DiSEqC_GetLNBOut_PARAM)pArgs)->bLNBOutLow);
		((PDVBS_DiSEqC_GetLNBOut_PARAM)pArgs)->bRet=bRet;

		break;

	case DMD_DVBS_DRV_CMD_DiSEqC_Set22kOnOff:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_DiSEqC_Set22kOnOff\n"));            
		bRet=psDVBSInstPri->fpDVBS_DiSEqC_Set22kOnOff(((PDVBS_DiSEqC_Set22kOnOff_PARAM)pArgs)->b22kOn);
		((PDVBS_DiSEqC_Set22kOnOff_PARAM)pArgs)->bRet=bRet;

		break;

	case DMD_DVBS_DRV_CMD_DiSEqC_Get22kOnOff:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_DiSEqC_Get22kOnOff\n"));            
		bRet=psDVBSInstPri->fpDVBS_DiSEqC_Get22kOnOff(((PDVBS_DiSEqC_Get22kOnOff_PARAM)pArgs)->b22kOn);
		((PDVBS_DiSEqC_Get22kOnOff_PARAM)pArgs)->bRet=bRet;

		break;

	case DMD_DVBS_DRV_CMD_DiSEqC_SendCmd:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_DiSEqC_SendCmd\n"));            
		bRet=psDVBSInstPri->fpDVBS_DiSEqC_SendCmd(((PDVBS_DiSEqC_SendCmd_PARAM)pArgs)->pCmd,((PDVBS_DiSEqC_SendCmd_PARAM)pArgs)->u8CmdSize);
		((PDVBS_DiSEqC_SendCmd_PARAM)pArgs)->bRet=bRet;

		break;

	case DMD_DVBS_DRV_CMD_DiSEqC_SetTxToneMode:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_DiSEqC_SetTxToneMode\n"));            
		bRet=psDVBSInstPri->fpDVBS_DiSEqC_SetTxToneMode(((PDVBS_DiSEqC_SetTxToneMode_PARAM)pArgs)->bTxTone22kOff);
		((PDVBS_DiSEqC_SetTxToneMode_PARAM)pArgs)->bRet=bRet;

		break;

	case DMD_DVBS_DRV_CMD_DiSEqC_SetTone:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_DiSEqC_SetTone\n"));            
		bRet=psDVBSInstPri->fpDVBS_DiSEqC_SetTone(((PDVBS_DiSEqC_SetTone_PARAM)pArgs)->bTone1);
		((PDVBS_DiSEqC_SetTone_PARAM)pArgs)->bRet=bRet;

		break;

	case DMD_DVBS_DRV_CMD_UnicableAGCCheckPower:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_UnicableAGCCheckPower\n"));            
		bRet=psDVBSInstPri->fpDVBS_UnicableAGCCheckPower(((PDVBS_UnicableAGCCheckPower_PARAM)pArgs)->pbAGCCheckPower);
		((PDVBS_UnicableAGCCheckPower_PARAM)pArgs)->bRet=bRet;

		break;
		
	case DMD_DVBS_DRV_CMD_GetIFAGC:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_GetIFAGC\n"));            
		bRet=psDVBSInstPri->fpDVBS_GetIFAGC(((PDVBS_GetIFAGC_PARAM)pArgs)->ifagc_reg, ((PDVBS_GetIFAGC_PARAM)pArgs)->ifagc_reg_lsb, ((PDVBS_GetIFAGC_PARAM)pArgs)->ifagc_err_reg);
		((PDVBS_GetIFAGC_PARAM)pArgs)->ret=bRet;

		break;
		
       case DMD_DVBS_DRV_CMD_GetTsDivNum:
       	DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_GetTsDivNum\n"));
		bRet=psDVBSInstPri->fpDVBS_GetTsDivNum(((PDVBS_GetTsDivNum_PARAM)pArgs)->u32SymbolRate, ((PDVBS_GetTsDivNum_PARAM)pArgs)->system_type_reg, ((PDVBS_GetTsDivNum_PARAM)pArgs)->code_rate_idx, ((PDVBS_GetTsDivNum_PARAM)pArgs)->fec_type_idx, ((PDVBS_GetTsDivNum_PARAM)pArgs)->pilot_flag, ((PDVBS_GetTsDivNum_PARAM)pArgs)->u32temp, ((PDVBS_GetTsDivNum_PARAM)pArgs)->code_rate_reg);
		((PDVBS_GetTsDivNum_PARAM)pArgs)->bRet=bRet;
           break;

	case DMD_DVBS_DRV_CMD_GetCurrentDemodCodeRate: 
       	DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_GetCurrentDemodCodeRate\n"));
		bRet=psDVBSInstPri->fpDVBS_GetCurrentDemodCodeRate(((PDVBS_GetCurrentDemodCodeRate_PARAM)pArgs)->pCodeRate);
		((PDVBS_GetCurrentDemodCodeRate_PARAM)pArgs)->bRet=bRet;
           break;

       case DMD_DVBS_DRV_CMD_GetTunrSignalLevel_PWR: 
       	DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_GetTunrSignalLevel_PWR\n"));
		bRet=psDVBSInstPri->fpDVBS_GetTunrSignalLevel_PWR(((PDVBS_GetTunrSignalLevel_PWR_PARAM)pArgs)->u16Data);
		((PDVBS_GetTunrSignalLevel_PWR_PARAM)pArgs)->bRet=bRet;
           break;
#ifdef UFO_SUPPORT_VCM
       case DMD_DVBS_DRV_CMD_Set_IS_ID:
       	DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_Set_IS_ID\n"));
		bRet=psDVBSInstPri->fpDVBS_Set_IS_ID(((PDVBS_SetVCM_PARAM)pArgs)->u8IS_ID);
		((PDVBS_SetVCM_PARAM)pArgs)->ret=bRet;
       break;            

       case DMD_DVBS_DRV_CMD_Set_Default_IS_ID:
       	DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_Set_Default_IS_ID\n"));
		bRet=psDVBSInstPri->fpDVBS_Set_Default_IS_ID(((PDVBS_SetDefaultVCM_PARAM)pArgs)->u8IS_ID, ((PDVBS_SetDefaultVCM_PARAM)pArgs)->u8IS_ID_table);
		((PDVBS_SetDefaultVCM_PARAM)pArgs)->ret=bRet;
        break;

        case DMD_DVBS_DRV_CMD_Get_IS_ID_INFO:
       	DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_Get_IS_ID_INFO\n"));
		bRet=psDVBSInstPri->fpDVBS_Get_IS_ID_INFO(((PDVBS_GetVCM_PARAM)pArgs)->u8IS_ID, ((PDVBS_GetVCM_PARAM)pArgs)->u8IS_ID_table);
		((PDVBS_GetVCM_PARAM)pArgs)->ret=bRet;
        break;

        case DMD_DVBS_DRV_CMD_VCM_Init:
       	DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_VCM_INIT\n"));
            bRet=psDVBSInstPri->fpDVBS_VCM_INIT(((PDVBS_VCM_INIT_PARAM)pArgs)->u8VCM_OPT, ((PDVBS_VCM_INIT_PARAM)pArgs)->u8IS_ID, ((PDVBS_VCM_INIT_PARAM)pArgs)->u32DVBS2_DJB_START_ADDR);
		((PDVBS_VCM_INIT_PARAM)pArgs)->ret=bRet;
        break;

        case DMD_DVBS_DRV_CMD_VCM_Check:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_VCM_CHECK\n"));
            bRet = psDVBSInstPri->fpDVBS_VCM_CHECK();
            ((PDVBS_VCM_CHECK)pArgs)->ret = bRet;
        break;

        case DMD_DVBS_DRV_CMD_VCM_Enabled:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_VCM_ENABLED\n"));            
            bRet = psDVBSInstPri->fpDVBS_VCM_ENABLED(( (PDVBS_VCM_ENABLED)pArgs)->u8VCM_ENABLED);            
            ((PDVBS_VCM_ENABLED)pArgs)->ret = bRet;
        break;

        case DMD_DVBS_DRV_CMD_VCM_Mode:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_VCM_MODE\n"));            
            bRet = psDVBSInstPri->fpDVBS_VCM_MODE(( (PDVBS_VCM_MODE)pArgs)->u8VCM_OPT);            
            ((PDVBS_VCM_MODE)pArgs)->ret = bRet;         
        break;            
#endif

#ifdef UFO_DEMOD_DVBS_SUPPORT_DMD_INT
        case DMD_DVBS_DRV_CMD_INT_MODE:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_DRV_CMD_INT_MODE\n"));            
            bRet = psDVBSInstPri->fpDVBS_INT_MODE(( (PDVBS_INT_MODE)pArgs)->fpCB);            
            ((PDVBS_INT_MODE)pArgs)->ret = bRet;
        break;                        
#endif            

#ifdef UFO_DEMOD_GetParam_NEW_FLOW
        case DMD_DVBS_DRV_CMD_GetParam:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_DRV_CMD_GetParam\n"));            
            bRet = psDVBSInstPri->fpDVBS_GetParam( (PDVBS_GET_INFO)pArgs );     
            ((PDVBS_GET_INFO)pArgs)->ret = bRet;
        break;            
#endif
#ifdef UFO_DEMOD_GET_AGC_INFO
        case DMD_DVBS_DRV_CMD_GetAGCInfo:
            DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSIoctl - MDrv_DMD_DVBS_DRV_CMD_GetAGC_INFO\n"));            
            bRet = psDVBSInstPri->fpDVBS_GetAGC_INFO( ((PDVBS_GET_AGC_INFO)pArgs)->u8dbg_mode, ((PDVBS_GET_AGC_INFO)pArgs)->pu16Data );     
            ((PDVBS_GET_AGC_INFO)pArgs)->ret = bRet;
        break;
#endif
         default:
            break;
    }
    
    //jway suggest UtopiaResourceRelease(pResource);
            
    return (bRet ? UTOPIA_STATUS_SUCCESS : UTOPIA_STATUS_FAIL);
}

MS_U32 DVBSClose(void* pInstance)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSClose\n"));

    UtopiaInstanceDelete(pInstance);

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 DVBSStr(MS_U32 u32PowerState, void* pModule)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    MS_U32 u32Ret = 0;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSStr\n"));

    //UtopiaModuleGetSTRPrivate(pModule, (void**));

    if (u32PowerState == E_POWER_SUSPEND)
    {
        /* Please Implement Module Suspend Flow Here. */
        u32Ret = DMD_DVBS_SetPowerState(E_POWER_SUSPEND);
        
        if(u32Ret == TRUE)
          u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
        else
          u32Return = UTOPIA_STATUS_FAIL;
        DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c][DVBSStr] SUSPEND !\n"));            
    }
    else if (u32PowerState == E_POWER_RESUME)
    {
        /* Please Implement Module Resume Flow Here. */
        u32Ret = DMD_DVBS_SetPowerState(E_POWER_RESUME);

        if(u32Ret == TRUE)        
          u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
        else
          u32Return = UTOPIA_STATUS_FAIL;
        DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c][DVBSStr] RESUME !\n"));               
    }
    else
    {
        u32Return = UTOPIA_STATUS_FAIL;
        DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c][DVBSStr] OTHERS !\n"));                     
    }

    return u32Return;// for success
}

void DVBSRegisterToUtopia(void)
{
    // 1. deal with module
    
    void* pUtopiaModule = NULL;
//    UtopiaModuleCreate(MODULE_DVBT, 8, &pUtopiaModule);   
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c]DVBSRegisterToUtopia\n"));

    UtopiaModuleCreate(MODULE_DVBS, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)DVBSOpen, (FUtopiaClose)DVBSClose, (FUtopiaIOctl)DVBSIoctl);
 
   // Utopia2K STR
#if defined(MSOS_TYPE_LINUX_KERNEL)
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBS_v2.c][DVBSRegisterToUtopia] KERNEL DVBSStr!\n"));
    UtopiaModuleSetupSTRFunctionPtr(pUtopiaModule,(FUtopiaSTR)DVBSStr);
#endif
}
