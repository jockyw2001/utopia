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
/// @file   drvGE.h
/// @brief  GE Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_PRI_GE_H_
#define _DRV_PRI_GE_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    GE_BlendOp                  eBldCoef;
    GE_AlphaSrc                 eABLSrc;
    MS_U32                      u32ABLConstCoef;
}GE_ABLINFO;

typedef struct
{
    MS_U8  u8X0;       ///< Position of the left edge of the bounding box
    MS_U8  u8Width;    ///< Width of the bounding box
    MS_U8  u8Y0;       ///< Position of the top edge of the bounding box
    MS_U8  u8Height;   ///< Height of the bounding box
} GE_GlyphBbox;

typedef struct
{
    MS_PHY addr;       // flat address of whole memory map
    MS_U32 len;
    MS_BOOL inUsed;
    MS_U32 width;
    MS_U32 pitch;
    MS_U32 height;
    GE_BufFmt fmt;
} GE_BitmapInfo;

typedef struct
{
    /// font start address
    MS_PHY addr;
    /// font length
    MS_U32 len;
    /// font structure is used
    MS_BOOL inUsed;
    /// character width (pixel unit)
    MS_U32 width;
    /// character pitch (byte unit)
    MS_U32 pitch;
    /// character height (pixel unit)
    MS_U32 height;
    /// one character length (byte unit)
    MS_U32 offset;
    /// Bounding Box offset
    GE_GlyphBbox* pBBox;
    /// font format
    GE_BufFmt fmt;
} GE_FontInfo;

typedef struct __attribute__((packed))
{
    GE_CTX_HAL_SHARED           halSharedCtx;
    MS_U32                      u32GEClientAllocator;
    MS_U32                      u32LstGEClientId;//for indicating context switch
    MS_U32                      u32LstGEPaletteClientId;
    MS_BOOL                     bNotFirstInit;
    MS_U32                      u32HWSemaphoreCnt;
    MS_U32                      u32VCmdQSize;                       /// MIN:4K, MAX:512K, <MIN:Disable
    MS_PHY                      PhyVCmdQAddr;                       //  8-byte aligned
    MS_BOOL                     bIsHK;                              /// Running as HK or Co-processor
    MS_BOOL                     bIsCompt;                           /// Running as compatible mode. In compatible mode, the parameter checking loose for NOS APP compatibility.  TRUE=>compatible with 51/Chakra , FALSE=>linux OS style
    MS_BOOL                     bInit;

}GE_CTX_SHARED;

typedef struct
{
    GE_Context                  ctxHeader;
    GE_CTX_SHARED*              pSharedCtx;//pointer to shared context paramemetrs
    GE_CTX_HAL_LOCAL            halLocalCtx;
    MS_U16                      u16ClipL;
    MS_U16                      u16ClipR;
    MS_U16                      u16ClipT;
    MS_U16                      u16ClipB;
#if GE_LOCK_SUPPORT
    MS_S32                      s32GEMutex;
    MS_S32                      s32GELock;
#endif
    MS_S32                      s32GE_Recursive_Lock_Cnt; //recursive lock number when bGELock is off
    MS_U32                      u32GEClientId;
    MS_U32                      u32GESEMID;
    MS_U16                      u16GEPrevSEMID; //backup previous Sem ID for palette restore.
    MS_BOOL                     bIsComp;
    MS_U32                      u32CTXInitMagic;
    MS_BOOL                     bSrcPaletteOn;
    MS_BOOL                     bDstPaletteOn;
    MS_PHY                    PhySrcAddr;
    MS_PHY                    PhyDstAddr;
    MS_PHY                      PhyVcmdqAddr;
    MS_U16                      u16VcmdqMiuMsb;
    MS_U16                      u16VcmdqSize;
}GE_CTX_LOCAL;


typedef struct
{
    MS_U32                  u32dbglvl;
    void                    (*fpGetBMP)(MS_S16, GE_BitmapInfo*);
    void                    (*fpGetFont)(MS_S8, GE_FontInfo*);
   // GFX_BlendCoef           _blendcoef;
    MS_U8                   _angle;
    MS_BOOL                 _bNearest;
    MS_BOOL                 _bPatchMode;
    MS_BOOL                 _bMirrorH;
    MS_BOOL                 _bMirrorV;
    MS_BOOL                 _bDstMirrorH;
    MS_BOOL                 _bDstMirrorV;
    MS_BOOL                 _bItalic;
    MS_BOOL                 _line_enable;
    MS_U8                   _line_pattern;
    MS_U8                   _line_factor;
    MS_BOOL                 bDither;

#ifdef DBGLOG
    //debug use only
    MS_BOOL _bOutFileLog;
    MS_U16  *_pu16OutLogAddr;
    MS_U16  _u16LogCount;
#endif
    GE_Context*             g_pGEContext ;
    GE_CHIP_PROPERTY *      pGeChipProperty;
    GE_ABLINFO              pABLInfo;
    MS_U32                  u32LockStatus;
    MS_BOOL                 _bInit;
    MS_U32                  u32geRgbColor;
} GFX_API_LOCAL;


typedef struct _GFX_Resource_PRIVATE
{

}GFX_Resource_PRIVATE;

typedef struct _GFX_INSTANT_PRIVATE
{
    /*Resource*/
    GFX_Resource_PRIVATE*       pResource;

    GFX_API_LOCAL               GFXPrivate_g_apiGFXLocal;
    GE_CTX_LOCAL                GFXPrivate_g_drvGECtxLocal;

    //GFX_BitBltInfo*             pGFXBufferInfo;
    //GFX_Function*               pGFXFunction;
    //GFX_SetConfig*              pGFXSetConfig;
}GFX_INSTANT_PRIVATE;

// Debug Log
#include "ULog.h"

// Debug Logs, level form low(INFO) to high(FATAL, always show)
// Function information, ex function entry
#define GE_D_INFO(x, args...) ULOGI("GE DRV", x, ##args)
// Warning, illegal paramter but can be self fixed in functions
#define GE_D_WARN(x, args...) ULOGW("GE DRV", x, ##args)
//  Need debug, illegal paramter.
#define GE_D_DBUG(x, args...) ULOGD("GE DRV", x, ##args)
// Error, function will be terminated but system not crash
#define GE_D_ERR(x, args...) ULOGE("GE DRV", x, ##args)
// Critical, system crash. (ex. assert)
#define GE_D_FATAL(x, args...) ULOGF("GE DRV", x, ##args)

#ifdef __cplusplus
}
#endif

#endif

