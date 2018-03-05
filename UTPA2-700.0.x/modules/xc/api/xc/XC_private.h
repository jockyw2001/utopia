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
// comply with all applicable laws and regulations
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
//    or otherwise
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

#ifndef _API_XC_PRIV_H_
#define _API_XC_PRIV_H_

#include "UFO.h"

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////

#include "utopia_relation.h"

#ifdef __cplusplus
extern "C"
{
#endif

/// Define XC device enum
typedef enum
{
    E_XC_DEVICE0,
    E_XC_DEVICE1,
    E_XC_DEVICE_MAX,
} XC_DEVICE_NUM;

typedef enum
{
    E_XC_UNSET,
    E_XC_FC100,
    E_XC_FC85,
    E_XC_FC50,
} XC_SC1_HVSPFILTER;

// APIXC_H
typedef E_APIXC_ReturnValue              (*IOCTL_XC_GETLIBVER)                               (void*, const MSIF_Version**);
typedef XC_ApiInfo*                      (*IOCTL_XC_GETINFO)                                 (void*);
typedef MS_BOOL                          (*IOCTL_XC_GETSTATUS)                               (void*, XC_ApiStatus*, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_GETSTATUS_NODELAY)                       (void*, ST_XC_APISTATUSNODELAY*, SCALER_WIN);
typedef MS_U16                           (*IOCTL_XC_GETSTATUSEX)                             (void*, XC_ApiStatusEx *, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_SETDBGLEVEL)                             (void*, MS_U16);
typedef void                             (*IOCTL_XC_FPLLDBGMODE)                             (void*, EN_FPLL_DBG_MODE, EN_FPLL_DBG_FLAG, MS_U32);
typedef void                             (*IOCTL_XC_FPLLCUSTOMERMODE)                        (void*, EN_FPLL_MODE, EN_FPLL_FLAG, MS_U32);
typedef MS_BOOL                          (*IOCTL_XC_SETIOMAPBASE)                            (void*);
typedef MS_BOOL                          (*IOCTL_XC_INIT)                                    (void*, XC_INITDATA*, MS_U32);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_GETCONFIG)                               (void*, XC_INITDATA*);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_INIT_MISC)                               (void*, XC_INITMISC *, MS_U32);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_GETMISCSTATUS)                           (void*, XC_INITMISC*);
typedef MS_U32                           (*IOCTL_XC_GETCAPABILITY)                           (void*, MS_U32);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_GETCHIPCAPS)                             (void*, E_XC_CAPS, MS_U32*, MS_U32);
typedef MS_BOOL                          (*IOCTL_XC_EXIT)                                    (void*);
typedef MS_BOOL                          (*IOCTL_XC_SETDYNAMICSCALING)                       (void*, XC_DynamicScaling_Info*, MS_U32, SCALER_WIN);
typedef void                               (*IOCTL_XC_SETDYNAMICSCALING_FLAG)                  (void*, MS_BOOL);
typedef MS_PHY                           (*IOCTL_XC_GET_DNRBASEOFFSET)                       (void*, SCALER_WIN);
typedef MS_U8                            (*IOCTL_XC_GET_FRAMENUMFACTOR)                      (void*, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_SETWINDOW)                               (void*, XC_SETWIN_INFO*, MS_U32, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_SETDUALWINDOW)                           (void*, XC_SETWIN_INFO*, XC_SETWIN_INFO*);
typedef MS_BOOL                          (*IOCTL_XC_SETTRAVELINGWINDOW)                      (void*, XC_SETWIN_INFO*, MS_U32, SCALER_WIN);
typedef void                             (*IOCTL_XC_SETINPUTSOURCE)                          (void*, INPUT_SOURCE_TYPE_t, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_ISYUVSPACE)                              (void*, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_ISMEMORYFORMAT422)                       (void*, SCALER_WIN);
typedef void                             (*IOCTL_XC_ENABLEFORCERGBIN)                        (void*, MS_BOOL, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_ENABLEMIRRORMODEEX)                      (void*, MirrorMode_t,  SCALER_WIN);
typedef MirrorMode_t                     (*IOCTL_XC_GETMIRRORMODETYPEEX)                     (void*, SCALER_WIN);
typedef void                             (*IOCTL_XC_GETSYNCSTATUS)                           (void*, INPUT_SOURCE_TYPE_t , XC_IP_SYNC_STATUS *, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_CONFIGCMA)                               (void*, XC_CMA_CONFIG *, XC_CMA_CLIENT, MS_U32, SCALER_WIN);
typedef MS_U8                            (*IOCTL_XC_WAITOUTPUTVSYNC)                         (void*, MS_U8, MS_U16, SCALER_WIN);
typedef MS_U8                            (*IOCTL_XC_WAITINPUTVSYNC)                          (void*, MS_U8, MS_U16, SCALER_WIN);
typedef void                             (*IOCTL_XC_SETHDMISYNCMODE)                         (void*, E_HDMI_SYNC_TYPE);
typedef E_HDMI_SYNC_TYPE                 (*IOCTL_XC_GETHDMISYNCMODE)                         (void*);
typedef void                             (*IOCTL_XC_SETREPWINDOW)                            (void*, MS_BOOL,MS_WINDOW_TYPE,MS_U8);
typedef void                             (*IOCTL_XC_SET_SKIP_OPWRITEOFF_IN_SETWINDOW)        (void*, MS_BOOL, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_GET_SKIP_OPWRITEOFF_IN_SETWINDOW)        (void*, SCALER_WIN);
typedef void                             (*IOCTL_XC_SET_SKIP_DISABLE_OPWRITEOFF_IN_FPLL)     (void*, MS_BOOL, SCALER_WIN);
typedef void                             (*IOCTL_XC_SET_OPWRITEOFFENABLE)                    (void*, MS_BOOL, SCALER_WIN);
typedef void                             (*IOCTL_XC_SET_OPWRITEOFF_ENABLE_TO_REG)            (void*, MS_BOOL, SCALER_WIN);
typedef void                             (*IOCTL_XC_FORCESET_OPWRITEOFF_ENABLE)              (void*, MS_BOOL, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_GET_OPWRITEOFFENABLE)                    (void*, SCALER_WIN);
typedef void                             (*IOCTL_XC_SETDISPWINTOREG)                         (void*, MS_WINDOW_TYPE *, SCALER_WIN);
typedef void                             (*IOCTL_XC_GETDISPWINFROMREG)                       (void*, MS_WINDOW_TYPE *, SCALER_WIN);
typedef void                             (*IOCTL_XC_FREEZEIMG)                               (void*, MS_BOOL, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_ISFREEZEIMG)                             (void*, SCALER_WIN);
typedef void                             (*IOCTL_XC_GENERATEBLACKVIDEOFORBOTHWIN)            (void*, MS_BOOL);
typedef void                             (*IOCTL_XC_SET_BLSK)                                (void*, MS_BOOL);
typedef void                             (*IOCTL_XC_GENERATEBLACKVIDEO)                      (void*, MS_BOOL, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_ISBLACKVIDEOENABLE)                      (void*, SCALER_WIN);
typedef void                             (*IOCTL_XC_ENABLEFRAMEBUFFERLESS)                   (void*, MS_BOOL);
typedef MS_BOOL                          (*IOCTL_XC_ISCURRENTFRAMEBUFFERLESSMODE)            (void*);
typedef void                             (*IOCTL_XC_ENABLEREQUEST_FRAMEBUFFERLESS)           (void*, MS_BOOL);
typedef MS_BOOL                          (*IOCTL_XC_ISCURRENTREQUEST_FRAMEBUFFERLESSMODE)    (void*);
typedef MS_U16                           (*IOCTL_XC_GET_3D_HW_VERSION)                       (void*);
typedef MS_BOOL                          (*IOCTL_XC_GET_3D_ISSUPPORTEDHW2DTO3D)              (void*);
typedef MS_BOOL                          (*IOCTL_XC_SET_3D_MODE)                             (void*, E_XC_3D_INPUT_MODE, E_XC_3D_OUTPUT_MODE, E_XC_3D_PANEL_TYPE, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_SET_3D_MAINWIN_FIRSTMODE)                (void*, MS_BOOL);
typedef MS_BOOL                          (*IOCTL_XC_SET_3D_LR_FRAME_EXCHG)                   (void*, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_3D_IS_LR_FRAME_EXCHGED)                  (void*, SCALER_WIN);
typedef E_XC_3D_INPUT_MODE               (*IOCTL_XC_GET_3D_INPUT_MODE)                       (void*, SCALER_WIN);
typedef E_XC_3D_OUTPUT_MODE              (*IOCTL_XC_GET_3D_OUTPUT_MODE)                      (void*);
#ifdef UFO_XC_GET_3D_FORMAT
typedef MS_BOOL                         (*IOCTL_XC_GET3DFORMAT)                       (void*, E_XC_3D_ATTRIBUTE_TYPE e3DAttrType, void* para, void* p3DFormat);
#endif
typedef E_XC_3D_PANEL_TYPE               (*IOCTL_XC_GET_3D_PANEL_TYPE)                       (void*);
typedef MS_BOOL                          (*IOCTL_XC_GET_3D_MAINWIN_FIRST)                    (void*);
typedef MS_BOOL                          (*IOCTL_XC_3DMAINSUB_IPSYNC)                        (void*);
typedef MS_BOOL                          (*IOCTL_XC_SET_3D_VERVIDEOOFFSET)                   (void*, MS_U16);
typedef MS_U16                           (*IOCTL_XC_GET_3D_VERVIDEOOFFSET)                   (void*);
typedef MS_BOOL                          (*IOCTL_XC_IS3DFORMATSUPPORTED)                     (void*, E_XC_3D_INPUT_MODE,E_XC_3D_OUTPUT_MODE);
typedef MS_BOOL                          (*IOCTL_XC_SET_3D_HSHIFT)                           (void*, MS_U16);
typedef MS_BOOL                          (*IOCTL_XC_ENABLE_3D_LR_SBS2LINE)                   (void*, MS_BOOL);
typedef MS_U16                           (*IOCTL_XC_GET_3D_HSHIFT)                           (void*);
typedef MS_BOOL                          (*IOCTL_XC_SET_3D_HW2DTO3D_BUFFER)                  (void*, MS_PHY, MS_PHY);
typedef MS_BOOL                          (*IOCTL_XC_SET_3D_HW2DTO3D_PARAMETERS)              (void*, MS_XC_3D_HW2DTO3D_PARA);
typedef MS_BOOL                          (*IOCTL_XC_GET_3D_HW2DTO3D_PARAMETERS)              (void*, MS_XC_3D_HW2DTO3D_PARA*);
typedef MS_BOOL                          (*IOCTL_XC_SET_3D_DETECT3DFORMAT_PARAMETERS)        (void*, MS_XC_3D_DETECT3DFORMAT_PARA*);
typedef MS_BOOL                          (*IOCTL_XC_GET_3D_DETECT3DFORMAT_PARAMETERS)        (void*, MS_XC_3D_DETECT3DFORMAT_PARA*);
typedef E_XC_3D_INPUT_MODE               (*IOCTL_XC_DETECT3DFORMATBYCONTENT)                 (void*, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_DETECTNL)                                (void*, SCALER_WIN, ST_DETECTNL_PARA*);
typedef MS_BOOL                          (*IOCTL_XC_3D_POSTPQSETTING)                        (void*, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_SET_3D_FPINFO)                           (void*, MS_XC_3D_FPINFO_PARA *);
typedef MS_BOOL                          (*IOCTL_XC_ENABLEAUTODETECT3D)                      (void*, MS_BOOL, E_XC_3D_AUTODETECT_METHOD);
typedef MS_BOOL                          (*IOCTL_XC_GETAUTODETECT3DFLAG)                     (void*, E_XC_3D_AUTODETECT_METHOD*, MS_BOOL*);
typedef MS_BOOL                          (*IOCTL_XC_SET_3D_SUBWINCLK)                        (void*);
typedef MS_BOOL                          (*IOCTL_XC_3D_IS_LR_SBS2LINE)                       (void*);
typedef MS_BOOL                          (*IOCTL_SC_3D_IS_SKIP_DEFAULT_LR_FLAG)              (void*);
typedef MS_BOOL                          (*IOCTL_XC_3D_ENABLE_SKIP_DEFAULT_LR_FLAG)          (void*, MS_BOOL);
typedef void                             (*IOCTL_XC_MUX_INIT)                                (void*, void (*input_source_to_input_port)(INPUT_SOURCE_TYPE_t, E_MUX_INPUTPORT*, MS_U8*));
typedef void                             (*IOCTL_XC_MUX_SOURCEMONITOR)                       (void*, MS_BOOL);
typedef MS_S16                           (*IOCTL_XC_MUX_CREATEPATH)                          (void*, XC_MUX_PATH_INFO*, MS_U32);
typedef MS_S16                           (*IOCTL_XC_MUX_DELETEPATH)                          (void*, INPUT_SOURCE_TYPE_t, E_DEST_TYPE);
typedef MS_S16                           (*IOCTL_XC_MUX_ENABLEPATH)                          (void*, MS_U16);
typedef void                             (*IOCTL_XC_MUX_TRIGGERPATHSYNCEVENT)                (void*, INPUT_SOURCE_TYPE_t, void*);
typedef void                             (*IOCTL_XC_MUX_TRIGGERDESTONOFFEVENT)               (void*, INPUT_SOURCE_TYPE_t , void*);
typedef MS_S16                           (*IOCTL_XC_MUX_ONOFFPERIODICHANDLER)                (void*, INPUT_SOURCE_TYPE_t, MS_BOOL);
typedef MS_U8                            (*IOCTL_XC_MUX_GETPATHINFO)                         (void*, XC_MUX_PATH_INFO*);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_MUX_SETSUPPORTMHLPATHINFO)               (void*, MS_U8);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_MUX_SETMHLHOTPLUGINVERSEINFO)            (void*, MS_BOOL);
typedef E_MUX_INPUTPORT                  (*IOCTL_XC_MUX_GETHDMIPORT)                         (void*, INPUT_SOURCE_TYPE_t);
typedef void                             (*IOCTL_XC_MUX_GETMAPPINGTAB)                       (void*, XC_MUX_INPUTSRCTABLE* ,MS_U32);
typedef MS_U8                            (*IOCTL_XC_MUX_MAPINPUTSOURCETOVDYMUXPORT)          (void*, INPUT_SOURCE_TYPE_t);
typedef void                             (*IOCTL_XC_SET_NR)                                  (void*, MS_BOOL, SCALER_WIN);
typedef void                             (*IOCTL_XC_FILMMODE_P)                              (void*);
typedef MS_BOOL                          (*IOCTL_XC_GETUCENABLED)                            (void*, SCALER_WIN);
typedef void                             (*IOCTL_XC_GENSPECIFICTIMING)                       (void*, XC_Internal_TimingType);
typedef MS_BOOL                          (*IOCTL_XC_GETDEBYPASSMODE)                         (void*, SCALER_WIN);
typedef void                             (*IOCTL_XC_GETDEWINDOW)                             (void*, MS_WINDOW_TYPE *, SCALER_WIN);
typedef void                             (*IOCTL_XC_GETDEWIDTHHEIGHTINDEBYPASSMODE)          (void*, MS_U16*,MS_U16* ,SCALER_WIN);
typedef void                             (*IOCTL_XC_GETCAPTUREWINDOW)                        (void*, MS_WINDOW_TYPE*, SCALER_WIN);
typedef void                             (*IOCTL_XC_SETCAPTUREWINDOWVSTART)                  (void*, MS_U16 , SCALER_WIN);
typedef void                             (*IOCTL_XC_SETCAPTUREWINDOWHSTART)                  (void*, MS_U16 , SCALER_WIN);
typedef void                             (*IOCTL_XC_SETCAPTUREWINDOWVSIZE)                   (void*, MS_U16 , SCALER_WIN);
typedef void                             (*IOCTL_XC_SETCAPTUREWINDOWHSIZE)                   (void*, MS_U16 , SCALER_WIN);
typedef void                             (*IOCTL_XC_SOFTWARERESET)                           (void*, MS_U8, SCALER_WIN);
typedef MS_U16                           (*IOCTL_XC_CALCULATEHFREQX10)                       (void*, MS_U16);
typedef MS_U32                           (*IOCTL_XC_CALCULATEHFREQX1K)                       (void*, MS_U16);
typedef MS_U16                           (*IOCTL_XC_CALCULATEVFREQX10)                       (void*, MS_U16, MS_U16);
typedef MS_U32                           (*IOCTL_XC_CALCULATEVFREQX1K)                       (void*, MS_U32, MS_U16);
typedef MS_U32                           (*IOCTL_XC_GETACCURATEVFREQX1K)                     (void*, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_INTERRUPTATTACH)                         (void*, SC_INT_SRC, SC_InterruptCb, void *);
typedef MS_BOOL                          (*IOCTL_XC_INTERRUPTDEATTACH)                       (void*, SC_INT_SRC, SC_InterruptCb, void *);
typedef void                             (*IOCTL_XC_DISABLEINPUTSOURCE)                      (void*, MS_BOOL, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_ISINPUTSOURCEDISABLED)                   (void*, SCALER_WIN);
typedef void                             (*IOCTL_XC_CHANGEPANELTYPE)                         (void*, XC_PANEL_INFO *);
typedef MS_U16                           (*IOCTL_XC_GETCURRENTREADBANK)                      (void*, SCALER_WIN);
typedef MS_U16                           (*IOCTL_XC_GETCURRENTWRITEBANK)                     (void*, SCALER_WIN);
typedef void                             (*IOCTL_XC_SETAUTOPRESCALING)                       (void*, MS_BOOL, SCALER_WIN);
typedef MS_U8                            (*IOCTL_XC_GETVSYNCWIDTH)                           (void*, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_SET_GOP_ENABLE)                          (void*, MS_U8, MS_U8, MS_U8, MS_BOOL);
typedef void                             (*IOCTL_XC_IP_SEL_FOR_GOP)                          (void*, MS_U8 , MS_XC_IPSEL_GOP);
typedef void                             (*IOCTL_XC_SETVOPNBL)                               (void*);
typedef MS_BOOL                          (*IOCTL_XC_GETDSTINFO)                              (void*, MS_XC_DST_DispInfo *, MS_U32, EN_GOP_XCDST_TYPE);
typedef void                             (*IOCTL_XC_SET_FD_MASK_BYWIN)                       (void*, MS_BOOL, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_GET_FD_MASK_BYWIN)                       (void*, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_GET_FD_MASK_STATUS)                       (void*, SCALER_WIN);
typedef void                             (*IOCTL_XC_SETIP1TESTPATTERN)                       (void*, MS_U8, MS_U16, SCALER_WIN);
#ifdef UFO_XC_TEST_PATTERN
typedef void                             (*IOCTL_XC_GENERATETESTPATTERN)                     (void*, EN_XC_TEST_PATTERN_MODE,void*, MS_U16);
#endif
typedef void                             (*IOCTL_XC_INITIPFORINTERNALTIMING)                 (void*, XC_Internal_TimingType);
typedef void                             (*IOCTL_XC_SETIPMUX)                                (void*, MS_U8);
typedef MS_BOOL                          (*IOCTL_XC_IS_H_SYNC_ACTIVE)                        (void*, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_IS_V_SYNC_ACTIVE)                        (void*, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_GETAUTOPOSITIONWINDOW)                   (void*, MS_U8, SCALER_WIN, MS_WINDOW_TYPE *);
typedef void                             (*IOCTL_XC_SETFRAMEBUFFERADDRESS)                   (void*, MS_PHY, MS_PHY, SCALER_WIN);
typedef void                             (*IOCTL_XC_SETFRCMFRAMEBUFFERADDRESS)               (void*, MS_PHY, MS_PHY, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_SETDUALFRAMEBUFFERADDRESS)                   (void*, MS_PHY, MS_PHY, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_ENABLEMIUDUALMODE)                       (void *, MS_BOOL, MS_U32, MS_U32, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_ISFRAMEBUFFERENOUGHFORCUSSCALING)        (void*, XC_SETWIN_INFO *, SCALER_WIN);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SETSCALERMEMORYREQUEST)                  (void*, MS_BOOL, E_XC_MEMORY_REQUEST_TYPE, SCALER_WIN);
typedef void                             (*IOCTL_XC_GET_PIXELDATA)                           (void*, MS_U16,  MS_U16, MS_PIXEL_32BIT*);
typedef MS_U32                           (*IOCTL_XC_GETAVAILABLESIZE)                        (void*, SCALER_WIN, MS_U8, MS_U32);
typedef void                             (*IOCTL_XC_SETFRAMECOLOR)                           (void*, MS_U32);
typedef void                             (*IOCTL_XC_SETDISPWINDOWCOLOR)                      (void*, MS_U8, SCALER_WIN);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SUPPORTSOURCETOVE)                       (void*, MS_U16*);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SETOUTPUTCAPTURE)                        (void*, MS_BOOL,E_XC_SOURCE_TO_VE);
typedef void                             (*IOCTL_XC_SETGAMMAONOFF)                           (void*, MS_BOOL);
typedef void                             (*IOCTL_XC_SETPREGAMMAGAIN)                         (void*, SCALER_WIN, MS_XC_VOP_CHANNEL_t, MS_U16);
typedef void                             (*IOCTL_XC_SETPREGAMMAOFFSET)                       (void*, SCALER_WIN, MS_XC_VOP_CHANNEL_t, MS_U16);
typedef void                             (*IOCTL_XC_SETPANELTIMING)                          (void*, XC_SetTiming_Info *, SCALER_WIN);
typedef void                             (*IOCTL_XC_SETOUTTIMINGMODE)                        (void*, E_XC_PNL_OUT_TIMING_MODE);
typedef void                             (*IOCTL_XC_SETFREERUNTIMING)                        (void*);
typedef MS_BOOL                          (*IOCTL_XC_SET_CUSTOMERSYNCINFO)                    (void*, MS_U16, MS_U16, MS_U16);
typedef MS_BOOL                          (*IOCTL_XC_WAITFPLLDONE)                            (void*);
typedef MS_U16                           (*IOCTL_XC_GETOUTPUTVFREQX100)                     (void*);
typedef MS_U16                           (*IOCTL_XC_GETOP1OUTPUTVFREQX100)                  (void*);
typedef MS_BOOL                          (*IOCTL_XC_FRAMELOCKCHECK)                          (void*);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_CUSTOMIZEFRCTABLE)                       (void*, SC_FRC_SETTING*);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_OUTPUTFRAMECTRL)                         (void*, MS_BOOL,XC_OUTPUTFRAME_Info *,SCALER_WIN);
typedef void                             (*IOCTL_SC_ENABLE_FPLL_FSM)                         (void*, MS_BOOL);
typedef void                             (*IOCTL_SC_FORCEFREERUN)                            (void*, MS_BOOL);
typedef MS_BOOL                          (*IOCTL_SC_ISFORCEFREERUN)                          (void*);
typedef void                             (*IOCTL_SC_SETFREERUNVFREQ)                         (void*, E_VFREQ_SEL);
typedef MS_BOOL                          (*IOCTL_XC_SETEXPANELINFO)                          (void*, MS_BOOL, XC_PANEL_INFO_EX *);
typedef void                             (*IOCTL_XC_ENABLE_FPLL_THRESH_MODE)                 (void*, MS_BOOL);
typedef MS_BOOL                          (*IOCTL_XC_GET_FPLL_THRESH_MODE)                    (void*);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_ENABLEIPAUTONOSIGNAL)                    (void*, MS_BOOL, SCALER_WIN );
typedef MS_BOOL                          (*IOCTL_XC_GETIPAUTONOSIGNAL)                       (void*, SCALER_WIN );
typedef void                             (*IOCTL_XC_ENABLEIPAUTOCOAST)                       (void*, MS_BOOL);
typedef void                             (*IOCTL_XC_ENABLEIPCOASTDEBOUNCE)                   (void*);
typedef void                             (*IOCTL_XC_CLEARIPCOASTSTATUS)                      (void*);
typedef void                             (*IOCTL_XC_ENABLEFPLLMANUALSETTING)                 (void*, MS_BOOL);
typedef void                             (*IOCTL_XC_FPLLBOUNDARYTEST)                        (void*, MS_U32, MS_U16);
typedef void                             (*IOCTL_XC_SETOFFLINEDETECTION)                     (void*, INPUT_SOURCE_TYPE_t);
typedef MS_U8                            (*IOCTL_XC_GETOFFLINEDETECTION)                     (void*, INPUT_SOURCE_TYPE_t);
typedef MS_BOOL                          (*IOCTL_XC_SETOFFLINESOGTHRESHOLD)                  (void*, MS_U8);
typedef MS_BOOL                          (*IOCTL_XC_SETOFFLINESOGBW)                         (void*, MS_U8);
typedef MS_BOOL                          (*IOCTL_XC_OFFLINEINIT)                             (void*);
typedef void                             (*IOCTL_XC_SET_EXTRA_FETCH_ADV_LINE)                (void*, MS_U8);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SETVGASOGEN)                             (void*, MS_BOOL);
typedef void                             (*IOCTL_XC_ENABLEWINDOW)                            (void*, MS_BOOL, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_IS_SUBWINDOWEANBLE)                      (void*);
typedef void                             (*IOCTL_XC_SETBORDERFORMAT)                         (void*, MS_U8, MS_U8, MS_U8, MS_U8, MS_U8, SCALER_WIN);
typedef void                             (*IOCTL_XC_ENABLEBORDER)                            (void*, MS_BOOL, SCALER_WIN);
typedef void                             (*IOCTL_XC_ZORDERMAINWINDOWFIRST)                   (void*, MS_BOOL);
typedef void                             (*IOCTL_XC_PQ_LOADFUNCTION)                         (void*, PQ_Function_Info* , MS_U32);
typedef MS_BOOL                          (*IOCTL_XC_CHECK_HNONLINEARSCALING)                 (void*);
typedef void                             (*IOCTL_XC_ENABLEEUROHDTVSUPPORT)                   (void*, MS_BOOL);
typedef void                             (*IOCTL_XC_ENABLEEUROHDTVDETECTION)                 (void*, MS_BOOL, SCALER_WIN);
typedef MS_U8                            (*IOCTL_XC_READBYTE)                                (void*, MS_U32);
typedef void                             (*IOCTL_XC_WRITEBYTE)                               (void*, MS_U32, MS_U8);
typedef void                             (*IOCTL_XC_WRITEBYTEMASK)                           (void*, MS_U32, MS_U8, MS_U8);
typedef void                             (*IOCTL_XC_WRITE2BYTEMASK)                          (void*, MS_U32, MS_U16, MS_U16);
typedef void                             (*IOCTL_XC_W2BYTE)                                  (void*, MS_U32, MS_U16);
typedef MS_U16                           (*IOCTL_XC_R2BYTE)                                  (void*, MS_U32);
typedef void                             (*IOCTL_XC_W4BYTE)                                  (void*, MS_U32, MS_U32);
typedef MS_U32                           (*IOCTL_XC_R4BYTE)                                  (void*, MS_U32);
typedef MS_U16                           (*IOCTL_XC_R2BYTEMSK)                               (void*, MS_U32, MS_U16);
typedef void                             (*IOCTL_XC_W2BYTEMSK)                               (void*, MS_U32, MS_U16, MS_U16);
typedef void                             (*IOCTL_XC_MLOAD_INIT)                              (void*, MS_PHY, MS_U32);
typedef void                             (*IOCTL_XC_MLOAD_ENABLE)                            (void*, MS_BOOL);
typedef void                             (*IOCTL_XC_MLOAD_CUS_INIT)                             (void*, EN_MLOAD_CLIENT_SELECT, MS_PHY, MS_U32);
typedef void                             (*IOCTL_XC_MLOAD_CUS_ENABLE)                           (void*, EN_MLOAD_CLIENT_SELECT, MS_BOOL);
typedef MLOAD_TYPE                       (*IOCTL_XC_MLOAD_GETSTATUS)                         (void*);
typedef MS_BOOL                          (*IOCTL_XC_MLOAD_WRITECMD_AND_FIRE)                 (void*, MS_U32, MS_U16, MS_U16);
typedef MS_BOOL                          (*IOCTL_XC_MLOAD_WRITECMDS_AND_FIRE)                (void*, MS_U32 *, MS_U16 *, MS_U16 *, MS_U16);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_MLOAD_COMBINE)                           (void*, MS_BOOL);
typedef void                             (*IOCTL_XC_MLG_INIT)                                (void*, MS_PHY, MS_U32);
typedef void                             (*IOCTL_XC_MLG_ENABLE)                              (void*, MS_BOOL);
typedef MS_BOOL                          (*IOCTL_XC_MLG_GETCAPS)                             (void*);
typedef MLG_TYPE                         (*IOCTL_XC_MLG_GETSTATUS)                           (void*);
typedef void                             (*IOCTL_XC_SETOSD2VEMODE)                           (void*, EN_VOP_SEL_OSD_XC2VE_MUX );
typedef void                             (*IOCTL_XC_IP2_PREFILTER_ENABLE)                    (void*, MS_BOOL);
typedef E_APIXC_GET_PixelRGB_ReturnValue (*IOCTL_XC_GET_PIXEL_RGB)                           (void*, XC_Get_Pixel_RGB *, SCALER_WIN);
typedef void                             (*IOCTL_XC_KEEPPIXELPOINTERAPPEAR)                  (void*, MS_BOOL);
typedef MS_BOOL                          (*IOCTL_XC_SET_MEMFMTEX)                            (void*, MS_XC_MEM_FMT, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_ISREQUESTFRAMEBUFFERLESSMODE)            (void*);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SKIPSWRESET)                             (void*, MS_BOOL);
typedef void                             (*IOCTL_XC_ENABLEREPWINDOWFORFRAMECOLOR)            (void*, MS_BOOL);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SETOSDLAYER)                             (void*, E_VOP_OSD_LAYER_SEL , SCALER_WIN);
typedef E_VOP_OSD_LAYER_SEL              (*IOCTL_XC_GETOSDLAYER)                             (void*, SCALER_WIN);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SETVIDEOALPHA)                           (void*, MS_U8, SCALER_WIN);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_GETVIDEOALPHA)                           (void*, MS_U8 *, SCALER_WIN);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SKIPWAITVSYNC)                           (void*, MS_BOOL,MS_BOOL);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SETCMAHEAPID)                            (void*, MS_U8,SCALER_WIN);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_OP2VOPDESEL)                             (void*, E_OP2VOP_DE_SEL);
typedef void                             (*IOCTL_XC_FRC_SETWINDOW)                           (void*, E_XC_3D_INPUT_MODE, E_XC_3D_OUTPUT_MODE, E_XC_3D_PANEL_TYPE);
typedef MS_BOOL                          (*IOCTL_XC_ENABLE_TWOINITFACTOR)                    (void*, MS_BOOL, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_ISFIELDPACKINGMODESUPPORTED)             (void*);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_PREINIT)                                 (void*, E_XC_PREINIT_t ,void*, MS_U32);
typedef void                             (*IOCTL_XC_GET_BUFFERDATA)                          (void*, E_XC_OUTPUTDATA_TYPE, MS_WINDOW_TYPE *, void *, SCALER_WIN);
typedef void                             (*IOCTL_XC_SET_BUFFERDATA)                          (void*, E_XC_INPUTDATA_TYPE, MS_WINDOW_TYPE *, void *, MS_WINDOW_TYPE *, SCALER_WIN);
typedef void                             (*IOCTL_XC_ENABLEMAINWINDOW)                        (void*, MS_BOOL);
typedef void                             (*IOCTL_XC_ENABLESUBWINDOW)                         (void*, MS_WINDOW_TYPE *);
typedef void                             (*IOCTL_XC_DISABLESUBWINDOW)                        (void*);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SETPIXELSHIFT)                           (void*, MS_S8, MS_S8);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SETVIDEOONOSD)                           (void*, E_VIDEO_ON_OSD_LAYER, SCALER_WIN);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SETOSDLAYERBLENDING)                     (void*, MS_U8, MS_BOOL, SCALER_WIN);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SETOSDLAYERALPHA)                        (void*, MS_U8, MS_U8);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SETOSDBLENDINGFORMULA)                   (void*, E_XC_OSD_INDEX, E_XC_OSD_BlENDING_TYPE, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_REPORTPIXELINFO)                         (void*, MS_XC_REPORT_PIXELINFO *);
typedef void                             (*IOCTL_XC_SETSCALING)                              (void*, MS_BOOL, E_XC_SCALING_TYPE, E_XC_VECTOR_TYPE, SCALER_WIN);
typedef void                             (*IOCTL_XC_SETMCDIBUFFERADDRESS)                    (void*, MS_PHY, MS_PHY, E_XC_MCDI_TYPE);
typedef void                             (*IOCTL_XC_ENABLEMCDI)                              (void*, MS_BOOL, E_XC_MCDI_TYPE);
typedef MS_BOOL                          (*IOCTL_XC_SENDCMDTOFRC)                            (void*, MS_U8, MS_U8, FRC_R2_CMD_PARAMETER_t);
typedef MS_BOOL                          (*IOCTL_XC_GETMSGFROMFRC)                           (void*, MS_U8*, MS_U8*, MS_U8*);
typedef void                             (*IOCTL_XC_ENABLERWBANKAUTO)                        (void*, MS_BOOL, SCALER_WIN);
typedef void                             (*IOCTL_XC_SETWRBANKMAPPINGNUM)                     (void*, MS_U8, SCALER_WIN);
typedef MS_U8                            (*IOCTL_XC_GETWRBANKMAPPINGNUM)                     (void*, SCALER_WIN);
typedef MS_U8                            (*IOCTL_XC_GETWRBANKMAPPINGNUMFORZAP)               (void*, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_SET_BOBMODE)                             (void*, MS_BOOL, SCALER_WIN);
typedef void                             (*IOCTL_XC_SET_FORCE_READBANK)                      (void*, MS_BOOL, MS_U8, SCALER_WIN);
typedef void                             (*IOCTL_XC_SET_LOCK_POINT)                      (void*, MS_U16, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_LD_INIT)                                 (void*, EN_LD_PANEL_TYPE);
typedef MS_BOOL                          (*IOCTL_XC_LD_SETMEMORYADDRESS)                     (void*, MS_U8,MS_PHY,MS_PHY,MS_PHY,MS_PHY,MS_PHY,MS_PHY);
typedef MS_BOOL                          (*IOCTL_XC_LD_GET_VALUE)                            (void*, MS_U8*, MS_U8, MS_U8);
typedef MS_BOOL                          (*IOCTL_XC_LD_SETLEVEL)                             (void*, EN_LD_MODE);
typedef MS_BOOL                          (*IOCTL_SET_TURNOFFLDBL)                            (void*, MS_BOOL);
typedef MS_BOOL                          (*IOCTL_SET_NOTUPDATESPIDATAFLAGS)                  (void*, MS_BOOL);
typedef MS_BOOL                          (*IOCTL_SET_USERMODELDFLAGS)                        (void*, MS_BOOL);
typedef MS_BOOL                          (*IOCTL_SET_BLLEVEL)                                (void*, MS_U8);
typedef void                             (*IOCTL_XC_SET_BWS_MODE)                            (void*, MS_BOOL, SCALER_WIN);
typedef void                             (*IOCTL_XC_FRC_COLORPATHCTRL)                       (void*, MS_U16, MS_BOOL);
typedef void                             (*IOCTL_XC_FRC_OP2_SETRGBGAIN)                      (void*, MS_U16, MS_U16, MS_U16);
typedef void                             (*IOCTL_XC_FRC_OP2_SETRGBOFFSET)                    (void*, MS_U16, MS_U16, MS_U16);
typedef void                             (*IOCTL_XC_FRC_OP2_SETDITHER)                       (void*, MS_U16);
typedef MS_BOOL                          (*IOCTL_XC_FRC_BYPASSMFC)                           (void*, MS_BOOL);
typedef void                             (*IOCTL_XC_FRC_MUTE)                                (void*, MS_BOOL);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_FORCEREADFRAME)                          (void*, MS_BOOL, MS_U16);
typedef void                             (*IOCTL_XC_SETCSC)                                  (void*, MS_BOOL, SCALER_WIN);
typedef void                             (*IOCTL_XC_REGISTERPQSETFPLLTHRESHMODE)             (void*, void (*fpPQCB)(MS_U8));
typedef MS_BOOL                          (*IOCTL_XC_GETFREERUNSTATUS)                        (void*);
typedef void                             (*IOCTL_XC_SET_BYPASS_CSC)                          (void*, MS_BOOL);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_GET_DSFORCEINDEXSUPPORTED)               (void*, SCALER_WIN);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SET_DSFORCEINDEX)                        (void*, MS_BOOL, MS_U8, SCALER_WIN);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SET_DSINDEXSOURCESELECT)                 (void*, E_XC_DS_INDEX_SOURCE, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_GET_DYNAMICSCALINGSTATUS)                (void*);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_OSDC_INITSETTING)                        (void*, E_XC_OSDC_TGEN_Type,MS_XC_OSDC_TGEN_INFO*,MS_XC_OSDC_CTRL_INFO*);
typedef void                             (*IOCTL_XC_OSDC_SET_OUTPUTVFREQX10)                 (void*, MS_U16);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_OSDC_CONTROL)                            (void*, MS_U32);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_OSDC_GETDSTINFO)                         (void*, MS_OSDC_DST_DispInfo*, MS_U32);
typedef MS_U32                           (*IOCTL_XC_SET_POWERSTATE)                          (void*, EN_POWER_MODE);
typedef void                             (*IOCTL_XC_SET_BYPASS_OSDVSYNC_POS)                 (void*, MS_U16, MS_U16);
typedef void                             (*IOCTL_XC_SET_BYPASS_INPUTSRC)                     (void*, MS_BOOL, E_XC_BYPASS_InputSource);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SET_SEAMELESS_ZAPPING)                   (void*, SCALER_WIN, MS_BOOL);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_GET_SEAMELESS_ZAPPING_STATUS)            (void*, SCALER_WIN, MS_BOOL*);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SET_VTRACK_PAYLOADDATA)                  (void*, MS_U16, MS_U8);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SET_VTRACK_USERDEFINED_SETTING)          (void*, MS_BOOL, MS_U8*);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SET_VTRACK_ENABLE)                       (void*, MS_U8, MS_BOOL);
typedef void                             (*IOCTL_XC_PRESET_PQINFO)                           (void*, XC_SETWIN_INFO*, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_GET_OP1_TESTPATTERN_ENABLED)             (void*);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SET_OP1_TESTPATTERN)                     (void*, EN_XC_OP1_PATTERN, EN_XC_OP1_PATGEN_MODE);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SET_OP2_TESTPATTERN)                     (void*, MS_BOOL, MS_U16, MS_U16, MS_U16);
typedef MS_BOOL                          (*IOCTL_XC_CHECK_WHITEBALANCE_PATTERN_MODE)         (void*, EN_XC_WB_PATTERN_MODE);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SET_HLINEARSCALING)                      (void*, MS_BOOL, MS_BOOL, MS_U16, SCALER_WIN);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_ENABLE_T3D)                              (void*, MS_BOOL);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SET_FRC_INPUTTIMING)                     (void*, E_XC_FRC_InputTiming);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_GET_FRC_INPUTTIMING)                     (void*, E_XC_FRC_InputTiming*);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_GET_VBOX_INFO)                           (void*, XC_VBOX_INFO*);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SET_OSD_DETECT)                          (void*, MS_BOOL, MS_U32);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_GET_OSD_DETECT)                          (void*, MS_BOOL*);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_ENABLE_LOCKFREQONLY)                    (void*, MS_BOOL);
typedef MS_BOOL                          (*IOCTL_XC_IS2K2KTOFRCMODE)                         (void*);
typedef MS_BOOL                          (*IOCTL_XC_SET_XC_VOP_DATA)                          (void*, MS_XC_VOP_Data);
typedef MS_U8                            (*IOCTL_XC_GETSWDSINDEX)                            (void*);
typedef MS_BOOL                          (*IOCTL_XC_GET_Is_SupportSWDS)                      (void*);
typedef MS_U8                            (*IOCTL_XC_GET_FRCM_FRAMENUM)                       (void*, SCALER_WIN);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SET_PIXELSHIFT_FEATURES)                 (void*, MS_U16, MS_U16, PIXEL_SHIFT_FEATURE);
typedef MS_BOOL                          (*IOCTL_XC_SET_BWD_CONFIG)                                 (void*, E_XC_BWD_CONFIG_TYPE, void *, SCALER_WIN);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SETFORCEWRITE)                           (void*, EN_MUTE_TYPE,MS_BOOL);
typedef MS_BOOL                          (*IOCTL_XC_GETFORCEWRITE)                           (void*, EN_MUTE_TYPE);
#ifdef UFO_XC_AUTO_DOWNLOAD
typedef E_APIXC_ReturnValue              (*IOCTL_XC_AUTODOWNLOAD_CONFIG_CTRL)                   (void*, XC_AUTODOWNLOAD_CONFIG_INFO*);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_AUTODOWNLOAD_WRITE_CTRL)                     (void*, XC_AUTODOWNLOAD_DATA_INFO*);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_AUTODOWNLOAD_FIRE_CTRL)                     (void*, EN_XC_AUTODOWNLOAD_CLIENT);
#endif
#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
typedef E_APIXC_ReturnValue              (*IOCTL_XC_HDR_CTRL)                                    (void*, EN_XC_HDR_CTRL_TYPE, void *);
#endif
#endif
typedef MS_BOOL                          (*IOCTL_XC_OPTEE_CTRL)                              (void *,EN_XC_OPTEE_ACTION ,XC_OPTEE_HANDLER*);
typedef MS_BOOL                          (*IOCTL_XC_OPTEE_CHECKRBASE)                        (void *,SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_OPTEE_MUX)                               (void *,EN_XC_OPTEE_ACTION ,XC_OPTEE_MUX_DATA);
typedef MS_U32                           (*IOCTL_XC_GET_RES_BY_PIPE)                         (void* ,MS_U32 , MS_U32* , RESOURCE*);
typedef MS_U32                           (*IOCTL_XC_CONFIG_PIPE)                             (void* ,MS_U32 , MS_U32 , MS_U32);
typedef MS_U32                           (*IOCTL_XC_CHECK_PIPE)                              (void* ,MS_U32 , MS_U32 , MS_U32);
// OBSOLETE FUNCTION POINTER START
typedef MS_U16                           (*IOCTL_SC_GET_OUTPUT_VFREQ)                        (void*);
typedef MS_U32                           (*IOCTL_XC_GET_CURRENT_OUTPUTVFREQX100)             (void*);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SETMEMORYWRITEREQUEST)                   (void*, MS_BOOL);
typedef MS_BOOL                          (*IOCTL_XC_SET_MEMFMT)                              (void*, MS_XC_MEM_FMT);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SETOUTPUTADJUSTSETTING)                  (void*, XC_OUTPUT_TIMING_ADJUST_SETTING*);
typedef void                             (*IOCTL_SCALER_FPLL_FSM)                            (void*, SCALER_WIN);
typedef void                             (*IOCTL_XC_ENABLEMIRRORMODE)                        (void*, MS_BOOL);
typedef void                             (*IOCTL_XC_ENABLEMIRRORMODE2)                       (void*, MirrorMode_t);
typedef MirrorMode_t                     (*IOCTL_XC_GETMIRRORMODETYPE)                       (void*);
// OBSOLETE FUNCTION POINTER END
typedef MS_BOOL                          (*IOCTL_XC_CMD_IS_SUPPORT_2_STEP_SCALING)           (void*);
typedef MS_BOOL                          (*IOCTL_XC_CMD_GET_PQ_PATH_STATUS)                  (void*, E_XC_PQ_Path_Type, MS_U16 , MS_U16);
#ifdef UFO_XC_SET_DSINFO_V0
typedef void                             (*IOCTL_XC_SETDSINFO)                               (void*,XC_DS_INFO*, MS_U32, SCALER_WIN);
#endif
typedef E_APIXC_ReturnValue              (*IOCTL_XC_VIDEO_PLAYBACK_CTRL)                     (void*, MS_U32, void*,MS_U32);
#ifdef UFO_XC_SETBLACKVIDEOBYMODE
typedef E_APIXC_ReturnValue              (*IOCTL_XC_GENERATEBLACKVIDEOBYMODE)              (void*, MS_BOOL, SCALER_WIN, EN_XC_BLACK_VIDEO_MODE);
#endif

#ifdef UFO_XC_FB_LEVEL
typedef E_APIXC_ReturnValue              (*IOCTL_XC_SET_FB_LEVEL)                            (void*, E_XC_FB_LEVEL, SCALER_WIN);
typedef E_XC_FB_LEVEL                    (*IOCTL_XC_GET_FB_LEVEL)                            (void*, SCALER_WIN);
#endif
typedef MS_BOOL                          (*IOCTL_XC_SET_SWDR_INFO)                          (void*, XC_SWDR_INFO*);
typedef MS_BOOL                          (*IOCTL_XC_GET_SWDR_INFO)                          (void*, XC_SWDR_INFO*);


// APIXC_ADC_H
typedef void                             (*IOCTL_XC_ADC_SETCVBSOUT)                         (void*, MS_BOOL, E_DEST_TYPE , INPUT_SOURCE_TYPE_t, MS_BOOL);
typedef MS_BOOL                          (*IOCTL_XC_ADC_ISCVBSOUTENABLED)                   (void*, E_DEST_TYPE);
typedef void                             (*IOCTL_XC_ADC_SETPCCLOCK)                         (void*, MS_U16);
typedef void                             (*IOCTL_XC_ADC_SETPHASE)                           (void*, MS_U8);
typedef void                             (*IOCTL_XC_ADC_SETPHASEEX)                         (void*, MS_U16);
typedef MS_U16                           (*IOCTL_XC_ADC_GETPHASERANGE)                      (void*);
typedef MS_U8                            (*IOCTL_XC_ADC_GETPHASE)                           (void*);
typedef MS_U16                           (*IOCTL_XC_ADC_GETPHASEEX)                         (void*);
typedef MS_BOOL                          (*IOCTL_XC_ADC_ISSCARTRGB)                         (void*);
typedef MS_U16                           (*IOCTL_XC_ADC_GETPCCLOCK)                         (void*);
typedef void                             (*IOCTL_XC_ADC_GETSOGLEVELRANGE)                   (void*, MS_U32*, MS_U32*, MS_U32*);
typedef void                             (*IOCTL_XC_ADC_SETSOGLEVEL)                        (void*, MS_U32);
typedef void                             (*IOCTL_XC_ADC_POWEROFF)                           (void*);
typedef void                             (*IOCTL_XC_ADC_GETDEFAULTGAINOFFSET)               (void*, INPUT_SOURCE_TYPE_t, APIXC_AdcGainOffsetSetting*);
typedef MS_U16                           (*IOCTL_XC_ADC_GETMAXIMALOFFSETVALUE)              (void*);
typedef MS_U16                           (*IOCTL_XC_ADC_GETMAXIMALGAINVALUE)                (void*);
typedef MS_U16                           (*IOCTL_XC_ADC_GETCENTERGAIN)                      (void*);
typedef MS_U16                           (*IOCTL_XC_ADC_GETCENTEROFFSET)                    (void*);
typedef void                             (*IOCTL_XC_ADC_SETGAIN)                            (void*, MS_U8, MS_U16);
typedef void                             (*IOCTL_XC_ADC_SETOFFSET)                          (void*, MS_U8, MS_U16);
typedef void                             (*IOCTL_XC_ADC_ADJUSTGAINOFFSET)                   (void*, APIXC_AdcGainOffsetSetting*);
typedef void                             (*IOCTL_XC_ADC_SOURCE_CALIBRATE)                   (void*, INPUT_SOURCE_TYPE_t);
typedef void                             (*IOCTL_XC_ADC_SETSOGCAL)                          (void*);
typedef void                             (*IOCTL_XC_ADC_SETRGB_PIPE_DELAY)                  (void*, MS_U8);
typedef void                             (*IOCTL_XC_ADC_SCARTRGB_SOG_CLAMPDELAY)            (void*, MS_U16, MS_U16);
typedef void                             (*IOCTL_XC_ADC_SET_YPBPRLOOSELPF)                  (void*, MS_BOOL);
typedef void                             (*IOCTL_XC_ADC_SET_SOGBW)                          (void*, MS_U16);
typedef void                             (*IOCTL_XC_ADC_SETCLAMPDURATION)                   (void*, MS_U16);
typedef MS_BOOL                          (*IOCTL_XC_ADC_ENABLEHWCALIBRATION)                (void*, MS_BOOL);
typedef MS_BOOL                          (*IOCTL_XC_ADC_SETIDACCURRENTMODE)                 (void*, ADC_DacCurrentMode_t);
typedef ADC_DacCurrentMode_t             (*IOCTL_XC_ADC_GETIDACCURRENTMODE)                 (void*);

// APIXC_AUTO_H
typedef MS_BOOL                          (*IOCTL_XC_AUTO_GEOMETRY)                          (void*, XC_Auto_TuneType, XC_Auto_Signal_Info*, XC_Auto_Signal_Info*, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_AUTO_GEOMETRY_EX)                       (void*, XC_Auto_TuneType, XC_Auto_Signal_Info_Ex*, XC_Auto_Signal_Info_Ex*, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_AUTO_STOPAUTOGEOMETRY)                  (void*);
typedef MS_BOOL                          (*IOCTL_XC_AUTO_GAINOFFSET)                        (void*, XC_Auto_CalibrationType, XC_Auto_TuneType, APIXC_AdcGainOffsetSetting*, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_AUTO_GETHWFIXEDGAINOFFSET)              (void*, XC_Auto_TuneType, APIXC_AdcGainOffsetSetting*);
typedef MS_BOOL                          (*IOCTL_XC_AUTO_SETVALIDDATA)                      (void*, MS_U8);
typedef void                             (*IOCTL_XC_AUTO_AUTOOFFSET)                        (void*, MS_BOOL, MS_BOOL);
typedef MS_BOOL                          (*IOCTL_XC_AUTO_DETECTWIDTH)                       (void*, XC_Auto_Signal_Info_Ex*, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_AUTO_SETCALIBRATIONMODE)                (void*, XC_Auto_CalibrationMode);
typedef void                             (*IOCTL_XC_AUTO_GETCALIBRATIONMODE)            (void*, XC_Auto_CalibrationMode*);
typedef void                             (*IOCTL_XC_AUTO_GETSYNCINFO)                       (void*, XC_AUTO_SYNC_INFO*);

// APIXC_MODEPARSE_H
typedef MS_BOOL                          (*IOCTL_XC_MODEPARSE_INIT)                         (void*);
typedef XC_MODEPARSE_RESULT              (*IOCTL_XC_MODEPARSE_MATCHMODE)                    (void*, MS_PCADC_MODETABLE_TYPE*, MS_U8, XC_MODEPARSE_INPUT_INFO*);
typedef XC_MODEPARSE_RESULT              (*IOCTL_XC_MODEPARSE_MATCHMODEEX)                  (void*, MS_PCADC_MODETABLE_TYPE_EX*, MS_U8, XC_MODEPARSE_INPUT_INFO*);

// APIXC_PCMONITOR_H
typedef MS_BOOL                          (*IOCTL_XC_PCMONITOR_INIT)                         (void*, MS_U8);
typedef void                             (*IOCTL_XC_PCMONITOR_RESTART)                      (void*, SCALER_WIN);
typedef void                             (*IOCTL_XC_PCMONITOR_SETTIMINGCOUNT)               (void*, MS_U16, MS_U16);
typedef XC_PCMONITOR_STATUS              (*IOCTL_XC_PCMONITOR)                              (void*, INPUT_SOURCE_TYPE_t, SCALER_WIN);
typedef XC_PCMONITOR_STATUS              (*IOCTL_XC_PCMONITOR_GETCURRENTSTATE)              (void*, SCALER_WIN);
typedef MS_U8                            (*IOCTL_XC_PCMONITOR_GETSYNCSTATUS)                (void*, SCALER_WIN);
typedef MS_U16                           (*IOCTL_XC_PCMONITOR_GET_HFREQX10)                 (void*, SCALER_WIN);
typedef MS_U32                           (*IOCTL_XC_PCMONITOR_GET_HFREQX1K)                 (void*, SCALER_WIN);
typedef MS_U16                           (*IOCTL_XC_PCMONITOR_GET_VFREQX10)                 (void*, SCALER_WIN);
typedef MS_U32                           (*IOCTL_XC_PCMONITOR_GET_VFREQX1K)                 (void*, SCALER_WIN);
typedef MS_U16                           (*IOCTL_XC_PCMONITOR_GET_VTOTAL)                   (void*, SCALER_WIN);
typedef void                             (*IOCTL_XC_PCMONITOR_GET_DVI_HDMI_DE_INFO)         (void*, SCALER_WIN, MS_WINDOW_TYPE*);
typedef MS_BOOL                          (*IOCTL_XC_PCMONITOR_SYNCLOSS)                     (void*, SCALER_WIN);
typedef MS_BOOL                          (*IOCTL_XC_PCMONITOR_INVALIDTIMINGDETECT)          (void*, MS_BOOL, SCALER_WIN);
typedef E_APIXC_ReturnValue              (*IOCTL_XC_PCMONITOR_SETTIMINGCOUNTEX)             (void*, INPUT_SOURCE_TYPE_t, MS_U16, MS_U16);


// DRVXC_HDMI_IF_H
typedef MS_U16                           (*IOCTL_HDMI_FUNC_CAPS)                            (void*);
typedef stHDMI_INITIAL_TABLE             (*IOCTL_HDMI_INIT)                                 (void*);
typedef void                             (*IOCTL_HDMI_EXIT)                                 (void*);
typedef void                             (*IOCTL_HDMI_PKT_RESET)                            (void*, HDMI_REST_t);
typedef void                             (*IOCTL_HDMI_PULLHPD)                              (void*, MS_BOOL, E_MUX_INPUTPORT, MS_BOOL);
typedef MS_U16                           (*IOCTL_HDMI_GC_INFO)                              (void*, MS_U8, HDMI_GControl_INFO_t);
typedef void                             (*IOCTL_HDMI_PACKET_RECEIVED)                      (void*, MS_U8, MS_BOOL, MS_U32);
typedef MS_HDMI_COLOR_FORMAT             (*IOCTL_HDMI_GET_COLORFORMAT)                      (void*, MS_U8);
typedef EN_HDMI_COLOR_RANGE              (*IOCTL_HDMI_GET_COLORRANGE)                       (void*, MS_U8);
typedef void                             (*IOCTL_HDMI_SET_EQ)                               (void*, MS_HDMI_EQ, MS_U8);
typedef MS_BOOL                          (*IOCTL_HDMI_SET_EQ_TO_PORT)                       (void*, MS_HDMI_EQ, MS_U8, E_MUX_INPUTPORT);
typedef void                             (*IOCTL_HDMI_AUDIO_MUTE_ENABLE)                    (void*, MS_U16, MS_U16);
typedef void                             (*IOCTL_HDMI_AUDIO_STATUS_CLEAR)                   (void*);
typedef MS_HDMI_AR_TYPE                  (*IOCTL_HDMI_GET_ASPECTRATIO)                      (void*, MS_U8, MS_BOOL, MS_BOOL);
typedef MS_HDMI_AR_TYPE                  (*IOCTL_HDMI_GET_ACTIVEFORMAT_ASPECTRATIO)         (void*, MS_U8, MS_BOOL);
typedef EN_AVI_INFOFRAME_VERSION         (*IOCTL_HDMI_GET_AVIINFOFRAMEVER)                  (void*, MS_U8);
typedef MS_U8                            (*IOCTL_HDMI_ERR_STATUS_UPDATE)                    (void*, MS_U8, MS_U8, MS_BOOL);
typedef MS_U16                           (*IOCTL_HDMI_GET_TMDS_FREQ)                        (void*);
typedef void                             (*IOCTL_HDMI_GET_POLLINGSTATUS)                    (void*, HDMI_POLLING_STATUS_t**);
typedef MS_BOOL                          (*IOCTL_HDMI_ISHDMI_MODE)                          (void*);
typedef void                             (*IOCTL_HDMI_GET_MID_INFO)                         (void*, MS_U8, MS_U8*);
typedef MS_BOOL                          (*IOCTL_HDMI_GET_PACKET_VALUE)                     (void*, MS_U8, MS_HDMI_PACKET_STATE_t, MS_U8, MS_U8*);
typedef MS_U16                           (*IOCTL_DVI_CHANNELPHASESTATUS)                    (void*,  MS_DVI_CHANNEL_TYPE);
typedef void                             (*IOCTL_DVI_SOFTWARERESET)                         (void*,  MS_U16);
typedef void                             (*IOCTL_DVI_RESET)                                 (void*);
typedef MS_BOOL                          (*IOCTL_HDMI_AVIINFO_ACTIVEINFOPRESENT)            (void*, MS_U8);
typedef void                             (*IOCTL_DVI_CLKPULLLOW)                            (void*, MS_BOOL, E_MUX_INPUTPORT);
typedef void                             (*IOCTL_DVI_SWITCHSRC)                             (void*, E_MUX_INPUTPORT, MS_U8);
typedef MS_BOOL                          (*IOCTL_HDMI_SETFORCLOCKLESSTHAN70MHZ)             (void*, E_MUX_INPUTPORT);
typedef void                             (*IOCTL_HDMI_DVI_ADJUST)                           (void*, MS_BOOL);
typedef void                             (*IOCTL_HDMI_SETUPDATEPHASELINECOUNT)              (void*, INPUT_SOURCE_TYPE_t);
typedef void                             (*IOCTL_HDCP_ENABLE)                               (void*, MS_BOOL);
typedef MS_BOOL                          (*IOCTL_HDMI_SETHDCPENABLE)                        (void*, E_MUX_INPUTPORT,MS_BOOL);
typedef void                             (*IOCTL_HDCP_CLEARSTATUS)                          (void*);
typedef void                             (*IOCTL_HDCP_INITPRODUCTIONKEY)                    (void*, MS_U8*);
typedef MS_U16                           (*IOCTL_HDCP_GETSTATUS)                            (void*);
typedef void                             (*IOCTL_HDCP_VSYNC_END_EN)                         (void*, MS_BOOL);
typedef void                             (*IOCTL_HDMI_AUDIO_OUTPUT)                         (void*, MS_BOOL);
typedef MS_U8                            (*IOCTL_HDMI_AUDIO_CP_HDR_INFO)                    (void*, MS_U8);
typedef MS_U8                            (*IOCTL_HDMI_AUDIO_CHANNEL_STATUS)                 (void*, MS_U8, MS_U8);
typedef HDMI_Ret_Value                   (*IOCTL_HDMI_GETLIBVER)                            (void*, const MSIF_Version**);
typedef HDMI_Ret_Value                   (*IOCTL_HDMI_GETINFO)                              (void*, MS_HDMI_INFO*);
typedef HDMI_Ret_Value                   (*IOCTL_HDMI_GETSTATUS)                            (void*, MS_HDMI_Status*);
typedef void                             (*IOCTL_HDMI_READ_DDCRAM)                          (void*, XC_DDCRAM_PROG_INFO*, MS_U32);
typedef void                             (*IOCTL_HDMI_PROG_DDCRAM)                          (void*, XC_DDCRAM_PROG_INFO*, MS_U32);
typedef MS_HDMI_CONTENT_TYPE             (*IOCTL_HDMI_GET_CONTENT_TYPE)                     (void*, MS_U8);
typedef MS_HDMI_EXT_COLORIMETRY_FORMAT   (*IOCTL_HDMI_GET_EXT_COLORIMETRY)                  (void*, MS_U8);
typedef MS_U8                            (*IOCTL_HDMI_GET_PIXEL_REPETITION)                 (void*, MS_U8);
typedef MS_BOOL                          (*IOCTL_HDMI_CHECK_4K2K)                           (void*, MS_U8);
typedef void                             (*IOCTL_HDMI_3D_4KX2K_PROCESS)                     (void*, MS_U8);
typedef void                             (*IOCTL_HDMI_AVG_SCALERINGDOWN)                    (void*, MS_U8, MS_BOOL);
typedef E_HDMI_ADDITIONAL_VIDEO_FORMAT   (*IOCTL_HDMI_CHECK_ADDITIONAL_FORMAT)              (void*, MS_U8);
typedef E_XC_3D_INPUT_MODE               (*IOCTL_HDMI_GET_3D_STRUCTURE)                     (void*, MS_U8);
typedef E_HDMI_3D_EXT_DATA_T             (*IOCTL_HDMI_GET_3D_EXT_DATA)                      (void*, MS_U8);
typedef void                             (*IOCTL_HDMI_GET_3D_META_FIELD)                    (void*, MS_U8, sHDMI_3D_META_FIELD*);
typedef MS_U8                            (*IOCTL_HDMI_GET_VIC_CODE)                         (void*, MS_U8);
typedef E_HDMI_VIC_4Kx2K_CODE            (*IOCTL_HDMI_GET_4KX2K_VIC_CODE)                   (void*, MS_U8);
typedef void                             (*IOCTL_HDMI_ARC_PINCONTROL)                       (void*, E_MUX_INPUTPORT, MS_BOOL, MS_BOOL);
typedef void                             (*IOCTL_DVI_SOFTWARE_RESET)                        (void*, E_MUX_INPUTPORT, MS_U16);
typedef MS_U32                           (*IOCTL_HDMI_SET_POWERSTATE)                       (void*, EN_POWER_MODE);
typedef void                             (*IOCTL_HDMI_DVI_HF_ADJUST)                        (void*);
typedef MS_BOOL                          (*IOCTL_HDMI_CHECKHDMI20_SETTING)                  (void*, E_MUX_INPUTPORT);
typedef void                             (*IOCTL_HDMI_SET_5V_DETECT_GPIO_SELECT)            (void*, MS_U32);
typedef MS_BOOL                          (*IOCTL_HDMI_GET_DE_STABLE_STATUS)                 (void*, E_MUX_INPUTPORT);
typedef MS_BOOL                          (*IOCTL_HDMI_CTRL)                                 (void*, MS_U8, MS_U32, void*, MS_U32);
typedef MS_U16                           (*IOCTL_HDMI_GET_DATA_INFO)                        (void*, E_HDMI_GET_DATA_INFO, MS_U8);
typedef void                             (*IOCTL_DUMP_SHM)                                  (void*);

#ifdef UFO_XC_VMARK
// APIXC_VMARK
typedef E_APIXC_ReturnValue          (*IOCTL_XC_VMARK_SET_PARAMETERS)                       (void*, MS_U8, XC_VMARK_PARAMETER*);
#endif

typedef void                             (*IOCTL_SWDS_Fire)                                  (void*, SCALER_WIN);
#ifdef UFO_XC_SUPPORT_DUAL_MIU
typedef MS_BOOL                          (*IOCTL_XC_ENABLE_DUAL_MODE)                      (void*, MS_BOOL , MS_U32 , MS_U32 , SCALER_WIN);
#endif

#ifdef UFO_XC_SUPPORT_HDMI_DOWNSCALE_OUTPUT_POLICY
typedef MS_U32                           (*IOCTL_XC_GET_HDMI_POLICY)                       (void*);
#endif
// apiXC_Auto.c
typedef struct __attribute__((aligned(4)))
{
    MS_BOOL bHaltAutoGeometry;
    MS_U8 _u8ThresholdValue;
    XC_Auto_CalibrationMode g_enCalbrationMode;
} ST_APIXC_AUTO;

// apiXC_PCMonitor.c
typedef struct __attribute__((aligned(4)))
{
    MS_U8 g_u8RGBSOG_DetectStatus;
    MS_U16 _u16TimingStableCounter[MAX_WINDOW];
    MS_U16 _u16TimingNosyncCounter[MAX_WINDOW];

    MS_U16 _u16TimingStableCounter_VGA;
    MS_U16 _u16TimingNosyncCounter_VGA;
    MS_U16 _u16TimingStableCounter_YPBPR;
    MS_U16 _u16TimingNosyncCounter_YPBPR;
    MS_U16 _u16TimingStableCounter_HDMI;
    MS_U16 _u16TimingNosyncCounter_HDMI;

    // for enable auto no signal detect use.
    MS_BOOL _bEnableAutoNoSignalDetect[MAX_WINDOW];

    // General function static variables
    // apiXC_PCMonitor()
    MS_U8 u8DeNoiseCount[MAX_WINDOW];
    MS_U8 u8DeStableCnt[MAX_WINDOW];

    // for 1080oi mode detect
    MS_BOOL _bNewModeInterlacedDetect;
} ST_APIXC_PCMONITOR;

typedef struct __attribute__((aligned(4)))
{
    //MS_HDMI_PACKET_VALUE_t pkt_value;

    /// Debug information
    MS_HDMI_INFO _info;
    MS_HDMI_Status _status;

    // mdrv_dvi_reset()
    MS_BOOL _ResetDone;

    // _MDrv_HDCP_GetStatus()
    MS_HDCP_STATUS_INFO_t enStatus_info;

    // MDrv_HDCP_Vsync_end_en()
    MS_BOOL _bHDCP_Vsync_done;
    MS_BOOL _bHDCP_Vsync_count;
    MS_BOOL bHDCP14RxREEFlag;
    MS_U32 u32VsyncTimer;
    MS_U32 ulPacketStatus[2];

    // MDrv_HDMI_SetPowerState()
    EN_POWER_MODE _prev_enPowerState;

    // HDCP22 relative

    void *pDummyCont;
#if !defined (__aarch64__)
    MS_U32 u32AlignmentDummy0;
#endif
}  ST_DRV_HDMI;

#if (LD_ENABLE==1)
typedef struct __attribute__((aligned(4)))
{
    MS_BOOL _bMDrvLD_InitFinished;
    ST_DRV_LD_INFO _stDrvLDInfo;
    EN_LD_PANEL_TYPE _enLDPanelType;
    ST_LD_DRAM_ADDR gsLDDramBaseAddr;

    // MDrv_LD_Test1()
    MS_U16 _u16LedPos;
    MS_U8 _au8Test_LedData[DRV_LD_SPI_BUF_MAX_SIZE];

    MS_BOOL _bIsLDPanel;

} ST_DRV_LD;

typedef struct __attribute__((aligned(4)))
{
    MS_U8 _u8Input_Block[18][32];
    MS_U8 _u8HDown_Block[18][32];
    MS_U8 _u8VDown_Block[18][32];
    MS_U8 _u8SBSDown_Block[18][32];
    MS_U8 _u8TBDown_Block[18][32];
    MS_U8 _u8Gamma_Block[18][32];
    MS_U8 _u8Histogram_Image[16];
    MS_U8 _u8VDown_Block_Pre[18][32];
    MS_U8 _u8PWM_Block[18][32];
    MS_U8 _u8LD_AlgoSPIBuffer[18*32];
    MS_U8 _u8IIR_T2[1152];
    MS_U32 _u32IIR_T2B[1152];
    MS_U32 _u32IIR_T2C[1152];
    MS_U8 _u8LED_resort[32*18];
    MS_U8 _u8H_Down_Ratio;
    MS_U8 _u8V_Down_Ratio;
    MS_U8 _u8H_Down_Ratio_3D;
    MS_U8 _u8V_Down_Ratio_3D;
    MS_U8 _u8Pack_Length_LD;
    MS_U8 _u8Pack_Length_PWM;
    MS_U8 _u8Frame_Idx;
    MS_U8 _u8Frame_Idx_Final;
    MS_U8 _u8Frame_Idx_Test;
    MS_U8 _u8Iluma_Pre;
    MS_U16 _u16Histogram_Zero;
    MS_U32 _u32Lluma_Sum;
    MS_U8 _u8DebugArrayDumpDummyCnt;
    MS_U8 _u8Gamma_Control;
    MS_U8 _u8OSDEnable;
    ST_TYPE_LD_PANEL_TYPE _stLDPanelPara;
    EN_LD_BLOCK_NUM _enLDBlock_Num;
    ST_DRV_LD_INFO _stLDInfo;
    MS_BOOL _bLDoff;
} ST_DRV_LDALGO;
#endif

typedef struct __attribute__((aligned(4)))
{
    E_XC_3D_INPUT_MODE _eInput3DMode[MAX_WINDOW]; // main and sub win could have different 3d input mode
    E_XC_3D_OUTPUT_MODE _eOutput3DMode; // main and sub win could only have the same 3d input mode
    E_XC_3D_PANEL_TYPE _e3DPanelType; // main and sub win could only have the same 3d input mode
    MS_BOOL  _b3DMainFirst; // main and sub win start address exchange
    MS_U16   _u163DHShift;            // horizontal shift pixel

#if HW_2DTO3D_SUPPORT
    MS_PHY _u32HW2DTO3D_DD_Buf;   //hw 2D to 3D depth detect buffer
    MS_PHY _u32HW2DTO3D_DR_Buf;   //hw 2D to 3D depth render buffer
    MS_XC_3D_HW2DTO3D_PARA _st3DHw2DTo3DPara; //hw 2d to 3d para
    MS_BOOL _bIsHW2Dto3DPatchEnabled;
#endif

#if (HW_DESIGN_3D_VER < 2)
    MS_BOOL  _bFrmpackToLinealterOpt;                    //optimize falg for framepacking in,line alternative out
#endif
    MS_BOOL  _bIsLR_Sbs2Line;                            //is sbs2line case
    MS_BOOL  _bSkipDefaultLRFlag;                        //do we need to skip lib default LR flag report, and using the Ap layer's setting?
    MS_XC_3D_DETECT3DFORMAT_PARA _stDetect3DFormatPara;  //detect 3d format para
    MS_XC_3D_FPINFO_PARA         _stFPInfoPara;          //define 3d fp info para
    MS_BOOL _bEnableAutoDetect3D;
    MS_BOOL _bCurEnableAutoDetect3D;
    E_XC_3D_AUTODETECT_METHOD _enAutoDetectMethod;
    E_XC_3D_INPUT_MODE _enCurDetectedRes;
    MS_BOOL _bResetFrameCount;
    MS_U16 _u16CheckedFrameCount;
    MS_BOOL _bDoubleAlign;
    MS_BOOL _bAutoDetect3DReadFirstBank;

#if HW_2DTO3D_SUPPORT
#if (HW_2DTO3D_VER >= 3)
    //mark Tp/Dn BG color
    PIXEL _stBlockStatus_Avg[NBLK_V_MAX][NBLK_H_MAX]; // avg color value
    PIXEL _stBlockStatus_YCCAvg[NBLK_V_MAX][NBLK_H_MAX]; // avg YCvCr value
    MS_U16   _u16BlockStatus_Edge[NBLK_V_MAX][NBLK_H_MAX]; // edge info
    MS_BOOL _bTpBGColorMarker[NBLK_V_MAX][NBLK_H_MAX];
    MS_BOOL _bDnBGColorMarker[NBLK_V_MAX][NBLK_H_MAX];
    MS_BOOL _bSkyMarker[NBLK_V_MAX][NBLK_H_MAX];
    MS_U32   _u32Histogram[32];//From DLC Histogram
    MS_S16 _s16TBB_BGColorSelWei;
    PIXEL _stBgPixelDn;
    PIXEL _stBgPixelTp;
    PIXEL _stBgPixelDnYCC;
    PIXEL _stBgPixelTpYCC;
    MS_S16 _s16TpBGCOLOR_ValidWei;//instead of TpBGCOLOR_Valid
    MS_S16 _s16DnBGCOLOR_ValidWei;//instead of DnBGCOLOR_Valid
    MS_S16 _s16DistantViewWei;// for blue sky ratio
    MS_S16 _s16DistantViewWei_Bottom;// for blue sky ratio
    MS_S16 _s16TBA_BGColorWei;
    MS_S16 _s16TBB_BGColorWei;
    MS_S16 _s16BothTpBGSKY_COLORCnt;
    MS_S16 _s16BothDnBGSKY_COLORCnt;
    MS_S16 _s16BlueSkyRatio;

    //for horizontal detection
    MS_U16 _u16Y_Sep;        // horizontal detection
    MS_U16 _u16Y_Sep_OUT;    // horizontal detection for output frame position

    MS_U8 _u8Pre_BlueAndFlatBlkCnt;
    MS_U8 _u8Pre_BlueAndFlatBlkCnt_Dn;

    //keeps blueSkyRatio stable
    MS_U8 _u8Pre_BlueSkyRatio1;
    MS_U8 _u8Pre_BlueSkyRatio2;

    //for blkbased Depth
    MS_U8 _u8Lowpass_En; // 1: 3x3 2: 5x5  3:7x7  4:disable bilinear
    MS_U8 _u8Bilateral_En;
    MS_U8 _u8DepthAB; //1:TableA 2:TableB 3:ABweight
    MS_U8 _u8Bilinear_En;
    MS_S16 _s16DistantViewRatio3;

    //multiview adjust dummy
    MS_S8 _s8Shift_Value;
    MS_S8 _s8Clamp_Value;
    MS_S8 _s8Multi_Gain;
    MS_U8 _u8RGB3DLUT_A[8][8][8]; //update to HW color table
    MS_U8 _u8RGB3DLUT_B[8][8][8]; //update to HW color table
    MS_U8 u8SRAMDataA[8][64];
    MS_U8 u8SRAMDataB[8][64];
    MS_U8 _u8RegionBlkWeightFrame[NBLK_V_MAX][NBLK_H_MAX]; //Table AB blending wei
    MS_U8 _u8DepthBlkBased[NBLK_V_MAX][NBLK_H_MAX];
    MS_S32 _s32Depth_Control_Var_IIR;
    MS_S32 _s32Mean_Depth_IIR;
    MS_U8 _u8Horizontal_Step;
    MS_U8 _u8Vertical_Step;

    //for IIR
    MS_U8  u8IIR_En;
    MS_U16 u16BL_IIRweight;
    MS_U16 u16PreDistantViewRatio1;
    MS_U16 u16PreDistantViewRatio2;
    MS_U16 u16Pre_RGB3DLUT_A[8][8][8];
    MS_U16 u16Pre_RGB3DLUT_B[8][8][8];
    MS_U16 u16Pre_YSep;
    MS_U16 u16Pre_depthBlkBased[NBLK_V_MAX][NBLK_H_MAX];
    MS_U16 u16Pre_RegionBlkWeightFrame[NBLK_V_MAX][NBLK_H_MAX];

    // _MDrv_SC_3D_2DTo3D_DepthDetectionPIX()
    MS_BOOL s_bFirst;
    //u9 new variable
    //HW_Info_Read
    MS_U8 _stBlockStatus_mot[NBLK_V_MAX][NBLK_H_MAX];
    MS_U8 NoMot_Cnt;
    MS_BOOL NoMot_Flg;
    MS_U8 _field[NBLK_V_MAX*NBLK_H_MAX];
    //BGColorDetection
    int BGColorBlkCntTp;
    int BGColorBlkCntBn;
    int BGColor_CbTp;
    int BGColor_CrTp;
    int BGColor_CbBn;
    int BGColor_CrBn;
    int BGColorBlkCntforTp;
    int BGColorBlkCntforBn;
    MS_U8 _u8TBABGColorWei;
    //MainObjDetection
    MS_U8 _u8NonSkyColorIdx[NBLK_H_MAX];
    MS_U8 _u8NonTpBGColorIdx[NBLK_H_MAX];
    //MS_U8 _u8MainObjBlkWeight[NBLK_V_MAX_OUT][NBLK_H_MAX_OUT]; //Table AB blending wei for MainObj
    MS_U8 _u8MainObjBlkWeight[NBLK_H_MAX][NBLK_H_MAX]; //Table AB blending wei for MainObj
    MS_U8 _u8MainObjWei;
    ////for LGE ver GLOBAL depth
    MS_S16 _s16BLKblueSkyRatio1;
    MS_S16 _s16BLKblueSkyRatio2;

    //for TrendDetection
    MS_U8 u8VerGLB_Wei;
    //for DEBUGFn
    MS_U8 u8DBG_En;
    MS_U8 u8Round;
    //for jubs division in refreshing LUT
    MS_U8 job_id;
    MS_U8 job_freq;
    MS_U8 updateSRAM_state;
    MS_U8 updateSRAM_PreState;
    //for Size related settings
    MS_U16 _u16PanelWidth;
    MS_U16 _u16PanelHeight;
    MS_U16 _u8NBLK_DR_H;
    MS_U16 _u8NBLK_DR_V;
#endif
#endif

    // MDrv_SC_3D_Adjust_PreHorDstSize()
#if HW_2DTO3D_SUPPORT
    MS_BOOL bAdjustDNRSize;
    MS_U32  u32OriginalDNRSize;
#endif

    // remap()
    MS_U8 s_u8flg;
    MS_U8 s_u8Ptr;

    // _MDrv_SC_3D_2DTo3D_DepthRefine()
    MS_U8 u8Ptr;
    MS_BOOL bFirstFlag;

    //  MDrv_XC_3D_LoadReg()
    E_XC_3D_INPUT_MODE  ePreInputMode;
    E_XC_3D_OUTPUT_MODE ePreOutputMode;

#if HW_2DTO3D_SUPPORT
    MS_U8  _u8FieldArray[ELEMENTNUM][HBLOCKNUM * VBLOCKNUM];
    MS_U8  _u8YCtrArray[MAXINDEX][HBLOCKNUM * VBLOCKNUM];
    MS_U32 _32YSumArray[HBLOCKNUM * VBLOCKNUM];
    MS_U32 _u32HisWeight;
    MS_U32 _u32ArtWeight;
    MS_U32 _u32ReturnWeightCur;
    MS_U32 _u32ReturnWeight;
    MS_U32 _u32X;
    MS_U32 _u32RemapAvgCurArray[MAXINDEX];
    MS_U32 _u32RemapAvgCurSum;
    MS_U32 _u32RemapAvgBseArray[MAXINDEX];
    MS_U32 _u32RemapAvgBseSum;
    MS_U32 _u32RemapAvgCur;
    MS_U32 _u32RemapAvgBse;
    MS_U8 _u8RemapAvgBias;
    MS_U8 _u8RemapLpfArray[HBLOCKNUM * VBLOCKNUM];
    MS_U16 _u16SBSCount;
    MS_U16 _u16TBCount;
    MS_U16 _u162DCount;
    MS_U16 _u16TotalCount;
#endif
    E_XC_3D_INPUT_MODE _ePreUsedInput3DMode[MAX_WINDOW]; // previous used 3d input format
    E_XC_3D_OUTPUT_MODE _ePreUsedOutput3DMode; // previous used 3d output format
    MS_BOOL             _bNeedRestore3DFormat; //need restore last using 3d format or not
    MS_BOOL  _bFALLRRToFA; //change 3D format frame_alternative_LLRR to frame_alternative in utopia
    MS_U16 _u16DDHSize; //DD H size
    MS_U16 _u16DDVSize; //DD V size
    E_XC_3D_INPUT_MODE _eFrcInput3DMode; // FRC 3d input mode
    E_XC_3D_OUTPUT_MODE _eFrcOutput3DMode; // FRC 3d output mode
    MS_BOOL  _bFAToTB; //change 3D format frame alternative to top bootom in utopia
    MS_BOOL  _bLAToTB; //change 3D format line alternative to top bootom in utopia
    MS_BOOL  _bFRC3DEnabled; //FRC 3D enable.
} ST_DRVXC_3D;

typedef struct __attribute__((aligned(4)))
{
    EN_FPLL_FSM _enFpllFsm; //PDP_FRAMELOCK
    EN_FPLL_THRESH_MODE _enFpllThreshMode;
    MS_BOOL _bFSM_FPLL_Working;
    MS_BOOL _bFSM_FPLL_Setting;
    MS_BOOL _bFPLL_Thresh_Mode;

    MS_BOOL  _bFpllCusFlag[E_FPLL_FLAG_MAX];
    MS_U32   _u32FpllCusData[E_FPLL_FLAG_MAX];
    MS_PHY   _u32MCDIFBAddress[E_XC_MCDI_MAX];
    MS_PHY   _u32MCDIFBSize[E_XC_MCDI_MAX];
    MS_BOOL _bForceFreerun;
    E_VFREQ_SEL _ForceVFreq;
    MS_U16 _u16DefaultVSyncWidth, _u16DefaultVSyncBackPorch, _u16DefaultHSyncWidth, _u16DefaultHSyncBackPorch;
    MS_BOOL _bHSyncChanged, _bVSyncChanged;

    //try to control flow: one setpaneltiming with one enable MC feature.
    //Note: if one setpaneltiming with two enable MC, some chaos flow may cause different param to Hal_SC_SetOPWriteOff().
    //if no fpll, it shouldn't enable MC feature    //Variable used for set MCNR in FPLL
    MS_BOOL _bNeedReloadMCNR;

    //PQ excel notify driver: whether this MADi mode needs MC feature
    MS_BOOL _bEnableMCNR[MAX_WINDOW];

    //MApi_XC_ForceSet_OPWriteOffEnable() will force disable MC directly, so we should skip other enable MC code until MApi_XC_ForceSet_OPWriteOffEnable(FALSE) is called
    MS_BOOL _bForceDisableMCNR;

    //Wait some vsync(_MDrv_SC_FPLL_GetMCNRDelayTime), then turn on MCNR, to avoid garbage
    MS_U32 _u32MCNRTimer;

    //when change zoom in UI, no need to do fpll again, so no need to do UCNR disable/Enable.
    //This flag is for skip UCNR disable action in setwindow for above case.
    MS_BOOL _abSkipOPWOffInSetWindow[MAX_WINDOW];

    MS_BOOL bFPLL_FSM;
    MS_U8 _u8CoastClearDebounce;
    MS_BOOL _bDisableCoastDebounce;
    MS_BOOL _bUseCustomFRCTable;
    MS_BOOL u32OutputPhaseStable;
    MS_BOOL u32OutputPRDLock;
    MS_BOOL _bSkipDisableOPWriteOffInFPLL[MAX_WINDOW];

    //FRC control
    SC_FRC_SETTING sFRC_Tbl[MAX_FRC_TABLE_INDEX];
    MS_U32 s_u32DelayT;
    MS_U32 s_u32InputVsyncStart;

    // _MApi_XC_FPLL_FSM_ISR()
    MS_U16 s_u16OrigPrd;         // max PRD error
    MS_U16 s_u16OrigPhase;             // max phase error
    MS_U32 s_u32FSMTimer;
#ifdef UFO_XC_AUTO_DOWNLOAD
    // auto download enable
    XC_AUTODOWNLOAD_CLIENT_INFO _stClientInfo[E_XC_AUTODOWNLOAD_CLIENT_MAX];
#endif
#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
    MS_BOOL _bEnableHDR;
    EN_XC_HDR_TYPE _enHDRType;
    EN_XC_INPUT_SOURCE_TYPE _enInputSourceType;
    XC_HDR_DMA_CONFIG_INFO _stDMAConfigInfo;
    XC_HDR_DMA_INIT_INFO _stDMAInitInfo;
#ifdef UFO_XC_HDMI_4K2K_DMA
    XC_HDR_DMA_INIT_INFO _stDMAInitInfo_Hdmi;
#endif
    EN_XC_HDR_COLOR_FORMAT _enColorFormat;
#endif
#endif
    //FrcInOut ratio
    MS_U8 g_u8FrcIn;
    MS_U8 g_u8FrcOut;
} ST_DRVXC_DISPLAY;

typedef struct __attribute__((aligned(4)))
{
    MS_BOOL _bDSForceIndexEnable[MAX_WINDOW];
} ST_DRVXC_DYNAMICSCALING;

typedef struct __attribute__((aligned(4)))
{
    XC_PCMONITOR_HK_INFO sPCMONITOR_status[2];

#if ENABLE_DEBUG_CAPTURE_N_FRAMES
    MS_BOOL bFinishedInputVSyncISR;
    MS_U8 u8Cnt;
#endif

    // MApi_XC_EuroHdtvHandler()
    MS_U8 u8Count;
} ST_DRVXC_IP;

typedef struct __attribute__((aligned(4)))
{
    XC_ISR_INFO stXCIsrInfo[MAX_SC_INT][MAX_ISR_NUM_OF_EACH_INT];
} ST_DRVXC_ISR;

typedef struct __attribute__((packed))
{
    MS_MLoad_Info stMLoadInfo;
    MS_MLoad_Info stMLGInfo;
    MS_U8 u8XCMLoadMIUSel;
    MS_BOOL g_bMLoadInitted;
} ST_DRVXC_MENULOAD;

typedef struct __attribute__((aligned(4)))
{
    MS_PHY _u32DNRBaseAddr0[2];
    MS_PHY _u32DNRBufSize[2];

    // add FRCM buffer baseaddress and buffer size
    MS_PHY _u32FRCMBaseAddr0[2];
    MS_PHY _u32FRCMBufSize[2];

    MS_BOOL _bHAutoFitPrescalingEna;
    MS_BOOL _bVAutoFitPrescalingEna; //TRUE: Means driver auto prescaling to fit display window

    //HSize After prescaling is changed in Driver logic(3D, ForcePrescaling, etc), in this case, PQ HSD setting should be skipped
    MS_BOOL _bHSizeChangedManually;
    MS_PHY u32DNRBaseOffset[2];
    MS_PHY u32FRCMBaseOffset[2];
    MS_U8  _au8StoreFrameNumFactor[2];
    MS_BOOL _bMainWinSkipWaitOutputVsync;
    MS_BOOL _bSubWinSkipWaitOutputVsync;
    MS_BOOL _bPQSetHPreScalingratio;

#ifdef ENABLE_SCALING_WO_MUTE
    MS_U8 _u8OPMFetchRatio[MAX_WINDOW]; // for skip OPM line
#endif
    MS_PHY _au32PreFBAddress[MAX_WINDOW];
    MS_PHY _au32PreFBSize[MAX_WINDOW];

    // MDrv_SC_set_prescaling_ratio()
    // Store Pre-scaling source / dest size of previouse
    MS_U16 _u16HScaleSrc[MAX_WINDOW];
    MS_U16 _u16HScaleDst[MAX_WINDOW];
    MS_U16 _u16VScaleSrc[MAX_WINDOW];
    MS_U16 _u16VScaleDst[MAX_WINDOW];
    MS_U8  _u8HSD_Idx;

#if (HW_DESIGN_4K2K_VER == 4)
    MS_U8  _u8StoreFRCMFrameNumFactor[2];
#endif

    // add dual miu buffer baseaddress and buffer size
    MS_PHY _u32PreDualFBAddress[MAX_WINDOW];
    MS_PHY _u32PreDualFBSize[MAX_WINDOW];
    MS_PHY _u32DualMiuDNRBaseAddr0[MAX_WINDOW];
    MS_PHY _u32DualMiuDNRBufSize[MAX_WINDOW];

    //The flag indicate Panel mute
    MS_BOOL _bPanelSkipWaitOutputVsync;
} ST_DRVXC_SCALING;

typedef struct __attribute__((aligned(4)))
{
    MS_BOOL bNeedInitShared;

#if(XC_CONTEXT_ENABLE)
    XC_Context _SContext;
    XC_Context_Shared _SCShared;
#endif

    MS_XC_MEM_FMT s_eMemFmt[MAX_WINDOW];
    MS_BOOL       s_bKeepPixelPointerAppear;
    InputSource_Mux_Attr s_InputSource_Mux_attr;

    // For skip redundant set-inputsource.
    // Notice!! If disconnect source, this source variable must be cleared
    // (To prevent below case: Create source A -> Delete source A -> Create source A again.
    // The 2nd create source will re-download ADC table or other setting. We need reset SC IP again for correct setting flow.
    // So we need SetScalerInputSource again.)
    INPUT_SOURCE_TYPE_t enOldInputSrc_ForScalerSetSource[MAX_WINDOW];

    XC_ApiInfo  _stXC_ApiInfo;
    MS_BOOL _bSkipSWReset;   ///< Flag for: If the system needs to show Logo from MBoot layer to APP layer without flashing. \n We bypass SW reset in MApi_XC_Init.
    MS_BOOL _bDFBInit;
    MS_S8 s8HPixelShift;
    MS_S8 s8VPixelShift;

#ifndef DISABLE_HW_PATTERN_FUNCTION
#ifdef SUPPORT_HW_TESTPATTERN
    MS_BOOL bIsHWPatternEnabled;
    MS_BOOL bIsMute;
    MS_BOOL bIsInputSrcDisabled;
#endif
#endif

#if SUPPORT_OP2_TEST_PATTERN
    MS_U16  u16OP2_color_3x3;
    MS_BOOL bIsOP2TestPattern;
    MS_BOOL bIsOP2InputSrcDisabled;
    MS_BOOL bIsOP2Mute;
#endif

#if FRC_INSIDE
    MS_BOOL bEnableUserMode;
#endif

#if (ENABLE_NONSTD_INPUT_MCNR==1)
    MS_BOOL bIsUnStdSignal;
    MS_U8 u8StdSignalStbCnt;
    MS_U8 u8UnStdOutput[MAX_WINDOW];
#endif

    MS_U16 OSDC_FREQ;  // default 60Hz

    // _MApi_XC_SetWindow()
    MS_BOOL _bSetDualMainWinDone;
    MS_BOOL _bSetDualSubWinDone;

    // MApi_XC_SetPowerState()
    EN_POWER_MODE _enPrevPowerState;

    // Pixel Shift related
    MS_U16 gu16HPixelShiftRange;
    MS_U16 gu16VPixelShiftRange;
    PIXEL_SHIFT_FEATURE gePixelShiftFeature;

    XC_MUX_INPUTSRCTABLE mapping_tab[50];
    MS_U32 matrix_length;

    //signal stabel for STR
    MS_BOOL bSignalStableForStr;

    //Use DDR4 DRAM
    MS_BOOL bUseDdr4DramForXc;
    MS_BOOL bUseDdr4DramForFrc;

    //Multi Process re-init
    MS_BOOL bModuleInited;

    MS_BOOL bTimingUnstableForStr;
    MS_U8  u8DebounceForStr;
} ST_DRVXC_MVIDEO;

typedef struct __attribute__((aligned(4)))
{
    E_HDMI_SYNC_TYPE s_HDMISYNC_type;
    MS_BOOL _bSupportHVMode;

    MS_BOOL g_bIsIMMESWITCH;
    MS_BOOL g_bIMMESWITCH_DVI_POWERSAVING;
    MS_BOOL g_bDVI_AUTO_EQ;
    MS_BOOL g_bIsMhlHotPlugInverse;
    MS_U8 _u8MhlSupportInfo;

    // XC Init
    XC_INITDATA g_XC_InitData;

    // XC Init MISC
    XC_INITMISC g_XC_Init_Misc;

    //SC
    SC_SWDB_INFO stDBreg;         ///< SW double buffer
    SC_SWDB_INFO stMainDBreg;     ///< SW double buffer main window
    SC_SWDB_INFO stSubDBreg;      ///< SW double buffer sub window

#ifndef DISABLE_HW_PATTERN_FUNCTION
#ifdef SUPPORT_HW_TESTPATTERN
    // For HW internal test pattern using
    MS_BOOL bOP1_PatGen_Init;
#endif
#endif

    MS_WINDOW_TYPE g_RepWindow;
    MS_BOOL g_bEnableRepWindowForFrameColor;
    MS_BOOL bMainIsOnTop;

    // for Fast Boot
    MS_U8 g_u8HdcpKeyDataBackUp[HDCP_KEY_LENGTH];
    MS_BOOL g_bHDMIInverseBackUp[4];
    MS_U8  g_u8EDID_DVIBackUp[5][EDID_KEY_LENGTH]; // 5 for 5 types of EDID
    MS_U32 g_u32InitDataLen_Suspend;
    MS_U32 g_u32InitMiscDataLen_Suspend;
    MS_PHY g_u32MLoadPhyAddr_Suspend;
    MS_U32 g_u32MLoadBufByteLen_Suspend;
    MS_U8 g_u8DispWindowColor_Suspend[MAX_WINDOW];
    MS_PHY g_u32MCDIFBAddress_Suspend[E_XC_MCDI_MAX];
    MS_PHY g_u32MCDIFBSize_Suspend[E_XC_MCDI_MAX];
    MS_BOOL g_bEnableMCDI_Suspend[E_XC_MCDI_MAX];
    MS_U32 g_u32OSDCCtrltype_Suspend;
    MS_U32 g_u32FrameColor_Suspend;

    //FRC
    XC_PREINIT_INFO_t g_XC_Pnl_Misc;

    //EURO HDTV support flag
    MS_BOOL _bEnableEuro;

#if FRC_INSIDE
    MS_BOOL bIsFRCInited;
    E_XC_FRC_InputTiming enFRC_CurrentInputTiming;
#if (HW_DESIGN_4K2K_VER == 6) // For Manhattan, FSC+FRC
    MS_BOOL g_bFSCEnabled;
    MS_BOOL g_bFRCEnabled;
    MS_U16  g_u16GOPOffsetH;
    MS_U16  g_u16GOPOffsetV;
    MS_U16  g_u16MEMCLevel;
    MS_BOOL bIsFRCMute;
    MS_BOOL g_bIsAddrUpdated;
    XC_FRC_ADDRESS_INFO g_stXC_FRC_Address;
#endif
#endif
    MS_BOOL g_bFast_Get_VFreq;
    MS_BOOL g_bEnableLockFreqOnly;
    ST_DRVXC_MENULOAD stdrvXC_Menuload[MAX_XC_DEVICE_NUM];
    MS_BOOL g_bSpreandEnable;
#if (HW_DESIGN_HDMITX_VER == 2)
    EN_XC_HDMITX_COLORDEPTH enHDMITXColorDepth;
#endif

} ST_DRVXC_MVIDEO_CONTEXT;

typedef struct __attribute__((aligned(4)))
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    wait_queue_head_t _XC_EventQueue;
    MS_U32            _XC_EventFlag;
    MS_BOOL           _XC_VSyncRun;
    MS_U32            _XC_VSyncCount;
    MS_U32            _XC_VSyncMax;
#endif
} ST_XC_UTILITY;

typedef struct __attribute__((aligned(4)))
{
    XC_Adc_BackupSetting _stAutoAdcSetting;
    MS_BOOL bIsYPbPrLooseLPF;
    MS_BOOL _bSourceSwitched;
    MS_BOOL _bEnableHWCalibration;
    MS_BOOL _bUseBandgap;
    E_ADC_CVBSOUT_TYPE _eCvbsOutType;

    // Hal_ADC_set_mode()
    ADC_INPUTSOURCE_TYPE enLastSourceType;
    MS_U16 u16LastHtotal;
    MS_U16 u16LastPixelClk;
    MS_U16 u16LastSamplingRatio;
    MS_U16 _u16IdacCurrrenMode;
} ST_HAL_ADC;

#if FRC_INSIDE
typedef struct __attribute__((aligned(4)))
{
    MS_AutoDownLoad_Info stADLGInfo;
    FRCTABLE_INFO _FRCTableInfo;
    MS_U8 _u8FRCTabIdx[FRC_IP_NUM_Passive]; // store all TabIdx of all IPs
    SC_MIUMASK_t FRC_Miu0Mask;
    SC_MIUMASK_t FRC_Miu1Mask;
    SC_MIUMASK_t FRC_Miu0MaskOld;
    SC_MIUMASK_t FRC_Miu1MaskOld;
} ST_HAL_FRC;
#endif

typedef struct __attribute__((aligned(4)))
{
    MS_BOOL _bSetVGASogEn;
    MS_U8   _u8SetSogDetectStatus;
} ST_HAL_IP;

typedef struct __attribute__((aligned(4)))
{
    SC_MIUMASK_t Miu0Mask;
    SC_MIUMASK_t Miu1Mask;
    SC_MIUMASK_t Miu2Mask;
    SC_MIUMASK_t Miu0MaskOld;
    SC_MIUMASK_t Miu1MaskOld;
    SC_MIUMASK_t Miu2MaskOld;
    MS_BOOL bDynamicScalingEnable;
    MS_PHY _u32DNRBaseAddress[MAX_WINDOW];
    MS_PHY _u32DNRBufferSize[MAX_WINDOW];

    MS_PHY _u32FRCBaseAddress[MAX_WINDOW];
    MS_PHY _u32FRCBufferSize[MAX_WINDOW];

    MS_U16 u16OP1_fclk_gate_en;
    MS_U16 u16OP1_reg_ip2_clk_gate_en;
    MS_U16 u16OP1_ficlk_status;
    MS_U32 u32OP1_reg_pre_down_f2;
    MS_U32 u32OP1_reg_pre_down_f1;
    MS_U16 u16OP1_color_3x3;
    MS_U16 u16OP1_color_vip;
    MS_U16 u16OP1_reg_di1lb_en;
    MS_U16 u16OP1_bypass_miu;
    MS_U16 u16OP1_vop_dualpixel;
    MS_U16  g_u16TmpValue;
    MS_BOOL g_bCntFlg;
    MS_BOOL bVSDshift[MAX_WINDOW];

    // MHAL_SC_Set_DynamicScaling()
    MS_S32 s32_HalXC_MotionHistoryRestore_TimerId;
    MS_U16 su16OldValue;
    MS_U16 su16OldValue_BWD_Status;
    MS_BOOL sbOldValueReaded;

    MS_BOOL bTrig_pStored;
    MS_BOOL bDynamicScalingOPMEnable;
    MS_BOOL bDynamicScalingOPSEnable;

    // HAL_SC_FilmMode_Patch1()
    MS_U8 u8Time_count;

    // For Seamless Zapping
#if SUPPORT_SEAMLESS_ZAPPING
    MS_PHY u32DynamicScalingBaseAddr;
    MS_U8 u8DynamicScalingIndexDepth;
    MS_BOOL bSeamlessZappingEnable[MAX_WINDOW];
    MS_U8 u8SeamlessZappingCurIndex[MAX_WINDOW];
    MS_U8 u8SeamlessZappingNextIndex[MAX_WINDOW];
#endif

#if (HW_DESIGN_4K2K_VER == 4)
    MS_PHY u32DynamicScalingBaseAddr;
    MS_U8  u8DynamicScalingIndexDepth;
    MS_U32 u32DynamicScalingBufSize;
    MS_U8  u8DynamicScalingCurrentIndex[MAX_WINDOW];
    MS_U8  u8DynamicScalingCurrentIndex_Ring[MAX_WINDOW];
    MS_U8  u8DynamicScalingNextIndex[MAX_WINDOW];
#endif

#if (HW_DESIGN_4K2K_VER == 6) || (HW_DESIGN_4K2K_VER == 7)
    MS_PHY u32DynamicScalingBaseAddr;
    MS_PHY u32DynamicScalingBaseAddr_OPM;
    MS_PHY u32DynamicScalingBaseAddr_IPS;
    MS_PHY u32DynamicScalingBaseAddr_OPS;
    MS_U8  u8DynamicScalingIndexDepth;
    MS_U32 u32DynamicScalingBufSize;
    MS_U8  u8DynamicScalingCurrentIndex[MAX_WINDOW];
    MS_U8  u8DynamicScalingCurrentIndex_Ring[MAX_WINDOW];
    MS_U8  u8DynamicScalingNextIndex[MAX_WINDOW];
#endif

    //save value array
    MS_U16  u16OldValue[E_STORE_VALUE_MAX];
    MS_U16  u16NewValue[E_STORE_VALUE_MAX];
    MS_BOOL bUsingNewValue[E_STORE_VALUE_MAX];

#if (HW_DESIGN_4K2K_VER == 7)
    MS_BOOL bSc1HvspFilterIsSet;
    MS_U16 u16Sc1HvspFilterValue;
#endif
} ST_HAL_SC;

typedef struct __attribute__((aligned(4)))
{
    XC_OPTEE_HANDLER op_tee_xc[MAX_WINDOW];
    XC_OPTEE_MUX_DATA op_tee_mux;
} ST_HAL_OPTEE;

#ifdef UFO_XC_PACKED_SHM
typedef struct __attribute__((packed))
#else
typedef struct __attribute__((aligned(4)))
#endif
{
    // flow control related
    MS_BOOL bResourceRegistered;

    ST_APIXC_AUTO stapiXC_Auto;
    ST_APIXC_PCMONITOR stapiXC_PCMonitor;
    ST_DRV_HDMI stdrv_HDMI;

#if (LD_ENABLE==1)
    ST_DRV_LD stdrv_LD;
    ST_DRV_LDALGO stdrv_LDALGO;
#endif

    ST_DRVXC_3D stdrvXC_3D;
    ST_DRVXC_DISPLAY stdrvXC_Display;
    ST_DRVXC_DYNAMICSCALING stdrvXC_Dynamicscaling;
    ST_DRVXC_IP stdrvXC_IP;
    ST_DRVXC_ISR stdrvXC_ISR;
    ST_DRVXC_SCALING stdrvXC_Scaling;
    ST_DRVXC_MVIDEO stdrvXC_MVideo;
    ST_DRVXC_MVIDEO_CONTEXT stdrvXC_MVideo_Context;
    ST_XC_UTILITY stXC_Utility;

    ST_HAL_ADC sthal_ADC;
#if FRC_INSIDE
    ST_HAL_FRC sthal_FRC;
#endif
    ST_HAL_IP sthal_IP;
    ST_HAL_SC sthal_SC;
    ST_HAL_OPTEE sthal_Optee;

    MS_PHY _u32DualDNRBaseAddress[MAX_WINDOW];
    MS_PHY _u32DualDNRBufferSize[MAX_WINDOW];

    MS_U16  usVSDBPacketInfo[HDMI_VSDB_PACKET_SIZE];
    MS_U16  usHDRPacketInfo[HDMI_HDR_PACKET_SIZE];
    MS_BOOL bVSDBReceiveFlag;
    MS_BOOL bHDRReceiveFlag;
    MS_BOOL bXCMWEStatus;
} XC_RESOURCE_PRIVATE;

typedef struct __attribute__((aligned(4)))
{
    ST_HAL_OPTEE sthal_Optee;
} XC_RESOURCE_PRIVATE_FOR_TEE;

typedef struct
{
    INPUT_SOURCE_TYPE_t input_source[MAX_WINDOW];
    XC_MUX_PATH_INFO Path_Info[MAX_DATA_PATH_SUPPORTED];
    MS_U8 u8PathStatus[MAX_DATA_PATH_SUPPORTED];
    XC_SETWIN_INFO pstXC_SetWin_Info[MAX_WINDOW];
    XC_SetTiming_Info pTimingInfo;
    MS_BOOL bPIP_enable;

    XC_Auto_CalibrationType enAuto_Cal_type;
    XC_Auto_TuneType enAutoTuneType;
    APIXC_AdcGainOffsetSetting stADCSetting;
} XC_REGS_SAVE_AREA;

typedef struct __attribute__((packed))
{
    MS_U64 dummy;
} MLOAD_RESOURCE_PRIVATE;

typedef struct __attribute__((packed))
{
    MS_U32 u32DeviceID;
    // APIXC_H
    IOCTL_XC_GETLIBVER                              fpXC_GetLibVer;
    IOCTL_XC_GETINFO                                fpXC_GetInfo;
    IOCTL_XC_GETSTATUS                              fpXC_GetStatus;
    IOCTL_XC_GETSTATUS_NODELAY                      fpXC_GetStatus_nodelay;
    IOCTL_XC_GETSTATUSEX                            fpXC_GetStatusEx;
    IOCTL_XC_SETDBGLEVEL                            fpXC_SetDbgLevel;
    IOCTL_XC_FPLLDBGMODE                            fpXC_FPLLDbgMode;
    IOCTL_XC_FPLLCUSTOMERMODE                       fpXC_FPLLCustomerMode;
    IOCTL_XC_SETIOMAPBASE                           fpXC_SetIOMapBase;
    IOCTL_XC_INIT                                   fpXC_Init;
    IOCTL_XC_GETCONFIG                              fpXC_GetConfig;
    IOCTL_XC_INIT_MISC                              fpXC_Init_MISC;
    IOCTL_XC_GETMISCSTATUS                          fpXC_GetMISCStatus;
    IOCTL_XC_GETCAPABILITY                          fpXC_GetCapability;
    IOCTL_XC_GETCHIPCAPS                            fpXC_GetChipCaps;
    IOCTL_XC_EXIT                                   fpXC_Exit;
    IOCTL_XC_SETDYNAMICSCALING                      fpXC_SetDynamicScaling;
    IOCTL_XC_SETDYNAMICSCALING_FLAG                 fpXC_SetDynamicScalingFlag;
    IOCTL_XC_GET_DNRBASEOFFSET                      fpXC_Get_DNRBaseOffset;
    IOCTL_XC_GET_FRAMENUMFACTOR                     fpXC_Get_FrameNumFactor;
    IOCTL_XC_SETWINDOW                              fpXC_SetWindow;
    IOCTL_XC_SETDUALWINDOW                          fpXC_SetDualWindow;
    IOCTL_XC_SETTRAVELINGWINDOW                     fpXC_SetTravelingWindow;
    IOCTL_XC_SETINPUTSOURCE                         fpXC_SetInputSource;
    IOCTL_XC_ISYUVSPACE                             fpXC_IsYUVSpace;
    IOCTL_XC_ISMEMORYFORMAT422                      fpXC_IsMemoryFormat422;
    IOCTL_XC_ENABLEFORCERGBIN                       fpXC_EnableForceRGBin;
    IOCTL_XC_ENABLEMIRRORMODEEX                     fpXC_EnableMirrorModeEx;
    IOCTL_XC_GETMIRRORMODETYPEEX                    fpXC_GetMirrorModeTypeEx;
    IOCTL_XC_GETSYNCSTATUS                          fpXC_GetSyncStatus;
    IOCTL_XC_CONFIGCMA                              fpXC_ConfigCMA;
    IOCTL_XC_WAITOUTPUTVSYNC                        fpXC_WaitOutputVSync;
    IOCTL_XC_WAITINPUTVSYNC                         fpXC_WaitInputVSync;
    IOCTL_XC_SETHDMISYNCMODE                        fpXC_SetHdmiSyncMode;
    IOCTL_XC_GETHDMISYNCMODE                        fpXC_GetHdmiSyncMode;
    IOCTL_XC_SETREPWINDOW                           fpXC_SetRepWindow;
    IOCTL_XC_SET_SKIP_OPWRITEOFF_IN_SETWINDOW       fpXC_SkipOPWriteOffInSetWindow;
    IOCTL_XC_GET_SKIP_OPWRITEOFF_IN_SETWINDOW       fpXC_GetSkipOPWriteOffInSetWindow;
    IOCTL_XC_SET_SKIP_DISABLE_OPWRITEOFF_IN_FPLL    fpXC_SetSkipDisableOPWriteOffInFPLL;
    IOCTL_XC_SET_OPWRITEOFFENABLE                   fpXC_Set_OPWriteOffEnable;
    IOCTL_XC_SET_OPWRITEOFF_ENABLE_TO_REG           fpXC_Set_OPWriteOffEnableToReg;
    IOCTL_XC_FORCESET_OPWRITEOFF_ENABLE             fpXC_ForceSet_OPWriteOffEnable;
    IOCTL_XC_GET_OPWRITEOFFENABLE                   fpXC_Get_OPWriteOffEnable;
    IOCTL_XC_SETDISPWINTOREG                        fpXC_SetDispWinToReg;
    IOCTL_XC_GETDISPWINFROMREG                      fpXC_GetDispWinFromReg;
    IOCTL_XC_FREEZEIMG                              fpXC_FreezeImg;
    IOCTL_XC_ISFREEZEIMG                            fpXC_IsFreezeImg;
    IOCTL_XC_GENERATEBLACKVIDEOFORBOTHWIN           fpXC_GenerateBlackVideoForBothWin;
    IOCTL_XC_SET_BLSK                               fpXC_Set_BLSK;
    IOCTL_XC_GENERATEBLACKVIDEO                     fpXC_GenerateBlackVideo;
    IOCTL_XC_ISBLACKVIDEOENABLE                     fpXC_IsBlackVideoEnable;
    IOCTL_XC_ENABLEFRAMEBUFFERLESS                  fpXC_EnableFrameBufferLess;
    IOCTL_XC_ISCURRENTFRAMEBUFFERLESSMODE           fpXC_IsCurrentFrameBufferLessMode;
    IOCTL_XC_ENABLEREQUEST_FRAMEBUFFERLESS          fpXC_EnableRequest_FrameBufferLess;
    IOCTL_XC_ISCURRENTREQUEST_FRAMEBUFFERLESSMODE   fpXC_IsCurrentRequest_FrameBufferLessMode;
    IOCTL_XC_GET_3D_HW_VERSION                      fpXC_Get_3D_HW_Version;
    IOCTL_XC_GET_3D_ISSUPPORTEDHW2DTO3D             fpXC_Get_3D_IsSupportedHW2DTo3D;
    IOCTL_XC_SET_3D_MODE                            fpXC_Set_3D_Mode;
    IOCTL_XC_SET_3D_MAINWIN_FIRSTMODE               fpXC_Set_3D_MainWin_FirstMode;
    IOCTL_XC_SET_3D_LR_FRAME_EXCHG                  fpXC_Set_3D_LR_Frame_Exchg;
    IOCTL_XC_3D_IS_LR_FRAME_EXCHGED                 fpXC_3D_Is_LR_Frame_Exchged;
    IOCTL_XC_GET_3D_INPUT_MODE                      fpXC_Get_3D_Input_Mode;
    IOCTL_XC_GET_3D_OUTPUT_MODE                     fpXC_Get_3D_Output_Mode;
    IOCTL_XC_GET_3D_PANEL_TYPE                      fpXC_Get_3D_Panel_Type;
    IOCTL_XC_GET_3D_MAINWIN_FIRST                   fpXC_Get_3D_MainWin_First;
    IOCTL_XC_3DMAINSUB_IPSYNC                       fpXC_3DMainSub_IPSync;
    IOCTL_XC_SET_3D_VERVIDEOOFFSET                  fpXC_Set_3D_VerVideoOffset;
    IOCTL_XC_GET_3D_VERVIDEOOFFSET                  fpXC_Get_3D_VerVideoOffset;
    IOCTL_XC_IS3DFORMATSUPPORTED                    fpXC_Is3DFormatSupported;
    IOCTL_XC_SET_3D_HSHIFT                          fpXC_Set_3D_HShift;
    IOCTL_XC_ENABLE_3D_LR_SBS2LINE                  fpXC_Enable_3D_LR_Sbs2Line;
    IOCTL_XC_GET_3D_HSHIFT                          fpXC_Get_3D_HShift;
    IOCTL_XC_SET_3D_HW2DTO3D_BUFFER                 fpXC_Set_3D_HW2DTo3D_Buffer;
    IOCTL_XC_SET_3D_HW2DTO3D_PARAMETERS             fpXC_Set_3D_HW2DTo3D_Parameters;
    IOCTL_XC_GET_3D_HW2DTO3D_PARAMETERS             fpXC_Get_3D_HW2DTo3D_Parameters;
    IOCTL_XC_SET_3D_DETECT3DFORMAT_PARAMETERS       fpXC_Set_3D_Detect3DFormat_Parameters;
    IOCTL_XC_GET_3D_DETECT3DFORMAT_PARAMETERS       fpXC_Get_3D_Detect3DFormat_Parameters;
    IOCTL_XC_DETECT3DFORMATBYCONTENT                fpXC_Detect3DFormatByContent;
    IOCTL_XC_DETECTNL                               fpXC_DetectNL;
    IOCTL_XC_3D_POSTPQSETTING                       fpXC_3D_PostPQSetting;
    IOCTL_XC_SET_3D_FPINFO                          fpXC_Set_3D_FPInfo;
    IOCTL_XC_ENABLEAUTODETECT3D                     fpXC_EnableAutoDetect3D;
    IOCTL_XC_GETAUTODETECT3DFLAG                    fpXC_GetAutoDetect3DFlag;
    IOCTL_XC_SET_3D_SUBWINCLK                       fpXC_Set_3D_SubWinClk;
    IOCTL_XC_3D_IS_LR_SBS2LINE                      fpXC_3D_Is_LR_Sbs2Line;
    IOCTL_SC_3D_IS_SKIP_DEFAULT_LR_FLAG             fpSC_3D_Is_Skip_Default_LR_Flag;
    IOCTL_XC_3D_ENABLE_SKIP_DEFAULT_LR_FLAG         fpXC_3D_Enable_Skip_Default_LR_Flag;
    IOCTL_XC_MUX_INIT                               fpXC_Mux_Init;
    IOCTL_XC_MUX_SOURCEMONITOR                      fpXC_Mux_SourceMonitor;
    IOCTL_XC_MUX_CREATEPATH                         fpXC_Mux_CreatePath;
    IOCTL_XC_MUX_DELETEPATH                         fpXC_Mux_DeletePath;
    IOCTL_XC_MUX_ENABLEPATH                         fpXC_Mux_EnablePath;
    IOCTL_XC_MUX_TRIGGERPATHSYNCEVENT               fpXC_Mux_TriggerPathSyncEvent;
    IOCTL_XC_MUX_TRIGGERDESTONOFFEVENT              fpXC_Mux_TriggerDestOnOffEvent;
    IOCTL_XC_MUX_ONOFFPERIODICHANDLER               fpXC_Mux_OnOffPeriodicHandler;
    IOCTL_XC_MUX_GETPATHINFO                        fpXC_Mux_GetPathInfo;
    IOCTL_XC_MUX_SETSUPPORTMHLPATHINFO              fpXC_Mux_SetSupportMhlPathInfo;
    IOCTL_XC_MUX_SETMHLHOTPLUGINVERSEINFO           fpXC_Mux_SetMhlHotPlugInverseInfo;
    IOCTL_XC_MUX_GETHDMIPORT                        fpXC_Mux_GetHDMIPort;
    IOCTL_XC_MUX_GETMAPPINGTAB                      fpXC_Mux_GetMappingTab;
    IOCTL_XC_MUX_MAPINPUTSOURCETOVDYMUXPORT         fpXC_MUX_MApinputSourceToVDYMuxPORT;
    IOCTL_XC_SET_NR                                 fpXC_Set_NR;
    IOCTL_XC_FILMMODE_P                             fpXC_FilmMode_P;
    IOCTL_XC_GETUCENABLED                           fpXC_GetUCEnabled;
    IOCTL_XC_GENSPECIFICTIMING                      fpXC_GenSpecificTiming;
    IOCTL_XC_GETDEBYPASSMODE                        fpXC_GetDEBypassMode;
    IOCTL_XC_GETDEWINDOW                            fpXC_GetDEWindow;
    IOCTL_XC_GETDEWIDTHHEIGHTINDEBYPASSMODE         fpXC_GetDEWidthHeightInDEByPassMode;
    IOCTL_XC_GETCAPTUREWINDOW                       fpXC_GetCaptureWindow;
    IOCTL_XC_SETCAPTUREWINDOWVSTART                 fpXC_SetCaptureWindowVstart;
    IOCTL_XC_SETCAPTUREWINDOWHSTART                 fpXC_SetCaptureWindowHstart;
    IOCTL_XC_SETCAPTUREWINDOWVSIZE                  fpXC_SetCaptureWindowVsize;
    IOCTL_XC_SETCAPTUREWINDOWHSIZE                  fpXC_SetCaptureWindowHsize;
    IOCTL_XC_SOFTWARERESET                          fpXC_SoftwareReset;
    IOCTL_XC_CALCULATEHFREQX10                      fpXC_CalculateHFreqx10;
    IOCTL_XC_CALCULATEHFREQX1K                      fpXC_CalculateHFreqx1K;
    IOCTL_XC_CALCULATEVFREQX10                      fpXC_CalculateVFreqx10;
    IOCTL_XC_CALCULATEVFREQX1K                      fpXC_CalculateVFreqx1K;
    IOCTL_XC_GETACCURATEVFREQX1K                    fpXC_GetAccurateVFreqx1K;
    IOCTL_XC_INTERRUPTATTACH                        fpXC_InterruptAttach;
    IOCTL_XC_INTERRUPTDEATTACH                      fpXC_InterruptDeAttach;
    IOCTL_XC_DISABLEINPUTSOURCE                     fpXC_DisableInputSource;
    IOCTL_XC_ISINPUTSOURCEDISABLED                  fpXC_IsInputSourceDisabled;
    IOCTL_XC_CHANGEPANELTYPE                        fpXC_ChangePanelType;
    IOCTL_XC_GETCURRENTREADBANK                     fpXC_GetCurrentReadBank;
    IOCTL_XC_GETCURRENTWRITEBANK                    fpXC_GetCurrentWriteBank;
    IOCTL_XC_SETAUTOPRESCALING                      fpXC_SetAutoPreScaling;
    IOCTL_XC_GETVSYNCWIDTH                          fpXC_GetVSyncWidth;
    IOCTL_XC_SET_GOP_ENABLE                         fpXC_set_GOP_Enable;
    IOCTL_XC_IP_SEL_FOR_GOP                         fpXC_ip_sel_for_gop;
    IOCTL_XC_SETVOPNBL                              fpXC_SetVOPNBL;
    IOCTL_XC_GETDSTINFO                             fpXC_GetDstInfo;
    IOCTL_XC_SET_FD_MASK_BYWIN                      fpXC_Set_FD_Mask_ByWin;
    IOCTL_XC_GET_FD_MASK_BYWIN                      fpXC_Get_FD_Mask_ByWin;
    IOCTL_XC_GET_FD_MASK_STATUS                     fpXC_Get_FD_Mask_Status;
    IOCTL_XC_SETIP1TESTPATTERN                      fpXC_SetIP1TestPattern;
#ifdef UFO_XC_TEST_PATTERN
    IOCTL_XC_GENERATETESTPATTERN                    fpXC_GenerateTestPattern;
#endif
    IOCTL_XC_INITIPFORINTERNALTIMING                fpXC_InitIPForInternalTiming;
    IOCTL_XC_SETIPMUX                               fpXC_SetIPMux;
    IOCTL_XC_IS_H_SYNC_ACTIVE                       fpXC_Is_H_Sync_Active;
    IOCTL_XC_IS_V_SYNC_ACTIVE                       fpXC_Is_V_Sync_Active;
    IOCTL_XC_GETAUTOPOSITIONWINDOW                  fpXC_GetAutoPositionWindow;
    IOCTL_XC_SETFRAMEBUFFERADDRESS                  fpXC_SetFrameBufferAddress;
    IOCTL_XC_SETFRCMFRAMEBUFFERADDRESS              fpXC_SetFRCMFrameBufferAddress;
    IOCTL_XC_SETDUALFRAMEBUFFERADDRESS              fpXC_SetDualFrameBufferAddress;
    IOCTL_XC_ENABLEMIUDUALMODE                      fpXC_EnableMiuDualMode;
    IOCTL_XC_ISFRAMEBUFFERENOUGHFORCUSSCALING       fpXC_IsFrameBufferEnoughForCusScaling;
    IOCTL_XC_SETSCALERMEMORYREQUEST                 fpXC_SetScalerMemoryRequest;
    IOCTL_XC_GET_PIXELDATA                          fpXC_Get_PixelData;
    IOCTL_XC_GETAVAILABLESIZE                       fpXC_GetAvailableSize;
    IOCTL_XC_SETFRAMECOLOR                          fpXC_SetFrameColor;
    IOCTL_XC_SETDISPWINDOWCOLOR                     fpXC_SetDispWindowColor;
    IOCTL_XC_SUPPORTSOURCETOVE                      fpXC_SupportSourceToVE;
    IOCTL_XC_SETOUTPUTCAPTURE                       fpXC_SetOutputCapture;
    IOCTL_XC_SETGAMMAONOFF                          fpXC_SetGammaOnOff ;
    IOCTL_XC_SETPREGAMMAGAIN                        fpXC_SetPreGammaGain;
    IOCTL_XC_SETPREGAMMAOFFSET                      fpXC_SetPreGammaOffset;
    IOCTL_XC_SETPANELTIMING                         fpXC_SetPanelTiming;
    IOCTL_XC_SETOUTTIMINGMODE                       fpXC_SetOutTimingMode;
    IOCTL_XC_SETFREERUNTIMING                       fpXC_SetFreeRunTiming;
    IOCTL_XC_SET_CUSTOMERSYNCINFO                   fpXC_Set_CustomerSyncInfo;
    IOCTL_XC_WAITFPLLDONE                           fpXC_WaitFPLLDone;
    IOCTL_XC_GETOUTPUTVFREQX100                     fpXC_GetOutputVFreqX100;
    IOCTL_XC_GETOP1OUTPUTVFREQX100                  fpXC_GetOP1OutputVFreqX100;
    IOCTL_XC_FRAMELOCKCHECK                         fpXC_FrameLockCheck;
    IOCTL_XC_CUSTOMIZEFRCTABLE                      fpXC_CustomizeFRCTable;
    IOCTL_XC_OUTPUTFRAMECTRL                        fpXC_OutputFrameCtrl;
    IOCTL_SC_ENABLE_FPLL_FSM                        fpSC_Enable_FPLL_FSM;
    IOCTL_SC_FORCEFREERUN                           fpSC_ForceFreerun;
    IOCTL_SC_ISFORCEFREERUN                         fpSC_IsForceFreerun;
    IOCTL_SC_SETFREERUNVFREQ                        fpSC_SetFreerunVFreq;
    IOCTL_XC_SETEXPANELINFO                         fpXC_SetExPanelInfo;
    IOCTL_XC_ENABLE_FPLL_THRESH_MODE                fpXC_Enable_FPLL_Thresh_Mode;
    IOCTL_XC_GET_FPLL_THRESH_MODE                   fpXC_Get_FPLL_Thresh_Mode;
    IOCTL_XC_ENABLEIPAUTONOSIGNAL                   fpXC_EnableIPAutoNoSignal;
    IOCTL_XC_GETIPAUTONOSIGNAL                      fpXC_GetIPAutoNoSignal;
    IOCTL_XC_ENABLEIPAUTOCOAST                      fpXC_EnableIPAutoCoast;
    IOCTL_XC_ENABLEIPCOASTDEBOUNCE                  fpXC_EnableIPCoastDebounce;
    IOCTL_XC_CLEARIPCOASTSTATUS                     fpXC_ClearIPCoastStatus;
    IOCTL_XC_ENABLEFPLLMANUALSETTING                fpXC_EnableFpllManualSetting;
    IOCTL_XC_FPLLBOUNDARYTEST                       fpXC_FpllBoundaryTest;
    IOCTL_XC_SETOFFLINEDETECTION                    fpXC_SetOffLineDetection ;
    IOCTL_XC_GETOFFLINEDETECTION                    fpXC_GetOffLineDetection;
    IOCTL_XC_SETOFFLINESOGTHRESHOLD                 fpXC_SetOffLineSogThreshold;
    IOCTL_XC_SETOFFLINESOGBW                        fpXC_SetOffLineSogBW;
    IOCTL_XC_OFFLINEINIT                            fpXC_OffLineInit;
    IOCTL_XC_SET_EXTRA_FETCH_ADV_LINE               fpXC_Set_Extra_fetch_adv_line;
    IOCTL_XC_SETVGASOGEN                            fpXC_SetVGASogEn;
    IOCTL_XC_ENABLEWINDOW                           fpXC_EnableWindow;
    IOCTL_XC_IS_SUBWINDOWEANBLE                     fpXC_Is_SubWindowEanble;
    IOCTL_XC_SETBORDERFORMAT                        fpXC_SetBorderFormat;
    IOCTL_XC_ENABLEBORDER                           fpXC_EnableBorder;
    IOCTL_XC_ZORDERMAINWINDOWFIRST                  fpXC_ZorderMainWindowFirst;
    IOCTL_XC_PQ_LOADFUNCTION                        fpXC_PQ_LoadFunction;
    IOCTL_XC_CHECK_HNONLINEARSCALING                fpXC_Check_HNonLinearScaling;
    IOCTL_XC_ENABLEEUROHDTVSUPPORT                  fpXC_EnableEuroHdtvSupport;
    IOCTL_XC_ENABLEEUROHDTVDETECTION                fpXC_EnableEuroHdtvDetection;
    IOCTL_XC_READBYTE                               fpXC_ReadByte;
    IOCTL_XC_WRITEBYTE                              fpXC_WriteByte;
    IOCTL_XC_WRITEBYTEMASK                          fpXC_WriteByteMask;
    IOCTL_XC_WRITE2BYTEMASK                         fpXC_Write2ByteMask;
    IOCTL_XC_W2BYTE                                 fpXC_W2BYTE;
    IOCTL_XC_R2BYTE                                 fpXC_R2BYTE;
    IOCTL_XC_W4BYTE                                 fpXC_W4BYTE;
    IOCTL_XC_R4BYTE                                 fpXC_R4BYTE;
    IOCTL_XC_R2BYTEMSK                              fpXC_R2BYTEMSK;
    IOCTL_XC_W2BYTEMSK                              fpXC_W2BYTEMSK;
    IOCTL_XC_MLOAD_INIT                             fpXC_MLoad_Init;
    IOCTL_XC_MLOAD_ENABLE                           fpXC_MLoad_Enable;
    IOCTL_XC_MLOAD_CUS_INIT                         fpXC_MLoad_Cus_Init;
    IOCTL_XC_MLOAD_CUS_ENABLE                       fpXC_MLoad_Cus_Enable;
    IOCTL_XC_MLOAD_GETSTATUS                        fpXC_MLoad_GetStatus;
    IOCTL_XC_MLOAD_WRITECMD_AND_FIRE                fpXC_MLoad_WriteCmd_And_Fire;
    IOCTL_XC_MLOAD_WRITECMDS_AND_FIRE               fpXC_MLoad_WriteCmds_And_Fire;
    IOCTL_XC_MLOAD_COMBINE                          fpXC_MLoad_Combine_Enable;
    IOCTL_XC_MLG_INIT                               fpXC_MLG_Init;
    IOCTL_XC_MLG_ENABLE                             fpXC_MLG_Enable;
    IOCTL_XC_MLG_GETCAPS                            fpXC_MLG_GetCaps;
    IOCTL_XC_MLG_GETSTATUS                          fpXC_MLG_GetStatus;
    IOCTL_XC_SETOSD2VEMODE                          fpXC_SetOSD2VEMode;
    IOCTL_XC_IP2_PREFILTER_ENABLE                   fpXC_IP2_PreFilter_Enable;
    IOCTL_XC_GET_PIXEL_RGB                          fpXC_Get_Pixel_RGB;
    IOCTL_XC_KEEPPIXELPOINTERAPPEAR                 fpXC_KeepPixelPointerAppear;
    IOCTL_XC_SET_MEMFMTEX                           fpXC_Set_MemFmtEx;
    IOCTL_XC_ISREQUESTFRAMEBUFFERLESSMODE           fpXC_IsRequestFrameBufferLessMode;
    IOCTL_XC_SKIPSWRESET                            fpXC_SkipSWReset;
    IOCTL_XC_ENABLEREPWINDOWFORFRAMECOLOR           fpXC_EnableRepWindowForFrameColor;
    IOCTL_XC_SETOSDLAYER                            fpXC_SetOSDLayer;
    IOCTL_XC_GETOSDLAYER                            fpXC_GetOSDLayer;
    IOCTL_XC_SETVIDEOALPHA                          fpXC_SetVideoAlpha;
    IOCTL_XC_GETVIDEOALPHA                          fpXC_GetVideoAlpha;
    IOCTL_XC_SKIPWAITVSYNC                          fpXC_SkipWaitVsync;
    IOCTL_XC_SETCMAHEAPID                           fpXC_SetCMAHeapID;
    IOCTL_XC_OP2VOPDESEL                            fpXC_OP2VOPDESel;
    IOCTL_XC_FRC_SETWINDOW                          fpXC_FRC_SetWindow;
    IOCTL_XC_ENABLE_TWOINITFACTOR                   fpXC_Enable_TwoInitFactor;
    IOCTL_XC_ISFIELDPACKINGMODESUPPORTED            fpXC_IsFieldPackingModeSupported;
    IOCTL_XC_PREINIT                                fpXC_PreInit;
    IOCTL_XC_GET_BUFFERDATA                         fpXC_Get_BufferData;
    IOCTL_XC_SET_BUFFERDATA                         fpXC_Set_BufferData;
    IOCTL_XC_ENABLEMAINWINDOW                       fpXC_EnableMainWindow;
    IOCTL_XC_ENABLESUBWINDOW                        fpXC_EnableSubWindow;
    IOCTL_XC_DISABLESUBWINDOW                       fpXC_DisableSubWindow;
    IOCTL_XC_SETPIXELSHIFT                          fpXC_SetPixelShift;
    IOCTL_XC_SETVIDEOONOSD                          fpXC_SetVideoOnOSD;
    IOCTL_XC_SETOSDLAYERBLENDING                    fpXC_SetOSDLayerBlending;
    IOCTL_XC_SETOSDLAYERALPHA                       fpXC_SetOSDLayerAlpha;
    IOCTL_XC_SETOSDBLENDINGFORMULA                  fpXC_SetOSDBlendingFormula;
    IOCTL_XC_REPORTPIXELINFO                        fpXC_ReportPixelInfo;
    IOCTL_XC_SETSCALING                             fpXC_SetScaling;
    IOCTL_XC_SETMCDIBUFFERADDRESS                   fpXC_SetMCDIBufferAddress;
    IOCTL_XC_ENABLEMCDI                             fpXC_EnableMCDI;
    IOCTL_XC_SENDCMDTOFRC                           fpXC_SendCmdToFRC;
    IOCTL_XC_GETMSGFROMFRC                          fpXC_GetMsgFromFRC;
    IOCTL_XC_ENABLERWBANKAUTO                       fpXC_EnableRWBankAuto;
    IOCTL_XC_SETWRBANKMAPPINGNUM                    fpXC_SetWRBankMappingNum;
    IOCTL_XC_GETWRBANKMAPPINGNUM                    fpXC_GetWRBankMappingNum;
    IOCTL_XC_GETWRBANKMAPPINGNUMFORZAP              fpXC_GetWRBankMappingNumForZap;
    IOCTL_XC_SET_BOBMODE                            fpXC_SetBOBMode;
    IOCTL_XC_SET_FORCE_READBANK                     fpXC_SetForceReadBank;
    IOCTL_XC_SET_LOCK_POINT                         fpXC_SetLockPoint;
    IOCTL_XC_LD_INIT                                fpXC_LD_Init;
    IOCTL_XC_LD_SETMEMORYADDRESS                    fpXC_LD_SetMemoryAddress;
    IOCTL_XC_LD_GET_VALUE                           fpXC_LD_Get_Value;
    IOCTL_XC_LD_SETLEVEL                            fpXC_LD_SetLevel;
    IOCTL_SET_TURNOFFLDBL                           fpSet_TurnoffLDBL;
    IOCTL_SET_NOTUPDATESPIDATAFLAGS                 fpSet_notUpdateSPIDataFlags;
    IOCTL_SET_USERMODELDFLAGS                       fpSet_UsermodeLDFlags;
    IOCTL_SET_BLLEVEL                               fpSet_BLLevel;
    IOCTL_XC_SET_BWS_MODE                           fpXC_Set_BWS_Mode;
    IOCTL_XC_FRC_COLORPATHCTRL                      fpXC_FRC_ColorPathCtrl;
    IOCTL_XC_FRC_OP2_SETRGBGAIN                     fpXC_FRC_OP2_SetRGBGain;
    IOCTL_XC_FRC_OP2_SETRGBOFFSET                   fpXC_FRC_OP2_SetRGBOffset;
    IOCTL_XC_FRC_OP2_SETDITHER                      fpXC_FRC_OP2_SetDither;
    IOCTL_XC_FRC_BYPASSMFC                          fpXC_FRC_BypassMFC;
    IOCTL_XC_FRC_MUTE                               fpXC_FRC_Mute;
    IOCTL_XC_FORCEREADFRAME                         fpXC_ForceReadFrame;
    IOCTL_XC_SETCSC                                 fpXC_SetCsc;
    IOCTL_XC_REGISTERPQSETFPLLTHRESHMODE            fpXC_RegisterPQSetFPLLThreshMode;
    IOCTL_XC_GETFREERUNSTATUS                       fpXC_GetFreeRunStatus;
    IOCTL_XC_SET_BYPASS_CSC                         fpXC_BYPASS_SetCSC;
    IOCTL_XC_GET_DSFORCEINDEXSUPPORTED              fpXC_Get_DSForceIndexSupported;
    IOCTL_XC_SET_DSFORCEINDEX                       fpXC_Set_DSForceIndex;
    IOCTL_XC_SET_DSINDEXSOURCESELECT                fpXC_Set_DSIndexSourceSelect;
    IOCTL_XC_GET_DYNAMICSCALINGSTATUS               fpXC_GetDynamicScalingStatus;
    IOCTL_XC_OSDC_INITSETTING                       fpXC_OSDC_InitSetting;
    IOCTL_XC_OSDC_SET_OUTPUTVFREQX10                fpXC_OSDC_SetOutVfreqx10;
    IOCTL_XC_OSDC_CONTROL                           fpXC_OSDC_Control;
    IOCTL_XC_OSDC_GETDSTINFO                        fpXC_OSDC_GetDstInfo;
    IOCTL_XC_SET_POWERSTATE                         fpXC_Set_PowerState;
    IOCTL_XC_SET_BYPASS_OSDVSYNC_POS                fpXC_BYPASS_SetOSDVsyncPos;
    IOCTL_XC_SET_BYPASS_INPUTSRC                    fpXC_BYPASS_SetInputSrc;
    IOCTL_XC_SET_SEAMELESS_ZAPPING                  fpXC_SetSeamlessZapping;
    IOCTL_XC_GET_SEAMELESS_ZAPPING_STATUS           fpXC_GetSeamlessZappingStatus;
    IOCTL_XC_SET_VTRACK_PAYLOADDATA                 fpXC_Vtrack_SetPayloadData;
    IOCTL_XC_SET_VTRACK_USERDEFINED_SETTING         fpXC_Vtrack_SetUserDefindedSetting;
    IOCTL_XC_SET_VTRACK_ENABLE                      fpXC_Vtrack_Enable;
    IOCTL_XC_PRESET_PQINFO                          fpXC_PreSetPQInfo;
    IOCTL_XC_GET_OP1_TESTPATTERN_ENABLED            fpXC_Is_OP1_TestPattern_Enabled;
    IOCTL_XC_SET_OP1_TESTPATTERN                    fpXC_Set_OP1_TestPattern;
    IOCTL_XC_SET_OP2_TESTPATTERN                    fpXC_Set_OP2_TestPattern;
    IOCTL_XC_CHECK_WHITEBALANCE_PATTERN_MODE        fpXC_Check_WhiteBalance_Pattern_Mode;
    IOCTL_XC_SET_HLINEARSCALING                     fpXC_Set_HLinearScaling;
    IOCTL_XC_ENABLE_T3D                             fpXC_EnableT3D;
    IOCTL_XC_SET_FRC_INPUTTIMING                    fpXC_Set_FRC_InputTiming;
    IOCTL_XC_GET_FRC_INPUTTIMING                    fpXC_Get_FRC_InputTiming;
    IOCTL_XC_GET_VBOX_INFO                          fpXC_Get_VirtualBox_Info;
    IOCTL_XC_SET_OSD_DETECT                         fpXC_Set_OSD_DETECT;
    IOCTL_XC_GET_OSD_DETECT                         fpXC_Get_OSD_DETECT;
    IOCTL_XC_ENABLE_LOCKFREQONLY                    fpXC_Enable_LockFreqOnly;
    IOCTL_XC_IS2K2KTOFRCMODE                        fpXC_Is2K2KToFrcMode;
    IOCTL_XC_SET_XC_VOP_DATA                        fpXC_SetXCVOPdata;
    IOCTL_XC_GETSWDSINDEX                           fpXC_GetSWDSIndex;
    IOCTL_XC_GET_Is_SupportSWDS                     fpXC_Get_Is_SupportSWDS;
    IOCTL_XC_GET_FRCM_FRAMENUM                      fpXC_Get_FRCM_FrameNum;
    IOCTL_XC_SET_PIXELSHIFT_FEATURES                fpXC_SetPixelShiftFeatures;
    IOCTL_XC_SET_BWD_CONFIG                         fpXC_SetBwdConfig;
    IOCTL_XC_CMD_IS_SUPPORT_2_STEP_SCALING          fpXC_IsSupport2StepScaling;
    IOCTL_XC_CMD_GET_PQ_PATH_STATUS                 fpXC_GetPQPathStatus;
    IOCTL_XC_SETFORCEWRITE                          fpXC_SetForceWrite;
    IOCTL_XC_GETFORCEWRITE                          fpXC_GetForceWrite;
#ifdef UFO_XC_SET_DSINFO_V0
    IOCTL_XC_SETDSINFO                              fpXC_SetDSInfo;
#endif
#ifdef UFO_XC_AUTO_DOWNLOAD
    IOCTL_XC_AUTODOWNLOAD_CONFIG_CTRL               fpXC_AutoDownloadConfig;
    IOCTL_XC_AUTODOWNLOAD_WRITE_CTRL                fpXC_AutoDownloadWrite;
    IOCTL_XC_AUTODOWNLOAD_FIRE_CTRL                 fpXC_AutoDownloadFire;
#endif
#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
    IOCTL_XC_HDR_CTRL                               fpXC_HDRControl;
#endif
#endif
    IOCTL_XC_OPTEE_CTRL                             fpXC_OPTEE_Ctrl;
    IOCTL_XC_OPTEE_CHECKRBASE                       fpXC_OPTEE_CheckRBase;
    IOCTL_XC_OPTEE_MUX                              fpXC_OPTEE_Mux;
    IOCTL_XC_GET_RES_BY_PIPE                        fpXC_GetResourceByPipeID;
    IOCTL_XC_CONFIG_PIPE                            fpXC_ConfigPipe;
    IOCTL_XC_CHECK_PIPE                             fpXC_CheckPipe;

    // OBSOLETE FUNCTION POINTER START
    // avoid using these functions pointers!!!!
    IOCTL_SC_GET_OUTPUT_VFREQ                       fpsc_get_output_vfreq;
    IOCTL_XC_GET_CURRENT_OUTPUTVFREQX100            fpXC_Get_Current_OutputVFreqX100;
    IOCTL_XC_SETMEMORYWRITEREQUEST                  fpXC_SetMemoryWriteRequest;
    IOCTL_XC_SET_MEMFMT                             fpXC_Set_MemFmt;
    IOCTL_XC_SETOUTPUTADJUSTSETTING                 fpXC_SetOutputAdjustSetting;
    IOCTL_SCALER_FPLL_FSM                           fsScaler_FPLL_FSM;
    IOCTL_XC_ENABLEMIRRORMODE                       fpXC_EnableMirrorMode;
    IOCTL_XC_ENABLEMIRRORMODE2                      fpXC_EnableMirrorMode2;
    IOCTL_XC_GETMIRRORMODETYPE                      fpXC_GetMirrorModeType;
    // OBSOLETE FUNCTION POINTER END
    // avoid using these functions pointers!!!!
    IOCTL_XC_VIDEO_PLAYBACK_CTRL                    fpXC_VideoPlaybackCtrl;
#ifdef UFO_XC_SETBLACKVIDEOBYMODE
    IOCTL_XC_GENERATEBLACKVIDEOBYMODE               fpXC_GenerateBlackVideoByMode;
#endif

#ifdef UFO_XC_FB_LEVEL
    IOCTL_XC_SET_FB_LEVEL                           fpXC_SetFBLevel;
    IOCTL_XC_GET_FB_LEVEL                           fpXC_GetFBLevel;
#endif
    IOCTL_XC_SET_SWDR_INFO                          fpXC_Set_SWDR_Info;
    IOCTL_XC_GET_SWDR_INFO                          fpXC_Get_SWDR_Info;

// APIXC_ADC_H
    IOCTL_XC_ADC_SETCVBSOUT                         fpXC_ADC_SetCVBSOut;
    IOCTL_XC_ADC_ISCVBSOUTENABLED                   fpXC_ADC_IsCVBSOutEnabled;
    IOCTL_XC_ADC_SETPCCLOCK                         fpXC_ADC_SetPcClock;
    IOCTL_XC_ADC_SETPHASE                           fpXC_ADC_SetPhase;
    IOCTL_XC_ADC_SETPHASEEX                         fpXC_ADC_SetPhaseEx;
    IOCTL_XC_ADC_GETPHASERANGE                      fpXC_ADC_GetPhaseRange;
    IOCTL_XC_ADC_GETPHASE                           fpXC_ADC_GetPhase;
    IOCTL_XC_ADC_GETPHASEEX                         fpXC_ADC_GetPhaseEx;
    IOCTL_XC_ADC_ISSCARTRGB                         fpXC_ADC_IsScartRGB;
    IOCTL_XC_ADC_GETPCCLOCK                         fpXC_ADC_GetPcClock;
    IOCTL_XC_ADC_GETSOGLEVELRANGE                   fpXC_ADC_GetSoGLevelRange;
    IOCTL_XC_ADC_SETSOGLEVEL                        fpXC_ADC_SetSoGLevel;
    IOCTL_XC_ADC_POWEROFF                           fpXC_ADC_PowerOff;
    IOCTL_XC_ADC_GETDEFAULTGAINOFFSET               fpXC_ADC_GetDefaultGainOffset;
    IOCTL_XC_ADC_GETMAXIMALOFFSETVALUE              fpXC_ADC_GetMaximalOffsetValue;
    IOCTL_XC_ADC_GETMAXIMALGAINVALUE                fpXC_ADC_GetMaximalGainValue;
    IOCTL_XC_ADC_GETCENTERGAIN                      fpXC_ADC_GetCenterGain;
    IOCTL_XC_ADC_GETCENTEROFFSET                    fpXC_ADC_GetCenterOffset;
    IOCTL_XC_ADC_SETGAIN                            fpXC_ADC_SetGain;
    IOCTL_XC_ADC_SETOFFSET                          fpXC_ADC_SetOffset;
    IOCTL_XC_ADC_ADJUSTGAINOFFSET                   fpXC_ADC_AdjustGainOffset;
    IOCTL_XC_ADC_SOURCE_CALIBRATE                   fpXC_ADC_Source_Calibrate;
    IOCTL_XC_ADC_SETSOGCAL                          fpXC_ADC_SetSoGCal;
    IOCTL_XC_ADC_SETRGB_PIPE_DELAY                  fpXC_ADC_SetRGB_PIPE_Delay;
    IOCTL_XC_ADC_SCARTRGB_SOG_CLAMPDELAY            fpXC_ADC_ScartRGB_SOG_ClampDelay;
    IOCTL_XC_ADC_SET_YPBPRLOOSELPF                  fpXC_ADC_Set_YPbPrLooseLPF;
    IOCTL_XC_ADC_SET_SOGBW                          fpXC_ADC_Set_SOGBW;
    IOCTL_XC_ADC_SETCLAMPDURATION                   fpXC_ADC_SetClampDuration;
    IOCTL_XC_ADC_ENABLEHWCALIBRATION                fpXC_ADC_EnableHWCalibration;
    IOCTL_XC_ADC_SETIDACCURRENTMODE                 fpXC_ADC_SetIdacCurrentMode;
    IOCTL_XC_ADC_GETIDACCURRENTMODE                 fpXC_ADC_GetIdacCurrentMode;

// APIXC_AUTO_H
    IOCTL_XC_AUTO_GEOMETRY                          fpXC_Auto_Geometry;
    IOCTL_XC_AUTO_GEOMETRY_EX                       fpXC_Auto_Geometry_Ex;
    IOCTL_XC_AUTO_STOPAUTOGEOMETRY                  fpXC_Auto_StopAutoGeometry;
    IOCTL_XC_AUTO_GAINOFFSET                        fpXC_Auto_GainOffset;
    IOCTL_XC_AUTO_GETHWFIXEDGAINOFFSET              fpXC_Auto_GetHWFixedGainOffset;
    IOCTL_XC_AUTO_SETVALIDDATA                      fpXC_Auto_SetValidData;
    IOCTL_XC_AUTO_AUTOOFFSET                        fpXC_Auto_AutoOffset;
    IOCTL_XC_AUTO_DETECTWIDTH                       fpXC_Auto_DetectWidth;
    IOCTL_XC_AUTO_SETCALIBRATIONMODE                fpXC_Auto_SetCalibrationMode;
    IOCTL_XC_AUTO_GETCALIBRATIONMODE            fpXC_Auto_GetCalibrationMode;
    IOCTL_XC_AUTO_GETSYNCINFO                       fpXC_Auto_GetSyncInfo;

// APIXC_MODEPARSE_H
    IOCTL_XC_MODEPARSE_INIT                         fpXC_ModeParse_Init;
    IOCTL_XC_MODEPARSE_MATCHMODE                    fpXC_ModeParse_MatchMode;
    IOCTL_XC_MODEPARSE_MATCHMODEEX                  fpXC_ModeParse_MatchModeEx;

// APIXC_PCMONITOR_H
    IOCTL_XC_PCMONITOR_INIT                         fpXC_PCMonitor_Init;
    IOCTL_XC_PCMONITOR_RESTART                      fpXC_PCMonitor_Restart;
    IOCTL_XC_PCMONITOR_SETTIMINGCOUNT               fpXC_PCMonitor_SetTimingCount;
    IOCTL_XC_PCMONITOR                              fpXC_PCMonitor;
    IOCTL_XC_PCMONITOR_GETCURRENTSTATE              fpXC_PCMonitor_GetCurrentState;
    IOCTL_XC_PCMONITOR_GETSYNCSTATUS                fpXC_PCMonitor_GetSyncStatus;
    IOCTL_XC_PCMONITOR_GET_HFREQX10                 fpXC_PCMonitor_Get_HFreqx10;
    IOCTL_XC_PCMONITOR_GET_HFREQX1K                 fpXC_PCMonitor_Get_HFreqx1K;
    IOCTL_XC_PCMONITOR_GET_VFREQX10                 fpXC_PCMonitor_Get_VFreqx10;
    IOCTL_XC_PCMONITOR_GET_VFREQX1K                 fpXC_PCMonitor_Get_VFreqx1K;
    IOCTL_XC_PCMONITOR_GET_VTOTAL                   fpXC_PCMonitor_Get_Vtotal;
    IOCTL_XC_PCMONITOR_GET_DVI_HDMI_DE_INFO         fpXC_PCMonitor_Get_Dvi_Hdmi_De_Info;
    IOCTL_XC_PCMONITOR_SYNCLOSS                     fpXC_PCMonitor_SyncLoss;
    IOCTL_XC_PCMONITOR_INVALIDTIMINGDETECT          fpXC_PCMonitor_InvalidTimingDetect;
    IOCTL_XC_PCMONITOR_SETTIMINGCOUNTEX             fpXC_PCMonitor_SetTimingCountEx;

// DRVXC_HDMI_IF_H
    IOCTL_HDMI_FUNC_CAPS                            fpHDMI_Func_Caps;
    IOCTL_HDMI_INIT                                 fpHDMI_init;
    IOCTL_HDMI_EXIT                                 fpHDMI_Exit;
    IOCTL_HDMI_PKT_RESET                            fpHDMI_pkt_reset;
    IOCTL_HDMI_PULLHPD                              fpHDMI_pullhpd;
    IOCTL_HDMI_GC_INFO                              fpHDMI_GC_Info;
    IOCTL_HDMI_PACKET_RECEIVED                      fpHDMI_Packet_Received;
    IOCTL_HDMI_GET_COLORFORMAT                      fpHDMI_Get_ColorFormat;
    IOCTL_HDMI_GET_COLORRANGE                       fpHDMI_Get_ColorRange;
    IOCTL_HDMI_SET_EQ                               fpHDMI_Set_EQ;
    IOCTL_HDMI_SET_EQ_TO_PORT                       fpHDMI_Set_EQ_To_Port;
    IOCTL_HDMI_AUDIO_MUTE_ENABLE                    fpHDMI_Audio_MUTE_Enable;
    IOCTL_HDMI_AUDIO_STATUS_CLEAR                   fpHDMI_Audio_Status_Clear;
    IOCTL_HDMI_GET_ASPECTRATIO                      fpHDMI_Get_AspectRatio;
    IOCTL_HDMI_GET_ACTIVEFORMAT_ASPECTRATIO         fpHDMI_Get_ActiveFormat_AspectRatio;
    IOCTL_HDMI_GET_AVIINFOFRAMEVER                  fpHDMI_Get_AVIInfoFrameVer;
    IOCTL_HDMI_ERR_STATUS_UPDATE                    fpHDMI_err_status_update;
    IOCTL_HDMI_GET_TMDS_FREQ                        fpHDMI_Get_TMDS_freq;
    IOCTL_HDMI_GET_POLLINGSTATUS                    fpHDMI_Get_PollingStatus;
    IOCTL_HDMI_ISHDMI_MODE                          fpHDMI_IsHDMI_Mode;
    IOCTL_HDMI_GET_MID_INFO                         fpHDMI_Get_MID_info;
    IOCTL_HDMI_GET_PACKET_VALUE                     fpHDMI_get_packet_value;
    IOCTL_DVI_CHANNELPHASESTATUS                    fpDVI_ChannelPhaseStatus;
    IOCTL_DVI_SOFTWARERESET                         fpDVI_SoftwareReset;
    IOCTL_DVI_RESET                                 fpdvi_reset;
    IOCTL_HDMI_AVIINFO_ACTIVEINFOPRESENT            fpHDMI_Get_AVIInfoActiveInfoPresent;
    IOCTL_DVI_CLKPULLLOW                            fpDVI_ClkPullLow;
    IOCTL_DVI_SWITCHSRC                             fpDVI_SwitchSrc;
    IOCTL_HDMI_SETFORCLOCKLESSTHAN70MHZ             fpHDMI_SetForClockLessThan70Mhz;
    IOCTL_HDMI_DVI_ADJUST                           fpHDMI_dvi_adjust;
    IOCTL_HDMI_SETUPDATEPHASELINECOUNT              fpHDMI_SetUpdatePhaseLineCount;
    IOCTL_HDCP_ENABLE                               fpHDCP_Enable;
    IOCTL_HDMI_SETHDCPENABLE                        fpHDMI_SetHdcpEnable;
    IOCTL_HDCP_CLEARSTATUS                          fpHDCP_ClearStatus;
    IOCTL_HDCP_INITPRODUCTIONKEY                    fpHDCP_initproductionkey;
    IOCTL_HDCP_GETSTATUS                            fpHDCP_GetStatus;
    IOCTL_HDCP_VSYNC_END_EN                         fpHDCP_Vsync_end_en;
    IOCTL_HDMI_AUDIO_OUTPUT                         fpHDMI_audio_output;
    IOCTL_HDMI_AUDIO_CP_HDR_INFO                    fpHDMI_audio_cp_hdr_info;
    IOCTL_HDMI_AUDIO_CHANNEL_STATUS                 fpHDMI_audio_channel_status;
    IOCTL_HDMI_GETLIBVER                            fpHDMI_GetLibVer;
    IOCTL_HDMI_GETINFO                              fpHDMI_GetInfo;
    IOCTL_HDMI_GETSTATUS                            fpHDMI_GetStatus;
    IOCTL_HDMI_READ_DDCRAM                          fpHDMI_READ_DDCRAM;
    IOCTL_HDMI_PROG_DDCRAM                          fpHDMI_PROG_DDCRAM;
    IOCTL_HDMI_GET_CONTENT_TYPE                     fpHDMI_Get_Content_Type;
    IOCTL_HDMI_GET_EXT_COLORIMETRY                  fpHDMI_Get_Ext_Colorimetry;
    IOCTL_HDMI_GET_PIXEL_REPETITION                 fpHDMI_Get_Pixel_Repetition;
    IOCTL_HDMI_CHECK_4K2K                           fpHDMI_Check4K2K;
    IOCTL_HDMI_3D_4KX2K_PROCESS                     fpHDMI_3D_4Kx2K_Process;
    IOCTL_HDMI_AVG_SCALERINGDOWN                    fpHDMI_AVG_ScaleringDown;
    IOCTL_HDMI_CHECK_ADDITIONAL_FORMAT              fpHDMI_Check_Additional_Format;
    IOCTL_HDMI_GET_3D_STRUCTURE                     fpHDMI_Get_3D_Structure;
    IOCTL_HDMI_GET_3D_EXT_DATA                      fpHDMI_Get_3D_Ext_Data;
    IOCTL_HDMI_GET_3D_META_FIELD                    fpHDMI_Get_3D_Meta_Field;
    IOCTL_HDMI_GET_VIC_CODE                         fpHDMI_Get_VIC_Code;
    IOCTL_HDMI_GET_4KX2K_VIC_CODE                   fpHDMI_Get_4Kx2K_VIC_Code;
    IOCTL_HDMI_ARC_PINCONTROL                       fpHDMI_ARC_PINControl;
    IOCTL_DVI_SOFTWARE_RESET                        fpDVI_Software_Reset;
    IOCTL_HDMI_SET_POWERSTATE                       fpHDMI_Set_PowerState;
    IOCTL_HDMI_DVI_HF_ADJUST                        fpHDMI_dvi_hf_adjust;
    IOCTL_HDMI_CHECKHDMI20_SETTING                  fpHDMI_checkhdmi20_setting;
    IOCTL_HDMI_SET_5V_DETECT_GPIO_SELECT            fpHDMI_Set5VDetectGPIOSelect;
    IOCTL_HDMI_GET_DE_STABLE_STATUS                 fpHDMI_GetDEStableStatus;
    IOCTL_HDMI_CTRL                                 fpHDMI_Ctrl;
    IOCTL_HDMI_GET_DATA_INFO                        fpHDMI_GetDataInfo;
    IOCTL_DUMP_SHM                                  fpXC_dump_shm;

#ifdef UFO_XC_VMARK
    IOCTL_XC_VMARK_SET_PARAMETERS                   fpXC_VMark_SetParameters;
#endif
    IOCTL_SWDS_Fire                                 fpXC_swds_fire;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
    IOCTL_XC_ENABLE_DUAL_MODE                       fpXC_enableDualMode;
#endif

#ifdef UFO_XC_GET_3D_FORMAT
    IOCTL_XC_GET3DFORMAT                        fpXC_Get3DFormat;
#endif

#ifdef UFO_XC_SUPPORT_HDMI_DOWNSCALE_OUTPUT_POLICY
    IOCTL_XC_GET_HDMI_POLICY                        fpXC_GetHDMIPolicy;
#endif
} XC_INSTANCE_PRIVATE;

// APIXC_H
E_APIXC_ReturnValue MApi_XC_GetLibVer_U2(void* pInstance, const MSIF_Version **ppVersion);
XC_ApiInfo * MApi_XC_GetInfo_U2(void* pInstance);
MS_BOOL MApi_XC_GetStatus_U2(void* pInstance, XC_ApiStatus *pDrvStatus, SCALER_WIN eWindow);
MS_BOOL MApi_XC_GetStatusNodelay_U2(void* pInstance, ST_XC_APISTATUSNODELAY *pDrvStatus, SCALER_WIN eWindow);
MS_U16  MApi_XC_GetStatusEx_U2(void* pInstance, XC_ApiStatusEx *pDrvStatusEx, SCALER_WIN eWindow);
MS_BOOL MApi_XC_SetDbgLevel_U2(void* pInstance, MS_U16 u16DbgSwitch);
void    MApi_XC_FPLLCustomerMode_U2(void* pInstance, EN_FPLL_MODE eMode, EN_FPLL_FLAG eFlag, MS_U32 u32Data);
MS_BOOL MDrv_XC_SetIOMapBase_U2(void* pInstance);
MS_BOOL MApi_XC_Init_U2(void* pInstance, XC_INITDATA *pXC_InitData, MS_U32 u32InitDataLen);
E_APIXC_ReturnValue MApi_XC_GetConfig_U2(void* pInstance, XC_INITDATA *pXC_InitData);
E_APIXC_ReturnValue MApi_XC_Init_MISC_U2(void* pInstance, XC_INITMISC *pXC_Init_Misc, MS_U32 u32InitMiscDataLen);
E_APIXC_ReturnValue MApi_XC_GetMISCStatus_U2(void* pInstance, XC_INITMISC *pXC_Init_Misc);
MS_U32  MApi_XC_GetCapability_U2(void* pInstance, MS_U32 u32Id);
E_APIXC_ReturnValue MApi_XC_GetChipCaps_U2(void* pInstance, E_XC_CAPS eCapType, MS_U32* pRet, MS_U32 ret_size);
MS_BOOL MApi_XC_Exit_U2(void* pInstance);
MS_BOOL MApi_XC_SetDynamicScaling_U2(void* pInstance, XC_DynamicScaling_Info *pstDSInfo, MS_U32 u32DSInfoLen, SCALER_WIN eWindow);
void    MApi_XC_Set_DynamicScalingFlag_U2(void *pInstance,MS_BOOL bEnable);
MS_PHY  MApi_XC_Get_DNRBaseOffset_U2(void* pInstance, SCALER_WIN eWindow);
MS_U8   MApi_XC_Get_FrameNumFactor_U2(void* pInstance, SCALER_WIN eWindow);
MS_BOOL MApi_XC_SetWindow_U2(void* pInstance, XC_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, SCALER_WIN eWindow);
MS_BOOL MApi_XC_SetDualWindow_U2(void* pInstance, XC_SETWIN_INFO *pstXC_SetWin_Info_Main, XC_SETWIN_INFO *pstXC_SetWin_Info_Sub);
MS_BOOL MApi_XC_SetTravelingWindow_U2(void* pInstance, XC_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, SCALER_WIN eWindow);
void    MApi_XC_SetInputSource_U2(void* pInstance,  INPUT_SOURCE_TYPE_t enInputSourceType, SCALER_WIN eWindow );
MS_BOOL MApi_XC_IsYUVSpace_U2(void* pInstance, SCALER_WIN eWindow);
MS_BOOL MApi_XC_IsMemoryFormat422_U2(void* pInstance, SCALER_WIN eWindow);
void    MApi_XC_EnableForceRGBin_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
MS_BOOL    MApi_XC_EnableMirrorModeEx_U2(void* pInstance,  MirrorMode_t eMirrorMode,  SCALER_WIN eWindow);
MirrorMode_t MApi_XC_GetMirrorModeTypeEx_U2(void* pInstance, SCALER_WIN eWindow);
void    MApi_XC_GetSyncStatus_U2(void* pInstance, INPUT_SOURCE_TYPE_t eCurrentSrc , XC_IP_SYNC_STATUS *sXC_Sync_Status, SCALER_WIN eWindow);
MS_BOOL MApi_XC_ConfigCMA_U2(void *pInstance, XC_CMA_CONFIG *pstXC_CMA_Config, XC_CMA_CLIENT enCmaClient, MS_U32 u32DataLen, SCALER_WIN eWindow);
MS_U8   MApi_XC_WaitOutputVSync_U2(void* pInstance, MS_U8 u8NumVSyncs, MS_U16 u16Timeout, SCALER_WIN eWindow);
MS_U8   MApi_XC_WaitInputVSync_U2(void* pInstance, MS_U8 u8NumVSyncs, MS_U16 u16Timeout, SCALER_WIN eWindow);
void    MApi_XC_SetHdmiSyncMode_U2(void* pInstance, E_HDMI_SYNC_TYPE esynctype);
E_HDMI_SYNC_TYPE MApi_XC_GetHdmiSyncMode_U2(void* pInstance);
void    MApi_XC_SetRepWindow_U2(void* pInstance, MS_BOOL bEnable,MS_WINDOW_TYPE Window,MS_U8 u8Color);
void    MApi_XC_SkipOPWriteOffInSetWindow_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
MS_BOOL MApi_XC_GetSkipOPWriteOffInSetWindow_U2(void* pInstance, SCALER_WIN eWindow);
void    MApi_XC_SetSkipDisableOPWriteOffInFPLL_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
void    MApi_XC_Set_OPWriteOffEnable_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
void    MApi_XC_Set_OPWriteOffEnableToReg_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
void    MApi_XC_ForceSet_OPWriteOffEnable_U2(void* pInstance, MS_BOOL bEnable ,SCALER_WIN eWindow);
MS_BOOL MApi_XC_Get_OPWriteOffEnable_U2(void* pInstance, SCALER_WIN eWindow);
void    MApi_XC_SetDispWinToReg_U2(void* pInstance, MS_WINDOW_TYPE *pstDspwin, SCALER_WIN eWindow);
void    MApi_XC_GetDispWinFromReg_U2(void* pInstance, MS_WINDOW_TYPE *pstDspwin, SCALER_WIN eWindow);
void    MApi_XC_FreezeImg_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
MS_BOOL MApi_XC_IsFreezeImg_U2(void* pInstance, SCALER_WIN eWindow);
void    MApi_XC_GenerateBlackVideoForBothWin_U2(void* pInstance,  MS_BOOL bEnable);
void    MApi_XC_Set_BLSK_U2(void* pInstance, MS_BOOL bEnable);
void    MApi_XC_GenerateBlackVideo_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
MS_BOOL MApi_XC_IsBlackVideoEnable_U2(void* pInstance, SCALER_WIN eWindow);
void    MApi_XC_EnableFrameBufferLess_U2(void* pInstance, MS_BOOL bEnable);
MS_BOOL MApi_XC_IsCurrentFrameBufferLessMode_U2(void* pInstance);
void    MApi_XC_EnableRequest_FrameBufferLess_U2(void* pInstance, MS_BOOL bEnable);
MS_BOOL MApi_XC_IsCurrentRequest_FrameBufferLessMode_U2(void* pInstance);
MS_U16  MApi_XC_Get_3D_HW_Version_U2(void* pInstance);
MS_BOOL MApi_XC_Get_3D_IsSupportedHW2DTo3D_U2(void* pInstance);
MS_BOOL MApi_XC_Set_3D_Mode_U2(void* pInstance, E_XC_3D_INPUT_MODE e3dInputMode, E_XC_3D_OUTPUT_MODE e3dOutputMode, E_XC_3D_PANEL_TYPE e3dPanelType, SCALER_WIN eWindow);
MS_BOOL MApi_XC_Set_3D_MainWin_FirstMode_U2(void* pInstance, MS_BOOL bMainFirst);
MS_BOOL MApi_XC_Set_3D_LR_Frame_Exchg_U2(void* pInstance, SCALER_WIN eWindow);
MS_BOOL MApi_XC_3D_Is_LR_Frame_Exchged_U2(void* pInstance, SCALER_WIN eWindow);
E_XC_3D_INPUT_MODE MApi_XC_Get_3D_Input_Mode_U2(void* pInstance, SCALER_WIN eWindow);
E_XC_3D_OUTPUT_MODE MApi_XC_Get_3D_Output_Mode_U2(void* pInstance);
#ifdef UFO_XC_GET_3D_FORMAT
MS_BOOL MApi_XC_Get3DFormat_U2(void* pInstance, E_XC_3D_ATTRIBUTE_TYPE e3DAttrType, void* para, void* p3DFormat);
#endif
E_XC_3D_PANEL_TYPE MApi_XC_Get_3D_Panel_Type_U2(void* pInstance);
MS_BOOL MApi_XC_Get_3D_MainWin_First_U2(void* pInstance);
MS_BOOL MApi_XC_3DMainSub_IPSync_U2(void* pInstance);
MS_BOOL MApi_XC_Set_3D_VerVideoOffset_U2(void* pInstance, MS_U16 u163DVerVideoOffset);
MS_U16  MApi_XC_Get_3D_VerVideoOffset_U2(void* pInstance);
MS_BOOL MApi_XC_Is3DFormatSupported_U2(void* pInstance, E_XC_3D_INPUT_MODE e3dInputMode,E_XC_3D_OUTPUT_MODE e3dOutputMode);
MS_BOOL MApi_XC_Set_3D_HShift_U2(void* pInstance, MS_U16 u16HShift);
MS_BOOL MApi_XC_Enable_3D_LR_Sbs2Line_U2(void* pInstance, MS_BOOL bEnable);
MS_U16  MApi_XC_Get_3D_HShift_U2(void* pInstance);
MS_BOOL MApi_XC_Set_3D_HW2DTo3D_Buffer_U2(void* pInstance, MS_PHY u32HW2DTO3D_DD_Buf, MS_PHY u32HW2DTO3D_DR_Buf);
MS_BOOL MApi_XC_Set_3D_HW2DTo3D_Parameters_U2(void* pInstance, MS_XC_3D_HW2DTO3D_PARA st3DHw2DTo3DPara);
MS_BOOL MApi_XC_Get_3D_HW2DTo3D_Parameters_U2(void* pInstance, MS_XC_3D_HW2DTO3D_PARA *pst3DHw2DTo3DPara);
MS_BOOL MApi_XC_Set_3D_Detect3DFormat_Parameters_U2(void* pInstance, MS_XC_3D_DETECT3DFORMAT_PARA *pstDetect3DFormatPara);
MS_BOOL MApi_XC_Get_3D_Detect3DFormat_Parameters_U2(void* pInstance, MS_XC_3D_DETECT3DFORMAT_PARA *pstDetect3DFormatPara);
E_XC_3D_INPUT_MODE MApi_XC_Detect3DFormatByContent_U2(void* pInstance, SCALER_WIN eWindow);
MS_BOOL MApi_XC_DetectNL_U2(void* pInstance, SCALER_WIN eWindow, ST_DETECTNL_PARA* pstDetectNLatticePara);
MS_BOOL MApi_XC_3D_PostPQSetting_U2(void* pInstance, SCALER_WIN eWindow);
MS_BOOL MApi_XC_Set_3D_FPInfo_U2(void* pInstance, MS_XC_3D_FPINFO_PARA *pstFPInfoPara);
MS_BOOL MApi_XC_EnableAutoDetect3D_U2(void* pInstance, MS_BOOL bEnable, E_XC_3D_AUTODETECT_METHOD enDetectMethod);
MS_BOOL MApi_XC_GetAutoDetect3DFlag_U2(void* pInstance, E_XC_3D_AUTODETECT_METHOD *penDetectMethod, MS_BOOL *pbEnable);
MS_BOOL MApi_XC_Set_3D_SubWinClk_U2(void* pInstance);
MS_BOOL MApi_XC_3D_Is_LR_Sbs2Line_U2(void* pInstance);
MS_BOOL MApi_SC_3D_Is_Skip_Default_LR_Flag_U2(void* pInstance);
MS_BOOL MApi_XC_3D_Enable_Skip_Default_LR_Flag_U2(void* pInstance, MS_BOOL bEnable);
void    MApi_XC_Mux_Init_U2(void* pInstance, void (*input_source_to_input_port)(INPUT_SOURCE_TYPE_t src_ids , E_MUX_INPUTPORT* port_ids , MS_U8* u8port_count ) );
void    MApi_XC_Mux_SourceMonitor_U2(void* pInstance, MS_BOOL bRealTimeMonitorOnly);
MS_S16  MApi_XC_Mux_CreatePath_U2(void* pInstance, XC_MUX_PATH_INFO* Path_Info, MS_U32 u32InitDataLen);
MS_S16  MApi_XC_Mux_DeletePath_U2(void* pInstance, INPUT_SOURCE_TYPE_t src, E_DEST_TYPE dest);
MS_S16  MApi_XC_Mux_EnablePath_U2(void* pInstance, MS_U16 PathId);
void    MApi_XC_Mux_TriggerPathSyncEvent_U2(void* pInstance,  INPUT_SOURCE_TYPE_t src , void* para);
void    MApi_XC_Mux_TriggerDestOnOffEvent_U2(void* pInstance,  INPUT_SOURCE_TYPE_t src , void* para);
MS_S16  MApi_XC_Mux_OnOffPeriodicHandler_U2(void* pInstance,  INPUT_SOURCE_TYPE_t src, MS_BOOL bEnable);
MS_U8   MApi_XC_Mux_GetPathInfo_U2(void* pInstance, XC_MUX_PATH_INFO* Paths);
E_APIXC_ReturnValue MApi_XC_Mux_SetSupportMhlPathInfo_U2(void* pInstance, MS_U8 u8MhlSupportInfo);
E_APIXC_ReturnValue MApi_XC_Mux_SetMhlHotPlugInverseInfo_U2(void* pInstance, MS_BOOL bIsMhlHotPlugInverse);
E_MUX_INPUTPORT MApi_XC_Mux_GetHDMIPort_U2(void* pInstance,  INPUT_SOURCE_TYPE_t src );
void MApi_XC_Mux_GetPortMappingMatrix_U2(void *pInstance,XC_MUX_INPUTSRCTABLE *mapping_tab,MS_U32 length);
MS_U8   MApi_XC_MUX_MapInputSourceToVDYMuxPORT_U2(void* pInstance,  INPUT_SOURCE_TYPE_t u8InputSourceType );
void    MApi_XC_Set_NR_U2(void* pInstance, MS_BOOL bEn, SCALER_WIN eWindow);
void    MApi_XC_FilmMode_P_U2(void* pInstance);
MS_BOOL MApi_XC_GetUCEnabled_U2(void* pInstance, SCALER_WIN eWindow);
void    MApi_XC_GenSpecificTiming_U2(void* pInstance, XC_Internal_TimingType timingtype);
MS_BOOL MApi_XC_GetDEBypassMode_U2(void* pInstance, SCALER_WIN eWindow);
void    MApi_XC_GetDEWindow_U2(void* pInstance, MS_WINDOW_TYPE *psWin, SCALER_WIN eWindow);
void    MApi_XC_GetDEWidthHeightInDEByPassMode_U2(void* pInstance, MS_U16* pu16Width,MS_U16* pu16Height ,SCALER_WIN eWindow);
void    MApi_XC_GetCaptureWindow_U2(void* pInstance, MS_WINDOW_TYPE* capture_win, SCALER_WIN eWindow);
void    MApi_XC_SetCaptureWindowVstart_U2(void* pInstance, MS_U16 u16Vstart , SCALER_WIN eWindow);
void    MApi_XC_SetCaptureWindowHstart_U2(void* pInstance, MS_U16 u16Hstart , SCALER_WIN eWindow);
void    MApi_XC_SetCaptureWindowVsize_U2(void* pInstance, MS_U16 u16Vsize , SCALER_WIN eWindow);
void    MApi_XC_SetCaptureWindowHsize_U2(void* pInstance, MS_U16 u16Hsize , SCALER_WIN eWindow);
void    MApi_XC_SoftwareReset_U2(void* pInstance, MS_U8 u8Reset, SCALER_WIN eWindow);
MS_U16  MApi_XC_CalculateHFreqx10_U2(void* pInstance, MS_U16 u16HPeriod);
MS_U32 MApi_XC_CalculateHFreqx1K_U2(void* pInstance, MS_U16 u16HPeriod);
MS_U16  MApi_XC_CalculateVFreqx10_U2(void* pInstance, MS_U16 u16HFreq, MS_U16 u16VTotal);
MS_U32 MApi_XC_CalculateVFreqx1K_U2(void* pInstance, MS_U32 u16HFreq, MS_U16 u16VTotal);
MS_U32 MApi_XC_GetAccurateVFreqx1K_U2(void* pInstance, SCALER_WIN eWindow);
MS_BOOL MApi_XC_InterruptAttach_U2(void* pInstance, SC_INT_SRC enIntNum, SC_InterruptCb pIntCb, void * pParam);
MS_BOOL MApi_XC_InterruptDeAttach_U2(void* pInstance, SC_INT_SRC enIntNum, SC_InterruptCb pIntCb, void * pParam);
void    MApi_XC_DisableInputSource_U2(void* pInstance, MS_BOOL bDisable, SCALER_WIN eWidnow);
MS_BOOL MApi_XC_IsInputSourceDisabled_U2(void* pInstance, SCALER_WIN eWindow);
void    MApi_XC_ChangePanelType_U2(void* pInstance, XC_PANEL_INFO *pstPanelInfo);
MS_U16  MApi_XC_GetCurrentReadBank_U2(void* pInstance, SCALER_WIN eWindow);
MS_U16  MApi_XC_GetCurrentWriteBank_U2(void* pInstance, SCALER_WIN eWindow);
void    MApi_XC_SetAutoPreScaling_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
MS_U8   MApi_XC_GetVSyncWidth_U2(void* pInstance, SCALER_WIN eWindow);
MS_BOOL MApi_XC_set_GOP_Enable_U2(void* pInstance, MS_U8 MaxGOP, MS_U8 UseNum, MS_U8 u8MuxNum, MS_BOOL bEnable);
void    MApi_XC_ip_sel_for_gop_U2(void* pInstance, MS_U8 u8MuxNum , MS_XC_IPSEL_GOP ipSelGop);
void    MApi_XC_SetVOPNBL_U2(void* pInstance);
MS_BOOL MApi_XC_GetDstInfo_U2(void* pInstance, MS_XC_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo, EN_GOP_XCDST_TYPE XCDstType);
void    MApi_XC_Set_FD_Mask_ByWin_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
MS_BOOL MApi_XC_Get_FD_Mask_ByWin_U2(void* pInstance, SCALER_WIN eWindow);
MS_BOOL MApi_XC_Get_FD_Mask_Status_U2(void* pInstance, SCALER_WIN eWindow);
void    MApi_XC_SetIP1TestPattern_U2(void* pInstance, MS_U8 u8Enable, MS_U16 u6Pattern_type, SCALER_WIN eWindow);
#ifdef UFO_XC_TEST_PATTERN
void    MApi_XC_GenerateTestPattern_U2(void* pInstance,EN_XC_TEST_PATTERN_MODE ePatternMode,void* para, MS_U16 u16Length);
#endif
void    MApi_XC_InitIPForInternalTiming_U2(void* pInstance, XC_Internal_TimingType timingtype);
void    MApi_XC_SetIPMux_U2(void* pInstance, MS_U8 u8Val);
MS_BOOL MApi_XC_Is_H_Sync_Active_U2(void* pInstance, SCALER_WIN eWindow);
MS_BOOL MApi_XC_Is_V_Sync_Active_U2(void* pInstance, SCALER_WIN eWindow);
MS_BOOL MApi_XC_GetAutoPositionWindow_U2(void* pInstance, MS_U8 u8ValidData, SCALER_WIN eWindow, MS_WINDOW_TYPE *pstAutoPositionWindow);
void    MApi_XC_SetFrameBufferAddress_U2(void* pInstance, MS_PHY u32FBAddress, MS_PHY u32FBSize, SCALER_WIN eWindow);
void    MApi_XC_SetFRCMFrameBufferAddress_U2(void* pInstance, MS_PHY u32FBAddress, MS_PHY u32FBSize, SCALER_WIN eWindow);
MS_BOOL MApi_XC_SetDualFrameBufferAddress_U2(void* pInstance, MS_PHY u32FBAddress, MS_PHY u32FBSize, SCALER_WIN eWindow);
#ifdef UFO_XC_SUPPORT_DUAL_MIU
MS_BOOL MApi_XC_EnableMiuDualMode_U2(void* pInstance, MS_BOOL bEnable, MS_U32 u32LeftFactor, MS_U32 u32RightFactor, SCALER_WIN eWindow);
#endif
MS_BOOL MApi_XC_IsFrameBufferEnoughForCusScaling_U2(void* pInstance, XC_SETWIN_INFO *pstXC_SetWin_Info, SCALER_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_SetScalerMemoryRequest_U2(void* pInstance, MS_BOOL bEnable, E_XC_MEMORY_REQUEST_TYPE eType, SCALER_WIN eWindow);
void    MApi_XC_Get_PixelData_U2(void* pInstance, MS_U16 u16CorX,  MS_U16 u16CorY, MS_PIXEL_32BIT* pixel);
MS_U32  MApi_XC_GetAvailableSize_U2(void* pInstance, SCALER_WIN eWindow, MS_U8 u8FBNum, MS_U32 u32InputSize);
void    MApi_XC_SetFrameColor_U2(void* pInstance, MS_U32 u32aRGB);
void    MApi_XC_SetDispWindowColor_U2(void* pInstance, MS_U8 u8Color, SCALER_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_SupportSourceToVE_U2(void* pInstance, MS_U16* pOutputCapability);
E_APIXC_ReturnValue MApi_XC_SetOutputCapture_U2(void* pInstance, MS_BOOL bEnable,E_XC_SOURCE_TO_VE eSourceToVE);
void    MApi_XC_SetGammaOnOff_U2(void* pInstance, MS_BOOL bEnable);
void    MApi_XC_SetPreGammaGain_U2(void* pInstance, SCALER_WIN eWindow, MS_XC_VOP_CHANNEL_t eVop_Channel, MS_U16 u16Val);
void    MApi_XC_SetPreGammaOffset_U2(void* pInstance, SCALER_WIN eWindow, MS_XC_VOP_CHANNEL_t eVop_Channel, MS_U16 u16Val);
void    MApi_XC_SetPanelTiming_U2(void* pInstance, XC_SetTiming_Info *pTimingInfo, SCALER_WIN eWindow);
void    MApi_XC_SetOutTimingMode_U2(void* pInstance, E_XC_PNL_OUT_TIMING_MODE enPnl_Out_Timing_Mode);
void    MApi_XC_SetFreeRunTiming_U2(void* pInstance);
MS_BOOL MApi_XC_Set_CustomerSyncInfo_U2(void* pInstance, MS_U16 u16FrontPorch, MS_U16 u16SyncWidth, MS_U16 u16OutputVfreq);
MS_BOOL MApi_XC_WaitFPLLDone_U2(void* pInstance);
MS_U16  MApi_XC_GetOutputVFreqX100_U2(void* pInstance);
MS_U16  MApi_XC_GetOP1OutputVFreqX100_U2(void* pInstance);
MS_BOOL MApi_XC_SetBwdConfig_U2(void* pInstance, E_XC_BWD_CONFIG_TYPE eType, void *pstParam, SCALER_WIN eWindow);

MS_BOOL MApi_XC_FrameLockCheck_U2(void* pInstance);
E_APIXC_ReturnValue MApi_XC_CustomizeFRCTable_U2(void* pInstance, SC_FRC_SETTING* stFRCTable);
E_APIXC_ReturnValue MApi_XC_OutputFrameCtrl_U2(void* pInstance, MS_BOOL bEnable,XC_OUTPUTFRAME_Info * stOutFrameInfo,SCALER_WIN eWindow);
void    MApi_SC_Enable_FPLL_FSM_U2(void* pInstance, MS_BOOL bTrue);
void    MApi_SC_ForceFreerun_U2(void* pInstance, MS_BOOL bEnable);
MS_BOOL MApi_SC_IsForceFreerun_U2(void* pInstance);
void    MApi_SC_SetFreerunVFreq_U2(void* pInstance, E_VFREQ_SEL VFreq);
MS_BOOL MApi_XC_SetExPanelInfo_U2(void* pInstance, MS_BOOL bEnable, XC_PANEL_INFO_EX *pPanelInfoEx);
void    MApi_XC_Enable_FPLL_Thresh_Mode_U2(void* pInstance, MS_BOOL bEnable);
MS_BOOL MApi_XC_Get_FPLL_Thresh_Mode_U2(void* pInstance);
E_APIXC_ReturnValue MApi_XC_EnableIPAutoNoSignal_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow );
MS_BOOL MApi_XC_GetIPAutoNoSignal_U2(void* pInstance, SCALER_WIN eWindow );
void    MApi_XC_EnableIPAutoCoast_U2(void* pInstance, MS_BOOL bEnable);
void    MApi_XC_EnableIPCoastDebounce_U2(void* pInstance);
void    MApi_XC_ClearIPCoastStatus_U2(void* pInstance);
void    MApi_XC_EnableFpllManualSetting_U2(void* pInstance, MS_BOOL bEnable);
void    MApi_XC_FpllBoundaryTest_U2(void* pInstance, MS_U32 u32FpllSetOffset, MS_U16 u16TestCnt);
void    MApi_XC_SetOffLineDetection_U2(void* pInstance, INPUT_SOURCE_TYPE_t);
MS_U8   MApi_XC_GetOffLineDetection_U2(void* pInstance,  INPUT_SOURCE_TYPE_t);
MS_BOOL MApi_XC_SetOffLineSogThreshold_U2(void* pInstance, MS_U8 u8Threshold);
MS_BOOL MApi_XC_SetOffLineSogBW_U2(void* pInstance, MS_U8 u8BW);
MS_BOOL MApi_XC_OffLineInit_U2(void* pInstance);
void    MApi_XC_Set_Extra_fetch_adv_line_U2(void* pInstance, MS_U8 u8val);
E_APIXC_ReturnValue MApi_XC_SetVGASogEn_U2(void* pInstance, MS_BOOL bVGASogEn);
void    MApi_XC_EnableWindow_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
MS_BOOL MApi_XC_Is_SubWindowEanble_U2(void* pInstance);
void    MApi_XC_SetBorderFormat_U2(void* pInstance,  MS_U8 u8Left, MS_U8 u8Right, MS_U8 u8Up, MS_U8 u8Down, MS_U8 u8color, SCALER_WIN eWindow );
void    MApi_XC_EnableBorder_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
void    MApi_XC_ZorderMainWindowFirst_U2(void* pInstance, MS_BOOL bMainFirst);
void    MApi_XC_PQ_LoadFunction_U2(void* pInstance, PQ_Function_Info* function_Info , MS_U32 u32InitDataLen);
MS_BOOL MApi_XC_Check_HNonLinearScaling_U2(void* pInstance);
void    MApi_XC_EnableEuroHdtvSupport_U2(void* pInstance, MS_BOOL bEnable);
void    MApi_XC_EnableEuroHdtvDetection_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
MS_U8   MApi_XC_ReadByte_U2(void* pInstance, MS_U32 u32Reg);
void    MApi_XC_WriteByte_U2(void* pInstance, MS_U32 u32Reg, MS_U8 u8Val);
void    MApi_XC_WriteByteMask_U2(void* pInstance, MS_U32 u32Reg, MS_U8 u8Val, MS_U8 u8Msk);
void    MApi_XC_Write2ByteMask_U2(void* pInstance, MS_U32 u32Reg, MS_U16 u16val, MS_U16 u16Mask);
void    MApi_XC_W2BYTE_U2(void* pInstance, MS_U32 u32Reg, MS_U16 u16Val);
MS_U16  MApi_XC_R2BYTE_U2(void* pInstance, MS_U32 u32Reg);
void    MApi_XC_W4BYTE_U2(void* pInstance, MS_U32 u32Reg, MS_U32 u32Val);
MS_U32  MApi_XC_R4BYTE_U2(void* pInstance, MS_U32 u32Reg);
MS_U16  MApi_XC_R2BYTEMSK_U2(void* pInstance, MS_U32 u32Reg, MS_U16 u16Mask);
void    MApi_XC_W2BYTEMSK_U2(void* pInstance, MS_U32 u32Reg, MS_U16 u16Val, MS_U16 u16Mask);
void    MApi_XC_MLoad_Init_U2(void* pInstance, MS_PHY PhyAddr, MS_U32 u32BufByteLen);
void    MApi_XC_MLoad_Enable_U2(void* pInstance, MS_BOOL bEnable);
void    MApi_XC_MLoad_Cus_Init_U2(void* pInstance, EN_MLOAD_CLIENT_SELECT eMloadSelect, MS_PHY PhyAddr, MS_U32 u32BufByteLen);
void    MApi_XC_MLoad_Cus_Enable_U2(void* pInstance, EN_MLOAD_CLIENT_SELECT eMloadSelect, MS_BOOL bEnable);
MLOAD_TYPE MApi_XC_MLoad_GetStatus_U2(void* pInstance);
MS_BOOL MApi_XC_MLoad_WriteCmd_And_Fire_U2(void* pInstance, MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
MS_BOOL MApi_XC_MLoad_WriteCmds_And_Fire_U2(void* pInstance, MS_U32 *pu32Addr, MS_U16 *pu16Data, MS_U16 *pu16Mask, MS_U16 u16CmdCnt);
void    MApi_XC_MLG_Init_U2(void* pInstance, MS_PHY PhyAddr, MS_U32 u32BufByteLen);
void    MApi_XC_MLG_Enable_U2(void* pInstance, MS_BOOL bEnable);
MS_BOOL MApi_XC_MLG_GetCaps_U2(void* pInstance);
MLG_TYPE MApi_XC_MLG_GetStatus_U2(void* pInstance);
void    MApi_XC_SetOSD2VEMode_U2(void* pInstance, EN_VOP_SEL_OSD_XC2VE_MUX  eVOPSelOSD_MUX);
void    MApi_XC_IP2_PreFilter_Enable_U2(void* pInstance, MS_BOOL bEnable);
E_APIXC_GET_PixelRGB_ReturnValue MApi_XC_Get_Pixel_RGB_U2(void* pInstance, XC_Get_Pixel_RGB *pData, SCALER_WIN eWindow);
void    MApi_XC_KeepPixelPointerAppear_U2(void* pInstance, MS_BOOL bEnable);
MS_BOOL MApi_XC_Set_MemFmtEx_U2(void* pInstance, MS_XC_MEM_FMT eMemFmt, SCALER_WIN eWindow);
MS_BOOL MApi_XC_IsRequestFrameBufferLessMode_U2(void* pInstance);
E_APIXC_ReturnValue MApi_XC_SkipSWReset_U2(void* pInstance, MS_BOOL bFlag);
void    MApi_XC_EnableRepWindowForFrameColor_U2(void* pInstance, MS_BOOL bEnable);
E_APIXC_ReturnValue MApi_XC_SetOSDLayer_U2(void* pInstance, E_VOP_OSD_LAYER_SEL  eVOPOSDLayer, SCALER_WIN eWindow);
E_VOP_OSD_LAYER_SEL MApi_XC_GetOSDLayer_U2(void* pInstance, SCALER_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_SetVideoAlpha_U2(void* pInstance, MS_U8 u8Val, SCALER_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_GetVideoAlpha_U2(void* pInstance, MS_U8 *pu8Val, SCALER_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_SkipWaitVsync_U2(void* pInstance,  MS_BOOL eWindow,MS_BOOL bIsSkipWaitVsyn);
E_APIXC_ReturnValue MApi_XC_SetCMAHeapID_U2( void* pInstance, MS_U8 u8CMAHeapID, SCALER_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_OP2VOPDESel_U2(void* pInstance, E_OP2VOP_DE_SEL eVopDESel);
void    MApi_XC_FRC_SetWindow_U2(void* pInstance, E_XC_3D_INPUT_MODE e3dInputMode, E_XC_3D_OUTPUT_MODE e3dOutputMode, E_XC_3D_PANEL_TYPE e3dPanelType);
MS_BOOL MApi_XC_Enable_TwoInitFactor_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
MS_BOOL MApi_XC_IsFieldPackingModeSupported_U2(void* pInstance);
E_APIXC_ReturnValue MApi_XC_PreInit_U2(void* pInstance,  E_XC_PREINIT_t eType ,void* para, MS_U32 u32Length);
void    MApi_XC_Get_BufferData_U2(void* pInstance, E_XC_OUTPUTDATA_TYPE eBufType, MS_WINDOW_TYPE *pRect, void *pRectBuf, SCALER_WIN eWindow);
void    MApi_XC_Set_BufferData_U2(void* pInstance, E_XC_INPUTDATA_TYPE eBufType, MS_WINDOW_TYPE *pDstRect, void *pSrcBuf, MS_WINDOW_TYPE *pSrcRect, SCALER_WIN eWindow);
void    MApi_XC_EnableMainWindow_U2(void* pInstance, MS_BOOL bEnable);
void    MApi_XC_EnableSubWindow_U2(void* pInstance, MS_WINDOW_TYPE *pstDispWin);
void    MApi_XC_DisableSubWindow_U2(void* pInstance);
E_APIXC_ReturnValue MApi_XC_SetPixelShift_U2(void* pInstance, MS_S8 s8H, MS_S8 s8V);
E_APIXC_ReturnValue MApi_XC_SetPixelShiftFeatures_U2(void* pInstance,
        MS_U16 u16HPixelShiftRange,
        MS_U16 u16VPixelShiftRange,
        PIXEL_SHIFT_FEATURE ePixelShiftFeature);
E_APIXC_ReturnValue MApi_XC_Combine_MLoadEn_U2(void* pInstance, MS_BOOL bEn);
E_APIXC_ReturnValue MApi_XC_SetVideoOnOSD_U2(void* pInstance, E_VIDEO_ON_OSD_LAYER enlayer, SCALER_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_SetOSDLayerBlending_U2(void* pInstance, MS_U8 u8Layer, MS_BOOL bEnable, SCALER_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_SetOSDLayerAlpha_U2(void* pInstance, MS_U8 u8Layer, MS_U8 u8Alpha);
E_APIXC_ReturnValue MApi_XC_SetOSDBlendingFormula_U2(void* pInstance, E_XC_OSD_INDEX enOsdIndex, E_XC_OSD_BlENDING_TYPE enType, SCALER_WIN eWindow);
MS_BOOL MApi_XC_ReportPixelInfo_U2(void* pInstance, MS_XC_REPORT_PIXELINFO *pstRepPixInfo);
void    MApi_XC_SetScaling_U2(void* pInstance,  MS_BOOL bEnable, E_XC_SCALING_TYPE eScaling_type, E_XC_VECTOR_TYPE eVector_type, SCALER_WIN eWindow );
void    MApi_XC_SetMCDIBufferAddress_U2(void* pInstance, MS_PHY u32FBAddress, MS_PHY u32FBSize, E_XC_MCDI_TYPE eType);
void    MApi_XC_EnableMCDI_U2(void* pInstance, MS_BOOL bEnable, E_XC_MCDI_TYPE eType);
MS_BOOL MApi_XC_SendCmdToFRC_U2(void* pInstance,  MS_U8 u8Cmd, MS_U8 count, FRC_R2_CMD_PARAMETER_t pFRC_R2_Para );
MS_BOOL MApi_XC_GetMsgFromFRC_U2(void* pInstance, MS_U8* pu8Cmd, MS_U8* pu8ParaCount, MS_U8* pu8Para);
void    MApi_XC_EnableRWBankAuto_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
void    MApi_XC_Dump_SHM_U2(void *pInstance);
void MApi_XC_SetWRBankMappingNum_U2(void* pInstance, MS_U8 u8Val, SCALER_WIN eWindow);
MS_U8 MApi_XC_GetWRBankMappingNum_U2(void* pInstance, SCALER_WIN eWindow);
MS_U8 MApi_XC_GetWRBankMappingNumForZap_U2(void* pInstance, SCALER_WIN eWindow);
MS_BOOL MApi_XC_SetBOBMode_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
void MApi_XC_SetForceReadBank_U2(void* pInstance, MS_BOOL bEnable, MS_U8 u8Bank, SCALER_WIN eWindow);
void MApi_XC_SetLockPoint_U2(void* pInstance, MS_U16 u16LockPoint, SCALER_WIN eWindow);
MS_BOOL MApi_XC_LD_Init_U2(void* pInstance, EN_LD_PANEL_TYPE eLDPanelType );
MS_BOOL MApi_XC_LD_SetMemoryAddress_U2(void* pInstance, MS_U8 u8MIU ,MS_PHY u32LDFBase0,MS_PHY u32LDFBase1,MS_PHY u32LDBBase0,MS_PHY u32LDBBase1,MS_PHY u32EDGE2DBase,MS_PHY u32LEDoffset);
MS_BOOL MApi_XC_LD_Get_Value_U2(void* pInstance, MS_U8 *pu8LDValue, MS_U8 u8WSize, MS_U8 u8HSize);
MS_BOOL MApi_XC_LD_SetLevel_U2(void* pInstance, EN_LD_MODE eMode);
MS_BOOL MApi_Set_TurnoffLDBL_U2(void* pInstance, MS_BOOL bturnoffbl);
MS_BOOL MApi_Set_notUpdateSPIDataFlags_U2(void* pInstance, MS_BOOL brefreshSpidata);
MS_BOOL MApi_Set_UsermodeLDFlags_U2(void* pInstance, MS_BOOL buserLDFlags);
MS_BOOL MApi_Set_BLLevel_U2(void* pInstance, MS_U8 u8BLLeve);
void MApi_XC_Set_BWS_Mode_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
void MApi_XC_FRC_ColorPathCtrl_U2(void* pInstance, MS_U16 u16Path_sel, MS_BOOL bEnable);
void MApi_XC_FRC_OP2_SetRGBGain_U2(void* pInstance, MS_U16 u16RedGain, MS_U16 u16GreenGain, MS_U16 u16BlueGain);
void MApi_XC_FRC_OP2_SetRGBOffset_U2(void* pInstance, MS_U16 u16RedOffset, MS_U16 u16GreenOffset, MS_U16 u16BlueOffset);
void MApi_XC_FRC_OP2_SetDither_U2(void* pInstance, MS_U16 u16dither);
MS_BOOL MApi_XC_FRC_BypassMFC_U2(void* pInstance, MS_BOOL bEnable);
void MApi_XC_FRC_Mute_U2(void* pInstance, MS_BOOL bEnable);
E_APIXC_ReturnValue MApi_XC_ForceReadFrame_U2(void* pInstance, MS_BOOL bEnable, MS_U16 u16FrameIndex);
void MApi_XC_SetCsc_U2(void* pInstance,  MS_BOOL bEnable, SCALER_WIN eWindow );
void MApi_XC_RegisterPQSetFPLLThreshMode_U2(void* pInstance, void (*fpPQCB)(MS_U8 u8PQWin));
MS_BOOL MApi_XC_GetFreeRunStatus_U2(void* pInstance);
void MApi_XC_BYPASS_SetCSC_U2(void* pInstance, MS_BOOL bEnable);
E_APIXC_ReturnValue MApi_XC_Get_DSForceIndexSupported_U2(void* pInstance, SCALER_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_Set_DSForceIndex_U2(void* pInstance, MS_BOOL bEnable, MS_U8 u8Index, SCALER_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_Set_DSIndexSourceSelect_U2(void* pInstance, E_XC_DS_INDEX_SOURCE eDSIdxSrc, SCALER_WIN eWindow);
MS_BOOL MApi_XC_GetDynamicScalingStatus_U2(void* pInstance);
E_APIXC_ReturnValue MApi_XC_OSDC_InitSetting_U2(void* pInstance, E_XC_OSDC_TGEN_Type E_XC_OSDC_TGEN_Type, MS_XC_OSDC_TGEN_INFO *pstOC_Tgen_Cus, MS_XC_OSDC_CTRL_INFO *pstOC_Ctrl);
void MApi_XC_OSDC_SetOutVfreqx10_U2(void* pInstance, MS_U16 u16Vfreq);
E_APIXC_ReturnValue MApi_XC_OSDC_Control_U2(void* pInstance, MS_U32 eCtrl_type);
E_APIXC_ReturnValue MApi_XC_OSDC_GetDstInfo_U2(void* pInstance, MS_OSDC_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo);
MS_U32 MApi_XC_SetPowerState_U2(void* pInstance, EN_POWER_MODE enPowerState);
void MApi_XC_BYPASS_SetOSDVsyncPos_U2(void* pInstance, MS_U16 u16VsyncStart, MS_U16 u16VsyncEnd);
void MApi_XC_BYPASS_SetInputSrc_U2(void* pInstance, MS_BOOL bEnable, E_XC_BYPASS_InputSource input);
E_APIXC_ReturnValue MApi_XC_SetSeamlessZapping_U2(void* pInstance, SCALER_WIN eWindow, MS_BOOL bEnable);
E_APIXC_ReturnValue MApi_XC_GetSeamlessZappingStatus_U2(void* pInstance, SCALER_WIN eWindow, MS_BOOL* pbEnable);
E_APIXC_ReturnValue MApi_XC_Vtrack_SetPayloadData_U2(void* pInstance, MS_U16 u16Timecode, MS_U8 u8OperatorID);
E_APIXC_ReturnValue MApi_XC_Vtrack_SetUserDefindedSetting_U2(void* pInstance, MS_BOOL bUserDefinded, MS_U8 *pu8Setting);
E_APIXC_ReturnValue MApi_XC_Vtrack_Enable_U2(void* pInstance, MS_U8 u8FrameRate, MS_BOOL bEnable);
void MApi_XC_PreSetPQInfo_U2(void* pInstance, XC_SETWIN_INFO *pstXC_SetWin_Info, SCALER_WIN eWindow);
MS_BOOL MApi_XC_Is_OP1_TestPattern_Enabled_U2(void* pInstance);
E_APIXC_ReturnValue MApi_XC_Set_OP1_TestPattern_U2(void* pInstance, EN_XC_OP1_PATTERN ePattern, EN_XC_OP1_PATGEN_MODE eMode);
E_APIXC_ReturnValue MApi_XC_Set_OP2_Pattern_U2(void* pInstance, MS_BOOL bEnable, MS_U16 u16R_Data, MS_U16 u16G_Data ,MS_U16 u16B_Data);
MS_BOOL MApi_XC_CheckWhiteBalancePatternModeSupport_U2(void* pInstance, EN_XC_WB_PATTERN_MODE enWBPatternMode);
E_APIXC_ReturnValue MApi_XC_SetHLinearScaling_U2(void* pInstance, MS_BOOL bEnable, MS_BOOL bSign, MS_U16 u16Delta, SCALER_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_SetForceWrite_U2(void* pInstance,  EN_MUTE_TYPE enMuteType, MS_BOOL bIsForceWrite);
MS_BOOL MApi_XC_GetForceWrite_U2(void* pInstance,  EN_MUTE_TYPE enMuteType);
// OBSOLETE FUNCTION POINTER START
MS_U16  mvideo_sc_get_output_vfreq_U2(void* pInstance);
MS_U32  MApi_XC_Get_Current_OutputVFreqX100_U2(void* pInstance);
E_APIXC_ReturnValue MApi_XC_SetMemoryWriteRequest_U2(void* pInstance, MS_BOOL bEnable);
MS_BOOL MApi_XC_Set_MemFmt_U2(void* pInstance, MS_XC_MEM_FMT eMemFmt);
E_APIXC_ReturnValue MApi_XC_SetOutputAdjustSetting_U2(void* pInstance, XC_OUTPUT_TIMING_ADJUST_SETTING *stAdjSetting);
void    msAPI_Scaler_FPLL_FSM_U2(void* pInstance, SCALER_WIN eWindow);
void    MApi_XC_EnableMirrorMode_U2(void* pInstance,  MS_BOOL bEnable );
void    MApi_XC_EnableMirrorMode2_U2(void* pInstance,  MirrorMode_t eMirrorMode );
MirrorMode_t MApi_XC_GetMirrorModeType_U2(void* pInstance);
// OBSOLETE FUNCTION POINTER END

E_APIXC_ReturnValue MApi_XC_EnableT3D_U2(void* pInstance, MS_BOOL bEnable);
E_APIXC_ReturnValue MApi_XC_Set_FRC_InputTiming_U2(void* pInstance, E_XC_FRC_InputTiming enFRC_InputTiming);
E_APIXC_ReturnValue MApi_XC_Get_FRC_InputTiming_U2(void* pInstance, E_XC_FRC_InputTiming *penFRC_InputTiming);
E_APIXC_ReturnValue MApi_XC_Get_VirtualBox_Info_U2(void * pInstance,XC_VBOX_INFO * pstXC_VboxInfo);
E_APIXC_ReturnValue MApi_XC_Set_OSD_Detect_U2(void* pInstance,MS_BOOL bEnable, MS_U32 Threhold);
E_APIXC_ReturnValue MApi_XC_Get_OSD_Detect_U2(void* pInstance,MS_BOOL *pbOSD);
E_APIXC_ReturnValue MApi_XC_Enable_LockFreqOnly_U2(void* pInstance, MS_BOOL bEnable);
MS_BOOL             MApi_XC_Is2K2KToFrcMode_U2(void* pInstance);
//MS_BOOL             MApi_XC_Set_2P_Mode_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
MS_BOOL MApi_XC_Set_XC_VOP_U2(void * pInstance, MS_XC_VOP_Data stXCVOPData);
MS_U8 MApi_XC_GetSWDSIndex_U2(void* pInstance);
MS_BOOL MApi_XC_Is_SupportSWDS_U2(void *pInstance);
MS_U8 MApi_XC_Get_FRCM_FrameNum_U2(void* pInstance, SCALER_WIN eWindow);

#ifdef UFO_XC_FB_LEVEL
E_APIXC_ReturnValue MApi_XC_Set_FB_Level_U2(void* pInstance, E_XC_FB_LEVEL eFBLevel, SCALER_WIN eWindow);
E_XC_FB_LEVEL MApi_XC_Get_FB_Level_U2(void* pInstance, SCALER_WIN eWindow);
#endif

#ifdef UFO_XC_SET_DSINFO_V0
void MApi_XC_SetDSInfo_U2(void* pInstance,XC_DS_INFO *pstSetDSInfo, MS_U32 u32DSInfoLen, SCALER_WIN eWindow);
#endif
E_APIXC_ReturnValue MApi_XC_VideoPlaybackCtrl_U2(void* pInstance,MS_U32 u32Cmd,void *pCmdArgs,MS_U32 u32CmdArgsSize);

#ifdef UFO_XC_AUTO_DOWNLOAD
E_APIXC_ReturnValue MApi_XC_AutoDownload_Config_U2(void* pInstance, XC_AUTODOWNLOAD_CONFIG_INFO *pstConfigInfo);
E_APIXC_ReturnValue MApi_XC_AutoDownload_Write_U2(void* pInstance, XC_AUTODOWNLOAD_DATA_INFO *pstDataInfo);
E_APIXC_ReturnValue MApi_XC_AutoDownload_Fire_U2(void* pInstance, EN_XC_AUTODOWNLOAD_CLIENT enClient);
#endif
#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
E_APIXC_ReturnValue MApi_XC_HDR_Control_U2(void* pInstance, EN_XC_HDR_CTRL_TYPE enCtrlType, void *pParam);
#endif
#endif
#ifdef UFO_XC_SETBLACKVIDEOBYMODE
E_APIXC_ReturnValue MApi_XC_GenerateBlackVideoByMode_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow, EN_XC_BLACK_VIDEO_MODE eMode);
#endif
MS_BOOL MApi_XC_OPTEE_Control_U2(void *pInstance,EN_XC_OPTEE_ACTION action,XC_OPTEE_HANDLER* xc_handler);
MS_BOOL  MApi_XC_OPTEE_CheckRBase_U2(void *pInstance,SCALER_WIN eWindow);
MS_BOOL MApi_XC_OPTEE_Mux_U2(void *pInstance,EN_XC_OPTEE_ACTION action,XC_OPTEE_MUX_DATA xc_mux_data);
MS_U32 MApi_XC_GetResourceByPipeID_U2(void* pInstance,MS_U32 u32PipeID, MS_U32*  U32RetResNum, RESOURCE* pResource);
MS_U32 MApi_XC_ConfigPipe_U2(void* pInstance,MS_U32 u32PipeID, MS_U32 u32SecureDMA, MS_U32 u32OperationMode);
MS_U32 MApi_XC_CheckPipe_U2(void* pInstance,MS_U32 u32PipeID, MS_U32 u32SecureDMA, MS_U32 u32OperationMode);

// APIXC_ADC_H
void    MApi_XC_ADC_SetCVBSOut_U2(void* pInstance, MS_BOOL bEnable, E_DEST_TYPE enOutputType , INPUT_SOURCE_TYPE_t enInputPortType, MS_BOOL isDigitalSource);
MS_BOOL MApi_XC_ADC_IsCVBSOutEnabled_U2(void* pInstance, E_DEST_TYPE enOutputType);
void    MApi_XC_ADC_SetPcClock_U2(void* pInstance, MS_U16 u16Clock);
void    MApi_XC_ADC_SetPhase_U2(void* pInstance, MS_U8 u8Value);
void    MApi_XC_ADC_SetPhaseEx_U2(void* pInstance, MS_U16 u16Value);
MS_U16  MApi_XC_ADC_GetPhaseRange_U2(void* pInstance);
MS_U8   MApi_XC_ADC_GetPhase_U2(void* pInstance);
MS_U16  MApi_XC_ADC_GetPhaseEx_U2(void* pInstance);
MS_BOOL MApi_XC_ADC_IsScartRGB_U2(void* pInstance);
MS_U16  MApi_XC_ADC_GetPcClock_U2(void* pInstance);
void    MApi_XC_ADC_GetSoGLevelRange_U2(void* pInstance, MS_U32 *u32min, MS_U32 *u32max, MS_U32 *u32Recommend_value);
void    MApi_XC_ADC_SetSoGLevel_U2(void* pInstance, MS_U32 u32Value);
void    MApi_XC_ADC_PowerOff_U2(void* pInstance);
void    MApi_XC_ADC_GetDefaultGainOffset_U2(void* pInstance, INPUT_SOURCE_TYPE_t eSource , APIXC_AdcGainOffsetSetting* pstADCSetting);
MS_U16  MApi_XC_ADC_GetMaximalOffsetValue_U2(void* pInstance);
MS_U16  MApi_XC_ADC_GetMaximalGainValue_U2(void* pInstance);
MS_U16  MApi_XC_ADC_GetCenterGain_U2(void* pInstance);
MS_U16  MApi_XC_ADC_GetCenterOffset_U2(void* pInstance);
void    MApi_XC_ADC_SetGain_U2(void* pInstance, MS_U8 u8Color, MS_U16 u16Value);
void    MApi_XC_ADC_SetOffset_U2(void* pInstance, MS_U8 u8Color, MS_U16 u16Value);
void    MApi_XC_ADC_AdjustGainOffset_U2(void* pInstance, APIXC_AdcGainOffsetSetting* pstADCSetting);
void    MApi_XC_ADC_Source_Calibrate_U2(void* pInstance, INPUT_SOURCE_TYPE_t enInputSourceType);
void    MApi_XC_ADC_SetSoGCal_U2(void* pInstance);
void    MApi_XC_ADC_SetRGB_PIPE_Delay_U2(void* pInstance, MS_U8 u8Value);
void    MApi_XC_ADC_ScartRGB_SOG_ClampDelay_U2(void* pInstance, MS_U16 u16Clpdly, MS_U16 u16Caldur);
void    MApi_XC_ADC_Set_YPbPrLooseLPF_U2(void* pInstance, MS_BOOL benable);
void    MApi_XC_ADC_Set_SOGBW_U2(void* pInstance, MS_U16 u16value);
void    MApi_XC_ADC_SetClampDuration_U2(void* pInstance, MS_U16 u16Value);
MS_BOOL MApi_XC_ADC_EnableHWCalibration_U2(void* pInstance, MS_BOOL bEnable);
MS_BOOL MApi_XC_ADC_SetIdacCurrentMode_U2(void* pInstance, ADC_DacCurrentMode_t eMode);
ADC_DacCurrentMode_t MApi_XC_ADC_GetIdacCurrentMode_U2(void* pInstance);


// APIXC_AUTO_H
MS_BOOL MApi_XC_Auto_Geometry_U2(void* pInstance, XC_Auto_TuneType enAutoTuneType, XC_Auto_Signal_Info *ActiveInfo, XC_Auto_Signal_Info *StandardInfo, SCALER_WIN eWindow);
MS_BOOL MApi_XC_Auto_Geometry_Ex_U2(void* pInstance, XC_Auto_TuneType enAutoTuneType, XC_Auto_Signal_Info_Ex *ActiveInfo, XC_Auto_Signal_Info_Ex *StandardInfo, SCALER_WIN eWindow);
MS_BOOL MApi_XC_Auto_StopAutoGeometry_U2(void* pInstance);
MS_BOOL MApi_XC_Auto_GainOffset_U2(void* pInstance, XC_Auto_CalibrationType type, XC_Auto_TuneType enAutoTuneType,  APIXC_AdcGainOffsetSetting *pstADCSetting, SCALER_WIN eWindow);
MS_BOOL MApi_XC_Auto_GetHWFixedGainOffset_U2(void* pInstance, XC_Auto_TuneType enAutoTuneType, APIXC_AdcGainOffsetSetting *pstADCSetting);
MS_BOOL MApi_XC_Auto_SetValidData_U2(void* pInstance, MS_U8 Value);
void    MApi_XC_Auto_AutoOffset_U2(void* pInstance, MS_BOOL bEnable, MS_BOOL bIsYPbPrFlag);
MS_BOOL MApi_XC_Auto_DetectWidth_U2(void* pInstance, XC_Auto_Signal_Info_Ex *pSignalInfo , SCALER_WIN eWindow);
MS_BOOL MApi_XC_Auto_SetCalibrationMode_U2(void* pInstance, XC_Auto_CalibrationMode eMode);
void    MApi_XC_Auto_GetCalibrationMode_U2(void* pInstance, XC_Auto_CalibrationMode *eMode);
void    MApi_XC_AUTO_GetSyncInfo_U2(void* pInstance,XC_AUTO_SYNC_INFO *pSyncInfo);

// APIXC_MODEPARSE_H
MS_BOOL MApi_XC_ModeParse_Init_U2(void* pInstance);
XC_MODEPARSE_RESULT MApi_XC_ModeParse_MatchMode_U2(void* pInstance, MS_PCADC_MODETABLE_TYPE* ModeDB, MS_U8 u8NumberOfItems , XC_MODEPARSE_INPUT_INFO *psInputInfo);
XC_MODEPARSE_RESULT MApi_XC_ModeParse_MatchModeEx_U2(void* pInstance, MS_PCADC_MODETABLE_TYPE_EX* ModeDB, MS_U8 u8NumberOfItems , XC_MODEPARSE_INPUT_INFO *psInputInfo);


// APIXC_PCMONITOR_H
MS_BOOL MApi_XC_PCMonitor_Init_U2(void* pInstance, MS_U8 u8MaxWindowNum);
void MApi_XC_PCMonitor_Restart_U2(void* pInstance, SCALER_WIN eWindow);
void MApi_XC_PCMonitor_SetTimingCount_U2(void* pIstance, MS_U16 u16TimingStableCounter, MS_U16 u16TimingNosyncCounter);
XC_PCMONITOR_STATUS MApi_XC_PCMonitor_U2(void* pInstance, INPUT_SOURCE_TYPE_t src, SCALER_WIN eWindow);
XC_PCMONITOR_STATUS MApi_XC_PCMonitor_GetCurrentState_U2(void* pInstance, SCALER_WIN eWindow);
MS_U8 MApi_XC_PCMonitor_GetSyncStatus_U2(void* pInstance, SCALER_WIN eWindow);
MS_U16 MApi_XC_PCMonitor_Get_HFreqx10_U2(void* pInstance, SCALER_WIN eWindow);
MS_U32 MApi_XC_PCMonitor_Get_HFreqx1K_U2(void* pInstance, SCALER_WIN eWindow);
MS_U16 MApi_XC_PCMonitor_Get_VFreqx10_U2(void* pInstance, SCALER_WIN eWindow);
MS_U32 MApi_XC_PCMonitor_Get_VFreqx1K_U2(void* pInstance, SCALER_WIN eWindow);
MS_U16 MApi_XC_PCMonitor_Get_Vtotal_U2(void* pInstance, SCALER_WIN eWindow);
void MApi_XC_PCMonitor_Get_Dvi_Hdmi_De_Info_U2(void* pInstance, SCALER_WIN eWindow,MS_WINDOW_TYPE* msWin);
MS_BOOL MApi_XC_PCMonitor_SyncLoss_U2(void* pInstance, SCALER_WIN eWindow);
MS_BOOL MApi_XC_PCMonitor_InvalidTimingDetect_U2(void* pInstance, MS_BOOL bPollingOnly, SCALER_WIN eWindow);
E_APIXC_ReturnValue MApi_XC_PCMonitor_SetTimingCountEx_U2(void* pInstance,  INPUT_SOURCE_TYPE_t eCurrentSrc, MS_U16 u16TimingStableCounter, MS_U16 u16TimingNosyncCounter);



// DRVXC_HDMI_IF_H
MS_U16 MDrv_HDMI_Func_Caps_U2(void* pInstance);
stHDMI_INITIAL_TABLE MDrv_HDMI_init_U2(void* pInstance);
void MDrv_HDMI_Exit_U2(void* pInstance);
void MDrv_HDMI_pkt_reset_U2(void* pInstance,  HDMI_REST_t breset );
void MDrv_HDMI_pullhpd_U2(void* pInstance,  MS_BOOL bHighLow, E_MUX_INPUTPORT enInputPortType, MS_BOOL bInverse);
MS_U16 MDrv_HDMI_GC_Info_U2(void* pInstance, MS_U8 ucHDMIInfoSource, HDMI_GControl_INFO_t gcontrol);
void MDrv_HDMI_Packet_Received_U2(void* pInstance, MS_U8 ucHDMIInfoSource, MS_BOOL bEnable, MS_U32 ulPacketStatus);
MS_HDMI_COLOR_FORMAT MDrv_HDMI_Get_ColorFormat_U2(void* pInstance, MS_U8 ucHDMIInfoSource);
EN_HDMI_COLOR_RANGE MDrv_HDMI_Get_ColorRange_U2(void* pInstance, MS_U8 ucHDMIInfoSource);
void MDrv_HDMI_Set_EQ_U2(void* pInstance, MS_HDMI_EQ enEq, MS_U8 u8EQValue);
MS_BOOL MDrv_HDMI_Set_EQ_To_Port_U2(void* pInstance, MS_HDMI_EQ enEq, MS_U8 u8EQValue, E_MUX_INPUTPORT enInputPortType);
void MDrv_HDMI_Audio_MUTE_Enable_U2(void* pInstance, MS_U16 u16MuteEvent, MS_U16 u16MuteMask);
void MDrv_HDMI_Audio_Status_Clear_U2(void* pInstance);
MS_HDMI_AR_TYPE MDrv_HDMI_Get_AspectRatio_U2(void* pInstance, MS_U8 ucHDMIInfoSource, MS_BOOL bEn_PAR, MS_BOOL bEn_AFAR);
MS_HDMI_AR_TYPE MDrv_HDMI_Get_ActiveFormat_AspectRatio_U2(void* pInstance, MS_U8 ucHDMIInfoSource, MS_BOOL bEn_AFAR);
EN_AVI_INFOFRAME_VERSION MDrv_HDMI_Get_AVIInfoFrameVer_U2(void* pInstance, MS_U8 ucHDMIInfoSource);
MS_U8 MDrv_HDMI_err_status_update_U2(void* pInstance, MS_U8 ucHDMIInfoSource, MS_U8 u8value, MS_BOOL bread);
void MDrv_HDMI_Get_PollingStatus_U2(void* pInstance, HDMI_POLLING_STATUS_t **pStatus);
MS_BOOL MDrv_HDMI_IsHDMI_Mode_U2(void* pInstance);
void MDrv_HDMI_Get_MID_info_U2(void* pInstance, MS_U8 ucHDMIInfoSource, MS_U8 * pu8ManufactorData);
MS_BOOL MDrv_HDMI_get_packet_value_U2(void* pInstance, MS_U8 ucHDMIInfoSource, MS_HDMI_PACKET_STATE_t u8state, MS_U8 u8byte_idx, MS_U8 *pValue);
MS_U16  MDrv_DVI_ChannelPhaseStatus_U2(void* pInstance,  MS_DVI_CHANNEL_TYPE u8Channel);
void MDrv_DVI_SoftwareReset_U2(void* pInstance,  MS_U16 u16Reset );
void mdrv_dvi_reset_U2(void* pInstance);
MS_BOOL MDrv_HDMI_Get_AVIInfoActiveInfoPresent_U2(void* pInstance, MS_U8 ucHDMIInfoSource);
void MDrv_DVI_ClkPullLow_U2(void* pInstance, MS_BOOL bPullLow, E_MUX_INPUTPORT enInputPortType);
void MDrv_DVI_SwitchSrc_U2(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8 ucHDMIInfoSource);
MS_BOOL MDrv_HDMI_SetForClockLessThan70Mhz_U2(void* pInstance,  E_MUX_INPUTPORT InputSourceType  );
void MDrv_HDMI_dvi_adjust_U2(void* pInstance,  MS_BOOL bClockLessThan70MHz);
void MDrv_HDMI_SetUpdatePhaseLineCount_U2(void* pInstance, INPUT_SOURCE_TYPE_t enInputSourceType);
void MDrv_HDCP_Enable_U2(void* pInstance, MS_BOOL bEnable);
MS_BOOL MDrv_HDMI_SetHdcpEnable_U2(void* pInstance, E_MUX_INPUTPORT enInputPortType,MS_BOOL bEnable);
void MDrv_HDCP_ClearStatus_U2(void* pInstance);
void MDrv_HDCP_initproductionkey_U2(void* pInstance,  MS_U8 * pu8HdcpKeyData );
MS_U16 MDrv_HDCP_GetStatus_U2(void* pInstance);
void MDrv_HDCP_Vsync_end_en_U2(void* pInstance, MS_BOOL bStable);
void MDrv_HDMI_audio_output_U2(void* pInstance,  MS_BOOL bEnable );
MS_U8 MDrv_HDMI_audio_cp_hdr_info_U2(void* pInstance, MS_U8 ucHDMIInfoSource);
MS_U8 MDrv_HDMI_audio_channel_status_U2(void* pInstance, MS_U8 ucHDMIInfoSource, MS_U8 u8byte);
HDMI_Ret_Value MDrv_HDMI_GetLibVer_U2(void* pInstance, const MSIF_Version **ppVersion);
HDMI_Ret_Value MDrv_HDMI_GetInfo_U2(void* pInstance, MS_HDMI_INFO *pInfo);
HDMI_Ret_Value MDrv_HDMI_GetStatus_U2(void* pInstance, MS_HDMI_Status *pStatus);
void MDrv_HDMI_READ_DDCRAM_U2(void* pInstance, XC_DDCRAM_PROG_INFO *pstDDCRam_Info, MS_U32 u32SizeOfInfo);
void MDrv_HDMI_PROG_DDCRAM_U2(void* pInstance, XC_DDCRAM_PROG_INFO *pstDDCRam_Info, MS_U32 u32SizeOfInfo);
MS_HDMI_CONTENT_TYPE MDrv_HDMI_Get_Content_Type_U2(void* pInstance, MS_U8 ucHDMIInfoSource);
MS_HDMI_EXT_COLORIMETRY_FORMAT MDrv_HDMI_Get_ExtColorimetry_U2(void* pInstance, MS_U8 ucHDMIInfoSource);
MS_U8 MDrv_HDMI_Get_Pixel_Repetition_U2(void* pInstance, MS_U8 ucHDMIInfoSource);
MS_BOOL MDrv_HDMI_Check4K2K_U2(void* pInstance, MS_U8 ucHDMIInfoSource);
void MDrv_HDMI_3D_4Kx2K_Process_U2(void* pInstance, MS_U8 ucHDMIInfoSource);
void MDrv_HDMI_AVG_ScaleringDown_U2(void* pInstance, MS_U8 ucHDMIInfoSource, MS_BOOL bTrue);
E_HDMI_ADDITIONAL_VIDEO_FORMAT MDrv_HDMI_Check_Additional_Format_U2(void* pInstance, MS_U8 ucHDMIInfoSource);
E_XC_3D_INPUT_MODE MDrv_HDMI_Get_3D_Structure_U2(void* pInstance, MS_U8 ucHDMIInfoSource);
E_HDMI_3D_EXT_DATA_T MDrv_HDMI_Get_3D_Ext_Data_U2(void* pInstance, MS_U8 ucHDMIInfoSource);
void MDrv_HDMI_Get_3D_Meta_Field_U2(void* pInstance, MS_U8 ucHDMIInfoSource, sHDMI_3D_META_FIELD *pdata);
MS_U8 MDrv_HDMI_Get_VIC_Code_U2(void* pInstance, MS_U8 ucHDMIInfoSource);
E_HDMI_VIC_4Kx2K_CODE MDrv_HDMI_Get_4Kx2K_VIC_Code_U2(void* pInstance, MS_U8 ucHDMIInfoSource);
void MDrv_HDMI_ARC_PINControl_U2(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_BOOL bEnable, MS_BOOL bDrivingHigh);
void MDrv_DVI_Software_Reset_U2(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U16 u16Reset);
MS_U32 MDrv_HDMI_SetPowerState_U2(void* pInstance, EN_POWER_MODE);
void MDrv_DVI_HF_adjust_U2(void* pInstance);
MS_U16 MDrv_HDMI_GetTMDSFreq_U2(void* pInstance);
MS_BOOL MDrv_HDMI_CheckHDMI20_Setting_U2(void* pInstance, E_MUX_INPUTPORT enInputPortType);
void MDrv_HDMI_Set5VDetectGPIOSelect_U2(void* pInstance, MS_U32 ul5VDetectGPIOIndex);
MS_BOOL MDrv_HDMI_GetDEStableStatus_U2(void* pInstance, E_MUX_INPUTPORT enInputPortType);
MS_BOOL MDrv_HDMI_Ctrl_U2(void* pInstance, MS_U8 ucHDMIInfoSource, MS_U32 u32Cmd, void *pBuf, MS_U32 u32BufSize);
MS_U16 MDrv_HDMI_GetDataInfo_U2(void* pInstance, E_HDMI_GET_DATA_INFO enInfo, MS_U8 ucHDMIInfoSource);
#ifdef UFO_XC_VMARK
// APIXC_VMARK_H
E_APIXC_ReturnValue MApi_XC_VMark_SetParameters_U2(void* pInstance, MS_U8 u8ServiceIndex, XC_VMARK_PARAMETER *pstXC_VMarkParameters);
#endif
void XCRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 XCOpenFull(void** ppInstance, const void* const pAttribute);
MS_U32 XCOpenMinimum(void** ppInstance, const void* const pAttribute);
MS_U32 XCOpenForTEE(void** ppInstance, const void* const pAttribute);
MS_U32 XCClose(void* pInstance);
MS_U32 XCIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
MS_U32 _XCIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
void XC_Initialize(XC_INSTANCE_PRIVATE* psXCInstPri, const void* const pAttribute);
MS_U32 _ADCIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
void ADC_Initialize(XC_INSTANCE_PRIVATE* psXCInstPri, const void* const pAttribute);
MS_U32 _AUTOIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
void AUTO_Initialize(XC_INSTANCE_PRIVATE* psXCInstPri, const void* const pAttribute);
MS_U32 _MODEPARSEIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
void MODEPARSE_Initialize(XC_INSTANCE_PRIVATE* psXCInstPri, const void* const pAttribute);
MS_U32 _PCMONITORIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
void PCMONITOR_Initialize(XC_INSTANCE_PRIVATE* psXCInstPri, const void* const pAttribute);
MS_U32 _HDMI_IF_Ioctl(void * pInstance, MS_U32 u32Cmd, void * pArgs);
void HDMI_IF_Initialize(XC_INSTANCE_PRIVATE* psXCInstPri, const void* const pAttribute);
MS_BOOL MApi_XC_IsSupport2StepScaling_U2(void* pInstance);
MS_BOOL MApi_XC_GetPQPathStatus_U2(void* pInstance, E_XC_PQ_Path_Type ePqPathType, MS_U16 u16Width, MS_U16 u16Height);
#ifdef UFO_XC_VMARK
void VMARK_Initialize(XC_INSTANCE_PRIVATE* psXCInstPri, const void* const pAttribute);
MS_U32 _VMARKIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
#endif
void MApi_SWDS_Fire_U2(void *pInstance, SCALER_WIN eWindow );
MS_BOOL MApi_XC_SetSWDRInfo_U2(void* pInstance, XC_SWDR_INFO *pSWDR_INFO);
MS_BOOL MApi_XC_GetSWDRInfo_U2(void* pInstance, XC_SWDR_INFO *pSWDR_INFO);
#ifdef UFO_XC_SUPPORT_HDMI_DOWNSCALE_OUTPUT_POLICY
MS_U32 MApi_XC_GetHDMIPolicy_U2(void* pInstance);
#endif

#ifdef __cplusplus
}
#endif

#undef _API_XC_PRIV_H_
#endif // _API_XC_PRIV_H_
