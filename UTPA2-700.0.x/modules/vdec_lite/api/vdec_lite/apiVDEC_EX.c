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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    apiVDEC_EX.c
/// @brief  VDEC EXTENSION API FOR DUAL STREAMS
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "MsCommon.h"
#include "MsVersion.h"
#include "apiVDEC_EX.h"
#include "apiMJPEG_EX.h"
#include "drvMMIO.h"
#include "MsOS.h"
#include "utopia.h"
#include "vdec_ex_private.h"
#include "drvSYS.h"
#include "tee_client_api.h"

#if defined(MSOS_TYPE_LINUX)
#if 1
#define CMA_DRV_DIRECT_INIT
#else
#define CMA_FW_INIT
#endif
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <asm/div64.h>
#endif
#ifdef _HVD_EVENT_DBG
#include "apiDMX.h"
#endif
#include "drvSYS.h"
#include "apiMBX_St.h"
#include "apiMBX.h"
#include "drvCMAPool.h"
#include "halCHIP.h"

#if defined(MSOS_TYPE_LINUX)
#include "msos/linux/mdrv_cma_pool_st.h"
#endif


//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
#define VDEC_ENABLE_LINK_WEAK_SYMBOL    1
#define VDEC_ENABLE_MVD     1
#define VDEC_ENABLE_HVD     1
#define VDEC_ENABLE_MVC     1

#if defined(CHIP_KANO) || defined(CHIP_CURRY)
#define VDEC_DTV_DIRECT_STC_PATCH     0
#define VDEC_EXCHANGE_CIDX_PATCH      1
#define VDEC_DUAL_FRAME_BUFFER_MANAGEMENT  1
#endif

#if defined(VESTEL_FEATURE)
    #define VESTEL_FEATURE_ENABLE 1
#else
    #define VESTEL_FEATURE_ENABLE 0
#endif

#if VDEC_ENABLE_LINK_WEAK_SYMBOL
    #include "_apiVDEC_EX_weak.h"
#else
    #include "../../drv/mvd_lite/drvMVD_EX.h"
    #include "../../drv/hvd_lite/drvHVD_EX.h"
#endif

#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE)) || defined(SUPPORT_X_MODEL_FEATURE)

#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL) || defined(MSOS_TYPE_ECOS)
    #define VDEC_EX_ISR_MONITOR     1
#else
    #define VDEC_EX_ISR_MONITOR     0
#endif

#ifdef VDEC_UTOPIA_2K
    #define VDEC_DRIVER_BASE KERNEL_MODE
#else
    #define VDEC_DRIVER_BASE 0
#endif

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifndef ANDROID
    #if ((defined(CHIP_A1) || defined(CHIP_A7) || defined(CHIP_AMETHYST)) && defined (__aeon__))
        #define VDEC_INFO(idx, x)
        #define VDEC_ERR(x, ...)
        #define VDEC_PRINT printf
    #elif defined(MSOS_TYPE_ECOS)
        #define VPRINTF diag_printf
        #define VDEC_INFO   diag_printf
        #define VDEC_ERR    diag_printf
        #define VDEC_PRINT  diag_printf
    #else
        #define VDEC_INFO(idx, x)       if (pVdecExContext->_Attr[idx].eDbgMsgLevel >= E_VDEC_EX_DBG_LEVEL_INFO) { (x); }
        #define VDEC_ERR                printf
        #define VDEC_PRINT printf
    #endif
#else
    #define VDEC_INFO(idx, x)       if (pVdecExContext->_Attr[idx].eDbgMsgLevel >= E_VDEC_EX_DBG_LEVEL_INFO) { (x); }
    #include <sys/mman.h>
    #include <cutils/ashmem.h>
    #include <cutils/log.h>
    #define VDEC_PRINT ALOGD
    #define VDEC_ERR   ALOGD
#endif


#ifdef MSOS_TYPE_LINUX_KERNEL
    #define VPRINTF  printk
#elif defined(MSOS_TYPE_ECOS)
    #define VPRINTF diag_printf
#else
    #ifndef ANDROID
        #define VPRINTF printf
    #else
        #define VPRINTF ALOGD
    #endif
#endif

#ifdef VDEC3
#define v3_temp 1
#define v3_thinplayer 0
#else
#define v3_temp 0
#define v3_thinplayer 0
#endif

#define ES_BUFFER_STATUS_UNKNOWN        0
#define ES_BUFFER_STATUS_UNDERFLOW      1
#define ES_BUFFER_STATUS_OVERFLOW       2
#define ES_BUFFER_STATUS_NORMAL         3
#ifdef VDEC3
#define  _VDEC_GET_DRV_IDX(x)    (MS_U8)((x) >> 16)
#define VDEC_MAX_SUPPORT_STREAM_NUM     16
#else
#define VDEC_MAX_SUPPORT_STREAM_NUM     2
#endif
#define MVD_SRCMODE_MAX 5
#define _VDEC_GET_IDX(x)    (MS_U8)((x)>>24)
#define _VDEC_GET_DRV_ID(x) ((x) & 0x00ffffffUL)
#define _VDEC_SHM_POINTER_CHECK(x)   \
    do\
    {\
        if (pVdecExContext == NULL)\
        {\
            return x;\
        }\
    }while(0)
#define _VDEC_INVALID_IDX_RET(x)                                                                    \
    do                                                                                              \
    {                                                                                               \
        if (x >= VDEC_MAX_SUPPORT_STREAM_NUM)                                                       \
        {                                                                                           \
            return E_VDEC_EX_RET_INVALID_PARAM;                                                     \
        }                                                                                           \
    } while(0)

#define _VDEC_INVALID_STREAM_ID_RET(x)                                                              \
    do                                                                                              \
    {                                                                                               \
        if (x == 0)                                                                                   \
        {                                                                                           \
            return E_VDEC_EX_RET_ILLEGAL_ACCESS;                                                    \
        }                                                                                           \
    } while(0)

#define _VDEC_NOT_INIT_RET(x)                                \
      do {                                                   \
            if (!(pVdecExContext->_Attr[x].bInit))                             \
            {                                                \
                return E_VDEC_EX_RET_NOT_INIT;               \
            }                                                \
      } while(0)

#define _VDEC_INVALID_DRVID_RET(x, id)                                                              \
    do                                                                                              \
    {                                                                                               \
        if (pVdecExContext->_Attr[x].u32DrvId != id)                                                                \
        {                                                                                           \
            return E_VDEC_EX_RET_INVALID_PARAM;                                                     \
        }                                                                                           \
    } while(0)

#define _VDEC_Memset(pDstAddr, u8value, u32Size)                \
    do {                                                        \
        MS_U32 i = 0;                                           \
        volatile MS_U8 *dst = (volatile MS_U8 *)(pDstAddr);     \
        for (i = 0; i < (u32Size); i++)                         \
        {                                                       \
            dst[i] = (u8value);                                 \
        }                                                       \
    } while (0)

#define _VDEC_Memcpy(pDstAddr, pSrcAddr, u32Size)               \
    do {                                                        \
        MS_U32 i = 0;                                           \
        volatile MS_U8 *dst = (volatile MS_U8 *)(pDstAddr);     \
        volatile MS_U8 *src = (volatile MS_U8 *)(pSrcAddr);     \
        for (i = 0; i < (u32Size); i++)                         \
        {                                                       \
            dst[i] = src[i];                                    \
        }                                                       \
    } while (0)

#define _MVD_RET_HANDLE(x)          \
    do                              \
    {                               \
        if (E_MVD_RET_OK != (x))    \
        {                           \
            return E_VDEC_EX_FAIL;  \
        }                           \
    } while (0)

#define _HVD_RET_HANDLE(x)          \
    do                              \
    {                               \
        if (E_HVD_EX_OK != (x))     \
        {                           \
            return E_VDEC_EX_FAIL;  \
        }                           \
    } while (0)

#define _MJPEG_RET_HANDLE(x)                \
    do                                      \
    {                                       \
        _VDEC_EX_API_MutexLock();\
        if (E_MJPEG_RET_SUCCESS != (x))     \
        {                                   \
            _VDEC_EX_API_MutexUnlock();\
            return E_VDEC_EX_FAIL;          \
        }                                   \
        _VDEC_EX_API_MutexUnlock();\
    } while (0)

#define _MVD_RET_TO_VDEC_RESULT(rst, b)          \
    do                              \
    {                               \
        if (E_MVD_RET_OK != (b))    \
        {                           \
            rst = E_VDEC_EX_FAIL;  \
        }                           \
        else                            \
        {                               \
            rst = E_VDEC_EX_OK;         \
        }                               \
    } while (0)

#define _HVD_RET_TO_VDEC_RESULT(rst, b)          \
    do                              \
    {                               \
        if (E_HVD_EX_OK != (b))    \
        {                           \
            rst = E_VDEC_EX_FAIL;  \
        }                           \
        else                            \
        {                               \
            rst = E_VDEC_EX_OK;         \
        }                               \
    } while (0)

#define _MJPEG_RET_TO_VDEC_RESULT(rst, b)          \
    do                              \
    {                               \
        if (E_MJPEG_RET_SUCCESS != (b))    \
        {                           \
            rst = E_VDEC_EX_FAIL;  \
        }                           \
        else                            \
        {                               \
            rst = E_VDEC_EX_OK;         \
        }                               \
    } while (0)

#define _BOOL_TO_VDEC_RESULT(rst, b)    \
    do                                  \
    {                                   \
        if (TRUE != (b))                \
        {                               \
            rst = E_VDEC_EX_FAIL;       \
        }                               \
        else                            \
        {                               \
            rst = E_VDEC_EX_OK;         \
        }                               \
    } while (0)

#define _BOOL_TO_MJPEG_RESULT(rst, b)   \
    do                                  \
    {                                   \
        if (TRUE != (b))                \
        {                               \
            rst = E_VDEC_EX_FAIL;       \
        }                               \
        else                            \
        {                               \
            rst = E_VDEC_EX_OK;         \
        }                               \
    } while (0)

#define _SET_DISPLAYMODE_INVALID(u8Idx) \
    do                                  \
    {                                   \
        pVdecExContext->_Pre_Ctrl[u8Idx].eDisplayMode = 0xFF;    \
    } while (0)


#if VDEC_ENABLE_LINK_WEAK_SYMBOL
#define _RET_VDEC_HVD_LINT_CHECK()                  \
    do                                              \
    {                                               \
        if (!MDrv_HVD_EX_Init)                      \
        {                                           \
            return E_VDEC_EX_RET_ILLEGAL_ACCESS;    \
        }                                           \
    } while (0)

#define _RET_VDEC_HVD_LINT_CHECK_AND_RETURN_MUTEX()                  \
    do                                                \
    {                                                \
        if (!MDrv_HVD_EX_Init)                        \
        {                                            \
            return E_VDEC_EX_RET_ILLEGAL_ACCESS;    \
        }                                            \
    } while (0)


#define _RET_VDEC_MVD_LINT_CHECK()                  \
    do                                              \
    {                                               \
        if (!MDrv_MVD_Init)                         \
        {                                           \
            return E_VDEC_EX_RET_ILLEGAL_ACCESS;    \
        }                                           \
    } while (0)

#define _RET_VDEC_MVD_LINT_CHECK_AND_RETURN_MUTEX()                  \
    do                                                \
    {                                                \
        if (!MDrv_MVD_Init)                         \
        {                                            \
            return E_VDEC_EX_RET_ILLEGAL_ACCESS;    \
        }                                            \
    } while (0)


#define _BOOL_VDEC_HVD_LINT_CHECK() \
    do                              \
    {                               \
        if (!MDrv_HVD_EX_Init)      \
        {                           \
            return FALSE;           \
        }                           \
    } while (0)

#define _BOOL_VDEC_MVD_LINT_CHECK() \
    do                              \
    {                               \
        if (!MDrv_MVD_Init)         \
        {                           \
            return FALSE;           \
        }                           \
    } while (0)
#else // VDEC_ENABLE_LINK_WEAK_SYMBOL

#define _RET_VDEC_MVD_LINT_CHECK()
#define _RET_VDEC_MVD_LINT_CHECK_AND_RETURN_MUTEX()
#define _RET_VDEC_HVD_LINT_CHECK()
#define _RET_VDEC_HVD_LINT_CHECK_AND_RETURN_MUTEX()
#define _BOOL_VDEC_MVD_LINT_CHECK()
#define _BOOL_VDEC_HVD_LINT_CHECK()

#endif //VDEC_ENABLE_LINK_WEAK_SYMBOL

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

#define VDEC_EX_STACK_SIZE 4096

#if (VDEC_EX_ISR_MONITOR)
#define E_VDEC_EX_EVENT_ISR_EVENT_CHANGE   0x80000000
#endif

#ifdef VDEC3
#define E_VDEC_EX_CB_MAX_NUM VDEC_MAX_SUPPORT_STREAM_NUM
#else
#define E_VDEC_EX_CB_MAX_NUM  (E_VDEC_EX_CB_SUB + 1)
#endif
#define VDEC_U32_MAX            0xffffffffUL
#define VDEC_U64_MAX            0xffffffffffffffffULL

#define MAX_VDEC_DQNUM          32
#define VDEC_DQ_EMPTY           0xff
#define ES_SAFE_SIZE            0x20

#if (VDEC_DUAL_FRAME_BUFFER_MANAGEMENT)
#define FBLOC_FRONT_TAIL_MASK      0x01
#define FBLOC_TASK_FB_MASK         0x10
#define FBLOC_FORCE_FRONT_MASK     0xF0
#define FBMNG_AVC4K_SIZE         0x8200000
#endif

#if 0
#define _VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,u32Arg_num,structArg_num,struct_size,pos_u32_0,pos_u32_1,pos_u32_2,pos_u32_3,pos_struct_0)\
        resume_data.u8Idx=u8Idx;\
        resume_data.cmd_id=cmd_id;\
        resume_data.param=param;\
        resume_data.u32Arg_num=u32Arg_num;\
        resume_data.structArg_num=structArg_num;\
        resume_data.struct_size=struct_size;\
        resume_data.position[0] = pos_u32_0;\
        resume_data.position[1] = pos_u32_1;\
        resume_data.position[2] = pos_u32_2;\
        resume_data.position[3] = pos_u32_3;\
        resume_data.position[4] = pos_struct_0;
#endif

//-------------------------------------------------------------------------------------------------
//  Local Enum
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_VDEC_EX_DECODER_MVD = 0,
    E_VDEC_EX_DECODER_HVD,
    E_VDEC_EX_DECODER_MJPEG,
    E_VDEC_EX_DECODER_NONE,
} VDEC_EX_Decoder;

typedef enum
{
    E_VDEC_EX_MVC_GET_BBU2_DECQ_VACANCY    = 0,
    E_VDEC_EX_MVC_GET_ES2_READ_PTR,
    E_VDEC_EX_MVC_GET_ES2_WRITE_PTR,

    E_VDEC_EX_MVC_SET_BBU2_PUSH_PACKET     = 0x100,
    E_VDEC_EX_MVC_SET_BBU2_FIRE_DECCMD,
} VDEC_MVC_Control_type;

// VDEC check capbility command
typedef enum
{
    E_VDEC_EX_CAP_XC_NOT_SUPPORT_DS  = 0,
    E_VDEC_EX_CAP_NUM,
} VDEC_EX_CapCmd;

typedef enum
{
    E_VDEC_EX_POWER_NONE,
    E_VDEC_EX_POWER_SUSPEND_ING,
    E_VDEC_EX_POWER_RESUME_ING,
    E_VDEC_EX_POWER_RESUME_DONE,

} VDEC_EX_PowerStatus_type;

// N Decode Buffer Allocator Enum Start
typedef enum
{
    E_VDEC_EX_CMA_POOL_BUFFER_FB1 = 0,
    E_VDEC_EX_CMA_POOL_BUFFER_FB2 = 1,
    E_VDEC_EX_CMA_POOL_MAX_SUPPORT_NUM = 2,
    E_VDEC_EX_CMA_POOL_INVALID = 0xFFFFFFFF,

} VDEC_EX_CMA_POOL_SELECT;

typedef enum
{
    E_VDEC_EX_ALLOCATOR_BUFFER_FB1 = 0,
    E_VDEC_EX_ALLOCATOR_BUFFER_FB2 = 1,
    E_VDEC_EX_ALLOCATOR_BUFFER_BS  = 2,

} VDEC_EX_ALLOCATOR_BUFFER_TYPE;

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

typedef struct
{
    MS_U32          u32EventBitMap;
    VDEC_EX_EventCb pVdecEventCb;
    void           *pParam;
} VDEC_EX_EventInfo;

#if (VDEC_EX_ISR_MONITOR)
// Info of Event Monitor
#ifdef VDEC3
typedef void (*MonitorISR_Proc)(MS_U8);
#else
typedef void (*MonitorISR_Proc)(void);
#endif

typedef struct
{
    MS_U8               u8VdecExStack[VDEC_EX_STACK_SIZE];
    MS_BOOL             bVdecExIsrMntr;
    MS_BOOL             bIsSysEnable;
    char                pu8VdecExEvent[20];
    MS_S32              s32VdecExTaskId;
    char                pu8VdecExTask[20];
    MonitorISR_Proc     pfMonISR_Proc;
} VDEC_EX_EventMonInfo;
#endif

typedef struct
{
    MS_BOOL             bUsed;
    MS_BOOL             bInit;
    VDEC_EX_DbgLevel    eDbgMsgLevel;
    VDEC_EX_CodecType   eCodecType;
    VDEC_EX_Decoder     eDecoder;
    VDEC_EX_SrcMode     eSrcMode;
    VDEC_EX_ErrCode     eErrCode;
    VDEC_EX_CCFormat    eCCFormat;      //unused
    VDEC_EX_EventInfo   vdecExEventInfo[E_VDEC_EX_CB_MAX_NUM];
#if (VDEC_EX_ISR_MONITOR)
    VDEC_EX_EventMonInfo stEventMon;
#endif
    VDEC_EX_InitParam   vdecExInitParam;
    MS_U32              u32DrvId;
    VDEC_EX_Stage  eStage;
    //MS_S32              s32VdecExMutexId;  //mutex ID
    //char                pu8VdecExMutex[20];    //mutex name
} VDEC_EX_Attr;

typedef struct
{
    //******** One Pending Buffer ********//
    MS_BOOL  bOnePendingBuffer;
    //******** HW Buffer ReMapping ********//
    MS_BOOL  bHWBufferReMapping;
    //******** Frame Rate Handling ********//
    MS_BOOL  bFrameRateHandling;
    MS_U32   u32PreSetFrameRate;
    MS_BOOL  bDisableTspInBbuMode;
    VDEC_EX_IapGnBufShareBWMode stIapGnShBWMode;
    VDEC_EX_MFCodec_mode eMFCodecMode;
    MS_BOOL  bForce8bitMode; //Only create 8bit YUV FrameBuffer
    VDEC_EX_Feature eVdecFeature; //BIT0=1:AP force VDEC only support HEVC Main profile file even chip spec is Main10
    VDEC_EX_DISPLAY_MODE eDisplayMode;
    MS_BOOL  bEnableDynamicCMA;   //enable dynamic cma
    //******** N Decode *******************//
    MS_BOOL bMonopolyBitstreamBuffer;
    MS_BOOL bMonopolyFrameBuffer;
    VDEC_EX_TASK_SPEC eTaskSpec;
    MS_BOOL bCalFrameRate;
    VDEC_EX_TotalBufRange stTotalBufRgnBS;
    VDEC_EX_TotalBufRange stTotalBufRgnFB1;
    VDEC_EX_TotalBufRange stTotalBufRgnFB2;
} VDEC_Pre_Ctrl;

// N Decode Buffer Allocator Structure Start

#define VDEC_EX_ALLOCATOR_ENABLE                   (TRUE)

#define VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED          ((MS_U16)0xFFFF)
#define VDEC_EX_ALLOCAOTR_ID_UNUSED                ((MS_U16)0xFFFF)

#define VDEC_EX_ALLOCAOTR_MAX_NUM_STREAM           VDEC_MAX_SUPPORT_STREAM_NUM
#define VDEC_EX_ALLOCAOTR_MAX_NUM_BLOCK            (128)       // Maximun number of blocks in block pool
#define VDEC_EX_ALLOCAOTR_MAX_NUM_ALLOCATOR        (32)        // Maximun number of allocators in allocator pool

#define VDEC_EX_ALLOCAOTR_PROPERTY_DEFAULT         (0)
#define VDEC_EX_ALLOCAOTR_PROPERTY_CAN_OVERLAP     (VDEC_EX_BIT(0)) // 1
#define VDEC_EX_ALLOCAOTR_PROPERTY_USECMA          (VDEC_EX_BIT(1)) // 2

#define VDEC_EX_ALLOCATOR_CRITERION_CHECK_NONE     (0x0)
#define VDEC_EX_ALLOCATOR_CRITERION_CHECK_ADDRESS  (VDEC_EX_BIT(0))
#define VDEC_EX_ALLOCATOR_CRITERION_CHECK_SIZE     (VDEC_EX_BIT(1))

// Patch: N Decode doesn't allocate framebuffer2 currently
#define VDEC_EX_ALLOCATOR_ENABLE_NDECODE_FB2       (FALSE)

// 16 bytes
typedef struct
{
    MS_U8   u8MIU;
    MS_U8   u8Property;
    MS_U16  u16Allocated;
    MS_PHY  phyAddr;            // start address of this block
    MS_SIZE szSize;             // total size of this block

} VDEC_EX_Allocator_BlockInfo;

// 24 bytes
typedef struct
{
    MS_U16  u16BlockId;         //
    MS_U16  u16BlockIdPrev;     // block ID of previous block using in list
    MS_U16  u16BlockIdNext;     // block ID of next     block using in list
    MS_U16  u16AllocatorIdExt;  // allocator ID of extend allocator
    VDEC_EX_Allocator_BlockInfo stBlockInfo;

} VDEC_EX_Allocator_Block;

// 6 bytes
typedef struct
{
    MS_U16 u16NumBlocks;        // total number of blocks using in list
    MS_U16 u16BlockIdHead;      // block ID of head block using in list
    MS_U16 u16BlockIdTail;      // block ID of tail block using in list

} VDEC_EX_Allocator_BlockList;

// 24 bytes
typedef struct
{
    MS_U16  u16AllocatorId;     //
    MS_PHY  phyStartAddr;       // start address of this memory section
    MS_SIZE szTotalSize;        // total size of this memory section
    MS_SIZE szUnusedSize;       // record unused size of this memory section
    VDEC_EX_Allocator_BlockList stBlockList;

} VDEC_EX_Allocator;

typedef struct
{
    MS_U16 u16BlockIdFB1Main;   // Frame Buffer1 Block ID in Main Allocator
    MS_U16 u16BlockIdFB1Sub;    // Frame Buffer1 Block ID in  Sub Allocator
    MS_U16 u16BlockIdFB2Main;   // Frame Buffer2 Block ID in Main Allocator
    MS_U16 u16BlockIdFB2Sub;    // Frame Buffer2 Block ID in  Sub Allocator
    MS_U16 u16BlockIdBS1Main;   // Bitstream Buffer Block ID in Main Allocator
    MS_U16 u16BlockIdBS1Sub;    // Bitstream Buffer Block ID in Main Allocator

} VDEC_EX_Allocator_Record;

typedef struct
{
    MS_BOOL bInit;
    MS_U16  u16AllocatorIdMain;
    VDEC_EX_Allocator_Record stAllocatorRecord[VDEC_MAX_SUPPORT_STREAM_NUM];
    VDEC_EX_Allocator_Block stBlockPool[VDEC_EX_ALLOCAOTR_MAX_NUM_BLOCK];
    VDEC_EX_Allocator stAllocatorPool[VDEC_EX_ALLOCAOTR_MAX_NUM_ALLOCATOR];
#ifdef CMA_DRV_DIRECT_INIT
    struct CMA_Pool_Init_Param stCmaInitParam[E_VDEC_EX_CMA_POOL_MAX_SUPPORT_NUM];
    MS_SIZE szCMAAllocLength[E_VDEC_EX_CMA_POOL_MAX_SUPPORT_NUM][VDEC_MAX_SUPPORT_STREAM_NUM];
#endif
} VDEC_EX_NDecAllocator;

typedef struct
{
    MS_U32  u32CheckItem;
    MS_PHY  phyAddr;
    MS_SIZE szSize;
    MS_U32  u32Alignment;

} VDEC_EX_Allocator_Criterion;

typedef struct
{
    MS_BOOL bCMA;
    MS_BOOL bDynamicCMA;
    MS_BOOL bNDecode;
    MS_BOOL bUseSubAllocator;
    MS_BOOL bUseThirdAllocator;
    MS_BOOL bMonopoly;
    VDEC_EX_SrcMode eSrcMode;
    VDEC_EX_CodecType eCodecType;
    VDEC_EX_TASK_SPEC eTaskSpec;

} VDEC_EX_NDecBufferProperty;

typedef struct
{
    MS_BOOL bSetTotalBuf;
    MS_PHY  phyTotalBufAddr;
    MS_SIZE szTotalBufSize;
    MS_PHY  phyBufAddr;
    MS_SIZE szBufSize;

} VDEC_EX_NDecBufRangeParam;

// N Decode Buffer Allocator Structure End
typedef struct
{
    MS_PHY  VPUBufAddr;
    MS_SIZE VPUBufSize;
    MS_PHY  BSBufAddr;
    MS_SIZE BSBufSize;
    MS_PHY  FBufAddr;
    MS_SIZE FBufSize;
    MS_U32  u32streamID;
    VDEC_EX_CodecType   eCodecType;
} VDEC_EX_OPTEE_SecureBuffer;

typedef struct
{
    MS_PHY  FBuf2Addr;
    MS_SIZE FBuf2Size;
    MS_U32  u32streamID;
} VDEC_EX_OPTEE_SecureFB2;

typedef void (*VDEC_EX_IsrHandle)(MS_U32 u32Sid);

#if VDEC_DUAL_FRAME_BUFFER_MANAGEMENT
typedef enum
{
    // 0xAB; A for FB0/1, B for Front/Tail
    E_VDEC_EX_FB0_FRONT = 0x00,
    E_VDEC_EX_FB0_TAIL  = 0x01,
    E_VDEC_EX_FB1_FRONT = 0x10,
    E_VDEC_EX_FB1_TAIL  = 0x11,
    E_VDEC_EX_FB_NONE   = 0xFF,
} VDEC_EX_FB_LOCATION;

typedef struct
{
    VDEC_EX_FB_LOCATION eLocation;  /// Using location.
    MS_U8               u8Miu;  //to distinguish MMAP_4K(fb on two miu) and MMAP_FHD(fb on the same miu)
    MS_PHY              u32FrameBufAddr;    /// Org frame buffer info.
    MS_SIZE             u32FrameBufSize;    /// Org frame buffer info.
} VDEC_EX_FB_MNG;
#endif

typedef struct
{
    // variables which need use mutex to protect
    MS_BOOL              _bSingleDecodeMode;
    MS_BOOL              bTurboMode;

    // variables which no need use mutex to protect
    VDEC_EX_Attr         _Attr[VDEC_MAX_SUPPORT_STREAM_NUM];
    VDEC_EX_IsrHandle    _pVdec_IsrProc[VDEC_MAX_SUPPORT_STREAM_NUM];
    MS_BOOL              _bEsBufMgmt[VDEC_MAX_SUPPORT_STREAM_NUM];
    VDEC_EX_DispFrame    _stDispQ[VDEC_MAX_SUPPORT_STREAM_NUM][MAX_VDEC_DQNUM];
    MS_U32               _u32VdecExIsrEventFlag[VDEC_MAX_SUPPORT_STREAM_NUM];
#if (VDEC_EX_ISR_MONITOR)
    MS_U32               _u32PreVdecExIsrEventFlag[VDEC_MAX_SUPPORT_STREAM_NUM];
#endif
    MS_BOOL              bDropErrFrm[VDEC_MAX_SUPPORT_STREAM_NUM];
    HVD_EX_FrmPackingSEI _stFrmPacking[VDEC_MAX_SUPPORT_STREAM_NUM];
    HVD_EX_ContentLightLevelInfoSEI _stCLLI[VDEC_MAX_SUPPORT_STREAM_NUM];
    HVD_EX_DisplayColourVolumeSEI _stDCV[VDEC_MAX_SUPPORT_STREAM_NUM];
    MS_BOOL             _bVdecDispOutSide[VDEC_MAX_SUPPORT_STREAM_NUM];
    MS_U32              u32PriData[VDEC_MAX_SUPPORT_STREAM_NUM];

    MS_BOOL             support_cc[VDEC_MAX_SUPPORT_STREAM_NUM];

    MS_BOOL             bFlush[VDEC_MAX_SUPPORT_STREAM_NUM];

    //char   _u8VDEC_Mutex[20];

    // for power state
    EN_POWER_MODE          _prev_u16PowerState;
    VDEC_EX_PowerStatus_type _power_state[VDEC_MAX_SUPPORT_STREAM_NUM];
    MS_U32                 u32ResumeStream[VDEC_MAX_SUPPORT_STREAM_NUM];
    VDEC_EX_CodecType      eCodecType[VDEC_MAX_SUPPORT_STREAM_NUM];
    MS_U8                  u8enable_store_set_cmd[VDEC_MAX_SUPPORT_STREAM_NUM];
    VDEC_EX_RECORD_SET_CMD store_set_cmd[MAX_NUM_OF_STORE_USER_SET_CMD]; // at most 100 set command
    MS_U8                  store_set_cmd_write_pointer[MAX_NUM_OF_STORE_USER_SET_CMD];
    MS_U8                  store_set_cmd_read_pointer[MAX_NUM_OF_STORE_USER_SET_CMD];
    VDEC_EX_FRC_OutputParam store_FRC_OutputParam[MAX_NUM_OF_STORE_USER_SET_CMD];
    VDEC_EX_Field_Polarity  store_Field_Polarity[MAX_NUM_OF_STORE_USER_SET_CMD];
    VDEC_EX_CC_InputPara    store_InputPara_setcfg[MAX_NUM_OF_STORE_USER_SET_CMD];
    VDEC_EX_CC_InputPara    store_InputPara_startaddr[MAX_NUM_OF_STORE_USER_SET_CMD];
    VDEC_EX_CC_InputPara    store_InputPara_readaddr[MAX_NUM_OF_STORE_USER_SET_CMD];

    MS_BOOL bEnableAutoInsertDummyPattern[VDEC_MAX_SUPPORT_STREAM_NUM];

    //pre_set
    VDEC_Pre_Ctrl       _Pre_Ctrl[VDEC_MAX_SUPPORT_STREAM_NUM];
    VDEC_EX_DecModCfg   _Pre_DecModeCfg;
    MS_BOOL           bTrickMode[VDEC_MAX_SUPPORT_STREAM_NUM];
#ifdef VDEC3
    MS_BOOL             bFWdecideFB;
    MS_BOOL             bDRVdecideBS;
    MS_PHY          u32BitstreamBufAddr;
    MS_U32              u32BitstreamBufSize;
#endif

#ifdef VDEC3
    MS_U32              u32FrameBaseMode[VDEC_MAX_SUPPORT_STREAM_NUM];
#endif

    MS_BOOL bCMAUsed;
    MS_BOOL bCMAInit;
#if defined(MSOS_TYPE_LINUX)
    struct CMA_Pool_Init_Param cmaInitParam[2]; // support two MIU
    struct CMA_Pool_Free_Param cmaFreeParam[2][VDEC_MAX_SUPPORT_STREAM_NUM];
    MS_BOOL bCMATwoMIU[VDEC_MAX_SUPPORT_STREAM_NUM];
#endif
#ifdef CMA_DRV_DIRECT_INIT
    MS_BOOL bCMAGetMem[2][VDEC_MAX_SUPPORT_STREAM_NUM];
#endif

    MS_BOOL bDirectStcThreadRunning[VDEC_MAX_SUPPORT_STREAM_NUM];
#if (VDEC_DUAL_FRAME_BUFFER_MANAGEMENT)
    VDEC_EX_FB_MNG _stFBMng[VDEC_MAX_SUPPORT_STREAM_NUM];
#endif

    // N Decode Buffer Allocator
    VDEC_EX_NDecAllocator stNDecAllocator;
    //MS_U32 u32MVDMinTspDataSize[VDEC_MAX_SUPPORT_STREAM_NUM];
} VDEC_EX_CTX;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
VDEC_EX_CTX* pVdecExContext = NULL;
VDEC_EX_CTX gVdecExContext;
MS_S32 s32VDECMutexID = -1;
MS_S32 s32VDECEVENTMutexID[VDEC_MAX_SUPPORT_STREAM_NUM] = {-1,-1};
MS_S32 s32VdecExTaskId[VDEC_MAX_SUPPORT_STREAM_NUM] = {-1,-1};
#ifdef VDEC3
MS_S32 s32VdecDirectStcId[VDEC_MAX_SUPPORT_STREAM_NUM] = {-1,-1,-1,-1};
#else
MS_S32 s32VdecDirectStcId[VDEC_MAX_SUPPORT_STREAM_NUM] = {-1,-1};
#endif
MS_U8  u8VdecDirectSTCStack[4096];

MS_U8 u8CodecCompare[VDEC_MAX_SUPPORT_STREAM_NUM] = {0};
MS_BOOL bCMAInitPool[2] = {0,0}; // each process should all init CMA

MS_S32 _s32VDEC_Callback_TaskId[VDEC_MAX_SUPPORT_STREAM_NUM] = {-1,-1};
MS_U8   u8VdecExCallbackStack[VDEC_MAX_SUPPORT_STREAM_NUM][VDEC_EX_STACK_SIZE];

//Disable MVD parser
const MS_U8 bMvdParserDisable[MVD_SRCMODE_MAX] =
{
    1, //E_MVD_FILE_MODE   : disable
    1, //E_MVD_SLQ_MODE    : disable
    0, //E_MVD_TS_MODE     : enable
    1, //E_MVD_SLQ_TBL_MODE: disable
    0  //E_MVD_TS_FILE_MODE: DISABLE_PKT_LEN
};
MS_BOOL  bIsSupportDivxPlus;
const MSIF_Version _api_vdec_version =
{
    .DDI = { VDEC_EX_API_VERSION, },
};
VDEC_EX_Info         _api_vdec_info;
void* pu32VDEC_EX_Inst = NULL;
VDEC_EX_DispFrame DispFrame_temp[VDEC_MAX_SUPPORT_STREAM_NUM]; // temp use for utopia 1.0, need to remove when really use utopia 2.0

MS_BOOL  bSecureModeEnable = FALSE;
MS_BOOL  bSecureModeEnableUser = FALSE;

#ifdef VDEC3
MS_S32 s32VdecExEventId[VDEC_MAX_SUPPORT_STREAM_NUM] = {-1,-1,-1,-1};
#else
MS_S32 s32VdecExEventId[VDEC_MAX_SUPPORT_STREAM_NUM] = {-1,-1};
#endif

static char* cbTaskName[4]={"VDEC_CALLBACK_TASK_0","VDEC_CALLBACK_TASK_1","VDEC_CALLBACK_TASK_2","VDEC_CALLBACK_TASK_3"};
//------------------------------------------------------------------------------
// Local Functions Prototype
//------------------------------------------------------------------------------


#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL) || defined(MSOS_TYPE_ECOS)

#define  _VDEC_EX_API_MutexLock()\
{\
    MsOS_ObtainMutex(s32VDECMutexID, MSOS_WAIT_FOREVER);\
}

#define _VDEC_EX_API_MutexUnlock()\
{\
    MsOS_ReleaseMutex(s32VDECMutexID);\
}

static MS_BOOL _VDEC_EX_API_MutexCreate(void)
{

#if defined(MSOS_TYPE_ECOS) // avoid creating the mutex name again for ecos project

    if (s32VDECMutexID > 0) // created already
    {
        return TRUE;
    }

#endif

    s32VDECMutexID = MsOS_CreateMutex(E_MSOS_FIFO,"VDEC_API_Mutex",MSOS_PROCESS_SHARED);

    if (s32VDECMutexID < 0)
    {
        return FALSE;
    }
    return TRUE;
}

#else
#define  _VDEC_EX_API_MutexLock()
#define _VDEC_EX_API_MutexUnlock()
#endif


MS_BOOL _VDEC_EX_Create_BBU_Task(MS_U32 u32Idx);
void _VDEC_EX_Delete_BBU_Task(MS_U32 u32Idx);

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

static VDEC_EX_Result _VDEC_EX_CheckStatus(MS_U8 u8Idx, MS_U32 u32DrvId, MS_U32 u32CallerId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;

    if(pVdecExContext == NULL)
    {
        return E_VDEC_EX_RET_NOT_INIT;
    }

    if (u8Idx >= VDEC_MAX_SUPPORT_STREAM_NUM)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    if(pVdecExContext->_Attr[u8Idx].bUsed == FALSE)
    {
        VDEC_INFO(u8Idx, VDEC_PRINT("%s err: %d is not init success\n",__FUNCTION__,u8Idx));
        return E_VDEC_EX_RET_NOT_INIT;
    }

    if (pVdecExContext->_Attr[u8Idx].u32DrvId != u32DrvId)
    {
        VDEC_INFO(u8Idx, VDEC_PRINT("%s %d err: invalid streamID u8Idx(0x%x), u32DrvId(0x%x)!=0x%x\n", __FUNCTION__, u32CallerId, u8Idx, u32DrvId, pVdecExContext->_Attr[u8Idx].u32DrvId));
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    return eRet;
}


VDEC_EX_Result MApi_VDEC_EX_DbgCmd(VDEC_StreamId *pStreamId, MS_U32 u8Cmd, MS_U32 u32CmdArg)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MVD_CmdArg stCmdArg;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    eRet = _VDEC_EX_CheckStatus(u8Idx, u32Id, __LINE__);
    if (E_VDEC_EX_OK != eRet)
    {
        return eRet;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _VDEC_Memset(&stCmdArg, 0x00, sizeof(MVD_CmdArg));

            if (u32CmdArg)
            {
                stCmdArg.Arg0 = (MS_U8) (u32CmdArg & 0x000000ff);
                stCmdArg.Arg1 = (MS_U8) ((u32CmdArg & 0x0000ff00) >> 8);
                stCmdArg.Arg2 = (MS_U8) ((u32CmdArg & 0x00ff0000) >> 16);
                stCmdArg.Arg3 = (MS_U8) ((u32CmdArg & 0xff000000) >> 24);
            }
            if (MDrv_MVD_MVDCommand(u8Cmd, &stCmdArg))
            {
                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_RET_TIMEOUT;
            }
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            if (E_HVD_EX_OK == MDrv_HVD_EX_SetCmd_Dbg(u32Id, (MS_U32) u8Cmd, u32CmdArg))
            {
                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_RET_TIMEOUT;
            }
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return eRet;
}

VDEC_EX_Result MApi_VDEC_EX_DbgSetData(VDEC_StreamId *pStreamId, MS_VIRT u32Addr, MS_U32 u32Data)
{
    VDEC_EX_Result eRet  = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    eRet = _VDEC_EX_CheckStatus(u8Idx, u32Id, __LINE__);
    if (E_VDEC_EX_OK != eRet)
    {
        return eRet;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            break;

        case E_VDEC_EX_DECODER_HVD:
            MDrv_HVD_EX_SetMem_Dbg(pStreamId->u32Id, u32Addr, u32Data);
            break;

        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            if (E_MJPEG_RET_SUCCESS != MApi_MJPEG_DbgSetData(pStreamId->u32Id, u32Addr, u32Data))
            {
                eRet = E_VDEC_EX_FAIL;
            }
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return eRet;
}

VDEC_EX_Result MApi_VDEC_EX_DbgGetData(VDEC_StreamId *pStreamId, MS_VIRT u32Addr, MS_U32 *u32Data)
{
    VDEC_EX_Result eRet  = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    eRet = _VDEC_EX_CheckStatus(u8Idx, u32Id, __LINE__);
    if (E_VDEC_EX_OK != eRet)
    {
        return eRet;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            E_MVD_Result ret = MDrv_MVD_DbgGetData(pStreamId->u32Id, u32Addr, u32Data);

            if (E_MVD_RET_OK != ret)
            {
                eRet = E_VDEC_EX_FAIL;
            }

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            *u32Data = MDrv_HVD_EX_GetMem_Dbg(pStreamId->u32Id, u32Addr);
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            MJPEG_Result ret = MApi_MJPEG_DbgGetData(pStreamId->u32Id, u32Addr, u32Data);

            if (E_MJPEG_RET_SUCCESS != ret)
            {
                eRet = E_VDEC_EX_FAIL;
            }
            _VDEC_EX_API_MutexUnlock();

            break;
        }
        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;

            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// @brief print vdec debug status
/// @param eStream \b IN : stream type
//------------------------------------------------------------------------------
void MApi_VDEC_EX_DbgDumpStatus(VDEC_StreamId *pStreamId)
{
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if (u8Idx >= VDEC_MAX_SUPPORT_STREAM_NUM)
    {
        return;
    }

    if(pVdecExContext == NULL)
    {
        return;
    }

    if (!(pVdecExContext->_Attr[u8Idx].bInit))
    {
        return;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MDrv_MVD_DbgDump(u32Id);
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            MDrv_HVD_EX_DbgDumpStatus(u32Id, E_HVD_EX_DUMP_STATUS_FW);
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            break;
        }
        default:
            break;
    }
}

extern void HAL_VPU_EX_InitRegBase(MS_VIRT u32RegBase) __attribute__((weak));
extern MS_U32 HAL_VPU_GetProgCnt(void) __attribute__((weak));


//------------------------------------------------------------------------------
/// @brief Get VPU program counter for debug
/// @retval program counter
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_DbgGetProgCnt(void)
{
#ifndef MSOS_TYPE_LINUX_KERNEL
    MS_PHY u32NonPMBankSize=0;
    MS_VIRT u32RiuBaseAdd=0;
    if( !MDrv_MMIO_GetBASE( &u32RiuBaseAdd, &u32NonPMBankSize, MS_MODULE_HW))
    {
        VDEC_PRINT("VDEC HVD MApi_VDEC_DbgGetProgramCnt Err: MMIO_GetBASE failure\n");
        return 0;
    }
    else
    {
        //VDEC_PRINT("HVD:1 u32RiuBaseAdd = %lx\n", u32RiuBaseAdd);
        if( HAL_VPU_EX_InitRegBase )
        {
            HAL_VPU_EX_InitRegBase(u32RiuBaseAdd);
        }
        else
        {
            return 0;
        }
    }
    if( HAL_VPU_GetProgCnt )
    {
        return HAL_VPU_GetProgCnt();
    }
    else
    {
        return 0;
    }
#else
        return 0;
#endif
}


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
#if VDEC_ENABLE_LINK_WEAK_SYMBOL
static void __attribute__((unused)) _VDEC_LinkWeakSymbolPatch(void)
{
#if VDEC_ENABLE_MVD
    MDrv_MVD_LinkWeakSymbolPatch();
#endif
#if VDEC_ENABLE_HVD
    MDrv_HVD_LinkWeakSymbolPatch();
#endif
}
#endif

#define MSTAR_INTERNAL_VDEC_UUID {0x61328cc8, 0x8df1, 0x4821,{0x89, 0x01, 0x76, 0x1b, 0xb2, 0xb9, 0x11, 0x5c} }
#define SYS_TEEC_OPERATION_INITIALIZER { 0 }
TEEC_Context mstar_vdec_teec_ctx;
TEEC_Session vdec_session = { 0 };
TEEC_UUID vdec_uuid = MSTAR_INTERNAL_VDEC_UUID;
TEEC_Operation vdec_op = SYS_TEEC_OPERATION_INITIALIZER;
static const char optee_vdec[] = "opteearmtz00";
char *_vdec_device = (char *)optee_vdec;
typedef enum
{
    E_VDEC_EX_OPTEE_GETSHMADDR = 0,
    E_VDEC_EX_OPTEE_LOADCODE,
    E_VDEC_EX_OPTEE_SECURE_BUFFER,
    E_VDEC_EX_OPTEE_SECURE_FB2,
    E_VDEC_EX_OPTEE_MAX,//max
} VDEC_EX_OPTEE_CMD;

extern MS_U32 MDrv_SYS_TEEC_InitializeContext(const char *name, TEEC_Context *context);
extern MS_U32 MDrv_SYS_TEEC_Open(TEEC_Context *context, TEEC_Session *session, const TEEC_UUID *destination, MS_U32 connection_method, const void *connection_data, TEEC_Operation *operation, MS_U32 *error_origin);
extern MS_U32 MDrv_SYS_TEEC_InvokeCmd(TEEC_Session *session, MS_U32 cmd_id, TEEC_Operation *operation, MS_U32 *error_origin);
extern void MDrv_SYS_TEEC_Close(TEEC_Session *session);
extern void MDrv_SYS_TEEC_FinalizeContext(TEEC_Context *context);
MS_BOOL _MApi_VDEC_EX_V2_Send_OPTEE_CMD(VDEC_EX_OPTEE_CMD eCMD,void* para,MS_U32 size)
{
    MS_BOOL bret = FALSE;
#if defined (MSOS_TYPE_LINUX)
    unsigned int ret_orig = 0;
    if (MDrv_SYS_TEEC_InitializeContext(_vdec_device, &mstar_vdec_teec_ctx) != TEEC_SUCCESS)
    {
        VDEC_PRINT("%s %d: Init Context failed\n",__func__,__LINE__);
        MDrv_SYS_TEEC_FinalizeContext(&mstar_vdec_teec_ctx);
        return bret;
    }

    if (MDrv_SYS_TEEC_Open(&mstar_vdec_teec_ctx, &vdec_session, &vdec_uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &ret_orig) != TEEC_SUCCESS)
    {
        VDEC_PRINT("%s %d: TEEC Open session failed\n",__func__,__LINE__);
        MDrv_SYS_TEEC_Close(&vdec_session);
        MDrv_SYS_TEEC_FinalizeContext(&mstar_vdec_teec_ctx);
        return bret;
    }

    if(size == 0)
    {
        vdec_op.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE,TEEC_NONE, TEEC_NONE);
    }
    else
    {
        vdec_op.params[0].tmpref.buffer = para;
        vdec_op.params[0].tmpref.size = size;
        vdec_op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE,TEEC_NONE, TEEC_NONE);
    }

    if (MDrv_SYS_TEEC_InvokeCmd(&vdec_session, (MS_U32)eCMD, &vdec_op, &ret_orig) != TEEC_SUCCESS)
    {
        VDEC_PRINT("%s %d: TEEC Invoke command failed\n",__func__,__LINE__);
    }
    else
    {
        bret = TRUE;
    }
    MDrv_SYS_TEEC_Close(&vdec_session);
    MDrv_SYS_TEEC_FinalizeContext(&mstar_vdec_teec_ctx);
#endif
    return bret;
}

//------------------------------------------------------------------------------
/// set share memory base address between driver with fw
/// @param u32base \b IN : base addresss
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_Set_ShareMemory_Base(VDEC_StreamId *pStreamId, MS_VIRT u32base)
{
    VDEC_EX_Result ret = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);

    if (pVdecExContext->_Attr[u8Idx].bInit)
    {
        return E_VDEC_EX_RET_ILLEGAL_ACCESS;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetShareMemoryBase(u32Id, u32base));
            ret = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_HVD:
            ret = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            ret = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            ret = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return ret ;

}

static void _VDEC_EX_Enable_VPU_SecurityMode(MS_BOOL enable)
{
    VPRINTF("[VDEC][TEE/OPTEE]%s,enable=%d\n",__FUNCTION__,enable);
    bSecureModeEnable = enable;
}

//------------------------------------------------------------------------------
/// set share memory base address between driver with fw
/// @param u32base \b IN : base addresss
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_Get_ShareMemory_Base(VDEC_StreamId *pStreamId, MS_VIRT *u32base)
{
    VDEC_EX_Result ret = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_GetShareMemoryOffset(u32Id, u32base));
            ret = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_HVD:
            ret = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            ret = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            ret = E_VDEC_EX_RET_UNSUPPORTED;
        break;
    }
    return ret ;

}

static MS_U8 _VDEC_Map2MVDCCFmt(VDEC_EX_CCFormat eFmt)
{
    MS_U8 u8Fmt = 0xff;
    u8Fmt = (E_VDEC_EX_CC_608==eFmt)?E_CC_MVD_TYPE_608:E_CC_MVD_TYPE_708;
    return u8Fmt;
}

static MS_U8 _VDEC_Map2MVDCCOpt(VDEC_EX_CCType eType)
{
    MS_U8 u8Operation = 0;
    switch (eType)
    {
        case E_VDEC_EX_CC_TYPE_NTSC_FIELD1:
            u8Operation = 0x01;
            break;
        case E_VDEC_EX_CC_TYPE_NTSC_FIELD2:
            u8Operation = 0x02;
            break;
        case E_VDEC_EX_CC_TYPE_NTSC_TWOFIELD:
            u8Operation = 0x03;
            break;
        case E_VDEC_EX_CC_TYPE_DTVCC:
        default:
            u8Operation = 0x04;
            break;
    }
    return u8Operation;
}



static MS_U8 _VDEC_Map2HVDCCFmt(VDEC_EX_CCFormat eFmt)
{
    MS_U8 u8Fmt = 0xff;
    u8Fmt = (E_VDEC_EX_CC_608==eFmt)?1:0;
    return u8Fmt;
}

static MS_U8 _VDEC_Map2HVDCCOpt(VDEC_EX_CCType eType)
{
    MS_U8 u8Operation = 0;
    switch (eType)
    {
        case E_VDEC_EX_CC_TYPE_NTSC_FIELD1:
            u8Operation = 0x01;
            break;
        case E_VDEC_EX_CC_TYPE_NTSC_FIELD2:
            u8Operation = 0x02;
            break;
        case E_VDEC_EX_CC_TYPE_NTSC_TWOFIELD:
            u8Operation = 0x03;
            break;
        case E_VDEC_EX_CC_TYPE_DTVCC:
        default:
            u8Operation = 0x04;
            break;
    }
    return u8Operation;
}

static VDEC_EX_Stage _VDEC_Map2HVDStatus(HVD_EX_GetPlayState eStatus)
{
    VDEC_EX_Stage eRet = E_VDEC_EX_STAGE_STOP;
    switch (eStatus)
    {
        case E_HVD_EX_GSTATE_INIT:
            eRet = E_VDEC_EX_STAGE_INIT;
            break;
        case E_HVD_EX_GSTATE_PLAY:
            eRet = E_VDEC_EX_STAGE_PLAY;
            break;
        case E_HVD_EX_GSTATE_PAUSE:
            eRet = E_VDEC_EX_STAGE_PAUSE;
            break;
        case E_HVD_EX_GSTATE_STOP:
        default:
            eRet = E_VDEC_EX_STAGE_STOP;
            break;
    }
    return eRet;
}

static MS_U32 _VDEC_Map2HVDErrCode(MS_U32 u32ErrCode)
{

    MS_U32 u32Ret=E_VDEC_EX_HVD_ERR_CODE_GENERAL_BASE;


    switch (u32ErrCode)
    {
        case E_HVD_EX_ERRCODE_OUT_OF_SPEC:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_OUT_OF_SPEC;
            break;
        case E_HVD_EX_ERRCODE_UNKNOW_ERR:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_UNKNOW_ERR;
            break;
        case E_HVD_EX_ERRCODE_HW_BREAK_DOWN:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_HW_BREAK_DOWN;
            break;
        case E_HVD_EX_ERRCODE_HW_DEC_TIMEOUT:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_HW_DEC_TIMEOUT;
            break;
        case E_HVD_EX_ERRCODE_OUT_OF_MEMORY:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_OUT_OF_MEMORY;
            break;
        case E_HVD_EX_ERRCODE_UNKNOWN_CODEC:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_UNKNOWN_CODEC;
            break;
        case E_HVD_EX_ERRCODE_RES_NOT_SUPPORT:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_RES_NOT_SUPPORT;
            break;
        case E_HVD_EX_ERRCODE_AVC_SPS_BROKEN:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_AVC_SPS_BROKEN;
            break;
        case E_HVD_EX_ERRCODE_AVC_SPS_NOT_IN_SPEC:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_AVC_SPS_NOT_IN_SPEC;
            break;
        case E_HVD_EX_ERRCODE_AVC_SPS_NOT_ENOUGH_FRM:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_AVC_SPS_NOT_ENOUGH_FRM;
            break;
        case E_HVD_EX_ERRCODE_AVC_PPS_BROKEN:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_AVC_PPS_BROKEN;
            break;
        case E_HVD_EX_ERRCODE_AVC_REF_LIST:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_AVC_REF_LIST;
            break;
        case E_HVD_EX_ERRCODE_AVC_NO_REF:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_AVC_NO_REF;
            break;
        case E_HVD_EX_ERRCODE_AVC_RES:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_AVC_RES;
            break;
        case E_HVD_EX_ERRCODE_AVS_RES:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_AVS_RES;
            break;
        case E_HVD_EX_ERRCODE_RM_PACKET_HEADER:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_RM_PACKET_HEADER;
            break;
        case E_HVD_EX_ERRCODE_RM_FRAME_HEADER:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_RM_FRAME_HEADER;
            break;
        case E_HVD_EX_ERRCODE_RM_SLICE_HEADER:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_RM_SLICE_HEADER;
            break;
        case E_HVD_EX_ERRCODE_RM_BYTE_CNT:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_RM_BYTE_CNT;
            break;
        case E_HVD_EX_ERRCODE_RM_DISP_TIMEOUT:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_RM_DISP_TIMEOUT;
            break;
        case E_HVD_EX_ERRCODE_RM_NO_REF:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_RM_NO_REF;
            break;
        case E_HVD_EX_ERRCODE_RM_RES:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_RM_RES;
            break;
        case E_HVD_EX_ERRCODE_RM_VLC:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_RM_VLC;
            break;
        case E_HVD_EX_ERRCODE_RM_SIZE_OUT_FB_LAYOUT:
            u32Ret = E_VDEC_EX_HVD_ERR_CODE_RM_SIZE_OUT_FB_LAYOUT;
            break;
        default:
            break;
    }
    return u32Ret;
}



static MS_U32 _VDEC_Map2ESBufStatus(MS_U32 u32ESBufStatus)
{
    MS_U32 u32Ret = ES_BUFFER_STATUS_UNKNOWN;

    switch (u32ESBufStatus)
    {
        case E_HVD_EX_ES_BUF_STATUS_UNDERFLOW:
            u32Ret = ES_BUFFER_STATUS_UNDERFLOW;
            break;
        case E_HVD_EX_ES_BUF_STATUS_OVERFLOW:
            u32Ret = ES_BUFFER_STATUS_OVERFLOW;
            break;
        case E_HVD_EX_ES_BUF_STATUS_NORMAL:
            u32Ret = ES_BUFFER_STATUS_NORMAL;
            break;
        default:
            break;
    }
    return u32Ret;

}

static HVD_EX_MFCodec_mode _VDEC_Map2HVDMFCodecMode(VDEC_EX_MFCodec_mode eMFCodecMode)
{
    HVD_EX_MFCodec_mode u32Ret = E_HVD_EX_MFCODEC_DEFAULT;

    switch (eMFCodecMode)
    {
        case E_VDEC_EX_MFCODEC_DEFAULT:
            u32Ret = E_HVD_EX_MFCODEC_DEFAULT;
            break;
        case E_VDEC_EX_MFCODEC_FORCE_ENABLE:
            u32Ret = E_HVD_EX_MFCODEC_FORCE_ENABLE;
            break;
        case E_VDEC_EX_MFCODEC_FORCE_DISABLE:
            u32Ret = E_HVD_EX_MFCODEC_FORCE_DISABLE;
            break;
        default:
            break;
    }
    return u32Ret;
}

static HVD_EX_Display_mode _VDEC_Map2HVDDispMode(VDEC_EX_DISPLAY_MODE eDisplayMode)
{
    HVD_EX_Display_mode u32Ret = E_HVD_EX_DISPLAY_MODE_DEFAULT;

    switch (eDisplayMode)
    {
        case E_VDEC_EX_DISPLAY_MODE_MCU:
            u32Ret = E_HVD_EX_DISPLAY_MODE_MCU;
            break;
        case E_VDEC_EX_DISPLAY_MODE_HARDWIRE:
            u32Ret = E_HVD_EX_DISPLAY_MODE_HARDWIRE;
            break;
        default:
            break;
    }
    return u32Ret;
}

static VDEC_EX_V2_User_Cmd _VDEC_MapUserCmd_V2(VDEC_EX_User_Cmd eUserCmd)
{
    VDEC_EX_V2_User_Cmd eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_CONTROL_BASE;
    switch (eUserCmd)
    {
        case E_VDEC_EX_USER_CMD_SET_CONTROL_BASE            : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_CONTROL_BASE;break;
        case E_VDEC_EX_USER_CMD_REPEAT_LAST_FIELD           : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_REPEAT_LAST_FIELD ;break;
        case E_VDEC_EX_USER_CMD_AVSYNC_REPEAT_TH            : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_AVSYNC_REPEAT_TH ;break;
        case E_VDEC_EX_USER_CMD_DISP_ONE_FIELD              : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_DISP_ONE_FIELD ;break;
        case E_VDEC_EX_USER_CMD_FD_MASK_DELAY_COUNT         : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_FD_MASK_DELAY_COUNT ;break;
        case E_VDEC_EX_USER_CMD_FRC_OUTPUT                  : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_FRC_OUTPUT ;break;
        case E_VDEC_EX_USER_CMD_FRC_DROP_TYPE               : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_FRC_DROP_TYPE ;break;
        case E_VDEC_EX_USER_CMD_FAST_DISPLAY                : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_FAST_DISPLAY ;break;
        case E_VDEC_EX_USER_CMD_IGNORE_ERR_REF              : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_IGNORE_ERR_REF ;break;
        case E_VDEC_EX_USER_CMD_FORCE_FOLLOW_DTV_SPEC       : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_FORCE_FOLLOW_DTV_SPEC ;break;
        case E_VDEC_EX_USER_CMD_AVC_MIN_FRM_GAP             : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_AVC_MIN_FRM_GAP ;break;
        case E_VDEC_EX_USER_CMD_DISABLE_SEQ_CHG             : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_DISABLE_SEQ_CHG ;break;
        case E_VDEC_EX_USER_CMD_SET_DISP_OUTSIDE_CTRL_MODE  : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_DISP_OUTSIDE_CTRL_MODE ;break;
        case E_VDEC_EX_USER_CMD_SET_DTV_USER_DATA_MODE      : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_DTV_USER_DATA_MODE ;break;
        case E_VDEC_EX_USER_CMD_SET_SINGLE_TASK_MODE        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_SINGLE_TASK_MODE ;break;
        case E_VDEC_EX_USER_CMD_AVC_DISABLE_ANTI_VDEAD      : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_AVC_DISABLE_ANTI_VDEAD ;break;
        case E_VDEC_EX_USER_CMD_DTV_RESET_MVD_PARSER        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_DTV_RESET_MVD_PARSER ;break;
        case E_VDEC_EX_USER_CMD_PVR_FLUSH_FRAME_BUFFER      : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_PVR_FLUSH_FRAME_BUFFER ;break;
        case E_VDEC_EX_USER_CMD_FORCE_INTERLACE_MODE        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_FORCE_INTERLACE_MODE ;break;
        case E_VDEC_EX_USER_CMD_RELEASE_FD_MASK             : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_RELEASE_FD_MASK ;break;
        case E_VDEC_EX_USER_CMD_SET_DECODE_MODE             : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_NULL ;break;
        case E_VDEC_EX_USER_CMD_SUPPORT_AVC_TO_MVC          : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SUPPORT_AVC_TO_MVC ;break;
        case E_VDEC_EX_USER_CMD_3DLR_VIEW_EXCHANGE          : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_3DLR_VIEW_EXCHANGE ;break;
        case E_VDEC_EX_USER_CMD_SET_VSIZE_ALIGN             : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_VSIZE_ALIGN ;break;
        case E_VDEC_EX_USER_CMD_SHOW_DECODE_ORDER           : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SHOW_DECODE_ORDER ;break;
        case E_VDEC_EX_USER_CMD_AVC_DISP_IGNORE_CROP        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_AVC_DISP_IGNORE_CROP ;break;
        case E_VDEC_EX_USER_CMD_SET_DISP_FINISH_MODE        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_DISP_FINISH_MODE ;break;
        case E_VDEC_EX_USER_CMD_SET_AVSYNC_MODE             : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_AVSYNC_MODE ;break;
        case E_VDEC_EX_USER_CMD_SUSPEND_DYNAMIC_SCALE       : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SUSPEND_DYNAMIC_SCALE ;break;
        case E_VDEC_EX_USER_CMD_FORCE_AUTO_MUTE             : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_FORCE_AUTO_MUTE ;break;
        case E_VDEC_EX_USER_CMD_AVC_NEW_SLOW_MOTION         : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_AVC_NEW_SLOW_MOTION ;break;
        case E_VDEC_EX_USER_CMD_PUSH_DISPQ_WITH_REF_NUM     : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_PUSH_DISPQ_WITH_REF_NUM ;break;
        case E_VDEC_EX_USER_CMD_DS_RESV_N_BUFFER            : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_DS_RESV_N_BUFFER ;break;
        case E_VDEC_EX_USER_CMD_RM_ENABLE_PTS_TBL           : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_RM_ENABLE_PTS_TBL ;break;
        case E_VDEC_EX_USER_CMD_FLUSH_PTS_BUF               : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_FLUSH_PTS_BUF ;break;
        case E_VDEC_EX_USER_CMD_SET_IDCT_MODE               : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_IDCT_MODE ;break;
        case E_VDEC_EX_USER_CMD_DROP_ERR_FRAME              : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_DROP_ERR_FRAME ;break;
        case E_VDEC_EX_USER_CMD_SET_CC608_INFO_ENHANCE_MODE : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_CC608_INFO_ENHANCE_MODE ;break;
        case E_VDEC_EX_USER_CMD_IGNORE_PIC_OVERRUN          : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_IGNORE_PIC_OVERRUN ;break;
        case E_VDEC_EX_USER_CMD_SET_SELF_SEQCHANGE          : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_SELF_SEQCHANGE ;break;
        case E_VDEC_EX_USER_CMD_AUTO_EXHAUST_ES_MODE        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_AUTO_EXHAUST_ES_MODE ;break;
        case E_VDEC_EX_USER_CMD_CTL_SPEED_IN_DISP_ONLY      : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_CTL_SPEED_IN_DISP_ONLY ;break;
        case E_VDEC_EX_USER_CMD_AVC_SUPPORT_REF_NUM_OVER_MAX_DPB_SIZE   : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_AVC_SUPPORT_REF_NUM_OVER_MAX_DPB_SIZE ;break;
        case E_VDEC_EX_USER_CMD_RETURN_INVALID_AFD          : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_RETURN_INVALID_AFD ;break;
        case E_VDEC_EX_USER_CMD_FIELD_POLARITY_DISPLAY_ONE_FIELD        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_FIELD_POLARITY_DISPLAY_ONE_FIELD ;break;
        case E_VDEC_EX_USER_CMD_AVC_FORCE_BROKEN_BY_US      : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_AVC_FORCE_BROKEN_BY_US ;break;
        case E_VDEC_EX_USER_CMD_SHOW_FIRST_FRAME_DIRECT     : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SHOW_FIRST_FRAME_DIRECT ;break;
        case E_VDEC_EX_USER_CMD_AVC_RESIZE_DOS_DISP_PEND_BUF            : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_AVC_RESIZE_DOS_DISP_PEND_BUF ;break;
        case E_VDEC_EX_USER_CMD_SET_XC_LOW_DELAY_PARA       : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_XC_LOW_DELAY_PARA ;break;
        case E_VDEC_EX_USER_CMD_SET_SECURE_MODE             : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_SECURE_MODE ;break;
        case E_VDEC_EX_USER_CMD_RVU_SETTING_MODE            : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_RVU_SETTING_MODE ;break;
        case E_VDEC_EX_USER_CMD_FRAMERATE_HANDLING          : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_FRAMERATE_HANDLING ;break;
        case E_VDEC_EX_USER_CMD_DUAL_NON_BLOCK_MODE         : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_DUAL_NON_BLOCK_MODE ;break;
        case E_VDEC_EX_USER_CMD_IGNORE_PIC_STRUCT_DISPLAY   : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_IGNORE_PIC_STRUCT_DISPLAY ;break;
        case E_VDEC_EX_USER_CMD_INPUT_PTS_FREERUN_MODE      : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_INPUT_PTS_FREERUN_MODE ;break;
        case E_VDEC_EX_USER_CMD_ERR_CONCEAL_SLICE_1ST_MB    : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_ERR_CONCEAL_SLICE_1ST_MB ;break;
        case E_VDEC_EX_USER_CMD_SET_EXTERNAL_DS_BUFFER      : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_EXTERNAL_DS_BUFFER ;break;
        case E_VDEC_EX_USER_CMD_SET_MIN_TSP_DATA_SIZE       : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_MIN_TSP_DATA_SIZE ;break;
        case E_VDEC_EX_USER_CMD_SET_DMX_FRAMERATE           : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_DMX_FRAMERATE ;break;
        case E_VDEC_EX_USER_CMD_SET_DMX_FRAMERATEBASE       : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_DMX_FRAMERATEBASE ;break;
        case E_VDEC_EX_USER_CMD_ENABLE_CC_608_EXTERNAL_BUFFER           : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_ENABLE_CC_608_EXTERNAL_BUFFER ;break;
        case E_VDEC_EX_USER_CMD_ENABLE_CC_708_EXTERNAL_BUFFER           : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_ENABLE_CC_708_EXTERNAL_BUFFER ;break;
        case E_VDEC_EX_USER_CMD_SET_TIME_INC_PREDICT_PARA               : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_TIME_INC_PREDICT_PARA ;break;
        case E_VDEC_EX_USER_CMD_ENABLE_DECODE_ENGINE_TIMEOUT            : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_ENABLE_DECODE_ENGINE_TIMEOUT ;break;
        case E_VDEC_EX_USER_CMD_AUTO_FREE_ES                : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_AUTO_FREE_ES ;break;
        case E_VDEC_EX_USER_CMD_FRAMEBUFFER_AUTO_MODE       : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_FRAMEBUFFER_AUTO_MODE ;break;
        case E_VDEC_EX_USER_CMD_SET_SMOOTH_REWIND           : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_SMOOTH_REWIND ;break;
        case E_VDEC_EX_USER_CMD_SET_ERROR_TOLERANCE         : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_ERROR_TOLERANCE ;break;
        case E_VDEC_EX_USER_CMD_AUTO_DROP_DISPLAY_QUEUE     : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_AUTO_DROP_DISPLAY_QUEUE ;break;
        case E_VDEC_EX_USER_CMD_USE_CPB_REMOVAL_DEALY       : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_USE_CPB_REMOVAL_DEALY ;break;
        case E_VDEC_EX_USER_CMD_SKIP_N_FRAME                : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SKIP_N_FRAME ;break;
        case E_VDEC_EX_USER_CMD_SET_PTS_US_MODE             : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_PTS_US_MODE ;break;
        case E_VDEC_EX_USER_CMD_AUTO_INSERT_DUMMY_DATA      : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_AUTO_INSERT_DUMMY_DATA ;break;
        case E_VDEC_EX_USER_CMD_DROP_ONE_PTS                : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_DROP_ONE_PTS ;break;
        case E_VDEC_EX_USER_CMD_PVR_TIMESHIFT_SEAMLESS_MODE : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_PVR_TIMESHIFT_SEAMLESS_MODE ;break;
        case E_VDEC_EX_USER_CMD_AUTO_REDUCE_ES_DATA         : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_AUTO_REDUCE_ES_DATA ;break;
        case E_VDEC_EX_USER_CMD_RM_FORCE_MCU_MODE_ES        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_RM_FORCE_MCU_MODE_ES ;break;
        case E_VDEC_EX_USER_CMD_FORCE_PROGRESSIVE_MODE      : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_FORCE_PROGRESSIVE_MODE ;break;
        case E_VDEC_EX_USER_CMD_SET_FRAMEBUFF2              : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_FRAMEBUFF2 ;break;
        case E_VDEC_EX_USER_CMD_ENABLE_PTS_DECTECTOR        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_ENABLE_PTS_DECTECTOR ;break;
        case E_VDEC_EX_USER_CMD_DISABLE_PBFRAME_MODE        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_DISABLE_PBFRAME_MODE ;break;
        case E_VDEC_EX_USER_CMD_SET_SLOW_SYNC               : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_SLOW_SYNC;break;
        case E_VDEC_EX_USER_CMD_SET_TRICKPLAY_2X_MODE       : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_TRICKPLAY_2X_MODE ;break;
        case E_VDEC_EX_USER_CMD_FRC_ONLY_SHOW_TOP_FIELD     : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_FRC_ONLY_SHOW_TOP_FIELD;break;
        case E_VDEC_EX_USER_CMD_DIRECT_STC_MODE             : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_DIRECT_STC_MODE;break;
        case E_VDEC_EX_USER_CMD_DISABLE_ES_FULL_STOP        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_DISABLE_ES_FULL_STOP ;break;

        case E_VDEC_EX_USER_CMD_MVC_SET_CMD_BASE            : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_MVC_SET_CMD_BASE ;break;
        case E_VDEC_EX_USER_CMD_MVC_BBU2_PUSH_PACKET        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_MVC_BBU2_PUSH_PACKET ;break;
        case E_VDEC_EX_USER_CMD_MVC_BBU2_FIRE_DECCMD        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_MVC_BBU2_FIRE_DECCMD ;break;

        case E_VDEC_EX_USER_CMD_UT_SET_CMD_BASE             : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_UT_SET_CMD_BASE ;break;
        case E_VDEC_EX_USER_CMD_UT_SET_DBG_MODE             : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_UT_SET_DBG_MODE ;break;
        case E_VDEC_EX_USER_CMD_UT_CLR_DBG_MODE             : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_UT_CLR_DBG_MODE ;break;
        case E_VDEC_EX_USER_CMD_SET_DV_XC_SHM_ADDR          : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_DV_XC_SHM_ADDR ;break;
#ifdef VDEC_CAP_DV_OTT_API
        case E_VDEC_EX_USER_CMD_SET_DV_INFO                 : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_DV_INFO ;break;
#endif

    //Group2:Get Control command================================

        case E_VDEC_EX_USER_CMD_GET_CONTROL_BASE            : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_CONTROL_BASE ;break;
        case E_VDEC_EX_USER_CMD_GET_CHROMA_TYPE             : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_CHROMA_TYPE ;break;
        case E_VDEC_EX_USER_CMD_GET_REAL_FRAMERATE          : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_REAL_FRAMERATE ;break;
        case E_VDEC_EX_USER_CMD_GET_COLOR_MATRIX            : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_COLOR_MATRIX ;break;
        case E_VDEC_EX_USER_CMD_GET_MAIN_STREAM_ID          : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_MAIN_STREAM_ID ;break;
        case E_VDEC_EX_USER_CMD_GET_SUB_STREAM_ID           : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_SUB_STREAM_ID ;break;
        case E_VDEC_EX_USER_CMD_GET_DYNSCALE_ENABLED        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_DYNSCALE_ENABLED ;break;
        case E_VDEC_EX_USER_CMD_GET_FPA_SEI                 : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_FPA_SEI ;break;
        case E_VDEC_EX_USER_CMD_GET_DCV_SEI                 : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_DCV_SEI ;break;
        case E_VDEC_EX_USER_CMD_GET_VUI_DISP_INFO           : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_VUI_DISP_INFO ;break;
        case E_VDEC_EX_USER_CMD_GET_CLLI_SEI                : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_CLLI_SEI ;break;
        case E_VDEC_EX_USER_CMD_GET_CODEC_CAP               : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_CODEC_CAP ;break;
        case E_VDEC_EX_USER_CMD_GET_U64PTS                  : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_U64PTS ;break;
        case E_VDEC_EX_USER_CMD_GET_PRE_PAS_U64PTS          : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_PRE_PAS_U64PTS ;break;
        case E_VDEC_EX_USER_CMD_GET_PVRSEAMLESS_INFO        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_PVRSEAMLESS_INFO ;break;
        case E_VDEC_EX_USER_CMD_GET_SEQ_CHANGE_INFO         : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_SEQ_CHANGE_INFO;break;
        case E_VDEC_EX_USER_CMD_GET_ORI_INTERLACE_MODE      : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_ORI_INTERLACE_MODE ;break;
        case E_VDEC_EX_USER_CMD_GET_MBS_ONLY_FLAG           : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_MBS_ONLY_FLAG ;break;
        case E_VDEC_EX_USER_CMD_GET_CRC_VALUE               : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_CRC_VALUE ;break;
        case E_VDEC_EX_USER_CMD_GET_BBU_Q_NUM               : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_BBU_Q_NUM ;break;
        case E_VDEC_EX_USER_CMD_GET_DISP_FRAME_NUM          : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_DISP_FRAME_NUM ;break;
        case E_VDEC_EX_USER_CMD_GET_FPA_SEI_EX              : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_FPA_SEI_EX ;break;
        case E_VDEC_EX_USER_CMD_GET_ES_BUFFER_STATUS        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_ES_BUFFER_STATUS ;break;
        case E_VDEC_EX_USER_CMD_GET_CODEC_TYPE              : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_CODEC_TYPE ;break;
        case E_VDEC_EX_USER_CMD_GET_SHAREMEMORY_BASE        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_SHAREMEMORY_BASE ;break;
        case E_VDEC_EX_USER_CMD_GET_IS_LEAST_DISPQ_SIZE_FLAG: eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_IS_LEAST_DISPQ_SIZE_FLAG ;break;
        case E_VDEC_EX_USER_CMD_GET_FIELD_PIC_FLAG          : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_FIELD_PIC_FLAG ;break;
        case E_VDEC_EX_USER_CMD_GET_SUPPORT_2ND_MVOP_INTERFACE          : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_SUPPORT_2ND_MVOP_INTERFACE ;break;
        case E_VDEC_EX_USER_CMD_GET_FB_USAGE_MEM            : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_FB_USAGE_MEM ;break;
        case E_VDEC_EX_USER_CMD_GET_XC_LOW_DELAY_INT_STATE  : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_XC_LOW_DELAY_INT_STATE ;break;
        case E_VDEC_EX_USER_CMD_GET_VSYNC_BRIDGE_ADDR       : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_VSYNC_BRIDGE_ADDR ;break;
        case E_VDEC_EX_USER_CMD_GET_FRAME_INFO_EX           : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_FRAME_INFO_EX ;break;
        case E_VDEC_EX_USER_CMD_GET_FLUSH_PATTEN_ENTRY_NUM  : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_FLUSH_PATTEN_ENTRY_NUM ;break;
        case E_VDEC_EX_USER_CMD_GET_DS_BUF_MIU_SEL          : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_DS_BUF_MIU_SEL ;break;
        case E_VDEC_EX_USER_CMD_GET_FW_STATUS_FLAG          : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_FW_STATUS_FLAG ;break;
        case E_VDEC_EX_USER_CMD_GET_HW_MAX_PIXEL            : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_HW_MAX_PIXEL ;break;
        case E_VDEC_EX_USER_CMD_GET_FLOW_CONTROL_U64PTS_DIFF    : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_FLOW_CONTROL_U64PTS_DIFF ;break;
        case E_VDEC_EX_USER_CMD_GET_NEXT_DISP_FRAME_INFO_EXT    : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_NEXT_DISP_FRAME_INFO_EXT ;break;
        case E_VDEC_EX_USER_CMD_GET_VSYNC_BRIDGE_EXT_ADDR   : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_VSYNC_BRIDGE_EXT_ADDR ;break;
        case E_VDEC_EX_USER_CMD_GET_DISP_QUEUE_EMPTY        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_DISP_QUEUE_EMPTY ;break;
        case E_VDEC_EX_USER_CMD_GET_NOT_SUPPORT_INFO            : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_NOT_SUPPORT_INFO ;break;
        case E_VDEC_EX_USER_CMD_GET_MIN_TSP_DATA_SIZE       : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_MIN_TSP_DATA_SIZE ;break;

    //MVC Get Control command================================
        case E_VDEC_EX_USER_CMD_MVC_GET_CMD_BASE            : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_MVC_GET_CMD_BASE ;break;
        case E_VDEC_EX_USER_CMD_GET_MVC_SUB_FRAME_DISP_INFO : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_MVC_SUB_FRAME_DISP_INFO ;break;
        case E_VDEC_EX_USER_CMD_GET_MVC_BBU2_DECQ_VACANCY   : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_MVC_BBU2_DECQ_VACANCY ;break;
        case E_VDEC_EX_USER_CMD_GET_MVC_ES2_READ_PTR        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_MVC_ES2_READ_PTR ;break;
        case E_VDEC_EX_USER_CMD_GET_MVC_ES2_WRITE_PTR       : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_MVC_ES2_WRITE_PTR ;break;
        case E_VDEC_EX_USER_CMD_GET_ES_QUANTITY             : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_ES_QUANTITY ;break;
        case E_VDEC_EX_USER_CMD_GET_ES2_QUANTITY            : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_GET_ES2_QUANTITY ;break;

    //Group3:System Preset Control command======================
    //Group3-1:Common system Preset Control command
        case E_VDEC_EX_USER_CMD_SYSTEM_PRESET_CONTROL_BASE  : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SYSTEM_PRESET_CONTROL_BASE ;break;
        case E_VDEC_EX_USER_CMD_SYSTEM_PRESET_VPU_CLOCK     : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SYSTEM_PRESET_VPU_CLOCK ;break;

    //Group3-2:HVD System Preset Control command
        case E_VDEC_EX_USER_CMD_SYSTEM_PRESET_HVD_BASE      : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SYSTEM_PRESET_HVD_BASE ;break;
        case E_VDEC_EX_USER_CMD_SYSTEM_PRESET_HVD_CLOCK     : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SYSTEM_PRESET_HVD_CLOCK ;break;

    //Group3-3:MVD System Preset Control command
        case E_VDEC_EX_USER_CMD_SYSTEM_PRESET_MVD_BASE      : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SYSTEM_PRESET_MVD_BASE ;break;
        case E_VDEC_EX_USER_CMD_SYSTEM_PRESET_MVD_CLOCK     : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SYSTEM_PRESET_MVD_CLOCK ;break;
        case E_VDEC_EX_USER_CMD_VPU_SECURITY_MODE           : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_VPU_SECURITY_MODE ;break;

    //Group3-4:Preset Control command=============================
        case E_VDEC_EX_USER_CMD_PRESET_CONTROL_BASE         : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_PRESET_CONTROL_BASE ;break;
        case E_VDEC_EX_USER_CMD_HVD_ONE_PENDING_BUFFER_MODE : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_HVD_ONE_PENDING_BUFFER_MODE ;break;
        case E_VDEC_EX_USER_CMD_MVD_HWBUFFER_REMAPPING_MODE : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_MVD_HWBUFFER_REMAPPING_MODE ;break;
        case E_VDEC_EX_USER_CMD_SET_SHAREMEMORY_BASE        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_SHAREMEMORY_BASE ;break;
        case E_VDEC_EX_USER_CMD_HVD_COL_BBU_MODE            : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_HVD_COL_BBU_MODE ;break;
        case E_VDEC_EX_USER_CMD_HVD_IAPGN_BUF_SHARE_BW_MODE : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_HVD_IAPGN_BUF_SHARE_BW_MODE ;break;
        case E_VDEC_EX_USER_CMD_DTV_DEBUG_MODE              : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_DTV_DEBUG_MODE ;break;
        case E_VDEC_EX_USER_CMD_HVD_TS_IN_BBU_MODE          : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_HVD_TS_IN_BBU_MODE ;break;
        case E_VDEC_EX_USER_CMD_AUTO_ARRANGE_FRAMEBUFFER_USAGE         : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_AUTO_ARRANGE_FRAMEBUFFER_USAGE ;break;
        case E_VDEC_EX_USER_CMD_THUMBNAIL_MODE              : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_THUMBNAIL_MODE ;break;
        case E_VDEC_EX_USER_CMD_FORCE_8BIT_DEC_MODE         : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_FORCE_8BIT_DEC_MODE ;break;
        case E_VDEC_EX_USER_CMD_MFCODEC_MODE                : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_MFCODEC_MODE ;break;
        case E_VDEC_EX_USER_CMD_VDEC_FEATURE                : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_VDEC_FEATURE ;break;
        case E_VDEC_EX_USER_CMD_DYNAMIC_CMA_MODE            : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_DYNAMIC_CMA_MODE ;break;
        case E_VDEC_EX_USER_CMD_SET_DISPLAY_MODE            : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_DISPLAY_MODE ;break;
        case E_VDEC_EX_USER_CMD_PRESET_STC                  : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_PRESET_STC ;break;
        case E_VDEC_EX_USER_CMD_CONNECT_INPUT_TSP           : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_CONNECT_INPUT_TSP ;break;
        case E_VDEC_EX_USER_CMD_CONNECT_DISPLAY_PATH        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_CONNECT_DISPLAY_PATH ;break;
        case E_VDEC_EX_USER_CMD_BITSTREAMBUFFER_MONOPOLY    : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_BITSTREAMBUFFER_MONOPOLY ;break;
        case E_VDEC_EX_USER_CMD_FRAMEBUFFER_MONOPOLY        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_FRAMEBUFFER_MONOPOLY ;break;
        case E_VDEC_EX_USER_CMD_SPECIFY_TASK_SPEC           : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SPECIFY_TASK_SPEC ;break;
        case E_VDEC_EX_USER_CMD_CAL_FRAMERATE               : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_CAL_FRAMERATE ;break;
        case E_VDEC_EX_USER_CMD_SET_TOTALRANGE_BITSTREAMBUFFER          : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_TOTALRANGE_BITSTREAMBUFFER ;break;
        case E_VDEC_EX_USER_CMD_SET_TOTALRANGE_FRAMEBUFFER1 : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_TOTALRANGE_FRAMEBUFFER1 ;break;
        case E_VDEC_EX_USER_CMD_SET_TOTALRANGE_FRAMEBUFFER2 : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SET_TOTALRANGE_FRAMEBUFFER2 ;break;

    //Group4:System Postset Control command======================
        case E_VDEC_EX_USER_CMD_SYSTEM_POSTSET_CONTROL_BASE : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SYSTEM_POSTSET_CONTROL_BASE ;break;
        case E_VDEC_EX_USER_CMD_SYSTEM_POSTSET_CLEAR_PROCESS_RELATED    : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SYSTEM_POSTSET_CLEAR_PROCESS_RELATED ;break;

    //Group5:PreGet Control command======================
        case E_VDEC_EX_USER_CMD_SYSTEM_PREGET_CONTROL_BASE  : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SYSTEM_PREGET_CONTROL_BASE ;break;
        case E_VDEC_EX_USER_CMD_SYSTEM_PREGET_FB_MEMORY_USAGE_SIZE      : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SYSTEM_PREGET_FB_MEMORY_USAGE_SIZE ;break;

#ifdef VDEC_CAP_SYSTEM_PREGET_API
#ifdef VDEC_CAP_DV_OTT_API
    //Group5:System PreGet Control command======================
    //Group5-1:Common system Preget Control command
        case E_VDEC_EX_USER_CMD_SYSTEM_PREGET_DV_SUPPORT_PROFILE        : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SYSTEM_PREGET_DV_SUPPORT_PROFILE ;break;
        case E_VDEC_EX_USER_CMD_SYSTEM_PREGET_DV_SUPPORT_LEVEL          : eEXUserCmd = E_VDEC_EX_V2_USER_CMD_SYSTEM_PREGET_DV_SUPPORT_LEVEL ;break;
#endif
#endif
        default:
            break;
    }
    return eEXUserCmd;
}

// N Decode Buffer Allocator Functions Start

#define ENABLE_ALLOCATOR_ERROR 1
#define ENABLE_ALLOCATOR_DEBUG 0
#define ENABLE_ALLOCATOR_TRACE 0

#if ENABLE_ALLOCATOR_ERROR
    #define ALLOCATOR_PRINTF_ERR(format,args...) VPRINTF(format, ##args)
#else
    #define ALLOCATOR_PRINTF_ERR(format,args...)
#endif

#if ENABLE_ALLOCATOR_DEBUG
    #define ALLOCATOR_PRINTF_DBG(format,args...) VPRINTF(format, ##args)
#else
    #define ALLOCATOR_PRINTF_DBG(format,args...)
#endif

// Functions of BlockInfo
static void _VDEC_EX_Allocator_BlockInfoReset(VDEC_EX_Allocator_BlockInfo *pstBlockInfo)
{
    pstBlockInfo->u8MIU = 0;
    pstBlockInfo->u8Property = VDEC_EX_ALLOCAOTR_PROPERTY_DEFAULT;
    pstBlockInfo->u16Allocated = 0;
    pstBlockInfo->phyAddr = 0;
    pstBlockInfo->szSize = 0;
    return;
}

static void _VDEC_EX_Allocator_BlockInfoCopy(VDEC_EX_Allocator_BlockInfo *pstBlockInfoDst,
    VDEC_EX_Allocator_BlockInfo *pstBlockInfoSrc)
{
    memcpy(pstBlockInfoDst, pstBlockInfoSrc, sizeof(VDEC_EX_Allocator_BlockInfo));
    return;
}

// Functions of Block
static void _VDEC_EX_Allocator_BlockReset(VDEC_EX_Allocator_Block *pstBlock, MS_U16 u16Id)
{
    pstBlock->u16BlockId = u16Id;
    pstBlock->u16BlockIdPrev = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    pstBlock->u16BlockIdNext = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    pstBlock->u16AllocatorIdExt = VDEC_EX_ALLOCAOTR_ID_UNUSED;
    _VDEC_EX_Allocator_BlockInfoReset(&(pstBlock->stBlockInfo));
    return;
}

static inline MS_BOOL _VDEC_EX_Allocator_BlockIsValidId(MS_U16 u16BlockID)
{
    return (u16BlockID < VDEC_EX_ALLOCAOTR_MAX_NUM_BLOCK) ? TRUE : FALSE;
}

static VDEC_EX_Allocator_Block *_VDEC_EX_Allocator_BlockGetAddressById(MS_U16 u16BlockID)
{
    VDEC_EX_Allocator_Block *pstBlockPool =
        &(pVdecExContext->stNDecAllocator.stBlockPool[0]);
    VDEC_EX_Allocator_Block *pstBlock = NULL;

    if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockID) == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid block ID: %d\n", __FUNCTION__, __LINE__, u16BlockID);
        goto exit;
    }

    pstBlock = &(pstBlockPool[u16BlockID]);

exit:
    return pstBlock;
}

static MS_U16 _VDEC_EX_Allocator_BlockGetNewId(void)
{
    VDEC_EX_Allocator_Block *pstBlockPool =
        &(pVdecExContext->stNDecAllocator.stBlockPool[0]);
    MS_U16 u16BlockIdNew = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    MS_U16 u16Id;

    for(u16Id = 0; u16Id < VDEC_EX_ALLOCAOTR_MAX_NUM_BLOCK; u16Id++)
    {
        VDEC_EX_Allocator_Block *pstBlock = &(pstBlockPool[u16Id]);
        if(pstBlock->u16BlockId == VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED)
        {
            u16BlockIdNew = u16Id;
            _VDEC_EX_Allocator_BlockReset(pstBlock, u16Id);
            break;
        }
    }

    return u16BlockIdNew;
}

#if ENABLE_ALLOCATOR_TRACE
static void _VDEC_EX_Allocator_BlockPrint(VDEC_EX_Allocator_Block *pstBlock)
{
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG] pstBlock(%p):\n", pstBlock);

    if(pstBlock == NULL)
    {
        return;
    }

    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG] \tId=%d, PrevId=%d, NextId=%d, AllocatorIdExt=%d\n",
        pstBlock->u16BlockId,
        pstBlock->u16BlockIdPrev,
        pstBlock->u16BlockIdNext,
        pstBlock->u16AllocatorIdExt);

    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG] \tMIU=%d, Property=0x%x, Allocated=%d, Addr=0x%llx, Size=0x%x\n",
        pstBlock->stBlockInfo.u8MIU,
        pstBlock->stBlockInfo.u8Property,
        pstBlock->stBlockInfo.u16Allocated,
        pstBlock->stBlockInfo.phyAddr,
        pstBlock->stBlockInfo.szSize);

    return;
}

static void _VDEC_EX_Allocator_BlockListPrint(VDEC_EX_Allocator_BlockList *pstBlockList)
{
    VDEC_EX_Allocator_Block *pstBlockCurr;
    MS_U16 u16BlockIdCurr;

    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG] pstBlockList(%p):\n", pstBlockList);

    if(pstBlockList == NULL)
    {
        return;
    }

    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG] \tnumblocks=%d, HeadId=%d, TailId=%d\n",
        pstBlockList->u16NumBlocks, pstBlockList->u16BlockIdHead, pstBlockList->u16BlockIdTail);

    u16BlockIdCurr = pstBlockList->u16BlockIdHead;
    while(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdCurr) == TRUE)
    {
        pstBlockCurr = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdCurr);

        _VDEC_EX_Allocator_BlockPrint(pstBlockCurr);

        u16BlockIdCurr = pstBlockCurr->u16BlockIdNext;
    }

    return;
}

static void _VDEC_EX_Allocator_Print(VDEC_EX_Allocator *pstAllocator)
{
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG] pstAllocator(%p):\n", pstAllocator);

    if(pstAllocator == NULL)
    {
        return;
    }

    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG] \tAllocatorId=%d, Addr=0x%llx, Size=0x%x, UnuseSize=0x%x\n",
        pstAllocator->u16AllocatorId, pstAllocator->phyStartAddr,
        pstAllocator->szTotalSize, pstAllocator->szUnusedSize);

    _VDEC_EX_Allocator_BlockListPrint(&(pstAllocator->stBlockList));

    return;
}
#endif

// Functions of BlockList
static void _VDEC_EX_Allocator_BlockListReset(VDEC_EX_Allocator_BlockList *pstBlockList)
{
    pstBlockList->u16NumBlocks = 0;
    pstBlockList->u16BlockIdHead = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    pstBlockList->u16BlockIdTail = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    return;
}

static MS_U16 _VDEC_EX_Allocator_BlockListAddtoHead(VDEC_EX_Allocator_BlockList *pstBlockList,
    VDEC_EX_Allocator_BlockInfo *pstBlockInfoRef)
{
    MS_U16 u16BlockIdNew = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    VDEC_EX_Allocator_Block *pstBlockNew;

    if((pstBlockList == NULL) || (pstBlockInfoRef == NULL))
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid input\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    // get new block
    u16BlockIdNew = _VDEC_EX_Allocator_BlockGetNewId();
    if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdNew) == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] BlockGetNewId failed\n", __FUNCTION__, __LINE__);
        u16BlockIdNew = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
        goto exit;
    }

    // initial the block info of this new block
    pstBlockNew = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdNew);
    _VDEC_EX_Allocator_BlockInfoCopy(&(pstBlockNew->stBlockInfo), pstBlockInfoRef);

    // add this new block to the head of list
    if(pstBlockList->u16NumBlocks == 0)
    {
        pstBlockNew->u16BlockIdPrev = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
        pstBlockNew->u16BlockIdNext = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;

        pstBlockList->u16BlockIdHead = u16BlockIdNew;
        pstBlockList->u16BlockIdTail = u16BlockIdNew;
    }
    else
    {
        VDEC_EX_Allocator_Block *pstBlockTemp;

        pstBlockNew->u16BlockIdPrev = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
        pstBlockNew->u16BlockIdNext = pstBlockList->u16BlockIdHead;

        pstBlockTemp = _VDEC_EX_Allocator_BlockGetAddressById(pstBlockList->u16BlockIdHead);
        pstBlockTemp->u16BlockIdPrev = u16BlockIdNew;
        pstBlockList->u16BlockIdHead = u16BlockIdNew;
    }

    // update counter
    pstBlockList->u16NumBlocks += 1;

exit:
    return u16BlockIdNew;
}

static MS_U16 _VDEC_EX_Allocator_BlockListAddtoTail(VDEC_EX_Allocator_BlockList *pstBlockList,
    VDEC_EX_Allocator_BlockInfo *pstBlockInfoRef)
{
    MS_U16 u16BlockIdNew = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    VDEC_EX_Allocator_Block *pstBlockNew;

    if((pstBlockList == NULL) || (pstBlockInfoRef == NULL))
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid input\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    // get new block
    u16BlockIdNew = _VDEC_EX_Allocator_BlockGetNewId();
    if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdNew) == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] BlockGetNewId failed\n", __FUNCTION__, __LINE__);
        u16BlockIdNew = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
        goto exit;
    }

    // initial the block info of this new block
    pstBlockNew = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdNew);
    _VDEC_EX_Allocator_BlockInfoCopy(&(pstBlockNew->stBlockInfo), pstBlockInfoRef);

    // add this new block to the head of list
    if(pstBlockList->u16NumBlocks == 0)
    {
        pstBlockNew->u16BlockIdPrev = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
        pstBlockNew->u16BlockIdNext = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;

        pstBlockList->u16BlockIdHead = u16BlockIdNew;
        pstBlockList->u16BlockIdTail = u16BlockIdNew;
    }
    else
    {
        VDEC_EX_Allocator_Block *pstBlockTemp;

        pstBlockNew->u16BlockIdPrev = pstBlockList->u16BlockIdTail;
        pstBlockNew->u16BlockIdNext = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;

        pstBlockTemp = _VDEC_EX_Allocator_BlockGetAddressById(pstBlockList->u16BlockIdTail);
        pstBlockTemp->u16BlockIdNext = u16BlockIdNew;
        pstBlockList->u16BlockIdTail = u16BlockIdNew;
    }

    // update counter
    pstBlockList->u16NumBlocks += 1;

exit:
    return u16BlockIdNew;
}

static MS_U16 _VDEC_EX_Allocator_BlockListInsertBefore(VDEC_EX_Allocator_BlockList *pstBlockList,
    VDEC_EX_Allocator_BlockInfo *pstBlockInfoRef, MS_U16 u16BlockIdCurr)
{
    MS_U16 u16BlockIdNew = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    VDEC_EX_Allocator_Block *pstBlockNew   = NULL;
    VDEC_EX_Allocator_Block *pstBlockCurr  = NULL;
    VDEC_EX_Allocator_Block *pstBlockUpper = NULL;

    if(pstBlockList == NULL || pstBlockInfoRef == NULL ||
       _VDEC_EX_Allocator_BlockIsValidId(u16BlockIdCurr) == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid input\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    if(pstBlockList->u16NumBlocks <= 1 ||
       pstBlockList->u16BlockIdHead == u16BlockIdCurr)
    {
        u16BlockIdNew = _VDEC_EX_Allocator_BlockListAddtoHead(pstBlockList, pstBlockInfoRef);
        if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdNew) == FALSE)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] BlockListAddtoHead failed\n", __FUNCTION__, __LINE__);
            u16BlockIdNew = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
            goto exit;
        }
    }
    else
    {
        // get new block
        u16BlockIdNew = _VDEC_EX_Allocator_BlockGetNewId();
        if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdNew) == FALSE)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] BlockGetNewId failed\n", __FUNCTION__, __LINE__);
            u16BlockIdNew = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
            goto exit;
        }

        pstBlockNew   = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdNew);
        pstBlockCurr  = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdCurr);
        pstBlockUpper = _VDEC_EX_Allocator_BlockGetAddressById(pstBlockCurr->u16BlockIdPrev);

        // initial new block header by pRefBlockInfo
        _VDEC_EX_Allocator_BlockInfoCopy(&(pstBlockNew->stBlockInfo), pstBlockInfoRef);

        // add to list, before current block

        pstBlockUpper->u16BlockIdNext = u16BlockIdNew;
        pstBlockNew->u16BlockIdPrev   = pstBlockCurr->u16BlockIdPrev;

        pstBlockNew->u16BlockIdNext   = u16BlockIdCurr;
        pstBlockCurr->u16BlockIdPrev  = u16BlockIdNew;

        // update counter
        pstBlockList->u16NumBlocks += 1;
    }

exit:
    return u16BlockIdNew;
}

static VDEC_EX_Result _VDEC_EX_Allocator_BlockListRemove(VDEC_EX_Allocator_BlockList *pstBlockList,
    MS_U16 u16BlockIdCurr)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    VDEC_EX_Allocator_Block *pstBlockCurr = NULL;
    VDEC_EX_Allocator_Block *pstBlockPrev = NULL;
    VDEC_EX_Allocator_Block *pstBlockNext = NULL;

    if((pstBlockList == NULL) ||
        _VDEC_EX_Allocator_BlockIsValidId(u16BlockIdCurr) == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid input\n", __FUNCTION__, __LINE__);
        eRet = E_VDEC_EX_RET_INVALID_PARAM;
        goto exit;
    }

    // update counter
    if(pstBlockList->u16NumBlocks == 0)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] u16NumBlocks = 0\n", __FUNCTION__, __LINE__);
        eRet = E_VDEC_EX_FAIL;
        goto exit;
    }
    pstBlockList->u16NumBlocks -= 1;

    // remove current block from list
    pstBlockCurr = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdCurr);
    pstBlockCurr->u16BlockId = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;

    // re-link BlockPrev and BlockNext
    if(_VDEC_EX_Allocator_BlockIsValidId(pstBlockCurr->u16BlockIdPrev) == TRUE)
    {
        pstBlockPrev = _VDEC_EX_Allocator_BlockGetAddressById(pstBlockCurr->u16BlockIdPrev);
    }
    if(_VDEC_EX_Allocator_BlockIsValidId(pstBlockCurr->u16BlockIdNext) == TRUE)
    {
        pstBlockNext = _VDEC_EX_Allocator_BlockGetAddressById(pstBlockCurr->u16BlockIdNext);
    }

    if(pstBlockPrev == NULL && pstBlockNext == NULL)
    {
        // currrnt block is the last block
        pstBlockList->u16BlockIdHead = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
        pstBlockList->u16BlockIdTail = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    }
    else if(pstBlockPrev == NULL && pstBlockNext != NULL)
    {
        // current block is head block
        pstBlockNext->u16BlockIdPrev = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
        pstBlockList->u16BlockIdHead = pstBlockCurr->u16BlockIdNext;
    }
    else if(pstBlockPrev != NULL && pstBlockNext == NULL)
    {
        // current block is tail block
        pstBlockPrev->u16BlockIdNext = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
        pstBlockList->u16BlockIdTail = pstBlockCurr->u16BlockIdPrev;
    }
    else
    {
        // current block is located between 2 blocks
        pstBlockPrev->u16BlockIdNext = pstBlockCurr->u16BlockIdNext;
        pstBlockNext->u16BlockIdPrev = pstBlockCurr->u16BlockIdPrev;
    }

exit:
    return eRet;
}

// Functions of Allocator
static void _VDEC_EX_Allocator_Reset(VDEC_EX_Allocator *pstAllocator, MS_U16 u16Id)
{
    pstAllocator->u16AllocatorId = u16Id;
    pstAllocator->phyStartAddr = 0;
    pstAllocator->szTotalSize = 0;
    pstAllocator->szUnusedSize = 0;
    _VDEC_EX_Allocator_BlockListReset(&(pstAllocator->stBlockList));
    return;
}

static inline MS_BOOL _VDEC_EX_Allocator_IsValidId(MS_U16 u16AllocatorId)
{
    return (u16AllocatorId < VDEC_EX_ALLOCAOTR_MAX_NUM_ALLOCATOR) ? TRUE : FALSE;
}

static MS_U16 _VDEC_EX_Allocator_CountUsedAllocators(VDEC_EX_NDecAllocator *pstNDecAllocator)
{
    MS_U16 u16Cnt = 0;
    int i;
    for(i = 0; i < VDEC_EX_ALLOCAOTR_MAX_NUM_ALLOCATOR; i++)
    {
        MS_U16 u16AllocatorId = pstNDecAllocator->stAllocatorPool[i].u16AllocatorId;
        if(_VDEC_EX_Allocator_IsValidId(u16AllocatorId) == TRUE)
        {
            u16Cnt++;
        }
    }
    return u16Cnt;
}

static MS_U16 _VDEC_EX_Allocator_CountUsedBlocks(VDEC_EX_NDecAllocator *pstNDecAllocator)
{
    MS_U16 u16Cnt = 0;
    int i;
    for(i = 0; i < VDEC_EX_ALLOCAOTR_MAX_NUM_BLOCK; i++)
    {
        MS_U16 u16BlockId = pstNDecAllocator->stBlockPool[i].u16BlockId;
        if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockId) == TRUE)
        {
            u16Cnt++;
        }
    }
    return u16Cnt;
}

static VDEC_EX_Allocator *_VDEC_EX_Allocator_GetAddressById(MS_U16 u16ID)
{
    VDEC_EX_Allocator *pstAllocatorPool =
        &(pVdecExContext->stNDecAllocator.stAllocatorPool[0]);
    VDEC_EX_Allocator *pstAllocator = NULL;

    if(_VDEC_EX_Allocator_IsValidId(u16ID) == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid u16ID: %d\n", __FUNCTION__, __LINE__, u16ID);
        goto exit;
    }

    pstAllocator = &(pstAllocatorPool[u16ID]);

exit:
    return pstAllocator;
}

static MS_U16 _VDEC_EX_Allocator_GetNewId(void)
{
    VDEC_EX_Allocator *pstAllocatorPool =
        &(pVdecExContext->stNDecAllocator.stAllocatorPool[0]);
    MS_U16 u16NewAllocatorId = VDEC_EX_ALLOCAOTR_ID_UNUSED;
    MS_U16 u16Id;

    for(u16Id = 0; u16Id < VDEC_EX_ALLOCAOTR_MAX_NUM_ALLOCATOR; u16Id++)
    {
        VDEC_EX_Allocator *pstAllocator = &(pstAllocatorPool[u16Id]);
        if(pstAllocator->u16AllocatorId == VDEC_EX_ALLOCAOTR_ID_UNUSED)
        {
            u16NewAllocatorId = u16Id;
            _VDEC_EX_Allocator_Reset(pstAllocator, u16Id);
            break;
        }
    }

    return u16NewAllocatorId;
}

static VDEC_EX_Result _VDEC_EX_Allocator_Init(VDEC_EX_Allocator *pstAllocator, VDEC_EX_Allocator_BlockInfo *pstBlockInfo)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U16 u16BlockId;

    if(pstAllocator == NULL)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid input\n", __FUNCTION__, __LINE__);
        eRet = E_VDEC_EX_RET_INVALID_PARAM;
        goto exit;
    }

    pstAllocator->phyStartAddr = pstBlockInfo->phyAddr;
    pstAllocator->szTotalSize = pstBlockInfo->szSize;
    pstAllocator->szUnusedSize = pstBlockInfo->szSize;

    u16BlockId = _VDEC_EX_Allocator_BlockListAddtoTail(&(pstAllocator->stBlockList), pstBlockInfo);
    if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockId) == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] BlockListAddtoTail failed\n", __FUNCTION__, __LINE__);
        eRet = E_VDEC_EX_FAIL;
        goto exit;
    }

exit:
    return eRet;
}

static MS_U16 _VDEC_EX_Allocator_CheckMergePrevious(VDEC_EX_Allocator *pstAllocator,
    MS_U16 u16BlockIdCurr)
{
    MS_U16 u16BlockIdMerge = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    MS_U16 u16BlockIdPrev  = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    VDEC_EX_Allocator_Block *pstBlockPrev  = NULL;
    VDEC_EX_Allocator_Block *pstBlockCurr  = NULL;
    MS_SIZE szMergeSize;

    if(pstAllocator == NULL ||
       _VDEC_EX_Allocator_BlockIsValidId(u16BlockIdCurr) == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid input\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    u16BlockIdMerge = u16BlockIdCurr;
    pstBlockCurr    = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdCurr);
    u16BlockIdPrev  = pstBlockCurr->u16BlockIdPrev;

    if(pstBlockCurr->stBlockInfo.u16Allocated == 1 ||
       _VDEC_EX_Allocator_BlockIsValidId(u16BlockIdPrev) == FALSE)
    {
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Doesn't merge\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    pstBlockPrev = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdPrev);

    if(pstBlockPrev->stBlockInfo.u8MIU != pstBlockCurr->stBlockInfo.u8MIU ||
       pstBlockPrev->stBlockInfo.u8Property != pstBlockCurr->stBlockInfo.u8Property ||
       pstBlockPrev->stBlockInfo.u16Allocated == 1 ||
       pstBlockPrev->stBlockInfo.szSize == 0)
    {
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Doesn't merge\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    // start merge
    szMergeSize = pstBlockCurr->stBlockInfo.szSize;

    if(E_VDEC_EX_OK != _VDEC_EX_Allocator_BlockListRemove(&(pstAllocator->stBlockList), u16BlockIdCurr))
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] BlockListRemove failed\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    pstBlockPrev->stBlockInfo.szSize += szMergeSize;
    u16BlockIdMerge = u16BlockIdPrev;

exit:
    return u16BlockIdMerge;
}

static MS_U16 _VDEC_EX_Allocator_CheckMergeNext(VDEC_EX_Allocator *pstAllocator,
    MS_U16 u16BlockIdCurr)
{
    MS_U16 u16BlockIdMerge = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    MS_U16 u16BlockIdNext  = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    VDEC_EX_Allocator_Block *pstBlockCurr  = NULL;
    VDEC_EX_Allocator_Block *pstBlockNext = NULL;

    if(pstAllocator == NULL ||
       _VDEC_EX_Allocator_BlockIsValidId(u16BlockIdCurr) == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid input\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    u16BlockIdMerge = u16BlockIdCurr;
    pstBlockCurr    = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdCurr);
    u16BlockIdNext  = pstBlockCurr->u16BlockIdNext;

    if(pstBlockCurr->stBlockInfo.u16Allocated == 1 ||
       _VDEC_EX_Allocator_BlockIsValidId(u16BlockIdNext) == FALSE)
    {
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Doesn't merge\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    pstBlockNext = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdNext);

    if(pstBlockNext->stBlockInfo.u8MIU != pstBlockCurr->stBlockInfo.u8MIU ||
       pstBlockNext->stBlockInfo.u8Property != pstBlockCurr->stBlockInfo.u8Property ||
       pstBlockNext->stBlockInfo.u16Allocated == 1 ||
       pstBlockNext->stBlockInfo.szSize == 0)
    {
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Doesn't merge\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    // start merge
    u16BlockIdMerge = _VDEC_EX_Allocator_CheckMergePrevious(pstAllocator, u16BlockIdNext);

exit:
    return u16BlockIdMerge;
}

#define SPLIT_FLAG_BITS_DEFAULT      0x0
#define SPLIT_FLAG_BITS_MERGE_UPPER  VDEC_EX_BIT(0)
#define SPLIT_FLAG_BITS_MERGE_BELOW  VDEC_EX_BIT(1)
#define SPLIT_FLAG_BITS_RETURN_BELOW VDEC_EX_BIT(2)
MS_U16 _VDEC_EX_Allocator_BlockSplitBySize(VDEC_EX_Allocator *pstAllocator,
      MS_U16 u16BlockIdCurr, MS_SIZE szUpperBlockSize, MS_U32 u32Flag)
{
    MS_U16 u16BlockIdReturn = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    MS_U16 u16BlockIdUpper  = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    MS_U16 u16BlockIdBelow  = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    VDEC_EX_Allocator_Block *pstBlockCurr  = NULL;
    VDEC_EX_Allocator_BlockInfo stBlockInfoRef;

    if(pstAllocator == NULL ||
       _VDEC_EX_Allocator_BlockIsValidId(u16BlockIdCurr) == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid input\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    pstBlockCurr = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdCurr);
    if(szUpperBlockSize > pstBlockCurr->stBlockInfo.szSize)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] No enough size to split\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    if(szUpperBlockSize == 0 ||
       szUpperBlockSize == pstBlockCurr->stBlockInfo.szSize)
    {
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Doesn't split\n", __FUNCTION__, __LINE__);
        u16BlockIdReturn = u16BlockIdCurr;
        goto exit;
    }
    else
    {
        // insert new block before pstBlockCurr
        stBlockInfoRef.u8MIU = pstBlockCurr->stBlockInfo.u8MIU;
        stBlockInfoRef.u8Property = pstBlockCurr->stBlockInfo.u8Property;
        stBlockInfoRef.u16Allocated = pstBlockCurr->stBlockInfo.u16Allocated;
        stBlockInfoRef.phyAddr = pstBlockCurr->stBlockInfo.phyAddr;
        stBlockInfoRef.szSize = szUpperBlockSize;
        u16BlockIdUpper = _VDEC_EX_Allocator_BlockListInsertBefore(&(pstAllocator->stBlockList), &stBlockInfoRef, u16BlockIdCurr);
        if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdUpper) == FALSE)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] BlockListInsertBefore failed\n", __FUNCTION__, __LINE__);
            goto exit;
        }

        // update info of pstBlockCurr
        pstBlockCurr->stBlockInfo.phyAddr += szUpperBlockSize;
        pstBlockCurr->stBlockInfo.szSize  -= szUpperBlockSize;

        u16BlockIdBelow = u16BlockIdCurr;

        // check merge
        if(u32Flag & SPLIT_FLAG_BITS_MERGE_UPPER)
        {
            u16BlockIdUpper = _VDEC_EX_Allocator_CheckMergePrevious(pstAllocator, u16BlockIdUpper);
        }
        if(u32Flag & SPLIT_FLAG_BITS_MERGE_BELOW)
        {
            u16BlockIdBelow = _VDEC_EX_Allocator_CheckMergeNext(pstAllocator, u16BlockIdBelow);
        }
    }

    if(u32Flag & SPLIT_FLAG_BITS_RETURN_BELOW)
    {
        u16BlockIdReturn = u16BlockIdBelow;
    }
    else
    {
        // default will run here
        u16BlockIdReturn = u16BlockIdUpper;
    }

exit:
    return u16BlockIdReturn;
}

static MS_U16 _VDEC_EX_Allocator_BlockCheckMatch(VDEC_EX_Allocator *pstAllocator,
    VDEC_EX_Allocator_Criterion *pstCriterion, MS_U16 u16BlockIdCurr)
{
    VDEC_EX_Allocator_Block *pstBlockCurr  = NULL;
    MS_U16  u16BlockIdMatch = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    MS_SIZE szBlockSize;
    MS_PHY  phyBlockStartAddr, phyBlockEndAddr;
    MS_SIZE szCheckSize;
    MS_PHY  phyCheckStartAddr, phyCheckEndAddr;
    MS_PHY  phyAlignShift = 0;

    if(pstAllocator == NULL || pstCriterion == NULL ||
       _VDEC_EX_Allocator_BlockIsValidId(u16BlockIdCurr) == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid input\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    pstBlockCurr = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdCurr);
    if((pstCriterion->szSize == 0) ||
       (pstBlockCurr->stBlockInfo.szSize == 0) ||
       ((pstBlockCurr->stBlockInfo.u16Allocated == 1) &&
        ((pstBlockCurr->stBlockInfo.u8Property & VDEC_EX_ALLOCAOTR_PROPERTY_CAN_OVERLAP) == 0)))
    {
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Pre condition doesn't match\n", __FUNCTION__, __LINE__);
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG]\tpstCriterion->szSize = 0x%x\n",                 pstCriterion->szSize);
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG]\tpstBlockCurr->stBlockInfo.szSize = 0x%x\n",     pstBlockCurr->stBlockInfo.szSize);
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG]\tpstBlockCurr->stBlockInfo.u16Allocated=0x%x\n", pstBlockCurr->stBlockInfo.u16Allocated);
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG]\tpstBlockCurr->stBlockInfo.u8Property = 0x%x\n", pstBlockCurr->stBlockInfo.u8Property);
        goto exit;
    }

    // boundary and size condition
    szCheckSize       = pstCriterion->szSize;
    phyCheckStartAddr = pstCriterion->phyAddr;
    phyCheckEndAddr   = phyCheckStartAddr + szCheckSize;

    szBlockSize       = pstBlockCurr->stBlockInfo.szSize;
    phyBlockStartAddr = pstBlockCurr->stBlockInfo.phyAddr;
    phyBlockEndAddr   = phyBlockStartAddr + szBlockSize;

    #if 1
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] CSize=0x%x, CStart=0x%llx, CEnd=0x%llx\n", __FUNCTION__, __LINE__,
        szCheckSize, phyCheckStartAddr, phyCheckEndAddr);
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] BSize=0x%x, BStart=0x%llx, BEnd=0x%llx\n", __FUNCTION__, __LINE__,
        szBlockSize, phyBlockStartAddr, phyBlockEndAddr);
    #endif

    // check alignment
    if(pstCriterion->u32Alignment != 0)
    {
        phyAlignShift = phyBlockStartAddr & ((MS_PHY)(pstCriterion->u32Alignment - 1));
        if(phyAlignShift != 0)
        {
            phyAlignShift = pstCriterion->u32Alignment - phyAlignShift;
        }

        if(phyAlignShift >= szBlockSize)
        {
            ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Alignment shift larger than current block size\n", __FUNCTION__, __LINE__);
            goto exit;
        }

        phyBlockStartAddr += phyAlignShift;
        szBlockSize       -= phyAlignShift;
    }

    // start checking

    if(pstCriterion->u32CheckItem & VDEC_EX_ALLOCATOR_CRITERION_CHECK_SIZE)
    {
        if(pstCriterion->u32CheckItem & VDEC_EX_ALLOCATOR_CRITERION_CHECK_ADDRESS)
        {
            if((phyCheckStartAddr >= phyBlockStartAddr) &&
               (phyCheckEndAddr   <= phyBlockEndAddr  ))
            {
                u16BlockIdMatch = u16BlockIdCurr;
            }
        }
        else
        {
            if(szCheckSize <= szBlockSize)
            {
                u16BlockIdMatch = u16BlockIdCurr;
            }
        }
    }

    // if aligment
    if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdMatch) == TRUE && phyAlignShift != 0)
    {
        u16BlockIdMatch = _VDEC_EX_Allocator_BlockSplitBySize(pstAllocator, u16BlockIdMatch,
            phyAlignShift, SPLIT_FLAG_BITS_RETURN_BELOW);
    }

exit:
    return u16BlockIdMatch;
}

static MS_U16 _VDEC_EX_Allocator_BlockListSearch(VDEC_EX_Allocator *pstAllocator,
    VDEC_EX_Allocator_Criterion *pstCriterion)
{
    VDEC_EX_Allocator_BlockList *pstBlockList = NULL;
    VDEC_EX_Allocator_Block *pstBlockCurr = NULL;
    MS_U16 u16BlockIdCurr  = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    MS_U16 u16BlockIdMatch = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;

    if(pstAllocator == NULL || pstCriterion == NULL)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid input\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    pstBlockList = &(pstAllocator->stBlockList);
    u16BlockIdCurr = pstBlockList->u16BlockIdHead;

    while(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdCurr) == TRUE)
    {
        u16BlockIdMatch = _VDEC_EX_Allocator_BlockCheckMatch(pstAllocator, pstCriterion, u16BlockIdCurr);
        if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdMatch) == TRUE)
        {
            break;
        }

        pstBlockCurr = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdCurr);
        u16BlockIdCurr = pstBlockCurr->u16BlockIdNext;
    }

exit:
    return u16BlockIdMatch;
}

static MS_U16 _VDEC_EX_Allocator_Assign(VDEC_EX_Allocator *pstAllocator,
    VDEC_EX_Allocator_Criterion *pstCriterion, MS_U16 u16BlockIdMatch)
{
    MS_U16 u16BlockIdAssign = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    VDEC_EX_Allocator_Block *pstBlockMatch  = NULL;
    MS_SIZE szAssignSize;
    MS_PHY phyAssignStartAddr, phyAssignEndAddr;

    if(pstAllocator == NULL || pstCriterion == NULL ||
       _VDEC_EX_Allocator_BlockIsValidId(u16BlockIdMatch) == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid input\n", __FUNCTION__, __LINE__);
        goto exit;
    }
    pstBlockMatch = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdMatch);

    szAssignSize = pstCriterion->szSize;
    phyAssignStartAddr = pstCriterion->phyAddr;
    if(phyAssignStartAddr == 0)
    {
        phyAssignStartAddr = pstBlockMatch->stBlockInfo.phyAddr;
    }

    if(szAssignSize == 0 ||
       pstBlockMatch->stBlockInfo.u16Allocated == 1 ||
       pstBlockMatch->stBlockInfo.szSize == 0)
    {
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Doesn't assign\n", __FUNCTION__, __LINE__);
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] szAssignSize=0x%x, u16Allocated=%d, szSize=0x%x\n", __FUNCTION__, __LINE__,
            szAssignSize, pstBlockMatch->stBlockInfo.u16Allocated, pstBlockMatch->stBlockInfo.szSize);
        goto exit;
    }

    phyAssignEndAddr = phyAssignStartAddr + szAssignSize;
    if((phyAssignStartAddr < pstBlockMatch->stBlockInfo.phyAddr) ||
       (phyAssignEndAddr   > (pstBlockMatch->stBlockInfo.phyAddr + pstBlockMatch->stBlockInfo.szSize)))
    {
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Doesn't assign\n", __FUNCTION__, __LINE__);
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] phyAssignStartAddr=0x%llx, phyAssignEndAddr=0x%llx\n", __FUNCTION__, __LINE__,
            phyAssignStartAddr, phyAssignEndAddr);
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] stBlockInfo.phyAddr=0x%llx, stBlockInfo.szSize=0x%x\n", __FUNCTION__, __LINE__,
            pstBlockMatch->stBlockInfo.phyAddr, pstBlockMatch->stBlockInfo.szSize);
        goto exit;
    }

    if(phyAssignStartAddr > pstBlockMatch->stBlockInfo.phyAddr)
    {
        MS_PHY phyAddrDiff = phyAssignStartAddr - pstBlockMatch->stBlockInfo.phyAddr;
        if(sizeof(MS_PHY) == 8 && (phyAddrDiff >> 32))
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Overflow\n", __FUNCTION__, __LINE__);
            goto exit;
        }

        u16BlockIdMatch = _VDEC_EX_Allocator_BlockSplitBySize(pstAllocator, u16BlockIdMatch,
            (MS_U32)phyAddrDiff, SPLIT_FLAG_BITS_MERGE_UPPER | SPLIT_FLAG_BITS_RETURN_BELOW);
        if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdMatch) == FALSE)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] BlockSplitBySize failed\n", __FUNCTION__, __LINE__);
            goto exit;
        }
        pstBlockMatch = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdMatch);
    }

    if(szAssignSize != pstBlockMatch->stBlockInfo.szSize)
    {
        u16BlockIdMatch = _VDEC_EX_Allocator_BlockSplitBySize(pstAllocator, u16BlockIdMatch,
            szAssignSize, SPLIT_FLAG_BITS_DEFAULT);
        if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdMatch) == FALSE)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] BlockSplitBySize failed\n", __FUNCTION__, __LINE__);
            goto exit;
        }
        pstBlockMatch = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdMatch);
    }

    // update Allocator info
    pstBlockMatch->stBlockInfo.u16Allocated = 1;
    pstAllocator->szUnusedSize -= pstBlockMatch->stBlockInfo.szSize;
    u16BlockIdAssign = u16BlockIdMatch;

exit:
    return u16BlockIdAssign;
}

static MS_U16 _VDEC_EX_Allocator_MallocStart(VDEC_EX_Allocator *pstAllocator,
    VDEC_EX_Allocator_Criterion *pstCriterion)
{
    VDEC_EX_Allocator_Block *pstBlockMatch = NULL;
    MS_U16 u16BlockIdMatch = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;

    if(pstAllocator == NULL || pstCriterion == NULL)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid input\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    u16BlockIdMatch = _VDEC_EX_Allocator_BlockListSearch(pstAllocator, pstCriterion);
    if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdMatch) == FALSE)
    {
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] BlockListSearch find none\n", __FUNCTION__, __LINE__);
        u16BlockIdMatch = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
        goto exit;
    }

    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Find u16BlockIdMatch=%d\n", __FUNCTION__, __LINE__, u16BlockIdMatch);

    pstBlockMatch = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdMatch);

    if((pstBlockMatch->stBlockInfo.u8Property & VDEC_EX_ALLOCAOTR_PROPERTY_CAN_OVERLAP) == 0)
    {
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Start Assign\n", __FUNCTION__, __LINE__);

        u16BlockIdMatch = _VDEC_EX_Allocator_Assign(pstAllocator, pstCriterion, u16BlockIdMatch);
        if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdMatch) == FALSE)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Allocator_Assign failed\n", __FUNCTION__, __LINE__);
            u16BlockIdMatch = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
            goto exit;
        }
    }

exit:
    return u16BlockIdMatch;
}

static MS_U16 _VDEC_EX_Allocator_Free(VDEC_EX_Allocator *pstAllocator, MS_U16 u16BlockIdFree)
{
    MS_U16 u16BlockIdMerge = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    VDEC_EX_Allocator_Block *pstBlockFree  = NULL;

    if(pstAllocator == NULL ||
       _VDEC_EX_Allocator_BlockIsValidId(u16BlockIdFree) == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid input\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    pstBlockFree = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdFree);

    if(pstBlockFree->stBlockInfo.u16Allocated == 0)
    {
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Doesn't free\n", __FUNCTION__, __LINE__);
        u16BlockIdMerge = u16BlockIdFree;
        goto exit;
    }

    pstAllocator->szUnusedSize += pstBlockFree->stBlockInfo.szSize;

    pstBlockFree->stBlockInfo.u16Allocated = 0;

    // check merge upper block
    u16BlockIdMerge = _VDEC_EX_Allocator_CheckMergePrevious(pstAllocator, u16BlockIdFree);
    if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdMerge) == TRUE)
    {
        u16BlockIdFree = u16BlockIdMerge;
    }

    // check merge lower block
    u16BlockIdMerge = _VDEC_EX_Allocator_CheckMergeNext(pstAllocator, u16BlockIdFree);
    if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdMerge) == TRUE)
    {
        u16BlockIdFree = u16BlockIdMerge;
    }

exit:
    return u16BlockIdMerge;
}

// N Decode Allocator Functions
static VDEC_EX_Result _VDEC_EX_NDecAllocator_Init(VDEC_EX_NDecAllocator *pstNDecAllocator)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U16 i;

    if(pstNDecAllocator->bInit == FALSE)
    {
        // only initial NDec Allocator onece
        pstNDecAllocator->bInit = TRUE;

        // initial Allocator Record for each decoding task
        for(i = 0; i < VDEC_MAX_SUPPORT_STREAM_NUM; i++)
        {
            VDEC_EX_Allocator_Record *pstRecord = &(pstNDecAllocator->stAllocatorRecord[i]);
            pstRecord->u16BlockIdFB1Main = VDEC_EX_ALLOCAOTR_ID_UNUSED;
            pstRecord->u16BlockIdFB1Sub  = VDEC_EX_ALLOCAOTR_ID_UNUSED;
            pstRecord->u16BlockIdFB2Main = VDEC_EX_ALLOCAOTR_ID_UNUSED;
            pstRecord->u16BlockIdFB2Sub  = VDEC_EX_ALLOCAOTR_ID_UNUSED;
            pstRecord->u16BlockIdBS1Main = VDEC_EX_ALLOCAOTR_ID_UNUSED;
            pstRecord->u16BlockIdBS1Sub  = VDEC_EX_ALLOCAOTR_ID_UNUSED;
#ifdef CMA_DRV_DIRECT_INIT
            pstNDecAllocator->szCMAAllocLength[E_VDEC_EX_CMA_POOL_BUFFER_FB1][i] = 0;
            pstNDecAllocator->szCMAAllocLength[E_VDEC_EX_CMA_POOL_BUFFER_FB2][i] = 0;
#endif
        }

        // initial Block Pool
        for(i = 0; i < VDEC_EX_ALLOCAOTR_MAX_NUM_BLOCK; i++)
        {
            VDEC_EX_Allocator_Block *pstBlock = &(pstNDecAllocator->stBlockPool[i]);
            _VDEC_EX_Allocator_BlockReset(pstBlock, VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED);
        }

        // initial Allocator Pool
        for(i = 0; i < VDEC_EX_ALLOCAOTR_MAX_NUM_ALLOCATOR; i++)
        {
            VDEC_EX_Allocator *pstAllocator = &(pstNDecAllocator->stAllocatorPool[i]);
            _VDEC_EX_Allocator_Reset(pstAllocator, VDEC_EX_ALLOCAOTR_ID_UNUSED);
        }

        // Get a valid allocator from allocator pool
        pstNDecAllocator->u16AllocatorIdMain = _VDEC_EX_Allocator_GetNewId();
        if(_VDEC_EX_Allocator_IsValidId(pstNDecAllocator->u16AllocatorIdMain) == FALSE)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Get Main Allocator Failed\n", __FUNCTION__, __LINE__);
            eRet = E_VDEC_EX_FAIL;
            goto exit;
        }
        else
        {
            MS_U16 u16BlockId;
            VDEC_EX_Allocator *pstAllocatorMain;
            VDEC_EX_Allocator_BlockInfo stBlockInfoRef;

            // TODO: Allocator Add block Module

            // Initial Allocator Main with block MIU0
            stBlockInfoRef.u8MIU = 0;
            stBlockInfoRef.u8Property = VDEC_EX_ALLOCAOTR_PROPERTY_DEFAULT;
            stBlockInfoRef.u16Allocated = 0;
            stBlockInfoRef.phyAddr = 0;
            stBlockInfoRef.szSize = 0x80000000;
            pstAllocatorMain = _VDEC_EX_Allocator_GetAddressById(pstNDecAllocator->u16AllocatorIdMain);
            eRet = _VDEC_EX_Allocator_Init(pstAllocatorMain, &stBlockInfoRef);
            if(eRet != E_VDEC_EX_OK)
            {
                ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Allocator_Init failed\n", __FUNCTION__, __LINE__);
                eRet = E_VDEC_EX_FAIL;
                goto exit;
            }

            // Add block MIU1
            stBlockInfoRef.u8MIU = 1;
            stBlockInfoRef.u8Property = VDEC_EX_ALLOCAOTR_PROPERTY_DEFAULT;
            stBlockInfoRef.u16Allocated = 0;
            stBlockInfoRef.phyAddr = 0x80000000;
            stBlockInfoRef.szSize = 0x80000000 - 1;  // prevent size(32bits) overflow
            u16BlockId = _VDEC_EX_Allocator_BlockListAddtoTail(&(pstAllocatorMain->stBlockList), &stBlockInfoRef);
            if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockId) == FALSE)
            {
                ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] BlockListAddtoTail failed\n", __FUNCTION__, __LINE__);
                goto exit;
            }
            pstAllocatorMain->szTotalSize += stBlockInfoRef.szSize;
            pstAllocatorMain->szUnusedSize += stBlockInfoRef.szSize;

            #if 0
            ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] After NDecAllocator_Init, pstAllocatorMain:\n", __FUNCTION__, __LINE__);
            _VDEC_EX_Allocator_Print(pstAllocatorMain);
            #endif
        }
    }

exit:
    return eRet;
}

static MS_BOOL _VDEC_EX_NdecAllocator_IsNDecode(void)
{
    return pVdecExContext->bDRVdecideBS;
}

static VDEC_EX_Result _VDEC_EX_NdecAllocator_GetBitstreamBufferProperty(MS_U8 u8Idx, VDEC_EX_NDecBufferProperty *pstProperty)
{
    MS_BOOL bNDecode  = _VDEC_EX_NdecAllocator_IsNDecode();
    MS_BOOL bMonoPoly = pVdecExContext->_Pre_Ctrl[u8Idx].bMonopolyBitstreamBuffer;
    MS_BOOL bSetTotalBuf = pVdecExContext->_Pre_Ctrl[u8Idx].stTotalBufRgnBS.bSetTotalBuf;
    VDEC_EX_SrcMode   eSrcMode   = pVdecExContext->_Attr[u8Idx].eSrcMode;
    VDEC_EX_CodecType eCodecType = pVdecExContext->_Attr[u8Idx].eCodecType;
    MS_BOOL bUseSubAllocator = FALSE;

    if(bNDecode == FALSE)
    {
        if(bSetTotalBuf == TRUE)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] bSetTotalBuf only valid in NDecode\n", __FUNCTION__, __LINE__);
            return E_VDEC_EX_FAIL;
        }
    }

    if(bNDecode == TRUE)
    {
        if(bMonoPoly == FALSE || bSetTotalBuf == TRUE)
        {
            bUseSubAllocator = TRUE;
        }
    }

    pstProperty->bCMA = FALSE;
    pstProperty->bDynamicCMA = FALSE;
    pstProperty->bNDecode = bNDecode;
    pstProperty->bUseSubAllocator = bUseSubAllocator;
    pstProperty->bUseThirdAllocator = FALSE;
    pstProperty->bMonopoly = bMonoPoly;
    pstProperty->eSrcMode = eSrcMode;
    pstProperty->eCodecType = eCodecType;
    pstProperty->eTaskSpec = E_VDEC_EX_SPEC_DEFAULT;

    return E_VDEC_EX_OK;
}

static VDEC_EX_Result _VDEC_EX_NdecAllocator_GetFrameBufferProperty(MS_U8 u8Idx, VDEC_EX_NDecBufferProperty *pstProperty)
{
    MS_BOOL bCMA        = pVdecExContext->bCMAUsed;
    MS_BOOL bCMADynamic = FALSE/*pVdecExContext->_Pre_Ctrl[u8Idx].bEnableDynamicCMA*/;
    MS_BOOL bNDecode    = _VDEC_EX_NdecAllocator_IsNDecode();
    MS_BOOL bMonoPoly   = pVdecExContext->_Pre_Ctrl[u8Idx].bMonopolyFrameBuffer;
    VDEC_EX_SrcMode   eSrcMode   = pVdecExContext->_Attr[u8Idx].eSrcMode;
    VDEC_EX_CodecType eCodecType = pVdecExContext->_Attr[u8Idx].eCodecType;
    MS_BOOL bUseSubAllocator = FALSE;
    MS_BOOL bUseThirdAllocator = FALSE;

    if(bNDecode == TRUE)
    {
        // If monopoly, disable NDecode
        if(bMonoPoly == TRUE)
        {
            bNDecode = FALSE;
        }
    }
    else
    {
        // If not N decode, only stream 0 will use CMA
        if(u8Idx != 0)
        {
            bCMA = FALSE;
        }
    }

    // only HEVC and HVD will use CMA_Dynamic
    //if((bCMA == FALSE) ||
    //   (eCodecType != E_VDEC_EX_CODEC_TYPE_HEVC && eCodecType != E_VDEC_EX_CODEC_TYPE_H264))
    {
        bCMADynamic = FALSE;
    }

    if(((bNDecode == TRUE) && (bMonoPoly == FALSE)) || (bCMADynamic == TRUE))
    {
        bUseSubAllocator = TRUE;
    }

    if((bNDecode == TRUE) && (bCMADynamic == TRUE))
    {
        bUseThirdAllocator = TRUE;
    }

    pstProperty->bCMA = bCMA;
    pstProperty->bDynamicCMA = bCMADynamic;
    pstProperty->bNDecode = bNDecode;
    pstProperty->bUseSubAllocator = bUseSubAllocator;
    pstProperty->bUseThirdAllocator = bUseThirdAllocator;
    pstProperty->bMonopoly = bMonoPoly;
    pstProperty->eSrcMode = eSrcMode;
    pstProperty->eCodecType = eCodecType;
    pstProperty->eTaskSpec = pVdecExContext->_Pre_Ctrl[u8Idx].eTaskSpec;

    return E_VDEC_EX_OK;
}

static VDEC_EX_CMA_POOL_SELECT _VDEC_EX_NdecAllocator_CMAPoolSelect(VDEC_EX_ALLOCATOR_BUFFER_TYPE eBufType)
{
    VDEC_EX_CMA_POOL_SELECT eCMAPoolSelect = E_VDEC_EX_CMA_POOL_INVALID;

    switch(eBufType)
    {
        case E_VDEC_EX_ALLOCATOR_BUFFER_FB1:
            eCMAPoolSelect = E_VDEC_EX_CMA_POOL_BUFFER_FB1;
            break;

        case E_VDEC_EX_ALLOCATOR_BUFFER_FB2:
            eCMAPoolSelect = E_VDEC_EX_CMA_POOL_BUFFER_FB2;
            break;

        default:
            break;
    }

    return eCMAPoolSelect;
}

#ifdef CMA_DRV_DIRECT_INIT
static VDEC_EX_Result _VDEC_EX_NdecAllocator_CMAInit(VDEC_EX_CMA_POOL_SELECT eCMAPoolSelect, MS_PHY phyBufAddr,
    MS_PHY *pphyCMAStartAddr, MS_SIZE *pszCMAHeapLength, MS_PHY *pphyUnUseSize)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    struct CMA_Pool_Init_Param *pstCmaInitParam = &(pVdecExContext->stNDecAllocator.stCmaInitParam[eCMAPoolSelect]);
    MS_PHY phyCMAStartAddr;
    MS_PHY phyUnUseSize = 0;

    //if(bCMAInitPool[eCMAPoolSelect] == FALSE)
    {
        memset(pstCmaInitParam, 0, sizeof(struct CMA_Pool_Init_Param));

        pstCmaInitParam->heap_id = (eCMAPoolSelect == E_VDEC_EX_CMA_POOL_BUFFER_FB1) ? ION_VDEC_HEAP_ID : ION_VDEC_HEAP_ID + 1;
        pstCmaInitParam->flags = CMA_FLAG_MAP_VMA | CMA_FLAG_CACHED;

        if (MApi_CMA_Pool_Init(pstCmaInitParam) == FALSE)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] MApi_CMA_Pool_Init(0x%x) failed\n", __FUNCTION__, __LINE__, pstCmaInitParam->heap_id);
            eRet = E_VDEC_EX_FAIL;
            goto exit;
        }

        bCMAInitPool[eCMAPoolSelect] = TRUE;
    }

    _miu_offset_to_phy(pstCmaInitParam->miu, pstCmaInitParam->heap_miu_start_offset, phyCMAStartAddr);

    if(phyBufAddr != 0)
    {
        phyUnUseSize = phyBufAddr - phyCMAStartAddr;
        if((sizeof(MS_PHY) == 8) && (phyUnUseSize >> 32))
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] phyUnUseSize(0x%llx) overflow\n", __FUNCTION__, __LINE__, phyUnUseSize);
            goto exit;
        }
    }

    *pphyCMAStartAddr = phyCMAStartAddr;
    *pszCMAHeapLength = pstCmaInitParam->heap_length;
    *pphyUnUseSize = phyUnUseSize;

    VPRINTF("[ALLOC][DBG][%s][%d] MApi_CMA_Pool_Init(%d): id=0x%x, miu=%d, offset=0x%llx, length=0x%x, BufAddr=0x%llx, UnUseSize=0x%llx\n",
        __FUNCTION__, __LINE__, eCMAPoolSelect,
        pstCmaInitParam->pool_handle_id,
        pstCmaInitParam->miu,
        pstCmaInitParam->heap_miu_start_offset,
        pstCmaInitParam->heap_length,
        phyBufAddr,
        phyUnUseSize);

exit:
    return eRet;
}

static VDEC_EX_Result _VDEC_EX_NdecAllocator_CMAAlloc(VDEC_EX_CMA_POOL_SELECT eCMAPoolSelect, MS_PHY phyBufAddr, MS_SIZE szBufSize)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;

    if(bCMAInitPool[eCMAPoolSelect] == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] bCMAInit[%d] == FALSE\n", __FUNCTION__, __LINE__, eCMAPoolSelect);
        eRet = E_VDEC_EX_FAIL;
    }
    else
    {
        struct CMA_Pool_Alloc_Param stAllocParam;
        struct CMA_Pool_Init_Param *pstCmaInitParam = &(pVdecExContext->stNDecAllocator.stCmaInitParam[eCMAPoolSelect]);
        MS_U64 u64offset;
        MS_U8 u8MIU;

        memset(&stAllocParam, 0, sizeof(struct CMA_Pool_Alloc_Param));

        _phy_to_miu_offset(u8MIU, u64offset, phyBufAddr);

        if(u64offset < pstCmaInitParam->heap_miu_start_offset)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] MApi_CMA_Pool_GetMem(%d) fail\n", __FUNCTION__, __LINE__, eCMAPoolSelect);
            eRet = E_VDEC_EX_FAIL;
            goto exit;
        }

        stAllocParam.pool_handle_id = pstCmaInitParam->heap_id;
        stAllocParam.flags = CMA_FLAG_VIRT_ADDR;
        stAllocParam.offset_in_pool = u64offset - pstCmaInitParam->heap_miu_start_offset;
        stAllocParam.length = szBufSize;

        VPRINTF("[ALLOC][DBG][%s][%d] MApi_CMA_Pool_GetMem(%d): id=0x%x, flags=0x%x, offset_in_pool=0x%llx, length=0x%x\n",
            __FUNCTION__, __LINE__, eCMAPoolSelect,
            stAllocParam.pool_handle_id,
            stAllocParam.flags,
            stAllocParam.offset_in_pool,
            stAllocParam.length);

        if(MApi_CMA_Pool_GetMem(&stAllocParam) == FALSE)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] MApi_CMA_Pool_GetMem(%d) fail\n", __FUNCTION__, __LINE__, eCMAPoolSelect);
            eRet = E_VDEC_EX_FAIL;
            goto exit;
        }
    }

exit:
    return eRet;
}

static VDEC_EX_Result _VDEC_EX_NdecAllocator_CMAFree(VDEC_EX_CMA_POOL_SELECT eCMAPoolSelect, MS_PHY phyBufAddr, MS_SIZE szBufSize)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;

    if(bCMAInitPool[eCMAPoolSelect] == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] bCMAInit[%d] == FALSE\n", __FUNCTION__, __LINE__, eCMAPoolSelect);
        eRet = E_VDEC_EX_FAIL;
    }
    else
    {
        struct CMA_Pool_Free_Param stCmaFreeParam;
        struct CMA_Pool_Init_Param *pstCmaInitParam = &(pVdecExContext->stNDecAllocator.stCmaInitParam[eCMAPoolSelect]);
        MS_U64 u64offset;
        MS_U8 u8MIU;

        memset(&stCmaFreeParam, 0, sizeof(struct CMA_Pool_Free_Param));

        _phy_to_miu_offset(u8MIU, u64offset, phyBufAddr);

        if(u64offset < pstCmaInitParam->heap_miu_start_offset)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] MApi_CMA_Pool_PutMem(%d) fail\n", __FUNCTION__, __LINE__, eCMAPoolSelect);
            eRet = E_VDEC_EX_FAIL;
            goto exit;
        }

        stCmaFreeParam.pool_handle_id = pstCmaInitParam->heap_id;
        stCmaFreeParam.offset_in_pool = u64offset - pstCmaInitParam->heap_miu_start_offset;
        stCmaFreeParam.length = szBufSize;

        VPRINTF("[ALLOC][DBG][%s][%d] MApi_CMA_Pool_PutMem(%d): id=0x%x, offset_in_pool=0x%llx, length=0x%x\n",
            __FUNCTION__, __LINE__, eCMAPoolSelect,
            stCmaFreeParam.pool_handle_id,
            stCmaFreeParam.offset_in_pool,
            stCmaFreeParam.length);

        if(MApi_CMA_Pool_PutMem(&stCmaFreeParam) == FALSE)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] MApi_CMA_Pool_PutMem(%d) fail\n", __FUNCTION__, __LINE__, eCMAPoolSelect);
            eRet = E_VDEC_EX_FAIL;
            goto exit;
        }
    }

exit:
    return eRet;
}
#endif

#define MEGA(v) ((v)*1024*1024)
static MS_SIZE _VDEC_EX_NdecAllocator_GetTaskSpecSize(VDEC_EX_TASK_SPEC eTaskSpec, VDEC_EX_CodecType eCodecType)
{
    MS_SIZE szFrameBufferSize = 0;

    if(eTaskSpec == E_VDEC_EX_SPEC_DEFAULT)
    {
        szFrameBufferSize = MDrv_HVD_EX_GetFrameBufferDefaultSize(eCodecType);
    }
    else
    {
        // FIXME: Table value
        switch(eTaskSpec)
        {
        case E_VDEC_EX_SPEC_HEVC_HD_LEVEL41:
            szFrameBufferSize = MEGA(100);
            break;
        case E_VDEC_EX_SPEC_HEVC_FHD_LEVEL41:
            szFrameBufferSize = MEGA(100);
            break;
        case E_VDEC_EX_SPEC_HEVC_FHD_LEVEL51:
            szFrameBufferSize = MEGA(160);
            break;
        case E_VDEC_EX_SPEC_HEVC_UHD_LEVEL51:
            szFrameBufferSize = MEGA(160);
            break;
        case E_VDEC_EX_SPEC_H264_HD_LEVEL40:
            szFrameBufferSize = MEGA(16);
            break;
        case E_VDEC_EX_SPEC_H264_FHD_LEVEL40:
            szFrameBufferSize = MEGA(27);
            break;
        case E_VDEC_EX_SPEC_H264_UHD_LEVEL50:
            szFrameBufferSize = MEGA(150);
            break;
        default:
            szFrameBufferSize = 0;
            break;
        }
    }

    return szFrameBufferSize;
}

static MS_U16 _VDEC_EX_NdecAllocator_GetMainBlock(VDEC_EX_NDecAllocator *pNDecAllocator,
    VDEC_EX_ALLOCATOR_BUFFER_TYPE eBufType, VDEC_EX_NDecBufRangeParam *pstBufRgn, VDEC_EX_NDecBufferProperty *pstProperty)
{
    VDEC_EX_Allocator_Criterion stCriterion;
    VDEC_EX_Allocator *pstAllocatorMain;
    MS_U16 u16BlockIdMain = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    MS_PHY phyMainBlockAddr = 0;
    MS_SIZE szMainBlockSize = 0;

    if(pNDecAllocator == NULL)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid input\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    if(_VDEC_EX_Allocator_IsValidId(pNDecAllocator->u16AllocatorIdMain) == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid u16AllocatorIdMain\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    // If setting total buffer, use it first
    if(pstBufRgn->bSetTotalBuf == TRUE)
    {
        phyMainBlockAddr = pstBufRgn->phyTotalBufAddr;
        szMainBlockSize = pstBufRgn->szTotalBufSize;
    }
    else
    {
        if(pstProperty->bCMA == FALSE)
        {
            phyMainBlockAddr = pstBufRgn->phyBufAddr;
            szMainBlockSize = pstBufRgn->szBufSize;
        }
#ifdef CMA_DRV_DIRECT_INIT
        else  // If using CMA, get CMA phyBufAddr and szBufSize
        {
            VDEC_EX_Result eRet;
            MS_PHY phyCMAStartAddr = 0;
            MS_SIZE szCMAHeapLength = 0;
            MS_PHY phyUnUseSize = 0;
            VDEC_EX_CMA_POOL_SELECT eCMAPoolSelect = _VDEC_EX_NdecAllocator_CMAPoolSelect(eBufType);

            if(eCMAPoolSelect == E_VDEC_EX_CMA_POOL_INVALID)
            {
                ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] eCMAPoolSelect Wrong Buf Type\n", __FUNCTION__, __LINE__);
                goto exit;
            }

            eRet = _VDEC_EX_NdecAllocator_CMAInit(eCMAPoolSelect, pstBufRgn->phyBufAddr, &phyCMAStartAddr, &szCMAHeapLength, &phyUnUseSize);
            if(eRet != E_VDEC_EX_OK)
            {
                ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] CMAInit failed\n", __FUNCTION__, __LINE__);
                goto exit;
            }

            if(pstProperty->bNDecode == FALSE && eBufType != E_VDEC_EX_ALLOCATOR_BUFFER_FB2)
            {
                // If not N Decode, get CMA size by MDrv_HVD_EX_GetCMAMemSize()

                VDEC_EX_CodecType eCodecType = pstProperty->eCodecType;
                VDEC_EX_SrcMode eSrcMode = pstProperty->eSrcMode;
                MS_U64 u64AllocOffset = 0;
                MS_SIZE szAlloclength = 0;

                eRet = MDrv_HVD_EX_GetCMAMemSize(eCodecType, eSrcMode, &u64AllocOffset, &szAlloclength, szCMAHeapLength, phyUnUseSize);
                if(eRet == FALSE)
                {
                    ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] MDrv_HVD_EX_GetCMAMemSize failed\n", __FUNCTION__, __LINE__);
                    goto exit;
                }

                szMainBlockSize = pstBufRgn->szBufSize;
                if(szMainBlockSize == 0 || szMainBlockSize > szAlloclength)
                {
                    szMainBlockSize = szAlloclength;
                }

                phyMainBlockAddr = phyCMAStartAddr - phyUnUseSize;
            }
            else
            {
                // If N Decode or FrameBuffer2, use whole CMA size

                phyMainBlockAddr = phyCMAStartAddr - phyUnUseSize;
                szMainBlockSize = szCMAHeapLength - phyUnUseSize;
            }
        }
#endif
    }

    // initial search criterion
    stCriterion.u32CheckItem =
        VDEC_EX_ALLOCATOR_CRITERION_CHECK_ADDRESS |
        VDEC_EX_ALLOCATOR_CRITERION_CHECK_SIZE;
    stCriterion.phyAddr = phyMainBlockAddr;
    stCriterion.szSize = szMainBlockSize;
    stCriterion.u32Alignment = 0;

    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] stCriterion.phyAddr=0x%llx, szSize=0x%x\n", __FUNCTION__, __LINE__, stCriterion.phyAddr, stCriterion.szSize);

    // start search a valid region
    pstAllocatorMain = _VDEC_EX_Allocator_GetAddressById(pNDecAllocator->u16AllocatorIdMain);
    u16BlockIdMain = _VDEC_EX_Allocator_MallocStart(pstAllocatorMain, &stCriterion);
    if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdMain) == FALSE)
    {
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] MallocStart failed\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    if(pstProperty->bUseSubAllocator == TRUE && pstProperty->bNDecode == TRUE)
    {
        VDEC_EX_Allocator_Block *pstBlockMain = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdMain);
        pstBlockMain->stBlockInfo.u8Property |= VDEC_EX_ALLOCAOTR_PROPERTY_CAN_OVERLAP;
    }

exit:
    return u16BlockIdMain;
}

static MS_U16 _VDEC_EX_NdecAllocator_GetExtendAllocator(VDEC_EX_Allocator_Block *pstBlock)
{
    MS_U16 u16AllocatorIdExt = VDEC_EX_ALLOCAOTR_ID_UNUSED;

    if(pstBlock == NULL)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid Input\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    if(_VDEC_EX_Allocator_IsValidId(pstBlock->u16AllocatorIdExt) == FALSE)
    {
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Create Extend Allocator\n", __FUNCTION__, __LINE__);

        VDEC_EX_Allocator_BlockInfo stBlockInfoRef;
        VDEC_EX_Allocator *pstAllocatorExt;

        u16AllocatorIdExt = _VDEC_EX_Allocator_GetNewId();

        if(_VDEC_EX_Allocator_IsValidId(u16AllocatorIdExt) == FALSE)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Allocator_GetNewId failed\n", __FUNCTION__, __LINE__);
            goto exit;
        }
        pstAllocatorExt = _VDEC_EX_Allocator_GetAddressById(u16AllocatorIdExt);

        stBlockInfoRef.u8MIU        = pstBlock->stBlockInfo.u8MIU;
        stBlockInfoRef.u8Property   = VDEC_EX_ALLOCAOTR_PROPERTY_DEFAULT;
        stBlockInfoRef.u16Allocated = 0;
        stBlockInfoRef.phyAddr      = pstBlock->stBlockInfo.phyAddr;
        stBlockInfoRef.szSize       = pstBlock->stBlockInfo.szSize;

        if(_VDEC_EX_Allocator_Init(pstAllocatorExt, &stBlockInfoRef) != E_VDEC_EX_OK)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Allocator_Init failed\n", __FUNCTION__, __LINE__);
            goto exit;
        }

        pstBlock->u16AllocatorIdExt = u16AllocatorIdExt;
    }

    u16AllocatorIdExt = pstBlock->u16AllocatorIdExt;

exit:
    return u16AllocatorIdExt;
}

static MS_U16 _VDEC_EX_NdecAllocator_GetSubBlock(VDEC_EX_Allocator_Block *pstBlockMain,
    VDEC_EX_ALLOCATOR_BUFFER_TYPE eBufType, VDEC_EX_NDecBufRangeParam *pstBufRgn, VDEC_EX_NDecBufferProperty *pstProperty)
{
    MS_U16 u16BlockIdSub = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    MS_U16 u16AllocatorIdExt = VDEC_EX_ALLOCAOTR_ID_UNUSED;
    VDEC_EX_Allocator *pstAllocatorSub = NULL;

    if(pstBlockMain == NULL)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid input\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    u16AllocatorIdExt = _VDEC_EX_NdecAllocator_GetExtendAllocator(pstBlockMain);
    if(_VDEC_EX_Allocator_IsValidId(u16AllocatorIdExt) == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] GetExtendAllocator failed\n", __FUNCTION__, __LINE__);
        goto exit;
    }
    pstAllocatorSub = _VDEC_EX_Allocator_GetAddressById(u16AllocatorIdExt);

    #if ENABLE_ALLOCATOR_TRACE
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] pstAllocatorSub:\n", __FUNCTION__, __LINE__);
    _VDEC_EX_Allocator_Print(pstAllocatorSub);
    #endif

    if(pstProperty->bNDecode == TRUE)
    {
        VDEC_EX_Allocator_Criterion stCriterion;

        _VDEC_Memset(&stCriterion, 0, sizeof(VDEC_EX_Allocator_Criterion));

        if(pstProperty->bMonopoly == FALSE)
        {
            stCriterion.u32CheckItem =
                VDEC_EX_ALLOCATOR_CRITERION_CHECK_SIZE;
            stCriterion.phyAddr = 0;
            stCriterion.szSize = _VDEC_EX_NdecAllocator_GetTaskSpecSize(pstProperty->eTaskSpec, pstProperty->eCodecType);

            if(eBufType == E_VDEC_EX_ALLOCATOR_BUFFER_FB1)
            {
                // patch for without Dynamic CMA
                stCriterion.szSize = MEGA(32);
            }
            else if(eBufType == E_VDEC_EX_ALLOCATOR_BUFFER_FB2)
            {
                stCriterion.szSize = stCriterion.szSize >> 2;
            }

            stCriterion.u32Alignment = 0x100000;

            ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] bMono=%d, TaskSpec=0x%x, stCriterion.szSize=0x%x\n", __FUNCTION__, __LINE__,
                pstProperty->bMonopoly, pstProperty->eTaskSpec, stCriterion.szSize);
        }
        else
        {
            stCriterion.u32CheckItem =
                VDEC_EX_ALLOCATOR_CRITERION_CHECK_SIZE |
                VDEC_EX_ALLOCATOR_CRITERION_CHECK_ADDRESS;
            stCriterion.phyAddr = pstBufRgn->phyBufAddr;
            stCriterion.szSize = pstBufRgn->szBufSize;
            stCriterion.u32Alignment = 0;

            ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] bMono=%d, stCriterion.phyBufAddr=0x%llx, stCriterion.szSize=0x%x\n", __FUNCTION__, __LINE__,
                pstProperty->bMonopoly, stCriterion.phyAddr, stCriterion.szSize);
        }

        u16BlockIdSub = _VDEC_EX_Allocator_MallocStart(pstAllocatorSub, &stCriterion);
        if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdSub) == FALSE)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][DBG][%s][%d] MallocStart failed\n", __FUNCTION__, __LINE__);
            goto exit;
        }
    }
    else
    {
        u16BlockIdSub = pstAllocatorSub->stBlockList.u16BlockIdHead;
    }

    #if ENABLE_ALLOCATOR_TRACE
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] pstAllocatorSub:\n", __FUNCTION__, __LINE__);
    _VDEC_EX_Allocator_Print(pstAllocatorSub);
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] u16BlockIdTask = %d\n", __FUNCTION__, __LINE__, u16BlockIdSub);
    #endif

exit:
    return u16BlockIdSub;
}

static MS_U16 _VDEC_EX_NdecAllocator_GetThirdBlock(VDEC_EX_Allocator_Block *pstBlockSub)
{
    MS_U16 u16BlockIdThird = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    MS_U16 u16AllocatorIdExt = VDEC_EX_ALLOCAOTR_ID_UNUSED;
    VDEC_EX_Allocator *pstAllocatorThird = NULL;

    if(pstBlockSub == NULL)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid input\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    u16AllocatorIdExt = _VDEC_EX_NdecAllocator_GetExtendAllocator(pstBlockSub);
    if(_VDEC_EX_Allocator_IsValidId(u16AllocatorIdExt) == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] GetExtendAllocator failed\n", __FUNCTION__, __LINE__);
        goto exit;
    }
    pstAllocatorThird = _VDEC_EX_Allocator_GetAddressById(u16AllocatorIdExt);

    u16BlockIdThird = pstAllocatorThird->stBlockList.u16BlockIdHead;

exit:
    return u16BlockIdThird;
}

static VDEC_EX_Result _VDEC_EX_NdecAllocator_GetBitstreamBufferTotalAddrSize(MS_PHY *pphyBSTotalAddrStart, MS_SIZE *pszBSToltalSize)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    VDEC_EX_NDecAllocator *pstNDecAllocator = &(pVdecExContext->stNDecAllocator);
    MS_PHY phyBSTotalAddrStart = 0;
    MS_PHY phyBSTotalAddrEnd = 0;
    MS_SIZE szBSToltalSize = 0;
    MS_BOOL bFind = FALSE;
    int i;

    if(pphyBSTotalAddrStart == NULL || pszBSToltalSize == NULL)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid input", __FUNCTION__, __LINE__);
        goto exit;
    }

    // search BS Buffer record
    for(i = 0; i < VDEC_EX_ALLOCAOTR_MAX_NUM_STREAM; i++)
    {
        MS_PHY phySizeDiff;
        VDEC_EX_Allocator_Block *pstBlockBS;
        MS_U16 u16BlockIdBS1Main = pstNDecAllocator->stAllocatorRecord[i].u16BlockIdBS1Main;
        MS_U16 u16BlockIdBS1Sub  = pstNDecAllocator->stAllocatorRecord[i].u16BlockIdBS1Sub;

        //ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] i=%d, IdBS1Main=%d, IdBS1Sub=%d", __FUNCTION__, __LINE__, i, u16BlockIdBS1Main, u16BlockIdBS1Sub);

        if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdBS1Main) == FALSE &&
           _VDEC_EX_Allocator_BlockIsValidId(u16BlockIdBS1Sub) == FALSE)
        {
            continue;
        }

        if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdBS1Sub) == TRUE)
        {
            pstBlockBS = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdBS1Sub);
        }
        else
        {
            pstBlockBS = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdBS1Main);
        }

        if(bFind == FALSE)
        {
            bFind = TRUE;
            phyBSTotalAddrStart = pstBlockBS->stBlockInfo.phyAddr;
            phyBSTotalAddrEnd = pstBlockBS->stBlockInfo.phyAddr + pstBlockBS->stBlockInfo.szSize;
        }
        else
        {
            if(phyBSTotalAddrStart > pstBlockBS->stBlockInfo.phyAddr)
            {
                ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Warnning: phyBSTotalAddrStart Changed !!", __FUNCTION__, __LINE__);
                phyBSTotalAddrStart = pstBlockBS->stBlockInfo.phyAddr;
            }
            if(phyBSTotalAddrEnd < (pstBlockBS->stBlockInfo.phyAddr + pstBlockBS->stBlockInfo.szSize))
            {
                phyBSTotalAddrEnd = pstBlockBS->stBlockInfo.phyAddr + pstBlockBS->stBlockInfo.szSize;
            }
        }

        phySizeDiff = phyBSTotalAddrEnd - phyBSTotalAddrStart;

        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] start=0x%llx, end=0x%llx, diff=0x%llx", __FUNCTION__, __LINE__,
            phyBSTotalAddrStart, phyBSTotalAddrEnd, phySizeDiff);

        if(sizeof(MS_PHY) == 8 && (phySizeDiff >> 32))
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Fail: phySizeDiff may be overflow !!", __FUNCTION__, __LINE__);
            //goto exit;
        }
        szBSToltalSize = (MS_SIZE)phySizeDiff;

        //ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] i=%d, phyBSTotalAddrStart=0x%llx, szBSToltalSize=0x%x\n",
        //    __FUNCTION__, __LINE__, i, phyBSTotalAddrStart, szBSToltalSize);
    }

    if(bFind == TRUE)
    {
        // output result
        *pphyBSTotalAddrStart = phyBSTotalAddrStart;
        *pszBSToltalSize = szBSToltalSize;

        // all success
        eRet = E_VDEC_EX_OK;

        //ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] phyBSTotalAddrStart=0x%llx, szBSToltalSize=0x%x\n",
        //    __FUNCTION__, __LINE__, phyBSTotalAddrStart, szBSToltalSize);
    }

exit:
    return eRet;
}

static VDEC_EX_Result _VDEC_EX_NdecAllocator_MallocBitstreamBuffer(MS_U8 u8Idx,
    VDEC_EX_ALLOCATOR_BUFFER_TYPE eBufType, VDEC_EX_NDecBufRangeParam *pstBufRgn, VDEC_EX_Allocator_Block **ppstBlockResult)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    VDEC_EX_NDecAllocator *pstNDecAllocator = &(pVdecExContext->stNDecAllocator);
    VDEC_EX_NDecBufferProperty stBufProperty;
    VDEC_EX_Allocator_Block *pstBlockTask = NULL;
    VDEC_EX_Allocator_Block *pstBlockMain = NULL;
    VDEC_EX_Allocator_Block *pstBlockSub  = NULL;
    MS_U16 u16BlockIdTask = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    MS_U16 u16BlockIdMain = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    MS_U16 u16BlockIdSub  = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;

    // initial N Decode Allocator
    if(_VDEC_EX_NDecAllocator_Init(pstNDecAllocator) != E_VDEC_EX_OK)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] NDecAllocator_Init failed\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    #if ENABLE_ALLOCATOR_TRACE
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] After NDecAllocator_Init, pstAllocatorMain:\n", __FUNCTION__, __LINE__);
    VDEC_EX_Allocator *pstAllocatorMain = _VDEC_EX_Allocator_GetAddressById(pstNDecAllocator->u16AllocatorIdMain);
    _VDEC_EX_Allocator_Print(pstAllocatorMain);
    #endif

    if(_VDEC_EX_NdecAllocator_GetBitstreamBufferProperty(u8Idx, &stBufProperty) != E_VDEC_EX_OK)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] GetBitstreamBufferProperty failed\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    #if ENABLE_ALLOCATOR_DEBUG
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] After GetBitstreamBufferProperty, stBufProperty:\n", __FUNCTION__, __LINE__);
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG]\tbNDec=%d, bSubAlloc=%d\n", stBufProperty.bNDecode, stBufProperty.bUseSubAllocator);
    #endif

    u16BlockIdMain = _VDEC_EX_NdecAllocator_GetMainBlock(pstNDecAllocator, eBufType, pstBufRgn, &stBufProperty);
    if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdMain) == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] GetMainBlock failed\n", __FUNCTION__, __LINE__);
        goto exit;
    }
    pstBlockMain = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdMain);

    #if ENABLE_ALLOCATOR_TRACE
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] After GetMainBlock, pstAllocatorMain:\n", __FUNCTION__, __LINE__);
    _VDEC_EX_Allocator_Print(pstAllocatorMain);
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] u16BlockIdMain = %d\n", __FUNCTION__, __LINE__, u16BlockIdMain);
    #endif

    if(stBufProperty.bUseSubAllocator == FALSE)
    {
        u16BlockIdTask = u16BlockIdMain;
        pstBlockTask = pstBlockMain;
    }
    else
    {
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Start GetSubBlock\n", __FUNCTION__, __LINE__);

        u16BlockIdSub = _VDEC_EX_NdecAllocator_GetSubBlock(pstBlockMain, eBufType, pstBufRgn, &stBufProperty);
        if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdSub) == FALSE)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] GetSubBlock failed\n", __FUNCTION__, __LINE__);
            goto exit;
        }
        pstBlockSub = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdSub);

        u16BlockIdTask = u16BlockIdSub;
        pstBlockTask = pstBlockSub;

        #if ENABLE_ALLOCATOR_TRACE
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] pstAllocatorSub:\n", __FUNCTION__, __LINE__);
        if(_VDEC_EX_Allocator_IsValidId(pstBlockMain->u16AllocatorIdExt) == TRUE)
        {
            VDEC_EX_Allocator *pstAllocatorSub = _VDEC_EX_Allocator_GetAddressById(pstBlockMain->u16AllocatorIdExt);
            _VDEC_EX_Allocator_Print(pstAllocatorSub);
        }
        #endif
    }

    // record
    if(pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdBS1Main != VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED ||
       pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdBS1Sub  != VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] BS: stAllocatorRecord[%d] in used\n", __FUNCTION__, __LINE__, u8Idx);
        goto exit;
    }
    pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdBS1Main = u16BlockIdMain;
    pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdBS1Sub  = u16BlockIdSub;

    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] BS: stAllocatorRecord[%d]:\n", __FUNCTION__, __LINE__, u8Idx);
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG]\tIdBS1Main=%d, IdBS1Sub=%d\n", u16BlockIdMain, u16BlockIdSub);

    // output result
    *ppstBlockResult = pstBlockTask;

    // all success
    eRet = E_VDEC_EX_OK;

    #if ENABLE_ALLOCATOR_TRACE
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] BS pstAllocatorMain:\n", __FUNCTION__, __LINE__);
    _VDEC_EX_Allocator_Print(pstAllocatorMain);
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] BS pstAllocatorSub:\n", __FUNCTION__, __LINE__);
    if(_VDEC_EX_Allocator_IsValidId(pstBlockMain->u16AllocatorIdExt) == TRUE)
    {
        VDEC_EX_Allocator *pstAllocatorSub = _VDEC_EX_Allocator_GetAddressById(pstBlockMain->u16AllocatorIdExt);
        _VDEC_EX_Allocator_Print(pstAllocatorSub);
    }
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] BS *ppstBlockResult:\n", __FUNCTION__, __LINE__);
    _VDEC_EX_Allocator_BlockPrint(*ppstBlockResult);
    #endif

exit:
    return eRet;
}

static VDEC_EX_Result _VDEC_EX_NdecAllocator_MallocFrameBuffer(MS_U8 u8Idx,
    VDEC_EX_ALLOCATOR_BUFFER_TYPE eBufType, VDEC_EX_NDecBufRangeParam *pstBufRgn, VDEC_EX_Allocator_Block **ppstBlockResult)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    VDEC_EX_NDecAllocator *pstNDecAllocator = &(pVdecExContext->stNDecAllocator);
    VDEC_EX_NDecBufferProperty stBufProperty;
    VDEC_EX_Allocator_Block *pstBlockTask = NULL;
    VDEC_EX_Allocator_Block *pstBlockMain = NULL;
    VDEC_EX_Allocator_Block *pstBlockSub  = NULL;
    VDEC_EX_Allocator_Block *pstBlockThird= NULL;
    //VDEC_EX_Allocator *pstAllocatorMain = NULL;
    //VDEC_EX_Allocator *pstAllocatorSub  = NULL;
    MS_U16 u16BlockIdTask = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    MS_U16 u16BlockIdMain = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    MS_U16 u16BlockIdSub  = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    MS_U16 u16BlockIdThird= VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;

    // initial N Decode Allocator
    if(_VDEC_EX_NDecAllocator_Init(pstNDecAllocator) != E_VDEC_EX_OK)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] NDecAllocator_Init failed\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    #if ENABLE_ALLOCATOR_TRACE
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] After NDecAllocator_Init, pstAllocatorMain:\n", __FUNCTION__, __LINE__);
    VDEC_EX_Allocator *pstAllocatorMain = _VDEC_EX_Allocator_GetAddressById(pstNDecAllocator->u16AllocatorIdMain);
    _VDEC_EX_Allocator_Print(pstAllocatorMain);
    #endif

    _VDEC_EX_NdecAllocator_GetFrameBufferProperty(u8Idx, &stBufProperty);

    #if ENABLE_ALLOCATOR_DEBUG
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] After GetFrameBufferProperty, stBufProperty:\n", __FUNCTION__, __LINE__);
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG]\tbCMA=%d, bDCMA=%d, bNDec=%d, bUseSubAllocator=%d\n",
        stBufProperty.bCMA,
        stBufProperty.bDynamicCMA,
        stBufProperty.bNDecode,
        stBufProperty.bUseSubAllocator);
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG]\tbUseThirdAllocator=%d, eSrcMode=%d, eCodecType=%d\n",
        stBufProperty.bUseThirdAllocator,
        stBufProperty.eSrcMode,
        stBufProperty.eCodecType);
    #endif

    u16BlockIdMain = _VDEC_EX_NdecAllocator_GetMainBlock(pstNDecAllocator, eBufType, pstBufRgn, &stBufProperty);
    if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdMain) == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] GetMainBlock failed\n", __FUNCTION__, __LINE__);
        goto exit;
    }
    pstBlockMain = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdMain);

    #if ENABLE_ALLOCATOR_TRACE
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] After GetMainBlock, pstAllocatorMain:\n", __FUNCTION__, __LINE__);
    _VDEC_EX_Allocator_Print(pstAllocatorMain);
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] u16BlockIdMain = %d\n", __FUNCTION__, __LINE__, u16BlockIdMain);
    #endif

    if(stBufProperty.bUseSubAllocator == FALSE)
    {
        u16BlockIdTask = u16BlockIdMain;
        pstBlockTask = pstBlockMain;
    }
    else
    {
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Start GetSubBlock\n", __FUNCTION__, __LINE__);

        u16BlockIdSub = _VDEC_EX_NdecAllocator_GetSubBlock(pstBlockMain, eBufType, pstBufRgn, &stBufProperty);
        if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdSub) == FALSE)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] GetSubBlock failed\n", __FUNCTION__, __LINE__);
            goto exit;
        }
        pstBlockSub = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdSub);

        #if ENABLE_ALLOCATOR_TRACE
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] pstAllocatorSub:\n", __FUNCTION__, __LINE__);
        if(_VDEC_EX_Allocator_IsValidId(pstBlockMain->u16AllocatorIdExt) == TRUE)
        {
            VDEC_EX_Allocator *pstAllocatorSub = _VDEC_EX_Allocator_GetAddressById(pstBlockMain->u16AllocatorIdExt);
            _VDEC_EX_Allocator_Print(pstAllocatorSub);
        }
        #endif

        if(stBufProperty.bUseThirdAllocator == FALSE)
        {
            u16BlockIdTask = u16BlockIdSub;
            pstBlockTask = pstBlockSub;
        }
        else
        {
            ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Start GetThirdBlock\n", __FUNCTION__, __LINE__);

            u16BlockIdThird = _VDEC_EX_NdecAllocator_GetThirdBlock(pstBlockSub);
            if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdThird) == FALSE)
            {
                ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] NdecAllocator_GetThirdBlock failed\n", __FUNCTION__, __LINE__);
                goto exit;
            }
            pstBlockThird = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdThird);

            u16BlockIdTask = u16BlockIdThird;
            pstBlockTask = pstBlockThird;

            #if ENABLE_ALLOCATOR_TRACE
            ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] pstAllocatorThird:\n", __FUNCTION__, __LINE__);
            if(_VDEC_EX_Allocator_IsValidId(pstBlockSub->u16AllocatorIdExt) == TRUE)
            {
                VDEC_EX_Allocator *pstAllocatorThird = _VDEC_EX_Allocator_GetAddressById(pstBlockSub->u16AllocatorIdExt);
                _VDEC_EX_Allocator_Print(pstAllocatorThird);
            }
            #endif
        }
    }

#ifdef CMA_DRV_DIRECT_INIT
    if(stBufProperty.bCMA == TRUE && stBufProperty.bDynamicCMA == FALSE)
    {
        VDEC_EX_CMA_POOL_SELECT eCMAPoolSelect = _VDEC_EX_NdecAllocator_CMAPoolSelect(eBufType);

        if(eCMAPoolSelect == E_VDEC_EX_CMA_POOL_INVALID)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] eCMAPoolSelect Wrong Buf Type\n", __FUNCTION__, __LINE__);
            goto exit;
        }

        if(_VDEC_EX_NdecAllocator_CMAAlloc(eCMAPoolSelect, pstBlockTask->stBlockInfo.phyAddr, pstBlockTask->stBlockInfo.szSize) != E_VDEC_EX_OK)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] CMAAlloc failed\n", __FUNCTION__, __LINE__);
            goto exit;
        }
        pstBlockTask->stBlockInfo.u8Property |= VDEC_EX_ALLOCAOTR_PROPERTY_USECMA;
    }
#endif

    // record
    if(eBufType != E_VDEC_EX_ALLOCATOR_BUFFER_FB2)
    {
        if(pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB1Main != VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED ||
           pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB1Sub  != VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] FB: stAllocatorRecord[%d] in used\n", __FUNCTION__, __LINE__, u8Idx);
            goto exit;
        }
        pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB1Main = u16BlockIdMain;
        pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB1Sub  = u16BlockIdSub;
        if((stBufProperty.bNDecode == FALSE) && (stBufProperty.bDynamicCMA == TRUE))
        {
            ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Not NDec + DCMA, Not save SB:\n", __FUNCTION__, __LINE__);
            pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB1Sub  = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
        }

        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] FB: stAllocatorRecord[%d]:\n", __FUNCTION__, __LINE__, u8Idx);
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG]\tIdFB1Main=%d, IdFB1Sub=%d\n",
            pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB1Main,
            pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB1Sub);
    }
    else
    {
        if(pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB2Main != VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED ||
           pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB2Sub  != VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] FB2: stAllocatorRecord[%d] in used\n", __FUNCTION__, __LINE__, u8Idx);
            goto exit;
        }
        pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB2Main = u16BlockIdMain;
        pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB2Sub  = u16BlockIdSub;
        if((stBufProperty.bNDecode == FALSE) && (stBufProperty.bDynamicCMA == TRUE))
        {
            ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Not NDec + DCMA, Not save SB:\n", __FUNCTION__, __LINE__);
            pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB2Sub  = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
        }

        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] FB2: stAllocatorRecord[%d]:\n", __FUNCTION__, __LINE__, u8Idx);
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG]\tIdFB2Main=%d, IdFB2Sub=%d\n",
            pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB2Main,
            pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB2Sub);
    }

    // output result
    *ppstBlockResult = pstBlockTask;

    // all success
    eRet = E_VDEC_EX_OK;

    #if ENABLE_ALLOCATOR_TRACE
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] FB pstAllocatorMain:\n", __FUNCTION__, __LINE__);
    _VDEC_EX_Allocator_Print(pstAllocatorMain);
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] FB pstAllocatorSub:\n", __FUNCTION__, __LINE__);
    if(_VDEC_EX_Allocator_IsValidId(pstBlockMain->u16AllocatorIdExt) == TRUE)
    {
        VDEC_EX_Allocator *pstAllocatorSub = _VDEC_EX_Allocator_GetAddressById(pstBlockMain->u16AllocatorIdExt);
        _VDEC_EX_Allocator_Print(pstAllocatorSub);
    }
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] FB *ppstBlockResult:\n", __FUNCTION__, __LINE__);
    _VDEC_EX_Allocator_BlockPrint(*ppstBlockResult);
    #endif

exit:
    return eRet;
}

static VDEC_EX_Result _VDEC_EX_NdecAllocator_FreeBlock(VDEC_EX_Allocator *pstAllocatorFree, MS_U16 u16BlockIdFree, VDEC_EX_CMA_POOL_SELECT eCMAPoolSelect)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    VDEC_EX_Allocator_Block *pstBlockFree;

    if(pstAllocatorFree == NULL ||
       _VDEC_EX_Allocator_BlockIsValidId(u16BlockIdFree) == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invalid input\n", __FUNCTION__, __LINE__);
        goto exit;
    }
    pstBlockFree = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdFree);

    if(_VDEC_EX_Allocator_IsValidId(pstBlockFree->u16AllocatorIdExt) == TRUE)
    {
        VDEC_EX_Allocator *pstAllocatorExt = _VDEC_EX_Allocator_GetAddressById(pstBlockFree->u16AllocatorIdExt);
        VDEC_EX_Allocator_Block *pstBlockExt;
        MS_U16 u16BlockIdExt;

        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Start Free pstAllocatorExt\n", __FUNCTION__, __LINE__);

        u16BlockIdExt = pstAllocatorExt->stBlockList.u16BlockIdHead;
        while(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdExt) == TRUE)
        {
            pstBlockExt = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdExt);

            #if ENABLE_ALLOCATOR_TRACE
            ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] pstBlockExt:\n", __FUNCTION__, __LINE__);
            _VDEC_EX_Allocator_BlockPrint(pstBlockExt);
            #endif

            if(pstBlockExt->stBlockInfo.u16Allocated == 1)
            {
#ifdef CMA_DRV_DIRECT_INIT
                // free CMA
                if(pstBlockExt->stBlockInfo.u8Property & VDEC_EX_ALLOCAOTR_PROPERTY_USECMA)
                {
                    if(_VDEC_EX_NdecAllocator_CMAFree(eCMAPoolSelect, pstBlockExt->stBlockInfo.phyAddr, pstBlockExt->stBlockInfo.szSize) != E_VDEC_EX_OK)
                    {
                        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] NdecAllocator_CMAFree failed\n", __FUNCTION__, __LINE__);
                        goto exit;
                    }
                    pstBlockExt->stBlockInfo.u8Property &= ~VDEC_EX_ALLOCAOTR_PROPERTY_USECMA;
                }
#endif
                // free block
                u16BlockIdExt = _VDEC_EX_Allocator_Free(pstAllocatorExt, u16BlockIdExt);
                if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdExt) == FALSE)
                {
                    ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Allocator_Free failed\n", __FUNCTION__, __LINE__);
                    goto exit;
                }
            }

            // if already free all blocks
            if(pstAllocatorExt->szUnusedSize == pstAllocatorExt->szTotalSize)
            {
                break;
            }
            pstBlockExt = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdExt);
            u16BlockIdExt = pstBlockExt->u16BlockIdNext;
        }

        // remove extend allocator
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] u16BlockIdExt=%d\n", __FUNCTION__, __LINE__, u16BlockIdExt);
        if(u16BlockIdExt != pstAllocatorExt->stBlockList.u16BlockIdHead)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Should not run here\n", __FUNCTION__, __LINE__);
            goto exit;
        }

        if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdExt) == FALSE)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] remove extend allocator failed\n", __FUNCTION__, __LINE__);
            goto exit;
        }
        pstBlockExt = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdExt);

        pstBlockExt->u16BlockId = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
        pstAllocatorExt->u16AllocatorId = VDEC_EX_ALLOCAOTR_ID_UNUSED;
        pstBlockFree->u16AllocatorIdExt = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
    }

#ifdef CMA_DRV_DIRECT_INIT
    // free CMA
    if(pstBlockFree->stBlockInfo.u8Property & VDEC_EX_ALLOCAOTR_PROPERTY_USECMA)
    {
        if(_VDEC_EX_NdecAllocator_CMAFree(eCMAPoolSelect, pstBlockFree->stBlockInfo.phyAddr, pstBlockFree->stBlockInfo.szSize) != E_VDEC_EX_OK)
        {
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] NdecAllocator_CMAFree failed\n", __FUNCTION__, __LINE__);
            goto exit;
        }
        pstBlockFree->stBlockInfo.u8Property &= ~VDEC_EX_ALLOCAOTR_PROPERTY_USECMA;
    }
#endif

    // free block
    u16BlockIdFree = _VDEC_EX_Allocator_Free(pstAllocatorFree, u16BlockIdFree);
    if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdFree) == FALSE)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Allocator_Free failed\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    // all success
    eRet = E_VDEC_EX_OK;
exit:
    return eRet;
}

static VDEC_EX_Result _VDEC_EX_NdecAllocator_FreeBuffer(MS_U8 u8Idx, VDEC_EX_ALLOCATOR_BUFFER_TYPE eBufType)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    VDEC_EX_NDecAllocator *pstNDecAllocator = &(pVdecExContext->stNDecAllocator);
    VDEC_EX_Allocator *pstAllocatorMain;
    MS_U16 u16BlockIdMain;
    MS_U16 u16BlockIdSub;
    VDEC_EX_CMA_POOL_SELECT eCMAPoolSelect = E_VDEC_EX_CMA_POOL_BUFFER_FB1;  // initial

    switch(eBufType)
    {
    case E_VDEC_EX_ALLOCATOR_BUFFER_FB1:
        eCMAPoolSelect = E_VDEC_EX_CMA_POOL_BUFFER_FB1;
        u16BlockIdMain = pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB1Main;
        u16BlockIdSub  = pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB1Sub;
        pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB1Main = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
        pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB1Sub = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
#ifdef CMA_DRV_DIRECT_INIT
        pstNDecAllocator->szCMAAllocLength[eCMAPoolSelect][u8Idx] = 0;
#endif
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Free FB1 Start, MB=%d, SB=%d\n", __FUNCTION__, __LINE__, u16BlockIdMain, u16BlockIdSub);
        break;
    case E_VDEC_EX_ALLOCATOR_BUFFER_FB2:
        eCMAPoolSelect = E_VDEC_EX_CMA_POOL_BUFFER_FB2;
        u16BlockIdMain = pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB2Main;
        u16BlockIdSub  = pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB2Sub;
        pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB2Main = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
        pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB2Sub = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
#ifdef CMA_DRV_DIRECT_INIT
        pstNDecAllocator->szCMAAllocLength[eCMAPoolSelect][u8Idx] = 0;
#endif
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Free FB2 Start, MB=%d, SB=%d\n", __FUNCTION__, __LINE__, u16BlockIdMain, u16BlockIdSub);
        break;
    case E_VDEC_EX_ALLOCATOR_BUFFER_BS:
        u16BlockIdMain = pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdBS1Main;
        u16BlockIdSub  = pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdBS1Sub;
        pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdBS1Main = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
        pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdBS1Sub = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Free BS Start, MB=%d, SB=%d\n", __FUNCTION__, __LINE__, u16BlockIdMain, u16BlockIdSub);
        break;
    default:
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Input type not define\n", __FUNCTION__, __LINE__);
        goto exit;
        break;
    }

    pstAllocatorMain = _VDEC_EX_Allocator_GetAddressById(pstNDecAllocator->u16AllocatorIdMain);

    if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdMain) == TRUE)
    {
        VDEC_EX_Allocator_Block *pstBlockMain = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdMain);

        // Case of Dynamic CMA using in original Main stream
        if((pstBlockMain->u16AllocatorIdExt != VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED) &&
           (_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdSub) == FALSE))
        {
            #if ENABLE_ALLOCATOR_TRACE
            ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Before FreeBuffer, pstAllocatorMain:\n", __FUNCTION__, __LINE__);
            _VDEC_EX_Allocator_Print(pstAllocatorMain);
            #endif

            if(_VDEC_EX_NdecAllocator_FreeBlock(pstAllocatorMain, u16BlockIdMain, eCMAPoolSelect) != E_VDEC_EX_OK)
            {
                ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] NdecAllocator_FreeBlock failed\n", __FUNCTION__, __LINE__);
                goto exit;
            }
        }
        else
        {
            if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdSub) == TRUE)
            {
                VDEC_EX_Allocator *pstAllocatorSub = _VDEC_EX_Allocator_GetAddressById(pstBlockMain->u16AllocatorIdExt);

                #if ENABLE_ALLOCATOR_TRACE
                ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Before Free pstAllocatorSub block %d:\n", __FUNCTION__, __LINE__, u16BlockIdSub);
                _VDEC_EX_Allocator_Print(pstAllocatorSub);
                #endif

                if(_VDEC_EX_NdecAllocator_FreeBlock(pstAllocatorSub, u16BlockIdSub, eCMAPoolSelect) != E_VDEC_EX_OK)
                {
                    ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] NdecAllocator_FreeBlock failed\n", __FUNCTION__, __LINE__);
                    goto exit;
                }

                #if ENABLE_ALLOCATOR_TRACE
                ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] After Free pstAllocatorSub block %d:\n", __FUNCTION__, __LINE__, u16BlockIdSub);
                _VDEC_EX_Allocator_Print(pstAllocatorSub);
                #endif

                // check remove sub allocator
                if(pstAllocatorSub->szUnusedSize == pstAllocatorSub->szTotalSize)
                {
                    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Start Remove pstAllocatorSub\n", __FUNCTION__, __LINE__);

                    VDEC_EX_Allocator_Block *pstBlock = _VDEC_EX_Allocator_BlockGetAddressById(pstAllocatorSub->stBlockList.u16BlockIdHead);
                    pstBlock->u16BlockId = VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED;
                    pstAllocatorSub->u16AllocatorId = VDEC_EX_ALLOCAOTR_ID_UNUSED;
                    pstBlockMain->u16AllocatorIdExt = VDEC_EX_ALLOCAOTR_ID_UNUSED;
                    pstBlockMain->stBlockInfo.u8Property &= ~VDEC_EX_ALLOCAOTR_PROPERTY_CAN_OVERLAP;
                }
            }

            if(pstBlockMain->u16AllocatorIdExt == VDEC_EX_ALLOCAOTR_BLOCK_ID_UNUSED)
            {
                #if ENABLE_ALLOCATOR_TRACE
                ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Before FreeBuffer, pstAllocatorMain:\n", __FUNCTION__, __LINE__);
                _VDEC_EX_Allocator_Print(pstAllocatorMain);
                #endif

                if(_VDEC_EX_NdecAllocator_FreeBlock(pstAllocatorMain, u16BlockIdMain, eCMAPoolSelect) != E_VDEC_EX_OK)
                {
                    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] _VDEC_EX_NdecAllocator_FreeBlock fail\n", __FUNCTION__, __LINE__);
                    goto exit;
                }
            }
        }
    }

    #if ENABLE_ALLOCATOR_TRACE
    ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] Final pstAllocatorMain:\n", __FUNCTION__, __LINE__);
    _VDEC_EX_Allocator_Print(pstAllocatorMain);
    #endif
    VPRINTF("[ALLOC][DBG][%s][%d] Used Allocators:%d, Used Blocks:%d\n", __FUNCTION__, __LINE__,
        _VDEC_EX_Allocator_CountUsedAllocators(pstNDecAllocator),
        _VDEC_EX_Allocator_CountUsedBlocks(pstNDecAllocator));

    eRet = E_VDEC_EX_OK;

exit:
    return eRet;
}
/*
static VDEC_EX_Result _VDEC_EX_NdecAllocator_GetFrameBufferBlock(MS_U8 u8Idx, VDEC_EX_ALLOCATOR_BUFFER_TYPE eBufType,
    VDEC_EX_Allocator_Block **ppstBlockFB)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    VDEC_EX_NDecAllocator *pstNDecAllocator = &(pVdecExContext->stNDecAllocator);
    VDEC_EX_Allocator_Block *pstBlockTask;
    MS_U16 u16BlockIdTask;
    MS_U16 u16BlockIdMain;
    MS_U16 u16BlockIdSub;

    switch(eBufType)
    {
    case E_VDEC_EX_ALLOCATOR_BUFFER_FB1:
        u16BlockIdMain = pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB1Main;
        u16BlockIdSub  = pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB1Sub;
        break;
    case E_VDEC_EX_ALLOCATOR_BUFFER_FB2:
        u16BlockIdMain = pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB2Main;
        u16BlockIdSub  = pstNDecAllocator->stAllocatorRecord[u8Idx].u16BlockIdFB2Sub;
        break;
    default:
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Input type not support\n", __FUNCTION__, __LINE__);
        break;
    }

    if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdSub) == TRUE)
    {
        u16BlockIdTask = u16BlockIdSub;
    }
    else if(_VDEC_EX_Allocator_BlockIsValidId(u16BlockIdMain) == TRUE)
    {
        u16BlockIdTask = u16BlockIdMain;
    }
    else
    {
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] No valid task block\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    pstBlockTask = _VDEC_EX_Allocator_BlockGetAddressById(u16BlockIdTask);
    *ppstBlockFB = pstBlockTask;

    // all success
    eRet = E_VDEC_EX_OK;

exit:
    return eRet;
}

static VDEC_EX_Result _VDEC_EX_NdecAllocator_GetFrameBufferSize(MS_U8 u8Idx,
    MS_SIZE *pszFrameBuf1, MS_SIZE *pszFrameBuf2)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    VDEC_EX_Allocator_Block *pstBlockFB1 = NULL;
    VDEC_EX_Allocator_Block *pstBlockFB2 = NULL;

    if(pszFrameBuf1 == NULL || pszFrameBuf2 == NULL)
    {
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] Invaild input\n", __FUNCTION__, __LINE__);
        goto exit;
    }

    if(_VDEC_EX_NdecAllocator_GetFrameBufferBlock(u8Idx, E_VDEC_EX_ALLOCATOR_BUFFER_FB1, &pstBlockFB1) == E_VDEC_EX_OK)
    {
        *pszFrameBuf1 = pstBlockFB1->stBlockInfo.szSize;
    }
    else
    {
        *pszFrameBuf1 = 0;
        ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d] No Frame Buf1 Info\n", __FUNCTION__, __LINE__);
    }

    if(_VDEC_EX_NdecAllocator_GetFrameBufferBlock(u8Idx, E_VDEC_EX_ALLOCATOR_BUFFER_FB2, &pstBlockFB2) == E_VDEC_EX_OK)
    {
        *pszFrameBuf2 = pstBlockFB2->stBlockInfo.szSize;
    }
    else
    {
        *pszFrameBuf2 = 0;
        ALLOCATOR_PRINTF_DBG("[ALLOC][DBG][%s][%d] No Frame Buf2 Info\n", __FUNCTION__, __LINE__);
    }

    // all success
    eRet = E_VDEC_EX_OK;

exit:
    return eRet;
}
*/
// N Decode Buffer Allocator Functions End

static void _VDEC_EX_InitDq(MS_U8 u8StreamNum)
{
    MS_U32 i = 0;

    for (i = 0; i < MAX_VDEC_DQNUM; i++)
    {
        pVdecExContext->_stDispQ[u8StreamNum][i].u32Idx = VDEC_DQ_EMPTY;
    }

}

static VDEC_EX_DispFrame *_VDEC_EX_MallocDq(MS_U8 u8StreamNum)
{
    VDEC_EX_DispFrame *pDisp = NULL;
    MS_U32 i = 0;

    if (u8StreamNum >= VDEC_MAX_SUPPORT_STREAM_NUM)
    {
        return NULL;
    }

    for (i = 0; i < MAX_VDEC_DQNUM; i++)
    {
        if (pVdecExContext->_stDispQ[u8StreamNum][i].u32Idx == VDEC_DQ_EMPTY)
        {
            pVdecExContext->_stDispQ[u8StreamNum][i].u32Idx = i;
            pDisp = &(pVdecExContext->_stDispQ[u8StreamNum][i]);
            //VDEC_PRINT("_VDEC_EX_MallocDq %ld\n", i);
            break;
        }
    }

    return pDisp;
}

static void _VDEC_EX_FreeDq(MS_U8 u8StreamNum, VDEC_EX_DispFrame *pDq)
{
    if (pDq && pDq->u32Idx != VDEC_DQ_EMPTY)
    {
        pVdecExContext->_stDispQ[u8StreamNum][pDq->u32Idx].u32Idx = VDEC_DQ_EMPTY;
    }
}

static MS_BOOL _VDEC_EX_IsDqValid(VDEC_EX_DispFrame *pDq)
{
    MS_BOOL bRet = FALSE;

    if (pDq)
    {
        bRet = (pDq->u32Idx == VDEC_DQ_EMPTY);
    }

    return bRet;
}



static void _VDEC_EX_InitLocalVar(MS_U32 u8Idx)
{
    MS_U32 cb_idx = 0;
    VDEC_EX_Attr* pAttr = &(pVdecExContext->_Attr[u8Idx]);
    pAttr->bInit          = FALSE;
    pAttr->eDbgMsgLevel   = VDEC_EX_DEFAULT_DBG_MSG_LEVEL;
    pAttr->eCodecType     = E_VDEC_EX_CODEC_TYPE_NONE;
    pAttr->eDecoder       = E_VDEC_EX_DECODER_NONE;
    pAttr->eErrCode       = E_VDEC_EX_ERR_CODE_BASE;
    pVdecExContext->u8enable_store_set_cmd[u8Idx] = FALSE;
    pVdecExContext->store_set_cmd_write_pointer[u8Idx] = 0;

#if (VDEC_EX_ISR_MONITOR)
    MsOS_ClearEvent(s32VdecExEventId[u8Idx], VDEC_U32_MAX);
    pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] = E_VDEC_EX_EVENT_ISR_EVENT_CHANGE;
    pVdecExContext->_u32PreVdecExIsrEventFlag[u8Idx] = E_VDEC_EX_EVENT_ISR_EVENT_CHANGE;
    MsOS_SetEvent(s32VdecExEventId[u8Idx], E_VDEC_EX_EVENT_ISR_EVENT_CHANGE);
#else
    pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] = 0;
#endif

    pVdecExContext->_pVdec_IsrProc[u8Idx] = NULL;
    for (cb_idx=0; cb_idx<E_VDEC_EX_CB_MAX_NUM; cb_idx++)
    {
        VDEC_EX_EventInfo* pEventInfo = &pAttr->vdecExEventInfo[cb_idx];
        pEventInfo->u32EventBitMap = 0;
        pEventInfo->pVdecEventCb = NULL;
        pEventInfo->pParam = NULL;
    }

    //_VDEC_Memset(&pAttr->vdecExInitParam, 0, sizeof(VDEC_EX_InitParam));

    _VDEC_EX_InitDq(u8Idx);
}

static MVD_CodecType _VDEC_EX_Map2MVDCodecType(VDEC_EX_CodecType vdecCodecType)
{
    MVD_CodecType mvdCodecType = E_MVD_CODEC_UNKNOWN;

    switch (vdecCodecType)
    {
        case E_VDEC_EX_CODEC_TYPE_MPEG2:
            mvdCodecType = E_MVD_CODEC_MPEG2;
            break;

        case E_VDEC_EX_CODEC_TYPE_H263:
            mvdCodecType = E_MVD_CODEC_MPEG4_SHORT_VIDEO_HEADER;
            break;

        case E_VDEC_EX_CODEC_TYPE_MPEG4:
        case E_VDEC_EX_CODEC_TYPE_DIVX412:
            mvdCodecType = E_MVD_CODEC_MPEG4;
            break;

        case E_VDEC_EX_CODEC_TYPE_DIVX311:
            mvdCodecType = E_MVD_CODEC_DIVX311;
            break;

        case E_VDEC_EX_CODEC_TYPE_FLV:
            mvdCodecType = E_MVD_CODEC_FLV;
            break;

        case E_VDEC_EX_CODEC_TYPE_VC1_ADV:
            mvdCodecType = E_MVD_CODEC_VC1_ADV;
            break;

        case E_VDEC_EX_CODEC_TYPE_VC1_MAIN:
            mvdCodecType = E_MVD_CODEC_VC1_MAIN;
            break;

        default:
            mvdCodecType = E_MVD_CODEC_UNKNOWN;
            break;
    }

    return mvdCodecType;
}

static HVD_EX_Codec _VDEC_EX_Map2HVDCodecType(VDEC_EX_CodecType vdecCodecType)
{
    HVD_EX_Codec hvdCodecType = E_HVD_EX_NONE;

    switch (vdecCodecType)
    {
        case E_VDEC_EX_CODEC_TYPE_RV8:
        case E_VDEC_EX_CODEC_TYPE_RV9:
            hvdCodecType = E_HVD_EX_RM;
            break;

        case E_VDEC_EX_CODEC_TYPE_H264:
            hvdCodecType = E_HVD_EX_AVC;
            break;

        case E_VDEC_EX_CODEC_TYPE_AVS:
            hvdCodecType = E_HVD_EX_AVS;
            break;

        case E_VDEC_EX_CODEC_TYPE_MVC:
            hvdCodecType = E_HVD_EX_MVC;
            break;

        case E_VDEC_EX_CODEC_TYPE_VP8:
            hvdCodecType = E_HVD_EX_VP8;
            break;

        case E_VDEC_EX_CODEC_TYPE_HEVC:
            hvdCodecType = E_HVD_EX_HEVC;
            break;

        case E_VDEC_EX_CODEC_TYPE_VP9:
            hvdCodecType = E_HVD_EX_VP9;
            break;

        default:
            hvdCodecType = E_HVD_EX_NONE;
            break;
    }

    return hvdCodecType;
}

static MVD_SrcMode _VDEC_EX_Map2MVDSrcMode(VDEC_EX_SrcMode vdecSrcMode)
{
    MVD_SrcMode mvdSrcMode = E_MVD_TS_MODE;

    switch (vdecSrcMode)
    {
        case E_VDEC_EX_SRC_MODE_DTV:
            mvdSrcMode = E_MVD_TS_MODE;
            break;

        case E_VDEC_EX_SRC_MODE_TS_FILE:
            mvdSrcMode = E_MVD_TS_FILE_MODE;
            break;

        case E_VDEC_EX_SRC_MODE_FILE:
            mvdSrcMode = E_MVD_SLQ_TBL_MODE;
            break;

        default:
            mvdSrcMode = E_MVD_TS_MODE;
            break;
    }

    return mvdSrcMode;
}

static VDEC_EX_FrameType _VDEC_EX_MapFrmType2MVD(MVD_PicType vdecPicType)
{
    VDEC_EX_FrameType mvdFrmType = E_VDEC_EX_FRM_TYPE_NUM;

    switch (vdecPicType)
    {
        case E_MVD_PIC_I:
            mvdFrmType = E_VDEC_EX_FRM_TYPE_I;
            break;

        case E_MVD_PIC_P:
            mvdFrmType = E_VDEC_EX_FRM_TYPE_P;
            break;

        case E_MVD_PIC_B:
            mvdFrmType = E_VDEC_EX_FRM_TYPE_B;
            break;

        case E_MVD_PIC_UNKNOWN:
            mvdFrmType = E_VDEC_EX_FRM_TYPE_OTHER;
            break;

        default:
            mvdFrmType = E_VDEC_EX_FRM_TYPE_NUM;
            break;
    }

    return mvdFrmType;
}

static VDEC_EX_FrameType _VDEC_EX_MapFrmType2HVD(HVD_EX_FrmType vdecFrmType)
{
    VDEC_EX_FrameType hvdFrmType = E_VDEC_EX_FRM_TYPE_NUM;

    switch (vdecFrmType)
    {
        case E_HVD_EX_FRM_TYPE_I:
            hvdFrmType = E_VDEC_EX_FRM_TYPE_I;
            break;

        case E_HVD_EX_FRM_TYPE_P:
            hvdFrmType = E_VDEC_EX_FRM_TYPE_P;
            break;

        case E_HVD_EX_FRM_TYPE_B:
            hvdFrmType = E_VDEC_EX_FRM_TYPE_B;
            break;

        case E_HVD_EX_FRM_TYPE_OTHER:
            hvdFrmType = E_VDEC_EX_FRM_TYPE_OTHER;
            break;

        default:
            hvdFrmType = E_VDEC_EX_FRM_TYPE_NUM;
            break;
    }

    return hvdFrmType;
}

static VDEC_EX_FrameType _VDEC_EX_MapFrmType2MJPEG(MJPEG_FrameType vdecFrmType)
{
    VDEC_EX_FrameType mjpegFrmType = E_VDEC_EX_FRM_TYPE_NUM;

    switch (vdecFrmType)
    {
        case E_MJPEG_FRM_TYPE_I:
            mjpegFrmType = E_VDEC_EX_FRM_TYPE_I;
            break;

        case E_MJPEG_FRM_TYPE_OTHER:
            mjpegFrmType = E_VDEC_EX_FRM_TYPE_OTHER;
            break;

        default:
            mjpegFrmType = E_VDEC_EX_FRM_TYPE_NUM;
            break;
    }

    return mjpegFrmType;
}


static VDEC_EX_FieldType _VDEC_EX_MapFieldType2HVD(HVD_EX_FieldType vdecFieldType)
{
    VDEC_EX_FieldType hvdFieldType = E_VDEC_EX_FIELDTYPE_NONE;

    switch (vdecFieldType)
    {
        case E_HVD_EX_FIELDTYPE_NONE:
            hvdFieldType = E_VDEC_EX_FIELDTYPE_NONE;
            break;

        case E_HVD_EX_FIELDTYPE_TOP:
            hvdFieldType = E_VDEC_EX_FIELDTYPE_TOP;
            break;

        case E_HVD_EX_FIELDTYPE_BOTTOM:
            hvdFieldType = E_VDEC_EX_FIELDTYPE_BOTTOM;
            break;

        case E_HVD_EX_FIELDTYPE_BOTH:
            hvdFieldType = E_VDEC_EX_FIELDTYPE_BOTH;
            break;

        default:
            hvdFieldType = E_VDEC_EX_FIELDTYPE_NUM;
            break;
    }

    return hvdFieldType;
}

static void _VDEC_EX_MVDCfgDivX(MS_U32 u32Id, VDEC_EX_CodecType vdecCodecType)
{
    switch (vdecCodecType)
    {
        case E_VDEC_EX_CODEC_TYPE_MPEG4:
            MDrv_MVD_SetDivXCfg(u32Id, 0x01, 0x01);
            break;

        case E_VDEC_EX_CODEC_TYPE_DIVX412:
            MDrv_MVD_SetDivXCfg(u32Id, 0x11, 0x01);
            break;

        case E_VDEC_EX_CODEC_TYPE_DIVX311:
            MDrv_MVD_SetDivXCfg(u32Id, 0x00, 0x00);
            break;

        default:
            break;
    }

    return;
}

MS_BOOL _VDEC_EX_IsIdle(VDEC_StreamId *pStreamId)
{
    MS_BOOL bRet;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);

    _VDEC_INVALID_IDX_RET(u8Idx);

    if (!(pVdecExContext->_Attr[u8Idx].bInit))
    {
        return FALSE;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            bRet = MDrv_MVD_IsIdle(pStreamId->u32Id);
            break;

        case E_VDEC_EX_DECODER_HVD:
            bRet = MDrv_HVD_EX_IsIdle(pStreamId->u32Id);
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            _VDEC_EX_API_MutexLock();
            bRet = MApi_MJPEG_IsIdle(pStreamId->u32Id);
            _VDEC_EX_API_MutexUnlock();
            break;

        default:
            bRet = FALSE;
            break;
    }

    return bRet;
}

static MS_U8 _VDEC_EX_Map2MVDSpeed(VDEC_EX_DispSpeed eSpeed)
{
    MS_U8 u8Speed = 0;
    switch (eSpeed)
    {
        case E_VDEC_EX_DISP_SPEED_32X:
            u8Speed = 32;
            break;

        case E_VDEC_EX_DISP_SPEED_16X:
            u8Speed = 16;
            break;

        case E_VDEC_EX_DISP_SPEED_8X:
            u8Speed = 8;
            break;

        case E_VDEC_EX_DISP_SPEED_4X:
            u8Speed = 4;
            break;

        case E_VDEC_EX_DISP_SPEED_2X:
            u8Speed = 2;
            break;

        case E_VDEC_EX_DISP_SPEED_1X:
            u8Speed = 1;
            break;

        default:
            u8Speed = 0;
            break;
    }
    return u8Speed;
}

//------------------------------------------------------------------------------
/// Enable to let decoder force to display repeat last field
/// @param bEnable \b IN : TRUE - enable; FALSE - disable
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_RepeatLastField(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_DispRepeatField(u32Id, bEnable));
            break;
        case E_VDEC_EX_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_EX_SetDispRepeatField(u32Id, bEnable));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Set the maximum repeat times for one frame when av is not sync.
/// @param -u32Th \b IN : repeat times. 0x01 ~ 0xFF
///                0xff - repeat current frame until STC catch up PTS.
/// @return -The result of command set sync repeat threashold
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_SetAVSyncRepeatThreshold(VDEC_StreamId *pStreamId, MS_U32 u32Th)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            MDrv_MVD_SetAVSyncThreshold(u32Id, u32Th);
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_EX_SetSyncRepeatTH(u32Id, u32Th));
            eRet = E_VDEC_EX_OK;
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Enable FW only show one field.
/// @param bEnable \b IN : TRUE - show one field; FALSE - disable
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_DispOneField(VDEC_StreamId *pStreamId, MS_U32 u32Arg)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_EnableDispOneField(u32Id, (MS_BOOL) u32Arg));
            break;

        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_SetDispOneField(u32Id, (MS_BOOL) u32Arg));
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Set fd mask delay count.
/// @param u32Arg \b IN : unit is in vsync base for muting the fd_mask
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_SetFdMaskDelayCnt(VDEC_StreamId *pStreamId, MS_U32 u32Arg)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetFdMaskDelayCount(u32Id, (MS_U16)u32Arg));
            break;

        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_SetFdMaskDelayCnt(u32Id, (MS_U8)u32Arg));
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return eRet;
}


//------------------------------------------------------------------------------
/// Set frame rate convert output mode
/// @param pFRC \b IN : the address of VDEC_FRC_OutputParam
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_FrameRateConvert(VDEC_StreamId *pStreamId, VDEC_EX_FRC_OutputParam *pFRC)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetOutputFRCMode(u32Id, (MS_U8)(pFRC->u32OutputFrameRate), pFRC->u8Interlace));
            break;

        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_SetOutputFRCMode(u32Id, (MS_U8)(pFRC->u32OutputFrameRate), pFRC->u8Interlace));
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Set ignore error reference
/// @param bIgnore \b IN : turn on / off.
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_SetIgnoreErrRef(VDEC_StreamId *pStreamId, MS_BOOL bIgnore)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_SetIgnoreErrRef(u32Id, bIgnore));
            break;
        case E_VDEC_EX_DECODER_MVD:
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Set ignore error reference
/// @param bIgnore \b IN : turn on / off.
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------

static VDEC_EX_Result _VDEC_EX_SetCcInfoEnhanceMode(VDEC_StreamId *pStreamId,MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if(pVdecExContext->support_cc[u8Idx] == FALSE)
    {
        return E_VDEC_EX_RET_UNSUPPORTED;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            eRet = (MDrv_CC_InfoEnhanceMode(u32Id,bEnable)) ? E_VDEC_EX_OK : E_VDEC_EX_FAIL ;
            break;

        case E_VDEC_EX_DECODER_HVD:
            eRet = (MDrv_HVD_EX_CC_InfoEnhanceMode(u32Id, bEnable)) ? E_VDEC_EX_OK : E_VDEC_EX_FAIL ;
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return eRet;
}


static VDEC_EX_Result _VDEC_EX_ForceFollowDTVSpec(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_ForceFollowDTVSpec(u32Id, bEnable));
            break;
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Set the theshold of H264 frame gap, 0xFFFFFFFF don't care frame gap
/// @param u32FrmGap \b IN : frame gap
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_AvcMinFrameGap(VDEC_StreamId *pStreamId, MS_U32 u32FrmGap)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_SetSettings_Pro(u32Id, E_HVD_EX_SSET_MIN_FRAME_GAP, u32FrmGap));
            break;
        case E_VDEC_EX_DECODER_MVD:
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Set fast display, not caring av-sync
/// @param bFastDisplay \b IN : turn on / off.
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_SetFastDisplay(VDEC_StreamId *pStreamId, MS_BOOL bFastDisplay)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_SetFastDisplay(u32Id, bFastDisplay));
            break;
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Set frame rate convert drop type
/// @param u32DropType \b IN : FRC drop type.
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_SetFRCDropType(VDEC_StreamId *pStreamId, MS_U32 u32DropType)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetFRCDropType(u32Id, (MS_U8)u32DropType));
            break;

        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_SetFRCDropType(u32Id, (MS_U8)u32DropType));
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Set disable resolution change
/// @param bEnable \b IN : turn on / off.
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_SetDisableSeqChange(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetDisableSeqChange(u32Id, bEnable));
            break;
        case E_VDEC_EX_DECODER_HVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Reset MVD parser
/// @param bEnable \b IN : turn on / off.
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_ResetMvdParser(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_ParserRstDone(u32Id, bEnable));
            break;
        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}


//------------------------------------------------------------------------------
/// Force into interlace mode
/// @param u32Mode \b IN : 0: Disable, 1: Enable
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_ForceInterlaceMode(VDEC_StreamId *pStreamId, MS_U32 u32Mode)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_ForceInterlaceMode(u32Id, (MS_U8)u32Mode));
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_IsChroma420(VDEC_StreamId *pStreamId, MS_U32* param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            if (MDrv_MVD_GetChromaFormat(u32Id) <= 1)
            {
                *param = TRUE;
            }
            else
            {
                *param = FALSE;
            }
            eRet = E_VDEC_EX_OK;
            break;

        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MJPEG:
            *param = TRUE;
            eRet = E_VDEC_EX_OK;
            break;

        default:
            *param = FALSE;
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_GetRealFrameRate(VDEC_StreamId *pStreamId, MS_U32 *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
        {
            if (E_HVD_EX_OK == MDrv_HVD_EX_CheckDispInfoRdy(u32Id))
            {
                *param = MDrv_HVD_EX_GetData(u32Id, E_HVD_EX_GDATA_TYPE_REAL_FRAMERATE);
                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_RET_NOT_READY;
            }
            break;
        }
        case E_VDEC_EX_DECODER_MVD:
        {
            if (0 != MDrv_MVD_GetDispRdy(u32Id))
            {
                MVD_FrameInfo info;
                _VDEC_Memset(&info, 0, sizeof(MVD_FrameInfo));
                MDrv_MVD_GetFrameInfo(u32Id, &info);
                *param = info.u32FrameRate;
                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_RET_NOT_READY;
            }
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            if(E_MJPEG_RET_SUCCESS == MApi_MJPEG_CheckDispInfoRdy(u32Id))
            {
                MJPEG_DISP_INFO info;
                _VDEC_Memset(&info, 0, sizeof(MJPEG_DISP_INFO));
                MApi_MJPEG_GetDispInfo(u32Id, &info);
                *param = info.u32FrameRate;
                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_RET_NOT_READY;
            }
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
        {
            eRet = E_VDEC_EX_RET_INVALID_PARAM;
            break;
        }
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_DISP_OUTSIDE_MODE(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    pVdecExContext->_bVdecDispOutSide[u8Idx] = bEnable;
    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetMcuMode(u32Id, bEnable));
#ifdef VDEC3
            if(bEnable == TRUE)
            {
                pVdecExContext->u32FrameBaseMode[u8Idx] = TRUE;
            }
            else
            {
                pVdecExContext->u32FrameBaseMode[u8Idx] = FALSE;
            }
#endif
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_DispOutsideMode(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            _MJPEG_RET_HANDLE(MApi_MJPEG_SetControl(u32Id, E_MJPEG_VDEC_CMD_SET_DISPLAY_OUTSIDE_MODE, (MS_U32)bEnable));
            break;
        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Set DTV User Data packet mode
/// @param u32Mode \b IN : User Data Packet Mode, 0: DVB normal, 1: ATSC DirectTV. default:0.
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_SetDTVUserDataMode(VDEC_StreamId *pStreamId, MS_U32 u32Mode)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_SetDTVUserDataMode(u32Id, (MS_U8)u32Mode));
            eRet = E_VDEC_EX_OK;
            break;

        case E_VDEC_EX_DECODER_MVD:
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Release fd mask
/// To fix the garbage when zooming at slow motion
/// @param bRelease \b IN : 1: update a new frame to release fd mask as slow-motion. default:0.
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_ReleaseFdMask(VDEC_StreamId *pStreamId, MS_BOOL bRelease)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_ReleaseFdMask(u32Id, bRelease));
            break;
        case E_VDEC_EX_DECODER_HVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_SetVSizeAlign(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetVSizeAlign(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;

        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_EnableVSizeAlign(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_ShowDecodeOrder(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_ShowDecodeOrder(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;

        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_ShowDecodeOrder(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Set Disp Ignore Crop : force ignore crop when input steam has SEI info
/// @param bEnable \b IN : turn on / off.
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_Disp_Ignore_Crop(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_Disp_Ignore_Crop(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_SuspendDynamicScale(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SuspendDynamicScale(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;

        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_SuspendDynamicScale(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_RmEnablePtsTbl(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        case E_VDEC_EX_DECODER_HVD:
            if((pVdecExContext->_Attr[u8Idx].eCodecType != E_VDEC_EX_CODEC_TYPE_RV8) && (pVdecExContext->_Attr[u8Idx].eCodecType != E_VDEC_EX_CODEC_TYPE_RV9) )
            {
                return E_VDEC_EX_RET_UNSUPPORTED;
            }

            _HVD_RET_HANDLE(MDrv_HVD_EX_RmEnablePtsTbl(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Force set FW to mute if resolution change
/// @param u32Mode \b IN : 0: Disable(default), 1: Enable
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_ForceAutoMute(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetAutoMute(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;

        case E_VDEC_EX_DECODER_HVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_SetDispFinishMode(VDEC_StreamId *pStreamId, MS_U8 u8Mode)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetDispFinishMode(u32Id, u8Mode));
            eRet = E_VDEC_EX_OK;
            break;

        case E_VDEC_EX_DECODER_HVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_SetAVSyncMode(VDEC_StreamId *pStreamId, MS_U8 u8Mode)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    //MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            //_MVD_RET_HANDLE(MDrv_MVD_SetAVSyncMode(u32Id, u8Mode));
            //eRet = E_VDEC_EX_OK;
            break;

        case E_VDEC_EX_DECODER_HVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Push Disp Q with Ref Number
/// @param u32Mode \b IN : 0: Disable, 1: Enable
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_PushDispQWithRefNum(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_PushDispQWithRefNum(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_SetIdctMode(VDEC_StreamId *pStreamId, MS_U8 u8Mode)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetIdctMode(u32Id, u8Mode));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_HVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Push First
/// @param u32Mode \b IN : 0: Disable, 1: Enable
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_ShowFirstFrameDirect(VDEC_StreamId *pStreamId,MS_U32 u32Mode)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if ((u32Mode != 0) && (u32Mode != 1))
        return eRet;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_ShowFirstFrameDirect(u32Id,u32Mode));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_ShowFirstFrameDirect(u32Id, u32Mode));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Set xc_low_delay parameter
/// @param u32Para \b IN :
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_SetXcLowDelayPara(VDEC_StreamId *pStreamId,MS_U32 u32Para)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        case E_VDEC_EX_DECODER_MVD:
             _MVD_RET_HANDLE(MDrv_MVD_SetXCLowDelayPara(u32Id,u32Para));
             eRet = E_VDEC_EX_OK;
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// @return XC_LOW_DELAY int state
//------------------------------------------------------------------------------
VDEC_EX_Result _VDEC_EX_GetXcLowDelayIntState(VDEC_StreamId *pStreamId,MS_U32* param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            *param = MDrv_MVD_GetXcLowDelayIntState(u32Id);
            eRet = E_VDEC_EX_OK;
            break;

        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            *param = VDEC_EX_XC_LOW_DELAY_NONE;
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_AVCResizeDosDispPendBuf(VDEC_StreamId *pStreamId,MS_U32 u32Size)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);


    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_AVCResizeDosDispPendBuf(u32Id, u32Size));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_IgnorePicOverrun(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_IgnorePicOverrun(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}


static VDEC_EX_Result _VDEC_EX_SetAutoExhaustESMode(VDEC_StreamId *pStreamId, MS_U32 u32ESBound)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_HVD:

            if(pVdecExContext->_Attr[u8Idx].eCodecType != E_VDEC_EX_CODEC_TYPE_H264)
            {
                return E_VDEC_EX_RET_UNSUPPORTED;
            }
            _HVD_RET_HANDLE(MDrv_HVD_EX_AutoExhaustESMode(u32Id, u32ESBound));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}



static VDEC_EX_Result _VDEC_EX_SetMinTspDataSize(VDEC_StreamId *pStreamId, MS_U32 u32Size)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            if(TRUE == MDrv_MVD_SetPrebufferSize(u32Id,u32Size))
            {
               eRet = E_VDEC_EX_OK;
            }
            break;
        case E_VDEC_EX_DECODER_HVD:

            if(pVdecExContext->_Attr[u8Idx].eCodecType != E_VDEC_EX_CODEC_TYPE_H264)
            {
                return E_VDEC_EX_RET_UNSUPPORTED;
            }
            _HVD_RET_HANDLE(MDrv_HVD_EX_SetMinTspSize(u32Id, u32Size));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}


static VDEC_EX_Result _VDEC_EX_CtlSpeedInDispOnly(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_CtlSpeedInDispOnly(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_AVCSupportRefNumOverMaxDBPSize(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_AVCSupportRefNumOverMaxDBPSize(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}


static VDEC_EX_Result _VDEC_EX_ReturnInvalidAFD(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_ReturnInvalidAFD(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_SetDmxFrameRate(VDEC_StreamId *pStreamId, MS_U32 u32Value)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            MDrv_MVD_SetDmxFrameRate(u32Id,u32Value);
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_SetDmxFrameRate(u32Id, u32Value));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_SetDmxFrameRateBase(VDEC_StreamId *pStreamId, MS_U32 u32Value)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            MDrv_MVD_SetDmxFrameRateBase(u32Id,u32Value);
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_SetDmxFrameRateBase(u32Id, u32Value));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_AVCForceBrokenByUs(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        case E_VDEC_EX_DECODER_HVD:
            if(pVdecExContext->_Attr[u8Idx].eCodecType != E_VDEC_EX_CODEC_TYPE_H264)
            {
                return E_VDEC_EX_RET_UNSUPPORTED;
            }

            _HVD_RET_HANDLE(MDrv_HVD_EX_AVCForceBrokenByUs(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return eRet;
}

static HVD_EX_ClockSpeed _VDEC_EX_Map2HVDClockSpeed(VDEC_EX_ClockSpeed ClockSpeed)
{
    HVD_EX_ClockSpeed hvdClockSpeed = E_HVD_EX_CLOCK_SPEED_NONE;

    switch(ClockSpeed)
    {
        case E_VDEC_EX_CLOCK_SPEED_HIGHEST:
            hvdClockSpeed = E_HVD_EX_CLOCK_SPEED_HIGHEST;
            break;
        case E_VDEC_EX_CLOCK_SPEED_HIGH:
            hvdClockSpeed = E_HVD_EX_CLOCK_SPEED_HIGH;
            break;
        case E_VDEC_EX_CLOCK_SPEED_MEDIUM:
            hvdClockSpeed = E_HVD_EX_CLOCK_SPEED_MEDIUM;
            break;
        case E_VDEC_EX_CLOCK_SPEED_LOW:
            hvdClockSpeed = E_HVD_EX_CLOCK_SPEED_LOW;
            break;
        case E_VDEC_EX_CLOCK_SPEED_LOWEST:
            hvdClockSpeed = E_HVD_EX_CLOCK_SPEED_LOWEST;
            break;
        case E_VDEC_EX_CLOCK_SPEED_DEFAULT:
            hvdClockSpeed = E_HVD_EX_CLOCK_SPEED_DEFAULT;
            break;
        default:
            hvdClockSpeed = E_HVD_EX_CLOCK_SPEED_NONE;
            break;
    }

    return hvdClockSpeed;
}

static MVD_EX_ClockSpeed _VDEC_EX_Map2MVDClockSpeed(VDEC_EX_ClockSpeed ClockSpeed)
{
    MVD_EX_ClockSpeed mvdClockSpeed = E_MVD_EX_CLOCK_SPEED_NONE;

    switch(ClockSpeed)
    {
        case E_VDEC_EX_CLOCK_SPEED_HIGHEST:
            mvdClockSpeed = E_MVD_EX_CLOCK_SPEED_HIGHEST;
            break;
        case E_VDEC_EX_CLOCK_SPEED_HIGH:
            mvdClockSpeed = E_MVD_EX_CLOCK_SPEED_HIGH;
            break;
        case E_VDEC_EX_CLOCK_SPEED_MEDIUM:
            mvdClockSpeed = E_MVD_EX_CLOCK_SPEED_MEDIUM;
            break;
        case E_VDEC_EX_CLOCK_SPEED_LOW:
            mvdClockSpeed = E_MVD_EX_CLOCK_SPEED_LOW;
            break;
        case E_VDEC_EX_CLOCK_SPEED_LOWEST:
            mvdClockSpeed = E_MVD_EX_CLOCK_SPEED_LOWEST;
            break;
        case E_VDEC_EX_CLOCK_SPEED_DEFAULT:
            mvdClockSpeed = E_MVD_EX_CLOCK_SPEED_DEFAULT;
            break;
        default:
            mvdClockSpeed = E_MVD_EX_CLOCK_SPEED_NONE;
            break;
    }

    return mvdClockSpeed;
}


static VDEC_EX_Result _VDEC_EX_SetVPUClockSpeed(VDEC_EX_ClockSpeed ClockSpeed)
{
    _HVD_RET_HANDLE(MDrv_HVD_EX_SetVPUClockSpeed(_VDEC_EX_Map2HVDClockSpeed(ClockSpeed)));

    return E_VDEC_EX_OK;
}


static VDEC_EX_Result _VDEC_EX_SetHVDClockSpeed(VDEC_EX_ClockSpeed ClockSpeed)
{
    _HVD_RET_HANDLE(MDrv_HVD_EX_SetHVDClockSpeed(_VDEC_EX_Map2HVDClockSpeed(ClockSpeed)));

    return E_VDEC_EX_OK;
}


static VDEC_EX_Result _VDEC_EX_SetMVDClockSpeed(VDEC_EX_ClockSpeed ClockSpeed)
{
    _MVD_RET_HANDLE(MDrv_MVD_EX_SetMVDClockSpeed(_VDEC_EX_Map2MVDClockSpeed(ClockSpeed)));

    return E_VDEC_EX_OK;
}

static VDEC_EX_Result _VDEC_EX_SetHVDColBBUMode(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);

    if (pVdecExContext->_Attr[u8Idx].bInit)
    {
        return E_VDEC_EX_RET_ILLEGAL_ACCESS;
    }

    _HVD_RET_HANDLE(MDrv_HVD_EX_SetHVDColBBUMode(u32Id, bEnable));
    eRet = E_VDEC_EX_OK;

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_SetSelfSeqChange(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetSelfSeqChange(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_DSReportDispInfoChange(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;

}

//------------------------------------------------------------------------------
/// Enable FW PTS abnormal detect.
/// @param bEnable \b IN : TRUE - show one field; FALSE - disable
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_EnablePTSDetector(VDEC_StreamId *pStreamId, MS_U32 u32Arg)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    _VDEC_INVALID_IDX_RET(u8Idx);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_EnablePTSDetector(u32Id, (MS_BOOL) u32Arg));
            break;

        case E_VDEC_EX_DECODER_HVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_DisablePBFrameMode(VDEC_StreamId * pStreamId, MS_U32 u32Arg)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    _VDEC_INVALID_IDX_RET(u8Idx);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_DisablePBFrameMode(u32Id, (MS_BOOL) u32Arg));
            break;

        case E_VDEC_EX_DECODER_HVD:
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            break;

        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_SetSecureMode(VDEC_StreamId *pStreamId, MS_U32 u32SecureMode)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_SetSecureMode(u32Id, u32SecureMode));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_OK;
            break;
        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Set RVU setting mode : Settings for RVU mode.
/// @param bEnable \u32 IN : 0: Disable, 1: Drop B-frame and force IDR.
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_RVU_Setting_Mode(VDEC_StreamId *pStreamId, MS_U32 u32Param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_RVU_Setting_Mode(u32Id, u32Param));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_FramerateHandling(VDEC_StreamId *pStreamId, MS_U32 u32Framerate)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_FramerateHandling(u32Id, u32Framerate));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_OK;
            break;
        default:
            break;
    }

    return eRet;
}


//------------------------------------------------------------------------------
/// Set _VDEC_EX_DualNonBlockMode
/// @param bEnable \b IN : turn on / off.
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_DualNonBlockMode(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result ret = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_DualNonBlockMode(u32Id, bEnable));
            ret = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MVD:
            ret = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            ret = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return ret;
}

static VDEC_EX_Result _VDEC_EX_ClearProcessRelated(MS_U32 param)
{
    MS_U8  u8Idx = 0;

    if(pVdecExContext == NULL)
    {
        VDEC_PRINT("Err,%s,pVdecExContext is Null\n",__FUNCTION__);
        return E_VDEC_EX_FAIL;
    }

    for(u8Idx = 0; u8Idx < VDEC_MAX_SUPPORT_STREAM_NUM; u8Idx++)
    {
#if 0
        if(pVdecExContext->_Attr[u8Idx].s32VdecExMutexId != -1)
        {
            pVdecExContext->_Attr[u8Idx].s32VdecExMutexId = -1;
        }
#endif
#if (VDEC_EX_ISR_MONITOR)
        if(s32VdecExEventId[u8Idx] != -1)
        {
            s32VdecExEventId[u8Idx] = -1;
        }

        if(s32VdecExTaskId[u8Idx] != -1)
        {
            s32VdecExTaskId[u8Idx] = -1;
        }
#endif
    }

    return E_VDEC_EX_OK;
}



//------------------------------------------------------------------------------
/// Set debug mode
/// @param u32Mode \b IN :
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_DbgMode(VDEC_StreamId *pStreamId, VDEC_EX_DbgMode eDbgMode, MS_BOOL bEn)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            switch (eDbgMode)
            {
                case E_VDEC_EX_DBG_MODE_BYPASS_INSERT_START_CODE:
                    {
                        _MVD_RET_HANDLE(MDrv_MVD_SetDbgMode(u32Id, E_MVD_EX_DBG_MODE_BYPASS_INSERT_START_CODE, bEn));
                        eRet = E_VDEC_EX_OK;
                    }
                    break;
                case E_VDEC_EX_DBG_MODE_BYPASS_DIVX_MC_PATCH:
                    {
                        _MVD_RET_HANDLE(MDrv_MVD_SetDbgMode(u32Id, E_MVD_EX_DBG_MODE_BYPASS_DIVX_MC_PATCH, bEn));
                        eRet = E_VDEC_EX_OK;
                    }
                    break;
                default:
                    eRet = E_VDEC_EX_RET_UNSUPPORTED;
                    break;
            }
            break;
        case E_VDEC_EX_DECODER_HVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_SetSTCMode(VDEC_StreamId *pStreamId, VDEC_EX_STCMode STCmode)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;

    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);

    if (pVdecExContext->_Attr[u8Idx].bInit)
    {
        return E_VDEC_EX_RET_ILLEGAL_ACCESS;
    }
    _HVD_RET_HANDLE(MDrv_HVD_SetSTCMode(u32Id,STCmode));
    eRet = E_VDEC_EX_OK;

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_SetDecodeMode(VDEC_StreamId *pStreamId, VDEC_EX_DecModCfg *pDecModCfg)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    HVD_EX_DecModCfg stHvdDecModCfg;
    MS_U8 i = 0;

    if (pDecModCfg == NULL)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    if(pDecModCfg->eDecMod != E_VDEC_EX_DEC_MODE_DUAL_INDIE)
    {
        for (i=0; i<VDEC_MAX_SUPPORT_STREAM_NUM; i++)
        {
            if (TRUE == pVdecExContext->_Attr[i].bInit)
            {
                //VDEC_PRINT("%s must be set before init all tasks (%d)\n", __FUNCTION__, i);
                return E_VDEC_EX_FAIL;
            }
        }
    }

    switch (pDecModCfg->eDecMod)
    {
        case E_VDEC_EX_DEC_MODE_MVC:  //E_VDEC_EX_DEC_MODE_SINGLE
            if (pDecModCfg->u8ArgSize == sizeof(MS_BOOL))
            {
                MS_BOOL bSingleDecMode = TRUE;
                bSingleDecMode = (MS_BOOL)pDecModCfg->u32Arg;
                eRet = MApi_VDEC_EX_SetSingleDecode(bSingleDecMode);
            }
            break;
        case E_VDEC_EX_DEC_MODE_DUAL_INDIE:
            stHvdDecModCfg.u8DecMod   = (MS_U8)pDecModCfg->eDecMod;
            stHvdDecModCfg.u8CodecCnt = pDecModCfg->u8CodecCnt;
            for (i=0; ((i<pDecModCfg->u8CodecCnt)&&(i<VDEC_MAX_DEC_NUM)); i++)
            {
                stHvdDecModCfg.u8CodecType[i] = (MS_U8)pDecModCfg->pstCodecInfo[i].eCodecType;
            }
            stHvdDecModCfg.u8ArgSize  = pDecModCfg->u8ArgSize;
            stHvdDecModCfg.u32Arg     = pDecModCfg->u32Arg;
            //--> HVD --> VPU
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_HVD_SetDecodeMode(pStreamId->u32Id, &stHvdDecModCfg));
            break;
        case E_VDEC_EX_DEC_MODE_DUAL_3D:
            if (pDecModCfg->u8CodecCnt == 2)
            {
                if ((E_VDEC_EX_CODEC_TYPE_MPEG2 == pDecModCfg->pstCodecInfo[0].eCodecType) &&
                    (E_VDEC_EX_CODEC_TYPE_H264 == pDecModCfg->pstCodecInfo[1].eCodecType))
                {
                    VDEC_PRINT("3D_TV\n");
                }
                else if (((E_VDEC_EX_CODEC_TYPE_VC1_ADV == pDecModCfg->pstCodecInfo[0].eCodecType) &&
                         (E_VDEC_EX_CODEC_TYPE_VC1_ADV == pDecModCfg->pstCodecInfo[1].eCodecType)) ||
                         ((E_VDEC_EX_CODEC_TYPE_VC1_MAIN == pDecModCfg->pstCodecInfo[0].eCodecType) &&
                         (E_VDEC_EX_CODEC_TYPE_VC1_MAIN == pDecModCfg->pstCodecInfo[1].eCodecType)))
                {
                    VDEC_PRINT("3D_WMV\n");
                }

                stHvdDecModCfg.u8DecMod   = (MS_U8)pDecModCfg->eDecMod;
                stHvdDecModCfg.u8CodecCnt = pDecModCfg->u8CodecCnt;
                for (i=0; ((i<pDecModCfg->u8CodecCnt)&&(i<VDEC_MAX_DEC_NUM)); i++)
                {
                    stHvdDecModCfg.u8CodecType[i] = (MS_U8)pDecModCfg->pstCodecInfo[i].eCodecType;
                }
                stHvdDecModCfg.u8ArgSize  = pDecModCfg->u8ArgSize;
                stHvdDecModCfg.u32Arg     = pDecModCfg->u32Arg;
                //--> HVD --> VPU
                _BOOL_TO_VDEC_RESULT(eRet, MDrv_HVD_SetDecodeMode(pStreamId->u32Id, &stHvdDecModCfg));
            }
            else
            {
                VDEC_PRINT("%s invalid u8CodecCnt(%d)\n", __FUNCTION__, pDecModCfg->u8CodecCnt);
                eRet = E_VDEC_EX_RET_INVALID_PARAM;
            }
            break;
        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_FlushPTSBuf(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch(pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_TO_VDEC_RESULT(eRet, MDrv_MVD_FlushPTSBuf(u32Id, bEnable));
            break;
        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

#if (VDEC_EX_ISR_MONITOR)
static MS_BOOL _VDEC_EX_SYS_Init(MS_U8 u8Idx)
{
    VDEC_EX_Attr* pAttr = &(pVdecExContext->_Attr[u8Idx]);
    //MS_S32* ps32VdecExMutexId = &pAttr->s32VdecExMutexId;

    char*   pu8VdecExEvent    = pAttr->stEventMon.pu8VdecExEvent;
    //MS_S32* ps32VdecExTaskId  = &pAttr->stEventMon.s32VdecExTaskId;
    char*   pu8VdecExTask     = pAttr->stEventMon.pu8VdecExTask;

#if 0
    //Create Mutex
    *ps32VdecExMutexId = MsOS_CreateMutex(E_MSOS_FIFO, (char *)pAttr->pu8VdecExMutex, MSOS_PROCESS_SHARED);

    if (*ps32VdecExMutexId < 0)
    {
        VDEC_PRINT("MsOS_CreateMutex failed!!\n");
        return FALSE;
    }
#endif

    if(s32VdecExEventId[u8Idx] == -1)
    {
        //Create Event Group
        s32VdecExEventId[u8Idx] = MsOS_CreateEventGroup(pu8VdecExEvent);

        if (s32VdecExEventId[u8Idx] < 0)
        {
            VDEC_PRINT("MsOS_CreateEventGroup failed!!\n");
            //MsOS_DeleteMutex(*ps32VdecExMutexId);
            return FALSE;
        }
    }

    if(s32VdecExTaskId[u8Idx] == -1)
    {
        //Create Vdec event monitor Task
        pAttr->stEventMon.bVdecExIsrMntr = TRUE;
        //create corresponding tasks to monitor ISR for each task
        //task0: _VDEC_EX_MonitorISR_Proc; task1: _VDEC_EX_MonitorSubISR_Proc
        s32VdecExTaskId[u8Idx] = MsOS_CreateTask((TaskEntry) pAttr->stEventMon.pfMonISR_Proc,
#ifdef VDEC3
                                                 (MS_U32) u8Idx,
#else
                                                 (MS_U32) NULL,
#endif
                                                 E_TASK_PRI_MEDIUM,
                                                 TRUE,
                                                 (void *)pAttr->stEventMon.u8VdecExStack,
                                                 VDEC_EX_STACK_SIZE,
                                                 pu8VdecExTask);
        if (s32VdecExTaskId[u8Idx] < 0)
        {
            VDEC_PRINT("MsOS_CreateTask failed!!\n");
            MsOS_DeleteEventGroup(s32VdecExEventId[u8Idx]);
           // MsOS_DeleteMutex(*ps32VdecExMutexId);
            return FALSE;
        }
    }

    return TRUE;
}
#else
#if 0
static MS_BOOL _VDEC_EX_MutexInit(MS_U8 u8Idx)
{
    pVdecExContext->_Attr[u8Idx].s32VdecExMutexId =
                                  MsOS_CreateMutex(E_MSOS_FIFO,
                                                  (char *)pVdecExContext->_Attr[u8Idx].pu8VdecExMutex,
                                                  MSOS_PROCESS_SHARED);

    return (pVdecExContext->_Attr[u8Idx].s32VdecExMutexId != -1);
}
#endif
#endif


#if 0
// Mutex function for VDEC API
static MS_BOOL _VDEC_EX_MutexLock(MS_U8 u8Idx)
{
    MS_S32 _s32VdecExMutexId = pVdecExContext->_Attr[u8Idx].s32VdecExMutexId;
    if (_s32VdecExMutexId != -1)
    {
        //VDEC_PRINT("____%s____\n", __FUNCTION__);
        return MsOS_ObtainMutex(_s32VdecExMutexId, MSOS_WAIT_FOREVER);
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _VDEC_EX_MutexUnlock(MS_U8 u8Idx)
{
    MS_S32 _s32VdecExMutexId = pVdecExContext->_Attr[u8Idx].s32VdecExMutexId;
    if (_s32VdecExMutexId != -1)
    {
        //VDEC_PRINT("____%s____\n", __FUNCTION__);
        return MsOS_ReleaseMutex(_s32VdecExMutexId);
    }
    else
    {
        return FALSE;
    }
}
#endif

static VDEC_EX_Decoder _VDEC_EX_GetDecoderByCodecType(VDEC_EX_CodecType eCodecType)
{
    VDEC_EX_Decoder eDecoder;

    switch (eCodecType)
    {
        case E_VDEC_EX_CODEC_TYPE_MPEG2:
        case E_VDEC_EX_CODEC_TYPE_H263:
        case E_VDEC_EX_CODEC_TYPE_MPEG4:
        #if (!VESTEL_FEATURE_ENABLE)
        case E_VDEC_EX_CODEC_TYPE_DIVX311:
        case E_VDEC_EX_CODEC_TYPE_DIVX412:
        case E_VDEC_EX_CODEC_TYPE_FLV:
        case E_VDEC_EX_CODEC_TYPE_VC1_ADV:
        case E_VDEC_EX_CODEC_TYPE_VC1_MAIN:
        #endif
            eDecoder = E_VDEC_EX_DECODER_MVD;
            bIsSupportDivxPlus = TRUE;
            break;

        case E_VDEC_EX_CODEC_TYPE_RV8:
        case E_VDEC_EX_CODEC_TYPE_RV9:
        #if VDEC_ENABLE_HVD
            eDecoder = E_VDEC_EX_DECODER_HVD;
        #else
            eDecoder = E_VDEC_EX_DECODER_NONE;
        #endif
            break;
        #if (!VESTEL_FEATURE_ENABLE)
        case E_VDEC_EX_CODEC_TYPE_H264:
        case E_VDEC_EX_CODEC_TYPE_AVS:
        case E_VDEC_EX_CODEC_TYPE_MVC:  /// SUPPORT_MVC
        case E_VDEC_EX_CODEC_TYPE_VP8:
        case E_VDEC_EX_CODEC_TYPE_HEVC:
        case E_VDEC_EX_CODEC_TYPE_VP9:
            eDecoder = E_VDEC_EX_DECODER_HVD;
            break;
        #endif
        case E_VDEC_EX_CODEC_TYPE_MJPEG:
            eDecoder = E_VDEC_EX_DECODER_MJPEG;

            break;

        default:
            eDecoder = E_VDEC_EX_DECODER_NONE;

            break;
    }

    return eDecoder;
}


#if (VDEC_EX_ISR_MONITOR)
#ifdef VDEC3
static void _VDEC_EX_MonitorNStreamISR_Proc(MS_U8 u8Idx)
{
#if 0//!defined(VDEC_UTOPIA_2K)
    MS_U32 u32VdecEventFlag = 0;
    MS_U32 cb_idx = 0;
    VDEC_EX_Attr* pAttr = &(pVdecExContext->_Attr[u8Idx]);

    while (pAttr->stEventMon.bVdecExIsrMntr == TRUE)
    {
        if(pVdecExContext->_u32PreVdecExIsrEventFlag[u8Idx] != pVdecExContext->_u32VdecExIsrEventFlag[u8Idx])
        {
            MsOS_ClearEvent(s32VdecExEventId[u8Idx], VDEC_U32_MAX);
            pVdecExContext->_u32PreVdecExIsrEventFlag[u8Idx] = pVdecExContext->_u32VdecExIsrEventFlag[u8Idx];
        }

        MsOS_WaitEvent(s32VdecExEventId[u8Idx],
            pVdecExContext->_u32PreVdecExIsrEventFlag[u8Idx],
            &u32VdecEventFlag,
            E_OR_CLEAR,
            MSOS_WAIT_FOREVER);

        //Sync CL487351
        if ((u32VdecEventFlag & ~E_VDEC_EX_EVENT_ISR_EVENT_CHANGE) & pVdecExContext->_u32VdecExIsrEventFlag[u8Idx])
        {
            for (cb_idx = 0; cb_idx < E_VDEC_EX_CB_MAX_NUM; cb_idx++)
            {
                VDEC_EX_EventInfo* pstEvInfo = &pAttr->vdecExEventInfo[cb_idx];
                MS_U32 u32Event = u32VdecEventFlag & (pstEvInfo->u32EventBitMap);
                VDEC_EX_EventCb pvdecNStreamCb = pstEvInfo->pVdecEventCb;

                if ((u32Event) && (pvdecNStreamCb))
                {
                    pvdecNStreamCb(u32Event, pstEvInfo->pParam);
                }
            }
        }
    }
#endif
}
#else
//------------------------------------------------------------------------------
/// The task for monitoring vdec isr
/// @return -None
//------------------------------------------------------------------------------
static void _VDEC_EX_MonitorISR_Proc(void)
{
#if 0//!defined(VDEC_UTOPIA_2K)
    MS_U32 u32VdecEventFlag = 0;
    MS_U32 cb_idx = 0;
    VDEC_EX_Attr* pAttr = &(pVdecExContext->_Attr[E_VDEC_EX_MAIN_STREAM]);

    while (pAttr->stEventMon.bVdecExIsrMntr == TRUE)
    {
        if(pVdecExContext->_u32PreVdecExIsrEventFlag[E_VDEC_EX_MAIN_STREAM] != pVdecExContext->_u32VdecExIsrEventFlag[E_VDEC_EX_MAIN_STREAM])
        {
            MsOS_ClearEvent(s32VdecExEventId[0], VDEC_U32_MAX);
            pVdecExContext->_u32PreVdecExIsrEventFlag[E_VDEC_EX_MAIN_STREAM] = pVdecExContext->_u32VdecExIsrEventFlag[E_VDEC_EX_MAIN_STREAM];
        }

        MsOS_WaitEvent(s32VdecExEventId[0],
            pVdecExContext->_u32PreVdecExIsrEventFlag[E_VDEC_EX_MAIN_STREAM],
            &u32VdecEventFlag,
            E_OR_CLEAR,
            MSOS_WAIT_FOREVER);

        //VDEC_PRINT("%s:flag=0x%lx\n", __FUNCTION__, u32VdecEventFlag);
        //Sync CL487351
        if ((u32VdecEventFlag & ~E_VDEC_EX_EVENT_ISR_EVENT_CHANGE) & pVdecExContext->_u32VdecExIsrEventFlag[E_VDEC_EX_MAIN_STREAM])
        {
            for (cb_idx = 0; cb_idx < E_VDEC_EX_CB_MAX_NUM; cb_idx++)
            {
                VDEC_EX_EventInfo* pstEvInfo = &pAttr->vdecExEventInfo[cb_idx];
                MS_U32 u32Event = u32VdecEventFlag & (pstEvInfo->u32EventBitMap);
                VDEC_EX_EventCb pvdecCb = pstEvInfo->pVdecEventCb;

                if ((u32Event) && (pvdecCb))
                {
                    pvdecCb(u32Event, pstEvInfo->pParam);
                }
            }
        }
    }
#endif
}

static void _VDEC_EX_MonitorSubISR_Proc(void)
{
#if 0//!defined(VDEC_UTOPIA_2K)
    MS_U32 u32VdecEventFlag = 0;
    MS_U32 cb_idx = 0;
    VDEC_EX_Attr* pAttr = &(pVdecExContext->_Attr[E_VDEC_EX_SUB_STREAM]);

    while (pAttr->stEventMon.bVdecExIsrMntr == TRUE)
    {
        if(pVdecExContext->_u32PreVdecExIsrEventFlag[E_VDEC_EX_SUB_STREAM] != pVdecExContext->_u32VdecExIsrEventFlag[E_VDEC_EX_SUB_STREAM])
        {
            MsOS_ClearEvent(s32VdecExEventId[1], VDEC_U32_MAX);
            pVdecExContext->_u32PreVdecExIsrEventFlag[E_VDEC_EX_SUB_STREAM] = pVdecExContext->_u32VdecExIsrEventFlag[E_VDEC_EX_SUB_STREAM];
        }

        MsOS_WaitEvent(s32VdecExEventId[1],
            pVdecExContext->_u32PreVdecExIsrEventFlag[E_VDEC_EX_SUB_STREAM],
            &u32VdecEventFlag,
            E_OR_CLEAR,
            MSOS_WAIT_FOREVER);

        //Sync CL487351
        if ((u32VdecEventFlag & ~E_VDEC_EX_EVENT_ISR_EVENT_CHANGE) & pVdecExContext->_u32VdecExIsrEventFlag[E_VDEC_EX_SUB_STREAM])
        {
            for (cb_idx = 0; cb_idx < E_VDEC_EX_CB_MAX_NUM; cb_idx++)
            {
                VDEC_EX_EventInfo* pstEvInfo = &pAttr->vdecExEventInfo[cb_idx];
                MS_U32 u32Event = u32VdecEventFlag & (pstEvInfo->u32EventBitMap);
                VDEC_EX_EventCb pvdecSubCb = pstEvInfo->pVdecEventCb;

                if ((u32Event) && (pvdecSubCb))
                {
                    pvdecSubCb(u32Event, pstEvInfo->pParam);
                }
            }
        }
    }
#endif
}
#endif
#endif

static void _IsrProc(MS_U8 u8Idx, MS_U32 u32Sid)
{
    //_VDEC_EX_MutexLock(u8Idx);

#if (VDEC_EX_ISR_MONITOR)
    if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & (~E_VDEC_EX_EVENT_ISR_EVENT_CHANGE))
#else
    if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx])
#endif
    {
        MS_U32 eEventFlag = E_VDEC_EX_EVENT_OFF;

        //Get event info from the specified decoder
        switch (pVdecExContext->_Attr[u8Idx].eDecoder)
        {
            case E_VDEC_EX_DECODER_MVD:
            {
                MS_U32 u32MVDIsrEvent = MDrv_MVD_GetIsrEvent(u32Sid);
                //VDEC_PRINT("%s u32Sid=0x%lx, u32MVDIsrEvent=0x%x\n", __FUNCTION__, u32Sid, u32MVDIsrEvent);
                if (u32MVDIsrEvent != E_MVD_EVENT_DISABLE_ALL)
                {
                    if (u32MVDIsrEvent & E_MVD_EVENT_DISP_VSYNC)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_DISP_ONE;
                    }

                    if (u32MVDIsrEvent & E_MVD_EVENT_DISP_RDY)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_DISP_INFO_RDY;
                    }
                    if (u32MVDIsrEvent & E_MVD_EVENT_SEQ_FOUND)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_SEQ_HDR_FOUND;
                        if (MDrv_MVD_IsSeqChg(u32Sid))
                        {
                            eEventFlag |= E_VDEC_EX_EVENT_DISP_INFO_CHG;
                        }
                    }
                    if (u32MVDIsrEvent & E_MVD_EVENT_USER_DATA)
                    {
                    #if !defined(SUPPORT_X_MODEL_FEATURE)
                        //exclude this for X4_CC for this is UserData in "decoding" order
                        eEventFlag |= E_VDEC_EX_EVENT_USER_DATA_FOUND;
                    #endif
                        MS_U8 u8Afd = MDrv_MVD_GetActiveFormat(u32Sid);

                        if ((u8Afd != 0) && (u8Afd != 0xff))
                        {
                            eEventFlag |= E_VDEC_EX_EVENT_AFD_FOUND;
                        }
                    }
                    if (u32MVDIsrEvent & E_MVD_EVENT_USER_DATA_DISP)
                    {
                        //this is UserData in "display" order
                        eEventFlag |= E_VDEC_EX_EVENT_USER_DATA_FOUND;
                    }
                    if (u32MVDIsrEvent & E_MVD_EVENT_UNMUTE)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_VIDEO_UNMUTE;
                    }
                    if (u32MVDIsrEvent & E_MVD_EVENT_FIRST_FRAME)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_FIRST_FRAME;
                    }
                    if (u32MVDIsrEvent & E_MVD_EVENT_DEC_ONE_FRAME)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_DEC_ONE;
                    }

                    if (u32MVDIsrEvent & E_MVD_EVENT_DEC_ERR)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_DEC_ERR;
                    }

                    if (u32MVDIsrEvent & E_MVD_EVENT_DEC_DATA_ERR)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_ES_DATA_ERR;
                    }
                    if (u32MVDIsrEvent & E_MVD_EVENT_XC_LOW_DEALY)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_XC_LOW_DEALY;
                    }
                    if (u32MVDIsrEvent & E_MVD_EVENT_DEC_I)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_DEC_I;
                    }
                }
                break;
            }
            case E_VDEC_EX_DECODER_HVD:
            {
                MS_U32 eEvent = E_HVD_EX_ISR_NONE;

                if (MDrv_HVD_EX_GetISRInfo(u32Sid, &eEvent))
                {
                    if (eEvent & E_HVD_EX_ISR_DISP_ONE)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_DISP_ONE;
                    }
                    if (eEvent & E_HVD_EX_ISR_DISP_REPEAT)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_DISP_REPEAT;
                    }
                    if (eEvent & E_HVD_EX_ISR_DISP_WITH_CC)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_DISP_WITH_CC;
                    }
                    if (eEvent & E_HVD_EX_ISR_DISP_FIRST_FRM)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_VIDEO_UNMUTE;
                    }
                    if (eEvent & E_HVD_EX_ISR_DEC_ONE)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_DEC_ONE;
                    }
                    if (eEvent & E_HVD_EX_ISR_DEC_I)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_DEC_I;
                    }
                    if (eEvent & E_HVD_EX_ISR_DEC_HW_ERR)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_DEC_ERR;
                    }
                    if (eEvent & E_HVD_EX_ISR_DEC_CC_FOUND)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_USER_DATA_FOUND;
                    }
                    if (eEvent & E_HVD_EX_ISR_DEC_DISP_INFO_CHANGE)
                    {
                        MS_U8 u8Afd = MDrv_HVD_EX_GetActiveFormat(u32Sid);

                        if (u8Afd != 0)
                        {
                            eEventFlag |= E_VDEC_EX_EVENT_AFD_FOUND;
                        }
                        eEventFlag |= E_VDEC_EX_EVENT_DISP_INFO_CHG;
                    }
                    if (eEvent & E_HVD_EX_ISR_DEC_DATA_ERR)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_ES_DATA_ERR;
                    }

                    if (eEvent & E_HVD_EX_ISR_DEC_FIRST_FRM)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_FIRST_FRAME;
                    }
                    if (eEvent & E_HVD_EX_ISR_DEC_SEQ_HDR_FOUND)
                    {
                        eEventFlag |= E_VDEC_EX_EVENT_SEQ_HDR_FOUND;
                    }
                }
                break;
            }
            case E_VDEC_EX_DECODER_MJPEG:
                break;
            default:
                break;
        }

        // Notify AP by calling callback functions if there are interested events
        if (E_VDEC_EX_EVENT_OFF != eEventFlag)
        {
#if (VDEC_EX_ISR_MONITOR)
            MsOS_SetEvent(s32VdecExEventId[u8Idx], eEventFlag);
#else
            MS_U32 cb_idx;
            for (cb_idx = 0; cb_idx < E_VDEC_EX_CB_MAX_NUM; cb_idx++)
            {
                VDEC_EX_EventInfo* pstEvInfo = &(pVdecExContext->_Attr[u8Idx].vdecExEventInfo[cb_idx]);
                MS_U32 u32Event = eEventFlag & (pstEvInfo->u32EventBitMap);

                if ((u32Event) && (pstEvInfo->pVdecEventCb))
                {
                    //_VDEC_EX_MutexUnlock(u8Idx);
                    pstEvInfo->pVdecEventCb(u32Event, pstEvInfo->pParam);
                    //_VDEC_EX_MutexLock(u8Idx);
                }
            }
#endif
         }

    }

    //_VDEC_EX_MutexUnlock(u8Idx);
}

#ifdef VDEC3
static void _VDEC_EX_NStream_IsrProc(MS_U32 u32Sid)
{
    _IsrProc(_VDEC_GET_DRV_IDX(u32Sid), u32Sid);
    return;
}
#else
static void _VDEC_EX_IsrProc(MS_U32 u32Sid)
{
    _IsrProc(E_VDEC_EX_MAIN_STREAM, u32Sid);
    return;
}

void _VDEC_EX_Sub_IsrProc(MS_U32 u32Sid)
{
     _IsrProc(E_VDEC_EX_SUB_STREAM, u32Sid);
    return;
}
#endif

static MS_VIRT _VDEC_EX_GetESBuffVacancy(VDEC_StreamId *pStreamId)
{
    MS_U8 u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    MS_VIRT u32ESRdPtr = 0;
    MS_VIRT u32ESWtPtr = 0;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            u32ESRdPtr = MDrv_MVD_GetESReadPtr(u32Id);
            u32ESWtPtr = MDrv_MVD_GetESWritePtr(u32Id);
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            u32ESRdPtr = MDrv_HVD_EX_GetESReadPtr(u32Id);
            u32ESWtPtr = MDrv_HVD_EX_GetESWritePtr(u32Id);
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            u32ESRdPtr = MApi_MJPEG_GetESReadPtr(u32Id);
            u32ESWtPtr = MApi_MJPEG_GetESWritePtr(u32Id);
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            break;
    }

    if (u32ESWtPtr == u32ESRdPtr)
    {
        return pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32BitstreamBufSize;
    }
    else if (u32ESWtPtr > u32ESRdPtr)
    {
        return pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32BitstreamBufSize - u32ESWtPtr + u32ESRdPtr;
    }

    return u32ESRdPtr - u32ESWtPtr;
}

static VDEC_EX_Result _VDEC_EX_GetESQuantity(VDEC_StreamId *pStreamId, MS_U32 *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    //MS_U32 u32SubViewId = (u32Id + 0x00011000);

    if (param == NULL)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            *param = MDrv_HVD_EX_GetESQuantity(u32Id);
            break;

        case E_VDEC_EX_DECODER_MVD:
        case E_VDEC_EX_DECODER_MJPEG:
            *param = 0;
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;

        default:
            *param = 0;
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_GetColorMatrixCoef(VDEC_StreamId *pStreamId, MS_U32 *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if (param == NULL)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            *param = MDrv_MVD_GetMatrixCoef(u32Id);
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            *param = 0xff;
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_GetIsDynScalingEnabled(VDEC_StreamId *pStreamId, MS_U32* param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if (!param)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }
    else
    {
        *param = FALSE;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
        {
            if (E_HVD_EX_OK == MDrv_HVD_EX_CheckDispInfoRdy(u32Id))
            {
                *param = (MS_BOOL)MDrv_HVD_EX_GetDynamicScalingInfo(u32Id, E_HVD_EX_DS_IS_ENABLED);
                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_RET_NOT_READY;
            }
            break;
        }
        case E_VDEC_EX_DECODER_MVD:
        {
            if (0 != MDrv_MVD_GetDispRdy(u32Id))
            {
                *param = MDrv_MVD_GetIsDynScalingEnabled(u32Id);
                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_RET_NOT_READY;
            }
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        default:
        {
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        }
    }

    VDEC_INFO(u8Idx, VDEC_PRINT("%s = %x\n", __FUNCTION__, (MS_BOOL)*param));
    return eRet;
}

static MS_BOOL _VDEC_EX_HandleSingleDecodeMode(VDEC_EX_Decoder eDec)
{
    MS_BOOL bRet = FALSE;

    switch (eDec)
    {
        case E_VDEC_EX_DECODER_MVD:
            bRet = MDrv_MVD_SetSingleDecodeMode(TRUE);
            break;
        case E_VDEC_EX_DECODER_HVD:
            bRet = MDrv_HVD_SetSingleDecodeMode(TRUE);
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            break;
        default:
            break;
    }

    return bRet;
}

#ifndef VDEC3
static VDEC_EX_Result _VDEC_EX_ReparseVP8Packet(MS_U8 u8Idx, HVD_EX_PacketInfo *pPacketInfo)
{
    MS_U8 *pData = (MS_U8 *) MsOS_PA2KSEG1(pPacketInfo->u32Staddr + pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32BitstreamBufAddr);
    MS_U32 u32FrmSize;
    MS_U32 u32FirstPartitionSize;
    MS_U8 u8FrmType; //u8Version;

    if (NULL == pData)
    {
        return E_VDEC_EX_FAIL;
    }

    u32FrmSize = pPacketInfo->u32Length;
    //u32FrmSize = (pData[3] << 24) | (pData[2] << 16) | (pData[1] << 8) | (pData[0]);
    //pData += 12;
    u8FrmType = (pData[0] & 1);
    //u8Version = (pData[0] >> 1) & 7;
    u32FirstPartitionSize = (pData[0] | (pData[1] << 8) | (pData[2] << 16)) >> 5;

    //pPacketInfo->u32Staddr += 12;
    pPacketInfo->u32Length  = u32FirstPartitionSize + (u8FrmType ? 3 : 10);
    pPacketInfo->u32Staddr2 = pPacketInfo->u32Staddr + pPacketInfo->u32Length;
    pPacketInfo->u32Length2 = u32FrmSize - pPacketInfo->u32Length;

    #if 0
    VDEC_PRINT("frm_type=%d, ver=%d, first_part_size=%lu\n",
        u8FrmType, u8Version, u32FirstPartitionSize);
    VDEC_PRINT("u32FrmSize=0x%lx, st1=0x%lx, size=0x%lx, st2=0x%lx, size=0x%lx\n",
        u32FrmSize,
        pPacketInfo->u32Staddr, pPacketInfo->u32Length,
        pPacketInfo->u32Staddr2, pPacketInfo->u32Length2);
    #endif

    //VDEC_PRINT("FRM: %x %x %x\n", pPacketInfo->u32Length, u32FirstPartitionSize, u8FrmType);
    //VDEC_PRINT("[VDEC] %x %x %x %x\n", pPacketInfo->u32Staddr, pPacketInfo->u32Length, pPacketInfo->u32Staddr2, pPacketInfo->u32Length2);

    return E_VDEC_EX_OK;
}
#endif

//------------------------------------------------------------------------------
/// Set Support AVC to MVC codec
/// @param bEnable \b IN : turn on / off.
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_Support_AVC2MVC(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_Support_AVC2MVC(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Set View exchange
/// @param bEnable \b IN : turn on / off.
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_3DLR_View_Excahnge(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_3DLR_View_Exchange(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

static void _VDEC_EX_Context_Init(void)
{
#ifdef VDEC3
    MS_U8 i;

    for (i = 0; i < VDEC_MAX_SUPPORT_STREAM_NUM; i++)
    {
#if (VDEC_EX_ISR_MONITOR)
        pVdecExContext->_Attr[i].stEventMon.bVdecExIsrMntr = TRUE;
        strncpy(pVdecExContext->_Attr[i].stEventMon.pu8VdecExEvent, "VDEC_EX_Event", sizeof(pVdecExContext->_Attr[i].stEventMon.pu8VdecExEvent));
        strncpy(pVdecExContext->_Attr[i].stEventMon.pu8VdecExTask, "VDEC_EX_Task", sizeof(pVdecExContext->_Attr[i].stEventMon.pu8VdecExTask));
        pVdecExContext->_u32VdecExIsrEventFlag[i] = E_VDEC_EX_EVENT_ISR_EVENT_CHANGE;
        pVdecExContext->_u32PreVdecExIsrEventFlag[i] = E_VDEC_EX_EVENT_ISR_EVENT_CHANGE;
#endif
        pVdecExContext->u32PriData[i] = VDEC_U32_MAX;
    }
#else
    //pVdecExContext->_Attr[0].s32VdecExMutexId = -1;
    //strncpy(pVdecExContext->_Attr[0].pu8VdecExMutex,"VDEC_EX_Mutex0",sizeof(pVdecExContext->_Attr[0].pu8VdecExMutex));
#if (VDEC_EX_ISR_MONITOR)
    pVdecExContext->_Attr[0].stEventMon.bVdecExIsrMntr = TRUE;
    strncpy(pVdecExContext->_Attr[0].stEventMon.pu8VdecExEvent,"VDEC_EX_Event0",sizeof(pVdecExContext->_Attr[0].stEventMon.pu8VdecExEvent));
    //pVdecExContext->_Attr[0].stEventMon.s32VdecExTaskId = -1;
    strncpy(pVdecExContext->_Attr[0].stEventMon.pu8VdecExTask,"VDEC_EX_Task0",sizeof(pVdecExContext->_Attr[0].stEventMon.pu8VdecExTask));
    //pVdecExContext->_Attr[0].stEventMon.pfMonISR_Proc = _VDEC_EX_MonitorISR_Proc;
#endif

    //pVdecExContext->_Attr[1].s32VdecExMutexId = -1;
    //strncpy(pVdecExContext->_Attr[1].pu8VdecExMutex,"VDEC_EX_Mutex1",sizeof(pVdecExContext->_Attr[1].pu8VdecExMutex));
#if (VDEC_EX_ISR_MONITOR)
    pVdecExContext->_Attr[1].stEventMon.bVdecExIsrMntr = TRUE;
    strncpy(pVdecExContext->_Attr[1].stEventMon.pu8VdecExEvent,"VDEC_EX_Event1",sizeof(pVdecExContext->_Attr[1].stEventMon.pu8VdecExEvent));
    //pVdecExContext->_Attr[1].stEventMon.s32VdecExTaskId = -1;
    strncpy(pVdecExContext->_Attr[1].stEventMon.pu8VdecExTask,"VDEC_EX_Task1",sizeof(pVdecExContext->_Attr[1].stEventMon.pu8VdecExTask));
    //pVdecExContext->_Attr[1].stEventMon.pfMonISR_Proc = _VDEC_EX_MonitorSubISR_Proc;
#endif

#if (VDEC_EX_ISR_MONITOR)
    pVdecExContext->_u32VdecExIsrEventFlag[0] = E_VDEC_EX_EVENT_ISR_EVENT_CHANGE;
    pVdecExContext->_u32VdecExIsrEventFlag[1] = E_VDEC_EX_EVENT_ISR_EVENT_CHANGE;
    pVdecExContext->_u32PreVdecExIsrEventFlag[0] = E_VDEC_EX_EVENT_ISR_EVENT_CHANGE;
    pVdecExContext->_u32PreVdecExIsrEventFlag[1] = E_VDEC_EX_EVENT_ISR_EVENT_CHANGE;
#endif

    pVdecExContext->u32PriData[0] = VDEC_U32_MAX;
    pVdecExContext->u32PriData[1] = VDEC_U32_MAX;
#endif

    //strncpy(pVdecExContext->_u8VDEC_Mutex,"VDEC_API_Mutex",sizeof(pVdecExContext->_u8VDEC_Mutex));
   // pVdecExContext->u32VdecMutexCnt[0] = 0;
   // pVdecExContext->u32VdecMutexCnt[1] = 0;
    for (i = 0; i < VDEC_MAX_SUPPORT_STREAM_NUM; i++)
    {
        pVdecExContext->_power_state[i] = E_VDEC_EX_POWER_NONE;
    }
    pVdecExContext->_prev_u16PowerState = E_POWER_MECHANICAL;
}

#ifdef VDEC3
static void _VDEC_EX_Context_Init_Function_Pointer(MS_U8 u8Idx)
#else
static void _VDEC_EX_Context_Init_Function_Pointer(VDEC_EX_Stream eStreamType)
#endif
{
#if (VDEC_EX_ISR_MONITOR)
#ifdef VDEC3
    pVdecExContext->_Attr[u8Idx].stEventMon.pfMonISR_Proc = _VDEC_EX_MonitorNStreamISR_Proc;
#else
    if(eStreamType == E_VDEC_EX_MAIN_STREAM)
    {
        pVdecExContext->_Attr[0].stEventMon.pfMonISR_Proc = _VDEC_EX_MonitorISR_Proc;
    }
    else if(eStreamType == E_VDEC_EX_SUB_STREAM)
    {
        pVdecExContext->_Attr[1].stEventMon.pfMonISR_Proc = _VDEC_EX_MonitorSubISR_Proc;
    }
#endif
#endif
}


static VDEC_EX_Result _MApi_VDEC_EX_GetFrmPackingArrSEI(VDEC_StreamId *pStreamId,void *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    VDEC_EX_Frame_packing_SEI *pFPASEIInfo = NULL;

    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if( param == NULL)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }
    else
    {
        pFPASEIInfo = (VDEC_EX_Frame_packing_SEI *)param;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            {
                VDEC_EX_Frame_packing_SEI_EX *pFPASEIInfo = (VDEC_EX_Frame_packing_SEI_EX *)param;
                MS_U8 u8StereoType = MDrv_MVD_GetStereoType(u32Id);

                if (u8StereoType != 0)
                {
                    pFPASEIInfo->u8Frm_packing_arr_type = u8StereoType;
                }
                else
                {
                    eRet = E_VDEC_EX_RET_INVALID_PARAM;
                }
            }
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_GetFrmPackingArrSEI(u32Id,&(pVdecExContext->_stFrmPacking[u8Idx])));

            pFPASEIInfo->u8Frm_packing_arr_cnl_flag = pVdecExContext->_stFrmPacking[u8Idx].u8Frm_packing_arr_cnl_flag;
            pFPASEIInfo->u8Frm_packing_arr_type = pVdecExContext->_stFrmPacking[u8Idx].u8Frm_packing_arr_type;
            pFPASEIInfo->u8content_interpretation_type = pVdecExContext->_stFrmPacking[u8Idx].u8content_interpretation_type;
            pFPASEIInfo->u1Quincunx_sampling_flag = pVdecExContext->_stFrmPacking[u8Idx].u1Quincunx_sampling_flag;

            pFPASEIInfo->u1Spatial_flipping_flag = pVdecExContext->_stFrmPacking[u8Idx].u1Spatial_flipping_flag;
            pFPASEIInfo->u1Frame0_flipping_flag = pVdecExContext->_stFrmPacking[u8Idx].u1Frame0_flipping_flag;
            pFPASEIInfo->u1Field_views_flag = pVdecExContext->_stFrmPacking[u8Idx].u1Field_views_flag;
            pFPASEIInfo->u1Current_frame_is_frame0_flag = pVdecExContext->_stFrmPacking[u8Idx].u1Current_frame_is_frame0_flag;

            pFPASEIInfo->u1Frame0_self_contained_flag = pVdecExContext->_stFrmPacking[u8Idx].u1Frame0_self_contained_flag;
            pFPASEIInfo->u1Frame1_self_contained_flag = pVdecExContext->_stFrmPacking[u8Idx].u1Frame1_self_contained_flag;
            pFPASEIInfo->u4Frame0_grid_position_x = pVdecExContext->_stFrmPacking[u8Idx].u4Frame0_grid_position_x;
            pFPASEIInfo->u4Frame0_grid_position_y = pVdecExContext->_stFrmPacking[u8Idx].u4Frame0_grid_position_y;

            pFPASEIInfo->u4Frame1_grid_position_x = pVdecExContext->_stFrmPacking[u8Idx].u4Frame1_grid_position_x;
            pFPASEIInfo->u4Frame1_grid_position_y = pVdecExContext->_stFrmPacking[u8Idx].u4Frame1_grid_position_y;

            break;
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }
    return eRet;
}

static VDEC_EX_Result _MApi_VDEC_EX_GetFrmPackingArrSEI_EX(VDEC_StreamId *pStreamId, void *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;

    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    if( param == NULL)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            {
                VDEC_EX_Frame_packing_SEI_EX *pFPASEIInfo = (VDEC_EX_Frame_packing_SEI_EX *)param;
                MS_U8 u8StereoType = MDrv_MVD_GetStereoType(u32Id);

                if (u8StereoType != 0)
                {
                    pFPASEIInfo->u8Frm_packing_arr_type = u8StereoType;
                }
                else
                {
                    eRet = E_VDEC_EX_RET_INVALID_PARAM;
                }
            }
            break;
        case E_VDEC_EX_DECODER_HVD:
            {
                VDEC_EX_VerCtl *pVerCtl = (VDEC_EX_VerCtl *)param;

                if((pVerCtl->u32version == 0)
                && (pVerCtl->u32size == sizeof(VDEC_EX_Frame_packing_SEI_EX)))
                {
                    VDEC_EX_Frame_packing_SEI_EX *pFPASEIInfo = (VDEC_EX_Frame_packing_SEI_EX *)param;
                    _HVD_RET_HANDLE(MDrv_HVD_EX_GetFrmPackingArrSEI(u32Id, &(pVdecExContext->_stFrmPacking[u8Idx])));

                    pFPASEIInfo->bUsed =pVdecExContext->_stFrmPacking[u8Idx].bUsed;
                    pFPASEIInfo->u8Frm_packing_arr_cnl_flag = pVdecExContext->_stFrmPacking[u8Idx].u8Frm_packing_arr_cnl_flag;
                    pFPASEIInfo->u8Frm_packing_arr_type = pVdecExContext->_stFrmPacking[u8Idx].u8Frm_packing_arr_type;
                    pFPASEIInfo->u8content_interpretation_type = pVdecExContext->_stFrmPacking[u8Idx].u8content_interpretation_type;
                    pFPASEIInfo->u1Quincunx_sampling_flag = pVdecExContext->_stFrmPacking[u8Idx].u1Quincunx_sampling_flag;
                    pFPASEIInfo->u1Spatial_flipping_flag = pVdecExContext->_stFrmPacking[u8Idx].u1Spatial_flipping_flag;
                    pFPASEIInfo->u1Frame0_flipping_flag = pVdecExContext->_stFrmPacking[u8Idx].u1Frame0_flipping_flag;
                    pFPASEIInfo->u1Field_views_flag = pVdecExContext->_stFrmPacking[u8Idx].u1Field_views_flag;
                    pFPASEIInfo->u1Current_frame_is_frame0_flag = pVdecExContext->_stFrmPacking[u8Idx].u1Current_frame_is_frame0_flag;
                    pFPASEIInfo->u1Frame0_self_contained_flag = pVdecExContext->_stFrmPacking[u8Idx].u1Frame0_self_contained_flag;
                    pFPASEIInfo->u1Frame1_self_contained_flag = pVdecExContext->_stFrmPacking[u8Idx].u1Frame1_self_contained_flag;
                    pFPASEIInfo->u4Frame0_grid_position_x = pVdecExContext->_stFrmPacking[u8Idx].u4Frame0_grid_position_x;
                    pFPASEIInfo->u4Frame0_grid_position_y = pVdecExContext->_stFrmPacking[u8Idx].u4Frame0_grid_position_y;
                    pFPASEIInfo->u4Frame1_grid_position_x = pVdecExContext->_stFrmPacking[u8Idx].u4Frame1_grid_position_x;
                    pFPASEIInfo->u4Frame1_grid_position_y = pVdecExContext->_stFrmPacking[u8Idx].u4Frame1_grid_position_y;
                    pFPASEIInfo->bottom = (MS_U32)pVdecExContext->_stFrmPacking[u8Idx].u16CropBottom;
                    pFPASEIInfo->left = (MS_U32)pVdecExContext->_stFrmPacking[u8Idx].u16CropLeft;
                    pFPASEIInfo->right = (MS_U32)pVdecExContext->_stFrmPacking[u8Idx].u16CropRight;
                    pFPASEIInfo->top = (MS_U32)pVdecExContext->_stFrmPacking[u8Idx].u16CropTop;
                    if((pFPASEIInfo->bIsCropInfo == FALSE)
                    && (pVdecExContext->_stFrmPacking[u8Idx].bvaild == TRUE)
                    && (pVdecExContext->_stFrmPacking[u8Idx].bUsed == TRUE))
                    {
#if 1//#ifdef VDEC_UTOPIA_2K
                        pFPASEIInfo->u32DataBuff = MsOS_VA2PA(pVdecExContext->_stFrmPacking[u8Idx].u32payload);
#else
                        pFPASEIInfo->u32DataBuff = pVdecExContext->_stFrmPacking[u8Idx].u32payload;
#endif
                        pFPASEIInfo->u32DataSize = (MS_U32)pVdecExContext->_stFrmPacking[u8Idx].u8payload_len;
                        pFPASEIInfo->bValid = TRUE;
                        pVdecExContext->_stFrmPacking[u8Idx].bvaild = FALSE;
                    }
                    else
                    {
                        pFPASEIInfo->bValid = FALSE;
                    }
                }
                else
                {
                    VDEC_PRINT("%s: invalid u32Version(%d) or u32Size(%d)\n", __FUNCTION__, pVerCtl->u32version, (MS_U32)sizeof(VDEC_EX_Frame_packing_SEI_EX));
                }
            }

            break;
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }
    return eRet;
}

static VDEC_EX_Result _MApi_VDEC_EX_GetDisplayColourVolumeArrSEI(VDEC_StreamId *pStreamId,void *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    VDEC_EX_DisplayColourVolume_SEI *pDCVSEIInfo = NULL;

    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if( param == NULL)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }
    else
    {
        pDCVSEIInfo = (VDEC_EX_DisplayColourVolume_SEI *)param;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_GetDisplayColourVolumeArrSEI(u32Id,&(pVdecExContext->_stDCV[u8Idx])));

            pDCVSEIInfo->bColourVolumeSEIEnabled = pVdecExContext->_stDCV[u8Idx].bColourVolumeSEIEnabled;
            pDCVSEIInfo->u32MaxLuminance         = pVdecExContext->_stDCV[u8Idx].u32MaxLuminance;
            pDCVSEIInfo->u32MinLuminance         = pVdecExContext->_stDCV[u8Idx].u32MinLuminance;

            pDCVSEIInfo->u16Primaries[0][0]      = pVdecExContext->_stDCV[u8Idx].u16Primaries[0][0];
            pDCVSEIInfo->u16Primaries[0][1]      = pVdecExContext->_stDCV[u8Idx].u16Primaries[0][1];
            pDCVSEIInfo->u16Primaries[1][0]      = pVdecExContext->_stDCV[u8Idx].u16Primaries[1][0];
            pDCVSEIInfo->u16Primaries[1][1]      = pVdecExContext->_stDCV[u8Idx].u16Primaries[1][1];
            pDCVSEIInfo->u16Primaries[2][0]      = pVdecExContext->_stDCV[u8Idx].u16Primaries[2][0];
            pDCVSEIInfo->u16Primaries[2][1]      = pVdecExContext->_stDCV[u8Idx].u16Primaries[2][1];

            pDCVSEIInfo->u16WhitePoint[0]        = pVdecExContext->_stDCV[u8Idx].u16WhitePoint[0];
            pDCVSEIInfo->u16WhitePoint[1]        = pVdecExContext->_stDCV[u8Idx].u16WhitePoint[1];

            break;
        case E_VDEC_EX_DECODER_MVD:
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }
    return eRet;
}

static VDEC_EX_Result _MApi_VDEC_EX_GetContentLightLevelInfoSEI(VDEC_StreamId *pStreamId,void *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    VDEC_EX_ContentLightLevelInfo_SEI *pCLLI_SEI = NULL;

    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    if (param == NULL)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }
    else
    {
        pCLLI_SEI = (VDEC_EX_ContentLightLevelInfo_SEI *)param;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_GetContentLightLevelInfoSEI(u32Id, &(pVdecExContext->_stCLLI[u8Idx])));

            pCLLI_SEI->bUsed = pVdecExContext->_stCLLI[u8Idx].bUsed;
            pCLLI_SEI->bContentLightLevelEnabled = pVdecExContext->_stCLLI[u8Idx].ContentLightLevelEnabled;
            pCLLI_SEI->u16MaxContentLightLevel = pVdecExContext->_stCLLI[u8Idx].maxContentLightLevel;
            pCLLI_SEI->u16MaxPicAverageLightLevel = pVdecExContext->_stCLLI[u8Idx].maxPicAverageLightLevel;
            break;

        case E_VDEC_EX_DECODER_MVD:
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            pCLLI_SEI->bUsed = 0;
            pCLLI_SEI->bContentLightLevelEnabled = 0;
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }
    return eRet;
}

#ifdef VDEC_CAP_DV_OTT_API
static VDEC_EX_Result _MApi_VDEC_EX_GetDVSupportProfiles(void *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    VDEC_EX_DV_Info *pDV_Info = NULL;
    MS_U32 u32DVSupportProfiles;

    if (param == NULL)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }
    else
    {
        pDV_Info = (VDEC_EX_DV_Info *)param;
    }

    if(MDrv_HVD_EX_GetDVSupportProfiles(&u32DVSupportProfiles) == E_HVD_EX_OK)
    {
        pDV_Info->u32DVSupportProfiles = u32DVSupportProfiles;
        eRet = E_VDEC_EX_OK;
    }

    return eRet;
}

static VDEC_EX_Result _MApi_VDEC_EX_GetDVSupportHighestLevel(void *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    VDEC_EX_DV_Info *pDV_Info = NULL;
    MS_U32 u32DVLevel;

    if (param == NULL)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }
    else
    {
        pDV_Info = (VDEC_EX_DV_Info *)param;
    }

    if(MDrv_HVD_EX_GetDVSupportHighestLevel(pDV_Info->eDVProfile, &u32DVLevel) == E_HVD_EX_OK)
    {
        pDV_Info->eDVLevel = (VDEC_EX_DV_Stream_Highest_Level)u32DVLevel;
        eRet = E_VDEC_EX_OK;
    }

    return eRet;
}
#endif

VDEC_EX_Result _MApi_VDEC_EX_V2_GetVUI_DISP_INFO(VDEC_StreamId *pStreamId, void *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    VDEC_EX_AVC_VUI_DISP_INFO *pVUI_DISP_Info = NULL;

    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if( param == NULL)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }
    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
        {
            VDEC_EX_AVC_VUI_DISP_INFO *pVUIInfo = (VDEC_EX_AVC_VUI_DISP_INFO *)param;
            pVUI_DISP_Info = (VDEC_EX_AVC_VUI_DISP_INFO *)MDrv_HVD_EX_GetData(u32Id, E_HVD_EX_GDATA_TYPE_AVC_VUI_DISP_INFO);

            pVUIInfo->bAspect_ratio_info_present_flag        = pVUI_DISP_Info->bAspect_ratio_info_present_flag;
            pVUIInfo->u8Aspect_ratio_idc                     = pVUI_DISP_Info->u8Aspect_ratio_idc;
            pVUIInfo->u16Sar_width                           = pVUI_DISP_Info->u16Sar_width;
            pVUIInfo->u16Sar_height                          = pVUI_DISP_Info->u16Sar_height;
            pVUIInfo->bOverscan_info_present_flag            = pVUI_DISP_Info->bOverscan_info_present_flag;
            pVUIInfo->bOverscan_appropriate_flag             = pVUI_DISP_Info->bOverscan_appropriate_flag;
            pVUIInfo->bVideo_signal_type_present_flag        = pVUI_DISP_Info->bVideo_signal_type_present_flag;
            pVUIInfo->u8Video_format                         = pVUI_DISP_Info->u8Video_format;
            pVUIInfo->bVideo_full_range_flag                 = pVUI_DISP_Info->bVideo_full_range_flag;
            pVUIInfo->bColour_description_present_flag       = pVUI_DISP_Info->bColour_description_present_flag;
            pVUIInfo->u8Colour_primaries                     = pVUI_DISP_Info->u8Colour_primaries;
            pVUIInfo->u8Transfer_characteristics             = pVUI_DISP_Info->u8Transfer_characteristics;
            pVUIInfo->u8Matrix_coefficients                  = pVUI_DISP_Info->u8Matrix_coefficients;
            pVUIInfo->bChroma_location_info_present_flag     = pVUI_DISP_Info->bChroma_location_info_present_flag;
            pVUIInfo->u8Chroma_sample_loc_type_top_field     = pVUI_DISP_Info->u8Chroma_sample_loc_type_top_field;
            pVUIInfo->u8Chroma_sample_loc_type_bottom_field  = pVUI_DISP_Info->u8Chroma_sample_loc_type_bottom_field;
            pVUIInfo->bTiming_info_present_flag              = pVUI_DISP_Info->bTiming_info_present_flag;
            pVUIInfo->bFixed_frame_rate_flag                 = pVUI_DISP_Info->bFixed_frame_rate_flag;
            pVUIInfo->u32Num_units_in_tick                   = pVUI_DISP_Info->u32Num_units_in_tick;
            pVUIInfo->u32Time_scale                          = pVUI_DISP_Info->u32Time_scale;

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        case E_VDEC_EX_DECODER_MVD:
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }
    return eRet;
}

//------------------------------------------------------------------------------
/// Set _VDEC_EX_AVC_Enable_New_Slow_Motion
/// @param bEnable \b IN : turn on / off.
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_AVC_Enable_New_Slow_Motion(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_Enable_New_Slow_Motion(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Set _VDEC_EX_DynamicScalingResvNBuffer
/// @param bEnable \b IN : turn on / off.
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_DynamicScalingResvNBuffer(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_DynamicScalingResvNBuffer(u32Id, bEnable));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_IgnorePicStructDisplay(VDEC_StreamId *pStreamId, MS_U32 param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_IgnorePicStructDisplay(u32Id, param));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_OK;
            break;
        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_Field_Polarity_Display_One_Field(VDEC_StreamId *pStreamId, VDEC_EX_Field_Polarity* pFieldPority)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if(pFieldPority == NULL)
    {
        return E_VDEC_EX_FAIL;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            if(MDrv_MVD_Field_Polarity_Display_One_field(u32Id,pFieldPority->bEnable,pFieldPority->u8DisplayTop) == TRUE)
            {
                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_FAIL;
            }
            break;
        case E_VDEC_EX_DECODER_HVD:
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_OK;
            break;
        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_InputPtsFreerunMode(VDEC_StreamId *pStreamId, MS_U32 param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_InputPtsFreerunMode(u32Id, param));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_ErrConcealStartSlice1stMB(VDEC_StreamId *pStreamId, MS_U32 param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_ErrConcealStartSlice1stMB(u32Id, param));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}


#if VDEC_ENABLE_MVC
static VDEC_EX_Result _MApi_VDEC_EX_MVC_SubFrameDispInfo(VDEC_StreamId *pStreamId, MS_U32* param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    MS_U32 u32SubViewId = (u32Id + 0x00011000);
    VDEC_EX_FrameInfo *pExFrmInfo = NULL;

    if(pVdecExContext->_Attr[u8Idx].eCodecType != E_VDEC_EX_CODEC_TYPE_MVC)
    {
        return E_VDEC_EX_RET_UNSUPPORTED;
    }

    if( param == NULL)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }
    else
    {
        pExFrmInfo = (VDEC_EX_FrameInfo *)param;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {

        case E_VDEC_EX_DECODER_HVD:
        {
            HVD_EX_FrameInfo info;

            _VDEC_Memset(&info, 0, sizeof(HVD_EX_FrameInfo));

            _HVD_RET_HANDLE(MDrv_HVD_EX_GetFrmInfo(u32SubViewId, E_HVD_EX_GFRMINFO_DISPLAY_SUB, &info));

            pExFrmInfo->eFrameType    = _VDEC_EX_MapFrmType2HVD(info.eFrmType);
            pExFrmInfo->u16Height     = info.u16Height;
            pExFrmInfo->u16Width      = info.u16Width;
            pExFrmInfo->u16Pitch      = info.u16Pitch;
            pExFrmInfo->u32ChromaAddr = info.u32ChromaAddr;
            pExFrmInfo->u32ID_H       = info.u32ID_H;
            pExFrmInfo->u32ID_L       = info.u32ID_L;
            pExFrmInfo->u32LumaAddr   = info.u32LumaAddr;
            pExFrmInfo->u32TimeStamp  = info.u32TimeStamp;
            pExFrmInfo->eFieldType    = _VDEC_EX_MapFieldType2HVD(info.eFieldType);

            eRet = E_VDEC_EX_OK;
            break;
        }

        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return eRet;
}

static VDEC_EX_Result _MApi_VDEC_EX_MVC_SetControl(VDEC_StreamId *pStreamId, VDEC_MVC_Control_type eType, void** param)
{
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    MS_U32 u32SubViewId = (u32Id + 0x00011000);

    if(pVdecExContext->_Attr[u8Idx].eCodecType != E_VDEC_EX_CODEC_TYPE_MVC)
    {
        return E_VDEC_EX_RET_UNSUPPORTED;
    }

    switch(eType)
    {
        case E_VDEC_EX_MVC_SET_BBU2_PUSH_PACKET:
        {
            HVD_EX_PacketInfo packetInfo;
            VDEC_EX_DecCmd *pInput;
            _VDEC_Memset(&packetInfo, 0, sizeof(HVD_EX_PacketInfo));
            pInput = (VDEC_EX_DecCmd *)(*((MS_VIRT*)(param[0])));

            packetInfo.u32Staddr    = pInput->u32StAddr;
            packetInfo.u32Length    = pInput->u32Size;
            packetInfo.u32TimeStamp = pInput->u32Timestamp;
            packetInfo.u32ID_H      = pInput->u32ID_H;
            packetInfo.u32ID_L      = pInput->u32ID_L;

            _HVD_RET_HANDLE(MDrv_HVD_EX_PushQueue(u32SubViewId, &packetInfo));
            break;
        }
        case E_VDEC_EX_MVC_SET_BBU2_FIRE_DECCMD:
            MDrv_HVD_EX_PushQueue_Fire(u32SubViewId);
            break;

         default:
            return E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }
    return E_VDEC_EX_OK;
}

static VDEC_EX_Result _MApi_VDEC_EX_MVC_GetControl(VDEC_StreamId *pStreamId, VDEC_MVC_Control_type eType, MS_U32* param)
{
    //VDEC_EX_Result ret = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    MS_U32 u32SubViewId = (u32Id + 0x00011000);

    if(pVdecExContext->_Attr[u8Idx].eCodecType != E_VDEC_EX_CODEC_TYPE_MVC)
    {
        return E_VDEC_EX_RET_UNSUPPORTED;
    }

    if( param == NULL)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    switch(eType)
    {
        case E_VDEC_EX_MVC_GET_BBU2_DECQ_VACANCY:
            *param = MDrv_HVD_EX_GetBBUVacancy(u32SubViewId);
            break;
        case E_VDEC_EX_MVC_GET_ES2_READ_PTR:
            *param = MDrv_HVD_EX_GetESReadPtr(u32SubViewId);
            break;
        case E_VDEC_EX_MVC_GET_ES2_WRITE_PTR:
            *param = MDrv_HVD_EX_GetESWritePtr(u32SubViewId);
            break;

        default:
            return E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return E_VDEC_EX_OK;
}

#endif /// VDEC_ENABLE_MVC


static VDEC_EX_Result _MApi_VDEC_EX_Get_U64PTS(VDEC_StreamId *pStreamId,void* param)
{
    MS_U64* ptr_tmp = NULL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if( param == NULL)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }
    else
    {
        ptr_tmp = (MS_U64*)param;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            if (MDrv_MVD_GetU64PTS(u32Id,E_MVD_PTS_DISP) == VDEC_U64_MAX)
            {
                *ptr_tmp = VDEC_U64_MAX;
            }
            else
            {
                *ptr_tmp = MDrv_MVD_GetU64PTS(u32Id,E_MVD_PTS_DISP) + MDrv_MVD_GetAVSyncDelay(u32Id);
            }
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            *ptr_tmp = MDrv_HVD_EX_GetU64PTS(u32Id);
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            *ptr_tmp = MApi_MJPEG_GetPTS(u32Id);
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            *ptr_tmp = 0;
            break;
    }
    return E_VDEC_EX_OK;
}

static VDEC_EX_Result _MApi_VDEC_EX_Get_PRE_PAS_U64PTS(VDEC_StreamId *pStreamId,void* param)
{
    MS_U64* ptr_tmp = NULL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    _VDEC_INVALID_DRVID_RET(u8Idx, u32Id);

    if( param == NULL)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }
    else
    {
        ptr_tmp = (MS_U64*)param;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            *ptr_tmp = MDrv_MVD_GetU64PTS(u32Id,E_MVD_PTS_PRE_PAS);
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            *ptr_tmp = MDrv_HVD_EX_GetU64PTS_PreParse(u32Id);
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            *ptr_tmp = 0;
            break;
    }
    return E_VDEC_EX_OK;
}


static VDEC_EX_Result _VDEC_EX_GetIsOriInterlaceMode(VDEC_StreamId *pStreamId, MS_U32 *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if (param == NULL)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MVD_FrameInfo info;
            _VDEC_Memset(&info, 0, sizeof(MVD_FrameInfo));
            MDrv_MVD_GetFrameInfo(u32Id, &info);
            *param = (MS_U32)info.u8Interlace;
            eRet = E_VDEC_EX_OK;
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            if ( MDrv_HVD_EX_CheckDispInfoRdy(u32Id) == E_HVD_EX_OK)
            {
                *param = MDrv_HVD_EX_GetData(u32Id,E_HVD_EX_GDATA_TYPE_IS_ORI_INTERLACE_MODE);
                eRet = E_VDEC_EX_OK;
            }
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            *param = 0;
            eRet = E_VDEC_EX_OK;
            break;
        }
       default:
            *param = 0;
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_GetMbsOnlyFlag(VDEC_StreamId *pStreamId, MS_U32 *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
        {
            if ( E_HVD_EX_OK == MDrv_HVD_EX_CheckDispInfoRdy(u32Id))
            {
                *param = MDrv_HVD_EX_GetData(u32Id, E_HVD_EX_GDATA_TYPE_FRAME_MBS_ONLY_FLAG);
                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_RET_NOT_READY;
            }
            break;
        }
        case E_VDEC_EX_DECODER_MVD:
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_IsLeastDispQSize(VDEC_StreamId *pStreamId, MS_U32 *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            *param = MDrv_HVD_EX_GetData(u32Id, E_HVD_EX_GDATA_TYPE_IS_LEAST_DISPQ_SIZE);
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MVD:
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_GetFieldPicFlag(VDEC_StreamId *pStreamId, MS_U32 *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            *param = MDrv_HVD_EX_GetData(u32Id, E_HVD_EX_GDATA_TYPE_FIELD_PIC_FLAG);
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MVD:
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_GetFlushPatternEntryNum(VDEC_StreamId *pStreamId, MS_U32 *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    //MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            *param = (MS_U32)2;
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MVD:
            *param = (MS_U32)1;
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_GetHWMaxPixel(VDEC_StreamId *pStreamId, MS_U32 *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            *param = MDrv_HVD_EX_GetData(u32Id,E_HVD_EX_GDATA_TYPE_HVD_HW_MAX_PIXEL);
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MVD:
            *param = MDrv_MVD_GetMaxPixel(u32Id);
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_GetDSBufMiuSel(VDEC_StreamId *pStreamId, MS_U32 *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            *param = (MS_U32)MDrv_HVD_EX_GetDSBufMiuSelect(u32Id);
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}



static VDEC_EX_Result _VDEC_EX_GetSupport2ndMVOPInterface(VDEC_StreamId *pStreamId, MS_BOOL* param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            *param = MDrv_HVD_EX_GetSupport2ndMVOPInterface();
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MVD:
            *param = MDrv_MVD_GetSupport2ndMVOPInterface();
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            *param = TRUE;
            eRet = E_VDEC_EX_OK;
            break;
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_GetVsyncBridgeAddr(VDEC_StreamId *pStreamId, MS_VIRT *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_VIRT u32SHMaddr = 0;
    MS_VIRT u32VsyncSHMOffset = 0;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MVD:
        case E_VDEC_EX_DECODER_MJPEG:
            MDrv_MVD_REE_GetSHMInformation(&u32SHMaddr,&u32VsyncSHMOffset);
            if(u32SHMaddr != 0)  // TEE project
            {
                *param = u32SHMaddr+u32VsyncSHMOffset;
            }
            else  // normal project
            {
                *param = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32CodeBufAddr + u32VsyncSHMOffset;
            }
            eRet = E_VDEC_EX_OK;
            break;
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_GetVsyncBridgeExtAddr(VDEC_StreamId *pStreamId, MS_VIRT *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_VIRT u32SHMaddr = 0;
    MS_VIRT u32VsyncExtSHMOffset = 0;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MVD:
        case E_VDEC_EX_DECODER_MJPEG:
            if(E_MVD_RET_OK == MDrv_MVD_REE_GetVsyncExtShm(&u32SHMaddr,&u32VsyncExtSHMOffset))
            {

                if(u32SHMaddr != 0)  // TEE project
                {
                    *param = u32SHMaddr + u32VsyncExtSHMOffset;
                }
                else  // normal project
                {
                    *param = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32CodeBufAddr + u32VsyncExtSHMOffset;
                }
                eRet = E_VDEC_EX_OK;
            }
            break;
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Set ignore error reference
/// @param VDEC_EX_CODEC_CAP_INFO, get chip codec param from hal
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_GetCodecCapInfo( VDEC_EX_CODEC_CAP_INFO *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;

    MDrv_HVD_EX_GetCodecCapInfo( param->u32CodecType, param);
    return eRet;
}

static VDEC_EX_Result _VDEC_EX_GetNotSupportInfo(VDEC_StreamId *pStreamId, MS_U32 *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            *param = MDrv_HVD_EX_GetData(u32Id,E_HVD_EX_GDATA_TYPE_GET_NOT_SUPPORT_INFO);
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MVD:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_GetMinTspDataSize(VDEC_StreamId *pStreamId, MS_U32 *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    VDEC_EX_DispInfo DisplayInfo ;
    _VDEC_Memset(&DisplayInfo, 0, sizeof(VDEC_EX_DispInfo));

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            if(pVdecExContext->_Attr[u8Idx].eCodecType != E_VDEC_EX_CODEC_TYPE_H264  &&
               pVdecExContext->_Attr[u8Idx].eCodecType != E_VDEC_EX_CODEC_TYPE_AVS  &&
               pVdecExContext->_Attr[u8Idx].eCodecType != E_VDEC_EX_CODEC_TYPE_HEVC)
            {
                return E_VDEC_EX_RET_UNSUPPORTED;
            }
            *param = MDrv_HVD_EX_GetData(u32Id,E_HVD_EX_GDATA_TYPE_GET_MIN_TSP_DATA_SIZE);
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MVD:
            if(pVdecExContext->_Attr[u8Idx].eCodecType != E_VDEC_EX_CODEC_TYPE_MPEG2)
            {
                return E_VDEC_EX_RET_UNSUPPORTED;
            }
            *param = MDrv_MVD_EX_GetMinTspDataSize(u32Id);
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

// Allocate share memory from OS
// This function can't be called at the same time in dual decode
// Because there is share memory used in this function
static VDEC_EX_Result _VDEC_Init_Share_Mem(VDEC_EX_Stream eStreamType)
{
#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_LINUX_KERNEL) || defined(MSOS_TYPE_ECOS)
#if !defined(SUPPORT_X_MODEL_FEATURE)
    MS_U32 u32ShmId;
    MS_VIRT u32Addr;
    MS_U32 u32BufSize;

    if (FALSE == MsOS_SHM_GetId( (MS_U8*)"Linux VDEC driver",
                                          sizeof(VDEC_EX_CTX),
                                          &u32ShmId,
                                          &u32Addr,
                                          &u32BufSize,
                                          MSOS_SHM_QUERY))
    {
        if (FALSE == MsOS_SHM_GetId((MS_U8*)"Linux VDEC driver",
                                             sizeof(VDEC_EX_CTX),
                                             &u32ShmId,
                                             &u32Addr,
                                             &u32BufSize,
                                             MSOS_SHM_CREATE))
        {
            VDEC_PRINT("[%s]SHM allocation failed!!!use global structure instead !!!\n",__FUNCTION__);
            if(pVdecExContext == NULL)
            {
                pVdecExContext = &gVdecExContext;
                memset(pVdecExContext,0,sizeof(VDEC_EX_CTX));
                _VDEC_EX_Context_Init();
                VDEC_PRINT("[%s]Global structure init Success!!!\n",__FUNCTION__);
            }
            else
            {
                VDEC_PRINT("[%s]Global structure exists!!!\n",__FUNCTION__);
            }
            //return E_VDEC_EX_FAIL;
        }
        else
        {
            memset((MS_U8*)u32Addr, 0, sizeof(VDEC_EX_CTX));
            pVdecExContext = (VDEC_EX_CTX*)u32Addr; // for one process
            _VDEC_EX_Context_Init();
#if defined(CHIP_MONACO) || defined(CHIP_CLIPPERS) || defined(CHIP_MUJI) || defined(CHIP_MONET) || defined(CHIP_MANHATTAN) || defined(CHIP_KANO) || defined(CHIP_CURRY)

#ifdef DONT_USE_CMA //new cma , work for muji only.
            pVdecExContext->bCMAUsed = FALSE; // Disable in the future
#else
            pVdecExContext->bCMAUsed = TRUE; // enable in the future
#endif

#endif
        }
    }
    else
    {
        pVdecExContext = (VDEC_EX_CTX*)u32Addr; // for another process
        MS_PHY u32NonPMBankSize = 0;
        MS_VIRT u32RiuBaseAddr = 0;

        if (!MDrv_MMIO_GetBASE(&u32RiuBaseAddr, &u32NonPMBankSize, MS_MODULE_HW))
        {
            VDEC_PRINT("VDEC HVD Init Err: MMIO_GetBASE failure\n");
            return E_VDEC_EX_FAIL;
        }
        else
        {
            MDrv_HVD_EX_SetOSRegBase(u32RiuBaseAddr);
            MDrv_MVD_RegSetBase(u32RiuBaseAddr);
        }
    }
#else
    if(pVdecExContext == NULL)
    {
        pVdecExContext = &gVdecExContext;
        memset(pVdecExContext,0,sizeof(VDEC_EX_CTX));
        _VDEC_EX_Context_Init();
    }
#endif

    if(_VDEC_EX_API_MutexCreate() != TRUE)
    {
        VDEC_PRINT("[%s] API Mutex create failed!\n",__FUNCTION__);
        return E_VDEC_EX_FAIL;
    }
#else
    if(pVdecExContext == NULL)
    {
        pVdecExContext = &gVdecExContext;
        memset(pVdecExContext,0,sizeof(VDEC_EX_CTX));
        _VDEC_EX_Context_Init();
    }
#endif

#ifndef VDEC3
    _VDEC_EX_Context_Init_Function_Pointer(eStreamType);
#endif

#ifdef CMA_FW_INIT
    if (pVdecExContext->bCMAUsed)
    {
        pVdecExContext->cmaInitParam[0].heap_id = ION_VDEC_HEAP_ID; // 19
        pVdecExContext->cmaInitParam[0].flags = CMA_FLAG_MAP_VMA| CMA_FLAG_CACHED;

        if (MApi_CMA_Pool_Init(&pVdecExContext->cmaInitParam[0]) == FALSE)
        {
            return E_VDEC_EX_FAIL;
        }
        else
        {
            bCMAInitPool[0] = TRUE;

            VPRINTF("[VDEC][%d]MApi_CMA_Pool_Init[0]: pool_handle_id=%x, miu=%d, offset=%llx, length=%x\n",
                                (unsigned int)eStreamType,
                                (unsigned int)pVdecExContext->cmaInitParam[0].pool_handle_id,
                                (unsigned int)pVdecExContext->cmaInitParam[0].miu,
                                (unsigned long long int)pVdecExContext->cmaInitParam[0].heap_miu_start_offset,
                                (unsigned int)pVdecExContext->cmaInitParam[0].heap_length);
        }

        pVdecExContext->cmaInitParam[1].heap_id = ION_VDEC_HEAP_ID + 1; // 20
        pVdecExContext->cmaInitParam[1].flags = CMA_FLAG_MAP_VMA;

        if (MApi_CMA_Pool_Init(&pVdecExContext->cmaInitParam[1]) == FALSE)
        {
            VDEC_ERR("[Error]:%s, the 2nd heap of CMA doesn't exist\n", __FUNCTION__);
        }
        else
        {
            bCMAInitPool[1] = TRUE;
        }

        VPRINTF("MApi_CMA_Pool_Init[1]: pool_handle_id=0x%lx, miu=%ld, offset=0x%llx, length=0x%llx\n",
                                 pVdecExContext->cmaInitParam[1].pool_handle_id,
                                 pVdecExContext->cmaInitParam[1].miu,
                                 pVdecExContext->cmaInitParam[1].heap_miu_start_offset,
                                 pVdecExContext->cmaInitParam[1].heap_length);
    }
#endif

    if(MDrv_MVD_Init_Share_Mem() != E_MVD_RET_OK)
    {
        VDEC_PRINT("[%s]MVD SHM allocation failed!\n",__FUNCTION__);
        return E_VDEC_EX_FAIL;
    }

    if(MDrv_HVD_Init_Share_Mem() != E_HVD_EX_OK)
    {
        VDEC_PRINT("[%s]HVD SHM allocation failed!\n",__FUNCTION__);
        return E_VDEC_EX_FAIL;
    }

#ifdef CMA_FW_INIT
    if(pVdecExContext->bCMAUsed == TRUE && pVdecExContext->bCMAInit == FALSE)
    {
        // mvd add in here
        MDrv_MVD_SetCMAInformation(&(pVdecExContext->cmaInitParam[0]));

        // hvd add in here
        MDrv_HVD_EX_SetCMAInformation(pVdecExContext->cmaInitParam);
        #if defined(MSOS_TYPE_LINUX) && defined(SUPPORT_CMA)
        // mjpeg add in here
        MApi_MJPEG_SetCMAInformation(&(pVdecExContext->cmaInitParam[0]));
        #endif

        pVdecExContext->bCMAInit = TRUE;
    }
#endif

    return E_VDEC_EX_OK;

}

//------------------------------------------------------------------------------
/// Get CRC value
/// @param pCrcIn \b IN : the structure of generate CRC
/// @param pCrcOut \b OUT : CRC value
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_GetCrcValue(VDEC_StreamId *pStreamId, VDEC_EX_CrcValue *pCrcValue)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MVD_CrcIn stMvdCrcIn;
            MVD_CrcOut stMvdCrcOut;

            _VDEC_Memset(&stMvdCrcIn, 0, sizeof(MVD_CrcIn));
            _VDEC_Memset(&stMvdCrcOut, 0, sizeof(MVD_CrcOut));

            stMvdCrcIn.u32HSize    = pCrcValue->stCrcIn.u32HorSize;
            stMvdCrcIn.u32VSize    = pCrcValue->stCrcIn.u32VerSize;
            stMvdCrcIn.u32Strip     = pCrcValue->stCrcIn.u32Strip;
            stMvdCrcIn.u32YStartAddr      = pCrcValue->stCrcIn.u32LumaStartAddr;
            stMvdCrcIn.u32UVStartAddr      = pCrcValue->stCrcIn.u32ChromaStartAddr;

            _MVD_RET_HANDLE(MDrv_MVD_GetCrcValue(u32Id, &stMvdCrcIn, &stMvdCrcOut));

            pCrcValue->stCrcOut.u32LumaCRC = stMvdCrcOut.u32YCrc;
            pCrcValue->stCrcOut.u32ChromaCRC = stMvdCrcOut.u32UVCrc;

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MJPEG:
        {
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        }
        default:
        {
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
        }
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_GetBBUQNum(VDEC_StreamId *pStreamId, MS_U32 *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            *param = MDrv_MVD_GetSLQNum(u32Id);
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_HVD:
            *param = MDrv_HVD_EX_GetBBUQNum(u32Id);
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_GetDispFrmNum(VDEC_StreamId *pStreamId, MS_U32 *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if (param == NULL)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            *param = MDrv_HVD_EX_GetDispFrmNum(u32Id);
            break;
        case E_VDEC_EX_DECODER_MVD:
            *param = MDrv_MVD_GetDispQNum(u32Id);
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            _VDEC_EX_API_MutexLock();
            *param = MApi_MJPEG_GetDispFrmNum(u32Id);;
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            _VDEC_EX_API_MutexUnlock();
            break;
        default:
            *param = 0;
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }
    return eRet;
}

static VDEC_EX_Result _VDEC_EX_Set_External_DS_Buffer(VDEC_StreamId *pStreamId, VDEC_EX_EXTERNAL_DS_BUFFER* pExternalDSBuf)
{

#define SIZE_4K 0x1000
#define SIZE_3K 0xC00

    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if(pExternalDSBuf == NULL)
    {
        return E_VDEC_EX_RET_ILLEGAL_ACCESS;
    }

    /// DS buffer need more than 4K buffer.
    if(pExternalDSBuf->u32DSBufSize < SIZE_4K)
    {
        return E_VDEC_EX_RET_UNSUPPORTED;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MVD_EX_ExternalDSBuf stMVDExternalDSBuf;
            stMVDExternalDSBuf.u32DSBufAddr = pExternalDSBuf->u32DSBufAddr;
            stMVDExternalDSBuf.u32DSBufSize = SIZE_3K; //pExternalDSBuf->u32DSBufSize; /// MVD only use 3K for IP/OP buffer.
            ///VDEC_PRINT("[EDS] API HVD 0x%x, 0x%lx, 0x%lx.\n",stMVDExternalDSBuf.bEnable,stMVDExternalDSBuf.u32DSBufAddr,stMVDExternalDSBuf.u32DSBufSize);
            _MVD_RET_HANDLE(MDrv_MVD_SetExternalDSBuffer(u32Id,&stMVDExternalDSBuf));
            eRet = E_VDEC_EX_OK;
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            HVD_EX_ExternalDSBuf stExternalDSBuf;
            stExternalDSBuf.u32DSBufAddr = pExternalDSBuf->u32DSBufAddr;
            stExternalDSBuf.u32DSBufSize = SIZE_3K;// pExternalDSBuf->u32DSBufSize;
            ///VDEC_PRINT("[EDS] API HVD 0x%x, 0x%lx, 0x%lx.\n",stExternalDSBuf.bEnable,stExternalDSBuf.u32DSBufAddr,stExternalDSBuf.u32DSBufSize);
            _HVD_RET_HANDLE(MDrv_HVD_EX_SetExternalDSBuffer(u32Id,&stExternalDSBuf));
            eRet = E_VDEC_EX_OK;
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_GetESBufferStatus(VDEC_StreamId *pStreamId, MS_U32 *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if (param == NULL)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
        {
            MS_U32 u32ESBufStatus;

            u32ESBufStatus =  MDrv_HVD_EX_GetESBufferStatus(u32Id);
            *param = _VDEC_Map2ESBufStatus(u32ESBufStatus);
            if(*param != ES_BUFFER_STATUS_UNKNOWN)
            {
                eRet = E_VDEC_EX_OK;
            }
            break;
        }
        case E_VDEC_EX_DECODER_MVD:
            *param = MDrv_MVD_GetESBufferStatus(u32Id);
            if(*param != ES_BUFFER_STATUS_UNKNOWN)
            {
                eRet = E_VDEC_EX_OK;
            }
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            *param = 0;
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            *param = 0;
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }
    return eRet;
}

static VDEC_EX_Result _VDEC_EX_SetDbgLevel(VDEC_StreamId *pStreamId, VDEC_EX_DbgLevel eDbgLevel)
{
    MS_U8 u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if (E_VDEC_EX_DBG_LEVEL_FW == eDbgLevel)
    {
        MS_PHY u32NonPMBankSize=0;
        MS_VIRT u32RiuBaseAdd=0;
        _RET_VDEC_HVD_LINT_CHECK();

        if (!MDrv_MMIO_GetBASE(&u32RiuBaseAdd, &u32NonPMBankSize, MS_MODULE_HW))
        {
            VDEC_PRINT("VDEC HVD MApi_VDEC_EX_SetDbgLevel Err: MMIO_GetBASE failure\n");
            return E_VDEC_EX_FAIL;
        }
        else
        {
            //VDEC_PRINT("HVD:1 u32RiuBaseAdd = %lx\n", u32RiuBaseAdd);
            MDrv_HVD_EX_SetOSRegBase(u32RiuBaseAdd);
        }

        MDrv_HVD_EX_SetDbgLevel(u32Id, E_HVD_EX_UART_LEVEL_FW);

        return E_VDEC_EX_OK;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _RET_VDEC_MVD_LINT_CHECK();
            MDrv_MVD_SetDbgLevel(eDbgLevel);

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _RET_VDEC_HVD_LINT_CHECK();
            if (E_VDEC_EX_DBG_LEVEL_NONE == eDbgLevel)
            {
                MDrv_HVD_EX_SetDbgLevel(u32Id, E_HVD_EX_UART_LEVEL_NONE);
            }
            if (E_VDEC_EX_DBG_LEVEL_ERR == eDbgLevel)
            {
                MDrv_HVD_EX_SetDbgLevel(u32Id, E_HVD_EX_UART_LEVEL_ERR);
            }
            else if (E_VDEC_EX_DBG_LEVEL_INFO == eDbgLevel)
            {
                MDrv_HVD_EX_SetDbgLevel(u32Id, E_HVD_EX_UART_LEVEL_INFO);
            }
            else if (E_VDEC_EX_DBG_LEVEL_DBG == eDbgLevel)
            {
                MDrv_HVD_EX_SetDbgLevel(u32Id, E_HVD_EX_UART_LEVEL_DBG);
            }
            else if (E_VDEC_EX_DBG_LEVEL_TRACE == eDbgLevel)
            {
                MDrv_HVD_EX_SetDbgLevel(u32Id, E_HVD_EX_UART_LEVEL_TRACE);
            }

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            MApi_MJPEG_DbgSetMsgLevel((MJPEG_DbgLevel)eDbgLevel);
            _VDEC_EX_API_MutexUnlock();

            break;
        }
        default:
            break;
    }

    return E_VDEC_EX_OK;
}

extern void HAL_HVD_EX_ExchangeCidx(MS_U32);
extern void HAL_VPU_EX_ExchangeCidx(MS_U32);
#ifdef VDEC3
extern void HAL_HVD_EX_SetCidx(MS_U32, MS_BOOL, MS_BOOL);
extern void HAL_VPU_EX_SetCidx(MS_U32, MS_BOOL, MS_BOOL);
#else
extern void HAL_HVD_EX_SetCidx(MS_U32, MS_BOOL);
extern void HAL_VPU_EX_SetCidx(MS_U32, MS_BOOL);
#endif
extern MS_U8 HAL_HVD_EX_GetCidx(MS_U32);

#if VDEC_EXCHANGE_CIDX_PATCH
static VDEC_EX_Result _VDEC_EX_ExchangeCidx(VDEC_StreamId *pStreamId)
{
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    HAL_HVD_EX_ExchangeCidx(u32Id);
    HAL_VPU_EX_ExchangeCidx(u32Id);

    return E_VDEC_EX_OK;
}
#endif

#ifdef VDEC3
static VDEC_EX_Result _VDEC_EX_SetCidx(VDEC_StreamId *pStreamId, MS_BOOL bIsEVD, MS_BOOL bIsNStreamMode)
#else
static VDEC_EX_Result _VDEC_EX_SetCidx(VDEC_StreamId *pStreamId, MS_BOOL bIsEVD)
#endif
{
    MS_U8 u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
#ifdef VDEC3
            HAL_HVD_EX_SetCidx(u32Id, FALSE, bIsNStreamMode);
            HAL_VPU_EX_SetCidx(u32Id, FALSE, bIsNStreamMode);
#else
            HAL_HVD_EX_SetCidx(u32Id, FALSE);
            HAL_VPU_EX_SetCidx(u32Id, FALSE);
#endif
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
#ifdef VDEC3
            HAL_HVD_EX_SetCidx(u32Id, bIsEVD, bIsNStreamMode);
            HAL_VPU_EX_SetCidx(u32Id, bIsEVD, bIsNStreamMode);
#else
            HAL_HVD_EX_SetCidx(u32Id, bIsEVD);
            HAL_VPU_EX_SetCidx(u32Id, bIsEVD);
#endif
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
#ifdef VDEC3
            HAL_HVD_EX_SetCidx(u32Id, FALSE, FALSE);
            HAL_VPU_EX_SetCidx(u32Id, FALSE, FALSE);
#else
            HAL_HVD_EX_SetCidx(u32Id, FALSE);
            HAL_VPU_EX_SetCidx(u32Id, FALSE);
#endif
            break;
        }
        default:
            break;
    }

    return E_VDEC_EX_OK;
}

static VDEC_EX_Result _VDEC_EX_GetCidx(VDEC_StreamId *pStreamId, MS_U8* u8Cidx)
{
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    *u8Cidx = HAL_HVD_EX_GetCidx(u32Id);
    return E_VDEC_EX_OK;
}

extern MS_BOOL HAL_VPU_EX_NotSupportDS(void);
static VDEC_EX_Result _VDEC_EX_GetCaps(VDEC_EX_CapCmd cmd_id, MS_U32* param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;

#if 0
    switch (cmd_id)
    {
        case E_VDEC_EX_CAP_XC_NOT_SUPPORT_DS:
            *param = HAL_VPU_EX_NotSupportDS();
            eRet = E_VDEC_EX_OK;
            break;
        default:
            eRet = E_VDEC_EX_RET_INVALID_PARAM
            break;
    }

#else
    if(cmd_id == E_VDEC_EX_CAP_XC_NOT_SUPPORT_DS)
    {
        *param = HAL_VPU_EX_NotSupportDS();
        eRet = E_VDEC_EX_OK;
    }
#endif
    return eRet;
}

//------------------------------------------------------------------------------
/// Enable to let decoder force to drop error frame
/// @param bEnable \b IN : TRUE - enable; FALSE - disable
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _VDEC_EX_DropErrFrame(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            if (FALSE == MDrv_MVD_DropErrorFrame(u32Id, bEnable))
            {
                eRet = E_VDEC_EX_FAIL;
            }
            pVdecExContext->bDropErrFrm[u8Idx] = bEnable;
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            if(MDrv_HVD_EX_SetDispErrFrm(u32Id, !bEnable) != E_HVD_EX_OK)
            {
                eRet = E_VDEC_EX_FAIL;
            }
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_GetFWCodecType(VDEC_StreamId *pStreamId, MS_U32 *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
        {
            if ( MDrv_HVD_EX_CheckDispInfoRdy(u32Id) == E_HVD_EX_OK)
            {
                *param = MDrv_HVD_EX_GetData(u32Id, E_HVD_EX_GDATA_TYPE_FW_CODEC_TYPE);
                switch (*param)
                {
                    case E_HVD_EX_AVC:
                    {
                        *param = E_VDEC_EX_CODEC_TYPE_H264;
                        break;
                    }
                    case E_HVD_EX_AVS:
                    {
                        *param = E_VDEC_EX_CODEC_TYPE_AVS;
                        break;
                    }
                    case E_HVD_EX_RM:
                    {
                        *param = E_VDEC_EX_CODEC_TYPE_RV9;
                        break;
                    }
                    case E_HVD_EX_MVC:
                    {
                        *param = E_VDEC_EX_CODEC_TYPE_MVC;
                        break;
                    }
                    case E_HVD_EX_VP8:
                    {
                        *param = E_VDEC_EX_CODEC_TYPE_VP8;
                        break;
                    }
                    case E_HVD_EX_HEVC:
                    {
                        *param = E_VDEC_EX_CODEC_TYPE_HEVC;
                        break;
                    }
                    case E_HVD_EX_NONE:
                    default:
                    {
                        *param = E_VDEC_EX_CODEC_TYPE_NONE;
                        break;
                    }
                }

                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_RET_NOT_READY;
            }
            break;
        }
        case E_VDEC_EX_DECODER_MVD:
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_Enable_External_CC_608_Buffer(VDEC_StreamId *pStreamId, MS_VIRT u32_ccinfo)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    MS_VIRT u32Buffer_Addr = (u32_ccinfo >> 8);//unit is kb
    MS_U8  u8Buffer_Size= u32_ccinfo & 0xFF;

    if(u32_ccinfo==0)
    VDEC_PRINT("\nIt will turn off this feature.......\n");

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {

        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetExternal_CC608_Buffer(u32Id,u32Buffer_Addr,u8Buffer_Size));
             eRet = E_VDEC_EX_OK;
             break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_SetExternal_CC608_Buffer(u32Id,u32Buffer_Addr,u8Buffer_Size));
             eRet = E_VDEC_EX_OK;
             break;

        case E_VDEC_EX_DECODER_MJPEG:
        default:
             eRet = E_VDEC_EX_RET_UNSUPPORTED;
             break;
    }
    return eRet;
}

static VDEC_EX_Result _VDEC_EX_Enable_External_CC_708_Buffer(VDEC_StreamId *pStreamId, MS_VIRT u32_ccinfo)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    MS_VIRT u32Buffer_Addr = (u32_ccinfo >> 8);//unit is kb
    MS_U8  u8Buffer_Size= u32_ccinfo & 0xFF;

    if(u32_ccinfo==0)
    VDEC_PRINT("\nIt will turn off this feature.......\n");

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {

        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetExternal_CC708_Buffer(u32Id,u32Buffer_Addr,u8Buffer_Size));
             eRet = E_VDEC_EX_OK;
             break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_SetExternal_CC708_Buffer(u32Id,u32Buffer_Addr,u8Buffer_Size));
             eRet = E_VDEC_EX_OK;
             break;

        case E_VDEC_EX_DECODER_MJPEG:
        default:
             eRet = E_VDEC_EX_RET_UNSUPPORTED;
             break;
    }
    return eRet;

}

#if VDEC_DTV_DIRECT_STC_PATCH
#include "apiDMX.h"
static void _Feed_Direct_STC(MS_U32 u32streamIdx)
{
    MS_U8  u8Idx = _VDEC_GET_IDX(u32streamIdx);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(u32streamIdx);
    MS_U32 u32High = 0;
    MS_U32 u32Low = 0;
    MS_U64 u64STC = 0;
    MS_U8  u8STCSource = u8Idx;  //main use tsp0, sub use tsp1
    pVdecExContext->bDirectStcThreadRunning[u8Idx] = TRUE;
    VDEC_PRINT("[%d] start of _Feed_Direct_STC \n", MsOS_GetSystemTime());

    while(pVdecExContext->bDirectStcThreadRunning[u8Idx])
    {
        MApi_DMX_Stc_Eng_Get(u8STCSource, &u32High, &u32Low);
        u64STC = ((((MS_U64) u32High) << 32) | u32Low) / 90;
        #if 0
        MS_U64 u64PCR = 0;
        MApi_DMX_Pcr_Eng_Get(u8STCSource, &u32High, &u32Low);
        u64PCR = ((((MS_U64) u32High) << 32) | u32Low) / 90;
        #endif

        MDrv_HVD_EX_DirectSTCMode(u32Id, u64STC);
        //VDEC_PRINT("[%d] id=%d, STC=%lld \n", MsOS_GetSystemTime(), u8Idx, u64STC);
        MsOS_DelayTask(12);
    }

    VDEC_PRINT("[%d] end of _Feed_Direct_STC \n", MsOS_GetSystemTime());
}

static MS_BOOL _VDEC_EX_Create_DirectSTC_Task(MS_U32 u32streamIdx)  //0x00004040, 0x01014141
{
    MS_U8  u8Idx = _VDEC_GET_IDX(u32streamIdx);

    VDEC_PRINT("%s \n", __FUNCTION__);

    if (s32VdecDirectStcId[u8Idx] == -1)
    {
        s32VdecDirectStcId[u8Idx] = MsOS_CreateTask((TaskEntry)_Feed_Direct_STC,
                                                 u32streamIdx,
                                                 E_TASK_PRI_MEDIUM,
                                                 TRUE,
                                                 (void *)u8VdecDirectSTCStack,
                                                 VDEC_EX_STACK_SIZE,
                                                 "VdecFeedDirectSTC");
        if (s32VdecDirectStcId[u8Idx] < 0)
        {
            VDEC_PRINT("[%s][%d] MsOS_CreateTask failed!!\n", __FUNCTION__, __LINE__);
            return FALSE;
        }
    }

    VDEC_PRINT("create thread VdecFeedDirectSTC, id=%d, pid=%d    \033[0m\n", u8Idx, s32VdecDirectStcId[u8Idx]);

    return TRUE;
}

static void _VDEC_EX_Delete_DirectSTC_Task(MS_U32 u32streamIdx)
{
    MS_U8  u8Idx = _VDEC_GET_IDX(u32streamIdx);

    VDEC_PRINT("%s \n", __FUNCTION__);

    pVdecExContext->bDirectStcThreadRunning[u8Idx] = FALSE;
    MsOS_DelayTask(24);

    if (s32VdecDirectStcId[u8Idx] != -1)
    {
        //MsOS_DeleteTask(s32VdecDirectStcId[u8Idx]);
        VDEC_PRINT("delete thread VdecFeedDirectSTC, id=%d, pid=%d \n", u8Idx, s32VdecDirectStcId[u8Idx]);
        s32VdecDirectStcId[u8Idx] = -1;
    }

    return;
}
#endif



#if 1//#ifdef VDEC_UTOPIA_2K

typedef struct
{
    MS_BOOL bRunning;
    MS_U8  u8Dbglevl;
    VDEC_StreamId StreamId;
    MS_U32 u32EventMap[E_VDEC_EX_CB_MAX_NUM];
    VDEC_EX_EventCb pfn[E_VDEC_EX_CB_MAX_NUM];
    void *param[E_VDEC_EX_CB_MAX_NUM];
}VDEC_EX_2k_CbData;

VDEC_EX_2k_CbData gVDEC_EX_2k_CbData[VDEC_MAX_SUPPORT_STREAM_NUM];



void _VDEC_Callback_Proc(MS_U32 u32streamIdx)
{
    MS_U32 u32VdecEventFlag = 0;
    MS_U32 u8Idx = 0;
    VDEC_StreamId* pStreamId;
    MS_U32 cb_type = 0;
    VDEC_EX_V2_IO_Param IO_arg;
    memset((void*)(&IO_arg),0,sizeof(VDEC_EX_V2_IO_Param));
    VDEC_EX_Result ret = E_VDEC_EX_FAIL;
    MS_U32 u32Event = 0;
    for(u8Idx=0;u8Idx<VDEC_MAX_SUPPORT_STREAM_NUM;u8Idx++)
    {
        pStreamId = &(gVDEC_EX_2k_CbData[u8Idx].StreamId);

        if(pStreamId->u32Id == u32streamIdx)
            break;
    }

    if(u8Idx >= VDEC_MAX_SUPPORT_STREAM_NUM) //CID 47265
    {
        VPRINTF("[Fail][%s,%d]Out of bound error\n",__FUNCTION__,__LINE__);
        return ;
    }

    while(gVDEC_EX_2k_CbData[u8Idx].bRunning)
    {
        MsOS_DelayTaskUs(1000);
        if (pu32VDEC_EX_Inst == NULL)
        {
            VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
            return ;
        }
        if(gVDEC_EX_2k_CbData[u8Idx].u8Dbglevl == 2)
        {
            VDEC_PRINT("EventMap:0x%x\n",gVDEC_EX_2k_CbData[u8Idx].u32EventMap[0]);
        }
        if(gVDEC_EX_2k_CbData[u8Idx].u32EventMap[0]||gVDEC_EX_2k_CbData[u8Idx].u32EventMap[1])
        {
            u32VdecEventFlag = 0;
            IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
            IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_EVENT_FLAG;
            IO_arg.param[0] = (void*)(&u32VdecEventFlag);
            IO_arg.param[1] = (void*)(&cb_type);
            IO_arg.pRet     = (void*)(&ret);

            if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
            {
                VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
            }
            for(cb_type=0;cb_type<E_VDEC_EX_CB_MAX_NUM;cb_type++)
            {
                u32Event = u32VdecEventFlag & gVDEC_EX_2k_CbData[u8Idx].u32EventMap[cb_type];
                if(gVDEC_EX_2k_CbData[u8Idx].u8Dbglevl == 2)
                {
                    VDEC_PRINT("Find event:0x%x\n",u32Event);
                }
                if (gVDEC_EX_2k_CbData[u8Idx].pfn[cb_type] && u32Event)
                {
                    gVDEC_EX_2k_CbData[u8Idx].pfn[cb_type](u32Event, gVDEC_EX_2k_CbData[u8Idx].param[cb_type]);
                }
            }
        }
    }
}

MS_BOOL _VDEC_EX_Create_Callback_task(VDEC_StreamId *pStreamId, VDEC_EX_CB_TYPE cb_type, VDEC_EX_EventCb pfn, void *param, MS_U32 u32EventFlag)
{
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);

    memcpy(&gVDEC_EX_2k_CbData[u8Idx].StreamId,pStreamId,sizeof(VDEC_StreamId));
    gVDEC_EX_2k_CbData[u8Idx].param[cb_type] = param;
    gVDEC_EX_2k_CbData[u8Idx].pfn[cb_type] = pfn;
    gVDEC_EX_2k_CbData[u8Idx].u32EventMap[cb_type] = u32EventFlag;
    gVDEC_EX_2k_CbData[u8Idx].bRunning = TRUE;

    //char task_name[100] = {0,};
    //snVDEC_PRINT(task_name,sizeof(task_name),"VDEC_Callback_Task_%d",u8Idx);

    if(_s32VDEC_Callback_TaskId[u8Idx] == -1 )
    {
        _s32VDEC_Callback_TaskId[u8Idx] = MsOS_CreateTask((TaskEntry)_VDEC_Callback_Proc,
                                              (MS_U32)(pStreamId->u32Id),
                                              E_TASK_PRI_MEDIUM,
                                              TRUE,
                                              (void*)u8VdecExCallbackStack[u8Idx],
                                              VDEC_EX_STACK_SIZE,
                                              cbTaskName[u8Idx]);

        if (_s32VDEC_Callback_TaskId[u8Idx] < 0)
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

void _VDEC_EX_UnSetEvent(VDEC_StreamId *pStreamId, VDEC_EX_CB_TYPE cb_type, MS_U32 u32EventFlag)
{
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    if (E_VDEC_EX_EVENT_OFF != u32EventFlag)
    {
        gVDEC_EX_2k_CbData[u8Idx].u32EventMap[cb_type] &= ~u32EventFlag ;
    }
}

void _VDEC_EX_Delete_Callback_Task(VDEC_StreamId *pStreamId)
{
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    int i;
    MS_BOOL eRet = FALSE;

    for(i=0;i<E_VDEC_EX_CB_MAX_NUM;i++)
    {

        gVDEC_EX_2k_CbData[u8Idx].u32EventMap[i] = 0;
    }

    if(_s32VDEC_Callback_TaskId[u8Idx]!=-1)
    {
        gVDEC_EX_2k_CbData[u8Idx].bRunning = FALSE;
        eRet = MsOS_DeleteTask(_s32VDEC_Callback_TaskId[u8Idx]);
        if(TRUE == eRet)
        {
            _s32VDEC_Callback_TaskId[u8Idx] = -1;
        }
    }
}
#endif



VDEC_EX_Result _MApi_VDEC_EX_ReturnCMABuffer(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);

#ifdef CMA_DRV_DIRECT_INIT
    MS_BOOL bNDecAllocator = _VDEC_EX_NdecAllocator_IsNDecode();//VDEC_EX_ALLOCATOR_ENABLE;

    if(bNDecAllocator == FALSE)
    {
        #if VDEC_DUAL_FRAME_BUFFER_MANAGEMENT
        MS_BOOL bReturnCMABuffer = TRUE;
        MS_U8 i;
        MS_U8 u8StreamCnt = 0;
        for (i=0; i<VDEC_MAX_SUPPORT_STREAM_NUM; i++)
        {
            if (TRUE == pVdecExContext->_Attr[i].bUsed)
            {
                u8StreamCnt++;
            }
        }
        if (u8StreamCnt > 1)
        {
            bReturnCMABuffer = FALSE;
        }
        #endif

        #if VDEC_DUAL_FRAME_BUFFER_MANAGEMENT
        if (pVdecExContext->bCMAUsed && bReturnCMABuffer)
        #else
        if (pVdecExContext->bCMAUsed && u8Idx == 0)
        #endif
        {
            if (pVdecExContext->bCMAGetMem[0][u8Idx])
            {
                if (MApi_CMA_Pool_PutMem(&pVdecExContext->cmaFreeParam[0][u8Idx]) == FALSE)
                {
                    VPRINTF("[VDEC][%d]MApi_CMA_Pool_PutMem0 fail in vdec: cmaFreeParam.pool_handle_id=%x, cmaFreeParam.offset_in_pool=%llx, cmaFreeParam.length=%x\n",
                                    (unsigned int)u8Idx,
                                    (unsigned int)pVdecExContext->cmaFreeParam[0][u8Idx].pool_handle_id,
                                    (unsigned long long int)pVdecExContext->cmaFreeParam[0][u8Idx].offset_in_pool,
                                    (unsigned int)pVdecExContext->cmaFreeParam[0][u8Idx].length);
                    return E_VDEC_EX_FAIL;
                }
                else
                {
                    pVdecExContext->bCMAGetMem[0][u8Idx] = FALSE;
                    VPRINTF("[VDEC][%d]MApi_CMA_Pool_PutMem0 in vdec: cmaFreeParam.pool_handle_id=%x, cmaFreeParam.offset_in_pool=%llx, cmaFreeParam.length=%x\n",
                                    (unsigned int)u8Idx,
                                    (unsigned int)pVdecExContext->cmaFreeParam[0][u8Idx].pool_handle_id,
                                    (unsigned long long int)pVdecExContext->cmaFreeParam[0][u8Idx].offset_in_pool,
                                    (unsigned int)pVdecExContext->cmaFreeParam[0][u8Idx].length);
                }
            }

            if (pVdecExContext->bCMAGetMem[1][u8Idx])
            {
                if (MApi_CMA_Pool_PutMem(&pVdecExContext->cmaFreeParam[1][u8Idx]) == FALSE)
                {
                    VPRINTF("[VDEC][%d]MApi_CMA_Pool_PutMem1 fail in vdec: cmaFreeParam.pool_handle_id=%x, cmaFreeParam.offset_in_pool=%llx, cmaFreeParam.length=%x\n",
                                    (unsigned int)u8Idx,
                                    (unsigned int)pVdecExContext->cmaFreeParam[0][u8Idx].pool_handle_id,
                                    (unsigned long long int)pVdecExContext->cmaFreeParam[0][u8Idx].offset_in_pool,
                                    (unsigned int)pVdecExContext->cmaFreeParam[0][u8Idx].length);
                    return E_VDEC_EX_FAIL;
                }
                else
                {
                    pVdecExContext->bCMAGetMem[1][u8Idx] = FALSE;
                    VPRINTF("[VDEC][%d]MApi_CMA_Pool_PutMem1 in vdec: cmaFreeParam.pool_handle_id=%x, cmaFreeParam.offset_in_pool=%llx, cmaFreeParam.length=%x\n",
                                    (unsigned int)u8Idx,
                                    (unsigned int)pVdecExContext->cmaFreeParam[1][u8Idx].pool_handle_id,
                                    (unsigned long long int)pVdecExContext->cmaFreeParam[1][u8Idx].offset_in_pool,
                                    (unsigned int)pVdecExContext->cmaFreeParam[1][u8Idx].length);
                }
            }
        }
    }
    else
#endif
    {
        _VDEC_EX_NdecAllocator_FreeBuffer(u8Idx, E_VDEC_EX_ALLOCATOR_BUFFER_FB1);
        _VDEC_EX_NdecAllocator_FreeBuffer(u8Idx, E_VDEC_EX_ALLOCATOR_BUFFER_FB2);
        _VDEC_EX_NdecAllocator_FreeBuffer(u8Idx, E_VDEC_EX_ALLOCATOR_BUFFER_BS);
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_GetFbMemUsageSize(VDEC_EX_FbMemUsage_Param *pParam)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;

    switch (_VDEC_EX_GetDecoderByCodecType(pParam->eCodecType))
    {
        case E_VDEC_EX_DECODER_HVD:
        {
            HVD_EX_FbMemUsage_Param info;

            _VDEC_Memset(&info, 0, sizeof(HVD_EX_FbMemUsage_Param));

            info.eCodecType = _VDEC_EX_Map2HVDCodecType(pParam->eCodecType);
            info.u32DataVAddr = MS_PA2KSEG1(pParam->u32DataAddr);
            info.u16DataSize = pParam->u16DataSize;

            if(MDrv_HVD_EX_GetFbMemUsageSize(&info) == E_HVD_EX_OK)
            {
                pParam->u32MemUsageSize = info.u32MemUsageSize;
                eRet = E_VDEC_EX_OK;
            }
        }
        break;

        default:
             eRet = E_VDEC_EX_RET_UNSUPPORTED;
             break;
    }
    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_Exit(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

#if (VDEC_EX_ISR_MONITOR)
    pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] = E_VDEC_EX_EVENT_ISR_EVENT_CHANGE;
    MsOS_SetEvent(s32VdecExEventId[u8Idx], E_VDEC_EX_EVENT_ISR_EVENT_CHANGE);
#else
    pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] = 0;
#endif

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_Exit(u32Id));
            if(eRet == E_VDEC_EX_OK)
            {
                pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_STOP;
            }
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _VDEC_EX_Delete_BBU_Task(pStreamId->u32Id);
            #if VDEC_DTV_DIRECT_STC_PATCH
            _VDEC_EX_Delete_DirectSTC_Task(pStreamId->u32Id);
            #endif
            _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_Exit(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            MDrv_HVD_EX_SetBurstMode(u32Id, FALSE);
            MDrv_HVD_EX_MJPEG_Exit(u32Id);
            _MJPEG_RET_TO_VDEC_RESULT(eRet, MApi_MJPEG_Stop(u32Id));
            if(eRet == E_VDEC_EX_OK)
            {
                pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_STOP;
            }
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            break;
    }

    _VDEC_EX_API_MutexLock();

    if(pVdecExContext->_power_state[u8Idx] != E_VDEC_EX_POWER_SUSPEND_ING)
    {
        _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
    }

    //When Exit, we need to re-init attributes of stream info
    _VDEC_EX_InitLocalVar(u8Idx);

    _VDEC_Memset((void*) &pVdecExContext->_Pre_Ctrl[u8Idx], 0, sizeof(VDEC_Pre_Ctrl));

    pVdecExContext->_Attr[u8Idx].bUsed = FALSE;
    pVdecExContext->bTrickMode[u8Idx] = FALSE;

    MS_U8 i;
    for (i=0; i<VDEC_MAX_SUPPORT_STREAM_NUM; i++)
    {
        if (TRUE == pVdecExContext->_Attr[i].bUsed) break;
    }
    if (i == VDEC_MAX_SUPPORT_STREAM_NUM)
    {
        _VDEC_Memset((void*) &pVdecExContext->_Pre_DecModeCfg, 0, sizeof(VDEC_EX_DecModCfg));

        pVdecExContext->bFWdecideFB = FALSE;
        pVdecExContext->bDRVdecideBS = FALSE;
    }

    pVdecExContext->_Attr[u8Idx].u32DrvId = 0;
    pVdecExContext->bEnableAutoInsertDummyPattern[u8Idx] = FALSE;
#ifdef VDEC3
    pVdecExContext->u32FrameBaseMode[u8Idx] = FALSE;
#endif
    pVdecExContext->bDirectStcThreadRunning[u8Idx] = FALSE;

    pVdecExContext->_bSingleDecodeMode = FALSE;
    _VDEC_EX_API_MutexUnlock();

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_Rst(VDEC_StreamId *pStreamId)
{
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            if(MDrv_MVD_Rst(u32Id,_VDEC_EX_Map2MVDCodecType(pVdecExContext->_Attr[u8Idx].eCodecType)) != E_MVD_RET_OK)
            {
                return E_VDEC_EX_FAIL;
            }

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            if(MDrv_HVD_EX_Rst(u32Id, FALSE) != E_HVD_EX_OK)
            {
                return E_VDEC_EX_FAIL;
            }

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();

            if(MApi_MJPEG_Rst(u32Id) != E_MJPEG_RET_SUCCESS)
            {
                _VDEC_EX_API_MutexUnlock();
                return E_VDEC_EX_FAIL;
            }
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            break;
    }

    return E_VDEC_EX_OK;
}


VDEC_EX_Result _MApi_VDEC_EX_V2_CheckDispInfoRdy(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            if (0 == MDrv_MVD_GetDispRdy(u32Id))
            {
                eRet = E_VDEC_EX_RET_NOT_READY;
            }
            break;

        case E_VDEC_EX_DECODER_HVD:
        {
            HVD_EX_Result ret = MDrv_HVD_EX_CheckDispInfoRdy(u32Id);

            if (E_HVD_EX_OK != ret)
            {
                if (E_HVD_EX_RET_NOTREADY == ret)
                {
                    eRet = E_VDEC_EX_RET_NOT_READY;
                }
                else if( ret == E_HVD_EX_RET_UNSUPPORTED)
                {
                    eRet =  E_VDEC_EX_RET_UNSUPPORTED;
                }
                else
                {
                    eRet = E_VDEC_EX_FAIL;
                }
            }

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            if (E_MJPEG_RET_SUCCESS != MApi_MJPEG_CheckDispInfoRdy(u32Id))
            {
                eRet = E_VDEC_EX_FAIL;
            }
            _VDEC_EX_API_MutexUnlock();

            break;
        }
        default:
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_SetFrcMode(VDEC_StreamId *pStreamId, VDEC_EX_FrcMode eFrcMode)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MVD_FrcMode eMvdFrcMode = E_MVD_FRC_NORMAL;

            switch(eFrcMode)
            {
                case E_VDEC_EX_FRC_NORMAL:
                    eMvdFrcMode = E_MVD_FRC_NORMAL;
                    break;
                case E_VDEC_EX_FRC_DISP_TWICE:
                    eMvdFrcMode = E_MVD_FRC_DISP_TWICE;
                    break;
                case E_VDEC_EX_FRC_3_2_PULLDOWN:
                    eMvdFrcMode = E_MVD_FRC_3_2_PULLDOWN;
                    break;
                case E_VDEC_EX_FRC_PAL_TO_NTSC:
                    eMvdFrcMode = E_MVD_FRC_PAL_TO_NTSC;
                    break;
                case E_VDEC_EX_FRC_NTSC_TO_PAL:
                    eMvdFrcMode = E_MVD_FRC_NTSC_TO_PAL;
                    break;
                default:
                    eRet = E_VDEC_EX_RET_INVALID_PARAM;
                    break;
            }

            if(E_VDEC_EX_RET_INVALID_PARAM != eRet)
            {
                eRet = MDrv_MVD_DispCtrl(u32Id, FALSE, pVdecExContext->bDropErrFrm[u8Idx], FALSE, eMvdFrcMode);
            }
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            HVD_EX_FrmRateConvMode eHvdFrcMode = E_HVD_EX_FRC_MODE_NORMAL;

            switch(eFrcMode)
            {
                case E_VDEC_EX_FRC_NORMAL:
                    eHvdFrcMode = E_HVD_EX_FRC_MODE_NORMAL;
                    break;
                case E_VDEC_EX_FRC_DISP_TWICE:
                    eHvdFrcMode = E_HVD_EX_FRC_MODE_DISP_2X;
                    break;
                case E_VDEC_EX_FRC_3_2_PULLDOWN:
                    eHvdFrcMode = E_HVD_EX_FRC_MODE_32PULLDOWN;
                    break;
                case E_VDEC_EX_FRC_PAL_TO_NTSC:
                    eHvdFrcMode = E_HVD_EX_FRC_MODE_PAL2NTSC;
                    break;
                case E_VDEC_EX_FRC_NTSC_TO_PAL:
                    eHvdFrcMode = E_HVD_EX_FRC_MODE_NTSC2PAL;
                    break;
                case E_VDEC_EX_FRC_MODE_50P_60P:
                    eHvdFrcMode = E_HVD_EX_FRC_MODE_50P_60P;
                    break;
                case E_VDEC_EX_FRC_MODE_60P_50P:
                    eHvdFrcMode = E_HVD_EX_FRC_MODE_60P_50P;
                    break;
                default:
                    eRet = E_VDEC_EX_RET_INVALID_PARAM;
                    break;
            }

            if(E_VDEC_EX_RET_INVALID_PARAM != eRet)
            {
                eRet = MDrv_HVD_EX_SetFrcMode(u32Id, eHvdFrcMode);
            }
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            eRet = E_VDEC_EX_RET_INVALID_PARAM;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_SetDynScalingParams(VDEC_StreamId *pStreamId, MS_PHY u32Addr, MS_U32 u32Size)
{
    VDEC_EX_Result eRet = E_VDEC_EX_RET_NOT_INIT;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_TO_VDEC_RESULT(eRet, MDrv_MVD_SetDynScalingParam(u32Id, u32Addr, u32Size));
            break;

        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetDynScalingParam(u32Id, (void*)MS_PA2KSEG1(u32Addr),u32Size));
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_OK;
            break;

        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }
    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_Play(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MDrv_MVD_Play(u32Id);
            pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_PLAY;
            eRet = E_VDEC_EX_OK;
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_Play(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _MJPEG_RET_TO_VDEC_RESULT(eRet, MApi_MJPEG_Play(u32Id));
            if(eRet == E_VDEC_EX_OK)
            {
                pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_PLAY;
            }
            else
            {
                pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_STOP;
            }
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            break;
    }

    pVdecExContext->u8enable_store_set_cmd[u8Idx] = FALSE;

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_Pause(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MDrv_MVD_Pause(u32Id);
            pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_PAUSE;
            eRet = E_VDEC_EX_OK;
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_Pause(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _MJPEG_RET_TO_VDEC_RESULT(eRet, MApi_MJPEG_Pause(u32Id));
            if(eRet == E_VDEC_EX_OK)
            {
                pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_PAUSE;
            }
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_Resume(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MDrv_MVD_Resume(u32Id);
            pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_PLAY;
            eRet = E_VDEC_EX_OK;
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_Play(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _MJPEG_RET_TO_VDEC_RESULT(eRet, MApi_MJPEG_Resume(u32Id));
            if(eRet == E_VDEC_EX_OK)
            {
                pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_PLAY;
            }
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_StepDisp(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_StepDisp(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_StepDisp(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _MJPEG_RET_TO_VDEC_RESULT(eRet, MApi_MJPEG_StepPlay(u32Id));
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_StepDecode(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_StepDecode(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_StepDecode(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            // Now we always return success
            _MJPEG_RET_TO_VDEC_RESULT(eRet, MApi_MJPEG_StepDecode(u32Id));
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    pVdecExContext->u8enable_store_set_cmd[u8Idx] = FALSE;

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_SetTrickMode(VDEC_StreamId *pStreamId, VDEC_EX_TrickDec eTrickDec)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            if (E_VDEC_EX_TRICK_DEC_ALL == eTrickDec)
            {
                _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_TrickPlay(u32Id, E_MVD_TRICK_DEC_ALL, 1));
            }
            else if (E_VDEC_EX_TRICK_DEC_IP == eTrickDec)
            {
                _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_TrickPlay(u32Id, E_MVD_TRICK_DEC_IP, 1));
            }
            else if (E_VDEC_EX_TRICK_DEC_I == eTrickDec)
            {
                _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_TrickPlay(u32Id, E_MVD_TRICK_DEC_I, 1));
            }
            else
            {
                eRet = E_VDEC_EX_RET_INVALID_PARAM;
            }

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            if (E_VDEC_EX_TRICK_DEC_ALL == eTrickDec)
            {
                _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetSkipDecMode(u32Id, E_HVD_EX_SKIP_DECODE_ALL));
            }
            else if (E_VDEC_EX_TRICK_DEC_IP == eTrickDec)
            {
                _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetSkipDecMode(u32Id, E_HVD_EX_SKIP_DECODE_IP));
            }
            else if (E_VDEC_EX_TRICK_DEC_I == eTrickDec)
            {
                _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetSkipDecMode(u32Id, E_HVD_EX_SKIP_DECODE_I));
            }
            else
            {
                eRet = E_VDEC_EX_RET_INVALID_PARAM;
            }

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_PushDecQ(VDEC_StreamId *pStreamId, VDEC_EX_DecCmd *pCmd)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if (pVdecExContext->_bEsBufMgmt[u8Idx])
    {
        //minus bsStartAdd automatically since GetESBuff returns PA but PushDecQ use offset
        pCmd->u32StAddr = pCmd->u32StAddr - pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32BitstreamBufAddr;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MVD_PacketInfo stMvdPktInfo;

            _VDEC_Memset(&stMvdPktInfo, 0, sizeof(MVD_PacketInfo));

            stMvdPktInfo.u32StAddr    = pCmd->u32StAddr;
            stMvdPktInfo.u32Length    = pCmd->u32Size;
            stMvdPktInfo.u32TimeStamp = pCmd->u32Timestamp;
            stMvdPktInfo.u32ID_H      = pCmd->u32ID_H;
            stMvdPktInfo.u32ID_L      = pCmd->u32ID_L;

            _MVD_RET_TO_VDEC_RESULT(eRet, MDrv_MVD_PushQueue(u32Id, &stMvdPktInfo));

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        { // File mode
            HVD_EX_PacketInfo packetInfo;

            _VDEC_Memset(&packetInfo, 0, sizeof(HVD_EX_PacketInfo));

            packetInfo.u32Staddr    = pCmd->u32StAddr;
            packetInfo.u32Length    = pCmd->u32Size;
            packetInfo.u32TimeStamp = pCmd->u32Timestamp;
            packetInfo.u32ID_H      = pCmd->u32ID_H;
            packetInfo.u32ID_L      = pCmd->u32ID_L;

#ifndef VDEC3
            if (pVdecExContext->_bEsBufMgmt[u8Idx])
            {
                packetInfo.u32AllocLength = packetInfo.u32Length;// + ES_SAFE_SIZE;
            }
            else
            {
                packetInfo.u32AllocLength = packetInfo.u32Length;
            }

            if (E_VDEC_EX_CODEC_TYPE_VP8 == pVdecExContext->_Attr[u8Idx].eCodecType)
            {
                _VDEC_EX_ReparseVP8Packet(u8Idx, &packetInfo);
            }
#endif
            _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_PushQueue(u32Id, &packetInfo));

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            MJPEG_Packet_Info packetInfo;

            _VDEC_Memset(&packetInfo, 0, sizeof(MJPEG_Packet_Info));

            packetInfo.u32StAddr    = pCmd->u32StAddr;
            packetInfo.u32Size      = pCmd->u32Size;
            packetInfo.u32TimeStamp = pCmd->u32Timestamp;
            packetInfo.u32ID_H      = pCmd->u32ID_H;
            packetInfo.u32ID_L      = pCmd->u32ID_L;

            _MJPEG_RET_TO_VDEC_RESULT(eRet, MApi_MJPEG_PushQueue(u32Id, &packetInfo));
            _VDEC_EX_API_MutexUnlock();

            break;
        }
        default:
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_Flush(VDEC_StreamId *pStreamId, VDEC_EX_FreezePicSelect eFreezePic)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _MVD_RET_TO_VDEC_RESULT(eRet, MDrv_MVD_FlushQueue(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            if (eFreezePic == E_VDEC_EX_FREEZE_AT_LAST_PIC)
            {
                _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_Flush(u32Id, TRUE));
            }
            else
            {
                _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_Flush(u32Id, FALSE));
            }
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _MJPEG_RET_TO_VDEC_RESULT(eRet, MApi_MJPEG_FlushQueue(u32Id));
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            break;
    }

    pVdecExContext->bFlush[u8Idx] = TRUE;

    return eRet;
}


VDEC_EX_Result _MApi_VDEC_EX_V2_EnableLastFrameShow(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _MVD_RET_TO_VDEC_RESULT(eRet, MDrv_MVD_EnableLastFrameShow(u32Id, bEnable));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetDataEnd(u32Id, bEnable));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _MJPEG_RET_TO_VDEC_RESULT(eRet, MApi_MJPEG_EnableLastFrameShow(u32Id, bEnable));
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            break;
    }

    return eRet;
}


VDEC_EX_Result _MApi_VDEC_EX_V2_SetSpeed(VDEC_StreamId *pStreamId, VDEC_EX_SpeedType eSpeedType, VDEC_EX_DispSpeed eSpeed)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MS_U8 u8MvdSpeed = _VDEC_EX_Map2MVDSpeed(eSpeed);

            if (u8MvdSpeed == 0)
            {
                return E_VDEC_EX_RET_INVALID_PARAM;
            }

            if (E_VDEC_EX_SPEED_FAST == eSpeedType)
            {
                _MVD_RET_TO_VDEC_RESULT(eRet, MDrv_MVD_SetSpeed(u32Id, E_MVD_SPEED_FAST, u8MvdSpeed));
                if(eRet == E_VDEC_EX_OK)
                {
                    pVdecExContext->bTrickMode[u8Idx] = TRUE;
                }
            }
            else if (E_VDEC_EX_SPEED_SLOW == eSpeedType)
            {
                _MVD_RET_TO_VDEC_RESULT(eRet, MDrv_MVD_SetSpeed(u32Id, E_MVD_SPEED_SLOW, u8MvdSpeed));
                if(eRet == E_VDEC_EX_OK)
                {
                    pVdecExContext->bTrickMode[u8Idx] = TRUE;
                }
            }
            else
            {
                //MS_ASSERT(u8MvdSpeed == 1);
                _MVD_RET_TO_VDEC_RESULT(eRet, MDrv_MVD_SetSpeed(u32Id, E_MVD_SPEED_DEFAULT, 1));
                if(eRet == E_VDEC_EX_OK)
                {
                    pVdecExContext->bTrickMode[u8Idx] = FALSE;
                }
            }

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            HVD_EX_DispSpeed eHvdDispSpeed = E_HVD_EX_DISP_SPEED_NORMAL_1X;
            if (E_VDEC_EX_SPEED_FAST == eSpeedType)
            {
                switch (eSpeed)
                {
                    case E_VDEC_EX_DISP_SPEED_32X:
                    {
                        eHvdDispSpeed = E_HVD_EX_DISP_SPEED_FF_32X;
                        break;
                    }
                    case E_VDEC_EX_DISP_SPEED_16X:
                    {
                        eHvdDispSpeed = E_HVD_EX_DISP_SPEED_FF_16X;
                        break;
                    }
                    case E_VDEC_EX_DISP_SPEED_8X:
                    {
                        eHvdDispSpeed = E_HVD_EX_DISP_SPEED_FF_8X;
                        break;
                    }
                    case E_VDEC_EX_DISP_SPEED_4X:
                    {
                        eHvdDispSpeed =  E_HVD_EX_DISP_SPEED_FF_4X;
                        break;
                    }
                    case E_VDEC_EX_DISP_SPEED_2X:
                    {
                        eHvdDispSpeed = E_HVD_EX_DISP_SPEED_FF_2X;
                        break;
                    }
                    case E_VDEC_EX_DISP_SPEED_1X:
                    {
                        eHvdDispSpeed = E_HVD_EX_DISP_SPEED_NORMAL_1X;
                        break;
                    }
                    default:
                        return E_VDEC_EX_FAIL;

                        break;
                }

                _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetDispSpeed(u32Id, eHvdDispSpeed));
            }
            else if (E_VDEC_EX_SPEED_SLOW == eSpeedType)
            {
                switch (eSpeed)
                {
                    case E_VDEC_EX_DISP_SPEED_32X:
                    {
                        eHvdDispSpeed = E_HVD_EX_DISP_SPEED_SF_32X;
                        break;
                    }
                    case E_VDEC_EX_DISP_SPEED_16X:
                    {
                        eHvdDispSpeed = E_HVD_EX_DISP_SPEED_SF_16X;
                        break;
                    }
                    case E_VDEC_EX_DISP_SPEED_8X:
                    {
                        eHvdDispSpeed = E_HVD_EX_DISP_SPEED_SF_8X;
                        break;
                    }
                    case E_VDEC_EX_DISP_SPEED_4X:
                    {
                        eHvdDispSpeed = E_HVD_EX_DISP_SPEED_SF_4X;
                        break;
                    }
                    case E_VDEC_EX_DISP_SPEED_2X:
                    {
                        eHvdDispSpeed = E_HVD_EX_DISP_SPEED_SF_2X;
                        break;
                    }
                    case E_VDEC_EX_DISP_SPEED_1X:
                    {
                        eHvdDispSpeed = E_HVD_EX_DISP_SPEED_NORMAL_1X;
                        break;
                    }
                    default:
                        return E_VDEC_EX_FAIL;

                        break;

                }

                _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetDispSpeed(u32Id, eHvdDispSpeed));
            }
            else
            {
                _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetDispSpeed(u32Id, E_HVD_EX_DISP_SPEED_NORMAL_1X));
            }

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _MJPEG_RET_TO_VDEC_RESULT(eRet, MApi_MJPEG_SetSpeed(u32Id, (MJPEG_SpeedType) eSpeedType, (MJPEG_DispSpeed) eSpeed));
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_SetFrmBuffAddr(VDEC_StreamId *pStreamId, MS_PHY u32FrmBuffAddr)
{
#ifdef VDEC3_FB
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            // TO DO
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            //if( (u32Id >> 24) == E_VDEC_EX_MAIN_STREAM)
            {
                _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetFrmBuffAddr(u32Id, u32FrmBuffAddr));
            }
            //else
            //{
            //    eRet = E_VDEC_EX_FAIL;
            //}
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            // TO DO
            break;
        }
        default:
            break;
    }
    return eRet;
#else
    return E_VDEC_EX_FAIL;
#endif
}

VDEC_EX_Result _MApi_VDEC_EX_V2_SetFrmBuffSize(VDEC_StreamId *pStreamId, MS_U32 u32FrmBuffSize)
{
#ifdef VDEC3_FB
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            // TO DO
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            //if((u32Id >> 24) == E_VDEC_EX_MAIN_STREAM)
            {
                _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetFrmBuffSize(u32Id, u32FrmBuffSize));

            }
            //else{
            //    eRet = E_VDEC_EX_FAIL;
            //}
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            // TO DO
            break;
        }
        default:
            break;
    }
    return eRet;
#else
    return E_VDEC_EX_FAIL;
#endif
}

VDEC_EX_Result _MApi_VDEC_EX_V2_SetFreezeDisp(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _MVD_RET_TO_VDEC_RESULT(eRet, MDrv_MVD_SetFreezeDisp(u32Id, bEnable));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetFreezeImg(u32Id, bEnable));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _MJPEG_RET_TO_VDEC_RESULT(eRet, MApi_MJPEG_SetFreezeDisp(u32Id, bEnable));
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_SetBlueScreen(VDEC_StreamId *pStreamId, MS_BOOL bOn)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_SetBlueScreen(u32Id, bOn));

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetBlueScreen(u32Id, bOn));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _MJPEG_RET_TO_VDEC_RESULT(eRet, MApi_MJPEG_SetBlueScreen(u32Id, bOn));
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_ResetPTS(VDEC_StreamId *pStreamId, MS_U32 u32PtsBase)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _MVD_RET_TO_VDEC_RESULT(eRet, MDrv_MVD_ResetPTS(u32Id, u32PtsBase));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_RstPTS(u32Id, u32PtsBase));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _MJPEG_RET_TO_VDEC_RESULT(eRet, MApi_MJPEG_ResetPTS(u32Id, u32PtsBase));
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_AVSyncOn(VDEC_StreamId *pStreamId, MS_BOOL bOn, MS_U32 u32SyncDelay, MS_U16 u16SyncTolerance)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            if(pVdecExContext->bTrickMode[u8Idx] == TRUE)
            {
                return E_VDEC_EX_OK;
            }
            MDrv_MVD_SetAVSync(u32Id, bOn, u32SyncDelay);

            if ((u16SyncTolerance!=0) && (FALSE == MDrv_MVD_ChangeAVsync(u32Id, bOn, u16SyncTolerance)))
            {
                eRet = E_VDEC_EX_FAIL;
            }
            else
            {
                eRet = E_VDEC_EX_OK;
            }

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            if (bOn)
            {
                _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetSyncActive(u32Id, TRUE));

                _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetSyncVideoDelay(u32Id, u32SyncDelay));

                _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetSyncTolerance(u32Id, u16SyncTolerance));
            }
            else
            {
                _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetSyncActive(u32Id, FALSE));
            }

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _MJPEG_RET_TO_VDEC_RESULT(eRet, MApi_MJPEG_AVSyncOn(u32Id, bOn, u32SyncDelay, u16SyncTolerance));
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            break;
    }

    return eRet;
}


VDEC_EX_Result _MApi_VDEC_EX_V2_SetAVSyncFreerunThreshold(VDEC_StreamId *pStreamId, MS_U32 u32Threshold)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MDrv_MVD_SetAVSyncFreerunThreshold(u32Id, u32Threshold);
            eRet = E_VDEC_EX_OK;
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetSyncFreeRunTH(u32Id, u32Threshold));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
            break;
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_SetEvent_MultiCallback(VDEC_StreamId *pStreamId, VDEC_EX_CB_TYPE cb_type, MS_U32 u32EventFlag, VDEC_EX_EventCb pfn, void *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    MS_U32 i = 0;

    if ((u32EventFlag != E_VDEC_EX_EVENT_OFF) && (pfn == NULL))
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    //_VDEC_EX_MutexLock(u8Idx);
    pVdecExContext->_Attr[u8Idx].vdecExEventInfo[cb_type].u32EventBitMap = u32EventFlag;
    pVdecExContext->_Attr[u8Idx].vdecExEventInfo[cb_type].pVdecEventCb = pfn;
    pVdecExContext->_Attr[u8Idx].vdecExEventInfo[cb_type].pParam = param;

#if (VDEC_EX_ISR_MONITOR)
    pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] = E_VDEC_EX_EVENT_ISR_EVENT_CHANGE;
#else
    pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] = 0; //reset event flag;
#endif
    for(i = 0; i < E_VDEC_EX_CB_MAX_NUM; i++)
    {
        pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] |= pVdecExContext->_Attr[u8Idx].vdecExEventInfo[i].u32EventBitMap;
    }

    VDEC_INFO(u8Idx, VDEC_PRINT("%s: flag=0x%x\n", __FUNCTION__, u32EventFlag));

#if (VDEC_EX_ISR_MONITOR)
    MsOS_SetEvent(s32VdecExEventId[u8Idx], E_VDEC_EX_EVENT_ISR_EVENT_CHANGE);
#endif

#ifdef VDEC3
    pVdecExContext->_pVdec_IsrProc[u8Idx] =  (VDEC_EX_IsrHandle) _VDEC_EX_NStream_IsrProc;
#else
    switch (u8Idx) // here need to review.....
    {
        case E_VDEC_EX_MAIN_STREAM:
            pVdecExContext->_pVdec_IsrProc[u8Idx] = (VDEC_EX_IsrHandle) _VDEC_EX_IsrProc;
            break;
        case E_VDEC_EX_SUB_STREAM:
            pVdecExContext->_pVdec_IsrProc[u8Idx] = (VDEC_EX_IsrHandle) _VDEC_EX_Sub_IsrProc;
            break;
        default:
            break;
    }
#endif

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MS_U32 u32Event = E_MVD_EVENT_DISABLE_ALL;

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DISP_ONE)
            {
                u32Event |= E_MVD_EVENT_DISP_VSYNC;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DISP_INFO_RDY)
            {
                u32Event |= E_MVD_EVENT_DISP_RDY;
            }

            if ((pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DISP_INFO_CHG) ||
                (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_SEQ_HDR_FOUND))

            {
                u32Event |= E_MVD_EVENT_SEQ_FOUND;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_USER_DATA_FOUND)
            {
#if defined(SUPPORT_X_MODEL_FEATURE)
                //subscribe event "CC data found in display order"
                u32Event |= E_MVD_EVENT_USER_DATA_DISP;
#else
                u32Event |= E_MVD_EVENT_USER_DATA;
#endif
            }

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_AFD_FOUND)
            {
                u32Event |= E_MVD_EVENT_USER_DATA;
            }

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_FIRST_FRAME)
            {
                u32Event |= E_MVD_EVENT_FIRST_FRAME;
            }

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_PIC_FOUND)
            {
                u32Event |= E_MVD_EVENT_PIC_FOUND;
            }

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_VIDEO_UNMUTE)
            {
                u32Event |= E_MVD_EVENT_UNMUTE;
            }

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DEC_ONE)
            {
                u32Event |= E_MVD_EVENT_DEC_ONE_FRAME;
            }

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DEC_ERR)
            {
                u32Event |= E_MVD_EVENT_DEC_ERR;
            }

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_ES_DATA_ERR)
            {
                u32Event |= E_MVD_EVENT_DEC_DATA_ERR;
            }

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_XC_LOW_DEALY)
            {
                u32Event |= E_MVD_EVENT_XC_LOW_DEALY;
            }

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DEC_I)
            {
                u32Event |= E_MVD_EVENT_DEC_I;
            }

#if 0
            if (u32Event == E_MVD_EVENT_DISABLE_ALL)
            {
                _VDEC_EX_MutexUnlock(u8Idx);
            }
#endif
            if (E_MVD_RET_OK != MDrv_MVD_SetIsrEvent(u32Id, u32Event, (MVD_InterruptCb) pVdecExContext->_pVdec_IsrProc[u8Idx]))
            {
                eRet = E_VDEC_EX_FAIL;
            }
            else
            {
                eRet = E_VDEC_EX_OK;
            }
 #if 0
            if (u32Event == E_MVD_EVENT_DISABLE_ALL)
            {
                _VDEC_EX_MutexLock(u8Idx);
            }
 #endif
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            MS_U32 eEvent = E_HVD_EX_ISR_NONE;

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DISP_ONE)
            {
                eEvent |= E_HVD_EX_ISR_DISP_ONE;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DISP_REPEAT)
            {
                eEvent |= E_HVD_EX_ISR_DISP_REPEAT;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DISP_WITH_CC)
            {
                eEvent |= E_HVD_EX_ISR_DISP_WITH_CC;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DEC_ONE)
            {
                eEvent |= E_HVD_EX_ISR_DEC_ONE;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DEC_I)
            {
                eEvent |= E_HVD_EX_ISR_DEC_I;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DEC_ERR)
            {
                eEvent |= E_HVD_EX_ISR_DEC_HW_ERR;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DISP_INFO_CHG)
            {
                eEvent |= E_HVD_EX_ISR_DEC_DISP_INFO_CHANGE;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_USER_DATA_FOUND)
            {
                eEvent |= E_HVD_EX_ISR_DEC_CC_FOUND;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DISP_INFO_RDY)
            {
                eEvent |= E_HVD_EX_ISR_DEC_DISP_INFO_CHANGE;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_FIRST_FRAME)
            {
                eEvent |= E_HVD_EX_ISR_DEC_FIRST_FRM;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_VIDEO_UNMUTE)
            {
                eEvent |= E_HVD_EX_ISR_DISP_FIRST_FRM;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_SEQ_HDR_FOUND)
            {
                eEvent |= E_HVD_EX_ISR_DEC_SEQ_HDR_FOUND;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_AFD_FOUND)
            {
                eEvent |= E_HVD_EX_ISR_DEC_DISP_INFO_CHANGE;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_ES_DATA_ERR)
            {
                eEvent |= E_HVD_EX_ISR_DEC_DATA_ERR;
            }
            if (E_HVD_EX_OK != MDrv_HVD_EX_SetISREvent(u32Id, eEvent, (HVD_InterruptCb) pVdecExContext->_pVdec_IsrProc[u8Idx]))
            {
                eRet = E_VDEC_EX_FAIL;
                goto _END;
            }

            eRet = E_VDEC_EX_OK;
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        }
        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

_END:
    //_VDEC_EX_MutexUnlock(u8Idx);
    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_UnsetEvent_MultiCallback(VDEC_StreamId *pStreamId, VDEC_EX_CB_TYPE cb_type, MS_U32 u32EventFlag)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    MS_U32 i = 0;

    //_VDEC_EX_MutexLock(u8Idx);

    if (E_VDEC_EX_EVENT_OFF == u32EventFlag)
    {
        //_VDEC_EX_MutexUnlock(u8Idx);
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    pVdecExContext->_Attr[u8Idx].vdecExEventInfo[cb_type].u32EventBitMap &= ~u32EventFlag;

    if (E_VDEC_EX_EVENT_OFF == pVdecExContext->_Attr[u8Idx].vdecExEventInfo[cb_type].u32EventBitMap)
    {
        pVdecExContext->_Attr[u8Idx].vdecExEventInfo[cb_type].pVdecEventCb = NULL;
        pVdecExContext->_Attr[u8Idx].vdecExEventInfo[cb_type].pParam = NULL;
    }

#if (VDEC_EX_ISR_MONITOR)
    pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] = E_VDEC_EX_EVENT_ISR_EVENT_CHANGE;
#else
    pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] = 0; //reset event flag;
#endif
    for(i = 0; i < E_VDEC_EX_CB_MAX_NUM; i++)
    {
        pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] |= pVdecExContext->_Attr[u8Idx].vdecExEventInfo[i].u32EventBitMap;
    }

#if (VDEC_EX_ISR_MONITOR)
    MsOS_SetEvent(s32VdecExEventId[u8Idx], E_VDEC_EX_EVENT_ISR_EVENT_CHANGE);
#endif

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MS_U32 u32Event = 0;

            //Set Event
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DISP_INFO_RDY)
            {
                u32Event |= E_MVD_EVENT_DISP_RDY;
            }

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DISP_INFO_CHG)
            {
                u32Event |= E_MVD_EVENT_SEQ_FOUND;
            }

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_SEQ_HDR_FOUND)
            {
                u32Event |= E_MVD_EVENT_SEQ_FOUND;
            }

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_USER_DATA_FOUND)
            {
#if defined(SUPPORT_X_MODEL_FEATURE)
                u32Event |= E_MVD_EVENT_USER_DATA_DISP;
#else
                u32Event |= E_MVD_EVENT_USER_DATA;
#endif
            }

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_AFD_FOUND)
            {
                u32Event |= E_MVD_EVENT_USER_DATA;
            }

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_FIRST_FRAME)
            {
                u32Event |= E_MVD_EVENT_FIRST_FRAME;
            }

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_PIC_FOUND)
            {
                u32Event |= E_MVD_EVENT_PIC_FOUND;
            }

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_VIDEO_UNMUTE)
            {
                u32Event |= E_MVD_EVENT_UNMUTE;
            }

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DEC_ONE)
            {
                u32Event |= E_MVD_EVENT_DEC_ONE_FRAME;
            }

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DEC_ERR)
            {
                u32Event |= E_MVD_EVENT_DEC_ERR;
            }

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_ES_DATA_ERR)
            {
                u32Event |= E_MVD_EVENT_DEC_DATA_ERR;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_XC_LOW_DEALY)
            {
                u32Event |= E_MVD_EVENT_XC_LOW_DEALY;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DEC_I)
            {
                u32Event |= E_MVD_EVENT_DEC_I;
            }
 #if 0
            if (u32Event == E_MVD_EVENT_DISABLE_ALL)
            {
                //to avoid deadlock: unlock mutex so that _VDEC_EX_IsrProc() can lock mutex
                _VDEC_EX_MutexUnlock(u8Idx);
            }
 #endif
            if (E_MVD_RET_OK != MDrv_MVD_SetIsrEvent(u32Id, u32Event, (MVD_InterruptCb) pVdecExContext->_pVdec_IsrProc[u8Idx]))
            {
                eRet = E_VDEC_EX_FAIL;
            }
            else
            {
                eRet = E_VDEC_EX_OK;
            }
  #if 0
            if (u32Event == E_MVD_EVENT_DISABLE_ALL)
            {
                _VDEC_EX_MutexLock(u8Idx);
            }
  #endif
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            HVD_EX_IsrEvent eEvent = E_HVD_EX_ISR_NONE;

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DISP_ONE)
            {
                eEvent |= E_HVD_EX_ISR_DISP_ONE;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DISP_REPEAT)
            {
                eEvent |= E_HVD_EX_ISR_DISP_REPEAT;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DISP_WITH_CC)
            {
                eEvent |= E_HVD_EX_ISR_DISP_WITH_CC;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DEC_ONE)
            {
                eEvent |= E_HVD_EX_ISR_DEC_ONE;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DEC_I)
            {
                eEvent |= E_HVD_EX_ISR_DEC_I;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DEC_ERR)
            {
                eEvent |= E_HVD_EX_ISR_DEC_HW_ERR;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DISP_INFO_CHG)
            {
                eEvent |= E_HVD_EX_ISR_DEC_DISP_INFO_CHANGE;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_USER_DATA_FOUND)
            {
                eEvent |= E_HVD_EX_ISR_DISP_WITH_CC;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_DISP_INFO_RDY)
            {
                eEvent |= E_HVD_EX_ISR_DEC_DISP_INFO_CHANGE;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_FIRST_FRAME)
            {
                eEvent |= E_HVD_EX_ISR_DEC_FIRST_FRM;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_VIDEO_UNMUTE)
            {
                eEvent |= E_HVD_EX_ISR_DISP_FIRST_FRM;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_AFD_FOUND)
            {
                eEvent |= E_HVD_EX_ISR_DEC_DISP_INFO_CHANGE;
            }

            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_SEQ_HDR_FOUND)
            {
                eEvent |= E_HVD_EX_ISR_DEC_SEQ_HDR_FOUND;
            }
            if (pVdecExContext->_u32VdecExIsrEventFlag[u8Idx] & E_VDEC_EX_EVENT_ES_DATA_ERR)
            {
                eEvent |= E_HVD_EX_ISR_DEC_DATA_ERR;
            }
            if (eEvent != E_HVD_EX_ISR_NONE)
            {
                if (E_HVD_EX_OK != MDrv_HVD_EX_SetISREvent(u32Id, eEvent, (HVD_InterruptCb) pVdecExContext->_pVdec_IsrProc[u8Idx]))
                {
                    //_VDEC_EX_MutexUnlock(u8Idx);
                    return E_VDEC_EX_FAIL;
                }
            }
            eRet = E_VDEC_EX_OK;
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        }
        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    //_VDEC_EX_MutexUnlock(u8Idx);
    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_FireDecCmd(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
#ifdef VDEC3
            if(pVdecExContext->u32FrameBaseMode[u8Idx] == FALSE)
 #endif
            {
                MDrv_MVD_SetSLQWritePtr(u32Id, TRUE);
            }
            eRet = E_VDEC_EX_OK;
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
#ifndef VDEC3
            MDrv_HVD_EX_PushQueue_Fire(u32Id);
#endif
            eRet = E_VDEC_EX_OK;
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_SeekToPTS(VDEC_StreamId *pStreamId, MS_U32 u32PTS)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_SeekToPTS(u32Id, u32PTS));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SeekToPTS(u32Id, u32PTS));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _MJPEG_RET_TO_VDEC_RESULT(eRet, MApi_MJPEG_SeekToPTS(u32Id, u32PTS));
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_SkipToPTS(VDEC_StreamId *pStreamId, MS_U32 u32PTS)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_SkipToPTS(u32Id, u32PTS));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SkipToPTS(u32Id, u32PTS));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _MJPEG_RET_TO_VDEC_RESULT(eRet, MApi_MJPEG_SkipToPTS(u32Id, u32PTS));
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_DisableDeblocking(VDEC_StreamId *pStreamId, MS_BOOL bDisable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            break;

        case E_VDEC_EX_DECODER_HVD:
        {
            _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetSettings_Pro(u32Id,E_HVD_EX_SSET_DISABLE_DEBLOCKING,bDisable));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            break;
        }
        default:
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_DisableQuarterPixel(VDEC_StreamId *pStreamId, MS_BOOL bDisable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            break;

        case E_VDEC_EX_DECODER_HVD:
        {
            _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetSettings_Pro(u32Id, E_HVD_EX_SSET_DISABLE_QUARTER_PIXEL ,bDisable));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            break;
        }
        default:
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_SetAutoRmLstZeroByte(VDEC_StreamId *pStreamId, MS_BOOL bOn)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            break;

        case E_VDEC_EX_DECODER_HVD:
        {
            _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetAutoRmLstZeroByte(u32Id, bOn));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            break;
        }
        default:
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_SetBalanceBW(VDEC_StreamId *pStreamId, MS_U8 u8QuarPixelTH, MS_U8 u8DeBlockingTH, MS_U8 u8UpperBound, MS_U8 u8SafeCoef)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            break;

        case E_VDEC_EX_DECODER_HVD:
        {
            _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_SetBalanceBW(u32Id, u8QuarPixelTH, u8DeBlockingTH, u8UpperBound));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            break;
        }
        default:
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_GenPattern(VDEC_StreamId *pStreamId, VDEC_EX_PatternType ePatternType, MS_PHY u32Addr, MS_U32* u32Size)
{
    VDEC_EX_Result eRet=E_VDEC_EX_RET_UNSUPPORTED;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            if (ePatternType == E_VDEC_EX_PATTERN_FLUSH)
            {
                _MVD_RET_TO_VDEC_RESULT(eRet, MDrv_MVD_GenPattern(u32Id, E_MVD_PATTERN_FLUSH, u32Addr, u32Size));
            }
            else if (ePatternType == E_VDEC_EX_PATTERN_FILEEND)
            {
                _MVD_RET_TO_VDEC_RESULT(eRet, MDrv_MVD_GenPattern(u32Id, E_MVD_PATTERN_FILEEND, u32Addr, u32Size));
            }
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            if (ePatternType == E_VDEC_EX_PATTERN_FLUSH)
            {
                _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_GenPattern(u32Id, E_HVD_EX_PATTERN_FLUSH, MS_PA2KSEG1(u32Addr), u32Size));
            }
            else if (ePatternType == E_VDEC_EX_PATTERN_FILEEND)
            {
                _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_GenPattern(u32Id, E_HVD_EX_PATTERN_FILEEND, MS_PA2KSEG1(u32Addr), u32Size));
            }
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
            break;

        default:
            break;
    }

    return eRet;
}


VDEC_EX_Result _MApi_VDEC_EX_V2_MHEG_DecodeIFrame(VDEC_StreamId *pStreamId,
                                            MS_PHY u32FrameBufAddr,
                                            MS_PHY u32SrcSt,
                                            MS_PHY u32SrcEnd)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            if ((u32FrameBufAddr == 0) && pVdecExContext->bCMAUsed && bCMAInitPool[0] && (u8Idx == 0))
            {
                u32FrameBufAddr = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufAddr;
            }
            _BOOL_TO_VDEC_RESULT(eRet,  MDrv_MVD_DecodeIFrame(u32Id, u32FrameBufAddr, u32SrcSt, u32SrcEnd));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _HVD_RET_TO_VDEC_RESULT(eRet,  MDrv_HVD_EX_DecodeIFrame(u32Id, u32SrcSt, (u32SrcEnd - u32SrcSt)));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;

            break;
    }

    pVdecExContext->u8enable_store_set_cmd[u8Idx] = FALSE;

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_MHEG_RstIFrameDec(VDEC_StreamId *pStreamId)
{
    VDEC_PRINT("VDEC_EX err: %s is obsolete!\n", __FUNCTION__);

    return E_VDEC_EX_RET_UNSUPPORTED;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_CC_StartParsing(VDEC_StreamId *pStreamId, VDEC_EX_CCCfg *pCCParam)
{
    VDEC_PRINT("%s is obsolete\n", __FUNCTION__);
    UNUSED(pStreamId);
    UNUSED(pCCParam);
    return E_VDEC_EX_RET_UNSUPPORTED;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_CC_StopParsing(VDEC_StreamId *pStreamId)
{
    VDEC_PRINT("%s is obsolete\n", __FUNCTION__);
    UNUSED(pStreamId);
    return E_VDEC_EX_RET_UNSUPPORTED;
}


VDEC_EX_Result _MApi_VDEC_EX_V2_CC_UpdateReadPtr(VDEC_StreamId *pStreamId, MS_U32 u32EachPacketSize)
{
    VDEC_PRINT("%s is obsolete\n", __FUNCTION__);
    UNUSED(pStreamId);
    UNUSED(u32EachPacketSize);
    return E_VDEC_EX_RET_UNSUPPORTED;
}


VDEC_EX_Result _MApi_VDEC_EX_V2_SetBlockDisplay(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_SetMStreamerMode(u32Id, bEnable));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _HVD_RET_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_EnableDispQue(u32Id, bEnable));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _MJPEG_RET_TO_VDEC_RESULT(eRet, MApi_MJPEG_EnableDispCmdQ(u32Id, bEnable));
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_EnableESBuffMalloc(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);

    if ((E_VDEC_EX_DECODER_MVD != pVdecExContext->_Attr[u8Idx].eDecoder) &&
        (E_VDEC_EX_DECODER_HVD != pVdecExContext->_Attr[u8Idx].eDecoder) &&
        (E_VDEC_EX_DECODER_MJPEG != pVdecExContext->_Attr[u8Idx].eDecoder))
    {
        return E_VDEC_EX_RET_UNSUPPORTED;
    }

    pVdecExContext->_bEsBufMgmt[u8Idx] = bEnable;
    return E_VDEC_EX_OK;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_DisplayFrame(VDEC_StreamId *pStreamId, VDEC_EX_DispFrame *pDispFrm)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if (!pDispFrm)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    if (_VDEC_EX_IsDqValid(pDispFrm))
    {
        VDEC_INFO(u8Idx, VDEC_PRINT("%s invalid pDispFrm 0x%x\n", __FUNCTION__, pDispFrm->u32PriData));
        return E_VDEC_EX_RET_ILLEGAL_ACCESS;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_FrameFlip(u32Id, pDispFrm->u32PriData));
            _VDEC_EX_FreeDq(u8Idx, pDispFrm);
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            if(MDrv_HVD_EX_DispFrame(u32Id, pDispFrm->u32PriData) != E_HVD_EX_OK)
            {
                return E_VDEC_EX_FAIL;
            }
            _VDEC_EX_FreeDq(u8Idx, pDispFrm);
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();

            if(MApi_MJPEG_DispFrame(u32Id, pDispFrm->u32PriData) != E_MJPEG_RET_SUCCESS)
            {
                _VDEC_EX_API_MutexUnlock();
                return E_VDEC_EX_FAIL;
            }
            _VDEC_EX_FreeDq(u8Idx, pDispFrm);
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}


VDEC_EX_Result _MApi_VDEC_EX_V2_ReleaseFrame(VDEC_StreamId *pStreamId, VDEC_EX_DispFrame *pDispFrm)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if (!pDispFrm)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    if (_VDEC_EX_IsDqValid(pDispFrm))
    {
        VDEC_INFO(u8Idx, VDEC_PRINT("%s invalid pDispFrm 0x%x\n", __FUNCTION__, pDispFrm->u32PriData));
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_FrameRelease(u32Id, pDispFrm->u32PriData));
            _VDEC_EX_FreeDq(u8Idx, pDispFrm);
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            if(MDrv_HVD_EX_FreeFrame(u32Id, pDispFrm->u32PriData) != E_HVD_EX_OK)
            {
                return E_VDEC_EX_FAIL;
            }
            _VDEC_EX_FreeDq(u8Idx, pDispFrm);
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();

            if(MApi_MJPEG_FreeFrame(u32Id, pDispFrm->u32PriData) != E_MJPEG_RET_SUCCESS)
            {
                _VDEC_EX_API_MutexUnlock();
                return E_VDEC_EX_FAIL;
            }
            _VDEC_EX_FreeDq(u8Idx, pDispFrm);
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_CaptureFrame(VDEC_StreamId *pStreamId, MS_U32 u32FrmPriData, MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if (u32FrmPriData == VDEC_U32_MAX)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    //check the disable frame is valid
    if ((FALSE == bEnable) && (pVdecExContext->u32PriData[u8Idx] != u32FrmPriData))
    {
        VDEC_PRINT("%s: Release an uncaptured frame(0x%x != 0x%x)!\n",
            __FUNCTION__, u32FrmPriData, pVdecExContext->u32PriData[u8Idx]);
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_FrameCapture(u32Id, u32FrmPriData, bEnable));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            break;

        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    if (E_VDEC_EX_OK == eRet)
    {
        if (FALSE == bEnable)
        {
            pVdecExContext->u32PriData[u8Idx] = VDEC_U32_MAX;
        }
        else
        {   //record the captured frame idx
            pVdecExContext->u32PriData[u8Idx] = u32FrmPriData;
        }
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_CC_Init(VDEC_StreamId *pStreamId, MS_U32 *pIn)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if(MDrv_SYS_Query(E_SYS_QUERY_DTVCC_SUPPORTED ) == TRUE)
    {
        pVdecExContext->support_cc[u8Idx] = TRUE;
    }
    else
    {
        pVdecExContext->support_cc[u8Idx] = FALSE;
        return E_VDEC_EX_RET_UNSUPPORTED;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            {
                MDrv_CC_Init(u32Id);
                eRet = E_VDEC_EX_OK;
            }
            break;

        case E_VDEC_EX_DECODER_HVD:
            {
                eRet = MDrv_HVD_EX_CC_Init(u32Id);
            }
            break;

        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_CC_SetCfg(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt, VDEC_EX_CCType eType, MS_U32 *pIn)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if(pVdecExContext->support_cc[u8Idx] == FALSE)
    {
        return E_VDEC_EX_RET_UNSUPPORTED;
    }

    if (!pIn)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            {
                MS_U8 u8Type = _VDEC_Map2MVDCCFmt(eFmt);
                MS_U8 u8Opt = _VDEC_Map2MVDCCOpt((VDEC_EX_CCType)eType);
                MS_U16 u16BufferSize = 0;
                VDEC_EX_CC_InputPara* pInput = (VDEC_EX_CC_InputPara*)pIn;
                if (pInput->u32Ver != 0)
                {
                    VDEC_PRINT("%s not supported ver %d\n", __FUNCTION__, pInput->u32Ver);
                    return E_VDEC_EX_FAIL;
                }
                u16BufferSize = (MS_U16)pInput->u32Val;
                MDrv_CC_CM_SetParsingType(u32Id, u8Opt, u16BufferSize, u8Type);
                eRet = E_VDEC_EX_OK;
            }
            break;
        case E_VDEC_EX_DECODER_HVD:
            {
                MS_U8 u8Type = _VDEC_Map2HVDCCFmt(eFmt);
                MS_U8 u8Opt = _VDEC_Map2HVDCCOpt((VDEC_EX_CCType)eType);
                MS_U16 u16BufferSize = 0;
                VDEC_EX_CC_InputPara* pInput = (VDEC_EX_CC_InputPara*)pIn;
                if (pInput->u32Ver != 0)
                {
                    VDEC_PRINT("%s not supported ver %d\n", __FUNCTION__, pInput->u32Ver);
                    return E_VDEC_EX_FAIL;
                }
                u16BufferSize = (MS_U16)pInput->u32Val;
                eRet = MDrv_HVD_EX_CC_SetCfg(u32Id, u8Opt, u16BufferSize, u8Type);

            }
            break;

        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_CC_SetBuffStartAdd(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt, MS_U32 *pIn)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if(pVdecExContext->support_cc[u8Idx] == FALSE)
    {
        return E_VDEC_EX_RET_UNSUPPORTED;
    }

    if (!pIn)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            {
                MS_U8 u8Type = _VDEC_Map2MVDCCFmt(eFmt);
                MS_VIRT u32StartAdd = 0;
                VDEC_EX_CC_InputPara* pInput = (VDEC_EX_CC_InputPara*)pIn;
                if (pInput->u32Ver != 0)
                {
                    VDEC_PRINT("%s not supported ver %d\n", __FUNCTION__, pInput->u32Ver);
                    return E_VDEC_EX_FAIL;
                }
                u32StartAdd = pInput->u32Val;
                MDrv_CC_CM_SetMVDRB_HWAddr(u32Id, u32StartAdd, u8Type);
                eRet = E_VDEC_EX_OK;
            }
            break;
        case E_VDEC_EX_DECODER_HVD:
            {
                MS_U8 u8Type = _VDEC_Map2HVDCCFmt(eFmt);
                MS_VIRT u32StartAdd = 0;
                VDEC_EX_CC_InputPara* pInput = (VDEC_EX_CC_InputPara*)pIn;
                if (pInput->u32Ver != 0)
                {
                    VDEC_PRINT("%s not supported ver %d\n", __FUNCTION__, pInput->u32Ver);
                    return E_VDEC_EX_FAIL;
                }
                u32StartAdd = pInput->u32Val;
                eRet = MDrv_HVD_EX_CC_Set_RB_StartAddr(u32Id, u32StartAdd, u8Type);

            }
            break;
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_CC_UpdateWriteAdd(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt, MS_U32 *pIn)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if(pVdecExContext->support_cc[u8Idx] == FALSE)
    {
        return E_VDEC_EX_RET_UNSUPPORTED;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            {
                MS_U8 u8Type = _VDEC_Map2MVDCCFmt(eFmt);
                MDrv_CC_CM_SyncMVDRB_SWAddr2HWAddr(u32Id, u8Type);
                eRet = E_VDEC_EX_OK;
            }
            break;
        case E_VDEC_EX_DECODER_HVD:
            {
                MS_U8 u8Type = _VDEC_Map2HVDCCFmt(eFmt);
                eRet = MDrv_HVD_EX_CC_SyncRB_RdAddr2WrAddr(u32Id, u8Type);

            }
            break;
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_CC_UpdateReadAdd(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt, MS_U32 *pIn)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if(pVdecExContext->support_cc[u8Idx] == FALSE)
    {
        return E_VDEC_EX_RET_UNSUPPORTED;
    }

    if (!pIn)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            {
                MS_U8 u8Type = _VDEC_Map2MVDCCFmt(eFmt);
                MS_U32 u32EachPktSize = 0;
                VDEC_EX_CC_InputPara* pInput = (VDEC_EX_CC_InputPara*)pIn;
                if (pInput->u32Ver != 0)
                {
                    VDEC_PRINT("%s not supported ver %d\n", __FUNCTION__, pInput->u32Ver);
                    return E_VDEC_EX_FAIL;
                }
                u32EachPktSize = pInput->u32Val;
                MDrv_CC_PM_SetMVDRB_ReadAddr(u32Id, u32EachPktSize, u8Type);
                eRet = E_VDEC_EX_OK;
            }
            break;

        case E_VDEC_EX_DECODER_HVD:
            {
                MS_U8 u8Type = _VDEC_Map2HVDCCFmt(eFmt);
                MS_U32 u32EachPktSize = 0;
                VDEC_EX_CC_InputPara* pInput = (VDEC_EX_CC_InputPara*)pIn;
                if (pInput->u32Ver != 0)
                {
                    VDEC_PRINT("%s not supported ver %d\n", __FUNCTION__, pInput->u32Ver);
                    return E_VDEC_EX_FAIL;
                }
                u32EachPktSize = pInput->u32Val;
                eRet = MDrv_HVD_EX_CC_Adv_RB_ReadAddr(u32Id, u32EachPktSize, u8Type);

            }
            break;

        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_CC_DisableParsing(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if(pVdecExContext->support_cc[u8Idx] == FALSE)
    {
        return E_VDEC_EX_RET_UNSUPPORTED;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            {
                MS_U8 u8Type = _VDEC_Map2MVDCCFmt(eFmt);
                MDrv_CC_CM_DisableParsing(u32Id, u8Type);
                eRet = E_VDEC_EX_OK;
            }
            break;
        case E_VDEC_EX_DECODER_HVD:
            {
                MS_U8 u8Type = _VDEC_Map2HVDCCFmt(eFmt);
                eRet = MDrv_HVD_EX_CC_DisableParsing(u32Id, u8Type);

            }
            break;
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_IsStepDispDone(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_IsStepDispDone(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_IsFrameShowed(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _BOOL_TO_VDEC_RESULT(eRet, MApi_MJPEG_IsStepPlayDone(u32Id));
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_IsStepDecodeDone(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_IsStepDecodeDone(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_IsStepDecodeDone(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _BOOL_TO_VDEC_RESULT(eRet, MApi_MJPEG_IsStepDecodeDone(u32Id));
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_GetDispInfo(VDEC_StreamId *pStreamId, VDEC_EX_DispInfo *pDispinfo)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MVD_FrameInfo info;

            _VDEC_Memset(&info, 0, sizeof(MVD_FrameInfo));

            MDrv_MVD_GetFrameInfo(u32Id, &info);

            pDispinfo->u16HorSize   = info.u16HorSize;
            pDispinfo->u16VerSize   = info.u16VerSize;
            pDispinfo->u32FrameRate = info.u32FrameRate;
            pDispinfo->u8AspectRate = info.u8AspectRate;
            pDispinfo->u8Interlace  = info.u8Interlace;
            pDispinfo->u8AFD        = info.u8AFD;
            ///VDEC SAR actually means PAR(Pixel Aspect Ratio)
            pDispinfo->u16SarWidth  = (MS_U16)info.u16par_width;
            pDispinfo->u16SarHeight = (MS_U16)info.u16par_height;
            if((pDispinfo->u16SarWidth >= 1) &&  (pDispinfo->u16SarHeight >= 1)  &&
               (pDispinfo->u16HorSize > 1 )  &&  (pDispinfo->u16VerSize > 1) &&
             (( info.u16CropRight + info.u16CropLeft) < pDispinfo->u16HorSize) &&
             ((info.u16CropTop + info.u16CropBottom) <pDispinfo->u16VerSize))
            {
                pDispinfo->u32AspectWidth = (MS_U32)pDispinfo->u16SarWidth * (MS_U32)(pDispinfo->u16HorSize- ( info.u16CropRight + info.u16CropLeft));
                pDispinfo->u32AspectHeight = (MS_U32)pDispinfo->u16SarHeight * (MS_U32)(pDispinfo->u16VerSize - ( info.u16CropTop + info.u16CropBottom));
            }
            else
            {
                pDispinfo->u32AspectWidth = pDispinfo->u16HorSize;
                pDispinfo->u32AspectHeight = pDispinfo->u16VerSize;
            }

            pDispinfo->u16CropRight = info.u16CropRight;
            pDispinfo->u16CropLeft  = info.u16CropLeft;
            pDispinfo->u16CropBottom = info.u16CropBottom;
            pDispinfo->u16CropTop   = info.u16CropTop;
            pDispinfo->u16Pitch     = info.u16Pitch;
            pDispinfo->u16PTSInterval = info.u16PTSInterval;
            pDispinfo->u8MPEG1      = info.u8MPEG1;
            pDispinfo->u8PlayMode   = info.u8PlayMode;
            pDispinfo->u8FrcMode    = info.u8FrcMode;
            pDispinfo->bWithChroma  = TRUE;
            pDispinfo->bEnableMIUSel = info.bEnableMIUSel;
            pDispinfo->u32DynScalingAddr= info.u32DynScalingAddr;
            pDispinfo->u8DynScalingDepth= info.u8DynScalingDepth;
            pDispinfo->u32DynScalingSize = info.u32DynScalingBufSize;


            if (MDrv_MVD_GetVideoRange(u32Id) == 1)
            {
                pDispinfo->bColorInXVYCC = TRUE;
            }
            else
            {
                pDispinfo->bColorInXVYCC = FALSE;
            }

            eRet = E_VDEC_EX_OK;
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            HVD_EX_DispInfo info;

            _VDEC_Memset(&info, 0, sizeof(HVD_EX_DispInfo));

            if(MDrv_HVD_EX_GetDispInfo(u32Id, &info) == E_HVD_EX_OK)
            {
                pDispinfo->u16HorSize   = info.u16HorSize;
                pDispinfo->u16VerSize   = info.u16VerSize;
                pDispinfo->u32FrameRate = info.u32FrameRate;
                pDispinfo->u8AspectRate = info.u8AspectRate;
                pDispinfo->u8Interlace  = info.u8Interlace;
                pDispinfo->u8AFD        = info.u8AFD;
                pDispinfo->u16SarWidth  = info.u16SarWidth;
                pDispinfo->u16SarHeight = info.u16SarHeight;

                if(  (pDispinfo->u16SarWidth >= 1) &&  (pDispinfo->u16SarHeight >= 1)  &&
                      (pDispinfo->u16HorSize > 1 )  &&  (pDispinfo->u16VerSize > 1) &&
                      (( info.u16CropRight + info.u16CropLeft) < pDispinfo->u16HorSize) &&
                      ((info.u16CropTop + info.u16CropBottom) <pDispinfo->u16VerSize))
                {
                    pDispinfo->u32AspectWidth = (MS_U32)pDispinfo->u16SarWidth * (MS_U32)(pDispinfo->u16HorSize- ( info.u16CropRight + info.u16CropLeft));
                    pDispinfo->u32AspectHeight = (MS_U32)pDispinfo->u16SarHeight * (MS_U32)(pDispinfo->u16VerSize - ( info.u16CropTop + info.u16CropBottom));
                }
                else
                {
                    pDispinfo->u32AspectWidth = pDispinfo->u16HorSize;
                    pDispinfo->u32AspectHeight = pDispinfo->u16VerSize;
                }

                pDispinfo->u16CropRight = info.u16CropRight;
                pDispinfo->u16CropLeft  = info.u16CropLeft;
                pDispinfo->u16CropBottom = info.u16CropBottom;
                pDispinfo->u16CropTop   = info.u16CropTop;
                pDispinfo->u16Pitch = info.u16Pitch;
                pDispinfo->bWithChroma  = !(info.bChroma_idc_Mono);
                pDispinfo->bColorInXVYCC = TRUE;
                pDispinfo->bEnableMIUSel = MDrv_HVD_EX_GetDynamicScalingInfo(u32Id, E_HVD_EX_DS_BUF_MIUSEL);
                pDispinfo->u32DynScalingAddr= MDrv_HVD_EX_GetDynamicScalingInfo(u32Id, E_HVD_EX_DS_BUF_ADDR);
                pDispinfo->u8DynScalingDepth= MDrv_HVD_EX_GetDynamicScalingInfo(u32Id, E_HVD_EX_DS_VECTOR_DEPTH);
                pDispinfo->u32DynScalingSize= MDrv_HVD_EX_GetDynamicScalingInfo(u32Id, E_HVD_EX_DS_BUF_SIZE);

                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_FAIL;
            }
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            MJPEG_DISP_INFO info;

            _VDEC_Memset(&info, 0, sizeof(MJPEG_DISP_INFO));

            if (E_MJPEG_RET_SUCCESS == MApi_MJPEG_GetDispInfo(u32Id, &info))
            {
                pDispinfo->u16HorSize = info.u16Width;
                pDispinfo->u16VerSize = info.u16Height;
                pDispinfo->u32FrameRate = info.u32FrameRate;
                pDispinfo->u8Interlace  = 0; // no interlace
                pDispinfo->u8AFD = 0; // not used
                pDispinfo->u16SarWidth = 1;
                pDispinfo->u16SarHeight = 1;
                pDispinfo->u32AspectWidth = pDispinfo->u16HorSize;
                pDispinfo->u32AspectHeight = pDispinfo->u16VerSize;
                pDispinfo->u16CropRight = info.u16CropRight;
                pDispinfo->u16CropLeft  = info.u16CropLeft;
                pDispinfo->u16CropBottom = info.u16CropBottom;
                pDispinfo->u16CropTop   = info.u16CropTop;
                pDispinfo->u16Pitch = info.u16Pitch;
                pDispinfo->u16PTSInterval = 0; //not used
                pDispinfo->u8MPEG1 = 0; // not used
                pDispinfo->u8PlayMode = 0; // not used
                pDispinfo->u8FrcMode = 0; // not used
                pDispinfo->u8AspectRate = 0; // not used
                pDispinfo->bWithChroma = TRUE;
                pDispinfo->bColorInXVYCC = TRUE;
                pDispinfo->u32DynScalingAddr = 0; // not used
                pDispinfo->u8DynScalingDepth = 0; // not used
                pDispinfo->u32DynScalingSize = 0; // not used
                pDispinfo->bEnableMIUSel = FALSE; // not used

                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_FAIL;
            }
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_IsAVSyncOn(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_GetIsAVSyncOn(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, (MS_BOOL) MDrv_HVD_EX_GetPlayMode(u32Id, E_HVD_EX_GMODE_IS_SYNC_ON));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _BOOL_TO_VDEC_RESULT(eRet, MApi_MJPEG_IsAVSyncOn(u32Id));
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            eRet =  E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_IsWithValidStream(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            if (MDrv_MVD_GetValidStreamFlag(u32Id))
            {
                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_FAIL;
            }

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_IsDispFinish(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _MVD_RET_TO_VDEC_RESULT(eRet, MDrv_MVD_IsDispFinish(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_IsDispFinish(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _BOOL_TO_VDEC_RESULT(eRet,  MApi_MJPEG_IsDispFinish(u32Id));
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}


VDEC_EX_Result _MApi_VDEC_EX_V2_IsIFrameFound(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_GetIsIPicFound(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_IsIFrmFound(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _BOOL_TO_VDEC_RESULT(eRet, MApi_MJPEG_IsIFrameFound(u32Id));
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

static VDEC_EX_Result _VDEC_EX_GetSeqChangeInfo(VDEC_StreamId *pStreamId, MS_U32 *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    MS_U32 changeInfo = 0;

    if (param == NULL)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    *param = 0;
    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            changeInfo = MDrv_HVD_EX_GetData(u32Id, E_HVD_EX_GDATA_TYPE_SEQ_CHANGE_INFO);
            if (changeInfo != E_HVD_SEQ_CHANGE_NONE)
            {
                if (changeInfo & E_HVD_SEQ_CHANGE_FIRST_TIME)
                    *param |= VDEC_EX_SEQ_CHANGE_FIRST_TIME;
                if (changeInfo & E_HVD_SEQ_CHANGE_RESOLUTION)
                    *param |= VDEC_EX_SEQ_CHANGE_RESOLUTION;
                if (changeInfo & E_HVD_SEQ_CHANGE_PICTURE_TYPE)
                    *param |= VDEC_EX_SEQ_CHANGE_PICTURE_TYPE;
                if (changeInfo & E_HVD_SEQ_CHANGE_FRAME_RATE)
                    *param |= VDEC_EX_SEQ_CHANGE_FRAME_RATE;
                if (changeInfo & E_HVD_SEQ_CHANGE_HDR_INFO)
                    *param |= VDEC_EX_SEQ_CHANGE_HDR_INFO;
                eRet = E_VDEC_EX_OK;
            }
            break;
        case E_VDEC_EX_DECODER_MVD:
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_IsSeqChg(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_IsSeqChg(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_IsDispInfoChg(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_FAIL;
            break;
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_IsReachSync(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet,  (MS_BOOL) (MDrv_MVD_GetSyncStatus(u32Id)==1));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_IsSyncReach(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _BOOL_TO_VDEC_RESULT(eRet, MApi_MJPEG_IsReachSync(u32Id));
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_IsStartSync(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            if (MDrv_MVD_GetIsSyncSkip(u32Id) || MDrv_MVD_GetIsSyncRep(u32Id))
            {
                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_FAIL;
            }
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_IsSyncStart(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_IsFreerun(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(eRet,  MDrv_MVD_GetIsFreerun(u32Id));
            break;

        case E_VDEC_EX_DECODER_HVD:
            break;

        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_IsWithLowDelay(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_GetLowDelayFlag(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_IsLowDelay(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            _BOOL_TO_VDEC_RESULT(eRet, MApi_MJPEG_IsWithLowDelay(u32Id));
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_IsAllBufferEmpty(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_IsAllBufferEmpty(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_IsAllBufferEmpty(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_FAIL;
            break;

        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_GetExtDispInfo(VDEC_StreamId *pStreamId, VDEC_EX_ExtDispInfo *pExtDispinfo)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MVD_ExtDispInfo stExtDispInfo;
            _VDEC_Memset(&stExtDispInfo, 0, sizeof(MVD_ExtDispInfo));
            if (E_MVD_RET_OK == MDrv_MVD_GetExtDispInfo(u32Id, &stExtDispInfo))
            {
                pExtDispinfo->u16VSize   = stExtDispInfo.u16VSize;
                pExtDispinfo->u16HSize   = stExtDispInfo.u16HSize;
                pExtDispinfo->s16VOffset = (MS_S16)stExtDispInfo.u16VOffset;
                pExtDispinfo->s16HOffset = (MS_S16)stExtDispInfo.u16HOffset;
                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_FAIL;
            }
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_IsDispQueueEmpty(VDEC_StreamId *pStreamId,MS_BOOL* bEmpty)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            *bEmpty = MDrv_MVD_EX_IsDispQueueEmpty(u32Id);
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            *bEmpty = MDrv_HVD_EX_IsDispQueueEmpty(u32Id);
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        }
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_GetDecFrameInfo(VDEC_StreamId *pStreamId, VDEC_EX_FrameInfo *pFrmInfo)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MVD_FrmInfo stFrm;
            _VDEC_Memset(&stFrm, 0, sizeof(MVD_FrmInfo));
            if (E_MVD_RET_OK == MDrv_MVD_GetFrmInfo(u32Id, E_MVD_FRMINFO_DECODE, &stFrm))
            {
                pFrmInfo->eFrameType    = _VDEC_EX_MapFrmType2MVD(stFrm.eFrmType);
                pFrmInfo->u16Height     = stFrm.u16Height;
                pFrmInfo->u16Width      = stFrm.u16Width;
                pFrmInfo->u16Pitch      = stFrm.u16Pitch;
                pFrmInfo->u32ChromaAddr = stFrm.u32ChromaAddr;
                pFrmInfo->u32ID_H       = stFrm.u32ID_H;
                pFrmInfo->u32ID_L       = stFrm.u32ID_L;
                pFrmInfo->u32LumaAddr   = stFrm.u32LumaAddr;
                pFrmInfo->u32TimeStamp  = stFrm.u32TimeStamp;
                pFrmInfo->eFieldType=  E_VDEC_EX_FIELDTYPE_BOTH;

                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_FAIL;
            }
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            HVD_EX_FrameInfo info;
            _VDEC_Memset(&info, 0, sizeof(HVD_EX_FrameInfo));
            if (E_HVD_EX_OK == MDrv_HVD_EX_GetFrmInfo(u32Id, E_HVD_EX_GFRMINFO_DECODE, &info))
            {
                pFrmInfo->eFrameType    = _VDEC_EX_MapFrmType2HVD(info.eFrmType);
                pFrmInfo->u16Height     = info.u16Height;
                pFrmInfo->u16Width      = info.u16Width;
                pFrmInfo->u16Pitch      = info.u16Pitch;
                pFrmInfo->u32ChromaAddr = info.u32ChromaAddr;
                pFrmInfo->u32ID_H       = info.u32ID_H;
                pFrmInfo->u32ID_L       = info.u32ID_L;
                pFrmInfo->u32LumaAddr   = info.u32LumaAddr;
                pFrmInfo->u32TimeStamp  = info.u32TimeStamp;
                pFrmInfo->eFieldType=  _VDEC_EX_MapFieldType2HVD(info.eFieldType);

                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_FAIL;
            }
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();

            MJPEG_FrameInfo info;
            _VDEC_Memset(&info, 0, sizeof(MJPEG_FrameInfo));
            if (E_MJPEG_RET_SUCCESS == MApi_MJPEG_GetDecFrameInfo(u32Id, &info))
            {
                pFrmInfo->eFrameType    =  _VDEC_EX_MapFrmType2MJPEG(info.u8FrmType);
                pFrmInfo->u16Height     = info.u16Height;
                pFrmInfo->u16Width      = info.u16Width;
                pFrmInfo->u16Pitch      = info.u16Pitch;
                pFrmInfo->u32ChromaAddr = info.u32ChromaAddr;
                pFrmInfo->u32ID_H       = info.u32ID_H;
                pFrmInfo->u32ID_L       = info.u32ID_L;
                pFrmInfo->u32LumaAddr   = info.u32LumaAddr;
                pFrmInfo->u32TimeStamp  = info.u32TimeStamp;
                pFrmInfo->eFieldType=  E_VDEC_EX_FIELDTYPE_BOTH;

                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_FAIL;
            }
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_GetDispFrameInfo(VDEC_StreamId *pStreamId, VDEC_EX_FrameInfo *pFrmInfo)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MVD_FrmInfo stFrm;
            _VDEC_Memset(&stFrm, 0, sizeof(MVD_FrmInfo));
            if (E_MVD_RET_OK == MDrv_MVD_GetFrmInfo(u32Id, E_MVD_FRMINFO_DISPLAY, &stFrm))
            {
                pFrmInfo->eFrameType    = _VDEC_EX_MapFrmType2MVD(stFrm.eFrmType);
                pFrmInfo->u16Height     = stFrm.u16Height;
                pFrmInfo->u16Width      = stFrm.u16Width;
                pFrmInfo->u16Pitch      = stFrm.u16Pitch;
                pFrmInfo->u32ChromaAddr = stFrm.u32ChromaAddr;
                pFrmInfo->u32ID_H       = stFrm.u32ID_H;
                pFrmInfo->u32ID_L       = stFrm.u32ID_L;
                pFrmInfo->u32LumaAddr   = stFrm.u32LumaAddr;
                pFrmInfo->u32TimeStamp  = stFrm.u32TimeStamp;
                pFrmInfo->eFieldType=  E_VDEC_EX_FIELDTYPE_BOTH;
                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_FAIL;
            }
            break;
        }

        case E_VDEC_EX_DECODER_HVD:
        {
            HVD_EX_FrameInfo info;

            _VDEC_Memset(&info, 0, sizeof(HVD_EX_FrameInfo));

            if (E_HVD_EX_OK == MDrv_HVD_EX_GetFrmInfo(u32Id, E_HVD_EX_GFRMINFO_DISPLAY, &info))
            {
                pFrmInfo->eFrameType    = _VDEC_EX_MapFrmType2HVD(info.eFrmType);
                pFrmInfo->u16Height     = info.u16Height;
                pFrmInfo->u16Width      = info.u16Width;
                pFrmInfo->u16Pitch      = info.u16Pitch;
                pFrmInfo->u32ChromaAddr = info.u32ChromaAddr;
                pFrmInfo->u32ID_H       = info.u32ID_H;
                pFrmInfo->u32ID_L       = info.u32ID_L;
                pFrmInfo->u32LumaAddr   = info.u32LumaAddr;
                pFrmInfo->u32TimeStamp  = info.u32TimeStamp;
                pFrmInfo->eFieldType =  _VDEC_EX_MapFieldType2HVD(info.eFieldType);
                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_FAIL;
            }
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            MJPEG_FrameInfo info;

            _VDEC_Memset(&info, 0, sizeof(MJPEG_FrameInfo));

            if (E_MJPEG_RET_SUCCESS == MApi_MJPEG_GetDispFrameInfo(u32Id, &info))
            {
                pFrmInfo->eFrameType    = _VDEC_EX_MapFrmType2MJPEG(info.u8FrmType);
                pFrmInfo->u16Height     = info.u16Height;
                pFrmInfo->u16Width      = info.u16Width;
                pFrmInfo->u16Pitch      = info.u16Pitch;
                pFrmInfo->u32ChromaAddr = info.u32ChromaAddr;
                pFrmInfo->u32ID_H       = info.u32ID_H;
                pFrmInfo->u32ID_L       = info.u32ID_L;
                pFrmInfo->u32LumaAddr   = info.u32LumaAddr;
                pFrmInfo->u32TimeStamp  = info.u32TimeStamp;
                pFrmInfo->eFieldType=  E_VDEC_EX_FIELDTYPE_BOTH;
                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_FAIL;
            }
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_GetDecTimeCode(VDEC_StreamId *pStreamId, VDEC_EX_TimeCode* pTimeCode)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);


    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MVD_TimeCode stTimeCode;
            _VDEC_Memset(&stTimeCode, 0, sizeof(MVD_TimeCode));
            if (E_MVD_RET_OK == MDrv_MVD_GetTimeCode(u32Id, E_MVD_FRMINFO_DECODE, &stTimeCode))
            {
                pTimeCode->u8TimeCodeHr  = stTimeCode.u8TimeCodeHr ;
                pTimeCode->u8TimeCodeMin = stTimeCode.u8TimeCodeMin;
                pTimeCode->u8TimeCodeSec = stTimeCode.u8TimeCodeSec;
                pTimeCode->u8TimeCodePic = stTimeCode.u8TimeCodePic;
                pTimeCode->u8DropFrmFlag = stTimeCode.u8DropFrmFlag;

                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_FAIL;
            }
            break;
        }

        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_GetDispTimeCode(VDEC_StreamId *pStreamId, VDEC_EX_TimeCode* pTimeCode)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MVD_TimeCode stTimeCode;
            _VDEC_Memset(&stTimeCode, 0, sizeof(MVD_TimeCode));
            if (E_MVD_RET_OK == MDrv_MVD_GetTimeCode(u32Id, E_MVD_FRMINFO_DISPLAY, &stTimeCode))
            {
                pTimeCode->u8TimeCodeHr  = stTimeCode.u8TimeCodeHr ;
                pTimeCode->u8TimeCodeMin = stTimeCode.u8TimeCodeMin;
                pTimeCode->u8TimeCodeSec = stTimeCode.u8TimeCodeSec;
                pTimeCode->u8TimeCodePic = stTimeCode.u8TimeCodePic;
                pTimeCode->u8DropFrmFlag = stTimeCode.u8DropFrmFlag;

                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_FAIL;
            }
            break;
        }

        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return eRet;
}


VDEC_EX_Result _MApi_VDEC_EX_V2_GetEventInfo(VDEC_StreamId *pStreamId, MS_U32* u32EventFlag)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            eRet = E_VDEC_EX_OK;
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            MS_U32 eEvent = E_HVD_EX_ISR_NONE;
            if( MDrv_HVD_EX_GetISRInfo(u32Id, &eEvent ) )
            {
                *u32EventFlag = E_VDEC_EX_EVENT_OFF;
                if ( eEvent & E_HVD_EX_ISR_DISP_ONE  )
                {
                    *u32EventFlag|=E_VDEC_EX_EVENT_DISP_ONE;
                }
                if (eEvent &E_HVD_EX_ISR_DISP_REPEAT )
                {
                    *u32EventFlag|=E_VDEC_EX_EVENT_DISP_REPEAT;
                }
                if (eEvent &E_HVD_EX_ISR_DISP_WITH_CC )
                {
                    *u32EventFlag|=E_VDEC_EX_EVENT_DISP_WITH_CC;
                }
                if (eEvent &E_HVD_EX_ISR_DISP_FIRST_FRM)
                {
                    *u32EventFlag|=E_VDEC_EX_EVENT_VIDEO_UNMUTE;
                }
                if (eEvent &E_HVD_EX_ISR_DEC_ONE )
                {
                    *u32EventFlag|=E_VDEC_EX_EVENT_DEC_ONE;
                }
                if (eEvent &E_HVD_EX_ISR_DEC_I )
                {
                    *u32EventFlag|=E_VDEC_EX_EVENT_DEC_I;
                }
                if (eEvent &E_HVD_EX_ISR_DEC_HW_ERR )
                {
                    *u32EventFlag|=E_VDEC_EX_EVENT_DEC_ERR;
                }
                if (eEvent &E_HVD_EX_ISR_DEC_CC_FOUND )
                {
                    *u32EventFlag|=E_VDEC_EX_EVENT_USER_DATA_FOUND;
                }
                if (eEvent &E_HVD_EX_ISR_DEC_DISP_INFO_CHANGE )
                {
                    MS_U8 u8Afd = MDrv_HVD_EX_GetActiveFormat(u32Id);
                    if (u8Afd != 0)
                    {
                        *u32EventFlag|=E_VDEC_EX_EVENT_AFD_FOUND;
                    }
                    *u32EventFlag|=E_VDEC_EX_EVENT_DISP_INFO_CHG;
                }
                if (eEvent &E_HVD_EX_ISR_DEC_DATA_ERR )
                {
                    *u32EventFlag|=E_VDEC_EX_EVENT_ES_DATA_ERR;
                }
                if (eEvent &E_HVD_EX_ISR_DEC_FIRST_FRM )
                {
                    *u32EventFlag|=E_VDEC_EX_EVENT_FIRST_FRAME;
                }

                if (eEvent &E_HVD_EX_ISR_DEC_SEQ_HDR_FOUND )
                {
                    *u32EventFlag|=E_VDEC_EX_EVENT_SEQ_HDR_FOUND;
                }
                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_FAIL;
            }
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_GetActiveFormat(VDEC_StreamId *pStreamId,MS_U8* u8ActFmt)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    *u8ActFmt = 0xFF;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            *u8ActFmt = MDrv_MVD_GetActiveFormat(u32Id);

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            *u8ActFmt = MDrv_HVD_EX_GetActiveFormat(u32Id);

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            *u8ActFmt = 0xFF;
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Get stream colour primaries
/// @param eStream \b IN : stream type
/// @return stream colour primaries
//------------------------------------------------------------------------------

VDEC_EX_Result _MApi_VDEC_EX_V2_GetColourPrimaries(VDEC_StreamId *pStreamId,MS_U8* u8ColourPrimaries)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    *u8ColourPrimaries = 0xFF;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            *u8ColourPrimaries = MDrv_MVD_GetColorFormat(u32Id);

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            HVD_EX_DispInfo info;

            _VDEC_Memset(&info, 0, sizeof(HVD_EX_DispInfo));

            if (E_HVD_EX_OK != MDrv_HVD_EX_GetDispInfo(u32Id, &info))
            {
                *u8ColourPrimaries = 0xFF;
            }
            else
            {
                *u8ColourPrimaries = info.u8ColourPrimaries;
            }

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            *u8ColourPrimaries = 0xFF;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_GetFwVersion(VDEC_StreamId *pStreamId, VDEC_EX_FwType eFwType,MS_U32* u32FWVer)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    const MVD_DrvInfo* pMvdDrvInfo = NULL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    *u32FWVer = 0;

    switch (eFwType)
    {
        case E_VDEC_EX_FW_TYPE_MVD:
            pMvdDrvInfo = MDrv_MVD_GetInfo();
            *u32FWVer = pMvdDrvInfo->u32FWVersion;
            if ((TRUE == pVdecExContext->_Attr[u8Idx].bInit) && (E_VDEC_EX_DECODER_MVD == pVdecExContext->_Attr[u8Idx].eDecoder))
            {
                if (MDrv_MVD_GetFWVer(u32Id) != 0)
                {
                    *u32FWVer = MDrv_MVD_GetFWVer(u32Id);
                }
            }
            break;

        case E_VDEC_EX_FW_TYPE_HVD:
            *u32FWVer = MDrv_HVD_EX_GetDrvFwVer();
            if ((TRUE == pVdecExContext->_Attr[u8Idx].bInit) && (E_VDEC_EX_DECODER_HVD == pVdecExContext->_Attr[u8Idx].eDecoder))
            {
                *u32FWVer = MDrv_HVD_EX_GetFwVer(u32Id);
            }
            break;

        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Get picture counter of current GOP
/// @param eStream \b IN : stream type
/// @return picture counter
//------------------------------------------------------------------------------

VDEC_EX_Result _MApi_VDEC_EX_V2_GetGOPCnt(VDEC_StreamId *pStreamId,MS_U8* u8Cnt)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    *u8Cnt = 0;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            *u8Cnt = MDrv_MVD_GetGOPCount(u32Id);
            break;

        case E_VDEC_EX_DECODER_HVD:
            // not supported in HVD
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            // not supported in MJPEG
            break;

        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Get decoder ES buffer read pointer
/// @param eStream \b IN : stream type
/// @return read pointer
//------------------------------------------------------------------------------
VDEC_EX_Result _MApi_VDEC_EX_V2_GetESReadPtr(VDEC_StreamId *pStreamId,MS_VIRT* u32Ptr)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    *u32Ptr = 0;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            *u32Ptr = (MS_VIRT) MDrv_MVD_GetESReadPtr(u32Id);

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            *u32Ptr = MDrv_HVD_EX_GetESReadPtr(u32Id);

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            *u32Ptr = MApi_MJPEG_GetESReadPtr(u32Id);
            _VDEC_EX_API_MutexUnlock();

            break;
        }
        default:
            *u32Ptr = 0;
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Get ES buffer write pointer
/// @param eStream \b IN : stream type
/// @return write pointer
//------------------------------------------------------------------------------
VDEC_EX_Result _MApi_VDEC_EX_V2_GetESWritePtr(VDEC_StreamId *pStreamId,MS_VIRT* u32Ptr)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    *u32Ptr = 0;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            *u32Ptr = MDrv_MVD_GetESWritePtr(u32Id);

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            *u32Ptr = MDrv_HVD_EX_GetESWritePtr(u32Id);

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            *u32Ptr = MApi_MJPEG_GetESWritePtr(u32Id);
            _VDEC_EX_API_MutexUnlock();

            break;
        }
        default:
            *u32Ptr = 0;
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Get the PTS of current displayed frame
/// @param eStream \b IN : stream type
/// @return PTS
//------------------------------------------------------------------------------

VDEC_EX_Result _MApi_VDEC_EX_V2_GetPTS(VDEC_StreamId *pStreamId,MS_U32* u32Pts)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    *u32Pts = 0;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            if(MDrv_MVD_GetPTS(u32Id) != VDEC_U32_MAX)
            {
                *u32Pts = MDrv_MVD_GetPTS(u32Id) + MDrv_MVD_GetAVSyncDelay(u32Id);
            }
            else
            {
                *u32Pts = VDEC_U32_MAX;
            }
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            *u32Pts = MDrv_HVD_EX_GetPTS(u32Id);

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            *u32Pts = MApi_MJPEG_GetPTS(u32Id);
            _VDEC_EX_API_MutexUnlock();

            break;
        }
        default:
            *u32Pts = 0;
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Get the PTS of next displayed frame
/// @return next PTS (unit:ms)
//------------------------------------------------------------------------------

VDEC_EX_Result _MApi_VDEC_EX_V2_GetNextPTS(VDEC_StreamId *pStreamId,MS_U32* u32Pts)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    *u32Pts = 0;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            *u32Pts = MDrv_MVD_GetNextPTS(u32Id);
            break;

        case E_VDEC_EX_DECODER_HVD:
            *u32Pts = MDrv_HVD_EX_GetNextPTS(u32Id);
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            *u32Pts = 0;
            break;

        default:
            *u32Pts = 0;
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Get the value of PTS - STC for video
/// @return the value of PTS - STC for video
//------------------------------------------------------------------------------

VDEC_EX_Result _MApi_VDEC_EX_V2_GetVideoPtsStcDelta(VDEC_StreamId *pStreamId,MS_S64* s64Pts)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    *s64Pts = 0;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            *s64Pts = MDrv_MVD_GetPtsStcDiff(u32Id);
            //VDEC_PRINT("s64Pts=%lld, s32=%ld\n", s64Pts, MDrv_MVD_GetPtsStcDiff(u32Id));
            break;

        case E_VDEC_EX_DECODER_HVD:
            *s64Pts = MDrv_HVD_EX_GetPtsStcDiff(u32Id);
            break;

        default:
            *s64Pts = 0;
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Get error code
/// @param eStream \b IN : stream type
/// @return error code
//------------------------------------------------------------------------------
VDEC_EX_Result _MApi_VDEC_EX_V2_GetErrCode(VDEC_StreamId *pStreamId,VDEC_EX_ErrCode* eErrCode)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if(u8CodecCompare[u8Idx] == 1)
    {
        pVdecExContext->_Attr[u8Idx].eErrCode = E_VDEC_EX_ERR_CODE_CODEC_COMPARE_CASE1;
    }
    else if(u8CodecCompare[u8Idx] == 2)
    {
        pVdecExContext->_Attr[u8Idx].eErrCode = E_VDEC_EX_ERR_CODE_CODEC_COMPARE_CASE2;
    }
    else
    {
    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MVD_ErrCode errCode = E_MVD_ERR_UNKNOWN;
            MVD_ErrStatus errStatus = E_MVD_ERR_STATUS_UNKOWN;

            MDrv_MVD_GetErrInfo(u32Id, &errCode, &errStatus);

            pVdecExContext->_Attr[u8Idx].eErrCode = (VDEC_EX_ErrCode) (errCode + (MS_U32) E_VDEC_EX_MVD_ERR_CODE_BASE);
            //Check if framerate is out of HW spec
            if (E_MVD_ERR_UNKNOWN == errCode)
            {
                //VDEC_PRINT("%s(%d) check MVD FrmRate eErrCode=0x%x\n", __FUNCTION__, __LINE__, _Attr[u8Idx].eErrCode);
                if (FALSE == MDrv_MVD_GetFrmRateIsSupported(u32Id))
                {
                    pVdecExContext->_Attr[u8Idx].eErrCode = E_VDEC_EX_ERR_CODE_FRMRATE_NOT_SUPPORT;
                    //VDEC_PRINT("%s(%d) NS!!! eErrCode=0x%x\n", __FUNCTION__, __LINE__, pVdecExContext->_Attr[u8Idx].eErrCode);
                }

                if ( (bIsSupportDivxPlus == FALSE) && (MDrv_MVD_GetDivxVer(u32Id) != 0) )
                {
                    pVdecExContext->_Attr[u8Idx].eErrCode = E_VDEC_EX_ERR_CODE_DIVX_PLUS_UNSUPPORTED;
                }
            }

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            MS_U32 HVDerrCode = 0;
            HVDerrCode = MDrv_HVD_EX_GetErrCode(u32Id);
            pVdecExContext->_Attr[u8Idx].eErrCode =(VDEC_EX_ErrCode)_VDEC_Map2HVDErrCode(HVDerrCode);
            //Check if framerate is out of HW spec
            if (HVDerrCode == 0)
            {
                //VDEC_PRINT("%s(%d) check HVD FrmRate eErrCode=0x%x\n", __FUNCTION__, __LINE__, _Attr[u8Idx].eErrCode);
                if (FALSE == MDrv_HVD_EX_GetFrmRateIsSupported(u32Id))
                {
                    pVdecExContext->_Attr[u8Idx].eErrCode = E_VDEC_EX_ERR_CODE_FRMRATE_NOT_SUPPORT;
                    //VDEC_PRINT("%s(%d) NS!!! eErrCode=0x%x\n", __FUNCTION__, __LINE__, pVdecExContext->_Attr[u8Idx].eErrCode);
                }
            }

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            pVdecExContext->_Attr[u8Idx].eErrCode = (VDEC_EX_ErrCode) (MApi_MJPEG_GetErrCode(u32Id) + (MS_U32) E_VDEC_EX_MJPEG_ERR_CODE_BASE);
            _VDEC_EX_API_MutexUnlock();

            break;
        }
        default:
            pVdecExContext->_Attr[u8Idx].eErrCode=(VDEC_EX_ErrCode)(0);
            break;
    }
    }

    if (((MS_U32)(pVdecExContext->_Attr[u8Idx].eErrCode) & 0x00ffffff) == 0)
    {
        pVdecExContext->_Attr[u8Idx].eErrCode = (VDEC_EX_ErrCode)(0);
    }

    *eErrCode = pVdecExContext->_Attr[u8Idx].eErrCode;

    return eRet;
}

//------------------------------------------------------------------------------
/// Get accumulated error counter
/// @param eStream \b IN : stream type
/// @return error counter
//------------------------------------------------------------------------------

VDEC_EX_Result _MApi_VDEC_EX_V2_GetErrCnt(VDEC_StreamId *pStreamId,MS_U32* u32Cnt)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    *u32Cnt = 0;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            *u32Cnt = MDrv_MVD_GetVldErrCount(u32Id);
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            *u32Cnt = MDrv_HVD_EX_GetDecErrCnt(u32Id) + MDrv_HVD_EX_GetDataErrCnt(u32Id);

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            *u32Cnt = MApi_MJPEG_GetErrCnt(u32Id);
            _VDEC_EX_API_MutexUnlock();

            break;
        }
        default:
            *u32Cnt = 0;
            break;
    }

    return eRet;
}


//------------------------------------------------------------------------------
/// Get bitrate which decoder retrieved from stream
/// @param eStream \b IN : stream type
/// @return bitrate
//------------------------------------------------------------------------------
VDEC_EX_Result _MApi_VDEC_EX_V2_GetBitsRate(VDEC_StreamId *pStreamId,MS_U32* u32Bitrate)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    *u32Bitrate = 0;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            *u32Bitrate =  MDrv_MVD_GetBitsRate(u32Id);
            break;
        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MJPEG: // unnecessary
        default:
            *u32Bitrate = 0;
            break;
    }

    return eRet;
}

//-----------------------------------------------------------------------------
/// Get accumulated decoded frame Count
/// @param eStream \b IN : stream type
/// @return - decoded frame Count
//-----------------------------------------------------------------------------

VDEC_EX_Result _MApi_VDEC_EX_V2_GetFrameCnt(VDEC_StreamId *pStreamId,MS_U32* u32Cnt)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    *u32Cnt = 0;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            *u32Cnt = MDrv_MVD_GetPicCounter(u32Id) + MDrv_MVD_GetSkipPicCounter(u32Id);

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            *u32Cnt = MDrv_HVD_EX_GetDecodeCnt(u32Id) + MDrv_HVD_EX_GetData(u32Id, E_HVD_EX_GDATA_TYPE_SKIP_CNT);

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            *u32Cnt = MApi_MJPEG_GetFrameCnt(u32Id);
            _VDEC_EX_API_MutexUnlock();

            break;
        }
        default:
            *u32Cnt = 0;
            break;
    }

    if(pVdecExContext->_power_state[u8Idx] == E_VDEC_EX_POWER_RESUME_DONE && *u32Cnt > 0)
    {
        if (E_VDEC_EX_CODEC_TYPE_H264 == pVdecExContext->_Attr[u8Idx].eCodecType)
            _MApi_VDEC_EX_V2_SetBlueScreen(pStreamId, FALSE);
        pVdecExContext->_power_state[u8Idx] = E_VDEC_EX_POWER_NONE;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Get skipped counter
/// @param eStream \b IN : stream type
/// @return counter
//------------------------------------------------------------------------------
VDEC_EX_Result _MApi_VDEC_EX_V2_GetSkipCnt(VDEC_StreamId *pStreamId,MS_U32* u32Cnt)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    *u32Cnt = 0;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            *u32Cnt = MDrv_MVD_GetSkipPicCounter(u32Id);
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            *u32Cnt = MDrv_HVD_EX_GetData(u32Id, E_HVD_EX_GDATA_TYPE_SKIP_CNT);
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
            _VDEC_EX_API_MutexLock();
            *u32Cnt = MApi_MJPEG_GetSkipCnt(u32Id);
            _VDEC_EX_API_MutexUnlock();
            break;

        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Get dropped frame counter
/// @param eStream \b IN : stream type
/// @return counter
//------------------------------------------------------------------------------

VDEC_EX_Result _MApi_VDEC_EX_V2_GetDropCnt(VDEC_StreamId *pStreamId,MS_U32* u32Cnt)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    *u32Cnt = 0;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            *u32Cnt = 0;
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            *u32Cnt = MDrv_HVD_EX_GetData(u32Id, E_HVD_EX_GDATA_TYPE_DROP_CNT);
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
            break;

        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Get displayed frame counter
/// @param pStreamId \b IN : pointer to stream ID
/// @return counter
//------------------------------------------------------------------------------
VDEC_EX_Result _MApi_VDEC_EX_V2_GetDispCnt(VDEC_StreamId *pStreamId,MS_U32* u32Cnt)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    *u32Cnt = 0;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            *u32Cnt = MDrv_MVD_GetDispCount(u32Id);
            break;

        case E_VDEC_EX_DECODER_HVD:
            *u32Cnt = MDrv_HVD_EX_GetData(u32Id, E_HVD_EX_GDATA_TYPE_DISP_CNT);
            break;

        case E_VDEC_EX_DECODER_MJPEG:
            break;

        default:
            *u32Cnt = 0;
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Get current queue vacancy
/// @param eStream \b IN : stream type
/// @return vacancy number
//------------------------------------------------------------------------------
MS_U32 _MApi_VDEC_EX_V2_GetDecQVacancy(VDEC_StreamId *pStreamId,MS_U32* u32Vacancy)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    *u32Vacancy = 0;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            *u32Vacancy = MDrv_MVD_GetQueueVacancy(u32Id, !pVdecExContext->_bVdecDispOutSide[u8Idx]);

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            *u32Vacancy = MDrv_HVD_EX_GetBBUVacancy(u32Id);

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            *u32Vacancy = MApi_MJPEG_GetQueueVacancy(u32Id);
            _VDEC_EX_API_MutexUnlock();

            break;
        }
        default:
            *u32Vacancy = 0;
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Get 3:2 pull down flag which decoder retrieved from stream
/// @param eStream \b IN : stream type
/// @return TRUE/FALSE
//------------------------------------------------------------------------------

VDEC_EX_Result _MApi_VDEC_EX_V2_Is32PullDown(VDEC_StreamId *pStreamId,MS_BOOL* bIs32PullDown)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    *bIs32PullDown = 0;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            *bIs32PullDown =  MDrv_MVD_GetIs32PullDown(u32Id);
            break;
        case E_VDEC_EX_DECODER_HVD:
            *bIs32PullDown = (MDrv_HVD_EX_GetData(u32Id, E_HVD_EX_GDATA_TYPE_FRC_MODE) == E_HVD_EX_FRC_MODE_32PULLDOWN) ? TRUE : FALSE;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            *bIs32PullDown = FALSE;
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Check whether the status of decoder is running or not.
/// @param eStream \b IN : stream type
/// @return VDEC_EX_Result: E_VDEC_EX_OK/E_VDEC_EX_RET_NOT_RUNNING/E_VDEC_EX_RET_NOT_INIT
//------------------------------------------------------------------------------

VDEC_EX_Result _MApi_VDEC_EX_V2_IsAlive(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            eRet = MDrv_MVD_IsAlive(u32Id);
            break;
        case E_VDEC_EX_DECODER_HVD:
        {
            if (E_HVD_EX_RET_NOT_RUNNING == MDrv_HVD_EX_IsAlive(u32Id))
            {
                eRet = E_VDEC_EX_RET_NOT_RUNNING;
            }
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Check cc is available.
/// @param eStream \b IN : stream type
/// @return VDEC_EX_Result: TRUE or NOT
//------------------------------------------------------------------------------

VDEC_EX_Result _MApi_VDEC_EX_V2_IsCCAvailable(VDEC_StreamId *pStreamId,MS_BOOL* is_available)
{
    VDEC_EX_Result ret = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = pVdecExContext->_Attr[u8Idx].u32DrvId;

    *is_available = FALSE;
    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            *is_available = MDrv_MVD_GetUsrDataIsAvailable(u32Id);
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            *is_available = MDrv_HVD_EX_GetUsrDataIsAvailable(u32Id);
            break;
        }
        default:
            break;
    }

    return ret;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_GetCCInfo(VDEC_StreamId *pStreamId, void *pInfo, MS_U32 u32Size)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if(pVdecExContext->support_cc[u8Idx] == FALSE)
    {
        return E_VDEC_EX_RET_UNSUPPORTED;
    }

    MS_U32 u32Version = *((MS_U32 *) pInfo);

    if (u32Version == 0 && u32Size == sizeof(VDEC_EX_CC_Info))
    {
        VDEC_EX_CC_Info *pCCInfo = (VDEC_EX_CC_Info *) pInfo;

        switch (pVdecExContext->_Attr[u8Idx].eDecoder)
        {
            case E_VDEC_EX_DECODER_MVD:
            {
                MVD_UsrDataInfo stUsrInfo;
                _VDEC_Memset(&stUsrInfo, 0, sizeof(MVD_UsrDataInfo));

                _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_GetUsrDataInfo(u32Id, &stUsrInfo));
                if (E_VDEC_EX_OK == eRet)
                {
                    pCCInfo->u8PicStructure     = (VDEC_EX_PicStructure)stUsrInfo.u8PicStruct;
                    pCCInfo->u8TopFieldFirst    = stUsrInfo.u8TopFieldFirst;
                    pCCInfo->u16TempRef         = stUsrInfo.u16TmpRef;
                    pCCInfo->u32Pts             = stUsrInfo.u32Pts;
#if 1//#ifdef VDEC_UTOPIA_2K
                    pCCInfo->u32UserDataBuf     = MsOS_VA2PA(stUsrInfo.u32DataBuf);
#else
                    pCCInfo->u32UserDataBuf     = stUsrInfo.u32DataBuf;
#endif

                    pCCInfo->u32UserDataSize    = stUsrInfo.u8ByteCnt;

                    if(1 == stUsrInfo.u8PicType) //according Mvd4_interface.h
                        pCCInfo->eFrameType = E_VDEC_EX_FRM_TYPE_I;
                    else if(2 == stUsrInfo.u8PicType)
                        pCCInfo->eFrameType = E_VDEC_EX_FRM_TYPE_P;
                    else if(3 == stUsrInfo.u8PicType)
                        pCCInfo->eFrameType = E_VDEC_EX_FRM_TYPE_B;
                    else
                        pCCInfo->eFrameType = E_VDEC_EX_FRM_TYPE_OTHER;
                }
                break;
            }
            case E_VDEC_EX_DECODER_HVD:
            {
                HVD_EX_UserData_Info stUsrInfo;
                _VDEC_Memset(&stUsrInfo, 0, sizeof(HVD_EX_UserData_Info));


                _BOOL_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_GetUserDataInfo(u32Id, &stUsrInfo));
                if (E_VDEC_EX_OK == eRet)
                {
                    pCCInfo->u8PicStructure     = (VDEC_EX_PicStructure)stUsrInfo.u8PicStruct;
                    pCCInfo->u8TopFieldFirst    = stUsrInfo.u8TopFieldFirst;
                    pCCInfo->u16TempRef         = stUsrInfo.u16TmpRef;
                    pCCInfo->u32Pts             = stUsrInfo.u32Pts;
#if 1//#ifdef VDEC_UTOPIA_2K
                    pCCInfo->u32UserDataBuf     = MsOS_VA2PA(stUsrInfo.u32DataBuf);
#else
                    pCCInfo->u32UserDataBuf     = stUsrInfo.u32DataBuf;
#endif
                    pCCInfo->u32UserDataSize    = stUsrInfo.u8ByteCnt;
                }
                break;
            }
            case E_VDEC_EX_DECODER_MJPEG:
            {
                break;
            }
            default:
                break;
        }
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Get current trick decode mode of decoder
/// @param eStream \b IN : stream type
/// @return VDEC_EX_TrickDec
//------------------------------------------------------------------------------
VDEC_EX_Result _MApi_VDEC_EX_V2_GetTrickMode(VDEC_StreamId *pStreamId,VDEC_EX_TrickDec* eTrick)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    *eTrick = E_VDEC_EX_TRICK_DEC_ALL;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MVD_TrickDec dec = MDrv_MVD_GetTrickMode(u32Id);

            if (E_MVD_TRICK_DEC_ALL == dec)
            {
                *eTrick = E_VDEC_EX_TRICK_DEC_ALL;
            }
            else if (E_MVD_TRICK_DEC_IP == dec)
            {
                *eTrick = E_VDEC_EX_TRICK_DEC_IP;
            }
            else if (E_MVD_TRICK_DEC_I == dec)
            {
                *eTrick = E_VDEC_EX_TRICK_DEC_I;
            }
            else
            {
               *eTrick = E_VDEC_EX_TRICK_DEC_NUM;
            }

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            HVD_EX_SkipDecode dec = (HVD_EX_SkipDecode) MDrv_HVD_EX_GetPlayMode(u32Id, E_HVD_EX_GMODE_SKIP_MODE);

            if (E_HVD_EX_SKIP_DECODE_ALL == dec)
            {
                *eTrick = E_VDEC_EX_TRICK_DEC_ALL;
            }
            else if (E_HVD_EX_SKIP_DECODE_I == dec)
            {
                *eTrick = E_VDEC_EX_TRICK_DEC_I;
            }
            else if (E_HVD_EX_SKIP_DECODE_IP == dec)
            {
                *eTrick = E_VDEC_EX_TRICK_DEC_IP;
            }
            else
            {
                *eTrick = E_VDEC_EX_TRICK_DEC_NUM;
            }

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
            break;
        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Get codec type which be initialed
/// @param eStream \b IN : stream type
/// @return VDEC_EX_CodecType
//------------------------------------------------------------------------------

VDEC_EX_Result _MApi_VDEC_EX_V2_GetActiveCodecType(VDEC_StreamId *pStreamId,VDEC_EX_CodecType* CodecType)
{
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    VDEC_EX_Result eRet = E_VDEC_EX_OK;

    *CodecType = pVdecExContext->_Attr[u8Idx].eCodecType;

    return eRet;
}

//------------------------------------------------------------------------------
/// Get the information of the least length of pattern used for specific function
/// @param eStream \b IN : stream type
/// @param pFrmInfo \b IN : the information of the least length of pattern used for specific function
/// @return -the length (Bytes)
//------------------------------------------------------------------------------

VDEC_EX_Result _MApi_VDEC_EX_V2_GetPatternLeastLength(VDEC_StreamId *pStreamId, VDEC_EX_PatternType ePatternType,MS_U32* length)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    *length = 0;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            *length = MDrv_MVD_GetPatternInfo();

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            if (ePatternType == E_VDEC_EX_PATTERN_FLUSH)
            {
                *length = MDrv_HVD_EX_GetPatternInfo(u32Id, E_HVD_EX_FLUSH_PATTERN_SIZE);
            }
            else if (ePatternType == E_VDEC_EX_PATTERN_FILEEND)
            {
                *length = MDrv_HVD_EX_GetPatternInfo(u32Id, E_HVD_EX_DUMMY_HW_FIFO);
            }
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
            break;

        default:
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// For MHEG5 MW, check if I frame decode done
/// @param eStream \b IN : stream type
/// @return VDEC_EX_Result
///     - E_VDEC_EX_OK: decode done
///     - E_VDEC_EX_FAIL: not yet
///     - E_VDEC_EX_RET_NOT_INIT: not initial yet
///     - E_VDEC_EX_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------

VDEC_EX_Result _MApi_VDEC_EX_V2_MHEG_IsIFrameDecoding(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_GetIsIFrameDecoding(u32Id));
            break;

        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// [Obsolete] Get write pointer of CC data buffer.
/// @param eStream \b IN : stream type
/// @param pu32Write \b OUT : current write pointer
/// @return -VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result _MApi_VDEC_EX_V2_CC_GetWritePtr(VDEC_StreamId *pStreamId, MS_U32 *pu32Write)
{
    VDEC_PRINT("%s is obsolete\n", __FUNCTION__);
    UNUSED(pStreamId);
    UNUSED(pu32Write);
    return E_VDEC_EX_RET_UNSUPPORTED;
}

//------------------------------------------------------------------------------
/// [Obsolete] Get the read pointer of CC data buffer.
/// @param eStream \b IN : stream type
/// @param pu32Read \b OUT : current read pointer
/// @return -VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result _MApi_VDEC_EX_V2_CC_GetReadPtr(VDEC_StreamId *pStreamId, MS_U32 *pu32Read)
{
    VDEC_PRINT("%s is obsolete\n", __FUNCTION__);
    UNUSED(pStreamId);
    UNUSED(pu32Read);
    return E_VDEC_EX_RET_UNSUPPORTED;
}

//------------------------------------------------------------------------------
/// [Obsolete] Get if CC data buffer is overflow.
/// @param eStream \b IN : stream type
/// @param pbOverflow \b OUT : overflow flag
/// @return -VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result _MApi_VDEC_EX_V2_CC_GetIsOverflow(VDEC_StreamId *pStreamId, MS_BOOL *pbOverflow)
{
    VDEC_PRINT("%s is obsolete\n", __FUNCTION__);
    UNUSED(pStreamId);
    UNUSED(pbOverflow);
    return E_VDEC_EX_RET_UNSUPPORTED;
}

//------------------------------------------------------------------------------
/// [Obsolete] Get HW key
/// @param eStream \b IN : stream type
/// @return -VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result _MApi_VDEC_EX_V2_GetHWKey(VDEC_StreamId *pStreamId, MS_U8 *pu8Key)
{
    VDEC_EX_Result eRet = E_VDEC_EX_RET_UNSUPPORTED;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    UNUSED(pu8Key);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            break;

        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// [Obsolete] Get ES buffer vancacy
/// @param eStream \b IN : stream type
/// @return -VDEC_EX_Result
//------------------------------------------------------------------------------

VDEC_EX_Result _MApi_VDEC_EX_V2_GetESBuffVacancy(VDEC_StreamId *pStreamId, MS_U32* u32ESBuffVacancy)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;

    *u32ESBuffVacancy = _VDEC_EX_GetESBuffVacancy(pStreamId);

    return eRet;
}

//------------------------------------------------------------------------------
/// [Obsolete] Get ES buffer
/// @param eStream \b IN : stream type
/// @return -VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result _MApi_VDEC_EX_V2_GetESBuff(VDEC_StreamId *pStreamId, MS_U32 u32ReqSize, MS_U32 *u32AvailSize, MS_PHY *u32Addr)
{
    MS_VIRT u32ReadPtr = 0;
    MS_VIRT u32WritePtr = 0;
    MS_VIRT u32WritePtrNew = 0;
    MS_VIRT u32BuffSt = 0;
    MS_VIRT u32BuffEnd = 0;
    MS_VIRT u32Vacancy = 0;
    VDEC_EX_SysCfg *pCfg = NULL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    MS_U32 u32paddingsize = 0;

#ifndef VDEC3
    if(pVdecExContext->bEnableAutoInsertDummyPattern[u8Idx] == TRUE && pVdecExContext->_Attr[u8Idx].eDecoder == E_VDEC_EX_DECODER_MVD)
#endif
    {
        u32paddingsize = 256; // mvd fifo size 256 bytes
    }

    u32ReqSize += u32paddingsize;

    pCfg = &(pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig);

    if (!(pVdecExContext->_bEsBufMgmt[u8Idx]))
    {
        //VDEC_PRINT("Please EnableESBuffMalloc before use GetESBuff\n");
        return E_VDEC_EX_RET_ILLEGAL_ACCESS;
    }

    //check input parameters
    if ((NULL == u32AvailSize) || (NULL == u32Addr))
    {
        //VDEC_PRINT("NULL pointer for output\n");
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    if(pVdecExContext->bFlush[u8Idx] == TRUE)
    {
        if((pCfg->u32BitstreamBufSize-pCfg->u32DrvProcBufSize) < (u32ReqSize+ES_SAFE_SIZE))
        {
            VDEC_INFO(u8Idx, VDEC_PRINT("ESVaca(%x) < ReqSize(%x)\n", (unsigned int)(pCfg->u32BitstreamBufSize-pCfg->u32DrvProcBufSize), (unsigned int)u32ReqSize));
            return E_VDEC_EX_FAIL;
        }

        u32WritePtr = pCfg->u32DrvProcBufSize;
        u32ReadPtr = pCfg->u32DrvProcBufSize;

        *u32Addr = u32WritePtr + pCfg->u32BitstreamBufAddr;
        *u32AvailSize = u32ReqSize+ES_SAFE_SIZE-u32paddingsize;

        VDEC_INFO(u8Idx, VDEC_PRINT("Rd=0x%lx, Wr=0x%lx, WrNew=0x%lx, BsBuff=0x%lx, ReqSize=0x%x\n", (unsigned long)u32ReadPtr, (unsigned long)u32WritePtr,
               (unsigned long)(u32WritePtr+(*u32AvailSize)), (unsigned long)pCfg->u32BitstreamBufAddr, u32ReqSize));

        pVdecExContext->bFlush[u8Idx] = FALSE;

        return E_VDEC_EX_OK;

    }

    if (_VDEC_EX_GetESBuffVacancy(pStreamId) < u32ReqSize)
    {
        VDEC_INFO(u8Idx, VDEC_PRINT("ESVaca(0x%lx) < ReqSize(0x%x)\n", (unsigned long)_VDEC_EX_GetESBuffVacancy(pStreamId), u32ReqSize));
        return E_VDEC_EX_FAIL;
    }

    //check DecCmdQ/BBU vacancy
    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            u32Vacancy = MDrv_MVD_GetQueueVacancy(u32Id, TRUE);
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            u32Vacancy = MDrv_HVD_EX_GetBBUVacancy(u32Id);
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            u32Vacancy = MApi_MJPEG_GetQueueVacancy(u32Id);
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
        {
            u32Vacancy = 0;
            break;
        }
    }

    if(u32Vacancy == 0)
    {
        VDEC_INFO(u8Idx, VDEC_PRINT("DecQ full\n"));
        goto _NO_ES_BUFF;
    }

#if 0//_VDEC_PTS_TABLE
    if (_VDEC_PtsTableIsFull())
    {
        VDEC_INFO(u8Idx, VDEC_INFO("PtsTable full\n"));
        goto _NO_ES_BUFF;
    }
#endif

    //check ES read/write pointer

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            u32ReadPtr = MDrv_MVD_GetESReadPtr(u32Id);
            u32WritePtr = MDrv_MVD_GetESWritePtr(u32Id);
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            u32ReadPtr = MDrv_HVD_EX_GetESReadPtr(u32Id);
            u32WritePtr = MDrv_HVD_EX_GetESWritePtr(u32Id);
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();
            u32ReadPtr = MApi_MJPEG_GetESReadPtr(u32Id);
            u32WritePtr = MApi_MJPEG_GetESWritePtr(u32Id);
            _VDEC_EX_API_MutexUnlock();
            break;
        }
        default:
            u32ReadPtr = u32WritePtr = 0;
            break;
    }

    u32BuffSt = pCfg->u32DrvProcBufSize;
    u32BuffEnd = pCfg->u32BitstreamBufSize;

    if (u32WritePtr == 0)
    {
        u32WritePtr = u32BuffSt;
    }

    u32WritePtr = ((u32WritePtr+3)>>2)<<2;   //4 byte alignment

    u32ReqSize += ES_SAFE_SIZE; //for safety
    u32WritePtrNew = u32WritePtr + u32ReqSize;

    if (u32ReadPtr <= u32WritePtr)
    {
        if (u32WritePtrNew >= u32BuffEnd)
        {
            u32WritePtrNew = u32BuffSt + u32ReqSize;

            if (u32WritePtrNew > u32ReadPtr)
            {
                VDEC_INFO(u8Idx, VDEC_PRINT("xRd=0x%lx, Wr=0x%lx, WrNew=0x%lx, BsBuff=0x%lx, ReqSize=0x%x\n", (unsigned long)u32ReadPtr, (unsigned long)u32WritePtr,
                    (unsigned long)u32WritePtrNew, (unsigned long)(pCfg->u32BitstreamBufAddr), u32ReqSize));
                goto _NO_ES_BUFF;
            }

            u32WritePtr = u32BuffSt;
        }
    }
    else
    {
        if (u32WritePtrNew > u32ReadPtr)
        {
            VDEC_INFO(u8Idx, VDEC_PRINT("xRd=0x%lx, Wr=0x%lx, WrNew=0x%lx, BsBuff=0x%lx, ReqSize=0x%x\n", (unsigned long)u32ReadPtr, (unsigned long)u32WritePtr,
                (unsigned long)u32WritePtrNew, (unsigned long)pCfg->u32BitstreamBufAddr, u32ReqSize));
            goto _NO_ES_BUFF;
        }
    }

    *u32Addr = u32WritePtr + pCfg->u32BitstreamBufAddr;
    *u32AvailSize = u32ReqSize-u32paddingsize;
    VDEC_INFO(u8Idx, VDEC_PRINT("Rd=0x%lx, Wr=0x%lx, WrNew=0x%lx, BsBuff=0x%lx, ReqSize=0x%x\n", (unsigned long)u32ReadPtr, (unsigned long)u32WritePtr,
           (unsigned long)u32WritePtrNew, (unsigned long)pCfg->u32BitstreamBufAddr, u32ReqSize));

    return E_VDEC_EX_OK;

_NO_ES_BUFF:
    *u32AvailSize = 0;
    *u32Addr = NULL;
    return E_VDEC_EX_FAIL;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_GetNextDispFrame(VDEC_StreamId *pStreamId, VDEC_EX_DispFrame *pDispFrm)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if (!pDispFrm)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MVD_FrmInfo stFrm;
            _VDEC_Memset(&stFrm, 0, sizeof(MVD_FrmInfo));
            if (E_MVD_RET_OK != MDrv_MVD_GetFrmInfo(u32Id, E_MVD_FRMINFO_NEXT_DISPLAY, &stFrm))
            {
                return E_VDEC_EX_FAIL;
            }

            VDEC_EX_DispFrame *pDispFrm_tmp = _VDEC_EX_MallocDq(u8Idx);
            if (!pDispFrm_tmp)
            {
                //VDEC_PRINT("No available DispQ!!!\n");
                eRet = E_VDEC_EX_FAIL;
                break;
            }

            pDispFrm->u32Idx = pDispFrm_tmp->u32Idx;
            pDispFrm->stFrmInfo.eFrameType    = _VDEC_EX_MapFrmType2MVD(stFrm.eFrmType);
            pDispFrm->stFrmInfo.u16Height     = stFrm.u16Height;
            pDispFrm->stFrmInfo.u16Width      = stFrm.u16Width;
            pDispFrm->stFrmInfo.u16Pitch      = stFrm.u16Pitch;
            pDispFrm->stFrmInfo.u32ChromaAddr = stFrm.u32ChromaAddr;
            pDispFrm->stFrmInfo.u32ID_H       = stFrm.u32ID_H;
            pDispFrm->stFrmInfo.u32ID_L       = stFrm.u32ID_L;
            pDispFrm->stFrmInfo.u32LumaAddr   = stFrm.u32LumaAddr;
            pDispFrm->stFrmInfo.u32TimeStamp  = stFrm.u32TimeStamp;
            pDispFrm->stFrmInfo.eFieldType    = E_VDEC_EX_FIELDTYPE_BOTH;
            pDispFrm->u32PriData = stFrm.u16FrmIdx;
            //VDEC_PRINT("apiVDEC_MVD pts=%ld, frmIdx=%d\n", pDispFrm->stFrmInfo.u32TimeStamp, stFrm.u16FrmIdx);

            eRet = E_VDEC_EX_OK;
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            HVD_EX_FrameInfo info;
            _VDEC_Memset(&info, 0, sizeof(HVD_EX_FrameInfo));
            if (E_HVD_EX_OK != MDrv_HVD_EX_GetFrmInfo(u32Id, E_HVD_EX_GFRMINFO_NEXT_DISPLAY, &info))
            {
                return E_VDEC_EX_FAIL;
            }
            VDEC_EX_DispFrame *pDispFrm_tmp = _VDEC_EX_MallocDq(u8Idx);

            if (!pDispFrm_tmp)
            {
                //VDEC_PRINT("No available DispQ!!!\n");
                eRet = E_VDEC_EX_FAIL;
                break;
            }

            pDispFrm->u32Idx = pDispFrm_tmp->u32Idx;
            pDispFrm->stFrmInfo.eFrameType    = _VDEC_EX_MapFrmType2HVD(info.eFrmType);
            pDispFrm->stFrmInfo.u16Height     = info.u16Height;
            pDispFrm->stFrmInfo.u16Width      = info.u16Width;
            pDispFrm->stFrmInfo.u16Pitch      = info.u16Pitch;
            pDispFrm->stFrmInfo.u32ChromaAddr = info.u32ChromaAddr;
            pDispFrm->stFrmInfo.u32ID_H       = info.u32ID_H;
            pDispFrm->stFrmInfo.u32ID_L       = info.u32ID_L;
            pDispFrm->stFrmInfo.u32LumaAddr   = info.u32LumaAddr;
            pDispFrm->stFrmInfo.u32TimeStamp  = info.u32TimeStamp;
            pDispFrm->stFrmInfo.eFieldType    = _VDEC_EX_MapFieldType2HVD(info.eFieldType);

            if(pVdecExContext->_bVdecDispOutSide[u8Idx]== TRUE)
            {
                pDispFrm->u32PriData = info.u32PrivateData;//info.u32ID_L; [STB]only for AVC
            }
            else
            {
                pDispFrm->u32PriData = MDrv_HVD_EX_GetNextDispQPtr(u32Id);
            }
            //VDEC_PRINT("apiVDEC pts=%ld pri=0x%lx\n", pDispFrm->stFrmInfo.u32TimeStamp, pDispFrm->u32PriData);

            eRet = E_VDEC_EX_OK;

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();

            MJPEG_FrameInfo info;
            _VDEC_Memset(&info, 0, sizeof(MJPEG_FrameInfo));
            if (E_MJPEG_RET_SUCCESS != MApi_MJPEG_GetNextDispFrame(u32Id, &info))
            {
                _VDEC_EX_API_MutexUnlock();
                return E_VDEC_EX_FAIL;
            }
            VDEC_EX_DispFrame* pDispFrm_tmp = _VDEC_EX_MallocDq(u8Idx);

            if (!pDispFrm_tmp)
            {
                //VDEC_PRINT("No available DispQ!!!\n");
                eRet = E_VDEC_EX_FAIL;
                break;
            }

            pDispFrm->u32Idx = pDispFrm_tmp->u32Idx;
            pDispFrm->stFrmInfo.eFrameType    = E_VDEC_EX_FRM_TYPE_I;
            pDispFrm->stFrmInfo.u16Height     = info.u16Height;
            pDispFrm->stFrmInfo.u16Width      = info.u16Width;
            pDispFrm->stFrmInfo.u16Pitch      = info.u16Pitch;
            pDispFrm->stFrmInfo.u32ChromaAddr = info.u32ChromaAddr;
            pDispFrm->stFrmInfo.u32ID_H       = info.u32ID_H;
            pDispFrm->stFrmInfo.u32ID_L       = info.u32ID_L;
            pDispFrm->stFrmInfo.u32LumaAddr   = info.u32LumaAddr;
            pDispFrm->stFrmInfo.u32TimeStamp  = info.u32TimeStamp;
            pDispFrm->stFrmInfo.eFieldType    = E_VDEC_EX_FIELDTYPE_BOTH;


            pDispFrm->u32PriData = MApi_MJPEG_GetNextDispQPtr(u32Id);
            //VDEC_PRINT("apiVDEC pts=%ld\n", pDispFrm->stFrmInfo.u32TimeStamp);

            eRet = E_VDEC_EX_OK;
            _VDEC_EX_API_MutexUnlock();

            break;
        }
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_GetFrameInfoEx(VDEC_StreamId *pStreamId, VDEC_EX_FrameInfoEX *pFrmInfoEx)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if (!pFrmInfoEx)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
        {
            HVD_EX_FrameInfo info;
            _VDEC_Memset(&info, 0, sizeof(HVD_EX_FrameInfo));

            if (E_HVD_EX_OK != MDrv_HVD_EX_GetFrmInfo(u32Id, E_HVD_EX_GFRMINFO_LAST_DISPLAY, &info))
            {
                return E_VDEC_EX_FAIL;
            }

            pFrmInfoEx->sFrameInfo.u32LumaAddr = info.u32LumaAddr;
            pFrmInfoEx->sFrameInfo.u32ChromaAddr = info.u32ChromaAddr;
            pFrmInfoEx->sFrameInfo.u32TimeStamp = info.u32TimeStamp;
            pFrmInfoEx->sFrameInfo.u32ID_L = info.u32ID_L;
            pFrmInfoEx->sFrameInfo.u32ID_H = info.u32ID_H;
            pFrmInfoEx->sFrameInfo.u16Pitch = info.u16Pitch;
            pFrmInfoEx->sFrameInfo.u16Width = info.u16Width;
            pFrmInfoEx->sFrameInfo.u16Height = info.u16Height;
            pFrmInfoEx->sFrameInfo.eFrameType = info.eFrmType;
            pFrmInfoEx->sFrameInfo.eFieldType = info.eFieldType;

            pFrmInfoEx->u32LumaAddr_2bit = info.u32LumaAddr_2bit;
            pFrmInfoEx->u32ChromaAddr_2bit = info.u32ChromaAddr_2bit;
            pFrmInfoEx->u8LumaBitdepth = info.u8LumaBitdepth;
            pFrmInfoEx->u8ChromaBitdepth = info.u8ChromaBitdepth;
            pFrmInfoEx->u16Pitch_2bit = info.u16Pitch_2bit;

            eRet = E_VDEC_EX_OK;
            break;
        }
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_GetNextDispFrameInfoExt(VDEC_StreamId *pStreamId, void *param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if( param == NULL)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    VDEC_EX_VerCtl *pVerCtl = (VDEC_EX_VerCtl *)param;
    int version = pVerCtl->u32version;
    int size = pVerCtl->u32size;

    if ((version == 0 && size == sizeof(VDEC_EX_FrameInfoExt_version0))
#if defined(VDEC_CAP_HEVC_HDR_V2)
     || (version == 2 && size == sizeof(VDEC_EX_FrameInfoExt_v2))
#endif
#if defined(VDEC_CAP_HEVC_HDR_V3)
     || (version == 3 && size == sizeof(VDEC_EX_FrameInfoExt_v3))
#endif
#if defined(VDEC_CAP_FRAME_INFO_EXT_V4)
     || (version == 4 && size == sizeof(VDEC_EX_FrameInfoExt_v4))
#endif
#if defined(VDEC_CAP_FRAME_INFO_EXT_V5)
     || (version == 5 && size == sizeof(VDEC_EX_FrameInfoExt_v5))
#endif
#if defined(VDEC_CAP_FRAME_INFO_EXT_V6)
     || (version == 6 && size == sizeof(VDEC_EX_FrameInfoExt_v6))
#endif
    )
    {
        // version and size match
    }
    else
    {
        VDEC_ERR("ERROR, ver %d, size %d\n", version, size);
        return E_VDEC_EX_RET_UNSUPPORTED;
    }

    memset(param, 0, size);
    // restore version and size
    pVerCtl->u32version = version;
    pVerCtl->u32size = size;

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            MVD_Color_Info stColorInfo;
             _VDEC_Memset(&stColorInfo, 0, sizeof(MVD_Color_Info));
            if(MDrv_MVD_GetColorInfo(u32Id, &stColorInfo) == FALSE)
            {
                eRet = E_VDEC_EX_FAIL;
            }
            if(version == 0)
            {
                eRet = E_VDEC_EX_FAIL;
            }
#if defined(VDEC_CAP_HEVC_HDR_V2)
            if(version >= 2)
            {
                VDEC_EX_FrameInfoExt_v2 *pFrmInfoEx2 = (VDEC_EX_FrameInfoExt_v2 *)param;

                if(version == 2)
                    memset(param, 0, sizeof(VDEC_EX_FrameInfoExt_v2));
#if defined(VDEC_CAP_HEVC_HDR_V3)
                else if(version == 3)
                    memset(param, 0, sizeof(VDEC_EX_FrameInfoExt_v3));
#endif
#if defined(VDEC_CAP_FRAME_INFO_EXT_V4)
                else if(version == 4)
                    memset(param, 0, sizeof(VDEC_EX_FrameInfoExt_v4));
#endif
#if defined(VDEC_CAP_FRAME_INFO_EXT_V5)
                else if(version == 5)
                    memset(param, 0, sizeof(VDEC_EX_FrameInfoExt_v5));
#endif
#if defined(VDEC_CAP_FRAME_INFO_EXT_V6)
                else if(version == 6)
                    memset(param, 0, sizeof(VDEC_EX_FrameInfoExt_v6));
#endif

                if(stColorInfo.bColor_Descript)
                {
                    pFrmInfoEx2->u8Frm_Info_Ext_avail       = 0x1;  //bit1: SEI_Enabled,  bit0=colur_description_present_flag
                    pFrmInfoEx2->u8Colour_primaries         = stColorInfo.u8Color_Primaries;
                    pFrmInfoEx2->u8Transfer_characteristics = stColorInfo.u8Transfer_Char;
                    pFrmInfoEx2->u8Matrix_coefficients      = stColorInfo.u8Matrix_Coef;
                }
            }
#endif
        break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {

            eRet = E_VDEC_EX_OK;

            HVD_EX_FrameInfo info;
            _VDEC_Memset(&info, 0, sizeof(HVD_EX_FrameInfo));

            if (E_HVD_EX_OK != MDrv_HVD_EX_GetFrmInfo(u32Id, E_HVD_EX_GFRMINFO_LAST_DISPLAY_EX, &info))
            {
                return E_VDEC_EX_FAIL;
            }

            if (version >= 0)
            {
                VDEC_EX_FrameInfoExt_version0 *pFrmInfoEx = (VDEC_EX_FrameInfoExt_version0 *)param;

                pFrmInfoEx->sFrameInfo.u32LumaAddr = info.u32LumaAddr;
                pFrmInfoEx->sFrameInfo.u32ChromaAddr = info.u32ChromaAddr;
                pFrmInfoEx->sFrameInfo.u32TimeStamp = info.u32TimeStamp;
                pFrmInfoEx->sFrameInfo.u32ID_L = info.u32ID_L;
                pFrmInfoEx->sFrameInfo.u32ID_H = info.u32ID_H;
                pFrmInfoEx->sFrameInfo.u16Pitch = info.u16Pitch;
                pFrmInfoEx->sFrameInfo.u16Width = info.u16Width;
                pFrmInfoEx->sFrameInfo.u16Height = info.u16Height;
                pFrmInfoEx->sFrameInfo.eFrameType = info.eFrmType;
                pFrmInfoEx->sFrameInfo.eFieldType = info.eFieldType;

                pFrmInfoEx->u32LumaAddr_2bit = info.u32LumaAddr_2bit;
                pFrmInfoEx->u32ChromaAddr_2bit = info.u32ChromaAddr_2bit;
                pFrmInfoEx->u32LumaAddrI = info.u32LumaAddrI;
                pFrmInfoEx->u32LumaAddrI_2bit = info.u32LumaAddrI_2bit;
                pFrmInfoEx->u32ChromaAddrI = info.u32ChromaAddrI;
                pFrmInfoEx->u32ChromaAddrI_2bit = info.u32ChromaAddrI_2bit;
                pFrmInfoEx->u32MFCodecInfo = info.u32MFCodecInfo;
                pFrmInfoEx->u32LumaMFCbitlen = info.u32LumaMFCbitlen;
                pFrmInfoEx->u32ChromaMFCbitlen = info.u32ChromaMFCbitlen;
                pFrmInfoEx->u16Pitch_2bit = info.u16Pitch_2bit;
                pFrmInfoEx->u8LumaBitdepth = info.u8LumaBitdepth;
                pFrmInfoEx->u8ChromaBitdepth = info.u8ChromaBitdepth;
            }
#if defined(VDEC_CAP_HEVC_HDR_V2)
            if (version >= 2)
            {
                VDEC_EX_FrameInfoExt_v2 *pFrmInfoEx2 = (VDEC_EX_FrameInfoExt_v2 *)param;

                pFrmInfoEx2->sDisplay_colour_volume.u32MaxLuminance    = info.u32MaxLuminance;
                pFrmInfoEx2->sDisplay_colour_volume.u32MinLuminance    = info.u32MinLuminance;
                pFrmInfoEx2->sDisplay_colour_volume.u16Primaries[0][0] = info.u16Primaries[0][0];
                pFrmInfoEx2->sDisplay_colour_volume.u16Primaries[0][1] = info.u16Primaries[0][1];
                pFrmInfoEx2->sDisplay_colour_volume.u16Primaries[1][0] = info.u16Primaries[1][0];
                pFrmInfoEx2->sDisplay_colour_volume.u16Primaries[1][1] = info.u16Primaries[1][1];
                pFrmInfoEx2->sDisplay_colour_volume.u16Primaries[2][0] = info.u16Primaries[2][0];
                pFrmInfoEx2->sDisplay_colour_volume.u16Primaries[2][1] = info.u16Primaries[2][1];
                pFrmInfoEx2->sDisplay_colour_volume.u16WhitePoint[0]   = info.u16WhitePoint[0];
                pFrmInfoEx2->sDisplay_colour_volume.u16WhitePoint[1]   = info.u16WhitePoint[1];
                pFrmInfoEx2->u8Frm_Info_Ext_avail                      = info.u8Frm_Info_Ext_avail;  //bit1: SEI_Enabled,  bit0=colur_description_present_flag
                pFrmInfoEx2->u8Colour_primaries                        = info.u8Colour_primaries;
                pFrmInfoEx2->u8Transfer_characteristics                = info.u8Transfer_characteristics;
                pFrmInfoEx2->u8Matrix_coefficients                     = info.u8Matrix_coefficients;
            }
#endif
#if defined(VDEC_CAP_HEVC_HDR_V3)
            if (version >= 3)
            {
                VDEC_EX_FrameInfoExt_v3 *pFrmInfoEx3 = (VDEC_EX_FrameInfoExt_v3 *)param;

                pFrmInfoEx3->u8DVMode          = info.u8DVMode;
                pFrmInfoEx3->u32DVMetaDataAddr = info.u32DVMetadataAddr;
                pFrmInfoEx3->u32DVMetaDataSize = ((info.u32DVDMSize & 0xFFFF) << 16) | (info.u32DVCompSize & 0xFFFF);
            }
#endif
#if defined(VDEC_CAP_FRAME_INFO_EXT_V4)
            if (version >= 4)
            {
                VDEC_EX_FrameInfoExt_v4 *pFrmInfoEx4 = (VDEC_EX_FrameInfoExt_v4 *)param;

                pFrmInfoEx4->u8CurrentIndex = info.u8CurrentIndex;
                pFrmInfoEx4->u32HDRRegAddr = info.u32HDRRegAddr;
                pFrmInfoEx4->u32HDRRegSize = info.u32HDRRegSize;
                pFrmInfoEx4->u32HDRLutAddr = info.u32HDRLutAddr;
                pFrmInfoEx4->u32HDRLutSize = info.u32HDRLutSize;
                pFrmInfoEx4->bDMEnable = info.bDMEnable;
                pFrmInfoEx4->bCompEnable = info.bCompEnable;

                pFrmInfoEx4->u8ComplexityLevel = info.u8ComplexityLevel;
            }
#endif
#if defined(VDEC_CAP_FRAME_INFO_EXT_V5)
            if (version >= 5)
            {
                VDEC_EX_FrameInfoExt_v5 *pFrmInfoEx5 = (VDEC_EX_FrameInfoExt_v5 *)param;

                pFrmInfoEx5->u32ParWidth = info.u32ParWidth;
                pFrmInfoEx5->u32ParHeight = info.u32ParHeight;
                pFrmInfoEx5->u16CropRight = info.u16CropRight;
                pFrmInfoEx5->u16CropLeft = info.u16CropLeft;
                pFrmInfoEx5->u16CropBottom = info.u16CropBottom;
                pFrmInfoEx5->u16CropTop = info.u16CropTop;

            }
#endif
#if defined(VDEC_CAP_FRAME_INFO_EXT_V6)
            if (version >= 6)
            {
                VDEC_EX_FrameInfoExt_v6 *pFrmInfoEx6 = (VDEC_EX_FrameInfoExt_v6 *)param;
                pFrmInfoEx6->eTileMode          = (VDEC_EX_TileMode)info.u8TileMode;
                pFrmInfoEx6->u16MIUBandwidth    = info.u16MIUBandwidth;
                pFrmInfoEx6->u16Bitrate         = info.u16Bitrate;
                pFrmInfoEx6->u8HTLBTableId      = info.u8HTLBTableId;
                pFrmInfoEx6->u8HTLBEntriesSize  = info.u8HTLBEntriesSize;
                pFrmInfoEx6->u32HTLBEntriesAddr = info.u32HTLBEntriesAddr;
            }
#endif
            break;
        }
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;

}

VDEC_EX_Result _MApi_VDEC_EX_V2_CC_GetInfo(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt, VDEC_EX_CCInfoCmd eCmd, MS_U32 *pOut)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if(pVdecExContext->support_cc[u8Idx] == FALSE)
    {
        return E_VDEC_EX_RET_UNSUPPORTED;
    }

    if (!pOut)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            {
                MS_U8 u8Type = _VDEC_Map2MVDCCFmt(eFmt);
                EN_CC_MVD_INFO eCCInfo = CC_SELECTOR_CCMVD_RINGBUFFER;
                MS_U32 u32Val1=0, u32Val2=0;
                if (E_VDEC_EX_CC_GET_708_ENABLE == eCmd)
                {
                    eCCInfo = CC_SELECTOR_708_SW;
                }
                MDrv_CC_CM_GetInfo(u32Id, eCCInfo, u8Type, &u32Val1, &u32Val2);
                *pOut = u32Val1;
                if (E_VDEC_EX_CC_GET_BUFF_SIZE == eCmd)
                {
                    *pOut = u32Val2;
                }
                eRet = E_VDEC_EX_OK;
            }
            break;

        case E_VDEC_EX_DECODER_HVD:
            {
                MS_U8 u8Type = _VDEC_Map2HVDCCFmt(eFmt);
                EN_CC_HVD_EX_INFO eCCInfo = HVD_EX_CC_SELECTOR_RINGBUFFER;
                MS_U32 u32Val1=0, u32Val2=0;
                if (E_VDEC_EX_CC_GET_708_ENABLE == eCmd)
                {
                    eCCInfo = CC_SELECTOR_708_SW;
                }
                eRet = MDrv_HVD_EX_CC_GetInfo(u32Id, eCCInfo, u8Type, &u32Val1, &u32Val2);
                *pOut = u32Val1;
                if (E_VDEC_EX_CC_GET_BUFF_SIZE == eCmd)
                {
                    *pOut = u32Val2;
                }
                //eRet = E_VDEC_EX_OK;
            }
            break;

        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// [Obsolete] Check CC is RST done
/// @param eStream \b IN : stream type
/// @return -VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result _MApi_VDEC_EX_V2_CC_GetIsRstDone(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if(pVdecExContext->support_cc[u8Idx] == FALSE)
    {
        return E_VDEC_EX_RET_UNSUPPORTED;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            {
                MS_U8 u8Type = _VDEC_Map2MVDCCFmt(eFmt);
                _BOOL_TO_VDEC_RESULT(eRet, MDrv_CC_CM_IsMvdRstDone(u32Id, u8Type));
            }
            break;

        case E_VDEC_EX_DECODER_HVD:
            {
                MS_U8 u8Type = _VDEC_Map2HVDCCFmt(eFmt);
                _BOOL_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_CC_IsHvdRstDone(u32Id, u8Type));
            }
            break;

        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// [Obsolete] Check CC buffer is overflow
/// @param eStream \b IN : stream type
/// @return -VDEC_EX_Result
//------------------------------------------------------------------------------

VDEC_EX_Result _MApi_VDEC_EX_V2_CC_GetIsBuffOverflow(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    if(pVdecExContext->support_cc[u8Idx] == FALSE)
    {
        return E_VDEC_EX_RET_UNSUPPORTED;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            {
                MS_U8 u8Type = _VDEC_Map2MVDCCFmt(eFmt);
                _BOOL_TO_VDEC_RESULT(eRet, MDrv_CC_CM_GetOverflowStatus(u32Id, u8Type));
            }
            break;

        case E_VDEC_EX_DECODER_HVD:
            {
                MS_U8 u8Type = _VDEC_Map2HVDCCFmt(eFmt);
                _BOOL_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_CC_GetOverflowStatus(u32Id, u8Type));
            }
            break;

        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return eRet;
}

//------------------------------------------------------------------------------
/// Report the physical address of write pointer
/// @param eFmt \b IN : the format of closed caption
/// @return MS_PHY
//------------------------------------------------------------------------------

VDEC_EX_Result _MApi_VDEC_EX_V2_CC_GetWriteAdd(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt,MS_PHY* u32CCWrPtr)
{
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    *u32CCWrPtr = VDEC_U32_MAX;
    if(pVdecExContext->support_cc[u8Idx] == FALSE)
    {
        return E_VDEC_EX_RET_UNSUPPORTED;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            {
                MS_U8 u8Type = _VDEC_Map2MVDCCFmt(eFmt);
                *u32CCWrPtr = MDrv_CC_PM_GetMVDRB_WriteAddr(u32Id, u8Type);
            }
            break;
        case E_VDEC_EX_DECODER_HVD:
            {
                MS_U8 u8Type = _VDEC_Map2HVDCCFmt(eFmt);
                *u32CCWrPtr = MDrv_HVD_EX_CC_Get_RB_WriteAddr(u32Id, u8Type);
            }
            break;
        default:
            break;
    }

    return E_VDEC_EX_OK;
}

//------------------------------------------------------------------------------
/// Report the physical address of read pointer
/// @param eFmt \b IN : the format of closed caption
/// @return MS_PHY
//------------------------------------------------------------------------------
VDEC_EX_Result _MApi_VDEC_EX_V2_CC_GetReadAdd(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt,MS_PHY* u32CCRdPtr)
{
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    *u32CCRdPtr = VDEC_U32_MAX;

    if(pVdecExContext->support_cc[u8Idx] == FALSE)
    {
        return E_VDEC_EX_RET_UNSUPPORTED;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            {
                MS_U8 u8Type = _VDEC_Map2MVDCCFmt(eFmt);
                *u32CCRdPtr = MDrv_CC_PM_GetMVDRB_ReadAddr(u32Id, u8Type);
            }
            break;
        case E_VDEC_EX_DECODER_HVD:
            {
                MS_U8 u8Type = _VDEC_Map2HVDCCFmt(eFmt);
                *u32CCRdPtr = MDrv_HVD_EX_CC_Get_RB_ReadAddr(u32Id, u8Type);
            }
            break;
        default:
            break;
    }

    return E_VDEC_EX_OK;
}

//------------------------------------------------------------------------------
/// Get VDEC version
/// @return -the pointer to the VDEC version
//------------------------------------------------------------------------------

VDEC_EX_Result _MApi_VDEC_EX_V2_GetLibVer(MSIF_Version* pVersion)
{
    if (pVersion == NULL)
    {
        return E_VDEC_EX_FAIL;
    }

    _VDEC_Memcpy(pVersion,&_api_vdec_version,sizeof(MSIF_Version));

    return E_VDEC_EX_OK;
}

//-----------------------------------------------------------------------------
/// Get VDEC info
/// @brief \b Function \b Description:  Get information of VDEC API
/// @return - the pointer to the VDEC information
//-----------------------------------------------------------------------------
VDEC_EX_Result _MApi_VDEC_EX_V2_GetInfo(VDEC_EX_Info* pInfo)
{
    if (pInfo == NULL)
    {
        return E_VDEC_EX_FAIL;
    }

    _VDEC_Memcpy(pInfo,&_api_vdec_info,sizeof(VDEC_EX_Info));

    return E_VDEC_EX_OK;
}

//------------------------------------------------------------------------------
/// Check if codec type is supported or not.
/// @param eCodecType \b IN : codec type
/// @return TRUE or FALSE
///     - TRUE: Success
///     - FALSE: Failed
//------------------------------------------------------------------------------
VDEC_EX_Result _MApi_VDEC_EX_V2_CheckCaps(VDEC_StreamId *pStreamId, VDEC_EX_CodecType eCodecType,MS_BOOL* bCaps)
{
    VDEC_EX_Decoder eDecoder;
    VDEC_EX_Result ret = E_VDEC_EX_OK;

    *bCaps = FALSE;

    eDecoder = _VDEC_EX_GetDecoderByCodecType(eCodecType);

    switch (eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        _BOOL_VDEC_MVD_LINT_CHECK();
        {
            MVD_Caps caps;

            _VDEC_Memset(&caps, 0, sizeof(MVD_Caps));

            if (MDrv_MVD_GetCaps(&caps))
            {
                if (E_VDEC_EX_CODEC_TYPE_MPEG2 == eCodecType)
                {
                    *bCaps = caps.bMPEG2;
                }
                else if (E_VDEC_EX_CODEC_TYPE_MPEG4 == eCodecType)
                {
                    *bCaps = caps.bMPEG4;
                }
                else if (E_VDEC_EX_CODEC_TYPE_VC1_ADV == eCodecType || E_VDEC_EX_CODEC_TYPE_VC1_MAIN == eCodecType)
                {
                    *bCaps = caps.bVC1;
                }
            }
            else
            {
                *bCaps = FALSE;
            }

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        _BOOL_VDEC_HVD_LINT_CHECK();
        {
            if (E_VDEC_EX_CODEC_TYPE_H264 == eCodecType)
            {
                *bCaps = MDrv_HVD_EX_GetCaps(E_HVD_EX_AVC);
            }
            if (E_VDEC_EX_CODEC_TYPE_MVC == eCodecType)
            {
                *bCaps = MDrv_HVD_EX_GetCaps(E_HVD_EX_MVC);
            }
            else if (E_VDEC_EX_CODEC_TYPE_AVS == eCodecType)
            {
                *bCaps = MDrv_HVD_EX_GetCaps(E_HVD_EX_AVS);
            }
            else if (E_VDEC_EX_CODEC_TYPE_RV8 == eCodecType || E_VDEC_EX_CODEC_TYPE_RV9 == eCodecType)
            {
                *bCaps = MDrv_HVD_EX_GetCaps(E_HVD_EX_RM);
            }
            else if (E_VDEC_EX_CODEC_TYPE_VP8 == eCodecType)
            {
                *bCaps = MDrv_HVD_EX_GetCaps(E_HVD_EX_VP8);
            }
            else
            {
                *bCaps = FALSE;
            }

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
            ret = E_VDEC_EX_RET_UNSUPPORTED;
            break;

        default:
            ret = E_VDEC_EX_FAIL;
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Enable turbo mode for VDEC.
/// @param bTurbo \b IN : TRUE for enable or FALSE for disable
/// @return VDEC_EX_Result
///     - E_VDEC_EX_OK: success
///     - E_VDEC_EX_FAIL: failed
//------------------------------------------------------------------------------
VDEC_EX_Result _MApi_VDEC_EX_V2_EnableTurboMode(MS_BOOL bEnable)
{
    _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);
    _VDEC_EX_API_MutexLock();
    pVdecExContext->bTurboMode = bEnable;
    _VDEC_EX_API_MutexUnlock();
    return E_VDEC_EX_OK;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_SetSingleDecode(MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8 i = 0;

    _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);

    for (i=0; i<VDEC_MAX_SUPPORT_STREAM_NUM; i++)
    {
        if ((TRUE == pVdecExContext->_Attr[i].bInit) && (TRUE == bEnable))
        {
            VDEC_PRINT("%s must be set before init all tasks (%d)\n", __FUNCTION__, i);
            eRet =  E_VDEC_EX_FAIL;
        }
    }

    _VDEC_EX_API_MutexLock();
    pVdecExContext->_bSingleDecodeMode = bEnable;
    _VDEC_EX_API_MutexUnlock();
    return eRet;
}

//------------------------------------------------------------------------------
/// Get decoder status
/// @param eStream \b IN : stream type
/// @param pStatus \b OUT : VDEC_EX_Status
/// @return TRUE or FALSE
///     - TRUE: Useful status got
///     - FALSE: No status because of not init yet
//------------------------------------------------------------------------------
VDEC_EX_Result _MApi_VDEC_EX_V2_GetStatus(VDEC_StreamId *pStreamId, VDEC_EX_Status *pStatus)
{
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    MS_BOOL bRet = E_VDEC_EX_FAIL;

    if(u8Idx >= VDEC_MAX_SUPPORT_STREAM_NUM)
    {
        return E_VDEC_EX_FAIL;
    }
    _VDEC_SHM_POINTER_CHECK(FALSE);

    pStatus->bInit = pVdecExContext->_Attr[u8Idx].bInit;

    if (!(pVdecExContext->_Attr[u8Idx].bInit))
    {
        pStatus->bIdle = TRUE;
        pStatus->eStage = E_VDEC_EX_STAGE_STOP;
        bRet = E_VDEC_EX_OK;
    }
    else
    {
        pStatus->bIdle = _VDEC_EX_IsIdle(pStreamId);
        switch (pVdecExContext->_Attr[u8Idx].eDecoder)
        {
            case E_VDEC_EX_DECODER_MJPEG:
            case E_VDEC_EX_DECODER_MVD:
            {
                pStatus->eStage = pVdecExContext->_Attr[u8Idx].eStage;
                break;
            }
            case E_VDEC_EX_DECODER_HVD:
            {
                pStatus->eStage =  _VDEC_Map2HVDStatus(MDrv_HVD_EX_GetPlayState(u32Id));
                break;
            }
            default:
                break;
        }
        bRet = E_VDEC_EX_OK;
    }

    return bRet;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_SetPowerState(EN_POWER_MODE u16PowerState)
{
    VDEC_EX_Result ret = E_VDEC_EX_FAIL;
    VDEC_StreamId StreamID;
    MS_U32 i;
    MS_U8 u8Idx;

    _VDEC_Memset(&StreamID,0,sizeof(VDEC_StreamId));

    _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);

    if (u16PowerState == E_POWER_SUSPEND)
    {
        if(pVdecExContext->_Attr[0].bInit == TRUE)
        {
            pVdecExContext->_power_state[0] = E_VDEC_EX_POWER_SUSPEND_ING;
            StreamID.u32Version = 0;
            StreamID.u32Id = (E_VDEC_EX_MAIN_STREAM << 24 | pVdecExContext->_Attr[0].u32DrvId);
            _MApi_VDEC_EX_V2_Exit(&StreamID);
            pVdecExContext->u32ResumeStream[0] = TRUE;
            pVdecExContext->eCodecType[0] = pVdecExContext->_Attr[0].eCodecType;
        }

        if(pVdecExContext->_Attr[1].bInit == TRUE)
        {
            pVdecExContext->_power_state[1] = E_VDEC_EX_POWER_SUSPEND_ING;
            StreamID.u32Version = 0;
            StreamID.u32Id = (E_VDEC_EX_SUB_STREAM << 24 | pVdecExContext->_Attr[1].u32DrvId);
            _MApi_VDEC_EX_V2_Exit(&StreamID);
            pVdecExContext->u32ResumeStream[1] = TRUE;
            pVdecExContext->eCodecType[1] = pVdecExContext->_Attr[1].eCodecType;
        }
        pVdecExContext->_prev_u16PowerState = u16PowerState;
        ret = E_VDEC_EX_OK;
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (pVdecExContext->_prev_u16PowerState == E_POWER_SUSPEND)
        {
            if(pVdecExContext->u32ResumeStream[0] == TRUE)
            {
                pVdecExContext->_power_state[0] = E_VDEC_EX_POWER_RESUME_ING;
                MApi_VDEC_EX_V2_GetFreeStream(&StreamID,
                                  sizeof(VDEC_StreamId),
                                  E_VDEC_EX_MAIN_STREAM,
                                  pVdecExContext->eCodecType[0]);
                MApi_VDEC_EX_V2_Init(&StreamID,&(pVdecExContext->_Attr[0].vdecExInitParam));

                // to do, set control
                pVdecExContext->store_set_cmd_read_pointer[_VDEC_GET_IDX(StreamID.u32Id)] = 0;
                u8Idx = _VDEC_GET_IDX(StreamID.u32Id);
                for(i=0;i<pVdecExContext->store_set_cmd_write_pointer[u8Idx];i++)
                {
                    MApi_VDEC_EX_V2_SetControl(&StreamID,
                                               pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].cmd,
                                               NULL,
                                               TRUE);
                }
                pVdecExContext->store_set_cmd_write_pointer[u8Idx] = 0;
                //~

                _MApi_VDEC_EX_V2_Play(&StreamID);
                pVdecExContext->u32ResumeStream[0] = FALSE;
                pVdecExContext->_power_state[0] = E_VDEC_EX_POWER_RESUME_DONE;
            }

            if(pVdecExContext->u32ResumeStream[1] == TRUE)
            {
                pVdecExContext->_power_state[1] = E_VDEC_EX_POWER_RESUME_ING;
                MApi_VDEC_EX_V2_GetFreeStream(&StreamID,
                                  sizeof(VDEC_StreamId),
                                  E_VDEC_EX_SUB_STREAM,
                                  pVdecExContext->eCodecType[1]);
                MApi_VDEC_EX_V2_Init(&StreamID,&(pVdecExContext->_Attr[1].vdecExInitParam));

                // to do, set control
                pVdecExContext->store_set_cmd_read_pointer[_VDEC_GET_IDX(StreamID.u32Id)] = 0;
                u8Idx = _VDEC_GET_IDX(StreamID.u32Id);
                for(i=0;i<pVdecExContext->store_set_cmd_write_pointer[u8Idx];i++)
                {
                    MApi_VDEC_EX_V2_SetControl(&StreamID,
                                               pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].cmd,
                                               NULL,
                                               TRUE);
                }
                pVdecExContext->store_set_cmd_write_pointer[u8Idx] = 0;
                //~

                _MApi_VDEC_EX_V2_Play(&StreamID);
                pVdecExContext->u32ResumeStream[1] = FALSE;
                pVdecExContext->_power_state[1] = E_VDEC_EX_POWER_RESUME_DONE;
            }
            pVdecExContext->_prev_u16PowerState = u16PowerState;
            ret = E_VDEC_EX_OK;
         }
         else
         {
             VDEC_ERR("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
             ret = E_VDEC_EX_FAIL;
         }
    }
    else
    {
        VDEC_ERR("[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        ret = E_VDEC_EX_FAIL;
    }
    return ret;
}


VDEC_EX_Result _MApi_VDEC_EX_IsFrameRdy(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_Is1stFrmRdy(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        {
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_HVD_EX_Is1stFrmRdy(u32Id));
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            eRet = E_VDEC_EX_OK;
            break;
        }
        default:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return eRet;
}


void _MApi_VDEC_EX_V2_StoreUserCmd(VDEC_EX_RESUME_SET_CMD* pData)
{
    MS_U32 i = 0;
    MS_U8 status = 0;

    if(pVdecExContext->u8enable_store_set_cmd[pData->u8Idx] == TRUE)
    {
        if(pVdecExContext->store_set_cmd_write_pointer[pData->u8Idx] < MAX_NUM_OF_STORE_USER_SET_CMD)
        {
            pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_write_pointer[pData->u8Idx]].cmd = pData->cmd_id;

#if 1
            if(pData->structArg_num == 1)
            {
#if 0
                pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_write_pointer[pData->u8Idx]].pStructArg = malloc(pData->struct_size);
                _VDEC_Memcpy(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_write_pointer[pData->u8Idx]].pStructArg,
                             pData->param[pData->position[MAX_NUM_OF_STORE_USER_SET_CMD_U32ARG]],
                             pData->struct_size);
#endif
                status = 1;
            }
            else if(pData->structArg_num > 1)
            {
                VDEC_ERR("[Error]:%s,struct param number(%d) should not over than 1\n",__FUNCTION__,pData->structArg_num);
                 return;
            }
#endif
            if(pData->u32Arg_num > 0)
            {
                if(pData->u32Arg_num > MAX_NUM_OF_STORE_USER_SET_CMD_U32ARG)
                {
                    VDEC_ERR("[Error]:%s,param_num(%d) is over %d\n",__FUNCTION__,(unsigned int)pData->u32Arg_num,MAX_NUM_OF_STORE_USER_SET_CMD_U32ARG);
#if 0
                    if(pData->structArg_num == 1)
                    {
                        free(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_write_pointer[pData->u8Idx]].pStructArg);
                    }
#endif
                    return;
                }

                for(i = 0;i < pData->u32Arg_num; i++)
                {
                    pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_write_pointer[pData->u8Idx]].u32Arg[i] =
                          *(MS_U32*)(pData->param[pData->position[i]]);
                }
                status = 1;
            }

            if(status == 1)
            {
                pVdecExContext->store_set_cmd_write_pointer[pData->u8Idx]++;
            }
        }
        else
        {
            VDEC_PRINT("[Error]%s,Stored user set command(%d) is over %d\n",__FUNCTION__,pVdecExContext->store_set_cmd_write_pointer[pData->u8Idx],MAX_NUM_OF_STORE_USER_SET_CMD);
        }
    }
}


static VDEC_EX_Result  _MApi_VDEC_EX_SetTimeIncPredictParam(VDEC_StreamId *pStreamId,MS_U32 u32Para)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {

        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetTimeIncPredictParam(u32Id,u32Para));
             eRet = E_VDEC_EX_OK;
             break;
        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MJPEG:
        default:
             eRet = E_VDEC_EX_RET_UNSUPPORTED;
             break;
    }
    return eRet;

}


static VDEC_EX_Result  _VDEC_EX_SetDecodeTimeoutParam(VDEC_StreamId *pStreamId,VDEC_EX_Decode_Timeout_Param* u32Para)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {

        case E_VDEC_EX_DECODER_MVD:
             if(TRUE == MDrv_MVD_SetDcodeTimeoutParam(u32Id,u32Para->bEnable,u32Para->u32Timeout))
             {
                eRet = E_VDEC_EX_OK;
             }
             else
             {
                eRet = E_VDEC_EX_FAIL;
             }
             break;
        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MJPEG:
        default:
             eRet = E_VDEC_EX_RET_UNSUPPORTED;
             break;
    }
    return eRet;

}

static VDEC_EX_Result  _MApi_VDEC_EX_SetFramebufferAutoMode(VDEC_StreamId *pStreamId,MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            if(MDrv_MVD_SetFramebufferAutoMode(u32Id,bEnable) == TRUE)
            {
                eRet = E_VDEC_EX_OK;
            }
            else
            {
                eRet = E_VDEC_EX_FAIL;
            }
            break;
        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MJPEG:
        default:
             eRet = E_VDEC_EX_RET_UNSUPPORTED;
             break;
    }
    return eRet;

}

//-------------------------------------------------------------------------------------------------
//  API Layer set smooth rewind Internal Function
//-------------------------------------------------------------------------------------------------
static VDEC_EX_Result _MApi_VDEC_EX_SetSmooth_Rewind(VDEC_StreamId *pStreamId,MS_U8 u8type)
{
    VDEC_EX_Result eRet  = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_Set_Smooth_Rewind(u32Id, u8type));
            break;
        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return eRet;
}


//-------------------------------------------------------------------------------------------------
//  API Layer set error tolerance Internal Function
//-------------------------------------------------------------------------------------------------
static VDEC_EX_Result _MApi_VDEC_EX_SetError_Tolerance(VDEC_StreamId *pStreamId,VDEC_EX_Err_Tolerance *pstErrTolerance)
{
    VDEC_EX_Result eRet  = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    MS_U16 u16Para;
    u16Para = (((pstErrTolerance->u8Tolerance << 8)&0xff00) | (pstErrTolerance->bEnable &0x00ff));
    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_Set_Err_Tolerance(u32Id, u16Para));
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_Set_Err_Tolerance(u32Id, (MS_U32)u16Para));
            break;
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return eRet;
}

#ifdef VDEC_CAP_DV_OTT_API
//-------------------------------------------------------------------------------------------------
//  API Layer set DV profile and level Internal Function
//-------------------------------------------------------------------------------------------------
static VDEC_EX_Result _MApi_VDEC_EX_SetDVInfo(VDEC_StreamId *pStreamId, VDEC_EX_DV_Info *pDV_Info)
{
    VDEC_EX_Result eRet  = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    MS_U32 u32Para;
#ifdef VDEC_CAP_DV_OTT_API_V2
    u32Para = (((pDV_Info->eDVMetaReorder << 16) & 0xff0000) | ((pDV_Info->eDVProfile << 8) & 0xff00) | (pDV_Info->eDVLevel & 0x00ff));
#else
    u32Para = (((pDV_Info->eDVProfile << 8) & 0xff00) | (pDV_Info->eDVLevel & 0x00ff));
#endif
    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_SetDVInfo(u32Id, u32Para));
            break;
        case E_VDEC_EX_DECODER_MJPEG:
        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return eRet;
}
#endif

VDEC_EX_Result _MApi_VDEC_EX_V2_GetEventFlag(VDEC_StreamId *pStreamId,MS_U32* u32EventFlag,  MS_U32 cb_idx)
{
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32VdecEventFlag = 0;

    if(pVdecExContext->_u32PreVdecExIsrEventFlag[u8Idx] != pVdecExContext->_u32VdecExIsrEventFlag[u8Idx])
    {
        MsOS_ClearEvent(s32VdecExEventId[u8Idx], VDEC_U32_MAX);
        pVdecExContext->_u32PreVdecExIsrEventFlag[u8Idx] = pVdecExContext->_u32VdecExIsrEventFlag[u8Idx];
    }


    MsOS_WaitEvent(s32VdecExEventId[u8Idx],
        (pVdecExContext->_u32PreVdecExIsrEventFlag[u8Idx] & ~E_VDEC_EX_EVENT_DEC_CMA_ACTION),
        &u32VdecEventFlag,
        E_OR_CLEAR,
        TICK_PER_ONE_MS*10);


    if ((u32VdecEventFlag & ~E_VDEC_EX_EVENT_ISR_EVENT_CHANGE) & pVdecExContext->_u32VdecExIsrEventFlag[u8Idx])
    {
        if(u32EventFlag!=NULL)
        {
           *u32EventFlag = u32VdecEventFlag;
        }
    }

    return E_VDEC_EX_OK;
}

static VDEC_EX_Result  _MApi_VDEC_EX_Set_PTS_USec_Mode(VDEC_StreamId *pStreamId,MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_SetPTSUsecMode(u32Id,bEnable));
            break;
        case E_VDEC_EX_DECODER_MVD:
        case E_VDEC_EX_DECODER_MJPEG:
        default:
             eRet = E_VDEC_EX_RET_UNSUPPORTED;
             break;
    }
    return eRet;

}

static VDEC_EX_Result  _MApi_VDEC_EX_Enable_AutoInsertDummyPattern(VDEC_StreamId *pStreamId,MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            pVdecExContext->bEnableAutoInsertDummyPattern[u8Idx] = bEnable;
            MDrv_MVD_EnableAutoInsertDummyPattern(u32Id,bEnable);
            break;
        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MJPEG:
        default:
             eRet = E_VDEC_EX_RET_UNSUPPORTED;
             break;
    }
    return eRet;

}


static VDEC_EX_Result _MApi_VDEC_EX_Drop_One_PTS(VDEC_StreamId *pStreamId)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            MDrv_MVD_Drop_One_PTS(u32Id);
            break;
        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MJPEG:
        default:
             eRet = E_VDEC_EX_OK;
             break;
    }
    return eRet;

}


static VDEC_EX_Result  _MApi_VDEC_EX_PVR_Seamless_mode(VDEC_StreamId *pStreamId,MS_U8 u8Arg)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            if(MDrv_MVD_PVR_Seamless_mode(u32Id,u8Arg) != E_MVD_RET_OK)
            {
                eRet = E_VDEC_EX_FAIL;
            }
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_PVRTimeShiftSeamlessMode(u32Id, u8Arg));
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MJPEG:
        default:
             eRet = E_VDEC_EX_RET_UNSUPPORTED;
             break;
    }
    return eRet;

}

static VDEC_EX_Result _VDEC_EX_GetPVRSeamlessInfo(VDEC_StreamId *pStreamId, VDEC_EX_PVR_Seamless_Info* param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
        {
            HVD_EX_PVR_Seamless_Info info;

            _VDEC_Memset(&info, 0, sizeof(HVD_EX_PVR_Seamless_Info));

            if(MDrv_HVD_EX_GetPVRSeamlessInfo(u8Idx, &info) == TRUE)
            {
                _VDEC_Memcpy(param, &info, sizeof(HVD_EX_PVR_Seamless_Info));

                eRet = E_VDEC_EX_OK;
            }

            break;
        }
        case E_VDEC_EX_DECODER_MVD:
        {
            if(MDrv_MVD_GetPVRSeamlessInfo(u8Idx,(void*)param) == TRUE)
            {
                eRet = E_VDEC_EX_OK;
            }
            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        default:
        {
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        }
    }

    return eRet;
}

static VDEC_EX_Result _MApi_VDEC_EX_V2_GetFWStatusFlag(VDEC_StreamId *pStreamId, MS_U32 *param)
{
     VDEC_EX_Result ret = E_VDEC_EX_FAIL;
     MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
     MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
     _VDEC_INVALID_IDX_RET(u8Idx);

     switch (pVdecExContext->_Attr[u8Idx].eDecoder)
     {
         case E_VDEC_EX_DECODER_HVD:
             *param = MDrv_HVD_EX_GetData(u32Id, E_HVD_EX_GDATA_TYPE_FW_STATUS_FLAG);
             ret = E_VDEC_EX_OK;
             break;
         case E_VDEC_EX_DECODER_MVD:
         case E_VDEC_EX_DECODER_MJPEG:
         default:
             ret = E_VDEC_EX_RET_UNSUPPORTED;
             break;
     }

     return ret;

}

MS_BOOL _MApi_VDEC_EX_IsSecureModeEnable(void)
{
    MS_BOOL bSecureMode = FALSE;
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        if(UtopiaOpen(MODULE_VDEC_EX | VDEC_DRIVER_BASE, &pu32VDEC_EX_Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            VPRINTF("UtopiaOpen VDEC_EX failed\n");
            return E_VDEC_EX_FAIL;
        }
    }

    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_SECURE_MODE;
    IO_arg.param[0] = (void*)(&bSecureMode);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret =  E_VDEC_EX_FAIL;
    }
    return bSecureModeEnableUser;
}

VDEC_EX_Result _MApi_VDEC_EX_V2_IsSecureMode(MS_BOOL* bIsSecureMode)
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    printk("[_MApi_VDEC_EX_V2_IsSecureMode] bSecureModeEnable = %d \n",bSecureModeEnable);
#endif
#if defined(ForceToNonteeTestmode)
    *bIsSecureMode = 0;  //Force to non-tee mode,test mode
#else
    *bIsSecureMode = bSecureModeEnable;
#endif
    return E_VDEC_EX_OK;
}

MS_BOOL _MApi_VDEC_EX_Set_MBX_param(MS_U8 u8APIMbxMsgClass)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        if(UtopiaOpen(MODULE_VDEC_EX | VDEC_DRIVER_BASE, &pu32VDEC_EX_Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            VPRINTF("UtopiaOpen VDEC_EX failed\n");
            return E_VDEC_EX_FAIL;
        }
    }

    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_SET_MBX_PARAM;
    IO_arg.param[0] = (void*)(&u8APIMbxMsgClass);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret =  E_VDEC_EX_FAIL;
    }
    return E_VDEC_EX_OK;
}

MS_BOOL _MApi_VDEC_EX_V2_Set_MBX_param(MS_U8 u8APIMbxMsgClass)
{
    MDrv_MVD_Set_MBX_param(u8APIMbxMsgClass);
    return E_VDEC_EX_OK;
}

#ifndef MSOS_TYPE_LINUX_KERNEL
static MS_BOOL _MApi_VDEC_EX_RegisterMBX(void)
{

    VPRINTF("_MApi_VDEC_EX_RegisterMBX \n");
    MBX_Result  mbxRet = 0;

    mbxRet = MApi_MBX_Init(E_MBX_CPU_MIPS,E_MBX_ROLE_HK,1000);
    if (mbxRet != E_MBX_SUCCESS && mbxRet != E_MBX_ERR_SLOT_AREADY_OPENNED)
    {
        VPRINTF("VDEC_TEE MApi_MBX_Init fail\n");
        return FALSE;
    }
    else
    {
        MApi_MBX_Enable(TRUE);
    }
#if 0
    MS_U8 ClassNum = 0;
    MBX_Result result;
    if (E_MBX_SUCCESS != MApi_MBX_QueryDynamicClass(E_MBX_CPU_MIPS_VPE1, "VDEC_TEE", (MS_U8 *)&ClassNum))
    {
        VPRINTF("VDEC_TEE MApi_MBX_QueryDynamicClass fail\n");
        return FALSE;
    }

    result = MApi_MBX_RegisterMSG(ClassNum, 10);

    if (( E_MBX_SUCCESS != result) && ( E_MBX_ERR_SLOT_AREADY_OPENNED != result ))
    {
        VPRINTF("%s fail\n",__FUNCTION__);
        return FALSE;
    }
    else
    {
        _MApi_VDEC_EX_Set_MBX_param(ClassNum);
        return TRUE;
    }
#endif
    return TRUE;
}
#endif

//------------------------------------------------------------------------------
/// set share memory base address between driver with fw
/// @param u32FrmBuffAddr \b IN : frame buffer address
/// @param u32FrmBuffSize \b IN : frame buffer size
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_EX_Result _MApi_VDEC_EX_SetFrameBuff2(VDEC_StreamId *pStreamId, MS_VIRT u32FrmBuffAddr, MS_U32 u32FrmBuffSize)
{
    VDEC_EX_Result ret = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);

    if (!pVdecExContext->_Attr[u8Idx].bInit)
    {
        return E_VDEC_EX_RET_ILLEGAL_ACCESS;
    }

#if !VDEC_EX_ALLOCATOR_ENABLE_NDECODE_FB2
    if(_VDEC_EX_NdecAllocator_IsNDecode() == TRUE)
    {
        return E_VDEC_EX_OK;
    }
#endif

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            ret = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_HVD:
            if((pVdecExContext->_Attr[u8Idx].eCodecType == E_VDEC_EX_CODEC_TYPE_HEVC) ||
               (pVdecExContext->_Attr[u8Idx].eCodecType == E_VDEC_EX_CODEC_TYPE_HEVC_DV) ||
               (pVdecExContext->_Attr[u8Idx].eCodecType == E_VDEC_EX_CODEC_TYPE_VP9))
            {
                _HVD_RET_TO_VDEC_RESULT(ret, MDrv_HVD_EX_SetFrmBuff2(u32Id, u32FrmBuffAddr, u32FrmBuffSize));
                if(bSecureModeEnable == TRUE)
                {
                    SYS_TEEINFO teemode;
                    MDrv_SYS_ReadKernelCmdLine();
                    MDrv_SYS_GetTEEInfo(&teemode);

                    if(teemode.OsType == SYS_TEEINFO_OSTYPE_OPTEE)
                    {
                        VDEC_EX_OPTEE_SecureFB2 SecureFB2;
                        SecureFB2.FBuf2Addr = u32FrmBuffAddr;
                        SecureFB2.FBuf2Size = u32FrmBuffSize;
                        SecureFB2.u32streamID = pStreamId->u32Id;
                        VPRINTF("[VDEC][OPTEE]Send Frame Buffer 2 command\n");
                        if(_MApi_VDEC_EX_V2_Send_OPTEE_CMD(E_VDEC_EX_OPTEE_SECURE_FB2,&SecureFB2,sizeof(SecureFB2)) == FALSE)
                        {
                           VPRINTF("[VDEC][OPTEE]Set Frame Buffer 2 command fail\n");
                           return E_VDEC_EX_FAIL;
                        }
                    }
                }
            }
            else
            {
                ret = E_VDEC_EX_RET_UNSUPPORTED;
            }
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            ret = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            ret = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return ret ;
}

static VDEC_EX_Result _MApi_VDEC_EX_V2_SetDVXCShmAddr(VDEC_StreamId *pStreamId, MS_PHY u32DVXCShmAddr)
{
    VDEC_EX_Result ret = E_VDEC_EX_FAIL;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);

    if (!pVdecExContext->_Attr[u8Idx].bInit)
    {
        return E_VDEC_EX_RET_ILLEGAL_ACCESS;
    }

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            /*
            if(MDrv_MVD_SetDVXCShmAddr(u32Id,pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32CodeBufAddr,u32DVXCShmAddr) == E_MVD_RET_OK)
            {
                ret = E_VDEC_EX_OK;
            }
            */
            ret = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_TO_VDEC_RESULT(ret, MDrv_HVD_EX_SetDVXCShmBuff(u32Id, pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32CodeBufAddr,
                                    u32DVXCShmAddr, (MS_SIZE)0));

            break;
        case E_VDEC_EX_DECODER_MJPEG:
            ret = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            ret = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return ret ;
}

static VDEC_EX_Result _MApi_VDEC_EX_SetTrickPlay_2X_MODE(VDEC_StreamId *pStreamId, VDEC_EX_TrickPlay2xMode eMode)
{
    VDEC_EX_Result ret = E_VDEC_EX_FAIL;
     MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:

            if(eMode == E_VDEC_EX_TRICKPLAY2X_COUNTVSYNC)
            {
                MDrv_MVD_TrickPlay2xAVSync(pStreamId->u32Id,FALSE);
            }
            else if(eMode == E_VDEC_EX_TRICKPLAY2X_AVSYNC)
            {
                MDrv_MVD_TrickPlay2xAVSync(pStreamId->u32Id,TRUE);
            }
            else
            {
                MDrv_MVD_TrickPlay2xAVSync(pStreamId->u32Id,FALSE);
            }
            ret = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_HVD:
        case E_VDEC_EX_DECODER_MJPEG:
            ret = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            ret = E_VDEC_EX_RET_UNSUPPORTED;
            break;
    }

    return ret ;

}

static VDEC_EX_Result  _MApi_VDEC_EX_FRC_OnlyShowTopField(VDEC_StreamId *pStreamId,MS_BOOL bEnable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_FRC_OnlyShowTopField(u32Id,bEnable));
            break;
        case E_VDEC_EX_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_FRC_OnlyShowTopField(u32Id, bEnable));
            break;
        case E_VDEC_EX_DECODER_MJPEG:
        default:
             eRet = E_VDEC_EX_RET_UNSUPPORTED;
             break;
    }
    return eRet;

}

static VDEC_EX_Result  _MApi_VDEC_EX_DisableEsFullStop(VDEC_StreamId *pStreamId,MS_BOOL bDisable)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(eRet, MDrv_MVD_DisableEsFullStop(u32Id, bDisable));
            break;
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_DisableEsFullStop(u32Id,bDisable));
            break;
        case E_VDEC_EX_DECODER_MJPEG:
        default:
             eRet = E_VDEC_EX_RET_UNSUPPORTED;
             break;
    }
    return eRet;

}

//-------------------------------------------------------------------------------------------------
//  API Layer set slow syncl Function
//-------------------------------------------------------------------------------------------------
static VDEC_EX_Result _MApi_VDEC_EX_SetSlow_Sync(VDEC_StreamId *pStreamId,VDEC_EX_Slow_Sync *pstSlowSync)
{
    VDEC_EX_Result eRet  = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    MS_U16 u16Para;
    u16Para = (((pstSlowSync->u8RepeatPeriod<< 8)&0xff00) | (pstSlowSync->u8DropPeriod&0x00ff));
    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EX_Set_Slow_Sync(u32Id, (MS_U32)u16Para));
            break;
        case E_VDEC_EX_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_Set_SlowSyncParam(u32Id, pstSlowSync->u8RepeatPeriod,pstSlowSync->u8DropPeriod));
            break;
        case E_VDEC_EX_DECODER_MJPEG:
            eRet = E_VDEC_EX_RET_UNSUPPORTED;
            break;
        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return eRet;
}

static VDEC_EX_Result  _MApi_VDEC_EX_Direct_STC_Mode(VDEC_StreamId *pStreamId, MS_U32 u32StcInMs)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    MS_U16 u16ChipECORev = (MS_U16)MDrv_SYS_GetChipRev();

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_HVD:
#if defined(CHIP_KANO) || defined(CHIP_CURRY)
            if(u16ChipECORev == 0)
            {
                _HVD_RET_HANDLE(MDrv_HVD_EX_DirectSTCMode(u32Id, u32StcInMs));
            }
#endif
            eRet = E_VDEC_EX_OK;
            break;
        case E_VDEC_EX_DECODER_MVD:
        case E_VDEC_EX_DECODER_MJPEG:
        default:
             eRet = E_VDEC_EX_RET_UNSUPPORTED;
             break;
    }

    return eRet;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Get VDEC version
/// @return -the pointer to the VDEC version
//------------------------------------------------------------------------------

VDEC_EX_Result MApi_VDEC_EX_GetLibVer(const MSIF_Version **ppVersion)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MSIF_Version Version;

    if (pu32VDEC_EX_Inst == NULL)
    {
        if(UtopiaOpen(MODULE_VDEC_EX | VDEC_DRIVER_BASE, &pu32VDEC_EX_Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            VPRINTF("UtopiaOpen VDEC_EX failed\n");
            return E_VDEC_EX_FAIL;
        }
    }

    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GETLIBVER;
    IO_arg.param[0] = (void*)(&Version);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret =  E_VDEC_EX_FAIL;
    }
    else
    {
        *ppVersion = &_api_vdec_version;
    }
    return ret;
}


//-----------------------------------------------------------------------------
/// Get VDEC info
/// @brief \b Function \b Description:  Get information of VDEC API
/// @return - the pointer to the VDEC information
//-----------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_GetInfo(const VDEC_EX_Info **ppInfo)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    VDEC_EX_Info Info;

    if (pu32VDEC_EX_Inst == NULL)
    {
        if(UtopiaOpen(MODULE_VDEC_EX | VDEC_DRIVER_BASE, &pu32VDEC_EX_Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            VPRINTF("UtopiaOpen VDEC_EX failed\n");
            return E_VDEC_EX_FAIL;
        }
    }

    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GETINFO;
    IO_arg.param[0] = (void*)(&Info);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }
    else
    {
        *ppInfo = &_api_vdec_info;
    }
    return ret;
}

//------------------------------------------------------------------------------
/// Get decoder status
/// @param eStream \b IN : stream type
/// @param pStatus \b OUT : VDEC_EX_Status
/// @return TRUE or FALSE
///     - TRUE: Useful status got
///     - FALSE: No status because of not init yet
//------------------------------------------------------------------------------
MS_BOOL MApi_VDEC_EX_GetStatus(VDEC_StreamId* pStreamId, VDEC_EX_Status* pStatus)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_PREGETSTATUS;
    IO_arg.param[0] = (void*)pStatus;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,E_VDEC_EX_V2_CMD_PRE_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    else
    {
        if(ret != E_VDEC_EX_OK)
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
}


//------------------------------------------------------------------------------
/// Check if codec type is supported or not.
/// @param eCodecType \b IN : codec type
/// @return TRUE or FALSE
///     - TRUE: Success
///     - FALSE: Failed
//------------------------------------------------------------------------------
MS_BOOL MApi_VDEC_EX_CheckCaps(VDEC_StreamId *pStreamId, VDEC_EX_CodecType eCodecType)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_BOOL bCaps = FALSE;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return bCaps;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.param[0] = (void*)(&eCodecType);
    IO_arg.param[1] = (void*)(&bCaps);
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_CHECKCAPS;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return bCaps;
}


//------------------------------------------------------------------------------
/// Enable turbo mode for VDEC.
/// @param bTurbo \b IN : TRUE for enable or FALSE for disable
/// @return VDEC_EX_Result
///     - E_VDEC_EX_OK: success
///     - E_VDEC_EX_FAIL: failed
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_EnableTurboMode(MS_BOOL bEnable)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        if(UtopiaOpen(MODULE_VDEC_EX | VDEC_DRIVER_BASE, &pu32VDEC_EX_Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            VPRINTF("UtopiaOpen VDEC_EX failed\n");
            return E_VDEC_EX_FAIL;
        }
    }

    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_PRESET_ENABLETURBOMODE;
    IO_arg.param[0] = (void*)(&bEnable);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_PRE_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}


VDEC_EX_Result MApi_VDEC_EX_SetSingleDecode(MS_BOOL bEnable)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        if(UtopiaOpen(MODULE_VDEC_EX | VDEC_DRIVER_BASE, &pu32VDEC_EX_Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            VPRINTF("UtopiaOpen VDEC_EX failed\n");
            return E_VDEC_EX_FAIL;
        }
    }

    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_PRESETSINGLEDECODE;
    IO_arg.param[0] = (void*)(&bEnable);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_PRE_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}



//------------------------------------------------------------------------------
/// Check if information for display setting is ready or not
/// @param eStream \b IN : stream type
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_CheckDispInfoRdy(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_CHECK_DISPINFO_READY;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Set up frame rate conversion mode
/// @param eStream \b IN : stream type
/// @param eFrcMode \b IN : frame rate conversion mode
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetFrcMode(VDEC_StreamId *pStreamId, VDEC_EX_FrcMode eFrcMode)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_SET_FRC_MODE;
    IO_arg.param[0] = (void*)(&eFrcMode);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Set up some parameters about dynamic scaling in FW
/// @param eStream \b IN : stream type
/// @param u32Addr \b IN : the physical start address of parameter set
/// @param u32Size \b IN : the size of parameter set
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetDynScalingParams(VDEC_StreamId *pStreamId, MS_PHY u32Addr, MS_U32 u32Size)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_SET_DYNSCALING_PARAMS;
    IO_arg.param[0] = (void*)(&u32Addr);
    IO_arg.param[1] = (void*)(&u32Size);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Set up debug message level
/// @param eStream \b IN : stream type
/// @param eDbgLevel \b IN : message level
/// @return VDEC_EX_Result
///     - E_VDEC_EX_OK: Success
///     - E_VDEC_EX_FAIL: Failed
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetDbgLevel(VDEC_StreamId *pStreamId, VDEC_EX_DbgLevel eDbgLevel)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_SET_DBG_LEVEL;
    IO_arg.param[0] = (void*)(&eDbgLevel);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}


//------------------------------------------------------------------------------
/// Start playback
/// @param pStreamId \b IN : the pointer to stream ID
/// @return return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_Play(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_PLAY;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Pause decode and display
/// @param eStream \b IN : stream type
/// @return return VDEC_EX_Result
//-----------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_Pause(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_PAUSE;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Resume decode and display
/// @param eStream \b IN : stream type
/// @return return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_Resume(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_RESUME;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}


//------------------------------------------------------------------------------
/// Trigger to display one frame (background may still decoding)
/// @param eStream \b IN : stream type
/// @return return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_StepDisp(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_STEP_DISP;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}
/*
VDEC_EX_Result MApi_VDEC_EX_SetFrmBuff2(VDEC_StreamId *pStreamId, VDEC_FRAMEBuff *framebuff)
{

    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_SET_FRAMEBUFF2;
    IO_arg.param[0] = (void*)(&framebuff);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}
*/

//------------------------------------------------------------------------------
/// Check if step play done or not
/// @param eStream \b IN : stream type
/// @return VDEC_EX_Result
///     - E_VDEC_EX_OK: step display done
///     - E_VDEC_EX_FAIL: not yet
///     - E_VDEC_EX_RET_NOT_INIT: not initial yet
///     - E_VDEC_EX_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsStepDispDone(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_IS_STEP_DISP_DONE;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Trigger VDEC to decode one frame
/// @param eStream \b IN : stream type
/// @return return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_StepDecode(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_STEP_DECODE;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Check if step decode done or not
/// @param eStream \b IN : stream type
/// @return VDEC_EX_Result
///     - E_VDEC_EX_OK: step decode done
///     - E_VDEC_EX_FAIL: not yet
///     - E_VDEC_EX_RET_NOT_INIT: not initial yet
///     - E_VDEC_EX_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsStepDecodeDone(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_IS_STEP_DECODE_DONE;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}


//------------------------------------------------------------------------------
/// Set up trick decode mode for decode I, decode IP and decode all.
/// @param eStream \b IN : stream type
/// @param eTrickDec \b IN : trick decode mode
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetTrickMode(VDEC_StreamId *pStreamId, VDEC_EX_TrickDec eTrickDec)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_SET_TRICK_MODE;
    IO_arg.param[0] = (void*)(&eTrickDec);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Push decode command queue
/// @param pStreamId \b IN : the pointer to stream ID
/// @param pCmd \b IN : the pointer to decode command
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_PushDecQ(VDEC_StreamId *pStreamId, VDEC_EX_DecCmd *pCmd)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_PUSH_DECQ;
    IO_arg.param[0] = (void*)(pCmd);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Fire the decode command which be pushed since last fired
/// @param pStreamId \b IN : the pointer to stream ID
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_FireDecCmd(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_FIRE_DEC;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Get current queue vacancy
/// @param eStream \b IN : stream type
/// @return vacancy number
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetDecQVacancy(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_U32 u32num = 0;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return u32num;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_DECQ_VACANCY;
    IO_arg.param[0] = (void*)(&u32num);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return u32num;
}

//------------------------------------------------------------------------------
/// Flush command queue and internal data of decoder
/// @param eStream \b IN : stream type
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_Flush(VDEC_StreamId *pStreamId, VDEC_EX_FreezePicSelect eFreezePic)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_FLUSH;
    IO_arg.param[0] = (void*)(&eFreezePic);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Enable to let decoder force to display all decoded frame when out of input data
/// @param eStream \b IN : stream type
/// @param bEnable \b IN : TRUE - enable; FALSE - disable
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_EnableLastFrameShow(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_ENABLE_LAST_FRAME_SHOW;
    IO_arg.param[0] = (void*)(&bEnable);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Check if all decoded frames are displayed when input stream is run out
/// @param eStream \b IN : stream type
/// @return VDEC_EX_Result
///     - E_VDEC_EX_OK: finished
///     - E_VDEC_EX_FAIL: not yet
///     - E_VDEC_EX_RET_NOT_INIT: not initial yet
///     - E_VDEC_EX_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsDispFinish(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_IS_DISP_FINISH;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Set up display speed
/// @param eStream \b IN : stream type
/// @param eSpeedType \b IN : display speed type
/// @param eSpeed \b IN : multiple
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetSpeed(VDEC_StreamId *pStreamId, VDEC_EX_SpeedType eSpeedType, VDEC_EX_DispSpeed eSpeed)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_SET_SPEED;
    IO_arg.param[0] = (void*)(&eSpeedType);
    IO_arg.param[1] = (void*)(&eSpeed);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Let FW stop updating frames when vsync, but decoding process is still going.
/// @param eStream \b IN : stream type
/// @param bEnable \b IN : Enable/Disable
/// @return -The result of command freeze image.
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetFreezeDisp(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_SET_FREEZE_DISP;
    IO_arg.param[0] = (void*)(&bEnable);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Set blue screen on/off
/// @param eStream \b IN : stream type
/// @param bOn \b IN : turn on / off blue screen
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetBlueScreen(VDEC_StreamId *pStreamId, MS_BOOL bOn)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_SET_BLUE_SCREEN;
    IO_arg.param[0] = (void*)(&bOn);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Reset PTS
/// @param eStream \b IN : stream type
/// @param u32PtsBase \b IN : new PTS base value
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_ResetPTS(VDEC_StreamId *pStreamId, MS_U32 u32PtsBase)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_RESET_PTS;
    IO_arg.param[0] = (void*)(&u32PtsBase);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Switch AV sync on/off
/// @param pStreamId \b IN : the pointer to stream ID
/// @param bOn \b IN : on/off
/// @param u32SyncDelay \b IN : delay value
/// @param u16SyncTolerance \b IN : tolerance value
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_AVSyncOn(VDEC_StreamId *pStreamId, MS_BOOL bOn, MS_U32 u32SyncDelay, MS_U16 u16SyncTolerance)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_AVSYNC_ON;
    IO_arg.param[0] = (void*)(&bOn);
    IO_arg.param[1] = (void*)(&u32SyncDelay);
    IO_arg.param[2] = (void*)(&u16SyncTolerance);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Switch AV sync free run threshold
/// @param eStream \b IN : stream type
/// @param u32Threshold \b IN : threshold value
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetAVSyncFreerunThreshold(VDEC_StreamId *pStreamId, MS_U32 u32Threshold)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_AVSYNC_FREERUN_THRESHOLD;
    IO_arg.param[0] = (void*)(&u32Threshold);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Check if AV sync on or not
/// @param eStream \b IN : stream type
/// @return VDEC_EX_Result
///     - E_VDEC_EX_OK: AV sync on
///     - E_VDEC_EX_FAIL: freerun
///     - E_VDEC_EX_RET_NOT_INIT: not initial yet
///     - E_VDEC_EX_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsAVSyncOn(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_IS_AVSYNC_ON;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Get display information
/// @param eStream \b IN : stream type
/// @param pDispinfo \b OUT : pointer to display setting information
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_GetDispInfo(VDEC_StreamId *pStreamId, VDEC_EX_DispInfo *pDispinfo)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_DISP_INFO;
    IO_arg.param[0] = (void*)(pDispinfo);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Check if decoder got valid stream to decode
/// @param eStream \b IN : stream type
/// @return TRUE or FALSE
///     - TRUE: find valid stream
///     - FALSE: not yet
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsWithValidStream(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_IS_WITH_VALID_STREAM;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Check if decoder found I frame after reset or flush queue
/// @param eStream \b IN : stream type
/// @return VDEC_EX_Result
///     - E_VDEC_EX_OK: found
///     - E_VDEC_EX_FAIL: not found
///     - E_VDEC_EX_RET_NOT_INIT: not initial yet
///     - E_VDEC_EX_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsIFrameFound(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_IS_IFRAME_FOUND;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Check if sequence header is different from previous one
/// @param eStream \b IN : stream type
/// @return VDEC_EX_Result
///     - E_VDEC_EX_OK: changed
///     - E_VDEC_EX_FAIL: not changed
///     - E_VDEC_EX_RET_NOT_INIT: not initial yet
///     - E_VDEC_EX_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsSeqChg(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_IS_SEQ_CHG;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Check if the distance of Video time stamp and STC is closed enough.
/// @param eStream \b IN : stream type
/// @return VDEC_EX_Result
///     - E_VDEC_EX_OK: sync complete
///     - E_VDEC_EX_FAIL: sync is incomplete
///     - E_VDEC_EX_RET_NOT_INIT: not initial yet
///     - E_VDEC_EX_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsReachSync(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_IS_REACH_SYNC;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Check if decoder is doing AV sync now
/// @param eStream \b IN : stream type
/// @return VDEC_EX_Result
///     - E_VDEC_EX_OK: doing AV sync
///     - E_VDEC_EX_FAIL: still freerun
///     - E_VDEC_EX_RET_NOT_INIT: not initial yet
///     - E_VDEC_EX_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsStartSync(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_IS_START_SYNC;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}


//------------------------------------------------------------------------------
/// Check if decoder is avsync freerun
/// @return VDEC_Result
///     - E_VDEC_OK: freerun
///     - E_VDEC_FAIL: not freerun
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsFreerun(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_IS_FREERUN;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Check if input stream is with low delay flag
/// @param eStream \b IN : stream type
/// @return VDEC_EX_Result
///     - E_VDEC_EX_OK: with low delay
///     - E_VDEC_EX_FAIL: without low delay
///     - E_VDEC_EX_RET_NOT_INIT: not initial yet
///     - E_VDEC_EX_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsWithLowDelay(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_IS_WITH_LOW_DELAY;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Check if all date buffers are empty or not
/// @param eStream \b IN : stream type
/// @return VDEC_EX_Result
///     - E_VDEC_EX_OK: All of the buffers are empty
///     - E_VDEC_EX_FAIL: Some of the buffers are not empty
///     - E_VDEC_EX_RET_NOT_INIT: not initial yet
///     - E_VDEC_EX_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsAllBufferEmpty(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_IS_ALL_BUFFER_EMPTY;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Get the extension information of decoded frame.
/// @param pExtDispinfo \b IN : the extension information of decoded frame.
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_GetExtDispInfo(VDEC_StreamId *pStreamId, VDEC_EX_ExtDispInfo *pExtDispinfo)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_EXT_DISP_INFO;
    IO_arg.param[0] = (void*)(pExtDispinfo);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}


//------------------------------------------------------------------------------
/// Get the information of the latest decoded frame.
/// @param eStream \b IN : stream type
/// @param pFrmInfo \b IN : the information of the latest decoded frame
/// @return -VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_GetDecFrameInfo(VDEC_StreamId *pStreamId, VDEC_EX_FrameInfo *pFrmInfo)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_DEC_FRAME_INFO;
    IO_arg.param[0] = (void*)(pFrmInfo);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Get the information of current displaying frame.
/// @param eStream \b IN : stream type
/// @param pFrmInfo \b IN : the information of displaying frame
/// @return -VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_GetDispFrameInfo(VDEC_StreamId *pStreamId, VDEC_EX_FrameInfo *pFrmInfo)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_DISP_FRAME_INFO;
    IO_arg.param[0] = (void*)(pFrmInfo);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Get the time_code of the latest decoded frame.
/// @param eStream \b IN : stream type
/// @param pTimeCode \b IN : the time_code of the latest decoded frame
/// @return -VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_GetDecTimeCode(VDEC_StreamId *pStreamId, VDEC_EX_TimeCode* pTimeCode)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_DEC_TIMECODE;
    IO_arg.param[0] = (void*)(pTimeCode);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Get the time_code of the latest displayed frame.
/// @param eStream \b IN : stream type
/// @param pTimeCode \b IN : the time_code of the latest displayed frame
/// @return -VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_GetDispTimeCode(VDEC_StreamId *pStreamId, VDEC_EX_TimeCode* pTimeCode)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_DISP_TIMECODE;
    IO_arg.param[0] = (void*)(pTimeCode);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Check if the first frame is showed after play function is called
/// @param eStream \b IN : stream type
/// @return VDEC_EX_Result
///     - E_VDEC_EX_OK: ready
///     - E_VDEC_EX_FAIL: not ready
///     - E_VDEC_EX_RET_NOT_INIT: not initial yet
///     - E_VDEC_EX_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsFrameRdy(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_IS_FRAME_RDY;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}


//------------------------------------------------------------------------------
/// Set decoder event and register it's callback function
/// @param eStream \b IN : stream type
/// @param u32EventFlag \b In : the events want to be turned on/off
/// @param pfn \b In : callback function
/// @param param \b In : parameter for callback function
/// @return -VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetEvent(VDEC_StreamId *pStreamId, MS_U32 u32EventFlag, VDEC_EX_EventCb pfn, void *param)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    VDEC_EX_CB_TYPE type = E_VDEC_EX_CB_MAIN;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_SET_EVENT_MULTICALLBACK;
    IO_arg.param[0] = (void*)(&type);
    IO_arg.param[1] = (void*)(&u32EventFlag);
    IO_arg.param[2] = (void*)(pfn);
    IO_arg.param[3] = (param);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

#if 1//#ifdef VDEC_UTOPIA_2K
     _VDEC_EX_Create_Callback_task(pStreamId,  type, pfn, param,u32EventFlag);
#endif

    return ret;
}

VDEC_EX_Result MApi_VDEC_EX_UnsetEvent(VDEC_StreamId *pStreamId, MS_U32 u32EventFlag)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    VDEC_EX_CB_TYPE type = E_VDEC_EX_CB_MAIN;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_UNSET_EVENT_MULTICALLBACK;
    IO_arg.param[0] = (void*)(&type);
    IO_arg.param[1] = (void*)(&u32EventFlag);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

#if 1//#ifdef VDEC_UTOPIA_2K
    _VDEC_EX_UnSetEvent(pStreamId,  type, u32EventFlag);
#endif

    return ret;
}

//------------------------------------------------------------------------------
/// Set decoder event and register it's callback function
/// @param eStream \b IN : stream type
/// @param u32EventFlag \b In : the events want to be turned on/off
/// @param pfn \b In : callback function
/// @param param \b In : parameter for callback function
/// @return -VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetEvent_MultiCallback(VDEC_StreamId *pStreamId, VDEC_EX_CB_TYPE cb_type, MS_U32 u32EventFlag, VDEC_EX_EventCb pfn, void *param)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_SET_EVENT_MULTICALLBACK;
    IO_arg.param[0] = (void*)(&cb_type);
    IO_arg.param[1] = (void*)(&u32EventFlag);
    IO_arg.param[2] = (void*)(pfn);
    IO_arg.param[3] = (param);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

#if 1//#ifdef VDEC_UTOPIA_2K
     _VDEC_EX_Create_Callback_task(pStreamId,  cb_type, pfn, param,u32EventFlag);
#endif

    return ret;
}

VDEC_EX_Result MApi_VDEC_EX_UnsetEvent_MultiCallback(VDEC_StreamId *pStreamId, VDEC_EX_CB_TYPE cb_type, MS_U32 u32EventFlag)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_UNSET_EVENT_MULTICALLBACK;
    IO_arg.param[0] = (void*)(&cb_type);
    IO_arg.param[1] = (void*)(&u32EventFlag);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

#if 1//#ifdef VDEC_UTOPIA_2K
    _VDEC_EX_UnSetEvent(pStreamId,  cb_type, u32EventFlag);
#endif

    return ret;
}

//------------------------------------------------------------------------------
/// Get ISR event status
/// @param eStream \b IN : stream type
/// @param u32EventFlag \b OUT : vdec event bitmap
/// @return status
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_GetEventInfo(VDEC_StreamId *pStreamId, MS_U32* u32EventFlag)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_EVENT_INFO;
    IO_arg.param[0] = (void*)(u32EventFlag);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Decoder will start decode with full  speed until target PTS is reached (equal or larger) then pause.
/// @param eStream \b IN : stream type
/// @param u32PTS \b IN : target PTS
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SeekToPTS(VDEC_StreamId *pStreamId, MS_U32 u32PTS)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_SEEK_TO_PTS;
    IO_arg.param[0] = (void*)(&u32PTS);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Decoder will skip all frames before target PTS, and start decode with full  speed after target PTS is reached.
/// @param eStream \b IN : stream type
/// @param u32PTS \b IN : target PTS
///         - 0: turn off this mode.
///         - any not zero: enable this mode.
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SkipToPTS(VDEC_StreamId *pStreamId, MS_U32 u32PTS)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_SKIP_TO_PTS;
    IO_arg.param[0] = (void*)(&u32PTS);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Set deblocking process on/off
/// @param eStream \b IN : stream type
/// @param bOn \b IN : turn on / off deblocking process
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_DisableDeblocking(VDEC_StreamId *pStreamId, MS_BOOL bDisable)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_DISABLE_DEBLOCKING;
    IO_arg.param[0] = (void*)(&bDisable);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Set Quarter Pixel process on/off
/// @param eStream \b IN : stream type
/// @param bOn \b IN : turn on / off Quarter Pixel process
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_DisableQuarterPixel(VDEC_StreamId *pStreamId, MS_BOOL bDisable)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_DISABLE_QUARTER_PIXEL;
    IO_arg.param[0] = (void*)(&bDisable);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Turn on/off Auto Remove Last Zero Byte process
/// @param eStream \b IN : stream type
/// @param bOn \b IN : turn on / off Auto Remove Last Zero Byte process
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetAutoRmLstZeroByte(VDEC_StreamId *pStreamId, MS_BOOL bOn)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_SET_AUTO_RM_LST_ZERO_BYTE;
    IO_arg.param[0] = (void*)(&bOn);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Set the parameters of Balancing BW
/// @param eStream \b IN : stream type
/// @param u8QuarPixelTH \b IN : set Quarter pixel threshold
/// @param u8DeBlockingTH \b IN : set deblocking threshold
/// @param u8UpperBound \b IN : set upper boundary of BW control counter
/// @param u8SafeCoef \b IN : set safety coeffecient of average MB tick.
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetBalanceBW(VDEC_StreamId *pStreamId, MS_U8 u8QuarPixelTH, MS_U8 u8DeBlockingTH, MS_U8 u8UpperBound, MS_U8 u8SafeCoef)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_SET_BALANCE_BW;
    IO_arg.param[0] = (void*)(&u8QuarPixelTH);
    IO_arg.param[1] = (void*)(&u8DeBlockingTH);
    IO_arg.param[2] = (void*)(&u8UpperBound);
    IO_arg.param[3] = (void*)(&u8SafeCoef);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Get stream active format which decoder retrieved
/// @param eStream \b IN : stream type
/// @return active format ID
//------------------------------------------------------------------------------
MS_U8 MApi_VDEC_EX_GetActiveFormat(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_U8 u8ActFmt = 0;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return u8ActFmt;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_ACTIVE_FORMAT;
    IO_arg.param[0] = (void*)(&u8ActFmt);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return u8ActFmt;
}


//------------------------------------------------------------------------------
/// Get stream colour primaries
/// @param eStream \b IN : stream type
/// @return stream colour primaries
//------------------------------------------------------------------------------
MS_U8 MApi_VDEC_EX_GetColourPrimaries(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_U8 u8ColourPrimaries = 0;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return u8ColourPrimaries;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_COLOUR_PRIMARIES;
    IO_arg.param[0] = (void*)(&u8ColourPrimaries);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return u8ColourPrimaries;
}

MS_U32 MApi_VDEC_EX_GetFwVersion(VDEC_StreamId *pStreamId, VDEC_EX_FwType eFwType)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_U32 u32FWVer = 0;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return u32FWVer;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_FW_VERSION;
    IO_arg.param[0] = (void*)(&eFwType);
    IO_arg.param[1] = (void*)(&u32FWVer);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return u32FWVer;
}

//------------------------------------------------------------------------------
/// Get picture counter of current GOP
/// @param eStream \b IN : stream type
/// @return picture counter
//------------------------------------------------------------------------------
MS_U8 MApi_VDEC_EX_GetGOPCnt(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_U8 u8Cnt = 0;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return u8Cnt;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_GOP_CNT;
    IO_arg.param[0] = (void*)(&u8Cnt);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return u8Cnt;
}

//------------------------------------------------------------------------------
/// Get ES buffer write pointer
/// @param eStream \b IN : stream type
/// @return write pointer
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetESWritePtr(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_U32 u32Ptr = 0;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return u32Ptr;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_ES_WRITE_PTR;
    IO_arg.param[0] = (void*)(&u32Ptr);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return u32Ptr;
}

//------------------------------------------------------------------------------
/// Get decoder ES buffer read pointer
/// @param eStream \b IN : stream type
/// @return read pointer
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetESReadPtr(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_U32 u32Ptr = 0;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return u32Ptr;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_ES_READ_PTR;
    IO_arg.param[0] = (void*)(&u32Ptr);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return u32Ptr;
}


//------------------------------------------------------------------------------
/// Get the PTS of current displayed frame
/// @param eStream \b IN : stream type
/// @return PTS
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetPTS(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_U32 u32Pts = 0;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return u32Pts;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_PTS;
    IO_arg.param[0] = (void*)(&u32Pts);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return u32Pts;
}

//------------------------------------------------------------------------------
/// Get the PTS of next displayed frame
/// @return next PTS (unit:ms)
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetNextPTS(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_U32 u32Pts = 0;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return u32Pts;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_NEXT_PTS;
    IO_arg.param[0] = (void*)(&u32Pts);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return u32Pts;
}

//------------------------------------------------------------------------------
/// Get the value of PTS - STC for video
/// @return the value of PTS - STC for video
//------------------------------------------------------------------------------
MS_S64 MApi_VDEC_EX_GetVideoPtsStcDelta(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_S64 s64Pts = 0;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return s64Pts;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_VIDEO_PTS_STC_DELTA;
    IO_arg.param[0] = (void*)(&s64Pts);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return s64Pts;
}

//------------------------------------------------------------------------------
/// Get error code
/// @param eStream \b IN : stream type
/// @return error code
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetErrCode(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    VDEC_EX_ErrCode eErrCode = E_VDEC_EX_ERR_CODE_ILLEGAL_ACCESS;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return eErrCode;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_ERR_CODE;
    IO_arg.param[0] = (void*)(&eErrCode);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return eErrCode;
}

//------------------------------------------------------------------------------
/// Get accumulated error counter
/// @param eStream \b IN : stream type
/// @return error counter
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetErrCnt(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_U32 u32Cnt = 0;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return u32Cnt;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_ERR_CNT;
    IO_arg.param[0] = (void*)(&u32Cnt);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return u32Cnt;
}

//------------------------------------------------------------------------------
/// Get bitrate which decoder retrieved from stream
/// @param eStream \b IN : stream type
/// @return bitrate
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetBitsRate(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_U32 u32Bitrate = 0;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return u32Bitrate;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_BITRATE;
    IO_arg.param[0] = (void*)(&u32Bitrate);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return u32Bitrate;
}

//-----------------------------------------------------------------------------
/// Get accumulated decoded frame Count
/// @param eStream \b IN : stream type
/// @return - decoded frame Count
//-----------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetFrameCnt(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_U32 u32Cnt = 0;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return u32Cnt;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_FRAME_CNT;
    IO_arg.param[0] = (void*)(&u32Cnt);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return u32Cnt;
}

//------------------------------------------------------------------------------
/// Get skipped counter
/// @param eStream \b IN : stream type
/// @return counter
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetSkipCnt(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_U32 u32Cnt = 0;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return u32Cnt;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_SKIP_CNT;
    IO_arg.param[0] = (void*)(&u32Cnt);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return u32Cnt;
}

//------------------------------------------------------------------------------
/// Get dropped frame counter
/// @param eStream \b IN : stream type
/// @return counter
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetDropCnt(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_U32 u32Cnt = 0;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return u32Cnt;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_DROP_CNT;
    IO_arg.param[0] = (void*)(&u32Cnt);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return u32Cnt;
}

//------------------------------------------------------------------------------
/// Get displayed frame counter
/// @param pStreamId \b IN : pointer to stream ID
/// @return counter
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetDispCnt(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_U32 u32Cnt = 0;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return u32Cnt;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_DISP_CNT;
    IO_arg.param[0] = (void*)(&u32Cnt);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return u32Cnt;
}

//------------------------------------------------------------------------------
/// Get 3:2 pull down flag which decoder retrieved from stream
/// @param eStream \b IN : stream type
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
MS_BOOL MApi_VDEC_EX_Is32PullDown(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_BOOL bIs32PullDown = FALSE;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return bIs32PullDown;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_IS_32_PULLDOWN;
    IO_arg.param[0] = (void*)(&bIs32PullDown);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return bIs32PullDown;
}

//------------------------------------------------------------------------------
/// Check whether the status of decoder is running or not.
/// @param eStream \b IN : stream type
/// @return VDEC_EX_Result: E_VDEC_EX_OK/E_VDEC_EX_RET_NOT_RUNNING/E_VDEC_EX_RET_NOT_INIT
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsAlive(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_IS_ALIVE;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

MS_BOOL MApi_VDEC_EX_IsCCAvailable(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_BOOL isavailable = FALSE;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return isavailable;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_IS_CC_AVAILABLE;
    IO_arg.param[0] = (void*)(&isavailable);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return isavailable;
}

VDEC_EX_Result MApi_VDEC_EX_GetCCInfo(VDEC_StreamId *pStreamId, void *pInfo, MS_U32 u32Size)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_CC_INFO;
    IO_arg.param[0] = (pInfo);
    IO_arg.param[1] = (void*)(&u32Size);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

#if 1//#ifdef VDEC_UTOPIA_2K
    VDEC_EX_CC_Info *ptr = pInfo;
    ptr->u32UserDataBuf = MsOS_PA2KSEG1(ptr->u32UserDataBuf);
#endif

    return ret;
}

//------------------------------------------------------------------------------
/// Get current trick decode mode of decoder
/// @param eStream \b IN : stream type
/// @return VDEC_EX_TrickDec
//------------------------------------------------------------------------------
VDEC_EX_TrickDec MApi_VDEC_EX_GetTrickMode(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    VDEC_EX_TrickDec eTrick = E_VDEC_EX_TRICK_DEC_ALL;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return eTrick;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_TRICK_MODE;
    IO_arg.param[0] = (void*)(&eTrick);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return eTrick;
}

//------------------------------------------------------------------------------
/// Get codec type which be initialed
/// @param eStream \b IN : stream type
/// @return VDEC_EX_CodecType
//------------------------------------------------------------------------------
VDEC_EX_CodecType MApi_VDEC_EX_GetActiveCodecType(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    VDEC_EX_CodecType CodecType = E_VDEC_EX_CODEC_TYPE_NONE;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return CodecType;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_ACTIVE_CODEC_TYPE;
    IO_arg.param[0] = (void*)(&CodecType);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return CodecType;
}


//------------------------------------------------------------------------------
/// Generate a special pattern to work with specific functions
/// @param eStream \b IN : stream type
/// @param ePatternType \b IN : Pattern type
/// @param u32Addr \b IN : Pattern buffer physical start address
/// @param u32Size \b IN,OUT : Pattern buffer size
///                             IN: Pattern buffer size
///                             OUT: Used Pattern buffer size
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_GenPattern(VDEC_StreamId *pStreamId, VDEC_EX_PatternType ePatternType, MS_PHY u32Addr, MS_U32* u32Size)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GEN_PATTERN;
    IO_arg.param[0] = (void*)(&ePatternType);
    IO_arg.param[1] = (void*)(&u32Addr);
    IO_arg.param[2] = (void*)(u32Size);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret =  E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Get the information of the least length of pattern used for specific function
/// @param eStream \b IN : stream type
/// @param pFrmInfo \b IN : the information of the least length of pattern used for specific function
/// @return -the length (Bytes)
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetPatternLeastLength(VDEC_StreamId *pStreamId, VDEC_EX_PatternType ePatternType)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_U32 length = 0;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return length;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_PATTERN_LEAST_LENGTH;
    IO_arg.param[0] = (void*)(&ePatternType);
    IO_arg.param[1] = (void*)(&length);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return length;
}


//------------------------------------------------------------------------------
/// For MHEG5 MW, decode one I frame
/// @param eStream \b IN : stream type
/// @param u32FrameBufAddr \b IN : output buffer address
/// @param u32SrcSt \b IN : source start address
/// @param u32SrcEnd \b IN : source end address
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_MHEG_DecodeIFrame(VDEC_StreamId *pStreamId,
                                            MS_PHY u32FrameBufAddr,
                                            MS_PHY u32SrcSt,
                                            MS_PHY u32SrcEnd)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_MHEG_DECODE_IFRAME;
    IO_arg.param[0] = (void*)(&u32FrameBufAddr);
    IO_arg.param[1] = (void*)(&u32SrcSt);
    IO_arg.param[2] = (void*)(&u32SrcEnd);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// For MHEG5 MW, check if I frame decode done
/// @param eStream \b IN : stream type
/// @return VDEC_EX_Result
///     - E_VDEC_EX_OK: decode done
///     - E_VDEC_EX_FAIL: not yet
///     - E_VDEC_EX_RET_NOT_INIT: not initial yet
///     - E_VDEC_EX_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_MHEG_IsIFrameDecoding(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_MHEG_IS_IFRAME_DECODING;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// For MHEG5 MW, reset decoder after I frame decode done
/// @param eStream \b IN : stream type
/// @return VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_MHEG_RstIFrameDec(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_MHEG_RST_IFRAME_DEC;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}


//------------------------------------------------------------------------------
/// [Obsolete] Start CC data parsing.
/// @param eStream \b IN : stream type
/// @param pCCParam \b IN : CC configuration
/// @return -VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_CC_StartParsing(VDEC_StreamId *pStreamId, VDEC_EX_CCCfg *pCCParam)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_CC_START_PARSING;
    IO_arg.param[0] = (void*)(pCCParam);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// [Obsolete] Stop CC data parsing.
/// @param eStream \b IN : stream type
/// @return -VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_CC_StopParsing(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_CC_STOP_PARSING;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// [Obsolete] Get write pointer of CC data buffer.
/// @param eStream \b IN : stream type
/// @param pu32Write \b OUT : current write pointer
/// @return -VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_CC_GetWritePtr(VDEC_StreamId *pStreamId, MS_U32 *pu32Write)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_CC_GET_WRITE_PTR;
    IO_arg.param[0] = (void*)(pu32Write);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// [Obsolete] Get the read pointer of CC data buffer.
/// @param eStream \b IN : stream type
/// @param pu32Read \b OUT : current read pointer
/// @return -VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_CC_GetReadPtr(VDEC_StreamId *pStreamId, MS_U32 *pu32Read)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_CC_GET_READ_PTR;
    IO_arg.param[0] = (void*)(pu32Read);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// [Obsolete] Update the read pointer of CC data buffer.
/// @param eStream \b IN : stream type
/// @param u32EachPacketSize \b IN : new read pointer
/// @return -VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_CC_UpdateReadPtr(VDEC_StreamId *pStreamId, MS_U32 u32EachPacketSize)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_CC_UPDATE_READ_PTR;
    IO_arg.param[0] = (void*)(&u32EachPacketSize);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// [Obsolete] Get if CC data buffer is overflow.
/// @param eStream \b IN : stream type
/// @param pbOverflow \b OUT : overflow flag
/// @return -VDEC_EX_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_CC_GetIsOverflow(VDEC_StreamId *pStreamId, MS_BOOL *pbOverflow)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_CC_GET_IS_OVERFLOW;
    IO_arg.param[0] = (void*)(pbOverflow);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}


//------------------------------------------------------------------------------
/// @brief Get the HW Key currently not support
/// @param eStream \b IN : stream type
/// @param [out] pu8Key 8bit HW key
/// @retval E_VDEC_EX_OK Success
/// @retval E_VDEC_EX_FAIL Failure
/// @retval E_VDEC_EX_RET_NOT_INIT The decoder is not initialized
/// @retval E_VDEC_EX_RET_UNSUPPORTED Not supported for current decoder configuration or codec type
/// @retval E_VDEC_EX_RET_ILLEGAL_ACCESS Illegal access
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_GetHWKey(VDEC_StreamId *pStreamId, MS_U8 *pu8Key)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_HW_KEY;
    IO_arg.param[0] = (void*)(pu8Key);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}


VDEC_EX_Result MApi_VDEC_EX_SetBlockDisplay(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_SET_BLOCK_DISPLAY;
    IO_arg.param[0] = (void*)(&bEnable);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

VDEC_EX_Result MApi_VDEC_EX_EnableESBuffMalloc(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_ENABLE_ES_BUFF_MALLOC;
    IO_arg.param[0] = (void*)(&bEnable);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

MS_U32 MApi_VDEC_EX_GetESBuffVacancy(VDEC_StreamId *pStreamId, void *pData)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_U32 u32ESBuffVacancy = 0;

    UNUSED(pData);

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return u32ESBuffVacancy;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_ES_BUFF_VACANCY;
    IO_arg.param[0] = (void*)(&u32ESBuffVacancy);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return u32ESBuffVacancy;
}

VDEC_EX_Result MApi_VDEC_EX_GetESBuff(VDEC_StreamId *pStreamId, MS_U32 u32ReqSize, MS_U32 *u32AvailSize, MS_PHY *u32Addr)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_ES_BUFF;
    IO_arg.param[0] = (void*)(&u32ReqSize);
    IO_arg.param[1] = (void*)(u32AvailSize);
    IO_arg.param[2] = (void*)(u32Addr);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

VDEC_EX_Result MApi_VDEC_EX_GetNextDispFrame(VDEC_StreamId *pStreamId, VDEC_EX_DispFrame **ppDispFrm)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);

    *ppDispFrm = &(DispFrame_temp[u8Idx]);

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_GET_NEXT_DISP_FRAME;
    IO_arg.param[0] = (void*)(&(DispFrame_temp[u8Idx]));
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Notify FW to display the specified frame
/// @param pDispFrm \b IN : the pointer of the frame to be displayed
/// @return VDEC_EX_Result
/// @retval -E_VDEC_EX_RET_NOT_INIT: VDEC is not initialized.
/// @retval -E_VDEC_EX_RET_INVALID_PARAM: the pointer is NULL.
/// @retval -E_VDEC_EX_RET_ILLEGAL_ACCESS: the pointer is invalid.
///          There might be multiple frame display/release.
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_DisplayFrame(VDEC_StreamId *pStreamId, VDEC_EX_DispFrame *pDispFrm)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_DISPLAY_FRAME;
    IO_arg.param[0] = (void*)(pDispFrm);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

VDEC_EX_Result MApi_VDEC_EX_ReleaseFrame(VDEC_StreamId *pStreamId, VDEC_EX_DispFrame *pDispFrm)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_RELEASE_FRAME;
    IO_arg.param[0] = (void*)(pDispFrm);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

VDEC_EX_Result MApi_VDEC_EX_CaptureFrame(VDEC_StreamId *pStreamId, MS_U32 u32FrmPriData, MS_BOOL bEnable)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_CAPTURE_FRAME;
    IO_arg.param[0] = (void*)(&u32FrmPriData);
    IO_arg.param[1] = (void*)(&bEnable);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

VDEC_EX_Result MApi_VDEC_EX_CC_Init(VDEC_StreamId *pStreamId, MS_U32 *pIn)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_CC_INIT;
    IO_arg.param[0] = (void*)(pIn);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

VDEC_EX_Result MApi_VDEC_EX_CC_SetCfg(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt, VDEC_EX_CCType eType, MS_U32 *pIn)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_CC_SET_CFG;
    IO_arg.param[0] = (void*)(&eFmt);
    IO_arg.param[1] = (void*)(&eType);
    IO_arg.param[2] = (void*)(pIn);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

VDEC_EX_Result MApi_VDEC_EX_CC_SetBuffStartAdd(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt, MS_U32 *pIn)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_CC_SET_BUFF_START_ADDR;
    IO_arg.param[0] = (void*)(&eFmt);
    IO_arg.param[1] = (void*)(pIn);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

VDEC_EX_Result MApi_VDEC_EX_CC_UpdateWriteAdd(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt, MS_U32 *pIn)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_CC_UPDATE_WRITE_ADDR;
    IO_arg.param[0] = (void*)(&eFmt);
    IO_arg.param[1] = (void*)(pIn);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

VDEC_EX_Result MApi_VDEC_EX_CC_UpdateReadAdd(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt, MS_U32 *pIn)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_CC_UPDATE_READ_ADDR;
    IO_arg.param[0] = (void*)(&eFmt);
    IO_arg.param[1] = (void*)(pIn);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

VDEC_EX_Result MApi_VDEC_EX_CC_DisableParsing(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_CC_DISABLE_PARSING;
    IO_arg.param[0] = (void*)(&eFmt);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

VDEC_EX_Result MApi_VDEC_EX_CC_GetInfo(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt, VDEC_EX_CCInfoCmd eCmd, MS_U32 *pOut)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_CC_GET_INFO;
    IO_arg.param[0] = (void*)(&eFmt);
    IO_arg.param[1] = (void*)(&eCmd);
    IO_arg.param[2] = (void*)(pOut);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

VDEC_EX_Result MApi_VDEC_EX_CC_GetIsRstDone(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_CC_GET_IS_RST_DONE;
    IO_arg.param[0] = (void*)(&eFmt);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

VDEC_EX_Result MApi_VDEC_EX_CC_GetIsBuffOverflow(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_CC_GET_IS_BUFF_OVERFLOW;
    IO_arg.param[0] = (void*)(&eFmt);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Report the physical address of write pointer
/// @param eFmt \b IN : the format of closed caption
/// @return MS_PHY
//------------------------------------------------------------------------------
MS_PHY MApi_VDEC_EX_CC_GetWriteAdd(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_PHY u32CCWrPtr = VDEC_U32_MAX;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return u32CCWrPtr;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_CC_GET_WRITE_ADDR;
    IO_arg.param[0] = (void*)(&eFmt);
    IO_arg.param[1] = (void*)(&u32CCWrPtr);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return u32CCWrPtr;
}

//------------------------------------------------------------------------------
/// Report the physical address of read pointer
/// @param eFmt \b IN : the format of closed caption
/// @return MS_PHY
//------------------------------------------------------------------------------
MS_PHY MApi_VDEC_EX_CC_GetReadAdd(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;
    MS_PHY u32CCRdPtr = VDEC_U32_MAX;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return u32CCRdPtr;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_CC_GET_READ_ADDR;
    IO_arg.param[0] = (void*)(&eFmt);
    IO_arg.param[1] = (void*)(&u32CCRdPtr);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
    }

    return u32CCRdPtr;
}

VDEC_EX_Result MApi_VDEC_EX_Rst(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_RST;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}


VDEC_EX_Result MApi_VDEC_EX_Exit(VDEC_StreamId *pStreamId)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        if(UtopiaOpen(MODULE_VDEC_EX | VDEC_DRIVER_BASE, &pu32VDEC_EX_Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            VPRINTF("UtopiaOpen VDEC_EX failed\n");
            return E_VDEC_EX_FAIL;
        }
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_EXIT;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,(MS_U32)E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

#if 0
    if(UtopiaClose(pu32VDEC_EX_Inst) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    pu32VDEC_EX_Inst = NULL;
#endif
    return ret;
}

VDEC_EX_Result MApi_VDEC_GetCaps(VDEC_HwCap *pVDECHWCap)
{
    MS_U16 u16CopiedLength = 0;
    VDEC_HwCap stVDECHWCap;

    _VDEC_Memset(&stVDECHWCap,0,sizeof(VDEC_HwCap));

    if(pVDECHWCap == NULL)
    {
        VPRINTF("Null paramter in %s\n",__FUNCTION__);
        return E_VDEC_EX_FAIL;
    }

    if(pVDECHWCap->u16ApiHW_Length == 0)
    {
        VPRINTF("Zero u16ApiHW_Length in %s\n",__FUNCTION__);
        return E_VDEC_EX_FAIL;
    }

    if(pVDECHWCap->u32ApiHW_Version < 1)
    {
        VPRINTF("Wrong u32ApiHW_Version(%d) in %s\n",(unsigned int)pVDECHWCap->u32ApiHW_Version,__FUNCTION__);
        return E_VDEC_EX_FAIL;
    }

    //new AP + old lib, only the length corresponding to old lib has meaning.
    if(pVDECHWCap->u32ApiHW_Version > API_HWCAP_VERSION)
    {
        VPRINTF("[VDEC] old lib version has only length:%lx\n",(unsigned long)sizeof(VDEC_HwCap));
        u16CopiedLength = sizeof(VDEC_HwCap);
    }

    //old AP + new lib, driver shouldn't access to the space which doesn't exist in old structure
    if((pVDECHWCap->u32ApiHW_Version < API_HWCAP_VERSION) || (pVDECHWCap->u16ApiHW_Length < sizeof(VDEC_HwCap)))
    {
        VPRINTF("new lib version access by old structure!!\n");
        u16CopiedLength = pVDECHWCap->u16ApiHW_Length;
    }

    stVDECHWCap.u32ApiHW_Version = API_HWCAP_VERSION;
    stVDECHWCap.u16ApiHW_Length = u16CopiedLength;

    if(MDrv_HVD_EX_CHIP_Capability(&stVDECHWCap) == TRUE)
    {
        memcpy(pVDECHWCap, &stVDECHWCap, u16CopiedLength);
        return E_VDEC_EX_OK;
    }
    else
    {
        VPRINTF("[VDEC] chip cap is not ready\n");
        return E_VDEC_EX_FAIL;
    }
}

//------------------------------------------------------------------------------
/// Get Stream ID from VDEC.
/// @param pInfo \b IN : pointer to VDEC_StreamId
/// @param u32Size \b IN : size of VDEC_StreamId
/// @param eStream \b IN : stream type
/// @param eCodecType \b IN : codec type
/// @return VDEC_EX_Result
///     - E_VDEC_EX_OK: success
///     - E_VDEC_EX_FAIL: failed
///     - E_VDEC_EX_RET_NOT_EXIT: not ext after last initialization
///     - E_VDEC_EX_RET_INVALID_PARAM: input parameter is invalid
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_GetFreeStream(void *pInfo, MS_U32 u32Size, VDEC_EX_Stream eStreamType, VDEC_EX_CodecType eCodecType)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        if(UtopiaOpen(MODULE_VDEC_EX | VDEC_DRIVER_BASE, &pu32VDEC_EX_Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            VPRINTF("UtopiaOpen VDEC_EX failed\n");
            return E_VDEC_EX_FAIL;
        }
    }

#ifndef MSOS_TYPE_LINUX_KERNEL
    if(_MApi_VDEC_EX_IsSecureModeEnable())// move mailbox register/enable to user mode
    {
        _MApi_VDEC_EX_RegisterMBX();
    }
#endif

    IO_arg.param[0] = pInfo;
    IO_arg.param[1] = (void*)(&u32Size);
    IO_arg.param[2] = (void*)(&eStreamType);
    IO_arg.param[3] = (void*)(&eCodecType);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst, E_VDEC_EX_V2_CMD_GET_FREE_STREAM_ID, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail]%s,%d\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    return ret;
}


VDEC_EX_Result MApi_VDEC_EX_V2_GetFreeStream(VDEC_StreamId *pInfo, MS_U32 u32Size, VDEC_EX_Stream eStreamType, VDEC_EX_CodecType eCodecType)
{
    MS_U32 u32Version;
    MS_U8 i;

    if(_VDEC_Init_Share_Mem(eStreamType) != E_VDEC_EX_OK)
    {
        VDEC_PRINT("Init Share Mem fail\n");
        return E_VDEC_EX_FAIL;
    }

#if (defined(MSOS_TYPE_LINUX)||defined(MSOS_TYPE_LINUX_KERNEL)) && !defined(SUPPORT_X_MODEL_FEATURE)
    //REE
    if(MDrv_MVD_EnableVPUSecurityMode(bSecureModeEnable) != TRUE)
    {
        return E_VDEC_EX_FAIL;
    }

    if (MDrv_HVD_EX_SetVPUSecureMode(bSecureModeEnable) != TRUE)
    {
        return E_VDEC_EX_FAIL;
    }

    if(bSecureModeEnable == TRUE)
    {
        SYS_TEEINFO teemode;
        MDrv_SYS_ReadKernelCmdLine();
        MDrv_SYS_GetTEEInfo(&teemode);
        VPRINTF("[VDEC][TEE/OPTEE]%s,TEE_type=%d\n",__FUNCTION__,teemode.OsType);
        if(teemode.OsType == SYS_TEEINFO_OSTYPE_NUTTX)
        {
            if (MDrv_MVD_REE_RegisterMBX() == FALSE)
            {
                return E_VDEC_EX_FAIL;
            }

            if(MDrv_MVD_REE_SetSHMBaseAddr((MS_U32)SYS_TEEINFO_OSTYPE_NUTTX,0,0,0) == FALSE)
            {
                return E_VDEC_EX_FAIL;
            }
        }
        else if(teemode.OsType == SYS_TEEINFO_OSTYPE_OPTEE)
        {
            MS_PHY SHM_Param[3] = {0};
            VPRINTF("[VDEC][OPTEE]Send get sharememory command\n");
            if(_MApi_VDEC_EX_V2_Send_OPTEE_CMD(E_VDEC_EX_OPTEE_GETSHMADDR,(void*)SHM_Param,sizeof(MS_PHY)*3) == TRUE)
            {
                VPRINTF("[VDEC][OPTEE]Get sharememory success\n");
                if(MDrv_MVD_REE_SetSHMBaseAddr((MS_U32)SYS_TEEINFO_OSTYPE_OPTEE,SHM_Param[0],SHM_Param[1],SHM_Param[2]) == FALSE)
                {
                    VPRINTF("[VDEC][OPTEE]Set sharememory fail\n");
                    return E_VDEC_EX_FAIL;
                }
            }
            else
            {
                VPRINTF("[VDEC][OPTEE]Get sharememory fail\n");
                return E_VDEC_EX_FAIL;
            }
        }
    }
#endif

    if(pInfo == NULL)
    {
        return E_VDEC_EX_FAIL;
    }

    u32Version = pInfo->u32Version;

    _VDEC_EX_API_MutexLock();

#ifdef VDEC3
    if (eStreamType == E_VDEC_EX_N_STREAM)
    {
        //pVdecExContext->bFWdecideFB = TRUE;
        pVdecExContext->bDRVdecideBS = TRUE;
    }
    else if (pVdecExContext->bDRVdecideBS && eStreamType != E_VDEC_EX_N_STREAM)
    {
        VDEC_PRINT("Stream type isn't consistant with the previous call\n");
        _VDEC_EX_API_MutexUnlock();
        return E_VDEC_EX_FAIL;
    }
#endif

    if (u32Version == 0 && u32Size == sizeof(VDEC_StreamId))
    {
        // Determine streamID from vdec layer
        VDEC_StreamId *pStreamId = pInfo;
        VDEC_EX_Decoder eDecoder = _VDEC_EX_GetDecoderByCodecType(eCodecType);
        MS_U8 u8Idx = (MS_U8) eStreamType; //u8Idx = 0(main), 1(sub).
#ifdef VDEC3
        if (eStreamType == E_VDEC_EX_N_STREAM)
        {
#if 1
            u8Idx = MDrv_MVD_CheckFreeStream();

            if (u8Idx == -1)
            {
                VDEC_PRINT("No available stream \n");
                _VDEC_EX_API_MutexUnlock();
                return E_VDEC_EX_FAIL;
            }
#else
            for (u8Idx = 0; u8Idx < VDEC_MAX_SUPPORT_STREAM_NUM; u8Idx++)
            {
                if (!pVdecExContext->_Attr[u8Idx].bUsed)
                    break;
            }

            if (u8Idx >= VDEC_MAX_SUPPORT_STREAM_NUM)
            {
                VDEC_PRINT("all streams are in used!\n");
                return E_VDEC_EX_FAIL;
            }
#endif
        }
#endif

        if (TRUE == pVdecExContext->_Attr[u8Idx].bUsed)
        {
            VDEC_PRINT("eStreamType(%x) is used!\n", eStreamType);
            _VDEC_EX_API_MutexUnlock();
            return E_VDEC_EX_FAIL;
        }

#ifdef VDEC3
        _VDEC_EX_Context_Init_Function_Pointer(u8Idx);
#endif

        // Get streamID from driver/hal layer
        switch (eDecoder)
        {
            case E_VDEC_EX_DECODER_MVD:
            {
                MVD_DRV_StreamType eMvdStreamType = E_MVD_DRV_STREAM_NONE;
#ifndef VDEC3
                if (eStreamType == E_VDEC_EX_MAIN_STREAM)
                {
                    eMvdStreamType = E_MVD_DRV_MAIN_STREAM;
                }
                else if (eStreamType == E_VDEC_EX_SUB_STREAM)
                {
                    eMvdStreamType = E_MVD_DRV_SUB_STREAM;
                }
#else
                eMvdStreamType = E_MVD_DRV_N_STREAM+u8Idx;
#endif
                //VDEC_PRINT("eMvdStreamType = 0x%x\n", eMvdStreamType);
                if (eMvdStreamType != E_MVD_DRV_STREAM_NONE)
                {
                    MS_U32 u32VdecStreamId = 0;

#ifdef VDEC3
                    if(MDrv_MVD_GetFreeStream(&u32VdecStreamId, eMvdStreamType, pVdecExContext->bDRVdecideBS) != E_MVD_RET_OK)
#else
                    if(MDrv_MVD_GetFreeStream(&u32VdecStreamId, eMvdStreamType) != E_MVD_RET_OK)
#endif
                    {
                        _VDEC_EX_API_MutexUnlock();
                        return E_VDEC_EX_FAIL;
                    }
                    VDEC_PRINT("=========> vdec_u8Idx=0x%x, u32VdecStreamId=0%x\n", u8Idx, u32VdecStreamId);

                    if (u32VdecStreamId)
                    {
                        pStreamId->u32Id = (u8Idx << 24 | u32VdecStreamId);
                        _SET_DISPLAYMODE_INVALID(u8Idx);
                         _VDEC_EX_API_MutexUnlock();
                        return E_VDEC_EX_OK;
                    }
                }

                break;
            }
            case E_VDEC_EX_DECODER_HVD:
            {
                MS_BOOL bIsEVD = FALSE;
                if ((eCodecType == E_VDEC_EX_CODEC_TYPE_HEVC) || (eCodecType == E_VDEC_EX_CODEC_TYPE_VP9))
                {
                    bIsEVD = TRUE;
                }

                // Special case: MVC.
                //  - No other tasks can be run when MVC is running.
                //  - MVC must be task0 and use main index (u8Idx=0).
                if (E_VDEC_EX_CODEC_TYPE_MVC == eCodecType)
                {
                    MS_U32 u32VdecStreamId = 0;

                    for (i = 0; i < VDEC_MAX_SUPPORT_STREAM_NUM; i++)
                    {
                        if (TRUE == pVdecExContext->_Attr[i].bUsed)
                        {
                            _VDEC_EX_API_MutexUnlock();
                            return E_VDEC_EX_FAIL;
                        }
                    }

                    /// MVC use main stream.
#ifdef VDEC3
                    if (E_HVD_EX_OK == MDrv_HVD_EX_GetFreeStream(&u32VdecStreamId, E_HVD_EX_DRV_MVC_STREAM, bIsEVD, pVdecExContext->bDRVdecideBS))
#else
                    if (E_HVD_EX_OK == MDrv_HVD_EX_GetFreeStream(&u32VdecStreamId, E_HVD_EX_DRV_MVC_STREAM, bIsEVD))
#endif
                    {
                        u8Idx = 0; /// main stream
                        pStreamId->u32Id = (u8Idx << 24 | u32VdecStreamId);
                        _SET_DISPLAYMODE_INVALID(u8Idx);
                         _VDEC_EX_API_MutexUnlock();
                        return E_VDEC_EX_OK;
                    }
                    else
                    {
                        _VDEC_EX_API_MutexUnlock();
                        return E_VDEC_EX_FAIL;
                    }
                }
#ifndef VDEC3
                else if((E_VDEC_EX_CODEC_TYPE_VP8 == eCodecType)   //VP8 using esb3, esb4 in the same time
                        || (E_VDEC_EX_CODEC_TYPE_HEVC == eCodecType))   //EVD support one esb now
                {
                    for (i = 0; i < VDEC_MAX_SUPPORT_STREAM_NUM; i++)
                    {
                        if (((E_VDEC_EX_CODEC_TYPE_VP8 == pVdecExContext->_Attr[i].eCodecType) && (E_VDEC_EX_CODEC_TYPE_VP8 == eCodecType))
                                || ((E_VDEC_EX_CODEC_TYPE_HEVC == pVdecExContext->_Attr[i].eCodecType) && (E_VDEC_EX_CODEC_TYPE_HEVC == eCodecType)))
                        {
                            _VDEC_EX_API_MutexUnlock();
                            return E_VDEC_EX_FAIL;
                        }
                    }

                    if (E_VDEC_EX_MAIN_STREAM == eStreamType)
                    {
                        MS_U32 u32VdecStreamId = 0;

                        if (E_HVD_EX_OK == MDrv_HVD_EX_GetFreeStream(&u32VdecStreamId, E_HVD_EX_DRV_MAIN_STREAM, bIsEVD))
                        {
                            pStreamId->u32Id = (u8Idx << 24 | u32VdecStreamId);
                            _SET_DISPLAYMODE_INVALID(u8Idx);
                            _VDEC_EX_API_MutexUnlock();
                            return E_VDEC_EX_OK;
                        }
                    }
                    else if (E_VDEC_EX_SUB_STREAM == eStreamType)
                    {
                        MS_U32 u32VdecStreamId = 0;

                        if (E_HVD_EX_OK == MDrv_HVD_EX_GetFreeStream(&u32VdecStreamId, E_HVD_EX_DRV_SUB_STREAM, bIsEVD))
                        {
                            pStreamId->u32Id = (u8Idx << 24 | u32VdecStreamId);
                            _SET_DISPLAYMODE_INVALID(u8Idx);
                            _VDEC_EX_API_MutexUnlock();
                            return E_VDEC_EX_OK;
                        }
                    }
                }
#endif
                else
                {
#ifdef VDEC3
                    MS_U32 u32VdecStreamId = 0;

                    if (E_HVD_EX_OK == MDrv_HVD_EX_GetFreeStream(&u32VdecStreamId, E_HVD_EX_DRV_N_STREAM + u8Idx, bIsEVD, pVdecExContext->bDRVdecideBS))
                    {
                        pStreamId->u32Id = (u8Idx << 24 | u32VdecStreamId);
                        _SET_DISPLAYMODE_INVALID(u8Idx);
                        _VDEC_EX_API_MutexUnlock();
                        return E_VDEC_EX_OK;
                    }
#else
                    if (E_VDEC_EX_MAIN_STREAM == eStreamType)
                    {
                        MS_U32 u32VdecStreamId = 0;

                        if (E_HVD_EX_OK == MDrv_HVD_EX_GetFreeStream(&u32VdecStreamId, E_HVD_EX_DRV_MAIN_STREAM, bIsEVD))
                        {
                            pStreamId->u32Id = (u8Idx << 24 | u32VdecStreamId);
                            _SET_DISPLAYMODE_INVALID(u8Idx);
                            _VDEC_EX_API_MutexUnlock();
                            return E_VDEC_EX_OK;
                        }
                    }
                    else if (E_VDEC_EX_SUB_STREAM == eStreamType)
                    {
                        MS_U32 u32VdecStreamId = 0;

                        if (E_HVD_EX_OK == MDrv_HVD_EX_GetFreeStream(&u32VdecStreamId, E_HVD_EX_DRV_SUB_STREAM, bIsEVD))
                        {
                            pStreamId->u32Id = (u8Idx << 24 | u32VdecStreamId);
                            _SET_DISPLAYMODE_INVALID(u8Idx);
                            _VDEC_EX_API_MutexUnlock();
                            return E_VDEC_EX_OK;
                        }
                    }
#endif
                }

                break;
            }
            case E_VDEC_EX_DECODER_MJPEG:
            {
#ifdef VDEC3
                    MS_U32 u32VdecStreamId = 0;

                    if (E_MJPEG_RET_SUCCESS == MApi_MJPEG_GetFreeStream(&u32VdecStreamId, E_MJPEG_API_N_STREAM + u8Idx))
                    {
                        pStreamId->u32Id = (u8Idx << 24 | u32VdecStreamId);
                        _SET_DISPLAYMODE_INVALID(u8Idx);
                        _VDEC_EX_API_MutexUnlock();
                        return E_VDEC_EX_OK;
                    }
#else
                if (E_VDEC_EX_MAIN_STREAM == eStreamType)
                {
                    MS_U32 u32VdecStreamId = 0;

                    if (E_MJPEG_RET_SUCCESS == MApi_MJPEG_GetFreeStream(&u32VdecStreamId, E_MJPEG_API_MAIN_STREAM))
                    {
                        pStreamId->u32Id = (u8Idx << 24 | u32VdecStreamId);
                        _SET_DISPLAYMODE_INVALID(u8Idx);
                        _VDEC_EX_API_MutexUnlock();
                        return E_VDEC_EX_OK;
                    }
                }
                else if (E_VDEC_EX_SUB_STREAM == eStreamType)
                {
                    MS_U32 u32VdecStreamId = 0;

                    if (E_MJPEG_RET_SUCCESS == MApi_MJPEG_GetFreeStream(&u32VdecStreamId, E_MJPEG_API_SUB_STREAM))
                    {
                        pStreamId->u32Id = (u8Idx << 24 | u32VdecStreamId);
                        _SET_DISPLAYMODE_INVALID(u8Idx);
                        _VDEC_EX_API_MutexUnlock();
                        return E_VDEC_EX_OK;
                    }
                }
#endif
                break;
            }
            default:
                break;
        }
    }
    else
    {
        VDEC_PRINT("%s: invalid u32Version(%d) or u32Size(%d)\n", __FUNCTION__, u32Version, (MS_U32)sizeof(VDEC_StreamId));
    }
    _VDEC_EX_API_MutexUnlock();
    return E_VDEC_EX_FAIL;
}

//------------------------------------------------------------------------------
///Map context for multi process use
/// @param eStream \b IN : stream type
/// @return VDEC_EX_Result
///     - E_VDEC_EX_OK: success
///     - E_VDEC_EX_FAIL: failed
///     - E_VDEC_EX_RET_NOT_EXIT: not ext after last initialization
///     - E_VDEC_EX_RET_INVALID_PARAM: input parameter is invalid
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_GetConfig(VDEC_EX_Stream eStreamType)
{
    MApi_VDEC_EX_GetFreeStream(NULL, 0, 0, E_VDEC_EX_CODEC_TYPE_NONE);

    VDEC_StreamId stStreamId, stGetStreamId;
    VDEC_EX_User_Cmd cmd_id;

    if(eStreamType == E_VDEC_EX_MAIN_STREAM)
    {
        cmd_id = E_VDEC_EX_USER_CMD_GET_MAIN_STREAM_ID;
    }
    else if(eStreamType == E_VDEC_EX_SUB_STREAM)
    {
        cmd_id = E_VDEC_EX_USER_CMD_GET_SUB_STREAM_ID;
    }
    else
    {
        VPRINTF("[Fail][%s,%d], eStreamType: %d\n",__FUNCTION__,__LINE__, eStreamType);
        return E_VDEC_EX_FAIL;
    }

    VDEC_EX_Result res = MApi_VDEC_EX_GetControl( &stStreamId, cmd_id, (MS_U32*)(&stGetStreamId));

    if(E_VDEC_EX_OK == res)
    {
        VDEC_EX_Status stGetStatus;
        MApi_VDEC_EX_GetStatus(&stGetStreamId, &stGetStatus);

        if(E_VDEC_EX_STAGE_STOP != stGetStatus.eStage)
            return E_VDEC_EX_OK;
        else
            return E_VDEC_EX_FAIL;
    }
    else
        return E_VDEC_EX_FAIL;
}

//------------------------------------------------------------------------------
/// Initial VDEC.
/// @param pStreamId \b IN : the pointer to stream ID
/// @param pInitParam \b IN : pointer to initial parameter
/// @return VDEC_EX_Result
///     - E_VDEC_EX_OK: success
///     - E_VDEC_EX_FAIL: failed
///     - E_VDEC_EX_RET_NOT_EXIT: not ext after last initialization
///     - E_VDEC_EX_RET_INVALID_PARAM: input parameter is invalid
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_Init(VDEC_StreamId *pStreamId, VDEC_EX_InitParam *pInitParam)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
#if 0//!defined(VDEC_UTOPIA_2K)
        MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
        MDrv_MVD_ReleaseFreeStream(u32Id);TODO: fix me
#endif
        return E_VDEC_EX_FAIL;
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.param[0] = (void*)pInitParam;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,E_VDEC_EX_V2_CMD_INIT, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        ret = E_VDEC_EX_FAIL;
    }

    if(ret != E_VDEC_EX_OK)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
#if 0//!defined(VDEC_UTOPIA_2K)
        MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
        MDrv_MVD_ReleaseFreeStream(u32Id);TODO: fix me
#endif
    }

    return ret;
}

#ifdef VDEC3
static void _MApi_VDEC_EX_BS_Init(MS_PHY addr, MS_U32 size)
{
    if (!pVdecExContext)
        return;

    pVdecExContext->u32BitstreamBufAddr = addr;
    pVdecExContext->u32BitstreamBufSize = size;
}

/*
static MS_PHY _MApi_VDEC_EX_BS_GetAddr(MS_U8 u8Idx)
{
    if (u8Idx >= VDEC_MAX_SUPPORT_STREAM_NUM || !pVdecExContext)
        return 0;

    return pVdecExContext->u32BitstreamBufAddr + (pVdecExContext->u32BitstreamBufSize / VDEC_MAX_SUPPORT_STREAM_NUM * u8Idx);
}

static MS_U32 _MApi_VDEC_EX_BS_GetSize(MS_U8 u8Idx)
{
    if (u8Idx >= VDEC_MAX_SUPPORT_STREAM_NUM || !pVdecExContext)
        return 0;

    return pVdecExContext->u32BitstreamBufSize / VDEC_MAX_SUPPORT_STREAM_NUM;
}
*/
#endif

VDEC_EX_Result MApi_VDEC_EX_V2_Init(VDEC_StreamId *pStreamId, VDEC_EX_InitParam *pInitParam)
{
    MS_BOOL bNotReloadFW = FALSE;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    VDEC_EX_CapCmd cap;
#ifdef CMA_DRV_DIRECT_INIT
    MS_SIZE unUseSize = 0;
#endif
    MS_BOOL bNDecAllocator = _VDEC_EX_NdecAllocator_IsNDecode();//VDEC_EX_ALLOCATOR_ENABLE;

    _VDEC_INVALID_IDX_RET(u8Idx);

    _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);

    if (pVdecExContext->_Attr[u8Idx].bInit)
    {
        VDEC_PRINT("VDEC Warn: re-init VDEC Driver\n");
#if defined( MS_DEBUG )
        return E_VDEC_EX_RET_NOT_EXIT;
#endif
    }

    if(bSecureModeEnable == TRUE)
    {
        SYS_TEEINFO teemode;
        MDrv_SYS_ReadKernelCmdLine();
        MDrv_SYS_GetTEEInfo(&teemode);
        VPRINTF("[VDEC][TEE/OPTEE]%s,TEEmode_type=%d\n",__FUNCTION__,teemode.OsType);
        if(teemode.OsType == SYS_TEEINFO_OSTYPE_OPTEE)
        {
            pVdecExContext->_Pre_Ctrl[u8Idx].bEnableDynamicCMA = 0;
            VPRINTF("[VDEC][OPTEE]secure mode can't use dynamic CMA\n");
        }
    }

    u8CodecCompare[u8Idx] = 0;

    _VDEC_EX_InitLocalVar(u8Idx);

    _VDEC_Memcpy(&(pVdecExContext->_Attr[u8Idx].vdecExInitParam), pInitParam, sizeof(VDEC_EX_InitParam));

    pVdecExContext->_Attr[u8Idx].eCodecType    = pInitParam->eCodecType;
    pVdecExContext->_Attr[u8Idx].eDecoder      = _VDEC_EX_GetDecoderByCodecType(pInitParam->eCodecType);
    pVdecExContext->_Attr[u8Idx].eSrcMode      = pInitParam->VideoInfo.eSrcMode;
    pVdecExContext->_Attr[u8Idx].eDbgMsgLevel  = pInitParam->SysConfig.eDbgMsgLevel;

    #if defined(CHIP_KANO) || defined(CHIP_CURRY) //|| defined(HAL_CHIP_SUPPORT_DUAL_R2)
    MS_BOOL bIsEVD = FALSE;
    MS_U8 u8Cidx = 0;
    MS_U16 u16ChipECORev = (MS_U16)MDrv_SYS_GetChipRev();
    bIsEVD = (E_VDEC_EX_CODEC_TYPE_HEVC == pVdecExContext->_Attr[u8Idx].eCodecType) || (E_VDEC_EX_CODEC_TYPE_VP9 == pVdecExContext->_Attr[u8Idx].eCodecType);

    _VDEC_EX_API_MutexLock();

    #ifdef VDEC3
    _VDEC_EX_SetCidx(pStreamId, bIsEVD, pVdecExContext->bDRVdecideBS);
    #else
    _VDEC_EX_SetCidx(pStreamId, bIsEVD);
    #endif
    _VDEC_EX_GetCidx(pStreamId, &u8Cidx);

    #if VDEC_EXCHANGE_CIDX_PATCH
    if(u16ChipECORev == 0)
    {
        if (u8Cidx == 1)
        {
            if ((E_VDEC_EX_SRC_MODE_DTV == pVdecExContext->_Attr[u8Idx].eSrcMode) ||
                (E_VDEC_EX_SRC_MODE_TS_FILE == pVdecExContext->_Attr[u8Idx].eSrcMode) ||
                (E_VDEC_EX_SRC_MODE_TS_FILE_DUAL_ES == pVdecExContext->_Attr[u8Idx].eSrcMode))
            {
                _VDEC_EX_ExchangeCidx(pStreamId);
                u8Cidx = 0;
            }
        }
    }
    #endif

    #ifdef VDEC3
    VDEC_EX_SysCfg* tmpCfg = &(pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig);
    #else
    VDEC_EX_SysCfg* tmpCfg = &(pInitParam->SysConfig);
    #endif

    //for MMAP with E_MMAP_ID_VDEC_CPU_LEN smaller than 0x600000, use HI_CODEC decoder only
    #define SIZE_3MB 0x300000
    #define SIZE_6MB 0x600000

    if (tmpCfg->u32CodeBufSize < SIZE_6MB)
    {
        if (u8Cidx == 1)
        {
            _VDEC_EX_ExchangeCidx(pStreamId);
            u8Cidx = 0;
        }
    }

    if (u8Cidx == 1)
    {
        tmpCfg->u32CodeBufAddr = tmpCfg->u32CodeBufAddr + SIZE_3MB;
    }

    _VDEC_EX_API_MutexUnlock();
    #endif

    if(bNDecAllocator == FALSE)
    {
#ifdef CMA_DRV_DIRECT_INIT
        if (pVdecExContext->bCMAUsed && u8Idx == 0 && (pVdecExContext->_power_state[u8Idx] != E_VDEC_EX_POWER_RESUME_ING))
        {
            pVdecExContext->cmaInitParam[0].heap_id = ION_VDEC_HEAP_ID;
            pVdecExContext->cmaInitParam[0].flags = CMA_FLAG_MAP_VMA| CMA_FLAG_CACHED;

            if (MApi_CMA_Pool_Init(&pVdecExContext->cmaInitParam[0]) == FALSE)
            {
                VPRINTF("[%s][%d]CMA init fail\n",__FUNCTION__,__LINE__);
                return E_VDEC_EX_FAIL;
            }
            else
            {
                bCMAInitPool[0] = TRUE;
                if(pInitParam->SysConfig.u32FrameBufAddr != 0)
                {
                    MS_PHY temp_addr;
                    _miu_offset_to_phy(pVdecExContext->cmaInitParam[0].miu,
                                       pVdecExContext->cmaInitParam[0].heap_miu_start_offset,
                                       temp_addr);

                    unUseSize = pInitParam->SysConfig.u32FrameBufAddr - temp_addr;
                }

                VPRINTF("[VDEC][%d] MApi_CMA_Pool_Init[0]: pool_handle_id=%x, miu=%d, offset=%llx, length=%x , FrameBuffer=%llx, unUseSize=%x\n",
                                    (unsigned int)u8Idx,
                                    (unsigned int)pVdecExContext->cmaInitParam[0].pool_handle_id,
                                    (unsigned int)pVdecExContext->cmaInitParam[0].miu,
                                    (unsigned long long int)pVdecExContext->cmaInitParam[0].heap_miu_start_offset,
                                    (unsigned int)pVdecExContext->cmaInitParam[0].heap_length,
                                    (unsigned long long int)pInitParam->SysConfig.u32FrameBufAddr,
                                    (unsigned int)unUseSize);
            }
            pVdecExContext->cmaInitParam[1].heap_id = ION_VDEC_HEAP_ID + 1;
            pVdecExContext->cmaInitParam[1].flags = CMA_FLAG_MAP_VMA | CMA_FLAG_CACHED;

            if (MApi_CMA_Pool_Init(&pVdecExContext->cmaInitParam[1]) == FALSE)
            {
                VPRINTF("[VDEC][%d] MApi_CMA_Pool_Init[1] Failed\n", (unsigned int)u8Idx);
            }
            else
            {
                bCMAInitPool[1] = TRUE;
                /*
                BALANCE_BW CMA ID=20, and always start from 0
                if(pInitParam->SysConfig.u32FrameBufAddr != 0)
                {
                    MS_PHY temp_addr;
                    _miu_offset_to_phy(pVdecExContext->cmaInitParam[1].miu,
                                       pVdecExContext->cmaInitParam[1].heap_miu_start_offset,
                                       temp_addr);

                    unUseSize = pInitParam->SysConfig.u32FrameBufAddr - temp_addr;
                }*/

                VPRINTF("[VDEC][%d] MApi_CMA_Pool_Init[1]: pool_handle_id=%x, miu=%d, offset=%llx, length=%x , FrameBuffer=%llx, unUseSize=%x\n",
                                    (unsigned int)u8Idx,
                                    (unsigned int)pVdecExContext->cmaInitParam[1].pool_handle_id,
                                    (unsigned int)pVdecExContext->cmaInitParam[1].miu,
                                    (unsigned long long int)pVdecExContext->cmaInitParam[1].heap_miu_start_offset,
                                    (unsigned int)pVdecExContext->cmaInitParam[1].heap_length,
                                    (unsigned long long int)pInitParam->SysConfig.u32FrameBufAddr,
                                    (unsigned int)unUseSize);
            }
        }
#endif
    }

#if (VDEC_EX_ISR_MONITOR)
    //if (FALSE == pVdecExContext->_Attr[u8Idx].stEventMon.bIsSysEnable)
    {
        if (FALSE == _VDEC_EX_SYS_Init(u8Idx))
        {
            return E_VDEC_EX_FAIL;
        }
        //pVdecExContext->_Attr[u8Idx].stEventMon.bIsSysEnable = TRUE;
    }
#else
    //_VDEC_EX_MutexInit(u8Idx);
#endif

    if(bNDecAllocator == FALSE)
    {
#ifdef CMA_DRV_DIRECT_INIT
        if (pVdecExContext->bCMAUsed && bCMAInitPool[0] && u8Idx == 0 && (pVdecExContext->_power_state[u8Idx] != E_VDEC_EX_POWER_RESUME_ING))
        {
            struct CMA_Pool_Alloc_Param alloc_param;

            memset(&alloc_param,0,sizeof(struct CMA_Pool_Alloc_Param));

            alloc_param.pool_handle_id = pVdecExContext->cmaInitParam[0].pool_handle_id;
            alloc_param.flags = CMA_FLAG_VIRT_ADDR;
            MS_BOOL result = MDrv_HVD_EX_GetCMAMemSize(
                pVdecExContext->_Attr[u8Idx].eCodecType, pVdecExContext->_Attr[u8Idx].eSrcMode,
                &alloc_param.offset_in_pool, &alloc_param.length, pVdecExContext->cmaInitParam[0].heap_length, unUseSize);

            #if VDEC_DUAL_FRAME_BUFFER_MANAGEMENT
            alloc_param.length = pVdecExContext->cmaInitParam[0].heap_length - unUseSize;
            #endif

            if( result == FALSE )
            {
                VPRINTF("[%s][%d]CMA FB resource is not enough\n",__FUNCTION__,__LINE__);
                return E_VDEC_EX_FAIL;
            }

            if(pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize != 0)
            {
                if(pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize >  alloc_param.length )
                {
                    VPRINTF("[VDEC][%d][CMA]u32FrameBufSize is not 0\n",(unsigned int)u8Idx);
                    pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize = alloc_param.length;
                }
                else //using min value to setup frame buffer from cma
                {
                    VPRINTF("[VDEC][%d][CMA]u32FrameBufSize is not 0 and small than cma length\n",(unsigned int)u8Idx);
                    alloc_param.length = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize;
                }
            }
            else //frame buffer size = 0 , using minimun size as frame buffer size, the alloc_param.length already be decided as min value
            {
                VPRINTF("[VDEC][%d][CMA]u32FrameBufSize is 0\n",(unsigned int)u8Idx);
                pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize = alloc_param.length;
            }

            if (pVdecExContext->bCMAGetMem[0][u8Idx] == FALSE)
            {
                pVdecExContext->bCMAGetMem[0][u8Idx] = TRUE;
                if (MApi_CMA_Pool_GetMem(&alloc_param) == FALSE)
                {
                    pVdecExContext->bCMAGetMem[0][u8Idx] = FALSE;
                    VPRINTF("[%s][%d]CMA GET fail\n",__FUNCTION__,__LINE__);
                    return E_VDEC_EX_FAIL;
                }
                else
                {
                    VPRINTF("[VDEC][%d]MApi_CMA_Pool_GetMem0: alloc_param.pool_handle_id=%x, alloc_param.flags=%x, alloc_param.offset_in_pool=%llx, alloc_param.length=%x\n",
                                        (unsigned int)u8Idx,
                                        (unsigned int)alloc_param.pool_handle_id,
                                        (unsigned int)alloc_param.flags,
                                        (unsigned long long int)alloc_param.offset_in_pool,
                                        (unsigned int)alloc_param.length);
                }
            }

            pVdecExContext->cmaFreeParam[0][u8Idx].pool_handle_id = pVdecExContext->cmaInitParam[0].pool_handle_id;
            pVdecExContext->cmaFreeParam[0][u8Idx].offset_in_pool = alloc_param.offset_in_pool;
            pVdecExContext->cmaFreeParam[0][u8Idx].length = alloc_param.length;

            _miu_offset_to_phy(pVdecExContext->cmaInitParam[0].miu,
                               pVdecExContext->cmaInitParam[0].heap_miu_start_offset + alloc_param.offset_in_pool,
                               pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufAddr);
#if 0
            if(pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize != 0)
            {
                if(pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize >  alloc_param.length )
                {
                    VPRINTF("[VDEC][%d][CMA]u32FrameBufSize is not 0\n",(unsigned int)u8Idx);
                    pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize = alloc_param.length;
                }
            }
            else
            {
                VPRINTF("[VDEC][%d][CMA]u32FrameBufSize is 0 \n",(unsigned int)u8Idx);
                pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize = alloc_param.length;
            }
#endif
            pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize = alloc_param.length;
        }
#endif

        #if VDEC_DUAL_FRAME_BUFFER_MANAGEMENT
        VDEC_EX_CodecType eCodecType = pVdecExContext->_Attr[u8Idx].eCodecType;
        VDEC_EX_Decoder eDecoder = pVdecExContext->_Attr[u8Idx].eDecoder;
        MS_BOOL bTaskIsMVD = (eDecoder==E_VDEC_EX_DECODER_MVD)?(TRUE):(FALSE);
        MS_BOOL bTaskIsHVD = ((eDecoder==E_VDEC_EX_DECODER_HVD)&&(eCodecType!=E_VDEC_EX_CODEC_TYPE_HEVC)&&(eCodecType!=E_VDEC_EX_CODEC_TYPE_VP9))?(TRUE):(FALSE);
        MS_BOOL bTaskIsMJPEG = (eDecoder==E_VDEC_EX_DECODER_MJPEG)?(TRUE):(FALSE);
        MS_BOOL bCodecIsVP8 = (eCodecType==E_VDEC_EX_CODEC_TYPE_VP8)?(TRUE):(FALSE);

        MS_U8 u8AnoIdx = 1 - u8Idx;
        VDEC_EX_CodecType eAnoCodecType = pVdecExContext->_Attr[u8AnoIdx].eCodecType;
        VDEC_EX_Decoder eAnoDecoder = pVdecExContext->_Attr[u8AnoIdx].eDecoder;
        MS_BOOL bAnoTaskIsMVD = (eAnoDecoder==E_VDEC_EX_DECODER_MVD)?(TRUE):(FALSE);
        MS_BOOL bAnoTaskIsHVD = ((eAnoDecoder==E_VDEC_EX_DECODER_HVD)&&(eAnoCodecType!=E_VDEC_EX_CODEC_TYPE_HEVC)&&(eAnoCodecType!=E_VDEC_EX_CODEC_TYPE_VP9))?(TRUE):(FALSE);
        MS_BOOL bAnoTaskIsEVD = (eAnoCodecType==E_VDEC_EX_CODEC_TYPE_HEVC) || (eAnoCodecType==E_VDEC_EX_CODEC_TYPE_VP9);

        MS_U8 u8MiuSel;
        MS_U8 u8IdxSel;
        MS_U32 u32StartOffset;
        _phy_to_miu_offset(u8MiuSel, u32StartOffset, pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufAddr);
        pVdecExContext->_stFBMng[u8Idx].u8Miu = u8MiuSel;

        pVdecExContext->_stFBMng[u8Idx].u32FrameBufAddr = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufAddr;
        pVdecExContext->_stFBMng[u8Idx].u32FrameBufSize = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize;

        // dispatch eLocation and frame buffer
        if (pVdecExContext->_stFBMng[u8Idx].u32FrameBufSize >= FBMNG_AVC4K_SIZE)
        {
            if (pVdecExContext->_Attr[u8AnoIdx].bUsed)
            {
                if(bTaskIsMJPEG || bCodecIsVP8)
                {
                    VDEC_EX_FB_LOCATION eAnoLocation = pVdecExContext->_stFBMng[u8AnoIdx].eLocation;
                    MS_U8 u8AnoIdxSel = (eAnoLocation & FBLOC_TASK_FB_MASK)?(1):(0);

                    if((bAnoTaskIsEVD && pVdecExContext->_stFBMng[u8AnoIdxSel].u8Miu == 1) ||
                       (bAnoTaskIsHVD && pVdecExContext->_stFBMng[u8AnoIdxSel].u8Miu == 0 && bCodecIsVP8))
                    {
                        _VDEC_EX_API_MutexLock();
                        _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                        _VDEC_EX_API_MutexUnlock();
                        VDEC_ERR("[VDEC][%s][%d] VP8 or MJPEG FB must use MIU1\n", __FUNCTION__, __LINE__);
                        return E_VDEC_EX_FAIL;
                    }
                    else
                    {
                        if(pVdecExContext->_stFBMng[u8AnoIdxSel].u8Miu == 1)
                        {
                            pVdecExContext->_stFBMng[u8Idx].eLocation = eAnoLocation^FBLOC_FRONT_TAIL_MASK;
                            u8IdxSel = ((pVdecExContext->_stFBMng[u8Idx].eLocation)&FBLOC_TASK_FB_MASK)?(1):(0);

                            if (pVdecExContext->_stFBMng[u8Idx].eLocation & FBLOC_FRONT_TAIL_MASK)  //tail
                            {
                                pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufAddr = pVdecExContext->_stFBMng[u8IdxSel].u32FrameBufAddr + FBMNG_AVC4K_SIZE;
                                pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize = pVdecExContext->_stFBMng[u8IdxSel].u32FrameBufSize - FBMNG_AVC4K_SIZE;
                            }
                            else  //front
                            {
                                pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufAddr = pVdecExContext->_stFBMng[u8IdxSel].u32FrameBufAddr;
                                pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize = FBMNG_AVC4K_SIZE;
                            }
                        }
                        else
                        {
                            pVdecExContext->_stFBMng[u8Idx].eLocation = eAnoLocation^FBLOC_TASK_FB_MASK;
                            pVdecExContext->_stFBMng[u8Idx].eLocation = (pVdecExContext->_stFBMng[u8Idx].eLocation)&FBLOC_FORCE_FRONT_MASK;  //force front

                            u8IdxSel = ((pVdecExContext->_stFBMng[u8Idx].eLocation)&FBLOC_TASK_FB_MASK)?(1):(0);

                            pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufAddr = pVdecExContext->_stFBMng[u8IdxSel].u32FrameBufAddr;
                            pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize = FBMNG_AVC4K_SIZE;
                        }
                    }
                }
                else
                {
                    if (pVdecExContext->_stFBMng[u8Idx].u8Miu != pVdecExContext->_stFBMng[u8AnoIdx].u8Miu)
                    {
                        if ((bTaskIsMVD && bAnoTaskIsMVD) || (bTaskIsHVD && bAnoTaskIsHVD))  // allocate from the same frame buffer of another task
                        {
                            if (pVdecExContext->_stFBMng[u8AnoIdx].eLocation != E_VDEC_EX_FB_NONE)
                            {
                                pVdecExContext->_stFBMng[u8Idx].eLocation = (pVdecExContext->_stFBMng[u8AnoIdx].eLocation)^FBLOC_FRONT_TAIL_MASK;
                                u8IdxSel = ((pVdecExContext->_stFBMng[u8Idx].eLocation)&FBLOC_TASK_FB_MASK)?(1):(0);

                                if (pVdecExContext->_stFBMng[u8Idx].eLocation & FBLOC_FRONT_TAIL_MASK)  //tail
                                {
                                    pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufAddr = pVdecExContext->_stFBMng[u8IdxSel].u32FrameBufAddr + FBMNG_AVC4K_SIZE;
                                    pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize = pVdecExContext->_stFBMng[u8IdxSel].u32FrameBufSize - FBMNG_AVC4K_SIZE;
                                }
                                else  //front
                                {
                                    pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufAddr = pVdecExContext->_stFBMng[u8IdxSel].u32FrameBufAddr;
                                    pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize = FBMNG_AVC4K_SIZE;
                                }
                            }
                            else
                            {
                                VDEC_PRINT("[%s][%d] The behavior of FrameBuffer Allocation is abnormal \n", __FUNCTION__, __LINE__);
                            }
                        }
                        else if (bTaskIsMVD || bTaskIsHVD)
                        {
                            pVdecExContext->_stFBMng[u8Idx].eLocation = (pVdecExContext->_stFBMng[u8AnoIdx].eLocation)^FBLOC_TASK_FB_MASK;
                            pVdecExContext->_stFBMng[u8Idx].eLocation = (pVdecExContext->_stFBMng[u8Idx].eLocation)&FBLOC_FORCE_FRONT_MASK;  //force front

                            u8IdxSel = ((pVdecExContext->_stFBMng[u8Idx].eLocation)&FBLOC_TASK_FB_MASK)?(1):(0);

                            pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufAddr = pVdecExContext->_stFBMng[u8IdxSel].u32FrameBufAddr;
                            pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize = FBMNG_AVC4K_SIZE;
                        }
                        else
                        {
                            pVdecExContext->_stFBMng[u8Idx].eLocation = (pVdecExContext->_stFBMng[u8AnoIdx].eLocation)^FBLOC_TASK_FB_MASK;
                            pVdecExContext->_stFBMng[u8Idx].eLocation = (pVdecExContext->_stFBMng[u8Idx].eLocation)&FBLOC_FORCE_FRONT_MASK;  //force front

                            u8IdxSel = ((pVdecExContext->_stFBMng[u8Idx].eLocation)&FBLOC_TASK_FB_MASK)?(1):(0);

                            pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufAddr = pVdecExContext->_stFBMng[u8IdxSel].u32FrameBufAddr;
                        }
                    }
                    else
                    {
                        VDEC_PRINT("[%s][%d] Two frame buffer on the same miu, no need to run FrameBuffer Allocation \n", __FUNCTION__, __LINE__);
                    }
                }
            }
            else
            {
                if (bTaskIsMVD || bTaskIsHVD || bTaskIsMJPEG)  // allocate front for MVD/HVD/MJPEG
                {
                    pVdecExContext->_stFBMng[u8Idx].eLocation = (u8Idx)?(E_VDEC_EX_FB1_FRONT):(E_VDEC_EX_FB0_FRONT);  //front
                    pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize = FBMNG_AVC4K_SIZE;
                }
                else  // EVD use the whole FB, marked as front
                {
                    pVdecExContext->_stFBMng[u8Idx].eLocation = (u8Idx)?(E_VDEC_EX_FB1_FRONT):(E_VDEC_EX_FB0_FRONT);  //front
                }
            }
        }
        else
        {
            VDEC_PRINT("[%s][%d] Two frame buffer on the same miu, no need to run FrameBuffer Allocation \n", __FUNCTION__, __LINE__);
        }

        VDEC_PRINT("VDEC FBMng0 0x%x, 0x%x, 0x%x \n", pVdecExContext->_stFBMng[0].eLocation, (MS_U32)(pVdecExContext->_stFBMng[0].u32FrameBufAddr), pVdecExContext->_stFBMng[0].u32FrameBufSize);
        VDEC_PRINT("VDEC FBMng1 0x%x, 0x%x, 0x%x \n", pVdecExContext->_stFBMng[1].eLocation, (MS_U32)(pVdecExContext->_stFBMng[1].u32FrameBufAddr), pVdecExContext->_stFBMng[1].u32FrameBufSize);
        VDEC_PRINT("VDEC FB Used      0x%x, 0x%x \n", (MS_U32)(pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufAddr), pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize);
        #endif
    }
    else
    {
        VDEC_EX_Allocator_Block *pstBlockFB1 = NULL, *pstBlockBS = NULL;
        VDEC_EX_Result eRet;
        VDEC_EX_ALLOCATOR_BUFFER_TYPE eBufType;
        VDEC_EX_NDecBufRangeParam stBufRgnFB1, stBufRgnBS;

        _VDEC_EX_API_MutexLock();

        // frame Buffer 1 allocating setting

        eBufType = E_VDEC_EX_ALLOCATOR_BUFFER_FB1;

        _VDEC_Memset(&stBufRgnFB1, 0, sizeof(VDEC_EX_NDecBufRangeParam));
        if(pVdecExContext->_Pre_Ctrl[u8Idx].stTotalBufRgnFB1.bSetTotalBuf == TRUE)
        {
            stBufRgnFB1.bSetTotalBuf = TRUE;
            stBufRgnFB1.phyTotalBufAddr = pVdecExContext->_Pre_Ctrl[u8Idx].stTotalBufRgnFB1.phyTotalBufAddr;
            stBufRgnFB1.szTotalBufSize = pVdecExContext->_Pre_Ctrl[u8Idx].stTotalBufRgnFB1.szTotalBufSize;
        }
        stBufRgnFB1.phyBufAddr = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufAddr;
        stBufRgnFB1.szBufSize = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize;

        // start allocating frame buffer 1

        eRet = _VDEC_EX_NdecAllocator_MallocFrameBuffer(u8Idx, eBufType, &stBufRgnFB1, &pstBlockFB1);
        if(eRet != E_VDEC_EX_OK)
        {
            _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
            _VDEC_EX_API_MutexUnlock();
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d][%d] Alloc Frame Buffer Fail\n", __FUNCTION__, __LINE__, u8Idx);
            return E_VDEC_EX_FAIL;
        }
        pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufAddr = pstBlockFB1->stBlockInfo.phyAddr;
        pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize = pstBlockFB1->stBlockInfo.szSize;

        // bitstream Buffer allocating setting

        eBufType = E_VDEC_EX_ALLOCATOR_BUFFER_BS;

        _VDEC_Memset(&stBufRgnBS, 0, sizeof(VDEC_EX_NDecBufRangeParam));
        if(pVdecExContext->_Pre_Ctrl[u8Idx].stTotalBufRgnBS.bSetTotalBuf == TRUE)
        {
            stBufRgnBS.bSetTotalBuf = TRUE;
            stBufRgnBS.phyTotalBufAddr = pVdecExContext->_Pre_Ctrl[u8Idx].stTotalBufRgnBS.phyTotalBufAddr;
            stBufRgnBS.szTotalBufSize = pVdecExContext->_Pre_Ctrl[u8Idx].stTotalBufRgnBS.szTotalBufSize;
        }
        stBufRgnBS.phyBufAddr = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32BitstreamBufAddr;
        stBufRgnBS.szBufSize = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32BitstreamBufSize;

        // start allocating bitstream buffer

        eRet = _VDEC_EX_NdecAllocator_MallocBitstreamBuffer(u8Idx, eBufType, &stBufRgnBS, &pstBlockBS);
        if(eRet != E_VDEC_EX_OK)
        {
            _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
            _VDEC_EX_API_MutexUnlock();
            ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d][%d] Alloc Bitstream Buffer Fail\n", __FUNCTION__, __LINE__, u8Idx);
            return E_VDEC_EX_FAIL;
        }
        pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32BitstreamBufAddr = pstBlockBS->stBlockInfo.phyAddr;
        pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32BitstreamBufSize = pstBlockBS->stBlockInfo.szSize;

        if(_VDEC_EX_NdecAllocator_IsNDecode() == TRUE)
        {
            MS_PHY phyBSTotalAddrStart = 0;
            MS_SIZE szBSToltalSize = 0;

            if(stBufRgnBS.bSetTotalBuf == TRUE)
            {
                phyBSTotalAddrStart = stBufRgnBS.phyTotalBufAddr;
                szBSToltalSize = stBufRgnBS.szTotalBufSize;
            }
            else
            {
                eRet = _VDEC_EX_NdecAllocator_GetBitstreamBufferTotalAddrSize(&phyBSTotalAddrStart, &szBSToltalSize);
                if(eRet != E_VDEC_EX_OK)
                {
                    _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                    _VDEC_EX_API_MutexUnlock();
                    ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d][%d] Get Total BS AddrSize Fail\n", __FUNCTION__, __LINE__, u8Idx);
                    return E_VDEC_EX_FAIL;
                }
            }

            _MApi_VDEC_EX_BS_Init(phyBSTotalAddrStart, szBSToltalSize);

            VPRINTF("[ALLOC][DBG][%s][%d][%d] BSTotalAddrStart=0x%llx, BSToltalSize=0x%x\n", __FUNCTION__, __LINE__, u8Idx,
                phyBSTotalAddrStart, szBSToltalSize);
        }
        VPRINTF("[ALLOC][DBG][%s][%d][%d] FB=0x%llx, Size=0x%x\n", __FUNCTION__, __LINE__, u8Idx,
            pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufAddr,
            pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize);
        VPRINTF("[ALLOC][DBG][%s][%d][%d] BS=0x%llx, Size=0x%x\n", __FUNCTION__, __LINE__, u8Idx,
            pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32BitstreamBufAddr,
            pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32BitstreamBufSize);

        _VDEC_EX_API_MutexUnlock();
    }

    //Check HW capability
    if(pInitParam->EnableDynaScale)
    {
        MS_U32 u32Caps = 0;
        cap = E_VDEC_EX_CAP_XC_NOT_SUPPORT_DS;
        if(E_VDEC_EX_OK == _VDEC_EX_GetCaps(cap, &u32Caps))
        {
            if(u32Caps)
            {
                VDEC_PRINT("VDEC_EX Warn: DS unsupported! Force EnableDynaScale as FALSE.\n");
                pInitParam->EnableDynaScale = FALSE;
                pVdecExContext->_Attr[u8Idx].vdecExInitParam.EnableDynaScale = FALSE;
            }
        }
    }

    // init debug message
    _VDEC_EX_SetDbgLevel(pStreamId, pVdecExContext->_Attr[u8Idx].eDbgMsgLevel);

#if VDEC_ENABLE_LINK_WEAK_SYMBOL
    // it is critical very any nos compile envi
    _VDEC_LinkWeakSymbolPatch();
#endif

    _VDEC_EX_API_MutexLock();

    if(bSecureModeEnable == TRUE)
    {
        SYS_TEEINFO teemode;
        MDrv_SYS_ReadKernelCmdLine();
        MDrv_SYS_GetTEEInfo(&teemode);
        VPRINTF("[VDEC][TEE/OPTEE]%s,TEEmode_type=%d\n",__FUNCTION__,teemode.OsType);

        if(teemode.OsType == SYS_TEEINFO_OSTYPE_OPTEE)
        {
            VPRINTF("[VDEC][OPTEE]Send Loadcode command\n");
            if(_MApi_VDEC_EX_V2_Send_OPTEE_CMD(E_VDEC_EX_OPTEE_LOADCODE,NULL,0) == FALSE)
            {
                VPRINTF("[VDEC][OPTEE]Loadcode command fail\n");
                return E_VDEC_EX_FAIL;
            }

            VDEC_EX_OPTEE_SecureBuffer SecureBuffer;
            SecureBuffer.VPUBufAddr = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32CodeBufAddr;
            SecureBuffer.VPUBufSize = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32CodeBufSize;
            SecureBuffer.BSBufAddr = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32BitstreamBufAddr;
            SecureBuffer.BSBufSize = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32BitstreamBufSize;
            SecureBuffer.FBufAddr = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufAddr;
            SecureBuffer.FBufSize = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize;
            SecureBuffer.u32streamID = pStreamId->u32Id;
            SecureBuffer.eCodecType = pVdecExContext->_Attr[u8Idx].eCodecType;

            VPRINTF("[VDEC][OPTEE]Send Set Buffer command\n");
            if(_MApi_VDEC_EX_V2_Send_OPTEE_CMD(E_VDEC_EX_OPTEE_SECURE_BUFFER,&SecureBuffer,sizeof(SecureBuffer)) == FALSE)
            {
                VPRINTF("[VDEC][OPTEE]Set Buffer command fail\n");
                return E_VDEC_EX_FAIL;
            }
        }
    }

    _VDEC_EX_SetDecodeMode(pStreamId, &(pVdecExContext->_Pre_DecModeCfg));

    if (pVdecExContext->bTurboMode)
    {
        bNotReloadFW = TRUE;
    }

    if (pVdecExContext->_bSingleDecodeMode == TRUE)
    {
        _VDEC_EX_HandleSingleDecodeMode(pVdecExContext->_Attr[u8Idx].eDecoder);
    }

    _VDEC_EX_API_MutexUnlock();

    switch (pVdecExContext->_Attr[u8Idx].eDecoder)
    {
        case E_VDEC_EX_DECODER_MVD:
        _RET_VDEC_MVD_LINT_CHECK_AND_RETURN_MUTEX();
        {
            pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_INIT;
            MVD_RecordStreamId(u32Id);

            if (pVdecExContext->_Pre_Ctrl[u8Idx].bHWBufferReMapping)
            {
                MDrv_MVD_HWBuffer_ReMappingMode(u32Id,pVdecExContext->_Pre_Ctrl[u8Idx].bHWBufferReMapping);
            }

#ifdef VDEC3
            VDEC_EX_SysCfg cfg = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig;
#else
            VDEC_EX_SysCfg cfg = pInitParam->SysConfig;
#endif
            MVD_FWCfg fwCfg;
            MVD_MEMCfg memCfg;
            MVD_CodecType eMvdCodecType = E_MVD_CODEC_UNKNOWN;
            MVD_SrcMode   eMvdSrcMode;
            MVD_TIMESTAMP_TYPE eSyncType = E_MVD_TIMESTAMP_FREERUN;
            //MS_BOOL bHDmode = TRUE;
            // check codec type
            eMvdCodecType = _VDEC_EX_Map2MVDCodecType(pVdecExContext->_Attr[u8Idx].eCodecType);
            if (eMvdCodecType == E_MVD_CODEC_UNKNOWN)
            {
                _VDEC_EX_API_MutexLock();
                _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                _VDEC_EX_API_MutexUnlock();
                //VDEC_PRINT("%s(%d): Unknown eMvdCodecType!\n", __FUNCTION__, __LINE__);
                pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_STOP;
                return E_VDEC_EX_FAIL;
            }

            // check source type
            eMvdSrcMode = _VDEC_EX_Map2MVDSrcMode(pVdecExContext->_Attr[u8Idx].eSrcMode);
            if ((eMvdSrcMode == 0xff) || (eMvdSrcMode >= MVD_SRCMODE_MAX))
            {
                _VDEC_EX_API_MutexLock();
                _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                _VDEC_EX_API_MutexUnlock();
                //VDEC_PRINT("%s(%d): Invalid eMvdSrcMode!\n", __FUNCTION__, __LINE__);
                pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_STOP;
                return E_VDEC_EX_FAIL;
            }

            _VDEC_Memset(&fwCfg, 0, sizeof(MVD_FWCfg));
            _VDEC_Memset(&memCfg, 0, sizeof(MVD_MEMCfg));

            fwCfg.eCodecType = eMvdCodecType;
            fwCfg.eSrcMode   = eMvdSrcMode;
            fwCfg.bDisablePESParsing = bMvdParserDisable[eMvdSrcMode];
            fwCfg.bNotReload = bNotReloadFW; //TRUE for not load fw more than once
            fwCfg.stFBReduction.LumaFBReductionMode = (MVD_FB_Reduction_Type)pInitParam->stFBReduction.eLumaFBReduction;
            fwCfg.stFBReduction.ChromaFBReductionMode = (MVD_FB_Reduction_Type)pInitParam->stFBReduction.eChromaFBReduction;
            memCfg.eFWSrcType = (MVD_FWSrcType)cfg.eFWSourceType;
            memCfg.u32FWBinAddr = cfg.u32FWBinaryAddr;
            memCfg.u32FWBinSize = cfg.u32FWBinarySize;
            memCfg.u32FWCodeAddr = cfg.u32CodeBufAddr;
            memCfg.u32FWCodeSize = cfg.u32CodeBufSize;
            memCfg.u32FBAddr = cfg.u32FrameBufAddr;
            memCfg.u32FBSize = cfg.u32FrameBufSize;
#if v3_thinplayer//def VDEC3
            if (pVdecExContext->bDRVdecideBS)
            {
                memCfg.u32BSAddr=pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32BitstreamBufAddr;
                memCfg.u32BSSize=pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32BitstreamBufSize;
            }
#else
            memCfg.u32BSAddr = cfg.u32BitstreamBufAddr;
            memCfg.u32BSSize = cfg.u32BitstreamBufSize;
#endif
            memCfg.u32DrvBufAddr = cfg.u32DrvProcBufAddr;
            memCfg.u32DrvBufSize = cfg.u32DrvProcBufSize;
            memCfg.bEnableDynScale = pInitParam->EnableDynaScale;
           // memCfg.bSupportSDModeOnly = !bHDmode;

           //the very first step: Get register base addr
           //notice: must do this first before accessing any register.
           MS_PHY u32NonPMBankSize;
           MS_VIRT u32NonPMBankAddr = 0;
           if (!MDrv_MMIO_GetBASE(&u32NonPMBankAddr, &u32NonPMBankSize, MS_MODULE_MVD))
           {
                _VDEC_EX_API_MutexLock();
                _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                _VDEC_EX_API_MutexUnlock();
               VDEC_PRINT("_MVD_Init: IOMap failure\n");
               pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_STOP;
               return E_VDEC_EX_FAIL;
           }
           else
           {
               MDrv_HVD_EX_SetOSRegBase(u32NonPMBankAddr);
               MDrv_MVD_RegSetBase(u32NonPMBankAddr);
               //VDEC_PRINT("_MVD_Init: u32RiuBaseAdd = %lx\n", u32NonPMBankAddr);
           }

            if(MDrv_MVD_SetCfg(u32Id, &fwCfg, &memCfg) != E_MVD_RET_OK)
            {
                _VDEC_EX_API_MutexLock();
                _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                _VDEC_EX_API_MutexUnlock();
                pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_STOP;
                return E_VDEC_EX_FAIL;
            }



            if (!MDrv_MVD_Init(u32Id,fwCfg.eCodecType))
            {
                _VDEC_EX_API_MutexLock();
                _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                _VDEC_EX_API_MutexUnlock();
                pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_STOP;
                return E_VDEC_EX_FAIL;
            }

            if(MDrv_MVD_SetCodecInfo(u32Id, eMvdCodecType, eMvdSrcMode, bMvdParserDisable[eMvdSrcMode]) == FALSE)
            {
                return E_VDEC_EX_FAIL;
            }

            if (eMvdSrcMode == E_MVD_TS_FILE_MODE)
            {
                MS_U32 u32bsBufUnderflowTH = ((((cfg.u32BitstreamBufSize*15)/16)>>3)<<3);
                MS_U32 u32bsBufOverflowTH  = (((cfg.u32BitstreamBufSize-0x4000)>>3)<<3); //16K
                MDrv_MVD_SetUnderflowTH(u32Id, u32bsBufUnderflowTH);
                MDrv_MVD_SetOverflowTH(u32Id, u32bsBufOverflowTH);
            }

            if (FALSE == MDrv_MVD_DropErrorFrame(u32Id, !pInitParam->bDisableDropErrFrame))
            {
                _VDEC_EX_API_MutexLock();
                _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                _VDEC_EX_API_MutexUnlock();
                pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_STOP;
                return E_VDEC_EX_FAIL;
            }
            pVdecExContext->bDropErrFrm[u8Idx] = (!pInitParam->bDisableDropErrFrame);

            if (FALSE == MDrv_MVD_DispRepeatField(u32Id, pInitParam->bRepeatLastField))
            {
                _VDEC_EX_API_MutexLock();
                _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                _VDEC_EX_API_MutexUnlock();
                pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_STOP;
                return E_VDEC_EX_FAIL;
            }

            MDrv_MVD_DisableErrConceal(u32Id, !pInitParam->bDisableErrConceal);
            /*
            E_MVD_Result ret = MDrv_MVD_DisableErrConceal(pInitParam->bDisableErrConceal);


            if (E_MVD_RET_OK != ret)
            {
                return E_VDEC_EX_FAIL;
            }
            */

            MDrv_MVD_SetVirtualBox(u32Id, pInitParam->u32DSVirtualBoxWidth, pInitParam->u32DSVirtualBoxHeight);

            if (eMvdSrcMode == E_MVD_SLQ_TBL_MODE || eMvdSrcMode == E_MVD_TS_FILE_MODE)
            {
                if (E_VDEC_EX_TIME_STAMP_DTS == pInitParam->VideoInfo.eTimeStampType)
                {
                    eSyncType = E_MVD_TIMESTAMP_DTS;
                }
                else if (E_VDEC_EX_TIME_STAMP_PTS == pInitParam->VideoInfo.eTimeStampType)
                {
                    eSyncType = E_MVD_TIMESTAMP_PTS;
                }
                else if (E_VDEC_EX_TIME_STAMP_DTS_MPEG_DIRECTV_SD == pInitParam->VideoInfo.eTimeStampType)
                {
                    eSyncType = E_MVD_TIMESTAMP_DTS_RVU;
                }
                else if (E_VDEC_EX_TIME_STAMP_PTS_MPEG_DIRECTV_SD == pInitParam->VideoInfo.eTimeStampType)
                {
                    eSyncType = E_MVD_TIMESTAMP_PTS_RVU;
                }
                else if (E_VDEC_EX_TIME_STAMP_STS == pInitParam->VideoInfo.eTimeStampType)
                {
                    eSyncType = E_MVD_TIMESTAMP_NEW_STS;
                    //eSyncType = E_MVD_TIMESTAMP_STS;
                }
                else if (E_VDEC_EX_TIME_STAMP_NONE == pInitParam->VideoInfo.eTimeStampType)
                {
                    eSyncType = E_MVD_TIMESTAMP_FREERUN;
                }
                else
                {
                    _VDEC_EX_API_MutexLock();
                    _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                    _VDEC_EX_API_MutexUnlock();
                    VDEC_PRINT("Invalid sync type %d for MVD\n", pInitParam->VideoInfo.eTimeStampType);
                    pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_STOP;
                    return E_VDEC_EX_FAIL;
                }
                if (FALSE == MDrv_MVD_SetFileModeAVSync(u32Id, eSyncType))
                {
                    _VDEC_EX_API_MutexLock();
                    _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                    _VDEC_EX_API_MutexUnlock();
                    pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_STOP;
                    return E_VDEC_EX_FAIL;
                }
            }

            if ((eMvdSrcMode == E_MVD_TS_FILE_MODE)||(eMvdSrcMode == E_MVD_SLQ_TBL_MODE))
            {  //set up MVD for MediaCodec playback
                _VDEC_EX_MVDCfgDivX(u32Id, pVdecExContext->_Attr[u8Idx].eCodecType);

                if (eMvdCodecType == E_MVD_CODEC_DIVX311)
                {
                    MVD_FrameInfo stFrminfo;
                    _VDEC_Memset(&stFrminfo, 0, sizeof(MVD_FrameInfo));
                    stFrminfo.u16HorSize = pInitParam->VideoInfo.u16Width[0];
                    stFrminfo.u16VerSize = pInitParam->VideoInfo.u16Height[0];
                    if(pInitParam->VideoInfo.u32FrameRateBase == 0)
                    {
                        VDEC_PRINT("Invalid FrameRate base!\n");
                        pInitParam->VideoInfo.u32FrameRateBase = 1;
                    }
#ifdef MSOS_TYPE_LINUX_KERNEL
                    MS_U64 u64Tmp = (MS_U64)pInitParam->VideoInfo.u32FrameRate * 1000;
                    do_div(u64Tmp, pInitParam->VideoInfo.u32FrameRateBase);
                    stFrminfo.u32FrameRate = (MS_U32)u64Tmp;
#else
                    stFrminfo.u32FrameRate =
                        (MS_U32)(((MS_U64)pInitParam->VideoInfo.u32FrameRate * 1000) / pInitParam->VideoInfo.u32FrameRateBase);
#endif
                    #if 0
                    VDEC_PRINT("==>DivX311 w=0x%x h=0x%x fr=0x%lx,0x%lx\n",
                        pInitParam->VideoInfo.u16Width[0],pInitParam->VideoInfo.u16Height[0],
                        pInitParam->VideoInfo.u32FrameRate, pInitParam->VideoInfo.u32FrameRateBase);
                    VDEC_PRINT("==>DivX311 w=0x%x h=0x%x fr=%ld\n",
                        stFrminfo.u16HorSize, stFrminfo.u16VerSize, stFrminfo.u32FrameRate);
                    #endif
                    MDrv_MVD_SetFrameInfo(u32Id, &stFrminfo);
                }
                else if (eMvdCodecType != E_MVD_CODEC_MPEG2)
                {
                    MVD_FrameInfo stFrminfo1;
                    _VDEC_Memset(&stFrminfo1, 0, sizeof(MVD_FrameInfo));
                    if(pInitParam->VideoInfo.u32FrameRateBase == 0)
                    {
                        VDEC_PRINT("Invalid FrameRate base\n");
                        pInitParam->VideoInfo.u32FrameRateBase = 1;
                    }
#ifdef MSOS_TYPE_LINUX_KERNEL
                    MS_U64 u64Tmp = (MS_U64)pInitParam->VideoInfo.u32FrameRate * 1000;
                    do_div(u64Tmp, pInitParam->VideoInfo.u32FrameRateBase);
                    stFrminfo1.u32FrameRate = (MS_U32)u64Tmp;
#else
                    stFrminfo1.u32FrameRate =
                        (MS_U32)(((MS_U64)pInitParam->VideoInfo.u32FrameRate * 1000) / pInitParam->VideoInfo.u32FrameRateBase);
#endif
                    MDrv_MVD_SetFrameInfo(u32Id, &stFrminfo1);
                }

                if (MDrv_MVD_SkipToIFrame(u32Id) != TRUE)
                {
                    _VDEC_EX_API_MutexLock();
                    _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                    _VDEC_EX_API_MutexUnlock();
                    pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_STOP;
                    return E_VDEC_EX_FAIL;
                }
            }
            //mvd Temp solution will remove when mvd ok in future
            if(pVdecExContext->_Pre_Ctrl[u8Idx].eDisplayMode != 0xFF)
            {
                if (pVdecExContext->_Pre_Ctrl[u8Idx].eDisplayMode == E_VDEC_EX_DISPLAY_MODE_MCU)
                {
                    pVdecExContext->u32FrameBaseMode[u8Idx] = TRUE;
                    MDrv_MVD_SetMStreamerMode(u32Id, TRUE);
					MDrv_MVD_SetMcuMode(u32Id, TRUE);
                }
                else if(pVdecExContext->_Pre_Ctrl[u8Idx].eDisplayMode == E_VDEC_EX_DISPLAY_MODE_HARDWIRE)
                {

                    pVdecExContext->u32FrameBaseMode[u8Idx] = FALSE;
                    MDrv_MVD_SetMStreamerMode(u32Id, FALSE);
					MDrv_MVD_SetMcuMode(u32Id, FALSE);
                }
            }

            if(pVdecExContext->_Pre_Ctrl[u8Idx].bCalFrameRate == TRUE)
            {
                if (MDrv_MVD_VariableFrameRate(u32Id) != TRUE)
                {
                    _VDEC_EX_API_MutexLock();
                    _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                    _VDEC_EX_API_MutexUnlock();
                    pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_STOP;
                    return E_VDEC_EX_FAIL;
                }
            }

            break;
        }
        case E_VDEC_EX_DECODER_HVD:
        _RET_VDEC_HVD_LINT_CHECK_AND_RETURN_MUTEX();
        {
            if (pVdecExContext->_Pre_Ctrl[u8Idx].bOnePendingBuffer)
            {
                MDrv_HVD_EX_OnePendingBufferMode(u32Id,pVdecExContext->_Pre_Ctrl[u8Idx].bOnePendingBuffer);
            }

            if (pVdecExContext->_Pre_Ctrl[u8Idx].bDisableTspInBbuMode)
            {
                MDrv_HVD_EX_TsInBbuMode(u32Id,pVdecExContext->_Pre_Ctrl[u8Idx].bDisableTspInBbuMode);
            }

            if (pVdecExContext->_Pre_Ctrl[u8Idx].stIapGnShBWMode.bEnable)
            {
                MDrv_HVD_EX_IapGnBufShareBWMode(u32Id,pVdecExContext->_Pre_Ctrl[u8Idx].stIapGnShBWMode.bEnable,
                    pVdecExContext->_Pre_Ctrl[u8Idx].stIapGnShBWMode.u32IapGnBufAddr, pVdecExContext->_Pre_Ctrl[u8Idx].stIapGnShBWMode.u32IapGnBufSize);
            }

            if (pVdecExContext->_Pre_Ctrl[u8Idx].bCalFrameRate)
            {
                MDrv_HVD_EX_SetCalFrameRate(u32Id,pVdecExContext->_Pre_Ctrl[u8Idx].bCalFrameRate);
            }

            MDrv_HVD_EX_PreSetMFCodecMode(u32Id, _VDEC_Map2HVDMFCodecMode(pVdecExContext->_Pre_Ctrl[u8Idx].eMFCodecMode));
            if(pVdecExContext->_Pre_Ctrl[u8Idx].eDisplayMode != 0xFF)
            {
                MDrv_HVD_EX_PreSetDisplayMode(u32Id, _VDEC_Map2HVDDispMode(pVdecExContext->_Pre_Ctrl[u8Idx].eDisplayMode));
            }
            MDrv_HVD_EX_PreSetForce8BitMode(u32Id, pVdecExContext->_Pre_Ctrl[u8Idx].bForce8bitMode);
            MDrv_HVD_EX_PreSetVdecFeature(u32Id, pVdecExContext->_Pre_Ctrl[u8Idx].eVdecFeature);
#ifdef VDEC3
            VDEC_EX_SysCfg cfg = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig;
#else
            VDEC_EX_SysCfg cfg = pInitParam->SysConfig;
#endif
            HVD_EX_MemCfg stMemCfg;
            HVD_EX_InitSettings stInitSettings;
            HVD_EX_RVInfo stRVInfo;

            _VDEC_Memset(&stMemCfg, 0, sizeof(HVD_EX_MemCfg));
            _VDEC_Memset(&stInitSettings, 0, sizeof(HVD_EX_InitSettings));
            _VDEC_Memset(&stRVInfo, 0, sizeof(HVD_EX_RVInfo));

            stInitSettings.pRVFileInfo = &stRVInfo;

            stMemCfg.eFWSourceType = (HVD_EX_FWSourceType) cfg.eFWSourceType;
            stMemCfg.u32FWBinaryAddr = (MS_PHY) cfg.u32FWBinaryAddr;

            if ((E_HVD_EX_FW_SOURCE_DRAM == stMemCfg.eFWSourceType)
             || (E_HVD_EX_FW_SOURCE_FLASH == stMemCfg.eFWSourceType))
            {
                stMemCfg.u32FWBinaryVAddr = MS_PA2KSEG1((MS_VIRT) cfg.u32FWBinaryAddr);
            }

            stMemCfg.u32FWBinarySize = cfg.u32FWBinarySize;
            stMemCfg.u32VLCBinaryAddr = (MS_PHY) cfg.u32VlcBinarySrcAddr;

            if ((E_HVD_EX_FW_SOURCE_DRAM == stMemCfg.eFWSourceType)
             || (E_HVD_EX_FW_SOURCE_FLASH == stMemCfg.eFWSourceType))
            {
                stMemCfg.u32VLCBinaryVAddr = MS_PA2KSEG1((MS_VIRT) cfg.u32VlcBinarySrcAddr);
            }

            stMemCfg.u32VLCBinarySize       = cfg.u32VlcTabBinarySize;
            stMemCfg.u32CodeBufAddr         = (MS_PHY) cfg.u32CodeBufAddr;
            stMemCfg.u32CodeBufVAddr        = MS_PA2KSEG1((MS_VIRT) cfg.u32CodeBufAddr);
            stMemCfg.u32CodeBufSize         = cfg.u32CodeBufSize;
            stMemCfg.u32FrameBufAddr        = (MS_PHY) cfg.u32FrameBufAddr;
            stMemCfg.u32FrameBufVAddr       = MS_PA2KSEG1((MS_VIRT) cfg.u32FrameBufAddr);
            stMemCfg.u32FrameBufSize        = cfg.u32FrameBufSize;
            stMemCfg.u32BitstreamBufAddr    = (MS_PHY) cfg.u32BitstreamBufAddr;
            stMemCfg.u32BitstreamBufVAddr   = MS_PA2KSEG1((MS_VIRT) cfg.u32BitstreamBufAddr);
            stMemCfg.u32BitstreamBufSize    = cfg.u32BitstreamBufSize;
            stMemCfg.u32DrvProcessBufAddr   = (MS_PHY) cfg.u32DrvProcBufAddr;
            stMemCfg.u32DrvProcessBufVAddr  = MS_PA2KSEG1((MS_VIRT) cfg.u32DrvProcBufAddr);
            stMemCfg.u32DrvProcessBufSize   = cfg.u32DrvProcBufSize;
#ifdef VDEC3
            if (pVdecExContext->bDRVdecideBS)
            {
                stMemCfg.u32TotalBitstreamBufAddr = pVdecExContext->u32BitstreamBufAddr;
                stMemCfg.u32TotalBitstreamBufSize = pVdecExContext->u32BitstreamBufSize;
            }
            else
            {
                stMemCfg.u32TotalBitstreamBufAddr = cfg.u32BitstreamBufAddr;
                stMemCfg.u32TotalBitstreamBufSize = cfg.u32BitstreamBufSize;
            }
#endif

            // set codec flag
            if (E_VDEC_EX_CODEC_TYPE_H264 == pVdecExContext->_Attr[u8Idx].eCodecType)
            {
                stInitSettings.u32ModeFlag |= HVD_INIT_HW_AVC;
            }
            else if (E_VDEC_EX_CODEC_TYPE_AVS == pVdecExContext->_Attr[u8Idx].eCodecType)
            {
                stInitSettings.u32ModeFlag |= HVD_INIT_HW_AVS;
            }
            else if (E_VDEC_EX_CODEC_TYPE_RV8 == pVdecExContext->_Attr[u8Idx].eCodecType
                || E_VDEC_EX_CODEC_TYPE_RV9 == pVdecExContext->_Attr[u8Idx].eCodecType)
            {
                stInitSettings.u32ModeFlag |= HVD_INIT_HW_RM;
            }
            else if (E_VDEC_EX_CODEC_TYPE_MVC == pVdecExContext->_Attr[u8Idx].eCodecType)
            {
                stInitSettings.u32ModeFlag |= HVD_INIT_HW_MVC;
            }
            else if (E_VDEC_EX_CODEC_TYPE_VP8 == pVdecExContext->_Attr[u8Idx].eCodecType)
            {
                stInitSettings.u32ModeFlag |= HVD_INIT_HW_VP8;
            }
            else if (E_VDEC_EX_CODEC_TYPE_HEVC == pVdecExContext->_Attr[u8Idx].eCodecType)
            {
                stInitSettings.u32ModeFlag |= HVD_INIT_HW_HEVC;
            }
            else if (E_VDEC_EX_CODEC_TYPE_VP9 == pVdecExContext->_Attr[u8Idx].eCodecType)
            {
                stInitSettings.u32ModeFlag |= HVD_INIT_HW_VP9;
            }

            // set input source mode
            if (E_VDEC_EX_SRC_MODE_DTV == pVdecExContext->_Attr[u8Idx].eSrcMode)
            {
                stInitSettings.u32ModeFlag |= HVD_INIT_MAIN_LIVE_STREAM;
                stInitSettings.u32ModeFlag |= HVD_INIT_INPUT_TSP;
                stInitSettings.u8SyncType = E_HVD_EX_SYNC_ATS;
            }
            else if ((E_VDEC_EX_SRC_MODE_TS_FILE == pVdecExContext->_Attr[u8Idx].eSrcMode) ||
             (E_VDEC_EX_SRC_MODE_TS_FILE_DUAL_ES == pVdecExContext->_Attr[u8Idx].eSrcMode) )
            {
                stInitSettings.u32ModeFlag |= HVD_INIT_MAIN_FILE_TS;
                stInitSettings.u32ModeFlag |= HVD_INIT_INPUT_TSP;
                stInitSettings.u8SyncType = E_HVD_EX_SYNC_ATS;
                if(E_VDEC_EX_SRC_MODE_TS_FILE_DUAL_ES == pVdecExContext->_Attr[u8Idx].eSrcMode)
                {
                    stInitSettings.u32ModeFlag |= HVD_INIT_DUAL_ES_ENABLE;
                }
                else
                {
                    stInitSettings.u32ModeFlag |= HVD_INIT_DUAL_ES_DISABLE;
                }
            }
            else if ((E_VDEC_EX_SRC_MODE_FILE == pVdecExContext->_Attr[u8Idx].eSrcMode) ||
              (E_VDEC_EX_SRC_MODE_FILE_DUAL_ES== pVdecExContext->_Attr[u8Idx].eSrcMode) )
            {
                stInitSettings.u32ModeFlag |= HVD_INIT_MAIN_FILE_RAW;
                stInitSettings.u32ModeFlag |= HVD_INIT_INPUT_DRV;
                if(E_VDEC_EX_SRC_MODE_FILE_DUAL_ES == pVdecExContext->_Attr[u8Idx].eSrcMode)
                {
                    stInitSettings.u32ModeFlag |= HVD_INIT_DUAL_ES_ENABLE;
                }
                else
                {
                    stInitSettings.u32ModeFlag |= HVD_INIT_DUAL_ES_DISABLE;
                }

                if (E_VDEC_EX_TIME_STAMP_NONE == pInitParam->VideoInfo.eTimeStampType)
                {
                    stInitSettings.u8SyncType = E_HVD_EX_SYNC_ATS;
                }
                else if (E_VDEC_EX_TIME_STAMP_STS == pInitParam->VideoInfo.eTimeStampType)
                {
                    stInitSettings.u8SyncType = E_HVD_EX_SYNC_STS;
                }
                else if (E_VDEC_EX_TIME_STAMP_DTS == pInitParam->VideoInfo.eTimeStampType)
                {
                    stInitSettings.u8SyncType = E_HVD_EX_SYNC_DTS;
                }
                else if (E_VDEC_EX_TIME_STAMP_PTS == pInitParam->VideoInfo.eTimeStampType)
                {
                    stInitSettings.u8SyncType = E_HVD_EX_SYNC_PTS;
                }
                else
                {
                    _VDEC_EX_API_MutexLock();
                    _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                    _VDEC_EX_API_MutexUnlock();
                    VDEC_PRINT("Invalid sync type %d\n", pInitParam->VideoInfo.eTimeStampType);
                    return E_VDEC_EX_FAIL;
                }
            }

            // set start code exist
            if (pInitParam->VideoInfo.bWithoutNalStCode)
            {
                stInitSettings.u32ModeFlag |= HVD_INIT_START_CODE_REMOVED;
            }
            else
            {
                stInitSettings.u32ModeFlag |= HVD_INIT_START_CODE_REMAINED;
            }

            // pro settings
            stInitSettings.u32FrameRate       = pInitParam->VideoInfo.u32FrameRate ;
            stInitSettings.u32FrameRateBase   = pInitParam->VideoInfo.u32FrameRateBase;

            if (E_VDEC_EX_SRC_MODE_FILE == pVdecExContext->_Attr[u8Idx].eSrcMode)
            {
                stInitSettings.u8MinFrmGap = 2; // default set
            }
            else
            {
                stInitSettings.u8MinFrmGap = 0; // default set
            }

            stInitSettings.bAutoPowerSaving = TRUE;
            stInitSettings.bFastDisplay = TRUE;
            stInitSettings.u8TimeUnit = 1;    // ms
            stInitSettings.bDynamicScaling = pInitParam->EnableDynaScale;

            if (bNotReloadFW)
            {
                stInitSettings.u8TurboInit |= E_HVD_EX_TURBOINIT_FW_RELOAD; //set for not load fw more than once
            }

            // for RM
            if (E_VDEC_EX_CODEC_TYPE_RV8 == pInitParam->eCodecType)
            {
                stRVInfo.RV_Version = 0;
            }
            else if (E_VDEC_EX_CODEC_TYPE_RV9 == pInitParam->eCodecType)
            {
                stRVInfo.RV_Version = 1;
            }

            stRVInfo.ulNumSizes = pInitParam->VideoInfo.u16NumSizes;

            _VDEC_Memcpy(stRVInfo.ulPicSizes_w, pInitParam->VideoInfo.u16Width, sizeof(pInitParam->VideoInfo.u16Width));
            _VDEC_Memcpy(stRVInfo.ulPicSizes_h, pInitParam->VideoInfo.u16Height, sizeof(pInitParam->VideoInfo.u16Height));

            {
                MS_PHY u32NonPMBankSize = 0;
                MS_VIRT u32RiuBaseAddr = 0;

                if (!MDrv_MMIO_GetBASE(&u32RiuBaseAddr, &u32NonPMBankSize, MS_MODULE_HW))
                {
                    _VDEC_EX_API_MutexLock();
                    _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                    _VDEC_EX_API_MutexUnlock();
                    VDEC_PRINT("VDEC HVD Init Err: MMIO_GetBASE failure\n");
                    return E_VDEC_EX_FAIL;
                }
                else
                {
                    MDrv_HVD_EX_SetOSRegBase(u32RiuBaseAddr);
                    MDrv_MVD_RegSetBase(u32RiuBaseAddr);
                }
            }

#ifdef VDEC3
            if(MDrv_HVD_EX_Init(u32Id, &stMemCfg, &stInitSettings, pVdecExContext->bFWdecideFB, pVdecExContext->bDRVdecideBS) != E_HVD_EX_OK)
#else
            if(MDrv_HVD_EX_Init(u32Id, &stMemCfg, &stInitSettings) != E_HVD_EX_OK)
#endif
            {
                _VDEC_EX_API_MutexLock();
                _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                _VDEC_EX_API_MutexUnlock();
                return E_VDEC_EX_FAIL;
            }

            _VDEC_EX_Create_BBU_Task(pStreamId->u32Id);

            #if VDEC_DTV_DIRECT_STC_PATCH
            if ((u8Cidx == 1) && (E_VDEC_EX_SRC_MODE_DTV == pVdecExContext->_Attr[u8Idx].eSrcMode))
            {
                if (FALSE == _VDEC_EX_Create_DirectSTC_Task(pStreamId->u32Id))
                {
                    _VDEC_EX_API_MutexLock();
                    _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                    _VDEC_EX_API_MutexUnlock();
                    return E_VDEC_EX_FAIL;
                }
            }
            #endif

            // SPS filter
            if (E_VDEC_EX_SRC_MODE_DTV == pVdecExContext->_Attr[u8Idx].eSrcMode)
            {
                HVD_EX_DispInfoThreshold  DispInfoTH;
                DispInfoTH.u32FrmrateLowBound=0;
                DispInfoTH.u32FrmrateUpBound=0;
                DispInfoTH.u32MvopLowBound=16000000;
                DispInfoTH.u32MvopUpBound=0;
                MDrv_HVD_EX_SetDispInfoTH(u32Id, &DispInfoTH);
            }

            if (pInitParam->EnableDynaScale)
            {
                if(MDrv_HVD_EX_SetVirtualBox(u32Id,
                         pInitParam->u32DSVirtualBoxWidth,
                         pInitParam->u32DSVirtualBoxHeight) != E_HVD_EX_OK)
                {
                    _VDEC_EX_API_MutexLock();
                    _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                    _VDEC_EX_API_MutexUnlock();
                    return E_VDEC_EX_FAIL;
                }
            }

            if(MDrv_HVD_EX_SetDispErrFrm(u32Id, pInitParam->bDisableDropErrFrame) != E_HVD_EX_OK)
            {
                _VDEC_EX_API_MutexLock();
                _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                _VDEC_EX_API_MutexUnlock();
                return E_VDEC_EX_FAIL;
            }

            if(MDrv_HVD_EX_SetErrConceal(u32Id, !pInitParam->bDisableErrConceal) != E_HVD_EX_OK)
            {
                _VDEC_EX_API_MutexLock();
                _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                _VDEC_EX_API_MutexUnlock();
                return E_VDEC_EX_FAIL;
            }

            break;
        }
        case E_VDEC_EX_DECODER_MJPEG:
        {
            _VDEC_EX_API_MutexLock();

#ifdef VDEC3
            VDEC_EX_SysCfg cfg = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig;
#else
            VDEC_EX_SysCfg cfg = pInitParam->SysConfig;
#endif
            MJPEG_INIT_PARAM initParam;
            pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_INIT;
            _VDEC_Memset(&initParam, 0, sizeof(MJPEG_INIT_PARAM));

            initParam.u32FwBinAddr = cfg.u32FWBinaryAddr;
            initParam.u32FwBinSize = cfg.u32FWBinarySize;
            initParam.u32FwAddr = cfg.u32CodeBufAddr;
            initParam.u32FwSize = cfg.u32CodeBufSize;
            initParam.u32MRCBufAddr = cfg.u32BitstreamBufAddr;
            initParam.u32MRCBufSize = cfg.u32BitstreamBufSize;
            initParam.u32MWCBufAddr = cfg.u32FrameBufAddr;
            initParam.u32MWCBufSize = cfg.u32FrameBufSize;
            initParam.u32InternalBufAddr = cfg.u32DrvProcBufAddr;
            initParam.u32InternalBufSize = cfg.u32DrvProcBufSize;
            initParam.u32FrameRate = pInitParam->VideoInfo.u32FrameRate;
            initParam.u32FrameRateBase= pInitParam->VideoInfo.u32FrameRateBase;
            initParam.eFwSrcType = (MJPEG_FWSourceType)cfg.eFWSourceType;
            initParam.u32Width = (MS_U32)pInitParam->VideoInfo.u16Width[0];
            initParam.u32Height= (MS_U32)pInitParam->VideoInfo.u16Height[0];

            MApi_MJPEG_EnableTurboFWMode(u32Id, bNotReloadFW);

            // the very first step
            {
                MS_PHY u32NonPMBankSize = 0;
                MS_VIRT u32RiuBaseAddr = 0;

                if (!MDrv_MMIO_GetBASE(&u32RiuBaseAddr, &u32NonPMBankSize, MS_MODULE_HW))
                {
                    _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                    VDEC_PRINT("VDEC MJPEG Init Err: MMIO_GetBASE failure\n");
                    _VDEC_EX_API_MutexUnlock();
                    pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_STOP;
                    return E_VDEC_EX_FAIL;
                }
                else
                {
                    MDrv_HVD_EX_SetOSRegBase(u32RiuBaseAddr);
                    MDrv_MVD_RegSetBase(u32RiuBaseAddr);
                }
            }

            MDrv_HVD_EX_SetCtrlsBase(u32Id);
            MDrv_HVD_EX_MJPEG_InitSharemem(u32Id, MS_PA2KSEG1((MS_VIRT) cfg.u32CodeBufAddr));
#ifdef VDEC3
            if(MApi_MJPEG_Init(u32Id, &initParam, pVdecExContext->bFWdecideFB) != E_MJPEG_RET_SUCCESS)
#else
            if(MApi_MJPEG_Init(u32Id, &initParam) != E_MJPEG_RET_SUCCESS)
#endif
            {
                _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                _VDEC_EX_API_MutexUnlock();
                pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_STOP;
                return E_VDEC_EX_FAIL;
            }
            MDrv_HVD_EX_SetBurstMode(u32Id, TRUE);
			//Mjpeg Temp solution will remove in future
            if(pVdecExContext->_Pre_Ctrl[u8Idx].eDisplayMode != 0xFF)
            {
                if(pVdecExContext->_Pre_Ctrl[u8Idx].eDisplayMode == E_VDEC_EX_DISPLAY_MODE_MCU)
                {
                   MApi_MJPEG_EnableDispCmdQ(u32Id, TRUE);
                   MApi_MJPEG_SetControl(u32Id, E_MJPEG_VDEC_CMD_SET_DISPLAY_OUTSIDE_MODE, (MS_U32)TRUE);
                }
                else if(pVdecExContext->_Pre_Ctrl[u8Idx].eDisplayMode == E_VDEC_EX_DISPLAY_MODE_HARDWIRE)
                {
                   MApi_MJPEG_EnableDispCmdQ(u32Id, FALSE);
                   MApi_MJPEG_SetControl(u32Id, E_MJPEG_VDEC_CMD_SET_DISPLAY_OUTSIDE_MODE, (MS_U32)FALSE);
                }
            }
            _VDEC_EX_API_MutexUnlock();

            break;
        }
        default:
        {
            if(pVdecExContext->_Attr[u8Idx].eDbgMsgLevel >= E_VDEC_EX_DBG_LEVEL_ERR)
            {
                VDEC_PRINT("VDEC_EX Err(%d): Decoder is not supported!!\n", __LINE__);
            }
            _VDEC_EX_API_MutexLock();
            _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
            _VDEC_EX_API_MutexUnlock();
            return E_VDEC_EX_RET_INVALID_PARAM;
        }
    }

    pVdecExContext->_bEsBufMgmt[u8Idx] = FALSE;
    pVdecExContext->_Attr[u8Idx].bInit = TRUE;
    pVdecExContext->_Attr[u8Idx].bUsed = TRUE;
    pVdecExContext->_Attr[u8Idx].u32DrvId = u32Id;
    pVdecExContext->bFlush[u8Idx] = TRUE;
    pVdecExContext->support_cc[u8Idx] = TRUE;
    #ifdef _MVD_EVENT_DBG
    _VDEC_Memcpy(&stTestStreamId, pStreamId, sizeof(VDEC_StreamId));
    _VDEC_MVDEventDbg();
    #endif

    #ifdef _HVD_EVENT_DBG
    _VDEC_Memcpy(&stTestStreamId, pStreamId, sizeof(VDEC_StreamId));
    _VDEC_HVDEventDbg();
    #endif

    if(bNDecAllocator == FALSE)
    {
#ifdef CMA_DRV_DIRECT_INIT
        // Allocate the second miu if HEVC and VP9
        if (pVdecExContext->bCMAUsed && bCMAInitPool[1] && u8Idx == 0  &&
            ((pVdecExContext->_Attr[u8Idx].eCodecType == E_VDEC_EX_CODEC_TYPE_HEVC)||
            (pVdecExContext->_Attr[u8Idx].eCodecType == E_VDEC_EX_CODEC_TYPE_VP9)) &&
             pVdecExContext->_Attr[u8Idx].eSrcMode != E_VDEC_EX_SRC_MODE_DTV &&
             (pVdecExContext->_power_state[u8Idx] != E_VDEC_EX_POWER_RESUME_ING)
            )
        {
            MS_VIRT u32FrmBuffAddr;
            MS_U32 u32FrmBuffSize;

            struct CMA_Pool_Alloc_Param alloc_param;

            alloc_param.pool_handle_id = pVdecExContext->cmaInitParam[1].pool_handle_id;
            alloc_param.flags = CMA_FLAG_VIRT_ADDR;
            alloc_param.offset_in_pool = 0;
            alloc_param.length = pVdecExContext->cmaInitParam[1].heap_length;

            if(pVdecExContext->bCMAGetMem[1][u8Idx] == FALSE)
            {
                pVdecExContext->bCMAGetMem[1][u8Idx] = TRUE;
                if (MApi_CMA_Pool_GetMem(&alloc_param) == FALSE)
                {
                    pVdecExContext->bCMAGetMem[1][u8Idx] = FALSE;
                    _VDEC_EX_API_MutexLock();
                    _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                    _VDEC_EX_API_MutexUnlock();
                    VPRINTF("[%s][%d]CMA GET fail\n",__FUNCTION__,__LINE__);
                    pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_STOP;
                    return E_VDEC_EX_FAIL;
                }
                else
                {
                    VPRINTF("[VDEC][%d]MApi_CMA_Pool_GetMem1: alloc_param.pool_handle_id=%x, alloc_param.flags=%x, alloc_param.offset_in_pool=%llx, alloc_param.length=%x\n",
                                        (unsigned int)u8Idx,
                                        (unsigned int)alloc_param.pool_handle_id,
                                        (unsigned int)alloc_param.flags,
                                        (unsigned long long int)alloc_param.offset_in_pool,
                                        (unsigned int)alloc_param.length);
                }
            }

            pVdecExContext->cmaFreeParam[1][u8Idx].pool_handle_id = pVdecExContext->cmaInitParam[1].pool_handle_id;
            pVdecExContext->cmaFreeParam[1][u8Idx].offset_in_pool = alloc_param.offset_in_pool;
            pVdecExContext->cmaFreeParam[1][u8Idx].length = alloc_param.length;

            _miu_offset_to_phy(pVdecExContext->cmaInitParam[1].miu,
                               pVdecExContext->cmaInitParam[1].heap_miu_start_offset + alloc_param.offset_in_pool,
                               u32FrmBuffAddr);

            u32FrmBuffSize = alloc_param.length;

            _MApi_VDEC_EX_SetFrameBuff2(pStreamId, u32FrmBuffAddr, u32FrmBuffSize);
        }
#endif
    }
    else
    {
#if VDEC_EX_ALLOCATOR_ENABLE_NDECODE_FB2
        if (pVdecExContext->bCMAUsed && (u8Idx == 0 || pVdecExContext->bDRVdecideBS == TRUE) &&
            ((pVdecExContext->_Attr[u8Idx].eCodecType == E_VDEC_EX_CODEC_TYPE_HEVC)||
             (pVdecExContext->_Attr[u8Idx].eCodecType == E_VDEC_EX_CODEC_TYPE_VP9)/*||
             (pVdecExContext->_Attr[u8Idx].eCodecType == E_VDEC_EX_CODEC_TYPE_HEVC_DV)*/) &&
            pVdecExContext->_Attr[u8Idx].eSrcMode != E_VDEC_EX_SRC_MODE_DTV)
        {
            VDEC_EX_Allocator_Block *pstBlockFB2 = NULL;
            VDEC_EX_Result eRet;
            VDEC_EX_ALLOCATOR_BUFFER_TYPE eBufType;
            VDEC_EX_NDecBufRangeParam stBufRgnFB2;

            _VDEC_EX_API_MutexLock();

            // frame Buffer 2 allocating setting

            eBufType = E_VDEC_EX_ALLOCATOR_BUFFER_FB2;

            _VDEC_Memset(&stBufRgnFB2, 0, sizeof(VDEC_EX_NDecBufRangeParam));
            if(pVdecExContext->_Pre_Ctrl[u8Idx].stTotalBufRgnFB2.bSetTotalBuf == TRUE)
            {
                stBufRgnFB2.bSetTotalBuf = TRUE;
                stBufRgnFB2.phyTotalBufAddr = pVdecExContext->_Pre_Ctrl[u8Idx].stTotalBufRgnFB2.phyTotalBufAddr;
                stBufRgnFB2.szTotalBufSize = pVdecExContext->_Pre_Ctrl[u8Idx].stTotalBufRgnFB2.szTotalBufSize;
            }
            stBufRgnFB2.phyBufAddr = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufAddr;
            stBufRgnFB2.szBufSize = pVdecExContext->_Attr[u8Idx].vdecExInitParam.SysConfig.u32FrameBufSize;

            // start allocating frame buffer 2

            eRet = _VDEC_EX_NdecAllocator_MallocFrameBuffer(u8Idx, eBufType, &stBufRgnFB2, &pstBlockFB2);
            if(eRet != E_VDEC_EX_OK)
            {
                _MApi_VDEC_EX_ReturnCMABuffer(pStreamId);
                ALLOCATOR_PRINTF_ERR("[ALLOC][ERR][%s][%d][%d] Alloc Frame Buffer2 Fail\n", __FUNCTION__, __LINE__, u8Idx);
                pVdecExContext->_Attr[u8Idx].eStage = E_VDEC_EX_STAGE_STOP;
                _VDEC_EX_API_MutexUnlock();
                return E_VDEC_EX_FAIL;
            }

            _VDEC_EX_API_MutexUnlock();

            VPRINTF("[ALLOC][DBG][%s][%d][%d] Alloc Frame Buffer2: Addr=0x%llx, Size=0x%x\n",
                __FUNCTION__, __LINE__, u8Idx, pstBlockFB2->stBlockInfo.phyAddr, pstBlockFB2->stBlockInfo.szSize);

            _MApi_VDEC_EX_SetFrameBuff2(pStreamId, pstBlockFB2->stBlockInfo.phyAddr, pstBlockFB2->stBlockInfo.szSize);
        }
#endif
    }

    return E_VDEC_EX_OK;
}



VDEC_EX_Result MApi_VDEC_EX_SetControl(VDEC_StreamId *pStreamId, VDEC_EX_User_Cmd cmd_id, MS_VIRT param)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }

    if(cmd_id == E_VDEC_EX_USER_CMD_SET_DECODE_MODE)
    {
        IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
        IO_arg.eUserCmd = E_VDEC_EX_V2_USER_CMD_PRESET_DECODE_MODE;
        IO_arg.param[0] = (void*)(&param);
        IO_arg.pRet     = (void*)(&ret);
        if(UtopiaIoctl(pu32VDEC_EX_Inst,E_VDEC_EX_V2_CMD_PRE_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
        {
            VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
            return E_VDEC_EX_FAIL;
        }
        else
        {
            return ret;
        }
    }
    else
    {
        IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
        IO_arg.eUserCmd = _VDEC_MapUserCmd_V2(cmd_id);
        IO_arg.param[0] = (void*)(&param);
        IO_arg.pRet     = (void*)(&ret);
        if(UtopiaIoctl(pu32VDEC_EX_Inst,E_VDEC_EX_V2_CMD_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
        {
            VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
            return E_VDEC_EX_FAIL;
        }
        else
        {
            return ret;
        }
    }
}

VDEC_EX_Result MApi_VDEC_EX_V2_SetControl(VDEC_StreamId* pStreamId, VDEC_EX_V2_User_Cmd cmd_id, void** param,MS_BOOL IsResume)
{
    VDEC_EX_Result ret = E_VDEC_EX_OK;
    MS_U8  u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
    MS_U32 u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
    VDEC_EX_RESUME_SET_CMD resume_data;
    //these cmd should receive between vdec init and vdec exit

    _VDEC_INVALID_IDX_RET(u8Idx);
    _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);

    if(cmd_id == E_VDEC_EX_V2_USER_CMD_EXIT)
    {
        MDrv_MVD_ReleaseFreeStream(u32Id);
    }

    _VDEC_NOT_INIT_RET(u8Idx);
    _VDEC_INVALID_DRVID_RET(u8Idx, u32Id);

    switch (cmd_id)
    {
        case E_VDEC_EX_V2_USER_CMD_REPEAT_LAST_FIELD:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_RepeatLastField(pStreamId,
                                               (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_RepeatLastField(pStreamId,(MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num=1;
                resume_data.structArg_num=0;
                //resume_data.struct_size=0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_AVSYNC_REPEAT_TH:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_SetAVSyncRepeatThreshold(pStreamId,
                                                        (MS_U32)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_SetAVSyncRepeatThreshold(pStreamId, (MS_U32)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_DISP_ONE_FIELD:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_DispOneField(pStreamId,
                                           (MS_U32)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_DispOneField(pStreamId, (MS_U32)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_FD_MASK_DELAY_COUNT:
            ret = _VDEC_EX_SetFdMaskDelayCnt(pStreamId, (MS_U32)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_FRC_OUTPUT:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_FrameRateConvert(pStreamId,&(pVdecExContext->store_FRC_OutputParam[u8Idx]));
                //free(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].pStructArg);
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_FrameRateConvert(pStreamId, (VDEC_EX_FRC_OutputParam*)(*((MS_VIRT*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,0,1,sizeof(VDEC_EX_FRC_OutputParam),0,1,2,3,0);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 0;
                resume_data.structArg_num = 1;
               // resume_data.struct_size = sizeof(VDEC_EX_FRC_OutputParam);
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 0;
                _VDEC_Memcpy((void*)(&(pVdecExContext->store_FRC_OutputParam[u8Idx])),
                             param[0],
                             sizeof(VDEC_EX_FRC_OutputParam));
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_FRC_DROP_TYPE:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_SetFRCDropType(pStreamId,
                                             (MS_U32)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_SetFRCDropType(pStreamId, (MS_U32)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_FAST_DISPLAY:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_SetFastDisplay(pStreamId,
                                             (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_SetFastDisplay(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_IGNORE_ERR_REF:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_SetIgnoreErrRef(pStreamId,
                                              (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_SetIgnoreErrRef(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_FORCE_FOLLOW_DTV_SPEC:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_ForceFollowDTVSpec(pStreamId,
                                                 (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_ForceFollowDTVSpec(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_AVC_MIN_FRM_GAP:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_AvcMinFrameGap(pStreamId,
                                             (MS_U32)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_AvcMinFrameGap(pStreamId, (MS_U32)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_DISABLE_SEQ_CHG:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_SetDisableSeqChange(pStreamId,
                                                  (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_SetDisableSeqChange(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_AVC_DISABLE_ANTI_VDEAD:
            //_HVD_RET_HANDLE(MDrv_HVD_EX_SetSettings_Pro(u32Id, E_HVD_SSET_DISABLE_ANTI_VDEAD, *((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_DTV_RESET_MVD_PARSER:
            ret = _VDEC_EX_ResetMvdParser(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_PVR_FLUSH_FRAME_BUFFER://PVR used,only used in S company
            //return _VDEC_DoFlushFrmBuffer(pStreamId, *((MS_BOOL*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_FORCE_INTERLACE_MODE:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_ForceInterlaceMode(pStreamId,
                                                  (MS_U32)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_ForceInterlaceMode(pStreamId, (MS_U32)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
#if VDEC_ENABLE_MVC
        case E_VDEC_EX_V2_USER_CMD_MVC_BBU2_PUSH_PACKET:
            ret = _MApi_VDEC_EX_MVC_SetControl(pStreamId, E_VDEC_EX_MVC_SET_BBU2_PUSH_PACKET, param);
            break;
        case E_VDEC_EX_V2_USER_CMD_MVC_BBU2_FIRE_DECCMD:
            ret = _MApi_VDEC_EX_MVC_SetControl(pStreamId, E_VDEC_EX_MVC_SET_BBU2_FIRE_DECCMD, param);
            break;
#endif
        case E_VDEC_EX_V2_USER_CMD_SET_DISP_OUTSIDE_CTRL_MODE:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_DISP_OUTSIDE_MODE(pStreamId,
                                                (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_DISP_OUTSIDE_MODE(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_DTV_USER_DATA_MODE:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_SetDTVUserDataMode(pStreamId,
                                                  (MS_U32)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_SetDTVUserDataMode(pStreamId, (MS_U32)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_RELEASE_FD_MASK:
            ret = _VDEC_EX_ReleaseFdMask(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_FLUSH_PTS_BUF:
            ret = _VDEC_EX_FlushPTSBuf(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_SUPPORT_AVC_TO_MVC:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_Support_AVC2MVC(pStreamId,
                                              (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_Support_AVC2MVC(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_3DLR_VIEW_EXCHANGE:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_3DLR_View_Excahnge(pStreamId,
                                              (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_3DLR_View_Excahnge(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_AVC_NEW_SLOW_MOTION:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_AVC_Enable_New_Slow_Motion(pStreamId,
                                                         (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_AVC_Enable_New_Slow_Motion(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_VSIZE_ALIGN:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_SetVSizeAlign(pStreamId,
                                            (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_SetVSizeAlign(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_SHOW_DECODE_ORDER:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_ShowDecodeOrder(pStreamId,
                                              (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_ShowDecodeOrder(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_AVC_DISP_IGNORE_CROP:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_Disp_Ignore_Crop(pStreamId,
                                               (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_Disp_Ignore_Crop(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_SUSPEND_DYNAMIC_SCALE:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_SuspendDynamicScale(pStreamId,
                                                  (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_SuspendDynamicScale(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_FORCE_AUTO_MUTE:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_ForceAutoMute(pStreamId,
                                            (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_ForceAutoMute(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_DISP_FINISH_MODE:
            ret = _VDEC_EX_SetDispFinishMode(pStreamId, (MS_U8)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_AVSYNC_MODE:
            ret = _VDEC_EX_SetAVSyncMode(pStreamId, (MS_U8)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_PUSH_DISPQ_WITH_REF_NUM:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_PushDispQWithRefNum(pStreamId,
                                                   (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_PushDispQWithRefNum(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_DS_RESV_N_BUFFER:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_DynamicScalingResvNBuffer(pStreamId,
                                                        (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_DynamicScalingResvNBuffer(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_UT_SET_DBG_MODE:
            ret = _VDEC_EX_DbgMode(pStreamId,(VDEC_EX_DbgMode)(*((MS_U32*)(param[0]))), TRUE);
            break;
        case E_VDEC_EX_V2_USER_CMD_UT_CLR_DBG_MODE:
            ret = _VDEC_EX_DbgMode(pStreamId, (VDEC_EX_DbgMode)(*((MS_U32*)(param[0]))), FALSE);
            break;
        case E_VDEC_EX_V2_USER_CMD_RM_ENABLE_PTS_TBL:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_RmEnablePtsTbl(pStreamId,
                                             (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_RmEnablePtsTbl(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_IDCT_MODE:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_SetIdctMode(pStreamId,
                                          (MS_U8)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_SetIdctMode(pStreamId, (MS_U8)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_DROP_ERR_FRAME:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_DropErrFrame(pStreamId,
                                          (MS_U8)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_DropErrFrame(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_CC608_INFO_ENHANCE_MODE:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_SetCcInfoEnhanceMode(pStreamId,
                                          (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_SetCcInfoEnhanceMode(pStreamId,(MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_IGNORE_PIC_OVERRUN:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_IgnorePicOverrun(pStreamId,
                                               (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_IgnorePicOverrun(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_SELF_SEQCHANGE:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_SetSelfSeqChange(pStreamId,
                                               (MS_U8)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_SetSelfSeqChange(pStreamId,(MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_AUTO_EXHAUST_ES_MODE:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_SetAutoExhaustESMode(pStreamId,
                                                   (MS_U32)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_SetAutoExhaustESMode(pStreamId, (MS_U32)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_CTL_SPEED_IN_DISP_ONLY:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_CtlSpeedInDispOnly(pStreamId,
                                                 (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_CtlSpeedInDispOnly(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_AVC_SUPPORT_REF_NUM_OVER_MAX_DPB_SIZE:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_AVCSupportRefNumOverMaxDBPSize(pStreamId,
                                                             (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_AVCSupportRefNumOverMaxDBPSize(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_RETURN_INVALID_AFD:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_ReturnInvalidAFD(pStreamId,
                                               (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_ReturnInvalidAFD(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_FIELD_POLARITY_DISPLAY_ONE_FIELD:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_Field_Polarity_Display_One_Field(pStreamId,&(pVdecExContext->store_Field_Polarity[u8Idx]));
                //free(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].pStructArg);
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_Field_Polarity_Display_One_Field(pStreamId,(VDEC_EX_Field_Polarity*)(*((MS_VIRT*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,0,1,sizeof(VDEC_EX_Field_Polarity),0,1,2,3,0);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 0;
                resume_data.structArg_num = 1;
                //resume_data.struct_size = sizeof(VDEC_EX_Field_Polarity);
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 0;
                _VDEC_Memcpy((void*)(&(pVdecExContext->store_Field_Polarity[u8Idx])),
                                     param[0],
                                     sizeof(VDEC_EX_Field_Polarity));
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_AVC_FORCE_BROKEN_BY_US:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_AVCForceBrokenByUs(pStreamId,
                                                 (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_AVCForceBrokenByUs(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_SHOW_FIRST_FRAME_DIRECT:
            if(IsResume == TRUE)
            {
                ret = _VDEC_EX_ShowFirstFrameDirect(pStreamId,
                                                   (MS_U32)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _VDEC_EX_ShowFirstFrameDirect(pStreamId,(MS_U32)(*((MS_U32*)(param[0]))));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_AVC_RESIZE_DOS_DISP_PEND_BUF:
            ret = _VDEC_EX_AVCResizeDosDispPendBuf(pStreamId,(MS_U32)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_XC_LOW_DELAY_PARA:
            ret = _VDEC_EX_SetXcLowDelayPara(pStreamId,(MS_U32)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_SECURE_MODE:
            ret = _VDEC_EX_SetSecureMode(pStreamId, (MS_U32)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_FRAMERATE_HANDLING:
            ret = _VDEC_EX_FramerateHandling(pStreamId, (MS_U32)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_RVU_SETTING_MODE:
            ret = _VDEC_EX_RVU_Setting_Mode(pStreamId, (MS_U32)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_DUAL_NON_BLOCK_MODE:
            ret = _VDEC_EX_DualNonBlockMode(pStreamId, (MS_BOOL)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_IGNORE_PIC_STRUCT_DISPLAY:
            ret = _VDEC_EX_IgnorePicStructDisplay(pStreamId, (MS_U32)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_INPUT_PTS_FREERUN_MODE:
            ret = _VDEC_EX_InputPtsFreerunMode(pStreamId, (MS_U32)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_ERR_CONCEAL_SLICE_1ST_MB:
            ret = _VDEC_EX_ErrConcealStartSlice1stMB(pStreamId, (MS_U32)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_EXTERNAL_DS_BUFFER:
            ret = _VDEC_EX_Set_External_DS_Buffer(pStreamId,(VDEC_EX_EXTERNAL_DS_BUFFER*)(*((MS_VIRT*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_MIN_TSP_DATA_SIZE:
            ret = _VDEC_EX_SetMinTspDataSize(pStreamId,(MS_U32)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_DMX_FRAMERATE:
            ret = _VDEC_EX_SetDmxFrameRate(pStreamId, (MS_U32)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_DMX_FRAMERATEBASE:
            ret = _VDEC_EX_SetDmxFrameRateBase(pStreamId, (MS_U32)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_ENABLE_CC_608_EXTERNAL_BUFFER:
            ret = _VDEC_EX_Enable_External_CC_608_Buffer(pStreamId,(MS_VIRT)(*((MS_VIRT*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_ENABLE_CC_708_EXTERNAL_BUFFER:
            ret = _VDEC_EX_Enable_External_CC_708_Buffer(pStreamId,(MS_VIRT)(*((MS_VIRT*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_EXIT:
            ret = _MApi_VDEC_EX_V2_Exit(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_RST:
            ret = _MApi_VDEC_EX_V2_Rst(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_CHECK_DISPINFO_READY:
            ret = _MApi_VDEC_EX_V2_CheckDispInfoRdy(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_FRC_MODE:
            if(IsResume == TRUE)
            {
                ret = _MApi_VDEC_EX_V2_SetFrcMode(pStreamId,
                                                 (VDEC_EX_FrcMode)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _MApi_VDEC_EX_V2_SetFrcMode(pStreamId,*((VDEC_EX_FrcMode*)(param[0])));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_DYNSCALING_PARAMS:
            if(IsResume == TRUE)
            {
                ret = _MApi_VDEC_EX_V2_SetDynScalingParams(pStreamId,
                                                          (MS_PHY)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]),
                                                          (MS_U32)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[1]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _MApi_VDEC_EX_V2_SetDynScalingParams(pStreamId,
                                                       *((MS_PHY*)(param[0])),
                                                       *((MS_U32*)(param[1])));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,2,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 2;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_DBG_LEVEL:
            ret = _VDEC_EX_SetDbgLevel(pStreamId,*((VDEC_EX_DbgLevel*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_PLAY:
            ret = _MApi_VDEC_EX_V2_Play(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_PAUSE:
            ret = _MApi_VDEC_EX_V2_Pause(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_RESUME:
            ret = _MApi_VDEC_EX_V2_Resume(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_STEP_DISP:
            ret = _MApi_VDEC_EX_V2_StepDisp(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_STEP_DECODE:
            ret = _MApi_VDEC_EX_V2_StepDecode(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_TRICK_MODE:
            ret = _MApi_VDEC_EX_V2_SetTrickMode(pStreamId,*((VDEC_EX_TrickDec*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_PUSH_DECQ:
            ret = _MApi_VDEC_EX_V2_PushDecQ(pStreamId,((VDEC_EX_DecCmd*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_FLUSH:
            ret = _MApi_VDEC_EX_V2_Flush(pStreamId,*((VDEC_EX_FreezePicSelect*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_ENABLE_LAST_FRAME_SHOW:
            ret = _MApi_VDEC_EX_V2_EnableLastFrameShow(pStreamId,*((MS_BOOL*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_SPEED:
            ret = _MApi_VDEC_EX_V2_SetSpeed(pStreamId,
                                         *((VDEC_EX_SpeedType*)(param[0])),
                                         *((VDEC_EX_DispSpeed*)(param[1])));
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_FREEZE_DISP:
            ret = _MApi_VDEC_EX_V2_SetFreezeDisp(pStreamId,*((MS_BOOL*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_BLUE_SCREEN:
            ret = _MApi_VDEC_EX_V2_SetBlueScreen(pStreamId,*((MS_BOOL*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_RESET_PTS:
            ret = _MApi_VDEC_EX_V2_ResetPTS(pStreamId,*((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_AVSYNC_ON:
            if(IsResume == TRUE)
            {
                ret = _MApi_VDEC_EX_V2_AVSyncOn(pStreamId,
                                               (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]),
                                               (MS_U32)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[1]),
                                               (MS_U16)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[2]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _MApi_VDEC_EX_V2_AVSyncOn(pStreamId,
                                            *((MS_BOOL*)(param[0])),
                                            *((MS_U32*)(param[1])),
                                            *((MS_U16*)(param[2])));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,3,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 3;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_AVSYNC_FREERUN_THRESHOLD:
            if(IsResume == TRUE)
            {
                ret = _MApi_VDEC_EX_V2_SetAVSyncFreerunThreshold(pStreamId,
                                                                (MS_U32)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _MApi_VDEC_EX_V2_SetAVSyncFreerunThreshold(pStreamId,*((MS_U32*)(param[0])));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_EVENT_MULTICALLBACK:
            if(IsResume == TRUE)
            {
                ret = _MApi_VDEC_EX_V2_SetEvent_MultiCallback(pStreamId,
                                                             (VDEC_EX_CB_TYPE)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]),
                                                             (MS_U32)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[1]),
                                                             (VDEC_EX_EventCb)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[2]),
                                                             (void*)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[3]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _MApi_VDEC_EX_V2_SetEvent_MultiCallback(pStreamId,
                                                          *((VDEC_EX_CB_TYPE*)(param[0])),
                                                          *((MS_U32*)(param[1])),
                                                          ((VDEC_EX_EventCb)(param[2])),
                                                          param[3]);
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,4,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 4;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_UNSET_EVENT_MULTICALLBACK:
            if(IsResume == TRUE)
            {
                ret = _MApi_VDEC_EX_V2_UnsetEvent_MultiCallback(pStreamId,
                                                               (VDEC_EX_CB_TYPE)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]),
                                                               (MS_U32)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[1]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _MApi_VDEC_EX_V2_UnsetEvent_MultiCallback(pStreamId,
                                                            *((VDEC_EX_CB_TYPE*)(param[0])),
                                                            *((MS_U32*)(param[1])));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,2,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 2;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_FIRE_DEC:
            ret = _MApi_VDEC_EX_V2_FireDecCmd(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_SEEK_TO_PTS:
            ret = _MApi_VDEC_EX_V2_SeekToPTS(pStreamId,*((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_SKIP_TO_PTS:
            ret = _MApi_VDEC_EX_V2_SkipToPTS(pStreamId,*((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_DISABLE_DEBLOCKING:
            if(IsResume == TRUE)
            {
                ret = _MApi_VDEC_EX_V2_DisableDeblocking(pStreamId,
                                                        (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _MApi_VDEC_EX_V2_DisableDeblocking(pStreamId,*((MS_BOOL*)(param[0])));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_DISABLE_QUARTER_PIXEL:
            if(IsResume == TRUE)
            {
                ret = _MApi_VDEC_EX_V2_DisableQuarterPixel(pStreamId,
                                                         (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _MApi_VDEC_EX_V2_DisableQuarterPixel(pStreamId,*((MS_BOOL*)(param[0])));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_AUTO_RM_LST_ZERO_BYTE:
            if(IsResume == TRUE)
            {
                ret = _MApi_VDEC_EX_V2_SetAutoRmLstZeroByte(pStreamId,
                                                         (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _MApi_VDEC_EX_V2_SetAutoRmLstZeroByte(pStreamId,*((MS_BOOL*)(param[0])));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_BALANCE_BW:
            if(IsResume == TRUE)
            {
                ret = _MApi_VDEC_EX_V2_SetBalanceBW(pStreamId,
                                                   (MS_U8)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]),
                                                   (MS_U8)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[1]),
                                                   (MS_U8)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[2]),
                                                   (MS_U8)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[3]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _MApi_VDEC_EX_V2_SetBalanceBW(pStreamId,
                                                *((MS_U8*)(param[0])),
                                                *((MS_U8*)(param[1])),
                                                *((MS_U8*)(param[2])),
                                                *((MS_U8*)(param[3])));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,4,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 4;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_GEN_PATTERN:
            ret = _MApi_VDEC_EX_V2_GenPattern(pStreamId,
                                              *((VDEC_EX_PatternType*)(param[0])),
                                              *((MS_PHY*)(param[1])),
                                              ((MS_U32*)(param[2])));
            break;
        case E_VDEC_EX_V2_USER_CMD_MHEG_DECODE_IFRAME:
            ret = _MApi_VDEC_EX_V2_MHEG_DecodeIFrame(pStreamId,
                                                    *((MS_PHY*)(param[0])),
                                                    *((MS_PHY*)(param[1])),
                                                    *((MS_PHY*)(param[2])));
            break;
        case E_VDEC_EX_V2_USER_CMD_MHEG_RST_IFRAME_DEC:
            ret = _MApi_VDEC_EX_V2_MHEG_RstIFrameDec(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_CC_START_PARSING:
            ret = _MApi_VDEC_EX_V2_CC_StartParsing(pStreamId,((VDEC_EX_CCCfg*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_CC_STOP_PARSING:
            ret = _MApi_VDEC_EX_V2_CC_StopParsing(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_CC_UPDATE_READ_PTR:
            ret = _MApi_VDEC_EX_V2_CC_UpdateReadPtr(pStreamId,*((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_BLOCK_DISPLAY:
            if(IsResume == TRUE)
            {
                ret = _MApi_VDEC_EX_V2_SetBlockDisplay(pStreamId,
                                                      (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _MApi_VDEC_EX_V2_SetBlockDisplay(pStreamId,*((MS_BOOL*)(param[0])));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_ENABLE_ES_BUFF_MALLOC:
            if(IsResume == TRUE)
            {
                ret = _MApi_VDEC_EX_V2_EnableESBuffMalloc(pStreamId,
                                                         (MS_BOOL)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _MApi_VDEC_EX_V2_EnableESBuffMalloc(pStreamId,*((MS_BOOL*)(param[0])));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_DISPLAY_FRAME:
            ret = _MApi_VDEC_EX_V2_DisplayFrame(pStreamId,((VDEC_EX_DispFrame*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_RELEASE_FRAME:
            ret = _MApi_VDEC_EX_V2_ReleaseFrame(pStreamId,((VDEC_EX_DispFrame*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_CAPTURE_FRAME:
            ret = _MApi_VDEC_EX_V2_CaptureFrame(pStreamId,*((MS_U32*)(param[0])),*((MS_BOOL*)(param[1])));
            break;
        case E_VDEC_EX_V2_USER_CMD_CC_INIT:
            ret = _MApi_VDEC_EX_V2_CC_Init(pStreamId,((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_CC_SET_CFG:
            if(IsResume == TRUE)
            {
                ret = _MApi_VDEC_EX_V2_CC_SetCfg(pStreamId,
                                                (VDEC_EX_CCFormat)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]),
                                                (VDEC_EX_CCType)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[1]),
                                                (MS_U32*)(&(pVdecExContext->store_InputPara_setcfg[u8Idx])));
                //free(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].pStructArg);
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _MApi_VDEC_EX_V2_CC_SetCfg(pStreamId,
                                             *((VDEC_EX_CCFormat*)(param[0])),
                                             *((VDEC_EX_CCType*)(param[1])),
                                             ((MS_U32*)(param[2])));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,2,1,sizeof(VDEC_EX_CC_InputPara),0,1,2,3,2);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 2;
                resume_data.structArg_num = 1;
                //resume_data.struct_size = sizeof(VDEC_EX_CC_InputPara);
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 2;
                _VDEC_Memcpy((void*)(&(pVdecExContext->store_InputPara_setcfg[u8Idx])),
                             param[2],
                             sizeof(VDEC_EX_CC_InputPara));
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_CC_SET_BUFF_START_ADDR:
            if(IsResume == TRUE)
            {
                ret = _MApi_VDEC_EX_V2_CC_SetBuffStartAdd(pStreamId,
                                                         (VDEC_EX_CCFormat)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]),
                                                         (MS_U32*)(&(pVdecExContext->store_InputPara_startaddr[u8Idx])));
                //free(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].pStructArg);
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _MApi_VDEC_EX_V2_CC_SetBuffStartAdd(pStreamId,
                                                      *((VDEC_EX_CCFormat*)(param[0])),
                                                      ((MS_U32*)(param[1])));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,1,sizeof(VDEC_EX_CC_InputPara),0,1,2,3,1);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 1;
                //resume_data.struct_size = sizeof(VDEC_EX_CC_InputPara);
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 1;
                _VDEC_Memcpy((void*)(&(pVdecExContext->store_InputPara_startaddr[u8Idx])),
                             param[1],
                             sizeof(VDEC_EX_CC_InputPara));
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_CC_UPDATE_WRITE_ADDR:
            if(IsResume == TRUE)
            {
                ret = _MApi_VDEC_EX_V2_CC_UpdateWriteAdd(pStreamId,
                                                        (VDEC_EX_CCFormat)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]),
                                                         NULL);
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _MApi_VDEC_EX_V2_CC_UpdateWriteAdd(pStreamId,
                                                      *((VDEC_EX_CCFormat*)(param[0])),
                                                      ((MS_U32*)(param[1])));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_CC_UPDATE_READ_ADDR:
            if(IsResume == TRUE)
            {
                ret = _MApi_VDEC_EX_V2_CC_UpdateReadAdd(pStreamId,
                                                       (VDEC_EX_CCFormat)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]),
                                                       (MS_U32*)(&(pVdecExContext->store_InputPara_readaddr[u8Idx])));
                //free(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].pStructArg);
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _MApi_VDEC_EX_V2_CC_UpdateReadAdd(pStreamId,
                                                    *((VDEC_EX_CCFormat*)(param[0])),
                                                    ((MS_U32*)(param[1])));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,1,sizeof(VDEC_EX_CC_InputPara),0,1,2,3,1);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 1;
                //resume_data.struct_size = sizeof(VDEC_EX_CC_InputPara);
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 1;
                _VDEC_Memcpy((void*)(&(pVdecExContext->store_InputPara_readaddr[u8Idx])),
                             param[1],
                             sizeof(VDEC_EX_CC_InputPara));
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_CC_DISABLE_PARSING:
            if(IsResume == TRUE)
            {
                ret = _MApi_VDEC_EX_V2_CC_DisableParsing(pStreamId,
                                                        (VDEC_EX_CCFormat)(pVdecExContext->store_set_cmd[pVdecExContext->store_set_cmd_read_pointer[u8Idx]].u32Arg[0]));
                pVdecExContext->store_set_cmd_read_pointer[u8Idx]++;
            }
            else
            {
                ret = _MApi_VDEC_EX_V2_CC_DisableParsing(pStreamId,*((VDEC_EX_CCFormat*)(param[0])));
                //_VDEC_PREPARE_RESUME_DATA(u8Idx,cmd_id,param,1,0,0,0,1,2,3,4);
                resume_data.u8Idx=u8Idx;
                resume_data.cmd_id=cmd_id;
                resume_data.param=param;
                resume_data.u32Arg_num = 1;
                resume_data.structArg_num = 0;
                //resume_data.struct_size = 0;
                resume_data.position[0] = 0;
                resume_data.position[1] = 1;
                resume_data.position[2] = 2;
                resume_data.position[3] = 3;
                //resume_data.position[4] = 4;
                _MApi_VDEC_EX_V2_StoreUserCmd(&resume_data);
            }
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_TIME_INC_PREDICT_PARA:
            ret = _MApi_VDEC_EX_SetTimeIncPredictParam(pStreamId,(MS_U32)(*((MS_U32*)(param[0]))));
            break;

        case E_VDEC_EX_V2_USER_CMD_ENABLE_DECODE_ENGINE_TIMEOUT:
            ret = _VDEC_EX_SetDecodeTimeoutParam(pStreamId,(VDEC_EX_Decode_Timeout_Param*)(*((MS_VIRT*)(param[0]))));
            break;

        case E_VDEC_EX_V2_USER_CMD_FRAMEBUFFER_AUTO_MODE:
            ret = _MApi_VDEC_EX_SetFramebufferAutoMode(pStreamId,(MS_BOOL)(*((MS_U32*)(param[0]))));
            break;

        case E_VDEC_EX_V2_USER_CMD_SET_SMOOTH_REWIND:
            ret = _MApi_VDEC_EX_SetSmooth_Rewind(pStreamId,(MS_U8)(*((MS_U32*)(param[0]))));
            break;

        case E_VDEC_EX_V2_USER_CMD_SET_ERROR_TOLERANCE:
            ret = _MApi_VDEC_EX_SetError_Tolerance(pStreamId,(VDEC_EX_Err_Tolerance*)(*((MS_VIRT*)(param[0]))));
            break;

        case E_VDEC_EX_V2_USER_CMD_SET_PTS_US_MODE:
            ret = _MApi_VDEC_EX_Set_PTS_USec_Mode(pStreamId,(MS_BOOL)(*((MS_U32*)(param[0]))));
            break;

        case E_VDEC_EX_V2_USER_CMD_AUTO_INSERT_DUMMY_DATA:
            ret = _MApi_VDEC_EX_Enable_AutoInsertDummyPattern(pStreamId,(MS_BOOL)(*((MS_U32*)(param[0]))));
            break;

        case E_VDEC_EX_V2_USER_CMD_DROP_ONE_PTS:
            ret = _MApi_VDEC_EX_Drop_One_PTS(pStreamId);
            break;

        case E_VDEC_EX_V2_USER_CMD_PVR_TIMESHIFT_SEAMLESS_MODE:
            ret = _MApi_VDEC_EX_PVR_Seamless_mode(pStreamId,(MS_U8)(*((MS_U32*)(param[0]))));
            break;

        case E_VDEC_EX_V2_USER_CMD_SET_MBX_PARAM:
            ret = _MApi_VDEC_EX_V2_Set_MBX_param((MS_U8)(*((MS_U8*)(param[0]))));
            break;

        case E_VDEC_EX_V2_USER_CMD_SET_FRAMEBUFF2:
            {
                VDEC_FRAMEBuff *framebuff = (VDEC_FRAMEBuff *)(*((MS_VIRT*)(param[0])));
                ret = _MApi_VDEC_EX_SetFrameBuff2(pStreamId, framebuff->u32Addr, framebuff->u32Size);
            }
            break;

        case E_VDEC_EX_V2_USER_CMD_ENABLE_PTS_DECTECTOR:
            ret = _VDEC_EX_EnablePTSDetector(pStreamId, (MS_U32)(*((MS_U32*)(param[0]))));
            break;

        case E_VDEC_EX_V2_USER_CMD_DISABLE_PBFRAME_MODE:
            ret = _VDEC_EX_DisablePBFrameMode(pStreamId, (MS_U32)(*((MS_U32*)(param[0]))));
            break;

        case E_VDEC_EX_V2_USER_CMD_SET_TRICKPLAY_2X_MODE:
            ret = _MApi_VDEC_EX_SetTrickPlay_2X_MODE(pStreamId,(VDEC_EX_TrickPlay2xMode)(*((MS_U32*)(param[0]))));
            break;

        case E_VDEC_EX_V2_USER_CMD_FRC_ONLY_SHOW_TOP_FIELD:
            ret = _MApi_VDEC_EX_FRC_OnlyShowTopField(pStreamId,(MS_BOOL)(*((MS_U32*)(param[0]))));
            break;

        case E_VDEC_EX_V2_USER_CMD_DISABLE_ES_FULL_STOP:
            ret = _MApi_VDEC_EX_DisableEsFullStop(pStreamId,(MS_BOOL)(*((MS_BOOL*)(param[0]))));
            break;

        case E_VDEC_EX_V2_USER_CMD_SET_SLOW_SYNC:
            ret = _MApi_VDEC_EX_SetSlow_Sync(pStreamId,(VDEC_EX_Slow_Sync*)(*((MS_VIRT*)(param[0]))));
            break;

        case E_VDEC_EX_V2_USER_CMD_DIRECT_STC_MODE:
            ret = _MApi_VDEC_EX_Direct_STC_Mode(pStreamId,(MS_U32)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_SET_DV_XC_SHM_ADDR:
            {
                VDEC_PRINT("Warning: This is old function. Use MApi_VDEC_EX_PreSetControl(E_VDEC_EX_USER_CMD_SET_BUFFER_INFO) instead.");
                MS_PHY *pXCShmAddr = (MS_PHY *)(*((MS_VIRT*)(param[0])));
                ret = _MApi_VDEC_EX_V2_SetDVXCShmAddr(pStreamId, (MS_PHY)*pXCShmAddr);
            }
            break;
#ifdef VDEC_CAP_DV_OTT_API
        case E_VDEC_EX_V2_USER_CMD_SET_DV_INFO:
            ret = _MApi_VDEC_EX_SetDVInfo(pStreamId,(VDEC_EX_DV_Info*)(*((MS_VIRT*)(param[0]))));
            break;
#endif

        default:
            ret = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return ret;
}


VDEC_EX_Result MApi_VDEC_EX_GetControl(VDEC_StreamId* pStreamId, VDEC_EX_User_Cmd cmd_id, MS_U32* param)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        if(UtopiaOpen(MODULE_VDEC_EX | VDEC_DRIVER_BASE, &pu32VDEC_EX_Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            VPRINTF("UtopiaOpen VDEC_EX failed\n");
            return E_VDEC_EX_FAIL;
        }
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = _VDEC_MapUserCmd_V2(cmd_id);
    IO_arg.param[0] = (void*)param;
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }
    else
    {
#if 1//#ifdef VDEC_UTOPIA_2K
        if(E_VDEC_EX_USER_CMD_GET_FPA_SEI_EX == cmd_id)
        {
            VDEC_EX_Frame_packing_SEI_EX* ptr = (VDEC_EX_Frame_packing_SEI_EX*)param;
            ptr->u32DataBuff = MsOS_PA2KSEG1(ptr->u32DataBuff);
        }
#endif
        return ret;
    }
}


VDEC_EX_Result MApi_VDEC_EX_V2_GetControl(VDEC_StreamId* pStreamId,VDEC_EX_V2_User_Cmd cmd_id,void** param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  u8Idx = 0;

    if((cmd_id == E_VDEC_EX_V2_USER_CMD_SYSTEM_PREGET_FB_MEMORY_USAGE_SIZE)
#ifdef VDEC_CAP_SYSTEM_PREGET_API
#ifdef VDEC_CAP_DV_OTT_API
       || (cmd_id == E_VDEC_EX_V2_USER_CMD_SYSTEM_PREGET_DV_SUPPORT_PROFILE)
       || (cmd_id == E_VDEC_EX_V2_USER_CMD_SYSTEM_PREGET_DV_SUPPORT_LEVEL)
#endif
#endif
      )
    {
        //do nothing
    }
    else
    {
        if(E_VDEC_EX_V2_USER_CMD_GET_CODEC_CAP == cmd_id)
        {
           return _VDEC_EX_GetCodecCapInfo((VDEC_EX_CODEC_CAP_INFO*)(param[0]));
        }

        if(E_VDEC_EX_V2_USER_CMD_GET_SECURE_MODE == cmd_id)
        {
            return _MApi_VDEC_EX_V2_IsSecureMode((MS_BOOL*)(param[0]));
        }

        if(pStreamId == NULL)
            return E_VDEC_EX_FAIL;

        u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);

        if(E_VDEC_EX_V2_USER_CMD_GET_MAIN_STREAM_ID == cmd_id)
        {
            u8Idx = 0;
        }
        else if(E_VDEC_EX_V2_USER_CMD_GET_SUB_STREAM_ID == cmd_id)
        {
            u8Idx = 1;
        }

        //these cmd should receive between vdec init and vdec exit
        _VDEC_INVALID_IDX_RET(u8Idx);
        _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);
        _VDEC_NOT_INIT_RET(u8Idx);

        if(cmd_id != E_VDEC_EX_V2_USER_CMD_GET_MAIN_STREAM_ID &&
            cmd_id != E_VDEC_EX_V2_USER_CMD_GET_SUB_STREAM_ID)
        {
            _VDEC_INVALID_STREAM_ID_RET(pStreamId->u32Id);
        }
    }

    switch (cmd_id)
    {
        case E_VDEC_EX_V2_USER_CMD_GET_CHROMA_TYPE:
            eRet =  _VDEC_EX_IsChroma420(pStreamId, ((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_REAL_FRAMERATE:
            eRet = _VDEC_EX_GetRealFrameRate(pStreamId, ((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_ES_QUANTITY:
            eRet = _VDEC_EX_GetESQuantity(pStreamId, ((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_COLOR_MATRIX:
            eRet = _VDEC_EX_GetColorMatrixCoef(pStreamId, ((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_DYNSCALE_ENABLED:
            eRet = _VDEC_EX_GetIsDynScalingEnabled(pStreamId, ((MS_U32*)(param[0])));
            break;
        // Report internal active stream ID, and will not use input pStreamId.
        case E_VDEC_EX_V2_USER_CMD_GET_MAIN_STREAM_ID:
        case E_VDEC_EX_V2_USER_CMD_GET_SUB_STREAM_ID:
            if (param)
            {
                VDEC_StreamId *pOutStreamID = ((VDEC_StreamId*)(param[0]));
                if (E_VDEC_EX_V2_USER_CMD_GET_MAIN_STREAM_ID == cmd_id)
                {
                    u8Idx = (MS_U8)E_VDEC_EX_MAIN_STREAM;
                }
                else if (E_VDEC_EX_V2_USER_CMD_GET_SUB_STREAM_ID == cmd_id)
                {
                    u8Idx = (MS_U8)E_VDEC_EX_SUB_STREAM;
                }
                pOutStreamID->u32Version = 0;
                pOutStreamID->u32Id = (u8Idx << 24 | pVdecExContext->_Attr[u8Idx].u32DrvId);
            }
            break;

        case E_VDEC_EX_V2_USER_CMD_GET_FPA_SEI:
            eRet = _MApi_VDEC_EX_GetFrmPackingArrSEI(pStreamId, param[0]);
            break;

        case E_VDEC_EX_V2_USER_CMD_GET_FPA_SEI_EX:
            eRet = _MApi_VDEC_EX_GetFrmPackingArrSEI_EX(pStreamId, param[0]);
            break;

        case E_VDEC_EX_V2_USER_CMD_GET_CLLI_SEI:
            eRet = _MApi_VDEC_EX_GetContentLightLevelInfoSEI(pStreamId, param[0]);
            break;

        case E_VDEC_EX_V2_USER_CMD_GET_DCV_SEI:
            eRet = _MApi_VDEC_EX_GetDisplayColourVolumeArrSEI(pStreamId, param[0]);
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_VUI_DISP_INFO:
            eRet = _MApi_VDEC_EX_V2_GetVUI_DISP_INFO(pStreamId, param[0]);
            break;
#if VDEC_ENABLE_MVC
        case E_VDEC_EX_V2_USER_CMD_GET_ES2_QUANTITY:
        {
            VDEC_StreamId stTmpId;
            stTmpId.u32Version = pStreamId->u32Version;
            stTmpId.u32Id = pStreamId->u32Id + 0x00011000;
            eRet = _VDEC_EX_GetESQuantity(&stTmpId, ((MS_U32*)(param[0])));
            break;
        }
        case E_VDEC_EX_V2_USER_CMD_GET_MVC_SUB_FRAME_DISP_INFO:
        {
            eRet = _MApi_VDEC_EX_MVC_SubFrameDispInfo(pStreamId, ((MS_U32*)(param[0])));
            break;
        }
        case E_VDEC_EX_V2_USER_CMD_GET_MVC_BBU2_DECQ_VACANCY:
            eRet = _MApi_VDEC_EX_MVC_GetControl(pStreamId, E_VDEC_EX_MVC_GET_BBU2_DECQ_VACANCY,((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_MVC_ES2_READ_PTR:
            eRet = _MApi_VDEC_EX_MVC_GetControl(pStreamId, E_VDEC_EX_MVC_GET_ES2_READ_PTR,((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_MVC_ES2_WRITE_PTR:
            eRet = _MApi_VDEC_EX_MVC_GetControl(pStreamId, E_VDEC_EX_MVC_GET_ES2_WRITE_PTR,((MS_U32*)(param[0])));
            break;
#endif /// VDEC_ENABLE_MVC
        case E_VDEC_EX_V2_USER_CMD_GET_U64PTS:
            eRet = _MApi_VDEC_EX_Get_U64PTS(pStreamId,param[0]);
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_PRE_PAS_U64PTS:
            eRet = _MApi_VDEC_EX_Get_PRE_PAS_U64PTS(pStreamId,param[0]);
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_ORI_INTERLACE_MODE:
            eRet = _VDEC_EX_GetIsOriInterlaceMode(pStreamId,((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_MBS_ONLY_FLAG:
            eRet = _VDEC_GetMbsOnlyFlag(pStreamId,((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_CRC_VALUE:
            eRet = _VDEC_EX_GetCrcValue(pStreamId,((VDEC_EX_CrcValue*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_BBU_Q_NUM:
            eRet = _VDEC_GetBBUQNum(pStreamId, ((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_DISP_FRAME_NUM:
            eRet = _VDEC_EX_GetDispFrmNum(pStreamId, ((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_ES_BUFFER_STATUS:
            eRet = _VDEC_EX_GetESBufferStatus(pStreamId, ((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_CODEC_TYPE:
            eRet = _VDEC_EX_GetFWCodecType(pStreamId, ((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_SHAREMEMORY_BASE:
            eRet = _VDEC_EX_Get_ShareMemory_Base(pStreamId, ((MS_VIRT*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_XC_LOW_DELAY_INT_STATE:
            eRet = _VDEC_EX_GetXcLowDelayIntState(pStreamId, ((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_IS_LEAST_DISPQ_SIZE_FLAG:
            eRet = _VDEC_IsLeastDispQSize(pStreamId, ((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_FIELD_PIC_FLAG:
            eRet = _VDEC_EX_GetFieldPicFlag(pStreamId, ((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_SUPPORT_2ND_MVOP_INTERFACE:
            eRet = _VDEC_EX_GetSupport2ndMVOPInterface(pStreamId, ((MS_BOOL*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_VSYNC_BRIDGE_ADDR:
            eRet = _VDEC_EX_GetVsyncBridgeAddr(pStreamId, ((MS_VIRT*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_SEQ_CHANGE_INFO:
            eRet = _VDEC_EX_GetSeqChangeInfo(pStreamId, ((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_IS_STEP_DISP_DONE:
            eRet = _MApi_VDEC_EX_V2_IsStepDispDone(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_IS_STEP_DECODE_DONE:
            eRet = _MApi_VDEC_EX_V2_IsStepDecodeDone(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_DISP_INFO:
            eRet = _MApi_VDEC_EX_V2_GetDispInfo(pStreamId,((VDEC_EX_DispInfo*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_IS_AVSYNC_ON:
            eRet = _MApi_VDEC_EX_V2_IsAVSyncOn(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_IS_WITH_VALID_STREAM:
            eRet = _MApi_VDEC_EX_V2_IsWithValidStream(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_IS_DISP_FINISH:
            eRet = _MApi_VDEC_EX_V2_IsDispFinish(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_IS_IFRAME_FOUND:
            eRet = _MApi_VDEC_EX_V2_IsIFrameFound(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_IS_SEQ_CHG:
            eRet = _MApi_VDEC_EX_V2_IsSeqChg(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_IS_REACH_SYNC:
            eRet = _MApi_VDEC_EX_V2_IsReachSync(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_IS_START_SYNC:
            eRet = _MApi_VDEC_EX_V2_IsStartSync(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_IS_FREERUN:
            eRet = _MApi_VDEC_EX_V2_IsFreerun(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_IS_WITH_LOW_DELAY:
            eRet = _MApi_VDEC_EX_V2_IsWithLowDelay(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_IS_ALL_BUFFER_EMPTY:
            eRet = _MApi_VDEC_EX_V2_IsAllBufferEmpty(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_EXT_DISP_INFO:
            eRet = _MApi_VDEC_EX_V2_GetExtDispInfo(pStreamId,((VDEC_EX_ExtDispInfo*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_DEC_FRAME_INFO:
            eRet = _MApi_VDEC_EX_V2_GetDecFrameInfo(pStreamId,((VDEC_EX_FrameInfo*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_DISP_FRAME_INFO:
            eRet = _MApi_VDEC_EX_V2_GetDispFrameInfo(pStreamId,((VDEC_EX_FrameInfo*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_DEC_TIMECODE:
            eRet = _MApi_VDEC_EX_V2_GetDecTimeCode(pStreamId,((VDEC_EX_TimeCode*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_DISP_TIMECODE:
            eRet = _MApi_VDEC_EX_V2_GetDispTimeCode(pStreamId,((VDEC_EX_TimeCode*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_EVENT_INFO:
            eRet = _MApi_VDEC_EX_V2_GetEventInfo(pStreamId,((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_ACTIVE_FORMAT:
            eRet = _MApi_VDEC_EX_V2_GetActiveFormat(pStreamId,((MS_U8*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_COLOUR_PRIMARIES:
            eRet = _MApi_VDEC_EX_V2_GetColourPrimaries(pStreamId,((MS_U8*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_FW_VERSION:
            eRet = _MApi_VDEC_EX_V2_GetFwVersion(pStreamId,*((VDEC_EX_FwType*)(param[0])),((MS_U32*)(param[1])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_GOP_CNT:
            eRet = _MApi_VDEC_EX_V2_GetGOPCnt(pStreamId,((MS_U8*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_ES_WRITE_PTR:
            eRet = _MApi_VDEC_EX_V2_GetESWritePtr(pStreamId,((MS_VIRT*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_ES_READ_PTR:
            eRet = _MApi_VDEC_EX_V2_GetESReadPtr(pStreamId,((MS_VIRT*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_PTS:
            eRet = _MApi_VDEC_EX_V2_GetPTS(pStreamId,((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_NEXT_PTS:
            eRet = _MApi_VDEC_EX_V2_GetNextPTS(pStreamId,((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_VIDEO_PTS_STC_DELTA:
            eRet = _MApi_VDEC_EX_V2_GetVideoPtsStcDelta(pStreamId,((MS_S64*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_ERR_CODE:
            eRet = _MApi_VDEC_EX_V2_GetErrCode(pStreamId,((VDEC_EX_ErrCode*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_ERR_CNT:
            eRet = _MApi_VDEC_EX_V2_GetErrCnt(pStreamId,((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_BITRATE:
            eRet = _MApi_VDEC_EX_V2_GetBitsRate(pStreamId,((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_FRAME_CNT:
            eRet = _MApi_VDEC_EX_V2_GetFrameCnt(pStreamId,((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_SKIP_CNT:
            eRet = _MApi_VDEC_EX_V2_GetSkipCnt(pStreamId,((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_DROP_CNT:
            eRet = _MApi_VDEC_EX_V2_GetDropCnt(pStreamId,((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_DISP_CNT:
            eRet = _MApi_VDEC_EX_V2_GetDispCnt(pStreamId,((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_DECQ_VACANCY:
            eRet = _MApi_VDEC_EX_V2_GetDecQVacancy(pStreamId,((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_IS_32_PULLDOWN:
            eRet = _MApi_VDEC_EX_V2_Is32PullDown(pStreamId,((MS_BOOL*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_IS_ALIVE:
            eRet = _MApi_VDEC_EX_V2_IsAlive(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_IS_CC_AVAILABLE:
            eRet = _MApi_VDEC_EX_V2_IsCCAvailable(pStreamId,((MS_BOOL*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_CC_INFO:
            eRet = _MApi_VDEC_EX_V2_GetCCInfo(pStreamId,param[0],*((MS_U32*)(param[1])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_TRICK_MODE:
            eRet = _MApi_VDEC_EX_V2_GetTrickMode(pStreamId,((VDEC_EX_TrickDec*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_ACTIVE_CODEC_TYPE:
            eRet = _MApi_VDEC_EX_V2_GetActiveCodecType(pStreamId,((VDEC_EX_CodecType*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_PATTERN_LEAST_LENGTH:
            eRet = _MApi_VDEC_EX_V2_GetPatternLeastLength(pStreamId,*((VDEC_EX_PatternType*)(param[0])),((MS_U32*)(param[1])));
            break;
        case E_VDEC_EX_V2_USER_CMD_MHEG_IS_IFRAME_DECODING:
            eRet = _MApi_VDEC_EX_V2_MHEG_IsIFrameDecoding(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_CC_GET_WRITE_PTR:
            eRet = _MApi_VDEC_EX_V2_CC_GetWritePtr(pStreamId,((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_CC_GET_READ_PTR:
            eRet = _MApi_VDEC_EX_V2_CC_GetReadPtr(pStreamId,((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_CC_GET_IS_OVERFLOW:
            eRet = _MApi_VDEC_EX_V2_CC_GetIsOverflow(pStreamId,((MS_BOOL*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_HW_KEY:
            eRet = _MApi_VDEC_EX_V2_GetHWKey(pStreamId,((MS_U8*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_ES_BUFF_VACANCY:
            eRet = _MApi_VDEC_EX_V2_GetESBuffVacancy(pStreamId,((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_ES_BUFF:
            eRet = _MApi_VDEC_EX_V2_GetESBuff(pStreamId,
                                              *((MS_U32*)(param[0])),
                                               ((MS_U32*)(param[1])),
                                               ((MS_PHY*)(param[2])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_NEXT_DISP_FRAME:
            eRet = _MApi_VDEC_EX_V2_GetNextDispFrame(pStreamId,((VDEC_EX_DispFrame*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_CC_GET_INFO:
            eRet = _MApi_VDEC_EX_V2_CC_GetInfo(pStreamId,
                                               *((VDEC_EX_CCFormat*)(param[0])),
                                               *((VDEC_EX_CCInfoCmd*)(param[1])),
                                               ((MS_U32*)(param[2])));
            break;
        case E_VDEC_EX_V2_USER_CMD_CC_GET_IS_RST_DONE:
            eRet = _MApi_VDEC_EX_V2_CC_GetIsRstDone(pStreamId,*((VDEC_EX_CCFormat*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_CC_GET_IS_BUFF_OVERFLOW:
            eRet = _MApi_VDEC_EX_V2_CC_GetIsBuffOverflow(pStreamId,*((VDEC_EX_CCFormat*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_CC_GET_WRITE_ADDR:
            eRet = _MApi_VDEC_EX_V2_CC_GetWriteAdd(pStreamId,
                                                  *((VDEC_EX_CCFormat*)(param[0])),
                                                  ((MS_PHY*)(param[1])));
            break;
        case E_VDEC_EX_V2_USER_CMD_CC_GET_READ_ADDR:
            eRet = _MApi_VDEC_EX_V2_CC_GetReadAdd(pStreamId,
                                                 *((VDEC_EX_CCFormat*)(param[0])),
                                                 ((MS_PHY*)(param[1])));
            break;
        case E_VDEC_EX_V2_USER_CMD_CHECKCAPS:
            eRet = _MApi_VDEC_EX_V2_CheckCaps(pStreamId,
                                              *((VDEC_EX_CodecType*)(param[0])),
                                              ((MS_BOOL*)(param[1])));
            break;
        case E_VDEC_EX_V2_USER_CMD_IS_FRAME_RDY:
            eRet = _MApi_VDEC_EX_IsFrameRdy(pStreamId);
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_FRAME_INFO_EX:
            eRet = _MApi_VDEC_EX_V2_GetFrameInfoEx(pStreamId,((VDEC_EX_FrameInfoEX*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_FLUSH_PATTEN_ENTRY_NUM:
            eRet = _VDEC_EX_GetFlushPatternEntryNum(pStreamId, ((MS_U32*)(param[0])));
            break;

        case E_VDEC_EX_V2_USER_CMD_GET_DS_BUF_MIU_SEL:
            eRet = _VDEC_EX_GetDSBufMiuSel(pStreamId, ((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_FW_STATUS_FLAG:
            eRet = _MApi_VDEC_EX_V2_GetFWStatusFlag(pStreamId,((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_HW_MAX_PIXEL:
            eRet = _VDEC_EX_GetHWMaxPixel(pStreamId, ((MS_U32*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_EVENT_FLAG:
            eRet = _MApi_VDEC_EX_V2_GetEventFlag(pStreamId,param[0] ,(*(MS_U32*)param[1]) );
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_NEXT_DISP_FRAME_INFO_EXT:
            eRet = _MApi_VDEC_EX_V2_GetNextDispFrameInfoExt(pStreamId, param[0]);
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_VSYNC_BRIDGE_EXT_ADDR:
            eRet = _VDEC_EX_GetVsyncBridgeExtAddr(pStreamId, ((MS_VIRT*)(param[0])));
            break;

        case E_VDEC_EX_V2_USER_CMD_GET_PVRSEAMLESS_INFO:
            eRet = _VDEC_EX_GetPVRSeamlessInfo(pStreamId, (VDEC_EX_PVR_Seamless_Info*)(param[0]));
            break;

        case E_VDEC_EX_V2_USER_CMD_SYSTEM_PREGET_FB_MEMORY_USAGE_SIZE:
            eRet =  _VDEC_EX_GetFbMemUsageSize((VDEC_EX_FbMemUsage_Param *)(param[0]));
            break;

        case E_VDEC_EX_V2_USER_CMD_GET_NOT_SUPPORT_INFO:
            eRet = _VDEC_EX_GetNotSupportInfo(pStreamId, ((MS_U32*)(param[0])));
            break;
#ifdef VDEC_CAP_SYSTEM_PREGET_API
#ifdef VDEC_CAP_DV_OTT_API
        case E_VDEC_EX_V2_USER_CMD_SYSTEM_PREGET_DV_SUPPORT_PROFILE:
            eRet = _MApi_VDEC_EX_GetDVSupportProfiles(((MS_VIRT*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_SYSTEM_PREGET_DV_SUPPORT_LEVEL:
            eRet = _MApi_VDEC_EX_GetDVSupportHighestLevel(((MS_VIRT*)(param[0])));
            break;
#endif
#endif
        case E_VDEC_EX_V2_USER_CMD_GET_DISP_QUEUE_EMPTY:
            eRet = _MApi_VDEC_EX_V2_IsDispQueueEmpty(pStreamId,((MS_BOOL*)(param[0])));
            break;
        case E_VDEC_EX_V2_USER_CMD_GET_MIN_TSP_DATA_SIZE:
            eRet = _VDEC_EX_GetMinTspDataSize(pStreamId, ((MS_U32*)(param[0])));
            break;
        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return eRet;
}

#ifdef VDEC_CAP_SYSTEM_PREGET_API
VDEC_EX_Result MApi_VDEC_EX_SystemPreGetControl(VDEC_EX_User_Cmd cmd_id, void* pParam)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        if(UtopiaOpen(MODULE_VDEC_EX | VDEC_DRIVER_BASE, &pu32VDEC_EX_Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            VPRINTF("UtopiaOpen VDEC_EX failed\n");
            return E_VDEC_EX_FAIL;
        }
    }

    IO_arg.eUserCmd = _VDEC_MapUserCmd_V2(cmd_id);
    IO_arg.param[0] = (pParam);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,E_VDEC_EX_V2_CMD_GET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }
    else
    {
        return ret;
    }
}
#endif

VDEC_EX_Result MApi_VDEC_EX_SystemPreSetControl(VDEC_EX_User_Cmd cmd_id, void* pParam)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        if(UtopiaOpen(MODULE_VDEC_EX | VDEC_DRIVER_BASE, &pu32VDEC_EX_Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            VPRINTF("UtopiaOpen VDEC_EX failed\n");
            return E_VDEC_EX_FAIL;
        }
    }

    IO_arg.eUserCmd = _VDEC_MapUserCmd_V2(cmd_id);
    IO_arg.param[0] = (pParam);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,E_VDEC_EX_V2_CMD_PRE_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }
    else
    {
        return ret;
    }
}

VDEC_EX_Result MApi_VDEC_EX_PreSetControl(VDEC_StreamId *pStreamId, VDEC_EX_User_Cmd cmd_id, MS_U32 param)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        if(UtopiaOpen(MODULE_VDEC_EX | VDEC_DRIVER_BASE, &pu32VDEC_EX_Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            VPRINTF("UtopiaOpen VDEC_EX failed\n");
            return E_VDEC_EX_FAIL;
        }
    }

    IO_arg.StreamID = (VDEC_EX_V2_StreamId*) pStreamId;
    IO_arg.eUserCmd = _VDEC_MapUserCmd_V2(cmd_id);
    IO_arg.param[0] = (void*)(&param);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,E_VDEC_EX_V2_CMD_PRE_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }
    else
    {
        return ret;
    }
}

VDEC_EX_Result MApi_VDEC_EX_V2_PreSetControl(VDEC_StreamId* pStreamId,VDEC_EX_V2_User_Cmd cmd_id,void** param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;
    MS_U8  i     = 0;
    MS_U8  u8Idx = 0;
    MS_U32 u32Id = 0;

    switch (cmd_id)
    {
        case E_VDEC_EX_V2_USER_CMD_SYSTEM_PRESET_VPU_CLOCK:
            eRet = _VDEC_EX_SetVPUClockSpeed((VDEC_EX_ClockSpeed)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_SYSTEM_PRESET_HVD_CLOCK:
            eRet = _VDEC_EX_SetHVDClockSpeed((VDEC_EX_ClockSpeed)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_SYSTEM_PRESET_MVD_CLOCK:
            eRet = _VDEC_EX_SetMVDClockSpeed((VDEC_EX_ClockSpeed)(*((MS_U32*)(param[0]))));
            break;
        case E_VDEC_EX_V2_USER_CMD_VPU_SECURITY_MODE:
            _VDEC_EX_Enable_VPU_SecurityMode((MS_BOOL)(*((MS_U32*)(param[0]))));
            break;

        case E_VDEC_EX_V2_USER_CMD_PRESET_DECODE_MODE:
            u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
            _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);

            if (pVdecExContext->_Attr[u8Idx].bInit)
            {
                eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
                break;
            }

            if ((*((MS_VIRT*)(param[0]))) == NULL)
            {
                eRet = E_VDEC_EX_RET_INVALID_PARAM;
                break;
            }

            if (((VDEC_EX_DecModCfg *)(*((MS_VIRT*)(param[0]))))->eDecMod != E_VDEC_EX_DEC_MODE_DUAL_INDIE)
            {
                for (i=0; i<VDEC_MAX_SUPPORT_STREAM_NUM; i++)
                {
                    if (TRUE == pVdecExContext->_Attr[i].bInit)
                    {
                        VDEC_PRINT("DecMode must be set before init all tasks (%d)\n", i);
                        eRet = E_VDEC_EX_FAIL;
                        break;
                    }
                }
            }

            pVdecExContext->_Pre_DecModeCfg.eDecMod = ((VDEC_EX_DecModCfg *)(*((MS_VIRT*)(param[0]))))->eDecMod;
            for (i=0; i<VDEC_MAX_DEC_NUM; i++)
            {
                pVdecExContext->_Pre_DecModeCfg.pstCodecInfo[i] = ((VDEC_EX_DecModCfg *)(*((MS_VIRT*)(param[0]))))->pstCodecInfo[i];
            }
            pVdecExContext->_Pre_DecModeCfg.u8CodecCnt = ((VDEC_EX_DecModCfg *)(*((MS_VIRT*)(param[0]))))->u8CodecCnt;
            pVdecExContext->_Pre_DecModeCfg.u8ArgSize  = ((VDEC_EX_DecModCfg *)(*((MS_VIRT*)(param[0]))))->u8ArgSize;
            pVdecExContext->_Pre_DecModeCfg.u32Arg     = ((VDEC_EX_DecModCfg *)(*((MS_VIRT*)(param[0]))))->u32Arg;

            break;

        case E_VDEC_EX_V2_USER_CMD_PRESET_ENABLETURBOMODE:
             eRet = _MApi_VDEC_EX_V2_EnableTurboMode(*((MS_BOOL*)(param[0])));
             break;
        case E_VDEC_EX_V2_USER_CMD_PRESETSINGLEDECODE:
             eRet = _MApi_VDEC_EX_V2_SetSingleDecode(*((MS_BOOL*)(param[0])));
             break;
        case E_VDEC_EX_V2_USER_CMD_PREGETSTATUS:
             eRet = _MApi_VDEC_EX_V2_GetStatus(pStreamId,((VDEC_EX_Status*)(param[0])));
             break;
        case E_VDEC_EX_V2_USER_CMD_SETPOWERSTATE:
             eRet = _MApi_VDEC_EX_V2_SetPowerState((EN_POWER_MODE)(*((MS_U32*)(param[0]))));
             break;

        case E_VDEC_EX_V2_USER_CMD_SET_SHAREMEMORY_BASE:  //common HVD+MVD
             eRet = _VDEC_EX_Set_ShareMemory_Base(pStreamId,(MS_VIRT)(*((MS_VIRT*)(param[0]))));
             break;
        case E_VDEC_EX_V2_USER_CMD_HVD_COL_BBU_MODE:     //HVD
             eRet = _VDEC_EX_SetHVDColBBUMode(pStreamId, (MS_BOOL)(*((MS_VIRT*)(param[0]))));
             break;
        case E_VDEC_EX_V2_USER_CMD_HVD_ONE_PENDING_BUFFER_MODE:  //HVD
             u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
             _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);

             if (pVdecExContext->_Attr[u8Idx].bInit)
             {
                 eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
                 break;
             }
             pVdecExContext->_Pre_Ctrl[u8Idx].bOnePendingBuffer = (MS_BOOL)(*((MS_U32*)(param[0])));
             break;
        case E_VDEC_EX_V2_USER_CMD_HVD_TS_IN_BBU_MODE:
             u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
             _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);

             if (pVdecExContext->_Attr[u8Idx].bInit)
             {
                 eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
                 break;
             }
             pVdecExContext->_Pre_Ctrl[u8Idx].bDisableTspInBbuMode = (MS_BOOL)(*((MS_U32*)(param[0])));
             break;
        case E_VDEC_EX_V2_USER_CMD_MVD_HWBUFFER_REMAPPING_MODE:  //MVD
             u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
             _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);

             if (pVdecExContext->_Attr[u8Idx].bInit)
             {
                 eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
                 break;
             }
             pVdecExContext->_Pre_Ctrl[u8Idx].bHWBufferReMapping = (MS_BOOL)(*((MS_U32*)(param[0])));
             break;

        case E_VDEC_EX_V2_USER_CMD_HVD_IAPGN_BUF_SHARE_BW_MODE:
            u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
            _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);

            if (pVdecExContext->_Attr[u8Idx].bInit)
            {
                eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
                break;
            }

            pVdecExContext->_Pre_Ctrl[u8Idx].stIapGnShBWMode.bEnable = ((VDEC_EX_IapGnBufShareBWMode*)(*((MS_VIRT*)(param[0]))))->bEnable;
            pVdecExContext->_Pre_Ctrl[u8Idx].stIapGnShBWMode.u32IapGnBufAddr= ((VDEC_EX_IapGnBufShareBWMode*)(*((MS_VIRT*)(param[0]))))->u32IapGnBufAddr;
            pVdecExContext->_Pre_Ctrl[u8Idx].stIapGnShBWMode.u32IapGnBufSize= ((VDEC_EX_IapGnBufShareBWMode*)(*((MS_VIRT*)(param[0]))))->u32IapGnBufSize;

            break;
        case E_VDEC_EX_V2_USER_CMD_AUTO_ARRANGE_FRAMEBUFFER_USAGE:
            u32Id = _VDEC_GET_DRV_ID(pStreamId->u32Id);
            _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);

            MDrv_MVD_Dynamic_FB_Mode(u32Id,
                                     ((VDEC_Framebuffer_Ctrl*)(*((MS_VIRT*)(param[0]))))->bEnableFramebufferCtrl,
                                     ((VDEC_Framebuffer_Ctrl*)(*((MS_VIRT*)(param[0]))))->u32FramebufferAddress,
                                     ((VDEC_Framebuffer_Ctrl*)(*((MS_VIRT*)(param[0]))))->u32FramebufferTotalsize);
            break;

        case E_VDEC_EX_V2_USER_CMD_MFCODEC_MODE: //EVD
             u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
             _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);

             if (pVdecExContext->_Attr[u8Idx].bInit)
             {
                 eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
                 break;
             }
             pVdecExContext->_Pre_Ctrl[u8Idx].eMFCodecMode = (VDEC_EX_MFCodec_mode)(*((MS_U32 *)(param[0])));
             break;

        case E_VDEC_EX_V2_USER_CMD_FORCE_8BIT_DEC_MODE: //EVD
             u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
             _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);

             if (pVdecExContext->_Attr[u8Idx].bInit)
             {
                 eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
                 break;
             }
             pVdecExContext->_Pre_Ctrl[u8Idx].bForce8bitMode = (MS_BOOL)(*((MS_U32 *)(param[0])));
             break;

        case E_VDEC_EX_V2_USER_CMD_VDEC_FEATURE:
             u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
             _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);

             if (pVdecExContext->_Attr[u8Idx].bInit)
             {
                 eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
                 break;
             }
             pVdecExContext->_Pre_Ctrl[u8Idx].eVdecFeature = (*((MS_U32 *)(param[0])));
             VDEC_PRINT("E_VDEC_EX_V2_USER_CMD_VDEC_FEATURE 0x%x\n", pVdecExContext->_Pre_Ctrl[u8Idx].eVdecFeature);
             break;

        case E_VDEC_EX_V2_USER_CMD_DYNAMIC_CMA_MODE:
             u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
             _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);

             if (pVdecExContext->_Attr[u8Idx].bInit)
             {
                 eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
                 break;
             }
             pVdecExContext->_Pre_Ctrl[u8Idx].bEnableDynamicCMA = (*((MS_BOOL *)(param[0])));
             VDEC_PRINT("E_VDEC_EX_V2_USER_CMD_DYNAMIC_CMA_MODE 0x%x\n", pVdecExContext->_Pre_Ctrl[u8Idx].bEnableDynamicCMA);
             break;

        case E_VDEC_EX_V2_USER_CMD_SET_DISPLAY_MODE:
             u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
             _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);
             if (pVdecExContext->_Attr[u8Idx].bInit)
             {
                 eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
                 break;
             }
             pVdecExContext->_Pre_Ctrl[u8Idx].eDisplayMode = (*((MS_U32 *)(param[0])));
             if(pVdecExContext->_Pre_Ctrl[u8Idx].eDisplayMode == E_VDEC_EX_DISPLAY_MODE_MCU)
             {
                 pVdecExContext->_bVdecDispOutSide[u8Idx] = TRUE;
             }
             else if(pVdecExContext->_Pre_Ctrl[u8Idx].eDisplayMode == E_VDEC_EX_DISPLAY_MODE_HARDWIRE)
             {
                 pVdecExContext->_bVdecDispOutSide[u8Idx] = FALSE;
             }
            break;
        case E_VDEC_EX_V2_USER_CMD_PRESET_STC:
             eRet = _VDEC_EX_SetSTCMode(pStreamId,(VDEC_EX_STCMode)(*((MS_U32*)(param[0]))));
             break;
        case E_VDEC_EX_V2_USER_CMD_CONNECT_INPUT_TSP:
             break;

        case E_VDEC_EX_V2_USER_CMD_CONNECT_DISPLAY_PATH:
             break;

        case E_VDEC_EX_V2_USER_CMD_BITSTREAMBUFFER_MONOPOLY:
             u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
             _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);
             pVdecExContext->_Pre_Ctrl[u8Idx].bMonopolyBitstreamBuffer = (MS_BOOL)(*((MS_U32 *)(param[0])));
             break;

        case E_VDEC_EX_V2_USER_CMD_FRAMEBUFFER_MONOPOLY:
             u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
             _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);
             pVdecExContext->_Pre_Ctrl[u8Idx].bMonopolyFrameBuffer = (MS_BOOL)(*((MS_U32 *)(param[0])));
             break;

        case E_VDEC_EX_V2_USER_CMD_SPECIFY_TASK_SPEC:
             u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
             _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);
             pVdecExContext->_Pre_Ctrl[u8Idx].eTaskSpec = (VDEC_EX_TASK_SPEC)(*((MS_U32 *)(param[0])));
             break;

        case E_VDEC_EX_V2_USER_CMD_CAL_FRAMERATE:
             u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
             _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);
             pVdecExContext->_Pre_Ctrl[u8Idx].bCalFrameRate = (*((MS_BOOL*)(param[0])));
             break;

        case E_VDEC_EX_V2_USER_CMD_SET_TOTALRANGE_BITSTREAMBUFFER:
             u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
             _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);
             pVdecExContext->_Pre_Ctrl[u8Idx].stTotalBufRgnBS.bSetTotalBuf    = ((VDEC_EX_TotalBufRange*)(*((MS_VIRT*)(param[0]))))->bSetTotalBuf;
             pVdecExContext->_Pre_Ctrl[u8Idx].stTotalBufRgnBS.phyTotalBufAddr = ((VDEC_EX_TotalBufRange*)(*((MS_VIRT*)(param[0]))))->phyTotalBufAddr;
             pVdecExContext->_Pre_Ctrl[u8Idx].stTotalBufRgnBS.szTotalBufSize  = ((VDEC_EX_TotalBufRange*)(*((MS_VIRT*)(param[0]))))->szTotalBufSize;
             break;

        case E_VDEC_EX_V2_USER_CMD_SET_TOTALRANGE_FRAMEBUFFER1:
             u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
             _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);
             pVdecExContext->_Pre_Ctrl[u8Idx].stTotalBufRgnFB1.bSetTotalBuf    = ((VDEC_EX_TotalBufRange*)(*((MS_VIRT*)(param[0]))))->bSetTotalBuf;
             pVdecExContext->_Pre_Ctrl[u8Idx].stTotalBufRgnFB1.phyTotalBufAddr = ((VDEC_EX_TotalBufRange*)(*((MS_VIRT*)(param[0]))))->phyTotalBufAddr;
             pVdecExContext->_Pre_Ctrl[u8Idx].stTotalBufRgnFB1.szTotalBufSize  = ((VDEC_EX_TotalBufRange*)(*((MS_VIRT*)(param[0]))))->szTotalBufSize;
             break;

        case E_VDEC_EX_V2_USER_CMD_SET_TOTALRANGE_FRAMEBUFFER2:
             u8Idx = _VDEC_GET_IDX(pStreamId->u32Id);
             _VDEC_SHM_POINTER_CHECK(E_VDEC_EX_FAIL);
             pVdecExContext->_Pre_Ctrl[u8Idx].stTotalBufRgnFB2.bSetTotalBuf    = ((VDEC_EX_TotalBufRange*)(*((MS_VIRT*)(param[0]))))->bSetTotalBuf;
             pVdecExContext->_Pre_Ctrl[u8Idx].stTotalBufRgnFB2.phyTotalBufAddr = ((VDEC_EX_TotalBufRange*)(*((MS_VIRT*)(param[0]))))->phyTotalBufAddr;
             pVdecExContext->_Pre_Ctrl[u8Idx].stTotalBufRgnFB2.szTotalBufSize  = ((VDEC_EX_TotalBufRange*)(*((MS_VIRT*)(param[0]))))->szTotalBufSize;
             break;

        default:
             eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
             break;
    }
    return eRet;
}

VDEC_EX_Result MApi_VDEC_EX_SystemPostSetControl(VDEC_EX_User_Cmd cmd_id, void *pParam)
{
    VDEC_EX_V2_IO_Param IO_arg;
    VDEC_EX_Result ret;

    if (pu32VDEC_EX_Inst == NULL)
    {
        if(UtopiaOpen(MODULE_VDEC_EX | VDEC_DRIVER_BASE, &pu32VDEC_EX_Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            VPRINTF("UtopiaOpen VDEC_EX failed\n");
            return E_VDEC_EX_FAIL;
        }
    }

    IO_arg.eUserCmd = _VDEC_MapUserCmd_V2(cmd_id);
    IO_arg.param[0] = (pParam);
    IO_arg.pRet     = (void*)(&ret);

    if(UtopiaIoctl(pu32VDEC_EX_Inst,E_VDEC_EX_V2_CMD_POST_SET_CONTROL, (void*)(&IO_arg)) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_EX_FAIL;
    }
    else
    {
        if(cmd_id == E_VDEC_EX_USER_CMD_SET_SECURE_MODE)
        {
            bSecureModeEnableUser = *((MS_BOOL*)pParam);
        }
        return ret;
    }
}


VDEC_EX_Result MApi_VDEC_EX_V2_PostSetControl(VDEC_StreamId* pStreamId,VDEC_EX_V2_User_Cmd cmd_id,void** param)
{
    VDEC_EX_Result eRet = E_VDEC_EX_OK;

    switch (cmd_id)
    {
        case E_VDEC_EX_V2_USER_CMD_SYSTEM_POSTSET_CLEAR_PROCESS_RELATED:
            eRet = _VDEC_EX_ClearProcessRelated(*((MS_U32*)(param[0])));
            break;

        default:
            eRet = E_VDEC_EX_RET_ILLEGAL_ACCESS;
            break;
    }

    return eRet;
}

MS_BOOL _VDEC_EX_Create_BBU_Task(MS_U32 u32streamIdx)
{
    MDrv_HVD_EX_BBU_Proc(u32streamIdx);
    return TRUE;
}

void _VDEC_EX_Delete_BBU_Task(MS_U32 u32streamIdx)
{
    MDrv_HVD_EX_BBU_StopProc(u32streamIdx);
}


#else
// TEE side
#if defined(MSOS_TYPE_NUTTX)
extern int lib_lowVDEC_PRINT(const char *fmt, ...);
#define PRINTF lib_lowVDEC_PRINT
#elif defined(MSOS_TYPE_OPTEE)
#include "drvSEAL.h"
#define PRINTF VDEC_PRINT
#endif

typedef enum
{
    E_VDEC_EX_REE_TO_TEE_MBX_MSG_NULL,
    E_VDEC_EX_REE_TO_TEE_MBX_MSG_FW_LoadCode,
    E_VDEC_EX_REE_TO_TEE_MBX_MSG_GETSHMBASEADDR,
} VDEC_REE_TO_TEE_MBX_MSG_TYPE;

typedef enum
{
    E_VDEC_EX_CODEC_TYPE_NONE = 0,
    E_VDEC_EX_CODEC_TYPE_MPEG2,
    E_VDEC_EX_CODEC_TYPE_H263,
    E_VDEC_EX_CODEC_TYPE_MPEG4,
    E_VDEC_EX_CODEC_TYPE_DIVX311,
    E_VDEC_EX_CODEC_TYPE_DIVX412,
    E_VDEC_EX_CODEC_TYPE_FLV,
    E_VDEC_EX_CODEC_TYPE_VC1_ADV,
    E_VDEC_EX_CODEC_TYPE_VC1_MAIN,
    E_VDEC_EX_CODEC_TYPE_RV8,
    E_VDEC_EX_CODEC_TYPE_RV9,
    E_VDEC_EX_CODEC_TYPE_H264,
    E_VDEC_EX_CODEC_TYPE_AVS,
    E_VDEC_EX_CODEC_TYPE_MJPEG,
    E_VDEC_EX_CODEC_TYPE_MVC,
    E_VDEC_EX_CODEC_TYPE_VP8,
    E_VDEC_EX_CODEC_TYPE_HEVC,
    E_VDEC_EX_CODEC_TYPE_VP9,
    E_VDEC_EX_CODEC_TYPE_HEVC_DV,
    E_VDEC_EX_CODEC_TYPE_H264_DV,
    E_VDEC_EX_CODEC_TYPE_NUM
} VDEC_EX_CodecType;

typedef enum
{
    E_Decode_NONE,
    E_Decode_HVD,
    E_Decode_EVD,
    E_Decode_MVD,
    E_Decode_VP8    //because special ES buffer
} VDEC_EX_DecodeType;
typedef struct
{
    MS_PHY Bitstream_Addr_Main;
    MS_U32 Bitstream_Len_Main;
    MS_PHY Bitstream_Addr_Sub;
    MS_U32 Bitstream_Len_Sub;
    MS_PHY MIU1_BaseAddr;
} VPU_EX_LOCK_DOWN_REGISTER;

typedef struct
{
    MS_PHY  VPUBufAddr;
    MS_SIZE VPUBufSize;
    MS_PHY  BSBufAddr;
    MS_SIZE BSBufSize;
    MS_PHY  FBufAddr;
    MS_SIZE FBufSize;
    MS_U32  u32streamID;
    VDEC_EX_CodecType   eCodecType;
} VDEC_EX_OPTEE_SecureBuffer;

typedef struct
{
    MS_PHY  FBuf2Addr;
    MS_SIZE FBuf2Size;
    MS_U32  u32streamID;
} VDEC_EX_OPTEE_SecureFB2;

typedef struct
{
    MS_PHY  BSBufAddr;
    MS_SIZE BSBufSize;
    MS_U8 stream_ID;
    VDEC_EX_DecodeType Decoder;
} Lock_Register_Info;

#define VDEC_DRIVER_PROC_BUFFER_SIZE 0xA000
MS_PHY VPU_CodeAddr = 0;
MS_U32 VPU_CodeSize = 0;
MS_PHY Bitstream_StartAddr_Main = 0;
MS_U32 Bitstream_Size_Main = 0;
MS_PHY Bitstream_StartAddr_Sub = 0;
MS_U32 Bitstream_Size_Sub = 0;
MS_PHY Framebuffer_StartAddr_Main = 0;
MS_U32 Framebuffer_Size_Main = 0;
MS_PHY Framebuffer_StartAddr_Sub = 0;
MS_U32 Framebuffer_Size_Sub = 0;
MS_PHY VPU_SHMAddr = 0;
MS_U32 VPU_SHMSize = 0; // need at least 256k bytes
MS_PHY MIU1_StartAddr = 0;
MS_PHY Framebuffer2_StartAddr_Main = 0;
MS_U32 Framebuffer2_Size_Main = 0;
MS_PHY Framebuffer2_StartAddr_Sub = 0;
MS_U32 Framebuffer2_Size_Sub = 0;
MS_U8 Secure_Status_Main= 0;
MS_U8 Secure_Status_Sub= 0;
MS_U32 VCodecType_Main= 0;
MS_U32 VCodecType_Sub= 0;

MS_BOOL _VDEC_TEE_Load_FwCode(MS_U32 u32DestAddr)
{
    return MDrv_HVD_EX_LoadCodeInSecure(u32DestAddr);
}

MS_BOOL _VDEC_TEE_Lock_Down_Register(VPU_EX_LOCK_DOWN_REGISTER* param,MS_U8 u8IsHVD)
{
    return MDrv_HVD_EX_SetLockDownRegister((void*)param,u8IsHVD);
}

#if defined(MSOS_TYPE_NUTTX)

typedef enum
{
    E_VDEC_EX_TEE_TO_REE_MBX_MSG_NULL,
    E_VDEC_EX_TEE_TO_REE_MBX_ACK_MSG_INVALID,
    E_VDEC_EX_TEE_TO_REE_MBX_ACK_MSG_NO_TEE,
    E_VDEC_EX_TEE_TO_REE_MBX_ACK_MSG_ACTION_SUCCESS,
    E_VDEC_EX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL
} VDEC_TEE_TO_REE_MBX_ACK_TYPE;

#define VDEC_EX_STACK_SIZE 4096


MS_S32  _s32VDEC_TEE_EventId = -1;
MS_S32  _s32VDEC_TEE_TaskId = -1;
MS_U8   u8VdecExTeeStack[4096];
MBX_Msg VDEC_TEE_msg;
MS_U8 u8VDECMbxMsgClass = 0;
MBX_Msg VDECTeeToReeMbxMsg;

void _VDEC_TEE_Proc(void)
{
    MS_U32 u32Events;
    MS_U8 u8Index = 0;
    MBX_Result result;
    MS_U8 u8IsHVD = 0;

    PRINTF("VDEC TEE_Proc\n");

    while (1)
    {
        MsOS_WaitEvent(_s32VDEC_TEE_EventId, 1, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);

        u8Index = VDEC_TEE_msg.u8Index;
        u8IsHVD = VDEC_TEE_msg.u8Parameters[0];

        memset(&VDECTeeToReeMbxMsg,0,sizeof(MBX_Msg));
        VDECTeeToReeMbxMsg.eRoleID = E_MBX_CPU_MIPS;
        VDECTeeToReeMbxMsg.u8Ctrl = 0;
        VDECTeeToReeMbxMsg.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        VDECTeeToReeMbxMsg.u8MsgClass = VDEC_TEE_msg.u8MsgClass;
        VDECTeeToReeMbxMsg.u8Index = E_VDEC_EX_TEE_TO_REE_MBX_ACK_MSG_ACTION_SUCCESS;

        switch(u8Index)
        {
            case E_VDEC_EX_REE_TO_TEE_MBX_MSG_FW_LoadCode:
                PRINTF("VDEC receive load f/w code in R2\n");

                if(VPU_CodeAddr == 0)
                {
                    VDECTeeToReeMbxMsg.u8Index = E_VDEC_EX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                }
                else
                {
                    if (_VDEC_TEE_Load_FwCode(MsOS_PA2KSEG1(VPU_CodeAddr)) == FALSE)
                    {
                        VDECTeeToReeMbxMsg.u8Index = E_VDEC_EX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                    }
                    else
                    {
                        VPU_EX_LOCK_DOWN_REGISTER register_lockdown;
                        register_lockdown.Bitstream_Addr_Main = Bitstream_StartAddr_Main;
                        register_lockdown.Bitstream_Len_Main  = Bitstream_Size_Main;

                        register_lockdown.Bitstream_Addr_Sub  = Bitstream_StartAddr_Sub;
                        register_lockdown.Bitstream_Len_Sub   = Bitstream_Size_Sub;
                        register_lockdown.MIU1_BaseAddr       = MIU1_StartAddr;

                        if(_VDEC_TEE_Lock_Down_Register(&register_lockdown,u8IsHVD) == TRUE)
                        {
                            PRINTF("VDEC receive load f/w code success in R2\n");
                        }
                    }
                }
                break;
            case E_VDEC_EX_REE_TO_TEE_MBX_MSG_GETSHMBASEADDR:
                PRINTF("VDEC receive get shm address in R2\n");
                if((VPU_SHMAddr == 0) || (VPU_SHMAddr <= VPU_CodeAddr) || (VPU_SHMSize < 0x40000))
                {
                    PRINTF("[VDEC]%s,share memory base (%x,%x) is not ready\n",__FUNCTION__,(unsigned int)VPU_SHMAddr,(unsigned int)VPU_CodeAddr);
                    VDECTeeToReeMbxMsg.u8Index = E_VDEC_EX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                }
                else
                {
                    MS_U32 SHM_offset;
                    if(VPU_SHMAddr >= MIU1_StartAddr)
                    {
                        SHM_offset = VPU_SHMAddr-MIU1_StartAddr;
                        VDECTeeToReeMbxMsg.u8Parameters[8] = 1;  // miu1
                    }
                    else
                    {
                        SHM_offset = VPU_SHMAddr;
                        VDECTeeToReeMbxMsg.u8Parameters[8] = 0;  // miu0
                    }

                    VDECTeeToReeMbxMsg.u8Index = E_VDEC_EX_TEE_TO_REE_MBX_ACK_MSG_ACTION_SUCCESS;
                    VDECTeeToReeMbxMsg.u8Parameters[0] =  SHM_offset&0xff;
                    VDECTeeToReeMbxMsg.u8Parameters[1] = (SHM_offset>>8)&0xff;
                    VDECTeeToReeMbxMsg.u8Parameters[2] = (SHM_offset>>16)&0xff;
                    VDECTeeToReeMbxMsg.u8Parameters[3] = (SHM_offset>>24)&0xff;
                    VDECTeeToReeMbxMsg.u8Parameters[4] =  VPU_SHMSize&0xff;
                    VDECTeeToReeMbxMsg.u8Parameters[5] = (VPU_SHMSize>>8)&0xff;
                    VDECTeeToReeMbxMsg.u8Parameters[6] = (VPU_SHMSize>>16)&0xff;
                    VDECTeeToReeMbxMsg.u8Parameters[7] = (VPU_SHMSize>>24)&0xff;
                    VDECTeeToReeMbxMsg.u8ParameterCount = 9;

                    PRINTF("VDEC receive get shm address success in R2\n");
                }
                break;
            default:
                VDECTeeToReeMbxMsg.u8Index = E_VDEC_EX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                PRINTF("[VDEC]%s,wrong command %d sent from HK\n",__FUNCTION__,(unsigned int)u8Index);
                break;
        }

        result = MApi_MBX_SendMsg(&VDECTeeToReeMbxMsg);

#if 0
        if (E_MBX_SUCCESS != result)
        {
            PRINTF("[VDEC]Ack fail,classid:%x,cmd:%x\n",VDEC_TEE_msg.u8MsgClass, VDECTeeToReeMbxMsg.u8Index);
        }
        else
        {
            PRINTF("[VDEC]Ack ok,classid:%x\n",VDEC_TEE_msg.u8MsgClass);
        }
#endif
    }
}


void _VDEC_TEE_HandleDynamicMBX(MBX_Msg *pMsg, MS_BOOL *pbAddToQueue)
{
    if(pMsg==NULL)
    {
        PRINTF("[VDEC]%s,pMsg is a null pointer\n",__FUNCTION__);
        return;
    }

    if (_s32VDEC_TEE_EventId < 0)
    {
        PRINTF("[VDEC]%s,VDEC TEE event group is not created\n",__FUNCTION__);
        return;
    }
    memcpy(&VDEC_TEE_msg, pMsg, sizeof(MBX_Msg));
    MsOS_SetEvent(_s32VDEC_TEE_EventId, 1);
    *pbAddToQueue = FALSE;
    return;
}

//------------------------------------------------------------------------------
/// @brief Create task for TEE, protect VDEC from hacking
/// @retval TRUE/FALSE
//------------------------------------------------------------------------------
MS_BOOL MApi_VDEC_EX_Create_Tee_System(void)
{
    if(u8VDECMbxMsgClass == 0)
    {
        if(MApi_MBX_GenerateDynamicClass(E_MBX_CPU_MIPS, "VDEC_TEE",&u8VDECMbxMsgClass) != E_MBX_SUCCESS)
        {
            PRINTF("[VDEC]TEE GenerateDynamicClass error\n");
            u8VDECMbxMsgClass = 0;
            return FALSE;
        }

        if ( E_MBX_SUCCESS != MApi_MBX_RegisterMSGWithCallBack(u8VDECMbxMsgClass,10,_VDEC_TEE_HandleDynamicMBX))
        {
            PRINTF("[VDEC]TEE Register MBX MSG error\n");
            u8VDECMbxMsgClass = 0;
            return FALSE;
        }
    }

    if(_s32VDEC_TEE_EventId == -1)
    {
        _s32VDEC_TEE_EventId = MsOS_CreateEventGroup("VDEC_TEE_Event");
        if (_s32VDEC_TEE_EventId < 0)
        {
            PRINTF("[VDEC]TEE CreateEventGroup error....\n");
            return FALSE;
        }
    }

    if(_s32VDEC_TEE_TaskId == -1)
    {
        _s32VDEC_TEE_TaskId = MsOS_CreateTask((TaskEntry)_VDEC_TEE_Proc,
                                              NULL,
                                              E_TASK_PRI_MEDIUM,
                                              TRUE,
                                              (void*)u8VdecExTeeStack,
                                              VDEC_EX_STACK_SIZE,
                                              "VDEC_TEE_Task");
        if (_s32VDEC_TEE_TaskId < 0)
        {
            MsOS_DeleteEventGroup(_s32VDEC_TEE_EventId);
            PRINTF("[VDEC]TEE CreateTask error....\n");
            return FALSE;
        }
    }
    return TRUE;
}
#endif

#if defined(MSOS_TYPE_OPTEE)
void _VDEC_EX_OPTEE_ResetBuffer(MS_U8 stream_ID)
{
    if(stream_ID ==0)
    {
        Bitstream_StartAddr_Main=0;
        Bitstream_Size_Main=0;
        Framebuffer_StartAddr_Main=0;
        Framebuffer_Size_Main=0;
        Framebuffer2_StartAddr_Main=0;
        Framebuffer2_Size_Main=0;
    }
    else if(stream_ID == 1)
    {
        Bitstream_StartAddr_Sub=0;
        Bitstream_Size_Sub=0;
        Framebuffer_StartAddr_Sub=0;
        Framebuffer_Size_Sub=0;
        Framebuffer2_StartAddr_Sub=0;
        Framebuffer2_Size_Sub=0;
    }
}

MS_BOOL _VDEC_EX_OPTEE_SET_SecureBuffer(VDEC_EX_OPTEE_SecureBuffer *buffer)
{
    MS_U8 stream_ID = (MS_U8)(buffer->u32streamID >> 24)&0xff;
    VDEC_PRINT("[VDEC][OPTEE]%s,stream_id=%d\n",__FUNCTION__,stream_ID);

    VPU_CodeAddr = buffer->VPUBufAddr;
    VPU_CodeSize = buffer->VPUBufSize;

    if(stream_ID ==0)
    {
        Bitstream_StartAddr_Main = buffer->BSBufAddr;
        Bitstream_Size_Main = buffer->BSBufSize;
        Framebuffer_StartAddr_Main= buffer->FBufAddr;
        Framebuffer_Size_Main= buffer->FBufSize;
        VCodecType_Main= buffer->eCodecType;
    }
    else if(stream_ID ==1)
    {
        Bitstream_StartAddr_Sub = buffer->BSBufAddr;
        Bitstream_Size_Sub = buffer->BSBufSize;
        Framebuffer_StartAddr_Sub = buffer->FBufAddr;
        Framebuffer_Size_Sub = buffer->FBufSize;
        VCodecType_Sub= buffer->eCodecType;
    }
    return TRUE;
}

MS_BOOL _VDEC_EX_OPTEE_SET_SecureFB2(VDEC_EX_OPTEE_SecureFB2 *buffer)
{
    MS_U8 stream_ID = (MS_U8)(buffer->u32streamID >> 24)&0xff;
    VDEC_PRINT("[VDEC][OPTEE]%s,stream_id=%d\n",__FUNCTION__,stream_ID);

    if(stream_ID ==0)
    {
        Framebuffer2_StartAddr_Main = buffer->FBuf2Addr;
        Framebuffer2_Size_Main = buffer->FBuf2Size;
    }
    else if(stream_ID ==1)
    {
        Framebuffer2_StartAddr_Sub = buffer->FBuf2Addr;
        Framebuffer2_Size_Sub= buffer->FBuf2Size;
    }
    return TRUE;
}

MS_U32 MDrv_VDEC_GetResourceByPipeNNum(MS_U64 u64PipeID, VDEC_EX_OPTEE_Buffer ResEnum, VDEC_EX_RESOURCE* pResouce)
{
    MS_U8 stream_ID = (MS_U8)((u64PipeID >> 24)&0xff);

    if(ResEnum == E_VDEC_VPU)
    {
        pResouce->ID[0]= VPU_CodeSize;
        pResouce->ID[1]= (VPU_CodeAddr& 0xFFFFFFFF);
        pResouce->ID[2]= ((VPU_CodeAddr>>32)& 0xFFFFFFFF);
        pResouce->ID[3]= 0;

        return UTOPIA_STATUS_SUCCESS;
    }
    if(stream_ID == 0)
    {
        if(ResEnum == E_VDEC_BS)
        {
            pResouce->ID[0]= Bitstream_Size_Main;
            pResouce->ID[1]= (Bitstream_StartAddr_Main& 0xFFFFFFFF);
            pResouce->ID[2]= ((Bitstream_StartAddr_Main>>32) & 0xFFFFFFFF);
            pResouce->ID[3]= 0;

            return UTOPIA_STATUS_SUCCESS;
        }
        else if(ResEnum == E_VDEC_FB)
        {
            pResouce->ID[0]= Framebuffer_Size_Main;
            pResouce->ID[1]= (Framebuffer_StartAddr_Main & 0xFFFFFFFF);
            pResouce->ID[2]= ((Framebuffer_StartAddr_Main>>32) & 0xFFFFFFFF);
            pResouce->ID[3]= 0;

            return UTOPIA_STATUS_SUCCESS;
        }
        else if(ResEnum == E_VDEC_FB2)
        {
            pResouce->ID[0]= Framebuffer2_Size_Main;
            pResouce->ID[1]= (Framebuffer2_StartAddr_Main& 0xFFFFFFFF);
            pResouce->ID[2]= ((Framebuffer2_StartAddr_Main>>32)& 0xFFFFFFFF);
            pResouce->ID[3]= 0;

            return UTOPIA_STATUS_SUCCESS;
        }
    }
    else if(stream_ID == 1)
    {
        if(ResEnum == E_VDEC_BS)
        {
            pResouce->ID[0]= Bitstream_Size_Sub;
            pResouce->ID[1]= (Bitstream_StartAddr_Sub& 0xFFFFFFFF);
            pResouce->ID[2]= ((Bitstream_StartAddr_Sub>>32) & 0xFFFFFFFF);
            pResouce->ID[3]= 0;

            return UTOPIA_STATUS_SUCCESS;
        }
        else if(ResEnum == E_VDEC_FB)
        {
            pResouce->ID[0]= Framebuffer_Size_Sub;
            pResouce->ID[1]= (Framebuffer_StartAddr_Sub & 0xFFFFFFFF);
            pResouce->ID[2]= ((Framebuffer_StartAddr_Sub>>32) & 0xFFFFFFFF);
            pResouce->ID[3]= 0;

            return UTOPIA_STATUS_SUCCESS;
        }
        else if(ResEnum == E_VDEC_FB2)
        {
            pResouce->ID[0]= Framebuffer2_Size_Sub;
            pResouce->ID[1]= (Framebuffer2_StartAddr_Sub& 0xFFFFFFFF);
            pResouce->ID[2]= ((Framebuffer2_StartAddr_Sub>>32)& 0xFFFFFFFF);
            pResouce->ID[3]= 0;

            return UTOPIA_STATUS_SUCCESS;
        }
    }
    return UTOPIA_STATUS_FAIL;
}

MS_U32 MDrv_VDEC_GetResourceByPipeID(MS_U64 u64PipeID, MS_U32*  U32MaxNRetResNum, VDEC_EX_RESOURCE* pResouce)
{
    MS_U8 stream_ID = (MS_U8)((u64PipeID >> 24)&0xff);
    //VDEC_PRINT("[VDEC][OPTEE]%s,stream_id=%d\n",__FUNCTION__,stream_ID);

    if(*U32MaxNRetResNum < 4)
    {
        VDEC_PRINT("[VDEC][OPTEE]Resource Number too less=%d\n",(int)*U32MaxNRetResNum);
        return UTOPIA_STATUS_FAIL;
    }

    if(pResouce == NULL)
    {
        VDEC_PRINT("[VDEC][OPTEE]Resource NULL\n");    //hsiaochi
        return UTOPIA_STATUS_FAIL;
    }

    if(stream_ID == 0)
    {
        pResouce[0].ID[0]= Bitstream_Size_Main;
        pResouce[0].ID[1]= (Bitstream_StartAddr_Main& 0xFFFFFFFF);
        pResouce[0].ID[2]= ((Bitstream_StartAddr_Main>>32) & 0xFFFFFFFF);
        pResouce[0].ID[3]= 0;
        pResouce[1].ID[0]= Framebuffer_Size_Main;
        pResouce[1].ID[1]= (Framebuffer_StartAddr_Main & 0xFFFFFFFF);
        pResouce[1].ID[2]= ((Framebuffer_StartAddr_Main>>32) & 0xFFFFFFFF);
        pResouce[1].ID[3]= 0;

        *U32MaxNRetResNum = 2;

        if(Framebuffer2_Size_Main>0)
        {
            pResouce[2].ID[0]= Framebuffer2_Size_Main;
            pResouce[2].ID[1]= (Framebuffer2_StartAddr_Main& 0xFFFFFFFF);
            pResouce[2].ID[2]= ((Framebuffer2_StartAddr_Main>>32)& 0xFFFFFFFF);
            pResouce[2].ID[3]= 0;

            *U32MaxNRetResNum = 3;
        }
    }
    else if(stream_ID == 1)
    {
        pResouce[0].ID[0]= Bitstream_Size_Sub;
        pResouce[0].ID[1]= (Bitstream_StartAddr_Sub& 0xFFFFFFFF);
        pResouce[0].ID[2]= ((Bitstream_StartAddr_Sub>>32) & 0xFFFFFFFF);
        pResouce[0].ID[3]= 0;
        pResouce[1].ID[0]= (Framebuffer_Size_Sub&  0xFFFFFFFF);
        pResouce[1].ID[1]= (Framebuffer_StartAddr_Sub & 0xFFFFFFFF) ;
        pResouce[1].ID[2]= ((Framebuffer_StartAddr_Sub>>32) & 0xFFFFFFFF);
        pResouce[1].ID[3]= 0;

        *U32MaxNRetResNum = 2;

        if(Framebuffer2_Size_Sub>0)
        {
            pResouce[2].ID[0]= Framebuffer2_Size_Sub;
            pResouce[2].ID[1]= (Framebuffer2_StartAddr_Sub& 0xFFFFFFFF);
            pResouce[2].ID[2]= ((Framebuffer2_StartAddr_Sub>>32) & 0xFFFFFFFF);
            pResouce[2].ID[3]= 0;

            *U32MaxNRetResNum = 3;
        }
    }

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 _VDEC_EX_OPTEE_LockRegister(Lock_Register_Info *Lock_info, MS_U32 u32OperationMode)
{
    switch(Lock_info->Decoder)
    {
        case E_Decode_HVD:
            if(u32OperationMode==1)
            {
                MDrv_HVD_EX_OPTEE_SetRegister(Lock_info);
            }
            if(Lock_info->stream_ID ==0)
            {
                MDrv_Seal_BufferLock(E_SEAL_HVD_ES0_BUF, u32OperationMode);    //lock register
                VDEC_PRINT("[VDEC][OPTEE]%d:lock HVD register\n",Lock_info->stream_ID);
            }
            else if(Lock_info->stream_ID ==1)
            {
                MDrv_Seal_BufferLock(E_SEAL_HVD_ES1_BUF, u32OperationMode);    //lock register
                VDEC_PRINT("[VDEC][OPTEE]%d:lock HVD register\n",Lock_info->stream_ID);
            }
            break;
        case E_Decode_VP8:
            if(u32OperationMode==1)
            {
                MDrv_HVD_EX_OPTEE_SetRegister(Lock_info);
            }
            MDrv_Seal_BufferLock(E_SEAL_HVD_ES2_BUF, u32OperationMode);    //lock register
            VDEC_PRINT("[VDEC][OPTEE]%d:lock VP8 register\n",Lock_info->stream_ID);
            break;
        case E_Decode_EVD:
            if(u32OperationMode==1)
            {
                MDrv_HVD_EX_OPTEE_SetRegister(Lock_info);
            }
            if(Lock_info->stream_ID ==0)
            {
                MDrv_Seal_BufferLock(E_SEAL_EVD_0_WR_PROTN_0, u32OperationMode);    //lock register
                VDEC_PRINT("[VDEC][OPTEE]%d:lock EVD register\n",Lock_info->stream_ID);
            }
            else if(Lock_info->stream_ID ==1)
            {
                MDrv_Seal_BufferLock(E_SEAL_EVD_0_WR_PROTN_1, u32OperationMode);    //lock register
                VDEC_PRINT("[VDEC][OPTEE]%d:lock EVD register\n",Lock_info->stream_ID);
            }
            break;
        default:
            break;
    }
    return TRUE;
}

MS_U32 _VDEC_EX_OPTEE_GetDecoderByCodecType(VDEC_EX_CodecType codetype)
{
    MS_U32 Decode_type;

    switch(codetype)
    {
        case E_VDEC_EX_CODEC_TYPE_H264:
        case E_VDEC_EX_CODEC_TYPE_RV8:
        case E_VDEC_EX_CODEC_TYPE_RV9:
        case E_VDEC_EX_CODEC_TYPE_AVS:
            Decode_type = E_Decode_HVD;
            break;
        case E_VDEC_EX_CODEC_TYPE_VP8:
            Decode_type = E_Decode_VP8;
            break;
        case E_VDEC_EX_CODEC_TYPE_HEVC:
        case E_VDEC_EX_CODEC_TYPE_VP9:
        case E_VDEC_EX_CODEC_TYPE_HEVC_DV:
            Decode_type = E_Decode_EVD;
            break;
        case E_VDEC_EX_CODEC_TYPE_MPEG2:
        case E_VDEC_EX_CODEC_TYPE_H263:
        case E_VDEC_EX_CODEC_TYPE_MPEG4:
        case E_VDEC_EX_CODEC_TYPE_DIVX311:
        case E_VDEC_EX_CODEC_TYPE_DIVX412:
        case E_VDEC_EX_CODEC_TYPE_FLV:
        case E_VDEC_EX_CODEC_TYPE_VC1_ADV:
        case E_VDEC_EX_CODEC_TYPE_VC1_MAIN:
            Decode_type = E_Decode_MVD;
            break;
        default:
            Decode_type = E_Decode_NONE;
            break;
    }

    return Decode_type;
}

MS_U32 MDrv_VDEC_ConfigPipe(MS_U64 u64PipeID, MS_U32 u32SecureDMA, MS_U32 u32OperationMode)
{
    MS_U8 stream_ID = (MS_U8)((u64PipeID >> 24)&0xff);
    Lock_Register_Info Lock_info;
    Secure_Status_Main=0;
    Secure_Status_Sub=0;
    VDEC_PRINT("[VDEC][OPTEE]%s,stream_id=%d\n",__FUNCTION__,stream_ID);
    VDEC_PRINT("[VDEC][OPTEE]secureDMA=%d,opeation=%d\n",u32SecureDMA,u32OperationMode);

    if(u32SecureDMA)
    {
        MDrv_Seal_ChangeIPSecureDMAAbillity(MODULE_VDEC_EX, E_SEAL_IPGROUP_VDEC_DECODER , u32SecureDMA);    //VDEC
        MDrv_Seal_ChangeIPSecureDMAAbillity(MODULE_VDEC_EX, E_SEAL_IPGROUP_VDEC_R2 , u32SecureDMA);   //VDEC R2
        MDrv_Seal_ChangeIPSecureDMAAbillity(MODULE_MVOP, 0 , u32SecureDMA);       //MVOP
    }

    //if(MDrv_Seal_SetSecureRange((VPU_CodeAddr+0x1000), (VPU_CodeSize-0x1000), u32OperationMode) == FALSE)
    //    return UTOPIA_STATUS_FAIL ;

    if(stream_ID == 0)
    {
        if(MDrv_Seal_SetSecureRange(Bitstream_StartAddr_Main, Bitstream_Size_Main, u32OperationMode) == FALSE)
           return UTOPIA_STATUS_FAIL;
        if(MDrv_Seal_SetSecureRange(Framebuffer_StartAddr_Main, Framebuffer_Size_Main, u32OperationMode) == FALSE)
           return UTOPIA_STATUS_FAIL;

        if( Framebuffer2_Size_Main>0)
        {
           if(MDrv_Seal_SetSecureRange(Framebuffer2_StartAddr_Main, Framebuffer2_Size_Main, u32OperationMode) == FALSE)
           return UTOPIA_STATUS_FAIL;
        }

        //lock register start
        VDEC_PRINT("[VDEC][OPTEE]%d:lock register start\n",stream_ID);
        Lock_info.BSBufAddr = Bitstream_StartAddr_Main;
        Lock_info.BSBufSize = Bitstream_Size_Main;
        Lock_info.stream_ID = stream_ID;
        Lock_info.Decoder = _VDEC_EX_OPTEE_GetDecoderByCodecType(VCodecType_Main);
        VDEC_PRINT("[VDEC][OPTEE]%d:Decoder_type=%x\n",stream_ID,Lock_info.Decoder);
        _VDEC_EX_OPTEE_LockRegister(&Lock_info,u32OperationMode);
        VDEC_PRINT("[VDEC][OPTEE]%d:lock register end\n",stream_ID);
        //lock register end

        Secure_Status_Main=1;
    }
    else if(stream_ID == 1)
    {
        if(MDrv_Seal_SetSecureRange(Bitstream_StartAddr_Sub, Bitstream_Size_Sub, u32OperationMode) == FALSE)
           return UTOPIA_STATUS_FAIL;
        if(MDrv_Seal_SetSecureRange(Framebuffer_StartAddr_Sub, Framebuffer_Size_Sub, u32OperationMode) == FALSE)
           return UTOPIA_STATUS_FAIL;

        if( Framebuffer2_Size_Sub>0)
        {
           if(MDrv_Seal_SetSecureRange(Framebuffer2_StartAddr_Sub, Framebuffer2_Size_Sub, u32OperationMode) == FALSE)
           return UTOPIA_STATUS_FAIL;
        }

        //lock register start
        VDEC_PRINT("[VDEC][OPTEE]%d:lock register start\n",stream_ID);
        Lock_info.BSBufAddr = Bitstream_StartAddr_Sub;
        Lock_info.BSBufSize = Bitstream_Size_Sub;
        Lock_info.stream_ID = stream_ID;
        Lock_info.Decoder = _VDEC_EX_OPTEE_GetDecoderByCodecType(VCodecType_Sub);
        VDEC_PRINT("[VDEC][OPTEE]%d:Decoder_type=%x\n",stream_ID,Lock_info.Decoder);
        _VDEC_EX_OPTEE_LockRegister(&Lock_info,u32OperationMode);
        VDEC_PRINT("[VDEC][OPTEE]%d:lock register end\n",stream_ID);
        //lock register end

        Secure_Status_Sub=1;
    }

    if(!u32SecureDMA)
    {
        MDrv_Seal_ChangeIPSecureDMAAbillity(MODULE_VDEC_EX, E_SEAL_IPGROUP_VDEC_DECODER , u32SecureDMA);    //VDEC
        MDrv_Seal_ChangeIPSecureDMAAbillity(MODULE_VDEC_EX, E_SEAL_IPGROUP_VDEC_R2 , u32SecureDMA);   //VDEC R2
        MDrv_Seal_ChangeIPSecureDMAAbillity(MODULE_MVOP, 0 , u32SecureDMA);       //MVOP
    }

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 MDrv_VDEC_CheckPipe(MS_U64 u64PipeID, MS_U32 u32SecureDMA, MS_U32 u32OperationMode)
{
    MS_U8 stream_ID = (MS_U8)((u64PipeID >> 24)&0xff);
    //VDEC_PRINT("[VDEC][OPTEE]%s,stream_id=%d\n",__FUNCTION__,stream_ID);

    if(stream_ID==0 && (!Secure_Status_Main))
    {
        VDEC_PRINT("[VDEC][OPTEE]stream0 check pipe fail\n");
        return UTOPIA_STATUS_FAIL;
    }
    else if(stream_ID==1 && (!Secure_Status_Sub))
    {
        VDEC_PRINT("[VDEC][OPTEE]stream1 check pipe fail\n");
        return UTOPIA_STATUS_FAIL;
    }

    return UTOPIA_STATUS_SUCCESS;
}
#endif

MS_BOOL _VDEC_EX_TEE_VPU_SETTING(VDEC_EX_TEE_VPU_Param* param)
{
    if(param == NULL)
    {
        return FALSE;
    }
    else
    {
        VPU_CodeAddr                 = param->u32VPUStartAddr;
        VPU_CodeSize                 = param->u32VPUCodeSize;
        Framebuffer_StartAddr_Main   = param->u32FrameBuffertartAddrMain;
        Framebuffer_Size_Main        = param->u32FrameBufferSizeMain;
        Bitstream_StartAddr_Main     = param->u32BitstreamStartAddrMain;
        Bitstream_Size_Main          = param->u32BitstreamSizeMain;
        Framebuffer_StartAddr_Sub    = param->u32FrameBuffertartAddrSub;
        Framebuffer_Size_Sub         = param->u32FrameBufferSizeSub;
        Bitstream_StartAddr_Sub      = param->u32BitstreamStartAddrSub;
        Bitstream_Size_Sub           = param->u32BitstreamSizeSub;
        VPU_SHMAddr                  = param->u32VPUSHMAddr;
        VPU_SHMSize                  = param->u32VPUSHMSize;
        MIU1_StartAddr               = param->u32MIU1StartAddr;

#if 1
        PRINTF("VDEC VPU_CodeAddr %x\n",(unsigned int)VPU_CodeAddr);
        PRINTF("VDEC VPU_CodeSize %x\n",(unsigned int)VPU_CodeSize);
        PRINTF("VDEC Framebuffer_StartAddr_Main %x\n",(unsigned int)Framebuffer_StartAddr_Main);
        PRINTF("VDEC Framebuffer_Size_Main %x\n",(unsigned int)Framebuffer_Size_Main);
        PRINTF("VDEC Bitstream_StartAddr_Main %x\n",(unsigned int)Bitstream_StartAddr_Main);
        PRINTF("VDEC Bitstream_Size_Main %x\n",(unsigned int)Bitstream_Size_Main);
        PRINTF("VDEC Framebuffer_StartAddr_Sub %x\n",(unsigned int)Framebuffer_StartAddr_Sub);
        PRINTF("VDEC Framebuffer_Size_Sub %x\n",(unsigned int)Framebuffer_Size_Sub);
        PRINTF("VDEC Bitstream_StartAddr_Sub %x\n",(unsigned int)Bitstream_StartAddr_Sub);
        PRINTF("VDEC Bitstream_Size_Sub %x\n",(unsigned int)Bitstream_Size_Sub);
        PRINTF("VDEC VPU_SHMAddr %x\n",(unsigned int)VPU_SHMAddr);
        PRINTF("VDEC VPU_SHMSize %x\n",(unsigned int)VPU_SHMSize);
        PRINTF("VDEC MIU1_StartAddr %x\n",(unsigned int)MIU1_StartAddr);
#endif
        return TRUE;
    }
}


MS_BOOL _VDEC_EX_TEE_GetVPUProtect(VDEC_EX_TEE_VPU_Param* param)
{
    if(param == NULL)
    {
        return FALSE;
    }
    else
    {
        param->u32VPUStartAddr = VPU_CodeAddr;
        param->u32VPUSHMAddr = VPU_SHMAddr;
        param->u32VPUSHMSize = VPU_SHMSize;
        param->u32MIU1StartAddr = MIU1_StartAddr;
        if(Bitstream_StartAddr_Main != 0 && Bitstream_StartAddr_Sub != 0)
        {
            param->u32ProtectNum = 3;

            param->ProtectInfo[0].u32Addr = VPU_CodeAddr + 0x1000; // buffer protect is 4k alignment;
            param->ProtectInfo[0].u32Size = VPU_CodeSize-0x1000;

            param->ProtectInfo[1].u32Addr = Bitstream_StartAddr_Main + VDEC_DRIVER_PROC_BUFFER_SIZE;
            param->ProtectInfo[1].u32Size = Bitstream_Size_Main-VDEC_DRIVER_PROC_BUFFER_SIZE;

            param->ProtectInfo[2].u32Addr = Bitstream_StartAddr_Sub + VDEC_DRIVER_PROC_BUFFER_SIZE; // buffer protect is 4k alignment;
            param->ProtectInfo[2].u32Size = Bitstream_Size_Sub-VDEC_DRIVER_PROC_BUFFER_SIZE;
            #if 0
            PRINTF("ProtectInfo[0].u32Addr %x\n",(unsigned int)param->ProtectInfo[0].u32Addr);
            PRINTF("ProtectInfo[0].u32Size %x\n",(unsigned int)param->ProtectInfo[0].u32Size);
            PRINTF("ProtectInfo[1].u32Addr %x\n",(unsigned int)param->ProtectInfo[1].u32Addr);
            PRINTF("ProtectInfo[1].u32Size %x\n",(unsigned int)param->ProtectInfo[1].u32Size);
            PRINTF("ProtectInfo[2].u32Addr %x\n",(unsigned int)param->ProtectInfo[2].u32Addr);
            PRINTF("ProtectInfo[2].u32Size %x\n",(unsigned int)param->ProtectInfo[2].u32Size);
            #endif
        }
        else if(Bitstream_StartAddr_Main != 0)
        {
            param->u32ProtectNum = 2;

            param->ProtectInfo[0].u32Addr = VPU_CodeAddr + 0x1000; // buffer protect is 4k alignment;
            param->ProtectInfo[0].u32Size = VPU_CodeSize-0x1000;

            param->ProtectInfo[1].u32Addr = Bitstream_StartAddr_Main + VDEC_DRIVER_PROC_BUFFER_SIZE;
            param->ProtectInfo[1].u32Size = Bitstream_Size_Main-VDEC_DRIVER_PROC_BUFFER_SIZE;

            #if 0
            PRINTF("ProtectInfo[0].u32Addr %x\n",(unsigned int)param->ProtectInfo[0].u32Addr);
            PRINTF("ProtectInfo[0].u32Size %x\n",(unsigned int)param->ProtectInfo[0].u32Size);
            PRINTF("ProtectInfo[1].u32Addr %x\n",(unsigned int)param->ProtectInfo[1].u32Addr);
            PRINTF("ProtectInfo[1].u32Size %x\n",(unsigned int)param->ProtectInfo[1].u32Size);
            #endif
        }
        else
        {
            PRINTF("VDEC MMAP is wrong\n");
            return FALSE;
        }

        return TRUE;
    }
}

//------------------------------------------------------------------------------
/// @brief Set parameter for TEE
/// @param cmd E_VDEC_EX_SET_VPU_SETTING
/// @param cmd E_VDEC_EX_GET_VPU_PROTECT_START_ADDR
/// @param param buffer address and size
/// @retval TRUE/FALSE : success/fail
//------------------------------------------------------------------------------
MS_BOOL MApi_VDEC_EX_TEE_SetControl(VDEC_EX_TEE_USER_CMD cmd,void* param)
{
    MS_BOOL ret = FALSE;

    if(param == NULL)
    {
        PRINTF("[VDEC]%s,Invalid param\n",__FUNCTION__);
    }

    switch(cmd)
    {
        case E_VDEC_EX_SET_VPU_SETTING:
            ret = _VDEC_EX_TEE_VPU_SETTING((VDEC_EX_TEE_VPU_Param*)param);
            break;
        case E_VDEC_EX_SET_VPU_LOADCODE:
            if(VPU_CodeAddr != 0)
            {
                PRINTF("%s,%d,PA:%x VA:%x\n",__FUNCTION__,__LINE__,(MS_U32)VPU_CodeAddr,(MS_U32)MsOS_PA2KSEG1(VPU_CodeAddr));
                if (_VDEC_TEE_Load_FwCode(MsOS_PA2KSEG1(VPU_CodeAddr)) == TRUE)
                {
                    PRINTF("VDEC receive load f/w code success in optee\n");
                    ret = TRUE;
                }
                else
                {
                    PRINTF("VDEC receive load f/w code fail in optee\n");
                }
            }
            else
            {
                PRINTF("VDEC receive load f/w code fail in optee due to code address is zero\n");
            }
            break;
#if defined(MSOS_TYPE_OPTEE)
        case E_VDEC_EX_SET_SECURE_BUFFER:
            {
                ret = _VDEC_EX_OPTEE_SET_SecureBuffer((VDEC_EX_OPTEE_SecureBuffer*)param);
            }
            break;
        case E_VDEC_EX_SET_SECURE_FB2:
            {
                ret = _VDEC_EX_OPTEE_SET_SecureFB2((VDEC_EX_OPTEE_SecureFB2*)param);
            }
            break;
#endif
        default:
            PRINTF("[VDEC]Invalid user cmd,%d\n",(unsigned int)cmd);
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// @brief Get parameter for TEE
/// @param cmd E_VDEC_EX_SET_VPU_SETTING
/// @param cmd E_VDEC_EX_GET_VPU_PROTECT_START_ADDR
/// @param param buffer address and size
/// @retval TRUE/FALSE : success/fail
//------------------------------------------------------------------------------
MS_BOOL MApi_VDEC_EX_TEE_GetControl(VDEC_EX_TEE_USER_CMD cmd,void* param)
{
    MS_BOOL ret = FALSE;

    switch(cmd)
    {
        case E_VDEC_EX_GET_VPU_PROTECT_START_ADDR:
            ret = _VDEC_EX_TEE_GetVPUProtect((VDEC_EX_TEE_VPU_Param*)param);
            break;
        default:
            PRINTF("[VDEC]%s,Invalid user cmd,%d\n",__FUNCTION__,(unsigned int)cmd);
            break;
    }

    return ret;
}

#endif


