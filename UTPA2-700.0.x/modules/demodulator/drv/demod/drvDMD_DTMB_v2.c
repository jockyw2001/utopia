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
//        including without limitation, you and yor any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitra«–cordance
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvDMD_DTMB_v2.c
/// @brief  DMD DTMB Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#include <stdio.h>
#endif

#include "utopia.h"
#include "utopia_dapi.h"

#include "drvDMD_DTMB_v2.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#ifdef MS_DEBUG
#define DMD_DBG(x)          (x)
#else
#define DMD_DBG(x)          //(x)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

typedef enum _DTMB_POOL_ID
{
    DTMB_POOL_ID_DMD0 = 0
} DTMB_POOL_ID;

typedef struct DLL_PACKED _DTMB_RESOURCE_PRIVATE
{
    DMD_DTMB_ResData sDMD_DTMB_ResData[DMD_DTMB_MAX_DEMOD_NUM];
} DTMB_RESOURCE_PRIVATE, *PDTMB_RESOURCE_PRIVATE;;

#ifdef UTPA2

typedef MS_BOOL                   (*IOCTL_DTMB_SetDbgLevel)(DMD_DTMB_DbgLv);
typedef DMD_DTMB_Info*            (*IOCTL_DTMB_GetInfo)(void);
typedef MS_BOOL                   (*IOCTL_DTMB_GetLibVer)(const MSIF_Version**);
typedef MS_BOOL                   (*IOCTL_DTMB_Init)(MS_U8, DMD_DTMB_InitData*, MS_U32);
typedef MS_BOOL                   (*IOCTL_DTMB_Exit)(MS_U8);
typedef MS_U32                    (*IOCTL_DTMB_GetConfig)(MS_U8, DMD_DTMB_InitData*);       
typedef MS_BOOL                   (*IOCTL_DTMB_SetConfig)(MS_U8, DMD_DTMB_DEMOD_TYPE, MS_BOOL);
typedef MS_BOOL                   (*IOCTL_DTMB_SetReset)(MS_U8);
typedef MS_BOOL                   (*IOCTL_DTMB_Set_QAM_SR)(MS_U8, DMD_DTMB_DEMOD_TYPE, MS_U16);
typedef MS_BOOL                   (*IOCTL_DTMB_SetActive)(MS_U8, MS_BOOL);
typedef MS_U32                    (*IOCTL_DTMB_SetPowerState)(MS_U8, EN_POWER_MODE);
typedef DMD_DTMB_LOCK_STATUS      (*IOCTL_DTMB_GetLock)(MS_U8, DMD_DTMB_GETLOCK_TYPE);
typedef MS_BOOL                   (*IOCTL_DTMB_GetModulationMode)(MS_U8, DMD_DTMB_MODULATION_INFO*);
typedef MS_BOOL                   (*IOCTL_DTMB_GetSignalStrength)(MS_U8, MS_U16*);
typedef MS_BOOL                   (*IOCTL_DTMB_ReadFrequencyOffset)(MS_U8, DMD_DTMB_CFO_DATA*);
typedef MS_U8                     (*IOCTL_DTMB_GetSignalQuality)(MS_U8);
typedef MS_BOOL                   (*IOCTL_DTMB_GetPreLdpcBer)(MS_U8, DMD_DTMB_BER_DATA*);
typedef MS_BOOL                   (*IOCTL_DTMB_GetPreViterbiBer)(MS_U8, DMD_DTMB_BER_DATA*);
typedef MS_BOOL                   (*IOCTL_DTMB_GetPostViterbiBer)(MS_U8, DMD_DTMB_BER_DATA*);
typedef MS_BOOL                   (*IOCTL_DTMB_GetSNR)(MS_U8, DMD_DTMB_SNR_DATA*);
typedef MS_BOOL                   (*IOCTL_DTMB_SetSerialControl)(MS_U8, MS_U8);
typedef MS_BOOL                   (*IOCTL_DTMB_IIC_BYPASS_MODE)(MS_U8, MS_BOOL);
typedef MS_BOOL                   (*IOCTL_DTMB_SWITCH_SSPI_GPIO)(MS_U8, MS_BOOL);
typedef MS_BOOL                   (*IOCTL_DTMB_GPIO_GET_LEVEL)(MS_U8, MS_U8, MS_BOOL*);
typedef MS_BOOL                   (*IOCTL_DTMB_GPIO_SET_LEVEL)(MS_U8, MS_U8, MS_BOOL);
typedef MS_BOOL                   (*IOCTL_DTMB_GPIO_OUT_ENABLE)(MS_U8, MS_U8, MS_BOOL);
typedef MS_BOOL                   (*IOCTL_DTMB_DoIQSwap)(MS_U8, MS_BOOL);
typedef MS_BOOL                   (*IOCTL_DTMB_GetReg)(MS_U8, MS_U16, MS_U8*);
typedef MS_BOOL                   (*IOCTL_DTMB_SetReg)(MS_U8, MS_U16, MS_U8);

typedef struct _DTMB_INSTANT_PRIVATE
{
    IOCTL_DTMB_SetDbgLevel              fpSetDbgLevel;
    IOCTL_DTMB_GetInfo                  fpGetInfo;
    IOCTL_DTMB_GetLibVer                fpGetLibVer;
    IOCTL_DTMB_Init                     fpInit;
    IOCTL_DTMB_Exit                     fpExit;
    IOCTL_DTMB_GetConfig                fpGetConfig;            
    IOCTL_DTMB_SetConfig                fpSetConfig;
    IOCTL_DTMB_SetReset                 fpSetReset;
    IOCTL_DTMB_Set_QAM_SR               fpSet_QAM_SR;
    IOCTL_DTMB_SetActive                fpSetActive;
    #if DMD_DTMB_STR_EN
    IOCTL_DTMB_SetPowerState            fpSetPowerState;
    #endif
    IOCTL_DTMB_GetLock                  fpGetLock;
    IOCTL_DTMB_GetModulationMode        fpGetModulationMode;
    IOCTL_DTMB_GetSignalStrength        fpGetSignalStrength;
    IOCTL_DTMB_ReadFrequencyOffset      fpReadFrequencyOffset;
    IOCTL_DTMB_GetSignalQuality         fpGetSignalQuality;
    IOCTL_DTMB_GetPreLdpcBer            fpGetPreLdpcBer;
    IOCTL_DTMB_GetPreViterbiBer         fpGetPreViterbiBer;
    IOCTL_DTMB_GetPostViterbiBer        fpGetPostViterbiBer;
    IOCTL_DTMB_GetSNR                   fpGetSNR;
    IOCTL_DTMB_SetSerialControl         fpSetSerialControl;
    IOCTL_DTMB_IIC_BYPASS_MODE          fpIIC_BYPASS_MODE;
    IOCTL_DTMB_SWITCH_SSPI_GPIO         fpSWITCH_SSPI_GPIO;
    IOCTL_DTMB_GPIO_GET_LEVEL           fpGPIO_GET_LEVEL;
    IOCTL_DTMB_GPIO_SET_LEVEL           fpGPIO_SET_LEVEL;
    IOCTL_DTMB_GPIO_OUT_ENABLE          fpGPIO_OUT_ENABLE;
    IOCTL_DTMB_DoIQSwap                 fpDoIQSwap;
    IOCTL_DTMB_GetReg                   fpGetReg;
    IOCTL_DTMB_SetReg                   fpSetReg;
} DTMB_INSTANT_PRIVATE;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

extern DMD_DTMB_ResData *psDMD_DTMB_ResData;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

MS_U32 DTMBOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    DTMB_INSTANT_PRIVATE *pDtmbPri = NULL;
    void *pDtmbPriVoid = NULL;
    
	printf("\n[DTMB INFO] dtmb open");
    
    // instance is allocated here, also can allocate private for internal use, ex, BDMA_INSTANT_PRIVATE
    UtopiaInstanceCreate(sizeof(DTMB_INSTANT_PRIVATE), ppInstance);
    // setup func in private and assign the calling func in func ptr in instance private
    UtopiaInstanceGetPrivate(*ppInstance, &pDtmbPriVoid);
    pDtmbPri = (DTMB_INSTANT_PRIVATE*)pDtmbPriVoid;
    
    pDtmbPri->fpSetDbgLevel         = _MDrv_DMD_DTMB_SetDbgLevel;
    pDtmbPri->fpGetInfo             = _MDrv_DMD_DTMB_GetInfo;
    pDtmbPri->fpGetLibVer           = _MDrv_DMD_DTMB_GetLibVer;
    pDtmbPri->fpInit                = _MDrv_DMD_DTMB_MD_Init;
    pDtmbPri->fpExit                = _MDrv_DMD_DTMB_MD_Exit;
    pDtmbPri->fpGetConfig           = _MDrv_DMD_DTMB_MD_GetConfig; 
    pDtmbPri->fpSetConfig           = _MDrv_DMD_DTMB_MD_SetConfig;
    pDtmbPri->fpSetReset            = _MDrv_DMD_DTMB_MD_SetReset;
    pDtmbPri->fpSet_QAM_SR          = _MDrv_DMD_DTMB_MD_Set_QAM_SR;
    pDtmbPri->fpSetActive           = _MDrv_DMD_DTMB_MD_SetActive;
    #if DMD_DTMB_STR_EN
    pDtmbPri->fpSetPowerState       = _MDrv_DMD_DTMB_MD_SetPowerState;
    #endif
    pDtmbPri->fpGetLock             = _MDrv_DMD_DTMB_MD_GetLock;
    pDtmbPri->fpGetModulationMode   = _MDrv_DMD_DTMB_MD_GetModulationMode;
    pDtmbPri->fpGetSignalStrength   = _MDrv_DMD_DTMB_MD_GetSignalStrength;
    pDtmbPri->fpReadFrequencyOffset = _MDrv_DMD_DTMB_MD_ReadFrequencyOffset;
    pDtmbPri->fpGetSignalQuality    = _MDrv_DMD_DTMB_MD_GetSignalQuality;
    pDtmbPri->fpGetPreLdpcBer       = _MDrv_DMD_DTMB_MD_GetPreLdpcBer;
    pDtmbPri->fpGetPreViterbiBer    = _MDrv_DMD_DTMB_MD_GetPreViterbiBer;
    pDtmbPri->fpGetPostViterbiBer   = _MDrv_DMD_DTMB_MD_GetPostViterbiBer;
    pDtmbPri->fpGetSNR              = _MDrv_DMD_DTMB_MD_GetSNR;
    pDtmbPri->fpSetSerialControl    = _MDrv_DMD_DTMB_MD_SetSerialControl;
    pDtmbPri->fpIIC_BYPASS_MODE     = _MDrv_DMD_DTMB_MD_IIC_BYPASS_MODE;
    pDtmbPri->fpSWITCH_SSPI_GPIO    = _MDrv_DMD_DTMB_MD_SWITCH_SSPI_GPIO;
    pDtmbPri->fpGPIO_GET_LEVEL      = _MDrv_DMD_DTMB_MD_GPIO_GET_LEVEL;
    pDtmbPri->fpGPIO_SET_LEVEL      = _MDrv_DMD_DTMB_MD_GPIO_SET_LEVEL;
    pDtmbPri->fpGPIO_OUT_ENABLE     = _MDrv_DMD_DTMB_MD_GPIO_OUT_ENABLE;
    pDtmbPri->fpDoIQSwap            = _MDrv_DMD_DTMB_MD_DoIQSwap;
    pDtmbPri->fpGetReg              = _MDrv_DMD_DTMB_MD_GetReg;
    pDtmbPri->fpSetReg              = _MDrv_DMD_DTMB_MD_SetReg;
     
	return UTOPIA_STATUS_SUCCESS;
}

#if DMD_DTMB_STR_EN
MS_U32 DTMBStr(MS_U32 u32PowerState, void* pModule)
{
    void* pResource = NULL;
	DTMB_RESOURCE_PRIVATE* psDTMBResPri = NULL;

	MS_U32 i = 0;
	MS_U32 u32Ret = FALSE;

	if (UtopiaResourceObtain(pModule, DTMB_POOL_ID_DMD0, &pResource) != 0)
    {
        printf("UtopiaResourceObtainToInstant fail\n");
	    return UTOPIA_STATUS_ERR_RESOURCE;
    }

    if (UtopiaResourceGetPrivate(pResource, (void*)&psDTMBResPri) != 0)
    {
        printf("UtopiaResourceGetPrivate fail\n");
        return UTOPIA_STATUS_FAIL;
    }

    psDMD_DTMB_ResData = ((PDTMB_RESOURCE_PRIVATE)psDTMBResPri)->sDMD_DTMB_ResData;
    
    do
    {
        u32Ret = _MDrv_DMD_DTMB_MD_SetPowerState(i++, u32PowerState);
    } while (i < DMD_DTMB_MAX_DEMOD_NUM && u32Ret);
    
    UtopiaResourceRelease(pResource);

    return (u32Ret ? UTOPIA_STATUS_SUCCESS : UTOPIA_STATUS_FAIL);
}
#endif

MS_U32 DTMBIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
	UtopiaInstanceGetModule(pInstance, &pModule);
	void* pResource = NULL;
	DTMB_RESOURCE_PRIVATE* psDTMBResPri = NULL;
    DTMB_INSTANT_PRIVATE* psDTMBInstPri = NULL;
    void* psDTMBInstPriVoid = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void**)&psDTMBInstPriVoid);
    psDTMBInstPri = (DTMB_INSTANT_PRIVATE*)psDTMBInstPriVoid;
    
    MS_U32 u32Ret = FALSE;
    
    if (UtopiaResourceObtain(pModule, DTMB_POOL_ID_DMD0, &pResource) != 0)
    {
        printf("UtopiaResourceObtainToInstant fail\n");
	    return UTOPIA_STATUS_ERR_RESOURCE;
    }
    
    if (UtopiaResourceGetPrivate(pResource, (void*)&psDTMBResPri) != 0)
    {
        printf("UtopiaResourceGetPrivate fail\n");
        return UTOPIA_STATUS_FAIL;
    }
    
    psDMD_DTMB_ResData = ((PDTMB_RESOURCE_PRIVATE)psDTMBResPri)->sDMD_DTMB_ResData;
    
    switch (u32Cmd)
    {
        case DMD_DTMB_DRV_CMD_SetDbgLevel:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_SetDbgLevel\n"));
            u32Ret = psDTMBInstPri->fpSetDbgLevel(((PDTMB_DBG_LEVEL_PARAM)pArgs)->u8DbgLevel);
            break;
        case DMD_DTMB_DRV_CMD_GetInfo:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_GetInfo\n"));
            ((PDTMB_GET_INFO_PARAM)pArgs)->pInfo = psDTMBInstPri->fpGetInfo();
            u32Ret = TRUE;
            break;
        case DMD_DTMB_DRV_CMD_GetLibVer:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_GetLibVer\n"));
            u32Ret = psDTMBInstPri->fpGetLibVer(((PDTMB_GET_LIB_VER_PARAM)pArgs)->ppVersion);
            break;
        case DMD_DTMB_DRV_CMD_Init:
            ((PDTMB_INIT_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_Init:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_Init\n"));
            u32Ret = psDTMBInstPri->fpInit(((PDTMB_INIT_PARAM)pArgs)->id, ((PDTMB_INIT_PARAM)pArgs)->pDMD_DTMB_InitData, ((PDTMB_INIT_PARAM)pArgs)->u32InitDataLen);
            break;
        case DMD_DTMB_DRV_CMD_Exit:
            ((PDTMB_ID_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_Exit:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_Exit\n"));
            u32Ret = psDTMBInstPri->fpExit(((PDTMB_ID_PARAM)pArgs)->id);
            break;
        case DMD_DTMB_DRV_CMD_GetConfig:
            ((PDTMB_INIT_PARAM)pArgs)->id = 0; 
        case DMD_DTMB_DRV_CMD_MD_GetConfig:    
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_GetConfig\n"));
            u32Ret = psDTMBInstPri->fpGetConfig(((PDTMB_INIT_PARAM)pArgs)->id, ((PDTMB_INIT_PARAM)pArgs)->pDMD_DTMB_InitData);
            break;
        case DMD_DTMB_DRV_CMD_SetConfig:
            ((PDTMB_SET_CONFIG_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_SetConfig:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_SetConfig\n"));
            u32Ret = psDTMBInstPri->fpSetConfig(((PDTMB_SET_CONFIG_PARAM)pArgs)->id, ((PDTMB_SET_CONFIG_PARAM)pArgs)->eType, ((PDTMB_SET_CONFIG_PARAM)pArgs)->bEnable);
            break;
        case DMD_DTMB_DRV_CMD_SetReset:
            ((PDTMB_ID_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_SetReset:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_SetReset\n"));
            u32Ret = psDTMBInstPri->fpSetReset(((PDTMB_ID_PARAM)pArgs)->id);
            break;
        case DMD_DTMB_DRV_CMD_Set_QAM_SR:
            ((PDTMB_SET_QAM_SR_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_Set_QAM_SR:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_Set_QAM_SR\n"));
            u32Ret = psDTMBInstPri->fpSet_QAM_SR(((PDTMB_SET_QAM_SR_PARAM)pArgs)->id, ((PDTMB_SET_QAM_SR_PARAM)pArgs)->eType, ((PDTMB_SET_QAM_SR_PARAM)pArgs)->symbol_rate);
            break;
        case DMD_DTMB_DRV_CMD_SetActive:
            ((PDTMB_SET_ACTIVE_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_SetActive:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_SetActive\n"));
            u32Ret = psDTMBInstPri->fpSetActive(((PDTMB_SET_ACTIVE_PARAM)pArgs)->id, ((PDTMB_SET_ACTIVE_PARAM)pArgs)->bEnable);
            break;
        case DMD_DTMB_DRV_CMD_SetPowerState:
            ((PDTMB_SET_POWER_STATE_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_SetPowerState:
            #if DMD_DTMB_STR_EN
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_SetPowerState\n"));
            u32Ret = psDTMBInstPri->fpSetPowerState(((PDTMB_SET_POWER_STATE_PARAM)pArgs)->id, ((PDTMB_SET_POWER_STATE_PARAM)pArgs)->u16PowerState);
            #else
            printf("DTMBIoctl - Don't support MDrv_DMD_DTMB_MD_SetPowerState\n");
            #endif
            break;
        case DMD_DTMB_DRV_CMD_GetLock:
            ((PDTMB_GET_LOCK_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_GetLock:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_GetLock\n"));
            ((PDTMB_GET_LOCK_PARAM)pArgs)->status = psDTMBInstPri->fpGetLock(((PDTMB_GET_LOCK_PARAM)pArgs)->id, ((PDTMB_GET_LOCK_PARAM)pArgs)->eType);
            u32Ret = TRUE;
            break;
        case DMD_DTMB_DRV_CMD_GetModulationMode:
            ((PDTMB_GET_MODULATION_MODE_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_GetModulationMode:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_GetModulationMode\n"));
            u32Ret = psDTMBInstPri->fpGetModulationMode(((PDTMB_GET_MODULATION_MODE_PARAM)pArgs)->id, &(((PDTMB_GET_MODULATION_MODE_PARAM)pArgs)->info));
            break;
        case DMD_DTMB_DRV_CMD_GetSignalStrength:
            ((PDTMB_GET_SIGNAL_STRENGTH_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_GetSignalStrength:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_GetSignalStrength\n"));
            u32Ret = psDTMBInstPri->fpGetSignalStrength(((PDTMB_GET_SIGNAL_STRENGTH_PARAM)pArgs)->id, &(((PDTMB_GET_SIGNAL_STRENGTH_PARAM)pArgs)->u16Strength));
            break;
        case DMD_DTMB_DRV_CMD_ReadFrequencyOffset:
            ((PDTMB_READ_FREQ_OFFSET_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_ReadFrequencyOffset:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_ReadFrequencyOffset\n"));
            u32Ret = psDTMBInstPri->fpReadFrequencyOffset(((PDTMB_READ_FREQ_OFFSET_PARAM)pArgs)->id, &(((PDTMB_READ_FREQ_OFFSET_PARAM)pArgs)->cfo));
            break;
        case DMD_DTMB_DRV_CMD_GetSignalQuality:
            ((PDTMB_GET_SIGNAL_QUALITY_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_GetSignalQuality:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_GetSignalQuality\n"));
            ((PDTMB_GET_SIGNAL_QUALITY_PARAM)pArgs)->u8Percentage = psDTMBInstPri->fpGetSignalQuality(((PDTMB_GET_SIGNAL_QUALITY_PARAM)pArgs)->id);
            u32Ret = TRUE;
            break;
        case DMD_DTMB_DRV_CMD_GetPreLdpcBer:
            ((PDTMB_GET_BER_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_GetPreLdpcBer:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_GetPreLdpcBer\n"));
            u32Ret = psDTMBInstPri->fpGetPreLdpcBer(((PDTMB_GET_BER_PARAM)pArgs)->id, &(((PDTMB_GET_BER_PARAM)pArgs)->ber));
            break;
        case DMD_DTMB_DRV_CMD_GetPreViterbiBer:
        case DMD_DTMB_DRV_CMD_MD_GetPreViterbiBer:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_GetPreViterbiBer\n"));
            u32Ret = psDTMBInstPri->fpGetPreViterbiBer(((PDTMB_GET_BER_PARAM)pArgs)->id, &(((PDTMB_GET_BER_PARAM)pArgs)->ber));
            break;
        case DMD_DTMB_DRV_CMD_GetPostViterbiBer:
            ((PDTMB_GET_BER_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_GetPostViterbiBer:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_GetPostViterbiBer\n"));
            u32Ret = psDTMBInstPri->fpGetPostViterbiBer(((PDTMB_GET_BER_PARAM)pArgs)->id, &(((PDTMB_GET_BER_PARAM)pArgs)->ber));
            break;
        case DMD_DTMB_DRV_CMD_GetSNR:
            ((PDTMB_GET_SNR_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_GetSNR:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_GetSNR\n"));
            u32Ret = psDTMBInstPri->fpGetSNR(((PDTMB_GET_SNR_PARAM)pArgs)->id, &(((PDTMB_GET_SNR_PARAM)pArgs)->snr));
            break;
        case DMD_DTMB_DRV_CMD_SetSerialControl:
            ((PDTMB_SET_SERIAL_CONTROL_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_SetSerialControl:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_SetSerialControl\n"));
            u32Ret = psDTMBInstPri->fpSetSerialControl(((PDTMB_SET_SERIAL_CONTROL_PARAM)pArgs)->id, ((PDTMB_SET_SERIAL_CONTROL_PARAM)pArgs)->u8TsConfigData);
            break;
        case DMD_DTMB_DRV_CMD_IIC_BYPASS_MODE:
            ((PDTMB_IIC_BYPASS_MODE_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_IIC_BYPASS_MODE:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_IIC_BYPASS_MODE\n"));
            u32Ret = psDTMBInstPri->fpIIC_BYPASS_MODE(((PDTMB_IIC_BYPASS_MODE_PARAM)pArgs)->id, ((PDTMB_IIC_BYPASS_MODE_PARAM)pArgs)->bEnable);
            break;
        case DMD_DTMB_DRV_CMD_SWITCH_SSPI_GPIO:
            ((PDTMB_SWITCH_SSPI_GPIO_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_SWITCH_SSPI_GPIO:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_SWITCH_SSPI_GPIO\n"));
            u32Ret = psDTMBInstPri->fpSWITCH_SSPI_GPIO(((PDTMB_SWITCH_SSPI_GPIO_PARAM)pArgs)->id, ((PDTMB_SWITCH_SSPI_GPIO_PARAM)pArgs)->bEnable);
            break;
        case DMD_DTMB_DRV_CMD_GPIO_GET_LEVEL:
            ((PDTMB_GPIO_LEVEL_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_GPIO_GET_LEVEL:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_GPIO_GET_LEVEL\n"));
            u32Ret = psDTMBInstPri->fpGPIO_GET_LEVEL(((PDTMB_GPIO_LEVEL_PARAM)pArgs)->id, ((PDTMB_GPIO_LEVEL_PARAM)pArgs)->u8Pin, &(((PDTMB_GPIO_LEVEL_PARAM)pArgs)->bLevel));
            break;
        case DMD_DTMB_DRV_CMD_GPIO_SET_LEVEL:
            ((PDTMB_GPIO_LEVEL_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_GPIO_SET_LEVEL:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_GPIO_SET_LEVEL\n"));
            u32Ret = psDTMBInstPri->fpGPIO_SET_LEVEL(((PDTMB_GPIO_LEVEL_PARAM)pArgs)->id, ((PDTMB_GPIO_LEVEL_PARAM)pArgs)->u8Pin, ((PDTMB_GPIO_LEVEL_PARAM)pArgs)->bLevel);
            break;
        case DMD_DTMB_DRV_CMD_GPIO_OUT_ENABLE:
            ((PDTMB_GPIO_OUT_ENABLE_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_GPIO_OUT_ENABLE:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_GPIO_OUT_ENABLE\n"));
            u32Ret = psDTMBInstPri->fpGPIO_OUT_ENABLE(((PDTMB_GPIO_OUT_ENABLE_PARAM)pArgs)->id, ((PDTMB_GPIO_OUT_ENABLE_PARAM)pArgs)->u8Pin, ((PDTMB_GPIO_OUT_ENABLE_PARAM)pArgs)->bEnableOut);
            break;
        case DMD_DTMB_DRV_CMD_DoIQSwap:
            ((PDTMB_DO_IQSWAP_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_DoIQSwap:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_DoIQSwap\n"));
            u32Ret = psDTMBInstPri->fpDoIQSwap(((PDTMB_DO_IQSWAP_PARAM)pArgs)->id, ((PDTMB_DO_IQSWAP_PARAM)pArgs)->bIsQPad);
            break;
        case DMD_DTMB_DRV_CMD_GetReg:
            ((PDTMB_REG_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_GetReg:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_GetReg\n"));
            u32Ret = psDTMBInstPri->fpGetReg(((PDTMB_REG_PARAM)pArgs)->id, ((PDTMB_REG_PARAM)pArgs)->u16Addr, &(((PDTMB_REG_PARAM)pArgs)->u8Data));
            break;
        case DMD_DTMB_DRV_CMD_SetReg:
            ((PDTMB_REG_PARAM)pArgs)->id = 0;
        case DMD_DTMB_DRV_CMD_MD_SetReg:
            DMD_DBG(printf("DTMBIoctl - MDrv_DMD_DTMB_MD_SetReg\n"));
            u32Ret = psDTMBInstPri->fpSetReg(((PDTMB_REG_PARAM)pArgs)->id, ((PDTMB_REG_PARAM)pArgs)->u16Addr, ((PDTMB_REG_PARAM)pArgs)->u8Data);
            break;
        default:
            break;
    }
    
    UtopiaResourceRelease(pResource);
            
    return (u32Ret ? UTOPIA_STATUS_SUCCESS : UTOPIA_STATUS_FAIL);
}

MS_U32 DTMBClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);

	return UTOPIA_STATUS_SUCCESS;
}

void DTMBRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
	UtopiaModuleCreate(MODULE_DTMB, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)DTMBOpen, (FUtopiaClose)DTMBClose, (FUtopiaIOctl)DTMBIoctl);
    // register func for module, after register here, then ap call UtopiaSTR can call to registered standard func
    #if DMD_DTMB_STR_EN
    UtopiaModuleSetupSTRFunctionPtr(pUtopiaModule, (FUtopiaSTR)DTMBStr);
    #endif
    
    // 2. deal with resource
    void* psResource = NULL;
    // start func to add res, call once will create 2 access in resource. Also can declare BDMA_POOL_ID_BDMA1 for another channel depend on driver owner.
	UtopiaModuleAddResourceStart(pUtopiaModule, DTMB_POOL_ID_DMD0);
    // resource can alloc private for internal use, ex, BDMA_RESOURCE_PRIVATE
    UtopiaResourceCreate("dtmb0", sizeof(DTMB_RESOURCE_PRIVATE), &psResource);
    // func to reg res
    UtopiaResourceRegister(pUtopiaModule, psResource, DTMB_POOL_ID_DMD0);
    // end function to add res
	UtopiaModuleAddResourceEnd(pUtopiaModule, DTMB_POOL_ID_DMD0);
}

#endif // #ifdef UTPA2
