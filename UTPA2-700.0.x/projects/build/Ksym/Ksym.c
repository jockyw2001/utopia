#include "MsTypes.h"
#include "MsCommon.h"
#include "MLog.h"
#include "apiACP.h"
#include "apiAUDIO.h"
#include "apiCEC.h"
#include "apiDAC.h"
#include "apiDMS.h"
#include "apiDMX.h"
#include "apiDMX_tee.h"
#include "apiDMX_tsio.h"
#include "apiGOP.h"
#include "apiGOPSC_Ex.h"
#include "apiGOP_SC.h"
#include "apiGPD.h"
#include "apiGPD_v2.h"
#include "apiHDMITx.h"
#include "apiHDMITx2.h"
#include "apiJPEG.h"
#include "apiJPEG_v2.h"
#include "apiLD.h"
#include "drvMBX.h"
#include "apiMFE_v2.h"
#include "apiMHL.h"
#include "apiPNL.h"
#include "apiPNL_EX.h"
#include "apiSWI2C.h"
#include "apiVDEC.h"
#include "apiVDEC_EX.h"
#include "apiVDEC_EX_v2.h"
#include "apiVOIPMFE.h"
#include "apiXC.h"
#include "apiXC_Ace.h"
#include "apiXC_Ace_EX.h"
#include "apiXC_Adc.h"
#include "apiXC_EX.h"
#include "apiXC_Adc_EX.h"
#include "apiXC_Auto.h"
#include "apiXC_Auto_EX.h"
#include "apiXC_DWIN.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "apiXC_DWIN_EX.h"
#include "apiXC_Dlc.h"
#include "apiXC_ModeParse_EX.h"
#include "apiXC_PCMonitor_EX.h"
#include "apiXC_VMark.h"
#include "apiXC_VMark_EX.h"
#include "asmCPU.h"
#include "drvAESDMA.h"
#include "drvCIPHER.h"
#include "drvAUDIO.h"
#include "drvAVD.h"
#include "drvBDMA.h"
#include "drvCA.h"
#include "drvCA_v2.h"
#include "drvCH34.h"
#include "drvCIPHER_v2.h"
#include "drvCLKM.h"
#include "drvCMAPool_v2.h"
#include "drvCMDQ.h"
#include "drvCPU.h"
#include "drvDDC2BI.h"
#include "drvDIP.h"
#include "drvDMD_ATSC.h"
#include "drvDMD_ATSC_v2.h"
#include "drvDMD_DTMB.h"
#include "drvDMD_DTMB_v2.h"
#include "drvDMD_common.h"
#include "drvDMD_ISDBT.h"
#include "drvDMD_ISDBT_v2.h"
#include "drvDMD_VD_MBX.h"
#include "drvDMD_INTERN_ATSC.h"
#include "drvDMD_INTERN_DVBC.h"
#include "drvDMD_INTERN_DVBC_v2.h"
#include "drvDMD_INTERN_DVBS.h"
#include "drvDMD_INTERN_DVBS_v2.h"
#include "drvDMD_INTERN_DVBT.h"
#include "drvDMD_INTERN_DVBT_v2.h"
#include "drvDMD_INTERN_DVBT2.h"
#include "drvDMD_INTERN_DVBT2_v2.h"
#include "drvDSCMB.h"
#include "drvDSCMB_v2.h"
#include "drvEMMFlt.h"
#include "drvGPIO.h"
#include "drvHDCP.h"
#include "drvHVD.h"
#include "drvHWI2C.h"
#include "drvIPAUTH.h"
#include "drvIR.h"
#include "drvIRQ.h"
#include "drvLDM.h"
#include "drvMFC.h"
#include "drvMMIO.h"
#include "drvMPIF.h"
#include "drvMSPI.h"
#include "drvMVD.h"
#include "drvMVOP.h"
#include "drvMVOP_v2.h"
#include "drvNDS.h"
#include "drvNSK2.h"
#include "drvPARFLASH.h"
#include "drvPCMCIA.h"
#include "drvPM.h"
#include "drvPVR_IframeLUT.h"
#include "drvPWM.h"
#include "drvPWS.h"
#include "drvRTC.h"
#include "drvRVD.h"
#include "drvSAR.h"
#include "drvSC.h"
#include "drvSEAL.h"
#include "drvSEM.h"
#include "drvSERFLASH.h"
#include "drvSMBX.h"
#include "drvSYS.h"
#include "drvTCF.h"
#include "drvTVEncoder.h"
#include "drvUART.h"
#include "drvURDMA.h"
#include "drvVBI.h"
#include "drvVIF.h"
#include "drvWBLE.h"
#include "drvWDT.h"
#include "drvXC_HDMI_if.h"
#include "drvXC_HDMI_if_EX.h"
#include "drv_IR_TX.h"
#include "drv_gop_sc.h"
#include "apiVDEC_EX.h"
#include "apiXC.h"
#include "drvCIPHER.h"
#include "drvDMD_common.h"
#include "drvLDM.h"
#include "drvSAR.h"
#include "drvTVEncoder.h"
#include "MsOS.h"
#include "apiAUDIO.h"
#include "apiDAC.h"
#include "apiDMS.h"
#include "apiDMX.h"
#include "apiGOP.h"
#include "apiPNL.h"
#include "apiXC.h"
#include "apiXC_DWIN.h"
#include "apiXC_Dlc.h"
#include "drvDMD_ATSC.h"
#include "drvMVOP.h"
#include "apiGOP.h"
#include "apiXC_DBC.h"
#include "apiMBX.h"
#include "cusSAR.h"
#ifdef _DAC_V2_H_
#ifdef __cplusplus
#endif
#ifdef UFO_XC_GETOUTPUTINTELACETIMING
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_XC_H_
#ifdef __cplusplus
#endif
#ifdef MVIDEO_C
#else
#endif
#ifdef MSOS_TYPE_LINUX_KERNEL
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#elif defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_300)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_212) ||defined(UFO_PUBLIC_HEADER_500_3) ||defined(UFO_PUBLIC_HEADER_700)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_212) ||defined(UFO_PUBLIC_HEADER_500_3) ||defined(UFO_PUBLIC_HEADER_700)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#endif
#ifdef UFO_XC_FORCEWRITE_V2
#else
#endif
#ifdef UFO_PUBLIC_HEADER_212
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#else
#endif
#if defined(UFO_XC_AUTO_DOWNLOAD)
#endif
#ifdef UFO_XC_HDR
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#endif
#if defined (UFO_PUBLIC_HEADER_212) ||defined (UFO_PUBLIC_HEADER_500_3) ||defined (UFO_PUBLIC_HEADER_700)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#else
#endif
#ifdef UFO_XC_ZORDER
#endif
#if defined(UFO_PUBLIC_HEADER_212) ||defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#if API_XCSTATUS_EX_VERSION >= 9
#endif
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_212) ||defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_700)
#endif
#ifdef UFO_XC_SETBLACKVIDEOBYMODE
#endif
#ifdef UFO_XC_TEST_PATTERN
#endif
#ifdef UFO_XC_AUTO_DOWNLOAD
#if defined(UFO_PUBLIC_HEADER_700)
#else
#endif
#endif
#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
#ifdef UFO_XC_HDMI_4K2K_DMA
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#endif
#endif
#ifdef UFO_XC_HDR
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#elif defined(UFO_PUBLIC_HEADER_500)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_700) || defined(UFO_PUBLIC_HEADER_500)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_700) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
#endif
#if defined(UFO_PUBLIC_HEADER_700) || defined(UFO_PUBLIC_HEADER_500) ||defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
#endif
#if defined(UFO_PUBLIC_HEADER_700) || defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212) || defined(UFO_PUBLIC_HEADER_500)
#endif
#if !defined (__aarch64__)
#endif
#if !defined (__aarch64__)
#endif
#if !defined (__aarch64__)
#endif
#if defined(UFO_PUBLIC_HEADER_700) ||defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#ifdef UFO_XC_SCAN_TYPE
#endif
EXPORT_SYMBOL(MApi_XC_GetLibVer);
EXPORT_SYMBOL(MApi_XC_GetInfo);
EXPORT_SYMBOL(MApi_XC_GetStatus);
EXPORT_SYMBOL(MApi_XC_GetStatusEx);
EXPORT_SYMBOL(MApi_XC_GetStatusNodelay);
EXPORT_SYMBOL(MApi_XC_SetDbgLevel);
EXPORT_SYMBOL(MApi_XC_FPLLDbgMode);
EXPORT_SYMBOL(MApi_XC_FPLLCustomerMode);
EXPORT_SYMBOL(MDrv_XC_SetIOMapBase);
EXPORT_SYMBOL(MApi_XC_Init);
EXPORT_SYMBOL(MApi_XC_GetConfig);
EXPORT_SYMBOL(MApi_XC_Init_MISC);
EXPORT_SYMBOL(MApi_XC_GetMISCStatus);
EXPORT_SYMBOL(MApi_XC_GetCapability);
EXPORT_SYMBOL(MApi_XC_GetChipCaps);
EXPORT_SYMBOL(MApi_XC_Exit);
EXPORT_SYMBOL(MApi_XC_SetDynamicScaling);
EXPORT_SYMBOL(MApi_XC_Set_DynamicScalingFlag);
EXPORT_SYMBOL(MApi_XC_GetDynamicScalingStatus);
#ifdef UFO_XC_SWDS_SW_FILM_ENABLE
#endif
#if defined(UFO_PUBLIC_HEADER_700)
EXPORT_SYMBOL(MApi_XC_Get_DNRBaseOffset);
#else
EXPORT_SYMBOL(MApi_XC_Get_DNRBaseOffset);
#endif
EXPORT_SYMBOL(MApi_XC_Get_FrameNumFactor);
EXPORT_SYMBOL(MApi_XC_Get_FRCMFrameNum);
EXPORT_SYMBOL(MApi_XC_SetWindow);
EXPORT_SYMBOL(MApi_XC_SetDualWindow);
EXPORT_SYMBOL(MApi_XC_SetTravelingWindow);
EXPORT_SYMBOL(MApi_XC_SetInputSource);
#ifdef UFO_PUBLIC_HEADER_212
#endif
EXPORT_SYMBOL(MApi_XC_IsYUVSpace);
EXPORT_SYMBOL(MApi_XC_IsMemoryFormat422);
EXPORT_SYMBOL(MApi_XC_EnableForceRGBin);
EXPORT_SYMBOL(MApi_XC_EnableMirrorModeEx);
EXPORT_SYMBOL(MApi_XC_GetMirrorModeTypeEx);
EXPORT_SYMBOL(MApi_XC_Is_SupportSWDS);
EXPORT_SYMBOL(MApi_XC_SetDSInfo);
EXPORT_SYMBOL(MApi_XC_GetSWDSIndex);
EXPORT_SYMBOL(MApi_XC_GetSyncStatus);
EXPORT_SYMBOL(MApi_XC_WaitOutputVSync);
EXPORT_SYMBOL(MApi_XC_WaitInputVSync);
EXPORT_SYMBOL(MApi_XC_SetHdmiSyncMode);
EXPORT_SYMBOL(MApi_XC_GetHdmiSyncMode);
EXPORT_SYMBOL(MApi_XC_SetRepWindow);
EXPORT_SYMBOL(MApi_XC_SetSkipDisableOPWriteOffInFPLL);
EXPORT_SYMBOL(MApi_XC_Set_OPWriteOffEnable);
EXPORT_SYMBOL(MApi_XC_ForceSet_OPWriteOffEnable);
EXPORT_SYMBOL(MApi_XC_Set_OPWriteOffEnableToReg);
EXPORT_SYMBOL(MApi_XC_Get_OPWriteOffEnable);
EXPORT_SYMBOL(MApi_XC_SkipOPWriteOffInSetWindow);
EXPORT_SYMBOL(MApi_XC_GetSkipOPWriteOffInSetWindow);
EXPORT_SYMBOL(MApi_XC_SetDispWinToReg);
EXPORT_SYMBOL(MApi_XC_GetDispWinFromReg);
EXPORT_SYMBOL(MApi_XC_FreezeImg);
EXPORT_SYMBOL(MApi_XC_IsFreezeImg);
EXPORT_SYMBOL(MApi_XC_GenerateBlackVideoForBothWin);
EXPORT_SYMBOL(MApi_XC_Set_BLSK);
EXPORT_SYMBOL(MApi_XC_GenerateBlackVideo);
EXPORT_SYMBOL(MApi_XC_IsBlackVideoEnable);
EXPORT_SYMBOL(MApi_XC_EnableFrameBufferLess);
EXPORT_SYMBOL(MApi_XC_IsCurrentFrameBufferLessMode);
EXPORT_SYMBOL(MApi_XC_EnableRequest_FrameBufferLess);
EXPORT_SYMBOL(MApi_XC_IsCurrentRequest_FrameBufferLessMode);
EXPORT_SYMBOL(MApi_XC_Get_3D_HW_Version);
EXPORT_SYMBOL(MApi_XC_Get_3D_IsSupportedHW2DTo3D);
EXPORT_SYMBOL(MApi_XC_Set_3D_Mode);
EXPORT_SYMBOL(MApi_XC_Set_3D_MainWin_FirstMode);
EXPORT_SYMBOL(MApi_XC_Set_3D_LR_Frame_Exchg);
EXPORT_SYMBOL(MApi_XC_3D_Is_LR_Frame_Exchged);
EXPORT_SYMBOL(MApi_XC_Get_3D_Input_Mode);
EXPORT_SYMBOL(MApi_XC_Get_3D_Output_Mode);
EXPORT_SYMBOL(MApi_XC_Get_3D_Panel_Type);
EXPORT_SYMBOL(MApi_XC_Get_3D_MainWin_First);
EXPORT_SYMBOL(MApi_XC_3DMainSub_IPSync);
EXPORT_SYMBOL(MApi_XC_Set_3D_VerVideoOffset);
EXPORT_SYMBOL(MApi_XC_Get_3D_VerVideoOffset);
EXPORT_SYMBOL(MApi_XC_Is3DFormatSupported);
EXPORT_SYMBOL(MApi_XC_Set_3D_HShift);
EXPORT_SYMBOL(MApi_XC_Enable_3D_LR_Sbs2Line);
EXPORT_SYMBOL(MApi_XC_Get_3D_HShift);
EXPORT_SYMBOL(MApi_XC_Set_3D_HW2DTo3D_Buffer);
EXPORT_SYMBOL(MApi_XC_Set_3D_HW2DTo3D_Parameters);
EXPORT_SYMBOL(MApi_XC_Get_3D_HW2DTo3D_Parameters);
EXPORT_SYMBOL(MApi_XC_Set_3D_Detect3DFormat_Parameters);
EXPORT_SYMBOL(MApi_XC_Get_3D_Detect3DFormat_Parameters);
EXPORT_SYMBOL(MApi_XC_Detect3DFormatByContent);
EXPORT_SYMBOL(MApi_XC_DetectNL);
EXPORT_SYMBOL(MApi_XC_3D_PostPQSetting);
EXPORT_SYMBOL(MApi_XC_Set_3D_FPInfo);
EXPORT_SYMBOL(MApi_XC_EnableAutoDetect3D);
EXPORT_SYMBOL(MApi_XC_GetAutoDetect3DFlag);
EXPORT_SYMBOL(MApi_XC_Set_3D_SubWinClk);
EXPORT_SYMBOL(MApi_XC_3D_Is_LR_Sbs2Line);
EXPORT_SYMBOL(MApi_SC_3D_Is_Skip_Default_LR_Flag);
EXPORT_SYMBOL(MApi_XC_3D_Enable_Skip_Default_LR_Flag);
EXPORT_SYMBOL(MApi_XC_Mux_Init);
EXPORT_SYMBOL(MApi_XC_Mux_SourceMonitor);
EXPORT_SYMBOL(MApi_XC_Mux_CreatePath);
EXPORT_SYMBOL(MApi_XC_Mux_DeletePath);
EXPORT_SYMBOL(MApi_XC_Mux_EnablePath);
EXPORT_SYMBOL(MApi_XC_Mux_TriggerPathSyncEvent);
EXPORT_SYMBOL(MApi_XC_Mux_TriggerDestOnOffEvent);
EXPORT_SYMBOL(MApi_XC_Mux_OnOffPeriodicHandler);
EXPORT_SYMBOL(MApi_XC_Mux_GetPathInfo);
EXPORT_SYMBOL(MApi_XC_Mux_SetSupportMhlPathInfo);
EXPORT_SYMBOL(MApi_XC_Mux_SetMhlHotPlugInverseInfo);
EXPORT_SYMBOL(MApi_XC_Mux_GetHDMIPort);
EXPORT_SYMBOL(MApi_XC_Mux_GetPortMappingMatrix);
EXPORT_SYMBOL(MApi_XC_MUX_MapInputSourceToVDYMuxPORT);
EXPORT_SYMBOL(MApi_XC_Set_NR);
EXPORT_SYMBOL(MApi_XC_FilmMode_P);
EXPORT_SYMBOL(MApi_XC_GetUCEnabled);
EXPORT_SYMBOL(MApi_XC_GenSpecificTiming);
EXPORT_SYMBOL(MApi_XC_GetDEBypassMode);
EXPORT_SYMBOL(MApi_XC_GetDEWindow);
EXPORT_SYMBOL(MApi_XC_GetDEWidthHeightInDEByPassMode);
EXPORT_SYMBOL(MApi_XC_GetCaptureWindow);
EXPORT_SYMBOL(MApi_XC_SetCaptureWindowVstart);
EXPORT_SYMBOL(MApi_XC_SetCaptureWindowHstart);
EXPORT_SYMBOL(MApi_XC_SetCaptureWindowVsize);
EXPORT_SYMBOL(MApi_XC_SetCaptureWindowHsize);
EXPORT_SYMBOL(MApi_XC_SoftwareReset);
EXPORT_SYMBOL(MApi_XC_CalculateHFreqx10);
EXPORT_SYMBOL(MApi_XC_CalculateHFreqx1K);
EXPORT_SYMBOL(MApi_XC_CalculateVFreqx10);
EXPORT_SYMBOL(MApi_XC_CalculateVFreqx1K);
EXPORT_SYMBOL(MApi_XC_InterruptAttach);
EXPORT_SYMBOL(MApi_XC_InterruptDeAttach);
EXPORT_SYMBOL(MApi_XC_DisableInputSource);
EXPORT_SYMBOL(MApi_XC_IsInputSourceDisabled);
EXPORT_SYMBOL(MApi_XC_ChangePanelType);
EXPORT_SYMBOL(MApi_XC_GetCurrentReadBank);
EXPORT_SYMBOL(MApi_XC_GetCurrentWriteBank);
EXPORT_SYMBOL(MApi_XC_SetAutoPreScaling);
EXPORT_SYMBOL(MApi_XC_GetVSyncWidth);
EXPORT_SYMBOL(MApi_XC_set_GOP_Enable);
EXPORT_SYMBOL(MApi_XC_ip_sel_for_gop);
EXPORT_SYMBOL(MApi_XC_SetVOPNBL);
EXPORT_SYMBOL(MApi_XC_GetDstInfo);
EXPORT_SYMBOL(MApi_XC_set_FD_Mask_byWin);
EXPORT_SYMBOL(MApi_XC_Get_FD_Mask_byWin);
EXPORT_SYMBOL(MApi_XC_Get_FD_Mask_Status);
#if defined(UFO_PUBLIC_HEADER_500_3)
EXPORT_SYMBOL(MApi_XC_SetIP1TestPattern);
#endif
EXPORT_SYMBOL(MApi_XC_InitIPForInternalTiming);
EXPORT_SYMBOL(MApi_XC_SetIPMux);
EXPORT_SYMBOL(MApi_XC_Is_H_Sync_Active);
EXPORT_SYMBOL(MApi_XC_Is_V_Sync_Active);
EXPORT_SYMBOL(MApi_XC_GetAutoPositionWindow);
EXPORT_SYMBOL(MApi_XC_SetFrameBufferAddress);
EXPORT_SYMBOL(MApi_XC_SetFRCMFrameBufferAddress);
EXPORT_SYMBOL(MApi_XC_IsFrameBufferEnoughForCusScaling);
EXPORT_SYMBOL(MApi_XC_SetScalerMemoryRequest);
EXPORT_SYMBOL(MApi_XC_Get_PixelData);
EXPORT_SYMBOL(MApi_XC_GetAvailableSize);
EXPORT_SYMBOL(MApi_XC_SetFrameColor);
EXPORT_SYMBOL(MApi_XC_SetDispWindowColor);
EXPORT_SYMBOL(MApi_XC_SupportSourceToVE);
EXPORT_SYMBOL(MApi_XC_SetOutputCapture);
EXPORT_SYMBOL(MApi_XC_SetGammaOnOff);
EXPORT_SYMBOL(MApi_XC_SetPreGammaGain);
EXPORT_SYMBOL(MApi_XC_SetPreGammaOffset);
EXPORT_SYMBOL(MApi_XC_SetPanelTiming);
EXPORT_SYMBOL(MApi_XC_SetOutTimingMode);
EXPORT_SYMBOL(MApi_XC_SetFreeRunTiming);
EXPORT_SYMBOL(MApi_XC_Set_CustomerSyncInfo);
EXPORT_SYMBOL(MApi_XC_WaitFPLLDone);
EXPORT_SYMBOL(MApi_XC_GetOutputVFreqX100);
EXPORT_SYMBOL(MApi_XC_GetOP1OutputVFreqX100);
EXPORT_SYMBOL(MApi_XC_FrameLockCheck);
EXPORT_SYMBOL(MApi_XC_CustomizeFRCTable);
EXPORT_SYMBOL(MApi_XC_OutputFrameCtrl);
EXPORT_SYMBOL(MApi_SC_Enable_FPLL_FSM);
EXPORT_SYMBOL(MApi_SC_ForceFreerun);
EXPORT_SYMBOL(MApi_SC_IsForceFreerun);
EXPORT_SYMBOL(MApi_SC_SetFreerunVFreq);
EXPORT_SYMBOL(MApi_XC_SetExPanelInfo);
EXPORT_SYMBOL(MApi_XC_Enable_FPLL_Thresh_Mode);
EXPORT_SYMBOL(MApi_XC_Get_FPLL_Thresh_Mode);
EXPORT_SYMBOL(MApi_XC_EnableIPAutoNoSignal);
EXPORT_SYMBOL(MApi_XC_GetIPAutoNoSignal);
EXPORT_SYMBOL(MApi_XC_EnableIPAutoCoast);
EXPORT_SYMBOL(MApi_XC_EnableIPCoastDebounce);
EXPORT_SYMBOL(MApi_XC_ClearIPCoastStatus);
EXPORT_SYMBOL(MApi_XC_EnableFpllManualSetting);
EXPORT_SYMBOL(MApi_XC_FpllBoundaryTest);
EXPORT_SYMBOL(MApi_XC_SetOffLineDetection);
EXPORT_SYMBOL(MApi_XC_GetOffLineDetection);
EXPORT_SYMBOL(MApi_XC_SetOffLineSogThreshold);
EXPORT_SYMBOL(MApi_XC_SetOffLineSogBW);
EXPORT_SYMBOL(MApi_XC_OffLineInit);
EXPORT_SYMBOL(MApi_XC_Set_Extra_fetch_adv_line);
EXPORT_SYMBOL(MApi_XC_SetVGASogEn);
EXPORT_SYMBOL(MApi_XC_EnableWindow);
EXPORT_SYMBOL(MApi_XC_Is_SubWindowEanble);
EXPORT_SYMBOL(MApi_XC_SetBorderFormat);
EXPORT_SYMBOL(MApi_XC_EnableBorder);
EXPORT_SYMBOL(MApi_XC_ZorderMainWindowFirst);
EXPORT_SYMBOL(MApi_XC_PQ_LoadFunction);
EXPORT_SYMBOL(MApi_XC_Check_HNonLinearScaling);
EXPORT_SYMBOL(MApi_XC_EnableEuroHdtvSupport);
EXPORT_SYMBOL(MApi_XC_EnableEuroHdtvDetection);
EXPORT_SYMBOL(MApi_XC_ReadByte);
EXPORT_SYMBOL(MApi_XC_WriteByte);
EXPORT_SYMBOL(MApi_XC_WriteByteMask);
EXPORT_SYMBOL(MApi_XC_Write2ByteMask);
EXPORT_SYMBOL(MApi_XC_W2BYTE);
EXPORT_SYMBOL(MApi_XC_R2BYTE);
EXPORT_SYMBOL(MApi_XC_W4BYTE);
EXPORT_SYMBOL(MApi_XC_R4BYTE);
EXPORT_SYMBOL(MApi_XC_R2BYTEMSK);
EXPORT_SYMBOL(MApi_XC_W2BYTEMSK);
EXPORT_SYMBOL(MApi_XC_MLoad_Init);
EXPORT_SYMBOL(MApi_XC_MLoad_Enable);
EXPORT_SYMBOL(MApi_XC_MLoad_Cus_Init);
EXPORT_SYMBOL(MApi_XC_MLoad_Cus_Enable);
EXPORT_SYMBOL(MApi_XC_MLoad_GetStatus);
EXPORT_SYMBOL(MApi_XC_MLoad_WriteCmd_And_Fire);
EXPORT_SYMBOL(MApi_XC_MLoad_WriteCmds_And_Fire);
EXPORT_SYMBOL(MApi_XC_MLG_Init);
EXPORT_SYMBOL(MApi_XC_MLG_Enable);
EXPORT_SYMBOL(MApi_XC_MLG_GetCaps);
EXPORT_SYMBOL(MApi_XC_MLG_GetStatus);
EXPORT_SYMBOL(MApi_XC_SetOSD2VEMode);
EXPORT_SYMBOL(MApi_XC_IP2_PreFilter_Enable);
EXPORT_SYMBOL(MApi_XC_Get_Pixel_RGB);
EXPORT_SYMBOL(MApi_XC_KeepPixelPointerAppear);
EXPORT_SYMBOL(MApi_XC_Set_MemFmtEx);
EXPORT_SYMBOL(MApi_XC_IsRequestFrameBufferLessMode);
EXPORT_SYMBOL(MApi_XC_SkipSWReset);
EXPORT_SYMBOL(MApi_XC_EnableRepWindowForFrameColor);
EXPORT_SYMBOL(MApi_XC_SetOSDLayer);
EXPORT_SYMBOL(MApi_XC_GetOSDLayer);
EXPORT_SYMBOL(MApi_XC_SetVideoAlpha);
EXPORT_SYMBOL(MApi_XC_GetVideoAlpha);
EXPORT_SYMBOL(MApi_XC_SkipWaitVsync);
EXPORT_SYMBOL(MApi_XC_SetCMAHeapID);
EXPORT_SYMBOL(MApi_XC_OP2VOPDESel);
EXPORT_SYMBOL(MApi_XC_FRC_SetWindow);
#ifdef SUPPORT_BWD
#endif
EXPORT_SYMBOL(MApi_XC_ConfigCMA);
EXPORT_SYMBOL(MApi_XC_Enable_TwoInitFactor);
EXPORT_SYMBOL(MApi_XC_IsFieldPackingModeSupported);
EXPORT_SYMBOL(MApi_XC_PreInit);
EXPORT_SYMBOL(MApi_XC_Get_BufferData);
EXPORT_SYMBOL(MApi_XC_Set_BufferData);
EXPORT_SYMBOL(MApi_XC_EnableMainWindow);
EXPORT_SYMBOL(MApi_XC_EnableSubWindow);
EXPORT_SYMBOL(MApi_XC_DisableSubWindow);
EXPORT_SYMBOL(MApi_XC_SetPixelShift);
EXPORT_SYMBOL(MApi_XC_SetPixelShiftFeatures);
EXPORT_SYMBOL(MApi_XC_Combine_MLoadEn);
EXPORT_SYMBOL(MApi_XC_SetVideoOnOSD);
EXPORT_SYMBOL(MApi_XC_SetOSDLayerBlending);
EXPORT_SYMBOL(MApi_XC_SetOSDLayerAlpha);
EXPORT_SYMBOL(MApi_XC_SetOSDBlendingFormula);
EXPORT_SYMBOL(MApi_XC_ReportPixelInfo);
EXPORT_SYMBOL(MApi_XC_SetScaling);
EXPORT_SYMBOL(MApi_XC_SetMCDIBufferAddress);
EXPORT_SYMBOL(MApi_XC_EnableMCDI);
EXPORT_SYMBOL(MApi_XC_SendCmdToFRC);
EXPORT_SYMBOL(MApi_XC_GetMsgFromFRC);
EXPORT_SYMBOL(MApi_XC_EnableRWBankAuto);
EXPORT_SYMBOL(MApi_XC_Dump_SHM);
EXPORT_SYMBOL(MApi_XC_SetWRBankMappingNum);
EXPORT_SYMBOL(MApi_XC_GetWRBankMappingNum);
EXPORT_SYMBOL(MApi_XC_GetWRBankMappingNumForZap);
EXPORT_SYMBOL(MApi_XC_SetBOBMode);
EXPORT_SYMBOL(MApi_XC_SetForceReadBank);
EXPORT_SYMBOL(MApi_XC_SetLockPoint);
EXPORT_SYMBOL(MApi_XC_LD_Init);
EXPORT_SYMBOL(MApi_XC_LD_SetMemoryAddress);
EXPORT_SYMBOL(MApi_XC_LD_Get_Value);
EXPORT_SYMBOL(MApi_XC_LD_SetLevel);
EXPORT_SYMBOL(MApi_Set_TurnoffLDBL);
EXPORT_SYMBOL(MApi_Set_notUpdateSPIDataFlags);
EXPORT_SYMBOL(MApi_Set_UsermodeLDFlags);
EXPORT_SYMBOL(MApi_Set_BLLevel);
EXPORT_SYMBOL(MApi_XC_Set_BWS_Mode);
EXPORT_SYMBOL(MApi_XC_FRC_ColorPathCtrl);
EXPORT_SYMBOL(MApi_XC_FRC_OP2_SetRGBGain);
EXPORT_SYMBOL(MApi_XC_FRC_OP2_SetRGBOffset);
EXPORT_SYMBOL(MApi_XC_FRC_OP2_SetDither);
EXPORT_SYMBOL(MApi_XC_FRC_BypassMFC);
EXPORT_SYMBOL(MApi_XC_FRC_Mute);
EXPORT_SYMBOL(MApi_XC_ForceReadFrame);
EXPORT_SYMBOL(MApi_XC_SetCsc);
EXPORT_SYMBOL(MApi_XC_RegisterPQSetFPLLThreshMode);
EXPORT_SYMBOL(MApi_XC_GetFreeRunStatus);
EXPORT_SYMBOL(MApi_XC_Get_DSForceIndexSupported);
EXPORT_SYMBOL(MApi_XC_Set_DSForceIndex);
EXPORT_SYMBOL(MApi_XC_Set_DSIndexSourceSelect);
EXPORT_SYMBOL(MApi_XC_OSDC_InitSetting);
EXPORT_SYMBOL(MApi_XC_OSDC_Control);
EXPORT_SYMBOL(MApi_XC_OSDC_GetDstInfo);
EXPORT_SYMBOL(MApi_XC_OSDC_SetOutVfreqx10);
EXPORT_SYMBOL(MApi_XC_BYPASS_SetOSDVsyncPos);
EXPORT_SYMBOL(MApi_XC_BYPASS_SetInputSrc);
EXPORT_SYMBOL(MApi_XC_BYPASS_SetCSC);
EXPORT_SYMBOL(MApi_XC_SetSeamlessZapping);
EXPORT_SYMBOL(MApi_XC_GetSeamlessZappingStatus);
EXPORT_SYMBOL(MApi_XC_Vtrack_SetPayloadData);
EXPORT_SYMBOL(MApi_XC_Vtrack_SetUserDefindedSetting);
EXPORT_SYMBOL(MApi_XC_Vtrack_Enable);
EXPORT_SYMBOL(MApi_XC_PreSetPQInfo);
EXPORT_SYMBOL(MApi_XC_Is_OP1_TestPattern_Enabled);
EXPORT_SYMBOL(MApi_XC_Set_OP1_TestPattern);
EXPORT_SYMBOL(MApi_XC_CheckWhiteBalancePatternModeSupport);
EXPORT_SYMBOL(MApi_XC_SetHLinearScaling);
EXPORT_SYMBOL(MApi_XC_SetPowerState);
EXPORT_SYMBOL(MApi_XC_EnableT3D);
EXPORT_SYMBOL(MApi_XC_Set_FRC_InputTiming);
EXPORT_SYMBOL(MApi_XC_Get_FRC_InputTiming);
EXPORT_SYMBOL(MApi_XC_Get_VirtualBox_Info);
EXPORT_SYMBOL(MApi_XC_Set_OSD_Detect);
EXPORT_SYMBOL(MApi_XC_Get_OSD_Detect);
EXPORT_SYMBOL(MApi_XC_Is2K2KToFrcMode);
EXPORT_SYMBOL(MApi_XC_Enable_LockFreqOnly);
#ifdef UFO_XC_TEST_PATTERN
#endif
EXPORT_SYMBOL(MApi_XC_Get_Current_OutputVFreqX100);
EXPORT_SYMBOL(MApi_XC_SetMemoryWriteRequest);
EXPORT_SYMBOL(MApi_XC_Set_MemFmt);
EXPORT_SYMBOL(MApi_XC_SetOutputAdjustSetting);
EXPORT_SYMBOL(MApi_XC_EnableMirrorMode);
EXPORT_SYMBOL(MApi_XC_EnableMirrorMode2);
EXPORT_SYMBOL(MApi_XC_GetMirrorModeType);
EXPORT_SYMBOL(MApi_XC_set_FD_Mask);
EXPORT_SYMBOL(MApi_XC_Get_FD_Mask);
EXPORT_SYMBOL(MApi_XC_GetAccurateVFreqx1K);
#ifdef UFO_XC_SETBLACKVIDEOBYMODE
EXPORT_SYMBOL(MApi_XC_GenerateBlackVideoByMode);
#endif
EXPORT_SYMBOL(MApi_XC_IsSupport2StepScaling);
#ifdef UFO_XC_PQ_PATH
#endif
EXPORT_SYMBOL(MApi_XC_SetIP1TestPattern);
EXPORT_SYMBOL(MApi_XC_Set_OP2_Pattern);
#ifdef UFO_XC_FORCEWRITE_V2
EXPORT_SYMBOL(MApi_XC_SetForceWrite);
EXPORT_SYMBOL(MApi_XC_GetForceWrite);
#endif
EXPORT_SYMBOL(MApi_XC_VideoPlaybackCtrl);
#ifdef UFO_XC_AUTO_DOWNLOAD
EXPORT_SYMBOL(MApi_XC_AutoDownload_Config);
EXPORT_SYMBOL(MApi_XC_AutoDownload_Write);
EXPORT_SYMBOL(MApi_XC_AutoDownload_Fire);
#endif
#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
EXPORT_SYMBOL(MApi_XC_HDR_Control);
#endif
#endif
#ifdef UFO_XC_SCAN_TYPE
#endif
EXPORT_SYMBOL(MApi_SWDS_Fire);
EXPORT_SYMBOL(MApi_XC_GetResourceByPipeID);
EXPORT_SYMBOL(MApi_XC_OPTEE_CheckRBase);
EXPORT_SYMBOL(MApi_XC_ConfigPipe);
EXPORT_SYMBOL(MApi_XC_CheckPipe);
EXPORT_SYMBOL(MApi_XC_OPTEE_Control);
EXPORT_SYMBOL(MApi_XC_OPTEE_Mux);
#ifdef UFO_XC_DE_MODE
#endif
EXPORT_SYMBOL(MApi_XC_SetSWDRInfo);
EXPORT_SYMBOL(MApi_XC_GetSWDRInfo);
#ifdef UFO_XC_SUPPORT_HDMI_DOWNSCALE_OUTPUT_POLICY
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_DVBT_H_
#ifdef __cplusplus
#endif
#if 1
#else
#endif
#if (SUPPORT_MULTI_DEMOD)
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _UFO_H_
#ifdef UFO_XC_HDR
#endif
#endif
#ifdef _UFO_H_
#ifdef UFO_XC_HDR
#endif
#endif
#ifdef _UFO_H_
#ifdef UFO_XC_HDR
#endif
#endif
#ifdef _UFO_H_
#ifdef UFO_XC_HDR
#endif
#endif
#ifdef _UFO_H_
#endif
#ifdef _UFO_H_
#endif
#ifdef _UFO_H_
#ifdef UFO_XC_HDR
#endif
#endif
#ifdef _UFO_H_
#endif
#ifdef _UFO_H_
#endif
#ifdef _UFO_H_
#ifdef UFO_XC_HDR
#endif
#endif
#ifdef _UFO_H_
#ifdef UFO_XC_HDR
#endif
#endif
#ifdef _UFO_H_
#ifdef UFO_XC_HDR
#endif
#endif
#ifdef _UFO_H_
#ifdef STELLAR
#else
#endif
#endif
#ifdef _UFO_H_
#ifdef UFO_XC_HDR
#endif
#endif
#ifdef _UFO_H_
#ifdef UFO_XC_HDR
#endif
#endif
#ifdef _UFO_H_
#endif
#ifdef _UFO_H_
#endif
#ifdef _UFO_H_
#endif
#ifdef _UFO_H_
#ifdef UFO_XC_HDR
#endif
#endif
#ifdef _UFO_H_
#endif
#ifdef _UFO_H_
#endif
#ifdef _UFO_H_
#ifdef STELLAR
#else
#endif
#endif
#ifdef _UFO_H_
#ifdef UFO_XC_HDR
#endif
#endif
#ifdef _UFO_H_
#ifdef UFO_XC_HDR
#endif
#endif
#ifdef _UFO_H_
#endif
#ifdef _UFO_H_
#ifdef UFO_XC_HDR
#endif
#endif
#ifdef _UFO_H_
#ifdef UFO_XC_HDR
#endif
#endif
#ifdef _UFO_H_
#endif
#ifdef _UFO_H_
#endif
#ifdef _UFO_H_
#ifdef UFO_XC_HDR
#endif
#endif
#ifdef _UFO_H_
#endif
#ifdef _UFO_H_
#ifdef STELLAR
#else
#endif
#endif
#ifdef _UFO_H_
#endif
#ifdef _UFO_H_
#ifdef UFO_XC_HDR
#endif
#endif
#ifdef _API_GFX_H_
#ifdef __cplusplus
#endif
#if 0
#endif
EXPORT_SYMBOL(MApi_GFX_Init);
EXPORT_SYMBOL(MApi_GFX_GetConfig);
EXPORT_SYMBOL(MApi_GFX_GetClip);
EXPORT_SYMBOL(MApi_GFX_GetIntensity);
EXPORT_SYMBOL(MApi_GFX_GetTAGID);
EXPORT_SYMBOL(MApi_GFX_GetNextTAGID);
EXPORT_SYMBOL(MApi_GFX_GetInfo);
EXPORT_SYMBOL(MApi_GFX_GetGECaps);
EXPORT_SYMBOL(MApi_GFX_DrawLine);
EXPORT_SYMBOL(MApi_GFX_RectFill);
EXPORT_SYMBOL(MApi_GFX_TriFill);
EXPORT_SYMBOL(MApi_GFX_SpanFill);
EXPORT_SYMBOL(MApi_GFX_SetSrcBufferInfo);
EXPORT_SYMBOL(MApi_GFX_SetDstBufferInfo);
EXPORT_SYMBOL(MApi_GFX_SetROP2);
EXPORT_SYMBOL(MApi_GFX_SetSrcColorKey);
EXPORT_SYMBOL(MApi_GFX_SetDstColorKey);
EXPORT_SYMBOL(MApi_GFX_SetAlpha);
EXPORT_SYMBOL(MApi_GFX_EnableAlphaBlending);
EXPORT_SYMBOL(MApi_GFX_EnableDFBBlending);
EXPORT_SYMBOL(MApi_GFX_SetDFBBldFlags);
EXPORT_SYMBOL(MApi_GFX_SetDFBBldOP);
EXPORT_SYMBOL(MApi_GFX_SetDFBBldConstColor);
EXPORT_SYMBOL(MApi_GFX_BitBlt);
EXPORT_SYMBOL(MApi_GFX_BitBltEx);
EXPORT_SYMBOL(MApi_GFX_SetIntensity);
EXPORT_SYMBOL(MApi_GFX_SetClip);
EXPORT_SYMBOL(MApi_GFX_SetItalic);
EXPORT_SYMBOL(MApi_GFX_SetDither);
EXPORT_SYMBOL(MApi_GFX_SetOnePixelMode);
EXPORT_SYMBOL(MApi_GFX_SetNearestMode);
EXPORT_SYMBOL(MApi_GFX_SetMirror);
EXPORT_SYMBOL(MApi_GFX_SetDstMirror);
EXPORT_SYMBOL(MApi_GFX_SetRotate);
EXPORT_SYMBOL(MApi_GFX_SetNearestMode);
EXPORT_SYMBOL(MApi_GFX_SetTAGID);
EXPORT_SYMBOL(MApi_GFX_WaitForTAGID);
EXPORT_SYMBOL(MApi_GFX_SetNextTAGID);
EXPORT_SYMBOL(MApi_GFX_EnableVCmdQueue);
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MApi_GFX_SetVCmdBuffer);
#else
EXPORT_SYMBOL(MApi_GFX_SetVCmdBuffer);
#endif
EXPORT_SYMBOL(MApi_GE_SetVCmd_W_Thread);
EXPORT_SYMBOL(MApi_GE_SetVCmd_R_Thread);
EXPORT_SYMBOL(MApi_GFX_FlushQueue);
EXPORT_SYMBOL(MApi_GFX_PowerOn);
EXPORT_SYMBOL(MApi_GFX_PowerOff);
EXPORT_SYMBOL(MApi_GFX_SetPowerState);
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MApi_GFX_WriteProtect);
#else
EXPORT_SYMBOL(MApi_GFX_WriteProtect);
#endif
EXPORT_SYMBOL(MApi_GFX_SetPaletteOpt);
EXPORT_SYMBOL(MApi_GFX_DrawBitmap);
EXPORT_SYMBOL(MApi_GFX_TextOut);
EXPORT_SYMBOL(MApi_GFX_CharacterOut);
EXPORT_SYMBOL(MApi_GFX_TrapezoidFill);
EXPORT_SYMBOL(MApi_GFX_EnableTrapezoidAA);
EXPORT_SYMBOL(MApi_GFX_EnableTrapSubPixCorr);
EXPORT_SYMBOL(MApi_GFX_QueryTextDispLength);
EXPORT_SYMBOL(MApi_GFX_SetAlphaSrcFrom);
EXPORT_SYMBOL(MApi_GFX_SetAlphaBlending);
EXPORT_SYMBOL(MApi_GFX_QueryDFBBldCaps);
EXPORT_SYMBOL(MApi_GFX_SetDFBBldSrcColorMask);
EXPORT_SYMBOL(MApi_GFX_Line_Pattern_Reset);
EXPORT_SYMBOL(MApi_GFX_Set_Line_Pattern);
EXPORT_SYMBOL(MApi_GFX_BeginDraw);
EXPORT_SYMBOL(MApi_GFX_EndDraw);
EXPORT_SYMBOL(MApi_GFX_RectFrame);
EXPORT_SYMBOL(MApi_GFX_SetDC_CSC_FMT);
EXPORT_SYMBOL(MApi_GFX_SetPatchMode);
EXPORT_SYMBOL(MApi_GFX_GetBufferInfo);
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MApi_GFX_ClearFrameBufferByWord);
#else
EXPORT_SYMBOL(MApi_GFX_ClearFrameBufferByWord);
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MApi_GFX_ClearFrameBuffer);
#else
EXPORT_SYMBOL(MApi_GFX_ClearFrameBuffer);
#endif
EXPORT_SYMBOL(MApi_GFX_SetAlpha_ARGB1555);
EXPORT_SYMBOL(MApi_GFX_GetAlpha_ARGB1555);
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MApi_GFX_RegisterGetFontCB);
#else
EXPORT_SYMBOL(MApi_GFX_RegisterGetFontCB);
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MApi_GFX_RegisterGetBMPCB);
#else
EXPORT_SYMBOL(MApi_GFX_RegisterGetBMPCB);
#endif
EXPORT_SYMBOL(MApi_GFX_SetAlphaCmp);
EXPORT_SYMBOL(MApi_GFX_SetDbgLevel);
EXPORT_SYMBOL(MApi_GFX_SetStrBltSckType);
EXPORT_SYMBOL(MApi_GFX_SetHK);
EXPORT_SYMBOL(MApi_GFX_GetHK);
EXPORT_SYMBOL(MApi_GFX_DrawOval);
EXPORT_SYMBOL(MApi_GFX_GetStatus);
EXPORT_SYMBOL(MApi_GFX_GetLibVer);
EXPORT_SYMBOL(MApi_GFX_GetAlignCaps);
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
EXPORT_SYMBOL(MDrv_GE_EnableAlphaBlending);
EXPORT_SYMBOL(MDrv_GE_SetAlphaBlending);
EXPORT_SYMBOL(MDrv_GE_PE_SetIntensity);
EXPORT_SYMBOL(MApi_GFX_SetTLBMode);
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MApi_GFX_SetTLBBaseADDR);
#else
EXPORT_SYMBOL(MApi_GFX_SetTLBBaseADDR);
#endif
EXPORT_SYMBOL(MApi_GFX_SetTLBFlushTable);
EXPORT_SYMBOL(MApi_GFX_SetTLBTag);
EXPORT_SYMBOL(MApi_GFX_BitbltByTwoSourceBuffer);
EXPORT_SYMBOL(MApi_GE_Exit);
#ifdef __cplusplus
#endif
#endif
#ifdef _MS_TYPES_H_
#ifdef CONFIG_ENABLE_MENUCONFIG
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#elif defined(UFO_PUBLIC_HEADER_212)
#else
#if defined (__aarch64__)
#elif defined(MSOS_TYPE_NUTTX)
#else
#if (defined(CONFIG_PURE_SN) || defined(CONFIG_MBOOT))
#else
#endif
#endif
#endif
#ifdef NULL
#endif
#ifndef MSOS_TYPE_LINUX_KERNEL
#ifndef true
#endif
#endif
#if !defined(TRUE) && !defined(FALSE)
#endif
#if defined(ENABLE) && (ENABLE!=1)
#else
#endif
#if defined(DISABLE) && (DISABLE!=0)
#else
#endif
#endif
#ifdef _API_XC_PC_MONITOR_EX_H_
#ifdef __cplusplus
#endif
EXPORT_SYMBOL(MApi_XC_PCMonitor_EX_Init);
EXPORT_SYMBOL(MApi_XC_PCMonitor_EX_Restart);
EXPORT_SYMBOL(MApi_XC_PCMonitor_EX_SetTimingCount);
EXPORT_SYMBOL(MApi_XC_PCMonitor_EX);
EXPORT_SYMBOL(MApi_XC_PCMonitor_EX_GetCurrentState);
EXPORT_SYMBOL(MApi_XC_PCMonitor_EX_GetSyncStatus);
EXPORT_SYMBOL(MApi_XC_PCMonitor_EX_Get_HFreqx10);
EXPORT_SYMBOL(MApi_XC_PCMonitor_EX_Get_HFreqx1K);
EXPORT_SYMBOL(MApi_XC_PCMonitor_EX_Get_VFreqx10);
EXPORT_SYMBOL(MApi_XC_PCMonitor_EX_Get_VFreqx1K);
EXPORT_SYMBOL(MApi_XC_PCMonitor_EX_Get_Vtotal);
EXPORT_SYMBOL(MApi_XC_PCMonitor_EX_Get_Dvi_Hdmi_De_Info);
EXPORT_SYMBOL(MApi_XC_PCMonitor_EX_SyncLoss);
EXPORT_SYMBOL(MApi_XC_PCMonitor_EX_InvalidTimingDetect);
EXPORT_SYMBOL(MApi_XC_PCMonitor_EX_SetTimingCountEx);
#ifdef __cplusplus
#endif
#endif
#ifdef _API_GOP_H_
#ifdef __cplusplus
#endif
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
#else
#endif
#ifndef MAX_GWIN_FB_SUPPORT
#endif
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
#else
#endif
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
#else
#endif
#if (defined(UFO_PUBLIC_HEADER_500)||defined(UFO_PUBLIC_HEADER_500_3)||defined(UFO_PUBLIC_HEADER_700))
#else
#endif
#if (defined(UFO_PUBLIC_HEADER_500)||defined(UFO_PUBLIC_HEADER_500_3)||defined(UFO_PUBLIC_HEADER_700))
#else
#endif
#if (defined(UFO_PUBLIC_HEADER_500)||defined(UFO_PUBLIC_HEADER_500_3)||defined(UFO_PUBLIC_HEADER_700))
#else
#endif
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
#else
#endif
#if (defined(UFO_PUBLIC_HEADER_500)||defined(UFO_PUBLIC_HEADER_500_3)||defined(UFO_PUBLIC_HEADER_700))
#else
#endif
EXPORT_SYMBOL(MApi_GOP_Init);
EXPORT_SYMBOL(MApi_GOP_GetInfo);
EXPORT_SYMBOL(MApi_GOP_GetStatus);
EXPORT_SYMBOL(MApi_GOP_SetDbgLevel);
EXPORT_SYMBOL(MApi_GOP_GetLibVer);
EXPORT_SYMBOL(MApi_GOP_InitByGOP);
EXPORT_SYMBOL(MApi_GOP_GWIN_SwitchGOP);
EXPORT_SYMBOL(MApi_GOP_Initialize_StretchWindow);
EXPORT_SYMBOL(MApi_GOP_SetGOPClk);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetCurrentGOP);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetMaxGOPNum);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetGwinNum);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetTotalGwinNum);
EXPORT_SYMBOL(MApi_GOP_PowerOn);
EXPORT_SYMBOL(MApi_GOP_PowerOff);
EXPORT_SYMBOL(Mapi_GOP_GWIN_ResetGOP);
EXPORT_SYMBOL(MApi_GOP_GWIN_ResetPool);
EXPORT_SYMBOL(MApi_GOP_SetPowerState);
EXPORT_SYMBOL(MApi_GOP_SetGOPBWStrength);
EXPORT_SYMBOL(MApi_GOP_GetGOPBWStrength);
EXPORT_SYMBOL(MApi_GOP_SetGOPHStart);
EXPORT_SYMBOL(MApi_GOP_SetGOPBrightness);
EXPORT_SYMBOL(MApi_GOP_GetGOPBrightness);
EXPORT_SYMBOL(MApi_GOP_EnableLBCouple);
EXPORT_SYMBOL(MApi_GOP_VE_SetOutputTiming);
EXPORT_SYMBOL(MApi_GOP_Enable_VEOSD);
EXPORT_SYMBOL(MApi_GOP_MIXER_SetOutputTiming);
EXPORT_SYMBOL(MApi_GOP_MIXER_SetMIXER2OPOutputTiming);
EXPORT_SYMBOL(MApi_GOP_MIXER_EnableVfilter);
EXPORT_SYMBOL(MApi_GOP_MIXER_EnableOldBlendMode);
EXPORT_SYMBOL(MApi_GOP_RestoreFromVsyncLimitation);
EXPORT_SYMBOL(MApi_GOP_MIUSel);
EXPORT_SYMBOL(MApi_GOP_SetUVSwap);
EXPORT_SYMBOL(MApi_GOP_SetYCSwap);
EXPORT_SYMBOL(MApi_GOP_GetMIUSel);
EXPORT_SYMBOL(MApi_GOP_SetGOPContrast);
EXPORT_SYMBOL(MApi_GOP_GetGOPContrast);
EXPORT_SYMBOL(MApi_GOP_EnaVECapture);
EXPORT_SYMBOL(MApi_GOP_GetVECaptureState);
EXPORT_SYMBOL(MApi_GOP_VECaptureWaitOnFrame);
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
EXPORT_SYMBOL(MApi_GOP_SetConfig);
#else
EXPORT_SYMBOL(MApi_GOP_SetConfig);
#endif
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
EXPORT_SYMBOL(MApi_GOP_SetConfigEx);
#else
EXPORT_SYMBOL(MApi_GOP_SetConfigEx);
#endif
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
EXPORT_SYMBOL(MApi_GOP_GetConfigEx);
#else
EXPORT_SYMBOL(MApi_GOP_GetConfigEx);
#endif
EXPORT_SYMBOL(MApi_GOP_Set3DOSDMode);
EXPORT_SYMBOL(MApi_GOP_Set3D_LR_FrameExchange);
EXPORT_SYMBOL(MApi_GOP_Set_GPIO3DPin);
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
EXPORT_SYMBOL(MApi_GOP_GetChipCaps);
#else
EXPORT_SYMBOL(MApi_GOP_GetChipCaps);
#endif
EXPORT_SYMBOL(MApi_GOP_TestPattern);
EXPORT_SYMBOL(MApi_GOP_TestPatternAlpha_Enable);
EXPORT_SYMBOL(MApi_GOP_IsRegUpdated);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetLayerFromGOP);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetGOPFromLayer);
EXPORT_SYMBOL(MApi_GOP_GWIN_Enable);
EXPORT_SYMBOL(MApi_GOP_GWIN_EnableTransClr);
EXPORT_SYMBOL(MApi_GOP_GWIN_EnableTransClr_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_EnableProgressive);
EXPORT_SYMBOL(MApi_GOP_GWIN_EnableProgressive_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_DeleteWin);
EXPORT_SYMBOL(MApi_GOP_GWIN_ReleaseWin);
EXPORT_SYMBOL(MApi_GOP_GWIN_DestroyWin);
EXPORT_SYMBOL(MApi_GOP_GWIN_DeleteWinHVSize);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetLayer);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetLayer);
EXPORT_SYMBOL(MApi_GOP_GWIN_OutputColor);
EXPORT_SYMBOL(MApi_GOP_GWIN_OutputColor_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_SwapOverlapWin);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetRelativeWinPrio);
EXPORT_SYMBOL(MApi_GOP_GWIN_Switch2Gwin);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetAlphaInverse);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetAlphaInverse_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetAlphaInverse);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetAlphaInverse_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetPalette);
EXPORT_SYMBOL(MApi_GOP_GWIN_EnableMultiAlpha);
EXPORT_SYMBOL(MApi_GOP_GWIN_IsMultiAlphaEnable);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetAlphaValue);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetScrollRate);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetScrollType);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetSWScrollBuffer);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetFMT0TransClr);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetFMT0TransClr_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetTransClr_8888);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetTransClr_8888_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_EnableT3DMode);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetBlending);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetDuplication);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetWinPosition);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetWinPositionOffset);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetWinProperty);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetGOPDst);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetBlink);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetFieldInver);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetFieldInver_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_Set_STRETCHWIN);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetStretchWinPosition);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetHDisplaySize);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetWinInfo);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetTransClr);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetTransparentClr);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetTransClr_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetWinDispPosition);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetHMirror);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetHMirror_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetVMirror);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetVMirror_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_IsMirrorOn);
EXPORT_SYMBOL(MApi_GOP_GWIN_IsMirrorOn_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_Set_HSCALE);
EXPORT_SYMBOL(MApi_GOP_GWIN_Set_HSCALE_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_Set_VSCALE);
EXPORT_SYMBOL(MApi_GOP_GWIN_Set_VSCALE_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_Set_HStretchMode);
EXPORT_SYMBOL(MApi_GOP_GWIN_Set_HStretchMode_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_Set_VStretchMode);
EXPORT_SYMBOL(MApi_GOP_GWIN_Set_VStretchMode_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_Set_TranspColorStretchMode);
EXPORT_SYMBOL(MApi_GOP_GWIN_Set_TranspColorStretchMode_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetForceWrite);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetBnkForceWrite);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetAlphaValue);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetWinPosition);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetWinProperty);
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
EXPORT_SYMBOL(MApi_GOP_GWIN_GetWinAttr);
#else
EXPORT_SYMBOL(MApi_GOP_GWIN_GetWinAttr);
#endif
EXPORT_SYMBOL(MApi_GOP_GWIN_GetWinInfo);
EXPORT_SYMBOL(MApi_GOP_GWIN_UpdateRegOnceEx);
EXPORT_SYMBOL(MApi_GOP_GWIN_UpdateRegOnceEx2);
EXPORT_SYMBOL(MApi_GOP_GWIN_UpdateRegOnceByMask);
EXPORT_SYMBOL(MApi_GOP_TriggerRegWriteIn);
EXPORT_SYMBOL(MApi_GOP_TriggerRegWriteIn_Ex);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetFadeInOut);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetGWinShared);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetGWinSharedCnt);
EXPORT_SYMBOL(MApi_GOP_GWIN_EnableTileMode);
EXPORT_SYMBOL(MApi_GOP_SetPINPON);
EXPORT_SYMBOL(MApi_GOP_SetGOPYUV);
EXPORT_SYMBOL(MApi_GOP_MIXER_SetMux);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetNewAlphaMode);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetPreAlphaMode);
EXPORT_SYMBOL(MApi_GOP_GWIN_CreateWin);
EXPORT_SYMBOL(MApi_GOP_GWIN_CreateWin2);
EXPORT_SYMBOL(MApi_GOP_GWIN_CreateWin_Assign_FB);
EXPORT_SYMBOL(MApi_GOP_GWIN_CreateWin_Assign_32FB);
EXPORT_SYMBOL(MApi_GOP_GWIN_CreateStaticWin);
EXPORT_SYMBOL(MApi_GOP_GWIN_CreateStaticWin2);
EXPORT_SYMBOL(MApi_GOP_GWIN_CreateStaticWin_Assign_FB);
EXPORT_SYMBOL(MApi_GOP_GWIN_CreateStaticWin_Assign_32FB);
EXPORT_SYMBOL(MApi_GOP_GWIN_IsGwinExist);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetFreeWinID);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetMAXWinID);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetCurrentWinId);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetMaxActiveGwin_v);
EXPORT_SYMBOL(MApi_GOP_GWIN_Get_HSTART);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetActiveGWIN);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetMaxVEnd);
EXPORT_SYMBOL(MApi_GOP_GWIN_IsAllGWINDisabled);
EXPORT_SYMBOL(MApi_GOP_GWIN_IsGWINEnabled);
EXPORT_SYMBOL(MApi_GOP_GWIN_IsEnabled);
EXPORT_SYMBOL(MApi_GOP_GWIN_CheckOpmodeIsOn);
EXPORT_SYMBOL(MApi_GOP_GWIN_CheckOpmodeIsOn_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetResolution);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetResolution_32FB);
EXPORT_SYMBOL(MApi_GOP_Exit);
EXPORT_SYMBOL(MApi_GOP_FB_SEL);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetDoubleHeap);
EXPORT_SYMBOL(MApi_GOP_GWIN_MapFB2Win);
EXPORT_SYMBOL(MApi_GOP_GWIN_Map32FB2Win);
EXPORT_SYMBOL(MApi_GOP_GWIN_Switch2FB);
EXPORT_SYMBOL(MApi_GOP_GWIN_Switch2_32FB);
EXPORT_SYMBOL(MApi_GOP_Switch_GWIN_2_FB);
EXPORT_SYMBOL(MApi_GOP_Switch_GWIN_2_32FB);
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
EXPORT_SYMBOL(MApi_GOP_Switch_GWIN_2_FB_BY_ADDR);
#else
EXPORT_SYMBOL(MApi_GOP_Switch_GWIN_2_FB_BY_ADDR);
#endif
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
EXPORT_SYMBOL(MApi_GOP_Switch_3DGWIN_2_FB_BY_ADDR);
#else
EXPORT_SYMBOL(MApi_GOP_Switch_3DGWIN_2_FB_BY_ADDR);
#endif
EXPORT_SYMBOL(MApi_GOP_Switch_Multi_GWIN_2_FB_BY_ADDR);
EXPORT_SYMBOL(MApi_GOP_GWIN_DeleteFB);
EXPORT_SYMBOL(MApi_GOP_GWIN_Delete32FB);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetFBRegion);
EXPORT_SYMBOL(MApi_GOP_GWIN_Get32FBRegion);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetFBPosition);
EXPORT_SYMBOL(MApi_GOP_GWIN_Get32FBPosition);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetFBInfo);
EXPORT_SYMBOL(MApi_GOP_GWIN_Set32FBInfo);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetFBInfo);
EXPORT_SYMBOL(MApi_GOP_GWIN_Get32FBInfo);
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
EXPORT_SYMBOL(MApi_GOP_GWIN_GetFBAddr);
#else
EXPORT_SYMBOL(MApi_GOP_GWIN_GetFBAddr);
#endif
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
EXPORT_SYMBOL(MApi_GOP_GWIN_Get32FBAddr);
#else
EXPORT_SYMBOL(MApi_GOP_GWIN_Get32FBAddr);
#endif
EXPORT_SYMBOL(MApi_GOP_GWIN_ClearFlipQueue);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetMaxFBNum);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetMax32FBNum);
EXPORT_SYMBOL(MApi_GOP_FB_Get);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetMAXFBID);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetMAX32FBID);
EXPORT_SYMBOL(MApi_GOP_GWIN_DestroyFB);
EXPORT_SYMBOL(MApi_GOP_GWIN_Destroy32FB);
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
EXPORT_SYMBOL(MApi_GOP_GWIN_CreateFBFrom3rdSurf);
#else
EXPORT_SYMBOL(MApi_GOP_GWIN_CreateFBFrom3rdSurf);
#endif
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
EXPORT_SYMBOL(MApi_GOP_GWIN_Create32FBFrom3rdSurf);
#else
EXPORT_SYMBOL(MApi_GOP_GWIN_Create32FBFrom3rdSurf);
#endif
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
EXPORT_SYMBOL(MApi_GOP_GWIN_CreateFBbyStaticAddr);
#else
EXPORT_SYMBOL(MApi_GOP_GWIN_CreateFBbyStaticAddr);
#endif
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
EXPORT_SYMBOL(MApi_GOP_GWIN_Create32FBbyStaticAddr);
#else
EXPORT_SYMBOL(MApi_GOP_GWIN_Create32FBbyStaticAddr);
#endif
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
EXPORT_SYMBOL(MApi_GOP_GWIN_CreateFBbyStaticAddr2);
#else
EXPORT_SYMBOL(MApi_GOP_GWIN_CreateFBbyStaticAddr2);
#endif
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
EXPORT_SYMBOL(MApi_GOP_GWIN_Create32FBbyStaticAddr2);
#else
EXPORT_SYMBOL(MApi_GOP_GWIN_Create32FBbyStaticAddr2);
#endif
EXPORT_SYMBOL(MApi_GOP_GWIN_CreateFB2);
EXPORT_SYMBOL(MApi_GOP_GWIN_Create32FB2);
EXPORT_SYMBOL(MApi_GOP_GWIN_CreateFB2_EXT);
EXPORT_SYMBOL(MApi_GOP_GWIN_Create32FB2_EXT);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetCurrentFBID);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetCurrent32FBID);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetFBfromGWIN);
EXPORT_SYMBOL(MApi_GOP_GWIN_Get32FBfromGWIN);
EXPORT_SYMBOL(MApi_GOP_GWIN_IsFBExist);
EXPORT_SYMBOL(MApi_GOP_GWIN_Is32FBExist);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetFreeFBID);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetFree32FBID);
EXPORT_SYMBOL(MApi_GOP_GWIN_CreateFB);
EXPORT_SYMBOL(MApi_GOP_GWIN_Create32FB);
EXPORT_SYMBOL(MApi_GOP_GWIN_GetFBFmt);
EXPORT_SYMBOL(MApi_GOP_GWIN_Get32FBFmt);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetPaletteOpt);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetPaletteOpt_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetPaletteRead);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetPaletteRead_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_ReadPalette);
EXPORT_SYMBOL(MApi_GOP_GWIN_ReadPalette_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_2GSetPaletteOpt);
EXPORT_SYMBOL(MApi_GOP_GWIN_2GSetPaletteOpt_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_2GSetPaletteRead);
EXPORT_SYMBOL(MApi_GOP_GWIN_2GSetPaletteRead_EX);
EXPORT_SYMBOL(MApi_GOP_GWIN_2GReadPalette);
EXPORT_SYMBOL(MApi_GOP_GWIN_2GReadPalette_EX);
EXPORT_SYMBOL(MApi_GOP_DWIN_Init);
EXPORT_SYMBOL(MApi_GOP_DWIN_SetSourceSel);
EXPORT_SYMBOL(MApi_GOP_DWIN_CaptureOneFrame2);
EXPORT_SYMBOL(MApi_GOP_DWIN_EnableIntr);
EXPORT_SYMBOL(MApi_GOP_DWIN_CaptureOneFrame);
EXPORT_SYMBOL(MApi_GOP_DWIN_Enable);
EXPORT_SYMBOL(MApi_GOP_DWIN_GetWinProperty);
EXPORT_SYMBOL(MApi_GOP_DWIN_SetWinProperty);
EXPORT_SYMBOL(MApi_GOP_DWIN_SelectSourceScanType);
EXPORT_SYMBOL(MApi_GOP_DWIN_SetDataFmt);
EXPORT_SYMBOL(MApi_GOP_DWIN_SetAlphaValue);
EXPORT_SYMBOL(MApi_GOP_DWIN_SetAlphaSrc);
EXPORT_SYMBOL(MApi_GOP_DWIN_SetAlphaInverse);
EXPORT_SYMBOL(MApi_GOP_DWIN_SetUVSample);
EXPORT_SYMBOL(MApi_GOP_DWIN_SetSkipFrame);
EXPORT_SYMBOL(MApi_GOP_DWIN_ClearIntr);
EXPORT_SYMBOL(MApi_GOP_DWIN_GetDWinIntInfo);
EXPORT_SYMBOL(MApi_GOP_DWIN_GetDWinIntInfoTimeout);
EXPORT_SYMBOL(MApi_GOP_SetClkForCapture);
EXPORT_SYMBOL(MApi_GOP_DWIN_GetIntrStatus);
EXPORT_SYMBOL(MApi_GOP_DWIN_EnableR2YCSC);
EXPORT_SYMBOL(MApi_GOP_DWIN_SetUVSwap);
EXPORT_SYMBOL(MApi_GOP_GWIN_BeginDraw);
EXPORT_SYMBOL(MApi_GOP_GWIN_EndDraw);
EXPORT_SYMBOL(MApi_GOP_GWIN_Enable_BGWIN);
EXPORT_SYMBOL(MApi_GOP_GWIN_Set_BGWIN);
EXPORT_SYMBOL(MApi_GOP_GWIN_Set_BGWIN_Alpha);
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
EXPORT_SYMBOL(MApi_GOP_DWIN_SetBufferPINPON);
#else
EXPORT_SYMBOL(MApi_GOP_DWIN_SetBufferPINPON);
#endif
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
EXPORT_SYMBOL(MApi_GOP_RegisterFBFmtCB);
#else
EXPORT_SYMBOL(MApi_GOP_RegisterFBFmtCB);
#endif
EXPORT_SYMBOL(MApi_GOP_RegisterXCIsInterlaceCB);
EXPORT_SYMBOL(MApi_GOP_RegisterXCGetCapHStartCB);
EXPORT_SYMBOL(MApi_GOP_RegisterXCReduceBWForOSDCB);
EXPORT_SYMBOL(MApi_GOP_RegisterEventNotify);
EXPORT_SYMBOL(MApi_GOP_RegisterXCSetDwinInfo);
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
EXPORT_SYMBOL(MApi_GOP_OC_SetOCFBinfo);
#else
EXPORT_SYMBOL(MApi_GOP_OC_SetOCFBinfo);
#endif
EXPORT_SYMBOL(MApi_GOP_GWIN_SetGPUTile);
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
EXPORT_SYMBOL(MApi_GOP_TLB_Enable);
#else
EXPORT_SYMBOL(MApi_GOP_TLB_Enable);
#endif
EXPORT_SYMBOL(MApi_GOP_GWIN_UpdateRegOnce);
EXPORT_SYMBOL(MApi_GOP_GWIN_SetMux);
EXPORT_SYMBOL(MApi_GOP_GWIN_OutputLayerSwitch);
#ifdef __cplusplus
#endif
#endif
#ifdef __DRV_SMBX_H__
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_CUS_SAR_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#if defined (MSOS_TYPE_LINUX)
#ifdef FRHSL_H
#ifdef __cplusplus
#endif
#if 1
#endif
#ifdef __cplusplus
#endif
#endif
#endif
#ifdef _DRVCMAPOOL_H_
#ifdef __cplusplus
#endif
#if 1
#endif
#if defined(CHIP_MONET)
#else
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef APIXC_DWIN_EX_H
#ifdef __cplusplus
#endif
#ifdef APIXC_DWIN_EX_C
#else
#endif
EXPORT_SYMBOL(MApi_XC_DIP_EX_InitByDIP);
EXPORT_SYMBOL(MApi_XC_DIP_EX_SetFrameBufferAddress);
EXPORT_SYMBOL(MApi_XC_DIP_EX_SetInputSource);
EXPORT_SYMBOL(MApi_XC_DIP_EX_GetInputSource);
EXPORT_SYMBOL(MApi_XC_DIP_EX_DisableInputSource);
EXPORT_SYMBOL(MApi_XC_DIP_EX_SetWindow);
EXPORT_SYMBOL(MApi_XC_DIP_EX_GetDEWindow);
EXPORT_SYMBOL(MApi_XC_DIP_EX_SetCaptureWindowVstart);
EXPORT_SYMBOL(MApi_XC_DIP_EX_SetCaptureWindowHstart);
EXPORT_SYMBOL(MApi_XC_DIP_EX_SetCaptureWindowVsize);
EXPORT_SYMBOL(MApi_XC_DIP_EX_SetCaptureWindowHsize);
EXPORT_SYMBOL(MApi_XC_DIP_EX_GetCaptureWindow);
EXPORT_SYMBOL(MApi_XC_DIP_EX_EnableR2Y);
EXPORT_SYMBOL(MApi_XC_DIP_EX_SWReset);
EXPORT_SYMBOL(MApi_XC_DIP_EX_FrameRateCtrl);
EXPORT_SYMBOL(MApi_XC_DIP_EX_CapOneFrame);
EXPORT_SYMBOL(MApi_XC_DIP_EX_CapOneFrameFast);
EXPORT_SYMBOL(MApi_XC_DIP_EX_SetPinpon);
EXPORT_SYMBOL(MApi_XC_DIP_EX_ClearInt);
EXPORT_SYMBOL(MApi_XC_DIP_EX_Ena);
EXPORT_SYMBOL(MApi_XC_DIP_EX_EnaInt);
EXPORT_SYMBOL(MApi_XC_DIP_EX_SetSourceScanType);
EXPORT_SYMBOL(MApi_XC_DIP_EX_EnaInterlaceWrite);
EXPORT_SYMBOL(MApi_XC_DIP_EX_SetOutputDataFmt);
EXPORT_SYMBOL(MApi_XC_DIP_EX_SetDIPWinProperty);
EXPORT_SYMBOL(MApi_XC_DIP_EX_EnableY2R);
EXPORT_SYMBOL(MApi_XC_DIP_EX_SetAlpha);
EXPORT_SYMBOL(MApi_XC_DIP_EX_SwapUV);
EXPORT_SYMBOL(MApi_XC_DIP_EX_SwapYC);
EXPORT_SYMBOL(MApi_XC_DIP_EX_SwapRGB);
EXPORT_SYMBOL(MApi_XC_DIP_EX_SetWinProperty_Ex);
EXPORT_SYMBOL(MApi_XC_DIP_EX_SetOutputCapture);
EXPORT_SYMBOL(MApi_XC_DIP_EX_SetMirror);
EXPORT_SYMBOL(MApi_XC_DIP_EX_SetDIPRProperty);
EXPORT_SYMBOL(MApi_XC_DIP_EX_InterruptAttach);
EXPORT_SYMBOL(MApi_XC_DIP_EX_InterruptDetach);
EXPORT_SYMBOL(MApi_XC_DIP_EX_GetIntStatus);
EXPORT_SYMBOL(MApi_XC_DIP_EX_GetBufInfo);
#ifdef __cplusplus
#endif
#endif
#ifdef _ASM_CPU_H_
#ifdef __cplusplus
#endif
#if defined(__MIPS16E__)
#else
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_XC_VMARK_EX_H_
#ifdef __cplusplus
#endif
#ifdef UFO_XC_VMARK
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_PVR_IframeLUT_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_DMS_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_DTMB_H_
#if !defined UTPA2
#else
#endif
#if DMD_DTMB_UTOPIA_EN || DMD_DTMB_UTOPIA2_EN
#ifndef MSIF_TAG
#endif
#endif
#if DMD_DTMB_UTOPIA2_EN || DMD_DTMB_STR_EN
#endif
#ifndef DLL_PUBLIC
#endif
#if DMD_DTMB_MULTI_DMD_EN
#else
#endif
#if DMD_DTMB_UTOPIA_EN || DMD_DTMB_UTOPIA2_EN
#endif
#ifndef BIT_
#endif
#ifdef UTPA2
#else
#endif
#ifdef UTPA2
#endif
#if !DMD_DTMB_UTOPIA_EN && !DMD_DTMB_UTOPIA2_EN
#endif
#if DMD_DTMB_STR_EN
#endif
#ifdef __cplusplus
#endif
#ifndef MSOS_TYPE_LINUX_KERNEL
#if DMD_DTMB_STR_EN
#endif
#if DMD_DTMB_STR_EN
#endif
#endif
#ifdef UTPA2
#if DMD_DTMB_STR_EN
#endif
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_MHL_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_SYS_H_
#ifdef __cplusplus
#endif
EXPORT_SYMBOL(MDrv_SYS_Init);
EXPORT_SYMBOL(MDrv_SYS_GlobalInit);
EXPORT_SYMBOL(MDrv_SYS_GetInfo);
EXPORT_SYMBOL(MDrv_SYS_GetChipRev);
EXPORT_SYMBOL(MDrv_SYS_GetChipID);
EXPORT_SYMBOL(MDrv_SYS_WDTEnable);
EXPORT_SYMBOL(MDrv_SYS_WDTClear);
EXPORT_SYMBOL(MDrv_SYS_WDTLastStatus);
EXPORT_SYMBOL(MDrv_SYS_WDTSetTime);
EXPORT_SYMBOL(MDrv_SYS_ResetChip);
EXPORT_SYMBOL(MDrv_SYS_ResetCPU);
EXPORT_SYMBOL(MDrv_SYS_SetDbgLevel);
EXPORT_SYMBOL(MDrv_SYS_VIFWriteByteByVDMbox);
EXPORT_SYMBOL(MDrv_SYS_VIFWriteByteMaskByVDMbox);
EXPORT_SYMBOL(MDrv_SYS_VIFWriteRegBitByVDMbox);
EXPORT_SYMBOL(MDrv_SYS_VIFReadByteByVDMbox);
EXPORT_SYMBOL(MDrv_SYS_VIFRead2ByteByVDMbox);
EXPORT_SYMBOL(MDrv_SYS_Query);
#ifdef _SYS_DAC_GAIN_CTRL_
#endif
EXPORT_SYMBOL(MDrv_SYS_GetSoftwareVersion);
EXPORT_SYMBOL(MDrv_SYS_SetAGCPadMux);
EXPORT_SYMBOL(MDrv_SYS_SetPadMux);
EXPORT_SYMBOL(MDrv_SYS_PackMode);
EXPORT_SYMBOL(MDrv_SYS_SetPCMCardDetectMode);
EXPORT_SYMBOL(MDrv_SYS_DisableDebugPort);
EXPORT_SYMBOL(MDrv_SYS_EnableDebugPort);
EXPORT_SYMBOL(MDrv_SYS_SetTSOutClockPhase);
EXPORT_SYMBOL(MDrv_SYS_SetTSClockPhase);
EXPORT_SYMBOL(MDrv_SYS_PadMuxTableSuspend);
EXPORT_SYMBOL(MDrv_SYS_PadMuxTableResume);
EXPORT_SYMBOL(MDrv_SYS_SetPowerState);
EXPORT_SYMBOL(MDrv_SYS_QueryDolbyHashInfo);
EXPORT_SYMBOL(MDrv_SYS_GetChipType);
EXPORT_SYMBOL(MDrv_SYS_SetChipType);
EXPORT_SYMBOL(MDrv_SYS_GetDolbyKeyCustomer);
EXPORT_SYMBOL(MDrv_SYS_ReadBrickTerminatorStatus);
EXPORT_SYMBOL(MDrv_SYS_WriteBrickTerminatorStatus);
EXPORT_SYMBOL(MDrv_SYS_GetEfuseDid);
EXPORT_SYMBOL(MDrv_SYS_ReadEfuseHDCPKey);
EXPORT_SYMBOL(MDrv_SYS_GetIpList);
EXPORT_SYMBOL(MDrv_SYS_RegisterIoProc);
EXPORT_SYMBOL(MDrv_SYS_ReadKernelCmdLine);
EXPORT_SYMBOL(MDrv_SYS_SetTEEInfo);
EXPORT_SYMBOL(MDrv_SYS_GetTEEInfo);
#ifdef UFO_SYS_PADMUX
#endif
EXPORT_SYMBOL(MDrv_SYS_GetUtopiaReleaseLabel);
#ifdef CONFIG_MSTAR_DVFS_KERNEL_SUPPORT
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef __API_DMX_TEE_H__
#ifdef __cplusplus
#endif
#ifdef MSOS_TYPE_OPTEE
#endif
#ifdef MSOS_TYPE_NUTTX
#endif
#ifdef MSOS_TYPE_OPTEE
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_HDCP_H_
#endif
#ifdef DRVXC_HDMI_IF_H
#ifdef __cplusplus
#endif
#ifdef MDRV_HDMI_C
#else
#endif
#ifdef DVI_NEW_Patch
#endif
EXPORT_SYMBOL(MDrv_HDMI_Func_Caps);
EXPORT_SYMBOL(MDrv_HDMI_init);
EXPORT_SYMBOL(MDrv_HDMI_Exit);
EXPORT_SYMBOL(MDrv_HDMI_pkt_reset);
EXPORT_SYMBOL(MDrv_HDMI_pullhpd);
EXPORT_SYMBOL(MDrv_HDMI_GC_Info);
EXPORT_SYMBOL(MDrv_HDMI_Packet_Received);
EXPORT_SYMBOL(MDrv_HDMI_Get_ColorFormat);
EXPORT_SYMBOL(MDrv_HDMI_Get_ColorRange);
EXPORT_SYMBOL(MDrv_HDMI_Get_Content_Type);
EXPORT_SYMBOL(MDrv_HDMI_Get_ExtColorimetry);
EXPORT_SYMBOL(MDrv_HDMI_Set_EQ);
EXPORT_SYMBOL(MDrv_HDMI_Set_EQ_To_Port);
EXPORT_SYMBOL(MDrv_HDMI_Audio_MUTE_Enable);
EXPORT_SYMBOL(MDrv_HDMI_Audio_Status_Clear);
EXPORT_SYMBOL(MDrv_HDMI_Get_AspectRatio);
EXPORT_SYMBOL(MDrv_HDMI_Get_ActiveFormat_AspectRatio);
EXPORT_SYMBOL(MDrv_HDMI_Get_AVIInfoFrameVer);
EXPORT_SYMBOL(MDrv_HDMI_err_status_update);
EXPORT_SYMBOL(MDrv_HDMI_Get_PollingStatus);
EXPORT_SYMBOL(MDrv_HDMI_IsHDMI_Mode);
EXPORT_SYMBOL(MDrv_HDMI_Get_MID_info);
EXPORT_SYMBOL(MDrv_HDMI_get_packet_value);
EXPORT_SYMBOL(MDrv_DVI_ChannelPhaseStatus);
EXPORT_SYMBOL(MDrv_DVI_HF_adjust);
EXPORT_SYMBOL(MDrv_DVI_SoftwareReset);
EXPORT_SYMBOL(MDrv_DVI_ClkPullLow);
EXPORT_SYMBOL(MDrv_DVI_SwitchSrc);
EXPORT_SYMBOL(MDrv_HDMI_SetForClockLessThan70Mhz);
EXPORT_SYMBOL(MDrv_HDMI_dvi_adjust);
EXPORT_SYMBOL(MDrv_HDMI_SetUpdatePhaseLineCount);
EXPORT_SYMBOL(MDrv_HDMI_GetTMDSFreq);
EXPORT_SYMBOL(MDrv_HDCP_Enable);
EXPORT_SYMBOL(MDrv_HDMI_SetHdcpEnable);
EXPORT_SYMBOL(MDrv_HDCP_ClearStatus);
EXPORT_SYMBOL(MDrv_HDCP_initproductionkey);
EXPORT_SYMBOL(MDrv_HDCP_GetStatus);
EXPORT_SYMBOL(MDrv_HDCP_Vsync_end_en);
EXPORT_SYMBOL(MDrv_HDMI_audio_output);
EXPORT_SYMBOL(MDrv_HDMI_audio_cp_hdr_info);
EXPORT_SYMBOL(MDrv_HDMI_audio_channel_status);
EXPORT_SYMBOL(MDrv_HDMI_GetLibVer);
EXPORT_SYMBOL(MDrv_HDMI_GetInfo);
EXPORT_SYMBOL(MDrv_HDMI_GetStatus);
EXPORT_SYMBOL(MDrv_HDMI_Get_AVIInfoActiveInfoPresent);
EXPORT_SYMBOL(MDrv_HDMI_READ_DDCRAM);
EXPORT_SYMBOL(MDrv_HDMI_PROG_DDCRAM);
EXPORT_SYMBOL(MDrv_HDMI_Get_Pixel_Repetition);
EXPORT_SYMBOL(MDrv_HDMI_3D_4Kx2K_Process);
EXPORT_SYMBOL(MDrv_HDMI_AVG_ScaleringDown);
EXPORT_SYMBOL(MDrv_HDMI_Check4K2K);
EXPORT_SYMBOL(MDrv_HDMI_Check_Additional_Format);
EXPORT_SYMBOL(MDrv_HDMI_Get_3D_Structure);
EXPORT_SYMBOL(MDrv_HDMI_Get_3D_Ext_Data);
EXPORT_SYMBOL(MDrv_HDMI_Get_3D_Meta_Field);
EXPORT_SYMBOL(MDrv_HDMI_Get_VIC_Code);
EXPORT_SYMBOL(MDrv_HDMI_Get_4Kx2K_VIC_Code);
EXPORT_SYMBOL(MDrv_HDMI_ARC_PINControl);
EXPORT_SYMBOL(MDrv_DVI_Software_Reset);
EXPORT_SYMBOL(MDrv_HDMI_CheckHDMI20_Setting);
EXPORT_SYMBOL(MDrv_HDMI_StablePolling);
EXPORT_SYMBOL(MDrv_HDMI_GetSourceVersion);
EXPORT_SYMBOL(MDrv_HDMI_Set5VDetectGPIOSelect);
EXPORT_SYMBOL(MDrv_HDMI_GetDEStableStatus);
EXPORT_SYMBOL(MDrv_HDMI_CheckHDCP14);
EXPORT_SYMBOL(MDrv_HDMI_CheckHDCPState);
EXPORT_SYMBOL(MDrv_HDMI_Ctrl);
EXPORT_SYMBOL(MDrv_HDMI_GetDataInfo);
EXPORT_SYMBOL(MDrv_HDMI_SetPowerState);
EXPORT_SYMBOL(MDrv_HDCP22_FillCipherKey);
EXPORT_SYMBOL(MDrv_HDCP22_InitCBFunc);
EXPORT_SYMBOL(MDrv_HDCP22_PortInit);
EXPORT_SYMBOL(MDrv_HDCP22_PollingReadDone);
EXPORT_SYMBOL(MDrv_HDCP22_EnableCipher);
EXPORT_SYMBOL(MDrv_HDCP22_SendMsg);
EXPORT_SYMBOL(MDrv_HDCP22_Handler);
#if 0
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRVBDMA_H_
#ifdef __cplusplus
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#ifdef MOBF_ENABLE
#endif
EXPORT_SYMBOL(MDrv_BDMA_GetLibVer);
EXPORT_SYMBOL(MDrv_BDMA_GetStatus);
EXPORT_SYMBOL(MDrv_BDMA_GetInfo);
EXPORT_SYMBOL(MDrv_BDMA_GetMinSize);
EXPORT_SYMBOL(MDrv_BDMA_SetDbgLevel);
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MDrv_BDMA_Init);
#else
EXPORT_SYMBOL(MDrv_BDMA_Init);
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MDrv_BDMA_Search);
#else
EXPORT_SYMBOL(MDrv_BDMA_Search);
#endif
#ifdef MOBF_ENABLE
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MDrv_BDMA_CRC32);
#else
EXPORT_SYMBOL(MDrv_BDMA_CRC32);
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MDrv_BDMA_PatternFill);
#else
EXPORT_SYMBOL(MDrv_BDMA_PatternFill);
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MDrv_BDMA_MemCopy);
#else
EXPORT_SYMBOL(MDrv_BDMA_MemCopy);
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MDrv_BDMA_FlashCopy2Dram);
#else
EXPORT_SYMBOL(MDrv_BDMA_FlashCopy2Dram);
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MDrv_BDMA_CopyHnd);
#else
EXPORT_SYMBOL(MDrv_BDMA_CopyHnd);
#endif
EXPORT_SYMBOL(MDrv_BDMA_SetPowerState);
EXPORT_SYMBOL(MDrv_BDMA_WaitFlashDone);
EXPORT_SYMBOL(MDrv_BDMA_SetSPIOffsetForMCU);
EXPORT_SYMBOL(MDrv_BDMA_DumpCB);
EXPORT_SYMBOL(MDrv_BDMA_Stop_All);
EXPORT_SYMBOL(MDrv_BDMA_Stop);
EXPORT_SYMBOL(MDrv_BDMA_Exit);
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_DVBT2_V2_H_
#ifdef __cplusplus
#endif
#if 0
#endif
#if 0
#else
#endif
#if 0
#else
#endif
#if 0
#else
#endif
#if 0
#else
#endif
#if 0
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef __DRV_DSCMB_V2_H__
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifndef __CMD_SERVICE_HH__
#endif
#ifdef _CH34_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_PWM_V2_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_SYS_DMD_VD_MBX_H_
#ifdef __cplusplus
#endif
EXPORT_SYMBOL(MDrv_SYS_DMD_VD_MBX_Init);
EXPORT_SYMBOL(MDrv_SYS_DMD_VD_MBX_SetType);
EXPORT_SYMBOL(MDrv_SYS_DMD_VD_MBX_ReadReg);
EXPORT_SYMBOL(MDrv_SYS_DMD_VD_MBX_ReadDSPReg);
EXPORT_SYMBOL(MDrv_SYS_DMD_VD_MBX_DBG_ReadReg);
EXPORT_SYMBOL(MDrv_SYS_DMD_VD_MBX_WriteReg);
EXPORT_SYMBOL(MDrv_SYS_DMD_VD_MBX_WriteDSPReg);
EXPORT_SYMBOL(MDrv_SYS_DMD_VD_MBX_DBG_WriteReg);
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_DVBC_V2_H_
#ifdef __cplusplus
#endif
#ifndef DLL_PUBLIC
#endif
#ifdef UFO_DEMOD_DVBC_GET_AGC_INFO
#endif
#ifdef UFO_DEMOD_DVBC_GET_AGC_INFO
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef __DRV_SEM_H__
#ifdef __cplusplus
#endif
#if (defined(MCU_AEON))
#else
#endif
EXPORT_SYMBOL(MDrv_SEM_Init);
EXPORT_SYMBOL(MDrv_SEM_Get_Resource);
EXPORT_SYMBOL(MDrv_SEM_Free_Resource);
EXPORT_SYMBOL(MDrv_SEM_Reset_Resource);
EXPORT_SYMBOL(MDrv_SEM_Get_ResourceID);
EXPORT_SYMBOL(MDrv_SEM_Get_Num);
EXPORT_SYMBOL(MDrv_SEM_GetLibVer);
EXPORT_SYMBOL(MDrv_SEM_SetPowerState);
EXPORT_SYMBOL(MDrv_SEM_Lock);
EXPORT_SYMBOL(MDrv_SEM_Unlock);
EXPORT_SYMBOL(MDrv_SEM_Delete);
EXPORT_SYMBOL(MDrv_SEM_SetDbgLevel);
#ifdef __cplusplus
#endif
#endif
#ifdef __DRVCIPHER_H__
#ifdef __cplusplus
#endif
#ifdef ENABLE_BGC
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_DVBS_H_
#ifdef __cplusplus
#endif
#ifdef UFO_PUBLIC_HEADER_212
#endif
#if defined(UFO_PUBLIC_HEADER_212) || defined(UFO_PUBLIC_HEADER_300) || defined(UFO_DEMOD_GetParam_NEW_FLOW)
#endif
#ifdef UFO_PUBLIC_HEADER_212
#endif
#ifdef UFO_SUPPORT_VCM
#endif
#ifdef UFO_SUPPORT_KERNEL_FLOATING
#endif
#ifdef UFO_DEMOD_BLINDSCAN_NEW_FLOW
#endif
#ifdef UFO_DEMOD_DVBS_CUSTOMIZED_DISEQC_SEND_CMD
#endif
#ifdef UFO_SUPPORT_VCM
#endif
#ifdef UFO_SUPPORT_KERNEL_FLOATING
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef __MSFS_H__
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _LABEL_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRVCMAPOOL_H_
#if defined (CONFIG_MSTAR_IPAPOOL)
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#endif
#ifdef _XC_HDMI_IF_H_
#ifdef __cplusplus
#endif
#if !defined(UFO_PUBLIC_HEADER_500_3)
#endif
#if defined(UFO_PUBLIC_HEADER_212) ||defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_XC_PANEL_EX_H_
#ifdef __cplusplus
#endif
#ifdef _MS_VERSION_H_
#endif
#if !(defined(UFO_PUBLIC_HEADER_212) || defined(UFO_PUBLIC_HEADER_300))
#if !defined (__aarch64__)
#endif
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#else
#endif
EXPORT_SYMBOL(MApi_PNL_EX_GetLibVer);
EXPORT_SYMBOL(MApi_PNL_EX_GetInfo);
EXPORT_SYMBOL(MApi_PNL_EX_GetStatus);
EXPORT_SYMBOL(MApi_PNL_EX_GetStatusEx);
EXPORT_SYMBOL(MApi_PNL_EX_SetDbgLevel);
EXPORT_SYMBOL(MApi_PNL_EX_IOMapBaseInit);
EXPORT_SYMBOL(MApi_PNL_EX_PreInit);
#ifndef _API_XC_PANEL_EX_C_
#endif
EXPORT_SYMBOL(MApi_PNL_EX_Init_Ex);
EXPORT_SYMBOL(MApi_PNL_EX_SetOutput);
EXPORT_SYMBOL(MApi_PNL_EX_ChangePanelType);
EXPORT_SYMBOL(MApi_PNL_EX_TCONMAP_DumpTable);
EXPORT_SYMBOL(MApi_PNL_EX_TCONMAP_Power_Sequence);
EXPORT_SYMBOL(MApi_PNL_EX_TCON_Count_Reset);
EXPORT_SYMBOL(MApi_PNL_EX_TCON_Init);
EXPORT_SYMBOL(MApi_PNL_EX_GetDstInfo);
EXPORT_SYMBOL(MApi_PNL_EX_Control_Out_Swing);
EXPORT_SYMBOL(MApi_PNL_EX_ForceSetPanelDCLK);
EXPORT_SYMBOL(MApi_PNL_EX_ForceSetPanelHStart);
EXPORT_SYMBOL(MApi_PNL_EX_SetOutputPattern);
EXPORT_SYMBOL(MApi_PNL_EX_Mod_Calibration_Setting);
EXPORT_SYMBOL(MApi_PNL_EX_Mod_Do_Calibration);
EXPORT_SYMBOL(MApi_PNL_EX_BD_LVDS_Output_Type);
EXPORT_SYMBOL(MApi_PNL_EX_SetLPLLTypeExt);
EXPORT_SYMBOL(MApi_PNL_EX_Init_MISC);
EXPORT_SYMBOL(MApi_PNL_EX_MOD_OutputConfig_User);
EXPORT_SYMBOL(MApi_PNL_EX_HWLVDSReservedtoLRFlag);
EXPORT_SYMBOL(MApi_PNL_EX_MOD_PVDD_Power_Setting);
EXPORT_SYMBOL(MApi_PNL_EX_SetSSC_En);
EXPORT_SYMBOL(MApi_PNL_EX_SetSSC_Fmodulation);
EXPORT_SYMBOL(MApi_PNL_EX_SetSSC_Rdeviation);
EXPORT_SYMBOL(MApi_PNL_EX_SkipTimingChange);
EXPORT_SYMBOL(MApi_PNL_EX_OverDriver_Init);
EXPORT_SYMBOL(MApi_PNL_EX_OverDriver_Enable);
EXPORT_SYMBOL(MApi_PNL_EX_Get_TCON_Capability);
EXPORT_SYMBOL(MApi_PNL_EX_SetPairSwap);
EXPORT_SYMBOL(MApi_PNL_EX_CalExtLPLLSETbyDClk);
EXPORT_SYMBOL(MApi_PNL_EX_SetDiffSwingLevel);
#ifdef __cplusplus
#endif
#endif
#ifdef _DRVTVENCODER_H
#ifdef __cplusplus
#endif
#ifdef UFO_VE_INITEX
#endif
EXPORT_SYMBOL(MDrv_VE_GetLibVer);
EXPORT_SYMBOL(MDrv_VE_GetInfo);
EXPORT_SYMBOL(MDrv_VE_GetStatus);
EXPORT_SYMBOL(MDrv_VE_SetDbgLevel);
EXPORT_SYMBOL(MDrv_VE_GetCaps);
EXPORT_SYMBOL(MDrv_VE_SwitchInputSource);
EXPORT_SYMBOL(MDrv_VE_SetInputSource);
EXPORT_SYMBOL(MDrv_VE_SwitchOuputDest);
EXPORT_SYMBOL(MDrv_VE_SetOutputCtrl);
EXPORT_SYMBOL(MDrv_VE_SetOutputVideoStd);
EXPORT_SYMBOL(MDrv_VE_SetCusTable);
EXPORT_SYMBOL(MDrv_VE_EnableCusTable);
EXPORT_SYMBOL(MDrv_VE_SetIOMapBase);
#if defined(UFO_PUBLIC_HEADER_700)
EXPORT_SYMBOL(MDrv_VE_Init);
#else
EXPORT_SYMBOL(MDrv_VE_Init);
#endif
#ifdef UFO_VE_INITEX
#endif
#if defined(UFO_PUBLIC_HEADER_700)
EXPORT_SYMBOL(MDrv_VE_GetConfig);
#else
EXPORT_SYMBOL(MDrv_VE_GetConfig);
#endif
EXPORT_SYMBOL(MDrv_VE_Exit);
EXPORT_SYMBOL(MDrv_VE_SetWSSData);
EXPORT_SYMBOL(MDrv_VE_GetWSSData);
EXPORT_SYMBOL(MDrv_VE_SetMode);
EXPORT_SYMBOL(MDrv_VE_SetBlackScreen);
EXPORT_SYMBOL(MDrv_VE_IsBlackScreenEnabled);
EXPORT_SYMBOL(MDrv_VE_EnableTtx);
#if defined(UFO_PUBLIC_HEADER_500_3)||defined(UFO_PUBLIC_HEADER_700)
EXPORT_SYMBOL(MDrv_VE_SetTtxBuffer);
#else
EXPORT_SYMBOL(MDrv_VE_SetTtxBuffer);
#endif
EXPORT_SYMBOL(MDrv_VE_ClearTtxReadDoneStatus);
EXPORT_SYMBOL(MDrv_VE_GetTtxReadDoneStatus);
EXPORT_SYMBOL(MDrv_VE_SetVbiTtxActiveLines);
EXPORT_SYMBOL(MDrv_VE_SetVbiTtxActiveLinesBitmap);
EXPORT_SYMBOL(MDrv_VE_SetVbiTtxRange);
EXPORT_SYMBOL(MDrv_VE_EnableCcSw);
EXPORT_SYMBOL(MDrv_VE_SetCcRange);
EXPORT_SYMBOL(MDrv_VE_SendCcData);
EXPORT_SYMBOL(MDrv_VE_set_display_window);
EXPORT_SYMBOL(MDrv_VE_SetFrameColor);
EXPORT_SYMBOL(MDrv_VE_SetOSD);
EXPORT_SYMBOL(MDrv_VE_Set_OSDLayer);
EXPORT_SYMBOL(MDrv_VE_Get_OSDLayer);
EXPORT_SYMBOL(MDrv_VE_Set_VideoAlpha);
EXPORT_SYMBOL(MDrv_VE_Get_VideoAlpha);
EXPORT_SYMBOL(MDrv_VE_SetRGBIn);
EXPORT_SYMBOL(MDrv_VE_Get_Output_Video_Std);
EXPORT_SYMBOL(MDrv_VE_SetCaptureMode);
EXPORT_SYMBOL(MApi_VE_GetDstInfo);
EXPORT_SYMBOL(MDrv_VE_Set_TestPattern);
EXPORT_SYMBOL(MApi_VE_W2BYTE_MSK);
EXPORT_SYMBOL(MApi_VE_R2BYTE_MSK);
EXPORT_SYMBOL(MDrv_VE_DumpTable);
EXPORT_SYMBOL(MDrv_VE_DisableRegWrite);
EXPORT_SYMBOL(MDrv_VE_ShowColorBar);
EXPORT_SYMBOL(MDrv_VE_AdjustPositionBase);
EXPORT_SYMBOL(MDrv_VE_SetFrameLock);
EXPORT_SYMBOL(MDrv_VE_Set_Customer_Scaling);
EXPORT_SYMBOL(MDrv_VE_set_crop_window);
EXPORT_SYMBOL(MDrv_VE_set_be_display_window);
EXPORT_SYMBOL(MDrv_VE_Get_Ext_Caps);
EXPORT_SYMBOL(MDrv_VE_SetWindow);
EXPORT_SYMBOL(MDrv_VE_InitVECapture);
EXPORT_SYMBOL(MDrv_VE_EnaVECapture);
EXPORT_SYMBOL(MDrv_VE_GetVECaptureState);
EXPORT_SYMBOL(MDrv_VE_VECaptureWaitOnFrame);
EXPORT_SYMBOL(MDrv_VE_Adjust_FrameStart);
EXPORT_SYMBOL(MDrv_VE_SetWSS525Data);
EXPORT_SYMBOL(MDrv_VE_GetWSS525Data);
EXPORT_SYMBOL(MDrv_VE_OnOffWSS);
EXPORT_SYMBOL(MDrv_VE_GetWSSStatus);
EXPORT_SYMBOL(MDrv_VE_OnOffMV);
EXPORT_SYMBOL(MDrv_VE_GetMVStatus);
EXPORT_SYMBOL(MDrv_VE_OnOffDCS);
EXPORT_SYMBOL(MDrv_VE_GetDCSStatus);
EXPORT_SYMBOL(MDrv_TVE_SetPowerState);
#if defined(UFO_PUBLIC_HEADER_212) || defined(UFO_PUBLIC_HEADER_500_3)
EXPORT_SYMBOL(MDrv_VE_ReloadSetting);
#endif
#ifdef UFO_PUBLIC_HEADER_212
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_WDT_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_LDM_H_
#ifdef __cplusplus
#endif
#ifdef MDRV_LDM_C
#else
#endif
#if defined(MSOS_TYPE_LINUX)
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_BDMA_V2_H_
#ifdef __cplusplus
#endif
#ifdef BIFROST_32BIT_MODE
#if defined(UFO_PUBLIC_HEADER_212) || defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#ifdef MOBF_ENABLE
#endif
#else
#if defined(UFO_PUBLIC_HEADER_212) || defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#ifdef MOBF_ENABLE
#endif
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_DMD_COMMON_H_
#ifdef __cplusplus
#endif
#if(1)
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_DVBT_H_
#ifdef __cplusplus
#endif
#if 1
#else
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_ISDBT_V2_H_
#ifdef __cplusplus
#endif
#ifndef DLL_PUBLIC
#endif
#ifdef UTPA2
#else
#endif
#ifdef UTPA2
#else
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRVCLKM_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _XC_PCMONITOR_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _UFO_H_
#ifdef UFO_XC_HDR
#endif
#endif
#ifdef _DRV_SERFLASH_V2_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_DVBT2_H_
#ifdef __cplusplus
#endif
#ifdef UFO_DEMOD_DVBT2_SUPPORT_DMD_INT
#endif
#ifdef UFO_DEMOD_DVBT2_SUPPORT_DMD_INT
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_XC_LD_V2_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_TEMP_H_
#ifdef __cplusplus
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _IOPORT_H_
#endif
#ifdef _API_VDEC_H_
#ifdef __cplusplus
#endif
#ifndef VDEC_CAP_SYSTEM_PREGET_FB_MEM_USAGE_SIZE
#endif
#if defined(UFO_PUBLIC_HEADER_700)||defined(UFO_PUBLIC_HEADER_500_3)||defined(UFO_PUBLIC_HEADER_500)||defined(UFO_PUBLIC_HEADER_300)
#endif
#if defined(UFO_PUBLIC_HEADER_212)
#endif
#if defined(UFO_PUBLIC_HEADER_700)||defined(UFO_PUBLIC_HEADER_300)
#endif
#if defined(UFO_PUBLIC_HEADER_500)||defined(UFO_PUBLIC_HEADER_500_3)
#endif
#if defined(UFO_PUBLIC_HEADER_212)
#endif
EXPORT_SYMBOL(MApi_VDEC_GetLibVer);
EXPORT_SYMBOL(MApi_VDEC_GetInfo);
EXPORT_SYMBOL(MApi_VDEC_GetStatus);
EXPORT_SYMBOL(MApi_VDEC_CheckCaps);
EXPORT_SYMBOL(MApi_VDEC_EnableTurboMode);
EXPORT_SYMBOL(MApi_VDEC_Init);
EXPORT_SYMBOL(MApi_VDEC_Init_EX);
EXPORT_SYMBOL(MApi_VDEC_Rst);
EXPORT_SYMBOL(MApi_VDEC_Exit);
EXPORT_SYMBOL(MApi_VDEC_CheckDispInfoRdy);
EXPORT_SYMBOL(MApi_VDEC_SetFrcMode);
EXPORT_SYMBOL(MApi_VDEC_SetDynScalingParams);
EXPORT_SYMBOL(MApi_VDEC_SetDbgLevel);
EXPORT_SYMBOL(MApi_VDEC_Play);
EXPORT_SYMBOL(MApi_VDEC_Pause);
EXPORT_SYMBOL(MApi_VDEC_Resume);
EXPORT_SYMBOL(MApi_VDEC_StepDisp);
EXPORT_SYMBOL(MApi_VDEC_IsStepDispDone);
EXPORT_SYMBOL(MApi_VDEC_StepDecode);
EXPORT_SYMBOL(MApi_VDEC_IsStepDecodeDone);
EXPORT_SYMBOL(MApi_VDEC_SetTrickMode);
EXPORT_SYMBOL(MApi_VDEC_SetBlockDisplay);
EXPORT_SYMBOL(MApi_VDEC_EnableESBuffMalloc);
EXPORT_SYMBOL(MApi_VDEC_GetESBuffVacancy);
EXPORT_SYMBOL(MApi_VDEC_GetESBuff);
EXPORT_SYMBOL(MApi_VDEC_PushDecQ);
EXPORT_SYMBOL(MApi_VDEC_Flush);
EXPORT_SYMBOL(MApi_VDEC_EnableLastFrameShow);
EXPORT_SYMBOL(MApi_VDEC_SetSpeed);
EXPORT_SYMBOL(MApi_VDEC_SetFreezeDisp);
EXPORT_SYMBOL(MApi_VDEC_SetBlueScreen);
EXPORT_SYMBOL(MApi_VDEC_ResetPTS);
EXPORT_SYMBOL(MApi_VDEC_AVSyncOn);
EXPORT_SYMBOL(MApi_VDEC_SetAVSyncFreerunThreshold);
EXPORT_SYMBOL(MApi_VDEC_GetDispInfo);
EXPORT_SYMBOL(MApi_VDEC_IsAVSyncOn);
EXPORT_SYMBOL(MApi_VDEC_IsWithValidStream);
EXPORT_SYMBOL(MApi_VDEC_IsDispFinish);
EXPORT_SYMBOL(MApi_VDEC_IsFrameRdy);
EXPORT_SYMBOL(MApi_VDEC_IsIFrameFound);
EXPORT_SYMBOL(MApi_VDEC_IsSeqChg);
EXPORT_SYMBOL(MApi_VDEC_IsReachSync);
EXPORT_SYMBOL(MApi_VDEC_IsFreerun);
EXPORT_SYMBOL(MApi_VDEC_IsStartSync);
EXPORT_SYMBOL(MApi_VDEC_IsWithLowDelay);
EXPORT_SYMBOL(MApi_VDEC_IsAllBufferEmpty);
EXPORT_SYMBOL(MApi_VDEC_GetExtDispInfo);
EXPORT_SYMBOL(MApi_VDEC_GetDecFrameInfo);
EXPORT_SYMBOL(MApi_VDEC_GetDispFrameInfo);
EXPORT_SYMBOL(MApi_VDEC_GetNextDispFrame);
EXPORT_SYMBOL(MApi_VDEC_DisplayFrame);
EXPORT_SYMBOL(MApi_VDEC_ReleaseFrame);
EXPORT_SYMBOL(MApi_VDEC_GetDecTimeCode);
EXPORT_SYMBOL(MApi_VDEC_GetDispTimeCode);
EXPORT_SYMBOL(MApi_VDEC_SetEvent);
EXPORT_SYMBOL(MApi_VDEC_UnsetEvent);
EXPORT_SYMBOL(MApi_VDEC_SetEvent_EX);
EXPORT_SYMBOL(MApi_VDEC_UnsetEvent_EX);
EXPORT_SYMBOL(MApi_VDEC_GetEventInfo);
EXPORT_SYMBOL(MApi_VDEC_FireDecCmd);
EXPORT_SYMBOL(MApi_VDEC_SeekToPTS);
EXPORT_SYMBOL(MApi_VDEC_SkipToPTS);
EXPORT_SYMBOL(MApi_VDEC_DisableDeblocking);
EXPORT_SYMBOL(MApi_VDEC_DisableQuarterPixel);
EXPORT_SYMBOL(MApi_VDEC_SetAutoRmLstZeroByte);
EXPORT_SYMBOL(MApi_VDEC_SetBalanceBW);
EXPORT_SYMBOL(MApi_VDEC_GetActiveFormat);
EXPORT_SYMBOL(MApi_VDEC_GetColourPrimaries);
EXPORT_SYMBOL(MApi_VDEC_GetGOPCnt);
EXPORT_SYMBOL(MApi_VDEC_GetESWritePtr);
EXPORT_SYMBOL(MApi_VDEC_GetESReadPtr);
EXPORT_SYMBOL(MApi_VDEC_GetPTS);
EXPORT_SYMBOL(MApi_VDEC_GetNextPTS);
EXPORT_SYMBOL(MApi_VDEC_GetErrCode);
EXPORT_SYMBOL(MApi_VDEC_GetErrCnt);
EXPORT_SYMBOL(MApi_VDEC_GetBitsRate);
EXPORT_SYMBOL(MApi_VDEC_GetFrameCnt);
EXPORT_SYMBOL(MApi_VDEC_GetSkipCnt);
EXPORT_SYMBOL(MApi_VDEC_GetDropCnt);
EXPORT_SYMBOL(MApi_VDEC_GetDecQVacancy);
EXPORT_SYMBOL(MApi_VDEC_Is32PullDown);
EXPORT_SYMBOL(MApi_VDEC_IsAlive);
EXPORT_SYMBOL(MApi_VDEC_GetVideoPtsStcDelta);
EXPORT_SYMBOL(MApi_VDEC_IsCCAvailable);
EXPORT_SYMBOL(MApi_VDEC_GetCCInfo);
EXPORT_SYMBOL(MApi_VDEC_GetTrickMode);
EXPORT_SYMBOL(MApi_VDEC_GetActiveCodecType);
EXPORT_SYMBOL(MDrv_VDEC_GenPattern);
EXPORT_SYMBOL(MApi_VDEC_GenPattern);
EXPORT_SYMBOL(MApi_VDEC_GetPatternLeastLength);
EXPORT_SYMBOL(MApi_VDEC_MHEG_DecodeIFrame);
EXPORT_SYMBOL(MApi_VDEC_MHEG_IsIFrameDecoding);
EXPORT_SYMBOL(MApi_VDEC_MHEG_RstIFrameDec);
EXPORT_SYMBOL(MApi_VDEC_CC_StartParsing);
EXPORT_SYMBOL(MApi_VDEC_CC_StopParsing);
EXPORT_SYMBOL(MApi_VDEC_CC_GetWritePtr);
EXPORT_SYMBOL(MApi_VDEC_CC_GetReadPtr);
EXPORT_SYMBOL(MApi_VDEC_CC_UpdateReadPtr);
EXPORT_SYMBOL(MApi_VDEC_CC_GetIsOverflow);
EXPORT_SYMBOL(MApi_VDEC_CC_Init);
EXPORT_SYMBOL(MApi_VDEC_CC_SetCfg);
EXPORT_SYMBOL(MApi_VDEC_CC_SetBuffStartAdd);
EXPORT_SYMBOL(MApi_VDEC_CC_UpdateWriteAdd);
EXPORT_SYMBOL(MApi_VDEC_CC_UpdateReadAdd);
EXPORT_SYMBOL(MApi_VDEC_CC_DisableParsing);
EXPORT_SYMBOL(MApi_VDEC_CC_GetIsRstDone);
EXPORT_SYMBOL(MApi_VDEC_CC_GetIsBuffOverflow);
EXPORT_SYMBOL(MApi_VDEC_CC_GetWriteAdd);
EXPORT_SYMBOL(MApi_VDEC_CC_GetReadAdd);
EXPORT_SYMBOL(MApi_VDEC_DbgCmd);
EXPORT_SYMBOL(MApi_VDEC_DbgSetData);
EXPORT_SYMBOL(MApi_VDEC_DbgGetData);
EXPORT_SYMBOL(MApi_VDEC_SetControl);
EXPORT_SYMBOL(MApi_VDEC_PreSetControl);
EXPORT_SYMBOL(MApi_VDEC_GetFwVersion);
EXPORT_SYMBOL(MApi_VDEC_GetDispCnt);
EXPORT_SYMBOL(MApi_VDEC_GetControl);
EXPORT_SYMBOL(MApi_VDEC_SystemPreSetControl);
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_MFC_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_ACE_PRIV_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_HDMITX_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_GPD_V2_H_
#ifdef __cplusplus
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_MFE_ST_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_ACP_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRVPWS_H_
#ifdef __cplusplus
#endif
#if defined (MSOS_TYPE_LINUX)
#endif
#if defined (MSOS_TYPE_LINUX)
#endif
#if defined (MSOS_TYPE_LINUX)
#endif
#if defined (MSOS_TYPE_LINUX)
#endif
#if defined (MSOS_TYPE_LINUX)
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_CIPHER_V2_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _MVOP_H_
#ifdef __cplusplus
#endif
#if 1
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_HDMITX_V2_H_
#ifdef __cplusplus
#endif
#if (defined(VANCLEEF_MCP) || defined(RAPTORS_MCP))
#endif
#if defined(VANCLEEF_MCP) || defined(RAPTORS_MCP)
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_XC_LD_H_
#ifdef __cplusplus
#endif
#if LD_LOG_DBG
#else
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_JPEG_H_
#ifdef __cplusplus
#endif
#if SUPPORT_MPO_FORMAT
#endif
#if SUPPORT_MPO_FORMAT
#endif
#if SUPPORT_GET_FULL_IMAGE_INFO
#endif
#if SUPPORT_MPO_FORMAT
#endif
EXPORT_SYMBOL(MApi_JPEG_EnableOJPD);
EXPORT_SYMBOL(MApi_JPEG_Init_UsingOJPD);
EXPORT_SYMBOL(MApi_JPEG_Init);
EXPORT_SYMBOL(MApi_JPEG_DecodeHdr);
EXPORT_SYMBOL(MApi_JPEG_Decode);
EXPORT_SYMBOL(MApi_JPEG_Exit);
EXPORT_SYMBOL(MApi_JPEG_GetErrorCode);
EXPORT_SYMBOL(MApi_JPEG_GetJPDEventFlag);
EXPORT_SYMBOL(MApi_JPEG_SetJPDEventFlag);
EXPORT_SYMBOL(MApi_JPEG_Rst);
EXPORT_SYMBOL(MApi_JPEG_PowerOn);
EXPORT_SYMBOL(MApi_JPEG_PowerOff);
EXPORT_SYMBOL(MApi_JPEG_GetCurVidx);
EXPORT_SYMBOL(MApi_JPEG_IsProgressive);
EXPORT_SYMBOL(MApi_JPEG_ThumbnailFound);
EXPORT_SYMBOL(MApi_JPEG_GetWidth);
EXPORT_SYMBOL(MApi_JPEG_GetHeight);
EXPORT_SYMBOL(MApi_JPEG_GetOriginalWidth);
EXPORT_SYMBOL(MApi_JPEG_GetOriginalHeight);
EXPORT_SYMBOL(MApi_JPEG_GetNonAlignmentWidth);
EXPORT_SYMBOL(MApi_JPEG_GetNonAlignmentHeight);
EXPORT_SYMBOL(MApi_JPEG_GetAlignedPitch);
EXPORT_SYMBOL(MApi_JPEG_GetAlignedPitch_H);
EXPORT_SYMBOL(MApi_JPEG_GetAlignedWidth);
EXPORT_SYMBOL(MApi_JPEG_GetAlignedHeight);
EXPORT_SYMBOL(MApi_JPEG_GetScaleDownFactor);
EXPORT_SYMBOL(MApi_JPEG_SetMaxDecodeResolution);
EXPORT_SYMBOL(MApi_JPEG_SetProMaxDecodeResolution);
EXPORT_SYMBOL(MApi_JPEG_SetMRBufferValid);
EXPORT_SYMBOL(MApi_JPEG_UpdateReadInfo);
EXPORT_SYMBOL(MApi_JPEG_ProcessEOF);
EXPORT_SYMBOL(MApi_JPEG_SetErrCode);
EXPORT_SYMBOL(MApi_JPEG_SetDbgLevel);
EXPORT_SYMBOL(MApi_JPEG_GetDbgLevel);
EXPORT_SYMBOL(MApi_JPEG_GetInfo);
EXPORT_SYMBOL(MApi_JPEG_GetStatus);
EXPORT_SYMBOL(MApi_JPEG_GetLibVer);
EXPORT_SYMBOL(MApi_JPEG_HdlVidxChk);
EXPORT_SYMBOL(MApi_JPEG_GetBuffLoadType);
EXPORT_SYMBOL(MApi_JPEG_EnableISR);
EXPORT_SYMBOL(MApi_JPEG_DisableISR);
EXPORT_SYMBOL(MApi_JPEG_WaitDone);
EXPORT_SYMBOL(MApi_JPEG_GetEXIFDateTime);
EXPORT_SYMBOL(MApi_JPEG_GetEXIFOrientation);
#if SUPPORT_EXIF_EXTRA_INFO
EXPORT_SYMBOL(MApi_JPEG_GetEXIFManufacturer);
EXPORT_SYMBOL(MApi_JPEG_GetEXIFModel);
EXPORT_SYMBOL(MApi_JPEG_GetEXIFFlash);
EXPORT_SYMBOL(MApi_JPEG_GetEXIFISOSpeedRatings);
EXPORT_SYMBOL(MApi_JPEG_GetEXIFShutterSpeedValue);
EXPORT_SYMBOL(MApi_JPEG_GetEXIFApertureValue);
EXPORT_SYMBOL(MApi_JPEG_GetEXIFExposureBiasValue);
EXPORT_SYMBOL(MApi_JPEG_GetEXIFFocalLength);
EXPORT_SYMBOL(MApi_JPEG_GetEXIFImageWidth);
EXPORT_SYMBOL(MApi_JPEG_GetEXIFImageHeight);
EXPORT_SYMBOL(MApi_JPEG_GetEXIFExposureTime);
EXPORT_SYMBOL(MApi_JPEG_GetEXIFFNumber);
#endif
EXPORT_SYMBOL(MApi_JPEG_GetControl);
EXPORT_SYMBOL(MApi_JPEG_DisableAddressConvert);
EXPORT_SYMBOL(MApi_JPEG_GetFreeMemory);
EXPORT_SYMBOL(MApi_JPEG_GetDataOffset);
EXPORT_SYMBOL(MApi_JPEG_GetSOFOffset);
EXPORT_SYMBOL(MApi_JPEG_SetNJPDInstance);
EXPORT_SYMBOL(MApi_JPEG_SupportCMYK);
EXPORT_SYMBOL(MApi_JPEG_SupportRGB);
EXPORT_SYMBOL(MApi_JPEG_SetMHEG5);
#if SUPPORT_MPO_FORMAT
EXPORT_SYMBOL(MApi_JPEG_IsMPOFormat);
EXPORT_SYMBOL(MApi_JPEG_GetMPOIndex);
EXPORT_SYMBOL(MApi_JPEG_GetMPOAttr);
EXPORT_SYMBOL(MApi_JPEG_DumpMPO);
EXPORT_SYMBOL(MApi_JPEG_SetMPOBuffer);
EXPORT_SYMBOL(MApi_JPEG_SetMPOMaxDecodeResolution);
EXPORT_SYMBOL(MApi_JPEG_SetMPOProMaxDecodeResolution);
#endif
EXPORT_SYMBOL(MApi_JPEG_SetVerificationMode);
EXPORT_SYMBOL(MApi_JPEG_GetVerificationMode);
EXPORT_SYMBOL(MApi_NJPD_Debug);
EXPORT_SYMBOL(MApi_JPEG_IsNJPD);
#ifdef __cplusplus
#endif
#endif
#ifdef __DRV_CH34_H__
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _MS_COMMON_H_
#ifndef MSOS_TYPE_LINUX_KERNEL
#else
#endif
#ifdef MSOS_TYPE_ECOS
#endif
#ifdef CONFIG_ENABLE_MENUCONFIG
#endif
#ifdef MSOS_TYPE_LINUX
#elif defined(MSOS_TYPE_ECOS)
#elif defined(MSOS_TYPE_OPTEE)
#else
#ifndef MSOS_TYPE_LINUX_KERNEL
#ifndef MSOS_TYPE_OPTEE
#else
#endif
#endif
#endif
#ifdef CONFIG_ENABLE_MENUCONFIG
#endif
#ifndef DLL_PUBLIC
#endif
#ifndef DLL_LOCAL
#endif
#ifndef MSOS_TYPE_LINUX_KERNEL
#else
#endif
#if 0
#ifdef __aeon__
#endif
#if defined(__mips__)
#endif
#endif
#if 0
#endif
#ifdef MSOS_TYPE_ECOS
#ifdef BLOCK_ASSERT
#else
#endif
#else
#ifdef MSOS_TYPE_LINUX_KERNEL
#ifdef BLOCK_ASSERT
#else
#endif
#else
#ifdef BLOCK_ASSERT
#else
#endif
#endif
#endif
#ifdef MSOS_TYPE_ECOS
#elif defined(MSOS_TYPE_OPTEE)
#else
#endif
#ifndef MIN
#endif
#ifndef MAX
#endif
#ifndef BIT
#endif
#if 1
#endif
#endif
#ifdef __DRV_TCF_H__
#endif
#ifdef _API_GOPSC_EX_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _MS_IRQ_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_CPU_H_
#ifdef __cplusplus
#endif
#if (CPU_UTOPIA20)
#endif
#if !defined(CONFIG_FRC)
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef __API_DMX_H__
#ifdef __cplusplus
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if 0
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if 0
#endif
#if 0
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_700)
#if !defined (__aarch64__)
#endif
#endif
#if defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_700)
#if !defined (__aarch64__)
#endif
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#ifdef DMX_UTOPIA_20
#endif
#if 0
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _IOPORT_EX_H_
#endif
#ifdef _DMS_V2_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef __MDRV_UART_H__
#ifdef __cplusplus
#endif
#if UART_TYPE_EYWA
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_VOIP_MFE_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef __DRV_SC_H__
#ifdef __cplusplus
#endif
#ifdef UFO_PUBLIC_HEADER_212
#endif
#ifdef UFO_PUBLIC_HEADER_212
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_MVOP_H_
#ifdef __cplusplus
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_500)
#endif
#if defined(UFO_PUBLIC_HEADER_212)
#endif
#if defined(UFO_PUBLIC_HEADER_500)
#endif
#if defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_212)
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_212) || defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_700) || defined(UFO_PUBLIC_HEADER_212)
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#endif
EXPORT_SYMBOL(MDrv_MVOP_GetConfig);
EXPORT_SYMBOL(MDrv_MVOP_Init);
EXPORT_SYMBOL(MDrv_MVOP_Exit);
EXPORT_SYMBOL(MDrv_MVOP_Enable);
EXPORT_SYMBOL(MDrv_MVOP_SetInputCfg);
EXPORT_SYMBOL(MDrv_MVOP_SetOutputCfg);
EXPORT_SYMBOL(MDrv_MVOP_EnableBlackBG);
EXPORT_SYMBOL(MDrv_MVOP_EnableUVShift);
EXPORT_SYMBOL(MDrv_MVOP_SetMonoMode);
EXPORT_SYMBOL(MDrv_MVOP_GetHSize);
EXPORT_SYMBOL(MDrv_MVOP_GetVSize);
EXPORT_SYMBOL(MDrv_MVOP_GetHStart);
EXPORT_SYMBOL(MDrv_MVOP_GetVStart);
EXPORT_SYMBOL(MDrv_MVOP_GetIsInterlace);
EXPORT_SYMBOL(MDrv_MVOP_GetIsHDuplicate);
EXPORT_SYMBOL(MDrv_MVOP_GetIsEnable);
EXPORT_SYMBOL(MDrv_MVOP_GetOutputTiming);
EXPORT_SYMBOL(MDrv_MVOP_GetLibVer);
EXPORT_SYMBOL(MDrv_MVOP_CheckCapability);
EXPORT_SYMBOL(MDrv_MVOP_GetMaxHOffset);
EXPORT_SYMBOL(MDrv_MVOP_GetMaxVOffset);
EXPORT_SYMBOL(MDrv_MVOP_SetDbgLevel);
EXPORT_SYMBOL(MDrv_MVOP_GetInfo);
EXPORT_SYMBOL(MDrv_MVOP_GetStatus);
EXPORT_SYMBOL(MDrv_MVOP_EnableInterrupt);
EXPORT_SYMBOL(MDrv_MVOP_SetPattern);
EXPORT_SYMBOL(MDrv_MVOP_SetTileFormat);
EXPORT_SYMBOL(MDrv_MVOP_GetDstInfo);
EXPORT_SYMBOL(MDrv_MVOP_SetFixVtt);
EXPORT_SYMBOL(MDrv_MVOP_SetMMIOMapBase);
EXPORT_SYMBOL(MDrv_MVOP_MiuSwitch);
EXPORT_SYMBOL(MDrv_MVOP_SetBaseAdd);
EXPORT_SYMBOL(MDrv_MVOP_SEL_OP_FIELD);
EXPORT_SYMBOL(MDrv_MVOP_SetRegSizeFromMVD);
EXPORT_SYMBOL(MDrv_MVOP_SetStartPos);
EXPORT_SYMBOL(MDrv_MVOP_SetImageWidthHight);
EXPORT_SYMBOL(MDrv_MVOP_SetVOPMirrorMode);
EXPORT_SYMBOL(MDrv_MVOP_INV_OP_VS);
EXPORT_SYMBOL(MDrv_MVOP_FORCE_TOP);
EXPORT_SYMBOL(MDrv_MVOP_EnableFreerunMode);
EXPORT_SYMBOL(MDrv_MVOP_GetBaseAdd);
EXPORT_SYMBOL(MDrv_MVOP_SubGetStatus);
EXPORT_SYMBOL(MDrv_MVOP_SubGetHStart);
EXPORT_SYMBOL(MDrv_MVOP_SubGetVStart);
EXPORT_SYMBOL(MDrv_MVOP_SubSetPattern);
EXPORT_SYMBOL(MDrv_MVOP_SubSetTileFormat);
EXPORT_SYMBOL(MDrv_MVOP_SubSetMMIOMapBase);
EXPORT_SYMBOL(MDrv_MVOP_SubInit);
EXPORT_SYMBOL(MDrv_MVOP_SubExit);
EXPORT_SYMBOL(MDrv_MVOP_SubEnable);
EXPORT_SYMBOL(MDrv_MVOP_SubGetIsEnable);
EXPORT_SYMBOL(MDrv_MVOP_SubEnableUVShift);
EXPORT_SYMBOL(MDrv_MVOP_SubEnableBlackBG);
EXPORT_SYMBOL(MDrv_MVOP_SubSetMonoMode);
EXPORT_SYMBOL(MDrv_MVOP_SubSetInputCfg);
EXPORT_SYMBOL(MDrv_MVOP_SubSetOutputCfg);
EXPORT_SYMBOL(MDrv_MVOP_SubGetOutputTiming);
EXPORT_SYMBOL(MDrv_MVOP_SubGetHSize);
EXPORT_SYMBOL(MDrv_MVOP_SubGetVSize);
EXPORT_SYMBOL(MDrv_MVOP_SubGetIsInterlace);
EXPORT_SYMBOL(MDrv_MVOP_SubGetIsHDuplicate);
EXPORT_SYMBOL(MDrv_MVOP_SubCheckCapability);
EXPORT_SYMBOL(MDrv_MVOP_SubGetMaxHOffset);
EXPORT_SYMBOL(MDrv_MVOP_SubGetMaxVOffset);
EXPORT_SYMBOL(MDrv_MVOP_SubGetDstInfo);
EXPORT_SYMBOL(MDrv_MVOP_SubSetFixVtt);
EXPORT_SYMBOL(MDrv_MVOP_SubMiuSwitch);
EXPORT_SYMBOL(MDrv_MVOP_SubSetVOPMirrorMode);
EXPORT_SYMBOL(MDrv_MVOP_SubEnableFreerunMode);
EXPORT_SYMBOL(MDrv_MVOP_SubSetBaseAdd);
EXPORT_SYMBOL(MDrv_MVOP_SubGetBaseAdd);
EXPORT_SYMBOL(MDrv_MVOP_EX_Init);
EXPORT_SYMBOL(MDrv_MVOP_EX_Exit);
EXPORT_SYMBOL(MDrv_MVOP_EX_Enable);
EXPORT_SYMBOL(MDrv_MVOP_EX_SetInputCfg);
EXPORT_SYMBOL(MDrv_MVOP_EX_SetOutputCfg);
EXPORT_SYMBOL(MDrv_MVOP_EX_SetPattern);
EXPORT_SYMBOL(MDrv_MVOP_EX_SetTileFormat);
EXPORT_SYMBOL(MDrv_MVOP_EX_EnableUVShift);
EXPORT_SYMBOL(MDrv_MVOP_EX_EnableBlackBG);
EXPORT_SYMBOL(MDrv_MVOP_EX_SetMonoMode);
EXPORT_SYMBOL(MDrv_MVOP_EX_SetFixVtt);
EXPORT_SYMBOL(MDrv_MVOP_EX_MiuSwitch);
EXPORT_SYMBOL(MDrv_MVOP_EX_SetVOPMirrorMode);
EXPORT_SYMBOL(MDrv_MVOP_EX_EnableFreerunMode);
EXPORT_SYMBOL(MDrv_MVOP_EX_GetOutputTiming);
EXPORT_SYMBOL(MDrv_MVOP_EX_GetIsEnable);
EXPORT_SYMBOL(MDrv_MVOP_EX_GetHStart);
EXPORT_SYMBOL(MDrv_MVOP_EX_GetVStart);
EXPORT_SYMBOL(MDrv_MVOP_EX_GetHSize);
EXPORT_SYMBOL(MDrv_MVOP_EX_GetVSize);
EXPORT_SYMBOL(MDrv_MVOP_EX_GetIsInterlace);
EXPORT_SYMBOL(MDrv_MVOP_EX_GetIsHDuplicate);
EXPORT_SYMBOL(MDrv_MVOP_EX_GetStatus);
#if defined (__aarch64__)
EXPORT_SYMBOL(MDrv_MVOP_EX_CheckCapability);
#else
EXPORT_SYMBOL(MDrv_MVOP_EX_CheckCapability);
#endif
EXPORT_SYMBOL(MDrv_MVOP_EX_GetDstInfo);
EXPORT_SYMBOL(MDrv_MVOP_SendBlueScreen);
EXPORT_SYMBOL(MDrv_MVOP_SetCommand);
EXPORT_SYMBOL(MDrv_MVOP_GetCommand);
EXPORT_SYMBOL(MDrv_MVOP_SetFrequency);
EXPORT_SYMBOL(MDrv_MVOP_SetPowerState);
#ifdef __cplusplus
#endif
#endif
#ifdef _API_XC_EX_H_
#ifdef __cplusplus
#endif
#ifdef MSOS_TYPE_LINUX_KERNEL
#else
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500) ||defined(UFO_PUBLIC_HEADER_300)||defined(UFO_PUBLIC_HEADER_700)||defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700) ||defined(UFO_PUBLIC_HEADER_300) ||defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#ifdef UFO_XC_SCAN_TYPE
#endif
EXPORT_SYMBOL(MApi_XC_EX_GetLibVer);
EXPORT_SYMBOL(MApi_XC_EX_GetInfo);
EXPORT_SYMBOL(MApi_XC_EX_GetStatus);
EXPORT_SYMBOL(MApi_XC_EX_GetStatusEx);
EXPORT_SYMBOL(MApi_XC_EX_SetDbgLevel);
EXPORT_SYMBOL(MApi_XC_EX_FPLLDbgMode);
EXPORT_SYMBOL(MApi_XC_EX_FPLLCustomerMode);
EXPORT_SYMBOL(MDrv_XC_EX_SetIOMapBase);
EXPORT_SYMBOL(MApi_XC_EX_Init);
EXPORT_SYMBOL(MApi_XC_EX_Init_MISC);
EXPORT_SYMBOL(MApi_XC_EX_GetMISCStatus);
EXPORT_SYMBOL(MApi_XC_EX_GetCapability);
EXPORT_SYMBOL(MApi_XC_EX_GetChipCaps);
EXPORT_SYMBOL(MApi_XC_EX_Exit);
EXPORT_SYMBOL(MApi_XC_EX_SetDynamicScaling);
EXPORT_SYMBOL(MApi_XC_EX_GetDynamicScalingStatus);
#if defined(UFO_PUBLIC_HEADER_700)
EXPORT_SYMBOL(MApi_XC_EX_Get_DNRBaseOffset);
#else
EXPORT_SYMBOL(MApi_XC_EX_Get_DNRBaseOffset);
#endif
EXPORT_SYMBOL(MApi_XC_EX_Get_FrameNumFactor);
EXPORT_SYMBOL(MApi_XC_EX_SetWindow);
EXPORT_SYMBOL(MApi_XC_EX_SetDualWindow);
EXPORT_SYMBOL(MApi_XC_EX_SetTravelingWindow);
EXPORT_SYMBOL(MApi_XC_EX_SetInputSource);
EXPORT_SYMBOL(MApi_XC_EX_IsYUVSpace);
EXPORT_SYMBOL(MApi_XC_EX_IsMemoryFormat422);
EXPORT_SYMBOL(MApi_XC_EX_EnableForceRGBin);
EXPORT_SYMBOL(MApi_XC_EX_EnableMirrorModeEx);
EXPORT_SYMBOL(MApi_XC_EX_GetMirrorModeTypeEx);
EXPORT_SYMBOL(MApi_XC_EX_GetSyncStatus);
EXPORT_SYMBOL(MApi_XC_EX_WaitOutputVSync);
EXPORT_SYMBOL(MApi_XC_EX_WaitInputVSync);
EXPORT_SYMBOL(MApi_XC_EX_SetHdmiSyncMode);
EXPORT_SYMBOL(MApi_XC_EX_GetHdmiSyncMode);
EXPORT_SYMBOL(MApi_XC_EX_SetRepWindow);
EXPORT_SYMBOL(MApi_XC_EX_Set_OPWriteOffEnable);
EXPORT_SYMBOL(MApi_XC_EX_ForceSet_OPWriteOffEnable);
EXPORT_SYMBOL(MApi_XC_EX_SetDispWinToReg);
EXPORT_SYMBOL(MApi_XC_EX_GetDispWinFromReg);
EXPORT_SYMBOL(MApi_XC_EX_FreezeImg);
EXPORT_SYMBOL(MApi_XC_EX_IsFreezeImg);
EXPORT_SYMBOL(MApi_XC_EX_GenerateBlackVideoForBothWin);
EXPORT_SYMBOL(MApi_XC_EX_Set_BLSK);
EXPORT_SYMBOL(MApi_XC_EX_GenerateBlackVideo);
EXPORT_SYMBOL(MApi_XC_EX_IsBlackVideoEnable);
EXPORT_SYMBOL(MApi_XC_EX_EnableFrameBufferLess);
EXPORT_SYMBOL(MApi_XC_EX_IsCurrentFrameBufferLessMode);
EXPORT_SYMBOL(MApi_XC_EX_EnableRequest_FrameBufferLess);
EXPORT_SYMBOL(MApi_XC_EX_IsCurrentRequest_FrameBufferLessMode);
EXPORT_SYMBOL(MApi_XC_EX_Get_3D_HW_Version);
EXPORT_SYMBOL(MApi_XC_EX_Get_3D_IsSupportedHW2DTo3D);
EXPORT_SYMBOL(MApi_XC_EX_Set_3D_Mode);
EXPORT_SYMBOL(MApi_XC_EX_Set_3D_MainWin_FirstMode);
EXPORT_SYMBOL(MApi_XC_EX_Set_3D_LR_Frame_Exchg);
EXPORT_SYMBOL(MApi_XC_EX_3D_Is_LR_Frame_Exchged);
EXPORT_SYMBOL(MApi_XC_EX_Get_3D_Input_Mode);
EXPORT_SYMBOL(MApi_XC_EX_Get_3D_Output_Mode);
EXPORT_SYMBOL(MApi_XC_EX_Get_3D_Panel_Type);
EXPORT_SYMBOL(MApi_XC_EX_Get_3D_MainWin_First);
EXPORT_SYMBOL(MApi_XC_EX_3DMainSub_IPSync);
EXPORT_SYMBOL(MApi_XC_EX_Set_3D_VerVideoOffset);
EXPORT_SYMBOL(MApi_XC_EX_Get_3D_VerVideoOffset);
EXPORT_SYMBOL(MApi_XC_EX_Is3DFormatSupported);
EXPORT_SYMBOL(MApi_XC_EX_Set_3D_HShift);
EXPORT_SYMBOL(MApi_XC_EX_Enable_3D_LR_Sbs2Line);
EXPORT_SYMBOL(MApi_XC_EX_Get_3D_HShift);
EXPORT_SYMBOL(MApi_XC_EX_Set_3D_HW2DTo3D_Buffer);
EXPORT_SYMBOL(MApi_XC_EX_Set_3D_HW2DTo3D_Parameters);
EXPORT_SYMBOL(MApi_XC_EX_Get_3D_HW2DTo3D_Parameters);
EXPORT_SYMBOL(MApi_XC_EX_Set_3D_Detect3DFormat_Parameters);
EXPORT_SYMBOL(MApi_XC_EX_Get_3D_Detect3DFormat_Parameters);
EXPORT_SYMBOL(MApi_XC_EX_Detect3DFormatByContent);
EXPORT_SYMBOL(MApi_XC_EX_DetectNL);
EXPORT_SYMBOL(MApi_XC_EX_3D_PostPQSetting);
EXPORT_SYMBOL(MApi_XC_EX_Set_3D_FPInfo);
EXPORT_SYMBOL(MApi_XC_EX_EnableAutoDetect3D);
EXPORT_SYMBOL(MApi_XC_EX_GetAutoDetect3DFlag);
EXPORT_SYMBOL(MApi_XC_EX_Set_3D_SubWinClk);
EXPORT_SYMBOL(MApi_XC_EX_3D_Is_LR_Sbs2Line);
EXPORT_SYMBOL(MApi_SC_EX_3D_Is_Skip_Default_LR_Flag);
EXPORT_SYMBOL(MApi_XC_EX_3D_Enable_Skip_Default_LR_Flag);
#ifdef UFO_XC_SCAN_TYPE
#endif
EXPORT_SYMBOL(MApi_XC_EX_Mux_Init);
EXPORT_SYMBOL(MApi_XC_EX_Mux_SourceMonitor);
EXPORT_SYMBOL(MApi_XC_EX_Mux_CreatePath);
EXPORT_SYMBOL(MApi_XC_EX_Mux_DeletePath);
EXPORT_SYMBOL(MApi_XC_EX_Mux_EnablePath);
EXPORT_SYMBOL(MApi_XC_EX_Mux_TriggerPathSyncEvent);
EXPORT_SYMBOL(MApi_XC_EX_Mux_TriggerDestOnOffEvent);
EXPORT_SYMBOL(MApi_XC_EX_Mux_OnOffPeriodicHandler);
EXPORT_SYMBOL(MApi_XC_EX_Mux_GetPathInfo);
EXPORT_SYMBOL(MApi_XC_EX_Mux_SetSupportMhlPathInfo);
EXPORT_SYMBOL(MApi_XC_EX_Mux_SetMhlHotPlugInverseInfo);
EXPORT_SYMBOL(MApi_XC_EX_Mux_GetHDMIPort);
EXPORT_SYMBOL(MApi_XC_EX_MUX_MapInputSourceToVDYMuxPORT);
EXPORT_SYMBOL(MApi_XC_EX_Set_NR);
EXPORT_SYMBOL(MApi_XC_EX_FilmMode_P);
EXPORT_SYMBOL(MApi_XC_EX_GetUCEnabled);
EXPORT_SYMBOL(MApi_XC_EX_GenSpecificTiming);
EXPORT_SYMBOL(MApi_XC_EX_GetDEBypassMode);
EXPORT_SYMBOL(MApi_XC_EX_GetDEWindow);
EXPORT_SYMBOL(MApi_XC_EX_GetDEWidthHeightInDEByPassMode);
EXPORT_SYMBOL(MApi_XC_EX_GetCaptureWindow);
EXPORT_SYMBOL(MApi_XC_EX_SetCaptureWindowVstart);
EXPORT_SYMBOL(MApi_XC_EX_SetCaptureWindowHstart);
EXPORT_SYMBOL(MApi_XC_EX_SetCaptureWindowVsize);
EXPORT_SYMBOL(MApi_XC_EX_SetCaptureWindowHsize);
EXPORT_SYMBOL(MApi_XC_EX_SoftwareReset);
EXPORT_SYMBOL(MApi_XC_EX_CalculateHFreqx10);
EXPORT_SYMBOL(MApi_XC_EX_CalculateHFreqx1K);
EXPORT_SYMBOL(MApi_XC_EX_CalculateVFreqx10);
EXPORT_SYMBOL(MApi_XC_EX_CalculateVFreqx1K);
EXPORT_SYMBOL(MApi_XC_EX_GetAccurateVFreqx1K);
EXPORT_SYMBOL(MApi_XC_EX_InterruptAttach);
EXPORT_SYMBOL(MApi_XC_EX_InterruptDeAttach);
EXPORT_SYMBOL(MApi_XC_EX_DisableInputSource);
EXPORT_SYMBOL(MApi_XC_EX_IsInputSourceDisabled);
EXPORT_SYMBOL(MApi_XC_EX_ChangePanelType);
EXPORT_SYMBOL(MApi_XC_EX_GetCurrentReadBank);
EXPORT_SYMBOL(MApi_XC_EX_GetCurrentWriteBank);
EXPORT_SYMBOL(MApi_XC_EX_SetAutoPreScaling);
EXPORT_SYMBOL(MApi_XC_EX_GetVSyncWidth);
EXPORT_SYMBOL(MApi_XC_EX_set_GOP_Enable);
EXPORT_SYMBOL(MApi_XC_EX_ip_sel_for_gop);
EXPORT_SYMBOL(MApi_XC_EX_SetVOPNBL);
EXPORT_SYMBOL(MApi_XC_EX_GetDstInfo);
EXPORT_SYMBOL(MApi_XC_EX_set_FD_Mask);
EXPORT_SYMBOL(MApi_XC_EX_Get_FD_Mask);
EXPORT_SYMBOL(MApi_XC_EX_set_FD_Mask_byWin);
EXPORT_SYMBOL(MApi_XC_EX_Get_FD_Mask_byWin);
EXPORT_SYMBOL(MApi_XC_EX_Get_FD_Mask_Status);
EXPORT_SYMBOL(MApi_XC_EX_SetIP1TestPattern);
EXPORT_SYMBOL(MApi_XC_EX_InitIPForInternalTiming);
EXPORT_SYMBOL(MApi_XC_EX_SetIPMux);
EXPORT_SYMBOL(MApi_XC_EX_Is_H_Sync_Active);
EXPORT_SYMBOL(MApi_XC_EX_Is_V_Sync_Active);
EXPORT_SYMBOL(MApi_XC_EX_GetAutoPositionWindow);
EXPORT_SYMBOL(MApi_XC_EX_SetFrameBufferAddress);
EXPORT_SYMBOL(MApi_XC_EX_IsFrameBufferEnoughForCusScaling);
EXPORT_SYMBOL(MApi_XC_EX_SetScalerMemoryRequest);
EXPORT_SYMBOL(MApi_XC_EX_Get_PixelData);
EXPORT_SYMBOL(MApi_XC_EX_GetAvailableSize);
EXPORT_SYMBOL(MApi_XC_EX_SetFrameColor);
EXPORT_SYMBOL(MApi_XC_EX_SetDispWindowColor);
EXPORT_SYMBOL(MApi_XC_EX_SupportSourceToVE);
EXPORT_SYMBOL(MApi_XC_EX_SetOutputCapture);
EXPORT_SYMBOL(MApi_XC_EX_SetGammaOnOff);
EXPORT_SYMBOL(MApi_XC_EX_SetPreGammaGain);
EXPORT_SYMBOL(MApi_XC_EX_SetPreGammaOffset);
EXPORT_SYMBOL(MApi_XC_EX_SetPanelTiming);
EXPORT_SYMBOL(MApi_XC_EX_SetOutTimingMode);
EXPORT_SYMBOL(MApi_XC_EX_SetFreeRunTiming);
EXPORT_SYMBOL(MApi_XC_EX_Set_CustomerSyncInfo);
EXPORT_SYMBOL(MApi_XC_EX_WaitFPLLDone);
EXPORT_SYMBOL(MApi_XC_EX_GetOutputVFreqX100);
EXPORT_SYMBOL(MApi_XC_EX_FrameLockCheck);
EXPORT_SYMBOL(MApi_XC_EX_CustomizeFRCTable);
EXPORT_SYMBOL(MApi_XC_EX_OutputFrameCtrl);
EXPORT_SYMBOL(MApi_SC_EX_Enable_FPLL_FSM);
EXPORT_SYMBOL(MApi_SC_EX_ForceFreerun);
EXPORT_SYMBOL(MApi_SC_EX_IsForceFreerun);
EXPORT_SYMBOL(MApi_SC_EX_SetFreerunVFreq);
EXPORT_SYMBOL(MApi_XC_EX_SetExPanelInfo);
EXPORT_SYMBOL(MApi_XC_EX_Enable_FPLL_Thresh_Mode);
EXPORT_SYMBOL(MApi_XC_EX_EnableIPAutoNoSignal);
EXPORT_SYMBOL(MApi_XC_EX_GetIPAutoNoSignal);
EXPORT_SYMBOL(MApi_XC_EX_EnableIPAutoCoast);
EXPORT_SYMBOL(MApi_XC_EX_EnableIPCoastDebounce);
EXPORT_SYMBOL(MApi_XC_EX_ClearIPCoastStatus);
EXPORT_SYMBOL(MApi_XC_EX_EnableFpllManualSetting);
EXPORT_SYMBOL(MApi_XC_EX_FpllBoundaryTest);
EXPORT_SYMBOL(MApi_XC_EX_SetOffLineDetection);
EXPORT_SYMBOL(MApi_XC_EX_GetOffLineDetection);
EXPORT_SYMBOL(MApi_XC_EX_SetOffLineSogThreshold);
EXPORT_SYMBOL(MApi_XC_EX_SetOffLineSogBW);
EXPORT_SYMBOL(MApi_XC_EX_OffLineInit);
EXPORT_SYMBOL(MApi_XC_EX_Set_Extra_fetch_adv_line);
EXPORT_SYMBOL(MApi_XC_EX_SetVGASogEn);
EXPORT_SYMBOL(MApi_XC_EX_EnableWindow);
EXPORT_SYMBOL(MApi_XC_EX_Is_SubWindowEanble);
EXPORT_SYMBOL(MApi_XC_EX_SetBorderFormat);
EXPORT_SYMBOL(MApi_XC_EX_EnableBorder);
EXPORT_SYMBOL(MApi_XC_EX_ZorderMainWindowFirst);
EXPORT_SYMBOL(MApi_XC_EX_PQ_LoadFunction);
EXPORT_SYMBOL(MApi_XC_EX_Check_HNonLinearScaling);
EXPORT_SYMBOL(MApi_XC_EX_EnableEuroHdtvSupport);
EXPORT_SYMBOL(MApi_XC_EX_EnableEuroHdtvDetection);
EXPORT_SYMBOL(MApi_XC_EX_ReadByte);
EXPORT_SYMBOL(MApi_XC_EX_WriteByte);
EXPORT_SYMBOL(MApi_XC_EX_WriteByteMask);
EXPORT_SYMBOL(MApi_XC_EX_Write2ByteMask);
EXPORT_SYMBOL(MApi_XC_EX_W2BYTE);
EXPORT_SYMBOL(MApi_XC_EX_R2BYTE);
EXPORT_SYMBOL(MApi_XC_EX_W4BYTE);
EXPORT_SYMBOL(MApi_XC_EX_R4BYTE);
EXPORT_SYMBOL(MApi_XC_EX_R2BYTEMSK);
EXPORT_SYMBOL(MApi_XC_EX_W2BYTEMSK);
EXPORT_SYMBOL(MApi_XC_EX_MLoad_Init);
EXPORT_SYMBOL(MApi_XC_EX_MLoad_Enable);
EXPORT_SYMBOL(MApi_XC_EX_MLoad_GetStatus);
EXPORT_SYMBOL(MApi_XC_EX_MLoad_WriteCmd_And_Fire);
EXPORT_SYMBOL(MApi_XC_EX_MLoad_WriteCmds_And_Fire);
EXPORT_SYMBOL(MApi_XC_EX_MLG_Init);
EXPORT_SYMBOL(MApi_XC_EX_MLG_Enable);
EXPORT_SYMBOL(MApi_XC_EX_MLG_GetCaps);
EXPORT_SYMBOL(MApi_XC_EX_MLG_GetStatus);
EXPORT_SYMBOL(MApi_XC_EX_SetOSD2VEMode);
EXPORT_SYMBOL(MApi_XC_EX_IP2_PreFilter_Enable);
EXPORT_SYMBOL(MApi_XC_EX_Get_Pixel_RGB);
EXPORT_SYMBOL(MApi_XC_EX_KeepPixelPointerAppear);
EXPORT_SYMBOL(MApi_XC_EX_Set_MemFmtEx);
EXPORT_SYMBOL(MApi_XC_EX_IsRequestFrameBufferLessMode);
EXPORT_SYMBOL(MApi_XC_EX_SkipSWReset);
EXPORT_SYMBOL(MApi_XC_EX_EnableRepWindowForFrameColor);
EXPORT_SYMBOL(MApi_XC_EX_SetOSDLayer);
EXPORT_SYMBOL(MApi_XC_EX_GetOSDLayer);
EXPORT_SYMBOL(MApi_XC_EX_SetVideoAlpha);
EXPORT_SYMBOL(MApi_XC_EX_GetVideoAlpha);
EXPORT_SYMBOL(MApi_XC_EX_SkipWaitVsync);
EXPORT_SYMBOL(MApi_XC_EX_OP2VOPDESel);
EXPORT_SYMBOL(MApi_XC_EX_FRC_SetWindow);
EXPORT_SYMBOL(MApi_XC_EX_Enable_TwoInitFactor);
EXPORT_SYMBOL(MApi_XC_EX_IsFieldPackingModeSupported);
EXPORT_SYMBOL(MApi_XC_EX_PreInit);
EXPORT_SYMBOL(MApi_XC_EX_Get_BufferData);
EXPORT_SYMBOL(MApi_XC_EX_Set_BufferData);
EXPORT_SYMBOL(MApi_XC_EX_EnableMainWindow);
EXPORT_SYMBOL(MApi_XC_EX_EnableSubWindow);
EXPORT_SYMBOL(MApi_XC_EX_DisableSubWindow);
EXPORT_SYMBOL(MApi_XC_EX_SetPixelShift);
EXPORT_SYMBOL(MApi_XC_EX_SetVideoOnOSD);
EXPORT_SYMBOL(MApi_XC_EX_SetOSDBlendingFormula);
EXPORT_SYMBOL(MApi_XC_EX_ReportPixelInfo);
EXPORT_SYMBOL(MApi_XC_EX_SetScaling);
EXPORT_SYMBOL(MApi_XC_EX_SetMCDIBufferAddress);
EXPORT_SYMBOL(MApi_XC_EX_EnableMCDI);
EXPORT_SYMBOL(MApi_XC_EX_SendCmdToFRC);
EXPORT_SYMBOL(MApi_XC_EX_GetMsgFromFRC);
EXPORT_SYMBOL(MApi_XC_EX_EnableRWBankAuto);
EXPORT_SYMBOL(MApi_XC_EX_SetWRBankMappingNum);
EXPORT_SYMBOL(MApi_XC_EX_GetWRBankMappingNum);
EXPORT_SYMBOL(MApi_XC_EX_GetWRBankMappingNumForZap);
EXPORT_SYMBOL(MApi_XC_EX_SetBOBMode);
EXPORT_SYMBOL(MApi_XC_EX_SetForceReadBank);
EXPORT_SYMBOL(MApi_XC_EX_LD_Init);
EXPORT_SYMBOL(MApi_XC_EX_LD_SetMemoryAddress);
EXPORT_SYMBOL(MApi_XC_EX_LD_SetLevel);
EXPORT_SYMBOL(MApi_XC_EX_Set_TurnoffLDBL);
EXPORT_SYMBOL(MApi_XC_EX_Set_notUpdateSPIDataFlags);
EXPORT_SYMBOL(MApi_XC_EX_Set_UsermodeLDFlags);
EXPORT_SYMBOL(MApi_XC_EX_Set_BLLevel);
EXPORT_SYMBOL(MApi_XC_EX_Set_BWS_Mode);
EXPORT_SYMBOL(MApi_XC_EX_FRC_ColorPathCtrl);
EXPORT_SYMBOL(MApi_XC_EX_FRC_OP2_SetRGBGain);
EXPORT_SYMBOL(MApi_XC_EX_FRC_OP2_SetRGBOffset);
EXPORT_SYMBOL(MApi_XC_EX_FRC_OP2_SetDither);
EXPORT_SYMBOL(MApi_XC_EX_FRC_BypassMFC);
EXPORT_SYMBOL(MApi_XC_EX_ForceReadFrame);
EXPORT_SYMBOL(MApi_XC_EX_SetCsc);
EXPORT_SYMBOL(MApi_XC_EX_RegisterPQSetFPLLThreshMode);
EXPORT_SYMBOL(MApi_XC_EX_GetFreeRunStatus);
EXPORT_SYMBOL(MApi_XC_EX_Get_DSForceIndexSupported);
EXPORT_SYMBOL(MApi_XC_EX_Set_DSForceIndex);
EXPORT_SYMBOL(MApi_XC_EX_Set_DSIndexSourceSelect);
EXPORT_SYMBOL(MApi_XC_EX_OSDC_InitSetting);
EXPORT_SYMBOL(MApi_XC_EX_OSDC_Control);
EXPORT_SYMBOL(MApi_XC_EX_OSDC_GetDstInfo);
EXPORT_SYMBOL(MApi_XC_EX_OSDC_SetOutVfreqx10);
EXPORT_SYMBOL(MApi_XC_EX_BYPASS_SetOSDVsyncPos);
EXPORT_SYMBOL(MApi_XC_EX_BYPASS_SetInputSrc);
EXPORT_SYMBOL(MApi_XC_EX_BYPASS_SetCSC);
EXPORT_SYMBOL(MApi_XC_EX_SetSeamlessZapping);
EXPORT_SYMBOL(MApi_XC_EX_Vtrack_SetPayloadData);
EXPORT_SYMBOL(MApi_XC_EX_Vtrack_SetUserDefindedSetting);
EXPORT_SYMBOL(MApi_XC_EX_Vtrack_Enable);
EXPORT_SYMBOL(MApi_XC_EX_PreSetPQInfo);
EXPORT_SYMBOL(MApi_XC_EX_Is_OP1_TestPattern_Enabled);
EXPORT_SYMBOL(MApi_XC_EX_Set_OP1_TestPattern);
EXPORT_SYMBOL(MApi_XC_EX_Set_WhiteBalance_Pattern);
#ifdef __cplusplus
#endif
#endif
#ifdef _DMX_TSIO_V2_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRVRASP_H_
#ifdef __cplusplus
#endif
#ifdef HW_PVR_ENABLE
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if 0
#endif
#ifdef UFO_PUBLIC_HEADER_500_3
#else
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef __MDRV_URDMA_H__
#ifdef __cplusplus
#endif
#ifdef MS_DEBUG
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_DVBT_H_
#ifdef __cplusplus
#endif
#ifdef UFO_DEMOD_DVBT_SUPPORT_DMD_INT
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_HDMITX_H_
#ifdef __cplusplus
#endif
EXPORT_SYMBOL(MApi_HDMITx_Init);
EXPORT_SYMBOL(MApi_HDMITx_Exit);
EXPORT_SYMBOL(MApi_HDMITx_TurnOnOff);
EXPORT_SYMBOL(MApi_HDMITx_EnablePacketGen);
EXPORT_SYMBOL(MApi_HDMITx_SetHDMITxMode);
EXPORT_SYMBOL(MApi_HDMITx_SetHDMITxMode_CD);
EXPORT_SYMBOL(MApi_HDMITx_SetTMDSOnOff);
EXPORT_SYMBOL(MApi_HDMITx_DisableTMDSCtrl);
EXPORT_SYMBOL(MApi_HDMITx_SetRBChannelSwap);
EXPORT_SYMBOL(MApi_HDMITx_Exhibit);
EXPORT_SYMBOL(MApi_HDMITx_ForceHDMIOutputMode);
EXPORT_SYMBOL(MApi_HDMITx_ForceHDMIOutputColorFormat);
EXPORT_SYMBOL(MApi_HDMITx_GetRxStatus);
EXPORT_SYMBOL(MApi_HDMITx_GetRxDCInfoFromEDID);
EXPORT_SYMBOL(MApi_HDMITx_GetRxVideoFormatFromEDID);
EXPORT_SYMBOL(MApi_HDMITx_GetVICListFromEDID);
EXPORT_SYMBOL(MApi_HDMITx_GetDataBlockLengthFromEDID);
EXPORT_SYMBOL(MApi_HDMITx_GetRxAudioFormatFromEDID);
EXPORT_SYMBOL(MApi_HDMITx_EDID_HDMISupport);
EXPORT_SYMBOL(MApi_HDMITx_GetRxIDManufacturerName);
EXPORT_SYMBOL(MApi_HDMITx_GetBksv);
EXPORT_SYMBOL(MApi_HDMITx_GetAksv);
EXPORT_SYMBOL(MApi_HDMITx_GetEDIDData);
EXPORT_SYMBOL(MApi_HDMITx_GetRx3DStructureFromEDID);
EXPORT_SYMBOL(MApi_HDMITx_GetColorFormatFromEDID);
EXPORT_SYMBOL(MApi_HDMITx_PKT_User_Define_Clear);
EXPORT_SYMBOL(MApi_HDMITx_PKT_User_Define);
EXPORT_SYMBOL(MApi_HDMITx_PKT_Content_Define);
EXPORT_SYMBOL(MApi_HDMITx_SetVideoOnOff);
EXPORT_SYMBOL(MApi_HDMITx_SetColorFormat);
EXPORT_SYMBOL(MApi_HDMITx_SetVideoOutputTiming);
EXPORT_SYMBOL(MApi_HDMITx_SetVideoOutputTimingByCustomer);
EXPORT_SYMBOL(MApi_HDMITx_SetVideoOutputAsepctRatio);
EXPORT_SYMBOL(MApi_HDMITx_SetVideoOutputOverscan_AFD);
EXPORT_SYMBOL(MApi_HDMITx_SetVideoOutputOverscan_AFD_II);
EXPORT_SYMBOL(MApi_HDMITx_Set_VS_InfoFrame);
EXPORT_SYMBOL(MApi_HDMITx_SetAVIInfoExtColorimetry);
EXPORT_SYMBOL(MApi_HDMITx_SetAudioOnOff);
EXPORT_SYMBOL(MApi_HDMITx_SetAudioFrequency);
EXPORT_SYMBOL(MApi_HDMITx_SetAudioConfiguration);
EXPORT_SYMBOL(MApi_HDMITx_GetAudioCTS);
EXPORT_SYMBOL(MApi_HDMITx_MuteAudioFIFO);
EXPORT_SYMBOL(MApi_HDMITx_SetAudioSourceFormat);
EXPORT_SYMBOL(MApi_HDMITx_GetHdcpKey);
EXPORT_SYMBOL(MApi_HDMITx_SetHDCPOnOff);
EXPORT_SYMBOL(MApi_HDMITx_SetAVMUTE);
EXPORT_SYMBOL(MApi_HDMITx_GetAVMUTEStatus);
EXPORT_SYMBOL(MApi_HDMITx_GetHDCPStatus);
EXPORT_SYMBOL(MApi_HDMITx_HDCP_StartAuth);
EXPORT_SYMBOL(MApi_HDMITx_GetINTHDCPStatus);
EXPORT_SYMBOL(MApi_HDMITx_GetHDCP_PreStatus);
EXPORT_SYMBOL(MApi_HDMITx_UnHDCPRxControl);
EXPORT_SYMBOL(MApi_HDMITx_HDCPRxFailControl);
EXPORT_SYMBOL(MApi_HDMITx_SetAksv2R0Interval);
EXPORT_SYMBOL(MApi_HDMITx_IsHDCPRxValid);
EXPORT_SYMBOL(MApi_HDMITx_HDCP_RevocationKey_Check);
EXPORT_SYMBOL(MApi_HDMITx_HDCP_RevocationKey_List);
EXPORT_SYMBOL(MApi_HDMITx_GetLibVer);
EXPORT_SYMBOL(MApi_HDMITx_GetInfo);
EXPORT_SYMBOL(MApi_HDMITx_GetStatus);
EXPORT_SYMBOL(MApi_HDMITx_HDCP_IsSRMSignatureValid);
EXPORT_SYMBOL(MApi_HDMITx_SetDbgLevel);
EXPORT_SYMBOL(MApi_HDMITx_SetHPDGpioPin);
EXPORT_SYMBOL(MApi_HDMITx_AnalogTuning);
EXPORT_SYMBOL(MApi_HDMITx_DisableRegWrite);
EXPORT_SYMBOL(MApi_HDMITx_GetEDIDPhyAdr);
EXPORT_SYMBOL(MApi_HDMITx_SetCECOnOff);
EXPORT_SYMBOL(MApi_HDMITx_GetCECStatus);
EXPORT_SYMBOL(MApi_HDMITx_EdidChecking);
EXPORT_SYMBOL(MApi_HDMITx_RxBypass_Mode);
EXPORT_SYMBOL(MApi_HDMITx_Disable_RxBypass);
EXPORT_SYMBOL(MApi_HDMITx_GetChipCaps);
EXPORT_SYMBOL(MApi_HDMITx_SetPowerState);
EXPORT_SYMBOL(MApi_HDMITx_GetEdidDataBlocks);
EXPORT_SYMBOL(MApi_HDMITx_GetKSVList);
EXPORT_SYMBOL(MApi_HDMITx_GeneralCtrl);
EXPORT_SYMBOL(MApi_HDMITx_HDCP2AccessX74);
EXPORT_SYMBOL(MApi_HDMITx_HDCP2TxInit);
EXPORT_SYMBOL(MApi_HDMITx_HDCP2TxEnableEncrypt);
EXPORT_SYMBOL(MApi_HDMITx_HDCP2TxFillCipherKey);
#ifdef __cplusplus
#endif
#endif
#ifdef _XC_AUTO_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_AVD_H_
#ifdef __cplusplus
#endif
#if !defined (__aarch64__)
#endif
#if !defined (__aarch64__)
#endif
#if 0
#endif
#if 0
#endif
#if 0
#endif
#if 0
#endif
#if 0
#endif
#if 0
#endif
#if 0
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_DTMB_V2_H_
#ifdef __cplusplus
#endif
#ifndef DLL_PUBLIC
#endif
#ifdef UTPA2
#else
#endif
#ifdef UTPA2
#else
#endif
#ifdef UTPA2
#else
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRVAUDIO_H_
#ifdef __cplusplus
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
EXPORT_SYMBOL(MDrv_AUD_GetLibVer);
EXPORT_SYMBOL(MDrv_AUDIO_ReadReg);
EXPORT_SYMBOL(MDrv_AUDIO_WriteReg);
EXPORT_SYMBOL(MDrv_AUDIO_WriteMaskReg);
EXPORT_SYMBOL(MDrv_AUDIO_ReadByte);
EXPORT_SYMBOL(MDrv_AUDIO_DecReadByte);
EXPORT_SYMBOL(MDrv_AUDIO_SeReadByte);
EXPORT_SYMBOL(MDrv_AUDIO_WriteByte);
EXPORT_SYMBOL(MDrv_AUDIO_DecWriteByte);
EXPORT_SYMBOL(MDrv_AUDIO_SeWriteByte);
EXPORT_SYMBOL(MDrv_AUDIO_WriteMaskByte);
EXPORT_SYMBOL(MDrv_AUDIO_DecWriteMaskByte);
EXPORT_SYMBOL(MDrv_AUDIO_SeWriteMaskByte);
EXPORT_SYMBOL(MDrv_AUDIO_ReadDecMailBox);
EXPORT_SYMBOL(MDrv_AUDIO_WriteDecMailBox);
EXPORT_SYMBOL(MDrv_AUDIO_ReadSeMailBox);
EXPORT_SYMBOL(MDrv_AUDIO_WriteSeMailBox);
EXPORT_SYMBOL(MDrv_AUDIO_InitMMIO);
EXPORT_SYMBOL(MDrv_AUDIO_AttachInterrupt);
EXPORT_SYMBOL(MDrv_AUDIO_Mutex_Init);
EXPORT_SYMBOL(MDrv_AUDIO_SHM_Init);
EXPORT_SYMBOL(MDrv_AUDIO_Init);
EXPORT_SYMBOL(MDrv_AUDIO_Close);
EXPORT_SYMBOL(MDrv_AUDIO_Open);
EXPORT_SYMBOL(MDrv_AUDIO_WriteInitTable);
EXPORT_SYMBOL(MDrv_AUDIO_WritePreInitTable);
EXPORT_SYMBOL(MDrv_AUDIO_EnaEarphone_LowPower_Stage);
EXPORT_SYMBOL(MDrv_AUDIO_EnaEarphone_HighDriving_Stage);
EXPORT_SYMBOL(MDrv_AUDIO_SwResetMAD);
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MDrv_AUDIO_SetDspBaseAddr);
#else
EXPORT_SYMBOL(MDrv_AUDIO_SetDspBaseAddr);
#endif
EXPORT_SYMBOL(MDrv_AUDIO_GetDspBinBaseAddr);
EXPORT_SYMBOL(MDrv_AUDIO_GetDspMadBaseAddr);
EXPORT_SYMBOL(MDrv_AUDIO_DspBootOnDDR);
EXPORT_SYMBOL(MDrv_AUDIO_DecoderLoadCode);
EXPORT_SYMBOL(MDrv_AUDIO_SeSystemLoadCode);
EXPORT_SYMBOL(MDrv_AUDIO_DspReboot);
EXPORT_SYMBOL(MDrv_AUDIO_RebootDecDSP);
EXPORT_SYMBOL(MDrv_AUDIO_TriggerSifPLL);
EXPORT_SYMBOL(MDrv_AUDIO_SetPlayFileFlag);
EXPORT_SYMBOL(MDrv_AUDIO_GetPlayFileFlag);
EXPORT_SYMBOL(MDrv_AUDIO_InitialVars);
EXPORT_SYMBOL(MDrv_AUDIO_SET_INIT_FLAG);
EXPORT_SYMBOL(MDrv_AUDIO_GET_INIT_FLAG);
EXPORT_SYMBOL(MDrv_AUDIO_ResetDspCodeType);
EXPORT_SYMBOL(MDrv_AUDIO_GetPathGroup);
EXPORT_SYMBOL(MDrv_AUDIO_SetNormalPath);
EXPORT_SYMBOL(MDrv_AUDIO_SetInputPath);
EXPORT_SYMBOL(MDrv_AUDIO_SetOutputPath);
EXPORT_SYMBOL(MDrv_AUDIO_SetInternalPath);
EXPORT_SYMBOL(MDrv_AUDIO_ReadDspCodeType);
EXPORT_SYMBOL(MDrv_AUDIO_BT_SetUploadRate);
EXPORT_SYMBOL(MDrv_AUDIO_BT_SetBufferCounter);
EXPORT_SYMBOL(MDrv_AUDIO_ReadDspCounter);
EXPORT_SYMBOL(MDrv_AUDIO_SetPowerDownWait);
EXPORT_SYMBOL(MDrv_AUDIO_IsDTV);
EXPORT_SYMBOL(MDrv_AUDIO_SetFading);
EXPORT_SYMBOL(MDrv_AUDIO_SPDIF_SetMute);
EXPORT_SYMBOL(MDrv_AUDIO_SPDIF_SetMode);
EXPORT_SYMBOL(MDrv_AUDIO_SPDIF_ByPassChannel);
EXPORT_SYMBOL(MDrv_AUDIO_SPDIF_SetChannelStatus);
EXPORT_SYMBOL(MDrv_AUDIO_SPDIF_Monitor_SamplingRate);
EXPORT_SYMBOL(MDrv_AUDIO_HDMI_Tx_GetStatus);
EXPORT_SYMBOL(MDrv_AUDIO_HDMI_DolbyMonitor);
EXPORT_SYMBOL(MDrv_AUDIO_HDMI_AC3_PathCFG);
EXPORT_SYMBOL(MDrv_AUDIO_HDMI_TX_SetMode);
EXPORT_SYMBOL(MDrv_AUDIO_AdcInit);
EXPORT_SYMBOL(MDrv_AUDIO_GetDecoderStatus);
EXPORT_SYMBOL(MDrv_AUDIO_GetUniDecodeDoneTag);
EXPORT_SYMBOL(MDrv_AUDIO_update_DspUsage);
EXPORT_SYMBOL(MDrv_AUDIO_ALSA_Enable);
EXPORT_SYMBOL(MDrv_AUDIO_DumpDspInfo);
EXPORT_SYMBOL(MDrv_AUDIO_GetCaps);
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_STBDC_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_MPIF_H_
#ifdef __cplusplus
#endif
#ifdef MPIF_SPI_SUPPORT
#endif
#ifdef MPIF_SPI_SUPPORT
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_VDEC_EX_H_
#ifdef __cplusplus
#endif
#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE)) || defined(SUPPORT_X_MODEL_FEATURE)
#if defined(UFO_PUBLIC_HEADER_300)
#elif defined(UFO_PUBLIC_HEADER_212)
#elif defined(UFO_PUBLIC_HEADER_500_3)
#else
#if defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_700)||defined(UFO_PUBLIC_HEADER_500)
#endif
#if defined(UFO_PUBLIC_HEADER_500)
#endif
#endif
#ifndef VDEC_CAP_SYSTEM_PREGET_FB_MEM_USAGE_SIZE
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_500)
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#endif
#if defined(UFO_PUBLIC_HEADER_212)
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_500)
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#endif
#if defined(UFO_PUBLIC_HEADER_212)
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#endif
#if (defined(UFO_PUBLIC_HEADER_500)||defined(UFO_PUBLIC_HEADER_500_3))
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#endif
#if defined(UFO_PUBLIC_HEADER_212)
#endif
#ifdef VDEC_CAP_DV_OTT_API
#endif
#ifdef VDEC_CAP_DV_OTT_API
#ifdef VDEC_CAP_DV_OTT_API_V2
#else
#endif
#endif
#ifdef VDEC_CAP_HEVC_HDR
#endif
#if !(defined(UFO_PUBLIC_HEADER_500)||defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_500_3))
#endif
#ifdef VDEC_CAP_HEVC_HDR_V2
#endif
#ifdef VDEC_CAP_HEVC_HDR_V3
#endif
#ifdef VDEC_CAP_FRAME_INFO_EXT_V4
#endif
#ifdef VDEC_CAP_FRAME_INFO_EXT_V5
#endif
#ifdef VDEC_CAP_FRAME_INFO_EXT_V6
#endif
#ifdef VDEC_CAP_FRAME_INFO_EXT_V7
#endif
EXPORT_SYMBOL(MApi_VDEC_EX_GetLibVer);
EXPORT_SYMBOL(MApi_VDEC_EX_GetInfo);
EXPORT_SYMBOL(MApi_VDEC_EX_GetStatus);
EXPORT_SYMBOL(MApi_VDEC_EX_EnableTurboMode);
EXPORT_SYMBOL(MApi_VDEC_EX_CheckCaps);
EXPORT_SYMBOL(MApi_VDEC_EX_SetSingleDecode);
EXPORT_SYMBOL(MApi_VDEC_EX_GetFreeStream);
EXPORT_SYMBOL(MApi_VDEC_EX_GetConfig);
EXPORT_SYMBOL(MApi_VDEC_EX_Init);
EXPORT_SYMBOL(MApi_VDEC_EX_Rst);
EXPORT_SYMBOL(MApi_VDEC_EX_Exit);
EXPORT_SYMBOL(MApi_VDEC_EX_CheckDispInfoRdy);
EXPORT_SYMBOL(MApi_VDEC_EX_SetFrcMode);
EXPORT_SYMBOL(MApi_VDEC_EX_SetDynScalingParams);
EXPORT_SYMBOL(MApi_VDEC_EX_SetDbgLevel);
EXPORT_SYMBOL(MApi_VDEC_EX_Play);
EXPORT_SYMBOL(MApi_VDEC_EX_Pause);
EXPORT_SYMBOL(MApi_VDEC_EX_Resume);
EXPORT_SYMBOL(MApi_VDEC_EX_StepDisp);
EXPORT_SYMBOL(MApi_VDEC_EX_IsStepDispDone);
EXPORT_SYMBOL(MApi_VDEC_EX_StepDecode);
EXPORT_SYMBOL(MApi_VDEC_EX_IsStepDecodeDone);
EXPORT_SYMBOL(MApi_VDEC_EX_SetTrickMode);
EXPORT_SYMBOL(MApi_VDEC_EX_PushDecQ);
EXPORT_SYMBOL(MApi_VDEC_EX_Flush);
EXPORT_SYMBOL(MApi_VDEC_EX_EnableLastFrameShow);
EXPORT_SYMBOL(MApi_VDEC_EX_SetSpeed);
EXPORT_SYMBOL(MApi_VDEC_EX_SetFreezeDisp);
EXPORT_SYMBOL(MApi_VDEC_EX_SetBlueScreen);
EXPORT_SYMBOL(MApi_VDEC_EX_ResetPTS);
EXPORT_SYMBOL(MApi_VDEC_EX_AVSyncOn);
EXPORT_SYMBOL(MApi_VDEC_EX_SetAVSyncFreerunThreshold);
EXPORT_SYMBOL(MApi_VDEC_EX_GetDispInfo);
EXPORT_SYMBOL(MApi_VDEC_EX_IsAVSyncOn);
EXPORT_SYMBOL(MApi_VDEC_EX_IsWithValidStream);
EXPORT_SYMBOL(MApi_VDEC_EX_IsDispFinish);
EXPORT_SYMBOL(MApi_VDEC_EX_IsFrameRdy);
EXPORT_SYMBOL(MApi_VDEC_EX_IsIFrameFound);
EXPORT_SYMBOL(MApi_VDEC_EX_IsSeqChg);
EXPORT_SYMBOL(MApi_VDEC_EX_GetActiveSrcMode);
EXPORT_SYMBOL(MApi_VDEC_EX_IsReachSync);
EXPORT_SYMBOL(MApi_VDEC_EX_IsStartSync);
EXPORT_SYMBOL(MApi_VDEC_EX_IsFreerun);
EXPORT_SYMBOL(MApi_VDEC_EX_IsWithLowDelay);
EXPORT_SYMBOL(MApi_VDEC_EX_IsAllBufferEmpty);
EXPORT_SYMBOL(MApi_VDEC_EX_GetExtDispInfo);
EXPORT_SYMBOL(MApi_VDEC_EX_GetDecFrameInfo);
EXPORT_SYMBOL(MApi_VDEC_EX_GetDispFrameInfo);
EXPORT_SYMBOL(MApi_VDEC_EX_GetDecTimeCode);
EXPORT_SYMBOL(MApi_VDEC_EX_GetDispTimeCode);
EXPORT_SYMBOL(MApi_VDEC_EX_SetEvent);
EXPORT_SYMBOL(MApi_VDEC_EX_UnsetEvent);
EXPORT_SYMBOL(MApi_VDEC_EX_SetEvent_MultiCallback);
EXPORT_SYMBOL(MApi_VDEC_EX_UnsetEvent_MultiCallback);
EXPORT_SYMBOL(MApi_VDEC_EX_GetEventInfo);
EXPORT_SYMBOL(MApi_VDEC_EX_FireDecCmd);
EXPORT_SYMBOL(MApi_VDEC_EX_SeekToPTS);
EXPORT_SYMBOL(MApi_VDEC_EX_SkipToPTS);
EXPORT_SYMBOL(MApi_VDEC_EX_DisableDeblocking);
EXPORT_SYMBOL(MApi_VDEC_EX_DisableQuarterPixel);
EXPORT_SYMBOL(MApi_VDEC_EX_SetAutoRmLstZeroByte);
EXPORT_SYMBOL(MApi_VDEC_EX_SetBalanceBW);
EXPORT_SYMBOL(MApi_VDEC_EX_GetActiveFormat);
EXPORT_SYMBOL(MApi_VDEC_EX_GetColourPrimaries);
EXPORT_SYMBOL(MApi_VDEC_EX_GetFwVersion);
EXPORT_SYMBOL(MApi_VDEC_EX_GetGOPCnt);
EXPORT_SYMBOL(MApi_VDEC_EX_GetESWritePtr);
EXPORT_SYMBOL(MApi_VDEC_EX_GetESReadPtr);
EXPORT_SYMBOL(MApi_VDEC_EX_GetPTS);
EXPORT_SYMBOL(MApi_VDEC_EX_GetNextPTS);
EXPORT_SYMBOL(MApi_VDEC_EX_GetVideoPtsStcDelta);
EXPORT_SYMBOL(MApi_VDEC_EX_GetErrCode);
EXPORT_SYMBOL(MApi_VDEC_EX_GetErrCnt);
EXPORT_SYMBOL(MApi_VDEC_EX_GetBitsRate);
EXPORT_SYMBOL(MApi_VDEC_EX_GetFrameCnt);
EXPORT_SYMBOL(MApi_VDEC_EX_GetSkipCnt);
EXPORT_SYMBOL(MApi_VDEC_EX_GetDropCnt);
EXPORT_SYMBOL(MApi_VDEC_EX_GetDispCnt);
EXPORT_SYMBOL(MApi_VDEC_EX_GetDecQVacancy);
EXPORT_SYMBOL(MApi_VDEC_EX_Is32PullDown);
EXPORT_SYMBOL(MApi_VDEC_EX_IsAlive);
EXPORT_SYMBOL(MApi_VDEC_EX_IsCCAvailable);
EXPORT_SYMBOL(MApi_VDEC_EX_GetCCInfo);
EXPORT_SYMBOL(MApi_VDEC_EX_GetTrickMode);
EXPORT_SYMBOL(MApi_VDEC_EX_GetActiveCodecType);
EXPORT_SYMBOL(MApi_VDEC_EX_GenPattern);
EXPORT_SYMBOL(MApi_VDEC_EX_GetPatternLeastLength);
EXPORT_SYMBOL(MApi_VDEC_EX_MHEG_DecodeIFrame);
EXPORT_SYMBOL(MApi_VDEC_EX_MHEG_IsIFrameDecoding);
EXPORT_SYMBOL(MApi_VDEC_EX_MHEG_RstIFrameDec);
EXPORT_SYMBOL(MApi_VDEC_EX_CC_StartParsing);
EXPORT_SYMBOL(MApi_VDEC_EX_CC_StopParsing);
EXPORT_SYMBOL(MApi_VDEC_EX_CC_GetWritePtr);
EXPORT_SYMBOL(MApi_VDEC_EX_CC_GetReadPtr);
EXPORT_SYMBOL(MApi_VDEC_EX_CC_UpdateReadPtr);
EXPORT_SYMBOL(MApi_VDEC_EX_CC_GetIsOverflow);
EXPORT_SYMBOL(MApi_VDEC_EX_DbgCmd);
EXPORT_SYMBOL(MApi_VDEC_EX_DbgSetData);
EXPORT_SYMBOL(MApi_VDEC_EX_DbgGetData);
EXPORT_SYMBOL(MApi_VDEC_EX_SetControl);
EXPORT_SYMBOL(MApi_VDEC_EX_GetControl);
EXPORT_SYMBOL(MApi_VDEC_EX_GetHWKey);
EXPORT_SYMBOL(MApi_VDEC_EX_DbgDumpStatus);
EXPORT_SYMBOL(MApi_VDEC_EX_DbgGetProgCnt);
EXPORT_SYMBOL(MApi_VDEC_EX_SetBlockDisplay);
EXPORT_SYMBOL(MApi_VDEC_EX_EnableESBuffMalloc);
EXPORT_SYMBOL(MApi_VDEC_EX_GetESBuffVacancy);
EXPORT_SYMBOL(MApi_VDEC_EX_GetESBuff);
EXPORT_SYMBOL(MApi_VDEC_EX_GetNextDispFrame);
EXPORT_SYMBOL(MApi_VDEC_EX_DisplayFrame);
EXPORT_SYMBOL(MApi_VDEC_EX_ReleaseFrame);
EXPORT_SYMBOL(MApi_VDEC_EX_CaptureFrame);
EXPORT_SYMBOL(MApi_VDEC_EX_CC_Init);
EXPORT_SYMBOL(MApi_VDEC_EX_CC_SetCfg);
EXPORT_SYMBOL(MApi_VDEC_EX_CC_SetBuffStartAdd);
EXPORT_SYMBOL(MApi_VDEC_EX_CC_UpdateWriteAdd);
EXPORT_SYMBOL(MApi_VDEC_EX_CC_UpdateReadAdd);
EXPORT_SYMBOL(MApi_VDEC_EX_CC_DisableParsing);
EXPORT_SYMBOL(MApi_VDEC_EX_CC_GetInfo);
EXPORT_SYMBOL(MApi_VDEC_EX_CC_GetIsRstDone);
EXPORT_SYMBOL(MApi_VDEC_EX_CC_GetIsBuffOverflow);
EXPORT_SYMBOL(MApi_VDEC_EX_CC_GetWriteAdd);
EXPORT_SYMBOL(MApi_VDEC_EX_CC_GetReadAdd);
EXPORT_SYMBOL(MApi_VDEC_EX_SystemPreSetControl);
#ifdef VDEC_CAP_SYSTEM_PREGET_API
EXPORT_SYMBOL(MApi_VDEC_EX_SystemPreGetControl);
#endif
EXPORT_SYMBOL(MApi_VDEC_EX_SystemPostSetControl);
EXPORT_SYMBOL(MApi_VDEC_EX_PreSetControl);
#ifdef AUTO_ASSIGN_FRAMEBUFFER
#endif
#else
#if defined(UFO_PUBLIC_HEADER_700) || defined(UFO_PUBLIC_HEADER_500)
#endif
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _VDEC_EX_V2_H_
#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE)) || defined(SUPPORT_X_MODEL_FEATURE)
#ifdef __cplusplus
#endif
#ifndef VDEC_CAP_SYSTEM_PREGET_FB_MEM_USAGE_SIZE
#endif
#ifdef VDEC_CAP_DV_OTT_API
#endif
#ifdef VDEC_CAP_SYSTEM_PREGET_API
#ifdef VDEC_CAP_DV_OTT_API
#endif
#endif
#ifdef __cplusplus
#endif
#endif
#endif
#ifdef _API_XC_PC_MONITOR_H_
#ifdef __cplusplus
#endif
EXPORT_SYMBOL(MApi_XC_PCMonitor_Init);
EXPORT_SYMBOL(MApi_XC_PCMonitor_Restart);
EXPORT_SYMBOL(MApi_XC_PCMonitor_SetTimingCount);
EXPORT_SYMBOL(MApi_XC_PCMonitor);
EXPORT_SYMBOL(MApi_XC_PCMonitor_GetCurrentState);
EXPORT_SYMBOL(MApi_XC_PCMonitor_GetSyncStatus);
EXPORT_SYMBOL(MApi_XC_PCMonitor_Get_HFreqx10);
EXPORT_SYMBOL(MApi_XC_PCMonitor_Get_HFreqx1K);
EXPORT_SYMBOL(MApi_XC_PCMonitor_Get_VFreqx10);
EXPORT_SYMBOL(MApi_XC_PCMonitor_Get_VFreqx1K);
EXPORT_SYMBOL(MApi_XC_PCMonitor_Get_Vtotal);
EXPORT_SYMBOL(MApi_XC_PCMonitor_Get_Dvi_Hdmi_De_Info);
EXPORT_SYMBOL(MApi_XC_PCMonitor_SyncLoss);
EXPORT_SYMBOL(MApi_XC_PCMonitor_InvalidTimingDetect);
EXPORT_SYMBOL(MApi_XC_PCMonitor_SetTimingCountEx);
#ifdef __cplusplus
#endif
#endif
#ifdef _DRVDDC2BI_H_
#endif
#ifdef _API_AUDIO_H_
#ifdef __cplusplus
#endif
#if defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
EXPORT_SYMBOL(MApi_AUDIO_SetPowerOn);
EXPORT_SYMBOL(MApi_AUDIO_Initialize);
EXPORT_SYMBOL(MApi_AUDIO_SetSystemInfo);
EXPORT_SYMBOL(MApi_Audio_Monitor);
EXPORT_SYMBOL(MApi_AUDIO_SetOutConnectivity);
EXPORT_SYMBOL(MApi_AUDIO_SetPathInfo);
EXPORT_SYMBOL(MApi_AUDIO_GetPathInfo);
EXPORT_SYMBOL(MApi_AUDIO_SetOutputInfo);
EXPORT_SYMBOL(MApi_AUDIO_SetSourceInfo);
EXPORT_SYMBOL(MApi_AUDIO_InputSwitch);
EXPORT_SYMBOL(MApi_AUDIO_SetDataCaptureSource);
EXPORT_SYMBOL(MApi_AUDIO_SetOutputSourceInfo);
EXPORT_SYMBOL(MApi_AUDIO_GetOutputSourceInfo);
EXPORT_SYMBOL(MApi_AUDIO_ReadMailBox);
EXPORT_SYMBOL(MApi_AUDIO_WriteMailBox);
EXPORT_SYMBOL(MApi_AUDIO_FwTriggerDSP);
EXPORT_SYMBOL(MApi_AUDIO_SetFwStatus);
EXPORT_SYMBOL(MApi_AUDIO_ExitAudioSystem);
EXPORT_SYMBOL(MApi_AUDIO_RebootDsp);
EXPORT_SYMBOL(MApi_AUDIO_SendIntrupt);
EXPORT_SYMBOL(MApi_AUDIO_I2S_SetMode);
EXPORT_SYMBOL(MApi_AUDIO_WritePreInitTable);
EXPORT_SYMBOL(MApi_AUDIO_EnaEarphone_LowPower_Stage);
EXPORT_SYMBOL(MApi_AUDIO_EnaEarphone_HighDriving_Stage);
EXPORT_SYMBOL(MApi_AUDIO_SetDspBaseAddr);
EXPORT_SYMBOL(MApi_AUDIO_GetDspMadBaseAddr);
EXPORT_SYMBOL(MApi_SND_ProcessEnable);
EXPORT_SYMBOL(MApi_SND_SetParam1);
EXPORT_SYMBOL(MApi_SND_GetParam1);
EXPORT_SYMBOL(MApi_AUDIO_SOUND_SetParam);
EXPORT_SYMBOL(MApi_AUDIO_SetAbsoluteVolume);
EXPORT_SYMBOL(MApi_AUDIO_SetPreScale);
EXPORT_SYMBOL(MApi_AUDIO_SetMute);
EXPORT_SYMBOL(MApi_AUDIO_SourceConnect);
EXPORT_SYMBOL(MApi_AUDIO_SetBalance);
EXPORT_SYMBOL(MApi_AUDIO_EnableEQ);
EXPORT_SYMBOL(MApi_AUDIO_EnableTone);
EXPORT_SYMBOL(MApi_AUDIO_EnableAutoVolume);
EXPORT_SYMBOL(MApi_AUDIO_EnableSurround);
EXPORT_SYMBOL(MApi_AUDIO_EnableBalance);
EXPORT_SYMBOL(MApi_AUDIO_SetDynamicBass);
EXPORT_SYMBOL(MApi_AUDIO_SetBass);
EXPORT_SYMBOL(MApi_AUDIO_AbsoluteBass);
EXPORT_SYMBOL(MApi_AUDIO_SetTreble);
EXPORT_SYMBOL(MApi_AUDIO_AbsoluteTreble);
EXPORT_SYMBOL(MApi_AUDIO_SetEq);
EXPORT_SYMBOL(MApi_AUDIO_SetEq_7band);
EXPORT_SYMBOL(MApi_AUDIO_SetAvcThreshold);
EXPORT_SYMBOL(MApi_AUDIO_SetAvcMode);
EXPORT_SYMBOL(MApi_AUDIO_SetAvcAT);
EXPORT_SYMBOL(MApi_AUDIO_SetAvcRT);
EXPORT_SYMBOL(MApi_AUDIO_SetBufferProcess);
EXPORT_SYMBOL(MApi_AUDIO_SetSurroundXA);
EXPORT_SYMBOL(MApi_AUDIO_SetSurroundXB);
EXPORT_SYMBOL(MApi_AUDIO_SetSurroundXK);
EXPORT_SYMBOL(MApi_AUDIO_SetSurroundLPFGain);
EXPORT_SYMBOL(MApi_AUDIO_ConvertVolumeUnit);
EXPORT_SYMBOL(MApi_AUDIO_SetPEQCoef);
EXPORT_SYMBOL(MApi_AUDIO_EnablePEQ);
EXPORT_SYMBOL(MApi_AUDIO_EnableDcRemove);
EXPORT_SYMBOL(MApi_AUDIO_ReleaseDecodeSystem);
EXPORT_SYMBOL(MApi_AUDIO_SetDecodeSystem);
EXPORT_SYMBOL(MApi_AUDIO_GetDecodeSystem);
EXPORT_SYMBOL(MApi_AUDIO_SetDecodeCmd);
EXPORT_SYMBOL(MApi_AUDIO_OpenDecodeSystem);
EXPORT_SYMBOL(MApi_AUDIO_SetSystem);
EXPORT_SYMBOL(MApi_AUDIO_GetDecSysSupportStatus);
EXPORT_SYMBOL(MApi_AUDIO_SetH264StreamID_Mod);
EXPORT_SYMBOL(MApi_AUDIO_IsMadLock);
EXPORT_SYMBOL(MApi_AUDIO_GetDecStatus);
EXPORT_SYMBOL(MApi_AUDIO_GetCommand);
EXPORT_SYMBOL(MApi_AUDIO_SetCommand);
EXPORT_SYMBOL(MApi_AUDIO_GetMAD_LOCK);
EXPORT_SYMBOL(MApi_AUDIO_SetADOutputMode);
EXPORT_SYMBOL(MApi_AUDIO_SetADAbsoluteVolume);
EXPORT_SYMBOL(MApi_AUDIO_ADSetMute);
EXPORT_SYMBOL(MApi_AUDIO_SetEncInit);
EXPORT_SYMBOL(MApi_AUDIO_SetEncCommand);
EXPORT_SYMBOL(MApi_AUDIO_GetEncodeFrameInfo);
EXPORT_SYMBOL(MApi_AUDIO_GetEncodeDoneFlag);
EXPORT_SYMBOL(MApi_AUDIO_SIF_SetStandard);
EXPORT_SYMBOL(MApi_AUDIO_SIF_GetSoundMode);
EXPORT_SYMBOL(MApi_AUDIO_SIF_SetSoundMode);
EXPORT_SYMBOL(MApi_AUDIO_SIF_GetAudioStatus);
EXPORT_SYMBOL(MApi_AUDIO_SIF_StartAutoStandardDetection);
EXPORT_SYMBOL(MApi_AUDIO_SIF_GetResultOfAutoStandardDetection);
EXPORT_SYMBOL(MApi_AUDIO_SIF_ConvertToBasicAudioStandard);
#if defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MApi_AUDIO_SIF_SetThreshold);
#else
EXPORT_SYMBOL(MApi_AUDIO_SIF_SetThreshold);
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MApi_AUDIO_SIF_SetPrescale);
#else
EXPORT_SYMBOL(MApi_AUDIO_SIF_SetPrescale);
#endif
EXPORT_SYMBOL(MApi_AUDIO_SIF_IsPALType);
EXPORT_SYMBOL(MApi_AUDIO_SIF_SetPALType);
EXPORT_SYMBOL(MApi_AUDIO_SIF_SendCmd);
EXPORT_SYMBOL(MApi_AUDIO_SIF_Shift);
EXPORT_SYMBOL(MApi_AUDIO_TriggerSifPLL);
EXPORT_SYMBOL(MApi_AUDIO_FM_RADIO_GetSoundMode);
EXPORT_SYMBOL(MApi_AUDIO_FM_RADIO_SetSoundMode);
EXPORT_SYMBOL(MApi_AUDIO_FM_RADIO_DeEmphassisOption);
EXPORT_SYMBOL(MApi_AUDIO_FM_RADIO_GET_DC_AMP);
EXPORT_SYMBOL(MApi_AUDIO_FM_RADIO_GET_NSR);
EXPORT_SYMBOL(MApi_AUDIO_FM_RADIO_RESET);
EXPORT_SYMBOL(MApi_AUDIO_SPDIF_HWEN);
EXPORT_SYMBOL(MApi_AUDIO_SPDIF_SetMute);
EXPORT_SYMBOL(MApi_AUDIO_SPDIF_SetMode);
EXPORT_SYMBOL(MApi_AUDIO_SPDIF_GetMode);
EXPORT_SYMBOL(MApi_AUDIO_SPDIF_SetSCMS);
EXPORT_SYMBOL(MApi_AUDIO_SPDIF_GetSCMS);
EXPORT_SYMBOL(MApi_Audio_SPDIF_Monitor);
EXPORT_SYMBOL(MApi_AUDIO_SPDIF_ChannelStatus_CTRL);
EXPORT_SYMBOL(MApi_AUDIO_DigitalOut_SetChannelStatus);
EXPORT_SYMBOL(MApi_AUDIO_DigitalOut_SetDeviceCapability);
EXPORT_SYMBOL(MApi_AUDIO_HDMI_Tx_SetMute);
EXPORT_SYMBOL(MApi_AUDIO_HDMI_Tx_GetStatus);
EXPORT_SYMBOL(MApi_AUDIO_HDMI_Monitor);
EXPORT_SYMBOL(MApi_AUDIO_HDMI_GetNonpcmFlag);
EXPORT_SYMBOL(MApi_AUDIO_HDMI_SetNonpcm);
EXPORT_SYMBOL(MApi_AUDIO_HDMI_RX_SetNonpcm);
EXPORT_SYMBOL(MApi_AUDIO_HDMI_RX_GetNonPCM);
EXPORT_SYMBOL(MApi_AUDIO_DTV_HDMI_CFG);
EXPORT_SYMBOL(MApi_AUDIO_HDMI_GetSynthFreq);
EXPORT_SYMBOL(MApi_AUDIO_HDMI_SetDownSample);
EXPORT_SYMBOL(MApi_AUDIO_HDMI_TX_SetMode);
EXPORT_SYMBOL(MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus);
EXPORT_SYMBOL(MApi_AUDIO_ADVSOUND_ProcessEnable);
EXPORT_SYMBOL(MApi_AUDIO_ADVSOUND_SubProcessEnable);
EXPORT_SYMBOL(MApi_AUDIO_ADVSOUND_SetParam);
EXPORT_SYMBOL(MApi_AUDIO_ADVSND_SetParam);
EXPORT_SYMBOL(MApi_AUDIO_ADVSOUND_GetInfo);
EXPORT_SYMBOL(MApi_DBXTV_SetMode);
EXPORT_SYMBOL(MApi_AUDIO_SeInit);
#if defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
EXPORT_SYMBOL(MApi_AUDIO_SetAdvSndSys);
#else
EXPORT_SYMBOL(MApi_AUDIO_SetAdvSndSys);
#endif
EXPORT_SYMBOL(MApi_AUDIO_SetVDS);
EXPORT_SYMBOL(MApi_AUDIO_SetVSPK);
EXPORT_SYMBOL(MApi_AUDIO_SetSRS);
EXPORT_SYMBOL(MApi_AUDIO_SetBBE);
EXPORT_SYMBOL(MApi_AUDIO_VSPK_WMod);
EXPORT_SYMBOL(MApi_AUDIO_VSPK_SMod);
EXPORT_SYMBOL(MApi_AUDIO_SRS_DC);
EXPORT_SYMBOL(MApi_AUDIO_SRS_TruBass);
EXPORT_SYMBOL(MApi_AUDIO_SRS_SetTsxtPara);
EXPORT_SYMBOL(MApi_AUDIO_SetSRSTSHD);
EXPORT_SYMBOL(MApi_AUDIO_TSHD_Definition);
EXPORT_SYMBOL(MApi_AUDIO_SRS_SetTshdPara);
EXPORT_SYMBOL(MApi_AUDIO_COPY_Parameter);
EXPORT_SYMBOL(MApi_AUDIO_SetKTVInfo);
EXPORT_SYMBOL(MApi_AUDIO_GetKTVInfo);
EXPORT_SYMBOL(MApi_AUDIO_SetMixModeVolume);
EXPORT_SYMBOL(MApi_AUDIO_SetMixModeMute);
EXPORT_SYMBOL(MApi_AUDIO_PlayMenuSound);
EXPORT_SYMBOL(MApi_AUDIO_SetCertMode);
EXPORT_SYMBOL(MApi_AUDIO_SetCommAudioInfo);
EXPORT_SYMBOL(MApi_AUDIO_SetMpegInfo);
EXPORT_SYMBOL(MApi_AUDIO_SetAC3Info);
EXPORT_SYMBOL(MApi_AUDIO_SetAC3PInfo);
EXPORT_SYMBOL(MApi_AUDIO_SetAACInfo);
EXPORT_SYMBOL(MApi_AUDIO_SetWmaInfo);
EXPORT_SYMBOL(MApi_AUDIO_SetDTSCommonCtrl);
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MApi_AUDIO_GetCommAudioInfo);
#else
EXPORT_SYMBOL(MApi_AUDIO_GetCommAudioInfo);
#endif
EXPORT_SYMBOL(MApi_AUDIO_GetMpegInfo);
EXPORT_SYMBOL(MApi_AUDIO_GetAC3Info);
EXPORT_SYMBOL(MApi_AUDIO_GetAC3PInfo);
EXPORT_SYMBOL(MApi_AUDIO_GetAACInfo);
EXPORT_SYMBOL(MApi_AUDIO_GetWmaInfo);
EXPORT_SYMBOL(MApi_AUDIO_GetDTSInfo);
#if defined(UFO_PUBLIC_HEADER_212) ||defined(UFO_PUBLIC_HEADER_500_3)
EXPORT_SYMBOL(MApi_AUDIO_XPCM_Param);
#else
EXPORT_SYMBOL(MApi_AUDIO_XPCM_Param);
#endif
#if defined(UFO_PUBLIC_HEADER_212) ||defined(UFO_PUBLIC_HEADER_500_3)
EXPORT_SYMBOL(MApi_AUDIO_XPCM2_Param);
#else
EXPORT_SYMBOL(MApi_AUDIO_XPCM2_Param);
#endif
EXPORT_SYMBOL(MApi_AUDIO_XPCM2_CheckIntStatus);
EXPORT_SYMBOL(MApi_AUDIO_RA8_Param);
EXPORT_SYMBOL(MApi_AUDIO_Init);
EXPORT_SYMBOL(MApi_AUDIO_StartDecode);
EXPORT_SYMBOL(MApi_AUDIO_StartBrowse);
EXPORT_SYMBOL(MApi_AUDIO_StopDecode);
EXPORT_SYMBOL(MApi_AUDIO_PauseDecode);
EXPORT_SYMBOL(MApi_AUDIO_CheckPlayDone);
EXPORT_SYMBOL(MApi_AUDIO_GetResidualBufferSize);
EXPORT_SYMBOL(MApi_AUDIO_GetPCMBufferSize);
EXPORT_SYMBOL(MApi_AUDIO_GetPCMBufferSize2);
EXPORT_SYMBOL(MApi_AUDIO_GetCurrentFrameNumber);
EXPORT_SYMBOL(MApi_AUDIO_GetSampleRate);
EXPORT_SYMBOL(MApi_AUDIO_GetBitRate);
EXPORT_SYMBOL(MApi_AUDIO_GetLayer);
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MApi_AUDIO_CheckInputRequest);
#else
EXPORT_SYMBOL(MApi_AUDIO_CheckInputRequest);
#endif
EXPORT_SYMBOL(MApi_AUDIO_SetInput);
EXPORT_SYMBOL(MApi_AUDIO_SetSampleRateIndex);
#if defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MApi_AUDIO_SetXPCMParam);
#else
EXPORT_SYMBOL(MApi_AUDIO_SetXPCMParam);
#endif
EXPORT_SYMBOL(MApi_AUDIO_FileEndNotification);
EXPORT_SYMBOL(MApi_AUDIO_FileEndDataHandle);
EXPORT_SYMBOL(MApi_AUDIO_GetPlayTick);
EXPORT_SYMBOL(MApi_AUDIO_GetEsMEMCnt);
EXPORT_SYMBOL(MApi_AUDIO_SetASFParm);
EXPORT_SYMBOL(MApi_AUDIO_MM_SetInput);
EXPORT_SYMBOL(MApi_AUDIO_MM_CheckPlayDone);
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MApi_AUDIO_MM_CheckInputRequest);
#else
EXPORT_SYMBOL(MApi_AUDIO_MM_CheckInputRequest);
#endif
EXPORT_SYMBOL(MApi_AUDIO_DmaReader_Init);
EXPORT_SYMBOL(MApi_AUDIO_DmaReader_AllInput_Init);
EXPORT_SYMBOL(MApi_AUDIO_DmaReader_WritePCM);
EXPORT_SYMBOL(MApi_AUDIO_DmaWriter_Init);
EXPORT_SYMBOL(MApi_AUDIO_USBPCM_Enable);
EXPORT_SYMBOL(MApi_AUDIO_USBPCM_SetFlag);
EXPORT_SYMBOL(MApi_AUDIO_USBPCM_GetFlag);
EXPORT_SYMBOL(MApi_AUDIO_USBPCM_GetMemInfo);
EXPORT_SYMBOL(MApi_AUDIO_PCMCapture_Init);
EXPORT_SYMBOL(MApi_AUDIO_PCMCapture_Start);
EXPORT_SYMBOL(MApi_AUDIO_PCMCapture_Stop);
EXPORT_SYMBOL(MApi_AUDIO_PCMCapture_Read);
EXPORT_SYMBOL(MApi_AUDIO_VoIP_Config);
EXPORT_SYMBOL(MApi_AUDIO_ALSA_Check);
EXPORT_SYMBOL(MApi_AUDIO_ALSA_Enable);
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MApi_AUDIO_UNI_CheckDecodeDone);
#else
EXPORT_SYMBOL(MApi_AUDIO_UNI_CheckDecodeDone);
#endif
EXPORT_SYMBOL(MApi_AUDIO_UNI_SetOutput);
EXPORT_SYMBOL(MApi_AUDIO_UNI_Set_PCMInputWriteAddr);
EXPORT_SYMBOL(MApi_AUDIO_UNI_Get_OutPCMLevel);
EXPORT_SYMBOL(MApi_AUDIO_RingTask);
EXPORT_SYMBOL(MApi_AUDIO_Ring_DataTransfer);
EXPORT_SYMBOL(MApi_AUDIO_MM2_initAesInfo);
EXPORT_SYMBOL(MApi_AUDIO_MM2_checkAesInfo);
EXPORT_SYMBOL(MApi_AUDIO_MM2_inputAesFinished);
EXPORT_SYMBOL(MApi_AUDIO_SetAudioParam2);
EXPORT_SYMBOL(MApi_AUDIO_GetAudioInfo2);
EXPORT_SYMBOL(MApi_AUDIO_GetDDRInfo);
EXPORT_SYMBOL(MApi_AUDIO_GetCaps);
EXPORT_SYMBOL(MApi_AUDIO_PCM_Open);
EXPORT_SYMBOL(MApi_AUDIO_PCM_Close);
EXPORT_SYMBOL(MApi_AUDIO_PCM_Start);
EXPORT_SYMBOL(MApi_AUDIO_PCM_Stop);
EXPORT_SYMBOL(MApi_AUDIO_PCM_Set);
EXPORT_SYMBOL(MApi_AUDIO_PCM_Get);
EXPORT_SYMBOL(MApi_AUDIO_PCM_Read);
EXPORT_SYMBOL(MApi_AUDIO_PCM_Write);
EXPORT_SYMBOL(MApi_AUDIO_PCM_Flush);
#if defined(UFO_PUBLIC_HEADER_700)
EXPORT_SYMBOL(MApi_AUDIO_ENCODER_Open);
EXPORT_SYMBOL(MApi_AUDIO_ENCODER_Close);
EXPORT_SYMBOL(MApi_AUDIO_ENCODER_Start);
EXPORT_SYMBOL(MApi_AUDIO_ENCODER_Stop);
EXPORT_SYMBOL(MApi_AUDIO_ENCODER_Ioctl);
EXPORT_SYMBOL(MApi_AUDIO_ENCODER_Read);
EXPORT_SYMBOL(MApi_AUDIO_ENCODER_Flush);
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef DRVXC_HDMI_IF_EX_H
#ifdef __cplusplus
#endif
#ifdef MDRV_HDMI_EX_C
#else
#endif
#ifdef DRVXC_HDMI_IF_H
#endif
#ifdef DRVXC_HDMI_IF_H
#endif
#ifdef DVI_NEW_Patch
#endif
EXPORT_SYMBOL(MDrv_HDMI_EX_GetLibVer);
EXPORT_SYMBOL(MDrv_HDMI_EX_GetInfo);
EXPORT_SYMBOL(MDrv_HDMI_EX_GetStatus);
EXPORT_SYMBOL(MDrv_HDMI_EX_Get_AVIInfoActiveInfoPresent);
EXPORT_SYMBOL(MDrv_HDMI_EX_Func_Caps);
EXPORT_SYMBOL(MDrv_HDMI_EX_init);
EXPORT_SYMBOL(MDrv_HDMI_EX_Exit);
EXPORT_SYMBOL(MDrv_HDMI_EX_pkt_reset);
EXPORT_SYMBOL(MDrv_HDMI_EX_pullhpd);
EXPORT_SYMBOL(MDrv_HDMI_EX_GC_Info);
EXPORT_SYMBOL(MDrv_HDMI_EX_Packet_Received);
EXPORT_SYMBOL(MDrv_HDMI_EX_Get_ColorFormat);
EXPORT_SYMBOL(MDrv_HDMI_EX_Get_ColorRange);
EXPORT_SYMBOL(MDrv_HDMI_EX_Get_Content_Type);
EXPORT_SYMBOL(MDrv_HDMI_EX_Get_ExtColorimetry);
EXPORT_SYMBOL(MDrv_HDMI_EX_Set_EQ);
EXPORT_SYMBOL(MDrv_HDMI_EX_Set_EQ_To_Port);
EXPORT_SYMBOL(MDrv_HDMI_EX_Audio_MUTE_Enable);
EXPORT_SYMBOL(MDrv_HDMI_EX_Audio_Status_Clear);
EXPORT_SYMBOL(MDrv_HDMI_EX_Get_AspectRatio);
EXPORT_SYMBOL(MDrv_HDMI_EX_Get_ActiveFormat_AspectRatio);
EXPORT_SYMBOL(MDrv_HDMI_EX_Get_AVIInfoFrameVer);
EXPORT_SYMBOL(MDrv_HDMI_EX_err_status_update);
EXPORT_SYMBOL(MDrv_HDMI_EX_Get_PollingStatus);
EXPORT_SYMBOL(MDrv_HDMI_EX_IsHDMI_Mode);
EXPORT_SYMBOL(MDrv_HDMI_EX_Get_MID_info);
EXPORT_SYMBOL(MDrv_HDMI_EX_get_packet_value);
EXPORT_SYMBOL(MDrv_DVI_EX_ChannelPhaseStatus);
EXPORT_SYMBOL(MDrv_DVI_EX_SoftwareReset);
EXPORT_SYMBOL(MDrv_DVI_EX_ClkPullLow);
EXPORT_SYMBOL(MDrv_DVI_EX_SwitchSrc);
EXPORT_SYMBOL(MDrv_HDMI_EX_SetForClockLessThan70Mhz);
EXPORT_SYMBOL(MDrv_HDMI_EX_dvi_adjust);
EXPORT_SYMBOL(MDrv_HDMI_EX_SetUpdatePhaseLineCount);
EXPORT_SYMBOL(MDrv_HDCP_EX_Enable);
EXPORT_SYMBOL(MDrv_HDMI_EX_SetHdcpEnable);
EXPORT_SYMBOL(MDrv_HDCP_EX_ClearStatus);
EXPORT_SYMBOL(MDrv_HDCP_EX_initproductionkey);
EXPORT_SYMBOL(MDrv_HDCP_EX_GetStatus);
EXPORT_SYMBOL(MDrv_HDCP_EX_Vsync_end_en);
EXPORT_SYMBOL(MDrv_HDMI_EX_audio_output);
EXPORT_SYMBOL(MDrv_HDMI_EX_audio_cp_hdr_info);
EXPORT_SYMBOL(MDrv_HDMI_EX_audio_channel_status);
EXPORT_SYMBOL(MDrv_HDMI_EX_READ_DDCRAM);
EXPORT_SYMBOL(MDrv_HDMI_EX_PROG_DDCRAM);
EXPORT_SYMBOL(MDrv_HDMI_EX_Get_Pixel_Repetition);
EXPORT_SYMBOL(MDrv_HDMI_EX_3D_4Kx2K_Process);
EXPORT_SYMBOL(MDrv_HDMI_EX_AVG_ScaleringDown);
EXPORT_SYMBOL(MDrv_HDMI_EX_Check4K2K);
EXPORT_SYMBOL(MDrv_HDMI_EX_Check_Additional_Format);
EXPORT_SYMBOL(MDrv_HDMI_EX_Get_3D_Structure);
EXPORT_SYMBOL(MDrv_HDMI_EX_Get_3D_Ext_Data);
EXPORT_SYMBOL(MDrv_HDMI_EX_Get_3D_Meta_Field);
EXPORT_SYMBOL(MDrv_HDMI_EX_Get_VIC_Code);
EXPORT_SYMBOL(MDrv_HDMI_EX_Get_4Kx2K_VIC_Code);
EXPORT_SYMBOL(MDrv_HDMI_EX_ARC_PINControl);
#if 0
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _XC_MODE_PARSE_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRVCMAPOOL_H_
#if defined (MSOS_TYPE_LINUX)
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#endif
#ifdef DRV_WBLE_H
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_MFE_V2_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef __DRV_CA_V2_H__
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_ATSC_H_
#if !defined UTPA2
#else
#endif
#if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
#ifndef MSIF_TAG
#endif
#endif
#if DMD_ATSC_UTOPIA2_EN || DMD_ATSC_STR_EN
#endif
#ifndef DLL_PUBLIC
#endif
#if DMD_ATSC_MULTI_DMD_EN
#else
#endif
#if DMD_ATSC_UTOPIA_EN || DMD_ATSC_UTOPIA2_EN
#endif
#ifndef BIT_
#endif
#ifdef UTPA2
#endif
#if !DMD_ATSC_UTOPIA_EN && !DMD_ATSC_UTOPIA2_EN
#endif
#if DMD_ATSC_STR_EN
#endif
#ifdef __cplusplus
#endif
#ifndef MSOS_TYPE_LINUX_KERNEL
#if DMD_ATSC_STR_EN
#endif
#ifndef UTPA2
#endif
#if DMD_ATSC_STR_EN
#endif
#endif
#ifdef UTPA2
#if DMD_ATSC_STR_EN
#endif
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_XC_DLC_H_
#ifdef __cplusplus
#endif
#if defined(UFO_PUBLIC_HEADER_500)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#ifndef UNUSED
#endif
#if defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#endif
#if !defined (__aarch64__)
#endif
#if defined(UFO_PUBLIC_HEADER_212) || defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_700)
#endif
#if (ENABLE_10_BIT_DLC)
#endif
#if defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_700)
#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
#endif
#endif
#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
#if defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#endif
#endif
#endif
#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
#endif
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_XC_ACE_H_
#ifdef __cplusplus
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#if !defined (__aarch64__)
#endif
#endif
#if defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#if !defined (__aarch64__)
#endif
#endif
#if defined(UFO_PUBLIC_HEADER_700) || defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
#endif
EXPORT_SYMBOL(MApi_XC_ACE_Init);
EXPORT_SYMBOL(MApi_XC_ACE_Exit);
EXPORT_SYMBOL(MApi_XC_ACE_DMS);
EXPORT_SYMBOL(MApi_XC_ACE_SetPCYUV2RGB);
EXPORT_SYMBOL(MApi_XC_ACE_SelectYUVtoRGBMatrix);
EXPORT_SYMBOL(MApi_XC_ACE_SetColorCorrectionTable);
EXPORT_SYMBOL(MApi_XC_ACE_SetPCsRGBTable);
EXPORT_SYMBOL(MApi_XC_ACE_GetColorMatrix);
EXPORT_SYMBOL(MApi_XC_ACE_SetColorMatrix);
EXPORT_SYMBOL(MApi_XC_ACE_SetBypassColorMatrix);
EXPORT_SYMBOL(MApi_XC_ACE_SetIHC);
EXPORT_SYMBOL(MApi_XC_ACE_SetICC);
EXPORT_SYMBOL(MApi_XC_ACE_SetIBC);
EXPORT_SYMBOL(MApi_XC_ACE_PatchDTGColorChecker);
EXPORT_SYMBOL(MApi_XC_ACE_GetACEInfo);
EXPORT_SYMBOL(MApi_XC_ACE_GetACEInfoEx);
EXPORT_SYMBOL(MApi_XC_ACE_PicSetContrast);
EXPORT_SYMBOL(MApi_XC_ACE_PicSetBrightness);
EXPORT_SYMBOL(MApi_XC_ACE_PicSetBrightnessPrecise);
EXPORT_SYMBOL(MApi_XC_ACE_PicSetPreYOffset);
EXPORT_SYMBOL(MApi_XC_ACE_PicGetPreYOffset);
EXPORT_SYMBOL(MApi_XC_ACE_PicSetHue);
EXPORT_SYMBOL(MApi_XC_ACE_SkipWaitVsync);
EXPORT_SYMBOL(MApi_XC_ACE_PicSetSaturation);
EXPORT_SYMBOL(MApi_XC_ACE_PicSetSharpness);
EXPORT_SYMBOL(MApi_XC_ACE_PicSetColorTemp);
EXPORT_SYMBOL(MApi_XC_ACE_PicSetColorTempEx);
EXPORT_SYMBOL(MApi_XC_ACE_PicSetPostColorTemp_V02);
#ifndef _API_XC_ACE_C_
#ifndef ENABLE_NEW_COLORTEMP_METHOD
#endif
#ifndef ENABLE_PRECISE_RGBBRIGHTNESS
#endif
#if ENABLE_NEW_COLORTEMP_METHOD
#if ENABLE_PRECISE_RGBBRIGHTNESS
#else
#endif
#else
#if ENABLE_PRECISE_RGBBRIGHTNESS
#endif
#endif
#endif
EXPORT_SYMBOL(MApi_XC_ACE_SetFleshTone);
EXPORT_SYMBOL(MApi_XC_ACE_SetBlackAdjust);
EXPORT_SYMBOL(MApi_XC_ACE_Set_IHC_SRAM);
EXPORT_SYMBOL(MApi_XC_ACE_Set_ICC_SRAM);
EXPORT_SYMBOL(MApi_XC_ACE_EnableMWE);
EXPORT_SYMBOL(MApi_XC_ACE_MWECloneVisualEffect);
EXPORT_SYMBOL(MApi_XC_ACE_MWESetRegTable);
EXPORT_SYMBOL(MApi_XC_ACE_SetMWELoadFromPQ);
EXPORT_SYMBOL(MApi_XC_ACE_MWEStatus);
EXPORT_SYMBOL(MApi_XC_ACE_MWEHandle);
EXPORT_SYMBOL(MApi_XC_ACE_MWEFuncSel);
EXPORT_SYMBOL(MApi_XC_ACE_MWESetDispWin);
EXPORT_SYMBOL(MApi_XC_ACE_3DClonePQMap);
EXPORT_SYMBOL(MApi_XC_ACE_DNR_Blending_NRTbl);
EXPORT_SYMBOL(MApi_XC_ACE_DNR_Blending_MISC);
EXPORT_SYMBOL(MApi_XC_ACE_DNR_Blending);
EXPORT_SYMBOL(MApi_XC_ACE_DNR_Get_PrecisionShift);
EXPORT_SYMBOL(MApi_XC_ACE_DNR_GetMotion_Weight);
EXPORT_SYMBOL(MApi_XC_ACE_DNR_GetLuma_Weight);
EXPORT_SYMBOL(MApi_XC_ACE_DNR_GetNoise_Weight);
EXPORT_SYMBOL(MApi_XC_ACE_DNR_GetMotion);
EXPORT_SYMBOL(MApi_XC_ACE_DNR_Init_Motion);
EXPORT_SYMBOL(MApi_XC_ACE_DNR_Init_Luma);
EXPORT_SYMBOL(MApi_XC_ACE_DNR_Init_Noise);
EXPORT_SYMBOL(MApi_XC_ACE_DNR_SetParam);
EXPORT_SYMBOL(MApi_XC_ACE_SetHDRInit);
EXPORT_SYMBOL(MApi_XC_ACE_DNR_GetParam);
EXPORT_SYMBOL(MApi_XC_ACE_DNR_SetNRTbl);
EXPORT_SYMBOL(MApi_XC_ACE_GetLibVer);
EXPORT_SYMBOL(MApi_XC_ACE_GetInfo);
EXPORT_SYMBOL(MApi_XC_ACE_GetStatus);
EXPORT_SYMBOL(MApi_XC_ACE_SetDbgLevel);
EXPORT_SYMBOL(MApi_XC_ACE_ColorCorrectionTable);
EXPORT_SYMBOL(MApi_XC_ACE_SetColorMatrixControl);
EXPORT_SYMBOL(MApi_XC_ACE_SetRBChannelRange);
EXPORT_SYMBOL(MApi_XC_ACE_SetPowerState);
EXPORT_SYMBOL(MApi_XC_ACE_PicSetPostColorTemp);
EXPORT_SYMBOL(MApi_XC_ACE_PicSetPostColorTempEx);
EXPORT_SYMBOL(MApi_XC_ACE_PicSetPostColorTemp2);
EXPORT_SYMBOL(MApi_XC_ACE_PicSetPostColorTemp2Ex);
#ifdef __cplusplus
#endif
#endif
#ifdef _DRVUSB_H
#ifdef USB_SYSTEM_STR_SUPPORT
#endif
#endif
#ifdef _DRV_SAR_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_MVD_H_
#ifdef __cplusplus
#endif
#ifdef REDLION_LINUX_KERNEL_ENVI
#else
#endif
EXPORT_SYMBOL(MDrv_MVD_SetCfg);
EXPORT_SYMBOL(MDrv_MVD_GetFWVer);
EXPORT_SYMBOL(MDrv_MVD_SetDbgLevel);
EXPORT_SYMBOL(MDrv_MVD_GetInfo);
EXPORT_SYMBOL(MDrv_MVD_GetLibVer);
EXPORT_SYMBOL(MDrv_MVD_SetCodecInfo);
EXPORT_SYMBOL(MDrv_MVD_SetDivXCfg);
EXPORT_SYMBOL(MDrv_MVD_SetFrameBuffAddr);
EXPORT_SYMBOL(MDrv_MVD_GetFrameInfo);
EXPORT_SYMBOL(MDrv_MVD_SetOverflowTH);
EXPORT_SYMBOL(MDrv_MVD_SetUnderflowTH);
EXPORT_SYMBOL(MDrv_MVD_RstIFrameDec);
EXPORT_SYMBOL(MDrv_MVD_GetIsIFrameDecoding);
EXPORT_SYMBOL(MDrv_MVD_GetSyncStatus);
EXPORT_SYMBOL(MDrv_MVD_GetIsFreerun);
EXPORT_SYMBOL(MDrv_MVD_GetIsIPicFound);
EXPORT_SYMBOL(MDrv_MVD_DecodeIFrame);
EXPORT_SYMBOL(MDrv_MVD_GetValidStreamFlag);
EXPORT_SYMBOL(MDrv_MVD_GetChromaFormat);
EXPORT_SYMBOL(MDrv_MVD_SetFrameInfo);
EXPORT_SYMBOL(MDrv_MVD_GetErrInfo);
EXPORT_SYMBOL(MDrv_MVD_GetSkipPicCounter);
EXPORT_SYMBOL(MDrv_MVD_SetSLQWritePtr);
EXPORT_SYMBOL(MDrv_MVD_GetSLQReadPtr);
EXPORT_SYMBOL(MDrv_MVD_GetPicType);
EXPORT_SYMBOL(MDrv_MVD_GetBitsRate);
EXPORT_SYMBOL(MDrv_MVD_GetVideoRange);
EXPORT_SYMBOL(MDrv_MVD_GetLowDelayFlag);
EXPORT_SYMBOL(MDrv_MVD_GetIs32PullDown);
EXPORT_SYMBOL(MDrv_MVD_GetIsDynScalingEnabled);
EXPORT_SYMBOL(MDrv_MVD_GetPtsStcDiff);
EXPORT_SYMBOL(MDrv_MVD_Pause);
EXPORT_SYMBOL(MDrv_MVD_Resume);
EXPORT_SYMBOL(MDrv_MVD_StepDisp);
EXPORT_SYMBOL(MDrv_MVD_IsStepDispDone);
EXPORT_SYMBOL(MDrv_MVD_StepDecode);
EXPORT_SYMBOL(MDrv_MVD_IsStepDecodeDone);
EXPORT_SYMBOL(MDrv_MVD_SeekToPTS);
EXPORT_SYMBOL(MDrv_MVD_IsStep2PtsDone);
EXPORT_SYMBOL(MDrv_MVD_SkipToPTS);
EXPORT_SYMBOL(MDrv_MVD_TrickPlay);
EXPORT_SYMBOL(MDrv_MVD_RegSetBase);
EXPORT_SYMBOL(MDrv_MVD_Init);
EXPORT_SYMBOL(MDrv_MVD_Exit);
EXPORT_SYMBOL(MDrv_MVD_Rst);
EXPORT_SYMBOL(MDrv_MVD_Play);
EXPORT_SYMBOL(MDrv_MVD_SetAVSync);
EXPORT_SYMBOL(MDrv_MVD_SetAVSyncThreshold);
EXPORT_SYMBOL(MDrv_MVD_SetAVSyncFreerunThreshold);
EXPORT_SYMBOL(MDrv_MVD_GetAVSyncDelay);
EXPORT_SYMBOL(MDrv_MVD_GetIsAVSyncOn);
EXPORT_SYMBOL(MDrv_MVD_GetIsSyncRep);
EXPORT_SYMBOL(MDrv_MVD_GetIsSyncSkip);
EXPORT_SYMBOL(MDrv_MVD_ChangeAVsync);
EXPORT_SYMBOL(MDrv_MVD_DispCtrl);
EXPORT_SYMBOL(MDrv_MVD_DispRepeatField);
EXPORT_SYMBOL(MDrv_MVD_GetColorFormat);
EXPORT_SYMBOL(MDrv_MVD_GetMatrixCoef);
EXPORT_SYMBOL(MDrv_MVD_GetActiveFormat);
EXPORT_SYMBOL(MDrv_MVD_GetDispRdy);
EXPORT_SYMBOL(MDrv_MVD_Is1stFrmRdy);
EXPORT_SYMBOL(MDrv_MVD_GetGOPCount);
EXPORT_SYMBOL(MDrv_MVD_GetPicCounter);
EXPORT_SYMBOL(MDrv_MVD_GetParserByteCnt);
EXPORT_SYMBOL(MDrv_MVD_GetDecodeStatus);
EXPORT_SYMBOL(MDrv_MVD_GetLastCmd);
EXPORT_SYMBOL(MDrv_MVD_GetVldErrCount);
EXPORT_SYMBOL(MDrv_MVD_DropErrorFrame);
EXPORT_SYMBOL(MDrv_MVD_MVDCommand);
EXPORT_SYMBOL(MDrv_MVD_SkipData);
EXPORT_SYMBOL(MDrv_MVD_SkipToIFrame);
EXPORT_SYMBOL(MDrv_MVD_GetCaps);
EXPORT_SYMBOL(MDrv_MVD_DisableErrConceal);
EXPORT_SYMBOL(MDrv_MVD_PushQueue);
EXPORT_SYMBOL(MDrv_MVD_FlushQueue);
EXPORT_SYMBOL(MDrv_MVD_FlushDisplayBuf);
EXPORT_SYMBOL(MDrv_MVD_GetQueueVacancy);
EXPORT_SYMBOL(MDrv_MVD_GetESReadPtr);
EXPORT_SYMBOL(MDrv_MVD_GetESWritePtr);
EXPORT_SYMBOL(MDrv_MVD_EnableLastFrameShow);
EXPORT_SYMBOL(MDrv_MVD_IsDispFinish);
EXPORT_SYMBOL(MDrv_MVD_SetSpeed);
EXPORT_SYMBOL(MDrv_MVD_ResetPTS);
EXPORT_SYMBOL(MDrv_MVD_GetPTS);
EXPORT_SYMBOL(MDrv_MVD_GetNextPTS);
EXPORT_SYMBOL(MDrv_MVD_GetTrickMode);
EXPORT_SYMBOL(MDrv_MVD_IsPlaying);
EXPORT_SYMBOL(MDrv_MVD_IsIdle);
EXPORT_SYMBOL(MDrv_MVD_IsSeqChg);
EXPORT_SYMBOL(MDrv_MVD_DbgSetData);
EXPORT_SYMBOL(MDrv_MVD_DbgGetData);
EXPORT_SYMBOL(MDrv_MVD_GetDecodedFrameIdx);
EXPORT_SYMBOL(MDrv_MVD_SetFileModeAVSync);
EXPORT_SYMBOL(MDrv_MVD_IsAllBufferEmpty);
EXPORT_SYMBOL(MDrv_MVD_GenPattern);
EXPORT_SYMBOL(MDrv_MVD_GetPatternInfo);
EXPORT_SYMBOL(MDrv_MVD_SetDynScalingParam);
EXPORT_SYMBOL(MDrv_MVD_SetVirtualBox);
EXPORT_SYMBOL(MDrv_MVD_SetBlueScreen);
EXPORT_SYMBOL(MDrv_MVD_EnableInt);
EXPORT_SYMBOL(MDrv_MVD_EnableDispOneField);
EXPORT_SYMBOL(MDrv_MVD_GetExtDispInfo);
EXPORT_SYMBOL(MDrv_MVD_GetFrmInfo);
EXPORT_SYMBOL(MDrv_MVD_GetTimeCode);
EXPORT_SYMBOL(MDrv_MVD_GetUsrDataIsAvailable);
EXPORT_SYMBOL(MDrv_MVD_GetUsrDataInfo);
EXPORT_SYMBOL(MDrv_MVD_SetFreezeDisp);
EXPORT_SYMBOL(MDrv_MVD_GetDispCount);
EXPORT_SYMBOL(MDrv_MVD_SetFdMaskDelayCount);
EXPORT_SYMBOL(MDrv_MVD_SetOutputFRCMode);
EXPORT_SYMBOL(MDrv_MVD_SetFRCDropType);
EXPORT_SYMBOL(MDrv_MVD_SetDisableSeqChange);
EXPORT_SYMBOL(MDrv_MVD_SetMStreamerMode);
EXPORT_SYMBOL(MDrv_MVD_FrameFlip);
EXPORT_SYMBOL(MDrv_MVD_FrameRelease);
EXPORT_SYMBOL(MDrv_MVD_FrameCapture);
EXPORT_SYMBOL(MDrv_MVD_FlushPTSBuf);
EXPORT_SYMBOL(MDrv_MVD_ParserRstDone);
EXPORT_SYMBOL(MDrv_MVD_DbgDump);
EXPORT_SYMBOL(MDrv_MVD_GetFrmRateIsSupported);
EXPORT_SYMBOL(MDrv_MVD_SetAutoMute);
EXPORT_SYMBOL(MDrv_MVD_SetVSizeAlign);
EXPORT_SYMBOL(MDrv_MVD_SetDispFinishMode);
EXPORT_SYMBOL(MDrv_MVD_SetIdctMode);
EXPORT_SYMBOL(MDrv_MVD_GetDivxVer);
#ifndef REDLION_LINUX_KERNEL_ENVI
#endif
#ifdef MVD_ENABLE_ISR
EXPORT_SYMBOL(MDrv_MVD_SetIsrEvent);
EXPORT_SYMBOL(MDrv_MVD_GetIsrEvent);
#else
#endif
EXPORT_SYMBOL(MDrv_MVD_ShowFirstFrameDirect);
#ifdef __cplusplus
#endif
#endif
#if defined(UFO_PUBLIC_HEADER_212)
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#ifdef _DRVIPAUTH_H_
#ifdef __cplusplus
#endif
#ifndef DLL_PUBLIC
#endif
#ifndef DLL_LOCAL
#endif
EXPORT_SYMBOL(MDrv_AUTH_IPCheck);
EXPORT_SYMBOL(MDrv_AUTH_GetHashInfo);
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_DVBC_H_
#ifdef __cplusplus
#endif
#ifdef UFO_DEMOD_DVBC_SUPPORT_DMD_INT
#endif
#ifdef UFO_DEMOD_DVBC_SUPPORT_DMD_INT
#endif
#ifdef UFO_DEMOD_DVBC_GET_AGC_INFO
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef __MSDLC_H
#ifdef _MSDLC_C_
#else
#endif
#if ENABLE_DLC
#if defined(UFO_PUBLIC_HEADER_500)
#endif
#if defined(UFO_PUBLIC_HEADER_500)
#endif
#if (ENABLE_DLC == DLC_8SEG_HIS_MODE)
#endif
#endif
#endif
#ifdef _DLC_V2_H_
#ifdef _DLC_C_
#else
#endif
#ifdef __cplusplus
#endif
#if defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_XC_HDR_VERSION) && (UFO_XC_HDR_VERSION == 2)
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_XC_ADC_H_
#ifdef __cplusplus
#endif
EXPORT_SYMBOL(MApi_XC_ADC_SetCVBSOut);
EXPORT_SYMBOL(MApi_XC_ADC_IsCVBSOutEnabled);
EXPORT_SYMBOL(MApi_XC_ADC_SetPcClock);
EXPORT_SYMBOL(MApi_XC_ADC_SetPhase);
EXPORT_SYMBOL(MApi_XC_ADC_SetPhaseEx);
EXPORT_SYMBOL(MApi_XC_ADC_GetPhaseRange);
EXPORT_SYMBOL(MApi_XC_ADC_GetPhase);
EXPORT_SYMBOL(MApi_XC_ADC_GetPhaseEx);
EXPORT_SYMBOL(MApi_XC_ADC_IsScartRGB);
EXPORT_SYMBOL(MApi_XC_ADC_GetPcClock);
EXPORT_SYMBOL(MApi_XC_ADC_GetSoGLevelRange);
EXPORT_SYMBOL(MApi_XC_ADC_SetSoGLevel);
EXPORT_SYMBOL(MApi_XC_ADC_PowerOff);
EXPORT_SYMBOL(MApi_XC_ADC_GetDefaultGainOffset);
EXPORT_SYMBOL(MApi_XC_ADC_GetMaximalOffsetValue);
EXPORT_SYMBOL(MApi_XC_ADC_GetMaximalGainValue);
EXPORT_SYMBOL(MApi_XC_ADC_GetCenterGain);
EXPORT_SYMBOL(MApi_XC_ADC_GetCenterOffset);
EXPORT_SYMBOL(MApi_XC_ADC_SetGain);
EXPORT_SYMBOL(MApi_XC_ADC_SetOffset);
EXPORT_SYMBOL(MApi_XC_ADC_AdjustGainOffset);
EXPORT_SYMBOL(MApi_XC_ADC_Source_Calibrate);
EXPORT_SYMBOL(MApi_XC_ADC_SetSoGCal);
EXPORT_SYMBOL(MApi_XC_ADC_SetRGB_PIPE_Delay);
EXPORT_SYMBOL(MApi_XC_ADC_ScartRGB_SOG_ClampDelay);
EXPORT_SYMBOL(MApi_XC_ADC_Set_YPbPrLooseLPF);
EXPORT_SYMBOL(MApi_XC_ADC_Set_SOGBW);
EXPORT_SYMBOL(MApi_XC_ADC_SetClampDuration);
EXPORT_SYMBOL(MApi_XC_ADC_EnableHWCalibration);
EXPORT_SYMBOL(MApi_XC_ADC_SetIdacCurrentMode);
EXPORT_SYMBOL(MApi_XC_ADC_GetIdacCurrentMode);
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_NSK2TYPE_H_
#endif
#ifdef _DRV_MIU_H_
#ifdef __cplusplus
#endif
#ifdef STELLAR
#endif
#ifdef ALIGN
#else
#endif
EXPORT_SYMBOL(MDrv_MIU_Mask_Req_OPM_R);
EXPORT_SYMBOL(MDrv_MIU_Mask_Req_DNRB_R);
EXPORT_SYMBOL(MDrv_MIU_Mask_Req_DNRB_W);
EXPORT_SYMBOL(MDrv_MIU_Mask_Req_DNRB_RW);
EXPORT_SYMBOL(MDrv_MIU_Mask_Req_SC_RW);
EXPORT_SYMBOL(MDrv_MIU_Mask_Req_MVOP_R);
EXPORT_SYMBOL(MDrv_MIU_Mask_Req_MVD_R);
EXPORT_SYMBOL(MDrv_MIU_Mask_Req_MVD_W);
EXPORT_SYMBOL(MDrv_MIU_Mask_Req_MVD_RW);
EXPORT_SYMBOL(MDrv_MIU_Mask_Req_AUDIO_RW);
EXPORT_SYMBOL(MDrv_MIU_MaskReq);
EXPORT_SYMBOL(MDrv_MIU_UnMaskReq);
EXPORT_SYMBOL(MDrv_MIU_InitCounter);
EXPORT_SYMBOL(MDrv_MIU_PrintMIUProtectInfo);
EXPORT_SYMBOL(MDrv_MIU_GetDefaultClientID_KernelProtect);
EXPORT_SYMBOL(MDrv_MIU_GetBusWidth);
EXPORT_SYMBOL(MDrv_MIU_SetSsc);
EXPORT_SYMBOL(MDrv_MIU_SetSscValue);
EXPORT_SYMBOL(MDrv_MIU_SetIOMapBase);
EXPORT_SYMBOL(MDrv_MIU_ProtectAlign);
EXPORT_SYMBOL(MDrv_MIU_Dram_Size);
EXPORT_SYMBOL(MDrv_MIU_Dram_ReadSize);
EXPORT_SYMBOL(MDrv_MIU_ClinetNumber);
EXPORT_SYMBOL(MDrv_MIU_Protect);
EXPORT_SYMBOL(MDrv_MIU_ProtectEx);
EXPORT_SYMBOL(MDrv_MIU_IsSupportMIU1);
EXPORT_SYMBOL(MDrv_MIU_SelMIU);
EXPORT_SYMBOL(MDrv_MIU_GetClientInfo);
EXPORT_SYMBOL(MDrv_MIU_GetProtectInfo);
EXPORT_SYMBOL(MDrv_MIU_SetGroupPriority);
EXPORT_SYMBOL(MDrv_MIU_SetHPriorityMask);
EXPORT_SYMBOL(MDrv_MIU_GetLibVer);
EXPORT_SYMBOL(MDrv_MIU_EnableScramble);
EXPORT_SYMBOL(MDrv_MIU_IsScrambleEnabled);
EXPORT_SYMBOL(MDrv_MIU_Init);
EXPORT_SYMBOL(MDrv_MIU_SetPowerState);
EXPORT_SYMBOL(MDrv_MIU_GetClientWidth);
EXPORT_SYMBOL(MDrv_MIU_GetDramType);
#ifdef _MIU_INTERNEL_USE
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef DRV_GOP_SC_H
#ifdef DRV_GOP_SC_C
#else
#endif
#endif
#ifdef _API_DAC_H_
#ifdef __cplusplus
#endif
#ifndef SYMBOL_WEAK
#endif
EXPORT_SYMBOL(MApi_DAC_GetLibVer);
EXPORT_SYMBOL(MApi_DAC_GetInfo);
EXPORT_SYMBOL(MApi_DAC_GetStatus);
#ifdef UFO_XC_GETOUTPUTINTELACETIMING
EXPORT_SYMBOL(MApi_DAC_GetOutputInterlaceTiming);
#endif
EXPORT_SYMBOL(MApi_DAC_SetDbgLevel);
EXPORT_SYMBOL(MApi_DAC_Init);
EXPORT_SYMBOL(MApi_DAC_Enable);
EXPORT_SYMBOL(MApi_DAC_SetClkInv);
EXPORT_SYMBOL(MApi_DAC_SetYPbPrOutputTiming);
EXPORT_SYMBOL(MApi_DAC_SetOutputSource);
EXPORT_SYMBOL(MApi_DAC_SetOutputLevel);
EXPORT_SYMBOL(MApi_DAC_SetOutputSwapSel);
EXPORT_SYMBOL(MApi_DAC_OnOffSD);
EXPORT_SYMBOL(MApi_DAC_GetSDStatus);
EXPORT_SYMBOL(MApi_DAC_OnOffHD);
EXPORT_SYMBOL(MApi_DAC_GetHDStatus);
EXPORT_SYMBOL(MApi_DAC_ClkSel);
EXPORT_SYMBOL(MApi_DAC_DumpTable);
EXPORT_SYMBOL(MApi_DAC_Exit);
EXPORT_SYMBOL(MApi_DAC_SetIHalfOutput);
EXPORT_SYMBOL(MApi_DAC_SetQuartOutput);
EXPORT_SYMBOL(MApi_DAC_SetDacState);
EXPORT_SYMBOL(MApi_DAC_HotPlugDetect);
EXPORT_SYMBOL(Mapi_DAC_SetPowerState);
EXPORT_SYMBOL(MApi_DAC_SetWSSOnOff);
EXPORT_SYMBOL(MApi_DAC_GetWSSStatus);
EXPORT_SYMBOL(MApi_DAC_ResetWSSData);
EXPORT_SYMBOL(MApi_DAC_SetWSSOutput);
EXPORT_SYMBOL(MApi_DAC_EnableICT);
EXPORT_SYMBOL(MApi_DAC_SetVGAHsyncVsync);
#ifdef __cplusplus
#endif
#endif
#ifdef __ULOG_H__
#ifdef __cplusplus
#endif
#if (defined CONFIG_UTOPIA_TEE || defined CONFIG_MBOOT || defined CONFIG_NOS || defined MSOS_TYPE_OPTEE )
#elif (defined CONFIG_ECOS)
#ifdef MS_DEBUG
#else
#endif
#elif (defined MSOS_TYPE_LINUX_KERNEL)
#elif (defined CONFIG_MLOG)
#elif (defined ANDROID)
#ifdef ALOG
#else
#endif
#else
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DMX_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_XC_MODEPARSE_H_
#ifdef __cplusplus
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#endif
EXPORT_SYMBOL(MApi_XC_ModeParse_Init);
EXPORT_SYMBOL(MApi_XC_ModeParse_MatchMode);
EXPORT_SYMBOL(MApi_XC_ModeParse_MatchModeEx);
#ifdef __cplusplus
#endif
#endif
#if defined (MSOS_TYPE_LINUX)
#ifdef FRHSL_SHM_H
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#endif
#if 0
#ifdef _DRV_ATSC_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#endif
#ifdef _DRVVIF_H_
#ifdef __cplusplus
#endif
#if 0
#endif
#ifdef _DRVVIF_C_
#else
#endif
#ifndef BOOL
#endif
#ifndef BYTE
#endif
#ifndef WORD
#endif
#ifndef DWORD
#endif
#ifndef U8
#endif
#ifndef U16
#endif
#ifndef U32
#endif
#ifndef VIRT
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _GOPSCD_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef __DRV_CA_H__
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _XC_H_
#ifdef __cplusplus
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_212)
#endif
#if defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_500) ||defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_212)
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#endif
#if defined(UFO_PUBLIC_HEADER_212) || defined(UFO_PUBLIC_HEADER_300)
#endif
#if defined(UFO_PUBLIC_HEADER_500_3) ||defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)||defined(UFO_PUBLIC_HEADER_500)||defined(UFO_PUBLIC_HEADER_700)
#ifdef UFO_XC_SET_DSINFO_V0
#endif
#elif defined(UFO_PUBLIC_HEADER_300)
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_500_3) ||defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_700) || defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
#elif (defined(UFO_PUBLIC_HEADER_500) && defined(UFO_XC_FORCEWRITE_V2))
#endif
#if defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#ifdef UFO_XC_AUTO_DOWNLOAD
#endif
#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
#endif
#endif
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#ifdef UFO_XC_SUPPORT_DUAL_MIU
#endif
#endif
#if defined(UFO_PUBLIC_HEADER_700) || defined(UFO_PUBLIC_HEADER_500_3)
#ifdef UFO_XC_SETBLACKVIDEOBYMODE
#endif
#endif
#if defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#ifdef UFO_XC_FB_LEVEL
#endif
#ifdef UFO_XC_DE_MODE
#endif
#endif
#if defined(UFO_PUBLIC_HEADER_700) || defined(UFO_PUBLIC_HEADER_500)
#endif
#if defined(UFO_PUBLIC_HEADER_300) && defined(STELLAR)
#endif
#if defined(UFO_PUBLIC_HEADER_700) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_300)
#endif
#if defined(UFO_PUBLIC_HEADER_700) || defined(UFO_PUBLIC_HEADER_500)
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#endif
#if defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_212) || defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_300) ||defined(UFO_PUBLIC_HEADER_500)
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_212) || defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_300) ||defined(UFO_PUBLIC_HEADER_500)
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#elif (defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700))
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#endif
#if defined(UFO_PUBLIC_HEADER_700) || defined(UFO_PUBLIC_HEADER_500)
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#else
#endif
#ifdef UFO_PUBLIC_HEADER_212
#endif
#ifdef UFO_XC_SCAN_TYPE
#endif
#ifdef UFO_XC_SET_DSINFO_V0
#endif
#if 0
#endif
#ifdef UFO_XC_SETBLACKVIDEOBYMODE
#endif
#ifdef UFO_XC_TEST_PATTERN
#endif
#ifdef UFO_XC_AUTO_DOWNLOAD
#endif
#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
#endif
#endif
#if defined(UFO_PUBLIC_HEADER_500)
#else
#endif
#ifdef UFO_XC_FRAMEBUFFERCAPS
#endif
#ifdef UFO_XC_DE_MODE
#endif
#ifdef UFO_XC_SUPPORT_DUAL_MIU
#endif
#ifdef UFO_XC_SUPPORT_HDMI_DOWNSCALE_OUTPUT_POLICY
#endif
#ifdef _XC_C_
#else
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef __USBCOMMON_H
#if 1
#else
#endif
#if 1
#else
#endif
#ifndef U32
#endif
#ifndef U16
#endif
#ifndef U8
#endif
#ifndef S32
#endif
#ifndef S16
#endif
#ifndef S8
#endif
#ifndef __u32
#endif
#ifndef __u16
#endif
#ifndef __u8
#endif
#ifndef __s32
#endif
#ifndef __s16
#endif
#ifndef __s8
#endif
#ifndef BOOL
#endif
#ifndef BOOLEAN
#endif
#ifdef MS_DEBUG
#else
#endif
#if defined(CHIP_U4)
#elif defined(CHIP_K1)
#elif defined(CHIP_K2)
#elif defined(CHIP_KAPPA)
#elif defined(CHIP_KELTIC)
#elif defined(CHIP_KENYA)
#elif defined(CHIP_KRITI)
#elif defined(CHIP_KRATOS)
#elif defined(CHIP_KAISER)
#elif defined(CHIP_KERES)
#elif defined(CHIP_KIRIN)
#elif defined(CHIP_KRIS)
#elif defined(CHIP_KIWI)
#elif defined(CHIP_CLIPPERS)
#elif defined(CHIP_KAYLA)
#elif defined(CHIP_KANO)
#elif defined(CHIP_K6)
#elif defined(CHIP_CURRY)
#elif defined(CHIP_K6LITE)
#elif defined(CHIP_K5TN)
#else
#endif
#endif
#ifdef _DRV_SERFLASH_H_
#ifdef __cplusplus
#endif
#ifdef MXIC_IBPM
#endif
#ifdef MUNINN_ENABLE
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _APICEC_H_
#ifdef __cplusplus
#endif
#if(HDMI_CEC_VERSION >= CEC_VERSION_13a)
#endif
#if(HDMI_CEC_VERSION >= CEC_VERSION_13a)
#endif
#if(HDMI_CEC_VERSION >= CEC_VERSION_13a)
#endif
#if(HDMI_CEC_VERSION >= CEC_VERSION_13a)
#endif
#if(HDMI_CEC_VERSION >= CEC_VERSION_13a)
#endif
#if ENABLE_CEC_MULTIPLE
#endif
#if ENABLE_CEC_MULTIPLE
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_RTC_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_PCMCIA_H_
#ifdef __cplusplus
#endif
#if PCMCIA_IRQ_ENABLE
#endif
#if 0
#if PCMCIA_IRQ_ENABLE
#endif
#else
#if PCMCIA_IRQ_ENABLE
#endif
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef __DRVGPIO_H__
#ifdef __cplusplus
#endif
EXPORT_SYMBOL(MDrv_GPIO_SetIOMapBase);
EXPORT_SYMBOL(MDrv_GPIO_GetLibVer);
EXPORT_SYMBOL(MDrv_GPIO_SetDbgLevel);
EXPORT_SYMBOL(MDrv_GPIO_GetInfo);
EXPORT_SYMBOL(MDrv_GPIO_GetStatus);
EXPORT_SYMBOL(MDrv_GPIO_SetPowerState);
#ifdef __cplusplus
#endif
#endif
#ifdef _DRVEMMFLT_H_
#ifdef __cplusplus
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_CPU_V2_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_HDMITX_NDS_H_
#ifdef CUSTOMER_NDS
#endif
#if 0
#ifdef __cplusplus
#endif
#ifdef CUSTOMER_NDS
#ifdef __HDMI_H__
#endif
#endif
#ifdef __cplusplus
#endif
#endif
#endif
#ifdef _DRV_HWI2C_V2_H_
#ifdef __cplusplus
#endif
#if defined(UFO_PUBLIC_HEADER_212) || defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_500_3)
#if 0
#endif
#if 1
#endif
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_MBX_H
#ifdef _DRV_MBX_C
#else
#endif
#ifdef __cplusplus
#endif
#if 1
#endif
#if 1
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_XC_CUS_H_
#ifdef __cplusplus
#endif
#ifdef STELLAR
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef __PRANA_DEVICE_H__
#ifndef POLLIN
#endif
#ifndef POLLOUT
#endif
#ifndef POLLHUP
#endif
#ifndef MSOS_TYPE_LINUX_KERNEL
#endif
#endif
#ifdef _DRV_PVR_IFRAMELUT_V2_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRVAESDMA_H_
#ifdef __cplusplus
#endif
#if (SHA_WAIT==1)
#if defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_500_3)
#elif defined(UFO_PUBLIC_HEADER_300)
#else
#endif
#endif
#if defined(UFO_PUBLIC_HEADER_500_3)||defined(UFO_PUBLIC_HEADER_700)
#else
#endif
#ifndef MSOS_TYPE_NUTTX
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_700)||defined(UFO_PUBLIC_HEADER_500)
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_SWI2C_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_XC_ADC_EX_H_
#ifdef __cplusplus
#endif
EXPORT_SYMBOL(MApi_XC_ADC_EX_SetCVBSOut);
EXPORT_SYMBOL(MApi_XC_ADC_EX_IsCVBSOutEnabled);
EXPORT_SYMBOL(MApi_XC_ADC_EX_SetPcClock);
EXPORT_SYMBOL(MApi_XC_ADC_EX_SetPhase);
EXPORT_SYMBOL(MApi_XC_ADC_EX_SetPhaseEx);
EXPORT_SYMBOL(MApi_XC_ADC_EX_GetPhaseRange);
EXPORT_SYMBOL(MApi_XC_ADC_EX_GetPhase);
EXPORT_SYMBOL(MApi_XC_ADC_EX_GetPhaseEx);
EXPORT_SYMBOL(MApi_XC_ADC_EX_IsScartRGB);
EXPORT_SYMBOL(MApi_XC_ADC_EX_GetPcClock);
EXPORT_SYMBOL(MApi_XC_ADC_EX_GetSoGLevelRange);
EXPORT_SYMBOL(MApi_XC_ADC_EX_SetSoGLevel);
EXPORT_SYMBOL(MApi_XC_ADC_EX_PowerOff);
EXPORT_SYMBOL(MApi_XC_ADC_EX_GetDefaultGainOffset);
EXPORT_SYMBOL(MApi_XC_ADC_EX_GetMaximalOffsetValue);
EXPORT_SYMBOL(MApi_XC_ADC_EX_GetMaximalGainValue);
EXPORT_SYMBOL(MApi_XC_ADC_EX_GetCenterGain);
EXPORT_SYMBOL(MApi_XC_ADC_EX_GetCenterOffset);
EXPORT_SYMBOL(MApi_XC_ADC_EX_SetGain);
EXPORT_SYMBOL(MApi_XC_ADC_EX_SetOffset);
EXPORT_SYMBOL(MApi_XC_ADC_EX_AdjustGainOffset);
EXPORT_SYMBOL(MApi_XC_ADC_EX_Source_Calibrate);
EXPORT_SYMBOL(MApi_XC_ADC_EX_SetSoGCal);
EXPORT_SYMBOL(MApi_XC_ADC_EX_SetRGB_PIPE_Delay);
EXPORT_SYMBOL(MApi_XC_ADC_EX_ScartRGB_SOG_ClampDelay);
EXPORT_SYMBOL(MApi_XC_ADC_EX_Set_YPbPrLooseLPF);
EXPORT_SYMBOL(MApi_XC_ADC_EX_Set_SOGBW);
EXPORT_SYMBOL(MApi_XC_ADC_EX_SetClampDuration);
EXPORT_SYMBOL(MApi_XC_ADC_EX_EnableHWCalibration);
#ifdef __cplusplus
#endif
#endif
#ifdef __MLOG_H__
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_XC_DBC_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_MSPI_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_PM_H_
#ifdef __cplusplus
#endif
#ifdef __AEONR2__
#endif
#if defined(MSOS_TYPE_ECOS)
#else
#endif
#if defined(UFO_PM_BACKLIGHT_CTRL)
#endif
#ifdef MSOS_TYPE_LINUX
#endif
#if defined(UFO_PM_BACKLIGHT_CTRL)
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_XC_VMARK_H_
#ifdef __cplusplus
#endif
#ifdef UFO_XC_VMARK
EXPORT_SYMBOL(MApi_XC_VMark_SetParameters);
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _XC_ADC_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_RVD_H_
#ifdef __cplusplus
#endif
#if 0
#if defined(REDLION_LINUX_KERNEL_ENVI)
#else
#endif
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef APIXC_DWIN_H
#ifdef __cplusplus
#endif
EXPORT_SYMBOL(MApi_XC_DIP_QueryResource);
EXPORT_SYMBOL(MApi_XC_DIP_GetResource);
EXPORT_SYMBOL(MApi_XC_DIP_ReleaseResource);
EXPORT_SYMBOL(MApi_XC_DIP_CMDQ_SetAction);
EXPORT_SYMBOL(MApi_XC_DIP_CMDQ_SetOperations);
EXPORT_SYMBOL(MApi_XC_DIP_InitByDIP);
EXPORT_SYMBOL(MApi_XC_DIP_SetFrameBufferAddress);
EXPORT_SYMBOL(MApi_XC_DIP_SetInputSource);
EXPORT_SYMBOL(MApi_XC_DIP_GetInputSource);
EXPORT_SYMBOL(MApi_XC_DIP_DisableInputSource);
EXPORT_SYMBOL(MApi_XC_DIP_SetWindow);
EXPORT_SYMBOL(MApi_XC_DIP_GetDEWindow);
EXPORT_SYMBOL(MApi_XC_DIP_SetCaptureWindowVstart);
EXPORT_SYMBOL(MApi_XC_DIP_SetCaptureWindowHstart);
EXPORT_SYMBOL(MApi_XC_DIP_SetCaptureWindowVsize);
EXPORT_SYMBOL(MApi_XC_DIP_SetCaptureWindowHsize);
EXPORT_SYMBOL(MApi_XC_DIP_GetCaptureWindow);
EXPORT_SYMBOL(MApi_XC_DIP_EnableR2Y);
EXPORT_SYMBOL(MApi_XC_DIP_SWReset);
EXPORT_SYMBOL(MApi_XC_DIP_FrameRateCtrl);
EXPORT_SYMBOL(MApi_XC_DIP_CapOneFrame);
EXPORT_SYMBOL(MApi_XC_DIP_CapOneFrameFast);
EXPORT_SYMBOL(MApi_XC_DIP_ClearInt);
EXPORT_SYMBOL(MApi_XC_DIP_Ena);
EXPORT_SYMBOL(MApi_XC_DIP_EnaInt);
EXPORT_SYMBOL(MApi_XC_DIP_SetSourceScanType);
EXPORT_SYMBOL(MApi_XC_DIP_EnaInterlaceWrite);
EXPORT_SYMBOL(MApi_XC_DIP_SetOutputDataFmt);
EXPORT_SYMBOL(MApi_XC_DIP_SetDIPWinProperty);
EXPORT_SYMBOL(MApi_XC_DIP_EnableY2R);
EXPORT_SYMBOL(MApi_XC_DIP_SetAlpha);
EXPORT_SYMBOL(MApi_XC_DIP_SwapUV);
EXPORT_SYMBOL(MApi_XC_DIP_SwapYC);
EXPORT_SYMBOL(MApi_XC_DIP_SwapRGB);
EXPORT_SYMBOL(MApi_XC_DIP_SetWinProperty_Ex);
EXPORT_SYMBOL(MApi_XC_DIP_SetOutputCapture);
EXPORT_SYMBOL(MApi_XC_DIP_SetMirror);
EXPORT_SYMBOL(MApi_XC_DIP_SetDIPRProperty);
EXPORT_SYMBOL(MApi_XC_DIP_SetDIPRProperty_EX);
EXPORT_SYMBOL(MApi_XC_DIP_InterruptAttach);
EXPORT_SYMBOL(MApi_XC_DIP_InterruptDetach);
EXPORT_SYMBOL(MApi_XC_DIP_SetPowerState);
EXPORT_SYMBOL(MApi_XC_DIP_Rotation);
EXPORT_SYMBOL(MApi_XC_DIP_SetPinpon);
EXPORT_SYMBOL(MApi_XC_DIP_SetHVSP);
EXPORT_SYMBOL(MApi_XC_DIP_Set420TileBlock);
EXPORT_SYMBOL(MApi_XC_DIP_SetDbgLevel);
EXPORT_SYMBOL(MApi_XC_DIP_GetIntStatus);
EXPORT_SYMBOL(MApi_XC_DIP_GetBufInfo);
EXPORT_SYMBOL(MDrv_XC_DIP_GetResourceByPipeID);
EXPORT_SYMBOL(MDrv_XC_DIP_ConfigPipe);
EXPORT_SYMBOL(MDrv_XC_DIP_CheckPipe);
#ifdef __cplusplus
#endif
#endif
#ifdef _API_XC_MODEPARSE_EX_H_
#ifdef __cplusplus
#endif
EXPORT_SYMBOL(MApi_XC_ModeParse_EX_Init);
EXPORT_SYMBOL(MApi_XC_ModeParse_EX_MatchMode);
EXPORT_SYMBOL(MApi_XC_ModeParse_EX_MatchModeEx);
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_AESDMA_V2_H_
#ifdef __cplusplus
#endif
#ifdef BIFROST_32BIT_MODE
#else
#endif
#ifdef BIFROST_32BIT_MODE
#else
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRVCMAPOOL_H_
#if defined (MSOS_TYPE_LINUX) || defined (MSOS_TYPE_OPTEE)
#ifdef __cplusplus
#endif
#if defined(CHIP_MONET) || defined(CHIP_MANHATTAN)
#else
#endif
#ifdef UFO_PUBLIC_HEADER_300
#else
#endif
#ifdef UFO_PUBLIC_HEADER_300
#else
#endif
#ifdef UFO_PUBLIC_HEADER_300
#else
#endif
#ifdef __cplusplus
#endif
#endif
#endif
#ifdef _API_XC_PANEL_V2_H_
#ifdef __cplusplus
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_IR_H_
#ifdef __cplusplus
#endif
#if 1
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef __DRV_MMIO_H__
#ifdef __cplusplus
#endif
EXPORT_SYMBOL(MDrv_MMIO_Init);
EXPORT_SYMBOL(MDrv_MMIO_GetBASE);
EXPORT_SYMBOL(MDrv_MMIO_Close);
#ifdef __cplusplus
#endif
#endif
#ifdef __DRV_DSCMB_H__
#ifdef __cplusplus
#endif
#if !(defined(MSOS_TYPE_OPTEE) || defined(MSOS_TYPE_NUTTX))
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_MFE_FRMFORMAT_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_MSPI_V2_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_DVBT_V2_H_
#ifdef __cplusplus
#endif
#if(0)
#endif
#if(0)
#endif
#if(0)
#endif
#if(0)
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef __DRV_VBI_V2_H__
#ifdef __cplusplus
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#else
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_XC_PANEL_H_
#ifdef __cplusplus
#endif
#ifdef _API_XC_PANEL_C_
#else
#endif
#ifdef MSOS_TYPE_LINUX_KERNEL
#else
#endif
#ifdef _MS_VERSION_H_
#endif
#if defined(UFO_PUBLIC_HEADER_500)
#endif
#if !defined(CONFIG_MBOOT)
#endif
#if !(defined(UFO_PUBLIC_HEADER_212) || defined(UFO_PUBLIC_HEADER_300))
#if !defined (__aarch64__)
#endif
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#else
#endif
EXPORT_SYMBOL(MApi_PNL_GetLibVer);
EXPORT_SYMBOL(MApi_PNL_GetInfo);
EXPORT_SYMBOL(MApi_PNL_GetStatus);
EXPORT_SYMBOL(MApi_PNL_GetStatusEx);
EXPORT_SYMBOL(MApi_PNL_SetDbgLevel);
EXPORT_SYMBOL(MApi_PNL_IOMapBaseInit);
EXPORT_SYMBOL(MApi_PNL_PreInit);
#ifndef _API_XC_PANEL_C_
#endif
EXPORT_SYMBOL(MApi_PNL_Init_Ex);
EXPORT_SYMBOL(MApi_PNL_GetConfig);
EXPORT_SYMBOL(MApi_PNL_SetOutput);
EXPORT_SYMBOL(MApi_PNL_ChangePanelType);
EXPORT_SYMBOL(MApi_PNL_TCONMAP_DumpTable);
EXPORT_SYMBOL(MApi_PNL_TCONMAP_Power_Sequence);
EXPORT_SYMBOL(MApi_PNL_TCON_Count_Reset);
EXPORT_SYMBOL(MApi_PNL_TCON_Init);
EXPORT_SYMBOL(MApi_PNL_GetDstInfo);
EXPORT_SYMBOL(MApi_PNL_Control_Out_Swing);
EXPORT_SYMBOL(MApi_PNL_ForceSetPanelDCLK);
EXPORT_SYMBOL(MApi_PNL_ForceSetPanelHStart);
EXPORT_SYMBOL(MApi_PNL_SetOutputPattern);
EXPORT_SYMBOL(MApi_Mod_Calibration_Setting);
EXPORT_SYMBOL(MApi_Mod_Do_Calibration);
EXPORT_SYMBOL(MApi_BD_LVDS_Output_Type);
EXPORT_SYMBOL(MApi_PNL_SetLPLLTypeExt);
EXPORT_SYMBOL(MApi_PNL_Init_MISC);
EXPORT_SYMBOL(MApi_PNL_GetMiscStatus);
EXPORT_SYMBOL(MApi_PNL_MOD_OutputConfig_User);
EXPORT_SYMBOL(MApi_PNL_MOD_OutputChannelOrder);
EXPORT_SYMBOL(MApi_PNL_HWLVDSReservedtoLRFlag);
EXPORT_SYMBOL(MApi_MOD_PVDD_Power_Setting);
EXPORT_SYMBOL(MApi_PNL_SetSSC_En);
EXPORT_SYMBOL(MApi_PNL_SetSSC_Fmodulation);
EXPORT_SYMBOL(MApi_PNL_SetSSC_Rdeviation);
EXPORT_SYMBOL(MApi_PNL_SetOSDSSC_En);
EXPORT_SYMBOL(MApi_PNL_SetOSDSSC_Fmodulation);
EXPORT_SYMBOL(MApi_PNL_SkipTimingChange);
EXPORT_SYMBOL(MApi_PNL_PreSetModeOn);
EXPORT_SYMBOL(MApi_PNL_OverDriver_Init);
EXPORT_SYMBOL(MApi_PNL_OverDriver_Enable);
EXPORT_SYMBOL(MApi_PNL_Get_TCON_Capability);
EXPORT_SYMBOL(MApi_PNL_SetPairSwap);
EXPORT_SYMBOL(MApi_PNL_SetExt_LPLL_Type);
EXPORT_SYMBOL(MApi_PNL_CalExtLPLLSETbyDClk);
EXPORT_SYMBOL(MApi_PNL_EnableInternalTermination);
EXPORT_SYMBOL(MApi_PNL_SetPowerState);
EXPORT_SYMBOL(MApi_PNL_SetDiffSwingLevel);
EXPORT_SYMBOL(MApi_PNL_OutputDeviceHandshake);
EXPORT_SYMBOL(MApi_PNL_OutputDeviceOCHandshake);
EXPORT_SYMBOL(MApi_PNL_SetOutputInterlaceTiming);
EXPORT_SYMBOL(MApi_PNL_GetPanelData);
EXPORT_SYMBOL(MApi_PNL_DumpPanelData);
EXPORT_SYMBOL(MApi_PNL_SetSSC);
EXPORT_SYMBOL(MApi_PNL_GetPanelOnTiming);
EXPORT_SYMBOL(MApi_PNL_GetPanelOffTiming);
EXPORT_SYMBOL(MApi_PNL_GetPanelDimCtrl);
EXPORT_SYMBOL(MApi_PNL_GetAllGammaTbl);
EXPORT_SYMBOL(MApi_PNL_EnablePanel);
EXPORT_SYMBOL(MApi_PNL_SetGammaTbl);
EXPORT_SYMBOL(MApi_PNL_GetGammaTbl);
EXPORT_SYMBOL(MApi_PNL_SetGammaValue);
EXPORT_SYMBOL(MApi_PNL_Check_VBY1_Handshake_Status);
EXPORT_SYMBOL(MApi_PNL_SetVideoHWTraining);
EXPORT_SYMBOL(MApi_PNL_SetOSDHWTraining);
EXPORT_SYMBOL(MApi_PNL_GetVideoHWTraining_Status);
EXPORT_SYMBOL(MApi_PNL_GetOSDHWTraining_Status);
EXPORT_SYMBOL(MApi_PNL_GetOutputInterlaceTiming);
EXPORT_SYMBOL(MApi_PNL_Setting);
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_ATSC_V2_H_
#ifdef __cplusplus
#endif
#ifndef DLL_PUBLIC
#endif
#ifdef UTPA2
#else
#endif
#ifdef UTPA2
#else
#endif
#ifdef UTPA2
#else
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_SEAL_H_
#ifdef __cplusplus
#endif
#if defined(UFO_PUBLIC_HEADER_500)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300)||defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300)||defined(UFO_PUBLIC_HEADER_700)
#if defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_700)
#endif
#else
#endif
#ifdef MSOS_TYPE_OPTEE
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_GPD_H_
#ifdef __cplusplus
#endif
#if !defined(U8)
#endif
#if !defined(U16)
#endif
#if !defined(U32)
#endif
#if !defined(S8)
#endif
#if !defined(S16)
#endif
#if !defined(S32)
#endif
#if !defined(TRUE) && !defined(FALSE)
#endif
#ifndef GPD_SUPPORT_MGIF
#endif
#ifndef GPD_NO_SUPPORT_MGIF
#endif
#if 1
EXPORT_SYMBOL(MApi_GPD_Init);
EXPORT_SYMBOL(MApi_GPD_InputSource);
EXPORT_SYMBOL(MApi_GPD_OutputDecode);
EXPORT_SYMBOL(MApi_GPD_OutputDecodeROI);
EXPORT_SYMBOL(MApi_GPD_OutputDecodeMGIF);
EXPORT_SYMBOL(MApi_GPD_GetDuration);
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MApi_GPD_SetGIFMode);
#else
EXPORT_SYMBOL(MApi_GPD_SetGIFMode);
#endif
#else
#endif
EXPORT_SYMBOL(MApi_GPD_ScalingEnable);
EXPORT_SYMBOL(MApi_GPD_ScalingDisable);
EXPORT_SYMBOL(MApi_GPD_SetControl);
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MApi_GPD_GetControl);
#else
EXPORT_SYMBOL(MApi_GPD_GetControl);
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MApi_GPD_GetCRCResult);
#else
EXPORT_SYMBOL(MApi_GPD_GetCRCResult);
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_DVBS_V2_H_
#ifdef __cplusplus
#endif
#ifndef DLL_PUBLIC
#endif
#ifdef UFO_SUPPORT_VCM
#endif
#ifdef UFO_DEMOD_DVBS_SUPPORT_DMD_INT
#endif
#ifdef UFO_DEMOD_GetParam_NEW_FLOW
#endif
#ifdef UFO_DEMOD_GET_AGC_INFO
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRVIRTX_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_PARFLASH_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _GOP_H_
#ifdef __cplusplus
#endif
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
#else
#endif
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
#else
#endif
#if (defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300))
#else
#endif
#ifdef __cplusplus
#endif
#endif
#if defined(UFO_PUBLIC_HEADER_700)
#endif
#if defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_700) || defined(UFO_PUBLIC_HEADER_500)
#endif
#if defined(UFO_PUBLIC_HEADER_300)|| defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_700) || defined(UFO_PUBLIC_HEADER_500)
#endif
#if defined(UFO_PUBLIC_HEADER_212) ||defined(UFO_PUBLIC_HEADER_500_3) || defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_500)
#endif
#ifdef _DRV_HWI2C_H_
#ifdef __cplusplus
#endif
EXPORT_SYMBOL(MDrv_HWI2C_Init);
EXPORT_SYMBOL(MDrv_HWI2C_Start);
EXPORT_SYMBOL(MDrv_HWI2C_Stop);
EXPORT_SYMBOL(MDrv_HWI2C_GetPortIndex);
EXPORT_SYMBOL(MDrv_HWI2C_SelectPort);
EXPORT_SYMBOL(MDrv_HWI2C_SetClk);
EXPORT_SYMBOL(MDrv_HWI2C_SetReadMode);
EXPORT_SYMBOL(MDrv_HWI2C_WriteByte);
EXPORT_SYMBOL(MDrv_HWI2C_WriteBytes);
EXPORT_SYMBOL(MDrv_HWI2C_ReadByte);
EXPORT_SYMBOL(MDrv_HWI2C_ReadBytes);
EXPORT_SYMBOL(MDrv_HWI2C_SelectPort1);
EXPORT_SYMBOL(MDrv_HWI2C_SetClkP1);
EXPORT_SYMBOL(MDrv_HWI2C_SetReadModeP1);
EXPORT_SYMBOL(MDrv_HWI2C_WriteByteP1);
EXPORT_SYMBOL(MDrv_HWI2C_WriteBytesP1);
EXPORT_SYMBOL(MDrv_HWI2C_ReadByteP1);
EXPORT_SYMBOL(MDrv_HWI2C_ReadBytesP1);
EXPORT_SYMBOL(MDrv_HWI2C_SetDbgLevel);
EXPORT_SYMBOL(MDrv_HWI2C_GetLibVer);
EXPORT_SYMBOL(MDrv_HWI2C_GetStatus);
EXPORT_SYMBOL(MDrv_HWI2C_GetInfo);
EXPORT_SYMBOL(MDrv_HWI2C_ConfigDelay);
EXPORT_SYMBOL(MDrv_HWI2C_SetPowerState);
EXPORT_SYMBOL(MDrv_HWI2C_ConfigDelay_EX);
EXPORT_SYMBOL(MDrv_HWI2C_GetStatus_EX);
EXPORT_SYMBOL(MDrv_HWI2C_Init_U2K);
EXPORT_SYMBOL(MDrv_HWI2C_Start_U2K);
EXPORT_SYMBOL(MDrv_HWI2C_Stop_U2K);
EXPORT_SYMBOL(MDrv_HWI2C_WriteBytes_U2K);
EXPORT_SYMBOL(MDrv_HWI2C_ReadBytes_U2K);
EXPORT_SYMBOL(MDrv_HWI2C_SelectPort_U2K);
EXPORT_SYMBOL(MDrv_HWI2C_SetClk_U2K);
#ifdef __cplusplus
#endif
#endif
#ifdef _DRVNSK2_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_VBI_H
#ifdef __cplusplus
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#else
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _APIXC_AUTO_H_
#ifdef __cplusplus
#endif
EXPORT_SYMBOL(MApi_XC_Auto_Geometry);
EXPORT_SYMBOL(MApi_XC_Auto_Geometry_Ex);
EXPORT_SYMBOL(MApi_XC_Auto_StopAutoGeometry);
EXPORT_SYMBOL(MApi_XC_Auto_GainOffset);
EXPORT_SYMBOL(MApi_XC_Auto_GetHWFixedGainOffset);
EXPORT_SYMBOL(MApi_XC_Auto_SetValidData);
EXPORT_SYMBOL(MApi_XC_Auto_AutoOffset);
EXPORT_SYMBOL(MApi_XC_Auto_DetectWidth);
EXPORT_SYMBOL(MApi_XC_Auto_SetCalibrationMode);
EXPORT_SYMBOL(MApi_XC_Auto_GetCalibrationMode);
EXPORT_SYMBOL(MApi_XC_AUTO_GetSyncInfo);
#ifdef __cplusplus
#endif
#endif
#ifdef _API_DMS_TYPE_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_XC_ACE_EX_H_
#ifdef __cplusplus
#endif
#if !defined (__aarch64__)
#endif
#if !defined (__aarch64__)
#endif
EXPORT_SYMBOL(MApi_XC_ACE_EX_Init);
EXPORT_SYMBOL(MApi_XC_ACE_EX_Exit);
EXPORT_SYMBOL(MApi_XC_ACE_EX_DMS);
EXPORT_SYMBOL(MApi_XC_ACE_EX_SetPCYUV2RGB);
EXPORT_SYMBOL(MApi_XC_ACE_EX_SelectYUVtoRGBMatrix);
EXPORT_SYMBOL(MApi_XC_ACE_EX_SetColorCorrectionTable);
EXPORT_SYMBOL(MApi_XC_ACE_EX_SetPCsRGBTable);
EXPORT_SYMBOL(MApi_XC_ACE_EX_GetColorMatrix);
EXPORT_SYMBOL(MApi_XC_ACE_EX_SetColorMatrix);
EXPORT_SYMBOL(MApi_XC_ACE_EX_SetBypassColorMatrix);
EXPORT_SYMBOL(MApi_XC_ACE_EX_SetIHC);
EXPORT_SYMBOL(MApi_XC_ACE_EX_PatchDTGColorChecker);
EXPORT_SYMBOL(MApi_XC_ACE_EX_GetACEInfo);
EXPORT_SYMBOL(MApi_XC_ACE_EX_GetACEInfoEx);
EXPORT_SYMBOL(MApi_XC_ACE_EX_PicSetContrast);
EXPORT_SYMBOL(MApi_XC_ACE_EX_PicSetBrightness);
EXPORT_SYMBOL(MApi_XC_ACE_EX_PicSetBrightnessPrecise);
EXPORT_SYMBOL(MApi_XC_ACE_EX_PicSetPreYOffset);
EXPORT_SYMBOL(MApi_XC_ACE_EX_PicGetPreYOffset);
EXPORT_SYMBOL(MApi_XC_ACE_EX_PicSetHue);
EXPORT_SYMBOL(MApi_XC_ACE_EX_SkipWaitVsync);
EXPORT_SYMBOL(MApi_XC_ACE_EX_PicSetSaturation);
EXPORT_SYMBOL(MApi_XC_ACE_EX_PicSetSharpness);
EXPORT_SYMBOL(MApi_XC_ACE_EX_PicSetColorTemp);
EXPORT_SYMBOL(MApi_XC_ACE_EX_PicSetColorTempEx);
EXPORT_SYMBOL(MApi_XC_ACE_EX_PicSetPostColorTemp_V02);
#ifndef _API_XC_ACE_EX_C_
#ifndef EX_ENABLE_NEW_COLORTEMP_METHOD
#endif
#ifndef EX_ENABLE_PRECISE_RGBBRIGHTNESS
#endif
#if EX_ENABLE_NEW_COLORTEMP_METHOD
#if EX_ENABLE_PRECISE_RGBBRIGHTNESS
#else
#endif
#else
#if EX_ENABLE_PRECISE_RGBBRIGHTNESS
#endif
#endif
#endif
EXPORT_SYMBOL(MApi_XC_ACE_EX_SetFleshTone);
EXPORT_SYMBOL(MApi_XC_ACE_EX_SetBlackAdjust);
EXPORT_SYMBOL(MApi_XC_ACE_EX_Set_IHC_SRAM);
EXPORT_SYMBOL(MApi_XC_ACE_EX_Set_ICC_SRAM);
EXPORT_SYMBOL(MApi_XC_ACE_EX_EnableMWE);
EXPORT_SYMBOL(MApi_XC_ACE_EX_MWECloneVisualEffect);
EXPORT_SYMBOL(MApi_XC_ACE_EX_MWESetRegTable);
EXPORT_SYMBOL(MApi_XC_ACE_EX_SetMWELoadFromPQ);
EXPORT_SYMBOL(MApi_XC_ACE_EX_MWEStatus);
EXPORT_SYMBOL(MApi_XC_ACE_EX_MWEHandle);
EXPORT_SYMBOL(MApi_XC_ACE_EX_MWEFuncSel);
EXPORT_SYMBOL(MApi_XC_ACE_EX_MWESetDispWin);
EXPORT_SYMBOL(MApi_XC_ACE_EX_3DClonePQMap);
EXPORT_SYMBOL(MApi_XC_ACE_EX_DNR_Blending_NRTbl);
EXPORT_SYMBOL(MApi_XC_ACE_EX_DNR_Blending_MISC);
EXPORT_SYMBOL(MApi_XC_ACE_EX_DNR_Blending);
EXPORT_SYMBOL(MApi_XC_ACE_EX_DNR_Get_PrecisionShift);
EXPORT_SYMBOL(MApi_XC_ACE_EX_DNR_GetMotion_Weight);
EXPORT_SYMBOL(MApi_XC_ACE_EX_DNR_GetLuma_Weight);
EXPORT_SYMBOL(MApi_XC_ACE_EX_DNR_GetNoise_Weight);
EXPORT_SYMBOL(MApi_XC_ACE_EX_DNR_GetMotion);
EXPORT_SYMBOL(MApi_XC_ACE_EX_DNR_Init_Motion);
EXPORT_SYMBOL(MApi_XC_ACE_EX_DNR_Init_Luma);
EXPORT_SYMBOL(MApi_XC_ACE_EX_DNR_Init_Noise);
EXPORT_SYMBOL(MApi_XC_ACE_EX_DNR_SetParam);
EXPORT_SYMBOL(MApi_XC_ACE_EX_DNR_GetParam);
EXPORT_SYMBOL(MApi_XC_ACE_EX_DNR_SetNRTbl);
EXPORT_SYMBOL(MApi_XC_ACE_EX_GetLibVer);
EXPORT_SYMBOL(MApi_XC_ACE_EX_GetInfo);
EXPORT_SYMBOL(MApi_XC_ACE_EX_GetStatus);
EXPORT_SYMBOL(MApi_XC_ACE_EX_SetDbgLevel);
EXPORT_SYMBOL(MApi_XC_ACE_EX_ColorCorrectionTable);
EXPORT_SYMBOL(MApi_XC_ACE_EX_SetColorMatrixControl);
EXPORT_SYMBOL(MApi_XC_ACE_EX_SetRBChannelRange);
EXPORT_SYMBOL(MApi_XC_ACE_EX_PicSetPostColorTemp);
EXPORT_SYMBOL(MApi_XC_ACE_EX_PicSetPostColorTempEx);
EXPORT_SYMBOL(MApi_XC_ACE_EX_PicSetPostColorTemp2);
EXPORT_SYMBOL(MApi_XC_ACE_EX_PicSetPostColorTemp2Ex);
#ifdef __cplusplus
#endif
#endif
#ifdef _MS_OS_H_
#ifdef __cplusplus
#endif
#if defined(MSOS_TYPE_LINUX_KERNEL)
#endif
#ifdef CONFIG_MSOS_EVENTGROUP_MAX_SEL
#else
#endif
#if defined(__MIPS16E__)
#else
#endif
#ifdef MSOS_PERF_DEBUG
#else
#endif
#if defined (MSOS_TYPE_ECOS) || defined (MSOS_TYPE_LINUX) || defined (MSOS_TYPE_NOS) || defined (MSOS_TYPE_UCOS) || defined (MSOS_TYPE_NUTTX) || defined (MSOS_TYPE_OPTEE)
#ifdef MSOS_TYPE_UCOS
#elif defined(MSOS_TYPE_NUTTX)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#elif defined (MSOS_TYPE_LINUX_KERNEL)
#if 0
#endif
#elif defined (MSOS_TYPE_XXX)
#endif
EXPORT_SYMBOL(MsOS_Init);
EXPORT_SYMBOL(MsOS_CreateMemoryPool);
EXPORT_SYMBOL(MsOS_DeleteMemoryPool);
EXPORT_SYMBOL(MsOS_InfoMemoryPool);
EXPORT_SYMBOL(MsOS_AllocateMemory);
EXPORT_SYMBOL(MsOS_FreeMemory);
EXPORT_SYMBOL(MsOS_CreateTask);
EXPORT_SYMBOL(MsOS_DeleteTask);
EXPORT_SYMBOL(MsOS_YieldTask);
EXPORT_SYMBOL(MsOS_DelayTask);
EXPORT_SYMBOL(MsOS_DelayTaskUs);
EXPORT_SYMBOL(MsOS_DelayTaskUs_Poll);
EXPORT_SYMBOL(MsOS_ResumeTask);
EXPORT_SYMBOL(MsOS_SuspendTask);
EXPORT_SYMBOL(MsOS_InfoTaskID);
EXPORT_SYMBOL(MsOS_GetTaskStatus);
EXPORT_SYMBOL(MsOS_GetOSThreadID);
EXPORT_SYMBOL(MsOS_CreateMutex);
EXPORT_SYMBOL(MsOS_DeleteMutex);
EXPORT_SYMBOL(MsOS_ObtainMutex);
EXPORT_SYMBOL(MsOS_ReleaseMutex);
EXPORT_SYMBOL(MsOS_EnableCrossThreadReleaseMutex);
EXPORT_SYMBOL(MsOS_InfoMutex);
EXPORT_SYMBOL(MsOS_CreateSemaphore);
EXPORT_SYMBOL(MsOS_DeleteSemaphore);
EXPORT_SYMBOL(MsOS_ObtainSemaphore);
EXPORT_SYMBOL(MsOS_ReleaseSemaphore);
EXPORT_SYMBOL(MsOS_CreateEventGroup);
EXPORT_SYMBOL(MsOS_DeleteEventGroup);
EXPORT_SYMBOL(MsOS_SetEvent);
EXPORT_SYMBOL(MsOS_ClearEvent);
EXPORT_SYMBOL(MsOS_WaitEvent);
EXPORT_SYMBOL(MsOS_WaitEvent_Interrupt);
EXPORT_SYMBOL(MsOS_CreateTimer);
EXPORT_SYMBOL(MsOS_DeleteTimer);
EXPORT_SYMBOL(MsOS_StartTimer);
EXPORT_SYMBOL(MsOS_StopTimer);
EXPORT_SYMBOL(MsOS_ResetTimer);
EXPORT_SYMBOL(MsOS_GetSystemTime);
EXPORT_SYMBOL(MsOS_Timer_DiffTimeFromNow);
EXPORT_SYMBOL(MsOS_Timer_DiffTime);
EXPORT_SYMBOL(MsOS_AttachInterrupt);
EXPORT_SYMBOL(MsOS_AttachInterrupt_Shared);
EXPORT_SYMBOL(MsOS_DetachInterrupt);
EXPORT_SYMBOL(MsOS_EnableInterrupt);
EXPORT_SYMBOL(MsOS_DisableInterrupt);
EXPORT_SYMBOL(MsOS_In_Interrupt);
EXPORT_SYMBOL(MsOS_DisableAllInterrupts);
EXPORT_SYMBOL(MsOS_RestoreAllInterrupts);
EXPORT_SYMBOL(MsOS_EnableAllInterrupts);
#if defined(__aeon__)
#if defined(MSOS_TYPE_NUTTX)
#else
#endif
#ifdef __AEONR2__
#endif
#else
#endif
#if defined (__arm__) || defined (__aarch64__)
#else
#if defined(MSOS_TYPE_NUTTX)
#else
#endif
#endif
EXPORT_SYMBOL(MsOS_CPU_DisableInterrupt);
EXPORT_SYMBOL(MsOS_CPU_EnableInterrupt);
EXPORT_SYMBOL(MsOS_CPU_RestoreInterrupt);
EXPORT_SYMBOL(MsOS_CPU_MaskAllInterrupt);
EXPORT_SYMBOL(MsOS_CPU_MaskInterrupt);
EXPORT_SYMBOL(MsOS_CPU_UnMaskInterrupt);
EXPORT_SYMBOL(MsOS_CPU_LockInterrupt);
EXPORT_SYMBOL(MsOS_CPU_UnLockInterrupt);
EXPORT_SYMBOL(MsOS_CPU_AttachInterrupt);
EXPORT_SYMBOL(MsOS_CPU_DetachInterrupt);
EXPORT_SYMBOL(MsOS_CPU_AttachException);
EXPORT_SYMBOL(MsOS_CPU_DetachExceptiont);
EXPORT_SYMBOL(MsOS_CPU_SetEBASE);
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MsOS_Dcache_Flush);
#else
EXPORT_SYMBOL(MsOS_Dcache_Flush);
#endif
EXPORT_SYMBOL(MsOS_Dcache_Flush_All);
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MsOS_Dcache_Invalidate);
#else
EXPORT_SYMBOL(MsOS_Dcache_Invalidate);
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MsOS_Dcache_Writeback);
#else
EXPORT_SYMBOL(MsOS_Dcache_Writeback);
#endif
#if defined(CHIP_T12) || defined(CHIP_T8) || defined(CHIP_J2) || defined(CHIP_A2) || defined(CHIP_A5) || defined(CHIP_A3) || defined(CHIP_Edison)
#endif
EXPORT_SYMBOL(MsOS_Sync);
EXPORT_SYMBOL(MsOS_MPool_SetDbgLevel);
EXPORT_SYMBOL(MsOS_MPool_IsInitialized);
EXPORT_SYMBOL(MsOS_MPool_Init);
EXPORT_SYMBOL(MsOS_MPool_Close);
#ifdef UFO_PUBLIC_HEADER_700
EXPORT_SYMBOL(MsOS_MPool_InfoMsg);
#else
EXPORT_SYMBOL(MsOS_MPool_InfoMsg);
#endif
EXPORT_SYMBOL(MsOS_MPool_VA2PA);
EXPORT_SYMBOL(MsOS_MPool_PA2KSEG1);
EXPORT_SYMBOL(MsOS_MPool_PA2KSEG0);
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MsOS_MPool_Dcache_Flush);
#else
EXPORT_SYMBOL(MsOS_MPool_Dcache_Flush);
#endif
EXPORT_SYMBOL(MsOS_MPool_Dcache_Flush_All);
#if defined(UFO_PUBLIC_HEADER_700)
EXPORT_SYMBOL(MsOS_MPool_Mapping);
#elif defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MsOS_MPool_Mapping);
#else
EXPORT_SYMBOL(MsOS_MPool_Mapping);
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MsOS_MPool_UnMapping);
#else
EXPORT_SYMBOL(MsOS_MPool_UnMapping);
#endif
#if defined(UFO_PUBLIC_HEADER_700)
EXPORT_SYMBOL(MsOS_MPool_Mapping_Dynamic);
#elif defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MsOS_MPool_Mapping_Dynamic);
#else
EXPORT_SYMBOL(MsOS_MPool_Mapping_Dynamic);
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
EXPORT_SYMBOL(MsOS_MPool_Kernel_Detect);
#else
EXPORT_SYMBOL(MsOS_MPool_Kernel_Detect);
#endif
#if defined(MSOS_TYPE_LINUX)
#ifdef ENABLE_KERNEL_DLMALLOC
#endif
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_300) || defined(UFO_PUBLIC_HEADER_212)
#else
#endif
EXPORT_SYMBOL(MsOS_SHM_Init);
EXPORT_SYMBOL(MsOS_SHM_GetId);
EXPORT_SYMBOL(MsOS_SHM_FreeId);
#if defined (MSOS_TYPE_NOS)
#endif
EXPORT_SYMBOL(MsOS_VA2PA);
EXPORT_SYMBOL(MsOS_PA2KSEG0);
EXPORT_SYMBOL(MsOS_PA2KSEG1);
EXPORT_SYMBOL(MsOS_FlushMemory);
EXPORT_SYMBOL(MsOS_ReadMemory);
#if defined (MSOS_TYPE_LINUX_KERNEL)
#endif
#if defined (MS_DEBUG)
#elif defined (MS_OPTIMIZE)
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRVDIP_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef __API_DMX_TSIO_H__
#ifdef __cplusplus
#endif
#ifdef DMX_UTOPIA_20
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _ACP_V2_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRVAUDSP_H_
#ifdef __cplusplus
#endif
EXPORT_SYMBOL(MDrv_AUD_GetLibVer);
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_IRQ_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_GOP_SC_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _API_MBX_H
#ifdef __cplusplus
#endif
EXPORT_SYMBOL(MApi_MBX_Init);
EXPORT_SYMBOL(MApi_MBX_Enable);
#ifdef __cplusplus
#endif
#endif
#ifdef _API_JPEG_V2_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_AUDIO_IF_H_
#ifdef __cplusplus
#endif
#if defined(UFO_PUBLIC_HEADER_300)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_212) || defined(UFO_PUBLIC_HEADER_500_3)
#endif
#if defined(UFO_PUBLIC_HEADER_212) || defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_212)
#endif
#if defined(UFO_PUBLIC_HEADER_500) || defined(UFO_PUBLIC_HEADER_700)
#else
#endif
#if defined(UFO_PUBLIC_HEADER_212) ||defined(UFO_PUBLIC_HEADER_500_3)
#else
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_NDS_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _XC_VMARK_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_ISDBT_H_
#if !defined UTPA2
#else
#endif
#if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN
#ifndef MSIF_TAG
#endif
#endif
#if DMD_ISDBT_UTOPIA2_EN || DMD_ISDBT_STR_EN
#endif
#ifndef DLL_PUBLIC
#endif
#if DMD_ISDBT_MULTI_DMD_EN
#else
#endif
#if DMD_ISDBT_UTOPIA_EN || DMD_ISDBT_UTOPIA2_EN
#endif
#ifndef BIT_
#endif
#ifdef UTPA2
#else
#endif
#ifdef UTPA2
#endif
#if !DMD_ISDBT_UTOPIA_EN && !DMD_ISDBT_UTOPIA2_EN
#endif
#if DMD_ISDBT_STR_EN
#endif
#ifdef __cplusplus
#endif
#ifndef MSOS_TYPE_LINUX_KERNEL
#if DMD_ISDBT_STR_EN
#endif
#if DMD_ISDBT_STR_EN
#endif
#endif
#ifdef UTPA2
#if DMD_ISDBT_STR_EN
#endif
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRVCMDQ_H_
#ifdef __cplusplus
#endif
EXPORT_SYMBOL(MDrv_CMDQ_Init);
EXPORT_SYMBOL(MDrv_CMDQ_GetLibVer);
EXPORT_SYMBOL(MDrv_CMDQ_Start);
EXPORT_SYMBOL(MDrv_CMDQ_Get_Memory_Size);
EXPORT_SYMBOL(MDrv_CMDQ_Set_Buffer);
EXPORT_SYMBOL(MDrv_CMDQ_Reset);
EXPORT_SYMBOL(MDrv_CMDQ_Exit);
EXPORT_SYMBOL(MDrv_CMDQ_Stop);
EXPORT_SYMBOL(MDrv_CMDQ_SetDbgLevel);
EXPORT_SYMBOL(MDrv_CMDQ_Receive);
EXPORT_SYMBOL(MDrv_CMDQ_Transfer);
EXPORT_SYMBOL(MDrv_CMDQ_Set_dummy_Address);
EXPORT_SYMBOL(MDrv_CMDQ_Set_MIU_SELECT);
EXPORT_SYMBOL(MDrv_CMDQ_Set_timer_ratio);
EXPORT_SYMBOL(MDrv_CMDQ_Printf_Crash_Command);
EXPORT_SYMBOL(MDrv_CMDQ_Gen_WaitTrigger_Bus_Command);
EXPORT_SYMBOL(MDrv_CMDQ_GetWritePoint);
EXPORT_SYMBOL(MDrv_CMDQ_SetPowerState);
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_HVD_H_
#ifdef __cplusplus
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _DRV_MIU_V2_H_
#ifdef __cplusplus
#endif
#ifdef BIFROST_32BIT_MODE
#if defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300)
#else
#endif
#else
#if defined(UFO_PUBLIC_HEADER_212)||defined(UFO_PUBLIC_HEADER_300)
#else
#endif
#endif
#ifdef __cplusplus
#endif
#endif
#ifdef _APIXC_AUTO_EX_H_
#ifdef __cplusplus
#endif
EXPORT_SYMBOL(MApi_XC_Auto_EX_Geometry);
EXPORT_SYMBOL(MApi_XC_Auto_EX_Geometry_Ex);
EXPORT_SYMBOL(MApi_XC_Auto_EX_StopAutoGeometry);
EXPORT_SYMBOL(MApi_XC_Auto_EX_GainOffset);
EXPORT_SYMBOL(MApi_XC_Auto_EX_GetHWFixedGainOffset);
EXPORT_SYMBOL(MApi_XC_Auto_EX_SetValidData);
EXPORT_SYMBOL(MApi_XC_Auto_EX_AutoOffset);
EXPORT_SYMBOL(MApi_XC_Auto_EX_DetectWidth);
EXPORT_SYMBOL(MApi_XC_Auto_EX_SetCalibrationMode);
EXPORT_SYMBOL(MApi_XC_Auto_EX_GetCalibrationMode);
#ifdef __cplusplus
#endif
#endif
EXPORT_SYMBOL(mdrv_gpio_set_high);
EXPORT_SYMBOL(mdrv_gpio_set_low);
EXPORT_SYMBOL(mdrv_gpio_set_input);
EXPORT_SYMBOL(mdrv_gpio_init);
EXPORT_SYMBOL(mdrv_gpio_get_level);
EXPORT_SYMBOL(mdrv_gpio_get_inout);
EXPORT_SYMBOL(MAsm_CPU_GetTrailOne);
EXPORT_SYMBOL(g_IPanel);
EXPORT_SYMBOL(g_IPanelEx);
EXPORT_SYMBOL(g_ACPMissingCounter);
EXPORT_SYMBOL(g_AVMUTEMissingCounter);
EXPORT_SYMBOL(g_AVIMissingCounter);
EXPORT_SYMBOL(g_HdmiPollingStatus);
