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
#include <string.h>
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"

// Internal Definition
//#include "regCHIP.h"
//#include "regAVD.h"
//#include "mapi_tuner.h"
#include "drvSYS.h"
//#include "drvDMD_VD_MBX.h"
#include "drvDMD_EXTERN_MSB124x.h"
#include "include/drvDMD_common.h"
#include "include/drvSAR.h"
#include "drvMMIO.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define ERR_DEMOD_MSB(x)     x
#ifdef MS_DEBUG
#define DBG_DEMOD_MSB(x)      x
#define DBG_DEMOD_FLOW(x)     x
#define DBG_GET_SIGNAL(x)      // x
#define DBG_DEMOD_LOAD_I2C(x)       x
#define DBG_DEMOD_CHECKSUM(x)        // x
#define DBG_FLASH_WP(x)        // x
#endif
#define DBG_KIRIN_BOND(x)  //x

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define PRINTE(p) printf p

#define LOAD_CODE_I2C_BLOCK_NUM          0x400//0x80
#define REG_MB_CNTL     0x0C80
#define REG_MB_ADDR_L   0x0C84
#define REG_MB_ADDR_H   0x0C82
#define REG_MB_DATA     0x0C86
#define REG_FSM_EN       0x0CB8

#define resetDemodTime  50
#define waitFlashTime   50

#define SDRAM_DATA_CHECK                 0
#define SRAM_DATA_CHECK                0
#define SDRAM_BASE                       0x5000
#define SRAM_BASE                        0x8000
#define SPI_DEVICE_BUFFER_SIZE           256
#define MAX_MSB124X_LIB_LEN              131072

#define MSB124X_BOOT  0x01
#define MSB124X_DVBT2 0x02
#define MSB124X_DVBT  0x04
#define MSB124X_DVBC  0x08
#define MSB124X_DVBS2 0x10
#define MSB124X_ALL   0x0F

#define MSB124X_BOOT_START_ADDR     0x00000
#define MSB124X_BOOT_END_ADDR       0x007FF
#define MSB124X_DVBT2_P1_START_ADDR 0x00800
#define MSB124X_DVBT2_P1_END_ADDR   0x087FF
#define MSB124X_DVBT2_P2_START_ADDR 0x08800
#define MSB124X_DVBT2_P2_END_ADDR   0x0FFFF
#define MSB124X_DVBT_START_ADDR     0x10000
#define MSB124X_DVBT_END_ADDR       0x17FFF
#define MSB124X_DVBC_START_ADDR     0x18000
#define MSB124X_DVBC_END_ADDR       0x1FFFF
#define MSB124X_DVBS2_P1_START_ADDR    0x20000
#define MSB124X_DVBS2_P1_END_ADDR      0x27FFF
#define MSB124X_DVBS2_P2_START_ADDR    0x28000
#define MSB124X_DVBS2_P2_END_ADDR      0x28FFF


#define MSB124X_WINDOWS_BASE                0x100
#define MSB124X_BOOT_WINDOWS_OFFSET         MSB124X_WINDOWS_BASE
#define MSB124X_DVBT2_P2_WINDOWS_OFFSET    (MSB124X_WINDOWS_BASE + 0x08)
#define MSB124X_DVBT2_P1_WINDOWS_OFFSET    (MSB124X_DVBT2_P2_WINDOWS_OFFSET + 0x08)
#define MSB124X_DVBT_WINDOWS_OFFSET        (MSB124X_DVBT2_P1_WINDOWS_OFFSET + 0x08)
#define MSB124X_DVBC_WINDOWS_OFFSET        (MSB124X_DVBT_WINDOWS_OFFSET + 0x08)
#define MSB124X_DVBS2_P2_WINDOWS_OFFSET       (MSB124X_DVBC_WINDOWS_OFFSET + 0x08)
#define MSB124X_DVBS2_P1_WINDOWS_OFFSET       (MSB124X_DVBS2_P2_WINDOWS_OFFSET + 0x08)

#define MSB124X_MAX_FLASH_ON_RETRY_NUM 3

#define UNUSED(x)       (x=x)

/////////////// CONSTANT /////////////////
#define PAGE_WRITE_SIZE         256
#define VERSION_CODE_ADDR       0xFC0
#define VERSION_CODE_SIZE       32


//kirin bonding option
#define DRV_RIU_ReadByte(_u32addr)	(*(volatile MS_U32*)(_u32addr) )
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#if 1

#define DMD_LOCK() _Lock()
#define DMD_UNLOCK()  _UnLock()

#else
#define DMD_LOCK()      \
    do{                         \
        MS_ASSERT(MsOS_In_Interrupt() == FALSE); \
        if (eDMD_MSB124X_DbgLevel == E_DMD_MSB124X_DBGLV_DEBUG) printf("%s lock mutex\n", __FUNCTION__);\
        MsOS_ObtainMutex(pDemod->_s32DMD_Mutex, MSOS_WAIT_FOREVER);\
        }while(0)

#define DMD_UNLOCK()      \
    do{                         \
        MsOS_ReleaseMutex(pDemod->_s32DMD_Mutex);\
        if (eDMD_MSB124X_DbgLevel == E_DMD_MSB124X_DBGLV_DEBUG) printf("%s unlock mutex\n", __FUNCTION__); \
        }while(0)
#endif
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if 1
static MSIF_Version _drv_dmd_msb124x_extern_version = {
    .MW = { DMD_MSB124X_EXTERN_VER, },
};
#else
static MSIF_Version _drv_dmd_msb123x_extern_version;
#endif

#define DEMOD_MAX_INSTANCE 2
#define DEMOD_GET_ACTIVE_NODE() &_gDemodNode[_gActiveInstanceIndex]


#if (defined CONFIG_EXT_DVBS_DISABLE) && (CONFIG_EXT_DVBS_DISABLE==1)
MS_U8 MSB124X_LIB[]={
#include "msb124x_dvbt.dat"
};
#else
MS_U8 MSB124X_LIB[]={
#include "msb124x_dvbs_s2_t_t2.dat"
};
#endif

MS_U8 MSB1245_LIB[]=
{
#include "msb1245_dvbs.dat"
};
//configure
//--------------------------------------------------------------------- for DVB-T2
// BW: 0->1.7M, 1->5M, 2->6M, 3->7M, 4->8M, 5->10M
#define T2_BW_VAL               0x04

// FC: FC = FS = 4800 = 0x12C0     (4.80MHz IF)
#define T2_FC_L_VAL            0xC0    // 4.80M
#define T2_FC_H_VAL            0x12
#define T2_TS_SERIAL_VAL        0x00
#define T2_TS_CLK_RATE_VAL      0x06
#define T2_TS_OUT_INV_VAL       0x01
#define T2_TS_DATA_SWAP_VAL     0x00
#define T2_TS_ERR_POL_VAL       0x00

#define T2_TS_SPREAD_SPAN          0x00 //Unit: kHz (0: disable; default: 40 kHz)
#define T2_TS_SPREAD_STEP_SIZE    0x00 //Unit: percent(%) (0: disable; default: 3.0 %)

#define T2_TS_PHASE_TUNING_EN     0x00 //0: disable; 1: enable
#define T2_TS_PHASE_TUNING_NUM    0x00 //Unit: degree  <range: 0x00 ~ 0x1F>


MS_U8 MSB124X_DVBT2_DSPREG_TABLE[] =
{
    T2_BW_VAL, T2_FC_L_VAL, T2_FC_H_VAL,  T2_TS_SERIAL_VAL, T2_TS_CLK_RATE_VAL,
    T2_TS_OUT_INV_VAL, T2_TS_DATA_SWAP_VAL, T2_TS_ERR_POL_VAL,
    T2_TS_SPREAD_SPAN,T2_TS_SPREAD_STEP_SIZE,T2_TS_PHASE_TUNING_EN,T2_TS_PHASE_TUNING_NUM
};

//--------------------------------------------------------------------- for DVB-T
//operation

//configure
#define T_FC_L                    0xc0
#define T_FC_H                    0x12
#define T_FS_L                    0x80
#define T_FS_H                    0x70
#define T_BW                      0x03
#define T_IQ_SWAP                 0x00
#define T_SERIAL_TS               0x00
#define T_TS_CLK_SEL              0x06
#define T_TS_OUT_INV              0x01
#define T_TS_DATA_SWAP            0x00
#define T_IF_INV_PWM_OUT_EN  0x00

#define T_TS_SPREAD_SPAN          0x00   //Unit: kHz (0: disable; default: 40 kHz)
#define T_TS_SPREAD_STEP_SIZE    0x00   //Unit: percent(%) (0: disable; default: 3.0 %)

#define T_TS_PHASE_TUNING_EN     0x00 //0: disable; 1: enable
#define T_TS_PHASE_TUNING_NUM    0x00 //Unit: degree  <range: 0x00 ~ 0x1F>

MS_U8 MSB124X_DVBT_DSPREG_TABLE[] =
{
    T_BW,  T_FC_L,  T_FC_H,  T_SERIAL_TS,  T_TS_CLK_SEL,  T_TS_OUT_INV,  T_TS_DATA_SWAP,
    T_IQ_SWAP,  T_IF_INV_PWM_OUT_EN,
    T_TS_SPREAD_SPAN,T_TS_SPREAD_STEP_SIZE,T_TS_PHASE_TUNING_EN,T_TS_PHASE_TUNING_NUM
};

//--------------------------------------------------------------------- for DVB-C
#define     C_AUTO_SCAN_SYM_RATE    1
#define     C_AUTO_SCAN_QAM	        1
#define     C_IF_INV_PWM_OUT_EN     0
#define     C_ZIF                   0
#define     C_FC_L                  0x88
#define     C_FC_H                  0x13
#define     C_FS_L                  0xC0
#define     C_FS_H                  0x5D
#define     C_BW_L                  0xDB
#define     C_BW_H                  0x1A
#define     C_BW1_L                 0xF4
#define     C_BW1_H                 0x1A
#define     C_BW2_L                 0xDB
#define     C_BW2_H                 0x1A
#define     C_BW3_L                 0x58
#define     C_BW3_H                 0x1B
#define     C_QAM                   2
#define     C_CCI                   0
#define     C_TS_SERIAL             0
#define     C_TS_CLK_RATE           6
#define     C_TS_OUT_INV            1
#define     C_TS_DATA_SWAP          0
#define     C_IQ_SWAP               0
#define     C_TS_SPREAD_SPAN          0x00 //Unit: kHz (0: disable; default: 40 kHz)
#define     C_TS_SPREAD_STEP_SIZE    0x00 //Unit: percent(%) (0: disable; default: 3.0 %)

#define     C_TS_PHASE_TUNING_EN     0x00 //0: disable; 1: enable
#define     C_TS_PHASE_TUNING_NUM    0x00 //Unit: degree  <range: 0x00 ~ 0x1F>

MS_U8 MSB124X_DVBC_DSPREG_TABLE[] =
{
    C_AUTO_SCAN_SYM_RATE,  //0x20
    C_AUTO_SCAN_QAM,
    C_IF_INV_PWM_OUT_EN,
    C_ZIF,
    C_FC_L,
    C_FC_H,
    C_FS_L,
    C_FS_H,

    C_BW_L,             //0x28
    C_BW_H,
    C_BW1_L,
    C_BW1_H,
    C_BW2_L,
    C_BW2_H,
    C_BW3_L,
    C_BW3_H,

    C_QAM,               //0x30
    C_CCI,
    C_TS_SERIAL,
    C_TS_CLK_RATE,
    C_TS_OUT_INV,
    C_TS_DATA_SWAP,
    C_IQ_SWAP,
};

typedef struct xMSB124XData
{
    MS_BOOL Active;
    MS_BOOL bDMD_MSB124X_Power_init_en;
    MS_U8 u8DMD_MSB124X_PowerOnInitialization_Flow;
    MS_U8 u8DMD_MSB124X_Sdram_Code;
    MS_U8 u8DMD_MSB124X_Sram_Code;
    sDMD_MSB124X_InitData _sDMD_MSB124X_InitData;
    eDMD_MSB124X_DemodulatorType eDMD_MSB124X_CurrentDemodulatorType;
    MS_BOOL bDemodRest;
    MS_U8 DVBT2_DSP_REG[sizeof(MSB124X_DVBT2_DSPREG_TABLE)];
    MS_U8 DVBT_DSP_REG[sizeof(MSB124X_DVBT_DSPREG_TABLE)];
    MS_U8 DVBC_DSP_REG[sizeof(MSB124X_DVBC_DSPREG_TABLE)];
}tMSB124XData;

static MS_S32 _gActiveInstanceIndex = 0;
static eDMD_MSB124X_DbgLv eDMD_MSB124X_DbgLevel=E_DMD_MSB124X_DBGLV_NONE;
static tMSB124XData _gDemodNode[DEMOD_MAX_INSTANCE]=
{
  {FALSE, FALSE, 0, 0, 0, {0}, E_DMD_MSB124X_DEMOD_DVBT,TRUE,{0},{0},{0}},
  {FALSE, FALSE, 0, 0, 0, {0}, E_DMD_MSB124X_DEMOD_DVBT,TRUE,{0},{0},{0}},
};

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define DMD_DBG(x)          (x)
#else
#define DMD_DBG(x)          //(x)
#endif
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_S32 _s32DMD_Mutex = -1;
static MS_S32 _s32LockOwner = -1;
static MS_S32 _sLockCount = 0;
static void _Lock(void)
{
    MS_S32 s32CurTaskId = MsOS_GetOSThreadID();

    if(_s32DMD_Mutex == -1)
    {
        return;
    }

    if (_s32LockOwner == s32CurTaskId)
    {
         _sLockCount++;
    }
    else
    {

        MsOS_ObtainMutex(_s32DMD_Mutex, MSOS_WAIT_FOREVER);
        //printf("m_CircularMutex lock\n");
        _sLockCount = 1;
        _s32LockOwner  = s32CurTaskId;
    }
}
static void _UnLock(void)
{
    MS_S32 s32CurTaskId = MsOS_GetOSThreadID();

    if(_s32DMD_Mutex == -1)
    {
        return;
    }
    if (s32CurTaskId == _s32LockOwner)
    {
       if (--_sLockCount == 0)
       {
           _s32LockOwner = -1;
           //printf("m_CircularMutex unlock\n");
           MsOS_ReleaseMutex(_s32DMD_Mutex);
       }
    }

}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
#define INDEX_TO_HANDLE(x) ((x)+1)
#define HANDLE_TO_INDEX(x) ((x)-1)
//If using legacy API, mutex is created in Init and deleted in Exit
//If using New API(EX API), mutex is created in CreateNode and deleted in DeleteNode
//Add _CreateMutex, called in Init and CreateNode
static MS_BOOL _CreateMutex(void)
{
    if (_s32DMD_Mutex == -1)
    {
        _sLockCount = 0;
        _s32LockOwner = -1;
        _s32DMD_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "_utopia_124x", MSOS_PROCESS_SHARED);
        if (_s32DMD_Mutex == -1)
        {
             DMD_DBG(printf(" Create Mutex Fail\n"));
             return FALSE;
        }
    }
    return TRUE;
}
//Add _DeleteMutex, called in Exit and DeleteNode

static MS_BOOL _DeleteMutex(void)
{
    int i=0;
    for(i=0; i<DEMOD_MAX_INSTANCE; i++)
    {
        //still having active instance, no need to DeleteMutex
        if(_gDemodNode[i].Active == TRUE)
        {
            return TRUE;
        }
    }
    if (_s32DMD_Mutex != -1)
    {
        MsOS_DeleteMutex(_s32DMD_Mutex);
        _s32DMD_Mutex = -1;
        _sLockCount = 0;
        _s32LockOwner = -1;
    }
    return TRUE;
}

extern MS_BOOL MDrv_DMD_MSB124X_SwitchHandle(MS_S32 s32Handle)
{
    MS_S32 s32Index = HANDLE_TO_INDEX(s32Handle);
    if(s32Index>=DEMOD_MAX_INSTANCE)
        return FALSE;
    if(s32Index<0)
        return FALSE;
    _gActiveInstanceIndex =s32Index;
    return TRUE;
}
extern MS_S32 MDrv_DMD_MSB124X_GetCurrentHandle(void)
{
    return INDEX_TO_HANDLE(_gActiveInstanceIndex);
}
extern MS_BOOL MDrv_MDM_MSB124X_CreateNode(MS_S32 *s32Handle)
{
    int i=0;
    if(_CreateMutex() == FALSE)
    {
        return FALSE;
    }
    for(i=0; i<DEMOD_MAX_INSTANCE; i++)
    {
        if(_gDemodNode[i].Active == FALSE)
        {
            _gDemodNode[i].Active  = TRUE;
            *s32Handle = INDEX_TO_HANDLE(i);
            return TRUE;
        }
    }
    printf("Slot Full\n");
    return FALSE;
}
extern MS_BOOL MDrv_MDM_MSB124X_DeleteNode(MS_S32 s32Handle)
{
    MS_S32 s32Index = HANDLE_TO_INDEX(s32Handle);
    if(s32Index>=DEMOD_MAX_INSTANCE)
        return FALSE;
    if(s32Index<0)
        return FALSE;
    if(_gDemodNode[s32Index].Active == TRUE)
    {
        _gDemodNode[s32Index].Active  = FALSE;
        _DeleteMutex();
        return TRUE;
    }
    return FALSE;
}


MS_BOOL MDrv_DMD_MSB124X_Init(sDMD_MSB124X_InitData *pDMD_MSB124X_InitData, MS_U32 u32InitDataLen)
{
    //char pDMD_MSB124X_MutexString[16], pDMD_MSB124X_MutexString_RegRW[16], pDMD_MSB124X_MutexString_DSPRegRW[16];
    //char pDMD_MSB124X_MutexString[16];
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    //if (NULL == strncpy(pDMD_MSB124X_MutexString,"Mutex DMD DVB",16))
    //{
       // DMD_DBG(printf("MDrv_DMD_MSB124X_Init strcpy Fail\n"));
        //return FALSE;
    //}

    if(_CreateMutex() == FALSE)
    {
        DMD_DBG(printf("MDrv_DMD_MSB124X_Init Create Mutex Fail\n"));
        return FALSE;
    }

    DMD_LOCK();
    if ( sizeof(sDMD_MSB124X_InitData) == u32InitDataLen)
    {
        memcpy(&pDemod->_sDMD_MSB124X_InitData, pDMD_MSB124X_InitData, u32InitDataLen);
    }
    else
    {
        DMD_DBG(printf("MDrv_DMD_MSB124X_Init input data structure incorrect\n"));
        DMD_UNLOCK();
        return FALSE;
    }

    if (pDemod->_sDMD_MSB124X_InitData.pDVBC_DSP_REG!= NULL)
    {
        printf("MDrv_DMD_MSB124X_Init Init DVBC DSP Table By Device Driver ... \n");
        #if 1

        memcpy (&pDemod->DVBC_DSP_REG[0],
                pDemod->_sDMD_MSB124X_InitData.pDVBC_DSP_REG ,
                sizeof(MSB124X_DVBC_DSPREG_TABLE));
        #else
        memcpy (&MSB124X_DVBC_DSPREG_TABLE[0],
                pDemod->_sDMD_MSB124X_InitData.pDVBC_DSP_REG ,
                sizeof(MSB124X_DVBC_DSPREG_TABLE));
        #endif
    }
    else
    {
        #if 1

        memcpy (&pDemod->DVBC_DSP_REG[0],
                MSB124X_DVBC_DSPREG_TABLE ,
                sizeof(MSB124X_DVBC_DSPREG_TABLE));
        #endif
        printf("MDrv_DMD_MSB124X_Init Init DVBC DSP Table By Device Driver ... \n");
    }

    if (pDemod->_sDMD_MSB124X_InitData.pDVBT_DSP_REG!= NULL)
    {
        printf("MDrv_DMD_MSB124X_Init Init DVBT DSP Table By Device Driver ... \n");
        #if 1
        memcpy (&pDemod->DVBT_DSP_REG[0],
                pDemod->_sDMD_MSB124X_InitData.pDVBT_DSP_REG ,
                sizeof(MSB124X_DVBT_DSPREG_TABLE));
        #else
        memcpy (&MSB124X_DVBT_DSPREG_TABLE[0],
                pDemod->_sDMD_MSB124X_InitData.pDVBT_DSP_REG ,
                sizeof(MSB124X_DVBT_DSPREG_TABLE));
        #endif
    }
    else
    {
        #if 1
        memcpy (&pDemod->DVBT_DSP_REG[0],
                MSB124X_DVBT_DSPREG_TABLE ,
                sizeof(MSB124X_DVBT_DSPREG_TABLE));
        #endif
        printf("MDrv_DMD_MSB124X_Init Init DVBT DSP Table By Device Driver ... \n");
    }

    if (pDemod->_sDMD_MSB124X_InitData.pDVBT2_DSP_REG!= NULL)
    {
        printf("MDrv_DMD_MSB124X_Init Init DVBT2 DSP Table By Device Driver ... \n");

        #if 1
        memcpy (&pDemod->DVBT2_DSP_REG[0],
                pDemod->_sDMD_MSB124X_InitData.pDVBT2_DSP_REG ,
                sizeof(MSB124X_DVBT2_DSPREG_TABLE));
        #else
        memcpy (&MSB124X_DVBT2_DSPREG_TABLE[0],
                pDemod->_sDMD_MSB124X_InitData.pDVBT2_DSP_REG ,
                sizeof(MSB124X_DVBT2_DSPREG_TABLE));
        #endif
    }
    else
    {
        #if 1
        memcpy (&pDemod->DVBT2_DSP_REG[0],
                MSB124X_DVBT2_DSPREG_TABLE,
                sizeof(MSB124X_DVBT2_DSPREG_TABLE));
        #endif
        printf("MDrv_DMD_MSB124X_Init Init DVBT2 DSP Table By Device Driver ... \n");
    }

    if (pDemod->_sDMD_MSB124X_InitData.bEnableSPILoadCode &&
        pDemod->_sDMD_MSB124X_InitData.fpMSB124x_SPIPAD_En != NULL)
    {
        if (!MDrv_DMD_SSPI_Init(0))
        {
            printf("MDrv_DMD_MSB124x_Init Init MDrv_DMD_SSPI_Init Fail \n");
        }
    }
    else
    {
        pDemod->_sDMD_MSB124X_InitData.bEnableSPILoadCode = FALSE;
    }

    //eDMD_MSB124X_DbgLevel = E_DMD_MSB124X_DBGLV_DEBUG;
    #ifdef MS_DEBUG
    if (eDMD_MSB124X_DbgLevel >= E_DMD_MSB124X_DBGLV_INFO)
    {
        printf("MDrv_DMD_MSB124X_Init %d\n", _gActiveInstanceIndex);
    }
    #endif

    DMD_UNLOCK();
    return TRUE;
}

MS_BOOL MDrv_DMD_MSB124X_Exit(void)
{
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();
    #ifdef MS_DEBUG
    if (eDMD_MSB124X_DbgLevel >= E_DMD_MSB124X_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_MSB124X_Exit %d\n", _gActiveInstanceIndex);
    }
    #endif
    pDemod->bDMD_MSB124X_Power_init_en = FALSE;
    pDemod->u8DMD_MSB124X_PowerOnInitialization_Flow = 0;
    pDemod->u8DMD_MSB124X_Sdram_Code = 0x0;
    pDemod->u8DMD_MSB124X_Sram_Code  = 0x0;
    pDemod->bDemodRest = TRUE;
    _DeleteMutex();
    return TRUE;
}

MS_BOOL MDrv_DMD_MSB124X_SetDbgLevel(eDMD_MSB124X_DbgLv u8DbgLevel)
{
    //DMD_LOCK();
    eDMD_MSB124X_DbgLevel = u8DbgLevel;
    //DMD_UNLOCK();
    return TRUE;
}


MS_BOOL MDrv_DMD_MSB124X_GetLibVer(const MSIF_Version **ppVersion)
{
    //tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();
    //DMD_LOCK();
    if (!ppVersion)
    {
      //  DMD_UNLOCK();
        return FALSE;
    }

    *ppVersion = &_drv_dmd_msb124x_extern_version;
    //DMD_UNLOCK();
    return TRUE;
}

MS_BOOL MDrv_DMD_MSB124X_GetFWVer(MS_U16 *ver)
{

    MS_BOOL bRet=TRUE;

    //DMD_LOCK();
    //printf("MDrv_DMD_DVBT_GetFWVer %x\n",*ver);
    //DMD_UNLOCK();

    return bRet;

}

static MS_BOOL _MDrv_DMD_MSB124X_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6];
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr & 0xff;

    u8MsbData[0] = 0x35;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8MsbData);
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_READ_BYTES, 0, NULL, 1, pu8Data);

    u8MsbData[0] = 0x34;
    bRet=(pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

    #ifdef MS_DEBUG
    if (eDMD_MSB124X_DbgLevel >= E_DMD_MSB124X_DBGLV_DEBUG)
    {
        printf("_MDrv_DMD_MSB124X_GetReg %x %x\n", u16Addr, *pu8Data);
    }
    #endif

    return bRet;
}

MS_BOOL MDrv_DMD_MSB124X_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet=TRUE;

    DMD_LOCK();
    bRet = _MDrv_DMD_MSB124X_GetReg(u16Addr, pu8Data);
    DMD_UNLOCK();

    return bRet;
}

static MS_BOOL _MDrv_DMD_MSB124X_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6];
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();
    #ifdef MS_DEBUG
    if (eDMD_MSB124X_DbgLevel >= E_DMD_MSB124X_DBGLV_DEBUG)
    {
        printf("_MDrv_DMD_MSB124X_SetReg %x %x\n", u16Addr, u8Data);
    }
    #endif

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;
    u8MsbData[5] = u8Data;

    u8MsbData[0] = 0x35;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 6, u8MsbData);

    u8MsbData[0] = 0x34;
    bRet=(pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);
    return bRet;
}

MS_BOOL MDrv_DMD_MSB124X_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;

    DMD_LOCK();
    bRet = _MDrv_DMD_MSB124X_SetReg(u16Addr, u8Data);
    DMD_UNLOCK();

    return bRet;
}


static MS_BOOL _MDrv_DMD_MSB124X_SetRegs(MS_U16 u16Addr, MS_U8* u8pData, MS_U16 data_size)
{
    MS_BOOL bRet=TRUE;
    MS_U8   u8MsbDataValue[LOAD_CODE_I2C_BLOCK_NUM + 5];
    MS_U16   idx = 0;
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();
    #ifdef MS_DEBUG
    if (eDMD_MSB124X_DbgLevel >= E_DMD_MSB124X_DBGLV_DEBUG)
    {
        printf("_MDrv_DMD_MSB124X_SetRegs %x %x\n", u16Addr, data_size);
    }
    #endif

    u8MsbDataValue[0] = 0x10;
    u8MsbDataValue[1] = 0x00;
    u8MsbDataValue[2] = 0x00;
    u8MsbDataValue[3] = (u16Addr >> 8) &0xff;
    u8MsbDataValue[4] = u16Addr & 0xff;
    // u8MsbDataValue[5] = 0x00;

    for(idx = 0; idx < data_size ; idx++)
    {
        u8MsbDataValue[5+idx] = u8pData[idx];
    }

    u8MsbDataValue[0] = 0x35;
    bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbDataValue);

    u8MsbDataValue[0] = 0x10;
    bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5 + data_size, u8MsbDataValue);

    u8MsbDataValue[0] = 0x34;
    bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbDataValue);

    return bRet;
}

MS_BOOL MDrv_DMD_MSB124X_SetRegs(MS_U16 u16Addr, MS_U8* u8pData, MS_U16 data_size)
{
    MS_BOOL bRet=TRUE;

    DMD_LOCK();
    bRet = _MDrv_DMD_MSB124X_SetRegs(u16Addr, u8pData, data_size);
    DMD_UNLOCK();

    return bRet;
}

static MS_BOOL _MDrv_DMD_MSB124X_SetReg2Bytes(MS_U16 u16Addr, MS_U16 u16Data)
{
    MS_BOOL bRet=TRUE;

    #ifdef MS_DEBUG
    if (eDMD_MSB124X_DbgLevel >= E_DMD_MSB124X_DBGLV_DEBUG)
    {
        printf("_MDrv_DMD_MSB124X_SetReg2Bytes %x %x\n", u16Addr, u16Data);
    }
    #endif

    bRet &= _MDrv_DMD_MSB124X_SetReg(u16Addr, (MS_U8)u16Data&0x00ff);
    bRet &= _MDrv_DMD_MSB124X_SetReg(u16Addr + 0x0001, (MS_U8)(u16Data>>8)&0x00ff);

    return bRet;
}

MS_BOOL MDrv_DMD_MSB124X_SetReg2Bytes(MS_U16 u16Addr, MS_U16 u16Data)
{
    MS_BOOL bRet=TRUE;

    DMD_LOCK();
    bRet = _MDrv_DMD_MSB124X_SetReg2Bytes(u16Addr, u16Data);
    DMD_UNLOCK();

    return bRet;
}


MS_BOOL _MDrv_DMD_MSB124X_GetDSPReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8     u8Cntl = 0x00;
    MS_U16    u16Cntr = 0x00;

    bRet &= _MDrv_DMD_MSB124X_SetReg(REG_MB_ADDR_H, (MS_U8)(u16Addr >> 8));
    bRet &= _MDrv_DMD_MSB124X_SetReg(REG_MB_ADDR_L, (MS_U8)(u16Addr));
    bRet &= _MDrv_DMD_MSB124X_SetReg(REG_MB_CNTL, 0x03);

    do
    {
        bRet &= _MDrv_DMD_MSB124X_GetReg(REG_MB_CNTL, &u8Cntl);
        if (u16Cntr++ > 0x7ff)
        {
            #ifdef MS_DEBUG
            //if (eDMD_MSB124X_DbgLevel >= E_DMD_MSB124X_DBGLV_DEBUG)
            {
                printf("MSB124X_MB_READ_FAILURE\n");
            }
            #endif
            return FALSE;
        }
    }
    while(u8Cntl != 0xff);

    bRet &= _MDrv_DMD_MSB124X_GetReg(REG_MB_DATA, pu8Data);

    #ifdef MS_DEBUG
    if (eDMD_MSB124X_DbgLevel >= E_DMD_MSB124X_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_MSB124X_GetDSPReg %x %x\n", u16Addr, *pu8Data);
    }
    #endif

    return bRet;
}

MS_BOOL MDrv_DMD_MSB124X_GetDSPReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet=TRUE;

    DMD_LOCK();
    bRet = _MDrv_DMD_MSB124X_GetDSPReg(u16Addr, pu8Data);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL _MDrv_DMD_MSB124X_SetDSPReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8     u8Cntl = 0x00;
    MS_U16    u16Cntr = 0x00;

    #ifdef MS_DEBUG
    if (eDMD_MSB124X_DbgLevel >= E_DMD_MSB124X_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_MSB124X_SetDSPReg %x %x\n", u16Addr, u8Data);
    }
    #endif

    bRet &= _MDrv_DMD_MSB124X_SetReg(REG_MB_DATA, u8Data);
    bRet &= _MDrv_DMD_MSB124X_SetReg(REG_MB_ADDR_H, (MS_U8)(u16Addr >> 8));
    bRet &= _MDrv_DMD_MSB124X_SetReg(REG_MB_ADDR_L, (MS_U8)(u16Addr));
    bRet &= _MDrv_DMD_MSB124X_SetReg(REG_MB_CNTL, 0x04);

    do
    {
        bRet &= _MDrv_DMD_MSB124X_GetReg(REG_MB_CNTL, &u8Cntl);
        if (u16Cntr++ > 0x7ff)
        {
            #ifdef MS_DEBUG
            //if (eDMD_MSB124X_DbgLevel >= E_DMD_MSB124X_DBGLV_DEBUG)
            {
                printf("MSB124X_MB_WRITE_FAILURE\n");
            }
            #endif
            return false;
        }
    }
    while(u8Cntl != 0xff);
    return bRet;
}

MS_BOOL MDrv_DMD_MSB124X_SetDSPReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;

    DMD_LOCK();
    bRet = _MDrv_DMD_MSB124X_SetDSPReg(u16Addr, u8Data);
    DMD_UNLOCK();

    return bRet;
}

static MS_BOOL _MSB124X_I2C_CH_Reset(MS_U8 ch_num)
{
    MS_BOOL bRet=TRUE;
    //MAPI_U8         addr[4] = {0x00, 0x00, 0x00, 0x00};
    MS_U8         u8data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    #ifdef MS_DEBUG
    printf("[msb124x][beg]I2C_CH_Reset, CH=0x%x\n",ch_num);
    #endif
    //DMD_LOCK_REG_RW();

    // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
    //u8data[0] = 0x53;
    //bRet &= (*_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8data);
    if (pDemod->bDemodRest)
    {
        pDemod->bDemodRest = FALSE;
        // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
        u8data[0] = 0x53;
        // Don't check Ack because this passward only ack one time for the first time.
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8data);
    }
    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_
    u8data[0] = 0x71;
    bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8data);

    // 8'hb2(SRID)->8,h81(CMD)  //TV.n_iic_sel_b0
    u8data[0] = ((ch_num & 0x01) != 0)? 0x81 : 0x80;
    bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8data);

    // 8'hb2(SRID)->8,h83(CMD)  //TV.n_iic_sel_b1
    u8data[0] = ((ch_num & 0x02) != 0)? 0x83 : 0x82;
    bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8data);

    // 8'hb2(SRID)->8,h84(CMD)  //TV.n_iic_sel_b2
    u8data[0] = ((ch_num & 0x04) != 0)? 0x85 : 0x84;
    bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8data);

    // 8'hb2(SRID)->8,h53(CMD)  //TV.n_iic_ad_byte_en2, 32bit read/write
    u8data[0] = 0x53;
    bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8data);

    // 8'hb2(SRID)->8,h7f(CMD)  //TV.n_iic_sel_use_cfg
    u8data[0] = 0x7f;
    bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8data);

/*
    // 8'hb2(SRID)->8,h35(CMD)  //TV.n_iic_use
    data[0] = 0x35;
    iptr->WriteBytes(0, NULL, 1, data);

    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_Re-shape
    data[0] = 0x71;
    iptr->WriteBytes(0, NULL, 1, data);
*/
    //DMD_UNLOCK_REG_RW();
    #ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][end]I2C_CH_Reset, CH=0x%x\n",ch_num));
    #endif
    return bRet;
}

static MS_BOOL _MSB124X_HW_init(void)
{
    MS_BOOL bRet = true;
    MS_U8 u8_tmp = 0;
    MS_U8 u8_timeout = 0;

    #ifdef MS_DEBUG
    printf("[msb124x][beg]MSB124X_HW_init\n");
    #endif
    //DMD_LOCK();

    // ASIC INIT for WDM DVB-T2
    // -------------------------------------------------------------------
    // Initialize DMD_ANA_MISC
    // -------------------------------------------------------------------
    // [0]	reg_tst_ldo25i
    // [1]	reg_tst_ldo25q
    // [5:4]	reg_tst_ldo25i_selfb
    // [7:6]	reg_tst_ldo25q_selfb
    // [8]	reg_pd_dm2p5ldoi = 1'b0
    // [9]	reg_pd_dm2p5ldoq = 1'b0
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x4f)*2, 0x0000);

    // [0]	reg_tst_ldo11_clk
    // [1]	reg_tst_ldo26
    // [2]	reg_tst_ldo11_cmp
    // [3]	reg_pd_dm1p1ldo_clk = 1'b0
    // [4]	reg_pd_dm1p1ldo_cmp = 1'b0
    // [6]	reg_tst_ldo26_selfb
    // [7]	reg_pd_dm2p6ldo = 1'b0
    // [9:8]	reg_tst_ldo11_cmp_selfb
    // [11:10]	reg_tst_ldo11_clk_selfb
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x4e)*2, 0x0000);

    // [1:0]	reg_mpll_loop_div_first       feedback divider 00:div by 1 01:div by 2 10:div by 4 11:div by 8
    // [15:8]	reg_mpll_loop_div_second      feedback divider, div by binary data number
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x33)*2, 0x1201);

    // [2:0]	reg_mpll_ictrl		    charge pump current control
    // [3]	reg_mpll_in_sel		    1.8V or 3.3V reference clock domain select (1'b0=0==>3.3 V reference clock domain)
    // [4]	reg_mpll_xtal2adc_sel	    select the XTAL clock bypass to MPLL_ADC_CLK
    // [5]	reg_mpll_xtal2next_pll_sel  crystal clock bypass to next PLL select
    // [6]	reg_mpll_vco_offset	    set VCO initial offset frequency
    // [7]	reg_mpll_pd		    gated reference clock and power down PLL analog_3v: 1=power down
    // [8]	reg_xtal_en		    XTAL enable register; 1: enable
    // [10:9]	reg_xtal_sel		    XTAL driven strength select.
    // [11]  	reg_mpll_porst		    MPLL input  power on reset, connect to reg as MPLL_RESET
    // [12]  	reg_mpll_reset		    PLL software reset; 1:reset
    // [13]  	reg_pd_dmpll_clk	    XTAL to MPLL clock reference power down
    // [14]  	reg_pd_3p3_1		    XTAL to CLK_24M_3P3_1 power down
    // [15]  	reg_pd_3p3_2		    XTAL to CLK_24M_3P3_2 power down
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x35)*2, 0x1803);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x35)*2, 0x0003);

    // [0]	reg_mpll_clk_dp_pd	dummy
    // [1]	reg_adc_clk_pd		ADC output clock power down
    // [2]	reg_mpll_div2_pd	MPLL_DIV2 power down
    // [3]	reg_mpll_div3_pd	MPLL_DIV3 power down
    // [4]	reg_mpll_div4_pd	MPLL_DIV4 power down
    // [5]	reg_mpll_div8_pd	MPLL_DIV8 power down
    // [6]	reg_mpll_div10_pd	MPLL_DIV10 power down
          // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h30, 2'b11, 16'h2400);  // divide ADC clock to 24Mhz = 24*36/36
          bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x30)*2, 0x2400);

    // $display("--------------------------------------");
    // $display("Initialize ADC I/Q");
    // $display("--------------------------------------");

    // [0]	Q channel ADC power down
    // [1]	I channel ADC power down
    // [2]	Q channel clamp enable. 0:enable, 1:disable
    // [3]	I channel clamp enable. 0:enable, 1:disable
    // [6:4]    I channel input mux control;
    //		3'b000=I channel ADC calibration mode input
    //	    	3'b001=VIF signal from VIFPGA
    //	    	3'b100=DVB or ATSC mode input from PAD_I(Q)P(M)
    //	    	all the other combination are only for test mode, don't use without understanding.
    // [10:8]   Q channel input mux control;
    //		3'b000=Q channel ADC calibration mode input
    //	    	3'b001=VIF signal from VIFPGA 3'b010 = SSIF signal from PAD_SIFP(M)
    //	    	3'b100=DVB or ATSC mode input from PAD_I(Q)P(M)
    //	    	all the other combination are only for test mode, don't use without understanding.
    // [12]	ADC I,Q swap enable; 1: swap
    // [13]	ADC clock out select; 1: ADC_CLKQ
    // [14]	ADC linear calibration bypass enable; 1:enable
    // [15]	ADC internal 1.2v regulator control always 0 in T3
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x01)*2, 0x0440);

    // [2:0]	reg_imuxs_s
    // [6:4]	reg_qmuxs_s
    // [9:8]	reg_iclpstr_s
    // [13:12]	reg_qclpstr_s
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x45)*2, 0x0000);

        //bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x51)*2, 0x0021);

    // [0]	Channel I ADC power down: 1=power dwon
    // [1]	Channel Q ADC power down: 1=power dwon
    // [2]	power down clamp buffer for test mode
    // [3]	change ADC reference voltage for SSIF
    // [6:4]    ADC source bias current control
    // [9:8]    XTAL receiver amp gain
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x0c)*2, 0x0002);

    // [0]	reg_linear_cal_start_q	0	0	1
    // [1]	reg_linear_cal_mode_q	0	0	1
    // [2]	reg_linear_cal_en_q	0	0	1
    // [3]	reg_linear_cal_code0_oren_q	0	0	1
    // [6:4]	reg_linear_cal_status_sel_q	2	0	3
    // [7]	reg_pwdn_vcalbuf	0	0	1
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x0f)*2, 0x0000);

    // [3:0]	clamp voltage control
    //          3'b000 = 0.7v
    //          3'b001 = 0.75v
    //          3'b010 = 0.5v
    //          3'b011 = 0.4v
    //          3'b100 = 0.8v
    //          3'b101 = 0.9v
    //          3'b110 = 0.65v
    //          3'b111 = 0.60v
    // [4]	REFERENCE power down
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x20)*2, 0x0000);

    // Set ADC gain is 1
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x0b)*2, 0x0909);

    // Disable ADC Sign bit
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x2e)*2, 0x0000);

    // ADC I channel offset
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x2a)*2, 0x0c00);

    // ADC Q channel offset
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x2b)*2, 0x0c00);

    // [2:0]reg_acl_ref
    // [5:4]reg_acl_isel
    // [8]	reg_xtal_pm_isel
    // [9]	reg_bond_mode
    // [10]	reg_clk_bond_mode
    // [11]	reg_clk_usb_3p3_en
    // [13:12]	reg_iq_ctrl	= 2'd1
	bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x19)*2, 0x1e00);

    // [ 4:0]reg_ckg_bist[4:0]
    // [11:8]reg_ckg_adcd_d2[3:0]
	bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x1c)*2, 0x0000);

    // [ 4:0]reg_ckg_dvbtm_sram_t11x_t22x[4:0]
    // [12:8]reg_ckg_dvbtm_sram_t11x_t24x[4:0]
	bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x1e)*2, 0x0000);


    // -------------------------------------------------------------------
    // Release clock gating
    // -------------------------------------------------------------------

    // [0]	reg_xtal_en
    // [9:8]	reg_clk_pd_iic
    // [10]	reg_clk_pd_all
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x09)*2, 0x0101);

    // [3:0]	reg_ckg_adcd
    // [7:4]	reg_ckg_sadc
    // [11:8]	reg_ckg_iicm
    // [13:12]	reg_ckg_sbus
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x0a)*2, 0x0000);

    // [5:0]	reg_ckg_mcu
    // [6]	reg_ckg_live
    // [11:8]	reg_ckg_inner
//      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x0b)*2, 0x0030);
    //set MCU ckg to 108MHz by jason
    // reg_ckg_mcu[4:2] = 0x0
    // reg_ckg_mcu[5] = 0x0
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x0b)*2, 0x0020);
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x0b)*2, 0x0000);

    // @0x0910
    // [3:0]	reg_ckg_dvbtm_adc
    // [6:4]	reg_ckg_dvbt_inner1x
    // [10:8]	reg_ckg_dvbt_inner2x
    // [14:12]	reg_ckg_dvbt_inner4x
    //DVBT2
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x10)*2, 0x1110);

    // @0x0911
    // [2:0]	reg_ckg_dvbt_outer1x
    // [6:4]	reg_ckg_dvbt_outer2x
    // [11:8]	reg_ckg_dvbtc_outer2x
    //DVBT2
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x11)*2, 0x0111);

    // @0x0912
    // [3:0]	reg_ckg_dvbtm_ts
    // [4]	reg_dvbtm_ts_out_mode
    // [5]	reg_dvbtm_ts_clk_pol
    // [15:8]	reg_dvbtm_ts_clk_divnum
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x12)*2, 0x1418);

    // @0x0913
    // [5:0]	reg_ckg_spi
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x13)*2, 0x0020);

    // @0x0914
    // [12:8]	reg_ckg_dvbtm_sram_t1o2x_t22x
    //DVBT2
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x14)*2, 0x0000);

    // @0x0915
    // [3:0]	reg_ckg_dvbc_inner
    // [6:4]	reg_ckg_dvbc_eq
    // [10:8]	reg_ckg_dvbc_eq8x
    //DVBT2
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x15)*2, 0x0111);

    // @0x0916
    // [8:4]	reg_ckg_dvbtm_sram_adc_t22x
    //DVBT2
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x16)*2, 0x0001);

    // @0x0917
    // [4:0]	reg_ckg_dvbtm_sram_t12x_t22x
    // [12:8]	reg_ckg_dvbtm_sram_t12x_t24x
    //DVBT2
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x17)*2, 0x0000);

    // @0x0918
    // [4:0]	reg_ckg_dvbtm_sram_t14x_t24x
    // [12:8]	reg_ckg_dvbtm_ts_in
    //DVBT2
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x18)*2, 0x0400);

    // @0x0919
    // [2:0]	reg_ckg_tdp_jl_inner1x
    // [6:4]	reg_ckg_tdp_jl_inner4x
    // [15:8]	reg_ckg_miu
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x19)*2, 0x3c00);

    // @0x091a
    // [6:4]	reg_ckg_dvbt2_inner1x
    // [10:8]	reg_ckg_dvbt2_inner2x
    // [14:12]	reg_ckg_dvbt2_inner4x
     //DVBT2
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x1a)*2, 0x0000);

    // @0x090e
    // [3:0]	reg_ckg_dvbs2_ldpc_inner_sram
    // [7:4]	reg_ckg_dvbs_viterbi_sram
    //DVBT2
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x0e)*2, 0x0000);

    // @0x091b
    // [2:0]	reg_ckg_dvbt2_outer1x
    // [6:4]	reg_ckg_dvbt2_outer2x
    // [10:8]	reg_ckg_syn_miu
    // [14:12]	reg_ckg_syn_ts
    //DVBT2
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x1b)*2, 0x0000);

    // @0x091c
    // [4:0]	reg_ckg_bist
    // [11:8]	reg_ckg_adcd_d2
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x1c)*2, 0x0000);

    // @0x091d
    // [3:0]	reg_ckg_dvbtm_adc_eq_1x
    // [7:4]	reg_ckg_dvbtm_adc_eq_0p5x
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x1d)*2, 0x0000);

    // @0x0921
    // [2:0]	reg_ckg_tdp_teq_inner1x
    // [14:12]	reg_ckg_tdp_teq_inner2x
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x21)*2, 0x0000);

    // @0x0922
    // [3:0]	reg_ckg_dvbt_t2_inner0p5x_dvbc_eq1x
    // [7:4]	reg_ckg_dvbt_t2_inner2x_dvbc_eq4x
    // [11:8]	reg_ckg_dvbt_t2_inner1x
    //DVBT2
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x22)*2, 0x0000);

    // [1:0]	reg_iicm_pad_sel
    // [4]	reg_i2c_sbpm_en
    // [12:8]	reg_i2c_sbpm_idle_num
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x08)*2, 0x0a01);

    // [8]	reg_turn_off_pad
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x28)*2, 0x0000);

    // @0x0970
    // [3:0]	reg_ckg_dvbt_inner2x_srd0p5x
    // [7:4]	reg_ckg_dvbt2_inner2x_srd0p5x
    // [12:8]	reg_ckg_dvbtm_sram_t1outer1x_t24x
    //DVBT2
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x70)*2, 0x0000);

    // @0x0971
    // [4:0]	reg_ckg_dvbtm_sram_t12x_t24x_srd1x
    // [12:8]	reg_ckg_dvbtm_sram_t14x_t24x_srd1x
    //DVBT2
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x71)*2, 0x0000);

    // @0x0972
    // [6:0]	reg_ckg_dvbt2_s2_bch_out
    // [12:8]	reg_ckg_dvbt2_outer2x
    //DVBT2
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x72)*2, 0x0000);

    // @0x0973
    // [3:0]	reg_ckg_dvbt2_inner4x_s2_inner
    //DVBT2
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x73)*2, 0x0000);

    // @0x0974
    // [4:0]	reg_ckg_dvbtm_sram_t12x_t24x_s2inner
    // [12:8]	reg_ckg_dvbtm_sram_t14x_t24x_s2inner
    //DVBT2
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x74)*2, 0x0000);

    // @0x0975
    // [4:0]	reg_ckg_dvbtc_rs
    // [11:8]	reg_ckg_dvbs_outer2x_dvbt_outer2x
    // [15:12]	reg_ckg_dvbs_outer2x_dvbt_outer2x_miu
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x75)*2, 0xc101);

    // @0x0976
    // [4:0]	reg_ckg_dvbs_outer2x_dvbt_outer2x_dvbt2_inner2x
    //DVBT2
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x76)*2, 0x000c);

    // @0x0977
    // [3:0]	reg_ckg_dvbt2_inner4x_dvbtc_rs
    // [8:4]	reg_ckg_dvbtm_sram_adc_t22x_dvbtc_rs
    //DVBT2
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x77)*2, 0x0000);

//    $display("--------------------------------------");
//    $display("Initialize Transport Stream synthesizer and APLL");
//    $display("--------------------------------------");

    // [15:0]	reg_synth_set[15: 0]
    // [ 7:0]	reg_synth_set[23:16]
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x51)*2, 0x0000);
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x52)*2, 0x0040);


    // [0]	reg_synth_reset
    // [1]	reg_synth_ssc_en
    // [2]	reg_synth_ssc_mode
    // [4]	reg_synth_sld
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x50)*2, 0x0010);

    // [1:0]	reg_apll_loop_div_first
    // [15:8]	reg_apll_loop_div_second
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x57)*2, 0x0000);

    // [0]	reg_apll_pd
    // [1]	reg_apll_reset
    // [2]	reg_apll_porst
    // [3]	reg_apll_vco_offset
    // [4]	reg_apll_en_ts
    // [5]	reg_apll_endcc
    // [6]	reg_apll_clkin_sel
    // [8]	reg_apll_ts_mode
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x55)*2, 0x0100);
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x55)*2, 0x0110);

    // [16:0]	reg_apll_test
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x59)*2, 0x0000);

    // 0x0920
    // [3:0]	reg_ckg_ts_apll_div[2:0]
      bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x20)*2, 0x0004);

//if(_sDMD_MSB124X_InitData.u8WO_Sdram==0)
{
    // -------------------------------------------------------------------
    // initialize MIU
    // -------------------------------------------------------------------
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x0f)*2, 0x0000);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x0f)*2, 0x0000);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x0f)*2, 0x0000);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x0f)*2, 0x0c01);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x0f)*2, 0x0c00);


// set frequence 180MHz
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x11)*2, 0x60cc);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x11)*2, 0x00cc);
    //bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x11)*2, 0x0066);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x11)*2, 0x004c);  // 133Mhz
    //bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x10)*2, 0x2666);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x10)*2, 0x33f8);  // 133Mhz
    // yihao 20130925 for new apll model
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x12)*2, 0x0000);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x13)*2, 0x0000);
    // yihao 20130925 for new apll model
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x16)*2, 0x0000);
    //bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x1c)*2, 0x0080);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x1c)*2, 0x00b0);  //fine tune by Yihao Lo

    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x01)*2, 0x8100);
    // cke				: [0]
    // reg_self_refresh		: [1]
    // reg_dynamic_cke		: [2]
    // reg_dynamic_ck_odt		: [3]
    // reg_dram_bus			: [5:4] 00: 16b, 01: 32b, 10: 64b
    // reg_dram_type			: [7:6] 00: sdr, 01: ddr, 10: ddr2
    // reg_4ba			: [8]    0: 2bk,  1: 4bk
    		// reg_col_size			: [10:9]
    // reg_cke_oenz			: [12]
    // reg_dq_oenz			: [13]
    // reg_adr_oenz			: [14]
    // reg_cs_z			: [15]
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x01)*2, 0xe100);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x01)*2, 0x8100);
    //bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x02)*2, 0x0360);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x02)*2, 0x0371);  //fine tune by Yihao Lo
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x03)*2, 0x0030);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x04)*2, 0x33c9);
    // reg_tRAS                      : [3:0]        9
    // reg_tRC                       : [7:4]        c
    // reg_tRCD                      : [11:8]       3
    // reg_tRP                       : [15:12]      3
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x05)*2, 0x4232);
    // reg_tRRD                      : [3:0]         2
    // tWR                           : [7:4]         3
    // reg_tMRD                      : [11:8]        2
    // reg_tRTP			: [15:12]	4
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x06)*2, 0x5532);
    // reg_w2r_dly(tWTR)             : [3:0]         2
    // reg_w2r_oen_dly               : [7:4]         3
    // reg_r2w_dly(tRTW)             : [11:8]        5
    // reg_r2w_oen_dly               : [15:12]       5
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x07)*2, 0x400c);
    // tRFC                          : [5:0]         c
    // reg_tRAS[4]                   : [6]           0
    // reg_tRC[4]                    : [7]           0
    // reg_write_latency             : [10:8]        0
    // reg_tCCD                      : [15:14]       1
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x0e)*2, 0x1800);

    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x23)*2, 0x7ffe);

    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x20)*2, 0xc001);

    //delay 1
    MsOS_DelayTaskUs(100);

    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x0f)*2, 0x0c01);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x0f)*2, 0x0c00);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x0f)*2, 0x0c01);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x0f)*2, 0x0c00);

    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x01)*2, 0x010d);


// $display("--------Initial DRAM start here!!!");
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x00)*2, 0x0001);

    // wait(test_chip_top.dut.i_dig_top.miu_inst.miu_reg_0.init_done);
    bRet &= _MDrv_DMD_MSB124X_GetReg(0x1201, &u8_tmp);
    //printf(("[MSB124X]MIU wait init done, u8_tmp=0x%x, bRet=0x%x\n", u8_tmp, bRet));
    while (u8_tmp != 0x80)
    {
        if (u8_timeout++ > 200)
        {
            printf(("[MSB124X][err]MIU init failure...\n"));
            return FALSE;
        }
        // 10us delay
        MsOS_DelayTaskUs(10);
        bRet &= _MDrv_DMD_MSB124X_GetReg(0x1201, &u8_tmp);
    }
    //printf(("[MSB124X]MIU init done, u8_tmp=0x%x, bRet=0x%x\n", u8_tmp, bRet));

    // $display("--------Initial Done");
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x08)*2, 0x0001);

    // $display("-------------------------");
    // $display("-- miu self test start --");
    // $display("-------------------------");
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x71)*2, 0x0000);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x72)*2, 0x0010);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x74)*2, 0x5aa5);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x70)*2, 0x0001);

    // #10000;
    //delay 1
    MsOS_DelayTaskUs(100);

    //wait test_finish
    bRet &= _MDrv_DMD_MSB124X_GetReg((0x1200 + (0x70) * 2 + 1), &u8_tmp);
    //printf(("[MSB124X]MIU wait test done, u8_tmp=0x%x,bRet=0x%x\n", u8_tmp, bRet));
    while ((u8_tmp & 0x80) != 0x80)
    {
        if (u8_timeout++ > 200)
        {
            printf(("[MSB124X][err]MIU self test NOT finished...\n"));
            return FALSE;
        }
        // 10us delay
        MsOS_DelayTaskUs(10);
        bRet &= _MDrv_DMD_MSB124X_GetReg((0x1200 + (0x70) * 2 + 1), &u8_tmp);
    }

     // #10000;
     //delay 1
     MsOS_DelayTaskUs(100);

    // MIU self test FAIL let program stuck in this while loop
    bRet &= _MDrv_DMD_MSB124X_GetReg((0x1200 + (0x70) * 2 + 1), &u8_tmp);

    if (u8_tmp == 0x40)
    {
        printf(("@MSB124X, error, MIU self test fail !!!!\n"));
        bRet = FALSE;
    }
    else
    {
        printf(("@MSB124X, MIU self test successful.\n"));
    }

    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1200+(0x23)*2, 0x0000);

    // -------------------------------------------------------------------
    // initialize MIU  finish
    // -------------------------------------------------------------------
}
    // -------------------------------------------------------------------
    //  Turn on pads
    // -------------------------------------------------------------------

    // ------Turn off all pad in
    // [0] reg_set_pad_low
    // [1] reg_set_pad_high
    // [2] reg_set_i2cs_pad_low
    // [3] reg_set_i2cs_pad_high
    // [8] reg_turn_off_pad
     bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x28)*2, 0x0000);

    // ------I2CM pad on
    // [1:0]    reg_iicm_pad_sel[1:0]	1:iicm enable 2:UART enable
    // [4]	    reg_i2c_sbpm_en		1: enable I2CS bypass to I2CM function
    // [12:8]   reg_i2c_sbpm_idle_num[4:0]	a: default
     bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x08)*2, 0x0a01);

    // ------Transport Stream pad on (except TS ERR pad)
    // [15:0]   reg_en_ts_pad[15:0]	0x00ff:normal TS location 0xff00:reverse TS location
     bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x2d)*2, 0x00ff);

    // ------Transport Stream pad on (TS ERR pad)
    // [0]	    reg_en_ts_err_pad	1: enable
    // [4]	    reg_ts_err_pol	1: inverse 0:normal
     bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x2e)*2, 0x0000);

    // ------AGC pad on
    // [0] reg_ifagc_t_enable
    // [1] reg_ifagc_t_odmode
    // [2] reg_ifagc_t_data_sel
    // [4] reg_ifagc_t_force_enable
    // [5] reg_ifagc_t_force_value
    // [8] reg_ifagc_s_enable
    // [9] reg_ifagc_s_odmode
    // [10] reg_ifagc_s_data_sel
    // [12] reg_ifagc_s_force_enable
    // [13] reg_ifagc_s_force_value
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0a00+(0x18)*2, 0x0001);

    //DMD_UNLOCK();
    #ifdef MS_DEBUG
    printf("[msb124x][end]MSB124X_HW_init, bRet=0x%x\n",bRet);
    #endif
    return bRet;
}

static MS_BOOL _Load2Sdram(MS_U8 *u8_ptr, MS_U16 data_length, MS_U16 sdram_win_offset_base)
{
    MS_BOOL bRet = true;
    MS_U16  sdram_win_offset = sdram_win_offset_base;
    MS_U16  x_data_offset = 0;
    MS_U16  y_cir_addr = 0;
    #ifdef MS_DEBUG
    MS_U32 u32tmm_3 = 0, u32tmm_4 = 0;
    #endif
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][beg]Load2Sdram, len=0x%x, win_offset=0x%x\n",data_length,sdram_win_offset_base));
    u32tmm_3 = MsOS_GetSystemTime();
    #endif

    // mask miu access for all and mcu
    bRet &= _MDrv_DMD_MSB124X_SetReg(0x1200+(0x23)*2 + 1,0x0f);
    bRet &= _MDrv_DMD_MSB124X_SetReg(0x1200+(0x23)*2,0xf0);
    // 10us delay
    MsOS_DelayTaskUs(10);

    // Disable MCU
    bRet &= _MDrv_DMD_MSB124X_SetReg(0x0b00+(0x19)*2, 0x03);

    if (pDemod->_sDMD_MSB124X_InitData.bEnableSPILoadCode)
    {
        MS_U32 u32Addr = 0;

        u32Addr = (MS_U32)sdram_win_offset_base * 0x1000; // unit: 4K
        bRet &= MDrv_DMD_SSPI_MIU_Writes(u32Addr, u8_ptr, data_length);
#ifdef MS_DEBUG
        DBG_DEMOD_LOAD_I2C(printf("[msb124x]u32Addr=%08lx\n",u32Addr));
#endif

#if (SDRAM_DATA_CHECK == 1)
#ifdef MS_DEBUG
        DBG_DEMOD_LOAD_I2C(printf("[msb124x]SDRAM data check...\n"));
#endif
        MS_U16 i = 0, j = 0, index = 0;
        MS_U8 buf[SPI_DEVICE_BUFFER_SIZE]= {0};

        if((data_length % SPI_DEVICE_BUFFER_SIZE) == 0)
            index = data_length / SPI_DEVICE_BUFFER_SIZE;
        else
            index = data_length / SPI_DEVICE_BUFFER_SIZE + 1;

        for (i=0; i<index; i++)
        {
            memset(buf, 0x00, SPI_DEVICE_BUFFER_SIZE);

            bRet &= MDrv_DMD_SSPI_MIU_Reads(u32Addr+SPI_DEVICE_BUFFER_SIZE*i, buf, SPI_DEVICE_BUFFER_SIZE);
            for (j=0; j<SPI_DEVICE_BUFFER_SIZE; j++)
            {
                if (buf[j] != u8_ptr[SPI_DEVICE_BUFFER_SIZE*i+j])
                {
                    printf("[msb124x]error, u32Addr=0x%08lx, y=0x%x, x=0x%x\n",u32Addr+SPI_DEVICE_BUFFER_SIZE*i+j, buf[j] , u8_ptr[j]);
                }
            }
        }
#ifdef MS_DEBUG
        DBG_DEMOD_LOAD_I2C(printf("[msb124x]SDRAM data check...Done\n"));
#endif
#endif
    }
    else
    {
        // Change CH from 3 to 0
        if (_MSB124X_I2C_CH_Reset(0) == FALSE)
        {
            printf(">>>MSB124X CH0 Reset:Fail\n");
            return FALSE;
        }
        else
        {

            // set xData map upper and low bound for 64k DRAM window
            bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x2B00+(0x63)*2, 0x3F24);

            // set xData map offset for 64k DRAM window, 64kbytes alignment
            bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x2B00+(0x64)*2, 0x0000);

            // set xData map upper and low bound for 4k DRAM window
            bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x2B00+(0x65)*2, 0x2014);

            // set xData map offset for 4k DRAM window, 4kbytes alignment
            bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x2B00+(0x66)*2, sdram_win_offset++);

            // enable xData map for DRAM
            bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x2B00+(0x62)*2, 0x0007);

            for(x_data_offset = 0, y_cir_addr = SDRAM_BASE; x_data_offset < data_length;)
            {
                if (y_cir_addr == 0x6000)
                {
                    //set xData map offset for 4k DRAM window, 4kbytes alignment
                    // 0x1000, 4096 bytes
                    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x2B00+(0x66)*2, sdram_win_offset++);
                    y_cir_addr = SDRAM_BASE;
                }

                // max 0x200, error above.....

                if((data_length - x_data_offset) >= LOAD_CODE_I2C_BLOCK_NUM)
                {
                    bRet &= _MDrv_DMD_MSB124X_SetRegs(y_cir_addr, (u8_ptr + x_data_offset),LOAD_CODE_I2C_BLOCK_NUM);
                    y_cir_addr += LOAD_CODE_I2C_BLOCK_NUM;
                    x_data_offset += LOAD_CODE_I2C_BLOCK_NUM;
                }
                else
                {
                    bRet &= _MDrv_DMD_MSB124X_SetRegs(y_cir_addr, (u8_ptr + x_data_offset),data_length - x_data_offset);
                    y_cir_addr += (data_length - x_data_offset);
                    x_data_offset += (data_length - x_data_offset);
                }
            }
            #ifdef MS_DEBUG
            DBG_DEMOD_LOAD_I2C(printf("[msb124x]x_data_offset=%d,y_cir_addr=%d,z_block_num=%d\n",x_data_offset,y_cir_addr,sdram_win_offset));
            #endif
#if (SDRAM_DATA_CHECK == 1)
            // beg data check.
            #ifdef MS_DEBUG
            DBG_DEMOD_LOAD_I2C(printf("[msb124x]SDRAM data check...\n"));
            #endif

            sdram_win_offset = sdram_win_offset_base;

            // set xData map offset for 4k DRAM window, 4kbytes alignment
            bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x2B00+(0x66)*2, sdram_win_offset++);

            for(x_data_offset = 0, y_cir_addr = SDRAM_BASE; x_data_offset < data_length;)
            {
                MS_U8 u8_tmp;
                if (y_cir_addr == 0x6000)
                {
                    //set xData map offset for 4k DRAM window, 4kbytes alignment
                    // 0x1000, 4096 bytes
                    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x2B00+(0x66)*2, sdram_win_offset++);
                    y_cir_addr = SDRAM_BASE;
                }

                bRet &= _MDrv_DMD_MSB124X_GetReg(y_cir_addr++, &u8_tmp);
                if(u8_tmp != *(u8_ptr + x_data_offset++))
                {
                    printf("[msb124x]error, idx=0x%x, y=0x%x, x=0x%x\n",y_cir_addr-1, u8_tmp, *(u8_ptr + x_data_offset-1));
                }
            }

            #ifdef MS_DEBUG
            DBG_DEMOD_LOAD_I2C(printf("[msb124x]SDRAM data check...Done\n"));
            #endif

            // end data check
#endif
            //  Release xData map for SDRAM
            bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x2B00+(0x62)*2, 0x0000);

            // Channel changed from CH 0x00 to CH 0x03
            if (_MSB124X_I2C_CH_Reset(3) == FALSE)
            {
                ERR_DEMOD_MSB(printf(">>>MSB124X CH3 Reset:Fail\n"));
                return FALSE;
            }
        }
    }
    #ifdef MS_DEBUG
    u32tmm_4 = MsOS_GetSystemTime();
    printf("[tmm2]t4-t3 = %ld (%ld - %ld)\n",u32tmm_4-u32tmm_3,u32tmm_4,u32tmm_3);
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][end]Load2Sdram, len=0x%x, win_offset=0x%x\n",data_length,sdram_win_offset_base));
    if (!bRet) printf("%s %d Error\n",__func__, __LINE__);
    #endif

    return bRet;
}

//no sdram case
static MS_BOOL _Load2Sram(MS_U8 *u8_ptr, MS_U16 data_length, MS_U16 sram_win_offset_base)
{
    MS_BOOL bRet = TRUE;
    MS_U16  sram_win_offset = sram_win_offset_base;
    MS_U16  x_data_offset = 0;
    MS_U16  y_cir_addr = 0;
    MS_U8 u8data;
    #ifdef MS_DEBUG
    MS_U32 u32tmm_3 = 0, u32tmm_4 = 0;
    #endif

    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    UNUSED(sram_win_offset);

#ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    u32tmm_3 = MsOS_GetSystemTime();
#endif

    if (pDemod->_sDMD_MSB124X_InitData.bEnableSPILoadCode)
    {
         MS_U32 u32Addr = 0;
         // mask miu access for all and mcu
         bRet &= _MDrv_DMD_MSB124X_SetReg(0x1200+(0x23)*2 + 1,0x0f);
         bRet &= _MDrv_DMD_MSB124X_SetReg(0x1200+(0x23)*2,0xf0);
         // 10us delay
         MsOS_DelayTaskUs(10);

         // Disable MCU
         bRet &= _MDrv_DMD_MSB124X_SetReg(0x0b00+(0x19)*2, 0x03);

        // Enable sspi to sram
        //[4:4]reg_sspi2sram_en;
	 bRet &= _MDrv_DMD_MSB124X_GetReg(0x0900 + (0x3a) * 2, &u8data);
	 bRet &= _MDrv_DMD_MSB124X_SetReg(0x0900 + (0x3a) * 2, u8data|0x10);

         //address[31:31] 1:direct to sram, 0:direct to sdram
         u32Addr = 0x80000000;//SRAM_BASE<<16;
         bRet &= MDrv_DMD_SSPI_MIU_Writes(u32Addr, u8_ptr, data_length);
         //debug only, must remove
         printf(" @@@ sram download by SPI \n");

         #ifdef MS_DEBUG
         DBG_DEMOD_LOAD_I2C(printf("[msb124x]u32Addr=%08lx\n",u32Addr));
         #endif

        #if (SRAM_DATA_CHECK == 1)
        #ifdef MS_DEBUG
                DBG_DEMOD_LOAD_I2C(printf("[msb124x]SRAM data check...\n"));
        #endif
                MS_U16 i = 0, j = 0, index = 0;
                MS_U8 buf[SPI_DEVICE_BUFFER_SIZE]= {0};

                if((data_length % SPI_DEVICE_BUFFER_SIZE) == 0)
                    index = data_length / SPI_DEVICE_BUFFER_SIZE;
                else
                    index = data_length / SPI_DEVICE_BUFFER_SIZE + 1;

                for (i=0; i<index; i++)
                {
                    memset(buf, 0x00, SPI_DEVICE_BUFFER_SIZE);

                    bRet &= MDrv_DMD_SSPI_MIU_Reads(u32Addr+SPI_DEVICE_BUFFER_SIZE*i, buf, SPI_DEVICE_BUFFER_SIZE);
                    for (j=0; j<SPI_DEVICE_BUFFER_SIZE; j++)
                    {
                        if (buf[j] != u8_ptr[SPI_DEVICE_BUFFER_SIZE*i+j])
                        {
                            printf("[msb124x]error, u32Addr=0x%08lx, y=0x%x, x=0x%x\n",u32Addr+SPI_DEVICE_BUFFER_SIZE*i+j, buf[j] , u8_ptr[j]);
                        }
                    }
                }
        #ifdef MS_DEBUG
                DBG_DEMOD_LOAD_I2C(printf("[msb124x]SRAM data check...Done\n"));
        #endif
        #endif

        // Disable sspi to sram
        //[4:4]reg_sspi2sram_en
	 bRet &= _MDrv_DMD_MSB124X_GetReg(0x0900 + (0x3a) * 2, &u8data);
	 bRet &= _MDrv_DMD_MSB124X_SetReg(0x0900 + (0x3a) * 2, u8data&(~0x10));

    }
    else
    {
       //

       //---------------------------------------------------
        if (_MSB124X_I2C_CH_Reset(3) == TRUE)
        {
             // Start MCU reset
             bRet &= _MDrv_DMD_MSB124X_SetReg(0x0b32, 0x01);

             // Disable Watch-Dog
             bRet &= _MSB124X_I2C_CH_Reset(0) ;
             bRet &= _MDrv_DMD_MSB124X_SetReg(0x3008, 0x00);
             bRet &= _MDrv_DMD_MSB124X_SetReg(0x3009, 0x00);
             bRet &= _MDrv_DMD_MSB124X_SetReg(0x300A, 0x00);
             bRet &= _MDrv_DMD_MSB124X_SetReg(0x300B, 0x00);
        }
         else
         {
              ERR_DEMOD_MSB( printf(" _MSB124X_I2C_CH_Reset(3) failed \n"));
    	    return FALSE;
         }

        if (_MSB124X_I2C_CH_Reset(3) == FALSE)
        {
            ERR_DEMOD_MSB(printf(">>>MSB124X CH3 Reset:Fail\n"));
            return FALSE;
       }
        else
        {
    	  //--------------------------------------------------------------------------
            //  Set xData map for Program SRAM
            //--------------------------------------------------------------------------
            //set lower bound "xb_eram_hb[5:0]"
            bRet &= _MDrv_DMD_MSB124X_SetReg(0x10e1, 0x20);
            //set upper bound "xb_eram_lb[5:0]"
            bRet &= _MDrv_DMD_MSB124X_SetReg(0x10e0, 0x3f);
            //set "reg_xd2eram_hk_mcu_psram_en"
            bRet &= _MDrv_DMD_MSB124X_SetReg(0x10e6, 0x08);

            if (_MSB124X_I2C_CH_Reset(0) == TRUE)
            {
                   for (x_data_offset = 0, y_cir_addr = SRAM_BASE; x_data_offset < data_length;)
                   {
                            if ((data_length - x_data_offset) >= LOAD_CODE_I2C_BLOCK_NUM)
                            {
                                bRet &= _MDrv_DMD_MSB124X_SetRegs(y_cir_addr, (u8_ptr + x_data_offset), LOAD_CODE_I2C_BLOCK_NUM);
                                y_cir_addr += LOAD_CODE_I2C_BLOCK_NUM;
                                x_data_offset += LOAD_CODE_I2C_BLOCK_NUM;
                            }
                            else
                            {
                                bRet &= _MDrv_DMD_MSB124X_SetRegs(y_cir_addr, (u8_ptr + x_data_offset), data_length - x_data_offset);
                                y_cir_addr += (data_length - x_data_offset);
                                x_data_offset += (data_length - x_data_offset);
                            }
                     }

           }
           else
           {
                ERR_DEMOD_MSB( printf(" _MSB124X_I2C_CH_Reset(0) failed \n"));
                return FALSE;
           }


            //--------------------------------------------------------------------------
            //  Release xData map for Program SRAM
            //--------------------------------------------------------------------------
            //clear  "reg_xd2eram_hk_mcu_psram_en"
            // `DBG.iic_write( 2, (`RIUBASE_MCU + 8'he6), 8'h00)

            if (_MSB124X_I2C_CH_Reset(3) == TRUE)
            {
                  bRet &= _MDrv_DMD_MSB124X_SetReg(0x10e6, 0x00);
            }
            else
            {
                  ERR_DEMOD_MSB( printf("_MSB124X_I2C_CH_Reset(3) failed \n"));
                  return FALSE;
            }

            //  Set Inst map for Program SRAM
            //set lower bound "SRAM_A_START_ADDR"
             bRet &= _MDrv_DMD_MSB124X_SetReg(0x1004, 0x00);
             bRet &= _MDrv_DMD_MSB124X_SetReg(0x1005, 0x00);
             bRet &= _MDrv_DMD_MSB124X_SetReg(0x1000, 0x00);
            //set upper bound "SRAM_A_END_ADDR"
             bRet &= _MDrv_DMD_MSB124X_SetReg(0x1006, 0x7F);
             bRet &= _MDrv_DMD_MSB124X_SetReg(0x1007, 0xFF);
             bRet &= _MDrv_DMD_MSB124X_SetReg(0x1002, 0x00);
             // Enable Program SRAM
             bRet &= _MDrv_DMD_MSB124X_SetReg(0x1018, 0x01);

             //  End MCU reset
             bRet &= _MDrv_DMD_MSB124X_SetReg(0x0b32, 0x00);

        }
    }

    #ifdef MS_DEBUG
    u32tmm_4 = MsOS_GetSystemTime();
    printf("[tmm2]t4-t3 = %ld (%ld - %ld)\n",u32tmm_4-u32tmm_3,u32tmm_4,u32tmm_3);
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][end]Load2Sram, len=0x%x, win_offset=0x%x\n",data_length,sram_win_offset_base));
    if (!bRet) printf("%s %d Error\n",__func__, __LINE__);
    #endif

    return bRet;

}

static MS_BOOL _LoadDspCodeToSDRAM_Boot(void)
{
    MS_BOOL bRet = true;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][beg]LoadDspCodeToSDRAM_Boot\n"));
    #endif

    if(!(pDemod->u8DMD_MSB124X_Sdram_Code&MSB124X_BOOT))
    {
        if(sizeof(MSB124X_LIB) > MSB124X_BOOT_START_ADDR)
        {
            // boot code
            data_ptr = MSB124X_LIB + MSB124X_BOOT_START_ADDR;
            code_size = MSB124X_BOOT_END_ADDR - MSB124X_BOOT_START_ADDR + 1;
            win_offset = MSB124X_BOOT_WINDOWS_OFFSET;
            bRet &= _Load2Sdram(data_ptr,code_size,win_offset);
            if(bRet == true)
            {
                pDemod->u8DMD_MSB124X_Sdram_Code |= MSB124X_BOOT;
            }
        }
        else
        {
            printf("@msb124x, boot code is unavailable!!!\n");
        }
    }
    #ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][end]LoadDspCodeToSDRAM_Boot\n"));
    #endif
    return bRet;
}

static MS_BOOL _LoadDspCodeToSDRAM_dvbt2(void)
{
    MS_BOOL bRet = true;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][beg]LoadDspCodeToSDRAM_dvbt2\n"));
    #endif

    if( !(pDemod->u8DMD_MSB124X_Sdram_Code&MSB124X_DVBT2) )
    {
        if(sizeof(MSB124X_LIB) > MSB124X_DVBT2_P1_START_ADDR)
        {
            // dvbt2_p2
            data_ptr = MSB124X_LIB + MSB124X_DVBT2_P2_START_ADDR;
            code_size = MSB124X_DVBT2_P2_END_ADDR - MSB124X_DVBT2_P2_START_ADDR + 1;
            win_offset = MSB124X_DVBT2_P2_WINDOWS_OFFSET;
            bRet &= _Load2Sdram(data_ptr,code_size,win_offset);

            // dvbt2_p1
            data_ptr = MSB124X_LIB + MSB124X_DVBT2_P1_START_ADDR;
            code_size = MSB124X_DVBT2_P1_END_ADDR - MSB124X_DVBT2_P1_START_ADDR + 1;
            win_offset = MSB124X_DVBT2_P1_WINDOWS_OFFSET;
            bRet &= _Load2Sdram(data_ptr,code_size,win_offset);

            if(bRet == true)
            {
                pDemod->u8DMD_MSB124X_Sdram_Code |= MSB124X_DVBT2;
            }
        }
        else
        {
            ERR_DEMOD_MSB(printf("@msb124x, dvbt2 code is unavailable!!!\n"));
        }
    }
    #ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][end]LoadDspCodeToSDRAM_dvbt2\n"));
    #endif
    return bRet;
}

static MS_BOOL _LoadDspCodeToSDRAM_dvbs2(void)
{
    MS_BOOL bRet = true;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][beg]LoadDspCodeToSDRAM_dvbs2\n"));
    #endif
    printf("[msb124x][beg]LoadDspCodeToSDRAM_dvbs2\n");

    if(!(pDemod->u8DMD_MSB124X_Sdram_Code&MSB124X_DVBS2))
    {
        // dvbt code
    if(sizeof(MSB124X_LIB) > MSB124X_DVBS2_P1_START_ADDR)
        {
            // dvbs2_p2
            data_ptr = MSB124X_LIB + MSB124X_DVBS2_P2_START_ADDR;
            code_size = MSB124X_DVBS2_P2_END_ADDR - MSB124X_DVBS2_P2_START_ADDR + 1;
            win_offset = MSB124X_DVBS2_P2_WINDOWS_OFFSET;
            bRet &= _Load2Sdram(data_ptr,code_size,win_offset);

            // dvbs2_p1
            data_ptr = MSB124X_LIB + MSB124X_DVBS2_P1_START_ADDR;
            code_size = MSB124X_DVBS2_P1_END_ADDR - MSB124X_DVBS2_P1_START_ADDR + 1;
            win_offset = MSB124X_DVBS2_P1_WINDOWS_OFFSET;
            bRet &= _Load2Sdram(data_ptr,code_size,win_offset);
         if(bRet == true)
         {
             pDemod->u8DMD_MSB124X_Sdram_Code |= MSB124X_DVBS2;
         }
        }
        else
        {
         printf("@msb124x, dvbs2 code is unavailable!!!\n");
        }
    }
    #ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][end]LoadDspCodeToSDRAM_dvbs2\n"));
    #endif
    return bRet;
}

//no sdram case -- DVBS
static MS_BOOL _LoadDspCodeToSRAM_dvbs2(void)
{
    MS_BOOL bRet = true;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;

    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][beg]LoadDspCodeToSRAM_dvbs2\n"));
    #endif


    if(!(pDemod->u8DMD_MSB124X_Sram_Code&MSB124X_DVBS2))
    {
        // dvbs code
        if(sizeof(MSB124X_LIB) > MSB124X_DVBS2_P1_START_ADDR)
        {
         data_ptr = MSB124X_LIB + MSB124X_DVBS2_P1_START_ADDR;
         code_size = MSB124X_DVBS2_P1_END_ADDR - MSB124X_DVBS2_P1_START_ADDR + 1;
         win_offset = MSB124X_DVBS2_P1_WINDOWS_OFFSET;
         bRet &= _Load2Sram(data_ptr,code_size,win_offset);
         if(bRet == true)
         {
             pDemod->u8DMD_MSB124X_Sram_Code |= MSB124X_DVBS2;
         }
        }
        else
        {
         printf("@msb124x, dvbs2 code is unavailable!!!\n");
        }
    }
    #ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][end]LoadDspCodeToSRAM_dvbs2\n"));
    #endif
    return bRet;
}


static MS_BOOL _LoadDspCodeToSDRAM_dvbt(void)
{
    MS_BOOL bRet = true;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][beg]LoadDspCodeToSDRAM_dvbt\n"));
    #endif

    if(!(pDemod->u8DMD_MSB124X_Sdram_Code&MSB124X_DVBT))
    {
        // dvbt code
        if(sizeof(MSB124X_LIB) > MSB124X_DVBT_START_ADDR)
        {
         data_ptr = MSB124X_LIB + MSB124X_DVBT_START_ADDR;
         code_size = MSB124X_DVBT_END_ADDR - MSB124X_DVBT_START_ADDR + 1;
         win_offset = MSB124X_DVBT_WINDOWS_OFFSET;
         bRet &= _Load2Sdram(data_ptr,code_size,win_offset);
         if(bRet == true)
         {
             pDemod->u8DMD_MSB124X_Sdram_Code |= MSB124X_DVBT;
         }
        }
        else
        {
         printf("@msb124x, dvbt code is unavailable!!!\n");
        }
    }
    #ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][end]LoadDspCodeToSDRAM_dvbt\n"));
    #endif
    return bRet;
}

//no sdram case
static MS_BOOL _LoadDspCodeToSRAM_dvbt(void)
{
    MS_BOOL bRet = true;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][beg]LoadDspCodeToSRAM_dvbt\n"));
    #endif

    if(!(pDemod->u8DMD_MSB124X_Sram_Code&MSB124X_DVBT))
    {
        // dvbt code
        if(sizeof(MSB124X_LIB) > MSB124X_DVBT_START_ADDR)
        {
         data_ptr = MSB124X_LIB + MSB124X_DVBT_START_ADDR;
         code_size = MSB124X_DVBT_END_ADDR - MSB124X_DVBT_START_ADDR + 1;
         win_offset = MSB124X_DVBT_WINDOWS_OFFSET;
         bRet &= _Load2Sram(data_ptr,code_size,win_offset);
         if(bRet == true)
         {
             pDemod->u8DMD_MSB124X_Sram_Code |= MSB124X_DVBT;
         }
        }
        else
        {
         printf("@msb124x, dvbt code is unavailable!!!\n");
        }
    }
    #ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][end]LoadDspCodeToSRAM_dvbt\n"));
    #endif
    return bRet;
}

static MS_BOOL _LoadDspCodeToSDRAM_dvbc(void)
{
    MS_BOOL bRet = true;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][beg]LoadDspCodeToSDRAM_dvbc\n"));
    #endif

    if(!(pDemod->u8DMD_MSB124X_Sdram_Code&MSB124X_DVBC))
    {
        // dvbc code
        if(sizeof(MSB124X_LIB) > MSB124X_DVBC_START_ADDR)
        {
            data_ptr = MSB124X_LIB + MSB124X_DVBC_START_ADDR;
            code_size = MSB124X_DVBC_END_ADDR - MSB124X_DVBC_START_ADDR + 1;
            win_offset = MSB124X_DVBC_WINDOWS_OFFSET;
            bRet &= _Load2Sdram(data_ptr,code_size,win_offset);
            if(bRet == true)
            {
               pDemod->u8DMD_MSB124X_Sdram_Code |= MSB124X_DVBC;
            }
        }
        else
        {
            printf("@msb124x, dvbc code is unavailable!!!\n");
        }
    }
    #ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][end]LoadDspCodeToSDRAM_dvbc\n"));
    #endif
    return bRet;
}

//no sdram case
static MS_BOOL _LoadDspCodeToSRAM_dvbc(void)
{
    MS_BOOL bRet = true;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][beg]LoadDspCodeToSRAM_dvbc\n"));
    #endif

    if(!(pDemod->u8DMD_MSB124X_Sram_Code&MSB124X_DVBC))
    {
        // dvbc code
        if(sizeof(MSB124X_LIB) > MSB124X_DVBC_START_ADDR)
        {
            data_ptr = MSB124X_LIB + MSB124X_DVBC_START_ADDR;
            code_size = MSB124X_DVBC_END_ADDR - MSB124X_DVBC_START_ADDR + 1;
            win_offset = MSB124X_DVBC_WINDOWS_OFFSET;
            bRet &= _Load2Sram(data_ptr,code_size,win_offset);
            if(bRet == true)
            {
               pDemod->u8DMD_MSB124X_Sram_Code |= MSB124X_DVBC;
            }
        }
        else
        {
            printf("@msb124x, dvbc code is unavailable!!!\n");
        }
    }
    #ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][end]LoadDspCodeToSRAM_dvbc\n"));
    #endif
    return bRet;
}

static MS_BOOL _LoadDspCodeToSDRAM(MS_U8 code_n)
{
    MS_BOOL bRet = true;
    #ifdef MS_DEBUG
    MS_U32 u32tmm_7 = 0, u32tmm_8 = 0;
    #endif
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();
    MS_U8 u8Data;
    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][beg]LoadDspCodeToSDRAM, code_n=0x%x\n",code_n));
    u32tmm_7 = MsOS_GetSystemTime();
    #endif

    if (pDemod->_sDMD_MSB124X_InitData.bEnableSPILoadCode)
    {
        (pDemod->_sDMD_MSB124X_InitData.fpMSB124x_SPIPAD_En)(TRUE);
        // ------enable to use TS_PAD as SSPI_PAD
        // [0:0] reg_en_sspi_pad
        // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
        _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900 + (0x3b) * 2, 0x0002);
        // ------- MSPI protocol setting
        // [8] cpha
        // [9] cpol
        _MDrv_DMD_MSB124X_GetReg(0x0900+(0x3a)*2+1,&u8Data);
        u8Data &= 0xFC;
        _MDrv_DMD_MSB124X_SetReg(0x0900+(0x3a)*2+1, u8Data);
        // ------- MSPI driving setting
        _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x2c)*2, 0x07ff);
	}

    switch(code_n)
    {
        case MSB124X_BOOT:
            {
                // boot code
                bRet &= _LoadDspCodeToSDRAM_Boot();
            }
        break;
        case MSB124X_DVBT2:
            {
                // dvbt2 code
                bRet &= _LoadDspCodeToSDRAM_dvbt2();
            }
            break;
        case MSB124X_DVBT:
            {
                // dvbt
                bRet &= _LoadDspCodeToSDRAM_dvbt();
            }
            break;
        case MSB124X_DVBC:
            {
                // dvbtc
                bRet &= _LoadDspCodeToSDRAM_dvbc();
            }
            break;
        case MSB124X_DVBS2:
            {
                // dvbtc
                bRet &= _LoadDspCodeToSDRAM_dvbs2();
            }
            break;

        case MSB124X_ALL:
        default:
            {
                // boot+dvbt2+dvbt+dvbc
                // boot code
                bRet &= _LoadDspCodeToSDRAM_Boot();
                // dvbt2
                bRet &= _LoadDspCodeToSDRAM_dvbt2();
                // dvbt
                bRet &= _LoadDspCodeToSDRAM_dvbt();
                // dvbtc
                bRet &= _LoadDspCodeToSDRAM_dvbc();
                bRet &= _LoadDspCodeToSDRAM_dvbs2();
            }
            break;
    }

    if (pDemod->_sDMD_MSB124X_InitData.bEnableSPILoadCode)
    {
        // ------disable to use TS_PAD as SSPI_PAD after load code
        // [0:0] reg_en_sspi_pad
        // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
        _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900 + (0x3b) * 2, 0x0001);
        (pDemod->_sDMD_MSB124X_InitData.fpMSB124x_SPIPAD_En)(FALSE);
    }

    #ifdef MS_DEBUG
    u32tmm_8 = MsOS_GetSystemTime();
    printf("[tmm4]t8-t7 = %ld (%ld - %ld)\n",u32tmm_8-u32tmm_7,u32tmm_8,u32tmm_7);
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][end]LoadDspCodeToSDRAM, code_n=0x%x, bRet=0x%x\n",code_n,bRet));
    #endif

    return bRet;
}

//no sdram case
static MS_BOOL _LoadDspCodeToSRAM(MS_U8 code_n)
{
    MS_BOOL bRet = true;
    #ifdef MS_DEBUG
   // MS_U32 u32tmm_7 = 0, u32tmm_8 = 0;
    #endif
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][beg]LoadDspCodeToSRAM, code_n=0x%x\n",code_n));
   // u32tmm_7 = MsOS_GetSystemTime();
    #endif

    if(code_n == pDemod->u8DMD_MSB124X_Sram_Code)
    {
        #ifdef MS_DEBUG
        printf("[msb124x]LoadDspCodeToSRAM, code is available.\n");
        #endif
        return bRet;
    }

    if (pDemod->_sDMD_MSB124X_InitData.bEnableSPILoadCode)
    {
        (pDemod->_sDMD_MSB124X_InitData.fpMSB124x_SPIPAD_En)(TRUE);
        // ------enable to use TS_PAD as SSPI_PAD
        // [0:0] reg_en_sspi_pad
        // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900 + (0x3b) * 2, 0x0002);
	 //[4:4]reg_sspi2sram_en;
	 //bRet &= _MDrv_DMD_MSB124X_GetReg(0x0900 + (0x3a) * 2, &u8data);
	 //bRet &= _MDrv_DMD_MSB124X_SetReg(0x0900 + (0x3a) * 2, u8data|0x10);

    }

    switch(code_n)
    {
        case MSB124X_DVBT:
            {
                // dvbt
                bRet &= _LoadDspCodeToSRAM_dvbt();
            }
            break;
        case MSB124X_DVBC:
            {
                // dvbtc
                bRet &= _LoadDspCodeToSRAM_dvbc();
            }
            break;
        case MSB124X_DVBS2:
            {
                // dvbs/s2
                bRet &= _LoadDspCodeToSRAM_dvbs2();
            }
            break;
        default:
            {
                // boot code
                bRet &= _LoadDspCodeToSRAM_dvbt();
            }
            break;
    }


    if (pDemod->_sDMD_MSB124X_InitData.bEnableSPILoadCode)
    {
        // ------disable to use TS_PAD as SSPI_PAD after load code
        // [0:0] reg_en_sspi_pad
        // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900 + (0x3b) * 2, 0x0001);
	 //[4:4]reg_sspi2sram_en
	 //bRet &= _MDrv_DMD_MSB124X_GetReg(0x0900 + (0x3a) * 2, &u8data);
	 //bRet &= _MDrv_DMD_MSB124X_SetReg(0x0900 + (0x3a) * 2, u8data&(~0x10));

        (pDemod->_sDMD_MSB124X_InitData.fpMSB124x_SPIPAD_En)(FALSE);
    }

    #ifdef MS_DEBUG
    //u32tmm_8 = MsOS_GetSystemTime();
    //printf("[tmm4]t8-t7 = %ld (%ld - %ld)\n",u32tmm_8-u32tmm_7,u32tmm_8,u32tmm_7);
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][end]LoadDspCodeToSRAM, code_n=0x%x, bRet=0x%x\n",code_n,bRet));
    #endif

    return bRet;
}



static MS_BOOL _MSB124X_MEM_switch(MS_U8 mem_type)
{
    MS_BOOL bRet = true;
    MS_U8 u8_tmp = 0;
    MS_U8 u8Data = 0;
    MS_U16 timeout = 0;
    #ifdef MS_DEBUG
    MS_U32 u32tmm_15 = 0, u32tmm_16 = 0;
    #endif
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();    

    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][beg]MSB124X_MEM_switch, mem_type=0x%x\n",mem_type));
    u32tmm_15 = MsOS_GetSystemTime();
    #endif

    switch(pDemod->eDMD_MSB124X_CurrentDemodulatorType)
    {
        case E_DMD_MSB124X_DEMOD_DVBT2:
            u8Data=1;
            break;
        case E_DMD_MSB124X_DEMOD_DVBT:
            u8Data=2;
            break;
        case E_DMD_MSB124X_DEMOD_DVBC:
            u8Data=3;
            break;
        case E_DMD_MSB124X_DEMOD_DVBS2:
            u8Data=4;
            break;
        default:
            u8Data=2;
            return FALSE;
    }
    
    if(mem_type == 1)
    {

        bRet &= _MDrv_DMD_MSB124X_SetReg(0x2B80,0x10);

        // SRAM_START_ADDR 0x0000
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1000,0x0000);
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1004,0x0000);

        // SRAM_END_ADDR 0x7FFF
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1002,0x0000);
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1006,0x7FFF);

        if(u8Data == 4)
        {//DVBS

            // Offset to 1M +128 KB
            bRet &= _MDrv_DMD_MSB124X_SetReg(0x2B80,0x12);//Uint : 64 KB

            // DRAM_START_ADDR 0x28000
            bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1008,0x0000);
            bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x100C,0x8000);

            // DRAM_END_ADDR    0x2FFFF
            bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x100A,0x0000);
            bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x100E,0xFFFF);
        }
        else
        {//DVBT2,DVBT,DVBC
        
            bRet &= _MDrv_DMD_MSB124X_SetReg(0x2B80,0x10);// Offset to 1M
            
        // DRAM_START_ADDR 0x8000
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1008,0x0000);
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x100C,0x8000);


        // DRAM_END_ADDR    0xFFFF
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x100A,0x0000);
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x100E,0xFFFF);

        //Disable romA_en & romB_en
        bRet &= _MDrv_DMD_MSB124X_SetReg(0x1038,0x00);
        }
        // Enable SRAM&SDRAM memory map
        bRet &= _MDrv_DMD_MSB124X_SetReg(0x1018,0x05);

        // Wait memory map to be enabled
        do
        {
            bRet &= _MDrv_DMD_MSB124X_GetReg(0x1018,&u8_tmp);
            if(timeout++ > 500)
            {
                printf("@msb124x, D+S memory mapping failure.!!!\n");
                return FALSE;
            }
        }
        while(u8_tmp != 0x05);
    }
    else if(mem_type == 0)
    {
        // Enable SRAM&SDRAM memory map

        bRet &= _MDrv_DMD_MSB124X_SetReg(0x2B80,0x10);

        // DRAM_START_ADDR 0x8000
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x1008,0x0000);
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x100C,0x0000);


        // DRAM_END_ADDR    0xFFFF
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x100A,0x0000);
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x100E,0x7FFF);

        // Enable SRAM&SDRAM memory map
        bRet &= _MDrv_DMD_MSB124X_SetReg(0x1018,0x04);

        // Wait memory map to be enabled
        do
        {
            bRet &= _MDrv_DMD_MSB124X_GetReg(0x1018,&u8_tmp);
            if(timeout++ > 500)
            {
                printf("@msb124x, D memory mapping failure.!!!\n");
                return FALSE;
            }
        }
        while(u8_tmp != 0x04);
    }
    else
    {
       printf("@msb124x, invalid mem type mapping.\n");
       return FALSE;
    }

    #ifdef MS_DEBUG
    u32tmm_16 = MsOS_GetSystemTime();
    printf("[tmm8]t16-t15 = %ld (%ld - %ld)\n",u32tmm_16-u32tmm_15,u32tmm_16,u32tmm_15);
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][end]MSB124X_MEM_switch, , mem_type=0x%x, bRet=0x%x\n",mem_type,bRet));
    #endif

    return bRet;
}

static MS_BOOL _LoadSdram2Sram(MS_U8 CodeNum)
{
    MS_BOOL bRet = true;
    MS_U8   u8_data = 0;
    MS_U8   u8_timeout = 0xFF;
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();


    #ifdef MS_DEBUG
    MS_U32 u32tmm_17 = 0, u32tmm_18 = 0;
    #endif

    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][beg]LoadSdram2Sram, g_sram_code=0x%x, codeNum=0x%x\n",u8DMD_MSB124X_Sram_Code,CodeNum));
    u32tmm_17 = MsOS_GetSystemTime();
    #endif

    if(CodeNum == pDemod->u8DMD_MSB124X_Sram_Code)
    {
        #ifdef MS_DEBUG
        printf("[msb124x]LoadSdram2Sram, code is available.\n");
        #endif
        return bRet;
    }
/*
    MS_U8   u8_tmp = 0;
    MS_U32  u32Timeout = 0;
    MS_U8   u8DoneFlag = 0;

    bRet &= _MSB124X_MEM_switch(0);

    if(CodeNum == MSB124X_DVBT2)
        u8_tmp = 1|0x10;
    else if(CodeNum == MSB124X_DVBT)
        u8_tmp = 2|0x10;
    else if(CodeNum == MSB124X_DVBC)
        u8_tmp = 3|0x10;
    else
        u8_tmp = 0|0x10;

    // Assign f/w code type to load => 0x11: dvbt2, 0x12: dvbt, 0x13: dvbc
    bRet &= _MDrv_DMD_MSB124X_SetReg(0x0900+(0x4f)*2, u8_tmp);

    // enable miu mask, miu, mcu, gdma
    bRet &= _MDrv_DMD_MSB124X_SetReg(0x1200+(0x23)*2 + 1,0x0f);
    bRet &= _MDrv_DMD_MSB124X_SetReg(0x1200+(0x23)*2,0xf0);

    MsOS_DelayTaskUs(10);
    // enable mcu
    bRet &= _MDrv_DMD_MSB124X_SetReg(0x0b00+(0x19)*2, 0x00);

    do
    {
        bRet &= _MDrv_DMD_MSB124X_GetReg(0x0900+(0x4f)*2, &u8DoneFlag);

        if (u32Timeout++ > 500)
        {
            printf("@msb124x, LoadSdram2Sram boot move code fail.!!!\n");
            return FALSE;
        }
        MsOS_DelayTaskUs(1*1000);

    }
    while(u8DoneFlag != 0xaa);

    // mask miu access of mcu
    bRet &= _MDrv_DMD_MSB124X_SetReg(0x1200+(0x23)*2,0xf2);

    // 10us delay
    MsOS_DelayTaskUs(10);

    // Disable MCU
    bRet &= _MDrv_DMD_MSB124X_SetReg(0x0b00+(0x19)*2, 0x03);

    // enable miu mask, miu, mcu, gdma
    bRet &= _MDrv_DMD_MSB124X_SetReg(0x1200+(0x23)*2 + 1,0x0f);
    bRet &= _MDrv_DMD_MSB124X_SetReg(0x1200+(0x23)*2,0xf0);
*/
    // mask miu access of fdp, tdi, djb
    bRet &= _MDrv_DMD_MSB124X_SetReg(0x1200 + (0x23) * 2 + 1, 0x0f);
    bRet &= _MDrv_DMD_MSB124X_SetReg(0x1200 + (0x23) * 2, 0xf0);

    // 10us delay
    MsOS_DelayTaskUs(10);

    // Disable MCU
    bRet &= _MDrv_DMD_MSB124X_SetReg(0x0b00+(0x19)*2, 0x03);

    // Use GDMA move code from SDRAM to SRAM
     bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0300+(0x00)*2, 0x4254);     // rst
     bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0300+(0x00)*2, 0x4257);    // cfg & trig

   switch(CodeNum)
   {
        case 0x02: //DVB-T2 @001000h ~ 008FFFh
            bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0300+(0x01)*2, 0x0000); // Set src_addr
            bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0300+(0x02)*2, 0x0011); // start from 1M+64k
            break;

        case 0x04: //DVB-T @010000h ~ 017FFFh
            bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0300+(0x01)*2, 0x8000); // Set src_addr
            bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0300+(0x02)*2, 0x0011); // start from 1M+96k
            break;

        case 0x08: //DVB-C @018000h ~ 01FFFFh
            bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0300+(0x01)*2, 0x0000); // Set src_addr
            bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0300+(0x02)*2, 0x0012); // start from 1M+128k
            break;
        case 0x10: //DVB-S2 @020000h ~ 027FDFh
            bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0300+(0x01)*2, 0x0000); // Set src_addr
            bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0300+(0x02)*2, 0x0013); // start from 1M+160k
            break;
        default:
            bRet &= false;
            break;
   }

    // Set dst_addr
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0300+(0x03)*2, 0x0000);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0300+(0x04)*2, 0x0000);

    // Set data_size
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0300+(0x05)*2, 0x8000);
    bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0300+(0x06)*2, 0x0000);

    bRet &= _MDrv_DMD_MSB124X_SetReg(0x0300+(0x07)*2, 0x01); //start GDMA

    // Wait for GDMA
    do
    {
        MsOS_DelayTaskUs(10);
        bRet &= _MDrv_DMD_MSB124X_GetReg(0x0300+(0x08)*2, &u8_data);
        u8_timeout--;
    }while(((u8_data&0x01) != 0x01) && (u8_timeout != 0x00));

    if(u8_data != 0x01)
    {
        printf("[msb124x][err]LoadSdram2Sram, GDMA move code fail!!\n");
        return false;
    }

    #ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb124x]LoadSdram2Sram, GDMA move code done!!\n"));
    #endif

    //if(CodeNum == 0x02)
    //    bRet &= _MSB124X_MEM_switch(2);  // setting for rom code
    //else
        bRet &= _MSB124X_MEM_switch(1);

    if(bRet == FALSE)
    {
        pDemod->u8DMD_MSB124X_Sram_Code = 0x00;
    }
    else
    {
        pDemod->u8DMD_MSB124X_Sram_Code = CodeNum;
    }

    #ifdef MS_DEBUG
    u32tmm_18 = MsOS_GetSystemTime();
    printf("[tmm9]t18-t17 = %ld (%ld - %ld)\n",u32tmm_18-u32tmm_17,u32tmm_18,u32tmm_17);
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][end]LoadSdram2Sram, codeNum=0x%x, g_sram_code=0x%x, bRet=0x%x\n",CodeNum,u8DMD_MSB124X_Sram_Code,bRet));
    #endif

    return bRet;
}

static MS_BOOL  _DTV_DVBC_DSPReg_Init(void)
{
    MS_U8    idx = 0;
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    #endif
    for (idx = 0; idx<sizeof(MSB124X_DVBC_DSPREG_TABLE); idx++)
    {
        if( _MDrv_DMD_MSB124X_SetDSPReg(idx + 0x20, pDemod->DVBC_DSP_REG[idx])!=TRUE)
        {
            printf("dvbc dsp reg init NG\n");
            return FALSE;
        }
    }



        if( _MDrv_DMD_MSB124X_SetDSPReg(C_config_spread_span, C_TS_SPREAD_SPAN)!=TRUE)
        {
            printf("dvbc dsp reg init NG\n");
            return FALSE;
        }
        if( _MDrv_DMD_MSB124X_SetDSPReg(C_config_spread_step, C_TS_SPREAD_STEP_SIZE)!=TRUE)
        {
            printf("dvbc dsp reg init NG\n");
            return FALSE;
        }

        if( _MDrv_DMD_MSB124X_SetDSPReg(C_phase_tuning_en, C_TS_PHASE_TUNING_EN)!=TRUE)
        {
            printf("dvbc dsp reg init NG\n");
            return FALSE;
        }
        if( _MDrv_DMD_MSB124X_SetDSPReg(C_phase_tuning_num, C_TS_PHASE_TUNING_NUM)!=TRUE)
        {
            printf("dvbc dsp reg init NG\n");
            return FALSE;
        }

    printf("DVBC dsp reg init ok\n");

    return TRUE;
}


static MS_U8  _DTV_DVBT_DSPReg_CRC(void)
{
    MS_U8 crc = 0;
    MS_U8 idx = 0;
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    #endif
    for (idx = 0; idx<(sizeof(pDemod->DVBT_DSP_REG)); idx++)
    {
        crc ^= pDemod->DVBT_DSP_REG[idx];
    }

    crc = ~crc;

    return crc;
}

static MS_BOOL  _DTV_DVBT_DSPReg_Init(void)
{
    #ifdef MS_DEBUG
    MS_U32 u32tmm_7 = 0, u32tmm_8 = 0;
    #endif
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    u32tmm_7 = MsOS_GetSystemTime();
    #endif

    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)T_CONFIG_BW, pDemod->DVBT_DSP_REG[0]) != TRUE)
    {
      printf("dvbt dsp reg init NG\n"); return FALSE;
    }
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)T_CONFIG_FC_L, pDemod->DVBT_DSP_REG[1]) != TRUE)
    {
      printf("dvbt dsp reg init NG\n"); return FALSE;
    }
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)T_CONFIG_FC_H, pDemod->DVBT_DSP_REG[2]) != TRUE)
    {
      printf("dvbt dsp reg init NG\n"); return FALSE;
    }
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)T_CONFIG_TS_SERIAL, pDemod->DVBT_DSP_REG[3]) != TRUE)
    {
      printf("dvbt dsp reg init NG\n"); return FALSE;
    }
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)T_CONFIG_TS_CLK_RATE, pDemod->DVBT_DSP_REG[4]) != TRUE)
    {
      printf("dvbt dsp reg init NG\n"); return FALSE;
    }
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)T_CONFIG_TS_OUT_INV, pDemod->DVBT_DSP_REG[5]) != TRUE)
    {
      printf("dvbt dsp reg init NG\n"); return FALSE;
    }
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)T_CONFIG_TS_DATA_SWAP, pDemod->DVBT_DSP_REG[6]) != TRUE)
    {
      printf("dvbt dsp reg init NG\n"); return FALSE;
    }
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)T_CONFIG_IQ_SWAP, pDemod->DVBT_DSP_REG[7]) != TRUE)
    {
      printf("dvbt dsp reg init NG\n"); return FALSE;
    }
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)T_CONFIG_IF_INV_PWM_OUT_EN, pDemod->DVBT_DSP_REG[8]) != TRUE)
    {
      printf("dvbt dsp reg init NG\n"); return FALSE;
    }

// APLL 
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)T_CONFIG_SPREAD_SPAN, T_TS_SPREAD_SPAN) != TRUE)
    {
      printf("dvbt dsp reg init NG\n"); return FALSE;
    }
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)T_CONFIG_SPREAD_STEP, T_TS_SPREAD_STEP_SIZE) != TRUE)
    {
      printf("dvbt dsp reg init NG\n"); return FALSE;
    }
//APLL

    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)T_PHASE_TUNING_EN, T_TS_PHASE_TUNING_EN) != TRUE)
    {
      printf("dvbt dsp reg init NG\n"); return FALSE;
    }
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)T_PHASE_TUNING_NUM, T_TS_PHASE_TUNING_NUM) != TRUE)
    {
      printf("dvbt dsp reg init NG\n"); return FALSE;
    }


    #ifdef MS_DEBUG
    u32tmm_8 = MsOS_GetSystemTime();
    printf("[3333]t8 - t7 = %ld (%ld - %ld) \n",u32tmm_8-u32tmm_7,u32tmm_8,u32tmm_7);
    #endif

    printf("dvbt dsp reg init ok\n");

    return TRUE;
}

static MS_BOOL _DTV_DVBT2_DSPReg_Init(void)
{
    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    #endif
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)E_T2_BW, pDemod->DVBT2_DSP_REG[0]) != TRUE)
    {
      printf("T2 dsp reg init NG\n"); return FALSE;
    }
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)E_T2_FC_L,  pDemod->DVBT2_DSP_REG[1]) != TRUE)
    {
      printf("T2 dsp reg init NG\n"); return FALSE;
    }
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)E_T2_FC_H,  pDemod->DVBT2_DSP_REG[2]) != TRUE)
    {
      printf("T2 dsp reg init NG\n"); return FALSE;
    }
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)E_T2_TS_SERIAL,  pDemod->DVBT2_DSP_REG[3]) != TRUE)
    {
      printf("T2 dsp reg init NG\n"); return FALSE;
    }
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)E_T2_TS_CLK_RATE,  pDemod->DVBT2_DSP_REG[4]) != TRUE)
    {
      printf("T2 dsp reg init NG\n"); return FALSE;
    }
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)E_T2_TS_OUT_INV,  pDemod->DVBT2_DSP_REG[5]) != TRUE)
    {
      printf("T2 dsp reg init NG\n"); return FALSE;
    }
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)E_T2_TS_DATA_SWAP,  pDemod->DVBT2_DSP_REG[6]) != TRUE)
    {
      printf("T2 dsp reg init NG\n"); return FALSE;
    }
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)E_T2_TS_ERR_POL,  pDemod->DVBT2_DSP_REG[7]) != TRUE)
    {
      printf("T2 dsp reg init NG\n"); return FALSE;
    }

// APLL span	
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)E_T2_SPREAD_SPAN, T2_TS_SPREAD_SPAN) != TRUE)
    {
      printf("T2 dsp reg init NG\n"); return FALSE;
    }
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)E_T2_SPREAD_STEP, T2_TS_SPREAD_STEP_SIZE) != TRUE)
    {
      printf("T2 dsp reg init NG\n"); return FALSE;
    }
// APLL span	

    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)E_T2_PHASE_TUNING_EN, T2_TS_PHASE_TUNING_EN) != TRUE)
    {
      printf("T2 dsp reg init NG\n"); return FALSE;
    }
    if( _MDrv_DMD_MSB124X_SetDSPReg((MS_U8)E_T2_PHASE_TUNING_NUM, T2_TS_PHASE_TUNING_NUM) != TRUE)
    {
      printf("T2 dsp reg init NG\n"); return FALSE;
    }

    printf("T2 dsp reg init ok\n");

    return TRUE;
}

static MS_BOOL _LoadDSPCode(void)
{
    ////MAPI_U32 u32Now = MsOS_GetSystemTime();
    //printf("\t\t\tLoadDSPCode TIME   %ld (=%ld-%ld)\n", u32Now-u32StartTime, u32Now, u32StartTime);
    ////DBG_DEMOD_MSB(printf("\t\t\tLoadDSPCode TIME   %ld (=%ld-%ld)\n", u32Now-u32StartTime, u32Now, u32StartTime));//to measure time
    //u32StartTime = u32Now;

    MS_U32        u32Timeout = 0;
    MS_U8         u8DoneFlag = 0;
    MS_U8         u8Data = 0;
    MS_BOOL       bRet = true;
    MS_U8 u8FirmwareType = MSB124X_DVBT;
    //kirin  bonding option
    MS_U32 Bonding_Control = 0;
    MS_U16 Chip_ID = 0;
    #ifdef MS_DEBUG
    MS_U32 u32tmm_9 = 0, u32tmm_10 = 0, u32tmm_11 = 0, u32tmm_12 = 0, u32tmm_13 = 0;
    #endif
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][beg]LoadDspCode\n"));
    DBG_DEMOD_MSB(printf(">>>MSB124X: Load DSP...\n"));
    #endif



	/*
	add this for Kirin(Windermere MCP) bonding option
	*/
	#if 1
	//kirin  bonding option


	Chip_ID = MDrv_SYS_GetChipID();
	DBG_KIRIN_BOND(printf(">>> Kirin Debug:Chip ID = %x \n",Chip_ID));

	if(((Chip_ID & 0xFF) ==0x98)&&((pDemod->eDMD_MSB124X_CurrentDemodulatorType)==E_DMD_MSB124X_DEMOD_DVBS2))//bit[8:7]
	{

        MS_VIRT u32MMIOBaseAdr;
        MS_PHY u32BankSize;
        if (MDrv_MMIO_GetBASE(&u32MMIOBaseAdr, &u32BankSize, MS_MODULE_HW))
	      {
            Bonding_Control = DRV_RIU_ReadByte(((0xBF342BEC-0xBF200000)+u32MMIOBaseAdr));
            DBG_KIRIN_BOND(printf(">>> Kirin Debug: Bouning_Control = %x \n",Bonding_Control));   //OTP_DVB_SEL; [8:7]=2b'10 means no DVBS/S2
        }
	      if (((Bonding_Control&0x0180)==0x100))
        {
		        DBG_KIRIN_BOND(printf(">>> Kirin Debug: disable DVBS load code of bonding option \n"));
		        return false;
	      }
	}
	#endif


    switch(pDemod->eDMD_MSB124X_CurrentDemodulatorType)
    {
        case E_DMD_MSB124X_DEMOD_DVBT2:
            u8Data=1;
            break;
        case E_DMD_MSB124X_DEMOD_DVBT:
            u8Data=2;
            break;
        case E_DMD_MSB124X_DEMOD_DVBC:
            u8Data=3;
            break;
        case E_DMD_MSB124X_DEMOD_DVBS2:
            u8Data=4;
            break;
        default:
            u8Data=2;
            return FALSE;
    }

    if(pDemod->_sDMD_MSB124X_InitData.u8WO_Sdram==1)   //  1 means no SDRAM on board
    {
           //no sdram case
        #ifdef MS_DEBUG
        u32tmm_11 = MsOS_GetSystemTime();
        #endif
        switch (u8Data)
        {
            //since no sdram IC not support DVBT2
            case 2:
            default:
                u8FirmwareType = MSB124X_DVBT;
                break;
            case 3:
                u8FirmwareType = MSB124X_DVBC;
                break;
            case 4:
                u8FirmwareType = MSB124X_DVBS2;
                break;
        }
        bRet &= _LoadDspCodeToSRAM(u8FirmwareType);

        bRet &= _MSB124X_MEM_switch(1);

        // Enable MCU
        bRet &= _MDrv_DMD_MSB124X_SetReg(0x0b00+(0x19)*2, 0x00);


    }
    else if((pDemod->_sDMD_MSB124X_InitData.u8WO_SPI_Flash == 1)&&(pDemod->_sDMD_MSB124X_InitData.u8WO_Sdram==0))
    {
        //MS_U8 u8FirmwareType = MSB124X_DVBT;

        #ifdef MS_DEBUG
        u32tmm_11 = MsOS_GetSystemTime();
        #endif

        switch (u8Data)
        {
            case 1:
                u8FirmwareType = MSB124X_DVBT2;
                break;
            case 2:
            default:
                u8FirmwareType = MSB124X_DVBT;
                break;
            case 3:
                u8FirmwareType = MSB124X_DVBC;
                break;
            case 4:
                u8FirmwareType = MSB124X_DVBS2;
                break;
        }

        bRet &= _LoadDspCodeToSDRAM(u8FirmwareType);

        bRet &= _LoadSdram2Sram(u8FirmwareType);

        // Enable MCU
        bRet &= _MDrv_DMD_MSB124X_SetReg(0x0b00+(0x19)*2, 0x00);
    }
    else
    {
        #ifdef MS_DEBUG
        u32tmm_9 = MsOS_GetSystemTime();
        #endif
        // mask miu access for all and mcu
        bRet &= _MDrv_DMD_MSB124X_SetReg(0x1200+(0x23)*2 + 1,0x7f);
        bRet &= _MDrv_DMD_MSB124X_SetReg(0x1200+(0x23)*2,0xfe);
        // 10us delay
        MsOS_DelayTaskUs(10);

        // Disable MCU
        _MDrv_DMD_MSB124X_SetReg(0x0b00+(0x19)*2, 0x03);

        // Run code on bootloader
        _MDrv_DMD_MSB124X_SetReg(0x1000+(0x0c)*2, 0x02);

        // Assign f/w code type to load => 0: boot-loader 1: dvbt2, 2: dvbt, 3: dvbc
        _MDrv_DMD_MSB124X_SetReg(0x0900 + (0x4f) * 2, u8Data);


        // enable miu access of mcu gdma
        bRet &= _MDrv_DMD_MSB124X_SetReg(0x1200+(0x23)*2,0xf0);
        // 10us delay
        MsOS_DelayTaskUs(10);

        // Enable MCU
        _MDrv_DMD_MSB124X_SetReg(0x0b00+(0x19)*2, 0x00);

        #ifdef MS_DEBUG
        u32tmm_10 = MsOS_GetSystemTime();
        printf("[tmm8]t10 - t9 = %ld (%ld - %ld)\n",u32tmm_10-u32tmm_9,u32tmm_10,u32tmm_9);
        #endif


        do
        {
            _MDrv_DMD_MSB124X_GetReg(0x0900+(0x4f)*2, &u8DoneFlag);

            if (u32Timeout++ > 500)
                return FALSE;

            MsOS_DelayTaskUs(1*1000);

        }
        while(u8DoneFlag != 0xaa);

        #ifdef MS_DEBUG
        u32tmm_11 = MsOS_GetSystemTime();
        printf("[tmm8]t11 - t10 = %ld (%ld - %ld)\n",u32tmm_11-u32tmm_10,u32tmm_11,u32tmm_10);
        #endif

        // mask miu access for all and mcu
        bRet &= _MDrv_DMD_MSB124X_SetReg(0x1200+(0x23)*2 + 1,0x7f);
        bRet &= _MDrv_DMD_MSB124X_SetReg(0x1200+(0x23)*2,0xfe);
        // 10us delay
        MsOS_DelayTaskUs(10);

        // Disable MCU
        _MDrv_DMD_MSB124X_SetReg(0x0b00+(0x19)*2, 0x03);

        // Run code on loaded firmware
        _MDrv_DMD_MSB124X_SetReg(0x1000+(0x0c)*2, 0x05);

        do
        {
            _MDrv_DMD_MSB124X_GetReg(0x1000+(0x0c)*2, &u8DoneFlag);

            if (u32Timeout++ > 500)
                return FALSE;

            MsOS_DelayTaskUs(1*1000);

        }
        while(u8DoneFlag != 0x05);

        // enable miu access of mcu gdma
        bRet &= _MDrv_DMD_MSB124X_SetReg(0x1200+(0x23)*2,0xf0);
        // 10us delay
        MsOS_DelayTaskUs(10);

        // Enable MCU
        _MDrv_DMD_MSB124X_SetReg(0x0b00+(0x19)*2, 0x00);
    }

    #ifdef MS_DEBUG
    u32tmm_12 = MsOS_GetSystemTime();
    printf("[tmm8]t12 - t11 = %ld (%ld - %ld), TYPE is %d \n",u32tmm_12-u32tmm_11,u32tmm_12,u32tmm_11, eDMD_MSB124X_CurrentDemodulatorType);
    #endif

    switch(pDemod->eDMD_MSB124X_CurrentDemodulatorType)
    {
        case E_DMD_MSB124X_DEMOD_DVBT2:
            _DTV_DVBT2_DSPReg_Init();
            break;
        case E_DMD_MSB124X_DEMOD_DVBT:
            _DTV_DVBT_DSPReg_Init();
            break;
        case E_DMD_MSB124X_DEMOD_DVBC://mick
            _DTV_DVBC_DSPReg_Init();
            break;
        case E_DMD_MSB124X_DEMOD_DVBS2:
            printf("_DTV_DVBS_DSPReg_Init NULL\n");
            break;
        default:
            return FALSE;
    }

    #ifdef MS_DEBUG
    u32tmm_13 = MsOS_GetSystemTime();
    printf("[tmm8]t13 - t12 = %ld (%ld - %ld)\n",u32tmm_13-u32tmm_12,u32tmm_13,u32tmm_12);
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][end]LoadDspCode\n"));
    #endif

    return bRet;
}

static MS_BOOL _msb124x_flash_mode_en(void)
{
    MS_BOOL  bRet = TRUE;
    MS_U8    data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    MS_U8    u8MsbData[6] = {0};
    MS_U8    ch_num  = 3;
    MS_U8    u8Data  = 0;
    MS_U16   u16Addr = 0;
    MS_U8    retry_num = MSB124X_MAX_FLASH_ON_RETRY_NUM;
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    #ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][beg]msb124x_flash_mode_en\n"));
    #endif

    do{

        if (retry_num != MSB124X_MAX_FLASH_ON_RETRY_NUM)
        {
            ERR_DEMOD_MSB(printf("[msb124x][error]flash mode en fail.....retry=%d\n",retry_num);)
        }
        // bRet = TRUE;
        // password
        // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
        data[0] = 0x53;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, data);

        // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_
        data[0] = 0x71;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, data);

        // 8'hb2(SRID)->8,h81(CMD)  //TV.n_iic_sel_b0
        data[0] = ((ch_num & 0x01) != 0)? 0x81 : 0x80;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, data);

        // 8'hb2(SRID)->8,h83(CMD)  //TV.n_iic_sel_b1
        data[0] = ((ch_num & 0x02) != 0)? 0x83 : 0x82;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, data);

        // 8'hb2(SRID)->8,h84(CMD)  //TV.n_iic_sel_b2
        data[0] = ((ch_num & 0x04) != 0)? 0x85 : 0x84;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, data);

        // 8'hb2(SRID)->8,h53(CMD)  //TV.n_iic_ad_byte_en2, 32bit read/write
        data[0] = 0x53;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, data);

        // 8'hb2(SRID)->8,h7f(CMD)  //TV.n_iic_sel_use_cfg
        data[0] = 0x7f;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, data);

    /*
        // 8'hb2(SRID)->8,h35(CMD)  //TV.n_iic_use
        data[0] = 0x35;
        bRet &= iptr->WriteBytes(0, NULL, 1, data);

        // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_Re-shape
        data[0] = 0x71;
        bRet &= iptr->WriteBytes(0, NULL, 1, data);
    */
        bRet = TRUE;

        // confirm first, 0x99 and 0xaa.
        // beg read register
        u16Addr = 0x0900+(0x4f<<1);
        u8Data = 0x0;

        u8MsbData[0] = 0x10;
        u8MsbData[1] = 0x00;
        u8MsbData[2] = 0x00;
        u8MsbData[3] = (u16Addr >> 8) &0xff;
        u8MsbData[4] = u16Addr &0xff;

        u8MsbData[0] = 0x35;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

        u8MsbData[0] = 0x10;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8MsbData);
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_READ_BYTES, 0, NULL, 1, &u8Data);

        u8MsbData[0] = 0x34;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);
        // end read register

        if ((u8Data == 0x99) || (u8Data == 0xaa))
        {
            ERR_DEMOD_MSB(printf("[msb124x][warning]flash is already on....\n");)
            break;
        }
        // flash mode enable.
        // beg read register
        u16Addr = 0x0900+(0x28<<1)+1;
        u8Data = 0x0;

        u8MsbData[0] = 0x10;
        u8MsbData[1] = 0x00;
        u8MsbData[2] = 0x00;
        u8MsbData[3] = (u16Addr >> 8) &0xff;
        u8MsbData[4] = u16Addr &0xff;

        u8MsbData[0] = 0x35;

        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);



        u8MsbData[0] = 0x10;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8MsbData);
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_READ_BYTES, 0, NULL, 1, &u8Data);

        u8MsbData[0] = 0x34;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);
        // end read register

        // beg write register
        u16Addr = 0x0900+(0x28<<1) + 1;
        u8Data &= (0xff-0x01);

        u8MsbData[0] = 0x10;
        u8MsbData[1] = 0x00;
        u8MsbData[2] = 0x00;
        u8MsbData[3] = (u16Addr >> 8) &0xff;
        u8MsbData[4] = u16Addr &0xff;
        u8MsbData[5] = u8Data;

        u8MsbData[0] = 0x35;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

        u8MsbData[0] = 0x10;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 6, u8MsbData);

        u8MsbData[0] = 0x34;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);
        // end write register

        // beg write register
        u16Addr = 0x0900+(0x06<<1);
        u8Data = 0x10;

        u8MsbData[0] = 0x10;
        u8MsbData[1] = 0x00;
        u8MsbData[2] = 0x00;
        u8MsbData[3] = (u16Addr >> 8) &0xff;
        u8MsbData[4] = u16Addr &0xff;
        u8MsbData[5] = u8Data;

        u8MsbData[0] = 0x35;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

        u8MsbData[0] = 0x10;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 6, u8MsbData);

        u8MsbData[0] = 0x34;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);
        // end write register

        // beg write register

        u16Addr = 0x0900+(0x07<<1);
        u8Data = 0x10;

        u8MsbData[0] = 0x10;
        u8MsbData[1] = 0x00;
        u8MsbData[2] = 0x00;
        u8MsbData[3] = (u16Addr >> 8) &0xff;
        u8MsbData[4] = u16Addr &0xff;
        u8MsbData[5] = u8Data;

        u8MsbData[0] = 0x35;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

        u8MsbData[0] = 0x10;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 6, u8MsbData);

        u8MsbData[0] = 0x34;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);
    }while( (bRet == FALSE) && (retry_num-- != 0));
    // end write register

    #ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][end]msb124x_flash_mode_en,bRet=%d\n",bRet));
    #endif

    return bRet;
}

static MS_BOOL _msb124x_flash_boot_ready_waiting(MS_U8 *ptimeout)
{

    MS_BOOL  bRet = TRUE;
//    MAPI_U8    data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    MS_U8    u8MsbData[6] = {0};
    MS_U8    u8Data  = 0;
    MS_U16   u16Addr = 0;
    MS_U8    u8_timeout = 0;
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    #ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][beg]msb124x_flash_boot_ready_waiting\n"));
    #endif

    // wait for flash->dram ready.
    // read register

    u16Addr = 0x0900+(0x4f<<1);
    u8Data = 0x0;

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;

    u8_timeout = 0xff;

    while( (u8Data != 0x99) && (u8Data != 0xaa) && (u8_timeout-->0))
    {
        u8MsbData[0] = 0x35;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

        u8MsbData[0] = 0x10;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8MsbData);
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_READ_BYTES, 0, NULL, 1, &u8Data);

        u8MsbData[0] = 0x34;
        bRet &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);
        // 10ms
        MsOS_DelayTaskUs(1000*10);
    }
    // end read register
    *ptimeout = 0;
    if (u8_timeout == 0x00)
    {
        *ptimeout = 1;
        ERR_DEMOD_MSB(printf("[msb124x][error]msb124x_flash_boot_ready_waiting, timeout....\n");)
    }

    #ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb124x][end]msb124x_flash_boot_ready_waiting, t=%d\n",u8_timeout));
    #endif
    return bRet;
}

static MS_BOOL _msb124x_flash_WP_reg_read(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet;
    MS_U8   u8MsbData[5];
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;

    u8MsbData[0] = 0x35;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8MsbData);
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_READ_BYTES, 0, NULL, 1, pu8Data);

    u8MsbData[0] = 0x34;
    bRet=(pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);
    return bRet;
}

static MS_BOOL _msb124x_flash_WP_reg_write(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet;
    MS_U8   u8MsbData[6];
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    //bRet &= iptr->SetSpeed(0);


    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;
    u8MsbData[5] = u8Data;

    u8MsbData[0] = 0x35;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 6, u8MsbData);

    u8MsbData[0] = 0x34;
    bRet=(pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);
    return bRet;
}

static MS_BOOL _msbMSB124X_flash_WRSR(MS_U8 reg)
{
    MS_U8 bWriteData[5]={0x4D, 0x53, 0x54, 0x41, 0x52};
    MS_U8     bAddr[1];
    MS_BOOL   bRet = TRUE;
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, bWriteData);

    // WREN
    bAddr[0] = 0x10;
    bWriteData[0] = 0x06;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 1, bAddr, 1, bWriteData);

    bWriteData[0] = 0x12;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

    // WRSR
    bAddr[0] = 0x10;
    bWriteData[0] = 0x01;
    bWriteData[1] = reg;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 1, bAddr, 2, bWriteData);

    bWriteData[0] = 0x12;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

    // WRDI
    bAddr[0] = 0x10;
    bWriteData[0] = 0x04;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 1, bAddr, 1, bWriteData);

    bWriteData[0] = 0x12;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

    // end
    bWriteData[0] = 0x24;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

    #ifdef MS_DEBUG
    DBG_FLASH_WP(printf("[wb]msbMSB124X_flash_WRSR, reg=0x%x\n",reg);)
    #endif

    return bRet;
}

static MS_BOOL _msbMSB124X_flash_SRSR(MS_U8 *p_reg)
{
    MS_U8 bWriteData[5]={0x4D, 0x53, 0x54, 0x41, 0x52};
    MS_U8     bAddr[1];
    MS_BOOL   bRet = TRUE;
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, bWriteData);

    bAddr[0] = 0x10;
    bWriteData[0] = 0x05;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 1, bAddr, 1, bWriteData);

    bAddr[0] = 0x11;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_READ_BYTES, 1, bAddr, 1, p_reg);

    bWriteData[0] = 0x12;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

    // end
    bWriteData[0] = 0x24 ;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

    #ifdef MS_DEBUG
    DBG_FLASH_WP(printf("[wb]msbMSB124X_flash_SRSR, reg=0x%x\n",*p_reg);)
    #endif

    return bRet;
}

static MS_BOOL _msb124x_flash_WP(MS_U8 enable)
#if 0
{
    MAPI_U8 reg = 0;
    MAPI_BOOL bRet = true;
    MAPI_U8 u8_count = 0;

    DBG_FLASH_WP(printf("[wb]msb1233c_flash_WP_Enable=%d\n",enable);)

    if (enable == 1)
    {
      u8_count = 20;
      do
      {
        msb1233c_flash_SRSR(&reg);
        usleep(1*1000);
      }while(reg&0x01 && u8_count--);

      if (u8_count == 0)
      {
        bRet = false;
        DBG_FLASH_WP(printf("[wb]Err, flash_SRSR timeout!!!\n");)
        return bRet;
      }

      msb1233c_flash_WRSR(reg|0x9c);


      u8_count = 20;
      do
      {
        msb1233c_flash_SRSR(&reg);
        usleep(1*1000);
      }while(reg&0x01 && u8_count--);

      if (u8_count == 0)
      {
        bRet = false;
        DBG_FLASH_WP(printf("[wb]Err, flash_SRSR timeout!!!\n");)
        return bRet;
      }

      // active low
      // pull low
      bRet &= msb1233c_flash_WP_reg_read(0x0900+0x63*2+1, &reg);
      bRet &= msb1233c_flash_WP_reg_write(0x0900+0x63*2+1, reg&(~0x08));

      // gpio11 output enable
      bRet &= msb1233c_flash_WP_reg_read(0x0900+0x64*2+1, &reg);
      bRet &= msb1233c_flash_WP_reg_write(0x0900+0x64*2+1, reg&(~0x08));
    }
    else
    {
      // unactive high
      // pull high
      bRet &= msb1233c_flash_WP_reg_read(0x0900+0x63*2+1, &reg);
      bRet &= msb1233c_flash_WP_reg_write(0x0900+0x63*2+1, reg|0x08);

      // gpio11 output enable
      bRet &= msb1233c_flash_WP_reg_read(0x0900+0x64*2+1, &reg);
      bRet &= msb1233c_flash_WP_reg_write(0x0900+0x64*2+1, reg&(~0x08));

      u8_count = 20;
      do
      {
        msb1233c_flash_SRSR(&reg);
        usleep(1*1000);
      }while(reg&0x01 && u8_count--);

      if (u8_count == 0)
      {
        bRet = false;
        DBG_FLASH_WP(printf("[wb]Err, flash_SRSR timeout!!!\n");)
        return bRet;
      }

      msb1233c_flash_WRSR(reg&(~0x9c));

      u8_count = 20;
      do
      {
        msb1233c_flash_SRSR(&reg);
        usleep(1*1000);
      }while(reg&0x01 && u8_count--);

      if (u8_count == 0)
      {
        bRet = false;
        DBG_FLASH_WP(printf("[wb]Err, flash_SRSR timeout!!!\n");)
        return bRet;
      }
    }

    return bRet;
}
#else
{
    MS_U8 reg = 0;
    MS_BOOL bRet = TRUE;
    MS_U8 u8_count = 0;

    #ifdef MS_DEBUG
    DBG_FLASH_WP(printf("[wb]msb124x_flash_WP_Enable=%d\n",enable);)
    #endif

    if (enable == 1)
    {
        u8_count = 20;
        do
        {
            _msbMSB124X_flash_SRSR(&reg);
            MsOS_DelayTaskUs(1*1000);
        }while(reg&0x01 && u8_count--);

        if (u8_count == 0)
        {
            bRet = FALSE;
            #ifdef MS_DEBUG
            DBG_FLASH_WP(printf("[wb]Err, flash_SRSR timeout!!!\n");)
            #endif
            return bRet;
        }

        _msbMSB124X_flash_WRSR(reg|0x9c);
        u8_count = 20;

        do
        {
            _msbMSB124X_flash_SRSR(&reg);
            MsOS_DelayTaskUs(1*1000);
        }while(reg&0x01 && u8_count--);

        if (u8_count == 0)
        {
            bRet = FALSE;
            #ifdef MS_DEBUG
            DBG_FLASH_WP(printf("[wb]Err, flash_SRSR timeout!!!\n");)
            #endif
            return bRet;
        }

        // active low
        // pull low
        bRet &= _msb124x_flash_WP_reg_read(0x0A00+0x22*2, &reg);
        bRet &= _msb124x_flash_WP_reg_write(0x0A00+0x22*2, reg&(~0x01));
        // gpio11 output enable
        bRet &= _msb124x_flash_WP_reg_read(0x0A00+0x22*2, &reg);
        bRet &= _msb124x_flash_WP_reg_write(0x0A00+0x22*2, reg&(~0x02));
    }
    else
    {
        // unactive high
        // pull high
        bRet &= _msb124x_flash_WP_reg_read(0x0A00+0x22*2, &reg);
        bRet &= _msb124x_flash_WP_reg_write(0x0A00+0x22*2, reg|0x01);
        // gpio11 output enable
        bRet &= _msb124x_flash_WP_reg_read(0x0A00+0x22*2, &reg);
        bRet &= _msb124x_flash_WP_reg_write(0x0A00+0x22*2, reg&(~0x02));
        u8_count = 20;

        do
        {
            _msbMSB124X_flash_SRSR(&reg);
            MsOS_DelayTaskUs(1*1000);
        }while(reg&0x01 && u8_count--);

        if (u8_count == 0)
        {
            bRet = FALSE;
            #ifdef MS_DEBUG
            DBG_FLASH_WP(printf("[wb]Err, flash_SRSR timeout!!!\n");)
            #endif
            return bRet;
        }
        _msbMSB124X_flash_WRSR(reg&(~0x9c));
        u8_count = 20;

        do
        {
            _msbMSB124X_flash_SRSR(&reg);
            MsOS_DelayTaskUs(1*1000);
        }while(reg&0x01 && u8_count--);

        if (u8_count == 0)
        {
            bRet = FALSE;
            #ifdef MS_DEBUG
            DBG_FLASH_WP(printf("[wb]Err, flash_SRSR timeout!!!\n");)
            #endif
            return bRet;
        }
    }
    return bRet;
}
#endif

MS_U16 _MSB124X_CHIP_MATCH_TABLE[] =
{
    //Kaiser, Kaiserin, Keltic, Kronus, Kappa
    0x56,       0x41,     0x72,  0x2F,  0x75,
};
/*
static MS_BOOL _msbMSB124X_set_bonding_option(MS_U16 u16ChipID)
{
    MS_BOOL  bRet = TRUE;
    MS_U8    u8Idx;
    MS_U8    u8MatchFlag  = 0;
    MS_U8    u8Data  = 0;
    MS_U32    u32_timeout = 0;
    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    #endif

    printf("_msbMSB124X_set_bonding_option u16ChipID %x\n", u16ChipID);

    for (u8Idx = 0 ; u8Idx < sizeof( _MSB124X_CHIP_MATCH_TABLE) ; u8Idx++)
    {
        if(u16ChipID == _MSB124X_CHIP_MATCH_TABLE[u8Idx])
        {
            u8MatchFlag = 0x01;
            break;
        }
        else
        {
            u8MatchFlag = 0x00;
        }
    }

    if (_MSB124X_I2C_CH_Reset(3) == FALSE)
    {
        printf(">>>MSB124X CH3 Reset:Fail\n");
        return FALSE;
    }

    // MSB124X : 0x0902[8]=0 , 0x0902[0]=0;
    // MSB1235 : 0x0902[8]=1 , 0x0902[0]=1; (before overwrite, SDRAM not enable)
    //                  0x0902[8]=1 , 0x0902[0]=0; (after overwrite, SDRAM enable)
    // check bonding value, 0x0902[8]
    bRet &= _MDrv_DMD_MSB124X_GetReg(0x0900+(0x02)*2+1, &u8Data);
    if((u8Data & 0x01) == 0x01) //for MSB1236C
    {
        if(u8MatchFlag == 0x01)
        {
            //check overwrite or not
            //0x0902[0] : reg_bonding[0]
            bRet &= _MDrv_DMD_MSB124X_GetReg(0x0900+(0x02)*2, &u8Data);
            if((u8Data & 0x01) != 0x00)
            {
                //0x0905[0] : reg_bond_ov_en[0] = 1
                //0x0905[8] : reg_bond_ov[0] = 0
                // set overwrite enable
                bRet &= _MDrv_DMD_MSB124X_SetReg(0x0900+(0x05)*2, 0x01);
                // set overwrite value
                bRet &= _MDrv_DMD_MSB124X_SetReg(0x0900+(0x05)*2+1, 0x00);
            }

            do
            {
                bRet &= _MDrv_DMD_MSB124X_GetReg(0x0900+(0x02)*2, &u8Data);
                if(u32_timeout++ > 500)
                {
                    ERR_DEMOD_MSB(printf("@msb124x, Set bonding option failure.!!!\n"));
                    return FALSE;
                }
            }while((u8Data & 0x01) == 0x01);

            #ifdef MS_DEBUG
            printf("@ Set bonding option for MSB1236 \n");
            #endif
        }
        else
        {
            return FALSE;
        }
    }
    else  // for MSB124X
    {
        //check overwrite or not
        //0x0902[0] : reg_bonding[0]
        bRet &= _MDrv_DMD_MSB124X_GetReg(0x0900+(0x02)*2, &u8Data);
        if((u8Data & 0x01) != 0x00)
        {
            //0x0905[0] : reg_bond_ov_en[0] = 1
            //0x0905[8] : reg_bond_ov[0] = 0
            // set overwrite enable
            bRet &= _MDrv_DMD_MSB124X_SetReg(0x0900+(0x05)*2, 0x01);
            // set overwrite value
            bRet &= _MDrv_DMD_MSB124X_SetReg(0x0900+(0x05)*2+1, 0x00);
        }

        do
        {
            bRet &= _MDrv_DMD_MSB124X_GetReg(0x0900+(0x02)*2, &u8Data);
            if(u32_timeout++ > 500)
            {
                ERR_DEMOD_MSB(printf("@msbMSB124X, Set bonding option failure.!!!\n"));
                return FALSE;
            }
        }while((u8Data & 0x01) == 0x01);

        #ifdef MS_DEBUG
        printf("@ Set bonding option for MSB124X \n");
        #endif
    }

    #ifdef MS_DEBUG
    if (!bRet) printf("%s %d Error\n", __func__, __LINE__);
    #endif
    return bRet;
}
*/
static MS_BOOL _IspCheckVer(MS_U8* pLibData, MS_BOOL* pMatch)
{
    MS_U8  bReadData[VERSION_CODE_SIZE];
    MS_U32  indx = 0;
    *pMatch = true;
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    MS_U8 bWriteData[5] = {0x4D, 0x53, 0x54, 0x41, 0x52};
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, bWriteData);

    MS_U8    bAddr[1], bError = true;
    //MAPI_U16   Count;

    memset(bReadData, 0 , sizeof(bReadData));

    bAddr[0] = 0x10;
    //dwStartAddr=0;

    bWriteData[0] = 0x03;
    bWriteData[1] = VERSION_CODE_ADDR >> 16;
    bWriteData[2] = VERSION_CODE_ADDR >> 8;
    bWriteData[3] = VERSION_CODE_ADDR & 0xFF;

    bError &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 1, bAddr, 4, bWriteData);

    bAddr[0] = 0x11;
    bError &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_READ_BYTES, 1, bAddr, VERSION_CODE_SIZE, bReadData);

    bWriteData[0] = 0x12;
    bError &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1,  bWriteData);

    if(FALSE == bError)
    {
        bWriteData[0] = 0x24 ;
        (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);
        return FALSE;
    }

    bWriteData[0] = 0x24 ;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

    printf("sttest version data = ");
    for(indx = 0; indx < (VERSION_CODE_SIZE); indx++)
    {
        printf(" %x ,", bReadData[indx]);
        if(pLibData[indx+VERSION_CODE_ADDR] != bReadData[indx])
        {
            *pMatch = FALSE;
            //break;
        }
    }
    printf(" \n");

    return TRUE;
}

static MS_BOOL _dram_crc_check(MS_U16 chksum_lib, MS_BOOL* pMatch)
{
    MS_U16  chksum = 0;
    // MAPI_U16  chksum_lib = 0;
    MS_U16  u16Addr = 0;
    MS_U8   u8MsbData[5];
    MS_U8   reg = 0;
    MS_BOOL bRet;
    MS_U8   mcu_status = 0;
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    *pMatch = false;

    // MAPI_U8 bWriteData[5]={0x4D, 0x53, 0x54, 0x41, 0x52};
    // iptr->WriteBytes(0, NULL, 5, bWriteData);

/// crc H byte
    u16Addr = 0x0c00+0x0d*2+1;

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;

    u8MsbData[0] = 0x35;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8MsbData);
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_READ_BYTES, 0, NULL, 1, &reg);

    u8MsbData[0] = 0x34;
    bRet=(pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);


   chksum = reg;

/// crc L byte
    u16Addr = 0x0c00+0x0d*2;

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;

    u8MsbData[0] = 0x35;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8MsbData);
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_READ_BYTES, 0, NULL, 1, &reg);

    u8MsbData[0] = 0x34;
    bRet=(pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);


   chksum = (chksum<<8)|reg;

// get mcu status

    u16Addr = 0x0900+0x4f*2;

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;

    u8MsbData[0] = 0x35;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8MsbData);
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_READ_BYTES, 0, NULL, 1, &reg);

    u8MsbData[0] = 0x34;
    bRet=(pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);


    mcu_status = reg;


/// check the crc in dsp lib array

    if (mcu_status == 0xaa && ((chksum_lib&0xff00) == (chksum&0xff00)) )
      *pMatch = true;
    else if(chksum_lib == chksum)
      *pMatch = true;

    #ifdef MS_DEBUG
    printf("[crc]chksum_lib=0x%x, chksum=0x%x, bRet=%d, Match=%d, mcu_status=0x%x\n",chksum_lib,chksum,bRet,*pMatch,mcu_status);
    #endif

    return bRet;
}

static MS_BOOL _IspProcFlash(MS_U8* pLibArry, MS_U32 dwLibSize)
{
    MS_U32    dwLoop, dwTimeOut;
    MS_U32    dwStartAddr, dwEndAddr;
    //MAPI_U16    wLoop;
    MS_U8     bError = false;//, bWriteData[PAGE_WRITE_SIZE];
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    MS_U8 bWriteData[5]={0x4D, 0x53, 0x54, 0x41, 0x52};
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, bWriteData);

    dwStartAddr = (MS_U32) 0;
    dwEndAddr = dwLibSize;


    MS_U8     bAddr[1];
    MS_U8     bReadData=0;
    for(dwLoop=dwStartAddr; (dwLoop < dwEndAddr); dwLoop+=PAGE_WRITE_SIZE)
    {

        dwTimeOut = 10000;

        while(dwTimeOut--)
        {
            bAddr[0] = 0x10;
            bWriteData[0] = 0x05;
            (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 1, bAddr, 1, bWriteData);

            bAddr[0] = 0x11;
            (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_READ_BYTES, 1, bAddr, 1, &bReadData);

            bWriteData[0] = 0x12;
            (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

            if (!(bReadData & 0x01))
                break;

            if(dwTimeOut==1)
            {
                bError = 1;
                break;
            }
            MsOS_DelayTaskUs(0);
        }

        if(!bError)
        {

            MS_U8    bAddr[5], bWriteData[1];
            MS_BOOL bError = TRUE;

            bAddr[0] = 0x10;
            bWriteData[0] = 0x06;
            bError &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 1, bAddr, 1, bWriteData);

            bWriteData[0] = 0x12;
            bError &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

            // Page Program
            bAddr[0] = 0x10;
            bAddr[1] = 0x02;
            bAddr[2] = dwLoop >> 16;
            bAddr[3] = dwLoop >> 8;
            bAddr[4] = dwLoop;

            bError &= (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 5, bAddr, PAGE_WRITE_SIZE, (pLibArry+dwLoop));

            bWriteData[0] = 0x12;
            bError &=  (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

            bAddr[0] = 0x10;
            bWriteData[0] = 0x04;
            bError &=  (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 1, bAddr, 1, bWriteData);

            bWriteData[0] = 0x12;
                bError &=  (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

            if(bError == FALSE)
            {
                break;
            }
        }
    }

    bWriteData[0]=0x24 ;
    (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

    if(bError==FALSE)
        return TRUE;
    else
        return FALSE;

}

void MDrv_DMD_MSB124X_SetCurrentDemodulatorType(eDMD_MSB124X_DemodulatorType eCurrentDemodulatorType)
{
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();
    #ifdef MS_DEBUG
    printf("MDrv_DMD_MSB124X_SetCurrentDemodulatorType %d\n", eCurrentDemodulatorType);
    #endif
    pDemod->eDMD_MSB124X_CurrentDemodulatorType = eCurrentDemodulatorType;
}

MS_BOOL MDrv_DMD_MSB124X_LoadDSPCode(void)
{
    MS_BOOL bRet = TRUE;
    DMD_LOCK();
    bRet = _LoadDSPCode();
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_MSB124X_DTV_DVBT_DSPReg_CRC(void)
{
    MS_BOOL bRet = TRUE;
    DMD_LOCK();
    bRet = _DTV_DVBT_DSPReg_CRC();
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_MSB1245_LoadDSPCodeToSram(void)
{
    MS_BOOL bRet=true;
    MS_U16 u16dat_size;
    MS_U16 u16Address;
    MS_U8 u8Data;
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    DMD_LOCK();

    if (pDemod->_sDMD_MSB124X_InitData.bPreloadDSPCodeFromMainCHIPI2C)
    {
        DMD_UNLOCK();
        return TRUE;
    }

    //change I2C channel
    bRet &=_MSB124X_I2C_CH_Reset(3);
    if (bRet==FALSE)
    {
            PRINTE(("utopia MSB1245 i2c change fail!!!\n"));
    }
    else
    {
            PRINTE(("utopia MSB1245 i2c change success!!!\n"));
    }


    //HW version Check
    u16Address=0x0900;
    bRet &=_MDrv_DMD_MSB124X_GetReg(u16Address, &u8Data);
    if (bRet==FALSE)
    {
        PRINTE(("utopia MSB1245 i2c read reg fail!!!\n"));
    }
    else
    {
        PRINTE(("utopia MSB1245 HW version: 0x%x=0x%x\n", u16Address, u8Data));
    }

    u16dat_size=sizeof(MSB1245_LIB);
    if (pDemod->_sDMD_MSB124X_InitData.bEnableSPILoadCode)
    {
        //SPI load code
        (pDemod->_sDMD_MSB124X_InitData.fpMSB124x_SPIPAD_En)(TRUE);
        // ------enable to use TS_PAD as SSPI_PAD
        // [0:0] reg_en_sspi_pad
        // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900 + (0x3b) * 2, 0x0002);
        //Turn off all pad in
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900 + (0x28) * 2, 0x0000);
        //Transport Stream pad on
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900 + (0x2d) * 2, 0x00ff);
        // ------- MSPI protocol setting
        // [8] cpha
        // [9] cpol
        _MDrv_DMD_MSB124X_GetReg(0x0900+(0x3a)*2+1,&u8Data);
        u8Data &= 0xFC;
        _MDrv_DMD_MSB124X_SetReg(0x0900+(0x3a)*2+1, u8Data);

	   // -------------------------------------------------------------------
       // Initialize DMD_ANA_MISC
       // -------------------------------------------------------------------
       // [0]  reg_tst_ldo25i
       // [1]  reg_tst_ldo25q
       // [5:4]    reg_tst_ldo25i_selfb
       // [7:6]    reg_tst_ldo25q_selfb
       // [8]  reg_pd_dm2p5ldoi = 1'b0
       // [9]  reg_pd_dm2p5ldoq = 1'b0
       bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x4f)*2, 0x0000);

       // [0]  reg_tst_ldo11_clk
       // [1]  reg_tst_ldo26
       // [2]  reg_tst_ldo11_cmp
       // [3]  reg_pd_dm1p1ldo_clk = 1'b0
       // [4]  reg_pd_dm1p1ldo_cmp = 1'b0
       // [6]  reg_tst_ldo26_selfb
       // [7]  reg_pd_dm2p6ldo = 1'b0
       // [9:8]    reg_tst_ldo11_cmp_selfb
       // [11:10]  reg_tst_ldo11_clk_selfb
       bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x4e)*2, 0x0000);

       // [1:0]    reg_mpll_loop_div_first       feedback divider 00:div by 1 01:div by 2 10:div by 4 11:div by 8
       // [15:8]   reg_mpll_loop_div_second      feedback divider, div by binary data number
       bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x33)*2, 0x1201);

       // [2:0]    reg_mpll_ictrl          charge pump current control
       // [3]  reg_mpll_in_sel         1.8V or 3.3V reference clock domain select (1'b0=0==>3.3 V reference clock domain)
       // [4]  reg_mpll_xtal2adc_sel       select the XTAL clock bypass to MPLL_ADC_CLK
       // [5]  reg_mpll_xtal2next_pll_sel  crystal clock bypass to next PLL select
       // [6]  reg_mpll_vco_offset     set VCO initial offset frequency
       // [7]  reg_mpll_pd         gated reference clock and power down PLL analog_3v: 1=power down
       // [8]  reg_xtal_en         XTAL enable register; 1: enable
       // [10:9]   reg_xtal_sel            XTAL driven strength select.
       // [11]     reg_mpll_porst          MPLL input  power on reset, connect to reg as MPLL_RESET
       // [12]     reg_mpll_reset          PLL software reset; 1:reset
       // [13]     reg_pd_dmpll_clk        XTAL to MPLL clock reference power down
       // [14]     reg_pd_3p3_1            XTAL to CLK_24M_3P3_1 power down
       // [15]     reg_pd_3p3_2            XTAL to CLK_24M_3P3_2 power down
       bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x35)*2, 0x1803);
       bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x35)*2, 0x0003);

       // [0]  reg_mpll_clk_dp_pd  dummy
       // [1]  reg_adc_clk_pd      ADC output clock power down
       // [2]  reg_mpll_div2_pd    MPLL_DIV2 power down
       // [3]  reg_mpll_div3_pd    MPLL_DIV3 power down
       // [4]  reg_mpll_div4_pd    MPLL_DIV4 power down
       // [5]  reg_mpll_div8_pd    MPLL_DIV8 power down
       // [6]  reg_mpll_div10_pd   MPLL_DIV10 power down
       // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h30, 2'b11, 16'h2400);  // divide ADC clock to 24Mhz = 24*36/36
       bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0A00+(0x30)*2, 0x0800);

       //set MCU CLK to 108MHz
       bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x0b)*2, 0x0020);
       bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900+(0x0b)*2, 0x0000);

        bRet &=_Load2Sram(MSB1245_LIB, u16dat_size,0);


        // ------disable to use TS_PAD as SSPI_PAD after load code
        // [0:0] reg_en_sspi_pad
        // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900 + (0x3b) * 2, 0x0001);
        //Transport Stream pad off
        bRet &= _MDrv_DMD_MSB124X_SetReg2Bytes(0x0900 + (0x2d) * 2, 0x0000);
        (pDemod->_sDMD_MSB124X_InitData.fpMSB124x_SPIPAD_En)(FALSE);

         //  Set Inst map for Program SRAM
        //set lower bound "SRAM_A_START_ADDR"
         bRet &= _MDrv_DMD_MSB124X_SetReg(0x1004, 0x00);
         bRet &= _MDrv_DMD_MSB124X_SetReg(0x1005, 0x00);
         bRet &= _MDrv_DMD_MSB124X_SetReg(0x1000, 0x00);
        //set upper bound "SRAM_A_END_ADDR"
         bRet &= _MDrv_DMD_MSB124X_SetReg(0x1006, 0x7F);
         bRet &= _MDrv_DMD_MSB124X_SetReg(0x1007, 0xFF);
         bRet &= _MDrv_DMD_MSB124X_SetReg(0x1002, 0x00);
         // Enable Program SRAM
         bRet &= _MDrv_DMD_MSB124X_SetReg(0x1018, 0x01);

         //  End MCU reset
         bRet &= _MDrv_DMD_MSB124X_SetReg(0x0b32, 0x00);

        if (bRet==FALSE)
        {
            PRINTE(("utopia MSB1245 SPI Load FW fail!!!\n"));
        }
        else
        {
            PRINTE(("utopia MSB1245 SPI Load FW success!!!\n"));
        }

        DMD_UNLOCK();
        return TRUE;
    }
    else
    {
        //I2C load code
        bRet &=_Load2Sram(MSB1245_LIB, u16dat_size,0);
        if (bRet==FALSE)
        {
            PRINTE(("utopia MSB1245 I2C Load FW fail!!!\n"));
        }
        else
        {
            PRINTE(("utopia MSB1245 I2C Load FW success!!!\n"));
        }
        DMD_UNLOCK();
        return TRUE;
    }

}

MS_BOOL MDrv_DMD_MSB124X_Power_On(void)
{
    MS_U8     status = TRUE;
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();

    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    #endif
/*
         printf("ppp used \n");
   if ( _msbMSB124X_set_bonding_option( MDrv_SYS_GetChipID() ) == FALSE )
    {
    printf("hhhhh-\n");
  //      return TRUE;
    }
   printf("hihi \n");
*/
    status &= _MSB124X_I2C_CH_Reset(3);
    status &= _MSB124X_HW_init();

    if(pDemod->_sDMD_MSB124X_InitData.u8WO_Sdram==0)//with sdram on board
    {
        status &= _LoadDspCodeToSDRAM(MSB124X_ALL);//need boot code into sdram
    }

    if (_LoadDSPCode() == FALSE)
    {
        printf(">>>>MSB124X:Fail\n");
        status= FALSE;
    }
    return TRUE;
}

MS_BOOL MDrv_DMD_MSB124X_Power_On_Initialization(void)
{
    MS_U8     status = TRUE;
    MS_BOOL   bMatch = false;
    MS_U8     u8RetryCnt = 6;
    tMSB124XData *pDemod = DEMOD_GET_ACTIVE_NODE();
    #ifdef MS_DEBUG
    MS_U32 u32tmm_1, u32tmm_2, u32tmm_3, u32tmm_4, u32tmm_5, u32tmm_6 = 0x00;
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    #endif

    DMD_LOCK();
    #ifdef MS_DEBUG
    u32tmm_1 = MsOS_GetSystemTime();
    #endif

    if (pDemod->_sDMD_MSB124X_InitData.bPreloadDSPCodeFromMainCHIPI2C)
    {
        DMD_UNLOCK();
        return TRUE;
    }

    #ifdef MS_DEBUG
    if (eDMD_MSB124X_DbgLevel >= E_DMD_MSB124X_DBGLV_DEBUG)
    {
        printf(">>>MSB124X: Enter Power_On_Initialization()\n");
    }
    #endif

    //if ( _msbMSB124X_set_bonding_option( MDrv_SYS_GetChipID() ) == FALSE )
    //{
    //    DMD_UNLOCK();
    //    return TRUE;
    //}

    if (pDemod->bDMD_MSB124X_Power_init_en == TRUE)
    {
        DMD_UNLOCK();
        return  TRUE;
    }
    else
    {
        pDemod->bDMD_MSB124X_Power_init_en = (pDemod->u8DMD_MSB124X_PowerOnInitialization_Flow == 2) ? (TRUE) : (FALSE);
    }

    if(pDemod->_sDMD_MSB124X_InitData.u8WO_SPI_Flash== 1)
    {
        if (pDemod->_sDMD_MSB124X_InitData.bPreloadDSPCodeFromMainCHIPI2C)
        {

        }
        else
        {
            status &= _MSB124X_I2C_CH_Reset(3);
            status &= _MSB124X_HW_init();
            if(pDemod->_sDMD_MSB124X_InitData.u8WO_Sdram==0)//with sdram on board
            {
            status &= _LoadDspCodeToSDRAM(MSB124X_ALL);
      }

            if (_LoadDSPCode() == FALSE)
            {
                printf(">>>>MSB124X:Fail\n");
                status= FALSE;
            }
        }
    }
    else
    {
        #ifdef MS_DEBUG
        if (eDMD_MSB124X_DbgLevel >= E_DMD_MSB124X_DBGLV_DEBUG)
        {
            printf("u8DMD_MSB124X_PowerOnInitialization_Flow = %d\n", u8DMD_MSB124X_PowerOnInitialization_Flow);
        }
        #endif
        if(pDemod->u8DMD_MSB124X_PowerOnInitialization_Flow ==0)
        {
                status = _msb124x_flash_mode_en();
                if (status == FALSE)
                {
                    printf("[msb124x][error]msb124x_flash_mode_en fail....\n");
                }
                pDemod->u8DMD_MSB124X_PowerOnInitialization_Flow++;
        }
        else
        {
            if(pDemod->u8DMD_MSB124X_PowerOnInitialization_Flow<2)
            {
                pDemod->u8DMD_MSB124X_PowerOnInitialization_Flow++;
            }

            MS_U8     u8DoneFlag = 0;
            MS_U16    u16_counter = 0;

            MS_U16    crc16 = 0;

            crc16 = MSB124X_LIB[sizeof(MSB124X_LIB)-2];
            crc16 = (crc16<<8)|MSB124X_LIB[sizeof(MSB124X_LIB)-1];

            do
            {
                printf(">>>MSB124X: u8RetryCnt = %d\n",u8RetryCnt);

                MS_U8 flash_waiting_ready_timeout = 0;
                u8RetryCnt--;

                #ifdef MS_DEBUG
                u32tmm_3 = MsOS_GetSystemTime();
                #endif

                status = _msb124x_flash_boot_ready_waiting(&flash_waiting_ready_timeout);
                if ( (flash_waiting_ready_timeout == 1) || (status == FALSE) )
                {
                    printf("[msb124x][error]msb124x_flash_boot_ready_waiting fail....\n");
                }

                #ifdef MS_DEBUG
                u32tmm_4 = MsOS_GetSystemTime();
                printf("[tmm1]t4-t3 = %ld (%ld - %ld)\n",u32tmm_4-u32tmm_3,u32tmm_4,u32tmm_3);
                #endif

                if(status == FALSE)
                {
                    if (pDemod->_sDMD_MSB124X_InitData.fpGPIOReset != NULL)
                    {
                        #ifdef MS_DEBUG
                        printf(">>>MSB124X: Reset Demodulator\n");
                        #endif
                        (pDemod->_sDMD_MSB124X_InitData.fpGPIOReset)(FALSE); // gptr->SetOff();
                        MsOS_DelayTaskUs(resetDemodTime*1000);
                        (pDemod->_sDMD_MSB124X_InitData.fpGPIOReset)(TRUE); // gptr->SetOn();
                        MsOS_DelayTaskUs(waitFlashTime * 1000);
                    }

                    if (_MSB124X_I2C_CH_Reset(3) == FALSE)
                    {
                        printf(">>>MSB124X CH Reset:Fail\n");
                        status= FALSE;
                        continue;
                    }

                    u16_counter = 1000;
                    do
                    {
                        // 10 ms
                        MsOS_DelayTaskUs(10*1000);
                        u16_counter--;
                        _MDrv_DMD_MSB124X_GetReg(0x0900+(0x4f)*2, &u8DoneFlag);
                    } while(u8DoneFlag != 0x99 && u16_counter != 0);

                    if(u16_counter == 0 && u8DoneFlag != 0x99)
                    {
                        printf("[wb]Err, MSB124X didn't ready yet\n");
                        status = false;
                    }
                    else
                    {
                        status = TRUE;
                    }

                }
                // No need to switch to CH0 before SPI Flash access.

                {
                    #ifdef MS_DEBUG
                    if (eDMD_MSB124X_DbgLevel >= E_DMD_MSB124X_DBGLV_DEBUG)
                    {
                        printf(">>>MSB124X: Check Version...");
                    }
                    #endif

                    if (_IspCheckVer(MSB124X_LIB, &bMatch) == FALSE)
                    {
                        printf(">>> ISP read FAIL!\n");
                        status= FALSE;
                        continue;
                    }


                    if(bMatch == FALSE)
                    {
                      printf(">>> IspCheckVer FAIL!\n");
                    }
                    else
                    {
                        if (_MSB124X_I2C_CH_Reset(3) == FALSE)
                        {
                            printf(">>>MSB124X CH Reset:Fail\n");
                            status= FALSE;
                            continue;
                        }
                        else
                        {
                            #ifdef MS_DEBUG
                            if (eDMD_MSB124X_DbgLevel >= E_DMD_MSB124X_DBGLV_DEBUG)
                            {
                                printf(">>>MSB124X CH Reset:OK\n");
                            }
                            #endif
                        }

                        if (_dram_crc_check(crc16, &bMatch) == FALSE)
                        {
                            printf(">>> reg read fail!\n");
                            status= FALSE;
                            continue;
                        }

                        if(bMatch == FALSE)
                            printf(">>> dram crc check FAIL!\n");
                        else
                            printf(">>> dram crc check OK!\n");


                    }

                    if (pDemod->_sDMD_MSB124X_InitData.bFlashWPEnable)
                    {
                        if (bMatch == FALSE)
                        {
                            // disable flash WP, pull high.
                            if(_msb124x_flash_WP(0) == FALSE)
                            {
                                printf("[wb]Err, FLASH WP Disable Fail!!!\n");
                            }
                            MsOS_DelayTaskUs(100*1000);
                        }
                    }

                    ////bMatch = true; //FIXME : Remove this to enable auto FW reload.
                    if (bMatch == FALSE)// Version not match
                    {
                        MS_U8 bAddr[1];
                        MS_U8 bWriteData[5]={0x4D, 0x53, 0x54, 0x41, 0x52};

                        printf(">>> Not match! Reload Flash...");
                        if ( (sizeof(MSB124X_LIB)%256) != 0)
                        {
                            printf(" MSB124X_LIB 256byte alignment error!%u \n",sizeof(MSB124X_LIB));
                        }

                        (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, bWriteData);

                        bAddr[0] = 0x10;
                        bWriteData[0] = 0x06;
                        (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 1, bAddr, 1, bWriteData);

                        bWriteData[0] = 0x12;
                        (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

                        bAddr[0] = 0x10;

                        bWriteData[0] = 0xC7;
                        (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 1, bAddr, 1, bWriteData);

                        bWriteData[0] = 0x12;
                        (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

                        bWriteData[0]=0x24 ;
                        (pDemod->_sDMD_MSB124X_InitData.fpMSB124X_I2C_Access)(E_DMD_MSB124X_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB124X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

                        #ifdef MS_DEBUG
                        printf("\t\t\tStart   %ld\n", MsOS_GetSystemTime());
                        #endif
                        if ( (sizeof(MSB124X_LIB) - 2) > MAX_MSB124X_LIB_LEN)
                        {
                          printf("Err, msb124xc_lib size(%d) is larger than flash size(%d)\n", sizeof(MSB124X_LIB), MAX_MSB124X_LIB_LEN);
                        }

                        // if (IspProcFlash(MSB124X_LIB, sizeof(MSB124X_LIB)) == MAPI_FALSE)
                        if (_IspProcFlash(MSB124X_LIB, sizeof(MSB124X_LIB)-2) == FALSE)
                        {
                            printf("ISP write FAIL\n");
                            status= FALSE;
                            continue;
                        }
                        else
                        {
                            #ifdef MS_DEBUG
                            printf("\t\t\tEnd   %ld\n", MsOS_GetSystemTime());
                            #endif
                            //check again
                           if ((_IspCheckVer(MSB124X_LIB, &bMatch) == FALSE)||(bMatch==false))
                            {
                                printf(">>> ISP read FAIL! bMatch %d \n",bMatch);
                                status= FALSE;
                                continue;
                            }
                            else // reset again
                            {
                                if (pDemod->_sDMD_MSB124X_InitData.fpGPIOReset != NULL)
                                {
                                    #ifdef MS_DEBUG
                                    printf(">>>MSB124X[2]: Reset Demodulator\n");
                                    #endif
                                    (pDemod->_sDMD_MSB124X_InitData.fpGPIOReset)(FALSE); // gptr->SetOff();
                                    MsOS_DelayTaskUs(resetDemodTime*1000);
                                    (pDemod->_sDMD_MSB124X_InitData.fpGPIOReset)(TRUE); // gptr->SetOn();
                                    MsOS_DelayTaskUs(waitFlashTime * 1000);
                                }

                                if (_MSB124X_I2C_CH_Reset(3) == FALSE)
                                {
                                    printf(">>>MSB124X CH Reset:Fail\n");
                                    status= FALSE;
                                    continue;
                                }

                                u16_counter = 1000;
                                do
                                {
                                    // 10 ms
                                    MsOS_DelayTaskUs(10*1000);
                                    u16_counter--;
                                    _MDrv_DMD_MSB124X_GetReg(0x0900+(0x4f)*2, &u8DoneFlag);
                                } while(u8DoneFlag != 0x99 && u16_counter != 0);

                                if(u16_counter == 0 && u8DoneFlag != 0x99)
                                {
                                    printf("[wb]Err, MSB124X didn't ready yet\n");
                                    status = FALSE;
                                }
                                else
                                    status = TRUE;

                                if (_MSB124X_I2C_CH_Reset(3) == FALSE)
                                {
                                    printf(">>>MSB124X CH Reset:Fail\n");
                                    status= FALSE;
                                    continue;
                                }
                                else
                                {
                                    printf(">>>MSB124X CH Reset:OK\n");
                                }

                                if (_dram_crc_check(crc16, &bMatch) == FALSE)
                                {
                                    printf(">>> reg read fail!\n");
                                    status= FALSE;
                                    continue;
                                }

                                if(bMatch == false)
                                  printf(">>> dram crc check FAIL!\n");
                                else
                                  printf(">>> dram crc check OK!\n");
                            }
                            #ifdef MS_DEBUG
                            printf(" OK\n");
                            #endif
                        }
                    }
                    else
                    {   // Version match, do nothing
                        #ifdef MS_DEBUG
                        printf(">>> Match\n");
                        #endif
                    } // if (bMatch == false)

                    if (pDemod->_sDMD_MSB124X_InitData.bFlashWPEnable)
                    {
                        if (bMatch == TRUE)
                        {
                            // Enable flash WP, pull high.
                            if(_msb124x_flash_WP(1) == FALSE)
                            {
                                printf("[wb]Err, FLASH WP Enable Fail!!!\n");
                            }
                            MsOS_DelayTaskUs(100*1000);
                        }
                    }  // bFlashWPEnable
                }


                if (_MSB124X_I2C_CH_Reset(3) == FALSE)
                {
                    printf(">>>MSB124X CH Reset:Fail\n");
                    status= FALSE;
                    continue;
                }
                else
                {
                    #ifdef MS_DEBUG
                    printf(">>>MSB124X CH Reset:OK\n");
                    #endif
                }

                #ifdef MS_DEBUG
                u32tmm_5 = MsOS_GetSystemTime();
                #endif

                if (_LoadDSPCode() == FALSE)
                {
                    printf(">>>>MSB124X:Fail\n");
                    status= FALSE;
                    continue;
                }
                else
                {
                    #ifdef MS_DEBUG
                    printf(">>>MSB124X:OK\n");
                    #endif
                }

                #ifdef MS_DEBUG
                u32tmm_6 = MsOS_GetSystemTime();
                printf("[tmm1]t6-t5 = %ld (%ld - %ld)\n",u32tmm_6-u32tmm_5,u32tmm_6,u32tmm_5);
                #endif

                #ifdef MS_DEBUG
                {
                    MS_U8 u8ChipRevId = 0;
                    _MDrv_DMD_MSB124X_GetReg(0x0900+(0x01)*2, &u8ChipRevId);
                    printf(">>>MSB124X:Edinburgh RevID:%x\n", u8ChipRevId);


                    _MDrv_DMD_MSB124X_GetReg(0x0900+(0x49)*2, &u8ChipRevId);
                    printf(">>>MSB124X:Edinburgh 0x49_L:%x\n", u8ChipRevId);

                    _MDrv_DMD_MSB124X_GetReg(0x0900+(0x49)*2+1, &u8ChipRevId);
                    printf(">>>MSB124X:Edinburgh 0x49_H:%x\n", u8ChipRevId);

                    _MDrv_DMD_MSB124X_GetReg(0x0900+(0x4A)*2, &u8ChipRevId);
                    printf(">>>MSB124X:Edinburgh 0x4A_L:%x\n", u8ChipRevId);
                }
                #endif

            }while((u8RetryCnt>0)&&(status==FALSE));
        }
    }

    if(status==FALSE)
    {
        printf("msb124x power_on_init FAIL !!!!!! \n\n");
    }
    else
    {
        #ifdef MS_DEBUG
        printf("msb124x power_on_init OK !!!!!! \n\n");
        u32tmm_2 = MsOS_GetSystemTime();
        printf("[tmm]t2-t1 = %ld (%ld - %ld)\n",u32tmm_2-u32tmm_1,u32tmm_2,u32tmm_1);
        #endif
    }

    DMD_UNLOCK();
    return status;
}


////////////////EXT API//////////////////////////////
MS_BOOL MDrv_DMD_MSB124X_Init_EX(MS_S32 s32Handle, sDMD_MSB124X_InitData *pDMD_MSB124X_InitData, MS_U32 u32InitDataLen)
{
    MS_BOOL bRet;
    DMD_LOCK();

    if(MDrv_DMD_MSB124X_SwitchHandle(s32Handle) == FALSE)
    {
        DMD_UNLOCK();
        return FALSE;
    }
    bRet = MDrv_DMD_MSB124X_Init(pDMD_MSB124X_InitData, u32InitDataLen);
    DMD_UNLOCK();
    return bRet;
}
MS_BOOL MDrv_DMD_MSB124X_Exit_EX(MS_S32 s32Handle)
{
    MS_BOOL bRet;
    DMD_LOCK();
    if(MDrv_DMD_MSB124X_SwitchHandle(s32Handle) == FALSE)
    {
        DMD_UNLOCK();
        return FALSE;
    }
    bRet = MDrv_DMD_MSB124X_Exit();
    DMD_UNLOCK();
    return bRet;
}
//////////////////////////////////////////////////////
MS_BOOL MDrv_DMD_MSB124X_GetReg_EX(MS_S32 s32Handle, MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet;

    DMD_LOCK();

    if(MDrv_DMD_MSB124X_SwitchHandle(s32Handle) == FALSE)
    {
        DMD_UNLOCK();
        return FALSE;
    }
    bRet = MDrv_DMD_MSB124X_GetReg(u16Addr, pu8Data);
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_MSB124X_SetReg_EX(MS_S32 s32Handle, MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet;

    DMD_LOCK();

    if(MDrv_DMD_MSB124X_SwitchHandle(s32Handle) == FALSE)
    {
        DMD_UNLOCK();
        return FALSE;
    }
    bRet = MDrv_DMD_MSB124X_SetReg(u16Addr, u8Data);
    DMD_UNLOCK();
    return bRet;

}
MS_BOOL MDrv_DMD_MSB124X_SetRegs_EX(MS_S32 s32Handle,MS_U16 u16Addr, MS_U8* u8pData, MS_U16 data_size)
{
    MS_BOOL bRet;

    DMD_LOCK();
    if(MDrv_DMD_MSB124X_SwitchHandle(s32Handle) == FALSE)
    {
        DMD_UNLOCK();
        return FALSE;
    }
    bRet = MDrv_DMD_MSB124X_SetRegs(u16Addr, u8pData, data_size);
    DMD_UNLOCK();
    return bRet;

}
MS_BOOL MDrv_DMD_MSB124X_SetReg2Bytes_EX(MS_S32 s32Handle, MS_U16 u16Addr, MS_U16 u16Data)
{
    MS_BOOL bRet;

    DMD_LOCK();
    if(MDrv_DMD_MSB124X_SwitchHandle(s32Handle) == FALSE)
    {
        DMD_UNLOCK();
        return FALSE;
    }
    bRet = MDrv_DMD_MSB124X_SetReg2Bytes(u16Addr, u16Data);
    DMD_UNLOCK();
    return bRet;

}

MS_BOOL MDrv_DMD_MSB124X_GetDSPReg_EX(MS_S32 s32Handle, MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet;

    DMD_LOCK();
    if(MDrv_DMD_MSB124X_SwitchHandle(s32Handle) == FALSE)
    {
        DMD_UNLOCK();
        return FALSE;
    }
    bRet = MDrv_DMD_MSB124X_GetDSPReg(u16Addr, pu8Data);
    DMD_UNLOCK();
    return bRet;

}

MS_BOOL MDrv_DMD_MSB124X_SetDSPReg_EX(MS_S32 s32Handle, MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet;

    DMD_LOCK();
    if(MDrv_DMD_MSB124X_SwitchHandle(s32Handle) == FALSE)
    {
        DMD_UNLOCK();
        return FALSE;
    }
    bRet = MDrv_DMD_MSB124X_SetDSPReg(u16Addr, u8Data);
    DMD_UNLOCK();
    return bRet;

}
void MDrv_DMD_MSB124X_SetCurrentDemodulatorType_EX(MS_S32 s32Handle, eDMD_MSB124X_DemodulatorType eCurrentDemodulatorType)
{

    DMD_LOCK();
    if(MDrv_DMD_MSB124X_SwitchHandle(s32Handle) == FALSE)
    {
        DMD_UNLOCK();
        return ;
    }
    MDrv_DMD_MSB124X_SetCurrentDemodulatorType(eCurrentDemodulatorType);
    DMD_UNLOCK();
    return ;
}
MS_BOOL MDrv_DMD_MSB124X_LoadDSPCode_EX(MS_S32 s32Handle)
{
    MS_BOOL bRet;

    DMD_LOCK();
    if(MDrv_DMD_MSB124X_SwitchHandle(s32Handle) == FALSE)
    {
        DMD_UNLOCK();
        return FALSE;
    }
    bRet = MDrv_DMD_MSB124X_LoadDSPCode();
    DMD_UNLOCK();
    return bRet;

}
MS_BOOL MDrv_DMD_MSB124X_DTV_DVBT_DSPReg_CRC_EX(MS_S32 s32Handle)
{
    MS_BOOL bRet;

    DMD_LOCK();
    if(MDrv_DMD_MSB124X_SwitchHandle(s32Handle) == FALSE)
    {
        DMD_UNLOCK();
        return FALSE;
    }
    bRet = MDrv_DMD_MSB124X_DTV_DVBT_DSPReg_CRC();
    DMD_UNLOCK();
    return bRet;

}
extern MS_BOOL MDrv_DMD_MSB124X_Power_On_Initialization_EX(MS_S32 s32Handle)
{
    MS_BOOL bRet;

    DMD_LOCK();
    if(MDrv_DMD_MSB124X_SwitchHandle(s32Handle) == FALSE)
    {
        DMD_UNLOCK();
        return FALSE;
    }
    bRet = MDrv_DMD_MSB124X_Power_On_Initialization();
    DMD_UNLOCK();
    return bRet;

}
////////////////////////////////////////////////////////////////////////////////
/// load dsp code
////////////////////////////////////////////////////////////////////////////////
extern MS_BOOL MDrv_DMD_MSB1245_LoadDSPCodeToSram_EX(MS_S32 s32Handle)
{
    MS_BOOL bRet;

    DMD_LOCK();
    if(MDrv_DMD_MSB124X_SwitchHandle(s32Handle) == FALSE)
    {
        DMD_UNLOCK();
        return FALSE;
    }
    bRet = MDrv_DMD_MSB1245_LoadDSPCodeToSram();
    DMD_UNLOCK();
    return bRet;

}




