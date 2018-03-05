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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_GOP_H_
#define _HAL_GOP_H_

#include "drvGOP.h"
#include "regGOP.h"

#include "apiGOP.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define Gop23_GwinCtl_Ofet                      0UL

#define MAX_GOP_MIUCOUNT                        2UL
#define MAX_GOP_MIUSEL                          MAX_GOP_MIUCOUNT-1
#define MAX_GOP_SUPPORT                         5UL
#define MAX_GOP_MUX                             4UL
#define MAX_GOP_MUX_SEL                         4UL
#define MAX_GOP_MUX_OPNum			            MAX_GOP_MUX
#define MAX_GOP_DualMUX_Num			            3UL
#define MAX_GOP0_GWIN                           2UL
#define MAX_GOP1_GWIN                           2UL
#define MAX_GOP2_GWIN                           1UL
#define MAX_GOP3_GWIN                           1UL
#define MAX_GOP4_GWIN                           0UL
#define MAX_GOP5_GWIN                           1UL

#define GOP0_Gwin0Id                            0UL
#define GOP0_Gwin1Id                            1UL
#define GOP1_Gwin0Id                            2UL
#define GOP1_Gwin1Id                            3UL
#define GOP2_Gwin0Id                            4UL
#define GOP3_Gwin0Id                            5UL
#define GOP4_Gwin0Id                            0xF0
#define GOP5_Gwin0Id                            6UL

#define GOP0_REG_FORM                           E_GOP_REG_FORM_2G + E_GOP_PAL_SIZE_256
#define GOP1_REG_FORM                           E_GOP_REG_FORM_2G + E_GOP_PAL_SIZE_256
#define GOP2_REG_FORM                           E_GOP_REG_FORM_T81G + E_GOP_PAL_SIZE_NONE
#define GOP3_REG_FORM                           E_GOP_REG_FORM_T81G + E_GOP_PAL_SIZE_NONE
#define GOP4_REG_FORM                           E_GOP_REG_FORM_NONE
#define GOP5_REG_FORM                           E_GOP_REG_FORM_T81G + E_GOP_PAL_SIZE_NONE
#define GOPD_REG_FORM                           E_GOPD_FIFO_DEPTH_64


#define GOP0_GwinIdBase                         GOP0_Gwin0Id
#define GOP1_GwinIdBase                         MAX_GOP0_GWIN
#define GOP2_GwinIdBase                         MAX_GOP0_GWIN + MAX_GOP1_GWIN
#define GOP3_GwinIdBase                         MAX_GOP0_GWIN + MAX_GOP1_GWIN + MAX_GOP2_GWIN
#define GOP4_GwinIdBase                         MAX_GOP0_GWIN + MAX_GOP1_GWIN + MAX_GOP2_GWIN + MAX_GOP3_GWIN
#define GOP5_GwinIdBase                         MAX_GOP0_GWIN + MAX_GOP1_GWIN + MAX_GOP2_GWIN + MAX_GOP3_GWIN + MAX_GOP4_GWIN

#define MAX_MIXER_MUX   						2
#define MIXER_MUX0Id 							0
#define MIXER_MUX1Id 							1


#define GOP_BIT0    0x01
#define GOP_BIT1    0x02
#define GOP_BIT2    0x04
#define GOP_BIT3    0x08
#define GOP_BIT4    0x10
#define GOP_BIT5    0x20
#define GOP_BIT6    0x40
#define GOP_BIT7    0x80
#define GOP_BIT8    0x0100
#define GOP_BIT9    0x0200
#define GOP_BIT10   0x0400
#define GOP_BIT11   0x0800
#define GOP_BIT12   0x1000
#define GOP_BIT13   0x2000
#define GOP_BIT14   0x4000
#define GOP_BIT15   0x8000

#define GOP_REG_WORD_MASK                       0xFFFFUL
#define GOP_REG_HW_MASK                         0xFF00UL
#define GOP_REG_LW_MASK                         0x00FFUL

#define GOP_VE_PAL_HS_DELAY           0xE4
#define GOP_VE_NTSC_HS_DELAY          0xD3
#define GOP_VE_PAL_HSTART_OFST  0x19
#define GOP_VE_PAL_VSTART_OFST  0x29
#define GOP_VE_NTSC_HSTART_OFST  0x19
#define GOP_VE_NTSC_VSTART_OFST  0x23
#define GOP_VE_PAL_WIDTH    0x2E9 //720+0x48
#define GOP_VE_PAL_HEIGHT    0x26A //0x269
#define GOP_VE_PAL_HTOTAL    0x359
#define GOP_VE_NTSC_WIDTH     0x2E9
#define GOP_VE_NTSC_HEIGHT    0x204 //0x203
#define GOP_VE_NTSC_HTOTAL    0x359

#define GOP_WordUnit                            32
#define GOP_DWIN_WordUnit                       32UL
#define GOP_TotalGwinNum                        (MAX_GOP0_GWIN+MAX_GOP1_GWIN+MAX_GOP2_GWIN+MAX_GOP3_GWIN+MAX_GOP4_GWIN+MAX_GOP5_GWIN)
#define HAL_GOP_BankOffset(pGOPHalLocal)        ((pGOPHalLocal)->bank_offset)

#define GOP_FIFO_BURST_ALL                      (GOP_BIT8|GOP_BIT9|GOP_BIT10|GOP_BIT11|GOP_BIT12)
#define GOP_FIFO_BURST_MIDDLE                   (GOP_BIT8|GOP_BIT9)
#define GOP_FIFO_BURST_SHORT                    (GOP_BIT8)

#define GOP_FIFO_BURST_MASK                     (GOP_BIT8|GOP_BIT9|GOP_BIT10|GOP_BIT11|GOP_BIT12)
#define GOP_FIFO_THRESHOLD                      0x30UL

#ifndef GOP_MIU0_LENGTH
#define GOP_MIU0_LENGTH                         HAL_MIU1_BASE
#endif

#define DWIN_SUPPORT_WINDOWDE_CAPTURE           FALSE //HW issue, Not support it, should use FrameDE to capture video for DWIN
#define DWIN_SUPPORT_OSD_CAPTURE                FALSE  //Support it
#define DWIN_SUPPORT_CLOCK_GATING               FALSE  //Support it

#define ENABLE_GOP_T3DPATCH
#ifdef ENABLE_GOP_T3DPATCH
#define GOP_PD_T3D                              0x153UL
#define GOP_PD_NORMAL                           0xD3UL
#endif

#define GOP_4K2K30

#define GOP_PUBLIC_UPDATE MAX_GOP_SUPPORT

#if (MAX_GOP_SUPPORT < 5)
#define GFLIP_REG_BANKS (MAX_GOP_SUPPORT * GOP_BANK_OFFSET)
#else
#define GFLIP_REG_BANKS (MAX_GOP_SUPPORT * GOP_BANK_OFFSET + 2)
#endif
#define GFLIP_REG16_NUM_PER_BANK                128UL

#define GPU_TILE_FORMAT_ARGB8888                0x5UL

#define AFBC_CORE_COUNT 2

#if (defined ANDROID) && (defined TV_OS)
#define GOP_CMDQ_ENABLE
#endif

#ifdef GOP_CMDQ_ENABLE
#include "drvCMDQ.h"
#endif

#define AFBC_ALIGN_FACTOR      16

#define VE_MUX_INIT_VALUE                       5UL

//Gwin enable HW bug
#define GOP_AUTO_CLK_GATING_PATCH

/*the following is for parameters for shared between multiple process context*/
typedef struct __attribute__((packed))
{
    GOP_CHIP_PROPERTY       gopChipProperty;
    MS_U16 u16GopSplitMode_LRWIDTH[SHARED_GOP_MAX_COUNT];
    DRV_GOPDstType GOP_Dst[SHARED_GOP_MAX_COUNT];
}GOP_CTX_HAL_SHARED;

/*the following is for parameters for used in local process context*/
typedef struct
{
    GOP_CTX_HAL_SHARED      *pHALShared;
    MS_VIRT                 va_mmio_base;
    MS_U32                  bank_offset;
    MS_U16                  u16Clk0Setting; ///Backup Current GOPG clock setting
    MS_U16                  u16Clk1Setting; ///Backup Current GOPD clock setting
    MS_U16                  u16Clk2Setting; ///Backup Current SRAM clock setting
    DRV_GOPDstType          drvGFlipGOPDst[MAX_GOP_SUPPORT];
    GOP_CHIP_PROPERTY       *pGopChipPro;
    DRV_GOP_CONSALPHA_BITS  User_ConsAlpha_bits;

    /*check all gop dst is valid or not for each mux*/
    MS_BOOL                 *pbIsMuxVaildToGopDst;
}GOP_CTX_HAL_LOCAL;

typedef struct
{
    GOP_CTX_HAL_LOCAL GOPHalSTRCtx;
    MS_U16 BankReg[GFLIP_REG_BANKS][GFLIP_REG16_NUM_PER_BANK];
    MS_U16 CKG_GopReg[10];
    MS_U16 GS_GopReg[3];
    MS_U16 XC_GopReg[20];
}GFLIP_REGS_SAVE_AREA;

//VE register bank
typedef enum
{
    MS_VE_REG_BANK_3B,
    MS_VE_REG_BANK_3E,
    MS_VE_REG_BANK_3F,
} MS_VE_REG_BANK;

typedef enum
{
    EN_OSD_0,
    EN_OSD_1,
}EN_VE_OSD_ENABLE;

/*To write VE bank register*/
extern void MApi_VE_W2BYTE_MSK(MS_VE_REG_BANK VE_BK, MS_U32 u32Reg, MS_U16 u16Val, MS_U16 u16Mask);
extern MS_U16 MApi_VE_R2BYTE_MSK(MS_VE_REG_BANK VE_BK, MS_U32 u32Reg, MS_U16 u16Mask);

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_GOP0 = 0,
    E_GOP1 = 1,
    E_GOP2 = 2,
    E_GOP3 = 3,
    E_GOP4 = 4,
    E_GOP_Dwin = 5,
    E_GOP_MIXER = 6,
    E_GOP5 = 7,
}E_GOP_TYPE;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
MS_BOOL _GetBnkOfstByGop(MS_U8 gop, MS_U32 *pBnkOfst);
void HAL_GOP_Init(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOPNum);
void HAL_GOP_Init_Context(GOP_CTX_HAL_LOCAL *pGOPHalLocal,
                                     GOP_CTX_HAL_SHARED *pHALShared, MS_BOOL bNeedInitShared);
void HAL_GOP_Chip_Proprity_Init(GOP_CTX_HAL_LOCAL *pGOPHalLocal);
void HAL_GOP_Restore_Ctx(GOP_CTX_HAL_LOCAL *pGOPHalLocal);
void HAL_GOP_Write16Reg(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32addr, MS_U16 u16val, MS_U16 mask);
void HAL_GOP_Write32Reg(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u16addr, MS_U32 u32val);
void HAL_GOP_Write32Pal(GOP_CTX_HAL_LOCAL *pGOPHalLocal,
MS_U8* pREGMAP_Base, MS_U16 *pREGMAP_Offset, MS_U32 u32REGMAP_Len,
MS_U8 u8Index, MS_U8 u8A, MS_U8 u8R, MS_U8 u8G, MS_U8 u8B);
void HAL_GOP_Read16Reg(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u16addr, MS_U16* pu16ret);
void HAL_GOP_GWIN_SetBlending(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8win, MS_BOOL bEnable, MS_U8 u8coef);
void HAL_GOP_SetIOMapBase(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_VIRT addr);
void HAL_GOP_SetIOFRCMapBase(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_VIRT addr);
void HAL_GOP_SetIOPMMapBase(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_VIRT addr);
void HAL_GOP_GWIN_GetMUX(GOP_CTX_HAL_LOCAL*pGOPHalLocal, MS_U8* u8GOPNum, Gop_MuxSel eGopMux);
void HAL_GOP_GWIN_SetMUX(GOP_CTX_HAL_LOCAL*pGOPHalLocal, MS_U8 u8GOPNum, Gop_MuxSel eGopMux);
void HAL_GOP_GetGOPEnum(GOP_CTX_HAL_LOCAL *pGOPHalLocal, GOP_TYPE_DEF* GOP_TYPE);
MS_U16 HAL_GOP_GetBPP(GOP_CTX_HAL_LOCAL *pGOPHalLocal, DRV_GOPColorType fbFmt);
GOP_Result HAL_GOP_SetGOPACKMask(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U16 u16GopMask);
GOP_Result HAL_GOP_SetGOPACK(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 gop);
MS_U16 HAL_GOP_GetGOPACK(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 gop);
MS_U8 HAL_GOP_GetMaxGwinNumByGOP(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GopNum);
MS_U8 HAL_GOP_SelGwinIdByGOP(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8Gop, MS_U8 u8Idx);
MS_U8 HAL_GOP_GetMIUDst(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopnum);
void HAL_GOP_SetIPSel2SC(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_IPSEL_GOP ipSelGop);
E_GOP_VIDEOTIMING_MIRRORTYPE HAL_GOP_GetVideoTimingMirrorType(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_BOOL bHorizontal);
MS_U8 HAL_GOP_GetDWINMIU(GOP_CTX_HAL_LOCAL *pGOPHalLocal);
GOP_Result HAL_GOP_DWIN_SetSourceSel(GOP_CTX_HAL_LOCAL *pGOPHalLocal, DRV_GOP_DWIN_SRC_SEL enSrcSel);
GOP_Result HAL_GOP_DWIN_EnableR2YCSC(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_BOOL bEnable);
GOP_Result HAL_GOP_SetDWINMIU(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 miu);
GOP_Result HAL_GOP_GetGOPDst(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8gopNum, DRV_GOPDstType *pGopDst);
GOP_Result HAL_GOP_GetMixerDst(GOP_CTX_HAL_LOCAL *pGOPHalLocal, DRV_GOPDstType *pGopDst);
GOP_Result HAL_GOP_SetMixerDst(GOP_CTX_HAL_LOCAL *pGOPHalLocal, DRV_GOPDstType eDstType);
GOP_Result HAL_GOP_GOPSel(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOPNum);
GOP_Result HAL_GOP_SetGOPHighPri(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum);
GOP_Result HAL_GOP_SetGOPEnable2SC(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, MS_BOOL bEnable);
GOP_Result HAL_GOP_SetGOP2Pto1P(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, MS_BOOL bEnable);
GOP_Result HAL_GOP_SetGOPEnable2Mode1(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, MS_BOOL bEnable);
GOP_Result HAL_GOP_GetGOPAlphaMode1(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, MS_BOOL *pbEnable);
GOP_Result HAL_GOP_GWIN_SetDstPlane(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 GopNum, DRV_GOPDstType eDstType,MS_BOOL bOnlyCheck);
GOP_Result HAL_GOP_SetGOPClk(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, DRV_GOPDstType eDstType);
GOP_Result HAL_GOP_SetClkForCapture(GOP_CTX_HAL_LOCAL *pGOPHalLocal, DRV_GOP_DWIN_SRC_SEL enSrcSel);
GOP_Result HAL_GOP_MIXER_SetGOPEnable2Mixer(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, MS_BOOL bEnable);
GOP_Result HAL_GOP_MIXER_EnableVfilter(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_BOOL bEn);
GOP_Result HAL_GOP_MIXER_SetMux(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, MS_U8 muxNum, MS_BOOL bEnable);
GOP_Result HAL_GOP_MIXER_EnableOldBlendMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP, MS_BOOL bEn);
GOP_Result HAL_GOP_EnableSCPerPixelNewAlphaMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal, Gop_MuxSel muxNum, MS_BOOL bEnable);
GOP_Result HAL_GOP_EnableSCNewAlphaMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal, Gop_MuxSel muxNum, MS_BOOL bEnable);
GOP_Result HAL_GOP_InitMux(GOP_CTX_HAL_LOCAL *pGOPHalLocal);
GOP_Result HAL_GOP_SetClock(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_BOOL bEnable);
GOP_Result HAL_ConvertAPIAddr(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gwinid, MS_PHY* u64Adr);
GOP_Result HAL_GOP_VE_SetOutputTiming(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32mode);
GOP_Result HAL_GOP_MIXER_SetOutputTiming(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32mode, GOP_DRV_MixerTiming *pTM);
GOP_Result HAL_GOP_GWIN_EnableTileMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8win, MS_BOOL bEnable, E_GOP_TILE_DATA_TYPE tilemode);
GOP_Result HAL_GOP_SetUVSwap(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOPNum,MS_BOOL bEn);
GOP_Result HAL_GOP_SetYCSwap(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOPNum,MS_BOOL bEn);
GOP_Result HAL_GOP_GWIN_GetNewAlphaMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8win, MS_BOOL* pEnable);
GOP_Result HAL_GOP_GWIN_SetNewAlphaMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8win, MS_BOOL bEnable);
GOP_Result HAL_GOP_GWiN_Set3DOSD_Sub(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 u8GOP ,MS_U8 u8Gwin, MS_PHY u32SubAddr);
GOP_Result HAL_GOP_VE_SetOSDEnable(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_BOOL bEnable, EN_VE_OSD_ENABLE eOSD, MS_U8 gopNum);
GOP_Result HAL_GOP_SetGOPToVE(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 gopNum, MS_BOOL bEn );
GOP_Result HAL_GOP_3D_SetMiddle(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 u8GOP,MS_U16 u16Middle);
GOP_Result HAL_GOP_OC_SetOCEn(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP, MS_BOOL bOCEn);
GOP_Result HAL_GOP_OC_Get_MIU_Sel(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 *MIUId);
GOP_Result HAL_GOP_OC_SetOCInfo(GOP_CTX_HAL_LOCAL *pGOPHalLocal, DRV_GOP_OC_INFO* pOCinfo);
GOP_Result HAL_GOP_DWIN_SetRingBuffer(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32RingSize,MS_U32 u32BufSize);
GOP_Result HAL_GOP_AdjustField(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 GopNum, DRV_GOPDstType eDstType);
GOP_Result HAL_GOP_TestPattern_IsVaild(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GopNum);
GOP_Result HAL_GOP_SetWinFmt(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 regForm, MS_U8 u8GOPNum, MS_U8 u8GwinNum, MS_U16 colortype);
GOP_Result HAL_GOP_Set_PINPON(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOPNum, MS_BOOL bEn, E_DRV_GOP_PINPON_MODE pinpon_mode);
GOP_Result HAL_GOP_HScalingDown(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 u8GOP, MS_BOOL bEnable,MS_U16 src, MS_U16 dst);
GOP_Result HAL_GOP_VScalingDown(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 u8GOP, MS_BOOL bEnable,MS_U16 src, MS_U16 dst);
GOP_Result HAL_GOP_DeleteWinHVSize(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 u8GOP, MS_U16 u16HSize, MS_U16 u16VSize);
GOP_Result HAL_GOP_DumpGOPReg(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32GopIdx, MS_U16 u16BankIdx, MS_U16 u16Addr, MS_U16* u16Val);
GOP_Result HAL_GOP_RestoreGOPReg(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32GopIdx, MS_U16 u16BankIdx, MS_U16 u16Addr, MS_U16 u16Val);
GOP_Result HAL_GOP_PowerState(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32PowerState, GFLIP_REGS_SAVE_AREA* pGOP_STRPrivate);
GOP_Result HAL_GOP_GWIN_SetGPUTileMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 gwinid, EN_DRV_GOP_GPU_TILE_MODE tile_mode);
GOP_Result HAL_GOP_EnableTLB(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 u8GOP, MS_BOOL bEnable);
GOP_Result HAL_GOP_SetTLBAddr(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP, MS_PHY u64TLBAddr, MS_U32 u32size);
GOP_Result HAL_GOP_SetTLBSubAddr(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP, MS_PHY u64TLBAddr);
#ifdef GOP_CMDQ_ENABLE
GOP_Result HAL_GOP_CMDQ_WriteCommand(GOP_CTX_HAL_LOCAL *pGOPHalLocal,CAF_Struct *cmdq_struct,MS_U32 *number,MS_U32 u32addr, MS_U16 u16val, MS_U16 mask);
GOP_Result HAL_GOP_CMDQ_BegineDraw(GOP_CTX_HAL_LOCAL *pGOPHalLocal,CAF_Struct *target,MS_U32 *number, MS_U32 *u32GopIdx);
GOP_Result HAL_GOP_CMDQ_EndDraw(GOP_CTX_HAL_LOCAL *pGOPHalLocal,CAF_Struct *target,MS_U32 *number, MS_U32 u32GopIdx);
GOP_Result HAL_GOP_CMDQ_SetGOPACKMask(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U16 u16GopMask);
GOP_Result HAL_GOP_CMDQ_SetGOPACK(GOP_CTX_HAL_LOCAL *pGOPHalLocal,MS_U8 gop);
#endif
GOP_Result HAL_GOP_EnableTwoLineBufferMode(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP, MS_BOOL bEnable);
GOP_Result HAL_GOP_Set_GWIN_INTERNAL_MIU(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP, MS_U8 miusel);
GOP_Result HAL_GOP_Set_MIU(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP,MS_U8 miusel);
GOP_Result HAL_GOP_SetGopGwinHVPixel(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP, MS_U8 u8win, MS_U16 hstart, MS_U16 hend, MS_U16 vstart, MS_U16 vend);
GOP_Result HAL_GOP_AFBC_GetCore(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP,MS_U8* u8Core);
GOP_Result Hal_SetCropWindow(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U8 u8GOP, EN_GOP_CROP_CTL crop_mode);
//GOP_Result HAL_GOP_SetDbgLevel(EN_GOP_DEBUG_LEVEL level);
#define HAL_GOP_SetDbgLevel(args...)
#endif // _HAL_TEMP_H_
