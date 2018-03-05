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

#ifndef _DEV_MAD_H_
#define _DEV_MAD_H_

#include "MsTypes.h"
#include "halAUDIO.h"
#include "drvAUDIO_if.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define XPCM_PARA_BASEADDR  0x27F0 //0x3F00
#define ENC_BUF_SIZE 6;

#define RTSP_MEM_ADDR   0x10000 //the base addr is a temporary space borrowed from MAD ES2(assuming it is idle in streaming application)
#define RTSP_MEM_SIZE   0x1000

#define DMA_READER_TEMP_BUFFER_ADDR   0x052200  //ring tone read buffer (Size: 4KB)

#define ACMOD_CH_MASK_C        0x0001
#define ACMOD_CH_MASK_L        0x0002
#define ACMOD_CH_MASK_R        0x0004
#define ACMOD_CH_MASK_LS       0x0008
#define ACMOD_CH_MASK_RS       0x0010   
#define ACMOD_CH_MASK_LFE      0x0020
#define ACMOD_CH_MASK_LF       0x0040
#define ACMOD_CH_MASK_RF       0x0080
#define ACMOD_CH_MASK_NULL     0x0000

//=====================================================
// AUDIO_MAD System Relational Hal Function
//=====================================================
MS_U32  HAL_MAD_Read_DSP_sram(MS_U16 dsp_addr,AUDIO_DSP_MEMORY_TYPE dsp_memory_type);
MS_BOOL HAL_MAD_Write_DSP_sram(MS_U16 dsp_addr, MS_U32 value, AUDIO_DSP_MEMORY_TYPE dsp_memory_type);
void    HAL_MAD_Init(void);
void    HAL_MAD_SetMemInfo(void);
void    HAL_MAD_SetDecCmd(AU_DVB_DECCMD decCmd);
void    HAL_MAD_SetDecCmd2(AUDIO_DEC_ID DecId, AU_DVB_DECCMD u8DecCmd);
void    HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD decCmd);
//void HAL_AUDIO_SetEncodeDoneFlag(MS_BOOL bSet);
void    HAL_MAD_SetEncCmd(AU_DVB_ENCCMD u8EncCmd);
void    HAL_MAD_EncodeInit(AU_DVB_ENCBR u8EncBr, MS_U8 u8EncFrameNum);
void    HAL_AUDIO_GetEncodeFrameInfo(AU_DVB_ENC_FRAME_INFO *FrameInfo);
AU_DVB_DECCMD   HAL_MAD_GetDecCmd(En_DVB_AudioDecoder AdoDecoderNo);
void    HAL_MAD_DisEn_MIUREQ(void);
void    HAL_MAD_Dis_MIUREQ(void);
void    HAL_MAD_RSTMAD_DisEn_MIUREQ(void);
MS_U8   HAL_MAD_GetReloadCodeAck(void);
MS_U8   HAL_MAD_GetLoadCodeAck(void);
MS_U8   HAL_MAD_Rpt_DTVES(void);
void    HAL_MAD_SetStreamID_Mod(MS_U8 stream_id_mod);
void    HAL_MAD_SetAdMixMode (AD_MIX_MODE u8Mix_mode, MS_U8  u8En_mix);
void    HAL_MAD_SetDspIDMA(void);
void    HAL_MAD_SetMcuCmd(MS_U8 cmd);
void    HAL_MAD_SetPIOCmd(MS_U8 cmd);
void    HAL_MAD_TriggerPIO8(void);
void    HAL_MAD_DvbFLockSynthesizer_En(void);
MS_BOOL HAL_MAD_ReLoadCode(AUDIO_DSP_CODE_TYPE u8Type1);
MS_BOOL HAL_MAD_LoadCode(AUDIO_DSP_CODE_TYPE u8Type);
MS_BOOL HAL_AUDIO_SetCertMode( Audio_CertMode CertType,Audio_CertEnable enCert );
MS_BOOL HAL_MAD_MM2_initAesInfo( AUDIO_DEC_ID  dec_id );
MS_BOOL HAL_MAD_MM2_checkAesInfo( AUDIO_DEC_ID  dec_id, AES_INFO *aes_info );
MS_BOOL HAL_MAD_MM2_inputAesFinished( AUDIO_DEC_ID dec_id, MS_U32 es_size, MS_BOOL ptsExist, MS_U64 pts );
//=====================================================
// Function
//=====================================================
MS_BOOL HAL_MAD_SetCommInfo( Audio_COMM_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_U64 HAL_MAD_GetCommInfo( Audio_COMM_infoType infoType );
MS_BOOL HAL_MAD_SetMpegInfo( Audio_MPEG_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_U32  HAL_MAD_GetMpegInfo( Audio_MPEG_infoType infoType );
MS_BOOL HAL_MAD_SetWmaInfo( Audio_WMA_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_U32  HAL_MAD_GetWmaInfo( Audio_WMA_infoType infoType );
MS_BOOL HAL_MAD_SetAC3Info( Audio_AC3_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_U32  HAL_MAD_GetAC3Info( Audio_AC3_infoType infoType );
MS_BOOL HAL_MAD_SetAC3PInfo( Audio_AC3P_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_U32  HAL_MAD_GetAC3PInfo( Audio_AC3P_infoType infoType );
MS_BOOL HAL_MAD_SetDTSCommonCtrl( Audio_DTS_infoType infoType, Audio_DTS_CtrlType ctrl_type );
MS_U32  HAL_MAD_GetDTSInfo( Audio_DTS_infoType infoType );
MS_BOOL HAL_MAD_SetAACInfo( Audio_AAC_infoType infoType, MS_VIRT param1, MS_VIRT param2 );
MS_U32  HAL_MAD_GetAACInfo( Audio_AAC_infoType infoType );
MS_BOOL HAL_MAD_SetDtsInfo( Audio_DTS_infoType infoType, MS_U32 param1, MS_U32 param2 );
MS_U32  HAL_MAD_GetDtsInfo( Audio_DTS_infoType infoType );
MS_BOOL HAL_MAD_SetRA8LBRInfo( Audio_RA8LBR_infoType infoType, MS_U32 param1, MS_U32 param2 );
MS_U32  HAL_MAD_GetRA8LBRInfo( Audio_RA8LBR_infoType infoType );

//=====================================================
// AUDIO_MAD MEPG Relational Hal Function
//=====================================================
MS_U32  HAL_MAD_MPEG_GetHeaderInfo(void);
MS_U16  HAL_MAD_MPEG_GetSampleRate(void);
MS_U16  HAL_MAD_MPEG_GetBitRate(void);
MPEG_SOUND_MODE   HAL_MAD_MPEG_GetSoundMode(void);

//=====================================================
// AUDIO_MAD Dolby Relational Hal Function
//=====================================================
void    HAL_MAD_Monitor_DDPlus_SPDIF_Rate( void );

//=====================================================
// AUDIO_MAD WMA Relational Hal Function
//=====================================================
void    HAL_MAD_WMA_SetASFParm(WMA_ASF_PARMTYPE parm_type, MS_U32 value);

//=====================================================
// AUDIO_MAD AAC Relational Hal Function
//=====================================================

//=====================================================
// AUDIO_MAD OTHERS Relational Driver Function
//=====================================================
MS_U16  HAL_MAD_XPCM_GetParaBase(void);
MS_U8   HAL_MAD_RA8_setParam(MS_U16 mNumCodecs, MS_U16 mSamples, MS_U16 mSampleRate,
                           MS_U16* Channels, MS_U16* Regions, MS_U16* cplStart, MS_U16* cplQbits,
                           MS_U16* FrameSize);

MS_U16  HAL_MAD_Ra8_GetParaBase(void);
extern  MS_BOOL HAL_AUDSP_CheckDecIdmaReady(MS_U8 u8IdmaChk_type );

MS_U8   HAL_MAD_XPCM_setParam(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate,
                                                    MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock);

//=====================================================
// AUDIO_MAD MM Mode Hal Function
//=====================================================
MS_U32  HAL_MAD_ReadTimeStamp(void);

//====== STAOS PIP START 2012/02/02 ========

MS_BOOL HAL_MAD_SetAudioParam2( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_VIRT Param );
MS_BOOL HAL_MAD_GetAudioInfo2( AUDIO_DEC_ID DecId, Audio_InfoType infoType, void * pInfo );
MS_BOOL HAL_MAD_GetAudioCapability(AUDIO_DEC_ID eDecId, AudioDecStatus_t * p_AudioDecStatus);

//====== STAOS PIP END 2012/02/02 ========

MS_S32 HAL_MAD_CommonCmd_TrasferData(void *pSrc, MS_U32 u32SrcSize, void *pDst, MS_U32 u32DstSzie);
MS_S32 HAL_MAD_SetAudioOutputDeviceSelection(AUDIO_OUTPUT_DEVICE_SELECTION_t * pAudioOutputDeviceSelection);
MS_S32 HAL_MAD_SpecifyDigitalOutputCodec(AUDIO_SPECIFIED_DIGITAL_OUTPUT_t * pAudioSpecifiedDigitalOutput, DIGITAL_OUTPUT_TYPE u32DigitalOutputType);
MS_S32 HAL_MAD_DAP_Control(AUDIO_DAP_CONTROL_t * pDapControl);
MS_S32 HAL_MAD_GetDecoderInfo(AUDIO_DECODER_INFO_t * pDecoderInfo);
MS_S32 HAL_MAD_CommonCmd(AUDIO_COMMON_CMD_t * cmd);

#endif // _DEV_MAD_H_