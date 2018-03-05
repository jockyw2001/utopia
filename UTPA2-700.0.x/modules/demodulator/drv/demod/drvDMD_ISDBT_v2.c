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
/// file    drvDMD_ISDBT_v2.c
/// @brief  DMD ISDBT Driver Interface
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

#include "drvDMD_ISDBT_v2.h"

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

typedef enum _ISDBT_POOL_ID
{
    ISDBT_POOL_ID_DMD0 = 0
} ISDBT_POOL_ID;

typedef struct DLL_PACKED _ISDBT_RESOURCE_PRIVATE
{
    DMD_ISDBT_ResData sDMD_ISDBT_ResData[DMD_ISDBT_MAX_DEMOD_NUM];
} ISDBT_RESOURCE_PRIVATE, *PISDBT_RESOURCE_PRIVATE;

#ifdef UTPA2

typedef MS_BOOL                       (*IOCTL_ISDBT_SetDbgLevel)(DMD_ISDBT_DbgLv);
typedef DMD_ISDBT_Info*               (*IOCTL_ISDBT_GetInfo)(void);
typedef MS_BOOL                       (*IOCTL_ISDBT_GetLibVer)(const MSIF_Version**);
typedef MS_BOOL                       (*IOCTL_ISDBT_Init)(MS_U8, DMD_ISDBT_InitData*, MS_U32);
typedef MS_BOOL                       (*IOCTL_ISDBT_Exit)(MS_U8);
typedef MS_U32                        (*IOCTL_ISDBT_GetConfig)(MS_U8, DMD_ISDBT_InitData*);     
typedef MS_BOOL                       (*IOCTL_ISDBT_GetFwVer)(MS_U8, MS_U16*);
typedef MS_BOOL                       (*IOCTL_ISDBT_AdvSetConfig)(MS_U8, DMD_ISDBT_DEMOD_TYPE, MS_BOOL);
typedef MS_U32                        (*IOCTL_ISDBT_SetPowerState)(MS_U8, EN_POWER_MODE);
typedef DMD_ISDBT_LOCK_STATUS         (*IOCTL_ISDBT_GetLock)(MS_U8, DMD_ISDBT_GETLOCK_TYPE);
typedef MS_BOOL                       (*IOCTL_ISDBT_GetModulationMode)(MS_U8, EN_ISDBT_Layer, sISDBT_MODULATION_MODE*);
typedef MS_BOOL                       (*IOCTL_ISDBT_GetSignalStrength)(MS_U8, MS_U16*);
typedef MS_BOOL                       (*IOCTL_ISDBT_GetFreqOffset)(MS_U8, DMD_ISDBT_CFO_DATA*);
typedef MS_U16                        (*IOCTL_ISDBT_GetSignalQuality)(MS_U8);
typedef MS_U16                        (*IOCTL_ISDBT_GetSignalQualityOfLayerA)(MS_U8);
typedef MS_U16                        (*IOCTL_ISDBT_GetSignalQualityOfLayerB)(MS_U8);
typedef MS_U16                        (*IOCTL_ISDBT_GetSignalQualityOfLayerC)(MS_U8);
typedef MS_U16                        (*IOCTL_ISDBT_GetSignalQualityCombine)(MS_U8);
typedef MS_U8                         (*IOCTL_ISDBT_GetSNR)(MS_U8, DMD_ISDBT_SNR_DATA*);
typedef MS_BOOL                       (*IOCTL_ISDBT_Read_PKT_ERR)(MS_U8, EN_ISDBT_Layer, MS_U16*);
typedef MS_BOOL                       (*IOCTL_ISDBT_GetPreViterbiBer)(MS_U8, DMD_ISDBT_GET_BER_VALUE*);
typedef MS_BOOL                       (*IOCTL_ISDBT_GetPostViterbiBer)(MS_U8, DMD_ISDBT_GET_BER_VALUE*);
typedef MS_BOOL                       (*IOCTL_ISDBT_SetSerialControl)(MS_U8, MS_U8);
typedef MS_BOOL                       (*IOCTL_ISDBT_IIC_BYPASS_MODE)(MS_U8, MS_BOOL);
typedef MS_BOOL                       (*IOCTL_ISDBT_SWITCH_SSPI_GPIO)(MS_U8, MS_BOOL);
typedef MS_BOOL                       (*IOCTL_ISDBT_GPIO_GET_LEVEL)(MS_U8, MS_U8, MS_BOOL*);
typedef MS_BOOL                       (*IOCTL_ISDBT_GPIO_SET_LEVEL)(MS_U8, MS_U8, MS_BOOL);
typedef MS_BOOL                       (*IOCTL_ISDBT_GPIO_OUT_ENABLE)(MS_U8, MS_U8, MS_BOOL);
typedef MS_BOOL                       (*IOCTL_ISDBT_GetReg)(MS_U8, MS_U16, MS_U8*);
typedef MS_BOOL                       (*IOCTL_ISDBT_SetReg)(MS_U8, MS_U16, MS_U8);

typedef struct _ISDBT_INSTANT_PRIVATE
{
    IOCTL_ISDBT_SetDbgLevel                      fpSetDbgLevel;
    IOCTL_ISDBT_GetInfo                          fpGetInfo;
    IOCTL_ISDBT_GetLibVer                        fpGetLibVer;
    IOCTL_ISDBT_Init                             fpInit;
    IOCTL_ISDBT_Exit                             fpExit;
    IOCTL_ISDBT_GetConfig                        fpGetConfig;            
    IOCTL_ISDBT_GetFwVer                         fpGetFwVer;
    IOCTL_ISDBT_AdvSetConfig                     fpAdvSetConfig;
    IOCTL_ISDBT_SetPowerState                    fpSetPowerState;
    IOCTL_ISDBT_GetLock                          fpGetLock;
    IOCTL_ISDBT_GetModulationMode                fpGetModulationMode;
    IOCTL_ISDBT_GetSignalStrength                fpGetSignalStrength;
    IOCTL_ISDBT_GetFreqOffset                    fpGetFreqOffset;
    IOCTL_ISDBT_GetSignalQuality                 fpGetSignalQuality;
    IOCTL_ISDBT_GetSignalQualityOfLayerA         fpGetSignalQualityOfLayerA;
    IOCTL_ISDBT_GetSignalQualityOfLayerB         fpGetSignalQualityOfLayerB;
    IOCTL_ISDBT_GetSignalQualityOfLayerC         fpGetSignalQualityOfLayerC;
    IOCTL_ISDBT_GetSignalQualityCombine          fpGetSignalQualityCombine;
    IOCTL_ISDBT_GetSNR                           fpGetSNR;
    IOCTL_ISDBT_Read_PKT_ERR                     fpRead_PKT_ERR;
    IOCTL_ISDBT_GetPreViterbiBer                 fpGetPreViterbiBer;
    IOCTL_ISDBT_GetPostViterbiBer                fpGetPostViterbiBer;
    IOCTL_ISDBT_SetSerialControl                 fpSetSerialControl;
    IOCTL_ISDBT_IIC_BYPASS_MODE                  fpIIC_BYPASS_MODE;
    IOCTL_ISDBT_SWITCH_SSPI_GPIO                 fpSWITCH_SSPI_GPIO;
    IOCTL_ISDBT_GPIO_GET_LEVEL                   fpGPIO_GET_LEVEL;
    IOCTL_ISDBT_GPIO_SET_LEVEL                   fpGPIO_SET_LEVEL;
    IOCTL_ISDBT_GPIO_OUT_ENABLE                  fpGPIO_OUT_ENABLE;
    IOCTL_ISDBT_GetReg                           fpGetReg;
    IOCTL_ISDBT_SetReg                           fpSetReg;
} ISDBT_INSTANT_PRIVATE;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

extern DMD_ISDBT_ResData *psDMD_ISDBT_ResData;

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

MS_U32 ISDBTOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    ISDBT_INSTANT_PRIVATE *pIsdbtPri = NULL;
    void *pIsdbtPriVoid = NULL;
    
	DMD_DBG(printf("\n[ISDBT INFO] isdbt open"));
    
    // instance is allocated here, also can allocate private for internal use, ex, BDMA_INSTANT_PRIVATE
    UtopiaInstanceCreate(sizeof(ISDBT_INSTANT_PRIVATE), ppInstance);
    // setup func in private and assign the calling func in func ptr in instance private
    UtopiaInstanceGetPrivate(*ppInstance, &pIsdbtPriVoid);
    pIsdbtPri = (ISDBT_INSTANT_PRIVATE*)pIsdbtPriVoid;

    pIsdbtPri->fpSetDbgLevel                    = _MDrv_DMD_ISDBT_SetDbgLevel;
    pIsdbtPri->fpGetInfo                        = _MDrv_DMD_ISDBT_GetInfo;
    pIsdbtPri->fpGetLibVer                      = _MDrv_DMD_ISDBT_GetLibVer;
    pIsdbtPri->fpInit                           = _MDrv_DMD_ISDBT_MD_Init;
    pIsdbtPri->fpExit                           = _MDrv_DMD_ISDBT_MD_Exit;
    pIsdbtPri->fpGetConfig                      = _MDrv_DMD_ISDBT_MD_GetConfig;     
    pIsdbtPri->fpAdvSetConfig                   = _MDrv_DMD_ISDBT_MD_AdvSetConfig;
    pIsdbtPri->fpSetPowerState                  = _MDrv_DMD_ISDBT_MD_SetPowerState;    
    pIsdbtPri->fpGetLock                        = _MDrv_DMD_ISDBT_MD_GetLock;          
    pIsdbtPri->fpGetModulationMode              = _MDrv_DMD_ISDBT_MD_GetModulationMode;
    pIsdbtPri->fpGetSignalStrength              = _MDrv_DMD_ISDBT_MD_GetSignalStrength;
    pIsdbtPri->fpGetFreqOffset                  = _MDrv_DMD_ISDBT_MD_GetFreqOffset;
    pIsdbtPri->fpGetSignalQuality               = _MDrv_DMD_ISDBT_MD_GetSignalQuality;
    pIsdbtPri->fpGetSignalQualityOfLayerA       = _MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerA;
    pIsdbtPri->fpGetSignalQualityOfLayerB       = _MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerB;
    pIsdbtPri->fpGetSignalQualityOfLayerC       = _MDrv_DMD_ISDBT_MD_GetSignalQualityOfLayerC;
    pIsdbtPri->fpGetSignalQualityCombine        = _MDrv_DMD_ISDBT_MD_GetSignalQualityCombine;
    pIsdbtPri->fpGetSNR                         = _MDrv_DMD_ISDBT_MD_GetSNR;
    pIsdbtPri->fpRead_PKT_ERR                   = _MDrv_DMD_ISDBT_MD_Read_PKT_ERR;
    pIsdbtPri->fpGetPreViterbiBer               = _MDrv_DMD_ISDBT_MD_GetPreViterbiBer;   
    pIsdbtPri->fpGetPostViterbiBer              = _MDrv_DMD_ISDBT_MD_GetPostViterbiBer;  
    pIsdbtPri->fpSetSerialControl               = _MDrv_DMD_ISDBT_MD_SetSerialControl;
    pIsdbtPri->fpIIC_BYPASS_MODE                = _MDrv_DMD_ISDBT_MD_IIC_BYPASS_MODE;   
    pIsdbtPri->fpSWITCH_SSPI_GPIO               = _MDrv_DMD_ISDBT_MD_SWITCH_SSPI_GPIO; 
    pIsdbtPri->fpGPIO_GET_LEVEL                 = _MDrv_DMD_ISDBT_MD_GPIO_GET_LEVEL;        
    pIsdbtPri->fpGPIO_SET_LEVEL                 = _MDrv_DMD_ISDBT_MD_GPIO_SET_LEVEL;        
    pIsdbtPri->fpGPIO_OUT_ENABLE                = _MDrv_DMD_ISDBT_MD_GPIO_OUT_ENABLE;           
    pIsdbtPri->fpGetReg                         = _MDrv_DMD_ISDBT_MD_GetReg;           
    pIsdbtPri->fpSetReg                         = _MDrv_DMD_ISDBT_MD_SetReg;           

	  return UTOPIA_STATUS_SUCCESS;
}

#if DMD_ISDBT_STR_EN
MS_U32 ISDBTStr(MS_U32 u32PowerState, void* pModule)
{
    void* pResource = NULL;
	ISDBT_RESOURCE_PRIVATE* psISDBTResPri = NULL;

    MS_U32 i = 0;
	MS_U32 u32Ret = FALSE;

	if (UtopiaResourceObtain(pModule, ISDBT_POOL_ID_DMD0, &pResource) != 0)
    {
        printf("UtopiaResourceObtainToInstant fail\n");
	    return UTOPIA_STATUS_ERR_RESOURCE;
    }

    if(UtopiaResourceGetPrivate(pResource,(void*)&psISDBTResPri) != 0)
    {
        printf("UtopiaResourceGetPrivate fail\n");
        return UTOPIA_STATUS_FAIL;
    }

    psDMD_ISDBT_ResData = ((PISDBT_RESOURCE_PRIVATE)psISDBTResPri)->sDMD_ISDBT_ResData;

    do
    {
        u32Ret=_MDrv_DMD_ISDBT_MD_SetPowerState(i++, u32PowerState);
    } while (i < DMD_ISDBT_MAX_DEMOD_NUM && u32Ret);
    
    UtopiaResourceRelease(pResource);

    return (u32Ret ? UTOPIA_STATUS_SUCCESS : UTOPIA_STATUS_FAIL);

}
#endif

MS_U32 ISDBTIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
	UtopiaInstanceGetModule(pInstance, &pModule);
	void* pResource = NULL;
	ISDBT_RESOURCE_PRIVATE* psISDBTResPri = NULL;
    ISDBT_INSTANT_PRIVATE* psISDBTInstPri = NULL;
    void* psISDBTInstPriVoid = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void**)&psISDBTInstPriVoid);
    psISDBTInstPri = (ISDBT_INSTANT_PRIVATE*)psISDBTInstPriVoid;
    
    MS_U32 u32Ret = FALSE;
    
    if (UtopiaResourceObtain(pModule, ISDBT_POOL_ID_DMD0, &pResource) != 0)
    {
        DMD_DBG(printf("UtopiaResourceObtainToInstant fail\n"));
	    return UTOPIA_STATUS_ERR_RESOURCE;
    }
    
    if(UtopiaResourceGetPrivate(pResource,(void*)&psISDBTResPri) != 0)
    {
        printf("UtopiaResourceGetPrivate fail\n");
        return UTOPIA_STATUS_FAIL;
    }
    
    psDMD_ISDBT_ResData = ((PISDBT_RESOURCE_PRIVATE)psISDBTResPri)->sDMD_ISDBT_ResData;

    switch (u32Cmd)
    {
        case DMD_ISDBT_DRV_CMD_SetDbgLevel:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_SetDbgLevel\n"));
            u32Ret = psISDBTInstPri->fpSetDbgLevel(((PISDBT_DBG_LEVEL_PARAM)pArgs)->u8DbgLevel);
            break;
        case DMD_ISDBT_DRV_CMD_GetInfo:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_GetInfo\n"));
            ((PISDBT_GET_INFO_PARAM)pArgs)->pInfo = psISDBTInstPri->fpGetInfo();
            u32Ret = TRUE;
            break;
        case DMD_ISDBT_DRV_CMD_GetLibVer:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_GetLibVer\n"));
            u32Ret = psISDBTInstPri->fpGetLibVer(((PISDBT_GET_LIB_VER_PARAM)pArgs)->ppVersion);
            break;
        case DMD_ISDBT_DRV_CMD_Init:
            ((PISDBT_INIT_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_Init:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_Init\n"));
            u32Ret = psISDBTInstPri->fpInit(((PISDBT_INIT_PARAM)pArgs)->id, ((PISDBT_INIT_PARAM)pArgs)->pDMD_ISDBT_InitData, ((PISDBT_INIT_PARAM)pArgs)->u32InitDataLen);
            break;
        case DMD_ISDBT_DRV_CMD_Exit:
            ((PISDBT_ID_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_Exit:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_Exit\n"));
            u32Ret = psISDBTInstPri->fpExit(((PISDBT_ID_PARAM)pArgs)->id);
            break;
        case DMD_ISDBT_DRV_CMD_GetConfig:   
        	((PISDBT_INIT_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_GetConfig:    
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_GetConfig\n"));
            u32Ret = psISDBTInstPri->fpGetConfig(((PISDBT_INIT_PARAM)pArgs)->id, ((PISDBT_INIT_PARAM)pArgs)->pDMD_ISDBT_InitData);
	        break;
        case DMD_ISDBT_DRV_CMD_AdvSetConfig:
            ((PISDBT_SET_CONFIG_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_AdvSetConfig:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_SetConfig\n"));
            u32Ret = psISDBTInstPri->fpAdvSetConfig(((PISDBT_SET_CONFIG_PARAM)pArgs)->id, ((PISDBT_SET_CONFIG_PARAM)pArgs)->eType, ((PISDBT_SET_CONFIG_PARAM)pArgs)->bEnable);
            break;
        case DMD_ISDBT_DRV_CMD_SetPowerState:
            ((PISDBT_SET_POWER_STATE_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_SetPowerState:
            #if DMD_ISDBT_STR_EN
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_SetPowerState\n"));            
            if (psISDBTInstPri->fpSetPowerState(((PISDBT_SET_POWER_STATE_PARAM)pArgs)->id, ((PISDBT_SET_POWER_STATE_PARAM)pArgs)->u16PowerState == UTOPIA_STATUS_SUCCESS))
                u32Ret = TRUE;
            else
                u32Ret = FALSE;
            #else
            DMD_DBG(printf("ATSCIoctl - Don't support MDrv_DMD_ATSC_MD_SetPowerState\n"));
            #endif
            break;
        case DMD_ISDBT_DRV_CMD_GetLock:
            ((PISDBT_GET_LOCK_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_GetLock:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_GetLock\n"));
            ((PISDBT_GET_LOCK_PARAM)pArgs)->status = psISDBTInstPri->fpGetLock(((PISDBT_GET_LOCK_PARAM)pArgs)->id, ((PISDBT_GET_LOCK_PARAM)pArgs)->eType);
            u32Ret = TRUE;
            break;
        case DMD_ISDBT_DRV_CMD_GetModulationMode:
            ((PISDBT_GET_MODULATION_MODE_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_GetModulationMode:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_GetModulationMode\n"));
            u32Ret = psISDBTInstPri->fpGetModulationMode(((PISDBT_GET_MODULATION_MODE_PARAM)pArgs)->id, ((PISDBT_GET_MODULATION_MODE_PARAM)pArgs)->eLayerIndex, &(((PISDBT_GET_MODULATION_MODE_PARAM)pArgs)->IsdbtModulationMode));
            break;
        case DMD_ISDBT_DRV_CMD_GetSignalStrength:
            ((PISDBT_GET_SIGNAL_STRENGTH_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_GetSignalStrength:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_GetSignalStrength\n"));
            u32Ret = psISDBTInstPri->fpGetSignalStrength(((PISDBT_GET_SIGNAL_STRENGTH_PARAM)pArgs)->id, &(((PISDBT_GET_SIGNAL_STRENGTH_PARAM)pArgs)->u16Strength));
            break;
        case DMD_ISDBT_DRV_CMD_GetSignalQuality:
        case DMD_ISDBT_DRV_CMD_GetSignalQualityOfLayerA:
            ((PISDBT_GET_SIGNAL_QUALITY_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_GetSignalQuality:
        case DMD_ISDBT_DRV_CMD_MD_GetSignalQualityOfLayerA:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_GetSignalQualityA\n"));
            ((PISDBT_GET_SIGNAL_QUALITY_PARAM)pArgs)->u16SignalQuality = psISDBTInstPri->fpGetSignalQualityOfLayerA(((PISDBT_GET_SIGNAL_QUALITY_PARAM)pArgs)->id);
            u32Ret = TRUE;
            break;
        case DMD_ISDBT_DRV_CMD_GetSignalQualityOfLayerB:
            ((PISDBT_GET_SIGNAL_QUALITY_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_GetSignalQualityOfLayerB:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_GetSignalQualityB\n"));
            ((PISDBT_GET_SIGNAL_QUALITY_PARAM)pArgs)->u16SignalQuality = psISDBTInstPri->fpGetSignalQualityOfLayerB(((PISDBT_GET_SIGNAL_QUALITY_PARAM)pArgs)->id);
            u32Ret = TRUE;
            break;
        case DMD_ISDBT_DRV_CMD_GetSignalQualityOfLayerC:
            ((PISDBT_GET_SIGNAL_QUALITY_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_GetSignalQualityOfLayerC:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_GetSignalQualityC\n"));
            ((PISDBT_GET_SIGNAL_QUALITY_PARAM)pArgs)->u16SignalQuality = psISDBTInstPri->fpGetSignalQualityOfLayerC(((PISDBT_GET_SIGNAL_QUALITY_PARAM)pArgs)->id);
            u32Ret = TRUE;
            break;
        case DMD_ISDBT_DRV_CMD_GetSignalQualityCombine:
            ((PISDBT_GET_SIGNAL_QUALITY_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_GetSignalQualityCombine:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_GetSignalQualityCombine\n"));
            ((PISDBT_GET_SIGNAL_QUALITY_PARAM)pArgs)->u16SignalQuality = psISDBTInstPri->fpGetSignalQualityCombine(((PISDBT_GET_SIGNAL_QUALITY_PARAM)pArgs)->id);
            u32Ret = TRUE;
            break;           
        case DMD_ISDBT_DRV_CMD_GetSNR:
            ((PISDBT_GET_SNR_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_GetSNR:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_GetSNR\n"));
            u32Ret = psISDBTInstPri->fpGetSNR(((PISDBT_GET_SNR_PARAM)pArgs)->id, &(((PISDBT_GET_SNR_PARAM)pArgs)->snr));
            break;
        case DMD_ISDBT_DRV_CMD_Read_PKT_ERR:
            ((PISDBT_READ_PKT_ERR_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_Read_PKT_ERR:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_Read_uCPKT_ERR\n"));
            u32Ret = psISDBTInstPri->fpRead_PKT_ERR(((PISDBT_READ_PKT_ERR_PARAM)pArgs)->id, ((PISDBT_READ_PKT_ERR_PARAM)pArgs)->eIsdbtLayer, &(((PISDBT_READ_PKT_ERR_PARAM)pArgs)->u16PacketErr));
            break;
        case DMD_ISDBT_DRV_CMD_GetPreViterbiBer:
            ((PISDBT_GET_BER_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_GetPreViterbiBer:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_GetPreViterbiBer\n"));
            u32Ret = psISDBTInstPri->fpGetPreViterbiBer(((PISDBT_GET_BER_PARAM)pArgs)->id, &(((PISDBT_GET_BER_PARAM)pArgs)->ber));
            break;
        case DMD_ISDBT_DRV_CMD_GetPostViterbiBer:
            ((PISDBT_GET_BER_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_GetPostViterbiBer:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_GetPostViterbiBer\n"));
            u32Ret = psISDBTInstPri->fpGetPostViterbiBer(((PISDBT_GET_BER_PARAM)pArgs)->id, &(((PISDBT_GET_BER_PARAM)pArgs)->ber));
            break;
        case DMD_ISDBT_DRV_CMD_GetFreqOffset:
            ((PISDBT_GET_FREQ_OFFSET_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_GetFreqOffset:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_ReadFrequencyOffset\n"));
            u32Ret = psISDBTInstPri->fpGetFreqOffset(((PISDBT_GET_FREQ_OFFSET_PARAM)pArgs)->id, &((PISDBT_GET_FREQ_OFFSET_PARAM)pArgs)->cfo);
            break;
        case DMD_ISDBT_DRV_CMD_SetSerialControl:
            ((PISDBT_SET_SERIAL_CONTROL_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_SetSerialControl:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_SetSerialControl\n"));
            u32Ret = psISDBTInstPri->fpSetSerialControl(((PISDBT_SET_SERIAL_CONTROL_PARAM)pArgs)->id, ((PISDBT_SET_SERIAL_CONTROL_PARAM)pArgs)->u8TsConfigData);
            break;
        case DMD_ISDBT_DRV_CMD_IIC_BYPASS_MODE:
            ((PISDBT_IIC_BYPASS_MODE_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_IIC_BYPASS_MODE:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_IIC_BYPASS_MODE\n"));
            u32Ret = psISDBTInstPri->fpIIC_BYPASS_MODE(((PISDBT_IIC_BYPASS_MODE_PARAM)pArgs)->id, ((PISDBT_IIC_BYPASS_MODE_PARAM)pArgs)->bEnable);
            break;
        case DMD_ISDBT_DRV_CMD_SWITCH_SSPI_GPIO:
            ((PISDBT_SWITCH_SSPI_GPIO_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_SWITCH_SSPI_GPIO:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_SWITCH_SSPI_GPIO\n"));
            u32Ret = psISDBTInstPri->fpSWITCH_SSPI_GPIO(((PISDBT_SWITCH_SSPI_GPIO_PARAM)pArgs)->id, ((PISDBT_SWITCH_SSPI_GPIO_PARAM)pArgs)->bEnable);
            break;
        case DMD_ISDBT_DRV_CMD_GPIO_GET_LEVEL:
            ((PISDBT_GPIO_LEVEL_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_GPIO_GET_LEVEL:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_GPIO_GET_LEVEL\n"));
            u32Ret = psISDBTInstPri->fpGPIO_GET_LEVEL(((PISDBT_GPIO_LEVEL_PARAM)pArgs)->id, ((PISDBT_GPIO_LEVEL_PARAM)pArgs)->u8Pin, &(((PISDBT_GPIO_LEVEL_PARAM)pArgs)->bLevel));
            break;
        case DMD_ISDBT_DRV_CMD_GPIO_SET_LEVEL:
            ((PISDBT_GPIO_LEVEL_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_GPIO_SET_LEVEL:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_GPIO_SET_LEVEL\n"));
            u32Ret = psISDBTInstPri->fpGPIO_SET_LEVEL(((PISDBT_GPIO_LEVEL_PARAM)pArgs)->id, ((PISDBT_GPIO_LEVEL_PARAM)pArgs)->u8Pin, ((PISDBT_GPIO_LEVEL_PARAM)pArgs)->bLevel);
            break;
        case DMD_ISDBT_DRV_CMD_GPIO_OUT_ENABLE:
            ((PISDBT_GPIO_OUT_ENABLE_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_GPIO_OUT_ENABLE:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_GPIO_OUT_ENABLE\n"));
            u32Ret = psISDBTInstPri->fpGPIO_OUT_ENABLE(((PISDBT_GPIO_OUT_ENABLE_PARAM)pArgs)->id, ((PISDBT_GPIO_OUT_ENABLE_PARAM)pArgs)->u8Pin, ((PISDBT_GPIO_OUT_ENABLE_PARAM)pArgs)->bEnableOut);
            break;
        case DMD_ISDBT_DRV_CMD_GetReg:
            ((PISDBT_REG_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_GetReg:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_GetReg\n"));
            u32Ret = psISDBTInstPri->fpGetReg(((PISDBT_REG_PARAM)pArgs)->id, ((PISDBT_REG_PARAM)pArgs)->u16Addr, &(((PISDBT_REG_PARAM)pArgs)->u8Data));
            break;
        case DMD_ISDBT_DRV_CMD_SetReg:
            ((PISDBT_REG_PARAM)pArgs)->id = 0; 
        case DMD_ISDBT_DRV_CMD_MD_SetReg:
            DMD_DBG(printf("ISDBTIoctl - MDrv_DMD_ISDBT_MD_SetReg\n"));
            u32Ret = psISDBTInstPri->fpSetReg(((PISDBT_REG_PARAM)pArgs)->id, ((PISDBT_REG_PARAM)pArgs)->u16Addr, ((PISDBT_REG_PARAM)pArgs)->u8Data);
            break;
        default:
            break;
    }
    
    UtopiaResourceRelease(pResource);
            
    return (u32Ret ? UTOPIA_STATUS_SUCCESS : UTOPIA_STATUS_FAIL);
}

MS_U32 ISDBTClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);

	return UTOPIA_STATUS_SUCCESS;
}

void ISDBTRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    void* pUtopiaModule = NULL;
	UtopiaModuleCreate(MODULE_ISDBT, 8, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)ISDBTOpen, (FUtopiaClose)ISDBTClose, (FUtopiaIOctl)ISDBTIoctl);
    // register func for module, after register here, then ap call UtopiaSTR can call to registered standard func
    #if DMD_ISDBT_STR_EN
    UtopiaModuleSetupSTRFunctionPtr(pUtopiaModule,(FUtopiaSTR)_MDrv_DMD_ISDBT_MD_SetPowerState);
    #endif
    
    // 2. deal with resource
    void* psResource = NULL;
    // start func to add res, call once will create 2 access in resource. Also can declare BDMA_POOL_ID_BDMA1 for another channel depend on driver owner.
	UtopiaModuleAddResourceStart(pUtopiaModule, ISDBT_POOL_ID_DMD0);
    // resource can alloc private for internal use, ex, BDMA_RESOURCE_PRIVATE
    UtopiaResourceCreate("ISDBT", sizeof(ISDBT_RESOURCE_PRIVATE), &psResource);
    // func to reg res
    UtopiaResourceRegister(pUtopiaModule, psResource, ISDBT_POOL_ID_DMD0);
    // end function to add res
	UtopiaModuleAddResourceEnd(pUtopiaModule, ISDBT_POOL_ID_DMD0);
}

#endif // #ifdef UTPA2