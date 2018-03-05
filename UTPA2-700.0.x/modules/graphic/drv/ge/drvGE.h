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

#ifndef _DRV_GE_H_
#define _DRV_GE_H_

//#include "MsTypes.h"
#include "MsDevice.h"
#if defined(MSOS_TYPE_LINUX_KERNEL)
#include "autoconf.h"
#endif

#ifdef ANDROID
#include <sys/mman.h>
#include <cutils/ashmem.h>
#include <cutils/log.h>
#endif
/*ALOGI*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _DRV_GE_C_
#define GFX_INTERFACE
#else
#define GFX_INTERFACE extern
#endif

#if defined(MSOS_TYPE_LINUX)
    #include <sys/types.h>
    #include <unistd.h>
#elif defined(MSOS_TYPE_LINUX_KERNEL)
    #define getpid()                pInstance
#else
#define getpid() 0UL
#endif

typedef enum
{
    E_GE_POOL_ID_INTERNAL_REGISTER=0,
    E_GE_POOL_ID_INTERNAL_VARIABLE=1,

    E_GE_POOL_ID_NUM,
    E_GE_POOL_ID_MAX = E_GE_POOL_ID_NUM,
} eGEPoolID;

// #define GFX Utopia2.0 capibility
GFX_INTERFACE void*   g_pGFXResource[E_GE_POOL_ID_NUM];

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
#define GE_PERFORMANCE_TEST         0UL
#define GE_MUTEX_DBG                0UL

#define GFX_CRITIAL_MSG(x)      (x)       // for dump debug message
#define GFX_DEBUG_MSG(x)        (x)       // for dump debug message

#if defined(CONFIG_API_GFX) || defined(CONFIG_API_GFX_MODULE)
#else
#define CONFIG_GFX_TEXTOUT
#define CONFIG_GFX_TRAPZOID
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define GFX_UTOPIA20                            1UL       //Using Ioctl
#define GE_API_MUTEX                            1UL       //Using API mutex lock/unlock;  other is Drv mutex lock/unlock
#define GE_SWTABLE                              0UL       //Using virtul table as value stored
#define GE_RESOURCE_SEM                         0UL       //Resource protect semaphore for Muti GE engine.
#define GE_PALETTE_NUM                          256UL
#define GE_FB_ADDR_MIU_MASK_BIT                 (1UL)

#if (defined(MSOS_TYPE_NOS))||(defined(MSOS_TYPE_OPTEE))
#define GE_LOCK_SUPPORT             0UL   //This is for MHEG5 HW semephore
#else
#define GE_LOCK_SUPPORT             1UL
#endif

#define GE_DRV_VERSION                  /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    {'G','E','_','_'},                  /* IP__                                             */  \
    {'1','7'},                          /* 0.0 ~ Z.Z                                        */  \
    {'0','2'},                          /* 00 ~ 99                                          */  \
    {'0','0','5','5','2','5','0','9'},  /* CL#                                              */  \
    MSIF_OS

#define ByteSwap16(x) (((x) & 0x00ffUL) << 8 | ((x) & 0xff00UL) >> 8)
#define ByteSwap32(x) \
    ((((x) & 0xff000000UL) >> 24) | (((x) & 0x00ff0000UL) >>  8) | \
     (((x) & 0x0000ff00UL) <<  8) | (((x) & 0x000000ffUL) << 24))

#define GE_BIT0                         0x01UL
#define GE_BIT1                         0x02UL
#define GE_BIT2                         0x04UL
#define GE_BIT3                         0x08UL
#define GE_BIT4                         0x10UL
#define GE_BIT5                         0x20UL
#define GE_BIT6                         0x40UL
#define GE_BIT7                         0x80UL
#define GE_BIT8                         0x0100UL
#define GE_BIT9                         0x0200UL
#define GE_BIT10                        0x0400UL
#define GE_BIT11                        0x0800UL
#define GE_BIT12                        0x1000UL
#define GE_BIT13                        0x2000UL
#define GE_BIT14                        0x4000UL
#define GE_BIT15                        0x8000UL

#ifdef ANDROID
    #define printf  ALOGI
#elif defined MSOS_TYPE_LINUX_KERNEL
    #define printf  printk
#endif


#if GE_MUTEX_DBG
    #define CMD(eCmd)                   printf("\33[0;34m [%s][%d] eCmd =%d\33[m;\n",__FUNCTION__,__LINE__, eCmd);
#else
    #define CMD(eCmd)                   do{}while(0);
#endif

#if GE_MUTEX_DBG//#if defined(GE_MUTEX_DBG)
    #define _GE_MUXTEX_ENTRY(pGECtx , POOL_ID)\
            printf("[1] [%s, %d] GE Prepare Get Mutex ID=%d [PID, TID]=[%x, %x]\n",__FUNCTION__,__LINE__, POOL_ID, getpid(), MsOS_GetOSThreadID());\
                GE_Get_Resource(pGECtx,TRUE);\
            printf("    [%s, %d] GE Got Mutex ID=%d [PID, TID]=[%x, %x]\n",__FUNCTION__,__LINE__, POOL_ID, getpid(), MsOS_GetOSThreadID());


    #define _GE_MUXTEX_RETURN(pGECtx, POOL_ID)\
            printf("[0] [%s, %d] GE Prepare Free Mutex ID=%d [PID, TID]=[%x, %x] \n",__FUNCTION__,__LINE__, POOL_ID, getpid(), MsOS_GetOSThreadID());\
                GE_Free_Resource(pGECtx, TRUE);\
            printf("    [%s, %d] GE Free Mutex ID=%d [PID, TID]=[%x, %x]\n",__FUNCTION__,__LINE__, POOL_ID, getpid(), MsOS_GetOSThreadID());
#else
    #define _GE_MUXTEX_ENTRY(pGECtx, POOL_ID)\
        if(POOL_ID == E_GE_POOL_ID_INTERNAL_VARIABLE)\
            GE_Get_Resource(pGECtx,TRUE);

    #define _GE_MUXTEX_RETURN(pGECtx, POOL_ID)\
        if(POOL_ID == E_GE_POOL_ID_INTERNAL_VARIABLE)\
            GE_Free_Resource(pGECtx, TRUE);
#endif


#if GE_MUTEX_DBG
    #define _GE_SEMAPHORE_ENTRY(pGECtx , POOL_ID)\
            printf("[1] [%s, %d] GE Prepare Get Semaphore ID=%d [PID, TID]=[%x, %x]\n",__FUNCTION__,__LINE__, POOL_ID, getpid(), MsOS_GetOSThreadID());\
            MDrv_GE_Get_Semaphore(pGECtx, POOL_ID);\
            printf("    [%s, %d] GE Got Semaphore ID=%d [PID, TID]=[%x, %x]\n",__FUNCTION__,__LINE__, POOL_ID, getpid(), MsOS_GetOSThreadID());


    #define _GE_SEMAPHORE_RETURN(pGECtx, POOL_ID)\
            printf("[0] [%s, %d] GE Prepare Free Semaphore ID=%d [PID, TID]=[%x, %x]\n",__FUNCTION__,__LINE__, POOL_ID, getpid(), MsOS_GetOSThreadID());\
            MDrv_GE_Free_Semaphore(pGECtx, POOL_ID);\
            printf("    [%s, %d] GE Free Semaphore ID=%d [PID, TID]=[%x, %x]\n",__FUNCTION__,__LINE__, POOL_ID, getpid(), MsOS_GetOSThreadID());
#else
    #define _GE_SEMAPHORE_ENTRY(pGECtx, POOL_ID)\
        if(POOL_ID == E_GE_POOL_ID_INTERNAL_REGISTER)\
        MDrv_GE_Get_Semaphore(pGECtx, POOL_ID);\

    #define _GE_SEMAPHORE_RETURN(pGECtx, POOL_ID)\
        if(POOL_ID == E_GE_POOL_ID_INTERNAL_REGISTER)\
        MDrv_GE_Free_Semaphore(pGECtx, POOL_ID);
#endif


#define TLB_ENRTY_MAPPING_SIZE    (32UL*1024UL*1024UL)
#ifndef MSOS_TYPE_LINUX_KERNEL
#define PAGE_SIZE           4096UL
#endif
#define TLB_PER_ENTRY_SIZE      4UL
#define TLB_ENRTY_COUNT     TLB_ENRTY_MAPPING_SIZE/PAGE_SIZE
#define TLB_ENRTY_SIZE      TLB_ENRTY_COUNT*TLB_PER_ENTRY_SIZE

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

/// Define GE bitblt down-scaling caps
typedef struct
{
    /// Bitblt down scale range start
    MS_U8 u8RangeMax;
    /// Bitblt down scale range end
    MS_U8 u8RangeMin;
    /// Bitblt down scale continuous range end
    MS_U8 u8ContinuousRangeMin;
    /// Is full range support down scaling.
    /// - TRUE: The down scale value between u8RangeMax to u8RangeMin is fully supported.
    /// - FALSE: The down scale value between u8RangeMax to u8ContinuousRangeMin is fully supported.
    ///          The down scale value between u8ContinuousRangeMin to u8RangeMin is supported
    ///          if the value is power of two (e.g., 4, 8, 16, and 32).
    MS_BOOL bFullRangeSupport;

    /// Help Maintain Functions GE_SetBltScaleRatio/GE_CalcBltScaleRatio
    /// 1>>u8ShiftRangeMax = u8RangeMax
    MS_U8 u8ShiftRangeMax;
    /// 1>>u8ShiftRangeMin = u8RangeMin
    MS_U8 u8ShiftRangeMin;
    /// 1>>u8ShiftContinuousRangeMin = u8ContinuousRangeMin
    MS_U8 u8ShiftContinuousRangeMin;
}GE_BLT_DownScaleCaps;

/// Define GE chip property for different chip characteristic.
typedef struct
{
    MS_U16 WordUnit;
    MS_BOOL bSupportFourePixelMode;
    MS_BOOL bFourPixelModeStable;
    MS_BOOL bSupportMultiPixel;
    MS_BOOL bSupportSpiltMode;
    MS_BOOL bSupportTLBMode;
    MS_BOOL bSupportTwoSourceBitbltMode;
    GE_BLT_DownScaleCaps BltDownScaleCaps;
    MS_U16 MIUSupportMaxNUM;
}GE_CHIP_PROPERTY;

///Define GE Return Value
typedef enum
{
    /// fail
    E_GE_FAIL = 0,
    /// success
    E_GE_OK,
    /// address error
    E_GE_FAIL_ADDR,
    /// pitch error
    E_GE_FAIL_PITCH,
    /// function parameter error
    E_GE_FAIL_PARAM,

    /// not support
    E_GE_NOT_SUPPORT,

    /// pixel format error
    E_GE_FAIL_FORMAT,
    /// bitblit start address error
    E_GE_FAIL_BLTADDR,
    /// bitblt overlap (if STRETCH, ITALIC, MIRROR, ROTATE)
    E_GE_FAIL_OVERLAP,
    /// stretch bitblt fail
    E_GE_FAIL_STRETCH,
    /// italic bitblt fail (if MIRROR, ROTATE)
    E_GE_FAIL_ITALIC,

    /// engine is locked by others
    E_GE_FAIL_LOCKED,

    /// primitive will not be drawn
    E_GE_NOT_DRAW,

    /// Dependent functions are not linked
    E_GE_NO_DEPENDENT,

    /// context not inited.
    E_GE_CTXMAG_FAIL,

    E_GE_LAST_RESULT,

} GE_Result;

///Define GE Init Configuration
typedef struct
{
    MS_U8                         u8Miu;
    MS_U8                         u8Dither;
    MS_U32                        u32VCmdQSize;                       /// MIN:4K, MAX:512K, <MIN:Disable
    MS_PHY                        PhyVCmdQAddr;                       // 8-byte aligned
    MS_BOOL                       bIsHK;                              /// Running as HK or Co-processor
    MS_BOOL                       bIsCompt;                           /// Running as compatible mode. In compatible mode, the parameter checking loose for NOS APP compatibility.  TRUE=>compatible with 51/Chakra , FALSE=>linux OS style
} GE_Config;


#define GE_FLAG_BLT_ROTATE_SHFT     16UL
#define GE_FLAG_BLT_ROTATE_MASK     (0x3UL << GE_FLAG_BLT_ROTATE_SHFT)

///Define GE Primitive Drawing Flags
typedef enum
{
    /// Line color gradient enable, color is gradient by major-axis
    E_GE_FLAG_LINE_CONSTANT         = 0x00000000,

    /// Line color gradient enable, color is gradient by major-axis
    E_GE_FLAG_LINE_GRADIENT         = 0x00000001,

    /// Rectangle horizonal color gradient enable
    E_GE_FLAG_RECT_GRADIENT_X       = 0x00000002,
    /// Rectangle vertical color gradient enable
    E_GE_FLAG_RECT_GRADIENT_Y       = 0x00000004,

    /// Trapezoid horizonal color gradient enable
    E_GE_FLAG_TRAPEZOID_GRADIENT_X  = 0x00000010,
    /// Rectangle vertical color gradient enable
    E_GE_FLAG_TRAPEZOID_GRADIENT_Y  = 0x00000020,

    /// Trapezoid flag: top & bottom in X direction parallel Trapezoid, excluded with E_GE_FLAG_TRAPEZOID_Y
    E_GE_FLAG_TRAPEZOID_X             = 0x00000040,
    /// Trapezoid flag: top & bottom in Y direction parallel Trapezoid, excluded with E_GE_FLAG_TRAPEZOID_X
    E_GE_FLAG_TRAPEZOID_Y             = 0x00000080,
    /// Support SRC/DST RECT overlap
    ///@note OVERLAP does not support BLT_STRETCH, BLT_MIRROR, BLT_ROTATE, and different SRC/DST buffers.
    E_GE_FLAG_BLT_OVERLAP           = 0x00000100,

    /// Stretch bitblt enable
    ///@note STRETCH does not support BLT_OVERLAP
    E_GE_FLAG_BLT_STRETCH           = 0x00000200,

    /// Bitblt italic style enable
    ///@note ITALIC does not support BLT_MIRROR, BLT_ROTATE, BLT_OVERLAP
    E_GE_FLAG_BLT_ITALIC            = 0x00000400,

    /// Horizontal mirror
    ///@note MIRROR does not support FMT_I1, FMT_I2, FMT_I4, BLT_ITALIC, BLT_OVERLAP
    E_GE_FLAG_BLT_MIRROR_H          = 0x00001000,
    /// Vertical mirror
    E_GE_FLAG_BLT_MIRROR_V          = 0x00002000,

    E_GE_FLAG_BLT_DST_MIRROR_H      = 0x00004000,

    E_GE_FLAG_BLT_DST_MIRROR_V      = 0x00008000,

    /// 90 deree clockwise rotation
    ///@note ROTATE does not support italic, BLT_OVERLAP
    E_GE_FLAG_BLT_ROTATE_90         = (0x01 << GE_FLAG_BLT_ROTATE_SHFT),
    /// 180 degree clockwise rotation
    E_GE_FLAG_BLT_ROTATE_180        = (0x02 << GE_FLAG_BLT_ROTATE_SHFT),
    /// 270 degree clockwise rotation
    E_GE_FLAG_BLT_ROTATE_270        = (0x03 << GE_FLAG_BLT_ROTATE_SHFT),

    /// BLT_STRETCH by nearest filter (default: BILINEAR)
    E_GE_FLAG_STRETCH_NEAREST       = 0x00100000,
//    E_GE_FLAG_STRETCH_CLAMP         = 0x00200000,

    // (RESERVED)
    E_GE_FLAG_BUF_TILE              = 0x01000000,

    E_GE_FLAG_BYPASS_STBCOEF         = 0x02000000,

} GE_Flag;


///Define Buffer Format
typedef enum
{
    /// color format I1
    E_GE_FMT_I1                     = E_MS_FMT_I1,
    /// color format I2
    E_GE_FMT_I2                     = E_MS_FMT_I2,
    /// color format I4
    E_GE_FMT_I4                     = E_MS_FMT_I4,
    /// color format palette 256(I8)
    E_GE_FMT_I8                     = E_MS_FMT_I8,
    /// color format blinking display
    E_GE_FMT_FaBaFgBg2266           = E_MS_FMT_FaBaFgBg2266,
    /// color format for blinking display format
    E_GE_FMT_1ABFgBg12355           = E_MS_FMT_1ABFgBg12355,
    /// color format RGB565
    E_GE_FMT_RGB565                 = E_MS_FMT_RGB565,
    /// color format ARGB1555
    /// @note <b>[URANUS] <em>ARGB1555 is only RGB555</em></b>
    E_GE_FMT_ARGB1555               = E_MS_FMT_ARGB1555,
    /// color format ARGB4444
    E_GE_FMT_ARGB4444               = E_MS_FMT_ARGB4444,
    /// color format ARGB1555 DST
    E_GE_FMT_ARGB1555_DST           = E_MS_FMT_ARGB1555_DST,
    /// color format YUV422
    E_GE_FMT_YUV422                 = E_MS_FMT_YUV422,
    /// color format ARGB8888
    E_GE_FMT_ARGB8888               = E_MS_FMT_ARGB8888,
    /// color format RGBA5551
    E_GE_FMT_RGBA5551                = E_MS_FMT_RGBA5551,
     /// color format RGBA4444
    E_GE_FMT_RGBA4444               = E_MS_FMT_RGBA4444,
    /// color format ABGR8888
    E_GE_FMT_ABGR8888                = E_MS_FMT_ABGR8888,
    /// New Color Format
    /// color format BGRA5551
    E_GE_FMT_BGRA5551               = E_MS_FMT_BGRA5551,
    /// color format ABGR1555
    E_GE_FMT_ABGR1555               = E_MS_FMT_ABGR1555,
    /// color format ABGR4444
    E_GE_FMT_ABGR4444               = E_MS_FMT_ABGR4444,
    /// color format BGRA4444
    E_GE_FMT_BGRA4444               = E_MS_FMT_BGRA4444,
    /// color format BGR565
    E_GE_FMT_BGR565               = E_MS_FMT_BGR565,
    /// color format RGBA8888
    E_GE_FMT_RGBA8888               = E_MS_FMT_RGBA8888,
    /// color format RGBA8888
    E_GE_FMT_BGRA8888               = E_MS_FMT_BGRA8888,

    E_GE_FMT_GENERIC                = E_MS_FMT_GENERIC,

} GE_BufFmt;

//GE TLB Mode
typedef enum
{
    /// TLB for None
    E_GE_TLB_NONE = 0,
    /// TLB for Source
    E_GE_TLB_SRC = 1,
    /// TLB for Destination
    E_GE_TLB_DST = 2,
    /// TLB for Source and Destination
    E_GE_TLB_SRC_DST = 3,
}GE_TLB_Mode;

///Define Colorkey Mode
typedef enum
{
    /// max(Asrc,Adst)
    E_GE_ACMP_OP_MAX                = 0,
    /// min(Asrc,Adst)
    E_GE_ACMP_OP_MIN                = 1,

} GE_ACmpOp;


///Define Colorkey Mode
typedef enum
{
    /// color in coler key range is keying.
    E_GE_CK_EQ                      = 0, // Color EQ CK is color key
    /// color NOT in color key range is keing.
    E_GE_CK_NE                      = 1, // Color NE CK is color key
    /// Alpha in coler key range is keying.
    E_GE_ALPHA_EQ                   = 2, // Color EQ Alpha is color key
    /// Alpha NOT in color key range is keing.
    E_GE_ALPHA_NE                   = 3, // Color NE Alpha is color key
} GE_CKOp;


///Define AlphaTest Mode
typedef enum
{
    /// color in coler key range is keying.
    E_GE_ATEST_EQ                   = 0, // Alpha EQ threshold
    /// color NOT in color key range is keing.
    E_GE_ATEST_NE                   = 1, // Alpha NE threshold

} GE_ATestOp;


///Define Raster Operation
typedef enum
{
    /// rop_result = 0;
    E_GE_ROP2_ZERO                  = 0,
    /// rop_result = ~( rop_src | rop_dst );
    E_GE_ROP2_NOT_PS_OR_PD          = 1,
    /// rop_result = ((~rop_src) & rop_dst);
    E_GE_ROP2_NS_AND_PD             = 2,
    /// rop_result = ~(rop_src);
    E_GE_ROP2_NS                    = 3,
    /// rop_result = (rop_src & (~rop_dst));
    E_GE_ROP2_PS_AND_ND             = 4,
    /// rop_result = ~(rop_dst);
    E_GE_ROP2_ND                    = 5,
    /// rop_result = ( rop_src ^ rop_dst);
    E_GE_ROP2_PS_XOR_PD             = 6,
    /// rop_result = ~(rop_src & rop_dst);
    E_GE_ROP2_NOT_PS_AND_PD         = 7,
    /// rop_result = (rop_src & rop_dst);
    E_GE_ROP2_PS_AND_PD             = 8,
    /// rop_result = ~(rop_dst ^ rop_src);
    E_GE_ROP2_NOT_PS_XOR_PD         = 9,
    /// rop_result = rop_dst;
    E_GE_ROP2_PD                    = 10,
    /// rop_result = (rop_dst | (~rop_src));
    E_GE_ROP2_NS_OR_PD              = 11,
    /// rop_result = rop_src;
    E_GE_ROP2_PS                    = 12,
    /// rop_result = (rop_src | (~rop_dst));
    E_GE_ROP2_PS_OR_ND              = 13,
    /// rop_result = (rop_dst | rop_src);
    E_GE_ROP2_PD_OR_PS              = 14,
    /// rop_result = 0xffffff;
    E_GE_ROP2_ONE                   = 15,

} GE_Rop2;


///Define Raster Operation
typedef enum
{
    /// once
    E_GE_LINPAT_REP1                = 0,
    /// 2 repeat
    E_GE_LINPAT_REP2                = 1,
    /// 3 repeat
    E_GE_LINPAT_REP3                = 2,
    /// 4 repeat
    E_GE_LINPAT_REP4                = 3,

} GE_LinePatRep;


///Define Blending Operation
typedef enum
{
    /// Cout = Csrc * A + Cdst * (1-A)
    /// 1
    E_GE_BLEND_ONE                  = 0,
    /// constant
    E_GE_BLEND_CONST                = 1,
    /// source alpha
    E_GE_BLEND_ASRC                 = 2,
    /// destination alpha
    E_GE_BLEND_ADST                 = 3,

    /// Cout = ( Csrc * (Asrc*Aconst) + Cdst * (1-Asrc*Aconst) ) / 2
    E_GE_BLEND_ROP8_ALPHA           = 4,
    /// Cout = ( Csrc * (Asrc*Aconst) + Cdst * (1-Asrc*Aconst) ) / ( Asrc*Aconst + Adst*(1-Asrc*Aconst) )
    E_GE_BLEND_ROP8_SRCOVER         = 5,
    /// Cout = ( Csrc * (Asrc*Aconst) + Cdst * (1-Asrc*Aconst) ) / ( Asrc*Aconst * (1-Adst) + Adst )
    ///@note
    /// <b>[URANUS] <em>It does not support BLEND_ALPHA, BLEND_SRCOVER, BLEND_DSTOVER</em></b>
    E_GE_BLEND_ROP8_DSTOVER         = 6,
    /// Cout = ( Csrc * Aconst)
    E_GE_BLEND_ALPHA_ADST           = 7,
    /// Cout = Csrc * A + Cdst * (1-A)
    /// 0
    E_GE_BLEND_ZERO                 = 8,
    /// 1 - constant
    E_GE_BLEND_CONST_INV            = 9,
    /// 1 - source alpha
    E_GE_BLEND_ASRC_INV             = 10,
    /// 1 - destination alpha
    E_GE_BLEND_ADST_INV             = 11,
    /// Csrc * Adst * Asrc * Aconst + Cdst * Adst * (1 - Asrc * Aconst)
    E_GE_BLEND_SRC_ATOP_DST         = 12,
    /// (Cdst * Asrc * Aconst * Adst + Csrc * Asrc * Aconst * (1 - Adst)) / ((Asrc * Aconst * (1 - Adst) + Asrc * Aconst * Adst)
    E_GE_BLEND_DST_ATOP_SRC         = 13,
    /// ((1 - Adst) * Csrc * Asrc * Aconst + Adst * Cdst * (1 - Asrc * Aconst)) / (Asrc * Aconst * (1 - Adst) + Adst * (1 - Asrc * Aconst))
    E_GE_BLEND_SRC_XOR_DST          = 14,
    /// Csrc * (1 - Aconst)
    E_GE_BLEND_INV_CONST            = 15,

} GE_BlendOp;


///Define alpha output selection
typedef enum
{
    /// constant
    E_GE_ALPHA_CONST                = 0,
    /// source alpha
    E_GE_ALPHA_ASRC                 = 1,
    /// destination alpha
    E_GE_ALPHA_ADST                 = 2,
    /// Aout = Asrc*Aconst
    E_GE_ALPHA_ROP8_SRC             = 3,
    /// Aout = Asrc*Aconst * Adst
    E_GE_ALPHA_ROP8_IN              = 4,
    /// Aout = (1-Asrc*Aconst) * Adst
    E_GE_ALPHA_ROP8_DSTOUT          = 5,
    /// Aout = (1-Adst) * Asrc*Aconst
    E_GE_ALPHA_ROP8_SRCOUT          = 6,
    /// Aout = (Asrc*Aconst) + Adst*(1-Asrc*Aconst) or (Asrc*Aconst)*(1-Adst) + Adst
    E_GE_ALPHA_ROP8_OVER            = 7,
    /// 1 - Aconst
    E_GE_ALPHA_ROP8_INV_CONST       = 8,
    /// 1 - Asrc
    E_GE_ALPHA_ROP8_INV_ASRC        = 9,
    /// 1 - Adst
    E_GE_ALPHA_ROP8_INV_ADST        = 10,
    /// Adst * Asrc * Aconst + Adst * (1 - Asrc * Aconst) A atop B
    E_GE_ALPHA_ROP8_SRC_ATOP_DST    = 11,
    /// Asrc * Aconst * Adst + Asrc * Aconst * (1 - Adst) B atop A
    E_GE_ALPHA_ROP8_DST_ATOP_SRC    = 12,
    /// (1 - Adst) * Asrc * Aconst + Adst * (1 - Asrc * Aconst) A xor B
    E_GE_ALPHA_ROP8_SRC_XOR_DST     = 13,
    /// Asrc * Asrc * Aconst + Adst * (1 - Asrc * Aconst)
    E_GE_ALPHA_ROP8_INV_SRC_ATOP_DST= 14,
    /// Asrc * (1 - Asrc * Aconst) + Adst * Asrc * Aconst
    E_GE_ALPHA_ROP8_INV_DST_ATOP_SRC= 15,
} GE_AlphaSrc;


//-------------------------------------------------
/// Define Virtual Command Buffer Size
typedef enum
{
    /// 4K
    E_GE_VCMD_4K = 0,
    /// 8K
    E_GE_VCMD_8K = 1,
    /// 16K
    E_GE_VCMD_16K = 2,
    /// 32K
    E_GE_VCMD_32K = 3,
    /// 64K
    E_GE_VCMD_64K = 4,
    /// 128K
    E_GE_VCMD_128K = 5,
    /// 256K
    E_GE_VCMD_256K = 6,
    /// 512K
    E_GE_VCMD_512K = 7,
    /// 1024k
    E_GE_VCMD_1024K = 8,
} GE_VcmqBufSize;

/// Define DFB Blending Related:
typedef enum
{
    E_GE_DFB_BLD_FLAG_COLORALPHA         = 0x0001,
    E_GE_DFB_BLD_FLAG_ALPHACHANNEL       = 0x0002,
    E_GE_DFB_BLD_FLAG_COLORIZE           = 0x0004,
    E_GE_DFB_BLD_FLAG_SRCPREMUL          = 0x0008,
    E_GE_DFB_BLD_FLAG_SRCPREMULCOL       = 0x0010,
    E_GE_DFB_BLD_FLAG_DSTPREMUL          = 0x0020,
    E_GE_DFB_BLD_FLAG_XOR                 = 0x0040,
    E_GE_DFB_BLD_FLAG_DEMULTIPLY         = 0x0080,
    E_GE_DFB_BLD_FLAG_SRCALPHAMASK       = 0x0100,
    E_GE_DFB_BLD_FLAG_SRCCOLORMASK       = 0x0200,
    E_GE_DFB_BLD_FLAG_ALL                 = 0x03FF,
}GE_DFBBldFlag;

typedef enum
{
    E_GE_DFB_BLD_OP_ZERO               = 0,
    E_GE_DFB_BLD_OP_ONE                = 1,
    E_GE_DFB_BLD_OP_SRCCOLOR          = 2,
    E_GE_DFB_BLD_OP_INVSRCCOLOR       = 3,
    E_GE_DFB_BLD_OP_SRCALPHA          = 4,
    E_GE_DFB_BLD_OP_INVSRCALPHA       = 5,
    E_GE_DFB_BLD_OP_DESTALPHA         = 6,
    E_GE_DFB_BLD_OP_INVDESTALPHA      = 7,
    E_GE_DFB_BLD_OP_DESTCOLOR         = 8,
    E_GE_DFB_BLD_OP_INVDESTCOLOR      = 9,
    E_GE_DFB_BLD_OP_SRCALPHASAT       = 10,
}GE_DFBBldOP;

///Define POINT
typedef struct
{
    MS_U16                          x;
    MS_U16                          y;

} GE_Point;


///Define the RECT
typedef struct
{
    /// X start address
    MS_U16                          x;
    /// Y start address
    MS_U16                          y;
    /// width
    MS_U16                          width;
    /// height
    MS_U16                          height;

} GE_Rect;

/*******************************************************************
// Define the Trapezoid                                            *
// [u16X0, u16Y0]: The Left point of top of Trapezoid;             *
// [u16X1, u16Y1]: The Left point of bottom of Trapezoid;          *
// [u16DeltaTop]: The Width of top of Trapezoid;                   *
// [u16DeltaBottom]: The Width of bottom of Trapezoid;             *
********************************************************************/
typedef struct
{
    /// x0 start address
    MS_U16                          u16X0;
    /// y0 start address
    MS_U16                          u16Y0;
    /// x1 start address
    MS_U16                          u16X1;
    /// y1 start address
    MS_U16                          u16Y1;
    /// delta of Top
    MS_U16                          u16DeltaTop;
    // delta of Bottom
    MS_U16                          u16DeltaBottom;
} GE_Trapezoid;

/****************************************************************************
*** Define the Normalized Implement definition of Trapezoid                 *
// [u16X0, u16Y0]: The Top-Left point of Trapezoid;                         *
// [u16X1]: DIRECTION_X-X of Top-Right Point; DIRECTION_Y-X of Bottom Edge; *
// [u16Y1]: DIRECTION_X-Y of Bottom Edge; DIRECTION_Y-Y of Top-Right Point; *
// [u32DeltaL]: The Delta Ratio of Left Edge;                               *
// [u32DeltaR]: The Delta Ratio of Right Edge;                              *
****************************************************************************/
typedef struct
{
    /// x0 start address
    MS_U16                          u16X0;
    /// y0 start address
    MS_U16                          u16Y0;
    /// x1 start address
    MS_U16                          u16X1;
    /// y1 start address
    MS_U16                          u16Y1;
    /// delta of x0
    MS_U32                          u32DeltaL;
    // delta of x1
    MS_U32                          u32DeltaR;
} GE_Normalized_Trapezoid;

typedef struct
{
    /// Destination block
    union
    {
        GE_Rect dstblk;
        GE_Trapezoid dsttrapeblk;
    };

}GE_DstBitBltType;


///Define the RECT
typedef struct
{
    /// STBB INIT X
    MS_U32                          init_x;
    /// STBB INIT Y
    MS_U32                          init_y;
    /// STBB X
    MS_U32                          x;
    /// STBB Y
    MS_U32                          y;

} GE_ScaleInfo;


///Define Buffer Format Information
typedef struct
{
    /// Specified format
    GE_BufFmt                       fmt;

    /// Base alignment in byte unit
    MS_U8                           u8BaseAlign;
    /// Pitch alignment in byte unit
    MS_U8                           u8PitchAlign;
    /// Non-1p mode base/pitch alignment in byte unit
    MS_U8                           u8Non1pAlign;
    /// Height alignment in pixel unit <b>(RESERVED)</b>
    MS_U8                           u8HeightAlign;
    /// StretchBlt base/pitch alignment in byte unit
    MS_U8                           u8StretchAlign;
    /// Tile mode base alignment in byte unit <b>(RESERVED)</b>
    MS_U8                           u8TileBaseAlign;
    /// Tile mode width alignment in pixel unit <b>(RESERVED)</b>
    MS_U8                           u8TileWidthAlign;
    /// Tile mode height alignment in pixel unit <b>(RESERVED)</b>
    MS_U8                           u8TileHeightAlign;

    /// Format capability flags <b>(RESERVED)</b>
    MS_U32                          u32CapFlags;

} GE_FmtCaps;


///Define Src/Dst Buffer Information
typedef struct
{
    /// Specified format
    GE_BufFmt                       srcfmt;
    /// Specified format
    GE_BufFmt                       dstfmt;
    /// Base alignment in byte unit
    MS_U16                          srcpit;
    /// Pitch alignment in byte unit
    MS_U16                          dstpit;
    /// Height alignment in pixel unit <b>(RESERVED)</b>
    MS_PHY                        srcaddr;
    /// StretchBlt base/pitch alignment in byte unit
    MS_PHY                        dstaddr;
    /// GE TLB Mode
    GE_TLB_Mode                     tlbmode;
    /// GE TLB Entry source base address
    MS_PHY                        tlbsrcaddr;
    /// GE TLB Entry destination base address
    MS_PHY                        tlbdstaddr;
    /// GE TLB Entry Flush Table
    MS_BOOL                         bEnflushtlbtable;
} GE_BufInfo;


/// GE dbg info
typedef struct
{
    /// Specified format
    MS_U8                       verctrl[32];
    /// Specified format
    MS_U16                       gedump[0x80];
    /// Base alignment in byte unit
    MS_U32                      semstaus;
} GE_DbgInfo;


typedef struct
{
    MS_PHY PhyAddr;
    GE_VcmqBufSize enBufSize;
} GE_VcmqBuffer;


/// GE SHM info
typedef struct
{
    /// Upon evey init of shm handle, the counter will be increased by 1
    MS_U32                       u32cnt;
    /// Last entity id which invoke the GE semaphore.
    MS_U32                       u32shmid;
} __attribute__((__packed__)) GE_ShmInfo;


//-------------------------------------------------
/// Define RGB color in LE
typedef struct
{
    /// Blue
    MS_U8 b;
    /// Green
    MS_U8 g;
    /// Red
    MS_U8 r;
    /// Alpha
    MS_U8 a;
} GE_RgbColor;


//-------------------------------------------------
/// Specify the blink data
        //          1 A B Fg Bg
        //          1 2 3  5  5
typedef struct
{
    /// BG color (for blink mode use)
    MS_U8                  background;             // 5 bits
    /// FG color (for blink mode use)
    MS_U8                  foreground;             // 5 bits
    /// Control flag\n
    union{
        MS_U16             ctrl_flag;
        struct{
            MS_U16         Blink:3;                // 3 bits
            MS_U16         Alpha:2;                // 2 bits
            MS_U16         blink_en:1;
            MS_U16         reserved1:10;
        } Bits;
        struct{
            MS_U16 BlinkAlpha:4;
            MS_U16 Alpha:2;
            MS_U16 Blink:3;
            MS_U16 reserved:7;
        }Bits2;
        struct{
            MS_U16 Fa:2;
            MS_U16 Ba:2;
            MS_U16 reserved:12;

        }Bits3;
    };


} GE_BlinkData;


/// define MHEG5 draw oval information
typedef struct
{
    /// dst block info
    GE_Rect       dstBlock;
    /// Color format
    GE_BufFmt fmt;
    union
    {
        GE_RgbColor color;
        GE_BlinkData blink_data;
    };
    MS_U32 u32LineWidth;
} GE_OVAL_FILL_INFO;


///Define Buffer Usage Type
typedef enum
{
    /// Desitination buffer for LINE, RECT, BLT
    E_GE_BUF_DST                    = 0,
    /// Source buffer for BLT
    E_GE_BUF_SRC                    = 1,
} GE_BufType;


///Define RGB2YUV conversion formula
typedef enum
{
    E_GE_YUV_RGB2YUV_PC             = 0,
    E_GE_YUV_RGB2YUV_255            = 1,

} GE_Csc_Rgb2Yuv;

///Define output YUV color domain
typedef enum
{
    E_GE_YUV_OUT_255                = 0,
    E_GE_YUV_OUT_PC                 = 1,

} GE_YUV_OutRange;

///Define input YUV color domain
typedef enum
{
    E_GE_YUV_IN_255                 = 0,
    E_GE_YUV_IN_127                 = 1,

} GE_YUV_InRange;

///Define YOU 422 format
typedef enum
{
    E_GE_YUV_YVYU                   = 0,
    E_GE_YUV_YUYV                   = 1,
    E_GE_YUV_VYUY                   = 2,
    E_GE_YUV_UYVY                   = 3,

} GE_YUV_422;

///Define paramter for YUV operation
typedef struct
{
    GE_Csc_Rgb2Yuv                  rgb2yuv;
    GE_YUV_OutRange                 out_range;
    GE_YUV_InRange                  in_range;
    GE_YUV_422                      dst_fmt;
    GE_YUV_422                      src_fmt;

} GE_YUVMode;


typedef enum
{
    E_GE_WP_IN_RANGE                = 0,
    E_GE_WP_OUT_RANGE               = 1,
    E_GE_WP_DISABLE                 = 0xFF,

} GE_WPType;


typedef struct
{
   void*            pInstance;
   MS_U32           u32GE_DRV_Version;
   MS_BOOL          bGEMode4MultiProcessAccess;
   MS_S32           s32CurrentProcess;
   MS_S32           s32CurrentThread;
   GE_BufInfo       pBufInfo;
}GE_Context;


typedef struct
{
    MS_U32              flags;
    GE_Rect             src;
    GE_DstBitBltType    dst;
    GE_ScaleInfo*       scaleinfo;
}PatchBitBltInfo;

typedef enum
{
   E_GE_DONOTHING = 0,
   E_GE_NEAREST = 1,
   E_GE_REPLACE_KEY_2_CUS = 2,

}GE_StretchCKType;

typedef enum
{
    E_GE_ROTATE_0 = 0,
    E_GE_ROTATE_90 = 1,
    E_GE_ROTATE_180 = 2,
    E_GE_ROTATE_270 = 3,
} GE_RotateAngle;

typedef struct
{
    MS_U16 GE0_Reg[0x80];
    MS_U16 GETLB_Reg[0x80];
    MS_U16 GECLK_Reg;
    MS_U32 GEPalette[256];
}GE_STR_SAVE_AREA;

typedef enum
{
    E_GE_SAVE_REG_GE_EN        = 0x1000,
    E_GE_RESTORE_REG_GE_EN     = 0x2000,
    E_GE_DISABLE_REG_EN        = 0x3000,
}GE_RESTORE_REG_ACTION;
//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

// GE Draw Functions
GE_Result GE_Get_Resource(GE_Context *pGECtx, MS_BOOL bHWSemLock);
GE_Result GE_Free_Resource(GE_Context *pGECtx, MS_BOOL bHWSemLock);


void MDrv_GE_Exit(void* pInstance);
GE_Result MDrv_GE_Reset(GE_Context *pGECtx);
GE_Result MDrv_GE_SetDither(GE_Context *pGECtx, MS_BOOL enable);
GE_Result MDrv_GE_SetOnePixelMode(GE_Context *pGECtx, MS_BOOL enable);
GE_Result MDrv_GE_SetSrcColorKey(GE_Context *pGECtx, MS_BOOL enable, GE_CKOp ck_mode, MS_U32 ck_low, MS_U32 ck_high);
GE_Result MDrv_GE_SetDstColorKey(GE_Context *pGECtx, MS_BOOL enable, GE_CKOp ck_mode, MS_U32 ck_low, MS_U32 ck_high);
GE_Result MDrv_GE_SetIntensity(GE_Context *pGECtx, MS_U32 id, MS_U32 color);
GE_Result MDrv_GE_GetIntensity(GE_Context *pGECtx, MS_U32 id, MS_U32 *color);
GE_Result MDrv_GE_SetROP2(GE_Context *pGECtx, MS_BOOL enable, GE_Rop2 eRop2);
GE_Result MDrv_GE_SetLinePattern(GE_Context *pGECtx, MS_BOOL enable, MS_U8 pattern, GE_LinePatRep eRep);
GE_Result MDrv_GE_ResetLinePattern(GE_Context *pGECtx);
GE_Result MDrv_GE_SetAlphaBlend(GE_Context *pGECtx, MS_BOOL enable, GE_BlendOp eBlendCoef);
GE_Result MDrv_GE_SetAlphaBlendCoef(GE_Context *pGECtx, GE_BlendOp eBlendOp);
GE_Result MDrv_GE_SetAlphaSrc(GE_Context *pGECtx, GE_AlphaSrc eAlphaSrc);
GE_Result MDrv_GE_SetAlphaCmp(GE_Context *pGECtx, MS_BOOL enable, GE_ACmpOp eACmpOp);
GE_Result MDrv_GE_SetAlphaTest(GE_Context *pGECtx, MS_BOOL enable, GE_ATestOp ck_mode, MS_U8 a_low, MS_U8 a_high);
GE_Result MDrv_GE_SetAlphaConst(GE_Context *pGECtx, MS_U8 u8AlphaConst);
GE_Result MDrv_GE_QueryDFBBldCaps(GE_Context *pGECtx, MS_U16 *pU16SupportedBldFlags);
GE_Result MDrv_GE_EnableDFBBlending(GE_Context *pGECtx, MS_BOOL enable);
GE_Result MDrv_GE_SetDFBBldFlags(GE_Context *pGECtx, MS_U16 u16DFBBldFlags);
GE_Result MDrv_GE_SetDFBBldOP(GE_Context *pGECtx, GE_DFBBldOP geSrcBldOP, GE_DFBBldOP geDstBldOP);
GE_Result MDrv_GE_SetDFBBldConstColor(GE_Context *pGECtx, GE_RgbColor geRgbColor);
GE_Result MDrv_GE_SetDFBBldSrcColorMask(GE_Context *pGECtx, GE_RgbColor geRgbColor);
GE_Result MDrv_GE_EnableTrapezoidAA(GE_Context *pGECtx, MS_BOOL bEnable);
GE_Result MDrv_GE_EnableTrapSubPixCorr(GE_Context *pGECtx, MS_BOOL bEnable);
GE_Result MDrv_GE_SetClipWindow(GE_Context *pGECtx, GE_Rect *rect);
GE_Result MDrv_GE_GetClipWindow(GE_Context *pGECtx, GE_Rect *rect);
GE_Result MDrv_GE_SetItalic(GE_Context *pGECtx, MS_U8 x_offset, MS_U8 y_offset, MS_U8 delta);
GE_Result MDrv_GE_SetPalette(GE_Context *pGECtx, MS_U16 start, MS_U16 num, MS_U32 *palette);
GE_Result MDrv_GE_GetPalette(GE_Context *pGECtx, MS_U16 start, MS_U16 num, MS_U32 *palette);
GE_Result MDrv_GE_SetYUVMode(GE_Context *pGECtx, GE_YUVMode *mode);
GE_Result MDrv_GE_SetSrcBuffer(GE_Context *pGECtx, GE_BufFmt src_fmt, MS_U16 pix_width, MS_U16 pix_height, MS_PHY addr, MS_U16 pitch, MS_U32 flags);
GE_Result MDrv_GE_SetDstBuffer(GE_Context *pGECtx, GE_BufFmt dst_fmt, MS_U16 pix_width, MS_U16 pix_height, MS_PHY addr, MS_U16 pitch, MS_U32 flags);
GE_Result MDrv_GE_DrawLine(GE_Context *pGECtx, GE_Point *v0, GE_Point *v1, MS_U32 color, MS_U32 color2, MS_U32 flags, MS_U32 width);
GE_Result MDrv_GE_FillRect(GE_Context *pGECtx, GE_Rect *rect, MS_U32 color, MS_U32 color2, MS_U32 flags);
GE_Result MDrv_GE_FillTrapezoid(GE_Context *pGECtx,GE_Trapezoid *pGeTrapezoid, MS_U32 u32ColorS, MS_U32 u32ColorE, MS_U32 u32Flags);
GE_Result MDrv_GE_WaitIdle(GE_Context *pGECtx);
GE_Result MDrv_GE_GetBufferInfo(GE_Context *pGECtx, GE_BufInfo *bufinfo);
GE_Result MDrv_GE_BitBltEX(GE_Context *pGECtx,GE_Rect *src, GE_DstBitBltType *dst, MS_U32 flags, GE_ScaleInfo* scaleinfo);
GE_Result MDrv_GE_BitBltEX_Trape(GE_Context *pGECtx,GE_Rect *pSrcRect, GE_DstBitBltType *pDstRect, MS_U32 u32Flags, GE_ScaleInfo* pScaleinfo);
GE_Result MDrv_GE_BitBltByTwoSourceBuffer(GE_Context *pGECtx, GE_BufFmt dst_fmt, GE_Rect *pSrc0Rect, GE_Rect *pSrc1Rect, GE_Rect *pDestRect, MS_PHY addr, MS_U16 pitch, MS_U32 flags, GE_ScaleInfo* scaleinfo);
// GE Utility Functions
GE_Result MDrv_GE_GetFmtCaps(GE_Context *pGECtx, GE_BufFmt fmt, GE_BufType type, GE_FmtCaps *caps);
GE_Result MDrv_GE_Init(void* pInstance, GE_Config *cfg, GE_Context **ppGECtx);
GE_Result MDrv_GE_GetConfig(GE_Config *cfg);
GE_Result MDrv_GE_WriteProtect(GE_Context *pGECtx, MS_U8 miu, MS_PHY addr_low, MS_PHY addr_high, GE_WPType eWPType);
GE_Result MDrv_GE_SetAlpha_ARGB1555(GE_Context *pGECtx, MS_U8 coef);
GE_Result MDrv_GE_GetAlpha_ARGB1555(GE_Context *pGECtx, MS_U8* coef);

GE_Result MDrv_GE_SetTAGID(GE_Context *pGECtx, MS_U16 u16tag);
GE_Result MDrv_GE_GetTAGID(GE_Context *pGECtx, MS_U16* u16tag);
GE_Result MDrv_GE_WaitTAGID(GE_Context *pGECtx, MS_U16 tagID);
GE_Result MDrv_GE_PollingTAGID(GE_Context *pGECtx, MS_U16 tagID);
GE_Result MDrv_GE_GetNextTAGID(GE_Context *pGECtx, MS_BOOL bStepTagBefore, MS_U16* u16NextTagID);
GE_Result MDrv_GE_SetNextTAGID(GE_Context *pGECtx, MS_U16 *tagID);
GE_Result MDrv_GE_SetDbgLevel(GE_Context *pGECtx, MS_U32 level);
GE_Result MDrv_GE_GetInfo(GE_Context *pGECtx, GE_DbgInfo* dbg);
#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
GE_Result MDrv_GE_GetGFXMdbInfo(GE_Context *pGECtx, MS_U64 *u64ReqHdl);
#endif
GE_Result MDrv_GE_GetStatus(GE_Context *pGECtx, MS_BOOL* bstatus);
GE_Result MDrv_GE_EnableVCmdQueue(GE_Context *pGECtx, MS_U16 blEnable);
GE_Result MDrv_GE_SetVCmdBuffer(GE_Context *pGECtx, MS_PHY PhyAddr, GE_VcmqBufSize enBufSize);
GE_Result MDrv_GE_SetVCmd_W_Thread(GE_Context *pGECtx, MS_U8 u8W_Threshold);
GE_Result MDrv_GE_SetVCmd_R_Thread(GE_Context *pGECtx, MS_U8 u8R_Threshold);
GE_Result MDrv_GE_GetLibVer(GE_Context *pGECtx, const MSIF_Version **ppVersion);
MS_U32    GE_Divide2Fixed(MS_U16 u16x, MS_U16 u16y, MS_U8 nInteger, MS_U8 nFraction);
GE_Result MDrv_GE_SetStrBltSckType(GE_Context *pGECtx,GE_StretchCKType TYPE, MS_U32 *CLR);
GE_Result MDrv_GE_Get_GetHK(GE_Context *pGECtx, MS_BOOL *bIsHK) ;
GE_Result MDrv_GE_Set_SetHK(GE_Context *pGECtx, MS_BOOL bIsHK) ;
GE_Result MDrv_GE_DrawOval(GE_Context *pGECtx, GE_OVAL_FILL_INFO* pOval);
MS_U8  _GFXAPI_MIU_ID(MS_PHY ge_fbaddr);
MS_PHY _GFXAPI_PHYS_ADDR_IN_MIU(MS_PHY ge_fbaddr) ;
MS_PHY _GFXAPI_PHYS_ADDR_2_API(MS_U8 u8MIUId, MS_PHY ge_addrInMIU);
GE_Result MDrv_GE_SetClock(GE_Context *pGECtx, MS_BOOL bEnable);
GE_Result MDrv_GE_BitbltPerformance(GE_Context *pGECtx);
GE_Result MDrv_GE_Chip_Proprity_Init(GE_Context *pGECtx, GE_CHIP_PROPERTY **pGeChipPro);
MS_BOOL _GE_TextOutPatch(GE_Context *pGECtx);
GE_Result MDrv_GE_SetPowerState(GE_Context *pGECtx, EN_POWER_MODE u16PowerState, void* pModule);
GE_Result MDrv_GE_STR_SetPowerState(EN_POWER_MODE u16PowerState, void* pModule);
GE_Result MDrv_GE_SetTLBMode(GE_Context *pGECtx, GE_TLB_Mode tlb_type);
GE_Result MDrv_GE_SetTLBSrcBaseAddr(GE_Context *pGECtx, MS_PHY phyaddr);
GE_Result MDrv_GE_SetTLBDstBaseAddr(GE_Context *pGECtx, MS_PHY phyaddr);
GE_Result MDrv_GE_TLBFlushTable(GE_Context *pGECtx,MS_BOOL bEnable);
GE_Result MDrv_GE_SetTLBTag(GE_Context *pGECtx,MS_U16 tag);
GE_Result MDrv_GE_GetTLBSRCADDR(GE_Context *pGECtx, MS_PHY* addr);
GE_Result MDrv_GE_GetTLBDSTADDR(GE_Context *pGECtx, MS_PHY* addr);
GE_Result MDrv_GE_StopTLBFlush(GE_Context *pGECtx);
GE_Result MDrv_GE_SetBurstMiuLen(GE_Context *pGECtx,MS_BOOL bEnable,MS_U32 u32BurstLen);
GE_Result MDrv_GE_RestoreRegInfo(GE_Context *pGECtx,GE_RESTORE_REG_ACTION eRESTORE_ACTION,MS_U16 *value);

#ifdef __cplusplus
}
#endif



#endif // _DRV_GE_H_

