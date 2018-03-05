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

#ifndef _HALVIF_H_
#define _HALVIF_H_

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsTypes.h"

#include "drvVIF.h"

#ifndef _BIT0
#define _BIT0  0x0001
#endif
#ifndef _BIT1
#define _BIT1  0x0002
#endif
#ifndef _BIT2
#define _BIT2  0x0004
#endif
#ifndef _BIT3
#define _BIT3  0x0008
#endif
#ifndef _BIT4
#define _BIT4  0x0010
#endif
#ifndef _BIT5
#define _BIT5  0x0020
#endif
#ifndef _BIT6
#define _BIT6  0x0040
#endif
#ifndef _BIT7
#define _BIT7  0x0080
#endif
#ifndef _BIT8
#define _BIT8  0x0100
#endif
#ifndef _BIT9
#define _BIT9  0x0200
#endif
#ifndef _BIT10
#define _BIT10 0x0400
#endif
#ifndef _BIT11
#define _BIT11 0x0800
#endif
#ifndef _BIT12
#define _BIT12 0x1000
#endif
#ifndef _BIT13
#define _BIT13 0x2000
#endif
#ifndef _BIT14
#define _BIT14 0x4000
#endif
#ifndef _BIT15
#define _BIT15 0x8000
#endif
#ifndef BOOL
#define BOOL MS_BOOL
#endif
#ifndef BYTE
#define BYTE MS_U8
#endif
#ifndef WORD
#define WORD MS_U16
#endif
#ifndef DWORD
#define DWORD MS_U32
#endif
#ifndef U8
#define U8 MS_U8
#endif
#ifndef U16
#define U16 MS_U16
#endif
#ifndef U32
#define U32 MS_U32
#endif

#define _bit0_(val)                (val & _BIT0)
#define _bit1_(val)                (val & _BIT1)
#define _bit2_(val)                (val & _BIT2)
#define _bit3_(val)                (val & _BIT3)
#define _bit4_(val)                (val & _BIT4)
#define _bit5_(val)                (val & _BIT5)
#define _bit6_(val)                (val & _BIT6)
#define _bit7_(val)                (val & _BIT7)
#define _bit8_(val)                (val & _BIT8)
#define _bit9_(val)                (val & _BIT9)
#define _bit10_(val)              (val & _BIT10)
#define _bit11_(val)              (val & _BIT11)
#define _bit12_(val)              (val & _BIT12)
#define _bit13_(val)              (val & _BIT13)
#define _bit14_(val)              (val & _BIT14)
#define _bit15_(val)              (val & _BIT15)

#define AGC_MEAN16_UPBOUND              0x8C     // 1.256*0xE0/2
#define AGC_MEAN16_LOWBOUND             0x13     // (1/4)*0x9A/2
#define AGC_MEAN16_UPBOUND_SECAM        0x28     // 1.256*0x40/2
#define AGC_MEAN16_LOWBOUND_SECAM       0x08     // (1/4)*0x40/2

#define FREQ_STEP_31_25KHz          	0x00
#define FREQ_STEP_50KHz             	0x01
#define FREQ_STEP_62_5KHz           	0x02

#define IF_FREQ_45_75                   0x01//IF_FREQ_MN
#define IF_FREQ_38_90                   0x02//IF_FREQ_B
#define IF_FREQ_38_00                   0x03//IF_FREQ_PAL_38

// register
typedef struct
{
    U32 u32Address;   // register index
    U8  u8Value;      // register value
} MS_VIF_REG_TYPE;

typedef enum
{
    VIF_START,
    VIF_AGC_STATUS,
    VIF_AFC_STATUS,
    VIF_AFC_STATUS2,
    VIF_STEADY_STATUS,
    VIF_NUMS
}VIFStatus;

typedef enum
{
    VCO_CAL_START,
    VCO_CAL_FINISH,
    VCO_CAL_CHECK,
    VCO_CAL_END,
    VCO_NUMS
}VCOCalStatus;

typedef enum
{
    EXTERNAL_DUAL_SAW,                   // 0
    EXTERNAL_SINGLE_SAW,               // 1
    SILICON_TUNER,                             // 2
    NO_SAW,                                          // 3
    INTERNAL_SINGLE_SAW,               // 4
    NO_SAW_DIF,                          //5
    SAVE_PIN_VIF,                                //6
    SAW_ARCH_NUMS
}VIFSawArch;

BYTE g_ucVifStatusStep;
BYTE g_ucVifSoundSystemType;
BOOL g_bCheckModulationType;  // 0: negative; 1: positive
BOOL g_bCheckIFFreq;          // 0: 38.9 MHz (PAL/SECAM L); 1: 33.9 MHz (SECAM L')
BYTE g_VifCrKp;
BYTE g_VifCrKi;
BOOL g_VifZeroDetFlag;
BOOL g_VifHWKpKiFlag;
BYTE g_VifCrKpKiAdjLoopCnt;
BYTE g_VifShiftClk;
BOOL g_VifChipVersion;
DWORD u32PMBank, u32PMBankSize;
FrequencyBand g_FreqBand;
IfFrequencyType g_FreqType;

void HAL_VIF_RegInit (void);
void HAL_VIF_SetClock(BOOL bEnable);
void msVifSetIfFreq(IfFrequencyType ucIfFreq);
void msVifLoad(void);
void msVifInitial(void);
void msVifExit(void);
void msVifHandler(BOOL bVifDbbAcq);
void msVifSetSoundSystem(VIFSoundSystem ucSoundSystem);
void msVifAdcInitial(void);
void msVifTopAdjust(void);
void msVifCrKpKiAutoAdjust(BYTE VifCrKpKiAdjustThr1, BYTE VifCrKpKiAdjustThr2);
void msVifSeriousACIDetection(void);
void msVifShiftClk(BYTE VifShiftClk);
U8 msVifReadCRFOE(void);
U8 msVifReadLockStatus(void);
void msVifLoadEQCoeff(BYTE VifSoundStandard);
void HAL_VIF_BypassDBBAudioFilter(BOOL bEnable);
BOOL HAL_VIF_GetInputLevelIndicator(void);
void HAL_VIF_WriteByteMask(U32 u32Reg, U8 u8Val, U8 u8Mask);
void HAL_VIF_WriteBit(U32 u32Reg, BOOL bEnable, U8 u8Mask);
void HAL_VIF_WriteByte(U32 u32Reg, U8 u8Val );
U8 HAL_VIF_ReadByte(U32 u32Reg );
U8 HAL_VIF_GetCrPDInverse(void);
void HAL_VIF_SetCrPDInverse(BOOL bEnable);
#endif //_HALVIF_H_

