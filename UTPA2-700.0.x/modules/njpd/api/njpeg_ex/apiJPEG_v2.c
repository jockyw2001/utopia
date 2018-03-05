
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#include <stdio.h>
#endif

#include "MsTypes.h"

#include "utopia_dapi.h"
#include "apiJPEG.h"
#include "MsOS.h"

#include "utopia.h"
#include "apiJPEG_priv.h"
#include "apiJPEG_v2.h"

#if (defined CONFIG_MLOG)
#include "ULog.h"
#endif

MS_U8 _u8JPEG_ApiV2DbgLevel = E_JPEG_DEBUG_NONE;


enum
{
    JPEG_POOL_ID_NJPD=0,
    JPEG_POOL_ID_NJPD_MAX
} eJpegPoolID;

#if (defined CONFIG_MLOG)
#define JPEG_DEBUG_API_MSG(format, args...)  do{if(_u8JPEG_ApiV2DbgLevel & E_JPEG_DEBUG_API) ULOGI("NJPD", format, ##args);}while(0)
#define JPEG_DEBUG_API_ERR(format, args...)  do{if(_u8JPEG_ApiV2DbgLevel & E_JPEG_DEBUG_ERR) ULOGE("NJPD", format, ##args);}while(0)
#define UTOPIA_20_DBG(format, args...)      do{if(_u8JPEG_ApiV2DbgLevel & E_JPEG_DEBUG_API) ULOGI("NJPD", format, ##args);}while(0)

#else

#ifndef ANDROID
#define UTOPIA_20_DBG(format, args...)        do{if(_u8JPEG_ApiV2DbgLevel & E_JPEG_DEBUG_API) printf(format, ##args);}while(0)
#else
#define LOG_TAG "API_JPEG"
#include <cutils/log.h>
#ifndef LOGD
#define printf(format, args...)          ALOGI(format, ##args);
#define UTOPIA_20_DBG(format, args...)  do{if(_u8JPEG_ApiV2DbgLevel & E_JPEG_DEBUG_API) ALOGE(format, ##args);}while(0)
#else
#define printf(format, args...)          LOGI(format, ##args);
#define UTOPIA_20_DBG(format, args...)  do{if(_u8JPEG_ApiV2DbgLevel & E_JPEG_DEBUG_API) LOGE(format, ##args);}while(0)
#endif
#endif
#define JPEG_DEBUG_API_ERR(format, args...)  UTOPIA_20_DBG(format, ##args)

#endif
#define JPEG_SUSPEND_TIMEOUT        1000



#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
#include "utopia_dapi.h"

MS_U32 NJPEG_EXMdbIoctl(MS_U32 u32Cmd, const void* const pArgs)
{
    MDBCMD_CMDLINE_PARAMETER *paraCmdLine;
    MDBCMD_GETINFO_PARAMETER *paraGetInfo;

    switch(u32Cmd)
    {
        case MDBCMD_CMDLINE:
            paraCmdLine = (MDBCMD_CMDLINE_PARAMETER *)pArgs;
            break;
        case MDBCMD_GETINFO:
            paraGetInfo = (MDBCMD_GETINFO_PARAMETER *)pArgs;
            _MApi_JPEG_GetMdbInfo(paraGetInfo->u64ReqHdl);
            break;
        default:
            break;
    }
    return 0;
}
#endif


// this func will be call to init by utopia20 framework
void NJPEG_EXRegisterToUtopia(FUtopiaOpen ModuleType)
{
    UTOPIA_20_DBG("%s(): start\n", __FUNCTION__);
    // 1. deal with module
    void* pModuleNJPEG_EX_V2 = NULL;
    UtopiaModuleCreate(MODULE_NJPEG_EX, 0, &pModuleNJPEG_EX_V2);
    UtopiaModuleRegister(pModuleNJPEG_EX_V2);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pModuleNJPEG_EX_V2,
                                    (FUtopiaOpen)JPEGOpen,
                                    (FUtopiaClose)JPEGClose,
                                    (FUtopiaIOctl)JPEGIoctl);

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
    UtopiaModuleRegisterMdbNode("njpd", (FUtopiaMdbIoctl)NJPEG_EXMdbIoctl);
#endif

#if defined(MSOS_TYPE_LINUX_KERNEL)
    UtopiaModuleSetupSTRFunctionPtr(pModuleNJPEG_EX_V2,(FUtopiaSTR)JPEGStr);
    UtopiaModuleSetSTRPrivate(pModuleNJPEG_EX_V2, sizeof(JPEG_INSTANCE_PRIVATE));
#endif
    // 2. deal with resource
    void* psResource = NULL;
    UtopiaModuleAddResourceStart(pModuleNJPEG_EX_V2, JPEG_POOL_ID_NJPD);
    UtopiaResourceCreate("NJPD", sizeof(JPEG_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pModuleNJPEG_EX_V2, psResource, JPEG_POOL_ID_NJPD);
    UtopiaModuleAddResourceEnd(pModuleNJPEG_EX_V2, JPEG_POOL_ID_NJPD);
    UTOPIA_20_DBG("%s(): end\n", __FUNCTION__);

}

MS_U32 JPEGOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    void* pResource = NULL;
    JPEG_RESOURCE_PRIVATE* pJPEG_ResourcePrivate = NULL;
    JPEG_INSTANCE_PRIVATE* psJPEG_InstPri = NULL;
    MS_U8 i = 0;

    UTOPIA_20_DBG("%s(): start\n", __FUNCTION__);
    void* pModuleNJPEG_EX_V2 = NULL;
    UtopiaModuleGetPtr(MODULE_NJPEG_EX,&pModuleNJPEG_EX_V2);

    // Check there is available decoder to be use
    if(UtopiaResourceObtain(pModuleNJPEG_EX_V2, JPEG_POOL_ID_NJPD, &pResource) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("UtopiaResourceObtain fail,%s,%d\n",__FUNCTION__,__LINE__);
        return UTOPIA_STATUS_ERR_RESOURCE;
    }

    UtopiaResourceGetPrivate(pResource,(void*)(&pJPEG_ResourcePrivate));

#if 0
    for(i=0;i<SUPPORT_NJPD_NUM_MAX;i++)
    {
        if(pJPEG_ResourcePrivate->u8DecoderInUse[i] == 0)
        {
            pJPEG_ResourcePrivate->u8DecoderInUse[i] = 1;
            break;
        }
    }

    if(i == SUPPORT_NJPD_NUM_MAX)
    {
        JPEG_DEBUG_API_ERR("No decoder can be used,%s,%d\n",__FUNCTION__,__LINE__);
        UtopiaResourceRelease(pResource);
        return UTOPIA_STATUS_ERR_RESOURCE;
    }
#endif

    UtopiaResourceRelease(pResource);

    //Create instance
    UtopiaInstanceCreate(sizeof(JPEG_INSTANCE_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psJPEG_InstPri);

    //Recode decoder index
    psJPEG_InstPri->Index_of_decoder = i;

    psJPEG_InstPri->fpJPEGEnableOJPD = (IOCTL_JPEG_ENABLE_OJPD)_MApi_JPEG_EnableOJPD;
    psJPEG_InstPri->fpJPEGInit_UsingOJPD = (IOCTL_JPEG_INIT_USING_OJPD)_MApi_JPEG_Init_UsingOJPD;
    psJPEG_InstPri->fpJPEGget_APP0_info = (IOCTL_JPEG_GET_APP0_INFO)_msAPI_JPEG_get_APP0_info;
    psJPEG_InstPri->fpJPEGInit = (IOCTL_JPEG_INIT)_MApi_JPEG_Init;
    psJPEG_InstPri->fpJPEGDecodeHdr = (IOCTL_JPEG_DECODE_HDR)_MApi_JPEG_DecodeHdr;
    psJPEG_InstPri->fpJPEGDecode = (IOCTL_JPEG_DECODE)_MApi_JPEG_Decode;
    psJPEG_InstPri->fpJPEGExit = (IOCTL_JPEG_EXIT)_MApi_JPEG_Exit;
    psJPEG_InstPri->fpJPEGGetErrorCode = (IOCTL_JPEG_GET_ERROR_CODE)_MApi_JPEG_GetErrorCode;
    psJPEG_InstPri->fpJPEGGetJPDEventFlag = (IOCTL_JPEG_GET_JPEG_EVENT_FLAG)_MApi_JPEG_GetJPDEventFlag;
    psJPEG_InstPri->fpJPEGSetJPDEventFlag = (IOCTL_JPEG_SET_JPEG_EVENT_FLAG)_MApi_JPEG_SetJPDEventFlag;
    psJPEG_InstPri->fpJPEGRst = (IOCTL_JPEG_RST)_MApi_JPEG_Rst;
    psJPEG_InstPri->fpJPEGPowerOn = (IOCTL_JPEG_POWER_ON)_MApi_JPEG_PowerOn;
    psJPEG_InstPri->fpJPEGPowerOff = (IOCTL_JPEG_POWER_OFF)_MApi_JPEG_PowerOff;
    psJPEG_InstPri->fpJPEGGetCurVidx = (IOCTL_JPEG_GET_CUR_VIDX)_MApi_JPEG_GetCurVidx;
    psJPEG_InstPri->fpJPEGIsProgressive = (IOCTL_JPEG_IS_PROGRESSIVE)_MApi_JPEG_IsProgressive;
    psJPEG_InstPri->fpJPEGThumbnailFound = (IOCTL_JPEG_THUMBNAIL_FOUND)_MApi_JPEG_ThumbnailFound;
    psJPEG_InstPri->fpJPEGGetWidth = (IOCTL_JPEG_GET_WIDTH)_MApi_JPEG_GetWidth;
    psJPEG_InstPri->fpJPEGGetHeight = (IOCTL_JPEG_GET_HEIGHT)_MApi_JPEG_GetHeight;
    psJPEG_InstPri->fpJPEGGetOriginalWidth = (IOCTL_JPEG_GET_ORIGINAL_WIDTH)_MApi_JPEG_GetOriginalWidth;
    psJPEG_InstPri->fpJPEGGetOriginalHeight = (IOCTL_JPEG_GET_ORIGINAL_HEIGHT)_MApi_JPEG_GetOriginalHeight;
    psJPEG_InstPri->fpJPEGGetNonAlignmentWidth = (IOCTL_JPEG_GET_NONALIGNMENT_WIDTH)_MApi_JPEG_GetNonAlignmentWidth;
    psJPEG_InstPri->fpJPEGGetNonAlignmentHeight = (IOCTL_JPEG_GET_NONALIGNMENT_HEIGHT)_MApi_JPEG_GetNonAlignmentHeight;
    psJPEG_InstPri->fpJPEGGetAlignedPitch = (IOCTL_JPEG_GET_ALIGNED_PITCH)_MApi_JPEG_GetAlignedPitch;
    psJPEG_InstPri->fpJPEGGetAlignedPitch_H = (IOCTL_JPEG_GET_ALIGNED_PITCH_H)_MApi_JPEG_GetAlignedPitch_H;
    psJPEG_InstPri->fpJPEGGetAlignedWidth = (IOCTL_JPEG_GET_ALIGNED_WIDTH)_MApi_JPEG_GetAlignedWidth;
    psJPEG_InstPri->fpJPEGGetAlignedHeight = (IOCTL_JPEG_GET_ALIGNED_HEIGHT)_MApi_JPEG_GetAlignedHeight;
    psJPEG_InstPri->fpJPEGGetScaleDownFactor = (IOCTL_JPEG_GET_SCALE_DOWN_FACTOR)_MApi_JPEG_GetScaleDownFactor;
    psJPEG_InstPri->fpJPEGSetMaxDecodeResolution = (IOCTL_JPEG_SET_MAX_DECODE_RESOLUTION)_MApi_JPEG_SetMaxDecodeResolution;
    psJPEG_InstPri->fpJPEGSetProMaxDecodeResolution = (IOCTL_JPEG_SET_PRO_MAX_DECODE_RESOLUTION)_MApi_JPEG_SetProMaxDecodeResolution;
    psJPEG_InstPri->fpJPEGSetMRBufferValid = (IOCTL_JPEG_SET_MRBUFFER_VALID)_MApi_JPEG_SetMRBufferValid;
    psJPEG_InstPri->fpJPEGUpdateReadInfo = (IOCTL_JPEG_UPDATE_READ_INFO)_MApi_JPEG_UpdateReadInfo;
    psJPEG_InstPri->fpJPEGProcessEOF = (IOCTL_JPEG_PROCESS_EOF)_MApi_JPEG_ProcessEOF;
    psJPEG_InstPri->fpJPEGSetErrCode = (IOCTL_JPEG_SET_ERROR_CODE)_MApi_JPEG_SetErrCode;
    psJPEG_InstPri->fpJPEGSetDbgLevel = (IOCTL_JPEG_SET_DBG_LEVEL)_MApi_JPEG_SetDbgLevel;
    psJPEG_InstPri->fpJPEGGetDbgLevel = (IOCTL_JPEG_GET_DBG_LEVEL)_MApi_JPEG_GetDbgLevel;
    psJPEG_InstPri->fpJPEGGetInfo = (IOCTL_JPEG_GET_INFO)_MApi_JPEG_GetInfo;
    psJPEG_InstPri->fpJPEGGetStatus = (IOCTL_JPEG_GET_STATUS)_MApi_JPEG_GetStatus;
    psJPEG_InstPri->fpJPEGGetLibVer = (IOCTL_JPEG_GET_LIB_VER)_MApi_JPEG_GetLibVer;
    psJPEG_InstPri->fpJPEGHdlVidxChk = (IOCTL_JPEG_HDL_VIDX_CHK)_MApi_JPEG_HdlVidxChk;
    psJPEG_InstPri->fpJPEGGetBuffLoadType = (IOCTL_JPEG_GET_BUFF_LOAD_TYPE)_MApi_JPEG_GetBuffLoadType;
    psJPEG_InstPri->fpJPEGEnableISR = (IOCTL_JPEG_ENABLE_ISR)_MApi_JPEG_EnableISR;
    psJPEG_InstPri->fpJPEGDisableISR = (IOCTL_JPEG_DISABLE_ISR)_MApi_JPEG_DisableISR;
    psJPEG_InstPri->fpJPEGWaitDone = (IOCTL_JPEG_WAIT_DONE)_MApi_JPEG_WaitDone;
    psJPEG_InstPri->fpJPEGGetEXIFDateTime = (IOCTL_JPEG_GET_EXIF_DATETIME)_MApi_JPEG_GetEXIFDateTime;
    psJPEG_InstPri->fpJPEGGetEXIFOrientation = (IOCTL_JPEG_GET_EXIF_ORIENTATION)_MApi_JPEG_GetEXIFOrientation;
    psJPEG_InstPri->fpJPEGGetEXIFManufacturer = (IOCTL_JPEG_GET_EXIF_MANUFACTURER)_MApi_JPEG_GetEXIFManufacturer;
    psJPEG_InstPri->fpJPEGGetEXIFModel = (IOCTL_JPEG_GET_EXIF_MODEL)_MApi_JPEG_GetEXIFModel;
    psJPEG_InstPri->fpJPEGGetEXIFFlash = (IOCTL_JPEG_GET_EXIF_FLASH)_MApi_JPEG_GetEXIFFlash;
    psJPEG_InstPri->fpJPEGGetEXIFISOSpeedRatings = (IOCTL_JPEG_GET_EXIF_ISO_SPEED_RATINGS)_MApi_JPEG_GetEXIFISOSpeedRatings;
    psJPEG_InstPri->fpJPEGGetEXIFShutterSpeedValue = (IOCTL_JPEG_GET_EXIF_SHUTTER_SPEED_VALUE)_MApi_JPEG_GetEXIFShutterSpeedValue;
    psJPEG_InstPri->fpJPEGGetEXIFApertureValue = (IOCTL_JPEG_GET_EXIF_APERTURE_VALUE)_MApi_JPEG_GetEXIFApertureValue;
    psJPEG_InstPri->fpJPEGGetEXIFExposureBiasValue = (IOCTL_JPEG_GET_EXIF_EXPOSURE_BIAS_VALUE)_MApi_JPEG_GetEXIFExposureBiasValue;
    psJPEG_InstPri->fpJPEGGetEXIFFocalLength = (IOCTL_JPEG_GET_EXIF_FOCAL_LENGTH)_MApi_JPEG_GetEXIFFocalLength;
    psJPEG_InstPri->fpJPEGGetEXIFImageWidth = (IOCTL_JPEG_GET_EXIF_IMAGE_WIDTH)_MApi_JPEG_GetEXIFImageWidth;
    psJPEG_InstPri->fpJPEGGetEXIFImageHeight = (IOCTL_JPEG_GET_EXIF_IMAGE_HEIGHT)_MApi_JPEG_GetEXIFImageHeight;
    psJPEG_InstPri->fpJPEGGetEXIFExposureTime = (IOCTL_JPEG_GET_EXIF_EXPOSURE_TIME)_MApi_JPEG_GetEXIFExposureTime;
    psJPEG_InstPri->fpJPEGGetEXIFFNumber = (IOCTL_JPEG_GET_EXIF_FNUMBER)_MApi_JPEG_GetEXIFFNumber;
    psJPEG_InstPri->fpJPEGGetFreeMemory = (IOCTL_JPEG_GET_FREE_MEMORY)_MApi_JPEG_GetFreeMemory;
    psJPEG_InstPri->fpJPEGGetDataOffset = (IOCTL_JPEG_GET_DATA_OFFSET)_MApi_JPEG_GetDataOffset;
    psJPEG_InstPri->fpJPEGGetSOFOffset = (IOCTL_JPEG_GET_SOF_OFFSET)_MApi_JPEG_GetSOFOffset;
    psJPEG_InstPri->fpJPEGSetNJPDInstance = (IOCTL_JPEG_SET_NJPD_INSTANCE)_MApi_JPEG_SetNJPDInstance;
    psJPEG_InstPri->fpJPEGSupportCMYK = (IOCTL_JPEG_SUPPORT_CMYK)_MApi_JPEG_SupportCMYK;
    psJPEG_InstPri->fpJPEGSupportRGB = (IOCTL_JPEG_SUPPORT_RGB)_MApi_JPEG_SupportRGB;
    psJPEG_InstPri->fpJPEGSetMHEG5 = (IOCTL_JPEG_SET_MHEG5)_MApi_JPEG_SetMHEG5;
    psJPEG_InstPri->fpJPEGIsMPOFormat = (IOCTL_JPEG_IS_MPO_FORMAT)_MApi_JPEG_IsMPOFormat;
    psJPEG_InstPri->fpJPEGGetMPOIndex = (IOCTL_JPEG_GET_MPO_INDEX)_MApi_JPEG_GetMPOIndex;
    psJPEG_InstPri->fpJPEGGetMPOAttr = (IOCTL_JPEG_GET_MPO_ATTR)_MApi_JPEG_GetMPOAttr;
    psJPEG_InstPri->fpJPEGDumpMPO = (IOCTL_JPEG_DUMP_MPO)_MApi_JPEG_DumpMPO;
    psJPEG_InstPri->fpJPEGSetMPOBuffer = (IOCTL_JPEG_SET_MPO_BUFFER)_MApi_JPEG_SetMPOBuffer;
    psJPEG_InstPri->fpJPEGSetMPOMaxDecodeResolution = (IOCTL_JPEG_SET_MPO_MAX_DECODE_RESOLUTION)_MApi_JPEG_SetMPOMaxDecodeResolution;
    psJPEG_InstPri->fpJPEGSetMPOProMaxDecodeResolution = (IOCTL_JPEG_SET_MPO_PRO_MAX_DECODE_RESOLUTION)_MApi_JPEG_SetMPOProMaxDecodeResolution;
    psJPEG_InstPri->fpJPEGSetVerificationMode = (IOCTL_JPEG_SET_VERIFICATION_MODE)_MApi_JPEG_SetVerificationMode;
    psJPEG_InstPri->fpJPEGGetVerificationMode = (IOCTL_JPEG_GET_VERIFICATION_MODE)_MApi_JPEG_GetVerificationMode;
    psJPEG_InstPri->fpJPEGDebug = (IOCTL_JPEG_DEBUG)_MApi_NJPD_Debug;
    psJPEG_InstPri->fpJPEGGetControl = (IOCTL_JPEG_GET_CONTROL)_MApi_JPEG_GetControl;
    psJPEG_InstPri->fpJPEGGetEvent = (IOCTL_JPEG_CTRL_CB_EVENT)_MApi_JPEG_GetEvent;
    psJPEG_InstPri->fpJPEGSetEvent = (IOCTL_JPEG_CTRL_CB_EVENT)_MApi_JPEG_SetEvent;
    UTOPIA_20_DBG("%s(): end\n", __FUNCTION__);

#if defined(MSOS_TYPE_LINUX_KERNEL)
    JPEG_INSTANCE_PRIVATE* psJPEG_STR_InstPri = NULL;
    UtopiaModuleGetSTRPrivate(pModuleNJPEG_EX_V2, (void**)&psJPEG_STR_InstPri);
    psJPEG_STR_InstPri->bJPEGInUse = TRUE;
    psJPEG_STR_InstPri->bJPEGSuspend = FALSE;
#endif

    return UTOPIA_STATUS_SUCCESS;
}


MS_U32 JPEGIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    JPEG_INSTANCE_PRIVATE* psJPEG_InstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&psJPEG_InstPri);
    E_JPEG_V2_IOCTL_CMD IOcmd = (E_JPEG_V2_IOCTL_CMD)u32Cmd;
    MS_U32 ret = UTOPIA_STATUS_SUCCESS;

    PJPEG_BOOL_PARAM pBoolPara = NULL;
    PJPEG_INIT_PARAM pInit = NULL;
    PJPEG_GETAPP0INFO_PARAM pGetApp0Info = NULL;
    PJPEG_RESULT_PARAM pResult = NULL;
    PJPEG_GETERRORCODE_PARAM pGetErrorCode = NULL;
    PJPEG_JPEGEVENT_PARAM pJpegEvent = NULL;
    PJPEG_U16_PARAM pU16Para = NULL;
    PJPEG_U8_PARAM pU8Para = NULL;
    PJPEG_RESOLUTION_PARAM pResolution = NULL;
    PJPEG_SETMBUFFERVALID_PARAM pSetMBufferValid = NULL;
    PJPEG_UPDATEREADINFO_PARAM pUpdateReadInfo = NULL;
    PJPEG_PROCESSEOF_PARAM pProcessEOF = NULL;
    PJPEG_SETERRORCODE_PARAM pSetErrorCode = NULL;
    PJPEG_GETINFO_PARAM pGetInfo = NULL;
    PJPEG_GETSTATUS_PARAM pGetStatus = NULL;
    PJPEG_GETLIBVER_PARAM pGetLibVer = NULL;
    PJPEG_GETBUFFLOADTYPE_PARAM pGetBuffLoadType = NULL;
    PJPEG_ENABLEISR_PARAM pEnableISR = NULL;
    PJPEG_WAITDONE_PARAM pWaitDone = NULL;
    PJPEG_GETEXIFDATETIME_PARAM pGetExifDateTime = NULL;
    PJPEG_GETEXIFORIENTATION_PARAM pGetExifOrientation = NULL;
    PJPEG_GETEXIFSTRING_PARAM pGetExifString = NULL;
    PJPEG_GETEXIFU16_PARAM pGetExifU16 = NULL;
    PJPEG_GETEXIFU32_PARAM pGetExifU32 = NULL;
    PJPEG_GETEXIFRATIONAL_PARAM pGetExifRational = NULL;
    PJPEG_GETFREEMEMORY_PARAM pGetFreeMemory = NULL;
    PJPEG_U32_PARAM pU32Para = NULL;
    PJPEG_SUPPORTCMYK_PARAM pSupportCMYK = NULL;
    PJPEG_GETMPOINDEX_PARAM pGetMPOIndex = NULL;
    PJPEG_GETMPOATTR_PARAM pGetMPOAttr = NULL;
    PJPEG_SETMPOBUFFER_PARAM pSetMPOBuffer = NULL;
    PJPEG_VERIFICATION_PARAM pVerification = NULL;
    PJPEG_GET_CTRL_PARAM pGetCtrl = NULL;
    PJPEG_CB_EVENT_PARAM pGetCbParam = NULL;

    switch(IOcmd)
    {
        case MApi_CMD_JPEG_EnableOJPD:
            pBoolPara = (PJPEG_BOOL_PARAM)pArgs;
            psJPEG_InstPri->fpJPEGEnableOJPD(pBoolPara->bValue);
            break;
        case MApi_CMD_JPEG_Init_UsingOJPD:
            pInit = (PJPEG_INIT_PARAM)pArgs;
            pInit->retVal = psJPEG_InstPri->fpJPEGInit_UsingOJPD(pInit->pInitParam);
            break;
        case MApi_CMD_JPEG_get_APP0_info:
            pGetApp0Info = (PJPEG_GETAPP0INFO_PARAM)pArgs;
            psJPEG_InstPri->fpJPEGget_APP0_info(pGetApp0Info->unit, pGetApp0Info->x, pGetApp0Info->y);
            break;
        case MApi_CMD_JPEG_Init:
            pInit = (PJPEG_INIT_PARAM)pArgs;
            pInit->retVal = psJPEG_InstPri->fpJPEGInit(pInit->pInitParam);
            break;
        case MApi_CMD_JPEG_DecodeHdr:
            pResult = (PJPEG_RESULT_PARAM)pArgs;
            pResult->retVal = psJPEG_InstPri->fpJPEGDecodeHdr();
            break;
        case MApi_CMD_JPEG_Decode:
            pResult = (PJPEG_RESULT_PARAM)pArgs;
            pResult->retVal = psJPEG_InstPri->fpJPEGDecode();
            break;
        case MApi_CMD_JPEG_Exit:
            psJPEG_InstPri->fpJPEGExit();
            break;
        case MApi_CMD_JPEG_GetErrorCode:
            pGetErrorCode= (PJPEG_GETERRORCODE_PARAM)pArgs;
            pGetErrorCode->ErrCode = psJPEG_InstPri->fpJPEGGetErrorCode();
            break;
        case MApi_CMD_JPEG_GetJPDEventFlag:
            pJpegEvent= (PJPEG_JPEGEVENT_PARAM)pArgs;
            pJpegEvent->eEvtVal = psJPEG_InstPri->fpJPEGGetJPDEventFlag();
            break;
        case MApi_CMD_JPEG_SetJPDEventFlag:
            pJpegEvent = (PJPEG_JPEGEVENT_PARAM)pArgs;
            psJPEG_InstPri->fpJPEGSetJPDEventFlag(pJpegEvent->eEvtVal);
            break;
        case MApi_CMD_JPEG_Rst:
            psJPEG_InstPri->fpJPEGRst();
            break;
        case MApi_CMD_JPEG_PowerOn:
            psJPEG_InstPri->fpJPEGPowerOn();
            break;
        case MApi_CMD_JPEG_PowerOff:
            psJPEG_InstPri->fpJPEGPowerOff();
            break;
        case MApi_CMD_JPEG_GetCurVidx:
            pU16Para= (PJPEG_U16_PARAM)pArgs;
            pU16Para->u16Value = psJPEG_InstPri->fpJPEGGetCurVidx();
            break;
        case MApi_CMD_JPEG_IsProgressive:
            pBoolPara = (PJPEG_BOOL_PARAM)pArgs;
            pBoolPara->bValue = psJPEG_InstPri->fpJPEGIsProgressive();
            break;
        case MApi_CMD_JPEG_ThumbnailFound:
            pBoolPara = (PJPEG_BOOL_PARAM)pArgs;
            pBoolPara->bValue = psJPEG_InstPri->fpJPEGThumbnailFound();
            break;
        case MApi_CMD_JPEG_GetWidth:
            pU16Para = (PJPEG_U16_PARAM)pArgs;
            pU16Para->u16Value = psJPEG_InstPri->fpJPEGGetWidth();
            break;
        case MApi_CMD_JPEG_GetHeight:
            pU16Para = (PJPEG_U16_PARAM)pArgs;
            pU16Para->u16Value = psJPEG_InstPri->fpJPEGGetHeight();
            break;
        case MApi_CMD_JPEG_GetOriginalWidth:
            pU16Para = (PJPEG_U16_PARAM)pArgs;
            pU16Para->u16Value = psJPEG_InstPri->fpJPEGGetOriginalWidth();
            break;
        case MApi_CMD_JPEG_GetOriginalHeight:
            pU16Para = (PJPEG_U16_PARAM)pArgs;
            pU16Para->u16Value = psJPEG_InstPri->fpJPEGGetOriginalHeight();
            break;
        case MApi_CMD_JPEG_GetNonAlignmentWidth:
            pU16Para = (PJPEG_U16_PARAM)pArgs;
            pU16Para->u16Value = psJPEG_InstPri->fpJPEGGetNonAlignmentWidth();
            break;
        case MApi_CMD_JPEG_GetNonAlignmentHeight:
            pU16Para = (PJPEG_U16_PARAM)pArgs;
            pU16Para->u16Value = psJPEG_InstPri->fpJPEGGetNonAlignmentHeight();
            break;
        case MApi_CMD_JPEG_GetAlignedPitch:
            pU16Para = (PJPEG_U16_PARAM)pArgs;
            pU16Para->u16Value = psJPEG_InstPri->fpJPEGGetAlignedPitch();
            break;
        case MApi_CMD_JPEG_GetAlignedPitch_H:
            pU16Para = (PJPEG_U16_PARAM)pArgs;
            pU16Para->u16Value = psJPEG_InstPri->fpJPEGGetAlignedPitch_H();
            break;
        case MApi_CMD_JPEG_GetAlignedWidth:
            pU16Para = (PJPEG_U16_PARAM)pArgs;
            pU16Para->u16Value = psJPEG_InstPri->fpJPEGGetAlignedWidth();
            break;
        case MApi_CMD_JPEG_GetAlignedHeight:
            pU16Para = (PJPEG_U16_PARAM)pArgs;
            pU16Para->u16Value = psJPEG_InstPri->fpJPEGGetAlignedHeight();
            break;
        case MApi_CMD_JPEG_GetScaleDownFactor:
            pU8Para = (PJPEG_U8_PARAM)pArgs;
            pU8Para->u8Value = psJPEG_InstPri->fpJPEGGetScaleDownFactor();
            break;
        case MApi_CMD_JPEG_SetMaxDecodeResolution:
            pResolution = (PJPEG_RESOLUTION_PARAM)pArgs;
            psJPEG_InstPri->fpJPEGSetMaxDecodeResolution(pResolution->u16Width, pResolution->u16Height);
            break;
        case MApi_CMD_JPEG_SetProMaxDecodeResolution:
            pResolution = (PJPEG_RESOLUTION_PARAM)pArgs;
            psJPEG_InstPri->fpJPEGSetProMaxDecodeResolution(pResolution->u16Width, pResolution->u16Height);
            break;
        case MApi_CMD_JPEG_SetMRBufferValid:
            pSetMBufferValid = (PJPEG_SETMBUFFERVALID_PARAM)pArgs;
            psJPEG_InstPri->fpJPEGSetMRBufferValid(pSetMBufferValid->u8MRBuffType);
            break;
        case MApi_CMD_JPEG_UpdateReadInfo:
            pUpdateReadInfo = (PJPEG_UPDATEREADINFO_PARAM)pArgs;
            psJPEG_InstPri->fpJPEGUpdateReadInfo(pUpdateReadInfo->u32DataRead, pUpdateReadInfo->bEOFflag);
            break;
        case MApi_CMD_JPEG_ProcessEOF:
            pProcessEOF = (PJPEG_PROCESSEOF_PARAM)pArgs;
            psJPEG_InstPri->fpJPEGProcessEOF(pProcessEOF->u8MRBuffType);
            break;
        case MApi_CMD_JPEG_SetErrCode:
            pSetErrorCode = (PJPEG_SETERRORCODE_PARAM)pArgs;
            psJPEG_InstPri->fpJPEGSetErrCode(pSetErrorCode->ErrStatus);
            break;
        case MApi_CMD_JPEG_SetDbgLevel:
            pU8Para = (PJPEG_U8_PARAM)pArgs;
            psJPEG_InstPri->fpJPEGSetDbgLevel(pU8Para->u8Value);
            break;
        case MApi_CMD_JPEG_GetDbgLevel:
            pU8Para = (PJPEG_U8_PARAM)pArgs;
            pU8Para->u8Value = psJPEG_InstPri->fpJPEGGetDbgLevel();
            break;
        case MApi_CMD_JPEG_GetInfo:
            pGetInfo = (PJPEG_GETINFO_PARAM)pArgs;
            psJPEG_InstPri->fpJPEGGetInfo(pGetInfo->pJPEG_Info);
            break;
        case MApi_CMD_JPEG_GetStatus:
            pGetStatus = (PJPEG_GETSTATUS_PARAM)pArgs;
            psJPEG_InstPri->fpJPEGGetStatus(pGetStatus->pJPEG_Status);
            break;
        case MApi_CMD_JPEG_GetLibVer:
            pGetLibVer = (PJPEG_GETLIBVER_PARAM)pArgs;
            pGetLibVer->retVal = psJPEG_InstPri->fpJPEGGetLibVer(pGetLibVer->ppVersion);
            break;
        case MApi_CMD_JPEG_HdlVidxChk:
            pResult = (PJPEG_RESULT_PARAM)pArgs;
            pResult->retVal = psJPEG_InstPri->fpJPEGHdlVidxChk();
            break;
        case MApi_CMD_JPEG_GetBuffLoadType:
            pGetBuffLoadType = (PJPEG_GETBUFFLOADTYPE_PARAM)pArgs;
            pGetBuffLoadType->retVal = psJPEG_InstPri->fpJPEGGetBuffLoadType(pGetBuffLoadType->pBuffLoadType);
            break;
        case MApi_CMD_JPEG_EnableISR:
            pEnableISR = (PJPEG_ENABLEISR_PARAM)pArgs;
            pEnableISR->retVal = psJPEG_InstPri->fpJPEGEnableISR(pEnableISR->IsrCb);
            break;
        case MApi_CMD_JPEG_DisableISR:
            pResult = (PJPEG_RESULT_PARAM)pArgs;
            pResult->retVal = psJPEG_InstPri->fpJPEGDisableISR();
            break;
        case MApi_CMD_JPEG_WaitDone:
            pWaitDone = (PJPEG_WAITDONE_PARAM)pArgs;
            pWaitDone->retVal = psJPEG_InstPri->fpJPEGWaitDone();
            break;
        case MApi_CMD_JPEG_GetEXIFDateTime:
            pGetExifDateTime = (PJPEG_GETEXIFDATETIME_PARAM)pArgs;
            pGetExifDateTime->retVal = psJPEG_InstPri->fpJPEGGetEXIFDateTime(pGetExifDateTime->DateTime);
            break;
        case MApi_CMD_JPEG_GetEXIFOrientation:
            pGetExifOrientation = (PJPEG_GETEXIFORIENTATION_PARAM)pArgs;
            pGetExifOrientation->retVal = psJPEG_InstPri->fpJPEGGetEXIFOrientation(pGetExifOrientation->eOrientation);
            break;
        case MApi_CMD_JPEG_GetEXIFManufacturer:
            pGetExifString = (PJPEG_GETEXIFSTRING_PARAM)pArgs;
            pGetExifString->retVal = psJPEG_InstPri->fpJPEGGetEXIFManufacturer(pGetExifString->pu8String, pGetExifString->u8size);
            break;
        case MApi_CMD_JPEG_GetEXIFModel:
            pGetExifString = (PJPEG_GETEXIFSTRING_PARAM)pArgs;
            pGetExifString->retVal = psJPEG_InstPri->fpJPEGGetEXIFModel(pGetExifString->pu8String, pGetExifString->u8size);
            break;
        case MApi_CMD_JPEG_GetEXIFFlash:
            pGetExifU16 = (PJPEG_GETEXIFU16_PARAM)pArgs;
            pGetExifU16->retVal = psJPEG_InstPri->fpJPEGGetEXIFFlash(pGetExifU16->pu16Value);
            break;
        case MApi_CMD_JPEG_GetEXIFISOSpeedRatings:
            pGetExifU32 = (PJPEG_GETEXIFU32_PARAM)pArgs;
            pGetExifU32->retVal = psJPEG_InstPri->fpJPEGGetEXIFISOSpeedRatings(pGetExifU32->pu32Value);
            break;
        case MApi_CMD_JPEG_GetEXIFShutterSpeedValue:
            pGetExifRational = (PJPEG_GETEXIFRATIONAL_PARAM)pArgs;
            pGetExifRational->retVal = psJPEG_InstPri->fpJPEGGetEXIFShutterSpeedValue(pGetExifRational->pRational);
            break;
        case MApi_CMD_JPEG_GetEXIFApertureValue:
            pGetExifRational = (PJPEG_GETEXIFRATIONAL_PARAM)pArgs;
            pGetExifRational->retVal = psJPEG_InstPri->fpJPEGGetEXIFApertureValue(pGetExifRational->pRational);
            break;
        case MApi_CMD_JPEG_GetEXIFExposureBiasValue:
            pGetExifRational = (PJPEG_GETEXIFRATIONAL_PARAM)pArgs;
            pGetExifRational->retVal = psJPEG_InstPri->fpJPEGGetEXIFExposureBiasValue(pGetExifRational->pRational);
            break;
        case MApi_CMD_JPEG_GetEXIFFocalLength:
            pGetExifRational = (PJPEG_GETEXIFRATIONAL_PARAM)pArgs;
            pGetExifRational->retVal = psJPEG_InstPri->fpJPEGGetEXIFFocalLength(pGetExifRational->pRational);
            break;
        case MApi_CMD_JPEG_GetEXIFImageWidth:
            pGetExifU32 = (PJPEG_GETEXIFU32_PARAM)pArgs;
            pGetExifU32->retVal = psJPEG_InstPri->fpJPEGGetEXIFImageWidth(pGetExifU32->pu32Value);
            break;
        case MApi_CMD_JPEG_GetEXIFImageHeight:
            pGetExifU32 = (PJPEG_GETEXIFU32_PARAM)pArgs;
            pGetExifU32->retVal = psJPEG_InstPri->fpJPEGGetEXIFImageHeight(pGetExifU32->pu32Value);
            break;
        case MApi_CMD_JPEG_GetEXIFExposureTime:
            pGetExifRational = (PJPEG_GETEXIFRATIONAL_PARAM)pArgs;
            pGetExifRational->retVal = psJPEG_InstPri->fpJPEGGetEXIFExposureTime(pGetExifRational->pRational);
            break;
        case MApi_CMD_JPEG_GetEXIFFNumber:
            pGetExifRational = (PJPEG_GETEXIFRATIONAL_PARAM)pArgs;
            pGetExifRational->retVal = psJPEG_InstPri->fpJPEGGetEXIFFNumber(pGetExifRational->pRational);
            break;
        case MApi_CMD_JPEG_GetFreeMemory:
            pGetFreeMemory = (PJPEG_GETFREEMEMORY_PARAM)pArgs;
            pGetFreeMemory->addr = psJPEG_InstPri->fpJPEGGetFreeMemory(pGetFreeMemory->size);
            break;
        case MApi_CMD_JPEG_GetDataOffset:
            pU32Para = (PJPEG_U32_PARAM)pArgs;
            pU32Para->u32Value = psJPEG_InstPri->fpJPEGGetDataOffset();
            break;
        case MApi_CMD_JPEG_GetSOFOffset:
            pU32Para = (PJPEG_U32_PARAM)pArgs;
            pU32Para->u32Value = psJPEG_InstPri->fpJPEGGetSOFOffset();
            break;
        case MApi_CMD_JPEG_SetNJPDInstance:
            pU8Para = (PJPEG_U8_PARAM)pArgs;
            psJPEG_InstPri->fpJPEGSetNJPDInstance(pU8Para->u8Value);
            break;
        case MApi_CMD_JPEG_SupportCMYK:
            pSupportCMYK = (PJPEG_SUPPORTCMYK_PARAM)pArgs;
            pSupportCMYK->retVal = psJPEG_InstPri->fpJPEGSupportCMYK(pSupportCMYK->bEnable);
            break;
        case MApi_CMD_JPEG_SupportRGB:
            pSupportCMYK = (PJPEG_SUPPORTCMYK_PARAM)pArgs;
            pSupportCMYK->retVal = psJPEG_InstPri->fpJPEGSupportRGB(pSupportCMYK->bEnable);
            break;
        case MApi_CMD_JPEG_SetMHEG5:
            pBoolPara = (PJPEG_BOOL_PARAM)pArgs;
            psJPEG_InstPri->fpJPEGSetMHEG5(pBoolPara->bValue);
            break;
        case MApi_CMD_JPEG_IsMPOFormat:
            pBoolPara = (PJPEG_BOOL_PARAM)pArgs;
            pBoolPara->bValue = psJPEG_InstPri->fpJPEGIsMPOFormat();
            break;
        case MApi_CMD_JPEG_GetMPOIndex:
            pGetMPOIndex = (PJPEG_GETMPOINDEX_PARAM)pArgs;
            //pGetMPOIndex->bretVal = psJPEG_InstPri->fpJPEGGetMPOIndex(pGetMPOIndex->ppMPOIndex);
            pGetMPOIndex->bretVal = psJPEG_InstPri->fpJPEGGetMPOIndex(pGetMPOIndex->pMPOIndex);
            break;
        case MApi_CMD_JPEG_GetMPOAttr:
            pGetMPOAttr = (PJPEG_GETMPOATTR_PARAM)pArgs;
             // pGetMPOAttr->bretVal = psJPEG_InstPri->fpJPEGGetMPOAttr(pGetMPOAttr->image_no, pGetMPOAttr->ppMPOAttr);
            pGetMPOAttr->bretVal = psJPEG_InstPri->fpJPEGGetMPOAttr(pGetMPOAttr->image_no, pGetMPOAttr->pMPOAttr);
            break;
        case MApi_CMD_JPEG_DumpMPO:
            psJPEG_InstPri->fpJPEGDumpMPO();
            break;
        case MApi_CMD_JPEG_SetMPOBuffer:
            pSetMPOBuffer = (PJPEG_SETMPOBUFFER_PARAM)pArgs;
            pSetMPOBuffer->bretVal = psJPEG_InstPri->fpJPEGSetMPOBuffer(pSetMPOBuffer->read_offset, pSetMPOBuffer->output_start);
            break;
        case MApi_CMD_JPEG_SetMPOMaxDecodeResolution:
            pResolution = (PJPEG_RESOLUTION_PARAM)pArgs;
            psJPEG_InstPri->fpJPEGSetMPOMaxDecodeResolution(pResolution->u16Width, pResolution->u16Height);
            break;
        case MApi_CMD_JPEG_SetMPOProMaxDecodeResolution:
            pResolution = (PJPEG_RESOLUTION_PARAM)pArgs;
            psJPEG_InstPri->fpJPEGSetMPOProMaxDecodeResolution(pResolution->u16Width, pResolution->u16Height);
            break;
        case MApi_CMD_JPEG_SetVerificationMode:
            pVerification = (PJPEG_VERIFICATION_PARAM)pArgs;
            psJPEG_InstPri->fpJPEGSetVerificationMode(pVerification->VerificationMode);
            break;
        case MApi_CMD_JPEG_GetVerificationMode:
            pVerification = (PJPEG_VERIFICATION_PARAM)pArgs;
            pVerification->VerificationMode = psJPEG_InstPri->fpJPEGGetVerificationMode();
            break;
        case MApi_CMD_JPEG_Debug:
            psJPEG_InstPri->fpJPEGDebug();
            break;
        case MApi_CMD_JPEG_GetControl:
            pGetCtrl = (PJPEG_GET_CTRL_PARAM)pArgs;
            psJPEG_InstPri->fpJPEGGetControl(pGetCtrl->eGetID, pGetCtrl->param, pGetCtrl->u32size);
            break;
        case MApi_CMD_JPEG_GetCbEvent:
            pGetCbParam = (PJPEG_CB_EVENT_PARAM)pArgs;
            psJPEG_InstPri->fpJPEGGetEvent(pGetCbParam);
            break;
        case MApi_CMD_JPEG_SetCbEvent:
            pGetCbParam = (PJPEG_CB_EVENT_PARAM)pArgs;
            psJPEG_InstPri->fpJPEGSetEvent(pGetCbParam);
            break;

        default:
            ret = UTOPIA_STATUS_NOT_SUPPORTED;
            JPEG_DEBUG_API_ERR("Error Command = %td\n",(ptrdiff_t)u32Cmd);
            break;

    }
    return ret;
}
MS_U32 JPEGClose(void* pInstance)
{
    void* pResource = NULL;
    JPEG_RESOURCE_PRIVATE* pJPEG_ResourcePrivate = NULL;
    JPEG_INSTANCE_PRIVATE* psJPEG_InstPri = NULL;
    UTOPIA_20_DBG("%s(): start\n", __FUNCTION__);
    void* pModuleNJPEG_EX_V2 = NULL;
    UtopiaModuleGetPtr(MODULE_NJPEG_EX,&pModuleNJPEG_EX_V2);

    // Get instance private
    UtopiaInstanceGetPrivate(pInstance, (void**)&psJPEG_InstPri);

    // Get resource private
    if(UtopiaResourceObtain(pModuleNJPEG_EX_V2, JPEG_POOL_ID_NJPD, &pResource) != UTOPIA_STATUS_SUCCESS)
    {
        JPEG_DEBUG_API_ERR("UtopiaResourceObtain fail,%s,%d\n",__FUNCTION__,__LINE__);
        return UTOPIA_STATUS_ERR_RESOURCE;
    }

    UtopiaResourceGetPrivate(pResource,(void*)(&pJPEG_ResourcePrivate));

    // Set using decoder index to 0
    pJPEG_ResourcePrivate->u8DecoderInUse[psJPEG_InstPri->Index_of_decoder] = 0;

    UtopiaResourceRelease(pResource);

    UtopiaInstanceDelete(pInstance);
    UTOPIA_20_DBG("%s(): end\n", __FUNCTION__);

#if defined(MSOS_TYPE_LINUX_KERNEL)
    JPEG_INSTANCE_PRIVATE* psJPEG_STR_InstPri = NULL;
    UtopiaModuleGetSTRPrivate(pModuleNJPEG_EX_V2, (void**)&psJPEG_STR_InstPri);
    psJPEG_STR_InstPri->bJPEGInUse = FALSE;
    psJPEG_STR_InstPri->bJPEGSuspend = FALSE;
#endif

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 JPEGStr(MS_U32 u32PowerState, void* pModule)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

#if defined(MSOS_TYPE_LINUX_KERNEL)
    JPEG_INSTANCE_PRIVATE* psJPEG_STR_InstPri = NULL;

    UtopiaModuleGetSTRPrivate(pModule, (void**)&psJPEG_STR_InstPri);

    //UtopiaModuleGetSTRPrivate(pModule, (void**));

    if (u32PowerState == E_POWER_SUSPEND)
    {
        MS_U32 u32Time=MsOS_GetSystemTime();
        while(1)
        {
            if(psJPEG_STR_InstPri->bJPEGInUse==FALSE)
            {
                psJPEG_STR_InstPri->bJPEGSuspend = TRUE;
                return UTOPIA_STATUS_SUCCESS;
            }
            if((MsOS_GetSystemTime() - u32Time) >= JPEG_SUSPEND_TIMEOUT)
            {
                JPEG_DEBUG_API_ERR("Suspend request Timeout!!!, force MApi_JPEG_Exit()\n");

                MApi_JPEG_Exit();
                psJPEG_STR_InstPri->bJPEGSuspend = TRUE;
                return UTOPIA_STATUS_SUCCESS;
            }
            else if(u32Time > MsOS_GetSystemTime())  // For the overflow case
            {
                u32Time = 0;
            }
            MsOS_DelayTask(1);
        }
        /* Please Implement Module Suspend Flow Here. */
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
    }
    else if (u32PowerState == E_POWER_RESUME)
    {
        if(psJPEG_STR_InstPri->bJPEGSuspend == FALSE)
        {
            JPEG_DEBUG_API_ERR("Resume fail!!! Not in suspend state!!!\n");
            u32Return = UTOPIA_STATUS_FAIL;
        }
        psJPEG_STR_InstPri->bJPEGSuspend = FALSE;
        /* Please Implement Module Resume Flow Here. */
        u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
    }
    else
    {
        u32Return = UTOPIA_STATUS_FAIL;
    }
#endif
    return u32Return;// for success
}


void JPEG_SetDbgLevel_V2(MS_U8 u8DbgLevel)
{
    _u8JPEG_ApiV2DbgLevel = u8DbgLevel;

}
