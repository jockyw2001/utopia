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
// comply with all applicable laws and regulations
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
//    or otherwise
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

#ifndef _API_ACE_PRIV_H_
#define _API_ACE_PRIV_H_


////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct __attribute__((packed))
{
    E_XC_ACE_MWE_FUNC  _MWEFunc;
    MS_U32       _MWETimer;
    MS_U32       _MWEFlag;
    MWE_RECT gMWERect;

    XC_ACE_InitData g_XC_ACE_InitData;
    MS_BOOL _bMWESkipDriverSetting;
    E_XC_ACE_MWE_MIRROR_TYPE _enMirrorMode;

    MS_U8  g_u8MotionStableCnt;
    MS_U8  g_u8LumaStableCnt;
    MS_U8  g_u8FlickStableCnt;
    MS_BOOL  g_bACEModuleInited[2];
} ST_APIACE;

typedef struct __attribute__((packed))
{
    MS_U16 u16RedGain;
    MS_U16 u16GreenGain;
    MS_U16 u16BlueGain;
} ACE_PostRGBGainSetting;

typedef struct __attribute__((packed))
{
    MS_U16 wContrast;
    MS_U16 wRCon;
    MS_U16 wGCon;
    MS_U16 wBCon;
    MS_U8 ucSaturation;
    MS_U8 ucHue;
    short sContrastRGBMatrix[3][3];
    short sVideoSatHueMatrix[3][3];
    short psColorCorrectionMatrix[32]; // this variable must pointer to a array[3][3]
#if !defined (__aarch64__)
   // MS_U32 u32AlignmentDummy0;
#endif
    short* psPCsRGBMatrix; // sRGB matrix for PC, this variable must pointer to a array[3][3]
#if !defined (__aarch64__)
    MS_U32 u32AlignmentDummy1;
#endif
    short psYVUtoRGBMatrix[9]; // this variable must pointer to a array[3][3]
#if !defined (__aarch64__)
    //MS_U32 u32AlignmentDummy2;
#endif
    MS_U8 bForceYUVtoRGB : 1;
    MS_U8 ucYUVtoRGBMatrixSel : 2;
}StruAceInfo;

typedef struct __attribute__((packed))
{
    ACE_PostRGBGainSetting stACEPostGainSetting[2];
    StruAceInfo s_AceInfo[2];
    PanelSizeType MWEPanel;
    MS_BOOL bBypassColorMatrix;
} ST_DRVACE;

typedef struct __attribute__((packed))
{
    MS_BOOL  bMainWinSkipWaitVsyn;
    MS_BOOL  bSubWinSkipWaitVsyn;
} ST_HALACE;

typedef struct __attribute__((packed))
{
    MS_BOOL bResourceRegistered;
    ST_APIACE stapiACE;
    ST_DRVACE stdrvACE;
    ST_HALACE sthalACE;
}ACE_RESOURCE_PRIVATE;

typedef struct
{
    MS_U32      ACE_Reg[2];
    MS_BOOL     bSetPCYUV2RGB_Enable;
    XC_ACE_color_temp_ex2 stColorTemp;

    MS_BOOL     bUseYUVSpace;
    MS_U8       u8Contrast;

    MS_U8       u8Brightness_R;
    MS_U8       u8Brightness_G;
    MS_U8       u8Brightness_B;

    MS_U8       u8Hue;
    MS_U8       u8Saturation;
    MS_U8       u8Sharpness;
}ACE_REGS_SAVE_AREA;
#define _ACE_SEMAPHORE_ENTRY(pInst)                                                              \
        if(MDrv_ACE_Get_Semaphore(pInst) != UTOPIA_STATUS_SUCCESS)                               \
        {      }

#define _ACE_GET_INST_PRIVATE()  \
    ACE_INSTANCE_PRIVATE *psACEInstPri = NULL; \
    UtopiaInstanceGetPrivate(pInstance, (void**)&psACEInstPri);


#define _ACE_GET_VARIABLE()  \
    ACE_INSTANCE_PRIVATE *psACEInstPri = NULL; \
    UtopiaInstanceGetPrivate(pInstance, (void**)&psACEInstPri);    \
    ACE_RESOURCE_PRIVATE *psACEResPri = NULL; \
    UtopiaResourceGetPrivate(pu32ACERes[psACEInstPri->u32DeviceID],(void**)(&psACEResPri));

#define _ACE_SEMAPHORE_RETURN(pInst)                                                             \
            MDrv_ACE_Release_Semaphore(pInst);


typedef MS_BOOL                             (*IOCTL_ACE_INIT)                                   (void *pInstance, XC_ACE_InitData *pstXC_ACE_InitData, MS_U32 u32InitDataLen);
typedef MS_BOOL                             (*IOCTL_ACE_EXIT)                                   (void *pInstance );
typedef void                                (*IOCTL_ACE_DMS)                                    (void *pInstance, MS_BOOL eWindow, MS_BOOL bisATV);
typedef void                                (*IOCTL_ACE_SETPCYUV2RGB)                           (void *pInstance, MS_BOOL eWindow, MS_BOOL bEnable);
typedef void                                (*IOCTL_ACE_SELECTYUVTORGBMATRIX)                   (void *pInstance, MS_BOOL eWindow, MS_U8 ucMatrix, MS_S16* psUserYUVtoRGBMatrix);
typedef void                                (*IOCTL_ACE_SETCOLORCORRECTIONTABLE)                (void *pInstance, MS_BOOL bScalerWin);
typedef void                                (*IOCTL_ACE_SETPCSRGBTABLE)                         (void *pInstance, MS_BOOL bScalerWin);
typedef void                                (*IOCTL_ACE_GETCOLORMATRIX)                         (void *pInstance, MS_BOOL eWindow, MS_U16* pu16Matrix);
typedef void                                (*IOCTL_ACE_SETCOLORMATRIX)                         (void *pInstance, MS_BOOL eWindow, MS_U16* pu16Matrix);
typedef E_XC_ACE_RESULT                     (*IOCTL_ACE_SETBYPASSCOLORMATRIX)                   (void *pInstance, MS_BOOL bEnable );
typedef MS_BOOL                             (*IOCTL_ACE_SETIHC)                                 (void *pInstance, MS_BOOL bScalerWin, XC_ACE_IHC_COLOR_TYPE eIHC, MS_U8 u8Val);
typedef MS_BOOL                             (*IOCTL_ACE_SETICC)                                 (void *pInstance, MS_BOOL bScalerWin, XC_ACE_ICC_COLOR_TYPE eICC, MS_U8 u8Val);
typedef MS_BOOL                             (*IOCTL_ACE_SETIBC)                                 (void *pInstance, MS_BOOL bScalerWin, XC_ACE_IBC_COLOR_TYPE eIBC, MS_U8 u8Val);
typedef void                                (*IOCTL_ACE_PATCHDTGCOLORCHECKER)                   (void *pInstance, MS_U8 u8Mode);
typedef MS_U16                              (*IOCTL_ACE_GETACEINFO)                             (void *pInstance, MS_BOOL bWindow, E_XC_ACE_INFOTYPE eXCACEInfoType );
typedef void                                (*IOCTL_ACE_GETACEINFOEX)                           (void *pInstance, MS_BOOL bWindow, XC_ACE_InfoEx *pInfoEx);
typedef void                                (*IOCTL_ACE_PICSETCONTRAST)                         (void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, MS_U8 u8Contrast);
typedef void                                (*IOCTL_ACE_PICSETBRIGHTNESS)                       (void *pInstance, MS_BOOL eWindow, MS_U8 u8Brightness_R, MS_U8 u8Brightness_G, MS_U8 u8Brightness_B);
typedef void                                (*IOCTL_ACE_PICSETBRIGHTNESSPRECISE)                (void *pInstance, MS_BOOL eWindow, MS_U16 u16Brightness_R, MS_U16 u16Brightness_G, MS_U16 u16Brightness_B);
typedef MS_BOOL                             (*IOCTL_ACE_PICSET_PRE_Y_OFFSET)                    (void *pInstance, MS_BOOL eWindow, MS_U8 u8PreYOffset);
typedef MS_U8                               (*IOCTL_ACE_PICGET_PRE_Y_OFFSET)                    (void *pInstance, MS_BOOL eWindow);
typedef void                                (*IOCTL_ACE_PICSETHUE)                              (void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, MS_U8 u8Hue );
typedef E_XC_ACE_RESULT                     (*IOCTL_ACE_SKIPWAITVSYNC)                          (void *pInstance, MS_BOOL eWindow,MS_BOOL bIsSkipWaitVsyn);
typedef void                                (*IOCTL_ACE_PICSETSATURATION)                       (void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, MS_U8 u8Saturation );
typedef void                                (*IOCTL_ACE_PICSETSHARPNESS)                        (void *pInstance, MS_BOOL eWindow, MS_U8 u8Sharpness );
typedef void                                (*IOCTL_ACE_PICSETCOLORTEMP)                        (void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_color_temp* pstColorTemp);
typedef void                                (*IOCTL_ACE_PICSETCOLORTEMPEX)                      (void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_color_temp_ex* pstColorTemp);
typedef void                                (*IOCTL_ACE_PICSETCOLORTEMP_10BITS)                      (void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_color_temp_ex* pstColorTemp);
typedef void                                (*IOCTL_ACE_PICSETPOSTCOLORTEMP_V02)                (void *pInstance, MS_BOOL eWindow, XC_ACE_color_temp_ex2* pstColorTemp);
typedef void                                (*IOCTL_ACE_SETFLESHTONE)                           (void *pInstance, MS_BOOL eWindow, MS_BOOL bEn, MS_U8 u8FleshTone);
typedef void                                (*IOCTL_ACE_SETBLACKADJUST)                         (void *pInstance, MS_BOOL eWindow, MS_U8 u8BlackAdjust);
typedef MS_BOOL                             (*IOCTL_ACE_SET_IHC_SRAM)                           (void *pInstance, MS_U16 *pBuf, MS_U16 u16ByteSize);
typedef MS_BOOL                             (*IOCTL_ACE_SET_ICC_SRAM)                           (void *pInstance, MS_U16 *pBuf, MS_U16 u16ByteSize);
typedef void                                (*IOCTL_ACE_ENABLEMWE)                              (void *pInstance, MS_BOOL bEnable);
typedef void                                (*IOCTL_ACE_MWECLONEVISUALEFFECT)                   (void *pInstance);
typedef MS_BOOL                             (*IOCTL_ACE_MWESETREGTABLE)                         (void *pInstance, XC_ACE_TAB_Info *pMWETable);
typedef MS_BOOL                             (*IOCTL_ACE_SETMWELOADFROMPQ)                       (void *pInstance, MS_BOOL bEnable);
typedef MS_BOOL                             (*IOCTL_ACE_MWESTATUS)                              (void *pInstance);
typedef void                                (*IOCTL_ACE_MWEHANDLE)                              (void *pInstance);
typedef void                                (*IOCTL_ACE_MWEFUNCSEL)                             (void *pInstance, MS_BOOL eWindow, E_XC_ACE_MWE_FUNC mwe_func);
typedef void                                (*IOCTL_ACE_MWESETDISPWIN)                          (void *pInstance, MS_U16 u16MWE_Disp_Hstart, MS_U16 u16MWE_Disp_Vstart, MS_U16 u16MWE_Disp_Width, MS_U16 u16MWE_Disp_Height);
typedef void                                (*IOCTL_ACE_3DCLONEPQMAP)                           (void *pInstance, XC_ACE_WEAVETYPE enWeaveType);
typedef void                                (*IOCTL_ACE_DNR_BLENDING_NRTBL)                     (void *pInstance, MS_U8 *pu8NewNR, MS_U8 u8Weight, MS_U8 u8Step, MS_U16 *pu16PreY0, MS_U16 *pu16PreY1, MS_U8 *pu8NRTBL);
typedef MS_U16                              (*IOCTL_ACE_DNR_BLENDING_MISC)                      (void *pInstance, MS_U8 u8NewItem, MS_U16 u16PreItem, MS_U8 u8Weight, MS_U8 u8Step);
typedef MS_U8                               (*IOCTL_ACE_DNR_BLENDING)                           (void *pInstance, MS_U8 u8NewItem, MS_U16 u16PreItem, MS_U8 u8Weight);
typedef MS_U16                              (*IOCTL_ACE_DNR_GET_PRECISIONSHIFT)                 (void *pInstance);
typedef MS_U8                               (*IOCTL_ACE_DNR_GETMOTION_WEIGHT)                   (void *pInstance, MS_U8 u8CurMotion, MS_U8 u8PreMotion, MS_U8 u8CurMotionLvl, MS_U8 u8PreMotionLvl, MS_U8 u8DeFlick_Thre, MS_U8 u8DeFilick_Step);
typedef MS_U8                               (*IOCTL_ACE_DNR_GETLUMA_WEIGHT)                     (void *pInstance, MS_U8 u8CurAvgLuma, MS_U8 u8PreAvgLuam, MS_U8 u8CurLumaLvl, MS_U8 u8PreLumaLvl, MS_U8 u8DeFlick_Th, MS_U8 u8DeFlick_Step);
typedef MS_U8                               (*IOCTL_ACE_DNR_GETNOISE_WEIGHT)                    (void *pInstance, MS_U8 u8CurNoise, MS_U8 u8PreNoise, MS_U8 u8Range, MS_U8 u8DeFlick_Th, MS_U8 u8DeFlick_Step);
typedef MS_U8                               (*IOCTL_ACE_DNR_GETMOTION)                          (void *pInstance);
typedef void                                (*IOCTL_ACE_DNR_INIT_MOTION)                        (void *pInstance);
typedef void                                (*IOCTL_ACE_DNR_INIT_LUMA)                          (void *pInstance);
typedef void                                (*IOCTL_ACE_DNR_INIT_NOISE)                         (void *pInstance);
typedef void                                (*IOCTL_ACE_DNR_SETPARAM)                           (void *pInstance, MS_BOOL eWindow, XC_ACE_DNR_Param eParam, MS_U16 u16val);
typedef MS_U8                               (*IOCTL_ACE_DNR_GETPARAM)                           (void *pInstance, MS_BOOL eWindow, XC_ACE_DNR_Param eParam);
typedef void                                (*IOCTL_ACE_DNR_SETNRTBL)                           (void *pInstance, MS_U8 *pu8Tbl);
typedef E_XC_ACE_RESULT                     (*IOCTL_ACE_GETLIBVER)                              (void *pInstance, const MSIF_Version **ppVersion);
typedef const XC_ACE_ApiInfo *              (*IOCTL_ACE_GETINFO)                                (void *pInstance);
typedef MS_BOOL                             (*IOCTL_ACE_GETSTATUS)                              (void *pInstance, XC_ACE_ApiStatus *pDrvStatus, MS_BOOL bWindow);
typedef MS_BOOL                             (*IOCTL_ACE_SETDBGLEVEL)                            (void *pInstance, MS_U16 u16DbgSwitch);
typedef void                                (*IOCTL_ACE_COLORCORRECTIONTABLE)                   (void *pInstance, MS_BOOL bScalerWin, MS_S16 *psColorCorrectionTable );
typedef void                                (*IOCTL_ACE_SETCOLORMATRIXCONTROL)                  (void *pInstance, MS_BOOL bScalerWin, MS_BOOL bEnable );
typedef void                                (*IOCTL_ACE_SETRBCHANNELRANGE)                      (void *pInstance, MS_BOOL bScalerWin, MS_BOOL bRange );
typedef void                                (*IOCTL_ACE_PICSETPOSTCOLORTEMP)                    (void *pInstance, MS_BOOL eWindow, XC_ACE_color_temp* pstColorTemp);
typedef void                                (*IOCTL_ACE_PICSETPOSTCOLORTEMPEX)                  (void *pInstance, MS_BOOL eWindow, XC_ACE_color_temp_ex* pstColorTemp);
typedef void                                (*IOCTL_ACE_PICSETPOSTCOLORTEMP2)                   (void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_color_temp* pstColorTemp);
typedef void                                (*IOCTL_ACE_PICSETPOSTCOLORTEMP2EX)                 (void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_color_temp_ex* pstColorTemp);
typedef MS_U32                              (*IOCTL_ACE_SET_POWERSTATE)                         (void*, EN_POWER_MODE);
typedef MS_BOOL                             (*IOCTL_ACE_SETHDRINIT)                             (void*, XC_ACE_HDRinit *pstACE_HDRInitData);

// OBSOLETE FUNCTION POINTER END

typedef struct __attribute__((packed))
{
    MS_U32 u32DeviceID;
    IOCTL_ACE_INIT                                  fpACE_Init;
    IOCTL_ACE_EXIT                                  fpACE_Exit;
    IOCTL_ACE_DMS                                   fpACE_DMS;
    IOCTL_ACE_SETPCYUV2RGB                          fpACE_SetPCYUV2RGB;
    IOCTL_ACE_SELECTYUVTORGBMATRIX                  fpACE_SelectYUVtoRGBMatrix;
    IOCTL_ACE_SETCOLORCORRECTIONTABLE               fpACE_SetColorCorrectionTable;
    IOCTL_ACE_SETPCSRGBTABLE                        fpACE_SetPCsRGBTable;
    IOCTL_ACE_GETCOLORMATRIX                        fpACE_GetColorMatrix;
    IOCTL_ACE_SETCOLORMATRIX                        fpACE_SetColorMatrix;
    IOCTL_ACE_SETBYPASSCOLORMATRIX                  fpACE_SetBypassColorMatrix;
    IOCTL_ACE_SETIHC                                fpACE_SetIHC;
    IOCTL_ACE_SETICC                                fpACE_SetICC;
    IOCTL_ACE_SETIBC                                fpACE_SetIBC;
    IOCTL_ACE_PATCHDTGCOLORCHECKER                  fpACE_PatchDTGColorChecker;
    IOCTL_ACE_GETACEINFO                            fpACE_GetACEInfo;
    IOCTL_ACE_GETACEINFOEX                          fpACE_GetACEInfoEx;
    IOCTL_ACE_PICSETCONTRAST                        fpACE_PicSetContrast;
    IOCTL_ACE_PICSETBRIGHTNESS                      fpACE_PicSetBrightness;
    IOCTL_ACE_PICSETBRIGHTNESSPRECISE               fpACE_PicSetBrightnessPrecise;
    IOCTL_ACE_PICSET_PRE_Y_OFFSET                   fpACE_PicSetPreYOffset;
    IOCTL_ACE_PICGET_PRE_Y_OFFSET                   fpACE_PicGetPreYOffset;
    IOCTL_ACE_PICSETHUE                             fpACE_PicSetHue;
    IOCTL_ACE_SKIPWAITVSYNC                         fpACE_SkipWaitVsync;
    IOCTL_ACE_PICSETSATURATION                      fpACE_PicSetSaturation;
    IOCTL_ACE_PICSETSHARPNESS                       fpACE_PicSetSharpness;
    IOCTL_ACE_PICSETCOLORTEMP                       fpACE_PicSetColorTemp;
    IOCTL_ACE_PICSETCOLORTEMPEX                     fpACE_PicSetColorTempEx;
    IOCTL_ACE_PICSETCOLORTEMP_10BITS                     fpACE_PicSetColorTemp_10Bits;
    IOCTL_ACE_PICSETPOSTCOLORTEMP_V02               fpACE_PicSetPostColorTemp_V02;
    IOCTL_ACE_SETFLESHTONE                          fpACE_SetFleshTone;
    IOCTL_ACE_SETBLACKADJUST                        fpACE_SetBlackAdjust;
    IOCTL_ACE_SET_IHC_SRAM                          fpACE_Set_IHC_SRAM;
    IOCTL_ACE_SET_ICC_SRAM                          fpACE_Set_ICC_SRAM;
    IOCTL_ACE_ENABLEMWE                             fpACE_EnableMWE;
    IOCTL_ACE_MWECLONEVISUALEFFECT                  fpACE_MWECloneVisualEffect;
    IOCTL_ACE_MWESETREGTABLE                        fpACE_MWESetRegTable;
    IOCTL_ACE_SETMWELOADFROMPQ                      fpACE_SetMWELoadFromPQ;
    IOCTL_ACE_MWESTATUS                             fpACE_MWEStatus;
    IOCTL_ACE_MWEHANDLE                             fpACE_MWEHandle;
    IOCTL_ACE_MWEFUNCSEL                            fpACE_MWEFuncSel;
    IOCTL_ACE_MWESETDISPWIN                         fpACE_MWESetDispWin;
    IOCTL_ACE_3DCLONEPQMAP                          fpACE_3DClonePQMap;
    IOCTL_ACE_DNR_BLENDING_NRTBL                    fpACE_DNR_Blending_NRTbl;
    IOCTL_ACE_DNR_BLENDING_MISC                     fpACE_DNR_Blending_MISC;
    IOCTL_ACE_DNR_BLENDING                          fpACE_DNR_Blending;
    IOCTL_ACE_DNR_GET_PRECISIONSHIFT                fpACE_DNR_Get_PrecisionShift;
    IOCTL_ACE_DNR_GETMOTION_WEIGHT                  fpACE_DNR_GetMotion_Weight;
    IOCTL_ACE_DNR_GETLUMA_WEIGHT                    fpACE_DNR_GetLuma_Weight;
    IOCTL_ACE_DNR_GETNOISE_WEIGHT                   fpACE_DNR_GetNoise_Weight;
    IOCTL_ACE_DNR_GETMOTION                         fpACE_DNR_GetMotion;
    IOCTL_ACE_DNR_INIT_MOTION                       fpACE_DNR_Init_Motion;
    IOCTL_ACE_DNR_INIT_LUMA                         fpACE_DNR_Init_Luma;
    IOCTL_ACE_DNR_INIT_NOISE                        fpACE_DNR_Init_Noise;
    IOCTL_ACE_DNR_SETPARAM                          fpACE_DNR_SetParam;
    IOCTL_ACE_DNR_GETPARAM                          fpACE_DNR_GetParam;
    IOCTL_ACE_DNR_SETNRTBL                          fpACE_DNR_SetNRTbl;
    IOCTL_ACE_GETLIBVER                             fpACE_GetLibVer;
    IOCTL_ACE_GETINFO                               fpACE_GetInfo;
    IOCTL_ACE_GETSTATUS                             fpACE_GetStatus;
    IOCTL_ACE_SETDBGLEVEL                           fpACE_SetDbgLevel;
    IOCTL_ACE_COLORCORRECTIONTABLE                  fpACE_ColorCorrectionTable;
    IOCTL_ACE_SETCOLORMATRIXCONTROL                 fpACE_SetColorMatrixControl;
    IOCTL_ACE_SETRBCHANNELRANGE                     fpACE_SetRBChannelRange;
    IOCTL_ACE_PICSETPOSTCOLORTEMP                   fpACE_PicSetPostColorTemp;
    IOCTL_ACE_PICSETPOSTCOLORTEMPEX                 fpACE_PicSetPostColorTempEx;
    IOCTL_ACE_PICSETPOSTCOLORTEMP2                  fpACE_PicSetPostColorTemp2;
    IOCTL_ACE_PICSETPOSTCOLORTEMP2EX                fpACE_PicSetPostColorTemp2Ex;
    IOCTL_ACE_SET_POWERSTATE                        fpACE_SetPowerState;
    IOCTL_ACE_SETHDRINIT                            fpACE_SetHDRInit;

    // OBSOLETE FUNCTION POINTER START
    // avoid using these functions pointers!!!!
    //
    // OBSOLETE FUNCTION POINTER END
    // avoid using these functions pointers!!!!
}ACE_INSTANCE_PRIVATE;

MS_BOOL MApi_XC_ACE_Init_U2(void *pInstance, XC_ACE_InitData *pstXC_ACE_InitData, MS_U32 u32InitDataLen);
MS_BOOL MApi_XC_ACE_Exit_U2(void *pInstance);
void MApi_XC_ACE_DMS_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bisATV);
void MApi_XC_ACE_SetPCYUV2RGB_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bEnable);
void MApi_XC_ACE_SelectYUVtoRGBMatrix_U2(void *pInstance, MS_BOOL eWindow, MS_U8 ucMatrix, MS_S16* psUserYUVtoRGBMatrix);
void MApi_XC_ACE_SetColorCorrectionTable_U2(void *pInstance, MS_BOOL bScalerWin);
void MApi_XC_ACE_SetPCsRGBTable_U2(void *pInstance, MS_BOOL bScalerWin);
void MApi_XC_ACE_GetColorMatrix_U2(void *pInstance, MS_BOOL eWindow, MS_U16* pu16Matrix);
void MApi_XC_ACE_SetColorMatrix_U2(void *pInstance, MS_BOOL eWindow, MS_U16* pu16Matrix);
E_XC_ACE_RESULT MApi_XC_ACE_SetBypassColorMatrix_U2(void *pInstance, MS_BOOL bEnable);
MS_BOOL MApi_XC_ACE_SetIHC_U2(void *pInstance, MS_BOOL bScalerWin, XC_ACE_IHC_COLOR_TYPE eIHC, MS_U8 u8Val);
MS_BOOL MApi_XC_ACE_SetICC_U2(void *pInstance, MS_BOOL bScalerWin, XC_ACE_ICC_COLOR_TYPE eICC, MS_U8 u8Val);
MS_BOOL MApi_XC_ACE_SetIBC_U2(void *pInstance, MS_BOOL bScalerWin, XC_ACE_IBC_COLOR_TYPE eIBC, MS_U8 u8Val);
void MApi_XC_ACE_PatchDTGColorChecker_U2(void *pInstance, MS_U8 u8Mode);
MS_U16 MApi_XC_ACE_GetACEInfo_U2(void *pInstance, MS_BOOL bWindow, E_XC_ACE_INFOTYPE eXCACEInfoType);
void MApi_XC_ACE_GetACEInfoEx_U2(void *pInstance, MS_BOOL bWindow, XC_ACE_InfoEx *pInfoEx);
void MApi_XC_ACE_PicSetBrightness_U2(void *pInstance, MS_BOOL eWindow, MS_U8 u8Brightness_R, MS_U8 u8Brightness_G, MS_U8 u8Brightness_B);
void MApi_XC_ACE_PicSetBrightnessPrecise_U2(void *pInstance, MS_BOOL eWindow, MS_U16 u16Brightness_R, MS_U16 u16Brightness_G, MS_U16 u16Brightness_B);
MS_BOOL MApi_XC_ACE_PicSetPreYOffset_U2(void *pInstance, MS_BOOL eWindow, MS_U8 u8PreYOffset);
MS_U8 MApi_XC_ACE_PicGetPreYOffset_U2(void *pInstance, MS_BOOL eWindow);
void MApi_XC_ACE_PicSetSharpness_U2(void *pInstance, MS_BOOL eWindow, MS_U8 u8Sharpness );
void MApi_XC_ACE_PicSetContrast_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, MS_U8 u8Contrast);
void MApi_XC_ACE_PicSetHue_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, MS_U8 u8Hue );
E_XC_ACE_RESULT MApi_XC_ACE_SkipWaitVsync_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bIsSkipWaitVsyn);
void MApi_XC_ACE_PicSetSaturation_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, MS_U8 u8Saturation);
void MApi_XC_ACE_SetFleshTone_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bEn, MS_U8 u8FleshTone);
void MApi_XC_ACE_SetBlackAdjust_U2(void *pInstance, MS_BOOL eWindow, MS_U8 u8BlackAdjust);
void MApi_XC_ACE_PicSetColorTemp_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_color_temp* pstColorTemp);
void MApi_XC_ACE_PicSetColorTempEx_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_color_temp_ex* pstColorTemp);
void MApi_XC_ACE_PicSetColorTemp_10Bits_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_color_temp_ex* pstColorTemp);
void MApi_XC_ACE_PicSetPostColorTemp_V02_U2(void *pInstance, MS_BOOL eWindow, XC_ACE_color_temp_ex2* pstColorTemp);
void MApi_XC_ACE_PicSetPostColorTemp_U2(void *pInstance, MS_BOOL eWindow, XC_ACE_color_temp* pstColorTemp);
void MApi_XC_ACE_PicSetPostColorTempEx_U2(void *pInstance, MS_BOOL eWindow, XC_ACE_color_temp_ex* pstColorTemp);
void MApi_XC_ACE_PicSetPostColorTemp2_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_color_temp* pstColorTemp);
void MApi_XC_ACE_PicSetPostColorTemp2Ex_U2(void *pInstance, MS_BOOL eWindow, MS_BOOL bUseYUVSpace, XC_ACE_color_temp_ex* pstColorTemp);
E_XC_ACE_RESULT MApi_XC_ACE_GetLibVer_U2(void *pInstance, const MSIF_Version **ppVersion);              // Get version
const XC_ACE_ApiInfo * MApi_XC_ACE_GetInfo_U2(void *pInstance);          // Get info from driver
MS_BOOL MApi_XC_ACE_GetStatus_U2(void *pInstance, XC_ACE_ApiStatus *pDrvStatus, MS_BOOL bWindow);    // Get DLC current status
MS_BOOL MApi_XC_ACE_SetDbgLevel_U2(void *pInstance, MS_U16 u16DbgSwitch);
void MApi_XC_ACE_EnableMWE_U2(void *pInstance, MS_BOOL bEnable);
void MApi_XC_ACE_MWESetMirrorMode_U2(void *pInstance, E_XC_ACE_MWE_MIRROR_TYPE eMirrorMode);
void MApi_XC_ACE_MWECloneVisualEffect_U2(void *pInstance);
void MApi_XC_ACE_3DClonePQMap_U2(void *pInstance, XC_ACE_WEAVETYPE enWeaveType);
MS_BOOL MApi_XC_ACE_MWESetRegTable_U2(void *pInstance, XC_ACE_TAB_Info *pMWETable);
MS_BOOL MApi_XC_ACE_SetMWELoadFromPQ_U2(void *pInstance, MS_BOOL bEnable);
void MApi_XC_ACE_MWESetDispWin_U2(void *pInstance, MS_U16 u16MWE_Disp_Hstart, MS_U16 u16MWE_Disp_Vstart, MS_U16 u16MWE_Disp_Width, MS_U16 u16MWE_Disp_Height);
MS_BOOL MApi_XC_ACE_MWEStatus_U2(void *pInstance);
void MApi_XC_ACE_MWEFuncSel_U2(void *pInstance, MS_BOOL eWindow, E_XC_ACE_MWE_FUNC  mwe_func);
void MApi_XC_ACE_MWEHandle_U2(void *pInstance);
MS_BOOL MApi_XC_ACE_Set_IHC_SRAM_U2(void *pInstance, MS_U16 *pBuf, MS_U16 u16ByteSize);
MS_BOOL MApi_XC_ACE_Set_ICC_SRAM_U2(void *pInstance, MS_U16 *pBuf, MS_U16 u16ByteSize);
MS_U16 MApi_XC_ACE_DNR_Get_PrecisionShift_U2(void *pInstance);
void MApi_XC_ACE_DNR_Blending_NRTbl_U2(
    void *pInstance,
    MS_U8 *pu8NewNR,
    MS_U8 u8Weight,
    MS_U8 u8Step,
    MS_U16 *pu16PreY0,
    MS_U16 *pu16PreY1,
    MS_U8 *pu8NRTBL);
MS_U8 MApi_XC_ACE_DNR_Blending_U2(void *pInstance, MS_U8 u8NewItem, MS_U16 u16PreItem, MS_U8 u8Weight);
MS_U16 MApi_XC_ACE_DNR_Blending_MISC_U2(
    void *pInstance,
    MS_U8 u8NewItem,
    MS_U16 u16PreItem,
    MS_U8 u8Weight,
    MS_U8 u8Step);
MS_U8 MApi_XC_ACE_DNR_GetMotion_U2(void *pInstance);
MS_U8 MApi_XC_ACE_DNR_GetMotion_Weight_U2(
    void *pInstance,
    MS_U8 u8CurMotion,
    MS_U8 u8PreMotion,
    MS_U8 u8CurMotionLvl,
    MS_U8 u8PreMotionLvl,
    MS_U8 u8DeFlick_Thre,
    MS_U8 u8DeFilick_Step);
MS_U8  MApi_XC_ACE_DNR_GetLuma_Weight_U2(
    void *pInstance,
    MS_U8 u8CurAvgLuma,
    MS_U8 u8PreAvgLuam,
    MS_U8 u8CurLumaLvl,
    MS_U8 u8PreLumaLvl,
    MS_U8 u8DeFlick_Th,
    MS_U8 u8DeFlick_Step);
MS_U8 MApi_XC_ACE_DNR_GetNoise_Weight_U2(
    void *pInstance,
    MS_U8 u8CurNoise,
    MS_U8 u8PreNoise,
    MS_U8 u8Range,
    MS_U8 u8DeFlick_Th,
    MS_U8 u8DeFlick_Step);
void MApi_XC_ACE_DNR_Init_Motion_U2(void *pInstance);
void MApi_XC_ACE_DNR_Init_Luma_U2(void *pInstance);
void MApi_XC_ACE_DNR_Init_Noise_U2(void *pInstance);
MS_U8 MApi_XC_ACE_DNR_GetParam_U2(void *pInstance, MS_BOOL eWindow, XC_ACE_DNR_Param eParam);
void MApi_XC_ACE_DNR_SetParam_U2(void *pInstance, MS_BOOL eWindow, XC_ACE_DNR_Param eParam, MS_U16 u16val);
void MApi_XC_ACE_DNR_SetNRTbl_U2(void *pInstance, MS_U8 *pu8Tbl);
void MApi_XC_ACE_ColorCorrectionTable_U2(void *pInstance, MS_BOOL bScalerWin, MS_S16 *psColorCorrectionTable);
void MApi_XC_ACE_SetColorMatrixControl_U2(void *pInstance, MS_BOOL bScalerWin, MS_BOOL bEnable);
void MApi_XC_ACE_SetRBChannelRange_U2(void *pInstance, MS_BOOL bScalerWin, MS_BOOL bRange);
MS_U32 MApi_XC_ACE_SetPowerState_U2(void* pInstance, EN_POWER_MODE enPowerState);
MS_BOOL MApi_XC_ACE_SetHDRInit_U2(void * pInstance ,XC_ACE_HDRinit * pstACE_HDRInitData);

//internal use only
MS_U32 _XC_ACE_GET_DEVICE_NUM(void);

void ACERegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 ACEOpen(void** ppInstance, const void* const pAttribute);
MS_U32 ACEClose(void* pInstance);
MS_U32 ACEIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);

#ifdef __cplusplus
}
#endif

#undef _API_ACE_PRIV_H_
#endif // _API_ACE_PRIV_H_
