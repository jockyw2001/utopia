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
#include "drvDMD_EXTERN_MSB123xc.h"
#include "include/drvDMD_common.h"
#include "include/drvSAR.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define ERR_DOMOD_MSB(x)     x
#define DBG_DOMOD_MSB(x)      x
#define DBG_DOMOD_FLOW(x)     x
#define DBG_GET_SIGNAL(x)      // x
#define DBG_DEMOD_LOAD_I2C(x)       x
#define DBG_DEMOD_CHECKSUM(x)        // x
#define DBG_FLASH_WP(x)        // x
#endif

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define LOAD_CODE_I2C_BLOCK_NUM          0x80
#define REG_MB_CNTL     0x0C00
#define REG_MB_ADDR_L   0x0C02
#define REG_MB_ADDR_H   0x0C03
#define REG_MB_DATA     0x0C04

#define resetDemodTime  50
#define waitFlashTime   50

#define SDRAM_DATA_CHECK                 0//1
#define SDRAM_BASE                       0x5000
#define SDRAM_WINDOW_SIZE       0x1000
#define SPI_DEVICE_BUFFER_SIZE           256
#define MAX_MSB123xc_LIB_LEN              131072

#define MSB123xc_BOOT  0x01
#define MSB123xc_DVBT2 0x02
#define MSB123xc_DVBT  0x04
#define MSB123xc_DVBC  0x08
#define MSB123xc_ALL   0x0F

#define EDINBURGH_BOOT_START_ADDR     0x00000
#define EDINBURGH_BOOT_END_ADDR       0x00FFF
#define EDINBURGH_DVBT2_P1_START_ADDR 0x01000
#define EDINBURGH_DVBT2_P1_END_ADDR   0x08FFF
#define EDINBURGH_DVBT2_P2_START_ADDR 0x09000
#define EDINBURGH_DVBT2_P2_END_ADDR   0x0FFFF
#define EDINBURGH_DVBT_START_ADDR     0x10000
#define EDINBURGH_DVBT_END_ADDR       0x17FFF
#define EDINBURGH_DVBC_START_ADDR     0x18000
#define EDINBURGH_DVBC_END_ADDR       0x1FFFF

#define EDINBURGH_WINDOWS_BASE                0x100
#define EDINBURGH_BOOT_WINDOWS_OFFSET         EDINBURGH_WINDOWS_BASE
#define EDINBURGH_DVBT2_P2_WINDOWS_OFFSET    (EDINBURGH_WINDOWS_BASE + 0x08)
#define EDINBURGH_DVBT2_P1_WINDOWS_OFFSET    (EDINBURGH_DVBT2_P2_WINDOWS_OFFSET + 0x08)
#define EDINBURGH_DVBT_WINDOWS_OFFSET        (EDINBURGH_DVBT2_P1_WINDOWS_OFFSET + 0x08)
#define EDINBURGH_DVBC_WINDOWS_OFFSET        (EDINBURGH_DVBT_WINDOWS_OFFSET + 0x08)

#define    MSB123xc_MAX_FLASH_ON_RETRY_NUM 3


/////////////// CONSTANT /////////////////
#define PAGE_WRITE_SIZE         256
#define VERSION_CODE_ADDR       0xFC0
#define VERSION_CODE_SIZE       32
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#define DMD_LOCK()      \
    do{                         \
        MS_ASSERT(MsOS_In_Interrupt() == FALSE); \
        if (eDMD_MSB123xc_DbgLevel == E_DMD_MSB123xc_DBGLV_DEBUG) printf("%s lock mutex\n", __FUNCTION__);\
        MsOS_ObtainMutex(_s32DMD_Mutex, MSOS_WAIT_FOREVER);\
        }while(0)

#define DMD_UNLOCK()      \
    do{                         \
        MsOS_ReleaseMutex(_s32DMD_Mutex);\
        if (eDMD_MSB123xc_DbgLevel == E_DMD_MSB123xc_DBGLV_DEBUG) printf("%s unlock mutex\n", __FUNCTION__); \
        }while(0)

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if 1
static MSIF_Version _drv_dmd_msb123xc_extern_version =
{
    .MW = { DMD_MSB123xc_EXTERN_VER, },
};
#else
static MSIF_Version _drv_dmd_msb123x_extern_version;
#endif
static MS_BOOL bDMD_MSB123xc_Power_init_en = FALSE;
static MS_U8 u8DMD_MSB123xc_PowerOnInitialization_Flow = 0;
static MS_U8 u8DMD_MSB123xc_Sdram_Code = 0x0;
static MS_U8 u8DMD_MSB123xc_Sram_Code  = 0x0;
static sDMD_MSB123xc_InitData _sDMD_MSB123xc_InitData;
static eDMD_MSB123xc_DbgLv eDMD_MSB123xc_DbgLevel=E_DMD_MSB123xc_DBGLV_NONE;
static eDMD_MSB123xc_DemodulatorType  eDMD_MSB123xc_CurrentDemodulatorType=E_DMD_MSB123xc_DEMOD_DVBT;
static MS_S32 _s32DMD_Mutex=-1;
static MS_BOOL bDemodRest = TRUE;

MS_U8 MSB123xc_LIB[]=
{
#include "msb123xc_dvbt.dat"
};
//--------------------------------------------------------------------- for DVB-T
//operation
#define RFAGC_EN                0x00
#define HUMDET_EN               0x00
#define DCR_EN                  0x01
#define IIS_EN                  0x01
#define CCI_EN                  0x01
#define ACI_EN                  0x01
#define IQB_EN                  0x00
#define AUTO_IQ_SWAP            0x01
#define AUTO_RFMAX              0x00
#define AUTO_ACI                0x00
#define MODE_CP_FORCED          0x00
#define TPS_FORCED              0x00
#define AUTO_SCAN               0x00
#define RSV_0D                  0x00
#define RSV_0E                  0x00
#define RSV_0F                  0x00

//configure
#define RSSI                    0x00
#define ZIF                     0x00
#define FREQ                    0x00
#define FC_L                    0x88
#define FC_H                    0x13
#define FS_L                    0x80
#define FS_H                    0x70
#define BW                      0x03
#define MODE                    0x01
#define CP                      0x03
#define LP_SEL                  0x00
#define CSTL                    0x00
#define HIER                    0x00
#define HPCR                    0x00
#define LPCR                    0x00
#define IQ_SWAP                 0x00
#define RFMAX                   0x01
#define ATV_SYSTEM              0x01
#define ICFO_RANGE              0x01
#define RFAGC_REF               0x64
#define IFAGC_REF_2K            0x4B
#define IFAGC_REF_8K            0x4B
#define IFAGC_REF_ACI           0x4B
#define IFAGC_REF_IIS_2K        0xA0
#define IFAGC_REF_IIS_8K        0xA0
#define IFAGC_ACI_DET_TH_L      0x9A
#define IFAGC_ACI_DET_TH_H      0x01
#define SERIAL_TS               0x00
#define TS_CLK_SEL              0x06
#define TS_OUT_INV              0x01
#define TS_DATA_SWAP            0x00
#define SFO_2K_H                0x00
#define SFO_2K_L                0xC8
#define SFO_8K_H                0x00
#define SFO_8K_L                0xC8
#define CHECK_CHANNEL           0x00
#define SNR_POS                 0x00
#define CCI_KP                  0x00
#define CCI_FSWEEP              0x00
#define TS_CLK_RATE_AUTO        0x00
#define DVBT_IF_INV_PWM_OUT_EN  0x00
#define DVBT_CRC                0x00


MS_U8 MSB123xc_DVBT_DSPREG_TABLE[] = // andy 2009-12-15  3:55:03 TW model
{
    RFAGC_EN,     HUMDET_EN,    DCR_EN,     IIS_EN,         CCI_EN,      ACI_EN,
    IQB_EN,       AUTO_IQ_SWAP, AUTO_RFMAX, AUTO_ACI,       MODE_CP_FORCED, TPS_FORCED,
    AUTO_SCAN,    RSV_0D,       RSV_0E,     RSV_0F,
    RSSI,         ZIF,          FREQ,       FC_L,           FC_H,        FS_L,      FS_H,
    BW,           MODE,         CP,         LP_SEL,         CSTL,        HIER,      HPCR,
    LPCR,         IQ_SWAP,      RFMAX,      ATV_SYSTEM,     ICFO_RANGE,  RFAGC_REF,
    IFAGC_REF_2K, IFAGC_REF_8K, IFAGC_REF_ACI,  IFAGC_REF_IIS_2K,  IFAGC_REF_IIS_8K, IFAGC_ACI_DET_TH_L,
    IFAGC_ACI_DET_TH_H,         SERIAL_TS,  TS_CLK_SEL,  TS_OUT_INV,  TS_DATA_SWAP,
    SFO_2K_H,     SFO_2K_L,     SFO_8K_H,   SFO_8K_L,       CHECK_CHANNEL,          SNR_POS,
    CCI_KP,       CCI_FSWEEP    , TS_CLK_RATE_AUTO, DVBT_IF_INV_PWM_OUT_EN, DVBT_CRC
};

//--------------------------------------------------------------------- for DVB-C
#define SR0_H_AUTO              0x0F
#define SR0_L_AUTO              0xA0
#define SR0_H                   0x1A
#define SR0_L                   0xDB
/*
#define REAGC_EN                0x00 //0x20
#define HUMDET_EN_C             0x00
#define DCR_EN                  0x01
#define IQB_EN                  0x00
#define AUTO_IQ                 0x01
#define AUTO_RFMAX              0x00
#define AUTO_ACI                0x01
*/
#define AUTO_SCAN               0x00
#define AUTO_SCAN_SYM_RATE      0x00 //0x28
#define AUTO_SCAN_QAM           0x00
#define ATV_DET_EN              0x01 //0x2A
/*
#define RSV_0B                  0x00
#define RSV_0C                  0x00
#define RSV_0D                  0x00
#define RSV_0E                  0x00
#define RSV_0F                  0x00
*/
#define RSSI                    0x00 //0x30
//#define ZIF                     0x00
#define FREQ                    0x00
//#define FC_L                    0xE0
//#define FC_H                    0x2E
//#define FS_L                    0x80
//#define FS_H                    0xBB
#define BW_L                    0xDB
#define BW_H                    0x1A
#define BW1_L                   0xF4
#define BW1_H                   0x1A
#define BW2_L                   0xDB
#define BW2_H                   0x1A
#define BW3_L                   0xDB
#define BW3_H                   0x1A
//#define RSV_0x1F              0x00
//#define RFMAX                 0x00 //0x40
#define QAM                     0x02
#define IQ_SWAP_C               0x01
#define CCI                     0x00
#define SERIAL_TS               0x00 //0: parallel 1:serial
//#define TS_SERIAL_CLK           0x05
#define TS_OUT_INV              0x01 //
#define TS_DATA_SWAP            0x00

MS_U8 MSB123xc_DVBC_DSPREG_TABLE[] =
{
    00,     00,     01,     00,     01,     00,     01,       AUTO_SCAN,
    AUTO_SCAN_SYM_RATE, AUTO_SCAN_QAM,  ATV_DET_EN,     00,     00,     00,     00,     00,
    00,     00,     00,     FC_L,           FC_H,           FS_L,           FS_H,
    BW_L,       BW_H,       BW1_L,      BW1_H,      BW2_L,      BW2_H,      BW3_L,      BW3_H,      00,
    00,     QAM,        IQ_SWAP_C,      CCI,        SERIAL_TS,      05,
    TS_OUT_INV,     TS_DATA_SWAP
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

MS_U8 MSB123xc_DVBT2_DSPREG_TABLE[] =
{
    T2_BW_VAL, T2_FC_L_VAL, T2_FC_H_VAL,  T2_TS_SERIAL_VAL, T2_TS_CLK_RATE_VAL,
    T2_TS_OUT_INV_VAL, T2_TS_DATA_SWAP_VAL, T2_TS_ERR_POL_VAL
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


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMD_MSB123xc_Init(sDMD_MSB123xc_InitData *pDMD_MSB123xc_InitData, MS_U32 u32InitDataLen)
{
    //char pDMD_MSB123xc_MutexString[16], pDMD_MSB123xc_MutexString_RegRW[16], pDMD_MSB123xc_MutexString_DSPRegRW[16];
    char pDMD_MSB123xc_MutexString[16];

    if (_s32DMD_Mutex != -1)
    {
        DMD_DBG(printf("MDrv_DMD_MSB123xc_Init more than once\n"));
        return FALSE;
    }

    if (NULL == strncpy(pDMD_MSB123xc_MutexString,"Mutex DMD DVB",16))
    {
        DMD_DBG(printf("MDrv_DMD_MSB123xc_Init strcpy Fail\n"));
        return FALSE;
    }
    _s32DMD_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, pDMD_MSB123xc_MutexString, MSOS_PROCESS_SHARED);
    if (_s32DMD_Mutex == -1)
    {
        DMD_DBG(printf("MDrv_DMD_MSB123xc_Init Create Mutex Fail\n"));
        return FALSE;
    }

    DMD_LOCK();
    if ( sizeof(_sDMD_MSB123xc_InitData) == u32InitDataLen)
    {
        memcpy(&_sDMD_MSB123xc_InitData, pDMD_MSB123xc_InitData, u32InitDataLen);
    }
    else
    {
        DMD_DBG(printf("MDrv_DMD_MSB123xc_Init input data structure incorrect\n"));
        DMD_UNLOCK();
        return FALSE;
    }

    if (_sDMD_MSB123xc_InitData.pDVBC_DSP_REG!= NULL)
    {
        printf("MDrv_DMD_MSB123xc_Init Init DVBC DSP Table By Device Driver ... \n");

        memcpy (&MSB123xc_DVBC_DSPREG_TABLE[0],_sDMD_MSB123xc_InitData.pDVBC_DSP_REG ,sizeof(MSB123xc_DVBC_DSPREG_TABLE));
    }
    else
    {
        printf("MDrv_DMD_MSB123xc_Init Init DVBC DSP Table By Device Driver ... \n");
    }

    if (_sDMD_MSB123xc_InitData.pDVBT_DSP_REG!= NULL)
    {
        printf("MDrv_DMD_MSB123xc_Init Init DVBT DSP Table By Device Driver ... \n");

        memcpy (&MSB123xc_DVBT_DSPREG_TABLE[0],_sDMD_MSB123xc_InitData.pDVBT_DSP_REG ,sizeof(MSB123xc_DVBT_DSPREG_TABLE));
    }
    else
    {
        printf("MDrv_DMD_MSB123xc_Init Init DVBT DSP Table By Device Driver ... \n");
    }

    if (_sDMD_MSB123xc_InitData.pDVBT2_DSP_REG!= NULL)
    {
        printf("MDrv_DMD_MSB123xc_Init Init DVBT2 DSP Table By Device Driver ... \n");

        memcpy (&MSB123xc_DVBT2_DSPREG_TABLE[0],_sDMD_MSB123xc_InitData.pDVBT2_DSP_REG ,sizeof(MSB123xc_DVBT2_DSPREG_TABLE));
    }
    else
    {
        printf("MDrv_DMD_MSB123xc_Init Init DVBT2 DSP Table By Device Driver ... \n");
    }

    if (_sDMD_MSB123xc_InitData.bEnableSPILoadCode && _sDMD_MSB123xc_InitData.fpMSB123xc_SPIPAD_En != NULL)
    {
        if (!MDrv_DMD_SSPI_Init(0))
        {
            printf("MDrv_DMD_MSB123xc_Init Init MDrv_DMD_SSPI_Init Fail \n");
        }
    }
    else
    {
        _sDMD_MSB123xc_InitData.bEnableSPILoadCode = FALSE;
    }

    //eDMD_MSB123xc_DbgLevel = E_DMD_MSB123xc_DBGLV_DEBUG;
#ifdef MS_DEBUG
    if (eDMD_MSB123xc_DbgLevel >= E_DMD_MSB123xc_DBGLV_INFO)
    {
        printf("MDrv_DMD_MSB123xc_Init\n");
    }
#endif

    DMD_UNLOCK();
    return TRUE;
}

MS_BOOL MDrv_DMD_MSB123xc_Exit(void)
{
#ifdef MS_DEBUG
    if (eDMD_MSB123xc_DbgLevel >= E_DMD_MSB123xc_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_MSB123xc_Exit \n");
    }
#endif

    MsOS_DeleteMutex(_s32DMD_Mutex);
    //MsOS_DeleteMutex(_s32DMD_Mutex_Reg_RW);
    //MsOS_DeleteMutex(_s32DMD_Mutex_DSPReg_RW);
    _s32DMD_Mutex= -1;
    //_s32DMD_Mutex_Reg_RW= -1;
    //_s32DMD_Mutex_DSPReg_RW= -1;

    bDMD_MSB123xc_Power_init_en = FALSE;
    u8DMD_MSB123xc_PowerOnInitialization_Flow = 0;
    u8DMD_MSB123xc_Sdram_Code = 0x0;
    u8DMD_MSB123xc_Sram_Code  = 0x0;
    bDemodRest = TRUE;

    return TRUE;
}

MS_BOOL MDrv_DMD_MSB123xc_SetDbgLevel(eDMD_MSB123xc_DbgLv u8DbgLevel)
{
    DMD_LOCK();
    eDMD_MSB123xc_DbgLevel = u8DbgLevel;
    DMD_UNLOCK();
    return TRUE;
}


MS_BOOL MDrv_DMD_MSB123xc_GetLibVer(const MSIF_Version **ppVersion)
{
    DMD_LOCK();
    if (!ppVersion)
    {
        DMD_UNLOCK();
        return FALSE;
    }

    *ppVersion = &_drv_dmd_msb123xc_extern_version;
    DMD_UNLOCK();
    return TRUE;
}

MS_BOOL MDrv_DMD_MSB123xc_GetFWVer(MS_U16 *ver)
{

    MS_BOOL bRet=TRUE;

    DMD_LOCK();
    //printf("MDrv_DMD_DVBT_GetFWVer %x\n",*ver);
    DMD_UNLOCK();

    return bRet;

}

static MS_BOOL _MDrv_DMD_MSB123xc_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6];

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr & 0xff;

    u8MsbData[0] = 0x35;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8MsbData);
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_READ_BYTES, 0, NULL, 1, pu8Data);

    u8MsbData[0] = 0x34;
    bRet=(*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

#ifdef MS_DEBUG
    if (eDMD_MSB123xc_DbgLevel >= E_DMD_MSB123xc_DBGLV_DEBUG)
    {
        printf("_MDrv_DMD_MSB123xc_GetReg %x %x\n", u16Addr, *pu8Data);
    }
#endif

    return bRet;
}

MS_BOOL MDrv_DMD_MSB123xc_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet=TRUE;

    DMD_LOCK();
    bRet = _MDrv_DMD_MSB123xc_GetReg(u16Addr, pu8Data);
    DMD_UNLOCK();

    return bRet;
}

static MS_BOOL _MDrv_DMD_MSB123xc_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6];

#ifdef MS_DEBUG
    if (eDMD_MSB123xc_DbgLevel >= E_DMD_MSB123xc_DBGLV_DEBUG)
    {
        printf("_MDrv_DMD_MSB123xc_SetReg %x %x\n", u16Addr, u8Data);
    }
#endif

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;
    u8MsbData[5] = u8Data;

    u8MsbData[0] = 0x35;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 6, u8MsbData);

    u8MsbData[0] = 0x34;
    bRet=(*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);
    return bRet;
}

MS_BOOL MDrv_DMD_MSB123xc_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;

    DMD_LOCK();
    bRet = _MDrv_DMD_MSB123xc_SetReg(u16Addr, u8Data);
    DMD_UNLOCK();

    return bRet;
}


static MS_BOOL _MDrv_DMD_MSB123xc_SetRegs(MS_U16 u16Addr, MS_U8* u8pData, MS_U16 data_size)
{
    MS_BOOL bRet=TRUE;
    MS_U8   u8MsbDataValue[LOAD_CODE_I2C_BLOCK_NUM + 5];
    MS_U16   idx = 0;

#ifdef MS_DEBUG
    if (eDMD_MSB123xc_DbgLevel >= E_DMD_MSB123xc_DBGLV_DEBUG)
    {
        printf("_MDrv_DMD_MSB123xc_SetRegs %x %x\n", u16Addr, data_size);
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
    bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbDataValue);

    u8MsbDataValue[0] = 0x10;
    bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5 + data_size, u8MsbDataValue);

    u8MsbDataValue[0] = 0x34;
    bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbDataValue);

    return bRet;
}

MS_BOOL MDrv_DMD_MSB123xc_SetRegs(MS_U16 u16Addr, MS_U8* u8pData, MS_U16 data_size)
{
    MS_BOOL bRet=TRUE;

    DMD_LOCK();
    bRet = _MDrv_DMD_MSB123xc_SetRegs(u16Addr, u8pData, data_size);
    DMD_UNLOCK();

    return bRet;
}

static MS_BOOL _MDrv_DMD_MSB123xc_SetReg2Bytes(MS_U16 u16Addr, MS_U16 u16Data)
{
    MS_BOOL bRet=TRUE;

#ifdef MS_DEBUG
    if (eDMD_MSB123xc_DbgLevel >= E_DMD_MSB123xc_DBGLV_DEBUG)
    {
        printf("_MDrv_DMD_MSB123xc_SetReg2Bytes %x %x\n", u16Addr, u16Data);
    }
#endif

    bRet &= _MDrv_DMD_MSB123xc_SetReg(u16Addr, (MS_U8)u16Data&0x00ff);
    bRet &= _MDrv_DMD_MSB123xc_SetReg(u16Addr + 0x0001, (MS_U8)(u16Data>>8)&0x00ff);

    return bRet;
}

MS_BOOL MDrv_DMD_MSB123xc_SetReg2Bytes(MS_U16 u16Addr, MS_U16 u16Data)
{
    MS_BOOL bRet=TRUE;

    DMD_LOCK();
    bRet = _MDrv_DMD_MSB123xc_SetReg2Bytes(u16Addr, u16Data);
    DMD_UNLOCK();

    return bRet;
}


MS_BOOL _MDrv_DMD_MSB123xc_GetDSPReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8     u8Cntl = 0x00;
    MS_U16    u16Cntr = 0x00;

    bRet &= _MDrv_DMD_MSB123xc_SetReg(REG_MB_ADDR_H, (MS_U8)(u16Addr >> 8));
    bRet &= _MDrv_DMD_MSB123xc_SetReg(REG_MB_ADDR_L, (MS_U8)(u16Addr));
    bRet &= _MDrv_DMD_MSB123xc_SetReg(REG_MB_CNTL, 0x03);

    do
    {
        bRet &= _MDrv_DMD_MSB123xc_GetReg(REG_MB_CNTL, &u8Cntl);
        if (u16Cntr++ > 0x7ff)
        {
#ifdef MS_DEBUG
            if (eDMD_MSB123xc_DbgLevel >= E_DMD_MSB123xc_DBGLV_DEBUG)
            {
                printf("MSB123xc_MB_READ_FAILURE\n");
            }
#endif
            return FALSE;
        }
    }
    while(u8Cntl != 0xff);

    bRet &= _MDrv_DMD_MSB123xc_GetReg(REG_MB_DATA, pu8Data);

#ifdef MS_DEBUG
    if (eDMD_MSB123xc_DbgLevel >= E_DMD_MSB123xc_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_MSB123xc_GetDSPReg %x %x\n", u16Addr, *pu8Data);
    }
#endif

    return bRet;
}

MS_BOOL MDrv_DMD_MSB123xc_GetDSPReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet=TRUE;

    DMD_LOCK();
    bRet = _MDrv_DMD_MSB123xc_GetDSPReg(u16Addr, pu8Data);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL _MDrv_DMD_MSB123xc_SetDSPReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8     u8Cntl = 0x00;
    MS_U16    u16Cntr = 0x00;

#ifdef MS_DEBUG
    if (eDMD_MSB123xc_DbgLevel >= E_DMD_MSB123xc_DBGLV_DEBUG)
    {
        printf("MDrv_DMD_MSB123xc_SetDSPReg %x %x\n", u16Addr, u8Data);
    }
#endif

    bRet &= _MDrv_DMD_MSB123xc_SetReg(REG_MB_DATA, u8Data);
    bRet &= _MDrv_DMD_MSB123xc_SetReg(REG_MB_ADDR_H, (MS_U8)(u16Addr >> 8));
    bRet &= _MDrv_DMD_MSB123xc_SetReg(REG_MB_ADDR_L, (MS_U8)(u16Addr));
    bRet &= _MDrv_DMD_MSB123xc_SetReg(REG_MB_CNTL, 0x04);

    do
    {
        bRet &= _MDrv_DMD_MSB123xc_GetReg(REG_MB_CNTL, &u8Cntl);
        if (u16Cntr++ > 0x7ff)
        {
#ifdef MS_DEBUG
            if (eDMD_MSB123xc_DbgLevel >= E_DMD_MSB123xc_DBGLV_DEBUG)
            {
                printf("MSB123xc_MB_WRITE_FAILURE\n");
            }
#endif
            return false;
        }
    }
    while(u8Cntl != 0xff);
    return bRet;
}

MS_BOOL MDrv_DMD_MSB123xc_SetDSPReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;

    DMD_LOCK();
    bRet = _MDrv_DMD_MSB123xc_SetDSPReg(u16Addr, u8Data);
    DMD_UNLOCK();

    return bRet;
}

static MS_BOOL _MSB123xc_I2C_CH_Reset(MS_U8 ch_num)
{
    MS_BOOL bRet=TRUE;
    //MAPI_U8         addr[4] = {0x00, 0x00, 0x00, 0x00};
    MS_U8         u8data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};

#ifdef MS_DEBUG
    printf("[msb123xc][beg]I2C_CH_Reset, CH=0x%x\n",ch_num);
#endif
    //DMD_LOCK_REG_RW();

    // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
    //u8data[0] = 0x53;
    //bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8data);
    if (bDemodRest)
    {
        bDemodRest = FALSE;
        // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
        u8data[0] = 0x53;
        // Don't check Ack because this passward only ack one time for the first time.
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8data);
    }
    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_
    u8data[0] = 0x71;
    bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8data);

    // 8'hb2(SRID)->8,h81(CMD)  //TV.n_iic_sel_b0
    u8data[0] = ((ch_num & 0x01) != 0)? 0x81 : 0x80;
    bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8data);

    // 8'hb2(SRID)->8,h83(CMD)  //TV.n_iic_sel_b1
    u8data[0] = ((ch_num & 0x02) != 0)? 0x83 : 0x82;
    bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8data);

    // 8'hb2(SRID)->8,h84(CMD)  //TV.n_iic_sel_b2
    u8data[0] = ((ch_num & 0x04) != 0)? 0x85 : 0x84;
    bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8data);

    // 8'hb2(SRID)->8,h53(CMD)  //TV.n_iic_ad_byte_en2, 32bit read/write
    u8data[0] = 0x53;
    bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8data);

    // 8'hb2(SRID)->8,h7f(CMD)  //TV.n_iic_sel_use_cfg
    u8data[0] = 0x7f;
    bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8data);

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
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]I2C_CH_Reset, CH=0x%x\n",ch_num));
#endif
    return bRet;
}

static MS_BOOL _MSB123xc_HW_init(void)
{
    MS_BOOL bRet = true;
    MS_U8 u8_tmp = 0;
    MS_U8 u8_timeout = 0;

#ifdef MS_DEBUG
    printf("[msb123xc][beg]MSB123xc_HW_init\n");
#endif
    //DMD_LOCK();

    // ASIC INIT for EDB DVB-T2
    {
        // Reset MCU
        // wreg 11 0x19 0x0003  // don't do this in MCU firmware!
        // -------------------------------------------------------------------
        // Initialize DMD_ANA_MISC
        // -------------------------------------------------------------------
        // [1:0]	reg_mpll_loop_div_first       feedback divider 00:div by 1 01:div by 2 10:div by 4 11:div by 8
        // [15:8]	reg_mpll_loop_div_second      feedback divider, div by binary data number
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h33, 2'b11, 16'h2400);  // Loop divider ; VCO = 24*(2^2)*9 = 864
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0A00+(0x33)*2, 0x2400);

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
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0A00+(0x35)*2, 0x1804);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0A00+(0x35)*2, 0x0004);

        // [15:0]	reg_mpll_test
        // [14]	mpll reset
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0A00+(0x34)*2, 0x4000);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0A00+(0x34)*2, 0x0000);

        // [0]	reg_mpll_clk_dp_pd	dummy
        // [1]	reg_adc_clk_pd		ADC output clock power down
        // [2]	reg_mpll_div2_pd	MPLL_DIV2 power down
        // [3]	reg_mpll_div3_pd	MPLL_DIV3 power down
        // [4]	reg_mpll_div4_pd	MPLL_DIV4 power down
        // [5]	reg_mpll_div8_pd	MPLL_DIV8 power down
        // [6]	reg_mpll_div10_pd	MPLL_DIV10 power down
        // [13:8]  reg_mpll_adc_div_sel	select the ADC clock divide ratio,ADC clk=XTAL_IN * (LOOP_DIV_FIRST*LOOP_DIV_SECOND)/div_ratio
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h30, 2'b11, 16'h1e00);  // divide ADC clock to 28.8Mhz = 24*36/30
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0A00+(0x30)*2, 0x1e00);


        //     --------------------------------------
        //     Initialize ADC I/Q");
        //     --------------------------------------
        // Set IMUXS QMUXS
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
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0A00+(0x01)*2, 0x0440);

        // Set enable ADC clock
        // [0]	Channel I ADC power down: 1=power dwon
        // [1]	Channel Q ADC power down: 1=power dwon
        // [2]	power down clamp buffer for test mode
        // [3]	change ADC reference voltage for SSIF
        // [6:4]    ADC source bias current control
        // [9:8]    XTAL receiver amp gain
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0A00+(0x0c)*2, 0x0000);

        // Disable PWDN_REF
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
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0A00+(0x20)*2, 0x0000);

        // Set ADC gain is 1
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0A00+(0x0b)*2, 0x0009);

        // Disable ADC Sign bit
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0A00+(0x2e)*2, 0x0000);

        // ADC I channel offset
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0A00+(0x2a)*2, 0x0c00);

        // ADC Q channel offset
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0A00+(0x2b)*2, 0x0c00);

        // [2:0]reg_acl_ref
        // [5:4]reg_acl_isel
        // [8]	reg_xtal_pm_isel
        // [9]	reg_bond_mode
        // [10]	reg_clk_bond_mode
        // [11]	reg_clk_usb_3p3_en
        // [13:12]	reg_iq_ctrl	= 2'd1
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0A00+(0x19)*2, 0x1e00);

        // [ 4:0]reg_ckg_bist[4:0]
        // [11:8]reg_ckg_adcd_d2[3:0]
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x1c)*2, 0x0000);

        // [ 4:0]reg_ckg_dvbtm_sram_t11x_t22x[4:0]
        // [12:8]reg_ckg_dvbtm_sram_t11x_t24x[4:0]
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x1e)*2, 0x0000);

        // [15:0]	reg_mpll_test
        // [4]	mpll lock detector enable
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0A00+(0x34)*2, 0x0010);

        // -------------------------------------------------------------------
        // Release clock gating
        // -------------------------------------------------------------------

        // [0]	reg_xtal_en
        // [9:8]	reg_clk_pd_iic
        // [10]	reg_clk_pd_all
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x09)*2, 0x0101);

        // [3:0]	reg_ckg_adcd
        // [7:4]	reg_ckg_sadc
        // [11:8]	reg_ckg_iicm
        // [13:12]	reg_ckg_sbus
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x0a)*2, 0x0000);

        // [5:0]	reg_ckg_mcu
        // [6]	reg_ckg_live
        // [11:8]	reg_ckg_inner
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x0b)*2, 0x0030);

        // @0x0910
        // [3:0]	reg_ckg_dvbtm_adc
        // [6:4]	reg_ckg_dvbt_inner1x
        // [10:8]	reg_ckg_dvbt_inner2x
        // [14:12]	reg_ckg_dvbt_inner4x
        //DVBT2
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x10)*2, 0x1110);

        // @0x0911
        // [2:0]	reg_ckg_dvbt_outer1x
        // [6:4]	reg_ckg_dvbt_outer2x
        // [11:8]	reg_ckg_dvbtc_outer2x
        //DVBT2
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x11)*2, 0x0111);

        // @0x0912
        // [3:0]	reg_ckg_dvbtm_ts
        // [4]	reg_dvbtm_ts_out_mode
        // [5]	reg_dvbtm_ts_clk_pol
        // [15:8]	reg_dvbtm_ts_clk_divnum
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x12)*2, 0x0618);

        // @0x0913
        // [5:0]	reg_ckg_spi
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x13)*2, 0x0020);

        // @0x0914
        // [12:8]	reg_ckg_dvbtm_sram_t1o2x_t22x
        //DVBT2
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x14)*2, 0x0000);
        // `endif

        // @0x0915
        // [3:0]	reg_ckg_dvbc_inner
        // [6:4]	reg_ckg_dvbc_eq
        // [10:8]	reg_ckg_dvbc_eq8x
        //DVBT2
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x15)*2, 0x0111);

        // @0x0916
        // [3:0]	reg_ckg_dvbtm_adc_2x_4x
        // [8:4]	reg_ckg_dvbtm_sram_adc_t22x
        //DVBT2
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x16)*2, 0x0001);
        // `endif

        // @0x0917
        // [4:0]	reg_ckg_dvbtm_sram_t12x_t22x
        // [12:8]	reg_ckg_dvbtm_sram_t12x_t24x
        //DVBT2
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x17)*2, 0x0000);

        // @0x0918
        // [4:0]	reg_ckg_dvbtm_sram_t14x_t24x
        // [12:8]	reg_ckg_dvbtm_ts_in
        //DVBT2
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x18)*2, 0x1000);
        // `endif

        // @0x0919
        // [2:0]	reg_ckg_tdp_jl_inner1x
        // [6:4]	reg_ckg_tdp_jl_inner4x
        // [15:8]	reg_ckg_miu
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x19)*2, 0x3c00);

        // @0x091a
        // [6:4]	reg_ckg_dvbt2_inner1x
        // [10:8]	reg_ckg_dvbt2_inner2x
        // [14:12]	reg_ckg_dvbt2_inner4x
        //DVBT2
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x1a)*2, 0x0000);

        // @0x091b
        // [2:0]	reg_ckg_dvbt2_outer1x
        // [6:4]	reg_ckg_dvbt2_outer2x
        // [10:8]	reg_ckg_syn_miu
        // [14:12]	reg_ckg_syn_ts
        //DVBT2
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x1b)*2, 0x0000);


        // @0x091c
        // [4:0]	reg_ckg_bist
        // [11:8]	reg_ckg_adcd_d2
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x1c)*2, 0x0000);


        // [1:0]	reg_iicm_pad_sel
        // [4]	reg_i2c_sbpm_en
        // [12:8]	reg_i2c_sbpm_idle_num
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x08)*2, 0x0a01);

        // [8]	reg_turn_off_pad
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x28)*2, 0x0000);

        // [15:0]	reg_synth_set[15: 0]
        // [ 7:0]	reg_synth_set[23:16]
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x51)*2, 0x0000);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x52)*2, 0x0020);


        // [0]	reg_synth_reset
        // [1]	reg_synth_ssc_en
        // [2]	reg_synth_ssc_mode
        // [4]	reg_synth_sld
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x50)*2, 0x0010);

        // [1:0]	reg_apll_loop_div_first
        // [15:8]	reg_apll_loop_div_second
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x57)*2, 0x0003);


        // [0]	reg_apll_pd
        // [1]	reg_apll_reset
        // [2]	reg_apll_porst
        // [3]	reg_apll_vco_offset
        // [4]	reg_apll_en_ts
        // [5]	reg_apll_endcc
        // [6]	reg_apll_clkin_sel
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x55)*2, 0x0006);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x55)*2, 0x0010);

        // [16:0]	reg_apll_test
        // [4]	lock detector enable
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x59)*2, 0x0010);

        // 0x0920
        // [3:0]	reg_ckg_ts_apll_div[2:0]
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x20)*2, 0x0004);

        // -------------------------------------------------------------------
        // initialize MIU
        // -------------------------------------------------------------------
        // $display("--------------------------------------");
        // $display("Initialize MIU ");
        // $display("--------------------------------------");

        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x0f)*2, 0x0000);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x0f)*2, 0x0000);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x0f)*2, 0x0000);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x0f)*2, 0x0c01);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x0f)*2, 0x0c00);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x11)*2, 0x0052);

        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x10)*2, 0x352b); // 0x29a378 = 166MHz, 0x352b52 = 130MHz
        //execute reset and porst
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x11)*2, 0x6052);
        //exit reset and porst
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x11)*2, 0x0052);

        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x12)*2, 0xc000);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x13)*2, 0x0000);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x16)*2, 0x0030);
        //bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x1c)*2, 0x0080);
        // 2012/12/06 add best MIU phase by jason-bf.huang
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x1c)*2, 0x00a0);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x1b)*2, 0x0067);


        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x01)*2, 0x8100);
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
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x01)*2, 0xe100);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x01)*2, 0x8100);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x02)*2, 0x0360);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x03)*2, 0x0030);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x04)*2, 0x33c9);
        // reg_tRAS                      : [3:0]        9
        // reg_tRC                       : [7:4]        c
        // reg_tRCD                      : [11:8]       3
        // reg_tRP                       : [15:12]      3
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x05)*2, 0x4232);
        // reg_tRRD                      : [3:0]         2
        // tWR                           : [7:4]         3
        // reg_tMRD                      : [11:8]        2
        // reg_tRTP			: [15:12]	4
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x06)*2, 0x5532);
        // reg_w2r_dly(tWTR)             : [3:0]         2
        // reg_w2r_oen_dly               : [7:4]         3
        // reg_r2w_dly(tRTW)             : [11:8]        5
        // reg_r2w_oen_dly               : [15:12]       5
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x07)*2, 0x400c);
        // tRFC                          : [5:0]         c
        // reg_tRAS[4]                   : [6]           0
        // reg_tRC[4]                    : [7]           0
        // reg_write_latency             : [10:8]        0
        // reg_tCCD                      : [15:14]       1
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x0e)*2, 0x1800);

        //mask other request
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x23)*2, 0x7ffe);

        //reg_rq0_round_robin		: [0]
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x20)*2, 0xc001);

        MsOS_DelayTaskUs(1000);

        //miu software reset
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x0f)*2, 0x0c01);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x0f)*2, 0x0c00);
        //miu software reset
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x0f)*2, 0x0c01);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x0f)*2, 0x0c00);

        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x01)*2, 0x010d);

        // --------Initial DRAM start here!!!-------
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x00)*2, 0x0001);

        bRet &= _MDrv_DMD_MSB123xc_GetReg(0x1201, &u8_tmp);
        u8_timeout = 0;
        while( (u8_tmp&0x80) != 0x80)
        {
            if(u8_timeout++>200)
            {
                printf("[msb1233c][err]MIU init failure...\n");
                return FALSE;
            }
            // 10us delay
            MsOS_DelayTaskUs(10);
            bRet &= _MDrv_DMD_MSB123xc_GetReg(0x1201, &u8_tmp);
        }
        // --------Initial Done-------

        //turn on report counter	: [0]
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x08)*2, 0x0001);

        // -- miu self test start --
        //base
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x71)*2, 0x0000);
        //length
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x72)*2, 0x0010);
        //test data
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x74)*2, 0x5aa5);
        //reg_test_en                   : [0]
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x70)*2, 0x0001);
        // reg_test_mode                 : [2:1] 00 : address,
        //                                      01 : test data
        //                                      10 : shift data
        // reg_inv_data                  : [3]
        // reg_test_loop                 : [4]
        // reg_force_out                 : [5]
        // reg_force_in                  : [6]
        // reg_read_only                 : [8]
        // reg_write_only                : [9]

        MsOS_DelayTaskUs(1000);

        //wait test_finish
        bRet &= _MDrv_DMD_MSB123xc_GetReg(0x12E1, &u8_tmp);
        u8_timeout = 0;
        while( (u8_tmp&0x80) != 0x80)
        {
            if(u8_timeout++>200)
            {
                printf("[msb1233c][err]MIU self test Phase1 failure...\n");
                return FALSE;
            }
            // 10us delay
            MsOS_DelayTaskUs(10);
            bRet &= _MDrv_DMD_MSB123xc_GetReg(0x12E1, &u8_tmp);
        }

        MsOS_DelayTaskUs(1000);

        // MIU self test FAIL let program stuck in this while loop
        bRet &= _MDrv_DMD_MSB123xc_GetReg(0x12E1, &u8_tmp);
        u8_timeout = 0;
        while( (u8_tmp&0x40) != 0x00)
        {
            if(u8_timeout++>200)
            {
                printf("[msb1233c][err]MIU self test Phase2 failure...\n");
                return FALSE;
            }
            // 10us delay
            MsOS_DelayTaskUs(10);
            bRet &= _MDrv_DMD_MSB123xc_GetReg(0x12E1, &u8_tmp);
        }

        //reg_rq0_mask
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1200+(0x23)*2, 0x0000);


        // -------------------------------------------------------------------
        // initialize MIU  finish
        // -------------------------------------------------------------------

        // -------------------------------------------------------------------
        //  Turn on pads
        // -------------------------------------------------------------------

        // ------Turn off all pad in
        // [0] reg_set_pad_low
        // [1] reg_set_pad_high
        // [2] reg_set_i2cs_pad_low
        // [3] reg_set_i2cs_pad_high
        // [8] reg_turn_off_pad
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x28)*2, 0x0000);

        // ------I2CM pad on
        // [1:0]    reg_iicm_pad_sel[1:0]	1:iicm enable 2:UART enable
        // [4]	    reg_i2c_sbpm_en		1: enable I2CS bypass to I2CM function
        // [12:8]   reg_i2c_sbpm_idle_num[4:0]	a: default
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x08)*2, 0x0a01);

        // ------Transport Stream pad on (except TS ERR pad)
        // [15:0]   reg_en_ts_pad[15:0]	0x00ff:normal TS location 0xff00:reverse TS location
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x2d)*2, 0x00ff);

        // ------Transport Stream pad on (TS ERR pad)
        // [0]	    reg_en_ts_err_pad	1: enable
        // [4]	    reg_ts_err_pol	1: inverse 0:normal
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900+(0x2e)*2, 0x0001);

        // ------AGC pad on
        // [4]    reg_ifagc_enable	Tuner IF AGC enable. 1:enable
        // [5]    reg_ifagc_odmode	1: inverse IF AGC
        // [6]    reg_ifagc_data_sel	IFAGC data select 0: IF AGC 1: RF AGC
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0a00+(0x18)*2, 0x0010);
    }

    //DMD_UNLOCK();
#ifdef MS_DEBUG
    printf("[msb123xc][end]MSB123xc_HW_init, bRet=0x%x\n",bRet);
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

#ifdef MS_DEBUG
    DBG_DOMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]Load2Sdram, len=0x%x, win_offset=0x%x\n",data_length,sdram_win_offset_base));
    u32tmm_3 = MsOS_GetSystemTime();
#endif

    if (_sDMD_MSB123xc_InitData.bEnableSPILoadCode)
    {
        MS_U32 u32Addr = 0;

        u32Addr = (MS_U32)sdram_win_offset_base * 0x1000; // unit: 4K
        bRet &= MDrv_DMD_SSPI_MIU_Writes(u32Addr, u8_ptr, data_length);
#ifdef MS_DEBUG
        DBG_DEMOD_LOAD_I2C(printf("[msb123xc]u32Addr=%08lx\n",u32Addr));
#endif

#if (SDRAM_DATA_CHECK == 1)
#ifdef MS_DEBUG
        DBG_DEMOD_LOAD_I2C(printf("[msb123xc]SDRAM data check...\n"));
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
                    printf("[msb123xc]error, u32Addr=0x%08lx, y=0x%x, x=0x%x\n",u32Addr+j, buf[j] , u8_ptr[j]);
                }
            }
        }
#ifdef MS_DEBUG
        DBG_DEMOD_LOAD_I2C(printf("[msb123xc]SDRAM data check...Done\n"));
#endif
#endif
    }
    else
    {
        if ( _MSB123xc_I2C_CH_Reset(0) == FALSE)
        {
            printf(">>>MSB123xc CH0 Reset:Fail\n");
            return FALSE;
        }
        else
        {

            // set xData map upper and low bound for 64k DRAM window
            bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x2B00+(0x63)*2, 0x3F24);

            // set xData map offset for 64k DRAM window, 64kbytes alignment
            bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x2B00+(0x64)*2, 0x0000);

            // set xData map upper and low bound for 4k DRAM window
            bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x2B00+(0x65)*2, 0x2014);

            // set xData map offset for 4k DRAM window, 4kbytes alignment
            bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x2B00+(0x66)*2, sdram_win_offset++);

            // enable xData map for DRAM
            bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x2B00+(0x62)*2, 0x0007);

            for(x_data_offset = 0, y_cir_addr = SDRAM_BASE; x_data_offset < data_length;)
            {
                if (y_cir_addr == (SDRAM_BASE+SDRAM_WINDOW_SIZE))
                {
                    //set xData map offset for 4k DRAM window, 4kbytes alignment
                    // 0x1000, 4096 bytes
                    bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x2B00+(0x66)*2, sdram_win_offset++);
                    y_cir_addr = SDRAM_BASE;
                }

                // max 0x200, error above.....

                if((data_length - x_data_offset) >= LOAD_CODE_I2C_BLOCK_NUM)
                {
                    bRet &= _MDrv_DMD_MSB123xc_SetRegs(y_cir_addr, (u8_ptr + x_data_offset),LOAD_CODE_I2C_BLOCK_NUM);
                    y_cir_addr += LOAD_CODE_I2C_BLOCK_NUM;
                    x_data_offset += LOAD_CODE_I2C_BLOCK_NUM;
                }
                else
                {
                    bRet &= _MDrv_DMD_MSB123xc_SetRegs(y_cir_addr, (u8_ptr + x_data_offset),data_length - x_data_offset);
                    y_cir_addr += (data_length - x_data_offset);
                    x_data_offset += (data_length - x_data_offset);
                }
            }

#ifdef MS_DEBUG
            DBG_DEMOD_LOAD_I2C(printf("[msb123xc]x_data_offset=%d,y_cir_addr=%d,z_block_num=%d\n",x_data_offset,y_cir_addr,sdram_win_offset));
#endif
#if (SDRAM_DATA_CHECK == 1)
            // beg data check.
#ifdef MS_DEBUG
            DBG_DEMOD_LOAD_I2C(printf("[msb123xc]SDRAM data check...\n"));
#endif

            sdram_win_offset = sdram_win_offset_base;

            // set xData map offset for 4k DRAM window, 4kbytes alignment
            bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x2B00+(0x66)*2, sdram_win_offset++);

            for(x_data_offset = 0, y_cir_addr = SDRAM_BASE; x_data_offset < data_length;)
            {
                MS_U8 u8_tmp;
                if (y_cir_addr == (SDRAM_BASE+SDRAM_WINDOW_SIZE))
                {
                    //set xData map offset for 4k DRAM window, 4kbytes alignment
                    // 0x1000, 4096 bytes
                    bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x2B00+(0x66)*2, sdram_win_offset++);
                    y_cir_addr = SDRAM_BASE;
                }

                bRet &= _MDrv_DMD_MSB123xc_GetReg(y_cir_addr++, &u8_tmp);
                if(u8_tmp != *(u8_ptr + x_data_offset++))
                {
                    printf("[msb123xc]error, idx=0x%x, y=0x%x, x=0x%x\n",y_cir_addr-1, u8_tmp, *(u8_ptr + x_data_offset-1));
                }
            }

#ifdef MS_DEBUG
            DBG_DEMOD_LOAD_I2C(printf("[msb123xc]SDRAM data check...Done\n"));
#endif

            // end data check
#endif
            //  Release xData map for SDRAM
            bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x2B00+(0x62)*2, 0x0000);

            // Channel changed from CH 0x00 to CH 0x03
            if (_MSB123xc_I2C_CH_Reset(3) == FALSE)
            {
#ifdef MS_DEBUG
                ERR_DOMOD_MSB(printf(">>>MSB123xc CH3 Reset:Fail\n"));
#endif
                return FALSE;
            }
        }
    }
#ifdef MS_DEBUG
    u32tmm_4 = MsOS_GetSystemTime();
    printf("[tmm2]t4-t3 = %ld (%ld - %ld)\n",u32tmm_4-u32tmm_3,u32tmm_4,u32tmm_3);
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]Load2Sdram, len=0x%x, win_offset=0x%x\n",data_length,sdram_win_offset_base));
    if (!bRet) printf("%s %d Error\n",__func__, __LINE__);
#endif

    return bRet;
}

static MS_BOOL _LoadDspCodeToSDRAM_Boot(void)
{
    MS_BOOL bRet = true;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;

#ifdef MS_DEBUG
    DBG_DOMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]LoadDspCodeToSDRAM_Boot\n"));
#endif

    if(!(u8DMD_MSB123xc_Sdram_Code&MSB123xc_BOOT))
    {
        if(sizeof(MSB123xc_LIB) > EDINBURGH_BOOT_START_ADDR)
        {
            // boot code
            data_ptr = MSB123xc_LIB + EDINBURGH_BOOT_START_ADDR;
            code_size = EDINBURGH_BOOT_END_ADDR - EDINBURGH_BOOT_START_ADDR + 1;
            win_offset = EDINBURGH_BOOT_WINDOWS_OFFSET;
            bRet &= _Load2Sdram(data_ptr,code_size,win_offset);
            if(bRet == true)
            {
                u8DMD_MSB123xc_Sdram_Code |= MSB123xc_BOOT;
            }
        }
        else
        {
            printf("@msb123xc, boot code is unavailable!!!\n");
        }
    }
#ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]LoadDspCodeToSDRAM_Boot\n"));
#endif
    return bRet;
}

static MS_BOOL _LoadDspCodeToSDRAM_dvbt2(void)
{
    MS_BOOL bRet = true;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;

#ifdef MS_DEBUG
    DBG_DOMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]LoadDspCodeToSDRAM_dvbt2\n"));
#endif

    if( !(u8DMD_MSB123xc_Sdram_Code&MSB123xc_DVBT2) )
    {
        if(sizeof(MSB123xc_LIB) > EDINBURGH_DVBT2_P1_START_ADDR)
        {
            // dvbt2_p2
            data_ptr = MSB123xc_LIB + EDINBURGH_DVBT2_P2_START_ADDR;
            code_size = EDINBURGH_DVBT2_P2_END_ADDR - EDINBURGH_DVBT2_P2_START_ADDR + 1;
            win_offset = EDINBURGH_DVBT2_P2_WINDOWS_OFFSET;
            bRet &= _Load2Sdram(data_ptr,code_size,win_offset);

            // dvbt2_p1
            data_ptr = MSB123xc_LIB + EDINBURGH_DVBT2_P1_START_ADDR;
            code_size = EDINBURGH_DVBT2_P1_END_ADDR - EDINBURGH_DVBT2_P1_START_ADDR + 1;
            win_offset = EDINBURGH_DVBT2_P1_WINDOWS_OFFSET;
            bRet &= _Load2Sdram(data_ptr,code_size,win_offset);

            if(bRet == true)
            {
                u8DMD_MSB123xc_Sdram_Code |= MSB123xc_DVBT2;
            }
        }
        else
        {
#ifdef MS_DEBUG
            ERR_DOMOD_MSB(printf("@msb123xc, dvbt2 code is unavailable!!!\n"));
#endif
        }
    }
#ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]LoadDspCodeToSDRAM_dvbt2\n"));
#endif
    return bRet;
}

static MS_BOOL _LoadDspCodeToSDRAM_dvbt(void)
{
    MS_BOOL bRet = true;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;

#ifdef MS_DEBUG
    DBG_DOMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]LoadDspCodeToSDRAM_dvbt\n"));
#endif

    if(!(u8DMD_MSB123xc_Sdram_Code&MSB123xc_DVBT))
    {
        // dvbt code
        if(sizeof(MSB123xc_LIB) > EDINBURGH_DVBT_START_ADDR)
        {
            data_ptr = MSB123xc_LIB + EDINBURGH_DVBT_START_ADDR;
            code_size = EDINBURGH_DVBT_END_ADDR - EDINBURGH_DVBT_START_ADDR + 1;
            win_offset = EDINBURGH_DVBT_WINDOWS_OFFSET;
            bRet &= _Load2Sdram(data_ptr,code_size,win_offset);
            if(bRet == true)
            {
                u8DMD_MSB123xc_Sdram_Code |= MSB123xc_DVBT;
            }
        }
        else
        {
            printf("@msb123xc, dvbt code is unavailable!!!\n");
        }
    }
#ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]LoadDspCodeToSDRAM_dvbt\n"));
#endif
    return bRet;
}

static MS_BOOL _LoadDspCodeToSDRAM_dvbc(void)
{
    MS_BOOL bRet = true;
    MS_U16  code_size, win_offset;
    MS_U8   *data_ptr;

#ifdef MS_DEBUG
    DBG_DOMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]LoadDspCodeToSDRAM_dvbc\n"));
#endif

    if(!(u8DMD_MSB123xc_Sdram_Code&MSB123xc_DVBC))
    {
        // dvbc code
        if(sizeof(MSB123xc_LIB) > EDINBURGH_DVBC_START_ADDR)
        {
            data_ptr = MSB123xc_LIB + EDINBURGH_DVBC_START_ADDR;
            code_size = EDINBURGH_DVBC_END_ADDR - EDINBURGH_DVBC_START_ADDR + 1;
            win_offset = EDINBURGH_DVBC_WINDOWS_OFFSET;
            bRet &= _Load2Sdram(data_ptr,code_size,win_offset);
            if(bRet == true)
            {
                u8DMD_MSB123xc_Sdram_Code |= MSB123xc_DVBC;
            }
        }
        else
        {
            printf("@msb123xc, dvbc code is unavailable!!!\n");
        }
    }
#ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]LoadDspCodeToSDRAM_dvbc\n"));
#endif
    return bRet;
}

static MS_BOOL _LoadDspCodeToSDRAM(MS_U8 code_n)
{
    MS_BOOL bRet = true;
#ifdef MS_DEBUG
    MS_U32 u32tmm_7 = 0, u32tmm_8 = 0;
#endif

#ifdef MS_DEBUG
    DBG_DOMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]LoadDspCodeToSDRAM, code_n=0x%x\n",code_n));
    u32tmm_7 = MsOS_GetSystemTime();
#endif

    if (_sDMD_MSB123xc_InitData.bEnableSPILoadCode)
    {
        (*_sDMD_MSB123xc_InitData.fpMSB123xc_SPIPAD_En)(TRUE);
        // ------disable to use TS_PAD as SSPI_PAD after load code
        // [0:0] reg_en_sspi_pad
        // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
        _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900 + (0x3b) * 2, 0x0002);
    }

    switch(code_n)
    {
    case MSB123xc_BOOT:
    {
        // boot code
        bRet &= _LoadDspCodeToSDRAM_Boot();
    }
    break;
    case MSB123xc_DVBT2:
    {
        // dvbt2 code
        bRet &= _LoadDspCodeToSDRAM_dvbt2();
    }
    break;
    case MSB123xc_DVBT:
    {
        // dvbt
        bRet &= _LoadDspCodeToSDRAM_dvbt();
    }
    break;
    case MSB123xc_DVBC:
    {
        // dvbtc
        bRet &= _LoadDspCodeToSDRAM_dvbc();
    }
    break;
    case MSB123xc_ALL:
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
    }
    break;
    }

    if (_sDMD_MSB123xc_InitData.bEnableSPILoadCode)
    {
        // ------disable to use TS_PAD as SSPI_PAD after load code
        // [0:0] reg_en_sspi_pad
        // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
        _MDrv_DMD_MSB123xc_SetReg2Bytes(0x0900 + (0x3b) * 2, 0x0001);
        (*_sDMD_MSB123xc_InitData.fpMSB123xc_SPIPAD_En)(FALSE);
    }

#ifdef MS_DEBUG
    u32tmm_8 = MsOS_GetSystemTime();
    printf("[tmm4]t8-t7 = %ld (%ld - %ld)\n",u32tmm_8-u32tmm_7,u32tmm_8,u32tmm_7);
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]LoadDspCodeToSDRAM, code_n=0x%x, bRet=0x%x\n",code_n,bRet));
#endif

    return bRet;
}

static MS_BOOL _MSB123xc_MEM_switch(MS_U8 mem_type)
{
    MS_BOOL bRet = true;
    MS_U8 u8_tmp = 0;
    MS_U16 timeout = 0;
#ifdef MS_DEBUG
    MS_U32 u32tmm_15 = 0, u32tmm_16 = 0;
#endif

#ifdef MS_DEBUG
    DBG_DOMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]MSB123xc_MEM_switch, mem_type=0x%x\n",mem_type));
    u32tmm_15 = MsOS_GetSystemTime();
#endif

    if(mem_type == 1)
    {

        bRet &= _MDrv_DMD_MSB123xc_SetReg(0x2B80,0x10);

        // SRAM_START_ADDR 0x0000
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1000,0x0000);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1004,0x0000);

        // SRAM_END_ADDR 0x7FFF
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1002,0x0000);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1006,0x7FFF);

        // DRAM_START_ADDR 0x8000
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1008,0x0000);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x100C,0x8000);


        // DRAM_END_ADDR    0xFFFF
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x100A,0x0000);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x100E,0xFFFF);

        // Enable SRAM&SDRAM memory map
        bRet &= _MDrv_DMD_MSB123xc_SetReg(0x1018,0x05);

        // Wait memory map to be enabled
        do
        {
            bRet &= _MDrv_DMD_MSB123xc_GetReg(0x1018,&u8_tmp);
            if(timeout++ > 500)
            {
                printf("@msb123xc, D+S memory mapping failure.!!!\n");
                return FALSE;
            }
        }
        while(u8_tmp != 0x05);
    }
    else if(mem_type == 0)
    {
        // Enable SRAM&SDRAM memory map

        bRet &= _MDrv_DMD_MSB123xc_SetReg(0x2B80,0x10);

        // DRAM_START_ADDR 0x8000
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x1008,0x0000);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x100C,0x0000);


        // DRAM_END_ADDR    0xFFFF
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x100A,0x0000);
        bRet &= _MDrv_DMD_MSB123xc_SetReg2Bytes(0x100E,0x7FFF);

        // Enable SRAM&SDRAM memory map
        bRet &= _MDrv_DMD_MSB123xc_SetReg(0x1018,0x04);

        // Wait memory map to be enabled
        do
        {
            bRet &= _MDrv_DMD_MSB123xc_GetReg(0x1018,&u8_tmp);
            if(timeout++ > 500)
            {
                printf("@msb123xc, D memory mapping failure.!!!\n");
                return FALSE;
            }
        }
        while(u8_tmp != 0x04);
    }
    else
    {
        printf("@msb123xc, invalid mem type mapping.\n");
        return FALSE;
    }

#ifdef MS_DEBUG
    u32tmm_16 = MsOS_GetSystemTime();
    printf("[tmm8]t16-t15 = %ld (%ld - %ld)\n",u32tmm_16-u32tmm_15,u32tmm_16,u32tmm_15);
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]MSB123xc_MEM_switch, , mem_type=0x%x, bRet=0x%x\n",mem_type,bRet));
#endif

    return bRet;
}

static MS_BOOL _LoadSdram2Sram(MS_U8 CodeNum)
{
    MS_BOOL bRet = true;
    MS_U8   u8_tmp = 0;
    MS_U8   u8DoneFlag = 0;
    MS_U32  u32Timeout = 0;
#ifdef MS_DEBUG
    MS_U32 u32tmm_17 = 0, u32tmm_18 = 0;
#endif

#ifdef MS_DEBUG
    DBG_DOMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]LoadSdram2Sram, g_sram_code=0x%x, codeNum=0x%x\n",u8DMD_MSB123xc_Sram_Code,CodeNum));
    u32tmm_17 = MsOS_GetSystemTime();
#endif

    if(CodeNum == u8DMD_MSB123xc_Sram_Code)
    {
#ifdef MS_DEBUG
        printf("[msb123xc]LoadSdram2Sram, code is available.\n");
#endif
        return bRet;
    }

    bRet &= _MSB123xc_MEM_switch(0);

    if(CodeNum == MSB123xc_DVBT2)
        u8_tmp = 1|0x10;
    else if(CodeNum == MSB123xc_DVBT)
        u8_tmp = 2|0x10;
    else if(CodeNum == MSB123xc_DVBC)
        u8_tmp = 3|0x10;
    else
        u8_tmp = 0|0x10;

    // Assign f/w code type to load => 0x11: dvbt2, 0x12: dvbt, 0x13: dvbc
    bRet &= _MDrv_DMD_MSB123xc_SetReg(0x0900+(0x4f)*2, u8_tmp);

    // enable miu mask, miu, mcu, gdma
    bRet &= _MDrv_DMD_MSB123xc_SetReg(0x1200+(0x23)*2 + 1,0x0f);
    bRet &= _MDrv_DMD_MSB123xc_SetReg(0x1200+(0x23)*2,0xf0);

    MsOS_DelayTaskUs(10);
    // enable mcu
    bRet &= _MDrv_DMD_MSB123xc_SetReg(0x0b00+(0x19)*2, 0x00);

    do
    {
        bRet &= _MDrv_DMD_MSB123xc_GetReg(0x0900+(0x4f)*2, &u8DoneFlag);

        if (u32Timeout++ > 500)
        {
            printf("@msb123xc, LoadSdram2Sram boot move code fail.!!!\n");
            return FALSE;
        }
        MsOS_DelayTaskUs(1*1000);

    }
    while(u8DoneFlag != 0xaa);

    // mask miu access of mcu
    bRet &= _MDrv_DMD_MSB123xc_SetReg(0x1200+(0x23)*2,0xf2);

    // 10us delay
    MsOS_DelayTaskUs(10);

    // Disable MCU
    bRet &= _MDrv_DMD_MSB123xc_SetReg(0x0b00+(0x19)*2, 0x03);

    // enable miu mask, miu, mcu, gdma
    bRet &= _MDrv_DMD_MSB123xc_SetReg(0x1200+(0x23)*2 + 1,0x0f);
    bRet &= _MDrv_DMD_MSB123xc_SetReg(0x1200+(0x23)*2,0xf0);

    bRet &= _MSB123xc_MEM_switch(1);

    if(bRet == FALSE)
    {
        u8DMD_MSB123xc_Sram_Code = 0x00;
    }
    else
    {
        u8DMD_MSB123xc_Sram_Code = CodeNum;
    }

#ifdef MS_DEBUG
    u32tmm_18 = MsOS_GetSystemTime();
    printf("[tmm9]t18-t17 = %ld (%ld - %ld)\n",u32tmm_18-u32tmm_17,u32tmm_18,u32tmm_17);
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]LoadSdram2Sram, codeNum=0x%x, g_sram_code=0x%x, bRet=0x%x\n",CodeNum,u8DMD_MSB123xc_Sram_Code,bRet));
#endif

    return bRet;
}

static MS_BOOL  _DTV_DVBC_DSPReg_Init(void)
{
    MS_U8    idx = 0;

#ifdef MS_DEBUG
    DBG_DOMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
#endif
    for (idx = 0; idx<((MS_U8)DVBC_PARAM_LEN - (MS_U8)C_opmode_rfagc_en); idx++)
    {
        if( _MDrv_DMD_MSB123xc_SetDSPReg(idx + (MS_U8)C_opmode_rfagc_en, MSB123xc_DVBC_DSPREG_TABLE[idx])!=TRUE)
        {
#ifdef MS_DEBUG
            ERR_DOMOD_MSB(printf("dsp reg init NG\n"));
#endif
            return FALSE;
        }
    }

#ifdef MS_DEBUG
    ERR_DOMOD_MSB(printf("DVBC dsp reg init ok\n"));
#endif

    return TRUE;
}


static MS_U8  _DTV_DVBT_DSPReg_CRC(void)
{
    MS_U8 crc = 0;
    MS_U8 idx = 0;

#ifdef MS_DEBUG
    DBG_DOMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
#endif
    for (idx = 0; idx<(sizeof(MSB123xc_DVBT_DSPREG_TABLE)); idx++)
    {
        crc ^= MSB123xc_DVBT_DSPREG_TABLE[idx];
    }

    crc = ~crc;

    return crc;
}

static MS_BOOL  _DTV_DVBT_DSPReg_Init(void)
{
    //MS_U8    idx = 0;
#ifdef MS_DEBUG
    MS_U32 u32tmm_7 = 0, u32tmm_8 = 0;
#endif

#ifdef MS_DEBUG
    DBG_DOMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    u32tmm_7 = MsOS_GetSystemTime();
#endif

    if( _MDrv_DMD_MSB123xc_SetDSPReg((MS_U8)T_CONFIG_FC_L, MSB123xc_DVBT_DSPREG_TABLE[0x13]) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( _MDrv_DMD_MSB123xc_SetDSPReg((MS_U8)T_CONFIG_FC_H, MSB123xc_DVBT_DSPREG_TABLE[0x14]) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( _MDrv_DMD_MSB123xc_SetDSPReg((MS_U8)T_CONFIG_BW, MSB123xc_DVBT_DSPREG_TABLE[0x17]) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( _MDrv_DMD_MSB123xc_SetDSPReg((MS_U8)T_CONFIG_IQ_SWAP, MSB123xc_DVBT_DSPREG_TABLE[0x1F]) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( _MDrv_DMD_MSB123xc_SetDSPReg((MS_U8)T_CONFIG_TS_SERIAL, MSB123xc_DVBT_DSPREG_TABLE[0x2B]) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( _MDrv_DMD_MSB123xc_SetDSPReg((MS_U8)T_CONFIG_TS_CLK_RATE, MSB123xc_DVBT_DSPREG_TABLE[0x2C]) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( _MDrv_DMD_MSB123xc_SetDSPReg((MS_U8)T_CONFIG_TS_OUT_INV, MSB123xc_DVBT_DSPREG_TABLE[0x2D]) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( _MDrv_DMD_MSB123xc_SetDSPReg((MS_U8)T_CONFIG_TS_DATA_SWAP, MSB123xc_DVBT_DSPREG_TABLE[0x2E]) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }


    /*
        for (idx = 0; idx<((MS_U8)DVBT_PARAM_LEN - (MS_U8)T_OPMODE_RFAGC_EN - 1); idx++)
        {
            if( _MDrv_DMD_MSB123xc_SetDSPReg(idx + (MS_U8)T_OPMODE_RFAGC_EN, MSB123xc_DVBT_DSPREG_TABLE[idx])!=TRUE)
            {
                printf("dsp reg init NG\n");
                return FALSE;
            }
        }

        #ifdef MS_DEBUG
        u32tmm_8 = MsOS_GetSystemTime();
        printf("[1111]t8 - t7 = %ld (%ld - %ld) \n",u32tmm_8-u32tmm_7,u32tmm_8,u32tmm_7);
        u32tmm_7 = MsOS_GetSystemTime();
        #endif

        MSB123xc_DVBT_DSPREG_TABLE[(MS_U8)T_PARAM_CHECK_SUM-(MS_U8)T_OPMODE_RFAGC_EN] = _DTV_DVBT_DSPReg_CRC();

        #ifdef MS_DEBUG
        u32tmm_8 = MsOS_GetSystemTime();
        printf("[2222]t8 - t7 = %ld (%ld - %ld) \n",u32tmm_8-u32tmm_7,u32tmm_8,u32tmm_7);
        u32tmm_7 = MsOS_GetSystemTime();
        #endif

        if( _MDrv_DMD_MSB123xc_SetDSPReg((MS_U8)T_PARAM_CHECK_SUM, MSB123xc_DVBT_DSPREG_TABLE[(MS_U8)T_PARAM_CHECK_SUM-(MS_U8)T_OPMODE_RFAGC_EN])!=TRUE)
        {
            printf("dsp reg write crc NG\n");
            return FALSE;
        }
    */
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
    DBG_DOMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
#endif

    if( _MDrv_DMD_MSB123xc_SetDSPReg((MS_U8)E_T2_BW, MSB123xc_DVBT2_DSPREG_TABLE[0]) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( _MDrv_DMD_MSB123xc_SetDSPReg((MS_U8)E_T2_FC_L, MSB123xc_DVBT2_DSPREG_TABLE[1]) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( _MDrv_DMD_MSB123xc_SetDSPReg((MS_U8)E_T2_FC_H, MSB123xc_DVBT2_DSPREG_TABLE[2]) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( _MDrv_DMD_MSB123xc_SetDSPReg((MS_U8)E_T2_TS_SERIAL, MSB123xc_DVBT2_DSPREG_TABLE[3]) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( _MDrv_DMD_MSB123xc_SetDSPReg((MS_U8)E_T2_TS_CLK_RATE, MSB123xc_DVBT2_DSPREG_TABLE[4]) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( _MDrv_DMD_MSB123xc_SetDSPReg((MS_U8)E_T2_TS_OUT_INV, MSB123xc_DVBT2_DSPREG_TABLE[5]) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( _MDrv_DMD_MSB123xc_SetDSPReg((MS_U8)E_T2_TS_DATA_SWAP, MSB123xc_DVBT2_DSPREG_TABLE[6]) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }
    if( _MDrv_DMD_MSB123xc_SetDSPReg((MS_U8)E_T2_TS_ERR_POL, MSB123xc_DVBT2_DSPREG_TABLE[7]) != TRUE)
    {
        printf("T2 dsp reg init NG\n");
    }

    printf("T2 dsp reg init ok\n");

    return TRUE;
}

static MS_BOOL _LoadDSPCode(void)
{
    ////MAPI_U32 u32Now = MsOS_GetSystemTime();
    //printf("\t\t\tLoadDSPCode TIME   %ld (=%ld-%ld)\n", u32Now-u32StartTime, u32Now, u32StartTime);
    ////DBG_DOMOD_MSB(printf("\t\t\tLoadDSPCode TIME   %ld (=%ld-%ld)\n", u32Now-u32StartTime, u32Now, u32StartTime));//to measure time
    //u32StartTime = u32Now;

    MS_U32        u32Timeout = 0;
    MS_U8         u8DoneFlag = 0;
    MS_U8         u8Data = 0;
    MS_BOOL       bRet = true;
#ifdef MS_DEBUG
    MS_U32 u32tmm_9 = 0, u32tmm_10 = 0, u32tmm_11 = 0, u32tmm_12 = 0, u32tmm_13 = 0;
#endif

#ifdef MS_DEBUG
    DBG_DOMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]LoadDspCode\n"));
    DBG_DOMOD_MSB(printf(">>>MSB123xc: Load DSP...\n"));
#endif

    switch(eDMD_MSB123xc_CurrentDemodulatorType)
    {
    case E_DMD_MSB123xc_DEMOD_DVBT2:
        u8Data=1;
        break;
    case E_DMD_MSB123xc_DEMOD_DVBT:
        u8Data=2;
        break;
    case E_DMD_MSB123xc_DEMOD_DVBC:
        u8Data=3;
        break;
    default:
        u8Data=2;
        return FALSE;
    }

    if(_sDMD_MSB123xc_InitData.u8WO_SPI_Flash == 1)
    {
        MS_U8 u8FirmwareType = MSB123xc_DVBT;

#ifdef MS_DEBUG
        u32tmm_11 = MsOS_GetSystemTime();
#endif

        switch (u8Data)
        {
        case 1:
            u8FirmwareType = MSB123xc_DVBT2;
            break;
        case 2:
        default:
            u8FirmwareType = MSB123xc_DVBT;
            break;
        case 3:
            u8FirmwareType = MSB123xc_DVBC;
            break;
        }

        bRet &= _LoadDspCodeToSDRAM(u8FirmwareType);

        // mask miu access for all and mcu
        bRet &= _MDrv_DMD_MSB123xc_SetReg(0x1200+(0x23)*2 + 1,0x7f);
        bRet &= _MDrv_DMD_MSB123xc_SetReg(0x1200+(0x23)*2,0xfe);
        // 10us delay
        MsOS_DelayTaskUs(10);

        // Disable MCU
        bRet &= _MDrv_DMD_MSB123xc_SetReg(0x0b00+(0x19)*2, 0x03);

        bRet &= _LoadSdram2Sram(u8FirmwareType);

        // enable miu access of mcu gdma
        bRet &= _MDrv_DMD_MSB123xc_SetReg(0x1200+(0x23)*2,0xf0);
        // 10us delay
        MsOS_DelayTaskUs(10);

        // Enable MCU
        bRet &= _MDrv_DMD_MSB123xc_SetReg(0x0b00+(0x19)*2, 0x00);
    }
    else
    {
#ifdef MS_DEBUG
        u32tmm_9 = MsOS_GetSystemTime();
#endif
        // mask miu access for all and mcu
        bRet &= _MDrv_DMD_MSB123xc_SetReg(0x1200+(0x23)*2 + 1,0x7f);
        bRet &= _MDrv_DMD_MSB123xc_SetReg(0x1200+(0x23)*2,0xfe);
        // 10us delay
        MsOS_DelayTaskUs(10);

        // Disable MCU
        _MDrv_DMD_MSB123xc_SetReg(0x0b00+(0x19)*2, 0x03);

        // Run code on bootloader
        _MDrv_DMD_MSB123xc_SetReg(0x1000+(0x0c)*2, 0x02);

        // Assign f/w code type to load => 0: boot-loader 1: dvbt2, 2: dvbt, 3: dvbc
        _MDrv_DMD_MSB123xc_SetReg(0x0900 + (0x4f) * 2, u8Data);


        // enable miu access of mcu gdma
        bRet &= _MDrv_DMD_MSB123xc_SetReg(0x1200+(0x23)*2,0xf0);
        // 10us delay
        MsOS_DelayTaskUs(10);

        // Enable MCU
        _MDrv_DMD_MSB123xc_SetReg(0x0b00+(0x19)*2, 0x00);

#ifdef MS_DEBUG
        u32tmm_10 = MsOS_GetSystemTime();
        printf("[tmm8]t10 - t9 = %ld (%ld - %ld)\n",u32tmm_10-u32tmm_9,u32tmm_10,u32tmm_9);
#endif


        do
        {
            _MDrv_DMD_MSB123xc_GetReg(0x0900+(0x4f)*2, &u8DoneFlag);

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
        bRet &= _MDrv_DMD_MSB123xc_SetReg(0x1200+(0x23)*2 + 1,0x7f);
        bRet &= _MDrv_DMD_MSB123xc_SetReg(0x1200+(0x23)*2,0xfe);
        // 10us delay
        MsOS_DelayTaskUs(10);

        // Disable MCU
        _MDrv_DMD_MSB123xc_SetReg(0x0b00+(0x19)*2, 0x03);

        // Run code on loaded firmware
        _MDrv_DMD_MSB123xc_SetReg(0x1000+(0x0c)*2, 0x05);

        do
        {
            _MDrv_DMD_MSB123xc_GetReg(0x1000+(0x0c)*2, &u8DoneFlag);

            if (u32Timeout++ > 500)
                return FALSE;

            MsOS_DelayTaskUs(1*1000);

        }
        while(u8DoneFlag != 0x05);

        // enable miu access of mcu gdma
        bRet &= _MDrv_DMD_MSB123xc_SetReg(0x1200+(0x23)*2,0xf0);
        // 10us delay
        MsOS_DelayTaskUs(10);

        // Enable MCU
        _MDrv_DMD_MSB123xc_SetReg(0x0b00+(0x19)*2, 0x00);
    }

#ifdef MS_DEBUG
    u32tmm_12 = MsOS_GetSystemTime();
    printf("[tmm8]t12 - t11 = %ld (%ld - %ld), TYPE is %d \n",u32tmm_12-u32tmm_11,u32tmm_12,u32tmm_11, eDMD_MSB123xc_CurrentDemodulatorType);
#endif

    switch(eDMD_MSB123xc_CurrentDemodulatorType)
    {
    case E_DMD_MSB123xc_DEMOD_DVBT2:
        _DTV_DVBT2_DSPReg_Init();
        break;
    case E_DMD_MSB123xc_DEMOD_DVBT:
        _DTV_DVBT_DSPReg_Init();
        break;
    case E_DMD_MSB123xc_DEMOD_DVBC://mick
        _DTV_DVBC_DSPReg_Init();
        break;
    default:
        return FALSE;
    }

#ifdef MS_DEBUG
    u32tmm_13 = MsOS_GetSystemTime();
    printf("[tmm8]t13 - t12 = %ld (%ld - %ld)\n",u32tmm_13-u32tmm_12,u32tmm_13,u32tmm_12);
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]LoadDspCode\n"));
#endif

    return bRet;
}

static MS_BOOL _msb123xc_flash_mode_en(void)
{
    MS_BOOL  bRet = TRUE;
    MS_U8    data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    MS_U8    u8MsbData[6] = {0};
    MS_U8    ch_num  = 3;
    MS_U8    u8Data  = 0;
    MS_U16   u16Addr = 0;
    MS_U8    retry_num = MSB123xc_MAX_FLASH_ON_RETRY_NUM;

#ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]msb123xc_flash_mode_en\n"));
#endif

    do
    {

        if (retry_num != MSB123xc_MAX_FLASH_ON_RETRY_NUM)
        {
#ifdef MS_DEBUG
            ERR_DOMOD_MSB(printf("[msb123xc][error]flash mode en fail.....retry=%d\n",retry_num);)
#endif
        }
        // bRet = TRUE;
        // password
        // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
        data[0] = 0x53;
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, data);

        // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_
        data[0] = 0x71;
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, data);

        // 8'hb2(SRID)->8,h81(CMD)  //TV.n_iic_sel_b0
        data[0] = ((ch_num & 0x01) != 0)? 0x81 : 0x80;
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, data);

        // 8'hb2(SRID)->8,h83(CMD)  //TV.n_iic_sel_b1
        data[0] = ((ch_num & 0x02) != 0)? 0x83 : 0x82;
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, data);

        // 8'hb2(SRID)->8,h84(CMD)  //TV.n_iic_sel_b2
        data[0] = ((ch_num & 0x04) != 0)? 0x85 : 0x84;
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, data);

        // 8'hb2(SRID)->8,h53(CMD)  //TV.n_iic_ad_byte_en2, 32bit read/write
        data[0] = 0x53;
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, data);

        // 8'hb2(SRID)->8,h7f(CMD)  //TV.n_iic_sel_use_cfg
        data[0] = 0x7f;
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, data);

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
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

        u8MsbData[0] = 0x10;
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8MsbData);
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_READ_BYTES, 0, NULL, 1, &u8Data);

        u8MsbData[0] = 0x34;
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);
        // end read register

        if ((u8Data == 0x99) || (u8Data == 0xaa))
        {
#ifdef MS_DEBUG
            ERR_DOMOD_MSB(printf("[msb123xc][warning]flash is already on....\n");)
#endif
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

        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);



        u8MsbData[0] = 0x10;
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8MsbData);
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_READ_BYTES, 0, NULL, 1, &u8Data);

        u8MsbData[0] = 0x34;
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);
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
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

        u8MsbData[0] = 0x10;
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 6, u8MsbData);

        u8MsbData[0] = 0x34;
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);
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
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

        u8MsbData[0] = 0x10;
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 6, u8MsbData);

        u8MsbData[0] = 0x34;
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);
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
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

        u8MsbData[0] = 0x10;
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 6, u8MsbData);

        u8MsbData[0] = 0x34;
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);
    }
    while( (bRet == FALSE) && (retry_num-- != 0));
    // end write register

#ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]msb123xc_flash_mode_en,bRet=%d\n",bRet));
#endif

    return bRet;
}

static MS_BOOL _msb123xc_flash_boot_ready_waiting(MS_U8 *ptimeout)
{

    MS_BOOL  bRet = TRUE;
//    MAPI_U8    data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    MS_U8    u8MsbData[6] = {0};
    MS_U8    u8Data  = 0;
    MS_U16   u16Addr = 0;
    MS_U8    u8_timeout = 0;

#ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][beg]msb123xc_flash_boot_ready_waiting\n"));
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
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

        u8MsbData[0] = 0x10;
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8MsbData);
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_READ_BYTES, 0, NULL, 1, &u8Data);

        u8MsbData[0] = 0x34;
        bRet &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);
        // 10ms
        MsOS_DelayTaskUs(1000*10);
    }
    // end read register
    *ptimeout = 0;
    if (u8_timeout == 0x00)
    {
        *ptimeout = 1;
#ifdef MS_DEBUG
        ERR_DOMOD_MSB(printf("[msb123xc][error]msb123xc_flash_boot_ready_waiting, timeout....\n");)
#endif
    }

#ifdef MS_DEBUG
    DBG_DEMOD_LOAD_I2C(printf("[msb123xc][end]msb123xc_flash_boot_ready_waiting, t=%d\n",u8_timeout));
#endif
    return bRet;
}

static MS_BOOL _msb123xc_flash_WP_reg_read(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet;
    MS_U8   u8MsbData[5];

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;

    u8MsbData[0] = 0x35;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8MsbData);
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_READ_BYTES, 0, NULL, 1, pu8Data);

    u8MsbData[0] = 0x34;
    bRet=(*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);
    return bRet;
}

static MS_BOOL _msb123xc_flash_WP_reg_write(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet;
    MS_U8   u8MsbData[6];

    //bRet &= iptr->SetSpeed(0);


    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;
    u8MsbData[5] = u8Data;

    u8MsbData[0] = 0x35;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 6, u8MsbData);

    u8MsbData[0] = 0x34;
    bRet=(*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);
    return bRet;
}

static MS_BOOL _msbMSB123xc_flash_WRSR(MS_U8 reg)
{
    MS_U8 bWriteData[5]= {0x4D, 0x53, 0x54, 0x41, 0x52};
    MS_U8     bAddr[1];
    MS_BOOL   bRet = TRUE;

    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, bWriteData);

    // WREN
    bAddr[0] = 0x10;
    bWriteData[0] = 0x06;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 1, bAddr, 1, bWriteData);

    bWriteData[0] = 0x12;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

    // WRSR
    bAddr[0] = 0x10;
    bWriteData[0] = 0x01;
    bWriteData[1] = reg;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 1, bAddr, 2, bWriteData);

    bWriteData[0] = 0x12;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

    // WRDI
    bAddr[0] = 0x10;
    bWriteData[0] = 0x04;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 1, bAddr, 1, bWriteData);

    bWriteData[0] = 0x12;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

    // end
    bWriteData[0] = 0x24;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

#ifdef MS_DEBUG
    DBG_FLASH_WP(printf("[wb]msbMSB123xc_flash_WRSR, reg=0x%x\n",reg);)
#endif

    return bRet;
}

static MS_BOOL _msbMSB123xc_flash_SRSR(MS_U8 *p_reg)
{
    MS_U8 bWriteData[5]= {0x4D, 0x53, 0x54, 0x41, 0x52};
    MS_U8     bAddr[1];
    MS_BOOL   bRet = TRUE;

    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, bWriteData);

    bAddr[0] = 0x10;
    bWriteData[0] = 0x05;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 1, bAddr, 1, bWriteData);

    bAddr[0] = 0x11;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_READ_BYTES, 1, bAddr, 1, p_reg);

    bWriteData[0] = 0x12;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

    // end
    bWriteData[0] = 0x24 ;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

#ifdef MS_DEBUG
    DBG_FLASH_WP(printf("[wb]msbMSB123xc_flash_SRSR, reg=0x%x\n",*p_reg);)
#endif

    return bRet;
}

static MS_BOOL _msb123xc_flash_WP(MS_U8 enable)
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
        }
        while(reg&0x01 && u8_count--);

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
        }
        while(reg&0x01 && u8_count--);

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
        }
        while(reg&0x01 && u8_count--);

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
        }
        while(reg&0x01 && u8_count--);

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
    DBG_FLASH_WP(printf("[wb]msb123xc_flash_WP_Enable=%d\n",enable);)
#endif

    if (enable == 1)
    {
        u8_count = 20;
        do
        {
            _msbMSB123xc_flash_SRSR(&reg);
            MsOS_DelayTaskUs(1*1000);
        }
        while(reg&0x01 && u8_count--);

        if (u8_count == 0)
        {
            bRet = FALSE;
#ifdef MS_DEBUG
            DBG_FLASH_WP(printf("[wb]Err, flash_SRSR timeout!!!\n");)
#endif
            return bRet;
        }

        _msbMSB123xc_flash_WRSR(reg|0x9c);
        u8_count = 20;

        do
        {
            _msbMSB123xc_flash_SRSR(&reg);
            MsOS_DelayTaskUs(1*1000);
        }
        while(reg&0x01 && u8_count--);

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
        bRet &= _msb123xc_flash_WP_reg_read(0x0A00+0x22*2, &reg);
        bRet &= _msb123xc_flash_WP_reg_write(0x0A00+0x22*2, reg&(~0x01));
        // gpio11 output enable
        bRet &= _msb123xc_flash_WP_reg_read(0x0A00+0x22*2, &reg);
        bRet &= _msb123xc_flash_WP_reg_write(0x0A00+0x22*2, reg&(~0x02));
    }
    else
    {
        // unactive high
        // pull high
        bRet &= _msb123xc_flash_WP_reg_read(0x0A00+0x22*2, &reg);
        bRet &= _msb123xc_flash_WP_reg_write(0x0A00+0x22*2, reg|0x01);
        // gpio11 output enable
        bRet &= _msb123xc_flash_WP_reg_read(0x0A00+0x22*2, &reg);
        bRet &= _msb123xc_flash_WP_reg_write(0x0A00+0x22*2, reg&(~0x02));
        u8_count = 20;

        do
        {
            _msbMSB123xc_flash_SRSR(&reg);
            MsOS_DelayTaskUs(1*1000);
        }
        while(reg&0x01 && u8_count--);

        if (u8_count == 0)
        {
            bRet = FALSE;
#ifdef MS_DEBUG
            DBG_FLASH_WP(printf("[wb]Err, flash_SRSR timeout!!!\n");)
#endif
            return bRet;
        }
        _msbMSB123xc_flash_WRSR(reg&(~0x9c));
        u8_count = 20;

        do
        {
            _msbMSB123xc_flash_SRSR(&reg);
            MsOS_DelayTaskUs(1*1000);
        }
        while(reg&0x01 && u8_count--);

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

MS_U8 _MSB123xc_CHIP_MATCH_TABLE[] =
{
    //Kaiser, Kaiserin, Keltic, Kronus, Kappa , Clippers, keres
    0x56,       0x41,     0x72,  0x2F,  0x75,   0x86,    0x7E
};

static MS_BOOL _msbMSB123xc_set_bonding_option(MS_U16 u16ChipID)
{
    MS_BOOL  bRet = TRUE;
    MS_U8    u8Idx;
    MS_U8    u8MatchFlag  = 0;
    MS_U8    u8Data  = 0;
    MS_U32    u32_timeout = 0;
#ifdef MS_DEBUG
    DBG_DOMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
#endif

    printf("_msbMSB123xc_set_bonding_option u16ChipID %x\n", u16ChipID);

    for (u8Idx = 0 ; u8Idx < sizeof( _MSB123xc_CHIP_MATCH_TABLE) ; u8Idx++)
    {
        if(u16ChipID == _MSB123xc_CHIP_MATCH_TABLE[u8Idx])
        {
            printf("_MSB123xc_CHIP_MATCH_TABLE matched!\n");
            u8MatchFlag = 0x01;
            break;
        }
        else
        {
            printf("_MSB123xc_CHIP_MATCH_TABLE did not matched!\n");
            u8MatchFlag = 0x00;
        }
    }

    if (_MSB123xc_I2C_CH_Reset(3) == FALSE)
    {
#ifdef MS_DEBUG
        ERR_DOMOD_MSB(printf(">>>MSB123xc CH3 Reset:Fail\n"));
#endif
        printf(">>>MSB123xc CH3 Reset:Fail\n");
        return FALSE;
    }

    // MSB123xc : 0x0902[8]=0 , 0x0902[0]=0;
    // MSB1235 : 0x0902[8]=1 , 0x0902[0]=1; (before overwrite, SDRAM not enable)
    //                  0x0902[8]=1 , 0x0902[0]=0; (after overwrite, SDRAM enable)
    // check bonding value, 0x0902[8]
    bRet &= _MDrv_DMD_MSB123xc_GetReg(0x0900+(0x02)*2+1, &u8Data);
    if((u8Data & 0x01) == 0x01) //for MSB1236C
    {
        if(u8MatchFlag == 0x01)
        {
            //check overwrite or not
            //0x0902[0] : reg_bonding[0]
            bRet &= _MDrv_DMD_MSB123xc_GetReg(0x0900+(0x02)*2, &u8Data);
            if((u8Data & 0x01) != 0x00)
            {
                //0x0905[0] : reg_bond_ov_en[0] = 1
                //0x0905[8] : reg_bond_ov[0] = 0
                // set overwrite enable
                bRet &= _MDrv_DMD_MSB123xc_SetReg(0x0900+(0x05)*2, 0x01);
                // set overwrite value
                bRet &= _MDrv_DMD_MSB123xc_SetReg(0x0900+(0x05)*2+1, 0x00);
            }

            do
            {
                bRet &= _MDrv_DMD_MSB123xc_GetReg(0x0900+(0x02)*2, &u8Data);
                if(u32_timeout++ > 500)
                {
#ifdef MS_DEBUG
                    ERR_DOMOD_MSB(printf("@msb1236c, Set bonding option failure.!!!\n"));
#endif
                    return FALSE;
                }
            }
            while((u8Data & 0x01) == 0x01);

#ifdef MS_DEBUG
            printf("@ Set bonding option for MSB1236 \n");
#endif
        }
        else
        {
            return FALSE;
        }
    }
    else  // for MSB123xc
    {
        //check overwrite or not
        //0x0902[0] : reg_bonding[0]
        bRet &= _MDrv_DMD_MSB123xc_GetReg(0x0900+(0x02)*2, &u8Data);
        if((u8Data & 0x01) != 0x00)
        {
            //0x0905[0] : reg_bond_ov_en[0] = 1
            //0x0905[8] : reg_bond_ov[0] = 0
            // set overwrite enable
            bRet &= _MDrv_DMD_MSB123xc_SetReg(0x0900+(0x05)*2, 0x01);
            // set overwrite value
            bRet &= _MDrv_DMD_MSB123xc_SetReg(0x0900+(0x05)*2+1, 0x00);
        }

        do
        {
            bRet &= _MDrv_DMD_MSB123xc_GetReg(0x0900+(0x02)*2, &u8Data);
            if(u32_timeout++ > 500)
            {
#ifdef MS_DEBUG
                ERR_DOMOD_MSB(printf("@msbMSB123xc, Set bonding option failure.!!!\n"));
#endif
                return FALSE;
            }
        }
        while((u8Data & 0x01) == 0x01);

#ifdef MS_DEBUG
        printf("@ Set bonding option for MSB123xc \n");
#endif
    }

#ifdef MS_DEBUG
    if (!bRet) printf("%s %d Error\n", __func__, __LINE__);
#endif
    return bRet;
}

static MS_BOOL _IspCheckVer(MS_U8* pLibData, MS_BOOL* pMatch)
{
    MS_U8  bReadData[VERSION_CODE_SIZE];
    MS_U32  indx = 0;
    *pMatch = true;

    MS_U8 bWriteData[5] = {0x4D, 0x53, 0x54, 0x41, 0x52};
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, bWriteData);

    MS_U8    bAddr[1], bError = true;
    //MAPI_U16   Count;

    memset(bReadData, 0 , sizeof(bReadData));

    bAddr[0] = 0x10;
    //dwStartAddr=0;

    bWriteData[0] = 0x03;
    bWriteData[1] = VERSION_CODE_ADDR >> 16;
    bWriteData[2] = VERSION_CODE_ADDR >> 8;
    bWriteData[3] = VERSION_CODE_ADDR & 0xFF;

    bError &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 1, bAddr, 4, bWriteData);

    bAddr[0] = 0x11;
    bError &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_READ_BYTES, 1, bAddr, VERSION_CODE_SIZE, bReadData);

    bWriteData[0] = 0x12;
    bError &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1,  bWriteData);

    if(FALSE == bError)
    {
        bWriteData[0] = 0x24 ;
        (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);
        return FALSE;
    }

    bWriteData[0] = 0x24 ;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

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
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8MsbData);
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_READ_BYTES, 0, NULL, 1, &reg);

    u8MsbData[0] = 0x34;
    bRet=(*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);


    chksum = reg;

/// crc L byte
    u16Addr = 0x0c00+0x0d*2;

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;

    u8MsbData[0] = 0x35;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8MsbData);
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_READ_BYTES, 0, NULL, 1, &reg);

    u8MsbData[0] = 0x34;
    bRet=(*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);


    chksum = (chksum<<8)|reg;

// get mcu status

    u16Addr = 0x0900+0x4f*2;

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = 0x00;
    u8MsbData[3] = (u16Addr >> 8) &0xff;
    u8MsbData[4] = u16Addr &0xff;

    u8MsbData[0] = 0x35;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8MsbData);
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_READ_BYTES, 0, NULL, 1, &reg);

    u8MsbData[0] = 0x34;
    bRet=(*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);


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

    MS_U8 bWriteData[5]= {0x4D, 0x53, 0x54, 0x41, 0x52};
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, bWriteData);

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
            (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 1, bAddr, 1, bWriteData);

            bAddr[0] = 0x11;
            (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_READ_BYTES, 1, bAddr, 1, &bReadData);

            bWriteData[0] = 0x12;
            (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

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
            bError &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 1, bAddr, 1, bWriteData);

            bWriteData[0] = 0x12;
            bError &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

            // Page Program
            bAddr[0] = 0x10;
            bAddr[1] = 0x02;
            bAddr[2] = dwLoop >> 16;
            bAddr[3] = dwLoop >> 8;
            bAddr[4] = dwLoop;

            bError &= (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 5, bAddr, PAGE_WRITE_SIZE, (pLibArry+dwLoop));

            bWriteData[0] = 0x12;
            bError &=  (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

            bAddr[0] = 0x10;
            bWriteData[0] = 0x04;
            bError &=  (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 1, bAddr, 1, bWriteData);

            bWriteData[0] = 0x12;
            bError &=  (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

            if(bError == FALSE)
            {
                break;
            }
        }
    }

    bWriteData[0]=0x24 ;
    (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

    if(bError==FALSE)
        return TRUE;
    else
        return FALSE;

}

void MDrv_DMD_MSB123xc_SetCurrentDemodulatorType(eDMD_MSB123xc_DemodulatorType eCurrentDemodulatorType)
{
#ifdef MS_DEBUG
    printf("MDrv_DMD_MSB123xc_SetCurrentDemodulatorType %d\n", eCurrentDemodulatorType);
#endif
    eDMD_MSB123xc_CurrentDemodulatorType = eCurrentDemodulatorType;
}

MS_BOOL MDrv_DMD_MSB123xc_LoadDSPCode(void)
{
    MS_BOOL bRet = TRUE;
    DMD_LOCK();
    bRet = _LoadDSPCode();
    DMD_UNLOCK();
    return bRet;
}
MS_BOOL MDrv_DMD_MSB123xc_DTV_DVBT_DSPReg_CRC(void)
{
    MS_BOOL bRet = TRUE;
    DMD_LOCK();
    bRet = _DTV_DVBT_DSPReg_CRC();
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_MSB123xc_Power_On_Initialization(void)
{
    MS_U8     status = TRUE;
    MS_BOOL   bMatch = false;
    MS_U8     u8RetryCnt = 6;
#ifdef MS_DEBUG
    MS_U32 u32tmm_1, u32tmm_2, u32tmm_3, u32tmm_4, u32tmm_5, u32tmm_6 = 0x00;
#endif

    DMD_LOCK();
#ifdef MS_DEBUG
    u32tmm_1 = MsOS_GetSystemTime();
#endif

    if (_sDMD_MSB123xc_InitData.bPreloadDSPCodeFromMainCHIPI2C)
    {
        DMD_UNLOCK();
        return TRUE;
    }

#ifdef MS_DEBUG
    if (eDMD_MSB123xc_DbgLevel >= E_DMD_MSB123xc_DBGLV_DEBUG)
    {
        printf(">>>MSB123xc: Enter Power_On_Initialization()\n");
    }
#endif

    if ( _msbMSB123xc_set_bonding_option( MDrv_SYS_GetChipID() ) == FALSE )
    {
        DMD_UNLOCK();
        return TRUE;
    }

    if (bDMD_MSB123xc_Power_init_en == TRUE)
    {
        DMD_UNLOCK();
        return  TRUE;
    }
    else
    {
        bDMD_MSB123xc_Power_init_en = (u8DMD_MSB123xc_PowerOnInitialization_Flow == 2) ? (TRUE) : (FALSE);
    }

    if(_sDMD_MSB123xc_InitData.u8WO_SPI_Flash== 1)
    {
        if (_sDMD_MSB123xc_InitData.bPreloadDSPCodeFromMainCHIPI2C)
        {

        }
        else
        {
            //status &= _MSB123xc_I2C_CH_Reset(3);
            status &= _MSB123xc_HW_init();
            status &= _LoadDspCodeToSDRAM(MSB123xc_ALL);
            if (_LoadDSPCode() == FALSE)
            {
                printf(">>>>MSB123xc:Fail\n");
                status= FALSE;
            }
        }
    }
    else
    {
#ifdef MS_DEBUG
        if (eDMD_MSB123xc_DbgLevel >= E_DMD_MSB123xc_DBGLV_DEBUG)
        {
            printf("u8DMD_MSB123xc_PowerOnInitialization_Flow = %d\n", u8DMD_MSB123xc_PowerOnInitialization_Flow);
        }
#endif
        if(u8DMD_MSB123xc_PowerOnInitialization_Flow ==0)
        {
            status = _msb123xc_flash_mode_en();
            if (status == FALSE)
            {
                printf("[msb123xc][error]msb123xc_flash_mode_en fail....\n");
            }
            u8DMD_MSB123xc_PowerOnInitialization_Flow++;
        }
        else
        {
            if(u8DMD_MSB123xc_PowerOnInitialization_Flow<2)
            {
                u8DMD_MSB123xc_PowerOnInitialization_Flow++;
            }

            MS_U8     u8DoneFlag = 0;
            MS_U16    u16_counter = 0;

            MS_U16    crc16 = 0;

            crc16 = MSB123xc_LIB[sizeof(MSB123xc_LIB)-2];
            crc16 = (crc16<<8)|MSB123xc_LIB[sizeof(MSB123xc_LIB)-1];

            do
            {
                printf(">>>MSB123xc: u8RetryCnt = %d\n",u8RetryCnt);

                MS_U8 flash_waiting_ready_timeout = 0;
                u8RetryCnt--;

#ifdef MS_DEBUG
                u32tmm_3 = MsOS_GetSystemTime();
#endif

                status = _msb123xc_flash_boot_ready_waiting(&flash_waiting_ready_timeout);
                if ( (flash_waiting_ready_timeout == 1) || (status == FALSE) )
                {
                    printf("[msb123xc][error]msb123xc_flash_boot_ready_waiting fail....\n");
                }

#ifdef MS_DEBUG
                u32tmm_4 = MsOS_GetSystemTime();
                printf("[tmm1]t4-t3 = %ld (%ld - %ld)\n",u32tmm_4-u32tmm_3,u32tmm_4,u32tmm_3);
#endif

                if(status == FALSE)
                {
                    if (_sDMD_MSB123xc_InitData.fpGPIOReset != NULL)
                    {
#ifdef MS_DEBUG
                        printf(">>>MSB123xc: Reset Demodulator\n");
#endif
                        (*_sDMD_MSB123xc_InitData.fpGPIOReset)(FALSE); // gptr->SetOff();
                        MsOS_DelayTaskUs(resetDemodTime*1000);
                        (*_sDMD_MSB123xc_InitData.fpGPIOReset)(TRUE); // gptr->SetOn();
                        MsOS_DelayTaskUs(waitFlashTime * 1000);
                    }

                    if (_MSB123xc_I2C_CH_Reset(3) == FALSE)
                    {
                        printf(">>>MSB123xc CH Reset:Fail\n");
                        status= FALSE;
                        continue;
                    }

                    u16_counter = 1000;
                    do
                    {
                        // 10 ms
                        MsOS_DelayTaskUs(10*1000);
                        u16_counter--;
                        _MDrv_DMD_MSB123xc_GetReg(0x0900+(0x4f)*2, &u8DoneFlag);
                    }
                    while(u8DoneFlag != 0x99 && u16_counter != 0);

                    if(u16_counter == 0 && u8DoneFlag != 0x99)
                    {
                        printf("[wb]Err, MSB123xc didn't ready yet\n");
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
                    if (eDMD_MSB123xc_DbgLevel >= E_DMD_MSB123xc_DBGLV_DEBUG)
                    {
                        printf(">>>MSB123xc: Check Version...");
                    }
#endif

                    if (_IspCheckVer(MSB123xc_LIB, &bMatch) == FALSE)
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
                        if (_MSB123xc_I2C_CH_Reset(3) == FALSE)
                        {
                            printf(">>>MSB123xc CH Reset:Fail\n");
                            status= FALSE;
                            continue;
                        }
                        else
                        {
#ifdef MS_DEBUG
                            if (eDMD_MSB123xc_DbgLevel >= E_DMD_MSB123xc_DBGLV_DEBUG)
                            {
                                printf(">>>MSB123xc CH Reset:OK\n");
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

                    if (_sDMD_MSB123xc_InitData.bFlashWPEnable)
                    {
                        if (bMatch == FALSE)
                        {
                            // disable flash WP, pull high.
                            if(_msb123xc_flash_WP(0) == FALSE)
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
                        MS_U8 bWriteData[5]= {0x4D, 0x53, 0x54, 0x41, 0x52};

                        printf(">>> Not match! Reload Flash...");
                        if ( (sizeof(MSB123xc_LIB)%256) != 0)
                        {
                            printf(" MSB123xc_LIB 256byte alignment error!%u \n",sizeof(MSB123xc_LIB));
                        }

                        (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, bWriteData);

                        bAddr[0] = 0x10;
                        bWriteData[0] = 0x06;
                        (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 1, bAddr, 1, bWriteData);

                        bWriteData[0] = 0x12;
                        (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

                        bAddr[0] = 0x10;

                        bWriteData[0] = 0xC7;
                        (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 1, bAddr, 1, bWriteData);

                        bWriteData[0] = 0x12;
                        (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

                        bWriteData[0]=0x24 ;
                        (*_sDMD_MSB123xc_InitData.fpMSB123xc_I2C_Access)(E_DMD_MSB123xc_DEMOD_I2C_DYNAMIC_SLAVE_ID_2, E_DMD_MSB123xc_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, bWriteData);

#ifdef MS_DEBUG
                        printf("\t\t\tStart   %ld\n", MsOS_GetSystemTime());
#endif
                        if ( (sizeof(MSB123xc_LIB) - 2) > MAX_MSB123xc_LIB_LEN)
                        {
                            printf("Err, msb123xcc_lib size(%d) is larger than flash size(%d)\n", sizeof(MSB123xc_LIB), MAX_MSB123xc_LIB_LEN);
                        }

                        // if (IspProcFlash(MSB123xc_LIB, sizeof(MSB123xc_LIB)) == MAPI_FALSE)
                        if (_IspProcFlash(MSB123xc_LIB, sizeof(MSB123xc_LIB)-2) == FALSE)
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
                            if ((_IspCheckVer(MSB123xc_LIB, &bMatch) == FALSE)||(bMatch==false))
                            {
                                printf(">>> ISP read FAIL! bMatch %d \n",bMatch);
                                status= FALSE;
                                continue;
                            }
                            else // reset again
                            {
                                if (_sDMD_MSB123xc_InitData.fpGPIOReset != NULL)
                                {
#ifdef MS_DEBUG
                                    printf(">>>MSB123xc[2]: Reset Demodulator\n");
#endif
                                    (*_sDMD_MSB123xc_InitData.fpGPIOReset)(FALSE); // gptr->SetOff();
                                    MsOS_DelayTaskUs(resetDemodTime*1000);
                                    (*_sDMD_MSB123xc_InitData.fpGPIOReset)(TRUE); // gptr->SetOn();
                                    MsOS_DelayTaskUs(waitFlashTime * 1000);
                                }

                                if (_MSB123xc_I2C_CH_Reset(3) == FALSE)
                                {
                                    printf(">>>MSB123xc CH Reset:Fail\n");
                                    status= FALSE;
                                    continue;
                                }

                                u16_counter = 1000;
                                do
                                {
                                    // 10 ms
                                    MsOS_DelayTaskUs(10*1000);
                                    u16_counter--;
                                    _MDrv_DMD_MSB123xc_GetReg(0x0900+(0x4f)*2, &u8DoneFlag);
                                }
                                while(u8DoneFlag != 0x99 && u16_counter != 0);

                                if(u16_counter == 0 && u8DoneFlag != 0x99)
                                {
                                    printf("[wb]Err, MSB123xc didn't ready yet\n");
                                    status = FALSE;
                                }
                                else
                                    status = TRUE;

                                if (_MSB123xc_I2C_CH_Reset(3) == FALSE)
                                {
                                    printf(">>>MSB123xc CH Reset:Fail\n");
                                    status= FALSE;
                                    continue;
                                }
                                else
                                {
                                    printf(">>>MSB123xc CH Reset:OK\n");
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
                    {
                        // Version match, do nothing
#ifdef MS_DEBUG
                        printf(">>> Match\n");
#endif
                    } // if (bMatch == false)

                    if (_sDMD_MSB123xc_InitData.bFlashWPEnable)
                    {
                        if (bMatch == TRUE)
                        {
                            // Enable flash WP, pull high.
                            if(_msb123xc_flash_WP(1) == FALSE)
                            {
                                printf("[wb]Err, FLASH WP Enable Fail!!!\n");
                            }
                            MsOS_DelayTaskUs(100*1000);
                        }
                    }  // bFlashWPEnable
                }


                if (_MSB123xc_I2C_CH_Reset(3) == FALSE)
                {
                    printf(">>>MSB123xc CH Reset:Fail\n");
                    status= FALSE;
                    continue;
                }
                else
                {
#ifdef MS_DEBUG
                    printf(">>>MSB123xc CH Reset:OK\n");
#endif
                }

#ifdef MS_DEBUG
                u32tmm_5 = MsOS_GetSystemTime();
#endif

                if (_LoadDSPCode() == FALSE)
                {
                    printf(">>>>MSB123xc:Fail\n");
                    status= FALSE;
                    continue;
                }
                else
                {
#ifdef MS_DEBUG
                    printf(">>>MSB123xc:OK\n");
#endif
                }

#ifdef MS_DEBUG
                u32tmm_6 = MsOS_GetSystemTime();
                printf("[tmm1]t6-t5 = %ld (%ld - %ld)\n",u32tmm_6-u32tmm_5,u32tmm_6,u32tmm_5);
#endif

#ifdef MS_DEBUG
                {
                    MS_U8 u8ChipRevId = 0;
                    _MDrv_DMD_MSB123xc_GetReg(0x0900+(0x01)*2, &u8ChipRevId);
                    printf(">>>MSB123xc:Edinburgh RevID:%x\n", u8ChipRevId);


                    _MDrv_DMD_MSB123xc_GetReg(0x0900+(0x49)*2, &u8ChipRevId);
                    printf(">>>MSB123xc:Edinburgh 0x49_L:%x\n", u8ChipRevId);

                    _MDrv_DMD_MSB123xc_GetReg(0x0900+(0x49)*2+1, &u8ChipRevId);
                    printf(">>>MSB123xc:Edinburgh 0x49_H:%x\n", u8ChipRevId);

                    _MDrv_DMD_MSB123xc_GetReg(0x0900+(0x4A)*2, &u8ChipRevId);
                    printf(">>>MSB123xc:Edinburgh 0x4A_L:%x\n", u8ChipRevId);
                }
#endif
            }
            while((u8RetryCnt>0)&&(status==FALSE));
        }
    }

    if(status==FALSE)
    {
        printf("msb123xc power_on_init FAIL !!!!!! \n\n");
    }
    else
    {
#ifdef MS_DEBUG
        printf("msb123xc power_on_init OK !!!!!! \n\n");
        u32tmm_2 = MsOS_GetSystemTime();
        printf("[tmm]t2-t1 = %ld (%ld - %ld)\n",u32tmm_2-u32tmm_1,u32tmm_2,u32tmm_1);
#endif
    }

    DMD_UNLOCK();
    return status;
}


