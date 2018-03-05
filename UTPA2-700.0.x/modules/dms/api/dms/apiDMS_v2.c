#define _DMS_C_

#include "MsTypes.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <string.h>
#endif
#include "MsCommon.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "apiDMS.h"
#include "apiDMS_v2.h"
#include "drvDMS.h"
#include "halDMS.h"
#include "apiDMS_private.h"

#include "UFO.h"
//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
// for semaphore POOL


//Below is dbg msg for some important dbg function, like:setmux, set gop dst, atexit,etc...


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

// this func will be call to init by utopia20 framework
void DMSRegisterToUtopia(void)
{
    MS_U32 u32ResourceStatusCheck[E_DMS_POOL_ID_MAX] = {UTOPIA_STATUS_FAIL};
    // 1. deal with module
    void* pUtopiaModule = NULL;
	UtopiaModuleCreate(MODULE_DMS, 0, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)DMSOpen, (FUtopiaClose)DMSClose, (FUtopiaIOctl)DMSIoctl);

    // 2. deal with resource
    void* psResource = NULL;
    // start func to add res, call once will create 2 access in resource. Also can declare BDMA_POOL_ID_BDMA1 for another channel depend on driver owner.
	UtopiaModuleAddResourceStart(pUtopiaModule, E_DMS_POOL_ID_INTERNAL_VARIABLE);
    // resource can alloc private for internal use, ex, BDMA_RESOURCE_PRIVATE
    u32ResourceStatusCheck[E_DMS_POOL_ID_INTERNAL_VARIABLE] = UtopiaResourceCreate("DMS", sizeof(DMS_RESOURCE_PRIVATE), &psResource);
    // func to reg res
    UtopiaResourceRegister(pUtopiaModule, psResource, E_DMS_POOL_ID_INTERNAL_VARIABLE);

	UtopiaModuleAddResourceEnd(pUtopiaModule, E_DMS_POOL_ID_INTERNAL_VARIABLE);

    //4. init resource private members here   (aka, global variable)
    DMS_RESOURCE_PRIVATE* pDMSResourcePrivate = NULL;
    UtopiaResourceGetPrivate(psResource,(void**)(&pDMSResourcePrivate));

    //pXCResourcePrivate->bResourceRegistered will automatically cleared by UtopiaResourceRegister
    if (u32ResourceStatusCheck[E_DMS_POOL_ID_INTERNAL_VARIABLE] == UTOPIA_STATUS_SHM_EXIST)
    {
        // do nothing, since it is already inited
    }
    else
    {
        // Init flow control related variables here. Other global variable should be
        // inited in each of its init function relatively.
        pDMSResourcePrivate->bResourceRegistered = TRUE;
        pDMSResourcePrivate->bDrvInit = FALSE;
        pDMSResourcePrivate->stHalDMS.phyDSAddr = 0x0;
        pDMSResourcePrivate->stHalDMS.u8DSIndex = 0;
        memset(&pDMSResourcePrivate->stHalDMS.stDMS_DSSWDB, 0 , sizeof(pDMSResourcePrivate->stHalDMS.stDMS_DSSWDB));

#ifdef DMS_VERSION_DIPGOP
        MS_U8 u8WindowID=0;

        memset(pDMSResourcePrivate->stDrvDMS._stXC_DIPMemInfo, 0 , sizeof(DMS_DIPMEM_INFO) * E_DMS_DIP_CLIENT_MAX);
        memset(pDMSResourcePrivate->stDrvDMS._stXC_PreDispWinInfo, 0 , sizeof(ST_DMS_WINDOW) * DMS_MAX_WINDOW_NUM);
        memset(pDMSResourcePrivate->stDrvDMS._stXC_PreDispWinInfo, 0 , sizeof(DMS_DISP_BUFFER_FLAG) * E_DMS_DIP_MULTIVIEW_BUF_ID_MAX * DMS_MAX_WINDOW_NUM);
        pDMSResourcePrivate->stDrvDMS.u32MaxOutputFrameRate = 0;
        pDMSResourcePrivate->stDrvDMS.u32VsyncCount = 0;
        pDMSResourcePrivate->stDrvDMS.u32GOPDispCount = 0;
        for(u8WindowID=0; u8WindowID<DMS_MAX_WINDOW_NUM; u8WindowID++)
        {
            pDMSResourcePrivate->stDrvDMS.u8DIPReadytoCaptureID[u8WindowID] = DMS_Invalid_ReadPointer_ID;
        }

        memset(pDMSResourcePrivate->stDrvDMS.u32DIPAddress_pa, 0 , sizeof(MS_PHY) * E_DMS_DIP_CLIENT_MAX);
        memset(pDMSResourcePrivate->stDrvDMS.u8DIP_Window, 0 , sizeof(MS_U8) * E_DMS_DIP_MAX);
        pDMSResourcePrivate->stDrvDMS.u32GEAddress_pa = 0x0;
        pDMSResourcePrivate->stDrvDMS.u8GOPGWinNum = 0xFF;
        pDMSResourcePrivate->stDrvDMS.bIsGOPInit = FALSE;
        pDMSResourcePrivate->stDrvDMS.bDIPWriteFirstAddr = TRUE;
        pDMSResourcePrivate->stDrvDMS.bDIPMVOPWriteFirstAddr = TRUE;
        pDMSResourcePrivate->stDrvDMS.u8EnableWindowCount = 0;
        pDMSResourcePrivate->stDrvDMS.u8UseDIPWindowCount = 0;
        pDMSResourcePrivate->stDrvDMS.bNeedGEstretch = FALSE;

        pDMSResourcePrivate->stDrvDMS.eDMS_MVOPFlowCtrl = E_DMS_MVOP_FLOW_UNKNOWN;
        pDMSResourcePrivate->stDrvDMS.bDMS_MVOPFlowFromDi = FALSE;
        pDMSResourcePrivate->stDrvDMS.eDMS_PreMVOPFlowCtrl = E_DMS_MVOP_FLOW_UNKNOWN;
        pDMSResourcePrivate->stDrvDMS.gDMS_MVOPShowBufferID = DMS_Invalid_ReadPointer_ID;
        pDMSResourcePrivate->stDrvDMS.gDMS_MVOPRemoveBufferID = DMS_Invalid_ReadPointer_ID;
        pDMSResourcePrivate->stDrvDMS.bMVOPMarkRemoveBuffer = FALSE;
        memset(&pDMSResourcePrivate->stDrvDMS.bIsWindowMute, 0, sizeof(MS_BOOL) * DMS_MAX_WINDOW_NUM);

        pDMSResourcePrivate->stDrvDMS.gDMS_ScalingCondition = 0;
        memset(pDMSResourcePrivate->stDrvDMS.gu8DMS_SetXC_Status, 0, sizeof(MS_U32) * DMS_MAX_WINDOW_NUM);
        pDMSResourcePrivate->stDrvDMS.gu8DMS_Seq_Change_ID = DMS_Invalid_ReadPointer_ID;
        memset(&pDMSResourcePrivate->stDrvDMS._stXC_XCDispWinInfo, 0, sizeof(pDMSResourcePrivate->stDrvDMS._stXC_XCDispWinInfo));
        memset(&pDMSResourcePrivate->stDrvDMS._stXC_XCCropWinInfo, 0, sizeof(pDMSResourcePrivate->stDrvDMS._stXC_XCCropWinInfo));
        memset(&pDMSResourcePrivate->stDrvDMS._stXC_XCDispWinInfo_DoubleBuffer, 0, sizeof(pDMSResourcePrivate->stDrvDMS._stXC_XCDispWinInfo_DoubleBuffer));
        memset(&pDMSResourcePrivate->stDrvDMS._stXC_XCCropWinInfo_DoubleBuffer, 0, sizeof(pDMSResourcePrivate->stDrvDMS._stXC_XCCropWinInfo_DoubleBuffer));

        memset(&pDMSResourcePrivate->stDrvDMS._stXC_LocalFrameRefCount, 0, sizeof(MS_U8) * DMS_MAX_WINDOW_NUM * DMS_DIP_QUEUEDEPTH);
        pDMSResourcePrivate->stDrvDMS.u32MVOPFreezeAddress_pa = 0x0;
        pDMSResourcePrivate->stDrvDMS.bDMS_Freeze_DoubleBuffer = FALSE;
        pDMSResourcePrivate->stDrvDMS._bForceSwitchBufferFlag = FALSE;
        memset(&pDMSResourcePrivate->stDrvDMS.u32ReleaseState, 0, sizeof(MS_U32) * DMS_DIP_QUEUEDEPTH);

        pDMSResourcePrivate->stDrvDMS.u32OutputRate = 0;
        memset(pDMSResourcePrivate->stDrvDMS.bStartCountFRC, 0, sizeof(MS_U32) * DMS_MAX_WINDOW_NUM);
        memset(pDMSResourcePrivate->stDrvDMS.u32AccumuleInputRate, 0, sizeof(MS_U32) * DMS_MAX_WINDOW_NUM);

        //Di
        pDMSResourcePrivate->stDrvDMSDi.bFlipToDiTask = FALSE;
        pDMSResourcePrivate->stDrvDMSDi._u8DMS_DiSetEventBufferID = DMS_Invalid_ReadPointer_ID;
        pDMSResourcePrivate->stDrvDMSDi._u8DMS_DiReleaseVDECFrameBufferID = DMS_Invalid_ReadPointer_ID;
        memset(&pDMSResourcePrivate->stDrvDMSDi._stXC_LocalFrameRefCountInterlace, 0, sizeof(MS_U8) * DMS_MAX_WINDOW_NUM * DMS_DIP_QUEUEDEPTH);
        memset(pDMSResourcePrivate->stDrvDMSDi.u8DIPReadytoCaptureIDInterlace,DMS_Invalid_ReadPointer_ID, sizeof(pDMSResourcePrivate->stDrvDMSDi.u8DIPReadytoCaptureIDInterlace));
        pDMSResourcePrivate->stDrvDMSDi.u8DIRingBufferID = 0;
        pDMSResourcePrivate->stDrvDMSDi._bDiNowShowRingBufID = INVALID_DI_RING_BUFFER_ID;
        pDMSResourcePrivate->stDrvDMSDi._bDiReleaseShowRingBufID = INVALID_DI_RING_BUFFER_ID;
        pDMSResourcePrivate->stDrvDMSDi._bSetwinToDiTask_DoubleBuffer = FALSE;
        for(u8WindowID=0; u8WindowID<DMS_MAX_WINDOW_NUM; u8WindowID++)
        {
            MS_U8 u8BufID = 0;
            for(u8BufID=0; u8BufID<DMS_DIP_QUEUEDEPTH; u8BufID++)
            {
                pDMSResourcePrivate->stDrvDMSDi._stXC_DIPDIInfo[u8WindowID][u8BufID].bValid = FALSE;
            }
        }
        pDMSResourcePrivate->stDrvDMSDi.u32DIPDIAddress_pa = 0x0;

        //Capture Mode
        memset(pDMSResourcePrivate->stDrvDMSCapMode._stXC_DIPCaptureWinInfo, 0 , sizeof(DMS_DIPCAPTUREWIN_INFO) * DMS_MAX_WINDOW_NUM);
        memset(&pDMSResourcePrivate->stDrvDMSCapMode._stXC_CaptureRingBufRefCount, 0, sizeof(MS_U8) * DMS_MAX_WINDOW_NUM * DMS_CAPTUREWIN_RING_BUFFER);
        memset(&pDMSResourcePrivate->stDrvDMSCapMode._stXC_LocalFrameRefCountCaptureMode, 0, sizeof(MS_U8) * DMS_MAX_WINDOW_NUM * DMS_DIP_QUEUEDEPTH);
        memset(&pDMSResourcePrivate->stDrvDMSCapMode._stXC_DIPCapWin_WriteReadPointer, 0, sizeof(DMS_DIPWriteReadPointer) * DMS_MAX_WINDOW_NUM);
        memset(&pDMSResourcePrivate->stDrvDMSCapMode._stXC_DIPCaptureWin_RingBufferPointer, 0, sizeof(DMS_DIPCaptureWin_WritePointer) * DMS_MAX_WINDOW_NUM);
        pDMSResourcePrivate->stDrvDMSCapMode.u8CapturedWinNum = 0;
        memset(pDMSResourcePrivate->stDrvDMSCapMode.u8frameRepeatCnt, 0, sizeof(MS_U8) * DMS_MAX_WINDOW_NUM * DMS_DIP_QUEUEDEPTH);
        memset(pDMSResourcePrivate->stDrvDMSCapMode.bIsFrameValid, 0, sizeof(MS_U32) * DMS_MAX_WINDOW_NUM * DMS_DIP_QUEUEDEPTH);
        memset(pDMSResourcePrivate->stDrvDMSCapMode.bIsRingBufAccessed, 0, sizeof(MS_U32) * DMS_CAPTUREWIN_RING_BUFFER);
#if DMS_DIP_Support_IMI
        memset(pDMSResourcePrivate->stDrvDMSCapMode._stXC_IMICaptureInfo, 0, sizeof(DMS_DIPCAPTURE_IMI_INFO) * DMS_MAX_WINDOW_NUM);
#endif
        pDMSResourcePrivate->stDrvDMS.bFirstPlay = FALSE;
        pDMSResourcePrivate->stDrvDMS.bDisableBlackScreen = FALSE;
        pDMSResourcePrivate->stDrvDMS.u32MuteCounter = 0;
        pDMSResourcePrivate->stDrvDMS.bXCEnableBob = FALSE;
#endif
    }
}

MS_U32 DMSOpen(void** ppInstance, const void* const pAttribute)
{
    DMS_INSTANT_PRIVATE *pDMSPri = NULL;

    //UTOPIA_TRACE(MS_UTOPIA_DB_LEVEL_TRACE,printf("enter %s %d\n",__FUNCTION__,__LINE__));
    // instance is allocated here, also can allocate private for internal use, ex, BDMA_INSTANT_PRIVATE
    UtopiaInstanceCreate(sizeof(DMS_INSTANT_PRIVATE), ppInstance);
    // setup func in private and assign the calling func in func ptr in instance private
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pDMSPri);

    pDMSPri->fpDMSInit              = MApi_DMS_Init_U2;
    pDMSPri->fpDMSSetMemType        = MApi_DMS_SetMemory_U2;
    pDMSPri->fpDMSCreateWin         = MApi_DMS_CreateWindow_U2;
    pDMSPri->fpDMSSetDigiDecInfo    = MApi_DMS_SetDigitalDecodeSignalInfo_U2;
    pDMSPri->fpDMSClearDigiDecInfo  = MApi_DMS_ClearDigitalDecodeSignalInfo_U2;
    pDMSPri->fpDMSDestoryWin        = MApi_DMS_DestroyWindow_U2;
    pDMSPri->fpDMSSetWin            = MApi_DMS_SetWindow_U2;
    pDMSPri->fpDMSGetWinInfo        = MApi_DMS_GetWindowInfo_U2;
    pDMSPri->fpDMSFlip              = MApi_DMS_Video_Flip_U2;
    pDMSPri->fpDMSZOrder            = MApi_DMS_SetZOrder_U2;
    pDMSPri->fpDMSMute              = MApi_DMS_Video_Mute_U2;
    pDMSPri->fpDMSSetMuteColor      = MApi_DMS_Set_MuteColor_U2;
    pDMSPri->fpDMSFreeze            = MApi_DMS_Video_Freeze_U2;
    pDMSPri->fpDMSGetStatus         = MApi_DMS_GetStatus_U2;
    pDMSPri->fpDMSGetCaps           = MApi_DMS_GetCapability_U2;
    pDMSPri->fpDMSGetExistWin       = MApi_DMS_GetExistedWindows_U2;
    pDMSPri->fpDMSSetOutputLayer    = MApi_DMS_SetOutputLayer_U2;
    pDMSPri->fpDMSSet3DMode         = MApi_DMS_Set_3D_Mode_U2;
    pDMSPri->fpDMSSetCaptureInfo    = MApi_DMS_Set_CaptureInfo_U2;
    pDMSPri->fpDMSGetCaptureBuf     = MApi_DMS_Get_CaptureBuffer_U2;
    pDMSPri->fpDMSReleaseCaptureBuf = MApi_DMS_Release_CaptureBuffer_U2;

	return UTOPIA_STATUS_SUCCESS;
}

MS_U32 DMSIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    if(pInstance == NULL)
    {
        printf("[%s] pInstance is NULL\n",__FUNCTION__);
        return UTOPIA_STATUS_FAIL;
    }
	void* pModule = NULL;
	UtopiaInstanceGetModule(pInstance, &pModule);
    DMS_INSTANT_PRIVATE* psDMSInstPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void**)&psDMSInstPri);
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    switch(u32Cmd)
    {
        case E_DMS_CMD_INIT:
        {
            PST_DMS_INIT ptr = (PST_DMS_INIT)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSInit(pInstance, ptr->pstDMS_InitData);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DMS_CMD_SETMEMORY:
        {
            PST_DMS_SETMEMTYPE ptr = (PST_DMS_SETMEMTYPE)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSSetMemType(pInstance, ptr->pstDMS_SetMemType);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DMS_CMD_CREATEWINDOW:
        {
            PST_DMS_CREATEWIN ptr = (PST_DMS_CREATEWIN)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSCreateWin(pInstance, ptr->pstOutputWin, ptr->pstCreateWin_Info, ptr->pu32WindowID);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DMS_CMD_SETDIGITALDECODESIGNALINFO:
        {
            PST_DMS_SETDIGIDECINFO ptr = (PST_DMS_SETDIGIDECINFO)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSSetDigiDecInfo(pInstance, ptr->u32WindowID, ptr->pstDispFrameFormat);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DMS_CMD_CLEARDIGITALDECODESIGNALINFO:
        {
            PST_DMS_CLEARDIGIDECINFO ptr = (PST_DMS_CLEARDIGIDECINFO)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSClearDigiDecInfo(pInstance, ptr->u32WindowID);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DMS_CMD_DESTORYWINDOW:
        {
            PST_DMS_DESTORYWIN ptr = (PST_DMS_DESTORYWIN)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSDestoryWin(pInstance, ptr->u32WindowID);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DMS_CMD_SETWINDOW:
        {
            PST_DMS_SETWIN ptr = (PST_DMS_SETWIN)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSSetWin(pInstance, ptr->u32WindowID, ptr->pstDMS_SetWin_Info);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DMS_CMD_GETWINDOWINFO:
        {
            PST_DMS_GETWININFO ptr = (PST_DMS_GETWININFO)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSGetWinInfo(pInstance, ptr->u32WindowID, ptr->pstDMS_GetWin_Info);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DMS_CMD_VIDEOFLIP:
        {
            PST_DMS_FLIP ptr = (PST_DMS_FLIP)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSFlip(pInstance, ptr->u32WindowID, ptr->pstDispFrameFormat);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DMS_CMD_SETZORDER:
        {
            PST_DMS_ZORDER ptr = (PST_DMS_ZORDER)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSZOrder(pInstance, ptr->u32WindowID, ptr->pstZorderInfo);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DMS_CMD_VIDEOMUTE:
        {
            PST_DMS_MUTE ptr = (PST_DMS_MUTE)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSMute(pInstance, ptr->u32WindowID, ptr->pstWindowMute);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DMS_CMD_SETMUTECOLOR:
        {
            PST_DMS_MUTECOLOR ptr = (PST_DMS_MUTECOLOR)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSSetMuteColor(pInstance, ptr->u32WindowID, ptr->pstMuteColor);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DMS_CMD_VIDEOFREEZE:
        {
            PST_DMS_FREEZE ptr = (PST_DMS_FREEZE)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSFreeze(pInstance, ptr->u32WindowID, ptr->pstWindowFreeze);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DMS_CMD_GETSTATUS:
        {
            PST_DMS_GETSTATUS ptr = (PST_DMS_GETSTATUS)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSGetStatus(pInstance, ptr->peStatus);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DMS_CMD_GETCAPS:
        {
            PST_DMS_GETCAPS ptr = (PST_DMS_GETCAPS)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSGetCaps(pInstance, ptr->peCapability);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DMS_CMD_GETEXISTEDWINDOW:
        {
            PST_DMS_GETEXISTWIN ptr = (PST_DMS_GETEXISTWIN)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSGetExistWin(pInstance, ptr->pu32WindowNums);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DMS_CMD_SETOUTPUTLAYER:
        {
            PST_DMS_SETOUTPUTLAYER ptr = (PST_DMS_SETOUTPUTLAYER)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSSetOutputLayer(pInstance, ptr->pstLayer);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DMS_CMD_SET3DMODE:
        {
            PST_DMS_SET3DMODE ptr = (PST_DMS_SET3DMODE)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSSet3DMode(pInstance, ptr->u32WindowID, ptr->pst3DInfo);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DMS_CMD_SETCAPTUREINFO:
        {
            PST_DMS_SETCAPTUREINFO ptr = (PST_DMS_SETCAPTUREINFO)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSSetCaptureInfo(pInstance, ptr->u32WindowID, ptr->pstSetCaptureInfo);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DMS_CMD_GETCAPTUREBUFFER:
        {
            PST_DMS_GETCAPTUREBUF ptr = (PST_DMS_GETCAPTUREBUF)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSGetCaptureBuf(pInstance, ptr->u32WindowID, ptr->pstCaptureInfo);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        case E_DMS_CMD_RELEASECAPTUREBUFFER:
        {
            PST_DMS_RELEASECAPTUREBUF ptr = (PST_DMS_RELEASECAPTUREBUF)pArgs;
            ptr->eReturnValue = psDMSInstPri->fpDMSReleaseCaptureBuf(pInstance, ptr->u32WindowID, ptr->pstCaptureInfo);
            u32Return = UTOPIA_STATUS_SUCCESS;
            break;
        }
        default:
            printf("[%s] out of DMS cmd:%td\n",__FUNCTION__,(ptrdiff_t)u32Cmd);
            break;
    };

	return u32Return;
}

MS_U32 DMSClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return TRUE;
}


