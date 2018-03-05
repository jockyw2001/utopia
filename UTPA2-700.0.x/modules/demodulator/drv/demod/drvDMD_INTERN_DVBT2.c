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
#include "drvDMD_INTERN_DVBT2.h"
#include "drvDMD_INTERN_DVBT2_v2.h"
#include "halDMD_INTERN_DVBT2.h"
#include "halDMD_INTERN_common.h"
#include "../../include/drvSAR.h"  // for Utopia2
//#include "../sar/drvSAR.h"
#include "utopia.h"
#include "ULog.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define DVBT2_BER_TH_HY 0.1
static float fBerFilteredDVBT2 = -1.0;

static float dvbt2_ssi_dbm_nordigp1[][6] =
{
    { -95.7, -94.4, -93.6, -92.6, -92.0, -91.5},
    { -90.8, -89.1, -87.9, -86.7, -85.8, -85.2},
    { -86.9, -84.6, -83.2, -81.4, -80.3, -79.7},
    { -83.5, -80.4, -78.6, -76.0, -74.4, -73.3},
};

// cr, 3/5(1),	2/3(2), 3/4 (3)
float fT2_SSI_formula[][12]=
{
	{1.0/5,  97.0,	3.0/2,	82.0, 16.0/5,  50.0, 29.0/10.0, 21.0, 18.0/15, 3.0, 3.0/5, 0.0}, // CR3/5
	{2.0/3,  95.0,	9.0/5,	77.0, 17.0/5,  43.0, 14.0/5.0,	15.0, 13.0/15, 2.0, 2.0/5, 0.0}, // CR2/3
	{1.0/2,  93.0, 19.0/10, 74.0, 31.0/10, 43.0, 22.0/10.0, 21.0, 18.0/15, 3.0, 3.0/5, 0.0}, // CR3/4
};

//arthur
static float dvbt2_sqi_db_nordigp1[][6] =
{
    { 3.5, 4.7, 5.6, 6.6, 7.2, 7.7},
    { 8.7, 10.1, 11.4, 12.5, 13.3, 13.8},
    { 13.0, 14.8, 16.2, 17.7, 18.7, 19.4},
    { 17.0, 19.4, 20.8, 22.9, 24.3, 25.1},
};
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#define DMD_LOCK()      \
    do{                         \
        MS_ASSERT(MsOS_In_Interrupt() == FALSE); \
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
/*static MSIF_Version _drv_dmd_dvbt2_intern_version = {
    .MW = { DMD_DVBT2_INTERN_VER, },
};*/
#else
static MSIF_Version _drv_dmd_dvbt_intern_version;
#endif

#if 0
//static DMD_DVBT2_InitData _sDMD_DVBT2_InitData;
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
#endif
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define DMD_DBG(x)          (x)
#else
#define DMD_DBG(x)          (x)
#endif
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void* ppDVBT2Instant = NULL;
static MS_U32 u32DVBT2open = 0;
static MS_U8 u8DVBT2UtopiaOpen = 0;   //for SetStillImagePara is earlier called than Init

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMD_DVBT2_Init(DMD_DVBT2_InitData *pDMD_DVBT2_InitData, MS_U32 u32InitDataLen)
{
    void* pAttribte = NULL;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_Init\n"));

    DVBT2_INIT_PARAM Drv_DVBT2_INIT_PARAM;
    DMD_DVBT2_InitData_Transform Init_Para_Temp;
    Drv_DVBT2_INIT_PARAM.ret=false;

    Init_Para_Temp.u8DMD_DVBT2_DSPRegInitExt=pDMD_DVBT2_InitData->u8DMD_DVBT2_DSPRegInitExt;
    Init_Para_Temp.u8DMD_DVBT2_DSPRegInitSize=pDMD_DVBT2_InitData->u8DMD_DVBT2_DSPRegInitSize;
    Init_Para_Temp.u8DMD_DVBT2_InitExt=pDMD_DVBT2_InitData->u8DMD_DVBT2_InitExt;
    Init_Para_Temp.u8SarChannel=pDMD_DVBT2_InitData->u8SarChannel;
    Init_Para_Temp.u32EqStartAddr=pDMD_DVBT2_InitData->u32EqStartAddr;
    Init_Para_Temp.u32TdiStartAddr=pDMD_DVBT2_InitData->u32TdiStartAddr;
    Init_Para_Temp.u32DjbStartAddr=pDMD_DVBT2_InitData->u32DjbStartAddr;
    Init_Para_Temp.u32FwStartAddr=pDMD_DVBT2_InitData->u32FwStartAddr;

    Drv_DVBT2_INIT_PARAM.pDMD_DVBT2_InitData=&Init_Para_Temp;
    Drv_DVBT2_INIT_PARAM.u32InitDataLen=sizeof(Init_Para_Temp);

    if(u8DVBT2UtopiaOpen == 0)  // First time open
    {
        if(UtopiaOpen(MODULE_DVBT2/*|KERNEL_MODE*/ , &ppDVBT2Instant, 0, pAttribte) == UTOPIA_STATUS_SUCCESS)  //kernel space
//if(UtopiaOpen(MODULE_DVBT , &ppDVBTInstant, 0, pAttribte) == UTOPIA_STATUS_SUCCESS)  //user space
        {
            u32DVBT2open = 1;
//            DMD_DBG(ULOGD("DEMOD","\r\n ======== DVBT2 Open Successful %x =========", (WORD)u32DVBT2open));
            DMD_DBG(ULOGD("DEMOD","\r\n ======== DVBT2 Open Successful  ========= \n"));
        }
        else
        {
//            DMD_DBG(ULOGD("DEMOD","\r\n ======== DVBT2 Open Fail %x =========", (WORD)u32DVBT2open));
            DMD_DBG(ULOGD("DEMOD","\r\n ======== DVBT2 Open Fail  =========\n"));
            return false;
        }

        u8DVBT2UtopiaOpen = 1;
    }

    UtopiaIoctl(ppDVBT2Instant,DMD_DVBT2_DRV_CMD_Init,&Drv_DVBT2_INIT_PARAM);
    return Drv_DVBT2_INIT_PARAM.ret;

#if 0
    char pDMD_DVBT2_MutexString[16];
    MS_U8 u8ADCIQMode = 0, u8PadSel = 0, bPGAEnable = 0, u8PGAGain = 5;
    MS_BOOL bRFAGCTristateEnable = 1;
    MS_BOOL bIFAGCTristateEnable = 0;

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
        MDrv_SAR_Adc_Config(_sDMD_DVBT2_InitData.u8SarChannel, TRUE);
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
    u32DMD_DVBT2_EQ_START_ADDR = _sDMD_DVBT2_InitData.u32EqStartAddr;
    u32DMD_DVBT2_TDI_START_ADDR = _sDMD_DVBT2_InitData.u32TdiStartAddr;
    u32DMD_DVBT2_DJB_START_ADDR = _sDMD_DVBT2_InitData.u32DjbStartAddr;
    u32DMD_DVBT2_FW_START_ADDR = _sDMD_DVBT2_InitData.u32FwStartAddr;

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
#endif
}

MS_BOOL MDrv_DMD_DVBT2_Exit(void)
{
    DVBT2_EXIT_PARAM Drv_DVBT2_EXIT_PARAM;
    Drv_DVBT2_EXIT_PARAM.ret=false;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_Exit\n"));

    if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_Exit, &Drv_DVBT2_EXIT_PARAM);
    else
        return false;

    return Drv_DVBT2_EXIT_PARAM.ret;
#if 0
    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_T2_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT2_Exit\n");
    }
    #endif
    DMD_LOCK();
    INTERN_DVBT2_Exit();
    DMD_UNLOCK();
    MsOS_DeleteMutex(_s32DMD_DVBT2_Mutex);
    _s32DMD_DVBT2_Mutex= -1;
    return TRUE;
#endif
}

MS_BOOL MDrv_DMD_DVBT2_SetDbgLevel(DMD_T2_DbgLv u8DbgLevel)
{
    DVBT2_SETDBGLEVEL_PARAM Drv_DVBT2_SETDBG_LEVEL_PARAM;
    Drv_DVBT2_SETDBG_LEVEL_PARAM.u8DbgLevel=u8DbgLevel;
    Drv_DVBT2_SETDBG_LEVEL_PARAM.ret=false;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT.c]MDrv_DMD_DVBT2_SetDbgLevel\n"));

     if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_SetDbgLevel, &Drv_DVBT2_SETDBG_LEVEL_PARAM);
     else
    return false;

    return Drv_DVBT2_SETDBG_LEVEL_PARAM.ret;
#if 0
    DMD_LOCK();
    _u8DMDDbgLevel = u8DbgLevel;
    DMD_UNLOCK();
    return TRUE;
#endif
}

DMD_DVBT2_Info* MDrv_DMD_DVBT2_GetInfo(DMD_DVBT2_INFO_TYPE eInfoType)
{
    //MS_BOOL return_val;
    DVBT2_GETINFO_PARAM Drv_DVBT2_GETINFO_PARAM;
    Drv_DVBT2_GETINFO_PARAM.eInfoType=eInfoType;
    Drv_DVBT2_GETINFO_PARAM.pInfo=NULL;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_GetInfo\n"));

    if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_GetInfo, &Drv_DVBT2_GETINFO_PARAM);

    return Drv_DVBT2_GETINFO_PARAM.pInfo;

#if 0
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
#endif
}

MS_BOOL MDrv_DMD_DVBT2_GetLibVer(const MSIF_Version **ppVersion)
{
    DVBT2_GETLIBVER_PARAM Drv_DVBT2_GETLIBVER_PARAM;
    Drv_DVBT2_GETLIBVER_PARAM.ppVersion=ppVersion;
    Drv_DVBT2_GETLIBVER_PARAM.ret=false;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_GetLibVer\n"));
    if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_GetLibVer, &Drv_DVBT2_GETLIBVER_PARAM);
    else
        return false;

    return Drv_DVBT2_GETLIBVER_PARAM.ret;
#if 0
    DMD_LOCK();
    if (!ppVersion)
    {
        return FALSE;
    }

    *ppVersion = &_drv_dmd_dvbt2_intern_version;
    DMD_UNLOCK();
    return TRUE;
#endif
}

MS_BOOL MDrv_DMD_DVBT2_GetFWVer(MS_U16 *ver)
{
     //MS_BOOL return_val;
    DVBT2_GETFWVER_PARAM Drv_DVBT2_GETFWVER_PARAM;
    Drv_DVBT2_GETFWVER_PARAM.ver=ver;
    Drv_DVBT2_GETFWVER_PARAM.ret=false;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2GetFWVer\n"));
    if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_GetFWVer, &Drv_DVBT2_GETFWVER_PARAM);
    else
        return false;

    return Drv_DVBT2_GETFWVER_PARAM.ret;
#if 0
    MS_BOOL bRet;

    DMD_LOCK();

    bRet = INTERN_DVBT2_Version(ver);
    //printf("MDrv_DMD_DVBT2_GetFWVer %x\n",*ver);
    DMD_UNLOCK();

    return bRet;
#endif
}

MS_BOOL MDrv_DMD_DVBT2_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet;

    DVBT2_GETREG_PARAM Drv_DVBT2_GETREG_PARAM;
    Drv_DVBT2_GETREG_PARAM.u16Addr=u16Addr;
    Drv_DVBT2_GETREG_PARAM.pu8Data=pu8Data;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_GetReg\n"));

    bRet=UtopiaIoctl(ppDVBT2Instant,DMD_DVBT2_DRV_CMD_GetReg,&Drv_DVBT2_GETREG_PARAM);
    return bRet;
#if 0
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
#endif
}

MS_BOOL MDrv_DMD_DVBT2_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL return_val;

    DVBT2_SETREG_PARAM Drv_DVBT2_SETREG_PARAM;
    Drv_DVBT2_SETREG_PARAM.u16Addr=u16Addr;
    Drv_DVBT2_SETREG_PARAM.u8Data=u8Data;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_SetReg\n"));
    return_val=UtopiaIoctl(ppDVBT2Instant,DMD_DVBT2_DRV_CMD_SetReg,&Drv_DVBT2_SETREG_PARAM);
    return return_val;
#if 0
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
#endif
}

MS_BOOL MDrv_DMD_DVBT2_SetSerialControl(MS_BOOL bEnable)
{
     //MS_BOOL return_val;
     DVBT2_SETSERIALCONTROL_PARAM Drv_DVBT2_SetSerialControl_PARAM;
     Drv_DVBT2_SetSerialControl_PARAM.bEnable=bEnable;
     Drv_DVBT2_SetSerialControl_PARAM.ret=false;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_SetSerialControl\n"));

    if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_SetSerialControl, &Drv_DVBT2_SetSerialControl_PARAM);
    else
        return false;

    return Drv_DVBT2_SetSerialControl_PARAM.ret;
#if 0
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
#endif
}

MS_BOOL MDrv_DMD_DVBT2_SetReset(void)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_SetReset\n"));

    if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_SetReset, NULL);
    else
        return false;

    return true;
#if 0
    MS_BOOL bRet;

    DMD_LOCK();
    bRet = INTERN_DVBT2_SoftReset();
    DMD_UNLOCK();

    return bRet;
#endif
}

MS_BOOL MDrv_DMD_DVBT2_SetConfig(DMD_DVBT2_RF_CHANNEL_BANDWIDTH BW, MS_BOOL bSerialTS, MS_U8 u8PlpID)
{
    //MS_BOOL return_val;
    DVBT2_SETCONFIG_PARAM Drv_DVBT2_SetConfig_PARAM;
    Drv_DVBT2_SetConfig_PARAM.BW=BW;
    Drv_DVBT2_SetConfig_PARAM.bSerialTS=bSerialTS;
    Drv_DVBT2_SetConfig_PARAM.u8PlpID=u8PlpID;
    Drv_DVBT2_SetConfig_PARAM.ret=false;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_SetConfig\n"));

    if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_SetConfig, &Drv_DVBT2_SetConfig_PARAM);
    else
    return false;

    return Drv_DVBT2_SetConfig_PARAM.ret;
#if 0
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
#endif
}

MS_BOOL MDrv_DMD_DVBT2_SetActive(MS_BOOL bEnable)
{
    //MS_BOOL return_val;
    DVBT2_SETACTIVE_PARAM Drv_DVBT2_SetActive_PARAM;
    Drv_DVBT2_SetActive_PARAM.bEnable=bEnable;
    Drv_DVBT2_SetActive_PARAM.ret=false;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_SetActive\n"));
    if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_SetActive, &Drv_DVBT2_SetActive_PARAM);
    else
        return false;

    return Drv_DVBT2_SetActive_PARAM.ret;
#if 0
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
#endif
}

MS_BOOL MDrv_DMD_DVBT2_GetLock(DMD_DVBT2_GETLOCK_TYPE eType, DMD_T2_LOCK_STATUS *eLockStatus)
{
    //MS_BOOL return_val;
    DVBT2_GETLOCK_PARAM Drv_DVBT2_GetLock_PARAM;
    Drv_DVBT2_GetLock_PARAM.eType=eType;
    Drv_DVBT2_GetLock_PARAM.eLockStatus=eLockStatus;
    Drv_DVBT2_GetLock_PARAM.ret=false;
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_GetLock\n"));

    if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_GetLock, &Drv_DVBT2_GetLock_PARAM);
    else
        return false;

    return Drv_DVBT2_GetLock_PARAM.ret;
#if 0
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
#endif
}

// Floating computation/data. Need to move to upper layer to action for kerel mode.
#ifndef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_DMD_DVBT2_GetSignalStrength(MS_U16 *u16Strength)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_GetSignalStrength\n"));

    return MDrv_DMD_DVBT2_GetSignalStrengthWithRFPower(u16Strength, 200.0f);

#if 0
    return MDrv_DMD_DVBT2_GetSignalStrengthWithRFPower(u16Strength, 200.0f);
#endif
}

#if 0
MS_BOOL MDrv_DMD_DVBT2_GetSignalStrengthWithRFPower(MS_U16 *u16Strength, float fRFPowerDbm)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_GetSignalStrengthWithRFPower\n"));

#if(0)
    DVBT2_GETSIGNALSTRENGTHWITHRFPOWER_PARAM Drv_DVBT2_GetSignalStrengthWithRFPower_PARAM;
    Drv_DVBT2_GetSignalStrengthWithRFPower_PARAM.u16Strength=u16Strength;
    Drv_DVBT2_GetSignalStrengthWithRFPower_PARAM.fRFPowerDbm=fRFPowerDbm;

    if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_GetSignalStrengthWithRFPower, &Drv_DVBT2_GetSignalStrengthWithRFPower_PARAM);
    else
        return false;

    return Drv_DVBT2_GetSignalStrengthWithRFPower_PARAM.ret;
#else
    *u16Strength=100;
    return true;
#endif

#if 0
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
#endif
}
#else
MS_BOOL MDrv_DMD_DVBT2_GetSignalStrengthWithRFPower(MS_U16 *u16Strength, float fRFPowerDbm)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_GetSignalStrengthWithRFPower\n"));

    MS_U8   status = true;
    float   ch_power_db = 0.0f;
    float   ch_power_ref = 11.0f;
    float   ch_power_rel = 0.0f;
    //MS_U8   u8_index = 0;
    MS_U16  L1_info_qam, L1_info_cr;
//    MS_U8  demodState = 0;
    DMD_T2_LOCK_STATUS eLockStatus;

    if(u32DVBT2open==1)
    {
        MDrv_DMD_DVBT2_GetLock(E_DMD_DVBT2_FEC_LOCK, &eLockStatus);
        if (E_DMD_T2_LOCK != eLockStatus)
        {
            *u16Strength = 0;
            return TRUE;
        }

#if 0
        // use pointer of IFAGC table to identify
        // case 1: RFAGC from SAR, IFAGC controlled by demod
        // case 2: RFAGC from tuner, ,IFAGC controlled by demod
        status &= HAL_DMD_GetRFLevel(&ch_power_db, fRFPowerDbm, u8SarValue,
                                                                sDMD_DVBT2_InitData->pTuner_RfagcSsi, sDMD_DVBT2_InitData->u16Tuner_RfagcSsi_Size,
                                                                sDMD_DVBT2_InitData->pTuner_IfagcSsi_HiRef, sDMD_DVBT2_InitData->u16Tuner_IfagcSsi_HiRef_Size,
                                                                sDMD_DVBT2_InitData->pTuner_IfagcSsi_LoRef, sDMD_DVBT2_InitData->u16Tuner_IfagcSsi_LoRef_Size,
                                                                sDMD_DVBT2_InitData->pTuner_IfagcErr_HiRef, sDMD_DVBT2_InitData->u16Tuner_IfagcErr_HiRef_Size,
                                                                sDMD_DVBT2_InitData->pTuner_IfagcErr_LoRef, sDMD_DVBT2_InitData->u16Tuner_IfagcErr_LoRef_Size);
#endif
        ch_power_db = fRFPowerDbm;  // get dBm from tuner now.

        if(MDrv_DMD_DVBT2_GetL1Info(&L1_info_qam, T2_MODUL_MODE) == FALSE)
            DMD_DBG(ULOGD("DEMOD","QAM parameter retrieve failure\n "));

        if(MDrv_DMD_DVBT2_GetL1Info(&L1_info_cr, T2_CODE_RATE) == FALSE)
            DMD_DBG(ULOGD("DEMOD","code rate parameter retrieve failure\n "));

        /*
        while(dvbt2_ssi_dbm_nordigp1[u8_index].constel != _UNKNOW_QAM)
        {
        if ( (dvbt2_ssi_dbm_nordigp1[u8_index].constel == (DMD_T2_CONSTEL)L1_info_qam)
            && (dvbt2_ssi_dbm_nordigp1[u8_index].code_rate == (DMD_T2_CODERATE)L1_info_cr))
        {
           ch_power_ref = dvbt2_ssi_dbm_nordigp1[u8_index].p_ref;
           break;
        }
        else
        {
           u8_index++;
        }
        }
        */
        ch_power_ref = dvbt2_ssi_dbm_nordigp1[(MS_U8)L1_info_qam][(MS_U8)L1_info_cr];

        //    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + (0x62*2), &demodState);

        if (ch_power_ref > 10.0f)
            *u16Strength = 0;
        else
        {
             // For Nordig's SSI test items
            if ( (L1_info_qam == 3) //256qam
            && (L1_info_cr > 0 && L1_info_cr < 4) // CR 3/5,2/3,3/4
            )
            {
              MS_U8 u8_x = L1_info_cr - 1;
              float f_ssi = 0.0;

              if(ch_power_db >= -45)f_ssi = 100;
              else if (ch_power_db >= -50)  f_ssi = fT2_SSI_formula[u8_x][0]*(ch_power_db + 50) + fT2_SSI_formula[u8_x][1];
              else if (ch_power_db >= -60)  f_ssi = fT2_SSI_formula[u8_x][2]*(ch_power_db + 60) + fT2_SSI_formula[u8_x][3];
              else if (ch_power_db >= -70)  f_ssi = fT2_SSI_formula[u8_x][4]*(ch_power_db + 70) + fT2_SSI_formula[u8_x][5];
              else if (ch_power_db >= -80)  f_ssi = fT2_SSI_formula[u8_x][6]*(ch_power_db + 80) + fT2_SSI_formula[u8_x][7];
              else if (ch_power_db >= -95)  f_ssi = fT2_SSI_formula[u8_x][8]*(ch_power_db + 95) + fT2_SSI_formula[u8_x][9];
              else if (ch_power_db >= -100) f_ssi = fT2_SSI_formula[u8_x][10]*(ch_power_db + 100) + fT2_SSI_formula[u8_x][11];

              if (f_ssi > 100) *u16Strength = 100;
              else if (f_ssi < 0) *u16Strength = 0;
              else *u16Strength = (MS_U16)(f_ssi+0.5);

                DMD_DBG(ULOGD("DEMOD"," SSI... RF_level=%d, f_ssi=%d, ssi=%d, cr=%d, mod=%d\n", (MS_S16)ch_power_db, (MS_S16)f_ssi, (MS_S16)(*u16Strength), L1_info_cr, L1_info_qam));
            }
            else
            {
              ch_power_rel = ch_power_db - ch_power_ref;
              /*
                    if (demodState != 0x09)
                    {
                        ch_power_rel = ch_power_db - (-50.0f);
                    }
                    else
                    {
                        ch_power_rel = ch_power_db - ch_power_ref;
                    }
              */
                if ( ch_power_rel < -15.0f )
                {
                    *u16Strength = 0;
                }
                else if ( ch_power_rel < 0.0f )
                {
                    *u16Strength = (MS_U16)(2.0f/3*(ch_power_rel + 15.0f));
                }
                else if ( ch_power_rel < 20 )
                {
                    *u16Strength = (MS_U16)(4.0f*ch_power_rel + 10.0f);
                }
                else if ( ch_power_rel < 35.0f )
                {
                    *u16Strength = (MS_U16)(2.0f/3*(ch_power_rel - 20.0f) + 90.0f);
                }
                else
                {
                    *u16Strength = 100;
              }
            }
        }

        DMD_DBG(ULOGD("DEMOD","ch_power_ref(dB) = %d , ch_power_db(dB) = %d, ch_power_rel(dB) = %d\n", (MS_S16)ch_power_ref, (MS_S16)ch_power_db, (MS_S16)ch_power_rel));
        DMD_DBG(ULOGD("DEMOD","SSI_CH_PWR(dB) = %d , Score = %d\n", (MS_S16)ch_power_db, *u16Strength));
        DMD_DBG(ULOGD("DEMOD","SSI = %d \n",  *u16Strength));   
    }
    return status;
}
#endif

MS_BOOL MDrv_DMD_DVBT2_GetSignalQuality(MS_U16 *u16Quality)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_GetSignalQuality\n"));

    return MDrv_DMD_DVBT2_GetSignalQualityWithRFPower(u16Quality, 200.0f);
#if 0
    return MDrv_DMD_DVBT2_GetSignalQualityWithRFPower(u16Quality, 200.0f);
#endif
}

#if 1
/****************************************************************************
  Subject:    To get the DVT Signal quility
  Function:   INTERN_DVBT2_GetSignalQuality
  Parmeter:  Quility
  Return:      E_RESULT_SUCCESS
                   E_RESULT_FAILURE
  Remark:    Here we have 4 level range
                  <1>.First Range => Quility =100  (You can define it by INTERN_DVBT2_SIGNAL_BASE_100)
                  <2>.2th Range => 60 < Quality < 100 (You can define it by INTERN_DVBT2_SIGNAL_BASE_60)
                  <3>.3th Range => 10 < Quality < 60  (You can define it by INTERN_DVBT2_SIGNAL_BASE_10)
                  <4>.4th Range => Quality <10
*****************************************************************************/
MS_BOOL MDrv_DMD_DVBT2_GetSignalQualityWithRFPower(MS_U16 *u16Quality, float fRFPowerDbm)
//MS_BOOL INTERN_DVBT2_GetSignalQuality(MS_U16 *quality, const DMD_DVBT2_InitData *sDMD_DVBT2_InitData, MS_U8 u8SarValue, float fRFPowerDbm)
{
//    float   ber_sqi, SQI;
    float   fber = 0;
    float   cn_rec = 0;
    float   cn_ref = 0;
    float   cn_rel = 0;
    float   fBerTH1[] = {1E-4, 1E-4*(1.0-DVBT2_BER_TH_HY), 1E-4*(1.0+DVBT2_BER_TH_HY), 1E-4};
    float   fBerTH2[] = {3E-7, 3E-7, 3E-7*(1.0-DVBT2_BER_TH_HY), 3E-7*(1.0+DVBT2_BER_TH_HY)};
    float   BER_SQI = (float)0.0;
    float   SQI = (float)0.0;
    static MS_U8 u8SQIState = 0;
    DMD_T2_LOCK_STATUS eLockStatus;

    MS_U8   status = true;
    MS_U16   L1_info_qam = 0, L1_info_cr = 0;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_GetSignalQualityWithRFPower\n"));

    if(u32DVBT2open==1)
    {
        MDrv_DMD_DVBT2_GetLock(E_DMD_DVBT2_FEC_LOCK, &eLockStatus);
        if (E_DMD_T2_LOCK == eLockStatus)
        {
#if 1 // copy from msb1240
            if (fBerFilteredDVBT2 < 0.0)
            {
                if (MDrv_DMD_DVBT2_GetPostLdpcBer(&fber) == FALSE)
                {
                    DMD_DBG(ULOGD("DEMOD","INTERN_DVBT2 PostLDPCBER error \n "));
                    return FALSE;
                }
                fBerFilteredDVBT2 = fber;
            }
            else
            {
                fber = fBerFilteredDVBT2;
            }

            if (fber > fBerTH1[u8SQIState])
            {
               BER_SQI = 0.0;
               u8SQIState = 1;
            }
            else if (fber >=fBerTH2[u8SQIState])
            {
               BER_SQI = 100.0/15;
               u8SQIState = 2;
            }
            else
            {
                BER_SQI = 100.0/6;
                u8SQIState = 3;
            }

            MDrv_DMD_DVBT2_GetSNR(&cn_rec);

            if (cn_rec < 0.0)
                return FALSE;

            ///////// Get Constellation and Code Rate to determine Ref. C/N //////////
            ///////// (refer to Teracom min. spec 2.0 4.1.1.7) /////
            cn_ref = (float)-1.0;

            if(MDrv_DMD_DVBT2_GetL1Info(&L1_info_qam, T2_MODUL_MODE) == FALSE)
                DMD_DBG(ULOGD("DEMOD","QAM parameter retrieve failure\n "));

            if(MDrv_DMD_DVBT2_GetL1Info(&L1_info_cr, T2_CODE_RATE) == FALSE)
                DMD_DBG(ULOGD("DEMOD","code rate parameter retrieve failure\n "));

    // YP:Need to use init data. Tmp remove
#if 0
            for(i = 0; i < sDMD_DVBT2_InitData->u16SqiCnNordigP1_Size; i++)
            {
                if ( (L1_info_qam == sDMD_DVBT2_InitData->pSqiCnNordigP1[i].constel)
                && (L1_info_cr == sDMD_DVBT2_InitData->pSqiCnNordigP1[i].code_rate) )
                {
                    cn_ref = sDMD_DVBT2_InitData->pSqiCnNordigP1[i].cn_ref;
                    break;
                }
            }
#endif

             // arthur 
            cn_ref = dvbt2_sqi_db_nordigp1[(MS_U8)L1_info_qam][(MS_U8)L1_info_cr];
           
            if (cn_ref < 0.0)
            {
                SQI = (float)0.0;
                DMD_DBG(ULOGD("DEMOD","SQI is zero, 1\n"));
            }
            else
            {
                // 0.7, snr offset
                cn_rel = cn_rec - cn_ref + 0.7f;
                if (cn_rel > 3.0)
                    SQI = 100;
                else if (cn_rel >= -3)
                {
                    SQI = (cn_rel+3)*BER_SQI;
                    if (SQI > 100.0) SQI = 100.0;
                    else if (SQI < 0.0) SQI = 0.0;
                }
                else
                {
                    SQI = (float)0.0;
                    DMD_DBG(ULOGD("DEMOD","SQI is zero, 2\n"));
                }
            }

            *u16Quality = (MS_U16)SQI;
        }
        else
        {
            *u16Quality = 0;
        }

        DMD_DBG(ULOGD("DEMOD","SNR = %f, QAM = %d, code Rate = %d\n", cn_rec, L1_info_qam, L1_info_cr));
        DMD_DBG(ULOGD("DEMOD","BER = %8.3e\n", fber));
        DMD_DBG(ULOGD("DEMOD","Signal Quility = %d\n", *u16Quality));
    }
    return status;
}

#else
MS_BOOL MDrv_DMD_DVBT2_GetSignalQualityWithRFPower(MS_U16 *u16Quality, float fRFPowerDbm)
{
    //MS_BOOL return_val;
    DVBT2_GETSIGNALQUALITYWITHRFPOWER_PARAM Drv_DVBT2_GetSignalQualityWithRFPower_PARAM;
    Drv_DVBT2_GetSignalQualityWithRFPower_PARAM.u16Quality=u16Quality;
    Drv_DVBT2_GetSignalQualityWithRFPower_PARAM.fRFPowerDbm=fRFPowerDbm;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_GetSignalQualityWithRFPower\n"));
    /*bryan temp mark */
#if(0)
    if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_GetSignalQualityWithRFPower, &Drv_DVBT2_GetSignalQualityWithRFPower_PARAM);
    else
        return false;
#else
    Drv_DVBT2_GetSignalQualityWithRFPower_PARAM.ret=true;
#endif

    return Drv_DVBT2_GetSignalQualityWithRFPower_PARAM.ret;

#if 0
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
#endif
}
#endif

MS_BOOL MDrv_DMD_DVBT2_GetSNR(float *fSNR)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_GetSNR\n"));

#if(1)
    MS_U16 u16_snr100;
    MS_U8 snr_cali;
    MS_U8 u8_gi;

    DVBT2_GETSNR_PARAM Drv_DVBT2_GetSNR_PARAM;
    Drv_DVBT2_GetSNR_PARAM.u16_snr100 = &u16_snr100;
    Drv_DVBT2_GetSNR_PARAM.snr_cali = &snr_cali;
    Drv_DVBT2_GetSNR_PARAM.u8_gi = &u8_gi;
    Drv_DVBT2_GetSNR_PARAM.ret = false;

    if(u32DVBT2open==1)
    {
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_GetSNR, &Drv_DVBT2_GetSNR_PARAM);

        *fSNR = (float)u16_snr100/100.0;

        if (snr_cali == 1)
        {
            float snr_offset = 0.0;
            float snr_cali = 0.0;

            if (u8_gi == 0) snr_offset = 0.157;
            else if(u8_gi == 1) snr_offset = 0.317;
            else if(u8_gi == 2) snr_offset = 0.645;
            else if(u8_gi == 3) snr_offset = 1.335;
            else if(u8_gi == 4) snr_offset = 0.039;
            else if(u8_gi == 5) snr_offset = 0.771;
            else if(u8_gi == 6) snr_offset = 0.378;

            snr_cali = *fSNR - snr_offset;
            if (snr_cali > 0.0) *fSNR = snr_cali;
        }
        //use Polynomial curve fitting to fix snr
        //snr_poly = 0.0027945*pow(*fSNR,3) - 0.2266*pow(*fSNR,2) + 6.0101*(*fSNR) - 53.3621;
        //f_snr = f_snr + snr_poly;

#endif
    }
    else
        return false;

    return Drv_DVBT2_GetSNR_PARAM.ret;
#else
    //Drv_DVBT_GetSNR_PARAM.ret=true;
    *fSNR=30;
    return true;
#endif

#if 0
    DMD_LOCK();
    *fSNR=INTERN_DVBT2_GetSNR();
    DMD_UNLOCK();

    return TRUE;
#endif
}

MS_BOOL MDrv_DMD_DVBT2_GetPostLdpcBer(float *ber)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_GetPostLdpcBer\n"));

#if(1)
    MS_U16 BitErrPeriod_reg;
    MS_U32 BitErr_reg;
    MS_U16 FecType;

    DVBT2_GETPOSTLDPCBER_PARAM Drv_DVBT2_GetPostLdpcBer_PARAM;
    Drv_DVBT2_GetPostLdpcBer_PARAM.BitErr_reg =  &BitErr_reg;
    Drv_DVBT2_GetPostLdpcBer_PARAM.BitErrPeriod_reg = &BitErrPeriod_reg;
    Drv_DVBT2_GetPostLdpcBer_PARAM.FecType = &FecType;
    Drv_DVBT2_GetPostLdpcBer_PARAM.ret=false;

    if(u32DVBT2open==1)
    {
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_GetPostLdpcBer, &Drv_DVBT2_GetPostLdpcBer_PARAM);

        if (BitErrPeriod_reg == 0)
            //protect 0
            BitErrPeriod_reg = 1;

        if (FecType & 0x0180)
        {
            if (BitErr_reg == 0)
                *ber = (float)0.5 / (float)(BitErrPeriod_reg * 64800);
            else
                *ber = (float)BitErr_reg / (float)(BitErrPeriod_reg * 64800);
        }
        else
        {
            if (BitErr_reg == 0)
                *ber = (float)0.5 / (float)(BitErrPeriod_reg * 16200);
            else
                *ber = (float)BitErr_reg / (float)(BitErrPeriod_reg * 16200);
        }
        DMD_DBG(ULOGD("DEMOD","INTERN_DVBT2 PostLDPCBER = %8.3e \n ", *ber));
    }
    else
        return false;

    return Drv_DVBT2_GetPostLdpcBer_PARAM.ret;
#else
    //Pre_DVBT_GetPostViterbiBer_PARAM.ret=true;
    *ber=0;
    return true;
#endif

#if 0
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBT2_GetPostLdpcBer(ber);
    DMD_UNLOCK();

    return bRet;
#endif
}

MS_BOOL MDrv_DMD_DVBT2_GetPreLdpcBer(float *ber)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_GetPreLdpcBer\n"));

#if(1)
        MS_U16 BitErrPeriod_reg;
        MS_U32 BitErr_reg;
        MS_U16 FecType;

        DVBT2_GETPRELDPCBERPARAM Drv_DVBT2_GetPreLdpcBer_PARAM;
        Drv_DVBT2_GetPreLdpcBer_PARAM.BitErr_reg =  &BitErr_reg;
        Drv_DVBT2_GetPreLdpcBer_PARAM.BitErrPeriod_reg = &BitErrPeriod_reg;
        Drv_DVBT2_GetPreLdpcBer_PARAM.FecType = &FecType;
        Drv_DVBT2_GetPreLdpcBer_PARAM.ret=false;

        if(u32DVBT2open==1)
        {
            UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_GetPreLdpcBer, &Drv_DVBT2_GetPreLdpcBer_PARAM);

            if (BitErrPeriod_reg == 0)
                //protect 0
                BitErrPeriod_reg = 1;

            if (FecType & 0x0180)
            {
                if (BitErr_reg == 0)
                    *ber = (float)0.5 / (float)(BitErrPeriod_reg * 64800);
                else
                    *ber = (float)BitErr_reg / (float)(BitErrPeriod_reg * 64800);
            }
            else
            {
                if (BitErr_reg == 0)
                    *ber = (float)0.5 / (float)(BitErrPeriod_reg * 16200);
                else
                    *ber = (float)BitErr_reg / (float)(BitErrPeriod_reg * 16200);
            }
            DMD_DBG(ULOGD("DEMOD","INTERN_DVBT2 PreLDPCBER = %8.3e \n ", *ber));
        }
        else
            return false;

        return Drv_DVBT2_GetPreLdpcBer_PARAM.ret;
#else
        //Pre_DVBT_GetPostViterbiBer_PARAM.ret=true;
        *ber=0;
        return true;
#endif

#if 0
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBT2_GetPreLdpcBer(ber);
    DMD_UNLOCK();

    return bRet;
#endif
}

MS_BOOL MDrv_DMD_DVBT2_GetFreqOffset(float *pFreqOff)
{
     DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_GetFreqOffset\n"));
#if(1)
    MS_U32 CfoTd_reg;
    MS_U32 CfoFd_reg;
    MS_U32 Icfo_reg;
    MS_U8 fft_reg;

    DVBT2_GETFREQOFFSET_PARAM Drv_DVBT2_GetFreqOffset_PARAM;
    Drv_DVBT2_GetFreqOffset_PARAM.CfoTd_reg = &CfoTd_reg;
    Drv_DVBT2_GetFreqOffset_PARAM.CfoFd_reg = &CfoFd_reg;
    Drv_DVBT2_GetFreqOffset_PARAM.Icfo_reg = &Icfo_reg;
    Drv_DVBT2_GetFreqOffset_PARAM.fft_reg = &fft_reg;
    Drv_DVBT2_GetFreqOffset_PARAM.ret = false;

    if(u32DVBT2open==1)
    {
        float         N = 0.0, FreqB = 0.0;
        float         FreqCfoTd = 0.0, FreqCfoFd = 0.0, FreqIcfo = 0.0;
        MS_U8     u8BW = 8;

        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_GetFreqOffset, &Drv_DVBT2_GetFreqOffset_PARAM);

         FreqB = (float)u8BW * 8 / 7;

         FreqCfoTd = (float)CfoTd_reg;

         if (CfoTd_reg & 0x800000)
             FreqCfoTd = FreqCfoTd - (float)0x1000000;

         FreqCfoTd = FreqCfoTd * FreqB * 0.00011642;

         FreqCfoFd = (float)CfoFd_reg;

         if (CfoFd_reg & 0x800000)
             FreqCfoFd = FreqCfoFd - (float)0x1000000;

         FreqCfoFd = FreqCfoFd * FreqB * 0.00011642;

         FreqIcfo = (float)Icfo_reg;

         if (Icfo_reg & 0x400)
             FreqIcfo = FreqIcfo - (float)0x800;

         switch (fft_reg)
         {
             case 0x00:  N = 2048;  break;
             case 0x20:  N = 4096;  break;
             case 0x10:
             default:    N = 8192;  break;
         }

         FreqIcfo = FreqIcfo * FreqB / N * 1000;         //unit: kHz


         //*pFreqOff = FreqIcfo + (FreqCfoFd + FreqCfoTd)/1000;
         *pFreqOff = (-1.0f)*(FreqIcfo + (FreqCfoFd + FreqCfoTd)/1000);
         // DBG_GET_SIGNAL(printf("FCFO = %f\n", FreqCfoFd));
         // DBG_GET_SIGNAL(printf("TCFO = %f\n", FreqCfoTd));
         // DBG_GET_SIGNAL(printf("ICFO = %f\n", FreqIcfo));
         DMD_DBG(ULOGD("DEMOD","INTERN_DVBT2 FreqOffset = %8.3e \n ", *pFreqOff));
    }

    return Drv_DVBT2_GetFreqOffset_PARAM.ret;
#if 0
 #if(0)
    DVBT2_GETFREQOFFSET_PARAM Drv_DVBT2_GetFreqOffset_PARAM;
    Drv_DVBT2_GetFreqOffset_PARAM.pFreqOff=pFreqOff;
    if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_GetFreqOffset, &Drv_DVBT2_GetFreqOffset_PARAM);
    else
        return false;

    return Drv_DVBT2_GetFreqOffset_PARAM.ret;
#else
    //Drv_DVBT_GetFreqOffset_PARAM.ret=true;
    *pFreqOff = 0;
    return true;
#endif
#endif
}
#endif

MS_BOOL MDrv_DMD_DVBT2_NORDIG_SSI_Table_Write(DMD_T2_CONSTEL constel, DMD_T2_CODERATE code_rate, float write_value)
{
    //   MS_BOOL return_val;
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_NORDIG_SSI_Table_Write\n"));

    dvbt2_ssi_dbm_nordigp1[constel][code_rate] = write_value;
    return TRUE;

#if 0
#if(0)
    DVBT2_NORDIG_SSI_TABLE_WRITE_PARAM Drv_DVBT2_NORDIGSSITableWrite_PARAM;
    Drv_DVBT2_NORDIGSSITableWrite_PARAM.constel=constel;
    Drv_DVBT2_NORDIGSSITableWrite_PARAM.code_rate=code_rate;
    Drv_DVBT2_NORDIGSSITableWrite_PARAM.write_value=write_value;

    if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_NORDIG_SSI_Table_Write, &Drv_DVBT2_NORDIGSSITableWrite_PARAM);
    else
        return false;

    return Drv_DVBT2_NORDIGSSITableWrite_PARAM.ret;
#else
    //       Drv_DVBT_NORDIGSSITableWrite_PARAM.ret=true;
    return true;
#endif
#endif
}

MS_BOOL MDrv_DMD_DVBT2_NORDIG_SSI_Table_Read(DMD_T2_CONSTEL constel, DMD_T2_CODERATE code_rate, float *read_value)
{
    //   MS_BOOL return_val;
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_NORDIG_SSI_Table_Read\n"));

    *read_value = dvbt2_ssi_dbm_nordigp1[constel][code_rate];
    return TRUE;
#if 0
#if(0)
    DVBT2_NORDIG_SSI_TABLE_READ_PARAM Drv_DVBT2_NORDIGSSITableRead_PARAM;
    Drv_DVBT2_NORDIGSSITableRead_PARAM.constel=constel;
    Drv_DVBT2_NORDIGSSITableRead_PARAM.code_rate=code_rate;
    Drv_DVBT2_NORDIGSSITableRead_PARAM.read_value=read_value;

    if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_NORDIG_SSI_Table_Read, &Drv_DVBT2_NORDIGSSITableRead_PARAM);
    else
        return false;

    return Drv_DVBT2_NORDIGSSITableRead_PARAM.ret;
#else
    //       Drv_DVBT_NORDIGSSITableWrite_PARAM.ret=true;
    return true;
#endif
#endif
}

#endif

MS_BOOL MDrv_DMD_DVBT2_GetPacketErr(MS_U16 *pktErr)
{
    //  MS_BOOL return_val;
    DVBT2_GETPACKETERRPARAM Drv_DVBT2_GetPacketErr_PARAM;
    Drv_DVBT2_GetPacketErr_PARAM.pktErr=pktErr;
    Drv_DVBT2_GetPacketErr_PARAM.ret=false;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_GetPacketErr\n"));
    if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_GetPacketErr, &Drv_DVBT2_GetPacketErr_PARAM);
    else
        return false;

    return Drv_DVBT2_GetPacketErr_PARAM.ret;

#if 0
    MS_BOOL bRet;
    float   fBER;

    DMD_LOCK();
    INTERN_DVBT2_GetPostLdpcBer(&fBER);
    bRet=INTERN_DVBT2_GetPacketErr(pktErr);
    if ((*pktErr ==1) && (fBER<= 0.000001)) // for no signal case, from Oga
    {
        *pktErr = 0x3FF;
    }
    #ifdef MS_DEBUG
    if (_u8DMDDbgLevel >= DMD_T2_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_DVBT2_GetPacketErr %d\n", *pktErr);
    }
    #endif
    DMD_UNLOCK();

    return bRet;
#endif
}

MS_BOOL MDrv_DMD_DVBT2_GetL1Info(MS_U16 *u16Info, DMD_DVBT2_SIGNAL_INFO eSignalType)
{
    DVBT2_GETL1INFO_PARAM Drv_DVBT2_GetL1Info_PARAM;
    Drv_DVBT2_GetL1Info_PARAM.u16Info=u16Info;
    Drv_DVBT2_GetL1Info_PARAM.eSignalType=eSignalType;
    Drv_DVBT2_GetL1Info_PARAM.ret=false;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_GetL1Info\n"));
    if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_GetL1Info, &Drv_DVBT2_GetL1Info_PARAM);
    else
        return false;

    return Drv_DVBT2_GetL1Info_PARAM.ret;

#if 0
    MS_BOOL bRet;

    DMD_LOCK();
    bRet=INTERN_DVBT2_Get_L1_Parameter(u16Info, eSignalType );
    DMD_UNLOCK();

    return bRet;
#endif
}

MS_U32 MDrv_DMD_DVBT2_SetPowerState(EN_POWER_MODE u16PowerState)
{
    //      MS_BOOL return_val;
    DVBT2_SETPOWERSTATE_PARAM Drv_DVBT2_SetPowerState_PARAM;
    Drv_DVBT2_SetPowerState_PARAM.u16PowerState=u16PowerState;
    Drv_DVBT2_SetPowerState_PARAM.ret=false;

    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_SetPowerState\n"));
    if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_SetPowerState, &Drv_DVBT2_SetPowerState_PARAM);
    else
        return false;

    return Drv_DVBT2_SetPowerState_PARAM.ret;

#if 0
    static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    u32Return = u32Return;
    if (u16PowerState == E_POWER_SUSPEND)
	{
        MDrv_DMD_DVBT2_Exit();
        _prev_u16PowerState = u16PowerState;
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (_prev_u16PowerState == E_POWER_SUSPEND)
        {
            MDrv_DMD_DVBT2_Init(&_sDMD_DVBT2_InitData, sizeof(_sDMD_DVBT2_InitData));
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
    return UTOPIA_STATUS_SUCCESS;
#endif
}

MS_BOOL MDrv_DMD_DVBT2_GetPlpBitMap(MS_U8* u8PlpBitMap)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_GetPlpBitMap\n"));

    DVBT2_GETPLPBITMAP_PARAM Drv_DVBT2_GetPlpBitMap_PARAM;
    Drv_DVBT2_GetPlpBitMap_PARAM.u8PlpBitMap=u8PlpBitMap;
    Drv_DVBT2_GetPlpBitMap_PARAM.ret=false;

    if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_GetPlpBitMap, &Drv_DVBT2_GetPlpBitMap_PARAM);
    else
        return false;

    return Drv_DVBT2_GetPlpBitMap_PARAM.ret;

#if 0
    return INTERN_DVBT2_GetPlpBitMap(u8PlpBitMap);
#endif
}

MS_BOOL MDrv_DMD_DVBT2_GetPlpGroupID(MS_U8 u8PlpID, MS_U8* u8GroupID)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_GetPlpGroupID\n"));

    DVBT2_GETPLPGROUPID_PARAM Drv_DVBT2_GetPlpGroupID_PARAM;
    Drv_DVBT2_GetPlpGroupID_PARAM.u8PlpID=u8PlpID;
    Drv_DVBT2_GetPlpGroupID_PARAM.u8GroupID=u8GroupID;
    Drv_DVBT2_GetPlpGroupID_PARAM.ret=false;

    if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_GetPlpGroupID, &Drv_DVBT2_GetPlpGroupID_PARAM);
    else
        return false;

    return Drv_DVBT2_GetPlpGroupID_PARAM.ret;

#if 0
    return INTERN_DVBT2_GetPlpGroupID(u8PlpID, u8GroupID);
#endif
}

MS_BOOL MDrv_DMD_DVBT2_SetPlpID(MS_U8 u8PlpID, MS_U8 u8GroupID)
{
    DMD_DBG(ULOGD("DEMOD","[drvDMD_INTERN_DVBT2.c]MDrv_DMD_DVBT2_SetPlpID\n"));

    DMD_DVBT2_SETPLPID_PARAM Drv_DVBT2_SetPlpID_PARAM;
    Drv_DVBT2_SetPlpID_PARAM.u8PlpID=u8PlpID;
    Drv_DVBT2_SetPlpID_PARAM.u8GroupID=u8GroupID;
    Drv_DVBT2_SetPlpID_PARAM.ret=false;

    if(u32DVBT2open==1)
        UtopiaIoctl(ppDVBT2Instant, DMD_DVBT2_DRV_CMD_SetPlpID, &Drv_DVBT2_SetPlpID_PARAM);
    else
        return false;

    return Drv_DVBT2_SetPlpID_PARAM.ret;

#if 0
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
#endif
}