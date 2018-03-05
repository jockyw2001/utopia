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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   apiDMX.c
/// @brief  Demux API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/compat.h>
#else
#include <string.h>
#endif
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
#include "ULog.h"

#ifdef MMFILEIN
#include "drvMMFilein.h"
#endif

#include "drvTSP.h"
#include "apiDMX.h"
#include "apiDMX_tsio.h"
#include "apiDMX_private.h"

#ifdef TSO_ENABLE
#include "drvTSO.h"
#endif

#ifdef TSIO_ENABLE
#include "drvTSIO.h"
#endif

#ifdef FQ_ENABLE
#include "drvFQ.h"
#endif

#include "utopia.h"

//#include "drvMIU.h"

#if DMX_DEBUG
#include "Internal_DMX_debug.h"
#endif

#ifdef DMX_RESET_FI_TIMESTAMP
#include "halCHIP.h"
#endif

// reserved functions, internal use only
TSP_Result MDrv_TSP_Suspend(void);
TSP_Result MDrv_TSP_Resume(MS_PHY phyFWAddr, MS_U32 u32FWSize);
TSP_Result MDrv_TSP_Alive(MS_U32 u32EngId);
TSP_Result MDrv_TSP_Reset(void);

#ifdef MSOS_TYPE_LINUX_KERNEL
extern void SerPrintf(char *fmt,...);
#endif

// Preprocessor warning notification
#if (DMX_SECT_FLT_DEPTH != DRVTSP_FILTER_DEPTH)
#error "DMX_SECT_FLT_DEPTH is inconsistent with DRVTSP_FILTER_DEPTH";
#endif

//------------------------------------------------------------------------------
// Compile options
//------------------------------------------------------------------------------
#define DMX_DBG_LINUX    FALSE

#define DMX_MUTEX

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
#define DMX_FLT_DS_SIZE                    160UL
#else
#define DMX_FLT_DS_SIZE                    768UL
#endif

#define DMX_MAX_IF_NUM                     DMX_PVR_ENG_NUM

// Pocily Mamager Pipie id definitation
#define DMX_PIPE_NULL                                       0xFFFFFFFF
#define DMX_PIPE_TSPFW                                      0x00000001
#define DMX_PIPE_TSPVQ                                      0x00000002
#define DMX_PIPE_PVR_0                                      0x80000000
#define DMX_PIPE_PVR_1                                      0x80000001
#define DMX_PIPE_FILE_0                                     0x40000000
#define DMX_PIPE_FILE_1                                     0x40000001
#define DMX_PIPE_FILE_2                                     0x40000002
#define DMX_PIPE_MMFI_0                                     0x41000000
#define DMX_PIPE_MMFI_1                                     0x41000001

//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
// for debugging test only
#define DMX_DBG_LINE()                          ULOGD("DMX", "[%s]-[%d]\n", __FUNCTION__, __LINE__);

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#define CPY_FROM_USER                   copy_from_user
#define CPY_to_USER                     copy_to_user
#else
#define CPY_FROM_USER                   memcpy
#define CPY_to_USER                     memcpy
#endif  //MSOS_TYPE_LINUX_KERNEL

#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_COMPAT))
#define CMP_CPY_FROM_USER(a,b,c)                (is_compat_task()? copy_from_user(a, compat_ptr((unsigned long)b), c) : copy_from_user(a, b, c))
#define CMP_CPY_TO_USER(a,b,c)                  (is_compat_task()? copy_to_user(compat_ptr((unsigned long)a), b, c) : copy_to_user(a, b, c))

#else
#define CMP_CPY_FROM_USER                       CPY_FROM_USER
#define CMP_CPY_TO_USER                         CPY_to_USER
#endif //CONFIG_COMPAT

//------------------------------------------------------------------------------
// Constant definition
//------------------------------------------------------------------------------
#define DMX_MAX_FLTID                      _pdmx_res->_u32TspFltNum
#define _DMX_MUTEX_TIMEOUT                 10000UL //MSOS_WAIT_FOREVER

//------------------------------------------------------------------------------
// Internal data structure
//------------------------------------------------------------------------------
typedef struct
{
    MS_U32                       FltId;
    MS_U32                       SecBufId;
    MS_U16                       u16Pid;
    DMX_Flt_info                 FltInfo;
    DMX_FILTER_TYPE              DmxFltType;
    DMX_Type2NotifyCb_Ex         pType2Notify_Ex;// section callback (tpye2)
#if !defined (__aarch64__)
    MS_U32                       u32AlignDummy0;  //align size for MI init share mem size check fail
#endif
    void *                       pType2NotifyParamEx;
#if !defined (__aarch64__)
    MS_U32                       u32AlignDummy1;  //align size for MI init share mem size check fail
#endif
} _DMX_Flt;

typedef struct _DMX_RESOURCE_PRIVATE
{
    MS_BOOL                     bResoureInit;

    _DMX_Flt                    _FltList[DMX_FLT_DS_SIZE];
    DMX_NotifyCb                _PvrNotify[DMX_MAX_IF_NUM];
#if !defined (__aarch64__)
    MS_U32                      u32AlignDummy[DMX_MAX_IF_NUM];  //align size for MI init share mem size check fail
#endif

#ifdef DMX_MUTEX
    MS_S32                      _Dmx_Mutex;
    MS_S32                      _Dmx_Pvr_Mutex;
    MS_S32                      _Dmx_MMFI_Mutex;
    MS_S32                      _Dmx_TSO_Mutex;
#endif // #ifdef DMX_MUTEX

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
    MS_BOOL                     _bPVRCA_RecAll[DMX_MAX_IF_NUM];
    MS_U32                      _u32PVREngSrc[DMX_MAX_IF_NUM];
#endif

    MS_BOOL                     _bFWInit;

#if(defined(VQ_ENABLE) && defined(TS2_IF_SUPPORT) && (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0)))
    MS_BOOL                     _bVQEnabled;
#endif
    MS_U32                      _u32TspFltNum;

    MS_PHY                      _phyTspFwAddr;
    MS_U32                      _u32TspFwSize;
    MS_PHY                      _phyVQAddr;
    MS_U32                      _u32VQSize;
    MS_U32                      _u32IsHK;

#ifdef DMX_RESET_FI_TIMESTAMP
    MS_BOOL                     _bPlayTimestampEnable[DMX_FILEIN_INVALID];
    MS_PHY                      _phyFI192DummyBufAddr;
    MS_U32                      _u32FI192DummyBufSize;
#endif

    MS_U32                      _u32LibMode;

    MS_BOOL                     _bEnableInitRefCnt;
    MS_U8                       _u8InitRefCnt;

#ifdef FQ_ENABLE
    DrvFQ_SrcIf                 _eFqSrc;
#endif

#ifdef SECURE_PVR_ENABLE
    DrvTSP_SecureBuf            _stSecBuf;
#endif

    MS_BOOL                     _bIsDMXOpenCalled;   //MApi_DMX_Open    is called
    MS_BOOL                     _bIsDMXOpenExCalled; //MApi_DMX_Open_Ex is called

}DMX_RESOURCE_PRIVATE;

typedef struct
{
    char            strCap[32];
    MS_S32          OutputSize;
    DMX_QUERY_TYPE  eCapType;
}_DMX_CAP_STRTBL;

//------------------------------------------------------------------------------
// Helper Macros
//------------------------------------------------------------------------------
// Filter management
#define _FLT_LIST_RESET()                                                        \
            {                                                                   \
                MS_U32     i;                                                      \
                for (i= 0; i< DMX_FLT_DS_SIZE; i++)                          \
                {                                                               \
                    _FLT_LIST_REMOVE(i);                                        \
                }                                                               \
            }

//For TV series, Section Filer ID is the same as section buffer ID, DMXId = FltId
//For Box series, PID filter ID is the same as Section Filter ID, FltId = SecBufId
#define _FLT_LIST_SECFLT_TYPE_SET(DMXId, inFltId, inSecBufId, inDmxFltType)                            \
                do{                                                       \
                    _pdmx_res->_FltList[(DMXId)].FltId = (inFltId);                                          \
                    _pdmx_res->_FltList[(DMXId)].SecBufId = (inSecBufId);                                    \
                    _pdmx_res->_FltList[(DMXId)].DmxFltType = (inDmxFltType);                                \
                }while(0);

#define _FLT_LIST_SECFLT_BUFID_GET(DMXId) _pdmx_res->_FltList[(DMXId)].SecBufId

#define _FLT_LIST_TYPE_GET(DMXId, pDmxFltType)                                                \
                *(pDmxFltType) = _pdmx_res->_FltList[(DMXId)].DmxFltType;              \

#define _FLT_LIST_INFO_SET(DMXId, pFltInfo)                                                   \
                    do{                                                                                     \
                        memcpy(&(_pdmx_res->_FltList[(DMXId)].FltInfo), (pFltInfo), sizeof(DMX_Flt_info));  \
                        _pdmx_res->_FltList[(DMXId)].pType2Notify_Ex = NULL;                                \
                        _pdmx_res->_FltList[(DMXId)].pType2NotifyParamEx = NULL;                            \
                    }while(0);

#define _FLT_LIST_INFO_SET_Ex(DMXId, pFltInfoEx)                                                                                                      \
                        do{                                                                                                                           \
                            if(pFltInfoEx->bEx == FALSE)                                                                                              \
                            {                                                                                                                         \
                               memcpy(&(_pdmx_res->_FltList[(DMXId)].FltInfo.Info.SectInfo), &(pFltInfoEx->DmxInfo.SectInfo), sizeof(DMX_Sect_info)); \
                               _pdmx_res->_FltList[(DMXId)].pType2Notify_Ex = NULL;                                                                   \
                               _pdmx_res->_FltList[(DMXId)].pType2NotifyParamEx = NULL;                                                               \
                            }                                                                                                                         \
                            else                                                                                                                      \
                            {                                                                                                                         \
                                _pdmx_res->_FltList[(DMXId)].FltInfo.Info.SectInfo.SectBufAddr = pFltInfoEx->DmxInfo.SectInfoEx.SectBufAddr ;         \
                                _pdmx_res->_FltList[(DMXId)].FltInfo.Info.SectInfo.SectBufSize = pFltInfoEx->DmxInfo.SectInfoEx.SectBufSize ;         \
                                _pdmx_res->_FltList[(DMXId)].FltInfo.Info.SectInfo.SectMode = pFltInfoEx->DmxInfo.SectInfoEx.SectMode;                \
                                _pdmx_res->_FltList[(DMXId)].FltInfo.Info.SectInfo.Event = pFltInfoEx->DmxInfo.SectInfoEx.Event;                      \
                                _pdmx_res->_FltList[(DMXId)].pType2Notify_Ex = pFltInfoEx->DmxInfo.SectInfoEx.pType2Notify_Ex;                        \
                                _pdmx_res->_FltList[(DMXId)].pType2NotifyParamEx = pFltInfoEx->DmxInfo.SectInfoEx.pType2NotifyParamEx;                \
                            }                                                                                                                         \
                        }while(0);



#define _FLT_LIST_INFO_GET(DMXId, pFltInfo)                                                   \
                    memcpy((pFltInfo), &(_pdmx_res->_FltList[(DMXId)].FltInfo), sizeof(DMX_Flt_info));     \

#define _FLT_LIST_INFO_GET_Ex(DMXId, pFltInfoEx)                                                                             \
                    do{                                                                                                      \
                        pFltInfoEx->SectInfoEx.SectBufAddr = _pdmx_res->_FltList[(DMXId)].FltInfo.Info.SectInfo.SectBufAddr; \
                        pFltInfoEx->SectInfoEx.SectBufSize = _pdmx_res->_FltList[(DMXId)].FltInfo.Info.SectInfo.SectBufSize; \
                        pFltInfoEx->SectInfoEx.SectMode = _pdmx_res->_FltList[(DMXId)].FltInfo.Info.SectInfo.SectMode;       \
                        pFltInfoEx->SectInfoEx.Event = _pdmx_res->_FltList[(DMXId)].FltInfo.Info.SectInfo.Event;             \
                        pFltInfoEx->SectInfoEx.pType2Notify_Ex = _pdmx_res->_FltList[(DMXId)].pType2Notify_Ex;               \
                        pFltInfoEx->SectInfoEx.pType2NotifyParamEx = _pdmx_res->_FltList[(DMXId)].pType2NotifyParamEx;       \
                    }while(0);

#define _FLT_LIST_INFO_GET_P(DMXId)              (&(_pdmx_res->_FltList[(DMXId)].FltInfo))

#define _FLT_LIST_PID_SET(DMXId, u16Pid_In)      _pdmx_res->_FltList[(DMXId)].u16Pid = (u16Pid_In)
#define _FLT_LIST_PID_GET(DMXId, u16Pid_In)      (u16Pid_In) = _pdmx_res->_FltList[(DMXId)].u16Pid

#define _FLT_LIST_REMOVE(DMXId)                                                                             \
                do{                                                                                         \
                    _pdmx_res->_FltList[(DMXId)].FltId=     0xFFFFFFFF;                                     \
                    _pdmx_res->_FltList[(DMXId)].SecBufId=  0xFFFFFFFF;                                     \
                    _pdmx_res->_FltList[(DMXId)].DmxFltType = (DMX_FILTER_TYPE)0xFF;                        \
                    _pdmx_res->_FltList[(DMXId)].pType2Notify_Ex = NULL;                                    \
                    _pdmx_res->_FltList[(DMXId)].pType2NotifyParamEx = NULL;                                \
                    memset(&(_pdmx_res->_FltList[(DMXId)].FltInfo), 0, sizeof(DMX_Flt_info));               \
                }while(0);

#define _MAX(A, B) (((A) > (B))?(A):(B))
#define _MIN(A, B) (((A) < (B))?(A):(B))
#define _DMX_OPEN_MIX_USED_CHECK()                    {                                                                                               \
                                                        if ( (_pdmx_res->_bIsDMXOpenCalled) && (_pdmx_res->_bIsDMXOpenExCalled) )                             \
                                                        {                                                                                           \
                                                            ULOGE("TSP", "[%s][%d]: Mix old & new API !!\n", __FUNCTION__, __LINE__);               \
                                                            return DMX_FILTER_STATUS_ERROR;                                                                      \
                                                        }                                                                                           \
                                                    }

#define _DMX_INFO_MIX_USED_CHECK(_IsEx)             {                                                                                               \
                                                        if( (_IsEx && (_pdmx_res->_bIsDMXOpenCalled)) || (!_IsEx && (_pdmx_res->_bIsDMXOpenExCalled))) \
                                                        {                                                                                           \
                                                            ULOGE("TSP", "[%s][%d]: Mix old & new DMX_OPEN & DMX_INFO !!\n", __FUNCTION__, __LINE__);               \
                                                            return DMX_FILTER_STATUS_ERROR;                                                                      \
                                                        }                                                                                           \
                                                    }
// Mutex function
#ifdef DMX_MUTEX

// For kernel STR callback function, should not use mutex
#define _DMX_ENTRY()                while(MsOS_ObtainMutex(_pdmx_res->_Dmx_Mutex, _DMX_MUTEX_TIMEOUT) == FALSE)    \
                                    {    \
                                        DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%s][%06d] DMX Mutex taking timeout\n", __FUNCTION__, __LINE__));    \
                                        MsOS_DelayTask(1);   \
                                    }

#define _DMX_RETURN(_ret)           do{    \
                                        if(MsOS_ReleaseMutex(_pdmx_res->_Dmx_Mutex) == FALSE)    \
                                        {    \
                                            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%s][%06d] DMX Mutex Free fail\n", __FUNCTION__, __LINE__));    \
                                            return DMX_FILTER_STATUS_ERROR;    \
                                        }    \
                                        return _ret; \
                                    } while(0);


// For kernel STR callback function, should not use mutex
#ifdef MSOS_TYPE_LINUX_KERNEL
#define _DMX_STR_ENTRY()

#define _DMX_STR_RETURN(_ret)       return _ret;

#else
#define _DMX_STR_ENTRY              _DMX_ENTRY
#define _DMX_STR_RETURN(_ret)       _DMX_RETURN(_ret)

#endif

#define _DMX_PVR_ENTRY()            while (MsOS_ObtainMutex(_pdmx_res->_Dmx_Pvr_Mutex, _DMX_MUTEX_TIMEOUT) == FALSE)    \
                                    {    \
                                        DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%s][%06d] PVR mutex taking timeout\n", __FUNCTION__, __LINE__));    \
                                        MsOS_DelayTask(1);    \
                                    }    \

#define _DMX_PVR_RETURN(_ret)       do{    \
                                        if(MsOS_ReleaseMutex(_pdmx_res->_Dmx_Pvr_Mutex) == FALSE)    \
                                        {    \
                                            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%s][%06d] PVR Mutex Free fail\n", __FUNCTION__, __LINE__));    \
                                            return DMX_FILTER_STATUS_ERROR;    \
                                        }    \
                                        return _ret;    \
                                    }while(0);

#define _DMX_MMFI_ENTRY()           while(MsOS_ObtainMutex(_pdmx_res->_Dmx_MMFI_Mutex, _DMX_MUTEX_TIMEOUT) == FALSE)    \
                                    {    \
                                        DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] MMFI Mutex taking timeout\n", __LINE__));    \
                                        MsOS_DelayTask(1);    \
                                    }

#define _DMX_MMFI_RETURN(_ret)      do{    \
                                        if(MsOS_ReleaseMutex(_pdmx_res->_Dmx_MMFI_Mutex) == FALSE)    \
                                        {    \
                                            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%s][%06d] MMFI Mutex Free fail\n", __FUNCTION__, __LINE__));    \
                                            return DMX_FILTER_STATUS_ERROR;    \
                                        }    \
                                        return _ret;    \
                                    }while(0);

#ifdef TSO_ENABLE
#define _DMX_TSO_ENTRY()            while (MsOS_ObtainMutex(_pdmx_res->_Dmx_TSO_Mutex, _DMX_MUTEX_TIMEOUT) == FALSE)    \
                                    {    \
                                        DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%06d] TSO Mutex taking timeout\n", __LINE__));    \
                                        MsOS_DelayTask(1);    \
                                    }

#define _DMX_TSO_RETURN(_ret)       do{   \
                                        if(MsOS_ReleaseMutex(_pdmx_res->_Dmx_TSO_Mutex) == FALSE)    \
                                        {    \
                                            DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%s][%06d] TSO Mutex Free fail\n", __FUNCTION__, __LINE__));    \
                                            return DMX_FILTER_STATUS_ERROR;    \
                                        }    \
                                        return _ret;    \
                                    }while(0);
#endif

#else // #ifdef DMX_MUTEX
#define _DMX_ENTRY()           while (0)
#define _DMX_RETURN(_ret)      return _ret
#define _DMX_PVR_ENTRY()       while (0)
#define _DMX_PVR_RETURN(_ret)  return _ret
#define _DMX_MMFI_ENTRY()      while (0)
#define _DMX_MMFI_RETURN(_ret) return _ret
#define _DMX_TSO_ENTRY()       while (0)
#define _DMX_TSO_RETURN(_ret)  return _ret

#endif // #ifdef DMX_MUTEX

#define DMX_ASSERT(_bool, _f)                   if (!(_bool)) { (_f); return DMX_FILTER_STATUS_ERROR;}
#define DMX_ASSERT2(_bool, _f)                  if (!(_bool)) { (_f); _DMX_RETURN(DMX_FILTER_STATUS_ERROR);}

#define DMX_PVR_ASSERT(_bool, _f)                   if (!(_bool)) { (_f); return DMX_FILTER_STATUS_ERROR;}
#define DMX_PVR_ASSERT2(_bool, _f)                  if (!(_bool)) { (_f); _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);}

#define DMX_MMFI_ASSERT(_bool, _f)                   if (!(_bool)) { (_f); return DMX_FILTER_STATUS_ERROR;}
#define DMX_MMFI_ASSERT2(_bool, _f)                  if (!(_bool)) { (_f); _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);}

#define DMX_DBGMSG(_level,_f)                   {if(_u32DMXDbgLevel >= (_level)) (_f);}
#define DMX_PVRDBGMSG(_level,_f)                {if(DMX_DBG_ERR == (_level)) (_f);}
#define DMX_TSOMSG(_level,_f)                   {if(DMX_DBG_ERR >= (_level)) (_f);}
//------------------------------------------------------------------------------
// Local Variable
//------------------------------------------------------------------------------
static _DMX_CAP_STRTBL _tblDmxCapStr[] =
{
    {"PID_FILTER_NUM",                  4, DMX_CAP_PID_FILTER_NUM},
    {"SEC_FILTER_NUM",                  4, DMX_CAP_SEC_FILTER_NUM},
    {"SEC_BUF_NUM",                     4, DMX_CAP_SEC_BUF_NUM},

    {"PVR_ENG_NUM",                     4, DMX_CAP_PVR_ENG_NUM},
    {"PVR_FILTER_NUM",                  4, DMX_CAP_PVR_FILTER_NUM},
    {"PVR1_FILTER_NUM",                 4, DMX_CAP_PVR1_FILTER_NUM},

    {"MMFI_AUDIO_FILTER_NUM",           4, DMX_CAP_MMFI_AUDIO_FILTER_NUM},
    {"MMFI_V3D_FILTER_NUM",             4, DMX_CAP_MMFI_V3D_FILTER_NUM},

    {"MMFI0_FILTER_NUM",                4, DMX_CAP_MMFI0_FILTER_NUM},
    {"MMFI1_FILTER_NUM",                4, DMX_CAP_MMFI1_FILTER_NUM},

    {"TSIF_NUM",                        4, DMX_CAP_TSIF_NUM},
    {"DEMOD_NUM",                       4, DMX_CAP_DEMOD_NUM},
    {"TS_PAD_NUM",                      4, DMX_CAP_TS_PAD_NUM},
    {"VQ_NUM",                          4, DMX_CAP_VQ_NUM},

    {"CA_FLT_NUM",                      4, DMX_CAP_CA_FLT_NUM},
    {"CA_KEY_NUM",                      4, DMX_CAP_CA_KEY_NUM},

    {"FW_ALIGN",                        4, DMX_CAP_FW_ALIGN},
    {"VQ_ALIGN",                        4, DMX_CAP_VQ_ALIGN},
    {"VQ_PITCH",                        4, DMX_CAP_VQ_PITCH},
    {"SEC_BUF_ALIGN",                   4, DMX_CAP_SEC_BUF_ALIGN},
    {"PVR_ALIGN",                       4, DMX_CAP_PVR_ALIGN},

    {"PVRCA_PATH_NUM",                  4, DMX_CAP_PVRCA_PATH_NUM},

    {"SHAREKEY_FLT_RANGE",              8, DMX_CAP_SHAREKEY_FLT_RANGE},
    {"PVRCA0_FLT_RANGE",                8, DMX_CAP_PVRCA0_FLT_RANGE},
    {"PVRCA1_FLT_RANGE",                8, DMX_CAP_PVRCA1_FLT_RANGE},
    {"PVRCA2_FLT_RANGE",                8, DMX_CAP_PVRCA2_FLT_RANGE},
    {"SHAREKEY_FLT1_RANGE",             8, DMX_CAP_SHAREKEY_FLT1_RANGE},
    {"SHAREKEY_FLT2_RANGE",             8, DMX_CAP_SHAREKEY_FLT2_RANGE},

    {"HW_TYPE",                         4, DMX_CAP_HW_TYPE},

    {"RESOURCE_SIZE",                   4, DMX_CAP_RESOURCE_SIZE},

    {"VFIFO_NUM",                       4, DMX_CAP_VFIFO_NUM},
    {"AFIFO_NUM",                       4, DMX_CAP_AFIFO_NUM},

    {"HWPCR_SUPPORT",                   4, DMX_CAP_HWPCR_SUPPORT},
    {"PCRFLT_START_IDX",                4, DMX_CAP_PCRFLT_START_IDX},
    {"TSP_HWWP_SET_NUM",                4, DMX_CAP_TSP_HWWP_SET_NUM},

    {"DSCMB_ENG_NUM",                   4, DMX_CAP_DSCMB_ENG_NUM},

    {"MAX_MERGESTR_NUM",                4, DMX_CAP_MAX_MERGESTR_NUM},

    {"MAX_SEC_FLT_DEPTH",               4, DMX_CAP_MAX_SEC_FLT_DEPTH},
    {"FW_BUF_SIZE",                     4, DMX_CAP_FW_BUF_SIZE},
    {"FW_BUF_RANGE",                    4, DMX_CAP_FW_BUF_RANGE},
    {"VQ_BUF_RANGE",                    4, DMX_CAP_VQ_BUF_RANGE},
    {"SEC_BUF_RANGE",                   4, DMX_CAP_SEC_BUF_RANGE},
    {"FIQ_NUM",                         4, DMX_CAP_FIQ_NUM},
};


static MSIF_Version _api_dmx_version = {
    .DDI = { DMX_API_VERSION },
};

#ifndef DMX_UTOPIA_20
static DMX_RESOURCE_PRIVATE         _dmx_res = {
    .bResoureInit                               = FALSE,

    ._PvrNotify                                 = {[0 ... (DMX_MAX_IF_NUM-1)] = NULL},
    ._Dmx_Mutex                                 = -1,
    ._Dmx_Pvr_Mutex                             = -1,
    ._Dmx_MMFI_Mutex                            = -1,
    ._Dmx_TSO_Mutex                             = -1,

  #if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
    ._bPVRCA_RecAll                             = {[0 ... (DMX_MAX_IF_NUM-1)] = FALSE},
    ._u32PVREngSrc                              = {0},
  #endif

    ._bFWInit                                   = FALSE,

  #if(defined(VQ_ENABLE) && defined(TS2_IF_SUPPORT) && (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0)))
    ._bVQEnabled                                = FALSE,
  #endif

    ._u32TspFltNum                              = 0UL,

    ._phyTspFwAddr                              = 0UL,
    ._u32TspFwSize                              = 0UL,
    ._phyVQAddr                                 = 0UL,
    ._u32VQSize                                 = 0UL,
    ._u32IsHK                                   = 0,

#ifdef DMX_RESET_FI_TIMESTAMP
    ._bPlayTimestampEnable                      = {[0 ... (DMX_FILEIN_INVALID-1)] = FALSE},
    ._phyFI192DummyBufAddr                      = 0UL,
    ._u32FI192DummyBufSize                      = 0UL,
#endif

    ._u32LibMode                                = 0,

    ._bEnableInitRefCnt                         = FALSE,
    ._u8InitRefCnt                              = 0,
#ifdef FQ_ENABLE
  #if((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0))
    ._eFqSrc                                    = E_DRVFQ_SRC_TSIF0,
  #else
    ._eFqSrc                                    = E_DRVFQ_SRC_DEFAULT,
  #endif
#endif

#ifdef SECURE_PVR_ENABLE
    ._stSecBuf                                   = {0, 0, 0, 0},
#endif
};

static DMX_RESOURCE_PRIVATE*        _pdmx_res = &_dmx_res;
#else
static DMX_RESOURCE_PRIVATE*        _pdmx_res       = NULL;
#endif

MS_U32                              _Owner        = 0UL;

static MS_U32                       _u32DMXDbgLevel = DMX_DBG_ERR;
static MS_U32                       _u32PESCpy_ContMode = 0;

#if DMX_DEBUG
extern MS_U32 u32DbgLogFlag;
extern MS_U32 u32DmxDbgFltId;
extern DMX_AV_FIFO_STR stDmxType[];
extern DMX_FLT_DBG_INFO stDmxDbgInfo[];
extern FILE* pfSEC;
#endif

//------------------------------------------------------------------------------
// Internal implementation
//------------------------------------------------------------------------------

#if 0
static const unsigned long drv_crc32_table[256] =
{
    0x00000000L, 0x04C11DB7L, 0x09823B6EL, 0x0D4326D9L, 0x130476DCL, 0x17C56B6BL, 0x1A864DB2L, 0x1E475005L,
    0x2608EDB8L, 0x22C9F00FL, 0x2F8AD6D6L, 0x2B4BCB61L, 0x350C9B64L, 0x31CD86D3L, 0x3C8EA00AL, 0x384FBDBDL,
    0x4C11DB70L, 0x48D0C6C7L, 0x4593E01EL, 0x4152FDA9L, 0x5F15ADACL, 0x5BD4B01BL, 0x569796C2L, 0x52568B75L,
    0x6A1936C8L, 0x6ED82B7FL, 0x639B0DA6L, 0x675A1011L, 0x791D4014L, 0x7DDC5DA3L, 0x709F7B7AL, 0x745E66CDL,
    0x9823B6E0L, 0x9CE2AB57L, 0x91A18D8EL, 0x95609039L, 0x8B27C03CL, 0x8FE6DD8BL, 0x82A5FB52L, 0x8664E6E5L,
    0xBE2B5B58L, 0xBAEA46EFL, 0xB7A96036L, 0xB3687D81L, 0xAD2F2D84L, 0xA9EE3033L, 0xA4AD16EAL, 0xA06C0B5DL,
    0xD4326D90L, 0xD0F37027L, 0xDDB056FEL, 0xD9714B49L, 0xC7361B4CL, 0xC3F706FBL, 0xCEB42022L, 0xCA753D95L,
    0xF23A8028L, 0xF6FB9D9FL, 0xFBB8BB46L, 0xFF79A6F1L, 0xE13EF6F4L, 0xE5FFEB43L, 0xE8BCCD9AL, 0xEC7DD02DL,
    0x34867077L, 0x30476DC0L, 0x3D044B19L, 0x39C556AEL, 0x278206ABL, 0x23431B1CL, 0x2E003DC5L, 0x2AC12072L,
    0x128E9DCFL, 0x164F8078L, 0x1B0CA6A1L, 0x1FCDBB16L, 0x018AEB13L, 0x054BF6A4L, 0x0808D07DL, 0x0CC9CDCAL,
    0x7897AB07L, 0x7C56B6B0L, 0x71159069L, 0x75D48DDEL, 0x6B93DDDBL, 0x6F52C06CL, 0x6211E6B5L, 0x66D0FB02L,
    0x5E9F46BFL, 0x5A5E5B08L, 0x571D7DD1L, 0x53DC6066L, 0x4D9B3063L, 0x495A2DD4L, 0x44190B0DL, 0x40D816BAL,
    0xACA5C697L, 0xA864DB20L, 0xA527FDF9L, 0xA1E6E04EL, 0xBFA1B04BL, 0xBB60ADFCL, 0xB6238B25L, 0xB2E29692L,
    0x8AAD2B2FL, 0x8E6C3698L, 0x832F1041L, 0x87EE0DF6L, 0x99A95DF3L, 0x9D684044L, 0x902B669DL, 0x94EA7B2AL,
    0xE0B41DE7L, 0xE4750050L, 0xE9362689L, 0xEDF73B3EL, 0xF3B06B3BL, 0xF771768CL, 0xFA325055L, 0xFEF34DE2L,
    0xC6BCF05FL, 0xC27DEDE8L, 0xCF3ECB31L, 0xCBFFD686L, 0xD5B88683L, 0xD1799B34L, 0xDC3ABDEDL, 0xD8FBA05AL,
    0x690CE0EEL, 0x6DCDFD59L, 0x608EDB80L, 0x644FC637L, 0x7A089632L, 0x7EC98B85L, 0x738AAD5CL, 0x774BB0EBL,
    0x4F040D56L, 0x4BC510E1L, 0x46863638L, 0x42472B8FL, 0x5C007B8AL, 0x58C1663DL, 0x558240E4L, 0x51435D53L,
    0x251D3B9EL, 0x21DC2629L, 0x2C9F00F0L, 0x285E1D47L, 0x36194D42L, 0x32D850F5L, 0x3F9B762CL, 0x3B5A6B9BL,
    0x0315D626L, 0x07D4CB91L, 0x0A97ED48L, 0x0E56F0FFL, 0x1011A0FAL, 0x14D0BD4DL, 0x19939B94L, 0x1D528623L,
    0xF12F560EL, 0xF5EE4BB9L, 0xF8AD6D60L, 0xFC6C70D7L, 0xE22B20D2L, 0xE6EA3D65L, 0xEBA91BBCL, 0xEF68060BL,
    0xD727BBB6L, 0xD3E6A601L, 0xDEA580D8L, 0xDA649D6FL, 0xC423CD6AL, 0xC0E2D0DDL, 0xCDA1F604L, 0xC960EBB3L,
    0xBD3E8D7EL, 0xB9FF90C9L, 0xB4BCB610L, 0xB07DABA7L, 0xAE3AFBA2L, 0xAAFBE615L, 0xA7B8C0CCL, 0xA379DD7BL,
    0x9B3660C6L, 0x9FF77D71L, 0x92B45BA8L, 0x9675461FL, 0x8832161AL, 0x8CF30BADL, 0x81B02D74L, 0x857130C3L,
    0x5D8A9099L, 0x594B8D2EL, 0x5408ABF7L, 0x50C9B640L, 0x4E8EE645L, 0x4A4FFBF2L, 0x470CDD2BL, 0x43CDC09CL,
    0x7B827D21L, 0x7F436096L, 0x7200464FL, 0x76C15BF8L, 0x68860BFDL, 0x6C47164AL, 0x61043093L, 0x65C52D24L,
    0x119B4BE9L, 0x155A565EL, 0x18197087L, 0x1CD86D30L, 0x029F3D35L, 0x065E2082L, 0x0B1D065BL, 0x0FDC1BECL,
    0x3793A651L, 0x3352BBE6L, 0x3E119D3FL, 0x3AD08088L, 0x2497D08DL, 0x2056CD3AL, 0x2D15EBE3L, 0x29D4F654L,
    0xC5A92679L, 0xC1683BCEL, 0xCC2B1D17L, 0xC8EA00A0L, 0xD6AD50A5L, 0xD26C4D12L, 0xDF2F6BCBL, 0xDBEE767CL,
    0xE3A1CBC1L, 0xE760D676L, 0xEA23F0AFL, 0xEEE2ED18L, 0xF0A5BD1DL, 0xF464A0AAL, 0xF9278673L, 0xFDE69BC4L,
    0x89B8FD09L, 0x8D79E0BEL, 0x803AC667L, 0x84FBDBD0L, 0x9ABC8BD5L, 0x9E7D9662L, 0x933EB0BBL, 0x97FFAD0CL,
    0xAFB010B1L, 0xAB710D06L, 0xA6322BDFL, 0xA2F33668L, 0xBCB4666DL, 0xB8757BDAL, 0xB5365D03L, 0xB1F740B4L
};

static int _TSP_GetCrc32(unsigned char *buffer, unsigned long length)
{
    unsigned long ulCRC = 0xFFFFFFFF;

    while (length--)
    {
        ulCRC = (ulCRC << 8) ^ drv_crc32_table[((ulCRC >> 24) & 0xFF) ^ *buffer++];
    }

    return (ulCRC);
}
#endif

#ifdef DMX_RESET_FI_TIMESTAMP
static MS_VIRT _DMX_PA2KSEG1(MS_PHY phyAddr, MS_U32 u32Size)
{
    MS_VIRT pdummy = 0;
    MS_U32 u32MiuOffset = 0;
    MS_U8 u8MiuSel = 0;

    pdummy = MsOS_PA2KSEG1(phyAddr);
    if(pdummy == 0)
    {
        printf("[%s][%d] PA2VA is 0, do MPool Mapping...\n", __FUNCTION__, __LINE__);

        _phy_to_miu_offset(u8MiuSel, u32MiuOffset, phyAddr);

        if(MsOS_MPool_Mapping(u8MiuSel, (phyAddr - u32MiuOffset), u32Size, TRUE) == TRUE)
        {
            pdummy = MsOS_PA2KSEG1(phyAddr);
        }
    }

    return pdummy;
}
#endif

static MS_BOOL _DMX_InitResource(DMX_RESOURCE_PRIVATE* presource)
{
    MS_U32 u32ii;

#ifdef DMX_MUTEX
    presource->_Dmx_Mutex =                 -1;
    presource->_Dmx_Pvr_Mutex =             -1;
    presource->_Dmx_MMFI_Mutex =            -1;
    presource->_Dmx_TSO_Mutex =             -1;
#endif

    for(u32ii = 0; u32ii < DMX_MAX_IF_NUM; u32ii++)
    {
        presource->_PvrNotify[u32ii] = NULL;
    }

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
    for(u32ii = 0; u32ii < DMX_MAX_IF_NUM; u32ii++)
    {
        presource->_bPVRCA_RecAll[u32ii] = FALSE;
        presource->_u32PVREngSrc[u32ii] = 0;
    }
#endif

    presource->_bFWInit = FALSE;

#if(defined(VQ_ENABLE) && defined(TS2_IF_SUPPORT) && (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0)))
    presource->_bVQEnabled = FALSE;
#endif

    presource->_u32TspFltNum = 0UL;

    presource->_phyTspFwAddr = 0UL;
    presource->_u32TspFwSize = 0UL;
    presource->_phyVQAddr    = 0UL;
    presource->_u32VQSize    = 0UL;
    presource->_u32IsHK       = 0;

#ifdef DMX_RESET_FI_TIMESTAMP
    for(u32ii = 0; u32ii < DMX_FILEIN_INVALID; u32ii++)
    {
        presource->_bPlayTimestampEnable[u32ii] = FALSE;
    }
    presource->_phyFI192DummyBufAddr = 0;
    presource->_u32FI192DummyBufSize = 0;
#endif

    presource->_u32LibMode = 0;


    presource->_bEnableInitRefCnt = FALSE;
    presource->_u8InitRefCnt  = 0;

#ifdef FQ_ENABLE
  #if ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0))
    presource->_eFqSrc = E_DRVFQ_SRC_TSIF0;
  #else
    presource->_eFqSrc = E_DRVFQ_SRC_DEFAULT;
  #endif
#endif

#ifdef SECURE_PVR_ENABLE
    presource->_stSecBuf.u32BufId = 0;
    presource->_stSecBuf.u32BufOpt = 0;
    presource->_stSecBuf.u32BufSize = 0;
    presource->_stSecBuf.phyBufAddr = 0;
#endif

    return TRUE;
}

static void _DMX_NotifyPvr(DrvTSP_Event eEvent, DrvTSP_Msg *pMsg)
{
    MS_U32 u32PVRID = 0;

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
  #ifdef TS3_IF_SUPPORT
    if(eEvent & (E_DRVTSP_EVENT_PVR2BUF_FULL|E_DRVTSP_EVENT_PVR2BUF_OVERFLOW))
    {
        u32PVRID = 1;
    }
  #endif

  #ifdef TS2_IF_SUPPORT
    if(eEvent & (E_DRVTSP_EVENT_CBBUF_FULL|E_DRVTSP_EVENT_CBBUF_OVERFLOW))
    {
        if (DRVTSP_OK != MDrv_TSP_GetCap(E_DRVTSP_CAP_PVR_FILTER_NUM, (void*)&u32PVRID))
        {
            u32PVRID = 1;
        }
        else
        {
            u32PVRID--;
        }
    }
  #endif
#endif

    if(_pdmx_res->_PvrNotify[u32PVRID] == NULL)
    {
        MS_CRITICAL_MSG(DMX_DBGMSG(DMX_DBG_WARN, ULOGW("DMX", "Warning: [MAPI DMX][%06d] NULL callback function\n", __LINE__)));
    }
    else
    {
        _pdmx_res->_PvrNotify[u32PVRID]((pMsg->PvrBufId)>> MSG_PVRBUF_ID_SHFT, (DMX_EVENT)eEvent);
    }
}

//Only for section now
static void _DMX_Notify(DrvTSP_Event eEvent, DrvTSP_Msg *pMsg)
{
    MS_U32 u32SecFltId = (pMsg->FltInfo & MSG_FLTINFO_SEC_ID_MASK);
    MS_U32 u32DMXId = u32SecFltId;

    //ULOGD("DMX", "[%s] FltID [%x]\n",__FUNCTION__, (int)u32SecFltId);
    //ULOGD("DMX", "[%s] Event [%d]\n",__FUNCTION__, eEvent);

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
    //searching mapping PID filter id
    if(u32SecFltId != _pdmx_res->_FltList[u32DMXId].SecBufId)
    {
        MS_U32 u32FltNum = 0;
        if(MDrv_TSP_GetCap(E_DRVTSP_CAP_PID_FILTER_NUM, (void*)&u32FltNum) != DRVTSP_OK)
        {
            return;
        }
        for(u32DMXId = 0; u32DMXId < u32FltNum; u32DMXId++)
        {
            if(_pdmx_res->_FltList[u32DMXId].SecBufId == 0xFFFFFFFF)
                continue;

            if(u32SecFltId == _pdmx_res->_FltList[u32DMXId].SecBufId)
            {
                break;
            }
        }
        if(u32DMXId == u32FltNum)
        {
            return;
        }
    }
#endif

#if DMX_DEBUG
    {
        MS_U32 u32TimeCB = 0;
        MS_BOOL bMonitorCB = FALSE;
        MS_BOOL bMonitorEvent = FALSE;
        dmx_dbg_IsFltMonitor(u32DMXId, bMonitorCB);
        bMonitorCB = bMonitorCB && (u32DbgLogFlag & DMX_DBG_LOG_FLT_CB);

        if(bMonitorCB)
            u32TimeCB = MsOS_GetSystemTime();

        dmx_dbg_IsFltMonitor(u32DMXId, bMonitorEvent);
        if(bMonitorEvent && (u32DbgLogFlag & DMX_DBG_LOG_FLT_SEC_OVF) && (eEvent == E_DRVTSP_EVENT_BUF_OVERFLOW))
            dmx_dbg_print("[DMX OVF]\tFlt[%03d]\n", (MS_S32)u32DMXId);

        if(bMonitorEvent && (u32DbgLogFlag & DMX_DBG_LOG_FLT_SEC_RDY) && (eEvent == E_DRVTSP_EVENT_DATA_READY))
            dmx_dbg_print("[DMX RDY]\tFlt[%03d]\n", (MS_S32)u32DMXId);

        if(eEvent == E_DRVTSP_EVENT_DATA_READY)
        {
            stDmxDbgInfo[u32SecFltId].u32CB_rdy ++;
        }
        else if(eEvent == E_DRVTSP_EVENT_BUF_OVERFLOW)
        {
            stDmxDbgInfo[u32SecFltId].u32CB_ovf ++;
        }

#endif

    if((_pdmx_res->_FltList[u32DMXId].FltInfo.Info.SectInfo.Event&DMX_EVENT_CB_SELF_TYPE2) == DMX_EVENT_CB_SELF_TYPE2)
    {

        if((_pdmx_res->_FltList[u32DMXId].FltInfo.Info.SectInfo.pType2Notify) && (_pdmx_res->_FltList[u32DMXId].pType2Notify_Ex))
        {
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Mixed usage of SecRdy Callback function\n", __LINE__));
        }
        else if((_pdmx_res->_FltList[u32DMXId].FltInfo.Info.SectInfo.pType2Notify) && !(_pdmx_res->_FltList[u32DMXId].pType2Notify_Ex))
        {
            _pdmx_res->_FltList[u32DMXId].FltInfo.Info.SectInfo.pType2Notify((MS_U8)u32DMXId, (DMX_EVENT)eEvent, _pdmx_res->_FltList[u32DMXId].FltInfo.Info.SectInfo.Type2NotifyParam1);
        }
        else if(!(_pdmx_res->_FltList[u32DMXId].FltInfo.Info.SectInfo.pType2Notify) && (_pdmx_res->_FltList[u32DMXId].pType2Notify_Ex))
        {
            _pdmx_res->_FltList[u32DMXId].pType2Notify_Ex(u32DMXId, (DMX_EVENT)eEvent, (_pdmx_res->_FltList[u32DMXId].pType2NotifyParamEx));
        }
        else
        {
            DMX_DBGMSG(DMX_DBG_ERR, ULOGW("DMX", "Warning: [MAPI DMX][%06d] NULL callback function\n", __LINE__));
        }
    }
    else
    {
        if(_pdmx_res->_FltList[u32DMXId].FltInfo.Info.SectInfo.pNotify)
        {
            _pdmx_res->_FltList[u32DMXId].FltInfo.Info.SectInfo.pNotify((MS_U8)u32DMXId, (DMX_EVENT)eEvent);
        }
        else
        {
            DMX_DBGMSG(DMX_DBG_WARN, ULOGW("DMX", "Warning: [MAPI DMX][%06d] NULL callback function\n", __LINE__));
        }
    }

#if DMX_DEBUG
        if(bMonitorCB)
        {
            dmx_dbg_print("[DMX CB]\tFlt[%03d] PID[0x%x] time[%d ms]\n",
                          (MS_S32)u32DMXId,
                          (MS_U32)_pdmx_res->_FltList[(u32DMXId)].u16Pid,
                          (MS_S32)(MsOS_GetSystemTime() - u32TimeCB));

        }
    }
#endif

}

// check if buffer range is already used by other filter
#if 1
static MS_BOOL _DMX_ChkBufOverlap(MS_U32 u32DmxId, MS_PHY phyAddr, MS_U32 u32Size)
{
    MS_S32 idx;

    for(idx = _pdmx_res->_u32TspFltNum - 1; idx >= 0 ; idx--)
    {
        DMX_FILTER_TYPE type;

        _FLT_LIST_TYPE_GET(idx, &type);
        if((type == DMX_FILTER_TYPE_SECTION)  ||
           (type == DMX_FILTER_TYPE_TELETEXT) ||
           (type == DMX_FILTER_TYPE_PES)      ||
           (type == DMX_FILTER_TYPE_PACKET)   ||
           (type == DMX_FILTER_TYPE_SECTION_VER) ||
           (type == DMX_FILTER_TYPE_SECTION_NO_PUSI))
        {   // filter is opened
            DMX_Sect_info*  pInfo = &(_FLT_LIST_INFO_GET_P(idx)->Info.SectInfo);
            if(idx != (MS_S32)u32DmxId)
            {
                MS_U32 max = (MS_U32)_MAX(pInfo->SectBufAddr, phyAddr);
                MS_U32 min = (MS_U32)_MIN(pInfo->SectBufAddr, phyAddr);
                MS_U32 size = (min == pInfo->SectBufAddr)? pInfo->SectBufSize : u32Size;

                if((pInfo->SectBufAddr == phyAddr) || ((min + size) > (max)))
                {
                #if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
                    if (_FLT_LIST_SECFLT_BUFID_GET(idx) == _FLT_LIST_SECFLT_BUFID_GET(u32DmxId))
                    {   // multi filter to one buffer case
                        continue;
                    }
                #endif

                    printf("[DMX ERROR]: [MAPI DMX][%06d][%d:%d] Buffer overlap, 0x%x (0x%x)<==> 0x%x (0x%x)\n",
                    __LINE__,
                    (int)u32DmxId,
                    (int)idx,
                    (int)pInfo->SectBufAddr,
                    (int)pInfo->SectBufSize,
                    (int)phyAddr,
                    (int)u32Size
                    );

                    return TRUE;
                }
            }
        }
    } // for loop

    return FALSE;
}
#endif

static MS_BOOL _DMX_InfoSet_Stream(MS_U32 u32DmxId, DMX_Flt_info* pDmxFltInfo)
{
    _FLT_LIST_INFO_SET(u32DmxId, pDmxFltInfo);
    return TRUE;
}

static MS_BOOL _DMX_InfoSet_Sect(MS_U32 u32DmxId, DMX_InfoSet* pDmxInfo)
{
    DMX_FILTER_TYPE        DmxFltType;
    MS_PHY                 SectBufAddr;
    MS_U32                 SectBufSize;
    DMX_SECT_MODE          DmxSectMode;
    DMX_EVENT              Event;

    if(pDmxInfo->bEx == FALSE)
    {
        SectBufAddr = pDmxInfo->DmxInfo.SectInfo.SectBufAddr;
        SectBufSize = pDmxInfo->DmxInfo.SectInfo.SectBufSize;
        DmxSectMode = pDmxInfo->DmxInfo.SectInfo.SectMode;
        Event       = pDmxInfo->DmxInfo.SectInfo.Event;
    }
    else
    {
        SectBufAddr = pDmxInfo->DmxInfo.SectInfoEx.SectBufAddr;
        SectBufSize = pDmxInfo->DmxInfo.SectInfoEx.SectBufSize;
        DmxSectMode = pDmxInfo->DmxInfo.SectInfoEx.SectMode;
        Event       = pDmxInfo->DmxInfo.SectInfoEx.Event;
    }


    _FLT_LIST_TYPE_GET(u32DmxId, &DmxFltType);
    if (DMX_FILTER_TYPE_PCR == DmxFltType)
    {
        return TRUE;
    }
    if (0 == SectBufSize)
    {
       DMX_DBGMSG(DMX_DBG_WARN, ULOGW("DMX", "warning: [MAPI DMX][%06d] buffer size = 0, buffer address = 0x%08x\n", __LINE__, (unsigned int)SectBufAddr));
    }

    MDrv_TSP_SecFlt_SetMode(0, _pdmx_res->_FltList[u32DmxId].SecBufId/*u8DmxId*/, (DrvTSP_FltMode)DmxSectMode);


    if(_DMX_ChkBufOverlap(u32DmxId, SectBufAddr, SectBufSize))
    {
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
        ULOGE("DMX", "FATAL ERROR: [MAPI DMX][%06d]Section buffer overlap!!!!!!!!!\n", __LINE__);
        MS_ASSERT(0);
        return FALSE;
#endif
    }
    MDrv_TSP_SecFlt_SetBuffer(0, _pdmx_res->_FltList[u32DmxId].SecBufId, SectBufAddr, SectBufSize);

    if(pDmxInfo->bEx == FALSE)
    {
        if (pDmxInfo->DmxInfo.SectInfo.pNotify || (((Event&DMX_EVENT_CB_SELF_TYPE2) == DMX_EVENT_CB_SELF_TYPE2) && pDmxInfo->DmxInfo.SectInfo.pType2Notify))
        {
            MDrv_TSP_SecFlt_Notify(0, _pdmx_res->_FltList[u32DmxId].SecBufId, (DrvTSP_Event)Event, _DMX_Notify);
        }
        else
        {
            DMX_DBGMSG(DMX_DBG_WARN, ULOGW("DMX", "Warning: [MAPI DMX][%06d] NULL callback function\n", __LINE__));
        }
    }
    else
    {
        if  (((Event&DMX_EVENT_CB_SELF_TYPE2) == DMX_EVENT_CB_SELF_TYPE2) && pDmxInfo->DmxInfo.SectInfoEx.pType2Notify_Ex)
        {
            MDrv_TSP_SecFlt_Notify(0, _pdmx_res->_FltList[u32DmxId].SecBufId, (DrvTSP_Event)Event, _DMX_Notify);
        }
        else
        {
            DMX_DBGMSG(DMX_DBG_WARN, ULOGW("DMX", "Warning: [MAPI DMX][%06d] NULL callback function\n", __LINE__));
        }
    }

    _FLT_LIST_INFO_SET_Ex(u32DmxId, pDmxInfo);
    return TRUE;
}

static MS_BOOL _DMX_ApiDrv_FltTypeMapping(DMX_FILTER_TYPE DmxFltType, DrvTSP_FltType *TspFltType)
{
    switch (DmxFltType)
    {
        case DMX_FILTER_TYPE_VIDEO:
            *TspFltType = E_DRVTSP_FLT_TYPE_VIDEO;
            break;
#ifdef MMFI_VD3D
        case DMX_FILTER_TYPE_VIDEO3D:
            *TspFltType = E_DRVTSP_FLT_TYPE_VIDEO3D;
            break;
#endif
#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
        case DMX_FILTER_TYPE_VIDEO3:
            *TspFltType = E_DRVTSP_FLT_TYPE_VIDEO3;
            break;
        case DMX_FILTER_TYPE_VIDEO4:
            *TspFltType = E_DRVTSP_FLT_TYPE_VIDEO4;
            break;
        case DMX_FILTER_TYPE_VIDEO5:
            *TspFltType = E_DRVTSP_FLT_TYPE_VIDEO5;
            break;
        case DMX_FILTER_TYPE_VIDEO6:
            *TspFltType = E_DRVTSP_FLT_TYPE_VIDEO6;
            break;
        case DMX_FILTER_TYPE_VIDEO7:
            *TspFltType = E_DRVTSP_FLT_TYPE_VIDEO7;
            break;
        case DMX_FILTER_TYPE_VIDEO8:
            *TspFltType = E_DRVTSP_FLT_TYPE_VIDEO8;
            break;
#endif
        case DMX_FILTER_TYPE_AUDIO:
            *TspFltType = E_DRVTSP_FLT_TYPE_AUDIO;
            break;
        case DMX_FILTER_TYPE_AUDIO2:
            *TspFltType = E_DRVTSP_FLT_TYPE_AUDIO2;
            break;
#if (!(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0)))  // For TSP_VER_3_0 and TSP_VER_4_0
        case DMX_FILTER_TYPE_AUDIO3:
            *TspFltType = E_DRVTSP_FLT_TYPE_AUDIO3;
            break;
        case DMX_FILTER_TYPE_AUDIO4:
            *TspFltType = E_DRVTSP_FLT_TYPE_AUDIO4;
            break;
#endif
#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
        case DMX_FILTER_TYPE_AUDIO5:
            *TspFltType = E_DRVTSP_FLT_TYPE_AUDIO5;
            break;
        case DMX_FILTER_TYPE_AUDIO6:
            *TspFltType = E_DRVTSP_FLT_TYPE_AUDIO6;
            break;
#endif
        default:
            *TspFltType = 0;
            return FALSE;
    }

    return TRUE;
}

#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))) // For TSP_VER_4_0

static MS_U32 _DMX_ApiDrv_FltSrcMapping(MS_U32 u32TSPFltSource)
{
    switch (u32TSPFltSource & DMX_FILTER_SOURCE_TYPE_MASK)
    {
        case DMX_FILTER_SOURCE_TYPE_LIVE:
            return E_TSP_FLT_SRC_LIVE0;
        case DMX_FILTER_SOURCE_TYPE_TS1:
            return E_TSP_FLT_SRC_LIVE1;
        case DMX_FILTER_SOURCE_TYPE_TS2:
            return E_TSP_FLT_SRC_LIVE2;
        case DMX_FILTER_SOURCE_TYPE_TS3:
            return E_TSP_FLT_SRC_LIVE3;
        case DMX_FILTER_SOURCE_TYPE_TS4:
            return E_TSP_FLT_SRC_LIVE4;
        case DMX_FILTER_SOURCE_TYPE_TS5:
            return E_TSP_FLT_SRC_LIVE5;
        case DMX_FILTER_SOURCE_TYPE_TS6:
            return E_TSP_FLT_SRC_LIVE6;
        case DMX_FILTER_SOURCE_TYPE_FILE:
            return E_TSP_FLT_SRC_FILE0;
        case DMX_FILTER_SOURCE_TYPE_FILE1:
            return E_TSP_FLT_SRC_FILE1;
        case DMX_FILTER_SOURCE_TYPE_FILE2:
            return E_TSP_FLT_SRC_FILE2;
        case DMX_FILTER_SOURCE_TYPE_FILE3:
            return E_TSP_FLT_SRC_FILE3;
        case DMX_FILTER_SOURCE_TYPE_FILE4:
            return E_TSP_FLT_SRC_FILE4;
        case DMX_FILTER_SOURCE_TYPE_FILE5:
            return E_TSP_FLT_SRC_FILE5;
        case DMX_FILTER_SOURCE_TYPE_FILE6:
            return E_TSP_FLT_SRC_FILE6;
            // for backward compatible start
        case DMX_FILTER_SOURCE_TYPE_PVR0:
            return E_TSP_FLT_SRC_LIVE2;
        case DMX_FILTER_SOURCE_TYPE_PVR1:
            return E_TSP_FLT_SRC_LIVE3;
            // for backward compatible end

        default:
            DMX_DBGMSG(DMX_DBG_ERR,  ULOGE("DMX", "[MAPI DMX][%06d] Bad filter src %u\n", __LINE__, (unsigned int)u32TSPFltSource));
            return E_TSP_FLT_SRC_LIVE0;
    }
}

static MS_U32 _DMX_ApiDrv_FltSrcIdMapping(MS_U32 u32TSPFltSource)
{
    const MS_U32    u32ShiftNum = __builtin_ctz(E_TSP_FLT_SRCID_1 / DMX_FILTER_SOURCEID_1); // 12
    MS_U32          u32ApiSrcId = u32TSPFltSource & DMX_FILTER_SOURCEID_MASK;

    if(u32ApiSrcId < DMX_FILTER_SOURCEID_16)
    {
        return (E_TSP_FLT_SRCID_0 + (u32ApiSrcId << u32ShiftNum));
    }
    else
    {
        return (E_TSP_FLT_SRCID_16 + ((u32ApiSrcId & ~DMX_FILTER_SOURCEID_16) << u32ShiftNum));
    }
}

static TSP_FILE_ENG _DMX_ApiDrv_FileinEngMapping(DMX_FILEIN_PATH eFileinEng)
{
    switch (eFileinEng)
    {
        case DMX_FILEIN_TSIF0:
            return E_TSP_FILE_ENG_TSIF0;
        case DMX_FILEIN_TSIF1:
            return E_TSP_FILE_ENG_TSIF1;
        case DMX_FILEIN_TSIF2:
            return E_TSP_FILE_ENG_TSIF2;
        case DMX_FILEIN_TSIF3:
            return E_TSP_FILE_ENG_TSIF3;
        case DMX_FILEIN_TSIF4:
            return E_TSP_FILE_ENG_TSIF4;
        case DMX_FILEIN_TSIF5:
            return E_TSP_FILE_ENG_TSIF5;
        case DMX_FILEIN_TSIF6:
            return E_TSP_FILE_ENG_TSIF6;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad DMX_FILEIN_ENG %u\n", __LINE__, (unsigned int)eFileinEng));
            return E_TSP_FILE_ENG_INVALID;
    }
}

static MS_BOOL _DMX_ApiDrv_DmxFlowMapping(DMX_FLOW eDmxFlow,MS_U32 *pu32TSIf)
{
    *pu32TSIf = 0;

    switch (eDmxFlow)
    {
        case DMX_FLOW_PLAYBACK:
            *pu32TSIf = E_TSP_TSIF_0;
            break;
        case DMX_FLOW_PLAYBACK1:
            *pu32TSIf = E_TSP_TSIF_1;
            break;
        case DMX_FLOW_PLAYBACK2:
            *pu32TSIf = E_TSP_TSIF_2;
            break;
        case DMX_FLOW_PLAYBACK3:
            *pu32TSIf = E_TSP_TSIF_3;
            break;
        case DMX_FLOW_PLAYBACK4:
            *pu32TSIf = E_TSP_TSIF_4;
            break;
        case DMX_FLOW_PLAYBACK5:
            *pu32TSIf = E_TSP_TSIF_5;
            break;
        case DMX_FLOW_PLAYBACK6:
            *pu32TSIf = E_TSP_TSIF_6;
            break;
        case DMX_FLOW_PVR:
            *pu32TSIf = E_TSP_TSIF_PVR0;
            break;
        case DMX_FLOW_PVR1:
            *pu32TSIf = E_TSP_TSIF_PVR1;
            break;
        case DMX_FLOW_PVR2:
            *pu32TSIf = E_TSP_TSIF_PVR2;
            break;
        case DMX_FLOW_PVR3:
            *pu32TSIf = E_TSP_TSIF_PVR3;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Unsupported DMX_FLOW_INPUT type error!\n", __LINE__));
            return FALSE;
    }

    return TRUE;
}

static MS_BOOL _DMX_ApiDrv_DmxFlowReverseMapping(TSP_TSIF eTSIf, MS_U32 *pu32DmxFlow)
{
    *pu32DmxFlow = 0;

    switch (eTSIf)
    {
        case E_TSP_TSIF_0:
            *pu32DmxFlow = DMX_FLOW_PLAYBACK;
            break;
        case E_TSP_TSIF_1:
            *pu32DmxFlow = DMX_FLOW_PLAYBACK1;
            break;
        case E_TSP_TSIF_2:
            *pu32DmxFlow = DMX_FLOW_PLAYBACK2;
            break;
        case E_TSP_TSIF_3:
            *pu32DmxFlow = DMX_FLOW_PLAYBACK3;
            break;
        case E_TSP_TSIF_4:
            *pu32DmxFlow = DMX_FLOW_PLAYBACK4;
            break;
        case E_TSP_TSIF_5:
            *pu32DmxFlow = DMX_FLOW_PLAYBACK5;
            break;
        case E_TSP_TSIF_6:
            *pu32DmxFlow = DMX_FLOW_PLAYBACK6;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Unsupported TSP_TSIF type error!\n", __LINE__));
            return FALSE;
    }

    return TRUE;
}

static MS_BOOL _DMX_ApiDrv_PvrEngMapping(DMX_PVR_ENG eEng, MS_U32 *u32Eng)
{
    switch(eEng)
    {
        case DMX_PVR_EGN0 ... DMX_PVR_EGN5:
            *u32Eng = eEng;
            break;
        case DMX_PVR_EGN6 ... DMX_PVR_EGN9:
            *u32Eng = eEng - 1;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Unsupported Pvr Eng type error!\n", __LINE__));
            *u32Eng = 0;
            return FALSE;
    }

    return TRUE;
}

#endif  // End of TSP_VER_4_0

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
static DMX_FILTER_STATUS _DMX_Open_Stream(MS_U32* pu32DmxId, DMX_FILTER_TYPE DmxFltType)
{
    MS_U32              FltId = 0;
    MS_U32              u32TSPFltSource;
    DrvTSP_FltType      eTspFltType = (DrvTSP_FltType)DMX_FILTER_TYPE_VIDEO;

    u32TSPFltSource = DmxFltType & ((MS_U32)DMX_FILTER_FLT_MASK);
    DmxFltType = DmxFltType & (DMX_FILTER_TYPE)(~DMX_FILTER_FLT_MASK);

    if ((DMX_FILTER_TYPE_VIDEO!= DmxFltType) && (DMX_FILTER_TYPE_AUDIO!= DmxFltType) &&
        (DMX_FILTER_TYPE_AUDIO2!= DmxFltType) && (DMX_FILTER_TYPE_VIDEO3D!= DmxFltType)
  #if(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
         && (DMX_FILTER_TYPE_AUDIO3!= DmxFltType)  && (DMX_FILTER_TYPE_AUDIO4!= DmxFltType)
         && (DMX_FILTER_TYPE_SCMBCHK!= DmxFltType) && (DMX_FILTER_TYPE_REC!= DmxFltType)
  #endif
         )
    {
        DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Unknown error. Should NOT happen\n", __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }

    if (DMX_FILTER_TYPE_VIDEO== DmxFltType)
    {
        eTspFltType= E_DRVTSP_FLT_TYPE_VIDEO | (DrvTSP_FltType)u32TSPFltSource;
    }
    else if (DMX_FILTER_TYPE_AUDIO== DmxFltType)
    {
        eTspFltType= E_DRVTSP_FLT_TYPE_AUDIO | (DrvTSP_FltType)u32TSPFltSource;
    }
    else if (DMX_FILTER_TYPE_AUDIO2== DmxFltType)
    {
        eTspFltType= E_DRVTSP_FLT_TYPE_AUDIO2 | (DrvTSP_FltType)u32TSPFltSource;
    }
  #if(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
    else if (DMX_FILTER_TYPE_AUDIO3== DmxFltType)
    {
        eTspFltType= E_DRVTSP_FLT_TYPE_AUDIO3 | (DrvTSP_FltType)u32TSPFltSource;
    }
    else if (DMX_FILTER_TYPE_AUDIO4== DmxFltType)
    {
        eTspFltType= E_DRVTSP_FLT_TYPE_AUDIO4 | (DrvTSP_FltType)u32TSPFltSource;
    }
    else if (DMX_FILTER_TYPE_SCMBCHK== DmxFltType)
    {
        eTspFltType = E_DRVTSP_FLT_TYPE_PVR | (DrvTSP_FltType)u32TSPFltSource; //allocate filter from 127 to 0
    }
  #endif
    else if (DMX_FILTER_TYPE_VIDEO3D== DmxFltType)
    {
        eTspFltType= E_DRVTSP_FLT_TYPE_VIDEO3D | (DrvTSP_FltType)u32TSPFltSource;
    }
    else if (DMX_FILTER_TYPE_REC== DmxFltType)
    {
        eTspFltType= E_DRVTSP_FLT_TYPE_PVR | (DrvTSP_FltType)u32TSPFltSource;
    }

    if (DRVTSP_OK!= MDrv_TSP_PidFlt_Alloc(_Owner, eTspFltType, &FltId))
    {
        return DMX_FILTER_STATUS_ERROR;
    }

  #if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
    //For getting PID filter scrambled status only
    if (DMX_FILTER_TYPE_SCMBCHK== DmxFltType)
    {
        if(MDrv_TSP_PidFlt_SetInputSrc(_Owner, FltId, (DrvTSP_FltType)(u32TSPFltSource & (E_DRVTSP_FLT_SOURCE_TYPE_MASK|E_DRVTSP_FLT_SOURCEID_MASK))) != DRVTSP_OK)
        {
            return DMX_FILTER_STATUS_ERROR;
        }
    }
  #endif

    DMX_ASSERT(FltId< DMX_MAX_FLTID, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad FltId %u\n", __LINE__, (unsigned int)FltId)));
    _FLT_LIST_SECFLT_TYPE_SET(FltId, FltId, 0xFFFFFFFF, DmxFltType);
    *pu32DmxId = FltId;

    return DMX_FILTER_STATUS_OK;

}  // End of TSP_VER_1_0 and TSP_VER_3_0

#else  // For TSP_VER_4_0

static DMX_FILTER_STATUS _DMX_Open_Stream(MS_U32* pu32DmxId, DMX_FILTER_TYPE DmxFltType)
{
    MS_U32              FltId = 0xFFFFFFFF;
    MS_U32              u32TSPFltSource, u32tmpTSPFltSource;
    MS_U32              u32IFrameLUTEn = 0x0;
    DrvTSP_FltType      eTspFltType = (DrvTSP_FltType)DMX_FILTER_TYPE_VIDEO;

    if(DmxFltType & ((DMX_FILTER_TYPE)DMX_FILTER_TYPE_EX_I_FRAME_PVR))
    {
        u32IFrameLUTEn = E_TSP_DST_IFRAME_LUT;
    }

    u32TSPFltSource = DmxFltType & ((MS_U32)DMX_FILTER_FLT_MASK);
    DmxFltType = DmxFltType & (DMX_FILTER_TYPE)(~DMX_FILTER_FLT_MASK);

    u32tmpTSPFltSource = _DMX_ApiDrv_FltSrcMapping(u32TSPFltSource);
    u32tmpTSPFltSource |= _DMX_ApiDrv_FltSrcIdMapping(u32TSPFltSource);
    u32TSPFltSource = u32tmpTSPFltSource;

    switch(DmxFltType)
    {
        case DMX_FILTER_TYPE_VIDEO:
            eTspFltType= E_DRVTSP_FLT_TYPE_VIDEO | (DrvTSP_FltType)u32TSPFltSource;
            break;
        case DMX_FILTER_TYPE_VIDEO3D:
            eTspFltType= E_DRVTSP_FLT_TYPE_VIDEO3D | (DrvTSP_FltType)u32TSPFltSource;
            break;
        case DMX_FILTER_TYPE_VIDEO3:
            eTspFltType= E_DRVTSP_FLT_TYPE_VIDEO3 | (DrvTSP_FltType)u32TSPFltSource;
            break;
        case DMX_FILTER_TYPE_VIDEO4:
            eTspFltType= E_DRVTSP_FLT_TYPE_VIDEO4 | (DrvTSP_FltType)u32TSPFltSource;
            break;
        case DMX_FILTER_TYPE_VIDEO5:
            eTspFltType= E_DRVTSP_FLT_TYPE_VIDEO5 | (DrvTSP_FltType)u32TSPFltSource;
            break;
        case DMX_FILTER_TYPE_VIDEO6:
            eTspFltType= E_DRVTSP_FLT_TYPE_VIDEO6 | (DrvTSP_FltType)u32TSPFltSource;
            break;
        case DMX_FILTER_TYPE_VIDEO7:
            eTspFltType= E_DRVTSP_FLT_TYPE_VIDEO7 | (DrvTSP_FltType)u32TSPFltSource;
            break;
        case DMX_FILTER_TYPE_VIDEO8:
            eTspFltType= E_DRVTSP_FLT_TYPE_VIDEO8 | (DrvTSP_FltType)u32TSPFltSource;
            break;
        case DMX_FILTER_TYPE_AUDIO:
            eTspFltType= E_DRVTSP_FLT_TYPE_AUDIO | (DrvTSP_FltType)u32TSPFltSource;
            break;
        case DMX_FILTER_TYPE_AUDIO2:
            eTspFltType= E_DRVTSP_FLT_TYPE_AUDIO2 | (DrvTSP_FltType)u32TSPFltSource;
            break;
        case DMX_FILTER_TYPE_AUDIO3:
            eTspFltType= E_DRVTSP_FLT_TYPE_AUDIO3 | (DrvTSP_FltType)u32TSPFltSource;
            break;
        case DMX_FILTER_TYPE_AUDIO4:
            eTspFltType= E_DRVTSP_FLT_TYPE_AUDIO4 | (DrvTSP_FltType)u32TSPFltSource;
            break;
        case DMX_FILTER_TYPE_AUDIO5:
            eTspFltType= E_DRVTSP_FLT_TYPE_AUDIO5 | (DrvTSP_FltType)u32TSPFltSource;
            break;
        case DMX_FILTER_TYPE_AUDIO6:
            eTspFltType= E_DRVTSP_FLT_TYPE_AUDIO6 | (DrvTSP_FltType)u32TSPFltSource;
            break;
        case DMX_FILTER_TYPE_REC:
            eTspFltType= E_DRVTSP_FLT_TYPE_REC | (DrvTSP_FltType)u32TSPFltSource;
            break;
        case DMX_FILTER_TYPE_PVR:
            eTspFltType= E_DRVTSP_FLT_TYPE_PVR | (DrvTSP_FltType)u32TSPFltSource | u32IFrameLUTEn;
            break;
        case DMX_FILTER_TYPE_PVR1:
            eTspFltType= E_DRVTSP_FLT_TYPE_PVR1 | (DrvTSP_FltType)u32TSPFltSource | u32IFrameLUTEn;
            break;
        case DMX_FILTER_TYPE_PVR2:
            eTspFltType= E_DRVTSP_FLT_TYPE_PVR2 | (DrvTSP_FltType)u32TSPFltSource | u32IFrameLUTEn;
            break;
        case DMX_FILTER_TYPE_PVR3:
            eTspFltType= E_DRVTSP_FLT_TYPE_PVR3 | (DrvTSP_FltType)u32TSPFltSource | u32IFrameLUTEn;
            break;
        case DMX_FILTER_TYPE_PVR4:
            eTspFltType= E_DRVTSP_FLT_TYPE_PVR4 | (DrvTSP_FltType)u32TSPFltSource | u32IFrameLUTEn;
            break;
        case DMX_FILTER_TYPE_PVR5:
            eTspFltType= E_DRVTSP_FLT_TYPE_PVR5 | (DrvTSP_FltType)u32TSPFltSource | u32IFrameLUTEn;
            break;
        case DMX_FILTER_TYPE_PVR6:
            eTspFltType= E_DRVTSP_FLT_TYPE_PVR6 | (DrvTSP_FltType)u32TSPFltSource | u32IFrameLUTEn;
            break;
        case DMX_FILTER_TYPE_PVR7:
            eTspFltType= E_DRVTSP_FLT_TYPE_PVR7 | (DrvTSP_FltType)u32TSPFltSource | u32IFrameLUTEn;
            break;
        case DMX_FILTER_TYPE_PVR8:
            eTspFltType= E_DRVTSP_FLT_TYPE_PVR8 | (DrvTSP_FltType)u32TSPFltSource | u32IFrameLUTEn;
            break;
        case DMX_FILTER_TYPE_PVR9:
            eTspFltType= E_DRVTSP_FLT_TYPE_PVR9 | (DrvTSP_FltType)u32TSPFltSource | u32IFrameLUTEn;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Unknown error. Should NOT happen\n", __LINE__));
            return DMX_FILTER_STATUS_ERROR;
    }

    if(_pdmx_res->_bIsDMXOpenCalled)
    {
        if (DRVTSP_OK!= MDrv_TSP_PidFlt_Alloc(_Owner, eTspFltType |u32TSPFltSource , &FltId))
        {
            return DMX_FILTER_STATUS_ERROR;
        }
    }
    else if(_pdmx_res->_bIsDMXOpenExCalled)
    {
        if (DRVTSP_OK!= MDrv_TSP_PidFlt_Alloc_Ex(_Owner, eTspFltType | u32TSPFltSource, &FltId))
        {
            return DMX_FILTER_STATUS_ERROR;
        }
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    DMX_ASSERT(FltId< DMX_MAX_FLTID, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad FltId %u\n", __LINE__, (unsigned int)FltId)));

    _FLT_LIST_SECFLT_TYPE_SET(FltId, FltId, 0xFFFFFFFF, DmxFltType);
    *pu32DmxId = FltId;

    return DMX_FILTER_STATUS_OK;
}
#endif // End of TSP_VER_4_0

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
static DMX_FILTER_STATUS _DMX_Open_Sect(MS_U32* pu32DmxId, DMX_FILTER_TYPE DmxFltType)
{
    MS_U32              FltId= 0;
    MS_U32              SecBufId= 0xFFFFFFFF;
    MS_U32              u32TSPFltSource;
    DrvTSP_FltType      eTspFltType = E_DRVTSP_FLT_TYPE_VIDEO;

    *pu32DmxId = 0xFFFFFFFF;
    u32TSPFltSource = DmxFltType & DMX_FILTER_FLT_MASK;
    DmxFltType = DmxFltType & (DMX_FILTER_TYPE)(~DMX_FILTER_FLT_MASK);

    DMX_ASSERT(
        (DMX_FILTER_TYPE_SECTION== DmxFltType) || (DMX_FILTER_TYPE_TELETEXT== DmxFltType) ||
        (DMX_FILTER_TYPE_PES== DmxFltType) || (DMX_FILTER_TYPE_PACKET== DmxFltType) ||
        (DMX_FILTER_TYPE_PCR== DmxFltType) || (DMX_FILTER_TYPE_SECTION_VER== DmxFltType) ||
        (DMX_FILTER_TYPE_SECTION_NO_PUSI == DmxFltType)

        , DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Unknown error. Should NOT happen\n", __LINE__)));

    switch (DmxFltType)
    {
        case DMX_FILTER_TYPE_SECTION:
            eTspFltType=            E_DRVTSP_FLT_TYPE_SECTION;
            break;
        case DMX_FILTER_TYPE_TELETEXT:
            eTspFltType=            E_DRVTSP_FLT_TYPE_TELETEXT;
            break;
        case DMX_FILTER_TYPE_PES:
            eTspFltType=            E_DRVTSP_FLT_TYPE_PES;
            break;
        case DMX_FILTER_TYPE_PACKET:
            eTspFltType=            E_DRVTSP_FLT_TYPE_PACKET;
            break;
        case DMX_FILTER_TYPE_PCR:
            eTspFltType=            E_DRVTSP_FLT_TYPE_PCR;
            break;
        case DMX_FILTER_TYPE_SECTION_NO_PUSI:
            eTspFltType=            E_DRVTSP_FLT_TYPE_SECTION_NO_PUSI;
            break;
        case DMX_FILTER_TYPE_SECTION_VER:
            eTspFltType =           E_DRVTSP_FLT_TYPE_SECTION_VER;
            break;
    }

    if (DRVTSP_OK!= MDrv_TSP_PidFlt_Alloc(_Owner , (DrvTSP_FltType)(eTspFltType|u32TSPFltSource), &FltId))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    DMX_ASSERT(FltId< DMX_MAX_FLTID, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad FltId %u\n", __LINE__, (unsigned int)FltId)));

    if ((DMX_FILTER_TYPE_PCR== DmxFltType)) // open pid filter only for pcr since we have HWPCR now
    {
        goto _Open_Sect_end;
    }

    SecBufId = FltId;

    if (DRVTSP_OK!= MDrv_TSP_SecFlt_Alloc(_Owner, &SecBufId))
    {
        MDrv_TSP_PidFlt_Free(0 , FltId);
        return DMX_FILTER_STATUS_ERROR;
    }

    MDrv_TSP_PidFlt_SelSecFlt(0, FltId, SecBufId);

_Open_Sect_end:

    _FLT_LIST_SECFLT_TYPE_SET(FltId, FltId, SecBufId, DmxFltType);
    *pu32DmxId = (MS_U8)FltId;

    return DMX_FILTER_STATUS_OK;

}  // End of TSP_VER_1_0 and TSP_VER_3_0

#else  // For TSP_VER_4_0

static DMX_FILTER_STATUS _DMX_Open_Sect(MS_U32* pu32DmxId, DMX_FILTER_TYPE DmxFltType)
{
    MS_U32              FltId= 0xFFFFFFFF;
    MS_U32              SecBufId= 0xFFFFFFFF;
    MS_U32              u32TSPFltSource, u32tmpTSPFltSource;
    DrvTSP_FltType      eTspFltType = E_DRVTSP_FLT_TYPE_VIDEO;

    u32TSPFltSource = DmxFltType & ((MS_U32)DMX_FILTER_FLT_MASK);
    DmxFltType = DmxFltType & (DMX_FILTER_TYPE)(~DMX_FILTER_FLT_MASK);

    DMX_ASSERT(
        (DMX_FILTER_TYPE_SECTION== DmxFltType) || (DMX_FILTER_TYPE_TELETEXT== DmxFltType) ||
        (DMX_FILTER_TYPE_PES== DmxFltType) || (DMX_FILTER_TYPE_PACKET== DmxFltType) ||
        (DMX_FILTER_TYPE_PCR== DmxFltType),
        DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Unknown error. Should NOT happen\n", __LINE__)));

    u32tmpTSPFltSource = _DMX_ApiDrv_FltSrcMapping(u32TSPFltSource);
    u32tmpTSPFltSource |= _DMX_ApiDrv_FltSrcIdMapping(u32TSPFltSource);
    u32TSPFltSource = u32tmpTSPFltSource;

    switch (DmxFltType)
    {
        case DMX_FILTER_TYPE_SECTION:
            eTspFltType=            E_DRVTSP_FLT_TYPE_SECTION;
            break;
        case DMX_FILTER_TYPE_TELETEXT:
            eTspFltType=            E_DRVTSP_FLT_TYPE_TELETEXT;
            break;
        case DMX_FILTER_TYPE_PES:
            eTspFltType=            E_DRVTSP_FLT_TYPE_PES;
            break;
        case DMX_FILTER_TYPE_PACKET:
            eTspFltType=            E_DRVTSP_FLT_TYPE_PACKET;
            break;
        case DMX_FILTER_TYPE_PCR:
            eTspFltType=            E_DRVTSP_FLT_TYPE_PCR;
            break;
    }

    if(_pdmx_res->_bIsDMXOpenCalled)
    {
        if (DRVTSP_OK!= MDrv_TSP_PidFlt_Alloc(_Owner, eTspFltType | u32TSPFltSource, &FltId))
        {
            return DMX_FILTER_STATUS_ERROR;
        }
    }
    else if(_pdmx_res->_bIsDMXOpenExCalled)
    {
        if (DRVTSP_OK!= MDrv_TSP_PidFlt_Alloc_Ex(_Owner, eTspFltType  | u32TSPFltSource, &FltId))
        {
            return DMX_FILTER_STATUS_ERROR;
        }
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    DMX_ASSERT(FltId< DMX_MAX_FLTID, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad FltId %u\n", __LINE__, (unsigned int)FltId)));

    if (DMX_FILTER_TYPE_PCR== DmxFltType)
    {
        //set pcr src
        MDrv_TSP_FLT_SetPCRSrc(FltId,u32TSPFltSource);
        goto _Open_Sect_end;
    }

    if ( (DMX_FILTER_TYPE_SECTION == DmxFltType) || (DMX_FILTER_TYPE_PES == DmxFltType) ||
        (DMX_FILTER_TYPE_PACKET == DmxFltType) || (DMX_FILTER_TYPE_TELETEXT == DmxFltType) )
    {
        if (DRVTSP_OK!= MDrv_TSP_SecFlt_Alloc(_Owner, &SecBufId))
        {
            MDrv_TSP_PidFlt_Free(0 , FltId);
            return DMX_FILTER_STATUS_ERROR;
        }

        MDrv_TSP_PidFlt_SelSecFlt(0, FltId, SecBufId);
    }
_Open_Sect_end:
// @F_TODO check if DmxFltType is use else where for sw status then use it to set HW status
    _FLT_LIST_SECFLT_TYPE_SET(FltId, FltId, SecBufId, DmxFltType);

    *pu32DmxId = FltId;
    return DMX_FILTER_STATUS_OK;
}
#endif  // End of TSP_VER_4_0

#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))) // For TSP_VER_4_0
static MS_BOOL _DMX_PVR_FlowSet(DMX_PVR_ENG eEng, DMX_TSIF ePvrSrcTSIf, MS_BOOL bDscmbRec)
{
    MS_U32 u32Eng = 0;
    MS_U32 u32PVRSrc = 0;

    if(_DMX_ApiDrv_PvrEngMapping(eEng, &u32Eng) == FALSE)
        return FALSE;

    switch (ePvrSrcTSIf)
    {
        case DMX_TSIF_LIVE0:
            u32PVRSrc = E_TSP_FLT_SRC_LIVE0;
            break;
        case DMX_TSIF_LIVE1:
            u32PVRSrc = E_TSP_FLT_SRC_LIVE1;
            break;
        case DMX_TSIF_LIVE2:
            u32PVRSrc = E_TSP_FLT_SRC_LIVE2;
            break;
        case DMX_TSIF_LIVE3:
            u32PVRSrc = E_TSP_FLT_SRC_LIVE3;
            break;
        case DMX_TSIF_FILE0:
            u32PVRSrc = E_TSP_FLT_SRC_FILE0;
            break;
        case DMX_TSIF_FILE1:
            u32PVRSrc = E_TSP_FLT_SRC_FILE1;
            break;
        case DMX_TSIF_FILE2:
            u32PVRSrc = E_TSP_FLT_SRC_FILE2;
            break;
        case DMX_TSIF_FILE3:
            u32PVRSrc = E_TSP_FLT_SRC_FILE3;
            break;
        case DMX_TSIF_LIVE4:
        case DMX_TSIF_FILE4:
            u32PVRSrc = E_TSP_FLT_SRC_LIVE4;
            break;
        case DMX_TSIF_LIVE5:
        case DMX_TSIF_FILE5:
            u32PVRSrc = E_TSP_FLT_SRC_LIVE5;
            break;
        case DMX_TSIF_LIVE6:
        case DMX_TSIF_FILE6:
            u32PVRSrc = E_TSP_FLT_SRC_LIVE6;
            break;
        default:
            return FALSE;
    }

    MDrv_TSP_PVR_SetFlowSource(u32Eng,u32PVRSrc);

    return TRUE;

}
#endif  // End of TSP_VER_4_0

#ifdef TSO_20_ENABLE
static MS_BOOL _TSO_Flow_InputCfg(DMX_TSO_InputCfg* pstInputCfg)
{
    MS_U8 u8Eng = 0;
    DrvTSO_If_Set IfSet;
    DrvTSO_If eIf = E_DRVTSO_IF_LIVE0;
    DrvTSO_CtrlMode eCtrMode = E_DRVTSO_CTRL_MODE_TS0;
    //MS_U32 u32timeout = 3000, u32StartTime = 0;
    //MS_BOOL bSVQBusy = TRUE;

    if(pstInputCfg == NULL)
        return FALSE;

    switch(pstInputCfg->eFlow)
    {
        case DMX_FLOW_TSO_PLAYBACK:
        case DMX_FLOW_TSO_MMT:
            u8Eng = 0;
            break;
        default:
            return FALSE;
    }

    switch(pstInputCfg->eTSOInIf)
    {
        case DMX_TSIF_LIVE0:
            eIf = E_DRVTSO_IF_LIVE0;
            eCtrMode = E_DRVTSO_CTRL_MODE_TS0;
            break;
        case DMX_TSIF_LIVE1:
            eIf = E_DRVTSO_IF_LIVE1;
            eCtrMode = E_DRVTSO_CTRL_MODE_TS1;
            break;
        case DMX_TSIF_LIVE2:
            eIf = E_DRVTSO_IF_LIVE2;
            eCtrMode = E_DRVTSO_CTRL_MODE_TS2;
            break;
        case DMX_TSIF_LIVE3:
            eIf = E_DRVTSO_IF_LIVE3;
            eCtrMode = E_DRVTSO_CTRL_MODE_TS3;
            break;
        case DMX_TSIF_FILE0:
            eIf = E_DRVTSO_IF_FILE0;
            eCtrMode = E_DRVTSO_CTRL_MODE_MEM;
            break;
        case DMX_TSIF_FILE1:
            eIf = E_DRVTSO_IF_FILE1;
            eCtrMode = E_DRVTSO_CTRL_MODE_MEM1;
            break;
        case DMX_TSIF_MMT:
            eIf = E_DRVTSO_IF_MMT0;
            eCtrMode = E_DRVTSO_CTRL_MODE_MMT;
            break;
        default:
            return FALSE;
    }

    if(pstInputCfg->eFlow != DMX_FLOW_TSO_MMT)
    {
        //Disable CH Interface first
        if(DRVTSO_OK != MDrv_TSO_Ch_InputEnable(u8Eng, eIf, FALSE))
        {
            return FALSE;
        }

        //-------   Check SVQ TX empty  ----------------------
        #if 0
        u32StartTime = MsOS_GetSystemTime();
        while((MsOS_GetSystemTime() - u32StartTime) < u32timeout)
        {
            if((bSVQBusy = MDrv_TSO_Check_IsSVQTxBusy(u8Eng, eIf)) == FALSE)
            {
                break;
            }
            MsOS_DelayTask(1);
        }
        if(bSVQBusy == TRUE)
            return FALSE;

        bSVQBusy = TRUE;
        u32StartTime = MsOS_GetSystemTime();
        while((MsOS_GetSystemTime() - u32StartTime) < u32timeout)
        {
            if((bSVQBusy = !MDrv_TSO_Check_IsSVQTxEmpty(u8Eng, eIf)) == FALSE)
            {
                break;
            }
            MsOS_DelayTask(1);
        }

        if(bSVQBusy == TRUE)
            return FALSE;

        #endif
        //------------------------------------------------------

        if(DRVTSO_OK!= MDrv_TSO_LocalStreamId(u8Eng, eIf, &(pstInputCfg->u8LocalStrId), TRUE))
        {
            return FALSE;
        }
    }

    if((pstInputCfg->eTSOInIf != DMX_TSIF_FILE0) && (pstInputCfg->eTSOInIf != DMX_TSIF_FILE1))
    {
        IfSet.bClkInv = pstInputCfg->stInputInfo.bClkInv;
        IfSet.bParallel = pstInputCfg->stInputInfo.bParallel;
        IfSet.bExtSync = pstInputCfg->stInputInfo.bExtSync;
        IfSet.ePad = 0;
        switch (pstInputCfg->stInputInfo.Input)
        {
            case DMX_FLOW_INPUT_DEMOD:
                IfSet.ePad = E_DRVTSO_PAD_DEMOD;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT0:
                IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT0;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT1:
                IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT1;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT2:
                IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT2;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT3:
                IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT3;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT4:
                IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT4;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT5:
                IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT5;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT0_3WIRE:
                IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT0_3WIRE;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT1_3WIRE:
                IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT1_3WIRE;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT2_3WIRE:
                IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT2_3WIRE;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT3_3WIRE:
                IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT3_3WIRE;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT4_3WIRE:
                IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT4_3WIRE;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT5_3WIRE:
                IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT5_3WIRE;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT6_3WIRE:
                IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT6_3WIRE;
                break;
            default:
                return FALSE;
        }

        if(MDrv_TSO_SelPad(u8Eng, eIf, &IfSet) != DRVTSO_OK)
        {
            return DMX_FILTER_STATUS_ERROR;
        }
    }

    if(MDrv_TSO_SetOperateMode(u8Eng, eCtrMode, pstInputCfg->bBypassAll, pstInputCfg->bEnable) != DRVTSO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    return TRUE;

}

static MS_BOOL _TSO_Flow_OutputCfg(DMX_TSO_OutputCfg* pstOutputCfg)
{
    MS_U8 u8Eng = 0;

    DrvTSO_OutClkSrc eOutClkSrc = E_DRVTSO_OUTCLKSRC_172M_2N;

    //default: 172.8/2(15+1) = 5.4M
    DrvTSO_OutClk eOutClk = E_DRVTSO_OUTCLK_DIV2N;
    MS_U16  u16DivNum = 0x0F;

    MS_BOOL bClkIv = FALSE;
    //DrvTSO_PadOut eOutPad = (DrvTSO_PadOut)(pstOutputCfg->eOutPad);

    if(pstOutputCfg == NULL)
        return FALSE;

    /*if(MDrv_TSO_OutputEnable(u8Eng, FALSE) != DRVTSO_OK)
    {
        return FALSE;
    }*/

    switch(pstOutputCfg->eFlow)
    {
        case DMX_FLOW_TSO_PLAYBACK:
            u8Eng = 0;
            break;
        default:
            return FALSE;
    }

#if !defined(TSO_HW_BOX_SERIES) //[temporarily]
    if(pstOutputCfg->eOutPad != E_DRVTSP_OUTPAD_NONE)
    {
        if(MDrv_TSP_OutputPadCfg(pstOutputCfg->eOutPad|E_DRVTSP_OUTPAD_FROM_TSO, E_DRVTSP_PAD_TSO, TRUE, TRUE) != DRVTSP_OK)
        {
            ULOGD("DMX", "[%s][%d] \n", __FUNCTION__, __LINE__);
            return FALSE;
        }
    }
#endif

    //default output pad select
    /*if(MDrv_TSO_OutputPad(0, &eOutPad, TRUE) != DRVTSO_OK)
    {
        return FALSE;
    }*/

    //Setting default output clock
    if(pstOutputCfg->bDefOutClk == FALSE)
    {
        eOutClk = pstOutputCfg->eTsoOutClk;
        eOutClkSrc = pstOutputCfg->eTsoOutClkSrc;
        u16DivNum = pstOutputCfg->u16DivNum;
        bClkIv = pstOutputCfg->bOutClkInv;
    }

    if(MDrv_TSO_SetOutClk(u8Eng, eOutClk, eOutClkSrc, u16DivNum, bClkIv) != DRVTSO_OK)
    {
        return FALSE;
    }

    if(MDrv_TSO_OutputPktSize(u8Eng, &(pstOutputCfg->u16OutPktSize), TRUE) != DRVTSO_OK)
    {
        return FALSE;
    }

    /*if(MDrv_TSO_OutputEnable(u8Eng, pstOutputCfg->bEnable) != DRVTSO_OK)
    {
        return FALSE;
    }*/
    return TRUE;
}

#elif (defined(TSO_ENABLE) && (!defined(TSO_HW_BOX_SERIES)))
//TSO HW 1.0 for TV series
static MS_BOOL _DMX_FlowSetTSO(DMX_FLOW DmxFlow, DMX_FLOW_INPUT DmxFlowInput, MS_BOOL bClkInv, MS_BOOL bExtSync, MS_BOOL bParallel)
{
    DrvTSO_OutClk eOutClk = E_DRVTSO_OUTCLK_DIV2N;
    DrvTSO_OutClkSrc eOutClkSrc = E_DRVTSO_OUTCLKSRC_172M_2N;
    DrvTSO_If_Set IfSet;
    DrvTSO_CtrlMode eCtrMode = E_DRVTSO_CTRL_MODE_TS0;
    MS_U16  u16DivNum = 0;
    MS_U8 u8Eng = 0;

    switch(DmxFlow)
    {
        case DMX_FLOW_TSO_PLAYBACK:
            u8Eng = 0;
            eCtrMode = E_DRVTSO_CTRL_MODE_TS0;
            break;

        default:
            return FALSE;
    }

    IfSet.bClkInv = FALSE;
    IfSet.bParallel = bParallel;
    IfSet.bExtSync = bExtSync;
    IfSet.ePad = 0;
    switch (DmxFlowInput)
    {
        case DMX_FLOW_INPUT_DEMOD:
            IfSet.ePad = E_DRVTSO_PAD_DEMOD;
            eOutClk = E_DRVTSO_OUTCLK_Dmd;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT0:
            IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT0;
            eOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
            eOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS0IN;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT1:
            IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT1;
            eOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
            eOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS1IN;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT2:
            IfSet.ePad = E_DRVTSO_PAD_EXT_INPUT2;
            eOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
            eOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS2IN;
            break;
        case DMX_FLOW_INPUT_MEM:
            eCtrMode = E_DRVTSO_CTRL_MODE_MEM;
            //default: 172.8/2(15+1) = 5.4M
            IfSet.ePad = E_DRVTSO_MEM;
            eOutClk = E_DRVTSO_OUTCLK_DIV2N;
            eOutClkSrc = E_DRVTSO_OUTCLKSRC_172M_2N;
            u16DivNum = 0x0F;
            break;

        default:
            return FALSE;
    }

    if(MDrv_TSO_SetOutClk(u8Eng, eOutClk, eOutClkSrc, u16DivNum, bClkInv) != DRVTSO_OK)
    {
        return FALSE;
    }

    if(MDrv_TSO_SelPad(u8Eng, &IfSet) != DRVTSO_OK)
    {
        return FALSE;
    }

    if(MDrv_TSO_SetOperateMode(u8Eng, eCtrMode, bClkInv, bParallel) != DRVTSO_OK)
    {
        return FALSE;
    }

    return TRUE;
}

#else

#ifdef TSO_ENABLE
//TSO HW 1.0 for BOX series
static MS_BOOL _DMX_FlowSetTSO(DMX_FLOW DmxFlow, DMX_FLOW_INPUT DmxFlowInput, MS_BOOL bClkInv, MS_BOOL bExtSync, MS_BOOL bParallel)
{

    DrvTSO_CtrlMode eDrvTSOCtrlMode= E_DRVTSO_CTRL_MODE_INVALID;
    MS_U8 u8TSOEng = 0;
    TSP_TSPad ePad = E_TSP_TS_INVALID;
    // STB drvTSP has different enum from TV's
    switch (DmxFlowInput)
    {
        case DMX_FLOW_INPUT_DEMOD:
            ePad = E_TSP_TS_DEMOD0;
            eDrvTSOCtrlMode = E_DRVTSO_CTRL_MODE_DEMOD0;
            break;
        case DMX_FLOW_INPUT_DEMOD1:
            ePad = E_TSP_TS_DEMOD1;
            eDrvTSOCtrlMode = E_DRVTSO_CTRL_MODE_DEMOD1;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT0:
            ePad = E_TSP_TS_PAD0;
            eDrvTSOCtrlMode = E_DRVTSO_CTRL_MODE_TS0;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT1:
            ePad = E_TSP_TS_PAD1;
            eDrvTSOCtrlMode = E_DRVTSO_CTRL_MODE_TS1;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT2:
            ePad = E_TSP_TS_PAD2;
            eDrvTSOCtrlMode = E_DRVTSO_CTRL_MODE_TS2;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT3:
            ePad = E_TSP_TS_PAD3;
            eDrvTSOCtrlMode = E_DRVTSO_CTRL_MODE_TS3;
            break;
        case DMX_FLOW_INPUT_MEM:
            ePad = E_TSP_TS_PAD0;
            eDrvTSOCtrlMode = E_DRVTSO_CTRL_MODE_MEM;
            break;
        case DMX_FLOW_INPUT_DISABLE:
            ePad = E_TSP_TS_PAD0;
            eDrvTSOCtrlMode = E_DRVTSO_CTRL_MODE_DEMOD0;
            break;

        case DMX_FLOW_INPUT_TSO:// TSO cannot input from TSO
        case DMX_FLOW_INPUT_TSO1:// TSO cannot input from TSO
        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Unsupported DMX_FLOW_INPUT type error!\n", __LINE__));
            return FALSE;
    }

    switch (DmxFlow)
    {
        case DMX_FLOW_TSO_PLAYBACK:
            u8TSOEng = 0;
            break;
        case DMX_FLOW_TSO_PLAYBACK1:
            u8TSOEng = 1;
            break;

        default:
            return FALSE;
    }
    MDrv_TSO_SetOperateMode(u8TSOEng, (eDrvTSOCtrlMode & 0xFF), bClkInv, bParallel);
    MDrv_TSP_TSO_ConfigPad(u8TSOEng, ePad);
//        MDrv_TSO_SetTSO_FROM_TSOUT_MUX; // @F_TODO not implement yet...

    return TRUE;
}
#endif //end of TSO_ENABLE

#endif //end of TSO_HW_BOX_SERIES

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
//-------------------------------------------------------------------------
// For TSP version 3.0:
//      DMX_FLOW_PLAYBACK                                    : TSIF0 / PIDFLT0 / Live and File
//      DMX_FLOW_PVR                                              : TSIF1 / PIDFLT1 / Rec0 (Could change input source later)
//      DMX_FLOW_PVR1                                             : TSIF2 / PIDFLT2 /Rec1  (Could change input source later)
//      DMX_FLOW_PVRCA                                          : TSIF0 / PIDFLT0 /Rec0 with DSCMB0
//      DMX_FLOW_MMFI0 (DMX_FLOW_FILEIN_MM)      : MMFI Audio / MMFIA PIDFLT / File audio
//      DMX_FLOW_MMFI1 (DMX_FLOW_FILEIN_MM3D)  : MMFI V3D / MMFIV3D PIDFLT / File V3D
//      DMX_FLOW_CIPHSS_PLAYBACK                        : TS1 / PIDFLTF for DSCMB / Live for SCMB packet
//      DMX_FLOW_CIPHSS_PVRCA                             : TS1 / PIDFLT1 for DSCMB / Rec0 with DSCMB
//      DMX_FLOW_PVR2                                             : TSIFCB / PIDFLTCB /RecCB
//
//      DMX_FLOW_TSO_PLAYBACK                             : TSOIF / PIDFLTSO /Live and File
//
//      DMX_FLOW_PLAYBACK1                                   : TSIF1 / PIDFLT1 / Live
//      DMX_FLOW_PVRCA1                                        : TSIF1/ PIDFLT1 /Rec1 with DSCMB1
//--------------------------------------------------------------------------
static MS_BOOL _DMX_FlowSet(DMX_FLOW DmxFlow, DMX_FLOW_INPUT DmxFlowInput, MS_BOOL bClkInv, MS_BOOL bExtSync, MS_BOOL bParallel)
{
    DrvTSP_If_Set IfSet;
    MS_U32  u32DScmbEngId = 0xFF;
    MS_BOOL bFileMode = FALSE;
    MS_BOOL bPvrBlockEnable = FALSE;

  #ifdef TSO_20_ENABLE

    if(DmxFlow == DMX_FLOW_TSO_PLAYBACK)
    {
        DMX_TSO_OutputCfg stOutputCfg;

        stOutputCfg.bDefOutClk = FALSE;
        stOutputCfg.bOutClkInv = FALSE;
        switch (DmxFlowInput)
        {
            case DMX_FLOW_INPUT_DEMOD:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_Dmd;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT0:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
                stOutputCfg.eTsoOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS0IN;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT1:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
                stOutputCfg.eTsoOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS1IN;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT2:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
                stOutputCfg.eTsoOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS2IN;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT3:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
                stOutputCfg.eTsoOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS3IN;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT4:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
                stOutputCfg.eTsoOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS4IN;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT5:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
                stOutputCfg.eTsoOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS5IN;
                break;
            case DMX_FLOW_INPUT_MEM:
            case DMX_FLOW_INPUT_MEM1:
            default:
               stOutputCfg.bDefOutClk = TRUE;
               break;
        }

        stOutputCfg.eFlow = DmxFlow;
        stOutputCfg.eOutPad = DMX_FLOW_OUTPUT_EXT_PAD1;
        stOutputCfg.u16OutPktSize = 0xBC;
        stOutputCfg.bEnable = TRUE;
        stOutputCfg.bSet = TRUE;
        if(_TSO_Flow_OutputCfg(&stOutputCfg) == FALSE)
            return FALSE;
    }

    if((DmxFlow == DMX_FLOW_TSO_PLAYBACK) || (DmxFlow == DMX_FLOW_TSO_MMT))
    {
        DMX_TSO_InputCfg stInputCfg;

        stInputCfg.eFlow = DmxFlow;
        stInputCfg.stInputInfo.Input = DmxFlowInput;
        stInputCfg.stInputInfo.bClkInv = bClkInv;
        stInputCfg.stInputInfo.bExtSync = bExtSync;
        stInputCfg.stInputInfo.bParallel = bParallel;
        stInputCfg.u8LocalStrId = 0x47;
        stInputCfg.bBypassAll = TRUE;
        stInputCfg.bEnable = TRUE;
        stInputCfg.bSet = TRUE;
        if(DMX_FLOW_TSO_PLAYBACK == DmxFlow)
        {
            if(DmxFlowInput == DMX_FLOW_INPUT_MEM)
            {
                stInputCfg.eTSOInIf = DMX_TSIF_FILE0;
            }
            else if(DmxFlowInput == DMX_FLOW_INPUT_MEM1)
            {
                stInputCfg.eTSOInIf = DMX_TSIF_FILE1;
            }
            else
            {
                stInputCfg.eTSOInIf = DMX_TSIF_LIVE0;
            }
        }
        else if(DMX_FLOW_TSO_MMT == DmxFlow)
        {
            stInputCfg.eTSOInIf = DMX_TSIF_MMT;
        }

        return _TSO_Flow_InputCfg(&stInputCfg);
    }

  #endif  // End of TSO_20_ENABLE

    if ((DMX_FLOW_INPUT_MEM == DmxFlowInput) || (DMX_FLOW_INPUT_MEM_NOPASSCA == DmxFlowInput)
        || (DMX_FLOW_INPUT_MEM_PASSCA1 == DmxFlowInput))
    {
        bPvrBlockEnable = TRUE;

        if(DMX_FLOW_PLAYBACK == DmxFlow)
        {
            if(DmxFlowInput == DMX_FLOW_INPUT_MEM)
            {
                MDrv_TSP_GetDscmbEngIdx_BySource(E_DRVTSP_PKTSRC_DEMUXFILE, &u32DScmbEngId);
                MDrv_TSP_SetOperateMode(0, E_DRVTSP_CTRL_MODE_MEM, (MS_U8)u32DScmbEngId);
            }
            else if(DmxFlowInput == DMX_FLOW_INPUT_MEM_NOPASSCA)
            {
                MDrv_TSP_SetOperateMode(0, E_DRVTSP_CTRL_MODE_MEM, 0xFF);
            }
            else if(DmxFlowInput == DMX_FLOW_INPUT_MEM_PASSCA1)
            {
                MDrv_TSP_SetOperateMode(0, E_DRVTSP_CTRL_MODE_MEM, 1);
            }

            MDrv_TSP_File_CMDQ_Reset();

            return TRUE;
        }
        else if(DMX_FLOW_MMFI0 == DmxFlow)
        {
            MDrv_TSP_File_TSIFSrcSel(E_DRVTSP_FLT_SOURCE_TYPE_TS1, FALSE); //Select MMFI CMDQ Src
            MDrv_MMFI_File_CmdQ_Reset(E_DRVMMFI_PATH0);
            return TRUE;
        }
        else if(DMX_FLOW_MMFI1 == DmxFlow)
        {
            MDrv_TSP_File_TSIFSrcSel(E_DRVTSP_FLT_SOURCE_TYPE_TS2, FALSE); //Select MMFI CMDQ Src
            MDrv_MMFI_File_CmdQ_Reset(E_DRVMMFI_PATH1);
            return TRUE;
        }
        else if(DMX_FLOW_PLAYBACK1 == DmxFlow)
        {
            bFileMode = TRUE;
            MDrv_MMFI_File_CmdQ_Reset(E_DRVMMFI_PATH0);
        }
        else if(DMX_FLOW_PLAYBACK2 == DmxFlow)
        {
            bFileMode = TRUE;
            MDrv_MMFI_File_CmdQ_Reset(E_DRVMMFI_PATH1);
        }
        else
        {
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] File in must work in playback flow\n", __LINE__));
            return FALSE;
        }
    }

    switch (DmxFlowInput)
    {
        case DMX_FLOW_INPUT_DEMOD:
            IfSet.ePad = E_DRVTSP_PAD_DEMOD;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT0:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT0;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT1:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT1;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT2:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT2;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT3:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT3;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT4:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT4;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT5:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT5;
            break;
        // 3Wire mode
        case DMX_FLOW_INPUT_EXT_INPUT0_3WIRE:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT0_3WIRE;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT1_3WIRE:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT1_3WIRE;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT2_3WIRE:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT2_3WIRE;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT3_3WIRE:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT3_3WIRE;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT4_3WIRE:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT4_3WIRE;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT5_3WIRE:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT5_3WIRE;
            break;
  #ifdef TSO_ENABLE
        case DMX_FLOW_INPUT_TSO:
            IfSet.ePad = E_DRVTSP_PAD_TSO;
            break;
  #endif
        default:
            break;
    }

    IfSet.bClkInv = bClkInv;
    IfSet.bExtSync = bExtSync;
    IfSet.bParallel = bParallel;

    DrvTSP_If TspIf;
    DrvTSP_CtrlMode CtrlMode;

    switch(DmxFlow)
    {
        case DMX_FLOW_PLAYBACK:
            TspIf = E_DRVTSP_IF_TS0;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS0;
            MDrv_TSP_GetDscmbEngIdx_BySource(E_DRVTSP_PKTSRC_DEMUX0, &u32DScmbEngId);
            break;
        case DMX_FLOW_PLAYBACK1:
            TspIf = E_DRVTSP_IF_TS1;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS1;
            MDrv_TSP_GetDscmbEngIdx_BySource(E_DRVTSP_PKTSRC_DEMUX1, &u32DScmbEngId);
            MDrv_TSP_File_TSIFSrcSel(E_DRVTSP_FLT_SOURCE_TYPE_TS1, bFileMode); //Select MMFI CMDQ Src
            break;
        case DMX_FLOW_PLAYBACK2:
            TspIf = E_DRVTSP_IF_TS2;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS2;
            MDrv_TSP_GetDscmbEngIdx_BySource(E_DRVTSP_PKTSRC_DEMUX2, &u32DScmbEngId);
            MDrv_TSP_File_TSIFSrcSel(E_DRVTSP_FLT_SOURCE_TYPE_TS2, bFileMode); //Select MMFI CMDQ Src
            break;
        #ifdef MERGE_STR_SUPPORT
        case DMX_FLOW_PLAYBACK_FI:
            TspIf = E_DRVTSP_IF_FI;
            CtrlMode = E_DRVTSP_CTRL_MODE_TSFI_LIVE;
            MDrv_TSP_GetDscmbEngIdx_BySource(E_DRVTSP_PKTSRC_DEMUXFILE, &u32DScmbEngId);
            MDrv_TSP_File_TSIFSrcSel(E_DRVTSP_FLT_SOURCE_TYPE_FILE, bFileMode);
            break;
        #endif
        case DMX_FLOW_PVR:
            TspIf = E_DRVTSP_IF_TS0;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS0;
            MDrv_TSP_GetDscmbEngIdx_BySource(E_DRVTSP_PKTSRC_DEMUX0, &u32DScmbEngId);
            MDrv_TSP_PVR_Eng_SelSrc(0, E_DRVTSP_PKTSRC_DEMUX0);
            MDrv_TSP_PVR_BlockEnable(0, bPvrBlockEnable);
            _pdmx_res->_u32PVREngSrc[0] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX0;
            break;
        case DMX_FLOW_PVR1:
            TspIf = E_DRVTSP_IF_TS1;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS1;
            MDrv_TSP_GetDscmbEngIdx_BySource(E_DRVTSP_PKTSRC_DEMUX1, &u32DScmbEngId);
            MDrv_TSP_PVR_Eng_SelSrc(1, E_DRVTSP_PKTSRC_DEMUX1);
            MDrv_TSP_PVR_BlockEnable(1, bPvrBlockEnable);
            _pdmx_res->_u32PVREngSrc[1] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX1;
            break;
        case DMX_FLOW_PVR2:   //PVR_CB
            TspIf = E_DRVTSP_IF_TS3;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS3;
            break;
        case DMX_FLOW_PVRCA:
        case DMX_FLOW_CIPHSS_PVRCA:
            TspIf = E_DRVTSP_IF_TS0;
            CtrlMode = E_DRVTSP_CTRL_MODE_PVR_CA;
            MDrv_TSP_GetDscmbEngIdx_BySource(E_DRVTSP_PKTSRC_DEMUX0, &u32DScmbEngId);
            MDrv_TSP_PVR_Eng_SelSrc(0, E_DRVTSP_PKTSRC_DEMUX0);
            MDrv_TSP_PVR_BlockEnable(0, bPvrBlockEnable);
            _pdmx_res->_u32PVREngSrc[0] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX0;
            break;
        case DMX_FLOW_PVRCA1:
            TspIf = E_DRVTSP_IF_TS1;
            CtrlMode = E_DRVTSP_CTRL_MODE_PVR1_CA;
            MDrv_TSP_GetDscmbEngIdx_BySource(E_DRVTSP_PKTSRC_DEMUX1, &u32DScmbEngId);
            MDrv_TSP_PVR_Eng_SelSrc(1, E_DRVTSP_PKTSRC_DEMUX1);
            MDrv_TSP_PVR_BlockEnable(1, bPvrBlockEnable);
            _pdmx_res->_u32PVREngSrc[1] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX1;
            break;
        case DMX_FLOW_CIPHSS_PLAYBACK:
            TspIf = E_DRVTSP_IF_TS1;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS1;
            break;

        default:
            return FALSE;
    }

    if(MDrv_TSP_SetOperateMode(0, CtrlMode, (MS_U8)u32DScmbEngId) != DRVTSP_OK)
        return FALSE;

    if (!((DMX_FLOW_INPUT_MEM == DmxFlowInput) || (DMX_FLOW_INPUT_MEM_NOPASSCA == DmxFlowInput)
        || (DMX_FLOW_INPUT_MEM_PASSCA1 == DmxFlowInput)))
    {
        if(MDrv_TSP_SelPad(0, TspIf, &IfSet) != DRVTSP_OK)
            return FALSE;
    }

    return TRUE;

}  // End of TSP_VER_3_0

#elif (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))

//-------------------------------------------------------------------------
// For TSP version 1.0:
//      DMX_FLOW_PLAYBACK              : TSIF0 / PIDFLT0 / Live and File
//      DMX_FLOW_PVR                      : TSIF1 / PIDFLT1 / Rec0
//      DMX_FLOW_PVR1                     : TSIFCB / PIDFLTCB /RecCB
//      DMX_FLOW_PVRCA                   : TSIF1 / PIDFLT1 /Rec0 with DSCMB
//      DMX_FLOW_PVRCA1                  : TSIF1 / PIDFLT1 /Rec1 with DSCMB (Source TSIF2 from TSIF1)
//      DMX_FLOW_FILEIN_MM             : MMFI Audio / MMFIA PIDFLT / File audio
//      DMX_FLOW_FILEIN_MM3D         : MMFI V3D / MMFIV3D PIDFLT / File V3D
//      DMX_FLOW_CIPHSS_PLAYBACK  : TS1 / PIDFLTF for DSCMB / Live for SCMB packet
//      DMX_FLOW_CIPHSS_PVRCA       : TS1 / PIDFLT1 for DSCMB / Rec0 with DSCMB
//--------------------------------------------------------------------------
static MS_BOOL _DMX_FlowSet(DMX_FLOW DmxFlow, DMX_FLOW_INPUT DmxFlowInput, MS_BOOL bClkInv, MS_BOOL bExtSync, MS_BOOL bParallel)
{
    DrvTSP_If_Set IfSet;

    //version 1.0 has only 1 ca0 engine, and no ca1 engine
    if(DmxFlowInput == DMX_FLOW_INPUT_MEM_PASSCA1)
        return FALSE;

  #ifdef TSO_ENABLE
    if(DMX_FLOW_TSO_PLAYBACK== DmxFlow)
    {
        return _DMX_FlowSetTSO(DmxFlow, DmxFlowInput, bClkInv, bExtSync, bParallel);
    }
  #endif

    if ((DMX_FLOW_INPUT_MEM == DmxFlowInput) || (DMX_FLOW_INPUT_MEM_NOPASSCA == DmxFlowInput))
    {
      #if 0
        if(!((DMX_FLOW_PLAYBACK == DmxFlow) || (DMX_FLOW_FILEIN_MM == DmxFlow) || (DMX_FLOW_FILEIN_MM3D == DmxFlow)))
        {
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] File in must work in playback flow\n", __LINE__));
            return FALSE;
        }
      #endif

        if(DmxFlow == DMX_FLOW_PVRCA)
        {
            MDrv_TSP_SetOperateMode(0, E_DRVTSP_CTRL_MODE_MEM_PVR);
        }
        if(DmxFlowInput == DMX_FLOW_INPUT_MEM)
        {
            MDrv_TSP_SetOperateMode(0, E_DRVTSP_CTRL_MODE_MEM);
        }
        else
        {
            MDrv_TSP_SetOperateMode(0, E_DRVTSP_CTRL_MODE_MEM_NOCA);
        }

        return TRUE;
    }

    memset(&IfSet, 0x0, sizeof(DrvTSP_If_Set));
    switch (DmxFlowInput)
    {
        case DMX_FLOW_INPUT_DEMOD:
            IfSet.ePad = E_DRVTSP_PAD_DEMOD;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT0:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT0;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT1:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT1;
            break;
        //-- Note: Not support for all chip -------------------
        #ifdef TS2_IF_SUPPORT
        case DMX_FLOW_INPUT_EXT_INPUT2:
            IfSet.ePad = E_DRVTSP_PAD_EXT_INPUT2;
            break;
        #endif
        //-----------------------------------------------

  #ifdef TSO_ENABLE
        case DMX_FLOW_INPUT_TSO:
            IfSet.ePad = E_DRVTSP_PAD_TSO;
            break;
  #endif

        default:
            MS_ASSERT(0);
            break;
    }

    IfSet.bClkInv = bClkInv;
    IfSet.bExtSync = bExtSync;
    IfSet.bParallel = bParallel;

    DrvTSP_If TspIf;
    DrvTSP_CtrlMode CtrlMode;

  #ifdef TS2_IF_SUPPORT
    MS_U32 u32data = 0;
  #endif

    switch(DmxFlow)
    {
        case DMX_FLOW_PLAYBACK:
            TspIf = E_DRVTSP_IF_PLAYBACK;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS0;
            break;
        case DMX_FLOW_PLAYBACK_NOCA:
            TspIf = E_DRVTSP_IF_PLAYBACK;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS0_NOCA;
            break;
        case DMX_FLOW_PLAYBACK_SRC_TS1: //TS0 playback, and the source is from TS1
            TspIf = E_DRVTSP_IF_PVR0;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS1_OUT_LIVE0;
            if(MDrv_TSP_SelPad(0, E_DRVTSP_IF_PLAYBACK, &IfSet) != DRVTSP_OK) //enable tsif0
            {
                return FALSE;
            }
            break;
        case DMX_FLOW_PVR:
            TspIf = E_DRVTSP_IF_PVR0;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS1;
            break;
        case DMX_FLOW_PVRCA:
            TspIf = E_DRVTSP_IF_PVR0;
            CtrlMode = E_DRVTSP_CTRL_MODE_PVR_CA;
            _pdmx_res->_u32PVREngSrc[0] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX0;
            if(MDrv_TSP_SelPad(0, TspIf, &IfSet) != DRVTSP_OK)
            {
                return FALSE;
            }
            break;

  #ifdef TS2_IF_SUPPORT
      #ifdef TS2_LIVE_SUPPORT
        case DMX_FLOW_PLAYBACK1:
      #endif
        case DMX_FLOW_PVR1:
            TspIf = E_DRVTSP_IF_PVR1;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS2;
            break;
        case DMX_FLOW_PVRCA1:
            //For Napoli U04, CA input from TSIIF1, and output to TSIF2
            //Must set TSIF1 mux as TSIF2, and Duplicate TS2 Filter to TS1 filter
            MDrv_TSP_CMD_Run(0x00000002, 0, 0, &u32data);
            if(u32data == 0)
            {
                return FALSE;
            }
            CtrlMode = E_DRVTSP_CTRL_MODE_PVR_TS2_CA;
            _pdmx_res->_u32PVREngSrc[0] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX1;
            if(MDrv_TSP_SelPad(0, E_DRVTSP_IF_PVR0, &IfSet) != DRVTSP_OK)
            {
                return FALSE;
            }
            TspIf = E_DRVTSP_IF_PVR1;
            break;
  #endif

        case DMX_FLOW_CIPHSS_PLAYBACK:
            TspIf = E_DRVTSP_IF_PVR0;
            CtrlMode = E_DRVTSP_CTRL_MODE_TS1_FILEFLT;
            break;
        case DMX_FLOW_CIPHSS_PVRCA:
            TspIf = E_DRVTSP_IF_PVR0;
            CtrlMode = E_DRVTSP_CTRL_MODE_PVR_TS0_CA;
            _pdmx_res->_u32PVREngSrc[0] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX0;
            break;

        default:
            return FALSE;
    }

    if(MDrv_TSP_SetOperateMode(0, CtrlMode) != DRVTSP_OK)
    {
        return FALSE;
    }
    if((DmxFlow != DMX_FLOW_PVRCA) && (DmxFlow != DMX_FLOW_CIPHSS_PVRCA))
    {
        if(MDrv_TSP_SelPad(0, TspIf, &IfSet) != DRVTSP_OK)
        {
            return FALSE;
        }
    }
    return TRUE;

}  // End of TSP_VER_1_0

#else  // For TSP_VER_4_0
//-------------------------------------------------------------------------
// For TSP version 4.0:
//--------------------------------------------------------------------------
static MS_BOOL _DMX_FlowSet(DMX_FLOW DmxFlow, DMX_FLOW_INPUT DmxFlowInput, MS_BOOL bClkInv, MS_BOOL bExtSync, MS_BOOL bParallel)
{
  #if 0 // we don't have this constrain in K3 every path is able to filein
    if ((DMX_FLOW_INPUT_MEM == DmxFlowInput) || (DMX_FLOW_INPUT_MEM_NOPASSCA == DmxFlowInput))
    {
        if(!((DMX_FLOW_PLAYBACK == DmxFlow) || (DMX_FLOW_FILEIN_MM == DmxFlow) || (DMX_FLOW_FILEIN_MM3D == DmxFlow)))
        {
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] File in must work in playback flow\n", __LINE__));
            return FALSE;
        }
        MDrv_TSP_SetTSIF(0, E_TSP_TSIF_EN | E_TSP_TSIF_EXTSYNC | E_TSP_TSIF_PARL, TRUE);
        return TRUE;
    }
  #endif

    TSP_TSIFCfg         cfg     = E_TSP_TSIF_EN;
    TSP_TSPad           pad     = E_TSP_TS_INVALID;
    TSP_TSId            etid     = E_TSP_TSID_INVALID;
    DRV_TSP_FLOW        eDst    = E_DRV_TSP_FLOW_INVALID;
    MS_BOOL             filein  = FALSE;
    MS_BOOL             bClkDis = FALSE;
    TSP_TSIF            eTSIF   = E_TSP_TSIF_INVALID;

  #if 1 // for TV SW, we add PVRCA support for TV needs.
    // all stb path can do a CA PVR record
    if (DMX_FLOW_PVRCA == DmxFlow)
    {
        //since PVRCA not support yet, all will go DMX_FLOW_PVR
        DmxFlow = DMX_FLOW_PVR;
    }
    if (DMX_FLOW_PVRCA1 == DmxFlow)
    {
        //since PVRCA1 not support yet, all will go DMX_FLOW_PVR1
        DmxFlow = DMX_FLOW_PVR1;
    }
  #endif

    //### organize setting into cfg
    if (bExtSync)
    {
        cfg |= E_TSP_TSIF_EXTSYNC;
    }
    if (bParallel)
    {
        cfg |= E_TSP_TSIF_PARL;
    }
    if((DmxFlowInput >= DMX_FLOW_INPUT_EXT_INPUT0_3WIRE) && (DmxFlowInput <= DMX_FLOW_INPUT_EXT_INPUT6_3WIRE))
    {
        cfg |= E_TSP_TSIF_3WIRE;
    }

  #ifdef TSO_20_ENABLE

    if(DmxFlow == DMX_FLOW_TSO_PLAYBACK)
    {
        DMX_TSO_InputCfg stInputCfg;
        DMX_TSO_OutputCfg stOutputCfg;

        memset(&stInputCfg, 0, sizeof(DMX_TSO_InputCfg));
        memset(&stOutputCfg, 0, sizeof(DMX_TSO_OutputCfg));
        stOutputCfg.bDefOutClk = FALSE;
        stOutputCfg.bOutClkInv = TRUE;
        switch (DmxFlowInput)
        {
            case DMX_FLOW_INPUT_DEMOD:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_Dmd;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT0:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
                stOutputCfg.eTsoOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS0IN;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT1:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
                stOutputCfg.eTsoOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS1IN;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT2:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
                stOutputCfg.eTsoOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS2IN;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT3:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
                stOutputCfg.eTsoOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS3IN;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT4:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
                stOutputCfg.eTsoOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS4IN;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT5:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_PTSOOUT;
                stOutputCfg.eTsoOutClkSrc = E_DRVTSO_OUTCLKSRC_P_TS5IN;
                break;
            case DMX_FLOW_INPUT_EXT_INPUT0_3WIRE:
            case DMX_FLOW_INPUT_EXT_INPUT1_3WIRE:
            case DMX_FLOW_INPUT_EXT_INPUT2_3WIRE:
            case DMX_FLOW_INPUT_EXT_INPUT3_3WIRE:
            case DMX_FLOW_INPUT_EXT_INPUT4_3WIRE:
            case DMX_FLOW_INPUT_EXT_INPUT5_3WIRE:
            case DMX_FLOW_INPUT_EXT_INPUT6_3WIRE:
                stOutputCfg.eTsoOutClk = E_DRVTSO_OUTCLK_27M;
                break;
            case DMX_FLOW_INPUT_MEM:
            case DMX_FLOW_INPUT_MEM1:
            default:
               stOutputCfg.bDefOutClk = TRUE;
               break;
        }

        stOutputCfg.eFlow = DmxFlow;
        stOutputCfg.eOutPad = DMX_FLOW_OUTPUT_EXT_PAD1;
        stOutputCfg.u16OutPktSize = 0xBC;
        stOutputCfg.bEnable = TRUE;
        stOutputCfg.bSet = TRUE;
        if(_TSO_Flow_OutputCfg(&stOutputCfg) == FALSE)
            return FALSE;

        stInputCfg.eFlow = DmxFlow;
        stInputCfg.stInputInfo.Input = DmxFlowInput;
        stInputCfg.stInputInfo.bClkInv = bClkInv;
        stInputCfg.stInputInfo.bExtSync = bExtSync;
        stInputCfg.stInputInfo.bParallel = bParallel;
        stInputCfg.u8LocalStrId = 0x47;
        stInputCfg.bBypassAll = TRUE;
        stInputCfg.bEnable = TRUE;
        stInputCfg.bSet = TRUE;
        if(DMX_FLOW_TSO_PLAYBACK== DmxFlow)
        {
            if(DmxFlowInput == DMX_FLOW_INPUT_MEM)
            {
                stInputCfg.eTSOInIf = DMX_TSIF_FILE0;
            }
            else if(DmxFlowInput == DMX_FLOW_INPUT_MEM1)
            {
                stInputCfg.eTSOInIf = DMX_TSIF_FILE1;
            }
            else
            {
                stInputCfg.eTSOInIf = DMX_TSIF_LIVE0;
            }
        }

        return _TSO_Flow_InputCfg(&stInputCfg);
    }

  #elif defined(TSO_ENABLE)

    if ((DmxFlow == DMX_FLOW_TSO_PLAYBACK) || (DmxFlow == DMX_FLOW_TSO_PLAYBACK1))
        return _DMX_FlowSetTSO(DmxFlow, DmxFlowInput, bClkInv, bExtSync, bParallel);

  #endif

  #ifdef TSIO_ENABLE
    if (DmxFlow == DMX_FLOW_TSIO_PLAYBACK)
    {
        DMX_TSO_InputCfg stInputCfg;
        MS_U16 u16Size = 200;

        memset(&stInputCfg, 0, sizeof(DMX_TSO_InputCfg));
        stInputCfg.eFlow = DMX_FLOW_TSO_PLAYBACK;
        stInputCfg.stInputInfo.Input = DmxFlowInput;
        stInputCfg.stInputInfo.bClkInv = bClkInv;
        stInputCfg.stInputInfo.bExtSync = bExtSync;
        stInputCfg.stInputInfo.bParallel = bParallel;
        stInputCfg.u8LocalStrId = 0x47;
        stInputCfg.bBypassAll = FALSE;
        stInputCfg.bEnable = TRUE;
        stInputCfg.bSet = TRUE;
        if(DmxFlowInput == DMX_FLOW_INPUT_MEM)
        {
            stInputCfg.eTSOInIf = DMX_TSIF_FILE0;
        }
        else if(DmxFlowInput == DMX_FLOW_INPUT_MEM1)
        {
            stInputCfg.eTSOInIf = DMX_TSIF_FILE1;
        }
        else
        {
            stInputCfg.eTSOInIf = DMX_TSIF_LIVE0;
        }

        MDrv_TSO_TsioMode_En(0, TRUE); //TSIO enable
        MDrv_TSO_OutputPktSize(0, &u16Size, true); //TSO output size
        return _TSO_Flow_InputCfg(&stInputCfg);
    }
  #endif  // End of TSIO_ENABLE

    switch (DmxFlowInput)
    {
        case DMX_FLOW_INPUT_DEMOD:
            pad = E_TSP_TS_DEMOD0;
            break;
        case DMX_FLOW_INPUT_DEMOD1:
            pad = E_TSP_TS_DEMOD1;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT0:
            pad = E_TSP_TS_PAD0;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT0_3WIRE:
            pad = E_TSP_TS_PAD0_3WIRE;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT1:
            pad = E_TSP_TS_PAD1;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT1_3WIRE:
            pad = E_TSP_TS_PAD1_3WIRE;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT2:
            pad = E_TSP_TS_PAD2;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT2_3WIRE:
            pad = E_TSP_TS_PAD2_3WIRE;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT3:
            pad = E_TSP_TS_PAD3;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT3_3WIRE:
            pad = E_TSP_TS_PAD3_3WIRE;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT4:
            pad = E_TSP_TS_PAD4;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT4_3WIRE:
            pad = E_TSP_TS_PAD4_3WIRE;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT5:
            pad = E_TSP_TS_PAD5;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT5_3WIRE:
            pad = E_TSP_TS_PAD5_3WIRE;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT6:
            pad = E_TSP_TS_PAD6;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT6_3WIRE:
            pad = E_TSP_TS_PAD6_3WIRE;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT7:
            pad = E_TSP_TS_PAD7;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT7_3WIRE:
            pad = E_TSP_TS_PAD7_3WIRE;
            break;
        case DMX_FLOW_INPUT_TSO:
            pad = E_TSP_TS_PAD_TSO0;
            break;
        case DMX_FLOW_INPUT_TSO1:
            pad = E_TSP_TS_PAD_TSO1;
            break;
        case DMX_FLOW_INPUT_MEM:
            pad = E_TSP_TS_PAD0;
            filein = TRUE;
            break;
        case DMX_FLOW_INPUT_DISABLE:
            pad = E_TSP_TS_PAD0;
            bClkDis = TRUE;
            break;
  #ifdef TSIO_ENABLE
        case DMX_FLOW_INPUT_TSIO:
            pad = E_TSP_TS_PAD_TSIO0;
            break;
  #endif

        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Unsupported DMX_FLOW_INPUT type error!\n", __LINE__));
            return FALSE;
    }

    switch (DmxFlow)
    {
        //support file-in and live-in simultaneously
        case DMX_FLOW_PLAYBACK:
            eDst = E_DRV_TSP_FLOW_PLAYBACK0;
            eTSIF = E_TSP_TSIF_0;//default mapping of DMX_FLOW_PLAYBACK to TSIF0
            etid = E_TSP_TSID_TSIF0;
            break;
        //support file-in and live-in simultaneously
        case DMX_FLOW_PLAYBACK1:
            eDst = E_DRV_TSP_FLOW_PLAYBACK1;
            eTSIF = E_TSP_TSIF_1;
            etid = E_TSP_TSID_TSIF1;
            break;
        //support file-in or live-in
        case DMX_FLOW_PLAYBACK2:
            eDst = E_DRV_TSP_FLOW_PLAYBACK2;
            eTSIF = E_TSP_TSIF_2;
            etid= E_TSP_TSID_TSIF2;
            break;
        //support file-in or live-in
        case DMX_FLOW_PLAYBACK3:
            eDst = E_DRV_TSP_FLOW_PLAYBACK3;
            eTSIF = E_TSP_TSIF_3;
            etid = E_TSP_TSID_TSIF3;
            break;
        //support file-in or live-in
        case DMX_FLOW_PLAYBACK4:
            eDst = E_DRV_TSP_FLOW_PLAYBACK4;
            eTSIF = E_TSP_TSIF_4;
            etid = E_TSP_TSID_INVALID;  // no use @ MDrv_TSP_SetFlowSource
            break;
        //support file-in or live-in
        case DMX_FLOW_PLAYBACK5:
            eDst = E_DRV_TSP_FLOW_PLAYBACK5;
            eTSIF = E_TSP_TSIF_5;
            etid = E_TSP_TSID_INVALID;  // no use @ MDrv_TSP_SetFlowSource
            break;
        //support file-in or live-in
        case DMX_FLOW_PLAYBACK6:
            eDst = E_DRV_TSP_FLOW_PLAYBACK6;
            eTSIF = E_TSP_TSIF_6;
            etid = E_TSP_TSID_INVALID;  // no use @ MDrv_TSP_SetFlowSource
            break;
        case DMX_FLOW_PVR:
            eDst = E_DRV_TSP_FLOW_PVR0;
            eTSIF = E_TSP_TSIF_PVR0;
            etid = E_TSP_TSID_TSIF0;
            break;
        case DMX_FLOW_PVR1:
            eDst = E_DRV_TSP_FLOW_PVR1;
            eTSIF = E_TSP_TSIF_PVR1;
            etid = E_TSP_TSID_TSIF1;
            break;
        case DMX_FLOW_PVR2:
            eDst = E_DRV_TSP_FLOW_PVR2;
            eTSIF = E_TSP_TSIF_PVR2;
            etid = E_TSP_TSID_TSIF2;
            break;
        case DMX_FLOW_PVR3:
            eDst = E_DRV_TSP_FLOW_PVR3;
            eTSIF = E_TSP_TSIF_PVR3;
            etid = E_TSP_TSID_TSIF3;
            break;

        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Unsupported DMX_FLOW_INPUT type error!\n", __LINE__));
            return FALSE;
    }

  #if 0
    if (E_TSP_TSID_INVALID != tid)//kaiser like series  // link dst to tsifX
    {
//            if ((tid != E_TSP_TSID_MMFI0) || (tid != E_TSP_TSID_MMFI1)) //
//            {
            if (u16TSIF != E_TSP_TSIF_INVALID)
            {
                MDrv_TSP_SelPad_ClkInv(u16TSIF, bClkInv);
                MDrv_TSP_SetTSIF(u16TSIF, cfg, filein); // we set TSIF at this function (external sync and  parallel)
            }
//            }
    }
    else if ((E_TSP_TS_INVALID != pad)) // old style we set specific path to dst
  #endif
    //File-in case
    //No need setting pad in file-in mod

    if (DRVTSP_OK != MDrv_TSP_SetFlowSource(eDst, etid)) //setting dst source from tid
    {
        DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Set flow source error!\n", __LINE__));
    }


    if(filein == TRUE)
    {
        MDrv_TSP_SetTSIF(eTSIF, E_TSP_TSIF_EN | E_TSP_TSIF_EXTSYNC | E_TSP_TSIF_PARL ,filein);
    }
    //Live-in and PVR case
    else
    {
        if ((E_TSP_TS_INVALID != pad)) // old style we set specific path to dst
        {
            MDrv_TSP_SelPad(eTSIF, pad);  // only old style has pad setting
        }

        MDrv_TSP_SelPad_ClkDis(eTSIF, bClkDis); //@FIXME check return value for each drv function
        MDrv_TSP_SelPad_ClkInv(eTSIF, bClkInv);
        MDrv_TSP_SetTSIF(eTSIF, cfg, filein); // we set TSIF at this function (external sync and  parallel)
   }

  #if 0 // @FIXME what is this? for TV SW?
    if (DMX_FLOW_PVRCA == DmxFlow)
    {
        //since PVRCA not support yet, all will go DMX_FLOW_PVR
        DmxFlow = DMX_FLOW_PVR;
    }
  #endif

    return TRUE;
}
#endif  // End of TSP_VER_4_0

static inline MS_U32 _DMX_RingBuffer_DataSize(MS_VIRT virtRead, MS_VIRT virtWrite, MS_VIRT virtBufStart, MS_VIRT virtBufEnd)
{
    return (MS_U32)((virtWrite >= virtRead) ? (virtWrite- virtRead): ((virtBufEnd- virtBufStart)- (virtRead- virtWrite)));
}

static MS_BOOL _DMX_RingBuffer_ChkPESLen(MS_U8* pu8Read, MS_U8* pu8Start, MS_U8* pu8End, MS_U32 *pSecLen)
{
    *pSecLen = 0;

    //check PES start code first
    if ((pu8Read + 3) <= pu8End)
    {
        if((*pu8Read != 0) || (*(pu8Read+1) != 0) || (*(pu8Read+2) != 0x01))
            return FALSE;
    }
    else if ((pu8Read + 2) <= pu8End)
    {
        if((*pu8Read != 0) || (*(pu8Read+1) != 0) || (*pu8Start != 0x01))
            return FALSE;
    }
    else if ((pu8Read + 1) <= pu8End)
    {
        if((*pu8Read != 0) || (*pu8Start != 0) || (*(pu8Start+1) != 0x01))
            return FALSE;
    }
    else
    {
        if((*pu8Start != 0) || (*(pu8Start+1) != 0) || (*(pu8Start+2) != 0x01))
            return FALSE;
    }

    //calculate PES length
    if ((pu8Read + 6) <= pu8End)
    {
        *pSecLen = ((*(pu8Read+ 4) & 0xFF) << 8) + *(pu8Read+ 5);
    }
    else if ((pu8Read + 5) <= pu8End)
    {
        *pSecLen = ((*(pu8Read+ 4) & 0xFF) << 8) + *(pu8Start);
    }
    else if ((pu8Read + 4) <= pu8End)
    {
        *pSecLen = ((*(pu8Start) & 0xFF) << 8) + *(pu8Start+ 1);
    }
    else if((pu8Read + 3) <= pu8End)
    {
        *pSecLen = ((*(pu8Start + 1) & 0xFF) << 8) + *(pu8Start+ 2);
    }
    else if((pu8Read + 2) <= pu8End)
    {
        *pSecLen = ((*(pu8Start + 2) & 0xFF) << 8) + *(pu8Start+ 3);
    }
    else if((pu8Read + 1) <= pu8End)
    {
        *pSecLen = ((*(pu8Start + 3) & 0xFF) << 8) + *(pu8Start+ 4);
    }
    else
    {
        *pSecLen = ((*(pu8Start+ 4) & 0xFF) << 8) + *(pu8Start+ 5);
    }

    if (*pSecLen == 0x00)
    {
        *pSecLen = 0xffff;
        //ULOGW("DMX", "[DMX][%d] Warning: PES length is 0xFFFF\n", __LINE__);
    }
    else
    {
        *pSecLen += 6;
    }

    return TRUE;
}

static DMX_FILTER_STATUS _DMX_GetRingBuffer(MS_U32 u32DmxId, MS_U8* pu8Buf, MS_U32 u32BufSize, MS_U32* pu32SecSize, MS_U32* pu32RmnSize, DMX_CheckCb pfCheckCB)
{
    MS_U32      u32RbSize = 0;
    MS_U32      u32SecBufSize= 0, u32SecLen= 0;
    MS_U8*      pu8Start;
    MS_U8*      pu8End = 0;
    MS_U8*      pu8Read;
    MS_U8*      pu8Write;
    MS_U32      u32CopySize= 0;
    MS_BOOL     bCopy;
    MS_PHY      temp = 0;
    MS_VIRT     virtStart = 0;
    MS_VIRT     virtEnd = 0;
    MS_VIRT     virtRead = 0;
    MS_VIRT     virtWrite = 0;

    //DMX_ASSERT(DMX_MAX_FLTID> u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));

#ifndef MSOS_TYPE_LINUX_KERNEL
    MsOS_Sync();
#endif
    MsOS_ReadMemory();

    // Get Section buffer read pointer
    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetReadAddr(0, _pdmx_res->_FltList[u32DmxId].SecBufId, &temp))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    virtRead = (MS_VIRT)temp;
    // Get Section buffer write pointer
    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetWriteAddr(0, _pdmx_res->_FltList[u32DmxId].SecBufId, &temp))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    virtWrite = (MS_VIRT)temp;

    *pu32SecSize=       0;
    if (virtRead== virtWrite)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    // Get Section buffer start address
    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetBufStart(0, _pdmx_res->_FltList[u32DmxId].SecBufId, &temp))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    virtStart = (MS_VIRT)temp;
    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetBufSize(0, _pdmx_res->_FltList[u32DmxId].SecBufId, &u32SecBufSize))
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    // Get Section buffer end address
    virtEnd =           virtStart + u32SecBufSize;
    if(virtRead >= virtEnd || virtRead < virtStart)
    {
        ULOGE("DMX", "[DMX][%d][%d] FATAL ERROR: PES buffer read address out of range!!!!!!!!\n", __LINE__, (unsigned int)MsOS_GetSystemTime());
        ULOGE("DMX", "[DMX][%d] Start %x, End %x , Read %x, Write %x \n", __LINE__, (unsigned int)virtStart, (unsigned int)virtEnd, (unsigned int)virtRead, (unsigned int)virtWrite);
        return DMX_FILTER_STATUS_ERROR;
    }

    if(virtWrite >= virtEnd || virtWrite < virtStart)
    {
        ULOGE("DMX", "[DMX][%d][%d] FATAL ERROR: PES buffer write address out of range!!!!!!!!\n", __LINE__, (unsigned int)MsOS_GetSystemTime());
        ULOGE("DMX", "[DMX][%d] Start %x, End %x , Read %x, Write %x \n", __LINE__, (unsigned int)virtStart, (unsigned int)virtEnd, (unsigned int)virtRead, (unsigned int)virtWrite);
        return DMX_FILTER_STATUS_ERROR;
    }
    if ((virtRead==0)||(virtWrite==0)||(virtStart==0))
    {
        ULOGE("DMX", "[DMX][%d][%d] FATAL ERROR: PES buffer address illegal!!!!!!!!\n", __LINE__, (unsigned int)MsOS_GetSystemTime());
        ULOGE("DMX", "[DMX][%d] Start %x, End %x , Read %x, Write %x \n", __LINE__, (unsigned int)virtStart, (unsigned int)virtEnd, (unsigned int)virtRead, (unsigned int)virtWrite);
        return DMX_FILTER_STATUS_ERROR;
    }

    *pu32RmnSize = u32RbSize= _DMX_RingBuffer_DataSize(virtRead, virtWrite, virtStart, virtEnd);
    if (u32RbSize> u32BufSize)
    {
        u32RbSize = u32BufSize;
    }

    //MS_ASSERT(pu8Start == (MS_U8*)_pdmx_res->_FltList[(u8DmxId)].FltInfo.Info.SectInfo.SectBufAddr);

    // transfer physical address to uncache address
    pu8Read=            (MS_U8*)MS_PA2KSEG1((MS_PHY)virtRead);
    pu8Write=           (MS_U8*)MS_PA2KSEG1((MS_PHY)virtWrite);
    pu8Start=           (MS_U8*)MS_PA2KSEG1((MS_PHY)virtStart);
    //pu8End=             (MS_U8*)MS_PA2KSEG1((MS_U32)pu8End);
    pu8End=             pu8Start + u32SecBufSize;

    /*ULOGD("DMX", "(Start, End, Read, Write) = (0x%08x, 0x%08x, 0x%08x, 0x%08x)\n",
            (int)pu8Start,
            (int)pu8End,
            (int)pu8Read,
            (int)pu8Write);*/

#ifndef MSOS_TYPE_LINUX_KERNEL
    bCopy = (pfCheckCB)? pfCheckCB((MS_U8)u32DmxId, pu8Start, pu8End, pu8Read, pu8Write): TRUE;
#else
    bCopy = TRUE; //can not callback user function in kernel mode
#endif

    //---------------------------------------------
    //    For Customer's request, only copy 1 PES at once
    //---------------------------------------------
    if((_pdmx_res->_FltList[u32DmxId].DmxFltType == DMX_FILTER_TYPE_PES) && (_u32PESCpy_ContMode == 0))
    {
        if(_DMX_RingBuffer_ChkPESLen(pu8Read, pu8Start, pu8End, &u32SecLen) == TRUE)
        {
            if(u32RbSize >= u32SecLen)
            {
                u32RbSize = u32SecLen;
            }
        }
    }
    //--------------------------------------------- end

    if ((pu8Read+ u32RbSize)>= pu8End)
    {
        u32CopySize=        pu8End- pu8Read;
        if (bCopy)
        {
            CMP_CPY_TO_USER(pu8Buf, (void*)pu8Read, u32CopySize);
        }
        pu8Read=            pu8Start;
        u32RbSize-=         u32CopySize;
        *pu32SecSize+=      u32CopySize;
        pu8Buf+=            u32CopySize;
    }
    if (u32RbSize)
    {
        if (bCopy)
        {
            CMP_CPY_TO_USER(pu8Buf, (void*)pu8Read, u32RbSize);
        }
        *pu32SecSize+=      u32RbSize;
        pu8Read+=           u32RbSize;
    }
    *pu32RmnSize-= *pu32SecSize;
    MS_ASSERT(pu8Read< pu8End);
    MDrv_TSP_SecFlt_SetReadAddr(0, _pdmx_res->_FltList[u32DmxId].SecBufId, MS_VA2PA((MS_VIRT)pu8Read));
    return DMX_FILTER_STATUS_OK;
}

static void _DumpSecHeader(MS_U8* pu8Start, MS_U8* pu8End, MS_U8* pu8Read, MS_U8* pu8Write)
{
#if defined (MS_DEBUG)
    MS_U32 u32SecLen = 0;
    MS_DEBUG_MSG(DMX_DBGMSG(DMX_DBG_ERR, ULOGD("DMX", "(Start, End, Read, Write) = (0x%08x, 0x%08x, 0x%08x, 0x%08x)\n",
            (MS_U32)pu8Start,
            (MS_U32)pu8End,
            (MS_U32)pu8Read,
            (MS_U32)pu8Write)));
    if ((pu8Read + 3) <= pu8End)
    {
        u32SecLen =     ((*(pu8Read+ 1) & 0x0F) << 8) + *(pu8Read+ 2);
        MS_DEBUG_MSG(DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[%d] %d: %02x %02x %02x\n", __LINE__, u32SecLen, (MS_U32)pu8Read[0], (MS_U32)pu8Read[1], (MS_U32)pu8Read[2])));
    }
    else if ((pu8Read + 2) <= pu8End)
    {
        u32SecLen =     ((*(pu8Read+ 1) & 0x0F) << 8) + *(pu8Start);
        MS_DEBUG_MSG(DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[%d] %d: %02x %02x %02x\n", __LINE__, u32SecLen, (MS_U32)pu8Read[0], (MS_U32)pu8Read[1], (MS_U32)pu8Start[0])));
    }
    else // ((uReadAddr+1) == uEndAddr)
    {
        u32SecLen =     ((*(pu8Start) & 0x0F) << 8) + *(pu8Start+ 1);
        MS_DEBUG_MSG(DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[%d] %d: %02x %02x %02x\n", __LINE__, (MS_U32)u32SecLen, (MS_U32)pu8Read[0], (MS_U32)pu8Start[0], (MS_U32)pu8Start[1])));
    }

  #if 0 //left for debug
    for (u32i = 0 ; u32i < u32SecLen+3 ; u32i++)
    {
        if (u32i%16 == 0)
            ULOGD("DMX", "\n");
        ULOGD("DMX", "%02x ",(MS_U32)pu8Read[u32i]);
    }
    ULOGD("DMX", "\n");
  #endif
#endif
}

static DMX_FILTER_STATUS _DMX_GetSect(MS_U32 u32DmxId, MS_U8* pu8Buf, MS_U32 u32BufSize, MS_U32* pu32SecSize, MS_U32* pu32RmnSize, DMX_CheckCb pfCheckCB)
{
    MS_U32      u32RbSize;
    MS_U32      u32SecBufSize= 0;
    MS_U8*      pu8Start;
    MS_U8*      pu8End = 0;
    MS_U8*      pu8Read;
    MS_U8*      pu8Write;
    MS_U32      u32CopySize= 0;
    MS_U32      u32SecLen= 0;
    MS_BOOL     bCopy;
    MS_PHY      temp = 0;
    MS_VIRT     virtStart = 0;
    MS_VIRT     virtEnd = 0;
    MS_VIRT     virtRead = 0;
    MS_VIRT     virtWrite = 0;

    //DMX_ASSERT(DMX_MAX_FLTID> u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));

#ifndef MSOS_TYPE_LINUX_KERNEL
    MsOS_Sync();
#endif

    MsOS_ReadMemory();

    // Get Section buffer read pointer
    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetReadAddr(0, _pdmx_res->_FltList[u32DmxId].SecBufId, &temp))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    virtRead = (MS_VIRT)temp;
    // Get Section buffer write pointer
    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetWriteAddr(0, _pdmx_res->_FltList[u32DmxId].SecBufId, &temp))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    virtWrite = (MS_VIRT)temp;

    *pu32SecSize = 0;
    *pu32RmnSize = 0;
    if (virtRead == virtWrite)
    {
        return DMX_FILTER_STATUS_ERROR; //fail
    }

    // Get Section buffer start address
    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetBufStart(0, _pdmx_res->_FltList[u32DmxId].SecBufId, &temp))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    virtStart = (MS_VIRT)temp;
    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetBufSize(0, _pdmx_res->_FltList[u32DmxId].SecBufId, &u32SecBufSize))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    //ULOGD("DMX", "[%s][%d][%d] bufsize %x\n", __FUNCTION__, __LINE__, (int)u8DmxId, (unsigned int)u32SecBufSize);
    //ASSERT(u32SecBufSize == _pdmx_res->_FltList[(u8DmxId)].FltInfo.Info.SectInfo.SectBufSize);
    // Get Section buffer end address
    virtEnd =            virtStart+ u32SecBufSize;

    if(virtRead >= virtEnd || virtRead < virtStart)
    {
        ULOGE("DMX", "[DMX][%d][%d] FATAL ERROR: Section buffer read address out of range!!!!!!!!\n", __LINE__, (unsigned int)MsOS_GetSystemTime());
        ULOGE("DMX", "[DMX][%d] Filter %d, Start %x, End %x , Read %x, Write %x \n", __LINE__, (int)u32DmxId, (unsigned int)virtStart, (unsigned int)virtEnd, (unsigned int)virtRead, (unsigned int)virtWrite);
        return DMX_FILTER_STATUS_ERROR;
    }

    if(virtWrite >= virtEnd || virtWrite < virtStart)
    {
        ULOGE("DMX", "[DMX][%d][%d] FATAL ERROR: Section buffer write address out of range!!!!!!!!\n", __LINE__, (unsigned int)MsOS_GetSystemTime());
        ULOGE("DMX", "[DMX][%d] Filter %d, Start %x, End %x , Read %x, Write %x \n", __LINE__, (int)u32DmxId, (unsigned int)virtStart, (unsigned int)virtEnd, (unsigned int)virtRead, (unsigned int)virtWrite);
        return DMX_FILTER_STATUS_ERROR;
    }

    *pu32RmnSize = u32RbSize = _DMX_RingBuffer_DataSize(virtRead, virtWrite, virtStart, virtEnd);
    if (3 > u32RbSize)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    MS_ASSERT(virtStart == (MS_VIRT)((MS_VIRT)_pdmx_res->_FltList[(u32DmxId)].FltInfo.Info.SectInfo.SectBufAddr));
    if ((virtRead==0)||(virtWrite==0)||(virtStart==0))
    {
        ULOGE("DMX", "[DMX][%d][%d] FATAL ERROR: Section buffer address illegal!!!!!!!!\n", __LINE__, (unsigned int)MsOS_GetSystemTime());
        ULOGE("DMX", "[DMX][%d] Filter %d, Start %x, End %x , Read %x, Write %x \n", __LINE__, (int)u32DmxId, (unsigned int)virtStart, (unsigned int)virtEnd, (unsigned int)virtRead, (unsigned int)virtWrite);
        return DMX_FILTER_STATUS_ERROR;
    }

    // transfer physical address to uncache address
    pu8Read  =          (MS_U8*)MS_PA2KSEG1((MS_PHY)virtRead);
    pu8Write =          (MS_U8*)MS_PA2KSEG1((MS_PHY)virtWrite);
    pu8Start =          (MS_U8*)MS_PA2KSEG1((MS_PHY)virtStart);
    //pu8End   =          (MS_U8*)MS_PA2KSEG1((MS_PHY)virtEnd);
    pu8End   =          pu8Start + u32SecBufSize;

    /* ULOGD("DMX", "(Start, End, Read, Write) = (0x%08x, 0x%08x, 0x%08x, 0x%08x)\n",
            (int)pu8Start,
            (int)pu8End,
            (int)pu8Read,
            (int)pu8Write);*/

#ifndef MSOS_TYPE_LINUX_KERNEL
    bCopy = (pfCheckCB) ? pfCheckCB((MS_U8)u32DmxId, pu8Start, pu8End, pu8Read, pu8Write) : TRUE;
#else
    bCopy = TRUE; //can not callback user function pointer in kernel driver
#endif

    // Check not enought section data
    if ((pu8Read + 3) <= pu8End)
    {
        u32SecLen =     ((*(pu8Read+ 1) & 0x0F) << 8) + *(pu8Read+ 2);
    }
    else if ((pu8Read + 2) <= pu8End)
    {
        u32SecLen =     ((*(pu8Read+ 1) & 0x0F) << 8) + *(pu8Start);
    }
    else // ((uReadAddr+1) == uEndAddr)
    {
        u32SecLen =     ((*(pu8Start) & 0x0F) << 8) + *(pu8Start+ 1);
    }
    u32SecLen +=        3;

    if (u32SecLen > u32RbSize)
    {
        DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Section length is larger than what section buffer has %u %u  DMXID : %u\n", __LINE__,
            (unsigned int)u32SecLen, (unsigned int)u32RbSize, (unsigned int)u32DmxId ));
         _DumpSecHeader(pu8Start, pu8End, pu8Read, pu8Write);
        return DMX_FILTER_STATUS_ERROR;
    }

    if (u32SecLen> 4096)
    {
        DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Section length is larger than 4096 bytes DMXID = %d \n", __LINE__,(unsigned int)u32DmxId));
        // Vincent.Lin request it
        return DMX_FILTER_STATUS_ERROR;
    }

    if(u32BufSize == 0)
    {
        *pu32SecSize = u32SecLen;
        return DMX_FILTER_STATUS_OK;
    }

    if ((u32SecLen > u32BufSize) || (!pu8Buf))
    {
        DMX_ASSERT(u32SecLen <= u32BufSize, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] fitler %d. Section data size > given buffer size (%u, %u)\n",
            __LINE__, (unsigned int)u32DmxId, (unsigned int)u32SecLen, (unsigned int)u32BufSize)));
        _DumpSecHeader(pu8Start, pu8End, pu8Read, pu8Write);
        bCopy = FALSE;
    }

    if ((pu8Read+ u32SecLen) >= pu8End)
    {
        u32CopySize =       pu8End - pu8Read;

        if (bCopy)
        {
            CMP_CPY_TO_USER(pu8Buf, (void*)pu8Read, u32CopySize);
        }
        pu8Read=            pu8Start;
        u32SecLen-=         u32CopySize;
        *pu32SecSize+=      u32CopySize;
        pu8Buf+=            u32CopySize;
    }
    if (u32SecLen)
    {
        if (bCopy)
        {
            CMP_CPY_TO_USER(pu8Buf, (void*)pu8Read, u32SecLen);
        }
        *pu32SecSize+=      u32SecLen;
        pu8Read+=           u32SecLen;
    }

    *pu32RmnSize -= *pu32SecSize;
    if (FALSE == bCopy)
    {
        *pu32SecSize = 0;
    }

    MS_ASSERT(pu8Read< pu8End);

    if (DRVTSP_OK != MDrv_TSP_SecFlt_SetReadAddr(0, _pdmx_res->_FltList[u32DmxId].SecBufId, MS_VA2PA((MS_VIRT)pu8Read)))
    {
        DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[%s][%d] MDrv_TSP_SecFlt_SetReadAddr fail %d %d\n", __FUNCTION__, __LINE__, (int)u32DmxId, (int)_pdmx_res->_FltList[u32DmxId].SecBufId));
    }

    if(*pu32RmnSize >= 3)
    {
        extern void MDrv_TSP_SecFlt_Update(MS_U32 u32EngId, MS_U32 u32SecFltId);
        // Check not enought section data
        if ((pu8Read + 3) <= pu8End)
        {
            u32SecLen =     ((*(pu8Read+ 1) & 0x0F) << 8) + *(pu8Read+ 2);
        }
        else if ((pu8Read + 2) <= pu8End)
        {
            u32SecLen =     ((*(pu8Read+ 1) & 0x0F) << 8) + *(pu8Start);
        }
        else // ((uReadAddr+1) == uEndAddr)
        {
            u32SecLen =     ((*(pu8Start) & 0x0F) << 8) + *(pu8Start+ 1);
        }
        u32SecLen +=        3;
        if (u32SecLen <= *pu32RmnSize)
        {
            MDrv_TSP_SecFlt_Update(0,_pdmx_res->_FltList[u32DmxId].SecBufId);
        }
    }

    return DMX_FILTER_STATUS_OK;
}

//------------------------------------------------------------------------------
// API implementation
//------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// Initialize Demux API
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @param pFwAddr \b IN: TSP firmware address in DRAM physical address
/// @param u32FwSize \b IN: TSP firmware size
/// It should be called before calling any other Demux API functions.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SetFW(MS_PHY pFwAddr, MS_U32 u32FwSize)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
    DMX_ASSERT(!(pFwAddr & 0xFF), DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] FW address must be 128 byte alignment\n", __LINE__)));
    DMX_ASSERT((u32FwSize > 0), DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] FW size is 0\n", __LINE__)));

    _pdmx_res->_phyTspFwAddr = pFwAddr;
    _pdmx_res->_u32TspFwSize = u32FwSize;
#endif

    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Initialize Demux API
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
/// It should be called before calling any other Demux API functions.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Init(void)
{
    MS_U32 temp= 0;
    DMX_FILTER_STATUS Res = DMX_FILTER_STATUS_OK;

    //printf("[%s] _bFWInit %d, _bEnableInitRefCnt %d, _u8InitRefCnt %d \n", __FUNCTION__, (int)_pdmx_res->_bFWInit, (int)_pdmx_res->_bEnableInitRefCnt, (int)_pdmx_res->_u8InitRefCnt );

    DMX_ASSERT((_pdmx_res != 0), DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%s][%06d] data pointer is 0!", __FUNCTION__, __LINE__)));

    _pdmx_res->_u8InitRefCnt++;
    if((_pdmx_res->_bEnableInitRefCnt) && (_pdmx_res->_u8InitRefCnt != 1))//not the first init
    {
        return DMX_FILTER_STATUS_OK;
    }

#ifdef MSOS_TYPE_LINUX_KERNEL
    if(_pdmx_res->_bFWInit == TRUE)
    {
        return DMX_FILTER_STATUS_OK;
    }
#endif //end of MSOS_TYPE_LINUX_KERNEL

    if (-1 != _pdmx_res->_Dmx_Mutex)
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Initialization more than once\n", __LINE__)));
    }

    _pdmx_res->_Dmx_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "MAPI init", MSOS_PROCESS_SHARED);
    if (-1 == _pdmx_res->_Dmx_Mutex )
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] create mutex fail\n", __LINE__)));
    }

    if (-1 != _pdmx_res->_Dmx_Pvr_Mutex)
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Create PVR mutex more than once\n", __LINE__)));
    }

    _pdmx_res->_Dmx_Pvr_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char*)"MAPI PVR mutex", MSOS_PROCESS_SHARED);
    if (-1 == _pdmx_res->_Dmx_Pvr_Mutex )
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] create PVR mutex fail\n", __LINE__)));
    }

    if (-1 != _pdmx_res->_Dmx_MMFI_Mutex)
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Create MMFI mutex more than once\n", __LINE__)));
    }

    _pdmx_res->_Dmx_MMFI_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, (char*)"MAPI MMFI mutex", MSOS_PROCESS_SHARED);
    if (-1 == _pdmx_res->_Dmx_MMFI_Mutex )
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Create MMFI mutex fail\n", __LINE__)));
    }

#ifdef TSO_ENABLE
    if (-1 != _pdmx_res->_Dmx_TSO_Mutex)
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Create TSO mutex more than once\n", __LINE__)));
    }
    _pdmx_res->_Dmx_TSO_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "MAPI TSO mutex", MSOS_PROCESS_SHARED);
    if (-1 == _pdmx_res->_Dmx_TSO_Mutex )
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Create TSO mutex fail\n", __LINE__)));
    }
#endif

    _DMX_ENTRY();
    _DMX_PVR_ENTRY();
    _DMX_MMFI_ENTRY();

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0))

    if (DRVTSP_OK != MDrv_TSP_Init(_pdmx_res->_phyTspFwAddr, _pdmx_res->_u32TspFwSize))
    {
        MsOS_ReleaseMutex(_pdmx_res->_Dmx_MMFI_Mutex);
        MsOS_ReleaseMutex(_pdmx_res->_Dmx_Pvr_Mutex);
        DMX_ASSERT2(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] tsp init fail\n", __LINE__)));
    }

    if (DRVTSP_OK != MDrv_TSP_GetCap(E_DRVTSP_CAP_PID_FILTER_NUM, &temp))
    {
        MS_ASSERT(0);
        Res =  DMX_FILTER_STATUS_ERROR;
    }

    _pdmx_res->_u32TspFltNum = temp;

  #ifdef MMFILEIN
    MDrv_MMFI_Init();
  #endif

  #ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();
    MDrv_TSO_Init();
    MsOS_ReleaseMutex(_pdmx_res->_Dmx_TSO_Mutex);
  #endif

#else  // For TSP_VER_4_0

    if (DRVTSP_OK != MDrv_TSP_GetCap(E_DRVTSP_CAP_FILTER_NUM, &temp))
    {
        MS_ASSERT(0);
    }

    temp = temp + 1; // 1 record PID filter for backward compatibility

    _pdmx_res->_u32TspFltNum = temp ;

#endif  // End of TSP_VER_4_0

    _FLT_LIST_RESET();

    MsOS_ReleaseMutex(_pdmx_res->_Dmx_MMFI_Mutex);
    MsOS_ReleaseMutex(_pdmx_res->_Dmx_Pvr_Mutex);

#if DMX_DEBUG
    _DMX_Debug(NULL);
#endif

    //printf("[%s] End \n", __FUNCTION__);

    _DMX_RETURN(Res);
}

//-------------------------------------------------------------------------------------------------
/// Initialize Demux API for Virtual Quere Enable
/// @param param                \b IN: Init Virtual Quere Setting
/// @return                     DMX_FILTER_STATUS_ERROR - Failure
/// @note if want to use virtual quere, please call this API after calling MApi_DMX_Init. (VQ does not support for all chip)
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSPInit(DMX_TSPParam *param)
{
    if(_pdmx_res->_bEnableInitRefCnt)
    {
        if(_pdmx_res->_u8InitRefCnt > 1) //not the first init
            return DMX_FILTER_STATUS_OK;
    }

    if (_pdmx_res->_bFWInit == TRUE)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0))

    _DMX_ENTRY();

  #if (defined(VQ_ENABLE))

    #ifdef DMX_RESET_FI_TIMESTAMP
    _pdmx_res->_u32FI192DummyBufSize = 576UL;
    if(param->u32VQSize > _pdmx_res->_u32FI192DummyBufSize + 200UL)
    {
        MS_U8* pdummy = 0;
        _pdmx_res->_phyFI192DummyBufAddr = (param->phyVQAddr + param->u32VQSize - _pdmx_res->_u32FI192DummyBufSize) & 0xFFFFFFF0;
        param->u32VQSize = _pdmx_res->_phyFI192DummyBufAddr - param->phyVQAddr;
        pdummy = (MS_U8*)_DMX_PA2KSEG1(_pdmx_res->_phyFI192DummyBufAddr, _pdmx_res->_u32FI192DummyBufSize);
        if(pdummy != 0)
        {
            memset(pdummy, 0x00, _pdmx_res->_u32FI192DummyBufSize);
            pdummy[4] = 0x47;
            pdummy[196] = 0x47;
            pdummy[388] = 0x47;
            ULOGD("DMX", "[DMX] VQ Buf %x ,VQ Size %x, Dummy Buf %x\n", (unsigned int)param->phyVQAddr, (unsigned int)param->u32VQSize, (unsigned int)_pdmx_res->_phyFI192DummyBufAddr);
        }
    }
    #endif

    _pdmx_res->_phyVQAddr = param->phyVQAddr;
    _pdmx_res->_u32VQSize = param->u32VQSize;

    MDrv_TSP_SetVQueBuf(param->phyVQAddr, param->u32VQSize);
    MDrv_TSP_VQueue_OverflowInt_En(FALSE);
    MDrv_TSP_VQueEnable(TRUE);

    #if (defined(TS2_IF_SUPPORT) && (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0)))
    _pdmx_res->_bVQEnabled = TRUE;
    #endif

  #endif // End of VQ_ENABLE

    _pdmx_res->_bFWInit = TRUE;

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#else  // For TSP_VER_4_0

    MS_S32              i;
    TSP_InitParam       tsp_param;

    _DMX_ENTRY();

    // check user param if it's init
    for (i = 0; i < sizeof(param->_zero_reserved); i++)
    {
        if ( (*(((MS_U8*)(&(param->_zero_reserved))) + i)) != 0 )
        {
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
        }
    }

    _pdmx_res->_phyTspFwAddr = param->phyFWAddr;
    _pdmx_res->_u32TspFwSize = param->u32FWSize;
    _pdmx_res->_phyVQAddr = param->phyVQAddr;
    _pdmx_res->_u32VQSize = param->u32VQSize;
    _pdmx_res->_u32IsHK = param->u32IsHK;

    tsp_param.phyFWAddr = param->phyFWAddr;
    tsp_param.u32FWSize = param->u32FWSize;
    tsp_param.phyVQAddr = param->phyVQAddr;
    tsp_param.u32VQSize = param->u32VQSize;

    if (DRVTSP_OK != MDrv_TSP_Init(&tsp_param))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

  #ifdef MMFILEIN
    _DMX_MMFI_ENTRY();
    MDrv_MMFI_Init();
    MsOS_ReleaseMutex(_pdmx_res->_Dmx_MMFI_Mutex);
  #endif

  #ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();
    MDrv_TSO_Init();
    MsOS_ReleaseMutex(_pdmx_res->_Dmx_TSO_Mutex);
  #endif

    _pdmx_res->_bFWInit = TRUE;

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#endif  // End of TSP_VER_4_0
}

//-------------------------------------------------------------------------------------------------
/// Get init parameters of MApi_DMX_TSPInit
/// @ingroup TSP_General
/// @param pstParam       \b OUT: init parameters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
//-------------------------------------------------------------------------------------------------
MS_U32 _MApi_DMX_TSPInit_GetConfig(DMX_TSPParam *param)
{
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    return UTOPIA_STATUS_NOT_SUPPORTED;

#else  // For TSP_VER_4_0

     _DMX_ENTRY();

    if(_pdmx_res->_bFWInit != TRUE)
        _DMX_RETURN(UTOPIA_STATUS_ERR_NOT_AVAIL);

    param->phyFWAddr = _pdmx_res->_phyTspFwAddr;
    param->u32FWSize = _pdmx_res->_u32TspFwSize;
    param->phyVQAddr = _pdmx_res->_phyVQAddr;
    param->u32VQSize = _pdmx_res->_u32VQSize;
    param->u32IsHK = _pdmx_res->_u32IsHK;

    MS_U8 i = 0;
    for(i = 0; i < sizeof(param->_zero_reserved)/sizeof(MS_U32); i++)
        param->_zero_reserved[i] = 0;

    _DMX_RETURN(UTOPIA_STATUS_SUCCESS);

#endif  // End of TSP_VER_4_0
}

//-------------------------------------------------------------------------------------------------
/// Exit Demux API
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Exit(void)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    //printf("[%s] _bFWInit %d, _bEnableInitRefCnt %d, _u8InitRefCnt %d \n", __FUNCTION__, (int)_pdmx_res->_bFWInit, (int)_pdmx_res->_bEnableInitRefCnt, (int)_pdmx_res->_u8InitRefCnt );

    if(_pdmx_res->_u8InitRefCnt > 0)
        _pdmx_res->_u8InitRefCnt--;
    if((_pdmx_res->_bEnableInitRefCnt) && (_pdmx_res->_u8InitRefCnt > 0))
    {
        return DMX_FILTER_STATUS_OK;
    }

#ifdef MSOS_TYPE_LINUX_KERNEL
    if(_pdmx_res->_bFWInit == FALSE)
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    if(_pdmx_res->_u8InitRefCnt > 0)
    {
        return DMX_FILTER_STATUS_MULTIPRC_RUNNING;
    }
#endif

    if (-1!= _pdmx_res->_Dmx_Mutex)
    {
        _DMX_ENTRY();
    }
    else
    {
       ;// do nothing
    }

    if (-1!= _pdmx_res->_Dmx_Pvr_Mutex)
    {
        _DMX_PVR_ENTRY();
    }
    else
    {
       ;// do nothing
    }

    if (-1!= _pdmx_res->_Dmx_MMFI_Mutex)
    {
        _DMX_MMFI_ENTRY();
    }
    else
    {
       ;// do nothing
    }

#ifdef TSO_ENABLE
    if (-1!= _pdmx_res->_Dmx_TSO_Mutex)
    {
        _DMX_TSO_ENTRY();
    }
    else
    {
       ;// do nothing
    }
#endif

#ifdef MMFILEIN
    MDrv_MMFI_Exit();
#endif

#ifdef TSO_ENABLE
    MDrv_TSO_Exit();
    if (_pdmx_res->_Dmx_TSO_Mutex != -1 )
    {
        MsOS_ReleaseMutex(_pdmx_res->_Dmx_TSO_Mutex);
        MsOS_DeleteMutex(_pdmx_res->_Dmx_TSO_Mutex);
        _pdmx_res->_Dmx_TSO_Mutex= -1;
    }
#endif

    if (MDrv_TSP_Exit() == DRVTSP_OK)
    {
        if (_pdmx_res->_Dmx_Mutex != -1 )
        {
            MsOS_ReleaseMutex(_pdmx_res->_Dmx_Mutex);
            MsOS_DeleteMutex(_pdmx_res->_Dmx_Mutex);
            _pdmx_res->_Dmx_Mutex= -1;
        }
        if (_pdmx_res->_Dmx_Pvr_Mutex != -1 )
        {
            MsOS_ReleaseMutex(_pdmx_res->_Dmx_Pvr_Mutex);
            MsOS_DeleteMutex(_pdmx_res->_Dmx_Pvr_Mutex);
            _pdmx_res->_Dmx_Pvr_Mutex= -1;
        }
        if (_pdmx_res->_Dmx_MMFI_Mutex != -1 )
        {
            MsOS_ReleaseMutex(_pdmx_res->_Dmx_MMFI_Mutex);
            MsOS_DeleteMutex(_pdmx_res->_Dmx_MMFI_Mutex);
            _pdmx_res->_Dmx_MMFI_Mutex= -1;
        }

        _pdmx_res->_bFWInit = FALSE;

#ifdef DMX_UTOPIA_20
        _pdmx_res           = NULL;
#endif
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        // do nothing
    }

    if (-1!= _pdmx_res->_Dmx_Mutex)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR; ; //donothing
    }
}

//-------------------------------------------------------------------------------------------------
/// Reset Demux API
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Reset(void)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

     _DMX_ENTRY();

    if (MDrv_TSP_Reset() == DRVTSP_OK)
    {
#ifdef MMFILEIN
        MDrv_MMFI_ResetAll();
#endif
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
}

//-------------------------------------------------------------------------------------------------
/// Force exit Demux API
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @special case for MM
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_ForceExit(void)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    if (MDrv_TSP_PowerOff() == DRVTSP_OK)
    {
#ifdef MMFILEIN
        MDrv_MMFI_Exit();
        MDrv_MMFI_ResetAll();
#endif
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }

    _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
}

//--------------------------------------------------------------------------------------------------
/// Resume TSP driver
/// @param phyFWAddr \b IN: TSP FW address
/// @param u32FWSize \b IN: TSP FW size
/// @return TSP_Result
/// @note
/// Restore TSP driver states from DRAM
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Resume(MS_PHY phyFWAddr, MS_U32 u32FWSize)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    //printf("[%s] _bFWInit %d, _bEnableInitRefCnt %d, _u8InitRefCnt %d \n", __FUNCTION__, (int)_pdmx_res->_bFWInit, (int)_pdmx_res->_bEnableInitRefCnt, (int)_pdmx_res->_u8InitRefCnt );

    if(_pdmx_res == 0)
    {
        //DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%s][%06d] Share memoery pointer is 0!", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }

    if(_pdmx_res->_bFWInit == FALSE)
    {
        return DMX_FILTER_STATUS_OK;
    }

    _DMX_STR_ENTRY();

#ifdef MSOS_TYPE_LINUX_KERNEL
    phyFWAddr = _pdmx_res->_phyTspFwAddr;
    u32FWSize = _pdmx_res->_u32TspFwSize;
#endif

    if(MDrv_TSP_Resume(phyFWAddr, u32FWSize) != DRVTSP_OK)
    {
        _DMX_STR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
  #if (defined(VQ_ENABLE))
    MDrv_TSP_SetVQueBuf(_pdmx_res->_phyVQAddr, _pdmx_res->_u32VQSize);
    MDrv_TSP_VQueEnable(TRUE);
  #endif
    MDrv_TSP_Resume_Filter();
#endif

#ifdef MMFILEIN
  #ifdef MSOS_TYPE_LINUX_KERNEL
    MDrv_MMFI_Resume();
  #else
    MDrv_MMFI_Init();
  #endif
#endif

#ifdef TSO_ENABLE
  #ifdef MSOS_TYPE_LINUX_KERNEL
    MDrv_TSO_Resume();
  #else
    _DMX_TSO_ENTRY();
     MDrv_TSO_Init();
     MsOS_ReleaseMutex(_pdmx_res->_Dmx_TSO_Mutex);
  #endif
#endif

#ifdef FQ_ENABLE
  #ifdef MSOS_TYPE_LINUX_KERNEL
    MDrv_FQ_Resume();
  #endif
#endif

    //printf("[%s][%d] OK \n", __FUNCTION__, __LINE__);

    _DMX_STR_RETURN(DMX_FILTER_STATUS_OK);
}

//--------------------------------------------------------------------------------------------------
/// Suspend TSP driver
/// @return TSP_Result
/// @note
/// Save TSP driver states to DRAM
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Suspend(void)
{
    DMX_FILTER_STATUS ret;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    //printf("[%s] _bFWInit %d, _bEnableInitRefCnt %d, _u8InitRefCnt %d \n", __FUNCTION__, (int)_pdmx_res->_bFWInit, (int)_pdmx_res->_bEnableInitRefCnt, (int)_pdmx_res->_u8InitRefCnt );

    if(_pdmx_res == 0)
    {
        //DMX_DBGMSG(DMX_DBG_ERR, printf("[MAPI DMX][%s][%06d] Share memoery pointer is 0!", __FUNCTION__, __LINE__));
        return DMX_FILTER_STATUS_ERROR;
    }

    if(_pdmx_res->_bFWInit == FALSE)
    {
        return DMX_FILTER_STATUS_OK;
    }

    _DMX_STR_ENTRY();

#ifdef MMFILEIN
    #ifdef MSOS_TYPE_LINUX_KERNEL
    MDrv_MMFI_Suspend();
    #else
    MDrv_MMFI_Exit();
    #endif
#endif

#ifdef TSO_ENABLE
    #ifdef MSOS_TYPE_LINUX_KERNEL
    MDrv_TSO_Suspend();
    #else
    MDrv_TSO_Exit();
    #endif
#endif

#ifdef FQ_ENABLE
    #ifdef MSOS_TYPE_LINUX_KERNEL
    MDrv_FQ_Suspend();
    #endif
#endif

    ret = ((MDrv_TSP_Suspend() == DRVTSP_OK) ? DMX_FILTER_STATUS_OK : DMX_FILTER_STATUS_ERROR);

    //printf("[%s][%d] ret %d \n", __FUNCTION__, __LINE__, (int)ret);

    _DMX_STR_RETURN(ret);

}

//-------------------------------------------------------------------------------------------------
/// Release TSP driver HW lock semaphone
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_ReleaseSemaphone(void)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0))
    MS_DEBUG_MSG(DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "%s [OBSOLETED]\n", __FUNCTION__)));
    // @FIXME: check root cause of why coprocess has unrelease lock condition.
    if (MDrv_TSP_HW_Lock_Release() == DRVTSP_OK)
    {
        return (DMX_FILTER_STATUS_OK);
    }
#endif

    return DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Set Demux Flow
/// @param DmxFlow \b IN: DMX_FLOW_PLAYBACK for playback flow
///                       DMX_FLOW_PVR for recording flow
///                       DMX_FLOW_PVRCA for CA recording flow
///                       Below Items are not support for all chips:
///                       DMX_FLOW_PVR1 for second recording flow.
///                       DMX_FLOW_PVR2 for third recording flow.
///                       DMX_FLOW_FILEIN_MM for file-in playback with independed audio path.
///                       DMX_FLOW_FILEIN_MM3D for file-in playback with 2nd video input source .
///
/// @param pDmxFlowInput \b OUT: pointer to store DMX input flow value.
//                              DMX_FLOW_INPUT_DEMOD for input from DVBC
///                            DMX_FLOW_INPUT_EXT_INPUT0 for input from TS0 Interface
///                            DMX_FLOW_INPUT_EXT_INPUT1 for input from TS1 Interface
///                            DMX_FLOW_INPUT_EXT_INPUT2 for input from TS2 Interface
///                            DMX_FLOW_INPUT_EXT_INPUT3 for input from TS3 Interface
///                            DMX_FLOW_INPUT_MEM for input from memory
///                            Below Items are not support for all chips:
///                            DMX_FLOW_INPUT_EXT_INPUT1for input from TS2 Interface
/// @param pbClkInv    \b OUT: pointer to store clock phase inversion
/// @param pbExtSync \b OUT: pointer to store sync by external signal
/// @param pbParallel  \b OUT: pointer to store parallel interface or serial interface
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0)
DMX_FILTER_STATUS _MApi_DMX_Get_FlowInput_Status(DMX_FLOW DmxFlow, DMX_FLOW_INPUT *pDmxFlowInput, MS_BOOL *pbClkInv, MS_BOOL *pbExtSync, MS_BOOL *pbParallel)
{
    DrvTSP_If_Set Ifset;
    DrvTSP_If eif = E_DRVTSP_IF_TS0;
    MS_U16 u16Clk = 0;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    *pDmxFlowInput = DMX_FLOW_INPUT_MEM;
    *pbClkInv = FALSE;
    *pbExtSync = FALSE;
    *pbParallel = FALSE;

    switch(DmxFlow)
    {
        case DMX_FLOW_PLAYBACK:
        case DMX_FLOW_PVRCA:
        case DMX_FLOW_CIPHSS_PVRCA:
            eif = E_DRVTSP_IF_TS0;
            break;
        case DMX_FLOW_PLAYBACK1:
        case DMX_FLOW_PVR:
        case DMX_FLOW_CIPHSS_PLAYBACK:
        case DMX_FLOW_PVRCA1:
            eif = E_DRVTSP_IF_TS1;
            break;
        case DMX_FLOW_PLAYBACK2:
        case DMX_FLOW_PVR1:
            eif = E_DRVTSP_IF_TS2;
            break;
        #ifdef MERGE_STR_SUPPORT
        case DMX_FLOW_PLAYBACK_FI:
            eif = E_DRVTSP_IF_FI;
            break;
        #endif
        case DMX_FLOW_PVR2:
            eif = E_DRVTSP_IF_TS3;
            break;
        case DMX_FLOW_MMFI0:
        case DMX_FLOW_MMFI1:
            DMX_DBGMSG(DMX_DBG_INFO,ULOGD("DMX", "Flow Input: From Memory \n"));
            break;
        default:
            DMX_DBGMSG(DMX_DBG_INFO,ULOGD("DMX", "Flow Input: Not support \n"));
            return DMX_FILTER_STATUS_ERROR;
    }

    Ifset.ePad = E_DRVTSP_PAD_DEMOD;
    Ifset.bClkInv = FALSE;
    Ifset.bExtSync = FALSE;
    Ifset.bParallel = FALSE;

    if(MDrv_TSP_GetTSIFStatus(eif, &Ifset, &u16Clk) != DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    if(Ifset.ePad == E_DRVTSP_PAD_DEMOD)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_DEMOD;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Playback, "));
    }
    else if(Ifset.ePad == E_DRVTSP_PAD_EXT_INPUT0)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT0;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Extern TS0, "));
    }
    else if(Ifset.ePad == E_DRVTSP_PAD_EXT_INPUT1)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT1;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Extern TS1, "));
    }
    else if(Ifset.ePad == E_DRVTSP_PAD_EXT_INPUT2)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT2;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Extern TS2, "));
    }
    else if(Ifset.ePad == E_DRVTSP_PAD_EXT_INPUT3)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT3;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Extern TS3, "));
    }
    else if(Ifset.ePad == E_DRVTSP_PAD_EXT_INPUT4)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT4;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Extern TS4, "));
    }
    else if(Ifset.ePad == E_DRVTSP_PAD_EXT_INPUT5)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT5;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Extern TS5, "));
    }
    else if(Ifset.ePad == E_DRVTSP_PAD_TSO)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_TSO;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Extern TSO, "));
    }
    else
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_MEM;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: From Memory , "));
    }

    *pbClkInv = Ifset.bClkInv;
    *pbExtSync = Ifset.bExtSync;
    *pbParallel = Ifset.bParallel;

    if(*pbClkInv)
    {
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "CLK Inverse, "));
    }
    if(*pbExtSync)
    {
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "External Sync, "));
    }
    if(*pbParallel)
    {
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Parallel mode \n"));
    }
    return (DMX_FILTER_STATUS_OK);

} // End of TSP_VER_3_0

#elif (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))

DMX_FILTER_STATUS _MApi_DMX_Get_FlowInput_Status(DMX_FLOW DmxFlow, DMX_FLOW_INPUT *pDmxFlowInput, MS_BOOL *pbClkInv, MS_BOOL *pbExtSync, MS_BOOL *pbParallel)
{
    DrvTSP_If_Set Ifset;
    DrvTSP_If eif = E_DRVTSP_IF_PLAYBACK;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    *pDmxFlowInput = DMX_FLOW_INPUT_MEM;
    *pbClkInv = FALSE;
    *pbExtSync = FALSE;
    *pbParallel = FALSE;

    switch(DmxFlow)
    {
        case DMX_FLOW_PLAYBACK:
        case DMX_FLOW_CIPHSS_PLAYBACK:
            eif = E_DRVTSP_IF_PLAYBACK;
            break;
        case DMX_FLOW_PVR:
        case DMX_FLOW_PVRCA:
        case DMX_FLOW_CIPHSS_PVRCA:
        case DMX_FLOW_PVRCA1:
            eif = E_DRVTSP_IF_PVR0;
            break;
  #ifdef TS2_IF_SUPPORT
        #ifdef TS2_LIVE_SUPPORT
        case DMX_FLOW_PLAYBACK1:
        #endif
        case DMX_FLOW_PVR1:
            eif = E_DRVTSP_IF_PVR1;
            break;
  #endif
  #ifdef TS3_IF_SUPPORT
        case DMX_FLOW_PVR2:
            eif = E_DRVTSP_IF_PVR2;
            break;
  #endif
        default:
            DMX_DBGMSG(DMX_DBG_INFO,ULOGD("DMX", "Flow Input: From Memory \n"));
            return DMX_FILTER_STATUS_ERROR;

    }

    Ifset.ePad = E_DRVTSP_PAD_DEMOD;
    Ifset.bClkInv = FALSE;
    Ifset.bExtSync = FALSE;
    Ifset.bParallel = FALSE;

    if(MDrv_TSP_GetTSIFStatus(eif, &Ifset) != DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    if(Ifset.ePad == E_DRVTSP_PAD_DEMOD)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_DEMOD;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Playback, "));
    }
    else if(Ifset.ePad == E_DRVTSP_PAD_EXT_INPUT0)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT0;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Extern TS0, "));
    }
    else if(Ifset.ePad == E_DRVTSP_PAD_EXT_INPUT1)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT1;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Extern TS1, "));
    }
  #ifdef TS2_IF_SUPPORT
    else if(Ifset.ePad == E_DRVTSP_PAD_EXT_INPUT2)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT2;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Extern TS2, "));
    }
  #endif
  #ifdef TS3_IF_SUPPORT
    else if(Ifset.ePad == E_DRVTSP_PAD_EXT_INPUT3)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT3;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Extern TS3, "));
    }
  #endif

    *pbClkInv = Ifset.bClkInv;
    *pbExtSync = Ifset.bExtSync;
    *pbParallel = Ifset.bParallel;

    if(*pbClkInv)
    {
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "CLK Inverse, "));
    }
    if(*pbExtSync)
    {
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "External Sync, "));
    }
    if(*pbParallel)
    {
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Parallel mode \n"));
    }

    return (DMX_FILTER_STATUS_OK);

} // End of TSP_VER_1_0

#else  // For TSP_VER_4_0

DMX_FILTER_STATUS _MApi_DMX_Get_FlowInput_Status(DMX_FLOW DmxFlow, DMX_FLOW_INPUT *pDmxFlowInput, MS_BOOL *pbClkInv, MS_BOOL *pbExtSync, MS_BOOL *pbParallel)
{
    DrvTSP_TsIf_Set TsIfset;
    MS_U32          u32TSIF = 0;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    *pDmxFlowInput = DMX_FLOW_INPUT_MEM;
    *pbClkInv = FALSE;
    *pbExtSync = FALSE;
    *pbParallel = FALSE;

    //if live-in and file-in of a DmxFlow map to diff TSIF, return the status of the live-in one
    if(_DMX_ApiDrv_DmxFlowMapping(DmxFlow, &u32TSIF) == FALSE)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    TsIfset.ePad = E_TSP_TS_INVALID;
    TsIfset.bClkInv = FALSE;
    TsIfset.bExtSync = FALSE;
    TsIfset.bParallel = FALSE;

    // get PAD and ClkInv for eTSIF
    if(MDrv_TSP_GetTSIFStatus((TSP_TSIF)u32TSIF, &(TsIfset.ePad), // @F_TODO do we have to modify this type convert into original type?
        &(TsIfset.bClkInv), &(TsIfset.bExtSync), &(TsIfset.bParallel)) != E_TSP_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    if(TsIfset.ePad == E_TSP_TS_DEMOD0)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_DEMOD;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: DEMOD, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_DEMOD1)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_DEMOD1;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: DEMOD1, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD0)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT0;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Extern Input0, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD1)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT1;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Extern Input1, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD2)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT2;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Extern Input2, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD3)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT3;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Extern Input3, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD4)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT4;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Extern Input4, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD5)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT5;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Extern Input5, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD6)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT6;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Extern Input6, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD7)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT7;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: Extern Input7, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD_TSO0)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_TSO;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: TSO0, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD_TSO1)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_TSO1;
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Flow Input: TSO1, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD0_3WIRE)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT0_3WIRE;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern Input0 3WIRE, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD1_3WIRE)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT1_3WIRE;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern Input1 3WIRE, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD2_3WIRE)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT2_3WIRE;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern Input2 3WIRE, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD3_3WIRE)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT3_3WIRE;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern Input3 3WIRE, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD4_3WIRE)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT4_3WIRE;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern Input4 3WIRE, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD5_3WIRE)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT5_3WIRE;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern Input5 3WIRE, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD6_3WIRE)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT6_3WIRE;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern Input6 3WIRE, "));
    }
    else if(TsIfset.ePad == E_TSP_TS_PAD7_3WIRE)
    {
        *pDmxFlowInput = DMX_FLOW_INPUT_EXT_INPUT7_3WIRE;
        DMX_DBGMSG(DMX_DBG_INFO, printf("Flow Input: Extern Input7 3WIRE, "));
    }

    *pbClkInv = TsIfset.bClkInv;
    *pbExtSync = TsIfset.bExtSync;
    *pbParallel = TsIfset.bParallel;

    if(*pbClkInv)
    {
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "CLK Inverse, "));
    }
    if(*pbExtSync)
    {
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "External Sync, "));
    }
    if(*pbParallel)
    {
        DMX_DBGMSG(DMX_DBG_INFO, ULOGD("DMX", "Parallel mode \n"));
    }

    return (DMX_FILTER_STATUS_OK);
}
#endif // End of TSP_VER_4_0

//-------------------------------------------------------------------------------------------------
/// Set Demux Flow
/// @param DmxFlow \b IN: DMX_FLOW_PLAYBACK for playback flow
///                       DMX_FLOW_PVR for recording flow
///                       DMX_FLOW_PVRCA for CA recording flow
///                       Below Items are not support for all chips:
///                       DMX_FLOW_PVR1 for second recording flow.
///                       DMX_FLOW_FILEIN_MM for file-in playback with independed audio path.
///                       DMX_FLOW_FILEIN_MM3D for file-in playback with 2nd video input source .
///
/// @param DmxFlowInput \b IN: DMX_FLOW_INPUT_DEMOD for input from DVBC
///                            DMX_FLOW_INPUT_EXT_INPUT0 for input from TS0 Interface
///                            DMX_FLOW_INPUT_EXT_INPUT1 for input from TS1 Interface
///                            DMX_FLOW_INPUT_MEM for input from memory
///                            Below Items are not support for all chips:
///                            DMX_FLOW_INPUT_EXT_INPUT1for input from TS2 Interface
/// @param bClkInv \b IN: TSin options: clock phase inversion
/// @param bExtSync \b IN: TSin options: sync by external signal
/// @param bParallel \b IN: TSin is parallel interface or serial interface
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_FlowSet(DMX_FLOW DmxFlow, DMX_FLOW_INPUT DmxFlowInput, MS_BOOL bClkInv, MS_BOOL bExtSync, MS_BOOL bParallel)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d (%d, %d, %d, %d, %d)\n",
        __FUNCTION__, __LINE__, (int)DmxFlow, (int)DmxFlowInput, (int)bClkInv, (int)bExtSync, (int)bParallel));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FLOWSET)
        dmx_dbg_print("[DMX FlwSet]\tFlow[0x%x] Input[0x%x] Inv[%d] Sync[%d] Parallel[%d]\n",
        (MS_U32)DmxFlow,
        (MS_U32)DmxFlowInput,
        (MS_U32)bClkInv,
        (MS_U32)bExtSync,
        (MS_U32)bParallel);
#endif

    _DMX_ENTRY();

    if(_DMX_FlowSet(DmxFlow, DmxFlowInput, bClkInv, bExtSync, bParallel) == FALSE)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

//--------------------------------------------------------------------------------------------------
/// Setting PVR flow
/// @param  Eng                                 \b IN: PVR Engine ID
/// @param  ePvrSrcTSIf                     \b IN: Select the record source
/// @param  bDscmbRec                     \b IN: TRUE for recording dscmbled stream; FALSE for recording orignal stream
/// @return DMX_FILTER_STATUS
/// @note
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_PVR_FlowSet(DMX_PVR_ENG Eng, DMX_TSIF ePvrSrcTSIf, MS_BOOL bDscmbRec)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d (DMX_PVR_ENG:%d, ePvrSrcTSIf:%d, BDSCMBREC:%d)\n",
           __FUNCTION__, __LINE__, (int)Eng, (int)ePvrSrcTSIf, (int)bDscmbRec));

    if(Eng == DMX_PVR_TSO0)
    {
#ifdef TSO_PVR_SUPPORT
        _DMX_TSO_ENTRY();

        if(ePvrSrcTSIf == DMX_TSIF_MMT)
        {
            MDrv_TSO_PVR_SelSrc(0, E_DRVTSO_PVR_SRC_MMT);
        }
        else
        {
            MDrv_TSO_PVR_SelSrc(0, E_DRVTSO_PVR_SRC_SVQ);
        }

        _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
#else
        return DMX_FILTER_STATUS_ERROR;

#endif //TSO_PVR_SUPPORT

    }

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    MS_U32 u32data = 0;
    DrvTSP_PKTDMXSrcType ePktDmx = E_DRVTSP_PKTSRC_DEMUX0;

    DMX_FLOW       DmxFlowOrg = DMX_FLOW_PLAYBACK;
    DMX_FLOW       DmxFlowNew = DMX_FLOW_PVR2;
    DMX_FLOW_INPUT FlowInput = DMX_FLOW_INPUT_DEMOD;
    MS_BOOL        bClkInv = FALSE;
    MS_BOOL        bExtSync = FALSE;
    MS_BOOL        bParallel = FALSE;
    MS_BOOL        bBlockEnable = FALSE;

  #if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))

    DrvTSP_DscmbCtrl DscmbPath = 0;

    //If not select input src, setting input src depend on CA Ctrl value
    if(ePvrSrcTSIf == DMX_TSIF_MAX)
    {
        MDrv_TSP_Dscmb_Path(0, &DscmbPath, FALSE);

        //ULOGD("DMX", "[%s][%d] DscmbPath 0x%lx \n", __FUNCTION__, __LINE__, DscmbPath);

        if((DscmbPath & E_DRVTSP_CA_OUTPUT_LIVE0) == 0)
        {
            ePvrSrcTSIf = DMX_TSIF_LIVE1;
        }
        else
        {
            ePvrSrcTSIf = DMX_TSIF_LIVE0;
        }
    }
  #endif  // End of TSP_VER_1_0

    if(MDrv_TSP_GetCap(E_DRVTSP_CAP_PVR_ENG_NUM, (void*)&u32data) != DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    if(Eng >= u32data)
    {
        return DMX_FILTER_STATUS_OK;
    }

    switch(ePvrSrcTSIf)
    {
        case DMX_TSIF_LIVE0:
            ePktDmx = E_DRVTSP_PKTSRC_DEMUX0;
            break;
        case DMX_TSIF_FILE0:
            ePktDmx = E_DRVTSP_PKTSRC_DEMUXFILE;
            bBlockEnable = TRUE;
            break;
        case DMX_TSIF_FILE1:
            bBlockEnable = TRUE;
            ePktDmx = E_DRVTSP_PKTSRC_DEMUX1;
            break;
        case DMX_TSIF_LIVE1:
            ePktDmx = E_DRVTSP_PKTSRC_DEMUX1;
            break;
        case DMX_TSIF_FILE2:
            bBlockEnable = TRUE;
            ePktDmx = E_DRVTSP_PKTSRC_DEMUX2;
            break;
        case DMX_TSIF_LIVE2:
            ePktDmx = E_DRVTSP_PKTSRC_DEMUX2;
            break;
        default:
            return DMX_FILTER_STATUS_ERROR;
    }
    MDrv_TSP_PVR_BlockEnable(Eng, bBlockEnable);

     _pdmx_res->_u32PVREngSrc[Eng] = (MS_U32)ePktDmx;

    // For Drvtsp3 and except of PVR_CB, just select PVR source directly.
    // For Drvtsp and Drvtsp3 PVR_CB, must set PVR flowset depnend on live flowset
  #if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0)
    if(Eng == DMX_PVR_EGN2) //PVRCB only
    {
  #endif //end of DRVTSP3 define

        switch(ePvrSrcTSIf)
        {
            case DMX_TSIF_LIVE0:
                DmxFlowOrg = DMX_FLOW_PLAYBACK;
                break;
            case DMX_TSIF_LIVE1:
                DmxFlowOrg = DMX_FLOW_PLAYBACK1;
                break;
            case DMX_TSIF_LIVE2:
                DmxFlowOrg = DMX_FLOW_PLAYBACK2;
                break;
            default:
                return DMX_FILTER_STATUS_ERROR;
        }

  #if(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))

        if(Eng == DMX_PVR_EGN0)
        {
            if(bDscmbRec)
            {
                // for napoli, if PVRCA path soruce is from TSIF1, not need to duplicate FLT0 Pid, and just set dscmb path
                // and just open PVR filter with MApi_DMX_Pvr_Pid_Open/MApi_DMX_Pvr_Pid_Close
                if(ePvrSrcTSIf == DMX_TSIF_LIVE1)
                {
                    DmxFlowNew = DMX_FLOW_PVR;
                }
                else
                {
                    MDrv_TSP_Dscmb_Path(0, &DscmbPath, FALSE);

                    //Already TS1 input, just set dscmb path
                    // and just open PVR filter with MApi_DMX_Pvr_Pid_Open/MApi_DMX_Pvr_Pid_Close
                    if(DscmbPath & E_DRVTSP_CA_INPUT_TSIF1)
                    {
                        DmxFlowNew = DMX_FLOW_PVR;
                    }
                    else
                    {
                        DmxFlowNew = DMX_FLOW_PVRCA;
                    }
                }
            }
            else
            {
                DmxFlowNew = DMX_FLOW_PVR;
            }
        }
        else if(Eng == DMX_PVR_EGN1)
        {
            if(bDscmbRec)
            {
                // for napoli, if PVRCA path soruce is from TSIF0, just set dscmb path
                // and just open PVR filter with MApi_DMX_Pvr_Pid_Open/MApi_DMX_Pvr_Pid_Close
                if(ePvrSrcTSIf == DMX_TSIF_LIVE0)
                {
                    DmxFlowNew = DMX_FLOW_PVR1;
                }
                else
                {
                    DmxFlowNew = DMX_FLOW_PVRCA1; //napoli u03 only
                }
            }
            else
            {
                DmxFlowNew = DMX_FLOW_PVR1;
            }
        }
  #endif //End of TSP_VER_1_0

        if(_MApi_DMX_Get_FlowInput_Status(DmxFlowOrg, &FlowInput, &bClkInv, &bExtSync, &bParallel) != DMX_FILTER_STATUS_OK)
        {
            return DMX_FILTER_STATUS_ERROR;
        }

        if(_MApi_DMX_FlowSet(DmxFlowNew, FlowInput, bClkInv, bExtSync, bParallel) == DMX_FILTER_STATUS_ERROR)
        {
            return DMX_FILTER_STATUS_ERROR;
        }
  #if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))
        if(bDscmbRec)
        {
            if(Eng == DMX_PVR_EGN0)
            {
                // for napoli, if PVRCA path soruce is from TSIF1, not need to duplicate FLT0 Pid, and just set dscmb path
                // and just open PVR filter with MApi_DMX_Pvr_Pid_Open/MApi_DMX_Pvr_Pid_Close
                if(ePvrSrcTSIf == DMX_TSIF_LIVE1)
                {
                    _DMX_FlowSet(DMX_FLOW_PVR, FlowInput, bClkInv, bExtSync, bParallel); //set TSIF1 for CA input
                    DscmbPath = E_DRVTSP_CA_INPUT_TSIF1|E_DRVTSP_CA_OUTPUT_REC0;
                    MDrv_TSP_Dscmb_Path(0, &DscmbPath, TRUE);
                }
                else
                {
                    MDrv_TSP_Dscmb_Path(0, &DscmbPath, FALSE);

                    //Already TS1 input, just set dscmb path
                    // and just open PVR filter with MApi_DMX_Pvr_Pid_Open/MApi_DMX_Pvr_Pid_Close
                    if(DscmbPath & E_DRVTSP_CA_INPUT_TSIF1)
                    {
                        _DMX_FlowSet(DMX_FLOW_PVR, FlowInput, bClkInv, bExtSync, bParallel);
                        DscmbPath |= E_DRVTSP_CA_OUTPUT_REC0;
                        MDrv_TSP_Dscmb_Path(0, &DscmbPath, TRUE);
                    }
                }
            }
            else if(Eng == DMX_PVR_EGN1)
            {
                // for napoli, if PVRCA path soruce is from TSIF0, just set dscmb path
                // and just open PVR filter with MApi_DMX_Pvr_Pid_Open/MApi_DMX_Pvr_Pid_Close
                if(ePvrSrcTSIf == DMX_TSIF_LIVE0)
                {
                    _DMX_FlowSet(DMX_FLOW_PVR, FlowInput, bClkInv, bExtSync, bParallel);  //set TSIF1 for CA input
                    DscmbPath = E_DRVTSP_CA_INPUT_TSIF1|E_DRVTSP_CA_OUTPUT_LIVE0|E_DRVTSP_CA_OUTPUT_TSIF2;
                    MDrv_TSP_Dscmb_Path(0, &DscmbPath, TRUE);
                    //ULOGD("DMX", "[%s][%d] DscmbPath 0x%lx \n", __FUNCTION__, __LINE__, DscmbPath);
                }
            }
        }
  #endif  //End of TSP_VER_1_0

        return DMX_FILTER_STATUS_OK;

  #if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0)
    }

    _DMX_ENTRY();

    MDrv_TSP_PVR_Eng_SelSrc((MS_U8)Eng, ePktDmx);

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

  #endif

#else  // For TSP_VER_4_0

    _DMX_ENTRY();

    if(_DMX_PVR_FlowSet(Eng, ePvrSrcTSIf, bDscmbRec) == FALSE)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#endif  // End of TSP_VER_4_0
}

//-------------------------------------------------------------------------------------------------
/// Open a demux filter
/// @param DmxFltType \b IN: the filter information to allocate
/// @param pu8DmxId \b OUT: the available demux filer Id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Open(DMX_FILTER_TYPE DmxFltType, MS_U32* pu32DmxId)
{
    _pdmx_res->_bIsDMXOpenCalled = TRUE;

    if(_MApi_DMX_Open_Common(DmxFltType,pu32DmxId) == UTOPIA_STATUS_SUCCESS)
    {
        return DMX_FILTER_STATUS_OK;
    }

    return DMX_FILTER_STATUS_ERROR;
}

DMX_FILTER_STATUS _MApi_DMX_Open_Ex(DMX_FILTER_TYPE DmxFltType, MS_U32* pu32DmxId)
{
    _pdmx_res->_bIsDMXOpenExCalled = TRUE;

    if(_MApi_DMX_Open_Common(DmxFltType,pu32DmxId) == UTOPIA_STATUS_SUCCESS)
    {
        return DMX_FILTER_STATUS_OK;
    }

    return DMX_FILTER_STATUS_ERROR;
}

DMX_FILTER_STATUS _MApi_DMX_Open_Common(DMX_FILTER_TYPE DmxFltType, MS_U32* pu32DmxId)
{
    DMX_FILTER_STATUS eRet= DMX_FILTER_STATUS_ERROR;
    MS_U32         u32TSPFltSource;
    u32TSPFltSource = DmxFltType & DMX_FILTER_FLT_MASK;
    DmxFltType = DmxFltType & (DMX_FILTER_TYPE)(~DMX_FILTER_FLT_MASK);

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));
    _DMX_OPEN_MIX_USED_CHECK();
    _DMX_ENTRY();
    switch (DmxFltType)
    {
        case DMX_FILTER_TYPE_VIDEO:
        case DMX_FILTER_TYPE_VIDEO3D:
#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
        case DMX_FILTER_TYPE_VIDEO3:
        case DMX_FILTER_TYPE_VIDEO4:
        case DMX_FILTER_TYPE_VIDEO5 ... DMX_FILTER_TYPE_VIDEO8:
#endif
        case DMX_FILTER_TYPE_AUDIO:
        case DMX_FILTER_TYPE_AUDIO2:
#if (!(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))) // For TSP_VER_3_0 and TSP_VER_4_0
        case DMX_FILTER_TYPE_AUDIO3:
        case DMX_FILTER_TYPE_AUDIO4:
#endif
#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
        case DMX_FILTER_TYPE_AUDIO5 ... DMX_FILTER_TYPE_AUDIO6:
#endif
#if(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
        case DMX_FILTER_TYPE_SCMBCHK:
#endif
        case DMX_FILTER_TYPE_REC:
        case DMX_FILTER_TYPE_PVR:
        case DMX_FILTER_TYPE_PVR1:
        case DMX_FILTER_TYPE_PVR2:
        case DMX_FILTER_TYPE_PVR3:
#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
        case DMX_FILTER_TYPE_PVR4 ... DMX_FILTER_TYPE_PVR9:
#endif
            eRet= _DMX_Open_Stream(pu32DmxId, (DMX_FILTER_TYPE)(DmxFltType|u32TSPFltSource));
            break;
        case DMX_FILTER_TYPE_SECTION:
        case DMX_FILTER_TYPE_TELETEXT:
        case DMX_FILTER_TYPE_PES:
        case DMX_FILTER_TYPE_PACKET:
        case DMX_FILTER_TYPE_PCR:
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
        case DMX_FILTER_TYPE_SECTION_NO_PUSI:
        case DMX_FILTER_TYPE_SECTION_VER:
#endif
            eRet= _DMX_Open_Sect(pu32DmxId, (DMX_FILTER_TYPE)(DmxFltType|u32TSPFltSource));
            break;

        default:
            DMX_ASSERT2(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad filter type %u\n", __LINE__, (unsigned int)DmxFltType)));
            break;
    }
#if DMX_DEBUG
    stDmxDbgInfo[(*pu32DmxId)].u32TimeCreate = MsOS_GetSystemTime();
    if(u32DbgLogFlag & DMX_DBG_LOG_FLT_OPEN)
    {
        if(eRet == DMX_FILTER_STATUS_OK)
            dmx_dbg_print("[DMX OPEN]\tFlt[%03d] Type[%s]\n", (int)(*pu32DmxId), stDmxType[DmxFltType].str);
        else
            dmx_dbg_print("[DMX OPEN]\tFailed\n");
    }
#endif

    //ULOGD("DMX", "[OPEN] %d \n", (int)*pu8DmxId);

    _DMX_RETURN(eRet);
}

extern MS_U32 _u32TaskLine;

//-------------------------------------------------------------------------------------------------
/// Close a demux filter
/// @param u8DmxId \b IN: the demux filer Id to free
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Close(MS_U32 u32DmxId)
{
    TSP_Result ret = (TSP_Result)DRVTSP_FAIL;
    MS_BOOL bPidFltReady = FALSE;
    MS_BOOL bPidReady = FALSE;
#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
    MS_BOOL bSecFltReady = FALSE;
#endif

#if DMX_DEBUG
    MS_BOOL bMonitor = FALSE;
#endif

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    DMX_ASSERT(-1!=_pdmx_res->_Dmx_Mutex, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%s][%06d] Demux has been close, close Demux Id %d fail\n",  __FUNCTION__, __LINE__, (int)u32DmxId)));
    DMX_ASSERT(DMX_MAX_FLTID > u32DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, (int)u32DmxId)));

#if DMX_DEBUG
    dmx_dbg_IsFltMonitor(u32DmxId, bMonitor);
    if((u32DbgLogFlag & DMX_DBG_LOG_FLT_CLOSE) && bMonitor)
        dmx_dbg_print("[DMX CLOSE]\tFlt[%03d]\n", (int)u32DmxId);
#endif
    do
    {
        _DMX_ENTRY();

        if(_pdmx_res->_FltList[u32DmxId].FltId == 0xFFFFFFFF)
        {
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[%s][%d] Filte %d is already freed\n", __FUNCTION__, __LINE__, (int)u32DmxId));
            _DMX_RETURN(DMX_FILTER_STATUS_OK);
        }

        if (FALSE == bPidReady)
        {
            if (DRVTSP_OK!= MDrv_TSP_PidFlt_SetPid(0, u32DmxId, DRVTSP_PID_NULL))
            {
                DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[%s][%d] Try to set pid failed,  Demux id: %d Filter Type: %d\n", __FUNCTION__, __LINE__, (int)u32DmxId, (unsigned int)_pdmx_res->_FltList[u32DmxId].DmxFltType));
                _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
            }
            _FLT_LIST_PID_SET(u32DmxId, DRVTSP_PID_NULL);
            bPidReady = TRUE;
        }

        if (FALSE == bPidFltReady)
        {
            if (DRVTSP_OK != MDrv_TSP_PidFlt_Free(0, u32DmxId))
            {
#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0))
                DMX_DBGMSG(DMX_DBG_WARN, ULOGE("DMX", "[%s][%d] Try to close filter %d fail -> TSP task line %d\n", __FUNCTION__, __LINE__, (int)u32DmxId, (int)_u32TaskLine));
#else
                extern MS_U32 MDrv_TSP_DBG_TaskLine(void);
                DMX_DBGMSG(DMX_DBG_WARN, ULOGE("DMX", "[%s][%d] Try to close filter %d fail -> TSP task line %d\n", __FUNCTION__, __LINE__, (int)u32DmxId, (int)MDrv_TSP_DBG_TaskLine()));
#endif
                goto release_mutex;
            }
            bPidFltReady = TRUE;
        }

#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
        if (FALSE == bSecFltReady)
        {
            if (0xFFFFFFFF != _pdmx_res->_FltList[u32DmxId].SecBufId)
            {
                if (DRVTSP_OK!= MDrv_TSP_SecFlt_Free(0, _pdmx_res->_FltList[u32DmxId].SecBufId))
                {
                    goto release_mutex;
                }
                bSecFltReady = TRUE;
            }
        }
#endif

        _FLT_LIST_REMOVE(u32DmxId);

        ret = (TSP_Result)DRVTSP_OK;
release_mutex:
        MsOS_ReleaseMutex(_pdmx_res->_Dmx_Mutex);
        if ((TSP_Result)DRVTSP_OK != ret)
        {
            return DMX_FILTER_STATUS_ERROR;
            //MsOS_DelayTask(4);
        }
    } while (DRVTSP_OK != ret);

    //ULOGD("DMX", "[CLOSE] %d \n", (int)u8DmxId);

    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Activate a demux filter
/// @param u8DmxId \b IN: the demux filer Id to activate
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Start(MS_U32 u32DmxId)
{

#if DMX_DEBUG
    MS_BOOL bMonitor = FALSE;
#endif

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    DMX_ASSERT(DMX_MAX_FLTID> u32DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, (int)u32DmxId)));

#if DMX_DEBUG
    dmx_dbg_IsFltMonitor(u32DmxId, bMonitor);
    if((u32DbgLogFlag & DMX_DBG_LOG_FLT_ENABLE) && bMonitor)
        dmx_dbg_print("[DMX ENABLE]\tFlt[%03d]\n", (MS_S32)u32DmxId);
#endif

    _DMX_ENTRY();

    MDrv_TSP_PidFlt_Enable(0, u32DmxId, TRUE);

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Deactivate a demux filter
/// @param u8DmxId \b IN: the demux filer Id to deactivate
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Stop(MS_U32 u32DmxId)
{
#if DMX_DEBUG
    MS_BOOL bMonitor = FALSE;
#endif

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    DMX_ASSERT((DMX_MAX_FLTID > u32DmxId), DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, (int)u32DmxId)));

#if DMX_DEBUG
    dmx_dbg_IsFltMonitor(u32DmxId, bMonitor);
    if((u32DbgLogFlag & DMX_DBG_LOG_FLT_ENABLE) && bMonitor)
        dmx_dbg_print("[DMX DISABLE]\tFlt[%03d]\n", u32DmxId;
#endif

    _DMX_ENTRY();

    MDrv_TSP_PidFlt_Enable(0, u32DmxId, FALSE);

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get a demux filter status
/// @param u32DmxId \b IN: the demux filer Id to activate
/// @param pbEnable \b OUT: the demux filer enable/disable
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_IsStart(MS_U32 u32DmxId, MS_BOOL* pbEnable)
{
#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0))

    DrvTSP_FltState ePidState = E_DRVTSP_FLT_STATE_FREE;
    DrvTSP_FltState eSecState = E_DRVTSP_FLT_STATE_FREE;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));
    DMX_ASSERT(DMX_MAX_FLTID> u32DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, (int)u32DmxId)));

    if ((DRVTSP_OK != MDrv_TSP_PidFlt_GetState(0, u32DmxId, &ePidState)))
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    if(_pdmx_res->_FltList[u32DmxId].SecBufId < 0xFFFFFFFF)
    {
        if ((DRVTSP_OK != MDrv_TSP_SecFlt_GetState(0, _pdmx_res->_FltList[u32DmxId].SecBufId, &eSecState)))
        {
            return DMX_FILTER_STATUS_ERROR;
        }
    }
    *pbEnable = ((ePidState|eSecState) & E_DRVTSP_FLT_STATE_ENABLE)? TRUE: FALSE;

#else  // For TSP_VER_4_0

    TSP_FltState    state;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));
    DMX_ASSERT(DMX_MAX_FLTID> u32DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u32DmxId)));

    if ( DRVTSP_OK != MDrv_TSP_FLT_GetState(_pdmx_res->_FltList[u32DmxId].FltId, &state) )
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    *pbEnable = (state & E_TSP_FLT_STATE_ENABLE) ? TRUE : FALSE;

#endif

    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get PID filter scramble status
/// @param  FltSrc                 \b IN     : Filter source
/// @param  u32FltGroupId     \b IN     : Pid filter group id, every 32 filter for 1 group. 0: filter 0~31, 1: 32~63, 2: 64~95, 3: 96~127
/// @param  PidFltId               \b IN     : Pid filter id for getting. If 0xFFFFFFFF, means getting the status of all filters of the same group
/// @param  pu32ScmbSts      \b OUT    : The scrmabling status of pid filter. If getting all group status, every bit means one status of every filter
///                                       If only checking one filter,  1 means scrambled status and 0 means non-scrambled status
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Get_FltScmbSts(DMX_FILTER_TYPE FltSrc, MS_U32 u32FltGroupId, MS_U32 PidFltId, MS_U32 *pu32ScmbSts)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    MS_U32            u32TSPFltSource;
    MS_U32            u32GroupId;

    u32TSPFltSource = FltSrc & ((MS_U32)DMX_FILTER_FLT_MASK);

    if( PidFltId == 0xFFFFFFFF)
    {
        u32GroupId = u32FltGroupId;
    }
    else
    {
        u32GroupId = PidFltId >> 5;
    }

#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
    u32TSPFltSource = _DMX_ApiDrv_FltSrcMapping(u32TSPFltSource);
#endif

    if(MDrv_TSP_PidFlt_GetScmbSts( (DrvTSP_FltType)u32TSPFltSource, u32GroupId, PidFltId, pu32ScmbSts) == DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }

    return DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Get the PES scrambling control status
/// @param u32DmxId \b IN: the demux filer Id
/// @param pu8scmb \b OUT: the PES scrambling status. bit 0~ bit3, Every bit indicates one status of one packet. bit4 ~ bit7, packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
/// Must open and start filter with DMX_FILTER_TYPE_PACKET type and DMX_SECT_MODE_PESSCMCHK mode
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Get_PesScmbSts(MS_U32 u32DmxId, MS_U8* pu8scmb)
{
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    if (DRVTSP_OK == MDrv_TSP_GetPesScmbSts(u32DmxId, pu8scmb))
    {
        return DMX_FILTER_STATUS_OK;
    }
#endif

    return DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Get the TS scrambling control status
/// @param u8DmxId \b IN: the demux filer Id
/// @param pu8ScmSts \b OUT: TS scrambling status. bit 0~ bit3, Every bit indicates one status of one packet. bit4 ~ bit7, packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
/// Must open and start filter with DMX_FILTER_TYPE_PACKET type and DMX_SECT_MODE_PESSCMCHK mode
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Get_TsScmbSts(MS_U32 u32DmxId, MS_U8* pu8ScmSts)
{
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    if (DRVTSP_OK != MDrv_TSP_GetTsScmbSts(u32DmxId, pu8ScmSts))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif

    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get demux alive
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_ChkAlive(void)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    if (MDrv_TSP_Alive(0) == DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }
    return DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Get a demux filter information
/// @param u8DmxId \b IN: the demux filer Id to activate
/// @param pDmxFltInfo \b OUT: the demux filer info
/// @param pDmxFltType \b OUT: the demux filer type
/// @param bSet \b IN: set/get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Info(MS_U32 u32DmxId, DMX_Flt_info* pDmxFltInfo, DMX_FILTER_TYPE* pDmxFltType, MS_BOOL bSet)
{
    _DMX_INFO_MIX_USED_CHECK(FALSE);
    MS_BOOL bRet = FALSE;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));
    DMX_ASSERT(DMX_MAX_FLTID> u32DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, (int)u32DmxId)));

    _DMX_ENTRY();

    if (!bSet)
    {
        _FLT_LIST_INFO_GET(u32DmxId, pDmxFltInfo);
        _FLT_LIST_TYPE_GET(u32DmxId, pDmxFltType);
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }

    //ULOGD("DMX", "[%s] DmxID [%d]\n",__FUNCTION__, u8DmxId);
    //ULOGD("DMX", "[%s] FltType [%d]\n",__FUNCTION__, _pdmx_res->_FltList[u8DmxId].DmxFltType);
    //ULOGD("DMX", "[%s] callback [%d]\n",__FUNCTION__, (MS_U32)(pDmxFltInfo->Info.SectInfo.pNotify));

    switch(_pdmx_res->_FltList[u32DmxId].DmxFltType)
    {
        case DMX_FILTER_TYPE_VIDEO:
        case DMX_FILTER_TYPE_VIDEO3D:
#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
        case DMX_FILTER_TYPE_VIDEO3:
        case DMX_FILTER_TYPE_VIDEO4:
        case DMX_FILTER_TYPE_VIDEO5 ... DMX_FILTER_TYPE_VIDEO8:
#endif
        case DMX_FILTER_TYPE_AUDIO:
        case DMX_FILTER_TYPE_AUDIO2:
#if (!(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0)))  // For TSP_VER_3_0 and TSP_VER_4_0
        case DMX_FILTER_TYPE_AUDIO3:
        case DMX_FILTER_TYPE_AUDIO4:
#endif
#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
        case DMX_FILTER_TYPE_AUDIO5 ... DMX_FILTER_TYPE_AUDIO6:
#endif
            bRet= _DMX_InfoSet_Stream(u32DmxId, pDmxFltInfo);
            break;
        case DMX_FILTER_TYPE_SECTION:
        case DMX_FILTER_TYPE_TELETEXT:
        case DMX_FILTER_TYPE_PES:
        case DMX_FILTER_TYPE_PACKET:
        case DMX_FILTER_TYPE_PCR:
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
        case DMX_FILTER_TYPE_SECTION_NO_PUSI:
        case DMX_FILTER_TYPE_SECTION_VER:
#endif

#if DMX_DEBUG
        if(u32DbgLogFlag & DMX_DBG_LOG_FLT_INFO)
        {
            DMX_Sect_info* pInfo = &(pDmxFltInfo->Info.SectInfo);

            dmx_dbg_print("[DMX INFO]\tFlt[%03d] addr[0x%x] size[0x%x] secMode[0x%x] event[0x%x]\n",
                            u32DmxId,
                            (MS_U32)pInfo->SectBufAddr,
                            pInfo->SectBufSize,
                            (MS_U32)pInfo->SectMode,
                            (MS_U32)pInfo->Event);
        }
#endif
       {
            DMX_InfoSet stDmxFltInfo;
            memset(&(stDmxFltInfo),0,sizeof(DMX_InfoSet));
            stDmxFltInfo.bEx = FALSE;
            stDmxFltInfo.u32InfoSetSize = sizeof(DMX_InfoSet);
            stDmxFltInfo.DmxFltType = *pDmxFltType;
            memcpy(&(stDmxFltInfo.DmxInfo.SectInfo), &(pDmxFltInfo->Info.SectInfo), sizeof(DMX_Sect_info));
            bRet= _DMX_InfoSet_Sect(u32DmxId, &stDmxFltInfo);

        }
        break;

        default:
            DMX_ASSERT2(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad filter type %d\n", __LINE__, (unsigned int)_pdmx_res->_FltList[u32DmxId].DmxFltType)));
            break;
    }

    if (bRet)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
}

DMX_FILTER_STATUS _MApi_DMX_Info_Ex(MS_U32 u32DmxId, DMX_Flt_info_Ex* pDmxFltInfo)
{
    _DMX_INFO_MIX_USED_CHECK(TRUE);
    MS_BOOL bRet = FALSE;
    ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__);

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));
    DMX_ASSERT(DMX_MAX_FLTID> u32DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, (int)u32DmxId)));

    _DMX_ENTRY();

    if (!pDmxFltInfo->bSet)
    {
        _FLT_LIST_INFO_GET_Ex(u32DmxId, pDmxFltInfo);
        _FLT_LIST_TYPE_GET(u32DmxId, &(pDmxFltInfo->DmxFltType));
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }

    //ULOGD("DMX", "[%s] FltType [%lx]\n",__FUNCTION__, (pDmxFltInfo->DmxFltType));
    //ULOGD("DMX", "[%s] event [%lx]\n",__FUNCTION__, (MS_U32)(pDmxFltInfo->SectInfoEx.Event));
    //ULOGD("DMX", "[%s] mode [%d]\n",__FUNCTION__, (MS_U32)(pDmxFltInfo->SectInfoEx.SectMode));

    switch(_pdmx_res->_FltList[u32DmxId].DmxFltType)
    {
        case DMX_FILTER_TYPE_SECTION:
        case DMX_FILTER_TYPE_TELETEXT:
        case DMX_FILTER_TYPE_PES:
        case DMX_FILTER_TYPE_PACKET:
        case DMX_FILTER_TYPE_PCR:
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
        case DMX_FILTER_TYPE_SECTION_NO_PUSI:
        case DMX_FILTER_TYPE_SECTION_VER:
#endif

        {
            DMX_InfoSet stDmxFltInfo;
            memset(&(stDmxFltInfo),0,sizeof(DMX_InfoSet));
            stDmxFltInfo.bEx = TRUE;
            stDmxFltInfo.u32InfoSetSize = sizeof(DMX_InfoSet);
            stDmxFltInfo.DmxFltType = pDmxFltInfo->DmxFltType;
            memcpy(&(stDmxFltInfo.DmxInfo.SectInfoEx), &(pDmxFltInfo->SectInfoEx), sizeof(DMX_Sect_info_Ex));
            bRet= _DMX_InfoSet_Sect(u32DmxId, &stDmxFltInfo);
            break;
        }
        default:
            DMX_ASSERT2(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad filter type %d\n", __LINE__, (unsigned int)_pdmx_res->_FltList[u32DmxId].DmxFltType)));
            break;
    }

    if (bRet)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
}

//-------------------------------------------------------------------------------------------------
/// Set a demux filter PID
/// @param u32DmxId \b IN: the demux filer Id to set
/// @param pu16Pid \b IN/OUT: PID to set
/// @param bSet \b IN: set/get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pid(MS_U32 u32DmxId, MS_U16* pu16Pid, MS_BOOL bSet)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));
    DMX_ASSERT(DMX_MAX_FLTID > u32DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, (unsigned int)u32DmxId)));

    if (bSet)
    {
#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FLT_PID)
        dmx_dbg_print("[DMX PID]\tFlt[%03d] PID[0x%X]\n", (MS_S32)u32DmxId, (MS_U32)(*pu16Pid));
#endif
        //ULOGD("DMX",  ("[%s] fltid: [%d], pid : [0x%x] time %ld\n",__FUNCTION__,(int)u8DmxId, *pu16Pid, MsOS_GetSystemTime());
        MDrv_TSP_PidFlt_SetPid(0, u32DmxId, *pu16Pid);
        _FLT_LIST_PID_SET(u32DmxId, *pu16Pid);
    }
    else
    {
        _FLT_LIST_PID_GET(u32DmxId, *pu16Pid);
    }
    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set demux filer match patterns
/// @param u8DmxId \b IN: the target demux filer Id
/// @param pPattern \b IN: the match patterns
/// @param pMask \b IN: the bit mask for match patterns
/// @param pu8NotMask \b IN: the bit mask for negative match patterns
/// @param MatchSize \b IN: the size in bytes for match patterns.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
/// The match pattern size is 16 bytes
///     match mask -- must set 1 to be compare (customer request)
///     not match mask -- must set 1 to compare
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SectPatternSet(MS_U32 u32DmxId, MS_U8* pPattern, MS_U8* pMask, MS_U8 *pu8NotMask, MS_U32 MatchSize)
{
    MS_U8 Pattern[DRVTSP_FILTER_DEPTH], Mask[DRVTSP_FILTER_DEPTH], NMatchMask[DRVTSP_FILTER_DEPTH];

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    DMX_ASSERT(DMX_MAX_FLTID > u32DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, (int)u32DmxId)));
    DMX_ASSERT(0xFFFFFFFF > _pdmx_res->_FltList[u32DmxId].SecBufId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Demux id has not been opened yet %d\n", __LINE__, (int)u32DmxId)));
    DMX_ASSERT(DRVTSP_FILTER_DEPTH>= MatchSize,
               DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Section filter match size\n", __LINE__)));

    _DMX_ENTRY();

    memset(Pattern, 0, DRVTSP_FILTER_DEPTH);
    memset(Mask, 0, DRVTSP_FILTER_DEPTH);
    memset(NMatchMask, 0, DRVTSP_FILTER_DEPTH);
    memcpy(Pattern, pPattern, MatchSize);
    memcpy(Mask, pMask, MatchSize);
    memcpy(NMatchMask, pu8NotMask, MatchSize);
    MDrv_TSP_SecFlt_SetPattern(0, _pdmx_res->_FltList[u32DmxId].SecBufId, Pattern, Mask, NMatchMask);
    _DMX_RETURN (DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Reset demux filer buffer
/// @param u8DmxId \b IN: the target demux filer Id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SectReset(MS_U32 u32DmxId)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));
    _DMX_ENTRY();
    DMX_ASSERT2(DMX_MAX_FLTID > u32DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, (int)u32DmxId)));
    DMX_ASSERT2(0xFFFFFFFF > _pdmx_res->_FltList[u32DmxId].SecBufId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] No section buffer, Demux Id %d\n", __LINE__, (int)u32DmxId)));
    MDrv_TSP_SecFlt_ResetBuffer(0, _pdmx_res->_FltList[u32DmxId].SecBufId);
    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set the read pointer of demux filer buffer
/// @param u8DmxId \b IN: the target demux filer Id
/// @param Read \b IN: the read pointer to be set
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SectReadSet(MS_U32 u32DmxId, MS_PHY Read)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();
    DMX_ASSERT2(DMX_MAX_FLTID > u32DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGD("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, (int)u32DmxId)));
    MDrv_TSP_SecFlt_SetReadAddr(0, _pdmx_res->_FltList[u32DmxId].SecBufId, Read);
    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get the read pointer of demux filer buffer
/// @param u8DmxId \b IN: the target demux filer Id
/// @param pRead \b OUT: the pointer to store the obtained read pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SectReadGet(MS_U32 u32DmxId, MS_PHY* pRead)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    DMX_ASSERT(DMX_MAX_FLTID > u32DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, (int)u32DmxId)));

    if(_pdmx_res->_FltList[u32DmxId].FltId == 0xFFFFFFFF)
    {
        //DMX_DBGMSG(DMX_DBG_ERR, ULOGD("DMX", "[%s][%d] Filte %d is already freed\n", __FUNCTION__, __LINE__, (int)u8DmxId));
        return DMX_FILTER_STATUS_ERROR;
    }

    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetReadAddr(0, _pdmx_res->_FltList[u32DmxId].SecBufId, pRead))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get the write pointer of demux filer buffer
/// @param u8DmxId \b IN: the target demux filer Id
/// @param pWrite \b OUT: the pointer to store the obtained write pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SectWriteGet(MS_U32 u32DmxId, MS_PHY* pWrite)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    DMX_ASSERT(DMX_MAX_FLTID > u32DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, (int)u32DmxId)));

    if(_pdmx_res->_FltList[u32DmxId].FltId == 0xFFFFFFFF)
    {
        //DMX_DBGMSG(DMX_DBG_ERR, ULOGD("DMX", "[%s][%d] Filte %d is already freed\n", __FUNCTION__, __LINE__, (int)u8DmxId));
        return DMX_FILTER_STATUS_ERROR;
    }

    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetWriteAddr(0, _pdmx_res->_FltList[u32DmxId].SecBufId, pWrite))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get the start pointer of demux filer buffer
/// @param u8DmxId \b IN: the target demux filer Id
/// @param pStart \b OUT: the pointer to store the obtained start pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SectStartGet(MS_U32 u32DmxId, MS_PHY* pStart)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));
    DMX_ASSERT(DMX_MAX_FLTID > u32DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, (int)u32DmxId)));

    if(_pdmx_res->_FltList[u32DmxId].FltId == 0xFFFFFFFF)
    {
        //DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[%s][%d] Filte %d is already freed\n", __FUNCTION__, __LINE__, (int)u8DmxId));
        return DMX_FILTER_STATUS_ERROR;
    }

    return (DRVTSP_OK == MDrv_TSP_SecFlt_GetBufStart(0, _pdmx_res->_FltList[u32DmxId].SecBufId, pStart))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Get the end pointer of demux filer buffer
/// @param u8DmxId \b IN: the target demux filer Id
/// @param pEnd \b OUT: the pointer to store the obtained end pointer
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SectEndGet(MS_U32 u32DmxId, MS_PHY* pEnd)
{
    MS_U32 u32Size = 0;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));
    DMX_ASSERT(DMX_MAX_FLTID > u32DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, (int)u32DmxId)));

    if(_pdmx_res->_FltList[u32DmxId].FltId == 0xFFFFFFFF)
    {
        //DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[%s][%d] Filte %d is already freed\n", __FUNCTION__, __LINE__, (int)u8DmxId));
        return DMX_FILTER_STATUS_ERROR;
    }

    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetBufStart(0, _pdmx_res->_FltList[u32DmxId].SecBufId, pEnd))
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    if (DRVTSP_OK != MDrv_TSP_SecFlt_GetBufSize(0, _pdmx_res->_FltList[u32DmxId].SecBufId, &u32Size))
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    *pEnd += u32Size;
    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// For setting MIU address for TSP aeon
/// @param phyDataAddr       \b IN: MIU phsyical address
/// @param u32size               \b IN: MIU buffer size
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SetFwDataAddr(MS_PHY phyDataAddr, MS_U32 u32size)
{
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
    _DMX_ENTRY();
    MDrv_TSP_SetFwDataAddr(phyDataAddr, u32size);
    _DMX_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

// special case for TTX
DMX_FILTER_STATUS _MApi_DMX_TTX_WriteGet(MS_U32 u32DmxId, MS_PHY* pWrite)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));
    DMX_ASSERT(DMX_MAX_FLTID > u32DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, (int)u32DmxId)));
    if(DRVTSP_OK != MDrv_TSP_TTX_SecFlt_GetWriteAddr(0, _pdmx_res->_FltList[u32DmxId].SecBufId, pWrite))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    else
    {
        return DMX_FILTER_STATUS_OK;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set playback time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pvr_SetPlaybackStamp(MS_U32 u32Stamp)
{
    DMX_PVRDBGMSG(DMX_DBG_PVR, ULOGD("DMX", "[%s] %d (%u)\n", __FUNCTION__, __LINE__, (unsigned int)u32Stamp));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FILEIN);
        dmx_dbg_print("[DMX FileIn timestamp]\t stamp[0x%08X]\n", u32Stamp);
#endif

    return (DRVTSP_OK== MDrv_TSP_PVR_TimeStampSetPlaybackStamp(u32Stamp))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Get playback time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pvr_GetPlaybackStamp(MS_U32* pu32Stamp)
{
    DMX_PVRDBGMSG(DMX_DBG_PVR, ULOGD("DMX", "[%s] %d (0x%p)\n", __FUNCTION__, __LINE__, pu32Stamp));

    return (DRVTSP_OK== MDrv_TSP_PVR_TimeStampGetPlaybackStamp(pu32Stamp))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Enable recording time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pvr_TimeStampEnable(void)
{
    DMX_PVRDBGMSG(DMX_DBG_PVR, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    if(MDrv_TSP_PVR_TimeStamp(TRUE) == DRVTSP_OK)
    {
#ifdef DMX_RESET_FI_TIMESTAMP
        _pdmx_res->_bPlayTimestampEnable[0] = TRUE;
#endif

        return DMX_FILTER_STATUS_OK;
    }

    return DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Disable recording time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pvr_TimeStampDisable(void)
{
    DMX_PVRDBGMSG(DMX_DBG_PVR, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    if(MDrv_TSP_PVR_TimeStamp(FALSE) == DRVTSP_OK)
    {
#ifdef DMX_RESET_FI_TIMESTAMP
        _pdmx_res->_bPlayTimestampEnable[0] = FALSE;
#endif

        return DMX_FILTER_STATUS_OK;
    }

    return DMX_FILTER_STATUS_ERROR;
}

//--------------------------------------------------------------------------------------
//New interface for multiple PVR engine
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Open(DMX_PVR_ENG Eng ,DMX_Pvr_info* pPvrInfo)
{
    DMX_PVR_ASSERT(!((MS_U32)pPvrInfo->pPvrBuf0 & 0xF),
               DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] PVR buffer must be 16 bytes alignment 0x%08x\n", __LINE__, (unsigned int)pPvrInfo->pPvrBuf0)));
    DMX_PVR_ASSERT(!((MS_U32)pPvrInfo->pPvrBuf1 & 0xF),
               DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] PVR buffer must be 16 bytes alignment 0x%08x\n", __LINE__, (unsigned int)pPvrInfo->pPvrBuf1)));
    DMX_PVR_ASSERT(!((MS_U32)pPvrInfo->PvrBufSize0 & 0xF),
               DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] PVR buffer size must be 16 bytes alignment 0x%08x\n", __LINE__, (unsigned int)pPvrInfo->PvrBufSize0)));
    DMX_PVR_ASSERT(!((MS_U32)pPvrInfo->PvrBufSize1 & 0xF),
               DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] PVR buffer size must be 16 bytes alignment 0x%08x\n", __LINE__, (unsigned int)pPvrInfo->PvrBufSize1)));

    if(Eng == DMX_PVR_TSO0)
    {
#ifdef TSO_PVR_SUPPORT
        _DMX_TSO_ENTRY();
        if(MDrv_TSO_PVR_SetBuf(0, pPvrInfo->pPvrBuf0, pPvrInfo->pPvrBuf1, pPvrInfo->PvrBufSize0, (MS_U32)pPvrInfo->PvrBufSize1) == DRVTSO_OK)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
        }
        else
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }
#else
        return DMX_FILTER_STATUS_ERROR;
#endif
    }

    _DMX_PVR_ENTRY();

#ifdef SECURE_PVR_ENABLE
    _pdmx_res->_stSecBuf.u32BufOpt = 0;
    _pdmx_res->_stSecBuf.u32BufId = ((MS_U32)Eng) & 0xFF;
    _pdmx_res->_stSecBuf.phyBufAddr = pPvrInfo->pPvrBuf0;
    _pdmx_res->_stSecBuf.u32BufSize = pPvrInfo->PvrBufSize0 + pPvrInfo->PvrBufSize1;
    if(MDrv_TSP_Ree_SendTeeCmd(E_DRVTSP_REE_TO_TEE_CMD_SET_PvrBuf, (void*)&(_pdmx_res->_stSecBuf), sizeof(DrvTSP_SecureBuf)) != DRVTSP_OK)
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    else
    {
        pPvrInfo->pPvrBuf0 = _pdmx_res->_stSecBuf.phyBufAddr;
        pPvrInfo->PvrBufSize0 = _pdmx_res->_stSecBuf.u32BufSize >> 1;
        pPvrInfo->pPvrBuf1 = pPvrInfo->pPvrBuf0 + pPvrInfo->PvrBufSize0;
        pPvrInfo->PvrBufSize1 = pPvrInfo->PvrBufSize0;
    }
#endif

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

  #if(defined(VQ_ENABLE) && defined(TS2_IF_SUPPORT) && (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0)))
    #ifndef FIVQ_ENABLE
    if((Eng == DMX_PVR_EGN1) && _pdmx_res->_bVQEnabled)
    {
        MDrv_TSP_VQueEnable(FALSE);
    }
    #endif
  #endif //VQ_ENABLE && TS2_IF_SUPPORT

    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_SetBuffer(Eng, (pPvrInfo->pPvrBuf0), (pPvrInfo->pPvrBuf1),
        (MS_U32)pPvrInfo->PvrBufSize0, (MS_U32)pPvrInfo->PvrBufSize1))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _pdmx_res->_PvrNotify[Eng] = pPvrInfo->pNotify;

    if (pPvrInfo->pNotify)
    {
        MDrv_TSP_PVR_Eng_Notify(Eng, (DrvTSP_Event)pPvrInfo->Event, _DMX_NotifyPvr);
    }
#else  // For TSP_VER_4_0

    MS_U32  u32PvrEng = 0;

    if(_DMX_ApiDrv_PvrEngMapping(Eng, &u32PvrEng) == FALSE)
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (DRVTSP_OK != MDrv_TSP_PVR_Eng_SetBuffer(u32PvrEng, pPvrInfo->pPvrBuf0, pPvrInfo->pPvrBuf1, (MS_U32)pPvrInfo->PvrBufSize0, (MS_U32)pPvrInfo->PvrBufSize1))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    if (pPvrInfo->pNotify)
    {
        _pdmx_res->_PvrNotify[Eng] = pPvrInfo->pNotify;
        MDrv_TSP_PVR_Eng_Notify(u32PvrEng, (DrvTSP_Event)pPvrInfo->Event, _DMX_NotifyPvr);
    }

#endif  // End of TSP_VER_4_0

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Close(DMX_PVR_ENG Eng)
{
    if(Eng == DMX_PVR_TSO0)
    {
#ifdef TSO_PVR_SUPPORT
        _DMX_TSO_ENTRY();
        if(MDrv_TSO_PVR_SetBuf(Eng, 0, 0, 0, 0) == DRVTSO_OK)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
        }
        else
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }
#else
        return DMX_FILTER_STATUS_ERROR;
#endif
    }

    _DMX_PVR_ENTRY();

#if ((defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))) && defined(SECURE_PVR_ENABLE))
    _pdmx_res->_stSecBuf.u32BufId = (MS_U32)Eng;
    _pdmx_res->_stSecBuf.u32BufOpt = 1; //clear buffer setting
    _pdmx_res->_stSecBuf.u32BufSize = 0;
    _pdmx_res->_stSecBuf.phyBufAddr = 0;
    if(MDrv_TSP_Ree_SendTeeCmd(E_DRVTSP_REE_TO_TEE_CMD_SET_PvrBuf, (void*)&(_pdmx_res->_stSecBuf), sizeof(DrvTSP_SecureBuf)) != DRVTSP_OK)
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#endif

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    MDrv_TSP_PVRCA_Close(Eng);

  #ifndef SECURE_PVR_ENABLE
    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_SetBuffer(Eng, 0,0,0,0))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
  #endif

  #if(defined(VQ_ENABLE) && defined(TS2_IF_SUPPORT) && (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0)))
    #ifndef FIVQ_ENABLE
    if((Eng == 1) && _pdmx_res->_bVQEnabled)
    {
        MApi_DMX_VQ_Enable(TRUE);
    }
    #endif
  #endif //VQ_ENABLE && TS2_IF_SUPPORT

#else  // For TSP_VER_4_0

  #ifndef SECURE_PVR_ENABLE
    MS_U32  u32PvrEng = 0;

    if(_DMX_ApiDrv_PvrEngMapping(Eng, &u32PvrEng) == FALSE)
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_SetBuffer(u32PvrEng, 0,0,0,0))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
  #endif
#endif  // End of TSP_VER_4_0

    _pdmx_res->_PvrNotify[Eng] = NULL;

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Stop/Resume recording
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Pause(DMX_PVR_ENG Eng ,MS_BOOL bPause)
{
    DMX_PVRDBGMSG(DMX_DBG_PVR, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_PVR_ENTRY();

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
    ULOGD("DMX", "[%s][%d] Not implement Yet \n",__FUNCTION__,__LINE__);
#else
    MS_U32  u32PvrEng = 0;

    if(_DMX_ApiDrv_PvrEngMapping(Eng, &u32PvrEng) == FALSE)
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    MDrv_TSP_PVR_Eng_Pause(u32PvrEng, bPause);
#endif

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
}

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Pid_Open(DMX_PVR_ENG Eng ,MS_U32 Pid, MS_U32* pu32DmxId)
{
    MS_U32 FltId = 0;

    DMX_ASSERT(DMX_PVR_TSO0 > Eng, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Invalid Engine ID!\n", __LINE__)));

    DrvTSP_FltType ftype = E_DRVTSP_FLT_SOURCE_TYPE_LIVE;
    DMX_PVRDBGMSG(DMX_DBG_PVR, ULOGD("DMX", "[%s] %d (%d, 0x%x, %lx\n", __FUNCTION__, __LINE__, (int)Eng, (unsigned int)Pid, (unsigned long)pu32DmxId));

    _DMX_PVR_ENTRY();

  #if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0)
    switch(Eng)
    {
        case DMX_PVR_EGN0 :
            ftype = E_DRVTSP_FLT_TYPE_PVR;
            break;
        case DMX_PVR_EGN1 :
            ftype = E_DRVTSP_FLT_TYPE_PVR1;
            break;
        default:
            _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    switch((DrvTSP_PKTDMXSrcType)(_pdmx_res->_u32PVREngSrc[(MS_U8)Eng]))
    {
        case E_DRVTSP_PKTSRC_DEMUX0:
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_LIVE;
            break;
        case E_DRVTSP_PKTSRC_DEMUXFILE:
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_FILE;
            break;
        case E_DRVTSP_PKTSRC_DEMUX1:
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_TS1;
            break;
        case E_DRVTSP_PKTSRC_DEMUX2:
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_TS2;
            break;
        default:
            break;
    }

    if(_pdmx_res->_u32PVREngSrc[(MS_U8)Eng] == 0)
    {
        if(Eng == DMX_PVR_EGN0)
        {
            _pdmx_res->_u32PVREngSrc[0] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX0;
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_LIVE;
        }
        else if(Eng == DMX_PVR_EGN1)
        {
            _pdmx_res->_u32PVREngSrc[1] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX1;
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_TS1;
        }
    }

  #else // For TSP_VER_1_0

    switch(Eng)
    {
        case DMX_PVR_EGN0 :
            ftype |= E_DRVTSP_FLT_TYPE_PVR;
            break;
        #ifdef TS2_IF_SUPPORT
        case DMX_PVR_EGN1 :
            ftype |= E_DRVTSP_FLT_TYPE_CB;
            break;
        #endif
        default:
            _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
  #endif

    if (DRVTSP_OK!= MDrv_TSP_PidFlt_Alloc(_Owner, ftype, &FltId))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    DMX_PVR_ASSERT2(FltId< DMX_MAX_FLTID, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad FltId %u\n", __LINE__, (unsigned int)FltId)));
    MDrv_TSP_PidFlt_SetPid(0, FltId, Pid);
    _FLT_LIST_PID_SET((MS_U8)FltId, (MS_U16)Pid);
    MDrv_TSP_PidFlt_Enable(0, FltId, TRUE);
    _FLT_LIST_SECFLT_TYPE_SET(FltId, FltId, 0xFFFFFFFF, (DMX_FILTER_TYPE)0xFF);
    *pu32DmxId =  FltId;

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);

}  // End of TSP_VER_1_0 & TSP_VER_3_0

#else  // For TSP_VER_4_0

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Pid_Open(DMX_PVR_ENG Eng ,MS_U32 Pid, MS_U32* pu32DmxId)
{
    MS_U32 FltId = 0xFFFFFFFFUL;
    DMX_PVRDBGMSG(DMX_DBG_PVR, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));
    _DMX_PVR_ENTRY();

    MS_U32  u32PvrEng = 0;

    if(_DMX_ApiDrv_PvrEngMapping(Eng, &u32PvrEng) == FALSE)
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (DRVTSP_OK != MDrv_TSP_PVR_Eng_FltAlloc(u32PvrEng, &FltId)) // Kaiser use tsp pidflt as PVR flt
    {
        DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Alloc Flt failed\n", __LINE__))
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
     _FLT_LIST_SECFLT_TYPE_SET(FltId, FltId, 0xFF, DMX_FILTER_TYPE_REC);

    //enable
    if (DRVTSP_OK != MDrv_TSP_PVR_Eng_FltEnable(u32PvrEng, FltId, TRUE))
    {
        DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Alloc Flt failed\n", __LINE__))
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    //set pid
    if (DRVTSP_OK != MDrv_TSP_PVR_Eng_FltSetPID(u32PvrEng, FltId, Pid)) //@FIXME check return value
    {
        DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Alloc Flt failed\n", __LINE__))
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _FLT_LIST_PID_SET(FltId, Pid);
    *pu32DmxId =  FltId;

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
}
#endif  // End of TSP_VER_4_0

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Pid_Close(DMX_PVR_ENG Eng ,MS_U32 u32DmxId)
{
    DMX_PVRDBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d (%d)\n", __FUNCTION__, __LINE__, (int)u32DmxId));
    DMX_ASSERT(DMX_PVR_TSO0 > Eng, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Invalid Engine ID!\n", __LINE__)));
    DMX_ASSERT(DMX_MAX_FLTID > u32DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, (unsigned int)u32DmxId)));

    _DMX_PVR_ENTRY();

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
    MDrv_TSP_PidFlt_Enable(0, u32DmxId, FALSE);
    MDrv_TSP_PidFlt_SetPid(0, u32DmxId, DRVTSP_PID_NULL);
    _FLT_LIST_PID_SET(u32DmxId, DRVTSP_PID_NULL);
    MDrv_TSP_PidFlt_Free(0, u32DmxId);
    _FLT_LIST_REMOVE(u32DmxId);
#else   // For TSP_VER_4_0
    MS_U32  u32PvrEng = 0;

    if(_DMX_ApiDrv_PvrEngMapping(Eng, &u32PvrEng) == FALSE)
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    MDrv_TSP_PVR_Eng_FltFree(u32PvrEng, u32DmxId);
    _FLT_LIST_REMOVE(u32DmxId);
#endif

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Check if PVR engine is started
/// @param Eng                                   \b IN: PVR engine ID
/// @param pbIsStart                           \b OUT: Pointer to store PVR engine starting status
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_IsStart(DMX_PVR_ENG Eng, MS_BOOL *pbIsStart)
{
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    _DMX_PVR_ENTRY();

    if(MDrv_TSP_PVR_Eng_IsStart((MS_U32)Eng, pbIsStart) != DRVTSP_OK)
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Pid(DMX_PVR_ENG Eng ,MS_U32 u32DmxId , MS_U32 *Pid, MS_BOOL bSet)
{
    DMX_PVRDBGMSG(DMX_DBG_PVR, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));
    DMX_ASSERT(DMX_PVR_TSO0 > Eng, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Invalid Engine ID!\n", __LINE__)));

    _DMX_PVR_ENTRY();

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
    //[TODO] wait for use case //
    //DMX_PVRDBGMSG(DMX_DBG_PVR, ULOGD("DMX", "[%s] %d Not implement yet!!\n", __FUNCTION__, __LINE__);
    _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);

#else  // For TSP_VER_4_0
    MS_U32  u32PvrEng = 0;

    if(_DMX_ApiDrv_PvrEngMapping(Eng, &u32PvrEng) == FALSE)
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (bSet)
    {
        MDrv_TSP_PVR_Eng_FltSetPID(u32PvrEng, u32DmxId, *Pid);
        _FLT_LIST_PID_SET(u32DmxId, *Pid);
    }
    else
    {
       *Pid =  DMX_PID_NULL;
        MDrv_TSP_PVR_Eng_FltGetPID(u32PvrEng, u32DmxId,(MS_U32*) Pid);  //@F_TODO not implement yet
    }

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
#endif
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Start(DMX_PVR_ENG Eng, MS_BOOL bPvrAll)
{
    if(Eng == DMX_PVR_TSO0)
    {
        if(bPvrAll == FALSE)  //only support record all mode
        {
            return DMX_FILTER_STATUS_ERROR;
        }

#ifdef TSO_PVR_SUPPORT
        _DMX_TSO_ENTRY();
        if(MDrv_TSO_PVR_Start(Eng, E_DRVTSO_REC_MODE_BYPASS, TRUE) == DRVTSO_OK)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
        }
        else
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }
#else
        return DMX_FILTER_STATUS_ERROR;
#endif //TSO_PVR_SUPPORT
    }

    _DMX_PVR_ENTRY();

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_Start(Eng, ((bPvrAll)? E_DRVTSP_REC_MODE_ENG0_BYPASS: E_DRVTSP_REC_MODE_ENG0_FLTTYPE), TRUE))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    else
#else  // For TSP_VER_4_0
    MS_U32  u32PvrEng = 0;

    if(_DMX_ApiDrv_PvrEngMapping(Eng, &u32PvrEng) == FALSE)
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_Start(u32PvrEng, bPvrAll, TRUE))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#endif

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_Stop(DMX_PVR_ENG Eng)
{
    if(Eng == DMX_PVR_TSO0)
    {
#ifdef TSO_PVR_SUPPORT
        _DMX_TSO_ENTRY();
        if(MDrv_TSO_PVR_Start(Eng, E_DRVTSO_REC_MODE_FLTTYPE, FALSE) == DRVTSO_OK)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
        }
        else
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }
#else
        return DMX_FILTER_STATUS_ERROR;
#endif //TSO_PVR_SUPPORT
    }

    _DMX_PVR_ENTRY();

    MS_U32  u32PvrEng = Eng;

#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
    if(_DMX_ApiDrv_PvrEngMapping(Eng, &u32PvrEng) == FALSE)
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#endif

    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_Start(u32PvrEng, E_DRVTSP_REC_MODE_ENG0_FLTTYPE, FALSE))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_WriteGet(DMX_PVR_ENG Eng ,MS_PHY* phyWrite)
{
    if(Eng == DMX_PVR_TSO0)
    {
#ifdef TSO_PVR_SUPPORT
        _DMX_TSO_ENTRY();
        if(MDrv_TSO_PVR_GetWriteAddr(Eng, phyWrite) == DRVTSO_OK)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
        }
        else
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }
#else
        return DMX_FILTER_STATUS_ERROR;
#endif //TSO_PVR_SUPPORT
    }

#if ((defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))) && defined(SECURE_PVR_ENABLE))

    _DMX_PVR_ENTRY();

    _pdmx_res->_stSecBuf.u32BufId = (MS_U32)Eng;
    _pdmx_res->_stSecBuf.u32BufOpt = 0;
    _pdmx_res->_stSecBuf.u32BufSize = 0;
    _pdmx_res->_stSecBuf.phyBufAddr = 0;

    if(MDrv_TSP_Ree_SendTeeCmd(E_DRVTSP_REE_TO_TEE_CMD_GET_PvrWPtr, (void*)&(_pdmx_res->_stSecBuf), sizeof(DrvTSP_SecureBuf)) != DRVTSP_OK)
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    else
    {
        *phyWrite = _pdmx_res->_stSecBuf.phyBufAddr;
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
    }
#endif //SECURE_PVR_ENABLE

#ifndef SECURE_PVR_ENABLE
    MS_U32  u32PvrEng = Eng;

#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
    if(_DMX_ApiDrv_PvrEngMapping(Eng, &u32PvrEng) == FALSE)
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#endif

    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_GetWriteAddr(u32PvrEng, phyWrite))
    {
        return (DMX_FILTER_STATUS_ERROR);
    }
#endif

    return DMX_FILTER_STATUS_OK;
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_SetPacketMode(DMX_PVR_ENG Eng ,MS_BOOL bSet)
{
    if(Eng == DMX_PVR_TSO0)
    {
#ifdef TSO_PVR_SUPPORT
        _DMX_TSO_ENTRY();
        if(MDrv_TSO_PVR_SetPacketMode(Eng, bSet) == DRVTSO_OK)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
        }
        else
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }
#else
        return DMX_FILTER_STATUS_ERROR;
#endif //TSO_PVR_SUPPORT
    }

    _DMX_PVR_ENTRY();

    MS_U32  u32PvrEng = Eng;

#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
    if(_DMX_ApiDrv_PvrEngMapping(Eng, &u32PvrEng) == FALSE)
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#endif

    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_SetPacketMode(u32PvrEng, bSet))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_SetRecordStamp(DMX_PVR_ENG Eng ,MS_U32 u32Stamp)
{
    if(Eng == DMX_PVR_TSO0)
    {
#ifdef TSO_PVR_SUPPORT
        _DMX_TSO_ENTRY();
        if(MDrv_TSO_PVR_TimeStampSetRecordStamp(Eng, u32Stamp) == DRVTSO_OK)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
        }
        else
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }
#else
        return DMX_FILTER_STATUS_ERROR;
#endif //TSO_PVR_SUPPORT
    }

    _DMX_PVR_ENTRY();

    MS_U32  u32PvrEng = Eng;

#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
    if(_DMX_ApiDrv_PvrEngMapping(Eng, &u32PvrEng) == FALSE)
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#endif

    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_TimeStampSetRecordStamp(u32PvrEng, u32Stamp))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_GetRecordStamp(DMX_PVR_ENG Eng ,MS_U32* pu32Stamp)
{
    if(Eng == DMX_PVR_TSO0)
    {
#ifdef TSO_PVR_SUPPORT
        _DMX_TSO_ENTRY();
        if(MDrv_TSO_PVR_TimeStampGetRecordStamp(Eng, pu32Stamp) == DRVTSO_OK)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
        }
        else
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }
#else
        return DMX_FILTER_STATUS_ERROR;
#endif //TSO_PVR_SUPPORT
    }

    MS_U32  u32PvrEng = Eng;

#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
    if(_DMX_ApiDrv_PvrEngMapping(Eng, &u32PvrEng) == FALSE)
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#endif

    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_TimeStampGetRecordStamp(u32PvrEng, pu32Stamp))
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    return DMX_FILTER_STATUS_OK;
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_SetPlaybackStampClk(MS_U8 u8Eng, DMX_TimeStamp_Clk eClkSrc)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0))

    if(u8Eng == 0)
    {
        if(DRVTSP_OK != MDrv_TSP_PVR_Eng_SetPlaybackStampClk(u8Eng, (MS_U32)eClkSrc))
        {
            return DMX_FILTER_STATUS_ERROR;
        }
    }
    else
    {
      #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))

        return DMX_FILTER_STATUS_ERROR;

      #else

        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(u8Eng == 2)
            mmpath = DMX_MMFI_PATH1;

        if(DRVMMFI_OK != MDrv_MMFI_File_SetTimeStampClk(mmpath, (MS_U32)eClkSrc))
        {
            return DMX_FILTER_STATUS_ERROR;
        }
      #endif
    }

    return DMX_FILTER_STATUS_OK;

#else  // For TSP_VER_4_0

    TSP_TimeStamp_Clk eClk = E_TSP_TIMESTAMP_CLK_INVALID;
    TSP_FILE_ENG      eEng = _DMX_ApiDrv_FileinEngMapping((DMX_FILEIN_PATH)u8Eng);

    if(eEng == E_TSP_FILE_ENG_INVALID)
        return DMX_FILTER_STATUS_ERROR;

    switch (eClkSrc)
    {
        case DMX_TIMESTAMP_CLK_90K:
            eClk = E_TSP_TIMESTAMP_CLK_90K;
            break;
        case DMX_TIMESTAMP_CLK_27M:
            eClk = E_TSP_TIMESTAMP_CLK_27M;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad DMX_TimeStamp_Clk %u\n", __LINE__, (unsigned int)eClkSrc));
            return DMX_FILTER_STATUS_ERROR;
    }

    _DMX_ENTRY();

    if (DRVTSP_OK!= MDrv_TSP_FILE_Eng_TimeStampSetPlaybackStampClk(eEng, eClk))
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Call MDrv_TSP_FILE_Eng_TimeStampSetPlaybackStampClk fail\n", __LINE__)));
    }

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#endif  // End of TSP_VER_4_0
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_SetRecordStampClk(DMX_PVR_ENG Eng, DMX_TimeStamp_Clk eClkSrc)
{
#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0))
  #ifdef TSO_PVR_SUPPORT
    if(Eng == DMX_PVR_TSO0)
    {
        if(MDrv_TSO_PVR_SetRecordStampClk((MS_U8)Eng, (MS_U32)eClkSrc) != DRVTSO_OK)
        {
            return DMX_FILTER_STATUS_ERROR;
        }
        else
        {
            return DMX_FILTER_STATUS_OK;
        }
    }
    else
 #endif  //TSO_PVR_SUPPORT
    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_SetRecordStampClk((MS_U32)Eng, (MS_U32)eClkSrc))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    else
    {
        return DMX_FILTER_STATUS_OK;
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_CallbackSize(DMX_PVR_ENG Eng, MS_U32* pu32CallbackSize, MS_BOOL bSet)
{
    return DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
///Set PVR CA SPS enable mode
/// @param Eng                   \b IN: PVR Engine ID
/// @param eCaMode               \b IN: CA PVR Selection, 0: from TSP, 1: from ca program0, 2: from ca program1
/// @param bspsEnable            \b IN: SPS mode enabled flag
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_SetCaMode(DMX_PVR_ENG Eng, DMX_CA_PVRMODE eCaMode, MS_BOOL bspsEnable)
{
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    return DMX_FILTER_STATUS_ERROR;

#else  // For TSP_VER_4_0
    MS_U32  u32PvrEng = 0;

    if(_DMX_ApiDrv_PvrEngMapping(Eng, &u32PvrEng) == FALSE)
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_SetCaMode(u32PvrEng, (MS_U16)eCaMode, bspsEnable))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    else
    {
        return DMX_FILTER_STATUS_OK;
    }
#endif
}

//-------------------------------------------------------------------------------------------------
/// Open PVR PID filter for recording scramble stream depended on Engine ID
/// @param Eng                                      \b IN: PVR engine ID
/// @param Pid                                       \b IN: PID to record
/// @param pu8DmxId                             \b IN: Pointer to store PVR PID filter index
/// @param u8ShareKeyType                   \b IN: PID filter share key type. 0: Not use sharekey filters.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_PvrCA_Eng_Pid_Open(DMX_PVR_ENG Eng, MS_U32 Pid, MS_U32* pu32DmxId, MS_U8 u8ShareKeyType)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s](%d, 0x%08x, 0x%lx) %d\n", __FUNCTION__, (int)Eng, (unsigned int)Pid, (unsigned long)pu32DmxId, __LINE__));

// @F_TODO what is the difference between pvrca pid open and pvrca Eng pid open

#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0)
    MS_U32 u32fltid = 0;
    DrvTSP_FltType ftype = 0;

    switch(Eng)
    {
        case DMX_PVR_EGN0 :
            ftype = E_DRVTSP_FLT_TYPE_PVR;
            break;
        case DMX_PVR_EGN1 :
            ftype = E_DRVTSP_FLT_TYPE_PVR1;
            break;
        default:
            return (DMX_FILTER_STATUS_ERROR);
    }

    _DMX_ENTRY();

    switch((DrvTSP_PKTDMXSrcType)(_pdmx_res->_u32PVREngSrc[(MS_U8)Eng]))
    {
        case E_DRVTSP_PKTSRC_DEMUX0:
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_LIVE;
            break;
        case E_DRVTSP_PKTSRC_DEMUXFILE:
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_FILE;
            break;
        case E_DRVTSP_PKTSRC_DEMUX1:
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_TS1;
            break;
        case E_DRVTSP_PKTSRC_DEMUX2:
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_TS2;
            break;
        default:
            break;
    }

    if(_pdmx_res->_u32PVREngSrc[(MS_U8)Eng] == 0)
    {
        if(Eng == DMX_PVR_EGN0)
        {
            _pdmx_res->_u32PVREngSrc[0] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX0;
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_LIVE;
        }
        else if(Eng == DMX_PVR_EGN1)
        {
            _pdmx_res->_u32PVREngSrc[1] = (MS_U32)E_DRVTSP_PKTSRC_DEMUX1;
            ftype |= E_DRVTSP_FLT_SOURCE_TYPE_TS1;
        }
    }

    if(u8ShareKeyType == 1)
    {
        ftype |= E_DRVTSP_FLT_TYPE_SCMB_SHAREKEY;
    }
    else
    {
        ftype |= E_DRVTSP_FLT_TYPE_SCMB;
    }

    if (DRVTSP_OK!= MDrv_TSP_PidFlt_Alloc(_Owner, ftype, &u32fltid))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    DMX_ASSERT2(u32fltid < DMX_MAX_FLTID, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad FltId %u\n", __LINE__, (unsigned int)u32fltid)));
    MDrv_TSP_PidFlt_SetPid(0, u32fltid, Pid);
    _FLT_LIST_PID_SET((MS_U8)u32fltid, (MS_U16)Pid);
    MDrv_TSP_PidFlt_Enable(0, u32fltid, TRUE);

    _FLT_LIST_SECFLT_TYPE_SET(u32fltid, u32fltid, 0xFFFFFFFF, (DMX_FILTER_TYPE)0xFF);
    *pu32DmxId =  u32fltid;

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#elif(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))

    if((Eng != DMX_PVR_EGN0) || _pdmx_res->_u32PVREngSrc[Eng] == E_DRVTSP_PKTSRC_DEMUX1) //PVRCA1 with PVR0 engine
    {
        return _MApi_DMX_Pvr_Eng_Pid_Open(Eng, Pid, pu32DmxId );
    }

    _DMX_ENTRY();
    if(MDrv_TSP_PVR_PidFlt_Reserved(Pid, pu32DmxId, TRUE) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

#else  // For TSP_VER_4_0

    return (_MApi_DMX_Pvr_Eng_Pid_Open(Eng,Pid,pu32DmxId));

#endif

    return DMX_FILTER_STATUS_ERROR;
}

//do nothing, before close path switch to TS1
//-------------------------------------------------------------------------------------------------
/// Close PVR PID filter depended on Engine ID
/// @param Eng                                      \b IN: PVR engine ID
/// @param u8DmxId                              \b IN: PID filter index to close
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_PvrCA_Eng_Pid_Close(DMX_PVR_ENG Eng, MS_U32 u32DmxId)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s](%d, %d) %d\n", __FUNCTION__, (int)Eng, (int)u32DmxId, __LINE__));

#if(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
    _DMX_ENTRY();
    MDrv_TSP_PidFlt_Enable(0, u32DmxId, FALSE);
    MDrv_TSP_PidFlt_SetPid(0, u32DmxId, DRVTSP_PID_NULL);
    _FLT_LIST_PID_SET(u32DmxId, DRVTSP_PID_NULL);
    MDrv_TSP_PidFlt_Free(0, u32DmxId);
    _FLT_LIST_REMOVE(u32DmxId);
    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#elif(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))

    if((Eng != DMX_PVR_EGN0) || _pdmx_res->_u32PVREngSrc[Eng] == E_DRVTSP_PKTSRC_DEMUX1)  //PVRCA1 with PVR0 engine
    {
        return _MApi_DMX_Pvr_Eng_Pid_Close(Eng, u32DmxId);
    }

    _DMX_ENTRY();

    if(MDrv_TSP_PVR_PidFlt_Reserved(0x1FFF, &u32DmxId, FALSE) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else  // For TSP_VER_4_0
    return (_MApi_DMX_Pvr_Eng_Pid_Close(Eng, u32DmxId));
#endif

    return DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Start to record scramble stream depend on engine id
/// @param Eng                                      \b IN: PVR engine ID
/// @param bPvrAll                                 \b IN: If true, record all stream data; if false, record data by PIDs
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_PvrCA_Eng_Start(DMX_PVR_ENG Eng, MS_BOOL bPvrAll)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s](%d, %d) %d\n", __FUNCTION__, (int)Eng, (int)bPvrAll, __LINE__));

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

  #if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))
    if((Eng != DMX_PVR_EGN0) || (_pdmx_res->_u32PVREngSrc[Eng] == E_DRVTSP_PKTSRC_DEMUX1))  //PVRCA1 with PVR0 engine
    {
        return _MApi_DMX_Pvr_Eng_Start(Eng, bPvrAll);
    }
  #endif

    _DMX_ENTRY();

    if(bPvrAll)
    {
        if(MDrv_TSP_PVR_Eng_Start(Eng, E_DRVTSP_REC_MODE_ENG0_BYPASS, TRUE) != DRVTSP_OK)
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
        _pdmx_res->_bPVRCA_RecAll[Eng] = TRUE;
    }
    else
    {
        if(MDrv_TSP_PVR_Eng_Start(Eng, E_DRVTSP_REC_MODE_ENG0_FLT1CA, TRUE) != DRVTSP_OK)
        {
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
        }
  #if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))
        if(MDrv_TSP_PVR_FLT1_StartRec(TRUE) != DRVTSP_OK)
        {
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
        }
  #endif
        _pdmx_res->_bPVRCA_RecAll[Eng] = FALSE;
    }

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#else  // For TSP_VER_4_0
    return (_MApi_DMX_Pvr_Eng_Start(Eng,bPvrAll));
#endif

    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Stop to record scramble stream
/// @param Eng                                      \b IN: PVR engine ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_PvrCA_Eng_Stop(DMX_PVR_ENG Eng)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s](%d) %d\n", __FUNCTION__, (int)Eng, __LINE__));

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

  #if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))
    if((Eng != DMX_PVR_EGN0) || (_pdmx_res->_u32PVREngSrc[Eng] == E_DRVTSP_PKTSRC_DEMUX1))  //PVRCA1 with PVR0 engine
    {
        return _MApi_DMX_Pvr_Eng_Stop(Eng);
    }
  #endif

    _DMX_ENTRY();

    if(_pdmx_res->_bPVRCA_RecAll[Eng])
    {
        if(MDrv_TSP_PVR_Eng_Start(Eng, E_DRVTSP_REC_MODE_ENG0_FLTTYPE, FALSE) != DRVTSP_OK)
        {
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
        }
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }

  #if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))
    if(MDrv_TSP_PVR_FLT1_StartRec(FALSE) != DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
  #endif

    if(MDrv_TSP_PVR_Eng_Start(Eng, E_DRVTSP_REC_MODE_ENG0_FLT1CA, FALSE) != DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#else  // For TSP_VER_4_0
    return (_MApi_DMX_Pvr_Eng_Stop(Eng));
#endif

    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Open & set pid filter for file-in PVR using
/// @param Eng                                       \b IN   : PVR eigine id
/// @param Pid                                        \b IN   : PVR PID to record
/// @param pu8DmxId                             \b OUT: Pointer to store PID filter id
/// @param u8ShareKeyType                    \b IN   : The flag of sharekey filter using. if 0, not use sharekey filters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Pvr_Eng_Pid_Open(DMX_PVR_ENG Eng, MS_U32 Pid, MS_U8* pu8DmxId, MS_U8 u8ShareKeyType)
{
#if(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))

    MS_U32 u32fltid = 0;
    DrvTSP_FltType ftype = E_DRVTSP_FLT_SOURCE_TYPE_FILE|E_DRVTSP_FLT_TYPE_PVR;

    if(u8ShareKeyType != 0)
    {
        ftype |= E_DRVTSP_FLT_TYPE_SCMB_SHAREKEY;
    }

    _DMX_ENTRY();

    if (DRVTSP_OK!= MDrv_TSP_PidFlt_Alloc(_Owner, ftype, &u32fltid))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    DMX_ASSERT2(u32fltid < DMX_MAX_FLTID, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad FltId %u\n", __LINE__, (int)u32fltid)));
    MDrv_TSP_PidFlt_SetPid(0, u32fltid, Pid);
    _FLT_LIST_PID_SET((MS_U8)u32fltid, (MS_U16)Pid);
    MDrv_TSP_PidFlt_Enable(0, u32fltid, TRUE);

    _FLT_LIST_SECFLT_TYPE_SET(u32fltid, u32fltid, 0xFF, (DMX_FILTER_TYPE)0xFF);
    *pu8DmxId =  (MS_U8)(u32fltid & 0xFF);

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Close pid filter of file-in PVR
/// @param Eng                                       \b IN   : PVR eigine id
/// @param u8DmxId                               \b IN   : PID filter id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Pvr_Eng_Pid_Close(DMX_PVR_ENG Eng, MS_U8 u8DmxId)
{
#if(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))

    DMX_PVRDBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d (%d)\n", __FUNCTION__, __LINE__, (int)u8DmxId));

    DMX_ASSERT(DMX_MAX_FLTID > u8DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, u8DmxId)));

    _DMX_ENTRY();

    MDrv_TSP_PidFlt_Enable(0, u8DmxId, FALSE);
    MDrv_TSP_PidFlt_SetPid(0, u8DmxId, DRVTSP_PID_NULL);
    _FLT_LIST_PID_SET(u8DmxId, DRVTSP_PID_NULL);
    MDrv_TSP_PidFlt_Free(0, u8DmxId);
    _FLT_LIST_REMOVE(u8DmxId);

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Start file-in PVR recording
/// @param Eng                                       \b IN   : PVR eigine id
/// @param bPvrAll                                  \b IN   : FALSE is record PID, TRUE is record All ts data
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Pvr_Eng_Start(DMX_PVR_ENG Eng, MS_BOOL bPvrAll)
{
#if(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))

    _DMX_PVR_ENTRY();

    if(bPvrAll)
    {
        if(MDrv_TSP_PVR_Eng_Start(Eng, E_DRVTSP_REC_MODE_ENG0_FI_BYPASS, TRUE) != DRVTSP_OK)
        {
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
        }
    }
    else
    {
        if(MDrv_TSP_PVR_Eng_Start(Eng, E_DRVTSP_REC_MODE_ENG0_FI_FLTTYPE, TRUE) != DRVTSP_OK)
        {
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
        }
    }

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);

#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Stop file-in PVR recording
/// @param Eng                                       \b IN   : PVR eigine id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Pvr_Eng_Stop(DMX_PVR_ENG Eng)
{
#if(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))

    _DMX_PVR_ENTRY();

    if(DRVTSP_OK != MDrv_TSP_PVR_Eng_Start(Eng, E_DRVTSP_REC_MODE_ENG0_FI_BYPASS, FALSE))
    {
        _DMX_PVR_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_PVR_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set up parameters for input bit stream from memory
/// @param pFileinInfo \b IN: the file in parameters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Info(DMX_Filein_info *pFileinInfo)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FILEIN);
        dmx_dbg_print("[DMX FileIn Info]\tRate[0x%x] PktMode[0x%x]\n",
            (MS_S32)pFileinInfo->Rate, (MS_S32)pFileinInfo->PKT_Mode);
#endif

#if defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0)
    MDrv_MMFI_File_SetPacketMode(E_DRVMMFI_PATH0, (DrvMMFI_PacketMode)pFileinInfo->PKT_Mode);
    MDrv_MMFI_File_SetTimer(E_DRVMMFI_PATH0, (MS_U8)(pFileinInfo->Rate & 0xFF));
    MDrv_MMFI_File_SetPacketMode(E_DRVMMFI_PATH1, (DrvMMFI_PacketMode)pFileinInfo->PKT_Mode);
    MDrv_MMFI_File_SetTimer(E_DRVMMFI_PATH1, (MS_U8)(pFileinInfo->Rate & 0xFF));
#else // For MMFI_VER_1_0

  #ifdef MMFILEIN
    MDrv_MMFI_File_SetPacketMode(E_DRVMMFI_PTH_AUDIO, (DrvMMFI_PacketMode)pFileinInfo->PKT_Mode);
    MDrv_MMFI_File_SetTimer(E_DRVMMFI_PTH_AUDIO, (MS_U8)(pFileinInfo->Rate & 0xFF));
  #endif //MMFILEIN

  #ifdef MMFI_VD3D
    MDrv_MMFI_File_SetPacketMode(E_DRVMMFI_PATH_VD3D, (DrvMMFI_PacketMode)pFileinInfo->PKT_Mode);
    MDrv_MMFI_File_SetTimer(E_DRVMMFI_PATH_VD3D, (MS_U8)(pFileinInfo->Rate & 0xFF));
  #endif //MMFILEIN

#endif // End of MMFI_VER_1_0

    MDrv_TSP_File_SetPacketMode((DrvTSP_PacketMode)(pFileinInfo->PKT_Mode));
    return (DRVTSP_OK == MDrv_TSP_File_SetRate(pFileinInfo->Rate))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
}

// @FIXME: ignore audio/video PES at this stage
//-------------------------------------------------------------------------------------------------
/// Start to get bit stream by memeory
/// @param Dst \b IN: file in destination type
/// @param pBuf \b IN: the memory containing the bit stream
/// @param u32BufSize \b IN: the size the memory to get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Start(DMX_FILEIN_DST Dst, MS_PHY pBuf, MS_U32 u32BufSize)
{
    DrvTSP_FileinMode eFileMode;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FILEIN);
        dmx_dbg_print("[DMX FileIn Start]\tDst[0x%08X] Addr[0x%08X] Size[0x%08X]\n",
            (MS_S32)Dst, (MS_U32)pBuf, u32BufSize);
#endif

    _DMX_ENTRY();

    switch (Dst)
    {
    case DMX_PES_NO_BYPASS_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_TS;
        break;
    case DMX_PES_AUDIO_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_APES;
        break;
    case DMX_PES_AUDIO2_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_A2PES;
        break;
#if (!(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))) // For TSP_VER_3_0 and TSP_VER_4_0
    case DMX_PES_AUDIO3_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_A3PES;
        break;
    case DMX_PES_AUDIO4_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_A4PES;
        break;
#endif
#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))) // For TSP_VER_4_0
    case DMX_PES_AUDIO5_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_A5PES;
        break;
    case DMX_PES_AUDIO6_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_A6PES;
        break;
#endif
    case DMX_PES_VIDEO_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_VPES;
        break;
#ifdef MMFI_VD3D
    case DMX_PES_VIDEO3D_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_V3DPES;
        break;
#endif
#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))) // For TSP_VER_4_0
    case DMX_PES_VIDEO3_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_V3PES;
        break;
    case DMX_PES_VIDEO4_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_V4PES;
        break;
    case DMX_PES_VIDEO5_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_V5PES;
        break;
    case DMX_PES_VIDEO6_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_V6PES;
        break;
    case DMX_PES_VIDEO7_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_V7PES;
        break;
    case DMX_PES_VIDEO8_FIFO:
        eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_V8PES;
        break;
#endif
    default:
        DMX_ASSERT2(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad DMX_FILEIN_DST %u\n", __LINE__, (unsigned int)Dst)));
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
        break;
    }

#ifdef SECURE_PVR_ENABLE
    _pdmx_res->_stSecBuf.u32BufId = 0;
    _pdmx_res->_stSecBuf.u32BufOpt = 0;
    _pdmx_res->_stSecBuf.phyBufAddr = pBuf;
    _pdmx_res->_stSecBuf.u32BufSize = u32BufSize;
    if(MDrv_TSP_Ree_SendTeeCmd(E_DRVTSP_REE_TO_TEE_CMD_SET_FileinBuf, (void*)&(_pdmx_res->_stSecBuf), sizeof(DrvTSP_SecureBuf)) != DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else

    if (DRVTSP_OK != MDrv_TSP_File_SetAddr(pBuf))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (DRVTSP_OK != MDrv_TSP_File_SetSize(u32BufSize))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#endif //SECURE_PVR_ENABLE

    if (DRVTSP_OK!= MDrv_TSP_File_Start(eFileMode))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Stop to get bit stream by memeory
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Stop(void)
{

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FILEIN);
        dmx_dbg_print("[DMX FileIn Stop]\n");
#endif

    _DMX_ENTRY();
    if (DRVTSP_OK != MDrv_TSP_File_Stop())
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Pause to get bit stream by memeory
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Pause(void)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FILEIN);
        dmx_dbg_print("[DMX FileIn Pause]\n");
#endif

    return (DRVTSP_OK== MDrv_TSP_File_Pause())? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Resume to get bit stream by memeory
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Resume(void)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FILEIN);
        dmx_dbg_print("[DMX FileIn Resume]\n");
#endif

    return (DRVTSP_OK== MDrv_TSP_File_Resume())? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Resume to get bit stream by memeory
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_CMDQ_Reset(void)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FILEIN);
        dmx_dbg_print("[DMX FileIn CMDQ Reset]\n");
#endif

    if (MDrv_TSP_File_CMDQ_Reset() == DRVTSP_OK)
    {
        #ifdef DMX_RESET_FI_TIMESTAMP
        if(_pdmx_res->_bPlayTimestampEnable[0] == TRUE)
        {
            _DMX_ENTRY();
            MDrv_TSP_File_SetAddr(_pdmx_res->_phyFI192DummyBufAddr);
            MDrv_TSP_File_SetSize(_pdmx_res->_u32FI192DummyBufSize);
            MDrv_TSP_File_Start(E_DRVTSP_FILEIN_MODE_ENG0_TS);
            _DMX_RETURN(DMX_FILTER_STATUS_OK);
        }
        #endif

        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
}

//-------------------------------------------------------------------------------------------------
/// Resume to get bit stream by memeory
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_CMDQ_GetEmptyNum(MS_U32 *pu32EmptySlot)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    MDrv_TSP_File_CMDQ_GetSlot(pu32EmptySlot);
    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get Command queue fifo level
/// @param  pu8CmdQStatus      \b OUT: fifo level, 0~3
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_CMDQ_FIFOWriteLevel(MS_U8 *pu8CmdQStatus)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    MDrv_TSP_CmdQFifo_Status(pu8CmdQStatus);
    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Check if no memory transfer is under going
/// @return TRUE if idle, FALSE otherwise
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL _MApi_DMX_Filein_IsIdle(void)
{
    DrvTSP_FileinState     FileinState = E_DRVTSP_FILEIN_STATE_BUSY;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    if (DRVTSP_OK!= MDrv_TSP_File_GetState(&FileinState))
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Call MDrv_TSP_File_GetState fail\n", __LINE__)));
    }

    return (E_DRVTSP_FILEIN_STATE_IDLE== FileinState);

}

//-------------------------------------------------------------------------------------------------
/// Check if memory transfer is under going
/// @return TRUE if busy, FALSE otherwise
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL _MApi_DMX_Filein_IsBusy(void)
{
    DrvTSP_FileinState     FileinState = E_DRVTSP_FILEIN_STATE_IDLE;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    if (DRVTSP_OK!= MDrv_TSP_File_GetState(&FileinState))
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Call MDrv_TSP_File_GetState fail\n", __LINE__)));
    }
    return (E_DRVTSP_FILEIN_STATE_BUSY== FileinState);
}

//-------------------------------------------------------------------------------------------------
/// Check if memory transfer is paused
/// @return TRUE if paused, FALSE otherwise
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL _MApi_DMX_Filein_IsPause(void)
{
    DrvTSP_FileinState     FileinState = E_DRVTSP_FILEIN_STATE_IDLE;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    if (DRVTSP_OK!= MDrv_TSP_File_GetState(&FileinState))
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Call MDrv_TSP_File_GetState fail\n", __LINE__)));
    }
    return (E_DRVTSP_FILEIN_STATE_PAUSE== FileinState);
}

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_Start(DMX_FILEIN_PATH ePath, DMX_FILEIN_DST Dst, MS_PHY pBuf, MS_U32 u32BufSize)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_Start(Dst, pBuf, u32BufSize);
    }
    else
    {
    #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))

        return DMX_FILTER_STATUS_ERROR;

        #else

        DMX_MMFI_DST eDst = 0;

        if(ePath == DMX_FILEIN_TSIF2)
        {
            eDst |= DMX_MMFI1_PES_TYPE_MASK;
        }

        switch (Dst)
        {
            case DMX_PES_NO_BYPASS_FIFO:
            eDst |= DMX_MMFI_PES_NO_BYPASS_TS;
                break;
            case DMX_PES_AUDIO2_FIFO:
                eDst |= DMX_MMFI_PES_PS_AUB;
                break;
            case DMX_PES_AUDIO_FIFO:
                eDst |= DMX_MMFI_PES_PS_AU;
                break;
            case DMX_PES_VIDEO_FIFO:
                eDst |= DMX_MMFI_PES_PS_VD;
                break;
        #ifdef MMFI_VD3D
            case DMX_PES_VIDEO3D_FIFO:
                eDst |= DMX_MMFI_PES_PS_V3D;
                break;
        #endif
            case DMX_PES_AUDIO3_FIFO:
                eDst |= DMX_MMFI_PES_PS_AUC;
                break;
            case DMX_PES_AUDIO4_FIFO:
                eDst |= DMX_MMFI_PES_PS_AUD;
                break;
            default:
                DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad DMX_FILEIN_DST %d\n", __LINE__, (int)Dst));
            return DMX_FILTER_STATUS_ERROR;
        }

        return _MApi_DMX_MMFI_Filein_Start(eDst, pBuf, u32BufSize);

    #endif
    }

#else  // For TSP_VER_4_0

    DrvTSP_FileinMode eFileMode;
    TSP_FILE_ENG eEng = _DMX_ApiDrv_FileinEngMapping(ePath);

    if(eEng == E_TSP_FILE_ENG_INVALID)
        return DMX_FILTER_STATUS_ERROR;

    _DMX_ENTRY();

    switch (Dst)
    {
        case DMX_PES_NO_BYPASS_FIFO:
            eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_TS;
            break;
        case DMX_PES_AUDIO_FIFO:
            eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_APES;
            break;
        case DMX_PES_AUDIO2_FIFO:
            eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_A2PES;
            break;
        case DMX_PES_AUDIO3_FIFO:
            eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_A3PES;
            break;
        case DMX_PES_AUDIO4_FIFO:
            eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_A4PES;
            break;
        case DMX_PES_AUDIO5_FIFO:
            eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_A5PES;
            break;
        case DMX_PES_AUDIO6_FIFO:
            eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_A6PES;
            break;
        case DMX_PES_VIDEO_FIFO:
            eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_VPES;
            break;
    #ifdef MMFI_VD3D
        case DMX_PES_VIDEO3D_FIFO:
            eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_V3DPES;
            break;
    #endif
        case DMX_PES_VIDEO3_FIFO:
            eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_V3PES;
            break;
        case DMX_PES_VIDEO4_FIFO:
            eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_V4PES;
            break;
        case DMX_PES_VIDEO5_FIFO:
            eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_V5PES;
            break;
        case DMX_PES_VIDEO6_FIFO:
            eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_V6PES;
            break;
        case DMX_PES_VIDEO7_FIFO:
            eFileMode = E_DRVTSP_FILEIN_MODE_ENG0_V7PES;
            break;

        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad DMX_FILEIN_DST %u\n", __LINE__, (unsigned int)Dst));
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

  #ifdef SECURE_PVR_ENABLE
    _pdmx_res->_stSecBuf.u32BufId = (MS_U32)eEng;
    _pdmx_res->_stSecBuf.u32BufOpt = 0;
    _pdmx_res->_stSecBuf.phyBufAddr = pBuf;
    _pdmx_res->_stSecBuf.u32BufSize = u32BufSize;
    if(MDrv_TSP_Ree_SendTeeCmd(E_DRVTSP_REE_TO_TEE_CMD_SET_FileinBuf, (void*)&(_pdmx_res->_stSecBuf), sizeof(DrvTSP_SecureBuf)) != DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
  #else
    if (DRVTSP_OK != MDrv_TSP_File_Eng_SetSize(eEng, u32BufSize))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    if (DRVTSP_OK != MDrv_TSP_File_Eng_SetAddr(eEng, pBuf))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
  #endif

    if (DRVTSP_OK!= MDrv_TSP_File_Eng_Start(eEng, eFileMode))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#endif  // End of TSP_VER_4_0
}

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_Stop(DMX_FILEIN_PATH ePath)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_Stop();
    }
    else
    {
  #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))
            return DMX_FILTER_STATUS_ERROR;
  #else

        DMX_MMFI_DST eDst = 0;

        if(ePath == DMX_FILEIN_TSIF2)
        {
            eDst |= DMX_MMFI1_PES_TYPE_MASK;
        }

        if(MDrv_MMFI_File_ModeEnable(eDst, FALSE) == DRVMMFI_FAIL)
        {
            return DMX_FILTER_STATUS_ERROR;
        }
        else
        {
            return DMX_FILTER_STATUS_OK;
        }
  #endif
    }

#else  // For TSP_VER_4_0

    TSP_FILE_ENG eEng = _DMX_ApiDrv_FileinEngMapping(ePath);

    if(eEng == E_TSP_FILE_ENG_INVALID)
        return DMX_FILTER_STATUS_ERROR;

    _DMX_ENTRY();

    if (DRVTSP_OK != MDrv_TSP_File_Eng_Stop(eEng))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#endif  // End of TSP_VER_4_0
}

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_Info(DMX_FILEIN_PATH ePath, DMX_Filein_info *pFileinInfo)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    if(ePath == DMX_FILEIN_TSIF0)
    {
        MDrv_TSP_File_SetPacketMode((DrvTSP_PacketMode)(pFileinInfo->PKT_Mode));
        return (DRVTSP_OK == MDrv_TSP_File_SetRate(pFileinInfo->Rate))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
    }
    else
    {
  #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))
        return DMX_FILTER_STATUS_ERROR;
  #else

        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
        {
            mmpath = DMX_MMFI_PATH1;
        }
        return _MApi_DMX_MMFI_Filein_Info(mmpath, pFileinInfo);
  #endif
    }
#else  // For TSP_VER_4_0

    TSP_FILE_ENG eEng = _DMX_ApiDrv_FileinEngMapping(ePath);

    if(eEng == E_TSP_FILE_ENG_INVALID)
        return DMX_FILTER_STATUS_ERROR;

    // @F_TODO we set every PATH with same parameter???? add info function for MMFI
  #if 0
    #if defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0)
        MDrv_MMFI_File_SetPacketMode(E_DRVMMFI_PATH0, (DrvMMFI_PacketMode)pFileinInfo->PKT_Mode);
        MDrv_MMFI_File_SetTimer(E_DRVMMFI_PATH0, (MS_U8)(pFileinInfo->Rate & 0xFF));
        MDrv_MMFI_File_SetPacketMode(E_DRVMMFI_PATH1, (DrvMMFI_PacketMode)pFileinInfo->PKT_Mode);
        MDrv_MMFI_File_SetTimer(E_DRVMMFI_PATH1, (MS_U8)(pFileinInfo->Rate & 0xFF));
    #else //MMFI_VER_2_0

    #ifdef MMFILEIN
        MDrv_MMFI_File_SetPacketMode(E_DRVMMFI_PTH_AUDIO, (DrvMMFI_PacketMode)pFileinInfo->PKT_Mode);
        MDrv_MMFI_File_SetTimer(E_DRVMMFI_PTH_AUDIO, (MS_U8)(pFileinInfo->Rate & 0xFF));
    #endif //MMFILEIN

    #ifdef MMFI_VD3D
        MDrv_MMFI_File_SetPacketMode(E_DRVMMFI_PATH_VD3D, (DrvMMFI_PacketMode)pFileinInfo->PKT_Mode);
        MDrv_MMFI_File_SetTimer(E_DRVMMFI_PATH_VD3D, (MS_U8)(pFileinInfo->Rate & 0xFF));
    #endif //MMFILEIN

    #endif //MMFI_VER_2_0
  #endif

    MDrv_TSP_File_Eng_SetPacketMode(eEng, (DrvTSP_PacketMode)(pFileinInfo->PKT_Mode));

    return (DRVTSP_OK == MDrv_TSP_File_Eng_SetRate(eEng, pFileinInfo->Rate))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;

#endif  // End of TSP_VER_4_0
}

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_Pause(DMX_FILEIN_PATH ePath)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_Pause();
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#else  // For TSP_VER_4_0

   TSP_FILE_ENG eEng = _DMX_ApiDrv_FileinEngMapping(ePath);

   if(eEng == E_TSP_FILE_ENG_INVALID)
       return DMX_FILTER_STATUS_ERROR;

   return (DRVTSP_OK == MDrv_TSP_File_Eng_Pause(eEng))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;

#endif  // End of TSP_VER_4_0
}

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_Resume(DMX_FILEIN_PATH ePath)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_Resume();
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#else  // For TSP_VER_4_0

    TSP_FILE_ENG eEng = _DMX_ApiDrv_FileinEngMapping(ePath);

    if(eEng == E_TSP_FILE_ENG_INVALID)
        return DMX_FILTER_STATUS_ERROR;

    return (DRVTSP_OK== MDrv_TSP_FILE_Eng_Resume(eEng))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;

#endif  // End of TSP_VER_4_0
}

MS_BOOL _MApi_DMX_Filein_Eng_IsIdle(DMX_FILEIN_PATH ePath)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_IsIdle();
    }
    else
    {
  #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))
        return DMX_FILTER_STATUS_ERROR;
  #else
        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_Filein_IsIdle(mmpath);
  #endif
    }
#else  // For TSP_VER_4_0

   DrvTSP_FileinState FileinState = E_DRVTSP_FILEIN_STATE_BUSY;
   TSP_FILE_ENG       eEng        = _DMX_ApiDrv_FileinEngMapping(ePath);

   if(eEng == E_TSP_FILE_ENG_INVALID)
       return DMX_FILTER_STATUS_ERROR;

   if (DRVTSP_OK!= MDrv_TSP_File_Eng_GetState(eEng, &FileinState))
   {
       DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Call MDrv_TSP_File_GetState fail\n", __LINE__)));
   }

   return (E_DRVTSP_FILEIN_STATE_IDLE== FileinState);

#endif  // End of TSP_VER_4_0
}

MS_BOOL _MApi_DMX_Filein_Eng_IsBusy(DMX_FILEIN_PATH ePath)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_IsBusy();
    }
    else
    {
  #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))
        return DMX_FILTER_STATUS_ERROR;
  #else
        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_Filein_IsBusy(mmpath);
  #endif
    }
#else  // For TSP_VER_4_0

    DrvTSP_FileinState FileinState = E_DRVTSP_FILEIN_STATE_IDLE;
    TSP_FILE_ENG       eEng        = _DMX_ApiDrv_FileinEngMapping(ePath);

    if(eEng == E_TSP_FILE_ENG_INVALID)
        return DMX_FILTER_STATUS_ERROR;

    if (DRVTSP_OK!= MDrv_TSP_File_Eng_GetState(eEng, &FileinState))
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Call MDrv_TSP_File_GetState fail\n", __LINE__)));
    }

    return (E_DRVTSP_FILEIN_STATE_BUSY == FileinState);

#endif  // End of TSP_VER_4_0
}

MS_BOOL _MApi_DMX_Filein_Eng_IsPause(DMX_FILEIN_PATH ePath)
{
   DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_IsPause();
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#else  // For TSP_VER_4_0

   DrvTSP_FileinState FileinState = E_DRVTSP_FILEIN_STATE_IDLE;
   TSP_FILE_ENG       eEng        = _DMX_ApiDrv_FileinEngMapping(ePath);

   if(eEng == E_TSP_FILE_ENG_INVALID)
        return DMX_FILTER_STATUS_ERROR;

   if (DRVTSP_OK!= MDrv_TSP_File_Eng_GetState(eEng, &FileinState))
   {
       DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Call MDrv_TSP_File_GetState fail\n", __LINE__)));
   }

   return (E_DRVTSP_FILEIN_STATE_PAUSE== FileinState);

#endif  // End of TSP_VER_4_0
}

// if we reset CMDQ when file in in busy .... filein hangs....
// @NOTE this is fixed in Kaiser
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_CMDQ_Reset(DMX_FILEIN_PATH ePath)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_CMDQ_Reset();
    }
    else
    {
  #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))
        return DMX_FILTER_STATUS_ERROR;
  #else
        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_Filein_CMDQ_Reset(mmpath);
  #endif
    }
#else  // For TSP_VER_4_0

    TSP_FILE_ENG  eEng = _DMX_ApiDrv_FileinEngMapping(ePath);

    if(eEng == E_TSP_FILE_ENG_INVALID)
        return DMX_FILTER_STATUS_ERROR;

    if (DRVTSP_OK == MDrv_TSP_File_Eng_CMDQ_Reset(eEng))
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#endif  // End of TSP_VER_4_0
}

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_CMDQ_GetEmptyNum(DMX_FILEIN_PATH ePath, MS_U32 *pu32EmptySlot)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_CMDQ_GetEmptyNum(pu32EmptySlot);
    }
    else
    {
  #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))
        return DMX_FILTER_STATUS_ERROR;
  #else
        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_Filein_CMDQ_GetEmptyNum(mmpath, pu32EmptySlot);
  #endif
    }
#else  // For TSP_VER_4_0

    TSP_FILE_ENG  eEng = _DMX_ApiDrv_FileinEngMapping(ePath);

    if(eEng == E_TSP_FILE_ENG_INVALID)
        return DMX_FILTER_STATUS_ERROR;

    if (DRVTSP_OK == MDrv_TSP_File_Eng_CMDQ_GetSlot(eEng, pu32EmptySlot))
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#endif  // End of TSP_VER_4_0
}

void _MApi_DMX_Filein_Eng_BypassFileInTimeStamp(DMX_FILEIN_PATH ePath, MS_BOOL bbypass)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    if(ePath == DMX_FILEIN_TSIF0)
    {
        _MApi_DMX_BypassFileInTimeStamp(bbypass);
    }
    else
    {
  #if (defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))

        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        _MApi_DMX_MMFI_Filein_BypassTimeStamp(mmpath, bbypass);
  #endif
    }
#else  // For TSP_VER_4_0

    TSP_FILE_ENG eEng = _DMX_ApiDrv_FileinEngMapping(ePath);
    MDrv_TSP_FILE_Eng_192BlockScheme_En(eEng, !bbypass);

#endif  // End of TSP_VER_4_0
}

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_CMDQ_FIFOWriteLevel(DMX_FILEIN_PATH ePath, MS_U8 *pu8CmdQStatus)
{
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_CMDQ_FIFOWriteLevel(pu8CmdQStatus);
    }
    else
    {
  #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))
        return DMX_FILTER_STATUS_ERROR;
  #else
        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_Filein_CMDQ_FIFOWriteLevel(mmpath, pu8CmdQStatus);
  #endif
    }
#else  // For TSP_VER_4_0

    TSP_FILE_ENG eEng = _DMX_ApiDrv_FileinEngMapping(ePath);

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    if (DRVTSP_OK == MDrv_TSP_FILE_Eng_CmdQFifo_Status(eEng, pu8CmdQStatus))
    {
       return DMX_FILTER_STATUS_OK;
    }
    else
    {
       return DMX_FILTER_STATUS_ERROR;
    }
#endif  // End of TSP_VER_4_0
}

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_GetFileInTimeStamp(DMX_FILEIN_PATH ePath, MS_U32 *pu32FileInTS)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_GetFileInTimeStamp(pu32FileInTS);
    }
    else
    {
  #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))
        return DMX_FILTER_STATUS_ERROR;
  #else
        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_GetFileInTimeStamp(mmpath, pu32FileInTS);
  #endif
    }
#else  // For TSP_VER_4_0

    TSP_FILE_ENG  eEng = _DMX_ApiDrv_FileinEngMapping(ePath);

    if(eEng == E_TSP_FILE_ENG_INVALID)
        return DMX_FILTER_STATUS_ERROR;

    if (DRVTSP_OK == MDrv_TSP_FILE_Eng_GetFileInTimeStamp(eEng, pu32FileInTS))
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#endif  // End of TSP_VER_4_0
}

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_GetReadAddr(DMX_FILEIN_PATH ePath, MS_PHY* pphyead)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_GetReadAddr(pphyead);
    }
    else
    {
  #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))
        return DMX_FILTER_STATUS_ERROR;
  #else

        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return MDrv_MMFI_File_GetReadAddr(mmpath, pphyead);
  #endif
    }
#else  // For TSP_VER_4_0
    TSP_FILE_ENG eEng  = _DMX_ApiDrv_FileinEngMapping(ePath);

    if(eEng == E_TSP_FILE_ENG_INVALID)
        return DMX_FILTER_STATUS_ERROR;

    if (E_TSP_OK == MDrv_TSP_FILE_Eng_GetFileInCurReadAddr(eEng,pphyead))
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#endif  // End of TSP_VER_4_0
}

//-------------------------------------------------------------------------------------------------
/// Enable FileEng Playback time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_PlaybackTimeStampEnable(DMX_FILEIN_PATH ePath)
{
    DMX_PVRDBGMSG(DMX_DBG_PVR, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FILEIN);
        dmx_dbg_print("[DMX FileIn timestamp enable]\tPath[%d]\n", (MS_U32)ePath);
#endif

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Pvr_TimeStampEnable();
    }
    else
    {
  #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))
        return DMX_FILTER_STATUS_ERROR;
  #else
        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_TimeStampEnable(mmpath);
  #endif
    }
#else  // For TSP_VER_4_0
    TSP_FILE_ENG eEng  = _DMX_ApiDrv_FileinEngMapping(ePath);

    if(eEng == E_TSP_FILE_ENG_INVALID)
        return DMX_FILTER_STATUS_ERROR;

    return (DRVTSP_OK== MDrv_TSP_FILE_Eng_TimeStampEnablePlaybackStamp(eEng,TRUE))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
#endif // End of TSP_VER_4_0
}

//-------------------------------------------------------------------------------------------------
/// Disable FileEng Playback time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_PlaybackTimeStampDisable(DMX_FILEIN_PATH ePath)
{
    DMX_PVRDBGMSG(DMX_DBG_PVR, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_FILEIN);
        dmx_dbg_print("[DMX FileIn timestamp disable]\tPath[%d]\n", (MS_S32)ePath);
#endif

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Pvr_TimeStampDisable();
    }
    else
    {
  #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))
        return DMX_FILTER_STATUS_ERROR;
  #else
        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_TimeStampDisable(mmpath);

  #endif
    }
#else  // For TSP_VER_4_0
    TSP_FILE_ENG eEng = _DMX_ApiDrv_FileinEngMapping(ePath);

    if(eEng == E_TSP_FILE_ENG_INVALID)
        return DMX_FILTER_STATUS_ERROR;

    return (DRVTSP_OK== MDrv_TSP_FILE_Eng_TimeStampEnablePlaybackStamp(eEng,FALSE))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
#endif // End of TSP_VER_4_0
}

//-------------------------------------------------------------------------------------------------
/// Set playback time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_SetPlaybackStamp(DMX_FILEIN_PATH ePath,MS_U32 u32Stamp)
{
    DMX_PVRDBGMSG(DMX_DBG_PVR, ULOGD("DMX", "[%s] %d (%u)\n", __FUNCTION__, __LINE__, (unsigned int)u32Stamp));

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Pvr_SetPlaybackStamp(u32Stamp);
    }
    else
    {
  #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))
        return DMX_FILTER_STATUS_ERROR;
  #else
        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_SetPlaybackTimeStamp(mmpath, u32Stamp);
  #endif
    }
#else  // For TSP_VER_4_0
    TSP_FILE_ENG eEng = _DMX_ApiDrv_FileinEngMapping(ePath);

    if(eEng == E_TSP_FILE_ENG_INVALID)
        return DMX_FILTER_STATUS_ERROR;

    return (DRVTSP_OK== MDrv_TSP_FILE_Eng_TimeStampSetPlaybackStamp(eEng,u32Stamp))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
#endif // End of TSP_VER_4_0
}

//-------------------------------------------------------------------------------------------------
/// Get playback time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_GetPlaybackStamp(DMX_FILEIN_PATH ePath,MS_U32* pu32Stamp)
{
    DMX_PVRDBGMSG(DMX_DBG_PVR, ULOGD("DMX", "[%s] %d (0x%p)\n", __FUNCTION__, __LINE__, pu32Stamp));

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Pvr_GetPlaybackStamp(pu32Stamp);
    }
    else
    {
  #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))
        return DMX_FILTER_STATUS_ERROR;
  #else
        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_GetPlaybackTimeStamp(mmpath, pu32Stamp);
  #endif
    }
#else  // For TSP_VER_4_0

    TSP_FILE_ENG eEng = _DMX_ApiDrv_FileinEngMapping(ePath);

    if(eEng == E_TSP_FILE_ENG_INVALID)
        return DMX_FILTER_STATUS_ERROR;

    return (DRVTSP_OK== MDrv_TSP_FILE_Eng_TimeStampGetPlaybackStamp(eEng,pu32Stamp))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
#endif // End of TSP_VER_4_0
}

DMX_FILTER_STATUS _MApi_DMX_Filein_Eng_MOBF_Enable(DMX_FILEIN_PATH ePath, MS_BOOL bEnable, MS_U32 u32key)
{
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    if(ePath == DMX_FILEIN_TSIF0)
    {
        return _MApi_DMX_Filein_MOBF_Enable(bEnable, u32key);
    }
    else
    {
  #if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))
        return DMX_FILTER_STATUS_ERROR;
  #else
        DMX_MMFI_PATH mmpath = DMX_MMFI_PATH0;

        if(ePath == DMX_FILEIN_TSIF2)
            mmpath = DMX_MMFI_PATH1;

        return _MApi_DMX_MMFI_MOBF_Enable(mmpath, bEnable, u32key);
  #endif
    }
#else  // For TSP_VER_4_0
    DMX_FILTER_STATUS eRet;

    TSP_FILE_ENG eEng = _DMX_ApiDrv_FileinEngMapping(ePath);

    if(eEng == E_TSP_FILE_ENG_INVALID)
        return DMX_FILTER_STATUS_ERROR;

    _DMX_ENTRY();
    eRet = MDrv_TSP_FILE_Eng_MOBF_Enable(eEng, bEnable, u32key);
    _DMX_RETURN(eRet);
#endif // End of TSP_VER_4_0
}

//-------------------------------------------------------------------------------------------------
/// Allocate DMX resource
/// @param eResType      \b IN: Resource type to allocate
/// @param pRes             \b OUT: Pointer to store resource content
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
/// Please use below resource content for the such resource item
/// DMX_RES_STC_ENG: MS_U32 u32STCEng
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_ResAllocate(DMX_RES_TYPE eResType, void *pRes)
{
#if(!(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0)))  // For TSP_VER_3_0 and TSP_VER_4_0

    TSP_Result  eRet = DRVTSP_OK;
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    switch(eResType)
    {
        case DMX_RES_STC_ENG:
            eRet = MDrv_TSP_STC_Alloc((MS_U32*)pRes);
            break;
    #if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0

        case DMX_RES_FLOW_PB_LIVE:
        case DMX_RES_FLOW_PB_FILE:
            {
                MS_U32  u32PathId = 0;
                DRV_TSP_PATH_TYPE   eDrvTspPathType = (eResType == DMX_RES_FLOW_PB_LIVE)? E_DRV_TSP_PATH_LIVE : E_DRV_TSP_PATH_FILE;

                eRet = MDrv_TSP_Path_Alloc(eDrvTspPathType, &u32PathId);
                _DMX_ApiDrv_DmxFlowReverseMapping((TSP_TSIF)u32PathId, pRes);
            }
            break;
        case DMX_RES_FLOW_MMFI:
            {
                MS_U32  u32PathId = 0;

                eRet = (MDrv_MMFI_Path_Alloc(&u32PathId) == DRVMMFI_OK)? DMX_FILTER_STATUS_OK : DMX_FILTER_STATUS_ERROR;

                switch((DrvMMFI_Path)u32PathId)
                {
                    case E_DRVMMFI_PATH0:
                        *((MS_U32*)pRes) = DMX_FLOW_MMFI0;
                        break;
                    case E_DRVMMFI_PATH1:
                        *((MS_U32*)pRes) = DMX_FLOW_MMFI1;
                        break;
                    default:
                        DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[%s] %d  Type not support !!\n", __FUNCTION__, __LINE__));
                        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
                }
            }
            break;

    #endif  // End of TSP_VER_4_0
        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGD("DMX", "[%s] %d  Type not support !!\n", __FUNCTION__, __LINE__));
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if(eRet == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else if(eRet == DRVTSP_NOT_SUPPORTED)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_NOT_SUPPORT);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else
    return DMX_FILTER_STATUS_NOT_SUPPORT;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Free  DMX resource
/// @param eResType      \b IN: Resource type to free
/// @param pRes             \b IN: Pointer to store resource content
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
/// Please use below resource content for the such resource item
/// DMX_RES_STC_ENG: MS_U32 u32STCEng
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_ResFree(DMX_RES_TYPE eResType, void *pRes)
{
#if(!(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0)))  // For TSP_VER_3_0 and TSP_VER_4_0

    TSP_Result  eRet = DRVTSP_OK;
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    switch(eResType)
    {
        case DMX_RES_STC_ENG:
            eRet = MDrv_TSP_STC_Free(*((MS_U32*)pRes));
            break;
    #if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0

        case DMX_RES_FLOW_PB_LIVE:
        case DMX_RES_FLOW_PB_FILE:
            {
                DRV_TSP_PATH_TYPE   eDrvTspPathType = (eResType == DMX_RES_FLOW_PB_LIVE)? E_DRV_TSP_PATH_LIVE : E_DRV_TSP_PATH_FILE;
                MS_U32  u32PathId = 0;

                _DMX_ApiDrv_DmxFlowMapping(*((MS_U32*)pRes), &u32PathId);
                eRet = MDrv_TSP_Path_Free(eDrvTspPathType, (TSP_TSIF)u32PathId);
            }
            break;
        case DMX_RES_FLOW_MMFI:
            {
                DrvMMFI_Path    ePath = E_DRVMMFI_PATH0;

                switch(*(MS_U32*)pRes)
                {
                    case DMX_FLOW_MMFI0:
                        ePath = E_DRVMMFI_PATH0;
                        break;
                    case DMX_FLOW_MMFI1:
                        ePath = E_DRVMMFI_PATH1;
                        break;
                    default:
                        DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[%s] %d  Type not support !!\n", __FUNCTION__, __LINE__));
                        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
                }

                eRet = MDrv_MMFI_Path_Free(ePath);
            }
            break;

    #endif  // End of TSP_VER_4_0
        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGD("DMX", "[%s] %d  Type not support !!\n", __FUNCTION__, __LINE__));
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if(eRet == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else if(eRet == DRVTSP_NOT_SUPPORTED)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_NOT_SUPPORT);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else
    return DMX_FILTER_STATUS_NOT_SUPPORT;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Get STC engine which PCR PID index mapping to
/// @param PcrFltId                 \b IN: PCR PID index
/// @param u32StcEng             \b OUT: STC engine id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pcr_Get_MapSTC(MS_U32 u32PcrFltId, MS_U32 *pu32StcEng)
{
#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
    DMX_FILTER_STATUS ret = DMX_FILTER_STATUS_ERROR;
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    if(MDrv_TSP_PcrId_To_StcId(u32PcrFltId, pu32StcEng) == DRVTSP_OK)
    {
        ret = DMX_FILTER_STATUS_OK;
    }

    _DMX_RETURN(ret);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Get STC counter
/// @param u8Eng \b IN: STC Engine ID
/// @param pu32Stc32 \b OUT: STC counter (bit 32)
/// @param pu32Stc \b OUT: STC counter (bit 31 to bit 0)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Stc_Eng_Get(MS_U8 u8Eng, MS_U32* pu32Stc32, MS_U32* pu32Stc)
{
    DMX_FILTER_STATUS ret;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();
    ret = (DRVTSP_OK== MDrv_TSP_GetSTC(u8Eng, pu32Stc32, pu32Stc)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
    _DMX_RETURN(ret);
}

//-------------------------------------------------------------------------------------------------
/// Set STC counter
/// @param u8Eng \b IN: STC Engine ID
/// @param u32Stc32 \b IN: STC counter (bit 32)
/// @param u32Stc \b IN: STC counter (bit 31 to bit 0)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Stc_Eng_Set(MS_U8 u8Eng, MS_U32 u32Stc32, MS_U32 u32Stc)
{
    DMX_FILTER_STATUS ret;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_STC_CHANGE);
        dmx_dbg_print("[DMX STC]\teng[%d] STC[0x%x]\n", (MS_S32)u8Eng, u32Stc);
#endif

    _DMX_ENTRY();
    ret = (DRVTSP_OK== MDrv_TSP_SetSTC(u8Eng, u32Stc32, u32Stc))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
    _DMX_RETURN(ret);
}

//-------------------------------------------------------------------------------------------------
/// Add or subtract STC Offset
/// @param u8Eng                      \b IN: STC Engine ID
/// @param u32Offset                  \b IN: STC offset value
/// @param bAdd                       \b IN:  If TRUE, add the offset value; otherwise subtract the offset value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Stc_Eng_SetOffset(MS_U32 u32Eng, MS_U32 u32Offset, MS_BOOL bAdd)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if (defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0)))

    if(bAdd == FALSE)  //Not support subtracting yet
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#endif

    _DMX_ENTRY();
    if(MDrv_TSP_SetSTCOffset(u32Eng, u32Offset, bAdd) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
}

//-------------------------------------------------------------------------------------------------
/// Set STC control
/// @param u8Eng \b         IN: STC Engine ID
/// @param eMode \b         IN: STC update control options
///                                         0x00 -> Update STC by TSP FW
///                                         0x01 -> Update STC by HK
///                                         0x02 -> Update STC Once when PCR reset
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_STC_UpdateCtrl(MS_U8 u8Eng, eStcUpdateCtrlMode eMode)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    if (MDrv_TSP_STC_UpdateCtrl(u8Eng, eMode) == DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }
    return DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Adjust STC Clock
/// @param u32EngId                     \b IN: STC Engine ID
/// @param bUpClk                         \b IN: If TRUE, Clk will be faster, and if FALSE, Clk will be slower
/// @param u32Percentage             \b IN: The percentage of STC clock ratio
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Stc_Clk_Adjust(MS_U32 u32EngId, MS_BOOL bUpClk, MS_U32 u32Percentage)
{
    DMX_FILTER_STATUS ret;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();
    ret = (DRVTSP_OK== MDrv_TSP_STCClk_Adjust(u32EngId, bUpClk, u32Percentage))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
    _DMX_RETURN(ret);
}

//-------------------------------------------------------------------------------------------------
/// Select STC Engine (Only support in driver version 3.0)
/// @param eFltSrc                                 \b IN: Fliter Source type and Filter source ID
/// @param u32StcEng                           \b IN: STC engine ID (0, 1)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Stc_Select(DMX_FILTER_TYPE eFltSrc, MS_U32 u32StcEng)
{
#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))

    DMX_FILTER_STATUS ret;
    DrvTSP_FltType eFltType;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    switch(eFltSrc & DMX_FILTER_SOURCE_TYPE_MASK)
    {
        case DMX_FILTER_SOURCE_TYPE_LIVE:
            eFltType = E_DRVTSP_FLT_SOURCE_TYPE_LIVE;
            break;
        case DMX_FILTER_SOURCE_TYPE_FILE:
            eFltType = E_DRVTSP_FLT_SOURCE_TYPE_FILE;
            break;
        case DMX_FILTER_SOURCE_TYPE_TS1:
            eFltType = E_DRVTSP_FLT_SOURCE_TYPE_TS1;
            break;
        case DMX_FILTER_SOURCE_TYPE_TS2:
            eFltType = E_DRVTSP_FLT_SOURCE_TYPE_TS2;
            break;
        case DMX_FILTER_SOURCE_TYPE_FILE1:
            eFltType = E_DRVTSP_FLT_SOURCE_TYPE_FILE1;
            break;
        case DMX_FILTER_SOURCE_TYPE_FILE2:
            eFltType = E_DRVTSP_FLT_SOURCE_TYPE_FILE2;
            break;
        default:
            return DMX_FILTER_STATUS_ERROR;
    }

    eFltType |= (eFltSrc & DMX_FILTER_SOURCEID_MASK);

    _DMX_ENTRY();
    ret = (DRVTSP_OK== MDrv_TSP_STC_Select(eFltType, u32StcEng)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
    _DMX_RETURN(ret);

#else

    return DMX_FILTER_STATUS_ERROR;

#endif

}

//-------------------------------------------------------------------------------------------------
/// Get PCR
/// @param pu32Pcr32 \b OUT: PCR counter (bit 32)
/// @param pu32Pcr \b OUT: PCR counter (bit 31 to bit 0)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pcr_Get(MS_U32* pu32Pcr32, MS_U32* pu32Pcr)
{
    DMX_FILTER_STATUS ret;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();
    ret = (DRVTSP_OK== MDrv_TSP_GetPCR(0, pu32Pcr32, pu32Pcr)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
    _DMX_RETURN(ret);
}

//-------------------------------------------------------------------------------------------------
/// Get PCR
/// @param u32PcrEng \b INPUT: PCR Eng
/// @param pu32Pcr32 \b OUT: PCR counter (bit 32)
/// @param pu32Pcr \b OUT: PCR counter (bit 31 to bit 0)
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pcr_Eng_Get(MS_U8 u8PcrEng, MS_U32* pu32Pcr32, MS_U32* pu32Pcr)
{
    DMX_FILTER_STATUS ret;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    ret = (DRVTSP_OK== MDrv_TSP_GetPCR(u8PcrEng, pu32Pcr32, pu32Pcr)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);

    _DMX_RETURN(ret);
}

//--------------------------------------------------------------------------------------------------
// Ask demuxer to process section/pvr data for Non-OS driver. Calling this function in OS environment causes nothing
// @param  u32FltId                 \b IN: Which filter id to process. 0xFFFFFFFF means process all filters
// @param  u32Timeout               \b IN: Max time for TSP to process
// @return DMX_FILTER_STATUS
// @note
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Proc(MS_U32 u32DmxId, DMX_EVENT* pEvent)      // for non-OS TSP scheduling
{
    TSP_Result bRet = (TSP_Result)DRVTSP_FAIL;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    if (0xFFFFFFFF != u32DmxId)
    {
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
        MDrv_TSP_Proc(0, u32DmxId, &bRet, (DrvTSP_Event*)pEvent);
#else
        bRet = MDrv_TSP_Proc(0, u32DmxId, (TSP_Event*)pEvent);
#endif
    }

    if (DRVTSP_OK == bRet)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
}

//-------------------------------------------------------------------------------------------------
// Invert Demux parallel input bit order
// @param DmxFlow \b IN: DMX_FLOW_PLAYBACK for playback flow
//                       DMX_FLOW_PVR for record flow
// @param bInvert \b IN: Invert bit order of TS parallel in
//                            TRUE for Invert
//                            FALSE for not invert
// @return DMX_FILTER_STATUS_OK - Success
// @return DMX_FILTER_STATUS_ERROR - Failure
// @note
//------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Parl_Invert(DMX_FLOW DmxFlow, MS_BOOL bInvert)
{
    TSP_Result          ret = (TSP_Result)DRVTSP_FAIL;

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
    DrvTSP_If           TspIf = E_DRVTSP_IF_NUM;
#endif

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

#if(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
    switch(DmxFlow)
    {
        case DMX_FLOW_PVRCA:
        case DMX_FLOW_PLAYBACK:
            TspIf = E_DRVTSP_IF_TS0;
            break;
        case DMX_FLOW_PLAYBACK1:
        case DMX_FLOW_PVR:
            TspIf = E_DRVTSP_IF_TS1;
            break;
        case DMX_FLOW_PVRCA1:
        case DMX_FLOW_PVR1:
            TspIf = E_DRVTSP_IF_TS2;
            break;
        case DMX_FLOW_PVR2:
            TspIf = E_DRVTSP_IF_TS3;
            break;
        default:
            MS_ASSERT(0);
            break;
    }
    ret = MDrv_TSP_Parl_BitOrderSwap(0, TspIf, bInvert);

#elif(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))
    switch(DmxFlow)
    {
        case DMX_FLOW_PLAYBACK:
            TspIf = E_DRVTSP_IF_PLAYBACK;
            break;
        case DMX_FLOW_PVR:
            TspIf = E_DRVTSP_IF_PVR0;
            break;
        default:
            MS_ASSERT(0);
            break;
    }
    ret = MDrv_TSP_Parl_BitOrderSwap(0, TspIf, bInvert);

#else  // For TSP_VER_4_0

    MS_U32 u32TsIf = 0;

    if(_DMX_ApiDrv_DmxFlowMapping(DmxFlow, &u32TsIf) == FALSE)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    TSP_TSIF eTSPTsif = MDrv_TSP_TsifMapping((TSP_TSIF)u32TsIf);
    ret = MDrv_TSP_Parl_BitOrderSwap(0, eTSPTsif, bInvert);

#endif  // End of TSP_VER_4_0

    if(DRVTSP_OK == ret)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
}

//--------------------------------------------------------------------------------------------------
/// Copy section data to assigned buffer
/// @param  u32DmxId                   \b IN: demux ID
/// @param  pu8Buf                      \b OUT: section buffer address
/// @param  u32BufSize                \b IN: section buffer size
/// @param  pu32ActualSize          \b IN: section data size
/// @param  pu32RmnSize             \b OUT: section buffer remainning data size
/// @param  pfCheckCB               \b IN: callback function to check data correct or not
/// @return DMX_FILTER_STATUS
/// @note
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_CopyData(MS_U32 u32DmxId, MS_U8* pu8Buf, MS_U32 u32BufSize, MS_U32* pu32ActualSize, MS_U32* pu32RmnSize, DMX_CheckCb pfCheckCB)
{
    DMX_FILTER_TYPE        DmxFltType;
    DMX_FILTER_STATUS      ret = DMX_FILTER_STATUS_ERROR;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));
    DMX_ASSERT(DMX_MAX_FLTID> u32DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, (int)u32DmxId)));

    _DMX_ENTRY();

    if(_pdmx_res->_FltList[u32DmxId].FltId == 0xFFFFFFFF)
    {
        //DMX_DBGMSG(DMX_DBG_ERR, ULOGD("DMX", "[%s][%d] Filte %d is already free\n", __FUNCTION__, __LINE__, (int)u8DmxId));
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _FLT_LIST_TYPE_GET(u32DmxId, &DmxFltType);
    switch (DmxFltType & (~DMX_FILTER_FLT_MASK))
    {
    case DMX_FILTER_TYPE_SECTION:
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
    case DMX_FILTER_TYPE_SECTION_NO_PUSI:
    case DMX_FILTER_TYPE_SECTION_VER:
#endif
        ret = _DMX_GetSect(u32DmxId, pu8Buf, u32BufSize, pu32ActualSize, pu32RmnSize, pfCheckCB);
        break;
    case DMX_FILTER_TYPE_PES:
    case DMX_FILTER_TYPE_TELETEXT:
    case DMX_FILTER_TYPE_PACKET:
        ret = _DMX_GetRingBuffer(u32DmxId, pu8Buf, u32BufSize, pu32ActualSize, pu32RmnSize, pfCheckCB);
        break;
    default:
        MS_ASSERT(0);
        break;
    }

#if DMX_DEBUG
    {
        MS_BOOL bFltMonitor = FALSE;
        MS_U32 u32TmpS, u32TmpSize, u32TmpR, u32TmpW;
        dmx_dbg_IsFltMonitor(u32DmxId, bFltMonitor);
        MS_U32 i=0;

        if((u32DbgLogFlag & (DMX_DBG_LOG_COPYDATA | DMX_DBG_LOG_COPYDATA1)) && bFltMonitor)
        {
            MDrv_TSP_SecFlt_GetBufStart(0, _pdmx_res->_FltList[u32DmxId].SecBufId, &u32TmpS);
            MDrv_TSP_SecFlt_GetBufSize(0, _pdmx_res->_FltList[u32DmxId].SecBufId, &u32TmpSize);
            MDrv_TSP_SecFlt_GetReadAddr(0, _pdmx_res->_FltList[u32DmxId].SecBufId, &u32TmpR);
            MDrv_TSP_SecFlt_GetWriteAddr(0, _pdmx_res->_FltList[u32DmxId].SecBufId, &u32TmpW);

            dmx_dbg_print("[DMX CpData]\tFlt[%03d] Copied[%04d] Rmn[0x%08X] S[0x%08X] E[0x%08X] R[0x%08X] W[0x%08X]\n",
            (MS_S32)u32DmxId,
            (MS_S32)(*pu32ActualSize),
            (MS_S32)(*pu32RmnSize),
            u32TmpS,
            (u32TmpS + u32TmpSize),
            u32TmpR,
            u32TmpW);
        }

        if((u32DbgLogFlag & DMX_DBG_LOG_COPYDATA1) && bFltMonitor)
        {
            if(pfSEC)
            {
                if(*pu32ActualSize != fwrite(pu8Buf, 1, *pu32ActualSize, pfSEC))
                {
                    dmx_dbg_print("write file error, length %d\n", *pu32ActualSize);
                    fclose(pfSEC);
                    pfSEC = NULL;
                }
            }
            else
            {
                ULOGD("DMX", "[dump]\t");
                for(;i<MAX_NUM_CP_DATA_DUMP;i++)
                {
                    if(i%16 == 0)
                        ULOGD("DMX", "\n\t\t");
                    ULOGD("DMX", "%02X ", pu8Buf[i]);
                }
                ULOGD("DMX", "\n");
            }
        }
    }
#endif

    _DMX_RETURN(ret);
}

void _MApi_DMX_WriteProtect_Enable(MS_BOOL bEnable, MS_PHY* pphyStartAddr, MS_PHY* pphyEndAddr)
{
    MDrv_TSP_WriteProtect_Enable(bEnable, pphyStartAddr, pphyEndAddr);
}

void _MApi_DMX_OrzWriteProtect_Enable(MS_BOOL bEnable, MS_PHY phyStartAddr, MS_PHY phyEndAddr)
{
    MDrv_TSP_OrzWriteProtect_Enable(bEnable, phyStartAddr, phyEndAddr);
}

DMX_FILTER_STATUS _MApi_DMX_FlowEnable(DMX_FLOW DmxFlow, MS_BOOL bEnable)
{
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    DrvTSP_If e_TsIf;

    _DMX_ENTRY();

    switch(DmxFlow)
    {
  #if((defined(TSP_VERSION)) && (TSP_VERSION == TSP_VER_3_0))
        case DMX_FLOW_PLAYBACK:
            e_TsIf = E_DRVTSP_IF_TS0;
            break;
        case DMX_FLOW_PVR:
            e_TsIf = E_DRVTSP_IF_TS0;
            break;
        case DMX_FLOW_FILEIN_MM:
            e_TsIf = E_DRVTSP_IF_TS1;
            break;

  #else
        case DMX_FLOW_PLAYBACK:
            e_TsIf = E_DRVTSP_IF_PLAYBACK;
            break;
        case DMX_FLOW_PVR:
            e_TsIf = E_DRVTSP_IF_PVR0;
            break;
        case DMX_FLOW_FILEIN_MM:
            e_TsIf = E_DRVTSP_IF_PVR1;
            break;
  #endif

        case DMX_FLOW_PVRCA:
        case DMX_FLOW_FILEIN_MM3D:
        case DMX_FLOW_NUM:
        default:
             _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }

    MDrv_TSP_TSIF_Enable(e_TsIf , bEnable);
    _DMX_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set/Get DScmbler engine id depend on TSIF path
/// @param  eTsIf                             \b IN: eTsIf
/// @param  pu32EngId                      \b IN/OUT: pointer to store dscmb engine id
/// @param  bSet                              \b IN: TRUE:Set dscmb engine id ; FALSE: Get dscmb engine id
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Flow_DscmbEng(DMX_TSIF eTsIf, MS_U32* pu32EngId, MS_BOOL bSet)
{
#if(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))

    DrvTSP_PKTDMXSrcType ePktDmxSrc = 0;

    switch(eTsIf)
    {
        case DMX_TSIF_LIVE0:
            ePktDmxSrc = E_DRVTSP_PKTSRC_DEMUX0;
            break;
        case DMX_TSIF_LIVE1:
        case DMX_TSIF_FILE1:
            ePktDmxSrc = E_DRVTSP_PKTSRC_DEMUX1;
            break;
        case DMX_TSIF_LIVE2:
        case DMX_TSIF_FILE2:
            ePktDmxSrc = E_DRVTSP_PKTSRC_DEMUX2;
            break;
        case DMX_TSIF_FILE0:
            ePktDmxSrc = E_DRVTSP_PKTSRC_DEMUXFILE;
            break;
        default:
            *pu32EngId = 0xFF;
            return DMX_FILTER_STATUS_ERROR;
    }

    _DMX_ENTRY();
    if(bSet == FALSE)
    {
        *pu32EngId = 0xFF;
        MDrv_TSP_GetDscmbEngIdx_BySource(ePktDmxSrc, pu32EngId);
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        if(MDrv_TSP_Dscmb_Source(*pu32EngId, &ePktDmxSrc, TRUE) == DRVTSP_OK)
        {
            _DMX_RETURN(DMX_FILTER_STATUS_OK);
        }
        else
        {
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
        }
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set enable/disable drop scramble packet of fifo/pvr
/// @param  DmxFltType                   \b IN: DmxFltType
/// @param  bEnable                         \b IN: Enable
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_DropScmbPkt(DMX_FILTER_TYPE DmxFltType, MS_BOOL bEnable)
{
#if (!(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0)))  // For TSP_VER_3_0 and TSP_VER_4_0

    DrvTSP_FltType _Flt_type;
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    switch (DmxFltType)
    {
        case DMX_FILTER_TYPE_VIDEO:
            _Flt_type = E_DRVTSP_FLT_TYPE_VIDEO;
            break;
        case DMX_FILTER_TYPE_VIDEO3D:
            _Flt_type = E_DRVTSP_FLT_TYPE_VIDEO3D;
            break;
    #if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
        case DMX_FILTER_TYPE_VIDEO3:
            _Flt_type = E_DRVTSP_FLT_TYPE_VIDEO3;
            break;
        case DMX_FILTER_TYPE_VIDEO4:
            _Flt_type = E_DRVTSP_FLT_TYPE_VIDEO4;
            break;
        case DMX_FILTER_TYPE_VIDEO5:
            _Flt_type = E_DRVTSP_FLT_TYPE_VIDEO5;
            break;
        case DMX_FILTER_TYPE_VIDEO6:
            _Flt_type = E_DRVTSP_FLT_TYPE_VIDEO6;
            break;
        case DMX_FILTER_TYPE_VIDEO7:
            _Flt_type = E_DRVTSP_FLT_TYPE_VIDEO7;
            break;
        case DMX_FILTER_TYPE_VIDEO8:
            _Flt_type = E_DRVTSP_FLT_TYPE_VIDEO8;
            break;
    #endif
        case DMX_FILTER_TYPE_AUDIO:
            _Flt_type = E_DRVTSP_FLT_TYPE_AUDIO;
            break;
        case DMX_FILTER_TYPE_AUDIO2:
            _Flt_type = E_DRVTSP_FLT_TYPE_AUDIO2;
            break;
        case DMX_FILTER_TYPE_AUDIO3:
            _Flt_type = E_DRVTSP_FLT_TYPE_AUDIO3;
            break;
        case DMX_FILTER_TYPE_AUDIO4:
            _Flt_type = E_DRVTSP_FLT_TYPE_AUDIO4;
            break;
    #if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
        case DMX_FILTER_TYPE_AUDIO5:
            _Flt_type = E_DRVTSP_FLT_TYPE_AUDIO5;
            break;
        case DMX_FILTER_TYPE_AUDIO6:
            _Flt_type = E_DRVTSP_FLT_TYPE_AUDIO6;
            break;
    #endif
        case DMX_FILTER_SOURCE_TYPE_PVR0:
            _Flt_type = E_DRVTSP_FLT_TYPE_PVR;
            break;
        case DMX_FILTER_SOURCE_TYPE_PVR1:
            _Flt_type = E_DRVTSP_FLT_TYPE_PVR1;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX",  "Incorrect type for DropScmbPkt\n"));
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (MDrv_TSP_DropScmbPkt(_Flt_type, bEnable) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    _DMX_RETURN(DMX_FILTER_STATUS_ERROR);

#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Configure output pad
/// @param  eOutPad                         \b IN: Select output pad
/// @param  eInSrcPad                       \b IN: Select input source pad
/// @param  bInParallel                       \b IN: TRUE: input source is paralleled; FALSE: input source is serial in
/// @param  u32ResvNum                    \b IN: Reserved value of the number of reserved parameters for future using
/// @param  pu32vRes                        \b IN: Reserved pointer to store parameters for future using
/// @note
/// Only EXT_PAD1 and EXT_PAD3 has output mode
///
/// If output from S2P mode, input source should be external input pad only
/// If output from TSO mode, eInputPad should be E_DRVTSP_PAD_TSO
/// If input source is internal demod, output mode can be demod out or TSO out
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TsOutputPadCfg(DMX_FLOW_OUTPUT_PAD eOutPad, DMX_FLOW_INPUT eInSrcPad, MS_BOOL bInParallel, MS_U32 u32ResvNum, MS_U32 *pu32Resv)
{
#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))

    DrvTSP_PadIn eInPad;
    MS_BOOL bEnable = TRUE;
    DrvTSP_PadOut eDrvOutPad = E_DRVTSP_OUTPAD_NONE;

    switch(eOutPad)
    {
        case DMX_FLOW_OUTPUT_EXT_PAD1:
            eDrvOutPad = E_DRVTSP_PAD_EXT_INPUT1;
            break;
        case DMX_FLOW_OUTPUT_EXT_PAD3:
            eDrvOutPad = E_DRVTSP_PAD_EXT_INPUT3;
            break;
        default:
            return DMX_FILTER_STATUS_ERROR;
    }

    switch(eInSrcPad)
    {
        case DMX_FLOW_INPUT_DEMOD:
            eInPad = E_DRVTSP_PAD_DEMOD;
            eDrvOutPad |= E_DRVTSP_OUTPAD_FROM_DMD;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT0:
            eInPad = E_DRVTSP_PAD_EXT_INPUT0;
            eDrvOutPad |= E_DRVTSP_OUTPAD_FROM_S2P;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT1:
            eInPad = E_DRVTSP_PAD_EXT_INPUT1;
            eDrvOutPad |= E_DRVTSP_OUTPAD_FROM_S2P;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT2:
            eInPad = E_DRVTSP_PAD_EXT_INPUT2;
            eDrvOutPad |= E_DRVTSP_OUTPAD_FROM_S2P;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT3:
            eInPad = E_DRVTSP_PAD_EXT_INPUT3;
            eDrvOutPad |= E_DRVTSP_OUTPAD_FROM_S2P;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT4:
            eInPad = E_DRVTSP_PAD_EXT_INPUT4;
            eDrvOutPad |= E_DRVTSP_OUTPAD_FROM_S2P;
            break;
        case DMX_FLOW_INPUT_EXT_INPUT5:
            eInPad = E_DRVTSP_PAD_EXT_INPUT5;
            eDrvOutPad |= E_DRVTSP_OUTPAD_FROM_S2P;
            break;
        case DMX_FLOW_INPUT_TSO:
            eInPad = E_DRVTSP_PAD_TSO;
            eDrvOutPad |= E_DRVTSP_OUTPAD_FROM_TSO;
            break;
        case DMX_FLOW_INPUT_DISABLE:
            eInPad = E_DRVTSP_PAD_DEMOD;
            bEnable = FALSE;
            break;
        default:
            return DMX_FILTER_STATUS_ERROR;
    }

    _DMX_ENTRY();

    if(MDrv_TSP_OutputPadCfg(eDrvOutPad, eInPad, bInParallel, bEnable) != DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Configure output pad
/// @param  u16Val                           \b IN: Value of phase
/// @param  bEnable                          \b IN: Enable/Disable phase tuning
/// @param  u32S2pOpt                      \b IN: Option value
/// @note
/// Configure for S2P mode
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TsS2POutputClkPhase(MS_U16 u16Val, MS_BOOL bEnable, MS_U32 u32S2pOpt)
{
#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))

    _DMX_ENTRY();
    if(MDrv_TSP_OutputClkPhase(0, u16Val, bEnable, u32S2pOpt) != DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif

}

DMX_FILTER_STATUS _MApi_DMX_Read_DropPktCnt(MS_U16* pu16ADropCnt, MS_U16* pu16VDropCnt)
{
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    DMX_FILTER_STATUS ret;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();
    ret = DRVTSP_OK== MDrv_TSP_ReadDropPktCnt(pu16ADropCnt, pu16VDropCnt)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR;
    _DMX_RETURN(ret);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//--------------------------------------------------------------------------------------------------
// Set demux filter owner
// @param  u8DmxIdStart        \b IN: demux start ID
// @param  u8DmxIdEnd          \b IN: demux end ID
// @param  bOwner                \b IN: TRUE for owner, FALSE for not owner
// @return DMX_FILTER_STATUS
// @note
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SetOwner(MS_U32 u32DmxIdStart, MS_U32 u32DmxIdEnd, MS_BOOL bOwner)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0))
    if (DRVTSP_OK != MDrv_TSP_Flt_SetOwner(0, u32DmxIdStart, u32DmxIdEnd, bOwner))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#endif

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

//--------------------------------------------------------------------------------------------------
// Get demux filter owner
// @param  u8DmxIdStart        \b IN: demux ID
// @param  bOwner                \b IN: pointer to store return value of owner flag
// @return DMX_FILTER_STATUS
// @note
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_GetOwner(MS_U32 u32DmxId, MS_BOOL* pbOwner)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0))
    *pbOwner = ((DRVTSP_OK == MDrv_TSP_PidFlt_ChkOwner(0, u32DmxId))? TRUE: FALSE);
#else
    *pbOwner = (DRVTSP_OK == MDrv_TSP_FLT_ChkOwner(_Owner, u32DmxId)) ? ((_Owner)?TRUE:FALSE) : ((_Owner)?FALSE:TRUE);
#endif

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

DMX_FILTER_STATUS _MApi_DMX_GetAccess(MS_U32 u32Try)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    return ((DRVTSP_OK == MDrv_TSP_IsAccess(u32Try))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
}

DMX_FILTER_STATUS _MApi_DMX_ReleaseAccess(void)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    return ((DRVTSP_OK == MDrv_TSP_UnlockAccess())? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
}

DMX_FILTER_STATUS _MApi_DMX_SetHK(MS_BOOL bIsHK)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _pdmx_res->_u32IsHK = bIsHK;

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0))
    return ((DRVTSP_OK == MDrv_TSP_SetHK(bIsHK))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
#else
    _Owner = (bIsHK) ? 0 : 1;
    return DMX_FILTER_STATUS_OK;
#endif
}

//--------------------------------------------------------------------------------------------------
/// Reset AVFIFO
/// @param  DmxFltType         \b IN: demux type
/// @param  bReset             \b IN: section buffer size
/// @return DMX_FILTER_STATUS
/// @note
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_AVFifo_Reset(DMX_FILTER_TYPE DmxFltType, MS_BOOL bReset)
{
    DrvTSP_FltType _AVFifo_type;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if DMX_DEBUG
    if(u32DbgLogFlag & DMX_DBG_LOG_AVFIFO)
    {
        if(DmxFltType <= DMX_FILTER_TYPE_SECTION_VER)
            dmx_dbg_print("[DMX FIFO RST]\t[%s]\n", stDmxType[DmxFltType].str);
        else
            dmx_dbg_print("[DMX FIFO RST]\t[unknown %d]\n", (MS_S32)DmxFltType);
    }
#endif

    _DMX_ENTRY();

    if(_DMX_ApiDrv_FltTypeMapping(DmxFltType, &_AVFifo_type) == FALSE)
    {
        DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "Incorrect type for AVFIFO\n"));
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (MDrv_TSP_AVFifo_Reset(_AVFifo_type, bReset) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
}

//--------------------------------------------------------------------------------------------------
/// Get AVFIFO status(level)
/// @param  DmxFltType        \b IN: demux type
/// @param  u32FifoLevel      \b OUT: fifo level
/// @return DMX_FILTER_STATUS
/// @note
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_AVFifo_Status(DMX_FILTER_TYPE DmxFltType, DMX_FIFO_STATUS* u32FifoLevel)
{
    DrvTSP_FltType _AVFifo_type;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    if(_DMX_ApiDrv_FltTypeMapping(DmxFltType, &_AVFifo_type) == FALSE)
    {
        DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "Incorrect type for AVFIFO\n"));
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (MDrv_TSP_AVFifo_Status(_AVFifo_type, u32FifoLevel) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
}

//--------------------------------------------------------------------------------------------------
/// Enable or disable AV FIFO block mechanism
/// @param  DmxFltType            \b IN: demux type
/// @param  bEnable                 \b OUT: TRUR: Enable block mechanism, FALSE: Disable block mechanism
/// @return DMX_FILTER_STATUS
/// @note Not supprot for all chips
//--------------------------------------------------------------------------------------------------
#if (!(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0)))  // For TSP_VER_3_0 and TSP_VER_4_0
#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
DMX_FILTER_STATUS _MApi_DMX_AVFifo_BlockEnable(DMX_FILTER_TYPE DmxFltType, MS_BOOL bEnable)
{
    DrvTSP_FltType _AVFifo_type;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    switch (DmxFltType)
    {
        case DMX_FILTER_TYPE_VIDEO:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_VIDEO;
            break;
        case DMX_FILTER_TYPE_AUDIO:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_AUDIO;
            break;
        case DMX_FILTER_TYPE_AUDIO2:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_AUDIO2;
            break;
        case DMX_FILTER_TYPE_VIDEO3D:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_VIDEO3D;
            break;
        case DMX_FILTER_TYPE_AUDIO3:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_AUDIO3;
            break;
        case DMX_FILTER_TYPE_AUDIO4:
            _AVFifo_type = E_DRVTSP_FLT_TYPE_AUDIO4;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX",  "Incorrect type for AVFIFO\n"));
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (MDrv_TSP_AVFifo_BlockEnable(_AVFifo_type, bEnable) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
}
#else // For TSP_VER_4_0
DMX_FILTER_STATUS _MApi_DMX_AVFifo_BlockEnable(DMX_FILTER_TYPE DmxFltType, MS_BOOL bEnable)
{
    TSP_FltType eFlttype;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    if(_DMX_ApiDrv_FltTypeMapping(DmxFltType, &eFlttype) == FALSE)
    {
        DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX",  "Incorrect type for AVFIFO\n"));
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (MDrv_TSP_FIFO_BlockEnable(eFlttype, bEnable) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}
#endif  // End of TSP_VER_4_0
#endif  // End of TSP_VER_3_0 and TSP_VER_4_0

/********************************************************************/
/// Get DMX Version.
///@param ppVersion \b OUT: DMX Version
///@return DMX_FILTER_STATUS
/********************************************************************/
DMX_FILTER_STATUS _MApi_DMX_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    *ppVersion = &_api_dmx_version;
    return DMX_FILTER_STATUS_OK;
}

//--------------------------------------------------------------------------------------------------
/// Query DMX specific hardware capability
/// @param  DmxQueryType      \b IN: query type
/// @param  pOutput           \b OUT: answer of query type
/// @return DMX_FILTER_STATUS
/// @note
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_GetCap(DMX_QUERY_TYPE DmxQueryType, void* pOutput)
{
    MS_U32 u32size = 0;

    DMX_ASSERT(DMX_CAP_NULL > DmxQueryType, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Capability type not Support!\n", __LINE__)));

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s][%d](0x%x, %p)\n", __FUNCTION__, __LINE__, DmxQueryType, pOutput));

    //DMX share resource = DMX own resource + TSP resource + MMFI resource + TSO resource
    if(DmxQueryType == DMX_CAP_RESOURCE_SIZE)
    {
        *((MS_U32*)pOutput) = sizeof(DMX_RESOURCE_PRIVATE);

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0))
        if(MDrv_TSP_GetCap(E_DRVTSP_CAP_RESOURCE_SIZE, (void*)(&u32size)) != DRVTSP_OK)
            return DMX_FILTER_STATUS_ERROR;
#else
        if(MDrv_TSP_GetCap(E_TSP_CAP_RESOURCE_SIZE, (void*)(&u32size)) != DRVTSP_OK)
            return DMX_FILTER_STATUS_ERROR;
#endif
        *((MS_U32*)pOutput) += u32size;

#if defined(MMFILEIN) || defined(MMFI_VD3D)
        if(MDrv_MMFI_GetCap(E_DRVMMFI_CAP_RESOURCE_SIZE, (void*)(&u32size)) != DRVMMFI_OK)
            return DMX_FILTER_STATUS_ERROR;
        *((MS_U32*)pOutput) += u32size;
#endif

#ifdef TSIO_ENABLE
        if(MDrv_TSIO_GetCap(DRVTSIO_CAP_RESOURCE_SIZE, (void*)(&u32size)) != DRVTSIO_OK)
            return DMX_FILTER_STATUS_ERROR;
        *((MS_U32*)pOutput) += u32size;
#endif

#ifdef TSO_ENABLE
        if(MDrv_TSO_GetCap(E_DRVTSO_CAP_RESOURCE_SIZE, (void*)(&u32size)) != DRVTSO_OK)
            return DMX_FILTER_STATUS_ERROR;
        *((MS_U32*)pOutput) += u32size;
#endif

        return DMX_FILTER_STATUS_OK;
    }

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0))

    if(MDrv_TSP_GetCap((DrvTSP_Cap)((MS_U32)DmxQueryType), pOutput) == DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#else  // For TSP_VER_4_0

   if(MDrv_TSP_GetCap((TSP_Caps)((MS_U32)DmxQueryType), pOutput) == DRVTSP_OK)
   {
       return DMX_FILTER_STATUS_OK;
   }
   else
   {
       return DMX_FILTER_STATUS_ERROR;
   }
#endif // End of TSP_VER_4_0
}

//--------------------------------------------------------------------------------------------------
/// Query DMX specific hardware capability by string type
/// @param  pstrQueryType                               \b IN: String of capability item
/// @param  s32Strlen                                         \b IN: String size of capability item
/// @param  pOutput                                        \b OUT: answer of query type
/// @param  pseOutSize                                       \b OUT: Output data length
/// @return DMX_FILTER_STATUS
/// @note
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_GetCap_Ex(char* pstrQueryType, MS_S32 s32Strlen, void* pOutput, MS_S32* ps32OutSize)
{
    MS_U32 ii;
    char* ptr = pstrQueryType;

    *ps32OutSize = 0;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s][%d](%s, %p)\n", __FUNCTION__, __LINE__, pstrQueryType, pOutput));
    DMX_ASSERT((pstrQueryType != NULL), DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Capability type string is NULL!\n", __LINE__)));
    DMX_ASSERT((pOutput != NULL), DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Output data pointer is NULL!\n", __LINE__)));
    DMX_ASSERT((s32Strlen > 8), DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Input Staring type is wrong!\n", __LINE__)));

    if(strncmp(ptr, "DMX_CAP_", 8) != 0)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    ptr += 8;
    for(ii = 0; ii < DMX_CAP_NULL; ii++)
    {
        if(strcmp(ptr, _tblDmxCapStr[ii].strCap) == 0)
            break;
    }
    if(ii == DMX_CAP_NULL)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    *ps32OutSize = _tblDmxCapStr[ii].OutputSize;

    return _MApi_DMX_GetCap(_tblDmxCapStr[ii].eCapType, pOutput);

}

DMX_FILTER_STATUS _MApi_DMX_SetBurstLen(DMX_BURSTTYPE BurstType)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    if(MDrv_TSP_BurstLen(BurstType) == DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
}

DMX_FILTER_STATUS _MApi_DMX_GetFileInTimeStamp(MS_U32 *pu32FileInTS)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    MDrv_TSP_GetFileInTimeStamp(pu32FileInTS);
    return DMX_FILTER_STATUS_OK;
}

DMX_FILTER_STATUS _MApi_DMX_Filein_GetReadAddr(MS_PHY* pphyRead)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    if(MDrv_TSP_File_GetReadAddr(pphyRead) != DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#endif

    return DMX_FILTER_STATUS_OK;
}

/********************************************************************/
/// Disable 192 mode blovk scheme to bypass fill-in timestamp
///@param bbypass \b IN: If true, bypass file-in timestamp.
/********************************************************************/
void _MApi_DMX_BypassFileInTimeStamp(MS_BOOL bbypass)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    MDrv_TSP_File_192BlockScheme_En(!bbypass);
}

DMX_FILTER_STATUS _MApi_DMX_GetDbgPortInfo(MS_U32 u32DbgSel,MS_U32* u32DbgInfo)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    MDrv_TSP_GetDbgPortInfo(u32DbgSel,u32DbgInfo);
    return DMX_FILTER_STATUS_OK;
}

//[RESERVED] Unofficial function for A/V flow control by APP
DMX_FILTER_STATUS _MApi_DMX_SetAVPause(MS_BOOL bSet)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

#if defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0)
    MDrv_TSP_SetAVPause(bSet);
#else
    // @TODO: hand over the control to APP if problem on LIVE mode
    // There is no know-how indicates that DSCMB burst output will cuase A/V FIFO overflow due to slow A/V
    // parser or decoder. - Jerry
#endif

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

#if 0
DMX_FILTER_STATUS MApi_DMX_ResetFileinTimestamp(void)
{
    MDrv_TSP_ResetFileinTimestamp();
    return DMX_FILTER_STATUS_OK;
}
#endif

DMX_FILTER_STATUS _MApi_TSP_Get_FW_VER(MS_U32* u32FWVer)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    MDrv_TSP_Get_FW_VER(u32FWVer);
    return DMX_FILTER_STATUS_OK;
}

/********************************************************************/
/// Enable remove duplicate A/V packets
/// @param  bEnable                \b IN: Enable or Disable
/// @return DMX_FILTER_STATUS
/********************************************************************/
DMX_FILTER_STATUS _MApi_DMX_RemoveDupAVPkt(MS_BOOL bEnable)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    if(MDrv_TSP_RemoveDupAVPkt(bEnable) == DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
}

/********************************************************************/
/// Enable remove duplicate A/V Fifo packets
/// @param  DmxFltType           \b IN: demux type
/// @param  bEnable                \b IN: Enable or Disable
/// @return DMX_FILTER_STATUS
/********************************************************************/
DMX_FILTER_STATUS _MApi_DMX_RemoveDupAVFifoPkt(DMX_FILTER_TYPE DmxFltType, MS_BOOL bEnable)
{
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    DrvTSP_FltType DrvFltTye;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    switch (DmxFltType)
    {
        case DMX_FILTER_TYPE_VIDEO:
            DrvFltTye = E_DRVTSP_FLT_TYPE_VIDEO;
            break;
        case DMX_FILTER_TYPE_AUDIO:
            DrvFltTye = E_DRVTSP_FLT_TYPE_AUDIO;
            break;
        case DMX_FILTER_TYPE_AUDIO2:
            DrvFltTye = E_DRVTSP_FLT_TYPE_AUDIO2;
            break;
        case DMX_FILTER_TYPE_VIDEO3D:
            DrvFltTye = E_DRVTSP_FLT_TYPE_VIDEO3D;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "Incorrect type for AVFIFO\n"));
            return DMX_FILTER_STATUS_ERROR;
    }

    if(MDrv_TSP_RemoveDupAVFifoPkt(DrvFltTye, bEnable) == DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }

#endif
    return DMX_FILTER_STATUS_ERROR;
}

/********************************************************************/
/// Enable or Disable to remove TEI  audio or video error packets.
/// @param  bEnable                \b IN: Enable or Disable
/// @return DMX_FILTER_STATUS
/********************************************************************/
DMX_FILTER_STATUS _MApi_DMX_TEI_RemoveErrorPkt(DMX_TEI_RmPktType eDmxPktType, MS_BOOL bEnable )
{
  DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if (!(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0)))  // For TSP_VER_3_0 and TSP_VER_4_0

    TSP_DRV_TEI_RmPktType eDrvPktType = E_TSP_DRV_TEI_REMOVE_AUDIO_PKT;

    switch ( eDmxPktType )
    {
        case E_DMX_TEI_REMOVE_AUDIO_PKT:
            eDrvPktType = E_TSP_DRV_TEI_REMOVE_AUDIO_PKT;
            break;
        case E_DMX_TEI_REMOVE_VIDEO_PKT:
            eDrvPktType = E_TSP_DRV_TEI_REMOVE_VIDEO_PKT;
            break;
        case E_DMX_TEI_REMOVE_PKT0_LIVE:
            eDrvPktType = E_TSP_DRV_TEI_REMOVE_PKT0_LIVE;
            break;
        case E_DMX_TEI_REMOVE_PKT0_FILE:
            eDrvPktType = E_TSP_DRV_TEI_REMOVE_PKT0_FILE;
            break;
        case E_DMX_TEI_REMOVE_PKT1:
            eDrvPktType = E_TSP_DRV_TEI_REMOVE_PKT1;
            break;
        case E_DMX_TEI_REMOVE_PKT2:
            eDrvPktType = E_TSP_DRV_TEI_REMOVE_PKT2;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad TEI__TYPE %u\n", __LINE__, (unsigned int)eDmxPktType));
            return DMX_FILTER_STATUS_ERROR;
    }

    if(MDrv_TSP_TEI_RemoveErrorPkt( eDrvPktType, bEnable ) == DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
static TSP_PktMode _DMX_ApiDrv_PacketModeMapping(DMX_PacketMode ePacketMode)
{
    switch (ePacketMode)
    {
        case DMX_PKTMODE_188:
            return E_TSP_PKTMODE_188;
        case DMX_PKTMODE_192:
            return E_TSP_PKTMODE_192;
        case DMX_PKTMODE_204:
            return E_TSP_PKTMODE_204;
        case DMX_PKTMODE_130:
            return E_TSP_PKTMODE_130;
        case DMX_PKTMODE_134:
            return E_TSP_PKTMODE_134;
        case DMX_PKTMODE_MERG188: //[STB]: merge188 -> CI+
            return E_TSP_PKTMODE_CIPLUS;
        case DMX_PKTMODE_MERG192: //[STB]: merge192 -> ATS
            return E_TSP_PKTMODE_ATS;
        case DMX_PKTMODE_200:     //[STB]: single/merge 200 -> OpenCable
        case DMX_PKTMODE_MERG200:
            return E_TSP_PKTMODE_OPENCABLE;
        case DMX_PKTMODE_MXL192:
            return E_TSP_PKTMODE_MXL192;
        case DMX_PKTMODE_MXL196:
            return E_TSP_PKTMODE_MXL196;
        case DMX_PKTMODE_MXL200:
            return E_TSP_PKTMODE_MXL200;
        default:
            DMX_DBGMSG(DMX_DBG_ERR,  ULOGE("DMX", "[MAPI DMX][%06d] Bad packet mode %d\n", __LINE__, (int)ePacketMode));
            return E_TSP_PKTMODE_188;
    }
}
#endif  // End of TSP_VER_4_0

//-------------------------------------------------------------------------------------------------
/// Set Demux Flow packet mode
/// @param DmxFlow \b IN: DMX_FLOW_PLAYBACK for playback flow
///                       DMX_FLOW_PVR: for recording flow
///                       DMX_FLOW_PVRCA: for CA recording flow
///                       Below Items are not support for all chips:
///                       DMX_FLOW_PVR1: for second recording flow.
///                       DMX_FLOW_FILEIN_MM: for file-in playback with independed audio path.
///                       DMX_FLOW_FILEIN_MM3D: for file-in playback with 2nd video input source .
///
/// @param ePktMode \b IN: Packet mode of flow path
///                            DMX_PKTMODE_188: Normal 188 packet mode
///                            DMX_PKTMODE_192: Normal 192 packet mode
///                            DMX_PKTMODE_204: Normal 204 packet mode
///                            DMX_PKTMODE_130: RVU 130 packet mode
///                            DMX_PKTMODE_134: RVU 134 packet mode
///                            DMX_PKTMODE_200: Open cable 200 packet mode
///                            DMX_PKTMODE_MERG188: Merge stream 188 packet mode
///                            DMX_PKTMODE_MERG192: Merge stream 192 packet mode
///                            DMX_PKTMODE_MERG200: Merge stream 200 packet mode
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SetPacketMode(DMX_FLOW DmxFlow, DMX_PacketMode ePktMode)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d (%d, %d)\n", __FUNCTION__, __LINE__, (int)DmxFlow, (int)ePktMode));

    _DMX_ENTRY();

#if(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))

    MS_U32 u32TSIf = 0;

    switch(DmxFlow)
    {
        case DMX_FLOW_PLAYBACK:
            u32TSIf = 0;
            break;
        case DMX_FLOW_PVR:
        case DMX_FLOW_PVRCA:
            u32TSIf = 1;
            break;
        case DMX_FLOW_PVR1:
            u32TSIf = 2;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[%s][%d] DMX flow Not Support !!\n",__FUNCTION__,__LINE__));
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if(MDrv_TSP_SetPacketMode(u32TSIf, (DrvTSP_PacketMode)ePktMode) != DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#elif(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0

    MS_U32 u32TSIf = 0;

    TSP_Result eRes = E_TSP_OK;

    if(!_DMX_ApiDrv_DmxFlowMapping(DmxFlow,&u32TSIf))
    {
        DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%s][%06d] _DMX_ApiDrv_DmxFlowMapping() fail \n", __FUNCTION__, __LINE__));
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if((eRes = MDrv_TSP_SetPacketMode((TSP_TSIF)u32TSIf, _DMX_ApiDrv_PacketModeMapping(ePktMode))) != DRVTSP_OK)
    {
        if(eRes == E_TSP_FAIL_NOT_SUPPORTED)
        {
            _DMX_RETURN(DMX_FILTER_STATUS_NOT_SUPPORT);
        }
        else
        {
            _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
        }
    }

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#endif  // End of TSP_VER_4_0

    _DMX_RETURN(DMX_FILTER_STATUS_NOT_SUPPORT);
}

//-------------------------------------------------------------------------------------------------
/// Set Sync Byte value to Stream source ID or TS source ID
/// @param u32SrcID \b               IN: Index of stream source id or TS source id
///                                                   Stream source ID is from 0 ~7
///                                                   TS source ID is mapping TSIF index
/// @param u8SyncByte \b            IN: Sync byte, defalut value is 0x47
///
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SetMergeStrSyncByte(MS_U32 u32SrcID, MS_U8 u8SyncByte)
{
#if(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))

    _DMX_ENTRY();

    if(MDrv_TSP_SetMergeStrSyncByte(u32SrcID, u8SyncByte) != DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_RETURN(DMX_FILTER_STATUS_OK);

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

/********************************************************************/
/// Change DMX filter source type
/// @param  u8DmxId                      \b IN: DMX filter ID
/// @param  DmxFltSrcType                \b IN: DMX filter Source Type and filter source id
/// @return DMX_FILTER_STATUS
/********************************************************************/
DMX_FILTER_STATUS _MApi_DMX_Change_FilterSource(MS_U32 u32DmxId, DMX_FILTER_TYPE DmxFltSrcType)
{
    _DMX_ENTRY();

    MS_U32 u32TSPFltSource = DmxFltSrcType & ((MS_U32)DMX_FILTER_FLT_MASK);

#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
    u32TSPFltSource  = _DMX_ApiDrv_FltSrcMapping(u32TSPFltSource);
#endif

    MDrv_TSP_PidFlt_ChangeSource(0, u32DmxId, u32TSPFltSource);

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set memory buffer information for TSP AEON to print debug message
/// @param phyAddr \b IN: physical address of buffer
/// @param u32Size \b IN: size of buffer
/// @param u32DbgWord \b IN: control word to filter debug message
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_SetFwDbgParam(MS_PHY phyAddr, MS_U32 u32Size, MS_U32 u32DbgWord)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d, u32Addr = 0x%x, u32Size = 0x%x\n", __FUNCTION__, __LINE__, (unsigned int)phyAddr, (unsigned int)u32Size));

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0))
    DMX_ASSERT((!(phyAddr & 0x3) && !(u32Size & 0x3)), DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] memory address 0x%x size 0x%x not alignment\n",
        __LINE__, (unsigned int)phyAddr, (unsigned int)u32Size)));

    memset((void*)((MS_VIRT)phyAddr), 0x0, u32Size);

    if(DRVTSP_OK == MDrv_TSP_SetFwDBGParam(phyAddr, u32Size, u32DbgWord))
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

DMX_FILTER_STATUS _MApi_DMX_SetDbgLevel(DMX_DBGMSG_LEVEL level)
{
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
    DrvTSP_DbgLevel drvLayerLevel = E_DRVTSP_DBG_L1;
#endif

#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    switch (level)
    {
        case DMX_DBG_ERR:
        case DMX_DBG_WARN:
        case DMX_DBG_INFO:
        case DMX_DBG_FUNC:
            drvLayerLevel = E_DRVTSP_DBG_L1;
            break;
        case DMX_DBG_NONE:
            drvLayerLevel = E_DRVTSP_DBG_Release;
            break;
        default:
            break;
    }

    MDrv_TSP_SetDbgLevel(drvLayerLevel);

#else  // For TSP_VER_4_0

    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));
    TSP_DbgLevel eDrvTspDbgLevel = E_TSP_DBG_ERROR;

    switch(level)
    {
        case DMX_DBG_NONE:
            eDrvTspDbgLevel = E_TSP_DBG_NONE;
           break;
        case DMX_DBG_ERR:
            eDrvTspDbgLevel = E_TSP_DBG_ERROR;
            break;
        case DMX_DBG_WARN:
            eDrvTspDbgLevel = E_TSP_DBG_WARNING;
            break;
        case DMX_DBG_INFO:
            eDrvTspDbgLevel = E_TSP_DBG_INFO;
            break;
        case DMX_DBG_FUNC:
            eDrvTspDbgLevel = E_TSP_DBG_TRACK;
            break;

         default:
            break;

    }
    printf("level = %d\n",level);
    MDrv_TSP_SetDbgLevel(eDrvTspDbgLevel);

#endif  // End of TSP_VER_4_0

    _u32DMXDbgLevel = level;
    return DMX_FILTER_STATUS_OK;
}

//--------------------------------------------------------------------------------------------------
/// Enable or Disable STC64 bit mode. Defalut is STC33 mode.
/// @param  bEnable           \b IN: Enable or Disable STC64 mode
/// @return DMX_FILTER_STATUS
/// @note
//--------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_STC64_Mode_Enable(MS_BOOL bEnable)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef STC64_SUPPORT
    MDrv_TSP_STC64_Mode_Enable(bEnable);
    return DMX_FILTER_STATUS_OK;
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Open a demux filter and attach to a existed filter and buffer
/// @param DmxFltType \b IN: the filter information to allocate
/// @param pu8DmxId \b OUT: the available demux filer Id
/// @param u8TargetDmxId \b IN: the target demux filer Id
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
/// @note  API for U4 TSP N filter to 1 Buffer HW architecture.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Open_MultiFlt(DMX_FILTER_TYPE DmxFltType , MS_U8* pu8DmxId, MS_U8 u8TargetDmxId)
{
#if (defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0)))

    ULOGE("DMX", "[%s] Function Not support!! \n",__FUNCTION__);
    return DMX_FILTER_STATUS_ERROR ;

#else  // For TSP_VER_4_0

    MS_U32              FltId= 0xFFFFFFFF;
    MS_U32              BufId= _pdmx_res->_FltList[u8TargetDmxId].SecBufId;
    DMX_FILTER_TYPE     TgtDmxFltType;
    MS_U32              u32TSPFltSource = DmxFltType & DMX_FILTER_FLT_MASK ;
    DrvTSP_FltType      eTspFltType = E_DRVTSP_FLT_TYPE_VIDEO;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));
    DMX_ASSERT(DMX_MAX_FLTID> u8TargetDmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Target Demux id %d\n", __LINE__, u8TargetDmxId)));

    _DMX_ENTRY();

    ULOGD("DMX", "input Filter Type = %08X \n",(unsigned int)DmxFltType);

    DmxFltType = DmxFltType & (DMX_FILTER_TYPE)(~DMX_FILTER_FLT_MASK);
    _FLT_LIST_TYPE_GET(u8TargetDmxId,&TgtDmxFltType);

    ULOGD("DMX", "Get FlType %08X \n",(unsigned int)TgtDmxFltType);

    switch (TgtDmxFltType)
    {
        case DMX_FILTER_TYPE_SECTION:
            eTspFltType=            E_DRVTSP_FLT_TYPE_SECTION;
            break;
        case DMX_FILTER_TYPE_TELETEXT:
            eTspFltType=            E_DRVTSP_FLT_TYPE_TELETEXT;
            break;
        case DMX_FILTER_TYPE_PES:
            eTspFltType=            E_DRVTSP_FLT_TYPE_PES;
            break;
        case DMX_FILTER_TYPE_PACKET:
            eTspFltType=            E_DRVTSP_FLT_TYPE_PACKET;
            break;
        case DMX_FILTER_TYPE_PCR:
            eTspFltType=            E_DRVTSP_FLT_TYPE_PCR;
            break;
    }

    if( DmxFltType != TgtDmxFltType)
    {
        ULOGE("DMX", "[MAPI DMX][%06d] MultiFlt Type mismatch %u\n", __LINE__, (unsigned int)FltId);
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if (DRVTSP_OK!= MDrv_TSP_PidFlt_Alloc(_Owner , (DrvTSP_FltType)(eTspFltType|u32TSPFltSource), &FltId))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    DMX_ASSERT2(FltId< DMX_MAX_FLTID, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad FltId %u\n", __LINE__, (unsigned int)FltId)));

    ULOGD("DMX", "MultiFlt FltId = %u \n",(unsigned int)FltId);

    MDrv_TSP_PidFlt_SelSecFlt(0, FltId, BufId);

    ULOGD("DMX", "MultiFlt AttachBuffer flt %u Buf %u\n",(unsigned int)FltId,(unsigned int)BufId);

    _FLT_LIST_SECFLT_TYPE_SET(FltId, FltId, BufId, TgtDmxFltType);
    *pu8DmxId = (MS_U8)FltId;

    _DMX_RETURN( DMX_FILTER_STATUS_OK);
#endif  // End of TSP_VER_4_0
}

/********************************************************************/
/// Get interrupt count
///@param pu32Count \b OUT: interrupt count
///@return DMX_FILTER_STATUS
/********************************************************************/
DMX_FILTER_STATUS _MApi_DMX_Get_Intr_Count(MS_U32* pu32Count)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0))
    MDrv_TSP_Get_Intr_Count(pu32Count);
    return DMX_FILTER_STATUS_OK;
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

DMX_FILTER_STATUS _MApi_DMX_Drop_Enable(MS_BOOL bSet)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    return DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Check if MMFilein no memory transfer is under going
/// @param  ePath                      \b IN: MM File-in Path
/// @return TRUE if busy, FALSE otherwise
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL _MApi_DMX_MMFI_Filein_IsIdle(DMX_MMFI_PATH ePath)
{
#if defined(MMFILEIN)

    DrvMMFI_FileinState     FileinState = E_DRVMMFI_STATE_UNKNOWN;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    if (DRVMMFI_OK!= MDrv_MMFI_File_GetState((DrvMMFI_Path)ePath, &FileinState))
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Call MDrv_MMFI_File_GetState fail\n", __LINE__)));
    }
    return (E_DRVMMFI_STATE_IDLE== FileinState);
#else
    return FALSE;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Check if MMFilein memory transfer is under going
/// @param  ePath                      \b IN: MM File-in Path
/// @return TRUE if busy, FALSE otherwise
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL _MApi_DMX_MMFI_Filein_IsBusy(DMX_MMFI_PATH ePath)
{
#if defined(MMFILEIN)
    DrvMMFI_FileinState     FileinState = E_DRVMMFI_STATE_UNKNOWN;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    if (DRVMMFI_OK!= MDrv_MMFI_File_GetState((DrvMMFI_Path)ePath, &FileinState))
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Call MDrv_MMFI_File_GetState fail\n", __LINE__)));
    }
    return (E_DRVMMFI_STATE_BUSY== FileinState);
#else
    return FALSE;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Reset MMFilein Command queue
/// @param  ePath                      \b IN: MM File-in Path
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_Filein_CMDQ_Reset(DMX_MMFI_PATH ePath)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if defined(MMFILEIN)

    if(MDrv_MMFI_File_CmdQ_Reset((DrvMMFI_Path)ePath) == DRVMMFI_OK)
    {
        #ifdef DMX_RESET_FI_TIMESTAMP
        if(_pdmx_res->_bPlayTimestampEnable[ePath+1] == TRUE)
        {
            _DMX_MMFI_ENTRY();
            MDrv_MMFI_File_SetAddr(ePath, _pdmx_res->_phyFI192DummyBufAddr);
            MDrv_MMFI_File_SetSize(ePath, _pdmx_res->_u32FI192DummyBufSize);
            MDrv_MMFI_File_Start(ePath);
            _DMX_MMFI_RETURN(DMX_FILTER_STATUS_OK);
        }
        #endif

        return DMX_FILTER_STATUS_OK;
    }
    else
    {
       return DMX_FILTER_STATUS_ERROR;
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Resume to get bit stream by memeory
/// @param  ePath                            \b IN: MM File-in Path
/// @param  pu32EmptySlot                  \b OUT: Empty slot
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_Filein_CMDQ_GetEmptyNum(DMX_MMFI_PATH ePath, MS_U32 *pu32EmptySlot)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef MMFILEIN
    MDrv_MMFI_File_CmdQ_GetSlot((DrvMMFI_Path)ePath, pu32EmptySlot);
    return DMX_FILTER_STATUS_OK;
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Start to get bit stream by memeory
/// @param eDst             \b IN: file in destination path
/// @param pBuf             \b IN: the memory containing the bit stream
/// @param u32BufSize \b IN: the size the memory to get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_Filein_Start(DMX_MMFI_DST eDst, MS_PHY pBuf, MS_U32 u32BufSize)
{
#if !(defined(MMFILEIN))
        return DMX_FILTER_STATUS_ERROR;
#else

#if defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0)
    DrvMMFI_Path path= E_DRVMMFI_PATH0;
#else
    DrvTSP_FileinMode FileMode = E_DRVTSP_FILEIN_MODE_ENG0_TS;
    DrvMMFI_Path path= E_DRVMMFI_PTH_AUDIO;
#endif

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_MMFI_ENTRY();

    if(MDrv_MMFI_File_ModeEnable((DrvMMFI_FileinMode)eDst, TRUE) != DRVMMFI_OK)
    {
        _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
    }

#if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))
    switch(eDst & (~DMX_MMFI1_PES_TYPE_MASK))
    {
        case DMX_MMFI_PES_NO_BYPASS_TS:
            FileMode = E_DRVTSP_FILEIN_MODE_ENG0_TS;
            break;
        case DMX_MMFI_PES_PS_AU:
            FileMode = E_DRVTSP_FILEIN_MODE_ENG0_APES;
            break;
        case DMX_MMFI_PES_PS_AUB:
            FileMode = E_DRVTSP_FILEIN_MODE_ENG0_A2PES;
            break;

        #if defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0)
        case DMX_MMFI_PES_PS_VD:
            FileMode = E_DRVTSP_FILEIN_MODE_ENG0_VPES;
            break;
        #endif //end MMFI_VER_2_0

        #ifdef MMFI_VD3D
        case DMX_MMFI_PES_PS_V3D:
            FileMode = E_DRVTSP_FILEIN_MODE_ENG0_V3DPES;
            break;
        #endif //end MMFI_VD3D
        default:
            break;
    }

    MDrv_TSP_File_PS_Path_Enable(FileMode);

#endif

#if defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0)

    switch(eDst & (~DMX_MMFI1_PES_TYPE_MASK))
    {
        case DMX_MMFI_PES_PS_AU:
            MDrv_TSP_AVFifo_BlockEnable(E_DRVTSP_FLT_TYPE_AUDIO, FALSE);
            break;
        case DMX_MMFI_PES_PS_AUB:
            MDrv_TSP_AVFifo_BlockEnable(E_DRVTSP_FLT_TYPE_AUDIO2, FALSE);
            break;
        case DMX_MMFI_PES_PS_VD:
            MDrv_TSP_AVFifo_BlockEnable(E_DRVTSP_FLT_TYPE_VIDEO, FALSE);
            break;
#ifdef MMFI_VD3D
        case DMX_MMFI_PES_PS_V3D:
            MDrv_TSP_AVFifo_BlockEnable(E_DRVTSP_FLT_TYPE_VIDEO3D, FALSE);
            break;
#endif //end MMFI_VD3D
        case DMX_MMFI_PES_PS_AUC:
            MDrv_TSP_AVFifo_BlockEnable(E_DRVTSP_FLT_TYPE_AUDIO3, FALSE);
            break;
        case DMX_MMFI_PES_PS_AUD:
            MDrv_TSP_AVFifo_BlockEnable(E_DRVTSP_FLT_TYPE_AUDIO4, FALSE);
            break;
#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
        case DMX_MMFI_PES_PS_AUE:
            MDrv_TSP_AVFifo_BlockEnable(E_DRVTSP_FLT_TYPE_AUDIO5, FALSE);
            break;
        case DMX_MMFI_PES_PS_AUF:
            MDrv_TSP_AVFifo_BlockEnable(E_DRVTSP_FLT_TYPE_AUDIO6, FALSE);
            break;
        case DMX_MMFI_PES_PS_VD3:
            MDrv_TSP_AVFifo_BlockEnable(E_DRVTSP_FLT_TYPE_VIDEO3, FALSE);
            break;
        case DMX_MMFI_PES_PS_VD4:
            MDrv_TSP_AVFifo_BlockEnable(E_DRVTSP_FLT_TYPE_VIDEO4, FALSE);
            break;
        case DMX_MMFI_PES_PS_VD5:
            MDrv_TSP_AVFifo_BlockEnable(E_DRVTSP_FLT_TYPE_VIDEO5, FALSE);
            break;
        case DMX_MMFI_PES_PS_VD6:
            MDrv_TSP_AVFifo_BlockEnable(E_DRVTSP_FLT_TYPE_VIDEO6, FALSE);
            break;
        case DMX_MMFI_PES_PS_VD7:
            MDrv_TSP_AVFifo_BlockEnable(E_DRVTSP_FLT_TYPE_VIDEO7, FALSE);
            break;
        case DMX_MMFI_PES_PS_VD8:
            MDrv_TSP_AVFifo_BlockEnable(E_DRVTSP_FLT_TYPE_VIDEO8, FALSE);
            break;
#endif
        default:
            break;
    }

    if(eDst & DMX_MMFI1_PES_TYPE_MASK)
    {
        path= E_DRVMMFI_PATH1;
    }
#else //MMFI_VER_2_0
    #ifdef MMFI_VD3D
    if((eDst == DMX_MMFI_PES_V3DPID_BYPASS) || (eDst == DMX_MMFI_PES_NO_BYPASS_V3D)
        || (eDst == DMX_MMFI_PES_PS_V3D))
    {
        path = E_DRVMMFI_PATH_VD3D;
    }
    #endif

#endif

    #ifdef SECURE_PVR_ENABLE
    _pdmx_res->_stSecBuf.u32BufId = ((MS_U8)path) & 0xFF;
    _pdmx_res->_stSecBuf.u32BufOpt = 0;
    _pdmx_res->_stSecBuf.phyBufAddr = pBuf;
    _pdmx_res->_stSecBuf.u32BufSize = u32BufSize;
    if(MDrv_TSP_Ree_SendTeeCmd(E_DRVTSP_REE_TO_TEE_CMD_SET_MMFIBuf, (void*)&(_pdmx_res->_stSecBuf), sizeof(DrvTSP_SecureBuf)) != DRVTSP_OK)
    {
        _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    #else
    if (DRVMMFI_OK != MDrv_MMFI_File_SetAddr(path, pBuf))
    {
        _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    if (DRVMMFI_OK != MDrv_MMFI_File_SetSize(path, u32BufSize))
    {
        _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    #endif  //SECURE_PVR_ENABLE

    if (DRVMMFI_OK!= MDrv_MMFI_File_Start(path))
    {
        _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_MMFI_RETURN(DMX_FILTER_STATUS_OK);

#endif //!(defined(MMFILEIN))
}

//-------------------------------------------------------------------------------------------------
/// Set up parameters for input bit stream from memory
/// @param  ePath                                    \b IN: MM File-in Path
/// @param pFileinInfo                              \b IN: the file in parameters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_Filein_Info(DMX_MMFI_PATH ePath, DMX_Filein_info *pFileinInfo)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef MMFILEIN
    _DMX_MMFI_ENTRY();
    MDrv_MMFI_File_SetPacketMode((DrvMMFI_Path)ePath, (DrvMMFI_PacketMode)pFileinInfo->PKT_Mode);
    MDrv_MMFI_File_SetTimer((DrvMMFI_Path)ePath, (MS_U8)(pFileinInfo->Rate & 0xFF));
    _DMX_MMFI_RETURN(DMX_FILTER_STATUS_OK);

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Disable 192 mode blovk scheme to bypass fill-in timestamp
/// @param  ePath                                    \b           IN: MM File-in Path
/// @param bbypass                                  \b          IN: If true, bypass file-in timestamp.
//-------------------------------------------------------------------------------------------------
void _MApi_DMX_MMFI_Filein_BypassTimeStamp(DMX_MMFI_PATH ePath, MS_BOOL bbypass)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef MMFILEIN
    _DMX_MMFI_ENTRY();
    MDrv_MMFI_File_192BlockScheme_En((DrvMMFI_Path)ePath, !bbypass);
    MsOS_ReleaseMutex(_pdmx_res->_Dmx_MMFI_Mutex);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Free the PID of MM Filein
/// @param  ePath                      \b IN: MM File-in Path
/// @param pu32FileInTS                       \b OUT: pointer for timestamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_GetFileInTimeStamp(DMX_MMFI_PATH ePath, MS_U32 *pu32FileInTS)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef MMFILEIN
    MDrv_MMFI_GetFileInTimeStamp((DrvMMFI_Path)ePath, pu32FileInTS);
    return DMX_FILTER_STATUS_OK;
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set the PID to be MM File-in
/// @param Pid \b IN: The target PID for MM Filein
/// @param pu8DmxId \b OUT: The demux filter Id for this MMFilein PID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_Pid_Open(DMX_MMFI_FLTTYPE flttype, MS_U16 u16Pid, MS_U8* pu8DmxId)
{
#ifndef MMFILEIN
    return DMX_FILTER_STATUS_ERROR;
#else

    DrvMMFI_FltType    type = E_DRVMMFI_FLTTYPE_NONE;

  #if defined(MMFI_VERSION) && (MMFI_VERSION == MMFI_VER_2_0)
    DrvMMFI_Path epath = E_DRVMMFI_PATH0;

    #if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
        TSP_TSId eTspTSid = E_TSP_TSID_INVALID;
        TSP_FileMode eFifoType = E_TSP_FILE_2_TSP;
    #endif

  #endif

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_MMFI_ENTRY();

    switch(flttype & ~(DMX_MMFI1_FLTTYPE_MASK))
    {
        case DMX_MMFI_FLTTYPE_AUD:
            type = E_DRVMMFI_FLTTYPE_AUD;
            break;
        case DMX_MMFI_FLTTYPE_AUDB:
            type = E_DRVMMFI_FLTTYPE_AUDB;
            break;
  #ifdef MMFI_VD3D
        case DMX_MMFI_FLTTYPE_VD3D:
            type = E_DRVMMFI_FLTTYPE_VD3D;
            break;
  #endif
  #if defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0)
        case DMX_MMFI_FLTTYPE_VD:
            type = E_DRVMMFI_FLTTYPE_VD;
            break;
        case DMX_MMFI_FLTTYPE_AUDC:
            type = E_DRVMMFI_FLTTYPE_AUDC;
            break;
        case DMX_MMFI_FLTTYPE_AUDD:
            type = E_DRVMMFI_FLTTYPE_AUDD;
            break;
    #if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
        case DMX_MMFI_FLTTYPE_AUDE:
            type = E_DRVMMFI_FLTTYPE_AUDE;
            break;
        case DMX_MMFI_FLTTYPE_AUDF:
            type = E_DRVMMFI_FLTTYPE_AUDF;
            break;
        case DMX_MMFI_FLTTYPE_VD3:
            type = E_DRVMMFI_FLTTYPE_VD3;
            break;
        case DMX_MMFI_FLTTYPE_VD4:
            type = E_DRVMMFI_FLTTYPE_VD4;
            break;
        case DMX_MMFI_FLTTYPE_VD5:
            type = E_DRVMMFI_FLTTYPE_VD5;
            break;
        case DMX_MMFI_FLTTYPE_VD6:
            type = E_DRVMMFI_FLTTYPE_VD6;
            break;
        case DMX_MMFI_FLTTYPE_VD7:
            type = E_DRVMMFI_FLTTYPE_VD7;
            break;
        case DMX_MMFI_FLTTYPE_VD8:
            type = E_DRVMMFI_FLTTYPE_VD8;
            break;
    #endif
  #endif
        default:
            _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
            break;
    }

  #if defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0)
    if(flttype & DMX_MMFI1_FLTTYPE_MASK)
    {
        epath = E_DRVMMFI_PATH1;
    }
    if(MDrv_MMFI_PidFlt_Alloc(epath, type, pu8DmxId) == DRVMMFI_FAIL)
    {
        _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    #if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0))

    MDrv_TSP_AVFifo_SourceSelect((MS_U32)epath + (MS_U32)E_DRVTSP_PKTSRC_DEMUXMMFI0, (MS_U32)(flttype & ~(DMX_MMFI1_FLTTYPE_MASK)));

    #else // For TSP_VER_4_0
    switch(epath)
    {
        case E_DRVMMFI_PATH0:
            eTspTSid = E_TSP_TSID_MMFI0;
            break;
        case E_DRVMMFI_PATH1:
            eTspTSid = E_TSP_TSID_MMFI1;
            break;
        default:
            _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
            break;
    }

    switch(type)
    {
        case E_DRVMMFI_FLTTYPE_AUD:
            eFifoType = E_TSP_FILE_2_AUDIO;
            break;
        case E_DRVMMFI_FLTTYPE_AUDB:
            eFifoType = E_TSP_FILE_2_AUDIO2;
            break;
        case E_DRVMMFI_FLTTYPE_AUDC:
            eFifoType = E_TSP_FILE_2_AUDIO3;
            break;
        case E_DRVMMFI_FLTTYPE_AUDD:
            eFifoType = E_TSP_FILE_2_AUDIO4;
            break;
        case E_DRVMMFI_FLTTYPE_AUDE:
            eFifoType = E_TSP_FILE_2_AUDIO5;
            break;
        case E_DRVMMFI_FLTTYPE_AUDF:
            eFifoType = E_TSP_FILE_2_AUDIO6;
            break;
        case E_DRVMMFI_FLTTYPE_VD:
            eFifoType = E_TSP_FILE_2_VIDEO;
            break;
        case E_DRVMMFI_FLTTYPE_VD3D:
            eFifoType = E_TSP_FILE_2_VIDEO3D;
            break;
        case E_DRVMMFI_FLTTYPE_VD3:
            eFifoType = E_TSP_FILE_2_VIDEO3;
            break;
        case E_DRVMMFI_FLTTYPE_VD4:
            eFifoType = E_TSP_FILE_2_VIDEO4;
            break;
        case E_DRVMMFI_FLTTYPE_VD5:
            eFifoType = E_TSP_FILE_2_VIDEO5;
            break;
        case E_DRVMMFI_FLTTYPE_VD6:
            eFifoType = E_TSP_FILE_2_VIDEO6;
            break;
        case E_DRVMMFI_FLTTYPE_VD7:
            eFifoType = E_TSP_FILE_2_VIDEO7;
            break;
        case E_DRVMMFI_FLTTYPE_VD8:
            eFifoType = E_TSP_FILE_2_VIDEO8;
            break;
        default:
            _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
            break;
    }

    MDrv_TSP_AVFifo_SourceSelect(eTspTSid,eFifoType);

    #endif  // End of TSP_VER_4_0

  #else //else MMFI_VER_2_0

    if(MDrv_MMFI_PidFlt_Alloc(type, pu8DmxId) == DRVMMFI_FAIL)
    {
        _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
    }
  #endif //endif MMFI_VER_2_0

    if(MDrv_MMFI_PidFlt_SetPid(*pu8DmxId, u16Pid) == DRVMMFI_FAIL)
    {
        _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    if(MDrv_MMFI_PidFlt_Enable(*pu8DmxId, TRUE) == DRVMMFI_FAIL)
    {
        _DMX_MMFI_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_MMFI_RETURN(DMX_FILTER_STATUS_OK);
#endif  // End of MMFILEIN
}

//-------------------------------------------------------------------------------------------------
/// Free the PID of MM Filein
/// @param u8DmxId \b IN: The demux filter Id from MApi_DMX_Pvr_Pid_Open
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_Pid_Close(MS_U8 u8DmxId)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifndef MMFILEIN
    return DMX_FILTER_STATUS_ERROR;
#else

    if(MDrv_MMFI_PidFlt_Free(u8DmxId) == DRVMMFI_FAIL)
    {
        return (DMX_FILTER_STATUS_ERROR);
    }

    return (DMX_FILTER_STATUS_OK);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Get MMFilein Command queue fifo level
/// @param  ePath                             \b IN: MM File-in Path
/// @param  pu8CmdQStatus                  \b OUT: fifo level, 0~3
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_Filein_CMDQ_FIFOWriteLevel(DMX_MMFI_PATH ePath, MS_U8 *pu8CmdQStatus)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifndef MMFILEIN
    return DMX_FILTER_STATUS_ERROR;
#else
    MDrv_MMFI_File_CmdQFifo_Status((DrvMMFI_Path)ePath, pu8CmdQStatus);
    return DMX_FILTER_STATUS_OK;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Abort to get bit stream by memeory
/// @param  ePath                      \b IN: MM File-in Path
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_Abort(DMX_MMFI_PATH ePath)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifndef MMFILEIN
        return DMX_FILTER_STATUS_ERROR;
#else
    if (DRVMMFI_OK!= MDrv_MMFI_File_Abort((DrvMMFI_Path)ePath))
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    return DMX_FILTER_STATUS_OK;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set MMFI playback timestamp
/// @param  ePath                          \b IN: MM File-in Path
/// @param u32pcr2                       \b IN: LPCR2 value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_SetPlaybackTimeStamp(DMX_MMFI_PATH ePath, MS_U32 u32pcr2)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifndef MMFILEIN
    return DMX_FILTER_STATUS_ERROR;
#else
    _DMX_MMFI_ENTRY();
    MDrv_MMFI_SetPlaybackTimeStamp((DrvMMFI_Path)ePath, u32pcr2);
    _DMX_MMFI_RETURN(DMX_FILTER_STATUS_OK);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Get MMFI Playback timestamp
/// @param ePath                          \b IN: MM File-in Path
/// @param pu32pcr2                     \b OUT: pointer to store LCPR2 value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_GetPlaybackTimeStamp(DMX_MMFI_PATH ePath, MS_U32 *pu32pcr2)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifndef MMFILEIN
        return DMX_FILTER_STATUS_ERROR;
#else
    MDrv_MMFI_GetPlaybackTimeStamp((DrvMMFI_Path)ePath, pu32pcr2);
    return DMX_FILTER_STATUS_OK;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Enable remove duplicate A/V packets
/// @param  bEnable                \b IN: Enable or Disable
/// @return DMX_FILTER_STATUS
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_RemoveDupAVPkt(MS_BOOL bEnable)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef MMFILEIN
    if(MDrv_MMFI_RemoveDupAVPkt(bEnable) == DRVMMFI_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }
#endif
    return DMX_FILTER_STATUS_ERROR;
}

//-------------------------------------------------------------------------------------------------
/// Enable MMFI timestamp mode
/// @param ePath                          \b IN: MM File-in Path
/// @return DMX_FILTER_STATUS
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_TimeStampEnable(DMX_MMFI_PATH ePath)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifndef MMFILEIN
    return DMX_FILTER_STATUS_ERROR;
#else
    if(MDrv_MMFI_File_192Mode_En((DrvMMFI_Path)ePath, TRUE) == TRUE)
    {
        #ifdef DMX_RESET_FI_TIMESTAMP
        _pdmx_res->_bPlayTimestampEnable[ePath+1UL] = TRUE;
        #endif

        return DMX_FILTER_STATUS_OK;
    }
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Disable MMFI timestamp mode
/// @param ePath                          \b IN: MM File-in Path
/// @return DMX_FILTER_STATUS
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MMFI_TimeStampDisable(DMX_MMFI_PATH ePath)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifndef MMFILEIN
    return DMX_FILTER_STATUS_ERROR;
#else
    if(MDrv_MMFI_File_192Mode_En((DrvMMFI_Path)ePath, FALSE) == TRUE)
    {
        #ifdef DMX_RESET_FI_TIMESTAMP
        _pdmx_res->_bPlayTimestampEnable[ePath+1] = FALSE;
        #endif

        return DMX_FILTER_STATUS_OK;
    }
    return DMX_FILTER_STATUS_ERROR;
#endif
}

DMX_FILTER_STATUS _MApi_DMX_MMFI_MOBF_Enable(DMX_MMFI_PATH ePath, MS_BOOL bEnable, MS_U32 u32key)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef MM_MOBF
    MDrv_MMFI_Set_MOBF_FileinKey((DrvMMFI_Path)ePath, u32key);
    MDrv_MMFI_MOBF_FileIn_Enable((DrvMMFI_Path)ePath, bEnable);
    return DMX_FILTER_STATUS_OK;
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

#if !(defined(MMFI_VERSION) && (MMFI_VERSION >= MMFI_VER_2_0))
DMX_FILTER_STATUS MApi_DMX_MMFI_MOBF_SetLevel(DMX_MMFI_PATH epath, MS_U8 u8level)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef MM_MOBF
    MDrv_MMFI_MOBF_FileIn_SetLevel((DrvMMFI_Path)epath, u8level);
    return DMX_FILTER_STATUS_OK;
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}
#endif //MMFI_VER_2_0

DMX_FILTER_STATUS _MApi_DMX_MMFI_TimeStampClk(DMX_MMFI_PATH ePath, DMX_TimeStamp_Clk eClk)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef MMFILEIN
    if(DRVMMFI_OK == MDrv_MMFI_File_SetTimeStampClk((DrvMMFI_Path)ePath, (MS_U32)eClk))
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }

#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Enable MOBF encrypt
/// @param bEnable \b IN: PVR MOBF Enable/Disable
/// @param u32key0 \b IN: MOBF encrypt key0
/// @param u32key1 \b IN: MOBF encrypt key1
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pvr_MOBF_Enable(MS_BOOL bEnable, MS_U32 u32key0, MS_U32 u32key1)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
    if(bEnable == TRUE)
    {
        MDrv_TSP_Set_MOBF_Set(1, u32key0, u32key1);
    }
    else
    {
        MDrv_TSP_Set_MOBF_Set(1, 0, 0);
    }
    return DMX_FILTER_STATUS_OK;
#elif(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))
    if(!bEnable)
    {
        MDrv_TSP_MOBF_PVR_Enable(FALSE);
        return DMX_FILTER_STATUS_OK;
    }

    MDrv_TSP_Set_MOBF_PVRKey(u32key0, u32key1);
    MDrv_TSP_MOBF_PVR_Enable(TRUE);
    return DMX_FILTER_STATUS_OK;
#else
    return DMX_FILTER_STATUS_ERROR;
#endif


}

//-------------------------------------------------------------------------------------------------
/// Enable MOBF encrypt
/// @param u8Eng \b IN: MOBF Engine ID
/// @param bEnable \b IN: PVR MOBF Enable/Disable
/// @param u32key0 \b IN: MOBF encrypt key0
/// @param u32key1 \b IN: MOBF encrypt key1
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Pvr_Eng_MOBF_Enable(MS_U8 u8Eng, MS_BOOL bEnable, MS_U32 u32key0, MS_U32 u32key1)
{
#if defined(MOBF_ENABLE)

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

  #if(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
    if(bEnable == TRUE)
    {
        MDrv_TSP_Set_MOBF_Set(u8Eng+1, u32key0, u32key1);
    }
    else
    {
        MDrv_TSP_Set_MOBF_Set(u8Eng+1, 0, 0);
    }
    return DMX_FILTER_STATUS_OK;

  #elif(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))

    MS_U32 u32MobfKey0 = u32key0, u32MobfKey1 = u32key1;

    if(bEnable == FALSE)
    {
        u32MobfKey0 = 0;
        u32MobfKey1 = 0;
    }

    if(u8Eng == 0)
    {
        MDrv_TSP_Set_MOBF_PVRKey(u32MobfKey0, u32MobfKey1);
    }
    else
    {
        MDrv_TSP_Set_MOBF_PVR1Key(u32MobfKey0, u32MobfKey1);
    }

    return DMX_FILTER_STATUS_OK;

  #else  // For TSP_VER_4_0

    MDrv_TSP_PVR_Eng_MOBF_Enable(u8Eng, bEnable, u32key0);
    return DMX_FILTER_STATUS_OK;

  #endif // End of TSP_VER_4_0

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Enable MOBF decrypt key
/// @param bEnable \b IN: File-in MOBF Enable/Disable
/// @param u32key   \b IN: MOBF dyncrypt u32key
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Filein_MOBF_Enable(MS_BOOL bEnable, MS_U32 u32key)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_3_0))
    if(bEnable == TRUE)
    {
        MDrv_TSP_Set_MOBF_Set(0, u32key, 0);
    }
    else
    {
        MDrv_TSP_Set_MOBF_Set(0, 0, 0);
    }
    return DMX_FILTER_STATUS_OK;

#elif(defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0))
    if(!bEnable)
    {
        MDrv_TSP_MOBF_FileIn_Enable(FALSE);
        return DMX_FILTER_STATUS_OK;
    }

    MDrv_TSP_Set_MOBF_FileinKey(u32key);
    MDrv_TSP_MOBF_FileIn_Enable(TRUE);
    return DMX_FILTER_STATUS_OK;

#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

/********************************************************************/
/// Enable or Disable VQ
///@param bEnable           \b IN: Enable/Disable
///@return DMX_FILTER_STATUS
/********************************************************************/
DMX_FILTER_STATUS _MApi_DMX_VQ_Enable(MS_BOOL bEnable)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#if (defined(VQ_ENABLE) && (defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))
    _DMX_ENTRY();
    MDrv_TSP_VQueEnable(bEnable);

  #if (defined(TS2_IF_SUPPORT) && (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_1_0)))
    _pdmx_res->_bVQEnabled = bEnable;
  #endif

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif //VQ_ENABLE
}

/********************************************************************/
///MApi_DMX_FilterFlowSwitch : Switch filter source from one channel to another.
///@param SrcFlow, TgtFlow
/// TSP1.0:
/// DMX_FILTER_SOURCE_TYPE_LIVE
/// DMX_FILTER_SOURCE_TYPE_TS1
/// TSP2.0:
/// DMX_FILTER_SOURCE_TYPE_PLAYBACK
/// DMX_FILTER_SOURCE_TYPE_PVR0
/// DMX_FILTER_SOURCE_TYPE_PVR1
/// TSP3.0:
/// DMX_FILTER_SOURCE_TYPE_LIVE
/// DMX_FILTER_SOURCE_TYPE_FILE
/// DMX_FILTER_SOURCE_TYPE_TS1
/// DMX_FILTER_SOURCE_TYPE_TS2
/// DMX_FILTER_SOURCE_TYPE_TS3
///@return DMX_FILTER_STATUS
/********************************************************************/
DMX_FILTER_STATUS _MApi_DMX_LiveSrcSwitch(DMX_FILTER_TYPE TgtFlow)
{
    _DMX_ENTRY();

    MDrv_TSP_FLT_LiveSrcSwitch(TgtFlow&DMX_FILTER_SOURCE_TYPE_MASK);

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
}

/********************************************************************/
/// Enable or Disable Audio AB S
///@param bEnable           \b IN: Enable/Disable
///@return DMX_FILTER_STATUS
/********************************************************************/
DMX_FILTER_STATUS _MApi_DMX_AU_BD_Mode_Enable(MS_BOOL bEnable)
{
    _DMX_ENTRY();

    if(MDrv_TSP_AU_BD_Mode_Enable(bEnable) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
}

//-------------------------------------------------------------------------------------------------
/// Set up parameters for input bit stream from memory of TSO path
/// @param u8Eng       \b IN: TSO engine ID
/// @param pFileinInfo \b IN: the file in parameters
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_Filein_Info(MS_U8 u8Eng, DMX_Filein_info *pFileinInfo)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();
    MDrv_TSO_SetPacketMode(u8Eng, (DrvTSO_PacketMode)(pFileinInfo->PKT_Mode));
    MDrv_TSO_Filein_Rate(u8Eng, (MS_U16)(pFileinInfo->Rate & 0xFFFF));
    _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Check if no memory transfer is under going of TSO path
/// @param u8Eng       \b IN: TSO engine ID
/// @return TRUE if idle, FALSE otherwise
/// @note
//-------------------------------------------------------------------------------------------------
MS_BOOL _MApi_DMX_TSO_Filein_IsIdle(MS_U8 u8Eng)
{
#ifdef TSO_ENABLE
    DMX_TSOMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    DrvTSO_FileinState     TsoFileinState;

    if (DRVTSO_OK!= MDrv_TSO_Filein_GetState(u8Eng, &TsoFileinState))
    {
        DMX_ASSERT(0, DMX_DBGMSG(DMX_DBG_ERR, ULOGD("DMX", "[MAPI DMX][%06d] Call MDrv_TSO_Filein_GetState fail\n", __LINE__)));
    }

    return (E_DRVTSO_STATE_IDLE == TsoFileinState);
#else

    return FALSE;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Get TSO file-in CMDQ empty number
/// @param u8Eng       \b IN: TSO engine ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_Filein_CMDQ_GetEmptyNum(MS_U8 u8Eng, MS_U32 *pu32EmptySlot)
{
    DMX_TSOMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE
    MDrv_TSO_Filein_CmdQ_GetSlot(u8Eng, pu32EmptySlot);
#endif

    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get TSO file-in CMDQ reset
/// @param u8Eng       \b IN: TSO engine ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_Filein_CMDQ_Reset(MS_U8 u8Eng)
{
    DMX_TSOMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();
    MDrv_TSO_Filein_CmdQ_Reset(u8Eng);
    _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
#endif

    return DMX_FILTER_STATUS_OK;
}


//-------------------------------------------------------------------------------------------------
/// Start to get bit stream by memeory of TSO path
/// @param u8Eng            \b IN: TSO engine ID
/// @param pBuf              \b IN: the memory containing the bit stream
/// @param u32BufSize    \b IN: the size the memory to get
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_Filein_Start(MS_U8 u8Eng, MS_PHY pBuf, MS_U32 u32BufSize)
{
    DMX_TSOMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();
    if (DRVTSO_OK != MDrv_TSO_Filein_SetAddr(u8Eng, pBuf))
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    if (DRVTSO_OK != MDrv_TSO_Filein_SetSize(u8Eng, u32BufSize))
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    if (DRVTSO_OK!= MDrv_TSO_Filein_Start(u8Eng))
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Stop to get bit stream by memeory of TSO path
/// @param u8Eng            \b IN: TSO engine ID
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_Filein_Stop(MS_U8 u8Eng)
{
    DMX_TSOMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();
    if (DRVTSO_OK != MDrv_TSO_Filein_Stop(u8Eng))
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Set TSO playback time stamp
/// @param u8Eng                  \b IN: TSO engine ID
/// @param u32Stamp            \b OUT: pointer to store timestamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_SetPlaybackTimeStamp(MS_U8 u8Eng, MS_U32 u32Stamp)
{
    DMX_TSOMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d (%u)\n", __FUNCTION__, __LINE__, (unsigned int)u32Stamp));

#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();
    if(DRVTSO_OK != MDrv_TSO_SetPlaybackTimeStamp(u8Eng, u32Stamp))
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);

#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Get TSO playback time stamp
/// @param u8Eng                  \b IN: TSO engine ID
/// @param pu32Stamp          \b OUT: pointer to store timestamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_GetPlaybackStamp(MS_U8 u8Eng, MS_U32* pu32Stamp)
{
    DMX_TSOMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d (0x%lx)\n", __FUNCTION__, __LINE__, (unsigned long)pu32Stamp));

#ifdef TSO_ENABLE
    return ((DRVTSO_OK== MDrv_TSO_GetPlaybackTimeStamp(u8Eng, pu32Stamp))? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Get TSO file-in  time stamp
/// @param u8Eng                  \b IN: TSO engine ID
/// @param pu32Stamp          \b OUT: pointer to store timestamp value
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_GetFileInTimeStamp(MS_U8 u8Eng, MS_U32 *pu32Stamp)
{
    DMX_TSOMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE
    MDrv_TSO_GetFileInTimeStamp(u8Eng, pu32Stamp);
    //ULOGD("DMX", "MApi_DMX_TSO_GetFileInTimeStamp %x\n", (int)(*pu32Stamp));
#endif
    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get TSO file-in  read address
/// @param u8Eng                  \b IN: TSO engine ID
/// @param pu32Read            \b OUT: pointer to store read address
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_Filein_GetReadAddr(MS_U8 u8Eng, MS_PHY* pphyRead)
{
    DMX_TSOMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();
    if(MDrv_TSO_Filein_GetReadAddr(u8Eng, pphyRead) != DRVTSO_OK)
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    else
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
    }
#else
    return DMX_FILTER_STATUS_OK;
#endif

}

//-------------------------------------------------------------------------------------------------
/// Disable 192 mode block scheme to bypass fill-in timestamp
/// @param bbypass                  \b IN: If true, bypass file-in timestamp.
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_BypassFileInTimeStamp(MS_U8 u8Eng, MS_BOOL bbypass)
{
    DMX_TSOMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();
    if(DRVTSO_OK == MDrv_TSO_Filein_192BlockScheme_En(u8Eng, !bbypass))
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }

#endif

    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Enable TSO file in  time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_TimeStampEnable(MS_U8 u8Eng)
{
    DMX_TSOMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY()
    if(DRVTSO_OK== MDrv_TSO_Filein_192Mode_En(u8Eng, TRUE))
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }

#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Disable TSO file in time stamp
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_TimeStampDisable(MS_U8 u8Eng)
{
    DMX_TSOMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();
    if(DRVTSO_OK== MDrv_TSO_Filein_192Mode_En(u8Eng, FALSE))
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
 #else

    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set TSO Out Clock
/// @param  u8Eng                                         \b IN: TSO engine ID
/// @param  eTsOutClk                                    \b IN: TSO out clock select
/// @param  eTsOutClkSrc                               \b IN: TSO out clock source select
/// @param  u16DivNum                                  \b IN: If select DMPLLDIV source, setting Divide number
/// @param  bClkInv                                        \b IN: If Out Clock invert
/// @return TSO_Result
/// @note
/// If eTsOutClk is E_DMX_TSO_OUTCLK_DIV2N, eTsOutClkSrc could be E_DMX_TSO_OUTCLKSRC_172M_2N/E_DMX_TSO_OUTCLKSRC_288M_2N/E_DMX_TSO_OUTCLKSRC_432M_2N,
/// and user should set u16DivNum to generate final output clock.
/// If eTsOutClk is E_DMX_TSO_OUTCLK_DIVN, eTsOutClkSrc could be E_DMX_TSO_OUTCLKSRC_216M_N, and user should set u16DivNum to generate final output clock.
/// If eTsOutClk is E_DMX_TSO_OUTCLK_PTSOOUT/E_DMX_TSO_OUTCLK_PTSOOUT_DIV8, eTsOutClkSrc could be E_DMX_TSO_OUTCLKSRC_P_TS0IN/E_DMX_TSO_OUTCLKSRC_P_TS1IN, etc,
/// and this colck is from external pad
/// E_DMX_TSO_OUTCLK_62M/E_DMX_TSO_OUTCLK_54M/E_DMX_TSO_OUTCLK_27M/E_DMX_TSO_OUTCLK_Dmd are constant values of output clock.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_SetOutClk(MS_U8 u8Eng, DMX_TSO_OutClk eTsOutClk, DMX_TSO_OutClkSrc eTsOutClkSrc, MS_U16 u16DivNum, MS_BOOL bClkInv)
{
    DMX_TSOMSG(DMX_DBG_FUNC, ULOGE("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();
    if(DRVTSO_OK== MDrv_TSO_SetOutClk(u8Eng, (DrvTSO_OutClk)eTsOutClk, (DrvTSO_OutClkSrc)eTsOutClkSrc, u16DivNum, bClkInv))
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif

}

//-------------------------------------------------------------------------------------------------
/// Enable or disable TSO output
/// @param  u8Eng                                         \b IN: TSO engine ID
/// @param  bEnable                                       \b IN: TRUE is enable, FALSE is disable
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_OutputEnable(MS_U8 u8Eng, MS_BOOL bEnable)
{
#ifdef TSO_ENABLE
    _DMX_TSO_ENTRY();

    if(MDrv_TSO_OutputEnable(u8Eng, bEnable) != DRVTSO_OK)
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif

}

//TSO2
//-------------------------------------------------------------------------------------------------
/// Set or get local stream ID of TSO Out
/// @param  u8Eng                                         \b IN: TSO engine ID
/// @param  eIf                                        \b IN: TSO input TSIF
/// @param  pu8StrId                                   \b IN or OUT: pointer to store local stream ID, default value is 0x47
/// @param  bSet                                       \b IN: If TRUE, set local stream id, otherwise get local stream id
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_LocalStreamId(MS_U8 u8Eng, DMX_TSIF eIf, MS_U8* pu8StrId, MS_BOOL bSet)
{
    DMX_TSOMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_20_ENABLE
    DrvTSO_If eTsoIf = E_DRVTSO_IF_LIVE0;

    switch(eIf)
    {
        case DMX_TSIF_LIVE0:
            eTsoIf = E_DRVTSO_IF_LIVE0;
            break;
        case DMX_TSIF_LIVE1:
            eTsoIf = E_DRVTSO_IF_LIVE1;
            break;
        case DMX_TSIF_LIVE2:
            eTsoIf = E_DRVTSO_IF_LIVE2;
            break;
        case DMX_TSIF_FILE0:
            eTsoIf = E_DRVTSO_IF_FILE0;
            break;
        case DMX_TSIF_FILE1:
            eTsoIf = E_DRVTSO_IF_FILE1;
            break;
        default:
            return DMX_FILTER_STATUS_ERROR;
    }

    _DMX_TSO_ENTRY();
    if(DRVTSO_OK== MDrv_TSO_LocalStreamId(u8Eng, eTsoIf, pu8StrId, bSet))
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif

}

//-------------------------------------------------------------------------------------------------
/// Set TSO virtual queue buffer
/// @param  u8Eng                                         \b IN: TSO engine ID
/// @param  u32Addr                                      \b IN: TSO VQ buffer address
/// @param  u32BufSize                                     \b IN: TSO VQ buffer size
/// @return TSO_Result
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_SVQBuf_Set(MS_U8 u8Eng, MS_PHY phyAddr, MS_U32 u32BufSize)
{
    DMX_TSOMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));
#ifdef TSO_20_ENABLE
    _DMX_TSO_ENTRY();
    if(DRVTSO_OK == MDrv_TSO_Set_SVQBuf(0,  phyAddr, u32BufSize))
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
///TSO (TS output) Demux Flow input sources configure getting and setting
/// @param eFlow                         \b IN: DMX TSO playback flow,  DMX_FLOW_TSO_PLAYBACK/DMX_FLOW_TSO_PLAYBACK1
/// @param eTSOInIf                     \b IN: TSO input interface,  DMX_TSIF_LIVE0/DMX_TSIF_LIVE1/DMX_TSIF_LIVE2/DMX_TSIF_FILE0/DMX_TSIF_FILE1
///
/// @param stInputInfo.Input               \b        IN\OUT: TSO input interface source,  enum item of DMX_FLOW_INPUT
/// @param stInputInfo.bClkInv            \b         IN\OUT: If TSO input interface source is from demod, please set clock invert type
/// @param stInputInfo.bExtSync         \b         IN\OUT: If TSO input interface source is from demod, please set external sync type
/// @param stInputInfo.bParallel           \b        IN\OUT: If TSO input interface source is from demod, please set parallel or serial type
///
/// @param u8LocalStrId               \b              IN\OUT: The local stream ID (TS output sync byte) value, default is 0x47
/// @param bBypassAll                  \b              IN\OUT: TRUE means bypass all ts data for TSO playback; FALSE means tso will output ts data by PIDs
/// @param bEnable                     \b               IN\OUT: TRUE means enable TSO input now; FALSE means disable this TSO input fource
///
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
///
/// @note This API is for TSO2 HW architecture
/// @note Serval TSO input will be merge to be one TSO output. This API is for configuring one of TSO input
/// @note If there are 3 TSO input source, you shold call this API 3 times for every TSO input path configure
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_Flow_InputCfg(DMX_TSO_InputCfg* pstInputCfg)
{
    DMX_TSOMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_20_ENABLE

    if(pstInputCfg->bSet == TRUE)
    {
        _DMX_TSO_ENTRY();

        if(_TSO_Flow_InputCfg(pstInputCfg) == TRUE)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
        }
        else
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }
    }
    else
    {
        DrvTSO_If stIf;
        MS_U8 u8Eng = 0;
        DrvTSO_If_Set stIfSet;

        if(pstInputCfg->eFlow == DMX_FLOW_TSO_PLAYBACK)
        {
            u8Eng = 0;
        }
        else if(pstInputCfg->eFlow == DMX_FLOW_TSO_PLAYBACK1)
        {
            u8Eng = 1;
        }
        else
        {
            return DMX_FILTER_STATUS_ERROR;
        }

        switch(pstInputCfg->eTSOInIf)
        {
            case DMX_TSIF_LIVE0:
                stIf = E_DRVTSO_IF_LIVE0;
                break;
            case DMX_TSIF_LIVE1:
                stIf = E_DRVTSO_IF_LIVE1;
                break;
            case DMX_TSIF_LIVE2:
                stIf = E_DRVTSO_IF_LIVE2;
                break;
            case DMX_TSIF_LIVE3:
                stIf = E_DRVTSO_IF_LIVE3;
                break;
            case DMX_TSIF_FILE0:
                stIf = E_DRVTSO_IF_FILE0;
                break;
            case DMX_TSIF_FILE1:
                stIf = E_DRVTSO_IF_FILE1;
                break;
            default:
                return DMX_FILTER_STATUS_ERROR;
        }

        if((pstInputCfg->eTSOInIf == DMX_TSIF_FILE0) && (pstInputCfg->eTSOInIf == DMX_TSIF_FILE1))
        {
            pstInputCfg->stInputInfo.Input = DMX_FLOW_INPUT_MEM;
            return DMX_FILTER_STATUS_OK;
        }

        _DMX_TSO_ENTRY();

        if(MDrv_TSO_LocalStreamId(u8Eng, stIf, &(pstInputCfg->u8LocalStrId), FALSE) == FALSE)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }

        if(MDrv_TSO_GetTSIFStatus(u8Eng, stIf, &stIfSet, &pstInputCfg->bBypassAll, &pstInputCfg->bEnable) != DRVTSO_OK)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }

        switch(stIfSet.ePad)
        {
            case E_DRVTSO_PAD_EXT_INPUT0:
                pstInputCfg->stInputInfo.Input = DMX_FLOW_INPUT_EXT_INPUT0;
                break;
            case E_DRVTSO_PAD_EXT_INPUT1:
                pstInputCfg->stInputInfo.Input = DMX_FLOW_INPUT_EXT_INPUT1;
                break;
            case E_DRVTSO_PAD_EXT_INPUT2:
                pstInputCfg->stInputInfo.Input = DMX_FLOW_INPUT_EXT_INPUT2;
                break;
            case E_DRVTSO_PAD_EXT_INPUT3:
                pstInputCfg->stInputInfo.Input = DMX_FLOW_INPUT_EXT_INPUT3;
                break;
            case E_DRVTSO_PAD_EXT_INPUT4:
                pstInputCfg->stInputInfo.Input = DMX_FLOW_INPUT_EXT_INPUT4;
                break;
            case E_DRVTSO_PAD_EXT_INPUT5:
                pstInputCfg->stInputInfo.Input = DMX_FLOW_INPUT_EXT_INPUT5;
                break;
            case E_DRVTSO_PAD_DEMOD:
                pstInputCfg->stInputInfo.Input = DMX_FLOW_INPUT_DEMOD;
                break;
            default:
                pstInputCfg->stInputInfo.Input = DMX_FLOW_INPUT_MEM;
                _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
        }
        pstInputCfg->stInputInfo.bClkInv = stIfSet.bClkInv;
        pstInputCfg->stInputInfo.bExtSync = stIfSet.bExtSync;
        pstInputCfg->stInputInfo.bParallel = stIfSet.bParallel;

        _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);

    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif

}

//-------------------------------------------------------------------------------------------------
///TSO (TS output) Demux Flow output path configure
/// @param eFlow                        \b IN: DMX TSO playback flow,  DMX_FLOW_TSO_PLAYBACK/DMX_FLOW_TSO_PLAYBACK1
/// @param eOutPad                    \b IN: TSO output pad  select,  DMX_FLOW_OUTPUT_EXT_PAD1/DMX_FLOW_OUTPUT_EXT_PAD3///
/// @param u16OutPktSize            \b IN/OUT: TSO output packet size. default vale is 188 bytes
/// @param bEnable                     \b IN/OUT: TRUE means enable TSO output now; FALSE means disable TSO output
///
/// @param bDefOutClk                 \b IN: TURE means using default clock setting given by driver; FALSE means set output clock by user parameters
/// @param bOutClkInv                 \b IN/OUT: Set inver type of TSO output clock
/// @param eTsoOutClk                \b IN/OUT: Select TSO output clock, enum item of DMX_TSO_OutClk
/// @param eTsoOutClkSrc            \b IN/OUT: Select TSO output clock source, enum item of DMX_TSO_OutClkSrc
/// @param u16DivNum                 \b IN/OUT: If TSO output clock source is E_DMX_TSO_OUTCLK_DIV2N/E_DMX_TSO_OUTCLK_DIVN, set this value for clock generatng.
///                                                            Other clock sources will not use this parameter
///
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
///
/// @note This API is for TSO2 HW architecture
/// @note Serval TSO input will be merge to be one TSO output. This API is for configuring TSO output path
/// @note TSO output clock source selection, please also refer to MApi_DMX_TSO_SetOutClk API
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_Flow_OutputCfg(DMX_TSO_OutputCfg* pstOutputCfg)
{
    DMX_TSOMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_20_ENABLE

    if(pstOutputCfg->bSet == TRUE)
    {
        _DMX_TSO_ENTRY();
        if(_TSO_Flow_OutputCfg(pstOutputCfg) == TRUE)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
        }
        else
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }
    }
    else
    {
        MS_U8 u8Eng = 0;
        DrvTSO_PadOut eOutPad = E_DRVTSO_PAD_NONE;
        DrvTSO_OutClk eOutClk = 0xFF;
        DrvTSO_OutClkSrc eOutClkSrc = 0xFF;

        switch(pstOutputCfg->eFlow)
        {
            case DMX_FLOW_TSO_PLAYBACK:
                u8Eng = 0;
                break;
            case DMX_FLOW_TSO_PLAYBACK1:
                u8Eng = 1;
                break;
            default:
                return DMX_FILTER_STATUS_ERROR;
        }

        _DMX_TSO_ENTRY();
        if(MDrv_TSO_OutputPktSize(u8Eng, &(pstOutputCfg->u16OutPktSize), FALSE) != DRVTSO_OK)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }
        if(MDrv_TSO_OutputPad(0, &eOutPad, FALSE) != DRVTSO_OK)
        {
            _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
        }

        if(pstOutputCfg->bDefOutClk == TRUE)
        {
            pstOutputCfg->eTsoOutClk = E_DRVTSO_OUTCLK_DIV2N;
            pstOutputCfg->eTsoOutClkSrc = E_DRVTSO_OUTCLKSRC_172M_2N;
            pstOutputCfg->u16DivNum = 0x0F;
            pstOutputCfg->bOutClkInv = FALSE;
        }
        else
        {
            if(MDrv_TSO_GetOutClk(u8Eng, &eOutClk, &eOutClkSrc, &(pstOutputCfg->u16DivNum), &(pstOutputCfg->bOutClkInv)) != DRVTSO_OK)
            {
                _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
            }
            switch(eOutClk)
            {
                case E_DRVTSO_OUTCLK_DIV2N:
                    pstOutputCfg->eTsoOutClk = E_DMX_TSO_OUTCLK_DIV2N;
                    break;
                case E_DRVTSO_OUTCLK_DIVN:
                    pstOutputCfg->eTsoOutClk = E_DMX_TSO_OUTCLK_DIVN;
                    break;
                case E_DRVTSO_OUTCLK_62M:
                    pstOutputCfg->eTsoOutClk = E_DMX_TSO_OUTCLK_62M;
                    break;
                case E_DRVTSO_OUTCLK_54M:
                    pstOutputCfg->eTsoOutClk = E_DMX_TSO_OUTCLK_54M;
                    break;
                case E_DRVTSO_OUTCLK_PTSOOUT:
                    pstOutputCfg->eTsoOutClk = E_DMX_TSO_OUTCLK_PTSOOUT;
                    break;
                case E_DRVTSO_OUTCLK_PTSOOUT_DIV8:
                    pstOutputCfg->eTsoOutClk = E_DMX_TSO_OUTCLK_PTSOOUT_DIV8;
                    break;
                case E_DRVTSO_OUTCLK_Dmd:
                    pstOutputCfg->eTsoOutClk = E_DMX_TSO_OUTCLK_Dmd;
                    break;
                case E_DRVTSO_OUTCLK_27M:
                    pstOutputCfg->eTsoOutClk = E_DMX_TSO_OUTCLK_27M;
                    break;
                default:
                    _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
            }
            switch(eOutClkSrc)
            {
                case E_DRVTSO_OUTCLKSRC_172M_2N:
                    pstOutputCfg->eTsoOutClkSrc = E_DMX_TSO_OUTCLKSRC_172M_2N;
                    break;
                case E_DRVTSO_OUTCLKSRC_288M_2N:
                    pstOutputCfg->eTsoOutClkSrc = E_DMX_TSO_OUTCLKSRC_288M_2N;
                    break;
                case E_DRVTSO_OUTCLKSRC_432M_2N:
                    pstOutputCfg->eTsoOutClkSrc = E_DMX_TSO_OUTCLKSRC_432M_2N;
                    break;
                case E_DRVTSO_OUTCLKSRC_216M_N:
                    pstOutputCfg->eTsoOutClkSrc = E_DMX_TSO_OUTCLKSRC_216M_N;
                    break;
                case E_DRVTSO_OUTCLKSRC_P_TS0IN:
                    pstOutputCfg->eTsoOutClkSrc = E_DMX_TSO_OUTCLKSRC_P_TS0IN;
                    break;
                case E_DRVTSO_OUTCLKSRC_P_TS1IN:
                    pstOutputCfg->eTsoOutClkSrc = E_DMX_TSO_OUTCLKSRC_P_TS1IN;
                    break;
                case E_DRVTSO_OUTCLKSRC_P_TS2IN:
                    pstOutputCfg->eTsoOutClkSrc = E_DMX_TSO_OUTCLKSRC_P_TS2IN;
                    break;
                case E_DRVTSO_OUTCLKSRC_P_TS3IN:
                    pstOutputCfg->eTsoOutClkSrc = E_DMX_TSO_OUTCLKSRC_P_TS3IN;
                    break;
                case E_DRVTSO_OUTCLKSRC_P_TS4IN:
                    pstOutputCfg->eTsoOutClkSrc = E_DMX_TSO_OUTCLKSRC_P_TS4IN;
                    break;
                case E_DRVTSO_OUTCLKSRC_P_TS5IN:
                    pstOutputCfg->eTsoOutClkSrc = E_DMX_TSO_OUTCLKSRC_P_TS5IN;
                    break;
                default:
                    break;
            }
        }

        _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif

}

//-------------------------------------------------------------------------------------------------
///TSO configuration
/// @param pstTsoCfg                \b IN/OUT: Pointer to store configure data
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
///
/// @note This API is for TSO2 HW architecture
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_Configure(DMX_TSO_Cfg* pstTsoCfg)
{
    DMX_TSOMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef TSO_20_ENABLE

    DrvTSO_Cfg stDrvTsoCfg;
    memset(&stDrvTsoCfg, 0x0, sizeof(stDrvTsoCfg));

    stDrvTsoCfg.u32TsoEng    = pstTsoCfg->u32TsoEng;
    stDrvTsoCfg.bSet         = pstTsoCfg->bSet;
    stDrvTsoCfg.eCfgCmd      = pstTsoCfg->eCfgCmd;
    stDrvTsoCfg.u32CmdData0  = pstTsoCfg->u32CmdData0;

    if(MDrv_TSO_Configure(&stDrvTsoCfg) == DRVTSO_OK)
    {
        return DMX_FILTER_STATUS_OK;
    }
    else
    {
        return DMX_FILTER_STATUS_ERROR;
    }

#else
    return DMX_FILTER_STATUS_ERROR;
#endif

}

//-------------------------------------------------------------------------------------------------
///TSO (TS output) input pid filter open
/// @param u8Eng                    \b IN: TSO engine ID
/// @param eTSOInSrc                \b IN: TSO PID filter source
/// @param u16Pid                       \b IN: PID value of TSO PID filter
/// @param pu16DmxId                    \b IN: Pointer to store PID filter ID
///
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
///
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_Pid_Open(MS_U8 u8Eng, DMX_TSIF eTSOInSrc, MS_U16 u16Pid, MS_U16* pu16DmxId)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s][%d](%x, %x, %x, %p)\n", __FUNCTION__, __LINE__, (unsigned int)u8Eng, eTSOInSrc, u16Pid, pu16DmxId));

#ifdef TSO_20_ENABLE
    DrvTSO_If eTsoif = E_DRVTSO_IF_LIVE0;

    switch(eTSOInSrc)
    {
        case DMX_TSIF_LIVE0:
            eTsoif = E_DRVTSO_IF_LIVE0;
            break;
        case DMX_TSIF_LIVE1:
            eTsoif = E_DRVTSO_IF_LIVE1;
            break;
        case DMX_TSIF_LIVE2:
            eTsoif = E_DRVTSO_IF_LIVE2;
            break;
        case DMX_TSIF_LIVE3:
            eTsoif = E_DRVTSO_IF_LIVE3;
            break;
        case DMX_TSIF_FILE0:
            eTsoif = E_DRVTSO_IF_FILE0;
            break;
        case DMX_TSIF_FILE1:
            eTsoif = E_DRVTSO_IF_FILE1;
            break;
        default:
            return DMX_FILTER_STATUS_ERROR;
    }

    _DMX_TSO_ENTRY();
    if(MDrv_TSO_PidFlt_Alloc(u8Eng, pu16DmxId) != DRVTSO_OK)
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    if(MDrv_TSO_PidFlt_SetPid(u8Eng, *pu16DmxId, u16Pid) != DRVTSO_OK)
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    if(MDrv_TSO_PidFlt_SetChSrc(u8Eng, *pu16DmxId, eTsoif) != DRVTSO_OK)
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);

#else
    return DMX_FILTER_STATUS_ERROR;
#endif

}

//-------------------------------------------------------------------------------------------------
///TSO (TS output) input pid filter close
/// @param u8Eng                    \b IN: TSO engine ID
/// @param u16DmxId                       \b IN: TSO PID filter source
///
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
///
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSO_Pid_Close(MS_U8 u8Eng, MS_U16 u16DmxId)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s][%d](%x, %d) \n", __FUNCTION__, __LINE__, (unsigned int)u8Eng, u16DmxId));

#ifdef TSO_20_ENABLE
    _DMX_TSO_ENTRY();

    if(MDrv_TSO_PidFlt_Free(u8Eng, u16DmxId) != DRVTSO_OK)
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);

#else
    return DMX_FILTER_STATUS_ERROR;
#endif

}

//FIQ API
DMX_FILTER_STATUS _MApi_DMX_FQ_SetFltRushPass(MS_U32 u32DmxId, MS_U8 u8Enable)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));
#ifdef FQ_ENABLE
    _DMX_ENTRY();
    DMX_ASSERT2(DMX_MAX_FLTID > u32DmxId, DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%06d] Bad Demux id %d\n", __LINE__, (int)u32DmxId)));

    MDrv_TSP_PidFlt_SetFltRushPass(0, u32DmxId, u8Enable);
    _FLT_LIST_PID_SET(u32DmxId, u8Enable);

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Initialize Demux API for FQ Enable
/// @param param                \b IN: Init FQ Setting
/// @return                     DMX_FILTER_STATUS_ERROR - Failure
/// @note if want to use FQ, please call this API after calling MApi_DMX_Init. (FQ does not support for all chip)
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_FQ_Init(MS_U32 u32FQEng, DMX_FQ_Info* pFQInfo)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

#ifdef FQ_ENABLE
    _DMX_ENTRY();

    if(!pFQInfo)
    {
        ULOGE("DMX", "ERROR!! pFQInfo is NULL!!\n");
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if(pFQInfo->u32BufSize % 192)
    {
        ULOGE("DMX", "ERROR!! u32BufSize is not 192 aligned!!\n");
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

    if(MDrv_FQ_Init(u32FQEng, pFQInfo->u8AddrMode) != DRVFQ_OK)
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);

    MDrv_TSP_FQ_SetMuxSwitch(u32FQEng, _pdmx_res->_eFqSrc);
    MDrv_FQ_SetBuffer(u32FQEng, pFQInfo->u32BufStart, pFQInfo->u32BufSize);
    MDrv_FQ_Start(u32FQEng, TRUE);
    _DMX_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

DMX_FILTER_STATUS _MApi_DMX_FQ_Exit(MS_U32 u32FQEng)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));
#ifdef FQ_ENABLE
    _DMX_ENTRY();
    MDrv_FQ_Start(u32FQEng, FALSE);
    MDrv_FQ_Exit(u32FQEng);
    _DMX_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

DMX_FILTER_STATUS _MApi_DMX_FQ_RushEnable(MS_U32 u32FQEng)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));
#ifdef FQ_ENABLE
    _DMX_ENTRY();
    MDrv_FQ_RushEnable(u32FQEng);
    _DMX_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

DMX_FILTER_STATUS _MApi_DMX_FQ_SkipRushData(MS_U32 u32FQEng, DMX_FQ_SkipPath eSkipPath)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));
#ifdef FQ_ENABLE
    _DMX_ENTRY();

    MDrv_FQ_SkipRushData(u32FQEng, (MS_U32)eSkipPath);

    _DMX_RETURN(DMX_FILTER_STATUS_OK);
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

DMX_FILTER_STATUS _MApi_DMX_FQ_Configure(DMX_FQ_Cfg* pstFqCfg)
{
#ifdef FQ_ENABLE

    MS_U32 u32Ret = DMX_FILTER_STATUS_OK;
    DrvFQ_SrcIf eFQSrcif = E_DRVFQ_SRC_TSIF0;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    if(E_DMX_FQ_CFG_SEL_SRCMUX == pstFqCfg->eCfgCmd)
    {
        if(pstFqCfg->bSet)
        {
            switch(pstFqCfg->eFqSrcSel)
            {
                case DMX_TSIF_LIVE0:
                    eFQSrcif = E_DRVFQ_SRC_TSIF0;
                    break;
                case DMX_TSIF_LIVE1:
                    eFQSrcif = E_DRVFQ_SRC_TSIF1;
                    break;
                case DMX_TSIF_LIVE2:
                    eFQSrcif = E_DRVFQ_SRC_TSIF2;
                    break;
                case DMX_TSIF_LIVE3:
                    eFQSrcif = E_DRVFQ_SRC_TSIF3;
                    break;
                default:
                    _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
            }

            _pdmx_res->_eFqSrc = eFQSrcif;

            if(DRVTSP_OK != MDrv_TSP_FQ_SetMuxSwitch(pstFqCfg->u32FQEng,eFQSrcif))
                u32Ret = DMX_FILTER_STATUS_ERROR;
        }
        else
        {
            if(DRVTSP_OK != MDrv_TSP_FQ_GetMuxSwitch(pstFqCfg->u32FQEng,&eFQSrcif))
                u32Ret = DMX_FILTER_STATUS_ERROR;

            pstFqCfg->eFqSrcSel = 0xFF;
            switch(eFQSrcif)
            {
                case E_DRVFQ_SRC_TSIF0:
                    pstFqCfg->eFqSrcSel = DMX_TSIF_LIVE0;
                    break;
                case E_DRVFQ_SRC_TSIF1:
                    pstFqCfg->eFqSrcSel = DMX_TSIF_LIVE1;
                    break;
                case E_DRVFQ_SRC_TSIF2:
                    pstFqCfg->eFqSrcSel = DMX_TSIF_LIVE2;
                    break;
                case E_DRVFQ_SRC_TSIF3:
                    pstFqCfg->eFqSrcSel = DMX_TSIF_LIVE3;
                    break;
                default:
                    u32Ret = DMX_FILTER_STATUS_ERROR;
                    break;
            }
        }
    }
    else if(E_DMX_FQ_CFG_FLT_NULL_PKT == pstFqCfg->eCfgCmd)
    {
        if(DRVTSP_OK != MDrv_TSP_FQ_FltNullPkt(pstFqCfg->u32FQEng, pstFqCfg->bSet))
            u32Ret = DMX_FILTER_STATUS_NOT_SUPPORT;
    }
    else
    {
        u32Ret = DMX_FILTER_STATUS_NOT_SUPPORT;
    }

    _DMX_RETURN(u32Ret);

#else
    return DMX_FILTER_STATUS_NOT_SUPPORT;
#endif
}

DMX_FILTER_STATUS _MApi_DMX_FQ_SetRushAddr(MS_U32 u32FQEng, MS_PHY phyRushAddr)
{
#ifdef FQ_ENABLE

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    if(DRVFQ_OK == MDrv_FQ_SetRushAddr(u32FQEng, phyRushAddr))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

#else
    return DMX_FILTER_STATUS_NOT_SUPPORT;
#endif
}

DMX_FILTER_STATUS _MApi_DMX_FQ_ReadGet(MS_U32 u32FQEng,MS_PHY* pphyRead)
{
#ifdef FQ_ENABLE

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    if(DRVFQ_OK == MDrv_FQ_GetReadAddr(u32FQEng, pphyRead))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

#else
    return DMX_FILTER_STATUS_NOT_SUPPORT;
#endif
}

DMX_FILTER_STATUS _MApi_DMX_FQ_WriteGet(MS_U32 u32FQEng,MS_PHY* pphyWrite)
{
#ifdef FQ_ENABLE

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    _DMX_ENTRY();

    if(DRVFQ_OK == MDrv_FQ_GetWriteAddr(u32FQEng, pphyWrite))
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }

#else
    return DMX_FILTER_STATUS_NOT_SUPPORT;
#endif
}

DMX_FILTER_STATUS _MApi_DMX_Get_PipeId(DMX_PIPEID_GROUP eGroup, DMX_GENERAL_ENGID EngId, MS_U32 *pu32PipeId)
{
    DMX_DBGMSG(DMX_DBG_FUNC, printf("[%s] %d\n", __FUNCTION__, __LINE__));

    MS_U32 u32FileEng = 0;

    *pu32PipeId = DMX_PIPE_NULL;

    switch(eGroup)
    {
        case DMX_PIPEID_GRP_PVR:
            *pu32PipeId = DMX_PIPE_PVR_0 + (MS_U32)(EngId.PvrEng);
            break;
        case DMX_PIPEID_GRP_FILE:
            u32FileEng = (MS_U32)(EngId.FileinPath);
            *pu32PipeId = DMX_PIPE_FILE_0 + u32FileEng;
            break;
        case DMX_PIPEID_GRP_MMFI:
            *pu32PipeId = DMX_PIPE_MMFI_0 + (MS_U32)(EngId.MmfiPath);
            break;

        default:
            return DMX_FILTER_STATUS_ERROR;
    }

    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Hardware general config for driver interface
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_CMD_Run(MS_U32 u32Cmd, MS_U32 u32Config, MS_U32 u32DataNum, void *pData)
{
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    switch(u32Cmd)
    {
        case DMX_CMD_SET_LIB_MODE:
            _pdmx_res->_u32LibMode = u32Config;
            break;

        case DMX_CMD_SET_PESCPY_CONT_MODE:
            _u32PESCpy_ContMode = u32Config;
            return DMX_FILTER_STATUS_OK;

        case DMX_CMD_ENABLE_INIT_REF_CNT:
            _pdmx_res->_bEnableInitRefCnt = (MS_BOOL)u32Config;
            return DMX_FILTER_STATUS_OK;

        default:
            break;
    }

    if(MDrv_TSP_CMD_Run(u32Cmd, u32Config, u32DataNum, pData) != DRVTSP_OK)
        return DMX_FILTER_STATUS_ERROR;
    else
        return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
// Debug Table
//-------------------------------------------------------------------------------------------------

#ifdef DEBUG_TABLE_SUPPORT
  #if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0
static DrvTSP_Debug_Cmd _DMX_ApiDrv_DebugCmdMapping(DMX_DEBUG_CMD eCmd)
{
    switch (eCmd)
    {
        case DMX_DEBUG_CMD_NONE:
            return E_DRVTSP_DEBUG_CMD_NONE;
        case DMX_DEBUG_CMD_CLEAR:
            return E_DRVTSP_DEBUG_CMD_CLEAR;
        case DMX_DEBUG_CMD_ENABLE:
            return E_DRVTSP_DEBUG_CMD_ENABLE;
        case DMX_DEBUG_CMD_DISABLE:
            return E_DRVTSP_DEBUG_CMD_DISABLE;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[%s][%d] UnSupported Debug DMX_DEBUG_CMD !!\n",__FUNCTION__,__LINE__));
            return DMX_DEBUG_CMD_NONE;
    }
}

static DrvTSP_AVFIFO _DMX_ApiDrv_DebugFifoMapping(DMX_DEBUG_FIFO eFIFO)
{
    switch (eFIFO)
    {
        case DMX_DEBUG_FIFO_VIDEO:
            return E_DRVTSP_AVFIFO_VIDEO;
        case DMX_DEBUG_FIFO_VIDEO3D:
            return E_DRVTSP_AVFIFO_VIDEO3D;
        case DMX_DEBUG_FIFO_VIDEO3:
            return E_DRVTSP_AVFIFO_VIDEO3;
        case DMX_DEBUG_FIFO_VIDEO4:
            return E_DRVTSP_AVFIFO_VIDEO4;
        case DMX_DEBUG_FIFO_VIDEO5:
            return E_DRVTSP_AVFIFO_VIDEO5;
        case DMX_DEBUG_FIFO_VIDEO6:
            return E_DRVTSP_AVFIFO_VIDEO6;
        case DMX_DEBUG_FIFO_VIDEO7:
            return E_DRVTSP_AVFIFO_VIDEO7;
        case DMX_DEBUG_FIFO_VIDEO8:
            return E_DRVTSP_AVFIFO_VIDEO8;
        case DMX_DEBUG_FIFO_AUDIO:
            return E_DRVTSP_AVFIFO_AUDIO;
        case DMX_DEBUG_FIFO_AUDIOB:
            return E_DRVTSP_AVFIFO_AUDIOB;
        case DMX_DEBUG_FIFO_AUDIOC:
            return E_DRVTSP_AVFIFO_AUDIOC;
        case DMX_DEBUG_FIFO_AUDIOD:
            return E_DRVTSP_AVFIFO_AUDIOD;
        case DMX_DEBUG_FIFO_AUDIOE:
            return E_DRVTSP_AVFIFO_AUDIOE;
        case DMX_DEBUG_FIFO_AUDIOF:
            return E_DRVTSP_AVFIFO_AUDIOF;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[%s][%d] UnSupported Debug DMX_DEBUG_FIFO !!\n",__FUNCTION__,__LINE__));
            return E_DRVTSP_AVFIFO_INVALID;
    }
}
  #endif
#endif

//-------------------------------------------------------------------------------------------------
/// Get dis-continue count by ts source and specific FIFO.
/// @param pDmxInfo           \b IN:  set control parameter to get related tsif packet count.
/// @param pu32Cnt            \b OUT: dis-continue packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Get_DisContiCnt(DMX_DisContiCnt_info* pDmxInfo, MS_U32 *pu32Cnt)
{
#ifdef DEBUG_TABLE_SUPPORT

    DMX_FILTER_STATUS ret;
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

  #if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0)))

    DrvTSP_DisContiCnt_info tspInfo;

    _DMX_ENTRY();

    memset(&tspInfo, 0, sizeof(DrvTSP_DisContiCnt_info));
    tspInfo.TspCmd  = pDmxInfo->TspCmd;
    tspInfo.TspFifo = pDmxInfo->TspFifo;
    tspInfo.TspSrc  = pDmxInfo->TspSrc;

    ret = (DRVTSP_OK == MDrv_TSP_Get_DisContiCnt(&tspInfo, pu32Cnt)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);

    _DMX_RETURN(ret);

  #else  // For TSP_VER_4_0

    DrvTSP_Debug_Cmd eDbgCmd  = _DMX_ApiDrv_DebugCmdMapping(pDmxInfo->TspCmd);
    DrvTSP_AVFIFO    eDbgAV   = _DMX_ApiDrv_DebugFifoMapping(pDmxInfo->TspFifo);
    DrvTSP_Flow      eDbgFlow = E_DRVTSP_FLOW_INVALID;

    switch(pDmxInfo->TspSrc)
    {
     case DMX_DEBUG_SRC_TS0:
         eDbgFlow = E_DRVTSP_FLOW_LIVE0;
         break;
     case DMX_DEBUG_SRC_TS1:
         eDbgFlow = E_DRVTSP_FLOW_LIVE1;
         break;
     case DMX_DEBUG_SRC_TS2:
         eDbgFlow = E_DRVTSP_FLOW_LIVE2;
         break;
     case DMX_DEBUG_SRC_TS3:
         eDbgFlow = E_DRVTSP_FLOW_LIVE3;
         break;
     case DMX_DEBUG_SRC_FILE0:
         eDbgFlow = E_DRVTSP_FLOW_FILE0;
         break;
     case DMX_DEBUG_SRC_FILE1:
         eDbgFlow = E_DRVTSP_FLOW_FILE1;
         break;
     case DMX_DEBUG_SRC_FILE2:
         eDbgFlow = E_DRVTSP_FLOW_FILE2;
         break;
     case DMX_DEBUG_SRC_FILE3:
         eDbgFlow = E_DRVTSP_FLOW_FILE3;
         break;
     case DMX_DEBUG_SRC_TS4:
     case DMX_DEBUG_SRC_FILE4:
         eDbgFlow = E_DRVTSP_FLOW_LIVE4;
         break;
     case DMX_DEBUG_SRC_TS5:
     case DMX_DEBUG_SRC_FILE5:
         eDbgFlow = E_DRVTSP_FLOW_LIVE5;
         break;
     case DMX_DEBUG_SRC_TS6:
     case DMX_DEBUG_SRC_FILE6:
         eDbgFlow = E_DRVTSP_FLOW_LIVE6;
         break;
     case DMX_DEBUG_SRC_MMFI0:
         eDbgFlow = E_DRVTSP_FLOW_MMFI0;
         break;
     case DMX_DEBUG_SRC_MMFI1:
         eDbgFlow = E_DRVTSP_FLOW_MMFI1;
         break;
     default:
         DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[%s][%d] UnSupported Debug Flow !!\n",__FUNCTION__,__LINE__));
         return DMX_FILTER_STATUS_ERROR;
    }

     _DMX_ENTRY();

    ret = (DRVTSP_OK == MDrv_TSP_Get_DisContiCnt(eDbgCmd, eDbgAV, eDbgFlow, pu32Cnt)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
    _DMX_RETURN(ret);

  #endif  // End of TSP_VER_4_0

    return DMX_FILTER_STATUS_ERROR;
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
///Get drop packet count by ts source and specific FIFO.
/// @param pDmxInfo           \b IN:  set control parameter to get related source packet count.
/// @param pu32Cnt            \b OUT: drop packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Get_DropPktCnt(DMX_DropPktCnt_info* pDmxInfo, MS_U32 *pu32Cnt)
{
#ifdef DEBUG_TABLE_SUPPORT

    DMX_FILTER_STATUS ret;
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));


  #if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0)))

    DrvTSP_DropPktCnt_info tspInfo;

    _DMX_ENTRY();

    memset(&tspInfo, 0, sizeof(DrvTSP_DropPktCnt_info));
    tspInfo.TspCmd  = pDmxInfo->TspCmd;
    tspInfo.TspFifo = pDmxInfo->TspFifo;
    tspInfo.TspSrc  = pDmxInfo->TspSrc;

    ret = (DRVTSP_OK == MDrv_TSP_Get_DropPktCnt(&tspInfo, pu32Cnt)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
    _DMX_RETURN(ret);

  #else  // For TSP_VER_4_0

    DrvTSP_Debug_Cmd eDbgCmd  = _DMX_ApiDrv_DebugCmdMapping(pDmxInfo->TspCmd);
    DrvTSP_AVFIFO    eDbgAV   = _DMX_ApiDrv_DebugFifoMapping(pDmxInfo->TspFifo);
    DrvTSP_Flow      eDbgFlow = E_DRVTSP_FLOW_INVALID;

    switch(pDmxInfo->TspSrc)
    {
         case DMX_DEBUG_SRC_TS0:
             eDbgFlow = E_DRVTSP_FLOW_LIVE0;
             break;
         case DMX_DEBUG_SRC_TS1:
             eDbgFlow = E_DRVTSP_FLOW_LIVE1;
             break;
         case DMX_DEBUG_SRC_TS2:
             eDbgFlow = E_DRVTSP_FLOW_LIVE2;
             break;
         case DMX_DEBUG_SRC_TS3:
             eDbgFlow = E_DRVTSP_FLOW_LIVE3;
             break;
         case DMX_DEBUG_SRC_FILE0:
             eDbgFlow = E_DRVTSP_FLOW_FILE0;
             break;
         case DMX_DEBUG_SRC_FILE1:
             eDbgFlow = E_DRVTSP_FLOW_FILE1;
             break;
         case DMX_DEBUG_SRC_FILE2:
             eDbgFlow = E_DRVTSP_FLOW_FILE2;
             break;
         case DMX_DEBUG_SRC_FILE3:
             eDbgFlow = E_DRVTSP_FLOW_FILE3;
             break;
         case DMX_DEBUG_SRC_TS4:
         case DMX_DEBUG_SRC_FILE4:
             eDbgFlow = E_DRVTSP_FLOW_LIVE4;
             break;
         case DMX_DEBUG_SRC_TS5:
         case DMX_DEBUG_SRC_FILE5:
             eDbgFlow = E_DRVTSP_FLOW_LIVE5;
             break;
         case DMX_DEBUG_SRC_TS6:
         case DMX_DEBUG_SRC_FILE6:
             eDbgFlow = E_DRVTSP_FLOW_LIVE6;
             break;
         case DMX_DEBUG_SRC_MMFI0:
             eDbgFlow = E_DRVTSP_FLOW_MMFI0;
             break;
         case DMX_DEBUG_SRC_MMFI1:
             eDbgFlow = E_DRVTSP_FLOW_MMFI1;
             break;
         default:
             DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX","[%s][%d] UnSupported Debug Flow !!\n",__FUNCTION__,__LINE__));
             return DMX_FILTER_STATUS_ERROR;
             break;
    }

    _DMX_ENTRY();
    ret = (DRVTSP_OK == MDrv_TSP_Get_DropPktCnt(eDbgCmd, eDbgAV, eDbgFlow, pu32Cnt)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
    _DMX_RETURN(ret);

  #endif  // End of TSP_VER_4_0

    return DMX_FILTER_STATUS_ERROR;
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
///Get locked packet count by ts source and specific FIFO.
/// @param pDmxInfo           \b IN:  set control parameter to get related tsif packet count.
/// @param pu32Cnt            \b OUT: Lock packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Get_LockPktCnt(DMX_LockPktCnt_info* pDmxInfo, MS_U32 *pu32Cnt)
{
#ifdef DEBUG_TABLE_SUPPORT

    DMX_FILTER_STATUS ret;
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

  #if (defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0)))

    DrvTSP_LockPktCnt_info tspInfo;

    _DMX_ENTRY();

    memset(&tspInfo, 0, sizeof(DrvTSP_LockPktCnt_info));
    tspInfo.TspCmd  = pDmxInfo->TspCmd;
    tspInfo.TspTsif = pDmxInfo->TspTsif;

    ret = (DRVTSP_OK == MDrv_TSP_Get_LockPktCnt(&tspInfo, pu32Cnt)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
    _DMX_RETURN(ret);

  #else  // For TSP_VER_4_0

    DrvTSP_Debug_Cmd eDbgCmd  = _DMX_ApiDrv_DebugCmdMapping(pDmxInfo->TspCmd);
    DrvTSP_Flow      eDbgFlow = E_DRVTSP_FLOW_INVALID;

    switch(pDmxInfo->TspTsif)
    {
        case DMX_DEBUG_TSIF_TS0:
            eDbgFlow = E_DRVTSP_FLOW_LIVE0;
            break;
        case DMX_DEBUG_TSIF_TS1:
            eDbgFlow = E_DRVTSP_FLOW_LIVE1;
            break;
        case DMX_DEBUG_TSIF_TS2:
            eDbgFlow = E_DRVTSP_FLOW_LIVE2;
            break;
        case DMX_DEBUG_TSIF_TS3:
            eDbgFlow = E_DRVTSP_FLOW_LIVE3;
            break;
        case DMX_DEBUG_TSIF_TS4:
            eDbgFlow = E_DRVTSP_FLOW_LIVE4;
            break;
        case DMX_DEBUG_TSIF_TS5:
            eDbgFlow = E_DRVTSP_FLOW_LIVE5;
            break;
        case DMX_DEBUG_TSIF_TS6:
            eDbgFlow = E_DRVTSP_FLOW_LIVE6;
            break;
        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX","[%s][%d] UnSupported Debug Flow !!\n",__FUNCTION__,__LINE__));
            return DMX_FILTER_STATUS_ERROR;
    }

    _DMX_ENTRY();

    ret = (DRVTSP_OK == MDrv_TSP_Get_LockPktCnt(eDbgCmd, eDbgFlow, pu32Cnt)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);

    _DMX_RETURN(ret);

  #endif  // End of TSP_VER_4_0

    return DMX_FILTER_STATUS_ERROR;
#else
    return DMX_FILTER_STATUS_ERROR;
#endif

}

//-------------------------------------------------------------------------------------------------
/// Get AV packet count by ts source and specific FIFO.
/// @param pDmxInfo           \b IN:  set control parameter to get related source packet count.
/// @param pu32Cnt            \b OUT: AV packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Get_AVPktCnt(DMX_AVPktCnt_info *DmxInfo, MS_U32 *pu32Cnt)
{
#ifdef DEBUG_TABLE_SUPPORT

    DMX_FILTER_STATUS ret;
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

  #if (defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0)))

    DrvTSP_AVPktCnt_info tspInfo;

    _DMX_ENTRY();

    memset(&tspInfo, 0, sizeof(DrvTSP_AVPktCnt_info));
    tspInfo.TspCmd     = DmxInfo->TspCmd;
    tspInfo.TspFifo    = DmxInfo->TspFifo;
    tspInfo.TspFifoSrc = DmxInfo->TspFifoSrc; // for new chip(after Nasa), old chip is not used.

    ret = (DRVTSP_OK == MDrv_TSP_Get_AVPktCnt(&tspInfo, pu32Cnt)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);
    _DMX_RETURN(ret);

  #else  // For TSP_VER_4_0

    DrvTSP_Debug_Cmd eDbgCmd  = _DMX_ApiDrv_DebugCmdMapping(DmxInfo->TspCmd);
    DrvTSP_AVFIFO    eDbgAV   = _DMX_ApiDrv_DebugFifoMapping(DmxInfo->TspFifo);
    DrvTSP_Flow      eDbgFlow = E_DRVTSP_FLOW_INVALID;

    switch(DmxInfo->TspFifoSrc)
    {
         case DMX_DEBUG_PKT_DEMUX_0:
             eDbgFlow = E_DRVTSP_FLOW_LIVE0;
             break;
         case DMX_DEBUG_PKT_DEMUX_1:
             eDbgFlow = E_DRVTSP_FLOW_LIVE1;
             break;
         case DMX_DEBUG_PKT_DEMUX_2:
             eDbgFlow = E_DRVTSP_FLOW_LIVE2;
             break;
         case DMX_DEBUG_PKT_DEMUX_3:
             eDbgFlow = E_DRVTSP_FLOW_LIVE3;
             break;
         case DMX_DEBUG_PKT_DEMUX_0_FILE:
             eDbgFlow = E_DRVTSP_FLOW_FILE0;
             break;
         case DMX_DEBUG_PKT_DEMUX_1_FILE:
             eDbgFlow = E_DRVTSP_FLOW_FILE1;
             break;
         case DMX_DEBUG_PKT_DEMUX_2_FILE:
             eDbgFlow = E_DRVTSP_FLOW_FILE2;
             break;
         case DMX_DEBUG_PKT_DEMUX_3_FILE:
             eDbgFlow = E_DRVTSP_FLOW_FILE3;
             break;
         case DMX_DEBUG_PKT_DEMUX_4:
         case DMX_DEBUG_PKT_DEMUX_4_FILE:
             eDbgFlow = E_DRVTSP_FLOW_LIVE4;
             break;
         case DMX_DEBUG_PKT_DEMUX_5:
         case DMX_DEBUG_PKT_DEMUX_5_FILE:
             eDbgFlow = E_DRVTSP_FLOW_LIVE5;
             break;
         case DMX_DEBUG_PKT_DEMUX_6:
         case DMX_DEBUG_PKT_DEMUX_6_FILE:
             eDbgFlow = E_DRVTSP_FLOW_LIVE6;
             break;
         case DMX_DEBUG_MMFI0:
             eDbgFlow = E_DRVTSP_FLOW_MMFI0;
             break;
         case DMX_DEBUG_MMFI1:
             eDbgFlow = E_DRVTSP_FLOW_MMFI1;
             break;
         default:
             DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX","[%s][%d] UnSupported Debug Flow !!\n",__FUNCTION__,__LINE__));
             return DMX_FILTER_STATUS_ERROR;
    }

    _DMX_ENTRY();

    ret = (DRVTSP_OK == MDrv_TSP_Get_AVPktCnt(eDbgCmd, eDbgAV, eDbgFlow, pu32Cnt)? DMX_FILTER_STATUS_OK: DMX_FILTER_STATUS_ERROR);

    _DMX_RETURN(ret);

  #endif  // End of TSP_VER_4_0

    return DMX_FILTER_STATUS_ERROR;
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
///Get section TEI packet count by ts source
/// @param FltSrc                     \b IN: TS source
/// @param pu32PktCnt            \b OUT: TEI packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Get_SecTEI_PktCount(DMX_FILTER_TYPE FltSrc, MS_U32* pu32PktCnt)
{
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    _DMX_ENTRY();

    if(MDrv_TSP_Get_SecTEI_PktCount(FltSrc, pu32PktCnt) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
///Reset section TEI packet count by DMX filter id
/// @param FltSrc                       \b IN: TS source
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Reset_SecTEI_PktCount(DMX_FILTER_TYPE FltSrc)
{
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    _DMX_ENTRY();

    if(MDrv_TSP_Reset_SecTEI_PktCount(FltSrc) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
///Get section dis-continue packet count
/// @param u32DmxID                     \b IN: DMX filter Id
/// @param pu32PktCnt                    \b OUT: Dis-continue packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Get_SecDisCont_PktCount(MS_U32 u32DmxID, MS_U32* pu32PktCnt)
{
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    _DMX_ENTRY();

    if(MDrv_TSP_Get_SecDisCont_PktCount(u32DmxID, pu32PktCnt) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
///Reset section dis-continue packet count
/// @param u32DmxID                     \b IN: DMX filter Id
/// @param pu32PktCnt                    \b OUT: Dis-continue packet count
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_Reset_SecDisCont_PktCount(MS_U32 u32DmxID)
{
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    _DMX_ENTRY();

    if(MDrv_TSP_Reset_SecDisCont_PktCount(u32DmxID) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else
    return DMX_FILTER_STATUS_ERROR;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Initialize lib resource API
/// @param pu32ResMemAddr                \b IN: Pointer to store resource memory addresses
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS MApi_DMX_InitLibResource(void *pResMemAddr)
{
    MS_U32     u32size = 0;
    MS_VIRT    ptrMemPos =  (MS_VIRT)pResMemAddr;

    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s][%d](%p)\n", __FUNCTION__, __LINE__, pResMemAddr));

    DMX_ASSERT((pResMemAddr != 0), DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[MAPI DMX][%s][%06d] data pointer is 0x%p!", __FUNCTION__, __LINE__, pResMemAddr)));

    _pdmx_res = (DMX_RESOURCE_PRIVATE*)pResMemAddr;

    if(_pdmx_res->bResoureInit != TRUE)
    {
        _DMX_InitResource(_pdmx_res);
    }

    ptrMemPos += sizeof(DMX_RESOURCE_PRIVATE);

    if(MDrv_TSP_InitLibResource((void*)ptrMemPos) != DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

#if defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0) || (TSP_VERSION == TSP_VER_3_0))
    if(MDrv_TSP_GetCap(E_DRVTSP_CAP_RESOURCE_SIZE, (void*)(&u32size)) != DRVTSP_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#else
    if(MDrv_TSP_GetCap(E_TSP_CAP_RESOURCE_SIZE, (void*)(&u32size)) != E_TSP_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }
#endif

    ptrMemPos += u32size;

#if defined(MMFILEIN) || defined(MMFI_VD3D)
    if(MDrv_MMFI_InitLibResource((void*)ptrMemPos) != DRVMMFI_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    if(MDrv_MMFI_GetCap(E_DRVMMFI_CAP_RESOURCE_SIZE, (void*)(&u32size)) != DRVMMFI_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    ptrMemPos += u32size;
#endif

#ifdef TSIO_ENABLE
    if(MDrv_TSIO_InitLibResource((void*)ptrMemPos) != DRVTSIO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    if(MDrv_TSIO_GetCap(DRVTSIO_CAP_RESOURCE_SIZE, (void*)(&u32size)) != DRVTSIO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }
    ptrMemPos += u32size;
#endif

#ifdef TSO_ENABLE
    if(MDrv_TSO_InitLibResource((void*)ptrMemPos) != DRVTSO_OK)
        return DMX_FILTER_STATUS_ERROR;
#endif

    _pdmx_res->bResoureInit = TRUE;

    return DMX_FILTER_STATUS_OK;
}

#ifdef MERGE_STR_SUPPORT
#if(!(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0))))  // For TSP_VER_4_0

static DrvTSP_If _DMX_ApiDrv_TsIfMapping(DMX_TSIF eTsIf)
{
    DrvTSP_If Out_TSP_If = 0;

    switch(eTsIf)
    {
        case DMX_TSIF_LIVE0:
        case DMX_TSIF_FILE0:
            Out_TSP_If = E_DRVTSP_IF_TSIF0;
            break;
        case DMX_TSIF_LIVE1:
        case DMX_TSIF_FILE1:
            Out_TSP_If = E_DRVTSP_IF_TSIF1;
            break;
        case DMX_TSIF_LIVE2:
        case DMX_TSIF_FILE2:
            Out_TSP_If = E_DRVTSP_IF_TSIF2;
            break;
        case DMX_TSIF_LIVE3:
        case DMX_TSIF_FILE3:
            Out_TSP_If = E_DRVTSP_IF_TSIF3;
            break;
        case DMX_TSIF_LIVE4:
        case DMX_TSIF_FILE4:
            Out_TSP_If = E_DRVTSP_IF_TSIF4;
            break;
        case DMX_TSIF_LIVE5:
        case DMX_TSIF_FILE5:
            Out_TSP_If = E_DRVTSP_IF_TSIF5;
            break;
        case DMX_TSIF_LIVE6:
        case DMX_TSIF_FILE6:
            Out_TSP_If = E_DRVTSP_IF_TSIF6;
            break;

        default:
            DMX_DBGMSG(DMX_DBG_ERR, ULOGE("DMX", "[%s][%d] TsIf Not Support !!\n",__FUNCTION__,__LINE__));
            break;
    }

    return Out_TSP_If;
}
#endif  // End of TSP_VER_4_0
#endif  // End of MERGE_STR_SUPPORT

//Merger stream API
//-------------------------------------------------------------------------------------------------
///Set or Get merge stream source id and sync byte
/// @param eIf                                 \b IN:  Eunm value of DMX TSIF selection
/// @param u8StrId                           \b IN:  Stream index
/// @param pu8SyncByte                   \b IN:  Pointer to sync bytearray of merege streams
/// @param bSet                              \b IN:  TRUE to setting data or FALSE to getting table
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note: Currently, maxmum number is 8, and don't call this API when stream processing is started
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_MStr_SyncByte(DMX_TSIF eIf,  MS_U8 u8StrId, MS_U8* pu8SyncByte, MS_BOOL bSet)
{
#ifdef MERGE_STR_SUPPORT
  #if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))
    DrvTSP_If eTspIf = E_DRVTSP_IF_TS0;

    switch(eIf)
    {
        case DMX_TSIF_LIVE0:
            eTspIf = E_DRVTSP_IF_TS0;
            break;
        case DMX_TSIF_LIVE1:
        case DMX_TSIF_FILE1:
            eTspIf = E_DRVTSP_IF_TS1;
            break;
        case DMX_TSIF_LIVE2:
        case DMX_TSIF_FILE2:
            eTspIf = E_DRVTSP_IF_TS2;
            break;
        case DMX_TSIF_FILE0:
            eTspIf = E_DRVTSP_IF_FI;
            break;
        default:
            return DMX_FILTER_STATUS_ERROR;
    }
  #else // For TSP_VER_4_0
    DrvTSP_If eTspIf = _DMX_ApiDrv_TsIfMapping(eIf);
  #endif

    _DMX_ENTRY();
    if(MDrv_TSP_MStr_SyncByte(eTspIf, u8StrId, pu8SyncByte, bSet) == DRVTSP_OK)
    {
        _DMX_RETURN(DMX_FILTER_STATUS_OK);
    }
    else
    {
        _DMX_RETURN(DMX_FILTER_STATUS_ERROR);
    }
#else
    return DMX_FILTER_STATUS_NOT_SUPPORT;
#endif
}

//-------------------------------------------------------------------------------------------------
/// STD general function
/// @param ePowerState              \b IN: TSP power state
/// @param u32FWAddr                  \b IN: FW address
/// @param u32FWSize                  \b IN: FW size
/// @return DMX_FILTER_STATUS_OK - Success
/// @return DMX_FILTER_STATUS_ERROR - Failure
/// @note
//-------------------------------------------------------------------------------------------------
MS_U32 _MApi_DMX_SetPowerState(EN_POWER_MODE ePowerState, MS_PHY phyFWAddr, MS_U32 u32FWSize)
{
#if(defined(TSP_VERSION) && ((TSP_VERSION == TSP_VER_1_0)||(TSP_VERSION == TSP_VER_3_0)))

    DMX_FILTER_STATUS _ret = DMX_FILTER_STATUS_ERROR;
    DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d\n", __FUNCTION__, __LINE__));

    switch (ePowerState)
    {
        case E_POWER_SUSPEND:
            _ret = _MApi_DMX_Suspend();
            break;

        case E_POWER_RESUME:
            _ret = _MApi_DMX_Resume(phyFWAddr, u32FWSize);
            break;

        case E_POWER_MECHANICAL:
        case E_POWER_SOFT_OFF:
        default:
            DMX_DBGMSG(DMX_DBG_FUNC, ULOGD("DMX", "[%s] %d Power state not support!!\n", __FUNCTION__, __LINE__));
            break;
    }

    return ((DMX_FILTER_STATUS_OK == _ret)? UTOPIA_STATUS_SUCCESS: UTOPIA_STATUS_FAIL);
#else
    return 0;
#endif
}

DMX_FILTER_STATUS _MApi_DMX_Wait_TspIoSignal(DMX_TSP_IOSIGNAL *pDmxTspIoSig)
{
    TSP_Result Res;

    pDmxTspIoSig->u32Eng    = 0;
    pDmxTspIoSig->u32TspEvt = 0;

    //first time to create process event id
    if(pDmxTspIoSig->s32KerModeTspEvtId == -1)
    {
        pDmxTspIoSig->u32DmxEvt = 0;
        if(MDrv_TSP_Create_IoSignal((DrvTSP_IoSignal*)pDmxTspIoSig) != DRVTSP_OK)
            return DMX_FILTER_STATUS_ERROR;
        else
            return DMX_FILTER_STATUS_OK;
    }

    //When user mode callback thread end, delete event
    if(pDmxTspIoSig->u32EvtWaitOpt == 0xFFFFFFFF)
    {
        pDmxTspIoSig->u32DmxEvt = 0;
        if(MDrv_TSP_Close_IoSignal((DrvTSP_IoSignal*)pDmxTspIoSig) != DRVTSP_OK)
            return DMX_FILTER_STATUS_ERROR;
        else
            return DMX_FILTER_STATUS_OK;
    }

    if((Res = MDrv_TSP_Wait_IoSignal((DrvTSP_IoSignal*)pDmxTspIoSig)) != DRVTSP_OK)
    {
        pDmxTspIoSig->u32DmxEvt = 0;

        if(Res == DRVTSP_FUNC_ERROR)
            return DMX_FILTER_STATUS_ERROR;
        else
            return DMX_FILTER_STATUS_OK;
    }

    MDrv_TSP_Proc_IoSignal((DrvTSP_IoSignal*)pDmxTspIoSig);

    pDmxTspIoSig->u32DmxEvt = 0;
    if(pDmxTspIoSig->u32PvrBufFullFlag[0] == 0xFF)
    {
        pDmxTspIoSig->u32DmxEvt |= DMX_EVENT_PVR1BUF_FULL;
    }
    else if(pDmxTspIoSig->u32PvrBufFullFlag[0] > 0)
    {
        pDmxTspIoSig->u32DmxEvt |= DMX_EVENT_PVRBUF_FULL;
    }

    if(pDmxTspIoSig->u32TspEvt & TSP_TASK_EVENT_SECTION_SELF)
    {
        pDmxTspIoSig->u32DmxEvt |= (DMX_EVENT_DATA_READY|DMX_EVENT_SEC_CRCERROR|DMX_EVENT_BUF_OVERFLOW);
    }

    pDmxTspIoSig->u32TspEvt &= ~TSP_TASK_EVENT_FLT_FREE;

    return DMX_FILTER_STATUS_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set DMAOUT VC to TSIO service
/// @param u16Handle                                    \b IN: Service handle ID
/// @param eSet                                            \b IN: Sgdma out param setting
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_SetDMAoutVC(MS_U16 u16Handle, TSIO_DMAOUT_SET *eSet)
{
#ifdef TSIO_ENABLE

    if (MDrv_TSIO_Service_SetDMAoutVC(u16Handle, (DRVTSIO_DMAOUT_SET*)eSet)!= DRVTSIO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    return DMX_FILTER_STATUS_OK;

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Get DMAOUT VC  write address
/// @param u16Handle                                    \b IN: Service handle ID
/// @param pphyWrite                                    \b OUT: dma out write address
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_DMAout_WriteGet(MS_U16 u16Handle, MS_PHY* pphyWrite)
{
#ifdef TSIO_ENABLE

    if (MDrv_TSIO_Service_DMAout_WriteGet(u16Handle, pphyWrite)!= DRVTSIO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    return DMX_FILTER_STATUS_OK;

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Set destination to TSIO service
/// @param u16Handle                                    \b IN: Service handle ID
/// @param eSet                                             \b IN: Sgdma out param setting
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_SetDestination(MS_U16 u16Handle, TSIO_SERVICEDEST eDest)
{
#ifdef TSIO_ENABLE

    DRVTSIO_SERVICEDEST edrvDest;
    switch (eDest)
    {
        case E_TSIO_SERVICEDEST_LIVE:
            edrvDest = E_DRVTSIO_SERVICEDEST_LIVE;
            break;

        case E_TSIO_SERVICEDEST_DMA:
            edrvDest = E_DRVTSIO_SERVICEDEST_DMA;
            break;

        default:
            return DMX_FILTER_STATUS_ERROR;
    }

    if (MDrv_TSIO_Service_SetDestination(u16Handle, edrvDest)!= DRVTSIO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    return DMX_FILTER_STATUS_OK;

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Set DMAIN Virture channel to TSIO service
/// @param u16Handle                                    \b IN: Service handle ID
/// @param eSet                                             \b IN: Sgdma in param setting
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_SetDMAinVC(MS_U16 u16Handle, TSIO_DMAIN_SET *eSet)
{
#ifdef TSIO_ENABLE

    if (MDrv_TSIO_Service_SetDMAinVC(u16Handle, (DRVTSIO_DMAIN_SET*)eSet)!= DRVTSIO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    return DMX_FILTER_STATUS_OK;

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// start DMAIN Virture channel
/// @param u16Handle                                    \b IN: Service handle ID
/// @param u32Pacing                                     \b IN: Sgdma in pacing rate
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_DMAinVC_Start(MS_U16 u16Handle, MS_U32 u32Pacing)
{
#ifdef TSIO_ENABLE

    if (MDrv_TSIO_Service_DMAinVC_Start(u16Handle, u32Pacing)!= DRVTSIO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    return DMX_FILTER_STATUS_OK;

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Set Locdecrypt key to TSIO service
/// @param u16Handle                                    \b IN: Service handle ID
/// @param eSet                                            \b IN: Pointer to store loc decrypt key address
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_SetLocdecKey(MS_U16 u16Handle, TSIO_LOCKEY_SET *eSet)
{
#ifdef TSIO_ENABLE

    if (eSet->bEvenkeyenable &&
        MDrv_TSIO_Service_SetLocdecKey(u16Handle, eSet->u8Evenkey, eSet->u16Evenkeylen, FALSE)!= DRVTSIO_OK)
    {
            return DMX_FILTER_STATUS_ERROR;
    }

    if (eSet->bOddkeyenable &&
        MDrv_TSIO_Service_SetLocdecKey(u16Handle, eSet->u8Oddkey, eSet->u16Oddkeylen, TRUE)!= DRVTSIO_OK)
    {
            return DMX_FILTER_STATUS_ERROR;
    }

    return DMX_FILTER_STATUS_OK;

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Enable/disable locdec key
/// @param u16Handle                                    \b IN: Service handle ID
/// @param bEnable                                        \b IN: Enable locdec key
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_LocdecKeyEnable(MS_U16 u16Handle, MS_BOOL bEnable)
{
#ifdef TSIO_ENABLE

    if (MDrv_TSIO_Service_LocdecKeyEnable(u16Handle, bEnable)!= DRVTSIO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    return DMX_FILTER_STATUS_OK;

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// TSIO input pid filter open.
/// @param u16Handle                                    \b IN:  Service handle ID
/// @param eTSOInSrc                                    \b IN: TSIO PID filter source
/// @param u16Pid                                          \b IN: PID value of TSO PID filter
/// @param pu16DmxId                                    \b OUT: Pointer to store PID filter ID
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_PidOpen(MS_U16 u16Handle, DMX_TSIF eTSOInSrc, MS_U16 u16Pid, MS_U16* pu16DmxId)
{
#ifdef TSIO_ENABLE

    DrvTSO_If eTsoif = E_DRVTSO_IF_LIVE0;
    MS_U8 u8Sid = 0xFF;
    switch(eTSOInSrc)
    {
        case DMX_TSIF_LIVE0:
            eTsoif = E_DRVTSO_IF_LIVE0;
            break;
        case DMX_TSIF_LIVE1:
            eTsoif = E_DRVTSO_IF_LIVE1;
            break;
        case DMX_TSIF_LIVE2:
            eTsoif = E_DRVTSO_IF_LIVE2;
            break;
        case DMX_TSIF_LIVE3:
            eTsoif = E_DRVTSO_IF_LIVE3;
            break;
        case DMX_TSIF_FILE0:
            eTsoif = E_DRVTSO_IF_FILE0;
            break;
        case DMX_TSIF_FILE1:
            eTsoif = E_DRVTSO_IF_FILE1;
            break;
        default:
            return DMX_FILTER_STATUS_ERROR;
    }

    _DMX_TSO_ENTRY();

    if (MDrv_TSIO_Service_GetSid(u16Handle, &u8Sid)!= DRVTSIO_OK)
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    if(MDrv_TSO_PidFlt_Alloc(0, pu16DmxId) != DRVTSO_OK)
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    if(MDrv_TSO_PidFlt_SetPid(0, *pu16DmxId, u16Pid) != DRVTSO_OK)
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    if(MDrv_TSO_PidFlt_SetChSrc(0, *pu16DmxId, eTsoif) != DRVTSO_OK)
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    if(MDrv_TSO_PidFlt_SetSid(0, *pu16DmxId, u8Sid) != DRVTSO_OK)
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// TSIO input pid filter close.
/// @param u16Handle                                    \b IN:  Service handle ID
/// @param u16DmxId                                     \b IN: TSO PID filter source
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_PidClose(MS_U16 u16Handle, MS_U16 u16DmxId)
{
#ifdef TSIO_ENABLE

    _DMX_TSO_ENTRY();
    if(MDrv_TSO_PidFlt_Free(0, u16DmxId) != DRVTSO_OK)
    {
        _DMX_TSO_RETURN(DMX_FILTER_STATUS_ERROR);
    }
    _DMX_TSO_RETURN(DMX_FILTER_STATUS_OK);

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Allocate TSIO service
/// @param u8Sid                                            \b IN: Service ID
/// @param u16Handle                                    \b OUT: pointer of service handle for return
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_Alloc(MS_U8 u8Sid, MS_U16 *u16Handle)
{
#ifdef TSIO_ENABLE

    if (MDrv_TSIO_Service_Alloc(u16Handle) != DRVTSIO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    if (MDrv_TSIO_Service_SetSid(*u16Handle, u8Sid) != DRVTSIO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    return DMX_FILTER_STATUS_OK;

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Free TSIO service
/// @param u16Handle                                    \b IN: Service handle ID
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSIO_Service_Free(MS_U16 u16Handle)
{
#ifdef TSIO_ENABLE

    if (MDrv_TSIO_Service_Free(u16Handle) != DRVTSIO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    return DMX_FILTER_STATUS_OK;

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Register TSIO callback function
/// @param TSIOIntTypeCB                                  \b IN: Callback function pointer
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSIO_RegisterIntCb(TSIOIntTypeCB efn)
{
#ifdef TSIO_ENABLE

        if (MDrv_TSIO_RegisterIntCb((TSIOIntCB)efn) != DRVTSIO_OK)
        {
            return DMX_FILTER_STATUS_ERROR;
        }

        return DMX_FILTER_STATUS_OK;

#else

        return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Init TSIO
/// @param eParm                         \b IN: init param
/// @return DMX_FILTER_STATUS
/// @note
/// It should be called before calling any TSIO API.
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSIO_Init(TSIO_Init_Set *eParm)
{
#ifdef TSIO_ENABLE

    if (MDrv_TSIO_Init() != DRVTSIO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    if (MDrv_TSIO_SGDMAIN_LastPktBuf_Set(eParm->phyLastpktBufAddr, eParm->u32LastpktBufSize) != DRVTSIO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    if (MDrv_TSIO_SGDMAIN_SGVQBuf_Set(eParm->phyDMAVQBufAddr, eParm->u32DMAVQBufSize) != DRVTSIO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    if (MDrv_TSIO_SGDMAIN_NodeBuf_Set(eParm->phyDMAinNodeAddr, eParm->u32DMAinNodeSize) != DRVTSIO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    if (MDrv_TSIO_SGDMAOUT_NodeBuf_Set(eParm->phyDMAoutNodeAddr, eParm->u32DMAoutNodeSize) != DRVTSIO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    return DMX_FILTER_STATUS_OK;

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Open TSIO
/// @param eClk                         \b IN: smc clk speed
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSIO_Open(TSIO_CLK eClk)
{
#ifdef TSIO_ENABLE

    DRVTSIO_CLK drvclk = DRVTSIO_CLK_200M;

    switch (eClk)
    {
        default:
        case TSIO_CLK_200M:
            drvclk = DRVTSIO_CLK_200M;
            break;

        case TSIO_CLK_100M:
            drvclk = DRVTSIO_CLK_100M;
            break;

        case TSIO_CLK_50M:
            drvclk = DRVTSIO_CLK_50M;
            break;

        case TSIO_CLK_25M:
            drvclk = DRVTSIO_CLK_25M;
            break;
    }

    if (MDrv_TSIO_Open(drvclk) != DRVTSIO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    return DMX_FILTER_STATUS_OK;

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Send ctrl command to TSIO
/// @param eCmd                                     \b IN: Command
/// @param u32Value                                \b IN: Commnad's param
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSIO_Cmd(TSIO_CMD eCmd, MS_U32 u32Value)
{
#ifdef TSIO_ENABLE

    switch (eCmd)
    {
        case TSIO_CMD_BYPASSSMC:
            if (MDrv_TSIO_BypassSMC_Set((MS_U8)u32Value) != DRVTSIO_OK)
            {
                return DMX_FILTER_STATUS_ERROR;
            }
            break;

        case TSIO_CMD_STUFFINGSID:
            if (MDrv_TSIO_StuffingSID_Set(u32Value) != DRVTSIO_OK)
            {
                return DMX_FILTER_STATUS_ERROR;
            }
            break;

        case TSIO_CMD_LOCDECRYPT:
            if (MDrv_TSIO_LocDecrypt_Set((MS_BOOL)u32Value) != DRVTSIO_OK)
            {
                return DMX_FILTER_STATUS_ERROR;
            }
            break;

        case TSIO_CMD_SYNCTHRESHOLD:
            if (MDrv_TSIO_Syncthreshold_Set((MS_U8)u32Value) != DRVTSIO_OK)
            {
                return DMX_FILTER_STATUS_ERROR;
            }
            break;

        case TSIO_CMD_DBGLEVEL:
            if (MDrv_TSIO_SetDbgLevel((DRVTSIO_DBGLEVEL)u32Value) != DRVTSIO_OK)
            {
                return DMX_FILTER_STATUS_ERROR;
            }
            break;


        case TSIO_CMD_PHASE_ADJUST:
            if (MDrv_TSIO_Analogphase_adj() != DRVTSIO_OK)
            {
                return DMX_FILTER_STATUS_ERROR;
            }
            break;

        case TSIO_CMD_BITTRAINING_ADJUST:
            if (MDrv_TSIO_Analogphase_adj_bittraining((MS_U8)u32Value) != DRVTSIO_OK)
            {
                return DMX_FILTER_STATUS_ERROR;
            }
            break;

        default:
            return DMX_FILTER_STATUS_NOT_SUPPORT;

    }

    return DMX_FILTER_STATUS_OK;

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Exit TSIO
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSIO_Exit(void)
{
#ifdef TSIO_ENABLE

    if (MDrv_TSIO_Exit() != DRVTSIO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    return DMX_FILTER_STATUS_OK;

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Close TSIO
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSIO_Close(void)
{
#ifdef TSIO_ENABLE

    if (MDrv_TSIO_Close() != DRVTSIO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    return DMX_FILTER_STATUS_OK;

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Send and Recv C&C haed-based cmd
/// @param Parm                                        \b IN: Pointer to store C&C cmd address
/// @param pResult                                     \b IN: Pointer to store result status
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSIO_CC(TSIO_CC_Set *Parm, TSIO_CC_RESULT* pResult)
{
#ifdef TSIO_ENABLE

    MS_U16 u16RxErrStatus = 0;

    if (pResult == NULL)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    if (Parm == NULL)
    {
        *pResult = TSIO_CC_RESULT_NORESULT;
        return DMX_FILTER_STATUS_ERROR;
    }

    if (Parm->pIn == NULL || Parm->pOut == NULL || Parm->pOutLen == NULL)
    {
        *pResult = TSIO_CC_RESULT_NORESULT;
        return DMX_FILTER_STATUS_ERROR;
    }

    //check input format
    if (Parm->u16InLen < TSIO_CC_CMD_MIN_LEN)
    {
        *pResult = TSIO_CC_RESULT_TXCMD_TOOSMALL;
        return DMX_FILTER_STATUS_ERROR;
    }

    if (Parm->u16InLen > TSIO_CC_CMD_MAX_LEN)
    {
        *pResult = TSIO_CC_RESULT_TXCMD_TOOBIG;
        return DMX_FILTER_STATUS_ERROR;
    }

    *pResult = TSIO_CC_RESULT_OK;


    //Send CC CMD
    if (MDrv_TSIO_CC_SendCmd(Parm->pIn, Parm->u16InLen) != DRVTSIO_OK)
    {
        *pResult = TSIO_CC_RESULT_TXCMD_TIMEOUT;
        goto FAIL;
    }

    //Receive CC CMD
    if (MDrv_TSIO_CC_ReceiveIntStatus(Parm->u32Timeout, &u16RxErrStatus) != DRVTSIO_OK)
    {
        *pResult = TSIO_CC_RESULT_RXCMD_TIMEOUT;
        goto FAIL;
    }

    if (u16RxErrStatus & TSIO_CC_CMD_ERR_STATUS_MASK)
    {
        *pResult = TSIO_CC_RESULT_ERR_FLAG;
        goto FAIL;
    }

    if (MDrv_TSIO_CC_ReceiveCmd(Parm->pOut, Parm->pOutLen) != DRVTSIO_OK)
    {
        if (*(Parm->pOutLen) < TSIO_CC_CMD_MIN_LEN)
        {
            *pResult = TSIO_CC_RESULT_RXCMD_TOOSMALL;
            goto FAIL;
        }

        if (*(Parm->pOutLen) > TSIO_CC_CMD_MAX_LEN)
        {
            *pResult = TSIO_CC_RESULT_RXCMD_TOOBIG;
            goto FAIL;
        }
    }

FAIL:
    if (*pResult != TSIO_CC_RESULT_OK)
        return DMX_FILTER_STATUS_ERROR;

    return DMX_FILTER_STATUS_OK;
#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

//-------------------------------------------------------------------------------------------------
/// Get TSIO informaiton
/// @param eInfo                                        \b IN: Info type
/// @param u32Arg                                      \b IN: detail value
/// @param u32Retinfo                                 \b OUT: Pointer to store Info address
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
DMX_FILTER_STATUS _MApi_DMX_TSIO_GetInfo(TSIO_INFO eInfo , MS_U32 u32Arg, MS_U32 *u32Retinfo)
{
#ifdef TSIO_ENABLE
    DRVTSIO_INFO drvInfo = DRVTSIO_INFO_MAX;
    switch (eInfo)
    {
        case TSIO_INFO_LOSELOCKCNT:
            drvInfo = DRVTSIO_INFO_LOSELOCKCNT;
            break;

        case TSIO_INFO_USEBUFLEN:
            drvInfo = DRVTSIO_INFO_USEBUFLEN;
            break;

        default:
            return DMX_FILTER_STATUS_ERROR;
    }

    if (MDrv_TSIO_GetInfo(drvInfo, u32Arg, u32Retinfo) != DRVTSIO_OK)
    {
        return DMX_FILTER_STATUS_ERROR;
    }

    return DMX_FILTER_STATUS_OK;

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

DMX_FILTER_STATUS _MApi_DMX_TSIO_WaitIoSignal(DMX_TSIO_IOSIGNAL *pDmxTsioIoSig)
{
#ifdef TSIO_ENABLE

    TSIO_Result Res;
    DRVTSIO_IOSIGNAL DrvIoSig;

    if((Res = MDrv_TSIO_Wait_IoSignal(&DrvIoSig)) != DRVTSIO_OK)
    {
        if(Res == DRVTSIO_FUNC_ERROR)
            return DMX_FILTER_STATUS_ERROR;
        else
            return DMX_FILTER_STATUS_OK;
    }

    pDmxTsioIoSig->u32Event = DrvIoSig.u32Event;
    pDmxTsioIoSig->u64DmainInt = DrvIoSig.u64DmainInt;
    pDmxTsioIoSig->u64DmaoutInt[0] = DrvIoSig.u64DmaoutInt[0];
    pDmxTsioIoSig->u64DmaoutInt[1] = DrvIoSig.u64DmaoutInt[1];
    pDmxTsioIoSig->u64DmaoutInt[2] = DrvIoSig.u64DmaoutInt[2];
    pDmxTsioIoSig->u64DmaoutInt[3] = DrvIoSig.u64DmaoutInt[3];

    return DMX_FILTER_STATUS_OK;

#else

    return DMX_FILTER_STATUS_ERROR;

#endif
}

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
char* _DMX_Str_Tok(char* pstrSrc, char* pstrDes, char delimit)
{
    char *pstrRes = pstrSrc;
    int i = 0;

    *pstrDes = '\0';

    if(pstrSrc == NULL)
        return NULL;

    while(*pstrRes != '\0')
    {
        if((*pstrRes == delimit) || (*pstrRes == ' '))
            break;
        *pstrDes++ = *pstrRes++;
    }
    *pstrDes = '\0';

    return (pstrRes + 1);
}

int _DMX_StrToInt(char* pstrSrc)
{
    int iRes = 0;
    char *pstrRes = pstrSrc;

    while(*pstrRes != '\0')
    {
        iRes *= 10;
        iRes += (int)((*pstrRes++) - '0');
    }

    return iRes;
}

static MS_BOOL _DMX_DBG_GetPidFltInfo(MS_U64* pu64ReqHdl)
{
    MS_U32 u32PidFltNum = 0, u32ii;
    DMX_FILTER_TYPE flttype;
    const char strType[][16] = {"VD", "AU", "AU2", "Sec", "Pes", "Pkt", "Pcr", "TTX", "V3D", "AU3", "AU4", "Rec", "ScmChk",
                             "Sec_VerChk", "VD3", "VD4", "Pvr1", "Pvr2", "Pvr3", "Pvr4", "Pvr5", "Pvr6", "Pvr7", "Pvr8", "Pvr9"};

    if(_MApi_DMX_GetCap(DMX_CAP_PID_FILTER_NUM, &u32PidFltNum) != DMX_FILTER_STATUS_OK)
    {
        MdbPrint(pu64ReqHdl,"[%d]DBG CMD Error!!!\n", __LINE__);
    }
    MdbPrint(pu64ReqHdl,"DMX PID Filter Num: %d\n", (int)u32PidFltNum);
    MdbPrint(pu64ReqHdl,"Id    PID        TS_Src     Type              Src_Id     Sec_Flt_Id     \n");
    MdbPrint(pu64ReqHdl,"====================================================================\n");

    for(u32ii = 0; u32ii < u32PidFltNum; u32ii++)
    {
        MdbPrint(pu64ReqHdl, "%d     %x       ", (int)u32ii, (int)_pdmx_res->_FltList[u32ii].u16Pid);
        switch(_pdmx_res->_FltList[u32ii].DmxFltType & DMX_FILTER_SOURCE_TYPE_MASK)
        {
            case DMX_FILTER_SOURCE_TYPE_LIVE:
                MdbPrint(pu64ReqHdl, "Live0    ");
                break;
            case DMX_FILTER_SOURCE_TYPE_FILE:
                MdbPrint(pu64ReqHdl, "File0    ");
                break;
            case DMX_FILTER_SOURCE_TYPE_TS1:
                MdbPrint(pu64ReqHdl, "Live1    ");
                break;
            case DMX_FILTER_SOURCE_TYPE_TS2:
                MdbPrint(pu64ReqHdl, "Live2    ");
                break;
            case DMX_FILTER_SOURCE_TYPE_TS3:
                MdbPrint(pu64ReqHdl, "Live3    ");
                break;
            case DMX_FILTER_SOURCE_TYPE_FILE1:
                MdbPrint(pu64ReqHdl, "File1    ");
                break;
            case DMX_FILTER_SOURCE_TYPE_FILE2:
                MdbPrint(pu64ReqHdl, "File2    ");
                break;
            case DMX_FILTER_SOURCE_TYPE_FILE3:
                MdbPrint(pu64ReqHdl, "File3    ");
                break;
            default:
                MdbPrint(pu64ReqHdl, "Unknown  ");
                break;
        }

        flttype = _pdmx_res->_FltList[u32ii].DmxFltType & ~(DMX_FILTER_SOURCE_TYPE_MASK|DMX_FILTER_SOURCEID_MASK);
        if(flttype <= DMX_FILTER_TYPE_PVR9)
        {
            MdbPrint(pu64ReqHdl, "  %s             ", &(strType[flttype][0]));
        }
        else
        {
            MdbPrint(pu64ReqHdl, "  Unknown       ");
        }
        flttype = _pdmx_res->_FltList[u32ii].DmxFltType & DMX_FILTER_SOURCEID_MASK;
        MdbPrint(pu64ReqHdl, "      %x        %x      \n", (int)flttype, (int)_pdmx_res->_FltList[u32ii].SecBufId);
    }

    return TRUE;
}

static MS_BOOL _DMX_DBG_GetSecFltInfo(MS_U64* pu64ReqHdl, int StarId, int EndId)
{
    MS_U32 u32SecFltNum = 0, u32ii, u32jj = 0, u32Size = 0;
    DMX_Sect_info* secinfo;
    MS_PHY phyStart = 0, phyEnd = 0, phyRead = 0, phyWrite = 0;
    MS_U8 u8Match[DRVTSP_FILTER_DEPTH], u8Mask[DRVTSP_FILTER_DEPTH], u8NMask[DRVTSP_FILTER_DEPTH];

    if(_MApi_DMX_GetCap(DMX_CAP_SEC_FILTER_NUM, &u32SecFltNum) != DMX_FILTER_STATUS_OK)
    {
        MdbPrint(pu64ReqHdl,"[%d]DBG CMD Error!!!\n", __LINE__);
    }
    MdbPrint(pu64ReqHdl,"DMX Section Filter Num: %d\n", (int)u32SecFltNum);
    MdbPrint(pu64ReqHdl,"Start Id: %d , End Id: %d \n", StarId, EndId);
    MdbPrint(pu64ReqHdl,"=======================================================================\n");
    MdbPrint(pu64ReqHdl,"Id           Mode             Buf_Addr          Buf_Size               \n");
    MdbPrint(pu64ReqHdl,"Start        End              Read              Write                  \n");
    MdbPrint(pu64ReqHdl,"Match                                                                  \n");
    MdbPrint(pu64ReqHdl,"Mask                                                                   \n");
    MdbPrint(pu64ReqHdl,"=======================================================================\n");

    if(EndId >= u32SecFltNum)
        EndId = u32SecFltNum - 1;
    for(u32ii = StarId; u32ii <= EndId; u32ii++)
    {
        secinfo = &(_pdmx_res->_FltList[u32ii].FltInfo.Info.SectInfo);
        MdbPrint(pu64ReqHdl, "%d        ", (int)u32ii);
        switch(secinfo->SectMode)
        {
            case DMX_SECT_MODE_CONTI:
                MdbPrint(pu64ReqHdl, "Cont           ");
                break;
            case DMX_SECT_MODE_ONESHOT:
                MdbPrint(pu64ReqHdl, "OneShot        ");
                break;
            case DMX_SECT_MODE_CRCCHK:
                MdbPrint(pu64ReqHdl, "CrcChk/Cont    ");
                break;
            case DMX_SECT_MODE_PESSCMCHK:
                MdbPrint(pu64ReqHdl, "PesScmChk     ");
                break;
            default:
                MdbPrint(pu64ReqHdl, "Unknown       ");
                break;
        }
        MdbPrint(pu64ReqHdl, "%x         %x \n", (int)secinfo->SectBufAddr, (int)secinfo->SectBufSize);
        MDrv_TSP_SecFlt_GetBufStart(0, u32ii, &phyStart);
        MDrv_TSP_SecFlt_GetBufSize(0, u32ii, &u32Size);
        MDrv_TSP_SecFlt_GetReadAddr(0, u32ii, &phyRead);
        MDrv_TSP_SecFlt_GetWriteAddr(0, u32ii, &phyWrite);
        phyEnd = phyStart + u32Size;
        MdbPrint(pu64ReqHdl, "%x        %x          %x         %x \n", (int)phyStart, (int)phyEnd, (int)phyRead, (int)phyWrite);
        MDrv_TSP_SecFlt_GetPattern(0, u32ii, u8Match, u8Mask, u8NMask);
        for(u32jj = 0; u32jj < DRVTSP_FILTER_DEPTH; u32jj++)
        {
            MdbPrint(pu64ReqHdl, "%x ", u8Match[u32jj]);
            if(((u32jj+1) % 4) == 0)
                MdbPrint(pu64ReqHdl, "    ");
        }
        MdbPrint(pu64ReqHdl, "\n");
        for(u32jj = 0; u32jj < DRVTSP_FILTER_DEPTH; u32jj++)
        {
            MdbPrint(pu64ReqHdl, "%x ", u8Mask[u32jj]);
            if(((u32jj+1) % 4) == 0)
                MdbPrint(pu64ReqHdl, "    ");
        }
        MdbPrint(pu64ReqHdl,"\n--------------------------------------------------------------------\n");
    }
    MdbPrint(pu64ReqHdl, "\n");

    return TRUE;
}

static MS_BOOL _DMX_DBG_GetDbgTblTsSrc(char* psubcmd, DMX_DEBUG_SRC *pdbgSrc)
{
    *pdbgSrc = DMX_DEBUG_SRC_TS0;

    if(strncmp(psubcmd, "ts0", 3) == 0)
    {
        *pdbgSrc = DMX_DEBUG_SRC_TS0;
    }
    else if(strncmp(psubcmd, "ts1", 3) == 0)
    {
        *pdbgSrc = DMX_DEBUG_SRC_TS1;
    }
    else if(strncmp(psubcmd, "ts2", 3) == 0)
    {
        *pdbgSrc = DMX_DEBUG_SRC_TS2;
    }
    else if(strncmp(psubcmd, "ts3", 3) == 0)
    {
        *pdbgSrc = DMX_DEBUG_SRC_TS3;
    }
    else if(strncmp(psubcmd, "fi0", 3) == 0)
    {
        *pdbgSrc = DMX_DEBUG_SRC_FILE;
    }
    else if(strncmp(psubcmd, "fi1", 3) == 0)
    {
        *pdbgSrc = DMX_DEBUG_SRC_FILE1;
    }
    else if(strncmp(psubcmd, "fi2", 3) == 0)
    {
        *pdbgSrc = DMX_DEBUG_SRC_FILE2;
    }
    else if(strncmp(psubcmd, "fi3", 3) == 0)
    {
        *pdbgSrc = DMX_DEBUG_SRC_FILE3;
    }
    else if(strncmp(psubcmd, "mf0", 3) == 0)
    {
        *pdbgSrc = DMX_DEBUG_SRC_MMFI0;
    }
    else if(strncmp(psubcmd, "mf1", 3) == 0)
    {
        *pdbgSrc = DMX_DEBUG_SRC_MMFI1;
    }
    else
        return FALSE;

    return TRUE;
}

static MS_BOOL _DMX_DBG_GetDbgTblFifoSrc(char* psubcmd, DMX_DEBUG_FIFO *pdbgFifo)
{
    *pdbgFifo = DMX_DEBUG_FIFO_VIDEO;

    if(strncmp(psubcmd, "vd0", 3) == 0)
    {
        *pdbgFifo = DMX_DEBUG_FIFO_VIDEO;
    }
    else if(strncmp(psubcmd, "au0", 3) == 0)
    {
        *pdbgFifo = DMX_DEBUG_FIFO_AUDIO;
    }
    else if(strncmp(psubcmd, "vd1", 3) == 0)
    {
        *pdbgFifo = DMX_DEBUG_FIFO_VIDEO3D;
    }
    else if(strncmp(psubcmd, "au2", 3) == 0)
    {
        *pdbgFifo = DMX_DEBUG_FIFO_AUDIOB;
    }
    else if(strncmp(psubcmd, "au3", 3) == 0)
    {
        *pdbgFifo = DMX_DEBUG_FIFO_AUDIOC;
    }
    else if(strncmp(psubcmd, "au4", 3) == 0)
    {
        *pdbgFifo = DMX_DEBUG_FIFO_AUDIOD;
    }
    else if(strncmp(psubcmd, "au4", 3) == 0)
    {
        *pdbgFifo = DMX_DEBUG_FIFO_AUDIOD;
    }
    else
        return FALSE;

    return TRUE;
}

static MS_BOOL _DMX_DBG_GetDisContCnt(MS_U64* pu64ReqHdl, char* psubcmd)
{
    DMX_DisContiCnt_info discInfo;
    MS_U32 u32Cnt = 0;

    //MdbPrint(pu64ReqHdl, "[%d] %s\n", __LINE__, psubcmd);

    if((_pdmx_res == NULL) || (_pdmx_res->_bFWInit == FALSE))
        return DMX_FILTER_STATUS_ERROR;

    discInfo.TspCmd = DMX_DEBUG_CMD_ENABLE;
    discInfo.TspSrc = DMX_DEBUG_SRC_TS0;
    discInfo.TspFifo = DMX_DEBUG_FIFO_VIDEO;

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
    if(_DMX_DBG_GetDbgTblTsSrc(psubcmd, &discInfo.TspSrc) == FALSE)
        return FALSE;
#else
    if(_DMX_DBG_GetDbgTblFifoSrc(psubcmd, &discInfo.TspFifo) == FALSE)
        return FALSE;
#endif

    if(MApi_DMX_Get_DisContiCnt(&discInfo, &u32Cnt) != DMX_FILTER_STATUS_OK)
        return FALSE;

    MdbPrint(pu64ReqHdl,"[Dis-continue count] = %d\n", (int)u32Cnt);

    return TRUE;
}

static MS_BOOL _DMX_DBG_GetDropCnt(MS_U64* pu64ReqHdl, char* psubcmd)
{
    DMX_DropPktCnt_info dropInfo;
    MS_U32 u32Cnt = 0;

    //MdbPrint(pu64ReqHdl, "[%d] %s\n", __LINE__, psubcmd);

    if((_pdmx_res == NULL) || (_pdmx_res->_bFWInit == FALSE))
        return DMX_FILTER_STATUS_ERROR;

    dropInfo.TspCmd = DMX_DEBUG_CMD_ENABLE;
    dropInfo.TspSrc = DMX_DEBUG_SRC_TS0;
    dropInfo.TspFifo = DMX_DEBUG_FIFO_VIDEO;

#if (defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_4_0))
    if(_DMX_DBG_GetDbgTblTsSrc(psubcmd, &dropInfo.TspSrc) == FALSE)
        return FALSE;
#else
    if(_DMX_DBG_GetDbgTblFifoSrc(psubcmd, &dropInfo.TspFifo) == FALSE)
        return FALSE;
#endif

    if(MApi_DMX_Get_DropPktCnt(&dropInfo, &u32Cnt) != DMX_FILTER_STATUS_OK)
        return FALSE;

    MdbPrint(pu64ReqHdl,"[Drop count] = %d\n", (int)u32Cnt);

    return TRUE;
}

static MS_BOOL _DMX_DBG_GetLockCnt(MS_U64* pu64ReqHdl, char* psubcmd)
{
    DMX_LockPktCnt_info lockInfo;
    DMX_DEBUG_SRC eTspSrc = DMX_DEBUG_SRC_TS0;
    MS_U32 u32Cnt = 0;

    //MdbPrint(pu64ReqHdl, "[%d] %s\n", __LINE__, psubcmd);

    if((_pdmx_res == NULL) || (_pdmx_res->_bFWInit == FALSE))
        return DMX_FILTER_STATUS_ERROR;

    lockInfo.TspCmd = DMX_DEBUG_CMD_ENABLE;
    lockInfo.TspTsif = DMX_DEBUG_TSIF_TS0;

    if(_DMX_DBG_GetDbgTblTsSrc(psubcmd, &eTspSrc) == FALSE)
        return FALSE;

    switch(eTspSrc)
    {
        case DMX_DEBUG_SRC_TS0:
            lockInfo.TspTsif = DMX_DEBUG_TSIF_TS0;
            break;
        case DMX_DEBUG_SRC_TS1:
            lockInfo.TspTsif = DMX_DEBUG_TSIF_TS1;
            break;
        case DMX_DEBUG_SRC_TS2:
            lockInfo.TspTsif = DMX_DEBUG_TSIF_TS2;
            break;
        case DMX_DEBUG_SRC_TS3:
            lockInfo.TspTsif = DMX_DEBUG_TSIF_TS3;
            break;
        default:
            return FALSE;
    }

    if(MApi_DMX_Get_LockPktCnt(&lockInfo, &u32Cnt) != DMX_FILTER_STATUS_OK)
        return FALSE;

    MdbPrint(pu64ReqHdl,"[Lock count] = %d\n", (int)u32Cnt);

    return TRUE;
}

static MS_BOOL _DMX_DBG_GetAVPktCnt(MS_U64* pu64ReqHdl, char* psubcmd)
{
    DMX_AVPktCnt_info avpktInfo;
    DMX_DEBUG_SRC eTspSrc = DMX_DEBUG_SRC_TS0;
    MS_U32 u32Cnt = 0;

    //MdbPrint(pu64ReqHdl, "[%d] %s\n", __LINE__, psubcmd);

    if((_pdmx_res == NULL) || (_pdmx_res->_bFWInit == FALSE))
        return DMX_FILTER_STATUS_ERROR;

    avpktInfo.TspCmd = DMX_DEBUG_CMD_ENABLE;
    avpktInfo.TspFifo = DMX_DEBUG_FIFO_VIDEO;
    avpktInfo.TspFifoSrc = DMX_DEBUG_PKT_DEMUX_0;

    if(_DMX_DBG_GetDbgTblTsSrc(psubcmd, &eTspSrc) == FALSE)
        return FALSE;

    switch(eTspSrc)
    {
        case DMX_DEBUG_SRC_TS0:
            avpktInfo.TspFifoSrc = DMX_DEBUG_PKT_DEMUX_0;
            break;
        case DMX_DEBUG_SRC_TS1:
            avpktInfo.TspFifoSrc = DMX_DEBUG_PKT_DEMUX_1;
            break;
        case DMX_DEBUG_SRC_TS2:
            avpktInfo.TspFifoSrc = DMX_DEBUG_PKT_DEMUX_2;
            break;
        case DMX_DEBUG_SRC_FILE:
            avpktInfo.TspFifoSrc = DMX_DEBUG_PKT_DEMUX_0_FILE;
            break;
        case DMX_DEBUG_SRC_FILE1:
            avpktInfo.TspFifoSrc = DMX_DEBUG_PKT_DEMUX_1_FILE;
            break;
        case DMX_DEBUG_SRC_FILE2:
            avpktInfo.TspFifoSrc = DMX_DEBUG_PKT_DEMUX_2_FILE;
            break;
        case DMX_DEBUG_SRC_MMFI0:
            avpktInfo.TspFifoSrc = DMX_DEBUG_MMFI0;
            break;
        case DMX_DEBUG_SRC_MMFI1:
            avpktInfo.TspFifoSrc = DMX_DEBUG_MMFI1;
            break;
        default:
            return FALSE;
    }

    if(_DMX_DBG_GetDbgTblFifoSrc(psubcmd + 4, &avpktInfo.TspFifo) == FALSE)
        return FALSE;

    if(MApi_DMX_Get_AVPktCnt(&avpktInfo, &u32Cnt) != DMX_FILTER_STATUS_OK)
        return FALSE;

    MdbPrint(pu64ReqHdl,"[AV Packet count] = %d\n", (int)u32Cnt);

    return TRUE;
}

DMX_FILTER_STATUS _MApi_DMX_DBG_GetModuleInfo(MS_U64* pu64ReqHdl)
{
    MS_U16          u16Clk = 0;
    DrvTSP_If       eif = E_DRVTSP_IF_TS0;
    DrvTSP_If_Set   Ifset;
    MS_U32          u32TSIFNum = 0, u32PidFltNum = 0, u32SecFltNum, u32VFifo, u32AFifo, U32Dscmb;
    MS_U32          u32ii = 0;
    const char      strPad[][16] = {
                        "Pad_TS0", "Pad_TS1", "Pad_TS2", "Pad_TS3",
                        "Pad_TS4", "Pad_TS5", "Pad_TSO", "Inter_Dmd",
                        "TS0_3Wire", "TS1_3Wire", "TS2_3Wire", "TS3_3Wire",
                        "TS4_3Wire", "TS5_3Wire"};

    if((_pdmx_res == NULL) || (_pdmx_res->_bFWInit == FALSE))
        return DMX_FILTER_STATUS_ERROR;

    if(_MApi_DMX_GetCap(DMX_CAP_TSIF_NUM, &u32TSIFNum) != DMX_FILTER_STATUS_OK)
    {
        MdbPrint(pu64ReqHdl,"[%d]Get DMX module info fail....\n", __LINE__);
    }
    if(_MApi_DMX_GetCap(DMX_CAP_PID_FILTER_NUM, &u32PidFltNum) != DMX_FILTER_STATUS_OK)
    {
        MdbPrint(pu64ReqHdl,"[%d]Get DMX module info fail....\n", __LINE__);
    }
    if(_MApi_DMX_GetCap(DMX_CAP_SEC_FILTER_NUM, &u32SecFltNum) != DMX_FILTER_STATUS_OK)
    {
        MdbPrint(pu64ReqHdl,"[%d]Get DMX module info fail....\n", __LINE__);
    }
    if(_MApi_DMX_GetCap(DMX_CAP_VFIFO_NUM, &u32VFifo) != DMX_FILTER_STATUS_OK)
    {
        MdbPrint(pu64ReqHdl,"[%d]Get DMX module info fail....\n", __LINE__);
    }
    if(_MApi_DMX_GetCap(DMX_CAP_AFIFO_NUM, &u32AFifo) != DMX_FILTER_STATUS_OK)
    {
        MdbPrint(pu64ReqHdl,"[%d]Get DMX module info fail....\n", __LINE__);
    }
    if(_MApi_DMX_GetCap(DMX_CAP_DSCMB_ENG_NUM, &U32Dscmb) != DMX_FILTER_STATUS_OK)
    {
        MdbPrint(pu64ReqHdl,"[%d]Get DMX module info fail....\n", __LINE__);
    }

    MdbPrint(pu64ReqHdl,"DMX Module Info: \nTotal DMX path: %d\n", (int)u32TSIFNum);
    MdbPrint(pu64ReqHdl,"Pid Filter Num: %d, Section Filter Num: %d\n", (int)u32PidFltNum, (int)u32SecFltNum);
    MdbPrint(pu64ReqHdl,"VFIFO Num: %d, AFIFO Num: %d\n", (int)u32VFifo, (int)u32AFifo);
    MdbPrint(pu64ReqHdl,"DSCMB Engine Num: %d\n", (int)U32Dscmb);
    MdbPrint(pu64ReqHdl,"TS_ID    Input_Src      Clk     Clk_Invert Sync_Mode   Pad_Mode    \n");
    MdbPrint(pu64ReqHdl,"===================================================================\n");

    Ifset.ePad = E_DRVTSP_PAD_EXT_INPUT0;
    Ifset.bClkInv = FALSE;
    Ifset.bExtSync = FALSE;
    Ifset.bParallel = FALSE;

    for(u32ii = 0; u32ii < u32TSIFNum; u32ii++)
    {
        eif = (DrvTSP_If)u32ii;
        Ifset.ePad = E_DRVTSP_PAD_DEMOD;
        Ifset.bClkInv = FALSE;
        Ifset.bExtSync = FALSE;
        Ifset.bParallel = FALSE;

        MDrv_TSP_GetTSIFStatus(eif, &Ifset, &u16Clk);

        MdbPrint(pu64ReqHdl,"TSIF%d    ", u32ii);
        MdbPrint(pu64ReqHdl,"%s      ", &(strPad[Ifset.ePad][0]));
        MdbPrint(pu64ReqHdl,"0x%x    ", u16Clk);

        if(Ifset.bClkInv)
            MdbPrint(pu64ReqHdl,"Invert       ");
        else
            MdbPrint(pu64ReqHdl,"None       ");

        if(Ifset.bExtSync)
            MdbPrint(pu64ReqHdl,"External    ");
        else
            MdbPrint(pu64ReqHdl,"Internal    ");

        if(Ifset.bParallel)
            MdbPrint(pu64ReqHdl,"Parallel    \n");
        else
            MdbPrint(pu64ReqHdl,"Serial      \n");
    }

    MdbPrint(pu64ReqHdl, "\n");

    return DMX_FILTER_STATUS_OK;
}

DMX_FILTER_STATUS _MApi_DMX_DBG_EchoCmd(MS_U64* pu64ReqHdl, MS_U32 u32CmdSize, char* pcmd)
{
    char* ptr = NULL;
    char strbuf[128] = {0};

    if((_pdmx_res == NULL) || (_pdmx_res->_bFWInit == FALSE))
        return DMX_FILTER_STATUS_ERROR;

    //MdbPrint(pu64ReqHdl, "[%d] %s\n", __LINE__, pcmd);

    if(strncmp(pcmd, "pid_flt", 7) == 0)
    {
        if(_DMX_DBG_GetPidFltInfo(pu64ReqHdl) == TRUE)
            return DMX_FILTER_STATUS_OK;
    }
    else if(strncmp(pcmd, "sec_flt=", 8) == 0)
    {
        int istart = 0, iend = 0;

        ptr = pcmd + 8;
        ptr = _DMX_Str_Tok(ptr, strbuf, ',');
        istart = _DMX_StrToInt(strbuf);
        ptr = _DMX_Str_Tok(ptr, strbuf, ',');
        iend = _DMX_StrToInt(strbuf);

        if(_DMX_DBG_GetSecFltInfo(pu64ReqHdl, istart, iend) == TRUE)
            return DMX_FILTER_STATUS_OK;
    }
    else if(strncmp(pcmd, "dgtbl=", 6) == 0)
    {
        ptr = pcmd + 6;

        //MdbPrint(pu64ReqHdl, "[%d] %s\n", __LINE__, ptr);

        if(strncmp(ptr, "dc_cnt,", 7) == 0)
        {
            if(_DMX_DBG_GetDisContCnt(pu64ReqHdl, ptr + 7) == TRUE)
                return DMX_FILTER_STATUS_OK;
        }
        else if(strncmp(ptr, "dp_cnt,", 7) == 0)
        {
            if(_DMX_DBG_GetDropCnt(pu64ReqHdl, ptr + 7) == TRUE)
                return DMX_FILTER_STATUS_OK;
        }
        else if(strncmp(ptr, "lock_cnt,", 9) == 0)
        {
            if(_DMX_DBG_GetLockCnt(pu64ReqHdl, ptr + 9) == TRUE)
                return DMX_FILTER_STATUS_OK;
        }
        else if(strncmp(ptr, "av_cnt,", 7) == 0)
        {
            if(_DMX_DBG_GetAVPktCnt(pu64ReqHdl, ptr + 7) == TRUE)
                return DMX_FILTER_STATUS_OK;
        }
    }

    MdbPrint(pu64ReqHdl,"[%d]DBG CMD Error!!!\n", __LINE__);
    return DMX_FILTER_STATUS_ERROR;
}
#endif //CONFIG_UTOPIA_PROC_DBG_SUPPORT
