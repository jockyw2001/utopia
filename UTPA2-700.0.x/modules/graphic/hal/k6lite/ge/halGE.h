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

#ifndef _HAL_GE_H_
#define _HAL_GE_H_


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define bNeedSetActiveCtrlMiu1      TRUE
#define GE_THRESHOLD_SETTING        0xDUL
#define GE_WordUnit                 32UL
#define GE_MAX_MIU                  2UL
#define GE_TABLE_REGNUM             0x80UL

#define GE_FMT_I1                 0x0UL
#define GE_FMT_I2                 0x1UL
#define GE_FMT_I4                 0x2UL
#define GE_FMT_I8                 0x4UL
#define GE_FMT_FaBaFgBg2266       0x6UL
#define GE_FMT_1ABFgBg12355       0x7UL
#define GE_FMT_RGB565             0x8UL
#define GE_FMT_ARGB1555           0x9UL
#define GE_FMT_ARGB4444           0xAUL
#define GE_FMT_ARGB1555_DST       0xCUL
#define GE_FMT_YUV422             0xEUL
#define GE_FMT_ARGB8888           0xFUL
#define GE_FMT_RGBA5551           0x10UL
#define GE_FMT_ABGR1555           0x11UL
#define GE_FMT_BGRA5551           0x12UL
#define GE_FMT_RGBA4444           0x13UL
#define GE_FMT_ABGR4444           0x14UL
#define GE_FMT_BGRA4444           0x15UL
#define GE_FMT_BGR565             0x16UL
#define GE_FMT_RGBA8888           0x1DUL
#define GE_FMT_ABGR8888           0x1EUL
#define GE_FMT_BGRA8888           0x1FUL
#define GE_FMT_GENERIC            0xFFFFUL

#if (GE_WordUnit==32)
#define GE_VQ_4K                  0x0UL
#define GE_VQ_8K                  0x0UL
#define GE_VQ_16K                 0x0UL
#define GE_VQ_32K                 0x1UL
#define GE_VQ_64K                 0x2UL
#define GE_VQ_128K                0x3UL
#define GE_VQ_256K                0x4UL
#define GE_VQ_512K                0x5UL
#define GE_VQ_1024K               0x6UL
#define GE_VQ_2048K               0x7UL
#else
#define GE_VQ_4K                  0x0UL
#define GE_VQ_8K                  0x0UL
#define GE_VQ_16K                 0x1UL
#define GE_VQ_32K                 0x2UL
#define GE_VQ_64K                 0x3UL
#define GE_VQ_128K                0x4UL
#define GE_VQ_256K                0x5UL
#define GE_VQ_512K                0x6UL
#define GE_VQ_1024K               0x7UL
#endif

#define GE_USE_HW_SEM (0)

//Patch
#define GE_SPLIT_MODE_PATCH       1//when open split mode, do fill rect + alpha blending will cause ge hang in certain case.
#define GE_PITCH_256_ALIGNED_UNDER_4P_MODE 0// For Curry/Kano, pitch must 256 algined under 4P mode
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef struct _GE_ColorDelta
{
    MS_U32                          r; // s7.12
    MS_U32                          g;
    MS_U32                          b;
    MS_U16                          a; // s4.11
} GE_ColorDelta;

/*the following is for parameters for shared between multiple process context*/
typedef struct __attribute__((packed))
{
   MS_BOOL bGE_DirectToReg;
   MS_U16 global_tagID;
   MS_U16 u16ShareRegImage[GE_TABLE_REGNUM];
   MS_U16 u16ShareRegImageEx[GE_TABLE_REGNUM];
}GE_CTX_HAL_SHARED;

/*the following is for parameters for used in local process context*/
typedef struct
{
   GE_CTX_HAL_SHARED *pHALShared;
   GE_CHIP_PROPERTY  *pGeChipPro;
   MS_VIRT           va_mmio_base;
   MS_VIRT           va_mmio_base2;
   MS_U16            u16RegGETable[GE_TABLE_REGNUM];                 //Store for GE RegInfo
   MS_U16            u16RegGETableEX[GE_TABLE_REGNUM];                 //Store for GE RegInfo
   MS_BOOL           bIsComp;
   MS_BOOL           bPaletteDirty;
   MS_U32            u32Palette[GE_PALETTE_NUM];
   MS_BOOL           bYScalingPatch;
}GE_CTX_HAL_LOCAL;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void        GE_Chip_Proprity_Init(GE_CTX_HAL_LOCAL *pGEHalLocal);
void        GE_ResetState(GE_CTX_HAL_LOCAL *pGEHalLocal);
void        GE_WaitIdle(GE_CTX_HAL_LOCAL *pGEHalLocal);
GE_Result   GE_Map_Share_Reg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr);
void        GE_WriteReg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr, MS_U16 value);
MS_U16      GE_ReadReg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr);
void        GE2_WriteReg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr, MS_U16 value);
MS_U16      GE2_ReadReg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr);
GE_Result   GE_InitCtxHalPalette(GE_CTX_HAL_LOCAL *pGEHalLocal);
void        GE_Init_HAL_Context(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_CTX_HAL_SHARED *pHALShared, MS_BOOL bNeedInitShared);
void        GE_Init_RegImage(GE_CTX_HAL_LOCAL *pGEHalLocal);
void        GE_Init(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_Config *cfg);
GE_Result   GE_SetRotate(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_RotateAngle geRotAngle);
GE_Result   GE_SetOnePixelMode(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL enable);
GE_Result   GE_SetBlend(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_BlendOp eBlendOp);
GE_Result   GE_SetAlpha(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_AlphaSrc eAlphaSrc);
GE_Result   GE_QueryDFBBldCaps(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 *pU16SupportedBldFlags);
GE_Result   GE_EnableDFBBld(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL enable);
GE_Result   GE_SetDFBBldFlags(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 u16DFBBldFlags);
GE_Result   GE_SetDFBBldOP(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_DFBBldOP geSrcBldOP, GE_DFBBldOP geDstBldOP);
GE_Result   GE_SetDFBBldConstColor(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_RgbColor geRgbColor);
GE_Result   GE_SetDFBBldSrcColorMask(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_RgbColor geRgbColor);
GE_Result   GE_WriteProtect(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U8 miu, MS_PHY addr_low, MS_PHY addr_high, GE_WPType eWPType);
GE_Result   GE_SetSrcTile(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL tile);
GE_Result   GE_SetDstTile(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL tile);
GE_Result   GE_GetFmtCaps(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_BufFmt fmt, GE_BufType type, GE_FmtCaps *caps);
GE_Result   GE_Set_IOMap_Base(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_VIRT addr);
GE_Result   GE_Set_IOMap_Base2(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_VIRT addr);
//GE_Result   GE_Get_BackupRegArray(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U32* addr);
//void           GE_RestoreReg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr, MS_U16 value);
//MS_U16      GE_BackupReg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr);
MS_U16      GE_GetNextTAGID(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL bStepTagBefore);
GE_Result   GE_WaitTAGID(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 tagID);
GE_Result   GE_PollingTAGID(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 tagID);
GE_Result   GE_Restore_HAL_Context(GE_CTX_HAL_LOCAL *pGEHalLocal);
GE_Result   GE_ClrInvalMIUFlg(GE_CTX_HAL_LOCAL *pGEHalLocal,MS_BOOL enable);
GE_Result   GE_EnableDynaClkGate(GE_CTX_HAL_LOCAL *pGEHalLocal,MS_BOOL enable);
GE_Result   GE_CalcBltScaleRatio(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 u16SrcWidth, MS_U16 u16SrcHeight, MS_U16 u16DstWidth, MS_U16 u16DstHeight, GE_ScaleInfo *pScaleinfo);
GE_Result   GE_SetBltScaleRatio(GE_CTX_HAL_LOCAL *pGEHalLocal,GE_Rect *src, GE_DstBitBltType *dst, GE_Flag flags, GE_ScaleInfo* scaleinfo);
GE_Result   GE_BitBltEX_Trape(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_Rect *pSrcRect, GE_Normalized_Trapezoid *pGENormTrapezoid, MS_U32 u32Flags, GE_ScaleInfo* pScaleinfo);
GE_Result   GE_FillTrapezoid(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL bYTrapezoid, GE_Normalized_Trapezoid *pGENormTrapezoid, MS_U32 u32Color, GE_ColorDelta *pColorDeltaX, GE_ColorDelta *pColorDeltaY);
GE_Result   GE_EnableTrapezoidAA(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL bEnable);
GE_Result   GE_EnableTrapSubPixCorr(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL bEnable);
GE_Result   GE_SetDisaMIUAccess(GE_CTX_HAL_LOCAL *pGEHalLocal,MS_BOOL enable);
GE_Result   GE_SetASCK(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL enable);
GE_Result   GE_SetDSCK(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL enable);
GE_Result   GE_SetVCmdBuffer(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_PHY PhyAddr, GE_VcmqBufSize enBufSize);
MS_PHY    GE_ConvertAPIAddr2HAL(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U8 u8MIUId, MS_PHY PhyGE_APIAddrInMIU);
MS_PHY    GE_ConvertHALAddr2API(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U8 u8MIUId, MS_PHY PhyGE_HALAddr);
void        GE_SetActiveCtrlMiu1(GE_CTX_HAL_LOCAL *pGEHalLocal);
GE_Result   GE_SetClock(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL bOnOff);
MS_BOOL     GE_NonOnePixelModeCaps(GE_CTX_HAL_LOCAL *pGEHalLocal, PatchBitBltInfo* patchInfo);
GE_Result   HAL_GE_EnableCalcSrc_WidthHeight(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL bEnable);
GE_Result   GEWD_ReadReg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr, MS_U16* value);
GE_Result   GEWD_WriteReg(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 addr, MS_U16 value);
GE_Result   GE_SetTLBMode(GE_CTX_HAL_LOCAL *pGEHalLocal, GE_TLB_Mode tlb_type);
GE_Result   GE_GetTLBSRCADDR(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_PHY* addr);
GE_Result   GE_GetTLBDSTADDR(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_PHY* addr);
GE_Result   GE_SetTLBSrcBaseAddr(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_PHY addr);
GE_Result   GE_SetTLBDstBaseAddr(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_PHY addr);
GE_Result   GE_FlushTLBTable(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL bEnable);
GE_Result   GE_SetTLBTag(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U16 tag);
GE_Result   GE_StopFlushTLB(GE_CTX_HAL_LOCAL *pGEHalLocal);
GE_Result   GE_Get_MIU_INTERVAL(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U8 miu, MS_PHY* value);
GE_Result   HAL_GE_AdjustDstWin( GE_CTX_HAL_LOCAL *pGEHalLocal, MS_BOOL bDstXInv );
GE_Result   HAL_GE_AdjustRotateDstWin(GE_CTX_HAL_LOCAL *pGEHalLocal, MS_U8 u8Rotate);
GE_Result   HAL_GE_SetBurstMiuLen(GE_CTX_HAL_LOCAL *pGEHalLocal,MS_BOOL bEnable,MS_U32 u32BurstLen);


#define      GE_SetSrcBufMIUId(pGEHalLocal, u8MIUId)  do{}while(0)
#define      GE_SetDstBufMIUId(pGEHalLocal, u8MIUId)  do{}while(0)
#define      GE_SetVQBufMIUId(pGEHalLocal, u8MIUId)  do{}while(0)
#define      GE_GetSrcBufMIUId(pGEHalLocal, u32GE_HALAddr)     ((MS_U8)(((u32GE_HALAddr)&(1UL<<31))>>31))
#define      GE_GetDstBufMIUId(pGEHalLocal, u32GE_HALAddr)     ((MS_U8)(((u32GE_HALAddr)&(1UL<<31))>>31))
#endif // _HAL_GE_H_
