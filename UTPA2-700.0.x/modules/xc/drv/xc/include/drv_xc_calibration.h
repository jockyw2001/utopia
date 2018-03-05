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
#ifndef DRV_XC_CALIBRATION_H
#define DRV_XC_CALIBRATION_H

#ifdef __cplusplus
extern "C"
{
#endif

////////////////////////////////////////////////////////////////////////////////
///Calibration
// the data path is BRI -> GAMMA -> CON_BRI -> DITHER
typedef enum
{
    AFTER_BRI,
    AFTER_CON_BRI,
    AFTER_GAMMA,
    AFTER_DITHER,
    AFTER_ADC,
} MS_IMAGE_CAP_POSITION;

typedef enum
{
    IMAGE_CAPTURE_DISABLE_CSC,
    IMAGE_CAPTURE_ENABLE_CSC,
} MS_IMAGE_CAP_CSC;

typedef enum
{
    CALIBRATION_STEP_INIT,
    CALIBRATION_STEP_CAPTURE,
    CALIBRATION_STEP_READ_MEMORY,
    CALIBRATION_STEP_END,
} MS_CALIBRATION_STEP;

/// ADC setting
typedef struct
{
    MS_U8 RedGain;      ///< ADC red gain
    MS_U8 GreenGain;    ///< ADC green gain
    MS_U8 BlueGain;     ///< ADC blue gain
    MS_U8 RedOffset;    ///< ADC red offset
    MS_U8 GreenOffset;  ///< ADC green offset
    MS_U8 BlueOffset;   ///< ADC blue offset
} MS_POST_CALIBRATION_SETTING;

typedef struct
{
    MS_U8   u8CapPosition;
    MS_U8   u8DIMode, u8F2OutputDataCfg;
    MS_U8   u8NREnable;
    MS_PHY  u32DNRBase0, u32DNRBase1, u32DNRWriteLimit;
    MS_U16  u16DNRFetch, u16DNROffset;
    MS_U16  u16VLimit;
    MS_U16  u16INT14, u16INT15;
    MS_U8   u8IPMUX, u8CHIPTOP;
    MS_U8   u8CapSrc, u8Glitch, u8IPDESetting, u8DEEnable;
    MS_U8   u8InterlaceDetect;
    MS_U16  u16HCapStart, u16HCapSize, u16VCapStart, u16VCapSize;
    MS_U8   u8CSC;
    MS_U8   u8PreV, u8PreH;
    MS_U8   u8444_422Filter, u8IP422Format, u8F2InputDataCfg, u8Force10bits;
    MS_U8   u8MemFmt;
    MS_U8   u8MACE;
    MS_U8   u8misc;
} MS_IMAGE_CAP_SETTING_TYPE;

typedef struct
{
    MS_IMAGE_CAP_CSC           eCapEnableCSC;
    MS_IMAGE_STORE_FMT         eStoreFmt;
    MS_IMAGE_CAP_POSITION      eCapPosition;
    MS_U16 u16HCapStart;
    MS_U16 u16VCapStart;
    MS_U16 u16HCapSize;
    MS_U16 u16VCapSize;
    MS_U16 u16CalibrationX0;
    MS_U16 u16CalibrationX1;
    MS_U16 u16CalibrationXLen;
    MS_U16 u16CalibrationY;
    MS_U16 u16CalibrationYLen;
    MS_IMAGE_MEM_INFO stMemInfo;
    MS_IMAGE_CAP_SETTING_TYPE stImageCapSetting;
} MS_IMAGE_CAP_INFO;





#ifdef MSOS_TYPE_LINUX_KERNEL
//extern MS_BOOL g_bDoCalibration;
#else
//MS_BOOL g_bDoCalibration;
#endif


//void MDrv_Calibration_Capture_Image( MS_CALIBRATION_STEP eCalibrationStep, MS_IMAGE_CAP_INFO *ptrCapInfo );
MS_AUTOADC_TYPE MDrv_XC_GetAverageData(void *pInstance, MS_PIXEL_32BIT *sMemBuf, MS_AUTO_CAL_WINDOW_t pCalWin, SCALER_WIN eWindow);
void MApi_XC_ReadRgbData(void *pInstance, MS_IMAGE_MEM_INFO * pMemInfo);
void MApi_XC_ReadVOPData(void *pInstance, MS_IMAGE_MEM_INFO * pMemInfo);
#ifdef __cplusplus
}
#endif

#endif /* DRV_XC_CALIBRATION_H */

