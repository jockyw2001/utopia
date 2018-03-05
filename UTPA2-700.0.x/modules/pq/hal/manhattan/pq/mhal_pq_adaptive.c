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
#define MHAL_PQ_ADAPTIVE_C

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include "string.h"
#else
#include <linux/string.h>
#endif
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"

// Internal Definition
#include "hwreg_utility2.h"
#include "color_reg.h"

#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_Dlc.h"
#include "drvPQ.h"
#include "mhal_pq_adaptive.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MSIF_ADAPTIVE_LIB_CODE_C            {'A','D','A','P','T','I','V','E','_'}
#define MSIF_ADAPTIVE_VERSION_C             {'0','0','0','0','0','1'}
#define PQ_ADAPTIVE_VERSION 9132

#ifndef UNUSED //to avoid compile warnings...
#define UNUSED(var) (void)((var) = (var))
#endif
//#define DBG_DBK_LC
//#define DBG_SD2HD_LC
//#define DBG_HISW8T_LC
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static MS_BOOL _bAnyCandenceEnable = TRUE;
static MS_BOOL _bVerticalMovingForDHD = FALSE;
//extern MS_U16 _u16PQSrcType_DBK_Detect[PQ_MAX_WINDOW]; //For Auto_DBK SW driver used
static MS_BOOL _bIsHDSource = true;             //For Check SD mode
static int normalize_factor = 255;
//DBK Variables
// Width candidate = 6 to 37, offset = 6, size = 32
XC_LetterBoxState lbState;
static MS_S32 frameWidth;
static MS_S32 frameHeight;
static MS_U8 lbConfirmLevel = 0;
static MS_U16 GuardBandWidth = 0x40;
#define MaxWidth 4097
#define MaxHeight 2161
#define paraSize 32
#define paraVerSize 16

const int paraWidth[32] = { 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35};
const int paraVerWidth[16] = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

const int WidthDetectCostDownBit = 0;

// For IIR
static MS_S32 WidthIIR = 8;
static int BlockinessCheckPre[MaxWidth] = { 0 };
static int BlockinessCheckIIR[MaxWidth] = { 0 };
static XC_MCNRLutQmapSetting m_stMCNRLutQmapSetting = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}};

// Threshold for blockness check
const int hitrate_threshold = 128; // 160/256
const int hitRateDiffThr = 16; // 16/256
// IIR Strength
int IIRStrength = 4; // IIR = 4 : Cur * 1/4 + IIR * 3/4
//Vertical Mark Strength
static int VMStrMax = 0xF;
static int VMStrMin = 0x0;
const int HDBK_Str_Thrd = 50;

//Patch switching control
#define HQV_JAGGY_PATTERN_ENBLE 0
#define SHIBASOKU_ENABLE 1
#define LG_SPECIFIC_PATTERN_ENABLE 0
#define SZ_PATTERN_ENABLE 1
#define SEC_HQV_PATTERN 0
#define KURO_PATTERN_ENABLE 0
#define HOUSE_MD_PATTERN_ENABLE 0

#define MCNR_LUT_LOAD_FROM_QMAP 1
#define PDNR_LUT_LOAD_FROM_QMAP 1

#if !defined(MSOS_TYPE_LINUX_KERNEL)
extern __attribute__((weak))MS_U8 MApi_XC_DLC_GetAverageValue(void);
#endif
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

/******************************************************************************/
/// Debug Info Control
/******************************************************************************/
void DEBUG_LC_Turn_On(void *pInstance)
{
	MApi_XC_W2BYTEMSK(REG_SC_BK30_38_L, 0x0001, 0x0001 );
	MApi_XC_W2BYTEMSK(REG_SC_BK30_3A_L, 0x0002, 0x0002 );
}

void DEBUG_LC(void *pInstance, const MS_U16 lightReg, const MS_U8 val, const MS_U8 color )
{
	if( val == 0 )
		MApi_XC_W2BYTEMSK(lightReg, ( color << (val<<2) ), ( 0x7 << (val<<2) ) );
	else if( val == 1 )
		MApi_XC_W2BYTE(lightReg, ( color << 4 ) | color );
	else if( val == 2 )
		MApi_XC_W2BYTE(lightReg, ( color << 8 ) | ( color << 4 ) | color );
	else
		MApi_XC_W2BYTE(lightReg, ( color << 12 ) | ( color << 8 ) | ( color << 4 ) | color );
}

void DEBUG_LC_40(void *pInstance, const MS_U8 a )
{
	if( (a > 0x00) && (a < 0x04) )
	{
		MApi_XC_W2BYTE(REG_SC_BK30_40_L, 0x0001 );
	}
	else if( (a > 0x03) && (a < 0x10) )
	{
		MApi_XC_W2BYTE(REG_SC_BK30_40_L, 0x0011 );
	}
	else if( (a > 0x0F) && (a < 0x40) )
	{
		MApi_XC_W2BYTE(REG_SC_BK30_40_L, 0x0111 );
	}
	else if( (a > 0x3F) /*&& (a < 0x100)*/ )
	{
		MApi_XC_W2BYTE(REG_SC_BK30_40_L, 0x1111 );
	}
	else
	{
		MApi_XC_W2BYTE(REG_SC_BK30_40_L, 0x0000 );
	}
}

void DEBUG_LC_41(void *pInstance, const MS_U8 a )
{
	if( (a > 0) && (a < 4) )
	{
		MApi_XC_W2BYTE(REG_SC_BK30_41_L, 0x0002 );
	}
	else if( (a > 3) && (a < 8) )
	{
		MApi_XC_W2BYTE(REG_SC_BK30_41_L, 0x0022 );
	}
	else if( (a > 7) && (a < 12) )
	{
		MApi_XC_W2BYTE(REG_SC_BK30_41_L, 0x0222 );
	}
	else if( (a > 11) )
	{
		MApi_XC_W2BYTE(REG_SC_BK30_41_L, 0x2222 );
	}
	else
	{
		MApi_XC_W2BYTE(REG_SC_BK30_41_L, 0x0000 );
	}
}

void DEBUG_LC_42(void *pInstance, const MS_BOOL isSceneChangeMc, const MS_BOOL isSceneChangeY )
{
	if( isSceneChangeMc )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_42_L, 0x0003, 0x0007);
	else
		MApi_XC_W2BYTEMSK(REG_SC_BK30_42_L, 0x0000, 0x0007);

	if( isSceneChangeY )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_42_L, 0x0040, 0x0070);
	else
		MApi_XC_W2BYTEMSK(REG_SC_BK30_42_L, 0x0000, 0x0070);
}

void DEBUG_LC_43(void *pInstance, MS_U8 noiseLevel )
{
	/*
	if( noiseLevel < 0 )
	MApi_XC_W2BYTEMSK(REG_SC_BK30_43_L, 0x1111, 0x7777 );
	else
	*/
	{
		MS_U8 val = noiseLevel >> 5;
		if( val == 0 )
			MApi_XC_W2BYTEMSK(REG_SC_BK30_43_L, 0x0000, 0x7777);
		else if( val == 1 )
			MApi_XC_W2BYTEMSK(REG_SC_BK30_43_L, 0x0002, 0x7777);
		else if( val == 2 )
			MApi_XC_W2BYTEMSK(REG_SC_BK30_43_L, 0x0022, 0x7777);
		else if( val == 3 )
			MApi_XC_W2BYTEMSK(REG_SC_BK30_43_L, 0x0222, 0x7777);
		else
			MApi_XC_W2BYTEMSK(REG_SC_BK30_43_L, 0x2222, 0x7777);
	}
}

void DEBUG_LC_55(void *pInstance, const MS_U8 GG )
{

	//BK30_40~BK30_5F
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right
	if ( GG == 1 )//white
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, 0x7777, 0x7777);
	}
	else if (GG == 2)//green
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, 0x2222, 0x7777);
	}
	else//black
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, 0x0000, 0x7777);
	}
}

void DEBUG_LC_56(void *pInstance, const MS_U8 GG )
{

	//BK30_40~BK30_5F
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right
	if ( GG == 1 )//white
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, 0x7777, 0x7777);
	}
	else if (GG == 2)//green
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, 0x2222, 0x7777);
	}
	else//black
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, 0x0000, 0x7777);
	}
}
void DEBUG_LC_57(void *pInstance, const MS_U8 GG )
{

	//BK30_40~BK30_5F
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right
	if ( GG == 1 )//white
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, 0x7777, 0x7777);
	}
	else if (GG == 2)//green
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, 0x2222, 0x7777);
	}
	else//black
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, 0x0000, 0x7777);
	}
}

void DEBUG_LC_5D(void *pInstance, const MS_U8 GG )
{

	//BK30_40~BK30_5F
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right
	if ( GG == 1 )//white
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5D_L, 0x7777, 0x7777);
	}
	else if (GG == 2)//green
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5D_L, 0x2222, 0x7777);
	}
	else//black
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5D_L, 0x0000, 0x7777);
	}
}
void DEBUG_LC_5E(void *pInstance, const MS_U8 GG )
{

	//BK30_40~BK30_5F
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right
	if ( GG == 1 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5E_L, 0x7777, 0x7777);
	}
	else if (GG == 2)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5E_L, 0x2222, 0x7777);
	}
	else
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5E_L, 0x0000, 0x7777);
	}
}
void DEBUG_LC_5F(void *pInstance, const MS_U8 GG )
{

	//BK30_40~BK30_5F
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right
	if ( GG == 1 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5F_L, 0x7777, 0x7777);
	}
	else if (GG == 2)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5F_L, 0x2222, 0x7777);
	}
	else
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5F_L, 0x0000, 0x7777);
	}
}

//Add by Bella for SHDH debug 8/30
//Renamed by Probex Sep.06 2013
void DEBUG_LC_Full6bit(void *pInstance, const MS_U8 data ) 
{
	//BK30_50, BK30_54, BK30_58
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right

	if ( data <= 7 )
	{

		MApi_XC_W2BYTEMSK(REG_SC_BK30_50_L, data, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_54_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_58_L, 0x0000, 0x7777);

  }
  else if ( data <= 14 )
	{
		MS_U16 dataExpression = ( (data-7) << 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_50_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_54_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_58_L, 0x0000, 0x7777);
	}
	else if ( data <= 21)
	{
		MS_U16 dataExpression = ( (data-14) << 8) + (7<<4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_50_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_54_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_58_L, 0x0000, 0x7777);
	}
	else if (data <= 28 )
	{
		MS_U16 dataExpression = ( (data-21) << 12) + (7<<8) + (7<<4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_50_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_54_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_58_L, 0x0000, 0x7777);
	}
	else if (data <= 35)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_50_L, 0x7777, 0x7777);
		MS_U16 dataExpression = ( data-28 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_54_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_58_L, 0x0000, 0x7777);

	}
	else if (data <= 42 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_50_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-35 )<< 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_54_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_58_L, 0x0000, 0x7777);
	}
  else if (data <= 49 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_50_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-42 )<< 8) + (7 << 4) +7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_54_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_58_L, 0x0000, 0x7777);

	}
	else if (data <= 56 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_50_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-49 )<< 12) + (7 << 8) + (7 << 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_54_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_58_L, 0x0000, 0x7777);
	}
	else if (data <= 63 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_50_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_54_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (data - 56 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_58_L, dataExpression, 0x7777);
	}
	else //>64
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_50_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_54_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_58_L, 0x7777, 0x7777);
	}
}
void DEBUG_LC_Full6bit2(void *pInstance, const MS_U8 data ) 
{
	//BK30_51, BK30_55, BK30_59
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right

	if ( data <= 7 )
	{

		MApi_XC_W2BYTEMSK(REG_SC_BK30_51_L, data, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_59_L, 0x0000, 0x7777);

  }
  else if ( data <= 14 )
	{
		MS_U16 dataExpression = ( (data-7) << 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_51_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_59_L, 0x0000, 0x7777);
	}
	else if ( data <= 21)
	{
		MS_U16 dataExpression = ( (data-14) << 8) + (7<<4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_51_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_59_L, 0x0000, 0x7777);
	}
	else if (data <= 28 )
	{
		MS_U16 dataExpression = ( (data-21) << 12) + (7<<8) + (7<<4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_51_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_59_L, 0x0000, 0x7777);
	}
	else if (data <= 35)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_51_L, 0x7777, 0x7777);
		MS_U16 dataExpression = ( data-28 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_59_L, 0x0000, 0x7777);

	}
	else if (data <= 42 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_51_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-35 )<< 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_59_L, 0x0000, 0x7777);
	}
  else if (data <= 49 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_51_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-42 )<< 8) + (7 << 4) +7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_59_L, 0x0000, 0x7777);

	}
	else if (data <= 56 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_51_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-49 )<< 12) + (7 << 8) + (7 << 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_59_L, 0x0000, 0x7777);
	}
	else if (data <= 63 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_51_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (data - 56 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_59_L, dataExpression, 0x7777);
	}
	else
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_51_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_59_L, 0x7777, 0x7777);
	}
}
void DEBUG_LC_Full6bit3(void *pInstance, const MS_U8 data ) 
{
	//BK30_52, BK30_56, BK30_5A
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right

	if ( data <= 7 )
	{

		MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, data, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5A_L, 0x0000, 0x7777);

  }
  else if ( data <= 14 )
	{
		MS_U16 dataExpression = ( (data-7) << 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5A_L, 0x0000, 0x7777);
	}
	else if ( data <= 21)
	{
		MS_U16 dataExpression = ( (data-14) << 8) + (7<<4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5A_L, 0x0000, 0x7777);
	}
	else if (data <= 28 )
	{
		MS_U16 dataExpression = ( (data-21) << 12) + (7<<8) + (7<<4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5A_L, 0x0000, 0x7777);
	}
	else if (data <= 35)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x7777, 0x7777);
		MS_U16 dataExpression = ( data-28 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5A_L, 0x0000, 0x7777);

	}
	else if (data <= 42 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-35 )<< 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5A_L, 0x0000, 0x7777);
	}
  else if (data <= 49 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-42 )<< 8) + (7 << 4) +7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5A_L, 0x0000, 0x7777);

	}
	else if (data <= 56 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-49 )<< 12) + (7 << 8) + (7 << 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5A_L, 0x0000, 0x7777);
	}
	else if (data <= 63 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (data - 56 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5A_L, dataExpression, 0x7777);
	}
	else
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5A_L, 0x7777, 0x7777);
	}
}
void DEBUG_LC_Full6bit4(void *pInstance, const MS_U8 data ) 
{
	//BK30_53, BK30_57, BK30_5B
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right

	if ( data <= 7 )
	{

		MApi_XC_W2BYTEMSK(REG_SC_BK30_53_L, data, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5B_L, 0x0000, 0x7777);

  }
  else if ( data <= 14 )
	{
		MS_U16 dataExpression = ( (data-7) << 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_53_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5B_L, 0x0000, 0x7777);
	}
	else if ( data <= 21)
	{
		MS_U16 dataExpression = ( (data-14) << 8) + (7<<4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_53_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5B_L, 0x0000, 0x7777);
	}
	else if (data <= 28 )
	{
		MS_U16 dataExpression = ( (data-21) << 12) + (7<<8) + (7<<4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_53_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5B_L, 0x0000, 0x7777);
	}
	else if (data <= 35)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_53_L, 0x7777, 0x7777);
		MS_U16 dataExpression = ( data-28 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5B_L, 0x0000, 0x7777);

	}
	else if (data <= 42 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_53_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-35 )<< 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5B_L, 0x0000, 0x7777);
	}
  else if (data <= 49 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_53_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-42 )<< 8) + (7 << 4) +7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5B_L, 0x0000, 0x7777);

	}
	else if (data <= 56 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_53_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-49 )<< 12) + (7 << 8) + (7 << 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5B_L, 0x0000, 0x7777);
	}
	else if (data <= 63 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_53_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (data - 56 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5B_L, dataExpression, 0x7777);
	}
	else
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_53_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5B_L, 0x7777, 0x7777);
	}
}
void DEBUG_LC_Full6bit5(void *pInstance, const MS_U8 data ) 
{
	//BK30_50, BK30_54, BK30_58
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right

	if ( data <= 7 )
	{

		MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, data, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_44_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_48_L, 0x0000, 0x7777);

  }
  else if ( data <= 14 )
	{
		MS_U16 dataExpression = ( (data-7) << 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_44_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_48_L, 0x0000, 0x7777);
	}
	else if ( data <= 21)
	{
		MS_U16 dataExpression = ( (data-14) << 8) + (7<<4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_44_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_48_L, 0x0000, 0x7777);
	}
	else if (data <= 28 )
	{
		MS_U16 dataExpression = ( (data-21) << 12) + (7<<8) + (7<<4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_44_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_48_L, 0x0000, 0x7777);
	}
	else if (data <= 35)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x7777, 0x7777);
		MS_U16 dataExpression = ( data-28 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_44_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_48_L, 0x0000, 0x7777);

	}
	else if (data <= 42 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-35 )<< 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_44_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_48_L, 0x0000, 0x7777);
	}
  else if (data <= 49 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-42 )<< 8) + (7 << 4) +7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_44_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_48_L, 0x0000, 0x7777);

	}
	else if (data <= 56 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-49 )<< 12) + (7 << 8) + (7 << 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_44_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_48_L, 0x0000, 0x7777);
	}
	else if (data <= 63 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_44_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (data - 56 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_48_L, dataExpression, 0x7777);
	}
	else //>64
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_44_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_48_L, 0x7777, 0x7777);
	}
}
void DEBUG_LC_Full6bit6(void *pInstance, const MS_U8 data ) 
{
	//BK30_50, BK30_54, BK30_58
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right

	if ( data <= 7 )
	{

		MApi_XC_W2BYTEMSK(REG_SC_BK30_41_L, data, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_45_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_49_L, 0x0000, 0x7777);

  }
  else if ( data <= 14 )
	{
		MS_U16 dataExpression = ( (data-7) << 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_41_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_45_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_49_L, 0x0000, 0x7777);
	}
	else if ( data <= 21)
	{
		MS_U16 dataExpression = ( (data-14) << 8) + (7<<4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_41_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_45_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_49_L, 0x0000, 0x7777);
	}
	else if (data <= 28 )
	{
		MS_U16 dataExpression = ( (data-21) << 12) + (7<<8) + (7<<4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_41_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_45_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_49_L, 0x0000, 0x7777);
	}
	else if (data <= 35)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_41_L, 0x7777, 0x7777);
		MS_U16 dataExpression = ( data-28 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_45_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_49_L, 0x0000, 0x7777);

	}
	else if (data <= 42 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_41_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-35 )<< 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_45_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_49_L, 0x0000, 0x7777);
	}
  else if (data <= 49 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_41_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-42 )<< 8) + (7 << 4) +7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_45_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_49_L, 0x0000, 0x7777);

	}
	else if (data <= 56 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_41_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-49 )<< 12) + (7 << 8) + (7 << 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_45_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_49_L, 0x0000, 0x7777);
	}
	else if (data <= 63 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_41_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_45_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (data - 56 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_49_L, dataExpression, 0x7777);
	}
	else //>64
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_41_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_45_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_49_L, 0x7777, 0x7777);
	}
}
void DEBUG_LC_Full6bit7(void *pInstance, const MS_U8 data ) 
{
	//BK30_50, BK30_54, BK30_58
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right

	if ( data <= 7 )
	{

		MApi_XC_W2BYTEMSK(REG_SC_BK30_42_L, data, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_46_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_4A_L, 0x0000, 0x7777);

  }
  else if ( data <= 14 )
	{
		MS_U16 dataExpression = ( (data-7) << 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_42_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_46_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_4A_L, 0x0000, 0x7777);
	}
	else if ( data <= 21)
	{
		MS_U16 dataExpression = ( (data-14) << 8) + (7<<4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_42_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_46_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_4A_L, 0x0000, 0x7777);
	}
	else if (data <= 28 )
	{
		MS_U16 dataExpression = ( (data-21) << 12) + (7<<8) + (7<<4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_42_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_46_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_4A_L, 0x0000, 0x7777);
	}
	else if (data <= 35)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_42_L, 0x7777, 0x7777);
		MS_U16 dataExpression = ( data-28 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_46_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_4A_L, 0x0000, 0x7777);

	}
	else if (data <= 42 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_42_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-35 )<< 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_46_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_4A_L, 0x0000, 0x7777);
	}
  else if (data <= 49 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_42_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-42 )<< 8) + (7 << 4) +7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_46_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_4A_L, 0x0000, 0x7777);

	}
	else if (data <= 56 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_42_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-49 )<< 12) + (7 << 8) + (7 << 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_46_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_4A_L, 0x0000, 0x7777);
	}
	else if (data <= 63 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_42_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_46_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (data - 56 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_4A_L, dataExpression, 0x7777);
	}
	else //>64
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_42_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_46_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_4A_L, 0x7777, 0x7777);
	}
}
void DEBUG_LC_Full6bit8(void *pInstance, const MS_U8 data ) 
{
	//BK30_50, BK30_54, BK30_58
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right

	if ( data <= 7 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_43_L, data, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_47_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_4B_L, 0x0000, 0x7777);
  }
  else if ( data <= 14 )
	{
		MS_U16 dataExpression = ( (data-7) << 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_43_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_47_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_4B_L, 0x0000, 0x7777);
	}
	else if ( data <= 21)
	{
		MS_U16 dataExpression = ( (data-14) << 8) + (7<<4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_43_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_47_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_4B_L, 0x0000, 0x7777);
	}
	else if (data <= 28 )
	{
		MS_U16 dataExpression = ( (data-21) << 12) + (7<<8) + (7<<4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_43_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_47_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_4B_L, 0x0000, 0x7777);
	}
	else if (data <= 35)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_43_L, 0x7777, 0x7777);
		MS_U16 dataExpression = ( data-28 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_47_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_4B_L, 0x0000, 0x7777);

	}
	else if (data <= 42 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_43_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-35 )<< 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_47_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_4B_L, 0x0000, 0x7777);
	}
  else if (data <= 49 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_43_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-42 )<< 8) + (7 << 4) +7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_47_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_4B_L, 0x0000, 0x7777);

	}
	else if (data <= 56 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_43_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (( data-49 )<< 12) + (7 << 8) + (7 << 4) + 7;
		MApi_XC_W2BYTEMSK(REG_SC_BK30_47_L, dataExpression, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_4B_L, 0x0000, 0x7777);
	}
	else if (data <= 63 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_43_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_47_L, 0x7777, 0x7777);
		MS_U16 dataExpression = (data - 56 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_4B_L, dataExpression, 0x7777);
	}
	else //>64
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_43_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_47_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_4B_L, 0x7777, 0x7777);
	}
}
void DEBUG_LC_DBKWidth(void *pInstance, const MS_U8 width )
{
	//BK30_40~BK30_5F
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right
	if ( width == 8 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5C_L, 0x0222, 0x0777);
	}
	else if ( width == 16 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5C_L, 0x0444, 0x0777);
	}
	else if ( width <=22 && width>=20 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5C_L, 0x0555, 0x0777);
	}
	else
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5C_L, 0x0111, 0x0777);
	}
}
void DEBUG_LC_DBKDET(void *pInstance, const MS_U8 GG )
{

	//BK30_40~BK30_5F
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right
	if ( GG == 0 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5C_L, 0x7000, 0x7000);
	}
	if ( GG == 1 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5C_L, 0x6000, 0x7000); //yellow
	}
	else if (GG == 2)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5C_L, 0x5000, 0x7000); //purple
	}
	else if (GG == 3)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5C_L, 0x4000, 0x7000); //4 = cyan
	}
	else if (GG == 4)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5C_L, 0x3000, 0x7000); //3 = blue
	}
	else if (GG == 5)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5C_L, 0x2000, 0x7000); //2 = green
	}
	else if (GG == 6)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5C_L, 0x0000, 0x7000);

	}
	else
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5C_L, 0x1111, 0x7000);
	}
}
void DEBUG_LC_FrameEdg(void *pInstance, const MS_U16 inValue )
{
	//BK30_40~BK30_5F
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right

	//long HDCONTROLVALUE = MApi_XC_R2BYTEMSK(REG_SC_BK60_39_L, 0xFFFF) + MApi_XC_R2BYTEMSK(REG_SC_BK60_3A_L, 0x00FF);

	//MS_BOOL HD_control = HDCONTROLVALUE > 0 ? true : false ;
	//PRT_TRP("HDCONTROLVALUE = %d;HD_control = %d;inValue = %d\n", HDCONTROLVALUE, HD_control, inValue);
	int TH1, TH2, TH3, TH4, TH5, TH6, TH7, TH8;
	{
		TH1 = 100;
		TH2 = 80;
		TH3 = 60;
		TH4 = 50;
		TH5 = 40;
		TH6 = 30;
		TH7 = 20;
		TH8 = 10;
	}
	if ( inValue> TH1 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_53_L, 0x7777, 0x7777);
	}
	else if ( inValue> TH2 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x7770, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_53_L, 0x7777, 0x7777);
	}
	else if ( inValue> TH3 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x7700, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_53_L, 0x7777, 0x7777);
	}
	else if ( inValue> TH4 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x7000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_53_L, 0x7777, 0x7777);
	}
	else if ( inValue > TH5 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_53_L, 0x7777, 0x7777);
	}
	else if ( inValue > TH6 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_53_L, 0x7770, 0x7777);
	}
	else if ( inValue > TH7 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_53_L, 0x7700, 0x7777);
	}
	else if ( inValue > TH8 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_53_L, 0x7000, 0x7777);
	}
	else
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_53_L, 0x0000, 0x7777);
	}
}
void DEBUG_LC_FrameBlk(void *pInstance, const MS_U16 inValue )
{
	//BK30_40~BK30_5F
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right

	//long HDCONTROLVALUE = MApi_XC_R2BYTEMSK(REG_SC_BK60_39_L, 0xFFFF) + MApi_XC_R2BYTEMSK(REG_SC_BK60_3A_L, 0x00FF);

	//MS_BOOL HD_control = HDCONTROLVALUE > 0 ? true : false ;
	//PRT_TRP("HDCONTROLVALUE = %d;HD_control = %d;inValue = %d\n", HDCONTROLVALUE, HD_control, inValue);
	int TH1, TH2, TH3, TH4, TH5, TH6, TH7, TH8;
	//if (HD_control == false)
	{
		TH1 = 100;
		TH2 = 80;
		TH3 = 60;
		TH4 = 50;
		TH5 = 40;
		TH6 = 30;
		TH7 = 20;
		TH8 = 10;
	}
	if ( inValue> TH1 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, 0x7777, 0x7777);
	}
	else if ( inValue> TH2 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, 0x7770, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, 0x7777, 0x7777);
	}
	else if ( inValue> TH3 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, 0x7700, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, 0x7777, 0x7777);
	}
	else if ( inValue> TH4 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, 0x7000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, 0x7777, 0x7777);
	}
	else if ( inValue > TH5 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, 0x7777, 0x7777);
	}
	else if ( inValue > TH6 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, 0x7770, 0x7777);
	}
	else if ( inValue > TH7 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, 0x7700, 0x7777);
	}
	else if ( inValue > TH8 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, 0x7000, 0x7777);
	}
	else
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_56_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_57_L, 0x0000, 0x7777);
	}
}
void DEBUG_LC_FrameDet(void *pInstance, const MS_U16 inValue )
{
	//BK30_40~BK30_5F
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right

	//long HDCONTROLVALUE = MApi_XC_R2BYTEMSK(REG_SC_BK60_39_L, 0xFFFF) + MApi_XC_R2BYTEMSK(REG_SC_BK60_3A_L, 0x00FF);

	//MS_BOOL HD_control = HDCONTROLVALUE > 0 ? true : false ;
	//PRT_TRP("HDCONTROLVALUE = %d;HD_control = %d;inValue = %d\n", HDCONTROLVALUE, HD_control, inValue);
	int TH1, TH2, TH3, TH4, TH5, TH6, TH7, TH8;
	//if (HD_control == false)
	{
		TH1 = 100;
		TH2 = 80;
		TH3 = 60;
		TH4 = 50;
		TH5 = 40;
		TH6 = 30;
		TH7 = 20;
		TH8 = 10;
	}
	if ( inValue> TH1 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5A_L, 0x7777, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5B_L, 0x7777, 0x7777);
	}
	else if ( inValue> TH2 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5A_L, 0x7770, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5B_L, 0x7777, 0x7777);
	}
	else if ( inValue> TH3 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5A_L, 0x7700, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5B_L, 0x7777, 0x7777);
	}
	else if ( inValue> TH4 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5A_L, 0x7000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5B_L, 0x7777, 0x7777);
	}
	else if ( inValue > TH5 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5A_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5B_L, 0x7777, 0x7777);
	}
	else if ( inValue > TH6 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5A_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5B_L, 0x7770, 0x7777);
	}
	else if ( inValue > TH7 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5A_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5B_L, 0x7700, 0x7777);
	}
	else if ( inValue > TH8 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5A_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5B_L, 0x7000, 0x7777);
	}
	else
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5A_L, 0x0000, 0x7777);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5B_L, 0x0000, 0x7777);
	}
}

void DEBUG_LC_ColorSum(void *pInstance, const MS_U16 inValue )
{
	//BK30_40~BK30_5F
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right

	//long HDCONTROLVALUE = MApi_XC_R2BYTEMSK(REG_SC_BK60_39_L, 0xFFFF) + MApi_XC_R2BYTEMSK(REG_SC_BK60_3A_L, 0x00FF);

	//MS_BOOL HD_control = HDCONTROLVALUE > 0 ? true : false ;
	//PRT_TRP("HDCONTROLVALUE = %d;HD_control = %d;inValue = %d\n", HDCONTROLVALUE, HD_control, inValue);
	int TH1, TH2, TH3, TH4, TH5, TH6, TH7, TH8;

	{
		TH1 = 100;
		TH2 = 80;
		TH3 = 60;
		TH4 = 50;
		TH5 = 40;
		TH6 = 30;
		TH7 = 20;
		TH8 = 10;
	}
	if ( inValue> TH1 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, 0x7777, 0x7777);
	else if ( inValue> TH2 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, 0x7774, 0x7777);
	else if ( inValue> TH3 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, 0x7770, 0x7777);
	else if ( inValue> TH4 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, 0x7740, 0x7777);
	else if ( inValue > TH5 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, 0x7700, 0x7777);
	else if ( inValue > TH6 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, 0x7400, 0x7777);
	else if ( inValue > TH7 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, 0x7000, 0x7777);
	else if ( inValue > TH8 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, 0x4000, 0x7777);
	else
		MApi_XC_W2BYTEMSK(REG_SC_BK30_55_L, 0x0000, 0x7777);
}
void DEBUG_LC_uComplex(void *pInstance, const MS_U16 inValue )
{
	//BK30_40~BK30_5F
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right

	//long HDCONTROLVALUE = MApi_XC_R2BYTEMSK(REG_SC_BK60_39_L, 0xFFFF) + MApi_XC_R2BYTEMSK(REG_SC_BK60_3A_L, 0x00FF);

	//MS_BOOL HD_control = HDCONTROLVALUE > 0 ? true : false ;
	//PRT_TRP("HDCONTROLVALUE = %d;HD_control = %d;inValue = %d\n", HDCONTROLVALUE, HD_control, inValue);
	int TH1, TH2, TH3, TH4, TH5, TH6, TH7, TH8;

	{
		TH1 = 0x1000;
		TH2 = 0x0800;
		TH3 = 0x0400;
		TH4 = 0x0200;
		TH5 = 0x0100;
		TH6 = 0x0080;
		TH7 = 0x0040;
		TH8 = 0x0020;
	}
	if ( inValue> TH1 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_51_L, 0x7777, 0x7777);
	else if ( inValue> TH2 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_51_L, 0x7774, 0x7777);
	else if ( inValue> TH3 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_51_L, 0x7770, 0x7777);
	else if ( inValue> TH4 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_51_L, 0x7740, 0x7777);
	else if ( inValue > TH5 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_51_L, 0x7700, 0x7777);
	else if ( inValue > TH6 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_51_L, 0x7400, 0x7777);
	else if ( inValue > TH7 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_51_L, 0x7000, 0x7777);
	else if ( inValue > TH8 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_51_L, 0x4000, 0x7777);
	else
		MApi_XC_W2BYTEMSK(REG_SC_BK30_51_L, 0x0000, 0x7777);

}
void DEBUG_LC_SDCnt(void *pInstance, const MS_U16 inValue )
{
	//BK30_40~BK30_5F
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right

	//long HDCONTROLVALUE = MApi_XC_R2BYTEMSK(REG_SC_BK60_39_L, 0xFFFF) + MApi_XC_R2BYTEMSK(REG_SC_BK60_3A_L, 0x00FF);

	//MS_BOOL HD_control = HDCONTROLVALUE > 0 ? true : false ;
	//PRT_TRP("HDCONTROLVALUE = %d;HD_control = %d;inValue = %d\n", HDCONTROLVALUE, HD_control, inValue);
	int TH1, TH2, TH3, TH4, TH5, TH6, TH7, TH8;

	{
		TH1 = 50000;
		TH2 = 20000;
		TH3 = 8000;
		TH4 = 3000;
		TH5 = 1000;
		TH6 = 300;
		TH7 = 100;
		TH8 = 10;
	}
	if ( inValue> TH1 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_59_L, 0x7777, 0x7777);
	else if ( inValue> TH2 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_59_L, 0x7774, 0x7777);
	else if ( inValue> TH3 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_59_L, 0x7770, 0x7777);
	else if ( inValue> TH4 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_59_L, 0x7740, 0x7777);
	else if ( inValue > TH5 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_59_L, 0x7700, 0x7777);
	else if ( inValue > TH6 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_59_L, 0x7400, 0x7777);
	else if ( inValue > TH7 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_59_L, 0x7000, 0x7777);
	else if ( inValue > TH8 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_59_L, 0x4000, 0x7777);
	else
		MApi_XC_W2BYTEMSK(REG_SC_BK30_59_L, 0x0000, 0x7777);
}
void DEBUG_LC_HDCnt(void *pInstance, const MS_U16 inValue )
{
	//BK30_40~BK30_5F
	//1 = red, 2 = green, 3 = blue, 4 = cyan, 5 =purple, 6 = yellow, 7 = white
	//LSB(low byte) = left, MSB(high byte) = right

	//long HDCONTROLVALUE = MApi_XC_R2BYTEMSK(REG_SC_BK60_39_L, 0xFFFF) + MApi_XC_R2BYTEMSK(REG_SC_BK60_3A_L, 0x00FF);

	//MS_BOOL HD_control = HDCONTROLVALUE > 0 ? true : false ;
	//PRT_TRP("HDCONTROLVALUE = %d;HD_control = %d;inValue = %d\n", HDCONTROLVALUE, HD_control, inValue);
	int TH1, TH2, TH3, TH4, TH5, TH6, TH7, TH8;

	{
		TH1 = 4000;
		TH2 = 1000;
		TH3 = 500;
		TH4 = 250;
		TH5 = 120;
		TH6 = 60;
		TH7 = 30;
		TH8 = 10;
	}
	if ( inValue> TH1 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5D_L, 0x7777, 0x7777);
	else if ( inValue> TH2 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5D_L, 0x7774, 0x7777);
	else if ( inValue> TH3 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5D_L, 0x7770, 0x7777);
	else if ( inValue> TH4 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5D_L, 0x7740, 0x7777);
	else if ( inValue > TH5 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5D_L, 0x7700, 0x7777);
	else if ( inValue > TH6 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5D_L, 0x7400, 0x7777);
	else if ( inValue > TH7 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5D_L, 0x7000, 0x7777);
	else if ( inValue > TH8 )
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5D_L, 0x4000, 0x7777);
	else
		MApi_XC_W2BYTEMSK(REG_SC_BK30_5D_L, 0x0000, 0x7777);
}
/******************************************************************************/
// Debug Info Control
/******************************************************************************/




//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
int __minmax(int a, int b, int c) // make sure b < c
{
	if (a<=b)
		return b;
	else if (a >= c)
		return c;
	else
		return a;
}

int __max(int a, int b)
{
	if (a>=b)
		return a;
	else
		return b;
}

int __max3(int a, int b, int c)
{
	if ( (a>=b) && (a>=c) )
		return a;
	else if ((b>=a) && (b>=c) )
		return b;
	else
		return c;
}

int __min(int a, int b)
{
	if (a>=b)
		return b;
	else
		return a;
}

int __min3(int a, int b, int c)
{
	if ( (a<=b) && (a<=c) )
		return a;
	else if ((b<=a) && (b<=c) )
		return b;
	else
		return c;
}

/******************************************************************************/
///Get control register for adaptive tuning function
///@return MS_U32: Read two byte for FSC
///Add by Bella 20150707
/******************************************************************************/
MS_U8 MDrv_UFSC_Read2Byte( MS_U64 AddresL)
{
    MS_U8 u8Value= 0;

    u8Value= (MS_U8)((MApi_XC_ReadByte(AddresL+1) << 8) |(MApi_XC_ReadByte(AddresL)));

    return u8Value;
}

/******************************************************************************/
///Get control register for adaptive tuning function
///@return MS_U32: Read two byte and MASK for FSC
///Add by Bella 20150707
/******************************************************************************/
MS_U16 MDrv_UFSC_Read2ByteMSK( MS_U64 AddresL, MS_U16 mask)
{
    MS_U16 u16Value= 0;

    u16Value= (MS_U16)((MApi_XC_ReadByte(AddresL+1) << 8) |(MApi_XC_ReadByte(AddresL)))&mask;

    return u16Value;
}

/******************************************************************************/
///Get control register for adaptive tuning function
///@return MS_U8: Control status
/******************************************************************************/
MS_U8 MDrv_SC_get_adaptive_ctrl(void *pInstance)
{
    MS_U8 u8Ctrl = 0;

	u8Ctrl = (MS_U8)MApi_XC_R2BYTEMSK(REG_SC_BK22_7C_L, 0xFF);

	return u8Ctrl;
}

/******************************************************************************/
///Get control register for adaptive tuning function
///@return MS_U8: Control status
/******************************************************************************/
MS_U8 MDrv_SC_get_adaptive_ctrl2(void *pInstance)
{
    MS_U8 u8Ctrl = 0;

	u8Ctrl = (MS_U8)(MApi_XC_R2BYTEMSK(REG_SC_BK22_7C_L, 0xFF00) >> 8);

	return u8Ctrl;
}

/******************************************************************************/
///Get control register for adaptive tuning function
///@return MS_U8: Control status
/******************************************************************************/
MS_U8 MDrv_UFSC_get_adaptive_ctrl3(void)
{
    MS_U8 u8UFSCCtrl = 0;

    u8UFSCCtrl =MDrv_UFSC_Read2ByteMSK(REG_FSC_BK30_3F_L, 0xFFFF)>>8;

    return u8UFSCCtrl;
}

/******************************************************************************/
/// Agate Function
/******************************************************************************/
void MDrv_SC_Agate_ME1_SetStatisticsWindow( void *pInstance )
{
    MS_U32 u32MaxHblkSize = 0, u32MaxVblkSize = 0;

	u32MaxHblkSize = MApi_XC_R2BYTEMSK(REG_SC_BK17_02_L, 0x00FF);   //ME2_H Max
	u32MaxVblkSize = MApi_XC_R2BYTEMSK(REG_SC_BK17_02_L, 0xFF00) >> 8;  //ME2_V Max

	u32MaxHblkSize = u32MaxHblkSize - 1;
	u32MaxVblkSize = u32MaxVblkSize - 1;

	MApi_XC_W2BYTEMSK( REG_SC_BK17_5C_L,  u32MaxHblkSize<<8 , 0xFF00 ); /*reg_stat_xblk_end*/
	MApi_XC_W2BYTEMSK( REG_SC_BK17_5D_L,  u32MaxVblkSize<<8 , 0xFF00 ); /*reg_stat_yblk_end*/
}

void MDrv_SC_Edison_ME1_SetStatisticsWindow(void * pInstance,MS_U32 u16Width, MS_U16 u16Height)
{
    MS_U16 u16MaxHblkSize = 0, u16MaxVblkSize = 0;

	u16MaxHblkSize  = (u16Width/10);  //ME2_H Max
	u16MaxVblkSize = (u16Height/6);  //ME2_V Max

	u16MaxHblkSize  = u16MaxHblkSize - 4;
	u16MaxVblkSize = u16MaxVblkSize - 4;

	MApi_XC_W2BYTEMSK( REG_SC_BK17_5C_L,  u16MaxHblkSize<<8 , 0xFF00 ); /*reg_stat_xblk_end*/
	MApi_XC_W2BYTEMSK( REG_SC_BK17_5D_L,  u16MaxVblkSize<<8 , 0xFF00 ); /*reg_stat_yblk_end*/
}

void MDrv_SC_Edison_GMV_reload( void * pInstance, const XC_GMvStates gMv )
{
    MS_BOOL bisPanScan = FALSE;
    static MS_U8 iir_cnt = 0;
    MS_S8 MvXdir = 0;
    MS_S8 MvYdir = 0;

	if( ( gMv.h1XYsum > 10 ) && ( gMv.h2XYsum > 10 ) && ( gMv.h1XYdiff < 5 ) && ( gMv.h2XYdiff < 5 ) ) //pan-scan case
	{
		bisPanScan = true;
	}
	else
	{
		bisPanScan = false;
	}

	if( bisPanScan )
	{
		iir_cnt = 120; //default:240
	}
	else
	{
		if( iir_cnt != 0 )
			iir_cnt--;
	}

	MvXdir = gMv.minMvXdir - 32;
	MvYdir = gMv.minMvYdir - 8;

	//ME1 refer GMV
	if( iir_cnt == 0 )
	{
		MApi_XC_W2BYTEMSK( REG_SC_BK17_50_L, 0x03  , 0x000F );
		MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, (0x00<<0) , 0x007F );
		MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, (0x00<<8) , 0x1F00 );
		MApi_XC_W2BYTEMSK( REG_SC_BK17_52_L, (0x00<<8) , 0xFF00 );
	}
	else
	{
		MApi_XC_W2BYTEMSK( REG_SC_BK17_50_L, 0x02  , 0x000F );
		MApi_XC_W2BYTEMSK( REG_SC_BK17_52_L, (0x01<<8) , 0xFF00 ); //ME1 favor GMV enable

		//Set ME1 MVX
		if( MvXdir >= 0 )
			MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, MvXdir , 0x007F );
		else
		{
			if( MvXdir == -32)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x0060 , 0x007F );
			else if( MvXdir == -31)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x0061 , 0x007F );
			else if( MvXdir == -30)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x0062 , 0x007F );
			else if( MvXdir == -29)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x0063 , 0x007F );
			else if( MvXdir == -28)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x0064 , 0x007F );
			else if( MvXdir == -27)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x0065 , 0x007F );
			else if( MvXdir == -26)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x0066 , 0x007F );
			else if( MvXdir == -25)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x0067 , 0x007F );
			else if( MvXdir == -24)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x0068 , 0x007F );
			else if( MvXdir == -23)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x0069 , 0x007F );
			else if( MvXdir == -22)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x006A , 0x007F );
			else if( MvXdir == -21)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x006B , 0x007F );
			else if( MvXdir == -20)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x006C , 0x007F );
			else if( MvXdir == -19)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x006D , 0x007F );
			else if( MvXdir == -18)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x006E , 0x007F );
			else if( MvXdir == -17)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x006F , 0x007F );
			else if( MvXdir == -16)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x0070 , 0x007F );
			else if( MvXdir == -15)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x0071 , 0x007F );
			else if( MvXdir == -14)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x0072 , 0x007F );
			else if( MvXdir == -13)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x0073 , 0x007F );
			else if( MvXdir == -12)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x0074 , 0x007F );
			else if( MvXdir == -11)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x0075 , 0x007F );
			else if( MvXdir == -10)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x0076 , 0x007F );
			else if( MvXdir == -9)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x0077 , 0x007F );
			else if( MvXdir == -8)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x0078 , 0x007F );
			else if( MvXdir == -7)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x0079 , 0x007F );
			else if( MvXdir == -6)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x007A , 0x007F );
			else if( MvXdir == -5)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x007B , 0x007F );
			else if( MvXdir == -4)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x007C , 0x007F );
			else if( MvXdir == -3)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x007D , 0x007F );
			else if( MvXdir == -2)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x007E , 0x007F );
			else if( MvXdir == -1)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x007F , 0x007F );
		}

		//Set ME1 MVY
		if( MvYdir >= 0 )
			MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, MvYdir<<8 , 0x1F00 );
		else
		{
			if( MvYdir == -8)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x1800 , 0x1F00 );
			else if( MvYdir == -7)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x1900 , 0x1F00 );
			else if( MvYdir == -6)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x1A00 , 0x1F00 );
			else if( MvYdir == -5)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x1B00 , 0x1F00 );
			else if( MvYdir == -4)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x1C00 , 0x1F00 );
			else if( MvYdir == -3)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x1D00 , 0x1F00 );
			else if( MvYdir == -2)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x1E00 , 0x1F00 );
			else if( MvYdir == -1)
				MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, 0x1F00 , 0x1F00 );
		}
	}
}

#define REG_MCDI_EODIW_SHIFT_MAX 3
#define REG_MCDI_EODIW_SHIFT_MIN 1
#define REG_MCDI_EODIW_AB_MAX_GAIN_MAX 12
#define REG_MCDI_EODIW_AB_MAX_GAIN_MIN 4
#define REG_MCDI_EODIW_MIN_SAD_GAIN_MAX 6
#define REG_MCDI_EODIW_MIN_SAD_GAIN_MIN 1
void MDrv_SC_Agate_SubMvPanScan( void *pInstance, const XC_GMvStates gMv, const MS_BOOL isSceneChange, const MS_BOOL isSdHqvJaggedTest, const MS_U16 motion )
{
    MS_BOOL bisSubMvPanScan = FALSE;
    static MS_U8 u8iir_cnt = 0;
    MS_BOOL bisMv0 = ( ( gMv.minMvXdir == 32 ) && ( gMv.minMvYdir == 8 ) ) ? true : false;
    static MS_U8 u8isMv0Cnt = 0;
    static MS_U8 u8isSubMvPanScanCnt = 0;

    static MS_U16 u16reg_mcdi_eodiw_shift = REG_MCDI_EODIW_SHIFT_MAX;
    static MS_U16 u16reg_mcdi_eodiw_ab_max_gain = REG_MCDI_EODIW_AB_MAX_GAIN_MIN;
    static MS_U16 u16reg_mcdi_eodiw_min_sad_gain = REG_MCDI_EODIW_MIN_SAD_GAIN_MAX;


	if( bisMv0 && ( motion < 0x100 ) )
	{
		if( u8isMv0Cnt < 20 )
			u8isMv0Cnt++;
	}
	else
	{
		if( u8isMv0Cnt > 0 )
			u8isMv0Cnt--;
	}

	if( ( gMv.h1XYsum > 10 ) && ( gMv.h2XYsum > 10 )
		&&( gMv.h1XYdiff < 5 ) && ( gMv.h2XYdiff < 5 ) && gMv.yMvIsMovingOdd )
	{
		bisSubMvPanScan = true;
	}
	else
	{
		bisSubMvPanScan = false;
	}


	if( bisSubMvPanScan )
	{
		if( u8isSubMvPanScanCnt < 20 )
			u8isSubMvPanScanCnt++;
	}
	else
	{
		if( u8isSubMvPanScanCnt > 0 )
			u8isSubMvPanScanCnt--;
	}


	//PRT_TRP(" isSubMvPanScan(%d)\n", isSubMvPanScan);
	//PRT_TRP(" isSubMvPanScan(%d)\n", isSubMvPanScan);

	if( ( u8isSubMvPanScanCnt > 5 ) || isSdHqvJaggedTest )
	{
		u8iir_cnt = 240;
	}
	else
	{
		if( u8iir_cnt != 0 )
			u8iir_cnt--;
	}

	if( isSceneChange ||  ( u8isMv0Cnt > 10 ) ) /* for pattern stable, if only check GMV-Mv0 have problem */
		u8iir_cnt = 0;

	if( u8iir_cnt == 0 )
	{
		if( u16reg_mcdi_eodiw_shift < REG_MCDI_EODIW_SHIFT_MAX )
			u16reg_mcdi_eodiw_shift++;

		if( u16reg_mcdi_eodiw_ab_max_gain > REG_MCDI_EODIW_AB_MAX_GAIN_MIN )
			u16reg_mcdi_eodiw_ab_max_gain--;

		if( u16reg_mcdi_eodiw_min_sad_gain < REG_MCDI_EODIW_MIN_SAD_GAIN_MAX )
			u16reg_mcdi_eodiw_min_sad_gain++;
	}
	else/* do sub pixel */
	{
		if( u16reg_mcdi_eodiw_shift > REG_MCDI_EODIW_SHIFT_MIN )
			u16reg_mcdi_eodiw_shift--;

		if( u16reg_mcdi_eodiw_ab_max_gain < REG_MCDI_EODIW_AB_MAX_GAIN_MAX )
			u16reg_mcdi_eodiw_ab_max_gain++;

		if( u16reg_mcdi_eodiw_min_sad_gain > REG_MCDI_EODIW_MIN_SAD_GAIN_MIN )
			u16reg_mcdi_eodiw_min_sad_gain--;
	}

	MApi_XC_W2BYTEMSK( REG_SC_BK22_29_L, u16reg_mcdi_eodiw_shift  , 0x000F );
	MApi_XC_W2BYTEMSK( REG_SC_BK22_29_L, (u16reg_mcdi_eodiw_ab_max_gain<<12) , 0xF000 );

     //Dummy to control write value, add by Bella 20150708
    if((MApi_XC_R2BYTE(REG_SC_BK23_5D_L)&0x0F00)!=0x0000)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_2A_L, MApi_XC_R2BYTE(REG_SC_BK23_5D_L), 0x0F00);
    }
    else
    {
        MApi_XC_W2BYTEMSK( REG_SC_BK22_2A_L, (u16reg_mcdi_eodiw_min_sad_gain<<8) , 0x0F00 );
    }
}

void MDrv_SC_Agate_PanScan_Church(void *pInstance, const XC_GMvStates gMv, const MS_U16 nonMv0Comfirm, const MS_U32 uComplex, const MS_BOOL meStill, const MS_BOOL isHDsource )
{
    MS_BOOL bisPanScanV = FALSE;
    static MS_U8 iir_cnt = 0;
    MS_U32 colorSum = 0;

    colorSum  = MApi_XC_R2BYTE(REG_SC_BK1A_6E_L);
    colorSum  = colorSum / 8;

    if( (gMv.h1XYsum > 10) && (gMv.h2XYsum > 10) && (gMv.h1XYdiff < 3) && (gMv.h2XYdiff < 3)
            && (gMv.minMvXdir == 32) && (nonMv0Comfirm > 6) &&(colorSum > 0x80) && (uComplex > 0x100)
            && (uComplex < 0x600) && (meStill == false) && (isHDsource == false) ) //vertical moving
    {
            bisPanScanV = true;
    }
    else
    {
            bisPanScanV = false;
    }

    if( bisPanScanV )
    {
        iir_cnt = 120; //default:240
    }
    else
    {
        if( iir_cnt != 0 )
        iir_cnt--;
    }

    if( isHDsource )
        iir_cnt = 0;

    if( iir_cnt == 0 )
    {
        MApi_XC_W2BYTEMSK( REG_SC_BK17_50_L, 0x03  , 0x000F );
        MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, (0x00<<8) , 0xFF00 );
        MApi_XC_W2BYTEMSK( REG_SC_BK17_52_L, (0x00<<8) , 0xFF00 );
        MApi_XC_W2BYTEMSK( REG_SC_BK17_54_L, (0x10) , 0x00FF );
     }
    else
    {
        MApi_XC_W2BYTEMSK( REG_SC_BK17_50_L, 0x01  , 0x000F );
        MApi_XC_W2BYTEMSK( REG_SC_BK17_51_L, (0x02<<8) , 0xFF00 );
        MApi_XC_W2BYTEMSK( REG_SC_BK17_52_L, (0x02<<8) , 0xFF00 );
        MApi_XC_W2BYTEMSK( REG_SC_BK17_54_L, (0x6C) , 0x00FF );
     }
}

void MDrv_SC_FilmReorder(void *pInstance, const XC_OpFrameStates opFrameStatus )
{
	static XC_FilmRoStates filmRoStates;
	static MS_BOOL init = true;
	static MS_U8 samplingTime = 0;
//	MS_BOOL isPoolOfFull;
    int vaildCnt = 0;
    int i = 0;

	samplingTime++;

	if( init )
	{
		for( i=0; i < 16 ; i++ )
		{
			filmRoStates.motion[i] = 0;
			filmRoStates.unmatch_1[i] = 0;
			filmRoStates.unmatch_3[i] = 0;
			filmRoStates.vaildCnt[i] = 0;
			filmRoStates.vaild[i] = false;
		}
		init = false;
	}

	/* Step0. loading states */
	filmRoStates.motion[opFrameStatus.curOpFrame] 			= MApi_XC_R2BYTEMSK(REG_SC_BK0A_1A_L, 0xFFFF);
	filmRoStates.unmatch_1[opFrameStatus.curOpFrame] 		= MApi_XC_R2BYTEMSK(REG_SC_BK0A_0A_L, 0xFFFF);
	filmRoStates.unmatch_3[opFrameStatus.curOpFrame] 		= MApi_XC_R2BYTEMSK(REG_SC_BK0A_0B_L, 0xFFFF);

	if( filmRoStates.vaildCnt[opFrameStatus.curOpFrame] < 255 )
		filmRoStates.vaildCnt[opFrameStatus.curOpFrame]++;

	filmRoStates.vaild[opFrameStatus.curOpFrame] 	      = true;

	/* Step1. 1s do clean unflash counter */
	if( samplingTime == 60 )
	{
		for( i = 0; i < opFrameStatus.maxOpFrame; i++ )
		{
			if( filmRoStates.vaildCnt[i] < 10 )
			{
				filmRoStates.vaild[i] 	      = false;
			}
		}
		samplingTime = 0;/* reset of sampling time */
	}

	/* Step2. Check pool is full */
	vaildCnt = 0;
	for( i = 0; i < opFrameStatus.maxOpFrame; i++ )
	{
		if( filmRoStates.vaild[i] )
			vaildCnt++;
	}
//	isPoolOfFull = ( vaildCnt > (opFrameStatus.maxOpFrame/2) ) ? true : false;

}

XC_HpfDefaultRegSetting MDrv_SC_SaveHpfSetting(void *pInstance, const MS_U8 isSourceChange )
{
	static XC_HpfDefaultRegSetting reg;

	if(  isSourceChange == 1 )
	{

		reg.HDSDD0_det_mode = (MS_BOOL)( MApi_XC_R2BYTEMSK(REG_SC_BK02_70_L, 0x0001) );
		reg.HDSDD1_det_mode = (MS_BOOL)( MApi_XC_R2BYTEMSK(REG_SC_BK02_70_L, 0x0010) >> 4 );

		reg.HDSDD0_det_mode_shift = (MS_U8)( MApi_XC_R2BYTEMSK(REG_SC_BK02_70_L, 0x0700) >> 8 );
		reg.HDSDD1_det_mode_shift = (MS_U8)( MApi_XC_R2BYTEMSK(REG_SC_BK02_70_L, 0x7000) >> 12 );

		reg.HDSDD0_det_mode_threshold = (MS_U16)( MApi_XC_R2BYTEMSK(REG_SC_BK02_71_L, 0xFFFF)  );
		reg.HDSDD1_det_mode_threshold = (MS_U16)( MApi_XC_R2BYTEMSK(REG_SC_BK02_72_L, 0xFFFF)  );

		reg.HDSDD_underflow_threshold = (MS_U8)( MApi_XC_R2BYTEMSK(REG_SC_BK02_73_L, 0x003F)  );
		reg.HDSDD_overflow_threshold = (MS_U8)( MApi_XC_R2BYTEMSK(REG_SC_BK02_73_L, 0x3F00) >> 8 );

		//reg.HDSD_detection_letter_box_enable = (MS_BOOL)( MApi_XC_R2BYTEMSK(REG_SC_BK02_70_L, 0x0080) >> 7 );
	}
	return reg;
}

#if 0
void print_hpfSetting(void)
{
	MS_BOOL HDSDD0_det_mode = (MS_BOOL)( MApi_XC_R2BYTEMSK(REG_SC_BK02_70_L, 0x0001) );
	MS_BOOL HDSDD1_det_mode = (MS_BOOL)( MApi_XC_R2BYTEMSK(REG_SC_BK02_70_L, 0x0010) >> 4 );

	MS_U8 HDSDD0_det_mode_shift = (MS_U8)( MApi_XC_R2BYTEMSK(REG_SC_BK02_70_L, 0x0700) >> 8 );
	MS_U8 HDSDD1_det_mode_shift = (MS_U8)( MApi_XC_R2BYTEMSK(REG_SC_BK02_70_L, 0x7000) >> 12 );

	MS_U16 HDSDD0_det_mode_threshold = (MS_U16)( MApi_XC_R2BYTEMSK(REG_SC_BK02_71_L, 0xFFFF)  );
	MS_U16 HDSDD1_det_mode_threshold = (MS_U16)( MApi_XC_R2BYTEMSK(REG_SC_BK02_72_L, 0xFFFF)  );

	MS_U8 HDSDD_underflow_threshold = (MS_U8)( MApi_XC_R2BYTEMSK(REG_SC_BK02_73_L, 0x003F)  );
	MS_U8 HDSDD_overflow_threshold  = (MS_U8)( MApi_XC_R2BYTEMSK(REG_SC_BK02_73_L, 0x3F00) >> 8 );

	MS_U16 widthCur = MApi_XC_R2BYTEMSK(REG_SC_BK12_0F_L, 0x0FFF);/* cur width */
	//MS_U16 boundarystart = (MS_U16)( MApi_XC_R2BYTEMSK(REG_SC_BK02_7C_L, 0x1FFF)  );
	//MS_U16 boundaryend   = (MS_U16)( MApi_XC_R2BYTEMSK(REG_SC_BK02_7D_L, 0x1FFF)  );
	MS_U16 uxx = (MS_U16)( MApi_XC_R2BYTEMSK(REG_SC_BK02_70_L, 0x0080)  );

	//PRT_TRP("det_mode_0 = %d, det_mode_1 = %d, det_mode_shift_0 = %d, det_mode_shift_1 = %d, det_mode_threshold_0 = %d, det_mode_threshold_1 = %d, HDSDD_underflow_threshold = %d, HDSDD_overflow_threshold = %d\n",
	HDSDD0_det_mode, HDSDD1_det_mode, HDSDD0_det_mode_shift, HDSDD1_det_mode_shift, HDSDD0_det_mode_threshold, HDSDD1_det_mode_threshold, HDSDD_underflow_threshold, HDSDD_overflow_threshold);

	//PRT_TRP("widthCur = %d, boundarystart = %d, boundaryend = %d, uxx = %d\n", widthCur, boundarystart, boundaryend, uxx);
}
#endif

void MDrv_SC_hpfDefaultRelod(void *pInstance, const XC_HpfDefaultRegSetting defaultReg )
{
	//const int boundary = 10;
	//MS_U16 widthCur = MApi_XC_R2BYTEMSK(REG_SC_BK12_0F_L, 0x0FFF);/* cur width */


//	MApi_XC_W2BYTEMSK(REG_SC_BK02_72_L, defaultReg.HDSDD1_det_mode_threshold, 0xFFFF);
//	MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, (defaultReg.HDSDD1_det_mode_shift)<<12, 0x7000);

//	MApi_XC_W2BYTEMSK(REG_SC_BK02_71_L, defaultReg.HDSDD0_det_mode_threshold, 0xFFFF);  /*for HD repot*/
//	MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, (defaultReg.HDSDD0_det_mode_shift)<<8, 0x0700); /*for HD repot*/

//	MApi_XC_W2BYTEMSK(REG_SC_BK02_73_L, defaultReg.HDSDD_underflow_threshold , 0x003F); /* underflow */
//	MApi_XC_W2BYTEMSK(REG_SC_BK02_73_L, (defaultReg.HDSDD_overflow_threshold)<<8, 0x3F00); /* overflow */

//	MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, defaultReg.HDSDD0_det_mode , 0x0001); /* mode */
//	MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, (defaultReg.HDSDD1_det_mode)<<4, 0x0010); /* mode */

	MApi_XC_W2BYTEMSK(REG_SC_BK02_72_L, 0x0190, 0xFFFF);
	MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x4000, 0x7000);

	MApi_XC_W2BYTEMSK(REG_SC_BK02_71_L, 0x0190, 0xFFFF);  /*for HD repot*/
	MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x0200, 0x0700); /*for HD repot*/

	MApi_XC_W2BYTEMSK(REG_SC_BK02_73_L, 0x000C, 0x003F); /* underflow */
	MApi_XC_W2BYTEMSK(REG_SC_BK02_73_L, 0x3200, 0x3F00); /* overflow */

	MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x0001 , 0x0001); /* mode */
	MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x0010, 0x0010); /* mode */


//	MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x0080, 0x0080);
//	MApi_XC_W2BYTEMSK(REG_SC_BK02_7C_L, boundary, 0x1FFF); /* set start */
//	MApi_XC_W2BYTEMSK(REG_SC_BK02_7D_L, widthCur-boundary, 0x1FFF);/* set end */
}


/*#define TIMEID_LRLETTERBOX 0
MS_BOOL MDrv_SC_Edison_LrLetterBox( const XC_OpFrameStates opFrameStatusCur, const XC_HpfDefaultRegSetting defaultReg, const MS_U8 _timeSharingID, const MS_U8 timingSegment )
{
	const int boundary = 10;
	static XC_OpFrameStates opFrameStatusLast;
    MS_U16 widthCur = 0;
	static MS_U16 widthLast;
	static MS_U8 operationCnt = 0;
	static int maxBreadthDetermine = 0;
    MS_U32 sdCntActL_cur = 0, sdCntActR_cur = 0;
	static MS_U32 sdCntActL_last = 0xFFFF;
	static MS_U32 sdCntActR_last = 0xFFFF;
	static int sdCntActLcnt = 0;
	static int sdCntActRcnt = 0;
	static MS_BOOL isL_letterBox = false;
	static MS_BOOL isR_letterBox = false;

	static MS_BOOL _return = false;

	widthCur = MApi_XC_R2BYTEMSK(REG_SC_BK12_0F_L, 0x0FFF);//cur width

	if( _timeSharingID == TIMEID_LRLETTERBOX )
	{
		if( timingSegment == 0 )
			MDrv_SC_hpfDefaultRelod( defaultReg );
		else
		{
			//enable window
			MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x0080, 0x0080);

			// max width ratio

			if( widthCur != widthLast )
			{
				maxBreadthDetermine = widthCur / 12;
			}

			// L/R width switch
			if( isL_letterBox )// if "L letter box" ready then do "R letter box"
			{
				//R side
				MApi_XC_W2BYTEMSK(REG_SC_BK02_7C_L, widthCur-boundary-maxBreadthDetermine, 0x1FFF);// set start
				MApi_XC_W2BYTEMSK(REG_SC_BK02_7D_L, widthCur-boundary, 0x1FFF);//set end
			}
			else
			{
				//L side
				MApi_XC_W2BYTEMSK(REG_SC_BK02_7C_L, 0+boundary, 0x1FFF); //set start
				MApi_XC_W2BYTEMSK(REG_SC_BK02_7D_L, maxBreadthDetermine+boundary, 0x1FFF);//set end
			}

			if( opFrameStatusCur.curOpFrame != opFrameStatusLast.curOpFrame )
			{
				if(operationCnt%2)
				{
					sdCntActL_cur = (MApi_XC_R2BYTE(REG_SC_BK02_77_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_76_L);
					sdCntActL_cur = ( sdCntActL_last*3 + sdCntActL_cur ) >> 2;
					sdCntActL_last = sdCntActL_cur;

					if( sdCntActL_last < 3 )
					{
						if( sdCntActLcnt < 256 ) // 256 = counter max
							sdCntActLcnt++;
					}
					else
					{
						if( sdCntActLcnt > 5 )
							sdCntActLcnt = sdCntActLcnt - 5;
					}
				}
				else
				{
					sdCntActR_cur = (MApi_XC_R2BYTE(REG_SC_BK02_77_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_76_L);
					sdCntActR_cur = ( sdCntActR_last*3 + sdCntActR_cur ) >> 2;
					sdCntActR_last = sdCntActR_cur;

					if( sdCntActR_last < 3 )
					{
						if( sdCntActRcnt < 256 )// 256 = counter max
							sdCntActRcnt++;
					}
					else
					{
						if( sdCntActRcnt > 5 )
							sdCntActRcnt = sdCntActRcnt - 5;
					}
				}
				operationCnt++;
			}

			isL_letterBox = ( sdCntActLcnt > 120 ) ? true : false;
			isR_letterBox = ( sdCntActRcnt > 120 ) ? true : false;

			opFrameStatusLast.curOpFrame = opFrameStatusCur.curOpFrame;
			widthLast = widthCur;

			_return = (isL_letterBox && isR_letterBox);
		}
	}
	else
	{
		//disable window
		//MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x0000, 0x0080);
		//MApi_XC_W2BYTEMSK(REG_SC_BK02_7C_L, boundary, 0x1FFF); // set start
		//    MApi_XC_W2BYTEMSK(REG_SC_BK02_7D_L, widthCur-boundary, 0x1FFF);// set end
	}

	return _return;
}
*/
MS_BOOL MDrv_SC_McFilmLike(void *pInstance, const XC_OpFrameStates opFrameStatusCur, MS_BOOL isHd )
{
	static XC_OpFrameStates opFrameStatusLast;
	static XC_McFilmStates mcFilmStates;
    MS_BOOL  isFilm = FALSE;
	static MS_U8 isFilmCnt = 0;
	static MS_U8 continuousError = 0;
	MS_U8 lowerBoundTh = 0;

	if( isHd )
		lowerBoundTh = 0xFF;
	else
      #if SZ_PATTERN_ENABLE
      lowerBoundTh = 0x80;
      #else
			lowerBoundTh = 0x40;
      #endif

	mcFilmStates.filmLikePrvPixels = MApi_XC_R2BYTEMSK(REG_SC_BK2A_75_L, 0xFFFF);
	mcFilmStates.filmLikeNxtPixels = MApi_XC_R2BYTEMSK(REG_SC_BK2A_76_L, 0xFFFF);

	if( opFrameStatusCur.curOpFrame != opFrameStatusLast.curOpFrame )
	{
		if(((mcFilmStates.filmLikePrvPixels>(mcFilmStates.filmLikeNxtPixels*5))&&(mcFilmStates.filmLikePrvPixels > lowerBoundTh))
			||((mcFilmStates.filmLikeNxtPixels>(mcFilmStates.filmLikePrvPixels*5))&&(mcFilmStates.filmLikeNxtPixels > lowerBoundTh)))
		{
			if( isFilmCnt < 255 )
				isFilmCnt++;

			continuousError = 0;
		}
		else
		{
			if( isFilmCnt > 0 )
				isFilmCnt--;

			if( continuousError < 255 )
				continuousError++;
		}

		if( continuousError > 4 )
			isFilmCnt = 0;
	}

	opFrameStatusLast.curOpFrame = opFrameStatusCur.curOpFrame;

	isFilm = ( isFilmCnt > 10 ) ? true : false;

	return isFilm;
}
MS_BOOL MDrv_SC_McFilmUkoSabihS(void *pInstance, const XC_OpFrameStates opFrameStatusCur, MS_BOOL isHd, MS_U32 _hpfHdCntReport )
{
	static XC_OpFrameStates opFrameStatusLast;
	static XC_McFilmStates mcFilmStates;
	MS_BOOL  isFilm;
	static MS_U8 isFilmCnt = 0;
	static MS_U8 continuousError = 0;
	MS_U8 lowerBoundTh = 0;

	if( isHd )
	{
		if (_hpfHdCntReport >= 50000)
			lowerBoundTh = 0xFF;//255
		else if (_hpfHdCntReport >= 32000)
			lowerBoundTh = 128+ 127*(_hpfHdCntReport-32000)/18000;
		else if (_hpfHdCntReport >= 6400)
			lowerBoundTh = 64+ 16*(_hpfHdCntReport-6400)/6400;
		else //if (_hpfHdCntReport <= 6400)
			lowerBoundTh = _hpfHdCntReport/100;
	}
	else
		lowerBoundTh = 0x80;

	mcFilmStates.filmLikePrvPixels = MApi_XC_R2BYTEMSK(REG_SC_BK2A_75_L, 0xFFFF);
	mcFilmStates.filmLikeNxtPixels = MApi_XC_R2BYTEMSK(REG_SC_BK2A_76_L, 0xFFFF);

	MS_U32 filmLikePrvPixelsAdj = __max(mcFilmStates.filmLikePrvPixels - lowerBoundTh*2, 0);
	MS_U32 filmLikeNxtPixelsAdj = __max(mcFilmStates.filmLikeNxtPixels - lowerBoundTh*2, 0);
	//DEBUG_LC_Full6bit3(__max(mcFilmStates.filmLikePrvPixels, mcFilmStates.filmLikeNxtPixels )/16);
	//DEBUG_LC_Full6bit4(__min(mcFilmStates.filmLikePrvPixels, mcFilmStates.filmLikeNxtPixels )/16);
	//DEBUG_LC_Full6bit5(lowerBoundTh);
	if( opFrameStatusCur.curOpFrame != opFrameStatusLast.curOpFrame )
	{
		if( (filmLikePrvPixelsAdj > 0) && (filmLikeNxtPixelsAdj > 0) &&
			( (filmLikePrvPixelsAdj>filmLikeNxtPixelsAdj*5 && filmLikePrvPixelsAdj > lowerBoundTh)
			||(filmLikeNxtPixelsAdj>filmLikePrvPixelsAdj*5 && filmLikeNxtPixelsAdj > lowerBoundTh) ) )
		{
			if( isFilmCnt < 63 )
				isFilmCnt++;

			continuousError = 0;
		}
		else if(((mcFilmStates.filmLikePrvPixels>(mcFilmStates.filmLikeNxtPixels*5))&&(mcFilmStates.filmLikePrvPixels > lowerBoundTh))
			||((mcFilmStates.filmLikeNxtPixels>(mcFilmStates.filmLikePrvPixels*5))&&(mcFilmStates.filmLikeNxtPixels > lowerBoundTh)))
		{
			if( isFilmCnt < 63 )
				isFilmCnt++;

			continuousError = 0;
		}
		else
		{
			if( isFilmCnt > 0 )
				isFilmCnt--;

			if( continuousError < 63 )
				continuousError++;
		}

		if( continuousError > 4 )
			isFilmCnt = 0;
	}

	opFrameStatusLast.curOpFrame = opFrameStatusCur.curOpFrame;

	isFilm = ( isFilmCnt > 10 ) ? true : false;
	//DEBUG_LC_Full6bit2(isFilmCnt);

	return isFilm;
}
MS_U8 MDrv_SC_Motion4StillCnt(void *pInstance, const MS_U16 motion4 )
{
	static MS_U8 motionCnt = 0;

	if( motion4 == 0 )
	{
	 //motionCnt = motionCnt < 12 ? motionCnt += 3 : 15);

	 if(motionCnt < 12)
	 {
	  motionCnt += 3;
	 }
	 else
	 {
	  motionCnt =15;
	 }
	}
	   else
	   {
	 //motionCnt = motionCnt > 3 ? motionCnt -= 3 : 0);
	 if(motionCnt > 3 )
	 {
	   motionCnt -= 3;
	}
	else
	{
	  motionCnt =0;
	  }
	}

	return motionCnt;
}

MS_U8 MDrv_SC_Me1Still(void *pInstance, const MS_U16 me13x3MwMax, const MS_U16 mv0Cost )
{
	static MS_U8 cnt = 0;

	if( ( me13x3MwMax < 30 ) && ( mv0Cost < 30) )
	{
		if( cnt < 15 )
			cnt++;
	}
	else
	{
		if( cnt > 0 )
			cnt--;
	}

	return cnt;
}

MS_U8 MDrv_SC_McSmallMoving(void *pInstance, const MS_U16 me13x3MwMin, const MS_U16 mv0Cost )
{
	static MS_U8 smallMovingCnt = 0;

	if( ( mv0Cost>>8 ) > me13x3MwMin )
	{
		if( smallMovingCnt < 255 )
			smallMovingCnt++;
	}
	else
	{
		if( smallMovingCnt > 0 )
			smallMovingCnt--;
	}

	return smallMovingCnt;
}

MS_BOOL MDrv_SC_McFilmLikeHd(void *pInstance, const XC_OpFrameStates opFrameStatusCur, MS_BOOL isHd )
{
	static XC_OpFrameStates opFrameStatusLast;
	static XC_McFilmStates mcFilmStates;
    MS_BOOL  isFilm = FALSE;
	static MS_U8 isFilmCnt = 0;
	static MS_U8 continuousError = 0;
	MS_U8 lowerBoundTh = 0;

	if( isHd )
		lowerBoundTh = 0xFF;
	else
		lowerBoundTh = 0x40;

	mcFilmStates.filmLikePrvPixels = MApi_XC_R2BYTEMSK(REG_SC_BK2A_75_L, 0xFFFF);
	mcFilmStates.filmLikeNxtPixels = MApi_XC_R2BYTEMSK(REG_SC_BK2A_76_L, 0xFFFF);

	if( opFrameStatusCur.curOpFrame != opFrameStatusLast.curOpFrame )
	{
		if(((mcFilmStates.filmLikePrvPixels>(mcFilmStates.filmLikeNxtPixels*15))&&(mcFilmStates.filmLikePrvPixels > lowerBoundTh ))      /*Sam modify*/
			||((mcFilmStates.filmLikeNxtPixels>(mcFilmStates.filmLikePrvPixels*15))&&(mcFilmStates.filmLikeNxtPixels > lowerBoundTh )))  /*Sam modify*/
		{
			if( isFilmCnt < 255 )
				isFilmCnt++;

			continuousError = 0;
		}
		else
		{
			if( isFilmCnt > 0 )
				isFilmCnt--;

			if( continuousError < 255 )
				continuousError++;
		}

		if( continuousError > 4 )
			isFilmCnt = 0;
	}

	opFrameStatusLast.curOpFrame = opFrameStatusCur.curOpFrame;

	isFilm = ( isFilmCnt > 10 ) ? true : false;

	return isFilm;
}

MS_BOOL MDrv_SC_McFilmLikeAny(void *pInstance, const XC_OpFrameStates opFrameStatusCur, const MS_BOOL isMcFilmLike, MS_BOOL isHd )
{
	static XC_OpFrameStates opFrameStatusLast;
	static XC_McFilmStates mcFilmStates;
    MS_BOOL  isFilmAny = FALSE;
	static MS_U8 isFilmCnt = 0;
	static MS_U8 continuousError = 0;
	static MS_U8 continuousZero = 0;
	static MS_U16 maxFilmLikeDiff = 0;
	MS_U8 lowerBoundTh = 0;

	if( isHd )
		lowerBoundTh = 0xFF;
	else
		lowerBoundTh = 0x80;

	mcFilmStates.filmLikePrvPixels = MApi_XC_R2BYTEMSK(REG_SC_BK2A_75_L, 0xFFFF);
	mcFilmStates.filmLikeNxtPixels = MApi_XC_R2BYTEMSK(REG_SC_BK2A_76_L, 0xFFFF);

	if( opFrameStatusCur.curOpFrame != opFrameStatusLast.curOpFrame )
	{

		if(((mcFilmStates.filmLikePrvPixels>(mcFilmStates.filmLikeNxtPixels*5))&&(mcFilmStates.filmLikePrvPixels > lowerBoundTh))
			||((mcFilmStates.filmLikeNxtPixels>(mcFilmStates.filmLikePrvPixels*5))&&(mcFilmStates.filmLikeNxtPixels > lowerBoundTh)))
		{
			if( isFilmCnt < 255 )
				isFilmCnt++;

			/* save maximum of diff */
			if( mcFilmStates.filmLikePrvPixels > mcFilmStates.filmLikeNxtPixels )
				maxFilmLikeDiff = mcFilmStates.filmLikePrvPixels;
			else
				maxFilmLikeDiff = mcFilmStates.filmLikeNxtPixels;

			continuousError = 0;
			continuousZero = 15; /* Keep to 10 * 1.5x times "O" */

		}
		else if(  ( maxFilmLikeDiff > mcFilmStates.filmLikePrvPixels*5 ) && ( maxFilmLikeDiff > mcFilmStates.filmLikeNxtPixels*5 ) && ( continuousZero > 0 ) )
		{
			if( continuousZero > 0 )
				continuousZero--;
		}
		else
		{
			if( isFilmCnt > 0 )
				isFilmCnt--;

			if( continuousError < 255 )
				continuousError++;

			continuousZero = 0;
			maxFilmLikeDiff = 0;
		}

		if( continuousError > 4 )
		{
			isFilmCnt = 0;
		}
	}

	opFrameStatusLast.curOpFrame = opFrameStatusCur.curOpFrame;

	isFilmAny = ( isFilmCnt > 10 ) ? true : false;

	return ( isFilmAny && ( !isMcFilmLike ) ); /* Film-Any(without 2:3:3:2/2:2:2:4) and Film-32/22 will not into same condition */
}

MS_BOOL MDrv_SC_McFilmLikeForSlowMotion(void *pInstance, const MS_U32 motion, const XC_GMvStates gMv  )
{
	MS_BOOL isMv0 = ((gMv.minMvXdir == 32)&&(gMv.minMvYdir == 8)) ? true : false;
	MS_BOOL maxBinFocus = ( gMv.h1XYsum > 0x40 ) ? true : false;
	MS_BOOL isStill = ( motion < 0x100 ) ? true : false;

	static XC_McFilmStates mcFilmStates;
    MS_BOOL  isFilm = FALSE;
	static MS_U8 isFilmCnt = 0;
	static MS_U8 continuousError = 0;

	mcFilmStates.filmLikePrvPixels = MApi_XC_R2BYTEMSK(REG_SC_BK2A_75_L, 0xFFFF);
	mcFilmStates.filmLikeNxtPixels = MApi_XC_R2BYTEMSK(REG_SC_BK2A_76_L, 0xFFFF);

	if( (mcFilmStates.filmLikePrvPixels>(mcFilmStates.filmLikeNxtPixels*1) )
		|| (mcFilmStates.filmLikeNxtPixels>(mcFilmStates.filmLikePrvPixels*1) ) )
	{
		if( isFilmCnt < 255 )
			isFilmCnt++;

		continuousError = 0;
	}
	else
	{
		if( isFilmCnt > 0 )
			isFilmCnt--;

		if( continuousError < 255 )
			continuousError++;
	}

	if( continuousError > 10 )
		isFilmCnt = 0;

	//isFilm = ( isFilmCnt > 10 ) ? true : false;
	isFilm = ( isFilmCnt > 5 ) ? true : false;

	return ( isFilm && isMv0 && maxBinFocus && isStill );
}

MS_BOOL MDrv_SC_HQV_JaggedTestPattern(void *pInstance, const XC_GMvStates gMv )
{
	MS_BOOL isMvX0 = (gMv.minMvXdir == 32) ? true : false;
	MS_BOOL isOddY = ( gMv.minMvYdir % 2 ) ? true : false;
	MS_BOOL maxBinNonFocus = ( gMv.h1XYsum < 0x10 ) ? true : false;

	if( isMvX0 && isOddY && maxBinNonFocus )
    {
        return  true;
		//printf("MDrv_SC_HQV_JaggedTestPattern == TRUE\n\n");
    }
    else
    {
        return false;
		//printf("MDrv_SC_HQV_JaggedTestPattern == FALSE\n\n");
    }
	//printf("isMvX0 == %d, isOddY == %d, maxBinNonFocus == %d\n\n", isMvX0, isOddY, maxBinNonFocus);
}

#define SPIKE_NR_MAX 15
#define SPIKE_NR_MIN 2
void MDrv_SC_HQV_RandomNoise(void *pInstance, const XC_GMvStates gMv,  const MS_U16 mvNotFound, const MS_U32 _hdFilterBaseReport,  const MS_U32 u32MotionValue_Sum )
{
	MS_BOOL isMv0 = ((gMv.minMvXdir == 32)&&(gMv.minMvYdir == 8)) ? true : false;
	MS_BOOL maxBinFocus = ( gMv.h1XYsum > 0xE0 ) ? true : false;
	MS_BOOL isMoving = ( u32MotionValue_Sum > 0x02 ) ? true : false;
	MS_BOOL notSoHighFreq = ( _hdFilterBaseReport < 0x700 ) ? true : false;
	MS_BOOL isNotFound =  ( mvNotFound > 0x300 ) ? true : false;

	static MS_BOOL isHqvRandomNoise = false;
	static MS_U8 confirmCnt = 0;
	static MS_U8 intensity = 0;

	if( isMv0 && maxBinFocus && isMoving && notSoHighFreq && isNotFound )
	{
		if( confirmCnt < 60 )
			confirmCnt++;
	}
	else
		confirmCnt = 0;

	isHqvRandomNoise = ( confirmCnt > 10 ) ? true : false;

	if( isHqvRandomNoise )
	{
		if( intensity < SPIKE_NR_MAX )
			intensity++;
	}
	else
	{
		if( intensity > SPIKE_NR_MIN )
			intensity--;
	}
	MApi_XC_W2BYTEMSK( REG_SC_BK26_50_L,  (MS_U16)( intensity << 8 )  , 0x0F00 );

	if( isHqvRandomNoise ) /* Noise trun-off noise masking motion protection */
		MApi_XC_W2BYTEMSK( REG_SC_BK26_50_L,  0x0000/*Off*/ , 0x0008 );
	else
		MApi_XC_W2BYTEMSK( REG_SC_BK26_50_L,  0x0008/*On*/ , 0x0008 );
	//return isHqvRandomNoise;
}

#define MF_Gain            6
MS_BOOL MDrv_SC_ZoomMoving( void *pInstance, const XC_GMvStates gMv , const MS_U16 me1IsNonMv0, const MS_U16 me1IsNotFound, const MS_U16 featherAndMotion, const MS_U16 motion )
{

	//MS_BOOL isMvY0 = (gMv.minMvYdir == 8) ? true : false;
	MS_BOOL isMvY0 = (( abs(32 - gMv.minMvXdir) < 4 )&&(gMv.minMvYdir == 8)) ? true : false;
	//MS_BOOL maxBin = true;//( gMv.h1XYsum < 0x30 && gMv.h1XYsum > 0x04 ) ? true : false;
	MS_BOOL isMoving = ( motion > 0x200 ) ? true : false;

    MS_BOOL isSmallMoving = FALSE;
	static MS_U16 prev_featherAndMotion = 0;
	MS_U16 cur_featherAndMotion = 0;
	MS_BOOL withoutFeather = false;
	static MS_U8 confirm_cnt = 0;

	cur_featherAndMotion = featherAndMotion;

	//denyMoving = ( me1IsNonMv0 + me1IsNotFound );

	cur_featherAndMotion = (prev_featherAndMotion*3 + cur_featherAndMotion ) >> 2;
	prev_featherAndMotion = cur_featherAndMotion;

	withoutFeather = ( cur_featherAndMotion < 8 ) ? true : false;

	if( (( me1IsNotFound > (me1IsNonMv0*MF_Gain)) && ( me1IsNotFound > 0x30 )) && withoutFeather && isMvY0 && isMoving )
		isSmallMoving = true;
	else
		isSmallMoving = false;

	if( isSmallMoving )
	{
		if( confirm_cnt < 40 )
			confirm_cnt++;
	}
	else
	{
		if( confirm_cnt > 0 )
			confirm_cnt--;
	}


	return ( confirm_cnt > 30 );
}


void MDrv_SC_EODi_Armored(void *pInstance, const XC_GMvStates gMv , const MS_U16 mv0Comfirm , const MS_U16 nonMv0Comfirm )
{
    MS_BOOL is_still = FALSE;
	MS_BOOL isMv0 = ( ( gMv.minMvXdir == 32 ) && ( gMv.minMvYdir == 8 ) ) ? true : false;
	static MS_U8 confirm_cnt = 0;
	static MS_BOOL reg_eodi_pwsam_en = false;

	if( isMv0 && (mv0Comfirm > 0x15) && (nonMv0Comfirm < 8))
		is_still = true;
	else
		is_still = false;

	if( is_still )
	{
		confirm_cnt = confirm_cnt + 5;

		if(confirm_cnt > 20)
			confirm_cnt = 20;
	}
	else
	{
		if(confirm_cnt > 0)
			confirm_cnt--;
		else
			confirm_cnt = 0;
	}

	if( confirm_cnt >= 10)
	{
		reg_eodi_pwsam_en = true;
	}
	else
	{
		reg_eodi_pwsam_en = false;
	}
	MApi_XC_W2BYTEMSK( REG_SC_BK21_3D_L, (MS_U16)( reg_eodi_pwsam_en << 3) , 0x0008 );
}

#define S_UPPERBOUND_TH 15
#define S_MATCH_TH 10
MS_BOOL MDrv_SC_Agate_ukoSabihS(void *pInstance, const XC_ApiStatus *stXCStatus, const XC_GMvStates gMv ,const MS_U16 nonMv0Comfirm, const XC_FilmStates filmstatus, const MS_BOOL isMcFilm,
				MS_BOOL *isUkosabihsMotion, MS_BOOL *isUkosabihsMc, MS_BOOL *isUkosabihsForFilm, const MS_U16 mv0Comfirm, MS_BOOL *isUkosabihsMv0, const MS_BOOL isHDSrc )
{
    MS_BOOL is_ukoSabihS = FALSE, is_ukoSabihSmv0 = FALSE;
	MS_BOOL isMv0 = (( gMv.minMvXdir == 32 ) && ( gMv.minMvYdir == 8 )) ? true : false;
	MS_BOOL isFilm = ( filmstatus.film_act || isMcFilm ) ? true : false;
	MS_BOOL isWithoutMv0Comfirm = ( mv0Comfirm < 0x20 ) ? true : false;
	MS_BOOL isCVBSinput,isTVinput;
    MS_U8 XYDiffTH= 0,SpeedUp_TH = 0,Confirm_TH = 0;
    int h2XYsumTH = 0, h2XYsumTH2 = 0,nonMv0ComfirmTH = 0, mv0ComfirmTH=0;

	static MS_U8 confirm_cnt = 0;
	static MS_U8 confirm_cnt_mv0 = 0;
	static MS_U8 even_cnt = 0;

	static MS_U8 confirm_cnt_film = 0;

	//MS_BOOL is_ukoSabihS_isHBurst = ( isHBurstMoving && ( gMv.h2XYdiff > 0x45 )) ? true : false;


	isCVBSinput = ((stXCStatus->bInterlace) && (stXCStatus->enInputSourceType >= INPUT_SOURCE_CVBS)&& (stXCStatus->enInputSourceType <= INPUT_SOURCE_CVBS_MAX));
	isTVinput = ((stXCStatus->bInterlace) && (stXCStatus->enInputSourceType == INPUT_SOURCE_TV));

	if(isHDSrc)
	{
		SpeedUp_TH = 200;
	}
	else
	{
		SpeedUp_TH = 40;
	}

	if(isCVBSinput || isTVinput)
	{
		XYDiffTH = 0x01;
    Confirm_TH =  0x80;
	}
	else
	{
		XYDiffTH = 0x06;
    Confirm_TH =0x40 ;
	}

    SpeedUp_TH = 200;
    nonMv0ComfirmTH = 0x40;
    mv0ComfirmTH = 0x200;
    h2XYsumTH = 0x16;
    h2XYsumTH2 = 0x32;

    nonMv0ComfirmTH = (nonMv0ComfirmTH * normalize_factor) / 256;
    mv0ComfirmTH = (mv0ComfirmTH * normalize_factor) / 256;
    h2XYsumTH = (h2XYsumTH * normalize_factor) / 256;
    h2XYsumTH2 = (h2XYsumTH2 * normalize_factor) / 256;
    SpeedUp_TH = (SpeedUp_TH * normalize_factor) / 256;
    XYDiffTH = (XYDiffTH *normalize_factor) / 256;
    Confirm_TH = (Confirm_TH *normalize_factor) / 256;


	if(isCVBSinput||isTVinput)
	{
		is_ukoSabihS = false;
	}
    else if((gMv.h2XYsum > h2XYsumTH)&& (gMv.h2XYdiff <= XYDiffTH)&& (!isMv0) && (nonMv0Comfirm> Confirm_TH ) && isWithoutMv0Comfirm && (isHDSrc) )
    {
        is_ukoSabihS = true;
    }
    else if((gMv.h2XYsum > h2XYsumTH2)&& (gMv.h2XYdiff <= XYDiffTH)&& (!isMv0) && (nonMv0Comfirm> Confirm_TH ) && isWithoutMv0Comfirm )
   {
		is_ukoSabihS = true;
	}
	else
	{
		is_ukoSabihS = false;
	}

	if( (!gMv.yMvIsMovingOdd) )
	{
		even_cnt++;

		if(even_cnt > S_UPPERBOUND_TH)
			even_cnt = S_UPPERBOUND_TH;
	}
	else
	{
		even_cnt = 0;
	}

	if( is_ukoSabihS )
	{
		if(gMv.h2XYsum > SpeedUp_TH )    /*for Speed up*/
		{
			confirm_cnt = confirm_cnt + 2;
		}
		else
		{
			confirm_cnt++;
		}

		if(confirm_cnt > S_UPPERBOUND_TH)
			confirm_cnt = S_UPPERBOUND_TH;
	}
	else
	{
		if(gMv.h2XYsum > SpeedUp_TH )    /*for Speed up*/
		{
			if( confirm_cnt > 0 )
				confirm_cnt--;
		}
		else
			confirm_cnt = 0;
	}

	if (isMcFilm)
	{
		confirm_cnt = 0;
	}

	if(( confirm_cnt >= S_MATCH_TH)&&( even_cnt >= S_MATCH_TH) )
	{
		*isUkosabihsMc = true;
		//MApi_XC_W2BYTEMSK( REG_SC_BK22_7A_L, 0x1000 , 0x1000 );
	}
	else
	{
		*isUkosabihsMc = false;
		//MApi_XC_W2BYTEMSK( REG_SC_BK22_7A_L, 0x0000 , 0x1000 );
	}

	if( confirm_cnt >= S_MATCH_TH)
	{
		*isUkosabihsMotion = true;
	}
	else
	{
		*isUkosabihsMotion = false;
	}

	if( confirm_cnt >= S_MATCH_TH)
	{
		*isUkosabihsForFilm = true;
	}
	else
	{
		*isUkosabihsForFilm = false;
	}
	if( *isUkosabihsForFilm )
	{
		confirm_cnt_film++;

		if(confirm_cnt_film > S_UPPERBOUND_TH)
			confirm_cnt_film = S_UPPERBOUND_TH;
	}
	else
	{
		confirm_cnt_film = 0;
	}
	if( confirm_cnt_film >= S_MATCH_TH)
	{
		MApi_XC_W2BYTEMSK( REG_SC_BK30_09_L, 0x0001 , 0x0001 );
		//MApi_XC_W2BYTEMSK( REG_SC_BK30_41_L, 0x7000 , 0x7000 );
	}
	else
	{
		MApi_XC_W2BYTEMSK( REG_SC_BK30_09_L, 0x0000 , 0x0001 );
		//MApi_XC_W2BYTEMSK( REG_SC_BK30_41_L, 0x0000 , 0x7000 );
	}

	if( isFilm )
	{
		//confirm_cnt = 0;
		*isUkosabihsMc = false;
		*isUkosabihsMotion = false;
		even_cnt = 0;
	}
	//==============================================================================================================

    if((gMv.h2XYsum > 0x3)&& (gMv.h2XYdiff <= 1) && (isMv0) && (mv0Comfirm>mv0ComfirmTH))
	{
		is_ukoSabihSmv0 = true;
	}
	else
	{
		is_ukoSabihSmv0 = false;
	}

	if( is_ukoSabihSmv0 )
	{
		confirm_cnt_mv0++;

		if(confirm_cnt_mv0 > S_UPPERBOUND_TH )
			confirm_cnt_mv0 = S_UPPERBOUND_TH;
	}
	else
	{
		confirm_cnt_mv0 = 0;
	}

	if( confirm_cnt_mv0 >= S_MATCH_TH )
	{
		*isUkosabihsMv0 = true;
	}
	else
	{
		*isUkosabihsMv0 = false;
	}
	//==============================================================================================================

	return ( *isUkosabihsMc || *isUkosabihsMotion );
}

MS_BOOL MDrv_SC_sweep_ukoSabihS(void *pInstance, const XC_GMvStates gMv,
				const MS_U16 nonMv0Comfirm, const MS_U16 mv0Comfirm,
				const XC_FilmStates filmstatus, const MS_BOOL isMcFilm,
				const MS_U32 uComplex,
				MS_BOOL *isUkosabihsMotionSweep )
{
    MS_BOOL is_ukoSabihS_sweep = FALSE;
	MS_BOOL isMv0 = (( gMv.minMvXdir == 32 ) && ( gMv.minMvYdir == 8 )) ? true : false;
	MS_BOOL isFilm = ( filmstatus.film22 || filmstatus.film22 || isMcFilm ) ? true : false;
    MS_BOOL isUkosabihsMcSweep = FALSE;

	static MS_U8 confirm_cnt = 0;
	static MS_U8 even_cnt = 0;

	if((gMv.h2XYsum > 0x9)&& (gMv.h2XYdiff <= 5) && (!isMv0) && (nonMv0Comfirm>0x300) && (uComplex>0x7300) && (mv0Comfirm <= 10) )
		is_ukoSabihS_sweep = true;
	else
		is_ukoSabihS_sweep = false;

	if( (!gMv.yMvIsMovingOdd) )
	{
		even_cnt++;

		if(even_cnt > 10)
			even_cnt = 10;
	}
	else
	{
		even_cnt = 0;
	}

	if( is_ukoSabihS_sweep )
	{
		confirm_cnt++;

		if(confirm_cnt > 10)
			confirm_cnt = 10;
	}
	else
	{
		confirm_cnt = 0;
	}

	if( isFilm )
	{
		confirm_cnt = 0;
		even_cnt = 0;
	}

	if(( confirm_cnt == 10)&&( even_cnt == 10) )
		isUkosabihsMcSweep = true;
	else
		isUkosabihsMcSweep = false;

	if( confirm_cnt == 10)
		*isUkosabihsMotionSweep = true;
	else
		*isUkosabihsMotionSweep = false;

	return isUkosabihsMcSweep;
}

XC_GMvStates MDrv_SC_Agate_GMV(void *pInstance, const XC_Me1HisStates H1,  const XC_Me1HisStates H2 )
{
	XC_GMvStates gMv;
//	MS_BOOL yIsMovingOdd;
//	MS_BOOL xIsMovingOdd;
//	MS_BOOL isMovingOdd;

	const MS_U8 const_GMV_X_LUT[9][8] =
	{
		{0,	1,	2,	3,	4,	5,	6,	7 },
		{8,	9,	10,	11,	12,	13,	14,	15},
		{16,	17,	18,	19,	20,	21,	22,	23},
		{24,	25,	26,	27,	28,	29,	30,	31},
		{33,	34,	35,	36,	37,	38,	39,	40},
		{41,	42,	43,	44,	45,	46,	47,	48},
		{49,	50,	51,	52,	53,	54,	55,	56},
		{57,	58,	59,	60,	61,	62,	63,	64},
		{32,	32,	32,	32,	32,	32,	32,	32}
	};

	const MS_U8 const_GMV_Y_LUT[9][2] =
	{
		{0,		1},
		{2,		3},
		{4,		5},
		{6,		7},
		{9,		10},
		{11,	12},
		{13,	14},
		{15,	16},
		{8,		8}
	};

	MS_U8 x_h1_idx = H1.minMvXdir;
	MS_U8 x_h2_idx = ( H1.minMvXdir == 8 ) ? 0 : H2.minMvXdir;
	MS_U8 y_h1_idx = H1.minMvYdir;
	MS_U8 y_h2_idx = ( H1.minMvYdir == 8 ) ? 0 : H2.minMvYdir;

	MS_U8 gm_x = const_GMV_X_LUT[x_h1_idx][x_h2_idx];
	MS_U8 gm_y = const_GMV_Y_LUT[y_h1_idx][y_h2_idx];

//	xIsMovingOdd = ( gm_x % 2 ) ? true : false;
//	yIsMovingOdd = ( gm_y % 2 ) ? true : false;
//	isMovingOdd = ( xIsMovingOdd | yIsMovingOdd );

	gMv.minMvXdir 	= gm_x;
	//gMv.minMvXbin	= H1.minMvXBin;
	gMv.h1XYdiff 	= abs(H1.minMvXBin-H1.minMvYBin);
	gMv.h1XYsum 	= H1.minMvXBin+H1.minMvYBin;
	gMv.minMvYdir 	= gm_y;
	//gMv.minMvYbin	= H1.minMvYBin;
	gMv.h2XYdiff 	= abs(H2.minMvXBin-H2.minMvYBin);
	gMv.h2XYsum 	= H2.minMvXBin+H2.minMvYBin;
	gMv.yMvIsMovingOdd = ( gm_y % 2 ) ? true : false;
#if 1
	//MApi_XC_W2BYTEMSK(REG_SC_BK30_08_L,  (gm_x<<8), 0xFF00);
	//MApi_XC_W2BYTEMSK(REG_SC_BK30_08_L,  gm_y, 0x00FF);

	//DEBUG_LC_Full6bit(gm_x);
	//DEBUG_LC_Full6bit2(gm_y);

	//MApi_XC_W2BYTEMSK(REG_SC_BK30_09_L,  (gMv.h2XYdiff), 0xFFFF);
	//MApi_XC_W2BYTEMSK(REG_SC_BK30_09_L,  (gMv.h1XYdiff<<8), 0xFF00);
	//MApi_XC_W2BYTEMSK(REG_SC_BK30_09_L,  gMv.h2XYdiff, 0x00FF);
	//MApi_XC_W2BYTEMSK(REG_SC_BK30_0A_L,  (gMv.h2XYsum), 0xFFFF);
	//MApi_XC_W2BYTEMSK(REG_SC_BK30_0A_L,  (((gMv.h1XYsum)>>2)<<8), 0xFF00);
	//MApi_XC_W2BYTEMSK(REG_SC_BK30_0A_L,  ((gMv.h2XYsum)>>2), 0x00FF);
#endif

	return gMv;
}

XC_Me1HisStates MDrv_SC_Agate_Hierarchy2(void *pInstance, const XC_Me1HisStates H1 )
{
	MS_U8 me1_X_histogram[8];
	MS_U8 me1_Y_histogram[2];
    MS_U8 minXdir = 0, minXdirBin = 0;
    MS_U8 minYdir = 0, minYdirBin = 0;
	XC_Me1HisStates H2States;
    int dir = 0;

	MApi_XC_W2BYTEMSK( REG_SC_BK17_5E_L,  H1.minMvXdir , 0x0007 ); /*reg_gmv_h_hier2_sel*/
	MApi_XC_W2BYTEMSK( REG_SC_BK17_5E_L,  H1.minMvYdir<<4 , 0x0070 ); /*reg_gmv_h_hier2_sel*/

	me1_X_histogram[0] = MApi_XC_R2BYTEMSK(REG_SC_BK17_64_L, 0x00FF);
	me1_X_histogram[1] = MApi_XC_R2BYTEMSK(REG_SC_BK17_64_L, 0xFF00) >> 8;
	me1_X_histogram[2] = MApi_XC_R2BYTEMSK(REG_SC_BK17_65_L, 0x00FF);
	me1_X_histogram[3] = MApi_XC_R2BYTEMSK(REG_SC_BK17_65_L, 0xFF00) >> 8;
	me1_X_histogram[4] = MApi_XC_R2BYTEMSK(REG_SC_BK17_66_L, 0x00FF);
	me1_X_histogram[5] = MApi_XC_R2BYTEMSK(REG_SC_BK17_66_L, 0xFF00) >> 8;
	me1_X_histogram[6] = MApi_XC_R2BYTEMSK(REG_SC_BK17_67_L, 0x00FF);
	me1_X_histogram[7] = MApi_XC_R2BYTEMSK(REG_SC_BK17_67_L, 0xFF00) >> 8;

	me1_Y_histogram[0] = MApi_XC_R2BYTEMSK(REG_SC_BK17_6C_L, 0x00FF);
	me1_Y_histogram[1] = MApi_XC_R2BYTEMSK(REG_SC_BK17_6C_L, 0xFF00) >> 8;

   /*FOR Vertical move 8 pixels*/
   if((H1.minMvYdir == 7)&&(abs (H1.minMvYBin -(me1_Y_histogram[0] +me1_Y_histogram[1] ))>3))//MV = 7 or 8
   	{
   		me1_Y_histogram[1] = (MApi_XC_R2BYTEMSK(REG_SC_BK17_6B_L, 0xFF00) >> 8) - MApi_XC_R2BYTEMSK(REG_SC_BK17_6C_L, 0x00FF); // MV+8 =  (H1+7)  - (MV+7)
   	}
   else if(H1.minMvYdir == 6 )//MV = 5or 6
   	{
   		me1_Y_histogram[0]= MApi_XC_R2BYTEMSK(REG_SC_BK17_6B_L, 0x00FF)  - (MApi_XC_R2BYTEMSK(REG_SC_BK17_6C_L, 0xFF00)>>8);  // MV+5= (H1+6) - (MV+6)
   	}

	//find max histogram
	minXdir = 0;
	minXdirBin = 0;
	for (dir=0; dir<=7 ; dir++)
	{
		if( minXdirBin < me1_X_histogram[dir] )
		{
			minXdir = dir;
			minXdirBin = me1_X_histogram[dir];
		}
	}

	minYdir = 0;
	minYdirBin = 0;
	for (dir=0; dir<=1 ; dir++)
	{
		if( minYdirBin < me1_Y_histogram[dir] )
		{
			minYdir = dir;
			minYdirBin = me1_Y_histogram[dir];
		}
	}

	if( H1.minMvXdir == 8 ) //MV0
	{
		H2States.minMvXdir = H1.minMvXdir;
		H2States.minMvXBin = H1.minMvXBin;
	}
	else
	{
		H2States.minMvXdir = minXdir;
		H2States.minMvXBin = minXdirBin;
	}

	if( H1.minMvYdir == 8 )
	{
		H2States.minMvYdir = H1.minMvYdir;
		H2States.minMvYBin = H1.minMvYBin;
	}
	else
	{
		H2States.minMvYdir = minYdir;
		H2States.minMvYBin = minYdirBin;
	}

	return H2States;
}

XC_Me1HisStates MDrv_SC_Agate_Hierarchy1( void *pInstance )
{
	MS_U8 me1_X_histogram[9];
	MS_U8 me1_Y_histogram[9];
    MS_U8 minXdir = 0, minXdirBin = 0;
    MS_U8 minYdir = 0, minYdirBin = 0;
	XC_Me1HisStates H1States;
    int dir = 0;

	me1_X_histogram[0] = MApi_XC_R2BYTEMSK(REG_SC_BK17_60_L, 0x00FF);
	me1_X_histogram[1] = MApi_XC_R2BYTEMSK(REG_SC_BK17_60_L, 0xFF00) >> 8;
	me1_X_histogram[2] = MApi_XC_R2BYTEMSK(REG_SC_BK17_61_L, 0x00FF);
	me1_X_histogram[3] = MApi_XC_R2BYTEMSK(REG_SC_BK17_61_L, 0xFF00) >> 8;
	me1_X_histogram[4] = MApi_XC_R2BYTEMSK(REG_SC_BK17_62_L, 0x00FF);
	me1_X_histogram[5] = MApi_XC_R2BYTEMSK(REG_SC_BK17_62_L, 0xFF00) >> 8;
	me1_X_histogram[6] = MApi_XC_R2BYTEMSK(REG_SC_BK17_63_L, 0x00FF);
	me1_X_histogram[7] = MApi_XC_R2BYTEMSK(REG_SC_BK17_63_L, 0xFF00) >> 8;
	me1_X_histogram[8] = MApi_XC_R2BYTEMSK(REG_SC_BK17_5F_L, 0x00FF);

	me1_Y_histogram[0] = MApi_XC_R2BYTEMSK(REG_SC_BK17_68_L, 0x00FF);
	me1_Y_histogram[1] = MApi_XC_R2BYTEMSK(REG_SC_BK17_68_L, 0xFF00) >> 8;
	me1_Y_histogram[2] = MApi_XC_R2BYTEMSK(REG_SC_BK17_69_L, 0x00FF);
	me1_Y_histogram[3] = MApi_XC_R2BYTEMSK(REG_SC_BK17_69_L, 0xFF00) >> 8;
	me1_Y_histogram[4] = MApi_XC_R2BYTEMSK(REG_SC_BK17_6A_L, 0x00FF);
	me1_Y_histogram[5] = MApi_XC_R2BYTEMSK(REG_SC_BK17_6A_L, 0xFF00) >> 8;
	me1_Y_histogram[6] = MApi_XC_R2BYTEMSK(REG_SC_BK17_6B_L, 0x00FF);
	me1_Y_histogram[7] = MApi_XC_R2BYTEMSK(REG_SC_BK17_6B_L, 0xFF00) >> 8;
	me1_Y_histogram[8] = MApi_XC_R2BYTEMSK(REG_SC_BK17_5F_L, 0xFF00) >> 8;

	minXdir = 0;
	minXdirBin = 0;
	for ( dir = 0; dir <= 8 ; dir ++ )
	{
		if( minXdirBin < me1_X_histogram[dir] )
		{
			minXdir = dir;
			minXdirBin = me1_X_histogram[dir];
		}
	}

	minYdir = 0;
	minYdirBin = 0;
	for ( dir = 0; dir <= 8 ; dir ++ )
	{
		if( minYdirBin < me1_Y_histogram[dir] )
		{
			minYdir = dir;
			minYdirBin = me1_Y_histogram[dir];
		}
	}
	H1States.minMvXdir = minXdir;
	H1States.minMvXBin = minXdirBin;
	H1States.minMvYdir = minYdir;
	H1States.minMvYBin = minYdirBin;

	return H1States;
}

/******************************************************************************/
/// Einstein Function
///
/******************************************************************************/
void MDrv_RegC_pdnr_me1_smv_nr_strength(void *pInstance, const MS_U8 smvw, const MS_U8 isSourceChange, const MS_BOOL sceneChange )
{
	static MS_U8 qMapDefault = 3;
	MS_U8 setValue;
	static MS_BOOL allowW = false;
	MS_U8 smallMovingW;

	smallMovingW = 15 - (smvw >> 4);

	if( (isSourceChange == 1) && (allowW == false) )
	{
		qMapDefault = (MS_U8)( MApi_XC_R2BYTEMSK(REG_SC_BK2A_66_L, 0x0F00) >> 8 );
		allowW = true;
	}

	if( sceneChange )
		setValue = 0x0; /* motion gain is Max when scene change happen */
	else if( smallMovingW > qMapDefault )
		setValue = qMapDefault;
	else
		setValue = smallMovingW;

	if( allowW )
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_66_L, setValue << 8 , 0x0F00);
}

void MDrv_Adaptive_Coring(void *pInstance, const MS_U8 noiseLevel, const MS_U8 isSourceChange, const MS_BOOL sceneChange )
{
	static MS_U8 qMapPostCoringTh1;
	static MS_U8 qMapPostCoringTh2;
	static MS_U8 noiseL1Cnt = 0, noiseL2Cnt = 0;

	if( isSourceChange )
	{
		qMapPostCoringTh1 = (MS_U8)MApi_XC_R2BYTEMSK(REG_SC_BK19_13_L, 0x000F );
		qMapPostCoringTh2 = (MS_U8)( MApi_XC_R2BYTEMSK(REG_SC_BK19_13_L, 0x00F0 ) >> 4 );

		noiseL1Cnt = noiseL2Cnt = 0;
	}
	else if( sceneChange )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK19_13_L, qMapPostCoringTh1 , 0x000F );
		MApi_XC_W2BYTEMSK(REG_SC_BK19_13_L, qMapPostCoringTh2 << 4 , 0x00F0 );

		noiseL1Cnt = noiseL2Cnt = 0;
	}
	else
	{
		if( noiseLevel > 0x40 )
		{
			if( noiseL1Cnt < 16 )
				++noiseL1Cnt;
		}
		else if( noiseL1Cnt > 0 )
			--noiseL1Cnt;

		if( noiseLevel > 0x80 )
		{
			if( noiseL2Cnt < 16 )
				++noiseL2Cnt;
		}
		else if( noiseL2Cnt > 0 )
			--noiseL2Cnt;

		MS_U8 postCoringTh1 = ( 0x9 * noiseL2Cnt + 0xE * (16 - noiseL2Cnt) + 8 ) >> 4;
		MS_U8 postCoringTh2 = ( 0xB * noiseL2Cnt + 0xF * (16 - noiseL2Cnt) + 8 ) >> 4;

		postCoringTh1 = ( postCoringTh1 * noiseL1Cnt + qMapPostCoringTh1 * (16 - noiseL1Cnt) + 8 ) >> 4;
		postCoringTh2 = ( postCoringTh2 * noiseL1Cnt + qMapPostCoringTh2 * (16 - noiseL1Cnt) + 8 ) >> 4;

		MApi_XC_W2BYTEMSK(REG_SC_BK19_13_L, postCoringTh1 , 0x000F );
		MApi_XC_W2BYTEMSK(REG_SC_BK19_13_L, postCoringTh2 << 4 , 0x00F0 );
	}
}

/******************************************************************************/
/// Agate Function
///
/******************************************************************************/


void MDrv_SC_Edison_D3D_driver( void *pInstance )
{
	MS_U16 LT_Lev[32];
	MS_U16 RB_Lev[32];
    MS_U8 idx = 0;

	static MS_U32 DIF_LR_sum = 0xFFFF;
	static MS_U32 DIF_TB_sum = 0xFFFF;
//	static MS_U8 D3D_mode=0; //0: video, 1: LR mode, 2:TB mode
	static MS_U8 D3D_ready=0;
	static MS_U8 LRTB_Cnt=0;
	static MS_U32 DIF_LR_sum_prv=0xFFFF;
	static MS_U32 DIF_TB_sum_prv=0xFFFF;
	static MS_BOOL begin_d3d = false;
//	static MS_U16 CB_status = 0;

	//first time
	if( (LRTB_Cnt == 0) && (begin_d3d == false))
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK1D_50_L, 0x0001, 0x0001);
		begin_d3d = true;
	}

	if( D3D_ready )
	{
		LT_Lev[0]  = MApi_XC_R2BYTEMSK(REG_SC_BK1D_10_L, 0xFFFF);
		LT_Lev[1]  = MApi_XC_R2BYTEMSK(REG_SC_BK1D_11_L, 0xFFFF);
		LT_Lev[2]  = MApi_XC_R2BYTEMSK(REG_SC_BK1D_12_L, 0xFFFF);
		LT_Lev[3]  = MApi_XC_R2BYTEMSK(REG_SC_BK1D_13_L, 0xFFFF);
		LT_Lev[4]  = MApi_XC_R2BYTEMSK(REG_SC_BK1D_14_L, 0xFFFF);
		LT_Lev[5]  = MApi_XC_R2BYTEMSK(REG_SC_BK1D_15_L, 0xFFFF);
		LT_Lev[6]  = MApi_XC_R2BYTEMSK(REG_SC_BK1D_16_L, 0xFFFF);
		LT_Lev[7]  = MApi_XC_R2BYTEMSK(REG_SC_BK1D_17_L, 0xFFFF);
		LT_Lev[8]  = MApi_XC_R2BYTEMSK(REG_SC_BK1D_18_L, 0xFFFF);
		LT_Lev[9]  = MApi_XC_R2BYTEMSK(REG_SC_BK1D_19_L, 0xFFFF);
		LT_Lev[10] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_1A_L, 0xFFFF);
		LT_Lev[11] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_1B_L, 0xFFFF);
		LT_Lev[12] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_1C_L, 0xFFFF);
		LT_Lev[13] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_1D_L, 0xFFFF);
		LT_Lev[14] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_1E_L, 0xFFFF);
		LT_Lev[15] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_1F_L, 0xFFFF);
		LT_Lev[16] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_20_L, 0xFFFF);
		LT_Lev[17] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_21_L, 0xFFFF);
		LT_Lev[18] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_22_L, 0xFFFF);
		LT_Lev[19] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_23_L, 0xFFFF);
		LT_Lev[20] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_24_L, 0xFFFF);
		LT_Lev[21] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_25_L, 0xFFFF);
		LT_Lev[22] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_26_L, 0xFFFF);
		LT_Lev[23] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_27_L, 0xFFFF);
		LT_Lev[24] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_28_L, 0xFFFF);
		LT_Lev[25] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_29_L, 0xFFFF);
		LT_Lev[26] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_2A_L, 0xFFFF);
		LT_Lev[27] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_2B_L, 0xFFFF);
		LT_Lev[28] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_2C_L, 0xFFFF);
		LT_Lev[29] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_2D_L, 0xFFFF);
		LT_Lev[30] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_2E_L, 0xFFFF);
		LT_Lev[31] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_2F_L, 0xFFFF);

		RB_Lev[0]  = MApi_XC_R2BYTEMSK(REG_SC_BK1D_30_L, 0xFFFF);
		RB_Lev[1]  = MApi_XC_R2BYTEMSK(REG_SC_BK1D_31_L, 0xFFFF);
		RB_Lev[2]  = MApi_XC_R2BYTEMSK(REG_SC_BK1D_32_L, 0xFFFF);
		RB_Lev[3]  = MApi_XC_R2BYTEMSK(REG_SC_BK1D_33_L, 0xFFFF);
		RB_Lev[4]  = MApi_XC_R2BYTEMSK(REG_SC_BK1D_34_L, 0xFFFF);
		RB_Lev[5]  = MApi_XC_R2BYTEMSK(REG_SC_BK1D_35_L, 0xFFFF);
		RB_Lev[6]  = MApi_XC_R2BYTEMSK(REG_SC_BK1D_36_L, 0xFFFF);
		RB_Lev[7]  = MApi_XC_R2BYTEMSK(REG_SC_BK1D_37_L, 0xFFFF);
		RB_Lev[8]  = MApi_XC_R2BYTEMSK(REG_SC_BK1D_38_L, 0xFFFF);
		RB_Lev[9]  = MApi_XC_R2BYTEMSK(REG_SC_BK1D_39_L, 0xFFFF);
		RB_Lev[10] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_3A_L, 0xFFFF);
		RB_Lev[11] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_3B_L, 0xFFFF);
		RB_Lev[12] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_3C_L, 0xFFFF);
		RB_Lev[13] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_3D_L, 0xFFFF);
		RB_Lev[14] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_3E_L, 0xFFFF);
		RB_Lev[15] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_3F_L, 0xFFFF);
		RB_Lev[16] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_40_L, 0xFFFF);
		RB_Lev[17] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_41_L, 0xFFFF);
		RB_Lev[18] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_42_L, 0xFFFF);
		RB_Lev[19] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_43_L, 0xFFFF);
		RB_Lev[20] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_44_L, 0xFFFF);
		RB_Lev[21] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_45_L, 0xFFFF);
		RB_Lev[22] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_46_L, 0xFFFF);
		RB_Lev[23] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_47_L, 0xFFFF);
		RB_Lev[24] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_48_L, 0xFFFF);
		RB_Lev[25] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_49_L, 0xFFFF);
		RB_Lev[26] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_4A_L, 0xFFFF);
		RB_Lev[27] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_4B_L, 0xFFFF);
		RB_Lev[28] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_4C_L, 0xFFFF);
		RB_Lev[29] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_4D_L, 0xFFFF);
		RB_Lev[30] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_4E_L, 0xFFFF);
		RB_Lev[31] = MApi_XC_R2BYTEMSK(REG_SC_BK1D_4F_L, 0xFFFF);


		if( 0 == (LRTB_Cnt % 2) )
		{
			DIF_LR_sum = 0;
			for( idx = 0; idx < 32; idx++ )
			{
				DIF_LR_sum = DIF_LR_sum + abs( LT_Lev[idx] - RB_Lev[idx] );
			}

			//IIR filter
			DIF_LR_sum = (DIF_LR_sum_prv*3 + DIF_LR_sum)/4;
			DIF_LR_sum_prv = DIF_LR_sum;
		}
		else
		{
			DIF_TB_sum = 0;
			for( idx = 0; idx < 32; idx++ )
			{
				DIF_TB_sum = DIF_TB_sum + abs( LT_Lev[idx] - RB_Lev[idx] );
			}

			//IIR filter
			DIF_TB_sum = (DIF_TB_sum_prv*3 + DIF_TB_sum)/4;
			DIF_TB_sum_prv = DIF_TB_sum;
		}

		//checkboard status
//		CB_status = MApi_XC_R2BYTEMSK(REG_SC_BK1D_53_L, 0xFFFF);
#if 0
		if((DIF_TB_sum > (4*DIF_LR_sum)) && (DIF_LR_sum < 1000))
			D3D_mode = 1; //LR mode
		else if((DIF_LR_sum > (4*DIF_TB_sum)) && (DIF_TB_sum < 1000))
			D3D_mode = 2; //TB mode
		else if( CB_status > 1000 )
			D3D_mode = 3; //Checkboard mode
		else if( (DIF_LR_sum < 400) && (DIF_TB_sum < 400) )
			D3D_mode = 4; //repeat previous frame
		else
			D3D_mode = 0; //video
#endif
		//for next D3D frame trigger
		if( 0 == (LRTB_Cnt % 2) )
			MApi_XC_W2BYTEMSK(REG_SC_BK1D_50_L, 0x02, 0x02); //for next frame: top-bottom
		else
			MApi_XC_W2BYTEMSK(REG_SC_BK1D_50_L, 0x00, 0x02); //for next frame: side-by-side

		MApi_XC_W2BYTEMSK(REG_SC_BK1D_50_L, 0x0001, 0x0001);
	}

	//for next start
	if( LRTB_Cnt >= 1 )
	{
		LRTB_Cnt=0;
	}
	else
	{
		LRTB_Cnt++;
	}

	//wait ready signal
	D3D_ready = MApi_XC_R2BYTEMSK(REG_SC_BK1D_50_L, 0x0080)>>7;
}

void MDrv_SC_ResetSourceChange(void *pInstance,const MS_BOOL sourceChangeForME ) //review MERESET
{

	static MS_U8 u8CurFrameCnt1 = 0;
	static MS_U8 u8CurFrameCnt2 = 0;
	static MS_BOOL meReset = false;
	static MS_BOOL isReset = false;

	if(sourceChangeForME == 1 )
	{
		if(u8CurFrameCnt1 != 60 )
			u8CurFrameCnt1++;
	}
	else
		u8CurFrameCnt1 = 0;

	if( meReset )
	{
		if(u8CurFrameCnt2 != 60 )
			u8CurFrameCnt2++;
	}
	else
		u8CurFrameCnt2 = 0;

	if( (u8CurFrameCnt1 == 30) && (!meReset) )
	{
		meReset = true;
	}

	if( (u8CurFrameCnt2 == 5) && meReset )
	{
		meReset = false;
		isReset = true;
	}
	else
	{
		isReset = false;
	}

	if(isReset)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_07_L,  0x0000, 0x0002); /* Reset Init Bit */
        u8CurFrameCnt1 = 0;
	}
}
MS_BOOL MDrv_SC_Agate_ME_Reset(void *pInstance, const MS_U32 u32MotionValue2, const MS_BOOL isSceneChangeMc, const MS_BOOL isSceneChangeY, const MS_BOOL isSourceChange  )//review MERESET
{
	static MS_U8 u8CurFrameCnt1 = 0;
	static MS_U8 u8CurFrameCnt2 = 0;
	static MS_BOOL meReset = false;
	MS_BOOL meResetFlag = false;

	if (u32MotionValue2 == 0 )
	{
		if(u8CurFrameCnt1 != 60 )
			u8CurFrameCnt1++;
	}
	else
	{
		u8CurFrameCnt1 = 0;
	}

	if ( meReset )
	{
		if(u8CurFrameCnt2 != 60 )
			u8CurFrameCnt2++;
	}
	else
	{
		u8CurFrameCnt2 = 0;
	}

	if( (u8CurFrameCnt1 == 30) && (!meReset) )
	{
		meReset = true;
	}

	if( (u8CurFrameCnt2 == 5) && meReset )
	{
		meReset = false;
		meResetFlag = true;
	}
	else
	{
		meResetFlag = false;
	}

	if( meReset || isSceneChangeMc || isSceneChangeY || isSourceChange)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK17_79_L,  0x0011, 0x0011);
	}
	else
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK17_79_L,  0x0000, 0x0011);
	}
	return meResetFlag;
}

/******************************************************************************/
///Read ME value (F2 ME status)
///@return MS_U16: Motion value
/******************************************************************************/
MS_U16 MDrv_SC_read_avgMcnrMovingWeightMin(void *pInstance)
{
	MS_U16 cur;
	static MS_U16 prv = 0;

	cur = MApi_XC_R2BYTE(REG_SC_BK09_68_L);
	cur = ((prv*3 )+ cur ) >> 2;
	prv = cur;

	return prv;
}

MS_U16 MDrv_SC_read_avgMcnrMovingWeight(void *pInstance)
{
	MS_U16 cur;
	static MS_U16 prv = 0;

	cur = MApi_XC_R2BYTE(REG_SC_BK09_6A_L);
	cur = ((prv*3 )+ cur ) >> 2;
	prv = cur;

	return prv;
}

MS_U16 MDrv_SC_read_avgMcnrMovingWeightMax(void *pInstance)
{
	MS_U16 cur;
	static MS_U16 prv = 0;

	cur = MApi_XC_R2BYTE(REG_SC_BK09_6B_L);
	cur = ((prv*3 )+ cur ) >> 2;
	prv = cur;

	return prv;
}

MS_U16 MDrv_SC_read_avgMv0MovingWeight(void *pInstance)
{
	MS_U16 cur;
	static MS_U16 prv = 0;

	cur = MApi_XC_R2BYTE(REG_SC_BK09_69_L);
	cur = ((prv*3 )+ cur ) >> 2;
	prv = cur;

	return prv;
}

MS_U16 MDrv_SC_read_me2SadDiff(void *pInstance)
{
	MS_U16 cur;
	static MS_U16 prv = 0;

	cur = MApi_XC_R2BYTE(REG_SC_BK09_6C_L);
	cur = ((prv*3 )+ cur ) >> 2;
	prv = cur;

	return prv;
}

/******************************************************************************/
///Read motion value (F2 motion status)
///@return MS_U8: Motion value
/******************************************************************************/
MS_U32 MDrv_SC_read_motion_value1(void *pInstance)
{
    MS_U32 u32MotionValue = 0;
    MS_U32 u32RegMadi_1C = 0, u32RegMadi_1B = 0, u32RegMadi_1A = 0;

	u32RegMadi_1C = (MS_U32)MApi_XC_R2BYTEMSK(REG_SC_BK22_0E_L, 0x3F);
	u32RegMadi_1B = (MS_U32)MApi_XC_R2BYTEMSK(REG_SC_BK22_0D_L, 0xFF00);
	u32RegMadi_1A = (MS_U32)MApi_XC_R2BYTEMSK(REG_SC_BK22_0D_L, 0x00FF);

	u32RegMadi_1C = u32RegMadi_1C * 0x10000UL;

	u32MotionValue = (u32RegMadi_1C + u32RegMadi_1B + u32RegMadi_1A)  ;

	DRVSCA_DBG(PRT_TRP("MotionValue = 0x%x\n", u32MotionValue));

	return u32MotionValue;
}

/******************************************************************************/
///Read motion value (Motion count status)
///@return MS_U8: Motion value
/******************************************************************************/
MS_U32 MDrv_SC_read_motion_value2(void *pInstance)
{
    MS_U32 u32MotionValue = 0;

    u32MotionValue = (MS_U32)MApi_XC_R2BYTEMSK(REG_SC_BK0A_1A_L, 0x7F);

	return u32MotionValue;
}

/******************************************************************************/
///Read motion value (Motion count status)
///@return MS_U32: Motion value
/******************************************************************************/
MS_U32 MDrv_SC_read_motion_value3(void *pInstance)
{
    MS_U32 u32MotionValue = 0;

	u32MotionValue = (MS_U32)MApi_XC_R2BYTEMSK(REG_SC_BK0A_18_L, 0x0FFF);

	return u32MotionValue;
}

MS_U16 MDrv_SC_read_motion_value4(void *pInstance)
{
    MS_U16 motionValue = 0;

	motionValue = MApi_XC_R2BYTEMSK(REG_SC_BK0A_2E_L, 0x7FFF);

	return motionValue;
}

//-------------------------------------------------------------------------------------------------
/// Read motion value (Motion count status)
/// @return MS_U8: Motion value
//-------------------------------------------------------------------------------------------------
MS_U8 Hal_PQ_GetCurrentMotionValue(void *pInstance)
{
    MS_U8 u8MotionValue = 0;

	u8MotionValue  = (MS_U8)((MApi_XC_R2BYTEMSK(REG_SC_BK22_0E_L, 0x3F) << 2)
		|(MApi_XC_R2BYTEMSK(REG_SC_BK22_0D_L, 0xC000) >> 6));

	return u8MotionValue;
}

/******************************************************************************/
///Read VCnt value
///@return MS_U8: VCnt value
/******************************************************************************/
MS_U8 MDrv_SC_read_v_cnt(void *pInstance)
{
    MS_U8 u8VCnt = 0;
	u8VCnt = (MS_U8)(MApi_XC_R2BYTEMSK(REG_SC_BK22_0E_L, 0xC0) >> 6);
	return u8VCnt;
}

/******************************************************************************/
///Read SDHDD Detect Threshold value
///@return MS_U16: HDSDD_Det_Threshold value
/******************************************************************************/
MS_U16 MDrv_SC_read_HDSDD_Det_Threshold_value(void *pInstance)
{
    MS_U16 u16Value = 0;

	u16Value = (MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK22_7F_L, 0xFFFF);

	return u16Value;
}

/******************************************************************************/
///Read DLC 32 section Histogram Data
///@return MS_U16: 32 section Histogram Data
/******************************************************************************/

XC_LumaHistogramStates MDrv_SC_GetLumaHistogram(void *pInstance)
{
	XC_LumaHistogramStates lumaStatus;
	lumaStatus.lumaBin[0] =  MApi_XC_R2BYTE( REG_SC_BK1A_40_L );
	lumaStatus.lumaBin[1] =  MApi_XC_R2BYTE( REG_SC_BK1A_41_L );
	lumaStatus.lumaBin[2] =  MApi_XC_R2BYTE( REG_SC_BK1A_42_L );
	lumaStatus.lumaBin[3] =  MApi_XC_R2BYTE( REG_SC_BK1A_43_L );
	lumaStatus.lumaBin[4] =  MApi_XC_R2BYTE( REG_SC_BK1A_44_L );
	lumaStatus.lumaBin[5] =  MApi_XC_R2BYTE( REG_SC_BK1A_45_L );
	lumaStatus.lumaBin[6] =  MApi_XC_R2BYTE( REG_SC_BK1A_46_L );
	lumaStatus.lumaBin[7] =  MApi_XC_R2BYTE( REG_SC_BK1A_47_L );
	lumaStatus.lumaBin[8] =  MApi_XC_R2BYTE( REG_SC_BK1A_48_L );
	lumaStatus.lumaBin[9] =  MApi_XC_R2BYTE( REG_SC_BK1A_49_L );
	lumaStatus.lumaBin[10] =  MApi_XC_R2BYTE( REG_SC_BK1A_4A_L );
	lumaStatus.lumaBin[11] =  MApi_XC_R2BYTE( REG_SC_BK1A_4B_L );
	lumaStatus.lumaBin[12] =  MApi_XC_R2BYTE( REG_SC_BK1A_4C_L );
	lumaStatus.lumaBin[13] =  MApi_XC_R2BYTE( REG_SC_BK1A_4D_L );
	lumaStatus.lumaBin[14] =  MApi_XC_R2BYTE( REG_SC_BK1A_4E_L );
	lumaStatus.lumaBin[15] =  MApi_XC_R2BYTE( REG_SC_BK1A_4F_L );
	lumaStatus.lumaBin[16] =  MApi_XC_R2BYTE( REG_SC_BK1A_50_L );
	lumaStatus.lumaBin[17] =  MApi_XC_R2BYTE( REG_SC_BK1A_51_L );
	lumaStatus.lumaBin[18] =  MApi_XC_R2BYTE( REG_SC_BK1A_52_L );
	lumaStatus.lumaBin[19] =  MApi_XC_R2BYTE( REG_SC_BK1A_53_L );
	lumaStatus.lumaBin[20] =  MApi_XC_R2BYTE( REG_SC_BK1A_54_L );
	lumaStatus.lumaBin[21] =  MApi_XC_R2BYTE( REG_SC_BK1A_55_L );
	lumaStatus.lumaBin[22] =  MApi_XC_R2BYTE( REG_SC_BK1A_56_L );
	lumaStatus.lumaBin[23] =  MApi_XC_R2BYTE( REG_SC_BK1A_57_L );
	lumaStatus.lumaBin[24] =  MApi_XC_R2BYTE( REG_SC_BK1A_58_L );
	lumaStatus.lumaBin[25] =  MApi_XC_R2BYTE( REG_SC_BK1A_59_L );
	lumaStatus.lumaBin[26] =  MApi_XC_R2BYTE( REG_SC_BK1A_5A_L );
	lumaStatus.lumaBin[27] =  MApi_XC_R2BYTE( REG_SC_BK1A_5B_L );
	lumaStatus.lumaBin[28] =  MApi_XC_R2BYTE( REG_SC_BK1A_5C_L );
	lumaStatus.lumaBin[29] =  MApi_XC_R2BYTE( REG_SC_BK1A_5D_L );
	lumaStatus.lumaBin[30] =  MApi_XC_R2BYTE( REG_SC_BK1A_5E_L );
	lumaStatus.lumaBin[31] =  MApi_XC_R2BYTE( REG_SC_BK1A_5F_L );

	return lumaStatus;
}

#define Confirm_Gain         5
MS_U16 MDrv_SC_Agate_ME1_UC_InSearchRange_MR_Offset(void *pInstance,MS_U16 *me1IsMv0, MS_U16 *me1IsNonMv0, MS_U16 *me1IsNotFound )
{
    MS_U16 MV0_Comfirm = 0,  nonMV0_Comfirm = 0, MV_Contradict = 0;
	static MS_U16 Prev_MV0_Comfirm = 0;
	static MS_U16 Prev_nonMV0_Comfirm = 0;
	static MS_U16 Prev_MV_Contradict = 0;
    MS_U16 MV_Comfirm = 0;
    MS_U16 MR_Offset = 0;

	MV0_Comfirm = MApi_XC_R2BYTE(REG_SC_BK17_48_L);
	nonMV0_Comfirm = MApi_XC_R2BYTE(REG_SC_BK17_49_L);
	MV_Contradict = MApi_XC_R2BYTE(REG_SC_BK17_4A_L);

	MV0_Comfirm = (Prev_MV0_Comfirm * 3 + MV0_Comfirm) >> 2;
	Prev_MV0_Comfirm = MV0_Comfirm;

	nonMV0_Comfirm = (Prev_nonMV0_Comfirm * 3 + nonMV0_Comfirm) >> 2;
	Prev_nonMV0_Comfirm = nonMV0_Comfirm;

	MV_Contradict = (Prev_MV_Contradict * 3 + MV_Contradict) >> 2;
	Prev_MV_Contradict = MV_Contradict;

	MV_Comfirm = MV0_Comfirm + nonMV0_Comfirm;

	if(MV_Comfirm > (MV_Contradict * Confirm_Gain))
		MR_Offset = nonMV0_Comfirm;
	else
		MR_Offset = 0;

	*me1IsMv0 = MV0_Comfirm;
	*me1IsNonMv0 = nonMV0_Comfirm;
	*me1IsNotFound = MV_Contradict;

	return MR_Offset;
}

MS_U16 MDrv_SC_Agate_ME2_UC_InSearchRange_MR_Offset(void *pInstance,MS_U16 *me2IsMv0, MS_U16 *me2IsNonMv0, MS_U16 *me2IsNotFound)
{
    MS_U16 MV0_Comfirm = 0,  nonMV0_Comfirm = 0, MV_Contradict = 0;
	static MS_U16 Prev_MV0_Comfirm = 0;
	static MS_U16 Prev_nonMV0_Comfirm = 0;
	static MS_U16 Prev_MV_Contradict = 0;
    MS_U16 MV_Comfirm = 0;
    MS_U16 MR_Offset = 0;

	MV0_Comfirm = MApi_XC_R2BYTE(REG_SC_BK17_4B_L);
	nonMV0_Comfirm = MApi_XC_R2BYTE(REG_SC_BK17_4C_L);
	MV_Contradict = MApi_XC_R2BYTE(REG_SC_BK17_4D_L);

	MV0_Comfirm = (Prev_MV0_Comfirm * 3 + MV0_Comfirm) >> 2;
	Prev_MV0_Comfirm = MV0_Comfirm;

	nonMV0_Comfirm = (Prev_nonMV0_Comfirm * 3 + nonMV0_Comfirm) >> 2;
	Prev_nonMV0_Comfirm = nonMV0_Comfirm;

	MV_Contradict = (Prev_MV_Contradict * 3 + MV_Contradict) >> 2;
	Prev_MV_Contradict = MV_Contradict;

	MV_Comfirm = MV0_Comfirm + nonMV0_Comfirm;

	if(MV_Comfirm > (MV_Contradict * Confirm_Gain))
		MR_Offset = nonMV0_Comfirm;
	else
		MR_Offset = 0;

	*me2IsMv0 = MV0_Comfirm;
	*me2IsNonMv0 = nonMV0_Comfirm;
	*me2IsNotFound = MV_Contradict;

	return MR_Offset;
}

#define ScenceChange_TH        0x1000
MS_BOOL MDrv_SC_Agate_SceneChangeY(void *pInstance, const  XC_LumaHistogramStates lumaHistogram )
{
	//XC_DLC_ApiStatus DLC_Status;
	//MApi_XC_DLC_GetStatus_Ex(&DLC_Status,MAIN_WINDOW);
	static MS_U16 Pre_LumaHistogram[32], Max_HistogramDiff = 0;
	MS_U16 HistogramDiff = 0;
    int i = 0;

	for(i=0;i<32;i++)
	{
		if(lumaHistogram.lumaBin[i] >= Pre_LumaHistogram[i])
		{
			HistogramDiff = lumaHistogram.lumaBin[i] - Pre_LumaHistogram[i];
		}
		else
		{
			HistogramDiff = Pre_LumaHistogram[i] - lumaHistogram.lumaBin[i];
		}

		if(HistogramDiff > Max_HistogramDiff)
		{
			Max_HistogramDiff = HistogramDiff;
		}

		Pre_LumaHistogram[i] = lumaHistogram.lumaBin[i];
	}

	if( Max_HistogramDiff > ScenceChange_TH )
	{
		Max_HistogramDiff = 0;
		return true;
	}
	else
	{
		Max_HistogramDiff = 0;
		return false;
	}
}

MS_BOOL MDrv_SC_Agate_SceneChangeMc(void *pInstance,const MS_U16 me1IsMv0, const MS_U16 me1IsNonMv0, const MS_U16 me1IsNotFound,
				    const MS_U16 me2IsMv0, const MS_U16 me2IsNonMv0, const MS_U16 me2IsNotFound)
{
    MS_U16 me1Comfirm = 0;
    MS_U16 me2Comfirm = 0;
	static MS_BOOL isNotFound           = false;
	static MS_BOOL isScencChange        = false;

	static MS_U8 notFoundDelayCnt = 0;
	static MS_U8 scencChangeDelayCnt = 0;

	me1Comfirm = me1IsMv0 + me1IsNonMv0;
	me2Comfirm = me2IsMv0 + me2IsNonMv0;

	if((me1IsNotFound > me1Comfirm*5)&&(me2IsNotFound > me2Comfirm*5) &&
		(me1IsNotFound > 0x10) && (me2IsNotFound > 0x10) )/* not found*/
	{
		isNotFound = true;
	}

	if( isNotFound )
	{
		if( notFoundDelayCnt != 60 )
			notFoundDelayCnt++;
	}

	if( notFoundDelayCnt > 10 )
	{
		if((me1IsNotFound*5 < me1Comfirm)&&(me2IsNotFound*5 < me2Comfirm) &&
			(me1Comfirm > 0x10) && (me2Comfirm > 0x10) )/* found */
		{
			isScencChange = true;
		}
		else
		{
			isScencChange = false;
		}

		notFoundDelayCnt = 0;
		isNotFound = false;
	}

	if( isScencChange )
	{
		if( scencChangeDelayCnt != 60 )
			scencChangeDelayCnt++;
	}

	if( scencChangeDelayCnt > 5 )
	{
		scencChangeDelayCnt = 0;
		isScencChange = false;
	}

	return isScencChange;
}

#define HIS_MOTION_TH 0x0A
MS_BOOL MDrv_SC_Bouncing_Status(const MS_U32 u32MotionValue)
{
	if( u32MotionValue < HIS_MOTION_TH )
	{
		return TRUE;                            //Still pattern
	}
	else
	{
		return FALSE;                           //Moving pattern
	}
}

#define HIS_MOTION_TH2 0x30
MS_BOOL MDrv_SC_Bouncing_Status2(const MS_U32 u32MotionValue)
{
	if( u32MotionValue < HIS_MOTION_TH2 )
	{
		return TRUE;                            //Still pattern
	}
	else
	{
		return FALSE;                           //Moving pattern
	}
}


#define Feather_Sum_TH 0x1500
#define Feather_Sum_TH2 0x230
MS_BOOL MDrv_SC_TotalFeather_Sum_Status(void *pInstance,const MS_U16 featherAndMotion, const MS_BOOL isSD_source, MS_BOOL *Feather_sum_status2 )
{
    MS_U32 SD2HD_Feather = 0;
	static MS_U32 SD2HD_Feather_iir = 0;
	MS_BOOL isFeatherAndMotion = ( featherAndMotion > 0x10 ) ? true : false;

    MS_BOOL isFeather1 = FALSE, isFeather2 = FALSE;

	static MS_U8 confirm_cnt1 = 0;
	static MS_U8 confirm_cnt2 = 0;

	SD2HD_Feather    = MApi_XC_R2BYTE(REG_SC_BK23_56_L);
	SD2HD_Feather = ((SD2HD_Feather_iir*3 )+ SD2HD_Feather ) >> 2;
	SD2HD_Feather_iir = SD2HD_Feather;

	if( (SD2HD_Feather_iir > Feather_Sum_TH2) &&(isSD_source) && (!isFeatherAndMotion) )
		//*Feather_sum_status2 = true;
		isFeather2 = true;
	else
		//*Feather_sum_status2 = false;
		isFeather2 = false;

	if( (SD2HD_Feather_iir > Feather_Sum_TH) &&(isSD_source) && (!isFeatherAndMotion) )
		isFeather1 = true;
	else
		isFeather1 = false;

	if( isFeather2 )
	{
		if( confirm_cnt2 < 120 )
			confirm_cnt2++;
	}
	else
	{
		confirm_cnt2 = 0;
	}

	if( isFeather1 )
	{
		if( confirm_cnt1 < 120 )
			confirm_cnt1++;
	}
	else
	{

		confirm_cnt1 = 0;
	}


	if( confirm_cnt2 > 60 )
		*Feather_sum_status2 = true;
	else
		*Feather_sum_status2 = false;

	return ( confirm_cnt1 > 60 );
}

MS_BOOL MDrv_SC_ScenceChangeDetectByNotFound(void *pInstance,const MS_U16 mvNotFound_noIIR)
{
	static MS_U16 mvNotFound_noIIR_pre = 0;
    MS_U16 Diff_notFound = 0;

	if(mvNotFound_noIIR > mvNotFound_noIIR_pre)
	{
		Diff_notFound = mvNotFound_noIIR - mvNotFound_noIIR_pre;
	}
	else
	{
		Diff_notFound = mvNotFound_noIIR_pre - mvNotFound_noIIR;
	}

	if(Diff_notFound > 0x10)
	{
		if( (mvNotFound_noIIR > (mvNotFound_noIIR_pre*2)) || (mvNotFound_noIIR < (mvNotFound_noIIR_pre*2)) )
		{
			mvNotFound_noIIR_pre = mvNotFound_noIIR;
			return true;
		}
		else
		{
			mvNotFound_noIIR_pre = mvNotFound_noIIR;
			return false;
		}
	}
	else
	{
		return false;
	}
}

#define TIME_SHARING_DELAY 4
#define TIME_SHARING_MAX_ID 4
MS_U8 MDrv_SC_TimeSharing(void *pInstance, const XC_OpFrameStates opFrameStatusCur,  MS_U8 *timingSegment , MS_BOOL isSourceChangeFlag)
{
	static XC_OpFrameStates opFrameStatusLast;
	static MS_U8 timingId = 0;

	if( opFrameStatusCur.curOpFrame != opFrameStatusLast.curOpFrame )
		timingId++;

	if( timingId == (TIME_SHARING_DELAY*TIME_SHARING_MAX_ID) || isSourceChangeFlag == 1)
		timingId = 0;

	opFrameStatusLast.curOpFrame = opFrameStatusCur.curOpFrame;

	// return
	*timingSegment = ( timingId % TIME_SHARING_DELAY ) ;
	return ( timingId / TIME_SHARING_DELAY );
}

XC_OpFrameStates MDrv_SC_OpFrame_Status(void *pInstance)
{
	XC_OpFrameStates opFrameStatus;

	opFrameStatus.curOpFrame = (MS_U8)( MApi_XC_R2BYTEMSK(REG_SC_BK12_3A_L, 0x00F0) >> 4 );
	opFrameStatus.maxOpFrame = (MS_U8)( MApi_XC_R2BYTEMSK(REG_SC_BK12_19_L, 0x001F) >> 0 );

	return opFrameStatus;
}

XC_FilmStates MDrv_SC_Film_Status(void *pInstance)
{
	XC_FilmStates filmstatus;

	filmstatus.film22 = FALSE;
	filmstatus.film32 = FALSE;
	filmstatus.filmany = FALSE;
	filmstatus.film_act = FALSE;

	if((MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0008) == 0x0008)   // film22
	{
		filmstatus.film22 = TRUE;
	}
	if((MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0010) == 0x0010)   // film32
	{
		filmstatus.film32 = TRUE;
	}

	if((MS_U16)MApi_XC_R2BYTEMSK(REG_SC_BK0A_21_L, 0x0040) == 0x0040)   // film any
	{
		filmstatus.filmany = TRUE;
	}

	if(filmstatus.film22 ||filmstatus.film32 ||filmstatus.filmany)
	{
		filmstatus.film_act = TRUE;
	}

	return filmstatus;
}

void MDrv_SC_KFC_EODiW_Ctrl(void *pInstance, const MS_BOOL isUkosabihsMc )
{
    MS_U16 reg = 0;

	if( isUkosabihsMc )
		reg = 0x0000; /* disable kfcEodiW  */
	else
		reg = 0x00C0; /* enable kfcEodiW  */

	MApi_XC_W2BYTEMSK( REG_SC_BK22_28_L, reg, 0x00C0 );
}

void MDrv_SC_Mcdi_EODiW_Ctrl(void *pInstance, const MS_BOOL isUkosabihsMc, const MS_BOOL isL107 )
{
    MS_U16 reg = 0;

	if( isUkosabihsMc || isL107 )
    {
        reg = 0x0000; /* disable mcEodiW */
        //printf("MDrv_SC_Mcdi_EODiW_Ctrl ==0\n\n");
    }
	else
    {
        reg = 0x0080; /* enable mcEodiW */
        //printf("MDrv_SC_Mcdi_EODiW_Ctrl ==80\n\n");
    }

    MApi_XC_W2BYTEMSK( REG_SC_BK22_28_L, reg, 0x0080 );
}

void MDrv_SC_Force_Mcdi_Ctrl(void *pInstance, const MS_BOOL isUkosabihsMc)
{
    MS_U16 reg_force_mcdi = 0;

	if( isUkosabihsMc )
		reg_force_mcdi = 0x8080;
	else
		reg_force_mcdi = 0x0000;

	MApi_XC_W2BYTEMSK( REG_SC_BK2A_2F_L, reg_force_mcdi , 0xFFFF );
}

void MDrv_SC_Force_Motion_Ctrl(void *pInstance, const MS_BOOL isL139, const MS_BOOL isUkosabihsMotion )
{
    MS_U16 reg_force_motion;
    MS_U32 mv0_feather, nonMv0_feather;
    MS_U32 total_feather;

    mv0_feather      = MApi_XC_R2BYTE(REG_SC_BK2A_77_L);
    nonMv0_feather   = MApi_XC_R2BYTE(REG_SC_BK2A_7B_L);

    total_feather =  mv0_feather + nonMv0_feather;

    if( (isUkosabihsMotion)&&(total_feather< 0x1200))
	{
		reg_force_motion = 0x008F; /* full motion */
	}
	else if( isL139 )
	{
		reg_force_motion = 0x0080; /* full still */
	}
	else
	{
		reg_force_motion = 0x0000;
	}

	MApi_XC_W2BYTEMSK(REG_SC_BK22_78_L, reg_force_motion, 0x00FF);
}

#define reg_mcdi_feather_gain_MaxValue 0x02
#define reg_mcdi_feather_gain_MinValue 0x00
void MDrv_SC_KFC_Ctrl(void *pInstance, const MS_BOOL isAbihsot )
{
	static MS_U16 u16regmcdifeathergain = reg_mcdi_feather_gain_MaxValue;

	if( isAbihsot )
	{
		if(u16regmcdifeathergain > reg_mcdi_feather_gain_MinValue)
			u16regmcdifeathergain--;
		else
			u16regmcdifeathergain = reg_mcdi_feather_gain_MinValue;
	}
	else
	{
		if(u16regmcdifeathergain < reg_mcdi_feather_gain_MaxValue)
			u16regmcdifeathergain++;
		else
			u16regmcdifeathergain = reg_mcdi_feather_gain_MaxValue;
	}

        //Dummy to control write value, add by Bella 20150708
        if((MApi_XC_R2BYTE(REG_SC_BK23_5D_L)&0x000F)!=0x0000)
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_2D_L, MApi_XC_R2BYTE(REG_SC_BK23_5C_L), 0x0F00);
        }
        else
        {
	MApi_XC_W2BYTEMSK(REG_SC_BK2A_2D_L, (MS_U16)u16regmcdifeathergain << 8, 0x0F00);
        }
}
MS_BOOL MDrv_SC_SourceChange(void *pInstance)
{
	static MS_BOOL bprevSourceChange = false;

	MS_BOOL sourceChangeInitReg = MApi_XC_R2BYTEMSK(REG_SC_BK30_07_L, 0x0001);
	sourceChangeInitReg = sourceChangeInitReg&&(!bprevSourceChange);

	if( sourceChangeInitReg == 1 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_07_L, sourceChangeInitReg << 1, 0x0002);
	}
	else
		MApi_XC_W2BYTEMSK(REG_SC_BK30_07_L, 0x0000, 0x0001);

	bprevSourceChange = sourceChangeInitReg;
	return sourceChangeInitReg;
}

MS_BOOL MDrv_SC_SourceChangeFlag(void *pInstance,MS_BOOL SourceChangeLastTime)
{
	static MS_BOOL bsourceChangeFrm1 = false;
	static MS_BOOL bsourceChangeFrm2 = false;
	static MS_BOOL bsourceChangeFrm3 = false;

	MS_BOOL sourceChangeFlag = bsourceChangeFrm1 || bsourceChangeFrm2 || bsourceChangeFrm3 || SourceChangeLastTime;

	bsourceChangeFrm1 = bsourceChangeFrm2;
	bsourceChangeFrm2 = bsourceChangeFrm3;
	bsourceChangeFrm3 = SourceChangeLastTime;


	return sourceChangeFlag;
}

#define VerticalMovingUdRatio_MaxValue 0x0F
#define VerticalMovingUdRatio_MinValue 0x07
void MDrv_SC_KFC_VerticalMovingUdRatio_Ctrl(void *pInstance,const MS_U32 inSearchUseFeather, MS_BOOL isfilmLike )
{
	static MS_U16 u16VerticalMovingUdRatioValue = VerticalMovingUdRatio_MaxValue;

	if( inSearchUseFeather > 0x300 )
	{
		if ( u16VerticalMovingUdRatioValue > VerticalMovingUdRatio_MinValue)
			u16VerticalMovingUdRatioValue--;
		else
			u16VerticalMovingUdRatioValue = VerticalMovingUdRatio_MinValue;
	}
	else
	{
		if ( u16VerticalMovingUdRatioValue < VerticalMovingUdRatio_MaxValue)
			u16VerticalMovingUdRatioValue++;
		else
			u16VerticalMovingUdRatioValue = VerticalMovingUdRatio_MaxValue;
	}

	if ( isfilmLike )
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_28_L, 0, 0x000F);
	else
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_28_L, (MS_U16)u16VerticalMovingUdRatioValue, 0x000F);

}

#define KFC_8bitMotionGain_MaxValue 0x08
#define KFC_8bitMotionGain_MinValue 0x04 //0 have feather issue.
void MDrv_SC_KFC_8bitMotionGain_Ctrl(void *pInstance,const MS_BOOL isZoomIn, const MS_BOOL isFilmLike, const MS_BOOL McSmallMoving )
{
	static MS_U16 u16KFC8bitMotionGainValue = KFC_8bitMotionGain_MaxValue;

	if( isFilmLike || McSmallMoving )
	{
		if ( u16KFC8bitMotionGainValue < 0x0F/*Max*/ )
			u16KFC8bitMotionGainValue++;
		else
			u16KFC8bitMotionGainValue = 0x0F;
	}
	else if( isZoomIn  )
	{
		//MApi_XC_W2BYTEMSK(REG_SC_BK22_7A_L, 0x1000, 0x1000);
		if ( u16KFC8bitMotionGainValue > KFC_8bitMotionGain_MinValue)
			u16KFC8bitMotionGainValue--;
		else
			u16KFC8bitMotionGainValue = KFC_8bitMotionGain_MinValue;
	}
	else
	{
		//MApi_XC_W2BYTEMSK(REG_SC_BK22_7A_L, 0x0000, 0x1000);
		if ( u16KFC8bitMotionGainValue < KFC_8bitMotionGain_MaxValue)
			u16KFC8bitMotionGainValue++;
		else
			u16KFC8bitMotionGainValue = KFC_8bitMotionGain_MaxValue;
	}

	MApi_XC_W2BYTEMSK(REG_SC_BK2A_3B_L, (MS_U16)u16KFC8bitMotionGainValue<<4, 0x00F0);
}

#define DNR_DIV_MAX 4
void MDrv_SC_DNR_DIV0_Ctrl(void *pInstance, const MS_BOOL isWeakMcNrFilm, const MS_U8 isSourceChange    )
{
	static MS_U8 orgDiv0 = 0;
	static MS_U8 curDiv0 = 0;

	if( isSourceChange )
		orgDiv0 = (MS_U8)(MApi_XC_R2BYTEMSK(REG_SC_BK06_27_L, 0x001C)) >> 2;


	if( isWeakMcNrFilm )
	{
		if( curDiv0 < DNR_DIV_MAX )
			curDiv0++;
	}
	else
	{
		if( curDiv0 > orgDiv0 )
			curDiv0--;
	}

	MApi_XC_W2BYTEMSK(REG_SC_BK06_27_L, ( curDiv0 << 2 ), 0x001C);
}

#define REG_W3_FinialGain_MAX  0x0F
#define REG_W3_FinialGain_MIN   0x08
void MDrv_SC_MCNR_PatchW3FinalGain_Ctrl(void *pInstance, const MS_BOOL isWeakMcNrFilm  )
{
	static MS_U8 reg_patch_w3_final_gain = REG_W3_FinialGain_MIN;

	if( isWeakMcNrFilm )
	{
		if( reg_patch_w3_final_gain < REG_W3_FinialGain_MAX )
			reg_patch_w3_final_gain++;
	}
	else
	{
		if( reg_patch_w3_final_gain > REG_W3_FinialGain_MIN )
			reg_patch_w3_final_gain--;
	}

	MApi_XC_W2BYTEMSK(REG_SC_BK2A_10_L, ( reg_patch_w3_final_gain << 4 ), 0x00F0 );
}

void MDrv_SC_MotionLimitGain_Ctrl(void *pInstance, const MS_BOOL isL153 )
{
	if( isL153 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_36_L, 0x0000, 0x000F);
	}
	else
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_36_L, 0x0008, 0x000F);
	}
}

void MDrv_SC_UCDi_Weight_Boundary_Ctrl(void *pInstance, const XC_Me1HisStates H1) 
{
    MS_BOOL isHorizontalscroll =  ( H1.minMvXdir != 8 ) ? true : false;
    if( isHorizontalscroll )
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_77_L, 0x1807, 0xFFFF);
    }
    else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK22_77_L, 0x0000, 0xFFFF);
    }
}

#define FeatherSstGain_Max 0x0F
#define FeatherSstGain_Min  0x02
void MDrv_SC_featherSstGain_Ctrl(void *pInstance, const MS_BOOL  isMcFilmSlowMotion )
{
	static MS_U8 reg_featherSstGain =  FeatherSstGain_Min;

	if( isMcFilmSlowMotion )
	{
		if( reg_featherSstGain < FeatherSstGain_Max )
			reg_featherSstGain++;
	}
	else
	{
		if( reg_featherSstGain > FeatherSstGain_Min )
			reg_featherSstGain--;
	}

	MApi_XC_W2BYTEMSK(REG_SC_BK2A_29_L, (MS_U16)(reg_featherSstGain << 12 ), 0xF000);
}

#define FeatherSstOffset_Max 0x01 /* default */
#define FeatherSstOffset_Min  0x00
void MDrv_SC_featherSstOffset_Ctrl(void *pInstance, const XC_OpFrameStates opFrameStatusCur, const MS_BOOL  isMcFilmSlowMotion, const MS_BOOL favorStill )
{
	static MS_U8 reg_featherSstOffset =  FeatherSstOffset_Max;
	static XC_OpFrameStates opFrameStatusLast;
	static MS_U8 frameCnt = 0;

	if( opFrameStatusCur.curOpFrame != opFrameStatusLast.curOpFrame )
	{
		if( frameCnt < 16 )
			frameCnt++;
		else
			frameCnt = 0;
	}

	if( isMcFilmSlowMotion )
	{
		if( reg_featherSstOffset > FeatherSstOffset_Min )
			reg_featherSstOffset--;
	}
	else if( favorStill )
	{
		if( frameCnt == 15 )
		{
			if( reg_featherSstOffset < 0x0F/* max of offset value */ )
				reg_featherSstOffset++;
		}
	}
	else
	{
		if( reg_featherSstOffset < FeatherSstOffset_Max )
			reg_featherSstOffset++;
		else if( reg_featherSstOffset > FeatherSstOffset_Max )
			reg_featherSstOffset--;
	}

	//PRT_TRP("==reg_featherSstOffset=%x\n",reg_featherSstOffset);

	opFrameStatusLast.curOpFrame = opFrameStatusCur.curOpFrame;

        //Dummy to control write value, add by Bella 20150708
        if((MApi_XC_R2BYTE(REG_SC_BK23_5C_L)&0x0F00)!=0x0000)
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_29_L, MApi_XC_R2BYTE(REG_SC_BK23_5C_L), 0x0F00);
        }
        else
        {
	MApi_XC_W2BYTEMSK(REG_SC_BK2A_29_L, (MS_U16)(reg_featherSstOffset << 8 ), 0x0F00);
}
}

void MDrv_SC_NR_CheckMotionBigChange(void *pInstance, const MS_BOOL isHDSrc, MS_BOOL *motionBigChange, MS_BOOL *motionBigChange2 )
{
	static MS_U16 motionValuePrv;
    MS_U16 motionValue = 0;

	motionValue = MApi_XC_R2BYTEMSK(REG_SC_BK0A_2E_L, 0xFFFF);

	*motionBigChange = false;
	*motionBigChange2 = false;
	if ( motionValue != motionValuePrv )
	{
		*motionBigChange =
			( motionValue & 0x7fff )    > 0x80 && ( ( motionValue & 0x7fff )    /8 > ( motionValuePrv & 0x7fff ) ) ? true :
			( motionValuePrv & 0x7fff ) > 0x80 && ( ( motionValuePrv & 0x7fff ) /8 > ( motionValue & 0x7fff ) ) ? true : false;

		if ( isHDSrc )
			*motionBigChange2 =
			( motionValue & 0x7fff )    > 0x800 && ( ( motionValue & 0x7fff )    /8 > ( motionValuePrv & 0x7fff ) ) ? true :
			( motionValuePrv & 0x7fff ) > 0x800 && ( ( motionValuePrv & 0x7fff ) /8 > ( motionValue & 0x7fff ) ) ? true : false;
		else
			*motionBigChange2 =
			( motionValue & 0x7fff )    > 0x200 && ( ( motionValue & 0x7fff )    /8 > ( motionValuePrv & 0x7fff ) ) ? true :
			( motionValuePrv & 0x7fff ) > 0x200 && ( ( motionValuePrv & 0x7fff ) /8 > ( motionValue & 0x7fff ) ) ? true : false;

	}
	motionValuePrv = motionValue;
}

#define ME_MovingWeight_Max_SD 0x10
#define ME_MovingWeight_Max_HD 0x10
#define ME_MovingWeight_Max2_SD 0x12
#define ME_MovingWeight_Max2_HD 0x12
#define ME_MovingWeight_Min  0x08

#define MCNR_MOTIONGAIN_MAX2 0xC
#define MCNR_MOTIONGAIN_MAX  0x8
#define MCNR_MOTIONGAIN_MIN  0x7

#define PDNR_MOTIONGAIN_MAX2 0xC
#define PDNR_MOTIONGAIN_MAX  0x8
#define PDNR_MOTIONGAIN_MIN  0x4

void MDrv_SC_NR_Driver(void *pInstance, const XC_FilmStates filmstatus, const MS_BOOL isHDSrc )
{
	static MS_U8 movingWeight = ME_MovingWeight_Max_SD;
	static MS_U8 mcnr_motionGain = MCNR_MOTIONGAIN_MAX;
	static MS_U8 pdnr_motionGain = 0x8;
	static MS_U8 isFilmandMotion = 0;
	static MS_U8 stillFrameCnt = 0;

	// special patch for HQVSD random noise pattern
	static MS_U8 patchHQVSDSlowMotionScenCnt = 0;
	MS_U8 me1_X_histogram[8];
    MS_U16 mv0featheringCnt = 0;
    MS_U16 mvhistogramMV0Cnt = 0;
    MS_U16 mvhistogramNonMV0Cnt = 0;
	// special patch for sec woman in dark
	static MS_U8 patchSecWomanInDarkScnenCnt = 0;
	static MS_U8 mcnrUpdateTableLevelCnt = 0x80;
    MS_U8 mcnrUpdateTableLevel = 0;
    MS_U16 mcnrMoveStep = 0;

    MS_U32 me1_mv0blk = 0, me1_nonMv0blk = 0;
    MS_U32 me2_mv0blk = 0, me2_nonMv0blk = 0;
    MS_BOOL movingBlkMoreThanMV0Blk1 = FALSE;
    MS_BOOL movingBlkMoreThanMV0Blk2 = FALSE;
    MS_BOOL isStill = FALSE;
    MS_BOOL isfilmNotfilm22 = FALSE;

    MS_BOOL motionBigChange = FALSE, motionBigChange2 = FALSE;
	MDrv_SC_NR_CheckMotionBigChange(pInstance, isHDSrc, &motionBigChange, &motionBigChange2  );

	// read status
	me1_mv0blk 	  = MApi_XC_R2BYTE(REG_SC_BK17_48_L);
	me1_nonMv0blk = MApi_XC_R2BYTE(REG_SC_BK17_49_L);
	me2_mv0blk 	  = MApi_XC_R2BYTE(REG_SC_BK17_4B_L);
	me2_nonMv0blk = MApi_XC_R2BYTE(REG_SC_BK17_4C_L);

	movingBlkMoreThanMV0Blk1 =
		isHDSrc ? ( ( me2_nonMv0blk == 0XFFF ) || ( ( me2_nonMv0blk > 0x1000 ) && ( me2_nonMv0blk > me1_mv0blk*2 ) ) ):
		( ( me1_nonMv0blk > 0x40  ) && ( me1_nonMv0blk > me1_mv0blk/2 ) ) || ( ( me2_nonMv0blk > 0x100 ) && ( me2_nonMv0blk > me1_mv0blk/2 ) );

	movingBlkMoreThanMV0Blk2 =
		isHDSrc ? ( ( me1_nonMv0blk > 0x10  ) && ( me1_nonMv0blk > me1_mv0blk*2 ) ) || ( ( me2_nonMv0blk > 0x100 ) && ( me2_nonMv0blk > me2_mv0blk*2 ) ) :
		( ( me1_nonMv0blk > 0x10  ) && ( me1_nonMv0blk > me1_mv0blk/8 ) ) || ( ( me2_nonMv0blk > 0x100 ) && ( me2_nonMv0blk > me2_mv0blk/4 ) );

	isStill = ( ( me1_mv0blk > 0x40 ) && ( me1_mv0blk > me1_nonMv0blk/8 ) ) ||
		( ( me1_mv0blk > 0x4  ) && ( me1_nonMv0blk == 0 ) && ( me2_nonMv0blk < 0x40 ) ); // patch for sony tree

	// special patch for HQVSD random noise pattern
	me1_X_histogram[0] = MApi_XC_R2BYTEMSK(REG_SC_BK17_60_L, 0x00FF);
	me1_X_histogram[1] = MApi_XC_R2BYTEMSK(REG_SC_BK17_60_L, 0xFF00) >> 8;
	me1_X_histogram[2] = MApi_XC_R2BYTEMSK(REG_SC_BK17_61_L, 0x00FF);
	me1_X_histogram[3] = MApi_XC_R2BYTEMSK(REG_SC_BK17_61_L, 0xFF00) >> 8;
	me1_X_histogram[4] = MApi_XC_R2BYTEMSK(REG_SC_BK17_62_L, 0x00FF);
	me1_X_histogram[5] = MApi_XC_R2BYTEMSK(REG_SC_BK17_62_L, 0xFF00) >> 8;
	me1_X_histogram[6] = MApi_XC_R2BYTEMSK(REG_SC_BK17_63_L, 0x00FF);
	me1_X_histogram[7] = MApi_XC_R2BYTEMSK(REG_SC_BK17_63_L, 0xFF00) >> 8;

	mv0featheringCnt = MApi_XC_R2BYTE(REG_SC_BK2A_77_L);
	mvhistogramMV0Cnt = MApi_XC_R2BYTEMSK(REG_SC_BK17_5F_L, 0x00FF);
	mvhistogramNonMV0Cnt = me1_X_histogram[0] + me1_X_histogram[1] + me1_X_histogram[2] + me1_X_histogram[3] +
		me1_X_histogram[4] + me1_X_histogram[5] + me1_X_histogram[6] + me1_X_histogram[7];

	if ( motionBigChange || motionBigChange2 )
		patchHQVSDSlowMotionScenCnt = 0;
	else if ( mv0featheringCnt == 0 && // mv0 feathering count
		me2_mv0blk > 0x140 && me2_nonMv0blk < 0xC0 &&
		mvhistogramNonMV0Cnt < 8 &&
		mvhistogramMV0Cnt < 0x20 )
	{
		if ( patchHQVSDSlowMotionScenCnt <= 0xf0 )
			patchHQVSDSlowMotionScenCnt ++;
	}
	else
	{
		if ( patchHQVSDSlowMotionScenCnt > 0x10 )
			patchHQVSDSlowMotionScenCnt -= 0x10;
		else
			patchHQVSDSlowMotionScenCnt = 0;
	}

	// special patch for sec woman in dark
	// 1. when in dark scene
	if ( mv0featheringCnt ==0  && // mv0 feathering count
		me2_nonMv0blk > me2_mv0blk/4 && me2_nonMv0blk > 0x25 &&
		mvhistogramNonMV0Cnt < 4 && mvhistogramMV0Cnt < 8 )
	{
		if ( patchSecWomanInDarkScnenCnt < 0xf0 )
			patchSecWomanInDarkScnenCnt ++;
	}
	else if (
		me2_nonMv0blk > me2_mv0blk/4 && me2_nonMv0blk > 0x30 &&
		mvhistogramNonMV0Cnt < 8 && mvhistogramMV0Cnt < 0x20 )
	{
		if ( patchSecWomanInDarkScnenCnt < 0xf0 )
			patchSecWomanInDarkScnenCnt ++;
	}
	else
	{
		if ( patchSecWomanInDarkScnenCnt > 0x10 )
			patchSecWomanInDarkScnenCnt -= 0x10;
		else
			patchSecWomanInDarkScnenCnt = 0;
	}


	mcnrMoveStep = 4; //(MApi_XC_R2BYTE(REG_SC_BK30_08_L)) >> 8;
	if ( isHDSrc || motionBigChange || motionBigChange2 )
	{
		mcnrUpdateTableLevelCnt = 0x80;
	}
	else if ( patchHQVSDSlowMotionScenCnt >= 0x10 )
	{
		if ( mcnrUpdateTableLevelCnt < 0xF0 )
			mcnrUpdateTableLevelCnt += mcnrMoveStep;
	}
	else if ( patchSecWomanInDarkScnenCnt >= 0x10 )
	{
		if ( mcnrUpdateTableLevelCnt > 10 )
			mcnrUpdateTableLevelCnt -= mcnrMoveStep;
	}
	else // go back to 0x80
	{
		if ( mcnrUpdateTableLevelCnt > 0x80 )
			mcnrUpdateTableLevelCnt -= mcnrMoveStep;
		else if ( mcnrUpdateTableLevelCnt < 0x80 )
			mcnrUpdateTableLevelCnt += mcnrMoveStep;
	}
	mcnrUpdateTableLevel = mcnrUpdateTableLevelCnt >> 4;

	if ( false == isHDSrc )
	{
		// Reduce PDNR
		if ( mcnrUpdateTableLevel <= 2 )
		{
			MApi_XC_W2BYTE(REG_SC_BK2A_1C_L, 0x4444 );
			MApi_XC_W2BYTE(REG_SC_BK2A_1D_L, 0x4444 );
			MApi_XC_W2BYTE(REG_SC_BK2A_1E_L, 0x3333 );
			MApi_XC_W2BYTE(REG_SC_BK2A_1F_L, 0x3300 );

			MApi_XC_W2BYTE(REG_SC_BK2A_24_L, 0x4444 );
			MApi_XC_W2BYTE(REG_SC_BK2A_25_L, 0x4444 );
			MApi_XC_W2BYTE(REG_SC_BK2A_26_L, 0x3333 );
			MApi_XC_W2BYTE(REG_SC_BK2A_27_L, 0x3300 );

			MApi_XC_W2BYTE(REG_SC_BK2A_20_L, 0x2222 );
			MApi_XC_W2BYTE(REG_SC_BK2A_21_L, 0x1111 );
		}
		else if ( mcnrUpdateTableLevel <= 6 )
		{
			MApi_XC_W2BYTE(REG_SC_BK2A_1C_L, 0x6666 );
			MApi_XC_W2BYTE(REG_SC_BK2A_1D_L, 0x5555 );
			MApi_XC_W2BYTE(REG_SC_BK2A_1E_L, 0x5555 );
			MApi_XC_W2BYTE(REG_SC_BK2A_1F_L, 0x4400 );

			MApi_XC_W2BYTE(REG_SC_BK2A_24_L, 0x6666 );
			MApi_XC_W2BYTE(REG_SC_BK2A_25_L, 0x5555 );
			MApi_XC_W2BYTE(REG_SC_BK2A_26_L, 0x5555 );
			MApi_XC_W2BYTE(REG_SC_BK2A_27_L, 0x4400 );

			MApi_XC_W2BYTE(REG_SC_BK2A_20_L, 0x3333 );
			MApi_XC_W2BYTE(REG_SC_BK2A_21_L, 0x2211 );
		}
		else //if ( mcnrUpdateTableLevel <= 10 ) // 6 ~ 10 default
		{
			MApi_XC_W2BYTE(REG_SC_BK2A_1C_L, 0x8888 );
			MApi_XC_W2BYTE(REG_SC_BK2A_1D_L, 0x7777 );
			MApi_XC_W2BYTE(REG_SC_BK2A_1E_L, 0x6666 );
			MApi_XC_W2BYTE(REG_SC_BK2A_1F_L, 0x6600 );

			MApi_XC_W2BYTE(REG_SC_BK2A_24_L, 0x8888 );
			MApi_XC_W2BYTE(REG_SC_BK2A_25_L, 0x7777 );
			MApi_XC_W2BYTE(REG_SC_BK2A_26_L, 0x6666 );
			MApi_XC_W2BYTE(REG_SC_BK2A_27_L, 0x6600 );

			MApi_XC_W2BYTE(REG_SC_BK2A_20_L, 0x4444 );
			MApi_XC_W2BYTE(REG_SC_BK2A_21_L, 0x4432 );
		}

		// Increase Chroma NR
		if ( mcnrUpdateTableLevel >= 0xC )
		{
			MApi_XC_W2BYTE(REG_SC_BK2A_30_L, 0xCCCC );
			MApi_XC_W2BYTE(REG_SC_BK2A_31_L, 0xCCCC );
			MApi_XC_W2BYTE(REG_SC_BK2A_32_L, 0xCCBA );
			MApi_XC_W2BYTE(REG_SC_BK2A_33_L, 0x8640 );
		}
		else if ( mcnrUpdateTableLevel >= 0xA )
		{
			MApi_XC_W2BYTE(REG_SC_BK2A_30_L, 0xAAAA );
			MApi_XC_W2BYTE(REG_SC_BK2A_31_L, 0xAAAA );
			MApi_XC_W2BYTE(REG_SC_BK2A_32_L, 0xA865 );
			MApi_XC_W2BYTE(REG_SC_BK2A_33_L, 0x4320 );
		}
		else //default
		{
			MApi_XC_W2BYTE(REG_SC_BK2A_30_L, 0x8888 );
			MApi_XC_W2BYTE(REG_SC_BK2A_31_L, 0x7766 );
			MApi_XC_W2BYTE(REG_SC_BK2A_32_L, 0x5432 );
			MApi_XC_W2BYTE(REG_SC_BK2A_33_L, 0x0000 );
		}
	}

	isfilmNotfilm22 = ( filmstatus.filmany || filmstatus.film32 ); // keep film22 as same NR strength
	if ( isfilmNotfilm22 )
	{
		if ( movingBlkMoreThanMV0Blk2 )
			isFilmandMotion = 0x10;
		else if ( isFilmandMotion != 0 )
			isFilmandMotion --;
	}
	else
		isFilmandMotion = 0;

	// patch of increase or reduce MV0moving weight
	if ( movingBlkMoreThanMV0Blk1 /*|| motionBigChange2*/ )
		movingWeight = ME_MovingWeight_Min;
	else if( movingBlkMoreThanMV0Blk2 )
	{
		if( movingWeight > ME_MovingWeight_Min )
			movingWeight--;
		else
			movingWeight = ME_MovingWeight_Min;
	}
	else if ( isStill || mcnrUpdateTableLevel >= 0xA )
	{
		if ( isHDSrc )
		{
			if( movingWeight < ME_MovingWeight_Max2_HD )
				movingWeight++;
			else
				movingWeight = ME_MovingWeight_Max2_HD;
		}
		else
		{
			if( movingWeight < ME_MovingWeight_Max2_SD )
				movingWeight++;
			else
				movingWeight = ME_MovingWeight_Max2_SD;
		}
	}
	else
	{
		if ( isHDSrc )
		{
			if( movingWeight < ME_MovingWeight_Max_HD )
				movingWeight++;
			else
				movingWeight = ME_MovingWeight_Max_HD;
		}
		else
		{
			if( movingWeight < ME_MovingWeight_Max_SD )
				movingWeight++;
			else
				movingWeight = ME_MovingWeight_Max_SD;
		}
	}

	MApi_XC_W2BYTEMSK(REG_SC_BK17_19_L, ( movingWeight<<8 )  | movingWeight, 0x1F1F);

	// 2. patch mcnr/pdnr motion gain
	if ( motionBigChange2 ) // for scene change or film mode moving
	{
		mcnr_motionGain = MCNR_MOTIONGAIN_MAX2;
	}
	else if ( motionBigChange ) // reduce MCNR when mv is changing...
	{
		if( mcnr_motionGain < MCNR_MOTIONGAIN_MAX )
			mcnr_motionGain ++;
		//else
		//	mcnr_motionGain = MCNR_MOTIONGAIN_MAX;
	}
	else //PDNR default park at min
	{
		if( mcnr_motionGain > MCNR_MOTIONGAIN_MIN )
			mcnr_motionGain --;
		else
			mcnr_motionGain = MCNR_MOTIONGAIN_MIN;
	}

	// reduce PDNR when bigChange, film & motion
	if ( motionBigChange2 )
	{
		pdnr_motionGain = PDNR_MOTIONGAIN_MAX2;
	}
	else if ( motionBigChange || movingBlkMoreThanMV0Blk2 || isFilmandMotion || mcnrUpdateTableLevel <= 0x4 )
	{
		if( pdnr_motionGain < PDNR_MOTIONGAIN_MAX )
			pdnr_motionGain ++;
		else
			pdnr_motionGain = PDNR_MOTIONGAIN_MAX;
	}
	else if ( isStill || mcnrUpdateTableLevel >= 0xA ) // for sony temple tree, only reduce NR when isStill
	{
		if( pdnr_motionGain > PDNR_MOTIONGAIN_MIN )
			pdnr_motionGain --;
		else
			pdnr_motionGain = PDNR_MOTIONGAIN_MIN;
	}
	else
	{
		if( pdnr_motionGain > PDNR_MOTIONGAIN_MIN )
			pdnr_motionGain --;
		else
			pdnr_motionGain = PDNR_MOTIONGAIN_MIN;
	}

	MApi_XC_W2BYTEMSK(REG_SC_BK2A_10_L, mcnr_motionGain<<8, 0x0F00);
	MApi_XC_W2BYTEMSK(REG_SC_BK2A_15_L, (pdnr_motionGain<<4) | pdnr_motionGain, 0x00FF);


	// Adjust NR table... strong NR table can reduce Noise
	if ( isStill )
	{
		if ( stillFrameCnt < 0x80 )
			stillFrameCnt ++;
	}
	else if ( motionBigChange || movingBlkMoreThanMV0Blk2 || isFilmandMotion )
	{
		stillFrameCnt = 0;
	}

}


void MDrv_SC_MCDi_En_Ctrl(void *pInstance, const MS_BOOL disable )
{
	if( disable )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, 0x0000, 0x0080); // disable MCDi
        //MApi_XC_W2BYTEMSK(REG_SC_BK22_18_L, 0x0080, 0x0080); // Enable MADi_SST
	}
	else
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, 0x0080, 0x0080);// enable MCDi
        //MApi_XC_W2BYTEMSK(REG_SC_BK22_18_L, 0x0000, 0x0080); // disable MADi_SST
	}
}

void MDrv_SC_SST_Ctrl( void *pInstance,const MS_BOOL disable, const MS_U32 FeatherSum, const MS_BOOL isHDSrc )
{
    static MS_BOOL reg_sst_en = true;
    MS_U32 mv0_feather;
    mv0_feather = MApi_XC_R2BYTE(REG_SC_BK2A_77_L);
    if( disable ||((FeatherSum > 0x1200)&&(mv0_feather>0x1100)&&(!isHDSrc)&&(SZ_PATTERN_ENABLE ==1))) //Add feathersum for TSB#3 capter4 white noise
	{
		reg_sst_en = false;
	}
	else
	{
		reg_sst_en = true;
	}

	MApi_XC_W2BYTEMSK(REG_SC_BK2A_04_L, (MS_U16)(reg_sst_en << 8), 0x0100);

}

MS_BOOL checkHQVMovingBarPatch(void *pInstance, const MS_U8 colorSum )  // special patch for SEC : HQV 2.0 moving white bar, need to reduce history weight
{
	MS_BOOL isHQVMovingBarPattern = false;

	if ( // BK17
		 ( MApi_XC_R2BYTEMSK(REG_SC_BK17_60_L, 0x00FF)      ) < 0x4 &&
		 ( MApi_XC_R2BYTEMSK(REG_SC_BK17_60_L, 0xFF00) >> 8 ) < 0x4 &&
		 ( MApi_XC_R2BYTEMSK(REG_SC_BK17_61_L, 0x00FF)      ) < 0x4 &&
		 ( MApi_XC_R2BYTEMSK(REG_SC_BK17_61_L, 0xFF00) >> 8 ) < 0x4 &&
		 ( MApi_XC_R2BYTEMSK(REG_SC_BK17_62_L, 0x00FF)      ) < 0x4 &&
		 ( MApi_XC_R2BYTEMSK(REG_SC_BK17_62_L, 0xFF00) >> 8 ) < 0x4 &&
		 ( MApi_XC_R2BYTEMSK(REG_SC_BK17_63_L, 0x00FF)      ) < 0x4 &&
		 ( MApi_XC_R2BYTEMSK(REG_SC_BK17_63_L, 0xFF00) >> 8 ) < 0x4 &&
		 ( MApi_XC_R2BYTEMSK(REG_SC_BK17_5F_L, 0x00FF)      ) > 0x40 &&
		 // BK2A
		 ( MApi_XC_R2BYTEMSK(REG_SC_BK2A_78_L, 0xFFFF)      ) > 0xB00 &&
		 ( MApi_XC_R2BYTEMSK(REG_SC_BK2A_7B_L, 0xFFFF)      ) < 0x50 &&
		 ( MApi_XC_R2BYTEMSK(REG_SC_BK2A_7F_L, 0xFFFF)      ) < 0x40 &&
		 ( MApi_XC_R2BYTEMSK(REG_SC_BK2A_74_L, 0xFFFF)      ) < 0x100 &&
		 ( MApi_XC_R2BYTEMSK(REG_SC_BK2A_77_L, 0xFFFF)      ) < 0x100 &&
		 colorSum == 0 )
		isHQVMovingBarPattern = true;
            return isHQVMovingBarPattern;
}

#define UCDi_OP1_HisWeight_MaxValue 0x0E    // history weight = 6
#define UCDi_OP1_HisWeight_MinValue 0x04    // history weight = 2
void MDrv_SC_UCDi_OP1_HisWeight_Ctrl(void *pInstance,const MS_BOOL Bouncing_status,const MS_BOOL Feather_sum_status,const XC_FilmStates filmstatus, const MS_U8 colorSum, const MS_U32 FeatherSum, const MS_BOOL isHDSrc)
{
    static MS_U32 reg_uc_his_weight = UCDi_OP1_HisWeight_MinValue;
    static MS_U32 mv0_feather_prv = 0x0000;
    MS_U32 mv0_feather;
    MS_U8 u8MotionValue;

    u8MotionValue = MApi_XC_R2BYTE(REG_SC_BK0A_1A_L)& 0x007F;

    mv0_feather = MApi_XC_R2BYTE(REG_SC_BK2A_77_L);
    //Add iir filter
    mv0_feather = (mv0_feather_prv*3 + mv0_feather)/4;
    mv0_feather_prv = mv0_feather;

    if((u8MotionValue<0x0A) && (SZ_PATTERN_ENABLE ==1))//Add for AV patterns, titles are shaking issue
    {
        reg_uc_his_weight = UCDi_OP1_HisWeight_MinValue;
        //printf("u8MotionValue = %d, SZ_PATTERN_ENABLE = %d\n\n",u8MotionValue, SZ_PATTERN_ENABLE);
    }
    else if(((FeatherSum > 0x1200)&&(mv0_feather>0x1100)&&(!isHDSrc))&& (SZ_PATTERN_ENABLE ==1)) //Add feathersum for TSB#3 capter4 white noise
	{
        reg_uc_his_weight = UCDi_OP1_HisWeight_MinValue;
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_50_L, 0x0000, 0x0002);
        //printf("FeatherSum = %d, mv0_feather = %d\n\n",(int)FeatherSum, (int)mv0_feather);
    }
    else if( Bouncing_status || Feather_sum_status )
	{
        reg_uc_his_weight = UCDi_OP1_HisWeight_MinValue;
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_50_L, 0x0002, 0x0002);
        //printf("Bouncing_status = %d, Feather_sum_status = %d\n\n",Bouncing_status, Feather_sum_status);
	}
#if SEC_HQV_PATTERN
	else if ( checkHQVMovingBarPatch( colorSum ) ) // special patch for SEC : HQV 2.0 moving white bar, need to reduce history weight
	{
		if( reg_uc_his_weight > 0x8 )
			reg_uc_his_weight--;
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_50_L, 0x0002, 0x0002);
		//printf("reg_uc_his_weight = %d\n\n",(int)reg_uc_his_weight);
	}
#endif
	else
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_50_L, 0x0002, 0x0002);
		if( reg_uc_his_weight < UCDi_OP1_HisWeight_MaxValue )
			reg_uc_his_weight++;
		else
			reg_uc_his_weight = UCDi_OP1_HisWeight_MaxValue;
        //printf("else reg_uc_his_weight= %d\n\n",(int)reg_uc_his_weight);
	}

	if(filmstatus.film_act)
	{
		reg_uc_his_weight = UCDi_OP1_HisWeight_MaxValue;
	}

        //Dummy to control write value, add by Bella 20150708
        if((MApi_XC_R2BYTE(REG_SC_BK23_5C_L)&0x000E)!=0x0000)
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, MApi_XC_R2BYTE(REG_SC_BK23_5C_L), 0x000E);
        }
        else
        {
	        MApi_XC_W2BYTEMSK(REG_SC_BK2A_02_L, reg_uc_his_weight, 0x000E);
        }

}

#define SPF_OP1_HisWeight_MaxValue 0x06    // history weight = 6
#define SPF_OP1_HisWeight_MinValue 0x02    // history weight = 2
void MDrv_SC_SPF_OP1_HisWeight_Ctrl(void *pInstance,const MS_BOOL Bouncing_status)
{
	static MS_U32 reg_uc_his_weight = SPF_OP1_HisWeight_MinValue;

	if( Bouncing_status  )
	{
		reg_uc_his_weight = SPF_OP1_HisWeight_MinValue;
#ifdef DBG_HISW8T_LC
		MApi_XC_W2BYTEMSK(REG_SC_BK30_46_L, 0x0007, 0x000F );
#endif
	}
	else
	{
		if( reg_uc_his_weight < SPF_OP1_HisWeight_MaxValue )
			reg_uc_his_weight++;
		else
			reg_uc_his_weight = SPF_OP1_HisWeight_MaxValue;
#ifdef DBG_HISW8T_LC
		MApi_XC_W2BYTEMSK(REG_SC_BK30_46_L, 0x0000, 0x000F );
#endif
	}

	MApi_XC_W2BYTEMSK(REG_SC_BK09_5C_L, reg_uc_his_weight, 0x0007);

}

//
//		MV0 comfirm for such as like "Piano Bar" motionless pattern
//
#define KFCnonHistoryMotionDivideMode_MaxValue 0x02
#define KFCnonHistoryMotionDivideMode_MinValue 0x01

void MDrv_SC_KFC_EODiW_Motionless(void *pInstance)
{
    MS_U32 mv0Comfirm = 0,  nonMV0Comfirm = 0, mvNotFound = 0, luma = 0;
	static MS_U32 prevMV0Comfirm = 0;
	static MS_U32 prevNonMV0Comfirm = 0;
	static MS_U32 prevMvNotFound = 0;
	static MS_U32 prevLuma = 0;
	static MS_U32 prevComfirmValue = 0;
    MS_U32 comfirmValue = 0;
	static MS_U32 prevComfirmValue2 = 0;
    MS_U32 comfirmValue2 =0;
	static MS_U32 KFCnonHistoryMotionDivideMode = KFCnonHistoryMotionDivideMode_MaxValue;

	mv0Comfirm			= MApi_XC_R2BYTE(REG_SC_BK09_78_L);
	nonMV0Comfirm	  = MApi_XC_R2BYTE(REG_SC_BK09_79_L);
	mvNotFound			= MApi_XC_R2BYTE(REG_SC_BK09_7A_L);
	luma            = MApi_XC_R2BYTE(REG_SC_BK1A_06_L);

	mv0Comfirm = (prevMV0Comfirm * 3 + mv0Comfirm) >> 2;
	prevMV0Comfirm = mv0Comfirm;

	nonMV0Comfirm = (prevNonMV0Comfirm * 3 + nonMV0Comfirm) >> 2;
	prevNonMV0Comfirm = nonMV0Comfirm;

	mvNotFound = (prevMvNotFound * 3 + mvNotFound) >> 2;
	prevMvNotFound = mvNotFound;

	luma = (prevLuma * 3 + luma) >> 2;
	prevLuma = luma;
	//  ==================================================	//
	/* mv0 check !! */
	if(mv0Comfirm > (mvNotFound * 20) )
		comfirmValue = mv0Comfirm - mvNotFound * 20;
	else
		comfirmValue = 0;

	comfirmValue = (prevComfirmValue * 3 + comfirmValue) >> 2;
	prevComfirmValue = comfirmValue;

	//  ==================================================	//
	/* Small moving check !! */
	if(mv0Comfirm > (nonMV0Comfirm * 1) )
		comfirmValue2 = mv0Comfirm - nonMV0Comfirm * 1;
	else
		comfirmValue2 = 0;

	comfirmValue2 = (prevComfirmValue2 * 3 + comfirmValue2) >> 2;
	prevComfirmValue2 = comfirmValue2;

	//  ==================================================	//
	if( (comfirmValue > 100) && (comfirmValue2 > 100) && (luma < 0x2000) )
	{
		//KFCnonHistoryMotionDivideMode = 0x00;
		if(KFCnonHistoryMotionDivideMode > KFCnonHistoryMotionDivideMode_MinValue)
			KFCnonHistoryMotionDivideMode--;
		else
			KFCnonHistoryMotionDivideMode = KFCnonHistoryMotionDivideMode_MinValue;
	}
	else
	{
		if(KFCnonHistoryMotionDivideMode < KFCnonHistoryMotionDivideMode_MaxValue)
			KFCnonHistoryMotionDivideMode++;
		else
			KFCnonHistoryMotionDivideMode = KFCnonHistoryMotionDivideMode_MaxValue;
	}
	/* Main window KFC non-history motion divide mode */
	MApi_XC_W2BYTEMSK(REG_SC_BK22_28_L, KFCnonHistoryMotionDivideMode << 4, 0x0030);
}

void MDrv_SC_Edison_VectorNR0(void *pInstance, const MS_U16 isFeatherAndMotion, const MS_BOOL isMcFilm,
			      const MS_BOOL isLrLetterBox, const MS_U32 uComplex, const MS_U16 mv0Comfirm,
			      const MS_BOOL isMcOutOfSearchRange )
{
    MS_U16 FAM = 0;
	static MS_U16 FAM_iir = 0;
	static MS_U8 _noFeatherCnt = 0;
    MS_BOOL mcnr_vector0 = FALSE;
    MS_BOOL isCplx = FALSE;
    MS_BOOL isRealMv0 = FALSE;

	isRealMv0 = ( mv0Comfirm > 0x20 ) ? true : false;
	isCplx = ( uComplex > 0x1500 ) ? true : false;

	FAM = isFeatherAndMotion;
	FAM = ((FAM_iir*3 )+ FAM ) >> 2;
	FAM_iir = FAM;

	if( FAM_iir < 5 )
	{
		if( _noFeatherCnt <= 64)
			_noFeatherCnt++;
	}
	else
	{
		_noFeatherCnt = 0;
	}

	mcnr_vector0 = ( _noFeatherCnt > 50 ) ? true : false;

	if ( ( mcnr_vector0 && isCplx && isRealMv0 ) || isMcFilm || isLrLetterBox || isMcOutOfSearchRange )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK17_70_L, 0x0002, 0x0002); /*F2 Force MNR interpolation MV enable*/
		MApi_XC_W2BYTEMSK(REG_SC_BK17_72_L, 0x0000, 0x3F3F); /*F2 Force MCNR interpolation YMV/XMV */
	}
	else/*disable*/
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK17_70_L, 0x0000, 0x0002); /*F2 Force MNR interpolation MV enable*/
		MApi_XC_W2BYTEMSK(REG_SC_BK17_72_L, 0x0000, 0x3F3F); /*F2 Force MCNR interpolation YMV/XMV */
	}
}

#define L139_COLORSUM 0x0A /*CVBS*/
MS_BOOL MDrv_SC_Agate_L139(void *pInstance, const MS_U32 mvComfirm, const MS_U32 colorSum, const MS_U32 uComplex, const MS_BOOL meStill, const  XC_Me1HisStates H2 /*, const MS_BOOL ukoSabihS_en*/ )
{
    MS_BOOL me1IsMv0 = 0;
    MS_BOOL isL139 = 0;

	if((H2.minMvXdir == 8) && (H2.minMvYdir == 8) &&(H2.minMvXBin > 13) && (H2.minMvYBin > 13))
	{
		me1IsMv0 = true;
	}
	else
	{
		me1IsMv0 = false;
	}

	if( (mvComfirm > 95) && (colorSum < L139_COLORSUM)&&(uComplex > 3456) && meStill && me1IsMv0 )
	{
		isL139 = true;
	}
	else
	{
		isL139 = false;
	}
    //printf("isL139== %d \n\n",isL139);
	return isL139;
}

MS_BOOL MDrv_SC_Agate_L107(void *pInstance, const MS_U32 mvComfirm, const MS_U32 uComplex, const XC_GMvStates gMv, const MS_U32 u32MotionValue, const MS_U32 total_feather, const MS_U8 inputSource )
{
    MS_BOOL mvComfirmId = FALSE, colorId = FALSE, cplxId = FALSE, motionId = FALSE, featherId = FALSE;
    MS_BOOL notFoundMvId = FALSE, foundMvId = FALSE, foundNonMv0Id = FALSE;
    MS_BOOL gMvId = FALSE;
    MS_BOOL L107 = FALSE;

    MS_U32 notFoundMv = 0;
	static MS_U32 notFoundMv_iir = 0;
    MS_U32 foundMv0 = 0;
	static MS_U32 foundMv0_iir = 0;
    MS_U32 foundNonMv0 = 0;
	static MS_U32 foundNonMv0_iir = 0;
    MS_U32 colorSum = 0;
	static MS_U32 colorSum_iir = 0;

	foundMv0 = MApi_XC_R2BYTE(REG_SC_BK17_48_L);
	foundMv0 = ((foundMv0_iir*3 )+ foundMv0 ) >> 2;
	foundMv0_iir = foundMv0;

	foundNonMv0 = MApi_XC_R2BYTE(REG_SC_BK17_49_L);
	foundNonMv0 = ((foundNonMv0_iir*3 )+ foundNonMv0 ) >> 2;
	foundNonMv0_iir = foundNonMv0;

	notFoundMv = MApi_XC_R2BYTE(REG_SC_BK17_4A_L);
	notFoundMv = ((notFoundMv_iir*3 )+ notFoundMv ) >> 2;
	notFoundMv_iir = notFoundMv;

	colorSum = MApi_XC_R2BYTE(REG_SC_BK1A_6E_L);
	colorSum = ((colorSum_iir*3 )+ colorSum ) >> 2;
	colorSum_iir = colorSum;

	motionId = (u32MotionValue > 0x2C00) ? true : false;
	mvComfirmId = ( mvComfirm < 16 ) ? true : false;
	if( inputSource == 1 ) /*CVBS*/
		colorId = ( (colorSum > 0x2300) && (colorSum < 0x2B00) ) ? true : false;
	else if( inputSource == 2 )
		colorId = ( (colorSum > 0x1700) && (colorSum < 0x1B00) ) ? true : false;
	else
		colorId = false;

	cplxId = ( (uComplex > 0x200) && (uComplex < 0x600) ) ? true : false;
	featherId = ( (total_feather < 0x40) && (total_feather > 0x20) ) ? true : false;

	foundMvId =( (foundMv0_iir < 0x90) && (foundMv0_iir > 0x50) ) ? true : false;
	foundNonMv0Id =( (foundNonMv0_iir < 0x20) && (foundNonMv0_iir > 0x01) ) ? true : false;
	notFoundMvId = ( notFoundMv_iir < 0x2F ) ? true : false;

	if( ( gMv.h1XYsum > 10) && ( gMv.h2XYsum > 10)&&( gMv.h1XYdiff < 5)
		&& ( gMv.h2XYdiff < 5) &&( gMv.minMvXdir == 0x20) && (gMv.minMvYdir == 8))
	{
		gMvId = true;
	}
	else
	{
		gMvId = false;
	}

	if( mvComfirmId && colorId && cplxId && gMvId && motionId && featherId
		&&foundMvId && foundNonMv0Id && notFoundMvId )
	{
		L107 = true;
	}
	else
	{
		L107 = false;
	}
    //printf("isL107 == %d\n\n",L107);
	return L107;
}

MS_BOOL MDrv_SC_Agate_L153(void *pInstance, const MS_BOOL movingWithoutFeather, const MS_U32 colorSum, const MS_U32 uComplex, const  XC_Me1HisStates H2 )
{
    MS_BOOL notXmoving = FALSE;
    MS_BOOL YmovingNotFocus = FALSE;
    MS_BOOL CplxRange = FALSE;
    MS_BOOL withoutColor = FALSE;
    MS_BOOL L153 = FALSE;

	if( colorSum < 5 )
		withoutColor = true;
	else
		withoutColor = false;

	if((H2.minMvXdir == 8) && (H2.minMvXBin > 10))
		notXmoving = true;
	else
		notXmoving = false;

	if( H2.minMvYBin < 5)
		YmovingNotFocus = true;
	else
		YmovingNotFocus = false;

	if(( uComplex > 0x200 ) && ( uComplex < 0x300 ))
		CplxRange = true;
	else
		CplxRange = false;

	if(notXmoving && YmovingNotFocus && movingWithoutFeather && withoutColor && CplxRange)
	{
		L153 = true;
		//MApi_XC_W2BYTEMSK(REG_SC_BK2A_36_L, 0x0000, 0x000F);
	}
	else
	{
		L153 = false;
		//MApi_XC_W2BYTEMSK(REG_SC_BK2A_36_L, 0x0008, 0x000F);
	}
    //printf("L153 == %d \n\n",L153);
	return L153;
}

MS_BOOL MDrv_SC_AbihsotArtifactFeatherPattern(void *pInstance,const MS_BOOL Feather_sum_status, const MS_BOOL Feather_sum_status2,
					      const MS_U16 me1IsMv0, const MS_U16 me1IsNonMv0 )
{
	static MS_BOOL isAbihsot = false;
	static MS_BOOL isStill = false;
	static MS_BOOL isStillFeather = false;

	isStill = ( me1IsMv0 > me1IsNonMv0*26 ) ? true : false;

	isStillFeather = ( isStill && Feather_sum_status2 ) ? true : false;

	if( Feather_sum_status || isStillFeather )
	{
		isAbihsot = true;
	}
	else
	{
		isAbihsot = false;
	}

	return ( isAbihsot );
}

MS_BOOL MDrv_SC_SBlueShirt(void *pInstance,const MS_U16 nonMv0Comfirm, const MS_U16 mv0Comfirm, const MS_U16 mvNotFound ,
			   const MS_U16 mvNotFoundME2, const MS_U32 uComplex, const  XC_Me1HisStates H2, const XC_GMvStates gMv )
{
    MS_BOOL XmovingNotFocus = FALSE;
    MS_BOOL Cplx_Range = FALSE;
    MS_BOOL is_SBlueWoman = FALSE,isSBlueShirtWoman = FALSE;

	if((gMv.h2XYdiff < 0x10) && (nonMv0Comfirm < 0x55) &&
		(mv0Comfirm < 0x40)&& (mvNotFound < 0x100)&& (mvNotFoundME2 > 0x350) )
	{
		is_SBlueWoman = true;
	}
	else
	{
		is_SBlueWoman = false;
	}

	if( H2.minMvXBin < 0x15)
		XmovingNotFocus = true;
	else
		XmovingNotFocus = false;

	if(( uComplex > 2000 ) && ( uComplex < 3000 ))
		Cplx_Range = true;
	else
		Cplx_Range = false;

	if(XmovingNotFocus && is_SBlueWoman && Cplx_Range)
	{
		isSBlueShirtWoman = true;
	}
	else
	{
		isSBlueShirtWoman = false;
	}

	return isSBlueShirtWoman;
}


#define L107_FEATHER_GAIN_MAX 2
#define L107_FEATHER_GAIN_MIN 0
#define L107_CPLX_OFFSET_MAX 6
#define L107_CPLX_OFFSET_MIN 0
#define L107_CPLX_GAIN_MAX 15
#define L107_CPLX_GAIN_MIN 4
MS_BOOL MDrv_SC_A5_L107(void *pInstance,const MS_U32 IsVerticalMoving, const MS_U32 IsHorizontalMoving, const MS_U32 total_feather, const MS_U32 mvComfirm, const MS_U32 colorSum,const MS_U32 u32MotionValue, const MS_U32 uComplex )
{
	static MS_U32 reg_mcdi_feather_gain = L107_FEATHER_GAIN_MAX ;
	static MS_U32 reg_mcdi_cplx_offset = L107_CPLX_OFFSET_MAX ;
	static MS_U32 reg_mcdi_cplx_gain = L107_CPLX_GAIN_MIN ;
    MS_BOOL bL107_en = FALSE;

	if((IsVerticalMoving < 20) && (IsHorizontalMoving < 200) && (total_feather < 100)
		&& (mvComfirm > 200) && (colorSum > 200) && (u32MotionValue > 0x3500) && (uComplex < 0x700))
	{
		bL107_en = TRUE;

		if( reg_mcdi_feather_gain > L107_FEATHER_GAIN_MIN )
			reg_mcdi_feather_gain--;
		else
			reg_mcdi_feather_gain = L107_FEATHER_GAIN_MIN;

		if( reg_mcdi_cplx_offset > L107_CPLX_OFFSET_MIN )
			reg_mcdi_cplx_offset--;
		else
			reg_mcdi_cplx_offset = L107_CPLX_OFFSET_MIN;

		if( reg_mcdi_cplx_gain < L107_CPLX_GAIN_MAX )
			reg_mcdi_cplx_gain++;
		else
			reg_mcdi_cplx_gain = L107_CPLX_GAIN_MAX;
	}
	else
	{
		bL107_en = FALSE;

		if( reg_mcdi_feather_gain < L107_FEATHER_GAIN_MAX )
			reg_mcdi_feather_gain++;
		else
			reg_mcdi_feather_gain = L107_FEATHER_GAIN_MAX;

		if( reg_mcdi_cplx_offset < L107_CPLX_OFFSET_MAX )
			reg_mcdi_cplx_offset++;
		else
			reg_mcdi_cplx_offset = L107_CPLX_OFFSET_MAX;

		if( reg_mcdi_cplx_gain > L107_CPLX_GAIN_MIN )
			reg_mcdi_cplx_gain--;
		else
			reg_mcdi_cplx_gain = L107_CPLX_GAIN_MIN;
	}
	MApi_XC_W2BYTEMSK(REG_SC_BK2A_2D_L, (MS_U16)reg_mcdi_feather_gain<<12, 0xF000); // 2 to 0
	MApi_XC_W2BYTEMSK(REG_SC_BK2A_2E_L, (MS_U16)reg_mcdi_cplx_offset<<4, 0x00F0);// 6 to 0
	MApi_XC_W2BYTEMSK(REG_SC_BK2A_2E_L, (MS_U16)reg_mcdi_cplx_gain<<8, 0x0F00);// 4 to F

	return bL107_en;
}

MS_U32 MDrv_SC_Agate_ComplexSum(void *pInstance)
{
	static MS_U32 IsComplex_iir = 0xFFFF;
    MS_U32 IsComplex = 0;

	IsComplex = MApi_XC_R2BYTE(REG_SC_BK2A_78_L);

	IsComplex = (IsComplex_iir*3 + IsComplex ) >> 2;
	IsComplex_iir = IsComplex;

	return IsComplex_iir;
}

MS_U32 MDrv_SC_A5_KFC_IsVerticalMoving( void *pInstance )
{
    MS_U32 u32_mvc = 0, u32_mvu = 0, u32_mvd = 0;
    MS_U32 me_vertical = 0;
	static MS_U32 IsVerticalMoving_iir = 0;

	u32_mvc     = MApi_XC_R2BYTE(REG_SC_BK2A_7C_L);
	u32_mvu     = MApi_XC_R2BYTE(REG_SC_BK2A_7D_L);
	u32_mvd     = MApi_XC_R2BYTE(REG_SC_BK2A_7E_L);

	if ((u32_mvu > u32_mvc) && (u32_mvu > u32_mvd))
	{
		if( u32_mvu > (u32_mvc+u32_mvd)/2 )
			me_vertical = (u32_mvu - (u32_mvc+u32_mvd)/2);
		else
			me_vertical = 0;
	}
	else if((u32_mvd > u32_mvc) && (u32_mvd > u32_mvu) )
	{
		if( u32_mvd > (u32_mvc+u32_mvu)/2 )
			me_vertical = (u32_mvd - (u32_mvc+u32_mvu)/2);
		else
			me_vertical = 0;
	}
	else
	{
		me_vertical = 0;
	}

	if( me_vertical > 255 )
		me_vertical = 255;

	me_vertical = (IsVerticalMoving_iir*3 + me_vertical ) >> 2;
	IsVerticalMoving_iir = me_vertical;

	_bVerticalMovingForDHD = (((u32_mvu > 0x100) && (u32_mvu > u32_mvd*2) && (u32_mvu > u32_mvc))  //moving up
		||( (u32_mvd > 0x100) && (u32_mvd > u32_mvu*2) && (u32_mvd > u32_mvc))  //moving down
		||( ((u32_mvu + u32_mvd) > 0x100) && ((u32_mvu/8 + u32_mvd/8) > u32_mvc))); // u/d motion is much than mvc

	return me_vertical;
}

MS_U32 MDrv_SC_A5_ColorPixelSum(void *pInstance)
{
	MS_U32 colorSum;
	static MS_U32 colorSum_iir = 0;

	colorSum  = MApi_XC_R2BYTE(REG_SC_BK1A_6E_L);
	colorSum  = colorSum / 8;

	if( colorSum > 255 )
		colorSum = 255;

	colorSum = ((colorSum_iir*3 )+ colorSum ) >> 2;
	colorSum_iir = colorSum;

	return colorSum_iir;
}

MS_U32 MDrv_SC_Agate_TotalFeather(void *pInstance)
{
	MS_U32 mv0_feather, nonMv0_feather;
	MS_U32 total_feather;
	static MS_U32 total_feather_iir = 255;

	mv0_feather      = MApi_XC_R2BYTE(REG_SC_BK2A_77_L);
	nonMv0_feather   = MApi_XC_R2BYTE(REG_SC_BK2A_7B_L);

	total_feather = ( mv0_feather + nonMv0_feather ) / 4;

	if( total_feather > 255 )
		total_feather = 255;

	total_feather = ((total_feather_iir*3 )+ total_feather ) >> 2;
	total_feather_iir = total_feather;

	return total_feather_iir;
}

MS_U32 MDrv_SC_TotalFeather_Sum(void *pInstance)
{
	MS_U32 mv0_feather, nonMv0_feather;
	MS_U32 total_feather;
	static MS_U32 total_feather_iir = 0;

	mv0_feather      = MApi_XC_R2BYTE(REG_SC_BK2A_77_L);
	nonMv0_feather   = MApi_XC_R2BYTE(REG_SC_BK2A_7B_L);

	total_feather =  mv0_feather + nonMv0_feather;

	total_feather = ((total_feather_iir*3 )+ total_feather ) >> 2;
	total_feather_iir = total_feather;

	return total_feather_iir;
}

#define OOSRF_Gain          4
#define OOSRF_Power        2
MS_U32 MDrv_SC_McInSearchRangeUseFeather( void *pInstance )
{
    MS_U32 before_MC_feather = 0, after_MC_feather = 0;
    MS_U32 MC_OOSRF = 0;
	static MS_U32 Prev_before_MC_feather = 0;
	static MS_U32 Prev_after_MC_feather = 0;

	before_MC_feather = MApi_XC_R2BYTE(REG_SC_BK2A_77_L) ;//mv0_feather
	after_MC_feather = MApi_XC_R2BYTE(REG_SC_BK2A_7B_L);//nonMv0_feather

	before_MC_feather = (Prev_before_MC_feather*3 + before_MC_feather ) >> 2;
	Prev_before_MC_feather = before_MC_feather;

	after_MC_feather = (Prev_after_MC_feather*3 + after_MC_feather ) >> 2;
	Prev_after_MC_feather = after_MC_feather;

	if( before_MC_feather > (after_MC_feather*OOSRF_Gain) >> OOSRF_Power  )//mv0_feather >> nonMv0_feather: MCDI is correct, therefore in search range
		MC_OOSRF = before_MC_feather - ((after_MC_feather*OOSRF_Gain) >> OOSRF_Power) ;
	else
		MC_OOSRF = 0;

	return MC_OOSRF;  // "Big": in seach range "Small": out of seach range
}

#define TOTAL_MV_TIME 10
MS_U32 MDrv_SC_Agate_TotalMvComfirm(void *pInstance)
{
    MS_U32 mv0 = 0, nonMv0 = 0, notFound = 0;
    MS_U32 total_mv = 0, mvComfirm = 0;
	static MS_U32 mvComfirm_iir = 0;

	mv0      = MApi_XC_R2BYTE(REG_SC_BK17_48_L);
	nonMv0   = MApi_XC_R2BYTE(REG_SC_BK17_49_L);
	notFound   = MApi_XC_R2BYTE(REG_SC_BK17_4A_L);

	total_mv = mv0 + nonMv0;

	if( total_mv > (notFound*TOTAL_MV_TIME))
		mvComfirm =   total_mv - notFound*TOTAL_MV_TIME;
	else
		mvComfirm = 0;

	if( mvComfirm > 255 )
		mvComfirm = 255;

	mvComfirm = ((mvComfirm_iir*3 )+ mvComfirm ) >> 2;
	mvComfirm_iir = mvComfirm;

	return mvComfirm_iir;
}

MS_U16 MDrv_SC_Agate_ME1_nonMv0Comfirm(void *pInstance)
{
    MS_U16 nonmv0Comfirm = 0;
	static MS_U16 nonmv0Comfirm_iir = 0;

	nonmv0Comfirm = MApi_XC_R2BYTE(REG_SC_BK17_49_L);

	nonmv0Comfirm = ((nonmv0Comfirm_iir*3 )+ nonmv0Comfirm ) >> 2;
	nonmv0Comfirm_iir = nonmv0Comfirm;

	return nonmv0Comfirm_iir;
}

MS_U16 MDrv_SC_Agate_ME1_nonMv0Comfirm_noIIR(void *pInstance)
{
    MS_U16 nonmv0Comfirm = 0;

	nonmv0Comfirm = MApi_XC_R2BYTE(REG_SC_BK17_49_L);
	return nonmv0Comfirm;
}

MS_U16 MDrv_SC_Agate_ME1_mv0Comfirm_noIIR(void *pInstance)
{
	MS_U16 mv0Comfirm = 0;

	mv0Comfirm = MApi_XC_R2BYTE(REG_SC_BK17_48_L);
	return mv0Comfirm;
}

MS_U16 MDrv_SC_Agate_ME1_mvNotFound(void *pInstance)
{
    MS_U16 mvNotFound = 0;
	static MS_U16 mvNotFound_iir = 0;

	mvNotFound = MApi_XC_R2BYTE(REG_SC_BK17_4A_L);

	mvNotFound = ((mvNotFound_iir*3 )+ mvNotFound ) >> 2;
	mvNotFound_iir = mvNotFound;

	return mvNotFound_iir;
}

#if 0
MS_U16 MDrv_SC_Agate_ME1_mvNotFound_noIIR(void)
{
    MS_U16 mvNotFound_noIIR = 0;

	mvNotFound_noIIR = MApi_XC_R2BYTE(REG_SC_BK17_4A_L);

	return mvNotFound_noIIR;
}
#endif

MS_U16 MDrv_SC_Agate_ME1_mv0Comfirm(void *pInstance)
{
    MS_U16 mv0Comfirm = 0;
	static MS_U16 mv0Comfirm_iir = 0;

	mv0Comfirm = MApi_XC_R2BYTE(REG_SC_BK17_48_L);

	mv0Comfirm = ((mv0Comfirm_iir*3 )+ mv0Comfirm ) >> 2;
	mv0Comfirm_iir = mv0Comfirm;

	return mv0Comfirm_iir;
}

MS_U16 MDrv_SC_Agate_ME2_nonMv0Comfirm(void *pInstance)
{
    MS_U16 nonmv0Comfirm = 0;
    static MS_U16 nonmv0Comfirm_iir = 0;

    nonmv0Comfirm = MApi_XC_R2BYTE(REG_SC_BK17_4C_L);

    nonmv0Comfirm = ((nonmv0Comfirm_iir*3 )+ nonmv0Comfirm ) >> 2;
    nonmv0Comfirm_iir = nonmv0Comfirm;

    return nonmv0Comfirm_iir;
}

MS_U16 MDrv_SC_Agate_ME2_mv0Comfirm(void *pInstance)
{
    MS_U16 mv0Comfirm = 0;
    static MS_U16 mv0Comfirm_iir = 0;

    mv0Comfirm = MApi_XC_R2BYTE(REG_SC_BK17_4B_L);

    mv0Comfirm = ((mv0Comfirm_iir*3 )+ mv0Comfirm ) >> 2;
    mv0Comfirm_iir = mv0Comfirm;

    return mv0Comfirm_iir;
}

MS_U16 MDrv_SC_Agate_ME2_mvNotFound(void *pInstance)
{
	MS_U16 mvNotFound = 0;
	static MS_U16 mvNotFound_iir = 0;

	mvNotFound = MApi_XC_R2BYTE(REG_SC_BK17_4D_L);

	mvNotFound = ((mvNotFound_iir*3 )+ mvNotFound ) >> 2;
	mvNotFound_iir = mvNotFound;

	return mvNotFound_iir;
}

MS_BOOL MDrv_SC_McOutOfSearchRange(void *pInstance, const MS_U16 mvNotFound, const MS_U16 mv0Comfirm, const MS_U16 nonMv0Comfirm, const MS_U16 isFeatherAndMotion, const XC_GMvStates gMv )
{

	MS_BOOL isBigGmvDiff = (( abs(32 - gMv.minMvXdir) > 15 ) || ( abs(8 - gMv.minMvYdir) > 4 ) ) ? true : false;
    MS_BOOL  isOutOfSearchRange = 0;
	static MS_U8 isCnt = 0;

	int notFound = (int)mvNotFound;
	int mv0 = (int) mv0Comfirm;
	int nonmv0 = (int) nonMv0Comfirm;

	/* where are nonMv0Comfirm ??? >_< */
    isOutOfSearchRange = ( (notFound > ((mv0+nonmv0)*8)) && ( (notFound) > 0x80 ) ) ? true : false;

	if( isOutOfSearchRange && (isFeatherAndMotion > 5) && isBigGmvDiff )
	{
		if( isCnt < 255 )
			isCnt++;
	}
	else
	{
		if( isCnt > 0 )
			isCnt--;
	}

	return ( isCnt > 15 ) ? true : false;
}

/*Added for VG846 Sparse Pattern to avoid forcing MCDi function */
MS_BOOL MDrv_SC_ukoSabihS_SparsePattern( void *pInstance,const MS_U16 mvNotFound, const MS_U16 mvNotFoundME2, const MS_U16 mv0Comfirm, const MS_U16 nonMv0Comfirm, const MS_U16 mv0ComfirmME2, const MS_U16 nonMv0ComfirmME2, const MS_BOOL isHDSrc )
{

    MS_BOOL  isSparse = 0;

    int notFound = (int)mvNotFound;
    int notFoundME2 = (int)mvNotFoundME2;
    int MVConfirm = (int) mv0Comfirm;
    int MVConfirmME2 = (int) mv0ComfirmME2;
    int nonmv0 = (int) nonMv0Comfirm;
    int nonmv0ME2 = (int) nonMv0ComfirmME2;
    int notFoundTH = 0;
    int notFoundME2TH = 0;

    /*if(isHDSrc)
    {
        isSparse = ( notFound > 0x200 ) && ( notFoundME2 > 0xF00 ) ? true : false;
    }
    else
    {
        isSparse = ( notFound > 0x40 ) && ( notFoundME2 > 0x800 ) ? true : false;
    }*/

    if(isHDSrc)
    {
    	 notFoundTH = (0x200* normalize_factor)/256;
   	 notFoundME2TH = (0xF00* normalize_factor)/256;
    }
    else
    {
 	   notFoundTH = (0x40* normalize_factor)/256;
 	   notFoundME2TH = (0x800* normalize_factor)/256;
    }
    isSparse = ( notFound > notFoundTH ) && ( notFoundME2 > notFoundME2TH ) ? true : false;

    if (isSparse && (notFound >(MVConfirm+nonmv0))&& (notFoundME2 >(MVConfirmME2+nonmv0ME2)))
    {
        	return true;
    }
    else
    {
            return false;
     }

}


MS_U16 MDrv_SC_Agate_FeatherAndMotion(void *pInstance)
{
    MS_U16 FAM = 0;
	FAM = MApi_XC_R2BYTE(REG_SC_BK2A_7F_L); //feather and motion pixels
	return FAM;
}

MS_BOOL MDrv_SC_ukoSabihS_HBurstMoving(void *pInstance, const MS_U32 _hpfSdCntReport )
{
    MS_U32 mv0_feather = 0;
    MS_BOOL isHBurst_act = FALSE;
	static MS_U8 u8HBurstConfirmCnt = 0;

	mv0_feather = MApi_XC_R2BYTE(REG_SC_BK2A_77_L);

	isHBurst_act = ( ((mv0_feather) > (_hpfSdCntReport*100)) && ( _hpfSdCntReport < 0x10 ) ) ? true : false;

	if(isHBurst_act)
	{
		if(u8HBurstConfirmCnt <= 0x20)
		{
			u8HBurstConfirmCnt++;
		}
	}
	else
	{
		u8HBurstConfirmCnt = 0;
	}

	if(u8HBurstConfirmCnt > 10)
	{
		return true;
	}
	else
	{
		return false;
	}
}

MS_BOOL MDrv_SC_ukoSabihS_ArtifactPattern(void *pInstance, const MS_U32 _hpfHdTolerantReport,const MS_BOOL isHDSrc )
{
    MS_BOOL isArtifactPattern_act = 0;
	static MS_U8 u8ArtifactPatternConfirmCnt = 0;

	if(isHDSrc)
	{
		isArtifactPattern_act = ( _hpfHdTolerantReport > 0x18000 ) ? true : false;
	}
	else
	{
		isArtifactPattern_act = ( _hpfHdTolerantReport > 0x4000 ) ? true : false;
	}

	if(isArtifactPattern_act)
	{
		if(u8ArtifactPatternConfirmCnt < 20)
		{
			u8ArtifactPatternConfirmCnt++;
		}
	}
	else
	{
		if(u8ArtifactPatternConfirmCnt > 0)
		{
			u8ArtifactPatternConfirmCnt--;
		}
	}

	if(u8ArtifactPatternConfirmCnt > 10)
	{
		return true;
	}
	else
	{
		return false;
	}

	//return isHBurstMoving;
}

MS_BOOL MDrv_SC_ukoSabihS_noLumaCplx( void *pInstance )
{
    MS_U8 reg_main_max_pixel_sat = 0;
    MS_U8 reg_main_min_pixel_sat = 0;

    MS_U16 reg_feather_and_motion_pixel = 0;

	static MS_U8 confirmCnt = 0;

	reg_main_max_pixel_sat = (MS_U8)( MApi_XC_R2BYTE(REG_SC_BK1A_61_L) & 0x00FF );
	reg_main_min_pixel_sat = (MS_U8)( MApi_XC_R2BYTE(REG_SC_BK1A_61_L) & 0xFF00 ) >> 8;
	reg_feather_and_motion_pixel = MApi_XC_R2BYTE(REG_SC_BK2A_78_L);

	if( ( reg_main_min_pixel_sat == 0 ) &&
		( reg_main_max_pixel_sat == 0 ) &&
		( reg_feather_and_motion_pixel > 0x5000 ) )
	{
		if( confirmCnt < 30 )
			confirmCnt++;
	}
	else
		confirmCnt = 0;

	return ( confirmCnt > 20 );
}

MS_BOOL MDrv_SC_ukoSabihS_HBurst(void *pInstance, const MS_U32 _hpfSdCntReport, const MS_U32 uComplex )
{
    MS_BOOL isHBurst = FALSE;

	isHBurst = ( ((uComplex) > (_hpfSdCntReport*100)) && ( _hpfSdCntReport < 0x10 ) ) ? true : false;

	return isHBurst;
}

MS_BOOL MDrv_SC_Agate_ME1Still(void *pInstance)
{
    MS_U32 mv0 = 0, nonMv0 = 0;
    MS_BOOL isStill = FALSE;

	static MS_U32 mv0_iir = 0;
	static MS_U32 nonMv0_iir = 0;

	mv0      = MApi_XC_R2BYTE(REG_SC_BK17_48_L);
	nonMv0   = MApi_XC_R2BYTE(REG_SC_BK17_49_L);

	if( mv0_iir > (nonMv0_iir*6) )
		isStill = true;
	else
		isStill = false;

	mv0 = ((mv0_iir*3 )+ mv0 ) >> 2;
	mv0_iir = mv0;

	nonMv0 = ((nonMv0_iir*3 )+ nonMv0 ) >> 2;
	nonMv0_iir = nonMv0;

	return isStill;
}

MS_BOOL MDrv_SC_Agate_MovingWithoutFeather(void *pInstance)
{
    MS_U32 mv0_feather = 0, nonMv0_feather = 0;
    MS_BOOL isMovingWithoutFeather = FALSE;

	static MS_U32 mv0_feather_iir = 0;
	static MS_U32 nonMv0_feather_iir = 0;

	mv0_feather      = MApi_XC_R2BYTE(REG_SC_BK2A_77_L);
	nonMv0_feather   = MApi_XC_R2BYTE(REG_SC_BK2A_7B_L);

	if( mv0_feather_iir > (nonMv0_feather_iir*60) )
		isMovingWithoutFeather = true;
	else
		isMovingWithoutFeather = false;

	mv0_feather = ((mv0_feather_iir*3 )+ mv0_feather ) >> 2;
	mv0_feather_iir = mv0_feather;

	nonMv0_feather = ((nonMv0_feather_iir*3 )+ nonMv0_feather ) >> 2;
	nonMv0_feather_iir = nonMv0_feather;

	return isMovingWithoutFeather;
}

/******************************************************************************/
///Extend MCNR Driver
/******************************************************************************/
void MDrv_SC_Extend_UCNR_driver(void *pInstance)
{
    MS_U32 u32_is_mv0 = 0, u32_non_mv0 = 0, u32_mvNotFound = 0, u32_mvc = 0, u32_mvu = 0, u32_mvd = 0;
    MS_BOOL me_not_confirm = FALSE, me_vertical = FALSE, still_pattern = FALSE;
	static MS_U8 u8notConfirmCnt;
//	static MS_U32 last_u32_mvNotFound;

	u32_is_mv0  = MApi_XC_R2BYTE(REG_SC_BK2A_78_L);
	u32_non_mv0 = MApi_XC_R2BYTE(REG_SC_BK2A_79_L);
	u32_mvNotFound = MApi_XC_R2BYTE(REG_SC_BK2A_7A_L);
	u32_mvc     = MApi_XC_R2BYTE(REG_SC_BK2A_7C_L);
	u32_mvu     = MApi_XC_R2BYTE(REG_SC_BK2A_7D_L);
	u32_mvd     = MApi_XC_R2BYTE(REG_SC_BK2A_7E_L);

	/////////////////////////////////////////////////////////////////
	//  NR Patch 1
	/////////////////////////////////////////////////////////////////

	me_not_confirm = (u32_is_mv0 < 0x0200) && (u32_non_mv0 < 0x0200) ;
	me_vertical    = ((u32_mvu > u32_mvc) && (u32_mvu > u32_mvd) && (u32_mvu > 0x200))
		||((u32_mvd > u32_mvc) && (u32_mvd > u32_mvu) && (u32_mvd > 0x200));

	still_pattern = (u32_is_mv0 < 0x0020) && (u32_non_mv0 < 0x0020) && (u32_mvNotFound < 0x0020);

	if(!still_pattern)
	{
		if(false == me_not_confirm)
		{
			if(u8notConfirmCnt > 0)
			{
				u8notConfirmCnt--;
			}
		}
		else
		{
			if(u8notConfirmCnt <= 0x40)
			{
				u8notConfirmCnt++;
			}
		}
	}

	if ( (u8notConfirmCnt > 0x30)|| me_vertical ) // low nr strength
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_08_L, (MS_U16)0xAAAA, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_09_L, (MS_U16)0xAAAA, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_0A_L, (MS_U16)0xAAAA, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_0B_L, (MS_U16)0xAAAA, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_0C_L, (MS_U16)0xAAAA, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_0D_L, (MS_U16)0xAA98, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_0E_L, (MS_U16)0x7654, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_0F_L, (MS_U16)0x3210, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_1B_L, (MS_U16)0x0064, 0x00FF); // moving picture, increase random motion
	}
	else
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_08_L, (MS_U16)0xAAAA, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_09_L, (MS_U16)0xAAAA, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_0A_L, (MS_U16)0xAAAA, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_0B_L, (MS_U16)0xAAAA, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_0C_L, (MS_U16)0xAAAA, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_0D_L, (MS_U16)0xAA98, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_0E_L, (MS_U16)0x7654, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_0F_L, (MS_U16)0x3210, 0xFFFF);
	}

	if ( (u32_is_mv0 > 0x200) && ((u32_is_mv0/2) > u32_non_mv0) && ((u32_is_mv0/2) > u32_mvNotFound) )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_1B_L, (MS_U16)0x0044, 0x00FF); // still picture, reduce random motion
	}
	else if(u32_non_mv0 > u32_is_mv0)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_1B_L, (MS_U16)0x0044, 0x00FF); // moving picture, increase random motion
	}
	else
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_1B_L, (MS_U16)0x0044, 0x00FF); // default random motion
	}

	//last_u32_mvNotFound = u32_mvNotFound;
}

/******************************************************************************/
///Noise Estimation Driver
/******************************************************************************/
MS_U8 MDrv_SC_GetNoiseLevel(void *pInstance, MS_U8 *estNoise, const XC_LetterBoxState *letterBoxState, const XC_ApiStatus *stXCStatus, const MS_BOOL bInterlace, const MS_BOOL isOpFrameChanged )
{
	const int defaultnoise = 35;
	static int noiseIIR = 35;
	static int noisehistory[5] = {0,0,0,0,0};
	static MS_BOOL long_noisehistory[11] = {0,0,0,0,0,0,0,0,0,0,0};
	static int safenoise = 0;
	static int resetcount = 0;
	MS_U8 reg_noise_confirm_level = 2;
	static MS_BOOL is_ne_set = false;

	int avgpixeldiff = defaultnoise;
	int NE_histogram[25];
	int MV0count = 0;

	if( isOpFrameChanged )
	{
		MS_U16 height = bInterlace ? stXCStatus->ScaledCropWin.height/2 : stXCStatus->ScaledCropWin.height;
		MS_U16 width = stXCStatus->ScaledCropWin.width;

		MS_U16 hSt = (letterBoxState->lbCntL + 49 )/ 10;
		MS_U16 hEnd = ( __max( width - letterBoxState->lbCntR - 50, 0 ) ) / 10;

		MS_U16 vSt = (letterBoxState->lbCntT + 29 )/ 6;
		MS_U16 vEnd = ( __max( height - letterBoxState->lbCntB - 30, 0 ) ) / 6;

		MApi_XC_W2BYTE(REG_SC_BK39_0E_L, (hEnd << 8) | hSt );
		MApi_XC_W2BYTE(REG_SC_BK39_0F_L, (vEnd << 8) | vSt );

		MS_U16 total_block_num=__max((vEnd+1)/4-(vSt+3)/4,0)*__max((hEnd+1)/4-(hSt+3)/4,0);


		if( !is_ne_set )
		{
			MApi_XC_W2BYTEMSK(REG_SC_BK39_08_L, 0x0001, 0x0001);
			is_ne_set = true;

			MApi_XC_W2BYTEMSK(REG_SC_BK39_0A_L, 0x0000, 0x0100);
			MApi_XC_W2BYTEMSK(REG_SC_BK39_0D_L, 0x0001, 0x0001); // active window en
		}
		else if (MApi_XC_R2BYTEMSK(REG_SC_BK39_08_L, 0x0080)!=0)//ready
		{
			NE_histogram[0]=MApi_XC_R2BYTEMSK(REG_SC_BK39_10_L,0x07FF);
			NE_histogram[1]=MApi_XC_R2BYTEMSK(REG_SC_BK39_11_L,0x07FF);
			NE_histogram[2]=MApi_XC_R2BYTEMSK(REG_SC_BK39_12_L,0x07FF);
			NE_histogram[3]=MApi_XC_R2BYTEMSK(REG_SC_BK39_13_L,0x07FF);
			NE_histogram[4]=MApi_XC_R2BYTEMSK(REG_SC_BK39_14_L,0x07FF);
			NE_histogram[5]=MApi_XC_R2BYTEMSK(REG_SC_BK39_15_L,0x07FF);
			NE_histogram[6]=MApi_XC_R2BYTEMSK(REG_SC_BK39_16_L,0x07FF);
			NE_histogram[7]=MApi_XC_R2BYTEMSK(REG_SC_BK39_17_L,0x07FF);
			NE_histogram[8]=MApi_XC_R2BYTEMSK(REG_SC_BK39_18_L,0x07FF);
			NE_histogram[9]=MApi_XC_R2BYTEMSK(REG_SC_BK39_19_L,0x07FF);
			NE_histogram[10]=MApi_XC_R2BYTEMSK(REG_SC_BK39_1A_L,0x07FF);
			NE_histogram[11]=MApi_XC_R2BYTEMSK(REG_SC_BK39_1B_L,0x07FF);
			NE_histogram[12]=MApi_XC_R2BYTEMSK(REG_SC_BK39_1C_L,0x07FF);
			NE_histogram[13]=MApi_XC_R2BYTEMSK(REG_SC_BK39_1D_L,0x07FF);
			NE_histogram[14]=MApi_XC_R2BYTEMSK(REG_SC_BK39_1E_L,0x07FF);
			NE_histogram[15]=MApi_XC_R2BYTEMSK(REG_SC_BK39_1F_L,0x07FF);
			NE_histogram[16]=MApi_XC_R2BYTEMSK(REG_SC_BK39_20_L,0x07FF);
			NE_histogram[17]=MApi_XC_R2BYTEMSK(REG_SC_BK39_21_L,0x07FF);
			NE_histogram[18]=MApi_XC_R2BYTEMSK(REG_SC_BK39_22_L,0x07FF);
			NE_histogram[19]=MApi_XC_R2BYTEMSK(REG_SC_BK39_23_L,0x07FF);
			NE_histogram[20]=MApi_XC_R2BYTEMSK(REG_SC_BK39_24_L,0x07FF);
			NE_histogram[21]=MApi_XC_R2BYTEMSK(REG_SC_BK39_25_L,0x07FF);
			NE_histogram[22]=MApi_XC_R2BYTEMSK(REG_SC_BK39_26_L,0x07FF);
			NE_histogram[23]=MApi_XC_R2BYTEMSK(REG_SC_BK39_27_L,0x07FF);
			NE_histogram[24]=MApi_XC_R2BYTEMSK(REG_SC_BK39_28_L,0x07FF);

			int i;
			for (i = 4; i > 0; i-- )
				noisehistory[i]=noisehistory[i-1];
			for (i = 10; i > 0; i-- )
				long_noisehistory[i]=long_noisehistory[i-1];
			for (i=0;i<25;i++)
				MV0count+=NE_histogram[i];
			if (MV0count<total_block_num/12 || total_block_num<24)
			{
				if (safenoise>0)
				{
					avgpixeldiff=safenoise;
					resetcount++;
					reg_noise_confirm_level=8;
				}
				else
				{
					avgpixeldiff=noiseIIR;
					reg_noise_confirm_level=0;
				}
				noisehistory[0]=0;
				long_noisehistory[0]=0;
			}
			else
			{
				int maxhis=1;
				int maxval=0;
				for (i=0;i<24;i++)
				{
					int curva;
					if (i==0)
						curva=2*NE_histogram[i]+NE_histogram[i+1];
					else if (i==23)
						curva=2*NE_histogram[i]+NE_histogram[i-1];
					else
						curva=NE_histogram[i-1]+2*NE_histogram[i]+NE_histogram[i+1];

					if (curva>maxval)
					{
						maxval=curva;
						maxhis=i;
					}
				}
				int thre1=4;
				int thre2=13;
				if (maxhis>12)
				{
					thre1=3;
					thre2=10;
				}
				if ((maxval>total_block_num/10 && (maxval-NE_histogram[maxhis])*10/MV0count>=thre1 && NE_histogram[maxhis] * 100 / MV0count>thre2))// || MV0count<total_block_num/24)
				{
					noisehistory[0]=10*maxhis+5;
					long_noisehistory[0]=1;
					int nonzerocount=0;
					int historytotal=0;
					int firstnoise=0;
					int long_nonzerocount=0;
					for ( i=0;i<5;i++)
					{
						historytotal=historytotal+noisehistory[i];
						if (noisehistory[i]>0)
						{
							nonzerocount++;
							firstnoise=noisehistory[i];
						}
					}
					for ( i=0;i<11;i++)
						long_nonzerocount+=long_noisehistory[i];
					if (nonzerocount>=3 && (maxhis<=3 || long_nonzerocount>=7))
					{
						int removefirstavg=(historytotal-firstnoise)/(nonzerocount-1);
						if ((removefirstavg+20)<firstnoise)
							safenoise=removefirstavg;
						else
							safenoise=historytotal/nonzerocount;
						resetcount=0;
						avgpixeldiff=noisehistory[0];
						reg_noise_confirm_level=8;
					}
					else if (safenoise>0)
					{
						avgpixeldiff=safenoise;
						resetcount++;
						reg_noise_confirm_level=8;
					}
					else
					{
						avgpixeldiff=noisehistory[0];
						reg_noise_confirm_level=1;
					}
				}
				else
				{
					noisehistory[0]=0;
					long_noisehistory[0]=0;
					if (safenoise>0)
					{
						avgpixeldiff=safenoise;
						resetcount++;
						reg_noise_confirm_level=8;
					}
					else
					{
						avgpixeldiff=10*maxhis+5;
						reg_noise_confirm_level=1;
					}
				}
			}
			if (resetcount>8)
			{
				safenoise=0;
				resetcount=0;
			}
			int tmpcount=0;
			int prenoise=0;
			for ( i = 0; i < 5; i++ )
			{
				if (noisehistory[i]<250 && noisehistory[i]>0)
				{
					tmpcount++;
					prenoise=prenoise+noisehistory[i];
				}
			}
			if (tmpcount>0)
				prenoise=prenoise/tmpcount;
			int reg_ne_mv0cost_th =2*prenoise/10;
			reg_ne_mv0cost_th = reg_ne_mv0cost_th > 3 ? reg_ne_mv0cost_th : 3;
			if (tmpcount==0)
				reg_ne_mv0cost_th =60;
			MApi_XC_W2BYTEMSK(REG_SC_BK39_0A_L, reg_ne_mv0cost_th, 0x003F);

			if (reg_noise_confirm_level==8)
				noiseIIR = (noiseIIR*3+avgpixeldiff) >> 2;

			MApi_XC_W2BYTEMSK(REG_SC_BK39_08_L, 0x0001, 0x0001);
			is_ne_set = true;
		}
	}

	*estNoise = (MS_U8)( noiseIIR <= 255 ? noiseIIR : 255 );

	return reg_noise_confirm_level;
}

/******************************************************************************/
///UCNR Driver
/******************************************************************************/
MS_BOOL Hal_PQ_LoadMCNRLutQmapSetting()
{
    //UCNR LUT
#if MCNR_LUT_LOAD_FROM_QMAP
    m_stMCNRLutQmapSetting.mcnrLut[0] = MApi_XC_R2BYTE(REG_SC_BK2A_08_L);
    m_stMCNRLutQmapSetting.mcnrLut[1] = MApi_XC_R2BYTE(REG_SC_BK2A_09_L);
    m_stMCNRLutQmapSetting.mcnrLut[2] = MApi_XC_R2BYTE(REG_SC_BK2A_0A_L);
    m_stMCNRLutQmapSetting.mcnrLut[3] = MApi_XC_R2BYTE(REG_SC_BK2A_0B_L);
    m_stMCNRLutQmapSetting.mcnrLut[4] = MApi_XC_R2BYTE(REG_SC_BK2A_0C_L);
    m_stMCNRLutQmapSetting.mcnrLut[5] = MApi_XC_R2BYTE(REG_SC_BK2A_0D_L);
    m_stMCNRLutQmapSetting.mcnrLut[6] = MApi_XC_R2BYTE(REG_SC_BK2A_0E_L);
    m_stMCNRLutQmapSetting.mcnrLut[7] = MApi_XC_R2BYTE(REG_SC_BK2A_0F_L);
#else
    m_stMCNRLutQmapSetting.mcnrLut[0] = 0xFFEE;
    m_stMCNRLutQmapSetting.mcnrLut[1] = 0xDDCC;
    m_stMCNRLutQmapSetting.mcnrLut[2] = 0xBBAA;
    m_stMCNRLutQmapSetting.mcnrLut[3] = 0x9988;
    m_stMCNRLutQmapSetting.mcnrLut[4] = 0x7766;
    m_stMCNRLutQmapSetting.mcnrLut[5] = 0x5544;
    m_stMCNRLutQmapSetting.mcnrLut[6] = 0x3322;
    m_stMCNRLutQmapSetting.mcnrLut[7] = 0x1100;
#endif

    //PDNR LUT
#if PDNR_LUT_LOAD_FROM_QMAP
    m_stMCNRLutQmapSetting.pdnrlowLut[0] = MApi_XC_R2BYTE(REG_SC_BK2A_1C_L);
    m_stMCNRLutQmapSetting.pdnrlowLut[1] = MApi_XC_R2BYTE(REG_SC_BK2A_1D_L);
    m_stMCNRLutQmapSetting.pdnrlowLut[2] = MApi_XC_R2BYTE(REG_SC_BK2A_1E_L);
    m_stMCNRLutQmapSetting.pdnrlowLut[3] = MApi_XC_R2BYTE(REG_SC_BK2A_1F_L);
    m_stMCNRLutQmapSetting.pdnrlow2Lut[0] = MApi_XC_R2BYTE(REG_SC_BK2A_20_L);
    m_stMCNRLutQmapSetting.pdnrlow2Lut[1] = MApi_XC_R2BYTE(REG_SC_BK2A_21_L);
    m_stMCNRLutQmapSetting.pdnrlow2Lut[2] = MApi_XC_R2BYTE(REG_SC_BK2A_22_L);
    m_stMCNRLutQmapSetting.pdnrlow2Lut[3] = MApi_XC_R2BYTE(REG_SC_BK2A_23_L);
    m_stMCNRLutQmapSetting.pdnrhighLut[0] = MApi_XC_R2BYTE(REG_SC_BK2A_24_L);
    m_stMCNRLutQmapSetting.pdnrhighLut[1] = MApi_XC_R2BYTE(REG_SC_BK2A_25_L);
    m_stMCNRLutQmapSetting.pdnrhighLut[2] = MApi_XC_R2BYTE(REG_SC_BK2A_26_L);
    m_stMCNRLutQmapSetting.pdnrhighLut[3] = MApi_XC_R2BYTE(REG_SC_BK2A_27_L);
#else
    m_stMCNRLutQmapSetting.pdnrlowLut[0] = 0x8888;
    m_stMCNRLutQmapSetting.pdnrlowLut[1] = 0x8888;
    m_stMCNRLutQmapSetting.pdnrlowLut[2] = 0x7654;
    m_stMCNRLutQmapSetting.pdnrlowLut[3] = 0x3210;
    m_stMCNRLutQmapSetting.pdnrlow2Lut[0] = 0x2100;
    m_stMCNRLutQmapSetting.pdnrlow2Lut[1] = 0x0000;
    m_stMCNRLutQmapSetting.pdnrlow2Lut[2] = 0x0000;
    m_stMCNRLutQmapSetting.pdnrlow2Lut[3] = 0x0000;
    if(_bIsHDSource)
    {
        m_stMCNRLutQmapSetting.pdnrhighLut[0] = 0x8888;
        m_stMCNRLutQmapSetting.pdnrhighLut[1] = 0x8888;
        m_stMCNRLutQmapSetting.pdnrhighLut[2] = 0x8887;
        m_stMCNRLutQmapSetting.pdnrhighLut[3] = 0x6420;
    }
    else
    {
        m_stMCNRLutQmapSetting.pdnrhighLut[0] = 0x8876;
        m_stMCNRLutQmapSetting.pdnrhighLut[1] = 0x5432;
        m_stMCNRLutQmapSetting.pdnrhighLut[2] = 0x1000;
        m_stMCNRLutQmapSetting.pdnrhighLut[3] = 0x0000;
    }
#endif

    return true;
}

MS_U16 MDrv_SC_NR_LUT_Aux(void *pInstance, MS_U16 nr_lut, MS_U8 sc)
{
	sc = sc < 16 ? sc : 16;

    MS_U8 h0 = (nr_lut >>  0) & 0xF;
    MS_U8 h1 = (nr_lut >>  4) & 0xF;
    MS_U8 h2 = (nr_lut >>  8) & 0xF;
    MS_U8 h3 = (nr_lut >> 12) & 0xF;

    XC_ApiStatus DrvStatus;
	MApi_XC_GetStatus(&DrvStatus, MAIN_WINDOW);
	if ((!DrvStatus.bInterlace) && (DrvStatus.u16InputVFreq < 300)) //reduce gain value if 24p
	{
		h0 = ((h0 * sc)/3) >> 4;
		h1 = ((h1 * sc)/3) >> 4;
		h2 = ((h2 * sc)/3) >> 4;
		h3 = ((h3 * sc)/3) >> 4;
	}
	else
	{
		h0 = (h0 * sc) >> 4;
	    h1 = (h1 * sc) >> 4;
	    h2 = (h2 * sc) >> 4;
	    h3 = (h3 * sc) >> 4;
	}

    return (h3 << 12) | (h2 << 8) | (h1 << 4) | (h0 << 0);
}

MS_U16 MDrv_SC_NR_LUT_Offset_Aux(void *pInstance, MS_U16 nr_lut, MS_U8 sc)
{
	sc = sc < 0xF ? sc : 0xF;

    MS_U8 h0 = (nr_lut >>  0) & 0xF;
    MS_U8 h1 = (nr_lut >>  4) & 0xF;
    MS_U8 h2 = (nr_lut >>  8) & 0xF;
    MS_U8 h3 = (nr_lut >> 12) & 0xF;

    h0 = (h0 > sc) ? h0 - sc : 0;
    h1 = (h1 > sc) ? h1 - sc : 0;
    h2 = (h2 > sc) ? h2 - sc : 0;
    h3 = (h3 > sc) ? h3 - sc : 0;

    return (h3 << 12) | (h2 << 8) | (h1 << 4) | (h0 << 0);
}

MS_U16 MDrv_SC_Luma_PK_LUT_Aux(void *pInstance, const MS_U8 table[], MS_U8 sc)
{
	sc = sc < 16 ? sc : 16;

	MS_U8 h0 = ( 8 * (16-sc) + table[0] * sc + 8) >> 4;
	MS_U8 h1 = ( 8 * (16-sc) + table[1] * sc + 8) >> 4;
	MS_U8 h2 = ( 8 * (16-sc) + table[2] * sc + 8) >> 4;
	MS_U8 h3 = ( 8 * (16-sc) + table[3] * sc + 8) >> 4;

	return (h0 << 12) | (h1 << 8) | (h2 << 4) | (h3 << 0);
}

void MDrv_SC_Set_PDNR_Gain(void *pInstance, MS_U8 gain, MS_BOOL isSourceChange )
{
    MApi_XC_W2BYTE(REG_SC_BK2A_1C_L, MDrv_SC_NR_LUT_Aux(pInstance, m_stMCNRLutQmapSetting.pdnrlowLut[0], gain ) );
    MApi_XC_W2BYTE(REG_SC_BK2A_1D_L, MDrv_SC_NR_LUT_Aux(pInstance, m_stMCNRLutQmapSetting.pdnrlowLut[1], gain ) );
    MApi_XC_W2BYTE(REG_SC_BK2A_1E_L, MDrv_SC_NR_LUT_Aux(pInstance, m_stMCNRLutQmapSetting.pdnrlowLut[2], gain ) );
    MApi_XC_W2BYTE(REG_SC_BK2A_1F_L, MDrv_SC_NR_LUT_Aux(pInstance, m_stMCNRLutQmapSetting.pdnrlowLut[3], gain ) );
    MApi_XC_W2BYTE(REG_SC_BK2A_20_L, MDrv_SC_NR_LUT_Aux(pInstance, m_stMCNRLutQmapSetting.pdnrlow2Lut[0], gain ) );
    MApi_XC_W2BYTE(REG_SC_BK2A_21_L, MDrv_SC_NR_LUT_Aux(pInstance, m_stMCNRLutQmapSetting.pdnrlow2Lut[1], gain ) );
    MApi_XC_W2BYTE(REG_SC_BK2A_22_L, MDrv_SC_NR_LUT_Aux(pInstance, m_stMCNRLutQmapSetting.pdnrlow2Lut[2], gain ) );
    MApi_XC_W2BYTE(REG_SC_BK2A_23_L, MDrv_SC_NR_LUT_Aux(pInstance, m_stMCNRLutQmapSetting.pdnrlow2Lut[3], gain ) );
    MApi_XC_W2BYTE(REG_SC_BK2A_24_L, MDrv_SC_NR_LUT_Aux(pInstance, m_stMCNRLutQmapSetting.pdnrhighLut[0], gain ) );
    MApi_XC_W2BYTE(REG_SC_BK2A_25_L, MDrv_SC_NR_LUT_Aux(pInstance, m_stMCNRLutQmapSetting.pdnrhighLut[1], gain ) );
    MApi_XC_W2BYTE(REG_SC_BK2A_26_L, MDrv_SC_NR_LUT_Aux(pInstance, m_stMCNRLutQmapSetting.pdnrhighLut[2], gain ) );
    MApi_XC_W2BYTE(REG_SC_BK2A_27_L, MDrv_SC_NR_LUT_Aux(pInstance, m_stMCNRLutQmapSetting.pdnrhighLut[3], gain ) );
}

void MDrv_SC_Set_MCNR_Gain(void *pInstance, MS_U8 gain, MS_BOOL isSourceChange )
{
    MApi_XC_W2BYTE(REG_SC_BK2A_08_L, MDrv_SC_NR_LUT_Aux(pInstance, m_stMCNRLutQmapSetting.mcnrLut[0], gain ) );
    MApi_XC_W2BYTE(REG_SC_BK2A_09_L, MDrv_SC_NR_LUT_Aux(pInstance, m_stMCNRLutQmapSetting.mcnrLut[1], gain ) );
    MApi_XC_W2BYTE(REG_SC_BK2A_0A_L, MDrv_SC_NR_LUT_Aux(pInstance, m_stMCNRLutQmapSetting.mcnrLut[2], gain ) );
    MApi_XC_W2BYTE(REG_SC_BK2A_0B_L, MDrv_SC_NR_LUT_Aux(pInstance, m_stMCNRLutQmapSetting.mcnrLut[3], gain ) );
    MApi_XC_W2BYTE(REG_SC_BK2A_0C_L, MDrv_SC_NR_LUT_Aux(pInstance, m_stMCNRLutQmapSetting.mcnrLut[4], gain ) );
    MApi_XC_W2BYTE(REG_SC_BK2A_0D_L, MDrv_SC_NR_LUT_Aux(pInstance, m_stMCNRLutQmapSetting.mcnrLut[5], gain ) );
    MApi_XC_W2BYTE(REG_SC_BK2A_0E_L, MDrv_SC_NR_LUT_Aux(pInstance, m_stMCNRLutQmapSetting.mcnrLut[6], gain ) );
    MApi_XC_W2BYTE(REG_SC_BK2A_0F_L, MDrv_SC_NR_LUT_Aux(pInstance, m_stMCNRLutQmapSetting.mcnrLut[7], gain ) );
}

void MDrv_SC_Set_PostDINR_Gain(void *pInstance, MS_U8 gain, MS_BOOL isSourceChange )
{
	static MS_U16 lut[4];
	if( isSourceChange )
	{
		lut[0] = MApi_XC_R2BYTE(REG_SC_BK61_08_L);
		lut[1] = MApi_XC_R2BYTE(REG_SC_BK61_09_L);
		lut[2] = MApi_XC_R2BYTE(REG_SC_BK61_0A_L);
		lut[3] = MApi_XC_R2BYTE(REG_SC_BK61_0B_L);
	}
	else
	{
		MApi_XC_W2BYTE(REG_SC_BK61_08_L, MDrv_SC_NR_LUT_Aux(pInstance, lut[0], gain ) );
		MApi_XC_W2BYTE(REG_SC_BK61_09_L, MDrv_SC_NR_LUT_Aux(pInstance, lut[1], gain ) );
		MApi_XC_W2BYTE(REG_SC_BK61_0A_L, MDrv_SC_NR_LUT_Aux(pInstance, lut[2], gain ) );
		MApi_XC_W2BYTE(REG_SC_BK61_0B_L, MDrv_SC_NR_LUT_Aux(pInstance, lut[3], gain ) );
	}
}

void MDrv_SC_Set_Luma_PK_Gain(void *pInstance, MS_U8 gain )
{
	const MS_U8 tableL[4] = { 2, 2, 8, 8 };
	const MS_U8 tableH[4] = { 10, 8, 8, 6 };

	MApi_XC_W2BYTE(REG_SC_BK19_55_L, 0x21 );
	MApi_XC_W2BYTE(REG_SC_BK19_56_L, MDrv_SC_Luma_PK_LUT_Aux(pInstance, tableL, gain ) );
	MApi_XC_W2BYTE(REG_SC_BK19_57_L, MDrv_SC_Luma_PK_LUT_Aux(pInstance, tableH, gain ) );
}

void MDrv_SC_GMV_Stability(void *pInstance, MS_U16 *gmvDiff, MS_U8 *gmvAvgX, MS_U8 *gmvAvgY, const XC_GMvStates *GMV, const MS_BOOL isOpFrameChanged )
{
	static XC_GMvStates gmvHist[8];
	static MS_U8 histPos = 0;
	static MS_U16 mvDiff = 0;
	static MS_U16 mvAvgX = 0, mvAvgY = 0;

	if( isOpFrameChanged )
	{
		gmvHist[histPos] = *GMV;
		histPos = histPos < 7 ? histPos+1 : 0;

		mvDiff = 0;
		mvAvgX = mvAvgY = 0;
		int i;
		for( i = 0 ; i < 8 ; ++i )
		{
			if( i != 0 )
				mvDiff += abs( gmvHist[i].minMvXdir - gmvHist[i-1].minMvXdir ) + abs( gmvHist[i].minMvYdir - gmvHist[i-1].minMvYdir );
			mvAvgY += gmvHist[i].minMvYdir;
			mvAvgX += gmvHist[i].minMvXdir;
		}
		mvAvgY = ( mvAvgY + 4 ) >> 3;
		mvAvgX = ( mvAvgX + 4 ) >> 3;
	}

	*gmvDiff = mvDiff;
	*gmvAvgX = mvAvgX;
	*gmvAvgY = mvAvgY;
}

MS_U8 MDrv_SC_NR_HouseMD_Patch(void *pInstance, 
	/*const MS_U8 gmvAvgX, const MS_U8 gmvAvgY, const int blkConfidence,*/ const MS_U16 me1NotFoundBlk, const XC_LumaHistogramStates *lumaHistogramStates,
	const MS_BOOL isSceneChange
)
{
	static MS_U8 u8LowLumaVerMovingCnt = 0;

	MS_U32 darkSum = 0, lowLumaSum = 0, avgLuma = 0, totalSum = 0;
	int i = 0;
	for( i = 0 ; i < 32 ; ++i )
	{
		if( i < 4 ) darkSum += lumaHistogramStates->lumaBin[i];
		if( i < 8 ) lowLumaSum += lumaHistogramStates->lumaBin[i];
		totalSum += lumaHistogramStates->lumaBin[i];
		avgLuma += lumaHistogramStates->lumaBin[i] * i;
	}
	if( totalSum != 0 )
	{
		avgLuma /= totalSum;
		darkSum = darkSum * 256 / totalSum;
		lowLumaSum = lowLumaSum * 256 / totalSum;
	}
	else
	{
		avgLuma = 0;
		darkSum = 0;
		lowLumaSum = 0;
	}

	MS_U16 u16TopCnt = MApi_XC_R2BYTEMSK(REG_SC_BK0B_0D_L, 0x03FF );
	MS_U16 u16BottomCnt = MApi_XC_R2BYTEMSK(REG_SC_BK0B_0E_L, 0x03FF );

	//PRT_TRP( "Luma dark, low, avg: %d, %d, %d\n", (int)darkSum, (int)lowLumaSum, (int)avgLuma );

	if(
		( u8LowLumaVerMovingCnt <= 8 ? (darkSum > 170) : (darkSum > 160)  )
		&& lowLumaSum > 230
		&& avgLuma < 4
		/*&& blkConfidence > 70*/
		&& me1NotFoundBlk > 0x300
		&& ( u8LowLumaVerMovingCnt <= 8 ? (u16TopCnt > 0xFF || u16BottomCnt > 0xFF) : (u16TopCnt > 0xF || u16BottomCnt > 0xF) )
	)
	{
		u8LowLumaVerMovingCnt = u8LowLumaVerMovingCnt < 64 ? u8LowLumaVerMovingCnt+1 : 64;
	}
	else
	{
		u8LowLumaVerMovingCnt = u8LowLumaVerMovingCnt > 0 ? u8LowLumaVerMovingCnt-1 : 0;
	}

	if( isSceneChange )
	{
		u8LowLumaVerMovingCnt = 0;
	}

	//MApi_XC_W2BYTEMSK(REG_SC_BK30_44_L, isVerMoving ? 0x0020 : 0x0010, 0x00F0 );

	return u8LowLumaVerMovingCnt;
}

void MDrv_SC_NR_Driver_E28
(
    void *pInstance, 
    const MS_U16 u16MotionValue4,
/*	const int blkConfidence,*/
    //const MS_U32 hpfHdTolerantReport,
    const XC_LumaHistogramStates *lumaHistogramStates,
    const XC_FilmStates *filmstatus,
    //const XC_LetterBoxState *letterBoxState,
    //const XC_ApiStatus *stXCStatus,
    const XC_GMvStates *GMV,
    const MS_BOOL isUkosabihsMotion,
    const MS_BOOL isSceneChangeExt,
    const MS_BOOL bInterlace,
    const MS_BOOL isHDSrc,
    const MS_BOOL isSourceChange,
    const MS_BOOL isOpFrameChanged,
    const MS_U8 *estNoise,
    const MS_U8 NE_confirm
)
{
    // Update MCNR/PDNR LUT from Qmap
    MS_BOOL bMCNR_LUT_Reload = (MApi_XC_R2BYTEMSK(REG_SC_BK30_0C_L, 0x0400)>>8);
    if(bMCNR_LUT_Reload)
    {
        if(Hal_PQ_LoadMCNRLutQmapSetting())
            MApi_XC_W2BYTEMSK(REG_SC_BK30_0C_L, 0x0000, 0x0400);
    }

	// debug en
	MS_BOOL dbgEn = MApi_XC_R2BYTEMSK(REG_SC_BK30_38_L, 0x0001 ) & (MApi_XC_R2BYTEMSK(REG_SC_BK30_39_L, 0x0001 ));
	if( dbgEn ) MApi_XC_W2BYTEMSK(REG_SC_BK30_3A_L, 0x0003, 0x0003 );

	static MS_BOOL prevIsSceneChangeExt = 0;
	MS_BOOL isSceneChange = 0;
	if( (!prevIsSceneChangeExt) && isSceneChangeExt )
		isSceneChange = 1;

	prevIsSceneChangeExt = isSceneChangeExt;

	static MS_U8 mcnrMotionCnt = 0;
	static MS_U8 noiseLevel = 0;
	//static MS_U8 smvFrameCnt = 0;
	static MS_U8 panScanCnt = 0;

	/* Get UCNR Status */
	MS_U16 me1MovingWeight3x3Min = MDrv_SC_read_avgMcnrMovingWeightMin(pInstance);
	//MS_U16 me1MovingWeight3x3Max = MDrv_SC_read_avgMcnrMovingWeightMax(pInstance);
	MS_U16 me1MovingWeight = MDrv_SC_read_avgMcnrMovingWeight(pInstance);
	MS_U16 me1Mv0Cost = MDrv_SC_read_avgMv0MovingWeight(pInstance);
	MS_U16 me2SadDiff = MDrv_SC_read_me2SadDiff(pInstance);

	MS_U16 mcnrWPixCnt = MApi_XC_R2BYTE(REG_SC_BK09_70_L);
	//MS_U16 mcnr2WPixCnt = MApi_XC_R2BYTE(REG_SC_BK09_71_L);
	MS_U16 pdnrWPixCnt = MApi_XC_R2BYTE(REG_SC_BK09_72_L);

	MS_U8 smallMovingW = MDrv_SC_McSmallMoving(pInstance, me1MovingWeight3x3Min, me1Mv0Cost );
	if (dbgEn)
	DEBUG_LC_40(pInstance, smallMovingW );

	MS_U8 motion4StillCnt = MDrv_SC_Motion4StillCnt(pInstance, u16MotionValue4 ); // 0 ~ 15
	//MS_U8 stillW = MDrv_SC_Me1Still(pInstance, me1MovingWeight3x3Max, me1Mv0Cost );

	/*1D motion status*/
	MS_U16 mv0pixels  = MApi_XC_R2BYTE(REG_SC_BK09_78_L);
	MS_U16 nonMv0Pixels = MApi_XC_R2BYTE(REG_SC_BK09_79_L);
	MS_U16 mvNotFoundPixels = MApi_XC_R2BYTE(REG_SC_BK09_7A_L);

	/*2D motion status*/
	MS_U16 me1Mv0Blk = MApi_XC_R2BYTE(REG_SC_BK17_48_L);
	MS_U16 me1NonMv0Blk = MApi_XC_R2BYTE(REG_SC_BK17_49_L);
	MS_U16 me1NotFoundBlk = MApi_XC_R2BYTE(REG_SC_BK17_4A_L);
	MS_U16 me2Mv0Blk = MApi_XC_R2BYTE(REG_SC_BK17_4B_L);
	MS_U16 me2NonMv0Blk = MApi_XC_R2BYTE(REG_SC_BK17_4C_L);
	MS_U16 me2NotFoundBlk = MApi_XC_R2BYTE(REG_SC_BK17_4D_L);

	static MS_BOOL lastEN = 0;


	if( isSceneChange || isSourceChange )
		noiseLevel = 0;
	else
	{
        //NE_confirm = MDrv_SC_GetNoiseLevel( &estNoise, letterBoxState, stXCStatus, bInterlace, isOpFrameChanged );
		if (NE_confirm==8)
			lastEN=1;
		else if (NE_confirm==0 || NE_confirm==1)
			lastEN=0;
		if( NE_confirm == 8 || (NE_confirm==2 && lastEN==1))
        noiseLevel = *estNoise;
		else
			noiseLevel = 0;
	}
	//printf("noise level : %d, %d\n", noiseLevel, NE_confirm);
	//printf("noise opframe change : %d\n", (int)isOpFrameChanged);

	// NE adaptive debug en
	MS_BOOL neAdptDbgEn = /*(MApi_XC_R2BYTE(REG_SC_BK2A_51_L) == 0x0100) && */(!isUkosabihsMotion);

	// NE adaptive dnr4w_max
	MS_U8 dnr4Max = noiseLevel > 40 ? (noiseLevel-40)/4 + 0xC : 0xC;
	dnr4Max = dnr4Max < 0xF ? dnr4Max : 0xF;
	dnr4Max = neAdptDbgEn ? dnr4Max : 0xC;
	MApi_XC_W2BYTEMSK(REG_SC_BK2A_4D_L, dnr4Max << 12, 0xF000 );

	// NE adaptive mcnr noise level
	MS_U8 nlMax = noiseLevel > 40 ? (noiseLevel-40)/4 + 8 : 8;
	nlMax = nlMax < 0xF ? nlMax : 0xF;
	nlMax = neAdptDbgEn ? nlMax : 8;
	MApi_XC_W2BYTEMSK(REG_SC_BK2A_18_L, nlMax << 8, 0x0F00 );

	MS_U16 gmvDiff = 0;
	MS_U8 gmvAvgX = 0, gmvAvgY = 0;
	MDrv_SC_GMV_Stability(pInstance, &gmvDiff, &gmvAvgX, &gmvAvgY, GMV, isOpFrameChanged );
	//printf( "GMV diff, avgX, avgY: %d, %d %d\n", gmvDiff, gmvAvgX, gmvAvgY );

	//DEBUG_LC_41(pInstance, motion4StillCnt );
	//DEBUG_LC_43(pInstance, noiseLevel );
	//printf( "noise level : %d\n", noiseLevel );

	// force film-like/m2s weight to max if source is progressive/film
	if( (!bInterlace) || filmstatus->film_act )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_6E_L, 0xE000, 0xFF00);
		if( dbgEn ) MApi_XC_W2BYTEMSK(REG_SC_BK30_44_L, 0x5000, 0xF000 );
	}
	else
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_6E_L, 0x0000, 0xFF00);
		if( dbgEn ) MApi_XC_W2BYTEMSK(REG_SC_BK30_44_L, 0x3000, 0xF000 );
	}

	MS_U16 mcnrWPixCntThL, pdnrWPixCntThL, mcnrWPixCntThH, pdnrWPixCntThH;
//	MS_U16 smvTh;
	MS_U16 panScanThL, panScanThH;
	MS_U16 almostStillTh;
	if( isHDSrc )
	{
		mcnrWPixCntThL = 0xF0;
		pdnrWPixCntThL = 0xF0;
		mcnrWPixCntThH = 0x3800;
		pdnrWPixCntThH = 0x3800;
		panScanThH = 0x3000;
		panScanThL = 0x2000;
		almostStillTh = 0x200;
#if 0
		if( noiseLevel > 0x40 )
			smvTh = 0xFF;
		else
			smvTh = 0x00;
#endif
	}
	else
	{
		mcnrWPixCntThL = 0xF0;
		pdnrWPixCntThL = 0xF0;
		mcnrWPixCntThH = 0x980; //0x580;
		pdnrWPixCntThH = 0xB80; //0xA00;
		panScanThH = 0x800;
		panScanThL = 0x500;
		almostStillTh = 0x30;
#if 0
		if( noiseLevel > 0x40 )
			smvTh = 0xFF;
		else
			smvTh = 0x00;
#endif
	}

	static MS_BOOL isStill = true, isStillDelay = true, isStillNoSceneChane = true, isStillDelayNoSceneChane = true;
	if( (mcnrWPixCnt < mcnrWPixCntThL && pdnrWPixCnt < pdnrWPixCntThL && motion4StillCnt > 8 )
		|| ( mcnrWPixCnt < mcnrWPixCntThH && pdnrWPixCnt < pdnrWPixCntThH && motion4StillCnt > 12 )
		)
	{
		mcnrMotionCnt = 0;
		isStill = isStillDelay = true;
		isStillNoSceneChane = isStillDelayNoSceneChane = isSceneChange ? false : true;
	}
	else
	{
		if( mcnrMotionCnt > 16 )
		{
			isStill = isStillNoSceneChane = false;

			if( mcnrMotionCnt > 32 )
				isStillDelay = isStillDelayNoSceneChane = false;
			else
			{
				isStillDelay = true;
				isStillDelayNoSceneChane = ( isSceneChange || !isStillDelayNoSceneChane ) ? false : true;
				++mcnrMotionCnt;
			}
		}
		else
		{
			isStill = isStillDelay = true;
			isStillNoSceneChane = ( isSceneChange || !isStillNoSceneChane ) ? false : true;
			isStillDelayNoSceneChane = ( isSceneChange || !isStillDelayNoSceneChane ) ? false : true;
			++mcnrMotionCnt;
		}
	}

	// reduce pdnr weight by ME1 smv at small moving or almost still scenes
//	MS_BOOL isAlmostStill;
	if( me1MovingWeight < almostStillTh )
	{
//		isAlmostStill = true;
		if( dbgEn ) MApi_XC_W2BYTEMSK(REG_SC_BK30_44_L, 0x0010, 0x00F0 );
	}
	else
	{
//		isAlmostStill = false;
		if( dbgEn ) MApi_XC_W2BYTEMSK(REG_SC_BK30_44_L, 0x0020, 0x00F0 );
	}

	// panScan
	static MS_BOOL isPanScan = false;
	if( me1MovingWeight > panScanThH )
	{
		if( panScanCnt < 16 )
			++panScanCnt;
	}
	else
	{
		if( !(isPanScan && me1MovingWeight > panScanThL) )
		{
			if( panScanCnt > 0 )
				--panScanCnt;
		}
	}

	if( panScanCnt > 6 )
	{
		isPanScan = true;
		if( dbgEn ) MApi_XC_W2BYTEMSK(REG_SC_BK30_44_L, 0x0100, 0x0F00 );
	}
	else
	{
		isPanScan = false;
		if( dbgEn ) MApi_XC_W2BYTEMSK(REG_SC_BK30_44_L, 0x0200, 0x0F00 );
	}

	// low texture moving
	static MS_U8 lowTexMovingCnt = 0; // 0~32
	MS_BOOL isMe1Moving = false, isMe2Moving = false, is1DLowTexMoving = false;
	if( me1Mv0Blk < 0x20 && me1NonMv0Blk < 0x40 && me1NotFoundBlk > 0x3D0 && me1MovingWeight > 0x500/*0x1300*/ )
	{
		isMe1Moving = true;
	}

	if( me2Mv0Blk < 0x800 && me2NonMv0Blk > 0x700 && me2NotFoundBlk > 0xD00 )
	{
		isMe2Moving = true;
	}

	if( mv0pixels < 0x2FF && nonMv0Pixels < 0xF00 && mvNotFoundPixels < 0x160 )
	{
		is1DLowTexMoving = true;
	}

	if( isMe1Moving && isMe2Moving && is1DLowTexMoving )
	{
		if( lowTexMovingCnt < 32 )
			++lowTexMovingCnt;
	}
	else if( lowTexMovingCnt > 0 )
	{
		--lowTexMovingCnt;
	}

	MS_BOOL isLowTexMoving = false;
	if( lowTexMovingCnt > 8 )
	{
		// enable keep detail
		isLowTexMoving = true;
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_18_L, 0x1000, 0xF000);
	}
	else
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_18_L, 0xF000, 0xF000);
	}

	static MS_U8 pdnrGain = 16;
	static MS_U8 me1NRStrength = 0xF;

	// isUkosabihsMotion then reduce PDNR table
	if( isUkosabihsMotion && isPanScan && !isLowTexMoving )
	{
		// maximum PDNR gain 12
		pdnrGain = pdnrGain > 12 ? pdnrGain-1 : (pdnrGain < 12 ? pdnrGain + 1 : pdnrGain );
		// reduce ME1_moving_w_nr_strength to 3
		me1NRStrength = me1NRStrength > 3 ? me1NRStrength - 1 : me1NRStrength;
	}
	// isLowTexMoving or isUkosabihsMotion but me1 moving weight is small then turn off PDNR
	else if( ( isUkosabihsMotion && !isPanScan ) || isLowTexMoving )
	{
		// turn off PDNR
			pdnrGain = pdnrGain > 6 ? pdnrGain - 6 : 0;
			me1NRStrength = me1NRStrength < 0xE ? me1NRStrength + 2 : 0xF;
	}
	else
	{
			pdnrGain = pdnrGain < 10 ? pdnrGain + 6 : 16;
			me1NRStrength = me1NRStrength < 0xB ? me1NRStrength + 4 : 0xF;
	}

	static MS_U8 maxSnrStrength = 32;
	// isUkosabihsMotion || isPanScan then turn off SNR
	if( isUkosabihsMotion || isPanScan )
		maxSnrStrength = maxSnrStrength > 0 ? maxSnrStrength-1 : 0;
	else
		maxSnrStrength = maxSnrStrength < 32 ? maxSnrStrength+1 : 32;

	// max SNR Weight
	MApi_XC_W2BYTEMSK(REG_SC_BK2A_69_L, maxSnrStrength << 8, 0x3F00);

#if HOUSE_MD_PATTERN_ENABLE
	// House MD patch
	MS_U8 u8LowLumaVerMovingCnt = MDrv_SC_NR_HouseMD_Patch( pInstance,/*gmvAvgX, gmvAvgY, blkConfidence, */me1NotFoundBlk, lumaHistogramStates, isSceneChange );
#else
    static MS_U8 u8LowLumaVerMovingCnt = 0;
#endif


	// force 2D for stable vertical moving
	static MS_U8 force2DCnt = 0;
	static MS_U8 force1Dcnt = 0;
	//if( isUkosabihsMotion && gmvDiff < 2 && gmvAvgY != 8 && abs( gmvAvgY - 8 ) < 7)
	if( isUkosabihsMotion && gmvDiff < 2 &&
		( (gmvAvgY != 8 && abs( gmvAvgY - 8 ) < 7) || ( abs( gmvAvgX - 32 ) < 16 && abs( gmvAvgX - 32 ) > 9 ) )
	)
	{
		if( force2DCnt < 32 )
			++force2DCnt;
	}
	else if(  gmvDiff < 5 &&
		(gmvAvgY != 8 && abs( gmvAvgY - 8 ) < 7) && isPanScan && ( abs( gmvAvgX - 32 ) < 3)  )
	{
		if( force2DCnt < 32 )
			++force2DCnt;
	}
	else
	{
		if( force2DCnt > 0 )
			--force2DCnt;
	}

	// force 1D if small moving horizontal burst ( me2 not stable )
	if( !isUkosabihsMotion /*&& hpfHdTolerantReport > 0xC000*/ && me1Mv0Cost > 0x1900 && me2SadDiff < 0x100 )
	{
		if( force1Dcnt < 32 )
			++force1Dcnt;
	}
	else
	{
		if( force1Dcnt > 0 )
			--force1Dcnt;
	}

	if( !isUkosabihsMotion && u8LowLumaVerMovingCnt > 8 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_6D_L, 0x4100, 0x7F00 );
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_07_L, 0x0000, 0x0008 );
	}
	else
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_07_L, 0x0008, 0x0008 );

	if( force2DCnt > 16 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_6D_L, 0x4000, 0x7F00 );
	}
	else if( force1Dcnt > 16 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_6D_L, 0x6000, 0x7F00 );
	}
	else if( force2DCnt < 8 || force1Dcnt < 8 )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK2A_6D_L, 0x0000, 0x7F00 );
	}
	}

	// still and vertical pan scan
	static MS_U8 mcnrGain = 16;
	if( isUkosabihsMotion )
	{
		if( gmvDiff < 2 && gmvAvgY != 8 && abs( gmvAvgY - 8 ) < 7 && abs( gmvAvgX - 32 ) < 16 )
		{
			// stable vertical moving
			mcnrGain = mcnrGain > 8 ? mcnrGain  - 1 : ( mcnrGain < 8 ? mcnrGain + 1 : 8 );
		}
		else if( gmvAvgY != 8 && abs( gmvAvgY - 8 ) < 7 && abs( gmvAvgX - 32 ) < 16 )
		{
			mcnrGain = mcnrGain > 4 ? mcnrGain  - 1 : ( mcnrGain < 4 ? mcnrGain + 1 : 4 );
		}
		else if( abs( gmvAvgY - 8 ) >= 7 || abs( gmvAvgX - 32 ) >= 16 )
		{
			// high mv vertical moving
			mcnrGain = mcnrGain > 0 ? mcnrGain  - 1 : 0;
		}
		else
		{
			mcnrGain = mcnrGain < 16 ? mcnrGain + 1 : 16;
		}
	}
	else if( isPanScan )
	{
		// not isUkosabihsMotion
		if( gmvAvgY != 8 && abs( gmvAvgY - 8 ) < 7 )
		{
			if( force2DCnt > 16 )
			{
				mcnrGain = mcnrGain < 16 ? mcnrGain + 1 : 16;
			}
			else
			{
			mcnrGain = mcnrGain > 4 ? mcnrGain  - 1 : ( mcnrGain < 4 ? mcnrGain + 1 : 4 );
		}
		}
		else if( abs( gmvAvgY - 8 ) >= 7 )
		{
			// high mv vertical moving
			mcnrGain = mcnrGain > 0 ? mcnrGain  - 1 : 0;
		}
		else
		{
			mcnrGain = mcnrGain < 16 ? mcnrGain + 1 : 16;
		}
	}
	else if( (abs( gmvAvgX - 32 ) >= 16) && (force2DCnt < 16) )
	{
		// motion is out of 1D search range
		// MCNR creates contours
		mcnrGain = mcnrGain > 8 ? mcnrGain  - 1 : mcnrGain;
		pdnrGain = pdnrGain < 20 ? pdnrGain + 2 : pdnrGain;
	}
	else
	{
		//mcnrGain = mcnrGain < 16 ? mcnrGain + 1 : 16;
		//mcnrGain = isSceneChange ? 16 : ( mcnrGain < 16 ? mcnrGain + 1 : 16 );
		 mcnrGain = isSceneChange ? 16 : ( mcnrGain < 12 ? mcnrGain + 4 :16 );
	}

	// direct overwrite
	if( isStillNoSceneChane /*isStill*/ || ( isStillDelayNoSceneChane/*isStillDelay*/ && gmvAvgY != 8) )
	{
		mcnrGain = 3;
	}

	// MCNR gain
	MDrv_SC_Set_MCNR_Gain( pInstance,mcnrGain, isSourceChange );

	// direct overwite
	if( isStill || isSceneChange )
	{
		pdnrGain = 0;
	}

	// PDNR gain
	//MDrv_SC_Set_PDNR_Gain( pdnrGain, isSourceChange );
	MDrv_SC_Set_PDNR_Gain(pInstance, ( isStill || isSceneChange ) ? 0 : pdnrGain,isSourceChange );
	// ME1_moving_w_nr_strength
	MApi_XC_W2BYTEMSK(REG_SC_BK2A_66_L, me1NRStrength << 12, 0xF000);


    // For BWS1&UCNR both ON, image sticking @scene change (turn on UCNR dither @scene change 12 times.)
    // 2014.9.16 Ryan
    static MS_U16 SceneCnt=0;
    if( ((MApi_XC_R2BYTE(REG_SC_BK08_01_L)& 0x0001) == 0x0000) && (isSceneChangeExt) ) //Due to compress issue, when BWS enable and scenechange, dither enable
    {
        SceneCnt = 12; //
    }
    else
    {
        if(SceneCnt>0)
        {
            SceneCnt = SceneCnt - 1;
        }
        else
        {
            SceneCnt = 0;
        }
    }

    if(SceneCnt>0)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_5C_L, 0x003C, 0x007F);
    }
    else
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK2A_5C_L, 0x007F, 0x007F);
    }


	static MS_U8 postDINRGain = 16;
	if( isUkosabihsMotion || isPanScan || is1DLowTexMoving )
	{
		postDINRGain = postDINRGain > 4 ? postDINRGain  - 4 : 0;
	}
	else
	{
		postDINRGain = postDINRGain < 16 ? postDINRGain + 1 : 16;
	}

	// PostDI NR gain
	MDrv_SC_Set_PostDINR_Gain(pInstance, postDINRGain, isSourceChange );

	//printf( "Gain : %d, %d, %d \n", mcnrGain, pdnrGain, postDINRGain );

#if 0
	// luma peaking
	if( isPanScan && gmvAvgX != 32 && gmvAvgY == 8 && !isUkosabihsMotion )
	{
		MDrv_SC_Set_Luma_PK_Gain(pInstance, panScanCnt );
	}
	else
	{
		MDrv_SC_Set_Luma_PK_Gain(pInstance, 0 );
	}
#endif

	if( dbgEn )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK30_45_L, (isMe1Moving ? 0x0002 : 0x0001), 0x000F );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_45_L, (isMe2Moving ? 0x0020 : 0x0010), 0x00F0 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_45_L, (is1DLowTexMoving ? 0x0200 : 0x0100), 0x0F00 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_45_L, (isLowTexMoving ? 0x2000 : 0x1000), 0xF000 );

		MApi_XC_W2BYTEMSK(REG_SC_BK30_44_L, (pdnrGain == 0) ? 0x0001 : 0x0002, 0x000F );

		MApi_XC_W2BYTEMSK(REG_SC_BK30_46_L, isUkosabihsMotion ? 0x0020 : 0x0010, 0x00F0 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_46_L, force1Dcnt > 16 ? 0x0200 : 0x0100, 0x0F00 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_46_L, mcnrGain == 16 ? 0x2000 : 0x1000, 0xF000 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_47_L, (isSceneChange ? 0x0002 : 0x0001), 0x000F );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_47_L, (isStillNoSceneChane ? 0x0020 : 0x0010), 0x00F0 );

		MApi_XC_W2BYTEMSK(REG_SC_BK30_47_L, ( ( (NE_confirm == 8) || (NE_confirm==2 && lastEN==1) ) ? 0x2000 : 0x1000), 0xF000 );

		MApi_XC_W2BYTEMSK(REG_SC_BK30_48_L, (noiseLevel > 10 ? 0x0002 : 0x0), 0x000F );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_48_L, (noiseLevel > 20 ? 0x0020 : 0x0), 0x00F0 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_48_L, (noiseLevel > 30 ? 0x0200 : 0x0), 0x0F00 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_48_L, (noiseLevel > 40 ? 0x2000 : 0x0), 0xF000 );

		MApi_XC_W2BYTEMSK(REG_SC_BK30_49_L, (noiseLevel > 50 ? 0x0002 : 0x0), 0x000F );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_49_L, (noiseLevel > 60 ? 0x0020 : 0x0), 0x00F0 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_49_L, (noiseLevel > 70 ? 0x0200 : 0x0), 0x0F00 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_49_L, (noiseLevel > 80 ? 0x2000 : 0x0), 0xF000 );

		MApi_XC_W2BYTEMSK(REG_SC_BK30_50_L, (noiseLevel >  90 ? 0x0002 : 0x0), 0x000F );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_51_L, (noiseLevel > 100 ? 0x0020 : 0x0), 0x00F0 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, (noiseLevel > 110 ? 0x0200 : 0x0), 0x0F00 );
		MApi_XC_W2BYTEMSK(REG_SC_BK30_53_L, (noiseLevel > 120 ? 0x2000 : 0x0), 0xF000 );
	}

	// adaptive coring
	//MDrv_Adaptive_Coring( noiseLevel, isSourceChange, isSceneChange );
}
/******************************************************************************/
///LetterBox Driver
/******************************************************************************/
XC_LetterBoxState MDrv_SC_Letter_Box_Driver(void *pInstance, /*const MS_BOOL isHDSrc,*/ const MS_BOOL isSceneChange, const MS_BOOL isSourceChange, const MS_BOOL isOpFrameChanged )
{
	//static MS_U8 lbConfirmLevel = 0;
	static MS_U8 triggerCnt = 0;
	static MS_U8 hisPos = 0;
	static MS_U8 hisCnt = 0;
	static MS_U16 au16HisCntT[16] = {0};
	static MS_U16 au16HisCntB[16] = {0};
	static MS_U16 au16HisCntL[16] = {0};
	static MS_U16 au16HisCntR[16] = {0};
	static MS_U16 cntIIRT = 0, cntIIRB = 0, cntIIRL = 0, cntIIRR = 0;
	static MS_U16 prevMinCntT = 0, prevMinCntB = 0, prevMinCntL = 0, prevMinCntR = 0;
	//static XC_LetterBoxState lbState;

	MS_U16 triggerMask = MApi_XC_R2BYTEMSK(REG_SC_BK0B_15_L, 0xF000) >> 12;
	triggerMask = triggerMask < 3 ? 3 : triggerMask;

	MApi_XC_W2BYTEMSK(REG_SC_BK0B_05_L, 0x7F, 0x1FFF );

	if( isSceneChange || isSourceChange )
	{
		lbConfirmLevel = 0;
		hisCnt = 0;
		memset( au16HisCntT, 0xFF, sizeof(au16HisCntT) );
		memset( au16HisCntB, 0xFF, sizeof(au16HisCntB) );
		memset( au16HisCntL, 0xFF, sizeof(au16HisCntL) );
		memset( au16HisCntR, 0xFF, sizeof(au16HisCntR) );
	}

	if( isOpFrameChanged )
	{
		if( (triggerCnt & triggerMask) == 0 )
		{
			au16HisCntT[hisPos] = MApi_XC_R2BYTEMSK(REG_SC_BK0B_0D_L, 0x03FF );
			au16HisCntB[hisPos] = MApi_XC_R2BYTEMSK(REG_SC_BK0B_0E_L, 0x03FF );
			au16HisCntL[hisPos] = MApi_XC_R2BYTEMSK(REG_SC_BK0B_0F_L, 0x07FF );
			au16HisCntR[hisPos] = MApi_XC_R2BYTEMSK(REG_SC_BK0B_10_L, 0x07FF );

			MS_U16 minCntT = au16HisCntT[0], minCntB = au16HisCntB[0], minCntL = au16HisCntL[0], minCntR = au16HisCntR[0];
			int i;
			for( i = 1 ; i < 16 ; ++i )
			{
				minCntT = au16HisCntT[i] < minCntT ? au16HisCntT[i] : minCntT;
				minCntB = au16HisCntB[i] < minCntB ? au16HisCntB[i] : minCntB;
				minCntL = au16HisCntL[i] < minCntL ? au16HisCntL[i] : minCntL;
				minCntR = au16HisCntR[i] < minCntR ? au16HisCntR[i] : minCntR;
			}

			if( hisPos == 15 )
				hisPos = 0;
			else
				++hisPos;

			if( hisCnt < 16 )
				++hisCnt;

			MS_U16 conf = abs( (int)(minCntT + minCntB + minCntL + minCntR) - (int)(prevMinCntT + prevMinCntB + prevMinCntL + prevMinCntR) );
			conf = conf < 256 ? 255 - conf : 0;
			if ( minCntL == 0x01FFF || minCntR == 0x01FFF || minCntT == 0x01FFF || minCntB == 0x01FFF)
				conf = 0;

			lbConfirmLevel = (lbConfirmLevel*6 + conf*2 )>> 3;
			lbState.lbComfirmLevel = conf;
			lbState.lbCntT = cntIIRT = ( cntIIRT * 7 + minCntT ) >> 3;
			lbState.lbCntB = cntIIRB = ( cntIIRB * 7 + minCntB ) >> 3;
			lbState.lbCntL = cntIIRL = ( cntIIRL * 7 + minCntL ) >> 3;
			lbState.lbCntR = cntIIRR = ( cntIIRR * 7 + minCntR ) >> 3;

			prevMinCntT = minCntT;
			prevMinCntB = minCntB;
			prevMinCntL = minCntL;
			prevMinCntR = minCntR;
			/*
			if( isHDSrc )
			{
				int ratio = 0x1FA4000 / ( (int)(1920-cntIIRL-cntIIRR) * (int)(1080-cntIIRT-cntIIRB) );
				lbState.lbRatioC = ratio < 256 ? ratio : 255;

				ratio = 0x1FA4000 / ( (int)(1920) * (int)(1080-cntIIRT-cntIIRB) );
				lbState.lbRatioTB = ratio < 256 ? ratio : 255;

				ratio = 0x1FA4000 / ( (int)(1920-cntIIRL-cntIIRR) * (int)(1080) );
				lbState.lbRatioLR = ratio < 256 ? ratio : 255;
			}
			else
			{
				int ratio = 0x546000 / ( (int)(1920-cntIIRL-cntIIRR) * (int)(1080-cntIIRT-cntIIRB) );
				lbState.lbRatioC = ratio < 256 ? ratio : 255;

				ratio = 0x546000 / ( (int)(1920) * (int)(1080-cntIIRT-cntIIRB) );
				lbState.lbRatioTB = ratio < 256 ? ratio : 255;

				ratio = 0x546000 / ( (int)(1920-cntIIRL-cntIIRR) * (int)(1080) );
				lbState.lbRatioLR = ratio < 256 ? ratio : 255;
			}*/
			int windowWidth  = __max((int)(frameWidth-cntIIRL-cntIIRR), 1);
			int windowHeight = __max((int)(frameHeight-cntIIRT-cntIIRB), 1);
			int ratio = 0x1FA40000 / ( windowWidth * windowHeight );
			lbState.lbRatioC = ratio < 256 ? ratio : 255;

			ratio = 0x1FA40000 / ( frameWidth * windowHeight );
			lbState.lbRatioTB = ratio < 256 ? ratio : 255;

			ratio = 0x1FA40000 / ( windowWidth * frameHeight );
			lbState.lbRatioLR = ratio < 256 ? ratio : 255;


			MApi_XC_W2BYTEMSK(REG_SC_BK0B_15_L, cntIIRL, 0x01FF);
			MApi_XC_W2BYTEMSK(REG_SC_BK0B_16_L, cntIIRR, 0x01FF);

			//PRT_TRP( "Letter Box L : %03d, R : %03d, confirm : %03d, ratio C/TB/LR : %02d.%2d, %02d.%2d, %02d.%2d \n", cntIIRL, cntIIRR, conf,
			//lbState.lbRatioC/16, (lbState.lbRatioC&0xF)*10/16, lbState.lbRatioTB/16, (lbState.lbRatioTB&0xF)*10/16, lbState.lbRatioLR/16, (lbState.lbRatioLR&0xF)*10/16 );
		}
		++triggerCnt;

		if (lbConfirmLevel > 224 && !(isSceneChange ||isSourceChange) && (lbState.lbCntL > 100) && (lbState.lbCntR > 100) )
		{
				MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x0080, 0x0080);
				MApi_XC_W2BYTEMSK(REG_SC_BK02_7C_L, lbState.lbCntL + 50, 0x1FFF);
				MApi_XC_W2BYTEMSK(REG_SC_BK02_7D_L, frameWidth - lbState.lbCntR - 50, 0x1FFF);
				MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x7000, 0x7000);
		}
		else
		{
				const int boundary = 10;
				MS_U16 widthCur = MApi_XC_R2BYTEMSK(REG_SC_BK12_0F_L, 0x0FFF);/* cur width */

				MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x0080, 0x0080);
				MApi_XC_W2BYTEMSK(REG_SC_BK02_7C_L, boundary, 0x1FFF); /* set start */
				MApi_XC_W2BYTEMSK(REG_SC_BK02_7D_L, widthCur-boundary, 0x1FFF);/* set end */
				MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0000, 0x7000);
		}
	}

	return lbState;
}

/******************************************************************************/
///DeFeathering
///@param u32MotionValue \     IN: Motion value
/******************************************************************************/
void MDrv_SC_de_feathering(void *pInstance,MS_U32 u32MotionValue)
{
	static MS_U32 u32DeFeatherCntLv1 = 0;
	static MS_U32 u32DeFeatherCntLv2 = 0;
	static MS_U32 u32DeFeatherCntLv3 = 0;
    MS_U8 u8SST_Static_Core_TH = 0;
    MS_U32 reg_defethering_lv1_cnt = 0;
    MS_U32 reg_defethering_lv2_cnt = 0;
    MS_U32 reg_mdp_cnt = 0;

	reg_defethering_lv1_cnt = 0x0A;
	reg_defethering_lv2_cnt = 0x08;
	reg_mdp_cnt = 0x01;

	// motion level count
	if(u32MotionValue >= DEFETHERING_LV1_TH)
	{
		if(u32DeFeatherCntLv1 < reg_defethering_lv1_cnt)
			u32DeFeatherCntLv1++;
	}
	else
	{
		if(u32DeFeatherCntLv1 >= reg_mdp_cnt)
			u32DeFeatherCntLv1 = u32DeFeatherCntLv1 - reg_mdp_cnt;
	}

	if(u32MotionValue >= DEFETHERING_LV2_TH)
	{
		if(u32DeFeatherCntLv2 < reg_defethering_lv2_cnt)
			u32DeFeatherCntLv2++;
	}
	else
	{
		if(u32DeFeatherCntLv2 >= reg_mdp_cnt)
			u32DeFeatherCntLv2 = u32DeFeatherCntLv2 - reg_mdp_cnt;
	}

	if(u32MotionValue >= DEFETHERING_LV3_TH)
	{
		if(u32DeFeatherCntLv3 < DEFETHERING_LV3_CNT)
			u32DeFeatherCntLv3++;
	}
	else
	{
		if(u32DeFeatherCntLv3 >= reg_mdp_cnt)
			u32DeFeatherCntLv3 = u32DeFeatherCntLv3 - reg_mdp_cnt;
	}

	//DeFeathering begin
	if(u32DeFeatherCntLv1 >= reg_defethering_lv1_cnt)
	{
		u8SST_Static_Core_TH    = SST_STATIC_CORE_TH_LV1_VALUE;
	}
	else if(u32DeFeatherCntLv2 >= reg_defethering_lv2_cnt)
	{
		u8SST_Static_Core_TH    = SST_STATIC_CORE_TH_LV2_VALUE;
	}
	else if(u32DeFeatherCntLv3 >= DEFETHERING_LV3_CNT)
	{
		u8SST_Static_Core_TH    = SST_STATIC_CORE_TH_LV3_VALUE;
	}
	else
	{
		u8SST_Static_Core_TH    = SST_STATIC_CORE_TH_LV4_VALUE;
	}

	MApi_XC_W2BYTEMSK(REG_SC_BK22_1A_L, (MS_U16)u8SST_Static_Core_TH, 0xFF);
}

/******************************************************************************/
///DeFlickering
///@param u32MotionValue \     IN: Motion value
/******************************************************************************/
void MDrv_SC_de_flickering(void *pInstance,MS_U32 u32MotionValue)
{
	static MS_S32 s32DeFlickerCnt = 0;
    MS_U32 reg_m_feat_smooth_hle_th = 0, reg_m_feat_smooth_shrink = 0;

	reg_m_feat_smooth_hle_th = (MS_U32)MApi_XC_R2BYTEMSK(REG_SC_BK22_1E_L, 0xF000); // Feat Smooth HLE TH
	reg_m_feat_smooth_shrink = (MS_U32)MApi_XC_R2BYTEMSK(REG_SC_BK22_1E_L, 0x8F);

	if(u32MotionValue >= DEFLICKERING_TH)
	{
		if(s32DeFlickerCnt < 65535)
			s32DeFlickerCnt++;
	}
	else
	{
		s32DeFlickerCnt = 0;
	}

	if(s32DeFlickerCnt >= DEFLICKERING_CNT)
	{
		reg_m_feat_smooth_hle_th += 0x0300;
		reg_m_feat_smooth_shrink += 0x10;
	}
	else
	{
		reg_m_feat_smooth_hle_th += 0x0700;
		reg_m_feat_smooth_shrink += 0x30;
	}

	MApi_XC_W2BYTEMSK(REG_SC_BK22_1E_L, reg_m_feat_smooth_shrink, 0xFF);
	MApi_XC_W2BYTEMSK(REG_SC_BK22_1E_L, reg_m_feat_smooth_hle_th, 0xFF00);
}

/******************************************************************************/
///DeBouncing
///@param u32MotionValue \     IN: Motion value
/******************************************************************************/
#define DEBOUNCING_GAIN 1 //0
void MDrv_SC_de_bouncing(void *pInstance,MS_U32 u32MotionValue)
{
	static MS_S32 s32DeBouncingCnt = 0;
    MS_U32 u32RegHisWtF2 = 0;
    MS_U32 u32regdebouncingth = 0;
    int reg_debouncing_cnt = 0;

	u32regdebouncingth = 0x0A;
	reg_debouncing_cnt = 0x03;

	u32RegHisWtF2 = (MS_U32)MApi_XC_R2BYTEMSK(REG_SC_BK22_0A_L, 0xF8); // history ratio weighting

	if(u32MotionValue <= u32regdebouncingth * DEBOUNCING_GAIN)
	{
		s32DeBouncingCnt = 0;
	}
	else
	{
		if(s32DeBouncingCnt < 65535)
			s32DeBouncingCnt++;
	}

	if(s32DeBouncingCnt >= reg_debouncing_cnt)
	{
		u32RegHisWtF2 += 0x06; // history = 6 moving
	}
	else
	{
		u32RegHisWtF2 += 0x03; // history = 3 still
	}
	MApi_XC_W2BYTEMSK(REG_SC_BK22_0A_L, u32RegHisWtF2, 0xFF);
}

/******************************************************************************/

void MDrv_SC_Edison_DHD_driver(void *pInstance, MS_U32 uComplex, MS_U16 nonMv0Comfirm, MS_BOOL me1Still, MS_U16 mv0Comfirm, XC_GMvStates gMV, MS_U32 _hpfHdCntReport )
{
	// dhd stadium patch: lower dhd strength
	static MS_U32 u32filmCnt = 0;
	int check_width = MApi_XC_R2BYTEMSK(REG_SC_BK12_0F_L, 0xFFFF);

	MS_U32 exactFeatherCnt = 0;
	static MS_U32 pre_exactFeatherCnt = 0;
	MS_BOOL isMv0 = ( ( gMV.minMvXdir == 32 ) && ( gMV.minMvYdir == 8 ) ) ? true : false;

	exactFeatherCnt = (MApi_XC_R2BYTE(REG_SC_BK23_6D_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK23_6C_L);
	exactFeatherCnt = ( pre_exactFeatherCnt*3 + exactFeatherCnt ) >> 2;
	pre_exactFeatherCnt = exactFeatherCnt;

    MS_U32 colorSum = 0;
	static MS_U32 colorSum_iir = 0;

	colorSum  = MApi_XC_R2BYTE(REG_SC_BK1A_6E_L);

	colorSum = ((colorSum_iir*3 )+ colorSum ) >> 2;
	colorSum_iir = colorSum;

	if((uComplex>0x680) && (nonMv0Comfirm > 0x70) && (me1Still == false) && (check_width > 0x700) && ((/*exactFeatherCnt > 0x30 &&*/colorSum > 0x1500 && colorSum < 0x2500 )|| mv0Comfirm < 0x50 ) ) //exactFeatherCnt > 0x30 for stadium & mv0Comfirm < 0x50 for building of "KURO" disc
	{
		u32filmCnt++;

		if( u32filmCnt >= 50 )
		{
			u32filmCnt = 50;
		}
	}
	else if((gMV.h2XYsum > 0x30)&& (gMV.h2XYdiff <= 0x06)&& (!isMv0) && (nonMv0Comfirm>0x40) && mv0Comfirm < 0x20 )//shibasoku pan
	{

		u32filmCnt++;

		if( u32filmCnt >= 50 )
		{
			u32filmCnt = 50;
		}
	}
	else if ( _hpfHdCntReport > 0xFFFF )
	{

		u32filmCnt++;

		if( u32filmCnt >= 50 )
		{
			u32filmCnt = 50;
		}
	}
	else
	{
		if( u32filmCnt == 0 )
		{
			u32filmCnt = 0;
		}
		else
		{
			u32filmCnt--;
		}
	}

	if( u32filmCnt <= 2 )
		MApi_XC_W2BYTEMSK(REG_SC_BK05_2B_L, 0x0F00, 0x0F00); //dhd user weight
	else if( u32filmCnt == 3 )
		MApi_XC_W2BYTEMSK(REG_SC_BK05_2B_L, 0x0C00, 0x0F00); //dhd user weight
	else if( u32filmCnt == 4 )
		MApi_XC_W2BYTEMSK(REG_SC_BK05_2B_L, 0x0B00, 0x0F00); //dhd user weight
	else if( u32filmCnt == 5 )
		MApi_XC_W2BYTEMSK(REG_SC_BK05_2B_L, 0x0900, 0x0F00); //dhd user weight
	else if( u32filmCnt == 6 )
		MApi_XC_W2BYTEMSK(REG_SC_BK05_2B_L, 0x0700, 0x0F00); //dhd user weight
	else if( u32filmCnt == 7 )
		MApi_XC_W2BYTEMSK(REG_SC_BK05_2B_L, 0x0500, 0x0F00); //dhd user weight
	else if( u32filmCnt == 8 )
		MApi_XC_W2BYTEMSK(REG_SC_BK05_2B_L, 0x0300, 0x0F00); //dhd user weight
	else if( u32filmCnt == 9 )
		MApi_XC_W2BYTEMSK(REG_SC_BK05_2B_L, 0x0100, 0x0F00); //dhd user weight
	else if( u32filmCnt >= 10 )
		MApi_XC_W2BYTEMSK(REG_SC_BK05_2B_L, 0x0000, 0x0F00); //dhd user weight

}

void MDrv_SC_Edison_DHD_Weather_driver(void *pInstance, MS_U32 uComplex, MS_U16 nonMv0Comfirm, MS_BOOL me1Still, MS_U16 mv0Comfirm )
{
	// dhd weather patch: enhance dhd strength
	static MS_U32 u32filmCnt = 0;
	int check_width = MApi_XC_R2BYTEMSK(REG_SC_BK12_0F_L, 0xFFFF);

	MS_U32 exactFeatherCnt = 0;
	static MS_U32 pre_exactFeatherCnt = 0;

	exactFeatherCnt = (MApi_XC_R2BYTE(REG_SC_BK23_6D_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK23_6C_L);
	exactFeatherCnt = ( pre_exactFeatherCnt*3 + exactFeatherCnt ) >> 2;
	pre_exactFeatherCnt = exactFeatherCnt;


    MS_U32 colorSum = 0;
	static MS_U32 colorSum_iir = 0;

	colorSum  = MApi_XC_R2BYTE(REG_SC_BK1A_6E_L);

	colorSum = ((colorSum_iir*3 )+ colorSum ) >> 2;
	colorSum_iir = colorSum;

	if((uComplex>0x800) && (mv0Comfirm > 0x140) && (nonMv0Comfirm < 0x40) && (me1Still == true) && (check_width>0x700) && (exactFeatherCnt < 0x10) && (colorSum > 0x700) )
	{
		u32filmCnt++;

		if( u32filmCnt >= 24 )
		{
			u32filmCnt = 24;
		}
	}
	else
	{
		if( u32filmCnt > 0 )
			u32filmCnt = u32filmCnt - 2;
	}

	if( check_width>0x700 )
	{

		if( u32filmCnt <= 2 )
		{
			MApi_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0x0040, 0x00FF); //dhd c edge diff(hor).
			MApi_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0xA000, 0xFF00); //dhd y edge diff(hor).
		}
		else if( u32filmCnt == 3 )
		{
			MApi_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0x0054, 0x00FF); //dhd c edge diff(hor).
			MApi_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0xAB00, 0xFF00); //dhd y edge diff(hor).
		}
		else if( u32filmCnt == 4 )
		{
			MApi_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0x0068, 0x00FF); //dhd c edge diff(hor).
			MApi_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0xB600, 0xFF00); //dhd y edge diff(hor).
		}
		else if( u32filmCnt == 5 )
		{
			MApi_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0x007C, 0x00FF); //dhd c edge diff(hor).
			MApi_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0xC100, 0xFF00); //dhd y edge diff(hor).
		}
		else if( u32filmCnt == 6 )
		{
			MApi_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0x0090, 0x00FF); //dhd c edge diff(hor).
			MApi_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0xCC00, 0xFF00); //dhd y edge diff(hor).
		}
		else if( u32filmCnt == 7 )
		{
			MApi_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0x00A4, 0x00FF); //dhd c edge diff(hor).
			MApi_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0xD700, 0xFF00); //dhd y edge diff(hor).
		}
		else if( u32filmCnt == 8 )
		{
			MApi_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0x00B8, 0x00FF); //dhd c edge diff(hor).
			MApi_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0xE200, 0xFF00); //dhd y edge diff(hor).
		}
		else if( u32filmCnt == 9 )
		{
			MApi_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0x00CC, 0x00FF); //dhd c edge diff(hor).
			MApi_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0xED00, 0xFF00); //dhd y edge diff(hor).
		}
		else if( u32filmCnt >= 10 )
		{
			MApi_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0x00E0, 0x00FF); //dhd c edge diff(hor).
			MApi_XC_W2BYTEMSK(REG_SC_BK05_26_L, 0xF800, 0xFF00); //dhd y edge diff(hor).
		}

	}

}

void MDrv_SC_Edison_DHD_GSLSM_driver(void *pInstance, MS_U32 uComplex, MS_U16 nonMv0Comfirm, MS_BOOL me1Still, MS_U16 mv0Comfirm )
{
	// dhd patch for global still & local moving region( dot-crawl on dirty background ): enhance dhd strength
	static MS_U32 u32filmCnt = 0;
	int check_width = MApi_XC_R2BYTEMSK(REG_SC_BK12_0F_L, 0xFFFF);

	MS_U32 exactFeatherCnt = 0;
	static MS_U32 pre_exactFeatherCnt = 0;

	exactFeatherCnt = (MApi_XC_R2BYTE(REG_SC_BK23_6D_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK23_6C_L);
	exactFeatherCnt = ( pre_exactFeatherCnt*3 + exactFeatherCnt ) >> 2;
	pre_exactFeatherCnt = exactFeatherCnt;

    MS_U32 colorSum = 0;
	static MS_U32 colorSum_iir = 0;

	colorSum  = MApi_XC_R2BYTE(REG_SC_BK1A_6E_L);

	colorSum = ((colorSum_iir*3 )+ colorSum ) >> 2;
	colorSum_iir = colorSum;

	if((uComplex>0x800) && (mv0Comfirm >= nonMv0Comfirm*3) && (me1Still == true) && (check_width>0x700) && ((exactFeatherCnt > 0x40) || (exactFeatherCnt < 0x10 && colorSum > 0xB00) ) ) //exactFeatherCnt < 0x10 && colorSum > 0xB00 for weather
	{
		u32filmCnt++;

		if( u32filmCnt >= 24 )
		{
			u32filmCnt = 24;
		}
	}
	else
	{
		if( u32filmCnt > 0 )
			u32filmCnt = u32filmCnt - 2;
	}

	if( check_width>0x700 )
	{
		if( u32filmCnt <= 2 )
			MApi_XC_W2BYTEMSK(REG_SC_BK05_22_L, 0x0024, 0x00FF); //dhd y(n), y(n-4) diff.
		else if( u32filmCnt == 3 )
			MApi_XC_W2BYTEMSK(REG_SC_BK05_22_L, 0x0026, 0x00FF); //dhd y(n), y(n-4) diff.
		else if( u32filmCnt == 4 )
			MApi_XC_W2BYTEMSK(REG_SC_BK05_22_L, 0x0029, 0x00FF); //dhd y(n), y(n-4) diff.
		else if( u32filmCnt == 5 )
			MApi_XC_W2BYTEMSK(REG_SC_BK05_22_L, 0x002B, 0x00FF); //dhd y(n), y(n-4) diff.
		else if( u32filmCnt == 6 )
			MApi_XC_W2BYTEMSK(REG_SC_BK05_22_L, 0x002E, 0x00FF); //dhd y(n), y(n-4) diff.
		else if( u32filmCnt == 7 )
			MApi_XC_W2BYTEMSK(REG_SC_BK05_22_L, 0x0030, 0x00FF); //dhd y(n), y(n-4) diff.
		else if( u32filmCnt == 8 )
			MApi_XC_W2BYTEMSK(REG_SC_BK05_22_L, 0x0033, 0x00FF); //dhd y(n), y(n-4) diff.
		else if( u32filmCnt == 9 )
			MApi_XC_W2BYTEMSK(REG_SC_BK05_22_L, 0x0035, 0x00FF); //dhd y(n), y(n-4) diff.
		else if( u32filmCnt >= 10 )
			MApi_XC_W2BYTEMSK(REG_SC_BK05_22_L, 0x0038, 0x00FF); //dhd y(n), y(n-4) diff.
	}

}

//
//To do (merge with MDrv_SC_Edison_DHD_driver)
//
void MDrv_SC_Edison_DHD_Stadium_driver(void *pInstance,MS_BOOL isMcFilmHd,const XC_FilmStates filmstatus)
{

	int check_width = MApi_XC_R2BYTEMSK(REG_SC_BK12_0F_L, 0xFFFF);
	int currentGain = MApi_XC_R2BYTEMSK(REG_SC_BK05_2B_L, 0x0F00) >> 8 ;
	if( check_width > 0x700 )
	{
		if( isMcFilmHd || filmstatus.filmany) //disable DHD when Film timing
			//if( filmstatus.film_act )
			MApi_XC_W2BYTEMSK(REG_SC_BK05_2B_L, 0x0000, 0x0F00);
		else
			MApi_XC_W2BYTEMSK(REG_SC_BK05_2B_L, (currentGain<<8), 0x0F00);
	}
	else
	{
		if( filmstatus.filmany) //disable DHD when SD film-any
			MApi_XC_W2BYTEMSK(REG_SC_BK05_2B_L, 0x0000, 0x0F00);
		else
			MApi_XC_W2BYTEMSK(REG_SC_BK05_2B_L, (currentGain<<8), 0x0F00);
	}

}

//
//To adjust the CCS_MR HisWeight Ctrl
//
#define CCS_MR_HisWeight_MaxValue 0x1F
#define CCS_MR_HisWeight_MinValue  0x18
void MDrv_SC_DHD_CCS_MR_HisWeight_Ctrl(const XC_FilmStates filmstatus)
{
	static MS_U8 Reg_CCS_MR_Weight = CCS_MR_HisWeight_MinValue;

	if( (filmstatus.film32) |(filmstatus.filmany))
	{
		Reg_CCS_MR_Weight = CCS_MR_HisWeight_MaxValue;
	}
	else
	{
		if( Reg_CCS_MR_Weight > CCS_MR_HisWeight_MinValue )
			Reg_CCS_MR_Weight--;
		else
			Reg_CCS_MR_Weight = CCS_MR_HisWeight_MinValue;
	}
	MApi_XC_W2BYTEMSK(REG_SC_BK06_77_L, Reg_CCS_MR_Weight<<8, 0x1F00);
}


/******************************************************************************/
///CCS Driver
/******************************************************************************/

void MDrv_SC_Edison_CCS_driver(void *pInstance, const MS_BOOL ukoSabihS_en )
{
	// horizontal moving => lower ccs weight
	static MS_U32 u32CCSvideoCnt = 0;
    MS_BOOL isMvPanScan = FALSE;

	//if((gMv.h1XYsum >10) && (gMv.h2XYsum > 10) && ( gMv.h1XYdiff < 5) && (gMv.h2XYdiff < 5))
	if( ukoSabihS_en )
	{
		isMvPanScan = true;
	}
	else
	{
		isMvPanScan = false;
	}

	if(isMvPanScan)
	{
		u32CCSvideoCnt++;

		if( u32CCSvideoCnt >= 20 )
		{
			u32CCSvideoCnt = 20;
		}
	}
	else
	{
		if( u32CCSvideoCnt == 0 )
		{
			u32CCSvideoCnt = 0;
		}
		else
		{
			u32CCSvideoCnt--;
		}
	}

	if( u32CCSvideoCnt <= 2 )
		MApi_XC_W2BYTEMSK(REG_SC_BK05_53_L, 0x000F, 0x000F); //ccs user weight
	else if( u32CCSvideoCnt == 3 )
		MApi_XC_W2BYTEMSK(REG_SC_BK05_53_L, 0x000D, 0x000F); //ccs user weight
	else if( u32CCSvideoCnt == 4 )
		MApi_XC_W2BYTEMSK(REG_SC_BK05_53_L, 0x000B, 0x000F); //ccs user weight
	else if( u32CCSvideoCnt == 5 )
		MApi_XC_W2BYTEMSK(REG_SC_BK05_53_L, 0x0009, 0x000F); //ccs user weight
	else if( u32CCSvideoCnt == 6 )
		MApi_XC_W2BYTEMSK(REG_SC_BK05_53_L, 0x0007, 0x000F); //ccs user weight
	else if( u32CCSvideoCnt == 7 )
		MApi_XC_W2BYTEMSK(REG_SC_BK05_53_L, 0x0005, 0x000F); //ccs user weight
	else if( u32CCSvideoCnt == 8 )
		MApi_XC_W2BYTEMSK(REG_SC_BK05_53_L, 0x0003, 0x000F); //ccs user weight
	else if( u32CCSvideoCnt == 9 )
		MApi_XC_W2BYTEMSK(REG_SC_BK05_53_L, 0x0001, 0x000F); //ccs user weight
	else if( u32CCSvideoCnt >= 10 )
		MApi_XC_W2BYTEMSK(REG_SC_BK05_53_L, 0x0000, 0x000F); //ccs user weight
}

MS_BOOL MDrv_SC_LowLumaDetection(void *pInstance)
{
    MS_BOOL isLowLuma = FALSE;
    MS_U16 MaxL_pixel = 0;

	MaxL_pixel = MApi_XC_R2BYTE(REG_SC_BK1A_0B_L)&0x00FF;

	if((MaxL_pixel<100)&&(MApi_XC_DLC_GetAverageValue()< 120))
	{
		isLowLuma = true;
	}
	else
	{
		isLowLuma = false;
	}

	return isLowLuma;
}

MS_U8 LumaAGCLUT(void *pInstance,const MS_U8 LumaCheck )
{
	MS_U8 lumaAgcGain = 0;

	if( LumaCheck > 120 )
		lumaAgcGain = 6;
	else if( ( LumaCheck > 114 ) &&  ( LumaCheck <= 120 ))
		lumaAgcGain = 7;
	else if( ( LumaCheck > 104 ) &&  ( LumaCheck <= 114 ))
		lumaAgcGain = 8;
	else if( ( LumaCheck > 94 ) &&  ( LumaCheck <= 104 ))
		lumaAgcGain = 8;
	else if( ( LumaCheck > 83 ) &&  ( LumaCheck <= 94 ))
		lumaAgcGain = 9;
	else if( ( LumaCheck > 73 ) &&  ( LumaCheck <= 83 ))
		lumaAgcGain = 10;
	else if( ( LumaCheck > 62 ) &&  ( LumaCheck <= 73 ))
		lumaAgcGain = 11;
	else if( ( LumaCheck > 51 ) &&  ( LumaCheck <= 62 ))
		lumaAgcGain = 13;
	else if( ( LumaCheck > 40 ) &&  ( LumaCheck <= 51 ))
		lumaAgcGain = 15;
	else if( ( LumaCheck > 26 ) &&  ( LumaCheck <= 40 ))
		lumaAgcGain = 17;
	else
		lumaAgcGain = 15;

	return lumaAgcGain;
}

#define HD_ACT_TH 10
#define HDSDD_SPEED 1
#define HDSDD_DIV 0
#define HD_SPEED 2
#define SD_SPEED 4

void MDrv_SC_Edision_stillFeatherDetection(void *pInstance,const MS_U32 u32MotionValue, const MS_BOOL isUkosabihsMotion,
					   const MS_U32 _hpfHdCntReport, const MS_BOOL isHBurstMoving, const MS_BOOL isHBurst, const MS_BOOL isUkosabihsMv0,
					   const MS_BOOL isMcFilmHd/*, const MS_BOOL isSceneChange*/, const MS_U32 _hpfSdCntReport )
{
	MS_U32 exactFeatherCnt = 0;
	static MS_U32 pre_exactFeatherCnt = 0;

	MS_U32 HD_ConfirmCnt_Reg = 0;
	static MS_U32 HD_ConfirmCnt_Reg_post = 0;
	static MS_U16 HD_ConfirmCnt = 0;

    MS_U8 u8MinPixel = 0;
	static MS_U8 u8PrevMinPixel = 0;

    MS_U8 u8MaxPixel = 0;
	static MS_U8 u8PrevMaxPixel = 0;

    MS_U16 Luma = 0;
	static MS_U16 LumaPrev = 0;
    MS_U8 LumaCheck = 0;

    MS_U16 Cplx = 0;
	static MS_U16 CplxPrev = 0;
    MS_U8 CplxCheck = 0;

	MS_U8 lumaAgcGain = 0;
	MS_BOOL cplxLumaPatch = false;

    MS_U8  featherRatio = 0;

//	MS_BOOL XXX;

	MS_BOOL isMovingImage = ( u32MotionValue!=0 ) ? true : false;

	MS_BOOL isWithoutHsignal = ( _hpfSdCntReport < 0x10 ) ? true : false;

//	XXX = isSceneChange;
	exactFeatherCnt = (MApi_XC_R2BYTE(REG_SC_BK23_6D_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK23_6C_L);
	exactFeatherCnt = ( pre_exactFeatherCnt*3 + exactFeatherCnt ) >> 2;
	pre_exactFeatherCnt = exactFeatherCnt;

	u8MaxPixel = MApi_XC_R2BYTEMSK(REG_SC_BK1A_0B_L, 0x00FF);
	u8MaxPixel = (u8PrevMaxPixel*3 + u8MaxPixel ) >> 2;
	u8PrevMaxPixel = u8MaxPixel;

	u8MinPixel = MApi_XC_R2BYTEMSK(REG_SC_BK1A_0B_L, 0xFF00)>>8;
	u8MinPixel = (u8PrevMinPixel*3 + u8MinPixel ) >> 2;
	u8PrevMinPixel = u8MinPixel;

	Luma = MApi_XC_R2BYTEMSK(REG_SC_BK1A_06_L, 0xFFFF);
	Luma = (LumaPrev*3 + Luma ) >> 2;
	LumaPrev = Luma;

	Cplx = MApi_XC_R2BYTEMSK(REG_SC_BK2A_78_L, 0xFFFF);
	Cplx = (CplxPrev*3 + Cplx ) >> 2;
	CplxPrev = Cplx;

	{
		LumaCheck = Luma >> 8;
		CplxCheck = Cplx >> 8;
		lumaAgcGain = LumaAGCLUT(pInstance, LumaCheck );
		cplxLumaPatch = ( CplxCheck > ( LumaCheck * lumaAgcGain ) >> 3 ) ? true : false;
	}

	if( ( isUkosabihsMv0 && !isMovingImage) || isMcFilmHd )
		featherRatio = 5;
	else if( !isUkosabihsMotion && isMovingImage )
		featherRatio = 2;
	else
		featherRatio = 4;

	if((exactFeatherCnt > 0x20)&&( exactFeatherCnt > (_hpfHdCntReport*featherRatio)))
	{
		if(HD_ConfirmCnt > (0xFF - SD_SPEED))
			HD_ConfirmCnt = 0xFF;
		else
			HD_ConfirmCnt = HD_ConfirmCnt + SD_SPEED;
	}
	else
	{
		if(HD_ConfirmCnt < 1)
			HD_ConfirmCnt = 0;
		else
			HD_ConfirmCnt = HD_ConfirmCnt - 1;
	}

	if( HD_ConfirmCnt > 0x80 )
		HD_ConfirmCnt_Reg = HD_ConfirmCnt - 0x80;
	else
		HD_ConfirmCnt_Reg = 0;

	HD_ConfirmCnt_Reg = (HD_ConfirmCnt_Reg*HDSDD_SPEED) >> HDSDD_DIV;

	if( HD_ConfirmCnt_Reg > 0x3F )
		HD_ConfirmCnt_Reg = 0x3F;

	if( cplxLumaPatch )
	{
		HD_ConfirmCnt_Reg_post = 0;
	}
	else if( isWithoutHsignal )
	{
		HD_ConfirmCnt_Reg_post = 0;
	}
	else if( (isHBurst || isHBurstMoving || isUkosabihsMotion ) && HD_ConfirmCnt_Reg != 0 )
	{
		if( HD_ConfirmCnt_Reg_post < 8 )
			HD_ConfirmCnt_Reg_post = 0;
		else
			HD_ConfirmCnt_Reg_post = HD_ConfirmCnt_Reg_post - 8;
	}
	else if( u8MaxPixel < 25 && HD_ConfirmCnt_Reg != 0 )
	{
		if( HD_ConfirmCnt_Reg_post < 8 )
			HD_ConfirmCnt_Reg_post = 0;
		else
			HD_ConfirmCnt_Reg_post = HD_ConfirmCnt_Reg_post - 8;
	}
	else
		HD_ConfirmCnt_Reg_post = HD_ConfirmCnt_Reg;

	//121 weight

	MApi_XC_W2BYTEMSK(REG_SC_BK23_54_L, (MS_U16)HD_ConfirmCnt_Reg_post, 0x001F);

}

void MDrv_SC_Edison_NEW_SDHD_DETECT_driver(void *pInstance)
{
    MS_U32 HDcnt_Act = 0;
    MS_U32 FeatherCnt = 0;
    MS_U32 SDcnt_Act = 0;
	MS_U32 NEW_FeatherCnt = 0;
	MS_U32 HD_ConfirmCnt_Reg = 0;

	static MS_U32 PreHDcnt_Act = 0;
	static MS_U32 PreSDcnt_Act = 0;
	static MS_U32 PreFeatherCnt = 0;
	static MS_U32 PreNEW_FeatherCnt = 0;
	static MS_U16 HD_ConfirmCnt = 0;//0xFF; // 0x7FFF == 0

	HDcnt_Act = (MApi_XC_R2BYTE(REG_SC_BK02_75_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_74_L);
	HDcnt_Act = ( PreHDcnt_Act*3 + HDcnt_Act ) >> 2;
	PreHDcnt_Act = HDcnt_Act;

	SDcnt_Act = (MApi_XC_R2BYTE(REG_SC_BK02_77_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_76_L);
	SDcnt_Act = ( PreSDcnt_Act*3 + SDcnt_Act ) >> 2;
	PreSDcnt_Act = SDcnt_Act;

	FeatherCnt = MApi_XC_R2BYTE(REG_SC_BK23_56_L); //New add for A1 chip
	FeatherCnt = ( PreFeatherCnt*3 + FeatherCnt ) >> 2;
	PreFeatherCnt = FeatherCnt;

	NEW_FeatherCnt = (MApi_XC_R2BYTE(REG_SC_BK23_6D_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK23_6C_L);
	NEW_FeatherCnt = ( PreNEW_FeatherCnt*3 + NEW_FeatherCnt ) >> 2;
	PreNEW_FeatherCnt = NEW_FeatherCnt;

	if( (NEW_FeatherCnt > 0x80) && (SDcnt_Act > 0x1800) && (NEW_FeatherCnt < 0xF00) && (HDcnt_Act < 0x3000))
	{
		if(HD_ConfirmCnt > (0xFF - SD_SPEED))
			HD_ConfirmCnt = 0xFF;
		else
			HD_ConfirmCnt = HD_ConfirmCnt + SD_SPEED;
	}
	else
	{
		if(HD_ConfirmCnt < 1)
			HD_ConfirmCnt = 0;
		else
			HD_ConfirmCnt = HD_ConfirmCnt - 1;
	}

	if( HD_ConfirmCnt > 0x80 )
		HD_ConfirmCnt_Reg = HD_ConfirmCnt - 0x80;
	else
		HD_ConfirmCnt_Reg = 0;

	HD_ConfirmCnt_Reg = (HD_ConfirmCnt_Reg*HDSDD_SPEED) >> HDSDD_DIV;

	if( HD_ConfirmCnt_Reg > 0x3F )
		HD_ConfirmCnt_Reg = 0x3F;

	//121 weight
	MApi_XC_W2BYTEMSK(REG_SC_BK23_54_L, (MS_U16)HD_ConfirmCnt_Reg, 0x003F);
}

#define TIMEID_hpfTolerantReport 2
MS_U32 MDrv_SC_hpfTolerantReport(void *pInstance, const MS_U8 _timeSharingID, const XC_HpfDefaultRegSetting defaultReg, MS_U32 *_hpfHdTolerantReport, const MS_U8 timingSegment )
{
	static MS_U32 _return_sd = 0;
	static MS_U32 _return_hd = 0;

	if( _timeSharingID == TIMEID_hpfTolerantReport )
	{
		if( timingSegment == 0 )
		{
			MDrv_SC_hpfDefaultRelod(pInstance, defaultReg );
		//else if( timingSegment == 1 ) /* Set Regiater */
		//{

			MApi_XC_W2BYTEMSK(REG_SC_BK02_72_L, 0x0100, 0xFFFF);
			MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, (0x02)<<12, 0x7000);

			MApi_XC_W2BYTEMSK(REG_SC_BK02_71_L, 0x0280, 0xFFFF);     /*for HD repot*/
			MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, (0x06)<<8, 0x0700);  /*for HD repot*/

			MApi_XC_W2BYTEMSK(REG_SC_BK02_73_L, 0x0000 , 0x003F); /* underflow */
			MApi_XC_W2BYTEMSK(REG_SC_BK02_73_L, 0x3F00, 0x3F00); /* overflow */
		}
		else if ( timingSegment > 1)
		{
			_return_sd =  (MApi_XC_R2BYTE(REG_SC_BK02_77_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_76_L);

			_return_hd =  (MApi_XC_R2BYTE(REG_SC_BK02_75_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_74_L);


		}
	}
	else
	{
		//MApi_XC_W2BYTEMSK(REG_SC_BK02_72_L, defaultReg.HDSDD1_det_mode_threshold, 0xFFFF);
		//MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, (defaultReg.HDSDD1_det_mode_shift)<<12, 0x7000);

		//MApi_XC_W2BYTEMSK(REG_SC_BK02_71_L, defaultReg.HDSDD0_det_mode_threshold, 0xFFFF);  /*for HD repot*/
		//MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, (defaultReg.HDSDD0_det_mode_shift)<<8, 0x0700); /*for HD repot*/

		//MApi_XC_W2BYTEMSK(REG_SC_BK02_73_L, defaultReg.HDSDD_underflow_threshold , 0x003F); /* underflow */
		//MApi_XC_W2BYTEMSK(REG_SC_BK02_73_L, (defaultReg.HDSDD_overflow_threshold)<<8, 0x3F00); /* overflow */
	}

	*_hpfHdTolerantReport = _return_hd;

	return _return_sd;
}

#define TIMEID_dynamicHDfilterBaseReport 1
MS_U32 MDrv_SC_dynamicHDfilterBaseReport(void *pInstance, const MS_U8 _timeSharingID, const MS_U32 u32MotionValue, const MS_BOOL isUkosabihsMv0, const XC_HpfDefaultRegSetting defaultReg, const MS_U8 timingSegment  )
{

	MS_BOOL isStillImage = ( u32MotionValue < 10 ) ? true : false;
	MS_U32 hdRepot= 0;
	static MS_U32 _return = 0;

	if( _timeSharingID == TIMEID_dynamicHDfilterBaseReport )
	{
		if( timingSegment == 0 )
		{
			MDrv_SC_hpfDefaultRelod(pInstance, defaultReg );
		//else if( timingSegment == 1 ) /* Set Regiater */
		//{
			if( ( isUkosabihsMv0 && isStillImage) )
			{
				MApi_XC_W2BYTEMSK(REG_SC_BK02_71_L, 0x0100, 0xFFFF);
				MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, (0x02)<<8, 0x0700);
			}
			else
			{
				MApi_XC_W2BYTEMSK(REG_SC_BK02_71_L, 0x0190, 0xFFFF);
				MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, (0x04)<<8, 0x0700);
			}
		}
		else if ( timingSegment > 1)
		{
			hdRepot =  (MApi_XC_R2BYTE(REG_SC_BK02_75_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_74_L);

			_return = hdRepot;
		}
	}
	else
	{
		//MApi_XC_W2BYTEMSK(REG_SC_BK02_71_L, defaultReg.HDSDD0_det_mode_threshold, 0xFFFF);  /*for HD repot*/
		//MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, (defaultReg.HDSDD0_det_mode_shift)<<8, 0x0700); /*for HD repot*/
		//MApi_XC_W2BYTEMSK(REG_SC_BK02_71_L, 0x0190, 0xFFFF);
		//MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, (0x04)<<8, 0x0700);
	}
	return _return;
}

#define TIMEID_FilterBaseReport 0
MS_U8 MDrv_SC_SDHD_FilterBaseReport(void *pInstance, const MS_U8 _timeSharingID, MS_U32 *_hpfHdCntReport, MS_U32 *_hpfSdCntReport, const XC_HpfDefaultRegSetting defaultReg,  const MS_U8 timingSegment  , MS_U32 *hdNactCntRet)
{
    MS_U8 u8Luma = 0;
    MS_U16 u16MinPixel = 0,u16MaxPixel = 0, MaxMinPixel = 0;
    static MS_U32 HDcnt_Act = 0, hdNactCnt = 0;
	static MS_U32 SDcnt_Act = 0;
    MS_U32 HDcnt_Act_Cur = 0;
    MS_U32 SDcnt_Act_Cur = 0;
    MS_U32 hdNactCntCur;
	static MS_U16 HD_ConfirmCnt = 0;//xFF; // 0x7FFF == 0
	static MS_U8 u8PrevLuma = 0;
	static MS_U16 u16PrevMinPixel = 0;
	static MS_U16 u16PrevMaxPixel = 0;
    MS_U32 HD_ConfirmCnt_Reg = 0;
    MS_U32 HDSDD_Det_threshold = 0;
	static MS_U8 _return = 0;

	if( _timeSharingID == TIMEID_FilterBaseReport )
	{
		if( timingSegment == 0 )
		{
			MDrv_SC_hpfDefaultRelod(pInstance, defaultReg );
		//else if( timingSegment == 1 ) /* Set Regiater */
		//{
			u8Luma=(MS_U32)((MS_U32)u8PrevLuma*3 + MApi_XC_DLC_GetAverageValue())>>2;
//			int AVG_LUMA = MApi_XC_DLC_GetAverageValue();
			u8PrevLuma = u8Luma;

			u16MaxPixel = MApi_XC_R2BYTEMSK(REG_SC_BK1A_0B_L, 0x00FF);
			u16MaxPixel = (u16PrevMaxPixel*3 + u16MaxPixel ) >> 2;
			u16PrevMaxPixel = u16MaxPixel;

			u16MinPixel = MApi_XC_R2BYTEMSK(REG_SC_BK1A_0B_L, 0xFF00)>>8;
			u16MinPixel = (u16PrevMinPixel*3 + u16MinPixel ) >> 2;
			u16PrevMinPixel = u16MinPixel;

			MaxMinPixel = u16MaxPixel - u16MinPixel;

			if(u8Luma>=0xFF)
				u8Luma = 0xFF;

			HDSDD_Det_threshold = ((MaxMinPixel*6)>>2) + ((u8Luma*5)>>2);

			if(HDSDD_Det_threshold >=0xFFFF)
				HDSDD_Det_threshold=0xFFFF;

			MApi_XC_W2BYTEMSK(REG_SC_BK02_71_L, (MS_U16)HDSDD_Det_threshold, 0xFFFF);
			MApi_XC_W2BYTEMSK(REG_SC_BK02_72_L, (MS_U16)(HDSDD_Det_threshold>>1), 0xFFFF);
		}
		else if( timingSegment == 1 )
		{

		}
        else /* Run */
        {
            HDcnt_Act_Cur =  (MApi_XC_R2BYTE(REG_SC_BK02_75_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_74_L);
            SDcnt_Act_Cur =  (MApi_XC_R2BYTE(REG_SC_BK02_77_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_76_L);
            hdNactCntCur =  (MApi_XC_R2BYTE(REG_SC_BK02_79_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_78_L)
            + (MApi_XC_R2BYTE(REG_SC_BK82_79_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK82_78_L);
            
            HDcnt_Act = (HDcnt_Act_Cur+ HDcnt_Act) >> 1;
            SDcnt_Act = (SDcnt_Act_Cur+ SDcnt_Act) >> 1;
            
            *_hpfHdCntReport = HDcnt_Act;
            *_hpfSdCntReport = SDcnt_Act;
            *hdNactCntRet = hdNactCnt = (hdNactCntCur+ hdNactCnt) >> 1;
            
            if( (HDcnt_Act > HD_ACT_TH) && (SDcnt_Act != 0) )
            {
            	if(HD_ConfirmCnt < HD_SPEED)
            		HD_ConfirmCnt = 0;
            	else
            		HD_ConfirmCnt = HD_ConfirmCnt - HD_SPEED;
            }
            else if( SDcnt_Act != 0 )
            {
            	if(HD_ConfirmCnt > (0x80 - SD_SPEED))
            		HD_ConfirmCnt = 0x80;
            	else
            		HD_ConfirmCnt = HD_ConfirmCnt + SD_SPEED;
            }
            else
            {
            	if(HD_ConfirmCnt < 1)
            		HD_ConfirmCnt = 0;
            	else
            		HD_ConfirmCnt = HD_ConfirmCnt - 1;
            }
            
            if( HD_ConfirmCnt > 0x20 )
            	HD_ConfirmCnt_Reg = HD_ConfirmCnt - 0x20;
            else
            	HD_ConfirmCnt_Reg = 0;
            
            HD_ConfirmCnt_Reg = (HD_ConfirmCnt_Reg*HDSDD_SPEED) >> HDSDD_DIV;
            
            if( HD_ConfirmCnt_Reg > 0x3F )
            	HD_ConfirmCnt_Reg = 0x3F;
            
            _return = HD_ConfirmCnt_Reg;
        }
	}
	else
	{
		//  return to the original condition
		//MApi_XC_W2BYTEMSK(REG_SC_BK02_71_L, defaultReg.HDSDD0_det_mode_threshold , 0xFFFF);
		//MApi_XC_W2BYTEMSK(REG_SC_BK02_72_L, defaultReg.HDSDD1_det_mode_threshold , 0xFFFF);
	}

	return _return;
}

#define TIMEID_FilterOneReport 3
MS_U8 MDrv_SC_SDHD_FilterOneReport(void *pInstance, const MS_U8 _timeSharingID, MS_U32 *_hpfHdCntReport, MS_U32 *_hpfSdCntReport, const XC_HpfDefaultRegSetting defaultReg,  const MS_U8 timingSegment  )
{
	MS_U8 u8Luma;
	MS_U16 u16MinPixel,u16MaxPixel, MaxMinPixel;
	static MS_U32 HDcnt_Act = 0;
	static MS_U32 SDcnt_Act = 0;
	MS_U32 HDcnt_Act_Cur;
	MS_U32 SDcnt_Act_Cur;
	static MS_U16 HD_ConfirmCnt = 0;//xFF; // 0x7FFF == 0
	static MS_U8 u8PrevLuma = 0;
	static MS_U16 u16PrevMinPixel = 0;
	static MS_U16 u16PrevMaxPixel = 0;
	MS_U32 HD_ConfirmCnt_Reg;
	MS_U32 HDSDD_Det_threshold;
	static MS_U8 _return = 0;

	if( _timeSharingID == TIMEID_FilterOneReport )
	{
		if( timingSegment == 0 )
		{
			MDrv_SC_hpfDefaultRelod(pInstance, defaultReg );
			u8Luma=(MS_U32)((MS_U32)u8PrevLuma*3 + MApi_XC_DLC_GetAverageValue())>>2;

			u8PrevLuma = u8Luma;

			u16MaxPixel = MApi_XC_R2BYTEMSK(REG_SC_BK1A_0B_L, 0x00FF);
			u16MaxPixel = (u16PrevMaxPixel*3 + u16MaxPixel ) >> 2;
			u16PrevMaxPixel = u16MaxPixel;

			u16MinPixel = MApi_XC_R2BYTEMSK(REG_SC_BK1A_0B_L, 0xFF00)>>8;
			u16MinPixel = (u16PrevMinPixel*3 + u16MinPixel ) >> 2;
			u16PrevMinPixel = u16MinPixel;

			MaxMinPixel = u16MaxPixel - u16MinPixel;

			if(u8Luma>=0xFF)
				u8Luma = 0xFF;

			HDSDD_Det_threshold = ((MaxMinPixel*6)>>2) + ((u8Luma*5)>>2);

			if(HDSDD_Det_threshold >=0xFFFF)
				HDSDD_Det_threshold=0xFFFF;

			MApi_XC_W2BYTEMSK(REG_SC_BK02_71_L, (MS_U16)HDSDD_Det_threshold, 0xFFFF);
			MApi_XC_W2BYTEMSK(REG_SC_BK02_72_L, (MS_U16)(HDSDD_Det_threshold>>1), 0xFFFF);

			MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x1000, 0x7000);
			MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x0100, 0x0700); /*for HD repot*/

			MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x0000 , 0x0001); /* mode */
			MApi_XC_W2BYTEMSK(REG_SC_BK02_70_L, 0x0000 , 0x0010); /* mode */


	       MApi_XC_W2BYTEMSK(REG_SC_BK02_73_L, 0x000C, 0x003F); /* underflow */
	       MApi_XC_W2BYTEMSK(REG_SC_BK02_73_L, 0x3200, 0x3F00); /* overflow */


		}
		else if( timingSegment == 1 )
		{

		}
		else /* Run */
		{
			HDcnt_Act_Cur =  (MApi_XC_R2BYTE(REG_SC_BK02_75_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_74_L);
			SDcnt_Act_Cur =  (MApi_XC_R2BYTE(REG_SC_BK02_77_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_76_L);

			HDcnt_Act = (HDcnt_Act_Cur+ HDcnt_Act*3) >> 2;
			SDcnt_Act = (SDcnt_Act_Cur+ SDcnt_Act*3) >> 2;

			*_hpfHdCntReport = HDcnt_Act;
			*_hpfSdCntReport = SDcnt_Act;


			if( (HDcnt_Act > HD_ACT_TH) && (SDcnt_Act != 0) )
			{
				if(HD_ConfirmCnt < HD_SPEED)
					HD_ConfirmCnt = 0;
				else
					HD_ConfirmCnt = HD_ConfirmCnt - HD_SPEED;
			}
			else if( SDcnt_Act != 0 )
			{
				if(HD_ConfirmCnt > (0x80 - SD_SPEED))
					HD_ConfirmCnt = 0x80;
				else
					HD_ConfirmCnt = HD_ConfirmCnt + SD_SPEED;
			}
			else
			{
				if(HD_ConfirmCnt < 1)
					HD_ConfirmCnt = 0;
				else
					HD_ConfirmCnt = HD_ConfirmCnt - 1;
			}

			if( HD_ConfirmCnt > 0x20 )
				HD_ConfirmCnt_Reg = HD_ConfirmCnt - 0x20;
			else
				HD_ConfirmCnt_Reg = 0;

			HD_ConfirmCnt_Reg = (HD_ConfirmCnt_Reg*HDSDD_SPEED) >> HDSDD_DIV;

			if( HD_ConfirmCnt_Reg > 0x3F )
				HD_ConfirmCnt_Reg = 0x3F;

			_return = HD_ConfirmCnt_Reg;
		}
	}
	else
	{
		//  return to the original condition
		//MApi_XC_W2BYTEMSK(REG_SC_BK02_71_L, defaultReg.HDSDD0_det_mode_threshold , 0xFFFF);
		//MApi_XC_W2BYTEMSK(REG_SC_BK02_72_L, defaultReg.HDSDD1_det_mode_threshold , 0xFFFF);
	}

	return _return;
}

MS_U8 MDrv_SC_Edison_NEW_SDHD_DETECT_report(void *pInstance)
{
	MS_U32 HDcnt_Act[5] = {0,0,0,0,0};
	MS_U32 SDcnt_Act[5] = {0,0,0,0,0};
	MS_U32 FeatherCnt[5] = {0,0,0,0,0};
	MS_U32 SDcnt_NonAct[5] = {0,0,0,0,0};
	static MS_U32 HDcnt_Act_prv[5] = {0,0,0,0,0};
	static MS_U32 SDcnt_Act_prv[5] = {0,0,0,0,0};
	static MS_U32 FeatherCnt_prv[5] = {0,0,0,0,0};
	static MS_U32 SDcnt_NonAct_prv[5] = {0,0,0,0,0};
	static MS_U8 SD2HD_CNT = 0;
	static MS_U16 HD_ConfirmCnt[5] = {0,0,0,0,0};//xFF;
	static MS_U32 HD_ConfirmCnt_Reg[5] = {0,0,0,0,0};
	static MS_U32 HD_ConfirmCnt_Reg_f_prv = 0;
	MS_U32 HD_ConfirmCnt_Reg_f = 0;

	if( SD2HD_CNT == 0 )
	{
		//empty stage
	}
	else if( SD2HD_CNT == 1 )
	{
		//Detect corner logo
		//left-side
		HDcnt_Act[0] = (MApi_XC_R2BYTE(REG_SC_BK02_75_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_74_L);
		SDcnt_Act[0] = (MApi_XC_R2BYTE(REG_SC_BK02_77_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_76_L);
		SDcnt_NonAct[0] = (MApi_XC_R2BYTE(REG_SC_BK02_7B_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_7A_L);
		FeatherCnt[0]   =  MApi_XC_R2BYTE(REG_SC_BK23_56_L);

		//IIR result
		HDcnt_Act[0]    = (HDcnt_Act_prv[0]*3 + HDcnt_Act[0])/4;
		SDcnt_Act[0]    = (SDcnt_Act_prv[0]*3 + SDcnt_Act[0])/4;
		SDcnt_NonAct[0] = (SDcnt_NonAct_prv[0]*3 + SDcnt_NonAct[0])/4;
		FeatherCnt[0]   = (FeatherCnt_prv[0]*3 + FeatherCnt[0])/4;
		HDcnt_Act_prv[0]    = HDcnt_Act[0];
		SDcnt_Act_prv[0]    = SDcnt_Act[0];
		SDcnt_NonAct_prv[0] = SDcnt_NonAct[0];
		FeatherCnt_prv[0]   = FeatherCnt[0];

		if((HDcnt_Act[0] > HD_ACT_TH) && (SDcnt_Act[0] != 0)) //debugging
		{
			if(HD_ConfirmCnt[0] < HD_SPEED*5)
				HD_ConfirmCnt[0] = 0;
			else
				HD_ConfirmCnt[0] = HD_ConfirmCnt[0] - HD_SPEED*5;
		}
		else if( SDcnt_Act[0] != 0 ) //w/o consider feathering
		{
			if(HD_ConfirmCnt[0] > (0xFF - SD_SPEED*5))
				HD_ConfirmCnt[0] = 0xFF;
			else
				HD_ConfirmCnt[0] = HD_ConfirmCnt[0] + SD_SPEED*5;
		}
		else
		{
			if(HD_ConfirmCnt[0] < 1)
				HD_ConfirmCnt[0] = 0;
			else
				HD_ConfirmCnt[0] = HD_ConfirmCnt[0] - 1;
		}

		if( HD_ConfirmCnt[0] > 0x80 )
			HD_ConfirmCnt_Reg[0] = HD_ConfirmCnt[0] - 0x80;
		else
			HD_ConfirmCnt_Reg[0] = 0;

		HD_ConfirmCnt_Reg[0] = (HD_ConfirmCnt_Reg[0]*HDSDD_SPEED) >> HDSDD_DIV;

		if( HD_ConfirmCnt_Reg[0] > 0x3F )
			HD_ConfirmCnt_Reg[0] = 0x3F;

		//for SD2HD_CNT == 2 window size
		//MApi_XC_W2BYTEMSK(REG_SC_BK02_7C_L, 480, 0x1FFF);
		//MApi_XC_W2BYTEMSK(REG_SC_BK02_7D_L, 1440, 0x1FFF);
	}
	else if( SD2HD_CNT == 2 )
	{
		//empty stage
	}
	else if( SD2HD_CNT == 3 )
	{
		HDcnt_Act[1] = (MApi_XC_R2BYTE(REG_SC_BK02_75_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_74_L);
		SDcnt_Act[1] = (MApi_XC_R2BYTE(REG_SC_BK02_77_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_76_L);
		SDcnt_NonAct[1] = (MApi_XC_R2BYTE(REG_SC_BK02_7B_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_7A_L);
		FeatherCnt[1]   =  MApi_XC_R2BYTE(REG_SC_BK23_56_L);

		//IIR result
		HDcnt_Act[1]    = (HDcnt_Act_prv[1]*3 + HDcnt_Act[1])/4;
		SDcnt_Act[1]    = (SDcnt_Act_prv[1]*3 + SDcnt_Act[1])/4;
		SDcnt_NonAct[1] = (SDcnt_NonAct_prv[1]*3 + SDcnt_NonAct[1])/4;
		FeatherCnt[1]   = (FeatherCnt_prv[1]*3 + FeatherCnt[1])/4;
		HDcnt_Act_prv[1]    = HDcnt_Act[1];
		SDcnt_Act_prv[1]    = SDcnt_Act[1];
		SDcnt_NonAct_prv[1] = SDcnt_NonAct[1];
		FeatherCnt_prv[1]   = FeatherCnt[1];

		if( (HDcnt_Act[1] > HD_ACT_TH) && (SDcnt_Act[1] != 0) )
		{
			if(HD_ConfirmCnt[1] < HD_SPEED*5)
				HD_ConfirmCnt[1] = 0;
			else
				HD_ConfirmCnt[1] = HD_ConfirmCnt[1] - HD_SPEED*5;

		}
		else if( SDcnt_Act[1] != 0 ) //w/o consider feathering
		{
			if(HD_ConfirmCnt[1] > (0xFF - SD_SPEED*5))
				HD_ConfirmCnt[1] = 0xFF;
			else
				HD_ConfirmCnt[1] = HD_ConfirmCnt[1] + SD_SPEED*5;
		}
		else
		{
			if(HD_ConfirmCnt[1] < 1)
				HD_ConfirmCnt[1] = 0;
			else
				HD_ConfirmCnt[1] = HD_ConfirmCnt[1] - 1;
		}

		if( HD_ConfirmCnt[1] > 0x80 )
			HD_ConfirmCnt_Reg[1] = HD_ConfirmCnt[1] - 0x80;
		else
			HD_ConfirmCnt_Reg[1] = 0;

		HD_ConfirmCnt_Reg[1] = (HD_ConfirmCnt_Reg[1]*HDSDD_SPEED) >> HDSDD_DIV;

		if( HD_ConfirmCnt_Reg[1] > 0x3F )
			HD_ConfirmCnt_Reg[1] = 0x3F;

		//for SD2HD_CNT == 4 window size
		//MApi_XC_W2BYTEMSK(REG_SC_BK02_7C_L, 640, 0x1FFF);
		//MApi_XC_W2BYTEMSK(REG_SC_BK02_7D_L, 1280, 0x1FFF);
	}
	else if( SD2HD_CNT == 4 )
	{
		//empty stage
	}
	else if( SD2HD_CNT == 5 )
	{
		HDcnt_Act[2] = (MApi_XC_R2BYTE(REG_SC_BK02_75_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_74_L);
		SDcnt_Act[2] = (MApi_XC_R2BYTE(REG_SC_BK02_77_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_76_L);
		SDcnt_NonAct[2] = (MApi_XC_R2BYTE(REG_SC_BK02_7B_L) << 16) + MApi_XC_R2BYTE(REG_SC_BK02_7A_L);
		FeatherCnt[2]   =  MApi_XC_R2BYTE(REG_SC_BK23_56_L);

		//IIR result
		HDcnt_Act[2]    = (HDcnt_Act_prv[2]*3 + HDcnt_Act[2])/4;
		SDcnt_Act[2]    = (SDcnt_Act_prv[2]*3 + SDcnt_Act[2])/4;
		SDcnt_NonAct[2] = (SDcnt_NonAct_prv[2]*3 + SDcnt_NonAct[2])/4;
		FeatherCnt[2]   = (FeatherCnt_prv[2]*3 + FeatherCnt[2])/4;
		HDcnt_Act_prv[2]    = HDcnt_Act[2];
		SDcnt_Act_prv[2]    = SDcnt_Act[2];
		SDcnt_NonAct_prv[2] = SDcnt_NonAct[2];
		FeatherCnt_prv[2]   = FeatherCnt[2];

		if( (HDcnt_Act[2] > HD_ACT_TH) && (SDcnt_Act[2] != 0) )
		{
			if(HD_ConfirmCnt[2] < HD_SPEED*5)
				HD_ConfirmCnt[2] = 0;
			else
				HD_ConfirmCnt[2] = HD_ConfirmCnt[2] - HD_SPEED*5;

		}
		else if( SDcnt_Act[2] != 0 ) //w/o consider feathering
		{
			if(HD_ConfirmCnt[2] > (0xFF - SD_SPEED*5))
				HD_ConfirmCnt[2] = 0xFF;
			else
				HD_ConfirmCnt[2] = HD_ConfirmCnt[2] + SD_SPEED*5;
		}
		else
		{
			if(HD_ConfirmCnt[2] < 1)
				HD_ConfirmCnt[2] = 0;
			else
				HD_ConfirmCnt[2] = HD_ConfirmCnt[2] - 1;
		}

		if( HD_ConfirmCnt[2] > 0x80 )
			HD_ConfirmCnt_Reg[2] = HD_ConfirmCnt[2] - 0x80;
		else
			HD_ConfirmCnt_Reg[2] = 0;

		HD_ConfirmCnt_Reg[2] = (HD_ConfirmCnt_Reg[2]*HDSDD_SPEED) >> HDSDD_DIV;

		if( HD_ConfirmCnt_Reg[2] > 0x3F )
			HD_ConfirmCnt_Reg[2] = 0x3F;

		//MApi_XC_W2BYTEMSK(REG_SC_BK02_7C_L, 40, 0x1FFF);
		//MApi_XC_W2BYTEMSK(REG_SC_BK02_7D_L, 1880, 0x1FFF);
	}

	if( SD2HD_CNT == 5 )
	{
		HD_ConfirmCnt_Reg_f = HD_ConfirmCnt_Reg[2] + ((HD_ConfirmCnt_Reg[0] + HD_ConfirmCnt_Reg[1]*3 )>>2);

		HD_ConfirmCnt_Reg_f = (HD_ConfirmCnt_Reg_f_prv*3 + HD_ConfirmCnt_Reg_f+2)/4;

		if( HD_ConfirmCnt_Reg_f >= 0x3E )
			HD_ConfirmCnt_Reg_f = 0x3F;
		else if( HD_ConfirmCnt_Reg_f <= 0x02 )
			HD_ConfirmCnt_Reg_f = 0;

		HD_ConfirmCnt_Reg_f_prv = HD_ConfirmCnt_Reg_f;
	}
	else
	{
		HD_ConfirmCnt_Reg_f = HD_ConfirmCnt_Reg_f_prv;
		//HD_ConfirmCnt_Reg_f_prv = HD_ConfirmCnt_Reg_f_prv;
	}

	//for next frame start
	if( SD2HD_CNT == 5 )
		SD2HD_CNT = 0;
	else
		SD2HD_CNT++;

	return HD_ConfirmCnt_Reg_f;
}

void MDrv_UFSC_PQ_BNF_status( XC_BackgroundState *backgroundState, MS_BOOL isOpFrameChanged  ) //Left half histogram data
{
    static MS_U16 iir_Ymax = 0;
    static MS_U16 iir_Umax = 0;
    static MS_U16 iir_Vmax = 0;
    static MS_U32 iir_pbgrate = 0;
    static MS_U32 iir_flatSkinCnt = 0;
    static MS_U32 iir_cplxCnt = 0;
    static MS_U32 iir_prvBgCnt = 0;
    static MS_U32 iir_skinRawCnt = 0;
    static MS_U32 iir_cplxRawCnt = 0;
    static MS_U32 iir_dswRawCnt = 0;

    MS_U32 BNF_Yhist[32], BNF_Uhist[32], BNF_Vhist[32];
    static MS_BOOL SetStatus = false;
    MS_U16 i, Ymax, Umax, Vmax;
    MS_U32 Ymax_cnt, Umax_cnt, Vmax_cnt, maxYUVcnt;
    MS_U32 totalPixel, pbgrate;

    MApi_XC_Write2ByteMask(REG_FSC_BK4D_39_L, 0 , 0x0FFF );
    MApi_XC_Write2ByteMask(REG_FSC_BK4D_3B_L, frameWidth/2-1 , 0x0FFF );
    MApi_XC_Write2ByteMask(REG_FSC_BK4D_3C_L, frameHeight-1 , 0x0FFF );

    MApi_XC_Write2ByteMask(REG_FSC_BKCD_39_L, GuardBandWidth , 0x0FFF );
    MApi_XC_Write2ByteMask(REG_FSC_BKCD_3B_L, GuardBandWidth+ frameWidth/2-1 , 0x0FFF );
    MApi_XC_Write2ByteMask(REG_FSC_BKCD_3C_L, frameHeight-1 , 0x0FFF );


    if ( isOpFrameChanged )
    {
        if( !SetStatus )
        {
            MApi_XC_Write2ByteMask(REG_FSC_BK4D_51_L, 0x0000 , 0x0004 );
            MApi_XC_Write2ByteMask(REG_FSC_BK4D_51_L, 0x0004 , 0x0004 );

            SetStatus = true;
        }
        else if (( MDrv_UFSC_Read2Byte(REG_FSC_BK4D_51_L)& 0x0008 ) &&  SetStatus )
        {
            // Y histogram
            MApi_XC_Write2ByteMask(REG_FSC_BK4D_51_L, 0x0000 , 0x0003 );
            for( i = 0 ; i < 32 ; ++i )
            {
                MS_U16 sh = ((i&0x3)<<2);
                            BNF_Yhist[i] = ( MDrv_UFSC_Read2Byte(REG_FSC_BK4D_58_L+ (i<<1) ) << 3 )
                    + ( (MDrv_UFSC_Read2ByteMSK(REG_FSC_BK4D_78_L + ((i>>2)<<1), (0x7000 >> sh))) >> (12-sh) )
                    + ( MDrv_UFSC_Read2Byte( REG_FSC_BKCD_58_L + (i<<1) ) << 3 )
                    + ( (MDrv_UFSC_Read2ByteMSK( REG_FSC_BKCD_78_L + ((i>>2)<<1), (0x7000 >> sh))) >> (12-sh) );
            }

#if 0
            totalPixel = 0;
            printf(" --- Y historgam -- \n");
            for ( i = 0 ; i < 32; i ++)
            {
                totalPixel = totalPixel + BNF_Yhist[i];
                printf("Yhist[%d] = %d\n", i, BNF_Yhist[i]);
            }
            printf("total pixel = %d\n", totalPixel );
#endif
            // U histogram
            MApi_XC_Write2ByteMask(REG_FSC_BK4D_51_L, 0x0001 , 0x0003 );
            for( i = 0 ; i < 32 ; ++i )
            {
                MS_U16 sh = ((i&0x3)<<2);
                            BNF_Uhist[i] = ( MDrv_UFSC_Read2Byte( REG_FSC_BK4D_58_L + (i<<1) ) << 3 )
                    + ( (MDrv_UFSC_Read2ByteMSK( REG_FSC_BK4D_78_L + ((i>>2)<<1), (0x7000 >> sh))) >> (12-sh) )
                    + ( MDrv_UFSC_Read2Byte( REG_FSC_BKCD_58_L + (i<<1) ) << 3 )
                    + ( (MDrv_UFSC_Read2ByteMSK( REG_FSC_BKCD_78_L + ((i>>2)<<1), (0x7000 >> sh))) >> (12-sh) );
            }
#if 0
            totalPixel = 0;
            printf(" --- U historgam -- \n");
            for ( i = 0 ; i < 32; i ++)
            {
                totalPixel = totalPixel + BNF_Uhist[i];
                printf("Uhist[%d] = %d\n", i, BNF_Uhist[i]);
            }
            printf("total pixel = %d\n", totalPixel );
#endif


            // V histogram
            MApi_XC_Write2ByteMask(REG_FSC_BK4D_51_L, 0x0002 , 0x0003 );
            for( i = 0 ; i < 32 ; ++i )
            {
                MS_U16 sh = ((i&0x3)<<2);
                            BNF_Vhist[i] = ( MDrv_UFSC_Read2Byte( REG_FSC_BK4D_58_L + (i<<1) ) << 3 )
                    + ( (MDrv_UFSC_Read2ByteMSK( REG_FSC_BK4D_78_L + ((i>>2)<<1), (0x7000 >> sh))) >> (12-sh) )
                    + ( MDrv_UFSC_Read2Byte( REG_FSC_BKCD_58_L + (i<<1) ) << 3 )
                    + ( (MDrv_UFSC_Read2ByteMSK( REG_FSC_BKCD_78_L + ((i>>2)<<1), (0x7000 >> sh))) >> (12-sh) );
            }
#if 0
            totalPixel = 0;
            printf(" --- V historgam -- \n");
            for ( i = 0 ; i < 32; i ++)
            {
                totalPixel = totalPixel + BNF_Vhist[i];
                printf("Vhist[%d] = %d\n", i, BNF_Vhist[i]);
            }
            printf("total pixel = %d\n", totalPixel );
#endif


            Ymax = 0;
            Umax = 0;
            Vmax = 0;
            Ymax_cnt = 0;
            Umax_cnt = 0;
            Vmax_cnt = 0;

            for ( i = 0; i< 32; i++ )
            {
                if ( BNF_Yhist[i] > Ymax_cnt )
                {
                    Ymax = i << 4;
                    Ymax_cnt = BNF_Yhist[i];
                }

                if ( BNF_Uhist[i] > Umax_cnt )
                {
                    Umax = i << 4;
                    Umax_cnt = BNF_Uhist[i];
                }

                if ( BNF_Vhist[i] > Vmax_cnt )
                {
                    Vmax = i << 4 ;
                    Vmax_cnt = BNF_Vhist[i];
                }

            }

            maxYUVcnt  = __min3(Ymax_cnt, Umax_cnt, Vmax_cnt);
            totalPixel = ( frameWidth* frameHeight ) >> 5;
            pbgrate = (totalPixel!=0) ? (maxYUVcnt*5000)/totalPixel : 0;

            Ymax = ((3*iir_Ymax)+ Ymax ) >> 2;
            iir_Ymax = Ymax;

            Umax = ((3*iir_Umax)+ Umax ) >> 2;
            iir_Umax = Umax;

            Vmax = ((3*iir_Vmax )+ Vmax ) >> 2;
            iir_Vmax = Vmax;

            pbgrate = ((iir_pbgrate*3 )+ pbgrate) >> 2;
            iir_pbgrate = pbgrate;

            Ymax = ( Ymax + 8 )>> 4;
            Umax = ( Umax + 8 )>> 4;
            Vmax = ( Vmax + 8 )>> 4;


#if 0
            printf(" max Ybin = %d, cnt = %d \n", Ymax, Ymax_cnt );
            printf(" max Ubin = %d, cnt = %d \n", Umax, Umax_cnt  );
            printf(" max Vbin = %d, cnt = %d \n", Vmax, Vmax_cnt  );
            printf(" totalPixel = %d \n", totalPixel );
            printf(" maxYUVcnt = %d \n", maxYUVcnt );
            printf(" pbgrate = %d \n", pbgrate );
#endif

            MApi_XC_Write2ByteMask(REG_FSC_BK4D_42_L, Ymax , 0x001F );
            MApi_XC_Write2ByteMask(REG_FSC_BK4D_42_L, Umax << 8 , 0x1F00 );
            MApi_XC_Write2ByteMask(REG_FSC_BK4D_43_L, Vmax , 0x001F );
            MApi_XC_Write2ByteMask(REG_FSC_BK4D_45_L, pbgrate , 0xDFFF );

            MS_U32 flatSkinCnt = ( (MDrv_UFSC_Read2ByteMSK(REG_FSC_BK4D_53_L, 0x0007 ) << 16) +  MDrv_UFSC_Read2Byte(REG_FSC_BK4D_52_L )
                        + (MDrv_UFSC_Read2ByteMSK(REG_FSC_BKCD_53_L, 0x0007 ) << 16) +  MDrv_UFSC_Read2Byte(REG_FSC_BKCD_52_L ) ) >> 1;
            MS_U32 cplxCnt = (( MDrv_UFSC_Read2ByteMSK(REG_FSC_BK4D_55_L, 0x0007 ) << 16) +  MDrv_UFSC_Read2Byte(REG_FSC_BK4D_54_L )
                        + (MDrv_UFSC_Read2ByteMSK(REG_FSC_BKCD_55_L, 0x0007 ) << 16) +  MDrv_UFSC_Read2Byte(REG_FSC_BKCD_54_L ) ) >> 1;
            MS_U32 prvBgCnt = ((MDrv_UFSC_Read2ByteMSK(REG_FSC_BK4D_57_L, 0x0007 ) << 16) +  MDrv_UFSC_Read2Byte(REG_FSC_BK4D_56_L )
                        + (MDrv_UFSC_Read2ByteMSK(REG_FSC_BKCD_57_L, 0x0007 ) << 16) +  MDrv_UFSC_Read2Byte(REG_FSC_BKCD_56_L ) ) >> 1;
            MS_U32 skinRawCnt = ((MDrv_UFSC_Read2ByteMSK(REG_FSC_BK4D_4B_L, 0x0007 ) << 16) +  MDrv_UFSC_Read2Byte(REG_FSC_BK4D_4A_L )
                        + (MDrv_UFSC_Read2ByteMSK(REG_FSC_BKCD_4B_L, 0x0007 ) << 16) +  MDrv_UFSC_Read2Byte(REG_FSC_BKCD_4A_L ) ) >> 1;
            MS_U32 cplxRawCnt = ((MDrv_UFSC_Read2ByteMSK(REG_FSC_BK4D_4D_L, 0x0007 ) << 16) +  MDrv_UFSC_Read2Byte(REG_FSC_BK4D_4C_L )
                        + (MDrv_UFSC_Read2ByteMSK(REG_FSC_BKCD_4D_L, 0x0007 ) << 16) +  MDrv_UFSC_Read2Byte(REG_FSC_BKCD_4C_L ) ) >> 1;
            MS_U32 dswRawCnt = ((MDrv_UFSC_Read2ByteMSK(REG_FSC_BK4D_4F_L, 0x0007 ) << 16) +    MDrv_UFSC_Read2Byte(REG_FSC_BK4D_4E_L )
                        + (MDrv_UFSC_Read2ByteMSK(REG_FSC_BKCD_4F_L, 0x0007 ) << 16) +  MDrv_UFSC_Read2Byte(REG_FSC_BKCD_4E_L ) ) >> 1;


            flatSkinCnt = ((iir_flatSkinCnt*3 )+ flatSkinCnt) >> 2;
            iir_flatSkinCnt = flatSkinCnt;

            cplxCnt = ((iir_cplxCnt*3 )+ cplxCnt) >> 2;
            iir_cplxCnt = cplxCnt;

            prvBgCnt = ((iir_prvBgCnt*3 )+ prvBgCnt) >> 2;
            iir_prvBgCnt = prvBgCnt;

            skinRawCnt = ((iir_skinRawCnt*3 )+ skinRawCnt) >> 2;
            iir_skinRawCnt = skinRawCnt;

            cplxRawCnt = ((iir_cplxRawCnt*3 )+ cplxRawCnt) >> 2;
            iir_cplxRawCnt = cplxRawCnt;

            dswRawCnt = ((iir_dswRawCnt*3 )+ dswRawCnt) >> 2;
            iir_dswRawCnt = dswRawCnt;


            backgroundState->flatSkinCnt = flatSkinCnt;
            backgroundState->cplxCnt = cplxCnt;
            backgroundState->prvBgCnt = prvBgCnt;
            backgroundState->skinRawCnt = skinRawCnt;
            backgroundState->cplxRawCnt = cplxRawCnt;
            backgroundState->dswRawCnt = dswRawCnt;

            backgroundState->pbgY = Ymax;
            backgroundState->pbgU = Umax;
            backgroundState->pbgV = Vmax;
            backgroundState->pbgrate = pbgrate;

            SetStatus = false;
        }
    }

}

void MDrv_UFSC_PQ_LSE( int frNNCnt2, int menuW, int edgePatW, int sdW, int srEnhStr )
{
    #define LSE_EDGE_G_LB 8
    int artW = __max( menuW, edgePatW );

    // CG pattern
    int nnW = __max( 0, frNNCnt2 - 2048 ) >> 3;
    nnW = __min( 256, nnW );

    // LSE adaptive edge gain
        int edgeGain = MDrv_UFSC_Read2ByteMSK( REG_FSC_BK30_38_L, 0x003F);
    int edgeGainLb = LSE_EDGE_G_LB;
    edgeGain = __max( edgeGain, edgeGainLb );

    int reduceW = __max( artW, sdW );
    int adptGain = __max( 0, edgeGain * (512-reduceW) >> 9 );
    adptGain = (adptGain * (256-nnW)) >> 8;
    adptGain = (adptGain * srEnhStr) >> 5;
    adptGain = __min( adptGain, 0x003F );
    MApi_XC_Write2ByteMask( REG_FSC_BK75_58_L, adptGain<<8, 0x3F00 );

    // detail gain
    reduceW = __max(0,256 - nnW);//max( 0, 256 - min( (sdW>>1), nnW ) );
    {
             int detGain = (MDrv_UFSC_Read2ByteMSK( REG_FSC_BK30_38_L, 0x3F00 ) * reduceW) >> 8;
        detGain = ( detGain * srEnhStr ) >> 5;
        detGain = __min( 0x003f, detGain );
        MApi_XC_Write2ByteMask( REG_FSC_BK75_58_L, detGain, 0x003F );
    }
    {
              int detGain = (MDrv_UFSC_Read2ByteMSK( REG_FSC_BK30_39_L, 0x003F ) * reduceW) >> 8;
        detGain = ( detGain * srEnhStr ) >> 5;
        detGain = __min( 0x003f, detGain );
        MApi_XC_Write2ByteMask( REG_FSC_BK75_59_L, detGain<<8, 0x3F00 );
    }
    {
              int detGain = (MDrv_UFSC_Read2ByteMSK( REG_FSC_BK30_39_L, 0x3F00 ) * reduceW) >> 8;
        detGain = ( detGain * srEnhStr ) >> 5;
        detGain = __min( 0x003f, detGain );
        MApi_XC_Write2ByteMask( REG_FSC_BK75_59_L, detGain, 0x003F );
    }
    // lut gain
    int finalLutGain = 0x08 + ( (0x08*sdW) >> 8 );
    MApi_XC_Write2ByteMask( REG_FSC_BK75_1C_L, finalLutGain, 0x001F );

}

void MDrv_UFSC_PQ_NLM( XC_BackgroundState *backgroundState, int menuW, int edgePatW, int srEnhStr )
{
    // coring
    MApi_XC_Write2ByteMask( REG_FSC_BK70_31_L, __max( edgePatW, menuW ) >> 3, 0x00FF );

    // NLM gain for skin, this should use colour adaptive
    int pbSkinW = __max( 0, (int)backgroundState->flatSkinCnt - 1200) >> 2;
    pbSkinW = __min( 16, pbSkinW );

    // NLM gain
        int finGain = /*0xA*/MDrv_UFSC_Read2ByteMSK( REG_FSC_BK30_30_L, 0x00FF ) - ( (4 * pbSkinW) >> 4 );
    finGain = ( finGain * srEnhStr ) >> 5;
    finGain = __min( 0x00FF, finGain );
    MApi_XC_Write2ByteMask( REG_FSC_BK70_31_L, finGain<<8, 0xFF00 );

    //printf( "pbSkinW, NLM gain : %d, %d\n", pbSkinW, finGain );

}

void MDrv_UFSC_PQ_PreEnh( int fHdW, int sdW, MS_U16 estNoise,MS_U8 estConfirm, int srEnhStr )
{
    // BLTI bias gain
    int biasGain = ( 0x10 * fHdW ) >> 8;
    if( fHdW == 256 )
    {
        biasGain = ( 0x10 * (256-sdW) + 0x20 * sdW ) >> 8;
    }

    MApi_XC_Write2ByteMask( REG_FSC_BK4C_52_L, biasGain, 0x007F );

    // LTI ac thrd
    int acThrd = ( 0x04 * (256-sdW) + 0x1C * sdW ) >> 8;
    MApi_XC_Write2ByteMask( REG_FSC_BK4C_53_L, acThrd<<8, 0xFF00 );

    // Pre PK3x3
    int finGain = (/*0x10*/MDrv_UFSC_Read2ByteMSK( REG_FSC_BK30_30_L, 0x3F00 ) * srEnhStr ) >> 5;
    finGain = __min( 0x3F00, finGain );
    MApi_XC_Write2ByteMask( REG_FSC_BK4C_48_L, finGain, 0x003F );

    // LDE
        int ldeDarkGain = MDrv_UFSC_Read2ByteMSK( REG_FSC_BK30_31_L, 0x1F00) ;
    int ldeDarkGainSD = MDrv_UFSC_Read2ByteMSK( REG_FSC_BK30_32_L, 0x001F) ;
    int ldeDarkGainFHD = (ldeDarkGain + ldeDarkGainSD) >> 1;
    int ldeGain = ( ldeDarkGain/*0xA*/ * (256 - fHdW) + ldeDarkGainFHD/*0x12*/ * fHdW ) >> 8;
    if( fHdW == 256 )
    {
        ldeGain = ( ldeDarkGainFHD * (256-sdW) + ldeDarkGainSD/*0x1A*/ * sdW ) >> 8;
    }

    MS_U8 report_noise = __minmax(estNoise,0,15);
    report_noise = (report_noise == 15) ? 16 : report_noise;
    MS_U8 report_confirm = __minmax(estConfirm-1,0,7);
    report_confirm = __max(0,report_confirm - 2) * 4;
    report_confirm = __minmax( report_confirm, 0, 16 );

    int noiseW = estNoise - (16-report_confirm);
    noiseW = 16 - __minmax( noiseW, 0, 16 );
    // back to default while noise is large
    ldeGain = ( ldeDarkGain * (16-noiseW) + ldeGain/*0xA*/ * noiseW ) >> 4;
    ldeGain = ( ldeGain * srEnhStr ) >> 5;

    MApi_XC_Write2ByteMask( REG_FSC_BK4C_25_L, ldeGain<<8, 0x1F00 );

    int ldeGainH =( MDrv_UFSC_Read2ByteMSK( REG_FSC_BK30_31_L, 0x001F )/*0xA*/ * noiseW ) >> 4;
    ldeGainH = ( ldeGainH * srEnhStr ) >> 5;
    MApi_XC_Write2ByteMask( REG_FSC_BK4C_25_L, ldeGainH, 0x001F );
}

void MDrv_UFSC_PQ_PostPK3x3( int menuW, int edgePatW, int fHdW, int sdW, int srEnhStr )
{
    // ds_minw
    edgePatW = __max( edgePatW, menuW );
    fHdW = __max( 0, fHdW - edgePatW );
       int dsmin = MDrv_UFSC_Read2ByteMSK( REG_FSC_BK30_33_L, 0xFF00);
       int dsminSD = __min( 0x00FF, dsmin + MDrv_UFSC_Read2ByteMSK( REG_FSC_BK30_34_L, 0x00FF) );
    int dsminFHD = (dsmin*170 + dsminSD*86) >> 8;
    int dsminW = ( dsmin * (256-fHdW) + /*0x20*/dsminFHD * fHdW ) >> 8;
    if( fHdW == 256 )
    {
        dsminW = ( /*0x20*/dsminFHD * (256-sdW) + /*0x60*/dsminSD * sdW ) >> 8;
    }
    MApi_XC_Write2ByteMask( REG_FSC_BK76_42_L, dsminW<<8, 0xFF00 );

    // ds gain
    //int dsw = MApi_XC_R2BYTEMSK( REG_SC_BK30_32_H, 0xFF );
    //int dswSD = MApi_XC_R2BYTEMSK( REG_SC_BK30_33_L, 0xFF );
    int dsGain = 0xB/*dsw*/ - ( ((0xB/*dswSD*/)*fHdW ) >> 8 );
    //int dsGain = ( dsw * (256-fHdW) + dswSD * fHdW ) >> 8;
    MApi_XC_Write2ByteMask( REG_FSC_BK76_42_L, (dsGain&0xFF), 0x00FF );

    // lower high gain
    int posGain = 0x30/*MApi_XC_R2BYTEMSK( REG_SC_BK30_34_H, 0xFF )*/ - ( (0x18/*MApi_XC_R2BYTEMSK( REG_SC_BK30_35_L, 0xFF )*/ * sdW) >> 8 );
    posGain = ( posGain * srEnhStr ) >> 5;
    posGain = __min( 0xFF, posGain );
    MApi_XC_Write2ByteMask( REG_FSC_BK76_44_L, posGain, 0x00FF );

    int negGain = ( 0x30/*MApi_XC_R2BYTEMSK( REG_FSC_BK30_35_H, 0xFF )*/ * srEnhStr ) >> 5;
    negGain = __min( 0xFF, negGain );
    MApi_XC_Write2ByteMask( REG_FSC_BK76_44_L, negGain<<8, 0xFF00 );
}

void MDrv_UFSC_PQ_HASC( int sdW, int srEnhStr )
{
       int lowGain = /*0x28*/MDrv_UFSC_Read2ByteMSK( REG_FSC_BK30_36_L, 0x7F00 )
        + ( (/*0x38*/MDrv_UFSC_Read2ByteMSK( REG_FSC_BK30_37_L, 0x007F ) * sdW) >> 8 );
    lowGain = ( lowGain * srEnhStr ) >> 5;
    lowGain = __min( lowGain, 0x007F );
    MApi_XC_Write2ByteMask( REG_FSC_BK74_2E_L, lowGain<<8, 0x7F00 );

       int highGain = ( /*0x18*/MDrv_UFSC_Read2ByteMSK( REG_FSC_BK30_36_L, 0x007F ) * srEnhStr ) >> 5;
    highGain = __min( 0x007F, highGain );
    MApi_XC_Write2ByteMask( REG_FSC_BK74_2E_L, highGain, 0x007F );
}

void MDrv_UFSC_PQ_P2M( int menuW, int edgePatW, int srEnhStr )
{
       int gain = ( /*9*/MDrv_UFSC_Read2ByteMSK( REG_FSC_BK30_37_L, 0x3F00 ) * (256 - __max(edgePatW, menuW)) ) >> 8;
    gain = (gain * srEnhStr ) >> 5;
    gain = __min( 0x3F00, gain );
    MApi_XC_Write2ByteMask( REG_FSC_BK72_41_L, gain, 0x003F );
}

void MDrv_UFSC_PQ_ANN( int frNNCnt2 )
{
    // CG pattern
    int nnW = __max( 0, frNNCnt2 - 2048 ) >> 3;
    nnW = __min( 256, nnW );
    int artNNW = __max( 0, frNNCnt2 - 8192 ) >> 4;
    artNNW = __min( 256, artNNW );

    int colorGain = ( 0x10 * (256-nnW) + 0x7 * nnW ) >> 8;
    colorGain = ( colorGain * (256-artNNW) + 0x04 * artNNW ) >> 8;
    int colorOffset = ( artNNW * 0x18 ) >> 8;

    int strictGain = ( 0x20 * (256-nnW) + 0x4 * nnW ) >> 8;
    strictGain = ( strictGain * (256-artNNW) + 0x2 * artNNW ) >> 8;
    int strictOffset = ( 0x17 * nnW ) >> 8;
    strictOffset = ( strictOffset * (256-artNNW) + 0x0 * artNNW ) >> 8;

    int nnGain = ( 0x10 * (256-artNNW) + 0x18 * artNNW ) >> 8;

    MApi_XC_Write2ByteMask( REG_FSC_BK72_25_L, ((colorGain<<8)+colorOffset), 0xFF00 );
    MApi_XC_Write2ByteMask( REG_FSC_BK72_26_L, ((strictGain<<8)+strictOffset), 0xFF00 );
    MApi_XC_Write2ByteMask( REG_FSC_BK72_2D_L, nnGain, 0x001F );

}

void MDrv_UFSC_PQ_DMS_referBNF( XC_BackgroundState *backgroundState )
{

    MS_U16 Ymax_idx = backgroundState->pbgY;
    MS_U16 Umax_idx = backgroundState->pbgU;
    MS_U16 Vmax_idx = backgroundState->pbgV;
    MS_U32 confidence = backgroundState->pbgrate;


    MApi_XC_Write2ByteMask(REG_FSC_BK26_44_L, Ymax_idx , 0x001F );
    MApi_XC_Write2ByteMask(REG_FSC_BK26_45_L, Umax_idx , 0x001F );
    MApi_XC_Write2ByteMask(REG_FSC_BK26_45_L, Vmax_idx << 8 , 0x1F00 );
    MApi_XC_Write2ByteMask(REG_FSC_BK26_46_L, confidence, 0x3FFF );

}

//-------------------------------------------------------------------------------------------------
/// LSE PQ function
//  1. Zone plate patch
//-------------------------------------------------------------------------------------------------

void MDrv_UFSC_PQ_LSE_Patch(void)
{
    static int iir_std_stat = 0;
    int std_stat_TH = 5;
       int reg_std_stat =  MDrv_UFSC_Read2Byte(REG_FSC_BK2C_79_L);
    int totalPixelCount = (( MDrv_UFSC_Read2Byte( REG_FSC_BK55_19_L ) << 16 ) + MDrv_UFSC_Read2Byte( REG_FSC_BK55_18_L ));
    int totalPixelYSum= ( MDrv_UFSC_Read2Byte( REG_FSC_BK55_1B_L ) << 16 ) + MDrv_UFSC_Read2Byte( REG_FSC_BK55_1A_L );

    totalPixelCount = (totalPixelCount / 256);

    if (totalPixelCount == 0)
        return;
    //printf("totalPixelCount:%d  ;totalPixelYSum%d \n",totalPixelCount,totalPixelYSum);

    int APL = (totalPixelYSum / totalPixelCount);

    /* IIR states */
    reg_std_stat = ((iir_std_stat*3 )+ reg_std_stat ) >> 2;
    iir_std_stat = reg_std_stat;

    APL = (APL == 0) ? 1 : APL;

    iir_std_stat = (iir_std_stat / APL );

    if ((iir_std_stat > std_stat_TH) || (APL < 20))
    {
        MApi_XC_Write2ByteMask(REG_FSC_BK75_18_L, 0x00, 0x0F);//reg_lse_local_cplx_lut_gain
        MApi_XC_Write2ByteMask(REG_FSC_BK71_31_L, 0x0000 , 0x0100);//reg_lse_nlm_gain_en
        MApi_XC_Write2ByteMask(REG_FSC_BK71_30_L, 0x0000 , 0x0100);//reg_lse_uc_en
        MApi_XC_Write2ByteMask(REG_FSC_BK57_22_L, 0x00, 0x03);//reg_time_of_cross_weight
        MApi_XC_Write2ByteMask(REG_FSC_BK57_22_L, 0x00, 0x18);//reg_time_of_x_weight
        MApi_XC_Write2ByteMask(REG_FSC_BK56_25_L, 0x00 , 0xF0);//reg_main_lti_jaggy_min
        MApi_XC_Write2ByteMask(REG_FSC_BK56_25_L, 0x0000 , 0x1F00);//reg_main_lti_jaggy_slope
        MApi_XC_Write2ByteMask(REG_FSC_BK56_25_L, 0x8000 , 0x8000);//reg_main_lti_jaggy_en
    }
    else
    {
        MApi_XC_Write2ByteMask(REG_FSC_BK75_18_L, 0x08, 0x0F);//reg_lse_local_cplx_lut_gain
        MApi_XC_Write2ByteMask(REG_FSC_BK71_31_L, 0x0100 , 0x0100);//reg_lse_nlm_gain_en
        MApi_XC_Write2ByteMask(REG_FSC_BK71_30_L, 0x0100 , 0x0100);//reg_lse_uc_en
        MApi_XC_Write2ByteMask(REG_FSC_BK57_22_L, 0x03, 0x03);//reg_time_of_cross_weight
        MApi_XC_Write2ByteMask(REG_FSC_BK57_22_L, 0x18, 0x18);//reg_time_of_x_weight
        MApi_XC_Write2ByteMask(REG_FSC_BK56_25_L, 0x40 , 0xF0);//reg_main_lti_jaggy_min
        MApi_XC_Write2ByteMask(REG_FSC_BK56_25_L, 0x0400 , 0x1F00);//reg_main_lti_jaggy_slope
        MApi_XC_Write2ByteMask(REG_FSC_BK56_25_L, 0x0000 , 0x8000);//reg_main_lti_jaggy_en
    }

    //printf("reg_std_stat:%d ; iir_std_stat %d APL %d\n",reg_std_stat,iir_std_stat,APL);


}


int normFactor(int avgBLKN)
{
	int normFac;
	if (avgBLKN > 32768)
		normFac = 11;
	else if (avgBLKN > 16384)
		normFac = 10;
	else if (avgBLKN > 8192)
		normFac = 9;
	else if (avgBLKN > 4096)
		normFac = 8;
	else if (avgBLKN > 2048)
		normFac = 7;
	else if (avgBLKN > 1024)
		normFac = 6;
	else if (avgBLKN > 512)
		normFac = 5;
	else if (avgBLKN > 256)
		normFac = 4;
	else if (avgBLKN > 128)
		normFac = 3;
	else if (avgBLKN > 64)
		normFac = 2;
	else if (avgBLKN > 32)
		normFac = 1;
	else if (avgBLKN > 16)
		normFac = 0;
	else
		normFac = 0;
	return normFac;
}

int normFactorWidth(int avgBLKN)
{
	int normFac = 0;

	if (avgBLKN > 1048576*1024)
		normFac = 25;
	else if (avgBLKN > 1048576*512)
		normFac = 24;
	else if (avgBLKN > 1048576*256)
		normFac = 23;
	else if (avgBLKN > 1048576*128)
		normFac = 22;
	else if (avgBLKN > 1048576*64)
		normFac = 21;
	else if (avgBLKN > 1048576*32)
		normFac = 20;
	else if (avgBLKN > 1048576*16)
		normFac = 19;
	else if (avgBLKN > 1048576*8)
		normFac = 18;
	else if (avgBLKN > 1048576*4)
		normFac = 17;
	else if (avgBLKN > 1048576*2)
		normFac = 16;
	else if (avgBLKN > 1048576)
		normFac = 15;
	else if (avgBLKN > 524288)
		normFac = 14;
	else if (avgBLKN > 262144)
		normFac = 13;
	else if (avgBLKN > 131072)
		normFac = 12;
	else if (avgBLKN > 65536)
		normFac = 11;
	else if (avgBLKN > 32768)
		normFac = 10;
	else if (avgBLKN > 16384)
		normFac = 9;
	else if (avgBLKN > 8192)
		normFac = 8;
	else if (avgBLKN > 4096)
		normFac = 7;
	else if (avgBLKN > 2048)
		normFac = 6;
	else if (avgBLKN > 1024)
		normFac = 5;
	else if (avgBLKN > 512)
		normFac = 4;
	else if (avgBLKN > 256)
		normFac = 3;
	else if (avgBLKN > 128)
		normFac = 2;
	else if (avgBLKN > 64)
		normFac = 1;
	else if (avgBLKN > 32)
		normFac = 0;
	else if (avgBLKN > 16)
		normFac = -1;
	else if (avgBLKN > 8)
		normFac = -2;
	else
		normFac = 0;
	return normFac;
}
int heightEstimate(int FrameWidth)
{
	if (FrameWidth < 720)
		return FrameWidth*3/4;
	else if (FrameWidth == 720)
		return 480;
	else if ((FrameWidth>720) && (FrameWidth<1440))
		return FrameWidth*9/16;
	else if (FrameWidth == 1440)
		return 900;
	else if ((FrameWidth>1440) && (FrameWidth<1920))
		return FrameWidth*9/16;
	else if (FrameWidth==1920)
		return 1080;
	else if (FrameWidth>=3840)
		return 2160;
	else //if (frameWidth>1920)
		return FrameWidth*9/16;
}
int DBK_StPtLeft( int width, MS_U8 *BLKN_adj, int rangeLeft, int rangeRight) //Left to right search
{
	int newSP=0;
	int candiValue = 0;
	int candiPos = 0;//-1
	int ThValue = 2;
	int bin=0;
	int pl2=0, pl1=0, pr1=0, pr2=0;
	int l1=0, r1=0;
	for(bin = rangeLeft; bin < rangeRight; bin++)
	{
		int L1Index  = __minmax(bin-width  , 0, frameWidth-1);
		int L1Index2 = __minmax(bin-width-1, 0, frameWidth-1);
		pl1 = __max(BLKN_adj[L1Index], BLKN_adj[L1Index2]);

		if (pl1 == BLKN_adj[L1Index])
			l1 = L1Index;
		else if (pl1 == BLKN_adj[L1Index2])
			l1 = L1Index2;

		int L2Index  = __minmax(l1 -width  , 0, frameWidth-1);
		int L2Index2 = __minmax(l1 -width-1, 0, frameWidth-1);
		pl2 = __max(BLKN_adj[L2Index], BLKN_adj[L2Index2]);

		int R1Index  = __minmax(bin+width  , 0, frameWidth-1);
		int R1Index2 = __minmax(bin+width+1, 0, frameWidth-1);
		pr1 = __max(BLKN_adj[R1Index], BLKN_adj[R1Index2]);

		if (pr1 == BLKN_adj[R1Index])
			r1 = R1Index;
		else if (pr1 == BLKN_adj[R1Index2])
			r1 = R1Index2;

		int R2Index  = __minmax(r1+width  , 0, frameWidth-1);
		int R2Index2 = __minmax(r1+width+1, 0, frameWidth-1);

		pr2 = __max(BLKN_adj[R2Index], BLKN_adj[R2Index2]);

		newSP = BLKN_adj[bin] + pl1 + pl2 + pr1 + pr2;

		if (newSP > candiValue)
		{
			candiValue = newSP;
			candiPos = bin;
		}
	}
	if (candiValue > ThValue)
		return candiPos;
	else
		return 9999;//-1
}
int DBK_StPtRight( int width, MS_U8 *BLKN_adj, int rangeLeft, int rangeRight) //right to left
{
	int newSP=0;
	int candiValue = 0;
	int candiPos = -1;
	int ThValue = 2;
	int bin=0;
	int pl2=0, pl1=0, pr1=0, pr2=0;
	int l1=0, r1=0;
	for(bin = rangeRight-1; bin >= rangeLeft; bin--)
	{
		int L1Index  = __minmax(bin-width  , 0, frameWidth-1);
		int L1Index2 = __minmax(bin-width-1, 0, frameWidth-1);
		pl1 = __max(BLKN_adj[L1Index], BLKN_adj[L1Index2]);

		if (pl1 == BLKN_adj[L1Index])
			l1 = L1Index;
		else if (pl1 == BLKN_adj[L1Index2])
			l1 = L1Index2;

		int L2Index  = __minmax(l1 -width  , 0, frameWidth-1);
		int L2Index2 = __minmax(l1 -width-1, 0, frameWidth-1);
		pl2 = __max(BLKN_adj[L2Index], BLKN_adj[L2Index2]);

		int R1Index  = __minmax(bin+width  , 0, frameWidth-1);
		int R1Index2 = __minmax(bin+width+1, 0, frameWidth-1);
		pr1 = __max(BLKN_adj[R1Index], BLKN_adj[R1Index2]);

		if (pr1 == BLKN_adj[R1Index])
			r1 = R1Index;
		else if (pr1 == BLKN_adj[R1Index2])
			r1 = R1Index2;

		int R2Index  = __minmax(r1+width  , 0, frameWidth-1);
		int R2Index2 = __minmax(r1+width+1, 0, frameWidth-1);
		pr2 = __max(BLKN_adj[R2Index], BLKN_adj[R2Index2]);

		newSP = BLKN_adj[bin] + pl1 + pl2 + pr1 + pr2;

		if (newSP > candiValue)
		{
			candiValue = newSP;
			candiPos = bin;
		}
	}
	if (candiValue > ThValue)
		return candiPos;
	else
		return 9999;//-1
}

int DBK_WidthPartHW(void *pInstance,int *WidthDiffHW, int* avg, MS_U8 u8SD2HDValue)
{
	int idx;
	int maxWidthDiff  = 0;
	int minWidthDiff  = 0x7FFFFFFF;
	int min2WidthDiff = 0x7FFFFFFF;
	int min3WidthDiff = 0x7FFFFFFF;
	int min4WidthDiff = 0x7FFFFFFF;
	int minIdx = -1, min2Idx = -1, min3Idx = -1, min4Idx = -1;// maxIdx = -1;

	int DiffThrd = 2048;

	WidthDiffHW[0 ] = MApi_XC_R2BYTEMSK(REG_SC_BK60_40_L,0xFFFF) ; // width 4
	WidthDiffHW[1 ] = MApi_XC_R2BYTEMSK(REG_SC_BK60_41_L,0xFFFF) ; // width 5
	WidthDiffHW[2 ] = MApi_XC_R2BYTEMSK(REG_SC_BK60_42_L,0xFFFF) ; // width 6
	WidthDiffHW[3 ] = MApi_XC_R2BYTEMSK(REG_SC_BK60_43_L,0xFFFF) ; // width 7
	WidthDiffHW[4 ] = MApi_XC_R2BYTEMSK(REG_SC_BK60_44_L,0xFFFF) ; // width 8
	WidthDiffHW[5 ] = MApi_XC_R2BYTEMSK(REG_SC_BK60_45_L,0xFFFF) ; // width 9
	WidthDiffHW[6 ] = MApi_XC_R2BYTEMSK(REG_SC_BK60_46_L,0xFFFF) ; // width 10
	WidthDiffHW[7 ] = MApi_XC_R2BYTEMSK(REG_SC_BK60_47_L,0xFFFF) ; // width 11
	WidthDiffHW[8 ] = MApi_XC_R2BYTEMSK(REG_SC_BK60_48_L,0xFFFF) ; // width 12
	WidthDiffHW[9 ] = MApi_XC_R2BYTEMSK(REG_SC_BK60_49_L,0xFFFF) ; // width 13
	WidthDiffHW[10] = MApi_XC_R2BYTEMSK(REG_SC_BK60_4A_L,0xFFFF) ; // width 14
	WidthDiffHW[11] = MApi_XC_R2BYTEMSK(REG_SC_BK60_4B_L,0xFFFF) ; // width 15
	WidthDiffHW[12] = MApi_XC_R2BYTEMSK(REG_SC_BK60_4C_L,0xFFFF) ; // width 16
	WidthDiffHW[13] = MApi_XC_R2BYTEMSK(REG_SC_BK60_4D_L,0xFFFF) ; // width 17
	WidthDiffHW[14] = MApi_XC_R2BYTEMSK(REG_SC_BK60_4E_L,0xFFFF) ; // width 18
	WidthDiffHW[15] = MApi_XC_R2BYTEMSK(REG_SC_BK60_4F_L,0xFFFF) ; // width 19
	WidthDiffHW[16] = MApi_XC_R2BYTEMSK(REG_SC_BK60_50_L,0xFFFF) ; // width 20
	WidthDiffHW[17] = MApi_XC_R2BYTEMSK(REG_SC_BK60_51_L,0xFFFF) ; // width 21
	WidthDiffHW[18] = MApi_XC_R2BYTEMSK(REG_SC_BK60_52_L,0xFFFF) ; // width 22
	WidthDiffHW[19] = MApi_XC_R2BYTEMSK(REG_SC_BK60_53_L,0xFFFF) ; // width 23
	WidthDiffHW[20] = MApi_XC_R2BYTEMSK(REG_SC_BK60_54_L,0xFFFF) ; // width 24
	WidthDiffHW[21] = MApi_XC_R2BYTEMSK(REG_SC_BK60_55_L,0xFFFF) ; // width 25
	WidthDiffHW[22] = MApi_XC_R2BYTEMSK(REG_SC_BK60_56_L,0xFFFF) ; // width 26
	WidthDiffHW[23] = MApi_XC_R2BYTEMSK(REG_SC_BK60_57_L,0xFFFF) ; // width 27
	WidthDiffHW[24] = MApi_XC_R2BYTEMSK(REG_SC_BK60_58_L,0xFFFF) ; // width 28
	WidthDiffHW[25] = MApi_XC_R2BYTEMSK(REG_SC_BK60_59_L,0xFFFF) ; // width 29
	WidthDiffHW[26] = MApi_XC_R2BYTEMSK(REG_SC_BK60_5A_L,0xFFFF) ; // width 30
	WidthDiffHW[27] = MApi_XC_R2BYTEMSK(REG_SC_BK60_5B_L,0xFFFF) ; // width 31
	WidthDiffHW[28] = MApi_XC_R2BYTEMSK(REG_SC_BK60_5C_L,0xFFFF) ; // width 32
	WidthDiffHW[29] = MApi_XC_R2BYTEMSK(REG_SC_BK60_5D_L,0xFFFF) ; // width 33
	WidthDiffHW[30] = MApi_XC_R2BYTEMSK(REG_SC_BK60_5E_L,0xFFFF) ; // width 34
	WidthDiffHW[31] = MApi_XC_R2BYTEMSK(REG_SC_BK60_5F_L,0xFFFF) ; // width 35
	for(idx = 1 ; idx < paraSize ; idx++)
	{
		if (WidthDiffHW[idx] > maxWidthDiff)
		{
			maxWidthDiff = WidthDiffHW[idx];
//			maxIdx = idx;
		}
	}
	for(idx = 1 ; idx < paraSize -1; idx+=2)
	{
		if ( (idx==3 || idx==11 || idx==19 || idx==27) && (WidthDiffHW[idx+1] - WidthDiffHW[idx ] <= DiffThrd) )
		{
			WidthDiffHW[idx+1]= __min(WidthDiffHW[idx ], WidthDiffHW[idx+1]);
			WidthDiffHW[idx  ]= maxWidthDiff;
		}
		else if ( (idx==7) && ( (WidthDiffHW[idx  ] - WidthDiffHW[idx-1 ] <= DiffThrd) || (WidthDiffHW[idx  ] - WidthDiffHW[idx+1 ] <= DiffThrd)) )
		{
			WidthDiffHW[idx  ]= __min3(WidthDiffHW[idx ], WidthDiffHW[idx-1], WidthDiffHW[idx+1]);
			WidthDiffHW[idx-1]= maxWidthDiff;
			WidthDiffHW[idx+1]= maxWidthDiff;
		}
	}
	for(idx = 1 ; idx < paraSize ; idx++)
	{
		(*avg) += WidthDiffHW[idx];

		if (WidthDiffHW[idx] < minWidthDiff)
		{
			min4WidthDiff = min3WidthDiff;
			min4Idx = min3Idx;
			min3WidthDiff = min2WidthDiff;
			min3Idx = min2Idx;
			min2WidthDiff = minWidthDiff;
			min2Idx = minIdx;

			minWidthDiff = WidthDiffHW[idx];
			minIdx = idx;
		}
		else if (WidthDiffHW[idx] < min2WidthDiff)
		{
			min4WidthDiff = min3WidthDiff;
			min4Idx = min3Idx;
			min3WidthDiff = min2WidthDiff;
			min3Idx = min2Idx;

			min2WidthDiff = WidthDiffHW[idx];
			min2Idx = idx;
		}
		else if (WidthDiffHW[idx] < min3WidthDiff)
		{
			min4WidthDiff = min3WidthDiff;
			min4Idx = min3Idx;

			min3WidthDiff = WidthDiffHW[idx];
			min3Idx = idx;
		}
		else if (WidthDiffHW[idx] < min4WidthDiff)
		{
			min4WidthDiff = WidthDiffHW[idx];
			min4Idx = idx;
		}
	}
	*avg = (*avg) / (paraSize-1);


	if ( ((minIdx == 7) || (min2Idx == 7) || (min3Idx == 7) || (min4Idx == 7)) && ((minIdx == 28) || (min2Idx == 28) || (min3Idx == 28) || (min4Idx == 28)) && ( ((minIdx == 16) || (min2Idx == 16) || (min3Idx == 16) || (min4Idx == 16)) || ((minIdx == 17) || (min2Idx == 17) || (min3Idx == 17) || (min4Idx == 17)) ))
	{
		if (u8SD2HDValue==0)
			return 7;
	}
	else if ( ((minIdx == 16 || min2Idx == 16 || min3Idx == 16) || (minIdx == 18 || min2Idx == 18 || min3Idx == 18) ) && (minIdx == 17 || min2Idx == 17 || min3Idx == 17)   )
	{
		if (u8SD2HDValue>=60)
			return 17;
	}


	if (minIdx == 4 || min2Idx == 4 || min3Idx == 4 || min4Idx == 4)
	{
		return 4;
	}
	else if (minIdx == 12 || min2Idx == 12 || min3Idx == 12 || min4Idx == 12)
	{
		return 12;
	}
	else
	{
		return -1;
	}
}

int DBK_WidthPartTop(void * pInstance,int *WidthDiffHW, int* MaxMinDiff, int* avg, MS_U8 u8SD2HDValue)
{
	int bin;
	int minValue = 0x7FFFFFFF;
	int maxValue = 0;
	int normFac, minIndex;

	minIndex = DBK_WidthPartHW(pInstance,WidthDiffHW, avg, u8SD2HDValue);
	if (minIndex == -1)
	{
		for (bin =0; bin<paraSize;bin++)
		{
			if (WidthDiffHW[bin] > maxValue )
			{
				maxValue = WidthDiffHW[bin];
			}
			if (WidthDiffHW[bin] < minValue )
			{
				minValue = WidthDiffHW[bin];
			}
		}

		*MaxMinDiff = maxValue - minValue;
		normFac = normFactorWidth(*MaxMinDiff);
		for (bin=0;bin<paraSize;bin++)
		{
			WidthDiffHW[bin] = (WidthDiffHW[bin] - minValue) >> normFac;
		}
		*avg = (*avg-minValue) >> normFac;
		*MaxMinDiff = (*MaxMinDiff) >>normFac;

		return -1;
	}
	else
		return minIndex;
}
int DBK_VerifyWidthMark( MS_U8 *BLKN_adj, int width, int iniIndex, MS_U8 markDefValue)
{

	int restart=0;

	int misscnt = 0;
	int ThValue = 2; //threshold value for BLKN_adj
	int hitrate = 0;
	int cnt = 0;
	int bin, CurH, sp;
	MS_BOOL hit;
	int n8[50];
	int maxOri, maxOriInd, offset;
	int maxLocal, sndLocal, maxIndex, sndIndex;
	int pos_fac, range, dummy, candiSP;
	offset = 4;//8;
	if (width >= 16)
		offset = 8;//16;

	misscnt = 0;
	//initialize start points
	sp = iniIndex;
	range = (width*2+4)>>3;
	dummy = ThValue *(width + offset)/offset;

	hitrate++;
	cnt++;
	//right hand side
	for (CurH=sp+width; CurH<frameWidth*3/4-(width+1)/2; )
	{
		maxOri = 0;
		maxOriInd = -1;
		maxLocal = 0;
		sndLocal = 0;
		maxIndex = 0;
		sndIndex = 0;
		for (bin=0; bin<width;bin++)
		{
			int curIndex = __minmax(CurH+(bin-(width-1)/2), 0, frameWidth-1);
			if (BLKN_adj[curIndex]-markDefValue > maxOri)
			{
				maxOri = BLKN_adj[curIndex]-markDefValue;
				maxOriInd = bin;
			}

			if ( (bin<=(width/4)) || (bin>=(width*3/4)) )
				n8[bin] =0;
			else
			{
				pos_fac = (width+1)/2 - abs(bin-(width-1)/2) + offset; //16 = offset
				n8[bin]= ( (BLKN_adj[curIndex]-markDefValue) * pos_fac + (offset/2) )/offset;
				if ( n8[bin] > maxLocal)
				{
					//test
					sndLocal = maxLocal;
					sndIndex = maxIndex;

					maxLocal = n8[bin];
					maxIndex = bin;
				}
				//test
				else if (n8[bin] > sndLocal)
				{
					sndLocal = n8[bin];
					sndIndex = bin;
				}
			}
		}


		//test
		if( abs(sndIndex-maxIndex) > range)
		{
			sndLocal = 0;
			sndIndex = 0;
		}

		int curIndex = __minmax(CurH+(maxIndex-(width-1)/2), 0, frameWidth-1);
		hit = ( maxOri <= BLKN_adj[curIndex]-markDefValue )|| ( (maxOriInd>=__min(maxIndex,maxIndex+sndIndex) )&& (maxOriInd <= __max(maxIndex,maxIndex+sndIndex) ) );

		if ( (!hit) || (maxLocal <= dummy) )
		{
			misscnt++;
		}

		if ( (maxLocal > dummy) && (misscnt <= 2) )
		{
			CurH = CurH+(maxIndex-(width-1)/2); //adjust to max position
			if (hit)
			{
				misscnt=0;
			}
			curIndex = __minmax(CurH+sndIndex, 0, frameWidth-1);
			if (BLKN_adj[curIndex]-markDefValue > ThValue ) // fill the gap between top 2 strength within range 4
			{
				CurH = CurH + ((sndIndex)/2); // move to center
			}
			CurH += width;
			hitrate++;
			cnt++;
		}
		else if (misscnt > 2)
		{
			if (misscnt>5)
			{
				return 0;
			}

			candiSP = DBK_StPtLeft( width, BLKN_adj, CurH, __min(CurH+40, frameWidth*3/4));
			if (candiSP != 9999)
			{
				restart++;
				misscnt = 0;
				CurH = candiSP;
			}
			else
			{
				CurH += 40;
			}
			cnt++;
		}
		else
		{
			CurH += width;
			cnt++;
		}
	}
	misscnt=0;
	//left hand side
	for (CurH=sp-width; CurH >= frameWidth*1/4+(width+1)/2;)
	{
		maxOri = 0;
		maxOriInd = -1;
		maxLocal = 0;
		maxIndex = 0;
		sndLocal = 0;
		sndIndex = 0;
		for (bin=0; bin<width;bin++)
		{
			int curIndex = __minmax(CurH+(bin-(width+1)/2), 0, frameWidth-1);
			if (BLKN_adj[curIndex]-markDefValue > maxOri)
			{
				maxOri = BLKN_adj[curIndex]-markDefValue;
				maxOriInd = bin;
			}
			if ( (bin<=width/4) || (bin>=width*3/4) )
				n8[bin] =0;
			else
			{
				pos_fac = (width+1)/2 - abs(bin-(width+1)/2) + offset; //16 = offset
				n8[bin]= ((BLKN_adj[curIndex]-markDefValue) * pos_fac + (offset/2) )/offset;
				if (n8[bin] > maxLocal)
				{
					//test
					sndLocal = maxLocal;
					sndIndex = maxIndex;

					maxLocal = n8[bin];
					maxIndex = bin;
				}
				//test
				else if (n8[bin] > sndLocal)
				{
					sndLocal = n8[bin];
					sndIndex = bin;
				}
			}
		}
		//test
		if( abs(sndIndex-maxIndex) > range)
		{
			sndLocal = 0;
			sndIndex = 0;
		}
		//if maximum inside range
		int curIndex = __minmax(CurH+(maxIndex-(width+1)/2), 0, frameWidth-1);
		hit = ( maxOri <= BLKN_adj[curIndex]-markDefValue ) || ( (maxOriInd>=__min(maxIndex,maxIndex+sndIndex) )&& (maxOriInd <= __max(maxIndex,maxIndex+sndIndex) ) );
		if ( (!hit) || (maxLocal <= dummy) )
		{
			misscnt++;
		}
		if ((maxLocal > dummy) && (misscnt <= 2) )
		{
			CurH = CurH+(maxIndex-(width+1)/2); //adjust to max position
			if (hit)
			{
				misscnt=0;
			}
			curIndex = __minmax(CurH+sndIndex, 0, frameWidth-1);
			if (BLKN_adj[curIndex]-markDefValue > ThValue )
			{
				CurH = CurH + ((sndIndex)/2); // move to center
			}
			CurH -= width;
			hitrate++;
			cnt++;
		}
		else if (misscnt > 2)
		{
			if (misscnt > 5)
			{
				return 0;
			}
			candiSP = DBK_StPtRight(width, BLKN_adj, __max((CurH-40), frameWidth*1/4), CurH+1);
			if (candiSP != 9999)
			{
				restart++;
				misscnt = 0;
				CurH = candiSP;
			}
			else
			{
				CurH -= 40;
			}
			cnt++;
		}
		else
		{
			CurH -= width;
			cnt++;
		}
	}
	if (cnt > 0)
		return ( (hitrate*256/cnt) - 8*restart);
	else
		return 0;
}

void DBK_VerifyWidthPart(  int *kk2, MS_U8* BLKN_adj, int partIndex, int* maxHitRate, int* minIndex, int* cnt, int sp, MS_U8 u8SD2HDValue, MS_U8 markDefValue)
{


	int hitrate = 0;
	int width, range, bin; //w2, X2, sp
	int X2term;
	int minWidth = 0;

	if (kk2[partIndex] == 1)
	{
		width = paraWidth[partIndex];
		if (*minIndex >= 0)
		{
			minWidth = paraWidth[*minIndex];
		}
		hitrate = DBK_VerifyWidthMark( BLKN_adj, width, sp, markDefValue); //replace with mark function
		//test for IIR stability

		if (width == WidthIIR)
			hitrate = hitrate+16;

		if ( (u8SD2HDValue>=60) && (width==21) &&(minWidth==11) )
			hitrate = hitrate+16;

		if (hitrate < hitrate_threshold)
		{
			kk2[partIndex] = 0;
		}
		else if (hitrate > *maxHitRate )
		{
			*maxHitRate = hitrate;
			*minIndex = partIndex;
			*cnt = 1;
		}
		else if (((*maxHitRate-hitrate) <= (hitRateDiffThr*3))
			&& (((width == 8) && ((minWidth%4) == 0))||(( (width == 11) && (minWidth == 32)))))
		{
			if ( (minWidth*2+3)/6 == width )
			{
				X2term = -1;
				for (bin =0; bin<paraSize;bin++)
				{
					if (paraWidth[bin] == (width+1)/2 )
						X2term = bin;

				}

				if ((X2term>=0) && (kk2[X2term] == 1 ))
				{
					kk2[X2term] = 0;
					*cnt = *cnt-1;
				}
			}

			*minIndex = partIndex;

		}
		else if ( (*maxHitRate-hitrate) <= hitRateDiffThr)
		{
			range = (width*2+4)>>3;

			if ( abs(minWidth - width) <= range )
			{
				if ( (width == 21) || (width == 8) || (width == 11)) //priorty of common width
				{
					*minIndex = partIndex;
				}
				else if ( ( minWidth== 21) || (minWidth == 8) || (minWidth == 11)) //priorty of common width
				{
					kk2[partIndex] = 0;
				}
				else //add for test
					*cnt = *cnt+1;
			}
			else
			{
				(*cnt)++;
			}
		}
	}
}

int DBK_ComputeWidthPart(  int *WidthDiffHW, int* WidthCandidate, int avg)
{
	int WTh = avg;
	int minIndex=0;
	int NumW=0;
	int bin;

	MS_BOOL if8 = 0, if11 = 0, if21 = 0, if32 = 0;

	for (bin=0;bin<paraSize;bin++)
	{
		if (WidthDiffHW[bin]<= WTh)
			WidthCandidate[bin] = 1;
		else
			WidthCandidate[bin] = 0;
	}

	if (1)
	{
		if (WidthCandidate[4] == 1 ) //8
		{
			if (WidthCandidate[3] == 1) //7
				WidthCandidate[3]  = 0;
			if (WidthCandidate[11] == 1) // 15
				WidthCandidate[11] = 0;
			if (WidthCandidate[12] == 1) // 16
				WidthCandidate[12] = 0;
			if (WidthCandidate[19] == 1) // 23
				WidthCandidate[19] = 0;
			if (WidthCandidate[20] == 1) // 24
				WidthCandidate[20] = 0;
			if8 = 1;
		}
		if (WidthCandidate[7] == 1 ) //11
		{
			if (WidthCandidate[6] == 1) //10
				WidthCandidate[6]  = 0;
			if (WidthCandidate[8] == 1) //12
				WidthCandidate[8]  = 0;

			if11 = 1;
		}
		if ( ( WidthCandidate[16] == 1 ) || ( WidthCandidate[17] == 1 ) ||( WidthCandidate[18] == 1 ) ) //20 21 22
		{
			WidthCandidate[16] = 0;
			WidthCandidate[17] = 1;
			WidthCandidate[18] = 0;
			if21 = 1;
		}
		if ( WidthCandidate[27] == 1 && WidthCandidate[28] == 1)
			if32 = 1;
		if (if11 && if21 && if32) //11/21/32 all true -> source = 11 ->delete 21
		{
			WidthCandidate[17] = 0;
		}
		if (if8 || if11 || if21)
		{
			if (WidthCandidate[27] == 1) // 31
				WidthCandidate[27] = 0;
			if (WidthCandidate[28] == 1) // 32
				WidthCandidate[28] = 0;
		}
	}
	for (bin=0;bin<paraSize;bin++) // count number of frequencies
	{
		if (WidthCandidate[bin] == 1)
		{
			minIndex = bin;
			NumW++;
		}
	}

	if (NumW == 1)
	{
		return minIndex;
	}
	else
	{
		return -1;
	}
}

int DBK_BlocknessAdjPart(long* histogram, MS_U8 *BLKN_adj, int* zeroCnt, int partIndex, MS_U8 markValue, MS_U8 markDefValue, int ADJTIMESLOT)
{

	//int frameUnit = __max(frameWidth*frameHeight, 1);
	//int avgBLKN = 0;
	//int normFac = 0;
	//const int localWidth = 10;
	//int ThrHei = 2*frameHeight;
	int valid = 0;
	//int bin,
       int CurH;

	//MS_U8 markNorm = markValue;
	//int movingSum = 0;
	//int blkness;

	int IniPos, EndPos;

    if ( (partIndex < ADJTIMESLOT) && ADJTIMESLOT>0)
	{
		IniPos = frameWidth*(partIndex)/ADJTIMESLOT;
		EndPos = frameWidth*(partIndex+1)/ADJTIMESLOT;
	}
	else
	{
		IniPos = 0;
		EndPos = 0;
	}
	//if ( 0 )
	//{
	//	markNorm = markValue;
	//}


	for(CurH = IniPos; CurH < EndPos; CurH++)
	{
		//avgBLKN = 0;
		//normFac = 0;
		int L1 = __minmax(CurH-1, 0, frameWidth-1);
		int L2 = __minmax(CurH-2, 0, frameWidth-1);
		int R1 = __minmax(CurH+1, 0, frameWidth-1);
		int R2 = __minmax(CurH+2, 0, frameWidth-1);
		long maxN = __max(__max(histogram[L1], histogram[L2]), __max(histogram[R1], histogram[R2]));

		if (histogram[CurH] == 0)
				(*zeroCnt)++;
		if ( __max(histogram[CurH], maxN) == histogram[CurH])
				{
			BLKN_adj[CurH] = markValue;
			BLKN_adj[L1] = markValue;
			BLKN_adj[R1] = markValue;

			CurH += 1;
			if (histogram[CurH] == 0)
				(*zeroCnt)++;
			}
			else
			BLKN_adj[CurH] = markDefValue;



		if (BLKN_adj[CurH] > markDefValue)
			valid++;
	}

	return valid;
}

void DBK_bypassmark(MS_U8* BLKN_mark, MS_U8 *BLKN_adj)
{
	int bin;
	for ( bin=0; bin<frameWidth;bin++ )
	{
		BLKN_mark[bin] = BLKN_adj[bin];
	}
}
int DBK_Mark(MS_U8* BLKN_mark, MS_U8 *BLKN_adj, int width, int iniIndex, MS_U8 markDefValue)//, MS_BOOL *BLKN_cpx)
{
	int restart=0;
	int misscnt = 0;
	int ThValue = 0;
	int hitrate = 0;
	int bin, CurH;
	int candiSP, sp;
	int cnt = 0;
	MS_BOOL hit;
	int offset;
	int n8[50];
	int maxOri, maxOriInd, pos_fac, range, dummy, r;
	int maxLocal = 0;
	int sndLocal = 0;
	int maxIndex = 0;
	int sndIndex = 0;
	misscnt = 0;
	//initialize start points
	sp = iniIndex;
	offset = 4;//8;
	if (width >= 16)
		offset = 8;//16;
	range = (width*2+4)>>3;
	r = (width+4)/8;
	dummy = ThValue *(width + offset)/offset;

	//sp

    sp=__minmax(sp, 1, frameWidth-2);
	BLKN_mark[sp] = BLKN_adj[sp];

    if (sp>=1)
	BLKN_mark[sp-1] = BLKN_adj[sp-1];

    if (sp<frameWidth-1)
	BLKN_mark[sp+1] = BLKN_adj[sp+1];

	hitrate++;
	cnt++;

	//right hand side
	for ( CurH=sp+width; CurH<frameWidth-(width-1)/2; )
	{
		maxOri = 0;
		maxOriInd = -1;
		maxLocal = 0;
		sndLocal = 0;
		maxIndex = -1;
		sndIndex = 0;
		for (bin=0; bin<width;bin++)
		{
			int curIndex = __minmax(CurH+(bin-(width-1)/2), 0, frameWidth-1);
			if( (BLKN_adj[curIndex]-markDefValue) > maxOri)
			{
				maxOri = BLKN_adj[curIndex]-markDefValue;
				maxOriInd = bin;
			}
			if ( (bin<=width/4) || (bin>=width*3/4) )
				n8[bin] =0;
			else
			{
				pos_fac = (width-1)/2 - abs(bin-(width-1)/2) + offset; //16 = offset
				n8[bin]= ((BLKN_adj[curIndex]-markDefValue) * pos_fac + (offset/2) )/offset;
				if ( n8[bin] > maxLocal)
				{
					maxLocal = n8[bin];
					maxIndex = bin;
				}
			}
		}
		int curIndex;
		if (maxIndex!=-1)
		{
		for (bin=(0-range); bin<=range; bin++)
		{
			if( (bin!=0 ) && ((maxIndex+bin) >=0) && ((maxIndex+bin) < width) )
			{
				if (n8[maxIndex+bin] > sndLocal)
				{
					sndLocal = n8[maxIndex+bin];
					sndIndex = bin; //related to maxIndex
				}
			}
		}
		curIndex = __minmax(CurH+(maxIndex-(width-1)/2), 0, frameWidth-1);
		hit = ( maxOri <= BLKN_adj[curIndex]-markDefValue )|| ( (maxOriInd>=__min(maxIndex,maxIndex+sndIndex) )&& (maxOriInd <= __max(maxIndex,maxIndex+sndIndex) ) );
		}
		else
		{
			curIndex = __minmax(CurH, 0, frameWidth-1);
			hit = 0;
		}

		if ( (!hit) || (maxLocal <= dummy) )
		{
			misscnt++;
		}

		if ((maxLocal > dummy) && (misscnt <= 2))
		{
			CurH = CurH+(maxIndex-(width-1)/2); //adjust to max position

			if (hit)
			{

				misscnt=0;
				BLKN_mark[CurH] = BLKN_adj[CurH];
			}
			else
			{
				BLKN_mark[CurH] = BLKN_adj[CurH]/2;
			}
			curIndex = __minmax(CurH+sndIndex, 0, frameWidth-1);
			if (BLKN_adj[curIndex]-markDefValue > ThValue ) // fill the gap between top 2 strength within range 4
			{
				if ( (sndIndex <= range-2 ) && (sndIndex > 0) )
				{
					if ( (CurH-1) >=0 )
						BLKN_mark[CurH-1] = BLKN_adj[CurH-1];
					if ( (CurH+sndIndex+1) < frameWidth)
						BLKN_mark[CurH+sndIndex+1] = BLKN_adj[CurH+sndIndex+1];
				}
				else if ( (sndIndex <0) && (sndIndex >= ((-1)*(range-2)) ) )
				{
					if ( (CurH+1) < frameWidth)
						BLKN_mark[CurH+1] = BLKN_adj[CurH+1];
					if ( (CurH+sndIndex-1) >= 0 )
						BLKN_mark[CurH+sndIndex-1] = BLKN_adj[CurH+sndIndex-1];
				}
				for (bin=__min(0, sndIndex); bin<=__max(0, sndIndex);bin++)
				{
					if ((CurH+bin >= 0) && (CurH+bin <frameWidth))
						BLKN_mark[CurH+bin] = BLKN_adj[CurH+bin];
				}
				CurH = CurH + ((sndIndex)/2); // move to center
			}
			CurH += width;
			hitrate++;
			cnt++;
		}
		else if (misscnt > 2)
		{
			candiSP = DBK_StPtLeft( width, (MS_U8*)BLKN_adj, CurH, __min(CurH+40, frameWidth));
			if (candiSP != 9999)
			{
				restart++;
				if (restart > 5)
					return 0;
				if ( (abs(candiSP - CurH)%width) != 0 )
				{
					for (bin=-4;bin<=4;bin++)
					{
						if ( ((CurH+bin) <frameWidth) && (CurH+bin >=0) && (BLKN_mark[CurH+bin]>markDefValue) )
							BLKN_mark[CurH+bin] = markDefValue;
						if ( ((CurH-width+bin) <frameWidth) && ((CurH-width+bin) >=0) && (BLKN_mark[CurH-width+bin]>markDefValue) )
							BLKN_mark[CurH-width+bin] = markDefValue;
						if ( ((CurH-width*2+bin) <frameWidth) && ((CurH-width*2+bin) >=0) && (BLKN_mark[CurH-width*2+bin]>markDefValue) )
							BLKN_mark[CurH-width*2+bin] = markDefValue;
					}
				}

				misscnt = 0;
				CurH = candiSP;


				for (bin=-r;bin<=r;bin++)
				{
					if (((CurH+bin)>=0) && ((CurH+bin) <frameWidth) )
						BLKN_mark[CurH+bin] = BLKN_adj[CurH+bin];
					if (((CurH-width+bin)>=0) && ((CurH-width+bin) <frameWidth) )
						BLKN_mark[CurH-width+bin] = BLKN_adj[CurH-width+bin];
					if (((CurH-width*2+bin)>=0) && ((CurH-width*2+bin) <frameWidth) )
						BLKN_mark[CurH-width*2+bin] = BLKN_adj[CurH-width*2+bin];
				}

				CurH += width;
			}
			else
			{
				for (bin = CurH; bin< __min((CurH+40), frameWidth); bin= bin+width )
				{
					if (bin-width >= 0 )
					BLKN_mark[bin] = (BLKN_adj[bin]+BLKN_adj[bin-width])/2;
				}
				CurH += 40;

			}
			cnt++;
		}
		else
		{
			BLKN_mark[CurH] = __max(BLKN_adj[CurH]/2, markDefValue);
			CurH += width;
			cnt++;
		}
	}

	misscnt=0;
	//left hand side
	for (CurH=sp-width; CurH >= (width)/2;)
	{
		maxOri = 0;
		maxOriInd = -1;
		maxLocal = 0;
		sndLocal = 0;
		maxIndex = -1;
		sndIndex = 0;
		for (bin=0; bin<width;bin++)
		{
			int curIndex = __minmax(CurH+(bin-(width)/2), 0, frameWidth-1);
			if (BLKN_adj[curIndex]-markDefValue > maxOri)
			{
				maxOri = BLKN_adj[curIndex]-markDefValue;
				maxOriInd = bin;
			}
			if ( (bin<=(width/4)) || (bin>=(width*3/4)) )
				n8[bin] =0;
			else
			{
				pos_fac = (width)/2 - abs(bin-(width)/2) + offset; //16 = offset
				n8[bin]= ((BLKN_adj[curIndex]-markDefValue) * pos_fac + (offset/2) )/offset;
				if ( n8[bin] > maxLocal)
				{
					maxLocal = n8[bin];
					maxIndex = bin;
				}
			}
		}
		int curIndex = 0;
		if ( maxIndex != -1)
		{
		for (bin=(0-range); bin<=range; bin++)
		{
			if( (bin!=0) &&  ((maxIndex+bin) >=0) && ((maxIndex+bin) < width) )
			{
				if (n8[maxIndex+bin] > sndLocal)
				{
					sndLocal = n8[maxIndex+bin];
					sndIndex = bin; //related to maxIndex
				}
			}
		}
		curIndex = __minmax(CurH+(maxIndex-(width+1)/2), 0, frameWidth-1);
		hit = ( maxOri <= BLKN_adj[curIndex]-markDefValue ) || ( (maxOriInd>=__min(maxIndex,maxIndex+sndIndex) )&& (maxOriInd <= __max(maxIndex,maxIndex+sndIndex) ) );
		}
		else
		{
		curIndex = __minmax(CurH, 0, frameWidth-1);
		hit = 0;

		}
		if ( (!hit) || (maxLocal <= dummy) )
		{

			misscnt++;
		}
		if ((maxLocal > dummy) && (misscnt <= 2) )
		{
			CurH = CurH+(maxIndex-(width)/2); //adjust to max position

			if (hit)
			{

				misscnt=0;
				BLKN_mark[CurH] = BLKN_adj[CurH];
			}
			else
			{
				BLKN_mark[CurH] = __max(BLKN_adj[CurH]/2, markDefValue);
			}
			curIndex = __minmax(CurH+sndIndex, 0, frameWidth-1);
			if (BLKN_adj[curIndex]-markDefValue > ThValue )// fill the gap between top 2 strength within range 4
			{
				if ( (sndIndex <=range-2) && (sndIndex > 0) )
				{
					if (CurH-1 >= 0)
						BLKN_mark[CurH-1] = BLKN_adj[CurH-1];
					if (CurH+sndIndex+1 < frameWidth)
						BLKN_mark[CurH+sndIndex+1] = BLKN_adj[CurH+sndIndex+1];
				}
				else if ( (sndIndex <0) && (sndIndex >= (-1)*(range-2)) )
				{
					if (CurH+1 < frameWidth)
						BLKN_mark[CurH+1] = BLKN_adj[CurH+1];
					if (CurH+sndIndex-1 >= 0)
						BLKN_mark[CurH+sndIndex-1] = BLKN_adj[CurH+sndIndex-1];
				}
				for (bin=__min(0, sndIndex); bin<=__max(0, sndIndex);bin++)
				{
					if ( ((CurH+bin) >= 0) && ((CurH+bin)<frameWidth) )
						BLKN_mark[CurH+bin] = BLKN_adj[CurH+bin];
				}
				CurH = CurH + ((sndIndex)/2); // move to center
			}
			CurH -= width;
			hitrate++;
			cnt++;
		}
		else if (misscnt > 2)
		{
			candiSP = DBK_StPtLeft(width, BLKN_adj, __max((CurH-40), 0), CurH+1);
			if (candiSP != 9999)
			{
				restart++;
				if (restart > 5)
					return 0;
				if ( (abs(candiSP - CurH)%width) != 0 )
				{
					for (bin=-4;bin<=4;bin++)
					{
						if ( ((CurH+bin) >=0) && ((CurH+bin) <  frameWidth) && (BLKN_mark[CurH+bin] > markDefValue) )
							BLKN_mark[CurH+bin] = markDefValue;
						if ( ((CurH+bin+width) >=0) && ((CurH+width+bin) <  frameWidth) && (BLKN_mark[CurH+width+bin] > markDefValue))
							BLKN_mark[CurH+width+bin] = markDefValue;
						if ( ((CurH+bin+width*2) >=0) && ((CurH+width*2+bin) <  frameWidth) && (BLKN_mark[CurH+width*2+bin] > markDefValue))
							BLKN_mark[CurH+width*2+bin] = markDefValue;
					}
				}

				misscnt = 0;
				CurH = candiSP;


				for (bin=-r;bin<=r;bin++)
				{
					if ((CurH+bin >= 0) && (CurH+bin <frameWidth) )
						BLKN_mark[CurH+bin] = BLKN_adj[CurH+bin];
					if (((CurH+width+bin) >= 0) && (CurH+width+bin <frameWidth) )
						BLKN_mark[CurH+width+bin] = BLKN_adj[CurH+width+bin];
					if (((CurH+width*2+bin) >= 0) && ((CurH+width*2+bin) <frameWidth) )
						BLKN_mark[CurH+width*2+bin] = BLKN_adj[CurH+width*2+bin];
				}
				CurH -= width;
			}
			else
			{
				for (bin = CurH; bin > __max((CurH-40), 0); bin= bin-width )
				{
					if (bin+width < frameWidth)
					BLKN_mark[bin] = (BLKN_adj[bin]+BLKN_adj[bin+width])/2;
				}
				CurH -= 40;
			}
			cnt++;
		}
		else
		{
			BLKN_mark[CurH] = __max(BLKN_adj[CurH]/2, markDefValue);
			CurH -= width;
			cnt++;
		}
	}
	//return 256;
	if (cnt>0)
	return (hitrate*256/cnt-8*restart);
	else
		return 0;
}
int readHistogramPart(void *pInstance,long* histogram, int partIndex, int SRAMTIMESLOT)
{
    int data_ack_en;
    int bin;
	int IniPos, EndPos;
	if( (partIndex < SRAMTIMESLOT) && (SRAMTIMESLOT!=0) )
	{
		IniPos = frameWidth*partIndex/SRAMTIMESLOT;
		EndPos = frameWidth*(partIndex+1)/SRAMTIMESLOT;
	}
	else
	{
		IniPos = 0;
		EndPos = 0;
	}
        //--------- Blockiness Histogram Request start ---------//
	if (partIndex == 0)
	{
        // write 1 to request register
        MApi_XC_W2BYTEMSK(REG_SC_BK0C_0D_L, 1, 0x0001);
	}

	if (1)
	{
        //--------- Blockiness Histogram Acknowledge start ---------//
		data_ack_en = MApi_XC_R2BYTEMSK(REG_SC_BK0C_0D_L, 0x0002);
        //--------- Function Process start ---------//
        if(data_ack_en)
        {
            // write 1 to sram_io_en register
            MApi_XC_W2BYTEMSK(REG_SC_BK0C_0D_L, 0x0100, 0x0100);
			for(bin = IniPos; bin < EndPos; bin++)
            {
				MApi_XC_W2BYTEMSK(REG_SC_BK0C_0E_L, (MS_U16)(0x8000 + bin), 0xFFFF);
				// read data from selected address
				histogram[bin] = MApi_XC_R2BYTEMSK(REG_SC_BK0C_0F_L, 0xFFFF);
			}
			if (partIndex == SRAMTIMESLOT-1)
			{
				MApi_XC_W2BYTEMSK(REG_SC_BK0C_0D_L, 0, 0x0001);
            }
            return 1;
        }
		else//(!data_ack_en)
        {
            return 0;
        }
    }
}
#if 0
int readHistogramPart(void *pInstance,long* histogram, int partIndex)
{
	int data_ack_en;
	int bin;
	int IniPos, EndPos;
	const int MaxPartIndex = 2;
	if (partIndex == 0)
	{
		IniPos = 0;
		EndPos = frameWidth/3;
	}
	else if (partIndex == 1)
	{
		IniPos = frameWidth/3;
		EndPos = frameWidth*2/3;
	}
	else if (partIndex == 2)
	{
		IniPos = frameWidth*2/3;
		EndPos = frameWidth;
	}
	else
	{
		IniPos = 0;
		EndPos = 0;
	}
	//--------- Blockiness Histogram Request start ---------//
	if (partIndex == 0)
	{
		// write 1 to request register
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_0D_L, 1, 0x0001);
	}

	if (1)
	{
		//--------- Blockiness Histogram Acknowledge start ---------//
		data_ack_en = MApi_XC_R2BYTEMSK(REG_SC_BK0C_0D_L, 0x0002);
		//--------- Function Process start ---------//
		if(data_ack_en)
		{
			// write 1 to sram_io_en register
			MApi_XC_W2BYTEMSK(REG_SC_BK0C_0D_L, 0x0100, 0x0100);
			for(bin = IniPos; bin < EndPos; bin++)
			{
				MApi_XC_W2BYTEMSK(REG_SC_BK0C_0E_L, (MS_U16)(0x8000 + bin), 0xFFFF);
				// read data from selected address
				histogram[bin] = MApi_XC_R2BYTEMSK(REG_SC_BK0C_0F_L, 0xFFFF) ;
			}
			if (partIndex == MaxPartIndex)
				MApi_XC_W2BYTEMSK(REG_SC_BK0C_0D_L, 0, 0x0001);
			return 1;
		}
		else//(!data_ack_en)
		{
			return 0;
		}
	}
}
#endif
int resetBKNPart(int value, int partIndex, int* blocknessCheck)
{
	int bin;
	int IniPos, EndPos;
	if (partIndex == 0)
	{
		IniPos = 0;
		EndPos = frameWidth/3;
	}
	else if (partIndex == 1)
	{
		IniPos = frameWidth/3;
		EndPos = frameWidth*2/3;
	}
	else if (partIndex == 2)
	{
		IniPos = frameWidth*2/3;
		EndPos = frameWidth;
	}
	else
	{
		IniPos = 0;
		EndPos = 0;
	}

	if (1)
	{
		for(bin=IniPos; bin<EndPos;bin++)
		{
			blocknessCheck[bin] = value;
		}
	}
	return	1;
}
int writeToSRAMPart(void *pInstance,int* BlockinessCheck, int partIndex, int SRAMTIMESLOT)
{
    int bin;
    int ack_flag =0;
    int count = 0;
    MS_U32 Data0, Data1,  Data2,  Data3, Data;
	int IniPos, EndPos;

	if( (partIndex < SRAMTIMESLOT) && (SRAMTIMESLOT!=0) )
	{
		IniPos = frameWidth*partIndex/SRAMTIMESLOT;
		EndPos = frameWidth*(partIndex+1)/SRAMTIMESLOT;
	}
	else
	{
		IniPos = 0;
		EndPos = 0;
    }

    //WRITE TO BLOCKNESS POSITION SRAM
    MApi_XC_W2BYTEMSK(REG_SC_BK0C_08_L,0x8000,0x8000);//blkwei  sram IO enable: h0008(0x08) 8000 = 15th bit = 1

        for(bin=IniPos; bin<EndPos;bin=bin+4)
        {
            Data0 = (BlockinessCheck[bin])    &0xF;
            Data1 = (BlockinessCheck[bin+1])&0xF;
            Data2 = (BlockinessCheck[bin+2])&0xF;
            Data3 = (BlockinessCheck[bin+3])&0xF;

            Data = (Data3<<12)|(Data2<<8)|(Data1<<4)|Data0; // 4 4bit combined to 16bit

            {
                MApi_XC_W2BYTEMSK(REG_SC_BK0C_08_L, bin/4,0x0FFF);//blkwei  sram io address
                MApi_XC_W2BYTEMSK(REG_SC_BK0C_0B_L,Data,0xFFFF);//blkwei  sram write data
                MApi_XC_W2BYTEMSK(REG_SC_BK0C_09_L,0x8000,0x8000);//blkwei  sram write enable
                ack_flag = (MApi_XC_R2BYTEMSK(REG_SC_BK0C_08_L,0x4000)>>14); // 14th bit of h0008
            }

            //Sleep( 1);
            count = 0;
            while ((ack_flag==0) && (count<=10))
            {
            count++;

                    MApi_XC_W2BYTEMSK(REG_SC_BK0C_08_L, bin/4,0x0FFF);//blkwei  sram io address
                    MApi_XC_W2BYTEMSK(REG_SC_BK0C_0B_L,Data,0xFFFF);//blkwei  sram write data
                    MApi_XC_W2BYTEMSK(REG_SC_BK0C_09_L,0x8000,0x8000);//blkwei  sram write enable
                    ack_flag = (MApi_XC_R2BYTEMSK(REG_SC_BK0C_08_L,0x4000)>>14); // 14th bit of h0008
                }
            }

//MApi_XC_W2BYTEMSK(REG_SC_BK0C_08_L,0x2000,0x2000);//blkwei  fun en  h0008(0x08) 8000 = 13th bit = 1

    return 	1;
}

int SDHDModeCheck( MS_U8 u8SD2HDValue, int width) //Register Settings to hardware
{
	//	MS_U16 test_switch = 0;//(MApi_XC_R2BYTE(REG_SC_BK30_0B_L) & 0x8000 )>>15;
	//	if (test_switch == 0) //new settings
	{
		if( (u8SD2HDValue>=60) && (frameWidth>0x700) && (width >= 0x10) ) //SD to HD mode
		{
			//MApi_XC_W2BYTEMSK(REG_SC_BK0C_41_L, 0x1, 0x0001); // HD_enable
			//MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0x6, 0x0007); // pulse_filter_type
			return 2;
		}
		else if ((frameWidth>0x50F) && (width >= 0x10) ) //Scaled 2x+ mode
		{
			//				MApi_XC_W2BYTEMSK(REG_SC_BK0C_41_L, 0x1, 0x0001); // HD_enable
			//MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0x6, 0x0007); // pulse_filter_type
			return 5;
		}
		else
		{
			if( (frameWidth>0x700) && (width ==0x8) ) // pure HD mode
			{
				//        	MApi_XC_W2BYTEMSK(REG_SC_BK0C_41_L, 0x0, 0x0001); // HD mode
				//MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0x2, 0x0007); // HD pulse_filter_type
				return 1;
			}
			else if ((frameWidth>0x700) && (width !=0x8)) // scaled HD, like 1440to1920
			{
				//          MApi_XC_W2BYTEMSK(REG_SC_BK0C_41_L, 0x0, 0x0001); // HD mode
				//MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0x2, 0x0007); // pulse_filter_type
				return 3;
			}
			else if (width ==0x8)// pure SD mode
			{
				//        	MApi_XC_W2BYTEMSK(REG_SC_BK0C_41_L, 0x0, 0x0001); // SD mode
				//MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0x2, 0x0007); // pulse_filter_type
				return 0;
			}
			else //unknown mode
			{
				//        	MApi_XC_W2BYTEMSK(REG_SC_BK0C_41_L, 0x0, 0x0001); // SD mode
				//MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0x2, 0x0007); // pulse_filter_type
				return 4;
			}
		}
	}
}

//---------------- 3DorderDetect begin --------------//
#define sensio_thrd 3
#define format_up_thrd 500
#define format_low_thrd 100
int MDrv_SC_3DorderDetect( void *pInstance)
{
	MApi_XC_W2BYTEMSK(REG_SC_BK3A_01_L, 0x0000, 0x0010 );  // turn on 3d format detection

	int PA_count = MApi_XC_R2BYTE(REG_SC_BK3A_20_L);
	int LA_count = MApi_XC_R2BYTE(REG_SC_BK3A_21_L);
	int CB_count = MApi_XC_R2BYTE(REG_SC_BK3A_22_L);
	int sensio_l_even = MApi_XC_R2BYTE(REG_SC_BK3A_23_L);
	int sensio_l_odd  = MApi_XC_R2BYTE(REG_SC_BK3A_24_L);
	int sensio_r_even = MApi_XC_R2BYTE(REG_SC_BK3A_25_L);
	int sensio_r_odd  = MApi_XC_R2BYTE(REG_SC_BK3A_26_L);

	int sensio_flag = 0;
	int format = 0;  // 0:2D, 1:PA, 2:LA, 3:CB, 4:Sensio

		int sensio_flag_l_ratio =0;
		int sensio_flag_r_ratio =0;
//	int sensio_L_is_EVEN= 0;  // =1 EVEN;  =0 ODD
//		int sensio_R_is_EVEN= 0;

		if ((sensio_l_even> sensio_l_odd) && (sensio_l_odd!=0))
		{
			sensio_flag_l_ratio = sensio_l_even/sensio_l_odd;
//		sensio_L_is_EVEN =1;
		}
		else if ((sensio_l_odd> sensio_l_even) && (sensio_l_even!=0))
			sensio_flag_l_ratio = sensio_l_odd/sensio_l_even;
		else
			sensio_flag_l_ratio =-1;

		if ((sensio_r_even> sensio_r_odd) && (sensio_r_odd!=0))
		{
			sensio_flag_r_ratio = sensio_r_even/sensio_r_odd;
//			sensio_R_is_EVEN =1;
		}
		else if ((sensio_r_odd> sensio_r_even) && (sensio_r_even!=0))
			sensio_flag_r_ratio = sensio_r_odd/sensio_r_even;
		else
			sensio_flag_r_ratio =-1;

		if (sensio_flag_l_ratio> sensio_thrd && sensio_flag_r_ratio> sensio_thrd)
			sensio_flag =1;
		else
			sensio_flag =0;


		// 0:2D, 1:PA, 2:LA, 3:CB, 4:Sensio
		if(sensio_flag)
		{
			format = 4;
			//MApi_XC_W2BYTEMSK(REG_SC_BK30_50_L, 0x0007, 0x0007 );  // debug light
		}
		else
		{
				if  ( ( LA_count < CB_count ) && ( PA_count < CB_count ) )
				{
					format = 3;
					//MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x0004, 0x0007 );  // debug light
				}
				else if ( ( LA_count < format_low_thrd ) && (PA_count > format_up_thrd) )
				{
					format = 1;
					//MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x0001, 0x0007 );  // debug light
				}
				else if ( ( PA_count < format_low_thrd ) && (LA_count > format_up_thrd) )
				{
					format = 2;
					//MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x0002, 0x0007 );  // debug light
				}
				else
				{
					format = 0;
					//MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x0000, 0x0007 );  // debug light
				}

		}

	//printf( "3D format is : %d\n", format );
	//printf( "PA_count : %d\n", PA_count );
	//printf( "LA_count : %d\n", LA_count );
	//printf( "CB_count : %d\n", CB_count );
	//printf( "sensio_l_even : %d\n", sensio_l_even );
	//printf( "sensio_l_odd  : %d\n", sensio_l_odd );
	//printf( "sensio_r_even : %d\n", sensio_r_even );
	//printf( "sensio_r_odd  : %d\n", sensio_r_odd );
	return format;

}

//----------------- 3DorderDetect end ---------------//

//------------- 3DorderDetect_TBLR begin -----------//
//       only support progressive input soure  	    //
//      every 4 horizontal pixel made one decision  //
//      histogram data has been shifted 1 bit	    //
#define TBLR_low_thrd  16
#define TBLR_high_thrd 20
int MDrv_SC_3DorderDetect_TBLR(void *pInstance)
{

  MApi_XC_W2BYTEMSK(REG_SC_BK3A_01_L, 0x0000, 0x0010 );  // turn on 3d format detection
	int FrameWidth  = MApi_XC_R2BYTEMSK(REG_SC_BK12_0F_L, 0xFFFF);
	int FrameHeight = heightEstimate(FrameWidth);
	int totalPix = FrameWidth*FrameHeight/4;

	//printf("FrameWidth = %d, FrameHeight= %d \n", FrameWidth, FrameHeight );

	MS_BOOL TBLR_MODE_flag = true ;
	static int TBLR_MODE_CNT = 0 ;
	static MS_BOOL isHistSet = false;

	int histbin = 0;
	int hist0[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int hist1[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int hist2[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int hist3[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	int sum0 = 0;
	int sum1 = 0;
	int sum2 = 0;
	int sum3 = 0;

	int Psum0 = 0;
	int Psum1 = 0;
	int Psum2 = 0;
	int Psum3 = 0;

	int diff_02= 0;
	int diff_13= 0;
	int diff_03= 0;
	int diff_12= 0;
	int shift = 0;

	int  sum_Diff_02 = 0;
	int  sum_Diff_13 = 0;
	int  sum_Diff_03 = 0;
	int  sum_Diff_12 = 0;
	int  sum_Diff_total = 0;

	static int  sum_Diff_TB02 = 0;
	static int  sum_Diff_TB13 = 0;
	static int  sum_Diff_TB03 = 0;
	static int  sum_Diff_TB12 = 0;
	static int  sum_Diff_LR02 = 0;
	static int  sum_Diff_LR13 = 0;
	static int  sum_Diff_LR03 = 0;
	static int  sum_Diff_LR12 = 0;
	static int  curStatus = 0;


	if((TBLR_MODE_CNT%2)==0)
		TBLR_MODE_flag= true;
	else if((TBLR_MODE_CNT%2)==1)
		TBLR_MODE_flag= false;

	if (TBLR_MODE_flag)  // TB
	{
			//printf("----- TB histogram------\n" );
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_40_L, 0x0000,					 0x01FFF );  		// hist win0 x_start
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_41_L, 0x0000, 				 0x01FFF );  		// hist win0 y_start
  			MApi_XC_W2BYTEMSK(REG_SC_BK3A_42_L, FrameWidth 	  -0x0001, 	 0x01FFF );			// hist win0 x_end
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_43_L, FrameHeight/4 -0x0001,   0x01FFF );			// hist win0 y_end

			MApi_XC_W2BYTEMSK(REG_SC_BK3A_44_L, 0x0000 , 				 0x01FFF );  		// hist win1 x_start
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_45_L, FrameHeight/4  , 		 0x01FFF );  		// hist win1 y_start
  			MApi_XC_W2BYTEMSK(REG_SC_BK3A_46_L, FrameWidth 	  -0x0001,	 0x01FFF );			// hist win1 x_end
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_47_L, FrameHeight/2 -0x0001,	 0x01FFF ); 		// hist win1 y_end


			MApi_XC_W2BYTEMSK(REG_SC_BK3A_48_L, 0x0000,		 			 0x01FFF );  		// hist win2 x_start
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_49_L, FrameHeight/2 , 		 0x01FFF );  		// hist win2 y_start
 		 	MApi_XC_W2BYTEMSK(REG_SC_BK3A_4A_L, FrameWidth 		-0x0001, 0x01FFF );			// hist win2 x_end
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_4B_L, FrameHeight*3/4 -0x0001, 0x01FFF );			// hist win2 y_end

			MApi_XC_W2BYTEMSK(REG_SC_BK3A_4C_L, 0x0000,					 0x01FFF );  		// hist win3 x_start
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_4D_L, FrameHeight*3/4 ,		 0x01FFF );  		// hist win3 y_start
 			MApi_XC_W2BYTEMSK(REG_SC_BK3A_4E_L, FrameWidth  -0x0001, 	 0x01FFF );  		// hist win3 x_end
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_4F_L, FrameHeight -0x0001, 	 0x01FFF );  		// hist win3 y_end

	}
	else
	{
		  //printf("----- LR histogram------\n" );
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_40_L, 0x0000, 0x01FFF );  						// hist win0 x_start
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_41_L, 0x0000, 0x01FFF );  						// hist win0 y_start
  			MApi_XC_W2BYTEMSK(REG_SC_BK3A_42_L, FrameWidth/4 	-0x0001, 	0x01FFF );		// hist win0 x_end
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_43_L, FrameHeight		-0x0001,	0x01FFF );		// hist win0 y_end

			MApi_XC_W2BYTEMSK(REG_SC_BK3A_44_L, FrameWidth/4, 				0x01FFF );  	// hist win1 x_start
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_45_L, 0x0000  ,					0x01FFF );		// hist win1 y_start
  			MApi_XC_W2BYTEMSK(REG_SC_BK3A_46_L, FrameWidth/2 	-0x0001,	0x01FFF );		// hist win1 x_end
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_47_L, FrameHeight		-0x0001 ,	0x01FFF ); 		// hist win1 y_end

			MApi_XC_W2BYTEMSK(REG_SC_BK3A_48_L, FrameWidth/2,		 		0x01FFF );  	// hist win2 x_start
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_49_L, 0x0000,					 	0x01FFF );  	// hist win2 y_start
 		 	MApi_XC_W2BYTEMSK(REG_SC_BK3A_4A_L, FrameWidth*3/4 	-0x0001,	0x01FFF );		// hist win2 x_end
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_4B_L, FrameHeight		-0x0001,	0x01FFF );		// hist win2 y_end

			MApi_XC_W2BYTEMSK(REG_SC_BK3A_4C_L, FrameWidth*3/4,				0x01FFF );  	// hist win3 x_start
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_4D_L, 0x0000,						0x01FFF );  	// hist win3 y_start
 			MApi_XC_W2BYTEMSK(REG_SC_BK3A_4E_L, FrameWidth  -0x0001, 		0x01FFF );  	// hist win3 x_end
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_4F_L, FrameHeight -0x0001, 		0x01FFF );  	// hist win3 y_end

	}

	if( !isHistSet )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK3A_39_L, 0x0001, 0x0001);
		isHistSet = true;
		//printf( "set trigger bit\n" );
	}
	MS_BOOL histReady = MApi_XC_R2BYTEMSK(REG_SC_BK3A_39_L, 0x0080) >> 7;  // if histogram is ready


  if( histReady )
  {
  	for ( histbin = 0; histbin< 32 ; histbin++ )
		{
			// every 4 pixel in horizontal
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_50_L, histbin, 0x001F );  		// select hist bin

			MApi_XC_W2BYTEMSK(REG_SC_BK3A_50_L, 0x0000,	 0x0300 );  		// select hist window0
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_51_L, 0x0001,	 0x0001 );  		// toggle histogram read
			hist0[histbin] = MApi_XC_R2BYTE(REG_SC_BK3A_52_L);

			MApi_XC_W2BYTEMSK(REG_SC_BK3A_50_L, 0x0100,	 0x0300 );  		// select hist window1
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_51_L, 0x0001,  0x0001 );  		// toggle histogram read
			hist1[histbin] = MApi_XC_R2BYTE(REG_SC_BK3A_52_L);

			MApi_XC_W2BYTEMSK(REG_SC_BK3A_50_L, 0x0200,  0x0300 );  		// select hist window2
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_51_L, 0x0001,  0x0001 );  		// toggle histogram read
			hist2[histbin] = MApi_XC_R2BYTE(REG_SC_BK3A_52_L);

			MApi_XC_W2BYTEMSK(REG_SC_BK3A_50_L, 0x0300,  0x0300 );  		// select hist window3
			MApi_XC_W2BYTEMSK(REG_SC_BK3A_51_L, 0x0001,  0x0001 );  		// toggle histogram read
			hist3[histbin] = MApi_XC_R2BYTE(REG_SC_BK3A_52_L);

			sum0 = sum0 + hist0[histbin]*histbin;
			sum1 = sum1 + hist1[histbin]*histbin;
			sum2 = sum2 + hist2[histbin]*histbin;
			sum3 = sum3 + hist3[histbin]*histbin;


			Psum0 = Psum0 + hist0[histbin];
			Psum1 = Psum1 + hist1[histbin];
			Psum2 = Psum2 + hist2[histbin];
			Psum3 = Psum3 + hist3[histbin];

			//printf( "hist[%d] = %d  %d  %d  %d \n", histbin, hist0[histbin], hist1[histbin], hist2[histbin], hist3[histbin]);

		}

  	//printf( "hist total pixel = %d  %d  %d  %d \n", Psum0, Psum1,Psum2, Psum3 );


  	sum0 = 4* sum0 / totalPix;
		sum1 = 4* sum1 / totalPix;
		sum2 = 4* sum2 / totalPix;
		sum3 = 4* sum3 / totalPix;

		shift = ((sum0+ sum1) - (sum2 +  sum3))/2;

		if ((sum0 - sum2)*(sum1 - sum3) <= 0)
			shift =0;

		if( abs(shift) >2)
			shift =0;

		//printf("sum0= %d, sum1=  %d, sum2=  %d, sum3=  %d, shift= %d \n", sum0, sum1, sum2, sum3, shift );

		//sum all the diff
		sum_Diff_02= 0;
		sum_Diff_13= 0;
		sum_Diff_03= 0;
		sum_Diff_12= 0;

		for ( histbin = 0; histbin< 32 ; histbin++)
		{

			  diff_02= abs(hist0[histbin] - hist2[histbin-shift]);
				diff_13= abs(hist1[histbin] - hist3[histbin-shift]);
				diff_03= abs(hist0[histbin] - hist3[histbin-shift]);
				diff_12= abs(hist1[histbin] - hist2[histbin-shift]);

				if (TBLR_MODE_flag )
				{
					sum_Diff_02 = sum_Diff_02 + diff_02;
					sum_Diff_13 = sum_Diff_13 + diff_13;
					sum_Diff_03 = sum_Diff_03 + diff_03;
					sum_Diff_12 = sum_Diff_12 + diff_12;

				//	printf(" sum_diff_TB : %d,  %d, %d, %d, , %d, %d, %d, %d \n", sum_Diff_02, sum_Diff_13, sum_Diff_03, sum_Diff_12 , diff_02, diff_13, diff_03, diff_12);
				}
				else
				{
					sum_Diff_02 = sum_Diff_02 + diff_02;
					sum_Diff_13 = sum_Diff_13 + diff_13;
					sum_Diff_03 = sum_Diff_03 + diff_03;
					sum_Diff_12 = sum_Diff_12 + diff_12;

				//	printf(" sum_diff_LR : %d,  %d, %d, %d, , %d, %d ,%d ,%d \n", sum_Diff_02, sum_Diff_13, sum_Diff_03, sum_Diff_12,  diff_02, diff_13, diff_03, diff_12);
				}
		}

		sum_Diff_total = __max( sum_Diff_02+ sum_Diff_13+ sum_Diff_03+ sum_Diff_12, 1);

		//normalization
		if (TBLR_MODE_flag )
		{
			sum_Diff_TB02= sum_Diff_02*100/sum_Diff_total;
			sum_Diff_TB13= sum_Diff_13*100/sum_Diff_total;
			sum_Diff_TB03= sum_Diff_03*100/sum_Diff_total;
			sum_Diff_TB12= sum_Diff_12*100/sum_Diff_total;
		}
		else
		{
			sum_Diff_LR02= sum_Diff_02*100/sum_Diff_total;
			sum_Diff_LR13= sum_Diff_13*100/sum_Diff_total;
			sum_Diff_LR03= sum_Diff_03*100/sum_Diff_total;
			sum_Diff_LR12= sum_Diff_12*100/sum_Diff_total;
		}


		if(TBLR_MODE_CNT%2)
		{

			//	printf("normalized TBdiff : %d %d %d %d \n", sum_Diff_TB02, sum_Diff_TB13, sum_Diff_TB03, sum_Diff_TB12 );
			//	printf("normalized LRdiff : %d %d %d %d \n", sum_Diff_LR02, sum_Diff_LR13, sum_Diff_LR03, sum_Diff_LR12 );

				if( __max(sum_Diff_LR02, sum_Diff_LR13) <  __max(sum_Diff_LR03, sum_Diff_LR12) &&  __min (sum_Diff_LR02, sum_Diff_LR13) < TBLR_low_thrd && __max(sum_Diff_TB02, sum_Diff_TB13) > TBLR_high_thrd)
				{
					curStatus = 2;//LR
					//MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x0020, 0x0070 );  // debug light green
				}
				else if( __max(sum_Diff_TB02, sum_Diff_TB13)<  __max(sum_Diff_TB03, sum_Diff_TB12) && __min (sum_Diff_TB02, sum_Diff_TB13) < TBLR_low_thrd && __max(sum_Diff_LR02, sum_Diff_LR13) > TBLR_high_thrd)
				{
					curStatus = 1;//TB
					//MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x0010, 0x0070 );  // debug light red
				}
				else
				{
					curStatus = 0;//Video
					//MApi_XC_W2BYTEMSK(REG_SC_BK30_52_L, 0x0000, 0x0070 );  // debug light off
				}

				//printf("3d format is : %d \n", curStatus);

		}

		isHistSet = false;
		TBLR_MODE_CNT ++;
	}

		if (TBLR_MODE_CNT > 300)
		{
				TBLR_MODE_CNT = 0;
		}
		//printf("TBLR_MODE_CNT = %d\n", TBLR_MODE_CNT );

	return curStatus;
}
// -------------- 3DorderDetect_TBLR end ----------- //

//-------- 3D source format detection begin ---------//

int MDrv_SC_3DfmtDetect(void *pInstance)
{
	static int format3D = 0;			// 0:2D, 1:TB, 2:LR(SS), 3:PA, 4:LA, 5:CB, 6:Sensio
	int isCBPALASensio =  MDrv_SC_3DorderDetect(pInstance); // 0:2D, 1:PA, 2:LA, 3:CB, 4:Sensio
	int isTBLR =  MDrv_SC_3DorderDetect_TBLR(pInstance);					//0:2D, 1:TB, 2:LR(SS)

	if( isCBPALASensio )
		format3D = isCBPALASensio +2;
	else
		format3D = isTBLR;

		//printf("3D format is = %d\n", format3D );

	return format3D;

}

//--------- 3D source format detection end  ---------//

int FeedBackSD2HD(void *pInstance,int width, MS_U8 u8SD2HDValue) // for test to feedback
{

	MS_BOOL ifSD2HD = ((width >=20 )&& (width<=22) );

	if (ifSD2HD) //SD to HD format
	{
		return SDHDModeCheck( __min(u8SD2HDValue+20,63) , width);
	}
	else //pure SD or HD format
	{
		return SDHDModeCheck( u8SD2HDValue, width);
	}
}

void WidthToRegister(void *pInstance,int width) // Write width to register
{
	int   left1,right1;
	if(width<8)
	{
		width = 8;
		left1 = 8;
		right1 = 9;
	}
	else if (width==8) //test 0808 for performance
	{
		left1 = 8;
		right1 = 8;
	}
	else
	{

		left1 = width - 1; //fix prevent defect
		right1 = (width >=31) ? 31 : width + 1;
	}
	//write interval left && interval right
	MApi_XC_W2BYTEMSK(REG_SC_BK0C_39_L, (MS_U16)((left1<<8)|right1), 0xFFFF);
}
void WidthIIR_H(int width) // Width IIR Protection
{
	int UnstableWidthCnt = 0;
	static int widthPP = 8;
	static int widthP  = 8;
	static int widthC  = 8;

	if (widthPP!= widthP)
		UnstableWidthCnt++;
	if (widthP!=widthC)
		UnstableWidthCnt++;
	if (widthC!=width)
		UnstableWidthCnt++;
	if (UnstableWidthCnt == 0)
		WidthIIR = width;
	/*	  else if (UnstableWidthCnt == 3)//reduce DBK Strengh
	MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0x2000, 0xF000);
	else if (UnstableWidthCnt == 2)//reduce DBK Strengh
	MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0x4000, 0xF000);
	else if (UnstableWidthCnt == 1)//reduce DBK Strengh
	MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0x6000, 0xF000);
	*/
	widthPP = widthP;
	widthP = widthC;
	widthC = width;
	//DEBUG_LC_5F( UnstableWidthCnt );
}

static int dbkgain = 0;
static int cpxgain = 0;

int parameterControl(void *pInstance,int frm_Edg, int frm_blk, int frm_Det, int cpxGain, const MS_U32 _hpfHdCntReport, const MS_U32 _hpfSdCntReport)
{

	static MS_U32 HDCnt = 0;
	static MS_U32 SDCnt = 0;

	HDCnt = (_hpfHdCntReport + HDCnt*3 ) >> 2;
	SDCnt = (_hpfSdCntReport + SDCnt*3 ) >> 2;




        MS_BOOL IfComplex1 = (HDCnt > 64000) && (HDCnt <= 80000) && (SDCnt > 200000) && ( frm_blk > 20 );
        MS_BOOL IfComplex2 = (HDCnt > 80000) && (SDCnt > 200000) && ( frm_blk > 20 );
        MS_BOOL IfViolinLike = (IfComplex1 && (SDCnt > 400000 ) ) || (IfComplex2 && (SDCnt > HDCnt*4 ) ) ;
        int ViolinInc = -1;
        if (IfViolinLike && (HDCnt>0) )
                ViolinInc = (SDCnt/HDCnt) -3;
        MS_BOOL IfFlashStyle = (IfComplex1 && (SDCnt < HDCnt*4 ) ) || (IfComplex2 && (SDCnt < 300000 + (HDCnt-80000) ));
        int FlashInc = -1;
        if (IfFlashStyle && (HDCnt>0) )
                FlashInc = 4 - (SDCnt/HDCnt) ;


	int STR_THR_1, STR_THR_2, STR_THR_3, STR_THR_4;

	static int ViolinCnt = 0;
	static int FlashSCnt = 0;
	if (IfViolinLike)
	{
                        ViolinCnt = (ViolinCnt > 20) ? 20 : (ViolinCnt+ViolinInc );
	}
	else
	{
                        ViolinCnt = (ViolinCnt == 0) ? 0 : (ViolinCnt+ViolinInc );
	}
	if (IfFlashStyle)
	{
			FlashSCnt = (FlashSCnt > 20) ? 20 : (FlashSCnt+FlashInc );
	}
	else
	{
                        FlashSCnt = (FlashSCnt == 0) ? 0 : (FlashSCnt+FlashInc );
	}
	STR_THR_1 = 80;
	STR_THR_2 = 60;
	STR_THR_3 = 40;
	STR_THR_4 = 20;
	if (ViolinCnt >= 10)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_38_L, 0x02, 0x02); // coring
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_3D_L, 0x0F, 0xFF); // coring thrd
	}
	else if (frm_Edg > STR_THR_1 || (FlashSCnt >= 10 ) )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_38_L, 0x02, 0x02); // coring
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_3D_L, 0x00, 0xFF); // coring thrd
	}
	else if ( (frm_Edg <= STR_THR_1) && (frm_Edg > STR_THR_2) )
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_38_L, 0x02, 0x02); // coring
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_3D_L, 0x00, 0xFF); // coring thrd
	}
	else if ((frm_Edg <= STR_THR_2) && (frm_Edg > STR_THR_3))
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_38_L, 0x02, 0x02); // coring
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_3D_L, 0x04, 0xFF); // coring thrd
	}
	else if ((frm_Edg <= STR_THR_3) && (frm_Edg > STR_THR_4))
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_38_L, 0x02, 0x02); // coring on
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_3D_L, 0x07, 0xFF); // coring thrd
	}
	else if ((frm_Edg <= STR_THR_4) && (frm_Edg > 0))
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_38_L, 0x02, 0x02); // coring on
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_3D_L, 0x0A, 0xFF); // coring thrd
	}
	else
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_38_L, 0x02, 0x02); // coring on
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_3D_L, 0x0A, 0xFF); // coring thrd
	}

//	int dbkcase = 0;
	static int return_value = 0;

	int dbkgain_cur = 0;
	int cpxgain_cur = 0;
	if (FlashSCnt >= 10)
	{
		frm_blk = frm_blk * 2;
		frm_Det = frm_Det / 2;
		//PRT_TRP("Flash Style!!!\n");
		//MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0007, 0x0077); // extend 1
	}
	else if (ViolinCnt >= 10)
	{
		frm_blk = frm_blk / 2;
		frm_Det = frm_Det * 2;
		//PRT_TRP("Violin Style!!!\n");
		//MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0070, 0x0077); // extend 1
	}
	else
	{
		//MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0000, 0x0077); // extend 1
	}
	if (cpxGain > 0) //BW water flow
	{
		//PRT_TRP("BW water patch!!!\n");
		int cpxGainP = __minmax(8-cpxGain, 0, 0x8);

		dbkgain_cur = 0xF;
		cpxgain_cur = cpxGainP;

//		dbkcase = 0;
		return_value = 100;
		//return frm_Edg;
	}
#if 0	//20141002 mikR
	else if (frameWidth <= 800)
	{
//		MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0xA000, 0xA000); //DBK Gain
//		MApi_XC_W2BYTEMSK(REG_SC_BK60_31_L, 0x0000, 0xFFFF); //Complex Control

		dbkgain_cur = 0xA;
		cpxgain_cur = 0x0;

		MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0x5<<8, 0x0F00); // extend 1
//		dbkcase = 9;
		return_value = 100;

	}
#endif	//20141002 mikR
	else if (frm_blk > frm_Det*2 && frm_blk > 8)
	{
		//MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0xF000, 0xF000); //DBK Gain
		//MApi_XC_W2BYTEMSK(REG_SC_BK60_31_L, 0x0000, 0xFFFF); //Complex Control

		dbkgain_cur = 0xF;
		cpxgain_cur = 0x0;

//		dbkcase = 1;
		return_value = 100;

	}
	else if (frm_blk > frm_Det*3/2 && frm_blk > 8)
	{
		//MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0xC000, 0xF000); //DBK Gain
		//MApi_XC_W2BYTEMSK(REG_SC_BK60_31_L, 0x1111, 0xFFFF); //Complex Control
		dbkgain_cur = 0xD;
		cpxgain_cur = 0x0;


//		dbkcase = 2;
		return_value = 90;
	}
	else if (frm_blk > frm_Det && frm_blk > 8)
	{
		//MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0x8000, 0xF000); //DBK Gain
		//MApi_XC_W2BYTEMSK(REG_SC_BK60_31_L, 0x2222, 0xFFFF); //Complex Control
		dbkgain_cur = 0xA;
		cpxgain_cur = 0x2;

//		dbkcase = 3;
		return_value = 80;
	}
	else if (frm_blk > frm_Det*5/6 && frm_blk > 8)
	{
		//MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0x6000, 0xF000); //DBK Gain
		//MApi_XC_W2BYTEMSK(REG_SC_BK60_31_L, 0x3333, 0xFFFF); //Complex Control
		dbkgain_cur = 0x9;
		cpxgain_cur = 0x3;

//		dbkcase = 4;
		return_value = 70;
	}
	else if (frm_blk > frm_Det*4/6 && frm_blk > 8)
	{
		//MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0x5000, 0xF000); //DBK Gain
		//MApi_XC_W2BYTEMSK(REG_SC_BK60_31_L, 0x4444, 0xFFFF); //Complex Control
		dbkgain_cur = 0x8;
		cpxgain_cur = 0x4;

		//dbkcase = 5;
		return_value = 60;
	}
	else if (frm_blk > frm_Det*3/6 && frm_blk > 8)
	{
		//MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0x4000, 0xF000); //DBK Gain
		//MApi_XC_W2BYTEMSK(REG_SC_BK60_31_L, 0x4444, 0xFFFF); //Complex Control
		dbkgain_cur = 0x8;
		cpxgain_cur = 0x5;

//		dbkcase = 5;
		return_value = 50;
	}
	else if (frm_blk > frm_Det*2/6 && frm_blk > 8)
	{
		//MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0x4000, 0xF000); //DBK Gain
		//MApi_XC_W2BYTEMSK(REG_SC_BK60_31_L, 0x4444, 0xFFFF); //Complex Control
		dbkgain_cur = 0x6;
		cpxgain_cur = 0x6;

		//dbkcase = 6;
		return_value = 35;
	}
	else if (frm_blk > frm_Det*1/6 && frm_blk > 8)
	{
		//MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0x3000, 0xF000); //DBK Gain
		//MApi_XC_W2BYTEMSK(REG_SC_BK60_31_L, 0x5555, 0xFFFF); //Complex Control
		dbkgain_cur = 0x4;
		cpxgain_cur = 0x8;

		//dbkcase = 7;
		return_value = 20;
	}
	else
	{
		//MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, 0x3000, 0xF000); //DBK Gain
		//MApi_XC_W2BYTEMSK(REG_SC_BK60_31_L, 0x6666, 0xFFFF); //Complex Control
		dbkgain_cur = 0x2;
		cpxgain_cur = 0x8;

		//dbkcase = 8;
		return_value = 0;

	}
	dbkgain = ( dbkgain * 6 + dbkgain_cur * 2 ) >> 3;
	cpxgain = ( cpxgain * 6 + cpxgain_cur * 2 ) >> 3;

	MS_U16 DBKUserLevel;

	DBKUserLevel = MApi_XC_R2BYTEMSK(REG_SC_BK30_0C_L,0x0300)>>8;

	if (DBKUserLevel==1)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, dbkgain<<12, 0xF000); //DBK Gain//Low
	}
	else if (DBKUserLevel==2)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, dbkgain<<12, 0xF000); //DBK Gain//Mid
	}
	else if (DBKUserLevel==3)
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, dbkgain<<12, 0xF000); //DBK Gain//High
	}
	else
	{
	MApi_XC_W2BYTEMSK(REG_SC_BK0C_42_L, dbkgain<<12, 0xF000); //DBK Gain
	}
	MApi_XC_W2BYTEMSK(REG_SC_BK60_31_L, (cpxgain<<12)+(cpxgain<<8)+(cpxgain<<4)+cpxgain, 0xFFFF); //Complex Control

//	PRT_TRP("dbkcase = %d, frm_blk = %d, frm_Det = %d \n", dbkcase, frm_blk, frm_Det);

	return	return_value;
}

void NoiseMaskingControl(int DBKStr,MS_BOOL isSceneChange)
{
	int NM_Gain = __minmax( DBKStr/10-7, 1, 3);
	//int NM_Gain = __minmax( DBKStr/10-4, 2, 6); decrease NM_gain by eason.lin 20140206

	MApi_XC_W2BYTEMSK(REG_SC_BK0C_52_L, NM_Gain, 0x003F);//Y noise masking gain

	if ( (DBKStr <= 40) || isSceneChange )
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_50_L, 0x00, 0x0008); //Y noise masking off
	else
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_50_L, 0x08, 0x0008); //Y noise masking on

}
void BlkStrIIRPart(MS_BOOL MarkSuccess, int* BlockinessCheck,MS_U8* BLKN_mark, MS_U8 markDefValue, int partIndex, int formatMode) //BLK Strength (mark) IIR
{
	int bin, blk_LPF, fac;
	static MS_U16 BlockinessChecktmp[MaxWidth];
	int PreH, NxtH;

	if (!MarkSuccess) //mark failed, use mark default value
	{

		for( bin = 0; bin < frameWidth; bin++ )
		{
			BlockinessCheckPre[bin] = BlockinessCheckIIR[bin];
			BlockinessCheck[bin] = (markDefValue*1+BlockinessCheckIIR[bin]*(IIRStrength-1) )/IIRStrength; //test for IIR
			BlockinessCheckIIR[bin] = BlockinessCheck[bin];
		}
	}
	else //mark success, ready to output BlockinessCheck
	{
		if (partIndex == 0)
		{
			//Extend marked pixels
			for( bin = 0; bin < frameWidth-1; bin++ )
			{
				//setting different software extend bits according to format
				if (formatMode == 0)  				//pure SD
				{
					BlockinessChecktmp[bin] = BLKN_mark[bin];
				}
				else if (formatMode == 1) //pure HD
				{
					PreH = (bin-1>=0)?bin-1:bin;
					NxtH = (bin+1<frameWidth)?bin+1:bin;

					BlockinessChecktmp[bin] = __max(BLKN_mark[PreH],BLKN_mark[bin]);
				}
				else if (formatMode == 2) //SD to HD
				{
					PreH = (bin-1>=0)?bin-1:bin;
					NxtH = (bin+1<frameWidth)?bin+1:bin;
					BlockinessChecktmp[bin] = __max(BLKN_mark[PreH],BLKN_mark[bin]);
				}
				else if (formatMode == 3) //scaled HD
				{
					PreH = (bin-1>=0)?bin-1:bin;
					NxtH = (bin+1<frameWidth)?bin+1:bin;
					BlockinessChecktmp[bin] = __max(BLKN_mark[PreH],BLKN_mark[bin]);
				}
				else //formatMode == 4
				{
					PreH = (bin-1>=0)?bin-1:bin;
					NxtH = (bin+1<frameWidth)?bin+1:bin;
					BlockinessChecktmp[bin] = __max(BLKN_mark[PreH],BLKN_mark[bin]);
				}

			}

		}
		else if (partIndex == 1)
		{
			for( bin = 0; bin < frameWidth; bin++ )
			{
				PreH  = __max(bin-1,0);
				NxtH  = __min(bin+1,frameWidth-1);

				blk_LPF = (BlockinessChecktmp[PreH]+2*BlockinessChecktmp[bin]+BlockinessChecktmp[NxtH])>>2;

				fac = abs(blk_LPF-BlockinessCheckPre[bin]);
				//fac = 0;
				BlockinessCheck[bin] = (blk_LPF * (16-fac) + BlockinessCheckIIR[bin] * fac + 8)>>4;
				BlockinessCheckIIR[bin] = BlockinessCheck[bin];
				BlockinessCheckPre[bin] = blk_LPF;
			}
		}
		else
			return;
	}
	/*    else //mark success, ready to output BlockinessCheck
	{
	if (partIndex == 0)
	{
	//Extend marked pixels
	for( bin = 0; bin < frameWidth/2; bin++ )
	{

	Pre3H = __max(bin-3,0);
	Pre2H = __max(bin-2,0);
	PreH  = __max(bin-1,0);
	NxtH  = __min(bin+1,frameWidth-1);
	Nxt2H = __min(bin+2,frameWidth-1);
	Nxt3H = __min(bin+3,frameWidth-1);

	if (MApi_XC_R2BYTEMSK(REG_SC_BK30_0F_L, 0x0400) > 0 && formatMode==1 )
	blk_LPF = (BLKN_mark[PreH]+2*BLKN_mark[bin]+BLKN_mark[NxtH])>>2;
	else
	{
	blk_LPF = (BLKN_mark[Pre3H]+BLKN_mark[Pre2H]+2*BLKN_mark[PreH]+8*BLKN_mark[bin]+2*BLKN_mark[NxtH]+BLKN_mark[Nxt2H]+BLKN_mark[Nxt3H])>>4;
	}

	fac = abs(blk_LPF-BlockinessCheckPre[bin]);
	//fac = 0;
	BlockinessCheck[bin] = (blk_LPF * (16-fac) + BlockinessCheckIIR[bin] * fac + 8)>>4;
	BlockinessCheckIIR[bin] = BlockinessCheck[bin];
	BlockinessCheckPre[bin] = blk_LPF;


	}

	}
	else if (partIndex == 1)
	{
	//for( bin = frameWidth/2; bin < frameWidth; bin++ )
	for( bin = 0; bin < frameWidth; bin++ )
	{
	Pre3H = __max(bin-3,0);
	Pre2H = __max(bin-2,0);
	PreH  = __max(bin-1,0);
	NxtH  = __min(bin+1,frameWidth-1);
	Nxt2H = __min(bin+2,frameWidth-1);
	Nxt3H = __min(bin+3,frameWidth-1);

	if (MApi_XC_R2BYTEMSK(REG_SC_BK30_0F_L, 0x0400) > 0 && formatMode==1 )
	blk_LPF = (BLKN_mark[PreH]+2*BLKN_mark[bin]+BLKN_mark[NxtH])>>2;
	else
	{
	blk_LPF = (BLKN_mark[Pre3H]+BLKN_mark[Pre2H]+2*BLKN_mark[PreH]+8*BLKN_mark[bin]+2*BLKN_mark[NxtH]+BLKN_mark[Nxt2H]+BLKN_mark[Nxt3H])>>4;
	}


	fac = abs(blk_LPF-BlockinessCheckPre[bin]);
	//fac = 0;
	BlockinessCheck[bin] = (blk_LPF * (16-fac) + BlockinessCheckIIR[bin] * fac + 8)>>4;
	BlockinessCheckIIR[bin] = BlockinessCheck[bin];
	BlockinessCheckPre[bin] = blk_LPF;
	}
	}
	else
	return;
	}
	*/
}

MS_U8 BWPatchDBK(void *pInstance, const XC_LumaHistogramStates lumaHistogram, MS_U32 colorsum, MS_U32 uComplex, MS_BOOL me1Still, MS_U16 mvNotFound, /*MS_U16 nonMv0Comfirm,*/ MS_U16 mv0Comfirm) //cplx gain
{

	int i, cpxGain;

	MS_U16 sum   = 0;
	MS_U16 BWSum = 0;
	for(i=0;i<32;i++)
	{
		sum += (lumaHistogram.lumaBin[i]);
		if( (i<=5) || (i>=26) )
			BWSum += lumaHistogram.lumaBin[i] ;

	}
	//PRT_TRP("BWSum= %d;sum = %d;me1Still =%d; colorsum=%d; uComplex=%d; mvNotFound =%d; nonMv0Comfirm=%d; mv0Comfirm=%d; \n",
	//BWSum, sum, me1Still, colorsum, uComplex, mvNotFound, nonMv0Comfirm, mv0Comfirm);


	if( (BWSum > sum * 3/8) && (BWSum < sum * 5/8)&& (colorsum < 0x18) && (!me1Still) && (uComplex > 0x2700)
		&& (mvNotFound > 0x200 ) /*&& (mvNotFound > mv0Comfirm*8)*/ && (mv0Comfirm < 0x10) )
	{
		//DEBUG_LC_5D(pInstance,1);
		cpxGain = 0x8;

	}
	else if (BWSum < sum * 1/8)
	{
		//DEBUG_LC_5D(pInstance,2);
		cpxGain = 0x0;

	}
	else
	{
		//		if (sum>0)
		//			cpxGain = (BWSum*8-sum)*8/sum;
		//		else
		cpxGain = 0;
		//DEBUG_LC_5D(pInstance,0);
	}
	MApi_XC_W2BYTEMSK(REG_SC_BK0C_47_L, 			0, 0x0F00); //Cplx gain for black white water flow
	MApi_XC_W2BYTEMSK(REG_SC_BK0C_47_L, cpxGain, 0x000F); //Cplx gain for black white water flow

	return cpxGain;
}
void setHDDefaultParam(void *pInstance)
{
	if (0) //moved to register map
	{
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_38_L, 0x4, 0x0004);
		MApi_XC_W2BYTEMSK(REG_SC_BK0C_45_L, 0x00C0, 0x00C0);

		MApi_XC_W2BYTEMSK(REG_SC_BK60_30_L, 0x0100, 0x0100);
		MApi_XC_W2BYTEMSK(REG_SC_BK60_30_L, 0x0080, 0x0080);

	}
	return;
}

int DBK_Detect_Partition(void *pInstance, int* BlockinessCheck, long* histogram, int* partiCounter, const MS_U8 _hdsddFilterBaseReport, MS_BOOL isSceneChangeMc, long FrameBLKStrTmp, int MARKTIMESLOT, int ADJTIMESLOT) // deblocking detect new test algorithm
{
	//Declaration
	static MS_U8 markDefValue, markValue;
	static int minIndex = -1;     //minIndex+width_offset = candidate width
	static MS_U8 BLKN_adj[MaxWidth] = {0};   //Normalized histogram
	static MS_U8 BLKN_mark[MaxWidth] = {0}; //blockness marking array
	//static MS_U8 BlockinessCheckIIR[MaxWidth] = { 0 };
	static int iniIndex = 0; 		//initial point for marking
	//For width calculation
	static int WidthCandidate[paraSize];
	static int WidthDiffHW[paraSize];
	static int WidthCandIdx[paraSize];
	static int StartPoint[paraSize];
	static int MaxMinDiff = 0;
	static int WidthAvg = 0;

	static long FrameBLKStrIIR = 0;
	static int maxHitRate = 0;
	static int cnt = 0;
	static int width, validCount, NumW;
	static MS_BOOL MarkSuccess = 0;
	static MS_U8 u8SD2HDValue;
	static int MarkConfidence = 0;
	int bin;
	static int zeroCnt;
	//static MS_BOOL artificialPattern = 0;
	const int partIni = 0;
	//Partition Parameters
	int BLKADJ_S = partIni+1; // 1
	int BLKADJ_E = BLKADJ_S+ADJTIMESLOT-1; //1
	int BLKNINTW = BLKADJ_E+1;//2
	int BLKCNTNUMW = BLKNINTW+1;//3
	int BLKVRFNINTW_S = BLKCNTNUMW+1;//4
	int BLKVRFNINTW_E = BLKVRFNINTW_S+2;//6
	int BLKOUTWID_S = BLKVRFNINTW_E+1;//7
	int BLKOUTWID_E = BLKOUTWID_S+MARKTIMESLOT-1;//7
	int BLK_FRMSTR = BLKOUTWID_E+1;//8
	int BLKIIR_S = BLK_FRMSTR+1;//9
	int BLKIIR_E = BLKIIR_S+1;//10
	//Partition Parameters End

	if ( (*partiCounter) == partIni)
	{
		setHDDefaultParam(pInstance);
		u8SD2HDValue = _hdsddFilterBaseReport;

		if (frameWidth <=800) //SD
		{
			markDefValue = 0x6;
			markValue	   = 0xC;
		}
		else if (frameWidth <=1980) //HD
		{
			markDefValue = 0x4;
			markValue	   = 0x8;
		}
		else if (frameWidth >=3840) //UHD
		{
			markDefValue = 0x4;
			markValue	   = 0x8;
		}
		else
		{
			markDefValue = 0x4;
			markValue	   = 0x8;
		}
		WidthAvg = 0;
		MaxMinDiff = 0;

		iniIndex = 0;
		minIndex = -1;
		MarkSuccess = 0;
		MarkConfidence = 0;
		zeroCnt = 0;
		validCount = 0;
		//artificialPattern = false;
		for (bin=0; bin < frameWidth; bin++)
		{
			BLKN_mark[bin] = markDefValue;
			BlockinessCheck[bin] = markDefValue;
		}
	}
	else if ( (*partiCounter >= BLKADJ_S) && (*partiCounter <= BLKADJ_E) )
	{
		//--------- Function Process start ---------//
		//Histogram normalization
		validCount += DBK_BlocknessAdjPart(histogram, BLKN_adj, &zeroCnt, (*partiCounter) - BLKADJ_S , markValue, markDefValue, ADJTIMESLOT );	//valid ratio too low -> weak blockness frame
		if (zeroCnt > frameWidth/2 ) //early terminate
		{
			//artificialPattern = true;
			(*partiCounter) = BLK_FRMSTR;
		}
	}
	else if ( ((*partiCounter) == BLKNINTW) )
	{
		MaxMinDiff = 0;
		WidthAvg = 0;
		minIndex = DBK_WidthPartTop(pInstance, WidthDiffHW, &MaxMinDiff, &WidthAvg, u8SD2HDValue);
		if (minIndex == -1)
			minIndex = DBK_ComputeWidthPart( WidthDiffHW, WidthCandidate, WidthAvg);
		else
		{
			for (bin=0;bin<paraSize;bin++)
			{
				if (minIndex==bin)
					WidthCandidate[bin] = 1;
				else
					WidthCandidate[bin] = 0;
			}
		}
	}
	else if ( ((*partiCounter) == BLKCNTNUMW) )
	{
		maxHitRate = 0;
		cnt = 0;
		NumW = 0;
		for (bin=0;bin<paraSize;bin++) // count number of frequencies
		{
			if (WidthCandidate[bin] == 1)
			{
				WidthCandIdx[NumW] = bin;
				NumW++;
				StartPoint[bin] = DBK_StPtLeft(paraWidth[bin], BLKN_adj, frameWidth*1/4, frameWidth*3/4);
				//StartPoint[bin] = DBK_StPtLeft(paraWidth[bin], BLKN_adj, 100, 100 + frameWidth/8 );
			}
		}
	}
	else if ( ((*partiCounter) >= BLKVRFNINTW_S) && ((*partiCounter) <= BLKVRFNINTW_E) )
	{
		if ((*partiCounter)-(BLKVRFNINTW_S) >= NumW-1 )
			(*partiCounter) = BLKVRFNINTW_E;
		if (NumW-((*partiCounter)-BLKVRFNINTW_S)-1 >= 0)
		{
			DBK_VerifyWidthPart( WidthCandidate, BLKN_adj, WidthCandIdx[NumW-((*partiCounter)-BLKVRFNINTW_S)-1], &maxHitRate, &minIndex, &cnt, StartPoint[WidthCandIdx[NumW-((*partiCounter)-BLKVRFNINTW_S)-1]], u8SD2HDValue, markDefValue);
    }
	}
	else if ( ((*partiCounter) >= BLKOUTWID_S) && ((*partiCounter) <= BLKOUTWID_E) )
	{
		if (minIndex >= 0 )
		{
			width = paraWidth[minIndex];
			iniIndex = StartPoint[minIndex];
			if (width == 32) //no such pattern yet, only artificials
			{
				minIndex = -1;
				width = 5;
			}
		}
		else
			width = 5;
		//--------- Mark Position and verification start ---------//
		//Check frame blockness strength and mark success
		if( (minIndex == -1) || (validCount<0x10))//(validRatio*width < 256))
			width = WidthIIR;
		else
		{
                        if(frameWidth <= 800)
                        {
			    DBK_bypassmark(BLKN_mark, BLKN_adj);
                            MarkConfidence = 255;
                        }
                        else
                        {
        		    if ( WidthIIR==8 && width==16 )
			        MarkConfidence = DBK_Mark(BLKN_mark, BLKN_adj, WidthIIR, iniIndex,  markDefValue)+32;
			    else
				MarkConfidence = DBK_Mark(BLKN_mark, BLKN_adj, width, iniIndex,  markDefValue);
                        }
			if ( (*partiCounter) == BLKOUTWID_E )
			{
				MarkSuccess = (MarkConfidence >= hitrate_threshold*1 ) ;
				WidthIIR_H(width);

				WidthToRegister(pInstance,WidthIIR);
				FeedBackSD2HD(pInstance,WidthIIR, 0);
			}
		}
	}
	else if (((*partiCounter) == BLK_FRMSTR) )
	{
		if ( !MarkSuccess )
			FrameBLKStrTmp = 0;
		FrameBLKStrIIR = (FrameBLKStrTmp + FrameBLKStrIIR*3 )>>2;
		return FrameBLKStrIIR;
	}
	else if ( ((*partiCounter) >= BLKIIR_S) && ((*partiCounter) <= BLKIIR_E) )//mark blockness points	+ IIR
		//BlkStrIIRPart(1/*MarkSuccess*/, BlockinessCheck, BlockinessCheckIIR, BLKN_mark, markDefValue, (*partiCounter)-BLKIIR_S);
   		BlkStrIIRPart(MarkSuccess, BlockinessCheck, BLKN_mark, markDefValue, (*partiCounter)-BLKIIR_S, 0);
	else
	{
		//DRVSCA_DBG(PRT_TRP(" Invalid partition number %d\n",partiCounter));
	}
	return -1;
}
#if 0
int DBK_Detect_Partition( void *pInstance,int* BlockinessCheck, long* histogram, int* partiCounter, const MS_U8 _hdsddFilterBaseReport
			 , MS_BOOL isSceneChangeMc, long FrameBLKStrTmp) // deblocking detect new test algorithm
{
	//Declaration
	static MS_U8 markDefValue, markValue;
	static int minIndex = -1;     //minIndex+width_offset = candidate width
	static MS_U8 BLKN_adj[MaxWidth] = {0};   //Normalized histogram
	static MS_U8 BLKN_mark[MaxWidth]; //blockness marking array
	static int iniIndex = 0; 		//initial point for marking
	//For width calculation
	static int WidthCandidate[paraSize];
	static int WidthDiffHW[paraSize];
	static int WidthCandIdx[paraSize];
	static int StartPoint[paraSize];
	static int MaxMinDiff = 0;
	static int WidthAvg = 0;

//	static long FrameEdg = 0;
//	static long FrameBlk = 0;
//	static long FrameDet = 0;

	static long FrameBLKStrIIR = 0;

	static int maxHitRate = 0;
	static int cnt = 0;
	static int width, validCount, NumW;
	static MS_U32 SystemTime1=0;
	static MS_BOOL MarkSuccess = 0;
	//static int formatMode;
	static MS_U8 u8SD2HDValue;
	int MarkConfidence = 0;
	int bin;
//	static MS_U8 cpxGain = 0;
	static int zeroCnt;

	//Partition Parameters
	const int partIni = 3;
	const int BLKADJ_S = partIni+1; // 4
	const int BLKADJ_E = BLKADJ_S+2; //6
	const int BLKNINTW = BLKADJ_E+1;//7
	const int BLKCNTNUMW = BLKNINTW+1;//8
	const int BLKVRFNINTW_S = BLKCNTNUMW+1;//9
	const int BLKVRFNINTW_E = BLKVRFNINTW_S+2;//11
	const int BLKOUTWID = BLKVRFNINTW_E+1;//12
	const int BLK_FRMSTR = BLKOUTWID+1;//13
	const int BLKIIR_S = BLK_FRMSTR+1;//14
	const int BLKIIR_E = BLKIIR_S+1;//15
	if (isSceneChangeMc)
	{
		FrameBLKStrIIR = FrameBLKStrIIR;///2;
	}
	//Partition Parameters End


	if ( (*partiCounter) == partIni)
	{
		setHDDefaultParam();

		u8SD2HDValue = _hdsddFilterBaseReport;

		//complex gain control
		//cpxGain = BWPatchDBK(lumaHistogram, colorsum, uComplex, me1Still, mvNotFound, /*nonMv0Comfirm,*/ mv0Comfirm);


		//Get mark strength info
		//mark strength 1.3 bits: 8 = 1.0x, 15 = 1.9x (max), 0 = disable pixel based
		MS_U8 markValueIn = (MS_U8)MApi_XC_R2BYTEMSK(REG_SC_BK30_0F_L, 0x00FF);
		if (markValueIn > 0)
		{
			markDefValue = (markValueIn & 0x0F   );
			markValue	   = (markValueIn & 0xF0)>>4;
		}
		else
		{
			markDefValue = 0x4;
			markValue	   = 0x8;
		}

		WidthAvg = 0;
		MaxMinDiff = 0;

		iniIndex = 0;
		minIndex = -1;
		MarkSuccess = 0;
		MarkConfidence = 0;
		zeroCnt = 0;
		validCount = 0;

		SystemTime1=MsOS_GetSystemTime();
		for (bin=0; bin < frameWidth; bin++)
		{
			BLKN_mark[bin] = markDefValue;
			BlockinessCheck[bin] = markDefValue;
		}
		SystemTime1=MsOS_GetSystemTime()-SystemTime1;
	}
	else if ( (*partiCounter >= BLKADJ_S) && (*partiCounter <= BLKADJ_E) )
	{
		//--------- Function Process start ---------//
		//Histogram normalization
		validCount += DBK_BlocknessAdjPart(histogram, BLKN_adj, &zeroCnt, (*partiCounter) - BLKADJ_S , markValue, markDefValue);	//valid ratio too low -> weak blockness frame
		if (zeroCnt > frameWidth/2 ) //early terminate
		{
			(*partiCounter) = BLK_FRMSTR;
		}

//		validRatio = validCount*256/frameWidth;
	}
	else if ( ((*partiCounter) == BLKNINTW) )
	{
		MaxMinDiff = 0;
		WidthAvg = 0;
		/*        if (u8SD2HDValue >= 60)
		DEBUG_LC_5E(1);
		else if (u8SD2HDValue == 0)
		DEBUG_LC_5E(2);
		else
		DEBUG_LC_5E(0);	*/
		minIndex = DBK_WidthPartTop( WidthDiffHW, &MaxMinDiff, &WidthAvg, u8SD2HDValue);
		if (minIndex == -1)
			minIndex = DBK_ComputeWidthPart( WidthDiffHW, WidthCandidate, WidthAvg);
		else
		{
			for (bin=0;bin<paraSize;bin++)
			{
				if (minIndex==bin)
					WidthCandidate[bin] = 1;
				else
					WidthCandidate[bin] = 0;
			}
		}
	}
	else if ( ((*partiCounter) == BLKCNTNUMW) )
	{
		maxHitRate = 0;
		cnt = 0;
		NumW = 0;
		for (bin=0;bin<paraSize;bin++) // count number of frequencies
		{
			if (WidthCandidate[bin] == 1)
			{
				WidthCandIdx[NumW] = bin;
				NumW++;
				StartPoint[bin] = DBK_StPtLeft(paraWidth[bin], BLKN_adj, frameWidth*1/4, frameWidth*3/4);
			}
		}
	}
	else if ( ((*partiCounter) >= BLKVRFNINTW_S) && ((*partiCounter) <= BLKVRFNINTW_E) )
	{
		if ((*partiCounter)-(BLKVRFNINTW_S) >= NumW-1 )
		{
			(*partiCounter) = BLKVRFNINTW_E;
		}
		if (NumW-((*partiCounter)-BLKVRFNINTW_S)-1 >= 0)
		DBK_VerifyWidthPart( WidthCandidate, BLKN_adj, WidthCandIdx[NumW-((*partiCounter)-BLKVRFNINTW_S)-1], &maxHitRate, &minIndex, &cnt, StartPoint[WidthCandIdx[NumW-((*partiCounter)-BLKVRFNINTW_S)-1]], u8SD2HDValue, markDefValue);
	}
	else if ( (*partiCounter) == BLKOUTWID)
	{
		if (minIndex >= 0 )
		{
			width = paraWidth[minIndex];
			iniIndex = StartPoint[minIndex];
			if (width == 32) //no such pattern yet, only artificials
			{
				minIndex = -1;
				width = 5;
			}
		}
		else
			width = 5;
		//--------- Compute Width and initial points end ---------//

		//--------- Mark Position and verification start ---------//
		//Check frame blockness strength and mark success
		//DEBUG_LC_Full6bit2(validCount);
		if( (minIndex == -1) || (validCount<0x10))//(validRatio*width < 256))
		{
			width = WidthIIR;

		}
		else
		{

			//MarkConfidence = 255;

                        if(frameWidth <= 800)
                        {
			    DBK_bypassmark(BLKN_mark, BLKN_adj);
                            MarkConfidence = 255;
                        }
                        else
                        {
			if ( WidthIIR==8 && width==16 )
				MarkConfidence = DBK_Mark(BLKN_mark, BLKN_adj, WidthIIR, iniIndex,  markDefValue)+32;
			else
				MarkConfidence = DBK_Mark(BLKN_mark, BLKN_adj, width, iniIndex,  markDefValue);
                        }
			//DEBUG_LC_Full6bit(MarkConfidence/4);

			MarkSuccess = (MarkConfidence >= hitrate_threshold ) ;
			WidthIIR_H(width);

			//DEBUG_LC_DBKWidth(WidthIIR);
			//write width to register for backward compatibility
			WidthToRegister(WidthIIR);
			//FeedBackSD2HD(WidthIIR, u8SD2HDValue);
			FeedBackSD2HD(WidthIIR, 0);
		}
	}
	else if (((*partiCounter) == BLK_FRMSTR) )
	{
		if ( !MarkSuccess )
			FrameBLKStrTmp = 0;//FrameEdg;

		FrameBLKStrIIR = (FrameBLKStrTmp + FrameBLKStrIIR*3 )>>2;

		return FrameBLKStrIIR;
	}
	else if ( ((*partiCounter) == BLKIIR_S)  || ((*partiCounter) == BLKIIR_E) )
	{
		//mark blockness points + IIR
		//BlkStrIIR(MarkSuccess, BlockinessCheck, BLKN_mark, markDefValue, frameWidth);
		BlkStrIIRPart(MarkSuccess, BlockinessCheck, BLKN_mark, markDefValue, (*partiCounter)-BLKIIR_S, 1);
		//BlkStrIIRPart(MarkSuccess, BlockinessCheck, BLKN_mark, markDefValue, (*partiCounter)-BLKIIR_S, formatMode);
		//--------- Mark Position and verification end ---------//
	}

	//--------- Mark Position and verification end ---------//
	else
	{
		//DRVSCA_DBG(PRT_TRP(" Invalid partition number %d\n",partiCounter));
	}
	return -1;
}
#endif
int readVHistogramPart(void *pInstance,long* histogram, int partIndex, int VSRAMTIMESLOT)
{
	int data_ack_en;
	int bin;
	int IniPos, EndPos;
	if( (partIndex < VSRAMTIMESLOT) && (VSRAMTIMESLOT!=0) )
	{
		IniPos = frameHeight*partIndex/VSRAMTIMESLOT;
		EndPos = frameHeight*(partIndex+1)/VSRAMTIMESLOT;
	}
	else
	{
		IniPos = 0;
		EndPos = 0;
		return 0;
	}
#if 0
	if (partIndex == 0)
	{
		IniPos = 0;
		EndPos = frameHeight/2;
	}
	else if (partIndex == 1)
	{
		IniPos = frameHeight/2;
		EndPos = frameHeight;
	}
	else
	{
		IniPos = 0;
		EndPos = 0;
		return 0;
	}
#endif
	//--------- Blockiness Histogram Request start ---------//
	if (partIndex == 0)
	{
		// write 1 to request register
		MApi_XC_W2BYTEMSK(REG_SC_BK60_7D_L, 1, 0x0001); //reg_bkn_statistic_ack
	}
	if (1)
	{
		//--------- Blockiness Histogram Acknowledge start ---------//
		data_ack_en = MApi_XC_R2BYTEMSK(REG_SC_BK60_7D_L, 0x0002); //reg_bkn_width_statistic_ack
		//--------- Function Process start ---------//
		if(data_ack_en)
		{
			// write 1 to sram_io_en register
			MApi_XC_W2BYTEMSK(REG_SC_BK60_7D_L, 0x0100, 0x0100); //reg_bkn_sram_io_en
			for(bin = IniPos; bin < EndPos; bin++)
			{
				MApi_XC_W2BYTEMSK(REG_SC_BK60_7E_L, (MS_U16)(0x8000 + bin), 0xFFFF); //reg_bkn_ioaddr
				// read data from selected address
				histogram[bin] = MApi_XC_R2BYTEMSK(REG_SC_BK60_7F_L, 0xFFFF) ; //reg_bkn_iordata

			}
			if (partIndex == VSRAMTIMESLOT-1)
				MApi_XC_W2BYTEMSK(REG_SC_BK60_7D_L, 0, 0x0001);//reg_bkn_statistic_ack
			return 1;
		}
		else//(!data_ack_en)
		{
			return 0;
		}
	}
}

int DBK_Mark_Ver(void *pInstance,MS_U8* BLKN_mark, MS_U8 *BLKN_adj, int width, int iniIndex, MS_U8 markDefValue)
{
	int restart=0;
	int misscnt = 0;
	int ThValue = 2;
	int hitrate = 0;
	int bin, CurV, idx, idx1;
	int candiSP, sp;
	MS_BOOL hit;
	int offset;
	int n8[50];
	int maxOri, maxOriInd, pos_fac, range, dummy, r;
	int maxLocal = 0;
	int sndLocal = 0;
	int maxIndex = 0;
	int sndIndex = 0;
	int BLKN_adj_comb;

	misscnt = 0;
	//initialize start points
	sp = iniIndex;
	offset = 4;//8;
	if (width >= 16)
		offset = 8;//16;
	range = (width*2+4)>>3;
	r = (width+4)/8;
	dummy = ThValue *(width + offset)/offset;

	//sp
	BLKN_mark[sp] = BLKN_adj[sp];
	BLKN_mark[sp-1] = BLKN_adj[sp-1];
	BLKN_mark[sp+1] = BLKN_adj[sp+1];
	hitrate++;
	//right hand side
	for ( CurV=sp+width; CurV<frameHeight; )
	{
		maxOri = 0;
		maxOriInd = -1;
		maxLocal = 0;
		sndLocal = 0;
		maxIndex = 0;
		sndIndex = 0;
		for (bin=0; bin<width;bin++)
		{
			idx = __minmax(CurV+(bin-(width-1)/2), 0, frameHeight-1);
			idx1= __minmax(idx+1, 0, frameHeight-1);
			BLKN_adj_comb = __min(BLKN_adj[idx], BLKN_adj[idx1]);
			if ( BLKN_adj_comb > maxOri)
			{
				maxOri = BLKN_adj_comb;
				maxOriInd = bin;
			}
			if ( (bin<=width/4) || (bin>=width*3/4) )
				n8[bin] =0;
			else
			{
				pos_fac = (width-1)/2 - abs(bin-(width-1)/2) + offset;
				n8[bin]= (BLKN_adj_comb * pos_fac + (offset/2) )/offset;
				if ( n8[bin] > maxLocal)
				{
					maxLocal = n8[bin];
					maxIndex = bin;
				}
			}
		}

		for (bin=(0-range); bin<=range; bin++)
		{
			if( (bin!=0 ) && ((maxIndex+bin) >=0) && ((maxIndex+bin) < width) )
			{
				if (n8[maxIndex+bin] > sndLocal)
				{
					sndLocal = n8[maxIndex+bin];
					sndIndex = bin; //related to maxIndex
				}
			}
		}
		idx = __minmax(CurV+(maxIndex-(width-1)/2), 0, frameHeight-1);
		idx1= __minmax(idx+1, 0, frameHeight-1);
		BLKN_adj_comb = __min(BLKN_adj[idx], BLKN_adj[idx1]);
		hit = ( maxOri <= BLKN_adj_comb )|| ( (maxOriInd>=__min(maxIndex, maxIndex+sndIndex) )&& (maxOriInd <= __max(maxIndex, maxIndex+sndIndex) ) );

		if ( (!hit) || (maxLocal <= dummy) )
		{
			misscnt++;
		}
		if ((maxLocal > dummy) && (misscnt <= 2))
		{
			CurV = idx; //adjust to max position

			if (hit)
			{
				misscnt=0;
				BLKN_mark[CurV] = BLKN_adj[CurV];
				if (CurV+1 < frameHeight)
					BLKN_mark[CurV+1] = BLKN_adj[CurV+1];
				if (CurV-1 >= 0)
					BLKN_mark[CurV-1] = BLKN_adj[CurV-1];
			}
			else
			{
				BLKN_mark[CurV] = BLKN_adj[CurV]/2;
				if (CurV+1 < frameHeight)
					BLKN_mark[CurV+1] = BLKN_adj[CurV+1]/2;
				if (CurV-1 >= 0)
					BLKN_mark[CurV-1] = BLKN_adj[CurV-1]/2;
			}
			/* // fill the gap between top 2 strength within range 4, pass in VDBK
			if (BLKN_adj[CurV+sndIndex] > ThValue )
			{
			if ( (sndIndex <= range-2 ) && (sndIndex > 0) )
			{
			if ( (CurV-1) >=0 )
			BLKN_mark[CurV-1] = BLKN_adj[CurV-1];
			if ( (CurV+sndIndex+1) < frameHeight)
			BLKN_mark[CurV+sndIndex+1] = BLKN_adj[CurV+sndIndex+1];
			}
			else if ( (sndIndex <0) && (sndIndex >= ((-1)*(range-2)) ) )
			{
			if ( (CurV+1) < frameHeight)
			BLKN_mark[CurV+1] = BLKN_adj[CurV+1];
			if ( (CurV+sndIndex-1) >= 0 )
			BLKN_mark[CurV+sndIndex-1] = BLKN_adj[CurV+sndIndex-1];
			}
			for (bin=__min(0, sndIndex); bin<=__max(0, sndIndex);bin++)
			{
			if ((CurV+bin >= 0) && (CurV+bin <frameHeight))
			BLKN_mark[CurV+bin] = BLKN_adj[CurV+bin];
			}
			CurV = CurV + ((sndIndex)/2); // move to center
			}*/
			CurV += width;
			hitrate++;
		}
		else if (misscnt > 2)
		{
			candiSP = DBK_StPtLeft( width, (MS_U8*)BLKN_adj, CurV, __min(CurV+40, frameHeight));
			if (candiSP != 9999)
			{
				restart++;
				if (restart > 5)
					return 0;
				if ( (abs(candiSP - CurV)%width) != 0 )
				{
					for (bin=-4;bin<=4;bin++)
					{
						if ( ((CurV+bin) <frameHeight) && (CurV+bin >=0) && (BLKN_mark[CurV+bin]>0) )
							BLKN_mark[CurV+bin] = markDefValue;
						if ( ((CurV-width+bin) <frameHeight) && ((CurV-width+bin) >=0) && (BLKN_mark[CurV-width+bin]>0) )
							BLKN_mark[CurV-width+bin] = markDefValue;
						if ( ((CurV-width*2+bin) <frameHeight) && ((CurV-width*2+bin) >=0) && (BLKN_mark[CurV-width*2+bin]>0) )
							BLKN_mark[CurV-width*2+bin] = markDefValue;
					}
				}
				misscnt = 0;
				CurV = candiSP;

				for (bin=-r;bin<=r;bin++)
				{
					if (((CurV+bin)>=0) && ((CurV+bin) <frameHeight) )
						BLKN_mark[CurV+bin] = BLKN_adj[CurV+bin];
					if (((CurV-width+bin)>=0) && ((CurV-width+bin) <frameHeight) )
						BLKN_mark[CurV-width+bin] = BLKN_adj[CurV-width+bin];
					if (((CurV-width*2+bin)>=0) && ((CurV-width*2+bin) <frameHeight) )
						BLKN_mark[CurV-width*2+bin] = BLKN_adj[CurV-width*2+bin];
				}

				CurV += width;
			}
			else
			{
				for (bin = CurV; bin< __min((CurV+40), frameHeight); bin= bin+width )
				{
					if (bin-width >= 0)
					BLKN_mark[bin] = (BLKN_adj[bin]+BLKN_adj[bin-width])/2;
				}
				CurV += 40;
			}
		}
		else
		{
			BLKN_mark[CurV] = BLKN_adj[CurV]/2;
			if (CurV+1 < frameHeight)
				BLKN_mark[CurV+1] = BLKN_adj[CurV+1]/2;
			if (CurV-1 >= 0)
				BLKN_mark[CurV-1] = BLKN_adj[CurV-1]/2;
			CurV += width;
		}
	}
	misscnt=0;
	//left hand side
	for (CurV=sp-width; CurV >= (width)/2;)
	{
		maxOri = 0;
		maxOriInd = -1;
		maxLocal = 0;
		sndLocal = 0;
		maxIndex = 0;
		sndIndex = 0;
		for (bin=0; bin<width;bin++)
		{
			idx = __minmax(CurV+(bin-(width)/2), 0, frameHeight);
			BLKN_adj_comb = __min(BLKN_adj[idx], BLKN_adj[idx+1]);
			if (BLKN_adj_comb > maxOri)
			{
				maxOri = BLKN_adj_comb;
				maxOriInd = bin;
			}
			if ( (bin<=(width/4)) || (bin>=(width*3/4)) )
				n8[bin] =0;
			else
			{
				pos_fac = (width)/2 - abs(bin-(width)/2) + offset;
				n8[bin]= (BLKN_adj_comb * pos_fac + (offset/2) )/offset;
				if ( n8[bin] > maxLocal)
				{
					maxLocal = n8[bin];
					maxIndex = bin;
				}
			}
		}

		for (bin=(0-range); bin<=range; bin++)
		{
			if( (bin!=0) &&  ((maxIndex+bin) >=0) && ((maxIndex+bin) < width) )
			{
				if (n8[maxIndex+bin] > sndLocal)
				{
					sndLocal = n8[maxIndex+bin];
					sndIndex = bin; //related to maxIndex
				}
			}
		}
		idx = __minmax(CurV+(maxIndex-(width+1)/2), 0, frameHeight);
		idx1= __minmax(idx+1, 0, frameHeight-1);
		BLKN_adj_comb = __min(BLKN_adj[idx], BLKN_adj[idx1]);
		hit = ( maxOri <= BLKN_adj_comb ) || ( (maxOriInd>=__min(maxIndex,maxIndex+sndIndex) )&& (maxOriInd <= __max(maxIndex,maxIndex+sndIndex) ) );

		if ( (!hit) || (maxLocal <= dummy) )
		{
			misscnt++;
		}
		if ((maxLocal > dummy) && (misscnt <= 2) )
		{
			CurV = idx; //adjust to max position
			if (hit)
			{

				misscnt=0;
				BLKN_mark[CurV] = BLKN_adj[CurV];
				if (CurV+1 < frameHeight)
					BLKN_mark[CurV+1] = BLKN_adj[CurV+1];
				if (CurV-1 >= 0)
					BLKN_mark[CurV-1] = BLKN_adj[CurV-1];
			}
			else
			{
				BLKN_mark[CurV] = BLKN_adj[CurV]/2;
				if (CurV+1 < frameHeight)
					BLKN_mark[CurV+1] = BLKN_adj[CurV+1]/2;
				if (CurV-1 >= 0)
					BLKN_mark[CurV-1] = BLKN_adj[CurV-1]/2;
			}

			/*// fill the gap between top 2 strength within range 4
			if (BLKN_adj[CurV+sndIndex] > ThValue )
			{
			if ( (sndIndex <=range-2) && (sndIndex > 0) )
			{
			if (CurV-1 >= 0)
			BLKN_mark[CurV-1] = BLKN_adj[CurV-1];
			if (CurV+sndIndex+1 < frameHeight)
			BLKN_mark[CurV+sndIndex+1] = BLKN_adj[CurV+sndIndex+1];
			}
			else if ( (sndIndex <0) && (sndIndex >= (-1)*(range-2)) )
			{
			if (CurV+1 < frameHeight)
			BLKN_mark[CurV+1] = BLKN_adj[CurV+1];
			if (CurV+sndIndex-1 >= 0)
			BLKN_mark[CurV+sndIndex-1] = BLKN_adj[CurV+sndIndex-1];
			}
			for (bin=__min(0, sndIndex); bin<=__max(0, sndIndex);bin++)
			{
			if ( ((CurV+bin) >= 0) && ((CurV+bin)<frameHeight) )
			BLKN_mark[CurV+bin] = BLKN_adj[CurV+bin];
			}
			CurV = CurV + ((sndIndex)/2); // move to center
			}
			*/
			CurV -= width;
			hitrate++;
			//cnt++;
		}
		else if (misscnt > 2)
		{
			candiSP = DBK_StPtLeft(width, BLKN_adj, __max((CurV-40), 0), CurV+1);
			if (candiSP != 9999)
			{
				restart++;
				if (restart > 5)
					return 0;
				if ( (abs(candiSP - CurV)%width) != 0 )
				{
					for (bin=-4;bin<=4;bin++)
					{
						if ( ((CurV+bin) >=0) && ((CurV+bin) <  frameHeight) && (BLKN_mark[CurV+bin] > 0) )
							BLKN_mark[CurV+bin] = markDefValue;
						if ( ((CurV+bin+width) >=0) && ((CurV+width+bin) <  frameHeight) && (BLKN_mark[CurV+width+bin] > 0))
							BLKN_mark[CurV+width+bin] = markDefValue;
						if ( ((CurV+bin+width*2) >=0) && ((CurV+width*2+bin) <  frameHeight) && (BLKN_mark[CurV+width*2+bin] > 0))
							BLKN_mark[CurV+width*2+bin] = markDefValue;
					}
				}
				misscnt = 0;
				CurV = candiSP;


				for (bin=-r;bin<=r;bin++)
				{
					if ((CurV+bin >= 0) && (CurV+bin <frameHeight) )
						BLKN_mark[CurV+bin] = BLKN_adj[CurV+bin];
					if (((CurV+width+bin) >= 0) && (CurV+width+bin <frameHeight) )
						BLKN_mark[CurV+width+bin] = BLKN_adj[CurV+width+bin];
					if (((CurV+width*2+bin) >= 0) && ((CurV+width*2+bin) <frameHeight) )
						BLKN_mark[CurV+width*2+bin] = BLKN_adj[CurV+width*2+bin];
				}
				CurV -= width;
			}
			else
			{
				for (bin = CurV; bin > __max((CurV-40), 0); bin= bin-width )
				{
					if (bin+width<frameHeight)
					BLKN_mark[bin] = (BLKN_adj[bin]+BLKN_adj[bin+width])/2;
				}
				CurV -= 40;
			}
		}
		else
		{
			BLKN_mark[CurV] = BLKN_adj[CurV]/2;
			if (CurV+1 < frameHeight)
				BLKN_mark[CurV+1] = BLKN_adj[CurV+1]/2;
			if (CurV-1 >= 0)
				BLKN_mark[CurV-1] = BLKN_adj[CurV-1]/2;
			CurV -= width;
		}
	}
	return hitrate;
}

int DBK_Mark_Ver_Test(void *pInstance,MS_U8* BLKN_mark, MS_U8 *BLKN_adj, const int width, const MS_U32 u32IniIndex, MS_U8 markValue, MS_U8 markDefValue)
{
	int hitrate = 0;
	int CurV;
	int MarkTh = 7;
	int L1, R1;

	//sp
	BLKN_mark[u32IniIndex] = BLKN_adj[u32IniIndex];
	if (u32IniIndex-1 >= 0)
		BLKN_mark[u32IniIndex-1] = BLKN_adj[u32IniIndex-1];
	if (u32IniIndex+1<frameHeight)
		BLKN_mark[u32IniIndex+1] = BLKN_adj[u32IniIndex+1];
	hitrate++;
	//right hand side
	for ( CurV=u32IniIndex+width; CurV<frameHeight; )
	{

		L1 = (CurV-width)<0?CurV:(CurV-width);
		R1 = (CurV+width)>=frameHeight?CurV:(CurV+width);

		//BLKN_mark[CurV] = __minmax((__max(BLKN_adj[L1], BLKN_adj[L2])+BLKN_adj[CurV]*2+__max(BLKN_adj[R1],BLKN_adj[R2]))/*>>1*/, markDefValue, markValue);
		BLKN_mark[CurV] = __minmax((BLKN_adj[L1]+BLKN_adj[CurV]*2+BLKN_adj[R1])>>1, markDefValue, markValue);
		if (CurV-1 >= 0)
		{
			L1 = (CurV-1-width)   < 0 ? (CurV-1) : (CurV-1-width);
			R1 = (CurV-1+width)  >= frameHeight ? (CurV-1) : (CurV-1+width);
			//BLKN_mark[CurV-1] = __minmax((__max(BLKN_adj[L1], BLKN_adj[L2])+BLKN_adj[CurV-1]*2+__max(BLKN_adj[R1],BLKN_adj[R2]))/*>>1*/, markDefValue, markValue);
			BLKN_mark[CurV-1] = __minmax((BLKN_adj[L1]+BLKN_adj[CurV-1]*2+BLKN_adj[R1])>>1, markDefValue, markValue);
		}
		if (CurV+1 < frameHeight)
		{
			L1 = (CurV+1-width)   < 0 ? (CurV+1) : (CurV+1-width);
			R1 = (CurV+1+width)   >=frameHeight ? (CurV+1) : (CurV+1+width);
			//BLKN_mark[CurV+1] = __minmax((__max(BLKN_adj[L1], BLKN_adj[L2])+BLKN_adj[CurV+1]*2+__max(BLKN_adj[R1],BLKN_adj[R2]))/*>>1*/, markDefValue, markValue);
			BLKN_mark[CurV+1] = __minmax((BLKN_adj[L1]+BLKN_adj[CurV+1]*2+BLKN_adj[R1])>>1, markDefValue, markValue);
		}
		if (__max3(BLKN_mark[CurV], BLKN_mark[CurV-1], BLKN_mark[CurV+1]) > MarkTh )
			hitrate++;
		CurV += width;
	}
	//left hand side
	for (CurV=u32IniIndex-width; CurV >= 0;)
	{
		L1 = (CurV-width)   < 0 ? CurV : (CurV-width);
		R1 = (CurV+width)   >=frameHeight ? CurV : (CurV+width);
		//BLKN_mark[CurV] = __minmax((__max(BLKN_adj[L1], BLKN_adj[L2])+BLKN_adj[CurV]*2+__max(BLKN_adj[R1],BLKN_adj[R2]))/*>>1*/, markDefValue, markValue);
		BLKN_mark[CurV] = __minmax((BLKN_adj[L1]+BLKN_adj[CurV]*2+BLKN_adj[R1])>>1, markDefValue, markValue);
		if (CurV-1 >= 0)
		{
			L1 = (CurV-1-width)   < 0 ? (CurV-1) : (CurV-1-width);
			R1 = (CurV-1+width)   >=frameHeight ? (CurV-1) : (CurV-1+width);
			//BLKN_mark[CurV-1] = __minmax((__max(BLKN_adj[L1], BLKN_adj[L2])+BLKN_adj[CurV-1]*2+__max(BLKN_adj[R1],BLKN_adj[R2]) )/*>>1*/, markDefValue, markValue);
			BLKN_mark[CurV-1] = __minmax((BLKN_adj[L1]+BLKN_adj[CurV-1]*2+BLKN_adj[R1])>>1, markDefValue, markValue);
		}
		if (CurV+1 < frameHeight)
		{

			L1 = (CurV+1-width)   < 0 ? (CurV+1) : (CurV+1-width);
			R1 = (CurV+1+width)   >=frameHeight ? (CurV+1) : (CurV+1+width);

			//BLKN_mark[CurV+1] = __minmax((__max(BLKN_adj[L1], BLKN_adj[L2])+BLKN_adj[CurV+1]*2+__max(BLKN_adj[R1], BLKN_adj[R2]) )/*>>1*/, markDefValue, markValue);
			BLKN_mark[CurV+1] = __minmax((BLKN_adj[L1]+BLKN_adj[CurV+1]*2+BLKN_adj[R1])>>1, markDefValue, markValue);
		}
		if (__max3(BLKN_mark[CurV], BLKN_mark[CurV-1], BLKN_mark[CurV+1]) > MarkTh )
			hitrate++;
		CurV -= width;
	}
	return hitrate;
}

int writeToVSRAMPart(void *pInstance,MS_U8* BlockinessCheck, int partIndex, int VSRAMTIMESLOT)
{
	int bin;
	int ack_flag =0;
	//static int pos = 0;
	int count = 0;
	MS_U32 Data0, Data1, Data;
	int IniPos, EndPos;
	if( (partIndex < VSRAMTIMESLOT) && (VSRAMTIMESLOT!=0) )
	{
		IniPos = frameHeight*partIndex/VSRAMTIMESLOT;
		EndPos = frameHeight*(partIndex+1)/VSRAMTIMESLOT;
	}
	else
	{
		IniPos = 0;
		EndPos = 0;
		return 0;
	}
	//WRITE TO BLOCKNESS POSITION SRAM
	MApi_XC_W2BYTEMSK(REG_SC_BK60_78_L,0x8000,0x8000);//blkwei  sram IO enable: h0008(0x08) 8000 = 15th bit = 1


	for(bin=IniPos; bin<EndPos;bin+=2)
	{
		Data0 = (BlockinessCheck[bin])  &0xF; //4bit
		Data1 = (BlockinessCheck[bin+1])&0xF; //4bit
		Data = (Data1<<4)|Data0; // Two 4bit combined to 8bit

		MApi_XC_W2BYTEMSK(REG_SC_BK60_78_L, bin/2,0x0FFF);//blkwei  sram io address
		MApi_XC_W2BYTEMSK(REG_SC_BK60_79_L,Data,0x00FF);//blkwei  sram write data
		MApi_XC_W2BYTEMSK(REG_SC_BK60_79_L,0x8000,0x8000);//blkwei  sram write enable

		//Sleep( 1);
		ack_flag = (MApi_XC_R2BYTEMSK(REG_SC_BK60_78_L,0x4000)>>14); // 14th bit of h0008
		count = 0;
		while ((ack_flag==0)&&(count<10))
		{
			count++;

			MApi_XC_W2BYTEMSK(REG_SC_BK60_78_L, bin/2,0x0FFF);//blkwei  sram io address
			MApi_XC_W2BYTEMSK(REG_SC_BK60_79_L,Data,0x00FF);//blkwei  sram write data
			MApi_XC_W2BYTEMSK(REG_SC_BK60_79_L,0x8000,0x8000);//blkwei  sram write enable
			ack_flag = (MApi_XC_R2BYTEMSK(REG_SC_BK60_78_L,0x4000)>>14); // 14th bit of h0008
		}
	}
	//pos = bin;
	//MApi_XC_W2BYTEMSK(REG_SC_BK60_78_L,0x2000,0x2000);//blkwei  fun en  h0008(0x08) 8000 = 13th bit = 1

	return 1;
}
void WidthIIR_V(void *pInstance,int width) // Width IIR Protection
{
	static int widthPP = 8;
	static int widthP  = 8;
	static int widthC  = 8;
	MS_BOOL sw = 0;
	if( (widthPP== widthP) && (widthP==widthC) && (widthC==width) )
		sw = 1;
	if (sw)
		WidthIIR = width;
	widthPP = widthP;
	widthP = widthC;
	widthC = width;
}
int VDBK_WidthPartHW(void *pInstance,int *WidthDiffHW, int* WidthCandidate)
{
	int MaxMinDiff;
	int avg =0;

	int idx;
	int maxWidthDiff  = 0;
	int minWidthDiff  = 0x7FFFFFFF;
	int min2WidthDiff = 0x7FFFFFFF;
	int min3WidthDiff = 0x7FFFFFFF;
	int min4WidthDiff = 0x7FFFFFFF;
	int outWidth = -1;
	int minIdx   = -1, min2Idx   = -1, min3Idx   = -1, min4Idx   = -1;
	int minWidth = -1, min2Width = -1, min3Width = -1, min4Width = -1;
    int DiffThrd = 2048;
	//long ScalingRatio = 1<<20;
	long ScalingRatio = (MApi_XC_R2BYTEMSK(REG_SC_BK23_0A_L,0x00FF)<<16) + MApi_XC_R2BYTEMSK(REG_SC_BK23_09_L,0xFFFF);
	const int OrigWidth = 8;
	long EstWidth = (ScalingRatio*OrigWidth + (1<<19) ) >> 20;

    WidthDiffHW[0 ] = MApi_XC_R2BYTEMSK(REG_SC_BK60_60_L,0xFFFF); // width 5
    WidthDiffHW[1 ] = MApi_XC_R2BYTEMSK(REG_SC_BK60_61_L,0xFFFF); // width 6
    WidthDiffHW[2 ] = MApi_XC_R2BYTEMSK(REG_SC_BK60_62_L,0xFFFF); // width 7
    WidthDiffHW[3 ] = MApi_XC_R2BYTEMSK(REG_SC_BK60_63_L,0xFFFF); // width 8
    WidthDiffHW[4 ] = MApi_XC_R2BYTEMSK(REG_SC_BK60_64_L,0xFFFF); // width 9
    WidthDiffHW[5 ] = MApi_XC_R2BYTEMSK(REG_SC_BK60_65_L,0xFFFF); // width 10
    WidthDiffHW[6 ] = MApi_XC_R2BYTEMSK(REG_SC_BK60_66_L,0xFFFF); // width 11
    WidthDiffHW[7 ] = MApi_XC_R2BYTEMSK(REG_SC_BK60_67_L,0xFFFF); // width 12
    WidthDiffHW[8 ] = MApi_XC_R2BYTEMSK(REG_SC_BK60_68_L,0xFFFF); // width 13
    WidthDiffHW[9 ] = MApi_XC_R2BYTEMSK(REG_SC_BK60_69_L,0xFFFF); // width 14
    WidthDiffHW[10] = MApi_XC_R2BYTEMSK(REG_SC_BK60_6A_L,0xFFFF); // width 15
    WidthDiffHW[11] = MApi_XC_R2BYTEMSK(REG_SC_BK60_6B_L,0xFFFF); // width 16
    WidthDiffHW[12] = MApi_XC_R2BYTEMSK(REG_SC_BK60_6C_L,0xFFFF); // width 17
    WidthDiffHW[13] = MApi_XC_R2BYTEMSK(REG_SC_BK60_6D_L,0xFFFF); // width 18
    WidthDiffHW[14] = MApi_XC_R2BYTEMSK(REG_SC_BK60_6E_L,0xFFFF); // width 19
    WidthDiffHW[15] = MApi_XC_R2BYTEMSK(REG_SC_BK60_6F_L,0xFFFF); // width 20
    for(idx = 0 ; idx < paraVerSize ; idx++)
	{
		if (WidthDiffHW[idx] > maxWidthDiff)
		{
			maxWidthDiff = WidthDiffHW[idx];
		}
	}
    for(idx = 0 ; idx < paraVerSize -1; idx+=2)
	{
		if ( (idx==2 || idx==10 ) && (WidthDiffHW[idx+1] - WidthDiffHW[idx ] <= DiffThrd) )
		{
			WidthDiffHW[idx+1]= __min(WidthDiffHW[idx ], WidthDiffHW[idx+1]);
			WidthDiffHW[idx  ]= maxWidthDiff;
		}
	}

	for(idx = 0 ; idx < paraVerSize ; idx++)
	{
		WidthCandidate[idx] = 0;
		avg += WidthDiffHW[idx];

		if (WidthDiffHW[idx] < minWidthDiff)
		{
			min4WidthDiff = min3WidthDiff;
			min4Idx = min3Idx;
			min3WidthDiff = min2WidthDiff;
			min3Idx = min2Idx;
			min2WidthDiff = minWidthDiff;
			min2Idx = minIdx;

			minWidthDiff = WidthDiffHW[idx];
			minIdx = idx;
		}
		else if (WidthDiffHW[idx] < min2WidthDiff)
		{
			min4WidthDiff = min3WidthDiff;
			min4Idx = min3Idx;
			min3WidthDiff = min2WidthDiff;
			min3Idx = min2Idx;

			min2WidthDiff = WidthDiffHW[idx];
			min2Idx = idx;
		}
		else if (WidthDiffHW[idx] < min3WidthDiff)
		{
			min4WidthDiff = min3WidthDiff;
			min4Idx = min3Idx;

			min3WidthDiff = WidthDiffHW[idx];
			min3Idx = idx;
		}
		else if (WidthDiffHW[idx] < min4WidthDiff)
		{
			min4WidthDiff = WidthDiffHW[idx];
			min4Idx = idx;
		}
	}

    avg = avg / paraVerSize;
	MaxMinDiff = maxWidthDiff - minWidthDiff;
	if ( minIdx  != -1)
		minWidth  = minIdx +5;
	if ( min2Idx != -1)
		min2Width = min2Idx+5;
	if ( min3Idx != -1)
		min3Width = min3Idx+5;
	if ( min4Idx != -1)
		min4Width = min4Idx+5;

	//PRT_TRP(" --min4WidthDiff = %d, min4Idx=%d, min4Width =%d \n", min4WidthDiff, min4Idx, min4Width);
	//PRT_TRP(" --min3WidthDiff = %d, min3Idx=%d, min3Width =%d \n", min3WidthDiff, min3Idx, min3Width);
	//PRT_TRP(" --min2WidthDiff = %d, min2Idx=%d, min2Width =%d \n", min2WidthDiff, min2Idx, min2Width);
	//PRT_TRP(" --minWidthDiff  = %d, minIdx =%d, minWidth  =%d \n", minWidthDiff , minIdx , minWidth );
	//PRT_TRP(" --maxWidthDiff  = %d, maxIdx =%d \n", maxWidthDiff , maxIdx );
	//PRT_TRP(" --avg  = %d \n", avg );
	//PRT_TRP(" --MaxMinDiff  = %d \n", MaxMinDiff );

	WidthCandidate[minIdx ] = 1;
	if (MaxMinDiff > 0)
	{
	if ( (min2WidthDiff-minWidthDiff)*256/MaxMinDiff <48)
		WidthCandidate[min2Idx] = 1;
	if ( (min3WidthDiff-minWidthDiff)*256/MaxMinDiff <48)
		WidthCandidate[min3Idx] = 1;
	if ( (min4WidthDiff-minWidthDiff)*256/MaxMinDiff <48)
		WidthCandidate[min4Idx] = 1;
	}

	if (minWidth == EstWidth || min2Width == EstWidth || min3Width == EstWidth || min4Width == EstWidth)
		outWidth = EstWidth;
	else if (minWidth == EstWidth*2 || min2Width == EstWidth*2 || min3Width == EstWidth*2 || min4Width == EstWidth*2)
		outWidth = EstWidth;
	else
	{
		if ( abs(min2Width - minWidth) == 1 )
		{
			if (min2Idx < minIdx)
			{
				WidthCandidate[ min2Idx] = 0;
			}
			else
			{
				WidthCandidate[ minIdx] = 0;
			}
		}
		if ( abs(min3Width - minWidth) == 1 )
		{
			if (min3Idx < minIdx)
			{
				WidthCandidate[ min3Idx] = 0;
			}
			else
			{
				WidthCandidate[ minIdx] = 0;
			}
		}
		if ( abs(min4Width - minWidth) == 1 )
		{
			if (min4Idx < minIdx)
			{
				WidthCandidate[ min4Idx] = 0;
			}
			else
			{
				WidthCandidate[ minIdx] = 0;
			}
		}

		if ( abs(min3Width - min2Width) == 1 )
		{
			if (min3Idx < min2Idx)
			{
				WidthCandidate[ min3Idx] = 0;
			}
			else
			{
				WidthCandidate[ min2Idx] = 0;
			}
		}
		if ( abs(min4Width - min2Width) == 1 )
		{
			if (min4Idx < min2Idx)
			{
				WidthCandidate[ min4Idx] = 0;
			}
			else
			{
				WidthCandidate[ min2Idx] = 0;
			}
		}
		if ( abs(min4Width - min3Width) == 1)
		{
			if (min4Idx < min3Idx)
			{
				WidthCandidate[ min4Idx] = 0;
			}
			else
			{
				WidthCandidate[ min3Idx] = 0;
			}
		}
		if (WidthCandidate[ min4Idx] > 0)
		{
			if ( min4Width == min3Width*2 || min4Width == min2Width*2 || min4Width == minWidth*2 )
			{
				WidthCandidate[ min4Idx] = 0;
			}
			if ( min3Width == min4Width*2 || min3Width == min2Width*2 || min3Width == minWidth*2 )
			{
				WidthCandidate[ min3Idx] = 0;
			}
			if ( min2Width == min4Width*2 || min2Width == min3Width*2 || min2Width == minWidth*2 )
			{
				WidthCandidate[ min2Idx] = 0;
			}
			if ( minWidth == min4Width*2 || minWidth == min3Width*2 || minWidth == min2Width*2 )
			{
				WidthCandidate[ minIdx] = 0;
			}
		}

		if (WidthCandidate[minIdx] + WidthCandidate[min2Idx] + WidthCandidate[min3Idx] + WidthCandidate[min4Idx] == 1)
		{
			outWidth = WidthCandidate[minIdx]* minWidth + WidthCandidate[min2Idx]* min2Width + WidthCandidate[min3Idx]* min3Width + WidthCandidate[min4Idx]* min4Width;
		}
		else
			outWidth = -1;
	}
	return outWidth;
}
int VDBK_BlocknessAdjPart(void *pInstance,long* histogram, MS_U8 *BLKN_adj, int partIndex, MS_U8 markValue, MS_U8 markDefValue, int VSRAMTIMESLOT)
{
	markValue = 0xF;
	markDefValue = 0x0;

	int avgBLKN = 0;
	const int localWidth = 10;
	int ThrHei = frameWidth;
	int valid = 0;
	int bin, CurV;
	//    MS_U8 markNorm = (markValue+1)/2;
	int movingSum = 0;
	int blkness;

	int IniPos, EndPos;
	if( (partIndex < VSRAMTIMESLOT) && (VSRAMTIMESLOT!=0) )
	{
		IniPos = frameHeight*partIndex/VSRAMTIMESLOT;
		EndPos = frameHeight*(partIndex+1)/VSRAMTIMESLOT;
	}
	else
	{
		IniPos = 0;
		EndPos = 0;
		return 0;
	}
#if 0
	if (partIndex == 0)
	{
		IniPos = 0;
		EndPos = frameHeight/2;
	}
	else if (partIndex == 1)
	{
		IniPos = frameHeight/2;
		EndPos = frameHeight;
	}
	else
	{
		IniPos = 0;
		EndPos = 0;
		return 0;
	}
#endif

	for(CurV = IniPos; CurV < EndPos; CurV++)
	{
		if ( ( CurV < (localWidth+1)/2 - 1) || ( CurV > frameHeight - (localWidth+1)/2 ) )
		{
			//Cutoff topmost, downmost positions
			avgBLKN = 0;
			BLKN_adj[CurV] = markDefValue;
		}
		else if (CurV == ( (localWidth+1/2) - 1) || CurV == IniPos )
		{
			movingSum = 0;
			for (bin=-(localWidth+1)/2+1;bin < (localWidth+1)/2;bin++)
			{
				movingSum += histogram[CurV+bin];
			}
			if (localWidth!= 0)
				avgBLKN = movingSum / localWidth;
			else
				avgBLKN = 0;

			blkness = (histogram[ CurV] - avgBLKN);
			if (blkness > ThrHei*2 )
				BLKN_adj[CurV] = markValue;
			else if (blkness > ThrHei )
				BLKN_adj[CurV] = (markValue+markDefValue)/2;
			else
				BLKN_adj[CurV] = markDefValue;
		}
		else
		{
			movingSum = movingSum - histogram[CurV-(localWidth+1)/2+1] + histogram[CurV+(localWidth+1)/2-1];
			if (localWidth!= 0)
				avgBLKN = movingSum / localWidth;
			else
				avgBLKN = 0;

			blkness = (histogram[ CurV] - avgBLKN);
			if (blkness > ThrHei*2 )
				BLKN_adj[CurV] = markValue;
			else if (blkness > ThrHei )
				BLKN_adj[CurV] = (markValue+markDefValue)/2;
			else
				BLKN_adj[CurV] = markDefValue;
		}
		if (CurV == frameHeight-1)
			BLKN_adj[CurV+1] = BLKN_adj[CurV]; //duplicate one point for boundary control

	}

	return valid;
}
void SetVDBKParameter(void *pInstance,int HDBK_Str, int HDBK_Width)
{
	//int bin, CurV;
	int VGain = 0;
	if (1)
	{
		VMStrMax = 0xF;

		MApi_XC_W2BYTEMSK(REG_SC_BK60_20_L, 0x0006, 0x0006);
		MApi_XC_W2BYTEMSK(REG_SC_BK60_20_L, 0x1000, 0x1000);
		MApi_XC_W2BYTEMSK(REG_SC_BK60_21_L, 0x0147, 0x0377);
		//MApi_XC_W2BYTEMSK(REG_SC_BK60_22_L, 0x0811, 0x0F33);
		MApi_XC_W2BYTEMSK(REG_SC_BK60_22_L, 0x0011, 0x0033);

		MApi_XC_W2BYTEMSK(REG_SC_BK60_23_L, 0x8000, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK60_24_L, 0x4088, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK60_25_L, 0x0182, 0x07FF);
		MApi_XC_W2BYTEMSK(REG_SC_BK60_26_L, 0x008F, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK60_27_L, 0x0888, 0x0FFF);
		//Statistics
		MApi_XC_W2BYTEMSK(REG_SC_BK60_28_L, 0x8000, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK60_29_L, 0x4088, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK60_2A_L, 0x0182, 0x07FF);
		MApi_XC_W2BYTEMSK(REG_SC_BK60_2B_L, 0x008F, 0xFFFF);
		MApi_XC_W2BYTEMSK(REG_SC_BK60_2C_L, 0x0888, 0xFFFF);

	}
	//long VScalingRatio = ( (MApi_XC_R2BYTEMSK(REG_SC_BK23_1D_L,0x00FF)<<16) + MApi_XC_R2BYTEMSK(REG_SC_BK23_1C_L,0xFFFF) ) >>16;
	//PRT_TRP(" --VScalingRatio--(%d) / frameHeight--(%d) ; \n", VScalingRatio, frameHeight);

	//Use HDBK strength to adjust VDBK controls
	MS_BOOL ifPIP_en = (MApi_XC_R2BYTEMSK(REG_SC_BK20_10_L, 0x0003) == 0x0003);
	if ((ifPIP_en) || (frameWidth <= 800))
		MApi_XC_W2BYTEMSK(REG_SC_BK60_20_L, 0x0000, 0x0001); //VDBK disable for PIP
	else
		MApi_XC_W2BYTEMSK(REG_SC_BK60_20_L, 0x0001, 0x0001); //VDBK enable

	if (HDBK_Width != 8)
	{


		if (HDBK_Str >= 80)
		{
			VGain = 0x8;
			MApi_XC_W2BYTEMSK(REG_SC_BK60_22_L, VGain<<8, 0x0F00); //v total gain
			MApi_XC_W2BYTEMSK(REG_SC_BK60_78_L,0x2000,0x2000);//blkwei  fun en  h0008(0x08) 8000 = 13th bit = 1
		}
		else if ( HDBK_Str >= 60 && HDBK_Str < 80)
		{
			VGain = 0x8/*(HDBK_Str-40)/(80-40)*/;
			MApi_XC_W2BYTEMSK(REG_SC_BK60_22_L, VGain<<8, 0x0F00); //v total gain
			MApi_XC_W2BYTEMSK(REG_SC_BK60_78_L,0x2000,0x2000);//blkwei  fun en  h0008(0x08) 8000 = 13th bit = 1
		}
		else if ( HDBK_Str >= 40 && HDBK_Str < 60)
		{
			VGain = 0x8*(HDBK_Str-20)/(60-20);
			MApi_XC_W2BYTEMSK(REG_SC_BK60_22_L, VGain<<8, 0x0F00); //v total gain
			MApi_XC_W2BYTEMSK(REG_SC_BK60_78_L,0x2000,0x2000);//blkwei  fun en  h0008(0x08) 8000 = 13th bit = 1
		}
		else if ( HDBK_Str >= 20 && HDBK_Str < 40)
		{
			VGain = 0x8*(HDBK_Str-20)/(60-20);
			MApi_XC_W2BYTEMSK(REG_SC_BK60_22_L, VGain<<8, 0x0F00); //v total gain
			MApi_XC_W2BYTEMSK(REG_SC_BK60_78_L,0x2000,0x2000);//blkwei  fun en  h0008(0x08) 8000 = 13th bit = 1
		}
		else
		{
			VGain = 0x0;
			MApi_XC_W2BYTEMSK(REG_SC_BK60_22_L, VGain<<8, 0x0F00); //v total gain
            MApi_XC_W2BYTEMSK(REG_SC_BK60_78_L,0x2000,0x2000);//blkwei  fun en  h0008(0x08) 8000 = 13th bit = 1
		}
	}
	else //HDBK_Width = 8
	{


		if (HDBK_Str >= 80)
		{
			VGain = 0x8;
			MApi_XC_W2BYTEMSK(REG_SC_BK60_22_L, VGain<<8, 0x0F00); //v total gain
			MApi_XC_W2BYTEMSK(REG_SC_BK60_78_L,0x2000,0x2000);//blkwei  fun en  h0008(0x08) 8000 = 13th bit = 1
		}
		else if ( HDBK_Str >= 60 && HDBK_Str < 80)
		{
			VGain = 0x8/*(HDBK_Str-40)/(80-40)*/;
			MApi_XC_W2BYTEMSK(REG_SC_BK60_22_L, VGain<<8, 0x0F00); //v total gain
			MApi_XC_W2BYTEMSK(REG_SC_BK60_78_L,0x2000,0x2000);//blkwei  fun en  h0008(0x08) 8000 = 13th bit = 1
		}
		else if ( HDBK_Str >= 40 && HDBK_Str < 60)
		{
			VGain = 0x8*(HDBK_Str-20)/(60-20);
			MApi_XC_W2BYTEMSK(REG_SC_BK60_22_L, VGain<<8, 0x0F00); //v total gain
			MApi_XC_W2BYTEMSK(REG_SC_BK60_78_L,0x2000,0x2000);//blkwei  fun en  h0008(0x08) 8000 = 13th bit = 1
		}
		else if ( HDBK_Str >= 20 && HDBK_Str < 40)
		{
			VGain = 0x8*(HDBK_Str-20)/(60-20);
			MApi_XC_W2BYTEMSK(REG_SC_BK60_22_L, VGain<<8, 0x0F00); //v total gain
			MApi_XC_W2BYTEMSK(REG_SC_BK60_78_L,0x2000,0x2000);//blkwei  fun en  h0008(0x08) 8000 = 13th bit = 1
		}
		else
		{
			VGain = 0x0;
			MApi_XC_W2BYTEMSK(REG_SC_BK60_22_L, VGain<<8, 0x0F00); //v total gain
			MApi_XC_W2BYTEMSK(REG_SC_BK60_78_L,0x0000,0x2000);//blkwei  fun en  h0008(0x08) 8000 = 13th bit = 1
		}
	}
	return;
}
static long vhistogram[MaxHeight] = {0};
int VDBK_Detect( void *pInstance,int HDBK_Str, int* cntForDBK, int VSRAMTIMESLOT)
{

	static int WidthCal = 0;
	static int HDBK_Width = 0;
	static int VDBK_Width = 0;
	static int WidthDiffHW[paraVerSize];
	static int WidthDiffCand[paraVerSize];
	static MS_U8 BLKN_adj_ver[MaxHeight] = {0};   //Normalized histogram
	static MS_U8 BLKN_mark_ver[MaxHeight]; //blockness marking array
	static MS_U8 BLKN_markext[MaxHeight]; //
	static MS_U8 BLKN_marklpf_ver[MaxHeight] = {0};   //
	int bin, CurV;

	MS_U32 u32IniIndex = 0;

	if (*cntForDBK == 0)
	{
        if ( MApi_XC_R2BYTEMSK(REG_SC_BK0C_39_L,0xFFFF) == 0x0808 )
			HDBK_Width = 0x8;
		else
			HDBK_Width = ( (MApi_XC_R2BYTEMSK(REG_SC_BK0C_39_L,0xFF00) >> 8 ) + MApi_XC_R2BYTEMSK(REG_SC_BK0C_39_L,0x00FF) ) / 2;


		for (bin=0; bin<paraVerSize; bin++)
			WidthDiffCand[bin] = 0;

		SetVDBKParameter(pInstance,HDBK_Str, HDBK_Width);
		if (HDBK_Str < HDBK_Str_Thrd)
		{
			*cntForDBK = VSRAMTIMESLOT*2+2;
			for (CurV = 0; CurV < frameHeight; CurV++)
			{
				BLKN_marklpf_ver[CurV] = 0;
			}
			return 0;
		}
		WidthCal = VDBK_WidthPartHW(pInstance,WidthDiffHW, WidthDiffCand);
		MApi_XC_W2BYTEMSK(REG_SC_BK30_0F_L, WidthCal<<12, 0xF000); //v width display
	}
	if (*cntForDBK <= (VSRAMTIMESLOT-1) ) //
	{
		int readHistAck;
		readHistAck = readVHistogramPart(pInstance,vhistogram, *cntForDBK, VSRAMTIMESLOT);
		if( (*cntForDBK == VSRAMTIMESLOT-1 )&& readHistAck)
		{
			MApi_XC_W2BYTEMSK(REG_SC_BK60_7D_L, 1, 0x0001);
		}
	}
	else if (*cntForDBK <= (VSRAMTIMESLOT*2-1))
	{
		VDBK_BlocknessAdjPart(pInstance,vhistogram, BLKN_adj_ver, *cntForDBK-2, VMStrMax, VMStrMin, VSRAMTIMESLOT);
	}
	else if (*cntForDBK <= (VSRAMTIMESLOT*2))
	{

		if (WidthCal != -1)
		{

			VDBK_Width = WidthCal;

			if (VDBK_Width == 8)
			{
				u32IniIndex = DBK_StPtLeft( VDBK_Width, BLKN_adj_ver, frameHeight/4, frameHeight*3/4); //Left to right search
                if(u32IniIndex<=MaxHeight)
                    DBK_Mark_Ver_Test(pInstance,BLKN_mark_ver, BLKN_adj_ver, VDBK_Width, u32IniIndex, 0xF, 0x0); //0x8, 0x4
			}
			else
			{
				for (CurV = 0; CurV < frameHeight; CurV++)
					BLKN_mark_ver[CurV] = 0;

			}
		}
		else
		{
			{
				for (CurV = 0; CurV < frameHeight; CurV++)
					BLKN_mark_ver[CurV] = 0;
			}
		}
	}
	else if (*cntForDBK <= (VSRAMTIMESLOT*2+1))
	{
		if ( 1)
		{
			int UpIdx, DnIdx;
			for (CurV = 0; CurV < frameHeight; CurV++)
			{
				UpIdx = CurV - 1;
				DnIdx = CurV + 1;
				if (UpIdx < 0)
				{
					//BLKN_markext[CurV] = __max3(BLKN_mark_ver[CurV ], BLKN_mark_ver[CurV], BLKN_mark_ver[DnIdx]);
					BLKN_markext[CurV] = __max(BLKN_mark_ver[CurV], BLKN_mark_ver[DnIdx]);
				}
				else if (DnIdx >= frameHeight)
				{
					//BLKN_markext[CurV] = __max3(BLKN_mark_ver[UpIdx], BLKN_mark_ver[CurV], BLKN_mark_ver[CurV ]);
					BLKN_markext[CurV] = __max(BLKN_mark_ver[CurV], BLKN_mark_ver[CurV ]);
				}
				else
				{
					//BLKN_markext[CurV] = __max3(BLKN_mark_ver[UpIdx], BLKN_mark_ver[CurV], BLKN_mark_ver[DnIdx]);
					BLKN_markext[CurV] = __max(BLKN_mark_ver[CurV], BLKN_mark_ver[DnIdx]);
				}
			}
		}
	}
	else if (*cntForDBK <= (VSRAMTIMESLOT*2+2))
	{
		if (1)
		{
			int UpIdx, DnIdx;
			for (CurV = 0; CurV < frameHeight; CurV++)
			{
				UpIdx = __minmax(CurV - 1, 0, frameHeight-1);
				DnIdx = __minmax(CurV + 1, 0, frameHeight-1);
				if (UpIdx < 0)
				{
					BLKN_marklpf_ver[CurV] = (BLKN_markext[CurV ]*2 + BLKN_markext[CurV]*4 + BLKN_markext[DnIdx]*2 + 4) >> 3;
				}
				else if (DnIdx >= frameHeight)
				{
					BLKN_marklpf_ver[CurV] = (BLKN_markext[UpIdx]*2 + BLKN_markext[CurV]*4 + BLKN_markext[CurV ]*2 + 4) >> 3;
				}
				else
				{
					BLKN_marklpf_ver[CurV] = (BLKN_markext[UpIdx]*2 + BLKN_markext[CurV]*4 + BLKN_markext[DnIdx]*2 + 4) >> 3;
				}

			}
		}
	}
	else if (*cntForDBK <= (VSRAMTIMESLOT*3+2))//7,8
	{
		writeToVSRAMPart(pInstance,BLKN_marklpf_ver, *cntForDBK-(VSRAMTIMESLOT*3+1), VSRAMTIMESLOT );
	}
	else
	{
		*cntForDBK+=10;
		return 1;
	}
	return 1;
}
void DBK_Frame_Estimate(void *pInstance, const int cntForDBK, long* CurFrameEdgStr, long* FrameBLKEdgIIR, long* CurFrameDetStr, long* FrameBLKDetIIR, long* CurFrameBLKStr, long* FrameBLKStrIIR, MS_BOOL SceneChange )
{
	static int IIRCurRatio = 4;
	if (SceneChange)
		IIRCurRatio = 12;
	else
		IIRCurRatio = 4;
	const int DETGAINSET = 0x5335;//MApi_XC_R2BYTEMSK(REG_SC_BK30_06_L, 0xFFFF);
	static MS_BOOL swSetting = 0;
	if ( ( cntForDBK >= 7 && cntForDBK <= 12) || ( cntForDBK >= 17 && cntForDBK <= 22) )
	{
		if ( MApi_XC_R2BYTEMSK(REG_SC_BK60_39_L, 0xFFFF) != DETGAINSET)
		{
			MApi_XC_W2BYTEMSK(REG_SC_BK60_39_L, DETGAINSET, 0xFFFF);
			swSetting = 1;
		}
		else
			//if ( cntForDBK != 6 && cntForDBK != 16 )
		{
			if (swSetting == 1)
			{
				swSetting = 0;//wait 1 frame
			}
			else
			{
				*CurFrameBLKStr = ( MApi_XC_R2BYTEMSK(REG_SC_BK0C_61_L, 0xFFFF) << 16 ) + MApi_XC_R2BYTEMSK(REG_SC_BK0C_60_L, 0xFFFF);
				*CurFrameBLKStr = (*CurFrameBLKStr)*8/(frameWidth*frameHeight);
				*FrameBLKStrIIR = ( (*FrameBLKStrIIR)*(16-IIRCurRatio) + (*CurFrameBLKStr)*IIRCurRatio )/16;
			}
		}
		if (cntForDBK == 12 || cntForDBK == 22)
		{
			MApi_XC_W2BYTEMSK(REG_SC_BK60_39_L, 0x0000, 0xFFFF);
			swSetting = 1;
		}
	}
	else
	{
		if ( MApi_XC_R2BYTEMSK(REG_SC_BK60_39_L, 0xFFFF) != 0x0000 )
		{
			MApi_XC_W2BYTEMSK(REG_SC_BK60_39_L, 0x0000, 0xFFFF);
			swSetting = 1;
		}
		//if (cntForDBK != 11 && cntForDBK != 21)
		else
		{
			if (swSetting == 1)
			{
				swSetting = 0;//wait 1 frame
			}
			else
			{
				(*CurFrameEdgStr) = ( MApi_XC_R2BYTEMSK(REG_SC_BK0C_61_L, 0xFFFF) << 16 ) + MApi_XC_R2BYTEMSK(REG_SC_BK0C_60_L, 0xFFFF);
				(*CurFrameEdgStr) = (*CurFrameEdgStr)*8/(frameWidth*frameHeight);
				(*FrameBLKEdgIIR) = ((*FrameBLKEdgIIR)*(16-IIRCurRatio) + (*CurFrameEdgStr)*IIRCurRatio )/16;
			}
		}
		if (cntForDBK == 6 || cntForDBK == 16 )
		{
			MApi_XC_W2BYTEMSK(REG_SC_BK60_39_L, DETGAINSET, 0xFFFF);
			swSetting = 1;
		}
	}
	(*CurFrameDetStr) = __max((*FrameBLKEdgIIR) - (*FrameBLKStrIIR), 0);
	(*FrameBLKDetIIR) = (*CurFrameDetStr);
	//FrameBLKDetIIR = (FrameBLKDetIIR*12 + CurFrameDetStr*4 )/16;

}
void DBK_Crma_Enable(void *pInstance) //Register Settings to hardware
{
		MS_BOOL DBK_En = (MApi_XC_R2BYTEMSK(REG_SC_BK0C_40_L, 0x0001) == 1)
					&& (MApi_XC_R2BYTEMSK(REG_SC_BK0C_10_L, 0x0001) == 1)
					&& (MApi_XC_R2BYTEMSK(REG_SC_BK0C_70_L, 0x0001) == 0);
		if (!DBK_En)
				MApi_XC_W2BYTEMSK(REG_SC_BK0C_02_L, 0, 0x0001);
		else
				MApi_XC_W2BYTEMSK(REG_SC_BK0C_02_L, 1, 0x0001);
}

void DMS_SDHDModeCheck(void *pInstance, MS_U8 u8SD2HDValue) //Register Settings to hardware
{


	if( (u8SD2HDValue>=60) && (frameWidth>0x700)  ) //SD to HD mode
		MApi_XC_W2BYTEMSK(REG_SC_BK26_10_L, 0x4000, 0x4000); // HD_enable
	else
		MApi_XC_W2BYTEMSK(REG_SC_BK26_10_L, 0x0000, 0x4000);
}
static MS_U16 g_u16BK0A_24 = 0;
static MS_U16 g_u16BK0C_10 = 0;
static MS_U16 g_u16BK0C_39 = 0;
static MS_U16 g_u16BK0C_3A = 0;
static MS_U16 g_u16BK0C_40 = 0;
//static MS_U16 g_u16BK0C_41 = 0;
static MS_U16 g_u16BK0C_42 = 0;
static MS_U16 g_u16BK26_10 = 0;
static MS_U16 g_u16BK26_11 = 0;
static MS_U16 g_u16BK26_12 = 0;
static MS_U16 g_u16BK26_13 = 0;
static MS_U16 g_u16BK26_18 = 0;

/******************************************************************************/
///Film mode control for Any Candence
///@param bEnable \     IN: Enable/Disable control
/******************************************************************************/
void Hal_PQ_FilmMode_AnyCandence_Enable(void *pInstance,MS_BOOL bEnable)
{
	MApi_XC_W2BYTEMSK(REG_SC_BK0A_24_L, (bEnable ? (BIT(15)) : 0), BIT(15));
	_bAnyCandenceEnable = bEnable;
}

/******************************************************************************/
///Store the Current value
/******************************************************************************/
void Hal_PQ_StoreCurrentValue(void *pInstance)
{
	//for AnyCandence Film Mode
	g_u16BK0A_24 = MApi_XC_R2BYTEMSK(REG_SC_BK0A_24_L, BIT(15));

	//for DBK
	g_u16BK0C_10 = MApi_XC_R2BYTE(REG_SC_BK0C_10_L);
	g_u16BK0C_39 = MApi_XC_R2BYTE(REG_SC_BK0C_39_L);
	g_u16BK0C_3A = MApi_XC_R2BYTE(REG_SC_BK0C_3A_L);
	g_u16BK0C_40 = MApi_XC_R2BYTE(REG_SC_BK0C_40_L);
	//g_u16BK0C_41 = MApi_XC_R2BYTE(REG_SC_BK0C_41_L);
	g_u16BK0C_42 = MApi_XC_R2BYTE(REG_SC_BK0C_42_L);

	//for DMS
	g_u16BK26_10 = MApi_XC_R2BYTE(REG_SC_BK26_10_L);
	g_u16BK26_11 = MApi_XC_R2BYTE(REG_SC_BK26_11_L);
	g_u16BK26_12 = MApi_XC_R2BYTE(REG_SC_BK26_12_L);
	g_u16BK26_13 = MApi_XC_R2BYTE(REG_SC_BK26_13_L);
	g_u16BK26_18 = MApi_XC_R2BYTE(REG_SC_BK26_18_L);
}

/******************************************************************************/
///Store the Current value
/******************************************************************************/
void Hal_PQ_ReStoreToDefault(void *pInstance)
{
	//for AnyCandence Film Mode
	MApi_XC_W2BYTEMSK(REG_SC_BK0A_24_L, g_u16BK0A_24, BIT(15));

	//for DBK
	MApi_XC_W2BYTE(REG_SC_BK0C_10_L, g_u16BK0C_10);
	MApi_XC_W2BYTE(REG_SC_BK0C_39_L, g_u16BK0C_39);
	MApi_XC_W2BYTE(REG_SC_BK0C_3A_L, g_u16BK0C_3A);
	MApi_XC_W2BYTE(REG_SC_BK0C_40_L, g_u16BK0C_40);
	//MApi_XC_W2BYTE(REG_SC_BK0C_41_L, g_u16BK0C_41);
	MApi_XC_W2BYTE(REG_SC_BK0C_42_L, g_u16BK0C_42);

	//for DMS
	MApi_XC_W2BYTE(REG_SC_BK26_10_L, g_u16BK26_10);
	MApi_XC_W2BYTE(REG_SC_BK26_11_L, g_u16BK26_11);
	MApi_XC_W2BYTE(REG_SC_BK26_12_L, g_u16BK26_12);
	MApi_XC_W2BYTE(REG_SC_BK26_13_L, g_u16BK26_13);
	MApi_XC_W2BYTE(REG_SC_BK26_18_L, g_u16BK26_18);
}
#define FPLL_PHASE_VAR_LIMIT    (0x10)          // If phase error variation exceed this, means un-stable
#define FPLL_PRD_THRESHOLD      (0x100)         // The PRD must be smaller than this value to indicate almost finished PRD lock
#define FPLL_PRD_VAR_LIMIT      (0x10)          // If PRD variation exceed this, means un-stable

//static MS_BOOL _Hal_PQ_FrameLockCheck (void *pInstance)
MS_BOOL _Hal_PQ_FrameLockCheck (void *pInstance)
{
    MS_U16 u16NewPhase,u16NewPrd;
    static MS_U16 _u16OrigPhase,_u16OrigPrd;

    u16NewPrd = MApi_XC_R2BYTEMSK(REG_LPLL_13_L, 0xFFFF);
    u16NewPhase = MApi_XC_R2BYTEMSK(REG_LPLL_11_L, 0xFFFF);

    if((u16NewPrd < FPLL_PRD_THRESHOLD) && (abs(_u16OrigPrd - u16NewPrd) < FPLL_PRD_VAR_LIMIT)
        && (abs(_u16OrigPhase - u16NewPhase) < FPLL_PHASE_VAR_LIMIT))
    {
        _u16OrigPhase = u16NewPhase;
        _u16OrigPrd = u16NewPrd;
        return TRUE;
    }
    else
    {
        _u16OrigPhase = u16NewPhase;
        _u16OrigPrd = u16NewPrd;
        return FALSE;
    }
}

static MS_BOOL bFakeOutCusEnable = TRUE;

MS_BOOL Hal_PQ_SetFakeOutEnable(void *pInstance,MS_BOOL bEnable)
{
    bFakeOutCusEnable = bEnable;
    return TRUE;
}

#define ENABLE_SCALER_DEFEATHERING           0x01   //BK22_7C_L
#define ENABLE_SCALER_DEFLICKERING           0x02
#define ENABLE_SCALER_DEBOUNCING             0x04
#define ENABLE_SCALER_D3D_DRIVER             0x08

#define ENABLE_SCALER_AUTO_DBK_DRIVER        0x01   //BK22_7C_H[0]
#define ENABLE_SCALER_MCDI_DRIVER            0x02   //BK22_7C_H[1]
#define ENABLE_SCALER_MCNR_DRIVER            0x04   //BK22_7C_H[2]
#define ENABLE_SCALER_SDHD_DETECT_DRIVER     0x08   //BK22_7C_H[3]

#define ENABLE_SCALER_CCS_DRIVER             0x20   //BK22_7C_H[5]
#define ENABLE_SCALER_DHD_DRIVER             0x40   //BK22_7C_H[6]
#define ENABLE_SCALER_AUTO_DBK_NMR_DRIVER    0x80   //BK22_7C_H[7]
#define ENABLE_SR_DRIVER    0x01  //BK2C30_3F_H[0]

//-------------------------------------------------------------------------------------------------
/// this function does adaptive tuning periodic
//-------------------------------------------------------------------------------------------------

void Hal_PQ_AdaptiveTuning(void *pInstance)
{

    MS_BOOL ukoSabihS_en = 0;
    MS_BOOL isUkosabihsMcSweep = false, isUkosabihsMotionSweep = false;
    MS_BOOL isUkosabihsForFilm = 0;
    static MS_U8 u8PreviousVcnt = 0;
    MS_U8 u8CurrentVcnt;
    MS_U8 ColorSum;
    //MS_U32 MR_Offset;
    //	MS_U16 MR_OffsetME1, MR_OffsetME2;
    MS_U32 u32MotionValue,u32MotionValue2,/*u32MotionValue3,*/TotalFeather,TotalMvComfirm;
    MS_U16 u16MotionValue4;
    MS_U32 uComplex,FeatherSum,u32MotionValue_Sum;
       MS_U8 NE_confirm = 0;
    
    XC_ApiStatus stXCStatus;
    memset(&stXCStatus, 0x00, sizeof(stXCStatus));
    
    MS_BOOL me1Still;
    MS_BOOL movingWithoutFeather;
    MS_BOOL isMcOutOfSearchRange;
    MS_U8 estNoise = 0;
    MS_U16 nonMv0Comfirm,mv0Comfirm,mvNotFound,nonMv0ComfirmME2,mv0ComfirmME2,mvNotFoundME2;//nonMv0Comfirm_noIIR,mv0Comfirm_noIIR,mvNotFound_noIIR;
    //	MS_U16 mvNotFoundME2,mvNotFound_noIIR;
    MS_U16 isFeatherAndMotion = false;
    
    MS_U16 me1IsMv0, me1IsNonMv0, me1IsNotFound;
    MS_U16 me2IsMv0, me2IsNonMv0, me2IsNotFound;
    MS_BOOL isSceneChangeMc, isSceneChangeY, isSceneChange = 0;//isScenceChangeByNotFound = 0;
    MS_BOOL isAbihsot = false; /* Toshixa */
    MS_BOOL isHBurstMoving = false, isHBurst = false;//isLowLuma;
    static MS_BOOL isMcFilm = false;
//	static MS_BOOL isMcFilmAny = false;
    static MS_BOOL isMcFilmHd = false;
    static MS_BOOL isMcFilmukoSabihS = false;
    
    MS_BOOL isMcFilmSlowMotion = true;
    
    //MS_BOOL isLrLetterBox = false;
    MS_BOOL isL139 = FALSE, isL107 = FALSE, isL153 =FALSE, isUkosabihsMotion = FALSE, isUkosabihsMc = FALSE, isUkosabihsMv0 = FALSE;// isSWoman = FALSE;
//	MS_BOOL isWeakMcNrFilm = false; /* for delay one frame issue */
    MS_BOOL Bouncing_status,bBouncing_Status2,Feather_sum_status,Feather_sum_status2;
    XC_FilmStates filmstatus;
    XC_OpFrameStates opFrameStatus;
    MS_BOOL isOpFrameChanged;
    
    static XC_OpFrameStates prevOpFrameStatus;
    
    XC_Me1HisStates ME1HisStates_1,ME1HisStates_2;
    XC_GMvStates GMV;
//	static MS_U8 u8SDHDDReport = 0;
    static MS_U8 _timeSharingID = 0;
    
    static MS_U8 timingSegment = 0;
    static MS_U8 _hdsddFilterBaseReport;
    static MS_U32 _hpfHdCntReport;
    static MS_U32 _hpfSdCntReport;
       static MS_U32 hdNactCnt;
    static MS_U32 _hdFilterBaseReport;
    static MS_U32 _hpfSdTolerantReport;
    static MS_U32 _hpfHdTolerantReport;
    
    static MS_U32 _hpfHdCntReport3;
    static MS_U32 _hpfSdCntReport3;


//	static MS_BOOL isMEReset;
    MS_BOOL isZoomMoving = false;
    MS_BOOL isBigFeather = false;
    MS_BOOL isSdHqvJaggedTest = false;
    XC_LumaHistogramStates lumaHistogramStates;
    static XC_HpfDefaultRegSetting hpfDefaultReg;
    
    MS_BOOL isSourceChange;
    MS_BOOL isSourceChangeForME = false;
    MS_BOOL isSourceChangeFlag = false;
    
    MS_U32 inSearchUseFeather;
    MS_BOOL isArtifactPattern = false;
//	MS_BOOL isMv0Comfirm = false;
    MS_BOOL isSparsePattern = false;
    
    MS_BOOL isNoLumaCplx = false;
    //DBK Variables
    
    static MS_BOOL sceneChg = false;
    static MS_BOOL sceneChgP1 = false;
    static MS_BOOL sceneChgP2 = false;
    
    
    static long histogram[MaxWidth] = {0};
    static int blocknessCheck[MaxWidth] = {0};
    long FrameBLKStrTmp = 0;
    static int blkConfidence = 0;
    int blkReturnValue = 0;
    int SRAMTIMESLOT = 4;
    int MARKTIMESLOT = 1;
    int ADJTIMESLOT = 3;
    int VSRAMTIMESLOT = 2;
    
    
    int readDBKHistAck = 0;
    int cntForVDBK = 0;
    MS_BOOL DBKFastUpEnable = false;
    MS_BOOL DBKDebugModeOn  = false;
    static MS_U8 cpxGain = 0;
    
    static int cntForDBK = 0; //counter for deblocking
    static long CurFrameEdgStr = 0;
    static long FrameBLKEdgIIR = 0;
    static long CurFrameDetStr = 0;
    static long FrameBLKDetIIR = 0;
    static long CurFrameBLKStr = 0;
    static long FrameBLKStrIIR = 0;
    
    //
    //****** Check VCnt******//
    //
    u8CurrentVcnt = MDrv_SC_read_v_cnt(pInstance);
    if (u8PreviousVcnt == u8CurrentVcnt)
    {
    	return;
    }
    else
    {
    	u8PreviousVcnt = u8CurrentVcnt;
    }
    
    if(MApi_XC_GetStatus(&stXCStatus, MAIN_WINDOW))
    {
	    //MDrv_SC_Edison_ME1_SetStatisticsWindow(pInstance,stXCStatus.stCropWin.width, stXCStatus.stCropWin.height);
	    if(stXCStatus.bInterlace)
	    {
	    	MDrv_SC_Edison_ME1_SetStatisticsWindow(pInstance,stXCStatus.ScaledCropWin.width, (stXCStatus.ScaledCropWin.height)/2);
	    }
	    else
	    {
	    	MDrv_SC_Edison_ME1_SetStatisticsWindow(pInstance,stXCStatus.ScaledCropWin.width, stXCStatus.ScaledCropWin.height);
	    }
    }
	frameWidth  = __minmax(stXCStatus.ScaledCropWin.width,  1, MaxWidth );
	frameHeight = __minmax(stXCStatus.ScaledCropWin.height, 1, MaxHeight);
#if 1
	if (frameWidth >= 3840) //UHD
	{
		SRAMTIMESLOT = 4;
		ADJTIMESLOT  = 4;
		VSRAMTIMESLOT = 3;
	}
	else if (frameWidth >= 1920)	//HD
	{
		SRAMTIMESLOT = 3;
		ADJTIMESLOT  = 3;
		VSRAMTIMESLOT = 2;
	}
	else if (frameWidth <= 800)	//SD
	{
		SRAMTIMESLOT = 1;
		ADJTIMESLOT  = 1;
		VSRAMTIMESLOT = 1;
	}
	else //Unknown source
	{
		SRAMTIMESLOT = 3;
		ADJTIMESLOT  = 3;
		VSRAMTIMESLOT = 2;
	}
	int HDBK_PARTIOVER= SRAMTIMESLOT+9+MARKTIMESLOT+ADJTIMESLOT; //19
	int DBK_PARTIOVER = HDBK_PARTIOVER+SRAMTIMESLOT+VSRAMTIMESLOT*3+2-1; //20~27, no V now

#endif

	//
    //****** Get input resolution info******//
	//
	if( stXCStatus.u16H_SizeAfterPreScaling >= 1280 )
	{
		_bIsHDSource= true;
	}
	else
	{
		_bIsHDSource= false;
	}

	//
	//****** source change******//
	//
	isSourceChange = MDrv_SC_SourceChange(pInstance); //Trace the rising signal for source change
	isSourceChangeForME = (MApi_XC_R2BYTEMSK(REG_SC_BK30_07_L, 0x0002)>>1) ;//hold for 35frames
	isSourceChangeFlag = MDrv_SC_SourceChangeFlag(pInstance,isSourceChange);		//hold for 4 frames

	//
	//****** save HPF original setting******/
	//
	hpfDefaultReg = MDrv_SC_SaveHpfSetting( pInstance,isSourceChangeFlag );

	// Get adaptive function control
	MS_U8 u8Ctrl  = MDrv_SC_get_adaptive_ctrl(pInstance);
	MS_U8 u8Ctrl2 = MDrv_SC_get_adaptive_ctrl2(pInstance);
        MS_U8 u8UFSCCtrl3 = MDrv_UFSC_get_adaptive_ctrl3();
	//MS_U8 u8Ctrl3 = MDrv_SC_get_adaptive_ctrl3();
	//MS_U8 u8Ctrl4 = MDrv_SC_get_adaptive_ctrl4();
	//MS_U8 u8Ctrl5 = MDrv_SC_get_adaptive_ctrl5();

	//
	//****** Get motion value******//
	//
    u32MotionValue = MDrv_SC_read_motion_value1(pInstance);
    u32MotionValue2 = MDrv_SC_read_motion_value2(pInstance);
    //u32MotionValue3 = MDrv_SC_read_motion_value3(pInstance);
    u16MotionValue4 = MDrv_SC_read_motion_value4(pInstance); /* Edison new arrive ( this is no-history motion */
    
    /*MR_OffsetME1 =*/ MDrv_SC_Agate_ME1_UC_InSearchRange_MR_Offset(pInstance, &me1IsMv0, &me1IsNonMv0, &me1IsNotFound );    
    /*MR_OffsetME2 =*/ MDrv_SC_Agate_ME2_UC_InSearchRange_MR_Offset(pInstance, &me2IsMv0, &me2IsNonMv0, &me2IsNotFound );	
    
    u32MotionValue_Sum = u32MotionValue2; //+ ( u16MotionValue4*1 );
    //luma, color, complex
    lumaHistogramStates = MDrv_SC_GetLumaHistogram(pInstance);
    ColorSum = MDrv_SC_A5_ColorPixelSum(pInstance); // #pixels of specified color window
    uComplex = MDrv_SC_Agate_ComplexSum(pInstance);
    //DEBUG_LC_uComplex(uComplex);
    
    //
    //******scene change******//
    //
    isSceneChangeMc = MDrv_SC_Agate_SceneChangeMc( pInstance,me1IsMv0, me1IsNonMv0, me1IsNotFound, me2IsMv0, me2IsNonMv0, me2IsNotFound );
    isSceneChangeY = MDrv_SC_Agate_SceneChangeY( pInstance,lumaHistogramStates );
    isSceneChange = ( isSceneChangeMc | isSceneChangeY );
    
    //Feather
    inSearchUseFeather = MDrv_SC_McInSearchRangeUseFeather(pInstance);
    
    TotalFeather = MDrv_SC_Agate_TotalFeather(pInstance); //clamp at 255
    FeatherSum = MDrv_SC_TotalFeather_Sum(pInstance); //no clamp
    
    //MV
    //nonMv0Comfirm_noIIR = MDrv_SC_Agate_ME1_nonMv0Comfirm_noIIR(pInstance);
    //	mv0Comfirm_noIIR = MDrv_SC_Agate_ME1_mv0Comfirm_noIIR(pInstance);
    nonMv0Comfirm = MDrv_SC_Agate_ME1_nonMv0Comfirm(pInstance);
    mv0Comfirm = MDrv_SC_Agate_ME1_mv0Comfirm(pInstance);
    mvNotFound = MDrv_SC_Agate_ME1_mvNotFound(pInstance);
    //	mvNotFound_noIIR = MDrv_SC_Agate_ME1_mvNotFound_noIIR(pInstance);
    //	mvNotFoundME2 = MDrv_SC_Agate_ME2_mvNotFound(pInstance);
    nonMv0ComfirmME2 = MDrv_SC_Agate_ME2_nonMv0Comfirm(pInstance);
    mv0ComfirmME2 = MDrv_SC_Agate_ME2_mv0Comfirm(pInstance);
    mvNotFoundME2 = MDrv_SC_Agate_ME2_mvNotFound(pInstance);
    
    ME1HisStates_1 = MDrv_SC_Agate_Hierarchy1(pInstance);
    ME1HisStates_2 = MDrv_SC_Agate_Hierarchy2(pInstance,ME1HisStates_1);
    GMV = MDrv_SC_Agate_GMV(pInstance, ME1HisStates_1, ME1HisStates_2 );
    
    //isMv0Comfirm = ( mv0Comfirm_noIIR > 0xA0 ) ? true : false;
    
    Bouncing_status = MDrv_SC_Bouncing_Status(u32MotionValue_Sum);
    bBouncing_Status2 = MDrv_SC_Bouncing_Status2(u16MotionValue4);
    
    //Frame Organization: Film and OPFrame
    
    opFrameStatus = MDrv_SC_OpFrame_Status(pInstance);
    isOpFrameChanged = ( opFrameStatus.curOpFrame != prevOpFrameStatus.curOpFrame );
    
    filmstatus = MDrv_SC_Film_Status(pInstance);
    isMcFilm	= MDrv_SC_McFilmLike(pInstance ,opFrameStatus, _bIsHDSource );
    isMcFilmHd	= MDrv_SC_McFilmLikeHd(pInstance, opFrameStatus, _bIsHDSource );
    //isMcFilmAny	= MDrv_SC_McFilmLikeAny( opFrameStatus, isMcFilm, _bIsHDSource );
    //Time Sharing ID
    _timeSharingID = MDrv_SC_TimeSharing(pInstance, opFrameStatus, &timingSegment, isSourceChangeFlag);
    
    //
    //******DI status******//
    //
    TotalMvComfirm = MDrv_SC_Agate_TotalMvComfirm(pInstance);
    me1Still = MDrv_SC_Agate_ME1Still(pInstance);
    isFeatherAndMotion = MDrv_SC_Agate_FeatherAndMotion(pInstance);
    movingWithoutFeather = MDrv_SC_Agate_MovingWithoutFeather(pInstance);
    //isScenceChangeByNotFound = MDrv_SC_ScenceChangeDetectByNotFound(mvNotFound_noIIR);
    Feather_sum_status= MDrv_SC_TotalFeather_Sum_Status(pInstance, isFeatherAndMotion, ( !_bIsHDSource ), &Feather_sum_status2 );
    
    //for different picture size case
    XC_ApiStatus DrvStatus;
    memset(&DrvStatus, 0x0, sizeof(XC_ApiStatus));
    MApi_XC_GetStatus(&DrvStatus, MAIN_WINDOW);
    if((DrvStatus.stCropWin.width)==0)
    {
        (DrvStatus.stCropWin.width)=1;
    }

     if((DrvStatus.stCropWin.height)==0)
    {
        (DrvStatus.stCropWin.height)=1;
    }

    if((DrvStatus.stCapWin.width)==0)
    {
        (DrvStatus.stCapWin.width)=1;
    }

    if((DrvStatus.stCapWin.height)==0)
    {
        (DrvStatus.stCapWin.height)=1;
    }

    normalize_factor  = ((256 * (DrvStatus.stCropWin.width) * (DrvStatus.stCropWin.height))/( (DrvStatus.stCapWin.width) * (DrvStatus.stCapWin.height)));

	//
	//****** Letter Box LR******//
	//
  	//if (!isSourceChangeFlag)
	//	isLrLetterBox = MDrv_SC_Edison_LrLetterBox( opFrameStatus, hpfDefaultReg, _timeSharingID,  timingSegment );
	XC_LetterBoxState letterBoxState = MDrv_SC_Letter_Box_Driver(pInstance, /*_bIsHDSource,*/ isSceneChange, isSourceChangeFlag, isOpFrameChanged );

	//
	//******DBK status******//
	//
#ifdef DBG_SD2HD_LC
    DEBUG_LC_Full6bit(pInstance,_hdsddFilterBaseReport);
#endif

	//isLowLuma = MDrv_SC_LowLumaDetection();

    DBK_Frame_Estimate( pInstance,cntForDBK, &CurFrameEdgStr, &FrameBLKEdgIIR, &CurFrameDetStr, &FrameBLKDetIIR, &CurFrameBLKStr, &FrameBLKStrIIR, isSceneChange );

	// hdsdd filter base reporter
	// Only DBK Use
	if (!isSourceChangeFlag)
    {
        _hdsddFilterBaseReport = MDrv_SC_SDHD_FilterBaseReport(pInstance, _timeSharingID, &_hpfHdCntReport, &_hpfSdCntReport, hpfDefaultReg, timingSegment  , &hdNactCnt);
        _hdFilterBaseReport = MDrv_SC_dynamicHDfilterBaseReport(pInstance, _timeSharingID, u32MotionValue, isUkosabihsMv0,  hpfDefaultReg, timingSegment  );
        _hpfSdTolerantReport = MDrv_SC_hpfTolerantReport(pInstance, _timeSharingID, hpfDefaultReg, &_hpfHdTolerantReport, timingSegment  );
        MDrv_SC_SDHD_FilterOneReport(pInstance, _timeSharingID, &_hpfHdCntReport3, &_hpfSdCntReport3, hpfDefaultReg, timingSegment  );
    }

#if 0
	MS_U8 _hdsddFilterBaseReportDbg = _hdsddFilterBaseReport / 15;
	if( _hdsddFilterBaseReportDbg == 0 )
		MApi_XC_W2BYTE(REG_SC_BK30_50_L, 0x0000 );
	else if( _hdsddFilterBaseReportDbg == 1 )
		MApi_XC_W2BYTE(REG_SC_BK30_50_L, 0x0002 );
	else if( _hdsddFilterBaseReportDbg == 2 )
		MApi_XC_W2BYTE(REG_SC_BK30_50_L, 0x0022 );
	else if( _hdsddFilterBaseReportDbg == 3 )
		MApi_XC_W2BYTE(REG_SC_BK30_50_L, 0x0222 );
	else
		MApi_XC_W2BYTE(REG_SC_BK30_50_L, 0x2222 );
#endif


	if (u8Ctrl2 & ENABLE_SCALER_AUTO_DBK_DRIVER)  //Edison_DBK_driver 0x01
	{
		//DEBUG_LC_Turn_On(pInstance);
		
		DBK_Crma_Enable(pInstance);									
		DBKDebugModeOn = (MApi_XC_R2BYTEMSK(REG_SC_BK0C_40_L, 0x0080)>0)?1:0;
		DBKFastUpEnable = (MApi_XC_R2BYTEMSK(REG_SC_BK30_0F_L, 0x1000)&& (DBKDebugModeOn == 0 )  );
#ifdef DBG_DBK_LC
		DEBUG_LC_FrameEdg(pInstance,FrameBLKEdgIIR);
		DEBUG_LC_FrameBlk(pInstance,FrameBLKStrIIR);
		DEBUG_LC_FrameDet(pInstance,FrameBLKDetIIR);
#endif
		//				DEBUG_LC_ColorSum(pInstance,ColorSum);

		if(MApi_XC_GetStatus(&stXCStatus, MAIN_WINDOW) == FALSE)
		{
			PRT_TRP("MApi_XC_GetStatus failed because of InitData wrong, please update header file and compile again\n");
		}

		sceneChgP2 = sceneChgP1;
		sceneChgP1 = sceneChg;
		sceneChg   = isSceneChangeY;

		if ( ((sceneChg==1)||(sceneChgP1==1)||(sceneChgP2==1) )&& DBKFastUpEnable && !DBKDebugModeOn)
		{
			//reset
			MApi_XC_W2BYTEMSK(REG_SC_BK0C_39_L, (MS_U16)((0x08<<8)|0x08), 0xFFFF); //width
			//MApi_XC_W2BYTEMSK(REG_SC_BK30_0F_L, 0x8<<4, 0x00F0); //markValue
			//MApi_XC_W2BYTEMSK(REG_SC_BK30_0F_L, 0x4   , 0x000F); //markDefValue
			MApi_XC_W2BYTEMSK(REG_SC_BK0C_47_L, 0x0   , 0x000F); //Cplx gain for black white water flow
			FrameBLKEdgIIR = 60;
			FrameBLKStrIIR = 30;
			FrameBLKDetIIR = 30;

			if (sceneChg == 1)
			{
				resetBKNPart(0x8, 0, blocknessCheck);
				resetBKNPart(0x8, 1, blocknessCheck);
				resetBKNPart(0x8, 2, blocknessCheck);
			}
			else if (sceneChgP1 == 1)
			{
				resetBKNPart(0x8, 0, BlockinessCheckIIR);
				resetBKNPart(0x8, 1, BlockinessCheckIIR);
				resetBKNPart(0x8, 2, BlockinessCheckIIR);
			}
			else if (sceneChgP2 == 1)
			{
				resetBKNPart(0x8, 0, BlockinessCheckPre);
				resetBKNPart(0x8, 1, BlockinessCheckPre);
				resetBKNPart(0x8, 2, BlockinessCheckPre);
				cntForDBK = DBK_PARTIOVER;
			}
			else
			{
				//DRVSCA_DBG(PRT_TRP("===========ghost hitting wall===========\n"));
			}

		}
		else
		{
			FrameBLKStrTmp = parameterControl(pInstance,FrameBLKEdgIIR, FrameBLKStrIIR, FrameBLKDetIIR, cpxGain, _hpfHdCntReport3, _hpfSdCntReport3);

			if ( (cntForDBK >= 0) && (cntForDBK < SRAMTIMESLOT ) )	// Read Histogram
			//if ( (cntForDBK == 0) || (cntForDBK == 1) || (cntForDBK == 2) )	// Read Histogram
			{
				MApi_XC_W2BYTEMSK(REG_SC_BK60_35_L, 0x3FFF, 0x3FFF);
				MApi_XC_W2BYTEMSK(REG_SC_BK60_3D_L, 0x3FFF, 0x3FFF);

				readDBKHistAck = readHistogramPart(pInstance,histogram, cntForDBK, SRAMTIMESLOT);

				if( (cntForDBK == SRAMTIMESLOT - 1 )&& readDBKHistAck)
				//if( (cntForDBK == 2 )&& readDBKHistAck)
				{
					MApi_XC_W2BYTEMSK(REG_SC_BK0C_0D_L, 1, 0x0001);
				}
				if (readDBKHistAck)
					cntForDBK++;
			}
			else if ( (cntForDBK >= SRAMTIMESLOT) && (cntForDBK < HDBK_PARTIOVER) )
			//else if ( (cntForDBK > 2) && (cntForDBK < HDBK_PARTIOVER) )
			{
				cpxGain = BWPatchDBK(pInstance,lumaHistogramStates, ColorSum, uComplex, me1Still, mvNotFound, /*nonMv0Comfirm,*/ mv0Comfirm);

				int cntForHDBK = 0;
				cntForHDBK = cntForDBK - SRAMTIMESLOT;
				blkReturnValue = DBK_Detect_Partition(pInstance, blocknessCheck, histogram, &cntForHDBK, _hdsddFilterBaseReport, isSceneChange, FrameBLKStrTmp, MARKTIMESLOT, ADJTIMESLOT);
				cntForDBK = cntForHDBK + SRAMTIMESLOT;
				if ( blkReturnValue != -1)
				{
					blkConfidence = blkReturnValue;

				}
				cntForDBK++;
			}
			else if ((cntForDBK >= HDBK_PARTIOVER) && (cntForDBK < HDBK_PARTIOVER+SRAMTIMESLOT) )
			//else if ((cntForDBK == HDBK_PARTIOVER) || (cntForDBK == HDBK_PARTIOVER+1) ||(cntForDBK == HDBK_PARTIOVER+2))
			{
				//WRITE TO BLOCKNESS POSITION SRAM
				writeToSRAMPart(pInstance,blocknessCheck, (cntForDBK-HDBK_PARTIOVER), SRAMTIMESLOT);
				cntForDBK++;
			}
			else if (cntForDBK >= HDBK_PARTIOVER+SRAMTIMESLOT )
			{
				cntForVDBK = cntForDBK - (HDBK_PARTIOVER+SRAMTIMESLOT);
				VDBK_Detect( pInstance,blkConfidence, &cntForVDBK, VSRAMTIMESLOT );
				cntForDBK = cntForVDBK + (HDBK_PARTIOVER+SRAMTIMESLOT);
				if (cntForDBK >= DBK_PARTIOVER)
					cntForDBK = 0;
				else
					cntForDBK++;
			}
			else
				cntForDBK = 0;

		}

        if(u8Ctrl2 & ENABLE_SCALER_AUTO_DBK_NMR_DRIVER)
        {
		NoiseMaskingControl(FrameBLKStrTmp,isSceneChange);
        }
	}


	if(u8Ctrl2 & ENABLE_SCALER_MCDI_DRIVER) //Edison_UCDi_driver 0x02
	{

		MDrv_SC_KFC_EODiW_Motionless(pInstance); /*2012-07-26 Author:sam.hung*/
#if HQV_JAGGY_PATTERN_ENBLE
        if(!_bIsHDSource)   //SD case
        isSdHqvJaggedTest = MDrv_SC_HQV_JaggedTestPattern( pInstance,GMV );
	//printf("HQV_JAGGY_PATTERN_ENBLE == %d\n\n",HQV_JAGGY_PATTERN_ENBLE);
#endif
	MDrv_SC_Agate_SubMvPanScan( pInstance,GMV, isSceneChangeMc, isSdHqvJaggedTest, u16MotionValue4 );
	MDrv_SC_EODi_Armored( pInstance,GMV, mv0Comfirm , nonMv0Comfirm);

	//isMcFilmSlowMotion = _bIsHDSource ? MDrv_SC_McFilmLikeForSlowMotion( u32MotionValue2, GMV ) : false;
    isSparsePattern = MDrv_SC_ukoSabihS_SparsePattern(pInstance, mvNotFound, mvNotFoundME2, mv0Comfirm, nonMv0Comfirm, mv0ComfirmME2, nonMv0ComfirmME2, _bIsHDSource); /*FOR VG846 sparse content pattern*/

#if SHIBASOKU_ENABLE
    isHBurstMoving = MDrv_SC_ukoSabihS_HBurstMoving(pInstance, _hpfSdTolerantReport/*_hpfSdCntReport*/ );
    isHBurst = MDrv_SC_ukoSabihS_HBurst(pInstance, _hpfSdTolerantReport/*_hpfSdCntReport*/, uComplex );
    ukoSabihS_en = MDrv_SC_Agate_ukoSabihS(pInstance,&stXCStatus, GMV,nonMv0Comfirm, filmstatus, isMcFilmukoSabihS, &isUkosabihsMotion, &isUkosabihsMc, &isUkosabihsForFilm, mv0Comfirm, &isUkosabihsMv0, _bIsHDSource);
    isUkosabihsMcSweep = MDrv_SC_sweep_ukoSabihS( pInstance, GMV,	nonMv0Comfirm, mv0Comfirm, filmstatus, isMcFilm, uComplex, &isUkosabihsMotionSweep );
    isUkosabihsMc = ( isUkosabihsMc & (!isBigFeather) );  /*isBigFeather for Ukosabihs scence change */
    isBigFeather = (isFeatherAndMotion > 0x100)? true	:	false;
    isArtifactPattern = MDrv_SC_ukoSabihS_ArtifactPattern(pInstance,_hpfHdTolerantReport,_bIsHDSource);
    isNoLumaCplx =  MDrv_SC_ukoSabihS_noLumaCplx(pInstance); //=>no chroma
    isMcFilmukoSabihS	= MDrv_SC_McFilmUkoSabihS(pInstance, opFrameStatus, _bIsHDSource, _hpfHdCntReport );
#else
    isBigFeather = isBigFeather;
    isMcFilmukoSabihS = isMcFilmukoSabihS;
    isUkosabihsForFilm = isUkosabihsForFilm;
#endif


    isMcOutOfSearchRange = MDrv_SC_McOutOfSearchRange( pInstance,mvNotFound, mv0Comfirm, nonMv0Comfirm, isFeatherAndMotion, GMV );

    if((_bIsHDSource)&& KURO_PATTERN_ENABLE == 1)   //HD case
    {
        isMcFilmSlowMotion  = MDrv_SC_McFilmLikeForSlowMotion( pInstance,u32MotionValue2, GMV ); /*fot Kuro increase SST */
        //printf("McFilmLikeForSlowMotion ==1\n\n");
    }
    else
    {
	isMcFilmSlowMotion = false;
        //printf("McFilmLikeForSlowMotion ==0\n\n");
    }

//isZoomMoving = _bIsHDSource ? false : MDrv_SC_ZoomMoving( GMV, me1IsNonMv0, me1IsNotFound, isFeatherAndMotion, u16MotionValue4 );
    if(!_bIsHDSource)   //SD case for S Tree and Flower
    {
        isZoomMoving = MDrv_SC_ZoomMoving(pInstance, GMV, me1IsNonMv0, me1IsNotFound, isFeatherAndMotion, u16MotionValue4 );
    }
    else
    {
        isZoomMoving = false;
    }

#if LG_SPECIFIC_PATTERN_ENABLE
    if(!_bIsHDSource)   //SD case
    {
			isL139 = MDrv_SC_Agate_L139(pInstance, TotalMvComfirm, ColorSum, uComplex, me1Still, ME1HisStates_2 );
			isL153 = MDrv_SC_Agate_L153(pInstance, movingWithoutFeather, ColorSum, uComplex, ME1HisStates_2 );
			isL107 = MDrv_SC_Agate_L107(pInstance, TotalMvComfirm, uComplex, GMV, u32MotionValue, TotalFeather, 2/*HDMI only*/ );
			//isSWoman = MDrv_SC_SBlueShirt(pInstance,nonMv0Comfirm, mv0Comfirm, mvNotFound, mvNotFoundME2, uComplex, ME1HisStates_2, GMV );
        //isWeakMcNrFilm = filmstatus.film_act;
	isAbihsot = false;
    }
    else    //HD case
    {
        isL139 = false;
        isL153 = false;
        isL107 = false;
        //isSWoman = false;
        //isWeakMcNrFilm = false;
        isAbihsot = false;
    }
#else
    movingWithoutFeather = movingWithoutFeather;
    TotalMvComfirm = TotalMvComfirm;
    TotalFeather = TotalFeather;
#endif

//Double Control
		MDrv_SC_KFC_VerticalMovingUdRatio_Ctrl(pInstance, inSearchUseFeather, isMcFilm );
		MDrv_SC_KFC_8bitMotionGain_Ctrl(pInstance, ( (isZoomMoving) & (!isSceneChangeMc)) , isMcFilm, isMcFilmSlowMotion );

		MDrv_SC_featherSstGain_Ctrl(pInstance, isMcFilmSlowMotion ); /* for kuro propeller.. */
		MDrv_SC_featherSstOffset_Ctrl(pInstance, opFrameStatus, isMcFilmSlowMotion, ( (isZoomMoving) & (!isSceneChangeMc) )); /* for kuro propeller.. */

		MDrv_SC_MotionLimitGain_Ctrl(pInstance, isL153 );
      MDrv_SC_UCDi_Weight_Boundary_Ctrl(pInstance, ME1HisStates_1 ); /*patch for Boundary flicker issue when vertical scroll*/
		MDrv_SC_MCDi_En_Ctrl(pInstance, ( isNoLumaCplx | isMcFilm | isMcOutOfSearchRange | isArtifactPattern | Bouncing_status | isHBurstMoving | isSceneChangeMc | isMcFilmSlowMotion ) );

    MDrv_SC_Force_Motion_Ctrl( pInstance,isL139, ( isUkosabihsMotion | isUkosabihsMotionSweep ) );
    MDrv_SC_Force_Mcdi_Ctrl(pInstance,isUkosabihsMc);
    MDrv_SC_KFC_EODiW_Ctrl( pInstance,( isUkosabihsMc | isUkosabihsMcSweep ) );
    MDrv_SC_Mcdi_EODiW_Ctrl( pInstance,( isUkosabihsMc | isUkosabihsMcSweep ), isL107 );
    MDrv_SC_KFC_Ctrl( pInstance,isAbihsot );
    MDrv_SC_UCDi_OP1_HisWeight_Ctrl(pInstance,Bouncing_status,Feather_sum_status,filmstatus, ColorSum, FeatherSum, _bIsHDSource);
    MDrv_SC_SPF_OP1_HisWeight_Ctrl(pInstance,Bouncing_status);
    MDrv_SC_SST_Ctrl(pInstance,( isNoLumaCplx | isMcFilm | isMcOutOfSearchRange | isArtifactPattern | bBouncing_Status2 | isHBurstMoving | isSceneChangeMc | isMcFilmSlowMotion ), FeatherSum, _bIsHDSource);

}
    /*isMEReset =*/ MDrv_SC_Agate_ME_Reset(pInstance,u32MotionValue2,isSceneChangeMc,isSceneChangeY, isSourceChangeForME);

    if(u8Ctrl2 & ENABLE_SCALER_MCNR_DRIVER)  //Edison_UCNR_driver 0x04
    {
        NE_confirm = MDrv_SC_GetNoiseLevel(pInstance, &estNoise, &letterBoxState, &stXCStatus, stXCStatus.bInterlace, isOpFrameChanged );
        MDrv_SC_NR_Driver_E28( pInstance,u16MotionValue4, /*blkConfidence,*/ /*_hpfHdTolerantReport,*/ &lumaHistogramStates, &filmstatus, &GMV, isUkosabihsMotion, isSceneChange, stXCStatus.bInterlace, _bIsHDSource, isSourceChangeFlag, isOpFrameChanged ,&estNoise, NE_confirm);
#ifdef DBG_SCCHG_LC
	DEBUG_LC_42( pInstance,isSceneChangeY, isSceneChangeMc );
#endif
    }

    if(u8Ctrl2 & ENABLE_SCALER_SDHD_DETECT_DRIVER)  //Edison_SDHD_driver 0x08
    {
        MDrv_SC_Edision_stillFeatherDetection( pInstance,u32MotionValue, isUkosabihsMotion, _hdFilterBaseReport, isHBurstMoving, isHBurst, isUkosabihsMv0, isMcFilmHd/*, isSceneChange*/, _hpfSdTolerantReport );

        static int DMS_SD2HD_Cnt = 0;
	// DMS SDtoHD mode
        MS_BOOL GMV_StillLike = (_bIsHDSource) && (GMV.minMvXdir >= 31) && (GMV.minMvXdir <= 33) && (GMV.minMvYdir >= 7) && (GMV.minMvYdir <= 9) ;
        MS_BOOL SD2HDCntLike1  = ( (_hpfHdCntReport>>7) > 2) && ( (_hpfHdCntReport>>7) < 8);
        MS_BOOL SD2HDCntLike2  = ( (_hpfSdCntReport>>3) > (_hpfHdCntReport*6) );
        MS_BOOL DBKLike = (FrameBLKEdgIIR > 31) && (FrameBLKEdgIIR < 80);
	//DEBUG_LC_Full6bit3(__minmax((_hpfHdCntReport>>7), 0, 63) );
	//DEBUG_LC_Full6bit4(__minmax((FrameBLKEdgIIR>>1), 0, 63) );
#if 0
		if ( GMV_StillLike )
			MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x7000, 0x7000);
		else
			MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0000, 0x7000);

		if ( SD2HDCntLike1 )
			MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0700, 0x0700);
		else
			MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0000, 0x0700);

		if ( SD2HDCntLike2 )
			MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0070, 0x0070);
		else
			MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0000, 0x0070);

		if ( DBKLike )
			MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0007, 0x0007);
		else
			MApi_XC_W2BYTEMSK(REG_SC_BK30_40_L, 0x0000, 0x0007);
#endif
        if (isSceneChange || isSourceChangeFlag)
        {
            DMS_SD2HD_Cnt = 0;
        }
        else if ( GMV_StillLike && SD2HDCntLike1 && SD2HDCntLike2 && DBKLike )
        {
            DMS_SD2HD_Cnt += 4;
        }
        else
        {
            if (!GMV_StillLike)
                DMS_SD2HD_Cnt -= 1;
            if (!SD2HDCntLike1)
                DMS_SD2HD_Cnt -= 1;
            if (!SD2HDCntLike2)
                DMS_SD2HD_Cnt -= 1;
            if (!DBKLike)
                DMS_SD2HD_Cnt -= 1;
        }
            DMS_SD2HD_Cnt = __minmax(DMS_SD2HD_Cnt, 0, 63);
            DMS_SDHDModeCheck(pInstance,DMS_SD2HD_Cnt);
        }


    if(u8Ctrl2 & ENABLE_SCALER_CCS_DRIVER)  //Edison_CCS_driver 0x20
    {
    	//MDrv_SC_Edison_CCS_driver(ukoSabihS_en);
    	MDrv_SC_Edison_CCS_driver(pInstance,ukoSabihS_en|isSparsePattern); //Add isSparsePattern for TG59 Variable Dot pattern(Pure Color pattern)
    }
    
    if(u8Ctrl2 & ENABLE_SCALER_DHD_DRIVER)  //Edison_DHD_driver 0x40
    {
        //		u8SDHDDReport = false;
        MDrv_SC_Edison_DHD_driver(pInstance,uComplex, nonMv0Comfirm, me1Still, mv0Comfirm, GMV, _hpfHdCntReport);
        MDrv_SC_Edison_DHD_Weather_driver(pInstance,uComplex, nonMv0Comfirm, me1Still, mv0Comfirm );
        MDrv_SC_Edison_DHD_GSLSM_driver(pInstance,uComplex, nonMv0Comfirm, me1Still, mv0Comfirm );
        MDrv_SC_DHD_CCS_MR_HisWeight_Ctrl(filmstatus);
        //MDrv_SC_Edison_DHD_Stadium_driver(pInstance,isMcFilmHd,filmstatus);
    }
    
    if(u8Ctrl & ENABLE_SCALER_D3D_DRIVER)  //Edison_Detect_3D_Source_driver
    {
    	 MDrv_SC_3DfmtDetect(pInstance); //2012.7.24 mark
    }
    
    if (u8Ctrl & ENABLE_SCALER_DEFEATHERING)
    {
    	MDrv_SC_de_feathering(pInstance,u32MotionValue);
    }
    
    if (u8Ctrl & ENABLE_SCALER_DEFLICKERING)
    {
    	MDrv_SC_de_flickering(pInstance,u32MotionValue);
    }
    
    if (u8Ctrl & ENABLE_SCALER_DEBOUNCING)
    {
    	MDrv_SC_de_bouncing(pInstance,u32MotionValue2);
    }
    
    if( (MApi_XC_R2BYTEMSK(REG_SC_BK0A_10_L, BIT(14)) == BIT(14))
            || (MApi_XC_R2BYTEMSK(REG_SC_BK0A_10_L, BIT(15)) == BIT(15)) )
    {
        if(!_Hal_PQ_FrameLockCheck(pInstance)) // Make not frame LOCKED then turn off FilmMode
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, BIT(1), BIT(1)); //hardware film mode DISABLE
            MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, BIT(1)|BIT(2), BIT(1) | BIT(2)); //software film mode DISABLE
        }
        else    // Make sure frame LOCKED then turn on FilmMode
        {
            if(bFakeOutCusEnable)
            {
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, 0, BIT(1)); //hardware film mode ENABLE
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, 0, BIT(1) | BIT(2)); //software film mode ENABLE
            }
            else
            {
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_1E_L, BIT(1), BIT(1)); //hardware film mode DISABLE
                MApi_XC_W2BYTEMSK(REG_SC_BK0A_21_L, BIT(1)|BIT(2), BIT(1) | BIT(2)); //software film mode DISABLE
            }
        }
    }
//
//******UFSC DRIVER******//
//
    MS_U16 FSC_estNoise=0;
    MS_U8 FSC_estConfirm=0;

    //NE transfer from SOC GetNoiseLevel
    FSC_estNoise = __minmax(estNoise/10, 0, 15);
    FSC_estConfirm = __minmax( NE_confirm, 1, 8);

    // SR enhancement strength
    int srEnhStr = (MDrv_UFSC_Read2Byte( REG_FSC_BK30_3E_L)& 0x003F ); // 1.5
    static XC_BackgroundState backgroundState;
    MDrv_UFSC_PQ_BNF_status( &backgroundState, isOpFrameChanged );

    // OSD info
    MS_BOOL isOSDActive = MDrv_UFSC_Read2Byte(REG_FSC_BK132_4A_L)>>11;
    static MS_U8 osdW = 0;
    if( isOSDActive )
    {
        osdW = osdW < 253 ? osdW + 2 : 255;
    }
    else
    {
        osdW = osdW > 16 ? osdW - 16 : 0;
    }

    static int dsStdCnt = 0, frNNCnt = 0, frNNCnt2 = 0;
    static int frDsSum = 0, frFetSum = 0, frNNSum = 0;
    {
        int dsStdCntTmp = MDrv_UFSC_Read2Byte(REG_FSC_BK2C_79_L) + MDrv_UFSC_Read2Byte(REG_FSC_BKAC_79_L);
        int frNNCntTmp = MDrv_UFSC_Read2Byte(REG_FSC_BK72_31_L) + MDrv_UFSC_Read2Byte(REG_FSC_BKF2_31_L);
        int frNNCnt2Tmp = MDrv_UFSC_Read2Byte(REG_FSC_BK72_33_L) + MDrv_UFSC_Read2Byte(REG_FSC_BKF2_33_L); // not changed with settings

        int frDsSumTmp = ( (MDrv_UFSC_Read2Byte(REG_FSC_BK72_7B_L) << 16) + MDrv_UFSC_Read2Byte(REG_FSC_BK72_7A_L)
                    + (MDrv_UFSC_Read2Byte(REG_FSC_BKF2_7B_L) << 16) + MDrv_UFSC_Read2Byte(REG_FSC_BKF2_7A_L) ) >> 1;
        int frFetSumTmp = ( (MDrv_UFSC_Read2Byte(REG_FSC_BK72_7D_L) << 16) + MDrv_UFSC_Read2Byte(REG_FSC_BK72_7C_L)
                    + (MDrv_UFSC_Read2Byte(REG_FSC_BKF2_7D_L) << 16) + MDrv_UFSC_Read2Byte(REG_FSC_BKF2_7C_L) ) >> 1;
        int frNNSumTmp = ( (MDrv_UFSC_Read2Byte(REG_FSC_BK72_7F_L) << 16) + MDrv_UFSC_Read2Byte(REG_FSC_BK72_7E_L)
                    + (MDrv_UFSC_Read2Byte(REG_FSC_BKF2_7F_L) << 16) + MDrv_UFSC_Read2Byte(REG_FSC_BKF2_7E_L) ) >> 1;

        dsStdCnt = ( dsStdCntTmp + dsStdCnt*3 ) >> 2;
        frNNCnt = ( frNNCntTmp + frNNCnt*3 ) >> 2;
        frNNCnt2 = ( frNNCnt2Tmp + frNNCnt2*3 ) >> 2;
        frDsSum = ( frDsSumTmp + frDsSum*3 ) >> 2;
        frFetSum = ( frFetSumTmp + frFetSum*3 ) >> 2;
        frNNSum = ( frNNSumTmp + frNNSum*3 ) >> 2;
    }

#if 1
    int menuW, edgePatW; // for menu and dragon ball like edge pattern
    {
        // menu pattern
    menuW = __max( 0, (int)backgroundState.cplxRawCnt - ((int)backgroundState.dswRawCnt * 3) );
    menuW = __min( 256, menuW );
    menuW = 256 - menuW;

    int cplxW = __max( 0 , (int)backgroundState.cplxRawCnt - 4096 ) >> 4;
    menuW = __max( 0, menuW - cplxW );

    int fetDsW = __max( 0, frFetSum - frDsSum ) >> 2;
    menuW = __max( 0, menuW - fetDsW );

    int fetW = __max( 0, frFetSum - 512 ) >> 2;
    menuW = __max( 0, menuW - fetW );

    // OSD
    menuW = __max( menuW, osdW );
    }
    {
    // edge pattern : dragon ball, AUO_0011,12
    edgePatW = (__max( 0, (int)backgroundState.cplxRawCnt - ( ((int)backgroundState.dswRawCnt * 19) >> 3 )))>>2;
    edgePatW = __min( 256, edgePatW );
    edgePatW = 256 - edgePatW;

    int dsFetW = __max( 0, frDsSum - frFetSum );
    dsFetW = __min( 256, dsFetW );
    int stdW = __max( 0, dsStdCnt - 1024 ) >> 2;
    dsFetW = __max( 0, dsFetW - stdW );
    edgePatW = __max( 0, edgePatW - dsFetW );

    //int flatW = max( 0 , 4096 - (int)backgroundState.cplxRawCnt ) >> 1;
    //edgePatW = max( 0, edgePatW - flatW );
    }
    int fHdW;
    {
        fHdW = __max( 0,  65536 - (int)_hpfHdCntReport ) >> 6;
    //fHdW = max( 0,  16384 - (int)hpfHdCntReport ) >> 4;
        fHdW = __min( 256, fHdW );
    }

    int sdW;
    {
        sdW = __max( 0, 2048 - (int)_hpfHdCntReport ) >> 2;
        sdW = __min( 256, sdW );
        sdW = __max( 0 , sdW - __max( edgePatW, menuW )/*edgePatW*/ );

        int hdClampW = __max( 0, (int)hdNactCnt - (int)65536 ) >> 3;
        sdW = __max( 0 , sdW - hdClampW );

    //printf( "SDW : %d\n", sdW );
    }
#endif

    if (u8UFSCCtrl3 & ENABLE_SR_DRIVER)
    {
        MDrv_UFSC_PQ_LSE( frNNCnt2, menuW, edgePatW, sdW, srEnhStr );
        MDrv_UFSC_PQ_NLM( &backgroundState, menuW, edgePatW, srEnhStr );
        MDrv_UFSC_PQ_PreEnh( fHdW, sdW, FSC_estNoise, FSC_estConfirm, srEnhStr );
        MDrv_UFSC_PQ_PostPK3x3( menuW, edgePatW, fHdW, sdW, srEnhStr );
        MDrv_UFSC_PQ_HASC( sdW, srEnhStr );
        MDrv_UFSC_PQ_P2M( menuW, edgePatW, srEnhStr );
        MDrv_UFSC_PQ_ANN( frNNCnt2 );
        MDrv_UFSC_PQ_DMS_referBNF( &backgroundState );
        MDrv_UFSC_PQ_LSE_Patch();
    }

	MDrv_SC_ResetSourceChange( pInstance,isSourceChangeForME );

	prevOpFrameStatus = opFrameStatus;
}

#undef DRV_SCALER_ADAPTIVE_C

