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
// Copyright (c) 2006-2010 MStar Semiconductor, Inc.
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvMfc.c
/// @brief  Mfc Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MDRV_MFC_H_
#define _MDRV_MFC_H_

//#include <linux/init.h>
//#include <linux/module.h>
//#include <linux/fs.h>
//#include <linux/cdev.h>
//#include <asm-mips/delay.h>
#include "mdrv_mfc_platform.h"
#include "mdrv_mfc_st.h"

#if(CODEBASE_SEL != CODEBASE_UTOPIA)
#include "mst_platform.h"
#include "mdrv_types.h"
#endif
//#include "mdrv_mfc_io.h"

/*
#define absGmvX(a)            (diff(a, 0x80))
#define absGmvY(a)            (diff(a, 0x40))
#define Posdiff(a,b)           (((a) > (b)) ? (a-b):0)
#define isMV0 ((gMfcType.GMVX == 0x80) && (gMfcType.GMVY == 0x40))
#define isMVNotFound ((gMfcType.GMVX == 0) && (gMfcType.GMVY == 0))
#define DIF_MAX_POS_SPEED_X (gMfcType.maxXPosSpeed-gMfcType.maxXNegSpeed)
#define DIF_MAX_POS_SPEED_Y (gMfcType.maxYPosSpeed-gMfcType.maxYNegSpeed)
#define MaxSpeedDifToGMV max((gMfcType.maxXPosSpeed-gMfcType.GMVX)>>3,(gMfcType.GMVX-gMfcType.maxXNegSpeed)>>3)
#define MinSpeedDifToGMV min((gMfcType.maxXPosSpeed-gMfcType.GMVX)>>3,(gMfcType.GMVX-gMfcType.maxXNegSpeed)>>3)
*/

//--MFC OP gain phase setting
#define _RATIO			1 // 905,969,664
#define _STEP			4 // 4,294,967,295
#define _GAIN_P(f)		(f/(_STEP))
#define _GAIN_I(f)		(f/(_STEP*_STEP/2))

//------------------------------------------------------------------------------
//  Enum
//------------------------------------------------------------------------------
typedef enum
{
    MFC_I2cUseBusN       = 0x34,
    MFC_I2cUseBus        = 0x35,
    MFC_CpuWaitN         = 0x36,
    MFC_CpuWait          = 0x37,
    MFC_SerialDebugEnter = 0x44,
    MFC_SerialDebugExit  = 0x45,
} MFC_SERIAL_DEBUG_CMD_e;
/*
typedef enum
{
    MIRROR_OFF,
    MIRROR_H_MODE,
    MIRROR_V_MODE,
    MIRROR_HV_MODE
} MFC_MirrorModeType;
*/
/*
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
} MST_MFC_SYS_INFO_t, *PMST_MFC_SYS_INFO_t;

*/
//------------------------------------------------------------------------------
// Global variable
//------------------------------------------------------------------------------
extern MST_MFC_SYS_INFO_t gmfcSysInfo;
//------------------------------------------------------------------------------
// Function prototype
//------------------------------------------------------------------------------
void MDrv_MFC_Init(PMST_MFC_SYS_INFO_t mfcSysInfo);
void MDrv_MFC_Reset(void);
void MDrv_MFC_COMPENSATION(U8 u8Blur, U8 u8Judder, U8 u8MFC);
void MDrv_MFC_TrueMotionDemo(U8 u8TrueMotionDemo);
void MDrv_MFC_VideoBlock(U8 u8Type , BOOL bOnOff);
void MDrv_MFC_Set_Bypass_Window(BOOL bOnOff, U8 u8WindowID, U16 u16HStart, U16 u16VStart, U16 u16HSize, U16 u16VSize);
U16 MDrv_MFC_GetSWVersion(void);
U16 MDrv_MFC_GetBinVersion(void);
void MDrv_MFC_UpdateSW(void);
void MDrv_MFC_DemoBarControl(BOOL bEnable, BOOL bDirection, U8 u8width, U8 u8color);
void MDrv_MFC_OnOff(BOOL bOnOff);
void MDrv_MFC_LVDSPowerOnOFF(BOOL bOnOff);
void MDrv_MFC_SetFrameRate(U8 u8PanelVfreq);
U8 MDrv_MFC_IsStable(void);
void MDrv_MFC_SetSSC(U16 u16KHz, U16 u16Percent, BOOL bEnable, BOOL bMiuLVDS);
void MDrv_MFC_SetReverseMode(U8 u8MirrorMode);
void MDrv_MFC_SetLVDSBit(U8 u8BitNum);
void MDrv_MFC_SetLVDSVesaJeida(BOOL bVesaJeida);
void MDrv_MFC_DebugBlock(BOOL bOnOff);
void MDrv_MFC_GetHVTotal(U16* u16HTotal, U16* u16VTotal);
void MDrv_MFC_SlowFrameLock(BOOL OnOff);
void MDrv_MFC_FrameLockMode(U8 u8Mode);
void MDrv_MFC_SetODCTable(void);

//------------------------------------------------------------------------------
// Register Read/Write
//------------------------------------------------------------------------------
U8 MDrv_MFC_ReadByte(U16 u16Addr);
U16 MDrv_MFC_Read2Bytes(U16 u16Reg);
U32 MDrv_MFC_Read3Bytes(U16 u16Reg);
BOOL MDrv_MFC_WriteByte(U16 u16Addr, U8 u8Val);
void MDrv_MFC_Write2Bytes(U16 u16Reg, U16 u16Value);
void MDrv_MFC_Write3Bytes(U16 u16Reg, U32 u32Value);
void MDrv_MFC_WriteBit(U16 u16Addr, U8 u8Bit, U8  u8BitPos);
void MDrv_MFC_WriteByteMask(U16 u16Addr, U8 u8Val, U8 u8Mask);
void MDrv_MFC_WriteRegsTbl(U16 wIndex, MST_MFC_RegUnitType_t *pTable);

//------------------------------------------------------------------------------
// Get/Set variable
//------------------------------------------------------------------------------
U8 MDrv_MFC_GetInitStatus(void);
void MDrv_MFC_SetInitStatus(U8 status);

//For MFC frame lock that controlled by scaler
void MDrv_MFC_SetVFreq(U16 u16Inputfreq, BOOL enableFPLL);

U8 MDrv_MFC_DbgReadByte(U16 u16Addr);
void MDrv_MFC_DbgWriteByte(U16 u16Addr, U8 u8Val);

void _MDrv_MFC_StartTimer(void);
void _MDrv_MFC_DeleteTimer(void);
void MDrv_MFC_SetSerialDebug(BOOL);

BOOL MDrv_MFC_SetSerialDebugMode(MFC_SERIAL_DEBUG_CMD_e cmd);

BOOL MDrv_MFC_IIC_Read(U8 u8ChIIC, U8 u8SlaveIdIIC, U8 *pu8AddrIIC, U8 u8AddrSizeIIC, U8 *pu8BufIIC, U32 u32BufSizeIIC );
BOOL MDrv_MFC_IIC_Write(U8 u8ChIIC, U8 u8SlaveIdIIC, U8 *pu8AddrIIC, U8 u8AddrSizeIIC, U8 *pu8BufIIC, U32 u32BufSizeIIC);

#endif
