//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2014 - 2016 MStar Semiconductor, Inc. All rights reserved.
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
// Copyright (c) 2014-2016 MStar Semiconductor, Inc.
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
/// file  DMX_adp.c
/// @brief  Demux adaption API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>

#include "MsTypes.h"
#include "utopia.h"
#include "apiAUDIO.h"
#include "apiAUDIO_v2.h"
#include "drvAUDIO_if.h"

#include "utopia_adp.h"
#include "AUDIO_adp.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern UADP_STRUCT_POINTER_TABLE spt_MS_U32[];
extern UADP_STRUCT_POINTER_TABLE spt_MS_U16[];
extern UADP_STRUCT_POINTER_TABLE spt_MS_U8[];
extern UADP_STRUCT_POINTER_TABLE spt_MS_NULL[];


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#define CPY_FROM_USER                   copy_from_user
#define CPY_to_USER                     copy_to_user
#else
#define CPY_FROM_USER                   memcpy
#define CPY_to_USER                     memcpy
#endif

#define AUDIO_ADP_MSG(_f, _a...)                             //printf("\033[1;35m"_f"\033[0m", ##_a)

UADP_SPT_NAMEnNXT_DEF(MS_PHY_AUDIO,0);

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct __attribute__((packed)) {
	MS_U32		value1;
	MS_U32		value2;
} u32param2_t;

typedef struct __attribute__((packed)) {
	MS_U32		value1;
	MS_U32		value2;
	MS_U32		value3;
	MS_U32		value4;
} u32param4_t;

#define SIF_THRTBL_LENGTH  74
#define AUDIO_STRING_NAME_MAX_LENGTH 64

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_U32 AUDIO_adp_Init(FUtopiaIOctl *pIoctl)
{
    UADP_SDT_NAME0(MS_PHY_AUDIO, MS_PHY);
    *pIoctl= (FUtopiaIOctl)AUDIO_adp_Ioctl;
    return 0;
}

MS_U32 AUDIO_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 						u32Ret = UTOPIA_STATUS_FAIL, u32data = 1;

    char 						buffer_arg[1024];
    PAUDIO_ST_DECODER_COMMAND   pDecodeCommandSrc                   = NULL, pDecodeCommandDes                       = NULL;
    PAUDIO_ST_SPDIF_OP          pSPDIF_OpSrc                        = NULL, pSPDIF_OpDes                            = NULL;
    PAUDIO_ST_SPDIF_SCMS        pSPDIF_SCMSSrc                      = NULL, pSPDIF_SCMSDes                          = NULL;
    PAUDIO_ST_HDMI_NONPCM_OP    pHDMI_NonPCM_OpSrc                  = NULL, pHDMI_NonPCM_OpDes                      = NULL;
    PAUDIO_ST_KTV_OP            pKTV_OpSrc                          = NULL, pKTV_OpDes                              = NULL;
    PAUDIO_ST_DMAREADER_WRITEPCM pDMAReader_WritePCMSrc             = NULL, pDMAReader_WrtiePCMDes                  = NULL;
    PAUDIO_ST_PCMCAPTURE_READ   pPCMCaptureReadSrc                  = NULL, pPCMCaptureReadDes                      = NULL;
    PAUDIO_ST_GETAUDIOINFO2     pGetAudioInfo2Src                   = NULL, pGetAudioInfo2Des                       = NULL;
    PAUDIO_ST_OUTPUT_SOURCE     pOutputSourceSrc                    = NULL, pOutputSourceDes                        = NULL;
    PAUDIO_ST_COPY_PARAMETER    pCopyParamterSrc                    = NULL, pCopyParamterDes                        = NULL;
    PAUDIO_ST_SET_AUDIOPARAM2   pSetAudioParam2Src                  = NULL, pSetAudioParam2Des                      = NULL;
    PAUDIO_ST_PCM_GET           pPcmGetSrc                          = NULL, pPcmGetDes                              = NULL;
    PAUDIO_ST_PCM_READ          pPcmReadSrc                         = NULL, pPcmReadDes                             = NULL;
    PAUDIO_ST_PCM_WRITE         pPcmWriteSrc                        = NULL, pPcmWriteDes                            = NULL;
    PAUDIO_ST_ENCODER_READ      pEncoderReadSrc                     = NULL, pEncoderReadDes                         = NULL;

    UADP_STRUCT_POINTER_TABLE spt_AUDIO_tmp[10];
    UADP_STRUCT_POINTER_TABLE spt_AUDIO_tmp1[10];
    UADP_STRUCT_POINTER_TABLE spt_AUDIO_tmp2[10];

#if 0
    char 						buffer_array0[1024];
    MS_U32 						*pu32Src                            = NULL, *pu32Des                                = NULL;
    MS_S32 						*ps32Src                            = NULL, *ps32Des                                = NULL;
    PAUDIO_ST_DECODESYSTEM      pDecodeSystemSrc                    = NULL, pDecodeSystemDes                        = NULL;
    PAUDIO_ST_DIGITALOUT_GETCHANNELSTATUS   pDigitalOut_GetChannelStatusSrc = NULL, pDigitalOut_GetChannelStatusDes = NULL;
    PAUDIO_ST_SETDECODEPARAM    pSetDecodeParamSrc                  = NULL, pSetDecodeParamDes                      = NULL;
	PAUDIO_ST_SPDIF_CHANNELSTATUS pSPDIF_ChannelStatusSrc           = NULL , pSPDIF_ChannelStatusDes                = NULL;
#endif

	AUDIO_ADP_MSG("[V] ADA Cmd %d \n", (int)u32Cmd);

    switch(u32Cmd)
    {
        //============================================================
        // AUDIO_SYSTEM RELATIONAL API FUNCTION
        //============================================================

        case MApi_CMD_AUDIO_Initialize:
            // void MApi_AUDIO_Initialize(void)
            AUDIO_ADP_MSG("MApi_CMD_AUDIO_Initialize cmd %d\n", (int)u32Cmd);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);

            break;

        case MApi_CMD_AUDIO_SetPowerOn:
            //void MApi_AUDIO_SetPowerOn(MS_BOOL bFlag)

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetDSPBaseAddr:
            AUDIO_ADP_MSG("MApi_CMD_AUDIO_SetDSPBaseAddr cmd %d\n", (int)u32Cmd);

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SET_DSPBASEADDR));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetDSPBaseAddr:

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_GET_DSPBASEADDR));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_WriteDecMailBox:

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_WRITE_DECMAILBOX));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_TriggerSIFPLL:

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_Monitor:
            //void MApi_Audio_Monitor(void)
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetSystemInfo:
            AUDIO_ADP_MSG("MApi_CMD_AUDIO_SetDSPBaseAddr cmd %d\n", (int)u32Cmd);
            //void MApi_AUDIO_SetSystemInfo(AUDIO_INIT_INFO *pau_info)
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_AUDIO_INIT_INFO, NULL, buffer_arg,sizeof(buffer_arg));
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_INIT_INFO));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_ASF_Data_To_DDR:
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_R2_DEBUG_MSG:
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_R2_DDR_LETENCY_MSG:
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetOutConnectivity:
            //void MApi_AUDIO_SetOutConnectivity(void)
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetPathInfo:
            //void MApi_AUDIO_SetPathInfo(AUDIO_PATH_INFO *path_info)
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_AUDIO_PATH_INFO, NULL, buffer_arg,sizeof(buffer_arg));
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_PATH_INFO));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetPathInfo:
            //void MApi_AUDIO_GetPathInfo(AUDIO_PATH_INFO *path_info)
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_AUDIO_PATH_INFO, NULL, buffer_arg,sizeof(buffer_arg));
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_PATH_INFO));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetOutputInfo:
            //void MApi_AUDIO_SetOutputInfo(AUDIO_OUT_INFO *pout_info)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_OUT_INFO));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetSourceInfo:
            //void MApi_AUDIO_SetSourceInfo(AUDIO_SOURCE_INFO_TYPE eSourceType)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_InputSwitch:
            //void MApi_AUDIO_InputSwitch(AUDIO_INPUT_TYPE enSource, AUDIO_SWITCH_GROUP enGroup)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_INPUTSWITCH));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_Set_Input_Scale:
            //void MApi_AUDIO_SetInputScale(int Step)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_Set_Scart_Scale:
            //void MApi_AUDIO_SetScartScale(int Step)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

#if 0
        case MApi_CMD_AUDIO_AUDYSSEY_VXT_SET_THRESH_LEVEL_TABLE:
            //MS_BOOL MApi_AUDIO_Audyssey_VXT_Set_THRESH_LEVEL_TABLE(int *thresh_level_table)
            pAudysseyVXTSetThreshLevelTableSrc = (PAUDIO_ST_AUDYSSEY_VXT_SET_THRESH_LEVEL_TABLE)pArgs;
            pAudysseyVXTSetThreshLevelTableDes = (PAUDIO_ST_AUDYSSEY_VXT_SET_THRESH_LEVEL_TABLE)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_AUDYSSEY_VXT_SET_THRESH_LEVEL_TABLE));

            pAudysseyVXTSetThreshLevelTableDes->pthresh_level_table = (int*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)pAudysseyVXTSetThreshLevelTableSrc->pthresh_level_table, sizeof(int)); //Fix me

            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            CPY_to_USER((void*)(&(pAudysseyVXTSetThreshLevelTableSrc->bStatus)), (void*)(&pAudysseyVXTSetThreshLevelTableDes->bStatus), sizeof(MS_BOOL));
            break;
#endif
        case MApi_CMD_AUDIO_SPDIF_CHANNELSTATUS:
#if 0
			// todo stanley
            pSPDIF_ChannelStatusSrc = (PAUDIO_ST_SPDIF_CHANNELSTATUS)pArgs;
            pSPDIF_ChannelStatusDes = (PAUDIO_ST_SPDIF_CHANNELSTATUS)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SPDIF_CHANNELSTATUS));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);

            if(pSPDIF_ChannelStatusSrc->bSet== FALSE)
            {
                CPY_to_USER((void*)(&(pSPDIF_ChannelStatusSrc->u32Ret)), (void*)(&(pSPDIF_ChannelStatusDes->u32Ret)), sizeof(MS_U32));
            }
#endif
            break;

        case MApi_CMD_AUDIO_SetDataCaptureSource:
            //MS_BOOL MApi_AUDIO_SetDataCaptureSource(AUDIO_DEVICE_TYPE eID, AUDIO_CAPTURE_SOURCE_TYPE eSource)
            // u32Param[0] = 0(MS_BOOL); u32Param[1] = ((MS_U32)eID) & 0xFFFF;  u32Param[2] = ((MS_U32)eSource) & 0xFFFF;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SET_DATACAPTURESOURCE));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_OutputSourceInfo:
            //void MApi_AUDIO_SetOutputSourceInfo(OUTPUT_SOURCE_INFO *pSourceInfo)
            //Status.bSet = TRUE/FALSE; Status.stOutputSourceInfo = pSourceInfo;
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_OUTPUT_SOURCE, NULL, buffer_arg,sizeof(buffer_arg));
			pOutputSourceSrc = (PAUDIO_ST_OUTPUT_SOURCE)pArgs;
            pOutputSourceDes = (PAUDIO_ST_OUTPUT_SOURCE)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_OUTPUT_SOURCE));

			UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(OUTPUT_SOURCE_INFO));
			UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_OUTPUT_SOURCE));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_OUTPUT_SOURCE, UADP_SDT_P2N, stOutputSourceInfo, spt_AUDIO_tmp1);
			UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

			if (pOutputSourceDes->bSet == FALSE)
				u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
			else
				u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_ReadMailBox:
            //MS_U16 MApi_AUDIO_ReadMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum)
            //u32Param[0] = return; u32Param[1] = bDspType; u32Param[2] = u8ParamNum;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_READ_MAILBOX));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_WriteMailBox:
            //void MApi_AUDIO_WriteMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum, MS_U16 u16Data)
            //u32Param[0] = bDspType; u32Param[1] = u8ParamNum; u32Param[2] = u16Data;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_WRITE_MAILBOX));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_FwTriggerDSP:
            //void MApi_AUDIO_FwTriggerDSP(MS_U16 u16Cmd)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetFwStatus:
            //void MApi_AUDIO_SetFwStatus(MS_BOOL bBootOnDDR)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_ExitAudioSystem:
            //void MApi_AUDIO_ExitAudioSystem(void)
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_RebootDSP:
            //void MApi_AUDIO_RebootDsp(MS_BOOL bDspType)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SendIntrupt:
            //void MApi_AUDIO_SendIntrupt(MS_BOOL bDspType,MS_U8 u8Cmd)
            //u32Param[0] = bDspType;u32Param[1] = u8Cmd);
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SEND_INTERRUPT));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_I2S_SetMode:
            //void MApi_AUDIO_I2S_SetMode(AUDIO_I2S_MODE_TYPE u8Mode, AUDIO_I2S_MODE_VALUE  u8Val)
            //u32Param[0] = u8Mode; u32Param[1] = u8Val;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_I2S_SETMODE));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_WritePreInitTable:
            //void MApi_AUDIO_WritePreInitTable(void)
            AUDIO_ADP_MSG("MApi_CMD_AUDIO_WritePreInitTable cmd %d\n", (int)u32Cmd);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_EnaEarphone_LowPower_Stage:
            //void MApi_AUDIO_EnaEarphone_LowPower_Stage(void)
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);

            break;

        case MApi_CMD_AUDIO_EnaEarphone_HighDriving_Stage:
            //void MApi_AUDIO_EnaEarphone_HighDriving_Stage(void)
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);

            break;

        //=============================================================
        // AUDIO_SOUND RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_SND_ProcessEnable:
            //MS_BOOL MApi_SND_ProcessEnable(Sound_ENABLE_Type Type, MS_BOOL enable)
            //u32Param[0] = return, u32Param[1] = Type; u32Param[2] = enable;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SND_PROCESSENABLE));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);

            break;

        case MApi_CMD_SND_SetParam1:
            //MS_BOOL MApi_SND_SetParam1( Sound_SET_PARAM_Type Type, MS_U16 param1, MS_U16 param2)
            //u32Param[0] = return; u32Param[1] = Type; u32Param[2] = param1; u32Param[3] = param2;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SND_SETPARAM1));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);

            break;

        case MApi_CMD_SND_GetParam1:
            //MS_U16 MApi_SND_GetParam1( Sound_GET_PARAM_Type Type, MS_U16 param1)
            //u32Param[0] = return; u32Param[1] = Type; u32Param[2] = param1;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SND_GETPARAM1));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_Sound_SetParam:
            //MS_BOOL MApi_AUDIO_SOUND_SetParam(Sound_SET_Type Type, MS_U32 *p_param )
			UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(u32param4_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SOUND_SETPARAM));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_SOUND_SETPARAM, UADP_SDT_P2N, pParam, spt_AUDIO_tmp1);
			UADP_SDT_FIN(&spt_AUDIO_tmp[2]);


			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);

            break;

        case MApi_CMD_AUDIO_SetEq_7band:
            //void MApi_AUDIO_SetEq_7band(MS_U8 u8band, MS_U8 u8level)
            //u32Param[0] = u8band; u32Param[1] = u8level;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SETEQ_7BAND));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);

            break;

        case MApi_CMD_AUDIO_SetAvcThreshold:
            //void _MApi_AUDIO_SetAvcThreshold(MS_U8 Level);
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);

            break;

        case MApi_CMD_AUDIO_SetAvcMode:
            //void _MApi_AUDIO_SetAvcMode(MS_U8 AvcMode);
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetAvcAT:
            //void _MApi_AUDIO_SetAvcAT(MS_U8 AvcAT);
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetAvcRT:
            //void _MApi_AUDIO_SetAvcRT(MS_U8 AvcRT);
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetSurroundXA:
            //void _MApi_AUDIO_SetSurroundXA(MS_U8 mode);
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetSurroundXB:
            //void _MApi_AUDIO_SetSurroundXB(MS_U8 mode);
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetSurroundXK:
            //void _MApi_AUDIO_SetSurroundXK(MS_U8 mode);
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetSurroundLPFGain:
            //void _MApi_AUDIO_SetSurroundLPFGain(MS_U8 mode);
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetAbsoluteVolume:
            //void MApi_AUDIO_SetAbsoluteVolume(MS_U8 u8Path, MS_U8 u8Vol1, MS_U8 u8Vol2)
            //u32Param[0] = u8Path; u32Param[1] = u8Vol1; u32Param[2] = u8Vol2;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SET_ABSOLUTEVOLUME));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SourceConnect:
            //void MApi_AUDIO_SourceConnect(AUDIO_OUTPORT_SOURCE_TYPE eConnectSourceType, MS_BOOL bConnect )
            //u32Param[0] = eConnectSourceType; u32Param[1] = bConnect;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SOURCECONNECT));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetDynamicBass:
            //void MApi_AUDIO_SetDynamicBass(MS_U8 u8DynamicBass )
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_AbsoluteTreble:
            //void MApi_AUDIO_AbsoluteTreble(MS_U8 u8Treble )
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetBufferProcess:
            //void MApi_AUDIO_SetBufferProcess(MS_U8 DelayTime)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_ConvertVolumeUnit:
            //MS_U8 MApi_AUDIO_ConvertVolumeUnit(MS_U8 UiVolume)
            //u32Param[0] = return; u32Param[1] = UiVolume;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_CONVERT_VOLUMEUNIT));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetPEQCoef:
            //void MApi_AUDIO_SetPEQCoef(AUDIO_PEQ_COEF *peq_coef)
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_AUDIO_PEQ_COEF, NULL, buffer_arg,sizeof(buffer_arg));
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_PEQ_COEF));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_EnableDcRemove:
            //MS_BOOL MApi_AUDIO_EnableDcRemove(MS_BOOL EnDcRemove)
            //u32Param[0] = return; u32Param[1] = EnDcRemove;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_ENABLE_DCREMOVE));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;
        //============================================================
        // AUDIO_DTV RELATIONAL API FUNCTION
        //============================================================
#if 0
        case MApi_CMD_AUDIO_DecodeSystem:
            //MS_BOOL MApi_AUDIO_ReleaseDecodeSystem(AUDIO_DEC_ID DecId)
            //Status.bStatus = return; Status.eType = DECODE_SYS_RELEASE; Status.DecId = DecId;
            pDecodeSystemSrc = (PAUDIO_ST_DECODESYSTEM)pArgs;
            pDecodeSystemDes = (PAUDIO_ST_DECODESYSTEM)buffer_arg;

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_DECODESYSTEM));
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, (void*)buffer_arg);
            if(pDecodeSystemSrc->eType == DECODE_SYS_GET)
            {
                CPY_to_USER((void*)(&(pDecodeSystemSrc->p_AudioDecStatus)), (void*)(&(pDecodeSystemDes->p_AudioDecStatus)), sizeof(AudioDecStatus_t));
            }
            CPY_to_USER((void*)(&(pDecodeSystemSrc->bStatus)), (void*)(&(pDecodeSystemDes->bStatus)), sizeof(MS_BOOL));
            break;
#endif

        case MApi_CMD_AUDIO_OpenDecodeSystem:
			UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(AudioDecStatus_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_OPEN_DECODESYSTEM));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_OPEN_DECODESYSTEM, UADP_SDT_P2N, p_AudioDecStatus, spt_AUDIO_tmp1);
			UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);

        	break;

        case MApi_CMD_AUDIO_ReleaseDecodeSystem:
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_RELEASE_DECODESYSTEM));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
        	break;

        case MApi_CMD_AUDIO_GetDecodeSystem:
			UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(AudioDecStatus_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_GET_DECODESYSTEM));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_GET_DECODESYSTEM, UADP_SDT_P2N, p_AudioDecStatus, spt_AUDIO_tmp1);
			UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
	        break;

        case MApi_CMD_AUDIO_SetDecodeSystem:
			UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(AudioDecStatus_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SET_DECODESYSTEM));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_SET_DECODESYSTEM, UADP_SDT_P2N, p_AudioDecStatus, spt_AUDIO_tmp1);
			UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
	        break;

        case MApi_CMD_AUDIO_SetDecodeCmd:
            //MS_BOOL MApi_AUDIO_SetDecodeCmd(AUDIO_DEC_ID DecId, En_DVB_decCmdType enDecComamnd)
            //u32Param[0] = return; u32Param[1] = DecId; u32Param[2] = enDecComamnd;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SET_DECCMD));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetSystem:
            //MS_BOOL MApi_AUDIO_SetSystem(En_DVB_decSystemType enDecSystem)
            //u32Param[0] = return; u32Param[1] = enDecSystem;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SET_SYSTEM));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetDecSysSupportStatus:
            //MS_BOOL MApi_AUDIO_GetDecSysSupportStatus(En_DVB_decSystemType enDecSystem)
            //u32Param[0] = return; u32Param[1] = enDecSystem;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_GET_DECSYSSUPPORTSTATUS));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_IsMadLock:
            //MS_BOOL MApi_AUDIO_IsMadLock(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetDecStatus:
            //En_DVB_decCmdType MApi_AUDIO_GetDecStatus(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_Decode_Command:
            //AU_DVB_DECCMD MApi_AUDIO_GetCommand(En_DVB_AudioDecoder AdoDecoderNo)
            //Status.bSet = TRUE/FALSE; Status.eDecCmd = Return; Status.eAdoDecoderNo = AdoDecoderNo;
			pDecodeCommandSrc = (PAUDIO_ST_DECODER_COMMAND)pArgs;
            pDecodeCommandDes = (PAUDIO_ST_DECODER_COMMAND)buffer_arg;

            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_DECODER_COMMAND));

            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_DECODER_COMMAND));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			if(pDecodeCommandDes->bSet == FALSE)
				u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
			else
				u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetMAD_Lock:
            //MS_BOOL MApi_AUDIO_GetMAD_LOCK(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_SETAD:
            //void MApi_AUDIO_SetADOutputMode(MS_U8 out_mode)
            //Status.eType = AD_OUTPUT_MODE; Status.u8Data = out_mode/AD_ABSOLUTE_VOLUME/AD_SET_MUTE/SET_MAIN_VOLUME ; Status.bMute = FALSE;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SET_AD));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        //============================================================
        // AUDIO_PVR RELATED FUNCTIONS
        //============================================================
        case MApi_CMD_AUDIO_SetEncInit:
            //void MApi_AUDIO_SetEncInit(AU_DVB_ENCBR BitRate, MS_U8 u8EncFrameNum)
            //u32Param[0] = BitRate; u32Param[1] = u8EncFrameNum;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SET_ENCINIT));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetEncCommand:
            //void MApi_AUDIO_SetEncCommand(AU_DVB_ENCCMD enEncComamnd)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetEncodeFrameInfo:     //HAL is empty
            //void MApi_AUDIO_GetEncodeFrameInfo(AU_DVB_ENC_FRAME_INFO *EncFrameInfo)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AU_DVB_ENC_FRAME_INFO));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetEncodeDoneFlag:
            //MS_BOOL MApi_AUDIO_GetEncodeDoneFlag(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        //============================================================
        // AUDIO_ATV RELATIONAL API FUNCTION
        //============================================================
        case MApi_CMD_AUDIO_SIF_SetStandard:
            //void MApi_AUDIO_SIF_SetStandard(AUDIOSTANDARD_TYPE standard_type)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SIF_GetSoundMode:
            //MS_U8 MApi_AUDIO_SIF_GetSoundMode(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_SIF_SetSoundMode:
            //MS_BOOL MApi_AUDIO_SIF_SetSoundMode(MS_U8 u8SifSoundMode)
            //u32Param[0] = return; u32Param[1] = u8SifSoundMode;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SIF_SET_SOUNDMODE));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_SIF_GetAudioStatus:
            //MS_BOOL MApi_AUDIO_SIF_GetAudioStatus(AUDIOSTATUS * eAudioStatus)
            //Status.bStatus = return;  Status.p_eAudioStatus = E_STATE_AUDIO_NO_CARRIER;
			UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(AUDIOSTATUS));
			UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SIF_GET_AUDIO_STATUS));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_SIF_GET_AUDIO_STATUS, UADP_SDT_P2N, p_eAudioStatus, spt_AUDIO_tmp1);
			UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_SIF_StartAutoStandardDetection:
            //void MApi_AUDIO_SIF_StartAutoStandardDetection(void)
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SIF_GetResultOfAutoStandardDetection:
            //AUDIOSTANDARD_TYPE MApi_AUDIO_SIF_GetResultOfAutoStandardDetection(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_SIF_ConvertToBasicAudioStandard:
            //AUDIOSTANDARD_TYPE MApi_AUDIO_SIF_ConvertToBasicAudioStandard(AUDIOSTANDARD_TYPE eStandard)
            //u32Param[0] = return; u32param[1] = eStandard;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SIF_SIF_CONVERTTOBASICAUDIOSTANDARD));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_SIF_SetThreshold:
            //void MApi_AUDIO_SIF_SetThreshold(THR_TBL_TYPE code *ThrTbl)
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_THR_TBLL_TYPE, NULL, buffer_arg,sizeof(buffer_arg));
			//UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(THR_TBL_TYPE));
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U16) * SIF_THRTBL_LENGTH);
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SIF_SetPrescale:
            //void MApi_AUDIO_SIF_SetPrescale(MS_U8 gain_type, int db_value)
            //u32Param[0] = gain_type; u32Param[1] = db_value;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SIF_SETPRESCALE));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SIF_IsPALType:
            //MS_BOOL MApi_AUDIO_SIF_IsPALType(AUDIO_SIF_PAL_TYPE pal_type)
            //u32Param[0] = return; u32param[1] = pal_type;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SIF_SIF_ISPALTYPE));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_SIF_SetPALType:
            //void MApi_AUDIO_SIF_SetPALType(AUDIO_SIF_PAL_TYPE pal_type)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SIF_SendCmd:
            //void MApi_AUDIO_SIF_SendCmd(En_AUD_SIF_CmdType enAudSifCommand, MS_U8 comm_arg1, MS_U8 comm_arg2)
            //u32Param[0] = enAudSifCommand; u32Param[1] = comm_arg1; u32Param[2] = comm_arg2;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SIF_SENDCMD));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SIF_Shift:
            //void MApi_AUDIO_SIF_Shift(En_AUD_VIF_Type type)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        //============================================================
        // AUDIO_FM_RADIO RELATIONAL API FUNCTION
        //============================================================
        case MApi_CMD_AUDIO_FM_RADIO_GetSoundMode:
            //MS_U8 MApi_AUDIO_FM_RADIO_GetSoundMode(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_FM_RADIO_SetSoundMode:
            //void MApi_AUDIO_FM_RADIO_SetSoundMode(MS_U8 u8FMRadioSoundMode)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_FM_RADIO_DeEmphassisOption:
            //void MApi_AUDIO_FM_RADIO_DeEmphassisOption(MS_BOOL Is_Europe)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_FM_RADIO_GetDC_Amp:
            //MS_S8 MApi_AUDIO_FM_RADIO_GET_DC_AMP(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_FM_RADIO_Get_NSR:
            //MS_U8 MApi_AUDIO_FM_RADIO_GET_NSR(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_FM_RADIO_Reset:
            //void MApi_AUDIO_FM_RADIO_RESET(void)
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);
            break;

        //=============================================================
        // AUDIO_SPDIF RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_AUDIO_SPDIF_OP:
            //void MApi_AUDIO_SPDIF_HWEN(MS_BOOL spdif_en)
            //Status.eType = SPDIF_HWEN/SPDIF_SETMUTE/SPDIF_SETMODE/SPDIF_GETMODE; Status.spdif_hwen = spdif_en; Status.mute_en = FALSE; Status.spdif_mode = SPDIF_OUT_NONE;
			pSPDIF_OpSrc = (PAUDIO_ST_SPDIF_OP)pArgs;
            pSPDIF_OpDes = (PAUDIO_ST_SPDIF_OP)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SPDIF_OP));

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SPDIF_OP));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			if (pSPDIF_OpDes->eType == SPDIF_GETMODE)
				u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
			else
				u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);

            break;

        case MApi_CMD_AUDIO_SPDIF_SCMS:
            //void MApi_AUDIO_SPDIF_SetSCMS(MS_U8 C_bit_en, MS_U8 L_bit_en)
            //Status.bSetSCMS = TRUE/FALSE; Status.C_bit_en = C_bit_en; Status.L_bit_en = L_bit_en; Status.Ret_bit = 0;
            pSPDIF_SCMSSrc = (PAUDIO_ST_SPDIF_SCMS)pArgs;
            pSPDIF_SCMSDes = (PAUDIO_ST_SPDIF_SCMS)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SPDIF_SCMS));

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SPDIF_SCMS));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			if (pSPDIF_SCMSDes->bSetSCMS == FALSE)
				u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
			else
				u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;
        case MApi_CMD_AUDIO_SPDIF_Monitor:
            //void MApi_Audio_SPDIF_Monitor(void)
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SPDIF_ChannelStatus_CTRL:
            //MS_BOOL MApi_AUDIO_SPDIF_ChannelStatus_CTRL(AUDIO_SPDIF_CS_TYPE cs_mode, AUDIO_SPDIF_CS_TYPE_STATUS status)
            //u32Param[0] = return; u32param[1] = cs_mode; u32param[2] = status;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SPDIF_CHANNELSTATUS_CTRL));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_DigitalOut_SetChannelStatus:
            //MS_BOOL MApi_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus)
            //Status.bStatus = return; Status.ePath = ePath; Status.stChannelStatus = stChannelStatus;
			UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(Digital_Out_Channel_Status_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_DIGITALOUT_SETCHANNELSTATUS));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_DIGITALOUT_SETCHANNELSTATUS, UADP_SDT_P2N, stChannelStatus, spt_AUDIO_tmp1);
			UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_DigitalOut_SetDeviceCapability:
            //MS_BOOL _MApi_AUDIO_DigitalOut_SetDeviceCapability(DIGITAL_OUTPUT_TYPE eoutType, AUDIO_FORMAT_CODE ecodecType, Digital_Out_Device_Capability_t *p_codecCapability)
			UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(Digital_Out_Device_Capability_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_DIGITALOUT_SETDEVICECAP));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_DIGITALOUT_SETDEVICECAP, UADP_SDT_P2N, p_codecCapability, spt_AUDIO_tmp1);
			UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_DigitalOut_GetChannelStatus:
#if 0
			// todo stanley
            pDigitalOut_GetChannelStatusSrc = (PAUDIO_ST_DIGITALOUT_GETCHANNELSTATUS)pArgs;
            pDigitalOut_GetChannelStatusDes = (PAUDIO_ST_DIGITALOUT_GETCHANNELSTATUS)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_DIGITALOUT_GETCHANNELSTATUS));
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pDigitalOut_GetChannelStatusSrc->stChannelStatus->u8Category)), (void*)(&(pDigitalOut_GetChannelStatusDes->stChannelStatus->u8Category)), sizeof(MS_U8));
            CPY_to_USER((void*)(&(pDigitalOut_GetChannelStatusSrc->stChannelStatus->stCopyRight.CBit)), (void*)(&(pDigitalOut_GetChannelStatusDes->stChannelStatus->stCopyRight.CBit)), sizeof(MS_BOOL));
            CPY_to_USER((void*)(&(pDigitalOut_GetChannelStatusSrc->stChannelStatus->stCopyRight.LBit)), (void*)(&(pDigitalOut_GetChannelStatusDes->stChannelStatus->stCopyRight.LBit)), sizeof(MS_BOOL));
#endif
            break;

        //=============================================================
        // AUDIO_HDMI RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_AUDIO_HDMI_Tx_SetMute:
            //void MApi_AUDIO_HDMI_Tx_SetMute(MS_BOOL mute_en)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_HDMI_Tx_GetStatus:
            //void MApi_AUDIO_HDMI_Tx_GetStatus(MS_BOOL *onOff, AUDIO_FS_TYPE *hdmi_SmpFreq, HDMI_TX_OUTPUT_TYPE *outType )
            //Status.onOff = return; Status.hdmi_smpFreq = return; Status.outType = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_HDMI_TX_GETSTATUS));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_HDMI_Monitor:
            //MS_U8 MApi_AUDIO_HDMI_Monitor(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_HDMI_GetNonpcmFlag:
            //MS_BOOL MApi_AUDIO_HDMI_GetNonpcmFlag(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_HDMI_SetNonpcm:
            //void MApi_AUDIO_HDMI_SetNonpcm(MS_U8 nonPCM_en)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_HDMI_Nonpcm_OP:
            //AUDIO_HDMI_RX_TYPE MApi_AUDIO_HDMI_RX_SetNonpcm(MS_U8 nonPCM_en)
            //Status.bSet = TRUE/FALSE; Status.nonPCM_en = nonPCM_en;Status.eRet = return;
            pHDMI_NonPCM_OpSrc = (PAUDIO_ST_HDMI_NONPCM_OP)pArgs;
            pHDMI_NonPCM_OpDes = (PAUDIO_ST_HDMI_NONPCM_OP)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_HDMI_NONPCM_OP));

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_HDMI_NONPCM_OP));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

            if(pHDMI_NonPCM_OpDes->bSet == FALSE)
            	u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            else
            	u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);

            break;

        case MApi_CMD_AUDIO_DTV_HDMI_CFG:
            //void MApi_AUDIO_DTV_HDMI_CFG(MS_U8 ctrl)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_HDMI_GetSynthFreq:
            //MS_U16 MApi_AUDIO_HDMI_GetSynthFreq(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_HDMI_SetDownSample:
            //void MApi_AUDIO_HDMI_SetDownSample(MS_U8 ratio)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_HDMI_TX_SetMode:
            //void MApi_AUDIO_HDMI_TX_SetMode(HDMI_TX_OUTPUT_TYPE outType)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_HDMI_RX_GetHdmiInAudioStatus:
            //MS_BOOL MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus(ST_HDMI_AUDIO_STATUS *p_hdmiAudioSts)
            //Status.bStatus = Return;  Status.pHdmi_status = p_hdmiAudioSts;
			UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(ST_HDMI_AUDIO_STATUS));
			UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_RX_HDMI_AUDIO_STATUS));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_RX_HDMI_AUDIO_STATUS, UADP_SDT_P2N, pHdmi_status, spt_AUDIO_tmp1);
			UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;
        //=============================================================
        // AUDIO_ADVANCE_SOUND RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_AUDIO_ADVSOUND_ProcessEnable:
            //pAdvSound_ProcessEnable = (PAUDIO_ST_ADVSOUND_PROCESSENABLE)pArgs;
            UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(char)*AUDIO_STRING_NAME_MAX_LENGTH);
            UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);
            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_ADVSOUND_PROCESSENABLE));
            UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_ADVSOUND_PROCESSENABLE, UADP_SDT_P2N, eType, spt_AUDIO_tmp1);
            UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_ADVSOUND_SubProcessEnable:
            UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(char)*AUDIO_STRING_NAME_MAX_LENGTH);
            UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);
            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_ADVSOUND_SUBPROCESSENABLE));
            UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_ADVSOUND_SUBPROCESSENABLE, UADP_SDT_P2N, eProc, spt_AUDIO_tmp1);
            UADP_SDT_FIN(&spt_AUDIO_tmp[2]);
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_ADVSOUND_SetParam:
            UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(char)*AUDIO_STRING_NAME_MAX_LENGTH);
            UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);
            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_ADVSOUND_SETPARAM));
            UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_ADVSOUND_SETPARAM, UADP_SDT_P2N, eParam, spt_AUDIO_tmp1);
            UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_ADVSND_SetParam:
            UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(char)*AUDIO_STRING_NAME_MAX_LENGTH);
            UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);
            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_ADVSND_SETPARAM));
            UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_ADVSND_SETPARAM, UADP_SDT_P2N, eParam, spt_AUDIO_tmp1);
            UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;
#if 0
        case MApi_CMD_AUDIO_ADVSOUND_GetParam:
            //MS_U32 MApi_AUDIO_ADVSOUND_GetParam(ADVSND_GET_PARAM param)
            //u32Param[0] = return; u32param[1] = param;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[0])), (void*)(&(pu32Des[0])), sizeof(MS_U32));
            break;
#endif

        case MApi_CMD_AUDIO_SetVDS:
            //void MApi_AUDIO_SetVDS(MS_U8 u8enVDS)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetVSPK:
            //void MApi_AUDIO_SetVSPK(MS_U8 u8enVSPK)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetSRS:
            //void MApi_AUDIO_SetSRS(MS_U8 u8enSRS)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetBBE:
            //void MApi_AUDIO_SetBBE(MS_U8 u8enBBE , MS_U8 u8BBEMode)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SETBBE));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_VSPK_WMod:
            //void MApi_AUDIO_VSPK_WMod(MS_U8 u8VSPK_WMode)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_VSPK_SMod:
            //void MApi_AUDIO_VSPK_SMod(MS_U8 u8VSPK_SMode)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SRS_DC:
            //void MApi_AUDIO_SRS_DC(MS_U8 u8SRSenDC)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;              //void MApi_AUDIO_SRS_DC(MS_U8 u8SRSenDC)

        case MApi_CMD_AUDIO_SRS_TruBass:
            //void MApi_AUDIO_SRS_TruBass(MS_U8 u8SRSenTruBass)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SRSTsxtPara:
            //void  MApi_AUDIO_SRS_SetTsxtPara(MS_U8 u8mode, MS_U8 u8value)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SRS_SETTSXTPARA));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetSRSTSHD:
            //void MApi_AUDIO_SetSRSTSHD(MS_U8 u8enTSHD)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_TSHD_TruBass:
            //void  MApi_AUDIO_TSHD_TruBass(MS_U8 u8TSHDenTruBass)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_TSHD_Definition:
            //void  MApi_AUDIO_TSHD_Definition(MS_U8 u8TSHDenDefinition)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_TSHD_Clarity:
            //void  MApi_AUDIO_TSHD_Clarity(MS_U8 u8TSHDenClarity)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SRSSetTshdPara:
            //void  MApi_AUDIO_SRS_SetTshdPara(MS_U8 u8mode, MS_U8 u8value)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SRS_SETTSHDPARA));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_ADVSOUND_SET_ASF_PARAM:
            //MS_BOOL MApi_AUDIO_ADVSOUND_SetASFParam(ADVSND_PARAM param, MS_S16 s16value1, MS_S16 s16value2)
            //s32Param[0] = return; s32param[1] = param; s32param[2] = s16value1; s32param[3] = s16value2
#if 0
			// todo stanley
            ps32Src = (MS_S32*)pArgs;
            ps32Des = (MS_S32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_S32) * 4);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(ps32Src[0])), (void*)(&(ps32Des[0])), sizeof(MS_S32));
#endif
            break;
#if 0
        case MApi_CMD_AUDIO_ADVSOUND_GET_ASF_PARAM:
            //MS_U32 MApi_AUDIO_ADVSOUND_GetASFParam(ADVSND_GET_PARAM param)
            //u32Param[0] = return; u32param[1] = param;
            pu32Src = (MS_U32*)pArgs;
            pu32Des = (MS_U32*)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(MS_U32) * 2);
            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            CPY_to_USER((void*)(&(pu32Src[0])), (void*)(&(pu32Des[0])), sizeof(MS_U32));
            break;
#endif
        case MApi_CMD_AUDIO_ADVSOUND_GetInfo:
            //MS_U32 MApi_AUDIO_ADVSOUND_GetInfo(AdvSound_GET_Type Type)
            //u32Param[0] = return; u32param[1] = Type;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SRS_ADVSOUND_GETINFO));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_DBXTV_SetMode:
            //void MApi_DBXTV_SetMode(EN_DBXTV_TotSonMode totsonmode, EN_DBXTV_TotVolMode totvolmode, EN_DBXTV_TotSurMode totsurmode,MS_U32 enable)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_DBXTV_SETMODE));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SeInit:
            //void MApi_AUDIO_SeInit(void)
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetAdvSndSys:
            UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(char)*AUDIO_STRING_NAME_MAX_LENGTH);
            UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);
            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SETADVSNDSYS));
            UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_SETADVSNDSYS, UADP_SDT_P2N, eType, spt_AUDIO_tmp1);
            UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        /*case MApi_CMD_AUDIO_ADV_SOUND_SET:
            //void MApi_AUDIO_ADVSND_Set_Sound_Param(AdvSound_SET_Type Type, MS_U32 *pParam) // New api for cover below functions
            pADV_Sound_SetParamSrc = (PAUDIO_ST_ADV_SOUND_SETPARAM)pArgs;
            pADV_Sound_SetParamDes = (PAUDIO_ST_ADV_SOUND_SETPARAM)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_ADV_SOUND_SETPARAM));

            pADV_Sound_SetParamDes->pParam = (MS_U32*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)pADV_Sound_SetParamSrc->pParam, sizeof(MS_U32) * 2);

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            break;*/

        case MApi_CMD_AUDIO_Copy_Parameter:
            pCopyParamterSrc = (PAUDIO_ST_COPY_PARAMETER)pArgs;
            pCopyParamterDes = (PAUDIO_ST_COPY_PARAMETER)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_COPY_PARAMETER));
            // printf("\n AUDIO_ST_COPY_PARAMETER size:%d at kernel mode.\n", sizeof(AUDIO_ST_COPY_PARAMETER));

            UADP_SDT_BGN(&spt_AUDIO_tmp1[0], (pCopyParamterDes->parameter_size+pCopyParamterDes->type_size));
            UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);
            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_COPY_PARAMETER));
            UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_COPY_PARAMETER, UADP_SDT_P2N, Parameter_ptr, spt_AUDIO_tmp1);
            UADP_SDT_FIN(&spt_AUDIO_tmp[2]);
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;
        //=============================================================
        // AUDIO_KTV RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_AUDIO_KTV_OP:
            //MS_BOOL MApi_AUDIO_SetKTVInfo(AUDIO_KTV_MODEL modelType, AUDIO_KTV_PARAMTYPE paramType, MS_U32 param1, MS_U32 param2)
            //Status.eType = KTV_SET_INFO; Status.modelType = modelType; Status.paramType = AUD_KTV_ParamType1; Status.param1 = param1; Status.param2 = param2; Status.bRet = Return;
            pKTV_OpSrc = (PAUDIO_ST_KTV_OP)pArgs;
            pKTV_OpDes = (PAUDIO_ST_KTV_OP)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_KTV_OP));

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_KTV_OP));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			if(pKTV_OpDes->eType == KTV_SET_INFO || pKTV_OpDes->eType == KTV_GET_INFO)
				u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
			else
				u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_PlayMenuSound:
            //void MApi_AUDIO_PlayMenuSound(void)
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);
            break;
        //=============================================================
        // AUDIO_MM RELATIONAL API FUNCTION
        //=============================================================
        case MApi_CMD_AUDIO_SetCommAudioInfo:
            //MS_BOOL MApi_AUDIO_SetCommAudioInfo( Audio_COMM_infoType infoType, MS_U32 param1, MS_U32 param2 )
            //u32Param[0] = return u32param[1] = infoType; u32param[2] = param1; u32param[3] = param2;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SET_COMMAUDIOINFO));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_SET_COMMAUDIOINFO, UADP_SDT_AT, Param1, spt_MS_NULL);
			UADP_SDT_KIND(&spt_AUDIO_tmp[2], AUDIO_ST_SET_COMMAUDIOINFO, UADP_SDT_AT, Param2, spt_MS_NULL);
			UADP_SDT_FIN(&spt_AUDIO_tmp[3]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;
#if 0
        case MApi_CMD_AUDIO_SetDecodeParam:
            //MS_BOOL MApi_ADEC_SetDecodeParam( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_U32* Param )
            //Status.DecID = DecId; Status.paramType = paramType; Status.Param = Param;
            pSetDecodeParamSrc = (PAUDIO_ST_SETDECODEPARAM)pArgs;
            pSetDecodeParamDes = (PAUDIO_ST_SETDECODEPARAM)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_SETDECODEPARAM));

            pSetDecodeParamDes->Param = (MS_U32*)buffer_array0;
            CPY_FROM_USER((void*)buffer_array0, (void*)pSetDecodeParamSrc->Param, sizeof(MS_U32) * 6);

            u32Ret = UtopiaIoctl(pInstanceTmp,u32Cmd,(void*)buffer_arg);
            if(!(pSetDecodeParamSrc->paramType == Audio_ParamType_setXPCMDecParam))
            {
                CPY_to_USER((void*)(&(pSetDecodeParamSrc->bStatus)), (void*)(&(pSetDecodeParamDes->bStatus)), sizeof(MS_BOOL));
            }
            break;
#endif
        case MApi_CMD_AUDIO_SetMpegInfo:
            //MS_BOOL _MApi_AUDIO_SetMpegInfo( Audio_MPEG_infoType infoType, MS_U32 param1, MS_U32 param2 );
            //u32Param[0] = return u32param[1] = infoType; u32param[2] = param1; u32param[3] = param2;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SET_MPEGINFO));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_SET_MPEGINFO, UADP_SDT_AT, Param1, spt_MS_NULL);
			UADP_SDT_KIND(&spt_AUDIO_tmp[2], AUDIO_ST_SET_MPEGINFO, UADP_SDT_AT, Param2, spt_MS_NULL);
			UADP_SDT_FIN(&spt_AUDIO_tmp[3]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetAC3Info:
            //MS_BOOL _MApi_AUDIO_SetAC3Info(Audio_AC3_infoType infoType, MS_U32 param1, MS_U32 param2 )
            //u32Param[0] = return u32param[1] = infoType; u32param[2] = param1; u32param[3] = param2;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SET_AC3INFO));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_SET_AC3INFO, UADP_SDT_AT, Param1, spt_MS_NULL);
			UADP_SDT_KIND(&spt_AUDIO_tmp[2], AUDIO_ST_SET_AC3INFO, UADP_SDT_AT, Param2, spt_MS_NULL);
			UADP_SDT_FIN(&spt_AUDIO_tmp[3]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetAC3PInfo:
            //MS_BOOL _MApi_AUDIO_SetAC3PInfo(Audio_AC3P_infoType infoType, MS_U32 param1, MS_U32 param2 );
            //u32Param[0] = return u32param[1] = infoType; u32param[2] = param1; u32param[3] = param2;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SET_AC3PINFO));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_SET_AC3PINFO, UADP_SDT_AT, Param1, spt_MS_NULL);
			UADP_SDT_KIND(&spt_AUDIO_tmp[2], AUDIO_ST_SET_AC3PINFO, UADP_SDT_AT, Param2, spt_MS_NULL);
			UADP_SDT_FIN(&spt_AUDIO_tmp[3]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetAACInfo:
            //MS_BOOL _MApi_AUDIO_SetAACInfo( Audio_AAC_infoType infoType, MS_U32 param1, MS_U32 param2 );
            //u32Param[0] = return u32param[1] = infoType; u32param[2] = param1; u32param[3] = param2;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SET_AACINFO));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_SET_AACINFO, UADP_SDT_AT, Param1, spt_MS_NULL);
			UADP_SDT_KIND(&spt_AUDIO_tmp[2], AUDIO_ST_SET_AACINFO, UADP_SDT_AT, Param2, spt_MS_NULL);
			UADP_SDT_FIN(&spt_AUDIO_tmp[3]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetWMAInfo:
            //MS_BOOL _MApi_AUDIO_SetWmaInfo(Audio_WMA_infoType infoType, MS_U32 param1, MS_U32 param2 );
            //u32Param[0] = return u32param[1] = infoType; u32param[2] = param1; u32param[3] = param2;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SET_WMAINFO));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_SET_WMAINFO, UADP_SDT_AT, Param1, spt_MS_NULL);
			UADP_SDT_KIND(&spt_AUDIO_tmp[2], AUDIO_ST_SET_WMAINFO, UADP_SDT_AT, Param2, spt_MS_NULL);
			UADP_SDT_FIN(&spt_AUDIO_tmp[3]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetDTSCommonCtrl:
            //MS_BOOL _MApi_AUDIO_SetDTSCommonCtrl( Audio_DTS_infoType infoType, Audio_DTS_CtrlType ctrl_type );
            //u32Param[0] = return u32param[1] = infoType u32param[2] = ctrl_type
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SET_DTSCOMMONCTRL));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_MM2_InitAesInfo:
            //MS_BOOL _MApi_AUDIO_MM2_initAesInfo( AUDIO_DEC_ID dec_id );
            //u32Param[0] = return u32param[1] = dec_id
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_MM2_INITAESINFO));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetXPCMParam:
            //void _MApi_AUDIO_SetXPCMParam(XPCM_TYPE audioType, MS_U8 channels, MS_U16 sampleRate, MS_U8 bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock);
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SET_XPCMPARAM));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetCommAudioInfo:
            //long long MApi_AUDIO_GetCommAudioInfo( Audio_COMM_infoType infoType )
            //(long long)u64param[0] = return, u64param[1] = infoType;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_GET_COMMAUDIOINFO));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetMpegInfo:
            //MS_U32 MApi_AUDIO_GetMpegInfo( Audio_MPEG_infoType infoType )
            //u32Param[0] = return u32param[1] = infoType;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_GET_MPEGINFO));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetAC3Info:
            //MS_U32 MApi_AUDIO_GetAC3Info(Audio_AC3_infoType infoType )
            //u32Param[0] = return u32param[1] = infoType;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_GET_AC3INFO));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetAC3PInfo:
            //MS_U32 MApi_AUDIO_GetAC3PInfo( Audio_AC3P_infoType infoType )
            //u32Param[0] = return u32param[1] = infoType;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_GET_AC3PINFO));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetAACInfo:
            //MS_U32 MApi_AUDIO_GetAACInfo( Audio_AAC_infoType infoType )
            //u32Param[0] = return u32param[1] = infoType;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_GET_AACINFO));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetWmaInfo:
            //MS_U32 MApi_AUDIO_GetWmaInfo( Audio_WMA_infoType infoType )
            //u32Param[0] = return u32param[1] = infoType;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_GET_WMAINFO));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetDTSInfo:
            //MS_U32 MApi_AUDIO_GetDTSInfo( Audio_DTS_infoType infoType )
            //u32Param[0] = return u32param[1] = infoType;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_GET_DTSINFO));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_XPCM_Param:
            //MS_U8 MApi_AUDIO_XPCM_Param(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate, MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
            //u32Param[0] = return u32param[1] = audioType; u32param[2] = channels, u32param[3] = sampleRate, u32param[4] = bitsPerSample, u32param[5] = blockSize, u32param[6] = samplePerBlock;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_XPCMPARAM));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;


        case MApi_CMD_AUDIO_XPCM2_Param:
            //void MApi_AUDIO_XPCM2_Param(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate, MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
            //u32Param[0] = audioType u32param[1] = channels; u32param[2] = sampleRate, u32param[3] = bitsPerSample, u32param[4] = blockSize, u32param[5] = samplePerBlock
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SET_XPCMPARAM));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_XPCM2_CheckIntStatus:
            //MS_U16 MApi_AUDIO_XPCM2_CheckIntStatus(void)
            //u32Param[1] = return
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_RA8_Param:
            //MS_U8 MApi_AUDIO_RA8_Param(MS_U16 mNumCodecs, MS_U16 mSamples, MS_U16 mSampleRate, MS_U16* Channels, MS_U16* Regions, MS_U16* cplStart, MS_U16* cplQbits, MS_U16* FrameSize)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_RA8_Param));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_RA8_Param, UADP_SDT_P2N, pChannels, spt_MS_U16);
			UADP_SDT_KIND(&spt_AUDIO_tmp[2], AUDIO_ST_RA8_Param, UADP_SDT_P2N, pRegions, spt_MS_U16);
			UADP_SDT_KIND(&spt_AUDIO_tmp[3], AUDIO_ST_RA8_Param, UADP_SDT_P2N, pcplStart, spt_MS_U16);
			UADP_SDT_KIND(&spt_AUDIO_tmp[4], AUDIO_ST_RA8_Param, UADP_SDT_P2N, pcplQbits, spt_MS_U16);
			UADP_SDT_KIND(&spt_AUDIO_tmp[5], AUDIO_ST_RA8_Param, UADP_SDT_P2N, pFrameSize, spt_MS_U16);
			UADP_SDT_FIN(&spt_AUDIO_tmp[6]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_Init:
            //void MApi_AUDIO_Init(En_DVB_decSystemType enDecSystem)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_StartDecode:
            //void MApi_AUDIO_StartDecode(void)
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_StartBroswe:
            //void MApi_AUDIO_StartBrowse(void)
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_StopDecode:
            //void MApi_AUDIO_StopDecode(void)
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_PauseDecode:
            //void MApi_AUDIO_PauseDecode(void)
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_CheckPlayDone:
            //MS_U8 MApi_AUDIO_CheckPlayDone(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetResidualBufferSize:
            //MS_U16 MApi_AUDIO_GetResidualBufferSize(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetPCMBufferSize:
            //MS_U16 MApi_AUDIO_GetPCMBufferSize(MS_U16 u16BitRate)
            //u32Param[0] = return; u32param[1] = u16BitRate;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_GET_PCMBUFFERSIZE));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetPCMBufferSize2:
            //MS_U16 MApi_AUDIO_GetPCMBufferSize2(MS_U16 u16BitRate,MS_U16 u16SampleRate)
            //u32Param[0] = return u32param[1] = u16BitRate; u32param[2] = u16SampleRate;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_GET_PCMBUFFERSIZE2));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetCurrentFrameNumber:
            //MS_U32 MApi_AUDIO_GetCurrentFrameNumber(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetSampleRates:
            //MS_U16 MApi_AUDIO_GetSampleRate(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetBitRates:
            //MS_U32 MApi_AUDIO_GetBitRate(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetLayer:
            //Audio_MP3_LAYER MApi_AUDIO_GetLayer(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_CheckInputRequest:
            //MS_U8 MApi_AUDIO_CheckInputRequest(MS_U32 *pU32WrtAddr, MS_U32 *pU32WrtBytes)
            //Status.u8Status = return; Status.pU32WrtAddr = pU32WrtAddr; Status.pU32WrtBytes = pU32WrtBytes;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_CHECK_INPUT_REQ));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_CHECK_INPUT_REQ, UADP_SDT_P2N, pPHYWrtAddr, spt_MS_PHY_AUDIO);
			UADP_SDT_KIND(&spt_AUDIO_tmp[2], AUDIO_ST_CHECK_INPUT_REQ, UADP_SDT_P2N, pU32WrtBytes, spt_MS_U32);
			UADP_SDT_FIN(&spt_AUDIO_tmp[3]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetInput:
            //void MApi_AUDIO_SetInput(void)
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetSampleRateIndex:
            //void MApi_AUDIO_SetSampleRateIndex(MS_U16 u16Index)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_FileEndNotification:
            //void MApi_AUDIO_FileEndNotification(void)
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_FileEndDataHandle:
            //void MApi_AUDIO_FileEndDataHandle(MS_U32 u32DataLeft)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetPlayTick:
            //MS_U32 MApi_AUDIO_GetPlayTick(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetEsMEMCnt:
            //MS_U16 MApi_AUDIO_GetEsMEMCnt(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetASFParm:
            //void MApi_AUDIO_SetASFParm(WMA_ASF_PARMTYPE parm_type, MS_U32 value)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SET_ASFPARM));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_MM_SetInput:
            //void MApi_AUDIO_MM_SetInput (En_DVB_AudioDecoder AUDDecoderNo, MS_U8 u8IntTag)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_MM_SETINPUT));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_MM_CheckPlayDone:
            //MS_U32 MApi_AUDIO_MM_CheckPlayDone (En_DVB_AudioDecoder AUDDecoderNo)
            //u32Param[0] = return u32param[1] = AUDDecoderNo;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_MM_CHECKPLAYDONE));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_MM_CheckInputRequest:
            //MS_U8 MApi_AUDIO_MM_CheckInputRequest(En_DVB_AudioDecoder AUDDecoderNo, MS_U32 *pU32WrtAddr, MS_U32 *pU32WrtBytes)
            //Status.u8Status = return; Status.AUDDecoderNo = AUDDecoderNo;Status.pU32WrtAddr = pU32WrtAddr; Status.pU32WrtBytes = pU32WrtBytes;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_MM_CHECK_INPUT_REQ));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_MM_CHECK_INPUT_REQ, UADP_SDT_P2N, pPHYWrtAddr, spt_MS_PHY_AUDIO);
			UADP_SDT_KIND(&spt_AUDIO_tmp[2], AUDIO_ST_MM_CHECK_INPUT_REQ, UADP_SDT_P2N, pU32WrtBytes, spt_MS_U32);
			UADP_SDT_FIN(&spt_AUDIO_tmp[3]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_DmaReader_Init:
            //void MApi_AUDIO_DmaReader_Init(SAMPLE_RATE sampleRate)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_DmaReader_AllInput_Init:
            //void MApi_AUDIO_DmaReader_AllInput_Init(void)
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_DmaReader_WritePCM:
                //MS_BOOL MApi_AUDIO_DmaReader_WritePCM(void* buffer, MS_U32 bytes)
                //Status.bStatus = return; Status.pBuffer = buffer; Status.bytes = bytes;
            pDMAReader_WritePCMSrc = (PAUDIO_ST_DMAREADER_WRITEPCM)pArgs;
			pDMAReader_WrtiePCMDes = (PAUDIO_ST_DMAREADER_WRITEPCM)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(AUDIO_ST_DMAREADER_WRITEPCM));

			UADP_SDT_BGN(&spt_AUDIO_tmp1[0], pDMAReader_WrtiePCMDes->bytes);
			UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_DMAREADER_WRITEPCM));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_DMAREADER_WRITEPCM, UADP_SDT_P2N, pBuffer, spt_AUDIO_tmp1);
			UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_DmaWrite_Init:
            //void MApi_AUDIO_DmaWriter_Init(void)
			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,NULL,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_USBPCM_Enable:
            //void MApi_AUDIO_USBPCM_Enable(MS_BOOL bEnable)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_USBPCM_SetFlag:
            //void MApi_AUDIO_USBPCM_SetFlag(MS_BOOL bEnable)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_USBPCM_GetFlag:
            //MS_BOOL MApi_AUDIO_USBPCM_GetFlag(void)
            //u32param[1] = return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_USBPCM_GetMemInfo:
            //void MApi_AUDIO_USBPCM_GetMemInfo(AUDIO_UPLOAD_INFO *uploadInfo)
            //u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_ST_UPLOAD_INFO, NULL, buffer_arg,sizeof(buffer_arg));
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_UPLOAD_INFO));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_PCMCapture_Init:
            //MS_BOOL MApi_AUDIO_PCMCapture_Init(const AUDIO_DEVICE_TYPE eID, const AUDIO_CAPTURE_SOURCE_TYPE eSource)
            // Status.bStatus = return; Status.eID = eID; Status.eSource = eSource;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_PCMCAPTURE_INIT));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_PCMCapture_Start:
            //MS_BOOL MApi_AUDIO_PCMCapture_Start(const AUDIO_DEVICE_TYPE eID)
            // Status.bStatus = Return; Status.eID = eID;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_PCMCAPTURE_INIT));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_PCMCapture_Stop:
            //MS_BOOL MApi_AUDIO_PCMCapture_Stop(const AUDIO_DEVICE_TYPE eID)
            // Status.bStatus = Return; Status.eID = eID;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_PCMCAPTURE_INIT));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_PCMCapture_Read:
            //MS_BOOL MApi_AUDIO_PCMCapture_Read(const AUDIO_DEVICE_TYPE eID, void *buffer, const MS_U32 bytes)
            // Status.bStatus = Return;Status.eID = eID;Status.pBuffer = buffer;Status.bytes = bytes;
			pPCMCaptureReadSrc = (PAUDIO_ST_PCMCAPTURE_READ)pArgs;
			pPCMCaptureReadDes = (PAUDIO_ST_PCMCAPTURE_READ)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(pPCMCaptureReadDes->bytes));

            UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(pPCMCaptureReadDes->bytes));
			UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_PCMCAPTURE_READ));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_PCMCAPTURE_READ, UADP_SDT_P2N, pBuffer,spt_AUDIO_tmp1);
			UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

			u32Ret=UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_VoIP_Config:
            //MS_BOOL MApi_AUDIO_VoIP_Config(AUDIO_VoIP_CONFIG_TYPE configType, MS_U32 param1, MS_U32 param2)
            //u32Param[0] = return u32param[1] = configType; u32param[2] = param1; u32param[3] = param2;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_VOIP_CONFIG));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_ALSA_Check:
            //MS_BOOL MApi_AUDIO_ALSA_Check(void)
            //u32param[1] = Return;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_ALSA_Enable:
            //MS_BOOL MApi_AUDIO_ALSA_Enable(MS_BOOL bEnable)
            //u32Param[0] = return u32param[1] = bEnable;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_ALSA_ENABLE));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_UNI_CheckDecodeDone:
            //MS_BOOL MApi_AUDIO_UNI_CheckDecodeDone(MS_U32 *pU32WrtAddr, MS_U32 *pU32WrtBytes)
            // Status.bStatus = return ;Status.pU32WrtAddr = pU32WrtAddr; Status.pU32WrtBytes = pU32WrtBytes;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_UNI_CHECK_DECODEDONE));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_UNI_CHECK_DECODEDONE, UADP_SDT_P2N, pPHYWrtAddr, spt_MS_PHY_AUDIO);
			UADP_SDT_KIND(&spt_AUDIO_tmp[2], AUDIO_ST_UNI_CHECK_DECODEDONE, UADP_SDT_P2N, pU32WrtBytes, spt_MS_U32);
			UADP_SDT_FIN(&spt_AUDIO_tmp[3]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_UNI_SetOutput:
            //void MApi_AUDIO_UNI_SetOutput (MS_U32 PCMOutCnt)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_UNI_Set_PCMInputWriteAddr:
            //void MApi_AUDIO_UNI_Set_PCMInputWriteAddr (MS_U32 PCMIn_Wptr)
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(MS_U32));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,NULL,NULL,0);
            break;

        case MApi_CMD_AUDIO_UNI_Get_OutPCMLevel:
            //MS_U32 MApi_AUDIO_UNI_Get_OutPCMLevel (void)
            //u32param[1] = return
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(u32param2_t));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_RingTask:
            //void MApi_AUDIO_RingTask(audioRingTask* auRingTask)
			UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(audioBufMonitor));
			UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(audioRingTask));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], audioRingTask, UADP_SDT_ES, _stBufInfo, spt_AUDIO_tmp1);
			UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_Ring_DataTransfer:
            //void MApi_AUDIO_Ring_DataTransfer(audioRingTask* auRingTask, audioRingTask* auRingTask_1)
			UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(audioBufMonitor));
			UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

			UADP_SDT_BGN(&spt_AUDIO_tmp2[0], sizeof(audioRingTask));
			UADP_SDT_KIND(&spt_AUDIO_tmp2[1], audioRingTask, UADP_SDT_ES, _stBufInfo, spt_AUDIO_tmp1);
			UADP_SDT_FIN(&spt_AUDIO_tmp2[2]);

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_RING_DATA_TRANS));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_RING_DATA_TRANS, UADP_SDT_P2N, pauRingTask, spt_AUDIO_tmp2);
			UADP_SDT_KIND(&spt_AUDIO_tmp[2], AUDIO_ST_RING_DATA_TRANS, UADP_SDT_P2N, pauRingTask_1, spt_AUDIO_tmp2);
			UADP_SDT_FIN(&spt_AUDIO_tmp[3]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_MM2_CheckAesInfo:
            //MS_BOOL MApi_AUDIO_MM2_checkAesInfo( AUDIO_DEC_ID  dec_id, AES_INFO *aes_info )
            // Status.bStatus = FALSE; Status.dec_id = dec_id; Status.paes_info = aes_info;
			UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(AES_INFO));
			UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_MM2_CHECKAESINFO));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_MM2_CHECKAESINFO, UADP_SDT_P2N, paes_info, spt_AUDIO_tmp1);
			UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_MM2_InputAesFinished:
            //MS_BOOL MApi_AUDIO_MM2_inputAesFinished( AUDIO_DEC_ID dec_id, MS_U32 es_size, MS_BOOL ptsExist, MS_U64 pts )
            // Status.bStatus = Return; Status.dec_id = dec_id; Status.es_size = es_size; Status.ptsExist = ptsExist; Status.pts = pts;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_MM2_INPUTAES_FINISH));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_SetAudioParam2:
            //MS_BOOL MApi_AUDIO_SetAudioParam2( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_U32 Param )
            //u32Param[0] = return u32param[1] = DecId; u32param[2] = paramType; u32param[3] = Param;
            pSetAudioParam2Src = (PAUDIO_ST_SET_AUDIOPARAM2)pArgs;
            pSetAudioParam2Des = (PAUDIO_ST_SET_AUDIOPARAM2)buffer_arg;
            CPY_FROM_USER((void *)&pSetAudioParam2Des->eParamType, (void *)&pSetAudioParam2Src->eParamType, sizeof(MS_U32));

            if (pSetAudioParam2Des->eParamType == Audio_ParamType_CommonCmd)
            {
                UADP_SDT_BGN(&spt_AUDIO_tmp2[0], sizeof(AUDIO_OUTPUT_DEVICE_SELECTION_t));
                UADP_SDT_FIN(&spt_AUDIO_tmp2[1]);

                UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(AUDIO_COMMON_CMD_t));
                UADP_SDT_KIND(&spt_AUDIO_tmp1[1], AUDIO_COMMON_CMD_t, UADP_SDT_P2N, pData, spt_AUDIO_tmp2);
                UADP_SDT_FIN(&spt_AUDIO_tmp1[2]);

                UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SET_AUDIOPARAM2));
                UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_SET_AUDIOPARAM2, UADP_SDT_P2N, Param, spt_AUDIO_tmp1);
                UADP_SDT_FIN(&spt_AUDIO_tmp[2]);
            }
            else
            {
                UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_SET_AUDIOPARAM2));
                UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_SET_AUDIOPARAM2, UADP_SDT_AT, Param, spt_MS_NULL);
                UADP_SDT_FIN(&spt_AUDIO_tmp[2]);
            }

            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetAudioInfo2:
			pGetAudioInfo2Src = (PAUDIO_ST_GETAUDIOINFO2)pArgs;
            pGetAudioInfo2Des = (PAUDIO_ST_GETAUDIOINFO2)buffer_arg;
            CPY_FROM_USER((void *)&pGetAudioInfo2Des->infoType, (void *)&pGetAudioInfo2Src->infoType, sizeof(MS_U32));

			if (pGetAudioInfo2Des->infoType == Audio_infoType_esBuf_req) {
				UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(Audio_MM_Data_Request));
				UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);
			}
			else if (pGetAudioInfo2Des->infoType == Audio_infoType_UNI_PCM1_Info)
			{
                UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(Audio_MM_PCM_Info));
                UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);
            }
            else if (pGetAudioInfo2Des->infoType == Audio_infoType_hashkey)
            {
                UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(Audio_Hashkey_Info));
                UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);
            }
            else
            {
                UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(MS_U32));
            	UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);
            }

			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_GETAUDIOINFO2));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_GETAUDIOINFO2, UADP_SDT_P2N, pInfo, spt_AUDIO_tmp1);
			UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetDDRInfo:
            //MS_U32 MApi_AUDIO_GetDDRInfo(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo)
            //u32Param[0] = return u32param[1] = DecId; u32param[2] = DDRInfo;
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_GET_DDRINFO));
			UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_GetCaps:
			UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_GETCAPS));
			UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_GETCAPS, UADP_SDT_P2N, pu32Caps, spt_MS_U32);
			UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

			u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_AUDIO_tmp,spt_AUDIO_tmp,NULL,0);
            break;

        case MApi_CMD_AUDIO_PcmOpen:
        {
            UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(AUDIO_PCM_INFO_t));
            UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_PCM_OPEN));
            UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_PCM_OPEN, UADP_SDT_P2N, pData, spt_AUDIO_tmp1);
            UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_AUDIO_tmp, spt_AUDIO_tmp, NULL, 0);
            break;
        }

        case MApi_CMD_AUDIO_PcmClose:
        {
            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_PCM_CLOSE));
            UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_AUDIO_tmp, spt_AUDIO_tmp, NULL, 0);
            break;
        }

        case MApi_CMD_AUDIO_PcmStart:
        {
            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_PCM_START));
            UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_AUDIO_tmp, spt_AUDIO_tmp, NULL, 0);
            break;
        }

        case MApi_CMD_AUDIO_PcmStop:
        {
            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_PCM_STOP));
            UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_AUDIO_tmp, spt_AUDIO_tmp, NULL, 0);
            break;
        }

        case MApi_CMD_AUDIO_PcmSet:
        {
            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_PCM_SET));
            UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_PCM_SET, UADP_SDT_P2N, pData, spt_MS_U32);
            UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_AUDIO_tmp, spt_AUDIO_tmp, NULL, 0);
            break;
        }

        case MApi_CMD_AUDIO_PcmGet:
        {
            pPcmGetSrc = (PAUDIO_ST_PCM_GET)pArgs;
            pPcmGetDes = (PAUDIO_ST_PCM_GET)buffer_arg;
            CPY_FROM_USER((void*)buffer_arg, pArgs, sizeof(PAUDIO_ST_PCM_GET));

            if (pPcmGetDes->u32Cmd == AUDIO_PCM_CMD_ALL)
            {
                UADP_SDT_BGN(&spt_AUDIO_tmp1[0], sizeof(AUDIO_PCM_INFO_t));
                UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

                UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_PCM_GET));
                UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_PCM_GET, UADP_SDT_P2N, pData, spt_AUDIO_tmp1);
                UADP_SDT_FIN(&spt_AUDIO_tmp[2]);
            }
            else
            {
                UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_PCM_GET));
                UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_PCM_GET, UADP_SDT_P2N, pData, spt_MS_U32);
                UADP_SDT_FIN(&spt_AUDIO_tmp[2]);
            }

            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_AUDIO_tmp, spt_AUDIO_tmp, NULL, 0);
            break;
        }

        case MApi_CMD_AUDIO_PcmRead:
        {
            pPcmReadSrc = (PAUDIO_ST_PCM_READ)pArgs;
            pPcmReadDes = (PAUDIO_ST_PCM_READ)buffer_arg;
            CPY_FROM_USER((void *)&pPcmReadDes->u32Size, (void *)&pPcmReadSrc->u32Size, sizeof(MS_U32));

            UADP_SDT_BGN(&spt_AUDIO_tmp1[0], pPcmReadDes->u32Size);
            UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_PCM_READ));
            UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_PCM_READ, UADP_SDT_P2N, pBuf, spt_AUDIO_tmp1);
            UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_AUDIO_tmp, spt_AUDIO_tmp, NULL, 0);
            break;
        }

        case MApi_CMD_AUDIO_PcmWrite:
        {
            pPcmWriteSrc = (PAUDIO_ST_PCM_WRITE)pArgs;
            pPcmWriteDes = (PAUDIO_ST_PCM_WRITE)buffer_arg;
            CPY_FROM_USER((void *)&pPcmWriteDes->u32Size, (void *)&pPcmWriteSrc->u32Size, sizeof(MS_U32));

            UADP_SDT_BGN(&spt_AUDIO_tmp1[0], pPcmWriteDes->u32Size);
            UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_PCM_WRITE));
            UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_PCM_WRITE, UADP_SDT_P2N, pBuf, spt_AUDIO_tmp1);
            UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_AUDIO_tmp, spt_AUDIO_tmp, NULL, 0);
            break;
        }

        case MApi_CMD_AUDIO_PcmFlush:
        {
            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_PCM_FLUSH));
            UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_AUDIO_tmp, spt_AUDIO_tmp, NULL, 0);
            break;
        }

        case MApi_CMD_AUDIO_EncoderOpen:
        {
            UADP_SDT_BGN(&spt_AUDIO_tmp1[0], AUDIO_STRNAME_LENGTH);
            UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_ENCODER_OPEN));
            UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_ENCODER_OPEN, UADP_SDT_P2N, pName, spt_AUDIO_tmp1);
            UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_AUDIO_tmp, spt_AUDIO_tmp, NULL, 0);
            break;
        }

        case MApi_CMD_AUDIO_EncoderClose:
        {
            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_ENCODER_CLOSE));
            UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_AUDIO_tmp, spt_AUDIO_tmp, NULL, 0);
            break;
        }

        case MApi_CMD_AUDIO_EncoderStart:
        {
            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_ENCODER_START));
            UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_AUDIO_tmp, spt_AUDIO_tmp, NULL, 0);
            break;
        }

        case MApi_CMD_AUDIO_EncoderStop:
        {
            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_ENCODER_STOP));
            UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_AUDIO_tmp, spt_AUDIO_tmp, NULL, 0);
            break;
        }

        case MApi_CMD_AUDIO_EncoderIoctl:
        {
            UADP_SDT_BGN(&spt_AUDIO_tmp1[0], AUDIO_STRCMD_LENGTH);
            UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_ENCODER_IOCTL));
            UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_ENCODER_IOCTL, UADP_SDT_P2N, pCmd, spt_AUDIO_tmp1);
            UADP_SDT_KIND(&spt_AUDIO_tmp[2], AUDIO_ST_ENCODER_IOCTL, UADP_SDT_P2N, pData, spt_MS_U32);
            UADP_SDT_FIN(&spt_AUDIO_tmp[3]);

            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_AUDIO_tmp, spt_AUDIO_tmp, NULL, 0);
            break;
        }

        case MApi_CMD_AUDIO_EncoderRead:
        {
            pEncoderReadSrc = (PAUDIO_ST_ENCODER_READ)pArgs;
            pEncoderReadDes = (PAUDIO_ST_ENCODER_READ)buffer_arg;
            CPY_FROM_USER((void *)&pEncoderReadDes->u32Size, (void *)&pEncoderReadSrc->u32Size, sizeof(MS_U32));

            UADP_SDT_BGN(&spt_AUDIO_tmp1[0], pEncoderReadDes->u32Size);
            UADP_SDT_FIN(&spt_AUDIO_tmp1[1]);

            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_ENCODER_READ));
            UADP_SDT_KIND(&spt_AUDIO_tmp[1], AUDIO_ST_ENCODER_READ, UADP_SDT_P2N, pBuf, spt_AUDIO_tmp1);
            UADP_SDT_FIN(&spt_AUDIO_tmp[2]);

            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_AUDIO_tmp, spt_AUDIO_tmp, NULL, 0);
            break;
        }

        case MApi_CMD_AUDIO_EncoderFlush:
        {
            UADP_SDT_BGN(&spt_AUDIO_tmp[0], sizeof(AUDIO_ST_ENCODER_FLUSH));
            UADP_SDT_FIN(&spt_AUDIO_tmp[1]);

            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_AUDIO_tmp, spt_AUDIO_tmp, NULL, 0);
            break;
        }

        default:
            AUDIO_ADP_MSG("AUDIOADP - Unknown commend!!!\n");
            break;
    }


	return u32Ret;

   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}

#endif
