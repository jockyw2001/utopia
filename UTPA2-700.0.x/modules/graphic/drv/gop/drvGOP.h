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
/// @file   drvGOP.h
/// @brief  Graphics Output Path Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_GOP_H_
#define _DRV_GOP_H_

#ifdef __cplusplus
extern "C"
{
#endif
#if defined(MSOS_TYPE_LINUX_KERNEL)
#include "autoconf.h"
#endif
#include "MsCommon.h"

#if defined(MSOS_TYPE_LINUX)
#include <sys/types.h>
#include <unistd.h>
#elif defined(MSOS_TYPE_LINUX_KERNEL)
#define getpid() pInstance
#else
#define getpid() 0UL
#endif

#include "apiGOP.h"

#define GETPIDTYPE ptrdiff_t

#if (defined ANDROID) && (defined TV_OS)
#include <cutils/log.h>
#define printf LOGD
#ifndef LOGD // android 4.1 rename LOGx to ALOGx
#define LOGD ALOGD
#endif
#endif

#ifdef  MSOS_TYPE_LINUX
#include <assert.h>
#include <unistd.h>
#define APIGOP_ASSERT(_bool, pri)  if (!(_bool)) {printf("\nAssert in %s,%d\n", __FUNCTION__, __LINE__); {pri;}; MsOS_DelayTask(100); assert(0);}
#else
#define APIGOP_ASSERT(_bool, pri)  if (!(_bool)) {printf("\nAssert in %s,%d\n", __FUNCTION__, __LINE__); {pri;};};
#endif

#if defined(CONFIG_API_GOP) || defined(CONFIG_API_GOP_MODULE)
#else
#define CONFIG_GOP_POOL_ARRANGE
#define CONFIG_GOP_GWIN_MISC
#define CONFIG_GOP_2ND_PAL
#define CONFIG_GOP_BG
#define CONFIG_GOP_FADE
#define CONFIG_GOP_YUV_SWAP
#define CONFIG_GOP_CONTRAST
#define CONFIG_GOP_GOP_VE_CAPTURE
#define CONFIG_GOP_SCROLL
#define CONFIG_GOP_TEST_PATTERN
#define CONFIG_GOP_VE_CAPTURE
#endif
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Customize Define
//-------------------------------------------------------------------------------------------------
#define CUS_SETTING               0UL
#define INSTANT_PRIVATE           1UL
#if CUS_SETTING
#define WINFB_INSHARED            1UL
#define POOL_INSHARED             1UL
#define CURSOR_SUPPORT            1UL
#define MUTEX_LOCK_ENABLE         0UL
#define ENABLE_GOP0_RBLK_MIRROR   FALSE
#ifndef DRV_MAX_GWIN_FB_SUPPORT
#define DRV_MAX_GWIN_FB_SUPPORT   0xFFFEUL
#endif
#define GFLIP_ENABLE              FALSE
#define BOOTLOGO_PATCH            TRUE
#define GOP_PIXEL_ALIGN           FALSE
#define GOP_SW_SCROLL             TRUE
#define GOP_3D_TYPE_SEL           E_DRV_3D_DUP_FULL
#define GOP_VSYNC_WAIT_BYSLEEP    FALSE

#else  //mstar default
#define WINFB_INSHARED            1UL
#define POOL_INSHARED             0UL
#define CURSOR_SUPPORT            0UL
#define MUTEX_LOCK_ENABLE         0UL
#define ENABLE_GOP0_RBLK_MIRROR   TRUE
#ifndef DRV_MAX_GWIN_FB_SUPPORT
#define DRV_MAX_GWIN_FB_SUPPORT   32UL
#endif

#ifdef CONFIG_GOP_DISABLE_GFLIP
#define GFLIP_ENABLE              FALSE
#else
#define GFLIP_ENABLE              TRUE
#endif
#define BOOTLOGO_PATCH            FALSE
#define GOP_PIXEL_ALIGN           FALSE
#define GOP_SW_SCROLL             FALSE
#define GOP_3D_TYPE_SEL           E_DRV_3D_DUP_HALF
#define GOP_VSYNC_WAIT_BYSLEEP    TRUE
#endif


#ifdef  MSOS_TYPE_NOS
#define GOP_LOCK_SUPPORT                0UL
#define GOP_XC_LOCK_SUPPORT             0UL
#define GOP_XC_SEMPHORE_PROTECT         0UL
#else
#define GOP_LOCK_SUPPORT                MUTEX_LOCK_ENABLE
#define GOP_XC_LOCK_SUPPORT             MUTEX_LOCK_ENABLE
#ifdef UTOPIA_V2  //XC
#define GOP_XC_SEMPHORE_PROTECT         1UL
#else
#define GOP_XC_SEMPHORE_PROTECT         0UL
#endif

#endif
#define GOP_XC_LOCK_DEBUG               0UL

#ifdef GOP_UTOPIA2K
    #define GOPDRIVER_BASE              KERNEL_MODE
#else
    #define GOPDRIVER_BASE              0UL
#endif
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define UNUSED( var )                   (void)((var) = (var))

#define MAX_GOP_INFO_COUNT      6
#define INVAILD_GOP_NUM                 0xFFUL
#define INVALID_GWIN_ID                 0xFFUL
#define GWIN_OSD_DEFAULT                (MS_U8)g_pGOPCtxLocal->pGopChipProperty->TotalGwinNum
#define INVALID_POOL_NEXT_FBID          0xFFFFFFFFUL
#define FB_FMT_AS_DEFAULT               0xFFFFUL
#define INVALID_CLIENT_PID              0x00UL //Invalid PID
#define INVALID_GOP_MUX_VAL             0xFFFFUL

#define MIU_0                           0UL
#define MIU_1                           1UL

#define GOP_STRETCH_WIDTH_UNIT          2UL    // 2-pixel

#define GOP_STRETCH_TABLE_NUM           50UL
#define GOP_VSTRETCH_TABLE_NUM          32UL
#define GOP_REG_MAX                     128UL


#define SHARED_GWIN_MAX_COUNT           32UL
#define SHARED_GOP_MAX_COUNT            8UL
#define MAX_GOP_SW_SCROLLNUM            16UL
#define MAX_MUX_NUMBER_SUPPORT          MAX_GOP_MUX

#define HAS_BIT(flag, bit)              ((flag) &  (1<<bit))
#define SET_BIT(flag, bit)              ((flag) |= (1<<bit))
#define RESET_BIT(flag, bit)            ((flag) &= (~(1<<bit)))

#define GOP_MIU_CLIENT_OC9              0x9UL
#define INVALID_BPP                     0xFFFFUL

#define SCALING_MULITPLIER              0x1000UL

#define GOP_FMT_RGB555_BLINK       0x0UL
#define GOP_FMT_RGB565             0x1UL
#define GOP_FMT_ARGB4444           0x2UL
#define GOP_FMT_FaBaFgBg2266       0x3UL
#define GOP_FMT_I8                 0x4UL
#define GOP_FMT_ARGB8888           0x5UL
#define GOP_FMT_ARGB1555           0x6UL
#define GOP_FMT_ABGR8888           0x7UL
#define GOP_FMT_ARGB1555_DST       0x8UL
#define GOP_FMT_YUV422             0x9UL
#define GOP_FMT_RGBA5551           10UL
#define GOP_FMT_RGBA4444           11UL
#define GOP_FMT_RGBA8888           12UL
#define GOP_FMT_BGR565             17UL
#define GOP_FMT_ABGR4444           18UL
#define GOP_FMT_ABGR1555           22UL
#define GOP_FMT_BGRA5551           26UL
#define GOP_FMT_BGRA4444           27UL
#define GOP_FMT_BGRA8888           28UL
#define GOP_FMT_GENERIC            0xFFFFUL

#ifndef PAGE_SIZE
#define PAGE_SIZE                  4096UL
#endif
#define PER_BYTE_BITS              8UL
#define TLB_PER_ENTRY_SIZE         4UL
#define PER_MIU_TLB_ENTRY_COUNT    8UL
#define ADDRESSING_8BYTE_UNIT      8UL

///GOP_PALETTE_ENTRY_NUM
#define GOP_PALETTE_ENTRY_NUM_MAX   256UL

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
/// Define GOP driver/hal layer return code, this enum should sync with E_GOP_API_Result
typedef enum
{
    GOP_FAIL                        = 0,
    GOP_SUCCESS                     = 1,
    GOP_NON_ALIGN_ADDRESS           ,
    GOP_NON_ALIGN_PITCH             ,
    GOP_DEPEND_NOAVAIL              ,
    GOP_MUTEX_OBTAIN_FAIL           ,
    GOP_MUTEX_OBTAINED              ,

    GOP_INVALID_BUFF_INFO          ,
    GOP_INVALID_PARAMETERS,
    GOP_FUN_NOT_SUPPORTED,
    GOP_ENUM_NOT_SUPPORTED,
    GOP_CRT_GWIN_FAIL               = 0xFE,
    GOP_CRT_GWIN_NOAVAIL            = 0xFF,
} GOP_Result;


/// Define GWIN color format.
typedef enum
{
    /// Color format RGB555 and Blink.
    E_DRV_GOP_COLOR_RGB555_BLINK    =E_MS_FMT_1ABFgBg12355,
    /// Color format RGB565.
    E_DRV_GOP_COLOR_RGB565          =E_MS_FMT_RGB565,
    /// Color format ARGB4444.
    E_DRV_GOP_COLOR_ARGB4444        =E_MS_FMT_ARGB4444,
    /// Color format alpha blink.
    E_DRV_GOP_COLOR_2266            =E_MS_FMT_FaBaFgBg2266,
    /// Color format I8 (256-entry palette).
    E_DRV_GOP_COLOR_I8              =E_MS_FMT_I8,
    /// Color format ARGB8888.
    E_DRV_GOP_COLOR_ARGB8888        =E_MS_FMT_ARGB8888,
    /// Color format ARGB1555.
    E_DRV_GOP_COLOR_ARGB1555        =E_MS_FMT_ARGB1555,
    /// Color format ARGB8888.  - Andriod format
    E_DRV_GOP_COLOR_ABGR8888        =E_MS_FMT_ABGR8888,
    /// Color format RGB555/YUV422.
    E_DRV_GOP_COLOR_RGB555YUV422    =E_MS_FMT_ARGB1555_DST,
    /// Color format YUV422.
    E_DRV_GOP_COLOR_YUV422          =E_MS_FMT_YUV422,
    /// Color format RGBA5551.  - Andriod format
    E_DRV_GOP_COLOR_RGBA5551        =E_MS_FMT_RGBA5551,
    /// Color format RGBA4444.  - Andriod format
    E_DRV_GOP_COLOR_RGBA4444        =E_MS_FMT_RGBA4444,
    /// Color format RGBA8888.    - Andriod Format
    E_DRV_GOP_COLOR_RGBA8888        =E_MS_FMT_RGBA8888,
    /// Color format BGR565.    - Andriod Format
    E_DRV_GOP_COLOR_BGR565          =E_MS_FMT_BGR565,
    /// Color format ABGR4444.    - Andriod Format
    E_DRV_GOP_COLOR_ABGR4444        =E_MS_FMT_ABGR4444,
    /// Color format AYUV8888.    - Andriod Format
    E_DRV_GOP_COLOR_AYUV8888        =E_MS_FMT_AYUV8888,
    /// Color format ABGR1555.    - Andriod Format
    E_DRV_GOP_COLOR_ABGR1555        =E_MS_FMT_ABGR1555,
    /// Color format BGRA5551.    - Andriod Format
    E_DRV_GOP_COLOR_BGRA5551        =E_MS_FMT_BGRA5551,
    /// Color format BGRA4444.    - Andriod Format
    E_DRV_GOP_COLOR_BGRA4444        =E_MS_FMT_BGRA4444,
    /// Color format BGRA8888.    - Andriod Format
    E_DRV_GOP_COLOR_BGRA8888        =E_MS_FMT_BGRA8888,

    /// Invalid color format.
    E_DRV_GOP_COLOR_INVALID         =E_MS_FMT_GENERIC
} DRV_GOPColorType;

typedef enum
{
    DRV_VALID_6BITS             =   0,
    DRV_VALID_8BITS             =   1,
    DRV_NONE_SETTING            =   2,
} DRV_GOP_CONSALPHA_BITS;

typedef enum
{
    E_DRV_3D_NONE               =   0, ///3D type: Not support
    E_DRV_3D_DUP_HALF           =   1, ///3D type: Support, but SW need expand GWIN and StretchWin
    E_DRV_3D_DUP_FULL           =   2, ///3D type: Support, and HW automatically expand GWIN and StretchWin
} EN_DRV_GOP_3D_TYPE;

/// Define Mux
typedef enum
{
    ///Select gop output to mux0
    E_GOP_MUX0 = 0,
    /// Select gop output to mux1
    E_GOP_MUX1 = 1,
    /// Select gop output to mux2
    E_GOP_MUX2 = 2,
    /// Select gop output to mux3
    E_GOP_MUX3 = 3,
    /// Select gop output to mux4
    E_GOP_MUX4 = 4,
    ///Select gop output to IP0
    E_GOP_IP0_MUX,
    /// Select gop output to IP1
    E_GOP_IP1_MUX,
    /// Select gop output to vop0
    E_GOP_VOP0_MUX,
    /// Select gop output to vop1
    E_GOP_VOP1_MUX,
    /// Select gop output to FRC mux0
    E_GOP_FRC_MUX0,
    /// Select gop output to FRC mux1
    E_GOP_FRC_MUX1,
    /// Select gop output to FRC mux2
    E_GOP_FRC_MUX2,
    /// Select gop output to FRC mux3
    E_GOP_FRC_MUX3,
    /// Select gop output to BYPASS mux
    E_GOP_BYPASS_MUX0,
    /// Select gop output to IP0_SUB
    E_GOP_IP0_SUB_MUX,
    /// Select gop output to VOP0_SUB
    E_GOP_VOP0_SUB_MUX,
    /// Select gop output to OP1
    E_GOP_OP1_MUX,
    /// Select gop output to DIP
    E_GOP_DIP_MUX,
    /// Select gop output to DIP
    E_GOP_GS_MUX,
    /// Select gop output to Mix layer0
    E_GOP_Mix_MUX0,
    /// Select gop output to Mix layer1
    E_GOP_Mix_MUX1,
    /// Select gop output to VE0
    E_GOP_VE0_MUX,
    /// Select gop output to VE1
    E_GOP_VE1_MUX,

    ///Select gop output to dual rate op mux0
    E_GOP_DUALRATE_OP_MUX0,
    ///Select gop output to dual rate op mux1
    E_GOP_DUALRATE_OP_MUX1,
    ///Select gop output to dual rate op mux2
    E_GOP_DUALRATE_OP_MUX2,

    MAX_GOP_MUX_SUPPORT,
    E_GOP_MUX_INVAILD,
} Gop_MuxSel;

/// Define GOP destination displayplane type
typedef enum
{
    E_DRV_GOP_DST_IP0       =   0,
    E_DRV_GOP_DST_IP0_SUB   =   1,
    E_DRV_GOP_DST_MIXER2VE  =   2,
    E_DRV_GOP_DST_OP0       =   3,
    E_DRV_GOP_DST_VOP       =   4,
    E_DRV_GOP_DST_IP1       =   5,
    E_DRV_GOP_DST_IP1_SUB   =   6,
    E_DRV_GOP_DST_MIXER2OP  =   7,
    E_DRV_GOP_DST_VOP_SUB   =   8,
    E_DRV_GOP_DST_FRC       =   9,
    E_DRV_GOP_DST_VE        =   10,
    E_DRV_GOP_DST_BYPASS    =   11,
    E_DRV_GOP_DST_OP1       =   12,
    E_DRV_GOP_DST_MIXER2OP1 =  13,
    E_DRV_GOP_DST_DIP       =   14,
    E_DRV_GOP_DST_GOPScaling  = 15,
    E_DRV_GOP_DST_OP_DUAL_RATE   = 16,
    MAX_DRV_GOP_DST_SUPPORT ,
    E_DRV_GOP_DST_INVALID  ,
} DRV_GOPDstType;

/// Define GOP chip property for different chip characteristic.
typedef struct __attribute__((packed))
{
    MS_BOOL bSetHSyncInverse;
    MS_BOOL bGop1GPalette;
    MS_BOOL bSetHPipeOfst;
    MS_BOOL bNeedCheckMVOP;
    MS_BOOL bNeedSetMUX1ToIP0;
    MS_BOOL bNeedSetMUX3ToIP0;
    MS_BOOL bNewMux;
    MS_BOOL bNewPalette;
    MS_BOOL bNewBwReg;
    MS_BOOL bGop2VStretch;
    MS_BOOL bIgnoreIPHPD;
    MS_BOOL bIgnoreIP1HPD;
    MS_BOOL bIgnoreVEHPD;
    MS_BOOL bhastilemode;
    MS_BOOL bInitNotEnableGOPToSC;
    MS_BOOL bXCDirrectBankSupport;
    MS_BOOL bTstPatternAlpha;
    MS_BOOL bFRCSupport;
    MS_BOOL bGOPMixerToVE;
    MS_BOOL bBnkForceWrite;
    MS_BOOL bPixelModeSupport;
    MS_BOOL bDWINSupport;
    MS_BOOL bScalingDownSupport;
    MS_BOOL b2Pto1PSupport;
    MS_BOOL bTLBSupport[6];
    MS_BOOL bInternalMIUSelect[8];
    MS_BOOL bAFBC_Support[6];

    MS_BOOL bAutoAdjustMirrorHSize;
    MS_BOOL bGOPWithVscale[6];
    MS_BOOL bGOPWithScaleDown[6];

    MS_U16  GOP_TestPattern_Vaild;
    MS_U16 DwinVer;
    MS_S16 GOP_PD;
    MS_U16 GOP_IP_PD;
    MS_U16 GOP_MVOP_PD;
    MS_U16 GOP_VE_PD;
    MS_U16 GOP_OP1_PD;
    MS_U16 GOP_MIXER_PD;
    MS_U16 GOP_NonVS_PD_Offset;
    MS_U16 GOP_NonVS_DualOpPD_Offset;
    MS_U16 GOP_VE_V_Offset;
    MS_U16 GOP_HDR_OP_PD;
    MS_U16 GOP_DUAL_OP_PD;

    MS_U16 GOP_MUX_Delta;
    MS_U16 GOP_Mux_Offset[6];
    MS_U16 GOP_Zorder_Mux_Offset[6];
    Gop_MuxSel GOP_MapLayer2Mux[6];
    Gop_MuxSel GOP_MapLayer2DualOpMux[6];
    MS_U16 GOP_Mux_FRC_offset;
    MS_U16 GOP_DualRateMux_Offset[6];
    MS_U16 GOP_REF_POSITION_X;

    MS_U16 WordUnit;
    MS_U16 TotalGwinNum;
    DRV_GOP_CONSALPHA_BITS Default_ConsAlpha_bits;
    EN_DRV_GOP_3D_TYPE enGOP3DType;
    MS_BOOL bOpInterlace;

    MS_BOOL bAFBCMIUSelDoubleBuffer;
    MS_BOOL bAFBC_Merge_GOP_Trig;
    MS_BOOL bGOPVscalePipeDelay[6];
    MS_BOOL bGOPNeedSplitMode4DualRate[6];
    MS_BOOL bOPHandShakeModeSupport;
    MS_BOOL bOPMuxDoubleBuffer;
    MS_BOOL bGOPAutoClkGating;
}GOP_CHIP_PROPERTY;

/// GOP0 gwin priority
typedef struct DLL_PACKED
{
    MS_U8 u81stGwin;
    MS_U8 u82stGwin;
    MS_U8 u83stGwin;
    MS_U8 u84stGwin;
}GOP0_GwinPri;

// GOP0/GOP1 gwin priority
typedef struct DLL_PACKED
{
    GOP0_GwinPri Gop0_GwinPri;
    MS_U8 Gop1_1stGwin  :1;
}GOP_GwinPriority;

/// GWIN Information
typedef struct DLL_PACKED
{
    MS_U16 u16DispVPixelStart;      //!< unit: pix
    MS_U16 u16DispVPixelEnd;        //!< unit: pix
    MS_U16 u16DispHPixelStart;      //!< unit: pix
    MS_U16 u16DispHPixelEnd;        //!< unit: pix
    MS_PHY u64DRAMRBlkStart;        //!< unit: Byte
    MS_U16 u16RBlkHPixSize;     //!< unit: pix
    MS_U16 u16RBlkVPixSize;     //!< unit: pix
    MS_U16 u16RBlkHRblkSize;       //!< unit: Byte
    MS_U16 u16WinX;             //!< unit: pix
    MS_U16 u16WinY;             //!< unit: pix
    MS_U32 u32scrX;             //!< unit: pix
    MS_U32 u32scrY;             //!< unit: pix
    DRV_GOPColorType clrType;       //!< color format of the buffer
} DRV_GOP_GWIN_INFO;

/// Dump Window Information
typedef struct DLL_PACKED
{
    MS_U16 u16VPixelStart;      //!< unit: pix
    MS_U16 u16VPixelEnd;        //!< unit: pix
    MS_U16 u16HPixelStart;      //!< unit: pix
    MS_U16 u16HPixelEnd;        //!< unit: pix
    MS_PHY u64TFDRAMAddr;       //!< unit: Byte
    MS_PHY u64BFDRAMAddr;       //!< unit: Byte
    MS_U16 u16DRAMJumpLen;      //!< unit: Byte
    MS_U8 u8fmt;                //!< DWIN format: 0 UV7Y8, 1 UV8Y8, 2 ARGB8888, 3 RGB565
} DRV_GOP_DWIN_INFO;

/// DWIN propert
typedef struct DLL_PACKED
{
    MS_U16 u16x;           ///< x
    MS_U16 u16y;           ///< y
    MS_U16 u16w;           ///< width
    MS_U16 u16h;           ///< height
    MS_PHY u64fbaddr0;     ///< Top field DRAM address
    MS_PHY u64fbaddr1;     ///< Bottom field DRAM address
    MS_U16 u16fbw;         ///< Number of bytes per horizontal line
} DRV_GOP_DwinProperty;

typedef union DLL_PACKED
{
    MS_U8 u8DWinIntInfo;
    struct DLL_PACKED
    {
        MS_U8 bDWinIntWADR:1;
        MS_U8 bDWinIntPROG:1;
        MS_U8 bDWinIntTF:1;
        MS_U8 bDWinIntBF:1;
        MS_U8 bDWinIntVS:1;
        MS_U8 reserved:3;
    }sDwinIntInfo;
} DRV_GOP_DWinIntInfo;

/// Frame buffer attributes for GWIN
typedef struct DLL_PACKED
{
    MS_U8 gWinId;          ///< id
    MS_U8 enable:1;        ///< 4bits enable
    MS_U8 allocated:1;    ///< 4bits

    MS_U16 x0;                 ///< MS_U16
    MS_U16 y0;                 ///< MS_U16
    MS_U16 x1;                 ///< MS_U16
    MS_U16 y1;                 ///< MS_U16
    MS_U16 width;                 ///< MS_U16
    MS_U16 height;                 ///< MS_U16
    MS_U16 pitch;                    ///< MS_U16
    MS_U16 fbFmt;                 ///< MS_U16

    MS_U32 addr;                     ///< MS_U32
    MS_U32 size;                     ///< MS_U32

    MS_U8 next;                 ///< MS_U8
    MS_U8 prev;                  ///< MS_U8
    MS_U8 string;               //store which AP use this FB
    //MS_U8 flag;                ///< bit 0:scroll
    MS_U16 s_x;                ///< MS_U16
    MS_U16 s_y;                ///< MS_U16
    MS_U16 dispWidth;          ///< MS_U16
    MS_U16 dispHeight;         ///< MS_U16
} DRV_Gwin_FB_Attr;

/// OC Info
typedef struct DLL_PACKED
{
    MS_PHY u64GOP_OC_DRAM_RBLK;         ///Bytes
    MS_PHY u64GOP_OC_ADDRPROTECT_STR;   ///Bytes
    MS_PHY u64GOP_OC_ADDRPROTECT_END;   ///Bytes
    MS_U32 u32GOP_OC_FETCH;
    MS_U16 u16GOP_OC_HSIZE;
    MS_U16 u16GOP_OC_VSIZE;
    MS_BOOL bGOP_OC_MIUID;
    MS_U8 u8GOP_OC_BANK_MODE;
} DRV_GOP_OC_INFO;

/// GWIN output color domain
typedef enum
{
    DRV_GOPOUT_RGB,     ///< 0: output color RGB
    DRV_GOPOUT_YUV,     ///< 1: output color YUV
} GOP_OupputColor;

/// Transparent color format
typedef enum
{
    DRV_GOPTRANSCLR_FMT0,   //!< RGB mode transparent color
    DRV_GOPTRANSCLR_FMT1,   //!< index mode transparent color
    DRV_GOPTRANSCLR_FMT2,   //!< ARGB 8888 transparent color
    DRV_GOPTRANSCLR_FMT3,   //!< YUV mode transparent color
} GOP_TransClrFmt;

/// Define Scaler GOP IP setting.
typedef enum
{
    MS_DRV_IP0_SEL_GOP0,
    MS_DRV_IP0_SEL_GOP1,
    MS_DRV_IP0_SEL_GOP2,
    MS_DRV_NIP_SEL_GOP0,
    MS_DRV_NIP_SEL_GOP1,
    MS_DRV_NIP_SEL_GOP2,
    MS_DRV_MVOP_SEL,
    MS_DRV_SUBMVOP_SEL,
    MS_DRV_IP1_SEL,
    MS_DRV_SUBIP0_SEL,
    MS_DRV_SC1OP_SEL,
    MS_DRV_DIP_SEL,
}MS_IPSEL_GOP;

typedef enum
{
    E_DRV_MIXER_DST_IP       =   0,
    E_DRV_MIXER_DST_MIXER    =   1,
    E_DRV_MIXER_DST_OP       =   2,
    E_DRV_MIXER_DST_VE       =   3,
    E_DRV_MIXER_DST_OP1      =   4,
    MAX_DRV_MIXER_DST_SUPPORT      ,
    E_DRV_MIXER_DST_INVALID   =   0xFF,
} DRV_MixerDstType;

/// Define GOP H-Stretch mode.
typedef enum
{
    E_DRV_GOP_HSTRCH_6TAPE = 0,
    E_DRV_GOP_HSTRCH_DUPLICATE = 1,
    E_DRV_GOP_HSTRCH_6TAPE_LINEAR  = 2,
    E_DRV_GOP_HSTRCH_6TAPE_NEAREST = 3,
    E_DRV_GOP_HSTRCH_6TAPE_GAIN0   = 4,
    E_DRV_GOP_HSTRCH_6TAPE_GAIN1   = 5,
    E_DRV_GOP_HSTRCH_6TAPE_GAIN2   = 6,
    E_DRV_GOP_HSTRCH_6TAPE_GAIN3   = 7,
    E_DRV_GOP_HSTRCH_6TAPE_GAIN4   = 8,
    E_DRV_GOP_HSTRCH_6TAPE_GAIN5   = 9,
    E_DRV_GOP_HSTRCH_4TAPE         = 0xA,
    E_DRV_GOP_HSTRCH_2TAPE         = 0xB,
    MAX_GOP_HSTRETCH_MODE_SUPPORT,
} DRV_GOPStrchHMode;

/// Define GOP V-Stretch mode.
typedef enum
{
    E_DRV_GOP_VSTRCH_LINEAR = 0,
    E_DRV_GOP_VSTRCH_DUPLICATE = 1,
    E_DRV_GOP_VSTRCH_NEAREST = 2,
    E_DRV_GOP_VSTRCH_LINEAR_GAIN0 = 3,
    E_DRV_GOP_VSTRCH_LINEAR_GAIN1 = 4,
    E_DRV_GOP_VSTRCH_LINEAR_GAIN2 = 5,
    E_DRV_GOP_VSTRCH_4TAP = 6,
    MAX_GOP_VSTRETCH_MODE_SUPPORT,
} DRV_GOPStrchVMode;

/// Define GOP Transparent color Stretch mode.
typedef enum
{
    E_DRV_GOP_TRANSPCOLOR_STRCH_DUPLICATE = 0,
    E_DRV_GOP_TRANSPCOLOR_STRCH_ASNORMAL = 1,
    MAX_GOP_TRANSPCOLOR_STRETCH_MODE_SUPPORT,
} DRV_GOPSTRCH_TRANSPCOLORMODE;

/// Define palette read type.
typedef enum
{
    /// Palette access from GOP0 RIU
    E_DRV_GOP_PAL_GOP_RIU    = 0,
    /// Palette access from GOP0 REGDMA
    E_DRV_GOP_PAL_GOP_REGDMA  = 1,
    /// Palette access from GOP1 RIU
    E_DRV_GOP1_PAL_GOP_RIU    = 2,
    /// Palette access from GOP1 REGDMA
    E_DRV_GOP1_PAL_GOP_REGDMA  = 3,
} DRV_GopPalReadType;

/// Scroll direction
typedef enum
{
    GOP_DRV_SCROLL_NONE = 0,
    GOP_DRV_SCROLL_UP,      // bottom to top
    GOP_DRV_SCROLL_DOWN,    // top to bottom
    GOP_DRV_SCROLL_LEFT,            //right to left
    GOP_DRV_SCROLL_RIGHT,   //left to right
    GOP_DRV_SCROLL_SW,          // Scroll by sw
    GOP_DRV_SCROLL_KERNEL,      // Scroll by sw on kernel
    GOP_DRV_SCROLL_MAX,
} E_GOP_SCROLL_TYPE;

/// DWIN capture data source
typedef enum
{
    GOPDWIN_DATA_SRC_SCALAR,    //!< scaler output
    GOPDWIN_DATA_SRC_EXTRGB,        //!< External, RGB, YUV(pass R2Y)
    GOPDWIN_DATA_SRC_VOP,       //!< DVB VOP (MVOP)
    GOPDWIN_DATA_SRC_MAX
} EN_GOP_DWIN_DATA_SRC;

/// DWIN scan mode
typedef enum
{
    GOPDWIN_SCAN_MODE_PROGRESSIVE,  ///< Progressive scan
    GOPDWIN_SCAN_MODE_INTERLACE,  ///< Interlace scan
    GOPDWIN_SCAN_MODE_MAX
} EN_GOP_DWIN_SCAN_TYPE;

/// DWIN source selection
typedef enum
{
    /// DWIN OP source
    GOP_DRV_DWIN_SRC_OP = 0,
    /// DWIN MVOP source
    GOP_DRV_DWIN_SRC_MVOP = 1,
    /// DWIN IP source
    GOP_DRV_DWIN_SRC_IP = 2,
    /// DWIN OP1 source
    GOP_DRV_DWIN_SRC_OP1 =3,
    /// DWIN SUB MVOP source
    GOP_DRV_DWIN_SRC_SUBMVOP =4,
    /// DWIN GOPScaling source
    GOP_DRV_DWIN_SRC_GOPScaling =5,
    GOP_DRV_DWIN_SRC_MAX = 6
}DRV_GOP_DWIN_SRC_SEL;

/// DWIN source data format
typedef enum
{
    GOPDWIN_DATA_FMT_UV7Y8 = 0,     //!< CrYCb domain
    GOPDWIN_DATA_FMT_UV8Y8,         //!< CrYCb domain
    GOPDWIN_DATA_FMT_ARGB8888,      //!< RGB domain (CSC bypass)
    GOPDWIN_DATA_FMT_RGB565,        //!< RGB domain (CSC bypass)
    GOPDWIN_DATA_FMT_MAX
} DRV_GOP_DWIN_DATA_FMT;

/// Define palette color format.
typedef enum
{
    /// Palette color format is RGB888.
    E_DRV_GOP_PAL_ARGB8888    = 0,
    /// Invalid palette color format.
    E_DRV_GOP_PAL_INVALID
} DRV_GopPalType;

/// Define GOP gwin fade in/out.
typedef enum
{
    /// GOP gwin fade in.
    E_DRV_GOP_GWIN_FADE_IN,
    /// GOP gwin fade out.
    E_DRV_GOP_GWIN_FADE_OUT,
    /// GOP gwin fade unknow type.
    E_DRV_GOP_GWIN_FADE_UNKNOWN,
} DRV_GOP_FADE_TYPE;

//----------------------------------------------------------------------------
// GOP Reg Page Form
//----------------------------------------------------------------------------
typedef enum
{
    E_GOP_REG_FORM_4G   = 0x0000,
    E_GOP_REG_FORM_T21G = 0x0001,
    E_GOP_REG_FORM_2G   = 0x0002,
    E_GOP_REG_FORM_T81G = 0x0003,
    E_GOP_REG_FORM_U42G = 0x0004,
    E_GOP_REG_FORM_NONE = 0x000F,
    E_GOP_REG_FORM_MASK = 0x000F,
}   E_GOP_REG_FORM;

typedef enum
{
    E_GOPD_FIFO_DEPTH_64   = 0x0000,
    E_GOPD_FIFO_DEPTH_128  = 0x0010,
    E_GOPD_FIFO_DEPTH_256  = 0x0020,
    E_GOPD_FIFO_DEPTH_MASK = 0x00F0,
}   E_GOPD_FIFO_DEPTH;

// GOP palette type
typedef enum
{
    E_GOP_PAL_SIZE_NONE = 0x0000,
    E_GOP_PAL_SIZE_256  = 0x0100,
    E_GOP_PAL_SIZE_64   = 0x0200,
    E_GOP_PAL_SIZE_MASK = 0x0F00,
}   E_GOP_PAL_SIZE;

/// GOP VE timing type
typedef enum
{
    E_GOP_VE_NTSC ,
    E_GOP_VE_PAL,
    E_GOP_VE_TIMEING_MAX,
}E_GOP_VE_TIMINGTYPE;

/// GOP MIXER timing type
typedef enum
{
    E_GOP_NTSC ,
    E_GOP_PAL,
    E_GOP_CUSTOM,
    E_GOP_CUSTOM_OP,
    E_GOP_MIXER_TIMEING_MAX,
}E_GOP_MIXER_TIMINGTYPE;

/// video timing mirror type
typedef enum
{
    E_GOP_VIDEOTIMING_MIRROR_BYSCALER,
    E_GOP_VIDEOTIMING_MIRROR_BYMVOP,
    E_GOP_VIDEOTIMING_MIRROR_MAX,
}E_GOP_VIDEOTIMING_MIRRORTYPE;


/// Define GOP burst length.
typedef enum
{
    /// burst length: long
    E_GOP_BURSTLEN_LONG,
    /// burst length: middle
    E_GOP_BURSTLEN_MIDDLE,
    /// burst length: short
    E_GOP_BURSTLEN_SHORT,
} EN_GOP_BURST_LEN;

/// Define GOP G3D DATA FORMAT.
typedef enum
{
    /// E_GOP_TILE_DATA_16BPP. 16 BPP DATA FOR TILE MODE
    E_DRV_GOP_TILE_DATA_16BPP    = 0,
    /// E_GOP_TILE_DATA_16BPP. 32 BPP DATA FOR TILE MODE
    E_DRV_GOP_TILE_DATA_32BPP    = 1,
} E_GOP_TILE_DATA_TYPE;

/// Define GOP MIU SEL
typedef enum
{
    /// E_DRV_GOP_SEL_MIU0. gop access miu 0
    E_DRV_GOP_SEL_MIU0    = 0,
    /// E_GOP_SEL_MIU1. gop access miu1
    E_DRV_GOP_SEL_MIU1    = 1,
    /// E_GOP_SEL_MIU2. gop access miu2
    E_DRV_GOP_SEL_MIU2    = 2,
    /// E_GOP_SEL_MIU3. gop access miu3
    E_DRV_GOP_SEL_MIU3    = 3,
} E_DRV_GOP_SEL_TYPE;

/// Define GOP PINPON Mode
typedef enum
{
    /// E_DRV_GOP_PINPON_G3D. gop set g3d pinpon
    E_DRV_GOP_PINPON_G3D    = 0,
    /// E_DRV_GOP_PINPON_VE. gop set ve pinpon
    E_DRV_GOP_PINPON_VE     = 1,
    /// E_DRV_GOP_PINPON_DWIN. gop set dwin pinpon
    E_DRV_GOP_PINPON_DWIN   = 2,
    /// E_DRV_GOP_PINPON_DIP. gop set dwin pinpon
    E_DRV_GOP_PINPON_DIP   = 3,
    /// E_DRV_GOP_PINPON_DWIN0. gop set dwin pinpon
    E_DRV_GOP_PINPON_DWIN0   = 4,
    /// E_DRV_GOP_PINPON_DWIN1. gop set dwin pinpon
    E_DRV_GOP_PINPON_DWIN1   = 5,

} E_DRV_GOP_PINPON_MODE;



/// Define the entry of palette.
typedef union DLL_PACKED
{
    /// ARGB888
    struct DLL_PACKED
    {
        MS_U8 u8A;
        MS_U8 u8R;
        MS_U8 u8G;
        MS_U8 u8B;
    } RGB;
    /// 32-bit direct access.
    MS_U8 u8Data[4];
} DRV_GopPaletteEntry;

typedef enum
{
   GOP_WINFB_POOL_NULL = 0,
   GOP_WINFB_POOL1 = 1,
   GOP_WINFB_POOL2= 2
}GOP_FBPOOL_ID;

typedef enum
{
    E_DRV_GOP_AFBC_SPILT            =0x1,
    E_DRV_GOP_AFBC_YUV_TRANSFER	    =0x2,
    E_DRV_GOP_AFBC_ARGB8888         =0x4,
    E_DRV_GOP_AFBC_MAX,
}EN_DRV_GOP_AFBC_CNTL;

/// Define GOP FB string.
typedef enum
{
    E_DRV_GOP_FB_NULL,
    /// OSD frame buffer.
    E_DRV_GOP_FB_OSD,
    /// Mute frame buffer.
    E_DRV_GOP_FB_MUTE,
    /// subtitle frame buffer.
    E_DRV_GOP_FB_SUBTITLE,
    /// teltext frame buffer.
    E_DRV_GOP_FB_TELTEXT,
    /// MHEG5 frame buffer.
    E_DRV_GOP_FB_MHEG5,
    /// CC frame buffer.
    E_DRV_GOP_FB_CLOSEDCAPTION,
    /// AFBC frame buffer.
    E_DRV_GOP_FB_AFBC_SPLT_YUVTRNSFER_ARGB8888 = 0x100,
    E_DRV_GOP_FB_AFBC_NONSPLT_YUVTRS_ARGB8888,
    E_DRV_GOP_FB_AFBC_SPLT_NONYUVTRS_ARGB8888,
    E_DRV_GOP_FB_AFBC_NONSPLT_NONYUVTRS_ARGB8888,
} EN_DRV_FRAMEBUFFER_STRING;

/// Frame buffer attributes for GWIN
typedef struct __attribute__((packed))
{
    MS_PHY addr;                     ///< MS_U64  /*For 32/64 share memory align*/
    MS_U32 size;                     ///< MS_U32
    MS_U32 nextFBIdInPool;           ///next winId in same pool

    MS_U16 x0;                 ///< MS_U16
    MS_U16 y0;                 ///< MS_U16
    MS_U16 x1;                 ///< MS_U16
    MS_U16 y1;                 ///< MS_U16
    MS_U16 width;                 ///< MS_U16
    MS_U16 height;                 ///< MS_U16
    MS_U16 pitch;                     ///< MS_U16
    MS_U16 fbFmt;                 ///< MS_U16

    MS_U16 s_x;                ///< MS_U16
    MS_U16 s_y;                ///< MS_U16
    MS_U16 dispWidth;          ///< MS_U16
    MS_U16 dispHeight;         ///< MS_U16

    GOP_FBPOOL_ID poolId;

    MS_U8 gWinId;
    MS_U8 string;               ///store which AP use this FB
    MS_U8 enable:1;
    MS_U8 in_use:1;
    MS_U32 u32GOPClientId;     ///<<store Client ID of using this FB
    MS_U8 obtain;
    EN_DRV_FRAMEBUFFER_STRING FBString;
} GOP_WinFB_INFO;

typedef struct __attribute__((packed))
{
   MS_U32 PalSWArray[GOP_PALETTE_ENTRY_NUM_MAX];
}GOP_Palette_INFO;

typedef struct __attribute__((packed))
{
   GOP_FBPOOL_ID poolId;
   MS_PHY GWinFB_Pool_BaseAddr;
   MS_U32 u32GWinFB_Pool_MemLen;
   MS_U32 u32FirstFBIdInPool;
}GOP_FB_POOL;

typedef struct __attribute__((packed))
{
    MS_U32 u32GOPClientId;
    MS_U32 u32CurFBId;
    MS_U16 u16SharedCnt;
    MS_BOOL bIsShared;
}GWIN_MAP;

typedef struct DLL_PACKED
{
    MS_PHY u64NonMirrorFBAdr;
    MS_U16 u16NonMirrorHStr;
    MS_U16 u16NonMirrorHEnd;
    MS_U16 u16NonMirrorVStr;
    MS_U16 u16NonMirrorVEnd;
    MS_U16 u16NonMirrorGOP0WinX;
    MS_U16 u16NonMirrorGOP0WinY;
}GOP_GwinMirror_Info;

///the GOP MIXER timing info
typedef struct DLL_PACKED
{
    MS_U32 hstart;
    MS_U32 hend;
    MS_U32 vstart;
    MS_U32 vend;
    MS_U32 htotal;
    MS_U32 hsyncdelay;
}GOP_DRV_MixerTiming;

///GOP Ignore init value
typedef enum
{
    E_DRV_GOP_MUX_IGNORE              =   0x0001,
    E_DRV_GOP_GWIN_IGNORE             =   0x0002,
    E_DRV_GOP_STRETCHWIN_IGNORE       =   0x0004,
    E_DRV_GOP_PALETTE_IGNORE          =   0x0008,
    E_DRV_GOP_SET_DST_OP_IGNORE       =   0x0010,
    E_DRV_GOP_ENABLE_TRANSCLR_IGNORE  =   0x0020,
    E_DRV_GOP_ALL_IGNORE              =   0xFFFF,
} DRV_GOP_IGNOREINIT;

///the GOP config
typedef struct DLL_PACKED
{
  DRV_GOP_IGNOREINIT eGopIgnoreInit;
}GOP_Config;

typedef struct __attribute__((packed))
{
        MS_U32 u32ClientIdAllocator;
        MS_U32 u32LstGOPClientId;
        MS_U16 u16PnlWidth[SHARED_GOP_MAX_COUNT];
        MS_U16 u16PnlHeight[SHARED_GOP_MAX_COUNT];
        MS_U16 u16PnlHStr[SHARED_GOP_MAX_COUNT];
        MS_BOOL bHMirror;
        MS_BOOL bVMirror;
        MS_BOOL b16TileMode[SHARED_GOP_MAX_COUNT];
        MS_BOOL b32TileMode[SHARED_GOP_MAX_COUNT];
        MS_U16  ScrollBufNum;
        MS_U16  ScrollBufIdx;
        MS_U32  ScrollBuffer[MAX_GOP_SW_SCROLLNUM];

        GWIN_MAP gwinMap[SHARED_GWIN_MAX_COUNT];

        GOP_FB_POOL fbPool1;
        GOP_FB_POOL fbPool2;
        GOP_FBPOOL_ID fb_currentPoolId;

        MS_S32 s32OutputColorType[SHARED_GOP_MAX_COUNT];
        MS_BOOL bGopHasInitialized[SHARED_GOP_MAX_COUNT];
        MS_BOOL bGWINEnable[SHARED_GWIN_MAX_COUNT];
#if WINFB_INSHARED
        GOP_WinFB_INFO winFB[DRV_MAX_GWIN_FB_SUPPORT+1];
#endif
        MS_U8 GOP_StereoMode[SHARED_GOP_MAX_COUNT];
        MS_U16 u16APIStretchWidth[SHARED_GOP_MAX_COUNT]; //Store API set stretch win width(May Not sync with HW register in GOP 3D SBS mode)
        MS_U16 u16APIStretchHeight[SHARED_GOP_MAX_COUNT];
        MS_U8 GOP_TestPatternMode;
        MS_BOOL bGOP3DLRSwitch[SHARED_GOP_MAX_COUNT]; //GOP 3D L/R Switch flag
        MS_U16 u16HScaleDst[SHARED_GOP_MAX_COUNT];
        MS_U16 u16VScaleDst[SHARED_GOP_MAX_COUNT];
        MS_BOOL bPixelMode[SHARED_GOP_MAX_COUNT];
        MS_BOOL bInitShared;
        MS_BOOL bDummyInit;//TRUE: the gop init's input parameter is NULL when SHM is firstly initialized
        MS_BOOL bEnableVsyncIntFlip;
        MS_S32 s32GOPLockTid;
        MS_U32  GOPLockPid;
        MS_BOOL bTLB[SHARED_GOP_MAX_COUNT];
        MS_PHY  u64TLBAddress[SHARED_GOP_MAX_COUNT];
        GOP_Palette_INFO GOPPaletteInfo[SHARED_GOP_MAX_COUNT];
#ifdef CONFIG_MSTAR_SRAMPD
        MS_BOOL bGOPPowerDown[SHARED_GOP_MAX_COUNT];
#endif
        MS_PHY phyGOPRegdmaAdr;
        MS_U32 u32GOPRegdmaLen;
} MS_GOP_CTX_SHARED;

typedef struct DLL_PACKED
{
    MS_GOP_CTX_SHARED *pGOPCtxShared;//pointer to shared context paramemetrs;
    GOP_CHIP_PROPERTY *pGopChipProperty;
    DRV_GOP_CONSALPHA_BITS *pGopConsAlphaBits;
    GOP_Config sGOPConfig;  //GOP configs set for special cases
    MS_U32 u32GOPClientId;
#if GOP_LOCK_SUPPORT
    MS_S32      s32GOPMutex;
    MS_S32      s32GOPLock;
    MS_S32      s32GOPMutexCnt;
#endif
#if GOP_XC_LOCK_SUPPORT
    MS_S32      s32GOPXC_Mutex;
    MS_S32      s32GOPXC_Lock;
    MS_S32      s32GOPXC_MutexCnt;
#endif
    //for local GWIN memory pool

#if WINFB_INSHARED
    //GOP_WinFB_INFO winFB[MAX_GWIN_FB_SUPPORT+1]; //move to MS_GOP_CTX_SHARED
#else
    GOP_WinFB_INFO winFB[DRV_MAX_GWIN_FB_SUPPORT+1];
#endif
    MS_U8 current_winId;


    MS_BOOL Gwin_H_Dup, Gwin_V_Dup;
    MS_U16 u16GOP_HMirrorRBLK_Adr;   //Consider 16Gwins totally
    MS_U16 u16GOP_VMirrorRBLK_Adr;   //Consider 16Gwins totally
    MS_U16 u16GOP_HMirror_HPos;      //Consider 16Gwins totally
    MS_U16 u16GOP_VMirror_VPos;      //Consider 16Gwins totally

    GOP_GwinMirror_Info* sMirrorInfo;
    MS_U8 MS_MuxGop[MAX_GOP_MUX_SUPPORT];
    MS_U8 u8ChgIpMuxGop;
    MS_BOOL IsChgMux;
    MS_BOOL IsClkClosed;
    MS_BOOL bUpdateRegOnce[SHARED_GOP_MAX_COUNT];
#ifdef CONFIG_GOP_TEST_PATTERN
    MS_U16 GOPBnk[SHARED_GOP_MAX_COUNT][GOP_REG_MAX];
    MS_U16 GWINBnk[SHARED_GOP_MAX_COUNT][GOP_REG_MAX];
    MS_U16 GOPStBnk[SHARED_GOP_MAX_COUNT][GOP_REG_MAX];
#endif
    MS_U16 GOP_FramePacking_Gap;
    MS_U16 u16GOPGWinMiddle[SHARED_GOP_MAX_COUNT];

    MS_BOOL bInitFWR;
}MS_GOP_CTX_LOCAL;

typedef struct DLL_PACKED
{
    MS_BOOL bEnable; //InOut
    MS_U8   u8FrameCount; //Out, value range: 0~3
    MS_U8   u8Result; //Out
}MS_DRVGOP_VECAPTURESTATE, *PMS_DRVGOP_VECAPTURESTATE;


//3D OSD mode type
typedef enum
{
    E_DRV_GOP_3D_DISABLE ,
    E_DRV_GOP_3D_SWITH_BY_FRAME,
    E_DRV_GOP_3D_SIDE_BY_SYDE,
    E_DRV_GOP_3D_TOP_BOTTOM,
    E_DRV_GOP_3D_LINE_ALTERNATIVE,
    E_DRV_GOP_3D_LINE_FRAMEPACKING,
}EN_DRV_GOP_3D_MODE;

typedef enum
{
    E_DRV_GOP_Deldirect_Front ,
    E_DRV_GOP_Deldirect_Back,
}E_DRV_GOP_DelDirect;


typedef enum
{
    E_DRV_GOP_MAIN_WINDOW=0,          ///< main window if with PIP or without PIP
    E_DRV_GOP_SUB_WINDOW=1,           ///< sub window if PIP
    E_DRV_GOP_MAX_WINDOW              /// The max support window
}E_DRV_GOP_SCALER_WIN;

///Dwin Capability of IC
typedef struct DLL_PACKED
{
    MS_BOOL bSupportWindowDECapture; //TRUE: Support it; FALSE:Not support, should use FrameDE to capture video
    MS_BOOL bSupportOSDCapture;      //TRUE: Support it; FALSE:Not support
}DRV_GOP_DWIN_CAP, *PDRV_GOP_DWIN_CAP;

///GOP MUX Caps
typedef struct DLL_PACKED
{
    MS_U8 u8MuxNumber;
    MS_U8 *pu8MuxProrityArray;
    MS_BOOL bNewMux;
    MS_BOOL bNeedSetMUX1ToIP0;
    MS_BOOL bNeedSetMUX3ToIP0;
}ST_DRV_GOP_MUX_CAP, *PST_DRV_GOP_MUX_CAP;

///GOP MUX Caps
typedef struct
{
    MS_U8 u8MuxNumber;
    MS_U8 pu8MuxProrityArray[MAX_GOP_INFO_COUNT];
    MS_BOOL bNewMux;
    MS_BOOL bNeedSetMUX1ToIP0;
    MS_BOOL bNeedSetMUX3ToIP0;
}ST_DRV_GOP_MUX_CAP_EX, *PST_DRV_GOP_MUX_CAP_EX;

typedef enum
{
    E_DRV_GOP_BNK_WRITE =0,    //restore the bank register
    E_DRV_GOP_BNK_READ,        //back the bank register
}E_DRV_GOP_BNK_STATUS;

typedef enum
{
    E_DRV_GOP_GPU_TILE_LINEAR =0,
    E_DRV_GOP_GPU_TILE_DIP8X,
    E_DRV_GOP_GPU_TILE_DIP16X,
    E_DRV_GOP_GPU_TILE_GPU,
}EN_DRV_GOP_GPU_TILE_MODE;

typedef enum
{
    EN_GOP_CROP_DISABLE = 0,
    EN_GOP_CROP_ENABLE = 1,
} EN_GOP_CROP_CTL;

///  Test Pattern
typedef struct DLL_PACKED
{
    /// TSTClr_en
    MS_BOOL bTSTClr_En;
    /// TSTClr_Alpha_En
    MS_BOOL bTSTClr_Alpha_En;

    ///TSTClr_Alpha
    MS_U8 u8TSTClr_Alpha;
    /// R_stc
    MS_U8 u8R_stc;
    /// G_stc.
    MS_U8 u8G_stc;
    /// B_stc.
    MS_U8 u8B_stc;

    /// TSTClr_hdup
    MS_U8 u8TSTClr_Hdup;
    /// TSTClr_vdup.
    MS_U8 u8TSTClr_Vdup;

    /// H_R_inc.
    MS_U8 u8HR_inc;
    /// H_R_inc_signz
    MS_BOOL u8HR_inc_signz;
    /// H_G_inc.
    MS_U8 u8HG_inc;
    /// H_G_inc_signz
    MS_BOOL u8HG_inc_signz;
    /// H_B_inc.
    MS_U8 u8HB_inc;
    /// H_B_inc_signz
    MS_BOOL u8HB_inc_signz;
    /// H_R_step
    MS_U8 u8HR_step;
    /// H_G_stepc
    MS_U8 u8HG_step;
    /// H_B_step.
    MS_U8 u8HB_step;

    /// V_R_inc.
    MS_U8 u8VR_inc;
    /// V_R_inc_signz
    MS_BOOL u8VR_inc_signz;
    /// V_G_inc.
    MS_U8 u8VG_inc;
    /// V_G_inc_signz
    MS_BOOL u8VG_inc_signz;
    /// V_B_inc.
    MS_U8 u8VB_inc;
    /// V_B_inc_signz
    MS_BOOL u8VB_inc_signz;
    /// V_R_step
    MS_U8 u8VR_step;
    /// V_G_step
    MS_U8 u8VG_step;
    /// V_B_step.
    MS_U8 u8VB_step;
} DRV_GOP_TSTPATTERN;


typedef struct DLL_PACKED
{
    MS_U8 GOP0;
    MS_U8 GOP1;
    MS_U8 GOP2;
    MS_U8 GOP3;
    MS_U8 GOP4;
    MS_U8 GOP5;
    MS_U8 DWIN;
    MS_U8 MIXER;
}GOP_TYPE_DEF;

typedef struct DLL_PACKED
{
    MS_PHY u64Addr;
    MS_U16 u16Pitch;
    MS_U16 u16Fmt;
}DRV_GOP_CBFmtInfo;

typedef struct DLL_PACKED
{
    MS_PHY u64DRAMAddr;       //!< unit: Byte
    MS_U16 u16HPixelStart;      //!< unit: pix
    MS_U16 u16HPixelEnd;        //!< unit: pix
    MS_U16 u16VPixelStart;      //!< unit: pix
    MS_U16 u16VPixelEnd;        //!< unit: pix
    MS_U16 u16Pitch;            //!< unit: Byte
    MS_U8  u8Fmt;
}DRV_GOP_AFBC_Info;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//======================================================================================
//  GOP Common utility
//======================================================================================
MS_U8 MDrv_DumpGopByGwinId(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U32 GwinID);
void* MDrv_GOP_GetShareMemory(MS_BOOL *pbNeedInitShared);
GOP_Result MDrv_GOP_Sel(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum);
MS_GOP_CTX_LOCAL*  Drv_GOP_Init_Context(void* pInstance,MS_BOOL *pbNeedInitShared);
void MDrv_GOP_Init(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, MS_PHY u64GOP_REGDMABASE, MS_U32 u32GOP_RegdmaLen, MS_BOOL bEnableVsyncIntFlip);
void MDrv_GOP_DeInit(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum);
void MDrv_GOP_PerClientInit(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, MS_PHY u64GOP_REGDMABASE, MS_U32 u32GOP_RegdmaLen, MS_BOOL bEnableVsyncIntFlip);
void MDrv_GOP_SetRegDMA(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_PHY u64GOP_REGDMABASE, MS_U32 u32GOP_RegdmaLen);
void MDrv_GOP_SetGOPEnable2SC(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 gopNum, MS_BOOL bEnable);
void MDrv_GOP_SetIPSel2SC(MS_GOP_CTX_LOCAL*pGOPCtx, MS_IPSEL_GOP ipSelGop);
void MDrv_GOP_SetClkForCapture(MS_GOP_CTX_LOCAL*pGOPCtx, DRV_GOP_DWIN_SRC_SEL enSrcSel);
void MDrv_GOP_SetVOPNBL(MS_GOP_CTX_LOCAL*pGOPCtx,MS_BOOL bEnable);
void MDrv_GOP_GWIN_UpdateReg(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP);
void MDrv_GOP_GWIN_UpdateRegWithSync(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_BOOL bSync);
void MDrv_GOP_GWIN_UpdateRegWithMaskSync(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U16 u16GopMask, MS_BOOL bSync);
void MDrv_GOP_TriggerRegWriteIn(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_BOOL bForceWriteIn, MS_BOOL bSync);
void MDrv_GOP_Restore_Ctx(MS_GOP_CTX_LOCAL*pGOPCtx);
MS_BOOL MDrv_GetAFBCCapsSupport(MS_GOP_CTX_LOCAL*pGOPCtx);
MS_U8 MDrv_GOP_Get(MS_GOP_CTX_LOCAL*pGOPCtx);
MS_U8 MDrv_GOP_GetGwinNum(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GopNum);
MS_U8 MDrv_GetMaxMuxOPNum(void);
MS_BOOL MDrv_GOP_SetIOMapBase(MS_GOP_CTX_LOCAL*pGOPCtx);
MS_BOOL MDrv_GOP_HasGop1GPalette(MS_GOP_CTX_LOCAL*pGOPCtx);
MS_BOOL MDrv_GOP_SwitchGWIN2Addr(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 gWinId, MS_PHY u64MainAddress, MS_PHY u64SubAddress, MS_U32 u32WaitTagID,  MS_U32 *pU32QueueCnt);
GOP_Result MDrv_GOP_SetGOPClk(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 gopNum, DRV_GOPDstType eDstType);
GOP_Result MDrv_GOP_MIXER_SetGOPEnable2Mixer(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 gopNum, MS_BOOL bEnable);
GOP_Result MDrv_GOP_SetGOPHighPri(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum);
GOP_Result MDrv_GOP_InitMux(MS_GOP_CTX_LOCAL*pGOPCtx);
MS_U8 MDrv_GOP_GetWordUnit(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum);
MS_U16 MDrv_GOP_GetGOPACK(MS_GOP_CTX_LOCAL *pGOPCtx, MS_U8 enGopType);
MS_U8 MDrv_GOP_GetMaxGOPNum(MS_GOP_CTX_LOCAL*pGOPCtx);
MS_U8 MDrv_GOP_GetMaxMuxSel(MS_GOP_CTX_LOCAL*pGOPCtx);
MS_U8 MDrv_GOP_SelGwinIdByGOP(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_U8 u8GWinIdx);
GOP_Result MDrv_GOP_SetClock(MS_GOP_CTX_LOCAL*pGOPCtx,MS_BOOL bEnable);
GOP_Result MDrv_GOP_ConvertAPIAddr2HAL(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8Gwinid, MS_PHY* u64ApiAdr);
GOP_Result MDrv_GOP_GWIN_SetFadeInOut(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 gId, DRV_GOP_FADE_TYPE eGwinFadeType, MS_BOOL bEnable, MS_U8 u8FadeRate);
MS_U32 MDrv_GOP_GetMIULen(MS_GOP_CTX_LOCAL*pGOPCtx);
void  MDrv_GOP_MIU_Sel(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum,E_DRV_GOP_SEL_TYPE miusel);
MS_U8 MDrv_GOP_Get_MIU_Sel(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum);
void MDrv_GOP_SetGOPBWStrength(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, EN_GOP_BURST_LEN eGOPBWStr);
EN_GOP_BURST_LEN MDrv_GOP_GetGOPBWStrength(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP);
void MDrv_GOP_SetGOPYUV(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP);
MS_U8 MDrv_GOP_GetHPipeOfst(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP_num, DRV_GOPDstType GopDst);
MS_U8 MDrv_GOP_GetDWINMIU(MS_GOP_CTX_LOCAL*pGOPCtx);
GOP_Result MDrv_GOP_SetDWINMIU(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 miu);
GOP_Result MDrv_GOP_VE_SetOutputTiming(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U32 u32mode);
GOP_Result MDrv_GOP_MIXER_SetOutputTiming(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U32 u32mode, GOP_DRV_MixerTiming *pTM);
GOP_Result MDrv_GOP_MIXER_EnableVfilter(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bEnable);
GOP_Result MDrv_GOP_MIXER_EnableOldBlendMode(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 gopNum, MS_BOOL bEnable);
MS_BOOL MDrv_GOP_RestoreFromVsyncLimitation(MS_GOP_CTX_LOCAL *pGOPCtx);
MS_U32 MDrv_GOP_GetRegForm(MS_GOP_CTX_LOCAL *pGOPCtx, MS_U8 u8gop, MS_U8 u8gwin);
GOP_Result MDrv_GOP_SetGOPBrightness(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP,MS_U16 u16BriVal,MS_BOOL bMSB);
GOP_Result MDrv_GOP_GetGOPBrightness(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP,MS_U16* u16BriVal,MS_BOOL* bMSB);
GOP_Result MDrv_GOP_EnableLBCouple(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_BOOL bEnable);
GOP_Result MDrv_GOP_Set_PINPON(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum,MS_BOOL bEn,E_DRV_GOP_PINPON_MODE pinpon_mode);
GOP_Result MDrv_GOP_Set_BufferPINPON(MS_GOP_CTX_LOCAL*pGOPCtx,MS_PHY u64fbaddr0,MS_PHY u64fbaddr1);
GOP_Result MDrv_GOP_Set_16bBus(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum,MS_BOOL bEn);
GOP_Result MDrv_GOP_SetUVSwap(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum,MS_BOOL bEn);
GOP_Result MDrv_GOP_SetYCSwap(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum,MS_BOOL bEn);
GOP_Result MDrv_GOP_SetGOPContrast(MS_GOP_CTX_LOCAL* pGOPCtx, MS_U8 u8GOP, MS_U16 u16YContrast, MS_U16 u16UContrast, MS_U16 u16VContrast);
GOP_Result MDrv_GOP_GetGOPContrast(MS_GOP_CTX_LOCAL* pGOPCtx, MS_U8 u8GOP, MS_U32* u32YContrast, MS_U32* u32UContrast, MS_U32* u32VContrast);
MS_BOOL MDrv_GOP_GetVECaptureState(MS_GOP_CTX_LOCAL *pGOPCtx, PMS_DRVGOP_VECAPTURESTATE pstVECapState);
MS_BOOL MDrv_GOP_VECaptureWaitOnFrame(MS_GOP_CTX_LOCAL *pGOPCtx, PMS_DRVGOP_VECAPTURESTATE pstVECapState);
MS_BOOL MDrv_GOP_EnaVECapture(MS_GOP_CTX_LOCAL *pGOPCtx, PMS_DRVGOP_VECAPTURESTATE pstVECapState);
GOP_Result MDrv_GOP_Set3DOSD_Mode(MS_GOP_CTX_LOCAL* pGOPCtx, MS_U8 u8GOP ,EN_DRV_GOP_3D_MODE en3DOSDMode);
GOP_Result MDrv_GOP_Set3DOSD_Sub(MS_GOP_CTX_LOCAL* pGOPCtx, MS_U8 u8Gwin, MS_PHY u64SubAddr);
GOP_Result MDrv_GOP_Set3D_LR_FrameExchange(MS_GOP_CTX_LOCAL* pGOPCtx, MS_U8 u8GOP);
MS_U16 MDrv_GOP_GetCurrentHSPipe(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP);
void MDrv_GOP_Set3DOSD_Middle(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP);
MS_BOOL MDrv_GOP_SetGPIO3DPin(MS_GOP_CTX_LOCAL*pGOPCtx,  MS_U32 u32GPIO3DPin);
void MDrv_GOP_Mask_First_Hsync(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_BOOL bMask );
GOP_Result MDrv_GOP_Set_VE_MUX(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 gopNum, MS_BOOL bEn);
GOP_Result MDrv_GOP_BNK(MS_GOP_CTX_LOCAL*pGOPCtx, E_DRV_GOP_BNK_STATUS Bnk_op);
MS_BOOL MDrv_GOP_GetGOPEnum(MS_GOP_CTX_LOCAL*pGOPCtx, GOP_TYPE_DEF* GOP_TYPE);
GOP_Result MDrv_GOP_GWIN_PowerState(void* pInstance, MS_U32 u32PowerState, void* pModule);
GOP_Result MDrv_GOP_AFBC_Core_Reset(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP);
GOP_Result MDrv_GOP_AFBC_Core_Enable(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_BOOL bEna);
GOP_Result MDrv_GOP_GWIN_AFBCMode(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL u8GOP, MS_BOOL bEnable, EN_DRV_GOP_AFBC_CNTL eCTL);
GOP_Result MDrv_GOP_GWIN_AFBCSetWindow(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, DRV_GOP_AFBC_Info* pinfo, MS_BOOL bChangePitch);
GOP_Result MDrv_GOP_SetDbgLevel(EN_GOP_DEBUG_LEVEL level);
//======================================================================================
//  GOP GWIN Setting
//======================================================================================
void MDrv_GOP_GWIN_EnableProgressive(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, MS_BOOL bEnable);
MS_BOOL MDrv_GOP_GWIN_IsProgressive(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum);
void MDrv_GOP_GWIN_SetForceWrite(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bEnable);
void MDrv_GOP_GWIN_SetBnkForceWrite(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_BOOL bEnable);
void MDrv_GOP_GWIN_ForceWrite_Update(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_BOOL bEnable);
MS_BOOL MDrv_GOP_GWIN_IsForceWrite(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP);
void MDrv_GOP_GWIN_OutputColor(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, GOP_OupputColor type);
void MDrv_GOP_GWIN_SetTransClr_8888(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_U32 clr, MS_U32 mask);
void MDrv_GOP_GWIN_SetTransClr_YUV(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_U32 clr, MS_U32 mask);
void MDrv_GOP_GWIN_EnableTransClr(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, GOP_TransClrFmt fmt, MS_BOOL bEnable);
void MDrv_GOP_GWIN_EnableMultiAlpha(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U32 u32GopIdx, MS_BOOL bEnable);
MS_BOOL MDrv_GOP_GWIN_IsMultiAlphaEnable(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U32 u32GopIdx);
void MDrv_GOP_GWIN_SetBlink(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, MS_BOOL bEnable, MS_U8 u8rate);
void MDrv_GOP_GWIN_SetFieldInver(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_BOOL enable);
void MDrv_GOP_GWIN_SetAlphaInverse(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8gopNum, MS_BOOL bEnable);
void MDrv_GOP_GWIN_GetAlphaInverse(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_BOOL *bBool);
GOP_Result MDrv_GOP_GWIN_GetDstPlane(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, DRV_GOPDstType *pGopDst);
void MDrv_GOP_GWIN_SetBlending(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, MS_BOOL bEnable, MS_U8 u8coef);
void MDrv_GOP_GWIN_SetWinFmt(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, DRV_GOPColorType clrtype);
void MDrv_GOP_GWIN_EnableGwin(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, MS_BOOL bEnable);
void MDrv_GOP_GWIN_Set_BGWIN(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 bgwID, MS_U16 x0, MS_U16 y0, MS_U16 x1, MS_U16 y1, MS_U16 color);
void MDrv_GOP_GWIN_Set_BGWIN_Alpha(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 bgwID, MS_BOOL bEnable, MS_U8 alpha);
void MDrv_GOP_GWIN_IsGWINEnabled(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, MS_BOOL* pbEnable );
void MDrv_GOP_GWIN_EnableHMirror(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, MS_BOOL bEnable);
void MDrv_GOP_GWIN_EnableVMirror(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, MS_BOOL bEnalbe);
void MDrv_GOP_IsGOPMirrorEnable(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, MS_BOOL *bHMirror, MS_BOOL *bVMirror);
void MDrv_GOP_GWIN_SetGwinPriority(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GopNum, GOP_GwinPriority *GwinPri);
void MDrv_GOP_GWIN_GetGwinPriority(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8Gopnum, GOP_GwinPriority *GwinPri);
void MDrv_GOP_GWIN_SetMux(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, Gop_MuxSel eGopMux);
void MDrv_GOP_MapLayer2Mux(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U32 u32Layer, MS_U8 u8Gopnum, MS_U32 *pu32Mux);
void MDrv_GOP_GWIN_GetMux(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8* u8GOPNum, Gop_MuxSel eGopMux);
MS_BOOL Mdrv_GOP_GetMuxPriority(MS_GOP_CTX_LOCAL*pGOPCtx, PST_DRV_GOP_MUX_CAP_EX pstMuxPriEx);
void MDrv_GOP_GWIN_SetHSPipe(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, MS_U16 u16HSPipe);
void MDrv_GOP_DWIN_GetWinProperty(MS_GOP_CTX_LOCAL*pGOPCtx, DRV_GOP_DwinProperty *pproperty);
void MDrv_GOP_DWIN_SetWinProperty(MS_GOP_CTX_LOCAL*pGOPCtx, DRV_GOP_DwinProperty *pproperty);
void MDrv_GOP_GWIN_SetVSyncinverse(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_BOOL bEnable);
void MDrv_GOP_SetGOPEnable2Mode1(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 gopNum, MS_BOOL bEnable);
void MDrv_GOP_GetGOPEnable2Mode1(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 gopNum, MS_BOOL *pbEnable);
GOP_Result MDrv_GOP_GWIN_GetGwinInfo(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, DRV_GOP_GWIN_INFO* pinfo);
GOP_Result MDrv_GOP_GWIN_SetGwinInfo(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, DRV_GOP_GWIN_INFO* pinfo);
MS_BOOL MDrv_GOP_GWIN_SendGwinInfo2Flip(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, DRV_GOP_GWIN_INFO* pinfo);
GOP_Result MDrv_GOP_GWIN_SetDstPlane(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, DRV_GOPDstType eDstType,MS_BOOL bOnlyCheck);
GOP_Result MDrv_GOP_GWIN_Enable_BGWIN(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 bgwID, MS_BOOL bEnable);
GOP_Result MDrv_GOP_GWIN_EnableTileMode(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8win, MS_BOOL bEnable,E_GOP_TILE_DATA_TYPE tilemode);
GOP_Result MDrv_GOP_GWIN_SetMixerDstPlane(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, DRV_GOPDstType eDstType);
GOP_Result MDrv_GOP_GWIN_GetMixerDstPlane(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, DRV_GOPDstType *pGopDst);
GOP_Result MDrv_GOP_MIXER_SetMux(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 gopNum, MS_U8 muxNum, MS_BOOL bEnable);
GOP_Result MDrv_GOP_GWIN_GetGwinNewAlphaModeEnable(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, MS_BOOL* bEnable);
GOP_Result MDrv_GOP_GWIN_SetNewAlphaMode(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, MS_BOOL bEnable);
GOP_Result MDrv_GOP_GWIN_ClearFlipQueue(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 gWinId);
MS_U16 MDrv_GOP_GetBPP(MS_GOP_CTX_LOCAL*pGOPCtx, DRV_GOPColorType fbFmt);
E_GOP_VIDEOTIMING_MIRRORTYPE Mdrv_GOP_GetVideoTimingMirrorType(MS_GOP_CTX_LOCAL* pGOPCtx, MS_BOOL bHorizontal);
GOP_Result MDrv_GOP_GWIN_DeleteWinHVSize(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_U16 u16HSize, MS_U16 u16VSize);
void MDrv_GOP_SelfFirstHs(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8Gop, MS_BOOL bEnable);

//======================================================================================
//  GOP Stretch Window Setting
//======================================================================================
void MDrv_GOP_GWIN_SetStretchWin(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_U16 x, MS_U16 y, MS_U16 width, MS_U16 height);
void MDrv_GOP_GWIN_SetStretchWinPosition(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP,MS_U16 u16x, MS_U16 u16y);
void MDrv_GOP_GWIN_SetHTotal(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_U16 width);
void MDrv_GOP_GWIN_Get_StretchWin(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_U16 *x, MS_U16 *y, MS_U16 *width, MS_U16 *height);
MS_BOOL MDrv_GOP_GWIN_Get_HVScaleRatio(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP_num, MS_U16 *pu16HScaleRat, MS_U16 *pu16VScaleRat);
void MDrv_GOP_GWIN_Set_HSCALE(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_BOOL bEnable, MS_U16 src, MS_U16 dst) ;
void MDrv_GOP_GWIN_Set_VSCALE(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_BOOL bEnable, MS_U16 src, MS_U16 dst) ;
void MDrv_GOP_GWIN_Set_HStretchMode(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, DRV_GOPStrchHMode HStrchMode);
void MDrv_GOP_GWIN_Set_VStretchMode(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, DRV_GOPStrchVMode VStrchMode);
GOP_Result MDrv_GOP_GWIN_Set_TranspColorStretchMode(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, DRV_GOPSTRCH_TRANSPCOLORMODE TColorStrchMode);
void MDrv_GOP_GWIN_Load_HStretchModeTable(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP , DRV_GOPStrchHMode HStrchMode);
void MDrv_GOP_GWIN_Load_VStretchModeTable(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, DRV_GOPStrchVMode VStrchMode);

//======================================================================================
//  GOP Palette setting
//======================================================================================
void MDrv_GOP_GWIN_SetPaletteOpt( MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, DRV_GopPaletteEntry *pPalArray, MS_U16 u32PalStart, MS_U16 u32PalEnd, DRV_GopPalType ePalType);
void MDrv_GOP_GWIN_SetPaletteRead(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, DRV_GopPalReadType type);
void MDrv_GOP_GWIN_ReadPalette(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_U8 idx, MS_U32* clr);
void MDrv_GOP_GWIN_2GSetPaletteOpt(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, DRV_GopPaletteEntry *pPalArray, MS_U16 u32PalStart, MS_U16 u32PalEnd);
void MDrv_GOP_GWIN_2GSetPaletteRead(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, DRV_GopPalReadType type);
void MDrv_GOP_GWIN_2GReadPalette(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_U8 idx, MS_U32* clr);

//======================================================================================
//  GOP GWIN H/V Scroll
//======================================================================================
void MDrv_GOP_GWIN_SetScrollRate(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8rate);
void MDrv_GOP_GWIN_SetHScroll(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, E_GOP_SCROLL_TYPE type, MS_U32 offset, MS_BOOL bEnable);
void MDrv_GOP_GWIN_SetVScroll(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, E_GOP_SCROLL_TYPE type, MS_U32 offset, MS_BOOL bEnable);
void MDrv_GOP_GWIN_Scroll_EnableAutoStop(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, MS_BOOL bEnable);
MS_BOOL MDrv_GOP_GWIN_Scroll_AutoStop_HSet(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, MS_U32 u32AutoHStopOffset);
MS_BOOL MDrv_GOP_GWIN_Scroll_AutoStop_VSet(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, MS_U32 u32AutoVStopOffset);


//======================================================================================
//  GOP DWIN Utility
//======================================================================================
void MDrv_GOP_DWIN_SetForceWrite(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bEnable);
void MDrv_GOP_DWIN_EnableCaptureStream(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bEnable);
void MDrv_GOP_DWIN_CaptureOneFrame(MS_GOP_CTX_LOCAL*pGOPCtx);
void MDrv_GOP_DWIN_CaptureOneFrame2(MS_GOP_CTX_LOCAL*pGOPCtx);
void MDrv_GOP_DWIN_SetWinInfo(MS_GOP_CTX_LOCAL*pGOPCtx, DRV_GOP_DWIN_INFO* pinfo);
void MDrv_GOP_DWIN_GetWinInfo(MS_GOP_CTX_LOCAL*pGOPCtx, DRV_GOP_DWIN_INFO* pinfo);
void MDrv_GOP_DWIN_EnableProgressive(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bEnable);
void MDrv_GOP_DWIN_SelectSource(MS_GOP_CTX_LOCAL*pGOPCtx, EN_GOP_DWIN_DATA_SRC enSrc);
void MDrv_GOP_DWIN_SelectSourceScanType(MS_GOP_CTX_LOCAL*pGOPCtx, EN_GOP_DWIN_SCAN_TYPE enScan);
void MDrv_GOP_DWIN_EnableIntr(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U16 u8mask, MS_BOOL bEnable);
void MDrv_GOP_DWIN_ClearIntr(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U16 u8mask);
void MDrv_GOP_DWIN_Set_CTRL0(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U16 val, MS_U16 mask);
void MDrv_GOP_DWIN_SetDataFmt(MS_GOP_CTX_LOCAL*pGOPCtx, DRV_GOP_DWIN_DATA_FMT fmt) ;
void MDrv_GOP_DWIN_GetDataFmt(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 *fmt);
void MDrv_GOP_DWIN_SetAlphaValue(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8AlphaVal);
void MDrv_GOP_DWIN_SetAlphaSrc(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 source);
void MDrv_GOP_DWIN_SetAlphaInverse(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bEnable);
void MDrv_GOP_DWIN_SetUVSample(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 mode);
void MDrv_GOP_DWIN_SetSkipFrame(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U32 framecount);
void MDrv_GOP_DWIN_EnableDither(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bEnable);
GOP_Result MDrv_GOP_DWIN_EnableR2YCSC(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bEnable);
void MDrv_GOP_DWIN_SetUVSwap(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bEnable);
MS_U16 MDrv_GOP_DWIN_GetIntrStatus(MS_GOP_CTX_LOCAL*pGOPCtx);
GOP_Result MDrv_GOP_DWIN_SetSourceSel(MS_GOP_CTX_LOCAL*pGOPCtx, DRV_GOP_DWIN_SRC_SEL enSrcSel);
GOP_Result MDrv_GOP_DWIN_GetDWinIntInfo(MS_GOP_CTX_LOCAL *pGOPCtx, DRV_GOP_DWinIntInfo *pGopDWinIntInfo,MS_U32 u32Timeout);
void Mdrv_GOP_GetDWinCapability(MS_GOP_CTX_LOCAL*pGOPCtx, PDRV_GOP_DWIN_CAP pDwinCap);

//======================================================================================
//  GOP OC
//======================================================================================
GOP_Result MDrv_GOP_OC_SetOCEn(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOPNum, MS_BOOL bOCEn);
GOP_Result MDrv_GOP_OC_SetOCInfo(MS_GOP_CTX_LOCAL*pGOPCtx, DRV_GOP_OC_INFO* OCinfo);

//======================================================================================
//  GOP GPU Tile
//======================================================================================
GOP_Result MDrv_GOP_GWIN_SetGPUTileMode(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8win, EN_DRV_GOP_GPU_TILE_MODE mode);


//======================================================================================
//  GOP TLB
//======================================================================================
GOP_Result MDrv_GOP_TLBEnable(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GOP, MS_BOOL bTLBEn);
GOP_Result MDrv_GOP_SetTLBAddr(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 GOP, MS_PHY u64TLBAddr,MS_BOOL bHMirror,MS_BOOL bVMirror,DRV_GOP_GWIN_INFO* pinfo);
GOP_Result MDrv_GOP_SetTLBSubAddr(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8GOP, MS_PHY u64TLBAddr);

//======================================================================================
//  GOP INTERNAL GET XC STATUS
//======================================================================================
GOP_Result MDrv_GOP_GetIPCaptureHStart(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U32 *u32Hstart,E_DRV_GOP_SCALER_WIN eWindow);
GOP_Result MDrv_GOP_GetIPInterlace(MS_GOP_CTX_LOCAL*pGOPCtx,MS_BOOL *bInterlace,E_DRV_GOP_SCALER_WIN eWindow);


//======================================================================================
//  GOP TestPattern Setting
//======================================================================================
GOP_Result MDrv_GOP_TestPattern_IsVaild(MS_GOP_CTX_LOCAL*pGOPCtx);
GOP_Result MDrv_GOP_TestPatternAlpha_Enable(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bTSTClr_Alpha_En);
GOP_Result MDrv_GOP_TestPattern(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL bTSTClr_En, MS_U8 u8TSTClr_Alpha);
GOP_Result MDrv_GOP_TestPattern_SetStartClr(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8StartR,MS_U8 u8StartG,MS_U8 u8StartB);
GOP_Result MDrv_GOP_TestPattern_SetHInitColor(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8HIniR,MS_U8 u8HIniG,MS_U8 u8HIniB);
GOP_Result MDrv_GOP_TestPattern_SetVInitColor(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8VIniR,MS_U8 u8VIniG,MS_U8 u8VIniB);
GOP_Result MDrv_GOP_TestPattern_SetHIncremental_Signz(MS_GOP_CTX_LOCAL*pGOPCtx,MS_BOOL bHIncreSigR,MS_BOOL bHIncreSigG,MS_BOOL bHIncreSigB);
GOP_Result MDrv_GOP_TestPattern_SetVIncremental_Signz(MS_GOP_CTX_LOCAL*pGOPCtx,MS_BOOL bVIncreSigR,MS_BOOL bVIncreSigG,MS_BOOL bVIncreSigB);
GOP_Result MDrv_GOP_TestPattern_SetHStep(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8HStepR,MS_U8 u8HStepG,MS_U8 u8HStepB);
GOP_Result MDrv_GOP_TestPattern_SetVStep(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8VStepR,MS_U8 u8VStepG,MS_U8 u8VStepB);
GOP_Result MDrv_GOP_TestPattern_SetHVDuplicate(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8HDup,MS_U8 u8VDup);
MS_BOOL MDrv_GOP_GWIN_BeginDraw(void);
MS_BOOL MDrv_GOP_GWIN_EndDraw(void);
void MDrv_GOP_GWIN_Interrupt(MS_GOP_CTX_LOCAL*pGOPCtx,MS_U8 u8Gop,MS_BOOL bEable);

//======================================================================================
//  GOP HDR
//======================================================================================
GOP_Result MDrv_GOP_IsHDREnabled(MS_GOP_CTX_LOCAL*pGOPCtx, MS_BOOL* pbHDREnable);

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
void MDrv_GOP_MDCMD_GETINFO(void* pInstance, MS_U64* u64ReqHdl);
void MDrv_GOP_MDCMD_EchoCmd(void* pInstance, MS_U64* u64ReqHdl, char* pcCmdLine);
#endif

#ifdef __cplusplus
}
#endif


#endif // _DRV_TEMP_H_
