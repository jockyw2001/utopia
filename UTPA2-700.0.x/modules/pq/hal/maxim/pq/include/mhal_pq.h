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
#ifndef MHAL_PQ_H
#define MHAL_PQ_H

#define MEMFMT24BIT_YC10BIT_MOT     0xBB
#define MEMFMT24BIT_YC8BIT_MOT      0xAA
#define MEMFMT24BIT_YC10BIT         0x99
#define MEMFMT24BIT_YC8BIT          0x88
#define MEMFMT24BIT_CLEAR           0x00

#define SC_FILTER_Y_SRAM1               0x00
#define SC_FILTER_Y_SRAM2               0x01
#define SC_FILTER_Y_SRAM3               0x02
#define SC_FILTER_Y_SRAM4               0x03
#define SC_FILTER_C_SRAM1               0x04
#define SC_FILTER_C_SRAM2               0x05
#define SC_FILTER_C_SRAM3               0x06
#define SC_FILTER_C_SRAM4               0x07
#define SC_FILTER_SRAM_COLOR_INDEX      0x08
#define SC_FILTER_SRAM_COLOR_GAIN_SNR   0x09
#define SC_FILTER_SRAM_COLOR_GAIN_DNR   0x0A
#define SC_FILTER_SRAM_ICC_CRD          0x0B
#define SC_FILTER_SRAM_IHC_CRD          0x0C
#define SC_FILTER_SRAM_XVYCC_DE_GAMMA   0x0D
#define SC_FILTER_SRAM_XVYCC_GAMMA      0x0E

#define SUPPORT_SCMI_V2     TRUE
#define SUPPORT_DYNAMIC_MEM_FMT TRUE

#define PQ_SUPPORT_DEVICE_NUM 1

#define Y2Y_709_601_VERSION 1

#define REG_CHIP_REVISION           0x1ECF

#define PQ_CFD_INFO

void Hal_PQ_init_riu_base(void *pInstance,MS_U32 u32riu_base);
MS_U8 Hal_PQ_get_sync_flag(void *pInstance,MS_BOOL bMainWin);
MS_U8 Hal_PQ_get_input_vsync_value(void *pInstance,MS_BOOL bMainWin);
MS_U8 Hal_PQ_get_output_vsync_value(void *pInstance,MS_BOOL bMainWin);
MS_U8 Hal_PQ_get_input_vsync_polarity(void *pInstance,MS_BOOL bMainWin);
void Hal_PQ_set_memfmt_doublebuffer(void *pInstance,MS_BOOL bEn);
void Hal_PQ_set_mem_fmt(void *pInstance,MS_BOOL bMainWin, MS_U16 u16val, MS_U16 u16Mask);
void Hal_PQ_set_mem_fmt_en(void *pInstance,MS_BOOL bMainWin, MS_U16 u16val, MS_U16 u16Mask);
void Hal_PQ_set_420upSample(void *pInstance,MS_U16 u16value);
void Hal_PQ_set_force_y_motion(void *pInstance,MS_BOOL bMainWin, MS_U16 u16value);
MS_U8 Hal_PQ_get_force_y_motion(void *pInstance,MS_BOOL bMainWin);
void Hal_PQ_set_force_c_motion(void *pInstance,MS_BOOL bMainWin, MS_U16 u16value);
MS_U8 Hal_PQ_get_force_c_motion(void *pInstance,MS_BOOL bMainWin);
void Hal_PQ_set_dipf_temporal(void *pInstance,MS_BOOL bMainWin, MS_U16 u16val);
MS_U16 Hal_PQ_get_dipf_temporal(void *pInstance,MS_BOOL bMainWin);
void Hal_PQ_set_dipf_spatial(void *pInstance,MS_BOOL bMainWin, MS_U16 u16val);
MS_U8 Hal_PQ_get_dipf_spatial(void *pInstance,MS_BOOL bMainWin);
void Hal_PQ_set_vsp_sram_filter(void *pInstance,MS_U8 u8vale);
MS_U8 Hal_PQ_get_vsp_sram_filter(void *pInstance);
void Hal_PQ_set_dnr(void *pInstance,MS_BOOL bMainWin, MS_U8 u8val);
MS_U8 Hal_PQ_get_dnr(void *pInstance,MS_BOOL bMainWin);
void Hal_PQ_set_presnr(void *pInstance,MS_BOOL bMainWin, MS_U8 u8val);
MS_U8 Hal_PQ_get_presnr(void *pInstance,MS_BOOL bMainWin);
void Hal_PQ_set_film(void *pInstance,MS_BOOL bMainWin, MS_U16 u16val);
MS_U8 Hal_PQ_get_film(void *pInstance,MS_BOOL bMainWin);
void Hal_PQ_set_c_sram_table(void *pInstance,MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_y_sram_table(void *pInstance,MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_c_sram_table_Main2(void *pInstance,MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_y_sram_table_Main2(void *pInstance,MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_sram_color_index_table(void *pInstance,MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_sram_color_gain_snr_table(void *pInstance,MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_sram_color_gain_dnr_table(void *pInstance,MS_U8 u8sramtype, void *pData);
MS_U16 Hal_PQ_get_hnonlinear_idx(void *pInstance,MS_HNONLINEAR_TYPE etype);
MS_U8 Hal_PQ_get_madi_fbl_mode(void *pInstance,MS_BOOL bMemFmt422,MS_BOOL bInterlace);
MS_U16 Hal_PQ_get_sram_size(void *pInstance,MS_U16 u16sramtype);
void Hal_PQ_OPWriteOff_Enable(void *pInstance,MS_BOOL bEna);
void Hal_PQ_set_sram_ihc_crd_table(void *pInstance,MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_sram_icc_crd_table(void *pInstance,MS_U8 u8sramtype, void *pData);
MS_U16 Hal_PQ_get_rw_method(void *pInstance,MS_BOOL bMainWin);
void Hal_PQ_set_rw_method(void *pInstance,MS_BOOL bMainWin, MS_U16 u16method);
void Hal_PQ_set_xvycc_matrix_coefficient(void *pInstance,MS_S32 pData[][3], MS_BOOL bMainWin);
void Hal_PQ_set_xvycc_matrix_enable(void *pInstance,MS_BOOL bEnable, MS_BOOL bMainWin);
MS_BOOL HAL_PQ_user_mode_control(void *pInstance);
void HAL_PQ_set_SelectCSC(void *pInstance,MS_U16 u16selection, MS_BOOL  bMainWin);
void Hal_PQ_set_sram_xvycc_gamma_table(void *pInstance,MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_UFSC_c_sram_table(void *pInstance, MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_UFSC_y_sram_table(void *pInstance, MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_UFSC_sram_ihc_crd_table(void *pInstance, MS_U8 u8sramtype, void *pData);
void Hal_PQ_set_UFSC_sram_icc_crd_table(void *pInstance, MS_U8 u8sramtype, void *pData);
MS_BOOL Hal_PQ_get_cfd_info(MS_U16 u16PqIndex, MS_U8 *pu8ColorFormat, MS_U8 *pu8ColorDataFormat, MS_BOOL *pbIsFullRange, MS_U8 *pu8ColorPrimaries, MS_U8 *pu8TransferCharacteristics, MS_U8 *pu8MatrixCoefficients);
#endif


