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
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// file drvPWS.c
/// @author MStar Semiconductor Inc.
/// @brief power saving driver
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// Header Files
//------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#if defined (MSOS_TYPE_LINUX)
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#endif

#include "MsCommon.h"
#include "MsVersion.h"
#include "UFO.h"
#include "drvPWS.h"
#include "halPWS.h"
#include "drvMMIO.h"
#include "utopia_dapi.h"
#include "MsOS.h"
#include "utopia.h"

#include "ULog.h"

//------------------------------------------------------------------------------
// Local defines & local structures
//------------------------------------------------------------------------------
#define TAG_PWS "PWS"

#define PWS_DBG_FUNC()             if (_pwsDbgLevel >= E_PWS_DBGLV_ALL) \
                                        {ULOGD(TAG_PWS, "\t====   %s   ====\n", __FUNCTION__);}
#define PWS_DBG_INFO(x, args...)   if (_pwsDbgLevel >= E_PWS_DBGLV_INFO ) \
                                        {ULOGI(TAG_PWS, x, ##args);}
#define PWS_DBG_ERR(x, args...)    if (_pwsDbgLevel >= E_PWS_DBGLV_ERR_ONLY) \
                                        {ULOGE(TAG_PWS, x, ##args);}
#define DEBUG_INFO(y)   //y

//------------------------------------------------------------------------------
// External funciton
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Local & Global Variables
//------------------------------------------------------------------------------
//define mutex
static MS_S32 g_s32PWSMutex=-1;
#define PWS_MUTEX_CREATE()          g_s32PWSMutex = MsOS_CreateMutex(E_MSOS_FIFO, (char*)"PWSMUTEX" , MSOS_PROCESS_SHARED)
#define PWS_MUTEX_LOCK()            MsOS_ObtainMutex(g_s32PWSMutex,MSOS_WAIT_FOREVER)
#define PWS_MUTEX_UNLOCK()          MsOS_ReleaseMutex(g_s32PWSMutex)
#define PWS_MUTEX_DELETE()          MsOS_DeleteMutex(g_s32PWSMutex)

static MS_BOOL PWS_Init = 0;
static E_PWS_Use PWS_UseStatus = E_PWS_API_UNUSED;
static MSIF_Version _api_pws_version = {
    .DDI = { PWS_API_VERSION },
};
static E_PWS_DBG_LEVEL _pwsDbgLevel = E_PWS_DBGLV_ERR_ONLY;
static E_PWS_VIF_type pws_vif_init_type = E_PWS_VIF_NO_SAW;
//------------------------------------------------------------------------------
// Local Function
//------------------------------------------------------------------------------
MS_U8 MDrv_PWS_SetIOMapBase(void)
{
    MS_VIRT u32BaseAddr = 0;
    MS_PHY  u32BaseSize = 0;

    DEBUG_INFO(ULOGI(TAG_PWS, "%s\n",__FUNCTION__));

    if(!MDrv_MMIO_GetBASE(&u32BaseAddr, &u32BaseSize, MS_MODULE_PWS))
    {
        DEBUG_INFO(ULOGE(TAG_PWS, "Get IOMAP Base faill!\n"));
        return 0;
    }
    DEBUG_INFO(ULOGI(TAG_PWS, "Get IOMAP ID:%u Base:%lx!\n", MS_MODULE_PWS, u32BaseAddr));
    HAL_PWS_SetIOMapBase(u32BaseAddr);
    return 1;
}

//------------------------------------------------------------------------------
// Global Function
//------------------------------------------------------------------------------
E_PWS_Result MDrv_PWS_GetLibVer(const MSIF_Version **ppVersion)
{
    DEBUG_INFO(ULOGI(TAG_PWS, "%s\n",__FUNCTION__));
    if(!ppVersion)
        return PWS_FAIL;

    *ppVersion = &_api_pws_version;
    return PWS_SUCCESS;
}

E_PWS_Result MDrv_PWS_SetDbgLevel(E_PWS_DBG_LEVEL eLEVEL)
{
    DEBUG_INFO(ULOGI(TAG_PWS, "%s\n",__FUNCTION__));

    _pwsDbgLevel = eLEVEL;
    return PWS_SUCCESS;
}

void MDrv_PWS_GetInfo(PWS_Info *pInfo)
{
    DEBUG_INFO(ULOGI(TAG_PWS, "%s\n",__FUNCTION__));
    pInfo->SourceList = HAL_PWS_GetSourceInfo();
    pInfo->u32IOMap = HAL_PWS_GetIOMapBase();
}

void MDrv_PWS_GetStatus(PWS_Status *pStatus)
{
    DEBUG_INFO(ULOGI(TAG_PWS, "%s\n",__FUNCTION__));
    pStatus->bInit = PWS_Init;
    pStatus->u8DbgLevel = _pwsDbgLevel;
}

E_PWS_SouceInfo MDrv_PWS_GetSourceInfo(void)
{
    DEBUG_INFO(ULOGI(TAG_PWS, "%s\n",__FUNCTION__));
    return HAL_PWS_GetSourceInfo();
}

MS_U16 MDrv_PWS_Read2Byte(MS_U32 u32RegAddr)
{
    DEBUG_INFO(ULOGI(TAG_PWS, "%s\n",__FUNCTION__));
    return HAL_PWS_Read2Byte(u32RegAddr);
}

E_PWS_Result MDrv_PWS_Init(E_PWS_VIF_type eVifType)
{
    E_PWS_Result ret = PWS_FAIL;
    pws_vif_init_type = eVifType;
    DEBUG_INFO(ULOGI(TAG_PWS, "%s\n",__FUNCTION__));

    if(!PWS_Init)
    {
        if (MDrv_PWS_SetIOMapBase() != TRUE)
        {
            DEBUG_INFO(ULOGE(TAG_PWS, "Get IO base fail\n"));
            return PWS_FAIL;
        }

        HAL_PWS_Init(eVifType);
        ret = PWS_SUCCESS;
    }

    #if ((defined(UFO_XC_DAC_POWERCONTROL)) && (!defined(CONFIG_MBOOT)))
    HAL_PWS_MonitorPowerProc();
    #endif

    PWS_Init = 1;
    return ret;
}

E_PWS_Result MDrv_PWS_IpPowerControl(E_PWS_IP_name eIpName, E_PWS_IpPowerCtrl pwr_type)
{
    if (eIpName > E_PWS_IP_MAX || pwr_type == E_PWS_IP_ON)
    {
        return PWS_FAIL;
    }

    MDrv_PWS_Init(pws_vif_init_type);

    return HAL_PWS_IpPowerControl(eIpName, pwr_type);;
}

E_PWS_Result MDrv_PWS_ScenePowerControl(E_PWS_SCENE_name eSceneName, E_PWS_IpPowerCtrl pwr_type)
{
#if ((defined(UFO_XC_DAC_POWERCONTROL)) && (!defined(CONFIG_MBOOT)))

    MDrv_PWS_Init(pws_vif_init_type);

    return HAL_PWS_ScenePowerControl(eSceneName, pwr_type);
#else
    return PWS_SUCCESS;
#endif
}

E_PWS_Result MDrv_PWS_HandleSource(E_PWS_Operation operation, E_PWS_SouceInfo source, E_PWS_Sync sync_type)
{
    E_PWS_Result ret = PWS_FAIL;

    DEBUG_INFO(ULOGI(TAG_PWS, "%s\n",__FUNCTION__));
    if(PWS_Init == 0)
        return ret;
    if(operation == PWS_INVAILD_OP)
        return ret;
    if((source == _NO_SOURCE_) || (source == _UNKNOWN_))
        return ret;
    if(PWS_UseStatus == E_PWS_USE_HANDLE_IP)
        return ret;

    PWS_UseStatus = E_PWS_USE_HANDLE_SOURCE;
    sync_type = sync_type;
    ret = HAL_PWS_HandleSource(operation,source);

    return ret;
}

E_PWS_Result MDrv_PWS_HandleIP(E_PWS_IpPowerCtrl IpPowerCtrl,const unsigned char *RegName)
{
    E_PWS_Result ret = PWS_FAIL;

    DEBUG_INFO(ULOGI(TAG_PWS, "%s\n",__FUNCTION__));
    if(PWS_Init == 0)
        return ret;
    if(PWS_UseStatus == E_PWS_USE_HANDLE_SOURCE)
        return ret;

    PWS_UseStatus = E_PWS_USE_HANDLE_IP;
    ret = HAL_PWS_HandleIP(IpPowerCtrl,RegName);

    return ret;
}

E_PWS_Result MDrv_PWS_RegisterCallback(MS_U8 index, E_PWS_CallbackCtrl cb_ctrl, unsigned char *RegName, P_PWS_Callback pfCallback)
{
    E_PWS_Result ret = PWS_FAIL;

    DEBUG_INFO(ULOGI(TAG_PWS, "%s\n",__FUNCTION__));
    if(index < PWS_Register_CB_size)
    {
        ret = HAL_PWS_RegisterCallback(index,cb_ctrl,RegName,pfCallback);
    }
    else
    {
        ret = PWS_FAIL;
    }

    return ret;
}

E_PWS_Result MDrv_PWS_CancelCallback(MS_U8 index)
{
    E_PWS_Result ret = PWS_FAIL;

    DEBUG_INFO(ULOGI(TAG_PWS, "%s\n",__FUNCTION__));
    if(index < PWS_Register_CB_size)
    {
        ret = HAL_PWS_CancelCallback(index);
    }
    else
    {
        ret = PWS_FAIL;
    }

    return ret;
}

#if defined (MSOS_TYPE_LINUX)
E_PWS_Result MDrv_PWS_PHYPower(E_PWS_PHY bSetFlag)
{
    struct ifreq ifr;
    int sockfd;
    const char s[] = "eth0" ;
	MS_U32 ret = PWS_SUCCESS;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
    {
        DEBUG_INFO(ULOGE(TAG_PWS, "sockfd is negative %s\n",__FUNCTION__));
        return PWS_FAIL;
    }
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, s, 4);

    if(bSetFlag == PHY_OFF)
    {
        if(ioctl(sockfd, SIOCDEVOFF, &ifr))
        {
            DEBUG_INFO(ULOGE(TAG_PWS, "IOCTL FAIL %s\n",__FUNCTION__));
            close(sockfd);
            return PWS_FAIL;
        }
    }
    else
    {
        if(ioctl(sockfd, SIOCDEVON, &ifr))
        {
            DEBUG_INFO(ULOGE(TAG_PWS, "IOCTL FAIL %s\n",__FUNCTION__));
            close(sockfd);
            return PWS_FAIL;
        }
    }

	close(sockfd);
    return ret;
}
#endif

MS_U32 MDrv_PWS_SetPowerState(EN_POWER_MODE u16PowerState)
{
    static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if(u16PowerState == E_POWER_SUSPEND)
    {
        _prev_u16PowerState = u16PowerState;
        PWS_Init = false; //set PWS init state to false
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;

    }
    else if(u16PowerState == E_POWER_RESUME)
    {
        if(_prev_u16PowerState == E_POWER_SUSPEND)
        {
            MDrv_PWS_Init(pws_vif_init_type);
            _prev_u16PowerState = u16PowerState;
            u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
        }
        else
        {
            ULOGE(TAG_PWS, "[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
            u32Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
        }
        return u32Return;
    }
    else
    {
        ULOGE(TAG_PWS, "[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        u32Return = UTOPIA_STATUS_FAIL;
    }

    return u32Return;// for success
}


enum
{
    PWS_POOL_ID_PWS0=0
} ePWS_PoolID;

//--------------------------------------------------------------------------------------------------
// Utopia2.0 will call this function to register PWS module
//--------------------------------------------------------------------------------------------------
void PWSRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. create a module(module_name, SHM_size), and register to utopia2.0
    void* pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_PWS, 0, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);

    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)PWSOpen, (FUtopiaClose)PWSClose, (FUtopiaIOctl)PWSIoctl);

    // 2. Resource register
    void* psResource = NULL;

    // start func to add resources of a certain Resource_Pool
    UtopiaModuleAddResourceStart(pUtopiaModule, PWS_POOL_ID_PWS0);

    // create a resouce and regiter it to a certain Resource_Pool, resource can alloc private for internal use
    UtopiaResourceCreate("pws0", sizeof(PWS_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, PWS_POOL_ID_PWS0);

    //UtopiaResourceCreate("pws1", sizeof(PWS_RESOURCE_PRIVATE), &psResource);
    //UtopiaResourceRegister(pUtopiaModule, psResource, PWS_POOL_ID_PWS0);

    // end func to add resources of a certain Resource_Pool(this will set the ResourceSemaphore of this ResourcePool)
    UtopiaModuleAddResourceEnd(pUtopiaModule, PWS_POOL_ID_PWS0);

    //MDrv_PWS_Init(E_PWS_VIF_DUAL_SAW);
    ULOGD(TAG_PWS, "\033[35mFunction = %s, Line = %d, PWS Register finished\033[m\n", __PRETTY_FUNCTION__, __LINE__);
}

//--------------------------------------------------------------------------------------------------
// Utopia2.0 will call this function to get a instance to use PWS
// @ \b in: 32ModuleVersion => this is for checking if API version is the same
//--------------------------------------------------------------------------------------------------
MS_U32 PWSOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    MS_VIRT u32RetAddr;
    MS_U32 u32RetShmID, u32RetShmSize;
    MS_U32 u32Ret, u32RetTmp;

    //Check DRV_REVISION
    if(u32ModuleVersion != PWS_VERSION)
    {
        //printf("\033[35mFunction = %s, Line = %d, ERROR, DRV_VERSION is not PWS_VERSION\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        //return FALSE;
    }

    // for multi-process safe, check if already other Instance exist
    PWS_MUTEX_LOCK();
    u32Ret = MsOS_SHM_GetId((MS_U8*)"PWS_Instance", sizeof(MS_U32), &u32RetShmID, &u32RetAddr, &u32RetShmSize, MSOS_SHM_QUERY);
    if(u32Ret == 0)
    {
        // first create, set value as 0
        u32RetTmp = MsOS_SHM_GetId((MS_U8*)"PWS_Instance", sizeof(MS_U32), &u32RetShmID, &u32RetAddr, &u32RetShmSize, MSOS_SHM_CREATE);

        if (u32RetTmp == 0)
        {
            ULOGE(TAG_PWS, "[%s error] something wrong in MsOS_SHM_GetId\n", __FUNCTION__);
            ULOGE(TAG_PWS, "is SHM_SIZE reached?\n");
            ULOGE(TAG_PWS, "is MAX_SHM_CLIENT_NUM reached?\n");

            ULOGE(TAG_PWS, "Edit while(1) to return for Coverity Warning;");
            return FALSE;
        }

        *(MS_U32 *)u32RetAddr = 0;
    }

    if(*(MS_U32 *)u32RetAddr == 1)
    {
        ULOGE(TAG_PWS, "\033[35mFunction = %s, Line = %d, [Multi-process Safe] A Instance is existed!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        PWS_MUTEX_UNLOCK();
        return FALSE;
    }
    else
    {
        *(MS_U32 *)u32RetAddr = 1;
        PWS_MUTEX_UNLOCK();
    }

    PWS_INSTANT_PRIVATE *pPWSPri = NULL;
    ULOGD(TAG_PWS, "\033[35mFunction = %s, Line = %d, [PWS INFO] OPEN INSTANCE...\033[m\n", __PRETTY_FUNCTION__, __LINE__); // joe.liu

    // instance is allocated here, also can allocate private for internal use, ex, PWS_INSTANT_PRIVATE, ppInstance will point to a pointer of the created UTOPIA_INSTANCE
    UtopiaInstanceCreate(sizeof(PWS_INSTANT_PRIVATE), ppInstance);
    // set the pPWSPri point to the private of UTOPIA_INSTANCE
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pPWSPri);

    // setup func in private and assign the calling func in func ptr in instance private
    pPWSPri->fpPWSGetLibVer = (IOCTL_PWS_GETLIBVER)MDrv_PWS_GetLibVer;
    pPWSPri->fpPWSSetDbgLevel = (IOCTL_PWS_SETDBGLEVEL)MDrv_PWS_SetDbgLevel;
    pPWSPri->fpPWSHandleSource = (IOCTL_PWS_HANDLESOURCE)MDrv_PWS_HandleSource;
#if defined (MSOS_TYPE_LINUX)
    pPWSPri->fpPWSPHYPower = (IOCTL_PWS_PHYPOWER)MDrv_PWS_PHYPower;
#endif

    return TRUE;
}

MS_U32 PWSIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    void* pResource = NULL;

    PWS_INSTANT_PRIVATE* psPWSInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psPWSInstPri);

    PPWS_GETLIBVER_PARAM pGetLibVerParam = NULL;
    PPWS_SETDBGLEVEL_PARAM pSetDbgLevelParam = NULL;
    PPWS_HANDLESOURCE_PARAM pHandleSourceParam = NULL;
#if defined (MSOS_TYPE_LINUX)
    PPWS_PHYPOWER_PARAM pPhyPowerParm = NULL;
#endif

    MS_U32 u32Ret;

    switch(u32Cmd)
    {
        case MDrv_CMD_PWS_GetLibVer:
            ULOGD(TAG_PWS, "PWSIoctl - MDrv_CMD_PWS_GetLibVer\n");
            pGetLibVerParam = (PPWS_GETLIBVER_PARAM)pArgs;
            // only print, do not need to get resource
            u32Ret = psPWSInstPri->fpPWSGetLibVer(pGetLibVerParam->ppVersion);
            return u32Ret;
        case MDrv_CMD_PWS_SetDbgLevel:
            ULOGD(TAG_PWS,"PWSIoctl - MDrv_CMD_PWS_SetDbgLevel\n");
            pSetDbgLevelParam = (PPWS_SETDBGLEVEL_PARAM)pArgs;
            // to check if enough resource?
            if(UtopiaResourceObtain(pModule, PWS_POOL_ID_PWS0, &pResource) != 0)
            {
                ULOGE(TAG_PWS,"UtopiaResourceObtainToInstant fail\n");
                return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psPWSInstPri->fpPWSSetDbgLevel(pSetDbgLevelParam->DbgLevel);
            UtopiaResourceRelease(pResource);
            return u32Ret;
        case MDrv_CMD_PWS_HandleSource:
            ULOGD(TAG_PWS,"PWSIoctl - MDrv_CMD_PWS_HandleSource\n");
            pHandleSourceParam = (PPWS_HANDLESOURCE_PARAM)pArgs;
            // to check if enough resource?
            if(UtopiaResourceObtain(pModule, PWS_POOL_ID_PWS0, &pResource) != 0)
            {
                ULOGE(TAG_PWS,"UtopiaResourceObtainToInstant fail\n");
                return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psPWSInstPri->fpPWSHandleSource(pHandleSourceParam->operation, pHandleSourceParam->source, pHandleSourceParam->sync_type);
            UtopiaResourceRelease(pResource);
            return u32Ret;
#if defined (MSOS_TYPE_LINUX)
        case MDrv_CMD_PWS_PHYPower:
            ULOGD(TAG_PWS,"PWSIoctl - MDrv_CMD_PWS_PHYPower\n");
            pPhyPowerParm = (PPWS_PHYPOWER_PARAM)pArgs;
            // to check if enough resource?
            if(UtopiaResourceObtain(pModule, PWS_POOL_ID_PWS0, &pResource) != 0)
            {
                ULOGE(TAG_PWS,"UtopiaResourceObtainToInstant fail\n");
                return 0xFFFFFFFF;
            }
            u32Ret = (MS_U32)psPWSInstPri->fpPWSPHYPower(pPhyPowerParm->bSetFlag);
            UtopiaResourceRelease(pResource);
            return u32Ret;
#endif
        default:
            break;
    };

	return 0; // FIXME: error code
}

MS_U32 PWSClose(void* pInstance)
{
    MS_VIRT u32RetAddr;
    MS_U32 u32RetShmID, u32RetShmSize;
    MS_U32 u32Ret;

	u32RetAddr = 0 ; //fix coverity isue
    UtopiaInstanceDelete(pInstance);

    u32Ret = MsOS_SHM_GetId((MS_U8*)"PWS_Instance", sizeof(MS_U32), &u32RetShmID, &u32RetAddr, &u32RetShmSize, MSOS_SHM_QUERY);

    if( (u32Ret == 0) || (*(MS_U32 *)u32RetAddr == 0) )
    {
        ULOGE(TAG_PWS,"\033[35mFunction = %s, Line = %d, [Multi-process Safe] u32IsInstanceCreated value is incorrect, which is %u!!\033[m\n", __PRETTY_FUNCTION__, __LINE__, *(unsigned int *)u32RetAddr);
        return FALSE;
    }
    else
    {
        ULOGD(TAG_PWS,"\033[35mFunction = %s, Line = %d, [Multi-process Safe] Release a Instance!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        PWS_MUTEX_LOCK();
        *(MS_U32 *)u32RetAddr = 0;
        PWS_MUTEX_UNLOCK();
        return TRUE;
    }
}


