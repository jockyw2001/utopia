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

////////////////////////////////////////////////////////////////////////////////
//
/// @file INTERN_DVBT.c
/// @brief INTERN_DVBT DVBT
/// @author MStar Semiconductor, Inc.
//
////////////////////////////////////////////////////////////////////////////////

#define _INTERN_DVBT_C_
#ifdef MSOS_TYPE_LINUX
#include <math.h>
#endif
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
//#include "apiPWS.h"

#include "MsTypes.h"
#include "drvBDMA.h"
//#include "drvIIC.h"
//#include "msAPI_Tuner.h"
//#include "msAPI_MIU.h"
//#include "BinInfo.h"
//#include "halVif.h"
#include "drvDMD_INTERN_DVBT.h"
#include "halDMD_INTERN_DVBT.h"
#include "halDMD_INTERN_common.h"
#ifdef SUPPORT_AP_BIN_IN_FLASH_2
#include "InfoBlock.h"
#endif
#include "drvMMIO.h"
//#include "TDAG4D01A_SSI_DVBT.c"
#include "drvDMD_VD_MBX.h"
#define TEST_EMBEDED_DEMOD 0
//U8 load_data_variable=1;
//-----------------------------------------------------------------------
#define BIN_ID_INTERN_DVBT_DEMOD BIN_ID_INTERN_DVBT

#define	TDE_REG_BASE  	0x2400UL
#define	DIV_REG_BASE  	0x2500UL
#define TR_REG_BASE   	0x2600UL
#define FTN_REG_BASE  	0x2700UL
#define FTNEXT_REG_BASE 0x2800UL



#if 0//ENABLE_SCAN_ONELINE_MSG
#define DBG_INTERN_DVBT_ONELINE(x)  x
#else
#define DBG_INTERN_DVBT_ONELINE(x) //  x
#endif

#ifdef MS_DEBUG
#define DBG_INTERN_DVBT(x) x
#define DBG_GET_SIGNAL(x)  x
#define DBG_INTERN_DVBT_TIME(x) x
#define DBG_INTERN_DVBT_LOCK(x)  x
#else
#define DBG_INTERN_DVBT(x) //x
#define DBG_GET_SIGNAL(x)  //x
#define DBG_INTERN_DVBT_TIME(x) // x
#define DBG_INTERN_DVBT_LOCK(x)  //x
#endif
#define DBG_DUMP_LOAD_DSP_TIME 0

#define INTERN_DVBT_TS_SERIAL_INVERSION         0
#define INTERN_DVBT_TS_PARALLEL_INVERSION       1
#define INTERN_DVBT_DTV_DRIVING_LEVEL           1
#define INTERN_DVBT_INTERNAL_DEBUG              1

#define SIGNAL_LEVEL_OFFSET     0.00
#define TAKEOVERPOINT           -59.0
#define TAKEOVERRANGE           0.5
#define LOG10_OFFSET            -0.21
#define INTERN_DVBT_USE_SAR_3_ENABLE 0
#define INTERN_DVBT_GET_TIME msAPI_Timer_GetTime0()


#if 0//(FRONTEND_TUNER_TYPE==MSTAR_AVATAR2)
#define TUNER_VPP  2
#define IF_AGC_VPP 2
#else
#define TUNER_VPP  1
#define IF_AGC_VPP 2
#endif

#if (TUNER_VPP == 1)
#define ADC_CH_I_PGA_GAIN_CTRL      0x5 // gain = 14.0/5.0
#elif (TUNER_VPP == 2)  // For Avatar tuner,ADC peak to peak voltage is 1 V
#define ADC_CH_I_PGA_GAIN_CTRL      0x5 // gain = 14.0/14.0
#endif

/*BEG INTERN_DVBT_DSPREG_TABLE*/
#define     D_DMD_DVBT_PARAM_VERSION                      0x01
#define     D_DMD_DVBT_OP_AUTO_SCAN_MODE_EN               0x01    // 0 for normal channel change, 1 for auto scanning
#define     D_DMD_DVBT_OP_RFAGC_EN                        0x00
#define     D_DMD_DVBT_OP_HUMDET_EN                       0x01
#define     D_DMD_DVBT_OP_AUTO_RF_MAX_EN                  0x00
#define     D_DMD_DVBT_OP_DCR_EN                          0x01
#define     D_DMD_DVBT_OP_IIS_EN                          0x01
#define     D_DMD_DVBT_OP_IQB_EN                          0x00
#define     D_DMD_DVBT_OP_AUTO_IQ_SWAP_EN                 0x01
#define     D_DMD_DVBT_OP_ACI_EN                          0x01
#define     D_DMD_DVBT_OP_CCI_EN                          0x01
#define     D_DMD_DVBT_OP_FIX_MODE_CP_EN                  0x00
#define     D_DMD_DVBT_OP_FIX_TPS_EN                      0x00
#define     D_DMD_DVBT_CFG_BW                             0x00  // BW: 0..3  for 5M, 6M, 7M, 8M Channel Allocation
#define     D_DMD_DVBT_CFG_MODE                           0x00  // 0, 1, 2 for 2K, 8K, 4K OFDM subcarriers
#define     D_DMD_DVBT_CFG_CP                             0x00  // 0..3  for Intervals of 1/32, 1/16, 1/8, 1/4
#define     D_DMD_DVBT_CFG_LP_SEL                         0x00  // HP or LP selection, 0:HP, 1:LP
#define     D_DMD_DVBT_CFG_CSTL                           0x02  // constellation, 0, 1, 2 for QPSK, 16QAM, 64QAM
#define     D_DMD_DVBT_CFG_HIER                           0x00  // 0..7  for None, Alpha = 1, 2, 4, or add 4 for indepth interleaver
#define     D_DMD_DVBT_CFG_HPCR                           0x01  // HP CR, 0..4  for Rates of 1/2, 2/3, 3/4, 5/6, 7/8
#define     D_DMD_DVBT_CFG_LPCR                           0x02  // LP CR, 0..4  for Rates of 1/2, 2/3, 3/4, 5/6, 7/8
#define     D_DMD_DVBT_CFG_RFMAX                          0x01  // work for RF AGC external mode enable.
#define     D_DMD_DVBT_CFG_ZIF                            0x00  // 0 for IF, 1 for ZIF structure
#define     D_DMD_DVBT_CFG_RSSI                           0x00  // 0 for NOT using RSSI, 1 for using RSSI
#define     D_DMD_DVBT_CFG_RFAGC_REF                      0x64
#define     D_DMD_DVBT_CFG_IFAGC_REF_2K                   0x4B  //0xB0 YP for sensitivity test
#define     D_DMD_DVBT_CFG_IFAGC_REF_8K                   0x4B
#define     D_DMD_DVBT_CFG_IFAGC_REF_ACI                  0x4B
#define     D_DMD_DVBT_CFG_IFAGC_REF_IIS                  0xA0
#define     D_DMD_DVBT_CFG_IFAGC_REF_2K_H                 0x03  //0xB0 YP for sensitivity test
#define     D_DMD_DVBT_CFG_IFAGC_REF_8K_H                 0x03
#define     D_DMD_DVBT_CFG_IFAGC_REF_ACI_H                0x00
#define     D_DMD_DVBT_CFG_IFAGC_REF_IIS_H                0x00

#define     D_DMD_DVBT_CFG_FC_L                           0x20  // 9394, Fc = Fs - IF = 45474 - 36167 = 9307
#define     D_DMD_DVBT_CFG_FC_H                           0x4E
#define     D_DMD_DVBT_CFG_FS_L                           0xC0  // 45474, Fs = 45.4738MHz
#define     D_DMD_DVBT_CFG_FS_H                           0x5D
#define     D_DMD_DVBT_CFG_IQ_SWAP                        0x00  // 1: iq swap, 0: non iq swap

#define     D_DMD_DVBT_CFG_8M_DACI_DET_TH_L               0xf0
#define     D_DMD_DVBT_CFG_8M_DACI_DET_TH_H               0x0a
#define     D_DMD_DVBT_CFG_8M_ANM1_DET_TH_L               0xc4
#define     D_DMD_DVBT_CFG_8M_ANM1_DET_TH_H               0x09
#define     D_DMD_DVBT_CFG_8M_ANP1_DET_TH_L               0xc4
#define     D_DMD_DVBT_CFG_8M_ANP1_DET_TH_H               0x09
#define     D_DMD_DVBT_CFG_7M_DACI_DET_TH_L               0xf0
#define     D_DMD_DVBT_CFG_7M_DACI_DET_TH_H               0x0a
#define     D_DMD_DVBT_CFG_7M_ANM1_DET_TH_L               0xc4
#define     D_DMD_DVBT_CFG_7M_ANM1_DET_TH_H               0x09
#define     D_DMD_DVBT_CFG_7M_ANP1_DET_TH_L               0xc4
#define     D_DMD_DVBT_CFG_7M_ANP1_DET_TH_H               0x09

#define     D_DMD_DVBT_CFG_CCI                            0x00  // PAL_I: 0: Non-Pal-I CCI, 1: Pal-I CCI (for UK)
#define     D_DMD_DVBT_CFG_ICFO_RANGE                     0x01  // ICFOE search range: 0: narrow , 1: medium, 2:large range
#define     D_DMD_DVBT_CFG_TS_SERIAL                      0x01  // 1: serial mode, 0: parallel mode.
//#define     DMD_DVBT_CFG_TS_PARALLEL                    0x00  // 1: serial mode, 0: parallel mode.
#if (INTERN_DVBT_TS_SERIAL_INVERSION)
#define     D_DMD_DVBT_CFG_TS_CLK_INV                     0x01  // Inversion
#else
#define     D_DMD_DVBT_CFG_TS_CLK_INV                     0x00  // non-Inversion
#endif
#define     D_DMD_DVBT_CFG_TS_DATA_SWAP                   0x00  // TS data reverse, 1: reverse, 0: non-reverse.
//#define     DMD_DVBT_CHECKSUM                           0x00
/*END INTERN_DVBT_DSPREG_TABLE*/
#define DVBT_FS     45474   // 24000
#define FC_H        0x4E    // 40474, Fc = Fs - IF = 45474 - 5000 = 40474 -> 0323 update
#define FC_L        0x20    // 0323 jason
#define FS_H        ((DVBT_FS>>8)&0xFF)    // FS=24000, Fs = 24MHz
#define FS_L        (DVBT_FS&0xFF)    // andy 2009-8-18 ¿ÀÈÄ 10:22:29 0x9E
#define SET_ZIF     0x00
#define IQB_EN      0x00

#define FORCE_MC	0x00    //0: auto 1: Force mode-cp
#define FORCE_TPS	0x00	//0: auto 1: Force TPS
#define AUTO_SCAN	0x00	// Auto Scan - 0:channel change, 1:auto-scan
#define	CSTL		0x02    //0:QPSK 1:16 2: 64
#define HIER		0x00
#define HPCR		0x01	// HP_CR 0:1/2, 1:2/3, 2: 3/4, 3: 5/6, 4:7/8
#define LPCR		0x01	// LP_CR 0:1/2, 1:2/3, 2: 3/4, 3: 5/6, 4:7/8
#define FFT_MODE	0x01	// FFT mode - 0:2K, 1:8K
#define CP			0x00	// CP - 0:1/32, 1/16, 1/8, 1/4
#define LP_SEL		0x00	// LP select
#define IQ_SWAP		0x00 //0x01
#define PAL_I		0x00	// PAL_I: 0: Non-Pal-I CCI, 1: Pal-I CCI (for UK)
#define	CFO_RANGE 	0x01	//0: 500KHz 1: 900KHz
#define	CFO_RANGE_TW 	0x00	//0: 500KHz 1: 900KHz
#define TS_SER      0
#define TS_INV      0
#define FIF_H       (MS_U8)(( (((MS_U16)FS_H<<8)|FS_L) - (((MS_U16)FC_H<<8)|FC_L))>>8)
#define FIF_L       (MS_U8)(( ((MS_U16)FS_H<<8)|FS_L) - (((MS_U16)FC_H<<8)|FC_L))
#define IF_INV_PWM    0x00
#define T_LOWIF     1

MS_U8 INTERN_DVBT_DSPREG[] =
{
0x00, 0x00, 0x00, D_DMD_DVBT_OP_DCR_EN, 0x01, 0x01, 0x01, IQB_EN, 0x01, 0x00, 0x00, FORCE_MC, FORCE_TPS, AUTO_SCAN, 0x00, SET_ZIF, //00-0F
0x00, T_LOWIF, 0x00, FS_L, FS_H, FIF_L, FIF_H, FC_L, FC_H, 0x03, FFT_MODE, CP, LP_SEL, CSTL, HIER, HPCR, //10-1F
LPCR, IQ_SWAP, 0x00, PAL_I, CFO_RANGE, D_DMD_DVBT_CFG_RFAGC_REF, D_DMD_DVBT_CFG_IFAGC_REF_2K, D_DMD_DVBT_CFG_IFAGC_REF_8K, D_DMD_DVBT_CFG_IFAGC_REF_ACI, D_DMD_DVBT_CFG_IFAGC_REF_IIS, //20-29
D_DMD_DVBT_CFG_IFAGC_REF_2K_H, D_DMD_DVBT_CFG_IFAGC_REF_8K_H, D_DMD_DVBT_CFG_IFAGC_REF_ACI_H, D_DMD_DVBT_CFG_IFAGC_REF_IIS_H, TS_SER, TS_INV, //2A-2F
0x00, 0xf0, 0x0a, 0xc4, 0x09, 0xc4, 0x09, 0xf0, 0x0a, 0xc4, 0x09, 0xc4, 0x09, 0x00, 0xd0, 0x80, //30-3F
0x7f, 0xa0, 0x23, 0x05, 0x05, 0x40, 0x34, 0x06, 0x00, 0x00, 0x00, 0x00, 0x45, 0x00, 0x65, 0x00, //40-4F
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //50-5F
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x33, 0x01, 0x03, //60-6F
0x03, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,       //70-7E
/*
//  0x00  0x01  0x02                0x03  0x04  0x05  0x06  0x07
    0x00, 0x01, DMD_DVBT_OP_DCR_EN, 0x01, 0x01, 0x01, IQB_EN, 0x01,
//  0x08  0x09      0x0a      0x0b       0x0c       0x0d  0x0e  0xf
    0x00, 0x00,     FORCE_MC, FORCE_TPS, AUTO_SCAN, 0x00, 0x00, 0x00,
//  0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
    0x00, SET_ZIF, 0xff, FC_L, FC_H, FS_L, FS_H, 0x03,
//  0x18      0x19	0x1a    0x1b   	0x1c	0x1d	0x1e	0x1f
    FFT_MODE, CP, 	LP_SEL, CSTL, 	HIER, 	HPCR, 	LPCR, 	IQ_SWAP,
//	0x20	0x21	0x22		0x23					0x24						0x25						0x26						0x27
    0x00, 	PAL_I, 	CFO_RANGE, 	DMD_DVBT_CFG_RFAGC_REF, DMD_DVBT_CFG_IFAGC_REF_2K, DMD_DVBT_CFG_IFAGC_REF_8K, 	DMD_DVBT_CFG_IFAGC_REF_ACI, DMD_DVBT_CFG_IFAGC_REF_IIS,
//  0x28  0x29  0x2a	0x2b   0x2c	   0x2d	 0x2e  0x2f
    0x9A, 0x01, TS_SER, 0x00,  TS_INV, 0x00, 0x00, 0xC8,
//  0x30  0x31  0x32  0x33  0x34  0x35  0x36  0x37  0x38       0x39  0x3A  0x3B  0x3C  0x3D  0x3E  0x3F
    0x00, 0xC8, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, T_LOWIF,   0x47, 0x8D, 0x01, 0x00, 0x00, 0x00, 0x00,
*/
};
/*END INTERN_DVBT_DSPREG_TABLE*/
//-----------------------------------------------------------------------
/****************************************************************
*Local Variables                                                                                              *
****************************************************************/
static MS_BOOL bFECLock=0;
static MS_BOOL bTPSLock = 0;
static MS_U32 u32ChkScanTimeStart = 0;
static MS_U32 u32FecFirstLockTime=0;
static MS_U32 u32FecLastLockTime=0;
static float fViterbiBerFiltered=-1;
//Global Variables
S_CMDPKTREG gsCmdPacket;
//U8 gCalIdacCh0, gCalIdacCh1;

#ifdef INTERN_DVBT_LOAD_FW_FROM_CODE_MEMORY
MS_U8 INTERN_DVBT_table[] = {
    #include "fwDMD_INTERN_DVBT.dat"
};

#endif

static DMD_SSI_DBM_NORDIGP1 dvbt_ssi_dbm_nordigp1[] =
{
  { _QPSK , _CR1Y2, -93},
  { _QPSK , _CR2Y3, -91},
  { _QPSK , _CR3Y4, -90},
  { _QPSK , _CR5Y6, -89},
  { _QPSK , _CR7Y8, -88},

  { _16QAM , _CR1Y2, -87},
  { _16QAM , _CR2Y3, -85},
  { _16QAM , _CR3Y4, -84},
  { _16QAM , _CR5Y6, -83},
  { _16QAM , _CR7Y8, -82},

  { _64QAM , _CR1Y2, -82},
  { _64QAM , _CR2Y3, -80},
  { _64QAM , _CR3Y4, -78},
  { _64QAM , _CR5Y6, -77},
  { _64QAM , _CR7Y8, -76},
  { _UNKNOW_QAM , _UNKNOW_CR, 0.0},
};



static void INTERN_DVBT_SignalQualityReset(void);
MS_BOOL INTERN_DVBT_Show_Demod_Version(void);

static void INTERN_DVBT_SignalQualityReset(void)
{
    u32FecFirstLockTime=0;
    fViterbiBerFiltered=-1;
}

MS_BOOL INTERN_DVBT_DSPReg_Init(const MS_U8 *u8DVBT_DSPReg,  MS_U8 u8Size)
{
    MS_U8   idx = 0, u8RegRead = 0, u8RegWrite = 0, u8Mask = 0;
    MS_BOOL status = TRUE;
    MS_U16 u16DspAddr = 0;

    DBG_INTERN_DVBT(printf("INTERN_DVBT_DSPReg_Init\n"));

    for (idx = 0; idx<sizeof(INTERN_DVBT_DSPREG); idx++)
        status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(idx, INTERN_DVBT_DSPREG[idx]);

    if (u8DVBT_DSPReg != NULL)
    {
        /*temp solution until new dsp table applied.*/
        // if (INTERN_DVBT_DSPREG[E_DMD_DVBT_PARAM_VERSION] == u8DVBT_DSPReg[0])
        if (u8DVBT_DSPReg[0] >= 1)
        {
            u8DVBT_DSPReg+=2;
            for (idx = 0; idx<u8Size; idx++)
            {
                u16DspAddr = *u8DVBT_DSPReg;
                u8DVBT_DSPReg++;
                u16DspAddr = (u16DspAddr) + ((*u8DVBT_DSPReg)<<8);
                u8DVBT_DSPReg++;
                u8Mask = *u8DVBT_DSPReg;
                u8DVBT_DSPReg++;
                status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(u16DspAddr, &u8RegRead);
                u8RegWrite = (u8RegRead & (~u8Mask)) | ((*u8DVBT_DSPReg) & (u8Mask));
                u8DVBT_DSPReg++;
                DBG_INTERN_DVBT(printf("DSP addr:%x mask:%x read:%x write:%x\n", u16DspAddr, u8Mask, u8RegRead, u8RegWrite));
                status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(u16DspAddr, u8RegWrite);
            }
        }
        else
        {
            printf("FATAL: parameter version incorrect\n");
        }
    }

    return status;
}

/***********************************************************************************
  Subject:    Command Packet Interface
  Function:   INTERN_DVBT_Cmd_Packet_Send
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
MS_BOOL INTERN_DVBT_Cmd_Packet_Send(S_CMDPKTREG* pCmdPacket, MS_U8 param_cnt)
{
    MS_U8   status = true, indx;
    MS_U8   reg_val=0, timeout = 0;
    return TRUE;
    //MsOS_ObtainMutex(_s32_Demod_DVBT_Mutex, MSOS_WAIT_FOREVER);
    // ==== Command Phase ===================
    DBG_INTERN_DVBT(printf("--->INTERN_DVBT (cmd=0x%x)(0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,) \n",pCmdPacket->cmd_code,
            pCmdPacket->param[0],pCmdPacket->param[1],
            pCmdPacket->param[2],pCmdPacket->param[3],
            pCmdPacket->param[4],pCmdPacket->param[5] ));

    // wait _BIT_END clear
    do
    {
        reg_val = HAL_DMD_RIU_ReadByte(REG_CMD_CTRL);
        if((reg_val & _BIT_END) != _BIT_END)
        {
            break;
        }
        MsOS_DelayTask(5);
        if (timeout++ > 200)
        {
            printf("---> INTERN_DVBT_Cmd_Packet_Send fail on 'wait _BIT_END clear' \n");
            //MsOS_ReleaseMutex(_s32_Demod_DVBT_Mutex);
            return false;
        }
    } while (1);

    // set cmd_3:0 and _BIT_START
    reg_val = HAL_DMD_RIU_ReadByte(REG_CMD_CTRL);
    reg_val = (reg_val & 0x0f)|(pCmdPacket->cmd_code << 4)|_BIT_START;
    HAL_DMD_RIU_WriteByte(REG_CMD_CTRL, reg_val);


    //DBG_INTERN_DVBT(printf("demod_config: cmd_code = %bx\n", pCmdPacket->cmd_code));
    // wait _BIT_START clear
    do
    {
        reg_val = HAL_DMD_RIU_ReadByte(REG_CMD_CTRL);
        if((reg_val & _BIT_START) != _BIT_START)
        {
            break;
        }
        MsOS_DelayTask(5);
        if (timeout++ > 200)
        {
            printf("---> INTERN_DVBT_Cmd_Packet_Send fail on 'wait _BIT_START clear' \n");
            //MsOS_ReleaseMutex(_s32_Demod_DVBT_Mutex);
            return false;
        }
    } while (1);

    // ==== Data Phase ======================

    HAL_DMD_RIU_WriteByte(REG_CMD_ADDR, 0x00);

    for (indx = 0; indx < param_cnt; indx++)
    {
        reg_val = HAL_DMD_RIU_ReadByte(REG_CMD_ADDR);
        //DBG_INTERN_DVBT(printf("demod_config: param[%bd] = %bx\n", reg_val, pCmdPacket->param[indx]));

        // set param[indx] and _BIT_DRQ
        HAL_DMD_RIU_WriteByte(REG_CMD_DATA, pCmdPacket->param[indx]);
        reg_val = HAL_DMD_RIU_ReadByte(REG_DTA_CTRL);
        HAL_DMD_RIU_WriteByte(REG_DTA_CTRL, reg_val|_BIT_DRQ);

        // wait _BIT_DRQ clear
        do
        {
            reg_val = HAL_DMD_RIU_ReadByte(REG_DTA_CTRL);
            if ((reg_val & _BIT_DRQ) != _BIT_DRQ)
            {
                break;
            }
            MsOS_DelayTask(5);
            if (timeout++ > 200)
            {
                printf("---> INTERN_DVBT_Cmd_Packet_Send fail on 'wait _BIT_DRQ clear' \n");
                //MsOS_ReleaseMutex(_s32_Demod_DVBT_Mutex);
                return false;
            }
        } while (1);
    }

    // ==== End Phase =======================

    // set _BIT_END to finish command
    reg_val = HAL_DMD_RIU_ReadByte(REG_CMD_CTRL);
    HAL_DMD_RIU_WriteByte(REG_CMD_CTRL, reg_val|_BIT_END);
    //MsOS_ReleaseMutex(_s32_Demod_DVBT_Mutex);
    return status;
}


/***********************************************************************************
  Subject:    Command Packet Interface
  Function:   INTERN_DVBT_Cmd_Packet_Exe_Check
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
MS_BOOL INTERN_DVBT_Cmd_Packet_Exe_Check(MS_BOOL* cmd_done)
{
    return TRUE;
}

/***********************************************************************************
  Subject:    SoftStop
  Function:   INTERN_DVBT_SoftStop
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/

MS_BOOL INTERN_DVBT_SoftStop ( void )
{
	#if 1
    MS_U16     u8WaitCnt=0;

    if (HAL_DMD_RIU_ReadByte(MBRegBase + 0x00))
    {
        printf(">> MB Busy!\n");
        return FALSE;
    }

    HAL_DMD_RIU_WriteByte(MBRegBase + 0x00, 0xA5);                 // MB_CNTL set read mode

    HAL_DMD_RIU_WriteByte(0x103483, 0x02);                         // assert interrupt to VD MCU51
    HAL_DMD_RIU_WriteByte(0x103483, 0x00);                         // de-assert interrupt to VD MCU51

    while(HAL_DMD_RIU_ReadByte(MBRegBase + 0x00)!= 0x5A)           // wait MB_CNTL set done
    {
#if TEST_EMBEDED_DEMOD
        MsOS_DelayTask(1);  // << Ken 20090629
#endif
        if (u8WaitCnt++ >= 0xFF)
        {
            printf(">> DVBT SoftStop Fail!\n");
            return FALSE;
        }
    }

    //HAL_DMD_RIU_WriteByte(0x103480, 0x01);                         // reset VD_MCU
    HAL_DMD_RIU_WriteByte(MBRegBase + 0x00, 0x00);                 // MB_CNTL clear
	#endif
    return TRUE;
}


/***********************************************************************************
  Subject:    Reset
  Function:   INTERN_DVBT_Reset
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
extern void HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake(void);
MS_BOOL INTERN_DVBT_Reset ( void )
{
    DBG_INTERN_DVBT(printf(" @INTERN_DVBT_reset\n"));

    DBG_INTERN_DVBT_TIME(printf("INTERN_DVBT_Reset, t = %ld\n",MsOS_GetSystemTime()));

    INTERN_DVBT_SoftStop();


    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00, 0x01);     // reset DMD_MCU
    //MDrv_SYS_DMD_VD_MBX_WriteReg(0x2002, 0x72);        // reset DVB-T
    MsOS_DelayTask(5);
    HAL_DMD_RIU_WriteByte(MBRegBase + 0x00 , 0x00);     // clear MB_CNTL
    // MDrv_SYS_DMD_VD_MBX_WriteReg(0x2002, 0x52);
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00, 0x00);
    MsOS_DelayTask(5);

    HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake();
    HAL_DMD_RIU_WriteByte(MBRegBase + 0x00 , 0x00);

    bFECLock = FALSE;
    bTPSLock = FALSE;
    u32ChkScanTimeStart = MsOS_GetSystemTime();
    return TRUE;
}

/***********************************************************************************
  Subject:    Exit
  Function:   INTERN_DVBT_Exit
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
MS_BOOL INTERN_DVBT_Exit ( void )
{

    INTERN_DVBT_SoftStop();


    //diable clk gen
    //HAL_DMD_RIU_WriteByte(0x103314, 0x01);   // reg_ckg_dvbtc_adc@0x0a[3:0] : ADC_CLK
    //HAL_DMD_RIU_WriteByte(0x103315, 0x01);   // reg_ckg_dvbtc_innc@0x0a[11:8]

    HAL_DMD_RIU_WriteByte(0x10330a, 0x01);   // reg_ckg_atsc_adcd_sync@0x05[3:0] : ADCCLK
    HAL_DMD_RIU_WriteByte(0x10330b, 0x00);

    HAL_DMD_RIU_WriteByte(0x10330c, 0x01);   // reg_ckg_dvbtc_inner1x@0x06[3:0] : MPLLDIV10/4=21.5MHz
    HAL_DMD_RIU_WriteByte(0x10330d, 0x01);   // reg_ckg_dvbtc_inner2x@0x06[11:8]: MPLLDIV10/2=43.2MHz

    HAL_DMD_RIU_WriteByte(0x10330e, 0x01);   // reg_ckg_dvbtc_inner4x@0x07[3:0] : MPLLDIV10=86.4MHz
    HAL_DMD_RIU_WriteByte(0x10330f, 0x00);

    HAL_DMD_RIU_WriteByte(0x103310, 0x01);   // reg_ckg_dvbtc_outer1x@0x08[3:0] : MPLLDIV10/2=43.2MHz
    HAL_DMD_RIU_WriteByte(0x103311, 0x01);   // reg_ckg_dvbtc_outer2x@0x08[11:8]: MPLLDIV10=86.4MHz

    HAL_DMD_RIU_WriteByte(0x103312, 0x01);   // dvbt_t:0x0000, dvb_c: 0x0004
    HAL_DMD_RIU_WriteByte(0x103313, 0x00);

    HAL_DMD_RIU_WriteByte(0x103314, 0x01);   // reg_ckg_dvbtc_adc@0x0a[3:0] : ADC_CLK
    HAL_DMD_RIU_WriteByte(0x103315, 0x01);   // reg_ckg_dvbtc_innc@0x0a[11:8]

    HAL_DMD_RIU_WriteByte(0x103316, 0x01);   // reg_ckg_dvbtc_eq8x@0x0b[3:0] : MPLLDIV3/2=144MHz
    HAL_DMD_RIU_WriteByte(0x103317, 0x01);   // reg_ckg_dvbtc_eq@0x0b[11:8] : MPLLDIV3/16=18MHz

    HAL_DMD_RIU_WriteByte(0x103318, 0x11);   // reg_ckg_dvbtc_sram0~3@0x0c[13:0]
    HAL_DMD_RIU_WriteByte(0x103319, 0x11);

    HAL_DMD_RIU_WriteByte(0x103308, 0x01);   // parallel mode:0x0001 / serial mode: 0x0401
    HAL_DMD_RIU_WriteByte(0x103309, 0x05);   // reg_ckg_dvbtc_ts@0x04

    HAL_DMD_RIU_WriteByte(0x101E3E, 0x00);   // DVBT = BIT1 clear

    return TRUE;
}

/***********************************************************************************
  Subject:    Load DSP code to chip
  Function:   INTERN_DVBT_LoadDSPCode
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
static MS_BOOL INTERN_DVBT_LoadDSPCode(void)
{
    MS_U8  udata = 0x00;
    MS_U16 i;
    MS_U16 fail_cnt=0;

#if (DBG_DUMP_LOAD_DSP_TIME==1)
    MS_U32 u32Time;
#endif


#ifndef INTERN_DVBT_LOAD_FW_FROM_CODE_MEMORY
    BININFO BinInfo;
    MS_BOOL bResult;
    MS_U32 u32GEAddr;
    MS_U8 Data;
    MS_S8 op;
    MS_U32 srcaddr;
    MS_U32 len;
    MS_U32 SizeBy4K;
    MS_U16 u16Counter=0;
    MS_U8 *pU8Data;
#endif

#if 0
    if(HAL_DMD_RIU_ReadByte(0x101E3E))
    {
        printf("Warring! Reg[0x101E3E]=%d\n", HAL_DMD_RIU_ReadByte(0x101E3E));
        return FALSE;
    }
#endif

  //  MDrv_Sys_DisableWatchDog();


    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00,  0x01);        // reset VD_MCU
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x01,  0x00);        // disable SRAM
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03,  0x50);        // enable "vdmcu51_if"
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03,  0x51);        // enable auto-increase
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x04,  0x00);        // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x05,  0x00);        // sram address high byte

    ////  Load code thru VDMCU_IF ////
    DBG_INTERN_DVBT(printf(">Load Code...\n"));
#ifdef INTERN_DVBT_LOAD_FW_FROM_CODE_MEMORY
    for ( i = 0; i < sizeof(INTERN_DVBT_table); i++)
    {
        HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x0C, INTERN_DVBT_table[i]); // write data to VD MCU 51 code sram
    }
#else
    BinInfo.B_ID = BIN_ID_INTERN_DVBT_DEMOD;
    msAPI_MIU_Get_BinInfo(&BinInfo, &bResult);
    if ( bResult != PASS )
    {
        return FALSE;
    }
    //printf("\t DEMOD_MEM_ADR  =%08LX\n", ((DEMOD_MEM_ADR & MIU1) ? (DEMOD_MEM_ADR | MIU_INTERVAL) : (DEMOD_MEM_ADR)));

#ifdef SUPPORT_AP_BIN_IN_FLASH_2
    InfoBlock_Flash_2_Checking_Start(&BinInfo);
#endif

#if OBA2
    MApi_BDMA_CopyFromResource(BinInfo.B_FAddr, _PA2VA((DEMOD_MEM_MEMORY_TYPE & MIU1) ? (DEMOD_MEM_ADR | MIU_INTERVAL) : (DEMOD_MEM_ADR)), MemAlign(BinInfo.B_Len, 8));
#else
    msAPI_MIU_Copy(BinInfo.B_FAddr, ((DEMOD_MEM_MEMORY_TYPE & MIU1) ? (DEMOD_MEM_ADR | MIU_INTERVAL) : (DEMOD_MEM_ADR)), MemAlign(BinInfo.B_Len, 8), MIU_FLASH2SDRAM);
#endif

#ifdef SUPPORT_AP_BIN_IN_FLASH_2
    InfoBlock_Flash_2_Checking_End(&BinInfo);
#endif

    //W1BaseAddr = MDrv_Sys_GetXdataWindow1Base();
    SizeBy4K=BinInfo.B_Len/0x1000;
    //printf("\t RRR: SizeBy4K=%d (L=%d)\n", SizeBy4K,BinInfo.B_Len);

#if (DBG_DUMP_LOAD_DSP_TIME==1)
    u32Time = msAPI_Timer_GetTime0();
#endif

    u32GEAddr = _PA2VA(((DEMOD_MEM_MEMORY_TYPE & MIU1) ? (DEMOD_MEM_ADR | MIU_INTERVAL) : (DEMOD_MEM_ADR)));

    for (i=0;i<=SizeBy4K;i++)
    {
        if(i==SizeBy4K)
            len=BinInfo.B_Len%0x1000;
        else
            len=0x1000;

        srcaddr = u32GEAddr+(0x1000*i);
        //printf("\t i = %08X\n", i);
        //printf("\t len = %08X\n", len);
        op = 1;
        u16Counter = 0 ;
        //printf("\t (B=0x%x)(Src=0x%x)Data =",i,srcaddr);
        while(len--)
        {
            u16Counter ++ ;
            //printf("file: %s, line: %d\n", __FILE__, __LINE__);
            //pU8Data = (U8 *)(srcaddr|0x80000000);
            #if OBA2
            pU8Data = (U8 *)(srcaddr);
            #else
            pU8Data = (U8 *)(srcaddr|0x80000000);
            #endif
            Data  = *pU8Data;

            #if 0
            if(u16Counter < 0x100)
                printf("0x%bx,", Data);
            #endif
            HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x0C, Data); // write data to VD MCU 51 code sram

            srcaddr += op;
        }
     //   printf("\n\n\n");
    }

#if (DBG_DUMP_LOAD_DSP_TIME==1)
    printf("------> INTERN_DVBT Load DSP Time:  (%lu)\n", msAPI_Timer_DiffTimeFromNow(u32Time)) ;
#endif

#endif

    ////  Content verification ////
    DBG_INTERN_DVBT(printf(">Verify Code...\n"));

    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x04, 0x00);         // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x05, 0x00);         // sram address high byte

#ifdef INTERN_DVBT_LOAD_FW_FROM_CODE_MEMORY
    for ( i = 0; i < sizeof(INTERN_DVBT_table); i++)
    {
        udata = HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x10);    // read sram data
        if (udata != INTERN_DVBT_table[i])
        {
            printf(">fail add = 0x%x\n", i);
            printf(">code = 0x%x\n", INTERN_DVBT_table[i]);
            printf(">data = 0x%x\n", udata);

            if (fail_cnt++ > 10)
            {
                printf(">DVB-T DSP Loadcode fail!");
                return false;
            }
        }
    }
#else
    for (i=0;i<=SizeBy4K;i++)
    {
        if(i==SizeBy4K)
            len=BinInfo.B_Len%0x1000;
        else
            len=0x1000;

        srcaddr = u32GEAddr+(0x1000*i);
        //printf("\t i = %08LX\n", i);
        //printf("\t len = %08LX\n", len);
        op = 1;
        u16Counter = 0 ;
        //printf("\t (B=0x%bx)(Src=0x%x)Data =",i,srcaddr);
        while(len--)
        {
            u16Counter ++ ;
            //printf("file: %s, line: %d\n", __FILE__, __LINE__);
            //pU8Data = (U8 *)(srcaddr|0x80000000);
            #if OBA2
            pU8Data = (U8 *)(srcaddr);
            #else
            pU8Data = (U8 *)(srcaddr|0x80000000);
            #endif
            Data  = *pU8Data;

            #if 0
            if(u16Counter < 0x100)
                printf("0x%bx,", Data);
            #endif
            udata = HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x10);    // read sram data
            if (udata != Data)
            {
                printf(">fail add = 0x%lx\n", (U32)((i*0x1000)+(0x1000-len)));
                printf(">code = 0x%x\n", Data);
                printf(">data = 0x%x\n", udata);

                if (fail_cnt++ > 10)
                {
                    printf(">DVB-T DSP Loadcode fail!");
                    return false;
                }
            }

            srcaddr += op;
        }
     //   printf("\n\n\n");
    }
#endif

    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, 0x50);     // diable auto-increase
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, 0x00);     // disable "vdmcu51_if"
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x01, 0x01);     // enable SRAM
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00, 0x00);     // release VD_MCU

    DBG_INTERN_DVBT(printf(">DSP Loadcode done."));
    //while(load_data_variable);

    HAL_DMD_RIU_WriteByte(0x101E3E, 0x02);     // DVBT = BIT1 -> 0x02

    return TRUE;
}

/***********************************************************************************
  Subject:    DVB-T CLKGEN initialized function
  Function:   INTERN_DVBT_Power_On_Initialization
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
void INTERN_DVBT_InitClkgen(MS_BOOL bRFAGCTristateEnable)
{
    MS_U8 temp_val;
    MS_U8  	udatatemp = 0x00;
    HAL_DMD_RIU_WriteByte(0x101e39,0x00);
//    HAL_DMD_RIU_WriteByte(0x112003, (HAL_DMD_RIU_ReadByte(0x112003)&(~(BIT(5)))));      // Release Ana misc resest

    // Release vivaldi2mi_bridge reset
    // [0]	    reg_vivaldi2mi_bridge_rst
    // `RIU_W( (`RIUBASE_DMD_TOP>>1)+7'h47, 2'b01, 16'h0000);
    // `RIU_W( (`RIUBASE_DMD_TOP>>1)+7'h47, 2'b01, 16'h0000);
//    HAL_DMD_RIU_WriteByte(0x11208E, (HAL_DMD_RIU_ReadByte(0x11208E)&(~(BIT(0)))));

    // ----------------------------------------------
    //  start demod CLKGEN setting
    // ----------------------------------------------
    // *** Set register at CLKGEN1
    // enable DMD MCU clock "bit[0] set 0"
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h0f, 2'b11, 16'h001c);
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h0f, 2'b11, 16'h001c);
    // CLK_DMDMCU clock setting
    // [0] disable clock
    // [1] invert clock
    // [4:2]
    //         000:170 MHz(MPLL_DIV_BUf)
    //         001:160MHz
    //         010:144MHz
    //         011:123MHz
    //         100:108MHz
    //         101:mem_clcok
    //         110:mem_clock div 2
    //         111:select XTAL
    HAL_DMD_RIU_WriteByte(0x10331f,0x00);
    HAL_DMD_RIU_WriteByte(0x10331e,0x10);//0331 patch

    // set parallet ts clock
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h00, 2'b11, 16'h0511);
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h00, 2'b11, 16'h0511);

    //HAL_DMD_RIU_WriteByte(0x103301,0x07);//0x060b
    temp_val=HAL_DMD_RIU_ReadByte(0x103301);
    temp_val|=0x07;
    HAL_DMD_RIU_WriteByte(0x103301,temp_val);

    HAL_DMD_RIU_WriteByte(0x103300,0x17);

    // enable atsc, DVBTC ts clock
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h04, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h04, 2'b11, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x103309,0x00);
    HAL_DMD_RIU_WriteByte(0x103308,0x00);

    // enable dvbc adc clock
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h0a, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h0a, 2'b11, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x103315,0x00);
    HAL_DMD_RIU_WriteByte(0x103314,0x00);

    udatatemp = HAL_DMD_RIU_ReadByte(0x111f2a);
    HAL_DMD_RIU_WriteByte(0x111f2a,udatatemp&0xF8);

	// Reset TS divider
    HAL_DMD_RIU_WriteByte(0x103302,0x01);
    HAL_DMD_RIU_WriteByte(0x103302,0x00);

    // enable vif DAC clock
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h0d, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h0d, 2'b11, 16'h0000);
//    HAL_DMD_RIU_WriteByte(0x10331b,0x00);
//    HAL_DMD_RIU_WriteByte(0x10331a,0x00);

    // Select MPLLDIV17
    // [0] : reg_atsc_adc_sel_mplldiv2
    // [1] : reg_atsc_eq_sel_mplldiv2
    // [2] : reg_eq25_sel_mplldiv3
    // [3] : reg_p4_cfo_sel_eq25
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h14, 2'b01, 16'h0003);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h14, 2'b01, 16'h0003);
//    HAL_DMD_RIU_WriteByte(0x111f28,0x03);

    // *** Set register at CLKGEN_DMD
    // enable atsc clock
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h01, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h01, 2'b11, 16'h0000);
//    HAL_DMD_RIU_WriteByte(0x111f03,0x04);
//    HAL_DMD_RIU_WriteByte(0x111f02,0x04);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h02, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h02, 2'b11, 16'h0000);
//    HAL_DMD_RIU_WriteByte(0x111f05,0x00);
//    HAL_DMD_RIU_WriteByte(0x111f04,0x00);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h03, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h03, 2'b11, 16'h0000);
//    HAL_DMD_RIU_WriteByte(0x111f07,0x04);
//    HAL_DMD_RIU_WriteByte(0x111f06,0x04);

    // enable clk_atsc_adcd_sync
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h05, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h05, 2'b11, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x111f0b,0x00);
    HAL_DMD_RIU_WriteByte(0x111f0a,0x00);

    // enable dvbt inner clock
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h06, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h06, 2'b11, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x111f0d,0x00);
    HAL_DMD_RIU_WriteByte(0x111f0c,0x00);

    // enable dvbt inner clock
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h07, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h07, 2'b11, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x111f0f,0x00);
    HAL_DMD_RIU_WriteByte(0x111f0e,0x00);

    // enable dvbt inner clock
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h08, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h08, 2'b11, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x111f11,0x00);
    HAL_DMD_RIU_WriteByte(0x111f10,0x00);

    // enable dvbc outer clock
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h09, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h09, 2'b11, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x111f13,0x00);
    HAL_DMD_RIU_WriteByte(0x111f12,0x00);

    // enable dvbc inner-c clock
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h0a, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h0a, 2'b11, 16'h0000);
//    HAL_DMD_RIU_WriteByte(0x111f15,0x00);
//    HAL_DMD_RIU_WriteByte(0x111f14,0x00);

    // enable dvbc eq clock
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h0b, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h0b, 2'b11, 16'h0000);
//    HAL_DMD_RIU_WriteByte(0x111f17,0x00);
//    HAL_DMD_RIU_WriteByte(0x111f16,0x00);

    // enable sram clock
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h0c, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h0c, 2'b11, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x111f19,0x00);
    HAL_DMD_RIU_WriteByte(0x111f18,0x00);

    // select clock
    // [3:0] : reg_ckg_frontend
    //         [0]  : disable clock
    //         [1]  : invert clock
    //         [3:2]: Select clock source
    //                00: select clk_dmplldiv17_div2    (25.41 MHz, ATSC)
    //                01: select clk_dmdadc             (48    MHz, DVBT/C)
    //                10: reserved
    //                11: select DFT_CLK
    // [7:4] : reg_ckg_tr
    //         [0]  : disable clock
    //         [1]  : invert clock
    //         [3:2]: Select clock source
    //                00: select clk_dmplldiv17_div2    (25.41 MHz, ATSC)
    //                01: select clk_dmdadc             (48    MHz, DVBT/C)
    //                10: reserved
    //                11: select DFT_CLK
    // [11:8]: reg_ckg_acifir
    //         [0]  : disable clock
    //         [1]  : invert clock
    //         [3:2]: Select clock source
    //                00: select clk_dmplldiv17_div2    (25.41 MHz, ATSC)
    //                01: select clk_dmdadc             (48    MHz, DVBT/C)
    //                10: clk_vif_ssc_mux               (43.2~50.82  MHz, VIF)
    //                11: select DFT_CLK
    // [15:12]: reg_ckg_frontend_d2
    //         [0]  : disable clock
    //         [1]  : invert clock
    //         [3:2]: Select clock source
    //                00: clk_dmdadc_div2
    //                01: clk_dmplldiv17_div4(12.705 MHz)
    //                10: reserved
    //                11: select DFT_CLK
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h11, 2'b11, 16'h0444);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h11, 2'b11, 16'h0444);
//    HAL_DMD_RIU_WriteByte(0x111f23,0x04);
//    HAL_DMD_RIU_WriteByte(0x111f22,0x44);

    // enable isdbt clock
    // [2:0] : reg_ckg_isdbt_inner1x
    //        [0]  : disable clock
    //        [1]  : invert clock
    //        [3:2]: Select clock source
    //               00: clk_dmplldiv10_div4(21.6MHz, ISDBT only)
    //               01: reserved
    //               10: reserved
    //               11: DFT_CLK
    // [6:4]: reg_ckg_isdbt_inner2x
    //         [0]  : disable clock
    //         [1]  : invert clock
    //         [2]: Select clock source
    //                00: clk_dmplldiv10_div2(43.2MHz,ISDBT only)
    //                01: reserved
    //                10: reserved
    //                11: DFT_CLK
    // [10:8] : reg_ckg_isdbt_inner4x
    //         [0]  : disable clock
    //         [1]  : invert clock
    //         [3:2]: Select clock source
    //                00: clk_dmplldiv10(86.4 MHz, DVBT only)
    //                01: reserved
    //                10: reserved
    //                11: DFT_CLK
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h21, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h21, 2'b11, 16'h0000);
//    HAL_DMD_RIU_WriteByte(0x111f43,0x00);
//    HAL_DMD_RIU_WriteByte(0x111f42,0x00);


    // enable isdbt outer clock
    // [3:0] : reg_ckg_isdbt_outer1x
    //         [0]  : disable clock
    //         [1]  : invert clock
    //         [3:2]: Select clock source
    //                00: isdbt_clk6_lat (6 MHz)
    //                01: isdbt_clk8_lat (8 MHz)
    //                10: reserved
    //                11: DFT_CLK
    // [6:4]: reg_ckg_isdbt_outer4x
    //         [0]  : disable clock
    //         [1]  : invert clock
    //         [3:2]: Select clock source
    //                00: isdbt_clk24_lat(24 MHz)
    //                01: isdbt_clk32_lat(32 MHz)
    //                10: reserved
    //                11: DFT_CLK
    // [10:8]: reg_ckg_isdbt_outer6x
    //         [0]  : disable clock
    //         [1]  : invert clock
    //         [2]	: Select clock source
    //                00: isdbt_clk36_lat(36 MHz)
    //                01: isdbt_clk48_lat(48 MHz)
    //                10: reserved
    //                11: DFT_CLK
    // [14:12]: reg_ckg_isdbt_outer12x
    //         [0]  : disable clock
    //         [1]  : invert clock
    //         [2]	: Select clock source
    //                00: isdbt_clk72_lat(72 MHz)
    //                01: isdbt_clk96_lat(96 MHz)
    //                10: reserved
    //                11: DFT_CLK
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h22, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h22, 2'b11, 16'h0000);
//    HAL_DMD_RIU_WriteByte(0x111f45,0x00);
//    HAL_DMD_RIU_WriteByte(0x111f44,0x00);

    // Enable ISDBT clk_outer_div
    // reg_clk_isdbt_outer_div_en[0]
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h23, 2'b01, 16'h0001);// enable isdbt outer div clock
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h23, 2'b01, 16'h0001);// enable isdbt outer div clock
//    HAL_DMD_RIU_WriteByte(0x111f46,0x01);

    // enable clk_dvbtc_sram4_isdbt_inner4x & clk_adc1x_eq1x clock
    // [1:0]  : reg_ckg_dvbtc_sram4_isdbt_inner4x
    //          [0]: disable clock
    //          [1]: invert clock
    // [5:4]  : reg_ckg_dvbtc_sram4_isdbt_outer6x
    //          [0]: disable clock
    //          [1]: invert clock
    // [9:8]  : reg_ckg_adc1x_eq1x
    //          [0]: disable clock
    //          [1]: invert clock
    // [13:12] : reg_ckg_adc0p5x_eq0p5x
    //          [0]: disable clock
    //          [1]: invert clock
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h24, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h24, 2'b11, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x111f49,0x00);
    HAL_DMD_RIU_WriteByte(0x111f48,0x00);

    // [1:0]  : reg_ckg_isdbt_outer6x_dvbt_inner1x
    //          [0]: disable clock
    //          [1]: invert clock
    // [5:4]  : reg_ckg_isdbt_outer6x_dvbt_inner2x
    //          [0]: disable clock
    //          [1]: invert clock
    // [9:8]  : reg_ckg_isdbt_outer6x_dvbt_outer2x
    //          [0]: disable clock
    //          [1]: invert clock
    // [13:12]: reg_ckg_isdbt_outer6x_dvbt_outer2x_c
    //          [0]: disable clock
    //          [1]: invert clock
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h25, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h25, 2'b11, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x111f4b,0x00);
    HAL_DMD_RIU_WriteByte(0x111f4a,0x00);

    // enable isdbt outer clock_rs
    // [7:4] : reg_ckg_isdbt_outer_rs
    //         [0]  : disable clock
    //         [1]  : invert clock
    //         [3:2]: Select clock source
    //                00: isdbt_clk36_lat (36 MHz)
    //                01: isdbt_clk48_lat (48 MHz)
    //                10: clk_dmplldiv3_div4(72 MHz)
    //                11: isdbt_clk96_buf (96 MHz)
    // enable share isdbt &dvbt logic clock
    // [1:0]  : reg_ckg_isdbt_inner2x_dvbt_inner2x
    //          [0]: disable clock
    //          [1]: invert clock
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h26, 2'b01, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h26, 2'b01, 16'h0000);
//    HAL_DMD_RIU_WriteByte(0x111f4c,0x00);
	HAL_DMD_RIU_WriteByte(0x111f4d,0x00);
	HAL_DMD_RIU_WriteByte(0x111f4c,0x10);

    // enable vif clock
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h0e, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h0e, 2'b11, 16'h0000);
//    HAL_DMD_RIU_WriteByte(0x111f1d,0x00);
//    HAL_DMD_RIU_WriteByte(0x111f1c,0x00);

    // enable DEMODE-DMA clock
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h10, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h10, 2'b11, 16'h0000);
//    HAL_DMD_RIU_WriteByte(0x111f21,0x00);
//    HAL_DMD_RIU_WriteByte(0x111f20,0x00);

    // select clock
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h11, 2'b11, 16'h0444);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h11, 2'b11, 16'h0444);
    HAL_DMD_RIU_WriteByte(0x111f23,0x04);
    HAL_DMD_RIU_WriteByte(0x111f22,0x44);


	// [15:12]: reg_ckg_dtmb_sram_dump
	// [0]  : disable clock
	// [1]  : invert clock
	// [3:2]: Select clock source 
	//		  00: dtmb_clk18_buf(16 MHz)		
	//		  01: dtmb_sram_dump_clk144_buf(128 MHz)	
	//		  10: dtmb_sram_dump_clk216_buf(192 MHz)
	// 		  11: dtmb_sram_dump_dmplldiv5_buf(153.6 MHz)
    HAL_DMD_RIU_WriteByte(0x111f71,0x18);
    HAL_DMD_RIU_WriteByte(0x111f70,0x81);

    HAL_DMD_RIU_WriteByte(0x111f77,0x00);
    HAL_DMD_RIU_WriteByte(0x111f76,0x00);

    HAL_DMD_RIU_WriteByte(0x111f4f,0x0c);
    HAL_DMD_RIU_WriteByte(0x111f4e,0x40);

    HAL_DMD_RIU_WriteByte(0x111f51,0x48);
    HAL_DMD_RIU_WriteByte(0x111f50,0x44);

    // Enable SAWLESS clock
    // reg_ckg_adcd_d2 @0x12[3:0]
    // reg_ckg_adcd_d4 @0x12[7:4]
    // reg_ckg_adcd_d6 @0x12[11:8]
    // reg_ckg_adcd_d12@0x12[15:12]
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h12, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h12, 2'b11, 16'h0000);
    // wriu 0x111f25 0x00
    // wriu 0x111f24 0x00
//    HAL_DMD_RIU_WriteByte(0x111f25,0x00);
//    HAL_DMD_RIU_WriteByte(0x111f24,0x00);

    // ----------------------------------------------
    //  start demod CLKGEN setting
    // ----------------------------------------------

    // reg_allpad_in=0
    // `RIU_W((`RIUBASE_CHIP>>1)+7'h50, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CHIP>>1)+7'h50, 2'b11, 16'h0000);
    // HAL_DMD_RIU_WriteByte(0x101ea1,0x00);
    // HAL_DMD_RIU_WriteByte(0x101ea0,0x00);

    // reg_ts1config=2
    // `RIU_W((`RIUBASE_CHIP>>1)+7'h57, 2'b11, 16'h1000);
    // `RIU_W((`RIUBASE_CHIP>>1)+7'h57, 2'b11, 16'h1000);
    //HAL_DMD_RIU_WriteByte(0x101eaf,0x10);
    //HAL_DMD_RIU_WriteByte(0x101eae,0x00);

    //  select DMD MCU
    // `RIU_W((`RIUBASE_CHIP>>1)+7'h1c, 2'b10, 16'h0300);
    // `RIU_W((`RIUBASE_CHIP>>1)+7'h1c, 2'b10, 16'h0300);
    // begin BY temp patch
//    HAL_DMD_RIU_WriteByte(0x1120A0,0x00);                        // wreg vdbank_DMD_ANA_MISC+0x20 0x0000
//    HAL_DMD_RIU_WriteByte(0x1120A1,0x00);                        // wreg vdbank_DMD_ANA_MISC+0x20 0x0000
    // end
//    HAL_DMD_RIU_WriteByte(0x101e39,0x03);
    // ----------------------------------------------
    //  Turn TSP
    // ----------------------------------------------
    // turn on ts1_clk, ts0_clk
    // `RIU_W((`RIUBASE_CLKGEN0>>1)+7'h28, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN0>>1)+7'h28, 2'b11, 16'h0000);
    // check TSP work or not
    //HAL_DMD_RIU_WriteByte(0x100b51,0x00);
    //HAL_DMD_RIU_WriteByte(0x100b50,0x00);

    // stream2miu_en, activate rst_wadr
    // `RIU_W((`RIUBASE_TSP0 >>1) + `REG16_HW_CONFIG8, 2'b11, 16'h0012);
//    HAL_DMD_RIU_WriteByte(0x103c0e,0x01);
    // stream2miu_en, turn off rst_wadr
    // `RIU_W((`RIUBASE_TSP0 >>1) + `REG16_HW_CONFIG8, 2'b11, 16'h0102);
    // wriu 0x000e13 0x01
    //HAL_DMD_RIU_WriteByte(0x000e13,0x01);
//    udatatemp = HAL_DMD_RIU_ReadByte(0x000e13);
//    HAL_DMD_RIU_WriteByte(0x000e13, udatatemp&0xFB);//Set 0e12,Bit10=0,
    HAL_DMD_RIU_WriteByte(0x101e39,0x03);
}

/***********************************************************************************
  Subject:    Power on initialized function
  Function:   INTERN_DVBT_Power_On_Initialization
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/

MS_BOOL INTERN_DVBT_Power_On_Initialization(MS_BOOL bRFAGCTristateEnable, MS_U8 u8ADCIQMode, MS_U8 u8PadSel, MS_BOOL bPGAEnable, MS_U8 u8PGAGain, const MS_U8 *u8DMD_DVBT_DSPRegInitExt, MS_U8 u8DMD_DVBT_DSPRegInitSize)
{
    MS_U16            status = true;
    MS_U8   cData = 0;
    //U8            cal_done;
    DBG_INTERN_DVBT(printf("INTERN_DVBT_Power_On_Initialization\n"));

#if defined(PWS_ENABLE)
    Mapi_PWS_Stop_VDMCU();
#endif

    INTERN_DVBT_InitClkgen(bRFAGCTristateEnable);
    HAL_DMD_ADC_IQ_Switch(u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain);
    //// Firmware download //////////
    DBG_INTERN_DVBT(printf("INTERN_DVBT Load DSP...\n"));
    //MsOS_DelayTask(100);

    //if (HAL_DMD_RIU_ReadByte(0x101E3E) != 0x02) // DVBT = BIT1 -> 0x02
    {
        if (INTERN_DVBT_LoadDSPCode() == FALSE)
        {
            printf("DVB-T Load DSP Code Fail\n");
            return FALSE;
        }
        else
        {
            DBG_INTERN_DVBT(printf("DVB-T Load DSP Code OK\n"));
        }
    }


    //// MCU Reset //////////
    DBG_INTERN_DVBT(printf("INTERN_DVBT Reset...\n"));
    if (INTERN_DVBT_Reset() == FALSE)
    {
        DBG_INTERN_DVBT(printf("Fail\n"));
        return FALSE;
    }
    else
    {
        DBG_INTERN_DVBT(printf("OK\n"));
    }

    // reset FDP
    MDrv_SYS_DMD_VD_MBX_WriteReg(0x2200, 0xFF);
    // SRAM setting, DVB-T use it.
    // 0x2204, Bit0, 0:DVB-T use, 1: VIF use
    MDrv_SYS_DMD_VD_MBX_ReadReg(0x2204,&cData);
    MDrv_SYS_DMD_VD_MBX_WriteReg(0x2204, cData&0xFE);

    status &= INTERN_DVBT_DSPReg_Init(u8DMD_DVBT_DSPRegInitExt, u8DMD_DVBT_DSPRegInitSize);
    return status;
}

/************************************************************************************************
  Subject:    Driving control
  Function:   INTERN_DVBT_Driving_Control
  Parmeter:   bInversionEnable : TRUE For High
  Return:      void
  Remark:
*************************************************************************************************/
void INTERN_DVBT_Driving_Control(MS_BOOL bEnable)
{
    MS_U8    u8Temp;

    u8Temp = HAL_DMD_RIU_ReadByte(0x101E10);

    if (bEnable)
    {
       u8Temp = u8Temp | 0x01; //bit0: clk, bit1~8:data , bit9: sync, bit10:valid
    }
    else
    {
       u8Temp = u8Temp & (~0x01);
    }

    DBG_INTERN_DVBT(printf("---> INTERN_DVBT_Driving_Control(Bit0) = 0x%x \n",u8Temp));
    HAL_DMD_RIU_WriteByte(0x101E10, u8Temp);
}
/************************************************************************************************
  Subject:    Clk Inversion control
  Function:   INTERN_DVBT_Clk_Inversion_Control
  Parmeter:   bInversionEnable : TRUE For Inversion Action
  Return:      void
  Remark:
*************************************************************************************************/
void INTERN_DVBT_Clk_Inversion_Control(MS_BOOL bInversionEnable)
{
    MS_U8   u8Temp;

    u8Temp = HAL_DMD_RIU_ReadByte(0x103301);

    if (bInversionEnable)
    {
       u8Temp = u8Temp | 0x02; //bit 9: clk inv
    }
    else
    {
       u8Temp = u8Temp & (~0x02);
    }

    DBG_INTERN_DVBT(printf("---> Inversion(Bit9) = 0x%x \n",u8Temp));
    HAL_DMD_RIU_WriteByte(0x103301, u8Temp);
}
/************************************************************************************************
  Subject:    Transport stream serial/parallel control
  Function:   INTERN_DVBT_Serial_Control
  Parmeter:   bEnable : TRUE For serial
  Return:     MS_BOOL :
  Remark:
*************************************************************************************************/
MS_BOOL INTERN_DVBT_Serial_Control(MS_BOOL bEnable, MS_U8 u8TSClk)
{
    MS_U8   status = true;
    MS_U8 temp_val;
    DBG_INTERN_DVBT(printf(" @INTERN_DVBT_ts... u8TSClk=%d\n",u8TSClk));

    if (u8TSClk == 0xFF) u8TSClk=0x13;
    if (bEnable)    //Serial mode for TS pad
    {
        // serial
        HAL_DMD_RIU_WriteByte(0x103308, 0x01);   // serial mode: 0x0401
        HAL_DMD_RIU_WriteByte(0x103309, 0x04);   // reg_ckg_dvbtc_ts@0x04

        HAL_DMD_RIU_WriteByte(0x103300, 0x00);   // serial mode 0x0400
#if(INTERN_DVBT_TS_SERIAL_INVERSION == 0)
        //HAL_DMD_RIU_WriteByte(0x103301, 0x04);   // reg_ckg_dvbtmk_ts_out_mode@0x00

        temp_val=HAL_DMD_RIU_ReadByte(0x103301);
        temp_val|=0x04;
        HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#else
        //HAL_DMD_RIU_WriteByte(0x103301, 0x07);   // reg_ckg_dvbtmk_ts_out_mode@0x00
        temp_val=HAL_DMD_RIU_ReadByte(0x103301);
        temp_val|=0x07;
        HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#endif
        //move to drvSYS HAL_DMD_RIU_WriteByte(0x101EA5, HAL_DMD_RIU_ReadByte(0x101EA5)&0xEF);   // PAD_TS1 is used as output
        //move to drvSYS HAL_DMD_RIU_WriteByte(0x101EAF, HAL_DMD_RIU_ReadByte(0x101EAF)&0xE3);   // PAD_TS1 Disable TS CLK PAD

        //// INTERN_DVBT TS Control: Serial //////////
        gsCmdPacket.cmd_code = CMD_TS_CTRL;

        gsCmdPacket.param[0] = TS_SERIAL;
#if(INTERN_DVBT_TS_SERIAL_INVERSION == 0)
        gsCmdPacket.param[1] = 0;//TS_CLK_NO_INV;
#else
        gsCmdPacket.param[1] = 1;//TS_CLK_INVERSE;
#endif
        status &= INTERN_DVBT_Cmd_Packet_Send(&gsCmdPacket, 2);
    }
    else
    {
        //parallel
        HAL_DMD_RIU_WriteByte(0x103308, 0x01);   // parallel mode:0x0001
        HAL_DMD_RIU_WriteByte(0x103309, 0x00);   // reg_ckg_dvbtc_ts@0x04

        //HAL_DMD_RIU_WriteByte(0x103300, 0x11);   // parallel mode: 0x0511 => ts_clk=288/(2*(0x11+1))=8MHz
        HAL_DMD_RIU_WriteByte(0x103300, u8TSClk);   // parallel mode: 0x0513 => ts_clk=288/(2*(0x16+1))=6.26MHz //@@++--
#if(INTERN_DVBT_TS_PARALLEL_INVERSION == 0)
        //HAL_DMD_RIU_WriteByte(0x103301, 0x05);   // reg_ckg_dvbtmk_ts_out_mode@0x00
        temp_val=HAL_DMD_RIU_ReadByte(0x103301);
        temp_val|=0x05;
        HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#else
        //HAL_DMD_RIU_WriteByte(0x103301, 0x07);   // reg_ckg_dvbtmk_ts_out_mode@0x00
        temp_val=HAL_DMD_RIU_ReadByte(0x103301);
        temp_val|=0x07;
        HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#endif

        //move to drvSYS HAL_DMD_RIU_WriteByte(0x101EA5, HAL_DMD_RIU_ReadByte(0x101EA5)|0x10);   // PAD_TS1 is used as output
        //move to drvSYS HAL_DMD_RIU_WriteByte(0x101EAF, (HAL_DMD_RIU_ReadByte(0x101EAF)&0xC0)|0x11);   // PAD_TS1 enable TS clk pad

        //// INTERN_DVBT TS Control: Parallel //////////
        gsCmdPacket.cmd_code = CMD_TS_CTRL;

        gsCmdPacket.param[0] = TS_PARALLEL;
#if(INTERN_DVBT_TS_PARALLEL_INVERSION == 0)
        gsCmdPacket.param[1] = 0;//TS_CLK_NO_INV;
#else
        gsCmdPacket.param[1] = 1;//TS_CLK_INVERSE;
#endif
        status &= INTERN_DVBT_Cmd_Packet_Send(&gsCmdPacket, 2);
    }

    DBG_INTERN_DVBT(printf("---> Inversion(Bit5) = 0x%x \n",gsCmdPacket.param[1] ));

    INTERN_DVBT_Driving_Control(INTERN_DVBT_DTV_DRIVING_LEVEL);
    return status;
}

/************************************************************************************************
  Subject:    TS1 output control
  Function:   INTERN_DVBT_PAD_TS1_Enable
  Parmeter:   flag : TRUE For Turn on TS1, FALSE For Turn off TS1
  Return:     void
  Remark:
*************************************************************************************************/
void INTERN_DVBT_PAD_TS1_Enable(MS_BOOL flag)
{
    DBG_INTERN_DVBT(printf(" @INTERN_DVBT_TS1_Enable... \n"));

    if(flag) // PAD_TS1 Enable TS CLK PAD
    {
        //printf("=== TS1_Enable ===\n");
        //HAL_DMD_RIU_WriteByte(0x101EA5, HAL_DMD_RIU_ReadByte(0x101EA5)|0x10);   //For T3
        //HAL_DMD_RIU_WriteByte(0x101EAF, HAL_DMD_RIU_ReadByte(0x101EAF)|0x18);   //For T4
        //move to drvSYS HAL_DMD_RIU_WriteByte(0x101EAF, HAL_DMD_RIU_ReadByte(0x101EAF)|0x11);   //For T8
    }
    else // PAD_TS1 Disable TS CLK PAD
    {
        //printf("=== TS1_Disable ===\n");
        //HAL_DMD_RIU_WriteByte(0x101EA5, HAL_DMD_RIU_ReadByte(0x101EA5)&0xEF);   //For T3
        //HAL_DMD_RIU_WriteByte(0x101EAF, HAL_DMD_RIU_ReadByte(0x101EAF)&0xE3);   //For T4
        //move to drvSYS HAL_DMD_RIU_WriteByte(0x101EAF, HAL_DMD_RIU_ReadByte(0x101EAF)&0xC0);   //For T8
    }
}

/************************************************************************************************
  Subject:    channel change config
  Function:   INTERN_DVBT_Config
  Parmeter:   BW: bandwidth
  Return:     MS_BOOL :
  Remark:
*************************************************************************************************/
MS_BOOL INTERN_DVBT_Config ( DMD_RF_CHANNEL_BANDWIDTH BW, MS_BOOL bSerialTS, MS_BOOL bPalBG, MS_BOOL bLPSel, MS_U8 u8TSClk, MS_U32 u32IFFreq, MS_U32 u32FSFreq, MS_U8 u8IQSwap)
{
    MS_U8   bandwidth;
    MS_U8   status = true;
    MS_U8 temp_val;
    DBG_INTERN_DVBT(printf(" @INTERN_DVBT_config %d %d %d %d %d %ld %ld %d\n", BW, bSerialTS, bPalBG, bLPSel, u8TSClk, u32IFFreq, u32FSFreq, u8IQSwap));
    DBG_INTERN_DVBT_TIME(printf("INTERN_DVBT_Config, t = %ld\n",MsOS_GetSystemTime()));

    if (u8TSClk == 0xFF) u8TSClk=0x13;
    switch(BW)
    {
        case E_DMD_RF_CH_BAND_6MHz:
            bandwidth = 1;
            break;
        case E_DMD_RF_CH_BAND_7MHz:
            bandwidth = 2;
            break;
        case E_DMD_RF_CH_BAND_8MHz:
        default:
            bandwidth = 3;
            break;
    }

    status &= INTERN_DVBT_Reset();

    // BW mode
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBT_N_CFG_BW, bandwidth);
    // TS mode
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBT_N_CFG_TS_SERIAL, bSerialTS? 0x01:0x00);
    // For Analog CCI 0:PAL B/G (Nordig), 1:PAL I (D-Book)
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBT_N_CFG_CCI, bPalBG? 0x00:0x01);
    // Hierarchy mode
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBT_N_CFG_LP_SEL, bLPSel? 0x01:0x00);
    // FC
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBT_N_CFG_FC_L, (abs(DVBT_FS-u32IFFreq))&0xff);
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBT_N_CFG_FC_H, (abs((DVBT_FS-u32IFFreq))>>8)&0xff);
    // FS
    //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBT_N_CFG_FS_L, (u32FSFreq));
    //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBT_N_CFG_FS_H, (u32FSFreq)>>8);
    // IQSwap
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBT_N_CFG_IQ_SWAP, (u8IQSwap));

    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBT_N_CFG_LIF, (u32IFFreq < 10000) ? 1 : 0);
    // Fif
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBT_N_CFG_FIF_L, (u32IFFreq)&0xff);
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_DVBT_N_CFG_FIF_H, (u32IFFreq>>8)&0xff);

//// INTERN_DVBT system init: DVB-T //////////
    gsCmdPacket.cmd_code = CMD_SYSTEM_INIT;

    gsCmdPacket.param[0] = E_SYS_DVBT;
    status &= INTERN_DVBT_Cmd_Packet_Send(&gsCmdPacket, 1);

    if(bSerialTS)
    {
        // serial
        HAL_DMD_RIU_WriteByte(0x103308, 0x01);   // parallel mode:0x0001 / serial mode: 0x0401
        HAL_DMD_RIU_WriteByte(0x103309, 0x04);   // reg_ckg_dvbtc_ts@0x04

        HAL_DMD_RIU_WriteByte(0x103300, 0x00);   // parallel mode: 0x0511 /serial mode 0x0400
#if(INTERN_DVBT_TS_SERIAL_INVERSION == 0)
//        HAL_DMD_RIU_WriteByte(0x103301, 0x04);   // reg_ckg_dvbtmk_ts_out_mode@0x00
        temp_val=HAL_DMD_RIU_ReadByte(0x103301);
        temp_val|=0x04;
        HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#else
        //HAL_DMD_RIU_WriteByte(0x103301, 0x07);   // reg_ckg_dvbtmk_ts_out_mode@0x00
        temp_val=HAL_DMD_RIU_ReadByte(0x103301);
        temp_val|=0x07;
        HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#endif
    }
    else
    {
        //parallel
        HAL_DMD_RIU_WriteByte(0x103308, 0x01);   // parallel mode:0x0001 / serial mode: 0x0401
        HAL_DMD_RIU_WriteByte(0x103309, 0x00);   // reg_ckg_dvbtc_ts@0x04

        //HAL_DMD_RIU_WriteByte(0x103300, 0x11);   // parallel mode: 0x0511 => ts_clk=288/(2*(0x11+1))=8MHz
        HAL_DMD_RIU_WriteByte(0x103300, u8TSClk);  // parallel mode: 0x0513 => ts_clk=288/(2*(0x16+1))=6.26MHz //@@++--
#if(INTERN_DVBT_TS_PARALLEL_INVERSION == 0)
//        HAL_DMD_RIU_WriteByte(0x103301, 0x05);   // reg_ckg_dvbtmk_ts_out_mode@0x00
        temp_val=HAL_DMD_RIU_ReadByte(0x103301);
        temp_val|=0x05;
        HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#else
        HAL_DMD_RIU_WriteByte(0x103301, 0x07);   // reg_ckg_dvbtmk_ts_out_mode@0x00
#endif
    }
    return status;
}
/************************************************************************************************
  Subject:    enable hw to lock channel
  Function:   INTERN_DVBT_Active
  Parmeter:   bEnable
  Return:     MS_BOOL
  Remark:
*************************************************************************************************/
MS_BOOL INTERN_DVBT_Active(MS_BOOL bEnable)
{
    MS_U8   status = true;

    DBG_INTERN_DVBT(printf(" @INTERN_DVBT_active\n"));

    //// INTERN_DVBT Finite State Machine on/off //////////
    #if 0
    gsCmdPacket.cmd_code = CMD_FSM_CTRL;

    gsCmdPacket.param[0] = (MS_U8)bEnable;
    status &= INTERN_DVBT_Cmd_Packet_Send(&gsCmdPacket, 1);
    #else
    HAL_DMD_RIU_WriteByte(MBRegBase + (0x0e)*2, 0x01);   // FSM_EN
    #endif
    INTERN_DVBT_SignalQualityReset();

    return status;
}
/************************************************************************************************
  Subject:    Return lock status
  Function:   INTERN_DVBT_Lock
  Parmeter:   eStatus :
  Return:     MS_BOOL
  Remark:
*************************************************************************************************/
DMD_LOCK_STATUS INTERN_DVBT_Lock(MS_U16 u16DMD_DVBT_TPS_Timeout, MS_U16 u16DMD_DVBT_FEC_Timeout)
{
    float fBER=0.0f;

    if (INTERN_DVBT_GetLock(E_DMD_COFDM_FEC_LOCK))
    {
        if (bFECLock ==  FALSE)
        {
            u32FecFirstLockTime = MsOS_GetSystemTime();
            DBG_INTERN_DVBT(printf("++++++++[utopia]dvbt lock\n"));
        }

        if(INTERN_DVBT_GetPostViterbiBer(&fBER) == TRUE)
        {
            if((fBER > 1.0E-8) && (fBER < 1.0E-1))
            {
                if(fViterbiBerFiltered <= 0.0)
                    fViterbiBerFiltered = fBER;
                else
                    fViterbiBerFiltered = 0.9f*fViterbiBerFiltered+0.1f*fBER;
            }
            DBG_INTERN_DVBT(printf("[dvbt]f_ber=%8.3e, g_viter_ber=%8.3e\n",fBER,fViterbiBerFiltered));
        }
        u32FecLastLockTime = MsOS_GetSystemTime();
        bFECLock = TRUE;
        return E_DMD_LOCK;
    }
    else
    {
        INTERN_DVBT_SignalQualityReset();
        if (bFECLock == TRUE)
        {
            if ((MsOS_GetSystemTime() - u32FecLastLockTime) < 1000)
            {
                return E_DMD_LOCK;
            }
        }
        bFECLock = FALSE;
    }

    if(!bTPSLock)
    {
        if (INTERN_DVBT_GetLock(E_DMD_COFDM_TPS_EVER_LOCK))
        {
            DBG_INTERN_DVBT(printf("==> INTERN_DVBT_Lock -- TPSLock \n"););
            bTPSLock = TRUE;
        }
    }
    if(bTPSLock)
    {
        DBG_INTERN_DVBT(printf("TPSLock %ld\n",MsOS_GetSystemTime()));
        if(MsOS_Timer_DiffTimeFromNow(u32ChkScanTimeStart) < u16DMD_DVBT_FEC_Timeout)
        {
            return E_DMD_CHECKING;
        }
    }
    else
    {
        if(MsOS_Timer_DiffTimeFromNow(u32ChkScanTimeStart) < u16DMD_DVBT_TPS_Timeout)
        {
            return E_DMD_CHECKING;
        }
    }
    return E_DMD_UNLOCK;

}


MS_BOOL INTERN_DVBT_GetLock(DMD_DVBT_GETLOCK_TYPE eStatus)
{
    MS_U16 u16Address = 0;
    MS_U8 cData = 0;
    MS_U8 cBitMask = 0;

    switch( eStatus )
    {
        case E_DMD_COFDM_FEC_LOCK:
            MDrv_SYS_DMD_VD_MBX_ReadReg(0x23E0, &cData);

            if (cData == 0x0B)
            {
                return TRUE;
            }
            else
            {
                return FALSE;      // continuously un-lock
            }
            break;

        case E_DMD_COFDM_PSYNC_LOCK:
            u16Address =  0x232C; //FEC: P-sync Lock,
            cBitMask = BIT(1);
            break;

        case E_DMD_COFDM_TPS_LOCK:
            u16Address =  0x2222; //TPS HW Lock,
            cBitMask = BIT(1);
            break;

        case E_DMD_COFDM_DCR_LOCK:
            u16Address =  0x2145; //DCR Lock,
            cBitMask = BIT(0);
            break;

        case E_DMD_COFDM_AGC_LOCK:
            u16Address =  0x212F; //AGC Lock,
            cBitMask = BIT(0);
            break;

        case E_DMD_COFDM_MODE_DET:
            u16Address =  0x24CF; //Mode CP Detect,
            cBitMask = BIT(4);
            break;

        case E_DMD_COFDM_TPS_EVER_LOCK:
            u16Address =  0x20C0;  //TPS Ever Lock,
            cBitMask = BIT(3);
            break;

        default:
            return FALSE;
    }

    if (MDrv_SYS_DMD_VD_MBX_ReadReg(u16Address, &cData) == FALSE)
        return FALSE;

    if ((cData & cBitMask) == cBitMask)
    {
        return TRUE;
    }

    return FALSE;

}

/****************************************************************************
  Subject:    To get the Post viterbi BER
  Function:   INTERN_DVBT_GetPostViterbiBer
  Parmeter:  Quility
  Return:       E_RESULT_SUCCESS
                   E_RESULT_FAILURE =>Read I2C fail, INTERN_DVBT_VIT_STATUS_NG
  Remark:     For the Performance issue, here we just return the Post Value.(Not BER)
                   We will not read the Period, and have the "/256/8"
*****************************************************************************/
MS_BOOL INTERN_DVBT_GetPostViterbiBer(float *ber)
{
    MS_BOOL            status = true;
    MS_U8            reg=0, reg_frz=0;
    MS_U16            BitErrPeriod;
    MS_U32            BitErr;
    MS_U16            PktErr;

    /////////// Post-Viterbi BER /////////////

    if ( MsOS_Timer_DiffTimeFromNow(u32FecFirstLockTime) < 300)
    {
        *ber = (float)-1.0;
        return false;
    }
    // bank 1f 0x03 [1:0] reg_bit_err_num_freeze
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x03, &reg_frz);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(BACKEND_REG_BASE+0x03, reg_frz|0x03);

    // bank 1f 0x46 [7:0] reg_bit_err_sblprd_7_0
    //             0x47 [15:8] reg_bit_err_sblprd_15_8
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x47, &reg);
    BitErrPeriod = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x46, &reg);
    BitErrPeriod = (BitErrPeriod << 8)|reg;

    // bank 1f 0x6a [7:0] reg_bit_err_num_7_0
    //             0x6b [15:8] reg_bit_err_num_15_8
    // bank 1f 0x6c [7:0] reg_bit_err_num_23_16
    //             0x6d [15:8] reg_bit_err_num_31_24
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x6d, &reg);
    BitErr = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x6c, &reg);
    BitErr = (BitErr << 8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x6b, &reg);
    BitErr = (BitErr << 8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x6a, &reg);
    BitErr = (BitErr << 8)|reg;

    // bank 1f 0x66 [7:0] reg_uncrt_pkt_num_7_0
    //             0x67 [15:8] reg_uncrt_pkt_num_15_8
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x67, &reg);
    PktErr = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x66, &reg);
    PktErr = (PktErr << 8)|reg;

    // bank 1f 0x03 [1:0] reg_bit_err_num_freeze
    reg_frz=reg_frz&(~0x03);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(BACKEND_REG_BASE+0x03, reg_frz);

    if (BitErrPeriod == 0 )    //protect 0
        BitErrPeriod = 1;

    if (BitErr <=0 )
        *ber = 0.5f / ((float)BitErrPeriod*128*188*8);
    else
        *ber = (float)BitErr / ((float)BitErrPeriod*128*188*8);


    DBG_GET_SIGNAL(printf("INTERN_DVBT PostVitBER = %8.3e \n ", *ber));
    DBG_GET_SIGNAL(printf("INTERN_DVBT PktErr = %d \n ", (int)PktErr));

    return status;
}

/****************************************************************************
  Subject:    To get the Pre viterbi BER
  Function:   INTERN_DVBT_GetPreViterbiBer
  Parmeter:   ber
  Return:     E_RESULT_SUCCESS
                   E_RESULT_FAILURE =>Read I2C fail, INTERN_DVBT_VIT_STATUS_NG
  Remark:     For the Performance issue, here we just return the Post Value.(Not BER)
                   We will not read the Period, and have the "/256/8"
*****************************************************************************/
MS_BOOL INTERN_DVBT_GetPreViterbiBer(float *ber)
{
    MS_U8            status = true;
    MS_U8            reg=0, reg_frz=0;
    MS_U16           BitErrPeriod;
    MS_U32           BitErr;
    MS_BOOL         BEROver;

    // bank 7 0x10 [3] reg_rd_freezeber
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FEC_REG_BASE + 0x10, &reg_frz);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FEC_REG_BASE + 0x10, reg_frz|0x08);

    // bank 7 0x16 [7:0] reg_ber_timerl
    //             [15:8] reg_ber_timerm
    // bank 7 0x18 [5:0] reg_ber_timerh
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FEC_REG_BASE + 0x18, &reg);
    BitErrPeriod = reg&0x3f;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FEC_REG_BASE + 0x17, &reg);
    BitErrPeriod = (BitErrPeriod << 8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FEC_REG_BASE + 0x16, &reg);
    BitErrPeriod = (BitErrPeriod << 8)|reg;

    // bank 7 0x1e [7:0] reg_ber_7_0
    //             [15:8] reg_ber_15_8
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FEC_REG_BASE + 0x1F, &reg);
    BitErr = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FEC_REG_BASE + 0x1E, &reg);
    BitErr = (BitErr << 8)|reg;

    // bank 7 0x1a [13:8] reg_cor_intstat_reg
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FEC_REG_BASE + 0x1B, &reg);
    if (reg & 0x10)
        BEROver = true;
    else
        BEROver = false;

    if (BitErrPeriod ==0 )//protect 0
    	BitErrPeriod=1;

    if (BEROver)
    {
        *ber = 1;
        printf("BER is over\n");
    }
    else
    {
        if (BitErr <=0 )
        *ber=0.5 / (float)(BitErrPeriod * 256);
        else
        *ber=(float)(BitErr) / (float)(BitErrPeriod * 256);
    }

    // bank 7 0x10 [3] reg_rd_freezeber
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FEC_REG_BASE + 0x10, reg_frz);

    return status;
}

/****************************************************************************
  Subject:    To get the Packet error
  Function:   INTERN_DVBT_GetPacketErr
  Parmeter:   pktErr
  Return:     E_RESULT_SUCCESS
                   E_RESULT_FAILURE =>Read I2C fail, INTERN_DVBT_VIT_STATUS_NG
  Remark:     For the Performance issue, here we just return the Post Value.(Not BER)
                   We will not read the Period, and have the "/256/8"
*****************************************************************************/
MS_BOOL INTERN_DVBT_GetPacketErr(MS_U16 *u16PktErr)
{
    MS_BOOL          status = true;
    MS_U8            reg = 0, reg_frz = 0;
    MS_U16           PktErr;

    // bank 1f 0x03 [1:0] reg_bit_err_num_freeze
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x03, &reg_frz);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(BACKEND_REG_BASE+0x03, reg_frz|0x03);

    // bank 1f 0x66 [7:0] reg_uncrt_pkt_num_7_0
    //             0x67 [15:8] reg_uncrt_pkt_num_15_8
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x67, &reg);
    PktErr = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(BACKEND_REG_BASE+0x66, &reg);
    PktErr = (PktErr << 8)|reg;

    // bank 1f 0x03 [1:0] reg_bit_err_num_freeze
    reg_frz=reg_frz&(~0x03);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(BACKEND_REG_BASE+0x03, reg_frz);

    DBG_GET_SIGNAL(printf("INTERN_DVBT PktErr = %d \n ", (int)PktErr));

    *u16PktErr = PktErr;

    return status;
}

/****************************************************************************
  Subject:    To get the DVBT parameter
  Function:   INTERN_DVBT_Get_TPS_Info
  Parmeter:   point to return parameter
              Constellation (b2 ~ b0)  : 0~2 => QPSK, 16QAM, 64QAM
              Hierarchy (b5 ~ b3))     : 0~3 => None, Aplha1, Aplha2, Aplha4
              LP Code Rate (b8 ~ b6)   : 0~4 => 1/2, 2/3, 3/4, 5/6, 7/8
              HP Code Rate (b11 ~ b9)  : 0~4 => 1/2, 2/3, 3/4, 5/6, 7/8
              GI (b13 ~ b12)           : 0~3 => 1/32, 1/16, 1/8, 1/4
              FFT ( b14)          : 0~1 => 2K, 8K
              Priority(bit 15)      : 0~1=> HP,LP
  Return:     TRUE
              FALSE
  Remark:   The TPS parameters will be available after TPS lock
*****************************************************************************/
MS_BOOL INTERN_DVBT_Get_TPS_Info( MS_U16 * TPS_parameter)
{
    MS_U8 u8Temp;

    if (MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x22, &u8Temp) == FALSE)
        return FALSE;

    if ((u8Temp& 0x02) != 0x02)
    {
        return FALSE; //TPS unlock
    }
    else
    {
        if ( MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x24, &u8Temp) == FALSE )
            return FALSE;

        *TPS_parameter = u8Temp & 0x03;         //Constellation (b2 ~ b0)
        *TPS_parameter |= (u8Temp & 0x70) >> 1; //Hierarchy (b5 ~ b3)

        if ( MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x25, &u8Temp) == FALSE )
            return FALSE;

        *TPS_parameter |= (MS_U16)(u8Temp & 0x07) << 6; //LP Code Rate (b8 ~ b6)
        *TPS_parameter |= (MS_U16)(u8Temp & 0x70) << 5; //HP Code Rate (b11 ~ b9)

        if ( MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x26, &u8Temp) == FALSE )
            return FALSE;

        *TPS_parameter |= (MS_U16)(u8Temp & 0x03) << 12; //GI (b13 ~ b12)
        *TPS_parameter |= (MS_U16)(u8Temp & 0x30) << 10;  //FFT ( b14)

        if ( MDrv_SYS_DMD_VD_MBX_ReadReg(FEC_REG_BASE + 0x0C, &u8Temp) == FALSE )
            return FALSE;

        *TPS_parameter |=(MS_U16)(u8Temp&0x08)<<12;//Priority(bit 15)

    }
    return TRUE;
}


/****************************************************************************
  Subject:    Read the signal to noise ratio (SNR)
  Function:   INTERN_DVBT_GetSNR
  Parmeter:   None
  Return:     -1 mean I2C fail, otherwise I2C success then return SNR value
  Remark:
*****************************************************************************/
float INTERN_DVBT_GetSNR (void)
{
    MS_U8            status = true;
    MS_U8            reg=0, reg_frz=0;
    MS_U32           noise_power;
    float         snr;

    // bank 6 0xfe [0] reg_fdp_freeze
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0xfe, &reg_frz);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xfe, reg_frz|0x01);

    // bank 6 0xff [0] reg_fdp_load
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xff, 0x01);

    // bank 6 0x4a [26:0] reg_snr_accu <27,1>
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x4d, &reg);
    noise_power = reg & 0x07;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x4c, &reg);
    noise_power = (noise_power << 8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x4b, &reg);
    noise_power = (noise_power << 8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x4a, &reg);
    noise_power = (noise_power << 8)|reg;

    noise_power = noise_power/2;

    // bank 6 0x26 [5:4] reg_transmission_mode
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x26, &reg);

    // bank 6 0xfe [0] reg_fdp_freeze
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xfe, reg_frz);

    // bank 6 0xff [0] reg_fdp_load
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xff, 0x01);


    if ((reg&0x30)==0x00)     //2K
    {
        if (noise_power<1512)
            snr = 0;
        else
#ifdef MSOS_TYPE_LINUX
            snr = 10*log10f((float)noise_power/1512);
#else
            snr = 10*Log10Approx((float)noise_power/1512);
#endif
    }
    //else if ((reg&0x30)==0x10)//8K
    else
    {
        if (noise_power<6048)
            snr = 0;
        else
#ifdef MSOS_TYPE_LINUX
            snr = 10*log10f((float)noise_power/6048);
#else
            snr = 10*Log10Approx((float)noise_power/6048);
#endif
    }
    /* ignore 4K
    else                       //4K
    {
      if (noise_power<3024)
        snr = 0;
      else
        snr = 10*Log10Approx(noise_power/3024);
    }
    */

    if (status == true)
        return snr;
    else
        return -1;

}

/****************************************************************************
  Subject:    To check if Hierarchy on
  Function:   INTERN_DVBT_Is_HierarchyOn
  Parmeter:
  Return:     BOOLEAN
*****************************************************************************/
MS_BOOL INTERN_DVBT_Is_HierarchyOn( void)
{
    MS_U16 u16_tmp;

    if(INTERN_DVBT_Get_TPS_Info(&u16_tmp) == FALSE)
        return FALSE;
    //printf("u16_tmp........%x %x\n",u16_tmp,u16_tmp&0x38);
    if(u16_tmp&0x38)
    {
        return TRUE;
    }
    return FALSE;
}

MS_BOOL INTERN_DVBT_GetSignalStrength(MS_U16 *strength,const DMD_DVBT_InitData *sDMD_DVBT_InitData, MS_U8 u8SarValue, float fRFPowerDbm)
{
    MS_U8   status = true;
    float   ch_power_db = 0.0f;
    float   ch_power_ref = 11.0f;
    float   ch_power_rel = 0.0f;
    MS_U8   u8_index = 0;
    MS_U16  tps_info_qam,tps_info_cr;

    if (FALSE == INTERN_DVBT_GetLock(E_DMD_COFDM_FEC_LOCK) )
    {
        *strength = 0;
        return TRUE;
    }
    DBG_INTERN_DVBT_TIME(printf("INTERN_DVBT_GetSignalStrength, t=%ld\n",MsOS_GetSystemTime()));

    // if (INTERN_DVBT_Lock(COFDM_TPS_LOCK))
        //if (INTERN_DVBT_Lock(COFDM_AGC_LOCK))
        /* Actually, it's more reasonable, that signal level depended on cable input power level
        * thougth the signal isn't dvb-t signal.
        */

    // use pointer of IFAGC table to identify
    // case 1: RFAGC from SAR, IFAGC controlled by demod
    // case 2: RFAGC from tuner, ,IFAGC controlled by demod
    status=HAL_DMD_GetRFLevel(&ch_power_db, fRFPowerDbm, u8SarValue,
                                                                sDMD_DVBT_InitData->pTuner_RfagcSsi, sDMD_DVBT_InitData->u16Tuner_RfagcSsi_Size,
                                                                sDMD_DVBT_InitData->pTuner_IfagcSsi_HiRef, sDMD_DVBT_InitData->u16Tuner_IfagcSsi_HiRef_Size,
                                                                sDMD_DVBT_InitData->pTuner_IfagcSsi_LoRef, sDMD_DVBT_InitData->u16Tuner_IfagcSsi_LoRef_Size,
                                                                sDMD_DVBT_InitData->pTuner_IfagcErr_HiRef, sDMD_DVBT_InitData->u16Tuner_IfagcErr_HiRef_Size,
                                                                sDMD_DVBT_InitData->pTuner_IfagcErr_LoRef, sDMD_DVBT_InitData->u16Tuner_IfagcErr_LoRef_Size);


    if(INTERN_DVBT_Get_TPS_Parameter_Const(&tps_info_qam, TS_MODUL_MODE) == FALSE)
        printf("[dvbt]TPS qam parameter retrieve failure\n");

    if(INTERN_DVBT_Get_TPS_Parameter_Const(&tps_info_cr, TS_CODE_RATE) == FALSE)
        printf("[dvbt]TPS cr parameter retrieve failure\n");


    while(dvbt_ssi_dbm_nordigp1[u8_index].constel != _UNKNOW_QAM)
    {
        if ( (dvbt_ssi_dbm_nordigp1[u8_index].constel == (DMD_CONSTEL)tps_info_qam)
            && (dvbt_ssi_dbm_nordigp1[u8_index].code_rate == (DMD_CODERATE)tps_info_cr))
        {
           ch_power_ref = dvbt_ssi_dbm_nordigp1[u8_index].p_ref;
           break;
        }
        else
        {
           u8_index++;
        }
    }

    if (ch_power_ref > 10.0f)
        *strength = 0;
    else
    {
        ch_power_rel = ch_power_db - ch_power_ref;

        if ( ch_power_rel < -15.0f )
        {
            *strength = 0;
        }
        else if ( ch_power_rel < 0.0f )
        {
            *strength = (MS_U16)(2.0f/3*(ch_power_rel + 15.0f));
        }
        else if ( ch_power_rel < 20 )
        {
            *strength = (MS_U16)(4.0f*ch_power_rel + 10.0f);
        }
        else if ( ch_power_rel < 35.0f )
        {
            *strength = (MS_U16)(2.0f/3*(ch_power_rel - 20.0f) + 90.0f);
        }
        else
        {
            *strength = 100;
        }
    }

    if (FALSE == INTERN_DVBT_GetLock(E_DMD_COFDM_FEC_LOCK) )
    {
        *strength = 0;
        return TRUE;
    }

    DBG_GET_SIGNAL(printf(">>> SSI_CH_PWR(dB) = %f , Score = %d<<<\n", ch_power_db, *strength));
    DBG_GET_SIGNAL(printf(">>> SSI = %d <<<\n", (int)*strength));

    return status;
}

/****************************************************************************
  Subject:    To get the DVT Signal quility
  Function:   INTERN_DVBT_GetSignalQuality
  Parmeter:  Quility
  Return:      E_RESULT_SUCCESS
                   E_RESULT_FAILURE
  Remark:    Here we have 4 level range
                  <1>.First Range => Quility =100  (You can define it by INTERN_DVBT_SIGNAL_BASE_100)
                  <2>.2th Range => 60 < Quality < 100 (You can define it by INTERN_DVBT_SIGNAL_BASE_60)
                  <3>.3th Range => 10 < Quality < 60  (You can define it by INTERN_DVBT_SIGNAL_BASE_10)
                  <4>.4th Range => Quality <10
*****************************************************************************/
MS_BOOL INTERN_DVBT_GetSignalQuality(MS_U16 *quality, const DMD_DVBT_InitData *sDMD_DVBT_InitData, MS_U8 u8SarValue, float fRFPowerDbm)
{
    float   ber_sqi;
    float   fber;
    float   cn_rec = 0;
    float   cn_nordig_p1 = 0;
    float   cn_rel = 0;

    MS_U8   status = true;
    MS_U8   tps_cnstl = 0, tps_cr = 0, i = 0;
    MS_U16  u16_tmp;

    DBG_INTERN_DVBT_TIME(printf("INTERN_DVBT_GetSignalQuality, t=%ld\n",MsOS_GetSystemTime()));

    if (TRUE == INTERN_DVBT_GetLock(E_DMD_COFDM_PSYNC_LOCK) )
    {

        if ( MsOS_Timer_DiffTimeFromNow(u32FecFirstLockTime) < 300)
        {
          MsOS_DelayTask(300 - MsOS_Timer_DiffTimeFromNow(u32FecFirstLockTime));
        }
        ///////// Get Pre-RS (Post-Viterbi) BER to determine BER_SQI //////////
        if(fViterbiBerFiltered<= 0.0)
        {
            if (INTERN_DVBT_GetPostViterbiBer(&fber) == FALSE)
            {
                DBG_INTERN_DVBT(printf("GetPostViterbiBer Fail!\n"));
                return FALSE;
            }
            fViterbiBerFiltered = fber;
        }
        else
        {
            fber = fViterbiBerFiltered;
        }

        if (fber > 1.0E-3)
            ber_sqi = 0.0;
        else if (fber > 8.5E-7)
#ifdef MSOS_TYPE_LINUX
            ber_sqi = (log10f(1.0f/fber))*20.0f - 22.0f;
#else
            ber_sqi = (Log10Approx(1.0f/fber))*20.0f - 22.0f;
#endif
        else
            ber_sqi = 100.0;

        cn_rec = INTERN_DVBT_GetSNR();

        if (cn_rec == -1)   //get SNR return fail
            status = false;

        ///////// Get Constellation and Code Rate to determine Ref. C/N //////////
        ///////// (refer to Teracom min. spec 2.0 4.1.1.7) /////
        tps_cnstl = 0xff;
        tps_cr = 0xff;
        if(INTERN_DVBT_Get_TPS_Parameter_Const( &u16_tmp, TS_MODUL_MODE) == TRUE)
            tps_cnstl = (MS_U8)u16_tmp&0x07;
        if(INTERN_DVBT_Get_TPS_Parameter_Const( &u16_tmp, TS_CODE_RATE) == TRUE)
            tps_cr = (MS_U8)u16_tmp&0x07;

        for(i = 0; i < sDMD_DVBT_InitData->u16SqiCnNordigP1_Size; i++)
        {
            if ( (tps_cnstl == sDMD_DVBT_InitData->pSqiCnNordigP1[i].constel)
            && (tps_cr == sDMD_DVBT_InitData->pSqiCnNordigP1[i].code_rate) )
            {
                cn_nordig_p1 = sDMD_DVBT_InitData->pSqiCnNordigP1[i].cn_ref;
                break;
            }
        }

        // 0,5, snr offset
        cn_rel = cn_rec - cn_nordig_p1 + 0.5f;

        // patch....
        // Noridg SQI,
        // 64QAM, CR34, GI14, SNR 22dB.
        if ( (tps_cnstl == _64QAM) && (tps_cr == _CR3Y4)
            && (cn_rel < 2.5f) && (cn_rel > 1.5f))
        {
            cn_rel += 1.5f;
        }

        if (cn_rel < -7.0f)
        {
            *quality = 0;
        }
        else if (cn_rel < 3.0)
            *quality = (MS_U16)(ber_sqi*((cn_rel - 3.0)/10.0 + 1.0));
        else
            *quality = (MS_U16)ber_sqi;
    }
    else
    {
        *quality = 0;
    }

    DBG_GET_SIGNAL(printf("SNR = %f, QAM = %d, code Rate = %d\n", cn_rec, tps_cnstl, tps_cr));
    DBG_GET_SIGNAL(printf("BER = %8.3e\n", fber));
    DBG_GET_SIGNAL(printf("Signal Quility = %d\n", *quality));
    return status;
}

/****************************************************************************
  Subject:    To get the Cell ID
  Function:   INTERN_DVBT_Get_CELL_ID
  Parmeter:   point to return parameter cell_id

  Return:     TRUE
              FALSE
  Remark:
*****************************************************************************/
MS_BOOL INTERN_DVBT_Get_CELL_ID(MS_U16 *cell_id)
{
    MS_U8 status = true;
    MS_U8 value1=0;
    MS_U8 value2=0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE+0x2A, &value1); //TPS Cell ID [15:8]
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE+0x2B, &value2); //TPS Cell ID [7:0]

    *cell_id = ((MS_U16)value1<<8)|value2;
    return status;
}
/*
FUNCTION_RESULT INTERN_DVBT_GetSignalQuality_Average(WORD *quality)
{
	#define SQI_LOOP_NUM 50
	U8 inn = 0;
	WORD sqi = 0;
	WORD ave_sqi = 0;
	WORD ave_num = 0;
	while(inn++<SQI_LOOP_NUM)
	{
		if(INTERN_DVBT_GetSignalQuality(&sqi) == E_RESULT_SUCCESS)
		{
			printf("[%d][t=%d],sqi=%d\n",inn,INTERN_DVBT_GET_TIME,sqi);
			ave_sqi+=sqi;
			ave_num++;
		}
		MsOS_DelayTask(50);
	}

	if(ave_num != 0 )
		*quality = ave_sqi/ave_num;

	return ave_num==0?E_RESULT_FAILURE:E_RESULT_SUCCESS;
}
*/
/****************************************************************************
  Subject:    To get the DVBT Carrier Freq Offset
  Function:   INTERN_DVBT_Get_FreqOffset
  Parmeter:   Frequency offset (in KHz), bandwidth
  Return:     E_RESULT_SUCCESS
              E_RESULT_FAILURE
  Remark:
*****************************************************************************/
MS_BOOL INTERN_DVBT_Get_FreqOffset(float *pFreqOff, MS_U8 u8BW)
{
    float         N, FreqB;
    float         FreqCfoTd, FreqCfoFd, FreqIcfo;
    MS_U32           RegCfoTd, RegCfoFd, RegIcfo;
    MS_U8            reg_frz=0, reg=0;
    MS_U8            status;

    FreqB = (float)u8BW * 8 / 7;

    status = MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x05, &reg_frz);

    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDP_REG_BASE + 0x05, reg_frz|0x80);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x24c8, &reg);
    RegCfoTd = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x24c7, &reg);
    RegCfoTd = (RegCfoTd << 8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x24c6, &reg);
    RegCfoTd = (RegCfoTd << 8)|reg;

    FreqCfoTd = (float)RegCfoTd;

    if (RegCfoTd & 0x800000)
        FreqCfoTd = FreqCfoTd - (float)0x1000000;

    FreqCfoTd = FreqCfoTd * FreqB * 0.00011642;

    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDP_REG_BASE + 0x05, reg_frz&(~0x80));

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0xfe, &reg_frz);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xfe, reg_frz|0x01);

    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xff, 0x01);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x33, &reg);
    RegCfoFd = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x32, &reg);
    RegCfoFd = (RegCfoFd << 8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x31, &reg);
    RegCfoFd = (RegCfoFd << 8)|reg;

    FreqCfoFd = (float)RegCfoFd;

    if (RegCfoFd & 0x800000)
        FreqCfoFd = FreqCfoFd - (float)0x1000000;

    FreqCfoFd = FreqCfoFd * FreqB * 0.00011642;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x19, &reg);
    RegIcfo = reg & 0x07;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x18, &reg);
    RegIcfo = (RegIcfo << 8)|reg;

    FreqIcfo = (float)RegIcfo;

    if (RegIcfo & 0x400)
        FreqIcfo = FreqIcfo - (float)0x800;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x26, &reg);
    reg = reg & 0x30;

    switch (reg)
    {
        case 0x00:  N = 2048;  break;
        case 0x20:  N = 4096;  break;
        case 0x10:
        default:    N = 8192;  break;
    }

    FreqIcfo = FreqIcfo * FreqB / N * 1000;         //unit: kHz
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xfe, reg_frz&(~0x01));
    //status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xfe+1, 0x01);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xff, 0x01);
    //*pFreqOff = FreqIcfo + (FreqCfoFd + FreqCfoTd)/1000;
    *pFreqOff = (-1.0f)*(FreqIcfo + (FreqCfoFd + FreqCfoTd)/1000);
    // DBG_GET_SIGNAL(printf("FCFO = %f\n", FreqCfoFd));
    // DBG_GET_SIGNAL(printf("TCFO = %f\n", FreqCfoTd));
    // DBG_GET_SIGNAL(printf("ICFO = %f\n", FreqIcfo));
    DBG_GET_SIGNAL(printf("CFOE = %f\n", *pFreqOff));

    if (status == TRUE)
        return TRUE;
    else
        return FALSE;
}


void INTERN_DVBT_Power_ON_OFF(MS_U8 bPowerOn)
{

    bPowerOn = bPowerOn;
}

MS_BOOL INTERN_DVBT_Power_Save(void)
{

    return TRUE;
}

/****************************************************************************
  Subject:    To get the DVBT constellation parameter
  Function:   INTERN_DVBT_Get_TPS_Parameter_Const
  Parmeter:   point to return parameter(0: QPSK, 1:16QAM, 2:64QAM)
  Return:     TRUE
              FALSE
  Remark:     The TPS parameters will be available after TPS lock
*****************************************************************************/
MS_BOOL INTERN_DVBT_Get_TPS_Parameter_Const( MS_U16 * TPS_parameter, E_SIGNAL_TYPE eSignalType)
{
    MS_U8 tps_param;

    //@@++ Arki 20100125
    if (eSignalType == TS_MODUL_MODE)
    {
        if ( MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x24, &tps_param) == FALSE ) return FALSE;
        *TPS_parameter = tps_param & (BIT(0)|BIT(1)) ;
    }

    if (eSignalType == TS_CODE_RATE)
    {
        if ( MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x25, &tps_param) == FALSE ) return FALSE;
        *TPS_parameter = (tps_param & (BIT(6)|BIT(5)|BIT(4)))>>4 ;
    }

    if (eSignalType == TS_GUARD_INTERVAL)
    {
        if ( MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x26, &tps_param) == FALSE ) return FALSE;
        *TPS_parameter = tps_param & (BIT(0)|BIT(1)) ;
    }

    if (eSignalType == TS_FFX_VALUE)
    {
        if ( MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x26, &tps_param) == FALSE ) return FALSE;
        *TPS_parameter = (tps_param & (BIT(5)|BIT(4)))>>4 ;
    }
    //@@-- Arki 20100125
    return TRUE;
}

MS_BOOL INTERN_DVBT_Version(MS_U16 *ver)
{

    MS_U8 status = true;
    MS_U8 tmp = 0;
    MS_U16 u16_INTERN_DVBT_Version;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0xC1, &tmp);
    u16_INTERN_DVBT_Version = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0xC2, &tmp);
    u16_INTERN_DVBT_Version = u16_INTERN_DVBT_Version<<8|tmp;
    *ver = u16_INTERN_DVBT_Version;

    return status;
}

MS_BOOL INTERN_DVBT_Version_minor(MS_U8 *ver2)
{

    MS_U8 status = true;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0xC3, ver2);

    return status;
}


MS_BOOL INTERN_DVBT_Show_Demod_Version(void)
{

    MS_BOOL status = true;
    MS_U16 u16_INTERN_DVBT_Version;
    MS_U8  u8_minor_ver;

    status &= INTERN_DVBT_Version(&u16_INTERN_DVBT_Version);
    status &= INTERN_DVBT_Version_minor(&u8_minor_ver);
    printf("[DVBT]Version = 0x%x,0x%x\n",u16_INTERN_DVBT_Version,u8_minor_ver);

    return status;
}

MS_BOOL INTERN_DVBT_NORDIG_SSI_Table_Write(DMD_CONSTEL constel, DMD_CODERATE code_rate, float write_value)
{
    MS_U8   u8_index = 0;
    MS_BOOL bRet     = false;

    while(dvbt_ssi_dbm_nordigp1[u8_index].constel != _UNKNOW_QAM)
    {
        if ( (dvbt_ssi_dbm_nordigp1[u8_index].constel == (DMD_CONSTEL)constel)
            && (dvbt_ssi_dbm_nordigp1[u8_index].code_rate == (DMD_CODERATE)code_rate))
        {
           dvbt_ssi_dbm_nordigp1[u8_index].p_ref = write_value;
           bRet = true;
           break;
        }
        else
        {
           u8_index++;
        }
    }
    return bRet;
}

MS_BOOL INTERN_DVBT_NORDIG_SSI_Table_Read(DMD_CONSTEL constel, DMD_CODERATE code_rate, float *read_value)
{
    MS_U8   u8_index = 0;
    MS_BOOL bRet     = false;

    while(dvbt_ssi_dbm_nordigp1[u8_index].constel != _UNKNOW_QAM)
    {
        if ( (dvbt_ssi_dbm_nordigp1[u8_index].constel == (DMD_CONSTEL)constel)
            && (dvbt_ssi_dbm_nordigp1[u8_index].code_rate == (DMD_CODERATE)code_rate))
        {
           *read_value = dvbt_ssi_dbm_nordigp1[u8_index].p_ref;
           bRet = true;
           break;
        }
        else
        {
           u8_index++;
        }
    }
    return bRet;
}


#if (INTERN_DVBT_INTERNAL_DEBUG == 1)
void INTERN_DVBT_get_demod_state(MS_U8* state)
{
   MDrv_SYS_DMD_VD_MBX_ReadReg(0x23E0, state);
   return;
}

MS_BOOL INTERN_DVBT_Show_ChannelLength(void)
{
    MS_U8 status = true;
    MS_U8 tmp = 0;
    MS_U16 len = 0;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x71,&tmp);
    len = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x70,&tmp);
    len = (len<<8)|tmp;
    printf("[dvbt]Hw_channel=%d\n",len);
    return status;
}

MS_BOOL INTERN_DVBT_Show_SW_ChannelLength(void)
{
    MS_U8 status = true;
    MS_U8 tmp = 0,peak_num = 0,insideGI = 0,stoptracking = 0,flag_short_echo = 0,fsa_mode = 0;
    MS_U16 sw_len = 0;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20C4,&tmp);
    sw_len = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20C3,&tmp);
    sw_len = (sw_len<<8)|tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20C2,&tmp);
    peak_num = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20C5,&tmp);
    insideGI = tmp&0x01;
    stoptracking = (tmp&0x02)>>1;
    flag_short_echo = (tmp&0x0C)>>2;
    fsa_mode = (tmp&0x30)>>4;

    printf("[dvbt]SW_len=%d, peak_num=%d, insideGI=%d, stoptrack=%d, short_echo=%d, fsa_mode=%d\n",
        sw_len,peak_num,insideGI,stoptracking,flag_short_echo,fsa_mode);

    return status;
}

MS_BOOL INTERN_DVBT_Show_ACI_CI(void)
{

    #define BIT4 0x10
    MS_U8 status = true;
    MS_U8 digACI =0 ,flag_CI = 0,td_coef = 0,tmp = 0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x2357,&tmp);
    digACI = (tmp&BIT4)>>4;

    // get flag_CI
    // 0: No interference
    // 1: CCI
    // 2: in-band ACI
    // 3: N+1 ACI
    // flag_ci = (tmp&0xc0)>>6;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x2356,&tmp);
    flag_CI = (tmp&0xC0)>>6;
    td_coef = (tmp&0x0C)>>2;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20E8,&tmp);

    printf("[dvbt]DigACI=%d, Flag_CI=%d, td_coef=%d\n",digACI,flag_CI,td_coef);

    return status;
}

MS_BOOL INTERN_DVBT_Show_FD_CH_LEN_S_SEL(void)
{
    MS_U8 status = true;
    MS_U8 tmp = 0,fd = 0,ch_len = 0,snr_sel = 0,pertone_num = 0;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x2355, &tmp);
    fd = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x2354, &tmp);
    ch_len = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x46, &tmp);
    snr_sel = (tmp>>4)&0x03;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x22AC, &tmp);
    pertone_num = tmp;

    printf("[dvbt]fd=0x%x, ch_len=0x%x, snr_sel=0x%x, pertone_num=0x%x\n",fd,ch_len,snr_sel,pertone_num);

    return status;
}

MS_BOOL INTERN_DVBT_Get_CFO(void)
{

    float         N = 0, FreqB = 0;
    float         FreqCfoTd = 0, FreqCfoFd = 0, FreqIcfo = 0, total_cfo = 0;
    MS_U32        RegCfoTd = 0, RegCfoFd = 0, RegIcfo = 0;
    MS_U8         reg_frz = 0, reg = 0;
    MS_U8         status = 0;
    MS_U8         u8BW = 8;

    FreqB = (float)u8BW * 8 / 7;

    status = MDrv_SYS_DMD_VD_MBX_ReadReg(TDP_REG_BASE + 0x05, &reg_frz);

    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDP_REG_BASE + 0x05, reg_frz|0x80);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x24c8, &reg);
    RegCfoTd = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x24c7, &reg);
    RegCfoTd = (RegCfoTd << 8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x24c6, &reg);
    RegCfoTd = (RegCfoTd << 8)|reg;

    FreqCfoTd = (float)RegCfoTd;

    if (RegCfoTd & 0x800000)
        FreqCfoTd = FreqCfoTd - (float)0x1000000;

    FreqCfoTd = FreqCfoTd * FreqB * 0.00011642;

    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(TDP_REG_BASE + 0x05, reg_frz&(~0x80));

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0xfe, &reg_frz);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xfe, reg_frz|0x01);

    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xff, 0x01);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x33, &reg);
    RegCfoFd = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x32, &reg);
    RegCfoFd = (RegCfoFd << 8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x31, &reg);
    RegCfoFd = (RegCfoFd << 8)|reg;

    FreqCfoFd = (float)RegCfoFd;

    if (RegCfoFd & 0x800000)
        FreqCfoFd = FreqCfoFd - (float)0x1000000;

    FreqCfoFd = FreqCfoFd * FreqB * 0.00011642;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x19, &reg);
    RegIcfo = reg & 0x07;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x18, &reg);
    RegIcfo = (RegIcfo << 8)|reg;

    FreqIcfo = (float)RegIcfo;

    if (RegIcfo & 0x400)
        FreqIcfo = FreqIcfo - (float)0x800;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x26, &reg);
    reg = reg & 0x30;

    switch (reg)
    {
        case 0x00:  N = 2048;  break;
        case 0x20:  N = 4096;  break;
        case 0x10:
        default:    N = 8192;  break;
    }

    FreqIcfo = FreqIcfo * FreqB / N * 1000;         //unit: kHz
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xfe, reg_frz&(~0x01));
    //status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xfe+1, 0x01);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FDP_REG_BASE + 0xff, 0x01);
    total_cfo = FreqIcfo + (FreqCfoFd + FreqCfoTd)/1000;

    printf("[CFO]t_cfo=%f Hz, f_cfo=%f Hz, icfo=%f KHz, cfo=%f KHz\n", FreqCfoTd,FreqCfoFd,FreqIcfo,total_cfo);

    return status;

}
MS_BOOL INTERN_DVBT_Get_SFO(void)
{
    MS_U32 Reg_TDP_SFO = 0, Reg_FDP_SFO = 0, Reg_FSA_SFO = 0, Reg_FSA_IN = 0;
    MS_BOOL status = true;
    MS_U8  reg = 0;
    float  FreqB = 9.143, FreqS = 45.473;  //20.48
    float  Float_TDP_SFO = 0, Float_FDP_SFO = 0, Float_FSA_SFO = 0, Float_FSA_IN = 0;
    float  sfo_value = 0;

    // get Reg_TDP_SFO,
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDE_REG_BASE + 0xCC, &reg);
    Reg_TDP_SFO = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDE_REG_BASE + 0xCB, &reg);
    Reg_TDP_SFO = (Reg_TDP_SFO<<8)|reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDE_REG_BASE + 0xCA, &reg);
    Reg_TDP_SFO = (Reg_TDP_SFO<<8)|reg;

    Float_TDP_SFO = (float)((MS_S32)(Reg_TDP_SFO<<8))/256*FreqB/FreqS*0.0018626;

    // get Reg_FDP_SFO,
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x30, &reg);
    Reg_FDP_SFO = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x2F, &reg);
    Reg_FDP_SFO = (Reg_FDP_SFO<<8)|reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x2E, &reg);
    Reg_FDP_SFO = (Reg_FDP_SFO<<8)|reg;

    Float_FDP_SFO = (float)((MS_S32)(Reg_FDP_SFO<<8))/256*FreqB/FreqS*0.0018626;

    // get Reg_FSA_SFO,
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x8C, &reg);
    Reg_FSA_SFO = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x8B, &reg);
    Reg_FSA_SFO = (Reg_FSA_SFO<<8)|reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x8A, &reg);
    Reg_FSA_SFO = (Reg_FSA_SFO<<8)|reg;

    // get Reg_FSA_IN,
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x8F, &reg);
    Reg_FSA_IN = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x8E, &reg);
    Reg_FSA_IN = (Reg_FSA_IN<<8)|reg;
    Float_FSA_IN = (float)((MS_S32)(Reg_FSA_IN<<19))/512/2048;

    //Float_FSA_SFO = (float)((MS_S16)(Reg_FSA_SFO<<8))/256*FreqB/FreqS*0.0037253;
    Float_FSA_SFO = (float)((MS_S32)(Reg_FSA_SFO<<8))/256*FreqB/FreqS*0.0018626;

    sfo_value = Float_TDP_SFO + Float_FDP_SFO + Float_FSA_SFO;
    // printf("\nReg_FSA_SFO = 0x%x\n",Reg_FSA_SFO);
    printf("[SFO]tdp_sfo=%f, fdp_sfo=%f, fsa_sfo=%f, Tot_sfo=%f, fsa_sfo_in=%f\n",Float_TDP_SFO,Float_FDP_SFO,Float_FSA_SFO,sfo_value,Float_FSA_IN);


    return status;
}

void INTERN_DVBT_Get_SYA_status(void)
{
    MS_U8  status = true;
    MS_U8  sya_k = 0,reg = 0;
    MS_U16 sya_th = 0,len_a = 0,len_b = 0,len_m = 0,sya_offset = 0,tracking_reg = 0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x6F, &reg);
    sya_k = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x69, &reg);
    sya_th = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x68, &reg);
    sya_th = (sya_th<<8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x95, &reg);
    sya_offset = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x94, &reg);
    sya_offset = (sya_offset<<8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x71, &reg);
    len_m = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x70, &reg);
    len_m = (len_m<<8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x87, &reg);
    len_b = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x86, &reg);
    len_b = (len_b<<8)|reg;


    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x89, &reg);
    len_a = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x88, &reg);
    len_a = (len_a<<8)|reg;


    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x66, &reg);
    tracking_reg = reg;


    printf("[SYA][1]sya_k = 0x%x, sya_th = 0x%x, sya_offset=0x%x\n",sya_k,sya_th,sya_offset);
    printf("[SYA][2]track_reg=0x%x, len_m = %d, len_e = %d [%d,%d]\n",tracking_reg,len_m,len_b-len_a,len_a,len_b);

    return;
}

void INTERN_DVBT_Get_cci_status(void)
{
    MS_U8  status = true;
    MS_U8 cci_fsweep = 0,cci_kp = 0,reg = 0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDE_REG_BASE + 0x08, &reg);
    cci_fsweep = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(TDE_REG_BASE + 0x0A, &reg);
    cci_kp = reg;

    printf("[CCI]fsweep=0x%x, k=0x%x\n",cci_fsweep,cci_kp);

    return;
}

MS_BOOL INTERN_DVBT_Show_PRESFO_Info(void)
{
    MS_U8 tmp = 0;
    MS_BOOL status = TRUE;
    printf("\n[SFO]");
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20D0,&tmp);
    printf("[%x]",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20D1,&tmp);
    printf("[%x]",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20D2,&tmp);
    printf("[%x]",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20D3,&tmp);
    printf("[%x]",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20D4,&tmp);
    printf("[%x]",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20D5,&tmp);
    printf("[%x]",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20D6,&tmp);
    printf("[%x]",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20D7,&tmp);
    printf("[%x][End]",tmp);

    return status;
}

MS_BOOL INTERN_DVBT_Get_Lock_Time_Info(MS_U16 *locktime)
{
    MS_BOOL status = true;

    *locktime = 0xffff;
    printf("[dvbt]INTERN_DVBT_Get_Lock_Time_Info not implement\n");

    status = false;
    return status;
}


MS_BOOL INTERN_DVBT_Show_Lock_Time_Info(void)
{
    MS_U16 locktime = 0;
    MS_BOOL status = TRUE;
    status &= INTERN_DVBT_Get_Lock_Time_Info(&locktime);
    printf("[DVBT]lock_time = %d ms\n",locktime);
    return status;
}

MS_BOOL INTERN_DVBT_Show_BER_Info(void)
{
    MS_U8 tmp = 0;
    MS_BOOL status = TRUE;
    printf("\n[BER]");
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20C6,&tmp);
    printf("[%x,",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20C7,&tmp);
    printf("%x]",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20C8,&tmp);
    printf("[%x,",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20C9,&tmp);
    printf("%x]",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20CA,&tmp);
    printf("[%x,",tmp);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x20CB,&tmp);
    printf("%x][End]",tmp);

    return status;

}


MS_BOOL INTERN_DVBT_Show_AGC_Info(void)
{
    MS_U8 tmp = 0;
    MS_U8 agc_k = 0,agc_ref = 0,d1_k = 0,d1_ref = 0,d2_k = 0,d2_ref = 0;
    MS_U16 if_agc_gain = 0,d1_gain = 0,d2_gain = 0;
    MS_U16 if_agc_err = 0;
    MS_BOOL status = TRUE;
    MS_U8  agc_lock = 0, d1_lock = 0, d2_lock = 0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x05,&agc_k);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x07,&agc_ref);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x82,&d1_k);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x84,&d1_ref);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTNEXT_REG_BASE + 0x00,&d2_k);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTNEXT_REG_BASE + 0x01,&d2_ref);


    // select IF gain to read
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x16, &tmp);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FTN_REG_BASE + 0x16, (tmp&0xF0)|0x03);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x19, &tmp);
    if_agc_gain = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x18, &tmp);
    if_agc_gain = (if_agc_gain<<8)|tmp;


    // select d1 gain to read.
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x8c, &tmp);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FTN_REG_BASE + 0x8c, (tmp&0xF0)|0x02);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x89, &tmp);
    d1_gain = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x88, &tmp);
    d1_gain = (d1_gain<<8)|tmp;

    // select d2 gain to read.
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTNEXT_REG_BASE + 0x06, &tmp);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FTNEXT_REG_BASE + 0x06, (tmp&0xF0)|0x02);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTNEXT_REG_BASE + 0x09, &tmp);
    d2_gain = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTNEXT_REG_BASE + 0x08, &tmp);
    d2_gain = (d2_gain<<8)|tmp;

    // select IF gain err to read
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x16, &tmp);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FTN_REG_BASE + 0x16, (tmp&0xF0)|0x00);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x19, &tmp);
    if_agc_err = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x18, &tmp);
    if_agc_err = (if_agc_err<<8)|tmp;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x1d, &agc_lock);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTN_REG_BASE + 0x99, &d1_lock);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FTNEXT_REG_BASE + 0x05, &d2_lock);



    printf("[dvbt]agc_k=0x%x, agc_ref=0x%x, d1_k=0x%x, d1_ref=0x%x, d2_k=0x%x, d2_ref=0x%x\n",
        agc_k,agc_ref,d1_k,d1_ref,d2_k,d2_ref);

    printf("[dvbt]agc_g=0x%x, d1_g=0x%x, d2_g=0x%x, agc_err=0x%x\n",if_agc_gain,d1_gain,d2_gain,if_agc_err);
    printf("[dvbt]agc_lock=0x%x, d1_lock=0x%x, d2_lock=0x%x\n",agc_lock,d1_lock,d2_lock);

    return status;

}

MS_BOOL INTERN_DVBT_Show_WIN_Info(void)
{
    MS_U8 tmp = 0;
    MS_U8 trigger = 0;
    MS_U16 win_len = 0;

    MS_BOOL status = TRUE;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x0B,&tmp);
    win_len = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x0A,&tmp);
    win_len = (win_len<<8)|tmp;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE + 0x08,&trigger);

    printf("[dvbt]win_len = %d, trigger=0x%x\n",win_len,trigger);

    return status;
}

void INTERN_DVBT_Show_td_coeff(void)
{
    MS_U8  status = true;
    MS_U8 w1 = 0,w2 = 0,reg = 0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x2561, &reg);
    w1 = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x2562, &reg);
    w2 = reg;

    printf("[td]w1=0x%x, w2=0x%x\n",w1,w2);

    return;
}

/********************************************************
 * Constellation (b2 ~ b0)  : 0~2 => QPSK, 16QAM, 64QAM
 * Hierarchy (b5 ~ b3))     : 0~3 => None, Aplha1, Aplha2, Aplha4
 * LP Code Rate (b8 ~ b6)     : 0~4 => 1/2, 2/3, 3/4, 5/6, 7/8
 * HP Code Rate (b11 ~ b9)  : 0~4 => 1/2, 2/3, 3/4, 5/6, 7/8
 * GI (b13 ~ b12)             : 0~3 => 1/32, 1/16, 1/8, 1/4
 * FFT ( b14)            : 0~1 => 2K, 8K
 ********************************/
MS_BOOL INTERN_DVBT_Show_Modulation_info(void)
{
    MS_U16 tps_info;

    // printf("[DVBT]TPS info, freq=%ld  ",CurRFParam.RfFreqInKHz);

    if(INTERN_DVBT_Get_TPS_Info( &tps_info))
    {
        MS_U8 fft       = (MS_U8)((tps_info&0x4000)>>14);
        MS_U8 constel = tps_info&0x0007;
        MS_U8 gi      = (MS_U8)((tps_info&0x3000)>>12);
        MS_U8 hp_cr   = (MS_U8)((tps_info&0x0E00)>>9);
        MS_U8 lp_cr   = (MS_U8)((tps_info&0x01C0)>>6);
        MS_U8 hiearchy = (MS_U8)((tps_info&0x0038)>>3);

        printf("tps=0x%x  ",tps_info);

        switch(fft)
        {
            case 0:
                printf("mode = 2K,");
                break;
            case 1:
                printf("mode = 8K,");
                break;
            default:
                printf("mode = unknow,");
                break;
        }
        switch(constel)
        {
            case 0:
                printf(" QPSK, ");
                break;
            case 1:
                printf("16QAM, ");
                break;
            case 2:
                printf("64QAM, ");
                break;
            default:
                printf("unknow QAM, ");
                break;
        }
        switch(gi)
        {
            case 0:
                printf("GI=1/32, ");
                break;
            case 1:
                printf("GI=1/16, ");
                break;
            case 2:
                printf("GI= 1/8, ");
                break;
            case 3:
                printf("GI= 1/4, ");
                break;
            default:
                printf("unknow GI, ");
                break;
        }

        switch(hp_cr)
        {
            case 0:
                printf("HP_CR=1/2, ");
                break;
            case 1:
                printf("HP_CR=2/3, ");
                break;
            case 2:
                printf("HP_CR=3/4, ");
                break;
            case 3:
                printf("HP_CR=5/6, ");
                break;
            case 4:
                printf("HP_CR=7/8, ");
                break;
            default:
                printf("unknow hp_cr, ");
                break;
        }

        switch(lp_cr)
        {
            case 0:
                printf("LP_CR=1/2, ");
                break;
            case 1:
                printf("LP_CR=2/3, ");
                break;
            case 2:
                printf("LP_CR=3/4, ");
                break;
            case 3:
                printf("LP_CR=5/6, ");
                break;
            case 4:
                printf("LP_CR=7/8, ");
                break;
            default:
                printf("unknow lp_cr, ");
                break;
        }

        printf(" Hiearchy=0x%x\n",hiearchy);

        // printf("\n");
        return TRUE;
    }
    else
    {
        printf("INVALID\n");
        return FALSE;
    }
}




void INTERN_DVBT_Show_BER_PacketErr(void)
{
  float  f_ber = 0;
  MS_U16 packetErr = 0;
  INTERN_DVBT_GetPostViterbiBer(&f_ber);
  INTERN_DVBT_GetPacketErr(&packetErr);

  printf("[dvbt]ber=%f, Err=%d\n",f_ber, packetErr);
  return;
}

MS_BOOL INTERN_DVBT_Show_Lock_Info(void)
{

  printf("[dvbt]INTERN_DVBT_Show_Lock_Info not implement!!!\n");
  return false;
}


MS_BOOL INTERN_DVBT_Show_Demod_Info(void)
{
  MS_U8         demod_state = 0;
  MS_BOOL       status = true;
  static MS_U8  counter = 0;

  INTERN_DVBT_get_demod_state(&demod_state);

  printf("==========[dvbt]state=%d\n",demod_state);
  if (demod_state < 5)
  {
    INTERN_DVBT_Show_Demod_Version();
    INTERN_DVBT_Show_AGC_Info();
    INTERN_DVBT_Show_ACI_CI();
  }
  else if(demod_state < 8)
  {
    INTERN_DVBT_Show_Demod_Version();
    INTERN_DVBT_Show_AGC_Info();
    INTERN_DVBT_Show_ACI_CI();
    INTERN_DVBT_Show_ChannelLength();
    INTERN_DVBT_Get_CFO();
    INTERN_DVBT_Get_SFO();
    INTERN_DVBT_Show_td_coeff();
  }
  else if(demod_state < 11)
  {
    INTERN_DVBT_Show_Demod_Version();
    INTERN_DVBT_Show_AGC_Info();
    INTERN_DVBT_Show_ACI_CI();
    INTERN_DVBT_Show_ChannelLength();
    INTERN_DVBT_Get_CFO();
    INTERN_DVBT_Get_SFO();
    INTERN_DVBT_Show_FD_CH_LEN_S_SEL();
    INTERN_DVBT_Get_SYA_status();
    INTERN_DVBT_Show_td_coeff();
  }
  else if((demod_state == 11) && ((counter%4) == 0))
  {
    INTERN_DVBT_Show_Demod_Version();
    INTERN_DVBT_Show_AGC_Info();
    INTERN_DVBT_Show_ACI_CI();
    INTERN_DVBT_Show_ChannelLength();
    INTERN_DVBT_Get_CFO();
    INTERN_DVBT_Get_SFO();
    INTERN_DVBT_Show_FD_CH_LEN_S_SEL();
    INTERN_DVBT_Get_SYA_status();
    INTERN_DVBT_Show_td_coeff();
    INTERN_DVBT_Show_Modulation_info();
    INTERN_DVBT_Show_BER_PacketErr();
  }
  else
    status = false;

  printf("===========================\n");
  counter++;

  return status;
}
#endif

