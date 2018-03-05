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

#ifndef _API_PNL_PRIV_H_
#define _API_PNL_PRIV_H_


////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

typedef APIPNL_Result                       (*IOCTL_PNL_GETLIBVER)                              (void*, const MSIF_Version**);
typedef const PNL_ApiInfo*                  (*IOCTL_PNL_GETINFO)                                (void*);
typedef MS_BOOL                             (*IOCTL_PNL_GETSTATUS)                              (void*, PNL_ApiStatus*);
typedef MS_BOOL                             (*IOCTL_PNL_GETSTATUSEX)                            (void*, PNL_ApiExtStatus*);
typedef MS_BOOL                             (*IOCTL_PNL_SETDBGLEVEL)                            (void*, MS_U16);
typedef MS_BOOL                             (*IOCTL_PNL_IOMAPBASEINIT)                          (void*);
typedef MS_BOOL                             (*IOCTL_PNL_PREINIT)                                (void*, E_PNL_PREINIT_OPTIONS);
typedef MS_BOOL                             (*IOCTL_PNL_INIT)                                   (void*, PanelType*);
typedef MS_BOOL                             (*IOCTL_PNL_INIT_EX)                                (void*, PanelType*, MSIF_Version);
typedef APIPNL_Result                       (*IOCTL_PNL_GETCONFIG)                               (void*, PanelType*);
typedef void                                (*IOCTL_PNL_SETOUTPUT)                              (void*, APIPNL_OUTPUT_MODE);
typedef MS_BOOL                             (*IOCTL_PNL_CHANGEPANELTYPE)                        (void*, PanelType*);
typedef MS_BOOL                             (*IOCTL_PNL_TCONMAP_DUMPTABLE)                      (void*, MS_U8* ,MS_U8);
typedef MS_BOOL                             (*IOCTL_PNL_TCONMAP_POWRE_SEQUENCE)                 (void*, MS_U8* ,MS_BOOL);
typedef void                                (*IOCTL_PNL_TCON_COUNT_RESET)                       (void*, MS_BOOL);
typedef void                                (*IOCTL_PNL_TCON_INIT)                              (void*);
typedef MS_BOOL                             (*IOCTL_PNL_GETDSTINFO)                             (void*, MS_PNL_DST_DispInfo* ,MS_U32);
typedef MS_BOOL                             (*IOCTL_PNL_CONTROL_OUT_SWING)                      (void*, MS_U16);
typedef void                                (*IOCTL_PNL_SETOUTPUTPATTERN)                       (void*, MS_BOOL ,MS_U16 ,MS_U16 ,MS_U16);
typedef MS_BOOL                             (*IOCTL_PNL_MOD_CALIBRATION_SETTING)                (void*, MS_PNL_ModCaliInfo*);
typedef MS_BOOL                             (*IOCTL_PNL_MOD_DO_CALIBRATION)                     (void*);
typedef void                                (*IOCTL_PNL_BD_LVDS_OUTPUT_TYPE)                    (void*, MS_U16);
typedef void                                (*IOCTL_PNL_SETLPLLTYPEEXT)                         (void*, APIPNL_LINK_EXT_TYPE);
typedef void                                (*IOCTL_PNL_INIT_MISC)                              (void*, APIPNL_MISC);
typedef MS_U32                              (*IOCTL_PNL_GET_MISC_STATUS)                        (void*);
typedef void                                (*IOCTL_PNL_MOD_OUTPUTCONFIG_USER)                  (void*, MS_U32 ,MS_U32 ,MS_U32);
typedef void                                (*IOCTL_PNL_MOD_OUTPUTCHANNELORDER)                 (void*, MS_U8, MS_U16, MS_U16, MS_U16, MS_U16);
typedef void                                (*IOCTL_PNL_HWLVDSRESERVEDTOLRFLAG)                 (void*, MS_PNL_HW_LVDSResInfo);
typedef void                                (*IOCTL_PNL_MOD_PVDD_POWER_SETTING)                 (void*, MS_BOOL);
typedef APIPNL_Result                       (*IOCTL_PNL_SETSSC_EN)                              (void*, MS_BOOL);
typedef APIPNL_Result                       (*IOCTL_PNL_SETSSC_FMODULATION)                     (void*, MS_U16);
typedef APIPNL_Result                       (*IOCTL_PNL_SETSSC_RDEVIATION)                      (void*, MS_U16);
typedef APIPNL_Result                       (*IOCTL_PNL_SETOSDSSC_EN)                           (void*, MS_BOOL);
typedef APIPNL_Result                       (*IOCTL_PNL_SETOSDSSC_FMODULATION)                  (void*, MS_U16);
typedef APIPNL_Result                       (*IOCTL_PNL_SETOSDSSC_RDEVIATION)                   (void*, MS_U16);
typedef APIPNL_Result                       (*IOCTL_PNL_SKIPTIMINGCHANGE)                       (void*, MS_BOOL);
typedef APIPNL_Result                       (*IOCTL_PNL_PRE_SET_MODE_ON)                        (void*, MS_BOOL);
typedef APIPNL_Result                       (*IOCTL_PNL_OVERDRIVER_INIT)                        (void*, MS_PNL_OD_INITDATA*, MS_U32);
typedef APIPNL_Result                       (*IOCTL_PNL_OVERDRIVER_ENABLE)                      (void*, MS_BOOL);
typedef MS_U8                               (*IOCTL_PNL_GET_LPLL_TYPE)                          (void*);
typedef MS_BOOL                             (*IOCTL_PNL_GET_TCON_CAPABILITY)                    (void*);
typedef void                                (*IOCTL_PNL_SETPAIRSWAP)                            (void*, MS_U32);
typedef void                                (*IOCTL_PNL_SET_EXT_LPLL_TYPE)                      (void*, MS_U16);
typedef void                                (*IOCTL_PNL_CALEXTLPLLSETBYDClk)                    (void*, MS_U32);
typedef MS_BOOL                             (*IOCTL_PNL_SETDIFFSWINGLEVEL)                      (void*, MS_U8);
typedef MS_U32                              (*IOCTL_PNL_SETPOWERSTATE)                          (void*, EN_POWER_MODE);
typedef MS_BOOL                             (*IOCTL_PNL_FORCESETPANELDCLK)                      (void*, MS_U16, MS_BOOL);
typedef MS_BOOL                             (*IOCTL_PNL_FORCESETPANELHSTART)                    (void*, MS_U16, MS_BOOL);
typedef MS_BOOL                             (*IOCTL_PNL_EABLEINTERNALTERMINATION)               (void*, MS_BOOL);
typedef MS_BOOL                             (*IOCTL_PNL_OUTPUTDEVICEHANDSHAKE)                  (void*);
typedef MS_BOOL                             (*IOCTL_PNL_OUTPUTDEVICEOCHANDSHAKE)                (void*);
typedef APIPNL_Result                       (*IOCTL_PNL_SETOUTPUTINTERLACETIMING)               (void*, MS_BOOL);
typedef MS_BOOL                             (*IOCTL_PNL_GETOUTPUTINTERLACETIMING)               (void*);
typedef void                                (*IOCTL_PNL_GETPANELDATA)                           (void*, PanelType*);
typedef void                                (*IOCTL_PNL_DUMPPANELDATA)                          (void*);
typedef void                                (*IOCTL_PNL_SETSSC)                                 (void*, MS_U16, MS_U16, MS_BOOL);
typedef MS_U16                              (*IOCTL_PNL_GETPANELONTIMING)                       (void*, APIPNL_POWER_TIMING_SEQ);
typedef MS_U16                              (*IOCTL_PNL_GETPANELOFFTIMING)                      (void*, APIPNL_POWER_TIMING_SEQ);
typedef MS_U8                               (*IOCTL_PNL_GETPANELDIMCTRL)                        (void*, APIPNL_DIMMING_CTRL);
typedef MS_U8**                             (*IOCTL_PNL_GETALLGAMMATBL)                         (void*);
typedef MS_BOOL                             (*IOCTL_PNL_ENABLEPANEL)                            (void*, MS_BOOL);
typedef MS_BOOL                             (*IOCTL_PNL_SETGAMMATBL)                            (void*, APIPNL_GAMMA_TYPE, MS_U8*[], APIPNL_GAMMA_MAPPEING_MODE);
typedef MS_BOOL                             (*IOCTL_PNL_GETGAMMATBL)                            (void*, APIPNL_GAMMA_TYPE, MS_U8*[], APIPNL_GAMMA_MAPPEING_MODE);
typedef MS_BOOL                             (*IOCTL_PNL_SETGAMMAVALUE)                          (void*, MS_U8, MS_U16, MS_U16);
typedef MS_U8                               (*IOCTL_PNL_GETLPLLMODE)                            (void*);
typedef MS_U16                              (*IOCTL_PNL_GETDEFVFREQ)                            (void*);
typedef const char*                         (*IOCTL_PNL_GETPANELNAME)                           (void*);
typedef MS_U16                              (*IOCTL_PNL_GETHSTART)                              (void*);
typedef MS_U16                              (*IOCTL_PNL_GETVSTART)                              (void*);
typedef MS_U16                              (*IOCTL_PNL_GETWIDTH)                               (void*);
typedef MS_U16                              (*IOCTL_PNL_GETHEIGHT)                              (void*);
typedef MS_U16                              (*IOCTL_PNL_GETHTOTAL)                              (void*);
typedef MS_U16                              (*IOCTL_PNL_GETVTOTAL)                              (void*);
typedef MS_U8                               (*IOCTL_PNL_GETHSYNCWIDTH)                          (void*);
typedef MS_U8                               (*IOCTL_PNL_GETHSYNCBACKPORCH)                      (void*);
typedef MS_U8                               (*IOCTL_PNL_GETVSYNCBACKPORCH)                      (void*);
typedef MS_U8                               (*IOCTL_PNL_GETLPLLTYPE)                            (void*);
typedef E_PNL_ASPECT_RATIO                  (*IOCTL_PNL_GETARC)                                 (void*);
typedef MS_U32                              (*IOCTL_PNL_GETMINSET)                              (void*);
typedef MS_U32                              (*IOCTL_PNL_GETMAXSET)                              (void*);
typedef APIPNL_OUT_TIMING_MODE              (*IOCTL_PNL_GETOUTTIMINGMODE)                       (void*);
typedef MS_BOOL                             (*IOCTL_PNL_CHECKVBY1HANDSHAKESTATUS)               (void*);
typedef void                                (*IOCTL_PNL_SETVIDEOHWTRAINING)                     (void*, MS_BOOL);
typedef void                                (*IOCTL_PNL_SETOSDHWTRAINING)                       (void*, MS_BOOL);
typedef MS_BOOL                             (*IOCTL_PNL_GETVIDEOHWTRAINING)                     (void*);
typedef MS_BOOL                             (*IOCTL_PNL_GETOSDHWTRAINING)                       (void*);
typedef APIPNL_Result                       (*IOCTL_PNL_SETTING)                                (void* ,MS_U32 ,void* ,MS_U32);

typedef struct __attribute__((packed))
{
#ifdef UTOPIA_20
    IOCTL_PNL_GETLIBVER                         fpPNL_GetLibVer;
    IOCTL_PNL_GETINFO                           fpPNL_GetInfo;
    IOCTL_PNL_GETSTATUS                         fpPNL_GetStatus;
    IOCTL_PNL_GETSTATUSEX                       fpPNL_GetStatusEx;
    IOCTL_PNL_SETDBGLEVEL                       fpPNL_SetDbgLevel;
    IOCTL_PNL_IOMAPBASEINIT                     fpPNL_IOMapBaseInit;
    IOCTL_PNL_PREINIT                           fpPNL_PreInit;
    IOCTL_PNL_INIT                              fpPNL_Init;
    IOCTL_PNL_INIT_EX                           fpPNL_Init_Ex;
    IOCTL_PNL_GETCONFIG                         fpPNL_GetConfig;
    IOCTL_PNL_SETOUTPUT                         fpPNL_SetOutput;
    IOCTL_PNL_CHANGEPANELTYPE                   fpPNL_ChangePanelType;
    IOCTL_PNL_TCONMAP_DUMPTABLE                 fpPNL_TCONMAP_DumpTable;
    IOCTL_PNL_TCONMAP_POWRE_SEQUENCE            fpPNL_TCONMAP_Power_Sequence;
    IOCTL_PNL_TCON_COUNT_RESET                  fpPNL_TCON_Count_Reset;
    IOCTL_PNL_TCON_INIT                         fpPNL_TCON_Init;
    IOCTL_PNL_GETDSTINFO                        fpPNL_GetDstInfo;
    IOCTL_PNL_CONTROL_OUT_SWING                 fpPNL_Control_Out_Swing;
    IOCTL_PNL_SETOUTPUTPATTERN                  fpPNL_SetOutputPattern;
    IOCTL_PNL_MOD_CALIBRATION_SETTING           fpPNL_Mod_Calibration_Setting;
    IOCTL_PNL_MOD_DO_CALIBRATION                fpPNL_Mod_Do_Calibration;
    IOCTL_PNL_BD_LVDS_OUTPUT_TYPE               fpPNL_BD_LVDS_Output_Type;
    IOCTL_PNL_SETLPLLTYPEEXT                    fpPNL_SetLPLLTypeExt;
    IOCTL_PNL_INIT_MISC                         fpPNL_Init_MISC;
    IOCTL_PNL_GET_MISC_STATUS                   fpPNL_Get_MISC_Status;
    IOCTL_PNL_MOD_OUTPUTCONFIG_USER             fpPNL_MOD_OutputConfig_User;
    IOCTL_PNL_MOD_OUTPUTCHANNELORDER            fpPNL_MOD_OutputChannelOrder;
    IOCTL_PNL_HWLVDSRESERVEDTOLRFLAG            fpPNL_HWLVDSReservedtoLRFlag;
    IOCTL_PNL_MOD_PVDD_POWER_SETTING            fpPNL_MOD_PVDD_Power_Setting;
    IOCTL_PNL_SETSSC_EN                         fpPNL_SetSSC_En;
    IOCTL_PNL_SETSSC_FMODULATION                fpPNL_SetSSC_Fmodulation;
    IOCTL_PNL_SETSSC_RDEVIATION                 fpPNL_SetSSC_Rdeviation;
    IOCTL_PNL_SETOSDSSC_EN                      fpPNL_SetOSDSSC_En;
    IOCTL_PNL_SETOSDSSC_FMODULATION             fpPNL_SetOSDSSC_Fmodulation;
    IOCTL_PNL_SETOSDSSC_RDEVIATION              fpPNL_SetOSDSSC_Rdeviation;
    IOCTL_PNL_SKIPTIMINGCHANGE                  fpPNL_SkipTimingChange;
    IOCTL_PNL_PRE_SET_MODE_ON                   fpPNL_PreSetModeOn;
    IOCTL_PNL_OVERDRIVER_INIT                   fpPNL_OverDriver_Init;
    IOCTL_PNL_OVERDRIVER_ENABLE                 fpPNL_OverDriver_Enable;
    IOCTL_PNL_GET_LPLL_TYPE                     fpPNL_Get_LPLL_Type;
    IOCTL_PNL_GET_TCON_CAPABILITY               fpPNL_Get_TCON_Capability;
    IOCTL_PNL_SETPAIRSWAP                       fpPNL_SetPairSwap;
    IOCTL_PNL_SET_EXT_LPLL_TYPE                 fpPNL_SetExt_LPLL_Type;
    IOCTL_PNL_CALEXTLPLLSETBYDClk               fpPNL_CalExtLPLLSETbyDClk;
    IOCTL_PNL_SETDIFFSWINGLEVEL                 fpPNL_SetDiffSwingLevel;
    IOCTL_PNL_SETPOWERSTATE                     fpPNL_SetPowerState;
    IOCTL_PNL_FORCESETPANELDCLK                 fpPNL_ForceSetPanelDCLK;
    IOCTL_PNL_FORCESETPANELHSTART               fpPNL_ForceSetPanelHStart;
    IOCTL_PNL_EABLEINTERNALTERMINATION          fpPNL_EnableInternalTermination;
    IOCTL_PNL_OUTPUTDEVICEHANDSHAKE             fpPNL_OutputDeviceHandshake;
    IOCTL_PNL_OUTPUTDEVICEOCHANDSHAKE           fpPNL_OutputDeviceOCHandshake;
    IOCTL_PNL_SETOUTPUTINTERLACETIMING          fpPNL_SetOutputInterlaceTiming;
    IOCTL_PNL_GETOUTPUTINTERLACETIMING          fpPNL_GetOutputInterlaceTiming;
    IOCTL_PNL_GETPANELDATA                      fpPNL_GetPanelData;
    IOCTL_PNL_DUMPPANELDATA                     fpPNL_DumpPanelData;
    IOCTL_PNL_SETSSC                            fpPNL_SetSSC;
    IOCTL_PNL_GETPANELONTIMING                  fpPNL_GetPanelOnTiming;
    IOCTL_PNL_GETPANELOFFTIMING                 fpPNL_GetPanelOffTiming;
    IOCTL_PNL_GETPANELDIMCTRL                   fpPNL_GetPanelDimCtrl;
    IOCTL_PNL_GETALLGAMMATBL                    fpPNL_GetAllGammaTbl;
    IOCTL_PNL_ENABLEPANEL                       fpPNL_EnablePanel;
    IOCTL_PNL_SETGAMMATBL                       fpPNL_SetGammaTbl;
    IOCTL_PNL_GETGAMMATBL                       fpPNL_GetGammaTbl;
    IOCTL_PNL_SETGAMMAVALUE                     fpPNL_SetGammaValue;
    IOCTL_PNL_GETLPLLMODE                       fpPNL_GetLPLLMode;
    IOCTL_PNL_GETDEFVFREQ                       fpPNL_GetDefVFreq;
    IOCTL_PNL_GETPANELNAME                      fpPNL_GetPanelName;
    IOCTL_PNL_GETHSTART                         fpPNL_GetHStart;
    IOCTL_PNL_GETVSTART                         fpPNL_GetVStart;
    IOCTL_PNL_GETWIDTH                          fpPNL_GetWidth;
    IOCTL_PNL_GETHEIGHT                         fpPNL_GetHeight;
    IOCTL_PNL_GETHTOTAL                         fpPNL_GetHtotal;
    IOCTL_PNL_GETVTOTAL                         fpPNL_GetVtotal;
    IOCTL_PNL_GETHSYNCWIDTH                     fpPNL_GetHsyncwidth;
    IOCTL_PNL_GETHSYNCBACKPORCH                 fpPNL_GetHsyncBackPorch;
    IOCTL_PNL_GETVSYNCBACKPORCH                 fpPNL_GetVsyncBackPorch;
    IOCTL_PNL_GETLPLLTYPE                       fpPNL_GetLPLLType;
    IOCTL_PNL_GETARC                            fpPNL_GetARC;
    IOCTL_PNL_GETMINSET                         fpPNL_GetMinSET;
    IOCTL_PNL_GETMAXSET                         fpPNL_GetMaxSET;
    IOCTL_PNL_GETOUTTIMINGMODE                  fpPNL_GetOutTimingMode;
    IOCTL_PNL_CHECKVBY1HANDSHAKESTATUS          fpPNL_CheckVBY1HandshakeStatus;
    IOCTL_PNL_SETVIDEOHWTRAINING                fpPNL_SetVideoHWTraining;
    IOCTL_PNL_SETOSDHWTRAINING                  fpPNL_SetOSDHWTraining;
    IOCTL_PNL_GETVIDEOHWTRAINING                fpPNL_GetVideoHWTraining_Status;
    IOCTL_PNL_GETOSDHWTRAINING                  fpPNL_GetOSDHWTraining_Status;
    IOCTL_PNL_SETTING                           fpPNL_Setting;
#endif
    // Device ID
    MS_U32 u32DeviceID;
}PNL_INSTANCE_PRIVATE;

// apiPNL.c
#define GammaTblSize 386
typedef struct __attribute__((packed))
{
    PanelType                    _stPanelTypeSaved;
    MS_U8                        **m_ppAllGammaTbls;
#if !defined (__aarch64__)
    MS_U32 u32AlignmentDummy0;
#endif
    APIPNL_GAMMA_TYPE            _eGammaType;
    APIPNL_GAMMA_MAPPEING_MODE   _eGammaMapMode;
    MS_U8                        _pu8GammaTab[3][GammaTblSize];
    MS_BOOL                      _bSkipTimingChange;

    // Library info
    PNL_ApiInfo         _cstPnlInfo;
    PNL_ApiStatus       _stPnlStatus;

    MS_U16 _u16PanelDCLK;
    MS_BOOL _bForceSetDCLKEnable;

    MS_U16 _u16PanelHStart;
    MS_BOOL _bForceSetHStartEnable;

    // MApi_PNL_SetPowerState()
    EN_POWER_MODE ePrevPowerState;
} ST_APIPNL;

// drvPNL.h
typedef struct __attribute__((packed))
{
    PNL_InitData _stPnlInitData;
    MS_BOOL g_bDrvPnl_UseTConTable_Mod;
} ST_DRVPNL;

// halPNL.c
typedef struct __attribute__((packed))
{
    // Output swing = 150mV + Code * 10mV.
    // Designer recommand value. 0x15 = 21 = 250mv
    MS_U16  _u16PnlDefault_SwingLevel;
    PNL_OUTPUT_MODE   _eDrvPnlInitOptions;
    MS_U8 _u8MOD_CALI_TARGET;     ///< MOD_REG(0x7D),[10:9]00: 250mV ,01: 350mV ,10:300mV ,11:200mV
    MS_U8 _u8MOD_CALI_PAIR_SEL;    ///< MOD_REG(0x7D)[3:2]00: ch2, 01:ch6, 10:ch8, 11:ch12

    MS_S8 _usMOD_CALI_OFFSET;        ///< MOD_REG(0x7D),[5:0]+ _usMOD_CALI_OFFSET
    MS_U8 _u8MOD_CALI_VALUE;     /// Final value

    MS_U8 _u8MOD_LVDS_Pair_Shift;       ///< 0:default setting, LVDS pair Shift
    MS_U8 _u8MOD_LVDS_Pair_Type;        ///< 0:default setting, LVDS data differential pair
    MS_U8  _u8PnlDiffSwingLevel;
    MS_BOOL _bPVDD_2V5;
} ST_HALPNL;

typedef struct __attribute__((packed))
{
    MSIF_Version _api_pnl_version_sel ;
    MS_BOOL bResourceRegistered;

    ST_APIPNL stapiPNL;
    ST_DRVPNL stdrvPNL;
    ST_HALPNL sthalPNL;
}PNL_RESOURCE_PRIVATE;
typedef struct
{
    MS_U16 BD_LVDS_Output_Type;
    PanelType pSelPanelType;
}PNL_REGS_SAVE_AREA;
APIPNL_Result MApi_PNL_GetLibVer_U2(void* pInstance, const MSIF_Version **ppVersion);
const PNL_ApiInfo* MApi_PNL_GetInfo_U2(void *pInstance);
MS_BOOL MApi_PNL_GetStatus_U2(void *pInstance, PNL_ApiStatus *pPnlStatus);
MS_BOOL MApi_PNL_GetStatusEx_U2(void *pInstance, PNL_ApiExtStatus *pPnlExtStatus);
MS_BOOL MApi_PNL_SetDbgLevel_U2(void *pInstance, MS_U16 u16DbgSwitch);
MS_BOOL MApi_PNL_IOMapBaseInit_U2(void *pInstance);
MS_BOOL MApi_PNL_PreInit_U2(void *pInstance, E_PNL_PREINIT_OPTIONS eInitParam);
MS_BOOL MApi_PNL_Init_U2(void *pInstance, PanelType *pSelPanelType/* <in > */);
MS_BOOL MApi_PNL_Init_Ex_U2(void *pInstance, PanelType *pSelPanelType/* <in > */, MSIF_Version LIBVER);
APIPNL_Result MApi_PNL_GetConfig_U2(void *pInstance, PanelType *pSelPanelType);
void MApi_PNL_SetOutput_U2(void *pInstance, APIPNL_OUTPUT_MODE eOutputMode);
MS_BOOL MApi_PNL_ChangePanelType_U2(void *pInstance, PanelType *pSelPanelType);
MS_BOOL MApi_PNL_TCONMAP_DumpTable_U2(void *pInstance, MS_U8 *pTCONTable, MS_U8 u8Tcontype);
MS_BOOL MApi_PNL_TCONMAP_Power_Sequence_U2(void *pInstance, MS_U8 *pTCONTable, MS_BOOL bEnable);
void MApi_PNL_TCON_Count_Reset_U2 (void *pInstance, MS_BOOL bEnable );
void MApi_PNL_TCON_Init_U2(void *pInstance);
MS_BOOL MApi_PNL_GetDstInfo_U2(void *pInstance, MS_PNL_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo);
MS_BOOL MApi_PNL_Control_Out_Swing_U2(void *pInstance, MS_U16 u16Swing_Level);
void MApi_PNL_SetOutputPattern_U2(void *pInstance, MS_BOOL bEnable, MS_U16 u16Red , MS_U16 u16Green, MS_U16 u16Blue);
MS_BOOL MApi_Mod_Calibration_Setting_U2(void *pInstance, MS_PNL_ModCaliInfo *pstModCaliInfo);
MS_BOOL MApi_Mod_Do_Calibration_U2(void *pInstance);
void MApi_BD_LVDS_Output_Type_U2(void *pInstance, MS_U16 Type);
void MApi_PNL_SetLPLLTypeExt_U2(void *pInstance, APIPNL_LINK_EXT_TYPE eLPLL_TypeExt);
void MApi_PNL_Init_MISC_U2(void *pInstance, APIPNL_MISC ePNL_MISC);
MS_U32 MApi_PNL_GetMiscStatus_U2(void* pInstance);
void MApi_PNL_MOD_OutputConfig_User_U2(void *pInstance, MS_U32 u32OutputCFG0_7, MS_U32 u32OutputCFG8_15, MS_U32 u32OutputCFG16_21);
void MApi_PNL_MOD_OutputChannelOrder_U2(void* pInstance,
                                        MS_U8  u8OutputOrderType,
                                        MS_U16 u16OutputOrder0_3,
                                        MS_U16 u16OutputOrder4_7,
                                        MS_U16 u16OutputOrder8_11,
                                        MS_U16 u16OutputOrder12_13);
void MApi_PNL_HWLVDSReservedtoLRFlag_U2(void *pInstance, MS_PNL_HW_LVDSResInfo lvdsresinfo);
void MApi_MOD_PVDD_Power_Setting_U2(void *pInstance, MS_BOOL bIs2p5);
APIPNL_Result MApi_PNL_SetSSC_En_U2(void *pInstance, MS_BOOL bEnable);
APIPNL_Result MApi_PNL_SetSSC_Fmodulation_U2(void *pInstance, MS_U16 u16Fmodulation);
APIPNL_Result MApi_PNL_SetSSC_Rdeviation_U2(void *pInstance, MS_U16 u16Rdeviation);
APIPNL_Result MApi_PNL_SetOSDSSC_En_U2(void *pInstance, MS_BOOL bEnable);
APIPNL_Result MApi_PNL_SetOSDSSC_Fmodulation_U2(void *pInstance, MS_U16 u16Fmodulation);
APIPNL_Result MApi_PNL_SetOSDSSC_Rdeviation_U2(void *pInstance, MS_U16 u16Rdeviation);
APIPNL_Result MApi_PNL_SkipTimingChange_U2(void *pInstance, MS_BOOL bFlag);
APIPNL_Result MApi_PNL_PreSetModeOn_U2(void *pInstance, MS_BOOL bSetMode);
APIPNL_Result MApi_PNL_OverDriver_Init_U2(void *pInstance, MS_PNL_OD_INITDATA *pPNL_ODInitData, MS_U32 u32ODInitDataLen);
APIPNL_Result MApi_PNL_OverDriver_Enable_U2(void *pInstance, MS_BOOL bEnable);
MS_U8 MApi_Pnl_Get_LPLL_Type_U2(void *pInstance);
MS_BOOL MApi_PNL_Get_TCON_Capability_U2(void *pInstance);
void MApi_PNL_SetPairSwap_U2(void *pInstance, MS_U32 u32Polarity);
void MApi_PNL_SetExt_LPLL_Type_U2(void *pInstance, MS_U16 u16Ext_lpll_type);
void MApi_PNL_CalExtLPLLSETbyDClk_U2(void *pInstance, MS_U32 ldHz);
MS_BOOL MApi_PNL_SetDiffSwingLevel_U2(void *pInstance, MS_U8 u8Swing_Level);
MS_U32 MApi_PNL_SetPowerState_U2(void *pInstance, EN_POWER_MODE);
MS_BOOL MApi_PNL_ForceSetPanelDCLK_U2(void *pInstance, MS_U16 u16PanelDCLK, MS_BOOL bSetDCLKEnable);
MS_BOOL MApi_PNL_ForceSetPanelHStart_U2(void *pInstance, MS_U16 u16PanelHStart ,MS_BOOL bSetHStartEnable);
MS_BOOL MApi_PNL_EnableInternalTermination_U2(void *pInstance, MS_BOOL bEnable);
MS_BOOL MApi_PNL_OutputDeviceHandshake_U2(void *pInstance);
MS_BOOL MApi_PNL_OutputDeviceOCHandshake_U2(void *pInstance);
APIPNL_Result MApi_PNL_SetOutputInterlaceTiming_U2(void* pInstance, MS_BOOL bEnable);
MS_BOOL MApi_PNL_GetOutputInterlaceTiming_U2(void *pInstance);
void MApi_PNL_GetPanelData_U2(void* pInstance, PanelType* pstPNLData);
void MApi_PNL_DumpPanelData_U2(void* pInstance);
void MApi_PNL_SetSSC_U2(void* pInstance, MS_U16 u16Fmodulation, MS_U16 u16Rdeviation, MS_BOOL bEnable);
MS_U16 MApi_PNL_GetPanelOnTiming_U2(void* pInstance, APIPNL_POWER_TIMING_SEQ seq);
MS_U16 MApi_PNL_GetPanelOffTiming_U2(void* pInstance, APIPNL_POWER_TIMING_SEQ seq);
MS_U8 MApi_PNL_GetPanelDimCtrl_U2(void* pInstance, APIPNL_DIMMING_CTRL dim_type);
MS_U8** MApi_PNL_GetAllGammaTbl_U2(void* pInstance);
MS_BOOL MApi_PNL_EnablePanel_U2(void* pInstance, MS_BOOL bPanelOn);
MS_BOOL MApi_PNL_SetGammaTbl_U2(void* pInstance, APIPNL_GAMMA_TYPE eGammaType, MS_U8* pu8GammaTab[3], APIPNL_GAMMA_MAPPEING_MODE Gamma_Map_Mode);
MS_BOOL MApi_PNL_GetGammaTbl_U2(void* pInstance, APIPNL_GAMMA_TYPE eGammaType, MS_U8* pu8GammaTab[3], APIPNL_GAMMA_MAPPEING_MODE Gamma_Map_Mode);
MS_BOOL MApi_PNL_SetGammaValue_U2(void* pInstance, MS_U8 u8Channel, MS_U16 u16Offset, MS_U16 u16GammaValue);
MS_U8 MApi_PNL_GetLPLLMode_U2(void *pInstance);
MS_U16 MApi_PNL_GetDefVFreq_U2(void *pInstance);
const char* MApi_PNL_GetName_U2(void *pInstance);
MS_U16 MAPI_PNL_GetPNLHstart_U2(void *pInstance);
MS_U16 MAPI_PNL_GetPNLVstart_U2(void *pInstance);
MS_U16 MAPI_PNL_GetPNLWidth_U2(void *pInstance);
MS_U16 MAPI_PNL_GetPNLHeight_U2(void *pInstance);
MS_U16 MAPI_PNL_GetPNLHtotal_U2(void *pInstance);
MS_U16 MAPI_PNL_GetPNLVtotal_U2(void *pInstance);
MS_U8  MAPI_PNL_GetPNLHsyncWidth_U2(void *pInstance);
MS_U8  MAPI_PNL_GetPNLHsyncBackPorch_U2(void *pInstance);
MS_U8  MAPI_PNL_GetPNLVsyncBackPorch_U2(void *pInstance);
MS_U8  MApi_PNL_GetLPLLType_U2(void *pInstance);
E_PNL_ASPECT_RATIO MApi_PNL_GetARC_U2(void *pInstance);
MS_U32 MApi_PNL_GetMinSET_U2(void *pInstance);
MS_U32 MApi_PNL_GetMaxSET_U2(void *pInstance);
APIPNL_OUT_TIMING_MODE MApi_PNL_GetOutTimingMode_U2(void *pInstance);
void MApi_PNL_SetVideoHWTraining_U2(void *pInstance, MS_BOOL bEnable);
void MApi_PNL_SetOSDHWTraining_U2(void *pInstance, MS_BOOL bEnable);
MS_BOOL MApi_PNL_GetVideoHWTraining_Status_U2(void *pInstance);
MS_BOOL MApi_PNL_GetOSDHWTraining_Status_U2(void *pInstance);
APIPNL_Result MApi_PNL_Setting_U2(void* pInstance,MS_U32 u32Cmd,void *pCmdArgs,MS_U32 u32CmdArgsSize);


void PNLRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 PNLOpen(void** ppInstance, const void* const pAttribute);
MS_U32 PNLClose(void* pInstance);
MS_U32 PNLIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);

//internal use only
MS_U32 _XC_PNL_GET_DEVICE_NUM(void);
MS_BOOL MApi_PNL_Check_VBY1_Handshake_Status_U2(void *pInstance);

#ifdef __cplusplus
}
#endif

#undef _API_PNL_PRIV_H_
#endif // _API_PNL_PRIV_H_
