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

#ifndef _DEV_AUDIO_H_
#define _DEV_AUDIO_H_
#include "MsCommon.h"
#include "halAUR2.h"


#include "MsTypes.h"
#include "drvAUDIO_if.h"
#include "../../../drv/audio/internal/drvAUDIO_internal.h"
#include "../../drv/mbx/apiMBX_St.h"
#include "../../drv/mbx/apiMBX.h"



//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#define AUDIO_SHM_NAME    "Linux AUDIO driver"
#define AUDIO_TEE_INFO_SHM_NAME    "AUDIO TEE INFO SHM"

#define AUD_CHK_DSP_READ_RDY            0x08
#define AUD_CHK_DSP_WRITE_RDY           0x10
#define AUD_R2_DMA_BURST_BYTES          0x4C

#define AUMASK_BIT0    0x0001
#define AUMASK_BIT1    0x0002
#define AUMASK_BIT2    0x0004
#define AUMASK_BIT3    0x0008
#define AUMASK_BIT4    0x0010
#define AUMASK_BIT5    0x0020
#define AUMASK_BIT6    0x0040
#define AUMASK_BIT7    0x0080

#define au_bit0_(val)  ((MS_U8)(val & AUMASK_BIT0))
#define au_bit1_(val)  ((MS_U8)(val & AUMASK_BIT1))
#define au_bit2_(val)  ((MS_U8)(val & AUMASK_BIT2))
#define au_bit3_(val)  ((MS_U8)(val & AUMASK_BIT3))
#define au_bit4_(val)  ((MS_U8)(val & AUMASK_BIT4))
#define au_bit5_(val)  ((MS_U8)(val & AUMASK_BIT5))
#define au_bit6_(val)  ((MS_U8)(val & AUMASK_BIT6))
#define au_bit7_(val)  ((MS_U8)(val & AUMASK_BIT7))

#define R1BYTE(u32Addr, u8mask) \
    (READ_BYTE ((MS_VIRT)_gMIO_MapBase + ((u32Addr) << 1) - ((u32Addr) & 1)) & (u8mask))

#define W1BYTE(u32Addr, u8Val, u8mask)      \
    (WRITE_BYTE((MS_VIRT)_gMIO_MapBase + ((u32Addr) << 1) - ((u32Addr) & 1), (R1BYTE(u32Addr, 0xFF) & ~(u8mask)) | ((u8Val) & (u8mask))))

// u32Addr must be 16bit aligned
#define R2BYTE(u32Addr, u16mask)  \
    (READ_WORD ((MS_VIRT)_gMIO_MapBase + ((u32Addr) << 1)) & (u16mask))

// u32Addr must be 16bit aligned
#define W2BYTE(u32Addr, u16Val, u16mask)    \
    (WRITE_WORD((MS_VIRT)_gMIO_MapBase + ((u32Addr) << 1), (R2BYTE(u32Addr, 15:0) & ~(u16mask)) | ((u16Val) & (u16mask))))

///////////////////////////////////////////////////////////////
#define _AU_AbsWriteByte( u32Reg, u8Val )   \
     do {   \
     (WRITE_BYTE((MS_VIRT)_gMIO_MapBase + ((u32Reg) << 1) - ((u32Reg) & 1), u8Val)); \
          }while(0)

#define _AU_AbsWrite2Byte( u32Reg, u16Val ) \
     do {   \
     (WRITE_WORD((MS_VIRT)_gMIO_MapBase + ((u32Reg) << 1), u16Val)); \
          }while(0)

#define _AU_AbsWriteMaskByte( u32Reg, u8Mask, u8Val )   \
    do {    \
    (WRITE_BYTE((MS_VIRT)_gMIO_MapBase + ((u32Reg) << 1) - ((u32Reg) & 1), (R1BYTE((u32Reg), 0xFF) & ~(u8Mask)) | ((u8Val) & (u8Mask))));    \
        }while(0)

#define _AU_AbsReadByte( u32Reg) (READ_BYTE ((MS_VIRT)_gMIO_MapBase + ((u32Reg) << 1) - ((u32Reg) & 1)))
#define _AU_AbsRead2Byte( u32Reg)  (READ_WORD ((MS_VIRT)_gMIO_MapBase + ((u32Reg) << 1)) )

#define HAL_AUDIO_DecReadReg    HAL_AUDIO_ReadReg
#define HAL_AUDIO_SeReadReg    HAL_AUDIO_ReadReg
#define HAL_AUDIO_DecReadByte   HAL_AUDIO_ReadByte
#define HAL_AUDIO_SeReadByte   HAL_AUDIO_ReadByte
#define HAL_AUDIO_DecWriteByte  HAL_AUDIO_WriteByte
#define HAL_AUDIO_SeWriteByte  HAL_AUDIO_WriteByte
#define HAL_AUDIO_DecWriteMaskByte  HAL_AUDIO_WriteMaskByte
#define HAL_AUDIO_SeWriteMaskByte  HAL_AUDIO_WriteMaskByte
#define HAL_AUDIO_DecWriteReg  HAL_AUDIO_WriteReg
#define HAL_AUDIO_SeWriteReg  HAL_AUDIO_WriteReg
#define HAL_AUDIO_DecWriteMaskReg  HAL_AUDIO_WriteMaskReg
#define HAL_AUDIO_SeWriteMaskReg  HAL_AUDIO_WriteMaskReg

//Enable MS10 DDT Dual decode
#define MS10_DDT_DUAL

#if (defined(CONFIG_MBOOT))
    #define AUDIO_DELAY1US(x)                {int xxxx, jjjj, iiii=0; for(xxxx=0; xxxx<x; xxxx++) for(jjjj=0; jjjj<10; jjjj++) iiii++; }
    #define AUDIO_DELAY1MS(x)                {int xxxx, jjjj, iiii=0; for(xxxx=0; xxxx<x; xxxx++) for(jjjj=0; jjjj<10000; jjjj++) iiii++; }
#elif (defined(MCU_AEON))
    #define AUDIO_DELAY1US(x)                MsOS_DelayTaskUs(x)
    #define AUDIO_DELAY1MS(x)                AUDIO_DELAY1US(x*1000)
#else
    #define AUDIO_DELAY1US(x)                MsOS_DelayTaskUs(x)
    #define AUDIO_DELAY1MS(x)                MsOS_DelayTask(x)
#endif

#define AUDIO_HW_DMA_READER1_SUPPORT    1
#define AUDIO_HW_DMA_READER2_SUPPORT    1
#define AUDIO_SW_DMA_READER1_SUPPORT    0
#define AUDIO_R2_DMA_READER1_SUPPORT    0
#define AUDIO_R2_DMA_READER2_SUPPORT    0
#define AUDIO_PCM_CAPTURE1_SUPPORT    1
#define AUDIO_PCM_CAPTURE2_SUPPORT    1
#define AUDIO_HW_DMA_WRITER1_SUPPORT    0

#define AUDIO_MP3_ENCODER1_SUPPORT    0
#define AUDIO_MP3_ENCODER2_SUPPORT    0

#define ASND_R2_SUPPORT    0

#define AUDIO_AD_SUPPORT    1

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_S64 curPTS;
    MS_U32 os_Time;
    MS_BOOL set2Clr;
} AUDIO_PTS_INFO;

typedef struct
{
    MS_U32 Mem_addr;
    MS_U32 Mem_size;
} AUDIO_RTSP_INFO;

/// Audio register structure
typedef struct
{
    MS_U32  u32Addr;                    ///< Reg address
    MS_U8   u8Mask;                     ///<Reg Mask
    MS_U8   u8Value;                    ///<Reg Value
}AUDIO_REG_TYPE, *PAUDIO_REG_TYPE;

/// Audio register structure2
typedef struct
{
    MS_U32   u32Addr;                    ///< Reg address2
    MS_U16   u16Mask;                     ///<Reg Mask2
    MS_U16   u16Value;                    ///<Reg Value2
}AUDIO_REG_TYPE2, *PAUDIO_REG_TYPE2;

//================================================================
//  Structure
//================================================================

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_AUDIO_InitialVars(void);
MS_BOOL HAL_AUDIO_DeInitialVars(void);
void HAL_AUDIO_SET_INIT_FLAG(MS_BOOL bSet);
MS_BOOL HAL_AUDIO_GET_INIT_FLAG(void);
MS_BOOL HAL_AUDIO_Mutex_Init(void);
MS_BOOL HAL_AUDIO_Mutex_DeInit(void);
void    HAL_AUDIO_Set_MMIO_BASE(MS_VIRT _MMIO_MapBase);
AU_CHIP_TYPE HAL_AUDIO_GetChipType(void);
void HAL_AUDIO_SET_IPAUTH(MS_U32 ip_auth);

//================================================================
//  Basic Register read/write function
//================================================================
MS_U8   HAL_AUDIO_AbsReadByte(MS_U32 u32RegAddr);
MS_U8   HAL_AUDIO_ReadByte(MS_U32 u32RegAddr);
MS_U16  HAL_AUDIO_AbsReadReg(MS_U32 u32RegAddr);
MS_U16  HAL_AUDIO_ReadReg(MS_U32 u32RegAddr);
void    HAL_AUDIO_AbsWriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);
void    HAL_AUDIO_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);
void    HAL_AUDIO_AbsWriteReg(MS_U32 u32RegAddr, MS_U16 u16Val);
void    HAL_AUDIO_WriteReg(MS_U32 u32RegAddr, MS_U16 u16Val);
void    HAL_AUDIO_AbsWriteMaskReg(MS_U32 u32RegAddr, MS_U16 u16Mask, MS_U16 u16Val);
void    HAL_AUDIO_WriteMaskReg(MS_U32 u32RegAddr, MS_U16 u16Mask, MS_U16 u16Val);
void    HAL_AUDIO_AbsWriteMaskByte(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_U8 u8Val);
void    HAL_AUDIO_WriteMaskByte(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_U8 u8Val);

void    _HAL_AUDIO_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val);

void    HAL_AUDIO_WriteMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum, MS_U16 u16Data);
MS_U16  HAL_AUDIO_ReadMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum);
void    HAL_AUDIO_DmaReader_AllInput_Init(void);

//================================================================
//  For Audio initialize
//================================================================
MS_BOOL HAL_AUDIO_Init(MS_BOOL Initype);
void    HAL_AUDIO_WriteInitTable(void);
void    HAL_AUDIO_WritePreInitTable(void);                //Refine power on sequence for earphone & DAC pop noise issue
void    HAL_AUDIO_WriteStopDecTable(void);                //Set stop command to R2/DSP
void    HAL_AUDIO_EnaEarphone_LowPower_Stage(void);        //Refine power on sequence for earphone & DAC pop noise issue
void    HAL_AUDIO_EnaEarphone_HighDriving_Stage(void);    //Refine power on sequence for earphone & DAC pop noise issue
void    HAL_AUDIO_SetPowerOn(MS_BOOL bPower_on);
void    HAL_AUDIO_ADCInit(void);
void    HAL_AUDIO_SwResetMAD(void);
void    HAL_AUDIO_ResetDSP(void);
void    HAL_AUDIO_TriggerSifPLL(void);

void    HAL_AUDIO_DecDspISR(void);
void    HAL_AUDIO_SeDspISR(void);

void    HAL_AUDIO_SetPlayFileFlag(MS_BOOL bDspType, MS_BOOL bSet);
MS_BOOL HAL_AUDIO_GetPlayFileFlag(MS_BOOL bDspType);
void    HAL_AUDIO_SetEncodeDoneFlag(MS_BOOL bSet);
MS_BOOL HAL_AUDIO_GetEncodeDoneFlag(void);
void    HAL_AUDIO_DmaWriter_Init(void);
MS_BOOL HAL_AUDIO_DmaWriter_Read(void* buffer, MS_U32 bytes);
void    HAL_AUDIO_DMIOReset(void);
MS_BOOL HAL_AUDIO_CheckBootOnInitState(void);

//================================================================
// Audio I/O Config. function
//================================================================
AUDIO_PATH_GROUP_TYPE HAL_AUDIO_GetPathGroup(void);
void    HAL_AUDIO_SetNormalPath(AUDIO_PATH_TYPE path, AUDIO_INPUT_TYPE input, AUDIO_OUTPUT_TYPE output);
void    HAL_AUDIO_SetInputPath(AUDIO_INPUT_TYPE u8Input, AUDIO_PATH_TYPE u8Path);
void    HAL_AUDIO_SetOutputPath(AUDIO_PATH_TYPE u8Path, AUDIO_OUTPUT_TYPE u8Output);
void    HAL_AUDIO_SetInternalPath(AUDIO_INTERNAL_PATH_TYPE u8Path,  AUDIO_OUTPUT_TYPE u8Output);
void    HAL_AUDIO_FwTriggerDSP(MS_U16 u16Cmd);
void    HAL_AUDIO_DspBootOnDDR(MS_BOOL bEnable);
MS_BOOL HAL_AUDIO_GetIsDtvFlag(void);
void    HAL_AUDIO_SetIsDtvFlag(MS_BOOL);

MS_U8   HAL_AUDIO_ReadDspCodeType(void);
MS_U8   HAL_AUDIO_Dec_Status(void);
MS_U8   HAL_AUDIO_Se_Status(void);
void    HAL_AUDIO_Set_Fading(MS_U32 u32VolFading);

void    HAL_AUDIO_BT_SetUploadRate(MS_BOOL bEnable,MS_U8 u8Samprate);
void    HAL_AUDIO_BT_SetBufferCounter(MS_U32 u32Counter);
void    HAL_AUDIO_USBPCM_Enable(MS_BOOL bEnable);
MS_BOOL HAL_AUDIO_USBPCM_GetFlag(void);
void    HAL_AUDIO_USBPCM_SetFlag(MS_BOOL bEnable);
void    HAL_AUDIO_USBPCM_GetMemInfo(AUDIO_UPLOAD_INFO *uploadInfo);
MS_BOOL HAL_AUDIO_SetDataCaptureSource(AUDIO_DEVICE_TYPE eID, AUDIO_CAPTURE_SOURCE_TYPE eSource);
MS_BOOL HAL_AUDIO_PCMCapture_Init(const AUDIO_DEVICE_TYPE eID, const AUDIO_CAPTURE_SOURCE_TYPE eSource);
MS_BOOL HAL_AUDIO_PCMCapture_Start(const AUDIO_DEVICE_TYPE eID);
MS_BOOL HAL_AUDIO_PCMCapture_Stop(const AUDIO_DEVICE_TYPE eID);
MS_BOOL HAL_AUDIO_PCMCapture_Read(const AUDIO_DEVICE_TYPE eID, void* buffer, const MS_U32 bytes);

void    _HAL_AUDIO_I2S_SetMode(MS_U8 u8Mode, MS_U8 u8Val);

//=====================================================
// AUDIO SPDIF Relational Hal Function
//=====================================================
void    _HAL_AUDIO_SPDIF_HWEN(MS_BOOL bEnable);
void    _HAL_AUDIO_SPDIF_SetMute(MS_BOOL bEnMute);
void    HAL_AUDIO_SPDIF_SetMode(MS_U8 u8Spdif_mode, MS_U8 u8Input_src);
MS_U8   _HAL_AUDIO_SPDIF_GetMode(void);
void    _HAL_AUDIO_SPDIF_ByPassChannel(MS_BOOL bEnable);
void    HAL_AUDIO_SPDIF_SetSCMS(MS_U8 C_bit_en, MS_U8 L_bit_en);
MS_U8   HAL_AUDIO_SPDIF_GetSCMS(void);
void    HAL_AUDIO_SetChannelStatus(SPDIF_CS_MODE_TYPE eType, SPDIF_CS_MODE_VALUE eValue);
MS_BOOL HAL_AUDIO_SPDIF_ChannelStatus_CTRL(AUDIO_SPDIF_CS_TYPE cs_mode, AUDIO_SPDIF_CS_TYPE_STATUS status);
void    HAL_AUDIO_SPDIF_Monitor_SamplingRate(void);
void    HAL_AUDIO_SPDIF_SetOutputType(AUDIO_SOURCE_INFO_TYPE eSource, AUDIO_SPDIF_OUTPUT_TYPE eType);

void HAL_AUDIO_SetDigitalOut(DIGITAL_OUTPUT_TYPE ePath, AUDIO_DIGITAL_OUTPUT_TYPE eDigitalMode);
MS_BOOL HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus);
MS_BOOL HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus);
void  HAL_AUDIO_Set_DecimationMode(AUDIO_OUTPUT_TYPE Output_Type, Audio_id ID);

//=====================================================
// AUDIO HDMI Relational Hal Function
//=====================================================
void    _HAL_AUDIO_HDMI_Tx_SetMute(MS_BOOL bEnMute);
MS_BOOL HAL_AUDIO_HDMI_NonpcmMonitor(void);
MS_U8   HAL_AUDIO_HDMI_NonpcmMonitor2(void);
MS_U8   HAL_AUDIO_HDMI_DolbyMonitor(void);
void    HAL_AUDIO_HDMI_SetNonpcm(MS_U8 u8NonPCM_en);
void    HAL_AUDIO_HDMI_AC3_PathCFG(MS_U8 u8Ctrl);
MS_U16  HAL_AUDIO_HDMI_Get_SYNTH_FREQ(void);
void    HAL_AUDIO_HDMI_Set_DwonSample(MS_U8 ratio);
void    HAL_AUDIO_HDMIAutoMute(MS_U8 ch, MS_U8 value);
void    HAL_AUDIO_HDMI_Tx_GetStatus(MS_BOOL *onOff, AUDIO_FS_TYPE *hdmi_SmpFreq, HDMI_TX_OUTPUT_TYPE *outType );
void    HAL_AUDIO_HDMI_TX_SetMode(HDMI_TX_OUTPUT_TYPE outType, AUDIO_SOURCE_INFO_TYPE eSource);
MS_BOOL HAL_AUDIO_HDMI_RX_GetHdmiInAudioStatus( ST_HDMI_AUDIO_STATUS *p_hdmiAudioSts);
//================================================================
// for audio driver internal use function
//================================================================
void    _HAL_AUDIO_InitStep1(void);
void    HAL_AUDIO_SetDspBaseAddr(MS_U8 u8Index, MS_PHY phyBin_Base_Address, MS_PHY phyMad_Base_Buffer_Adr);
MS_PHY  HAL_AUDIO_GetDspMadBaseAddr(MS_U8 u8Index);
MS_PHY  HAL_AUDIO_GetDspBinBaseAddr(MS_U8 u8Index);
void    HAL_AUDIO_SetDspCodeType(AUDIO_DSP_CODE_TYPE dspCodeType, MS_BOOL bFlag);
AUDIO_DSP_CODE_TYPE   HAL_AUDIO_GetDspCodeType(void);
void HAL_AUDIO_SetDsp2CodeType(AUDIO_DSP_CODE_TYPE dspCodeType, MS_BOOL bFlag);
AUDIO_DSP_CODE_TYPE   HAL_AUDIO_GetDsp2CodeType(void);
MS_U8   HAL_AUDIO_GetDSPalive(void);
void    HAL_AUDIO_SetDSPalive(MS_U8 alive);
void    HAL_AUDIO_SetAdvSndEffDspCodeType(AUDIO_DSP_CODE_TYPE dspCodeType, MS_BOOL bFlag);
AUDIO_DSP_CODE_TYPE HAL_AUDIO_GetAdvSndEffDspCodeType(void);
void    HAL_AUDIO_BackupMailbox(void);
void    HAL_AUDIO_RestoreMailbox(void);
MS_U8   HAL_AUDIO_Enc_Status(void);
void    HAL_AUDIO_DmaReader_Init(SAMPLE_RATE sampleRate);
void    HAL_AUDIO_DmaReader_AllInput_Init(void);
MS_BOOL HAL_AUDIO_DmaReader_WritePCM(void* buffer, MS_U32 bytes);
void    HAL_AUDIO_SetEntertainmentMode(AUDIO_ENTERTAINMENT_MODE eMode);
MS_BOOL HAL_AUDIO_AlgReloadCode(AUDIO_DSP_CODE_TYPE dspCodeType);
MS_BOOL HAL_AUDIO_Alg2ReloadCode(AUDIO_DSP_CODE_TYPE dspCodeType);

MS_BOOL HAL_AUDIO_SetSystem(En_DVB_decSystemType enDecSystem);
MS_BOOL HAL_AUDIO_GetDecSysSupportStatus(En_DVB_decSystemType enDecSystem);

MS_U16  HAL_AUDIO_GetUniDecodeDoneTag(void);
MS_U16  HAL_AUDIO_GetUniDecodeFrameCnt(AUDIO_DEC_ID eDSPId);
void    HAL_AUDIO_PTS_info( MS_VIRT );

void    HAL_AUDIO_RebootDecDSP(void);
MS_U8   HAL_AUDIO_ReadDspCounter(AUDIO_DSP_COUNTER_TYPE  CounterType);
void    HAL_AUDIO_DspReboot(MS_U8 alive);
void    HAL_AUDIO_SetPowerDownWait(MS_BOOL bEnable);
void    HAL_AUDIO_SendIntrupt(MS_BOOL bDspType,MS_U16 u8Cmd);
MS_BOOL HAL_AUDIO_SeSystemLoadCode(void);
MS_BOOL HAL_AUDIO_DecoderLoadCode(void);

extern MS_BOOL HAL_AUDSP_DspLoadCode(AUDIO_DSP_CODE_TYPE dspCodeType);
void    HAL_AUDIO_update_DspUsage(AUDIO_SOURCE_INFO_TYPE eSourceType);
Audio_id HAL_AUDIO_Convert_DecId_to_ADECId(AUDIO_DEC_ID dec_id);

MS_U32 HAL_AUDIO_GetCaps(AUDIO_DEC_ID DecId, MS_U32 *pCaps);
MS_BOOL HAL_AUDIO_CheckPlayDone(void);

MS_S32 HAL_AUDIO_InputSwitch(AUDIO_SWITCH_GROUP enGroup);

//================================================================
// Audio ALSA Interface Relational Function
//================================================================
MS_BOOL HAL_AUDIO_ALSA_Check(void);
MS_BOOL HAL_AUDIO_ALSA_Enable(MS_BOOL bEnable);

MS_BOOL HAL_AUDIO_ReloadCode(AUDIO_DSP_ID id, AUDIO_DSP_CODE_TYPE code_type);
MS_BOOL HAL_AUDIO_AlgReloadCode2(AUDIO_DSP_ID id, void * pau_info);
MS_BOOL HAL_AUDIO_SetSystem2(AUDIO_DEC_ID DecId, En_DVB_decSystemType Param);
MS_BOOL HAL_AUDIO_SetDecodeSystem(AUDIO_DEC_ID DecId, AudioDecStatus_t *p_AudioDecStatus);
//================================================================
// Audio dump debug message
//================================================================
void HAL_AUDIO_DumpDspInfo(void);
AUDIO_DEC_ID HAL_AUDIO_OpenDecodeSystem(AudioDecStatus_t * p_AudioDecStatus);
MS_PHY HAL_AUDIO_GetDDRInfo(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo);
MS_BOOL HAL_AUDIO_CheckVersionInfo(void);

//================================================================
// Audio Common PCM Interface
//================================================================
#if AUDIO_HW_DMA_READER1_SUPPORT
MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Open(void *pData);
MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Close(void);
MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Start(void);
MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Stop(void);
MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Set(MS_U32 u32Cmd, const void *pData);
MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Get(MS_U32 u32Cmd, void *pData);
MS_U32 HAL_AUDIO_PCM_HwDma_Reader1_Write(const void *pBuf, MS_U32 u32Size);
MS_S32 HAL_AUDIO_PCM_HwDma_Reader1_Flush(void);
#endif

#if AUDIO_HW_DMA_READER2_SUPPORT
MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Open(void *pData);
MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Close(void);
MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Start(void);
MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Stop(void);
MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Set(MS_U32 u32Cmd, const void *pData);
MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Get(MS_U32 u32Cmd, void *pData);
MS_U32 HAL_AUDIO_PCM_HwDma_Reader2_Write(const void *pBuf, MS_U32 u32Size);
MS_S32 HAL_AUDIO_PCM_HwDma_Reader2_Flush(void);
#endif

#if AUDIO_SW_DMA_READER1_SUPPORT
MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Open(void *pData);
MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Close(void);
MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Start(void);
MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Stop(void);
MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Set(MS_U32 u32Cmd, const void *pData);
MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Get(MS_U32 u32Cmd, void *pData);
MS_U32 HAL_AUDIO_PCM_SwDma_Reader1_Write(const void *pBuf, MS_U32 u32Size);
MS_S32 HAL_AUDIO_PCM_SwDma_Reader1_Flush(void);
#endif

#if AUDIO_R2_DMA_READER1_SUPPORT
MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Open(void *pData);
MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Close(void);
MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Start(void);
MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Stop(void);
MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Set(MS_U32 u32Cmd, const void *pData);
MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Get(MS_U32 u32Cmd, void *pData);
MS_U32 HAL_AUDIO_PCM_R2Dma_Reader1_Write(const void *pBuf, MS_U32 u32Size);
MS_S32 HAL_AUDIO_PCM_R2Dma_Reader1_Flush(void);
#endif

#if AUDIO_R2_DMA_READER2_SUPPORT
MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Open(void *pData);
MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Close(void);
MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Start(void);
MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Stop(void);
MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Set(MS_U32 u32Cmd, const void *pData);
MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Get(MS_U32 u32Cmd, void *pData);
MS_U32 HAL_AUDIO_PCM_R2Dma_Reader2_Write(const void *pBuf, MS_U32 u32Size);
MS_S32 HAL_AUDIO_PCM_R2Dma_Reader2_Flush(void);
#endif

#if AUDIO_PCM_CAPTURE1_SUPPORT
MS_S32 HAL_AUDIO_PCM_Capture1_Open(void *pData);
MS_S32 HAL_AUDIO_PCM_Capture1_Close(void);
MS_S32 HAL_AUDIO_PCM_Capture1_Start(void);
MS_S32 HAL_AUDIO_PCM_Capture1_Stop(void);
MS_S32 HAL_AUDIO_PCM_Capture1_Set(MS_U32 u32Cmd, const void *pData);
MS_S32 HAL_AUDIO_PCM_Capture1_Get(MS_U32 u32Cmd, void *pData);
MS_U32 HAL_AUDIO_PCM_Capture1_Read(void *pBuf, MS_U32 u32Size);
MS_S32 HAL_AUDIO_PCM_Capture1_Flush(void);
#endif

#if AUDIO_PCM_CAPTURE2_SUPPORT
MS_S32 HAL_AUDIO_PCM_Capture2_Open(void *pData);
MS_S32 HAL_AUDIO_PCM_Capture2_Close(void);
MS_S32 HAL_AUDIO_PCM_Capture2_Start(void);
MS_S32 HAL_AUDIO_PCM_Capture2_Stop(void);
MS_S32 HAL_AUDIO_PCM_Capture2_Set(MS_U32 u32Cmd, const void *pData);
MS_S32 HAL_AUDIO_PCM_Capture2_Get(MS_U32 u32Cmd, void *pData);
MS_U32 HAL_AUDIO_PCM_Capture2_Read(void *pBuf, MS_U32 u32Size);
MS_S32 HAL_AUDIO_PCM_Capture2_Flush(void);
#endif

#if AUDIO_HW_DMA_WRITER1_SUPPORT
MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Open(void *pData);
MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Close(void);
MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Start(void);
MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Stop(void);
MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Set(MS_U32 u32Cmd, const void *pData);
MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Get(MS_U32 u32Cmd, void *pData);
MS_U32 HAL_AUDIO_PCM_HwDma_Writer1_Read(void *pBuf, MS_U32 u32Size);
MS_S32 HAL_AUDIO_PCM_HwDma_Writer1_Flush(void);
#endif

//================================================================
// Audio Common Encoder Interface
//================================================================
//#if AUDIO_MP3_ENCODER1_SUPPORT
MS_S32 HAL_AUDIO_MP3_ENCODER1_Open(void);
MS_S32 HAL_AUDIO_MP3_ENCODER1_Close(void);
MS_S32 HAL_AUDIO_MP3_ENCODER1_Start(void);
MS_S32 HAL_AUDIO_MP3_ENCODER1_Stop(void);
MS_S32 HAL_AUDIO_MP3_ENCODER1_Ioctl(MS_U32 u32Cmd, void *pData);
MS_U32 HAL_AUDIO_MP3_ENCODER1_Read(void *pBuf, MS_U32 u32Size);
MS_S32 HAL_AUDIO_MP3_ENCODER1_Flush(void);
//#endif

//#if AUDIO_MP3_ENCODER2_SUPPORT
MS_S32 HAL_AUDIO_MP3_ENCODER2_Open(void);
MS_S32 HAL_AUDIO_MP3_ENCODER2_Close(void);
MS_S32 HAL_AUDIO_MP3_ENCODER2_Start(void);
MS_S32 HAL_AUDIO_MP3_ENCODER2_Stop(void);
MS_S32 HAL_AUDIO_MP3_ENCODER2_Ioctl(MS_U32 u32Cmd, void *pData);
MS_U32 HAL_AUDIO_MP3_ENCODER2_Read(void *pBuf, MS_U32 u32Size);
MS_S32 HAL_AUDIO_MP3_ENCODER2_Flush(void);
//#endif

//================================================================
// Specific Definitions for Audio TEE
//================================================================
#define ree_to_tee_mbx_msg    pAudioTeeInfoShm->ReeToTeeMbxMsg
#define tee_to_ree_mbx_msg    pAudioTeeInfoShm->TeeToReeMbxMsg
#define tee_mbx_msg_class    pAudioTeeInfoShm->u8MbxMsgClass
#define audio_tee_enabled    pAudioTeeInfoShm->g_bAudioTeeEnabled
#define audio_tee_mbx_initialized    pAudioTeeInfoShm->g_bAudioTeeMbxInitialized
#define dec_dsp_secure_tee_accessed    pAudioTeeInfoShm->g_bDecDspSecureTeeAccessed
#define snd_dsp_secure_tee_accessed    pAudioTeeInfoShm->g_bSndDspSecureTeeAccessed
#define mbx_msg_ack_status    pAudioTeeInfoShm->ack_status

AUDIO_SOURCE_INFO_TYPE HAL_AUDIO_ConvertInputType2SourceInfo(AUDIO_INPUT_TYPE eInputType);

#define HALAUDIO_CHECK_TEE_INFO_SHM_INIT \
    do { \
        if (pAudioTeeInfoShm == NULL) \
        { \
            if (HAL_AUDIO_AllocateTeeInfoShm() == NULL) \
            { \
                MS_ASSERT(0); \
            } \
        } \
    } while(0)

#define AUDIO_TEE_INFO_SHM_CHECK_NULL \
    do { \
        MS_ASSERT(pAudioTeeInfoShm != NULL); \
    } while(0)

#define REE_TO_TEE_MBX_MSG_SET_PARAM_COUNT(count) \
    do { \
        ree_to_tee_mbx_msg.u8ParameterCount = count; \
    } while(0)

#define REE_TO_TEE_MBX_MSG_SET_PARAM(index, parameter) \
    do { \
        ree_to_tee_mbx_msg.u8Parameters[index] = parameter; \
    } while(0)

#define REE_TO_TEE_MBX_MSG_INIT \
    do{ \
        ree_to_tee_mbx_msg.eRoleID=E_MBX_CPU_MIPS_VPE1; \
        ree_to_tee_mbx_msg.u8Ctrl=0; \
        ree_to_tee_mbx_msg.eMsgType=E_MBX_MSG_TYPE_INSTANT; \
    }while(0)

#define TEE_TO_REE_MBX_MSG_INIT(msg) \
    do{ \
        msg.eRoleID=E_MBX_CPU_MIPS; \
        msg.u8Ctrl=0; \
        msg.eMsgType=E_MBX_MSG_TYPE_INSTANT; \
    }while(0)

#define MBX_MSG_SET_CLASSID(msg,classID)\
    do{ \
        msg.u8MsgClass = classID; \
    }while(0)

#define MBX_MSG_SET_CMD(msg,cmd)\
    do{ \
        msg.u8Index = cmd; \
    }while(0)

#define TEE_MBX_MSG_CLASS_SET(class) \
    do{ \
        tee_mbx_msg_class = class;	\
    }while(0)

//================================================================
// Audio TEE MBX Message Enum Definitions (REE side to TEE siede)
//================================================================
typedef enum
{
    REE_TO_TEE_MBX_MSG_NULL                     = 0x0000,

    REE_TO_TEE_MBX_MSG_TYPE_GENERAL = 0x20,
    REE_TO_TEE_MBX_MSG_TYPE_DEC = 0x40,
    REE_TO_TEE_MBX_MSG_TYPE_SE = 0x80,
    // NonDSP Specific General MBX Command
    REE_TO_TEE_MBX_MSG_HAL_AUDIO_SetDspBaseAddr = 0x01,
    // DSP Specific MBX Command
    REE_TO_TEE_MBX_MSG_HAL_AUDSP_DspLoadCode    = 0x11
} REE_TO_TEE_MBX_MSG_TYPE;

//================================================================
// Audio TEE MBX Message Enum Definitions (TEE side to REE siede)
//================================================================
typedef enum
{
   TEE_TO_REE_MBX_MSG_NULL                 = 0x00,
   TEE_TO_REE_MBX_ACK_MSG_INVALID          = 0x80,
   TEE_TO_REE_MBX_ACK_MSG_NO_TEE           = 0x81,
   TEE_TO_REE_MBX_ACK_MSG_ACTION_SUCCESS   = 0x90,
   TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL      = 0x91
} TEE_TO_REE_MBX_ACK_MSG_TYPE;

typedef struct
{
   MS_U32 g_ShmID;
   MS_BOOL g_bAudioTeeEnabled;
   MS_BOOL g_bAudioTeeMbxInitialized;

   MS_BOOL g_bDecDspSecureTeeAccessed;
   MS_BOOL g_bSndDspSecureTeeAccessed;

   MBX_Msg ReeToTeeMbxMsg;
   MBX_Msg TeeToReeMbxMsg;
   MS_U8 u8MbxMsgClass;
   MS_U32 g_u32ClientCounter;
   TEE_TO_REE_MBX_ACK_MSG_TYPE ack_status;
} AUDIO_TEE_INFO_SHARE_MEM;

#ifndef MSOS_TYPE_NUTTX
MS_BOOL HAL_AUDIO_ResetTeeInfoShmToDefault(void);
AUDIO_TEE_INFO_SHARE_MEM *HAL_AUDIO_AllocateTeeInfoShm(void);
MS_BOOL HAL_AUDIO_DeAllocateTeeInfoShm(void);

//================================================================
// Audio Mailbox Relational Function
//================================================================
MS_BOOL HAL_AUDIO_RegisterMBX(void);
TEE_TO_REE_MBX_ACK_MSG_TYPE HAL_AUDIO_SendMBXMsg(REE_TO_TEE_MBX_MSG_TYPE msg_type);
#endif

AUDIO_DSP_CODE_TYPE HAL_AUDIO_Convert_EnDVBdecSystemType_to_AudioDspCodeType(En_DVB_decSystemType enDecSystem);
void HAL_AUDIO_ResetDspCodeType(void);
MS_S32 HAL_AUDIO_CheckHashkeyDone(MS_U32 ip_auth);
#endif // _DEV_AUDIO_H_
