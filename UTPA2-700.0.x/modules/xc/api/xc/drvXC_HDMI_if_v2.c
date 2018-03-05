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
///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvXC_HDMI_if_v2.c
/// @author MStar Semiconductor Inc.
/// @brief  HDMI Rx driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_HDMI_RX_V2_C_
#define _API_HDMI_RX_V2_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include "string.h"
#endif
#include "utopia.h"
#include "utopia_dapi.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "apiXC_ModeParse.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "drvXC_HDMI_if.h"
#include "mhal_xc_chip_config.h"
#include "mvideo_context.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drv_sc_ip.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "XC_private.h"
#include "apiXC_v2.h"
#include "drvXC_HDMI_if_v2.h"
#include "apiHDMIRx_private.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if(defined(CONFIG_MLOG))
#include "ULog.h"

#define MAPI_HDMIRX_MSG_INFO(format, args...)       ULOGI("HDMIRX", format, ##args)
#define MAPI_HDMIRX_MSG_WARNING(format, args...)    ULOGW("HDMIRX", format, ##args)
#define MAPI_HDMIRX_MSG_DEBUG(format, args...)      ULOGD("HDMIRX", format, ##args)
#define MAPI_HDMIRX_MSG_ERROR(format, args...)      ULOGE("HDMIRX", format, ##args)
#define MAPI_HDMIRX_MSG_FATAL(format, args...)      ULOGF("HDMIRX", format, ##args)

#else
#define MAPI_HDMIRX_MSG_INFO(format, args...)       printf(format, ##args)
#define MAPI_HDMIRX_MSG_WARNING(format, args...)    printf(format, ##args)
#define MAPI_HDMIRX_MSG_DEBUG(format, args...)      printf(format, ##args)
#define MAPI_HDMIRX_MSG_ERROR(format, args...)      printf(format, ##args)
#define MAPI_HDMIRX_MSG_FATAL(format, args...)      printf(format, ##args)

#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  HDMI_IF_Initialize()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void HDMI_IF_Initialize(XC_INSTANCE_PRIVATE* psXCInstPri, const void* const pAttribute)
{
    psXCInstPri->fpHDMI_Func_Caps =     MDrv_HDMI_Func_Caps_U2;
    psXCInstPri->fpHDMI_init =     MDrv_HDMI_init_U2;
    psXCInstPri->fpHDMI_Exit =     MDrv_HDMI_Exit_U2;
    psXCInstPri->fpHDMI_pkt_reset =     MDrv_HDMI_pkt_reset_U2;
    psXCInstPri->fpHDMI_pullhpd =     MDrv_HDMI_pullhpd_U2;
    psXCInstPri->fpHDMI_GC_Info =     MDrv_HDMI_GC_Info_U2;
    psXCInstPri->fpHDMI_Packet_Received =     MDrv_HDMI_Packet_Received_U2;
    psXCInstPri->fpHDMI_Get_ColorFormat =     MDrv_HDMI_Get_ColorFormat_U2;
    psXCInstPri->fpHDMI_Get_ColorRange =     MDrv_HDMI_Get_ColorRange_U2;
    psXCInstPri->fpHDMI_Set_EQ =     MDrv_HDMI_Set_EQ_U2;
    psXCInstPri->fpHDMI_Set_EQ_To_Port = MDrv_HDMI_Set_EQ_To_Port_U2;
    psXCInstPri->fpHDMI_Audio_MUTE_Enable =     MDrv_HDMI_Audio_MUTE_Enable_U2;
    psXCInstPri->fpHDMI_Audio_Status_Clear =     MDrv_HDMI_Audio_Status_Clear_U2;
    psXCInstPri->fpHDMI_Get_AspectRatio =     MDrv_HDMI_Get_AspectRatio_U2;
    psXCInstPri->fpHDMI_Get_ActiveFormat_AspectRatio =     MDrv_HDMI_Get_ActiveFormat_AspectRatio_U2;
    psXCInstPri->fpHDMI_Get_AVIInfoFrameVer =     MDrv_HDMI_Get_AVIInfoFrameVer_U2;
    psXCInstPri->fpHDMI_err_status_update =     MDrv_HDMI_err_status_update_U2;
    psXCInstPri->fpHDMI_Get_TMDS_freq     =     MDrv_HDMI_GetTMDSFreq_U2;
    psXCInstPri->fpHDMI_Get_PollingStatus =     MDrv_HDMI_Get_PollingStatus_U2;
    psXCInstPri->fpHDMI_IsHDMI_Mode =     MDrv_HDMI_IsHDMI_Mode_U2;
    psXCInstPri->fpHDMI_Get_MID_info =     MDrv_HDMI_Get_MID_info_U2;
    psXCInstPri->fpHDMI_get_packet_value =     MDrv_HDMI_get_packet_value_U2;
    psXCInstPri->fpDVI_ChannelPhaseStatus =     MDrv_DVI_ChannelPhaseStatus_U2;
    psXCInstPri->fpDVI_SoftwareReset =     MDrv_DVI_SoftwareReset_U2;
    psXCInstPri->fpdvi_reset =     mdrv_dvi_reset_U2;
    psXCInstPri->fpHDMI_Get_AVIInfoActiveInfoPresent = MDrv_HDMI_Get_AVIInfoActiveInfoPresent_U2;
    psXCInstPri->fpDVI_ClkPullLow =     MDrv_DVI_ClkPullLow_U2;
    psXCInstPri->fpDVI_SwitchSrc =     MDrv_DVI_SwitchSrc_U2;
    psXCInstPri->fpHDMI_SetForClockLessThan70Mhz =     MDrv_HDMI_SetForClockLessThan70Mhz_U2;
    psXCInstPri->fpHDMI_dvi_adjust =     MDrv_HDMI_dvi_adjust_U2;
    psXCInstPri->fpHDMI_SetUpdatePhaseLineCount =     MDrv_HDMI_SetUpdatePhaseLineCount_U2;
    psXCInstPri->fpHDCP_Enable =     MDrv_HDCP_Enable_U2;
    psXCInstPri->fpHDMI_SetHdcpEnable = MDrv_HDMI_SetHdcpEnable_U2;
    psXCInstPri->fpHDCP_ClearStatus =     MDrv_HDCP_ClearStatus_U2;
    psXCInstPri->fpHDCP_initproductionkey =     MDrv_HDCP_initproductionkey_U2;
    psXCInstPri->fpHDCP_GetStatus =     MDrv_HDCP_GetStatus_U2;
    psXCInstPri->fpHDCP_Vsync_end_en =     MDrv_HDCP_Vsync_end_en_U2;
    psXCInstPri->fpHDMI_audio_output =     MDrv_HDMI_audio_output_U2;
    psXCInstPri->fpHDMI_audio_cp_hdr_info =     MDrv_HDMI_audio_cp_hdr_info_U2;
    psXCInstPri->fpHDMI_audio_channel_status =     MDrv_HDMI_audio_channel_status_U2;
    psXCInstPri->fpHDMI_GetLibVer =     MDrv_HDMI_GetLibVer_U2;
    psXCInstPri->fpHDMI_GetInfo =     MDrv_HDMI_GetInfo_U2;
    psXCInstPri->fpHDMI_GetStatus =     MDrv_HDMI_GetStatus_U2;
    psXCInstPri->fpHDMI_READ_DDCRAM =   MDrv_HDMI_READ_DDCRAM_U2;
    psXCInstPri->fpHDMI_PROG_DDCRAM = MDrv_HDMI_PROG_DDCRAM_U2;
    psXCInstPri->fpHDMI_Get_Ext_Colorimetry = MDrv_HDMI_Get_ExtColorimetry_U2;
    psXCInstPri->fpHDMI_Get_Content_Type =     MDrv_HDMI_Get_Content_Type_U2;
    psXCInstPri->fpHDMI_Get_Pixel_Repetition =     MDrv_HDMI_Get_Pixel_Repetition_U2;
    psXCInstPri->fpHDMI_Check4K2K        = MDrv_HDMI_Check4K2K_U2;
    psXCInstPri->fpHDMI_3D_4Kx2K_Process =     MDrv_HDMI_3D_4Kx2K_Process_U2;
    psXCInstPri->fpHDMI_AVG_ScaleringDown = MDrv_HDMI_AVG_ScaleringDown_U2;
    psXCInstPri->fpHDMI_Check_Additional_Format =     MDrv_HDMI_Check_Additional_Format_U2;
    psXCInstPri->fpHDMI_Get_3D_Structure =     MDrv_HDMI_Get_3D_Structure_U2;
    psXCInstPri->fpHDMI_Get_3D_Ext_Data =     MDrv_HDMI_Get_3D_Ext_Data_U2;
    psXCInstPri->fpHDMI_Get_3D_Meta_Field =     MDrv_HDMI_Get_3D_Meta_Field_U2;
    psXCInstPri->fpHDMI_Get_VIC_Code =     MDrv_HDMI_Get_VIC_Code_U2;
    psXCInstPri->fpHDMI_Get_4Kx2K_VIC_Code =     MDrv_HDMI_Get_4Kx2K_VIC_Code_U2;
    psXCInstPri->fpHDMI_ARC_PINControl =     MDrv_HDMI_ARC_PINControl_U2;
    psXCInstPri->fpDVI_Software_Reset =     MDrv_DVI_Software_Reset_U2;
    psXCInstPri->fpHDMI_Set_PowerState =    MDrv_HDMI_SetPowerState_U2;
    psXCInstPri->fpHDMI_dvi_hf_adjust =     MDrv_DVI_HF_adjust_U2;
    psXCInstPri->fpHDMI_checkhdmi20_setting = MDrv_HDMI_CheckHDMI20_Setting_U2;
    psXCInstPri->fpHDMI_Set5VDetectGPIOSelect = MDrv_HDMI_Set5VDetectGPIOSelect_U2;
    psXCInstPri->fpHDMI_GetDEStableStatus=     MDrv_HDMI_GetDEStableStatus_U2;
    psXCInstPri->fpHDMI_Ctrl = MDrv_HDMI_Ctrl_U2;
    psXCInstPri->fpHDMI_GetDataInfo = MDrv_HDMI_GetDataInfo_U2;
    return;
}

//**************************************************************************
//  [Function Name]:
//                  _HDMI_IF_Ioctl()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 _HDMI_IF_Ioctl(void * pInstance, MS_U32 u32Cmd, void * pArgs)
{
    //void* pModule = NULL;
    //void* pResource = NULL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    XC_INSTANCE_PRIVATE* psXCInstPri = NULL;

    //UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaInstanceGetPrivate(pInstance, (void*)&psXCInstPri)!=UTOPIA_STATUS_SUCCESS)
    {
        MAPI_HDMIRX_MSG_ERROR("[%s,%5d] Instance Get Private failed\n",__FUNCTION__,__LINE__);
    }

    switch(u32Cmd)
    {
        case E_XC_HDMI_IF_CMD_FUNC_CAPS:
            {

                pstHDMI_FUNC_CAPS pXCArgs = (pstHDMI_FUNC_CAPS) pArgs;
                pXCArgs->u16ReturnValue = psXCInstPri->fpHDMI_Func_Caps(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_INIT:
            {
                pstHDMI_INITIAL_SETTING pXCArgs = (pstHDMI_INITIAL_SETTING)pArgs;

                pXCArgs->stInitialTable = psXCInstPri->fpHDMI_init(pInstance);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                //FIX ME: other hdmi interface may assess global vars,maybe should add device1 flow
                CHECK_DEVICE1_INSTANCE();
                pXCArgs->stInitialTable = psXCInstPri->fpHDMI_init(g_pDevice1Instance);
#endif
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_EXIT:
            {

                psXCInstPri->fpHDMI_Exit(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_PKT_RESET:
            {

                pstHDMI_PKT_RESET pXCArgs = (pstHDMI_PKT_RESET) pArgs;
                psXCInstPri->fpHDMI_pkt_reset(pInstance, pXCArgs->breset);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_PULLHPD:
            {

                pstHDMI_PULLHPD pXCArgs = (pstHDMI_PULLHPD) pArgs;
                psXCInstPri->fpHDMI_pullhpd(pInstance, pXCArgs->bHighLow,
                                                       pXCArgs->enInputPortType,
                                                       pXCArgs->bInverse);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GC_INFO:
            {
                pstHDMI_GC_INFO pXCArgs = (pstHDMI_GC_INFO) pArgs;
                pXCArgs->u16ReturnValue = psXCInstPri->fpHDMI_GC_Info(pInstance, pXCArgs->ucHDMIInfoSource, pXCArgs->gcontrol);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_PACKET_RECEIVED:
            {
                pstHDMI_PACKET_RECEIVED pXCArgs = (pstHDMI_PACKET_RECEIVED) pArgs;
                psXCInstPri->fpHDMI_Packet_Received(pInstance, pXCArgs->ucHDMIInfoSource, pXCArgs->bEnable, pXCArgs->ulPacketStatus);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_COLORFORMAT:
            {
                pstHDMI_GET_COLORFORMAT pXCArgs = (pstHDMI_GET_COLORFORMAT) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_Get_ColorFormat(pInstance, pXCArgs->ucHDMIInfoSource);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_COLORRANGE:
            {
                pstHDMI_GET_COLORRANGE pXCArgs = (pstHDMI_GET_COLORRANGE) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_Get_ColorRange(pInstance, pXCArgs->ucHDMIInfoSource);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_XC_HDMI_IF_CMD_SET_EQ:
            {

                pstHDMI_SET_EQ pXCArgs = (pstHDMI_SET_EQ) pArgs;
                psXCInstPri->fpHDMI_Set_EQ(pInstance, pXCArgs->enEq, pXCArgs->u8EQValue);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_SET_EQ_TO_PORT:
            {
                pstHDMI_SET_EQ_TO_PORT pXCArgs = (pstHDMI_SET_EQ_TO_PORT) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpHDMI_Set_EQ_To_Port(pInstance, pXCArgs->enEq, pXCArgs->u8EQValue, pXCArgs->enInputPortType);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_AUDIO_MUTE_ENABLE:
            {

                pstHDMI_AUDIO_MUTE_ENABLE pXCArgs = (pstHDMI_AUDIO_MUTE_ENABLE) pArgs;
                psXCInstPri->fpHDMI_Audio_MUTE_Enable(pInstance, pXCArgs->u16MuteEvent,
                                                                 pXCArgs->u16MuteMask);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_AUDIO_STATUS_CLEAR:
            {

                psXCInstPri->fpHDMI_Audio_Status_Clear(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_ASPECTRATIO:
            {
                pstHDMI_GET_ASPECTRATIO pXCArgs = (pstHDMI_GET_ASPECTRATIO) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_Get_AspectRatio(pInstance, pXCArgs->ucHDMIInfoSource, pXCArgs->bEn_PAR, pXCArgs->bEn_AFAR);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_ACTIVEFORMAT_ASPECTRATIO:
            {
                pstHDMI_GET_ACTIVEFORMAT_ASPECTRATIO pXCArgs = (pstHDMI_GET_ACTIVEFORMAT_ASPECTRATIO) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_Get_ActiveFormat_AspectRatio(pInstance, pXCArgs->ucHDMIInfoSource, pXCArgs->bEn_AFAR);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_AVIINFOFRAMEVER:
            {
                pstHDMI_GET_AVIINFOFRAMEVER pXCArgs = (pstHDMI_GET_AVIINFOFRAMEVER) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_Get_AVIInfoFrameVer(pInstance, pXCArgs->ucHDMIInfoSource);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_ERR_STATUS_UPDATE:
            {
                pstHDMI_ERR_STATUS_UPDATE pXCArgs = (pstHDMI_ERR_STATUS_UPDATE) pArgs;
                pXCArgs->u8ReturnValue = psXCInstPri->fpHDMI_err_status_update(pInstance, pXCArgs->ucHDMIInfoSource, pXCArgs->u8value, pXCArgs->bread);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_TMDS_FREQ:
            {

                pstHDMI_GET_TMDS_FREQ pXCArgs = (pstHDMI_GET_TMDS_FREQ) pArgs;
                pXCArgs->u8ReturnValue = psXCInstPri->fpHDMI_Get_TMDS_freq(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_POLLINGSTATUS:
            {

                pstHDMI_GET_POLLINGSTATUS pXCArgs = (pstHDMI_GET_POLLINGSTATUS) pArgs;
                psXCInstPri->fpHDMI_Get_PollingStatus(pInstance, pXCArgs->pStatus);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_ISHDMI_MODE:
            {

                pstHDMI_ISHDMI_MODE pXCArgs = (pstHDMI_ISHDMI_MODE) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpHDMI_IsHDMI_Mode(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_MID_INFO:
            {
                pstHDMI_GET_MID_INFO pXCArgs = (pstHDMI_GET_MID_INFO) pArgs;
                psXCInstPri->fpHDMI_Get_MID_info(pInstance, pXCArgs->ucHDMIInfoSource, pXCArgs->pu8ManufactorData);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_PACKET_VALUE:
            {
                pstHDMI_GET_PACKET_VALUE pXCArgs = (pstHDMI_GET_PACKET_VALUE) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpHDMI_get_packet_value(pInstance, pXCArgs->ucHDMIInfoSource, pXCArgs->u8state, pXCArgs->u8byte_idx, pXCArgs->pValue);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_DVI_IF_CMD_CHANNELPHASESTATUS:
            {

                pstDVI_CHANNELPHASESTATUS pXCArgs = (pstDVI_CHANNELPHASESTATUS) pArgs;
                pXCArgs->u16ReturnValue = psXCInstPri->fpDVI_ChannelPhaseStatus(pInstance, pXCArgs->u8Channel);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_DVI_IF_CMD_SOFTWARERESET:
            {

                pstDVI_SOFTWARERESET pXCArgs = (pstDVI_SOFTWARERESET) pArgs;
                psXCInstPri->fpDVI_SoftwareReset(pInstance, pXCArgs->u16Reset);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_DVI_IF_CMD_RESET:
            {

                psXCInstPri->fpdvi_reset(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_AVIINFO_ACTIVEINFOPRESENT:
            {
                pstHDMI_GET_AVIIINFO_ACTIVEINFOPRESENT pXCArgs = (pstHDMI_GET_AVIIINFO_ACTIVEINFOPRESENT) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpHDMI_Get_AVIInfoActiveInfoPresent(pInstance, pXCArgs->ucHDMIInfoSource);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_DVI_IF_CMD_CLKPULLLOW:
            {

                pstDVI_CLKPULLLOW pXCArgs = (pstDVI_CLKPULLLOW) pArgs;
                psXCInstPri->fpDVI_ClkPullLow(pInstance,
                                              pXCArgs->bPullLow,
                                              pXCArgs->enInputPortType);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_DVI_IF_CMD_SWITCHSRC:
            {
                pstDVI_SWITCHSRC pXCArgs = (pstDVI_SWITCHSRC) pArgs;
                psXCInstPri->fpDVI_SwitchSrc(pInstance, pXCArgs->enInputPortType, pXCArgs->ucHDMIInfoSource);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_SETFORCLOCKLESSTHAN70MHZ:
            {

                pstHDMI_SETFORCLOCKLESSTHAN70MHZ pXCArgs = (pstHDMI_SETFORCLOCKLESSTHAN70MHZ) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpHDMI_SetForClockLessThan70Mhz(pInstance, pXCArgs->enInputPortType);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_DVI_ADJUST:
            {

                pstHDMI_DVI_ADJUST pXCArgs = (pstHDMI_DVI_ADJUST) pArgs;
                psXCInstPri->fpHDMI_dvi_adjust(pInstance, pXCArgs->bClockLessThan70MHz);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_SETUPDATEPHASELINECOUNT:
            {

                pstHDMI_SETUPDATEPHASELINECOUNT pXCArgs = (pstHDMI_SETUPDATEPHASELINECOUNT) pArgs;
                psXCInstPri->fpHDMI_SetUpdatePhaseLineCount(pInstance, pXCArgs->enInputSourceType);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDCP_IF_CMD_ENABLE:
            {

                pstHDCP_ENABLE pXCArgs = (pstHDCP_ENABLE) pArgs;
                psXCInstPri->fpHDCP_Enable(pInstance, pXCArgs->bEnable);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_SET_HDCP_ENABLE:
            {
                pstHDMI_SET_HDCPENABLE pXCArgs = (pstHDMI_SET_HDCPENABLE) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpHDMI_SetHdcpEnable(pInstance, pXCArgs->enInputPortType, pXCArgs->bEnable);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDCP_IF_CMD_CLEARSTATUS:
            {
                psXCInstPri->fpHDCP_ClearStatus(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDCP_IF_CMD_INITPRODUCTIONKEY:
            {
                pstHDCP_INITPRODUCTIONKEY pXCArgs = (pstHDCP_INITPRODUCTIONKEY) pArgs;
                psXCInstPri->fpHDCP_initproductionkey(pInstance, pXCArgs->pu8HdcpKeyData);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDCP_IF_CMD_GETSTATUS:
            {
                pstHDCP_GETSTATUS pXCArgs = (pstHDCP_GETSTATUS)pArgs;
                pXCArgs->usHDCPStatus = psXCInstPri->fpHDCP_GetStatus(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDCP_IF_CMD_VSYNC_END_EN:
            {

                pstHDCP_VSYNC_END_EN pXCArgs = (pstHDCP_VSYNC_END_EN) pArgs;
                psXCInstPri->fpHDCP_Vsync_end_en(pInstance, pXCArgs->bStable);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_AUDIO_OUTPUT:
            {

                pstHDMI_AUDIO_OUTPUT pXCArgs = (pstHDMI_AUDIO_OUTPUT) pArgs;
                psXCInstPri->fpHDMI_audio_output(pInstance, pXCArgs->bEnable);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_AUDIO_CP_HDR_INFO:
            {
                pstHDMI_AUDIO_CP_HDR_INFO pXCArgs = (pstHDMI_AUDIO_CP_HDR_INFO) pArgs;
                pXCArgs->u8ReturnValue = psXCInstPri->fpHDMI_audio_cp_hdr_info(pInstance, pXCArgs->ucHDMIInfoSource);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_AUDIO_CHANNEL_STATUS:
            {
                pstHDMI_AUDIO_CHANNEL_STATUS pXCArgs = (pstHDMI_AUDIO_CHANNEL_STATUS) pArgs;
                pXCArgs->u8ReturnValue = psXCInstPri->fpHDMI_audio_channel_status(pInstance, pXCArgs->ucHDMIInfoSource, pXCArgs->u8byte);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GETLIBVER:
            {

                pstHDMI_GETLIBVER pXCArgs = (pstHDMI_GETLIBVER) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_GetLibVer(pInstance, pXCArgs->ppVersion);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GETINFO:
            {

                pstHDMI_GETINFO pXCArgs = (pstHDMI_GETINFO) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_GetInfo(pInstance, pXCArgs->pInfo);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GETSTATUS:
            {

                pstHDMI_GETSTATUS pXCArgs = (pstHDMI_GETSTATUS) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_GetStatus(pInstance, pXCArgs->pStatus);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_READ_DDCRAM:
            {
                pstHDMI_READ_DDCRAM pXCArgs = (pstHDMI_READ_DDCRAM) pArgs;
                psXCInstPri->fpHDMI_READ_DDCRAM(pInstance, pXCArgs->pstDDCRam_Info, pXCArgs->u32SizeOfInfo);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_PROG_DDCRAM:
            {
                pstHDMI_PROG_DDCRAM pXCArgs = (pstHDMI_PROG_DDCRAM) pArgs;
                psXCInstPri->fpHDMI_PROG_DDCRAM(pInstance, pXCArgs->pstDDCRam_Info, pXCArgs->u32SizeOfInfo);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_CONTENT_TYPE:
            {
                pstHDMI_GET_CONTENT_TYPE pXCArgs = (pstHDMI_GET_CONTENT_TYPE) pArgs;
                pXCArgs->enReturnValue = psXCInstPri->fpHDMI_Get_Content_Type(pInstance, pXCArgs->ucHDMIInfoSource);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_EXTCOLORIMETRY:
            {
                pstHDMI_GET_EXT_COLORIMETRY pXCArgs = (pstHDMI_GET_EXT_COLORIMETRY) pArgs;
                pXCArgs->enReturnValue = psXCInstPri->fpHDMI_Get_Ext_Colorimetry(pInstance, pXCArgs->ucHDMIInfoSource);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_PIXEL_REPETITION:
            {
                pstHDMI_GET_PIXEL_REPETITION pXCArgs = (pstHDMI_GET_PIXEL_REPETITION) pArgs;
                pXCArgs->u8ReturnValue = psXCInstPri->fpHDMI_Get_Pixel_Repetition(pInstance, pXCArgs->ucHDMIInfoSource);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_CHECK_4K2K:
            {
                pstHDMI_CHECK_4K2K pXCArgs = (pstHDMI_CHECK_4K2K) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpHDMI_Check4K2K(pInstance, pXCArgs->ucHDMIInfoSource);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_3D_4KX2K_PROCESS:
            {
                pstHDMI_3D_4K2K_PROCESS pXCArgs = (pstHDMI_3D_4K2K_PROCESS) pArgs;
                psXCInstPri->fpHDMI_3D_4Kx2K_Process(pInstance, pXCArgs->ucHDMIInfoSource);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_AVG_SCALERINGDOWN:
            {
                pstHDMI_AVG_SCALERINGDOWN pXCArgs = (pstHDMI_AVG_SCALERINGDOWN) pArgs;
                psXCInstPri->fpHDMI_AVG_ScaleringDown(pInstance, pXCArgs->ucHDMIInfoSource, pXCArgs->bTrue);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_CHECK_ADDITIONAL_FORMAT:
            {
                pstHDMI_CHECK_ADDITIONAL_FORMAT pXCArgs = (pstHDMI_CHECK_ADDITIONAL_FORMAT) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_Check_Additional_Format(pInstance, pXCArgs->ucHDMIInfoSource);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_3D_STRUCTURE:
            {
                pstHDMI_GET_3D_STRUCTURE pXCArgs = (pstHDMI_GET_3D_STRUCTURE) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_Get_3D_Structure(pInstance, pXCArgs->ucHDMIInfoSource);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_CMD_GET_3D_EXT_DATA:
            {
                pstHDMI_GET_3D_EXT_DATA pXCArgs = (pstHDMI_GET_3D_EXT_DATA) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_Get_3D_Ext_Data(pInstance, pXCArgs->ucHDMIInfoSource);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_3D_META_FIELD:
            {
                pstHDMI_GET_3D_META_FIELD pXCArgs = (pstHDMI_GET_3D_META_FIELD) pArgs;
                psXCInstPri->fpHDMI_Get_3D_Meta_Field(pInstance, pXCArgs->ucHDMIInfoSource, pXCArgs->pdata);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_VIC_CODE:
            {
                pstHDMI_GET_VIC_CODE pXCArgs = (pstHDMI_GET_VIC_CODE) pArgs;
                pXCArgs->u8ReturnValue = psXCInstPri->fpHDMI_Get_VIC_Code(pInstance, pXCArgs->ucHDMIInfoSource);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_4KX2K_VIC_CODE:
            {
                pstHDMI_GET_4KX2K_VIC_CODE pXCArgs = (pstHDMI_GET_4KX2K_VIC_CODE) pArgs;
                pXCArgs->eReturnValue = psXCInstPri->fpHDMI_Get_4Kx2K_VIC_Code(pInstance, pXCArgs->ucHDMIInfoSource);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_ARC_PINCONTROL:
            {
                pstHDMI_ARC_PINCONTROL pXCArgs = (pstHDMI_ARC_PINCONTROL) pArgs;
                psXCInstPri->fpHDMI_ARC_PINControl(pInstance,
                                                   pXCArgs->enInputPortType,
                                                   pXCArgs->bEnable,
                                                   pXCArgs->bDrivingHigh);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_DVI_SOFTWARE_RESET:
            {
                pstDVI_SOFTWARE_RESET pXCArgs = (pstDVI_SOFTWARE_RESET) pArgs;
                psXCInstPri->fpDVI_Software_Reset(pInstance,
                                                  pXCArgs->enInputPortType,
                                                  pXCArgs->u16Reset);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_SET_POWERSTATE:
            {
                pstHDMI_SET_POWERSTATE pXCArgs = (pstHDMI_SET_POWERSTATE) pArgs;
                pXCArgs->u32ReturnValue = psXCInstPri->fpHDMI_Set_PowerState(pInstance, pXCArgs->enPowerState);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_DVI_HF_ADJUST:
            {
                psXCInstPri->fpHDMI_dvi_hf_adjust(pInstance);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_CHECK_HDMI20_SETTING:
            {
                pstHDMI_CHECK_HDMI20_SETTING pXCArgs = (pstHDMI_CHECK_HDMI20_SETTING) pArgs;
                pXCArgs->bReturnValue = psXCInstPri->fpHDMI_checkhdmi20_setting(pInstance, pXCArgs->enInputPortType);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_SET_5V_DETECT_GPIO_SELECT:
            {
                pstHDMI_SET_5V_DETECT_GPIO_SELECT pXCArgs = (pstHDMI_SET_5V_DETECT_GPIO_SELECT) pArgs;
                psXCInstPri->fpHDMI_Set5VDetectGPIOSelect(pInstance, pXCArgs->ul5VDetectGPIOSelect);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_GET_DE_STABLE_STATUS:
            {
                pstHDMI_GET_DE_STABLE_STATUS pXCArgs = (pstHDMI_GET_DE_STABLE_STATUS) pArgs;
                pXCArgs->bStatus = psXCInstPri->fpHDMI_GetDEStableStatus(pInstance, pXCArgs->enInputPortType);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_CTRL:
            {
                pstHDMI_CONTROL pXCArgs = (pstHDMI_CONTROL)pArgs;
                pXCArgs->bRet = psXCInstPri->fpHDMI_Ctrl(pInstance, pXCArgs->ucHDMIInfoSource, pXCArgs->u32Cmd, pXCArgs->pBuf, pXCArgs->u32BufSize);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_XC_HDMI_IF_CMD_GET_DATA_INFO:
            {
                pstHDMI_GET_DATA_INFO pXCArgs = (pstHDMI_GET_DATA_INFO)pArgs;
                pXCArgs->usReturnValue = psXCInstPri->fpHDMI_GetDataInfo(pInstance, pXCArgs->enInfo, pXCArgs->ucHDMIInfoSource);

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_XC_HDMI_IF_CMD_NONE:
        case E_XC_HDMI_IF_CMD_MAX:
        default:
            {
                MAPI_HDMIRX_MSG_WARNING("Command %td is not existed\n",(ptrdiff_t)u32Cmd);
                u32Return = UTOPIA_STATUS_ERR_INVALID_HANDLE;
                break;
            }
    }
    return u32Return;
}

//**************************************************************************
//  [Function Name]:
//                  HDMIRxOpen()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 HDMIRxOpen(void** ppInstance, const void* const pAttribute)
{
    HDMI_RX_INSTANT_PRIVATE *psHDMIRxInstPri = NULL;

    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));
    // instance is allocated here, also can allocate private for internal use, ex, BDMA_INSTANT_PRIVATE
    UtopiaInstanceCreate(sizeof(HDMI_RX_INSTANT_PRIVATE), ppInstance);
    // setup func in private and assign the calling func in func ptr in instance private
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psHDMIRxInstPri);

    psHDMIRxInstPri->fpHDMIRxInitial = (IOCTL_HDMIRX_INITIAL)mdrv_HDMI_InitialSetting;
    psHDMIRxInstPri->fpHDMIRxStablePolling = (IOCTL_HDMIRX_STABLE_POLLING)mdrv_HDMI_StablePolling;
    psHDMIRxInstPri->fpHDMIRxGetSourceVersion = (IOCTL_HDMIRX_GET_SOURCE_VERSION)mdrv_HDMI_GetSourceVersion;
    psHDMIRxInstPri->fpHDMIRxCheckHDCPState = (IOCTL_HDMIRX_CHECK_HDCP_STATE)mdrv_HDMI_CheckHDCPState;
    psHDMIRxInstPri->fpHDMIRxCheckHDCPENCState = (IOCTL_HDMIRX_CHECK_HDCP_ENC_STATE)mdrv_HDMI_CheckHDCPENCState;
    psHDMIRxInstPri->fpHDMIRxCheckHDCP14KeyVaild = (IOCTL_HDMIRX_CHECK_HDCP14_KEY_VAILD)mdrv_HDMI_CheckHDCP14KeyVaild;
    psHDMIRxInstPri->fpHDMIRxGetHDCP14KeyVaildFlag = (IOCTL_HDMIRX_GET_HDCP14_KEY_VAILD_FLAG)mdrv_HDMI_GetHDCP14KeyVaildFlag;
    psHDMIRxInstPri->fpHDMIRxSetHPDInvertFlag = (IOCTL_HDMIRX_SET_HPD_INVERT_FLAG)mdrv_HDMI_SetHPDInvertFlag;
    psHDMIRxInstPri->fpHDMIRxGetPacketReceive = (IOCTL_HDMIRX_GET_PACKET_RECEIVE)mdrv_HDMI_GetPacketReceiveStatus;
    /*************************** HDCP Repeater ***************************/
    psHDMIRxInstPri->fpHDMIRxWriteX74 = (IOCTL_HDMIRX_WRITE_X74)mdrv_HDCP_WriteX74;
    psHDMIRxInstPri->fpHDMIRxReadX74 = (IOCTL_HDMIRX_READ_X74)mdrv_HDCP_ReadX74;
    psHDMIRxInstPri->fpHDMIRxSetRepeater = (IOCTL_HDMIRX_SET_REPEATER)mdrv_HDCP_SetRepeater;
    psHDMIRxInstPri->fpHDMIRxSetBstatus = (IOCTL_HDMIRX_SET_BSTATUS)mdrv_HDCP_SetBstatus;
    psHDMIRxInstPri->fpHDMIRxSetHDMIMode = (IOCTL_HDMIRX_SET_HDMI_MODE)mdrv_HDCP_SetHDMIMode;
    psHDMIRxInstPri->fpHDMIRxGetInterruptStatus = (IOCTL_HDMIRX_GET_INTERRUPT_STATUS)mdrv_HDCP_GetInterruptStatus;
    psHDMIRxInstPri->fpHDMIRxWriteKSVList = (IOCTL_HDMIRX_WRITE_KSV_LIST)mdrv_HDCP_WriteKSVList;
    psHDMIRxInstPri->fpHDMIRxSetVPrime = (IOCTL_HDMIRX_SET_VPRIME)mdrv_HDCP_SetVPrime;
    /*************************** HDCP Repeater ***************************/
    psHDMIRxInstPri->fpHDMIRxFillCipherKey = (IOCTL_HDMIRX_FILL_CIPHER_KEY)mdrv_HDCP22_FillCipherKey;
    psHDMIRxInstPri->fpHDMIRxPortInit = (IOCTL_HDMIRX_PORT_INIT)mdrv_HDCP22_PortInit;
    psHDMIRxInstPri->fpHDMIRxPollingReadDone = (IOCTL_HDMIRX_POLLING_READ_DONE)mdrv_HDCP22_PollingReadDone;
    psHDMIRxInstPri->fpHDMIRxEnableCipher = (IOCTL_HDMIRX_ENABLE_CIPHER)mdrv_HDCP22_EnableCipher;
    psHDMIRxInstPri->fpHDMIRxSendMessage = (IOCTL_HDMIRX_SEND_MESSAGE)mdrv_HDCP22_SendMessage;
    psHDMIRxInstPri->fpHDMIRxHDCP22Handler = (IOCTL_HDMIRX_HDCP22_HANDER)mdrv_HDCP22_Handler;
    psHDMIRxInstPri->fpHDMIRxHDCP22WaitEvent = (IOCTL_HDMIRX_HDCP22_WAIT_EVENT)mdrv_HDCP22_WaitEvent;

    return UTOPIA_STATUS_SUCCESS;
}

//**************************************************************************
//  [Function Name]:
//                  HDMIRxIoctl()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 HDMIRxIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
    MS_U32 ulReturnValue = UTOPIA_STATUS_SUCCESS;
    HDMI_RX_INSTANT_PRIVATE* psHDMIRxInstPri = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);
    UtopiaInstanceGetPrivate(pInstance, (void**)&psHDMIRxInstPri);

    switch(u32Cmd)
    {
        case MAPI_CMD_HDMIRX_INITIAL:
            {
                pstHDMI_INITIAL_SETTING pHDMIRxArgs = (pstHDMI_INITIAL_SETTING)pArgs;
                psHDMIRxInstPri->fpHDMIRxInitial(pInstance, pHDMIRxArgs->stInitialTable);
            }
            break;

        case MAPI_CMD_HDMIRX_STABLE_POLLING:
            psHDMIRxInstPri->fpHDMIRxStablePolling(pInstance);
            break;

        case MAPI_CMD_HDMIRX_GET_SOURCE_VERSION:
            {
                pstHDMI_GET_SOURCE_VERSION pHDMIRxArgs = (pstHDMI_GET_SOURCE_VERSION)pArgs;
                pHDMIRxArgs->ucSourceVersion = psHDMIRxInstPri->fpHDMIRxGetSourceVersion(pInstance, pHDMIRxArgs->enInputPortType);
            }
            break;

        case MAPI_CMD_HDMIRX_CHECK_HDCP_STATE:
            {
                pstHDMI_CHECK_HDCP_STATE pHDMIRxArgs = (pstHDMI_CHECK_HDCP_STATE)pArgs;
                pHDMIRxArgs->ucHDCPState = psHDMIRxInstPri->fpHDMIRxCheckHDCPState(pInstance, pHDMIRxArgs->enInputPortType);
            }
            break;

        case MAPI_CMD_HDMIRX_CHECK_HDCP_ENC_STATE:
            {
                pstHDMI_CHECK_HDCP_ENC_STATE pHDMIRxArgs = (pstHDMI_CHECK_HDCP_ENC_STATE)pArgs;
                pHDMIRxArgs->ucHDCPENCState = psHDMIRxInstPri->fpHDMIRxCheckHDCPENCState(pInstance, pHDMIRxArgs->enInputPortType);
            }
            break;

        case MAPI_CMD_HDMIRX_CHECK_HDCP14_KEY:
            {
                pstHDCP_INITPRODUCTIONKEY pHDMIRxArgs = (pstHDCP_INITPRODUCTIONKEY)pArgs;
                psHDMIRxInstPri->fpHDMIRxCheckHDCP14KeyVaild(pInstance, pHDMIRxArgs->pu8HdcpKeyData);
            }
            break;

        case MAPI_CMD_HDMIRX_GET_HDCP14_KEY_VAILD:
            {
                pstHDMI_GET_HDCP14_KEY_VAILD pHDMIRxArgs = (pstHDMI_GET_HDCP14_KEY_VAILD)pArgs;
                pHDMIRxArgs->bHDCP14KeyVaildFlag = psHDMIRxInstPri->fpHDMIRxGetHDCP14KeyVaildFlag(pInstance);
            }
            break;

        case MAPI_CMD_HDMIRX_SET_HPD_INVERT_FLAG:
            {
                pstHDMI_SET_HPD_INVERT_FLAG pHDMIRxArgs = (pstHDMI_SET_HPD_INVERT_FLAG)pArgs;
                psHDMIRxInstPri->fpHDMIRxSetHPDInvertFlag(pInstance, pHDMIRxArgs->enInputPortType, pHDMIRxArgs->bInverse);
            }
            break;

        case MAPI_CMD_HDMIRX_GET_PACKET_RECEIVE:
            {
                pstHDMI_PACKET_RECEIVED pHDMIRxArgs = (pstHDMI_PACKET_RECEIVED)pArgs;
                pHDMIRxArgs->ulPacketStatus = psHDMIRxInstPri->fpHDMIRxGetPacketReceive(pInstance, pHDMIRxArgs->ucHDMIInfoSource);
            }
            break;

        /*************************** HDCP Repeater ***************************/
        case MAPI_CMD_HDMIRX_WRITE_X74:
            {
                pstHDCP_WRITE_X74 pHDMIRxArgs = (pstHDCP_WRITE_X74)pArgs;
                psHDMIRxInstPri->fpHDMIRxWriteX74(pInstance, pHDMIRxArgs->enInputPortType, pHDMIRxArgs->ucOffset, pHDMIRxArgs->ucData);
            }
            break;

        case MAPI_CMD_HDMIRX_READ_X74:
            {
                pstHDCP_READ_X74 pHDMIRxArgs = (pstHDCP_READ_X74)pArgs;
                pHDMIRxArgs->ucRetData = psHDMIRxInstPri->fpHDMIRxReadX74(pInstance, pHDMIRxArgs->enInputPortType, pHDMIRxArgs->ucOffset);
            }
            break;

        case MAPI_CMD_HDMIRX_SET_REPEATER:
            {
                pstHDCP_SET_REPEATER pHDMIRxArgs = (pstHDCP_SET_REPEATER)pArgs;
                psHDMIRxInstPri->fpHDMIRxSetRepeater(pInstance, pHDMIRxArgs->enInputPortType, pHDMIRxArgs->bIsRepeater);
            }
            break;

        case MAPI_CMD_HDMIRX_SET_BSTATUS:
            {
                pstHDCP_SET_BSTATUS pHDMIRxArgs = (pstHDCP_SET_BSTATUS)pArgs;
                psHDMIRxInstPri->fpHDMIRxSetBstatus(pInstance, pHDMIRxArgs->enInputPortType, pHDMIRxArgs->usBstatus);
            }
            break;

        case MAPI_CMD_HDMIRX_SET_HDMI_MODE:
            {
                pstHDCP_SET_HDMI_MODE pHDMIRxArgs = (pstHDCP_SET_HDMI_MODE)pArgs;
                psHDMIRxInstPri->fpHDMIRxSetHDMIMode(pInstance, pHDMIRxArgs->enInputPortType, pHDMIRxArgs->bHDMIMode);
            }
            break;

        case MAPI_CMD_HDMIRX_GET_INTERRUPT_STATUS:
            {
                pstHDCP_GET_INTERRUPT_STATUS pHDMIRxArgs = (pstHDCP_GET_INTERRUPT_STATUS)pArgs;
                pHDMIRxArgs->ucRetIntStatus = psHDMIRxInstPri->fpHDMIRxGetInterruptStatus(pInstance, pHDMIRxArgs->enInputPortType);
            }
            break;

        case MAPI_CMD_HDMIRX_WRITE_KSV_LIST:
            {
                pstHDCP_WRITE_KSV_LIST pHDMIRxArgs = (pstHDCP_WRITE_KSV_LIST)pArgs;
                psHDMIRxInstPri->fpHDMIRxWriteKSVList(pInstance, pHDMIRxArgs->enInputPortType, pHDMIRxArgs->pucKSV, pHDMIRxArgs->ulLen);
            }
            break;

        case MAPI_CMD_HDMIRX_SET_VPRIME:
            {
                pstHDCP_SET_VPRIME pHDMIRxArgs = (pstHDCP_SET_VPRIME)pArgs;
                psHDMIRxInstPri->fpHDMIRxSetVPrime(pInstance, pHDMIRxArgs->enInputPortType, pHDMIRxArgs->pucVPrime);
            }
            break;
        /*************************** HDCP Repeater ***************************/

        case MAPI_CMD_HDMIRX_HDCP22_FILL_CIPHER_KEY:
            {
                pstHDCP22_FILL_CIPHER_KEY pHDMIRxArgs = (pstHDCP22_FILL_CIPHER_KEY)pArgs;
                psHDMIRxInstPri->fpHDMIRxFillCipherKey(pInstance, pHDMIRxArgs->ucPortIdx, pHDMIRxArgs->pucRiv, pHDMIRxArgs->pucSessionKey);
            }
            break;

        case MAPI_CMD_HDMIRX_HDCP22_PORT_INIT:
            {
                pstHDCP22_PORT_INIT pHDMIRxArgs = (pstHDCP22_PORT_INIT)pArgs;
                psHDMIRxInstPri->fpHDMIRxPortInit(pInstance, pHDMIRxArgs->ucPortIdx);
            }
            break;

        case MAPI_CMD_HDMIRX_HDCP22_POLLING_READ_DONE:
            {
                pstHDCP22_POLLING_READ_DONE pHDMIRxArgs = (pstHDCP22_POLLING_READ_DONE)pArgs;
                pHDMIRxArgs->bReturnValue = psHDMIRxInstPri->fpHDMIRxPollingReadDone(pInstance, pHDMIRxArgs->ucPortIdx);
            }
            break;

        case MAPI_CMD_HDMIRX_HDCP22_ENABLE_CIPHER:
            {
                pstHDCP22_ENABLE_CIPHER pHDMIRxArgs = (pstHDCP22_ENABLE_CIPHER)pArgs;
                psHDMIRxInstPri->fpHDMIRxEnableCipher(pInstance, pHDMIRxArgs->ucPortType, pHDMIRxArgs->ucPortIdx, pHDMIRxArgs->bIsEnable);
            }
            break;

        case MAPI_CMD_HDMIRX_HDCP22_SEND_MESSAGE:
            {
                pstHDCP22_SEND_MESSAGE pHDMIRxArgs = (pstHDCP22_SEND_MESSAGE)pArgs;
                psHDMIRxInstPri->fpHDMIRxSendMessage(pInstance, pHDMIRxArgs->ucPortType, pHDMIRxArgs->ucPortIdx, pHDMIRxArgs->pucData, pHDMIRxArgs->dwDataLen, pHDMIRxArgs->pDummy);
            }
            break;

        case MAPI_CMD_HDMIRX_HDCP22_HANDLER:
            {
                pstHDCP22_HANDLER pHDMIRxArgs = (pstHDCP22_HANDLER)pArgs;
                pHDMIRxArgs->bHdcp22RecvMsgFlag = psHDMIRxInstPri->fpHDMIRxHDCP22Handler(pInstance, pHDMIRxArgs->ucPortIdx, &pHDMIRxArgs->ucHdcp22PortType, pHDMIRxArgs->pucHdcp22MsgData, &pHDMIRxArgs->ucHdcp22MsgLen, pHDMIRxArgs->bIRQModeFlag);
            }
            break;

        case MAPI_CMD_HDMIRX_HDCP22_WAIT_EVENT:
            {
                pstHDCP22_WAIT_EVENT pHDMIRxArgs = (pstHDCP22_WAIT_EVENT)pArgs;
                pHDMIRxArgs->ucHDCPWriteDoneIndex = psHDMIRxInstPri->fpHDMIRxHDCP22WaitEvent(pInstance);
            }
            break;

        default:
            ulReturnValue = UTOPIA_STATUS_FAIL;
            break;
    };

    return ulReturnValue;
}

//**************************************************************************
//  [Function Name]:
//                  HDMIRxClose()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 HDMIRxClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);

    return TRUE;
}

//**************************************************************************
//  [Function Name]:
//                  HDMIRxSTR()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 HDMIRxSTR(MS_U32 ulPowerState, void* pModule)
{
    MS_U32 ulReturnValue = UTOPIA_STATUS_FAIL;

    ulReturnValue = mdrv_HDMI_STREventProc(pModule, (EN_POWER_MODE)ulPowerState);

    return ulReturnValue;
}

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
//**************************************************************************
//  [Function Name]:
//                  HDMIRxMdbIoctl()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 HDMIRxMdbIoctl(MS_U32 cmd, const void* const pArgs)
{
    void* pInstance = NULL;
    MS_U32 ulReturnValue = UTOPIA_STATUS_SUCCESS;
    MDBCMD_CMDLINE_PARAMETER *paraCmdLine;
    MDBCMD_GETINFO_PARAMETER *paraGetInfo;

    pInstance = UtopiaModuleGetLocalInstantList(MODULE_HDMIRX, pInstance);

    switch(cmd)
    {
        case MDBCMD_CMDLINE:
            paraCmdLine = (MDBCMD_CMDLINE_PARAMETER *)pArgs;
            MdbPrint(paraCmdLine->u64ReqHdl,"u32CmdSize: %d\n", paraCmdLine->u32CmdSize);
            mdrv_HDMI_MDCMDEchoCommand(pInstance, paraCmdLine->u64ReqHdl, paraCmdLine->pcCmdLine);
            paraCmdLine->result = MDBRESULT_SUCCESS_FIN;
            break;

        case MDBCMD_GETINFO:
            paraGetInfo = (MDBCMD_GETINFO_PARAMETER *)pArgs;
            mdrv_HDMI_MDCMDGetInfo(pInstance, paraGetInfo->u64ReqHdl);
            paraGetInfo->result = MDBRESULT_SUCCESS_FIN;
            break;

        default:
            paraGetInfo = (MDBCMD_GETINFO_PARAMETER *)pArgs;
            MdbPrint(paraGetInfo->u64ReqHdl,"unknown cmd\n", __LINE__);
            break;
    };

    return ulReturnValue;
}
#endif

//**************************************************************************
//  [Function Name]:
//                  HDMIRXRegisterToUtopia()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void HDMIRXRegisterToUtopia(FUtopiaOpen ModuleType)
{
    void* pUtopiaModule = NULL;
    void* psResource = NULL;

    // 1. deal with module
    UtopiaModuleCreate(MODULE_HDMIRX, 0, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)HDMIRxOpen, (FUtopiaClose)HDMIRxClose, (FUtopiaIOctl)HDMIRxIoctl);

#if(defined(MSOS_TYPE_LINUX_KERNEL))
    UtopiaModuleSetupSTRFunctionPtr(pUtopiaModule, (FUtopiaSTR)HDMIRxSTR);
#endif

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
    UtopiaModuleRegisterMdbNode("HDMIRX", (FUtopiaMdbIoctl)HDMIRxMdbIoctl);
#endif

    // 2. deal with resource
    // start func to add res, call once will create 2 access in resource. Also can declare BDMA_POOL_ID_BDMA1 for another channel depend on driver owner.
    UtopiaModuleAddResourceStart(pUtopiaModule, HDMI_RX_POOL);
    // resource can alloc private for internal use, ex, BDMA_RESOURCE_PRIVATE
    UtopiaResourceCreate("HDMIRX", sizeof(HDMI_RX_RESOURCE_PRIVATE), &psResource);
    // func to reg res
    UtopiaResourceRegister(pUtopiaModule, psResource, HDMI_RX_POOL);

    UtopiaModuleAddResourceEnd(pUtopiaModule, HDMI_RX_POOL);
}

#endif // _API_HDMI_RX_V2_C_

