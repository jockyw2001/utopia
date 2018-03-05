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
/// file   apiHDMITx_v2.c
/// @brief  Demux 1.0 API wrapper for Demux 2.0
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MsTypes.h"
#include "MsCommon.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <string.h>
#endif
#include "utopia_dapi.h"
#include "apiHDMITx.h"
#include "drvHDMITx.h"
#include "HDMITX_private.h"
#include "apiHDMITx_v2.h"
#include "utopia.h"


/*********************************************************************/
/*                                                                                                                     */
/*                                                      Defines                                                    */
/*                                                                                                                     */
/*********************************************************************/
#define EN_HDMITX_V2_DBG                (0)

#if (EN_HDMITX_V2_DBG == 1)
#define DBG_HDMITX_V2(_f)                  _f
#else
#define DBG_HDMITX_V2(_f)
#endif

void* pModuleHDMITX = NULL;

void HDMITXRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    printf("HDMITxRegisterToUtopia..................\r\n");
    UtopiaModuleCreate(MODULE_HDMITX, 0, &pModuleHDMITX);
    UtopiaModuleRegister(pModuleHDMITX);

    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pModuleHDMITX,
                                    (FUtopiaOpen)HDMITXOpen,
                                    (FUtopiaClose)HDMITXClose,
                                    (FUtopiaIOctl)HDMITXIoctl);

    #ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
    UtopiaModuleRegisterMdbNode("HDMITX", (FUtopiaMdbIoctl)HDMITXMdbIoctl);
    #endif

    // 2. deal with resource
    void* psResource = NULL;

    UtopiaModuleAddResourceStart(pModuleHDMITX, E_HDMITX_POOL_ID_INTERNAL_0);
    UtopiaResourceCreate("HDMITX", sizeof(HDMITX_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pModuleHDMITX, psResource, E_HDMITX_POOL_ID_INTERNAL_0);
    UtopiaModuleAddResourceEnd(pModuleHDMITX, E_HDMITX_POOL_ID_INTERNAL_0);
    DBG_HDMITX_V2(printf("%s(): end\n", __FUNCTION__));

}

MS_U32 HDMITXOpen(void** ppInstance, const void* const pAttribute)
{
    HDMITX_INSTANCE_PRIVATE* psHDMITXInstPri = NULL;
    DBG_HDMITX_V2(printf("%s(): start...\n", __FUNCTION__));

    //Create instance
    UtopiaInstanceCreate(sizeof(HDMITX_INSTANCE_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psHDMITXInstPri);

#ifdef UTOPIA_20
    psHDMITXInstPri->fpHDMITX_INIT = MApi_HDMITx_Init_U2;
    psHDMITXInstPri->fpHDMITX_EXIT = MApi_HDMITx_Exit_U2;
    psHDMITXInstPri->fpHDMITX_TUNRONOFF = MApi_HDMITx_TurnOnOff_U2;
    psHDMITXInstPri->fpHDMITX_ENABLEPACKETGEN = MApi_HDMITx_EnablePacketGen_U2;
	psHDMITXInstPri->fpHDMITX_SETHDMITXMODE = MApi_HDMITx_SetHDMITxMode_U2;
    psHDMITXInstPri->fpHDMITX_SETHDMITXMODE_CD = MApi_HDMITx_SetHDMITxMode_CD_U2;
    psHDMITXInstPri->fpHDMITX_SETTMDSONOFF = MApi_HDMITx_SetTMDSOnOff_U2;
    psHDMITXInstPri->fpHDMITX_DISABLETMDSCTRL = MApi_HDMITx_DisableTMDSCtrl_U2;
    psHDMITXInstPri->fpHDMITX_SETRBCHANNELSWAP = MApi_HDMITx_SetRBChannelSwap_U2;
    psHDMITXInstPri->fpHDMITX_EXHIBIT = MApi_HDMITx_Exhibit_U2;

    psHDMITXInstPri->fpHDMITX_GETRXSTATUS = MApi_HDMITx_GetRxStatus_U2;
    psHDMITXInstPri->fpHDMITX_GETRXDCINFOFROMEDID = MApi_HDMITx_GetRxDCInfoFromEDID_U2;
    psHDMITXInstPri->fpHDMITX_GETRXVIDEOFORMATFROMEDID = MApi_HDMITx_GetRxVideoFormatFromEDID_U2;
    psHDMITXInstPri->fpHDMITX_GETVICLISTFROMEDID = MApi_HDMITx_GetVICListFromEDID_U2;
    psHDMITXInstPri->fpHDMITX_GETCOLORFORMATFROMEDID = MApi_HDMITx_GetColorFormatFromEDID_U2;
    psHDMITXInstPri->fpHDMITX_GETDATABLOCKLENGTHFROMEDID = MApi_HDMITx_GetDataBlockLengthFromEDID_U2;
    psHDMITXInstPri->fpHDMITX_GETRXAUDIOFORMATFROMEDID = MApi_HDMITx_GetRxAudioFormatFromEDID_U2;
    psHDMITXInstPri->fpHDMITX_EDID_HDMISUPPORT = MApi_HDMITx_EDID_HDMISupport_U2;
    psHDMITXInstPri->fpHDMITX_GETRXIDMANUFACTURERNAME = MApi_HDMITx_GetRxIDManufacturerName_U2;
    psHDMITXInstPri->fpHDMITX_GETEDIDDATA = MApi_HDMITx_GetEDIDData_U2;
    psHDMITXInstPri->fpHDMITX_GETRX3DSTRUCTUREFROMEDID = MApi_HDMITx_GetRx3DStructureFromEDID_U2;
    psHDMITXInstPri->fpHDMITX_PKT_USER_DEFINE_CLEAR = MApi_HDMITx_PKT_User_Define_Clear_U2;

    psHDMITXInstPri->fpHDMITX_PKT_USER_DEFINE = MApi_HDMITx_PKT_User_Define_U2;
    psHDMITXInstPri->fpHDMITX_PKT_CONTENT_DEFINE = MApi_HDMITx_PKT_Content_Define_U2;
    psHDMITXInstPri->fpHDMITX_SETVIDEOONOFF = MApi_HDMITx_SetVideoOnOff_U2;
    psHDMITXInstPri->fpHDMITX_SETCOLORFORMAT = MApi_HDMITx_SetColorFormat_U2;
    psHDMITXInstPri->fpHDMITX_SET_VS_INFOFRAME = MApi_HDMITx_Set_VS_InfoFrame_U2;
    psHDMITXInstPri->fpHDMITX_SETVIDEOOUTPUTTIMING = MApi_HDMITx_SetVideoOutputTiming_U2;
    psHDMITXInstPri->fpHDMITX_SETVIDEOOUTPUTASEPECTRATIO = MApi_HDMITx_SetVideoOutputAsepctRatio_U2;
    psHDMITXInstPri->fpHDMITX_SETVIDEOOUTPUTOVERSCAN_AFD = MApi_HDMITx_SetVideoOutputOverscan_AFD_U2;
    psHDMITXInstPri->fpHDMITX_SETAUDIOONOFF = MApi_HDMITx_SetAudioOnOff_U2;
    psHDMITXInstPri->fpHDMITX_SETAUDIOFREQUENCY = MApi_HDMITx_SetAudioFrequency_U2;

    psHDMITXInstPri->fpHDMITX_SETVIDEOOUTPUTOVERSCAN_AFD_II = MApi_HDMITx_SetVideoOutputOverscan_AFD_II_U02;
    psHDMITXInstPri->fpHDMITX_SETAUDIOCONFIGURATION = MApi_HDMITx_SetAudioConfiguration_U2;
    psHDMITXInstPri->fpHDMITX_SETAUDIOSOURCEFORMAT = MApi_HDMITx_SetAudioSourceFormat_U2;
    psHDMITXInstPri->fpHDMITX_SETAVIINFOCOLORIMETRY = MApi_HDMITx_SetAVIInfoExtColorimetry_U2;
    psHDMITXInstPri->fpHDMITX_GETAUDIOCTS = MApi_HDMITx_GetAudioCTS_U2;
    psHDMITXInstPri->fpHDMITX_MUTEAUDIOFIFO = MApi_HDMITx_MuteAudioFIFO_U2;
    psHDMITXInstPri->fpHDMITX_GETHDCPKEY = MApi_HDMITx_GetHdcpKey_U2;
    psHDMITXInstPri->fpHDMITX_GETBKSV = MApi_HDMITx_GetBksv_U2;
    psHDMITXInstPri->fpHDMITX_GETAKSV = MApi_HDMITx_GetAksv_U2;
    psHDMITXInstPri->fpHDMITX_SETHDCPONOFF = MApi_HDMITx_SetHDCPOnOff_U2;
    psHDMITXInstPri->fpHDMITX_SETAVMUTE = MApi_HDMITx_SetAVMUTE_U2;

    psHDMITXInstPri->fpHDMITX_GETAVMUTESTATUS = MApi_HDMITx_GetAVMUTEStatus_U2;
    psHDMITXInstPri->fpHDMITX_HDCP_REVOCATIONKEY_LIST = MApi_HDMITx_HDCP_RevocationKey_List_U2;
    psHDMITXInstPri->fpHDMITX_HDCP_REVOCATIONKEY_CHECK = MApi_HDMITx_HDCP_RevocationKey_Check_U2;
    psHDMITXInstPri->fpHDMITX_HDCP_ISSRMSIGNATUREVALID = MApi_HDMITx_HDCP_IsSRMSignatureValid_U2;
    psHDMITXInstPri->fpHDMITX_GETHDCPSTATUS = MApi_HDMITx_GetHDCPStatus_U2;
    psHDMITXInstPri->fpHDMITX_HDCP_STARTAUTH = MApi_HDMITx_HDCP_StartAuth_U2;
    psHDMITXInstPri->fpHDMITX_GETINTHDCPSTATUS = MApi_HDMITx_GetINTHDCPStatus_U2;
    psHDMITXInstPri->fpHDMITX_GETHDCP_PRESTATUS = MApi_HDMITx_GetHDCP_PreStatus_U2;
    psHDMITXInstPri->fpHDMITX_UNHDCPRXCONTROL = MApi_HDMITx_UnHDCPRxControl_U2;
	psHDMITXInstPri->fpHDMITX_HDCPRXFAILCONTROL = MApi_HDMITx_HDCPRxFailControl_U2;

    psHDMITXInstPri->fpHDMITX_GETLIBVER = MApi_HDMITx_GetLibVer_U2;
	psHDMITXInstPri->fpHDMITX_GETINFO = MApi_HDMITx_GetInfo_U2;
    psHDMITXInstPri->fpHDMITX_GETSTATUS = MApi_HDMITx_GetStatus_U2;
    psHDMITXInstPri->fpHDMITX_SETDBGLEVEL = MApi_HDMITx_SetDbgLevel_U2;
    psHDMITXInstPri->fpHDMITX_SETHPDGPIOPIN = MApi_HDMITx_SetHPDGpioPin_U2;
    psHDMITXInstPri->fpHDMITX_ANALOGTUNING = MApi_HDMITx_AnalogTuning_U2;
    psHDMITXInstPri->fpHDMITX_FORCEHDMIOUTPUTMODE = MApi_HDMITx_ForceHDMIOutputMode_U2;
    psHDMITXInstPri->fpHDMITX_FORCEHDMIOUTPUTCOLORFORMAT = MApi_HDMITx_ForceHDMIOutputColorFormat_U2;
    psHDMITXInstPri->fpHDMITX_DISABLEREGWRITE = MApi_HDMITx_DisableRegWrite_U2;
    psHDMITXInstPri->fpHDMITX_GETEDIDPHYADR = MApi_HDMITx_GetEDIDPhyAdr_U2;

    psHDMITXInstPri->fpHDMITX_SETCECONOFF = MApi_HDMITx_SetCECOnOff_U2;
    psHDMITXInstPri->fpHDMITX_GETCECSTATUS = MApi_HDMITx_GetCECStatus_U2;
    psHDMITXInstPri->fpHDMITX_EDIDCHECKING = MApi_HDMITx_EdidChecking_U2;
    psHDMITXInstPri->fpHDMITX_RXBYPASS_MODE = MApi_HDMITx_RxBypass_Mode_U2;
    psHDMITXInstPri->fpHDMITX_DISABLE_RXBYPASS = MApi_HDMITx_Disable_RxBypass_U2;
    psHDMITXInstPri->fpHDMITX_SETAKSV2R0INTERVAL = MApi_HDMITx_SetAksv2R0Interval_U2;
    psHDMITXInstPri->fpHDMITX_ISRXVALID = MApi_HDMITx_IsHDCPRxValid_U2;
    psHDMITXInstPri->fpHDMITX_GETCHIPCAPS = MApi_HDMITx_GetChipCaps_U2;
    psHDMITXInstPri->fpHDMITX_SETPOWERSTATE = MApi_HDMITx_SetPowerState_U2;
    psHDMITXInstPri->fpHDMITX_GETEDIDDATABLOCK = MApi_HDMITx_GetEdidDataBlocks_U2;
    psHDMITXInstPri->fpHDMITX_GETKSVLIST = MApi_HDMITx_GetKSVList_U2;
    psHDMITXInstPri->fpHDMITX_HDCP2ACCESSX74OFFSET = MApi_HDMITx_HDCP2AccessX74_U2;
    psHDMITXInstPri->fpHDMITx_HDCP2TxInit = MApi_HDMITx_HDCP2TxInit_U2;
    psHDMITXInstPri->fpHDMITx_HDCP2TxEnableEncrypt = MApi_HDMITx_HDCP2TxEnableEncrypt_U2;
    psHDMITXInstPri->fpHDMITx_HDCP2TxFillCipherKey = MApi_HDMITx_HDCP2TxFillCipherKey_U2;
    psHDMITXInstPri->fpHDMITx_GeneralCtrl = MApi_HDMITx_GeneralCtrl_U2;
    psHDMITXInstPri->fpHDMITx_Cmd_ColorAndRangeTransform = MApi_HDMITx_ColorandRange_Transform_U2;
    psHDMITXInstPri->fpHDMITx_Cmd_SSCEnable = MApi_HDMITx_SSC_Enable_U2;
    psHDMITXInstPri->fpHDMITx_Cmd_Set_Colorimetry = MApi_HDMITx_SetAVIInfoColorimetry_U2;
    psHDMITXInstPri->fpHDMITx_Cmd_Get_Full_Rx_Status = MApi_HDMITx_GetFullRxStatus_U2;
    psHDMITXInstPri->fpHDMITx_Cmd_Timing_Capability_Check = MApi_HDMITx_CheckLegalTiming_U2;
    psHDMITXInstPri->fpHDMITx_Cmd_Get_TMDS_Status = MApi_HDMITx_GetTMDSStatus_U2;
    psHDMITXInstPri->fpHDMITX_SETVIDEOOUTPUTTIMINGBYCUSTOMER = MApi_HDMITx_SetVideoOutputTimingByCustomer_U2;
#if defined(VANCLEEF_MCP) || defined(RAPTORS_MCP)
    psHDMITXInstPri->fpHDMITX_VANCLEEF_LOADHDCPKEY = MApi_HDMITx_Vancleef_LoadHDCPKey_U2;
#endif
#endif
    return UTOPIA_STATUS_SUCCESS;
}


#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT//**************************************************************************//  [Function Name]://                  HDMITXMdbIoctl()
//  [Description]//                  //  [Arguments]:////  [Return]:////**************************************************************************MS_U32 HDMITXMdbIoctl(MS_U32 cmd, const void* const pArgs)
{    void* pInstance = NULL;    MS_U32 ulReturnValue = UTOPIA_STATUS_SUCCESS;    MDBCMD_CMDLINE_PARAMETER *paraCmdLine;    MDBCMD_GETINFO_PARAMETER *paraGetInfo;    pInstance = UtopiaModuleGetLocalInstantList(MODULE_HDMITX, pInstance);
    switch(cmd)    {        case MDBCMD_CMDLINE:            paraCmdLine = (MDBCMD_CMDLINE_PARAMETER *)pArgs;            MdbPrint(paraCmdLine->u64ReqHdl,"u32CmdSize: %d\n", paraCmdLine->u32CmdSize);            MDrv_HDMITx_MDCMDEchoCommand(pInstance, paraCmdLine->u64ReqHdl, paraCmdLine->pcCmdLine);
            paraCmdLine->result = MDBRESULT_SUCCESS_FIN;            break;        case MDBCMD_GETINFO:            paraGetInfo = (MDBCMD_GETINFO_PARAMETER *)pArgs;            MDrv_HDMITx_MDCMDGetInfo(pInstance, paraGetInfo->u64ReqHdl);
            paraGetInfo->result = MDBRESULT_SUCCESS_FIN;            break;        default:            paraGetInfo = (MDBCMD_GETINFO_PARAMETER *)pArgs;            MdbPrint(paraGetInfo->u64ReqHdl,"unknown cmd\n", __LINE__);            break;    };    return ulReturnValue;}#endif

MS_U32 HDMITXIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
#ifdef UTOPIA_20
    void* pModule = NULL;
    HDMITX_INSTANCE_PRIVATE* psHDMITXInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&psHDMITXInstPri);
    UtopiaInstanceGetModule(pInstance, &pModule);
    //printf("[%s,%5d] u32Cmd type = %ld\n",__FUNCTION__,__LINE__,u32Cmd);

    switch(u32Cmd)
    {

        case E_HDMITX_CMD_INIT:
            {
                pstHDMITx_Init pHDMITXArgs = (pstHDMITx_Init)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_INIT(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_EXIT:
            {
                pstHDMITx_Exit pHDMITXArgs = (pstHDMITx_Exit)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_EXIT(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_HDMITX_CMD_TURNONOFF:
            {
                pstHDMITx_TurnOnOff pHDMITXArgs = (pstHDMITx_TurnOnOff)pArgs;
                psHDMITXInstPri->fpHDMITX_TUNRONOFF(pInstance,pHDMITXArgs->state);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_ENABLEPACKETGEN:
            {
                pstHDMITx_EnablePacketGen pHDMITXArgs = (pstHDMITx_EnablePacketGen)pArgs;
                psHDMITXInstPri->fpHDMITX_ENABLEPACKETGEN(pInstance,pHDMITXArgs->bflag);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETHDMITXMODE:
            {
                pstHDMITx_SetHDMITxMode pHDMITXArgs = (pstHDMITx_SetHDMITxMode)pArgs;
                psHDMITXInstPri->fpHDMITX_SETHDMITXMODE(pInstance, pHDMITXArgs->mode);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETHDMITXMODE_CD:
            {
                pstHDMITx_SetHDMITxMode_CD pHDMITXArgs = (pstHDMITx_SetHDMITxMode_CD)pArgs;
                psHDMITXInstPri->fpHDMITX_SETHDMITXMODE_CD(pInstance, pHDMITXArgs->mode, pHDMITXArgs->val);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETTMDSONOFF:
            {
                pstHDMITx_SetTMDSOnOff pHDMITXArgs = (pstHDMITx_SetTMDSOnOff)pArgs;
                psHDMITXInstPri->fpHDMITX_SETTMDSONOFF(pInstance, pHDMITXArgs->state);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_DISABLETMDSCTRL:
            {
                pstHDMITx_DisableTMDSCtrl pHDMITXArgs = (pstHDMITx_DisableTMDSCtrl)pArgs;
                psHDMITXInstPri->fpHDMITX_DISABLETMDSCTRL(pInstance, pHDMITXArgs->bFlag);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETRBCHANNELSWAP:
            {
                pstHDMITx_SetRBChannelSwap pHDMITXArgs = (pstHDMITx_SetRBChannelSwap)pArgs;
                psHDMITXInstPri->fpHDMITX_SETRBCHANNELSWAP(pInstance, pHDMITXArgs->state);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_EXHIBIT:
            {
                psHDMITXInstPri->fpHDMITX_EXHIBIT(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETRXSTATUS:
            {
                pstHDMITx_GetRxStatus pHDMITXArgs = (pstHDMITx_GetRxStatus)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_GETRXSTATUS();

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETRXDCINFOFROMEDID:
            {
                pstHDMITx_GetRxDCInfoFromEDID pHDMITXArgs = (pstHDMITx_GetRxDCInfoFromEDID)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_GETRXDCINFOFROMEDID(pInstance, pHDMITXArgs->val);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETRXVIDEODORMATFROMEDID:
            {
                pstHDMITx_GetRxVideoFormatFromEDID pHDMITXArgs = (pstHDMITx_GetRxVideoFormatFromEDID)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_GETRXVIDEOFORMATFROMEDID(pInstance, pHDMITXArgs->pu8Buffer, pHDMITXArgs->u8BufSize);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETVICLISTFROMEDID:
            {
                pstHDMITx_GetVICListFromEDID pHDMITXArgs = (pstHDMITx_GetVICListFromEDID)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_GETVICLISTFROMEDID(pInstance, pHDMITXArgs->pu8Buffer, pHDMITXArgs->u8BufSize);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETDATABLOCKLENGTHFROMEDID:
            {
                pstHDMITx_GetDataBlockLengthFromEDID pHDMITXArgs = (pstHDMITx_GetDataBlockLengthFromEDID)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_GETDATABLOCKLENGTHFROMEDID(pInstance, pHDMITXArgs->pu8Length, pHDMITXArgs->u8TagCode);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETCOLORFORMATFROMEDID:
            {
                pstHDMITx_GetColorFormatFromEDID pHDMITXArgs = (pstHDMITx_GetColorFormatFromEDID)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_GETCOLORFORMATFROMEDID(pInstance, pHDMITXArgs->timing, pHDMITXArgs->pColorFmt);

                break;
            }

        case E_HDMITX_CMD_GETRXAUDIOFORMATFROMEDID:
            {
                pstHDMITx_GetRxAudioFormatFromEDID pHDMITXArgs = (pstHDMITx_GetRxAudioFormatFromEDID)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_GETRXAUDIOFORMATFROMEDID(pInstance, pHDMITXArgs->pu8Buffer, pHDMITXArgs->u8BufSize);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_EDID_HDMISUPPORT:
            {
                pstHDMITx_EDID_HDMISupport pHDMITXArgs = (pstHDMITx_EDID_HDMISupport)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_EDID_HDMISUPPORT(pInstance, pHDMITXArgs->HDMI_Support);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETRXIDMANUFACTURERNAME:
            {
                pstHDMITx_GetRxIDManufacturerName pHDMITXArgs = (pstHDMITx_GetRxIDManufacturerName)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_GETRXIDMANUFACTURERNAME(pInstance, pHDMITXArgs->pu8Buffer);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETEDIDDATA:
            {
                pstHDMITx_GetEDIDData pHDMITXArgs = (pstHDMITx_GetEDIDData)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_GETEDIDDATA(pInstance, pHDMITXArgs->pu8Buffer, pHDMITXArgs->BlockIdx);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETRX3DSTRUCTUREFROMEDID:
            {
                pstHDMITx_GetRx3DStructureFromEDID pHDMITXArgs = (pstHDMITx_GetRx3DStructureFromEDID)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_GETRX3DSTRUCTUREFROMEDID(pInstance, pHDMITXArgs->timing, pHDMITXArgs->p3DStructure);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_PKT_USER_DEFINE_CLEAR:
            {
                psHDMITXInstPri->fpHDMITX_PKT_USER_DEFINE_CLEAR(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_PKT_USER_DEFINE:
            {
                pstHDMITx_PKT_User_Define pHDMITXArgs = (pstHDMITx_PKT_User_Define)pArgs;
                psHDMITXInstPri->fpHDMITX_PKT_USER_DEFINE(pInstance, pHDMITXArgs->packet_type, pHDMITXArgs->def_flag, pHDMITXArgs->def_process, pHDMITXArgs->def_fcnt);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_PKT_CONTENT_DEFINE:
            {
                pstHDMITx_PKT_Content_Define pHDMITXArgs = (pstHDMITx_PKT_Content_Define)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_PKT_CONTENT_DEFINE(pInstance, pHDMITXArgs->packet_type, pHDMITXArgs->data, pHDMITXArgs->length);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETVIDEOONOFF:
            {
                pstHDMITx_SetVideoOnOff pHDMITXArgs = (pstHDMITx_SetVideoOnOff)pArgs;
                psHDMITXInstPri->fpHDMITX_SETVIDEOONOFF (pInstance, pHDMITXArgs->state);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETCOLORFORMAT:
            {
                pstHDMITx_SetColorFormat pHDMITXArgs = (pstHDMITx_SetColorFormat)pArgs;
                psHDMITXInstPri->fpHDMITX_SETCOLORFORMAT (pInstance, pHDMITXArgs->in_color, pHDMITXArgs->out_color);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SET_VS_INFOFRAME:
            {
                pstHDMITx_Set_VS_InfoFrame pHDMITXArgs = (pstHDMITx_Set_VS_InfoFrame)pArgs;
                psHDMITXInstPri->fpHDMITX_SET_VS_INFOFRAME(pInstance, pHDMITXArgs->vs_format, pHDMITXArgs->vs_3d, pHDMITXArgs->vs_vic);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETVIDEOOUTPUTTIMING:
            {
                pstHDMITx_SetVideoOutputTiming pHDMITXArgs = (pstHDMITx_SetVideoOutputTiming)pArgs;
                psHDMITXInstPri->fpHDMITX_SETVIDEOOUTPUTTIMING(pInstance, pHDMITXArgs->mode);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETVIDEOOUTPUTASEPCTRATIO:
            {
                pstHDMITx_SetVideoOutputAsepctRatio pHDMITXArgs = (pstHDMITx_SetVideoOutputAsepctRatio)pArgs;
                psHDMITXInstPri->fpHDMITX_SETVIDEOOUTPUTASEPECTRATIO(pInstance, pHDMITXArgs->out_ar);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETVIDEOOUTPUTOVERSCAN_AFD:
            {
                pstHDMITx_SetVideoOutputOverscan_AFD pHDMITXArgs = (pstHDMITx_SetVideoOutputOverscan_AFD)pArgs;
                psHDMITXInstPri->fpHDMITX_SETVIDEOOUTPUTOVERSCAN_AFD(pInstance, pHDMITXArgs->bflag, pHDMITXArgs->out_scaninfo, pHDMITXArgs->out_afd);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETAUDIOONOFF:
            {
                pstHDMITx_SetAudioOnOff pHDMITXArgs = (pstHDMITx_SetAudioOnOff)pArgs;
                psHDMITXInstPri->fpHDMITX_SETAUDIOONOFF(pInstance, pHDMITXArgs->state);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETAUDIOFREQUENCY:
            {
                pstHDMITx_SetAudioFrequency pHDMITXArgs = (pstHDMITx_SetAudioFrequency)pArgs;
                psHDMITXInstPri->fpHDMITX_SETAUDIOFREQUENCY(pInstance, pHDMITXArgs->freq);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETVIDEOOUTPUTOVERSCAN_AFD_II:
            {
                pstHDMITx_SetVideoOutputOverscan_AFD_II pHDMITXArgs = (pstHDMITx_SetVideoOutputOverscan_AFD_II)pArgs;
                psHDMITXInstPri->fpHDMITX_SETVIDEOOUTPUTOVERSCAN_AFD_II(pInstance, pHDMITXArgs->bflag, pHDMITXArgs->out_scaninfo, pHDMITXArgs->out_afd, pHDMITXArgs->A0);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETAUDIOCONFIGURATION:
            {
                pstHDMITx_SetAudioConfiguration pHDMITXArgs = (pstHDMITx_SetAudioConfiguration)pArgs;
                psHDMITXInstPri->fpHDMITX_SETAUDIOCONFIGURATION(pInstance, pHDMITXArgs->freq, pHDMITXArgs->ch, pHDMITXArgs->type);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETAUDIOSOURCEFORMAT:
            {
                pstHDMITx_SetAudioSourceFormat pHDMITXArgs = (pstHDMITx_SetAudioSourceFormat)pArgs;
                psHDMITXInstPri->fpHDMITX_SETAUDIOSOURCEFORMAT(pInstance, pHDMITXArgs->fmt);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETAVIINFOEXTCOLORIMETRY:
            {
                pstHDMITx_SetAVIInfoExtColorimetry pHDMITXArgs = (pstHDMITx_SetAVIInfoExtColorimetry)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_SETAVIINFOCOLORIMETRY(pInstance, pHDMITXArgs->enExtColorimetry, pHDMITXArgs->enYccQuantRange);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETAUDIOCTS:
            {
                pstHDMITx_GetAudioCTS pHDMITXArgs = (pstHDMITx_GetAudioCTS)pArgs;
                pHDMITXArgs->u32Return = psHDMITXInstPri->fpHDMITX_GETAUDIOCTS(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_MUTEAUDIOFIFO:
            {
                pstHDMITx_MuteAudioFIFO pHDMITXArgs = (pstHDMITx_MuteAudioFIFO)pArgs;
                psHDMITXInstPri->fpHDMITX_MUTEAUDIOFIFO(pInstance, pHDMITXArgs->bflag);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETHDCPKEY:
            {
                pstHDMITx_GetHdcpKey pHDMITXArgs = (pstHDMITx_GetHdcpKey)pArgs;
                psHDMITXInstPri->fpHDMITX_GETHDCPKEY(pInstance, pHDMITXArgs->useinternalkey, pHDMITXArgs->data);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETBKSV:
            {
                pstHDMITx_GetBksv pHDMITXArgs = (pstHDMITx_GetBksv)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_GETBKSV(pInstance, pHDMITXArgs->pdata);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETAKSV:
            {
                pstHDMITx_GetAksv pHDMITXArgs = (pstHDMITx_GetAksv)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_GETAKSV(pInstance, pHDMITXArgs->pdata);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETHDCPONOFF:
            {
                pstHDMITx_SetHDCPOnOff pHDMITXArgs = (pstHDMITx_SetHDCPOnOff)pArgs;
                psHDMITXInstPri->fpHDMITX_SETHDCPONOFF(pInstance, pHDMITXArgs->state);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETAVMUTE:
            {
                pstHDMITx_SetAVMUTE pHDMITXArgs = (pstHDMITx_SetAVMUTE)pArgs;
                psHDMITXInstPri->fpHDMITX_SETAVMUTE(pInstance, pHDMITXArgs->bflag);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETAVMUTESTATUS:
            {
                pstHDMITx_GetAVMUTEStatus pHDMITXArgs = (pstHDMITx_GetAVMUTEStatus)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_GETAVMUTESTATUS(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_HDCP_REVOCATIONKEY_LIST:
            {
                pstHDMITx_HDCP_RevocationKey_List pHDMITXArgs = (pstHDMITx_HDCP_RevocationKey_List)pArgs;
                psHDMITXInstPri->fpHDMITX_HDCP_REVOCATIONKEY_LIST(pInstance, pHDMITXArgs->data, pHDMITXArgs->size);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_HDCP_REVOCATIONKEY_CHECK:
            {
                pstHDMITx_HDCP_RevocationKey_Check pHDMITXArgs = (pstHDMITx_HDCP_RevocationKey_Check)pArgs;
                pHDMITXArgs->stReturn = psHDMITXInstPri->fpHDMITX_HDCP_REVOCATIONKEY_CHECK(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_HDCP_ISSRMSIGNATUREVALID:
            {
                pstHDMITx_HDCP_IsSRMSignatureValid pHDMITXArgs = (pstHDMITx_HDCP_IsSRMSignatureValid)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_HDCP_ISSRMSIGNATUREVALID(pInstance, pHDMITXArgs->data, pHDMITXArgs->size);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETHDCPSTATUS:
            {
                pstHDMITx_GetHDCPStatus pHDMITXArgs = (pstHDMITx_GetHDCPStatus)pArgs;
                pHDMITXArgs->stReturn = psHDMITXInstPri->fpHDMITX_GETHDCPSTATUS(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_HDCP_STARTAUTH:
            {
                pstHDMITx_HDCP_StartAuth pHDMITXArgs = (pstHDMITx_HDCP_StartAuth)pArgs;
                psHDMITXInstPri->fpHDMITX_HDCP_STARTAUTH(pInstance, pHDMITXArgs->bFlag);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_HDMITX_CMD_GETINTHDCPSTATUS:
            {
                pstHDMITx_GetINTHDCPStatus pHDMITXArgs = (pstHDMITx_GetINTHDCPStatus)pArgs;
                pHDMITXArgs->stReturn = psHDMITXInstPri->fpHDMITX_GETINTHDCPSTATUS(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETHDCP_PRESTATUS:
            {
                pstHDMITx_GetHDCP_PreStatus pHDMITXArgs = (pstHDMITx_GetHDCP_PreStatus)pArgs;
                pHDMITXArgs->stReturn = psHDMITXInstPri->fpHDMITX_GETHDCP_PRESTATUS(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_UNHDCPRXCONTROL:
            {
                pstHDMITx_UnHDCPRxControl pHDMITXArgs = (pstHDMITx_UnHDCPRxControl)pArgs;
                psHDMITXInstPri->fpHDMITX_UNHDCPRXCONTROL(pInstance, pHDMITXArgs->state);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_HDMITX_CMD_HDCPRXFAILCONTROL:
            {
                pstHDMITx_HDCPRxFailControl pHDMITXArgs = (pstHDMITx_HDCPRxFailControl)pArgs;
                psHDMITXInstPri->fpHDMITX_HDCPRXFAILCONTROL(pInstance, pHDMITXArgs->state);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETLIBVER:
            {
                pstHDMITx_GetLibVer pHDMITXArgs = (pstHDMITx_GetLibVer)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_GETLIBVER(pInstance, pHDMITXArgs->ppVersion);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETINFO:
            {
                pstHDMITx_GetInfo pHDMITXArgs = (pstHDMITx_GetInfo)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_GETINFO(pInstance, pHDMITXArgs->pInfo);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETSTATUS:
            {
                pstHDMITx_GetStatus pHDMITXArgs = (pstHDMITx_GetStatus)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_GETSTATUS(pInstance, pHDMITXArgs->pStatus);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETDBGLEVEL:
            {
                pstHDMITx_SetDbgLevel pHDMITXArgs = (pstHDMITx_SetDbgLevel)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_SETDBGLEVEL(pHDMITXArgs->u16DbgSwitch);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETHPDGPIOPIN:
            {
                pstHDMITx_SetHPDGpioPin pHDMITXArgs = (pstHDMITx_SetHPDGpioPin)pArgs;
                psHDMITXInstPri->fpHDMITX_SETHPDGPIOPIN(pInstance, pHDMITXArgs->u8pin);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_ANALOGTUNING:
            {
                pstHDMITx_AnalogTuning pHDMITXArgs = (pstHDMITx_AnalogTuning)pArgs;
                psHDMITXInstPri->fpHDMITX_ANALOGTUNING(pInstance, pHDMITXArgs->pInfo);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_FORCEHDMIOUTPUTMODE:
            {
                pstHDMITx_ForceHDMIOutputMode pHDMITXArgs = (pstHDMITx_ForceHDMIOutputMode)pArgs;
                psHDMITXInstPri->fpHDMITX_FORCEHDMIOUTPUTMODE(pInstance, pHDMITXArgs->bflag, pHDMITXArgs->output_mode);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_FORCEHDMIOUTPUTCOLORFORMAT:
            {
                pstHDMITx_ForceHDMIOutputColorFormat pHDMITXArgs = (pstHDMITx_ForceHDMIOutputColorFormat)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_FORCEHDMIOUTPUTCOLORFORMAT(pInstance, pHDMITXArgs->bflag, pHDMITXArgs->output_color);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_DISABLEREGWRITE:
            {
                pstHDMITx_DisableRegWrite pHDMITXArgs = (pstHDMITx_DisableRegWrite)pArgs;
                psHDMITXInstPri->fpHDMITX_DISABLEREGWRITE(pInstance, pHDMITXArgs->bFlag);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETEDIDPHYADR:
            {
                pstHDMITx_GetEDIDPhyAdr pHDMITXArgs = (pstHDMITx_GetEDIDPhyAdr)pArgs;
                psHDMITXInstPri->fpHDMITX_GETEDIDPHYADR(pInstance, pHDMITXArgs->pdata);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETCECONOFF:
            {
                pstHDMITx_SetCECOnOff pHDMITXArgs = (pstHDMITx_SetCECOnOff)pArgs;
                psHDMITXInstPri->fpHDMITX_SETCECONOFF(pInstance, pHDMITXArgs->bflag);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETCECSTATUS:
            {
                pstHDMITx_GetCECStatus pHDMITXArgs = (pstHDMITx_GetCECStatus)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_GETCECSTATUS(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_EDIDCHECKING:
            {
                pstHDMITx_EdidChecking pHDMITXArgs = (pstHDMITx_EdidChecking)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_EDIDCHECKING(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_RXBYPASS_MODE:
            {
                pstHDMITx_RxBypass_Mode pHDMITXArgs = (pstHDMITx_RxBypass_Mode)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_RXBYPASS_MODE(pInstance, pHDMITXArgs->freq, pHDMITXArgs->bflag);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_DISABLE_RXBYPASS:
            {
                pstHDMITx_Disable_RxBypass pHDMITXArgs = (pstHDMITx_Disable_RxBypass)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_DISABLE_RXBYPASS(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETAKSV2R0INTERVAL:
            {
                pstHDMITx_SetAksv2R0Interval pHDMITXArgs = (pstHDMITx_SetAksv2R0Interval)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_SETAKSV2R0INTERVAL(pInstance, pHDMITXArgs->u32Interval);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_ISRXVALID:
            {
                pstHDMITx_IsHDCPRxValid pHDMITXArgs = (pstHDMITx_IsHDCPRxValid)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_ISRXVALID(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETCHIPCAPS:
            {
                pstHDMITx_GetChipCaps pHDMITXArgs = (pstHDMITx_GetChipCaps)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_GETCHIPCAPS(pInstance, pHDMITXArgs->eCapType, pHDMITXArgs->pRet, pHDMITXArgs->ret_size);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETPOWERSTATE:
            {
                pstHDMITx_SetPowerState pHDMITXArgs = (pstHDMITx_SetPowerState)pArgs;
                pHDMITXArgs->u32Return = psHDMITXInstPri->fpHDMITX_SETPOWERSTATE(pInstance, pHDMITXArgs->u16PowerState);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETEDIDDATABLOCK:
            {
                pstHDMITx_GetEdidDataBlocks pHDMITXArgs = (pstHDMITx_GetEdidDataBlocks)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_GETEDIDDATABLOCK(pInstance, pHDMITXArgs->enTagCode, pHDMITXArgs->enExtTagCode, pHDMITXArgs->pu8Data, pHDMITXArgs->u32DataLen, pHDMITXArgs->pu32RealLen);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GETKSVLIST:
            {
                pstHDMITx_GetKSVList pHDMITXArgs = (pstHDMITx_GetKSVList)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_GETKSVLIST(pInstance, pHDMITXArgs->pu8Bstatus, pHDMITXArgs->pu8KSVList, pHDMITXArgs->u16BufLen, pHDMITXArgs->pu16KSVLength);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_HDCP2ACCESSX74:
            {
                pstHDMITx_HDCP2AccessX74 pHDMITXArgs = (pstHDMITx_HDCP2AccessX74)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITX_HDCP2ACCESSX74OFFSET(pInstance, pHDMITXArgs->u8PortIdx, pHDMITXArgs->u8OffsetAddr, pHDMITXArgs->u8OpCode, pHDMITXArgs->pu8RdBuf, pHDMITXArgs->u16RdLen, pHDMITXArgs->pu8WRBuff, pHDMITXArgs->u16WrLen);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_HDCP2TxInit:
            {
                pstHDMITx_HDCP2TxInit pHDMITXArgs = (pstHDMITx_HDCP2TxInit)pArgs;
                psHDMITXInstPri->fpHDMITx_HDCP2TxInit(pInstance, pHDMITXArgs->u8PortIdx, pHDMITXArgs->bEnable);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_HDCP2TxEnableEncrypt:
            {
                pstHDMITx_HDCP2TxEnableEncrypt pHDMITXArgs = (pstHDMITx_HDCP2TxEnableEncrypt)pArgs;
                psHDMITXInstPri->fpHDMITx_HDCP2TxEnableEncrypt(pInstance, pHDMITXArgs->u8PortIdx, pHDMITXArgs->bEnable);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_HDCP2TxFillCipherKey:
            {
                pstHDMITx_HDCP2TxFillCipherKey pHDMITXArgs = (pstHDMITx_HDCP2TxFillCipherKey)pArgs;
                psHDMITXInstPri->fpHDMITx_HDCP2TxFillCipherKey(pInstance, pHDMITXArgs->u8PortIdx, pHDMITXArgs->pu8Riv, pHDMITXArgs->pu8KsXORLC128);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GENERALCTRL:
            {
                pstHDMITx_GeneralCtrl pHDMITXArgs = (pstHDMITx_GeneralCtrl)pArgs;
                pHDMITXArgs->bReturn = psHDMITXInstPri->fpHDMITx_GeneralCtrl(pInstance, pHDMITXArgs->u32Cmd, pHDMITXArgs->pu8Buf, pHDMITXArgs->u32BufSize);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_COLOR_AND_RANGE_TRANSFORM:
            {
                pstHDMITX_COLOR_AND_RANGE_TRANSFORM_PARAMETERS pHDMITXArgs = (pstHDMITX_COLOR_AND_RANGE_TRANSFORM_PARAMETERS)pArgs;
                pHDMITXArgs->result = psHDMITXInstPri->fpHDMITx_Cmd_ColorAndRangeTransform(pInstance, pHDMITXArgs->input_color, pHDMITXArgs->output_color, pHDMITXArgs->input_range, pHDMITXArgs->output_range);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SSC_ENABLE:
            {
                pstHDMITX_SSCENABLE pHDMITXArgs = (pstHDMITX_SSCENABLE)pArgs;
                pHDMITXArgs->result = psHDMITXInstPri->fpHDMITx_Cmd_SSCEnable(pInstance, pHDMITXArgs->ubSSCEn);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SET_COLORIMETRY:
            {
                pstHDMITX_SET_COLORIMETRY pHDMITXArgs = (pstHDMITX_SET_COLORIMETRY)pArgs;
                pHDMITXArgs->u8Return = psHDMITXInstPri->fpHDMITx_Cmd_Set_Colorimetry(pInstance, pHDMITXArgs->colorimetry);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GET_FULL_RX_STATUS:
            {
                pstHDMITX_GET_FULL_RX_STATUS pHDMITXArgs = (pstHDMITX_GET_FULL_RX_STATUS)pArgs;
                pHDMITXArgs->u32RxStatus = psHDMITXInstPri->fpHDMITx_Cmd_Get_Full_Rx_Status(pInstance);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_TIMING_CAPABILITY_CHECK:
            {
                pstHDMITX_CHECK_LEGAL_TIMING pHDMITXArgs = (pstHDMITX_CHECK_LEGAL_TIMING)pArgs;
                pHDMITXArgs->ubRet = psHDMITXInstPri->fpHDMITx_Cmd_Timing_Capability_Check(pInstance, pHDMITXArgs->eOutputMode, pHDMITXArgs->eTiming, pHDMITXArgs->eInColor, pHDMITXArgs->eOutColor, pHDMITXArgs->eColorDepth);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_GET_TMDS_STATUS:
            {
                pstHDMITX_GET_TMDS_STATUS pHDMITXArgs = (pstHDMITX_GET_TMDS_STATUS)pArgs;
                pHDMITXArgs->u32TMDSStatus = psHDMITXInstPri->fpHDMITx_Cmd_Get_TMDS_Status(pInstance);
                pHDMITXArgs->u32Ret = pHDMITXArgs->u32TMDSStatus;

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_HDMITX_CMD_SETVIDEOUTPUTTIMINGBYCUSTOMER:
            {
                pstHDMITx_stHDMITx_SetVideoOutputTimingByCustomer pHDMITXArgs = (pstHDMITx_stHDMITx_SetVideoOutputTimingByCustomer)pArgs;
                psHDMITXInstPri->fpHDMITX_SETVIDEOOUTPUTTIMINGBYCUSTOMER(pInstance, pHDMITXArgs->u8Mode, pHDMITXArgs->stTiminginfo);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }

#if defined(VANCLEEF_MCP) || defined(RAPTORS_MCP)
        case E_HDMITX_CMD_VANCLEEF_LOADHDCPKEY:
            {
                pstHDMITx_Vancleef_LoadHDCPKey pHDMITXArgs = (pstHDMITx_Vancleef_LoadHDCPKey)pArgs;
                psHDMITXInstPri->fpHDMITX_VANCLEEF_LOADHDCPKEY(pInstance, pHDMITXArgs->pHdcpKey, pHDMITXArgs->u32Size);

                return UTOPIA_STATUS_SUCCESS;
                break;
            }
#endif

        default:
            printf("Error Command = 0x%X\n", u32Cmd);
            break;

    }
#endif
    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 HDMITXClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);

    return UTOPIA_STATUS_SUCCESS;
}


