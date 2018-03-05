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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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

#ifndef __DRV_MFC_ST_H__
#define __DRV_MFC_ST_H__


//------------------------------------------------------------------------------
// Definition
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Structure
//------------------------------------------------------------------------------
typedef struct
{
    U16 u16Addr;
    U16 u16Mask;
    U16 u16Data;
} MFC_REG_DATA;

typedef struct IO_MFC_PANEL_INFO_s
{
    U32* pPanelInfo;
    U16  u16Len;
} IO_MFC_PANEL_INFO_t, *PIO_MFC_PANEL_INFO_t;

typedef struct MST_MFC_RegUnitType_s
{
    S16 ucIndex;
    S16 ucValue;
}MST_MFC_RegUnitType_t;

typedef struct MST_MFC_SYS_INFO_s
{
    //titania to URSA
	U8  u8LVDSChannel; 		//Single, Dual
    U8  u8LVDSBitNum;  		//8bits, 10 bits
    U8  u8LVDSTiMode;  		//Thin/Ti mode scaler 40-bit2
    U8  u8LVDSSwapMsbLsb; 	//ursa scaler
    U8  u8LVDSSwap_P_N; 		//ursa scaler
    U8  u8LVDSSwapOddEven; 	//ursa scaler

	//URSA to Panel info
	U8	u8PanelType;  			//TTL, Mini_LVDS, LVDS
	U8  u8PanelBitNum; 			//Panel bit number
	U8  u8PanelChannel; 		//Single, Dual, Quad, Quad_LR
	U8	u8PanelDither;
	U8	u8PanelLVDSTiMode; 		//Panel TI/Thin mode
	U8	u8PanelLVDSSwapPol; 	//Panel LVDS polarity swap
	U8	u8PanelLVDSSwapCH; 		//LVDS chenel swap ABCD
	U8	u8PanelLVDSSwapPair; 	//Panel LVDS pair swap
	U8	u8PanelLVDSShiftPair;
	U8	u8PanelBlankCPVC;			//Panel Mini LVDS use
	U8	u8PanelBlankOEC;			//Panel Mini LVDS use
	U8	u8PanelBlankTPC;			//Panel Mini LVDS use
	U8	u8PanelBlankSTHC;			//Panel Mini LVDS use

	U16 u16HStart;				//ursa scaler
    U16 u16VStart; 				//ursa scaler
    U16 u16Width; 				//ursa scaler
    U16 u16Height; 				//ursa scaler
    U16 u16HTotal; 				//ursa scaler
    U16 u16VTotal; 				//ursa scaler
    U8  u8PanelVfreq; 			//Panel frame rate 60Hz, 120Hz, 240Hz
	U8	u8PanelIncVtotalFor50Hz;	//Change Vtotal for DCLK
	U8	u8PanelCSC;					//LVDS CSC enable/disable
	U16	u16MFCMemoryClk;			//MFC memory clock MHz
	U16	u16MFCMemoryType;			//MFC memory type
	U8	u8PanelGAMMA;
	U8  u8ODMode;
	U8	u8IPMode;
	U8	u8Preset;
	U8	u8MirrorMode;
	U8  u8UseMPIF;
    U8  u8ChipRevision;
} MST_MFC_SYS_INFO_t, *PMST_MFC_SYS_INFO_t;

typedef struct MST_MFC_STATUS_s
{
// REG_29CC
    U8 GMVX;
// REG_29CD
    U8 GMVY        : 7;
    U8 GMVConfirm    : 2;
// REG_29CE
    U8 FrameEnd     : 1;
    U8 Skip_Thd     : 2;
// REG_29CF
    U8 Film22Mode : 1;
    U8 Film32Mode : 1;
    U8 Film22Idx     : 1;
    U8 Film32Idx     : 3;
    U8 FDUP         : 1;

    U8 MaxSAD_MSB;
    U8 MinSAD_MSB;
    U8 maxXNegSpeed; //0x29c2;
    U8 maxXPosSpeed; //0x29c2;
    U8 maxYNegSpeed; //0x29c2;
    U8 maxYPosSpeed; //0x29c2;

// REG_29E0
    U16 ErrorBlk1;
    U16 ErrorBlk2;
    U16 ErrorBlk3;
    U16 ErrorBlk4;
    U16 ErrorBlk5;
    U16 ErrorBlk6;

    U16 GMV0Err;
    U16 MINonContinuousBoundary;
    U16 gmvBlkCnt;
    U16 mv0BlkCnt;
    U16 cplxBlkCnt2;
    U16 MinSAD;
    U16 MovingBlkCnt;
    U16 cplxBlkCnt;
    U16 veryCplxBlkCnt;
    U16 unMatchPointCnt;
}MST_MFC_STATUS_t, *PMST_MFC_STATUS_t;

typedef struct MST_MFC_ADJ_s
{
	U8 u8BlurAdj;
	U8 u8JudderAdj;
	U8 u8MFCAdj;
}MST_MFC_ADJ_t, *PMST_MFC_ADJ_t;


typedef struct MST_MFC_VIDEOBLOCK_s
{
	U8 u8Type;
	BOOL bOnOff;
}MST_MFC_VIDEOBLOCK_t, *PMST_MFC_VIDEOBLOCK_t;

typedef struct MST_MFC_BYPASSWINDOW_s
{
	BOOL	bOnOff;
	U8		u8WindowID;
	U16		u16HStart;
	U16		u16VStart;
	U16		u16HSize;
	U16		u16VSize;
}MST_MFC_BYPASSWINDOW_t, *PMST_MFC_BYPASSWINDOW_t;

typedef struct MST_MFC_DEMO_BAR_s
{
	BOOL bEnable;
	BOOL bDirection;
	U8 u8width;
	U8 u8color;
}MST_MFC_DEMO_BAR_t, *PMST_MFC_DEMO_BAR_t;

typedef struct MST_MFC_SSC_s
{
	U16 u16KHz;
	U16 u16Percent;
	BOOL bEnable;
	BOOL bMiuLvds;
}MST_MFC_SSC_t, *PMST_MFC_SSC_t;

typedef struct MST_MFC_HVTOTAL_s
{
	U16 u16HTotal;
	U16 u16VTotal;
}MST_MFC_HVTOTAL_t, *PMST_MFC_HVTOTAL_t;

#endif//__DRV_MFC_ST_H__

