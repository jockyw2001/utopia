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
// (：MStar Confidential Information；) by the recipient.
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
//0312

#define _INTERN_DVBS_C_
#include <math.h>
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
#include "drvDMD_INTERN_DVBS.h"
#include "halDMD_INTERN_DVBS.h"
#include "halDMD_INTERN_common.h"

#include "drvMMIO.h"
//#include "TDAG4D01A_SSI_DVBT.c"
#include "drvDMD_VD_MBX.h"
//-----------------------------------------------------------------------
#define BIN_ID_INTERN_DVBS_DEMOD BIN_ID_INTERN_DVBS

//For DVBS
//#define DVBT2FEC_REG_BASE           0x3300 
#define DVBS2OPPRO_REG_BASE         0x3E00    
#define TOP_REG_BASE                0x2000    //DMDTOP
#define REG_BACKEND                 0x1F00//_REG_BACKEND
#define DVBSFEC_REG_BASE            0x3F00    
#define DVBS2FEC_REG_BASE           0x3300  
#define DVBS2_REG_BASE              0x3A00
#define DVBS2_INNER_REG_BASE        0x3B00
#define DVBS2_INNER_EXT_REG_BASE    0x3C00
#define DVBS2_INNER_EXT2_REG_BASE    0x3D00
//#define DVBSTFEC_REG_BASE           0x2300    //DVBTFEC
#define FRONTEND_REG_BASE           0x2800
#define FRONTENDEXT_REG_BASE        0x2900
#define FRONTENDEXT2_REG_BASE       0x2A00
#define DMDANA_REG_BASE                      0x2E00    //DMDDTOP//reg_dmdana.xls
#define DVBTM_REG_BASE                       0x3400

#define SAMPLING_RATE_FS                    (144000)//(108000)//(96000)
#define INTERN_DVBS_DEMOD_WAIT_TIMEOUT      (6000)
#define INTERN_DVBS_TUNER_WAIT_TIMEOUT      (50)

//#define DVBS2_Function                      (1)
//#define MSB131X_ADCPLL_IQ_SWAP            0
//#define INTERN_DVBS_TS_DATA_SWAP            0

//#define MS_DEBUG //enable debug dump

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
//----------------------------------------------------------
#define DBG_DUMP_LOAD_DSP_TIME 0


#define SIGNAL_LEVEL_OFFSET     0.00f
#define TAKEOVERPOINT           -60.0f
#define TAKEOVERRANGE           0.5f
#define LOG10_OFFSET            -0.21f
#define INTERN_DVBS_USE_SAR_3_ENABLE 0
//extern MS_U32 msAPI_Timer_GetTime0(void);
//#define INTERN_DVBS_GET_TIME msAPI_Timer_GetTime0()


//Debug Info
//Lock/Done Flag
#define AGC_LOCK                                    0x28170100
#define DAGC0_LOCK                                  0x283B0001
#define DAGC1_LOCK                                  0x285B0001
#define DAGC2_LOCK                                  0x28620001 //ACIDAGC 1 2
#define DAGC3_LOCK                                  0x286E0001
#define DCR_LOCK                                    0x28220100
#define COARSE_SYMBOL_RATE_DONE                     0x2A200001 //CSRD 1 2
#define FINE_SYMBOL_RATE_DONE                       0x2A200008 //FSRD 1 2
#define POWER4CFO_DONE                              0x29280100 //POWER4CFO 1 2
//#define CLOSE_COARSE_CFO_LOCK                       0x244E0001
#define TR_LOCK                                     0x3B0E0100 //TR 1 2
#define PR_LOCK                                     0x3B401000
#define FRAME_SYNC_ACQUIRE                          0x3B300001
#define EQ_LOCK                                     0x3B5A1000
#define P_SYNC_LOCK                                 0x22160002
#define IN_SYNC_LOCK                                0x3F0D8000

//AGC / DAGC
#define DEBUG_SEL_IF_AGC_GAIN                       0x28260003
#define DEBUG_SEL_AGC_ERR                           0x28260004
#define DEBUG_OUT_AGC                               0x2828

#define DEBUG_SEL_DAGC0_GAIN                        0x28E80003
#define DEBUG_SEL_DAGC0_ERR                         0x28E80001
#define DEBUG_SEL_DAGC0_PEAK_MEAN                   0x28E80005
#define DEBUG_OUT_DAGC0                             0x2878

#define DEBUG_SEL_DAGC1_GAIN                        0x28E80003//???
#define DEBUG_SEL_DAGC1_ERR                         0x28E80001
#define DEBUG_SEL_DAGC1_PEAK_MEAN                   0x28E80005
#define DEBUG_OUT_DAGC1                             0x28B8

#define DEBUG_SEL_DAGC2_GAIN                        0x28E80003
#define DEBUG_SEL_DAGC2_ERR                         0x28E80001
#define DEBUG_SEL_DAGC2_PEAK_MEAN                   0x28E80005
#define DEBUG_OUT_DAGC2                             0x28C4

#define DEBUG_SEL_DAGC3_GAIN                        0x29DA0003
#define DEBUG_SEL_DAGC3_ERR                         0x29DA0001
#define DEBUG_SEL_DAGC3_PEAK_MEAN                   0x29DA0005
#define DEBUG_OUT_DAGC3                             0x29DC

#define INNER_DEBUG_SEL_TR                          0x24080D00  //TR
#define DEBUG_SEL_TR_SFO_CONVERGE                   0x24080B00
#define DEBUG_SEL_TR_INPUT                          0x24080F00

#define FRONTEND_FREEZE_DUMP                        0x27028000
#define INNER_FREEZE_DUMP                           0x24080010

#define DCR_OFFSET                                      0x2740
#define INNER_DEBUG_SEL                                 0x2408
#define INNEREXT_FINEFE_DBG_OUT0                        0x2550
#define INNEREXT_FINEFE_DBG_OUT2                        0x2552
#define INNEREXT_FINEFE_KI_FF0                          0x2556
#define INNEREXT_FINEFE_KI_FF2                          0x2558
#define INNEREXT_FINEFE_KI_FF4                          0x255A
#define INNER_PR_DEBUG_OUT0                             0x2486
#define INNER_PR_DEBUG_OUT2                             0x2488

#define IIS_COUNT0                                      0x2746
#define IIS_COUNT2                                      0x2748
#define IQB_PHASE                                       0x2766
#define IQB_GAIN                                        0x2768
#define TR_INDICATOR_FF0                                0x2454
#define TR_INDICATOR_FF2                                0x2456
#define INNER_TR_LOPF_VALUE_DEBUG0                      0x2444
#define INNER_TR_LOPF_VALUE_DEBUG2                      0x2446
#define INNER_TR_LOPF_VALUE_DEBUG4                      0x2448
//------------------------------------------------------------
//Init Mailbox parameter.
#define     INTERN_DVBS_TS_SERIAL_INVERSION 0
//For Parameter Init Setting
#define     A_S2_ZIF_EN                     0x01                //[0]
#define     A_S2_RF_AGC_EN                  0x00                //[0]
#define     A_S2_DCR_EN                     0x00                //[0]       0=Auto :1=Force
#define     A_S2_IQB_EN                     0x01                //[2]
#define     A_S2_IIS_EN                     0x00                //[0]
#define     A_S2_CCI_EN                     0x00                //[0]       0:1=Enable
#define     A_S2_FORCE_ACI_SELECT           0xFF                //[3:0]     0xFF=OFF(internal default)
#define     A_S2_IQ_SWAP                    0x01                //[0]
#define     A_S2_AGC_REF_EXT_0              0x00                //[7:0]  //0x00 0x90
#define     A_S2_AGC_REF_EXT_1              0x02                //[11:8] //0x02 0x07
#define     A_S2_AGC_K                      0x07                //[15:12]
#define     A_S2_ADCI_GAIN                  0x0F                //[4:0]
#define     A_S2_ADCQ_GAIN                  0x0F                //[12:8]
#define     A_S2_SRD_SIG_SRCH_RNG           0x6A                //[7:0]
#define     A_S2_SRD_DC_EXC_RNG             0x16                //[7:0]
//FRONTENDEXT_SRD_FRC_CFO
#define     A_S2_FORCE_CFO_0                0x00                //[7:0]
#define     A_S2_FORCE_CFO_1                0x00                //[11:8]
#define     A_S2_DECIMATION_NUM             0x00                //[3:0]     00=(Internal Default)
#define     A_S2_PSD_SMTH_TAP               0x29                //[6:0]     Bit7 no define.
//CCI Parameter
//Set_Tuner_BW=(((U16)REG_BASE[DIG_SWUSE1FH]<<8)|REG_BASE[DIG_SWUSE1FL]);
#define     A_S2_CCI_FREQN_0_L              0x00                //[7:0]
#define     A_S2_CCI_FREQN_0_H              0x00                //[11:8]
#define     A_S2_CCI_FREQN_1_L              0x00                //[7:0]
#define     A_S2_CCI_FREQN_1_H              0x00                //[11:8]
#define     A_S2_CCI_FREQN_2_L              0x00                //[7:0]
#define     A_S2_CCI_FREQN_2_H              0x00                //[11:8]
//Inner TR Parameter
#define     A_S2_TR_LOPF_KP                 0x00                //[4:0]     00=(Internal Default)
#define     A_S2_TR_LOPF_KI                 0x00                //[4:0]     00=(Internal Default)
//Inner FineFE Parameter
#define     A_S2_FINEFE_KI_SWITCH_0         0x00                //[15:12]   00=(Internal Default)
#define     A_S2_FINEFE_KI_SWITCH_1         0x00                //[3:0]     00=(Internal Default)
#define     A_S2_FINEFE_KI_SWITCH_2         0x00                //[7:4]     00=(Internal Default)
#define     A_S2_FINEFE_KI_SWITCH_3         0x00                //[11:8]    00=(Internal Default)
#define     A_S2_FINEFE_KI_SWITCH_4         0x00                //[15:12]   00=(Internal Default)
//Inner PR KP Parameter
#define     A_S2_PR_KP_SWITCH_0             0x00                //[11:8]    00=(Internal Default)
#define     A_S2_PR_KP_SWITCH_1             0x00                //[15:12]   00=(Internal Default)
#define     A_S2_PR_KP_SWITCH_2             0x00                //[3:0]     00=(Internal Default)
#define     A_S2_PR_KP_SWITCH_3             0x00                //[7:4]     00=(Internal Default)
#define     A_S2_PR_KP_SWITCH_4             0x00                //[11:8]    00=(Internal Default)
//Inner FS Parameter
#define     A_S2_FS_GAMMA                   0x10                //[7:0]
#define     A_S2_FS_ALPHA0                  0x10                //[7:0]
#define     A_S2_FS_ALPHA1                  0x10                //[7:0]
#define     A_S2_FS_ALPHA2                  0x10                //[7:0]
#define     A_S2_FS_ALPHA3                  0x10                //[7:0]

#define     A_S2_FS_H_MODE_SEL              0x01                //[0]
#define     A_S2_FS_OBSWIN                  0x08                //[12:8]
#define     A_S2_FS_PEAK_DET_TH_L           0x00                //[7:0]
#define     A_S2_FS_PEAK_DET_TH_H           0x01                //[15:8]
#define     A_S2_FS_CONFIRM_NUM             0x01                //[3:0]
//Inner EQ Parameter
#define     A_S2_EQ_MU_FFE_DA               0x00                //[3:0]     00=(Internal Default)
#define     A_S2_EQ_MU_FFE_DD               0x00                //[7:4]     00=(Internal Default)
#define     A_S2_EQ_ALPHA_SNR_DA            0x00                //[7:4]     00=(Internal Default)
#define     A_S2_EQ_ALPHA_SNR_DD            0x00                //[11:8]    00=(Internal Default)
//Outer FEC Parameter
#define     A_S2_FEC_ALFA                   0x00                //[12:8]
#define     A_S2_FEC_BETA                   0x01                //[7:4]
#define     A_S2_FEC_SCALING_LLR            0x00                //[7:0]     00=(Internal Default)
//TS Parameter
#if INTERN_DVBS_TS_SERIAL_INVERSION
#define     A_S2_TS_SERIAL                  0x01                //[0]
#else
#define     A_S2_TS_SERIAL                  0x00                //[0]
#endif
#define     A_S2_TS_CLK_RATE                0x00
#define     A_S2_TS_OUT_INV                 0x00                //[5]
#define     A_S2_TS_DATA_SWAP               0x00                //[5]
//Rev Parameter

#define     A_S2_FW_VERSION_L               0x00                //From FW
#define     A_S2_FW_VERSION_H               0x00                //From FW
#define     A_S2_CHIP_VERSION               0x01
#define     A_S2_FS_L                       0x00
#define     A_S2_FS_H                       0x00
#define     A_S2_MANUAL_TUNE_SYMBOLRATE_L   0x20
#define     A_S2_MANUAL_TUNE_SYMBOLRATE_H   0x4E

MS_U8 INTERN_DVBS_DSPREG[] =
{
    A_S2_ZIF_EN,            A_S2_RF_AGC_EN,         A_S2_DCR_EN,             A_S2_IQB_EN,               A_S2_IIS_EN,              A_S2_CCI_EN,              A_S2_FORCE_ACI_SELECT,          A_S2_IQ_SWAP,                   // 00H ~ 07H
    A_S2_AGC_REF_EXT_0,     A_S2_AGC_REF_EXT_1,     A_S2_AGC_K,              A_S2_ADCI_GAIN,            A_S2_ADCQ_GAIN,           A_S2_SRD_SIG_SRCH_RNG,    A_S2_SRD_DC_EXC_RNG,            A_S2_FORCE_CFO_0,               // 08H ~ 0FH
    A_S2_FORCE_CFO_1,       A_S2_DECIMATION_NUM,    A_S2_PSD_SMTH_TAP,       A_S2_CCI_FREQN_0_L,        A_S2_CCI_FREQN_0_H,       A_S2_CCI_FREQN_1_L,       A_S2_CCI_FREQN_1_H,             A_S2_CCI_FREQN_2_L,             // 10H ~ 17H
    A_S2_CCI_FREQN_2_H,     A_S2_TR_LOPF_KP,        A_S2_TR_LOPF_KI,         A_S2_FINEFE_KI_SWITCH_0,   A_S2_FINEFE_KI_SWITCH_1,  A_S2_FINEFE_KI_SWITCH_2,  A_S2_FINEFE_KI_SWITCH_3,        A_S2_FINEFE_KI_SWITCH_4,        // 18H ~ 1FH
    A_S2_PR_KP_SWITCH_0,    A_S2_PR_KP_SWITCH_1,    A_S2_PR_KP_SWITCH_2,     A_S2_PR_KP_SWITCH_3,       A_S2_PR_KP_SWITCH_4,      A_S2_FS_GAMMA,            A_S2_FS_ALPHA0,                 A_S2_FS_ALPHA1,                 // 20H ~ 27H
    A_S2_FS_ALPHA2,         A_S2_FS_ALPHA3,         A_S2_FS_H_MODE_SEL,      A_S2_FS_OBSWIN,            A_S2_FS_PEAK_DET_TH_L,    A_S2_FS_PEAK_DET_TH_H,    A_S2_FS_CONFIRM_NUM,            A_S2_EQ_MU_FFE_DA,              // 28h ~ 2FH
    A_S2_EQ_MU_FFE_DD,      A_S2_EQ_ALPHA_SNR_DA,   A_S2_EQ_ALPHA_SNR_DD,    A_S2_FEC_ALFA,             A_S2_FEC_BETA,            A_S2_FEC_SCALING_LLR,     A_S2_TS_SERIAL,                 A_S2_TS_CLK_RATE,               // 30H ~ 37H
    A_S2_TS_OUT_INV,        A_S2_TS_DATA_SWAP,      A_S2_FW_VERSION_L,       A_S2_FW_VERSION_H,         A_S2_CHIP_VERSION,        A_S2_FS_L,                A_S2_FS_H,                      A_S2_MANUAL_TUNE_SYMBOLRATE_L,  // 38H ~ 3CH
    A_S2_MANUAL_TUNE_SYMBOLRATE_H,
};

/****************************************************************
*Local Variables                                                                                              *
****************************************************************/


static MS_U16             _u16SignalLevel[185][2]=
{//AV2028 SR=22M, 2/3 CN=5.9
    {255,    920},{255,    915},{255,    910},{255,    905},{255,    900},{255,    895},{255,    890},{255,    885},{255,    880},{255,    875},
    {255,    870},{255,    865},{255,    860},{255,    855},{255,    850},{2121,    845},{3988,    840},{11629,    835},{19270,    830},{19744,    825},
    {20218,    820},{20692,    815},{21166,    810},{21640,    805},{22114,    800},{22350,    795},{22587,    790},{22823,    785},{23059,    780},{23296,    775},
    {23532,    770},{23790,    765},{24049,    760},{24307,    755},{24566,    750},{24777,    745},{24988,    740},{25198,    735},{25409,    730},{25548,    725},
    {25687,    720},{25826,    715},{25965,    710},{26104,    705},{26242,    700},{26311,    695},{26380,    690},{26449,    685},{26517,    680},{26586,    675},
    {26655,    670},{26723,    665},{26792,    660},{26861,    655},{26929,    650},{27079,    645},{27229,    640},{27379,    635},{27529,    630},{27733,    625},
    {27937,    620},{28140,    615},{28344,    610},{28547,    605},{28751,    600},{28763,    595},{28775,    590},{28787,    585},{28800,    580},{28812,    575},
    {28824,    570},{29001,    565},{29178,    560},{29354,    555},{29531,    550},{29603,    545},{29674,    540},{29746,    535},{29818,    530},{29890,    525},
    {29961,    520},{30033,    515},{30105,    510},{30177,    505},{30248,    500},{30382,    495},{30497,    490},{30593,    485},{30718,    480},{30803,    475},
    {30899,    470},{30981,    465},{31074,    460},{31150,    455},{31238,    450},{31320,    445},{31373,    440},{31459,    435},{31529,    430},{31610,    425},
    {31696,    420},{31735,    415},{31794,    410},{31839,    405},{31901,    400},{31974,    395},{32040,    390},{32078,    385},{32156,    380},{32205,    375},
    {32255,    370},{32305,    365},{32347,    360},{32389,    355},{32435,    350},{32452,    345},{32470,    340},{32540,    335},{32590,    330},{32650,    325},
    {32710,    320},{32740,    315},{32790,    310},{32830,    305},{32870,    300},{32920,    295},{32950,    290},{32990,    285},{33040,    280},{33090,    275},
    {33130,    270},{33160,    265},{33180,    260},{33230,    255},{33270,    250},{33300,    245},{33330,    240},{33390,    235},{33440,    230},{33470,    225},
    {33480,    220},{33550,    215},{33610,    210},{33650,    205},{33710,    200},{33730,    195},{33790,    190},{33830,    185},{33900,    180},{33940,    175},
    {34010,    170},{34050,    165},{34100,    160},{34140,    155},{34190,    150},{34250,    145},{34300,    140},{34390,    135},{34450,    130},{34510,    125},
    {34550,    120},{34610,    115},{34670,    110},{34730,    105},{34770,    100},{34850,     95},{34920,     90},{34990,     85},{35040,     80},{35120,     75},
    {35140,     70},{35210,     65},{35290,     60},{35320,     55},{35350,     50},{35420,     45},{35500,     40},{35530,     35},{35560,     30},{35600,     25},
    {35670,     20},{35700,     15},{35720,     10},{35770,      5},{35780,      0}
};

/*
{//AV2028 SR=22M, 2/3 CN=5.9
    {32100,    920},{32200,    915},{32350,    910},{32390,    905},{32480,    900},{32550,    895},{32620,    890},{32680,    885},{32750,    880},{32830,    875},
    {32930,    870},{33010,    865},{33100,    860},{33200,    855},{33310,    850},{33410,    845},{33520,    840},{33640,    835},{33770,    830},{33900,    825},
    {34030,    820},{34150,    815},{34290,    810},{34390,    805},{34490,    800},{34580,    795},{34700,    790},{34800,    785},{34880,    780},{34940,    775},
    {35030,    770},{35130,    765},{35180,    760},{35260,    755},{35310,    750},{35340,    745},{35380,    740},{35400,    735},{35450,    730},{35550,    725},
    {35620,    720},{35700,    715},{35800,    710},{35890,    705},{36000,    700},{36120,    695},{36180,    690},{36280,    685},{36400,    680},{36570,    675},
    {36730,    670},{36910,    665},{37060,    660},{37100,    655},{37260,    650},{37340,    645},{37410,    640},{37580,    635},{37670,    630},{37700,    625},
    {37750,    620},{37800,    615},{37860,    610},{37980,    605},{38050,    600},{38170,    595},{38370,    590},{38540,    585},{38710,    580},{38870,    575},
    {39020,    570},{39070,    565},{39100,    560},{39180,    555},{39280,    550},{39460,    545},{39510,    540},{39600,    535},{39620,    530},{39680,    525},
    {39720,    520},{39830,    515},{39880,    510},{39930,    505},{39960,    500},{40000,    495},{40200,    490},{40360,    485},{40540,    480},{40730,    475},
    {40880,    470},{41020,    465},{41150,    460},{41280,    455},{41410,    450},{41520,    445},{41620,    440},{41730,    435},{41840,    430},{41930,    425},
    {42010,    420},{42100,    415},{42180,    410},{42260,    405},{42350,    400},{42440,    395},{42520,    390},{42580,    385},{42660,    380},{42730,    375},
    {42800,    370},{42870,    365},{42940,    360},{43000,    355},{43060,    350},{43130,    345},{43180,    340},{43250,    335},{43310,    330},{43370,    325},
    {43420,    320},{43460,    315},{43520,    310},{43570,    305},{43620,    300},{43660,    295},{43710,    290},{43750,    285},{43810,    280},{43860,    275},
    {43910,    270},{43940,    265},{43990,    260},{44020,    255},{44060,    250},{44110,    245},{44140,    240},{44190,    235},{44230,    230},{44270,    225},
    {44320,    220},{44370,    215},{44400,    210},{44450,    205},{44490,    200},{44530,    195},{44590,    190},{44630,    185},{44660,    180},{44720,    175},
    {44750,    170},{44790,    165},{44830,    160},{44880,    155},{44910,    150},{44960,    145},{45000,    140},{45030,    135},{45070,    130},{45100,    125},
    {45130,    120},{45160,    115},{45200,    110},{45240,    105},{45270,    100},{45300,     95},{45330,     90},{45360,     85},{45400,     80},{45430,     75},
    {45460,     70},{45490,     65},{45530,     60},{45560,     55},{45590,     50},{45630,     45},{45670,     40},{45690,     35},{45740,     30},{45760,     25},
    {45800,     20},{45830,     15},{45860,     10},{45880,      5},{45920,      0}
};
*/

MS_U8 u8DemodLockFlag;
MS_U8       modulation_order;
static  MS_BOOL     _bDemodType=FALSE;//DVBS:FALSE   ;  S2:TRUE
//static MS_BOOL TPSLock = 0;
static MS_U32       u32ChkScanTimeStartDVBS = 0;
static MS_U8        g_dvbs_lock = 0;
//static float intern_dvb_s_qam_ref[] = {3.0, 0.0, 0.0, 0.0, 0.0, 80.0}; //16q,32q,64q,128q,256q, and others
static  MS_U8       _u8_DVBS2_CurrentCodeRate;
static  float       _fPostBer=0;
static  float       _f_DVBS_CurrentSNR=0;
static  MS_U8       _u8ToneBurstFlag=0;
static  MS_U16      _u16BlindScanStartFreq=0;
static  MS_U16      _u16BlindScanEndFreq=0;
static  MS_U16      _u16TunerCenterFreq=0;
static  MS_U16      _u16ChannelInfoIndex=0;
//Debug Only+
static  MS_U16      _u16NextCenterFreq=0;
static  MS_U16      _u16LockedSymbolRate=0;
static  MS_U16      _u16LockedCenterFreq=0;
static  MS_U16      _u16PreLockedHB=0;
static  MS_U16      _u16PreLockedLB=0;
static  MS_U16      _u16CurrentSymbolRate=0;
static  MS_S16      _s16CurrentCFO=0;
static  MS_U16      _u16CurrentStepSize=0;
//Debug Only-
static  MS_U16      _u16ChannelInfoArray[2][1000];
//static  MS_U32      _u32CurrentSR=0;
static  MS_BOOL        _bSerialTS=FALSE;
static  MS_BOOL        _bTSDataSwap=FALSE;

//Global Variables
S_CMDPKTREG gsCmdPacketDVBS;
//MS_U8 gCalIdacCh0, gCalIdacCh1;
static MS_BOOL bDMD_DVBS_NoChannelDetectedWithRFPower = FALSE;
static MS_U32 u32DMD_DVBS_NoChannelTimeAccWithRFPower = 0;
extern MS_U32  u32DMD_DVBS2_DJB_START_ADDR;
#ifdef INTERN_DVBS_LOAD_FW_FROM_CODE_MEMORY
MS_U8 INTERN_DVBS_table[] =
{
#include "fwDMD_INTERN_DVBS.dat"
};

#endif

MS_BOOL INTERN_DVBS_Show_Demod_Version(void);
MS_BOOL INTERN_DVBS_GetCurrentModulationType(DMD_DVBS_MODULATION_TYPE *pQAMMode);
MS_BOOL INTERN_DVBS_GetCurrentDemodType(DMD_DVBS_DEMOD_TYPE *pDemodType);
MS_BOOL INTERN_DVBS_GetCurrentDemodCodeRate(DMD_DVBS_CODE_RATE_TYPE *pCodeRate);
MS_BOOL INTERN_DVBS_GetCurrentSymbolRate(MS_U32 *u32SymbolRate);
MS_BOOL INTERN_DVBS_GetCurrentSymbolRateOffset(MS_U16 *pData);

#if (INTERN_DVBS_INTERNAL_DEBUG)
void INTERN_DVBS_info(void);
MS_BOOL INTERN_DVBS_Show_AGC_Info(void);
#endif

//------------------------------------------------------------------
//  System Info Function
//------------------------------------------------------------------
//=====================================================================================
MS_U16 INTERN_DVBS_DSPReg_Init(const MS_U8 *u8DVBS_DSPReg,  MS_U8 u8Size)
{
#if 0
    MS_U8   idx = 0, u8RegRead = 0, u8RegWrite = 0, u8Mask = 0;
#endif
    MS_U8   status = true;
#if 0
    MS_U16  u16DspAddr = 0;
#endif
    DBG_INTERN_DVBS(printf("INTERN_DVBS_DSPReg_Init\n"));

#if 0//def MS_DEBUG
    {
        MS_U8 u8buffer[256];
        printf("INTERN_DVBS_DSPReg_Init Reset\n");
        for (idx = 0; idx<sizeof(INTERN_DVBS_DSPREG); idx++)
            MDrv_SYS_DMD_VD_MBX_WriteDSPReg(idx, 0);

        for (idx = 0; idx<sizeof(INTERN_DVBS_DSPREG); idx++)
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(idx, &(u8buffer[idx]));
        printf("INTERN_DVBS_DSPReg_Init ReadBack, should be all 0\n");
        for (idx = 0; idx<sizeof(INTERN_DVBS_DSPREG); idx++)
            printf("%x ", u8buffer[idx]);
        printf("\n");

        printf("INTERN_DVBS_DSPReg_Init Value\n");
        for (idx = 0; idx<sizeof(INTERN_DVBS_DSPREG); idx++)
            printf("%x ", INTERN_DVBS_DSPREG[idx]);
        printf("\n");
    }
#endif

    //for (idx = 0; idx<sizeof(INTERN_DVBS_DSPREG); idx++)
        //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(idx, INTERN_DVBS_DSPREG[idx]);

    // readback to confirm.
    // ~read this to check mailbox initial values
#if 0
    for (idx = 0; idx<sizeof(INTERN_DVBS_DSPREG); idx++)
    {
        status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(idx, &u8RegRead);
        if (u8RegRead != INTERN_DVBS_DSPREG[idx])
        {
            DBG_INTERN_DVBS(printf("[Error]INTERN_DVBS_DSPReg_Init, idx=%d, drv_val=0x%x, firmware_val=0x%x\n",idx,INTERN_DVBS_DSPREG[idx],u8RegRead));
        }
    }
#endif
#if 0
    if (u8DVBS_DSPReg != NULL)
    {
        if (1 == u8DVBS_DSPReg[0])
        {
            u8DVBS_DSPReg+=2;
            for (idx = 0; idx<u8Size; idx++)
            {
                u16DspAddr = *u8DVBS_DSPReg;
                u8DVBS_DSPReg++;
                u16DspAddr = (u16DspAddr) + ((*u8DVBS_DSPReg)<<8);
                u8DVBS_DSPReg++;
                u8Mask = *u8DVBS_DSPReg;
                u8DVBS_DSPReg++;
                status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(u16DspAddr, &u8RegRead);
                u8RegWrite = (u8RegRead & (~u8Mask)) | ((*u8DVBS_DSPReg) & (u8Mask));
                u8DVBS_DSPReg++;
                DBG_INTERN_DVBS(printf("DSP addr:%x mask:%x read:%x write:%x\n", u16DspAddr, u8Mask, u8RegRead, u8RegWrite));
                status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(u16DspAddr, u8RegWrite);
            }
        }
        else
        {
            DBG_INTERN_DVBS(printf("FATAL: parameter version incorrect\n"));
        }
    }
#endif
#if 0//def MS_DEBUG
    {
        MS_U8 u8buffer[256];
        for (idx = 0; idx<sizeof(INTERN_DVBS_DSPREG); idx++)
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(idx, &(u8buffer[idx]));
        printf("INTERN_DVBC_DSPReg_Init ReadBack\n");
        for (idx = 0; idx<sizeof(INTERN_DVBS_DSPREG); idx++)
            printf("%x ", u8buffer[idx]);
        printf("\n");
    }
#endif

#if 0//def MS_DEBUG
    {
        MS_U8 u8buffer[256];
        for (idx = 0; idx<128; idx++)
            status &= MDrv_SYS_DMD_VD_MBX_ReadReg(0x2380+idx, &(u8buffer[idx]));
        printf("INTERN_DVBS_DSPReg_Init ReadReg 0x2000~0x207F\n");
        for (idx = 0; idx<128; idx++)
        {
            printf("%x ", u8buffer[idx]);
            if ((idx & 0xF) == 0xF) printf("\n");
        }
        printf("\n");
    }
#endif
    return status;
}

/***********************************************************************************
  Subject:    Command Packet Interface
  Function:   INTERN_DVBS_Cmd_Packet_Send
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
MS_BOOL INTERN_DVBS_Cmd_Packet_Send(S_CMDPKTREG* pCmdPacket, MS_U8 param_cnt)
{
    MS_U8   status = true, indx;
    MS_U8   reg_val, timeout = 0;
    return true;

    // ==== Command Phase ===================
    DBG_INTERN_DVBS(printf("--->INTERN_DVBS (cmd=0x%x)(0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,) \n",pCmdPacket->cmd_code,
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
        if (timeout > 200)
        {
            DBG_INTERN_DVBS(printf("---> INTERN_DVBS_Cmd_Packet_Send fail on 'wait _BIT_END clear' \n"));
            return false;
        }
        timeout++;
    } while (1);

    // set cmd_3:0 and _BIT_START
    reg_val = HAL_DMD_RIU_ReadByte(REG_CMD_CTRL);
    reg_val = (reg_val & 0x0f)|(pCmdPacket->cmd_code << 4)|_BIT_START;
    HAL_DMD_RIU_WriteByte(REG_CMD_CTRL, reg_val);


    //DBG_INTERN_DVBS(printf("demod_config: cmd_code = %bx\n", pCmdPacket->cmd_code));
    // wait _BIT_START clear
    do
    {
        reg_val = HAL_DMD_RIU_ReadByte(REG_CMD_CTRL);
        if((reg_val & _BIT_START) != _BIT_START)
        {
            break;
        }
        MsOS_DelayTask(10);
        if (timeout > 200)
        {
            DBG_INTERN_DVBS(printf("---> INTERN_DVBS_Cmd_Packet_Send fail on 'wait _BIT_START clear' \n"));
            return false;
        }
        timeout++;
    } while (1);

    // ==== Data Phase ======================

    HAL_DMD_RIU_WriteByte(REG_CMD_ADDR, 0x00);

    for (indx = 0; indx < param_cnt; indx++)
    {
        reg_val = HAL_DMD_RIU_ReadByte(REG_CMD_ADDR);
        //DBG_INTERN_DVBS(printf("demod_config: param[%bd] = %bx\n", reg_val, pCmdPacket->param[indx]));

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
            if (timeout > 200)
            {
                DBG_INTERN_DVBS(printf("---> INTERN_DVBS_Cmd_Packet_Send fail on 'wait _BIT_DRQ clear' \n"));
                return false;
            }
            timeout++;
        } while (1);
    }

    // ==== End Phase =======================

    // set _BIT_END to finish command
    reg_val = HAL_DMD_RIU_ReadByte(REG_CMD_CTRL);
    HAL_DMD_RIU_WriteByte(REG_CMD_CTRL, reg_val|_BIT_END);

    return status;
}

/***********************************************************************************
  Subject:    Command Packet Interface
  Function:   INTERN_DVBS_Cmd_Packet_Exe_Check
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
MS_BOOL INTERN_DVBS_Cmd_Packet_Exe_Check(MS_BOOL* cmd_done)
{
    return TRUE;
}

/***********************************************************************************
  Subject:    SoftStop
  Function:   INTERN_DVBS_SoftStop
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
MS_BOOL INTERN_DVBS_SoftStop ( void )
{
#if 1
    MS_U16     u16WaitCnt=0;

    if (HAL_DMD_RIU_ReadByte(MBRegBase + 0x00))
    {
        DBG_INTERN_DVBS(printf(">> MB Busy!\n"));
        return FALSE;
    }

    HAL_DMD_RIU_WriteByte(MBRegBase + 0x00, 0xA5);                 // MB_CNTL set read mode

    HAL_DMD_RIU_WriteByte(0x103483, 0x02);                         // assert interrupt to VD MCU51
    HAL_DMD_RIU_WriteByte(0x103483, 0x00);                         // de-assert interrupt to VD MCU51

    while(HAL_DMD_RIU_ReadByte(MBRegBase + 0x00)!= 0x5A)           // wait MB_CNTL set done
    {
        if (u16WaitCnt++ >= 0xFFF)// 0xFF)
        {
            DBG_INTERN_DVBS(printf(">> DVBT SoftStop Fail!\n"));
            return FALSE;
        }
    }

    //HAL_DMD_RIU_WriteByte(0x103460, 0x01);                       // reset VD_MCU
    HAL_DMD_RIU_WriteByte(MBRegBase + 0x00, 0x00);                 // MB_CNTL clear
#endif
    return TRUE;
}

/***********************************************************************************
  Subject:    Reset
  Function:   INTERN_DVBC_Reset
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
extern void HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake(void);

MS_BOOL INTERN_DVBS_Reset ( void )// no midify
{
    DBG_INTERN_DVBS(printf(" @INTERN_DVBS_reset\n"));

    DBG_INTERN_DVBS_TIME(printf("INTERN_DVBS_Reset, t = %d\n",MsOS_GetSystemTime()));

   //INTERN_DVBS_SoftStop();

    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00, 0x02);     // reset RIU remapping reset
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00, 0x03);     // reset DMD_MCU

    MsOS_DelayTask(1);
    HAL_DMD_RIU_WriteByte(MBRegBase + 0x00 , 0x00);     // clear MB_CNTL

    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00, 0x00);
    MsOS_DelayTask(5);

    HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake();
    HAL_DMD_RIU_WriteByte(MBRegBase + 0x00 , 0x00);

    u32ChkScanTimeStartDVBS = MsOS_GetSystemTime();
    g_dvbs_lock = 0;

    return TRUE;
}
MS_BOOL INTERN_DVBS_PowerSaving ( void )
{
    	MS_U8 i;

        //---P2=0---/;
	for( i = 0; i < 231; i++){
        MDrv_SYS_DMD_VD_MBX_WriteReg(0x350A + i, 0x11);} 
	// `M3_RIU_W((`RIUBASE_DMD_CLKGEN>>1)+7'h40, 2'b01, 16'h0000); 
	MDrv_SYS_DMD_VD_MBX_WriteReg(0x3580, 0x00);
	
	//---P2=1---/;
	for( i = 0; i < 146; i++){
   	MDrv_SYS_DMD_VD_MBX_WriteReg(0xA202 + i, 0x11);}
	// `M3_RIU_W((`RIUBASE_DMD_CLKGEN_EXT>>1)+7'h14, 2'b01, 16'h0003);                
	MDrv_SYS_DMD_VD_MBX_WriteReg(0xA228, 0x03);
		
	// ================================================================	
	// DEMOD_1 CLOCK GATED	
	// ================================================================	
	//---P2=0---/;
	for( i = 0; i <= 177; i++){
  	MDrv_SYS_DMD_VD_MBX_WriteReg(0x3635+ i, 0x11);}
	// `M3_RIU_W((`RIUBASE_DMD_CLKGEN_1>>1)+7'h1b, 2'b01, 16'h000f);       
	MDrv_SYS_DMD_VD_MBX_WriteReg(0x3636, 0x0f);

	
	// ================================================================	
// SRAM Power Down	
// ================================================================	
// [ 0]reg_force_allsram_on                 = 1'b0	
// [ 1]reg_force_allsram_on_demod_1         = 1'b0	
// [ 2]                                     = 1'b0	
// [ 3]reg_demod_1_sram_sd_en               = 1'b0	
// [ 4]reg_manhattan_sram_share_sram_sd_en  = 1'b0	
// [ 5]reg_mulan_sram_share_sram_sd_en      = 1'b0	
// [ 6]reg_dvb_frontend_sram_sd_en          = 1'b0	
// [ 7]reg_dtmb_sram_sd_en                  = 1'b0	
// [ 8]reg_dvbt_sram_sd_en                  = 1'b0	
// [ 9]reg_atsc_sram_sd_en                  = 1'b0	
// [10]reg_vif_sram_sd_en                   = 1'b0	
// [11]reg_backend_sram_sd_en               = 1'b0	
// [12]reg_adcdma_sram_sd_en                = 1'b0	
// [13]reg_isdbt_sram_sd_en                 = 1'b0	
// [14]reg_dvbt2_sram_sd_en                 = 1'b0 	
// [15]reg_dvbs2_sram_sd_en                 = 1'b0	
 // `M3_RIU_W( (`RIUBASE_DMD_TOP>>1)+7'h48, 2'b11, 16'hfffc);	
 // `M3_RIU_W( (`RIUBASE_DMD_TOP>>1)+7'h48, 2'b11, 16'hfffc);	
 MDrv_SYS_DMD_VD_MBX_WriteReg (0x2091, 0xff);	
 MDrv_SYS_DMD_VD_MBX_WriteReg (0x2090, 0xfc);	
	
// all controlled by reg_mulan_sram_share_sram_sd_en	
// reg_sram_pwr_ctrl_sel[15:0]	
 // `M3_RIU_W( (`RIUBASE_DMD_TOP_EXT>>1)+7'h70, 2'b11, 16'h0000);	
 // `M3_RIU_W( (`RIUBASE_DMD_TOP_EXT>>1)+7'h70, 2'b11, 16'h0000);	
 MDrv_SYS_DMD_VD_MBX_WriteReg (0xA1e1, 0x00);	
 MDrv_SYS_DMD_VD_MBX_WriteReg (0xA1e0, 0x00);	
// reg_sram_pwr_ctrl_sel[31:16]	
 // `M3_RIU_W( (`RIUBASE_DMD_TOP_EXT>>1)+7'h71, 2'b11, 16'h0000);	
 // `M3_RIU_W( (`RIUBASE_DMD_TOP_EXT>>1)+7'h71, 2'b11, 16'h0000);	
MDrv_SYS_DMD_VD_MBX_WriteReg (0xA1e3, 0x00);	
MDrv_SYS_DMD_VD_MBX_WriteReg (0xA1e2, 0x00);	
// reg_sram_pwr_ctrl_sel[47:32]	
 // `M3_RIU_W( (`RIUBASE_DMD_TOP_EXT>>1)+7'h72, 2'b11, 16'h0000);	
 // `M3_RIU_W( (`RIUBASE_DMD_TOP_EXT>>1)+7'h72, 2'b11, 16'h0000);	
 MDrv_SYS_DMD_VD_MBX_WriteReg (0xA1e5, 0x00);	
 MDrv_SYS_DMD_VD_MBX_WriteReg (0xA1e4, 0x00);	
// reg_sram_pwr_ctrl_sel[63:48]	
 // `M3_RIU_W( (`RIUBASE_DMD_TOP_EXT>>1)+7'h73, 2'b11, 16'h0000);	
 // `M3_RIU_W( (`RIUBASE_DMD_TOP_EXT>>1)+7'h73, 2'b11, 16'h0000);	
MDrv_SYS_DMD_VD_MBX_WriteReg (0xA1e7, 0x00);	
MDrv_SYS_DMD_VD_MBX_WriteReg (0xA1e6, 0x00);	
// reg_sram_pwr_ctrl_sel[79:64]	
 // `M3_RIU_W( (`RIUBASE_DMD_TOP_EXT>>1)+7'h74, 2'b11, 16'h0000);	
 // `M3_RIU_W( (`RIUBASE_DMD_TOP_EXT>>1)+7'h74, 2'b11, 16'h0000);	
MDrv_SYS_DMD_VD_MBX_WriteReg (0xA1e9, 0x00);	
MDrv_SYS_DMD_VD_MBX_WriteReg (0xA1e8, 0x00);	

// $display("================================================================");	
// $display("Reset");	
// $display("================================================================");	
// Release DVBT2 & dmd_ana_misc Reset	
// [0]       reg_atsc_on[0] 	
// [1]       reg_dvbt_on[1]	
// [2]       reg_vif_on[2]	
// [3]       reg_isdbt_on[3]	
// [4]       reg_atsc_rst[4]	
// [5]       reg_dvbt_rst[5]	
// [6]       reg_vif_rst[6]	
// [7]       reg_get_adc[7]	
// [8]       reg_ce8x_gate[8]	
// [9]       reg_ce_gate[9]	
// [10]      reg_dac_clk_inv[10]	
// [11]      reg_vdmcu_clock_faster[11]	
// [12]      reg_vif_if_agc_sel[12]	
// [13]      reg_dmd_ana_misc_rst[13]	
// [14]      reg_adcd_wmask[14]	
// [15]      reg_sif_only[15]	
// `M3_RIU_W( (`RIUBASE_DMD_TOP>>1)+7'h01, 2'b11, 16'h2070);	

// Release DTMB Reset & Enable Manhattan frontend Enable	
// [0]       reg_dtmb_on	
// [1]       reg_dtmb_rst	
// [4]	    reg_manhattan_frontend_on    //No used @ Maserati
// [5]	    reg_manhattan_dvb_srd_sw_rst (1'b1 for DTMB)
// `M3_RIU_W( (`RIUBASE_DMD_TOP>>1)+7'h02, 2'b01, 16'h0022);	
	
// ================================================================	
// MPLL Power Down	
// ================================================================	
// Set MPLL_ADC_DIV_SE	
// [0]   : reg_mpll_adc_clk_cc_en	
// [1]   : reg_adc_clk_pd	
// [2]   : reg_mpll_div2_pd	
// [3]   : reg_mpll_div3_pd	
// [4]   : reg_mpll_div4_pd	
// [5]   : reg_mpll_div8_pd	
// [6]   : reg_mpll_div10_pd	
// [7]   : reg_mpll_div17_pd	
// [13:8]: reg_mpll_adc_div_sel 	
// `M3_RIU_W((`RIUBASE_DMD_ANA_MISC_M3>>1)+7'h30, 2'b01, 16'h12fe);//Different	
// `M3_RIU_W((`RIUBASE_DMD_ANA_MISC_M3>>1)+7'h30, 2'b01, 16'h12fe);//Different	
MDrv_SYS_DMD_VD_MBX_WriteReg (0x2e60, 0xfe);	
	
// [2:0] : reg_mpll_ictrl   set 3'h3	
// [3]   : reg_mpll_in_sel  set 1'h0	
// [4]   : reg_mpll_xtal2adc_sel if 1'h1 ADC_CLK=XTAL.	
// [5]   : reg_mpll_xtal2next_pll_sel 	
// [6]   : reg_mpll_vco_offset(T8), reg_mpll_adc_clk_cc_mode(T9)	
// [7]   : reg_mpll_pd      set 1'b1	
// [8]   : reg_xtal_en      set 1'b0	
// [10:9]: reg_xtal_sel     set 2'h3 XTAL strength	
// [11]  : reg_mpll_porst   set 1'b1	
// [12]  : reg_mpll_reset   set 1'b1	
// [13]  : reg_pd_dmpll_clk XTAL to MPLL clock reference power down	
// [14]  : reg_mpll_pdiv_clk_pd  set 1'b0	
// Set MPLL_RESET=MPLL_PORST=1	
// `M3_RIU_W((`RIUBASE_DMD_ANA_MISC_M3>>1)+7'h35, 2'b11, 16'h1e83);//Different	
// `M3_RIU_W((`RIUBASE_DMD_ANA_MISC_M3>>1)+7'h35, 2'b11, 16'h1e83);//Different	
MDrv_SYS_DMD_VD_MBX_WriteReg (0x2e6b, 0x1e);	
MDrv_SYS_DMD_VD_MBX_WriteReg (0x2e6a, 0x83);

return TRUE;
}
/***********************************************************************************
  Subject:    Exit
  Function:   INTERN_DVBC_Exit
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
MS_BOOL INTERN_DVBS_Exit ( void )
{
    MS_U8 u8Data=0;
    MS_U8 u8Data_temp=0;

    u8Data_temp=HAL_DMD_RIU_ReadByte(0x101E39);
    HAL_DMD_RIU_WriteByte(0x101E39, 0);

    u8Data=HAL_DMD_RIU_ReadByte(0x1128C0);
    u8Data&=~(0x02);
    HAL_DMD_RIU_WriteByte(0x1128C0, u8Data);//revert IQ Swap status

    HAL_DMD_RIU_WriteByte(0x101E39, u8Data_temp);
    DBG_INTERN_DVBS(printf(" @INTERN_DVBS_Exit\n"));
    INTERN_DVBS_SoftStop();
    INTERN_DVBS_PowerSaving();

    return TRUE;
}

/***********************************************************************************
  Subject:    Load DSP code to chip
  Function:   INTERN_DVBS_LoadDSPCode
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
static MS_BOOL INTERN_DVBS_LoadDSPCode(void)
{
    MS_U8  udata = 0x00;
    MS_U16 i;
    MS_U16 fail_cnt=0;

#if (DBG_DUMP_LOAD_DSP_TIME==1)
    MS_U32 u32Time;
#endif

    //MDrv_Sys_DisableWatchDog();
/*
    HAL_DMD_RIU_WriteByte(0x103480, 0x01);//reference GUI//reset
    HAL_DMD_RIU_WriteByte(0x103481, 0x00);
    HAL_DMD_RIU_WriteByte(0x103480, 0x00);
    HAL_DMD_RIU_WriteByte(0x103483, 0x50);
    HAL_DMD_RIU_WriteByte(0x103483, 0x51);
    HAL_DMD_RIU_WriteByte(0x103484, 0x00);
    HAL_DMD_RIU_WriteByte(0x103485, 0x00);
*/
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00,  0x01);        // reset VD_MCU
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x01,  0x00);        // disable SRAM
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03,  0x50);        // enable "vdmcu51_if"
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03,  0x51);        // enable auto-increase
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x04,  0x00);        // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x05,  0x00);        // sram address high byte    

    ////  Load code thru VDMCU_IF ////
    DBG_INTERN_DVBS(printf(">Load Code.....\n"));
    for ( i = 0; i < sizeof(INTERN_DVBS_table); i++)
    {
        HAL_DMD_RIU_WriteByte(0x10348C, INTERN_DVBS_table[i]);
        //HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x0C, INTERN_DVBS_table[i]); // write data to VD MCU 51 code sram
    }

    ////  Content verification ////
    DBG_INTERN_DVBS(printf(">Verify Code...\n"));

    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x04, 0x00);         // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x05, 0x00);         // sram address high byte

    for ( i = 0; i < sizeof(INTERN_DVBS_table); i++)
    {
        udata = HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x10);    // read sram data
        if (udata != INTERN_DVBS_table[i])
        {
            printf(">fail add = 0x%x\n", i);
            printf(">code = 0x%x\n", INTERN_DVBS_table[i]);
            printf(">data = 0x%x\n", udata);

            if (fail_cnt > 10)
            {
                printf(">DVB-S DSP Loadcode fail!");
                return false;
            }
            fail_cnt++;
        }
    }

#if 0 //use for Kris DJB with VCM
    //====================================================================
    // add S2 DRAM bufer start address into fixed location
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x04,  0x30);        // sram address low byte; 0x30 is defined in FW
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x05,  0x00);        // sram address high byte

    //0x30~0x33
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)u32DMD_DVBS2_DJB_START_ADDR);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)(u32DMD_DVBS2_DJB_START_ADDR >> 8));
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)(u32DMD_DVBS2_DJB_START_ADDR >> 16));
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)(u32DMD_DVBS2_DJB_START_ADDR >> 24));

    printf("@@@@@ share dram address = 0x %x \n ",u32DMD_DVBS2_DJB_START_ADDR);
   //=====================================================================
#endif

/*
    HAL_DMD_RIU_WriteByte(0x103483, 0x50);
    HAL_DMD_RIU_WriteByte(0x103483, 0x00);
    HAL_DMD_RIU_WriteByte(0x103480, 0x01);
    HAL_DMD_RIU_WriteByte(0x103481, 0x01);
    HAL_DMD_RIU_WriteByte(0x103480, 0x00);
*/
    
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, 0x50);     // diable auto-increase
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, 0x00);     // disable "vdmcu51_if"
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x01, 0x01);     // enable SRAM
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x00, 0x00);     // release VD_MCU
    

    DBG_INTERN_DVBS(printf(">DSP Loadcode done."));
#if 0
    INTERN_DVBS_Config(6875, 128, 36125, 0,1);
    INTERN_DVBS_Active(ENABLE);
    while(1);
#endif
    //HAL_DMD_RIU_WriteByte(0x101E3E, 0x04);     // DVBT = BIT1 -> 0x02

    return TRUE;
}

/***********************************************************************************
  Subject:    DVB-S CLKGEN initialized function
  Function:   INTERN_DVBS_Power_On_Initialization
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
void INTERN_DVBS_InitClkgen(MS_BOOL bRFAGCTristateEnable)
{
    //MS_U8    u8Temp=0;
    // This file is translated by Steven Hung's riu2script.pl

    // ==============================================================
    // Start demod top initial setting by HK MCU ......
    // ==============================================================
    // [8] : reg_chiptop_dummy_0 (reg_dmdtop_dmd_sel)
    //       1'b0->reg_DMDTOP control by HK_MCU.
    //       1'b1->reg_DMDTOP control by DMD_MCU.
    // [9] : reg_chiptop_dummy_0 (reg_dmd_ana_regsel)
    //       1'b0->reg_DMDANA control by HK_MCU.
    //       1'b1->reg_DMDANA control by DMD_MCU.
    // select HK MCU ......
    // `RIU_W((`RIUBASE_CHIP>>1)+7'h1c, 2'b10, 16'h0000);
    // `RIU_W((`RIUBASE_CHIP>>1)+7'h1c, 2'b10, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x101e39,0x00);

    // ============================================================== ADC SYNC FLOW START
    // DMD_ANA_ADC_SYNC CLK_W
    // [4:0] : reg_ckg_adcd 
    //       [0]  : disable clock = 1'b1 
    //       [1]  : invert clock
    // `RIU_W((`RIUBASE_DMD_ANA_MISC>>1)+7'h68, 2'b01, 16'h0001);
    // `RIU_W((`RIUBASE_DMD_ANA_MISC>>1)+7'h68, 2'b01, 16'h0001);
    //wriu 0x1128d0 0x01
    HAL_DMD_RIU_WriteByte(0x1128d0,0x01);
    // ============================================================== ADC SYNC FLOW END

    // ==============================================================
    // Start TOP CLKGEN initial setting ......
    // ==============================================================
    // CLK_DMDMCU clock setting 
    // reg_ckg_dmdmcu@0x0f[4:0]
    // [0]  : disable clock
    // [1]  : invert clock
    // [4:2]:
    //        000:170 MHz(MPLL_DIV_BUF)
    //        001:160MHz
    //        010:144MHz
    //        011:123MHz
    //        100:108MHz (Kriti:DVBT2)
    //        101:mem_clcok
    //        110:mem_clock div 2
    //        111:select XTAL
     // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h0f, 2'b11, 16'h001c);
     // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h0f, 2'b11, 16'h001c);
     HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
     HAL_DMD_RIU_WriteByte(0x10331e, 0x10);


    // set parallel ts clock
    // [11] : reg_ckg_demod_test_in_en = 0
    //        0: select internal ADC CLK
    //        1: select external test-in clock 
    // [10] : reg_ckg_dvbtm_ts_out_mode = 1
    //        0: select gated clock 
    //        1: select free-run clock
    // [9]  : reg_ckg_atsc_dvbtc_ts_inv = 0
    //        0: normal phase to pad
    //        1: invert phase to pad
    // [8]  : reg_ckg_atsc_dvb_div_sel  = 1
    //        0: select clk_dmplldiv5
    //        1: select clk_dmplldiv3
    // [4:0]: reg_ckg_dvbtm_ts_divnum   = 11
    //        Demod TS output clock phase tuning number
    //        If (reg_ckg_tsout_ph_tun_num == reg_ckg_dvbtm_ts_divnum),
    //        Demod TS output clock is equal Demod TS internal working clock.
    //        => TS clock = (864/3)/(2*(5+1)) = 24MHz
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h00, 2'b11, 16'h0511);
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h00, 2'b11, 16'h0511);
    HAL_DMD_RIU_WriteByte(0x103301, 0x05);
    HAL_DMD_RIU_WriteByte(0x103300, 0x05);


    // enable DVBTC ts clock
    // [11:8]: reg_ckg_dvbtc_ts
    //      [8]  : disable clock
    //      [9]  : invert clock
    //      [11:10]: Select clock source
    //             00:clk_atsc_dvb_div
    //             01:62 MHz
    //             10:54 MHz
    //             11:reserved
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h04, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h04, 2'b11, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    HAL_DMD_RIU_WriteByte(0x103308, 0x00);


    // enable dvbc adc clock
    // [3:0]: reg_ckg_dvbtc_adc
    //       [0]  : disable clock 
    //       [1]  : invert clock 
    //       [3:2]: Select clock source => for demod clkgen clk_dvbtc_adc
    //          00:  clk_dmdadc
    //          01:  clk_dmdadc_div2
    //          10:  clk_dmdadc_div4
    //          11:  DFT_CLK  
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h0a, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h0a, 2'b11, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    HAL_DMD_RIU_WriteByte(0x103314, 0x00);


    // ==============================================================
    // Start demod_0 CLKGEN setting ......
    // ==============================================================
    // enable atsc_adcd_sync clock
    // [3:0] : reg_ckg_atsc_adcd_sync
    //         [0]  : disable clock 
    //         [1]  : invert clock 
    //         [3:2]: Select clock source
    //                00:  clk_dmdadc_sync
    //                01:  1'b0
    //                10:  1'b0
    //                11:  DFT_CLK  
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h05, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h05, 2'b11, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);
	
    // DMD_ANA_ADC_SYNC CLK_W
    // [4:0] : reg_ckg_adcd
    //       [0]  : disable clock = 1'b0 
    //       [1]  : invert clock
    // [12:8] : reg_ckg_adcd_q
    //       [0]  : disable clock = 1'b0 
    //       [1]  : invert clock
    // `RIU_W((`RIUBASE_DMD_ANA_MISC>>1)+7'h68, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_DMD_ANA_MISC>>1)+7'h68, 2'b11, 16'h0000);
    //wriu 0x1128d1 0x00
    //wriu 0x1128d0 0x00
    HAL_DMD_RIU_WriteByte(0x1128d1, 0x00);
    HAL_DMD_RIU_WriteByte(0x1128d0, 0x00);
	
    // DVBS2
    // @0x350c
    // [3:0] : reg_ckg_dvbs_outer1x
    //         [0]  : disable clock 
    //         [1]  : invert clock 
    //         [3:2]: Select clock source
    //               00:  adc_clk_buf
    //               01:  dvb_clk86_buf
    //               10:  dvb_clk43_buf
    //               11:  1'b0
    // [6:4] : reg_ckg_dvbs_outer2x
    //         [4] : disable clock 
    //         [5] : invert clock 
    //         [6] : Select clock source
    //               00:  adc_clk_buf
    //               01:  1'b0
    //               10:  1'b0
    //               11:  DFT_CLK  
    // [10:8]: reg_ckg_dvbs2_inner
    //         [8] : disable clock 
    //         [9] : invert clock 
    //         [10]: Select clock source
    //               00:  adc_clk_buf
    //               01:  1'b0
    //               10:  1'b0
    //               11:  DFT_CLK
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h0c, 2'b11, 16'h0000);
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h0c, 2'b11, 16'h0000);
      HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
      HAL_DMD_RIU_WriteByte(0x111f18, 0x00);


    // DVBS2
    // @0x350d
    // [11:8]: reg_ckg_dvbs2_oppro
    //         [8]    : disable clock 
    //         [9]    : invert clock 
    //         [11:10]: Select clock source
    //                  00:  mpll_clk144_buf
    //                  01:  mpll_clk96_buf
    //                  10:  mpll_clk72_buf
    //                  11:  mpll_clk48_buf
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h0d, 2'b11, 16'h0000);
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h0d, 2'b11, 16'h0000);
      HAL_DMD_RIU_WriteByte(0x111f1b, 0x00);
      HAL_DMD_RIU_WriteByte(0x111f1a, 0x00);


    // @0x3510
    // [3:0] : reg_ckg_dvbtm_adc
    //         N/A
    // [6:4] : reg_ckg_dvbt_inner1x
    //         [4] : disable clock 
    //         [5] : invert clock 
    //         [6] : Select clock source
    //               00:  dvb_clk24_buf
    //               01:  dvb_clk21p5_buf
    //               10:  1'b0
    //               11:  DFT_CLK  
    // [10:8]    reg_ckg_dvbt_inner2x
    //         [8] : disable clock 
    //         [9] : invert clock 
    //         [10]: Select clock source
    //               00:  dvb_clk48_buf
    //               01:  dvb_clk43_buf
    //               10:  1'b0
    //               11:  DFT_CLK
    // [14:12]    reg_ckg_dvbt_inner4x
    //         [12]: disable clock 
    //         [13]: invert clock 
    //         [14]: Select clock source
    //               00:  dvb_clk96_buf
    //               01:  dvb_clk86_buf
    //               10:  1'b0
    //               11:  DFT_CLK
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h10, 2'b11, 16'h1110);
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h10, 2'b11, 16'h1110);
      HAL_DMD_RIU_WriteByte(0x111f21, 0x11);
      HAL_DMD_RIU_WriteByte(0x111f20, 0x10);

    // @0x3511
    // [2:0] : reg_ckg_dvbt_outer1x
    //         [0] : disable clock 
    //         [1] : invert clock 
    //         [2] : Select clock source
    //               00:  dvb_clk48_buf
    //               01:  dvb_clk43_buf
    //               10:  1'b0
    //               11:  DFT_CLK
    // [6:4] : reg_ckg_dvbt_outer2x
    //         [4] : disable clock 
    //         [5] : invert clock 
    //         [6] : Select clock source
    //               00:  dvb_clk96_buf
    //               01:  dvb_clk86_buf
    //               10:  1'b0
    //               11:  DFT_CLK
    // [11:8]: reg_ckg_dvbtc_outer2x
    //         [8] : disable clock 
    //         [9] : invert clock 
    //         [11:10]: Select clock source
    //               00:  mpll_clk57p6_buf
    //               01:  dvb_clk43_buf
    //               10:  dvb_clk86_buf
    //               11:  dvb_clk96_buf
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h11, 2'b11, 16'h0c11);
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h11, 2'b11, 16'h0c11);
      HAL_DMD_RIU_WriteByte(0x111f23, 0x0c);
      HAL_DMD_RIU_WriteByte(0x111f22, 0x11);


    // @0x3512
    // [11:8]: reg_ckg_acifir
    //         [8] : disable clock 
    //         [9] : invert clock 
    //         [11:10]: Select clock source
    //               000:  1'b0
    //               001:  clk_dmdadc
    //               010:  clk_vif_ssc_mux
    //               011:  1'b0
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h12, 2'b10, 16'h0400);
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h12, 2'b10, 16'h0400);
      HAL_DMD_RIU_WriteByte(0x111f25, 0x04);


    // @0x3514
    // [12:8]: reg_ckg_dvbtm_sram_t1o2x_t22x
    //         [8] : disable clock 
    //         [9] : invert clock 
    //         [12:10]: Select clock source
    //               000:  dvb_clk48_buf
    //               001:  dvb_clk43_buf
    //               010:  1'b0
    //               011:  1'b0
    //               100:  1'b0
    //               101:  1'b0
    //               110:  1'b0
    //               111:  1'b0
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h14, 2'b11, 16'h0000);
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h14, 2'b11, 16'h0000);
      HAL_DMD_RIU_WriteByte(0x111f29, 0x00);
      HAL_DMD_RIU_WriteByte(0x111f28, 0x00);


    // @0x3516
    // [8:4] : reg_ckg_dvbtm_sram_adc_t22x
    //         [4]  : disable clock 
    //         [5]  : invert clock 
    //         [8:6]: Select clock source
    //                000:  dvb_clk48_buf
    //                001:  dvb_clk43_buf
    //                010:  1'b0
    //                011:  1'b0
    //                100:  adc_clk_buf
    //                101:  1'b0
    //                110:  1'b0
    //                111:  1'b0
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h16, 2'b11, 16'h0001);
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h16, 2'b11, 16'h0001);
      HAL_DMD_RIU_WriteByte(0x111f2d, 0x00);
      HAL_DMD_RIU_WriteByte(0x111f2c, 0x01);


    // @0x3517
    // [4:0] : reg_ckg_dvbtm_sram_t12x_t22x
    //         [0]  : disable clock 
    //         [1]  : invert clock 
    //         [4:2]: Select clock source
    //                000:  dvb_clk48_buf
    //                001:  dvb_clk43_buf
    //                010:  1'b0
    //                011:  1'b0
    //                100:  1'b0
    //                101:  1'b0
    //                110:  1'b0
    //                111:  1'b0
    // [12:8]    reg_ckg_dvbtm_sram_t12x_t24x
    //         [8]  : disable clock 
    //         [9]  : invert clock 
    //         [12:10]: Select clock source
    //                000:  dvb_clk96_buf
    //                001:  dvb_clk86_buf
    //                010:  dvb_clk48_buf
    //                011:  dvb_clk43_buf
    //                100:  1'b0
    //                101:  1'b0
    //                110:  1'b0
    //                111:  1'b0
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h17, 2'b11, 16'h0000);
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h17, 2'b11, 16'h0000);
      HAL_DMD_RIU_WriteByte(0x111f2f, 0x00);
      HAL_DMD_RIU_WriteByte(0x111f2e, 0x00);


    // @0x3518
    // [4:0] : reg_ckg_dvbtm_sram_t14x_t24x
    //         [0]  : disable clock 
    //         [1]  : invert clock 
    //         [4:2]: Select clock source
    //                000:  dvb_clk96_buf
    //                001:  dvb_clk96_buf
    //                010:  1'b0
    //                011:  1'b0
    //                100:  1'b0
    //                101:  1'b0
    //                110:  1'b0
    //                111:  1'b0
    // [12:8]: reg_ckg_dvbtm_ts_in
    //         [8]  : disable clock 
    //         [9]  : invert clock 
    //         [12:10]: Select clock source
    //                000:  clk_dvbtc_rs_p
    //                001:  dvb_clk48_buf
    //                010:  dvb_clk43_buf
    //                011:  clk_dvbs_outer1x_pre_mux4
    //                100:  clk_dvbs2_oppro_pre_mux4
    //                101:  1'b0
    //                110:  1'b0
    //                111:  1'b0
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h18, 2'b11, 16'h0001);
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h18, 2'b11, 16'h0001);
      HAL_DMD_RIU_WriteByte(0x111f31, 0x00);
      HAL_DMD_RIU_WriteByte(0x111f30, 0x01);


    // @0x3519
    // [2:0] : reg_ckg_tdp_jl_inner1x
    //         [0] : disable clock 
    //         [1] : invert clock 
    //         [2] : Select clock source
    //               00:  dvb_clk24_buf
    //               01:  dvb_clk21p5_buf
    //               10:  1'b0
    //               11:  DFT_CLK
    // [6:4] : reg_ckg_tdp_jl_inner4x
    //         [4] : disable clock 
    //         [5] : invert clock 
    //         [6] : Select clock source
    //               00:  dvb_clk96_buf
    //               01:  dvb_clk86_buf
    //               10:  1'b0
    //               11:  DFT_CLK
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h19, 2'b11, 16'h3c00);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h19, 2'b11, 16'h3c00);
    HAL_DMD_RIU_WriteByte(0x111f33, 0x3c);
    HAL_DMD_RIU_WriteByte(0x111f32, 0x00);


    // @0x351a
    // [6:4] : reg_ckg_dvbt2_inner1x
    //         [4] : disable clock 
    //         [5] : invert clock 
    //         [6] : Select clock source
    //               00:  dvb_clk96_buf
    //               01:  dvb_clk86_buf
    //               10:  1'b0
    //               11:  DFT_CLK
    // [10:8]: reg_ckg_dvbt2_inner2x
    //         [8] : disable clock 
    //         [9] : invert clock 
    //         [10]: Select clock source
    //               00:  dvb_clk48_buf
    //               01:  dvb_clk43_buf
    //               10:  1'b0
    //               11:  DFT_CLK
    // [14:12]:reg_ckg_dvbt2_inner4x
    //         [12] : disable clock 
    //         [13] : invert clock 
    //         [14] : Select clock source
    //               00:  dvb_clk96_buf
    //               01:  dvb_clk86_buf
    //               10:  1'b0
    //               11:  DFT_CLK
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h1a, 2'b11, 16'h0000);
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h1a, 2'b11, 16'h0000);
      HAL_DMD_RIU_WriteByte(0x111f35, 0x00);
      HAL_DMD_RIU_WriteByte(0x111f34, 0x00);


    // @0x351b
    // [1:0] : reg_ckg_dvbt2_ldpc
    //         DVBT2 LDPC gated clock control register
    //         [0] = 1:clock enable.
    //         [1] = 1:manual mode. 
    // [3:2] : reg_ckg_dvbt2_bch
    //         DVBT2 BCH gated clock control register; 
    //         [0] = 1:clock enable
    //         [1] = 1:manual mode. 
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h1b, 2'b11, 16'h0011);
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h1b, 2'b11, 16'h0011);
      HAL_DMD_RIU_WriteByte(0x111f37, 0x00);
      HAL_DMD_RIU_WriteByte(0x111f36, 0x11);


    // @0x351d
    // [4:0] : reg_ckg_dvbtm_adc_eq_1x
    //         [0] : disable clock 
    //         [1] : invert clock 
    //         [2] : Select clock source
    //               00:  adc_clk_buf
    //               01:  1'b0
    //               10:  1'b0
    //               11:  DFT_CLK
    // [12:8]: reg_ckg_dvbtm_adc_eq_0p5x
    //         [4] : disable clock 
    //         [5] : invert clock 
    //         [6]: Select clock source
    //               00:  clk_adc_div2_buf
    //               01:  1'b0
    //               10:  1'b0
    //               11:  DFT_CLK
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h1d, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h1d, 2'b11, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x111f3b, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f3a, 0x00);


    // @0x351e
    // [4:0] : reg_ckg_dvbtm_sram_t11x_t22x
    //         [0]  : disable clock 
    //         [1]  : invert clock 
    //         [4:2]: Select clock source
    //                000:  dvb_clk48_buf
    //                001:  dvb_clk43_buf
    //                010:  dvb_clk24_buf
    //                011:  dvb_clk21p5_buf
    //                100:  1'b0
    //                101:  1'b0
    //                110:  1'b0
    //                111:  1'b0
    // [12:8]: reg_ckg_dvbtm_sram_t11x_t24x
    //         [8]  : disable clock 
    //         [9]  : invert clock 
    //         [:2]: Select clock source
    //                000:  dvb_clk48_buf
    //                001:  dvb_clk43_buf
    //                010:  dvb_clk24_buf
    //                011:  dvb_clk21p5_buf
    //                100:  1'b0
    //                101:  1'b0
    //                110:  1'b0
    //                111:  1'b0
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h1e, 2'b11, 16'h0c04);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h1e, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h1e, 2'b11, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x111f3d, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f3c, 0x00);


    // @0x3522
    // [3:0] : reg_ckg_dvbt_t2_inner0p5x_dvbc_eq1x
    //         [0] : disable clock 
    //         [1] : invert clock 
    //         [2] : Select clock source
    //               00:  dvb_clk12_buf
    //               01:  dvb_clk10p75_buf
    //               10:  1'b0
    //               11:  DFT_CLK
    // [7:4] : reg_ckg_dvbt_t2_inner2x_dvbc_eq4x
    //         [4] : disable clock 
    //         [5] : invert clock 
    //         [6] : Select clock source
    //               00:  dvb_clk48_buf
    //               01:  dvb_clk43_buf
    //               10:  1'b0
    //               11:  DFT_CLK
    // [11:8]: reg_ckg_dvbt_t2_inner1x
    //         [8] : disable clock 
    //         [9] : invert clock 
    //         [11:10]: Select clock source
    //               00:  dvb_clk24_buf
    //               01:  dvb_clk21p5_buf
    //               10:  1'b0
    //               11:  DFT_CLK
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h22, 2'b11, 16'h0111);
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h22, 2'b11, 16'h0111);
      HAL_DMD_RIU_WriteByte(0x111f45, 0x01);
      HAL_DMD_RIU_WriteByte(0x111f44, 0x11);

    // @0x353a
    // [2:0] : reg_ckg_dvbtm_sram_t12x_t24x_srd1x_isdbt_inner2x
    //         [0] : disable clock 
    //         [1] : invert clock 
    //         [2] : Select clock source
    //               00:  clk_dvbtm_sram_t12x_t24x_srd1x_p
    //               01:  clk_isdbt_inner2x_p
    //               10:  1'b0
    //               11:  DFT_CLK
    // [6:4] : reg_ckg_dvbtm_sram_t12x_t24x_isdbt_inner2x
    //         [4] : disable clock 
    //         [5] : invert clock 
    //         [6] : Select clock source
    //               00:  clk_dvbtm_sram_t12x_t24x_p
    //               01:  clk_isdbt_inner2x_p
    //               10:  1'b0
    //               11:  DFT_CLK
    // [10:8]: reg_ckg_dvbtm_sram_t24x_isdbt_inner2x
    //         [8] : disable clock 
    //         [9] : invert clock 
    //         [10]: Select clock source
    //               00:  clk_dvbtm_sram_t14x_t24x_p
    //               01:  clk_isdbt_inner2x_p
    //               10:  1'b0
    //               11:  DFT_CLK
    // [14:12]: reg_ckg_dvbtm_sram_t12x_t24x_s2inner_isdbt_inner4x
    //         [12] : disable clock 
    //         [13] : invert clock 
    //         [14] : Select clock source
    //               00:  clk_dvbtm_sram_t12x_t24x_s2inner_p
    //               01:  clk_isdbt_inner4x_p
    //               10:  1'b0
    //               11:  DFT_CLK
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h3a, 2'b11, 16'h0110);
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h3a, 2'b11, 16'h0110);
      HAL_DMD_RIU_WriteByte(0x111f75, 0x01);
      HAL_DMD_RIU_WriteByte(0x111f74, 0x10);

    // @0x353b
    // [2:0] : reg_ckg_dvbtm_sram_t12x_t24x_s2inner_isdbt_inner2x
    //         [0] : disable clock 
    //         [1] : invert clock 
    //         [2] : Select clock source
    //               00:  clk_dvbtm_sram_t12x_t24x_s2inner_p
    //               01:  clk_isdbt_inner2x_p
    //               10:  1'b0
    //               11:  DFT_CLK
    // [6:4] : reg_ckg_dvbtm_sram_t22x_isdbt_inner2x
    //         [4] : disable clock 
    //         [5] : invert clock 
    //         [6] : Select clock source
    //               00:  clk_dvbtm_sram_t12x_t22x_p
    //               01:  clk_isdbt_inner2x_p
    //               10:  1'b0
    //               11:  DFT_CLK
    // [10:8]: reg_ckg_dvbtm_sram_t14x_t24x_s2inner_isdbt_inner2x
    //         [8] : disable clock 
    //         [9] : invert clock 
    //         [10]: Select clock source
    //               00:  clk_dvbtm_sram_t14x_t24x_s2inner_p
    //               01:  clk_isdbt_inner2x_p
    //               10:  1'b0
    //               11:  DFT_CLK
    // [14:12]: reg_ckg_dvbtm_sram_t12x_t24x_srd1x_isdbt_inner4x
    //         [12] : disable clock 
    //         [13] : invert clock 
    //         [14]: Select clock source
    //               00:  clk_dvbtm_sram_t12x_t24x_srd1x_p
    //               01:  clk_isdbt_inner4x_p
    //               10:  1'b0
    //               11:  DFT_CLK
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h3b, 2'b11, 16'h0010);
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h3b, 2'b11, 16'h0010);
      HAL_DMD_RIU_WriteByte(0x111f77, 0x00);
      HAL_DMD_RIU_WriteByte(0x111f76, 0x10);

    // @0x353c
    // [2:0] : reg_ckg_dvbtm_sram_t14x_t24x_srd1x_vifssc_isdbt_inner4x
    //         [0] : disable clock 
    //         [1] : invert clock 
    //         [2] : Select clock source
    //               00:  clk_dvbtm_sram_t14x_t24x_srd1x_vifssc_p
    //               01:  clk_isdbt_inner4x_p
    //               10:  1'b0
    //               11:  DFT_CLK
    // [6:4] : reg_ckg_dvbtm_sram_t12x_t22x_isdbt_inner2x
    //         [4] : disable clock 
    //         [5] : invert clock 
    //         [6] : Select clock source
    //               00:  clk_dvbtm_sram_t12x_t22x_p
    //               01:  clk_isdbt_inner2x_p
    //               10:  1'b0
    //               11:  DFT_CLK
    // [10:8]: reg_ckg_dvbtm_sram_t11x_t22x_isdbt_inner2x
    //         [8] : disable clock 
    //         [9] : invert clock 
    //         [10]: Select clock source
    //               00:  clk_dvbtm_sram_t11x_t22x_p
    //               01:  clk_isdbt_inner2x_p
    //               10:  1'b0
    //               11:  DFT_CLK
    // [14:12]: reg_ckg_dvbtm_sram_t12x_t24x_isdbt_outer6x
    //         [12] : disable clock 
    //         [13] : invert clock 
    //         [14]: Select clock source
    //               00:  clk_dvbtm_sram_t12x_t24x_p
    //               01:  clk_isdbt_outer6x_dvbt_outer2x_c_mux
    //               10:  1'b0
    //               11:  DFT_CLK
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h3c, 2'b11, 16'h0110);
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h3c, 2'b11, 16'h0110);
      HAL_DMD_RIU_WriteByte(0x111f79, 0x01);
      HAL_DMD_RIU_WriteByte(0x111f78, 0x10);

    // @0x353e
    // [2:0] : reg_ckg_dvbs_outer2x_dvbt_outer2x_dvbt2_inner2x_isdbt_outer6x
    //         [0] : disable clock 
    //         [1] : invert clock 
    //         [2] : Select clock source
    //               00:  clk_dvbs_outer2x_dvbt_outer2x_dvbt2_inner2x_mux8
    //               01:  clk_isdbt_outer6x_p
    //               10:  1'b0
    //               11:  DFT_CLK
    // [6:4] : reg_ckg_dvbtm_sram_t22x_miu
    //         [4] : disable clock 
    //         [5] : invert clock 
    //         [6] : Select clock source
    //               00:  clk_dvbt2_inner2x_p
    //               01:  clk_miu_p
    //               10:  1'b0
    //               11:  DFT_CLK
    // [10:8]: reg_ckg_dvbtm_sram_adc_t22x_isdbt_inner2x
    //         [8] : disable clock 
    //         [9] : invert clock 
    //         [10]: Select clock source
    //               00:  clk_dvbtm_sram_adc_t22x_p
    //               01:  clk_isdbt_inner2x_p
    //               10:  1'b0
    //               11:  DFT_CLK
    // [14:12]: reg_ckg_dvbs_outer2x_dvbt_outer2x_dvbt2_inner2x_miu
    //         [12] : disable clock 
    //         [13] : invert clock 
    //         [14]: Select clock source
    //               00:  clk_dvbs_outer2x_dvbt_outer2x_dvbt2_inner2x_mux8
    //               01:  clk_miu_p
    //               10:  1'b0
    //               11:  DFT_CLK
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h3e, 2'b11, 16'h1111);
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h3e, 2'b11, 16'h1111);
      HAL_DMD_RIU_WriteByte(0x111f7d, 0x11);
      HAL_DMD_RIU_WriteByte(0x111f7c, 0x11);

    // @0x353f
    // [2:0] : reg_ckg_dvbs_outer2x_dvbt_outer2x_miu_isdbt_outer6x
    //         [0] : disable clock 
    //         [1] : invert clock 
    //         [2] : Select clock source
    //               00:  clk_dvbs_outer2x_dvbt_outer2x_miu_mux8
    //               01:  clk_isdbt_outer6x_p
    //               10:  1'b0
    //               11:  DFT_CLK
    // [6:4] : reg_ckg_dvbtm_sram_t22x_dvbtc_rs
    //         [4] : disable clock 
    //         [5] : invert clock 
    //         [6] : Select clock source
    //               00:  clk_dvbt2_inner2x_p
    //               01:  clk_dvbtc_rs_p
    //               10:  1'b0
    //               11:  DFT_CLK
    // [10:8]: reg_ckg_dvbtc_outer2x_isdbt_outer_rs
    //         [8] : disable clock 
    //         [9] : invert clock 
    //         [10]: Select clock source
    //               00:  clk_dvbtc_outer2x_p
    //               01:  clk_isdbt_outer_rs_p
    //               10:  1'b0
    //               11:  DFT_CLK
    // [14:12]: reg_ckg_dvbtm_sram_t22x_isdbt_outer6x_dvbt_outer2x
    //         [12] : disable clock 
    //         [13] : invert clock 
    //         [14]: Select clock source
    //               00:  clk_dvbtm_sram_t12x_t22x_p
    //               01:  clk_isdbt_outer6x_dvbt_outer2x_mux
    //               10:  1'b0
    //               11:  DFT_CLK
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h3f, 2'b11, 16'h1041);
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h3f, 2'b11, 16'h1041);
      HAL_DMD_RIU_WriteByte(0x111f7f, 0x10);
      HAL_DMD_RIU_WriteByte(0x111f7e, 0x41);


    // @0x3570
    // [4:0] : reg_ckg_dvbt_inner2x_srd0p5x
    //         [0] : disable clock 
    //         [1] : invert clock 
    //         [3:2]: Select clock source
    //               00:  dvb_clk48_buf
    //               01:  dvb_clk43_buf
    //               10:  clk_adc_div2_buf
    //               11:  1'b0
    //               11:  1'b0
    // [13:8]: reg_ckg_dvbtm_sram_t1outer1x_t24x
    //         [8] : disable clock 
    //         [9] : invert clock 
    //         [12:10]: Select clock source
    //                  000:  dvb_clk96_buf
    //                  001:  dvb_clk86_buf
    //                  010:  dvb_clk48_buf
    //                  011:  dvb_clk43_buf
    //                  100:  1'b0
    //                  101:  1'b0
    //                  110:  1'b0
    //                  111:  1'b0
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h70, 2'b11, 16'h0008);
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h70, 2'b11, 16'h0008);
      HAL_DMD_RIU_WriteByte(0x111fe1, 0x00);
      HAL_DMD_RIU_WriteByte(0x111fe0, 0x08);


    // @0x3571
    // [4:0] : reg_ckg_dvbtm_sram_t12x_t24x_srd1x
    //         [0] : disable clock 
    //         [1] : invert clock 
    //         [3:2]: Select clock source
    //                000:  dvb_clk96_buf
    //                001:  dvb_clk86_buf
    //                010:  dvb_clk48_buf
    //                011:  dvb_clk43_buf
    //                100:  adc_clk_buf
    //                101:  1'b0
    //                110:  1'b0
    //                111:  1'b0
    // [12:8]: reg_ckg_dvbtm_sram_t14x_t24x_srd1x
    //         [8] : disable clock 
    //         [9] : invert clock 
    //         [12:10]: Select clock source
    //                000:  dvb_clk96_buf
    //                001:  dvb_clk86_buf
    //                010:  adc_clk_buf
    //                011:  1'b0
    //                100:  1'b0
    //                101:  1'b0
    //                110:  1'b0
    //                111:  1'b0
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h71, 2'b11, 16'h0810);
      // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h71, 2'b11, 16'h0810);
      HAL_DMD_RIU_WriteByte(0x111fe3, 0x08);
      HAL_DMD_RIU_WriteByte(0x111fe2, 0x10);


    // @0x3572
    // [6:0] : reg_ckg_dvbt2_s2_bch_out
    //         [0] : disable clock 
    //         [1] : invert clock 
    //         [2] : Select clock source
    //               00:  dvb_clk48_buf
    //               01:  dvb_clk43_buf
    //               10:  1'b0
    //               11:  DFT_CLK
    // [12:8]: reg_ckg_dvbt2_outer2x
    //         [8] : disable clock 
    //         [9] : invert clock 
    //         [12:10]: Select clock source
    //                  000:  mpll_clk144_buf
    //                  001:  mpll_clk108_buf
    //                  010:  mpll_clk96_buf
    //                  011:  mpll_clk72_buf
    //                  100:  mpll_clk54_buf
    //                  101:  mpll_clk48_buf
    //                  110:  mpll_clk36_buf
    //                  111:  mpll_clk24_buf
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h72, 2'b11, 16'h0008);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h72, 2'b11, 16'h0008);
    HAL_DMD_RIU_WriteByte(0x111fe5, 0x00);
    HAL_DMD_RIU_WriteByte(0x111fe4, 0x08);


    // @0x3573
    // [3:0] : reg_ckg_dvbt2_inner4x_s2_inner
    //         [0] : disable clock 
    //         [1] : invert clock 
    //         [2] : Select clock source
    //               00:  dvb_clk96_buf
    //               01:  dvb_clk86_buf
    //               10:  1'b0
    //               11:  DFT_CLK
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h73, 2'b11, 16'h0008);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h73, 2'b11, 16'h0008);
    HAL_DMD_RIU_WriteByte(0x111fe7, 0x00);
    HAL_DMD_RIU_WriteByte(0x111fe6, 0x08);


    // @0x3574
    // [4:0]    reg_ckg_dvbtm_sram_t12x_t24x_s2inner
    //         [0] : disable clock 
    //         [1] : invert clock 
    //         [4:2]:Select clock source
    //                  000:  dvb_clk96_buf
    //                  001:  dvb_clk86_buf
    //                  010:  dvb_clk48_buf
    //                  011:  dvb_clk43_buf
    //                  100:  adc_clk_buf
    //                  101:  1'b0
    //                  110:  1'b0
    //                  111:  1'b0
    // [12:8]    reg_ckg_dvbtm_sram_t14x_t24x_s2inner
    //         [8] : disable clock 
    //         [9] : invert clock 
    //         [12:10]: Select clock source
    //                  000:  dvb_clk96_buf
    //                  001:  dvb_clk86_buf
    //                  010:  adc_clk_buf
    //                  011:  dvb_clk24_buf         //JL SRAM Share (Windermere U02 ECO)
    //                  100:  dvb_clk21p5_buf       //JL SRAM Share (Windermere U02 ECO)
    //                  101:  1'b0
    //                  110:  1'b0
    //                  111:  1'b0
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h74, 2'b11, 16'h0810);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h74, 2'b11, 16'h0810);
    HAL_DMD_RIU_WriteByte(0x111fe9, 0x08);
    HAL_DMD_RIU_WriteByte(0x111fe8, 0x10);


    // @0x3575
    // [4:0] : reg_ckg_dvbtc_rs
    //         [0] : disable clock 
    //         [1] : invert clock 
    //         [4:2]:Select clock source
    //               000:  mpll_clk216_buf
    //               001:  mpll_clk172p8_buf
    //               010:  mpll_clk144_buf
    //               011:  mpll_clk288_buf 
    //               100:  dvb_clk96_buf
    //               101:  dvb_clk86_buf
    //               110:  mpll_clk57p6_buf
    //               111:  dvb_clk43_buf
    // [11:8] : reg_ckg_dvbs_outer2x_dvbt_outer2x (N/A)
    // [15:12]: reg_ckg_dvbs_outer2x_dvbt_outer2x_miu
    //         [12] : disable clock 
    //         [13] : invert clock 
    //         [15:14]:Select clock source
    //                 000:  1'b0
    //                 001:  dvb_clk96_buf
    //                 010:  dvb_clk86_buf
    //                 011:  clk_miu
    //                 100:  1'b0
    //                 101:  1'b0
    //                 110:  1'b0
    //                 111:  1'b0
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h75, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h75, 2'b11, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x111feb, 0x00);
    HAL_DMD_RIU_WriteByte(0x111fea, 0x00);


    // @0x3576
    // [4:0] : reg_ckg_dvbs_outer2x_dvbt_outer2x_dvbt2_inner2x
    //         [0] : disable clock 
    //         [1] : invert clock 
    //         [4:2]:Select clock source
    //               000:  1'b0
    //               001:  dvb_clk96_buf
    //               010:  dvb_clk86_buf
    //               011:  dvb_clk48_buf
    //               100:  dvb_clk43_buf
    //               101:  1'b0
    //               110:  1'b0
    //               111:  1'b0
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h76, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h76, 2'b11, 16'h0000);
    HAL_DMD_RIU_WriteByte(0x111fed, 0x00);
    HAL_DMD_RIU_WriteByte(0x111fec, 0x00);


    // @0x3577
    // [3:0] : reg_ckg_dvbt2_inner4x_dvbtc_rs
    //         [0] : disable clock 
    //         [1] : invert clock 
    //         [3:2]: Select clock source
    //               00:  dvb_clk96_buf
    //               01:  dvb_clk86_buf
    //               10:  clk_dvbtc_rs_p
    //               11:  1'b0
    // [8:4] : reg_ckg_dvbtm_sram_adc_t22x_dvbtc_rs
    //         [4] : disable clock 
    //         [5] : invert clock 
    //         [6] : Select clock source
    //               000:  dvb_clk48_buf
    //               001:  dvb_clk43_buf
    //               010:  1'b0
    //               011:  adc_clk_buf
    //               100:  1'b0
    //               101:  1'b0
    //               110:  1'b0
    //               111:  1'b0
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h77, 2'b11, 16'h0088);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h77, 2'b11, 16'h0088);
    HAL_DMD_RIU_WriteByte(0x111fef, 0x00);
    HAL_DMD_RIU_WriteByte(0x111fee, 0x88);


    // Maserati
    // @0x3578
    // [4:0] : reg_ckg_dvbt2_inner2x_srd0p5x
    //         [0] : disable clock 
    //         [1] : invert clock 
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h78, 2'b01, 16'h0008);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h78, 2'b01, 16'h0008);
    HAL_DMD_RIU_WriteByte(0x111ff0, 0x08);

    // [3:0] : reg_ckg_sram_t22x_isdbt_inn2x_dtmb_inn2x
    //         [0] : disable clock 
    //         [1] : invert clock 
    //         [3:2]:Select clock source
    //               000:  clk_dvbtm_sram_t12x_t22x_p
    //               001:  clk_isdbt_inner2x_p
    //               010:  clk_share_dtmb_inner2x_isdbt_sram4_mux
    //               011:   
    // [7:4] : reg_ckg_sram_t14x_t24x_s2inn_isdbt_inn2x_dtmb_inn6x
    //         [4] : disable clock 
    //         [5] : invert clock 
    //         [7:6]:Select clock source
    //               000:  clk_dvbtm_sram_t14x_t24x_s2inner_p
    //               001:  clk_isdbt_inner2x_p
    //               010:  clk_share_dtmb_inner6x_isdbt_sram3_mux
    //               011:   
    // [11:8] : reg_ckg_sram_t14x_t24x_s2inn_isdbt_inn2x_dtmb_eq2x
    //         [4] : disable clock 
    //         [5] : invert clock 
    //         [7:6]:Select clock source
    //               000:  clk_dvbtm_sram_t14x_t24x_s2inner_p
    //               001:  clk_isdbt_inner2x_p
    //               010:  clk_share_dtmb_eq2x_isdbt_sram3_mux
    //               011:   
    // [15:12]: reg_ckg_dvbtm_sram_t14x_t24x_srd1x_vifssc_isdbt_inner4x_dtmb_inner12x
    //         [12] : disable clock 
    //         [13] : invert clock 
    //         [15:14]:Select clock source
    //                 000:  clk_dvbtm_sram_t14x_t24x_srd1x_vifssc_p
    //                 001:  clk_isdbt_inner4x_p
    //                 010:  clk_dvbtc_sram2_p
    //                 011:  clk_dtmb_eq2x_inner2x_12x_mux
    // `RIU_W((`RIUBASE_DEMOD_8>>1)+7'h48, 2'b11, 16'h0001);
    // `RIU_W((`RIUBASE_DEMOD_8>>1)+7'h48, 2'b11, 16'h0001);
    HAL_DMD_RIU_WriteByte(0x152991, 0x00);
    HAL_DMD_RIU_WriteByte(0x152990, 0x01);
    // ==============================================================
    // End demod top initial setting by HK MCU ......
    // ==============================================================
//wriu 0x101e39 0x03
    HAL_DMD_RIU_WriteByte(0x101e39, 0x03);

    //==========================================================
    //diseqc_out : PAD_GPIO13_I
    //swich to Diseqc out pin from GPIO
    //==========================================================
    //Bank: Reg_CHIP_TOP(0x101e)
    //reg_test_out_mode : addr h・12, [6:4] = 3・h0
    //reg_ts4config : addr h・40, [11:10] = 2・h0
    //reg_ts5config : addr h・40, [13:12] = 2・h0
    //reg_i2smutemode : addr h・2, [15:14] = 2・h0
    //reg_fifthuartmode : h・4, [3:2] = 2・h0
    //reg_od5thuart : h・55, [5:4] = 2・h0
    //reg_diseqc_out_config : ．h45, [1] = 1・b1
    //u8Temp = HAL_DMD_RIU_ReadByte(0x101E38);
    //u8Temp|=0x20;
    //HAL_DMD_RIU_WriteByte(0x101E38, u8Temp);

    // SRAM allocation 64K  avoid change souce from T2 failed.
    HAL_DMD_RIU_WriteByte(0x111701,0x00);
    HAL_DMD_RIU_WriteByte(0x111700,0x00);

    HAL_DMD_RIU_WriteByte(0x111705,0x00);
    HAL_DMD_RIU_WriteByte(0x111704,0x00);

    HAL_DMD_RIU_WriteByte(0x111703,0xff);
    HAL_DMD_RIU_WriteByte(0x111702,0xff);

    HAL_DMD_RIU_WriteByte(0x111707,0xff);
    HAL_DMD_RIU_WriteByte(0x111706,0xff);

    //Diff from TV tool
    HAL_DMD_RIU_WriteByte(0x111708,0x01);
    HAL_DMD_RIU_WriteByte(0x111709,0x00);

    HAL_DMD_RIU_WriteByte(0x11170a,0x0f);
    HAL_DMD_RIU_WriteByte(0x11170b,0x00);

    HAL_DMD_RIU_WriteByte(0x111718,0x02);
    HAL_DMD_RIU_WriteByte(0x111719,0x00);

    HAL_DMD_RIU_WriteByte(0x11171a,0x00);
    HAL_DMD_RIU_WriteByte(0x11171b,0x00);

    HAL_DMD_RIU_WriteByte(0x1117e0,0x14);
    HAL_DMD_RIU_WriteByte(0x1117e1,0x14);

    HAL_DMD_RIU_WriteByte(0x1117e4,0x00);
    HAL_DMD_RIU_WriteByte(0x1117e5,0x00);

    HAL_DMD_RIU_WriteByte(0x1117e6,0x00);
    HAL_DMD_RIU_WriteByte(0x1117e7,0x00);

   // SRAM End Address
   HAL_DMD_RIU_WriteByte(0x111707,0xff);
   HAL_DMD_RIU_WriteByte(0x111706,0xff);

   // DRAM Disable
   HAL_DMD_RIU_WriteByte(0x111718,HAL_DMD_RIU_ReadByte(0x111718)&(~0x04));
	
    DBG_INTERN_DVBS(printf("INTERN_DVBS_InitClkgen\n"));
}

/***********************************************************************************
  Subject:    Power on initialized function
  Function:   INTERN_DVBS_Power_On_Initialization
  Parmeter:
  Return:     MS_BOOL
  Remark:
************************************************************************************/
MS_BOOL INTERN_DVBS_Power_On_Initialization(MS_BOOL bRFAGCTristateEnable, MS_U8 u8ADCIQMode, MS_U8 u8PadSel, MS_BOOL bPGAEnable, MS_U8 u8PGAGain, const MS_U8 *u8DMD_DVBS_DSPRegInitExt, MS_U8 u8DMD_DVBS_DSPRegInitSize)
{
    MS_U8       status = true;
    //MS_U8        u8ChipVersion;

    DBG_INTERN_DVBS(printf("INTERN_DVBS_Power_On_Initialization\n"));

#if defined(PWS_ENABLE)
    Mapi_PWS_Stop_VDMCU();
#endif
    INTERN_DVBS_InitClkgen(bRFAGCTristateEnable);//~~ no modify
    HAL_DMD_ADC_IQ_Switch(u8ADCIQMode, u8PadSel, bPGAEnable, u8PGAGain);//~~ no modify

    DBG_INTERN_DVBS(printf("@@@@@INTERN_DVBS_Power_On_Initialization1:bRFAGCTristateEnable=%d ;u8ADCIQMode=%d \n",bRFAGCTristateEnable,u8ADCIQMode));
    DBG_INTERN_DVBS(printf("@@@@@INTERN_DVBS_Power_On_Initialization2:u8PadSel=%d ;bPGAEnable=%d \n",u8PadSel,bPGAEnable));
    DBG_INTERN_DVBS(printf("@@@@@INTERN_DVBS_Power_On_Initialization2:u8PGAGain=%d \n",u8PGAGain));

    //// Firmware download //////////
    DBG_INTERN_DVBS(printf("INTERN_DVBS Load DSP...\n"));
    //MsOS_DelayTask(100);

    {
        if (INTERN_DVBS_LoadDSPCode() == FALSE)
        {
            DBG_INTERN_DVBS(printf("DVB-S Load DSP Code Fail\n"));
            return FALSE;
        }
        else
        {
            DBG_INTERN_DVBS(printf("DVB-S Load DSP Code OK\n"));
        }
    }

    //// MCU Reset //////////
    if (INTERN_DVBS_Reset() == FALSE)
    {
        DBG_INTERN_DVBS(printf("INTERN_DVBS Reset...Fail\n"));
        return FALSE;
    }
    else
    {
        DBG_INTERN_DVBS(printf("INTERN_DVBS Reset...OK\n"));
    }

    
    status &= INTERN_DVBS_DSPReg_Init(u8DMD_DVBS_DSPRegInitExt, u8DMD_DVBS_DSPRegInitSize);
    //status &= INTERN_DVBS_Active(ENABLE);//enable this

    //Read Demod FW Version.
    INTERN_DVBS_Show_Demod_Version();

    return status;
}
/************************************************************************************************
  Subject:    Driving control
  Function:   INTERN_DVBC_Driving_Control
  Parmeter:   bInversionEnable : TRUE For High
  Return:      void
  Remark:
*************************************************************************************************/
void INTERN_DVBS_Driving_Control(MS_BOOL bEnable)
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

    DBG_INTERN_DVBS(printf("---> INTERN_DVBS_Driving_Control(Bit0) = 0x%x \n",u8Temp));
    HAL_DMD_RIU_WriteByte(0x101E10, u8Temp);
}

/************************************************************************************************
  Subject:    Clk Inversion control
  Function:   INTERN_DVBS_Clk_Inversion_Control
  Parmeter:   bInversionEnable : TRUE For Inversion Action
  Return:      void
  Remark:
*************************************************************************************************/
void INTERN_DVBS_Clk_Inversion_Control(MS_BOOL bInversionEnable)
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

    DBG_INTERN_DVBS(printf("---> Inversion(Bit9) = 0x%x \n",u8Temp));
    HAL_DMD_RIU_WriteByte(0x103301, u8Temp);
}

/************************************************************************************************
  Subject:    Transport stream serial/parallel control
  Function:   INTERN_DVBS_Serial_Control
  Parmeter:   bEnable : TRUE For serial
  Return:     MS_BOOL :
  Remark:
*************************************************************************************************/
MS_BOOL INTERN_DVBS_Serial_Control(MS_BOOL bEnable, MS_U8 u8TSClk)
{
    MS_U8   status = true;
    MS_U8   temp_val;
    DBG_INTERN_DVBS(printf(" @INTERN_DVBS_ts... u8TSClk=%d\n", u8TSClk));

    if (u8TSClk == 0xFF) u8TSClk=0x13;
    if (bEnable)    //Serial mode for TS pad
    {
        // serial
        HAL_DMD_RIU_WriteByte(0x103308, 0x01);   // serial mode: 0x0401
        HAL_DMD_RIU_WriteByte(0x103309, 0x04);   // reg_ckg_dvbtc_ts@0x04

        HAL_DMD_RIU_WriteByte(0x103300, 0x00);   // serial mode 0x0400
#if(INTERN_DVBS_TS_SERIAL_INVERSION == 0)
        //HAL_DMD_RIU_WriteByte(0x103301, 0x04);   // reg_ckg_dvbtmk_ts_out_mode@0x00
        temp_val=HAL_DMD_RIU_ReadByte(0x103301);
        temp_val|=0x04;
        HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#else
        // HAL_DMD_RIU_WriteByte(0x103301, 0x07);   // reg_ckg_dvbtmk_ts_out_mode@0x00
        temp_val=HAL_DMD_RIU_ReadByte(0x103301);
        temp_val|=0x07;
        HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#endif
        //move to drvSYS HAL_DMD_RIU_WriteByte(0x101EA5, HAL_DMD_RIU_ReadByte(0x101EA5)&0xEF);   // PAD_TS1 is used as output
        //move to drvSYS HAL_DMD_RIU_WriteByte(0x101EAF, HAL_DMD_RIU_ReadByte(0x101EAF)&0xE3);   // PAD_TS1 Disable TS CLK PAD

        //// INTERN_DVBS TS Control: Serial //////////

        //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_TS_SERIAL, TS_SERIAL);

#if(INTERN_DVBS_TS_SERIAL_INVERSION == 0)
        //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_TS_OUT_INV, 0);
#else
        //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_TS_OUT_INV, 1);
#endif
        gsCmdPacketDVBS.cmd_code = CMD_TS_CTRL;

        gsCmdPacketDVBS.param[0] = TS_SERIAL;
#if(INTERN_DVBS_TS_SERIAL_INVERSION == 0)
        gsCmdPacketDVBS.param[1] = 0;//TS_CLK_NO_INV;
#else
        gsCmdPacketDVBS.param[1] = 1;//TS_CLK_INVERSE;
#endif
        status &= INTERN_DVBS_Cmd_Packet_Send(&gsCmdPacketDVBS, 2);
    }
    else
    {
        //parallel
        HAL_DMD_RIU_WriteByte(0x103308, 0x01);   // parallel mode:0x0001
        HAL_DMD_RIU_WriteByte(0x103309, 0x00);   // reg_ckg_dvbtc_ts@0x04

        //HAL_DMD_RIU_WriteByte(0x103300, 0x11);    // parallel mode: 0x0511 => ts_clk=288/(2*(0x11+1))=8MHz
        HAL_DMD_RIU_WriteByte(0x103300, u8TSClk);   // parallel mode: 0x0513 => ts_clk=288/(2*(0x16+1))=6.26MHz //@@++--
#if(INTERN_DVBS_TS_PARALLEL_INVERSION == 0)
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

        //move to drvSYS HAL_DMD_RIU_WriteByte(0x101EA5, HAL_DMD_RIU_ReadByte(0x101EA5)|0x10);          // PAD_TS1 is used as output
        //move to drvSYS HAL_DMD_RIU_WriteByte(0x101EAF, (HAL_DMD_RIU_ReadByte(0x101EAF)&0xC0)|0x11);   // PAD_TS1 enable TS clk pad

        //// INTERN_DVBS TS Control: Parallel //////////

        //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_TS_SERIAL, TS_PARALLEL);

#if(INTERN_DVBS_TS_SERIAL_INVERSION == 0)
        //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_TS_OUT_INV, 0);
#else
        //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_TS_OUT_INV, 1);
#endif
        //// INTERN_DVBC TS Control: Parallel //////////
        gsCmdPacketDVBS.cmd_code = CMD_TS_CTRL;

        gsCmdPacketDVBS.param[0] = TS_PARALLEL;
#if(INTERN_DVBS_TS_PARALLEL_INVERSION == 0)
        gsCmdPacketDVBS.param[1] = 0;//TS_CLK_NO_INV;
#else
        gsCmdPacketDVBS.param[1] = 1;//TS_CLK_INVERSE;
#endif
        status &= INTERN_DVBS_Cmd_Packet_Send(&gsCmdPacketDVBS, 2);
    }

#if(INTERN_DVBS_TS_SERIAL_INVERSION == 0)
    DBG_INTERN_DVBS(printf("---> Inversion(Bit5) = %d \n",0 ));
#else
    DBG_INTERN_DVBS(printf("---> Inversion(Bit5) = %d \n",1 ));
#endif

    INTERN_DVBS_Driving_Control(INTERN_DVBS_DTV_DRIVING_LEVEL);
    return status;
}

/************************************************************************************************
  Subject:    TS1 output control
  Function:   INTERN_DVBS_PAD_TS1_Enable
  Parmeter:   flag : TRUE For Turn on TS1, FALSE For Turn off TS1
  Return:     void
  Remark:
*************************************************************************************************/
void INTERN_DVBS_PAD_TS1_Enable(MS_BOOL flag)
{
    DBG_INTERN_DVBS(printf(" @INTERN_DVBS_TS1_Enable... \n"));

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
  Function:   INTERN_DVBC_Config
  Parmeter:   BW: bandwidth
  Return:     MS_BOOL :
  Remark:
*************************************************************************************************/
MS_BOOL INTERN_DVBS_Config(MS_U32 u32SymbolRate, DMD_DVBS_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U8 u8TSClk, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num)
{

    MS_BOOL         status= true;
    MS_U16          u16CenterFreq;
    // MS_U16       u16Fc = 0;
    MS_U8             temp_val;
    MS_U8           u8Data =0;
    MS_U8           u8counter = 0;
    MS_U32          u32CurrentSR;

    u32CurrentSR = u32SymbolRate/1000;  //KHz
    //u32SymbolRate= u32SymbolRate/1000;//to match fw format
    u16CenterFreq  =u32IFFreq;
    DBG_INTERN_DVBS(printf(" @INTERN_DVBS_config+, SR=%d, QAM=%d, u32IFFreq=%d, bSpecInv=%d, bSerialTS=%d, u8TSClk=%d\n", u32CurrentSR, eQamMode, u32IFFreq, bSpecInv, bSerialTS, u8TSClk));
    DBG_INTERN_DVBS(printf("INTERN_DVBS_Config, t = %d\n",MsOS_GetSystemTime()));

    u8TSClk=0x05;//if (u8TSClk == 0xFF) u8TSClk=0x13;//set a fater speed for initial condition
    status &= INTERN_DVBS_Reset();

    u8DemodLockFlag=0;

    // Symbol Rate
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_L, u32CurrentSR&0xff);
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_H, (u32CurrentSR>>8)&0xff);
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_DMDTOP_DBG_5, (u32CurrentSR>>16)&0xff);
    DBG_INTERN_DVBS(printf(">>>INTERN_DVBS_Config u32SymbolRate %d<<<\n", u32CurrentSR));

#if 0    
    //========  check SR is right or not ===========
    MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_L, &u8Data);
    DBG_INTERN_DVBS(printf(">>>Mailbox read: E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_L addr=%d    mailbox value=%d<<<\n", E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_L,u8Data));
    u32SR =u8Data;
    MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_H, &u8Data);
    DBG_INTERN_DVBS(printf(">>>Mailbox read: E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_H addr=%d    mailbox value=%d<<<\n", E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_H,u8Data));
    u32SR =((U32)u8Data<<8)|u32SR  ;
    MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_DBG_5, &u8Data);
    DBG_INTERN_DVBS(printf(">>>Mailbox read: E_DMD_S2_MB_DMDTOP_DBG_5 addr=%d    mailbox value=%d<<<\n", E_DMD_S2_MB_DMDTOP_DBG_5,u8Data));
    u32SR =((U32)u8Data<<16)|u32SR;
    //=================================================
#endif

    // IQ Swap ,ADCPLL IQ swap / DMDANA_IQ_SWAP
    //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_IQ_SWAP, bSpecInv? 0x01:0x00);
    if(bSpecInv)
    {
        status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DMDANA_REG_BASE+0xC0, &u8Data);//Demod\reg_dmdana.xls
        u8Data|=(0x02);
        status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DMDANA_REG_BASE+0xC0, u8Data);
    }

    // TS mode
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_TS_SERIAL, bSerialTS? 0x01:0x00); 
    DBG_INTERN_DVBS(printf(">>>INTERN_DVBS_Config TS Serial %d<<<\n", bSerialTS));
    _bSerialTS = bSerialTS;

    if (bSerialTS)
    {
        // serial
        HAL_DMD_RIU_WriteByte(0x103308, 0x01);   // parallel mode:0x0001 / serial mode: 0x0401
        HAL_DMD_RIU_WriteByte(0x103309, 0x04);   // reg_ckg_dvbtc_ts@0x04

        HAL_DMD_RIU_WriteByte(0x103300, 0x00);   // parallel mode: 0x0511 /serial mode 0x0400
#if(INTERN_DVBS_TS_SERIAL_INVERSION == 0)
        temp_val=HAL_DMD_RIU_ReadByte(0x103301);
        temp_val|=0x04;
        HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#else
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
#if(INTERN_DVBS_TS_PARALLEL_INVERSION == 0)
        temp_val=HAL_DMD_RIU_ReadByte(0x103301);
        temp_val|=0x05;
        HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#else
        temp_val=HAL_DMD_RIU_ReadByte(0x103301);
        temp_val|=0x07;
        HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#endif
    }
#if (INTERN_DVBS_INTERNAL_DEBUG == 1)
    INTERN_DVBS_Show_Demod_Version();
#endif

    //-----------------------------------------------------------
    //From INTERN_DVBS_Demod_Restart function.

    //FW sw reset
    //[0]: 0: SW Reset, 1: Start state machine
    //[1]: 1: Blind scan enable, 0: manual scan
    //[2]: 1: Code flow track enable
    //[3]: 1: go to AGC state
    //[4]: 1: set DiSEqC
    MDrv_SYS_DMD_VD_MBX_ReadReg((TOP_REG_BASE + 0x60*2), &u8Data);
    u8Data = (u8Data&0xF0)|0x01;
    MDrv_SYS_DMD_VD_MBX_WriteReg((TOP_REG_BASE + 0x60*2), u8Data);
    //DBG_INTERN_DVBS(printf(">>>REG write check: addr=%d    value=%d<<<\n",(TOP_REG_BASE + 0x60*2), u8Data));
    MDrv_SYS_DMD_VD_MBX_ReadReg((TOP_REG_BASE + 0x60*2), &u8Data);
    //DBG_INTERN_DVBS(printf(">>>REG read check: addr=%d    value=%d<<<\n",(TOP_REG_BASE + 0x60*2), u8Data));

    u8counter = 20;
    while( ((u8Data&0x01) == 0x00) && (u8counter != 0) )
    {
        MsOS_DelayTask(1);
        printf("TOP_WR_DBG_90=0x%x, status=%d, u8counter=%d\n", u8Data, status, u8counter);
        u8Data|=0x01;
        MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE + 0x60*2, u8Data);
        MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0x60*2, &u8Data);
        DBG_INTERN_DVBS(printf(">>>(while)REG read check: addr=%d    value=%d<<<\n",(TOP_REG_BASE + 0x60*2), u8Data));
        u8counter--;
    }

    if((u8Data & 0x01)==0x00)
    {
        status = FALSE;
    }

    DBG_INTERN_DVBS(printf("INTERN_DVBS_config done\n"));
    return status;
}
/************************************************************************************************
  Subject:    channel change config
  Function:   INTERN_DVBS_Blind_Scan_Config
  Parmeter:   BW: bandwidth
  Return:     MS_BOOL :
  Remark:
*************************************************************************************************/
MS_BOOL INTERN_DVBS_BlindScan_Config(MS_U32 u32SymbolRate, DMD_DVBS_MODULATION_TYPE eQamMode, MS_U32 u32IFFreq, MS_BOOL bSpecInv, MS_BOOL bSerialTS, MS_U8 u8TSClk, MS_U16 *pu16_symbol_rate_list,MS_U8 u8_symbol_rate_list_num)
{

    MS_BOOL         status= true;
    MS_U16          u16CenterFreq;
    // MS_U16       u16Fc = 0;
    MS_U8             temp_val;
    MS_U8           u8Data=0;
    MS_U16           u16WaitCount = 0;
    MS_U32          u32CurrentSR;

    u32CurrentSR = u32SymbolRate/1000;  //KHz
    //u32SymbolRate= u32SymbolRate/1000;//to match fw format
    u16CenterFreq  =u32IFFreq;
    DBG_INTERN_DVBS(printf(" @INTERN_DVBS_blindScan_Config+, SR=%d, QAM=%d, u32IFFreq=%d, bSpecInv=%d, bSerialTS=%d, u8TSClk=%d\n", u32CurrentSR, eQamMode, u32IFFreq, bSpecInv, bSerialTS, u8TSClk));
    DBG_INTERN_DVBS(printf("INTERN_DVBS_blindScan_Config, t = %d\n",MsOS_GetSystemTime()));

    //status &= INTERN_DVBS_Reset();
    g_dvbs_lock = 0;
    u8DemodLockFlag=0;

    // Symbol Rate
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_L, u32CurrentSR&0xff);
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_H, (u32CurrentSR>>8)&0xff);
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_DMDTOP_DBG_5, (u32CurrentSR>>16)&0xff);
    DBG_INTERN_DVBS(printf(">>>INTERN_DVBS_Config u32SymbolRate %d<<<\n", u32CurrentSR));

#if 0    
    //========  check SR is right or not ===========
    MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_L, &u8Data);
    DBG_INTERN_DVBS(printf(">>>Mailbox read: E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_L addr=%d    mailbox value=%d<<<\n", E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_L,u8Data));
    u32SR =u8Data;
    MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_H, &u8Data);
    DBG_INTERN_DVBS(printf(">>>Mailbox read: E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_H addr=%d    mailbox value=%d<<<\n", E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_H,u8Data));
    u32SR =((U32)u8Data<<8)|u32SR  ;
    MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_DBG_5, &u8Data);
    DBG_INTERN_DVBS(printf(">>>Mailbox read: E_DMD_S2_MB_DMDTOP_DBG_5 addr=%d    mailbox value=%d<<<\n", E_DMD_S2_MB_DMDTOP_DBG_5,u8Data));
    u32SR =((U32)u8Data<<16)|u32SR;
    //=================================================
#endif

    // IQ Swap ,ADCPLL IQ swap / DMDANA_IQ_SWAP
    //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_IQ_SWAP, bSpecInv? 0x01:0x00);
    if(bSpecInv)
    {
        status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DMDANA_REG_BASE+0xC0, &u8Data);//Demod\reg_dmdana.xls
        u8Data|=(0x02);
        status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DMDANA_REG_BASE+0xC0, u8Data);
    }

    // TS mode
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_TS_SERIAL, bSerialTS? 0x01:0x00);
    DBG_INTERN_DVBS(printf(">>>INTERN_DVBS_Config TS Serial %d<<<\n", bSerialTS));
    _bSerialTS = bSerialTS;
    u8TSClk=0x05;//if (u8TSClk == 0xFF) u8TSClk=0x13;//set a fater speed for initial condition

    if (bSerialTS)
    {
        // serial
        HAL_DMD_RIU_WriteByte(0x103308, 0x01);   // parallel mode:0x0001 / serial mode: 0x0401
        HAL_DMD_RIU_WriteByte(0x103309, 0x04);   // reg_ckg_dvbtc_ts@0x04

        HAL_DMD_RIU_WriteByte(0x103300, 0x00);   // parallel mode: 0x0511 /serial mode 0x0400
#if(INTERN_DVBS_TS_SERIAL_INVERSION == 0)
        temp_val=HAL_DMD_RIU_ReadByte(0x103301);
        temp_val|=0x04;
        HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#else
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
#if(INTERN_DVBS_TS_PARALLEL_INVERSION == 0)
        temp_val=HAL_DMD_RIU_ReadByte(0x103301);
        temp_val|=0x05;
        HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#else
        temp_val=HAL_DMD_RIU_ReadByte(0x103301);
        temp_val|=0x07;
        HAL_DMD_RIU_WriteByte(0x103301,temp_val);
#endif
    }
#if (INTERN_DVBS_INTERNAL_DEBUG == 1)
    INTERN_DVBS_Show_Demod_Version();
#endif

    //-----------------------------------------------------------
    //From INTERN_DVBS_Demod_Restart function.
    
    //enable send DiSEqC
    //[0]: 0: SW Reset, 1: Start state machine
    //[1]: 1: Blind scan enable, 0: manual scan
    //[2]: 1: Code flow track enable
    //[3]: 1: go to AGC state
    //[4]: 1: set DiSEqC
    MDrv_SYS_DMD_VD_MBX_ReadReg((TOP_REG_BASE + 0x60*2), &u8Data);
    u8Data |= 0x08;
    MDrv_SYS_DMD_VD_MBX_WriteReg((TOP_REG_BASE + 0x60*2), u8Data);

    u16WaitCount=0;
    do
    {
        status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_STATE_FLAG, &u8Data);//SubState
        u16WaitCount++;
        //DBG_INTERN_DVBS(printf("MDrv_Demod_BlindScan_NextFreq u8Data:0x%x u16WaitCount:%d\n", u8Data, u16WaitCount));
        MsOS_DelayTask(1);
    }while((u8Data!=0x01)&&(u16WaitCount<INTERN_DVBS_DEMOD_WAIT_TIMEOUT));

    // disable blind scan
    MDrv_SYS_DMD_VD_MBX_ReadReg((TOP_REG_BASE + 0x60*2), &u8Data);
    u8Data&=~(0x02);
    MDrv_SYS_DMD_VD_MBX_WriteReg((TOP_REG_BASE + 0x60*2), u8Data);

    //disble send DiSEqC
    MDrv_SYS_DMD_VD_MBX_ReadReg((TOP_REG_BASE + 0x60*2), &u8Data);
    u8Data&=~(0x08);
    MDrv_SYS_DMD_VD_MBX_WriteReg((TOP_REG_BASE + 0x60*2), u8Data);


    DBG_INTERN_DVBS(printf("INTERN_DVBS_blindScan_Config done\n"));
    return status;
}

void INTERN_DVBS_Power_ON_OFF(MS_U8 bPowerOn)
{
    bPowerOn = bPowerOn;
}

MS_BOOL INTERN_DVBS_Power_Save(void)
{
    return TRUE;
}
//------------------------------------------------------------------
//  END System Info Function
//------------------------------------------------------------------

//------------------------------------------------------------------
//  Get And Show Info Function
//------------------------------------------------------------------
/************************************************************************************************
  Subject:    enable hw to lock channel
  Function:   INTERN_DVBS_Active
  Parmeter:   bEnable
  Return:     MS_BOOL
  Remark:
*************************************************************************************************/
MS_BOOL INTERN_DVBS_Active(MS_BOOL bEnable)
{
    MS_U8   status = TRUE;
    //MS_U8 u8Data;

    DBG_INTERN_DVBS(printf(" @INTERN_DVBS_Active\n"));

    //// INTERN_DVBS Finite State Machine on/off //////////
#if 0
    gsCmdPacketDVBC.cmd_code = CMD_FSM_CTRL;

    gsCmdPacketDVBS.param[0] = (MS_U8)bEnable;
    status &= INTERN_DVBS_Cmd_Packet_Send(&gsCmdPacketDVBS, 1);
#else

    HAL_DMD_RIU_WriteByte(MBRegBase + (0x0e)*2, 0x01); // FSM_EN
#endif

    bDMD_DVBS_NoChannelDetectedWithRFPower = FALSE;
    u32DMD_DVBS_NoChannelTimeAccWithRFPower = 0;
    return status;
}

MS_BOOL INTERN_DVBS_GetTsDivNum(MS_FLOAT* fTSDivNum)
{
    MS_U8 u8Data = 0;
    MS_BOOL     status = true;
    MS_U32      u32SymbolRate=0;
    //float       fSymbolRate;
    //MS_U8 ISSY_EN = 0;
    MS_U8 code_rate_idx = 0;
    MS_U8 pilot_flag = 0;
    MS_U8 fec_type_idx = 0;
    MS_U8 mod_type_idx = 0;
    MS_U16 k_bch_array[2][11] ={
                {16008, 21408, 25728, 32208, 38688, 43040, 48408, 51648, 53840, 57472, 58192},
                { 3072,  5232,  6312,  7032,  9552, 10632, 11712, 12432, 13152, 14232,     0}};
    MS_U16 n_ldpc_array[2] = {64800, 16200};
    MS_FLOAT pilot_term = 0;
    MS_FLOAT k_bch;    
    MS_FLOAT n_ldpc;
    MS_FLOAT ts_div_num_offset = 2.0;
    //MS_U32 u32Time_start,u32Time_end;
    //MS_U32 u32temp;
    //MS_FLOAT pkt_interval;
    //MS_U8 time_counter=0;

    INTERN_DVBS_GetCurrentSymbolRate(&u32SymbolRate);
    //fSymbolRate=u32SymbolRate+0.0;///1000.0;//Symbol Rate(KHz)
    DBG_INTERN_DVBS(printf("INTERN_DVBS_GetTsDivNum u32SymbolRate=%d\n", u32SymbolRate));
//   DMD_DVBS_MODULATION_TYPE pQAMMode;

    MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_SYSTEM_TYPE, &u8Data);//V
    DBG_INTERN_DVBS(printf("INTERN_DVBS_GetTsDivNum GetCurrentDemodType E_DMD_S2_SYSTEM_TYPE=%d\n", u8Data));//u8Data:0 is S2;  1 is DVBS
    
    if(!u8Data)//DVBS2
    {
#if 0
        //Get DVBS2 Code Rate
        MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_CODE_RATE, &u8Data);//V
        printf("[S2]INTERN_DVBS_GetTsDivNum DVBS2 E_DMD_S2_CODERATE=0x%x\n", u8Data);
        switch (u8Data)
        {
            case 0x03: //CR 1/2
                  k_bch=32208.0;
                  _u8_DVBS2_CurrentCodeRate = 5;
                  DBG_INTERN_DVBS(printf("INTERN_DVBS_GetTsDivNum INTERN_DVBS2_GetCurrentCodeRate=1_2\n"));
               break;
            case 0x01: //CR 1/3
                  k_bch=21408.0; //8PSK???
                  _u8_DVBS2_CurrentCodeRate = 6;
                  DBG_INTERN_DVBS(printf("INTERN_DVBS_GetTsDivNum INTERN_DVBS2_GetCurrentCodeRate=1_3\n"));
               break;
            case 0x05: //CR 2/3
                  k_bch=43040.0;
                  _u8_DVBS2_CurrentCodeRate = 7;
                  DBG_INTERN_DVBS(printf("INTERN_DVBS_GetTsDivNum INTERN_DVBS2_GetCurrentCodeRate=2_3\n"));
               break;
            case 0x00: //CR 1/4
                  k_bch=16008.0; //8PSK???
                  _u8_DVBS2_CurrentCodeRate = 8;
                  DBG_INTERN_DVBS(printf("INTERN_DVBS_GetTsDivNum INTERN_DVBS2_GetCurrentCodeRate=1_4\n"));
               break;
            case 0x06: //CR 3/4
                  k_bch=48408.0;
                  _u8_DVBS2_CurrentCodeRate = 9;
                  DBG_INTERN_DVBS(printf("INTERN_DVBS_GetTsDivNum INTERN_DVBS2_GetCurrentCodeRate=3_4\n"));
               break;
            case 0x02: //CR 2/5
                  k_bch=25728.0; //8PSK???
                  _u8_DVBS2_CurrentCodeRate = 10;
                  DBG_INTERN_DVBS(printf("INTERN_DVBS_GetTsDivNum INTERN_DVBS2_GetCurrentCodeRate=2_5\n"));
               break;
            case 0x04: //CR 3/5
                  k_bch=38688.0;
                  _u8_DVBS2_CurrentCodeRate = 11;
                  DBG_INTERN_DVBS(printf("INTERN_DVBS_GetTsDivNum INTERN_DVBS2_GetCurrentCodeRate=3_5\n"));
               break;
            case 0x07: //CR 4/5
                  k_bch=51648.0;
                  _u8_DVBS2_CurrentCodeRate = 12;
                  DBG_INTERN_DVBS(printf("INTERN_DVBS_GetTsDivNum INTERN_DVBS2_GetCurrentCodeRate=4_5\n"));
               break;
            case 0x08: //CR 5/6
                  k_bch=53840.0;
                  _u8_DVBS2_CurrentCodeRate = 13;
                  DBG_INTERN_DVBS(printf("INTERN_DVBS_GetTsDivNum INTERN_DVBS2_GetCurrentCodeRate=5_6\n"));
               break;
            case 0x09: //CR 8/9
                  k_bch=57472.0;
                  _u8_DVBS2_CurrentCodeRate = 14;
                  DBG_INTERN_DVBS(printf("INTERN_DVBS_GetTsDivNum INTERN_DVBS2_GetCurrentCodeRate=8_9\n"));
               break;
            case 0x0A: //CR 9/10
                  k_bch=58192.0;
                  _u8_DVBS2_CurrentCodeRate = 15;
                  DBG_INTERN_DVBS(printf("INTERN_DVBS_GetTsDivNum INTERN_DVBS2_GetCurrentCodeRate=9_10\n"));
               break;
            default:
                  k_bch=58192.0;
                  _u8_DVBS2_CurrentCodeRate = 15;
                  DBG_INTERN_DVBS(printf("INTERN_DVBS_GetTsDivNum INTERN_DVBS2_GetCurrentCodeRate= default 9_10\n"));
               break;
        }   //printf("INTERN_DVBS_GetTsDivNum k_bch=%ld\n", (MS_U32)k_bch);
#endif
        //INTERN_DVBS_GetCurrentModulationType(&pQAMMode);  //V
        //printf("INTERN_DVBS_GetTsDivNum Mod_order=%d\n", modulation_order);

        // pilot_flag     =>   0 : off    1 : on
        // fec_type_idx   =>   0 : normal 1 : short
        // mod_type_idx   =>   0 : QPSK   1 : 8PSK   2 : 16APSK   3 : 32APSK
        // code_rate_idx  =>   d0: 1/4, d1: 1/3, d2: 2/5, d3: 1/2, d4: 3/5, d5: 2/3, d6: 3/4, d7: 4/5, d8: 5/6, d9: 8/9, d10: 9/10
        //set TS clock rate
        MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_CODE_RATE, &code_rate_idx);
        MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_FEC_TYPE, &fec_type_idx);
        MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MOD_TYPE, &mod_type_idx);
        modulation_order = mod_type_idx;
        MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_PILOT_FLAG, &pilot_flag);
        //MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_PILOT_FLAG, &u8Data);
        
        //MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_ISSY_ACTIVE, &ISSY_EN);        
        //if(ISSY_EN==0)
        //{
        k_bch = k_bch_array[fec_type_idx][code_rate_idx];
        n_ldpc = n_ldpc_array[fec_type_idx];
        pilot_term = ((float) n_ldpc / modulation_order / 1440 * 36) * pilot_flag;
        if(_bSerialTS)//serial mode
        {
            *fTSDivNum =288000.0/(k_bch/((n_ldpc/modulation_order+90+pilot_term)/u32SymbolRate));
            *fTSDivNum = *fTSDivNum/2 -1;
        }
        else//parallel mode
        {
            *fTSDivNum = 288000.0/(k_bch/((n_ldpc/modulation_order+90+pilot_term)/u32SymbolRate)/8);
            *fTSDivNum = *fTSDivNum/2 -1;
        }
        *fTSDivNum-=ts_div_num_offset;
        //}
#if 0
        else if(ISSY_EN==1)//ISSY = 1
        {
               //u32Time_start = msAPI_Timer_GetTime0();
               time_counter=0;
            do
            {
                 MDrv_SYS_DMD_VD_MBX_ReadReg((0x3E00 + 0x4D*2), &u8Data);//DVBS2OPPRO_ISCR_CAL_DONE     (_REG_DVBS2OPPRO(0x4D)+0)
                 u8Data &= 0x01;
                // u32Time_end =msAPI_Timer_GetTime0();
                MsOS_DelayTask(1);
                time_counter = time_counter +1;
            }while( (u8Data!=0x01) && ( (time_counter )< 50) );

            //read pkt interval
            MDrv_SYS_DMD_VD_MBX_ReadReg((0x3E00 + 0x70*2), &u8Data);
            u32temp = u8Data;
            MDrv_SYS_DMD_VD_MBX_ReadReg((DVBS2OPPRO_REG_BASE + 0x70*2+1), &u8Data);
            u32temp |= (MS_U32)u8Data<<8;
            MDrv_SYS_DMD_VD_MBX_ReadReg((DVBS2OPPRO_REG_BASE + 0x71*2), &u8Data);
            u32temp |= (MS_U32)u8Data<<16;
            MDrv_SYS_DMD_VD_MBX_ReadReg((DVBS2OPPRO_REG_BASE + 0x71*2+1), &u8Data);
            u32temp |= (MS_U32)u8Data<<24;
            pkt_interval = (MS_FLOAT) u32temp / 1024.0;
            if(_bSerialTS)//serial mode
            {
                 *fTSDivNum=288000.0 / (188*8*u32SymbolRate/pkt_interval) - ts_div_num_offset;
                 *fTSDivNum = (*fTSDivNum-1)/2;
            }
            else
            {
                 *fTSDivNum=288000.0 / (188*u32SymbolRate/pkt_interval) - ts_div_num_offset;
                *fTSDivNum = (*fTSDivNum-1)/2;
            }

        }

        else
        {
            *fTSDivNum =0x0A;
        }

        if(*fTSDivNum>255)
            *fTSDivNum=255;
        if(*fTSDivNum<1)
            *fTSDivNum=1;

       //printf("INTERN_DVBS_GetTsDivNum Pilot E_DMD_S2_MB_DMDTOP_DBG_9=%d\n", u8Data);
       /*if(u8Data) // Pilot ON
             printf(">>>INTERN_DVBS_GetTsDivNum Pilot ON<<<\n");
         else //Pilot off
             printf(">>>INTERN_DVBS_GetTsDivNum Pilot off<<<\n");
         */
       if(_bSerialTS)
       {
          if(u8Data)//if pilot ON
          {
            if(modulation_order==2)
               *u8TSDivNum = (MS_U8)(144000/(k_bch/((64800/modulation_order+90+36*22)/u32SymbolRate)) - 3);
            else if(modulation_order==3)
               *u8TSDivNum = (MS_U8)(144000/(k_bch/((64800/modulation_order+90+36*15)/u32SymbolRate)) - 3);
          }
          else
            *u8TSDivNum = (MS_U8)(144000/(k_bch/((64800/modulation_order+90)/u32SymbolRate)) - 3);
        }
        else//Parallel mode
        {
            if(u8Data)
            {
               if(modulation_order==2)
                  *u8TSDivNum = (MS_U8)(float)(144000/(k_bch/((64800/modulation_order+90.0+36*22)/u32SymbolRate)/8.0) - 3);
               else if(modulation_order==3)
                  *u8TSDivNum = (MS_U8)(float)(144000/(k_bch/((64800/modulation_order+90.0+36*15)/u32SymbolRate)/8.0) - 3);
            }
            else
               *u8TSDivNum = (MS_U8)(float)(144000/(k_bch/((64800/modulation_order+90.0)/u32SymbolRate)/8.0) - 3);
        }
#endif
    }
    else                                            //S
    {
        //status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSTFEC_REG_BASE + 0x3A*2, &u8Data);
        //u8_gCodeRate = (u8Data & 0x70)>>4;
        //DVBS Code Rate
        //switch (u8_gCodeRate)
        MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_CODE_RATE, &u8Data);
        switch (u8Data)
        {
            case 0x00: //CR 1/2
                _u8_DVBS2_CurrentCodeRate = 0;
                DBG_INTERN_DVBS(printf("INTERN_DVBS_GetTsDivNum INTERN_DVBS_GetCurrentCodeRate=1_2\n"));
                if(_bSerialTS)
                    *fTSDivNum = (288000.0/((1.0/2.0)*(188.0/204.0)*u32SymbolRate*2));
                else
                    *fTSDivNum = (288000.0/((1.0/2.0)*(188.0/204.0)*u32SymbolRate*2/8.0));

                *fTSDivNum = *fTSDivNum/2-1-5;
                break;
            case 0x01: //CR 2/3
                _u8_DVBS2_CurrentCodeRate = 1;
                DBG_INTERN_DVBS(printf("INTERN_DVBS_GetTsDivNum INTERN_DVBS_GetCurrentCodeRate=2_3\n"));
                if(_bSerialTS)
                    *fTSDivNum = (MS_U8)(288000.0/((2.0/3.0)*(188.0/204.0)*u32SymbolRate*2));
                else
                    *fTSDivNum = (MS_U8)(288000.0/((2.0/3.0)*(188.0/204.0)*u32SymbolRate*2/8.0));

                *fTSDivNum = *fTSDivNum/2-1-5;
                break;
            case 0x02: //CR 3/4
                _u8_DVBS2_CurrentCodeRate = 2;
                DBG_INTERN_DVBS(printf("INTERN_DVBS_GetTsDivNum INTERN_DVBS_GetCurrentCodeRate=3_4\n"));
                if(_bSerialTS)
                    *fTSDivNum = (288000.0/((3.0/4.0)*(188.0/204.0)*u32SymbolRate*2));
                else
                    *fTSDivNum = (288000.0/((3.0/4.0)*(188.0/204.0)*u32SymbolRate*2/8.0));
          
                *fTSDivNum = *fTSDivNum/2-1-5;
                break;
            case 0x03: //CR 5/6
                _u8_DVBS2_CurrentCodeRate = 3;
                DBG_INTERN_DVBS(printf("INTERN_DVBS_GetTsDivNum INTERN_DVBS_GetCurrentCodeRate=5_6\n"));
                if(_bSerialTS)
                    *fTSDivNum = (288000.0/((5.0/6.0)*(188.0/204.0)*u32SymbolRate*2));
                else
                    *fTSDivNum = (288000.0/((5.0/6.0)*(188.0/204.0)*u32SymbolRate*2/8.0));

                *fTSDivNum = *fTSDivNum/2-1-5;
                break;
            case 0x04: //CR 7/8
                _u8_DVBS2_CurrentCodeRate = 4;
                DBG_INTERN_DVBS(printf("INTERN_DVBS_GetTsDivNum INTERN_DVBS_GetCurrentCodeRate=7_8\n"));
                if(_bSerialTS)
                    *fTSDivNum =(288000.0/((7.0/8.0)*(188.0/204.0)*u32SymbolRate*2));
                else
                    *fTSDivNum = (288000.0/((7.0/8.0)*(188.0/204.0)*u32SymbolRate*2/8.0));

                *fTSDivNum = *fTSDivNum/2-1-5;
                break;
            default:
                _u8_DVBS2_CurrentCodeRate = 4;
                DBG_INTERN_DVBS(printf("INTERN_DVBS_GetTsDivNum INTERN_DVBS_GetCurrentCodeRate= default 7_8\n"));
                if(_bSerialTS)
                    *fTSDivNum = (288000.0/((7.0/8.0)*(188.0/204.0)*u32SymbolRate*2));
                else
                    *fTSDivNum = (288000.0/((7.0/8.0)*(188.0/204.0)*u32SymbolRate*2/8.0));

                *fTSDivNum = *fTSDivNum/2-1-5;
                break;
        }
    } //printf("INTERN_DVBS_GetTsDivNum u8TSClk = 0x%x\n", *u8TSDivNum);
    return status;
}

MS_BOOL INTERN_DVBS_GetLock(DMD_DVBS_GETLOCK_TYPE eType, float fCurrRFPowerDbm, float fNoChannelRFPowerDbm, MS_U32 u32TimeInterval)
{
    MS_U8 u8Data =0; //MS_U8 u8Data2 =0;
    MS_U8 bRet = TRUE;
    MS_FLOAT fTSDivNum=0;

    switch( eType )
    {
        case DMD_DVBS_GETLOCK:
#if (INTERN_DVBS_INTERNAL_DEBUG)
            INTERN_DVBS_info();
#endif
            bRet &= MDrv_SYS_DMD_VD_MBX_ReadReg((TOP_REG_BASE + 0x60*2), &u8Data);
            DBG_INTERN_DVBS(printf(">>>INTERN_DVBS_GetLock manual tune=%d<<<\n", u8Data));
            if ((u8Data&0x02)==0x00)//manual mode
            {
                bRet &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_STATE_FLAG, &u8Data);
                DBG_INTERN_DVBS(printf(">>>INTERN_DVBS_GetLock MailBox state=%d<<<\n", u8Data));

                if((u8Data == 15) || (u8Data == 16))
                {
                    if (u8Data==15)
                    {
                        _bDemodType=FALSE;   //S
                        DBG_INTERN_DVBS(printf(">>>INTERN_DVBS_Demod DVBS Lock<<<\n"));
                    }
                    else if(u8Data==16)
                    {
                        _bDemodType=TRUE;    //S2
                        DBG_INTERN_DVBS(printf(">>>INTERN_DVBS_Demod DVBS2 Lock<<<\n"));
                    }
                    if(g_dvbs_lock == 0)
                    {
                        g_dvbs_lock = 1;
                    }

                    if(u8DemodLockFlag==0)
                    {
                        u8DemodLockFlag=1;

                        // caculate TS clock divider number
                        INTERN_DVBS_GetTsDivNum(&fTSDivNum);  //ts_div_num
                        
                        if (fTSDivNum > 0x1F)
                            fTSDivNum = 0x1F;
                        else if (fTSDivNum < 0x00) 
                            fTSDivNum=0x00;

                        u8Data = (MS_U8)fTSDivNum;
                        DBG_INTERN_DVBS(printf(">>>INTERN_DVBS_GetLock TsClkDivNum = 0x%x<<<\n", u8Data));
                        HAL_DMD_RIU_WriteByte(0x103300, u8Data);
                        
                        //Ts Output Enable
                        //HAL_DMD_RIU_WriteByte(0x101eaa,0x10);
                    }
                    DBG_INTERN_DVBS(printf("@INTERN_DVBS_Demod Lock+++\n"));
                    bRet = TRUE;
                }
                else
                {
                    if(g_dvbs_lock == 1)
                    {
                        g_dvbs_lock = 0; 
                        u8DemodLockFlag=0;
                    }
                    DBG_INTERN_DVBS(printf("@INTERN_DVBS_Demod UnLock---\n"));
                    bRet = FALSE;
                }

                if(_bSerialTS==1)
                {
                    if (bRet==FALSE)
                    {
                        _bTSDataSwap=FALSE;
                    }
                    else
                    {
                        if (_bTSDataSwap==FALSE)
                        {
                            _bTSDataSwap=TRUE;
                            MDrv_SYS_DMD_VD_MBX_ReadReg( (DVBTM_REG_BASE + 0x20*2), &u8Data);//DVBTM_REG_BASE
                            u8Data^=0x20;//h0020    h0020    5    5    reg_ts_data_reverse
                            MDrv_SYS_DMD_VD_MBX_WriteReg( (DVBTM_REG_BASE + 0x20*2), u8Data);
                        }
                    }
                }
            }
            else
            {
                bRet = TRUE;
            }
            break;

        default:
            bRet = FALSE;
    }
    return bRet;
}

float INTERN_DVBS_GetTunrSignalLevel_PWR(void)// Need check debug out table
{
    MS_BOOL status=TRUE;
    MS_U16 u16Data =0;
    MS_U8  u8Data =0;
    MS_U8  u8Index =0;
    float  fCableLess = 0.0;

    if (FALSE == INTERN_DVBS_GetLock(DMD_DVBS_GETLOCK, 200.0f, -200.0f, 0) )//Demod unlock
    {
        fCableLess = 0;
    }

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE+0x11*2, &u8Data);//FRONTEND_AGC_DEBUG_SEL
    u8Data=(u8Data&0xF0)|0x03;
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FRONTEND_REG_BASE+0x11*2, u8Data);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE+0x02, &u8Data);//FRONTEND_LATCH
    u8Data|=0x80;
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FRONTEND_REG_BASE+0x02, u8Data);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE+0x12*2+1, &u8Data);//FRONTEND_AGC_DEBUG_OUT_R1
    u16Data=u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE+0x12*2, &u8Data);//FRONTEND_AGC_DEBUG_OUT_R0
    u16Data=(u16Data<<8)|u8Data;
    //printf("===========================Tuner 65535-u16Data = %d\n", (65535-u16Data));
    //MsOS_DelayTask(400);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE+0x02, &u8Data);//FRONTEND_LOAD0
    u8Data&=~(0x80);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FRONTEND_REG_BASE+0x02, u8Data);

    if (status==FALSE)
    {
        DBG_GET_SIGNAL_DVBS(printf("INTERN_DVBS GetSignalStrength fail!!! \n "));
        fCableLess = 0;
    }

    DBG_INTERN_DVBS(printf("#### INTERN_DVBS_GetTunrSignalLevel_PWR u16Data = %d\n", (int)u16Data));
    for (u8Index=0; u8Index < (sizeof(_u16SignalLevel)/sizeof(_u16SignalLevel[0])); u8Index++)
    {
        if ((65535 - u16Data) <= _u16SignalLevel[u8Index][0])
        {
            if (u8Index >=1)
            {
                fCableLess = (float)(_u16SignalLevel[u8Index][1])+((float)(_u16SignalLevel[u8Index][0] - (65535 - u16Data)) / (float)(_u16SignalLevel[u8Index][0] - _u16SignalLevel[u8Index-1][0]))*(float)(_u16SignalLevel[u8Index-1][1] - _u16SignalLevel[u8Index][1]);
                break;
            }
            else
            {
                fCableLess = _u16SignalLevel[u8Index][1];
		  break;		
            }
        }
    }
//---------------------------------------------------
    /*
    if (fCableLess >= 350)
        fCableLess = fCableLess - 35;
    else if ((fCableLess < 350) && (fCableLess >= 250))
        fCableLess = fCableLess - 25;
    else
        fCableLess = fCableLess - 5;
    */

    if (fCableLess < 0)
        fCableLess = 0;
    if (fCableLess > 920)
        fCableLess = 920;

    fCableLess = (-1.0)*(fCableLess/10.0);

    DBG_INTERN_DVBS(printf("INTERN_DVBS GetSignalStrength %f\n", fCableLess));

    return fCableLess;
}

/****************************************************************************
  Subject:    To get the Post viterbi BER
  Function:   INTERN_DVBS_GetPostViterbiBer
  Parmeter:  Quility
  Return:       E_RESULT_SUCCESS
                   E_RESULT_FAILURE =>Read I2C fail, INTERN_DVBC_VIT_STATUS_NG
  Remark:     For the Performance issue, here we just return the Post Value.(Not BER)
                   We will not read the Period, and have the "/256/8"
*****************************************************************************/
MS_BOOL INTERN_DVBS_GetPostViterbiBer(float *postber)//POST BER //V
{
    MS_BOOL           status = true;
    MS_U8             reg = 0, reg_frz = 0;
    MS_U16            BitErrPeriod;
    MS_U32            BitErr;

    /////////// Post-Viterbi BER /////////////After Viterbi

    // freeze ,DVBSRS_BACKEND_BIT_ERR_NUM_FREEZE
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSFEC_REG_BASE+0x1*2+1, &reg_frz);//h0001    h0001    8    8    reg_ber_en
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBSFEC_REG_BASE+0x1*2+1, reg_frz|0x01);

    // bank 1f 0x46 [7:0] reg_bit_err_sblprd_7_0 ,DVBSFEC_BIT_ERR_SBLPRD_7_0
    //             0x47 [15:8] reg_bit_err_sblprd_15_8
    //KRIS register table
    //h0018    h0018    7    0    reg_bit_err_sblprd_7_0
    //h0018    h0018    15    8    reg_bit_err_sblprd_15_8
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSFEC_REG_BASE+0x18*2+1, &reg);
    BitErrPeriod = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSFEC_REG_BASE+0x18*2, &reg);
    BitErrPeriod = (BitErrPeriod << 8)|reg;


    //h001d    h001d    7    0    reg_bit_err_num_7_0
    //h001d    h001d    15    8    reg_bit_err_num_15_8
    //h001e    h001e    7    0    reg_bit_err_num_23_16
    //h001e    h001e    15    8    reg_bit_err_num_31_24

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSFEC_REG_BASE+0x1E*2+1, &reg);
    BitErr = reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSFEC_REG_BASE+0x1E*2, &reg);
    BitErr = (BitErr << 8)|reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSFEC_REG_BASE+0x1D*2+1, &reg);
    BitErr = (BitErr << 8)|reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSFEC_REG_BASE+0x1D*2, &reg);
    BitErr = (BitErr << 8)|reg;

    // bank 1f 0x03 [1:0] reg_bit_err_num_freeze
    reg_frz=reg_frz&(~0x01);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBSFEC_REG_BASE+0x1*2+1, reg_frz);

    if (BitErrPeriod == 0 )    //PRD
        BitErrPeriod = 1;

    if (BitErr <= 0 )
        *postber = 0.5f / ((float)BitErrPeriod*128*188*8);
    else
        *postber = (float)BitErr / ((float)BitErrPeriod*128*188*8);

    if (*postber <= 0.0f)
        *postber = 1.0e-10f;

    DBG_GET_SIGNAL_DVBS(printf("INTERN_DVBS PostVitBER = %8.3e \n", *postber));

    return status;
}


MS_BOOL INTERN_DVBS_GetPreViterbiBer(float *preber)//PER BER // not yet
{
    MS_BOOL           status = true;
    //MS_U8             reg = 0, reg_frz = 0;
    //MS_U16            BitErrPeriod;
    //MS_U32            BitErr;

#if 0
    /////////// Pre-Viterbi BER /////////////Before Viterbi

    // freeze ,DVBSRS_BACKEND_BIT_ERR_NUM_FREEZE
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSTFEC_REG_BASE+0x10, &reg_frz);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBSTFEC_REG_BASE+0x10, reg_frz|0x08);

    // bank 1f 0x46 [7:0] reg_bit_err_sblprd_7_0 ,DVBSFEC_BIT_ERR_SBLPRD_7_0
    //             0x47 [15:8] reg_bit_err_sblprd_15_8
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSTFEC_REG_BASE+0x19, &reg);
    BitErrPeriod = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSTFEC_REG_BASE+0x18, &reg);
    BitErrPeriod = (BitErrPeriod << 8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSTFEC_REG_BASE+0x17, &reg);
    BitErrPeriod = (BitErrPeriod << 8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSTFEC_REG_BASE+0x16, &reg);
    BitErrPeriod = (BitErrPeriod << 8)|reg;
    BitErrPeriod = (BitErrPeriod & 0x3FFF);

    // bank 1f 0x6a [7:0] reg_bit_err_num_7_0
    //             0x6b [15:8] reg_bit_err_num_15_8
    // bank 1f 0x6c [7:0] reg_bit_err_num_23_16
    //             0x6d [15:8] reg_bit_err_num_31_24
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSTFEC_REG_BASE+0x1F, &reg);
    BitErr = reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSTFEC_REG_BASE+0x1E, &reg);
    BitErr = (BitErr << 8)|reg;

    // bank 1f 0x03 [1:0] reg_bit_err_num_freeze
    reg_frz=reg_frz&(~0x08);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBSFEC_REG_BASE+0x10, reg_frz);

    if (BitErrPeriod ==0 )//protect 0
        BitErrPeriod=1;
    if (BitErr <=0 )
        *perber=0.5f / (float)BitErrPeriod / 256;
    else
        *perber=(float)BitErr / (float)BitErrPeriod / 256;

    if (*perber <= 0.0f)
        *perber = 1.0e-10f;

    DBG_GET_SIGNAL_DVBS(printf("INTERN_DVBS PerVitBER = %8.3e \n", *perber));
#endif

    return status;
}

/****************************************************************************
  Subject:    To get the Packet error
  Function:   INTERN_DVBS_GetPacketErr
  Parmeter:   pktErr
  Return:     E_RESULT_SUCCESS
                   E_RESULT_FAILURE =>Read I2C fail, INTERN_DVBT_VIT_STATUS_NG
  Remark:     For the Performance issue, here we just return the Post Value.(Not BER)
                   We will not read the Period, and have the "/256/8"
*****************************************************************************/
MS_BOOL INTERN_DVBS_GetPacketErr(MS_U16 *pktErr)//V
{
    MS_BOOL          status = true;
    MS_U8            u8Data = 0;
    MS_U16           u16PktErr = 0;

    MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_SYSTEM_TYPE, &u8Data);
    if(!u8Data) //DVB-S2
    {
        status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2FEC_REG_BASE+0x02*2, &u8Data);//DVBS2FEC_OUTER_FREEZE   (_REG_DVBS2FEC(0x02)+0)     //[0]
        status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2FEC_REG_BASE+0x02*2, u8Data|0x01);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2FEC_REG_BASE+0x26*2+1, &u8Data);//DVBS2FEC_BCH_EFLAG2_SUM1 (_REG_DVBS2FEC(0x2B)+1)
    u16PktErr = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2FEC_REG_BASE+0x26*2, &u8Data);
    u16PktErr = (u16PktErr << 8)|u8Data;

        status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2FEC_REG_BASE+0x02*2, &u8Data);
        status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2FEC_REG_BASE+0x02*2, u8Data&(~0x01));
    }
    else
    { //DVB-S
        //status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSFEC_REG_BASE+0x19*2, &u8Data); //0x19 [7] reg_bit_err_num_freeze
        //status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBSFEC_REG_BASE+0x19*2, u8Data|0x80);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(REG_BACKEND+0x33*2+1, &u8Data);// DVBSFEC_UNCRT_PKT_NUM_15_8    (_REG_DVBSFEC(0x1F)+1)
    u16PktErr = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(REG_BACKEND+0x33*2, &u8Data);
    u16PktErr = (u16PktErr << 8)|u8Data;

        //status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSFEC_REG_BASE+0x19*2, &u8Data); //0x19 [7] reg_bit_err_num_freeze
        //status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBSFEC_REG_BASE+0x19*2, u8Data&(~0x80));
    }
    *pktErr = u16PktErr;

    DBG_INTERN_DVBS(printf("INTERN_DVBS PktErr = %d \n", (int)u16PktErr));

    return status;
}

/****************************************************************************
  Subject:    Read the signal to noise ratio (SNR)
  Function:   INTERN_DVBS_GetSNR
  Parmeter:   None
  Return:     -1 mean I2C fail, otherwise I2C success then return SNR value
  Remark:
*****************************************************************************/
MS_BOOL INTERN_DVBS_GetSNR(float *f_snr)//V
{
    MS_BOOL status= TRUE;
    MS_U8  u8Data =0, reg_frz =0;
    //NDA SNR
    MS_U32 u32NDA_SNR_A =0;
    MS_U32 u32NDA_SNR_AB =0;
    //NDA SNR
    float NDA_SNR_A =0.0;
    float NDA_SNR_AB =0.0;
    float NDA_SNR =0.0;
    double NDA_SNR_LINEAR=0.0;
    //float snr_poly =0.0;
    //float Fixed_SNR =0.0;

    if (INTERN_DVBS_GetLock(DMD_DVBS_GETLOCK, 200.0f, -200.0f, 0)== FALSE)
    {
        return 0;
    }

    // freeze
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE+0x04*2, &reg_frz);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2_INNER_REG_BASE+0x04*2, reg_frz|0x10);//INNE_LATCH      bit[4]

    //NDA SNR_A
    // read Linear_SNR
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0x47*2, &u8Data);
    u32NDA_SNR_A=(u8Data&0x03);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0x46*2 + 1, &u8Data);
    u32NDA_SNR_A=(u32NDA_SNR_A<<8)|u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0x46*2, &u8Data);
    u32NDA_SNR_A=(u32NDA_SNR_A<<8)|u8Data;
    //NDA SNR_AB
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0x49*2+1, &u8Data);
    u32NDA_SNR_AB=(u8Data&0x3F);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0x49*2, &u8Data);
    u32NDA_SNR_AB = (u32NDA_SNR_AB<<8)|u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0x48*2 + 1, &u8Data);
    u32NDA_SNR_AB=(u32NDA_SNR_AB<<8)|u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0x48*2, &u8Data);
    u32NDA_SNR_AB=(u32NDA_SNR_AB<<8)|u8Data;

    //UN_freeze
    reg_frz=reg_frz&(~0x10);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2_INNER_REG_BASE+0x08, reg_frz);

    if (status== FALSE)
    {
        DBG_INTERN_DVBS(printf("INTERN_DVBS_GetSNR Fail! \n"));
        return 0;
    }
    //NDA SNR
    NDA_SNR_A=(float)u32NDA_SNR_A/65536;
    NDA_SNR_AB=(float)u32NDA_SNR_AB/4194304;
    //
    //since support 16,32APSK we need to add judgement
    if(modulation_order==4)
        NDA_SNR_AB=(float)sqrt(NDA_SNR_AB/(2-1.252295758529242));//for 16APSK CR2/3
    else if(modulation_order==5)//(2-1.41333232789)
        NDA_SNR_AB=(float)sqrt(NDA_SNR_AB/(2-1.41333232789));//for 32APSK CR3/4
    else
        NDA_SNR_AB=(float)sqrt(NDA_SNR_AB);

    NDA_SNR_LINEAR =(1/((NDA_SNR_A/NDA_SNR_AB)-1)) ;

    if(NDA_SNR_LINEAR<=0)
        NDA_SNR=1.0;
    else
         NDA_SNR=10*log10(NDA_SNR_LINEAR);

    //printf("[DVBS]: NDA_SNR ================================: %.1f\n", NDA_SNR);
    _f_DVBS_CurrentSNR = NDA_SNR;
    /*
        //[DVBS/S2, QPSK/8PSK, 1/2~9/10 the same CN]
        snr_poly = 0.0;     //use Polynomial curve fitting to fix SNR
        snr_poly = 0.005261367463671*pow(NDA_SNR, 3)-0.116517828301214*pow(NDA_SNR, 2)+0.744836970505452*pow(NDA_SNR, 1)-0.86727609780167;
        Fixed_SNR = NDA_SNR + snr_poly;
        //printf("[DVBS]: NDA_SNR + snr_poly =====================: %.1f\n", Fixed_SNR);

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
    */
    *f_snr = NDA_SNR;
     //printf("[DVBS]: NDA_SNR=============================: %.1f\n", NDA_SNR);

    return status;
}

//SSI
MS_BOOL INTERN_DVBS_GetSignalStrength(MS_U16 *pu16SignalBar, const DMD_DVBS_InitData *sDMD_DVBS_InitData, MS_U8 u8SarValue, float fRFPowerDbm)
{
    //-1.2~-92.2 dBm
    MS_BOOL status = true;
    MS_U8   u8Data =0;
    MS_U8   _u8_DVBS2_CurrentCodeRateLocal = 0;
    float   ch_power_db=0.0f, ch_power_db_rel=0.0f;

    DBG_INTERN_DVBS_TIME(printf("INTERN_DVBS_GetSignalStrength, t=%d, RF level=%f, Table=%x\n",MsOS_GetSystemTime(), fRFPowerDbm, (MS_U32)(sDMD_DVBS_InitData->pTuner_RfagcSsi)));

    // if (INTERN_DVBC_Lock(COFDM_TPS_LOCK))
    // if (INTERN_DVBC_Lock(COFDM_AGC_LOCK))
    // Actually, it's more reasonable, that signal level depended on cable input power level
    // thougth the signal isn't dvb-t signal.
    //
    // use pointer of IFAGC table to identify
    // case 1: RFAGC from SAR, IFAGC controlled by demod
    // case 2: RFAGC from tuner, ,IFAGC controlled by demod
    //status=HAL_DMD_GetRFLevel(&ch_power_db, fRFPowerDbm, u8SarValue,
    //                          sDMD_DVBS_InitData->pTuner_RfagcSsi, sDMD_DVBS_InitData->u16Tuner_RfagcSsi_Size,
    //                          sDMD_DVBS_InitData->pTuner_IfagcSsi_HiRef, sDMD_DVBS_InitData->u16Tuner_IfagcSsi_HiRef_Size,
    //                          sDMD_DVBS_InitData->pTuner_IfagcSsi_LoRef, sDMD_DVBS_InitData->u16Tuner_IfagcSsi_LoRef_Size,
    //                          sDMD_DVBS_InitData->pTuner_IfagcErr_HiRef, sDMD_DVBS_InitData->u16Tuner_IfagcErr_HiRef_Size,
    //                          sDMD_DVBS_InitData->pTuner_IfagcErr_LoRef, sDMD_DVBS_InitData->u16Tuner_IfagcErr_LoRef_Size);
    ch_power_db = INTERN_DVBS_GetTunrSignalLevel_PWR();
    //printf("@@@@@@@@@ ch_power_db = %f \n", ch_power_db);

    MS_U8   u8Data2 = 0;
    MS_U8   _u8_DVBS2_CurrentConstellationLocal = 0;
    DMD_DVBS_DEMOD_TYPE pDemodType = 0;


    status &= INTERN_DVBS_GetCurrentDemodType(&pDemodType);

    if((MS_U8)pDemodType == (MS_U8)DMD_SAT_DVBS)//S
    {
        float fDVBS_SSI_Pref[]=
        {
            //0,       1,       2,       3,       4
            -78.9,   -77.15,  -76.14,  -75.19,  -74.57,//QPSK
        };
        status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSFEC_REG_BASE + 0x84, &u8Data);
        _u8_DVBS2_CurrentCodeRateLocal = (u8Data & 0x07);
        ch_power_db_rel = ch_power_db - fDVBS_SSI_Pref[_u8_DVBS2_CurrentCodeRateLocal];
    }
    else
    {
        float fDVBS2_SSI_Pref[][11]=
        {
            //  0,    1,       2,       3,       4,       5,       6,       7,       8,        9,       10
            //1/4,    1/3,     2/5,     1/2,     3/5,     2/3,     3/4,     4/5,     5/6,      8/9,     9/10
            {-85.17, -84.08,  -83.15,  -81.86,  -80.63,  -79.77,  -78.84,  -78.19,  -77.69,   -76.68,  -76.46}, //QPSK
            {   0.0,    0.0,     0.0,     0.0,  -77.36,  -76.24,  -74.95,     0.0,  -73.52,   -72.18,  -71.84}  //8PSK
        };

        status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0xD7, &u8Data);
        _u8_DVBS2_CurrentCodeRateLocal = (u8Data & 0x3C)>>2;

        status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0xD7, &u8Data);
        status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0xD6, &u8Data2);

        if(((u8Data & 0x03)==0x01) && ((u8Data2 & 0x80)==0x00))
        {
            _u8_DVBS2_CurrentConstellationLocal = DMD_DVBS_QPSK;
        }
        else if (((u8Data & 0x03)==0x01) && ((u8Data2 & 0x80)==0x80))
        {
            _u8_DVBS2_CurrentConstellationLocal = DMD_DVBS_8PSK;//8PSK
        }
        ch_power_db_rel = ch_power_db - fDVBS2_SSI_Pref[_u8_DVBS2_CurrentConstellationLocal][_u8_DVBS2_CurrentCodeRateLocal];
    }

    if(ch_power_db_rel <= -15.0f)
    {
        *pu16SignalBar = 0;
    }
    else if (ch_power_db_rel <= 0.0f)
    {
        *pu16SignalBar = (MS_U16)(2.0f/3 * (ch_power_db_rel+15.0f));
    }
    else if (ch_power_db_rel <= 20.0f)
    {
        *pu16SignalBar = (MS_U16)(4.0f * ch_power_db_rel + 10.0f);
    }
    else if (ch_power_db_rel <= 35.0f)
    {
        *pu16SignalBar = (MS_U16)(2.0f/3 * (ch_power_db_rel-20.0f) + 90.0);
    }
    else
    {
        *pu16SignalBar = 100;
    }

    DBG_INTERN_DVBS(printf(">>>>>Signal Strength(SSI) = %d\n", (int)*pu16SignalBar));

    return status;
}

//SQI
/****************************************************************************
  Subject:    To get the DVT Signal quility
  Function:   INTERN_DVBS_GetSignalQuality
  Parmeter:  Quility
  Return:      E_RESULT_SUCCESS
                   E_RESULT_FAILURE
  Remark:    Here we have 4 level range
                  <1>.First Range => Quility =100  (You can define it by INTERN_DVBT_SIGNAL_BASE_100)
                  <2>.2th Range => 60 < Quality < 100 (You can define it by INTERN_DVBT_SIGNAL_BASE_60)
                  <3>.3th Range => 10 < Quality < 60  (You can define it by INTERN_DVBT_SIGNAL_BASE_10)
                  <4>.4th Range => Quality <10
*****************************************************************************/
MS_BOOL INTERN_DVBS_GetSignalQuality(MS_U16 *quality, const DMD_DVBS_InitData *sDMD_DVBS_InitData, MS_U8 u8SarValue, float fRFPowerDbm)
{

    float       fber = 0.0;
    //float       log_ber;
    MS_BOOL     status = TRUE;
    float       f_snr = 0.0, ber_sqi = 0.0, cn_rel = 0.0;
    MS_U8       u8Data =0;
    MS_U16      u16Data =0;
    DMD_DVBS_CODE_RATE_TYPE       _u8_DVBS2_CurrentCodeRateLocal ;
    MS_U16     bchpkt_error,BCH_Eflag2_Window;
    //fRFPowerDbm = fRFPowerDbm;
    float snr_poly =0.0;
    float Fixed_SNR =0.0;
    double eFlag_PER=0.0;

    if (TRUE == INTERN_DVBS_GetLock(DMD_DVBS_GETLOCK, 200.0f, -200.0f, 0))
    {
        if(_bDemodType)  //S2
        {

           //INTERN_DVBS_GetSNR(&f_snr);
           status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_DBG_7, &u8Data);
           u16Data=u8Data;
           status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_DBG_6, &u8Data);
           u16Data = (u16Data<<8)|u8Data;
           f_snr=(float)u16Data/256.0;
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

            *quality = Fixed_SNR/30*ber_sqi;
            DBG_INTERN_DVBS(printf(" Fixed_SNR %f\n",Fixed_SNR));
            DBG_INTERN_DVBS(printf(" BCH_Eflag2_Window %d\n",BCH_Eflag2_Window));
            DBG_INTERN_DVBS(printf(" eFlag_PER [%f]\n fber [%8.3e]\n ber_sqi [%f]\n",eFlag_PER,fber,ber_sqi));
        }
        else  //S
        {
            if (INTERN_DVBS_GetPostViterbiBer(&fber) == FALSE)//ViterbiBer
            {
                DBG_INTERN_DVBS(printf("\nGetPostViterbiBer Fail!"));
                return FALSE;
            }
            _fPostBer=fber;


            if (status==FALSE)
            {
                DBG_INTERN_DVBS(printf("MSB131X_DTV_GetSignalQuality GetPostViterbiBer Fail!\n"));
                return 0;
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

            //status &= INTERN_DVBS_GetSNR(&f_snr);
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_DBG_7, &u8Data);
            u16Data=u8Data;
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_DBG_6, &u8Data);
            u16Data = (u16Data<<8)|u8Data;
            f_snr=(float)u16Data/256.0;
            DBG_GET_SIGNAL_DVBS(printf("INTERN_DVBS GetSNR = %d \n", (int)f_snr));

            cn_rel = f_snr - fDVBS_SQI_CNref[_u8_DVBS2_CurrentCodeRateLocal];

            DBG_INTERN_DVBS(printf(" fber = %f\n",fber));
            DBG_INTERN_DVBS(printf(" f_snr = %f\n",f_snr));
            DBG_INTERN_DVBS(printf(" cn_nordig_s1 = %f\n",fDVBS_SQI_CNref[_u8_DVBS2_CurrentCodeRateLocal]));
            DBG_INTERN_DVBS(printf(" cn_rel = %f\n",cn_rel));
            DBG_INTERN_DVBS(printf(" ber_sqi = %f\n",ber_sqi));

            if (cn_rel < -7.0f)
            {
                *quality = 0;
            }
            else if (cn_rel < 3.0)
            {
                *quality = (MS_U16)(ber_sqi*((cn_rel - 3.0)/10.0 + 1.0));
            }
            else
            {
                *quality = (MS_U16)ber_sqi;
            }


        }
            //INTERN_DVBS_GetTunrSignalLevel_PWR();//For Debug.
            DBG_INTERN_DVBS(printf(">>>>>Signal Quility(SQI) = %d\n", *quality));
            return TRUE;
    }
    else
    {
        *quality = 0;
    }

    return TRUE;
}

/****************************************************************************
  Subject:    To get the Cell ID
  Function:   INTERN_DVBS_Get_CELL_ID
  Parmeter:   point to return parameter cell_id

  Return:     TRUE
              FALSE
  Remark:
*****************************************************************************/
MS_BOOL INTERN_DVBS_Get_CELL_ID(MS_U16 *cell_id)
{
    MS_BOOL status = true;
    MS_U8 value1 = 0;
    MS_U8 value2 = 0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE+0x2B, &value2); //TPS Cell ID [15:8]
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FDP_REG_BASE+0x2A, &value1); //TPS Cell ID [7:0]

    *cell_id = ((MS_U16)value1<<8)|value2;
    return status;
}

/****************************************************************************
  Subject:    To get the DVBC Carrier Freq Offset
  Function:   INTERN_DVBS_Get_FreqOffset
  Parmeter:   Frequency offset (in KHz), bandwidth
  Return:     E_RESULT_SUCCESS
              E_RESULT_FAILURE
  Remark:
*****************************************************************************/
MS_BOOL INTERN_DVBS_Get_FreqOffset(float *pFreqOff, MS_U8 u8BW)
{
    //MS_U8       u8Data;
    //MS_U16      u16Data;
    //MS_S16      s16CFO;
    //float       FreqOffset;
    //MS_U32      u32FreqOffset = 0;
    //MS_U8       reg = 0;
    MS_BOOL     status = TRUE;
  #if 0
    DBG_INTERN_DVBS(printf(">>> INTERN_DVBS_Get_FreqOffset DVBS_Estimated_CFO <<<\n"));
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_TOP_WR_DBG_97, &u8Data);
    u16Data=u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_TOP_WR_DBG_96, &u8Data);
    u16Data=(u16Data<<8)|u8Data;                            //Center_Freq_Offset
    if (u16Data >= 0x8000)
    {
        u16Data=0x10000- u16Data;
        s16CFO=-1*u16Data;
    }
    else
    {
        s16CFO=u16Data;
    }
    DBG_INTERN_DVBS(printf(">>> INTERN_DVBS_Get_FreqOffset CFO = %d[KHz] <<<\n", s16CFO));
    if(abs(s16CFO)%1000 >= 500)
    {
        if(s16CFO < 0)
            *pFreqOff=(s16CFO/1000)-1.0;
        else
            *pFreqOff=(s16CFO/1000)+1.0;
    }
    else
        *pFreqOff = s16CFO/1000;
    DBG_INTERN_DVBS(printf(">>> INTERN_DVBS_Get_FreqOffset *pFreqOff = %d[MHz] <<<\n", (MS_S16)*pFreqOff));
    // no use.
    u8BW = u8BW;
    /*
    printf("INTERN_DVBS_Get_FreqOffset\n");//DBG_INTERN_DVBS(printf("INTERN_DVBS_Get_FreqOffset\n"));

    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FRONTENDEXT2_REG_BASE + 0x1C*2 + 1, 0x08);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x05, &reg);
    reg|=0x80;
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FRONTEND_REG_BASE + 0x05, reg);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTENDEXT2_REG_BASE + 0x23*2, &reg);
    u32FreqOffset=reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTENDEXT2_REG_BASE + 0x22*2 + 1, &reg);
    u32FreqOffset=(u32FreqOffset<<8)|reg;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTENDEXT2_REG_BASE + 0x22*2, &reg);
    u32FreqOffset=(u32FreqOffset<<8)|reg;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x05, &reg);
    reg&=~(0x80);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FRONTEND_REG_BASE + 0x05, reg);

    FreqOffset=(float)u32FreqOffset;
    if (FreqOffset>=2048)
    {
        FreqOffset=FreqOffset-4096;
    }
    FreqOffset=(FreqOffset/4096)*SAMPLING_RATE_FS;

    *pFreqOff = FreqOffset/1000;    //KHz
    printf("INTERN_DVBS_Get_FreqOffset:%d[MHz]\n", (MS_S16)FreqOffset/1000);//DBG_INTERN_DVBS(printf("INTERN_DVBS_Get_FreqOffset:%f[MHz]\n", FreqOffset/1000));
    */
 #endif
    return status;
}

/****************************************************************************
  Subject:    To get the current modulation type at the DVB-S Demod
  Function:   INTERN_DVBS_GetCurrentModulationType
  Parmeter:   pointer for return QAM type

  Return:     TRUE
              FALSE
  Remark:
*****************************************************************************/
MS_BOOL INTERN_DVBS_GetCurrentModulationType(DMD_DVBS_MODULATION_TYPE *pQAMMode)
{
    MS_U8 u8Data=0;
    MS_U16 u16tmp=0;
    MS_U8 MOD_type;
    MS_BOOL     status = true;
    //DMD_DVBS_DEMOD_TYPE pDemodType = 0;

    DBG_INTERN_DVBS(printf("INTERN_DVBS_GetCurrentModulationType\n"));

    //status &= INTERN_DVBS_GetCurrentDemodType(&pDemodType);

    // read code rate, pilot on/off, long/short FEC type, and modulation type for calculating TOP_DVBTM_TS_CLK_DIVNUM
    // pilot_flag     =>   0 : off    1 : on
    // fec_type_idx   =>   0 : normal 1 : short
    // mod_type_idx   =>   0 : QPSK   1 : 8PSK   2 : 16APSK
    // code_rate_idx  =>   0 : 1/4    1 : 1/3    2 : 2/5    3 : 1/2    4 : 3/5    5 : 2/3
    //                     6 : 3/4    7 : 4/5    8 : 5/6    9 : 8/9   10 : 9/10
    MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_SYSTEM_TYPE, &u8Data);//V
    if(u8Data)
    {
        *pQAMMode = DMD_DVBS_QPSK;
        modulation_order=2;
        printf("INTERN_DVBS_GetCurrentModulationType [dvbs]ModulationType=DVBS_QPSK\n");//DBG_INTERN_DVBS_LOCK(printf("[dvbs]ModulationType=DVBS_QPSK\n"));
        //return TRUE;
    }
    else                                        //S2
    {
        //MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0x96, &u8Data);
        //printf(">>> INTERN_DVBS_GetCurrentModulationType INNER 0x4B = 0x%x <<<\n", u8Data);
        //if((u8Data & 0x0F)==0x02)       //QPSK
        /*MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MOD_TYPE, &u8Data1);
      printf("@@@@@E_DMD_S2_MOD_TYPE = %d \n ",u8Data1);
      printf("@@@@@  E_DMD_S2_MOD_TYPE=%d  \n",E_DMD_S2_MOD_TYPE);

        MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_IS_ID, &u8Data1);
      printf("@@@@@E_DMD_S2_IS_ID = %d \n ",u8Data1);
      printf("@@@@@  E_DMD_S2_IS_ID=%d  \n",E_DMD_S2_IS_ID);*/

        // INNER_DEBUG_SEL
        MDrv_SYS_DMD_VD_MBX_ReadReg(0x3b00+0x04*2+1, &u8Data);
        u8Data = u8Data & 0xc0;
        MDrv_SYS_DMD_VD_MBX_WriteReg(0x3b00+0x04*2+1, u8Data);

        // reg_plscdec_debug_out
        // PLSCDEC info
        //[0:4] PLSC MODCOD
        //[5] dummy frame
        //[6] reserve frame
        //[7:9] modulation type
        //[10:13] code rate type
        //[14] FEC type
        //[15] pilot type
        MDrv_SYS_DMD_VD_MBX_ReadReg(0x3b00+0x6b*2  , &u8Data);
        u16tmp = (MS_U16)u8Data;
        MDrv_SYS_DMD_VD_MBX_ReadReg(0x3b00+0x6b*2+1 , &u8Data);
        u16tmp |= (MS_U16)u8Data << 8;
        MOD_type = ((MS_U8)(u16tmp>>7)&0x07);  // 2:QPSK, 3:8PSK, 4:16APSK, 5:32APSK

        if(MOD_type==2)
        {
            *pQAMMode = DMD_DVBS_QPSK;
        modulation_order=2;
            printf("INTERN_DVBS_GetCurrentModulationType [dvbs2]ModulationType=DVBS2_QPSK\n");//DBG_INTERN_DVBS_LOCK(printf("[dvbs]ModulationType=DVBS2_QPSK\n"));
            //return TRUE;
        }
        else if(MOD_type==3)
        {
            *pQAMMode = DMD_DVBS_8PSK;
        modulation_order=3;
            printf("INTERN_DVBS_GetCurrentModulationType [dvbs2]ModulationType=DVBS2_8PSK\n");//DBG_INTERN_DVBS_LOCK(printf("[dvbs]ModulationType=DVBS2_8PSK\n"));
            //return TRUE;
        }
         else if(MOD_type==4)
         {
            *pQAMMode = DMD_DVBS_16APSK;
        modulation_order=4;
            printf("INTERN_DVBS_GetCurrentModulationType [dvbs2]ModulationType=DVBS2_16APSK\n");
         }
        else
        {
            *pQAMMode = DMD_DVBS_QPSK;
            modulation_order=2;
            printf("INTERN_DVBS_GetCurrentModulationType [dvbs2]ModulationType=NOT SUPPORT\n");
            return FALSE;
        }

    }

    return status;
/*#else
    *pQAMMode = DMD_DVBS_QPSK;
    printf("[dvbs]ModulationType=DVBS_QPSK\n");//DBG_INTERN_DVBS_LOCK(printf("[dvbs]ModulationType=DVBS_QPSK\n"));
    //return true;
#endif*/
}

/****************************************************************************
  Subject:    To get the current DemodType at the DVB-S Demod
  Function:   INTERN_DVBS_GetCurrentDemodType
  Parmeter:   pointer for return DVBS/DVBS2 type

  Return:     TRUE
              FALSE
  Remark:
*****************************************************************************/
MS_BOOL INTERN_DVBS_GetCurrentDemodType(DMD_DVBS_DEMOD_TYPE *pDemodType)//V
{
    MS_U8 u8Data=0;
    MS_BOOL     status = true;

    DBG_INTERN_DVBS(printf("INTERN_DVBS_GetCurrentDemodType\n"));

    //MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0x80, &u8Data);//status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0x80, &u8Data);
    //printf(">>> INTERN_DVBS_GetCurrentDemodType INNER 0x40 = 0x%x <<<\n", u8Data);
    //if ((u8Data & 0x01) == 0)
    MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_SYSTEM_TYPE, &u8Data);//E_DMD_S2_SYSTEM_TYPE 0: S2 ; 1 :S
    if(!u8Data)                                                       //S2
    {
        *pDemodType = DMD_SAT_DVBS2;
        DBG_INTERN_DVBS(printf("[dvbs]DemodType=DVBS2\n"));
    }
    else                                                                            //S
    {
        *pDemodType = DMD_SAT_DVBS;
        DBG_INTERN_DVBS(printf("[dvbs]DemodType=DVBS\n"));
    }
    return status;
}
/****************************************************************************
  Subject:    To get the current CodeRate at the DVB-S Demod
  Function:   INTERN_DVBS_GetCurrentCodeRate
  Parmeter:   pointer for return Code Rate type

  Return:     TRUE
              FALSE
  Remark:
*****************************************************************************/
MS_BOOL INTERN_DVBS_GetCurrentDemodCodeRate(DMD_DVBS_CODE_RATE_TYPE *pCodeRate)
{
    MS_U8 u8Data = 0;//, u8_gCodeRate = 0;
    MS_BOOL     status = true;

    DBG_INTERN_DVBS(printf("INTERN_DVBS_GetCurrentCodeRate\n"));
    //DMD_DVBS_DEMOD_TYPE pDemodType = 0;
    MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_SYSTEM_TYPE, &u8Data);
    //status &= INTERN_DVBS_GetCurrentDemodType(&pDemodType);
    if(!u8Data)
    //if((MS_U8)pDemodType == (MS_U8)DMD_SAT_DVBS2 )  //S2
    {
        MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_CODE_RATE, &u8Data);
        //status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0xD7, &u8Data);
        //u8_gCodeRate = (u8Data & 0x3C);
        //_u8_DVBS2_CurrentCodeRate = 0;
        switch (u8Data)
        //switch (u8_gCodeRate)
        {
        case 0x03:
            *pCodeRate = DMD_CONV_CODE_RATE_1_2;
            _u8_DVBS2_CurrentCodeRate = 5;//0;
            DBG_INTERN_DVBS(printf("INTERN_DVBS2_GetCurrentCodeRate=1_2\n"));
            break;
        case 0x01:
            *pCodeRate = DMD_CONV_CODE_RATE_1_3;
            _u8_DVBS2_CurrentCodeRate = 6;//1;
            DBG_INTERN_DVBS(printf("INTERN_DVBS2_GetCurrentCodeRate=1_3\n"));
            break;
        case 0x05:
            *pCodeRate = DMD_CONV_CODE_RATE_2_3;
            _u8_DVBS2_CurrentCodeRate = 7;//2;
            DBG_INTERN_DVBS(printf("INTERN_DVBS2_GetCurrentCodeRate=2_3\n"));
            break;
        case 0x00:
            *pCodeRate = DMD_CONV_CODE_RATE_1_4;
            _u8_DVBS2_CurrentCodeRate = 8;//3;
            DBG_INTERN_DVBS(printf("INTERN_DVBS2_GetCurrentCodeRate=1_4\n"));
            break;
        case 0x06:
            *pCodeRate = DMD_CONV_CODE_RATE_3_4;
            _u8_DVBS2_CurrentCodeRate = 9;//4;
            DBG_INTERN_DVBS(printf("INTERN_DVBS2_GetCurrentCodeRate=3_4\n"));
            break;
        case 0x02:
            *pCodeRate = DMD_CONV_CODE_RATE_2_5;
            _u8_DVBS2_CurrentCodeRate = 10;//5;
            DBG_INTERN_DVBS(printf("INTERN_DVBS2_GetCurrentCodeRate=2_5\n"));
            break;
        case 0x04:
            *pCodeRate = DMD_CONV_CODE_RATE_3_5;
            _u8_DVBS2_CurrentCodeRate = 11;//6;
            DBG_INTERN_DVBS(printf("INTERN_DVBS2_GetCurrentCodeRate=3_5\n"));
            break;
        case 0x07:
            *pCodeRate = DMD_CONV_CODE_RATE_4_5;
            _u8_DVBS2_CurrentCodeRate = 12;//7;
            DBG_INTERN_DVBS(printf("INTERN_DVBS2_GetCurrentCodeRate=4_5\n"));
            break;
        case 0x08:
            *pCodeRate = DMD_CONV_CODE_RATE_5_6;
            _u8_DVBS2_CurrentCodeRate = 13;//8;
            DBG_INTERN_DVBS(printf("INTERN_DVBS2_GetCurrentCodeRate=5_6\n"));
            break;
        case 0x09:
            *pCodeRate = DMD_CONV_CODE_RATE_8_9;
            _u8_DVBS2_CurrentCodeRate = 14;//9;
            DBG_INTERN_DVBS(printf("INTERN_DVBS2_GetCurrentCodeRate=8_9\n"));
            break;
        case 0x0a:
            *pCodeRate = DMD_CONV_CODE_RATE_9_10;
            _u8_DVBS2_CurrentCodeRate = 15;//10;
            DBG_INTERN_DVBS(printf("INTERN_DVBS2_GetCurrentCodeRate=9_10\n"));
            break;
        default:
            *pCodeRate = DMD_CONV_CODE_RATE_9_10;
            _u8_DVBS2_CurrentCodeRate = 15;//10;
            DBG_INTERN_DVBS(printf("INTERN_DVBS2_GetCurrentCodeRate=DVBS2_Default\n"));
        }
    }
    else                                            //S
    {
        MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_CODE_RATE, &u8Data);
        //status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBSTFEC_REG_BASE + 0x3A*2, &u8Data);
        //u8_gCodeRate = (u8Data & 0x70)>>4;
        switch (u8Data)
        //switch (u8_gCodeRate)
        {
        case 0x00:
            *pCodeRate = DMD_CONV_CODE_RATE_1_2;
            _u8_DVBS2_CurrentCodeRate = 0;
            DBG_INTERN_DVBS(printf("INTERN_DVBS_GetCurrentCodeRate=1_2\n"));
            break;
        case 0x01:
            *pCodeRate = DMD_CONV_CODE_RATE_2_3;
            _u8_DVBS2_CurrentCodeRate = 1;
            DBG_INTERN_DVBS(printf("INTERN_DVBS_GetCurrentCodeRate=2_3\n"));
            break;
        case 0x02:
            *pCodeRate = DMD_CONV_CODE_RATE_3_4;
            _u8_DVBS2_CurrentCodeRate = 2;
            DBG_INTERN_DVBS(printf("INTERN_DVBS_GetCurrentCodeRate=3_4\n"));
            break;
        case 0x03:
            *pCodeRate = DMD_CONV_CODE_RATE_5_6;
            _u8_DVBS2_CurrentCodeRate = 3;
            DBG_INTERN_DVBS(printf("INTERN_DVBS_GetCurrentCodeRate=5_6\n"));
            break;
        case 0x04:
            *pCodeRate = DMD_CONV_CODE_RATE_7_8;
            _u8_DVBS2_CurrentCodeRate = 4;
            DBG_INTERN_DVBS(printf("INTERN_DVBS_GetCurrentCodeRate=7_8\n"));
            break;
        default:
            *pCodeRate = DMD_CONV_CODE_RATE_7_8;
            _u8_DVBS2_CurrentCodeRate = 4;
            DBG_INTERN_DVBS(printf("INTERN_DVBS_GetCurrentCodeRate=DVBS_Default\n"));
        }
    }
    return status;
}

/****************************************************************************
  Subject:    To get the current symbol rate at the DVB-S Demod
  Function:   INTERN_DVBS_GetCurrentSymbolRate
  Parmeter:   pointer pData for return Symbolrate

  Return:     TRUE
              FALSE
  Remark:
*****************************************************************************/
MS_BOOL INTERN_DVBS_GetCurrentSymbolRate(MS_U32 *u32SymbolRate)
{
    MS_U8  tmp = 0;
    MS_U16 u16SymbolRateTmp = 0;

    MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_H, &tmp);
    u16SymbolRateTmp = tmp;
    MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MANUAL_TUNE_SYMBOLRATE_L, &tmp);
    u16SymbolRateTmp = (u16SymbolRateTmp<<8)|tmp;

    MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_DBG_5, &tmp);
    *u32SymbolRate = (tmp<<16)|u16SymbolRateTmp;

    DBG_INTERN_DVBS_LOCK(printf("[dvbs]Symbol Rate=%d\n",*u32SymbolRate));

    return TRUE;
}

MS_BOOL INTERN_DVBS_Version(MS_U16 *ver)
{
    MS_U8 status = true;
    MS_U8 tmp = 0;
    MS_U16 u16_INTERN_DVBS_Version;

    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_FW_VERSION_L, &tmp);
    u16_INTERN_DVBS_Version = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_FW_VERSION_H, &tmp);
    u16_INTERN_DVBS_Version = u16_INTERN_DVBS_Version<<8|tmp;
    *ver = u16_INTERN_DVBS_Version;

    return status;
}

MS_BOOL INTERN_DVBS_Show_Demod_Version(void)
{
    MS_BOOL status = true;
    MS_U16 u16_INTERN_DVBS_Version;

    status &= INTERN_DVBS_Version(&u16_INTERN_DVBS_Version);

    printf(">>> [Macan]Demod FW Version: R%d.%d <<<\n", ((u16_INTERN_DVBS_Version>>8)&0x00FF),(u16_INTERN_DVBS_Version&0x00FF));


    return status;
}

MS_BOOL INTERN_DVBS_GetRollOff(MS_U8 *pRollOff)
{
    MS_BOOL status=TRUE;
    MS_U8 u8Data=0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_INNER_REG_BASE + 0x1E, &u8Data);//#define INNER_TR_ROLLOFF (_REG_INNER(0x0F)+0)
    if ((u8Data&0x03)==0x00)
        *pRollOff = 0;  //Rolloff 0.35
    else if (((u8Data&0x03)==0x01) || ((u8Data&0x03)==0x03))
        *pRollOff = 1;  //Rolloff 0.25
    else
        *pRollOff = 2;  //Rolloff 0.20
    DBG_INTERN_DVBS(printf("INTERN_DVBS_GetRollOff:%d\n", *pRollOff));

    return status;
}

MS_BOOL INTERN_DVBS_Get_DTV_SQuality_BAR(MS_U8 *u8_gSQValue)
{
    MS_BOOL     status=TRUE;
    MS_U16      u16_gSignalQualityValue;
    MS_U16      _u16_packetError;

    status = INTERN_DVBS_GetSignalQuality(&u16_gSignalQualityValue,0,0,0);
    status = INTERN_DVBS_GetPacketErr(&_u16_packetError);

    if ((_u16_packetError >= 1) && (u16_gSignalQualityValue > 30))           //Average
    {
        *u8_gSQValue = 30;
    }
    else if ((_u16_packetError >= 1) && (u16_gSignalQualityValue > 10))      //Poor
    {
        *u8_gSQValue = 10;
    }

    return status;
}

/****************************************************************************
**      Function: Read demod related information
**      Polling after demod lock
**      GAIN & DCR /Fine CFO & PR & IIS & IQB & SNR /PacketErr & BER
****************************************************************************/
MS_BOOL INTERN_DVBS_Show_AGC_Info(void)
{
    MS_BOOL status = TRUE;

    //MS_U8 tmp = 0;
    //MS_U8 agc_k = 0,d0_k = 0,d0_ref = 0,d1_k = 0,d1_ref = 0,d2_k = 0,d2_ref = 0,d3_k = 0,d3_ref = 0;
    //MS_U16 if_agc_gain = 0,d0_gain = 0,d1_gain = 0,d2_gain = 0,d3_gain = 0, agc_ref = 0;
    //MS_U16 if_agc_err = 0;
#if 0
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x08*2 + 1,&agc_k);
    agc_k = ((agc_k & 0xF0)>>4);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x09*2 + 1,&tmp);
    agc_ref = tmp;
    //status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTENDEXT_REG_BASE + 0xE8,&tmp);
    //agc_ref = (agc_ref<<8)|tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x38*2,&d0_k);
    d0_k = ((d0_k & 0xF0)>>4);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x38*2 + 1,&d0_ref);
    d0_ref = (d0_ref & 0xFF);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x58*2,&d1_k);
    d1_k = (d1_k & 0xF0)>>4;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x58*2 + 1,&d1_ref);
    d1_ref = (d1_ref & 0xFF);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x5E*2,&d2_k);
    d2_k = ((d2_k & 0xF0)>>4);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTENDEXT_REG_BASE + 0x5E*2 + 1,&d2_ref);
    d2_ref = (d2_ref & 0xFF);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTENDEXT_REG_BASE + 0x6A*2,&d3_k);
    d3_k = ((d3_k & 0xF0)>>4);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTENDEXT2_REG_BASE + 0x6A*2 + 1,&d3_ref);
    d3_ref = (d3_ref & 0xFF);


    // select IF gain to read
    //Debug Select
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x13*2, &tmp);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FRONTEND_REG_BASE + 0x13*2, (tmp&0xF0)|0x03);
    //IF_AGC_GAIN
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x14*2 + 1, &tmp);
    if_agc_gain = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x14*2, &tmp);
    if_agc_gain = (if_agc_gain<<8)|tmp;


    // select d0 gain to read.
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x74*2 + 1, &tmp);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FRONTEND_REG_BASE + 0x74*2 + 1, (tmp&0xF0)|0x03);
    //DAGC0_GAIN
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x3D*2, &tmp);
    d0_gain = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x3C*2 + 1, &tmp);
    d0_gain = (d0_gain<<8)|tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x3C*2, &tmp);
    d0_gain = (d0_gain<<4)|(tmp>>4);


    // select d1 gain to read.
    //status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x8C, &tmp);
    //status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FRONTEND_REG_BASE + 0x8C, (tmp&0xF0)|0x00);
    //DAGC1_GAIN
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x5C*2 + 1, &tmp);
    d1_gain = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x5C*2, &tmp);
    d1_gain = (d1_gain<<8)|tmp;


    // select d2 gain to read.
    //status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTENDEXT_REG_BASE + 0x06, &tmp);
    //status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FRONTENDEXT_REG_BASE + 0x06, (tmp&0xF0)|0x03);
    //DAGC2_GAIN
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x62*2 + 1, &tmp);
    d2_gain = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x62*2, &tmp);
    d2_gain = (d2_gain<<8)|tmp;


    // select d3 gain to read.
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTENDEXT2_REG_BASE + 0x6D*2, &tmp);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FRONTENDEXT2_REG_BASE + 0x6D*2, (tmp&0xF0)|0x03);
    //DAGC3_GAIN
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTENDEXT_REG_BASE + 0x6F*2, &tmp);
    d3_gain = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTENDEXT_REG_BASE + 0x6E*2 + 1, &tmp);
    d3_gain = (d3_gain<<8)|tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTENDEXT_REG_BASE + 0x6E*2, &tmp);
    d3_gain = (d3_gain<<4)|(tmp>>4);


    // select IF gain err to read
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x13*2, &tmp);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(FRONTEND_REG_BASE + 0x13*2, (tmp&0xF0)|0x00);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x14*2 + 1, &tmp);
    if_agc_err = tmp;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(FRONTEND_REG_BASE + 0x14*2, &tmp);
    if_agc_err = (if_agc_err<<8)|tmp;


    DBG_INTERN_DVBS(printf("[dvbs]agc_k=0x%x, agc_ref=0x%x, d0_k=0x%x, d0_ref=0x%x, d1_k=0x%x, d1_ref=0x%x, d2_k=0x%x, d2_ref=0x%x, d3_k=0x%x, d3_ref=0x%x\n",
                           agc_k, agc_ref, d0_k, d0_ref, d1_k, d1_ref, d2_k, d2_ref, d3_k, d3_ref));

    DBG_INTERN_DVBS(printf("[dvbs]agc_g=0x%x, d0_g=0x%x, d1_g=0x%x, d2_g=0x%x, d3_g=0x%x, agc_err=0x%x\n", if_agc_gain, d0_gain, d1_gain, d2_gain, d3_gain, if_agc_err));

    DBG_INTERN_DVBS(printf("[dvbs]agc_k=0x%x, agc_ref=0x%x, d0_k=0x%x, d0_ref=0x%x, d1_k=0x%x, d1_ref=0x%x, d2_k=0x%x, d2_ref=0x%x, d3_k=0x%x, d3_ref=0x%x\n",
                           agc_k, agc_ref, d0_k, d0_ref, d1_k, d1_ref, d2_k, d2_ref, d3_k, d3_ref));

    DBG_INTERN_DVBS(printf("[dvbs]agc_g=0x%x, d0_g=0x%x, d1_g=0x%x, d2_g=0x%x, d3_g=0x%x, agc_err=0x%x\n", if_agc_gain, d0_gain, d1_gain, d2_gain, d3_gain, if_agc_err));
#endif
    return status;
}

void INTERN_DVBS_info(void)
{
    //status &= INTERN_DVBS_Show_Demod_Version();
    //status &= INTERN_DVBS_Demod_Get_Debug_Info_get_once();
    //status &= INTERN_DVBS_Demod_Get_Debug_Info_polling();
}

MS_BOOL INTERN_DVBS_Demod_Get_Debug_Info_get_once(void)
{
    MS_BOOL             status = TRUE;
    //MS_U8               u8Data = 0;
    //MS_U16              u16Data = 0, u16Address = 0;
    //float               psd_smooth_factor;
    //float               srd_right_bottom_value, srd_right_top_value, srd_left_bottom_value, srd_left_top_value;
    //MS_U16              u32temp5;
    //MS_U16              srd_left, srd_right, srd_left_top, srd_left_bottom, srd_right_top, srd_right_bottom;

#if 0
//Lock Flag
    printf("========================================================================\n");
    printf("Debug Message Flag [Lock Flag]==========================================\n");

    u16Address = (AGC_LOCK>>16)&0xffff;
    status &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    if ((u16Data&(AGC_LOCK&0xffff))!=(AGC_LOCK&0xffff))
        printf("[DVBS]: AGC LOCK ======================: Fail. \n");
    else
        printf("[DVBS]: AGC LOCK ======================: OK. \n");

    u16Address = (DAGC0_LOCK>>16)&0xffff;
    status &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    if ((u16Data&(DAGC0_LOCK&0xffff))!=(DAGC0_LOCK&0xffff))
        printf("[DVBS]: DAGC0 LOCK ====================: Fail. \n");
    else
        printf("[DVBS]: DAGC0 LOCK ====================: OK. \n");

    u16Address = (DAGC1_LOCK>>16)&0xffff;
    status &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    if ((u16Data&(DAGC1_LOCK&0xffff))!=(DAGC1_LOCK&0xffff))
        printf("[DVBS]: DAGC1 LOCK ====================: Fail. \n");
    else
        printf("[DVBS]: DAGC1 LOCK ====================: OK. \n");

    u16Address = (DAGC2_LOCK>>16)&0xffff;
    status &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    if ((u16Data&(DAGC2_LOCK&0xffff))!=(DAGC2_LOCK&0xffff))
        printf("[DVBS]: DAGC2 LOCK ====================: Fail. \n");
    else
        printf("[DVBS]: DAGC2 LOCK ====================: OK. \n");

    u16Address = (DAGC3_LOCK>>16)&0xffff;
    status &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    if ((u16Data&(DAGC3_LOCK&0xffff))!=(DAGC3_LOCK&0xffff))
        printf("[DVBS]: DAGC3 LOCK ====================: Fail. \n");
    else
        printf("[DVBS]: DAGC3 LOCK ====================: OK. \n");

    u16Address = (DCR_LOCK>>16)&0xffff;
    status &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    if ((u16Data&(DCR_LOCK&0xffff))!=(DCR_LOCK&0xffff))
        printf("[DVBS]: DCR LOCK ======================: Fail. \n");
    else
        printf("[DVBS]: DCR LOCK ======================: OK. \n");
//Mark Coarse SRD
//Mark Fine SRD
/*
    u16Address = (CLOSE_COARSE_CFO_LOCK>>16)&0xffff;
    status &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    if ((u16Data&(CLOSE_COARSE_CFO_LOCK&0xffff))!=(CLOSE_COARSE_CFO_LOCK&0xffff))
        printf("[DVBS]: Close CFO =====================: Fail. \n");
    else
        printf("[DVBS]: Close CFO =====================: OK. \n");
*/
    u16Address = (TR_LOCK>>16)&0xffff;
    status &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    if ((u16Data&(TR_LOCK&0xffff))!=(TR_LOCK&0xffff))
        printf("[DVBS]: TR LOCK =======================: Fail. \n");
    else
        printf("[DVBS]: TR LOCK =======================: OK. \n");

    u16Address = (FRAME_SYNC_ACQUIRE>>16)&0xffff;
    status &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    if ((u16Data&(FRAME_SYNC_ACQUIRE&0xffff))!=(FRAME_SYNC_ACQUIRE&0xffff))
        printf("[DVBS]: FS Acquire ====================: Fail. \n");
    else
        printf("[DVBS]: FS Acquire ====================: OK. \n");

    u16Address = (PR_LOCK>>16)&0xffff;
    status &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    if ((u16Data&(PR_LOCK&0xffff))!=(PR_LOCK&0xffff))
        printf("[DVBS]: PR LOCK =======================: Fail. \n");
    else
        printf("[DVBS]: PR LOCK =======================: OK. \n");

    u16Address = (EQ_LOCK>>16)&0xffff;
    status &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    if ((u16Data&(EQ_LOCK&0xffff))!=(EQ_LOCK&0xffff))
        printf("[DVBS]: EQ LOCK =======================: Fail. \n");
    else
        printf("[DVBS]: EQ LOCK =======================: OK. \n");

    u16Address = (P_SYNC_LOCK>>16)&0xffff;
    status &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    if ((u16Data&0x0002)!=0x0002)
        printf("[DVBS]: P_sync ========================: Fail. \n");
    else
        printf("[DVBS]: P_sync ========================: OK. \n");

    u16Address = (IN_SYNC_LOCK>>16)&0xffff;
    status &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    if ((u16Data&0x8000)!=0x8000)
        printf("[DVBS]: In_sync =======================: Fail. \n");
    else
        printf("[DVBS]: In_sync =======================: OK. \n");
//---------------------------------------------------------
//Lock Time
    printf("------------------------------------------------------------------------\n");
    printf("Debug Message [Lock Time]===============================================\n");

    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_INFO_05, &u8Data);
    printf("[DVBS]: AGC Lock Time =================: %d\n",u8Data&0x00FF);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_INFO_06, &u8Data);
    printf("[DVBS]: DCR Lock Time =================: %d\n",u8Data&0x00FF);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_DBG_2, &u8Data);
    printf("[DVBS]: TR Lock Time ==================: %d\n",u8Data&0x00FF);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_DBG_3, &u8Data);
    printf("[DVBS]: FS Lock Time ==================: %d\n",u8Data&0x00FF);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_DBG_4, &u8Data);
    printf("[DVBS]: PR Lock Time ==================: %d\n",u8Data&0x00FF);
    //printf("[DVBS]: PLSC Lock Time ================: %d\n",(u16Data>>8)&0x00FF);//No used
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_DBG_6, &u8Data);
    printf("[DVBS]: EQ Lock Time ==================: %d\n",u8Data&0x00FF);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_DBG_7, &u8Data);
    printf("[DVBS]: FEC Lock Time =================: %d\n",u8Data&0x00FF);

    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_DBG_0, &u8Data);
    printf("[DVBS]: CSRD ==========================: %d\n",u8Data&0x00FF);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_DBG_1, &u8Data);
    printf("[DVBS]: FSRD ==========================: %d\n",u8Data&0x00FF);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_INFO_01, &u8Data);
    printf("[DVBS]: CCFO ==========================: %d\n",u8Data&0x00FF);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_INFO_02, &u8Data);
    printf("[DVBS]: FCFO ==========================: %d\n",u8Data&0x00FF);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_STATE_FLAG, &u8Data);
    printf("[DVBS]: State =========================: %d\n",u8Data&0x00FF);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_SUBSTATE_FLAG, &u8Data);
    printf("[DVBS]: SubState ======================: %d\n",u8Data&0x00FF);

    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_SWUSE00H, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_SWUSE00L, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    printf("[DVBS]: DBG1: =========================: 0x%x\n",u16Data);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_SWUSE01H, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_SWUSE01L, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    printf("[DVBS]: DBG2: =========================: 0x%x\n",u16Data);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_SWUSE02H, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_SWUSE02L, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    printf("[DVBS]: DBG3: =========================: 0x%x\n",u16Data);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_SWUSE03H, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_SWUSE03L, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    printf("[DVBS]: DBG4: =========================: 0x%x\n",u16Data);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_SWUSE04H, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_SWUSE04L, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    printf("[DVBS]: DBG5: =========================: 0x%x\n",u16Data);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_SWUSE05H, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_SWUSE05L, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    printf("[DVBS]: DBG6: =========================: 0x%x\n",u16Data);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_SWUSE06H, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_SWUSE06L, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    printf("[DVBS]: EQ Sum: =======================: 0x%x\n",u16Data);
//---------------------------------------------------------
//FIQ Status
    printf("------------------------------------------------------------------------\n");
    printf("Debug Message [FIQ Status]==============================================\n");
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_SWUSE00H, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_SWUSE00L, &u8Data);
    u16Data = (u16Data<<8)|u8Data;

    if ((u16Data&0x0001)==0x0000)
        printf("[DVBS]: AGC Lock ======================: Fail. \n");
    else
        printf("[DVBS]: AGC Lock ======================: OK. \n");

    if ((u16Data&0x0002)==0x0000)
        printf("[DVBS]: Hum Detect ====================: Fail. \n");
    else
        printf("[DVBS]: Hum Detect ====================: OK. \n");

    if ((u16Data&0x0004)==0x0000)
        printf("[DVBS]: DCR Lock ======================: Fail. \n");
    else
        printf("[DVBS]: DCR Lock ======================: OK. \n");

    if ((u16Data&0x0008)==0x0000)
        printf("[DVBS]: IIS Detect ====================: Fail. \n");
    else
        printf("[DVBS]: IIS Detect ====================: OK. \n");

    if ((u16Data&0x0010)==0x0000)
        printf("[DVBS]: DAGC0 Lock ====================: Fail. \n");
    else
        printf("[DVBS]: DAGC0 Lock ====================: OK. \n");

    if ((u16Data&0x0020)==0x0000)
        printf("[DVBS]: DAGC1 Lock ====================: Fail. \n");
    else
        printf("[DVBS]: DAGC1 Lock ====================: OK. \n");

    if ((u16Data&0x0040)==0x0000)
        printf("[DVBS]: DAGC2 Lock ====================: Fail. \n");
    else
        printf("[DVBS]: DAGC2 Lock ====================: OK. \n");

    if ((u16Data&0x0080)==0x0000)
        printf("[DVBS]: CCI Detect ====================: Fail. \n");
    else
        printf("[DVBS]: CCI Detect ====================: OK. \n");

    if ((u16Data&0x0100)==0x0000)
        printf("[DVBS]: SRD Coarse Done ===============: Fail. \n");
    else
        printf("[DVBS]: SRD Coarse Done ===============: OK. \n");

    if ((u16Data&0x0200)==0x0000)
        printf("[DVBS]: SRD Fine Done =================: Fail. \n");
    else
        printf("[DVBS]: SRD Fine Done =================: OK. \n");

    if ((u16Data&0x0400)==0x0000)
        printf("[DVBS]: EQ Lock =======================: Fail. \n");
    else
        printf("[DVBS]: EQ Lock =======================: OK. \n");

    if ((u16Data&0x0800)==0x0000)
        printf("[DVBS]: FineFE Done ===================: Fail. \n");
    else
        printf("[DVBS]: FineFE Done ===================: OK. \n");

    if ((u16Data&0x1000)==0x0000)
        printf("[DVBS]: PR Lock =======================: Fail. \n");
    else
        printf("[DVBS]: PR Lock =======================: OK. \n");

    if ((u16Data&0x2000)==0x0000)
        printf("[DVBS]: Reserved Frame ================: Fail. \n");
    else
        printf("[DVBS]: Reserved Frame ================: OK. \n");

    if ((u16Data&0x4000)==0x0000)
        printf("[DVBS]: Dummy Frame ===================: Fail. \n");
    else
        printf("[DVBS]: Dummy Frame ===================: OK. \n");

    if ((u16Data&0x8000)==0x0000)
        printf("[DVBS]: PLSC Done =====================: Fail. \n");
    else
        printf("[DVBS]: PLSC Done =====================: OK. \n");

    printf("------------------------------------------------------------------------\n");
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_SWUSE01H, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_SWUSE01L, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    if ((u16Data&0x0001)==0x0000)
        printf("[DVBS]: FS Get Info From Len ==========: Fail. \n");
    else
        printf("[DVBS]: FS Get Info From Len ==========: OK. \n");

    if ((u16Data&0x0002)==0x0000)
        printf("[DVBS]: IQ Swap Detect ================: Fail. \n");
    else
        printf("[DVBS]: IQ Swap Detect ================: OK. \n");

    if ((u16Data&0x0004)==0x0000)
        printf("[DVBS]: FS Acquisition ================: Fail. \n");
    else
        printf("[DVBS]: FS Acquisition ================: OK. \n");

    if ((u16Data&0x0008)==0x0000)
        printf("[DVBS]: TR Lock =======================: Fail. \n");
    else
        printf("[DVBS]: TR Lock =======================: OK. \n");

    if ((u16Data&0x0010)==0x0000)
        printf("[DVBS]: CLCFE Lock ====================: Fail. \n");
    else
        printf("[DVBS]: CLCFE Lock ====================: OK. \n");

    if ((u16Data&0x0020)==0x0000)
        printf("[DVBS]: OLCFE Lock ====================: Fail. \n");
    else
        printf("[DVBS]: OLCFE Lock ====================: OK. \n");

    if ((u16Data&0x0040)==0x0000)
        printf("[DVBS]: Fsync Found ===================: Fail. \n");
    else
        printf("[DVBS]: Fsync Found ===================: OK. \n");

    if ((u16Data&0x0080)==0x0000)
        printf("[DVBS]: Fsync Lock ====================: Fail. \n");
    else
        printf("[DVBS]: Fsync Lock ====================: OK. \n");

    if ((u16Data&0x0100)==0x0000)
        printf("[DVBS]: Fsync Fail Search =============: Fail. \n");
    else
        printf("[DVBS]: Fsync Fail Search =============: OK. \n");

    if ((u16Data&0x0200)==0x0000)
        printf("[DVBS]: Fsync Fail Lock ===============: Fail. \n");
    else
        printf("[DVBS]: Fsync Fail Lock ===============: OK. \n");

    if ((u16Data&0x0400)==0x0000)
        printf("[DVBS]: False Alarm ===================: Fail. \n");
    else
        printf("[DVBS]: False Alarm ===================: OK. \n");

    if ((u16Data&0x0800)==0x0000)
        printf("[DVBS]: Viterbi In Sync ===============: Fail. \n");
    else
        printf("[DVBS]: Viterbi In Sync ===============: OK. \n");

    if ((u16Data&0x1000)==0x0000)
        printf("[DVBS]: Uncrt Over ====================: Fail. \n");
    else
        printf("[DVBS]: Uncrt Over ====================: OK. \n");

    if ((u16Data&0x2000)==0x0000)
        printf("[DVBS]: CLK Cnt Over ==================: Fail. \n");
    else
        printf("[DVBS]: CLK Cnt Over ==================: OK. \n");

    //if ((u16Data&0x4000)==0x0000)
    //    printf("[DVBS]: Data In Ready FIFO ============: Fail. \n");
    //else
    //    printf("[DVBS]: Data In Ready FIFO ============: OK. \n");

    //if ((u16Data&0x8000)==0x0000)
    //    printf("[DVBS]: IIR Buff Busy =================: Fail. \n");
    //else
    //    printf("[DVBS]: IIR Buff Busy =================: OK. \n");

    /*
    printf("------------------------------------------------------------------------\n");
    u16Address = 0x0B64;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(u16Address+1, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(u16Address , &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    if ((u16Data&0x0001)==0x0000)
        printf("[DVBS]: IIR Busy LDPC =================: Fail. \n");
    else
        printf("[DVBS]: IIR Busy LDPC =================: OK. \n");

    if ((u16Data&0x0002)==0x0000)
        printf("[DVBS]: BCH Busy ======================: Fail. \n");
    else
        printf("[DVBS]: BCH Busy ======================: OK. \n");

    if ((u16Data&0x0004)==0x0000)
        printf("[DVBS]: Oppro Ready Out ===============: Fail. \n");
    else
        printf("[DVBS]: Oppro Ready Out ===============: OK. \n");

    if ((u16Data&0x0008)==0x0000)
        printf("[DVBS]: LDPC Win ======================: Fail. \n");
    else
        printf("[DVBS]: LDPC Win ======================: OK. \n");

    if ((u16Data&0x0010)==0x0000)
        printf("[DVBS]: LDPC Error ====================: Fail. \n");
    else
        printf("[DVBS]: LDPC Error ====================: OK. \n");

    if ((u16Data&0x0020)==0x0000)
        printf("[DVBS]: Out BCH Error =================: Fail. \n");
    else
        printf("[DVBS]: Out BCH Error =================: OK. \n");

    if ((u16Data&0x0040)==0x0000)
        printf("[DVBS]: Descr BCH FEC Num Error =======: Fail. \n");
    else
        printf("[DVBS]: Descr BCH FEC Num Error =======: OK. \n");

    if ((u16Data&0x0080)==0x0000)
        printf("[DVBS]: Descr BCH Data Num Error ======: Fail. \n");
    else
        printf("[DVBS]: Descr BCH Data Num Error ======: OK. \n");

    if ((u16Data&0x0100)==0x0000)
        printf("[DVBS]: Packet Error Out ==============: Fail. \n");
    else
        printf("[DVBS]: Packet Error Out ==============: OK. \n");

    if ((u16Data&0x0200)==0x0000)
        printf("[DVBS]: BBH CRC Error =================: Fail. \n");
    else
        printf("[DVBS]: BBH CRC Error =================: OK. \n");

    if ((u16Data&0x0400)==0x0000)
        printf("[DVBS]: BBH Decode Done ===============: Fail. \n");
    else
        printf("[DVBS]: BBH Decode Done ===============: OK. \n");

    if ((u16Data&0x0800)==0x0000)
        printf("[DVBS]: ISRC Calculate Done ===========: Fail. \n");
    else
        printf("[DVBS]: ISRC Calculate Done ===========: OK. \n");

    if ((u16Data&0x1000)==0x0000)
        printf("[DVBS]: Syncd Check Error =============: Fail. \n");
    else
        printf("[DVBS]: Syncd Check Error =============: OK. \n");

    //if ((u16Data&0x2000)==0x0000)
    //      printf("[DVBS]: Syncd Check Error======: Fail. \n");
    //else
    //      printf("[DVBS]: Syncd Check Error======: OK. \n");

    if ((u16Data&0x4000)==0x0000)
        printf("[DVBS]: Demap Init ====================: Fail. \n");
    else
        printf("[DVBS]: Demap Init ====================: OK. \n");
    */
//Spectrum Information
    printf("------------------------------------------------------------------------\n");

    u16Address = 0x2836;
    status &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    psd_smooth_factor=(u16Data>>8)&0x7F;

    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE12H, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE12L, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    u32temp5=u16Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE13H, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE13L, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    u32temp5|=(u16Data<<16);
    if (psd_smooth_factor!=0)
        srd_left_top_value=(float)u32temp5/256.0/psd_smooth_factor;
    else
        srd_left_top_value=0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE14H, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE14L, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    u32temp5=u16Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE15H, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE15L, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    u32temp5|=(u16Data<<16);
    if (psd_smooth_factor!=0)
        srd_left_bottom_value=(float)u32temp5/256.0/psd_smooth_factor;
    else
        srd_left_bottom_value=0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE16H, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE16L, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    u32temp5=u16Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE17H, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE17L, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    u32temp5|=(u16Data<<16);
    if (psd_smooth_factor!=0)
        srd_right_top_value=(float)u32temp5/256.0/psd_smooth_factor;
    else
        srd_right_top_value=0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE18H, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE18L, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    u32temp5=u16Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE19H, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE19L, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    u32temp5|=(u16Data<<16);
    if (psd_smooth_factor!=0)
        srd_right_bottom_value=(float)u32temp5/256.0/psd_smooth_factor;
    else
        srd_right_bottom_value=0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE1AH, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE1AL, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    srd_left=u16Data;
    printf("[DVBS]: FFT Left ======================: %d, %f\n", srd_left, srd_left_top_value - srd_left_bottom_value);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE1BH, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE1BL, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    srd_right=u16Data;
    printf("[DVBS]: FFT Right =====================: %d, %f\n", srd_right, srd_right_top_value - srd_right_bottom_value);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE1CH, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE1CL, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    srd_left_top=u16Data;
    printf("[DVBS]: FFT Left Top ==================: %d, %f\n", srd_left_top, srd_left_top_value);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE1DH, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE1DL, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    srd_left_bottom=u16Data;
    printf("[DVBS]: FFT Left Bottom ===============: %d, %f\n", srd_left_bottom, srd_left_bottom_value);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE1EH, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE1EL, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    srd_right_top=u16Data;
    printf("[DVBS]: FFT Right Top =================: %d, %f\n", srd_right_top, srd_right_top_value);
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE1FH, &u8Data);
    u16Data = u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE1FL, &u8Data);
    u16Data = (u16Data<<8)|u8Data;
    srd_right_bottom=u16Data;
    printf("[DVBS]: FFT Right Bottom ==============: %d, %f\n", srd_right_bottom, srd_right_bottom_value);

    printf("-----------------------------------------\n");
    printf("[DVBS]: Left-Bottom ===================: %d\n", srd_left-srd_left_bottom);
    printf("[DVBS]: Left-Top ======================: %d\n", srd_left_top - srd_left);
    printf("[DVBS]: Right-Top =====================: %d\n", srd_right - srd_right_top);
    printf("[DVBS]: Right-Bottom ==================: %d\n", srd_right_bottom - srd_right);

    if (psd_smooth_factor!=0)
    {
        if ((srd_left_top-srd_left_bottom)!=0)
            printf("[DVBS]: Left Slope ====================: %f\n", (srd_left_top_value - srd_left_bottom_value)/(srd_left_top-srd_left_bottom));
        else
            printf("[DVBS]: Left Slope ====================: %f\n", 0.000000);

        if((srd_right_bottom - srd_right_top)!=0)
            printf("[DVBS]: Right Slope ===================: %f\n", (srd_right_top_value - srd_right_bottom_value)/(srd_right_bottom - srd_right_top));
        else
            printf("[DVBS]: Right Slope ===================: %f\n", 0.000000);

        if (((srd_right_top_value - srd_right_bottom_value)!=0)&&((srd_right_bottom - srd_right_top))!=0)
            printf("[DVBS]: Slope Ratio ===================: %f\n", ((srd_left_top_value - srd_left_bottom_value)/(srd_left_top-srd_left_bottom))/((srd_right_top_value - srd_right_bottom_value)/(srd_right_bottom - srd_right_top)));
        else
            printf("[DVBS]: Slope Ratio ===================: %f\n", 0.000000);
    }
    else
    {
        printf("[DVBS]: Left Slope ======================: %d\n", 0);
        printf("[DVBS]: Right Slope =====================: %d\n", 0);
        printf("[DVBS]: Slope Ratio =====================: %d\n", 0);
    }
#endif
    return status;
}

MS_BOOL INTERN_DVBS_Demod_Get_Debug_Info_polling(void)
{
    MS_BOOL bRet = FALSE;
#if 0
    MS_U8                u8Data = 0;
    MS_U16               u16Data = 0;
    MS_U16               u16Address = 0;
    MS_U32               u32DebugInfo_Fb = 0;            //Fb, SymbolRate
    MS_U32               u32DebugInfo_Fs = 96000;        //Fs, 96000k
    float                AGC_IF_Gain;
    float                DAGC0_Gain, DAGC1_Gain, DAGC2_Gain, DAGC3_Gain, DAGC0_Peak_Mean, DAGC1_Peak_Mean, DAGC2_Peak_Mean, DAGC3_Peak_Mean;
    short                AGC_Err, DAGC0_Err, DAGC1_Err, DAGC2_Err, DAGC3_Err;
    float                DCR_Offset_I, DCR_Offset_Q;
    float                FineCFO_loop_input_value, FineCFO_loop_out_value;
    double               FineCFO_loop_ki_value, TR_loop_ki;
    float                PR_in_value, PR_out_value, PR_loop_ki, PR_loopback_ki;
    float                IQB_Phase, IQB_Gain;
    MS_U16               IIS_cnt, ConvegenceLen;
    float                Linear_SNR_dd, SNR_dd_dB, Linear_SNR_da, SNR_da_dB, SNR_nda_dB, Linear_SNR;
    float                Packet_Err, BER;
    float                TR_Indicator_ff, TR_SFO_Converge, Fs_value, Fb_value;
    float                TR_Loop_Output, TR_Loop_Ki, TR_loop_input, TR_tmp0, TR_tmp1, TR_tmp2;
    float                Eq_variance_da, Eq_variance_dd;
    float                ndasnr_ratio, ndasnr_a, ndasnr_ab;
    MS_U16               BitErr, BitErrPeriod;
    MS_BOOL              BEROver;

    //Fb
    bRet &= MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0x60*2, &u8Data);
    //bRet &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_TOP_WR_DBG_90, &u8Data);
    if((u8Data&0x02)==0x00)                                         //Manual Tune
    {
        u32DebugInfo_Fb = 0x0;//_u32CurrentSR;
    }
    else                                                            //Blind Scan
    {
        bRet &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE14H, &u8Data);
        u16Data = u8Data;
        bRet &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE14L, &u8Data);
        u16Data = (u16Data<<8)|u8Data;
        u32DebugInfo_Fb = u16Data;
    }
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("Fs ====================================: %lu [kHz]\n",u32DebugInfo_Fs);
    printf("Fb ====================================: %lu [kHz]\n",u32DebugInfo_Fb);
//---------------------------------------------------------
//Page1-GAIN & DCR
//---------------------------------------------------------
//GAIN
    printf("\n");
    printf("========================================================================\n");
    printf("Debug Message [GAIN & DCR]==============================================\n");

    //Debug select
    u16Address = (DEBUG_SEL_IF_AGC_GAIN>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0xfff0)|DEBUG_SEL_IF_AGC_GAIN)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_FrontendSetFreeze();
    //AGC_IF_GAIN
    u16Address = (DEBUG_OUT_AGC)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    AGC_IF_Gain=u16Data;
    //Unfreeze
    bRet &= INTERN_DVBS_DTV_FrontendUnFreeze();

    AGC_IF_Gain=AGC_IF_Gain/0x8000;     //(16, 15)
    printf("[DVBS]: AGC_IF_Gain ===================: %f\n", AGC_IF_Gain);
//---------------------------------------------------------
    //Debug select
    u16Address = (DEBUG_SEL_DAGC0_GAIN>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0xfff0)|DEBUG_SEL_DAGC0_GAIN)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_FrontendSetFreeze();
    //DAGC0_GAIN
    u16Address = (DEBUG_OUT_DAGC0)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data = (u16Data>>4);
    DAGC0_Gain=(u16Data&0x0fff);
    //Unfreeze
    bRet &= INTERN_DVBS_DTV_FrontendUnFreeze();
//---------------------------------------------------------
    //Debug select
    u16Address = (DEBUG_SEL_DAGC1_GAIN>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0xfff0)|DEBUG_SEL_DAGC1_GAIN)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_FrontendSetFreeze();
    //DAGC1_GAIN
    u16Address = (DEBUG_OUT_DAGC1)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    DAGC1_Gain=(u16Data&0x07ff);
    //Unfreeze
    bRet &= INTERN_DVBS_DTV_FrontendUnFreeze();
//---------------------------------------------------------
    //Debug select
    u16Address = (DEBUG_SEL_DAGC2_GAIN>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0xfff0)|DEBUG_SEL_DAGC2_GAIN)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_FrontendSetFreeze();
    //DAGC2_GAIN
    u16Address = (DEBUG_OUT_DAGC2)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    DAGC2_Gain=(u16Data&0x0fff);
    //Unfreeze
    bRet &= INTERN_DVBS_DTV_FrontendUnFreeze();
//---------------------------------------------------------
    //Debug select
    u16Address = (DEBUG_SEL_DAGC3_GAIN>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0xfff0)|DEBUG_SEL_DAGC3_GAIN)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_FrontendSetFreeze();
    //DAGC3_GAIN
    u16Address = (DEBUG_OUT_DAGC3)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data = (u16Data>>4);
    DAGC3_Gain=(u16Data&0x0fff);
    //Unfreeze
    bRet &= INTERN_DVBS_DTV_FrontendUnFreeze();
//---------------------------------------------------------

    DAGC0_Gain=DAGC0_Gain/0x200; //<12,9>
    DAGC1_Gain=DAGC1_Gain/0x200; //<11,9>
    DAGC2_Gain=DAGC2_Gain/0x200; //<12,9>
    DAGC3_Gain=DAGC3_Gain/0x200; //<12,9>
    printf("[DVBS]: DAGC0_Gain ====================: %f\n", DAGC0_Gain);
    printf("[DVBS]: DAGC1_Gain ====================: %f\n", DAGC1_Gain);
    printf("[DVBS]: DAGC2_Gain ====================: %f\n", DAGC2_Gain);
    printf("[DVBS]: DAGC3_Gain ====================: %f\n", DAGC3_Gain);

//---------------------------------------------------------
//ERROR
    //Debug select
    u16Address = (DEBUG_SEL_AGC_ERR>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0xfff0)|DEBUG_SEL_AGC_ERR)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_FrontendSetFreeze();
    //AGC_ERR
    u16Address = (DEBUG_OUT_AGC)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    AGC_Err=(u16Data&0x03ff);
    //Unfreeze
    bRet &= INTERN_DVBS_DTV_FrontendUnFreeze();

    //Debug select
    u16Address = (DEBUG_SEL_DAGC0_ERR>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0xfff0)|DEBUG_SEL_DAGC0_ERR)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_FrontendSetFreeze();
    //DAGC0_ERR
    u16Address = (DEBUG_OUT_DAGC0)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data = (u16Data>>4);
    DAGC0_Err=(u16Data&0x7fff);
    //Unfreeze
    bRet &= INTERN_DVBS_DTV_FrontendUnFreeze();

    //Debug select
    u16Address = (DEBUG_SEL_DAGC1_ERR>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0xfff0)|DEBUG_SEL_DAGC1_ERR)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_FrontendSetFreeze();
    //DAGC1_ERR
    u16Address = (DEBUG_OUT_DAGC1)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    DAGC1_Err=(u16Data&0x7fff);
    //Unfreeze
    bRet &= INTERN_DVBS_DTV_FrontendUnFreeze();

    //Debug select
    u16Address = (DEBUG_SEL_DAGC2_ERR>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0xfff0)|DEBUG_SEL_DAGC2_ERR)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_FrontendSetFreeze();
    //DAGC2_ERR
    u16Address = (DEBUG_OUT_DAGC2)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    DAGC2_Err=(u16Data&0x7fff);
    //Unfreeze
    bRet &= INTERN_DVBS_DTV_FrontendUnFreeze();

    //Debug select
    u16Address = (DEBUG_SEL_DAGC3_ERR>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0xfff0)|DEBUG_SEL_DAGC3_ERR)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_FrontendSetFreeze();
    //DAGC3_ERR
    u16Address = (DEBUG_OUT_DAGC3)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data = (u16Data>>4);
    DAGC3_Err=(u16Data&0x7fff);
    //Unfreeze
    bRet &= INTERN_DVBS_DTV_FrontendUnFreeze();

    if (AGC_Err>=0x200)
        AGC_Err=AGC_Err-0x400;
    if (DAGC0_Err>=0x4000)
        DAGC0_Err=DAGC0_Err-0x8000;
    if (DAGC1_Err>=0x4000)
        DAGC1_Err=DAGC1_Err-0x8000;
    if (DAGC2_Err>=0x4000)
        DAGC2_Err=DAGC2_Err-0x8000;
    if (DAGC3_Err>=0x4000)
        DAGC3_Err=DAGC3_Err-0x8000;

    printf("[DVBS]: AGC_Err =========================: %.3f\n", (float)AGC_Err);
    printf("[DVBS]: DAGC0_Err =======================: %.3f\n", (float)DAGC0_Err);
    printf("[DVBS]: DAGC1_Err =======================: %.3f\n", (float)DAGC1_Err);
    printf("[DVBS]: DAGC2_Err =======================: %.3f\n", (float)DAGC2_Err);
    printf("[DVBS]: DAGC3_Err =======================: %.3f\n", (float)DAGC3_Err);
//---------------------------------------------------------
//PEAK_MEAN
    //Debug select
    u16Address = (DEBUG_SEL_DAGC0_PEAK_MEAN>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0xfff0)|DEBUG_SEL_DAGC0_PEAK_MEAN)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_FrontendSetFreeze();
    //DAGC0_PEAK_MEAN
    u16Address = (DEBUG_OUT_DAGC0)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data = (u16Data>>4);
    DAGC0_Peak_Mean=(u16Data&0x0fff);
    //Unfreeze
    bRet &= INTERN_DVBS_DTV_FrontendUnFreeze();

    //Debug select
    u16Address = (DEBUG_SEL_DAGC1_PEAK_MEAN>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0xfff0)|DEBUG_SEL_DAGC1_PEAK_MEAN)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_FrontendSetFreeze();
    //DAGC1_PEAK_MEAN
    u16Address = (DEBUG_OUT_DAGC1)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    DAGC1_Peak_Mean=(u16Data&0x0fff);
    //Unfreeze
    bRet &= INTERN_DVBS_DTV_FrontendUnFreeze();

    //Debug select
    u16Address = (DEBUG_SEL_DAGC2_PEAK_MEAN>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0xfff0)|DEBUG_SEL_DAGC2_PEAK_MEAN)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_FrontendSetFreeze();
    //DAGC2_PEAK_MEAN
    u16Address = (DEBUG_OUT_DAGC2)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    DAGC2_Peak_Mean=(u16Data&0x0fff);
    //Unfreeze
    bRet &= INTERN_DVBS_DTV_FrontendUnFreeze();

    //Debug select
    u16Address = (DEBUG_SEL_DAGC3_PEAK_MEAN>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0xfff0)|DEBUG_SEL_DAGC3_PEAK_MEAN)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_FrontendSetFreeze();
    //DAGC3_PEAK_MEAN
    u16Address = (DEBUG_OUT_DAGC3)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data = (u16Data>>4);
    DAGC3_Peak_Mean=(u16Data&0x0fff);
    //Unfreeze
    bRet &= INTERN_DVBS_DTV_FrontendUnFreeze();


    DAGC0_Peak_Mean = DAGC0_Peak_Mean / 0x800;  //<12,11>
    DAGC1_Peak_Mean = DAGC1_Peak_Mean / 0x800;  //<12,11>
    DAGC2_Peak_Mean = DAGC2_Peak_Mean / 0x800;  //<12,11>
    DAGC3_Peak_Mean = DAGC3_Peak_Mean / 0x800;  //<12,11>

    printf("[DVBS]: DAGC0_Peak_Mean ===============: %f\n", DAGC0_Peak_Mean);
    printf("[DVBS]: DAGC1_Peak_Mean ===============: %f\n", DAGC1_Peak_Mean);
    printf("[DVBS]: DAGC2_Peak_Mean ===============: %f\n", DAGC2_Peak_Mean);
    printf("[DVBS]: DAGC3_Peak_Mean ===============: %f\n", DAGC3_Peak_Mean);
//---------------------------------------------------------
    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_FrontendSetFreeze();

    u16Address = (DCR_OFFSET)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);

    DCR_Offset_I=(u16Data&0xff);
    if (DCR_Offset_I >= 0x80)
        DCR_Offset_I = DCR_Offset_I-0x100;
    DCR_Offset_I = DCR_Offset_I/0x80;

    DCR_Offset_Q=(u16Data>>8)&0xff;
    if (DCR_Offset_Q >= 0x80)
        DCR_Offset_Q = DCR_Offset_Q-0x100;
    DCR_Offset_Q = DCR_Offset_Q/0x80;

    //Unfreeze
    bRet &= INTERN_DVBS_DTV_FrontendUnFreeze();

    printf("[DVBS]: DCR_Offset_I ==================: %f\n", DCR_Offset_I);
    printf("[DVBS]: DCR_Offset_Q ==================: %f\n", DCR_Offset_Q);
//---------------------------------------------------------
////Page1-FineCFO & PR & IIS & IQB
//---------------------------------------------------------
//FineCFO
    printf("------------------------------------------------------------------------\n");
    printf("Debug Message [FineCFO & PR & IIS & IQB & SNR Status]===================\n");
    //Debug Select
    u16Address = INNER_DEBUG_SEL;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=((u16Data&0xC0FF)|0x0400);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_InnerSetFreeze();

    u16Address = INNEREXT_FINEFE_DBG_OUT0;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    FineCFO_loop_out_value=u16Data;
    u16Address = INNEREXT_FINEFE_DBG_OUT2;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    FineCFO_loop_out_value=(FineCFO_loop_out_value+(float)u16Data*pow(2.0, 16));

    //Too large.Use 10Bit
    u16Address = INNEREXT_FINEFE_KI_FF0;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    FineCFO_loop_ki_value=u16Data;
    u16Address = INNEREXT_FINEFE_KI_FF2;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    FineCFO_loop_ki_value=(FineCFO_loop_ki_value+(float)u16Data*pow(2.0, 16));
    u16Address = INNEREXT_FINEFE_KI_FF4;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    FineCFO_loop_ki_value=(FineCFO_loop_ki_value+(double)(u16Data&0x00FF)*pow(2.0, 32));
    //Unfreeze
    bRet &= INTERN_DVBS_DTV_InnerUnFreeze();

//---------------------------------------------------------
    //Debug Select
    u16Address = INNER_DEBUG_SEL;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=((u16Data&0xC0FF)|0x0100);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_InnerSetFreeze();

    u16Address = INNEREXT_FINEFE_DBG_OUT0;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    FineCFO_loop_input_value=u16Data;
    u16Address = INNEREXT_FINEFE_DBG_OUT2;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    FineCFO_loop_input_value=(FineCFO_loop_input_value+(float)u16Data*pow(2.0, 16));

    //Unfreeze
    bRet &= INTERN_DVBS_DTV_InnerUnFreeze();

    FineCFO_loop_ki_value = FineCFO_loop_ki_value/1024;

    if (FineCFO_loop_out_value > 8388608)
        FineCFO_loop_out_value=FineCFO_loop_out_value - 16777216;
    if (FineCFO_loop_ki_value > 536870912)//549755813888/1024)
        FineCFO_loop_ki_value=FineCFO_loop_ki_value - 1073741824;//1099511627776/1024;
    if (FineCFO_loop_input_value> 1048576)
        FineCFO_loop_input_value=FineCFO_loop_input_value - 2097152;

    FineCFO_loop_out_value = ((float)FineCFO_loop_out_value/16777216);
    FineCFO_loop_ki_value = ((double)FineCFO_loop_ki_value/67108864*u32DebugInfo_Fb);//68719476736/1024*Fb
    FineCFO_loop_input_value = ((float)FineCFO_loop_input_value/2097152);

    printf("[DVBS]: FineCFO_loop_out_value ========: %f \n", FineCFO_loop_out_value);
    printf("[DVBS]: FineCFO_loop_ki_value =========: %f \n", FineCFO_loop_ki_value);
    printf("[DVBS]: FineCFO_loop_input_value ======: %f \n", FineCFO_loop_input_value);

//---------------------------------------------------------
//Phase Recovery
    //Debug select
    u16Address = INNER_DEBUG_SEL;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0x00FF)|0x0600)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_InnerSetFreeze();

    u16Address = INNER_PR_DEBUG_OUT0;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    PR_out_value=u16Data;
    if (PR_out_value>=0x1000)
        PR_out_value=PR_out_value-0x2000;

    //Unfreeze
    bRet &= INTERN_DVBS_DTV_InnerUnFreeze();
//---------------------------------------------------------
    //Debug select
    u16Address = INNER_DEBUG_SEL;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0x00FF)|0x0100)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_InnerSetFreeze();

    u16Address = INNER_PR_DEBUG_OUT0;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    PR_in_value=u16Data;
    u16Address = INNER_PR_DEBUG_OUT2;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    PR_in_value=(((u16Data&0x000F)<<16)|(MS_U16)PR_in_value);
    if (PR_in_value>=0x80000)
        PR_in_value=PR_in_value-0x100000;

    //Unfreeze
    bRet &= INTERN_DVBS_DTV_InnerUnFreeze();
//---------------------------------------------------------
    //Debug select
    u16Address = INNER_DEBUG_SEL;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0xC0FF)|0x0400)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_InnerSetFreeze();

    u16Address = INNER_PR_DEBUG_OUT0;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    PR_loop_ki=u16Data;
    u16Address = INNER_PR_DEBUG_OUT2;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    PR_loop_ki=(((u16Data&0x00FF)<<16)+PR_loop_ki);
    if (PR_loop_ki>=0x800000)
        PR_loop_ki=PR_loop_ki-0x1000000;

    //Unfreeze
    bRet &= INTERN_DVBS_DTV_InnerUnFreeze();
//---------------------------------------------------------
    //Debug select
    u16Address = INNER_DEBUG_SEL;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0x00FF)|0x0500)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_InnerSetFreeze();

    u16Address = INNER_PR_DEBUG_OUT0;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    PR_loopback_ki=u16Data;
    u16Address = INNER_PR_DEBUG_OUT2;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    PR_loopback_ki=(((u16Data&0x00FF)<<16)+PR_loopback_ki);
    if (PR_loopback_ki>=0x800000)
        PR_loopback_ki=PR_loopback_ki-0x1000000;

    //Unfreeze
    bRet &= INTERN_DVBS_DTV_InnerUnFreeze();

    PR_out_value = ((float)PR_out_value/4096);
    PR_in_value = ((float)PR_in_value/131072);
    PR_loop_ki = ((float)PR_loop_ki/67108864*u32DebugInfo_Fb);
    PR_loopback_ki = ((float)PR_loopback_ki/67108864*u32DebugInfo_Fb);

    printf("[DVBS]: PR_out_value ==================: %f\n", PR_out_value);
    printf("[DVBS]: PR_in_value ===================: %f\n", PR_in_value);
    printf("[DVBS]: PR_loop_ki ====================: %f\n", PR_loop_ki);
    printf("[DVBS]: PR_loopback_ki ================: %f\n", PR_loopback_ki);
//---------------------------------------------------------
//IIS
    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_FrontendSetFreeze();

    u16Address = (IIS_COUNT0)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    IIS_cnt=u16Data;
    u16Address = (IIS_COUNT2)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    IIS_cnt=(u16Data&0x1f)<<16|IIS_cnt;

    printf("[DVBS]: IIS_cnt =======================: %d\n", IIS_cnt);

    //Unfreeze
    bRet &= INTERN_DVBS_DTV_FrontendUnFreeze();
//IQB
    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_FrontendSetFreeze();

    u16Address = (IQB_PHASE)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    IQB_Phase=u16Data&0x3FF;
    if (IQB_Phase>=0x200)
        IQB_Phase=IQB_Phase-0x400;
    IQB_Phase=IQB_Phase/0x400*180;

    u16Address = (IQB_GAIN)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    IQB_Gain=u16Data&0x7FF;
    IQB_Gain=IQB_Gain/0x400;

    printf("[DVBS]: IQB_Phase =====================: %f\n", IQB_Phase);
    printf("[DVBS]: IQB_Gain ======================: %f\n", IQB_Gain);

    //Unfreeze
    bRet &= INTERN_DVBS_DTV_FrontendUnFreeze();
//---------------------------------------------------------
//SNR
    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_InnerSetFreeze();

    Eq_variance_da=0;
    u16Address = 0x249E;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    Eq_variance_da=u16Data;
    u16Address = 0x24A0;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    Eq_variance_da=((float)(u16Data&0x03fff)*pow(2.0, 16)+Eq_variance_da)/pow(2.0, 29);

    if (Eq_variance_da==0)
        Eq_variance_da=1;
    Linear_SNR_da=1.0/Eq_variance_da;
    SNR_da_dB=10*log10(Linear_SNR_da);

    Eq_variance_dd=0;
    u16Address = 0x24A2;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    Eq_variance_dd=u16Data;
    u16Address = 0x24A4;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    Eq_variance_dd=(((float)(u16Data&0x3fff)*65536)+Eq_variance_dd)/pow(2.0, 29);

    if (Eq_variance_dd==0)
        Eq_variance_dd=1;
    Linear_SNR_dd=1.0/Eq_variance_dd;
    SNR_dd_dB=10*log10(Linear_SNR_dd);

    ndasnr_a=0;
    u16Address = 0x248C;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    ndasnr_a=u16Data;
    u16Address = 0x248E;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    ndasnr_a=(((float)(u16Data&0x0003)*pow(2.0, 16))+ndasnr_a)/65536;

    ndasnr_ab=0;
    u16Address = 0x2490;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    ndasnr_ab=u16Data;
    u16Address = 0x2492;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    ndasnr_ab=(((float)(u16Data&0x03ff)*pow(2.0, 16))+ndasnr_ab)/4194304;

    ndasnr_ab=sqrt(ndasnr_ab);
    if (ndasnr_ab==0)
        ndasnr_ab=1;
    ndasnr_ratio=(float)ndasnr_a/ndasnr_ab;
    if (ndasnr_ratio> 1)
        SNR_nda_dB=10*log10(1/(ndasnr_ratio - 1));
    else
        SNR_nda_dB=0;

    u16Address = 0x24BA;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    Linear_SNR=u16Data;
    u16Address = 0x24BC;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    Linear_SNR=(((float)(u16Data&0x0007)*pow(2.0, 16))+Linear_SNR)/64;
    if (Linear_SNR==0)
        Linear_SNR=1;
    Linear_SNR=10*log10(Linear_SNR);

    //Unfreeze
    bRet &= INTERN_DVBS_DTV_InnerUnFreeze();
    printf("[DVBS]: SNR ===========================: %.2f\n", Linear_SNR);
    printf("[DVBS]: SNR_DA_dB =====================: %.2f\n", SNR_da_dB);
    printf("[DVBS]: SNR_DD_dB =====================: %.2f\n", SNR_dd_dB);
    printf("[DVBS]: SNR_NDA_dB ====================: %.2f\n", SNR_nda_dB);
//---------------------------------------------------------
    printf("------------------------------------------------------------------------\n");
    printf("Debug Message [DVBS - PacketErr & BER]==================================\n");
//BER
    //freeze
    u16Address = 0x2103;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=u16Data|0x0001;
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    // bank 17 0x18 [7:0] reg_bit_err_sblprd_7_0  [15:8] reg_bit_err_sblprd_15_8
    u16Address = 0x2166;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    Packet_Err=u16Data;

    printf("[DVBS]: Packet Err ====================: %.3E\n", Packet_Err);

    /////////// Post-Viterbi BER /////////////
    // bank 7 0x18 [7:0] reg_bit_err_sblprd_7_0
    //             [15:8] reg_bit_err_sblprd_15_8
    u16Address = 0x2146;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    BitErrPeriod=u16Data;

    // bank 17 0x1D [7:0] reg_bit_err_num_7_0   [15:8] reg_bit_err_num_15_8
    // bank 17 0x1E [7:0] reg_bit_err_num_23_16 [15:8] reg_bit_err_num_31_24
    u16Address = 0x216A;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    BitErr=u16Data;
    u16Address = 0x216C;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    BitErr=(u16Data<<16)|BitErr;

    if (BitErrPeriod ==0 )//protect 0
        BitErrPeriod=1;
    if (BitErr <=0 )
        BER=0.5 / (float)(BitErrPeriod*128*188*8);
    else
        BER=(float)(BitErr) / (float)(BitErrPeriod*128*188*8);

    printf("[DVBS]: Post-Viterbi BER ==============: %.3E\n", BER);

    // bank 7 0x19 [7] reg_bit_err_num_freeze
    u16Address = 0x2103;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=u16Data&(~0x0001);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    /////////// Pre-Viterbi BER /////////////
    // bank 17 0x08 [3] reg_rd_freezeber
    u16Address = 0x2110;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=u16Data|0x0008;
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    // bank 17 0x0b [7:0] reg_ber_timerl  [15:8] reg_ber_timerm
    // bank 17 0x0c [5:0] reg_ber_timerh
    u16Address = 0x2116;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    BitErrPeriod=u16Data;
    u16Address = 0x2118;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    BitErrPeriod=((u16Data&0x3f)<<16)|BitErrPeriod;

    // bank 17 0x0f [7:0] reg_ber_7_0  [15:8] reg_ber_15_8
    u16Address = 0x211E;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    BitErr=u16Data;

    // bank 17 0x0D [13:8] reg_cor_intstat_reg
    u16Address = 0x211A;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    if (u16Data & 0x1000)
    {
        BEROver = true;
    }
    else
    {
        BEROver = false;
    }

    if (BitErrPeriod ==0 )//protect 0
        BitErrPeriod=1;
    if (BitErr <=0 )
        BER=0.5 / (float)(BitErrPeriod) / 256;
    else
        BER=(float)(BitErr) / (float)(BitErrPeriod) / 256;
    printf("[DVBS]: Pre-Viterbi BER ===============: %.3E\n", BER);

    // bank 17 0x08 [3] reg_rd_freezeber
    u16Address = 0x2110;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=u16Data&(~0x0008);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    u16Address = 0x2188;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    ConvegenceLen = ((u16Data>>8)&0xFF);
    printf("[DVBS]: ConvegenceLen =================: %d\n", ConvegenceLen);

//---------------------------------------------------------
//Timing Recovery
    //Debug select
    u16Address = (INNER_DEBUG_SEL_TR>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0x00ff)|INNER_DEBUG_SEL_TR)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_InnerSetFreeze();

    u16Address = (TR_INDICATOR_FF0)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    TR_Indicator_ff=u16Data;
    u16Address = (TR_INDICATOR_FF0)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    TR_Indicator_ff=((u16Data<<16) | (MS_U16)TR_Indicator_ff)&0x7fffff;
    if (TR_Indicator_ff >= 0x400000)
        TR_Indicator_ff=TR_Indicator_ff - 0x800000;

    //Unfreeze
    bRet &= INTERN_DVBS_DTV_InnerUnFreeze();

    //Debug select
    u16Address = (DEBUG_SEL_TR_SFO_CONVERGE>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0x00ff)|DEBUG_SEL_TR_SFO_CONVERGE)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_InnerSetFreeze();

    u16Address = (TR_INDICATOR_FF0)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    TR_SFO_Converge=u16Data;
    u16Address = (TR_INDICATOR_FF0)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    TR_SFO_Converge=((u16Data<<16) | (MS_U16)TR_SFO_Converge)&0x7fffff;
    if (TR_SFO_Converge >= 0x400000)
        TR_SFO_Converge=TR_SFO_Converge - 0x800000;

    u16Address = INNER_TR_LOPF_VALUE_DEBUG0;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    TR_loop_ki=u16Data;
    u16Address = INNER_TR_LOPF_VALUE_DEBUG2;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    TR_loop_ki=((float)u16Data*pow(2.0, 16))+TR_loop_ki;
    u16Address = INNER_TR_LOPF_VALUE_DEBUG4;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    TR_loop_ki=(((double)(u16Data&0x01ff)*pow(2.0, 32))+ TR_loop_ki);
    if (TR_loop_ki>=pow(2.0, 40))
        TR_loop_ki=TR_loop_ki-pow(2.0, 41);

    //Unfreeze
    bRet &= INTERN_DVBS_DTV_InnerUnFreeze();

    //Debug select
    u16Address = (DEBUG_SEL_TR_INPUT>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data=(((u16Data&0x00ff)|DEBUG_SEL_TR_INPUT)&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    //Freeze and dump
    bRet &= INTERN_DVBS_DTV_InnerSetFreeze();

    u16Address = (TR_INDICATOR_FF0)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    TR_loop_input=u16Data;
    //banknum=(TR_INDICATOR_FF1>>8)&0xff;
    //addr=(TR_INDICATOR_FF1)&0xff;
    //if(InformRead(banknum, addr, &data)==FALSE) return;
    //TR_loop_input=((float)((data&0x00ff)<<16) + TR_loop_input);
    if (TR_loop_input >= 0x8000)
        TR_loop_input=TR_loop_input - 0x10000;

    //Unfreeze
    bRet &= INTERN_DVBS_DTV_InnerUnFreeze();

    Fs_value=u32DebugInfo_Fs;
    Fb_value=u32DebugInfo_Fb;
    TR_tmp0=(float)TR_SFO_Converge/0x200000;
    TR_tmp2=TR_loop_ki/pow(2.0, 39);
    TR_tmp1=(float)Fs_value/2/Fb_value;

    TR_Indicator_ff = (TR_Indicator_ff/0x400);
    TR_Loop_Output = (TR_tmp0/TR_tmp1*1000000);
    TR_Loop_Ki = (TR_tmp2/TR_tmp1*1000000);
    TR_loop_input = (TR_loop_input/0x8000);

    printf("[DVBS]: TR_Indicator_ff================: %f \n", TR_Indicator_ff);
    printf("[DVBS]: TR_Loop_Output=================: %f [ppm]\n", TR_Loop_Output);
    printf("[DVBS]: TR_Loop_Ki=====================: %f [ppm]\n", TR_Loop_Ki);
    printf("[DVBS]: TR_loop_input==================: %f \n", TR_loop_input);
#endif
    bRet=true;
    return bRet;
}

//------------------------------------------------------------------
//  END Get And Show Info Function
//------------------------------------------------------------------

//------------------------------------------------------------------
//  BlindScan Function
//------------------------------------------------------------------
MS_BOOL INTERN_DVBS_BlindScan_Start(MS_U16 u16StartFreq,MS_U16 u16EndFreq)
{
    MS_BOOL status=TRUE;
    MS_U8 u8Data=0;

    DBG_INTERN_DVBS(printf("MDrv_Demod_BlindScan_Start+\n"));

    _u16BlindScanStartFreq=u16StartFreq;
    _u16BlindScanEndFreq=u16EndFreq;
    _u16TunerCenterFreq=0;
    _u16ChannelInfoIndex=0;

    MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0x60*2, &u8Data);
    u8Data&=0xd0;
    MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE + 0x60*2, u8Data);

    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_TOP_WR_DBG_92, (MS_U8)_u16BlindScanStartFreq&0x00ff);
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_TOP_WR_DBG_93, (MS_U8)(_u16BlindScanStartFreq>>8)&0x00ff);

    DBG_INTERN_DVBS(printf("MDrv_Demod_BlindScan_Start- _u16BlindScanStartFreq%d u16StartFreq %d u16EndFreq %d\n", _u16BlindScanStartFreq, u16StartFreq, u16EndFreq));

    return status;
}

MS_BOOL INTERN_DVBS_BlindScan_NextFreq(MS_BOOL* bBlindScanEnd)
{
    MS_BOOL status=TRUE;
    MS_U8   u8Data=0;

    DBG_INTERN_DVBS(printf("MDrv_Demod_BlindScan_NextFreq+\n"));

    * bBlindScanEnd=FALSE;

    if (_u16TunerCenterFreq >=_u16BlindScanEndFreq)
    {
        DBG_INTERN_DVBS(printf("MDrv_Demod_BlindScan_NextFreq . _u16TunerCenterFreq %d _u16BlindScanEndFreq%d\n", _u16TunerCenterFreq, _u16BlindScanEndFreq));
        * bBlindScanEnd=TRUE;

        return status;
    }
    //Set Tuner Frequency
    MsOS_DelayTask(10);

    MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0x60*2, &u8Data);
    if ((u8Data&0x02)==0x00)//Manual Tune
    {
        u8Data&=~(0x28);
        MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE + 0x60*2, u8Data);
        u8Data|=0x02;
        MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE + 0x60*2, u8Data);
        u8Data|=0x01;
        MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE + 0x60*2, u8Data);
    }
    else
    {
        u8Data&=~(0x28);
        MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE + 0x60*2, u8Data);
    }

    return status;
}

MS_BOOL INTERN_DVBS_BlindScan_GetTunerFreq(MS_U16 *u16TunerCenterFreq, MS_U16 *u16TunerCutOffFreq)
{
    MS_BOOL status=TRUE;
    MS_U8   u8Data=0;
    MS_U16  u16WaitCount;
    MS_U16  u16TunerCutOff;

    DBG_INTERN_DVBS(printf("MDrv_Demod_BlindScan_SetTunerFreq+\n"));

    MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0x60*2, &u8Data);
    if ((u8Data&0x02)==0x02)
    {
        u8Data|=0x08;
        MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE + 0x60*2, u8Data);
        u16WaitCount=0;
        do
        {
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_STATE_FLAG, &u8Data);//SubState
            u16WaitCount++;
            //DBG_INTERN_DVBS(printf("MDrv_Demod_BlindScan_NextFreq u8Data:0x%x u16WaitCount:%d\n", u8Data, u16WaitCount));
            MsOS_DelayTask(1);
            }while((u8Data!=0x01)&&(u16WaitCount<INTERN_DVBS_DEMOD_WAIT_TIMEOUT));
    }
    else if((u8Data&0x01)==0x01)
    {
        u8Data|=0x20;
        MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE + 0x60*2, u8Data);
        u16WaitCount=0;
        do
        {
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_STATE_FLAG, &u8Data);//SubState
            u16WaitCount++;
            //DBG_INTERN_DVBS(printf("MDrv_Demod_BlindScan_NextFreq u8Data:0x%x u16WaitCount:%d\n", u8Data, u16WaitCount));
            MsOS_DelayTask(1);
        }while((u8Data!=0x01)&&(u16WaitCount<INTERN_DVBS_DEMOD_WAIT_TIMEOUT));
        MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0x60*2, &u8Data);
        u8Data|=0x02;
        MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE + 0x60*2, u8Data);
    }
    u16WaitCount=0;

    _u16TunerCenterFreq=0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_TOP_WR_DBG_93, &u8Data);
    //printf("INTERN_DVBS_BlindScan_GetTunerFreq E_DMD_S2_BS_Tuner_Center_Freq_H=%d\n", u8Data);//RRRRR
    _u16TunerCenterFreq=u8Data;
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_TOP_WR_DBG_92, &u8Data);
    //printf("INTERN_DVBS_BlindScan_GetTunerFreq E_DMD_S2_BS_Tuner_Center_Freq_L=%d\n", u8Data);//RRRRR
    _u16TunerCenterFreq=(_u16TunerCenterFreq<<8)|u8Data;

    *u16TunerCenterFreq = _u16TunerCenterFreq;
//claire test
    u16TunerCutOff=44000;
    if(_u16TunerCenterFreq<=990)//980
    {

       status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_BALANCE_TRACK, &u8Data);
       if(u8Data==0x01)
       {
          if(_u16TunerCenterFreq<970)//970
          {
            u16TunerCutOff=10000;
          }
          else
          {
            u16TunerCutOff=20000;
          }
          u8Data=0x02;
          status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_BALANCE_TRACK, u8Data);
       }
       else if(u8Data==0x02)
       {
          u8Data=0x00;
          status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_BALANCE_TRACK, u8Data);
       }
    }
    if(u16TunerCutOffFreq != NULL)
    {
          *u16TunerCutOffFreq = u16TunerCutOff;
    }

//end claire test

    DBG_INTERN_DVBS(printf("MDrv_Demod_BlindScan_SetTunerFreq- _u16TunerCenterFreq:%d\n", _u16TunerCenterFreq));


    return status;
}

MS_BOOL INTERN_DVBS_BlindScan_WaitCurFreqFinished(MS_U8* u8Progress,MS_U8 *u8FindNum)
{
    MS_BOOL status=TRUE;
    MS_U32  u32Data=0;
    MS_U16  u16Data=0;
    MS_U8   u8Data=0, u8Data2=0;
    MS_U16  u16WaitCount;

    DBG_INTERN_DVBS(printf("MDrv_Demod_BlindScan_WaitCurFreqFinished+\n"));

    u16WaitCount=0;
    *u8FindNum=0;
    *u8Progress=0;

    do
    {
        status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_STATE_FLAG, &u8Data);        //State=BlindScan
        status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_BLINDSCAN_CHECK, &u8Data2);    //SubState=BlindScan
        u16WaitCount++;
        DBG_INTERN_DVBS(printf("INTERN_DVBS_BlindScan_WaitCurFreqFinished State: 0x%x Status: 0x%x u16WaitCount:%d\n", u8Data, u8Data2, u16WaitCount));
        //printf("INTERN_DVBS_BlindScan_WaitCurFreqFinished State: 0x%x Status: 0x%x u16WaitCount:%d\n", u8Data, u8Data2, u16WaitCount);

        MsOS_DelayTask(1);
    }while(((u8Data!=17)||(u8Data2!=0xff))&&(u16WaitCount<INTERN_DVBS_DEMOD_WAIT_TIMEOUT));//E_DMD_S2_STATE_FLAG



    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DUMMY_REG_2, &u8Data);
    u16Data=u8Data;


    DBG_INTERN_DVBS(printf("MDrv_Demod_BlindScan_WaitCurFreqFinished OuterCheckStatus:0x%x\n", u16Data));

    if (u16WaitCount>=INTERN_DVBS_DEMOD_WAIT_TIMEOUT)
    {
        status=false;
        printf("Debug blind scan wait finished time out!!!!\n");
    }
    else
    {

        status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_SUBSTATE_FLAG, &u8Data);//SubState
        if (u8Data==0)
        {

            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE13L, &u8Data);
            u32Data=u8Data;
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE12H, &u8Data);
            u32Data=(u32Data<<8)|u8Data;
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE12L, &u8Data);
            u32Data=(u32Data<<8)|u8Data;
            _u16ChannelInfoArray[0][_u16ChannelInfoIndex]=((u32Data+500)/1000);
            _u16LockedCenterFreq=((u32Data+500)/1000);                //Center Freq


            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE14H, &u8Data);
            u16Data=u8Data;
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE14L, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16ChannelInfoArray[1][_u16ChannelInfoIndex]=(u16Data);//Symbol Rate
            _u16LockedSymbolRate=u16Data;
            _u16ChannelInfoIndex++;
            *u8FindNum=_u16ChannelInfoIndex;
            //printf("claire debug blind scan: find TP frequency %d SR %d index %d\n",_u16LockedCenterFreq,_u16LockedSymbolRate,_u16ChannelInfoIndex);


            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE15H, &u8Data);
            u16Data=u8Data;
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE15L, &u8Data);
            u16Data=(u16Data<<8)|u8Data;                            //Center_Freq_Offset_Locked
            if (u16Data*1000 >= 0x8000)
            {
                u16Data=0x10000- u16Data*1000;
                _s16CurrentCFO=-1*u16Data/1000;
            }
            else
            {
                _s16CurrentCFO=u16Data;
            }

            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE16H, &u8Data);
            u16Data=u8Data;
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE16L, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16CurrentStepSize=u16Data;            //Tuner_Frequency_Step


            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE18H, &u8Data);
            u16Data=u8Data;
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE18L, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16PreLockedHB=u16Data;                //Pre_Scanned_HB


            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE19H, &u8Data);
            u16Data=u8Data;
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE19L, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16PreLockedLB=u16Data;                //Pre_Scanned_LB


            DBG_INTERN_DVBS(printf("Current Locked CF:%d BW:%d BWH:%d BWL:%d CFO:%d Step:%d\n", _u16LockedCenterFreq, _u16LockedSymbolRate,_u16PreLockedHB, _u16PreLockedLB, _s16CurrentCFO, _u16CurrentStepSize));
        }
        else if (u8Data==1)
        {
            //printf("claire debug blind scan: no find TP\n");


            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_TOP_WR_DBG_93, &u8Data);
            u16Data=u8Data;
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_TOP_WR_DBG_92, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16NextCenterFreq=u16Data;


            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE12H, &u8Data);
            u16Data=u8Data;
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE12L, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16PreLockedHB=u16Data;            //Pre_Scanned_HB



            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE13H, &u8Data);
            u16Data=u8Data;
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE13L, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16PreLockedLB=u16Data;            //Pre_Scanned_LB


            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE14H, &u8Data);
            u16Data=u8Data;
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE14L, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16CurrentSymbolRate=u16Data;        //Fine_Symbol_Rate


            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE15H, &u8Data);
            u16Data=u8Data;
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE15L, &u8Data);
            u16Data=(u16Data<<8)|u8Data;        //Center_Freq_Offset
            if (u16Data*1000 >= 0x8000)
            {
                u16Data=0x1000- u16Data*1000;
                _s16CurrentCFO=-1*u16Data/1000;
            }
            else
            {
                _s16CurrentCFO=u16Data;
            }

            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE16H, &u8Data);
            u16Data=u8Data;
            status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_SWUSE16L, &u8Data);
            u16Data=(u16Data<<8)|u8Data;
            _u16CurrentStepSize=u16Data;        //Tuner_Frequency_Step


            DBG_INTERN_DVBS(printf("Pre Locked CF:%d BW:%d HBW:%d LBW:%d Current CF:%d BW:%d CFO:%d Step:%d\n", _u16LockedCenterFreq, _u16LockedSymbolRate,_u16PreLockedHB, _u16PreLockedLB,  _u16NextCenterFreq-_u16CurrentStepSize, _u16CurrentSymbolRate, _s16CurrentCFO, _u16CurrentStepSize));
        }
    }
    *u8Progress=100;

    DBG_INTERN_DVBS(printf("MDrv_Demod_BlindScan_WaitCurFreqFinished- u8Progress: %d u8FindNum %d\n", *u8Progress, *u8FindNum));

    return status;
}

MS_BOOL INTERN_DVBS_BlindScan_Cancel(void)
{
    MS_BOOL status=TRUE;
    MS_U8   u8Data=0;
    MS_U16  u16Data;

    DBG_INTERN_DVBS(printf("MDrv_Demod_BlindScan_Cancel+\n"));

    MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0x60*2, &u8Data);
    //status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_TOP_WR_DBG_90, &u8Data);
    u8Data&=0xF0;
    MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE + 0x60*2, u8Data);
    //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_TOP_WR_DBG_90, u8Data);

    //MDrv_SYS_DMD_VD_MBX_WriteReg(0x20E1, 0x00);
    //MDrv_SYS_DMD_VD_MBX_WriteReg(0x20E0, 0x00);
    u16Data = 0x0000;
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_TOP_WR_DBG_92, (MS_U8)u16Data&0x00ff);
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_TOP_WR_DBG_93, (MS_U8)(u16Data>>8)&0x00ff);

    _u16TunerCenterFreq=0;
    _u16ChannelInfoIndex=0;

    DBG_INTERN_DVBS(printf("MDrv_Demod_BlindScan_Cancel-\n"));

    return status;
}

MS_BOOL INTERN_DVBS_BlindScan_End(void)
{
    MS_BOOL status=TRUE;
    MS_U8   u8Data=0;
    MS_U16  u16Data;

    DBG_INTERN_DVBS(printf("MDrv_Demod_BlindScan_End+\n"));

    MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0x60*2, &u8Data);
    //status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_TOP_WR_DBG_90, &u8Data);
    u8Data&=0xF0;
    MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE + 0x60*2, u8Data);
    //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_TOP_WR_DBG_90, u8Data);

    //MDrv_SYS_DMD_VD_MBX_WriteReg(0x20E1, 0x00);
    //MDrv_SYS_DMD_VD_MBX_WriteReg(0x20E0, 0x00);
    u16Data = 0x0000;
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_TOP_WR_DBG_92, (MS_U8)u16Data&0x00ff);
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_TOP_WR_DBG_93, (MS_U8)(u16Data>>8)&0x00ff);

    _u16TunerCenterFreq=0;
    _u16ChannelInfoIndex=0;

    DBG_INTERN_DVBS(printf("MDrv_Demod_BlindScan_End-\n"));

    return status;
}

MS_BOOL INTERN_DVBS_BlindScan_GetChannel(MS_U16 u16ReadStart,MS_U16* u16TPNum,HAL_DEMOD_MS_FE_CARRIER_PARAM *pTable)
{
    MS_BOOL status=TRUE;
    MS_U16  u16TableIndex;

    *u16TPNum=_u16ChannelInfoIndex-u16ReadStart;
    for(u16TableIndex = 0; u16TableIndex < (*u16TPNum); u16TableIndex++)
    {
        pTable[u16TableIndex].u32Frequency = _u16ChannelInfoArray[0][_u16ChannelInfoIndex-1];
        pTable[u16TableIndex].SatParam.u32SymbolRate = _u16ChannelInfoArray[1][_u16ChannelInfoIndex-1];
        DBG_INTERN_DVBS(printf("MDrv_Demod_BlindScan_GetChannel Freq: %d SymbolRate: %d\n", pTable[u16TableIndex].u32Frequency, pTable[u16TableIndex].SatParam.u32SymbolRate));
    }
    DBG_INTERN_DVBS(printf("INTERN_DVBS_u16TPNum:%d\n", *u16TPNum));

    return status;
}

MS_BOOL INTERN_DVBS_BlindScan_GetCurrentFreq(MS_U32 *u32CurrentFeq)
{
    MS_BOOL status=TRUE;
    DBG_INTERN_DVBS(printf("INTERN_DVBS_BlindScan_GetCurrentFreq+\n"));

    *u32CurrentFeq=_u16TunerCenterFreq;
    DBG_INTERN_DVBS(printf("INTERN_DVBS_BlindScan_GetCurrentFreq-: %d\n", _u16TunerCenterFreq));
    return status;
}
//------------------------------------------------------------------
//  END BlindScan Function
//------------------------------------------------------------------

//------------------------------------------------------------------
//  DiSEqc Function
//------------------------------------------------------------------
MS_BOOL INTERN_DVBS_DiSEqC_Init(void)
{
    MS_BOOL status = true;
    MS_U8 u8Data = 0;

    //Clear status
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_REG_BASE + 0xCD, &u8Data);
    u8Data=(u8Data|0x3E)&(~0x3E);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2_REG_BASE + 0xCD, u8Data);

    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2_REG_BASE + 0xC0, 0x00);
    //Tone En
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_REG_BASE + 0xC0, &u8Data);
    u8Data=(u8Data&(~0x06))|(0x06);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2_REG_BASE + 0xC0, u8Data);

    DBG_INTERN_DVBS(printf("INTERN_DVBS_DiSEqC_Init\n"));

    return status;
}

MS_BOOL INTERN_DVBS_DiSEqC_SetTone(MS_BOOL bTone1)
{
    MS_BOOL status=TRUE;
    MS_U8 u8Data=0;
    MS_U8 u8ReSet22k=0;

    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2_REG_BASE + 0xC4, 0x01);//0x62;DVBS2_DISEQC_TX1
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2_REG_BASE + 0xC0, 0x4E);//0x60
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2_REG_BASE + 0xCC, 0x88);//0x66

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_REG_BASE + 0xC2, &u8Data);//0x61
    u8ReSet22k=u8Data;

    if (bTone1==TRUE)
    {
        //Tone burst 1
        status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2_REG_BASE + 0xC2, 0x19);
        _u8ToneBurstFlag=1;
    }
    else
    {
        //Tone burst 0
        status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2_REG_BASE + 0xC2, 0x11);
        _u8ToneBurstFlag=2;
    }
    //DIG_DISEQC_TX_EN
    //status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_REG_BASE + 0xCD, &u8Data);
    //u8Data=u8Data&~(0x01);//Tx Disable
    //status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2_REG_BASE + 0xCD, u8Data);

    MsOS_DelayTask(1);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_REG_BASE + 0xCD, &u8Data);//0x66 high byte DVBS2_DISEQC_TX_EN
    u8Data=u8Data|0x3E;     //Status clear
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2_REG_BASE + 0xCD, u8Data);
    MsOS_DelayTask(10);
    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_REG_BASE + 0xCD, &u8Data);
    u8Data=u8Data&~(0x3E);
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2_REG_BASE + 0xCD, u8Data);
    MsOS_DelayTask(1);

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_REG_BASE + 0xCD, &u8Data);
    u8Data=u8Data|0x01;      //Tx Enable
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2_REG_BASE + 0xCD, u8Data);

    MsOS_DelayTask(30);//(100)
    //For ToneBurst 22k issue.
    u8Data=u8ReSet22k;
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2_REG_BASE + 0xC2, u8Data);//0x61

    DBG_INTERN_DVBS(printf("INTERN_DVBS_DiSEqC_SetTone:%d\n", bTone1));
    //MsOS_DelayTask(100);
    return status;
}

MS_BOOL INTERN_DVBS_DiSEqC_SetLNBOut(MS_BOOL bLow)
{
    MS_BOOL status=TRUE;
    MS_U8 u8Data=0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_REG_BASE + 0xC2, &u8Data);
    if (bLow==TRUE)
    {
        u8Data=(u8Data|0x40);    //13V
    }
    else
    {
        u8Data=(u8Data&(~0x40));//18V
    }
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2_REG_BASE + 0xC2, u8Data);

    return status;
}

MS_BOOL INTERN_DVBS_DiSEqC_GetLNBOut(MS_BOOL* bLNBOutLow)
{
    MS_BOOL status=TRUE;
    MS_U8 u8Data=0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_REG_BASE + 0xC2, &u8Data);
    if( (u8Data&0x40)==0x40)
    {
        * bLNBOutLow=TRUE;
    }
    else
    {
        * bLNBOutLow=FALSE;
    }

    return status;
}

MS_BOOL INTERN_DVBS_DiSEqC_Set22kOnOff(MS_BOOL b22kOn)
{
    MS_BOOL status=TRUE;
    MS_U8   u8Data=0;

    //Set DiSeqC 22K
    //status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2_REG_BASE + 0xCC, 0x44);        //Set 11K-->22K

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_REG_BASE + 0xC2, &u8Data);

    if (b22kOn==TRUE)
    {
        u8Data=(u8Data&0xc7);
        u8Data=(u8Data|0x08);
    }
    else
    {
        u8Data=(u8Data&0xc7);
    }
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2_REG_BASE + 0xC2, u8Data);

    DBG_INTERN_DVBS(printf("INTERN_DVBS_DiSEqC_Set22kOnOff:%d\n", b22kOn));
    return status;
}

MS_BOOL INTERN_DVBS_DiSEqC_Get22kOnOff(MS_BOOL* b22kOn)
{
    MS_BOOL status=TRUE;
    MS_U8   u8Data=0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_REG_BASE + 0xC2, &u8Data);
    if ((u8Data&0x38)==0x08)
    {
        *b22kOn=TRUE;
    }
    else
    {
        *b22kOn=FALSE;
    }

    return status;
}

MS_BOOL INTERN_DVBS_DiSEqC_SendCmd(MS_U8* pCmd,MS_U8 u8CmdSize)
{
    MS_BOOL status=TRUE;
    MS_U8   u8Data;
    MS_U8   u8Index;
    MS_U16  u16WaitCount;
/*
    MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0x60*2, &u8Data);
    //status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_TOP_WR_DBG_90, &u8Data);
    u8Data=(u8Data&~(0x10));
    MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE + 0x60*2, u8Data);
    //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_TOP_WR_DBG_90, u8Data);
*/
#if 0       //For Unicable command timing
    u16WaitCount=0;
    do
    {
        MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_DISEQC_STATUS_FLAG, &u8Data);
        //printf(">>> INTERN_DVBS_DiSEqC_SendCmd DiSEqC Status = 0x%x <<<\n", u8Data);
        MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0x60*2, &u8Data);
        //status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_TOP_WR_DBG_90, &u8Data);
        MsOS_DelayTask(1);
        u16WaitCount++;
    }while(((u8Data&0x10)==0x10)&&(u16WaitCount < INTERN_DVBS_DEMOD_WAIT_TIMEOUT));

    if (u16WaitCount >= INTERN_DVBS_DEMOD_WAIT_TIMEOUT)
    {
        DBG_INTERN_DVBS(printf("INTERN_DVBS DiSEqC Send Command Busy!!!\n"));
        return FALSE;
    }
#endif

    //u16Address=0x0BC4;
    for (u8Index=0; u8Index < u8CmdSize; u8Index++)
    {
        u8Data=*(pCmd+u8Index);
        status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2_REG_BASE + 0xC4 + u8Index, u8Data);//#define DVBS2_DISEQC_TX1                            (_REG_DVBS2(0x62)+0)//[7:0]
         DBG_INTERN_DVBS(printf("=============INTERN_DVBS_DiSEqC_SendCmd(Demod1) = 0x%X\n",u8Data));
    }

    //set DiSEqC Tx Length, Odd Enable, Tone Burst Mode
    u8Data=((u8CmdSize-1)&0x07)|0x40;
    if (_u8ToneBurstFlag==1)
    {
        u8Data|=0x80;//0x20;
    }
    else if (_u8ToneBurstFlag==2)
    {
        u8Data|=0x20;//0x80;
    }
    _u8ToneBurstFlag=0;
    status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_DMDTOP_DBG_8, u8Data);

   //add this only for check mailbox R/W
    #if 1
    DBG_INTERN_DVBS(printf(" Write into E_DMD_S2_MB_DMDTOP_DBG_8 = 0x%X!!!\n",u8Data));
    status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_DMDTOP_DBG_8, &u8Data);
    DBG_INTERN_DVBS(printf(" Read from E_DMD_S2_MB_DMDTOP_DBG_8 = 0x%X!!!\n",u8Data));
    #endif

    MsOS_DelayTask(25);//MsOS_DelayTask(10);
    MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0x60*2, &u8Data);//#define TOP_WR_DBG_90                           (_REG_DMDTOP(0x3A)+0)
    //u8Data=u8Data|0x10;
    MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE + 0x60*2, u8Data|0x10);//enable DiSEqC_Data_Tx
    //MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_REG_BASE + 0xC2, &u8Data); DBG_INTERN_DVBS(printf(">>>+++ INTERN_DVBS_DiSEqC_SendCmd DiSEqC_Init_Mode = 0x%X +++<<<\n",u8Data));
    //MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_DISEQC_STATUS_FLAG, &u8Data); DBG_INTERN_DVBS(printf(">>>+++ INTERN_DVBS_DiSEqC_SendCmd DiSEqC_State_Flag = %d +++<<<\n",u8Data));

#if 1           //For Unicable command timing???
    u16WaitCount=0;
    do
    {
        //MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_REG_BASE + 0xC2, &u8Data); DBG_INTERN_DVBS(printf(">>>*** INTERN_DVBS_DiSEqC_SendCmd DiSEqC_Init_Mode = 0x%X ***<<<\n",u8Data));
        //MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_DISEQC_STATUS_FLAG, &u8Data); DBG_INTERN_DVBS(printf(">>>*** INTERN_DVBS_DiSEqC_SendCmd DiSEqC_State_Flag = %d ***<<<\n",u8Data));
        MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0x60*2, &u8Data);
        //status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_TOP_WR_DBG_90, &u8Data);
        MsOS_DelayTask(1);
        u16WaitCount++;
    }while(((u8Data&0x10)==0x10)&&(u16WaitCount < INTERN_DVBS_DEMOD_WAIT_TIMEOUT)) ;

    if (u16WaitCount >= INTERN_DVBS_DEMOD_WAIT_TIMEOUT)
    {
        DBG_INTERN_DVBS(printf("INTERN_DVBS DiSEqC Send Command Busy!!!\n"));
        return FALSE;
    }
     else
    {
        DBG_INTERN_DVBS(printf("INTERN_DVBS DiSEqC Send Command Success!!!\n"));
        return TRUE;
    }


#endif
        //MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_REG_BASE + 0xC2, &u8Data); printf(">>>--- INTERN_DVBS_DiSEqC_SendCmd DiSEqC_Init_Mode = 0x%X ---<<<\n",u8Data);
        //MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_DISEQC_STATUS_FLAG, &u8Data); printf(">>>--- INTERN_DVBS_DiSEqC_SendCmd DiSEqC_State_Flag = %d ---<<<\n",u8Data+1);

    return status;
}

MS_BOOL INTERN_DVBS_DiSEqC_SetTxToneMode(MS_BOOL bTxTone22kOff)
{
    MS_BOOL status=TRUE;
    MS_U8   u8Data=0;

    status &= MDrv_SYS_DMD_VD_MBX_ReadReg(DVBS2_REG_BASE + 0xD7, &u8Data);//h006b    h006b    15    15    reg_diseqc_tx_tone_mode
    if (bTxTone22kOff==TRUE)
    {
        u8Data=(u8Data|0x80);                   //1: without 22K.
    }
    else
    {
        u8Data=(u8Data&(~0x80));                //0: with 22K.
    }
    status &= MDrv_SYS_DMD_VD_MBX_WriteReg(DVBS2_REG_BASE + 0xD7, u8Data);

    return status;
}

MS_BOOL INTERN_DVBS_UnicableAGCCheckPower(MS_BOOL pbAGCCheckPower)
{
    //MS_BOOL status = TRUE;
    MS_U8 u8Data=0;

    //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_TOP_WR_DBG_91, 0x00);

    MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0x60*2, &u8Data);
    u8Data &= 0xFE;//clean bit0
    MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE + 0x60*2, u8Data);

    if (pbAGCCheckPower == FALSE)//0
    {
        //status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_TOP_WR_DBG_91, &u8Data);
        u8Data &= 0xFE;//clean bit0
        //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_TOP_WR_DBG_91, u8Data);
        //printf("CMD=MS_FALSE==============================\n");
    }
    else
    {
        //status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_TOP_WR_DBG_91, &u8Data);
        u8Data |= 0x01;           //bit1=1
        //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_TOP_WR_DBG_91, u8Data);
        //printf("CMD=MS_TRUE==============================\n");
    }

    MDrv_SYS_DMD_VD_MBX_ReadReg(TOP_REG_BASE + 0x60*2, &u8Data);
    //status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_TOP_WR_DBG_90, &u8Data);
    u8Data &= 0xF0;
    u8Data |= 0x01;
    MDrv_SYS_DMD_VD_MBX_WriteReg(TOP_REG_BASE + 0x60*2, u8Data);
    //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_TOP_WR_DBG_90, u8Data);
    MsOS_DelayTask(500);

    //status &= MDrv_SYS_DMD_VD_MBX_ReadDSPReg(E_DMD_S2_MB_TOP_WR_DBG_91, &u8Data);
    u8Data &= 0x80;             //Read bit7
    if (u8Data == 0x80)
    {
        u8Data = 0x00;
        //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_TOP_WR_DBG_90, u8Data);
        u8Data = 0x00;
        //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_TOP_WR_DBG_91, u8Data);
        return TRUE;
    }
    else
    {
        u8Data = 0x00;
        //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_TOP_WR_DBG_90, u8Data);
        u8Data = 0x00;
        //status &= MDrv_SYS_DMD_VD_MBX_WriteDSPReg(E_DMD_S2_MB_TOP_WR_DBG_91, u8Data);
        return FALSE;
    }
}

//------------------------------------------------------------------
//  END DiSEqc Function
//------------------------------------------------------------------
//------------------------------------------------------------------
//  R/W Function
//------------------------------------------------------------------
MS_BOOL INTERN_DVBS_WriteReg2bytes(MS_U16 u16Addr, MS_U16 u16Data)
{
    MS_BOOL     bRet= TRUE;
    bRet &= MDrv_SYS_DMD_VD_MBX_WriteReg(u16Addr, (MS_U8)u16Data&0x00ff);
    bRet &= MDrv_SYS_DMD_VD_MBX_WriteReg(u16Addr + 0x0001, (MS_U8)(u16Data>>8)&0x00ff);
    return bRet;
}

MS_BOOL INTERN_DVBS_ReadReg2bytes(MS_U16 u16Addr, MS_U16 *pu16Data)
{
    MS_BOOL   bRet= TRUE;
    MS_U8     u8Data =0;
    MS_U16    u16Data =0;

    bRet &= MDrv_SYS_DMD_VD_MBX_ReadReg(u16Addr + 0x0001, &u8Data);
    u16Data = u8Data;
    bRet &= MDrv_SYS_DMD_VD_MBX_ReadReg(u16Addr, &u8Data);
    *pu16Data = (u16Data<<8)|u8Data;

    return bRet;
}

//Frontend Freeze
MS_BOOL INTERN_DVBS_DTV_FrontendSetFreeze(void)
{
    MS_BOOL       bRet= TRUE;
    MS_U16        u16Address;
    MS_U16        u16Data=0;

    u16Address = (FRONTEND_FREEZE_DUMP>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data|=(FRONTEND_FREEZE_DUMP&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    return bRet;
}

MS_BOOL INTERN_DVBS_DTV_FrontendUnFreeze(void)
{
    MS_BOOL     bRet= TRUE;
    MS_U16      u16Address;
    MS_U16      u16Data=0;

    u16Address = (FRONTEND_FREEZE_DUMP>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data&=~(FRONTEND_FREEZE_DUMP&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    return bRet;
}

//Inner Freeze
MS_BOOL INTERN_DVBS_DTV_InnerSetFreeze(void)
{
    MS_BOOL       bRet= TRUE;
    MS_U16        u16Address;
    MS_U16        u16Data=0;

    u16Address = (INNER_FREEZE_DUMP>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data|=(INNER_FREEZE_DUMP&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    return bRet;
}

MS_BOOL INTERN_DVBS_DTV_InnerUnFreeze(void)
{
    MS_BOOL     bRet= TRUE;
    MS_U16      u16Address;
    MS_U16      u16Data=0;

    u16Address = (INNER_FREEZE_DUMP>>16)&0xffff;
    bRet &= INTERN_DVBS_ReadReg2bytes(u16Address, &u16Data);
    u16Data&=~(INNER_FREEZE_DUMP&0xffff);
    bRet &= INTERN_DVBS_WriteReg2bytes(u16Address, u16Data);

    return bRet;
}
//------------------------------------------------------------------
//  END R/W Function
//------------------------------------------------------------------


/***********************************************************************************
  Subject:    read register
  Function:   MDrv_1210_IIC_Bypass_Mode
  Parmeter:
  Return:
  Remark:
************************************************************************************/
//void MDrv_1210_IIC_Bypass_Mode(MS_BOOL enable)
//{
//    UNUSED(enable);
//    if (enable)
//        MDrv_SYS_DMD_VD_MBX_WriteReg(0x8010, 0x10);        // IIC by-pass mode on
//    else
//        MDrv_SYS_DMD_VD_MBX_WriteReg(0x8010, 0x00);        // IIC by-pass mode off
//}

