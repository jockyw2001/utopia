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
#define _MSACE_C_

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "apiXC_Ace.h"
#include "mhal_ace.h"
#include "ace_hwreg_utility2.h"
#include "MsAce_LIB.h"
#include "drvACE.h"
#ifdef MSOS_TYPE_LINUX
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h> // O_RDWR
#endif
#include "../xc/mdrv_xc_io.h"
#include "UFO.h"

//--------------------------------------------------------------------------------------------------
// msACE version history:
// v100(2008-04-16): Add version infromation
//       Change internal contrast/RGB gain variable to WORD(12bits)
//       Add new function for adjust contrast/RGB gain
//          void msAdjustVideoContrast_10Bits( BOOL bScalerWin, WORD wContrast );
//          void msAdjustVideoRGB_10Bits( BOOL bScalerWin, WORD ucRCon, WORD ucGCon, WORD ucBCon);
//          void msAdjustPCContrast_10Bits( BOOL bScalerWin, WORD wContrast );
//          void msAdjustPCRGB_10Bits( BOOL bScalerWin, WORD wRed, WORD wGreen, WORD wBlue );
// v101(2008-04-16): Modify msACE_GetVersion()
// v102(2008-09-04):
//  1.Fine tune color correction control.
//  2.Fine tune sRGB control
// v103(2008-09-08):
//  1.Add new function for get more infromation from ACE library
//  2.Seperate chip relative code to another file.
// v104(2008-09-17):
//  1.Use WriteByteMask to access REG_ADDR_CM_CTL_MAIN
//  2.Fix msACE_GetInfo() uncall warning.
// v105(2009-07-14):
//  1.Fine tune the define in LIB_Group.
//  2.Add new group for external chip.
// v106(2009-10-28):
//  1.Add C51 version info.
//  2.Add debug-info-YUVtoRGB Matrix selection
//  3.Add debug function to get Matrix
//  4.Add option for using internal sin/cos function.
//  5.Change some function interface to static.
// v107(2010-01-26):
//  1.Refine Sine&Cosine function: Using lookup table.
// v108(2010-04-13):
//  1.Enable Sine&Cosine function using lookup table.
//  2.Add 2 function: msACE_SetColorMatrixControl(), msACE_SetRBChannelRange()
#define MSACE_VERSION   (108)
//----------------------------------------------------------------------------------------------------

#if defined(__C51__)
#define STR_C51_VERSION(x) "C51:"##  #x

#define C51_VERSION(x) STR_C51_VERSION(x)

char* ace_c51version = C51_VERSION(__C51__);
#endif

#define MAIN_WINDOW     0
#define SUB_WINDOW      1
#define CONTRAST_MIN_VALUE 0x400
#define CONTRAST_MAX_VALUE 0x7FF

typedef short Typ3x3Array[3][3];

#if 0
typedef struct
{
    WORD wContrast;
    WORD wRCon;
    WORD wGCon;
    WORD wBCon;
    BYTE ucSaturation;
    BYTE ucHue;
    short sContrastRGBMatrix[3][3];
    short sVideoSatHueMatrix[3][3];
    short* psColorCorrectionMatrix; // this variable must pointer to a array[3][3]
    short* psPCsRGBMatrix; // sRGB matrix for PC, this variable must pointer to a array[3][3]
    short* psYVUtoRGBMatrix; // this variable must pointer to a array[3][3]
    BYTE bForceYUVtoRGB : 1;
    BYTE ucYUVtoRGBMatrixSel : 2;
}StruAceInfo;
#endif

/// CFD OSD structure version
/// Version 0: Init structure
/// Version 1: Add backlight and hue/contrast/saturation valid flag
/// Version 2: Add color range and main/sub window.
/// Version 3: Add ultra black / white
/// Version 4: Add skip picture setting (hue/ contrast/ saturation)
/// Version 5: Add color correction matrix
#define ACE_OSD_CONTROL_VERSION 5

/// Update type
typedef enum
{
    /// Update all
    E_XC_CFD_UPDATE_TYPE_ALL,
    /// Update OSD only
    E_XC_CFD_UPDATE_TYPE_OSD_ONLY,
    /// Max
    E_XC_CFD_UPDATE_TYPE_MAX
} MS_XC_CFD_UPDATE_TYPE;

//controls from Panel OSD
typedef struct
{
    MS_U32 u32Version;   ///<Version of current structure. Please always set to "ACE_OSD_CONTROL_VERSION" as input
    MS_U16 u16Length;    ///<Length of this structure, u16Length=sizeof(ST_HAL_PQ_CFD_OSD_CONTROL)

    MS_U16 u16Hue;
    MS_U16 u16Saturation;
    MS_U16 u16Contrast;

    //default on , not in the document
    MS_U8  u8OSDUIEn;

    //Mode 0: update matrix by OSD and color format driver
    //Mode 1: only update matrix by OSD controls
    //for mode1 : the configures of matrix keep the same as the values by calling CFD last time
    MS_U8  u8OSDUIMode;

    ///
    MS_BOOL bHueValid;
    MS_BOOL bSaturationValid;
    MS_BOOL bContrastValid;

    MS_BOOL bBacklightValid;
    MS_U32 u32MinBacklight;
    MS_U32 u32MaxBacklight;
    MS_U32 u32Backlight;

    MS_U8 u8Win;
    MS_BOOL bColorRangeValid;
    MS_U8 u8ColorRange; // 0: Auto, 1: Full Range (0~255), 2: Limit Range (16~235)

    MS_BOOL bUltraBlackLevelValid;
    MS_U8 u8UltraBlackLevel;
    MS_BOOL bUltraWhiteLevelValid;
    MS_U8 u8UltraWhiteLevel;

    MS_BOOL bSkipPictureSettingValid;
    MS_BOOL bSkipPictureSetting;

    MS_BOOL bColorCorrectionValid;
    MS_S16 s16ColorCorrectionMatrix[32];

    MS_BOOL bYVUtoRGBMatrixValid;
    MS_S16 s16YVUtoRGBMatrix[32];

    MS_U16 u16R;
    MS_U16 u16G;
    MS_U16 u16B;

    MS_BOOL bR;
    MS_BOOL bG;
    MS_BOOL bB;
} ST_ACE_CFD_OSD_CONTROL;

typedef struct
{
    /// Structure version
    MS_U32 u32Version;
    /// Structure length
    MS_U16 u16Length;

    /// Window (main or sub window)
    MS_U8 u8Win;
    /// Input source, reference PQ_INPUT_SOURCE_TYPE in apxXC.h
    MS_U8 u8InputSource;
    /// Update type, reference MS_XC_CFD_UPDATE_TYPE
    MS_U8 u8UpdateType;
    /// RGB bypass
    MS_BOOL bIsRgbBypass;
    /// HD mode
    MS_BOOL bIsHdMode;
} ST_ACE_CFD_FIRE_CONTROL;

#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_MSTAR_XC_HDR_SUPPORT))
extern MS_BOOL MDrv_XC_CFDControl(ST_KDRV_XC_CFD_CONTROL_INFO *pstKdrvCFDCtrlInfo);
#endif

#include "ACE_private.h"
#include "utopia_dapi.h"
#include "utopia.h"

extern void* pu32ACEInst;
extern void* pu32ACERes[E_ACE_POOL_ID_MAX];

#if 0
static StruAceInfo s_AceInfo[XC_ACE_EX_MAX_DEVICE_NUM][2];
static BOOL bBypassColorMatrix[XC_ACE_EX_MAX_DEVICE_NUM];
#else
#define s_AceInfo psACEResPri->stdrvACE.s_AceInfo
#define bBypassColorMatrix psACEResPri->stdrvACE.bBypassColorMatrix

#endif

extern ACE_DRV_HDRinit g_ACEinitParameters;
static MS_BOOL _bSetHDRInit=FALSE;
static MS_BOOL _bSetGenRead=FALSE;

static MS_U16 s16PreRegs[9]={0};
static MS_U16 _u16HdrY2RRatio = 2;
static MS_U8 _u8PreMatrixCoefficients=0;
static MS_U8 _u8PreColorimetry = 0;
static MS_U8 _u8PreExtendedColorimetry = 0;
#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
#ifdef MSOS_TYPE_LINUX
static MS_S32   _s32FdScaler = -1;
#endif
#endif
#endif
///////////////////////////////////////////////////////////////////////////////////////////////

void msAdjustPCContrast_10Bits(void* pInstance, BOOL bScalerWin, WORD wContrast );
void msAdjustVideoContrast_10Bits(void* pInstance, BOOL bScalerWin, WORD wContrast );

///////////////////////////////////////////////////////////////////////////////////////////////
WORD msACE_GetVersion(void)
{
    return MSACE_VERSION;
}

static WORD _TransferColorCorrectionOffsetValue(short s)
{
    WORD w;

    w = s;
    if( w > 0x7F )
    {
        w = s*-1;
        w |= 0x80;
    }
    return w;
}
WORD msACE_GetInfo(void* pInstance, BOOL bWindow, EnuACEInfoType ucInfoType )
{
    WORD wReturnValue = 0xFFFF;
    _ACE_GET_VARIABLE();

    switch( ucInfoType )
    {
        case ACE_INFO_TYPE_VERSION:
            wReturnValue = MSACE_VERSION;
            break;
        case ACE_INFO_TYPE_CONTRAST:
            wReturnValue = s_AceInfo[bWindow].wContrast;
            break;
        case ACE_INFO_TYPE_CONTRAST_MIN_VALUE:
            wReturnValue = CONTRAST_MIN_VALUE;
            break;
        case ACE_INFO_TYPE_CONTRAST_MAX_VALUE:
            wReturnValue = CONTRAST_MAX_VALUE;
            break;
        case ACE_INFO_TYPE_R_GAIN:
            wReturnValue = s_AceInfo[bWindow].wRCon;
            break;
        case ACE_INFO_TYPE_G_GAIN:
            wReturnValue = s_AceInfo[bWindow].wGCon;
            break;
        case ACE_INFO_TYPE_B_GAIN:
            wReturnValue = s_AceInfo[bWindow].wBCon;
            break;
        case ACE_INFO_TYPE_SATURATION:
            wReturnValue = s_AceInfo[bWindow].ucSaturation;
            break;
        case ACE_INFO_TYPE_HUE:
            wReturnValue = s_AceInfo[bWindow].ucHue;
            break;
        case ACE_INFO_TYPE_COLOR_CORRECTION_XY_R:
            if( s_AceInfo[bWindow].psColorCorrectionMatrix != NULL )
            {
                wReturnValue = s_AceInfo[bWindow].psColorCorrectionMatrix[26]*-1;
                wReturnValue = ((wReturnValue&0xFF00)>>4)|(wReturnValue&0xFF);
            }
            else
                wReturnValue = 0;
            break;
        case ACE_INFO_TYPE_COLOR_CORRECTION_XY_G:
            if( s_AceInfo[bWindow].psColorCorrectionMatrix != NULL )
            {
                wReturnValue = s_AceInfo[bWindow].psColorCorrectionMatrix[27];
                wReturnValue = ((wReturnValue&0xFF00)>>4)|(wReturnValue&0xFF);
            }
            else
                wReturnValue = 0;
            break;
        case ACE_INFO_TYPE_COLOR_CORRECTION_XY_B:
            if( s_AceInfo[bWindow].psColorCorrectionMatrix != NULL )
            {
                wReturnValue = s_AceInfo[bWindow].psColorCorrectionMatrix[28]*-1;
                wReturnValue = ((wReturnValue&0xFF00)>>4)|(wReturnValue&0xFF);
            }
            else
                wReturnValue = 0;
            break;
        case ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_R:
            if( s_AceInfo[bWindow].psColorCorrectionMatrix != NULL )
            {
                wReturnValue = _TransferColorCorrectionOffsetValue(s_AceInfo[bWindow].psColorCorrectionMatrix[29]);
            }
            else
                wReturnValue = 0;
            break;
        case ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_G:
            if( s_AceInfo[bWindow].psColorCorrectionMatrix != NULL )
            {
                wReturnValue = _TransferColorCorrectionOffsetValue(s_AceInfo[bWindow].psColorCorrectionMatrix[30]);
            }
            else
                wReturnValue = 0;
            break;
        case ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_B:
            if( s_AceInfo[bWindow].psColorCorrectionMatrix != NULL )
            {
                wReturnValue = _TransferColorCorrectionOffsetValue(s_AceInfo[bWindow].psColorCorrectionMatrix[31]);
            }
            else
                wReturnValue = 0;
            break;

        case ACE_INFO_TYPE_COMPILER_VERSION:
    #if defined(__C51__)
            wReturnValue = __C51__;
    #else
            wReturnValue = 0;
    #endif
            break;

        case ACE_INFO_TYPE_YUV_TO_RGB_MATRIX_SEL:
            wReturnValue = s_AceInfo[bWindow].ucYUVtoRGBMatrixSel;
            break;

        case ACE_INFO_TYPE_IHC_USER_COLOR1:
        case ACE_INFO_TYPE_IHC_USER_COLOR2:
        case ACE_INFO_TYPE_IHC_USER_COLOR3:
        case ACE_INFO_TYPE_IHC_USER_COLOR4:
        case ACE_INFO_TYPE_IHC_USER_COLOR5:
        case ACE_INFO_TYPE_IHC_USER_COLOR6:
        case ACE_INFO_TYPE_IHC_USER_COLOR7:
        case ACE_INFO_TYPE_IHC_USER_COLOR8:
        case ACE_INFO_TYPE_IHC_USER_COLOR9:
        case ACE_INFO_TYPE_IHC_USER_COLOR10:
        case ACE_INFO_TYPE_IHC_USER_COLOR11:
        case ACE_INFO_TYPE_IHC_USER_COLOR12:
        case ACE_INFO_TYPE_IHC_USER_COLOR13:
        case ACE_INFO_TYPE_IHC_USER_COLOR14:
        case ACE_INFO_TYPE_IHC_USER_COLOR15:
        case ACE_INFO_TYPE_IHC_USER_COLOR0:
            {
                MS_U8 u8Val = 0;
                XC_ACE_IHC_COLOR_TYPE eIHC;
                switch(ucInfoType)
                {
                    case ACE_INFO_TYPE_IHC_USER_COLOR1:
                        eIHC = E_XC_ACE_INFO_TYPE_IHC_USER_COLOR1;
                        break;
                    case ACE_INFO_TYPE_IHC_USER_COLOR2:
                        eIHC = E_XC_ACE_INFO_TYPE_IHC_USER_COLOR2;
                        break;
                    case ACE_INFO_TYPE_IHC_USER_COLOR3:
                        eIHC = E_XC_ACE_INFO_TYPE_IHC_USER_COLOR3;
                        break;
                    case ACE_INFO_TYPE_IHC_USER_COLOR4:
                        eIHC = E_XC_ACE_INFO_TYPE_IHC_USER_COLOR4;
                        break;
                    case ACE_INFO_TYPE_IHC_USER_COLOR5:
                        eIHC = E_XC_ACE_INFO_TYPE_IHC_USER_COLOR5;
                        break;
                    case ACE_INFO_TYPE_IHC_USER_COLOR6:
                        eIHC = E_XC_ACE_INFO_TYPE_IHC_USER_COLOR6;
                        break;
                    case ACE_INFO_TYPE_IHC_USER_COLOR7:
                        eIHC = E_XC_ACE_INFO_TYPE_IHC_USER_COLOR7;
                        break;
                    case ACE_INFO_TYPE_IHC_USER_COLOR8:
                        eIHC = E_XC_ACE_INFO_TYPE_IHC_USER_COLOR8;
                        break;
                    case ACE_INFO_TYPE_IHC_USER_COLOR9:
                        eIHC = E_XC_ACE_INFO_TYPE_IHC_USER_COLOR9;
                        break;
                    case ACE_INFO_TYPE_IHC_USER_COLOR10:
                        eIHC = E_XC_ACE_INFO_TYPE_IHC_USER_COLOR10;
                        break;
                    case ACE_INFO_TYPE_IHC_USER_COLOR11:
                        eIHC = E_XC_ACE_INFO_TYPE_IHC_USER_COLOR11;
                        break;
                    case ACE_INFO_TYPE_IHC_USER_COLOR12:
                        eIHC = E_XC_ACE_INFO_TYPE_IHC_USER_COLOR12;
                        break;
                    case ACE_INFO_TYPE_IHC_USER_COLOR13:
                        eIHC = E_XC_ACE_INFO_TYPE_IHC_USER_COLOR13;
                        break;
                    case ACE_INFO_TYPE_IHC_USER_COLOR14:
                        eIHC = E_XC_ACE_INFO_TYPE_IHC_USER_COLOR14;
                        break;
                    case ACE_INFO_TYPE_IHC_USER_COLOR15:
                        eIHC = E_XC_ACE_INFO_TYPE_IHC_USER_COLOR15;
                        break;
                    case ACE_INFO_TYPE_IHC_USER_COLOR0:
                        eIHC = E_XC_ACE_INFO_TYPE_IHC_USER_COLOR0;
                        break;
                    default:
                        break;
                }
                MDrv_ACE_GetIHC(pInstance, bWindow, eIHC, &u8Val);
                wReturnValue = u8Val;
            }
            break;


        case ACE_INFO_TYPE_ICC_USER_COLOR1:
        case ACE_INFO_TYPE_ICC_USER_COLOR2:
        case ACE_INFO_TYPE_ICC_USER_COLOR3:
        case ACE_INFO_TYPE_ICC_USER_COLOR4:
        case ACE_INFO_TYPE_ICC_USER_COLOR5:
        case ACE_INFO_TYPE_ICC_USER_COLOR6:
        case ACE_INFO_TYPE_ICC_USER_COLOR7:
        case ACE_INFO_TYPE_ICC_USER_COLOR8:
        case ACE_INFO_TYPE_ICC_USER_COLOR9:
        case ACE_INFO_TYPE_ICC_USER_COLOR10:
        case ACE_INFO_TYPE_ICC_USER_COLOR11:
        case ACE_INFO_TYPE_ICC_USER_COLOR12:
        case ACE_INFO_TYPE_ICC_USER_COLOR13:
        case ACE_INFO_TYPE_ICC_USER_COLOR14:
        case ACE_INFO_TYPE_ICC_USER_COLOR15:
        case ACE_INFO_TYPE_ICC_USER_COLOR0:
            {
                MS_U8 u8Val = 0;
                XC_ACE_ICC_COLOR_TYPE eICC;
                switch(ucInfoType)
                {
                    case ACE_INFO_TYPE_ICC_USER_COLOR1:
                        eICC = E_ACE_ICC_USER_COLOR1;
                        break;
                    case ACE_INFO_TYPE_ICC_USER_COLOR2:
                        eICC = E_ACE_ICC_USER_COLOR2;
                        break;
                    case ACE_INFO_TYPE_ICC_USER_COLOR3:
                        eICC = E_ACE_ICC_USER_COLOR3;
                        break;
                    case ACE_INFO_TYPE_ICC_USER_COLOR4:
                        eICC = E_ACE_ICC_USER_COLOR4;
                        break;
                    case ACE_INFO_TYPE_ICC_USER_COLOR5:
                        eICC = E_ACE_ICC_USER_COLOR5;
                        break;
                    case ACE_INFO_TYPE_ICC_USER_COLOR6:
                        eICC = E_ACE_ICC_USER_COLOR6;
                        break;
                    case ACE_INFO_TYPE_ICC_USER_COLOR7:
                        eICC = E_ACE_ICC_USER_COLOR7;
                        break;
                    case ACE_INFO_TYPE_ICC_USER_COLOR8:
                        eICC = E_ACE_ICC_USER_COLOR8;
                        break;
                    case ACE_INFO_TYPE_ICC_USER_COLOR9:
                        eICC = E_ACE_ICC_USER_COLOR9;
                        break;
                    case ACE_INFO_TYPE_ICC_USER_COLOR10:
                        eICC = E_ACE_ICC_USER_COLOR10;
                        break;
                    case ACE_INFO_TYPE_ICC_USER_COLOR11:
                        eICC = E_ACE_ICC_USER_COLOR11;
                        break;
                    case ACE_INFO_TYPE_ICC_USER_COLOR12:
                        eICC = E_ACE_ICC_USER_COLOR12;
                        break;
                    case ACE_INFO_TYPE_ICC_USER_COLOR13:
                        eICC = E_ACE_ICC_USER_COLOR13;
                        break;
                    case ACE_INFO_TYPE_ICC_USER_COLOR14:
                        eICC = E_ACE_ICC_USER_COLOR14;
                        break;
                    case ACE_INFO_TYPE_ICC_USER_COLOR15:
                        eICC = E_ACE_ICC_USER_COLOR15;
                        break;
                    case ACE_INFO_TYPE_ICC_USER_COLOR0:
                        eICC = E_ACE_ICC_USER_COLOR0;
                        break;
                    default:
                        break;
                }
                MDrv_ACE_GetICC(pInstance, bWindow, eICC, &u8Val);
                wReturnValue = u8Val;
            }
            break;

        case ACE_INFO_TYPE_IBC_USER_COLOR1:
        case ACE_INFO_TYPE_IBC_USER_COLOR2:
        case ACE_INFO_TYPE_IBC_USER_COLOR3:
        case ACE_INFO_TYPE_IBC_USER_COLOR4:
        case ACE_INFO_TYPE_IBC_USER_COLOR5:
        case ACE_INFO_TYPE_IBC_USER_COLOR6:
        case ACE_INFO_TYPE_IBC_USER_COLOR7:
        case ACE_INFO_TYPE_IBC_USER_COLOR8:
        case ACE_INFO_TYPE_IBC_USER_COLOR9:
        case ACE_INFO_TYPE_IBC_USER_COLOR10:
        case ACE_INFO_TYPE_IBC_USER_COLOR11:
        case ACE_INFO_TYPE_IBC_USER_COLOR12:
        case ACE_INFO_TYPE_IBC_USER_COLOR13:
        case ACE_INFO_TYPE_IBC_USER_COLOR14:
        case ACE_INFO_TYPE_IBC_USER_COLOR15:
        case ACE_INFO_TYPE_IBC_USER_COLOR0:
            {
                MS_U8 u8Val = 0;
                XC_ACE_IBC_COLOR_TYPE eIBC;
                switch(ucInfoType)
                {
                    case ACE_INFO_TYPE_IBC_USER_COLOR1:
                        eIBC = E_ACE_IBC_USER_COLOR1;
                        break;
                    case ACE_INFO_TYPE_IBC_USER_COLOR2:
                        eIBC = E_ACE_IBC_USER_COLOR2;
                        break;
                    case ACE_INFO_TYPE_IBC_USER_COLOR3:
                        eIBC = E_ACE_IBC_USER_COLOR3;
                        break;
                    case ACE_INFO_TYPE_IBC_USER_COLOR4:
                        eIBC = E_ACE_IBC_USER_COLOR4;
                        break;
                    case ACE_INFO_TYPE_IBC_USER_COLOR5:
                        eIBC = E_ACE_IBC_USER_COLOR5;
                        break;
                    case ACE_INFO_TYPE_IBC_USER_COLOR6:
                        eIBC = E_ACE_IBC_USER_COLOR6;
                        break;
                    case ACE_INFO_TYPE_IBC_USER_COLOR7:
                        eIBC = E_ACE_IBC_USER_COLOR7;
                        break;
                    case ACE_INFO_TYPE_IBC_USER_COLOR8:
                        eIBC = E_ACE_IBC_USER_COLOR8;
                        break;
                    case ACE_INFO_TYPE_IBC_USER_COLOR9:
                        eIBC = E_ACE_IBC_USER_COLOR9;
                        break;
                    case ACE_INFO_TYPE_IBC_USER_COLOR10:
                        eIBC = E_ACE_IBC_USER_COLOR10;
                        break;
                    case ACE_INFO_TYPE_IBC_USER_COLOR11:
                        eIBC = E_ACE_IBC_USER_COLOR11;
                        break;
                    case ACE_INFO_TYPE_IBC_USER_COLOR12:
                        eIBC = E_ACE_IBC_USER_COLOR12;
                        break;
                    case ACE_INFO_TYPE_IBC_USER_COLOR13:
                        eIBC = E_ACE_IBC_USER_COLOR13;
                        break;
                    case ACE_INFO_TYPE_IBC_USER_COLOR14:
                        eIBC = E_ACE_IBC_USER_COLOR14;
                        break;
                    case ACE_INFO_TYPE_IBC_USER_COLOR15:
                        eIBC = E_ACE_IBC_USER_COLOR15;
                        break;
                    case ACE_INFO_TYPE_IBC_USER_COLOR0:
                        eIBC = E_ACE_IBC_USER_COLOR0;
                        break;
                    default:
                        break;
                }
                MDrv_ACE_GetIBC(pInstance, bWindow, eIBC, &u8Val);
                wReturnValue = u8Val;
            }
            break;

        case ACE_INFO_TYPE_DUMMY:
        default:
            wReturnValue = 0;
            break;
    }

    return wReturnValue;
}

// matrix used to convert RGB color space to YUV color space for SDTV
//code short tSDTVRgb2yuv[3][3] =
//{
//    {  0x0107,  0x0204,  0x0064 }, // 0.257,  0.504,  0.098
//    { -0x0098, -0x012A,  0x01C2 }, // -0.148, -0.291, 0.439
//    {  0x01C2, -0x0179, -0x0049 }  // 0.439,  -0.368, -0.071
//};

// matrix used to convert YUV color space to RGB color space, used for video input for SDTV
static code short tSDTVYuv2rgb[3][3] =
#if 0
{
    {   0x0662, 0x04A8,  0x0000 }, // 1.596,  1.164, 0
    {  -0x0341, 0x04A8, -0x0190 }, // -0.813, 1.164, -0.391
    {   0x0000, 0x04A8,  0x0812 }  // 0,      1.164, 2.018
};
#else
{
    {   0x0667, 0x04AC,  0x0000 }, // 1.6005,  1.1679, 0
    {  -0x0343, 0x04AC, -0x0192 }, // -0.8154, 1.1679, -0.3925
    {   0x0000, 0x04AC,  0x0818 }  // 0,      1.1679, 2.0234
};
#endif

// matrix used to convert RGB color space to YUV color space for HDTV
//code short tHDTVRgb2yuv[3][3] =
//{
//    {  0x00DA,  0x02DC,  0x004A }, // 0.213,  0.715,  0.072
//    { -0x0078, -0x0193,  0x020B }, // -0.117, -0.394, 0.511
//    {  0x020B, -0x01DB, -0x0030 }  // 0.511,  -0.464, -0.047
//};

// matrix used to convert YUV color space to RGB color space, used for video input for HDTV
static code short tHDTVYuv2rgb[3][3] =
#if 0
{
    {   0x072C, 0x04A8,  0x0000 }, // 1.793,  1.164, 0
    {  -0x0223, 0x04A8, -0x00DA }, // -0.534, 1.164, -0.213
    {   0x0000, 0x04A8,  0x0876 }  // 0,      1.164, 2.115
};
#else
{
    {   0x0731, 0x04AC,  0x0000 }, // 1.7978,  1.1679, 0
    {  -0x0223, 0x04AC, -0x00DB }, // -0.5341, 1.1679, -0.2138
    {   0x0000, 0x04AC,  0x0879 }  // 0,      1.1679, 2.1181
};
#endif
static void CopyMatrix(WORD* pwBufSrc, WORD* pwBufTgt, BYTE ucLen )
{
    BYTE i;
    for( i = 0; i < ucLen; ++ i )
    {
        pwBufTgt[i] = pwBufSrc[i];
    }
}
BOOL msACE_GetMatrix(void* pInstance, BOOL bWindow, BYTE ucMatrixId, WORD* pwBuf, BYTE ucBufLen)
{
    BYTE ucCopyLen = 9;

    _ACE_GET_VARIABLE();

    // Check parameter ...

    //if( ucBufLen < ucCopyLen )
        ucCopyLen = ucBufLen;

    switch(ucMatrixId)
    {
        case ACE_MATRIX_ID__YUV_TO_RGB:
            if( s_AceInfo[bWindow].ucYUVtoRGBMatrixSel == ACE_YUV_TO_RGB_MATRIX_SDTV )
            {
                CopyMatrix( (WORD*)tSDTVYuv2rgb, pwBuf, ucCopyLen );
            }
            else if( s_AceInfo[bWindow].ucYUVtoRGBMatrixSel == ACE_YUV_TO_RGB_MATRIX_HDTV )
            {
                CopyMatrix( (WORD*)tHDTVYuv2rgb, pwBuf, ucCopyLen );
            }
            else if( s_AceInfo[bWindow].ucYUVtoRGBMatrixSel == ACE_YUV_TO_RGB_MATRIX_USER )
            {
                if( s_AceInfo[bWindow].psYVUtoRGBMatrix == NULL )
                    return FALSE;
                CopyMatrix( (WORD*)s_AceInfo[bWindow].psYVUtoRGBMatrix, pwBuf, ucCopyLen );
            }
            break;
        case ACE_MATRIX_ID__COLOR_CORRECTION:
            if( s_AceInfo[bWindow].psColorCorrectionMatrix == NULL)
                return FALSE;
            CopyMatrix( (WORD*)s_AceInfo[bWindow].psColorCorrectionMatrix, pwBuf, ucCopyLen );
            break;

        case ACE_MATRIX_ID__SRGB:
            if( s_AceInfo[bWindow].psPCsRGBMatrix == NULL)
                return FALSE;
            CopyMatrix( (WORD*)s_AceInfo[bWindow].psPCsRGBMatrix, pwBuf, ucCopyLen );
            break;
    }
    return TRUE;
}

void msACEPCForceYUVtoRGB(void* pInstance, BOOL bWin, BOOL bEnable )
{
    _ACE_GET_VARIABLE();
    if( bEnable )
        s_AceInfo[bWin].bForceYUVtoRGB = 1;
    else
        s_AceInfo[bWin].bForceYUVtoRGB = 0;
}
void msACESelectYUVtoRGBMatrix(void* pInstance, BOOL bWindow, BYTE ucMatrix, short* psUserYUVtoRGBMatrix )
{
    DECLARA_BANK_VARIABLE

    _ACE_GET_VARIABLE();

    s_AceInfo[bWindow].ucYUVtoRGBMatrixSel = ucMatrix;
    if( ucMatrix == ACE_YUV_TO_RGB_MATRIX_USER )
    {
		//printf("Color table User mode\n");

        if(DISABLE==g_ACEinitParameters.bHDREnable)
            //s_AceInfo[bWindow].psYVUtoRGBMatrix = psUserYUVtoRGBMatrix;
            memcpy(s_AceInfo[bWindow].psYVUtoRGBMatrix,psUserYUVtoRGBMatrix,sizeof(s_AceInfo[bWindow].psYVUtoRGBMatrix));
    }
    else if( ucMatrix == ACE_YUV_TO_RGB_MATRIX_HDTV )
    {
		//printf("Color table HD mode\n");
	    BACKUP_SC_BANK
		SET_SC_BANK(BANK_ACE);
		if(bWindow==0) // 0: main, 1:sub
		{
			//printf("Prepare Load Main color table\n");
	    	if(ACE_READ_CTL(psACEInstPri->u32DeviceID, REG_ADDR_ACE_R2Y_EQ_SEL,0x04)) // (BK18_6F[2] for main)
    		{
				//printf("Load SD table\n");
				if(DISABLE==g_ACEinitParameters.bHDREnable)
    			    //s_AceInfo[bWindow].psYVUtoRGBMatrix = (short*)tSDTVYuv2rgb;
                    memcpy(s_AceInfo[bWindow].psYVUtoRGBMatrix,(short*)tSDTVYuv2rgb,sizeof(s_AceInfo[bWindow].psYVUtoRGBMatrix));
    		}
			else
			{
				//printf("Load HD table\n");
				if(DISABLE==g_ACEinitParameters.bHDREnable)
				    //s_AceInfo[bWindow].psYVUtoRGBMatrix = (short*)tHDTVYuv2rgb;
                    memcpy(s_AceInfo[bWindow].psYVUtoRGBMatrix,(short*)tHDTVYuv2rgb,sizeof(s_AceInfo[bWindow].psYVUtoRGBMatrix));
			}
		}
		else
		{
			//printf("Prepare Load Sub color table\n");
	    	if(ACE_READ_CTL(psACEInstPri->u32DeviceID, REG_ADDR_ACE_R2Y_EQ_SEL,0x40))//(BK18_6F[6] for sub)
    		{
				//printf("Load SD table\n");
				if(DISABLE==g_ACEinitParameters.bHDREnable)
    			    //s_AceInfo[bWindow].psYVUtoRGBMatrix = (short*)tSDTVYuv2rgb;
                    memcpy(s_AceInfo[bWindow].psYVUtoRGBMatrix,(short*)tSDTVYuv2rgb,sizeof(s_AceInfo[bWindow].psYVUtoRGBMatrix));
    		}
			else
			{
				//printf("Load HD table\n");
				if(DISABLE==g_ACEinitParameters.bHDREnable)
				    //s_AceInfo[bWindow].psYVUtoRGBMatrix = (short*)tHDTVYuv2rgb;
                    memcpy(s_AceInfo[bWindow].psYVUtoRGBMatrix,(short*)tHDTVYuv2rgb,sizeof(s_AceInfo[bWindow].psYVUtoRGBMatrix));
			}
		}
	    RECOVER_SC_BANK
    }
    else // ACE_YUV_TO_RGB_MATRIX_SDTV
    {
		//printf("Color table SD mode\n");
		if(DISABLE==g_ACEinitParameters.bHDREnable)
            //s_AceInfo[bWindow].psYVUtoRGBMatrix = (short*)tSDTVYuv2rgb;
            memcpy(s_AceInfo[bWindow].psYVUtoRGBMatrix,(short*)tSDTVYuv2rgb,sizeof(s_AceInfo[bWindow].psYVUtoRGBMatrix));
    }
}
//---------------------------------------------------------------------------
// Specify the color correction table.
//---------------------------------------------------------------------------
void msACESetColorCorrectionTable(void* pInstance, BOOL bWindow, short* psColorCorrectionTable )
{
    _ACE_GET_VARIABLE();
    if(psColorCorrectionTable!=NULL)
    {
        memcpy(s_AceInfo[bWindow].psColorCorrectionMatrix,psColorCorrectionTable,sizeof(MS_S16)*32);

        ST_ACE_CFD_OSD_CONTROL stAceOSDControl;
        memset(&stAceOSDControl, 0, sizeof(ST_ACE_CFD_OSD_CONTROL));
        stAceOSDControl.u32Version = ACE_OSD_CONTROL_VERSION;
        stAceOSDControl.u16Length = sizeof(ST_ACE_CFD_OSD_CONTROL);
        stAceOSDControl.bColorCorrectionValid = TRUE;
        memcpy(stAceOSDControl.s16ColorCorrectionMatrix, psColorCorrectionTable, sizeof(MS_S16)*32);
        stAceOSDControl.u8OSDUIEn = 1;
        stAceOSDControl.u8OSDUIMode = 1;
        stAceOSDControl.u8Win = (bWindow==TRUE)?1:0;

        ST_KDRV_XC_CFD_CONTROL_INFO stCFDCtrlInfo;
        memset(&stCFDCtrlInfo, 0, sizeof(ST_KDRV_XC_CFD_CONTROL_INFO));
        stCFDCtrlInfo.enCtrlType = E_KDRV_XC_CFD_CTRL_SET_OSD;
        stCFDCtrlInfo.pParam = &stAceOSDControl;
        stCFDCtrlInfo.u32ParamLen = sizeof(ST_ACE_CFD_OSD_CONTROL);
#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
    //_s32FdScaler only defined by UFO_XC_HDR_VERSION == 2
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
        if(0 > _s32FdScaler)
        {
            _s32FdScaler = open("/dev/scaler", O_RDWR);

            if(0 > _s32FdScaler)
            {
                printf("Scaler device not opened!!!!\n");
                return ;
            }
        }

        if (ioctl(_s32FdScaler, MDRV_XC_IOC_CFDCONCTRL, &stCFDCtrlInfo))
        {
            printf("MDRV_XC_IOC_CFDCONCTRL OSD fail!!!!\n");
            return ;
        }

        if (stCFDCtrlInfo.u16ErrCode != 0)
        {
            printf("MDRV_XC_IOC_CFDCONCTRL OSD fail, errCode: %d\n", stCFDCtrlInfo.u16ErrCode);
        }
#endif
#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_MSTAR_XC_HDR_SUPPORT))
        MDrv_XC_CFDControl(&stCFDCtrlInfo);
#endif
#endif
#endif



    }
}

//---------------------------------------------------------------------------
// Specify the sRGB table.
//---------------------------------------------------------------------------
void msACESetPCsRGBTable(void* pInstance, BOOL bWin, short* psPCsRGBTable )
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bWin].psPCsRGBMatrix = psPCsRGBTable;
}

void InitACEVar(void* pInstance)
{
    BYTE i;
    _ACE_GET_VARIABLE();

    for( i = 0; i < 2; ++ i )
    {
        s_AceInfo[i].wRCon = 1024;
        s_AceInfo[i].wGCon = 1024;
        s_AceInfo[i].wBCon = 1024;
        s_AceInfo[i].wContrast = 1024;
        s_AceInfo[i].ucSaturation = 0x80;
        s_AceInfo[i].ucHue = 50;

        s_AceInfo[i].sContrastRGBMatrix[0][0] =
        s_AceInfo[i].sContrastRGBMatrix[1][1] =
        s_AceInfo[i].sContrastRGBMatrix[2][2] = 1024;
        s_AceInfo[i].sContrastRGBMatrix[0][1] =
        s_AceInfo[i].sContrastRGBMatrix[1][0] =
        s_AceInfo[i].sContrastRGBMatrix[2][0] =
        s_AceInfo[i].sContrastRGBMatrix[0][2] =
        s_AceInfo[i].sContrastRGBMatrix[1][2] =
        s_AceInfo[i].sContrastRGBMatrix[2][1] = 0;

        s_AceInfo[i].sVideoSatHueMatrix[0][0] =
        s_AceInfo[i].sVideoSatHueMatrix[1][1] =
        s_AceInfo[i].sVideoSatHueMatrix[2][2] = 1024;
        s_AceInfo[i].sVideoSatHueMatrix[0][1] =
        s_AceInfo[i].sVideoSatHueMatrix[1][0] =
        s_AceInfo[i].sVideoSatHueMatrix[2][0] =
        s_AceInfo[i].sVideoSatHueMatrix[0][2] =
        s_AceInfo[i].sVideoSatHueMatrix[1][2] =
        s_AceInfo[i].sVideoSatHueMatrix[2][1] = 0;

        //psColorCorrectionMatrix change type from a pointer to array,
        //we init this array by a elementary array instead of zero array
        s_AceInfo[i].psColorCorrectionMatrix[0] = 0x400;
        s_AceInfo[i].psColorCorrectionMatrix[4] = 0x400;
        s_AceInfo[i].psColorCorrectionMatrix[8] = 0x400;

        //s_AceInfo[i].psColorCorrectionMatrix = NULL;
        msACESetColorCorrectionTable(pInstance, i, NULL );

        //s_AceInfo[i].psPCsRGBMatrix = NULL;
        msACESetPCsRGBTable(pInstance, i, NULL );

        //s_AceInfo[i].bForceYUVtoRGB = 0;
        msACEPCForceYUVtoRGB(pInstance, i, FALSE );

        //s_AceInfo[i].ucYUVtoRGBMatrixSel = ACE_YUV_TO_RGB_MATRIX_SDTV;
        //s_AceInfo[i].psYVUtoRGBMatrix = (short*)tSDTVYuv2rgb;
        msACESelectYUVtoRGBMatrix(pInstance, i, ACE_YUV_TO_RGB_MATRIX_SDTV, NULL );
    }

#if defined(__C51__)
    // For uncall warning...
    if( i == 0 )
    {
        msACE_GetVersion();
        msACE_GetInfo(pInstance, 0, 0);
        msAdjustPCContrast_10Bits(pInstance, 0,0);
        msAdjustPCRGB_10Bits(pInstance, 0, 0, 0, 0 );
        msAdjustVideoContrast_10Bits(pInstance, 0,0);
        msAdjustVideoRGB_10Bits(pInstance, 0, 0, 0, 0 );
        msACE_GetMatrix(pInstance, 0, 0, NULL, 0);
        msACE_SetColorMatrixControl(pInstance, 0, 0 );
        msACE_SetRBChannelRange(pInstance, 0, 0 );
    }
#endif
}

#define ENABLE_SINE_COSINE_USING_LOOKUP_TABLE   1

#if( ENABLE_SINE_COSINE_USING_LOOKUP_TABLE ) // sine/cosine use lookup table

static short sine(BYTE u8Hue)
{
    code short tHueToSine[] =
    {
        784, // 0
        772, // 1
        760, // 2
        748, // 3
        736, // 4
        724, // 5
        711, // 6
        698, // 7
        685, // 8
        671, // 9
        658, // 10
        644, // 11
        630, // 12
        616, // 13
        601, // 14
        587, // 15
        572, // 16
        557, // 17
        542, // 18
        527, // 19
        511, // 20
        496, // 21
        480, // 22
        464, // 23
        448, // 24
        432, // 25
        416, // 26
        400, // 27
        383, // 28
        366, // 29
        350, // 30
        333, // 31
        316, // 32
        299, // 33
        282, // 34
        265, // 35
        247, // 36
        230, // 37
        212, // 38
        195, // 39
        177, // 40
        160, // 41
        142, // 42
        124, // 43
        107, // 44
        89, // 45
        71, // 46
        53, // 47
        35, // 48
        17, // 49
        0, // 50
        -17, // 51
        -35, // 52
        -53, // 53
        -71, // 54
        -89, // 55
        -107, // 56
        -124, // 57
        -142, // 58
        -160, // 59
        -177, // 60
        -195, // 61
        -212, // 62
        -230, // 63
        -247, // 64
        -265, // 65
        -282, // 66
        -299, // 67
        -316, // 68
        -333, // 69
        -350, // 70
        -366, // 71
        -383, // 72
        -400, // 73
        -416, // 74
        -432, // 75
        -448, // 76
        -464, // 77
        -480, // 78
        -496, // 79
        -512, // 80
        -527, // 81
        -542, // 82
        -557, // 83
        -572, // 84
        -587, // 85
        -601, // 86
        -616, // 87
        -630, // 88
        -644, // 89
        -658, // 90
        -671, // 91
        -685, // 92
        -698, // 93
        -711, // 94
        -724, // 95
        -736, // 96
        -748, // 97
        -760, // 98
        -772, // 99
        -784, // 100
    };
    if( u8Hue > 100 )
        u8Hue = 100;
    return tHueToSine[u8Hue];
}
// cos function
static short cosine(BYTE u8Hue)
{
    code short tHueToCosine[] =
    {
        658, // 0
        671, // 1
        685, // 2
        698, // 3
        711, // 4
        724, // 5
        736, // 6
        748, // 7
        760, // 8
        772, // 9
        784, // 10
        795, // 11
        806, // 12
        817, // 13
        828, // 14
        838, // 15
        848, // 16
        858, // 17
        868, // 18
        877, // 19
        886, // 20
        895, // 21
        904, // 22
        912, // 23
        920, // 24
        928, // 25
        935, // 26
        942, // 27
        949, // 28
        955, // 29
        962, // 30
        968, // 31
        973, // 32
        979, // 33
        984, // 34
        989, // 35
        993, // 36
        997, // 37
        1001, // 38
        1005, // 39
        1008, // 40
        1011, // 41
        1014, // 42
        1016, // 43
        1018, // 44
        1020, // 45
        1021, // 46
        1022, // 47
        1023, // 48
        1023, // 49
        1024, // 50
        1023, // 51
        1023, // 52
        1022, // 53
        1021, // 54
        1020, // 55
        1018, // 56
        1016, // 57
        1014, // 58
        1011, // 59
        1008, // 60
        1005, // 61
        1001, // 62
        997, // 63
        993, // 64
        989, // 65
        984, // 66
        979, // 67
        973, // 68
        968, // 69
        962, // 70
        955, // 71
        949, // 72
        942, // 73
        935, // 74
        928, // 75
        920, // 76
        912, // 77
        904, // 78
        895, // 79
        886, // 80
        877, // 81
        868, // 82
        858, // 83
        848, // 84
        838, // 85
        828, // 86
        817, // 87
        806, // 88
        795, // 89
        784, // 90
        772, // 91
        760, // 92
        748, // 93
        736, // 94
        724, // 95
        711, // 96
        698, // 97
        685, // 98
        671, // 99
        658, // 100
    };
    if( u8Hue > 100 )
        u8Hue = 100;
    return tHueToCosine[u8Hue];
}

#else

#define PI  (3.14159265358979323846)

#if( USE_ACE_INTERNAL_SIN_COS ) // Use user define
static double _ACE_sin( double f0)
{
    double  f2, f4;
    int     r0;

    f0 *= 6.3661977236758134e-01;
    if (f0 < 0)
    {
        if (f0 < -1073741824.0)
        {

            if (f0 < -1.0e+18)
                    f0 = 0;

            f0 /= 1073741824.0;
            r0 = (int)f0;

            f2 = (double) r0;
            f0 -= f2;
            f0 *= 1073741824.0;
        }

        f2 = f0;
        f2 -= 0.5;
    }
    else
    {
        if (f0 >= 1073741824.0)
        {
            if (f0 > 1.0e+18)
                f0 = 0;
            f0 /= 1073741824.0;

            r0 = f0;
            f2 = (double) r0;
            f0 -= f2;
            f0 *= 1073741824.0;
        }

        f2 = f0;
        f2 += 0.5;
    }

    r0 = (int) f2;
    f2 = (double) r0;
    f0 -= f2;
    if (r0 & 1)
    {
        f0 *= f0;
        f2 = 6.5659631149794723e-11;
        f2 *= f0;
        f2 += -6.3866030837918522e-09;
        f2 *= f0;
        f2 += 4.7108747788181715e-07;
        f2 *= f0;
        f2 += -2.5202042373060605e-05;
        f2 *= f0;
        f2 += 9.1926027483942659e-04;
        f2 *= f0;
        f2 += -2.0863480763352961e-02;
        f2 *= f0;
        f2 += 2.5366950790104802e-01;
        f2 *= f0;
        f2 += -1.2337005501361698e+00;
        f0 *= f2;
        f0 += 1.0;
    }
    else
    {
        f4 = f0;
        f0 *= f0;
        f2 = -6.6880351098114673e-10;
        f2 *= f0;
        f2 += 5.6921729219679268e-08;
        f2 *= f0;
        f2 += -3.5988432352120853e-06;
        f2 *= f0;
        f2 += 1.6044118478735982e-04;
        f2 *= f0;
        f2 += -4.6817541353186881e-03;
        f2 *= f0;
        f2 += 7.9692626246167046e-02;
        f2 *= f0;
        f2 += -6.4596409750624625e-01;
        f0 *= f2;
        f0 += 1.5707963267948966e+00;
        f0 *= f4;
    }

    if (r0 & 2)
      f0 = -f0;

    return(f0);
}

static double _ACE_cos(double f0)
{
    double f2,f4;
    int r0;

    f0 *= 6.3661977236758134e-01;
    if (f0 < 0)
      f0 = -f0;

    if (f0 > 1073741824.0)
    {
        if (f0 > 1.0e+18)
            f0 = 0;

        f0 /= 1073741824.0;
        r0 = f0;
        f0 = (double) r0;
        f0 -= f2;
        f0 *= 1073741824.0;
    }

    f2 = f0;
    f0 += 0.5;
    r0 = (int)f0;
    f0 = (double) r0;
    f0 -= f2;

    if (r0 & 1)
    {
        f4 = f0;
        f0 *= f0;
        f2 = -6.6880351098114673e-10;
        f2 *= f0;
        f2 += 5.6921729219679268e-08;
        f2 *= f0;
        f2 += -3.5988432352120853e-06;
        f2 *= f0;
        f2 += 1.6044118478735982e-04;
        f2 *= f0;
        f2 += -4.6817541353186881e-03;
        f2 *= f0;
        f2 += 7.9692626246167046e-02;
        f2 *= f0;
        f2 += -6.4596409750624625e-01;
        f0 *= f2;
        f0 += 1.5707963267948966e+00;
        f0 *= f4;
    }
    else
    {
        f0 *= f0;
        f2 = 6.5659631149794723e-11;
        f2 *= f0;
        f2 += -6.3866030837918522e-09;
        f2 *= f0;
        f2 += 4.7108747788181715e-07;
        f2 *= f0;
        f2 += -2.5202042373060605e-05;
        f2 *= f0;
        f2 += 9.1926027483942659e-04;
        f2 *= f0;
        f2 += -2.0863480763352961e-02;
        f2 *= f0;
        f2 += 2.5366950790104802e-01;
        f2 *= f0;
        f2 += -1.2337005501361698e+00;
        f0 *= f2;
        f0 += 1.0;
    }

    if (r0 & 2)
        f0 = -f0;

    return(f0);
}
#define sin     _ACE_sin
#define cos     _ACE_cos

#endif

//#define PI  (3.14159265)
// sin function
static short sine(WORD u16Hue)
{
    return (short)(sin((u16Hue * PI) / 180.) * 1024.);
}
// cos function
static short cosine(WORD u16Hue)
{
    return (short)(cos((u16Hue * PI) / 180.) * 1024.);
}
#endif

static void SetVideoSatHueMatrix(void *pInstance, BOOL bWindow )
{
    short sTmp;
    WORD u16Hue;
    _ACE_GET_VARIABLE();

#if( ENABLE_SINE_COSINE_USING_LOOKUP_TABLE )
    u16Hue = s_AceInfo[bWindow].ucHue;
#else
    u16Hue = ((s_AceInfo[bWindow].ucHue <= 50) ? (50 - s_AceInfo[bWindow].ucHue) : (360-(s_AceInfo[bWindow].ucHue-50)));
#endif

    sTmp = ((short)s_AceInfo[bWindow].ucSaturation * 8);

    s_AceInfo[bWindow].sVideoSatHueMatrix[2][2] = ((((long)cosine(u16Hue) * sTmp))>>10);
    s_AceInfo[bWindow].sVideoSatHueMatrix[0][0] = ((( (long)cosine(u16Hue)* sTmp))>>10);
    s_AceInfo[bWindow].sVideoSatHueMatrix[2][0] = ((((long)sine(u16Hue)   * sTmp))>>10);
    s_AceInfo[bWindow].sVideoSatHueMatrix[0][2] = (((-(long)sine(u16Hue)  * sTmp))>>10);

    s_AceInfo[bWindow].sVideoSatHueMatrix[1][1] = 1024;
    s_AceInfo[bWindow].sVideoSatHueMatrix[0][1] = s_AceInfo[bWindow].sVideoSatHueMatrix[1][0] = s_AceInfo[bWindow].sVideoSatHueMatrix[1][2] = s_AceInfo[bWindow].sVideoSatHueMatrix[2][1] = 0;
}

static void SetVideoContrastMatrix(void *pInstance, BOOL bWindow )
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bWindow].sContrastRGBMatrix[0][0] = ( (DWORD)s_AceInfo[bWindow].wRCon * (s_AceInfo[bWindow].wContrast)) >> 10;
    s_AceInfo[bWindow].sContrastRGBMatrix[1][1] = ( (DWORD)s_AceInfo[bWindow].wGCon * (s_AceInfo[bWindow].wContrast)) >> 10;
    s_AceInfo[bWindow].sContrastRGBMatrix[2][2] = ( (DWORD)s_AceInfo[bWindow].wBCon * (s_AceInfo[bWindow].wContrast)) >> 10;
}

static void ArrayMultiply(short sFirst[3][3], short sSecond[3][3], short sResult[3][3])
{
    BYTE ucRow, ucCol;

    //go line by line
    for(ucRow=0; ucRow!=3; ucRow++)
    {
        // go column by column
        for(ucCol=0; ucCol!=3; ucCol++)
        {
            sResult[ucRow][ucCol] = (((long)sFirst[ucRow][0] * sSecond[0][ucCol]) +
                                     ((long)sFirst[ucRow][1] * sSecond[1][ucCol]) +
                                     ((long)sFirst[ucRow][2] * sSecond[2][ucCol])) >> 10;
        } // for
    } // for
}

static void msWriteColorMatrix(void *pInstance, BOOL bWindow, short* psMatrix )
{
#if defined(UFO_XC_HDR) && defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2) && !defined(ACE_NOT_IN_KERNEL)
#else
    BYTE i,j;
    BYTE ucAddr;
    short sTmp;
    _ACE_GET_INST_PRIVATE();
    DECLARA_BANK_VARIABLE

    BACKUP_SC_BANK
    if( bWindow == MAIN_WINDOW )
    {
        SET_SC_BANK(BANK_CM_MAIN);
        ucAddr = REG_ADDR_CM_MAIN&0xFF;
    }
    else
    {
        SET_SC_BANK(BANK_CM_SUB);
        ucAddr = REG_ADDR_CM_SUB&0xFF;
    }

    for(i=0; i!=3; i++)
    {
        for(j=0; j!=3; j++)
        {
            sTmp = psMatrix[i*3+j];
            if( sTmp >= 0 )
            {
                if( sTmp > 0xfff )
                {
                    sTmp = 0xfff;
                }
                //SC_WRITE_2BYTE( ucAddr, sTmp );
            }
            else
            {
                sTmp = sTmp * -1;
                if( sTmp > 0xfff )
                {
                    sTmp = 0xfff;
                }
                #ifdef USE_NEW_ACE_MATRIX_RULE
                    sTmp = ~sTmp +1;
                #else
                sTmp |= 0x1000;
                #endif

                //SC_WRITE_2BYTE( ucAddr, sTmp );
            }
            ACE_WRITE_CM(psACEInstPri->u32DeviceID, ucAddr, sTmp );

            ucAddr += 2;
        } // for
    } // for

    RECOVER_SC_BANK
#endif
}

void msSetVideoColorMatrix(void *pInstance, BOOL bScalerWin )
{
    short sResultTmp1[3][3];
    short sResultTmp2[3][3];
    short* psLastResult;
    DECLARA_BANK_VARIABLE
    _ACE_GET_VARIABLE();

    U8 i,j;
    short ColorMatrix_1[3][3];
    short ColorMatrix_2[3][3];

    if( TRUE == bBypassColorMatrix)
    {
        return ;
    }

    BACKUP_SC_BANK


    // Adjust hue&saturation, and then YUV to RGB
    //ArrayMultiply( (Typ3x3Array)(s_AceInfo[bScalerWin].psYVUtoRGBMatrix), s_AceInfo[bScalerWin].sVideoSatHueMatrix, sResultTmp1 );

        for ( i=0;i<3;i++ )
        {
            for ( j=0;j<3;j++ )
            {
               ColorMatrix_1[i][j]=*(s_AceInfo[bScalerWin].psYVUtoRGBMatrix +(i*3)+j);;
               ColorMatrix_2[i][j]= s_AceInfo[bScalerWin].sVideoSatHueMatrix[i][j];
            }
        }
       ArrayMultiply(ColorMatrix_1,ColorMatrix_2, sResultTmp1 );




    // Do color correction
    if(( NULL != s_AceInfo[bScalerWin].psColorCorrectionMatrix )&&(DISABLE==g_ACEinitParameters.bHDREnable))
    {
      // ArrayMultiply( (Typ3x3Array)(s_AceInfo[bScalerWin].psColorCorrectionMatrix), sResultTmp1, sResultTmp2 );
        for ( i=0;i<3;i++ )
        {
            for ( j=0;j<3;j++ )
            {
               ColorMatrix_1[i][j]=*(s_AceInfo[bScalerWin].psColorCorrectionMatrix +(i*3)+j);
            }
         }
         ArrayMultiply( ColorMatrix_1, sResultTmp1, sResultTmp2 );


        // Adjust contrast-RGB
        ArrayMultiply( s_AceInfo[bScalerWin].sContrastRGBMatrix, sResultTmp2, sResultTmp1 );

        psLastResult = (short*)sResultTmp1;
    }
    else // No color correction
    {
        // Adjust contrast-RGB
        ArrayMultiply( s_AceInfo[bScalerWin].sContrastRGBMatrix, sResultTmp1, sResultTmp2 );
        psLastResult = (short*)sResultTmp2;
    }
    if (MDrv_ACE_IsSupportMLoad(pInstance) && (!MDrv_ACE_GetSkipWaitVsync(pInstance, bScalerWin)))
    {
        MDrv_ACE_WriteColorMatrixBurst(pInstance, bScalerWin, (MS_U16*)psLastResult );
    }
    else
    {
        msWriteColorMatrix(pInstance, bScalerWin, psLastResult );
    }

#if defined(UFO_XC_HDR) && defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2) && !defined(ACE_NOT_IN_KERNEL)
#else
    if( bScalerWin == MAIN_WINDOW )
    {
        SET_SC_BANK(BANK_CM_MAIN);
        ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_MAIN, ENABLE_CM_CTL1, MASK_CM_CTL ); // Enable color matrix&Change R/B range
        //ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_MAIN, 0x35, MASK_CM_CTL ); // Enable color matrix&Change R/B range
    }
    else // Sub window
    {
        SET_SC_BANK(BANK_CM_SUB);
        ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_SUB, ENABLE_CM_CTL1, MASK_CM_CTL ); // Enable color matrix&Change R/B range
        //ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_SUB, 0x35, MASK_CM_CTL ); // Enable color matrix&Change R/B range
    }
#endif

    RECOVER_SC_BANK
}

void msAdjustHSC(void *pInstance, BOOL bScalerWin, BYTE ucHue, BYTE ucSaturation, BYTE ucContrast)
{
    _ACE_GET_VARIABLE();

    s_AceInfo[bScalerWin].ucSaturation = ucSaturation;
    s_AceInfo[bScalerWin].wContrast = (WORD)ucContrast<<3;
    s_AceInfo[bScalerWin].ucHue = ucHue;
    SetVideoSatHueMatrix(pInstance, bScalerWin );
    SetVideoContrastMatrix(pInstance, bScalerWin );

    msSetVideoColorMatrix(pInstance, bScalerWin );
}

MS_BOOL msAdjustCFDOSDInfo(void *pInstance, BOOL bScalerWin)
{
    _ACE_GET_VARIABLE();

    if(psACEInstPri->u32DeviceID == 1)
    {
        printf("STB CFD not support SC1 3x3 matrix now!!!!\n");
        return FALSE;
    }

    ST_ACE_CFD_OSD_CONTROL stAceOSDControl;
    memset(&stAceOSDControl, 0, sizeof(ST_ACE_CFD_OSD_CONTROL));
    stAceOSDControl.u32Version = ACE_OSD_CONTROL_VERSION;
    stAceOSDControl.u16Length = sizeof(ST_ACE_CFD_OSD_CONTROL);
    stAceOSDControl.bContrastValid = TRUE;
    stAceOSDControl.u16Contrast = s_AceInfo[bScalerWin].wContrast;
    stAceOSDControl.bHueValid = TRUE;
    stAceOSDControl.u16Hue = s_AceInfo[bScalerWin].ucHue;
    stAceOSDControl.bSaturationValid = TRUE;
    stAceOSDControl.u16Saturation = s_AceInfo[bScalerWin].ucSaturation;
    if(s_AceInfo[bScalerWin].psYVUtoRGBMatrix != NULL)
    {
        stAceOSDControl.bYVUtoRGBMatrixValid = TRUE;
        memcpy(stAceOSDControl.s16YVUtoRGBMatrix, s_AceInfo[bScalerWin].psYVUtoRGBMatrix, sizeof(short)*9);
    }
    stAceOSDControl.bR = TRUE;
    stAceOSDControl.bG = TRUE;
    stAceOSDControl.bB = TRUE;
    stAceOSDControl.u16R = s_AceInfo[bScalerWin].wRCon;
    stAceOSDControl.u16G = s_AceInfo[bScalerWin].wGCon;
    stAceOSDControl.u16B = s_AceInfo[bScalerWin].wBCon;
    stAceOSDControl.u8OSDUIEn = 1;
    stAceOSDControl.u8OSDUIMode = 1;
    stAceOSDControl.u8Win = (bScalerWin==TRUE)?1:0;

    ST_KDRV_XC_CFD_CONTROL_INFO stCFDCtrlInfo;
    memset(&stCFDCtrlInfo, 0, sizeof(ST_KDRV_XC_CFD_CONTROL_INFO));
    stCFDCtrlInfo.enCtrlType = E_KDRV_XC_CFD_CTRL_SET_OSD;
    stCFDCtrlInfo.pParam = &stAceOSDControl;
    stCFDCtrlInfo.u32ParamLen = sizeof(ST_ACE_CFD_OSD_CONTROL);
#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
//_s32FdScaler only defined by UFO_XC_HDR_VERSION == 2
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
    if(0 > _s32FdScaler)
    {
        _s32FdScaler = open("/dev/scaler", O_RDWR);

        if(0 > _s32FdScaler)
        {
            printf("Scaler device not opened!!!!\n");
            return FALSE;
        }
    }

    if (ioctl(_s32FdScaler, MDRV_XC_IOC_CFDCONCTRL, &stCFDCtrlInfo))
    {
        printf("MDRV_XC_IOC_CFDCONCTRL OSD fail!!!!\n");
        return FALSE;
    }

    if (stCFDCtrlInfo.u16ErrCode != 0)
    {
        printf("MDRV_XC_IOC_CFDCONCTRL OSD fail, errCode: %d\n", stCFDCtrlInfo.u16ErrCode);
    }

    ST_ACE_CFD_FIRE_CONTROL stAceCfdFire;
    memset(&stAceCfdFire, 0, sizeof(ST_ACE_CFD_FIRE_CONTROL));
    stAceCfdFire.u32Version = 0;
    stAceCfdFire.u16Length = sizeof(ST_ACE_CFD_FIRE_CONTROL);
    stAceCfdFire.u8UpdateType = E_XC_CFD_UPDATE_TYPE_OSD_ONLY;
    stAceCfdFire.u8Win = bScalerWin;

    stCFDCtrlInfo.enCtrlType = E_KDRV_XC_CFD_CTRL_SET_FIRE;
    stCFDCtrlInfo.pParam = &stAceCfdFire;
    stCFDCtrlInfo.u32ParamLen = sizeof(ST_ACE_CFD_FIRE_CONTROL);
    if (ioctl(_s32FdScaler, MDRV_XC_IOC_CFDCONCTRL, &stCFDCtrlInfo))
    {
        printf("MDRV_XC_IOC_CFDCONCTRL E_KDRV_XC_CFD_CTRL_FIRE fail!!!!\n");
        return FALSE;
    }

    if (stCFDCtrlInfo.u16ErrCode != 0)
    {
        printf("MDRV_XC_IOC_CFDCONCTRL E_KDRV_XC_CFD_CTRL_FIRE fail, errCode: %d\n", stCFDCtrlInfo.u16ErrCode);
    }
#endif
#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_MSTAR_XC_HDR_SUPPORT))
    MDrv_XC_CFDControl(&stCFDCtrlInfo);
    ST_ACE_CFD_FIRE_CONTROL stAceCfdFire;
    memset(&stAceCfdFire, 0, sizeof(ST_ACE_CFD_FIRE_CONTROL));
    stAceCfdFire.u32Version = 0;
    stAceCfdFire.u16Length = sizeof(ST_ACE_CFD_FIRE_CONTROL);
    stAceCfdFire.u8UpdateType = E_XC_CFD_UPDATE_TYPE_OSD_ONLY;
    stAceCfdFire.u8Win = bScalerWin;

    stCFDCtrlInfo.enCtrlType = E_KDRV_XC_CFD_CTRL_SET_FIRE;
    stCFDCtrlInfo.pParam = &stAceCfdFire;
    stCFDCtrlInfo.u32ParamLen = sizeof(ST_ACE_CFD_FIRE_CONTROL);
    MDrv_XC_CFDControl(&stCFDCtrlInfo);
#endif
#endif
#endif

    return TRUE;
}

void msAdjustVideoContrast(void *pInstance, BOOL bScalerWin, BYTE ucContrast )
{
    _ACE_GET_VARIABLE();

    s_AceInfo[bScalerWin].wContrast = (WORD)ucContrast<<3;
    SetVideoContrastMatrix(pInstance, bScalerWin );
    msSetVideoColorMatrix(pInstance, bScalerWin );
}
void msAdjustVideoContrast_10Bits(void *pInstance, BOOL bScalerWin, WORD wContrast )
{
    _ACE_GET_VARIABLE();

    s_AceInfo[bScalerWin].wContrast = wContrast<<1;
    SetVideoContrastMatrix(pInstance, bScalerWin );
    msSetVideoColorMatrix(pInstance, bScalerWin );
}

void msAdjustVideoSaturation(void *pInstance, BOOL bScalerWin, BYTE ucSaturation )
{
    _ACE_GET_VARIABLE();

    s_AceInfo[bScalerWin].ucSaturation = ucSaturation;
    SetVideoSatHueMatrix(pInstance, bScalerWin);
    msSetVideoColorMatrix(pInstance, bScalerWin);
}

void msAdjustVideoHue(void *pInstance, BOOL bScalerWin, BYTE ucHue )
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bScalerWin].ucHue = ucHue;
    SetVideoSatHueMatrix(pInstance, bScalerWin);
    msSetVideoColorMatrix(pInstance, bScalerWin);
}
void msAdjustVideoRGB(void *pInstance, BOOL bScalerWin, BYTE ucRCon, BYTE ucGCon, BYTE ucBCon)
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bScalerWin].wRCon = (WORD)ucRCon<<3;
    s_AceInfo[bScalerWin].wGCon = (WORD)ucGCon<<3;
    s_AceInfo[bScalerWin].wBCon = (WORD)ucBCon<<3;
    SetVideoContrastMatrix(pInstance, bScalerWin);
    msSetVideoColorMatrix(pInstance, bScalerWin);
}
void msAdjustVideoRGB_10Bits(void *pInstance, BOOL bScalerWin, WORD wRCon, WORD wGCon, WORD wBCon)
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bScalerWin].wRCon = wRCon<<1;
    s_AceInfo[bScalerWin].wGCon = wGCon<<1;
    s_AceInfo[bScalerWin].wBCon = wBCon<<1;
    SetVideoContrastMatrix(pInstance, bScalerWin );
    msSetVideoColorMatrix(pInstance, bScalerWin );
}

static void SetPCConRGBMatrix(void *pInstance, BOOL bWindow )
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bWindow].sContrastRGBMatrix[0][0] = ( (DWORD)s_AceInfo[bWindow].wRCon * (s_AceInfo[bWindow].wContrast)) >> 10;
    s_AceInfo[bWindow].sContrastRGBMatrix[1][1] = ( (DWORD)s_AceInfo[bWindow].wGCon * (s_AceInfo[bWindow].wContrast)) >> 10;
    s_AceInfo[bWindow].sContrastRGBMatrix[2][2] = ( (DWORD)s_AceInfo[bWindow].wBCon * (s_AceInfo[bWindow].wContrast)) >> 10;
}

void msSetPCColorMatrix(void *pInstance, BOOL bScalerWin )
{
    _ACE_GET_VARIABLE();
    short sResultTmp1[3][3];
    short sResultTmp2[3][3];
    short* psOutTab;
    //BYTE ucCMCtlVal;
    DECLARA_CMCTL_VARIABLE
    DECLARA_BANK_VARIABLE

    U8 i,j;
    short ColorMatrix_1[3][3];
    short ColorMatrix_2[3][3];

    if( TRUE == bBypassColorMatrix)
    {
        return ;
    }

    BACKUP_SC_BANK


    psOutTab = (short*)s_AceInfo[bScalerWin].sContrastRGBMatrix;

    // YUV => RGB
    if( (s_AceInfo[bScalerWin].bForceYUVtoRGB) || (bScalerWin == SUB_WINDOW) )
    {
        //ArrayMultiply( s_AceInfo[bScalerWin].sContrastRGBMatrix, (Typ3x3Array)(s_AceInfo[bScalerWin].psYVUtoRGBMatrix), sResultTmp1 );
        for ( i=0;i<3;i++ )
        {
            for ( j=0;j<3;j++ )
            {
               ColorMatrix_1[i][j]=*(s_AceInfo[bScalerWin].psYVUtoRGBMatrix +(i*3)+j);;
            }
        }
        ArrayMultiply( s_AceInfo[bScalerWin].sContrastRGBMatrix, ColorMatrix_1, sResultTmp1 );


        psOutTab = (short*)sResultTmp1;
        //ucCMCtlVal = 0x35;
        CTLVal = ENABLE_CM_CTL1;
    }
    else
    {
        CTLVal = ENABLE_CM_CTL2;
        //ucCMCtlVal = 0x30;
    }

    // Convert to sRGB
    if( s_AceInfo[bScalerWin].psPCsRGBMatrix != NULL )
    {
        //ArrayMultiply( (Typ3x3Array)s_AceInfo[bScalerWin].psPCsRGBMatrix, (Typ3x3Array)psOutTab, sResultTmp2 );
        for ( i=0;i<3;i++ )
        {
            for ( j=0;j<3;j++ )
            {
               ColorMatrix_1[i][j]=*(s_AceInfo[bScalerWin].psPCsRGBMatrix +(i*3)+j);;
               ColorMatrix_2[i][j]=*(psOutTab +(i*3)+j);;
            }
        }
        ArrayMultiply( ColorMatrix_1, ColorMatrix_2, sResultTmp2 );


        psOutTab = (short*)sResultTmp2;
    }

    if (MDrv_ACE_IsSupportMLoad(pInstance))
    {
        MDrv_ACE_WriteColorMatrixBurst(pInstance, bScalerWin, (MS_U16*)psOutTab );
    }
    else
    {
        msWriteColorMatrix(pInstance, bScalerWin, psOutTab );
    }

#if defined(UFO_XC_HDR) && defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2) && !defined(ACE_NOT_IN_KERNEL)
#else
    if( bScalerWin == MAIN_WINDOW )
    {
        SET_SC_BANK(BANK_CM_MAIN);
        ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_MAIN, CTLVal, MASK_CM_CTL );
    }
    else
    {
        SET_SC_BANK(BANK_CM_SUB);
        ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_SUB, CTLVal, MASK_CM_CTL );
    }
#endif

    RECOVER_SC_BANK
}

void msAdjustPCContrast(void *pInstance, BOOL bScalerWin, BYTE ucContrast )
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bScalerWin].wContrast = (WORD)ucContrast<<3;
    SetPCConRGBMatrix(pInstance, bScalerWin );
    msSetPCColorMatrix(pInstance, bScalerWin );
}
void msAdjustPCContrast_10Bits(void *pInstance, BOOL bScalerWin, WORD wContrast )
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bScalerWin].wContrast = wContrast<<1;
    SetPCConRGBMatrix(pInstance, bScalerWin );
    msSetPCColorMatrix(pInstance, bScalerWin );
}

void msAdjustPCRGB(void *pInstance, BOOL bScalerWin, BYTE ucRed, BYTE ucGreen, BYTE ucBlue )
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bScalerWin].wRCon = (WORD)ucRed<<3;
    s_AceInfo[bScalerWin].wGCon = (WORD)ucGreen<<3;
    s_AceInfo[bScalerWin].wBCon = (WORD)ucBlue<<3;
    SetPCConRGBMatrix(pInstance,  bScalerWin );
    msSetPCColorMatrix(pInstance,  bScalerWin );
}
void msAdjustPCRGB_10Bits(void *pInstance, BOOL bScalerWin, WORD wRed, WORD wGreen, WORD wBlue )
{
    _ACE_GET_VARIABLE();
    s_AceInfo[bScalerWin].wRCon = wRed<<1;
    s_AceInfo[bScalerWin].wGCon = wGreen<<1;
    s_AceInfo[bScalerWin].wBCon = wBlue<<1;
    SetPCConRGBMatrix(pInstance, bScalerWin );
    msSetPCColorMatrix(pInstance, bScalerWin );
}

//-------------------------------------------------------------------------------
// Directly control color matrix on/off
//-------------------------------------------------------------------------------
void msACE_SetColorMatrixControl(void *pInstance, BOOL bScalerWin, BOOL bEnable )
{
    DECLARA_CMCTL_VARIABLE
    DECLARA_BANK_VARIABLE

    BACKUP_SC_BANK
    _ACE_GET_INST_PRIVATE();

    if( bEnable )
        CTLVal = MASK_CM_CTL_EN;
    else
        CTLVal = 0;

    if( bScalerWin == MAIN_WINDOW )
    {
        SET_SC_BANK(BANK_CM_MAIN);
        ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_MAIN, CTLVal, MASK_CM_CTL_EN );
    }
    else
    {
        SET_SC_BANK(BANK_CM_SUB);
        ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_SUB, CTLVal, MASK_CM_CTL_EN );
    }
    RECOVER_SC_BANK
}

//-------------------------------------------------------------------------------
// Directly control color matrix R/B range
// bRange: 0 -> 0~255, 1 -> -128~127
//-------------------------------------------------------------------------------
void msACE_SetRBChannelRange(void *pInstance, BOOL bScalerWin, BOOL bRange )
{
    DECLARA_CMCTL_VARIABLE

    DECLARA_BANK_VARIABLE

    BACKUP_SC_BANK

    if( bRange )
        CTLVal = MASK_CM_CTL_RB_RANGE;
    else
        CTLVal = 0;
    _ACE_GET_INST_PRIVATE();

    if( bScalerWin == MAIN_WINDOW )
    {
        SET_SC_BANK(BANK_CM_MAIN);
        ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_MAIN, CTLVal, MASK_CM_CTL_RB_RANGE );
    }
    else
    {
        SET_SC_BANK(BANK_CM_SUB);
        ACE_WRITE_CM_CTL(psACEInstPri->u32DeviceID,  REG_ADDR_CM_CTL_SUB, CTLVal, MASK_CM_CTL_RB_RANGE );
    }
    RECOVER_SC_BANK
}

MS_BOOL msACESetBypassColorMatrix(void *pInstance, MS_BOOL bEnable)
{
    _ACE_GET_VARIABLE();
    bBypassColorMatrix = bEnable;
    return TRUE ;
}

void msACE_HDR_MatrixInvGen(void *pInstance,MS_S32 Minv[3][3], MS_S32 M[3][3])
{
    MS_S32 a = M[0][0];
    MS_S32 b = M[0][1];
    MS_S32 c = M[0][2];
    MS_S32 d = M[1][0];
    MS_S32 e = M[1][1];
    MS_S32 f = M[1][2];
    MS_S32 g = M[2][0];
    MS_S32 h = M[2][1];
    MS_S32 i = M[2][2];

    MS_S64 ei_fh = (MS_S64)e*i - (MS_S64)f*h; // a b c a
    MS_S64 fg_di = (MS_S64)f*g - (MS_S64)d*i; // d e f d
    MS_S64 dh_eg = (MS_S64)d*h - (MS_S64)e*g; // g h i g
    MS_S64 hc_ib = (MS_S64)h*c - (MS_S64)i*b; // a b c a
    MS_S64 ia_gc = (MS_S64)i*a - (MS_S64)g*c;
    MS_S64 gb_ha = (MS_S64)g*b - (MS_S64)h*a;
    MS_S64 bf_ce = (MS_S64)b*f - (MS_S64)c*e;
    MS_S64 cd_af = (MS_S64)c*d - (MS_S64)a*f;
    MS_S64 ae_bd = (MS_S64)a*e - (MS_S64)b*d;

    MS_S64 det = a*ei_fh + b*fg_di + c*dh_eg;

    Minv[0][0] = ei_fh * 2500000000LL / det;
    Minv[1][0] = fg_di * 2500000000LL / det;
    Minv[2][0] = dh_eg * 2500000000LL / det;
    Minv[0][1] = hc_ib * 2500000000LL / det;
    Minv[1][1] = ia_gc * 2500000000LL / det;
    Minv[2][1] = gb_ha * 2500000000LL / det;
    Minv[0][2] = bf_ce * 2500000000LL / det;
    Minv[1][2] = cd_af * 2500000000LL / det;
    Minv[2][2] = ae_bd * 2500000000LL / det;

}

void msACE_HDR_YUV2RGBGen(void *pInstance,ACE_DRV_HDR_YUV2RGBData* pData)
{
    MS_S32 Mr2y[3][3], My2r[3][3];
    MS_S64 Kr, Kb, Kg;
    MS_U16 u16Regs[9];

    memset(Mr2y, 0, sizeof(MS_S32)*3*3);
    memset(My2r, 0, sizeof(MS_S32)*3*3);
    memset(u16Regs, 0, sizeof(MS_U16)*9);

    if (pData->u8SMode == 0)
    {
        Kr = pData->u16SKr;
        Kb = pData->u16SKb;
        Kg = 50000 - Kb - Kr;
        Mr2y[0][0] = Kr;
        Mr2y[0][1] = Kg;
        Mr2y[0][2] = Kb;

        Mr2y[1][0] = -Kr * 25000 / (50000 - Kb);
        Mr2y[1][1] = -Kg * 25000 / (50000 - Kb);

        Mr2y[1][2] = (50000 - Kb) * 25000 / (50000 - Kb);
        Mr2y[2][0] = (50000 - Kr) * 25000 / (50000 - Kr);

        Mr2y[2][1] = -Kg * 25000 / (50000 - Kr);
        Mr2y[2][2] = -Kb * 25000 / (50000 - Kr);

        msACE_HDR_MatrixInvGen(pInstance,My2r, Mr2y);

    }
    else if (pData->u8SMode == 1) // YCgCo
    {
        // TODO
    }
    else// if (pData->sMode == 2) // 2020CL
    {
        // TODO
    }

    if ((g_ACEinitParameters.ACE_HDRMetadataMpegVUI.u8TransferCharacteristics == 16) ||
       (g_ACEinitParameters.ACE_HDRMetadataHdmiTxInfoFrame.u8EOTF == 2))    // HDMI HDR is SMPTE ST2084.
    {
        // BT.2084.
        _u16HdrY2RRatio = 2;
    }
    else
    {
        // Others.
        _u16HdrY2RRatio = 1;
    }

    u16Regs[1] = (((My2r[0][0] << 10)*_u16HdrY2RRatio + 25000) / 50000*1167/1000) & 0x1FFF;
    u16Regs[2] = (((My2r[0][1] << 10)*_u16HdrY2RRatio + 25000) / 50000) & 0x1FFF;
    u16Regs[0] = (((My2r[0][2] << 10)*_u16HdrY2RRatio + 25000) / 50000) & 0x1FFF;
    u16Regs[4] = (((My2r[1][0] << 10)*_u16HdrY2RRatio + 25000) / 50000*1167/1000) & 0x1FFF;
    u16Regs[5] = (((My2r[1][1] << 10)*_u16HdrY2RRatio + 25000) / 50000) & 0x1FFF;
    u16Regs[3] = (((My2r[1][2] << 10)*_u16HdrY2RRatio + 25000) / 50000) & 0x1FFF;
    u16Regs[7] = (((My2r[2][0] << 10)*_u16HdrY2RRatio + 25000) / 50000*1167/1000) & 0x1FFF;
    u16Regs[8] = (((My2r[2][1] << 10)*_u16HdrY2RRatio + 25000) / 50000) & 0x1FFF;
    u16Regs[6] = (((My2r[2][2] << 10)*_u16HdrY2RRatio + 25000) / 50000) & 0x1FFF;

    BYTE i;
    BYTE ucCopyLen = 9;

    for( i = 0; i < ucCopyLen; ++ i )
    {
        if(u16Regs[i] >0xFFF)
        {
            u16Regs[i]= u16Regs[i]|0xF000;
        }
    }

    _ACE_GET_VARIABLE();

    CopyMatrix( u16Regs,(WORD*)s_AceInfo[MAIN_WINDOW].psYVUtoRGBMatrix  ,ucCopyLen );

    msSetVideoColorMatrix(pInstance, MAIN_WINDOW );
}

MS_BOOL msACE_HDR_Set3x3Gen(void *pInstance,MS_BOOL bWrite)
{
    _ACE_GET_VARIABLE();
    BYTE ucCopyLen = 9;
    MS_BOOL bRet = FALSE;

    if(TRUE==bWrite)
    {
        if(_bSetGenRead==TRUE)
        {
            CopyMatrix( s16PreRegs,(WORD*)s_AceInfo[MAIN_WINDOW].psYVUtoRGBMatrix  ,ucCopyLen );
            _bSetGenRead=FALSE;
            bRet = TRUE;
        }
    }
    else
    {

        if(_bSetGenRead==FALSE)
        {
            if(FALSE==_bSetHDRInit)
            {
                _u16HdrY2RRatio = 2;

                _bSetHDRInit=TRUE;
            }
            CopyMatrix( (WORD*)s_AceInfo[MAIN_WINDOW].psYVUtoRGBMatrix,s16PreRegs  ,ucCopyLen );

            _bSetGenRead=TRUE;

            bRet = TRUE;
        }
    }

    return bRet;
}
void msACE_HDR_ConfigMetadata_HdmiTxInfoFrame(void *pInstance,ACE_DRV_HDR_YUV2RGBData* pY2RData, ACE_DRV_HDRHdmiTxAviInfoFrame *stMetadata)
{
    // Matrix Coefficients (Note: Don't care about whether the pixel format is RGB or YCC. Just assume input to VOP2 is YCC!! Important!!!!!!!!!)
    pY2RData->bSValid = TRUE;

    if (stMetadata->Colorimetry == 0 || stMetadata->Colorimetry == 2) // No data or 709
    {
        // 709 matrices
        pY2RData->u8SMode = 0;
        pY2RData->u16SKr = 10630; //0.2126
        pY2RData->u16SKb = 3610; //0.0722
    }
    else if (stMetadata->Colorimetry == 1) // 170M (=601)
    {
        // 601 matrices
        pY2RData->u8SMode = 0;
        pY2RData->u16SKr = 14950; //0.299
        pY2RData->u16SKb = 5700; //0.114
    }
    else if (stMetadata->Colorimetry == 3 && (stMetadata->ExtendedColorimetry == 1)) // xvYCC 709
    {
        // 709 matrices
        pY2RData->u8SMode = 0;
        pY2RData->u16SKr = 10630; //0.2126
        pY2RData->u16SKb = 3610; //0.0722
    }
    else if (stMetadata->Colorimetry == 3 && (stMetadata->ExtendedColorimetry == 0 || stMetadata->ExtendedColorimetry == 2 || stMetadata->ExtendedColorimetry == 3)) // xvYCC 601 or sYCC 601 or Adobe YCC 601
    {
        // 601 matrices
        pY2RData->u8SMode = 0;
        pY2RData->u16SKr = 14950; //0.299
        pY2RData->u16SKb = 5700; //0.114
    }
    else if (stMetadata->Colorimetry == 3 && (stMetadata->ExtendedColorimetry == 4)) // Adobe RGB
    {
        // 601 matrices (NOTE : Assume the input of VOP2 is YCC)
        pY2RData->u8SMode = 0;
        pY2RData->u16SKr = 14950; //0.299
        pY2RData->u16SKb = 5700; //0.114
    }
    else if (stMetadata->Colorimetry == 3 && (stMetadata->ExtendedColorimetry == 5 || stMetadata->ExtendedColorimetry == 6)) // BT2020 CL YCC
    {
        // 2020 matrices
        pY2RData->u8SMode = 0;
        pY2RData->u16SKr = 13135; //0.2627
        pY2RData->u16SKb = 2965; //0.0593
    }
    else
    {
        pY2RData->bSValid = FALSE;
    }
}

void msACE_HDR_ConfigMetadata_MpegVUI(void *pInstance,ACE_DRV_HDR_YUV2RGBData* pY2RData)
{
    // Matrix Coefficients
    pY2RData->bSValid = TRUE;
    switch (g_ACEinitParameters.ACE_HDRMetadataMpegVUI.u8MatrixCoefficients)
    {
     case 1: // 709
        pY2RData->u8SMode = 0;
        pY2RData->u16SKr = 10630; //0.2126
        pY2RData->u16SKb = 3610; //0.0722
        break;
     case 4: // 47
        pY2RData->u8SMode = 0;
        pY2RData->u16SKr = 15000; //0.30
        pY2RData->u16SKb = 5500; //0.11
        break;
     case 5: // 601 (625)
     case 6: // 601 (525)
        pY2RData->u8SMode = 0;
        pY2RData->u16SKr = 14950; //0.299
        pY2RData->u16SKb = 5700; //0.114
        break;
     case 7: // 240M
        pY2RData->u8SMode = 0;
        pY2RData->u16SKr = 10600; //0.212
        pY2RData->u16SKb = 4350; //0.087
        break;
     case 8: // YCgCo
        pY2RData->u8SMode = 1;
        pY2RData->u16SKr = 2500;
        pY2RData->u16SKb = 1250;
        break;
     case 9: // 2020NCL
        pY2RData->u8SMode = 0;
        pY2RData->u16SKr = 13135; //0.2627
        pY2RData->u16SKb = 2965; //0.0593
        break;
     case 10: // 2020CL
        pY2RData->u8SMode = 0;  // Spec is 2. Reference mantis 0959025.
        pY2RData->u16SKr = 13135; //0.2627
        pY2RData->u16SKb = 2965; //0.0593
        break;
     case 2: // Unspecified
     default:
        pY2RData->bSValid = FALSE;
        break;
    }
}

void msACE_HDR_Main(void *pInstance)
{
    ACE_DRV_HDR_YUV2RGBData stY2RData = {0, 0, 0, 3610, 10630, 0, 3610, 10630};


    if(ENABLE==g_ACEinitParameters.bHDREnable)
    {
        // Read 3*3
        msACE_HDR_Set3x3Gen(pInstance,FALSE);
        if(g_ACEinitParameters.u16HDRFunctionSelect & 0x01)
        {
            msACE_HDR_ConfigMetadata_MpegVUI(pInstance,&stY2RData);

            if(g_ACEinitParameters.ACE_HDRMetadataMpegVUI.u8MatrixCoefficients !=_u8PreMatrixCoefficients)
            {
                _u8PreMatrixCoefficients = g_ACEinitParameters.ACE_HDRMetadataMpegVUI.u8MatrixCoefficients;

                msACE_HDR_YUV2RGBGen(pInstance,&stY2RData);
            }
        }
        // For HDMI HDR
        if(g_ACEinitParameters.u16HDRFunctionSelect==0x10)
        {

            ACE_DRV_HDRHdmiTxAviInfoFrame stAviInfoFrame;
            memcpy(&stAviInfoFrame,&g_ACEinitParameters.ACE_HDRHdmiTxAviInfoFrame,sizeof(ACE_DRV_HDRHdmiTxAviInfoFrame));

            msACE_HDR_ConfigMetadata_HdmiTxInfoFrame(pInstance,&stY2RData,&stAviInfoFrame);

            if((stAviInfoFrame.ExtendedColorimetry !=_u8PreExtendedColorimetry) || (stAviInfoFrame.Colorimetry != _u8PreColorimetry))
            {
                printf("PixelFormat: %x\n",stAviInfoFrame.PixelFormat);
                printf("Colorimetry: %x\n",stAviInfoFrame.Colorimetry);
                printf("ExtendedColorimetry: %x\n",stAviInfoFrame.ExtendedColorimetry);
                printf("RgbQuantizationRange: %x\n",stAviInfoFrame.RgbQuantizationRange);
                printf("YccQuantizationRange: %x\n",stAviInfoFrame.YccQuantizationRange);
                printf("StaticMetadataDescriptorID: %x\n",stAviInfoFrame.StaticMetadataDescriptorID);
                printf("_u8PreColorimetry: %x\n",_u8PreColorimetry);
                printf("_u8PreExtendedColorimetry: %x\n",_u8PreExtendedColorimetry);
                _u8PreColorimetry = stAviInfoFrame.Colorimetry;
                _u8PreExtendedColorimetry = stAviInfoFrame.ExtendedColorimetry;
                msACE_HDR_YUV2RGBGen(pInstance,&stY2RData);
            }
        }
    }
    else
    {
        // Write 3*3
        if (msACE_HDR_Set3x3Gen(pInstance,TRUE) == TRUE)
        {
            msSetVideoColorMatrix(pInstance, MAIN_WINDOW );
        }
        _u8PreMatrixCoefficients=0;
         _u8PreExtendedColorimetry = 0;
         _u8PreColorimetry = 0;
    }

}


