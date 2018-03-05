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
#include <math.h>
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"

// Internal Definition
//#include "regCHIP.h"
//#include "regAVD.h"
//#include "mapi_tuner.h"
#include "drvSYS.h"
//#include "drvDMD_VD_MBX.h"
#include "drvDMD_EXTERN_MSB201X.h"
#include "include/drvDMD_common.h"
#include "include/drvSAR.h"
#include "drvMMIO.h"
#include "drvMSPI.h"
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

// BIT mask
#define _BIT0 0x01
#define _BIT1 0x02
#define _BIT2 0x04
#define _BIT3 0x08
#define _BIT4 0x10
#define _BIT5 0x20
#define _BIT6 0x40
#define _BIT7 0x80

// Demod bank number
#define TOP_REG_BASE  0x2000
#define TDP_REG_BASE  0x2100
#define FDP_REG_BASE  0x2200
#define FEC_REG_BASE  0x2300
#define TDF_REG_BASE  0x2700
#define TDFE_REG_BASE  0x2800
#define BACKEND_REG_BASE  0x2100
#define TDE_REG_BASE  0x2700
#define INNC_REG_BASE  0x2900
#define EQE_REG_BASE  0x2A00


MS_U8 MSB201X_DVBC_table[] = {    
#include "MSB201X_DVBC.dat"
};

float MSB201X_Spur_freq_table[255] = {
	48, 54, 56.5, 62, 64, 69, 72, 80.5, 81.5, 90,
	92, 96, 98, 103.5, 108, 115, 118, 124, 126.5, 136,
	138, 144, 152, 154, 161, 164, 170, 172.8, 180, 184,
	190, 192, 193, 198, 204, 207, 208, 216, 218, 226,
	230, 234, 236, 239, 240, 252, 253, 256, 262, 264,
	268, 276, 288, 299, 300, 308, 311, 312, 322, 324,
	330, 337, 340, 345, 345.6, 357, 360, 368, 380, 383,
	386, 391, 396, 409, 412, 429, 432, 437, 452, 458,
	460, 464, 468, 481, 484, 503, 506, 521.14, 524, 527,
	552, 556, 573, 575, 576, 596, 599, 602, 608, 612,
	625, 628, 644, 645, 648, 651, 668, 674, 680, 684,
	697, 703, 713, 714, 720, 723, 736, 740, 746, 749,
	752, 756, 759, 769, 772, 786, 789, 792, 795, 802,
	812, 818, 828, 841, 844, 846.85, 0
	};
#define MAX_SPUR_NUM	2
#define SRAM_Write_Buffer 128 // must < 255
#define SRAM_BASE          0x8000

#define LOAD_CODE_I2C_BLOCK_NUM          0x400//0x80

#define REG_MCU51_INTR		0x103400UL   //need to be check
#define REG_DMD0_MB_CNTL     0x103500UL
#define REG_DMD0_MB_ADDR_L   0x103501UL
#define REG_DMD0_MB_ADDR_H   0x103502UL
#define REG_DMD0_MB_DATA     0x103503UL
#define REG_DMD0_FSM_EN       0x103528UL
#define REG_DMD1_MB_CNTL     0x103600UL
#define REG_DMD1_MB_ADDR_L   0x103601UL
#define REG_DMD1_MB_ADDR_H   0x103602UL
#define REG_DMD1_MB_DATA     0x103603UL
#define REG_DMD1_FSM_EN       0x103628UL
/*  koln have only 2 demod
#define REG_DMD2_MB_CNTL     0x3300
#define REG_DMD2_MB_ADDR_L   0x3301
#define REG_DMD2_MB_ADDR_H   0x3302
#define REG_DMD2_MB_DATA     0x3303
#define REG_DMD2_FSM_EN       0x3328
#define REG_DMD3_MB_CNTL     0x3700
#define REG_DMD3_MB_ADDR_L   0x3701
#define REG_DMD3_MB_ADDR_H   0x3702
#define REG_DMD3_MB_DATA     0x3703
#define REG_DMD3_FSM_EN       0x3728
*/
#define resetDemodTime  50
#define waitFlashTime   50

#define SRAM_DATA_CHECK                0
#define SRAM_BASE                        0x8000
#define SPI_DEVICE_BUFFER_SIZE           256
#define MAX_MSB201X_LIB_LEN              131072

#define MSB201X_BOOT  0x01
#define MSB201X_DVBC  0x08
#define MSB201X_ALL   0x0F

#define MSB201X_BOOT_START_ADDR     0x00000
#define MSB201X_BOOT_END_ADDR       0x00FFF
#define MSB201X_DVBC_START_ADDR     0x18000
#define MSB201X_DVBC_END_ADDR       0x1FFFF

#define MSB201X_WINDOWS_BASE                0x100
#define MSB201X_BOOT_WINDOWS_OFFSET         MSB201X_WINDOWS_BASE
#define MSB201X_DVBC_WINDOWS_OFFSET        (MSB201X_WINDOWS_BASE + 0x18 + 0x08)

#define MSB201X_MAX_FLASH_ON_RETRY_NUM 3

#define UNUSED(x)       (x=x)

/////////////// CONSTANT /////////////////
#define PAGE_WRITE_SIZE         256
#define VERSION_CODE_ADDR       0xFC0
#define VERSION_CODE_SIZE       32

//static eDMD_MSB201X_DbgLv eDMD_MSB201X_DbgLevel=E_DMD_MSB201X_DBGLV_NONE;

//kirin bonding option
#define DRV_RIU_ReadByte(_u32addr)	(*(volatile MS_U32*)(_u32addr) )
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static MS_U8 g_dvbc_lock[DEMOD_MAX_INSTANCE][DEMOD_MAX_CHANNEL] = {{0,0}};

#if 0

#define DMD_LOCK() _Lock()
#define DMD_UNLOCK()  _UnLock()

#else
#define DMD_LOCK()      \
    do{                         \
        MS_ASSERT(MsOS_In_Interrupt() == FALSE); \
        MsOS_ObtainMutex(pDemod->s32_MSB201X_Mutex, MSOS_WAIT_FOREVER);\
        }while(0)

#define DMD_UNLOCK()      \
    do{                         \
        MsOS_ReleaseMutex(pDemod->s32_MSB201X_Mutex);\
        }while(0)

#endif
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if 1
/*
static MSIF_Version _drv_dmd_MSB201X_extern_version = {
    .MW = { DMD_MSB201X_EXTERN_VER, },
};
*/
#else
static MSIF_Version _drv_dmd_msb123x_extern_version;
#endif

#define DEMOD_GET_ACTIVE_NODE(x) (pstDemod+x) //&_gDemodNode[x]


//configure
//--------------------------------------------------------------------- for DVB-C
#define TUNER_IF 		5000

#define TS_SER_C        0x00    //0: parallel 1:serial

#if 1//test (INTERN_DVBC_TS_SERIAL_INVERSION)
#define TS_INV_C        0x01
#else
#define TS_INV_C        0x00
#endif

#define DVBC_FS         48000
#define CFG_ZIF         0x00    //For ZIF ,FC=0
#define FC_H_C          ( (DVBC_FS-TUNER_IF)>0 ? (((DVBC_FS-TUNER_IF)>>8)&0xFF) : (((TUNER_IF-DVBC_FS)>>8)&0xFF) )
#define FC_L_C          ( (DVBC_FS-TUNER_IF)>0 ? (((DVBC_FS-TUNER_IF))&0xFF) : (((TUNER_IF-DVBC_FS))&0xFF) )
#define FS_H_C          ((DVBC_FS>>8)&0xFF)         // FS
#define FS_L_C          (DVBC_FS&0xFF)
#define AUTO_SCAN_C     0x00    // Auto Scan - 0:channel change, 1:auto-scan
#define IQ_SWAP_C       0x01
#define PAL_I_C         0x00    // PAL_I: 0: Non-Pal-I CCI, 1: Pal-I CCI (for UK)
// Bxko 6875, 6900, 7000, 6125, 4000, 6950
// Symbol Rate: 6875 = 0x1ADB
// Symbol Rate: 6900 = 0x1AF4
// Symbol Rate: 7000 = 0x1B58
// Symbol Rate: 4000 = 0x0FA0
// Symbol Rate: 6125 = 0x17ED
#define SR0_H           0x1A
#define SR0_L           0xF4	//6900
#define SR1_H           0x1B
#define SR1_L           0x58	//7000
#define SR2_H           0x17
#define SR2_L           0xED	//6125
#define SR3_H           0x0F
#define SR3_L           0xA0	//4000
#define SR4_H           0x1B
#define SR4_L           0x26	//6950
#define SR5_H           0x1A
#define SR5_L           0xDB	//6875
#define SR6_H           0x1C
#define SR6_L           0x20	//7200
#define SR7_H           0x1C
#define SR7_L           0x52	//7250
#define SR8_H           0x0B
#define SR8_L           0xB8	//3000
#define SR9_H           0x03
#define SR9_L           0xE8	//1000
#define SR10_H          0x07
#define SR10_L          0xD0	//2000
#define SR11_H          0x00
#define SR11_L          0x00	//0000


#define QAM             0x04 // QAM: 0:16, 1:32, 2:64, 3:128, 4:256

// SAR dependent
#define NO_SIGNAL_TH_A  0xA3
// Tuner dependent
#define NO_SIGNAL_TH_B_L  0xFF //0x00 , Gain
#define NO_SIGNAL_TH_B_H  0xFF //0xDD
#define NO_SIGNAL_TH_C_L  0xff //0x64 , Err
#define NO_SIGNAL_TH_C_H  0xff //0x00
#define DAGC1_REF               0x70
#define DAGC2_REF               0x30
#define AGC_REF_L               0xFF
#define AGC_REF_H         0x05

#define INTERN_AUTO_SR_C  1
#define INTERN_AUTO_QAM_C 1

#define ATV_DET_EN        1

MS_U8 MSB201X_DVBC_DSPREG_TABLE[] =
{
	0x00, 0x01, INTERN_AUTO_SR_C, INTERN_AUTO_QAM_C, 0x00, 0x01, 0x00, 0x88, 0x13, SR0_L, SR0_H,
	QAM, IQ_SWAP_C, PAL_I_C, AGC_REF_L, AGC_REF_H, 0x00, NO_SIGNAL_TH_B_L, NO_SIGNAL_TH_B_H, NO_SIGNAL_TH_C_L, NO_SIGNAL_TH_C_H,
	0x00, 
};

typedef struct xMSB201X_Demod_Data
{
    MS_BOOL Active;
    MS_S32 s32_MSB201X_Mutex;
    MS_BOOL bDMD_MSB201X_Power_init_en;
    MS_U8 u8DMD_MSB201X_PowerOnInitialization_Flow;
    MS_U8 u8DMD_MSB201X_Sram_Code;
    sDMD_MSB201X_InitData _sDMD_MSB201X_InitData;
    eDMD_MSB201X_DemodulatorType eDMD_MSB201X_CurrentDemodulatorType;
    MS_BOOL bDemodRest;
    MS_U8 DVBC_DSP_REG[DEMOD_MAX_CHANNEL][sizeof(MSB201X_DVBC_DSPREG_TABLE)];
    sDMD_MSB201X_Info sDMD_MSB201X_Info[DEMOD_MAX_CHANNEL];
    MS_U32 u32DMD_DVBC_PrevScanTime[DEMOD_MAX_CHANNEL];
    MS_U32 u32DMD_DVBC_ScanCount[DEMOD_MAX_CHANNEL];
    MS_BOOL DMD_Lock_Status[DEMOD_MAX_CHANNEL];
    sDMD_MSB201X_IFAGC IFAGC_Data[DEMOD_MAX_CHANNEL];
    e_MSB201X_DSP_ReadWrite DSP_ReadWrite_Mode; 
    sDMD_MSB201X_TS_Param sDMD_MSB201X_TS_Param;
    MS_BOOL bDMD_MSB201X_TS_Param_Init_Done;
    sDMD_MSB201X_extHeader sDMD_MSB201X_extHeader_Param;
    sDMD_MSB201X_CIHeader sDMD_MSB201X_CIHeader_Param;
}tMSB201X_Demod_Data;

//static MS_S32 _gActiveInstanceIndex = 0;
//static eDMD_MSB201X_DbgLv eDMD_MSB201X_DbgLevel=E_DMD_MSB201X_DBGLV_NONE;
static tMSB201X_Demod_Data* pstDemod = NULL;
static MS_BOOL* pDemodRest = NULL;



static tMSB201X_Demod_Data MSB201X_Demod_Init=
{
	FALSE,
	-1,
  	FALSE, 
  	0, 
  	0, 
  	{0}, 
  	E_DMD_MSB201X_DEMOD_DVBC,
  	TRUE, 
  	{{0,0}},
	{{0,0}},
	{0},
	{0},
	{0}
};

// Timeout setting
static MS_U16 u16DMD_DVBC_AutoSymbol_Timeout = 10000, u16DMD_DVBC_FixSymbol_AutoQam_Timeout=2000, u16DMD_DVBC_FixSymbol_FixQam_Timeout=1000;

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
#define INDEX_TO_HANDLE(x) ((x)+1)
#define HANDLE_TO_INDEX(x) ((x)-1)
#define CFG_W_CMD 0x05
MS_BOOL _MDrv_DMD_MSB201X_SSPI_CFG_W(MS_U8 u8_addr,MS_U8 data) 
{
		MS_BOOL bRet = TRUE;
    MS_U8 Wdata[3];
    
    Wdata[0] = CFG_W_CMD;
    Wdata[1] = u8_addr;
    Wdata[2] = data;

    // Write operation
    MDrv_MSPI_SlaveEnable(TRUE);
    // send write address
    MDrv_MSPI_Write(Wdata,sizeof(Wdata));
    MDrv_MSPI_SlaveEnable(FALSE);

    return bRet;
}

static MS_BOOL _MSB201X_I2C_CH_Reset(MS_U8 devID, MS_U8 ch_num)
{
    MS_BOOL bRet=TRUE;
    //MAPI_U8         addr[4] = {0x00, 0x00, 0x00, 0x00};
    MS_U8         u8data[5] = {0x53, 0x45, 0x52, 0x44, 0x42};
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);

    #ifdef MS_DEBUG
    printf("[MSB201X][beg]I2C_CH_Reset, CH=0x%x\n",ch_num);
    #endif

    if(E_MSB201X_SPI_READ_WRITE == pDemod->DSP_ReadWrite_Mode)
    {
    	return TRUE;
    }
    
    //DMD_LOCK_REG_RW();

    // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
    //u8data[0] = 0x53;
    //bRet &= (*_sDMD_MSB201X_InitData.fpMSB201X_I2C_Access)(E_DMD_MSB201X_DEMOD_I2C_DYNAMIC_SLAVE_ID_1, E_DMD_MSB201X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8data);
    if (pDemod->bDemodRest)
    {
        pDemod->bDemodRest = FALSE;
        // 8'hb2(SRID)->8,h53(PWD1)->8,h45(PWD2)->8,h52(PWD3)->8,h44(PWD4)->8,h42(PWD5)
        u8data[0] = 0x53;
        // Don't check Ack because this passward only ack one time for the first time.
        bRet &= (pDemod->_sDMD_MSB201X_InitData.fpMSB201X_I2C_Access)(devID, E_DMD_MSB201X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8data);
    }
    // 8'hb2(SRID)->8,h71(CMD)  //TV.n_iic_
    u8data[0] = 0x71;
    bRet &= (pDemod->_sDMD_MSB201X_InitData.fpMSB201X_I2C_Access)(devID, E_DMD_MSB201X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8data);

    // 8'hb2(SRID)->8,h81(CMD)  //TV.n_iic_sel_b0
    u8data[0] = ((ch_num & 0x01) != 0)? 0x81 : 0x80;
    bRet &= (pDemod->_sDMD_MSB201X_InitData.fpMSB201X_I2C_Access)(devID, E_DMD_MSB201X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8data);

    // 8'hb2(SRID)->8,h83(CMD)  //TV.n_iic_sel_b1
    u8data[0] = ((ch_num & 0x02) != 0)? 0x83 : 0x82;
    bRet &= (pDemod->_sDMD_MSB201X_InitData.fpMSB201X_I2C_Access)(devID, E_DMD_MSB201X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8data);

    // 8'hb2(SRID)->8,h84(CMD)  //TV.n_iic_sel_b2
    u8data[0] = ((ch_num & 0x04) != 0)? 0x85 : 0x84;
    bRet &= (pDemod->_sDMD_MSB201X_InitData.fpMSB201X_I2C_Access)(devID, E_DMD_MSB201X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8data);

    // 8'hb2(SRID)->8,h53(CMD)  //TV.n_iic_ad_byte_en2, 32bit read/write
    u8data[0] = 0x53;
    bRet &= (pDemod->_sDMD_MSB201X_InitData.fpMSB201X_I2C_Access)(devID, E_DMD_MSB201X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8data);

    // 8'hb2(SRID)->8,h7f(CMD)  //TV.n_iic_sel_use_cfg
    u8data[0] = 0x7f;
    bRet &= (pDemod->_sDMD_MSB201X_InitData.fpMSB201X_I2C_Access)(devID, E_DMD_MSB201X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8data);

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
    DBG_DEMOD_LOAD_I2C(printf("[MSB201X][end]I2C_CH_Reset, CH=0x%x\n",ch_num));
    #endif
    return bRet;
}

static MS_BOOL _MDrv_DMD_MSB201X_GetReg(MS_U8 devID, MS_U32 u32Addr, MS_U8 *pu8Data)  //koln
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6];
    MS_U16 u16Addr=0;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);

    if(E_MSB201X_SPI_READ_WRITE == pDemod->DSP_ReadWrite_Mode)
    {
    	if((u32Addr&0x100000)==0x100000)
	{
		_MDrv_DMD_MSB201X_SSPI_CFG_W(0x02,0x10);   
	}
	else
	{
		_MDrv_DMD_MSB201X_SSPI_CFG_W(0x02,0x00);   
	}
	u16Addr=(MS_U16)u32Addr;
    	return MDrv_DMD_SSPI_RIU_Read8(u16Addr, pu8Data);
    }

    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = (u32Addr>>16)&0xff;
    u8MsbData[3] = (u32Addr >> 8) &0xff;
    u8MsbData[4] = u32Addr & 0xff;
    
    u8MsbData[0] = 0x35;
    (pDemod->_sDMD_MSB201X_InitData.fpMSB201X_I2C_Access)(devID, E_DMD_MSB201X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    (pDemod->_sDMD_MSB201X_InitData.fpMSB201X_I2C_Access)(devID, E_DMD_MSB201X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5, u8MsbData);
    bRet=(pDemod->_sDMD_MSB201X_InitData.fpMSB201X_I2C_Access)(devID, E_DMD_MSB201X_DEMOD_I2C_READ_BYTES, 0, NULL, 1, pu8Data);

    u8MsbData[0] = 0x34;

		(pDemod->_sDMD_MSB201X_InitData.fpMSB201X_I2C_Access)(devID, E_DMD_MSB201X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);
    #ifdef MS_DEBUG
    if (eDMD_MSB201X_DbgLevel >= E_DMD_MSB201X_DBGLV_DEBUG)
    {
        printf("_MDrv_DMD_MSB201X_GetReg %x %x\n", u16Addr, *pu8Data);
    }
    #endif

    return bRet;
}

MS_BOOL MDrv_DMD_MSB201X_GetReg(MS_U8 devID, MS_U32 u32Addr, MS_U8 *pu8Data)  //koln
{
    MS_BOOL bRet=TRUE;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);

    DMD_LOCK();
    bRet = _MDrv_DMD_MSB201X_GetReg(devID, u32Addr, pu8Data);
    DMD_UNLOCK();

    return bRet;
}

static MS_BOOL _MDrv_DMD_MSB201X_SetReg(MS_U8 devID, MS_U32 u32Addr, MS_U8 u8Data)  //koln
{
    MS_BOOL bRet=TRUE;
    MS_U8 u8MsbData[6];
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);
    MS_U16 u16Addr;

    if(E_MSB201X_SPI_READ_WRITE == pDemod->DSP_ReadWrite_Mode)
    {
	if((u32Addr&0x100000)==0x100000)
	{
		_MDrv_DMD_MSB201X_SSPI_CFG_W(0x02,0x10);   
	}
	else
	{
		_MDrv_DMD_MSB201X_SSPI_CFG_W(0x02,0x00);   
	}
    
    	u16Addr=(MS_U16)u32Addr;
    	return MDrv_DMD_SSPI_RIU_Write8(u16Addr, u8Data);
    }
    u8MsbData[0] = 0x10;
    u8MsbData[1] = 0x00;
    u8MsbData[2] = (u32Addr>>16)&0xff;
    u8MsbData[3] = (u32Addr >> 8) &0xff;
    u8MsbData[4] = u32Addr &0xff;
    u8MsbData[5] = u8Data;

    u8MsbData[0] = 0x35;
    (pDemod->_sDMD_MSB201X_InitData.fpMSB201X_I2C_Access)(devID, E_DMD_MSB201X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);

    u8MsbData[0] = 0x10;
    (pDemod->_sDMD_MSB201X_InitData.fpMSB201X_I2C_Access)(devID, E_DMD_MSB201X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 6, u8MsbData);

    u8MsbData[0] = 0x34;
    bRet=(pDemod->_sDMD_MSB201X_InitData.fpMSB201X_I2C_Access)(devID, E_DMD_MSB201X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbData);
    return bRet;
}

MS_BOOL MDrv_DMD_MSB201X_SetReg(MS_U8 devID, MS_U32 u32Addr, MS_U8 u8Data)  //koln
{
    MS_BOOL bRet=TRUE;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);

    DMD_LOCK();
    bRet = _MDrv_DMD_MSB201X_SetReg(devID, u32Addr, u8Data);
    DMD_UNLOCK();

    return bRet;
}

static MS_BOOL _MDrv_DMD_MSB201X_SetRegs(MS_U8 devID, MS_U32 u32Addr, MS_U8* u8pData, MS_U16 data_size)  //koln
{
    MS_BOOL bRet=TRUE;
    MS_U8   u8MsbDataValue[LOAD_CODE_I2C_BLOCK_NUM + 5];
    MS_U16   idx = 0;
    MS_U16   u16Addr=0;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);

    if(E_MSB201X_SPI_READ_WRITE == pDemod->DSP_ReadWrite_Mode)
    {
	if((u32Addr&0x100000)==0x100000)
	{
		_MDrv_DMD_MSB201X_SSPI_CFG_W(0x02,0x10);   
	}
	else
	{
		_MDrv_DMD_MSB201X_SSPI_CFG_W(0x02,0x00);   
	}
    	u16Addr=(MS_U16)u32Addr;
       for(idx = 0; idx < data_size; idx++)
       {
           MDrv_DMD_SSPI_RIU_Write8(u16Addr + idx, *(u8pData + idx));
       }
       return TRUE;
    }

    u8MsbDataValue[0] = 0x10;
    u8MsbDataValue[1] = 0x00;
    u8MsbDataValue[2] = (u32Addr>>16)& 0xff;
    u8MsbDataValue[3] = (u32Addr >> 8) &0xff;
    u8MsbDataValue[4] = u32Addr & 0xff;
    // u8MsbDataValue[5] = 0x00;

    for(idx = 0; idx < data_size ; idx++)
    {
        u8MsbDataValue[5+idx] = u8pData[idx];
    }

    u8MsbDataValue[0] = 0x35;
    bRet &= (pDemod->_sDMD_MSB201X_InitData.fpMSB201X_I2C_Access)(devID, E_DMD_MSB201X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbDataValue);

    u8MsbDataValue[0] = 0x10;
    bRet &= (pDemod->_sDMD_MSB201X_InitData.fpMSB201X_I2C_Access)(devID, E_DMD_MSB201X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 5 + data_size, u8MsbDataValue);

    u8MsbDataValue[0] = 0x34;
    bRet &= (pDemod->_sDMD_MSB201X_InitData.fpMSB201X_I2C_Access)(devID, E_DMD_MSB201X_DEMOD_I2C_WRITE_BYTES, 0, NULL, 1, u8MsbDataValue);

    return bRet;
}

MS_BOOL MDrv_DMD_MSB201X_SetRegs(MS_U8 devID, MS_U32 u32Addr, MS_U8* u8pData, MS_U16 data_size)  //Koln 
{
    MS_BOOL bRet=TRUE;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);

    DMD_LOCK();
    bRet = _MDrv_DMD_MSB201X_SetRegs(devID, u32Addr, u8pData, data_size);
    DMD_UNLOCK();

    return bRet;
}

static MS_BOOL _MDrv_DMD_MSB201X_SetReg2Bytes(MS_U8 devID, MS_U32 u32Addr, MS_U16 u16Data)  //koln
{
    MS_BOOL bRet=TRUE; 
    MS_U16 u16Addr=0;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);
    
    if(E_MSB201X_SPI_READ_WRITE == pDemod->DSP_ReadWrite_Mode)
    {

	if((u32Addr&0x100000)==0x100000)
	{
		_MDrv_DMD_MSB201X_SSPI_CFG_W(0x02,0x10);   
	}
	else
	{
		_MDrv_DMD_MSB201X_SSPI_CFG_W(0x02,0x00);   
	}

	u16Addr=(MS_U16)u32Addr;

	MDrv_DMD_SSPI_RIU_Write8(u16Addr, u16Data);
	return MDrv_DMD_SSPI_RIU_Write8(u16Addr+1, (u16Data>>8));
    }
    
    bRet &= _MDrv_DMD_MSB201X_SetReg(devID, u32Addr, (MS_U8)u16Data&0x00ff);
    bRet &= _MDrv_DMD_MSB201X_SetReg(devID, u32Addr + 0x0001, (MS_U8)(u16Data>>8)&0x00ff);

    return bRet;
}

MS_BOOL MDrv_DMD_MSB201X_SetReg2Bytes(MS_U8 devID, MS_U32 u32Addr, MS_U16 u16Data)   //koln
{
    MS_BOOL bRet=TRUE;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);	
    DMD_LOCK();


    
    bRet = _MDrv_DMD_MSB201X_SetReg2Bytes(devID, u32Addr, u16Data);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL _MDrv_DMD_MSB201X_GetDSPReg(MS_U8 devID, MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8     u8Cntl = 0x00;
    MS_U16    u16Cntr = 0x00;
    MS_U32    u32REG_MB_CNTL, u32REG_MB_ADDR_L, u32REG_MB_ADDR_H, u32REG_MB_DATA, u32REG_EXT_INTR;

    switch(u8DemodIndex)
    {
    	case 0:
			u32REG_MB_CNTL = REG_DMD0_MB_CNTL;
			u32REG_MB_ADDR_L = REG_DMD0_MB_ADDR_L;
			u32REG_MB_ADDR_H = REG_DMD0_MB_ADDR_H;
			u32REG_MB_DATA = REG_DMD0_MB_DATA;
			u32REG_EXT_INTR = (REG_MCU51_INTR + (0x41)*2+1);
			break;
	case 1:
			u32REG_MB_CNTL = REG_DMD1_MB_CNTL;
			u32REG_MB_ADDR_L = REG_DMD1_MB_ADDR_L;
			u32REG_MB_ADDR_H = REG_DMD1_MB_ADDR_H;
			u32REG_MB_DATA = REG_DMD1_MB_DATA;
			u32REG_EXT_INTR = (REG_MCU51_INTR + (0x51)*2+1);
			break;
/*			
	case 2:
			u16REG_MB_CNTL = REG_DMD2_MB_CNTL;
			u16REG_MB_ADDR_L = REG_DMD2_MB_ADDR_L;
			u16REG_MB_ADDR_H = REG_DMD2_MB_ADDR_H;
			u16REG_MB_DATA = REG_DMD2_MB_DATA;
			u16REG_EXT_INTR = (REG_MCU51_INTR + (0x60)*2);
			break;
	case 3:
			u16REG_MB_CNTL = REG_DMD3_MB_CNTL;
			u16REG_MB_ADDR_L = REG_DMD3_MB_ADDR_L;
			u16REG_MB_ADDR_H = REG_DMD3_MB_ADDR_H;
			u16REG_MB_DATA = REG_DMD3_MB_DATA;
			u16REG_EXT_INTR = (REG_MCU51_INTR + (0x70)*2);
			break;
*/
	default:
			u32REG_MB_CNTL = REG_DMD0_MB_CNTL;
			u32REG_MB_ADDR_L = REG_DMD0_MB_ADDR_L;
			u32REG_MB_ADDR_H = REG_DMD0_MB_ADDR_H;
			u32REG_MB_DATA = REG_DMD0_MB_DATA;
			u32REG_EXT_INTR = (REG_MCU51_INTR + (0x40)*2+1);
			break;
    }

    // Write into MB
    bRet &= _MDrv_DMD_MSB201X_SetReg(devID, u32REG_MB_ADDR_H, (MS_U8)(u16Addr >> 8));
    bRet &= _MDrv_DMD_MSB201X_SetReg(devID, u32REG_MB_ADDR_L, (MS_U8)(u16Addr));
    bRet &= _MDrv_DMD_MSB201X_SetReg(devID, u32REG_MB_CNTL, 0x03);
    // assert interrupt to DMD MCU51
    bRet &= _MDrv_DMD_MSB201X_GetReg(devID, u32REG_EXT_INTR , &u8Cntl);
    u8Cntl |= 0x02; //assert interrupt bit
    bRet &= _MDrv_DMD_MSB201X_SetReg(devID, u32REG_EXT_INTR , u8Cntl);

    do
    {
        bRet &= _MDrv_DMD_MSB201X_GetReg(devID, u32REG_MB_CNTL, &u8Cntl);
        if (u16Cntr++ > 0x7ff)
        {
            //#ifdef MS_DEBUG
            //if (eDMD_MSB201X_DbgLevel >= E_DMD_MSB201X_DBGLV_DEBUG)
            {
                printf("MSB201X_MB_READ_FAILURE\n");
            }
            //#endif
            return FALSE;
        }
    }
    while(u8Cntl != 0xff);

    bRet &= _MDrv_DMD_MSB201X_GetReg(devID, u32REG_MB_DATA, pu8Data);


    bRet &= _MDrv_DMD_MSB201X_GetReg(devID, u32REG_EXT_INTR , &u8Cntl);
    u8Cntl &= (~0x02); //deassert interrupt bit
    bRet &= _MDrv_DMD_MSB201X_SetReg(devID, u32REG_EXT_INTR , u8Cntl);    
    
    return bRet;
}

//1 means bypass en, 0 means bypass disable
MS_BOOL MDrv_DMD_MSB201X_I2C_BYPASS(MS_U8 devID,MS_U8 bypass_en)
{
		MS_BOOL bRet=TRUE;
		MS_U8 u8Data=0;
		tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);
		_MSB201X_I2C_CH_Reset(devID, 3);
		
		DMD_LOCK();
		bRet &= _MDrv_DMD_MSB201X_GetReg(devID, 0x100910, &u8Data);
		
		if(bypass_en==1)
		{
				u8Data|=0x10;
				bRet &= _MDrv_DMD_MSB201X_SetReg(devID, 0x100910, u8Data);
		}
		else
		{
				u8Data&=(~0x10);
			  bRet &= _MDrv_DMD_MSB201X_SetReg(devID, 0x100910, u8Data);
		}
		DMD_UNLOCK();
		
		return bRet;
}


MS_BOOL MDrv_DMD_MSB201X_GetDSPReg(MS_U8 devID, MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 *pu8Data)
{
    MS_BOOL bRet=TRUE;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);
    _MSB201X_I2C_CH_Reset(devID, 3);
    
    

    DMD_LOCK();
    bRet = _MDrv_DMD_MSB201X_GetDSPReg(devID, u8DemodIndex, u16Addr, pu8Data);
    DMD_UNLOCK();

    return bRet;
}

MS_BOOL _MDrv_DMD_MSB201X_SetDSPReg(MS_U8 devID, MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    MS_U8     u8Cntl = 0x00;
    MS_U16    u16Cntr = 0x00;
   
    MS_U32    u32REG_MB_CNTL, u32REG_MB_ADDR_L, u32REG_MB_ADDR_H, u32REG_MB_DATA, u32REG_EXT_INTR;

    switch(u8DemodIndex)
    {
    	case 0:
			u32REG_MB_CNTL = REG_DMD0_MB_CNTL;
			u32REG_MB_ADDR_L = REG_DMD0_MB_ADDR_L;
			u32REG_MB_ADDR_H = REG_DMD0_MB_ADDR_H;
			u32REG_MB_DATA = REG_DMD0_MB_DATA;
			u32REG_EXT_INTR = (REG_MCU51_INTR + (0x41)*2+1);
			break;
	case 1:
			u32REG_MB_CNTL = REG_DMD1_MB_CNTL;
			u32REG_MB_ADDR_L = REG_DMD1_MB_ADDR_L;
			u32REG_MB_ADDR_H = REG_DMD1_MB_ADDR_H;
			u32REG_MB_DATA = REG_DMD1_MB_DATA;
			u32REG_EXT_INTR = (REG_MCU51_INTR + (0x51)*2+1);
			break;
	/*		
	case 2:
			u16REG_MB_CNTL = REG_DMD2_MB_CNTL;
			u16REG_MB_ADDR_L = REG_DMD2_MB_ADDR_L;
			u16REG_MB_ADDR_H = REG_DMD2_MB_ADDR_H;
			u16REG_MB_DATA = REG_DMD2_MB_DATA;
			u16REG_EXT_INTR = (REG_MCU51_INTR + (0x60)*2);
			break;
	case 3:
			u16REG_MB_CNTL = REG_DMD3_MB_CNTL;
			u16REG_MB_ADDR_L = REG_DMD3_MB_ADDR_L;
			u16REG_MB_ADDR_H = REG_DMD3_MB_ADDR_H;
			u16REG_MB_DATA = REG_DMD3_MB_DATA;
			u16REG_EXT_INTR = (REG_MCU51_INTR + (0x70)*2);
			break;
	*/
	default:
			u32REG_MB_CNTL = REG_DMD0_MB_CNTL;
			u32REG_MB_ADDR_L = REG_DMD0_MB_ADDR_L;
			u32REG_MB_ADDR_H = REG_DMD0_MB_ADDR_H;
			u32REG_MB_DATA = REG_DMD0_MB_DATA;
			u32REG_EXT_INTR = (REG_MCU51_INTR + (0x41)*2+1);
			break;
    }

    _MSB201X_I2C_CH_Reset(devID, 3);
    // Write into MB
 //   pDemod->DSP_ReadWrite_Mode = E_MSB201X_SPI_READ_WRITE;
    bRet &= _MDrv_DMD_MSB201X_SetReg(devID, u32REG_MB_DATA, u8Data);
    
    bRet &= _MDrv_DMD_MSB201X_SetReg(devID, u32REG_MB_ADDR_H, (MS_U8)(u16Addr >> 8)); 

    bRet &= _MDrv_DMD_MSB201X_SetReg(devID, u32REG_MB_ADDR_L, (MS_U8)(u16Addr));

    bRet &= _MDrv_DMD_MSB201X_SetReg(devID, u32REG_MB_CNTL, 0x04);
    // assert interrupt to DMD MCU51
    bRet &= _MDrv_DMD_MSB201X_GetReg(devID, u32REG_EXT_INTR , &u8Cntl);
    u8Cntl |= 0x02; //assert interrupt bit
    bRet &= _MDrv_DMD_MSB201X_SetReg(devID, u32REG_EXT_INTR , u8Cntl);

    u8Cntl &= (~0x02); //deassert interrupt bit
    bRet &= _MDrv_DMD_MSB201X_SetReg(devID, u32REG_EXT_INTR , u8Cntl);    

    do
    {
        bRet &= _MDrv_DMD_MSB201X_GetReg(devID, u32REG_MB_CNTL, &u8Cntl);
        if (u16Cntr++ > 0x7ff)
        {
            //#ifdef MS_DEBUG
            //if (eDMD_MSB201X_DbgLevel >= E_DMD_MSB201X_DBGLV_DEBUG)
            {
                printf("MSB201X_MB_WRITE_FAILURE\n");
            }
            //#endif
            return false;
        }
    }
    while(u8Cntl != 0xff);
    


    
    return bRet;
}

MS_BOOL MDrv_DMD_MSB201X_SetDSPReg(MS_U8 devID, MS_U8 u8DemodIndex, MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_BOOL bRet=TRUE;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);
	
    DMD_LOCK();
    bRet = _MDrv_DMD_MSB201X_SetDSPReg(devID, u8DemodIndex, u16Addr, u8Data);
    DMD_UNLOCK();

    return bRet;
}

static MS_BOOL _MSB201X_Delay_Task(MS_U32 task_num)
{
	unsigned int n = 0;

	while(n < task_num)
	{
		n++;
	}
	return TRUE;
}

MS_BOOL _MDrv_DMD_MSB201X_Version(MS_U8 devID, MS_U8 u8DemodIndex, MS_U16 *ver)
{

    MS_U8 status = true;
    MS_U8 tmp = 0;
    MS_U16 u16_Demod_Version;

    _MSB201X_I2C_CH_Reset(devID, 5);
    status &= _MDrv_DMD_MSB201X_GetReg(devID, TOP_REG_BASE + 0xC1, &tmp);
    u16_Demod_Version = tmp;
    status &= _MDrv_DMD_MSB201X_GetReg(devID, TOP_REG_BASE + 0xC2, &tmp);
    u16_Demod_Version = u16_Demod_Version<<8|tmp;
    *ver = u16_Demod_Version;

    return status;
}

//new TS pad switching adding
MS_BOOL _MDrv_DMD_MSB201X_Set_TSOut(MS_U8 devID, sDMD_MSB201X_TS_Param *pDMD_MSB201X_TS_Param)
{
	MS_U8 u8Data = 0, u8Data_tmp = 0, idx = 0;

	tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);

	if(TRUE == pDemod->bDMD_MSB201X_TS_Param_Init_Done)
	{
		if(pDMD_MSB201X_TS_Param->eTSMode != pDemod->sDMD_MSB201X_TS_Param.eTSMode)
			pDemod->bDMD_MSB201X_TS_Param_Init_Done = FALSE;
	}
	
	_MSB201X_I2C_CH_Reset(devID, 3);

	// reg_swrst_ts_mux
	_MDrv_DMD_MSB201X_SetReg(devID, 0x100B00 + (0x70)*2, 0x00);


				// Set clock
				if(E_DMD_MSB201X_TS_CLK_MAX == pDMD_MSB201X_TS_Param->eTSClk)
			  {
                    _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2 + 1, 0x06);
        }
				else   //TS clock setting using AP parameter //later
			  {
			  			switch(pDMD_MSB201X_TS_Param->eTSClk)
			  			{
                case  E_DMD_MSB201X_TS_CLK_216MHz :                                 
                	_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2 + 1, 0x00); 
                break;                                                            
                case  E_DMD_MSB201X_TS_CLK_108MHz  :                               
                	_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2 + 1, 0x01); 
                break;                                                            
                case  E_DMD_MSB201X_TS_CLK_72MHz  :                               
                	_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2 + 1, 0x02); 
                break;                                                            
                case  E_DMD_MSB201X_TS_CLK_54MHz  :                               
                	_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2 + 1, 0x03); 
                break;                                                            
                case  E_DMD_MSB201X_TS_CLK_43p2MHz:                               
                	_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2 + 1, 0x04); 
                break;                                                            
                case  E_DMD_MSB201X_TS_CLK_36MHz  :                               
                	_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2 + 1, 0x05); 
                break;                                                            
                case  E_DMD_MSB201X_TS_CLK_30p8MHz:                               
                	_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2 + 1, 0x06); 
                break;                                                            
                case  E_DMD_MSB201X_TS_CLK_27MHz:                               
                	_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2 + 1, 0x07); 
                break;                                                            
                case  E_DMD_MSB201X_TS_CLK_24MHz  :                               
                	_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2 + 1, 0x08); 
                break;                                                            
                case  E_DMD_MSB201X_TS_CLK_21p6MHz:                               
                	_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2 + 1, 0x09); 
                break;                                                            
                case  E_DMD_MSB201X_TS_CLK_19p6MHz :                               
                	_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2 + 1, 0x0a); 
                break;                                                            
                case  E_DMD_MSB201X_TS_CLK_18MHz   :                               
                	_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2 + 1, 0x0b); 
                break;                                                            
                case  E_DMD_MSB201X_TS_CLK_16p6MHz :                               
                	_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2 + 1, 0x0c); 
                break;                                                            
                case  E_DMD_MSB201X_TS_CLK_15p4MHz :                               
                	_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2 + 1, 0x0d); 
                break;                                                            
                case  E_DMD_MSB201X_TS_CLK_14p4MHz :                               
                	_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2 + 1, 0x0e); 
                break;                                                            
                case  E_DMD_MSB201X_TS_CLK_13p5MHz :                               
                	_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2 + 1, 0x0f); 
                break;                   
								default:   
								_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2 + 1, 0x06);
								break;
			  			}
			  						
                    //_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2 + 1, 0x06);
        }


  //need to clear this to output TS normally               
  _MDrv_DMD_MSB201X_SetReg(devID,0x103100+(0x02)*2+1,0x00);      
	
	if(FALSE == pDemod->bDMD_MSB201X_TS_Param_Init_Done)
	{
		memcpy(&pDemod->sDMD_MSB201X_TS_Param, pDMD_MSB201X_TS_Param, sizeof(sDMD_MSB201X_TS_Param));

		switch(pDMD_MSB201X_TS_Param->eTSMode)
		{
			case E_DMD_MSB201X_PARALLEL:
				

				// Out mode : 1 => Normal mode, 0 => Gating mode.
				_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2, 0x10);
				//_MDrv_DMD_MSR1742_GetReg(devID, 0x0900 + (0x12)*2, &u8Data);
	 			//u8Data |= _BIT4;
        			//_MDrv_DMD_MSR1742_SetReg(devID, 0x0900 + (0x12)*2, u8Data);	

				// Clock inverse
				_MDrv_DMD_MSB201X_GetReg(devID, 0x100900 + (0x12)*2, &u8Data);
				if(TRUE == pDMD_MSB201X_TS_Param->bCLKInverse)
				{
					u8Data |= _BIT5;
				}
				else
				{
					u8Data &= (~_BIT5);
				}
        			_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2, u8Data);	

				// reg_ckg_ts_mux
				_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x1e) * 2, 0x4444);

				// reg_ckg_ts_*
				_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x23) * 2, 0x0000);
				
				// all pad in
				_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x28) * 2, 0x0000);

				// Enable TS
				_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x2e) * 2, 0x0103);

				// Disable SSPI
				_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x3b) * 2, 0x0000);



				_MDrv_DMD_MSB201X_SetReg(devID, 0x103100 + (0x00)*2 + 1, 0x00);
				_MDrv_DMD_MSB201X_SetReg(devID, 0x103100 + (0x00)*2, 0xff);

				// reg_client_mask
				_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x103100 + (0x06) * 2, 0x0020);

				// reg_ts_mux_swrst
				u8Data = 0xf0;
				for(idx = 0; idx<DEMOD_MAX_CHANNEL; idx++)
				{
					if(FALSE == pDMD_MSB201X_TS_Param->bEnable[idx])
					{
						u8Data_tmp = 0x01;
						u8Data |= (u8Data_tmp << idx);
					}
					else
					{
						u8Data_tmp = 0x01;
						u8Data &= (~(u8Data_tmp << idx));
					}
				}
				_MDrv_DMD_MSB201X_SetReg(devID, 0x103100 + (0x00)*2, u8Data);

				break;
			case E_DMD_MSB201X_3_WIRE_REMUX2TS0:
								
				break;
			case E_DMD_MSB201X_3_WIRE_REMUX2TS1:
				
				break;
			case E_DMD_MSB201X_3_WIRE_REMUX2TS2:
				
				break;
			case E_DMD_MSB201X_3_WIRE_REMUX2TS3:
				
				break;
			case E_DMD_MSB201X_3_WIRE_REMUX2TS4:
				
				break;
			case E_DMD_MSB201X_3_WIRE_REMUX2TS5:
				
				break;
			case E_DMD_MSB201X_3_WIRE_REMUX2TS6:
				
				break;
			case E_DMD_MSB201X_3_WIRE_REMUX2TS7:
				
				break;
			case E_DMD_MSB201X_3_WIRE_DMD0_TS0_DMD1_TS1:
				// Set clock
				//_MDrv_DMD_MSB201X_SetReg(devID, 0x103100 + (0x12)*2 + 1, 0x02);

				// Out mode : 1 => Normal mode, 0 => Gating mode.
				_MDrv_DMD_MSB201X_SetReg(devID, 0x103100 + (0x12)*2, 0x00);
				//_MDrv_DMD_MSR1742_GetReg(devID, 0x0900 + (0x12)*2, &u8Data);
	 			//u8Data &= (~_BIT4);
        			//_MDrv_DMD_MSR1742_SetReg(devID, 0x0900 + (0x12)*2, u8Data);	

				// Clock inverse
				_MDrv_DMD_MSB201X_GetReg(devID, 0x100900 + (0x12)*2, &u8Data);
				if(TRUE == pDMD_MSB201X_TS_Param->bCLKInverse)
				{
					u8Data |= _BIT5;
				}
				else
				{
					u8Data &= (~_BIT5);
				}
        			_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2, u8Data);

				// reg_ckg_ts_mux
				_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x1e) * 2, 0x4444);

				// reg_ckg_ts_*
				_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x23) * 2, 0x0000);
				
				// all pad in
				_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x28) * 2, 0x0000);

				// Enable TS
				//wait for modified(to achieve pad compatible)
				_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x29) * 2, 0x0000);
				_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x2a) * 2, 0x0000);
				_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x2d) * 2, 0x0003);
				_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x2e) * 2, 0x0000);
				
				// Disable SSPI
				_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x3b) * 2, 0x0000);

				// reg_ckg_ts_*
				_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x23) * 2, 0x8888);

				// reg_swrst_ts_mux
				_MDrv_DMD_MSB201X_SetReg(devID, 0x100B00 + (0x70)*2, 0x00);

				_MDrv_DMD_MSB201X_SetReg(devID, 0x103100 + (0x00)*2 + 1, 0x00);
				_MDrv_DMD_MSB201X_SetReg(devID, 0x103100 + (0x00)*2, 0xff);

				// reg_serial_order
				u8Data = 0x00;
				for(idx = 0; idx<DEMOD_MAX_CHANNEL; idx++)
				{
					if(TRUE == pDMD_MSB201X_TS_Param->bEnable[idx])
					{
						u8Data_tmp = 0x01;
						u8Data |= (u8Data_tmp << (idx+1));
					}
					else
					{
						u8Data_tmp = 0x01;
						u8Data &= (~(u8Data_tmp << (idx+1)));
					}
				}
				_MDrv_DMD_MSB201X_SetReg(devID, 0x103100 + (0x01)*2, u8Data);

				// reg_ts_mux_swrst
				u8Data = 0x0f;
				for(idx = 0; idx<DEMOD_MAX_CHANNEL; idx++)
				{
					if(FALSE == pDMD_MSB201X_TS_Param->bEnable[idx])
					{
						u8Data_tmp = 0x01;
						u8Data |= (u8Data_tmp << (idx+4));
					}
					else
					{
						u8Data_tmp = 0x01;
						u8Data &= (~(u8Data_tmp << (idx+4)));
					}
				}
				_MDrv_DMD_MSB201X_SetReg(devID, 0x103100 + (0x00)*2, u8Data);
				
				break;
			case E_DMD_MSB201X_3_WIRE_DMD0_TS1_DMD1_TS0:
				
				break;
			case E_DMD_MSB201X_4_WIRE_REMUX2TS0:
				
				break;
			case E_DMD_MSB201X_4_WIRE_REMUX2TS1:
				
				break;
			case E_DMD_MSB201X_4_WIRE_REMUX2TS2:
				
				break;
			case E_DMD_MSB201X_4_WIRE_REMUX2TS3:
				
				break;
			case E_DMD_MSB201X_4_WIRE_REMUX2TS4:
				
				break;
			case E_DMD_MSB201X_4_WIRE_REMUX2TS5:
				
				break;
			case E_DMD_MSB201X_4_WIRE_DMD0_TS0_DMD1_TS1:
				
				break;
			case E_DMD_MSB201X_4_WIRE_DMD0_TS1_DMD1_TS0:
				
				break;
			default :
				
				break;
		}

		pDemod->bDMD_MSB201X_TS_Param_Init_Done = TRUE;
		
	}

	// Check reg_ts_mux_swrst
	_MDrv_DMD_MSB201X_GetReg(devID, 0x100B00 + (0x70)*2, &u8Data);
	if((u8Data & 0x01) == 0x01)
	{
		_MDrv_DMD_MSB201X_SetReg(devID, 0x100B00 + (0x70)*2, 0x00);
	}
	
	
	switch(pDemod->sDMD_MSB201X_TS_Param.eTSDrv)
	{
		case E_DMD_MSB201X_TS_DRVING_LO:
				//set to high driving
				_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x2C)*2, 0x00);	
				_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x2C)*2+1, 0x00);			
			break;
		case E_DMD_MSB201X_TS_DRVING_HI:
				//set to high driving
				_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x2C)*2, 0xff);	
				_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x2C)*2+1, 0xff);			
			break;
		case E_DMD_MSB201X_TS_DRVING_MAX:
				//set to high driving
				_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x2C)*2, 0xff);	
				_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x2C)*2+1, 0xff);						
			break;
	}
	


	if(TRUE == pDemod->bDMD_MSB201X_TS_Param_Init_Done)
	{
		//if(pDMD_MSR1742_InitData->bCLKInverse != pDemod->sDMD_MSR1742_TS_Param.bCLKInverse)

	}

	return TRUE;
}

MS_BOOL _MDrv_DMD_MSB201X_CfgExtHeader(MS_U8 devID, MS_U8 u8DemodIndex,  sDMD_MSB201X_extHeader *pDMD_MSB201X_extHeader_Param)
{
	MS_U8 u8Data_h = 0, u8Data_l = 0, u8Data = 0, idx = 0;
	MS_U16 u16Data = 0;
	MS_U32 u32HeaderReg = 0;
	tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);

	_MSB201X_I2C_CH_Reset(devID, 3);
	
	// Check reg_ts_mux_swrst
	_MDrv_DMD_MSB201X_GetReg(devID, 0x100B00 + (0x70)*2, &u8Data);
	if((u8Data & 0x01) == 0x01)
	{
		_MDrv_DMD_MSB201X_SetReg(devID, 0x100B00 + (0x70)*2, 0x00);
	}
	
	// Keep original swrst settings
	_MDrv_DMD_MSB201X_GetReg(devID, 0x103100 + (0x00)*2 + 1, &u8Data_h);
	u16Data = (u8Data_h & 0x0f);
	_MDrv_DMD_MSB201X_GetReg(devID, 0x103100 + (0x00)*2, &u8Data_l);
	u16Data = (u16Data << 8) | u8Data_l;
	
	
	

	#ifdef MS_DEBUG
		printf("===_MDrv_DMD_MSR1742_CfgExtHeader=== Original swrst = 0x%x\n", u16Data);
	#endif
		
	if(FALSE == pDMD_MSB201X_extHeader_Param->bEnable)
	{
		// Disable ExtHeader
		// Clear bit 10 and 11
		u16Data &= 0x03ff;

		// Restart TS
		_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x103100 + (0x00) * 2, 0x0fff);
		MsOS_DelayTask(1);
		_MDrv_DMD_MSB201X_SetReg(devID, 0x103100 + (0x00)*2 + 1, ((u16Data & 0x0f00)>>8));
		_MDrv_DMD_MSB201X_SetReg(devID, 0x103100 + (0x00)*2, (u16Data & 0x00ff));

		#ifdef MS_DEBUG
			printf("===Enable FALSE===\n");
			printf("===_MDrv_DMD_MSB201X_CfgExtHeader=== swrst = 0x%x\n", u16Data);
		#endif
		
		return TRUE;
	}
	else
	{		
		// Set extend header length
		//if(pDemod->sDMD_MSR1742_extHeader_Param.u8HeaderSize != pDMD_MSR1742_extHeader_Param->u8HeaderSize)
		{
			u8Data = pDMD_MSB201X_extHeader_Param->u8HeaderSize;
			_MDrv_DMD_MSB201X_SetReg(devID, 0x103100 + (0x07)*2 + 1, ((u8Data - 1) & 0x0f));

			#ifdef MS_DEBUG
				printf("===_MDrv_DMD_MSB201X_CfgExtHeader=== header size = %d\n", (int)u8Data);
				_MDrv_DMD_MSB201X_GetReg(devID, 0x103100 + (0x07)*2 + 1, &u8Data);
				printf("===Enable TRUE===\n");
				printf("===_MDrv_DMD_MSB201X_CfgExtHeader=== read header size = %d\n", (int)u8Data);
			#endif
		}
		/* Remove header size checking
		else
		{
			#if(HEADER_DBG)
				_MDrv_DMD_MSR1742_GetReg(devID, 0x2A00 + (0x07)*2 + 1, &u8Data);
				printf("===_MDrv_DMD_MSR1742_CfgExtHeader=== read header size = %d\n", (int)u8Data);
				printf("===Enable TRUE===\n");
				printf("===_MDrv_DMD_MSR1742_CfgExtHeader=== header size doesn't change! \n");
			#endif
		}
		*/
		
		// Set extend header list
		switch(u8DemodIndex)
		{
			case DEMOD0:
				u32HeaderReg = 0x103100 + (0x08)*2;
				break;
			case DEMOD1:
				u32HeaderReg = 0x103100 + (0x10)*2;
				break;
				/*
			case DEMOD2:
				u32HeaderReg = 0x2A00 + (0x18)*2;
				break;
			case DEMOD3:
				u32HeaderReg = 0x2A00 + (0x20)*2;
				break;
				*/
			default :
				u32HeaderReg = 0x103100 + (0x08)*2;
				break;
		}
		for(idx = 0; idx < pDMD_MSB201X_extHeader_Param->u8HeaderSize; idx++)
		{
			_MDrv_DMD_MSB201X_SetReg(devID, u32HeaderReg + idx, *(pDMD_MSB201X_extHeader_Param->pHeaderPtr + idx));


		}
	
		// Enable ExtHeader
		// Clear bit 10 and 11, then set bit 10 to add extend header
		u16Data &= 0x03ff;
		u16Data |= 0x0400;

		// Restart TS
		_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x103100 + (0x00) * 2, 0x0fff);
		MsOS_DelayTask(1);
		_MDrv_DMD_MSB201X_SetReg(devID, 0x103100 + (0x00)*2 + 1, ((u16Data & 0x0f00)>>8));
		_MDrv_DMD_MSB201X_SetReg(devID, 0x103100 + (0x00)*2, (u16Data & 0x00ff));

		#ifdef MS_DEBUG
			printf("===_MDrv_DMD_MSR1742_CfgExtHeader=== swrst = 0x%x \n", u16Data);
			printf("===Done===\n");
		#endif

		// Save setting parameters for next time comparison
		memcpy(&pDemod->sDMD_MSB201X_extHeader_Param, pDMD_MSB201X_extHeader_Param, sizeof(sDMD_MSB201X_extHeader));
	}

	return TRUE;
}

MS_BOOL _MDrv_DMD_MSB201X_CfgCIHeader(MS_U8 devID, MS_U8 u8DemodIndex,  sDMD_MSB201X_CIHeader *pDMD_MSB201X_CIHeader_Param)
{
	MS_U8 u8Data_h = 0, u8Data_l = 0, u8Data = 0;
	MS_U16 u16Data = 0;
	MS_U32 u32HeaderReg = 0;
	tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);

	_MSB201X_I2C_CH_Reset(devID, 3);
	
	// Check reg_ts_mux_swrst
	_MDrv_DMD_MSB201X_GetReg(devID, 0x100B00 + (0x70)*2, &u8Data);
	if((u8Data & 0x01) == 0x01)
	{
		_MDrv_DMD_MSB201X_SetReg(devID, 0x100B00 + (0x70)*2, 0x00);
	}
	
	// Keep original swrst settings
	_MDrv_DMD_MSB201X_GetReg(devID, 0x103100 + (0x00)*2 + 1, &u8Data_h);
	u16Data = (u8Data_h & 0x0f);
	_MDrv_DMD_MSB201X_GetReg(devID, 0x103100 + (0x00)*2, &u8Data_l);
	u16Data = (u16Data << 8) | u8Data_l;
		
	if(FALSE == pDMD_MSB201X_CIHeader_Param->bEnable)
	{
		// Disable ExtHeader
		// Clear bit 10 and 11
		u16Data &= 0x03ff;

		// Restart TS
		_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x103100 + (0x00) * 2, 0x0fff);
		MsOS_DelayTask(1);
		_MDrv_DMD_MSB201X_SetReg(devID, 0x103100 + (0x00)*2 + 1, ((u16Data & 0x0f00)>>8));
		_MDrv_DMD_MSB201X_SetReg(devID, 0x103100 + (0x00)*2, (u16Data & 0x00ff));
		
		return TRUE;
	}
	else
	{			
		// Set extend header list
		switch(u8DemodIndex)
		{
			case DEMOD0:
				u32HeaderReg = 0x103100 + (0x28)*2;
				break;
			case DEMOD1:
				u32HeaderReg = 0x103100 + (0x28)*2 + 1;
				break;
			/*
			case DEMOD2:
				u32HeaderReg = 0x2A00 + (0x29)*2;
				break;
			case DEMOD3:
				u32HeaderReg = 0x2A00 + (0x29)*2 + 1;
				break;
		  */
			default :
				u32HeaderReg = 0x103100 + (0x28)*2;
				break;
		}
		if((*pDemod->sDMD_MSB201X_CIHeader_Param.pHeaderPtr) != *(pDMD_MSB201X_CIHeader_Param->pHeaderPtr))
		{
			_MDrv_DMD_MSB201X_SetReg(devID, u32HeaderReg, *(pDMD_MSB201X_CIHeader_Param->pHeaderPtr));
		}
	
		// Enable ExtHeader
		// Clear bit 10 and 11, then set bit 11 to enable CI header
		u16Data &= 0x03ff;
		u16Data |= 0x0800;

		// Restart TS
		_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x103100 + (0x00) * 2, 0x0fff);
		MsOS_DelayTask(1);
		_MDrv_DMD_MSB201X_SetReg(devID, 0x103100 + (0x00)*2 + 1, ((u16Data & 0x0f00)>>8));
		_MDrv_DMD_MSB201X_SetReg(devID, 0x103100 + (0x00)*2, (u16Data & 0x00ff));

		// Save setting parameters for next time comparison
		memcpy(&pDemod->sDMD_MSB201X_CIHeader_Param, pDMD_MSB201X_CIHeader_Param, sizeof(sDMD_MSB201X_CIHeader));
	}

	return TRUE;
}


MS_BOOL _MDrv_DMD_MSB201X_Active(MS_U8 devID, MS_U8 u8DemodIndex, MS_BOOL bEnable)
{
    MS_U8   status = true;
    MS_U32 banknum = 0;

    _MSB201X_I2C_CH_Reset(devID, 3);

    switch(u8DemodIndex)
    {
    	case 0:
			banknum = REG_DMD0_MB_CNTL;
			break;
	case 1:
			banknum = REG_DMD1_MB_CNTL;
			break;
	/*		
	case 2:
			banknum = REG_DMD2_MB_CNTL;
			break;
	case 3:
			banknum = REG_DMD3_MB_CNTL;
			break;
	*/
	default:
			banknum = REG_DMD0_MB_CNTL;
			break;
    }
	
    if(bEnable == TRUE)
    	_MDrv_DMD_MSB201X_SetReg(devID, banknum + (0x0e)*2, 0x01);
    //HAL_DMD_RIU_WriteByte(0x112600 + (0x0e)*2, 0x01);   // FSM_EN

    return status;
}


static MS_BOOL _MSB201X_Demod_LoadAll(MS_U8 devID, eDMD_MSB201X_Demod_Index eDemod_Index)
{
    MS_U16 i;
    unsigned short tmp;

    if(eDemod_Index != ALL_DEMOD)
    {
		return FALSE;
    }
		
    _MSB201X_I2C_CH_Reset(devID, 3);
    //MCU 0
    //Release dmd_mcu rst to wake up dmd51
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0x80,  0x01);        
    //Load code finish, eable i-side to sram
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0x81,  0x00);    
    //mux select for xdata setting from top riu to dmd riu    
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103700 + 0x1C,  0x01);
    //Release dmd_mcu rst to wake up dmd51
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0x86,  0x01);
    //MCU 1
    //Release dmd_mcu rst to wake up dmd51
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0xa6,  0x01);
    //Release dmd_mcu rst to wake up dmd51
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0xa0,  0x01);        
    //mux select for xdata setting from top riu to dmd riu
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103800 + 0x1C,  0x01);
    //Load code finish, eable i-side to sram
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0xa1,  0x00);
    
    //disable all pad in
    _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + 0x51,  0x00);  
    //DMD 51 interrupt by PM51 DMD0
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0x83,  0x02);  
    //DMD 51 interrupt by PM51 DMD1
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0xa2,  0x02); 
    //Enable program sram clock clk sram mcu
    _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + 0x17,  0x02);
    //default
    //_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + 0x16,  0x30);  //24Mhz
    _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + 0x16,  0x00);  //108Mhz
    
    //MCU0
    //release dmd mcu periphal rst
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0x86,  0x00);
    //set upper bound
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103700 + 0xe0,  0x3f);
    //set lower bound
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103700 + 0xe1,  0x20);
    //enable
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103700 + 0xe6,  0x11);
    
    //MCU1
    //release dmd mcu periphal rst
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0xa6,  0x00);
    //set upper bound
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103800 + 0xe0,  0x3f);
    //set lower bound
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103800 + 0xe1,  0x20);
    //enable
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103800 + 0xe6,  0x11);
    
    
    //select all demod
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103C00 + 0x28,  0x01);
    
     _MSB201X_I2C_CH_Reset(devID, 1);
    _MDrv_DMD_MSB201X_SetReg(devID, 0x000000 + 0x00,  0x00);
 

    ////  Load code thru VDMCU_IF ////
    #ifdef MS_DEBUG
    	printf(">Load Demod Code.....\n");
    #endif

    for ( i = 0; i < sizeof(MSB201X_DVBC_table);)
    {
            tmp = SRAM_BASE + i;

            if (i+SRAM_Write_Buffer-1<sizeof(MSB201X_DVBC_table))
            {
                MDrv_DMD_MSB201X_SetRegs(devID, tmp,MSB201X_DVBC_table+i,SRAM_Write_Buffer);

                i=i+SRAM_Write_Buffer-1;
            }
            else
            {
                MDrv_DMD_MSB201X_SetRegs(devID, tmp,MSB201X_DVBC_table+i,sizeof(MSB201X_DVBC_table)-i);

                i=sizeof(MSB201X_DVBC_table);
            }
    }

    _MSB201X_I2C_CH_Reset(devID, 3);
 
 		//mcu0
    //mux select for xdata setting from top riu to dmd riu
 		_MDrv_DMD_MSB201X_SetReg(devID, 0x103700 + 0x1C,  0x00);
 		//Load code finish, eable i-side to sram
 		_MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0x81,  0x01);
 		//Release dmd_mcu rst to wake up dmd51
 		_MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0x80,  0x00);
 		
 		//mcu1
 		//mux select for xdata setting from top riu to dmd riu
 		_MDrv_DMD_MSB201X_SetReg(devID, 0x103800 + 0x1C,  0x00);
 		//Load code finish, eable i-side to sram
 		_MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0xa1,  0x01);
 		//Release dmd_mcu rst to wake up dmd51
 		_MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0xa0,  0x00);
 		
 		
 		_MSB201X_I2C_CH_Reset(devID, 5);
 		

    #ifdef MS_DEBUG
    	printf(">Demod Loadcode done.");
    #endif
	
    return TRUE;
}

static MS_BOOL _MSB201X_Demod_LoadSingle(MS_U8 devID, MS_U8 u8DemodIndex)
{
    MS_U16 i;
    unsigned short tmp;

    if(u8DemodIndex >= ALL_DEMOD)
    {
			return FALSE;
    }

	
		if(u8DemodIndex==DEMOD0)
		{
			//MCU 0
	    //Release dmd_mcu rst to wake up dmd51
	    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0x80,  0x01);        
	    //Load code finish, eable i-side to sram
	    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0x81,  0x00);    
	    //mux select for xdata setting from top riu to dmd riu    
	    _MDrv_DMD_MSB201X_SetReg(devID, 0x103700 + 0x1C,  0x01);
	    //Release dmd_mcu rst to wake up dmd51
	    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0x86,  0x01);
		}
		
		if(u8DemodIndex==DEMOD1)
		{
	    //MCU 1
	    //Release dmd_mcu rst to wake up dmd51
	    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0xa6,  0x01);
	    //Release dmd_mcu rst to wake up dmd51
	    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0xa0,  0x01);        
	    //mux select for xdata setting from top riu to dmd riu
	    _MDrv_DMD_MSB201X_SetReg(devID, 0x103800 + 0x1C,  0x01);
	    //Load code finish, eable i-side to sram
	    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0xa1,  0x00);			
		}
		
    //disable all pad in
    _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + 0x51,  0x00);  
    //DMD 51 interrupt by PM51 DMD0
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0x83,  0x02);  
    //DMD 51 interrupt by PM51 DMD1
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0xa2,  0x02); 
    //Enable program sram clock clk sram mcu
    _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + 0x17,  0x02);
    //default
    //_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + 0x16,  0x30);		  //24MHz
		
		_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + 0x16,  0x00);		  //108MHz

		if(u8DemodIndex==DEMOD0)
		{
	    //MCU0
	    //release dmd mcu periphal rst
	    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0x86,  0x00);
	    //set upper bound
	    _MDrv_DMD_MSB201X_SetReg(devID, 0x103700 + 0xe0,  0x3f);
	    //set lower bound
	    _MDrv_DMD_MSB201X_SetReg(devID, 0x103700 + 0xe1,  0x20);
	    //enable
	    _MDrv_DMD_MSB201X_SetReg(devID, 0x103700 + 0xe6,  0x11);		
	    //DMD SINGLE DEMOD0
	    _MDrv_DMD_MSB201X_SetReg(devID, 0x103C00 + 0x28,  0x00);
	    _MDrv_DMD_MSB201X_SetReg(devID, 0x103C00 + 0x24,  0x00);			
		}
		
		if(u8DemodIndex==DEMOD1)
		{
	    //MCU1
	    //release dmd mcu periphal rst
	    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0xa6,  0x00);
	    //set upper bound
	    _MDrv_DMD_MSB201X_SetReg(devID, 0x103800 + 0xe0,  0x3f);
	    //set lower bound
	    _MDrv_DMD_MSB201X_SetReg(devID, 0x103800 + 0xe1,  0x20);
	    //enable
	    _MDrv_DMD_MSB201X_SetReg(devID, 0x103800 + 0xe6,  0x11);		
	    //DMD SINGLE DEMOD1	
	    _MDrv_DMD_MSB201X_SetReg(devID, 0x103C00 + 0x28,  0x00);
	    _MDrv_DMD_MSB201X_SetReg(devID, 0x103C00 + 0x24,  0x01);		
		}

    _MSB201X_I2C_CH_Reset(devID, 1);
    _MDrv_DMD_MSB201X_SetReg(devID, 0x0000 + 0x00,  0x00);        // reset VD_MCU  
    
    ////  Load code thru VDMCU_IF ////
    #ifdef MS_DEBUG
    	printf(">Load Demod Code.....\n");
    #endif

    for ( i = 0; i < sizeof(MSB201X_DVBC_table);)
    {
            tmp = SRAM_BASE + i;

            if (i+SRAM_Write_Buffer-1<sizeof(MSB201X_DVBC_table))
            {
                MDrv_DMD_MSB201X_SetRegs(devID, tmp,MSB201X_DVBC_table+i,SRAM_Write_Buffer);

                i=i+SRAM_Write_Buffer-1;
            }
            else
            {
                MDrv_DMD_MSB201X_SetRegs(devID, tmp,MSB201X_DVBC_table+i,sizeof(MSB201X_DVBC_table)-i);

                i=sizeof(MSB201X_DVBC_table);
            }
    }

    _MSB201X_I2C_CH_Reset(devID, 3);
 		
 		if(u8DemodIndex==DEMOD0)
 		{
 			//mux select for xdata setting from top riu to dmd riu
 			_MDrv_DMD_MSB201X_SetReg(devID, 0x103700 + 0x1C,  0x00);        
 			//Load code finish, eable i-side to sram
 			_MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0x81,  0x01);        
 			//Release dmd_mcu rst to wake up dmd51
 			_MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0x80,  0x00);        
 			
 		}
 		
 		if(u8DemodIndex==DEMOD1)
 		{
 			//mux select for xdata setting from top riu to dmd riu
 			_MDrv_DMD_MSB201X_SetReg(devID, 0x103800 + 0x1C,  0x00);        
 			//Load code finish, eable i-side to sram
 			_MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0xa1,  0x01);
 			//Release dmd_mcu rst to wake up dmd51
 			_MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + 0xa0,  0x00);
 		}
 		
 		_MSB201X_I2C_CH_Reset(devID, 5);
 		

    #ifdef MS_DEBUG
    	printf(">Demod Loadcode done.");
    #endif
	
    return TRUE;
}

/*
static MS_BOOL  _MDrv_DMD_MSB201X_DSPReg_Init(MS_U8 devID)
{
    MS_U8    idx = 0, idx2 = 0;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);

    for (idx = 0; idx<sizeof(MSB201X_DVBC_DSPREG_TABLE); idx++)
    {
    	for(idx2 = 0; idx2<DEMOD_MAX_CHANNEL; idx2++)
    	{
        if( _MDrv_DMD_MSB201X_SetDSPReg(devID, idx2, idx, pDemod->DVBC_DSP_REG[idx2][idx])!=TRUE)
        {
            printf("dvbc dsp reg init NG\n");
            return FALSE;
        }
    	}
    }

    // FSM enable
    for(idx = 0; idx<DEMOD_MAX_CHANNEL; idx++)
    	_MDrv_DMD_MSB201X_Active(devID, idx, TRUE);
	
    printf("DVBC dsp reg init ok\n");

    return TRUE;
}
*/


MS_U16 _MSB201X_CHIP_MATCH_TABLE[] =
{
    //Kaiser, Kaiserin, Keltic, Kronus, Kappa
    0x56,       0x41,     0x72,  0x2F,  0x75,
};

MS_BOOL MDrv_DMD_MSB201X_LoadDSPCode(MS_U8 devID)
{
    MS_BOOL bRet = TRUE;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);
    DMD_LOCK();
    //bRet = _LoadDSPCode();
    DMD_UNLOCK();
    return bRet;
}

MS_BOOL MDrv_DMD_MSB201X_SetActive(MS_U8 devID, MS_U8 u8DemodIndex, MS_BOOL bEnable)
{
    MS_BOOL bRet;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);

    DMD_LOCK();
	
    bRet=_MDrv_DMD_MSB201X_Active(devID, u8DemodIndex, bEnable);

    DMD_UNLOCK();
    return bRet;
}

MS_BOOL _MDrv_DMD_MSB201X_InitClkgen(MS_U8 devID)  //koln need modified
{
    _MSB201X_I2C_CH_Reset(devID, 3);
	
	  //inivec.task.cpp
	
	
    // DMD HK init clk start
// -------------------------------------------------------------------
// Initialize DMD_ANA_MISC
// -------------------------------------------------------------------

    // Koln add
    // [0]	reg_pd_ldo25i_ana   // 2.5v LDO power down
    // [1]	reg_pd_ldo25q_ana   // 2.5v LDO power down
    // [2]	reg_pd_ldo25i_dig   // 2.5v LDO power down
    // [3]	reg_pd_ldo25q_dig   // 2.5v LDO power down
    // [4]	reg_pd_ldo25_ref    // 2.5v LDO power down
    // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h53, 2'b01, 16'h0000);
    //wreg 4106 0x53 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x53)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x53)*2+1,  0x00);

    // [0]	reg_tst_ldo25i
    // [1]	reg_tst_ldo25q
    // [5:4]	reg_tst_ldo25i_selfb
    // [7:6]	reg_tst_ldo25q_selfb
    // [8]	reg_pd_dm2p5ldoi = 1'b0
    // [9]	reg_pd_dm2p5ldoq = 1'b0
    // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h4f, 2'b11, 16'h0000);
    // wreg 4106 0x4f 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x4f)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x4f)*2+1,  0x00);

    // [0]	reg_tst_ldo11_clk
    // [1]	reg_tst_ldo26
    // [2]	reg_tst_ldo11_cmp
    // [3]	reg_pd_dm1p1ldo_clk = 1'b0
    // [4]	reg_pd_dm1p1ldo_cmp = 1'b0
    // [6]	reg_tst_ldo26_selfb
    // [7]	reg_pd_dm2p6ldo = 1'b0
    // [9:8]	reg_tst_ldo11_cmp_selfb
    // [11:10]	reg_tst_ldo11_clk_selfb
    // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h4e, 2'b11, 16'h0000);
    //wreg 4106 0x4e 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x4e)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x4e)*2+1,  0x00);


    // [1:0]	reg_mpll_loop_div_first       feedback divider 00:div by 1 01:div by 2 10:div by 4 11:div by 8
    // [15:8]	reg_mpll_loop_div_second      feedback divider, div by binary data number
    // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h33, 2'b11, 16'h1201);  // Loop divider ; VCO = 24*(2^2)*9 = 864
//    wreg 4106 0x33 0x1201
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x33)*2  ,  0x01);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x33)*2+1,  0x12);

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
    // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h35, 2'b11, 16'h1803); // MPLL reset
    //wreg 4106 0x35 0x1803
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x35)*2  ,  0x03);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x35)*2+1,  0x18);    
    
    // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h35, 2'b11, 16'h0003); // release MPLl reset
    //wreg 4106 0x35 0x0003
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x35)*2  ,  0x03);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x35)*2+1,  0x00);    

    // [0]	reg_mpll_clk_dp_pd	dummy
    // [1]	reg_adc_clk_pd		ADC output clock power down
    // [2]	reg_mpll_div2_pd	MPLL_DIV2 power down
    // [3]	reg_mpll_div3_pd	MPLL_DIV3 power down
    // [4]	reg_mpll_div4_pd	MPLL_DIV4 power down
    // [5]	reg_mpll_div8_pd	MPLL_DIV8 power down
    // [6]	reg_mpll_div10_pd	MPLL_DIV10 power down
    // [13:8]  reg_mpll_adc_div_sel	select the ADC clock divide ratio,ADC clk=XTAL_IN
      // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h30, 2'b11, 16'h1e00);  // divide ADC clock to 28.8Mhz = 24*36/30
      //wreg 4106 0x30 0x1e00
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x30)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x30)*2+1,  0x1e);    


    //$display("--------------------------------------");
    //$display("Initialize ADC I/Q");
    //$display("--------------------------------------");

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
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h01, 2'b11, 16'h0440); // Set IMUXS QMUXS
        //wreg 4106 0x01 0x0440
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x01)*2  ,  0x40);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x01)*2+1,  0x04);    

    // [2:0]	reg_imuxs_s
    // [6:4]	reg_qmuxs_s
    // [9:8]	reg_iclpstr_s
    // [13:12]	reg_qclpstr_s
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h45, 2'b01, 16'h0000); // Set IMUXS QMUXS
        //wreg 4106 0x45 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x45)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x45)*2+1,  0x00);    

    
    // [0]	Channel I ADC power down: 1=power dwon
    // [1]	Channel Q ADC power down: 1=power dwon
    // [2]	power down clamp buffer for test mode
    // [3]	change ADC reference voltage for SSIF
    // [6:4]    ADC source bias current control
    // [9:8]    XTAL receiver amp gain
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h0c, 2'b11, 16'h0000); // Set enable ADC clock
    //    wreg 4106 0x0c 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x0c)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x0c)*2+1,  0x00);    

    
    // [0]	reg_linear_cal_start_q	0	0	1
    // [1]	reg_linear_cal_mode_q	0	0	1
    // [2]	reg_linear_cal_en_q	0	0	1
    // [3]	reg_linear_cal_code0_oren_q	0	0	1
    // [6:4]	reg_linear_cal_status_sel_q	2	0	3
    // [7]	reg_pwdn_vcalbuf	0	0	1

      // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h0f, 2'b01, 16'h0000); // Set reg_pwdn_vcalbuf = 1'b0
 //     wreg 4106 0x0f 0x0000
 	   _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x0f)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x0f)*2+1,  0x00);    


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
      // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h20, 2'b11, 16'h0000); // Disable PWDN_REF
      //wreg 4106 0x20 0x0000
 	   _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x20)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x20)*2+1,  0x00);    
    // Set ADC gain is 1
      // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h0b, 2'b11, 16'h0505);
      //wreg 4106 0x0b 0x0505
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x0b)*2  ,  0x05);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x0b)*2+1,  0x05);    

    // Disable ADC Sign bit
      // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h2e, 2'b11, 16'h0000);
      //wreg 4106 0x2e 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x2e)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x2e)*2+1,  0x00);    

    // ADC I channel offset
      // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h2a, 2'b11, 16'h0c00);
      //wreg 4106 0x2a 0x0c00
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x2a)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x2a)*2+1,  0x0c); 

    // ADC Q channel offset
      // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h2b, 2'b11, 16'h0c00);
      //wreg 4106 0x2b 0x0c00
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x2b)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x2b)*2+1,  0x0c); 

    
        // [5:0]reg_ckg_mcu
        // [6]	reg_power_good_mask
        // [11:8]reg_ckg_inner
	// [15:12]reg_ckg_iicm1
    	// `RIU_W((`RIUBASE_TOP>>1)+7'h0b, 2'b11, 16'h0430);
    	//wreg 4105 0x0b 0x0030
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x2b)*2  ,  0x30);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x2b)*2+1,  0x00); 
    
    
        // [1:0]reg_chanout_sel
        // [2]	reg_iq_filter_enable	= 1
        // [3]	reg_iq_filter_sel
        // [5:4]reg_adc_debug_clk_sel
    	// `RIU_W((`RIUBASE_ADCPLL>>1)+7'h17, 2'b11, 16'h0004);
    	//wreg 4106 0x17 0x0004
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x17)*2  ,  0x04);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x17)*2+1,  0x00); 
    


    // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h51, 2'b01, 16'h0081); // 2 channel DVBC
    //wreg 4106 0x51 0x0081
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x51)*2  ,  0x81);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x51)*2+1,  0x00); 


// -------------------------------------------------------------------
// Release clock gating
// -------------------------------------------------------------------
    //$display("--------------------------------------");
    //$display("Release clock gating");
    //$display("--------------------------------------");

    // [0]	reg_xtal_en
    // [9:8]	reg_clk_pd_iic
    // [10]	reg_clk_pd_all
      // `RIU_W((`RIUBASE_TOP>>1)+7'h09, 2'b11, 16'h0101);
      //wreg 4105 0x09 0x0101
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x09)*2  ,  0x01);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x09)*2+1,  0x01); 

    // [3:0]	reg_ckg_adcd
    // [7:4]	reg_ckg_sadc
    // [11:8]	reg_ckg_iicm
    // [13:12]	reg_ckg_sbus
      // `RIU_W((`RIUBASE_TOP>>1)+7'h0a, 2'b11, 16'h0000);
      //wreg 4105 0x0a 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x0a)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x0a)*2+1,  0x00); 

    // [5:0]	reg_ckg_mcu
    // [6]	reg_ckg_live
    // [11:8]	reg_ckg_inner
      // `RIU_W((`RIUBASE_TOP>>1)+7'h0b, 2'b11, 16'h0030);
//      wreg 4105 0x0b 0x0030
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x0b)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x0b)*2+1,  0x00); 


    // @0x0912
    // [3:0]	reg_ckg_dvbtm_ts
    // [4]	reg_dvbtm_ts_out_mode
    // [5]	reg_dvbtm_ts_clk_pol
    // [15:8]	reg_dvbtm_ts_clk_divnum
      // `RIU_W((`RIUBASE_TOP>>1)+7'h12, 2'b11, 16'h1418);
      //wreg 4105 0x12 0x1418
     //_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2  ,  0x18);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2  ,  0x10);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2+1,  0x14); 

    // @0x0913
    // [5:0]	reg_ckg_spi
      // `RIU_W((`RIUBASE_TOP>>1)+7'h13, 2'b11, 16'h0020);
      //wreg 4105 0x13 0x0020
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x13)*2  ,  0x20);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x13)*2+1,  0x00); 


    // @0x091b
    // [10:8]	reg_ckg_syn_miu
    // [14:12]	reg_ckg_syn_ts
      // `RIU_W((`RIUBASE_TOP>>1)+7'h1b, 2'b11, 16'h0000);
//      wreg 4105 0x1b 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x1b)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x1b)*2+1,  0x00); 



    // @0x091c
    // [4:0]	reg_ckg_bist
    // [11:8]	reg_ckg_adcd_d2
      // `RIU_W((`RIUBASE_TOP>>1)+7'h1c, 2'b11, 16'h0000);
      //wreg 4105 0x1c 0x0000

     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x1c)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x1c)*2+1,  0x00);


    // [1:0]	reg_iicm_pad_sel
    // [4]	reg_i2c_sbpm_en
    // [12:8]	reg_i2c_sbpm_idle_num
      // `RIU_W((`RIUBASE_TOP>>1)+7'h08, 2'b01, 16'h0a01);
      //wreg 4105 0x08 0x0a01

     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x08)*2  ,  0x01);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x08)*2+1,  0x0a);

    // [8]	reg_turn_off_pad
      // `RIU_W((`RIUBASE_TOP>>1)+7'h28, 2'b10, 16'h0000);
     // wreg 4105 0x28 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x28)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x28)*2+1,  0x00);




    //$display("--------------------------------------");
    //$display("Initialize Transport Stream synthesizer and APLL");
    //$display("--------------------------------------");

    // ////////////////////////////////////////////////////
    //
    //	According to analog APLL designer's suggest:
    //	APLL_LOOP_DIV = 5'b00000
    //	apll input frequency range 54MHz~106MHz synthesizer clock
    //	so apll_1x_out = synthesizer_out * (apll_ts_mode + 1)
    //
    //	=> apll_1x_out should 40Mhz ~ 130Mhz
    //
    //	Current setting:
    //	apll_1x_out = (432/8.0) * (1+1) = 108MHz
    //	choose reg_ckg_ts_apll_div[2:0] = 3'd4
    //	ts_clk_apll_div = 108/(2^4) = 6.75MHz
    //
    // ////////////////////////////////////////////////////


    // [15:0]	reg_synth_set[15: 0]
    // [ 7:0]	reg_synth_set[23:16]
      // `RIU_W((`RIUBASE_TOP>>1)+7'h51, 2'b11, 16'h0000);
      //wreg 4105 0x51 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x51)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x51)*2+1,  0x00);
      // `RIU_W((`RIUBASE_TOP>>1)+7'h52, 2'b11, 16'h0040);
      //wreg 4105 0x52 0x0040
		 _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x52)*2  ,  0x40);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x52)*2+1,  0x00);


    // [0]	reg_synth_reset
    // [1]	reg_synth_ssc_en
    // [2]	reg_synth_ssc_mode
    // [4]	reg_synth_sld
      // `RIU_W((`RIUBASE_TOP>>1)+7'h50, 2'b01, 16'h0010);
      //wreg 4105 0x50 0x0010

		 _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x50)*2  ,  0x10);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x50)*2+1,  0x00);

    // #10_000;
    //delay 0  ****


    // [1:0]	reg_apll_loop_div_first
    // [15:8]	reg_apll_loop_div_second
      // `RIU_W((`RIUBASE_TOP>>1)+7'h57, 2'b11, 16'h0000);
     //wreg 4105 0x57 0x0000
     
		 _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x57)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x57)*2+1,  0x00);


    // [0]	reg_apll_pd
    // [1]	reg_apll_reset
    // [2]	reg_apll_porst
    // [3]	reg_apll_vco_offset
    // [4]	reg_apll_en_ts
    // [5]	reg_apll_endcc
    // [6]	reg_apll_clkin_sel
    // [8]	reg_apll_ts_mode
      // `RIU_W((`RIUBASE_TOP>>1)+7'h55, 2'b11, 16'h0100);
      //wreg 4105 0x55 0x0100
		 _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x55)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x55)*2+1,  0x01);
      // `RIU_W((`RIUBASE_TOP>>1)+7'h55, 2'b01, 16'h0110);
      //wreg 4105 0x55 0x0110
		 _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x55)*2  ,  0x10);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x55)*2+1,  0x01);

    // [16:0]	reg_apll_test
      // `RIU_W((`RIUBASE_TOP>>1)+7'h59, 2'b11, 16'h0000);
      //wreg 4105 0x59 0x0000     
		 _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x59)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x59)*2+1,  0x00);

    // 0x0920
    // [3:0]	reg_ckg_ts_apll_div[2:0]
      // `RIU_W((`RIUBASE_TOP>>1)+7'h20, 2'b01, 16'h0004);
      //wreg 4105 0x20 0x0004
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x20)*2  ,  0x04);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x20)*2+1,  0x00);

		//  Following register control by reg_CLKGEN1
		//        reg_ckg_atsc_dvb_div_sel,  (clkgen0)
		//        reg_ckg_atsc_dvbtc_ts_inv, (clkgen0)
		//        reg_ckg_atsc_ts,           (clkgen0)
		//	reg_ckg_demod_test_in_en,  (clkgen0, clkgen_dmd)
		//        reg_ckg_dmdmcu,            (clkgen0)
		//        reg_ckg_dvbtc_adc,         (clkgen0, clkgen_dmd)
		//	reg_ckg_dvbtc_ts,          (clkgen0)
		//        reg_ckg_dvbtm_ts_divnum,   (clkgen0)
		//        reg_ckg_dvbtm_ts_out_mode, (clkgen0)
		//  reg_ckg_vifdbb_dac,        (clkgen0, clkgen_dmd)
		//	reg_ckg_vifdbb_vdac,       (clkgen0, clkgen_dmd)
		
		
		
		//$display("Set register at TOP (clkgen) ......");
		
		// { 1'b0, reg_ckg_adcd1[3:0], reg_clk_pd_all, 1'b0, reg_clk_pd_iic, 7'h0, reg_xtal_en }
		// `RIU_W((`RIUBASE_TOP>>1)+7'h09, 2'b10, 16'h0000);	        
		//wreg 4105 0x09 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x09)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x09)*2+1,  0x00);		
		// { 2'h0, reg_ckg_sbus[1:0], reg_ckg_iicm[3:0], reg_ckg_sadc[3:0], reg_ckg_adcd[3:0] }
		// `RIU_W((`RIUBASE_TOP>>1)+7'h0a, 2'b01, 16'h1110);	        
//		wreg 4105 0x0a 0x1110
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x0a)*2  ,  0x10);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x0a)*2+1,  0x11);
     		
		// { reg_ckg_demod_mpll[3:0], 4'h0, reg_ckg_dmdxtali[3:0], reg_ckg_dmdmcu[3:0] }
		// `RIU_W((`RIUBASE_TOP>>1)+7'h0d, 2'b10, 16'h0000);	
		//wreg 4105 0x0d 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x0d)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x0d)*2+1,  0x00);		

		// DVBC : 24*36/30=28.8 MHz
		// `RIU_W((`RIUBASE_ADCPLL>>1)+7'h33, 2'b11, 16'h1201);       // Set MPLL_LOOP_DIV_FIRST and SECOND
		//wreg 4106 0x33 0x1201
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x33)*2  ,  0x01);   
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x33)*2+1,  0x12);				
     // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h30, 2'b11, 16'h1e00+16'h1); // Set MPLL_ADC_DIV_SEL
		
		// reg_ckg_ts_0 = 4'd0;
		// reg_ckg_ts_1 = 4'd0;
		// `RIU_W((`RIUBASE_TOP>>1)+7'h23, 2'b01, 16'h0000);
		//wreg 4105 0x23 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x23)*2  ,  0x00);   
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x23)*2+1,  0x00);
     				
		//$display("=================================================");
		//$display("start demod atop ADC setting ......");
		//$display("=================================================");
		
		// { 8'h0, reg_ana_setting_sel[3:0], 3'h0, reg_ana_setting_enable } )
		// `RIU_W((`RIUBASE_ADCPLL>>1)+7'h51, 2'b01, 16'h0081);	
		//wreg 4106 0x51 0x0081
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x51)*2  ,  0x81);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x51)*2+1,  0x00);		
		
		//if agc enable
		//wreg 4106 0x18 0x0101  
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x18)*2  ,  0x01); 		
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x18)*2+1,  0x01);			
		
		//wreg 4106 0x30 0x1200
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x30)*2  ,  0x00); 		
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x30)*2+1,  0x12);		
		




    return TRUE;
}

MS_BOOL _MDrv_DMD_MSB201X_SoftStop(MS_U8 devID, MS_U8 u8DemodIndex)
{
    MS_U16     u8WaitCnt=0;
    MS_U16	banknum;
    MS_U8		tmp, intr_shift;

    _MSB201X_I2C_CH_Reset(devID, 3);
    // Select mailbox bank number
    switch(u8DemodIndex)
   {
   	case 0:
   		banknum = 0x2600;	
		intr_shift = 0x41;
		break;
	case 1:
   		banknum = 0x2900;
		intr_shift = 0x51;
		break;
	case 2:
   		banknum = 0x3300;	
		intr_shift = 0x61;
		break;
	case 3:
   		banknum = 0x3700;
		intr_shift = 0x71;
		break;
	default :
		banknum = 0x2600;
		intr_shift = 0x41;
		break;
   }

    _MDrv_DMD_MSB201X_SetReg(devID, banknum + 0x00, 0xA5);                 // MB_CNTL set read mode

    _MDrv_DMD_MSB201X_SetReg(devID, 0x3400 + (intr_shift * 2) + 1, 0x02);                         // assert interrupt to VD MCU51
    _MDrv_DMD_MSB201X_SetReg(devID, 0x3400 + (intr_shift * 2) + 1, 0x00);                         // de-assert interrupt to VD MCU51

    _MDrv_DMD_MSB201X_GetReg(devID, banknum + 0x00, &tmp);
    while(tmp != 0x5A)           // wait MB_CNTL set done
    {
        if (u8WaitCnt++ >= 0xFF)
        {
            printf(">> SoftStop Fail!\n");
            return FALSE;
        }
	 _MDrv_DMD_MSB201X_GetReg(devID, banknum + 0x00, &tmp);
    }

    _MDrv_DMD_MSB201X_SetReg(devID, banknum + 0x00, 0x00);                 // MB_CNTL clear

    return TRUE;
}

MS_BOOL _MDrv_DMD_MSB201X_Select_Demod_RIU(MS_U8 devID, MS_U8 u8DemodIndex)  //koln
{
    _MSB201X_I2C_CH_Reset(devID, 3);
    _MDrv_DMD_MSB201X_SetReg(devID, 0x103C00 + (0x13)*2, u8DemodIndex);

    return TRUE;
}

#if(0)
MS_BOOL _MDrv_DMD_MSB201X_TS_MUX_Serial(MS_U8 devID)
{


	_MSB201X_I2C_CH_Reset(devID, 3);
	
// This file is translated by Steven Hung's riu2script.pl									test
									
    // `RIU_W ((`RIUBASE_TOP>>1)+7'h12, 2'b11, 16'h0610); //reg_ckg_dvbtm_ts_mux									
    //wreg  0x1009 0x12 0x0220									
    _MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x12)*2, 0x0220);
    // `RIU_W ((`RIUBASE_TOP>>1)+7'h1e, 2'b11, 16'h4444); //reg_ckg_ts_mux									
    //wreg  0x1009 0x1e 0x4444									
    _MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x1e)*2, 0x4444);
    // `RIU_W ((`RIUBASE_TOP>>1)+7'h23, 2'b11, 16'h0000); //reg_ckg_ts_*									
    //wreg  0x1009 0x23 0x0000									
     _MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x23)*2, 0x0000);
    // `RIU_W ((`RIUBASE_TOP>>1)+7'h28, 2'b11, 16'h0000); //all pad in									
    //wreg  0x1009 0x28 0x0000									
    _MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x28)*2, 0x0000);
    // `RIU_W ((`RIUBASE_TOP>>1)+7'h2e, 2'b11, 16'h0103); //en ts	
    // reg_en_ts_err_pad 								
    ////wreg 0x1009 0x2e 0x0103	
    // reg_en_ts_pad_serial_3wire_mode
    //wreg  0x1009 0x2d 0x0003		
     _MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x2d)*2, 0x0003);
    // reg_en_ts_pad_parallel_mode
    //wreg  0x1009 0x2e 0x0000   
    _MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x2e)*2, 0x0000);
    // reg_en_ts_pad_serial_4wire_mode
    //wreg  0x1009 0x29 0x0100							
    _MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x29)*2, 0x0100);
    // `RIU_W ((`RIUBASE_TOP>>1)+7'h3b, 2'b11, 16'h0000); //disable reg_ts_sspi_en									
    //wreg  0x1009 0x3b 0x0000	
    _MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x3b)*2, 0x0000);
    
    // reg_ckg_ts_0~3
    //wreg  0x1009 0x23 0x8888								
    _MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100900 + (0x23)*2, 0x8888);
									
    // `RIU_W ((`RIUBASE_DIG>>1)+7'h70, 2'b11, 16'h0000); //reg_swrst_ts_mux									
    //wreg 0x100b 0x70 0x0000							
    _MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x100b00 + (0x70)*2, 0x0000);
    
    
    //TS remux bank setting		
    // wreg 42 0x00 0x00ff									
    //wreg 0x1031 0x00 0x00ff		
    _MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x103100 + (0x00)*2, 0x00ff);

    //serial mode	
    //wreg 42 0x01 0x001e 					
    //wreg 0x1031 0x01 0x001e		
    _MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x103100 + (0x01)*2, 0x001e);
									

   //*****************************/
   //wreg 0x1031 0x04 0x802a
	 _MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x103100 + (0x04)*2, 0x802a);
	 // `RIU_W ((`RIUBASE_TS_MUX>>1)+7'h06, 2'b11, 16'h0020); //reg_client_mask
	 //wreg 0x1031 0x06 0x0020
	 _MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x103100 + (0x06)*2, 0x0020);

 	 //disable sync mask function
   //wreg 0x1031 0x02 0x001f
   _MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x103100 + (0x02)*2, 0x001f);
 	// _MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x103100 + (0x02)*2, 0x001f);			    
  //*******************************/		

    // wreg  42 0x00 0x03ff
    //wreg 0x1031 0x00 0x03ff		
     _MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x103100 + (0x00)*2, 0x03ff);
    // wreg  42 0x00 0x00ff
    //wreg 0x1031 0x00 0x00ff
    _MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x103100 + (0x00)*2, 0x00ff);
    // wreg  42 0x00 0x000f
    //wreg 0x1031 0x00 0x000f
    _MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x103100 + (0x00)*2, 0x000f);
 
	return TRUE;
}
#endif

MS_BOOL _MDrv_DMD_MSB201X_Restart(MS_U8 devID, MS_U8 u8DemodIndex)
{
    MS_U8		reset_shift;
    MS_U16		banknum;


    // Select mailbox bank number and DMD_MCU reset shift
    switch(u8DemodIndex)
   {
   	case 0:
   		banknum = 0x1035;	
		reset_shift = 0x40;
		break;
	case 1:
   		banknum = 0x1036;
		reset_shift = 0x50;
		break;
	/*
	case 2:
   		banknum = 0x3300;	
		reset_shift = 0x60;
		break;
	case 3:
   		banknum = 0x3700;
		reset_shift = 0x70;
		break;
	*/
	default :
   		banknum = 0x1035;	
		reset_shift = 0x40;
		break;
   }
	
    printf(" @MDrv_DMD_MSB201X_Restart!\n");

    _MSB201X_I2C_CH_Reset(devID, 3);	
    _MDrv_DMD_MSB201X_SoftStop(devID, u8DemodIndex);

    _MDrv_DMD_MSB201X_Select_Demod_RIU(devID, u8DemodIndex);

    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + reset_shift*2, 0x01);     // reset DMD_MCU

    _MSB201X_Delay_Task(5);
    _MDrv_DMD_MSB201X_SetReg(devID, banknum + 0x00 , 0x00);     // clear MB_CNTL

    _MDrv_DMD_MSB201X_SetReg(devID, 0x103400 + reset_shift*2, 0x00);
    _MSB201X_Delay_Task(5);

    _MDrv_DMD_MSB201X_SetReg(devID, banknum + 0x00 , 0x00);

    return TRUE;
}

MS_BOOL MDrv_DMD_MSB201X_Restart(MS_U8 devID, MS_U8 u8DemodIndex)
{
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);
    DMD_LOCK();

    _MDrv_DMD_MSB201X_Restart(devID, u8DemodIndex);

    DMD_UNLOCK();
    return TRUE;
}

MS_BOOL _MDrv_DMD_MSB201X_GetPacketErr(MS_U8 devID,MS_U8 u8DemodIndex, MS_U16 *pktErr)
{
    MS_U8            reg = 0, reg_frz = 0;
    MS_U16           PktErr;
	
    _MSB201X_I2C_CH_Reset(devID, 5);

    // bank 1f 0x03 [1:0] reg_bit_err_num_freeze
    _MDrv_DMD_MSB201X_GetReg(devID, BACKEND_REG_BASE+0x03, &reg_frz);
    _MDrv_DMD_MSB201X_SetReg(devID, BACKEND_REG_BASE+0x03, reg_frz|0x03);

    // bank 1f 0x66 [7:0] reg_uncrt_pkt_num_7_0
    //             0x67 [15:8] reg_uncrt_pkt_num_15_8
    _MDrv_DMD_MSB201X_GetReg(devID, BACKEND_REG_BASE+0x67, &reg);
    PktErr = reg;

    _MDrv_DMD_MSB201X_GetReg(devID, BACKEND_REG_BASE+0x66, &reg);
    PktErr = (PktErr << 8)|reg;

    // bank 1f 0x03 [1:0] reg_bit_err_num_freeze
    reg_frz=reg_frz&(~0x03);
    _MDrv_DMD_MSB201X_SetReg(devID, BACKEND_REG_BASE+0x03, reg_frz);

    #if 1
    	printf("MSB201X Demod %d PktErr = %d \n ", (int)u8DemodIndex, (int)PktErr);
    #endif
	
    *pktErr = PktErr;

    return TRUE;
}

MS_BOOL MDrv_DMD_MSB201X_GetPacketErr(MS_U8 devID,MS_U8 u8DemodIndex, MS_U16 *pktErr)
{
    MS_BOOL          status = true;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);
    DMD_LOCK();

    _MDrv_DMD_MSB201X_Select_Demod_RIU(devID, u8DemodIndex);
    _MDrv_DMD_MSB201X_GetPacketErr(devID,u8DemodIndex, pktErr);

    DMD_UNLOCK();
	
    return status;
}

MS_BOOL _MDrv_DMD_MSB201X_GetPostViterbiBer(MS_U8 devID,MS_U8 u8DemodIndex, float *ber)
{
    MS_BOOL           status = true;
    MS_U8             reg = 0, reg_frz = 0;
    MS_U16            BitErrPeriod;
    MS_U32            BitErr;
	
    _MSB201X_I2C_CH_Reset(devID, 5);

    // bank 1f 0x03 [1:0] reg_bit_err_num_freeze
    _MDrv_DMD_MSB201X_GetReg(devID, BACKEND_REG_BASE+0x03, &reg_frz);
    _MDrv_DMD_MSB201X_SetReg(devID, BACKEND_REG_BASE+0x03, reg_frz|0x03);

    // bank 1f 0x46 [7:0] reg_bit_err_sblprd_7_0    
    //             0x47 [15:8] reg_bit_err_sblprd_15_8
    _MDrv_DMD_MSB201X_GetReg(devID, BACKEND_REG_BASE+0x47, &reg);
    BitErrPeriod = reg;

    _MDrv_DMD_MSB201X_GetReg(devID, BACKEND_REG_BASE+0x46, &reg);
    BitErrPeriod = (BitErrPeriod << 8)|reg;

    // bank 1f 0x6a [7:0] reg_bit_err_num_7_0
    //             0x6b [15:8] reg_bit_err_num_15_8
    // bank 1f 0x6c [7:0] reg_bit_err_num_23_16
    //             0x6d [15:8] reg_bit_err_num_31_24
    status &= _MDrv_DMD_MSB201X_GetReg(devID, BACKEND_REG_BASE+0x6d, &reg);
    BitErr = reg;

    status &= _MDrv_DMD_MSB201X_GetReg(devID, BACKEND_REG_BASE+0x6c, &reg);
    BitErr = (BitErr << 8)|reg;

    status &= _MDrv_DMD_MSB201X_GetReg(devID, BACKEND_REG_BASE+0x6b, &reg);
    BitErr = (BitErr << 8)|reg;

    status &= _MDrv_DMD_MSB201X_GetReg(devID, BACKEND_REG_BASE+0x6a, &reg);
    BitErr = (BitErr << 8)|reg;

    // bank 1f 0x03 [1:0] reg_bit_err_num_freeze
    reg_frz=reg_frz&(~0x03);
    _MDrv_DMD_MSB201X_SetReg(devID, BACKEND_REG_BASE+0x03, reg_frz);

    if (BitErrPeriod == 0 )    //protect 0
        BitErrPeriod = 1;

    if (BitErr <=0 )
        *ber = 0.5f / ((float)BitErrPeriod*128*188*8);
    else
        *ber = (float)BitErr / ((float)BitErrPeriod*128*188*8);

    printf("MSB201X Demod %d PostVitBER = %8.3e \n ", (int)u8DemodIndex, *ber);

    return TRUE;
}

MS_BOOL MDrv_DMD_MSB201X_GetPostViterbiBer(MS_U8 devID,MS_U8 u8DemodIndex, float *ber)
{
    MS_BOOL          status = true;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);
    DMD_LOCK();

    _MDrv_DMD_MSB201X_Select_Demod_RIU(devID, u8DemodIndex);
    _MDrv_DMD_MSB201X_GetPostViterbiBer(devID,u8DemodIndex, ber);

    DMD_UNLOCK();
	
    return status;
}

MS_BOOL _MDrv_DMD_MSB201X_GetSNR(MS_U8 devID,MS_U8 u8DemodIndex, float *f_snr)
{
    MS_BOOL status = true;
    MS_U8 u8Data = 0;//reg_frz = 0;
    // MS_U8 freeze = 0;
    MS_U16 noisepower = 0;

    if (TRUE == MDrv_DMD_MSB201X_Demod_GetLock(devID, u8DemodIndex) )
    {
        _MSB201X_I2C_CH_Reset(devID, 5);
		
        // bank 2c 0x3d [0] reg_bit_err_num_freeze
        status &= _MDrv_DMD_MSB201X_SetReg(devID, EQE_REG_BASE + 0x3a, 0x20);
        status &= _MDrv_DMD_MSB201X_SetReg(devID, INNC_REG_BASE + 0x05, 0x80);
        // read vk
        status &= _MDrv_DMD_MSB201X_GetReg(devID, EQE_REG_BASE + 0x45, &u8Data);
        noisepower = u8Data;
        status &= _MDrv_DMD_MSB201X_GetReg(devID, EQE_REG_BASE + 0x44, &u8Data);
        noisepower = (noisepower<<8)|u8Data;

        // bank 2c 0x3d [0] reg_bit_err_num_freeze
        status &= _MDrv_DMD_MSB201X_SetReg(devID, EQE_REG_BASE + 0x3a, 0x00);
        status &= _MDrv_DMD_MSB201X_SetReg(devID, INNC_REG_BASE + 0x05, 0x00);

        if(noisepower == 0x0000)
            noisepower = 0x0001;

        *f_snr = 10.0f*log10f(65536.0f/(float)noisepower);

    }
    else
    {
        *f_snr = 0.0f;
    }
	
    return status;
}

MS_BOOL MDrv_DMD_MSB201X_GetSNR(MS_U8 devID,MS_U8 u8DemodIndex, float *fSNR)
{
    MS_BOOL bRet;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);
    DMD_LOCK();

    _MDrv_DMD_MSB201X_Select_Demod_RIU(devID, u8DemodIndex);
    bRet=_MDrv_DMD_MSB201X_GetSNR(devID, u8DemodIndex, fSNR);
	
    DMD_UNLOCK();

    return bRet;
}

static MS_BOOL _MDrv_DMD_MSB201X_Variable_alloc(void)
{
    MS_U8 i;
    tMSB201X_Demod_Data *pMSB201X = NULL;

    if(NULL == pstDemod)
    {
        pstDemod = (tMSB201X_Demod_Data *)malloc(sizeof(tMSB201X_Demod_Data) * DEMOD_MAX_INSTANCE);
        if(NULL == pstDemod)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< DEMOD_MAX_INSTANCE; i++)
            {
                pMSB201X = (pstDemod + i);
                memcpy(pMSB201X, &MSB201X_Demod_Init, sizeof(tMSB201X_Demod_Data));
            }
        }
    }

    if(NULL == pDemodRest)
    {
        pDemodRest = (MS_BOOL*)malloc(sizeof(MS_BOOL) * DEMOD_MAX_INSTANCE);
        if(NULL == pDemodRest)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< DEMOD_MAX_INSTANCE; i++)
                *(pDemodRest + i) = TRUE;
        }
    }

    return TRUE;
}









static MS_BOOL _MSB201X_Demod_SPILoadAll(MS_U8 devID, eDMD_MSB201X_Demod_Index eDemod_Index, MS_U8 *u8_ptr, MS_U16 data_length)
{
	MS_BOOL bRet = TRUE;
	MS_U32 u32Addr = 0;

	_MSB201X_I2C_CH_Reset(devID, 3);
	


  //2.Enable sspi2dmd51 sram & select sspi mode
  //SSPI_RIU_W, addr:0x0974, data:0x3100  (dmdall)
  MDrv_DMD_SSPI_RIU_Write8(0x0974, 0x00);
  MDrv_DMD_SSPI_RIU_Write8(0x0975, 0x30);
  //3. Disable all pad in
  //SSPI_RIU_W, addr:0x0950, data:0x0000  (dmdall) 
  MDrv_DMD_SSPI_RIU_Write8(0x0950, 0x00);
  MDrv_DMD_SSPI_RIU_Write8(0x0951, 0x00); 
  //4. DMD51 interrupt by PM51 (Option)
  //SSPI_RIU_W, addr:0x3482, data:0x0200
  MDrv_DMD_SSPI_RIU_Write8(0x3482, 0x00);
  MDrv_DMD_SSPI_RIU_Write8(0x3483, 0x02);
  //5. Enable program sram clock




  #if(0)
  /*  bryan mark test for SPI
  //b. SSPI_RIU_W, addr:0x0916, data:0x1c30 //xtali
  MDrv_DMD_SSPI_RIU_Write8(0x0916,0x30);
  MDrv_DMD_SSPI_RIU_Write8(0x0917,0x1C);
  //SSPI_RIU_W, addr:0x0916, data:0x1c20 //[5]deglitch
  MDrv_DMD_SSPI_RIU_Write8(0x0916,0x20);
  MDrv_DMD_SSPI_RIU_Write8(0x0917,0x1c);
  */
  #else
  //b. SSPI_RIU_W, addr:0x0916, data:0x1c30 //xtali
  MDrv_DMD_SSPI_RIU_Write8(0x0916,0x00);
  MDrv_DMD_SSPI_RIU_Write8(0x0917,0x00);  
  #endif
  
  //6. Release dmd_mcu periphal rst (All/Single)
  //SSPI_RIU_W, addr:0x3486, data:0x0000 (dmd0)

  //load all demod
  MDrv_DMD_SSPI_RIU_Write8(0x3486,0x00);
  MDrv_DMD_SSPI_RIU_Write8(0x3487,0x00);
  MDrv_DMD_SSPI_RIU_Write8(0x34a6,0x00);
  MDrv_DMD_SSPI_RIU_Write8(0x34a7,0x00);
  
	u32Addr = 0x80000000;//SRAM_BASE<<16;
       bRet &= MDrv_DMD_SSPI_MIU_Writes(u32Addr, u8_ptr, data_length);

	MDrv_DMD_SSPI_RIU_Write8(0x0974, 0x00);
	MDrv_DMD_SSPI_RIU_Write8(0x0975, 0x00);//0x01

	//MDrv_DMD_SSPI_RIU_Write8(0x0919, 0x00);
	
	MDrv_DMD_SSPI_RIU_Write8(0x3480, 0x00);
	MDrv_DMD_SSPI_RIU_Write8(0x3481, 0x01);
	MDrv_DMD_SSPI_RIU_Write8(0x34A0, 0x00);
	MDrv_DMD_SSPI_RIU_Write8(0x34A1, 0x01);
	
	
	return bRet;
}

MS_BOOL MDrv_DMD_MSB201X_Init(MS_U8 devID, MS_U8 u8DemodIndex, sDMD_MSB201X_InitData *pDMD_MSB201X_InitData, MS_U32 u32InitDataLen)
{
    MS_U8 idx = 0, u8Mask = 0, u8RegWrite = 0, u8Channel = 0;
    MS_U16 u16DspAddr = 0;
    MS_U8 idx2 = 0;
    MS_U8 u8Data;
    MS_U8 *code_ptr;
    MS_U16 code_size;
    
    _MDrv_DMD_MSB201X_Variable_alloc();
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);

    DMD_LOCK();
	
    if ( sizeof(sDMD_MSB201X_InitData) == u32InitDataLen)
    {
        memcpy(&pDemod->_sDMD_MSB201X_InitData, pDMD_MSB201X_InitData, u32InitDataLen);
    }
    else
    {
        DMD_DBG(printf("MDrv_DMD_MSB201X_Init input data structure incorrect\n"));
        //DMD_UNLOCK();
        return FALSE;
    }
    
    pDemod->DSP_ReadWrite_Mode = E_MSB201X_I2C_READ_WRITE;

   
    
    if(u8DemodIndex == ALL_DEMOD)
    {
	 //for SPI communication
    if (pDemod->_sDMD_MSB201X_InitData.bEnableSPILoadCode &&
        pDemod->_sDMD_MSB201X_InitData.fpMSB201X_SPIPAD_En != NULL)
    {
    	 _MSB201X_I2C_CH_Reset(devID, 3);
        if (!MDrv_DMD_SSPI_Init(0))
        {
            printf("MDrv_DMD_MSB201X_Init Init MDrv_DMD_SSPI_Init Fail \n");
        }
        //add today
        pDemod->DSP_ReadWrite_Mode = E_MSB201X_I2C_READ_WRITE;

        
        MDrv_MasterSPI_CsPadConfig(0, 0xff);
        MDrv_MasterSPI_MaxClkConfig(0, 1);
        //switch SPI pad on demod side
        (pDemod->_sDMD_MSB201X_InitData.fpMSB201X_SPIPAD_En)(TRUE);
        // ------enable to use TS_PAD as SSPI_PAD
        // [0:0] reg_en_sspi_pad
        // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
        _MDrv_DMD_MSB201X_SetReg2Bytes(devID,0x100900 + (0x3b) * 2, 0x0002);


        /*
        	//Turn off all pad in
        	_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x0900 + (0x28) * 2, 0x0000);
        	//Transport Stream pad on
        	_MDrv_DMD_MSB201X_SetReg2Bytes(devID, 0x0900 + (0x2d) * 2, 0x00ff);
        */
        // ------- MSPI protocol setting
        // [8] cpha
        // [9] cpol
        _MDrv_DMD_MSB201X_GetReg(devID,0x100900+(0x3a)*2+1,&u8Data);
        u8Data &= 0xFC;
        _MDrv_DMD_MSB201X_SetReg(devID,0x100900+(0x3a)*2+1, u8Data);
        // ------- MSPI driving setting
        _MDrv_DMD_MSB201X_SetReg2Bytes(devID,0x100900+(0x2c)*2, 0x07ff);


        _MDrv_DMD_MSB201X_SSPI_CFG_W(0x02,0x10);       

	 _MDrv_DMD_MSB201X_InitClkgen(devID);

		code_ptr = MSB201X_DVBC_table;
		code_size = sizeof(MSB201X_DVBC_table);
    	 _MSB201X_Demod_SPILoadAll(devID, ALL_DEMOD, code_ptr, code_size);	//load demod FW code via SPI

		//_MDrv_DMD_MSR1742_SetReg2Bytes(devID, 0x0900 + (0x3b) * 2, 0x0000);
        	//(pDemod->_sDMD_MSR1742_InitData.fpMSR1742_SPIPAD_En)(FALSE);

		//time_end = MsOS_GetSystemTime();
		//printf("=====Brown Demod SPI load code time = %d\n", (time_end-time_start));
	pDemod->DSP_ReadWrite_Mode = E_MSB201X_SPI_READ_WRITE;
        
        
        
    }
    else
    {
    		 _MDrv_DMD_MSB201X_InitClkgen(devID);
    	        pDemod->_sDMD_MSB201X_InitData.bEnableSPILoadCode = FALSE;
    		_MSB201X_Demod_LoadAll(devID, ALL_DEMOD);	//load demod FW code
    		pDemod->DSP_ReadWrite_Mode = E_MSB201X_I2C_READ_WRITE;
    }
    
		
    	// Init DSP table for all channels
    	for(idx = 0; idx < DEMOD_MAX_CHANNEL; idx++)
    	{
    		memcpy (&pDemod->DVBC_DSP_REG[idx][0],
                MSB201X_DVBC_DSPREG_TABLE ,
                sizeof(MSB201X_DVBC_DSPREG_TABLE));

            
            



		
		for(idx2 = 0; idx2 < sizeof(MSB201X_DVBC_DSPREG_TABLE); idx2++)
		{
			_MDrv_DMD_MSB201X_SetDSPReg(devID, idx, idx2, MSB201X_DVBC_DSPREG_TABLE[idx2]);
			
		}
		

    	}


    	
    }
    else
    {
    	_MDrv_DMD_MSB201X_InitClkgen(devID);
    	_MSB201X_Demod_LoadSingle(devID, u8DemodIndex);	//load demod FW code


	memcpy (&pDemod->DVBC_DSP_REG[u8DemodIndex][0],
                MSB201X_DVBC_DSPREG_TABLE ,
                sizeof(MSB201X_DVBC_DSPREG_TABLE));


   
		for(idx2 = 0; idx2 < sizeof(MSB201X_DVBC_DSPREG_TABLE); idx2++)
		{
			_MDrv_DMD_MSB201X_SetDSPReg(devID, u8DemodIndex, idx2, MSB201X_DVBC_DSPREG_TABLE[idx2]);
		}
	
    }

    /* DSP table example :
	static MS_U8 u8DSPTable[] =
    	{
        //addr_L add_H mask value demod_channel
        0x01, 00,
        0x16, 0x00, 0xff, 0xBB, 0, //FS_H  45.474M :0xB1  48M:0xBB
        0x15, 0x00, 0xff, 0x80, 0, //FS_L  45.474M :0xA2  48M:0x80
    	};
    */

    if (pDemod->_sDMD_MSB201X_InitData.pDVBC_DSP_REG!= NULL)
    {
     

	 if(1 == pDemod->_sDMD_MSB201X_InitData.pDVBC_DSP_REG[0])
	 {
	 	pDemod->_sDMD_MSB201X_InitData.pDVBC_DSP_REG += 2;
		for(idx = 0; idx < pDemod->_sDMD_MSB201X_InitData.DVBC_DSP_REG_Length-2; idx++)
		{
			u16DspAddr = *(pDemod->_sDMD_MSB201X_InitData.pDVBC_DSP_REG);
			pDemod->_sDMD_MSB201X_InitData.pDVBC_DSP_REG ++;
			u16DspAddr = (u16DspAddr) + (*(pDemod->_sDMD_MSB201X_InitData.pDVBC_DSP_REG)<<8);
			pDemod->_sDMD_MSB201X_InitData.pDVBC_DSP_REG ++;
			u8Mask = *(pDemod->_sDMD_MSB201X_InitData.pDVBC_DSP_REG);
			pDemod->_sDMD_MSB201X_InitData.pDVBC_DSP_REG ++;
			u8RegWrite = (*(pDemod->_sDMD_MSB201X_InitData.pDVBC_DSP_REG)) & u8Mask;
			pDemod->_sDMD_MSB201X_InitData.pDVBC_DSP_REG ++;
			u8Channel = *(pDemod->_sDMD_MSB201X_InitData.pDVBC_DSP_REG);

			//pDemod->DVBC_DSP_REG[u8Channel][u16DspAddr] = u8RegWrite;

			if(ALL_DEMOD == u8DemodIndex)
				_MDrv_DMD_MSB201X_SetDSPReg(devID, u8Channel, u16DspAddr, u8RegWrite);
			else if(u8Channel == u8DemodIndex)
				_MDrv_DMD_MSB201X_SetDSPReg(devID, u8Channel, u16DspAddr, u8RegWrite);
			
			memcpy (&pDemod->DVBC_DSP_REG[u8Channel][u16DspAddr],
                		&u8RegWrite ,
                		1);
                	
		}
	 }
    }
    
    
    
    if(u8DemodIndex == ALL_DEMOD)
    {
    	for(idx = 0; idx < DEMOD_MAX_CHANNEL; idx++)
    	{
    		// Demod active for all channel
    		_MDrv_DMD_MSB201X_Active(devID, idx, TRUE);
    	}
    }
    else
    {
    		// Demod active for single channel
    	  _MDrv_DMD_MSB201X_Active(devID, u8DemodIndex, TRUE);
    }
    

    if (E_MSB201X_SPI_READ_WRITE==pDemod->DSP_ReadWrite_Mode)
    {
    		//SPI function
        // ------disable to use TS_PAD as SSPI_PAD after load code
        // [0:0] reg_en_sspi_pad
        // [1:1] reg_ts_sspi_en, 1: use TS_PAD as SSPI_PAD
        pDemod->DSP_ReadWrite_Mode = E_MSB201X_I2C_READ_WRITE;
        //_MDrv_DMD_MSB201X_SetReg2Bytes(devID,0x100900 + (0x3b) * 2, 0x0001);
         _MDrv_DMD_MSB201X_SetReg2Bytes(devID,0x100900 + (0x3b) * 2, 0x0000);
        (pDemod->_sDMD_MSB201X_InitData.fpMSB201X_SPIPAD_En)(FALSE);
        
    }
		
    DMD_UNLOCK();
    return TRUE;
}


MS_BOOL _MDrv_DMD_MSB201X_GetRFLevel(MS_U8 devID, MS_U8 u8DemodIndex, float *fRFPowerDbmResult, float fRFPowerDbm,
                                                     sDMD_MSB201X_IFAGC_SSI *pIfagcSsi_LoRef, MS_U16 u16IfagcSsi_LoRef_Size,
                                                     sDMD_MSB201X_IFAGC_ERR *pIfagcErr_LoRef, MS_U16 u16IfagcErr_LoRef_Size)
{
    sDMD_MSB201X_IFAGC_SSI   *ifagc_ssi;
    sDMD_MSB201X_IFAGC_ERR   *ifagc_err;    
    float   ch_power_db=0.0f;
    float   ch_power_rf=0.0f;
    float   ch_power_if=0.0f, ch_power_ifa = 0.0f, ch_power_ifb =0.0f;
    float   ch_power_takeover=0.0f;
    MS_U16  if_agc_err = 0;    
    MS_U8   status = true;
    MS_U8   reg_tmp = 0, reg_tmp2 =0, reg_frz = 0,if_agc_val =0, if_agc_vala =0, if_agc_valb =0, if_agc_val_lsb =0, i;
    MS_U8   ssi_tbl_len = 0, err_tbl_len = 0;    

    _MDrv_DMD_MSB201X_Select_Demod_RIU(devID, u8DemodIndex);
    _MSB201X_I2C_CH_Reset(devID, 5);
    if (pIfagcSsi_LoRef !=NULL)
    {      
	ch_power_rf = fRFPowerDbm;
	// get IFAGC status
        {
            #ifdef MS_DEBUG
            printf("AGC_REF = %d\n", (MS_U16)reg_tmp);
            #endif
			
		  ifagc_ssi = pIfagcSsi_LoRef;
                ssi_tbl_len = u16IfagcSsi_LoRef_Size;
                ifagc_err = pIfagcErr_LoRef;
                err_tbl_len = u16IfagcErr_LoRef_Size;

            // bank 5 0x24 [15:0] reg_agc_gain2_out
            // use only high byte value

            // select IF gain to read
            status &= _MDrv_DMD_MSB201X_SetReg(devID, TDF_REG_BASE + 0x16, 0x03);
            status &= _MDrv_DMD_MSB201X_GetReg(devID, TDF_REG_BASE + 0x03, &reg_frz);
            status &= _MDrv_DMD_MSB201X_SetReg(devID, TDF_REG_BASE + 0x03, reg_frz | 0x80);
            status &= _MDrv_DMD_MSB201X_GetReg(devID, TDF_REG_BASE + 0x19, &reg_tmp);
            if_agc_val = reg_tmp;
            status &= _MDrv_DMD_MSB201X_GetReg(devID, TDF_REG_BASE + 0x18, &reg_tmp);
            if_agc_val_lsb = reg_tmp;    
            status &= _MDrv_DMD_MSB201X_SetReg(devID, TDF_REG_BASE + 0x03, reg_frz);
            #ifdef MS_DEBUG
            printf("SSI_IFAGC_H = 0x%x 0x%x\n", if_agc_val,if_agc_val_lsb);
            #endif

            ch_power_if=ifagc_ssi[0].power_db;
            if (if_agc_val >=ifagc_ssi[0].agc_val)
            {
                for(i = 1; i < ssi_tbl_len; i++)
                {
                    if (if_agc_val < ifagc_ssi[i].agc_val)
                    {
                        if_agc_valb = ifagc_ssi[i].agc_val;
                        ch_power_ifb = ifagc_ssi[i].power_db;
        
                        i--;
                        if_agc_vala = ifagc_ssi[i].agc_val;
                        ch_power_ifa=ifagc_ssi[i].power_db;
                        while ((i>1) && (if_agc_vala==ifagc_ssi[i-1].agc_val))
                        {
                            ch_power_ifa=ifagc_ssi[i-1].power_db;
                            i--;
                        }
                        ch_power_if = ch_power_ifa+(ch_power_ifb-ch_power_ifa)*(float)((if_agc_val-if_agc_vala)*256+if_agc_val_lsb)/((if_agc_valb-if_agc_vala)*256);
                        break;
                    }
                }
            }
            #ifdef MS_DEBUG
            printf("if prev %f %x\n", ch_power_ifa, if_agc_vala);
            printf("if next %f %x\n", ch_power_ifb, if_agc_valb);  
            #endif
            
            for(i = 0; i < ssi_tbl_len; i++)
            {
                if (ifagc_ssi[i].agc_val <= ifagc_ssi[i+1].agc_val)
                {
                    ch_power_takeover = ifagc_ssi[i+1].power_db;
                    break;
                }
            }

	     // Only use IFAGC
	     if(fRFPowerDbm >= 200.0)
	     {
		 	ch_power_takeover = 100.0;
	     }
		 
            #ifdef MS_DEBUG
            printf("ch_power_rf = %f\n", ch_power_rf);
            printf("ch_power_if = %f\n", ch_power_if);
            printf("ch_power_takeover = %f\n", ch_power_takeover);
            #endif

            // ch_power_db = (ch_power_rf > ch_power_if)? ch_power_rf : ch_power_if;

            if(ch_power_rf > (ch_power_takeover + 0.5))
            {
                ch_power_db = ch_power_rf;
            }
            else if(ch_power_if < (ch_power_takeover - 0.5))
            {
                ch_power_db = ch_power_if;
            }
            else
            {
                ch_power_db = (ch_power_if + ch_power_rf)/2;
            }

            // ch_power_db = (ch_power_rf > ch_power_if)? ch_power_if : ch_power_rf;

            ///////// IF-AGC Error for Add. Attnuation /////////////
            if(if_agc_val == 0xff)
            {
#if 0
#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD )
                status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x22, &reg_tmp);
                status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDP_REG_BASE + 0x22, (reg_tmp&0xf0));
#endif
#endif
                // bank 5 0x04 [15] reg_tdp_lat
                status &= _MDrv_DMD_MSB201X_SetReg(devID, TDF_REG_BASE + 0x16, 0x00);                
                status &= _MDrv_DMD_MSB201X_GetReg(devID, TDF_REG_BASE + 0x03, &reg_frz);
                status &= _MDrv_DMD_MSB201X_SetReg(devID, TDF_REG_BASE + 0x03, reg_frz | 0x80);
#if 0
        //#if ( CHIP_FAMILY_TYPE == CHIP_FAMILY_S7LD )
                        // bank 5 0x2c [9:0] reg_agc_error
                        status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x25, &reg_tmp);
                        // if_agc_err = reg_tmp & 0x03;
                        status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x24, &reg_tmp2);
                        // if_agc_err = (if_agc_err << 6)|(reg_tmp >> 2);
        //#else
#endif
                // bank 5 0x2c [9:0] reg_agc_error
                status &= _MDrv_DMD_MSB201X_GetReg(devID, TDF_REG_BASE + 0x19, &reg_tmp);
                // if_agc_err = reg_tmp & 0x03;
                status &= _MDrv_DMD_MSB201X_GetReg(devID, TDF_REG_BASE + 0x18, &reg_tmp2);
                // if_agc_err = (if_agc_err << 6)|(reg_tmp >> 2);
        //#endif

                if(reg_tmp&0x2)
                {
                    if_agc_err = ((((~reg_tmp)&0x03)<<8)|((~reg_tmp2)&0xff)) + 1;
                }
                else
                {
                    if_agc_err = reg_tmp<<8|reg_tmp2;
                }

                // release latch
                status &= _MDrv_DMD_MSB201X_SetReg(devID, TDF_REG_BASE + 0x03, reg_frz);

                for(i = 0; i < err_tbl_len; i++)
                {
                    if ( if_agc_err <= ifagc_err[i].agc_err )        // signed char comparison
                    {
                        ch_power_db += ifagc_err[i].attn_db;
                        break;
                    }
                }
                #ifdef MS_DEBUG
                printf("if_agc_err = 0x%x\n", if_agc_err);
                #endif
                }

                // BY 20110812 temporaily remove ch_power_db += SIGNAL_LEVEL_OFFSET;                
        }      
    }
    else
    {
        #ifdef MS_DEBUG
        if (fRFPowerDbm>=100.0) // unreasonable input value, get RF level from RFAGG
        {
            printf("Error!! please add AGC table\n");
        }
        #endif
        ch_power_db = fRFPowerDbm;
    }        
    *fRFPowerDbmResult=ch_power_db;
    return status;
}

//MS_BOOL MDrv_DMD_MSB201X_SPI_Pre_Setting(MS_U8 devID, sDMD_MSB201X_InitData *pDMD_MSB201X_InitData)
MS_BOOL MDrv_DMD_MSB201X_SPI_Pre_Setting(MS_U8 devID)
{
//	tMSB201X_Demod_Data *pDemod; 
	MS_U8 i;
    tMSB201X_Demod_Data *pMSB201X = NULL;
    {
        pstDemod = (tMSB201X_Demod_Data *)malloc(sizeof(tMSB201X_Demod_Data) * DEMOD_MAX_INSTANCE);
        if(NULL == pstDemod)
        {
            return FALSE;
        }
        else
        {
            for(i=0; i< DEMOD_MAX_INSTANCE; i++)
            {
                pMSB201X = (pstDemod + i);
                memcpy(pMSB201X, &MSB201X_Demod_Init, sizeof(tMSB201X_Demod_Data));
            }
        }
    }

       pMSB201X = pstDemod + devID;
       //pMSB201X->_sDMD_MSB201X_InitData.fpMSB201X_I2C_Access = pDMD_MSB201X_InitData->fpMSB201X_I2C_Access;
       pMSB201X->DSP_ReadWrite_Mode = E_MSB201X_SPI_READ_WRITE;

//	   pDemod = DEMOD_GET_ACTIVE_NODE(devID);

//	   _MSB201X_I2C_CH_Reset(devID, 3);
	
      //pMSB201X->DSP_ReadWrite_Mode = E_MSB201X_SPI_READ_WRITE;  //for inseting the SPI and I2C mode

    // DMD HK init clk start
// -------------------------------------------------------------------
// Initialize DMD_ANA_MISC
// -------------------------------------------------------------------

    // Koln add
    // [0]	reg_pd_ldo25i_ana   // 2.5v LDO power down
    // [1]	reg_pd_ldo25q_ana   // 2.5v LDO power down
    // [2]	reg_pd_ldo25i_dig   // 2.5v LDO power down
    // [3]	reg_pd_ldo25q_dig   // 2.5v LDO power down
    // [4]	reg_pd_ldo25_ref    // 2.5v LDO power down
    // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h53, 2'b01, 16'h0000);
    //wreg 4106 0x53 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x53)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x53)*2+1,  0x00);

    // [0]	reg_tst_ldo25i
    // [1]	reg_tst_ldo25q
    // [5:4]	reg_tst_ldo25i_selfb
    // [7:6]	reg_tst_ldo25q_selfb
    // [8]	reg_pd_dm2p5ldoi = 1'b0
    // [9]	reg_pd_dm2p5ldoq = 1'b0
    // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h4f, 2'b11, 16'h0000);
    // wreg 4106 0x4f 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x4f)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x4f)*2+1,  0x00);

    // [0]	reg_tst_ldo11_clk
    // [1]	reg_tst_ldo26
    // [2]	reg_tst_ldo11_cmp
    // [3]	reg_pd_dm1p1ldo_clk = 1'b0
    // [4]	reg_pd_dm1p1ldo_cmp = 1'b0
    // [6]	reg_tst_ldo26_selfb
    // [7]	reg_pd_dm2p6ldo = 1'b0
    // [9:8]	reg_tst_ldo11_cmp_selfb
    // [11:10]	reg_tst_ldo11_clk_selfb
    // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h4e, 2'b11, 16'h0000);
    //wreg 4106 0x4e 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x4e)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x4e)*2+1,  0x00);


    // [1:0]	reg_mpll_loop_div_first       feedback divider 00:div by 1 01:div by 2 10:div by 4 11:div by 8
    // [15:8]	reg_mpll_loop_div_second      feedback divider, div by binary data number
    // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h33, 2'b11, 16'h1201);  // Loop divider ; VCO = 24*(2^2)*9 = 864
//    wreg 4106 0x33 0x1201
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x33)*2  ,  0x01);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x33)*2+1,  0x12);

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
    // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h35, 2'b11, 16'h1803); // MPLL reset
    //wreg 4106 0x35 0x1803
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x35)*2  ,  0x03);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x35)*2+1,  0x18);    
    
    // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h35, 2'b11, 16'h0003); // release MPLl reset
    //wreg 4106 0x35 0x0003
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x35)*2  ,  0x03);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x35)*2+1,  0x00);    

    // [0]	reg_mpll_clk_dp_pd	dummy
    // [1]	reg_adc_clk_pd		ADC output clock power down
    // [2]	reg_mpll_div2_pd	MPLL_DIV2 power down
    // [3]	reg_mpll_div3_pd	MPLL_DIV3 power down
    // [4]	reg_mpll_div4_pd	MPLL_DIV4 power down
    // [5]	reg_mpll_div8_pd	MPLL_DIV8 power down
    // [6]	reg_mpll_div10_pd	MPLL_DIV10 power down
    // [13:8]  reg_mpll_adc_div_sel	select the ADC clock divide ratio,ADC clk=XTAL_IN
      // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h30, 2'b11, 16'h1e00);  // divide ADC clock to 28.8Mhz = 24*36/30
      //wreg 4106 0x30 0x1e00
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x30)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x30)*2+1,  0x1e);    


    //$display("--------------------------------------");
    //$display("Initialize ADC I/Q");
    //$display("--------------------------------------");

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
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h01, 2'b11, 16'h0440); // Set IMUXS QMUXS
        //wreg 4106 0x01 0x0440
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x01)*2  ,  0x40);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x01)*2+1,  0x04);    

    // [2:0]	reg_imuxs_s
    // [6:4]	reg_qmuxs_s
    // [9:8]	reg_iclpstr_s
    // [13:12]	reg_qclpstr_s
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h45, 2'b01, 16'h0000); // Set IMUXS QMUXS
        //wreg 4106 0x45 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x45)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x45)*2+1,  0x00);    

    
    // [0]	Channel I ADC power down: 1=power dwon
    // [1]	Channel Q ADC power down: 1=power dwon
    // [2]	power down clamp buffer for test mode
    // [3]	change ADC reference voltage for SSIF
    // [6:4]    ADC source bias current control
    // [9:8]    XTAL receiver amp gain
        // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h0c, 2'b11, 16'h0000); // Set enable ADC clock
    //    wreg 4106 0x0c 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x0c)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x0c)*2+1,  0x00);    

    
    // [0]	reg_linear_cal_start_q	0	0	1
    // [1]	reg_linear_cal_mode_q	0	0	1
    // [2]	reg_linear_cal_en_q	0	0	1
    // [3]	reg_linear_cal_code0_oren_q	0	0	1
    // [6:4]	reg_linear_cal_status_sel_q	2	0	3
    // [7]	reg_pwdn_vcalbuf	0	0	1

      // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h0f, 2'b01, 16'h0000); // Set reg_pwdn_vcalbuf = 1'b0
 //     wreg 4106 0x0f 0x0000
 	   _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x0f)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x0f)*2+1,  0x00);    


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
      // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h20, 2'b11, 16'h0000); // Disable PWDN_REF
      //wreg 4106 0x20 0x0000
 	   _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x20)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x20)*2+1,  0x00);    
    // Set ADC gain is 1
      // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h0b, 2'b11, 16'h0505);
      //wreg 4106 0x0b 0x0505
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x0b)*2  ,  0x05);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x0b)*2+1,  0x05);    

    // Disable ADC Sign bit
      // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h2e, 2'b11, 16'h0000);
      //wreg 4106 0x2e 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x2e)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x2e)*2+1,  0x00);    

    // ADC I channel offset
      // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h2a, 2'b11, 16'h0c00);
      //wreg 4106 0x2a 0x0c00
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x2a)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x2a)*2+1,  0x0c); 

    // ADC Q channel offset
      // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h2b, 2'b11, 16'h0c00);
      //wreg 4106 0x2b 0x0c00
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x2b)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100A00 + (0x2b)*2+1,  0x0c); 

    
        // [5:0]reg_ckg_mcu
        // [6]	reg_power_good_mask
        // [11:8]reg_ckg_inner
	// [15:12]reg_ckg_iicm1
    	// `RIU_W((`RIUBASE_TOP>>1)+7'h0b, 2'b11, 16'h0430);
    	//wreg 4105 0x0b 0x0030
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x2b)*2  ,  0x30);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x2b)*2+1,  0x00); 
    
    
        // [1:0]reg_chanout_sel
        // [2]	reg_iq_filter_enable	= 1
        // [3]	reg_iq_filter_sel
        // [5:4]reg_adc_debug_clk_sel
    	// `RIU_W((`RIUBASE_ADCPLL>>1)+7'h17, 2'b11, 16'h0004);
    	//wreg 4106 0x17 0x0004
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x17)*2  ,  0x04);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x17)*2+1,  0x00); 
    


    // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h51, 2'b01, 16'h0081); // 2 channel DVBC
    //wreg 4106 0x51 0x0081
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x51)*2  ,  0x81);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x51)*2+1,  0x00); 


// -------------------------------------------------------------------
// Release clock gating
// -------------------------------------------------------------------
    //$display("--------------------------------------");
    //$display("Release clock gating");
    //$display("--------------------------------------");

    // [0]	reg_xtal_en
    // [9:8]	reg_clk_pd_iic
    // [10]	reg_clk_pd_all
      // `RIU_W((`RIUBASE_TOP>>1)+7'h09, 2'b11, 16'h0101);
      //wreg 4105 0x09 0x0101
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x09)*2  ,  0x01);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x09)*2+1,  0x01); 

    // [3:0]	reg_ckg_adcd
    // [7:4]	reg_ckg_sadc
    // [11:8]	reg_ckg_iicm
    // [13:12]	reg_ckg_sbus
      // `RIU_W((`RIUBASE_TOP>>1)+7'h0a, 2'b11, 16'h0000);
      //wreg 4105 0x0a 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x0a)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x0a)*2+1,  0x00); 

    // [5:0]	reg_ckg_mcu
    // [6]	reg_ckg_live
    // [11:8]	reg_ckg_inner
      // `RIU_W((`RIUBASE_TOP>>1)+7'h0b, 2'b11, 16'h0030);
//      wreg 4105 0x0b 0x0030
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x0b)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x0b)*2+1,  0x00); 


    // @0x0912
    // [3:0]	reg_ckg_dvbtm_ts
    // [4]	reg_dvbtm_ts_out_mode
    // [5]	reg_dvbtm_ts_clk_pol
    // [15:8]	reg_dvbtm_ts_clk_divnum
      // `RIU_W((`RIUBASE_TOP>>1)+7'h12, 2'b11, 16'h1418);
      //wreg 4105 0x12 0x1418
     //_MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2  ,  0x18);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2  ,  0x10);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x12)*2+1,  0x14); 

    // @0x0913
    // [5:0]	reg_ckg_spi
      // `RIU_W((`RIUBASE_TOP>>1)+7'h13, 2'b11, 16'h0020);
      //wreg 4105 0x13 0x0020
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x13)*2  ,  0x20);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x13)*2+1,  0x00); 


    // @0x091b
    // [10:8]	reg_ckg_syn_miu
    // [14:12]	reg_ckg_syn_ts
      // `RIU_W((`RIUBASE_TOP>>1)+7'h1b, 2'b11, 16'h0000);
//      wreg 4105 0x1b 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x1b)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x1b)*2+1,  0x00); 



    // @0x091c
    // [4:0]	reg_ckg_bist
    // [11:8]	reg_ckg_adcd_d2
      // `RIU_W((`RIUBASE_TOP>>1)+7'h1c, 2'b11, 16'h0000);
      //wreg 4105 0x1c 0x0000

     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x1c)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x1c)*2+1,  0x00);


    // [1:0]	reg_iicm_pad_sel
    // [4]	reg_i2c_sbpm_en
    // [12:8]	reg_i2c_sbpm_idle_num
      // `RIU_W((`RIUBASE_TOP>>1)+7'h08, 2'b01, 16'h0a01);
      //wreg 4105 0x08 0x0a01

     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x08)*2  ,  0x01);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x08)*2+1,  0x0a);

    // [8]	reg_turn_off_pad
      // `RIU_W((`RIUBASE_TOP>>1)+7'h28, 2'b10, 16'h0000);
     // wreg 4105 0x28 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x28)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x28)*2+1,  0x00);




    //$display("--------------------------------------");
    //$display("Initialize Transport Stream synthesizer and APLL");
    //$display("--------------------------------------");

    // ////////////////////////////////////////////////////
    //
    //	According to analog APLL designer's suggest:
    //	APLL_LOOP_DIV = 5'b00000
    //	apll input frequency range 54MHz~106MHz synthesizer clock
    //	so apll_1x_out = synthesizer_out * (apll_ts_mode + 1)
    //
    //	=> apll_1x_out should 40Mhz ~ 130Mhz
    //
    //	Current setting:
    //	apll_1x_out = (432/8.0) * (1+1) = 108MHz
    //	choose reg_ckg_ts_apll_div[2:0] = 3'd4
    //	ts_clk_apll_div = 108/(2^4) = 6.75MHz
    //
    // ////////////////////////////////////////////////////


    // [15:0]	reg_synth_set[15: 0]
    // [ 7:0]	reg_synth_set[23:16]
      // `RIU_W((`RIUBASE_TOP>>1)+7'h51, 2'b11, 16'h0000);
      //wreg 4105 0x51 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x51)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x51)*2+1,  0x00);
      // `RIU_W((`RIUBASE_TOP>>1)+7'h52, 2'b11, 16'h0040);
      //wreg 4105 0x52 0x0040
		 _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x52)*2  ,  0x40);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x52)*2+1,  0x00);


    // [0]	reg_synth_reset
    // [1]	reg_synth_ssc_en
    // [2]	reg_synth_ssc_mode
    // [4]	reg_synth_sld
      // `RIU_W((`RIUBASE_TOP>>1)+7'h50, 2'b01, 16'h0010);
      //wreg 4105 0x50 0x0010

		 _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x50)*2  ,  0x10);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x50)*2+1,  0x00);

    // #10_000;
    //delay 0  ****


    // [1:0]	reg_apll_loop_div_first
    // [15:8]	reg_apll_loop_div_second
      // `RIU_W((`RIUBASE_TOP>>1)+7'h57, 2'b11, 16'h0000);
     //wreg 4105 0x57 0x0000
     
		 _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x57)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x57)*2+1,  0x00);


    // [0]	reg_apll_pd
    // [1]	reg_apll_reset
    // [2]	reg_apll_porst
    // [3]	reg_apll_vco_offset
    // [4]	reg_apll_en_ts
    // [5]	reg_apll_endcc
    // [6]	reg_apll_clkin_sel
    // [8]	reg_apll_ts_mode
      // `RIU_W((`RIUBASE_TOP>>1)+7'h55, 2'b11, 16'h0100);
      //wreg 4105 0x55 0x0100
		 _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x55)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x55)*2+1,  0x01);
      // `RIU_W((`RIUBASE_TOP>>1)+7'h55, 2'b01, 16'h0110);
      //wreg 4105 0x55 0x0110
		 _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x55)*2  ,  0x10);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x55)*2+1,  0x01);

    // [16:0]	reg_apll_test
      // `RIU_W((`RIUBASE_TOP>>1)+7'h59, 2'b11, 16'h0000);
      //wreg 4105 0x59 0x0000     
		 _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x59)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x59)*2+1,  0x00);

    // 0x0920
    // [3:0]	reg_ckg_ts_apll_div[2:0]
      // `RIU_W((`RIUBASE_TOP>>1)+7'h20, 2'b01, 16'h0004);
      //wreg 4105 0x20 0x0004
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x20)*2  ,  0x04);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x20)*2+1,  0x00);

		//  Following register control by reg_CLKGEN1
		//        reg_ckg_atsc_dvb_div_sel,  (clkgen0)
		//        reg_ckg_atsc_dvbtc_ts_inv, (clkgen0)
		//        reg_ckg_atsc_ts,           (clkgen0)
		//	reg_ckg_demod_test_in_en,  (clkgen0, clkgen_dmd)
		//        reg_ckg_dmdmcu,            (clkgen0)
		//        reg_ckg_dvbtc_adc,         (clkgen0, clkgen_dmd)
		//	reg_ckg_dvbtc_ts,          (clkgen0)
		//        reg_ckg_dvbtm_ts_divnum,   (clkgen0)
		//        reg_ckg_dvbtm_ts_out_mode, (clkgen0)
		//  reg_ckg_vifdbb_dac,        (clkgen0, clkgen_dmd)
		//	reg_ckg_vifdbb_vdac,       (clkgen0, clkgen_dmd)
		
		
		
		//$display("Set register at TOP (clkgen) ......");
		
		// { 1'b0, reg_ckg_adcd1[3:0], reg_clk_pd_all, 1'b0, reg_clk_pd_iic, 7'h0, reg_xtal_en }
		// `RIU_W((`RIUBASE_TOP>>1)+7'h09, 2'b10, 16'h0000);	        
		//wreg 4105 0x09 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x09)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x09)*2+1,  0x00);		
		// { 2'h0, reg_ckg_sbus[1:0], reg_ckg_iicm[3:0], reg_ckg_sadc[3:0], reg_ckg_adcd[3:0] }
		// `RIU_W((`RIUBASE_TOP>>1)+7'h0a, 2'b01, 16'h1110);	        
//		wreg 4105 0x0a 0x1110
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x0a)*2  ,  0x10);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x0a)*2+1,  0x11);
     		
		// { reg_ckg_demod_mpll[3:0], 4'h0, reg_ckg_dmdxtali[3:0], reg_ckg_dmdmcu[3:0] }
		// `RIU_W((`RIUBASE_TOP>>1)+7'h0d, 2'b10, 16'h0000);	
		//wreg 4105 0x0d 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x0d)*2  ,  0x00);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x0d)*2+1,  0x00);		

		// DVBC : 24*36/30=28.8 MHz
		// `RIU_W((`RIUBASE_ADCPLL>>1)+7'h33, 2'b11, 16'h1201);       // Set MPLL_LOOP_DIV_FIRST and SECOND
		//wreg 4106 0x33 0x1201
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x33)*2  ,  0x01);   
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x33)*2+1,  0x12);				
     // `RIU_W((`RIUBASE_ADCPLL>>1)+7'h30, 2'b11, 16'h1e00+16'h1); // Set MPLL_ADC_DIV_SEL
		
		// reg_ckg_ts_0 = 4'd0;
		// reg_ckg_ts_1 = 4'd0;
		// `RIU_W((`RIUBASE_TOP>>1)+7'h23, 2'b01, 16'h0000);
		//wreg 4105 0x23 0x0000
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x23)*2  ,  0x00);   
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100900 + (0x23)*2+1,  0x00);
     				
		//$display("=================================================");
		//$display("start demod atop ADC setting ......");
		//$display("=================================================");
		
		// { 8'h0, reg_ana_setting_sel[3:0], 3'h0, reg_ana_setting_enable } )
		// `RIU_W((`RIUBASE_ADCPLL>>1)+7'h51, 2'b01, 16'h0081);	
		//wreg 4106 0x51 0x0081
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x51)*2  ,  0x81);
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x51)*2+1,  0x00);		
		
		//if agc enable
		//wreg 4106 0x18 0x0101  
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x18)*2  ,  0x01); 		
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x18)*2+1,  0x01);			
		
		//wreg 4106 0x30 0x1200
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x30)*2  ,  0x00); 		
     _MDrv_DMD_MSB201X_SetReg(devID, 0x100a00 + (0x30)*2+1,  0x12);		


      free(pstDemod);
      pstDemod=NULL;
      return TRUE;
}

MS_BOOL MDrv_DMD_MSB201X_GetRFLevel(MS_U8 devID, MS_U8 u8DemodIndex, float *fRFPowerDbmResult, float fRFPowerDbm)
{
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);
	
    DMD_LOCK();

    _MDrv_DMD_MSB201X_GetRFLevel(devID, u8DemodIndex, fRFPowerDbmResult, fRFPowerDbm,
                                                     pDemod->_sDMD_MSB201X_InitData.IFAGC_Data[u8DemodIndex].pIFAGC_SSI, pDemod->_sDMD_MSB201X_InitData.IFAGC_Data[u8DemodIndex].IFAGC_SSI_Length,
                                                     pDemod->_sDMD_MSB201X_InitData.IFAGC_Data[u8DemodIndex].pIFAGC_ERR, pDemod->_sDMD_MSB201X_InitData.IFAGC_Data[u8DemodIndex].IFAGC_ERR_Length);

    DMD_UNLOCK();

    return TRUE;
}

MS_BOOL _MDrv_DMD_MSB201X_GetLock(MS_U8 devID, MS_U8 u8DemodIndex, eDMD_MSB201X_GETLOCK_TYPE eType, float fCurrRFPowerDbm, float fNoChannelRFPowerDbm, MS_U32 u32TimeInterval)
{
    MS_U16 u16Address = 0;
    MS_U8 cData = 0;
    MS_U8 cBitMask = 0;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);

    _MDrv_DMD_MSB201X_Select_Demod_RIU(devID, u8DemodIndex);
    _MSB201X_I2C_CH_Reset(devID, 5);
#if 0
    if (fCurrRFPowerDbm < 100.0f)
    {
        if (eType == DMD_DVBC_GETLOCK_NO_CHANNEL)
        {
            MDrv_SYS_DMD_VD_MBX_ReadReg(FEC_REG_BASE + 0xE0, &cData);
            if (cData > 5)
            {
                bDMD_DVBC_NoChannelDetectedWithRFPower = FALSE;
                u32DMD_DVBC_NoChannelTimeAccWithRFPower = 0;
            }
            else
            {
                if ((fCurrRFPowerDbm<fNoChannelRFPowerDbm) && (u32DMD_DVBC_NoChannelTimeAccWithRFPower<10000))
                {
                    u32DMD_DVBC_NoChannelTimeAccWithRFPower+=u32TimeInterval;
                }
                if (u32DMD_DVBC_NoChannelTimeAccWithRFPower>1500)
                {
                    bDMD_DVBC_NoChannelDetectedWithRFPower=1;
                    #ifdef MS_DEBUG
                    printf("INTERN_DVBC_GetLock:DMD_DVBC_GETLOCK_NO_CHANNEL Detected Detected Detected!!\n");
                    #endif
                    return TRUE;
                }
            }
            #ifdef MS_DEBUG
            printf("INTERN_DVBC_GetLock:DMD_DVBC_GETLOCK_NO_CHANNEL FSM:%d InputPower:%f Threshold:%f Interval:%ld TimeAcc:%ld NoChannelDetection:%d\n",cData, fCurrRFPowerDbm, fNoChannelRFPowerDbm, u32TimeInterval, u32DMD_DVBC_NoChannelTimeAccWithRFPower, bDMD_DVBC_NoChannelDetectedWithRFPower);
            #endif
        }
    }
#endif

    {
        switch( eType )
        {
            case DMD_DVBC_GETLOCK_FEC_LOCK:
                _MDrv_DMD_MSB201X_GetReg(devID, FEC_REG_BASE + 0xE0, &cData);
  
                printf(" @_MDrv_DMD_MSB201X_GetLock DevID = %d, Channel = %d, FSM 0x%x\n",devID, u8DemodIndex, cData);
                if (cData == 0x0C)
                {
                    if(pDemod->DMD_Lock_Status[u8DemodIndex]== 0)
                    {
                      pDemod->DMD_Lock_Status[u8DemodIndex] = 1;
                      printf("[Demod]lock++++\n");

                    }
                    return TRUE;
                }
                else
                {
                    if(pDemod->DMD_Lock_Status[u8DemodIndex] == 1)
                    {
                      pDemod->DMD_Lock_Status[u8DemodIndex] = 0;
                      printf("[Demod]unlock----\n");
                    }
                    return FALSE;
                }
                break;

            case DMD_DVBC_GETLOCK_PSYNC_LOCK:
                u16Address =  FEC_REG_BASE + 0x2C; //FEC: P-sync Lock,
                cBitMask = _BIT1;
                break;

            case DMD_DVBC_GETLOCK_DCR_LOCK:
                u16Address =  TDP_REG_BASE + 0x45; //DCR Lock,
                cBitMask = _BIT0;
                break;

            case DMD_DVBC_GETLOCK_AGC_LOCK:
                u16Address =  TDP_REG_BASE + 0x2F; //AGC Lock,
                cBitMask = _BIT0;
                break;

            case DMD_DVBC_GETLOCK_NO_CHANNEL:
                u16Address =  TOP_REG_BASE + 0xC3; //no channel,
                cBitMask = _BIT2 | _BIT3 | _BIT4;
                break;

            case DMD_DVBC_GETLOCK_ATV_DETECT:
                u16Address =  TOP_REG_BASE + 0xC4; //ATV detection,
                cBitMask = _BIT1; // check atv
                break;

            case DMD_DVBC_GETLOCK_TR_LOCK:
                #if 0 // 20111108 temporarily solution
                u16Address =  INNC_REG_BASE + 0x0A*2 + 1; //TR lock indicator,
                cBitMask = _BIT4;
                break;
                #endif
            case DMD_DVBC_GETLOCK_TR_EVER_LOCK:
                u16Address =  TOP_REG_BASE + 0xC4; //TR lock indicator,
                cBitMask = _BIT4;
                break;

            default:
                return FALSE;
        }

        if (_MDrv_DMD_MSB201X_GetReg(devID, u16Address, &cData) == FALSE)
            return FALSE;

        if ((cData & cBitMask) != 0)
        {
            return TRUE;
        }

        return FALSE;
    }

    return FALSE;
}

MS_BOOL MDrv_DMD_MSB201X_GetLock(MS_U8 devID, MS_U8 u8DemodIndex, eDMD_MSB201X_GETLOCK_TYPE eType, eDMD_MSB201X_LOCK_STATUS *eLockStatus)
{
    return MDrv_DMD_MSB201X_GetLockWithRFPower(devID, u8DemodIndex, eType, eLockStatus, 200.0f, -200.0f);
}

MS_BOOL MDrv_DMD_MSB201X_GetLockWithRFPower(MS_U8 devID, MS_U8 u8DemodIndex, eDMD_MSB201X_GETLOCK_TYPE eType, eDMD_MSB201X_LOCK_STATUS *eLockStatus, float fCurrRFPowerDbm, float fNoChannelRFPowerDbm)
{
    MS_U32 u32CurrScanTime=0;
    MS_BOOL bRet=FALSE;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);
	
    DMD_LOCK();
    if ( eType == DMD_DVBC_GETLOCK ) // for channel scan
    {
        if (_MDrv_DMD_MSB201X_GetLock(devID, u8DemodIndex, DMD_DVBC_GETLOCK_FEC_LOCK, 200.0f, -200.0f, 0))
        {
            *eLockStatus = DMD_DVBC_LOCK;
        }
        else
        {
            MS_U32 u32Timeout = 10000;//(sDMD_DVBC_Info.u16SymbolRate) ? 5000 : 15000;
            if (pDemod->sDMD_MSB201X_Info[u8DemodIndex].u16SymbolRate == 0)
            {
                u32Timeout = u16DMD_DVBC_AutoSymbol_Timeout;
            }
            else
            {
                u32Timeout = (pDemod->sDMD_MSB201X_Info[u8DemodIndex].eQamMode == DMD_DVBC_QAMAUTO) ? u16DMD_DVBC_FixSymbol_AutoQam_Timeout : u16DMD_DVBC_FixSymbol_FixQam_Timeout;
            }

            u32CurrScanTime=MsOS_GetSystemTime();
            if (u32CurrScanTime - pDemod->sDMD_MSB201X_Info[u8DemodIndex].u32ChkScanTimeStart < u32Timeout)
            {
                if (pDemod->u32DMD_DVBC_ScanCount[u8DemodIndex]==0)
                {
                    pDemod->u32DMD_DVBC_PrevScanTime[u8DemodIndex]=u32CurrScanTime;
                    pDemod->u32DMD_DVBC_ScanCount[u8DemodIndex]++;
                }

                bRet=_MDrv_DMD_MSB201X_GetRFLevel(devID, u8DemodIndex, &fCurrRFPowerDbm, fCurrRFPowerDbm,
                                                     pDemod->_sDMD_MSB201X_InitData.IFAGC_Data[u8DemodIndex].pIFAGC_SSI, pDemod->_sDMD_MSB201X_InitData.IFAGC_Data[u8DemodIndex].IFAGC_SSI_Length,
                                                     pDemod->_sDMD_MSB201X_InitData.IFAGC_Data[u8DemodIndex].pIFAGC_ERR, pDemod->_sDMD_MSB201X_InitData.IFAGC_Data[u8DemodIndex].IFAGC_ERR_Length);

                if (_MDrv_DMD_MSB201X_GetLock(devID, u8DemodIndex, DMD_DVBC_GETLOCK_NO_CHANNEL, fCurrRFPowerDbm, fNoChannelRFPowerDbm, u32CurrScanTime-pDemod->u32DMD_DVBC_PrevScanTime[u8DemodIndex]))
                {
                    #ifdef MS_DEBUG
                    printf("%s %ld UNLOCK:NO_CHANNEL\n", __FUNCTION__, u32CurrScanTime-pDemod->sDMD_MSB201X_Info[u8DemodIndex].u32ChkScanTimeStart);
                    #endif
                    *eLockStatus = DMD_DVBC_UNLOCK;
                }
                else if (_MDrv_DMD_MSB201X_GetLock(devID, u8DemodIndex, DMD_DVBC_GETLOCK_ATV_DETECT, 200.0f, -200.0f, 0))
                {
                    #ifdef MS_DEBUG
                    printf("%s %ld UNLOCK:ATV_DETECT\n", __FUNCTION__, u32CurrScanTime-pDemod->sDMD_MSB201X_Info[u8DemodIndex].u32ChkScanTimeStart);
                    #endif
                    *eLockStatus = DMD_DVBC_UNLOCK;
                }
#ifdef NEW_TR_MODULE
                else if ((_MDrv_DMD_MSB201X_GetLock(devID, u8DemodIndex, DMD_DVBC_GETLOCK_TR_EVER_LOCK, 200.0f, -200.0f, 0) == FALSE) && ((u32CurrScanTime - pDemod->sDMD_MSB201X_Info[u8DemodIndex].u32ChkScanTimeStart) > 500))
#else
                else if ((_MDrv_DMD_MSB201X_GetLock(devID, u8DemodIndex, DMD_DVBC_GETLOCK_TR_EVER_LOCK, 200.0f, -200.0f, 0) == FALSE) && ((u32CurrScanTime - pDemod->sDMD_MSB201X_Info[u8DemodIndex].u32ChkScanTimeStart) > 5000))
#endif
                {
                    #ifdef MS_DEBUG
                    printf("%s %ld UNLOCK:TR\n", __FUNCTION__, u32CurrScanTime-pDemod->sDMD_MSB201X_Info[u8DemodIndex].u32ChkScanTimeStart);
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
                printf("%s %ld UNLOCK:TIMEOUT %ld\n", __FUNCTION__, MsOS_GetSystemTime(), u32Timeout);
                #endif
                *eLockStatus = DMD_DVBC_UNLOCK;
            }
            pDemod->u32DMD_DVBC_PrevScanTime[u8DemodIndex]=u32CurrScanTime;
        }
		
    }
    else
    {
        if (_MDrv_DMD_MSB201X_GetLock(devID, u8DemodIndex, eType, 200.0f, -200.0f, 0) == TRUE)
        {
            *eLockStatus = DMD_DVBC_LOCK;
        }
        else
        {
            *eLockStatus = DMD_DVBC_UNLOCK;
        }
    }
    DMD_UNLOCK();

    bRet=TRUE;
    return bRet;
}

MS_BOOL MDrv_DMD_MSB201X_Demod_GetLock(MS_U8 devID, MS_U8 u8DemodIndex)
{
    MS_U8		cData;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);
    DMD_LOCK();

    _MDrv_DMD_MSB201X_Select_Demod_RIU(devID, u8DemodIndex);
	
    printf(" MDrv_DMD_MSB201X_Demod_GetLock : devID = %d, Channel index = %d \n", (int)devID, (int)u8DemodIndex);

    _MSB201X_I2C_CH_Reset(devID, 5);

// Only check demod lock or not!
    _MDrv_DMD_MSB201X_GetReg(devID, FEC_REG_BASE + 0xE0, &cData);
    printf(" MDrv_DMD_MSB201X_Demod_GetLock FSM 0x%x\n",cData);
    if(cData == 0x0C)
    {
    	if(g_dvbc_lock[devID][u8DemodIndex] == 0)
    	{
    		g_dvbc_lock[devID][u8DemodIndex] = 1;
		printf(" MDrv_DMD_MSB201X_Demod_GetLock : Demod Lock! \n");
    	}
	_MSB201X_I2C_CH_Reset(devID, 3);
	return TRUE;
    }
    else
    {
    	if(g_dvbc_lock[devID][u8DemodIndex] == 1)
    	{
    		g_dvbc_lock[devID][u8DemodIndex] = 0;
		printf(" MDrv_DMD_MSB201X_Demod_GetLock : Demod Unlock! \n");
    	}
	_MSB201X_I2C_CH_Reset(devID, 3);
	return FALSE;
    }

    DMD_UNLOCK();
    return FALSE;
}

MS_BOOL MDrv_DMD_MSB201X_Set_TSOut(MS_U8 devID, sDMD_MSB201X_TS_Param *pDMD_MSB201X_TS_Param)
{
	tMSB201X_Demod_Data *pDemod=NULL; 
	pDemod= DEMOD_GET_ACTIVE_NODE(devID);
    	DMD_LOCK();

	_MDrv_DMD_MSB201X_Set_TSOut(devID, pDMD_MSB201X_TS_Param);

	DMD_UNLOCK();
	return TRUE;
}

MS_BOOL MDrv_DMD_MSB201X_CfgExtHeader(MS_U8 devID, MS_U8 u8DemodIndex,  sDMD_MSB201X_extHeader *pDMD_MSB201X_extHeader_Param)
{
	MS_BOOL bRet = TRUE;
	tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);
    	DMD_LOCK();

	bRet = _MDrv_DMD_MSB201X_CfgExtHeader(devID, u8DemodIndex, pDMD_MSB201X_extHeader_Param);

	DMD_UNLOCK();
	return bRet;
}

MS_BOOL MDrv_DMD_MSB201X_CfgCIHeader(MS_U8 devID, MS_U8 u8DemodIndex,  sDMD_MSB201X_CIHeader *pDMD_MSB201X_CIHeader_Param)
{
	MS_BOOL bRet = TRUE;
	tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);
    	DMD_LOCK();

	bRet = _MDrv_DMD_MSB201X_CfgCIHeader(devID, u8DemodIndex, pDMD_MSB201X_CIHeader_Param);

	DMD_UNLOCK();
	return bRet;
}

MS_BOOL _MDrv_DMD_MSB201X_GetCurrentModulationType(MS_U8 devID, MS_U8 u8DemodIndex, eDMD_MSB201X_MODULATION_TYPE *pQAMMode)
{
    MS_U8 u8Data=0;

    _MDrv_DMD_MSB201X_Select_Demod_RIU(devID, u8DemodIndex);
    _MSB201X_I2C_CH_Reset(devID, 5);
	
    _MDrv_DMD_MSB201X_GetReg(devID, EQE_REG_BASE + 0x02, &u8Data);

    switch(u8Data&0x07)
    {
        case 0:
            *pQAMMode = DMD_DVBC_QAM16;
            return TRUE;
             break;
        case 1:
            *pQAMMode = DMD_DVBC_QAM32;
            return TRUE;
            break;
        case 2:
            *pQAMMode = DMD_DVBC_QAM64;
            return TRUE;
            break;
        case 3:
            *pQAMMode = DMD_DVBC_QAM128;
            return TRUE;
            break;
        case 4:
            *pQAMMode = DMD_DVBC_QAM256;
            return TRUE;
            break;
        default:
            *pQAMMode = DMD_DVBC_QAMAUTO;
            return FALSE;
    }
}

MS_BOOL _MDrv_DMD_MSB201X_GetCurrentSymbolRate(MS_U8 devID, MS_U8 u8DemodIndex, MS_U16 *u16SymbolRate)
{
    MS_U8  tmp = 0;
    MS_U16 u16SymbolRateTmp = 0;

    _MDrv_DMD_MSB201X_Select_Demod_RIU(devID, u8DemodIndex);
    _MSB201X_I2C_CH_Reset(devID, 5);

    // intp
    _MDrv_DMD_MSB201X_GetReg(devID, TOP_REG_BASE + 0xd2, &tmp);
    u16SymbolRateTmp = tmp;
    _MDrv_DMD_MSB201X_GetReg(devID, TOP_REG_BASE + 0xd1, &tmp);
    u16SymbolRateTmp = (u16SymbolRateTmp<<8)|tmp;

    if (abs(u16SymbolRateTmp-6900)<2)
    {
        u16SymbolRateTmp=6900;
    }

    if (abs(u16SymbolRateTmp-6875)<2)
    {
        u16SymbolRateTmp=6875;
    }

    *u16SymbolRate = u16SymbolRateTmp;

    return TRUE;
}

MS_BOOL _MDrv_DMD_MSB201X_Get_FreqOffset(MS_U8 devID, MS_U8 u8DemodIndex, float *pFreqOff, MS_U8 u8BW)
{
    MS_U16      FreqB, config_Fc=0;
    float       FreqCfo_offset,f_Fc;
    MS_U32      RegCfo_offset, Reg_Fc_over_Fs;
    //MS_U8       reg_frz = 0, reg = 0;
    MS_U8       reg = 0;
    MS_BOOL     status = TRUE;

    // no use.
    u8BW = u8BW;

    _MDrv_DMD_MSB201X_Select_Demod_RIU(devID, u8DemodIndex);
    _MSB201X_I2C_CH_Reset(devID, 5);
	
    // bank 2c 0x3d [0] reg_bit_err_num_freeze
    status &= _MDrv_DMD_MSB201X_SetReg(devID, EQE_REG_BASE + 0x3A, 0x20);
    status &= _MDrv_DMD_MSB201X_SetReg(devID, INNC_REG_BASE + 0x05, 0x80);

    status &= _MDrv_DMD_MSB201X_GetReg(devID, EQE_REG_BASE + 0x43, &reg);
    RegCfo_offset = reg;
    status &= _MDrv_DMD_MSB201X_GetReg(devID, EQE_REG_BASE + 0x42, &reg);
    RegCfo_offset = (RegCfo_offset<<8)|reg;
    status &= _MDrv_DMD_MSB201X_GetReg(devID, EQE_REG_BASE + 0x41, &reg);
    RegCfo_offset = (RegCfo_offset<<8)|reg;
    status &= _MDrv_DMD_MSB201X_GetReg(devID, EQE_REG_BASE + 0x40, &reg);
    RegCfo_offset = (RegCfo_offset<<8)|reg;

    // bank 2c 0x3d [0] reg_bit_err_num_freeze
    status &= _MDrv_DMD_MSB201X_SetReg(devID, EQE_REG_BASE + 0x3A, 0x00);
    status &= _MDrv_DMD_MSB201X_SetReg(devID, INNC_REG_BASE + 0x05, 0x00);

    status &= _MDrv_DMD_MSB201X_GetReg(devID, TDF_REG_BASE + 0x5b, &reg);
    Reg_Fc_over_Fs = reg;
    status &= _MDrv_DMD_MSB201X_GetReg(devID, TDF_REG_BASE + 0x5a, &reg);
    Reg_Fc_over_Fs = (Reg_Fc_over_Fs<<8)|reg;
    status &= _MDrv_DMD_MSB201X_GetReg(devID, TDF_REG_BASE + 0x59, &reg);
    Reg_Fc_over_Fs = (Reg_Fc_over_Fs<<8)|reg;
    status &= _MDrv_DMD_MSB201X_GetReg(devID, TDF_REG_BASE + 0x58, &reg);
    Reg_Fc_over_Fs = (Reg_Fc_over_Fs<<8)|reg;

    status &= _MDrv_DMD_MSB201X_GetReg(devID, E_DMD_DVBC_CFG_FIF_H, &reg);
    config_Fc = reg;
    status &= _MDrv_DMD_MSB201X_GetReg(devID, E_DMD_DVBC_CFG_FIF_L, &reg);
    config_Fc = (config_Fc<<8)|reg;

    f_Fc = (float)Reg_Fc_over_Fs/134217728.0f * ((float)DVBC_FS);

    FreqCfo_offset = (MS_S32)(RegCfo_offset<<4)/16;

    FreqCfo_offset = FreqCfo_offset/0x8000000/8.0f;

    status &= _MDrv_DMD_MSB201X_GetCurrentSymbolRate(devID, u8DemodIndex, &FreqB);

    FreqCfo_offset = FreqCfo_offset * FreqB + (f_Fc-(float)config_Fc);

    *pFreqOff = FreqCfo_offset;

    return status;
}

MS_BOOL MDrv_DMD_MSB201X_GetStatus(MS_U8 devID, MS_U8 u8DemodIndex, eDMD_MSB201X_MODULATION_TYPE *pQAMMode, MS_U16 *u16SymbolRate, float *pFreqOff)
{
    MS_BOOL bRet=TRUE;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);

    DMD_LOCK();
	
    _MDrv_DMD_MSB201X_GetCurrentModulationType(devID, u8DemodIndex, pQAMMode);
    _MDrv_DMD_MSB201X_GetCurrentSymbolRate(devID, u8DemodIndex, u16SymbolRate);
    _MDrv_DMD_MSB201X_Get_FreqOffset(devID, u8DemodIndex, pFreqOff,8);
	
    DMD_UNLOCK();

    return bRet;
}


MS_BOOL MDrv_DMD_MSB201X_GetFWVer(MS_U8 devID, MS_U8 u8DemodIndex, MS_U16 *ver)
{

    MS_BOOL bRet;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);

    DMD_LOCK();
    _MDrv_DMD_MSB201X_Select_Demod_RIU(devID, u8DemodIndex);
    bRet = _MDrv_DMD_MSB201X_Version(devID, u8DemodIndex, ver);
    //printf("MDrv_DMD_DVBC_GetFWVer %x\n", *ver);
    DMD_UNLOCK();

    return bRet;

}


#if 0
MS_BOOL MDrv_DMD_MSB201X_Exit(MS_U8 devID)
{
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);

    pDemod->bDMD_MSB201X_Power_init_en = FALSE;
    pDemod->u8DMD_MSB201X_PowerOnInitialization_Flow = 0;
    pDemod->u8DMD_MSB201X_Sram_Code  = 0x0;
    pDemod->bDemodRest = TRUE;

    return TRUE;
}
#endif

MS_BOOL _MDrv_DMD_MSB201X_Config(MS_U8 devID, MS_U8 u8DemodIndex, MS_U16 u16SymbolRate, eDMD_MSB201X_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv)
{

    MS_U8              status = true;
    MS_U8              reg_symrate_l, reg_symrate_h;
    //MS_U16             u16Fc = 0;

    _MDrv_DMD_MSB201X_Select_Demod_RIU(devID, u8DemodIndex);
    printf(" @_MDrv_DMD_MSB201X_Config, SR=%d, QAM=%d, u32IFFreq=%ld, bSpecInv=%d \n",u16SymbolRate,eQamMode,(long int)u32IFFreq,bSpecInv);
    printf(" @_MDrv_DMD_MSB201X_Config, t = %ld\n",(long int)MsOS_GetSystemTime());
    printf("set demod index=%d    dev_id=%d\n",u8DemodIndex,devID);

    reg_symrate_l = (MS_U8) (u16SymbolRate & 0xff);
    reg_symrate_h = (MS_U8) (u16SymbolRate >> 8);

    status &= _MDrv_DMD_MSB201X_Restart(devID, u8DemodIndex);


    if (eQamMode == DMD_DVBC_QAMAUTO)
    {
    	#ifdef MS_DEBUG
        printf("DMD_DVBC_QAMAUTO\n");
        #endif
        status &= _MDrv_DMD_MSB201X_SetDSPReg(devID, u8DemodIndex, E_DMD_DVBC_OP_AUTO_SCAN_QAM, 0x01);
        // give default value.
        status &= _MDrv_DMD_MSB201X_SetDSPReg(devID, u8DemodIndex, E_DMD_DVBC_CFG_QAM, QAM);
    }
    else
    {
    	#ifdef MS_DEBUG
        printf("DMD_DVBC_QAM %d\n", eQamMode);
        #endif
        status &= _MDrv_DMD_MSB201X_SetDSPReg(devID, u8DemodIndex, E_DMD_DVBC_OP_AUTO_SCAN_QAM, 0x00);
        status &= _MDrv_DMD_MSB201X_SetDSPReg(devID, u8DemodIndex, E_DMD_DVBC_CFG_QAM, eQamMode);
    }
    		// auto symbol rate enable/disable
    		#ifdef MS_DEBUG
    		printf("u16SymbolRate %d\n", u16SymbolRate);
		#endif
		if (u16SymbolRate == 0)
    {
        status &= _MDrv_DMD_MSB201X_SetDSPReg(devID, u8DemodIndex, E_DMD_DVBC_OP_AUTO_SCAN_SYM_RATE, 0x01);
    }
    else
    {
      status &= _MDrv_DMD_MSB201X_SetDSPReg(devID, u8DemodIndex, E_DMD_DVBC_OP_AUTO_SCAN_SYM_RATE, 0x00);
	 		status &= _MDrv_DMD_MSB201X_SetDSPReg(devID, u8DemodIndex, E_DMD_DVBC_CFG_BW0_L, reg_symrate_l);
	 		status &= _MDrv_DMD_MSB201X_SetDSPReg(devID, u8DemodIndex, E_DMD_DVBC_CFG_BW0_H, reg_symrate_h);
    }


    // IQ Swap
    status &= _MDrv_DMD_MSB201X_SetDSPReg(devID, u8DemodIndex, E_DMD_DVBC_CFG_IQ_SWAP, bSpecInv? 0x01:0x00);

    // Lif
    status &= _MDrv_DMD_MSB201X_SetDSPReg(devID, u8DemodIndex, E_DMD_DVBC_OP_LIF_EN, (u32IFFreq < 10000) ? 1 : 0);
    // Fif
    status &= _MDrv_DMD_MSB201X_SetDSPReg(devID, u8DemodIndex, E_DMD_DVBC_CFG_FIF_L, (u32IFFreq)&0xff);
    status &= _MDrv_DMD_MSB201X_SetDSPReg(devID, u8DemodIndex, E_DMD_DVBC_CFG_FIF_H, (u32IFFreq>>8)&0xff);

    status &= _MDrv_DMD_MSB201X_Active(devID, u8DemodIndex, TRUE);
    return status;
}

MS_BOOL MDrv_DMD_MSB201X_SetConfig(MS_U8 devID, MS_U8 u8DemodIndex, MS_U16 u16SymbolRate, eDMD_MSB201X_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv)
{
    MS_BOOL bRet;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);

    DMD_LOCK();
	
#if 0
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
#endif
    bRet=_MDrv_DMD_MSB201X_Config(devID, u8DemodIndex, u16SymbolRate, eQamMode, u32IFFreq, bSpecInv);
    pDemod->sDMD_MSB201X_Info[u8DemodIndex].u16SymbolRate = u16SymbolRate;
    pDemod->sDMD_MSB201X_Info[u8DemodIndex].eQamMode = eQamMode;
    pDemod->sDMD_MSB201X_Info[u8DemodIndex].u32IFFreq = u32IFFreq;
    pDemod->sDMD_MSB201X_Info[u8DemodIndex].bSpecInv = bSpecInv;
    //pDemod->sDMD_MSB201X_Info.bSerialTS = bSerialTS;
    
    DMD_UNLOCK();
	
    return bRet;
}


MS_BOOL MDrv_DMD_MSB201X_Power_On_Initialization(MS_U8 devID, MS_U8 u8DemodIndex)
{
    //MS_U8     status = TRUE;
    tMSB201X_Demod_Data *pDemod = DEMOD_GET_ACTIVE_NODE(devID);
    DMD_LOCK();

    #ifdef MS_DEBUG
    DBG_DEMOD_FLOW(printf("%s(),%d\n",__func__,__LINE__));
    #endif

    //_MSB201X_I2C_CH_Reset(devID, 3);
    
    //_MSB201X_I2C_CH_Reset(devID, 3);

    //_MDrv_DMD_MSB201X_InitClkgen(devID);

    
    //_MDrv_DMD_MSB201X_TS_MUX_Serial(devID);

    DMD_UNLOCK();
    return TRUE;
}




