#ifndef _TVE_PRIVATE_
#define _TVE_PRIVATE_

#include "drvTVEncoder.h"
#include "utopia_relation.h"

#define MAGIC_NUM 0x12345678

typedef const VE_DrvInfo *                         (*IOCTL_VE_GetInfo)                            (void);
typedef VE_Result                                      (*IOCTL_VE_GetStatus)                        (VE_DrvStatus* pDrvStatus);
typedef void                                             (*IOCTL_VE_SetDbgLevel)                    (MS_U16 level);
typedef void                                             (*IOCTL_VE_SwitchInputSource)         (PMS_Switch_VE_Src_Info pSwitchInfo);
typedef MS_SWITCH_VE_SRC_STATUS          (*IOCTL_VE_SetInputSource)               (PMS_VE_InputSrc_Info pInputSrcInfo);
typedef void                                             (*IOCTL_VE_SwitchOuputDest)             (PMS_Switch_VE_Dest_Info pSwitchInfo);
typedef void                                             (*IOCTL_VE_SetOutputCtrl)                   (PMS_VE_Output_Ctrl pOutputCtrl);
typedef MS_BOOL                                      (*IOCTL_VE_SetOutputVideoStd)          (MS_VE_VIDEOSYS VideoSystem);
typedef void                                             (*IOCTL_VE_SetMemoryBaseAddr)       (MS_PHY phyMIUAddress, MS_U32 u32MIUSize);
typedef void                                             (*IOCTL_VE_Init)                                      (MS_PHY u32MIUAddress);
typedef MS_BOOL                                      (*IOCTL_VE_Exit)                                     (void);
typedef void                                             (*IOCTL_VE_SetWSSData)                     (MS_BOOL bEn, MS_U16 u16WSSData);
typedef MS_U16                                      (*IOCTL_VE_GetWSSData)                      (void);
typedef void                                            (*IOCTL_VE_SetMode)                             (MS_VE_Set_Mode_Type *pVESetMode);
typedef void                                             (*IOCTL_VE_SetBlackScreen)                 (MS_BOOL bEn);
typedef MS_BOOL                                     (*IOCTL_VE_IsBlackScreenEnabled)       (void);
typedef void                                            (*IOCTL_VE_SetTtxBuffer)                        (MS_PHY u32StartAddr, MS_U32 u32Size);
typedef MS_BOOL                                     (*IOCTL_VE_GetTtxReadDoneStatus)            (void);
typedef void                                            (*IOCTL_VE_EnableCcSw)                              (MS_BOOL bEnable);
typedef void                                            (*IOCTL_VE_SetCcRange)                          (MS_U16 odd_start, MS_U16 odd_end, MS_U16 even_start, MS_U16 enen_end);
typedef void                                            (*IOCTL_VE_SendCcData)                          (MS_BOOL bIsOdd, MS_U16 data);
typedef void                                        (*IOCTL_VE_set_display_window)                     (MS_VE_WINDOW_TYPE stDispWin);
typedef void                                         (*IOCTL_VE_SetOSD)                                     (MS_BOOL bEnable);
typedef void                                            (*IOCTL_VE_SetRGBIn)                   (MS_BOOL bEnable);
typedef MS_VE_VIDEOSYS                       (*IOCTL_VE_Get_Output_Video_Std)                     (void);
typedef void                                            (*IOCTL_VE_SetCaptureMode)                             (MS_BOOL bEnable);
typedef MS_BOOL                                  (*IOCTL_VE_GetDstInfo)                      (MS_VE_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo);
typedef void                                     (*IOCTL_VE_Set_TestPattern)                            (MS_BOOL bEn);
typedef VE_Result                               (*IOCTL_VE_DisableRegWrite)                         (MS_BOOL bFlag);
typedef VE_Result                               (*IOCTL_VE_AdjustPositionBase)                      (MS_S32 s32WAddrAdjustment, MS_S32 s32RAddrAdjustment);
typedef VE_Result                               (*IOCTL_VE_Set_Customer_Scaling)                 (MS_VE_CusScalingInfo *pstVECusScalingInfo);
typedef void                                        (*IOCTL_VE_InitVECapture)                          (PMS_VE_Output_CAPTURE pVECapture);
typedef MS_BOOL                                  (*IOCTL_VE_EnaVECapture)                    (PMS_VE_VECAPTURESTATE pstVECapState);
typedef VE_Result                               (*IOCTL_VE_Adjust_FrameStart)                         (MS_VE_WINDOW_TYPE *stSrcWin,MS_S16 pixel_offset);
typedef MS_U32                                  (*IOCTL_TVE_SetPowerState)                       (EN_POWER_MODE u16PowerState);
typedef MS_BOOL                                  (*IOCTL_VE_SetCusTable)                 (MS_VE_VIDEOSYS VideoSystem, MS_U8* pTbl);
typedef void                                        (*IOCTL_VE_EnableCusTable)                             (MS_BOOL bEnable);
typedef VE_Result                               (*IOCTL_VE_SetFrameColor)                     (MS_U32 u32aRGB);
typedef VE_Result                                (*IOCTL_VE_Set_OSDLayer)                  (MS_VE_OSD_LAYER_SEL eVideoOSDLayer);
typedef MS_VE_OSD_LAYER_SEL              (*IOCTL_VE_Get_OSDLayer)                    (void);
typedef VE_Result                                   (*IOCTL_VE_Set_VideoAlpha)                    (MS_U8 u8Val);
typedef VE_Result                                (*IOCTL_VE_Get_VideoAlpha)                        (MS_U8 *pu8Val);
typedef VE_Result                                (*IOCTL_VE_DCS_SetType)                   (MS_BOOL bEnable, MS_U32 eType);
typedef VE_Result                                (*IOCTL_VE_DCS_SetActivationKey)                                  (MS_U8 *pu8ActivationKeyTbl, MS_U8 u8ActivationKeyTblSize);
typedef VE_Result                                   (*IOCTL_VE_ReloadSetting)                 (void);
typedef MS_BOOL                                     (*IOCTL_VE_GetConfig)               (void* pInstance, MS_PHY *pMIUAddress);


typedef struct __attribute__((packed))
{
    MS_BOOL bInited_Drv;
    MS_U32  u32Magic;
    MS_PHY u32MIUAddress;
}TVENCODER_RESOURCE_PRIVATE;

typedef struct
{
    //MS_BOOL bInited_Drv;
    //IOCTL_TVENCODER_SETINPUTSRC fpTVESetInputSRC;
    IOCTL_VE_GetInfo                fpVE_GetInfo;
    IOCTL_VE_GetStatus              fpVE_GetStatus;
    IOCTL_VE_SetDbgLevel                fpVE_SetDbgLevel;
    IOCTL_VE_SwitchInputSource              fpVE_SwitchInputSource;
    IOCTL_VE_SetInputSource                 fpVE_SetInputSource;
    IOCTL_VE_SwitchOuputDest                fpVE_SwitchOuputDest;
    IOCTL_VE_SetOutputCtrl              fpVE_SetOutputCtrl;
    IOCTL_VE_SetOutputVideoStd              fpVE_SetOutputVideoStd;
    IOCTL_VE_SetMemoryBaseAddr              fpVE_SetMemoryBaseAddr;
    IOCTL_VE_Init               fpVE_Init;
    IOCTL_VE_Exit               fpVE_Exit;
    IOCTL_VE_SetWSSData             fpVE_SetWSSData;
    IOCTL_VE_GetWSSData             fpVE_GetWSSData;
    IOCTL_VE_SetMode                fpVE_SetMode;
    IOCTL_VE_SetBlackScreen             fpVE_SetBlackScreen;
    IOCTL_VE_IsBlackScreenEnabled               fpVE_IsBlackScreenEnabled;
    IOCTL_VE_SetTtxBuffer               fpVE_SetTtxBuffer;
    IOCTL_VE_GetTtxReadDoneStatus               fpVE_GetTtxReadDoneStatus;
    IOCTL_VE_EnableCcSw             fpVE_EnableCcSw;
    IOCTL_VE_SetCcRange             fpVE_SetCcRange;
    IOCTL_VE_SendCcData             fpVE_SendCcData;
    IOCTL_VE_set_display_window            fpVE_set_display_window;
    IOCTL_VE_SetOSD             fpVE_SetOSD;
    IOCTL_VE_SetRGBIn               fpVE_SetRGBIn;
    IOCTL_VE_Get_Output_Video_Std               fpVE_Get_Output_Video_Std;
    IOCTL_VE_SetCaptureMode             fpVE_SetCaptureMode;
    IOCTL_VE_GetDstInfo             fpVE_GetDstInfo;
    IOCTL_VE_Set_TestPattern                fpVE_Set_TestPattern;
    IOCTL_VE_DisableRegWrite                fpVE_DisableRegWrite;
    IOCTL_VE_AdjustPositionBase             fpVE_AdjustPositionBase;
    IOCTL_VE_Set_Customer_Scaling               fpVE_Set_Customer_Scaling;
    IOCTL_VE_InitVECapture              fpVE_InitVECapture;
    IOCTL_VE_EnaVECapture               fpVE_EnaVECapture;
    IOCTL_VE_Adjust_FrameStart              fpVE_Adjust_FrameStart;
    IOCTL_TVE_SetPowerState             fpTVE_SetPowerState;
    IOCTL_VE_SetCusTable                fpVE_SetCusTable;
    IOCTL_VE_EnableCusTable             fpVE_EnableCusTable;
    IOCTL_VE_SetFrameColor              fpVE_SetFrameColor;
    IOCTL_VE_Set_OSDLayer               fpVE_Set_OSDLayer;
    IOCTL_VE_Get_OSDLayer               fpVE_Get_OSDLayer;
    IOCTL_VE_Set_VideoAlpha             fpVE_Set_VideoAlpha;
    IOCTL_VE_Get_VideoAlpha             fpVE_Get_VideoAlpha;
    IOCTL_VE_DCS_SetType                fpVE_DCS_SetType;
    IOCTL_VE_DCS_SetActivationKey               fpVE_DCS_SetActivationKey;
    IOCTL_VE_ReloadSetting              fpVE_ReloadSetting;
    IOCTL_VE_GetConfig              fpVE_GetConfig;

}TVENCODER_INSTANT_PRIVATE;


////////////////////////////////////////////////////////////////////////////////
// Reduce driver code size
////////////////////////////////////////////////////////////////////////////////

#define _MODULE_VE_U1_U2_RELATION \
        URELATION(fpVE_GetInfo,(IOCTL_VE_GetInfo)_MDrv_VE_GetInfo,MDrv_VE_GetInfo) \
        URELATION(fpVE_GetStatus,(IOCTL_VE_GetStatus)_MDrv_VE_GetStatus,MDrv_VE_GetStatus) \
        URELATION(fpVE_SetDbgLevel,(IOCTL_VE_SetDbgLevel)_MDrv_VE_SetDbgLevel,MDrv_VE_SetDbgLevel) \
        URELATION(fpVE_SwitchInputSource,(IOCTL_VE_SwitchInputSource)_MDrv_VE_SwitchInputSource,MDrv_VE_SwitchInputSource) \
        URELATION(fpVE_SetInputSource,(IOCTL_VE_SetInputSource)_MDrv_VE_SetInputSource,MDrv_VE_SetInputSource) \
        URELATION(fpVE_SwitchOuputDest,(IOCTL_VE_SwitchOuputDest)_MDrv_VE_SwitchOuputDest,MDrv_VE_SwitchOuputDest) \
        URELATION(fpVE_SetOutputCtrl,(IOCTL_VE_SetOutputCtrl)_MDrv_VE_SetOutputCtrl,MDrv_VE_SetOutputCtrl) \
        URELATION(fpVE_SetOutputVideoStd,(IOCTL_VE_SetOutputVideoStd)_MDrv_VE_SetOutputVideoStd,MDrv_VE_SetOutputVideoStd) \
        URELATION(fpVE_Init,(IOCTL_VE_Init)_MDrv_VE_Init,MDrv_VE_Init) \
        URELATION(fpVE_Exit,(IOCTL_VE_Exit)_MDrv_VE_Exit,MDrv_VE_Exit) \
        URELATION(fpVE_SetWSSData,(IOCTL_VE_SetWSSData)_MDrv_VE_SetWSSData,MDrv_VE_SetWSSData) \
        URELATION(fpVE_GetWSSData,(IOCTL_VE_GetWSSData)_MDrv_VE_GetWSSData,MDrv_VE_GetWSSData) \
        URELATION(fpVE_SetMode,(IOCTL_VE_SetMode)_MDrv_VE_SetMode,MDrv_VE_SetMode) \
        URELATION(fpVE_SetBlackScreen,(IOCTL_VE_SetBlackScreen)_MDrv_VE_SetBlackScreen,MDrv_VE_SetBlackScreen) \
        URELATION(fpVE_IsBlackScreenEnabled,(IOCTL_VE_IsBlackScreenEnabled)_MDrv_VE_IsBlackScreenEnabled,MDrv_VE_IsBlackScreenEnabled) \
        URELATION(fpVE_SetTtxBuffer,(IOCTL_VE_SetTtxBuffer)_MDrv_VE_SetTtxBuffer,MDrv_VE_SetTtxBuffer) \
        URELATION(fpVE_GetTtxReadDoneStatus,(IOCTL_VE_GetTtxReadDoneStatus)_MDrv_VE_GetTtxReadDoneStatus,MDrv_VE_GetTtxReadDoneStatus) \
        URELATION(fpVE_EnableCcSw,(IOCTL_VE_EnableCcSw)_MDrv_VE_EnableCcSw,MDrv_VE_EnableCcSw) \
        URELATION(fpVE_SetCcRange,(IOCTL_VE_SetCcRange)_MDrv_VE_SetCcRange,MDrv_VE_SetCcRange) \
        URELATION(fpVE_SendCcData,(IOCTL_VE_SendCcData)_MDrv_VE_SendCcData,MDrv_VE_SendCcData) \
        URELATION(fpVE_set_display_window,(IOCTL_VE_set_display_window)_MDrv_VE_set_display_window,MDrv_VE_set_display_window) \
        URELATION(fpVE_SetOSD,(IOCTL_VE_SetOSD)_MDrv_VE_SetOSD,MDrv_VE_SetOSD) \
        URELATION(fpVE_SetRGBIn,(IOCTL_VE_SetRGBIn)_MDrv_VE_SetRGBIn,MDrv_VE_SetRGBIn) \
        URELATION(fpVE_Get_Output_Video_Std,(IOCTL_VE_Get_Output_Video_Std)_MDrv_VE_Get_Output_Video_Std,MDrv_VE_Get_Output_Video_Std) \
        URELATION(fpVE_SetCaptureMode,(IOCTL_VE_SetCaptureMode)_MDrv_VE_SetCaptureMode,MDrv_VE_SetCaptureMode) \
        URELATION(fpVE_GetDstInfo,(IOCTL_VE_GetDstInfo)_MApi_VE_GetDstInfo,MApi_VE_GetDstInfo) \
        URELATION(fpVE_Set_TestPattern,(IOCTL_VE_Set_TestPattern)_MDrv_VE_Set_TestPattern,MDrv_VE_Set_TestPattern) \
        URELATION(fpVE_DisableRegWrite,(IOCTL_VE_DisableRegWrite)_MDrv_VE_DisableRegWrite,MDrv_VE_DisableRegWrite) \
        URELATION(fpVE_AdjustPositionBase,(IOCTL_VE_AdjustPositionBase)_MDrv_VE_AdjustPositionBase,MDrv_VE_AdjustPositionBase) \
        URELATION(fpVE_Set_Customer_Scaling,(IOCTL_VE_Set_Customer_Scaling)_MDrv_VE_Set_Customer_Scaling,MDrv_VE_Set_Customer_Scaling) \
        URELATION(fpVE_InitVECapture,(IOCTL_VE_InitVECapture)_MDrv_VE_InitVECapture,MDrv_VE_InitVECapture) \
        URELATION(fpVE_EnaVECapture,(IOCTL_VE_EnaVECapture)_MDrv_VE_EnaVECapture,MDrv_VE_EnaVECapture) \
        URELATION(fpVE_Adjust_FrameStart,(IOCTL_VE_Adjust_FrameStart)_MDrv_VE_Adjust_FrameStart,MDrv_VE_Adjust_FrameStart) \
        URELATION(fpTVE_SetPowerState,(IOCTL_TVE_SetPowerState)_MDrv_TVE_SetPowerState,MDrv_TVE_SetPowerState) \
        URELATION(fpVE_SetCusTable,(IOCTL_VE_SetCusTable)_MDrv_VE_SetCusTable,MDrv_VE_SetCusTable) \
        URELATION(fpVE_EnableCusTable,(IOCTL_VE_EnableCusTable)_MDrv_VE_EnableCusTable,MDrv_VE_EnableCusTable) \
        URELATION(fpVE_SetFrameColor,(IOCTL_VE_SetFrameColor)_MDrv_VE_SetFrameColor,MDrv_VE_SetFrameColor) \
        URELATION(fpVE_Set_OSDLayer,(IOCTL_VE_Set_OSDLayer)_MDrv_VE_Set_OSDLayer,MDrv_VE_Set_OSDLayer) \
        URELATION(fpVE_Get_OSDLayer,(IOCTL_VE_Get_OSDLayer)_MDrv_VE_Get_OSDLayer,MDrv_VE_Get_OSDLayer) \
        URELATION(fpVE_Set_VideoAlpha,(IOCTL_VE_Set_VideoAlpha)_MDrv_VE_Set_VideoAlpha,MDrv_VE_Set_VideoAlpha) \
        URELATION(fpVE_Get_VideoAlpha,(IOCTL_VE_Get_VideoAlpha)_MDrv_VE_Get_VideoAlpha,MDrv_VE_Get_VideoAlpha) \
        URELATION(fpVE_GetConfig,(IOCTL_VE_GetConfig)_MDrv_VE_GetConfig,MDrv_VE_GetConfig)


typedef struct
{
    MS_U32 VE_Reg[2];
}VE_REGS_SAVE_AREA;
const VE_DrvInfo * _MDrv_VE_GetInfo(void);
VE_Result _MDrv_VE_GetStatus(VE_DrvStatus* pDrvStatus);
void _MDrv_VE_SetDbgLevel(MS_U16 level);
void _MDrv_VE_SwitchInputSource(PMS_Switch_VE_Src_Info pSwitchInfo);
MS_SWITCH_VE_SRC_STATUS _MDrv_VE_SetInputSource(PMS_VE_InputSrc_Info pInputSrcInfo);
void _MDrv_VE_SwitchOuputDest(PMS_Switch_VE_Dest_Info pSwitchInfo);
void _MDrv_VE_SetOutputCtrl(PMS_VE_Output_Ctrl pOutputCtrl);
MS_BOOL _MDrv_VE_SetOutputVideoStd(MS_VE_VIDEOSYS VideoSystem);
void _MDrv_VE_SetMemoryBaseAddr(MS_PHY phyMIUAddress, MS_U32 u32MIUSize);
void _MDrv_VE_Init(MS_PHY u32MIUAddress);
MS_BOOL _MDrv_VE_Exit(void);
void _MDrv_VE_SetWSSData(MS_BOOL bEn, MS_U16 u16WSSData);
MS_U16 _MDrv_VE_GetWSSData(void);
void _MDrv_VE_SetMode(MS_VE_Set_Mode_Type *pVESetMode);
void _MDrv_VE_SetBlackScreen(MS_BOOL bEn);
MS_BOOL _MDrv_VE_IsBlackScreenEnabled(void);
void _MDrv_VE_SetTtxBuffer(MS_PHY u32StartAddr, MS_U32 u32Size);
MS_BOOL _MDrv_VE_GetTtxReadDoneStatus(void);
void _MDrv_VE_EnableCcSw(MS_BOOL bEnable);
void _MDrv_VE_SetCcRange(MS_U16 odd_start, MS_U16 odd_end, MS_U16 even_start, MS_U16 enen_end);
void _MDrv_VE_SendCcData(MS_BOOL bIsOdd, MS_U16 data);
void _MDrv_VE_set_display_window(MS_VE_WINDOW_TYPE stDispWin);
void _MDrv_VE_SetOSD(MS_BOOL bEnable);
void _MDrv_VE_SetRGBIn(MS_BOOL bEnable);
MS_VE_VIDEOSYS _MDrv_VE_Get_Output_Video_Std(void);
void _MDrv_VE_SetCaptureMode(MS_BOOL bEnable);
MS_BOOL _MApi_VE_GetDstInfo(MS_VE_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo);
void _MDrv_VE_Set_TestPattern(MS_BOOL bEn);
VE_Result _MDrv_VE_DisableRegWrite(MS_BOOL bFlag);
VE_Result _MDrv_VE_AdjustPositionBase(MS_S32 s32WAddrAdjustment, MS_S32 s32RAddrAdjustment);
VE_Result  _MDrv_VE_Set_Customer_Scaling(MS_VE_CusScalingInfo *pstVECusScalingInfo);
void _MDrv_VE_InitVECapture(PMS_VE_Output_CAPTURE pVECapture);
MS_BOOL _MDrv_VE_EnaVECapture(PMS_VE_VECAPTURESTATE pstVECapState);
VE_Result _MDrv_VE_Adjust_FrameStart(MS_VE_WINDOW_TYPE *stSrcWin,MS_S16 pixel_offset);
MS_U32 _MDrv_TVE_SetPowerState(EN_POWER_MODE u16PowerState);

MS_BOOL _MDrv_VE_SetCusTable(MS_VE_VIDEOSYS VideoSystem, MS_U8* pTbl);
void _MDrv_VE_EnableCusTable(MS_BOOL bEnable);
VE_Result _MDrv_VE_SetFrameColor(MS_U32 u32aRGB);
VE_Result _MDrv_VE_Set_OSDLayer(MS_VE_OSD_LAYER_SEL eVideoOSDLayer);
MS_VE_OSD_LAYER_SEL _MDrv_VE_Get_OSDLayer(void);
VE_Result _MDrv_VE_Set_VideoAlpha(MS_U8 u8Val);
VE_Result _MDrv_VE_Get_VideoAlpha(MS_U8 *pu8Val);

VE_Result _MDrv_VE_DCS_SetType(MS_BOOL bEnable, MS_U32 eType);
VE_Result _MDrv_VE_DCS_SetActivationKey(MS_U8 *pu8ActivationKeyTbl, MS_U8 u8ActivationKeyTblSize);

VE_Result _MDrv_VE_ReloadSetting(void);

MS_BOOL _MDrv_VE_GetConfig(void* pInstance, MS_PHY *pMIUAddress);

MS_U32 TVENCODEROpen(void** ppInstance, const void* const pAttribute);
MS_U32 TVENCODERClose(void* pInstance);
MS_U32 TVENCODERIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);

#endif //_TVE_PRIVATE_
