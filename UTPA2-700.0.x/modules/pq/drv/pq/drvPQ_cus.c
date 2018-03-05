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

#define  _MDRV_PQ_CUS_C_

#ifdef MSOS_TYPE_LINUX
#include <pthread.h>
#endif
#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include "string.h"
#else
#include <linux/string.h>
#endif

#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h> // O_RDWR
#endif

#include "MsCommon.h"
//#include "MsVersion.h"
#include "MsOS.h"
#include "MsTypes.h"
//#include "hwreg_utility2.h"
#include "color_reg.h"
#include "utopia.h"
#include "utopia_dapi.h"

#include "drvXC_IOPort.h"
#include "apiXC.h"
//#include "apiXC_Ace.h"
//#include "drvMVOP.h"

#include "drvPQ_Define.h"
#include "drvPQ_Define_cus.h"

#include "drvPQ_Declare.h"
#include "drvPQ.h"
#include "drvPQ_cus.h"
#include "drvPQ_Datatypes.h"
#include "mhal_pq.h"
#include "mhal_pq_cus.h"
#include "mhal_pq_adaptive.h"
#include "drvPQ_Bin.h"
#include "apiPNL.h"

#include "../../modules/xc/drv/xc/mdrv_xc_io.h"
#include "../../modules/xc/drv/xc/mdrv_xc_st.h"

#include "QualityMode.h"
#include "apiPQ_v2.h"
#include "PQ_private.h"
#if PQ_ENABLE_DEBUG
extern MS_U16 _u16PQDbgSwitch;
#endif

#ifndef UNUSED //to avoid compile warnings...
#define UNUSED(var) (void)((var) = (var))
#endif

#ifndef PQ_GRULE_SLZ_ENABLE
#define PQ_GRULE_SLZ_ENABLE 0
#endif

#ifndef PQ_QM_HDMI_4K2K
#define PQ_QM_HDMI_4K2K 0
#endif

#ifndef PQ_QM_DTV_4K2K
#define PQ_QM_DTV_4K2K 0
#endif

#ifndef PQ_OSD_BW_VER
#define PQ_OSD_BW_VER 1
#endif

#ifndef ENABLE_PQ_UFSC_DLC
#define ENABLE_PQ_UFSC_DLC   0
#endif

#ifndef ENABLE_PQ_UFSC_BYPASS
#define ENABLE_PQ_UFSC_BYPASS 0
#endif

/********************************************************************************
// PQ_IOCTL_SET_UC_FEATURE is flag for set UCNR  During SetWin(if enable this switch please sync CL641359)
// and please check  PQ excel skipRule(UC_CTL/UCDi/MADi_EODiW/MADi_SSTMADi_Motion need skip)item to prevent to set UCNR twice
**********************************************************************************/
#ifndef PQ_IOCTL_SET_UC_FEATURE
#define PQ_IOCTL_SET_UC_FEATURE   0
#endif

#ifndef PQ_VIP_OFF_MINUS16
#define PQ_VIP_OFF_MINUS16 0
#endif

#define PQGRULE_DBG(x) do { if( _u16PQDbgSwitch & PQ_DBG_GRULE) x; } while(0);
#define PQMODE_DBG(x)   //x
#define PQ_IS_4K2K_INPUT(HSize,Vsize)  (((HSize) > 3800) && ((Vsize) >= 1900))
#define GateFor30P 450
#define PQ_IS_4K_INPUT(HSize,Vsize)  (((HSize) > 3000) && ((Vsize) >= 1050)) //Load 4k2k PQ mode if input is 4k2k or 4k1k.
#define PQ_IS_DTV_4K_INPUT(HSize,Vsize)  (((HSize) > 2096) && ((Vsize) >= 1050)) // SC line buffer size is 2096, Load 4k2k PQ mode if input H > 2096

#define CFD_ANALOG_VERSION 0
#define CFD_LINEAR_RGB_VERSION 0
#define CFD_FIRE_VERSION 0

//#define TRACE_DBG
extern MS_U16 _u16PQSrcType[PQ_MAX_WINDOW];
extern MS_U16 _u16PQDbgSwitch;

extern MS_BOOL gbPQBinEnable;
extern MS_PQBin_Header_Info stPQBinHeaderInfo[MAX_PQ_BIN_NUM];
extern MS_PQTextBin_Header_Info stPQTextBinHeaderInfo[MAX_PQ_TEXT_BIN_NUM];
extern MS_BOOL gbEnablePQTextBin;
extern MS_PQ_Vd_Info _stVD_Info[PQ_MAX_WINDOW];
extern MS_PQ_MuliMedia_Info _stMultiMedia_Info[PQ_MAX_WINDOW];
extern MS_PQ_Mode_Info  _stMode_Info[PQ_MAX_WINDOW];
extern MS_U8 _u8ModeIndex[PQ_MAX_WINDOW];
extern MS_PQ_Hdmi_Info _stHDMI_Info[PQ_MAX_WINDOW];
extern MS_PQ_Dtv_Info _stDTV_Info[PQ_MAX_WINDOW];
extern MS_BOOL _bOSD_On;
extern MS_BOOL _gbMemfmt422[PQ_MAX_WINDOW];
extern MS_BOOL _gbMemfmt422[PQ_MAX_WINDOW];
extern MS_BOOL _bDS_En;
extern MS_U16 _u16RW_Method;
extern MS_BOOL _bRWNumIsReset[PQ_MAX_WINDOW];
extern MS_BOOL bSetFrameCount;
extern MS_U16 _gu16SetMemfmt_Madi[PQ_MAX_WINDOW];
extern MS_U16 _gu16SetMemfmt_Madi_Motion[PQ_MAX_WINDOW];

MS_BOOL _gIsH264 = 0;
MS_BOOL _gIsMVC4kx1k = 0;
MS_BOOL _gbIsG3D = 0; //priority for G3D
MS_BOOL _gIsMMplayer=0;
MS_BOOL _gIsRmvb[PQ_MAX_WINDOW] = {0,0};
MS_BOOL _gIsNetworkMM = 0;
MS_BOOL _gbSubWin_En = FALSE; //indicate subwindow enable or not, for bw setting
MS_BOOL _gIsDualDecode = 0;
MS_BOOL _gIs1KTraveling = 0;
MS_BOOL _gIsThroughMode = FALSE;
static MS_U16 gu16DisplayWidth = 0;

#if (PQ_ENABLE_PIP)
static PQ_DISPLAY_TYPE genDisplayType = PQ_DISPLAY_ONE;
#endif

#if PQ_GRULE_DDR_SELECT_ENABLE
extern BW_INFO_t bw_info;
#endif
#if PQ_DLC_CTL_FOR_MVC4KX1K
extern MS_BOOL _gIsMVC4kx1k;
#endif
#if (PQ_QM_BYPASS_COLOR)
extern MS_BOOL _bBypassModeOn[PQ_MAX_WINDOW];
#endif

#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
extern MS_BOOL _bCustomerMainPQEn;
#endif
#if ENABLE_PQ_CUSTOMER_BIN_SUB_GRULE
extern MS_BOOL _bCustomerSubPQEn;
#endif

extern MS_BOOL _enPQDisplayType;

#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
#ifdef MSOS_TYPE_LINUX
static MS_S32   _s32FdScaler = -1;
#endif
#endif
#endif

#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_MSTAR_XC_HDR_SUPPORT))
extern MS_BOOL MDrv_XC_CFDControl(ST_KDRV_XC_CFD_CONTROL_INFO *pstKdrvCFDCtrlInfo);
#endif
MS_BOOL MDrv_PQ_SendCfd(EN_PQ_CFD_CTRL_TYPE enCtrlType, void *pstCfdInfo, MS_U32 u32Size)
{
#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)

    ST_PQ_CFD_CONTROL_INFO stCfdCtrlInfo;
    memset(&stCfdCtrlInfo, 0, sizeof(ST_PQ_CFD_CONTROL_INFO));
    stCfdCtrlInfo.enCtrlType = enCtrlType;
    stCfdCtrlInfo.pParam = pstCfdInfo;
    stCfdCtrlInfo.u32ParamLen = u32Size;
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)

    if(0 > _s32FdScaler)
    {
        _s32FdScaler = open("/dev/scaler", O_RDWR);

        if(0 > _s32FdScaler)
        {
            return FALSE;
        }
    }

    if (ioctl(_s32FdScaler, MDRV_XC_IOC_CFDCONCTRL, &stCfdCtrlInfo))
    {
        return FALSE;
    }

    return TRUE;
#elif (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_MSTAR_XC_HDR_SUPPORT))
    MDrv_XC_CFDControl(&stCfdCtrlInfo);
    return TRUE;
#else
    printf("MDrv_PQ_SendCfd unsupport OS type\n");
#endif
#endif
#endif
    return FALSE;
}
MS_U16 QM_InputSourceToIndex(void* pInstance, PQ_WIN eWindow, MS_U8 enInputSourceType)
{
    MS_U16 u16SrcType = 0;
    MS_U16 u16Input_HSize = QM_GetInputHSize(eWindow);
    MS_U16 u16Input_VSize = QM_GetInputVSize(eWindow);
    MS_U16 u16Input_VFreq = QM_GetInputVFreq(eWindow);
    MS_BOOL bIsInterlaced = QM_IsInterlaced(eWindow);

#if (PQ_QM_CVBS)
    PQ_VIDEOSTANDARD_TYPE eStandard;
#endif

#if (PQ_QM_HDMI_PC) || (PQ_QM_PC)
    MS_U16 u16Disp_HSize = QM_GetDispHSize(eWindow);
    MS_U16 u16Disp_VSize = QM_GetDispVSize(eWindow);

    PQMODE_DBG(printf("\r\n QM: Src = %u, H = %u, V = %u", enInputSourceType, u16Input_HSize, u16Input_VSize));
    PQMODE_DBG(printf("\r\n     VFreq = %u, IsInterlace = %u", u16Input_VFreq, (MS_U16)bIsInterlaced));
    PQMODE_DBG(printf("\r\n     DispH = %u, DispV = %u", u16Disp_HSize, u16Disp_VSize));
#endif

#if (PQ_QM_BYPASS_COLOR)

    //  Only when bypass mode is on .
    if (_bBypassModeOn[eWindow])
    {
        if (bIsInterlaced)
            u16SrcType = QM_BypassColor_interlace_Main;
        else
            u16SrcType = QM_BypassColor_progressive_Main;

        return u16SrcType;
    }
#endif

#if ENABLE_VGA_EIA_TIMING
    // Only Progressiv mode
    if ((QM_IsSourceVGA(enInputSourceType) || QM_IsSourceDVI_HDMIPC(enInputSourceType, eWindow)) && (!bIsInterlaced))
#else
    // Only DVI progressive mode, VGA could be interlace
    if (QM_IsSourceVGA(enInputSourceType) || (QM_IsSourceDVI_HDMIPC(enInputSourceType, eWindow) && !bIsInterlaced))
#endif
    {
        //DVI YUV mode is some special pattern gen timing in Korea company
        if(QM_IsSourceHDMI(enInputSourceType) && QM_HDMIPC_COLORYUV444(eWindow))
        {
#if (PQ_QM_HDMI_PC)
            // PC YUV444
            if (u16Input_HSize < u16Disp_HSize)
            {
                if (u16Input_VSize < u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_444_PC_Hup_Vup_Main;
                }
                else if (u16Input_VSize > u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_444_PC_Hup_Vdown_Main;
                }
                else
                {
                    u16SrcType = QM_HDMI_444_PC_Hup_Vno_Main;
                }
            }
            else if (u16Input_HSize > u16Disp_HSize)
            {
                if (u16Input_VSize < u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_444_PC_Hdown_Vup_Main;
                }
                else if (u16Input_VSize > u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_444_PC_Hdown_Vdown_Main;
                }
                else
                {
                    u16SrcType = QM_HDMI_444_PC_Hdown_Vno_Main;
                }
            }
            else
            {
                if (u16Input_VSize < u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_444_PC_Hno_Vup_Main;
                }
                else if (u16Input_VSize > u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_444_PC_Hno_Vdown_Main;
                }
                else
                {
                    u16SrcType = QM_HDMI_444_PC_Hno_Vno_Main;
                }
            }
#endif
        }
        else if (QM_IsSourceHDMI(enInputSourceType) && QM_HDMIPC_COLORYUV422(eWindow))
        {
#if (PQ_QM_HDMI_PC)
            // PC YUV422
            if (u16Input_HSize < u16Disp_HSize)
            {
                if (u16Input_VSize < u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_422_PC_Hup_Vup_Main;
                }
                else if (u16Input_VSize > u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_422_PC_Hup_Vdown_Main;
                }
                else
                {
                    u16SrcType = QM_HDMI_422_PC_Hup_Vno_Main;
                }
            }
            else if (u16Input_HSize > u16Disp_HSize)
            {
                if (u16Input_VSize < u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_422_PC_Hdown_Vup_Main;
                }
                else if (u16Input_VSize > u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_422_PC_Hdown_Vdown_Main;
                }
                else
                {
                    u16SrcType = QM_HDMI_422_PC_Hdown_Vno_Main;
                }
            }
            else
            {
                if (u16Input_VSize < u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_422_PC_Hno_Vup_Main;
                }
                else if (u16Input_VSize > u16Disp_VSize)
                {
                    u16SrcType = QM_HDMI_422_PC_Hno_Vdown_Main;
                }
                else
                {
                    u16SrcType = QM_HDMI_422_PC_Hno_Vno_Main;
                }
            }
#endif
        }
        else //(QM_HDMIPC_COLORRGB(eWindow)) & default
        {
#if (PQ_QM_PC)
            // PC RGB
            if (u16Input_HSize < u16Disp_HSize)
            {
                if (u16Input_VSize < u16Disp_VSize)
                {
                    u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vup_Main;
                }
                else if (u16Input_VSize > u16Disp_VSize)
                {
                    u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vdown_Main;
                }
                else
                {
                    u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hup_Vno_Main;
                }
            }
            else if (u16Input_HSize > u16Disp_HSize)
            {
                if (u16Input_VSize < u16Disp_VSize)
                {
                    u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vup_Main;
                }
                else if (u16Input_VSize > u16Disp_VSize)
                {
                    u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vdown_Main;
                }
                else
                {
                    u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hdown_Vno_Main;
                }
            }
            else
            {
                if (u16Input_VSize < u16Disp_VSize)
                {
                    u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vup_Main;
                }
                else if (u16Input_VSize > u16Disp_VSize)
                {
                    u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vdown_Main;
                }
                else
                {
                    u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vno_Main;
                }
            }
#endif
        }
    }
    else if (QM_IsSourceHDMI(enInputSourceType)
#if ENABLE_VGA_EIA_TIMING
             || QM_IsSourceVGA(enInputSourceType) //interlace VGA comes here
#endif
            )
    {
#if (PQ_QM_HMDI)
#if 0//debug message
    if(QM_HDMIPC_COLORRGB(eWindow))
        printf("HDMI RGB\n");
    else if(QM_HDMIPC_COLORYUV444(eWindow))
        printf("HDMI YUV 444\n");
    else
        printf("HDMI YUV 422\n");
#endif
if( QM_IsSourceHDMI(enInputSourceType) && QM_HDMIPC_COLORRGB(eWindow) &&
            (MDrv_PQ_Get_PointToPoint_U2(pInstance, eWindow)) && (!bIsInterlaced) &&
            (!PQ_IS_4K2K_INPUT(u16Input_HSize,u16Input_VSize)))
        {
            // HDMI RGB progressive
            u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vno_Main;
        }
        else if( QM_IsSourceHDMI(enInputSourceType) && QM_HDMIPC_COLORYUV444(eWindow) &&
                    (MDrv_PQ_Get_PointToPoint_U2(pInstance,eWindow)) && (!bIsInterlaced) &&
                     (!PQ_IS_4K2K_INPUT(u16Input_HSize,u16Input_VSize)))
        {
           //  HDMI YUV444 progressive
            u16SrcType = QM_HDMI_444_PC_Hno_Vno_Main;
        }
#if (PQ_SUPPORT_HDMI_4K2K_P2P == 1)
        else if( QM_IsSourceHDMI(enInputSourceType) && QM_HDMIPC_COLORRGB(eWindow) &&
                 (MDrv_PQ_Get_PointToPoint_U2(pInstance,eWindow)) && (!bIsInterlaced) &&
                 PQ_IS_4K2K_INPUT(u16Input_HSize,u16Input_VSize))
        {
            // HDMI 4K2K RGB progressive
            u16SrcType = QM_DVI_Dsub_HDMI_RGB_PC_Hno_Vno_4K_Main;
        }
        else if( QM_IsSourceHDMI(enInputSourceType) && QM_HDMIPC_COLORYUV444(eWindow) &&
                 (MDrv_PQ_Get_PointToPoint_U2(pInstance,eWindow)) && (!bIsInterlaced) &&
                 PQ_IS_4K2K_INPUT(u16Input_HSize,u16Input_VSize))
        {
           //  HDMI YUV444 progressive
            u16SrcType = QM_HDMI_444_PC_Hno_Vno_4K_Main;
        }
#endif
        else
        {
            // HDMI
            if ((u16Input_HSize < 800) && (u16Input_VSize < 500))
            {
                if (bIsInterlaced)
                    u16SrcType = QM_HDMI_480i_Main;
                else
                    u16SrcType = QM_HDMI_480p_Main;
            }
            else if ((u16Input_HSize < 800) && (u16Input_VSize < 600))
            {
                if (bIsInterlaced)
                    u16SrcType = QM_HDMI_576i_Main;
                else
                    u16SrcType = QM_HDMI_576p_Main;
            }
            else if ((u16Input_HSize < 1300) && (u16Input_VSize < 800) && (!bIsInterlaced))
            {
                if (u16Input_VFreq > 550)
                {
                    u16SrcType = QM_HDMI_720p_60hz_Main;
                }
                else if(u16Input_VFreq > 250)
                {
                    u16SrcType = QM_HDMI_720p_50hz_Main;
                }
                else
                {
                    u16SrcType = QM_HDMI_720p_24hz_Main;
                }
            }
#if (PQ_QM_HDMI_4K2K)
            else if(PQ_IS_4K2K_INPUT(u16Input_HSize,u16Input_VSize))
            {
                if (u16Input_VFreq > 280)
                {
                    u16SrcType = QM_4K2K_30Hz_Main;
                }
                else
                {
                    u16SrcType = QM_4K2K_24Hz_Main;
                }
            }
#endif
            else
            {
                if (bIsInterlaced)
                {
#if PQ_QM_HDMI_FP_VIDEO
                    if(u16Input_VSize > 2000) //1920x2228 i.it will not go this path now, but may used in future.
                    {
                        u16SrcType = QM_HDMI_FP_1080i_Main;
                    }
                    else
#endif
                    if (u16Input_VFreq > 550)
                    {
                        u16SrcType = QM_HDMI_1080i_60hz_Main;
                    }
                    else
                    {
                        u16SrcType = QM_HDMI_1080i_50hz_Main;
                    }
                }
                else
                {
#if PQ_QM_HDMI_FP_VIDEO
                    if((u16Input_HSize > 1910)
                       && (u16Input_HSize < 1930)
                       && (u16Input_VSize > 2218)
                        && (u16Input_VSize < 2238)) //1920x2228 i frame packing
                    {
                        u16SrcType = QM_HDMI_FP_1080i_Main;
                    }
                    else if(((u16Input_HSize > 1430) && (u16Input_HSize < 1450))
                        && (((u16Input_VSize > 591) && (u16Input_VSize < 611)) || ((u16Input_VSize > 493) && (u16Input_VSize < 513))))
                    {
                        u16SrcType = QM_HDMI_FP_1080i_Main;
                    }
                    else if((u16Input_HSize > 1910)
                            && (u16Input_HSize < 1930)
                            && (u16Input_VSize > 2195)
                            && (u16Input_VSize < 2215)) //1920x2205 p frame packing
                    {
                        u16SrcType = QM_HDMI_FP_1080p_Main;
                    }
                    else if((u16Input_HSize > 1270)
                            && (u16Input_HSize < 1290)
                            && (u16Input_VSize > 1460)
                            && (u16Input_VSize < 1480)) //1280x1470 p frame packing
                    {
                        u16SrcType = QM_HDMI_FP_720p_Main;
                    }
                    else
#endif
                    // 1500 is for framepacking case vsize 2205
                    if ((u16Input_VFreq > 550) || (u16Input_VSize > 1500))
                    {
                        u16SrcType = QM_HDMI_1080p_60hz_Main;
                    }
                    else if(u16Input_VFreq > GateFor30P)
                    {
                        u16SrcType = QM_HDMI_1080p_50hz_Main;
                    }
                    else  // Other timing
                    {
                        u16SrcType = QM_HDMI_1080p_24hz_Main;
                    }
                }
            }
        }
#endif
    }
    else if (QM_IsSourceYPbPr(enInputSourceType))
    {
#if (PQ_QM_YPBPR)
        if( QM_IsSourceYPbPr(enInputSourceType) &&
            (MDrv_PQ_Get_PointToPoint_U2(pInstance,eWindow)) && (!bIsInterlaced))
        {
            // YUV444 progressive
            u16SrcType = QM_HDMI_444_PC_Hno_Vno_Main;
        }
        else if (QM_IsYPbPr_720x480_60I(eWindow)){
            u16SrcType = QM_YPbPr_480i_Main;
        }
        else if (QM_IsYPbPr_720x480_60P(eWindow)){
            u16SrcType = QM_YPbPr_480p_Main;
        }
        else if (QM_IsYPbPr_720x576_50I(eWindow)){
            u16SrcType = QM_YPbPr_576i_Main;
        }
        else if (QM_IsYPbPr_720x576_50P(eWindow)){
            u16SrcType = QM_YPbPr_576p_Main;
        }
        else if (QM_IsYPbPr_1280x720_50P(eWindow)){
            u16SrcType = QM_YPbPr_720p_50hz_Main;
        }
        else if (QM_IsYPbPr_1280x720_60P(eWindow)){
            u16SrcType = QM_YPbPr_720p_60hz_Main;
        }
        else if (QM_IsYPbPr_1920x1080_50I(eWindow)){
            u16SrcType = QM_YPbPr_1080i_50hz_Main;
        }
        else if (QM_IsYPbPr_1920x1080_60I(eWindow)){
            u16SrcType = QM_YPbPr_1080i_60hz_Main;
        }
        else if (QM_IsYPbPr_1920x1080_24P(eWindow) || QM_IsYPbPr_1920x1080_30P(eWindow) || QM_IsYPbPr_1920x1080_25P(eWindow) ){
            u16SrcType = QM_YPbPr_1080p_24hz_Main;
        }
        else if (QM_IsYPbPr_1920x1080_50P(eWindow)){
            u16SrcType = QM_YPbPr_1080p_50hz_Main;
        }
        else if (QM_IsYPbPr_1920x1080_60P(eWindow)){
            u16SrcType = QM_YPbPr_1080p_60hz_Main;
        }
        else {
            u16SrcType = QM_YPbPr_720p_24hz_Main;
        }
#endif
    }
    else if (QM_IsSourceDTV(enInputSourceType))
    {
#if (PQ_QM_DTV)
        if(QM_IsDTV_IFRAME(eWindow))
        {
          #if 1 //( CHIP_FAMILY_TYPE != CHIP_FAMILY_S7J)
            //If PQ has devide the iframe setting according to P/I mode, then use new PQ srctype
            if (u16Input_VSize < 720)
            {
                if (bIsInterlaced)
                    u16SrcType = QM_DTV_iFrame_SD_interlace_Main;
                else
                    u16SrcType = QM_DTV_iFrame_SD_progressive_Main;
            }
            else
            {
                if (bIsInterlaced)
                    u16SrcType = QM_DTV_iFrame_HD_interlace_Main;
                else
                    u16SrcType = QM_DTV_iFrame_HD_progressive_Main;
            }
          #else
             u16SrcType = QM_DTV_iFrame_Main;
          #endif
        }
        else if (QM_IsDTV_MPEG2(eWindow))
        {
            if (u16Input_VSize < 500)
            {
                if ((u16Input_HSize < 400) && bIsInterlaced)
                {
                    u16SrcType = QM_DTV_480i_352x480_MPEG2_Main;
                }
                else
                {
                    if (bIsInterlaced)
                        u16SrcType = QM_DTV_480i_MPEG2_Main;
                    else
                        u16SrcType = QM_DTV_480p_MPEG2_Main;
                }
            }
            else if (u16Input_VSize < 650)
            {
                if (bIsInterlaced)
                    u16SrcType = QM_DTV_576i_MPEG2_Main;
                else
                    u16SrcType = QM_DTV_576p_MPEG2_Main;
            }
            else if ((u16Input_VSize < 900) && (bIsInterlaced==0))
            {
                if (u16Input_VFreq > 550)
                {
                    u16SrcType = QM_DTV_720p_60hz_MPEG2_Main;
                }
                else if(u16Input_VFreq > 250)
                {
                    u16SrcType = QM_DTV_720p_50hz_MPEG2_Main;
                }
                else
                {
                    u16SrcType = QM_DTV_720p_24hz_MPEG2_Main;
                }
            }
            else
            {
                if (bIsInterlaced)
                {
                    #if PQ_QM_DTV_4K2K
                    if(PQ_IS_DTV_4K_INPUT(u16Input_HSize,u16Input_VSize))  // 4K2K case
                    {
                        #if (PQ_QM_4K2K_60Hz)
                        if(u16Input_VFreq > GateFor30P)
                            u16SrcType = QM_DTV_4K2K_60Hz_MPEG2_Main;
                        else
                        #endif
                            u16SrcType = QM_DTV_4K2K_MPEG2_Main;
                    }
                    else
                    #endif
                    if (u16Input_VFreq > 550)
                    {
                        u16SrcType = QM_DTV_1080i_60hz_MPEG2_Main;
                    }
                    else
                    {
                        u16SrcType = QM_DTV_1080i_50hz_MPEG2_Main;
                    }
                }
                else
                {
                    #if PQ_QM_DTV_4K2K
                    if(PQ_IS_DTV_4K_INPUT(u16Input_HSize,u16Input_VSize))  // 4K2K case
                    {
                        #if (PQ_QM_4K2K_60Hz)
                        if(u16Input_VFreq > GateFor30P)
                            u16SrcType = QM_DTV_4K2K_60Hz_MPEG2_Main;
                        else
                        #endif
                            u16SrcType = QM_DTV_4K2K_MPEG2_Main;
                    }
                    else
                    #endif
                    if (u16Input_VFreq > 550)
                    {
                        u16SrcType = QM_DTV_1080p_60hz_MPEG2_Main;
                    }
                    else if(u16Input_VFreq > 250)
                    {
                        u16SrcType = QM_DTV_1080p_50hz_MPEG2_Main;
                    }
                    else
                    {
                        u16SrcType = QM_DTV_1080p_24hz_MPEG2_Main;
                    }

                }
            }
        }
        else // QM_IsDTV_H264(eWindow)
        {
            if ((u16Input_HSize < 1260) && (u16Input_VSize < 650))
            {
                if (u16Input_VSize < 500)
                {
                    if ((u16Input_HSize < 400) && bIsInterlaced)
                    {
                        u16SrcType = QM_DTV_480i_352x480_H264_Main;
                    }
                    else
                    {
                        if (bIsInterlaced)
                            u16SrcType = QM_DTV_480i_H264_Main;
                        else
                            u16SrcType = QM_DTV_480p_H264_Main;
                    }
                }
                else
                {
                    if (bIsInterlaced)
                        u16SrcType = QM_DTV_576i_H264_Main;
                    else
                        u16SrcType = QM_DTV_576p_H264_Main;
                }
            }
            else
            {
                if ((u16Input_VSize < 900) && (bIsInterlaced==0))
                {
                    if (u16Input_VFreq > 550)
                    {
                        u16SrcType = QM_DTV_720p_60hz_H264_Main;
                    }
                    else if(u16Input_VFreq > 250)
                    {
                        u16SrcType = QM_DTV_720p_50hz_H264_Main;
                    }
                    else
                    {
                        u16SrcType = QM_DTV_720p_24hz_H264_Main;
                    }
                }
                else
                {
                    if (bIsInterlaced)
                    {
                        #if PQ_QM_DTV_4K2K
                        if(PQ_IS_DTV_4K_INPUT(u16Input_HSize,u16Input_VSize))  // 4K2K case
                        {
                            #if (PQ_QM_4K2K_60Hz)
                            if(u16Input_VFreq > GateFor30P)
                                u16SrcType = QM_DTV_4K2K_60Hz_H264_Main;
                            else
                            #endif
                                u16SrcType = QM_DTV_4K2K_H264_Main;
                        }
                        else
                        #endif
                        if (u16Input_VFreq > 550)
                        {
                            u16SrcType = QM_DTV_1080i_60hz_H264_Main;
                        }
                        else
                        {
                            u16SrcType = QM_DTV_1080i_50hz_H264_Main;
                        }
                    }
                    else
                    {
                        #if PQ_QM_DTV_4K2K
                        if(PQ_IS_DTV_4K_INPUT(u16Input_HSize,u16Input_VSize))  // 4K2K case
                        {
                            #if (PQ_QM_4K2K_60Hz)
                            if(u16Input_VFreq > GateFor30P)
                                u16SrcType = QM_DTV_4K2K_60Hz_H264_Main;
                            else
                            #endif
                                u16SrcType = QM_DTV_4K2K_H264_Main;
                        }
                        else
                        #endif
                        if (u16Input_VFreq > 550)
                        {
                            u16SrcType = QM_DTV_1080p_60hz_H264_Main;
                        }
                        else if(u16Input_VFreq > 250)
                        {
                            u16SrcType = QM_DTV_1080p_50hz_H264_Main;
                        }
                        else
                        {
                            u16SrcType = QM_DTV_1080p_24hz_H264_Main;
                        }
                    }
                }
            }
        }
#endif
    }
    else if (QM_IsSourceMultiMedia(enInputSourceType))
    {
#if (PQ_QM_MM_VIDEO) || (PQ_QM_MM_PHOTO)
#if PQ_QM_NETMM_VIDEO
        if(_gIsNetworkMM)
        {
            if ((u16Input_HSize <= 720) && (u16Input_VSize <= 576))
            {
                if (bIsInterlaced)
                {
                    u16SrcType = QM_Multimedia_video_online_SD_interlace_Main;
                }
                else
                {
                    if(u16Input_VFreq > GateFor30P)
                    {
                        u16SrcType = QM_Multimedia_video_online_SD_progressive_Main;
                    }
                    else
                    {
                        u16SrcType = QM_Multimedia_video_online_SD_progressive_24hz_Main;
                    }
                }
            }
            else
            {
                if (bIsInterlaced)
                {
                    u16SrcType = QM_Multimedia_video_online_HD_interlace_Main;
                }
                else
                {
                    if(u16Input_VFreq > GateFor30P)
                    {
                        u16SrcType = QM_Multimedia_video_online_HD_progressive_Main;
                    }
                    else
                    {
                        u16SrcType = QM_Multimedia_video_online_HD_progressive_24hz_Main;
                    }
                }
            }
        }
        else
#endif
#if ((defined  (ANDROID)||defined(MSOS_TYPE_LINUX_KERNEL)) && defined(UFO_XC_DS_PQ))
    if(MApi_XC_Is_SupportSWDS())
    {
        _stMultiMedia_Info[eWindow].eType = E_PQ_MULTIMEDIA_MOVIE;
    }
#endif
        if (QM_IsMultiMediaMOVIE(eWindow))
        {
#if PQ_QM_HDMI_FP_VIDEO
            if((u16Input_HSize > 1910)
               && (u16Input_HSize < 1930)
               && (u16Input_VSize > 2218)
                && (u16Input_VSize < 2238)) //1920x2228 i frame packing
            {
                u16SrcType = QM_HDMI_FP_1080i_Main;
            }
            else if(((u16Input_HSize > 1430) && (u16Input_HSize < 1450))
                && (((u16Input_VSize > 591) && (u16Input_VSize < 611)) || ((u16Input_VSize > 493) && (u16Input_VSize < 513))))
            {
                u16SrcType = QM_HDMI_FP_1080i_Main;
            }
            else if((u16Input_HSize > 1910)
                    && (u16Input_HSize < 1930)
                    && (u16Input_VSize > 2195)
                    && (u16Input_VSize < 2215)) //1920x2205 p frame packing
            {
                u16SrcType = QM_HDMI_FP_1080p_Main;
            }
            else if((u16Input_HSize > 1270)
                    && (u16Input_HSize < 1290)
                    && (u16Input_VSize > 1460)
                    && (u16Input_VSize < 1480)) //1280x1470 p frame packing
            {
                u16SrcType = QM_HDMI_FP_720p_Main;
            }
            else
#endif

            if ((u16Input_HSize <= 720) && (u16Input_VSize <= 576))
            {
                if (bIsInterlaced)
                {
                    u16SrcType = QM_Multimedia_video_SD_interlace_Main;
                }
                else
                {
#if PQ_QM_24HZMM_VIDEO
                    if(u16Input_VFreq > GateFor30P)
                    {
                        u16SrcType = QM_Multimedia_video_SD_progressive_Main;
                    }
                    else
                    {
                        u16SrcType = QM_Multimedia_video_SD_progressive_24hz_Main;
                    }
#else
                        u16SrcType = QM_Multimedia_video_SD_progressive_Main;
#endif
                }
            }
            else
            {
                if (bIsInterlaced)
                {
                    u16SrcType = QM_Multimedia_video_HD_interlace_Main;
                }
                else
                {
#if PQ_QM_24HZMM_VIDEO
#if (PQ_QM_4K2K)
                    if(_gIsMVC4kx1k)
                    {
                        u16SrcType = QM_Multimedia_video_4K2K_FS_progressive_Main;
                    }
                    else
#endif
                    if(u16Input_VFreq > GateFor30P)
                    {
                        u16SrcType = QM_Multimedia_video_HD_progressive_Main;
                    }
                    else
                    {
                        u16SrcType = QM_Multimedia_video_HD_progressive_24hz_Main;
                    }
#else
#if (PQ_QM_4K2K)
                    if(_gIsMVC4kx1k)
                    {
                        u16SrcType = QM_Multimedia_video_4K2K_FS_progressive_Main;
                    }
                    else
#endif
                    {
                        u16SrcType = QM_Multimedia_video_HD_progressive_Main;
                    }
#endif

#if (PQ_QM_4K2K)
                    if((u16Input_HSize > 3800) && (u16Input_VSize > 1050) && (u16Input_VSize < 1900))
                    {
                        //#if(ENABLE_4K2K_NIKEU == 1)
                        //u16SrcType = QM_MM_4K1K_Main;
                        //#else
                        #if (PQ_QM_4K2K_60Hz)
                        if(u16Input_VFreq > GateFor30P)
                            u16SrcType = QM_MM_4K2K_60Hz_Main;
                        else
                        #endif
                            u16SrcType = QM_MM_4K2K_Main;
                        //#endif
                    }
                       else if(PQ_IS_4K2K_INPUT(u16Input_HSize,u16Input_VSize))
                    {
                        #if (PQ_QM_4K2K_60Hz)
                        if(u16Input_VFreq > GateFor30P)
                            u16SrcType = QM_MM_4K2K_60Hz_Main;
                        else
                        #endif
                            u16SrcType = QM_MM_4K2K_Main;
                    }
#endif
                }
            }
        }
        else //QM_IsMultiMediaPHOTO(eWindow)
        {
            if ((u16Input_HSize <= 720) && (u16Input_VSize <= 576))
            {
                u16SrcType = QM_Multimedia_photo_SD_progressive_Main;
            }
#if (PQ_QM_4K2K)
            ///for 4K photo
            else if((u16Input_HSize < 1500) && (u16Input_VSize >= 1900) )
            {
                u16SrcType = QM_MM_non_4K2K_Main;
            }
            else if((u16Input_HSize > 3800) && (u16Input_VSize > 1050))
            {
                u16SrcType = QM_MM_4K2K_Photo_Main;
            }
#endif
            else
            {
                u16SrcType = QM_Multimedia_photo_HD_progressive_Main;
            }
        }
#endif
    }
    else if (QM_IsSourceScartCVBS(enInputSourceType, eWindow) || QM_IsSourceScartRGB(enInputSourceType, eWindow))
    {
#if (PQ_QM_CVBS)
        eStandard = QM_GetATVStandard(eWindow);
        switch(eStandard)
        {
        case E_PQ_VIDEOSTANDARD_PAL_M:
            u16SrcType = QM_SCART_AV_PAL_M_Main;
            break;
        case E_PQ_VIDEOSTANDARD_PAL_N:
            u16SrcType = QM_SCART_AV_PAL_N_Main;
            break;
        case E_PQ_VIDEOSTANDARD_NTSC_44:
            u16SrcType = QM_SCART_AV_NTSC_44_Main;
            break;
        case E_PQ_VIDEOSTANDARD_PAL_60:
            u16SrcType = QM_SCART_AV_PAL_60_Main;
            break;
        case E_PQ_VIDEOSTANDARD_NTSC_M:
            u16SrcType = QM_SCART_AV_NTSC_M_Main;
            break;
        case E_PQ_VIDEOSTANDARD_SECAM:
            u16SrcType = QM_SCART_AV_SECAM_Main;
            break;
        case E_PQ_VIDEOSTANDARD_PAL_BGHI:
        default:
            u16SrcType = QM_SCART_AV_PAL_BGHI_Main;
            break;
        }
#endif
    }
    else if (QM_IsSourceATV(enInputSourceType))
    {
#if (PQ_QM_CVBS)
        eStandard = QM_GetATVStandard(eWindow);
        if (QM_FRONTEND_RFIN(eWindow))
        {
            switch(eStandard)
            {
            case E_PQ_VIDEOSTANDARD_PAL_M:
                u16SrcType = QM_RF_PAL_M_Main;
                break;
            case E_PQ_VIDEOSTANDARD_PAL_N:
                u16SrcType = QM_RF_PAL_N_Main;
                break;
            case E_PQ_VIDEOSTANDARD_NTSC_44:
                u16SrcType = QM_RF_NTSC_44_Main;
                break;
            case E_PQ_VIDEOSTANDARD_PAL_60:
                u16SrcType = QM_RF_PAL_60_Main;
                break;
            case E_PQ_VIDEOSTANDARD_NTSC_M:
                u16SrcType = QM_RF_NTSC_M_Main;
                break;
            case E_PQ_VIDEOSTANDARD_SECAM:
                u16SrcType = QM_RF_SECAM_Main;
                break;
            case E_PQ_VIDEOSTANDARD_PAL_BGHI:
            default:
                u16SrcType = QM_RF_PAL_BGHI_Main;
                break;
            }
        }
        else // QM_FRONTEND_VIFIN(eWindow)
        {
            switch(eStandard)
            {
            case E_PQ_VIDEOSTANDARD_PAL_M:
                u16SrcType = QM_VIF_PAL_M_Main;
                break;
            case E_PQ_VIDEOSTANDARD_PAL_N:
                u16SrcType = QM_VIF_PAL_N_Main;
                break;
            case E_PQ_VIDEOSTANDARD_NTSC_44:
                u16SrcType = QM_VIF_NTSC_44_Main;
                break;
            case E_PQ_VIDEOSTANDARD_PAL_60:
                u16SrcType = QM_VIF_PAL_60_Main;
                break;
            case E_PQ_VIDEOSTANDARD_NTSC_M:
                u16SrcType = QM_VIF_NTSC_M_Main;
                break;
            case E_PQ_VIDEOSTANDARD_SECAM:
                u16SrcType = QM_VIF_SECAM_Main;
                break;
            case E_PQ_VIDEOSTANDARD_PAL_BGHI:
            default:
                u16SrcType = QM_VIF_PAL_BGHI_Main;
                break;
            }
        }
#endif
    }
    else if (QM_IsSourceSV(enInputSourceType))
    {
#if (PQ_QM_CVBS)
        eStandard = QM_GetATVStandard(eWindow);
        switch(eStandard)
        {
        case E_PQ_VIDEOSTANDARD_PAL_M:
            u16SrcType = QM_SV_PAL_M_Main;
            break;
        case E_PQ_VIDEOSTANDARD_PAL_N:
            u16SrcType = QM_SV_PAL_N_Main;
            break;
        case E_PQ_VIDEOSTANDARD_NTSC_44:
            u16SrcType = QM_SV_NTSC_44_Main;
            break;
        case E_PQ_VIDEOSTANDARD_PAL_60:
            u16SrcType = QM_SV_PAL_60_Main;
            break;
        case E_PQ_VIDEOSTANDARD_NTSC_M:
            u16SrcType = QM_SV_NTSC_M_Main;
            break;
        case E_PQ_VIDEOSTANDARD_SECAM:
            u16SrcType = QM_SV_SECAM_Main;
            break;
        case E_PQ_VIDEOSTANDARD_PAL_BGHI:
        default:
            u16SrcType = QM_SV_PAL_BGHI_Main;
            break;
        }
#endif
    }
    else // AV
    {
#if (PQ_QM_CVBS)
        eStandard = QM_GetATVStandard(eWindow);
        switch(eStandard)
        {
        case E_PQ_VIDEOSTANDARD_PAL_M:
            u16SrcType = QM_AV_PAL_M_Main;
            break;
        case E_PQ_VIDEOSTANDARD_PAL_N:
            u16SrcType = QM_AV_PAL_N_Main;
            break;
        case E_PQ_VIDEOSTANDARD_NTSC_44:
            u16SrcType = QM_AV_NTSC_44_Main;
            break;
        case E_PQ_VIDEOSTANDARD_PAL_60:
            u16SrcType = QM_AV_PAL_60_Main;
            break;
        case E_PQ_VIDEOSTANDARD_NTSC_M:
            u16SrcType = QM_AV_NTSC_M_Main;
            break;
        case E_PQ_VIDEOSTANDARD_SECAM:
            u16SrcType = QM_AV_SECAM_Main;
            break;
        case E_PQ_VIDEOSTANDARD_PAL_BGHI:
        default:
            u16SrcType = QM_AV_PAL_BGHI_Main;
            break;
        }
#endif
    }

#ifdef PQ_CFD_INFO
    ST_PQ_CFD_ANALOG stCfdAnalog;
    memset(&stCfdAnalog, 0, sizeof(ST_PQ_CFD_ANALOG));
    stCfdAnalog.u32Version = CFD_ANALOG_VERSION;
    stCfdAnalog.u16Length = sizeof(ST_PQ_CFD_ANALOG);
    stCfdAnalog.u8Win = eWindow;
    if (Hal_PQ_get_cfd_info(u16SrcType, &stCfdAnalog.u8ColorFormat, &stCfdAnalog.u8ColorDataFormat, &stCfdAnalog.bIsFullRange, &stCfdAnalog.u8ColorPrimaries, &stCfdAnalog.u8TransferCharacteristics, &stCfdAnalog.u8MatrixCoefficients) == TRUE)
    {
        if (QM_IsSourceVGA(enInputSourceType))
        {
            MDrv_PQ_SendCfd(E_PQ_CFD_CTRL_SET_VGA, &stCfdAnalog, sizeof(ST_PQ_CFD_ANALOG));
        }
        else if (QM_IsSourceATV(enInputSourceType))
        {
            MDrv_PQ_SendCfd(E_PQ_CFD_CTRL_SET_TV, &stCfdAnalog, sizeof(ST_PQ_CFD_ANALOG));
        }
        else if (QM_IsSourceAV(enInputSourceType))
        {
            MDrv_PQ_SendCfd(E_PQ_CFD_CTRL_SET_CVBS, &stCfdAnalog, sizeof(ST_PQ_CFD_ANALOG));
        }
        else if (QM_IsSourceSV(enInputSourceType))
        {
            MDrv_PQ_SendCfd(E_PQ_CFD_CTRL_SET_SVIDEO, &stCfdAnalog, sizeof(ST_PQ_CFD_ANALOG));
        }
        else if (QM_IsSourceYPbPr(enInputSourceType))
        {
            MDrv_PQ_SendCfd(E_PQ_CFD_CTRL_SET_YPBPR, &stCfdAnalog, sizeof(ST_PQ_CFD_ANALOG));
        }
        else if (QM_IsSourceScart(enInputSourceType))
        {
            MDrv_PQ_SendCfd(E_PQ_CFD_CTRL_SET_SCART, &stCfdAnalog, sizeof(ST_PQ_CFD_ANALOG));
        }
        else if (QM_IsSourceDVI_HDMIPC(enInputSourceType, eWindow))
        {
            MDrv_PQ_SendCfd(E_PQ_CFD_CTRL_SET_DVI, &stCfdAnalog, sizeof(ST_PQ_CFD_ANALOG));
        }
    }
#endif

    return u16SrcType;
}

#ifdef UFO_XC_PQ_SUPPORT_REPEATER_COLOR_THROUGH_MODE
void MDrv_PQ_SetColorThroughMode_U2(void* pInstance, PQ_WIN eWindow, MS_BOOL bEn)
{
	_gIsThroughMode = bEn;
}

void MDrv_PQ_SetColorThroughMode(PQ_WIN eWindow, MS_BOOL bEn)
{
#ifdef TRACE_DBG
    printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
	if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_SetColorThroughMode PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bEn = bEn;
    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_COLOR_THROUGH_MODE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return;
    }
    else
    {
        return;
    }
}
#endif


MS_U16 QM_InputSourceToIndex_UFSC(void* pInstance,PQ_WIN eWindow, MS_U8 enInputSourceType)
{
    MS_U16 u16SrcType = 0;
#if defined(ENABLE_PQ_BIN_UFSC) && ENABLE_PQ_BIN_UFSC
    MS_U16 u16Input_HSize = QM_GetInputHSize(eWindow);
    MS_U16 u16Input_VSize = QM_GetInputVSize(eWindow);
    MS_BOOL bRGB444 = FALSE, bYUV422 = FALSE;

    XC_ApiStatusEx stDrvStatusEx;
    memset(&stDrvStatusEx, 0, sizeof(XC_ApiStatusEx));
    stDrvStatusEx.u16ApiStatusEX_Length = sizeof(XC_ApiStatusEx);
    stDrvStatusEx.u32ApiStatusEx_Version = API_STATUS_EX_VERSION;
    MApi_XC_GetStatusEx(&stDrvStatusEx, MAIN_WINDOW);
    if (stDrvStatusEx.bMemYUVFmt)
    {
        if (stDrvStatusEx.bMemFmt422)
        {
            bYUV422 = TRUE;
        }
    }
    else
    {
        bRGB444 = TRUE;
    }

#if(ENABLE_PQ_UFSC_DLC)
    if(E_XC_3D_OUTPUT_MODE_NONE != MApi_XC_Get_3D_Output_Mode())
    {
        u16SrcType = QM_FSC_3D_UFSC; /// 3d
    }
    else
#endif
 if(
    ((MDrv_PQBin_GetPanelIdx_UFSC(pInstance)==PQ_BIN_PNLTYPE_4K2K) && (MDrv_PQ_GetPQPathStatus(E_PQ_UFSC_4K_PATH, u16Input_HSize, u16Input_VSize))) ||
    (!(MDrv_PQBin_GetPanelIdx_UFSC(pInstance)==PQ_BIN_PNLTYPE_4K2K) )
    )
    {
#if(ENABLE_PQ_UFSC_BYPASS)
    #if (PQ_QM_DTV)
        if((QM_IsSourceDTV(enInputSourceType)) && (QM_IsDTV_IFRAME(eWindow)))
        {
            u16SrcType = QM_4K2K_PQbypass_UFSC; //QM_4K2K_PQbypass_UFSC
        }
        else
    #endif
#endif
        {
            if (bRGB444)
            {
                if( MApi_XC_GetDynamicScalingStatus())
                {
                    u16SrcType = QM_4K2K_RGB444_DS_UFSC;//4K2K_RGB444_DS
                }
                else
                {
                   u16SrcType = QM_4K2K_RGB444_UFSC; //4K2K_RGB444
                }
            }
            else if (bYUV422)
            {
                if( MApi_XC_GetDynamicScalingStatus())
                {
                    u16SrcType = QM_4K2K_YUV422_DS_UFSC;//4K2K_YUV422_DS
                }
                else
                {
                    u16SrcType = QM_4K2K_YUV422_UFSC; //4K2K_YUV422
                }
            }
            else
            {
                if( MApi_XC_GetDynamicScalingStatus())
                {
                    u16SrcType = QM_4K2K_YUV444_DS_UFSC;//4K2K_YUV444_DS
                }
                else
                {
                    u16SrcType = QM_4K2K_YUV444_UFSC; //4K2K_YUV444
                }
            }
        }
    }
    else
    {
#if(ENABLE_PQ_UFSC_BYPASS)
    #if (PQ_QM_DTV)
        if((QM_IsSourceDTV(enInputSourceType)) && (QM_IsDTV_IFRAME(eWindow)))
        {
            u16SrcType = QM_FHD_PQBypass_UFSC; //QM_FHD_PQBypass_UFSC
        }
        else
    #endif
#endif
        {
            if (bRGB444)
            {
                if( MApi_XC_GetDynamicScalingStatus())
                {
                    u16SrcType = QM_FHD_RGB444_DS_UFSC; //FHD_RGB444_DS
                }
                else
                {
                    u16SrcType = QM_FHD_RGB444_UFSC; //FHD_RGB444
                }
            }
            else if (bYUV422)
            {
                if( MApi_XC_GetDynamicScalingStatus())
                {
                    u16SrcType = QM_FHD_YUV422_DS_UFSC; //FHD_YUV422_DS
                }
                else
                {
                    u16SrcType = QM_FHD_YUV422_UFSC; //FHD_YUV422
                }
            }
            else
            {
                if( MApi_XC_GetDynamicScalingStatus())
                {
                    u16SrcType = QM_FHD_YUV444_DS_UFSC; //FHD_YUV444_DS
                }
                else
                {
                    u16SrcType = QM_FHD_YUV444_UFSC; //FHD_YUV444
                }
            }
        }
    }
    printf("[Load UFSC PQ] Source index=%d\n", u16SrcType);
#endif
    return u16SrcType;
}
#if (ENABLE_PQ_BIN_CF)
MS_U16 QM_InputSourceToIndex_CF(void* pInstance,PQ_WIN eWindow, MS_U8 enInputSourceType)
{
    MS_U16 u16SrcType = 0;

#if (CFD_STATUS_VERSION >=2)
    XC_CFD_STATUS stCfdStatus;
    memset(&stCfdStatus, 0, sizeof(XC_CFD_STATUS));
    stCfdStatus.u32Version=CFD_STATUS_VERSION;
    stCfdStatus.u16Length=sizeof(XC_CFD_STATUS);
    stCfdStatus.u8Win=eWindow;

     XC_CFD_CONTROL_INFO stCfdCtrlInfo;
     memset(&stCfdCtrlInfo, 0, sizeof(XC_CFD_CONTROL_INFO));
     stCfdCtrlInfo.enCtrlType = E_XC_CFD_CTRL_TYPE_STATUS;
     stCfdCtrlInfo.pParam = &stCfdStatus;
     stCfdCtrlInfo.u32ParamLen = sizeof(XC_CFD_STATUS);
     MApi_XC_HDR_Control(E_XC_HDR_CTRL_CFD_CONTROL, &stCfdCtrlInfo);

    u16SrcType=stCfdStatus.u8ColorType;
    printf("[Load CF PQ] Source index=%d\n", u16SrcType);
#else
    printf("Wrong Color format type, Please check CFD_STATUS_VERSION\n");
#endif
    return u16SrcType;
}
#endif
#if PQ_GRULE_OSD_BW_ENABLE
static void _MDrv_PQ_LoadOSD_BWTable(void* pInstance, PQ_WIN eWindow, MS_U16 u16PQ_OSD_BW_Idx)
{
    MS_U16 i, u16IPIdx = 0, u16TabIdx = 0;

    if(u16PQ_OSD_BW_Idx > PQ_GRULE_OSD_BW_NUM_Main)
    {
        MS_ASSERT(0);
        return;
    }

    for(i = 0; i < PQ_GRULE_OSD_BW_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex_U2(pInstance, eWindow, PQ_GRule_OSD_BW_Main, i);

        if(u16PQ_OSD_BW_Idx == PQ_GRULE_OSD_BW_NUM_Main)  // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, u16IPIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_OSD_BW_Main, u16PQ_OSD_BW_Idx, i);

        PQGRULE_DBG(printf("[NR]SRC: %u, NR: %u, NRIPIdx:%u, IPIdx:%u, u8TabIdx:%u\n",
                           _u16PQSrcType[eWindow], u16PQ_OSD_BW_Idx, i, u16IPIdx, u16IPIdx));

        MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx, u16IPIdx);
    }
}


void MDrv_PQ_LoadOSD_BWTable(void* pInstance, PQ_WIN eWindow, PQ_OSD_BW_FUNCTION_TYPE enOSD_BW_Type)
{
    PQGRULE_DBG(printf("[PQ_Load_OSD_BW_Table] "));
    MS_U16 u16PQ_NRIdx;

    if(enOSD_BW_Type == PQ_OSD_BW_ON)
    {
        PQGRULE_DBG(printf("On\n"));

#if (PQ_OSD_BW_VER == 1)
        #if PQ_GRULE_DDR_SELECT_ENABLE
        if(bw_info.u32Miu1MemSize == 0)
        {
            u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_1DDR_On_Main);
            _MDrv_PQ_LoadOSD_BWTable(pInstance, eWindow, u16PQ_NRIdx);
        }
        else
        {
            u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_2DDR_On_Main);
            _MDrv_PQ_LoadOSD_BWTable(pInstance, eWindow, u16PQ_NRIdx);
        }
        #else
        u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_On_Main);
        _MDrv_PQ_LoadOSD_BWTable(pInstance, eWindow, u16PQ_NRIdx);
       #endif
#elif (PQ_OSD_BW_VER == 2)
        #if PQ_GRULE_DDR_SELECT_ENABLE
        if(bw_info.bDDR2 == FALSE) // DDR 3
        {
             if(MApi_XC_GetMirrorModeTypeEx(MAIN_WINDOW) != MIRROR_NORMAL)
             {
                 PQMADi_DBG(printf("MDrv_PQ_GetMADiFromGrule_OSDBW_Ver2: DDR3 mirror\n"));
                 u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_On_DDR3_Mirror_Main);
                 _MDrv_PQ_LoadOSD_BWTable(pInstance, eWindow, u16PQ_NRIdx);
             }
             else
             {
                 PQMADi_DBG(printf("MDrv_PQ_GetMADiFromGrule_OSDBW_Ver2: DDR3 non-mirror\n"));
                 u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_On_DDR3_Main);
                 _MDrv_PQ_LoadOSD_BWTable(pInstance, eWindow, u16PQ_NRIdx);
             }
        }
        else // DDR 2
        {
             if(MApi_XC_GetMirrorModeTypeEx(MAIN_WINDOW) != MIRROR_NORMAL)
             {
                 PQMADi_DBG(printf("MDrv_PQ_GetMADiFromGrule_OSDBW_Ver2: DDR2 mirror\n"));
                 u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_On_DDR2_Mirror_Main);
                 _MDrv_PQ_LoadOSD_BWTable(pInstance, eWindow, u16PQ_NRIdx);
             }
             else
             {
                 PQMADi_DBG(printf("MDrv_PQ_GetMADiFromGrule_OSDBW_Ver2: DDR2 non-mirror\n"));
                 u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_On_DDR2_Main);
                 _MDrv_PQ_LoadOSD_BWTable(pInstance, eWindow, u16PQ_NRIdx);
             }
        }
        #endif
#endif
    }
    else
    {
        MS_ASSERT(0);
    }
}
#endif

#if PQ_GRULE_FILM_MODE_ENABLE
static void _MDrv_PQ_LoadFilmModeTable(void* pInstance, PQ_WIN eWindow, MS_U16 u16PQ_FilmModeIdx)
{
    MS_U16 i, u16IPIdx = 0, u16TabIdx = 0;
    if(u16PQ_FilmModeIdx > PQ_GRULE_FILM_MODE_NUM_Main)
    {
        MS_ASSERT(0);
        return;
    }

    for(i = 0; i < PQ_GRULE_FILM_MODE_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex_U2(pInstance, eWindow, PQ_GRule_FILM_MODE_Main, i);

        if(u16PQ_FilmModeIdx == PQ_GRULE_FILM_MODE_NUM_Main)  // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, u16TabIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_FILM_MODE_Main, u16PQ_FilmModeIdx, i);

        PQGRULE_DBG(printf("[FM]SRC: %u, FM: %u, FMIPIdx:%u, IPIdx:%u, u16TabIdx:%u\n",
                           _u16PQSrcType[eWindow], u16PQ_FilmModeIdx, i, u16IPIdx, u16TabIdx));
        MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx, u16IPIdx);
    }
}
#endif


void MDrv_PQ_LoadFilmModeTable_U2(void* pInstance,PQ_WIN eWindow, PQ_FILM_MODE_FUNCTION_TYPE enFilmModeType)
{
#if PQ_GRULE_FILM_MODE_ENABLE
    MS_U16 u16PQ_FilmModeIdx;

    if(enFilmModeType == PQ_FilmMode_OFF)
    {
        u16PQ_FilmModeIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_FILM_MODE_Main, PQ_GRule_FILM_MODE_Off_Main);
        _MDrv_PQ_LoadFilmModeTable(pInstance, eWindow, u16PQ_FilmModeIdx);

    }
    else if(enFilmModeType == PQ_FilmMode_ON)
    {
        u16PQ_FilmModeIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_FILM_MODE_Main, PQ_GRule_FILM_MODE_On_Main);
        _MDrv_PQ_LoadFilmModeTable(pInstance, eWindow, u16PQ_FilmModeIdx);
    }
    else
    {
        MS_ASSERT(0);
    }
#else
    UNUSED(eWindow);
    UNUSED(enFilmModeType);
#endif
}
void MDrv_PQ_LoadFilmModeTable(PQ_WIN eWindow, PQ_FILM_MODE_FUNCTION_TYPE enFilmModeType)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_LoadFilmModeTable PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.enFilmModeType = enFilmModeType;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_LOAD_FILMMODE_TABLE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}

#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
static void _MDrv_PQ_DynamicContrastTable(void* pInstance, PQ_WIN eWindow, MS_U16 u16PQ_DynContrIdx)
{
    MS_U16 i, u16IPIdx = 0, u16TabIdx = 0;
    if(u16PQ_DynContrIdx > PQ_GRULE_DYNAMIC_CONTRAST_NUM_Main)
    {
        MS_ASSERT(0);
        return;
    }

    for(i = 0; i < PQ_GRULE_DYNAMIC_CONTRAST_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex_U2(pInstance, eWindow, PQ_GRule_DYNAMIC_CONTRAST_Main, i);
#if PQ_DLC_CTL_FOR_MVC4KX1K
        if((_gIsMVC4kx1k == TRUE) && (u16IPIdx == PQ_IP_VIP_DLC_Main))
        {
            printf("[Grule-DynamicContrast] cannot load DLC table for MVC4kx1k\n");
            continue;
        }
#endif
        if(u16PQ_DynContrIdx == PQ_GRULE_DYNAMIC_CONTRAST_NUM_Main)  // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, u16TabIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_DYNAMIC_CONTRAST_Main, u16PQ_DynContrIdx, i);

        PQGRULE_DBG(printf("[DC]SRC: %u, DC: %u, DCIPIdx:%u, IPIdx:%u, u16TabIdx:%u\n",
                           _u16PQSrcType[eWindow], u16PQ_DynContrIdx, i, u16IPIdx, u16TabIdx));
        MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx, u16IPIdx);
    }
}
#endif

void MDrv_PQ_LoadDynamicContrastTable_U2(void* pInstance,PQ_WIN eWindow, PQ_DYNAMIC_CONTRAST_FUNCTION_TYPE enDynamicContrastType)
{
#if PQ_GRULE_DYNAMIC_CONTRAST_ENABLE
    MS_U16 u16PQ_DynContrIdx;

    if(enDynamicContrastType == PQ_DynContr_OFF)
    {
        u16PQ_DynContrIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_DYNAMIC_CONTRAST_Main, PQ_GRule_DYNAMIC_CONTRAST_Off_Main);
        _MDrv_PQ_DynamicContrastTable(pInstance, eWindow, u16PQ_DynContrIdx);
    }
    else if(enDynamicContrastType == PQ_DynContr_ON)
    {
        u16PQ_DynContrIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_DYNAMIC_CONTRAST_Main, PQ_GRule_DYNAMIC_CONTRAST_On_Main);
        _MDrv_PQ_DynamicContrastTable(pInstance, eWindow, u16PQ_DynContrIdx);
    }
    else
    {
        MS_ASSERT(0);
    }
#else
    UNUSED(eWindow);
    UNUSED(enDynamicContrastType);
#endif
}
void MDrv_PQ_LoadDynamicContrastTable(PQ_WIN eWindow, PQ_DYNAMIC_CONTRAST_FUNCTION_TYPE enDynamicContrastType)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_LoadDynamicContrastTable PQArgs;
    PQArgs.eWindow =eWindow;
    PQArgs.enDynamicContrastType = enDynamicContrastType;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_LOAD_DYNAMIC_CONTRAST_TABLE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}

#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE || (ENABLE_PQ_CUSTOMER_BIN_SUB_GRULE && PQ_ENABLE_PIP)
static void _MDrv_PQ_LoadCustomerNRTable(PQ_WIN eWindow, MS_U16 u16PQ_NRIdx)
{
    MS_U16 i, u16IPIdx = 0, u16TabIdx = 0;

    // follow setgrulestatus, only load big pq or small pq, not both
    if ((eWindow == PQ_MAIN_WINDOW) && _bCustomerMainPQEn)
    {
        if(u16PQ_NRIdx > PQ_GRULE_NR_NUM_Main_Color)
        {
            MS_ASSERT(FALSE);
            return;
        }

        for(i = 0; i < PQ_GRULE_NR_IP_NUM_Main_Color; i++)
        {
            u16IPIdx = MDrv_PQ_GetCustomerGRule_IPIndex(eWindow, PQ_GRule_NR_Main_Color, i);

            if(u16PQ_NRIdx == PQ_GRULE_NR_NUM_Main_Color)  // see it as default
            {
                u16TabIdx = MDrv_PQ_GetCustomerTableIndex(eWindow, u16IPIdx);
            }
            else
            {
                u16TabIdx = MDrv_PQ_GetGRule_CustomerTableIndex(eWindow, PQ_GRule_NR_Main_Color, u16PQ_NRIdx, i);
            }
            MDrv_PQ_LoadCustomerTable(eWindow, u16TabIdx, u16IPIdx);
        }
    }
#if PQ_ENABLE_PIP
    else if ((eWindow == PQ_SUB_WINDOW) && _bCustomerSubPQEn)
    {
        if(u16PQ_NRIdx > PQ_GRULE_NR_NUM_Sub_Color)
        {
            MS_ASSERT(FALSE);
            return;
        }

        for(i = 0; i < PQ_GRULE_NR_IP_NUM_Sub_Color; i++)
        {
            u16IPIdx = MDrv_PQ_GetCustomerGRule_IPIndex(eWindow, PQ_GRule_NR_Sub_Color, i);

            if(u16PQ_NRIdx == PQ_GRULE_NR_NUM_Sub_Color)  // see it as default
            {
                u16TabIdx = MDrv_PQ_GetCustomerTableIndex(eWindow, u16IPIdx);
            }
            else
            {
                u16TabIdx = MDrv_PQ_GetGRule_CustomerTableIndex(eWindow, PQ_GRule_NR_Sub_Color, u16PQ_NRIdx, i);
            }
            MDrv_PQ_LoadCustomerTable(eWindow, u16TabIdx, u16IPIdx);
        }
    }
#endif
    else
    {
        PQGRULE_DBG(printf("[NR] CUSTOMER PQ Not support case \n"));
    }

}
#endif

static void _MDrv_PQ_LoadNRTable(void* pInstance, PQ_WIN eWindow, MS_U16 u16PQ_NRIdx)
{
    MS_U16 i, u16IPIdx = 0, u16TabIdx = 0;

    // follow setgrulestatus, only load big pq or small pq, not both
    if (eWindow == PQ_MAIN_WINDOW)
    {
        {
            if(u16PQ_NRIdx > PQ_GRULE_NR_NUM_Main)
            {
                MS_ASSERT(FALSE);
                return;
            }

            for(i = 0; i < PQ_GRULE_NR_IP_NUM_Main; i++)
            {
                u16IPIdx = MDrv_PQ_GetGRule_IPIndex_U2(pInstance, eWindow, PQ_GRule_NR_Main, i);

                if(u16PQ_NRIdx == PQ_GRULE_NR_NUM_Main)  // see it as default
                {
                    u16TabIdx = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, u16IPIdx);
                }
                else
                {
                    u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_NR_Main, u16PQ_NRIdx, i);
                }
                PQGRULE_DBG(printf("[NR]SRC: %u, NR: %u, NRIPIdx:%u, IPIdx:%u, u16TabIdx:%u\n",
                               _u16PQSrcType[eWindow], u16PQ_NRIdx, i, u16IPIdx, u16TabIdx));
                MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx, u16IPIdx);
            }
        }
    }
#if PQ_ENABLE_PIP
    else if (eWindow == PQ_SUB_WINDOW)
    {
        {
            if(u16PQ_NRIdx > PQ_GRULE_NR_NUM_Sub)
            {
                MS_ASSERT(FALSE);
                return;
            }

            for(i = 0; i < PQ_GRULE_NR_IP_NUM_Sub; i++)
            {
                u16IPIdx = MDrv_PQ_GetGRule_IPIndex_U2(pInstance, eWindow, PQ_GRule_NR_Sub, i);

                if(u16PQ_NRIdx == PQ_GRULE_NR_NUM_Sub)  // see it as default
                {
                    u16TabIdx = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, u16IPIdx);
                }
                else
                {
                    u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_NR_Sub, u16PQ_NRIdx, i);
                }
                PQGRULE_DBG(printf("[NR]SRC: %u, NR: %u, NRIPIdx:%u, IPIdx:%u, u16TabIdx:%u\n",
                               _u16PQSrcType[eWindow], u16PQ_NRIdx, i, u16IPIdx, u16TabIdx));
                MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx, u16IPIdx);
            }
        }

    }
#endif
    else
    {
        PQGRULE_DBG(printf("[NR] Not support case \n"));
    }

}

void MDrv_PQ_LoadNRTable_U2(void* pInstance,PQ_WIN eWindow, PQ_3D_NR_FUNCTION_TYPE en3DNRType)
{
    PQGRULE_DBG(printf("[PQ_LoadNRTable] "));
    MS_U16 u16PQ_NRIdx = PQ_GRule_NR_Off_Main;
    MS_U16 u16GRuleType = PQ_GRule_NR_Main;
    MS_U16 u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Off_Main;
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
    MS_U16 u16CustomerPQ_NRIdx = PQ_GRule_NR_Off_Main;
    MS_U16 u16CustomerGRuleType = PQ_GRule_NR_Main_Color;
    MS_U16 u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Off_Main_Color;
#endif

    if (eWindow == PQ_MAIN_WINDOW)
    {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
        u16CustomerGRuleType = PQ_GRule_NR_Main_Color + MDrv_PQ_GetGRule_GRuleNum_U2(pInstance, eWindow);
#endif
        u16GRuleType = PQ_GRule_NR_Main;

        if(en3DNRType == PQ_3D_NR_DEFAULT)
        {
            PQGRULE_DBG(printf("Default\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
            if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
            {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                u16CustomerPQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, u16CustomerGRuleType, PQ_GRule_Lvl_NR_Low_3D_Main_Color);
#endif
                u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, u16GRuleType, PQ_GRule_Lvl_NR_Low_3D_Main);
            }
            else
#endif
            {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                u16CustomerPQ_NRIdx = PQ_GRULE_NR_NUM_Main_Color;
#endif
                u16PQ_NRIdx = PQ_GRULE_NR_NUM_Main;
            }
        }
        else
        {
            if(en3DNRType == PQ_3D_NR_OFF)
            {
                PQGRULE_DBG(printf("Off\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
                if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Off_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Off_Main;
                }
                else
#endif
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Off_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Off_Main;
                }
            }
            else if(en3DNRType == PQ_3D_NR_AUTO)
            {
                // Auto NR will be process in msAPI_DynamicNR_Handler(), hence we only need to pre-setup the PQ table.
                PQGRULE_DBG(printf("Auto\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
                if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Low_3D_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Low_3D_Main;
                }
                else
#endif
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Low_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Low_Main;
                }
            }
            else if(en3DNRType == PQ_3D_NR_LOW)
            {
                PQGRULE_DBG(printf("Low\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
                if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Low_3D_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Low_3D_Main;
                }
                else
#endif
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Low_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Low_Main;
                }
            }
            else if(en3DNRType == PQ_3D_NR_MID)
            {
                PQGRULE_DBG(printf("Mid\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
                if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Middle_3D_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Middle_3D_Main;
                }
                else
#endif
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Middle_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Middle_Main;
                }
            }
            else if(en3DNRType == PQ_3D_NR_HIGH)
            {
                PQGRULE_DBG(printf("High\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
                if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_High_3D_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_High_3D_Main;
                }
                else
#endif
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_High_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_High_Main;
                }
            }
#if (PQ_ENABLE_MULTI_LEVEL_AUTO_NR == 1)
            else if(en3DNRType == PQ_3D_NR_AUTO_LOW_L)
            {
                PQGRULE_DBG(printf("AUTO_LOW_L\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
                if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Low_3D_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Low_3D_Main;
                }
                else
#endif
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Auto_Low_L_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Auto_Low_L_Main;
                }
            }
            else if(en3DNRType == PQ_3D_NR_AUTO_LOW_M)
            {
                PQGRULE_DBG(printf("AUTO_LOW_M\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
                if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Low_3D_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Low_3D_Main;
                }
                else
#endif
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Auto_Low_M_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Auto_Low_M_Main;
                }
            }
            else if(en3DNRType == PQ_3D_NR_AUTO_LOW_H)
            {
                PQGRULE_DBG(printf("AUTO_LOW_H\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
                if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Low_3D_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Low_3D_Main;
                }
                else
#endif
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Auto_Low_H_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Auto_Low_H_Main;
                }
            }
            else if(en3DNRType == PQ_3D_NR_AUTO_MID_L)
            {
                PQGRULE_DBG(printf("AUTO_MID_L\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
                if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Middle_3D_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Middle_3D_Main;
                }
                else
#endif
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Auto_Middle_L_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Auto_Middle_L_Main;
                }
            }
            else if(en3DNRType == PQ_3D_NR_AUTO_MID_M)
            {
                PQGRULE_DBG(printf("AUTO_MID_M\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
                if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Middle_3D_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Middle_3D_Main;
                }
                else
#endif
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Auto_Middle_M_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Auto_Middle_M_Main;
                }
            }
            else if(en3DNRType == PQ_3D_NR_AUTO_MID_H)
            {
                PQGRULE_DBG(printf("AUTO_MID_H\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
                if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Middle_3D_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Middle_3D_Main;
                }
                else
#endif
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Auto_Middle_H_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Auto_Middle_H_Main;
                }
            }
            else if(en3DNRType == PQ_3D_NR_AUTO_HIGH_L)
            {
                PQGRULE_DBG(printf("AUTO_HIGH_L\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
                if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_High_3D_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_High_3D_Main;
                }
                else
#endif
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Auto_High_L_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Auto_High_L_Main;
                }
            }
            else if(en3DNRType == PQ_3D_NR_AUTO_HIGH_M)
            {
                PQGRULE_DBG(printf("AUTO_HIGH_M\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
                if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_High_3D_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_High_3D_Main;
                }
                else
#endif
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Auto_High_M_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Auto_High_M_Main;
                }
            }
            else if(en3DNRType == PQ_3D_NR_AUTO_HIGH_H)
            {
                PQGRULE_DBG(printf("AUTO_HIGH_H\n"));
#if (PQ_ENABLE_3D_STRENGTHEN_NR == 1)
                if(MApi_XC_Get_3D_Output_Mode() != E_XC_3D_OUTPUT_MODE_NONE)
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_High_3D_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_High_3D_Main;
                }
                else
#endif
                {
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
                    u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Auto_High_H_Main_Color;
#endif
                    u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Auto_High_H_Main;
                }
            }
#endif
            else
            {
                MS_ASSERT(FALSE);
            }
            // Third parameter of MDrv_PQ_GetGRule_LevelIndex should be level index, not group index.
            u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, u16GRuleType, u16GRuleLevelIndex);
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
            u16CustomerPQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, u16CustomerGRuleType, u16CustomerGRuleLevelIndex);
#endif
        }
        _MDrv_PQ_LoadNRTable(pInstance, eWindow, u16PQ_NRIdx);
#if ENABLE_PQ_CUSTOMER_BIN_MAIN_GRULE
        _MDrv_PQ_LoadCustomerNRTable(eWindow, u16CustomerPQ_NRIdx);
#endif
    }
#if PQ_ENABLE_PIP
    else if (eWindow == PQ_SUB_WINDOW)
    {
#if ENABLE_PQ_CUSTOMER_BIN_SUB_GRULE
        u16CustomerGRuleType = PQ_GRule_NR_Sub_Color + MDrv_PQ_GetGRule_GRuleNum_U2(pInstance, eWindow);
#endif
        u16GRuleType = PQ_GRule_NR_Sub;

        if(en3DNRType == PQ_3D_NR_DEFAULT)
        {
            PQGRULE_DBG(printf("Default\n"));
#if ENABLE_PQ_CUSTOMER_BIN_SUB_GRULE
            u16CustomerPQ_NRIdx = PQ_GRULE_NR_NUM_Sub_Color;
#endif
            u16PQ_NRIdx = PQ_GRULE_NR_NUM_Sub;
        }
        else
        {
            if(en3DNRType == PQ_3D_NR_OFF)
            {
                PQGRULE_DBG(printf("Off\n"));
#if ENABLE_PQ_CUSTOMER_BIN_SUB_GRULE
                u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Off_Sub_Color;
#endif
                u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Off_Sub;
            }
            else if(en3DNRType == PQ_3D_NR_AUTO)
            {
                // Auto NR will be process in msAPI_DynamicNR_Handler(), hence we only need to pre-setup the PQ table.
                PQGRULE_DBG(printf("Auto\n"));
#if ENABLE_PQ_CUSTOMER_BIN_SUB_GRULE
                u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Low_Sub_Color;
#endif
                u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Low_Sub;
            }
            else if(en3DNRType == PQ_3D_NR_LOW)
            {
                PQGRULE_DBG(printf("Low\n"));
#if ENABLE_PQ_CUSTOMER_BIN_SUB_GRULE
                u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Low_Sub_Color;
#endif
                u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Low_Sub;
            }
            else if(en3DNRType == PQ_3D_NR_MID)
            {
                PQGRULE_DBG(printf("Mid\n"));
#if ENABLE_PQ_CUSTOMER_BIN_SUB_GRULE
                u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_Middle_Sub_Color;
#endif
                u16GRuleLevelIndex = PQ_GRule_Lvl_NR_Middle_Sub;
            }
            else if(en3DNRType == PQ_3D_NR_HIGH)
            {
                PQGRULE_DBG(printf("High\n"));
#if ENABLE_PQ_CUSTOMER_BIN_SUB_GRULE
                u16CustomerGRuleLevelIndex = PQ_GRule_Lvl_NR_High_Sub_Color;
#endif
                u16GRuleLevelIndex = PQ_GRule_Lvl_NR_High_Sub;
            }
            else
            {
                MS_ASSERT(FALSE);
            }
            u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, u16GRuleType, u16GRuleLevelIndex);
#if ENABLE_PQ_CUSTOMER_BIN_SUB_GRULE
            u16CustomerPQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, u16CustomerGRuleType, u16CustomerGRuleLevelIndex);
#endif
        }
        _MDrv_PQ_LoadNRTable(pInstance, eWindow, u16PQ_NRIdx);
#if ENABLE_PQ_CUSTOMER_BIN_SUB_GRULE
        _MDrv_PQ_LoadCustomerNRTable(eWindow, u16CustomerPQ_NRIdx);
#endif
    }
#endif
    else
    {
        PQGRULE_DBG(printf("Not support case \n"));
    }
}
void MDrv_PQ_LoadNRTable(PQ_WIN eWindow, PQ_3D_NR_FUNCTION_TYPE en3DNRType)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_LoadNRTable PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.en3DNRType = en3DNRType;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_LOAD_NR_TABLE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}

#if PQ_GRULE_MPEG_NR_ENABLE
static void _MDrv_PQ_LoadMPEGNRTable(void* pInstance, PQ_WIN eWindow, MS_U16 u16PQ_NRIdx)
{
    MS_U16 i, u16IPIdx = 0, u16TabIdx = 0;
    if(u16PQ_NRIdx > PQ_GRULE_MPEG_NR_NUM_Main)
    {
        MS_ASSERT(0);
        return;
    }

    for(i = 0; i < PQ_GRULE_MPEG_NR_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex_U2(pInstance, eWindow, PQ_GRule_MPEG_NR_Main, i);

        if(u16PQ_NRIdx == PQ_GRULE_MPEG_NR_NUM_Main)  // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, u16IPIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_MPEG_NR_Main, u16PQ_NRIdx, i);

        PQGRULE_DBG(printf("[MPEGNR]SRC: %u, NR: %u, NRIPIdx:%u, IPIdx:%u, u16TabIdx:%u\n",
                           _u16PQSrcType[eWindow], u16PQ_NRIdx, i, u16IPIdx, u16TabIdx));
        MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx, u16IPIdx);
    }
}
#endif

void MDrv_PQ_LoadMPEGNRTable_U2(void* pInstance,PQ_WIN eWindow, PQ_MPEG_NR_FUNCTION_TYPE enMPEGNRType)
{
#if(PQ_GRULE_MPEG_NR_ENABLE)
    PQGRULE_DBG(printf("[PQ_LoadMPEGNRTable] "));
    MS_U16 u16PQ_NRIdx;
    if(eWindow == PQ_MAIN_WINDOW)
    {
        if(enMPEGNRType == PQ_MPEG_NR_OFF)
        {
            PQGRULE_DBG(printf("Off\n"));
            u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_MPEG_NR_Main, PQ_GRule_MPEG_NR_Off_Main);
            _MDrv_PQ_LoadMPEGNRTable(pInstance, eWindow, u16PQ_NRIdx);
        }
        else if( enMPEGNRType == PQ_MPEG_NR_AUTO )
        {
            // Auto NR will be process in msAPI_DynamicNR_Handler(), hence we only need to pre-setup the PQ table.
            PQGRULE_DBG(printf("Auto\n"));
            u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_MPEG_NR_Main, PQ_GRule_MPEG_NR_Low_Main);
            _MDrv_PQ_LoadMPEGNRTable(pInstance, eWindow, u16PQ_NRIdx);
        }
        else if(enMPEGNRType == PQ_MPEG_NR_LOW)
        {
            PQGRULE_DBG(printf("Low\n"));
            u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_MPEG_NR_Main, PQ_GRule_MPEG_NR_Low_Main);
            _MDrv_PQ_LoadMPEGNRTable(pInstance, eWindow, u16PQ_NRIdx);
        }
        else if(enMPEGNRType == PQ_MPEG_NR_MID)
        {
            PQGRULE_DBG(printf("Mid\n"));
            u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_MPEG_NR_Main, PQ_GRule_MPEG_NR_Middle_Main);
            _MDrv_PQ_LoadMPEGNRTable(pInstance, eWindow, u16PQ_NRIdx);
        }
        else if(enMPEGNRType == PQ_MPEG_NR_HIGH)
        {
            PQGRULE_DBG(printf("High\n"));
            u16PQ_NRIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_MPEG_NR_Main, PQ_GRule_MPEG_NR_High_Main);
            _MDrv_PQ_LoadMPEGNRTable(pInstance, eWindow, u16PQ_NRIdx);
        }
        else if(enMPEGNRType == PQ_MPEG_NR_DEFAULT)
        {
            PQGRULE_DBG(printf("Default\n"));
            _MDrv_PQ_LoadMPEGNRTable(pInstance, eWindow, PQ_GRULE_MPEG_NR_NUM_Main);
        }
        else
        {
            MS_ASSERT(0);
        }
    }
#else
    UNUSED(eWindow);
    UNUSED(enMPEGNRType);
#endif
}
void MDrv_PQ_LoadMPEGNRTable(PQ_WIN eWindow, PQ_MPEG_NR_FUNCTION_TYPE enMPEGNRType)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_LoadMPEGNRTable PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.enMPEGNRType = enMPEGNRType;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_LOAD_MPEG_NR_TABLE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}

#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
void _MDrv_PQ_Load_ULTRACLEAR_Table(void* pInstance, PQ_WIN eWindow, MS_U16 u16PQ_ULTRACLEAR_Idx)
{
    MS_U16 i = 0, u16IPIdx = 0, u16TabIdx = 0;

    if(u16PQ_ULTRACLEAR_Idx > PQ_GRULE_ULTRAT_CLEAR_NUM_Main)
    {
        MS_ASSERT(0);
        return;
    }

    for(i = 0; i < PQ_GRULE_ULTRAT_CLEAR_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex_U2(pInstance, eWindow, PQ_GRule_ULTRAT_CLEAR_Main, i);
#if PQ_LCE_CTL_FOR_MVC4KX1K
        if((_gIsMVC4kx1k == TRUE) && (u16IPIdx == PQ_IP_VIP_LCE_Main))
        {
            printf("[Grule-ULTRACLEAR] cannot load LCE table for MVC4kx1k\n");
            continue;
        }
#endif
        if(u16PQ_ULTRACLEAR_Idx == PQ_GRULE_ULTRAT_CLEAR_NUM_Main)  // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, u16IPIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_ULTRAT_CLEAR_Main, u16PQ_ULTRACLEAR_Idx, i);

        PQGRULE_DBG(printf("[UC]SRC: %u, UC: %u, UCIPIdx:%u, IPIdx:%u, u8TabIdx:%u\n",
                           _u16PQSrcType[eWindow], u16PQ_ULTRACLEAR_Idx, i, u16IPIdx, u16IPIdx));

        MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx, u16IPIdx);
    }
}
#endif

void MDrv_PQ_Load_ULTRACLEAR_Table_U2(void* pInstance,PQ_WIN eWindow, PQ_FEATURE_SWITCH_TYPE enCtrlType)
{
    MS_U16 u16PQ_TableIdx = 0;
#if PQ_GRULE_ULTRAT_CLEAR_ENABLE
    PQGRULE_DBG(printf("[PQ_Load_ULTRACLEAR_Table] "));
    if(enCtrlType == PQ_FEATURE_SWITCH_ON)
    {
        PQGRULE_DBG(printf("On\n"));
        u16PQ_TableIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_ULTRAT_CLEAR_Main, PQ_GRule_ULTRAT_CLEAR_On_Main);
        _MDrv_PQ_Load_ULTRACLEAR_Table(pInstance, eWindow, u16PQ_TableIdx);
    }
    else
    {
        PQGRULE_DBG(printf("Off\n"));
        u16PQ_TableIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_ULTRAT_CLEAR_Main, PQ_GRule_ULTRAT_CLEAR_Off_Main);
        _MDrv_PQ_Load_ULTRACLEAR_Table(pInstance, eWindow, u16PQ_TableIdx);
    }
#else
    UNUSED(u16PQ_TableIdx);
    UNUSED(eWindow);
    UNUSED(enCtrlType);

#endif
}
void MDrv_PQ_Load_ULTRACLEAR_Table(PQ_WIN eWindow, PQ_FEATURE_SWITCH_TYPE enCtrlType)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_Load_ULTRACLEAR_Table PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.enCtrlType = enCtrlType;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_LOAD_ULTRACLEAR_TABLE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}

void MDrv_PQ_Load_HDR_Table_U2(void *pInstance,PQ_WIN eWindow, PQ_HDR_MODE enHdrMode)
{
#if PQ_GRULE_HDR_ENABLE
    if (enHdrMode == PQ_HDR_ON)
    {
        MDrv_PQ_SetGRuleStatus_U2(pInstance,eWindow, PQ_GRule_HDR_Main, PQ_GRule_HDR_On_Main);
    }
#else
    UNUSED(eWindow);
    UNUSED(enHdrMode);
#endif
}
void MDrv_PQ_Load_HDR_Table(PQ_WIN eWindow, PQ_HDR_MODE enHdrMode)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_Load_HDR_Table PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.enHdrMode = enHdrMode;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_LOAD_HDR_TABLE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}

void MDrv_PQ_Load_XVYCC_Table_U2(void* pInstance,PQ_WIN eWindow, PQ_XVYCC_TYPE enXvyccType)
{
#ifdef PQ_CFD_INFO
    if (eWindow == PQ_MAIN_WINDOW)
    {
        ST_PQ_CFD_LINEAR_RGB stCfdLinearRgb;
        memset(&stCfdLinearRgb, 0, sizeof(ST_PQ_CFD_LINEAR_RGB));
        stCfdLinearRgb.u32Version = CFD_LINEAR_RGB_VERSION;
        stCfdLinearRgb.u16Length = sizeof(ST_PQ_CFD_LINEAR_RGB);
        stCfdLinearRgb.u8Win = eWindow;

        if(enXvyccType == PQ_XVYCC_NORMAL)
        {
            stCfdLinearRgb.bEnable = TRUE;
        }
        else if(enXvyccType == PQ_XVYCC_ON_XVYCC)
        {
            stCfdLinearRgb.bEnable = TRUE;
        }
        else if(enXvyccType == PQ_XVYCC_ON_SRGB)
        {
            stCfdLinearRgb.bEnable = FALSE;
        }

        MDrv_PQ_SendCfd(E_PQ_CFD_CTRL_SET_LINEAR_RGB, &stCfdLinearRgb, sizeof(ST_PQ_CFD_LINEAR_RGB));
    }
#else
#if PQ_GRULE_XVYCC_ENABLE
    if (!QM_IsSourceMultiMedia(MDrv_PQ_GetInputSourceType(eWindow)))
    {
        PQGRULE_DBG(printf("[PQ_Load_XVYCC_Table] "));
        if(enXvyccType == PQ_XVYCC_NORMAL)
        {
            PQGRULE_DBG(printf("Normal\n"));
            MDrv_PQ_SetGRuleStatus(eWindow, PQ_GRule_XVYCC_Main, PQ_GRule_XVYCC_Normal_Main);//PQ_GRule_XVYCC_Normal_Main
        }
        else if(enXvyccType == PQ_XVYCC_ON_XVYCC)
        {
            PQGRULE_DBG(printf("On xcYCC\n"));
            MDrv_PQ_SetGRuleStatus(eWindow, PQ_GRule_XVYCC_Main, PQ_GRule_XVYCC_On_xvYCC_Main);//PQ_GRule_XVYCC_On_xvYCC_Main
        }
        else if(enXvyccType == PQ_XVYCC_ON_SRGB)
        {
            PQGRULE_DBG(printf("On sRGB\n"));
            MDrv_PQ_SetGRuleStatus(eWindow, PQ_GRule_XVYCC_Main, PQ_GRule_XVYCC_On_sRGB_Main);//PQ_GRule_XVYCC_On_sRGB_Main
        }
        else
        {
            MS_ASSERT(0);
        }
    }
#else
    UNUSED(eWindow);
    UNUSED(enXvyccType);
#endif
#endif
}
void MDrv_PQ_Load_XVYCC_Table(PQ_WIN eWindow, PQ_XVYCC_TYPE enXvyccType)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_Load_XVYCC_Table PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.enXvyccType = enXvyccType;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_LOAD_XVYCC_TABLE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}

MS_BOOL MDrv_PQ_GRULE_Get_Support_Status_U2(void* pInstance,PQ_SUPPORTED_TYPE enType)
{
    MS_BOOL bStatus = FALSE;
    switch(enType)
    {
        case E_PQ_SUPPORTED_FILM:
            if(PQ_GRULE_FILM_MODE_ENABLE)
                bStatus = TRUE;
            break;
        case E_PQ_SUPPORTED_NR:
            if(PQ_GRULE_NR_ENABLE)
                bStatus = TRUE;
            break;
        case E_PQ_SUPPORTED_MPEG_NR:
            if(PQ_GRULE_MPEG_NR_ENABLE)
                bStatus = TRUE;
            break;
        case E_PQ_SUPPORTED_BLACK_LEVEL:
            bStatus = TRUE;
            break;
        case E_PQ_SUPPORTED_ULTRA_CLEAR:
            if(PQ_GRULE_ULTRAT_CLEAR_ENABLE)
                bStatus = TRUE;
            break;
        case E_PQ_SUPPORTED_HDR:
            if(PQ_GRULE_HDR_ENABLE )
                bStatus = TRUE;
            break;
        default:
            break;
    }

    return bStatus;

}
MS_BOOL MDrv_PQ_GRULE_Get_Support_Status(PQ_SUPPORTED_TYPE enType)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GRULE_Get_Support_Status PQArgs;
    PQArgs.enType = enType;
    PQArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GRULE_GET_SUPPORT_STATUS, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

//////////////////////////////////////////////////////////////////
// Load MADi/444To422/422To444 table
//
// parameter:
//    [IN]  u8PQTabType
//          bMemFmt422: TRUE  - 422
//                      FALSE - 444
//          bFBL:       TRUE  - framebuffer-less mode
//                      FALSE - framebuffer mode
//    [OUT] u8BitsPerPixel
//                      bits per pixel for decided memory format
//
// return:  deinterlace (MADi) mode
//
PQ_DEINTERLACE_MODE MDrv_PQ_SetMemFormat_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bMemFmt422, MS_BOOL bFBL, MS_U8 *pu8BitsPerPixel)
{
    MS_U16 u16TabIdx_MemFormat = 0;
    MS_U16 u16TabIdx_MADi = 0;
    MS_U16 u16TabIdx_MADi_Motion=0xFFFF;
    MS_U16 u16TabIdx_444To422 = 0;
    MS_U16 u16TabIdx_422To444 = 0;
    PQ_DEINTERLACE_MODE eDeInterlaceMode = PQ_DEINT_OFF;
    MS_U8 u8FrameCount = 4;

    if(eWindow == PQ_MAIN_WINDOW)
    {
        MApi_XC_Set_OPWriteOffEnable(ENABLE, MAIN_WINDOW); //default
    }
    else
    {
        PQINFO_DBG(printf("No need to enable OP write with sub window, it would change main window setting \n"));
    }

    PQBW_DBG(printf("==>In PQ : eWindow =%d\n", eWindow));

    if(!bFBL)
    {
        // if MADi mode change from 25 <=> 27, cropping base address need to be recalculated.
#if PQ_GRULE_OSD_BW_ENABLE
        if(((_stMode_Info[eWindow].u16input_hsize >= 1280) && (_stMode_Info[eWindow].u16input_vsize >= 720)) &&
                (_bOSD_On))
        {
            MDrv_PQ_GetMADiFromOSDBWGrule(eWindow, &u16TabIdx_MADi, &u16TabIdx_MADi_Motion);
        }
        else
#endif
        {
            MDrv_PQ_GetMADiInGeneral_U2(pInstance, eWindow, &u16TabIdx_MADi, &u16TabIdx_MADi_Motion);
#if PQ_GRULE_OSD_BW_ENABLE
            MDrv_PQ_Patch2Rto4RForFieldPackingMode_U2(pInstance, eWindow, u16TabIdx_MADi);
#endif
        }

        MDrv_PQ_GetMADiForRFBL_U2(pInstance, eWindow, bFBL, &u16TabIdx_MADi_Motion, &u16TabIdx_MADi);
    }
    else // FBL
    {
        MS_U8 u8FblMode = Hal_PQ_get_madi_fbl_mode(pInstance,bMemFmt422, _stMode_Info[eWindow].bInterlace);
        u16TabIdx_MADi = Hal_PQ_get_madi_idx(pInstance,(eWindow == PQ_MAIN_WINDOW) ? TRUE : FALSE, (MS_MADI_TYPE)u8FblMode);
    }

    PQMADi_DBG(printf("Memory is %s mode \n", (bFBL)?("FBL"):("FB")));
    PQMADi_DBG(printf("get MADi idx = %u\n", u16TabIdx_MADi));

    MDrv_PQ_GetMemFmtInGeneral_U2(pInstance, eWindow, bMemFmt422, &u16TabIdx_MemFormat, &u16TabIdx_444To422, &u16TabIdx_422To444);

    PQTAB_DBG(printf("%s :u16TabIdx_444To422=%u\n", eWindow?"SubWin":"MainWin",u16TabIdx_444To422));
    PQTAB_DBG(printf("%s :u16TabIdx_422To444=%u\n", eWindow?"SubWin":"MainWin",u16TabIdx_422To444));
    PQMADi_DBG(printf("MemFmt is 422 : %s\n",(bMemFmt422)?("Yes"):("No")));
    PQMADi_DBG(printf("u16TabIdx_MemFormat is %u\n",u16TabIdx_MemFormat));

    MDrv_PQ_GetBPPInfoFromMADi_U2(
                               pInstance,
                               eWindow,
                               bMemFmt422,
                               u16TabIdx_MADi,
                               u16TabIdx_MemFormat,
                               pu8BitsPerPixel,
                               &eDeInterlaceMode,
                               &u8FrameCount);

    PQMADi_DBG(printf("BitPerPixel =%d, DIMode =%x\n", *pu8BitsPerPixel, eDeInterlaceMode));

    _gbMemfmt422[eWindow] = bMemFmt422;
    _gu16SetMemfmt_Madi[eWindow] = u16TabIdx_MADi;
    _gu16SetMemfmt_Madi_Motion[eWindow] = u16TabIdx_MADi_Motion;

    MDrv_PQ_ChangeMemConfigFor3D_U2(pInstance, eWindow, bMemFmt422, &u16TabIdx_MemFormat, &u16TabIdx_444To422, &u16TabIdx_422To444,
                                    pu8BitsPerPixel, &u16TabIdx_MADi_Motion, &u16TabIdx_MADi);

    if(eWindow == PQ_MAIN_WINDOW)
    {
#if(ENABLE_PQ_MLOAD)
        MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
#endif
        MDrv_PQ_LoadTable_U2(pInstance, PQ_MAIN_WINDOW, u16TabIdx_MemFormat, PQ_IP_MemFormat_Main);

        MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_MADi, PQ_IP_MADi_Main);
#if PQ_GRULE_OSD_BW_ENABLE
        if(0xFFFF != u16TabIdx_MADi_Motion)
        {
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_MADi_Motion, PQ_IP_MADi_Motion_Main);
        }
#endif
#if(ENABLE_PQ_MLOAD)
        MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
#endif

        MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_444To422, PQ_IP_444To422_Main);
        MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_422To444, PQ_IP_422To444_Main);
    }
#if PQ_ENABLE_PIP
    else
    {
#if(ENABLE_PQ_MLOAD)
        MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
#endif
        MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_MemFormat, PQ_IP_MemFormat_Sub);

        MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_MADi, PQ_IP_MADi_Sub);
#if PQ_GRULE_OSD_BW_ENABLE
        if(0xFFFF != u16TabIdx_MADi_Motion)
        {
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_MADi_Motion, PQ_IP_MADi_Motion_Sub);
        }
#endif
#if(ENABLE_PQ_MLOAD)
        MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
#endif
        MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_444To422, PQ_IP_444To422_Sub);
        MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx_422To444, PQ_IP_422To444_Sub);

        //R W bank mapping mode patch for Qmap hasn't this item for sub
        if(_stMode_Info[eWindow].bInterlace)
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK12_47_L, 0x4000 , 0xE000 );
        }
        else
        {
            MApi_XC_W2BYTEMSK(REG_SC_BK12_47_L, 0x2000 , 0xE000 );
        }
    }
#endif

    PQTAB_DBG(printf("u16TabIdx_MemFormat=%u\n", u16TabIdx_MemFormat));
    PQTAB_DBG(printf("%s, u16TabIdx_MADi=%u\n", eWindow?"SUB_WIN":"MAIN_WIN", u16TabIdx_MADi));
    PQTAB_DBG(printf("%s, u16TabIdx_MADi_Motion=%u\n", eWindow?"SUB_WIN":"MAIN_WIN", u16TabIdx_MADi_Motion));
    PQTAB_DBG(printf("%s :u16TabIdx_444To422=%u\n", eWindow?"SUB_WIN":"MAIN_WIN", u16TabIdx_444To422));
    PQTAB_DBG(printf("%s :u16TabIdx_422To444=%u\n", eWindow?"SUB_WIN":"MAIN_WIN", u16TabIdx_422To444));

#if (PQ_XRULE_DB_ENABLE == 1)
    MDrv_PQ_Set_DBRule(eWindow, u16TabIdx_MADi);
#endif

    MDrv_PQ_ForceBPPForDynamicMemFmt_U2(pInstance, eWindow, bMemFmt422, pu8BitsPerPixel);
    MDrv_PQ_SuggestFrameNum_U2(pInstance, eWindow, u16TabIdx_MADi, u8FrameCount);

    if(_bDS_En)
    {
        _u16RW_Method = Hal_PQ_get_rw_method(pInstance,eWindow == PQ_MAIN_WINDOW ? TRUE : FALSE);
        Hal_PQ_set_rw_method(pInstance,eWindow == PQ_MAIN_WINDOW ? TRUE : FALSE, 0x4000);
    }

    _bRWNumIsReset[eWindow] = TRUE;

    return eDeInterlaceMode;
}
PQ_DEINTERLACE_MODE MDrv_PQ_SetMemFormat(PQ_WIN eWindow, MS_BOOL bMemFmt422, MS_BOOL bFBL, MS_U8 *pu8BitsPerPixel)
{
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return PQ_DEINT_OFF;
        }
    }
    stPQ_SetMemFormat PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bMemFmt422 = bMemFmt422;
    PQArgs.bFBL = bFBL;
    PQArgs.pu8BitsPerPixel = pu8BitsPerPixel;
    PQArgs.bReturnValue = PQ_DEINT_OFF;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_MEMFORMAT, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return PQ_DEINT_OFF;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

void MDrv_PQ_ReduceBW_ForOSD_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bOSD_On)
{
#if PQ_GRULE_OSD_BW_ENABLE
    MS_U8 u8BitPerPixel = 0;
{
    if((_stMode_Info[eWindow].u16input_hsize >= 1280) && (_stMode_Info[eWindow].u16input_vsize >= 720))
    {
        PQBW_DBG(printf("[PQ_ReduceBW_ForOSD]:%u\n", bOSD_On));

        _bOSD_On = bOSD_On;
        bSetFrameCount = FALSE;

        MDrv_PQ_SetMemFormat(
            eWindow,
            _gbMemfmt422[eWindow],
            _stMode_Info[eWindow].bFBL,
            &u8BitPerPixel);
        bSetFrameCount = TRUE;

    }
}
#else
    UNUSED(eWindow);
    UNUSED(bOSD_On);
#endif

    return;
}
void MDrv_PQ_ReduceBW_ForOSD(PQ_WIN eWindow, MS_BOOL bOSD_On)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_ReduceBW_ForOSD PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bOSD_On = bOSD_On;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_REDUCEBW_ForOSD, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}

#if PQ_GRULE_PTP_ENABLE
static void _MDrv_PQ_LoadPTPTable(void* pInstance, PQ_WIN eWindow, MS_U16 u16PQ_PTP_Idx)
{
    MS_U16 i, u16IPIdx=0, u16TabIdx=0;

    if(eWindow == PQ_SUB_WINDOW)
    {
        return;
    }

    if (u16PQ_PTP_Idx > PQ_GRULE_PTP_LVL_NUM_Main)
    {
        MS_ASSERT(0);
        return;
    }

    for(i=0; i<PQ_GRULE_PTP_IP_NUM_Main; i++)
    {
        u16IPIdx = MDrv_PQ_GetGRule_IPIndex_U2(pInstance, eWindow, PQ_GRule_PTP_Main, i);

        if (u16PQ_PTP_Idx == PQ_GRULE_PTP_NUM_Main) // see it as default
            u16TabIdx = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, u16IPIdx);
        else
            u16TabIdx = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_PTP_Main, u16PQ_PTP_Idx, i);

        PQP2P_DBG(printf("[PTP]SRC: %u, u16PQ_PTP_Idx: %u, PIPIPIdx:%u, IPIdx:%u, u8TabIdx:%u\n",
            _u16PQSrcType[eWindow], u16PQ_PTP_Idx, i, u16IPIdx, u16TabIdx);)

        MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx, u16IPIdx);
    }
}
#endif

void MDrv_PQ_LoadPTPTable_U2(void* pInstance,PQ_WIN eWindow, PQ_PTP_FUNCTION_TYPE enPTP_Type)
{
#if PQ_GRULE_PTP_ENABLE
    PQGRULE_DBG(printf("[PQ_LoadPTPTable] "));
    MS_U16 u16PQ_PTPIdx;

    if(eWindow == PQ_SUB_WINDOW)
    {
        return;
    }

     if( enPTP_Type == PQ_PTP_PTP )
     {
         PQP2P_DBG(printf("PTP\n");)
         u16PQ_PTPIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_PTP_Main, PQ_GRule_Lvl_PTP_On_Main);
         _MDrv_PQ_LoadPTPTable(pInstance, eWindow, u16PQ_PTPIdx);
#if (PQ_4K2K_P2P_H_OFFSET_LIMITIOM == 1)
         // 4k2k case, p2p should patch offset to 1, by designer
         if((QM_GetInputHSize(eWindow) == 3840) && (QM_GetInputVSize(eWindow) == 2160))
         {
            MApi_XC_W2BYTEMSK(REG_SC_BK20_1C_L, 0x01, 0xFF);
             //MDrv_PQ_LoadTable_U2(pInstance, eWindow, PQ_IP_VIP_Peaking_ON_Voff_Main, PQ_IP_VIP_Peaking_Main);
         }
#endif
     }
     else
     {
        MS_ASSERT(0);
     }
#else
    UNUSED(eWindow);
    UNUSED(enPTP_Type);
#endif
}
void MDrv_PQ_LoadPTPTable(PQ_WIN eWindow, PQ_PTP_FUNCTION_TYPE enPTP_Type)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_LoadPTPTable PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.enPTP_Type = enPTP_Type;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_LOAD_PTP_TABLE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}

typedef struct {
  MS_U8 *pQuality_Map_Aray;
#if (ENABLE_PQ_EX)
  MS_U8 *pQuality_Map_Aray_Ex;
#endif
  MS_U8 *pGRule_Level[E_GRULE_NUM];
} QMAP_TBL_INIT_INFO;

#define QMAP_TABLE_PANEL_NUMBER 1 // 1920, 1366 , 4K2K
#if PQ_ENABLE_PIP
#define QMAP_TABLE_DISPLAY_NUMBER PQ_DISPLAY_POP+1
#else
#define QMAP_TABLE_DISPLAY_NUMBER PQ_DISPLAY_ONE+1
#endif
QMAP_TBL_INIT_INFO QmapTblInitInfo_Main[QMAP_TABLE_PANEL_NUMBER][QMAP_TABLE_DISPLAY_NUMBER] = {
    { // 1920
        { // ONE
            (void*)QMAP_1920_Main,
#if (ENABLE_PQ_EX)
            (void*)QMAP_1920_Main_Ex,
#endif
            {
#if (PQ_GRULE_NR_ENABLE)
                (void*)MST_GRule_1920_NR_Main,
#endif
#if (PQ_GRULE_OSD_BW_ENABLE == 1)
                (void*)MST_GRule_1920_OSD_BW_Main,
#endif
#if (PQ_GRULE_MPEG_NR_ENABLE)
                (void*)MST_GRule_1920_MPEG_NR_Main,
#endif
#if (PQ_GRULE_FILM_MODE_ENABLE)
                (void*)MST_GRule_1920_FILM_MODE_Main,
#endif
#if (PQ_GRULE_DYNAMIC_CONTRAST_ENABLE)
                (void*)MST_GRule_1920_DYNAMIC_CONTRAST_Main,
#endif
#if (PQ_GRULE_DDR_SELECT_ENABLE == 1)
                (void*)MST_GRule_1920_DDR_SELECT_Main,
#endif
#if (PQ_GRULE_ULTRAT_CLEAR_ENABLE)
                (void*)MST_GRule_1920_ULTRAT_CLEAR_Main,
#endif
#if (PQ_GRULE_PTP_ENABLE == 1)
                (void*)MST_GRule_1920_PTP_Main,
#endif
#if (PQ_GRULE_XVYCC_ENABLE == 1 )
                (void*)MST_GRule_1920_XVYCC_Main,
#endif
#if (PQ_GRULE_GAME_MODE_ENABLE == 1)
                (void*)MST_GRule_1920_GAME_MODE_Main,
#endif
#if (PQ_GRULE_DS_PARAMETERS_ENABLE == 1)
                (void*)MST_GRule_1920_DS_PARAMETERS_Main,
#endif
#if (PQ_GRULE_HDR_ENABLE == 1)
                (void*)MST_GRule_1920_HDR_Main,
#endif
            }
        },
#if PQ_ENABLE_PIP
        { // PIP
            (void*)QMAP_1920_PIP_Main,
#if (ENABLE_PQ_EX)
            (void*)QMAP_1920_PIP_Main_Ex,
#endif
            {
#if (PQ_GRULE_NR_ENABLE)
                (void*)MST_GRule_1920_PIP_NR_Main,
#endif
#if (PQ_GRULE_OSD_BW_ENABLE == 1)
                (void*)MST_GRule_1920_PIP_OSD_BW_Main,
#endif
#if (PQ_GRULE_MPEG_NR_ENABLE)
                (void*)MST_GRule_1920_PIP_MPEG_NR_Main,
#endif
#if (PQ_GRULE_FILM_MODE_ENABLE)
                (void*)MST_GRule_1920_PIP_FILM_MODE_Main,
#endif
#if (PQ_GRULE_DYNAMIC_CONTRAST_ENABLE)
                (void*)MST_GRule_1920_PIP_DYNAMIC_CONTRAST_Main,
#endif
#if (PQ_GRULE_DDR_SELECT_ENABLE == 1)
                (void*)MST_GRule_1920_PIP_DDR_SELECT_Main,
#endif
#if (PQ_GRULE_ULTRAT_CLEAR_ENABLE)
                (void*)MST_GRule_1920_PIP_ULTRAT_CLEAR_Main,
#endif
#if (PQ_GRULE_PTP_ENABLE == 1)
                (void*)MST_GRule_1920_PIP_PTP_Main,
#endif
#if (PQ_GRULE_XVYCC_ENABLE == 1 )
                (void*)MST_GRule_1920_PIP_XVYCC_Main,
#endif
#if (PQ_GRULE_GAME_MODE_ENABLE == 1)
                (void*)MST_GRule_1920_PIP_GAME_MODE_Main,
#endif
#if (PQ_GRULE_DS_PARAMETERS_ENABLE == 1)
                (void*)MST_GRule_1920_PIP_DS_PARAMETERS_Main,
#endif
#if (PQ_GRULE_HDR_ENABLE == 1)
                (void*)MST_GRule_1920_PIP_HDR_Main,
#endif
            }
        },
        { // POP
            (void*)QMAP_1920_POP_Main,
#if (ENABLE_PQ_EX)
            (void*)QMAP_1920_POP_Main_Ex,
#endif
            {
#if (PQ_GRULE_NR_ENABLE)
                (void*)MST_GRule_1920_POP_NR_Main,
#endif
#if (PQ_GRULE_OSD_BW_ENABLE == 1)
                (void*)MST_GRule_1920_POP_OSD_BW_Main,
#endif
#if (PQ_GRULE_MPEG_NR_ENABLE)
                (void*)MST_GRule_1920_POP_MPEG_NR_Main,
#endif
#if (PQ_GRULE_FILM_MODE_ENABLE)
                (void*)MST_GRule_1920_POP_FILM_MODE_Main,
#endif
#if (PQ_GRULE_DYNAMIC_CONTRAST_ENABLE)
                (void*)MST_GRule_1920_POP_DYNAMIC_CONTRAST_Main,
#endif
#if (PQ_GRULE_DDR_SELECT_ENABLE == 1)
                (void*)MST_GRule_1920_POP_DDR_SELECT_Main,
#endif
#if (PQ_GRULE_ULTRAT_CLEAR_ENABLE)
                (void*)MST_GRule_1920_POP_ULTRAT_CLEAR_Main,
#endif
#if (PQ_GRULE_PTP_ENABLE == 1)
                (void*)MST_GRule_1920_POP_PTP_Main,
#endif
#if (PQ_GRULE_XVYCC_ENABLE == 1 )
                (void*)MST_GRule_1920_POP_XVYCC_Main,
#endif
#if (PQ_GRULE_GAME_MODE_ENABLE == 1)
                (void*)MST_GRule_1920_POP_GAME_MODE_Main,
#endif
#if (PQ_GRULE_DS_PARAMETERS_ENABLE == 1)
                (void*)MST_GRule_1920_POP_DS_PARAMETERS_Main,
#endif
#if (PQ_GRULE_HDR_ENABLE == 1)
                (void*)MST_GRule_1920_POP_HDR_Main,
#endif
            }
        },
#endif
    },
    
};

#if PQ_ENABLE_PIP
QMAP_TBL_INIT_INFO QmapTblInitInfo_Sub[QMAP_TABLE_PANEL_NUMBER][QMAP_TABLE_DISPLAY_NUMBER] = {
    { // 1920
        { // ONE
            (void*)QMAP_1920_Sub,
#if (ENABLE_PQ_EX)
            (void*)QMAP_1920_Sub_Ex,
#endif
            {
#if (PQ_GRULE_NR_ENABLE)
                (void*)MST_GRule_1920_NR_Sub,
#endif
            }
        },
        { // PIP
            (void*)QMAP_1920_PIP_Sub,
#if (ENABLE_PQ_EX)
            (void*)QMAP_1920_PIP_Sub_Ex,
#endif
            {
#if (PQ_GRULE_NR_ENABLE)
                (void*)MST_GRule_1920_PIP_NR_Sub,
#endif
            }
        },
        { // POP
            (void*)QMAP_1920_POP_Sub,
#if (ENABLE_PQ_EX)
            (void*)QMAP_1920_POP_Sub_Ex,
#endif
            {
#if (PQ_GRULE_NR_ENABLE)
                (void*)MST_GRule_1920_POP_NR_Sub,
#endif
            }
        },
    },

};
#endif

static MS_U8 _MDrv_PQ_Get_PanelIndexByDisplayWidth(MS_U16 u16DisplayWidth)
{
    MS_U8 u8PanelIndex = 0;

    switch(u16DisplayWidth)
    {
        case 1920:
        default:
            u8PanelIndex = PQ_BIN_PNLTYPE_1920;
            break;
    }
    return u8PanelIndex;
}

static MS_BOOL _MDrv_PQ_Set_DisplayType_Main(void *pInstance,MS_U16 u16DisplayWidth, PQ_DISPLAY_TYPE enDisplaType, MS_BOOL bSkipCommTable)
{
    MS_U8 u8PanelIndex = 0;
    PQTABLE_INFO PQTableInfo;
    PQTABLE_INFO PQTableInfoEx;

    memset(&PQTableInfo, 0, sizeof(PQTABLE_INFO));
    memset(&PQTableInfoEx, 0, sizeof(PQTABLE_INFO));

    u8PanelIndex = _MDrv_PQ_Get_PanelIndexByDisplayWidth(u16DisplayWidth);

    PQTableInfo.pQuality_Map_Aray = (void*)QmapTblInitInfo_Main[u8PanelIndex][enDisplaType].pQuality_Map_Aray;
#if (PQ_GRULE_NR_ENABLE)
    PQTableInfo.pGRule_Level[E_GRULE_NR] =
        (void*)QmapTblInitInfo_Main[u8PanelIndex][enDisplaType].pGRule_Level[E_GRULE_NR];
#endif
#if (PQ_GRULE_MPEG_NR_ENABLE)
    PQTableInfo.pGRule_Level[E_GRULE_MPEG_NR] =
        (void*)QmapTblInitInfo_Main[u8PanelIndex][enDisplaType].pGRule_Level[E_GRULE_MPEG_NR];
#endif
#if (PQ_GRULE_FILM_MODE_ENABLE)
    PQTableInfo.pGRule_Level[E_GRULE_FilmMode] =
        (void*)QmapTblInitInfo_Main[u8PanelIndex][enDisplaType].pGRule_Level[E_GRULE_FilmMode];
#endif
#if (PQ_GRULE_DYNAMIC_CONTRAST_ENABLE)
    PQTableInfo.pGRule_Level[E_GRULE_DynContr] =
        (void*)QmapTblInitInfo_Main[u8PanelIndex][enDisplaType].pGRule_Level[E_GRULE_DynContr];
#endif
#if (PQ_GRULE_ULTRAT_CLEAR_ENABLE)
    PQTableInfo.pGRule_Level[E_GRULE_ULTRACLEAR] =
        (void*)QmapTblInitInfo_Main[u8PanelIndex][enDisplaType].pGRule_Level[E_GRULE_ULTRACLEAR];
#endif
#if (PQ_GRULE_DDR_SELECT_ENABLE == 1)
    PQTableInfo.pGRule_Level[E_GRULE_DDR_SELECT] =
        (void*)QmapTblInitInfo_Main[u8PanelIndex][enDisplaType].pGRule_Level[E_GRULE_DDR_SELECT];
#endif
#if (PQ_GRULE_OSD_BW_ENABLE == 1)
    PQTableInfo.pGRule_Level[E_GRULE_OSD_BW] =
        (void*)QmapTblInitInfo_Main[u8PanelIndex][enDisplaType].pGRule_Level[E_GRULE_OSD_BW];
#endif
#if (PQ_GRULE_PTP_ENABLE == 1)
    PQTableInfo.pGRule_Level[E_GRULE_PTP] =
        (void*)QmapTblInitInfo_Main[u8PanelIndex][enDisplaType].pGRule_Level[E_GRULE_PTP];
#endif
#if (PQ_GRULE_XVYCC_ENABLE == 1 )
    PQTableInfo.pGRule_Level[E_GRULE_XVYCC] =
        (void*)QmapTblInitInfo_Main[u8PanelIndex][enDisplaType].pGRule_Level[E_GRULE_XVYCC];
#endif
#if (PQ_GRULE_GAME_MODE_ENABLE == 1)
    PQTableInfo.pGRule_Level[E_GRULE_GAME_MODE] =
        (void*)QmapTblInitInfo_Main[u8PanelIndex][enDisplaType].pGRule_Level[E_GRULE_GAME_MODE];
#endif
#if (PQ_GRULE_DS_PARAMETERS_ENABLE == 1)
    PQTableInfo.pGRule_Level[E_GRULE_DS_PARAMETERS] =
        (void*)QmapTblInitInfo_Main[u8PanelIndex][enDisplaType].pGRule_Level[E_GRULE_DS_PARAMETERS];
#endif
#if (PQ_GRULE_HDR_ENABLE == 1)
    PQTableInfo.pGRule_Level[E_GRule_HDR_Main] =
        (void*)QmapTblInitInfo_Main[u8PanelIndex][enDisplaType].pGRule_Level[E_GRule_HDR_Main];
#endif


    // table config parameter
    PQTableInfo.u8PQ_InputType_Num = QM_INPUTTYPE_NUM_Main;
    PQTableInfo.u8PQ_IP_Num = PQ_IP_NUM_Main;
    PQTableInfo.pIPTAB_Info = (void*)PQ_IPTAB_INFO_Main;
    PQTableInfo.pSkipRuleIP = (void*)MST_SkipRule_IP_Main;

    PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_HSD] = PQ_HSDRule_IP_NUM_Main;
    PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_VSD] = PQ_VSDRule_IP_NUM_Main;
    PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_HSP] = PQ_HSPRule_IP_NUM_Main;
    PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_VSP] = PQ_VSPRule_IP_NUM_Main;
    PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_CSC] = PQ_CSCRule_IP_NUM_Main;
#if (PQ_XRULE_DB_ENABLE == 1)
    PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_DB_NTSC] = PQ_DBRule_NTSC_IP_NUM_Main;
    PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_DB_PAL]  = PQ_DBRule_PAL_IP_NUM_Main;
#endif

    PQTableInfo.pXRule_IP_Index[E_XRULE_HSD] = (void*)MST_HSDRule_IP_Index_Main;
    PQTableInfo.pXRule_IP_Index[E_XRULE_VSD] = (void*)MST_VSDRule_IP_Index_Main;
    PQTableInfo.pXRule_IP_Index[E_XRULE_HSP] = (void*)MST_HSPRule_IP_Index_Main;
    PQTableInfo.pXRule_IP_Index[E_XRULE_VSP] = (void*)MST_VSPRule_IP_Index_Main;
    PQTableInfo.pXRule_IP_Index[E_XRULE_CSC] = (void*)MST_CSCRule_IP_Index_Main;
#if (PQ_XRULE_DB_ENABLE == 1)
    PQTableInfo.pXRule_IP_Index[E_XRULE_DB_NTSC] = (void*)MST_DBRule_NTSC_IP_Index_Main;
    PQTableInfo.pXRule_IP_Index[E_XRULE_DB_PAL]  = (void*)MST_DBRule_PAL_IP_Index_Main;
#endif

    PQTableInfo.pXRule_Array[E_XRULE_HSD] = (void*)MST_HSDRule_Array_Main;
    PQTableInfo.pXRule_Array[E_XRULE_VSD] = (void*)MST_VSDRule_Array_Main;
    PQTableInfo.pXRule_Array[E_XRULE_HSP] = (void*)MST_HSPRule_Array_Main;
    PQTableInfo.pXRule_Array[E_XRULE_VSP] = (void*)MST_VSPRule_Array_Main;
    PQTableInfo.pXRule_Array[E_XRULE_CSC] = (void*)MST_CSCRule_Array_Main;
#if (PQ_XRULE_DB_ENABLE == 1)
    PQTableInfo.pXRule_Array[E_XRULE_DB_NTSC] = (void*)MST_DBRule_NTSC_Array_Main;
    PQTableInfo.pXRule_Array[E_XRULE_DB_PAL]  = (void*)MST_DBRule_PAL_Array_Main;
#endif

    PQTableInfo.u8PQ_GRule_Num[E_GRULE_NR] = PQ_GRULE_NR_NUM_Main;
    PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_NR] = PQ_GRULE_NR_IP_NUM_Main;
    PQTableInfo.pGRule_IP_Index[E_GRULE_NR] = (void*)MST_GRule_NR_IP_Index_Main;
    PQTableInfo.pGRule_Array[E_GRULE_NR] = (void*)MST_GRule_NR_Main;
#if (PQ_GRULE_MPEG_NR_ENABLE)
    PQTableInfo.u8PQ_GRule_Num[E_GRULE_MPEG_NR] = PQ_GRULE_MPEG_NR_NUM_Main;
    PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_MPEG_NR] = PQ_GRULE_MPEG_NR_IP_NUM_Main;
    PQTableInfo.pGRule_IP_Index[E_GRULE_MPEG_NR] = (void*)MST_GRule_MPEG_NR_IP_Index_Main;
    PQTableInfo.pGRule_Array[E_GRULE_MPEG_NR] = (void*)MST_GRule_MPEG_NR_Main;
#endif

#if (PQ_GRULE_FILM_MODE_ENABLE)
    PQTableInfo.u8PQ_GRule_Num[E_GRULE_FilmMode] = PQ_GRULE_FILM_MODE_NUM_Main;
    PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_FilmMode] = PQ_GRULE_FILM_MODE_IP_NUM_Main;
    PQTableInfo.pGRule_IP_Index[E_GRULE_FilmMode] = (void*)MST_GRule_FILM_MODE_IP_Index_Main;
    PQTableInfo.pGRule_Array[E_GRULE_FilmMode] = (void*)MST_GRule_FILM_MODE_Main;
#endif

#if (PQ_GRULE_DYNAMIC_CONTRAST_ENABLE)
    PQTableInfo.u8PQ_GRule_Num[E_GRULE_DynContr] = PQ_GRULE_DYNAMIC_CONTRAST_NUM_Main;
    PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_DynContr] = PQ_GRULE_DYNAMIC_CONTRAST_IP_NUM_Main;
    PQTableInfo.pGRule_IP_Index[E_GRULE_DynContr] = (void*)MST_GRule_DYNAMIC_CONTRAST_IP_Index_Main;
    PQTableInfo.pGRule_Array[E_GRULE_DynContr] = (void*)MST_GRule_DYNAMIC_CONTRAST_Main;
#endif

#if (PQ_GRULE_ULTRAT_CLEAR_ENABLE)
    PQTableInfo.u8PQ_GRule_Num[E_GRULE_ULTRACLEAR] = PQ_GRULE_ULTRAT_CLEAR_NUM_Main;
    PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_ULTRACLEAR] = PQ_GRULE_ULTRAT_CLEAR_IP_NUM_Main;
    PQTableInfo.pGRule_IP_Index[E_GRULE_ULTRACLEAR] = (void*)MST_GRule_ULTRAT_CLEAR_IP_Index_Main;
    PQTableInfo.pGRule_Array[E_GRULE_ULTRACLEAR] = (void*)MST_GRule_ULTRAT_CLEAR_Main;
#endif

#if (PQ_GRULE_DDR_SELECT_ENABLE == 1)
    PQTableInfo.u8PQ_GRule_Num[E_GRULE_DDR_SELECT] = PQ_GRULE_DDR_SELECT_NUM_Main;
    PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_DDR_SELECT] = PQ_GRULE_DDR_SELECT_IP_NUM_Main;
    PQTableInfo.pGRule_IP_Index[E_GRULE_DDR_SELECT] = (void*)MST_GRule_DDR_SELECT_IP_Index_Main;
    PQTableInfo.pGRule_Array[E_GRULE_DDR_SELECT] = (void*)MST_GRule_DDR_SELECT_Main;
#endif

#if (PQ_GRULE_OSD_BW_ENABLE == 1)
    PQTableInfo.u8PQ_GRule_Num[E_GRULE_OSD_BW] = PQ_GRULE_OSD_BW_NUM_Main;
    PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_OSD_BW] = PQ_GRULE_OSD_BW_IP_NUM_Main;
    PQTableInfo.pGRule_IP_Index[E_GRULE_OSD_BW] = (void*)MST_GRule_OSD_BW_IP_Index_Main;
    PQTableInfo.pGRule_Array[E_GRULE_OSD_BW] = (void*)MST_GRule_OSD_BW_Main;
#endif

#if (PQ_GRULE_PTP_ENABLE == 1)
    PQTableInfo.u8PQ_GRule_Num[E_GRULE_PTP] = PQ_GRULE_PTP_NUM_Main;
    PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_PTP] = PQ_GRULE_PTP_IP_NUM_Main;
    PQTableInfo.pGRule_IP_Index[E_GRULE_PTP] = (void*)MST_GRule_PTP_IP_Index_Main;
    PQTableInfo.pGRule_Array[E_GRULE_PTP] = (void*)MST_GRule_PTP_Main;
#endif

#if (PQ_GRULE_XVYCC_ENABLE == 1 )
    PQTableInfo.u8PQ_GRule_Num[E_GRULE_XVYCC] = PQ_GRULE_XVYCC_NUM_Main;
    PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_XVYCC] = PQ_GRULE_XVYCC_IP_NUM_Main;
    PQTableInfo.pGRule_IP_Index[E_GRULE_XVYCC] = (void*)MST_GRule_XVYCC_IP_Index_Main;
    PQTableInfo.pGRule_Array[E_GRULE_XVYCC] = (void*)MST_GRule_XVYCC_Main;
#endif

#if (PQ_GRULE_GAME_MODE_ENABLE == 1)
    PQTableInfo.u8PQ_GRule_Num[E_GRULE_GAME_MODE] = PQ_GRULE_GAME_MODE_NUM_Main;
    PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_GAME_MODE] = PQ_GRULE_GAME_MODE_IP_NUM_Main;
    PQTableInfo.pGRule_IP_Index[E_GRULE_GAME_MODE] = (void*)MST_GRule_GAME_MODE_IP_Index_Main;
    PQTableInfo.pGRule_Array[E_GRULE_GAME_MODE] = (void*)MST_GRule_GAME_MODE_Main;
#endif
#if (PQ_GRULE_DS_PARAMETERS_ENABLE == 1)
    PQTableInfo.u8PQ_GRule_Num[E_GRULE_DS_PARAMETERS] = PQ_GRULE_DS_PARAMETERS_NUM_Main;
    PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_DS_PARAMETERS] = PQ_GRULE_DS_PARAMETERS_IP_NUM_Main;
    PQTableInfo.pGRule_IP_Index[E_GRULE_DS_PARAMETERS] = (void*)MST_GRule_DS_PARAMETERS_IP_Index_Main;
    PQTableInfo.pGRule_Array[E_GRULE_DS_PARAMETERS] = (void*)MST_GRule_DS_PARAMETERS_Main;
#endif
#if (PQ_GRULE_HDR_ENABLE == 1)
    PQTableInfo.u8PQ_GRule_Num[E_GRule_HDR_Main] = PQ_GRULE_HDR_NUM_Main;
    PQTableInfo.u8PQ_GRule_IPNum[E_GRule_HDR_Main] = PQ_GRULE_HDR_IP_NUM_Main;
    PQTableInfo.pGRule_IP_Index[E_GRule_HDR_Main] = (void*)MST_GRule_HDR_IP_Index_Main;
    PQTableInfo.pGRule_Array[E_GRule_HDR_Main] = (void*)MST_GRule_HDR_Main;
#endif

    MDrv_PQ_AddTable_(MAIN,pInstance, &PQTableInfo);
    if(!bSkipCommTable)
    {
        MDrv_PQ_LoadCommTable_(MAIN,pInstance);
    }

#if (ENABLE_PQ_EX)
    // table config parameter
    PQTableInfoEx = PQTableInfo;

    PQTableInfoEx.u8PQ_InputType_Num = QM_INPUTTYPE_NUM_Main_Ex;
    PQTableInfoEx.u8PQ_IP_Num = PQ_IP_NUM_Main_Ex;
    PQTableInfoEx.pIPTAB_Info = (void*)PQ_IPTAB_INFO_Main_Ex;
    PQTableInfoEx.pSkipRuleIP = (void*)MST_SkipRule_IP_Main_Ex;

    PQTableInfoEx.pQuality_Map_Aray =
        (void*)QmapTblInitInfo_Main[u8PanelIndex][enDisplaType].pQuality_Map_Aray_Ex;

    MDrv_PQ_AddTable_(MAINEX,pInstance, &PQTableInfoEx);
    if(!bSkipCommTable)
    {
        MDrv_PQ_LoadCommTable_(MAINEX,pInstance);
    }
    if((MS_U16)QM_INPUTTYPE_NUM_Main != (MS_U16)QM_INPUTTYPE_NUM_Main_Ex)
    {
        //printf("PQ INPUTTYPE NUM MISMATCH: MAIN != EX");
        //MS_ASSERT(0);
    }
#endif
    return TRUE;
}

static MS_BOOL _MDrv_PQ_Set_DisplayType_Sub(void *pInstance,MS_U16 u16DisplayWidth, PQ_DISPLAY_TYPE enDisplaType, MS_BOOL bSkipCommTable)
{
#if PQ_ENABLE_PIP
    MS_U8 u8PanelIndex = 0;

    PQTABLE_INFO PQTableInfo;
    PQTABLE_INFO PQTableInfoEx;

    memset(&PQTableInfo, 0, sizeof(PQTABLE_INFO));
    memset(&PQTableInfoEx, 0, sizeof(PQTABLE_INFO));

    u8PanelIndex = _MDrv_PQ_Get_PanelIndexByDisplayWidth(u16DisplayWidth);

    PQTableInfo.pQuality_Map_Aray =
        (void*)QmapTblInitInfo_Sub[u8PanelIndex][enDisplaType].pQuality_Map_Aray;
#if (PQ_GRULE_NR_ENABLE)
    PQTableInfo.pGRule_Level[E_GRULE_NR] =
        (void*)QmapTblInitInfo_Sub[u8PanelIndex][enDisplaType].pGRule_Level[E_GRULE_NR];

#endif

    // table config parameter
    PQTableInfo.u8PQ_InputType_Num = QM_INPUTTYPE_NUM_Sub;
    PQTableInfo.u8PQ_IP_Num = PQ_IP_NUM_Sub;
    PQTableInfo.pIPTAB_Info = (void*)PQ_IPTAB_INFO_Sub;
    PQTableInfo.pSkipRuleIP = (void*)MST_SkipRule_IP_Sub;

    PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_HSD] = PQ_HSDRule_IP_NUM_Sub;
    PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_VSD] = PQ_VSDRule_IP_NUM_Sub;
    PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_HSP] = PQ_HSPRule_IP_NUM_Sub;
    PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_VSP] = PQ_VSPRule_IP_NUM_Sub;
    PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_CSC] = PQ_CSCRule_IP_NUM_Sub;
#if (PQ_XRULE_DB_ENABLE == 1)
    PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_DB_NTSC] = PQ_DBRule_NTSC_IP_NUM_Sub;
    PQTableInfo.u8PQ_XRule_IP_Num[E_XRULE_DB_PAL] = PQ_DBRule_PAL_IP_NUM_Sub;
#endif

    PQTableInfo.pXRule_IP_Index[E_XRULE_HSD] = (void*)MST_HSDRule_IP_Index_Sub;
    PQTableInfo.pXRule_IP_Index[E_XRULE_VSD] = (void*)MST_VSDRule_IP_Index_Sub;
    PQTableInfo.pXRule_IP_Index[E_XRULE_HSP] = (void*)MST_HSPRule_IP_Index_Sub;
    PQTableInfo.pXRule_IP_Index[E_XRULE_VSP] = (void*)MST_VSPRule_IP_Index_Sub;
    PQTableInfo.pXRule_IP_Index[E_XRULE_CSC] = (void*)MST_CSCRule_IP_Index_Sub;
#if (PQ_XRULE_DB_ENABLE == 1)
    PQTableInfo.pXRule_IP_Index[E_XRULE_DB_NTSC] = (void*)MST_DBRule_NTSC_IP_Index_Sub;
    PQTableInfo.pXRule_IP_Index[E_XRULE_DB_PAL]  = (void*)MST_DBRule_PAL_IP_Index_Sub;
#endif

    PQTableInfo.pXRule_Array[E_XRULE_HSD] = (void*)MST_HSDRule_Array_Sub;
    PQTableInfo.pXRule_Array[E_XRULE_VSD] = (void*)MST_VSDRule_Array_Sub;
    PQTableInfo.pXRule_Array[E_XRULE_HSP] = (void*)MST_HSPRule_Array_Sub;
    PQTableInfo.pXRule_Array[E_XRULE_VSP] = (void*)MST_VSPRule_Array_Sub;
    PQTableInfo.pXRule_Array[E_XRULE_CSC] = (void*)MST_CSCRule_Array_Sub;
#if (PQ_XRULE_DB_ENABLE == 1)
    PQTableInfo.pXRule_Array[E_XRULE_DB_NTSC] = (void*)MST_DBRule_NTSC_Array_Sub;
    PQTableInfo.pXRule_Array[E_XRULE_DB_PAL]  = (void*)MST_DBRule_PAL_Array_Sub;
#endif

    PQTableInfo.u8PQ_GRule_Num[E_GRULE_NR] = PQ_GRULE_NR_NUM_Sub;
    PQTableInfo.u8PQ_GRule_IPNum[E_GRULE_NR] = PQ_GRULE_NR_IP_NUM_Sub;
    PQTableInfo.pGRule_IP_Index[E_GRULE_NR] = (void*)MST_GRule_NR_IP_Index_Sub;
    PQTableInfo.pGRule_Array[E_GRULE_NR] = (void*)MST_GRule_NR_Sub;

    MDrv_PQ_AddTable_(SUB,pInstance, &PQTableInfo);
    if(!bSkipCommTable)
    {
        MDrv_PQ_LoadCommTable_(SUB,pInstance);
    }

#if (ENABLE_PQ_EX)
    // table config parameter
    PQTableInfoEx = PQTableInfo;

    PQTableInfoEx.u8PQ_InputType_Num = QM_INPUTTYPE_NUM_Sub_Ex;
    PQTableInfoEx.u8PQ_IP_Num = PQ_IP_NUM_Sub_Ex;
    PQTableInfoEx.pIPTAB_Info = (void*)PQ_IPTAB_INFO_Sub_Ex;
    PQTableInfoEx.pSkipRuleIP = (void*)MST_SkipRule_IP_Sub_Ex;

    PQTableInfoEx.pQuality_Map_Aray =
        (void*)QmapTblInitInfo_Sub[u8PanelIndex][enDisplaType].pQuality_Map_Aray_Ex;

    MDrv_PQ_AddTable_(SUBEX,pInstance, &PQTableInfoEx);
    if(!bSkipCommTable)
    {
        MDrv_PQ_LoadCommTable_(SUBEX,pInstance);
    }
#endif
#endif
    return TRUE;
}

void MDrv_PQ_Init_DisplayType_U2(void* pInstance,MS_U16 u16DisplayWidth, PQ_DISPLAY_TYPE enDisplaType)
{
    PQINFO_DBG(printf("%s: enDisplaType=%u, u16DisplayWidth=%u\n", __FUNCTION__, enDisplaType, u16DisplayWidth));

    if(gbPQBinEnable)
    {
        MDrv_PQBin_SetPanelID(pInstance,_MDrv_PQ_Get_PanelIndexByDisplayWidth(u16DisplayWidth));
        MDrv_PQBin_SetDisplayType(pInstance,enDisplaType == PQ_DISPLAY_ONE ? PQ_BIN_DISPLAY_ONE :
                                  enDisplaType == PQ_DISPLAY_PIP ? PQ_BIN_DISPLAY_PIP :
                                  PQ_BIN_DISPLAY_POP);
    }
    else
    {
        MS_BOOL bSkipCommTable = PQ_SKIPCOMMTB_ENABLE;

        _MDrv_PQ_Set_DisplayType_Sub(pInstance,u16DisplayWidth, enDisplaType, bSkipCommTable);
        _MDrv_PQ_Set_DisplayType_Main(pInstance,u16DisplayWidth, enDisplaType, bSkipCommTable);
        gu16DisplayWidth = u16DisplayWidth;
    }
#if (PQ_ENABLE_PIP)
    genDisplayType = PQ_DISPLAY_ONE;
#endif
}
void MDrv_PQ_Init_DisplayType(MS_U16 u16DisplayWidth, PQ_DISPLAY_TYPE enDisplaType)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_Init_DisplayType PQArgs;
    PQArgs.u16DisplayWidth = u16DisplayWidth;
    PQArgs.enDisplaType = enDisplaType;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_INIT_DISPLAY_TYPE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}

void MDrv_PQ_Set_DisplayType_U2(void* pInstance,MS_U16 u16DisplayWidth, PQ_DISPLAY_TYPE enDisplaType)
{
    if(enDisplaType != PQ_DISPLAY_ONE)
    {
        _gbSubWin_En = TRUE;
    }
    else
    {
        _gbSubWin_En = FALSE;
    }

    _enPQDisplayType = enDisplaType;

    // QMAP(one, pip, pop) should be the same.
    // we need to handle the differnce between one/pip/pop by Grule, not by summary page.
    // because Qmap is complicatedly related with VD, ADC sampling. It's not simply dumping table.
    // eg. single-->pop, we will load 1920_pop page, but VD won't be re-set again according to
    // new VD sampling setting in Pop Page. So, conflict.
    {
        enDisplaType = PQ_DISPLAY_ONE;
    }

#if (PQ_ENABLE_PIP)
    PQINFO_DBG(printf("%s: enDisplaType=%u, u16DisplayWidth=%u\n", __FUNCTION__, enDisplaType, u16DisplayWidth));

    if(gbPQBinEnable)
    {
        MDrv_PQBin_SetPanelID(pInstance,_MDrv_PQ_Get_PanelIndexByDisplayWidth(u16DisplayWidth));
        MDrv_PQBin_SetDisplayType(pInstance,enDisplaType == PQ_DISPLAY_ONE ? PQ_BIN_DISPLAY_ONE :
                                  enDisplaType == PQ_DISPLAY_PIP ? PQ_BIN_DISPLAY_PIP :
                                  PQ_BIN_DISPLAY_POP);
    }
    else
    {
        MS_BOOL bSkipCommTable = TRUE;

        if((genDisplayType != enDisplaType) || (gu16DisplayWidth != u16DisplayWidth))
        {
            _MDrv_PQ_Set_DisplayType_Sub(pInstance,u16DisplayWidth, enDisplaType, bSkipCommTable);
            _MDrv_PQ_Set_DisplayType_Main(pInstance,u16DisplayWidth, enDisplaType, bSkipCommTable);
            genDisplayType = enDisplaType;
            gu16DisplayWidth = u16DisplayWidth;
        }
    }

#else
    if(gbPQBinEnable)
    {
        UNUSED(u16DisplayWidth);
        UNUSED(enDisplaType);
    }
    else
    {
        MS_BOOL bSkipCommTable = TRUE;

        if((gu16DisplayWidth != u16DisplayWidth))
        {
            _MDrv_PQ_Set_DisplayType_Main(pInstance,u16DisplayWidth, enDisplaType, bSkipCommTable);
            gu16DisplayWidth = u16DisplayWidth;
        }
    }

#endif
}
void MDrv_PQ_Set_DisplayType(MS_U16 u16DisplayWidth, PQ_DISPLAY_TYPE enDisplaType)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_Set_DisplayType PQArgs;
    PQArgs.u16DisplayWidth = u16DisplayWidth;
    PQArgs.enDisplaType = enDisplaType;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_DISPLAY_TYPE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}

void MDrv_PQDS_Update_PanelID(void *pInstance, MS_U16 u16DisplayWidth)
{
    if(u16DisplayWidth <= 1366)
    {
       MDrv_PQBin_SetPanelID(pInstance,_MDrv_PQ_Get_PanelIndexByDisplayWidth(1366));
    }
    else if (u16DisplayWidth == 1920)
    {
        MDrv_PQBin_SetPanelID(pInstance,_MDrv_PQ_Get_PanelIndexByDisplayWidth(1920));
    }
    else if (u16DisplayWidth == 3840)
    {
        MDrv_PQBin_SetPanelID(pInstance,_MDrv_PQ_Get_PanelIndexByDisplayWidth(3840));
    }
    else
    {
        MDrv_PQBin_SetPanelID(pInstance,_MDrv_PQ_Get_PanelIndexByDisplayWidth(1920));
    }

}

MS_BOOL MDrv_PQ_GetMADiFromOSDBWGrule_U2(void* pInstance,PQ_WIN eWindow,
                                           MS_U16 *pu16TabIdx_MADi,
                                           MS_U16 *pu16TabIdx_MADi_Motion)
{
#if PQ_GRULE_OSD_BW_ENABLE
    MS_U16 u16PQ_OSDBWIdx = 0;

    if(eWindow == PQ_MAIN_WINDOW)
    {
    #if (PQ_OSD_BW_VER == 1)
        #if PQ_GRULE_DDR_SELECT_ENABLE
        {
            if(bw_info.u32Miu1MemSize ==0) // 1 DDR
            {
                u16PQ_OSDBWIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_1DDR_On_Main);
                *pu16TabIdx_MADi = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_OSD_BW_Main, u16PQ_OSDBWIdx, 0); // only MADi ip
            }
            else // 2 DDR
            {
                u16PQ_OSDBWIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_2DDR_On_Main);
                *pu16TabIdx_MADi = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_OSD_BW_Main, u16PQ_OSDBWIdx, 0); // only MADi ip
            }
        }
        #else //DDR select disable
        {
            u16PQ_OSDBWIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_On_Main);
            *pu16TabIdx_MADi = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_OSD_BW_Main, u16PQ_OSDBWIdx, 0); // only MADi ip
        }
        #endif
    #elif (PQ_OSD_BW_VER == 2)
        #if PQ_GRULE_DDR_SELECT_ENABLE
        if(bw_info.bDDR2 == FALSE) // DDR 3
        {
             if(MApi_XC_GetMirrorModeTypeEx(MAIN_WINDOW)  != MIRROR_NORMAL)
             {
                 PQMADi_DBG(printf("MDrv_PQ_GetMADiFromGrule_OSDBW_Ver2: DDR3 mirror\n"));
                 u16PQ_OSDBWIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_On_DDR3_Mirror_Main);
                 *pu16TabIdx_MADi = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_OSD_BW_Main, u16PQ_OSDBWIdx, 0); // only MADi ip
             }
             else
             {
                 PQMADi_DBG(printf("MDrv_PQ_GetMADiFromGrule_OSDBW_Ver2: DDR3 non-mirror\n"));
                 u16PQ_OSDBWIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_On_DDR3_Main);
                 *pu16TabIdx_MADi = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_OSD_BW_Main, u16PQ_OSDBWIdx, 0); // only MADi ip
             }
        }
        else // DDR 2
        {
             if(MApi_XC_GetMirrorModeTypeEx(MAIN_WINDOW)  != MIRROR_NORMAL)
             {
                 PQMADi_DBG(printf("MDrv_PQ_GetMADiFromGrule_OSDBW_Ver2: DDR2 mirror\n"));
                 u16PQ_OSDBWIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_On_DDR2_Mirror_Main);
                 *pu16TabIdx_MADi = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_OSD_BW_Main, u16PQ_OSDBWIdx, 0); // only MADi ip
             }
             else
             {
                 PQMADi_DBG(printf("MDrv_PQ_GetMADiFromGrule_OSDBW_Ver2: DDR2 non-mirror\n"));
                 u16PQ_OSDBWIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_OSD_BW_Main, PQ_GRule_Lvl_OSD_BW_On_DDR2_Main);
                 *pu16TabIdx_MADi = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_OSD_BW_Main, u16PQ_OSDBWIdx, 0); // only MADi ip
             }
        }
        #endif
    #endif

    #if (PQ_GRULE_OSD_BW_IP_NUM_Main > 1)
        *pu16TabIdx_MADi_Motion = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_OSD_BW_Main, u16PQ_OSDBWIdx, 1); // Add MADi_Motion ip
    #else
        UNUSED(pu16TabIdx_MADi_Motion);
    #endif
    }
    else
    {
        // TODO: Should sub window need to consider the OSD BW ??
    }
#else
    UNUSED(eWindow);
    UNUSED(pu16TabIdx_MADi_Motion);
    UNUSED(pu16TabIdx_MADi);
#endif

    return TRUE;
}
MS_BOOL MDrv_PQ_GetMADiFromOSDBWGrule(PQ_WIN eWindow,
                                           MS_U16 *pu16TabIdx_MADi,
                                           MS_U16 *pu16TabIdx_MADi_Motion)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetMADiFromOSDBWGrule PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.pu16TabIdx_MADi = pu16TabIdx_MADi;
    PQArgs.pu16TabIdx_MADi_Motion = pu16TabIdx_MADi_Motion;
    PQArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_MADI_FROM_OSDBW_GRULE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_GetMADiInGeneral_U2(void* pInstance,PQ_WIN eWindow, MS_U16 *pu16TabIdx_MADi, MS_U16 *pu16TabIdx_MADi_Motion)
{
    UNUSED(pu16TabIdx_MADi_Motion);
    UNUSED(pu16TabIdx_MADi);
#if PQ_GRULE_DDR_SELECT_ENABLE
    if(eWindow == PQ_MAIN_WINDOW)
    {
         MS_U16 u16PQ_DDRSELIdx = 0;
    #if (PQ_DDR_SELECT_VER == 1)
         if(bw_info.u32Miu1MemSize==0) // 1 DDR
         {
              PQMADi_DBG(printf("MDrv_PQ_GetMADiInGeneral: 1 DDR \n"));
              u16PQ_DDRSELIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_DDR_SELECT_Main, PQ_GRule_Lvl_DDR_SELECT_1DDR_Main);
         }
         else // 2 DDR
         {
              PQMADi_DBG(printf("MDrv_PQ_GetMADiInGeneral: 2 DDR \n"));
              u16PQ_DDRSELIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_DDR_SELECT_Main, PQ_GRule_Lvl_DDR_SELECT_2DDR_Main);
         }
         *pu16TabIdx_MADi = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_DDR_SELECT_Main, u16PQ_DDRSELIdx, 0); // only MADi ip
    #elif (PQ_DDR_SELECT_VER == 2)
        if(bw_info.bDDR2 == FALSE) // DDR 3
        {
             if(MApi_XC_GetMirrorModeTypeEx(MAIN_WINDOW)  != MIRROR_NORMAL)
             {
                 PQMADi_DBG(printf("MDrv_PQ_GetDDRMirrorSelectInfo: DDR3 mirror\n"));
                 u16PQ_DDRSELIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_DDR_SELECT_Main, PQ_GRule_Lvl_DDR_SELECT_DDR3_Mirror_Main);
             }
             else
             {
                 PQMADi_DBG(printf("MDrv_PQ_GetDDRMirrorSelectInfo: DDR3 non-mirror\n"));
                 u16PQ_DDRSELIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_DDR_SELECT_Main, PQ_GRule_Lvl_DDR_SELECT_DDR3_Main);
             }
        }
        else // DDR 2
        {
             if(MApi_XC_GetMirrorModeTypeEx(MAIN_WINDOW)  != MIRROR_NORMAL)
             {
                 PQMADi_DBG(printf("MDrv_PQ_GetDDRMirrorSelectInfo: DDR2 mirror\n"));
                 u16PQ_DDRSELIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_DDR_SELECT_Main, PQ_GRule_Lvl_DDR_SELECT_DDR2_Mirror_Main);
             }
             else
             {
                 PQMADi_DBG(printf("MDrv_PQ_GetDDRMirrorSelectInfo: DDR2 non-mirror\n"));
                 u16PQ_DDRSELIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_DDR_SELECT_Main, PQ_GRule_Lvl_DDR_SELECT_DDR2_Main);
             }
        }
        *pu16TabIdx_MADi = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_DDR_SELECT_Main, u16PQ_DDRSELIdx, 0);
        *pu16TabIdx_MADi_Motion = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_DDR_SELECT_Main, u16PQ_DDRSELIdx, 1);
    #endif
    }
    else
    {
        #if (ENABLE_LITE_SN == 0)
         *pu16TabIdx_MADi = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_MADi_Sub);
        #endif
    }
#else //DDR select disable
    if(eWindow == PQ_MAIN_WINDOW)
    {
        *pu16TabIdx_MADi = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_MADi_Main);
#if PQ_GRULE_OSD_BW_ENABLE
        *pu16TabIdx_MADi_Motion = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_MADi_Motion_Main); //y MADi_motion ip
#endif
    }
    #if PQ_ENABLE_PIP
    else
    {
        *pu16TabIdx_MADi = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_MADi_Sub);
#if PQ_GRULE_OSD_BW_ENABLE
        *pu16TabIdx_MADi_Motion = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_MADi_Motion_Sub); // MADi_motion ip
#endif
    }
    #endif
     //printf("$$[%s]:%d, %s, u16TabIdx_MADi=%u\n", __FUNCTION__, __LINE__, eWindow?"SUB":"MAIN", u16TabIdx_MADi);

#endif

#if PQ_GRULE_SLZ_ENABLE
     MS_U16 u16PQ_SLZIdx = 0;
     MS_BOOL bSeamlessZappingEnable;

     if(MApi_XC_GetSeamlessZappingStatus(eWindow, &bSeamlessZappingEnable) == E_APIXC_RET_OK)
     {
        if(bSeamlessZappingEnable)
        {
            if(eWindow == PQ_MAIN_WINDOW)
            {
                 u16PQ_SLZIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_SLZ_Main, PQ_GRule_Lvl_SLZ_On_Main);
                 PQMADi_DBG(printf("MDrv_PQ_GetMADiInGeneral: Main LSZ =%u\n", u16PQ_SLZIdx));
                 *pu16TabIdx_MADi        = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_SLZ_Main, u16PQ_SLZIdx, 0); //y MADi ip
                 *pu16TabIdx_MADi_Motion = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_SLZ_Main, u16PQ_SLZIdx, 1); //y MADi_motion ip
            }
            #if PQ_ENABLE_PIP
            else
            {
                 u16PQ_SLZIdx = MDrv_PQ_GetGRule_LevelIndex_U2(pInstance, eWindow, PQ_GRule_SLZ_Sub, PQ_GRule_Lvl_SLZ_On_Sub);
                 PQMADi_DBG(printf("MDrv_PQ_GetMADiInGeneral: Sub LSZ =%u\n", u16PQ_SLZIdx));
                 *pu16TabIdx_MADi        = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_SLZ_Sub, u16PQ_SLZIdx, 0); // MADi ip
                 *pu16TabIdx_MADi_Motion = MDrv_PQ_GetGRule_TableIndex(eWindow, PQ_GRule_SLZ_Sub, u16PQ_SLZIdx, 1); // MADi_motion ip
            }
            #endif
        }
        else
        {
            //it's not slz case
        }
     }
     else
     {
         printf("MApi_XC_GetStatusEx () version is incorrect at %s %d, the code's purpose is not fulfilled.\n",__FILE__,(int)__LINE__);
     }
#else
    UNUSED(pu16TabIdx_MADi_Motion);

#endif

    return TRUE;
}
MS_BOOL MDrv_PQ_GetMADiInGeneral(PQ_WIN eWindow, MS_U16 *pu16TabIdx_MADi, MS_U16 *pu16TabIdx_MADi_Motion)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_GetMADiInGeneral PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.pu16TabIdx_MADi = pu16TabIdx_MADi;
    PQArgs.pu16TabIdx_MADi_Motion = pu16TabIdx_MADi_Motion;
    PQArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_GET_MADI_IN_GENERAL, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_Check_PointToPoint_Condition_U2(void* pInstance,XC_SETWIN_INFO *pstXC_SetWin_Info,PQ_WIN eWindow)
{
    UNUSED(eWindow);
    MS_BOOL bRet = FALSE;

#if PQ_GRULE_PTP_ENABLE
    // HDMI RGB, HDMI YUV444, DVI, VGA.
    // all in progressive mode
    PQP2P_DBG(printf("----HDMI source=%u, HDMIPacket=%u, RGB=%u, YUV444=%u, VGA=%u, interlace=%u\n",
            (IsSrcTypeHDMI(pstXC_SetWin_Info->enInputSourceType)),
            (_stHDMI_Info[eWindow].bIsHDMI == TRUE),
            (QM_HDMIPC_COLORRGB(PQ_MAIN_WINDOW)),
            (QM_HDMIPC_COLORYUV444(PQ_MAIN_WINDOW)),
            (IsSrcTypeVga(pstXC_SetWin_Info->enInputSourceType)),
            (pstXC_SetWin_Info->bInterlace)
            );)
    //at this time, the PQ input source has not been inited. so we use XC_SETWIN_INFO to check
    //we cannot do the PTP when timing size is bigger than panel size
    if((((IsSrcTypeHDMI(pstXC_SetWin_Info->enInputSourceType)) && (_stHDMI_Info[eWindow].bIsHDMI == TRUE) && (QM_HDMIPC_COLORRGB(PQ_MAIN_WINDOW))) ||
         ((IsSrcTypeHDMI(pstXC_SetWin_Info->enInputSourceType)) && (_stHDMI_Info[eWindow].bIsHDMI == TRUE) && (QM_HDMIPC_COLORYUV444(PQ_MAIN_WINDOW))) ||
         ((IsSrcTypeHDMI(pstXC_SetWin_Info->enInputSourceType)) && (_stHDMI_Info[eWindow].bIsHDMI == FALSE) && (QM_HDMIPC_COLORYUV444(PQ_MAIN_WINDOW))) ||
         ((IsSrcTypeHDMI(pstXC_SetWin_Info->enInputSourceType)) && (_stHDMI_Info[eWindow].bIsHDMI == FALSE) && (QM_HDMIPC_COLORRGB(PQ_MAIN_WINDOW))) ||
         (IsSrcTypeVga(pstXC_SetWin_Info->enInputSourceType))) &&
         (!pstXC_SetWin_Info->bInterlace) &&
         ((g_IPanel.Width() >= pstXC_SetWin_Info->stCropWin.width) && (g_IPanel.Height() >= pstXC_SetWin_Info->stCropWin.height) &&
           (eWindow == PQ_MAIN_WINDOW))
         )
    {
        bRet = TRUE;
    }
#else
    UNUSED(pstXC_SetWin_Info);
#endif

    PQP2P_DBG(printf("[MDrv_PQ_Check_PointToPoint_Condition] %u\n",bRet);)

    return bRet;
}
MS_BOOL MDrv_PQ_Check_PointToPoint_Condition(XC_SETWIN_INFO *pstXC_SetWin_Info,PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_Check_PointToPoint_Condition PQArgs;
    PQArgs.pstXC_SetWin_Info = pstXC_SetWin_Info;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_CHECK_POINT2POINT_CONDITION, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_SetPostCCSOnOff_U2(void* pInstance,PQ_WIN eWindow, MS_BOOL bEnable)
{
    if (eWindow >= PQ_MAX_WINDOW)
        return FALSE;

    if (bEnable)
    {
        MS_U16 u16TabIdx =0;
        u16TabIdx = MDrv_PQ_GetTableIndex_U2(pInstance,eWindow, PQ_IP_PostCCS_Main);
        #if(ENABLE_PQ_MLOAD)
            MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
        #endif
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, u16TabIdx, PQ_IP_PostCCS_Main);
        #if(ENABLE_PQ_MLOAD)
            MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
        #endif
    }
    else
    {
        #if(ENABLE_PQ_MLOAD)
            MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
        #endif
            MDrv_PQ_LoadTable_U2(pInstance, eWindow, PQ_IP_PostCCS_OFF_Main, PQ_IP_PostCCS_Main);
        #if(ENABLE_PQ_MLOAD)
            MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
        #endif
    }
    return TRUE;
}
MS_BOOL MDrv_PQ_SetPostCCSOnOff(PQ_WIN eWindow, MS_BOOL bEnable)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_SetPostCCSOnOff PQArgs;
    PQArgs.eWindow = eWindow;
    PQArgs.bEnable = bEnable;
    PQArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_POSTCCS_ONOFF, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

void MDrv_PQ_AdaptiveTuning_U2(void* pInstance)
{
    Hal_PQ_AdaptiveTuning(pInstance);
}
void MDrv_PQ_AdaptiveTuning(void)
{
#ifdef TRACE_DBG
        //printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return ;
        }
    }

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_ADAPTIVE_TUNING, NULL) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}
#ifdef UFO_XC_PQ_SUPPORT_SWDRIVING_MULTI_DEVICES
void MDrv_PQ_EX_AdaptiveTuning(PQ_DEVICE_ID *pDeviceId)
{
    if (pDeviceId->u32Id == 1)
    {
        //need add a PQ_device_1 instance
        return ;
    }

    if (pu32PQInst == NULL)
    {
	    if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return ;
        }
    }

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_ADAPTIVE_TUNING, NULL) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}
#endif
void MDrv_PQ_FilmMode_AnyCandence_Enable_U2(void* pInstance,MS_BOOL bEnable)
{
    Hal_PQ_FilmMode_AnyCandence_Enable(pInstance,bEnable);
}
void MDrv_PQ_FilmMode_AnyCandence_Enable(MS_BOOL bEnable)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stPQ_FilmMode_AnyCandence_Enable PQArgs;
    PQArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_FILMMODE_ANYCANDENCE_ENABLE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}

MS_BOOL MDrv_PQ_SetH264_OnOff_U2(void* pInstance,MS_BOOL bEn)
{
    _gIsH264 = bEn;
    return TRUE;
}
MS_BOOL MDrv_PQ_SetH264_OnOff(MS_BOOL bEn)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return FALSE;
        }
    }

    stPQ_SetH264_OnOff PQArgs;
    PQArgs.bEn = bEn;
    PQArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SETH264_ONOFF, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_SetG3D_OnOff_U2(void* pInstance,MS_BOOL bEn)
{
    _gbIsG3D = bEn;
    return TRUE;
}
MS_BOOL MDrv_PQ_SetG3D_OnOff(MS_BOOL bEn)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_SetG3D_OnOff PQArgs;
    PQArgs.bEn = bEn;
    PQArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SETG3D_ONOFF, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_SetMVC4kx1k_OnOff_U2(void* pInstance,MS_BOOL bEn)
{
    _gIsMVC4kx1k = bEn;
    return TRUE;
}
MS_BOOL MDrv_PQ_SetMVC4kx1k_OnOff(MS_BOOL bEn)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_SetMVC4kx1k_OnOff PQArgs;
    PQArgs.bEn = bEn;
    PQArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SETMVC4KX1K_ONOFF, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_SetNetworkMM_OnOff_U2(void* pInstance,MS_BOOL bEn)
{
    _gIsNetworkMM = bEn;
    return TRUE;
}
MS_BOOL MDrv_PQ_SetNetworkMM_OnOff(MS_BOOL bEn)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return FALSE;
        }
    }

    stPQ_SetNetworkMM_OnOff PQArgs;
    PQArgs.bEn = bEn;
    PQArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_NETWORKMM_ONOFF, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_SetMM_OnOff_U2(void* pInstance,MS_BOOL bEn)
{
    _gIsMMplayer = bEn;
    return TRUE;
}
MS_BOOL MDrv_PQ_SetMM_OnOff(MS_BOOL bEn)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_SetMM_OnOff PQArgs;
    PQArgs.bEn = bEn;
    PQArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SETMM_ONOFF, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_SetRmvb_OnOff_U2(void* pInstance,MS_BOOL bEn, PQ_WIN ePQWindow)
{
    _gIsRmvb[ePQWindow] = bEn;
    return TRUE;
}
MS_BOOL MDrv_PQ_SetRmvb_OnOff(MS_BOOL bEn, PQ_WIN ePQWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        if(UtopiaOpen(MODULE_PQ , &pu32PQInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen PQ failed\n");
            return FALSE;
        }
    }

    stPQ_SetRmvb_OnOff PQArgs;
    PQArgs.bEn = bEn;
    PQArgs.ePQWindow = ePQWindow;
    PQArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_RMVB_ONOFF, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_Set1KTravelingEnable_U2(void* pInstance,MS_BOOL bEn)
{
    _gIs1KTraveling = bEn;
    return TRUE;
}

MS_BOOL MDrv_PQ_Set1KTravelingEnable(MS_BOOL bEn)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_Set1KTravelingEnable PQArgs;
    PQArgs.bEn = bEn;
    PQArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_1KTRAVELINGENABLE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_SetVIPBypass_OnOff_U2(void* pInstance,MS_BOOL bEnable, PQ_WIN eWindow)
{
    if (eWindow == PQ_MAIN_WINDOW)
    {
#if PQ_VIP_CTL
#if(ENABLE_PQ_MLOAD)
        MDrv_PQ_Set_MLoadEn(eWindow, TRUE);
#endif
        if(bEnable)
        {
#if PQ_VIP_OFF_MINUS16
            if(MDrv_PQ_IsForceVideoInputMode_U2(pInstance, PQ_MAIN_WINDOW) == TRUE
               || MApi_XC_IsYUVSpace(MAIN_WINDOW) == TRUE)
            {
                MDrv_PQ_LoadTable_U2(pInstance, PQ_MAIN_WINDOW, PQ_IP_VIP_OFF_Minus16_Main, PQ_IP_VIP_Main);
            }
            else
            {
                MDrv_PQ_LoadTable_U2(pInstance, PQ_MAIN_WINDOW, PQ_IP_VIP_OFF_Main, PQ_IP_VIP_Main);
            }
#else
            MDrv_PQ_LoadTable_U2(pInstance, PQ_MAIN_WINDOW, PQ_IP_VIP_OFF_Main, PQ_IP_VIP_Main);
#endif
        }
        else
        {
            MDrv_PQ_LoadTable_U2(pInstance, PQ_MAIN_WINDOW, PQ_IP_VIP_ON_Main, PQ_IP_VIP_Main);
        }
#if(ENABLE_PQ_MLOAD)
        MDrv_PQ_Set_MLoadEn(eWindow, FALSE);
#endif
#endif
    }
    else if (eWindow == PQ_SUB_WINDOW)
    {
        //none
    }
    else
    {
        printf("Invalid window type, please check ! \n");
        return FALSE;
    }

    return TRUE;
}
MS_BOOL MDrv_PQ_SetVIPBypass_OnOff(MS_BOOL bEnable, PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_SetVIPBypass_OnOff PQArgs;
    PQArgs.bEnable = bEnable;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_VIPBYPASS_ONOFF, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_SetPeaking_OnOff_U2(void* pInstance,MS_BOOL bEnable, PQ_WIN eWindow)
{
    if (eWindow == PQ_MAIN_WINDOW)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK19_10_L, bEnable, 0x0001);
    }
    else if (eWindow == PQ_SUB_WINDOW)
    {
        MApi_XC_W2BYTEMSK(REG_SC_BK19_14_L, bEnable, 0x0001);
    }
    else
    {
        printf("Invalid window type, please check ! \n");
        return FALSE;
    }
    return TRUE;
}
MS_BOOL MDrv_PQ_SetPeaking_OnOff(MS_BOOL bEnable, PQ_WIN eWindow)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_SetPeaking_OnOff PQArgs;
    PQArgs.bEnable = bEnable;
    PQArgs.eWindow = eWindow;
    PQArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_PEAKING_ONOFF, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

MS_BOOL MDrv_PQ_SetFakeOutEnable_U2(void* pInstance,MS_BOOL bEnable)
{
    return Hal_PQ_SetFakeOutEnable(pInstance,bEnable);
}
MS_BOOL MDrv_PQ_SetFakeOutEnable(MS_BOOL bEnable)
{
#ifdef TRACE_DBG
        printf("\33[1;33m[MARCOS](%s %d)\33[m\n", __FUNCTION__, __LINE__);
#endif
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_SetFakeOutEnable PQArgs;
    PQArgs.bEnable = bEnable;
    PQArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_SET_FAKE_OUT_ENABLE, (void*)&PQArgs) != 0)
    {
        printf("Obtain PQ engine fail\n");
        return FALSE;
    }
    else
    {
        return PQArgs.bReturnValue;
    }
}

EXPORT_SYMBOL(MDrv_PQ_AdaptiveTuning);
