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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
//==============================================================================
// [mhal_hdmi.h]
// Date: 20081203
// Descriptions: Add a new layer for HW setting
//==============================================================================
#ifndef MHAL_HDMI_H
#define MHAL_HDMI_H

#include "hwreg_ddc.h"
#include "hwreg_adc_atop.h"
#include "hwreg_adc_dtop.h"
#include "hwreg_hdcp.h"
#include "hwreg_hdmi.h"
#include "hwreg_sc.h"

#include "drvXC_HDMI_if.h"
#include "apiXC.h"



//==============================================================================
//==============================================================================
/* DDC SRAM SEL (After T3) */
#define     DDC_RAM_SRAM_DVI    0
#define     DDC_RAM_SRAM_ADC	1
#define     DDC_RAM_SRAM_DVI0   0
#define     DDC_RAM_SRAM_DVI1   1
#define     DDC_RAM_SRAM_DVI2   2
#define     DDC_RAM_SRAM_DVI3   3

#define INPUT_PORT_DVI_END              INPUT_PORT_DVI3

#define ABS_MINUS(a, b)                     ((a > b)? (a -b): (b -a))

#define HDMI_GET_PORT_SELECT(a)             ((MS_U8)(a - INPUT_PORT_DVI0))

//==============================================================================
//==============================================================================

#define IRQ_DVI_CK_CHG                      BIT(0)      //#[0]
#define IRQ_HDMI_MODE_CHG                   BIT(1)    //#[1]
#define IRQ_SCART_ID0_CHG                   BIT(2)     //#[2]
#define IRQ_SCART_ID1_CHG                   BIT(3)     //#[3]
#define IRQ_SAR_DET_UPD                     BIT(4)        //#[4]
#define IRQ_RESERVE                         (BIT(7)|BIT(6)|BIT(5))                 //#[5:7]
#define IRQ_ALL_BIT                         (BIT(4)|BIT(3)|BIT(2)|BIT(1)|BIT(0))

#define HDMI_WB_DETECT_LOCK_THRESHOLD       8
#define HDMI_WB_DETECT_UNLOCK_THRESHOLD     0

#define HDMI_B_CHANNEL_EQ_VALUE             13
#define HDMI_G_CHANNEL_EQ_VALUE             8
#define HDMI_R_CHANNEL_EQ_VALUE             8

#define HDMI_I_CONTROL_VALUE_HIGH           0
#define HDMI_I_CONTROL_VALUE_MEDIUM         0
#define HDMI_I_CONTROL_VALUE_LOW            7

#define HDMI_HF_SETTING_VALUE_HIGH          3
#define HDMI_HF_SETTING_VALUE_MEDIUM        1
#define HDMI_HF_SETTING_VALUE_LOW           0

typedef struct
{
    MS_BOOL bNoInputFlag;
    MS_BOOL bHDMI20Flag;
    MS_BOOL bYUV420Flag;
    MS_BOOL bHDMIModeFlag;
    MS_BOOL bAutoEQEnable;
    MS_BOOL bAutoEQRetrigger;
    MS_BOOL bTimingStableFlag;
    MS_BOOL bPowerOnLane;
    MS_BOOL bIsRepeater;
    MS_BOOL bPowerSavingFlag;
    MS_U8 ucAutoEQState;
    MS_U8 ucSourceVersion;
    MS_U8 ucCheckErrorInterval;
    MS_U8 ucAutoEQ14Mode;
    MS_U8 ucCheckScrambleCounter;
    MS_U8 ucHDCPState;
    MS_U8 ucHDCPInt;
    MS_U16 usClockCount;
    MS_U16 usCheckErrorTimes;
    MS_U16 usAutoEQProcCounter;
} stHDMI_POLLING_INFO;

enum HDMI_SOURCE_VERSION_TYPE
{
    HDMI_SOURCE_VERSION_NOT_SURE = 0,
    HDMI_SOURCE_VERSION_HDMI14,
    HDMI_SOURCE_VERSION_HDMI20,
};

enum HDMI_HDCP_STATE
{
    HDMI_HDCP_NO_ENCRYPTION = 0,
    HDMI_HDCP_1_4,
    HDMI_HDCP_2_2,
};

enum HDMI_HDCP_ENCRYPTION_STATE
{
    HDMI_HDCP_NOT_ENCRYPTION = 0,
    HDMI_HDCP_1_4_ENCRYPTION,
    HDMI_HDCP_2_2_ENCRYPTION,
};

enum HDMI_STATUS_FLAG_TYPE
{
    HDMI_STATUS_MPEG_PACKET_RECEIVE_FLAG = BIT(0),
    HDMI_STATUS_AUDIO_PACKET_RECEIVE_FLAG = BIT(1),
    HDMI_STATUS_SPD_PACKET_RECEIVE_FLAG = BIT(2),
    HDMI_STATUS_AVI_PACKET_RECEIVE_FLAG = BIT(3),
    HDMI_STATUS_GCP_PACKET_RECEIVE_FLAG = BIT(4),
    HDMI_STATUS_AUDIO_SAMPLE_PACKET_RECEIVE_FLAG = BIT(5),
    HDMI_STATUS_ACR_PACKET_RECEIVE_FLAG = BIT(6),
    HDMI_STATUS_VS_PACKET_RECEIVE_FLAG = BIT(7),
    HDMI_STATUS_NULL_PACKET_RECEIVE_FLAG = BIT(8),
    HDMI_STATUS_ISRC2_PACKET_RECEIVE_FLAG = BIT(9),
    HDMI_STATUS_ISRC1_PACKET_RECEIVE_FLAG = BIT(10),
    HDMI_STATUS_ACP_PACKET_RECEIVE_FLAG = BIT(11),
    HDMI_STATUS_DSD_PACKET_RECEIVE_FLAG = BIT(12),
    HDMI_STATUS_GM_PACKET_RECEIVE_FLAG = BIT(13),
    HDMI_STATUS_HBR_PACKET_RECEIVE_FLAG = BIT(14),
    HDMI_STATUS_VBI_PACKET_RECEIVE_FLAG = BIT(15),
    HDMI_STATUS_HDR_PACKET_RECEIVE_FLAG = BIT(16),
    HDMI_STATUS_RESERVED_PACKET_RECEIVE_FLAG = BIT(17),
    HDMI_STATUS_EDR_VALID_FLAG = BIT(18),
};

enum HDMI_INFO_SOURCE_TYPE
{
    HDMI_INFO_SOURCE0 = 0,
    HDMI_INFO_SOURCE_MAX,
    HDMI_INFO_SOURCE1,
};

//==============================================================================
//==============================================================================
#ifdef MHAL_HDMI_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

INTERFACE MS_BOOL Hal_HDMI_Check_MHL_Attach(void);
INTERFACE MS_BOOL Hal_HDCP_GetEncryptionFlag(E_MUX_INPUTPORT enInputPortType);
INTERFACE MS_U16 Hal_HDMI_Func_Caps(void);
////--------------> for HDCP
//INTERFACE void Hal_HDCP_clearflag( void );
INTERFACE void Hal_HDCP_ddc_en(E_MUX_INPUTPORT enInputPortType, MS_BOOL bEnable);
INTERFACE void Hal_HDCP_Vsync_end_en( MS_BOOL bEnalbe );

////--------------> for HDMI
INTERFACE void Hal_HDMI_pullhpd(MS_BOOL bHighLow, E_MUX_INPUTPORT enInputPortType, MS_BOOL bInverse, MS_U8 ucMHLSupportPath);
INTERFACE void Hal_HDMI_init(MS_BOOL bImmeswitchSupport, MS_U8 ucMHLSupportPath);
//INTERFACE void Hal_HDMI_init( void );
INTERFACE void Hal_HDMI_exit(E_MUX_INPUTPORT enInputPortType);

INTERFACE void Hal_HDMI_Set_EQ(E_MUX_INPUTPORT enInputPortType, MS_HDMI_EQ enEq, MS_U8 u8EQValue);
INTERFACE void Hal_HDMI_Audio_MUTE_Enable(MS_U16 u16MuteEvent, MS_U16 u16MuteMask);
INTERFACE void Hal_HDMI_Audio_Status_Clear(void);

INTERFACE MS_U16 Hal_HDMI_gcontrol_info(HDMI_GControl_INFO_t gcontrol, MS_U8 ucHDMIInfoSource);
INTERFACE MS_U16 Hal_HDMI_pll_ctrl1(HDMI_PLL_CTRL_t pllctrl, MS_BOOL bread, MS_U16 u16value);
INTERFACE MS_U16 Hal_HDMI_pll_ctrl2(HDMI_PLL_CTRL2_t pllctrl, MS_BOOL bread, MS_U16 u16value);
INTERFACE MS_U16 Hal_HDMI_GetHDEInfo(MS_U8 ucHDMIInfoSource);
INTERFACE void Hal_DVI_Accumulator_Monitor( void );

////--------------> for HDMI Audio
INTERFACE void Hal_HDMI_audio_output(MS_BOOL bEnable);

////--------------> for DVI
INTERFACE MS_BOOL Hal_DVI_clk_lessthan70mhz_usingportc( MS_BOOL bIsPortC);
INTERFACE MS_U16 Hal_DVI_ChannelPhaseStatus( MS_U8 u8Channel);
INTERFACE MS_BOOL Hal_DVI_clk_stable_usingportc(MS_BOOL bIsPortC);
INTERFACE MS_BOOL Hal_DVI_clklose_det(E_MUX_INPUTPORT enInputPortType);
INTERFACE void Hal_DVI_IMMESWITCH_PS_SW_Path(void);
INTERFACE void Hal_DVI_sw_reset(MS_U16 u16Reset);
INTERFACE MS_U16 Hal_DVI_irq_info(E_MUX_INPUTPORT enInputPortType, MS_U16 irqbit);
INTERFACE void Hal_DVI_irq_mask(E_MUX_INPUTPORT enInputPortType, MS_U16 irqbit);
INTERFACE void Hal_DVI_irq_forcemode(E_MUX_INPUTPORT enInputPortType, MS_U16 irqbit);
INTERFACE void Hal_DVI_irq_clear(E_MUX_INPUTPORT enInputPortType, MS_U16 irqbit);
INTERFACE void Hal_DVI_ClkPullLow(MS_BOOL bPullLow, E_MUX_INPUTPORT enInputPortType);
INTERFACE void Hal_DVI_SwitchSrc(E_MUX_INPUTPORT enInputPortType, MS_U8 ucHDMIInfoSource);

INTERFACE MS_U8 Hal_HDMI_err_status_update(MS_U8 ucHDMIInfoSource, MS_U8 u8value, MS_BOOL bread);
INTERFACE void Hal_HDMI_pkt_reset(E_MUX_INPUTPORT enInputPortType, HDMI_REST_t breset);
INTERFACE void Hal_HDMI_hpd_en(MS_BOOL benable);
INTERFACE void Hal_HDCP_initproductionkey( MS_U8 * pu8HdcpKeyData );
INTERFACE void Hal_HDCP_clearflag(E_MUX_INPUTPORT enInputPortType);
INTERFACE void Hal_HDMI_audio_output( MS_BOOL bEnable );
INTERFACE MS_U16 Hal_HDMI_packet_info(MS_U8 ucHDMIInfoSource);
INTERFACE MS_U8 Hal_HDMI_audio_content_protection_info(MS_U8 ucHDMIInfoSource);
INTERFACE EN_AVI_INFOFRAME_VERSION Hal_HDMI_avi_infoframe_info_ver(MS_U8 ucHDMIInfoSource);
INTERFACE MS_U8 Hal_HDMI_avi_infoframe_info(MS_U8 ucHDMIInfoSource, MS_U8 u8byte);
INTERFACE MS_BOOL Hal_HDMI_get_packet_value(MS_U8 ucHDMIInfoSource, MS_HDMI_PACKET_STATE_t u8state, MS_U8 u8byte_idx, MS_U8 *pu8Value);

INTERFACE MS_U8 Hal_HDMI_audio_channel_status(MS_U8 ucHDMIInfoSource, MS_U8 u8byte);
INTERFACE MS_U16 Hal_HDCP_getstatus(E_MUX_INPUTPORT enInputPortType);
INTERFACE void Hal_DVI_adc_adjust( MS_BOOL bClockLessThan70MHz);

INTERFACE void Hal_HDMI_packet_ctrl(MS_U8 ucHDMIInfoSource, MS_U16 u16pkt);
INTERFACE MS_U8 Hal_HDMI_audio_payload_info(MS_U8 ucHDMIInfoSource, MS_U8 u8byte);

// DDC
INTERFACE void HAL_HDMI_DDCRam_SelectPort(E_XC_DDCRAM_PROG_TYPE eDDCRamType);
INTERFACE void HAL_HDMI_DDCRAM_SetPhyAddr(XC_DDCRAM_PROG_INFO *pstDDCRam_Info);


// HDMI 1.4 new feature:
INTERFACE E_HDMI_ADDITIONAL_VIDEO_FORMAT Hal_HDMI_Check_Additional_Format(MS_U8 ucHDMIInfoSource);
INTERFACE E_XC_3D_INPUT_MODE Hal_HDMI_Get_3D_Structure(MS_U8 ucHDMIInfoSource);
INTERFACE E_HDMI_3D_EXT_DATA_T Hal_HDMI_Get_3D_Ext_Data(MS_U8 ucHDMIInfoSource);
INTERFACE void Hal_HDMI_Get_3D_Meta_Field(MS_U8 ucHDMIInfoSource, sHDMI_3D_META_FIELD *pdata);
INTERFACE MS_U8 Hal_HDMI_Get_VIC_Code(MS_U8 ucHDMIInfoSource);
INTERFACE E_HDMI_VIC_4Kx2K_CODE Hal_HDMI_Get_4Kx2K_VIC_Code(MS_U8 ucHDMIInfoSource);
INTERFACE void Hal_HDMI_Set_YUV422to444_Bypass(MS_BOOL btrue);
INTERFACE MS_BOOL Hal_HDMI_Is_Input_Large_166MHz(MS_U8 ucHDMIInfoSource);
INTERFACE void Hal_HDMI_AVG_ScaleringDown(MS_U8 ucHDMIInfoSource, MS_BOOL btrue);
INTERFACE void Hal_HDMI_ARC_PINControl(E_MUX_INPUTPORT enInputPortType, MS_BOOL bEnable, MS_BOOL bDrivingHigh);
INTERFACE void Hal_DVI_Software_Reset(E_MUX_INPUTPORT enInputPortType, MS_U16 u16Reset);

MS_BOOL Hal_DVI_GetPowerSavingStatus(E_MUX_INPUTPORT enInputPortType);
MS_BOOL Hal_DVI_GetDEStableStatus(E_MUX_INPUTPORT enInputPortType);
void Hal_DVI_EnhanceImmeswitch(E_MUX_INPUTPORT enInputPortType, MS_BOOL bflag);
void Hal_DVI_ForceAllPortsEnterPS(void);
MS_BOOL Hal_HDMI_Calibration( MS_U8 Port );
INTERFACE MS_U16 Hal_HDMI_GetTMDSFreq(void);
INTERFACE MS_BOOL Hal_HDMI_CheckHDMI20_Setting(E_MUX_INPUTPORT enInputPortType);

#define Hal_HDMI_CheckHDMI20_Setting(args...) 0

//INTERFACE void Hal_HDCP22_InitCBFunc(HDCP22_Recv_CBF pCBFunc, void* pContext);
INTERFACE void Hal_HDCP22_PortInit(MS_U8 ucPortIdx);
INTERFACE MS_BOOL Hal_HDCP22_PollingWriteDone(MS_U8 ucPortIdx);
INTERFACE MS_BOOL Hal_HDCP22_PollingReadDone(MS_U8 ucPortIdx);
INTERFACE void Hal_HDCP22_EnableCipher(MS_U8 ucPortType, MS_U8 ucPortIdx, MS_BOOL bIsEnable);
INTERFACE void Hal_HDCP22_FillCipherKey(MS_U8 ucPortIdx, MS_U8 *pucRiv, MS_U8 *pucSessionKey);
INTERFACE void Hal_HDCP22_FetchMsg(MS_U8 ucPortIdx, MS_U8* pucData, MS_U32 dwDataLen);
INTERFACE MS_BOOL Hal_HDCP22_RecvMsg(MS_U8 ucPortIdx, MS_U8 *ucMsgData);
INTERFACE void Hal_HDCP22_SendMsg(MS_U8 ucPortType, MS_U8 ucPortIdx, MS_U8* pucData, MS_U32 dwDataLen, void* pDummy, MS_U8* ubRecIDListSetDone);

INTERFACE void Hal_DVI_HF_adjust(void);
INTERFACE void Hal_HDMI_StablePolling(MS_U8 ucMHLSupportPath, stHDMI_POLLING_INFO *stHDMIPollingInfo);
INTERFACE MS_U8 Hal_HDMI_GetSourceVersionType(E_MUX_INPUTPORT enInputPortType);
INTERFACE void Hal_HDMI_Set5VDetectGPIOSelect(MS_U32 ul5VDetectGPIOIndex);
INTERFACE MS_BOOL Hal_HDMI_CheckHDCP14(void);
INTERFACE MS_BOOL Hal_HDCP_WriteX74(E_MUX_INPUTPORT enInputPortType, MS_U8 ubOffset, MS_U8 ubData);
INTERFACE MS_U8 Hal_HDCP_ReadX74(E_MUX_INPUTPORT enInputPortType, MS_U8 ubOffset);
INTERFACE void Hal_HDCP_SetReady(E_MUX_INPUTPORT enInputPortType, MS_BOOL bIsReady);
INTERFACE void Hal_HDCP_WriteKSVList(E_MUX_INPUTPORT enInputPortType, MS_U8* pucData, MS_U32 dwDataLen);
INTERFACE void Hal_HDCP_ClearStatus(E_MUX_INPUTPORT enInputPortType, MS_U16 usInt);
INTERFACE void Hal_HDCP_WriteDoneInterruptEnable(E_MUX_INPUTPORT enInputPortType, MS_BOOL bEnableIRQ);

#undef INTERFACE
#endif /* MHAL_HDMI_H */

