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
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvSYS.c
/// @brief  System Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#if defined (MSOS_TYPE_LINUX)
#include <errno.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#include "MsCommon.h"
#include "MsVersion.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <string.h>
#endif
#include "regCHIP.h"
#include "halCHIP.h"
#include "drvSYS.h"
#include "drvMMIO.h"
#include "halSYS.h"

#include "tee_client_api.h"
#include "tee_ioc.h"
//#include "teec_trace.h"
#include "utopia_dapi.h"
#include "drvSYS_priv.h"
#include "utopia.h"

#include "label.h"


#include "drvMIU.h"
#include "drvMBX.h"
#include "apiMBX.h"
#include "drvBDMA.h"
#include "drvSEM.h"
#include "ULog.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#ifndef MS_NOSAPI
#define SYS_DIS_ALL_INT(x)          { x = MsOS_DisableAllInterrupts(); }
#define SYS_RES_ALL_INT(x)          { MsOS_RestoreAllInterrupts(x); }
#else   // MS_NOSAPI
#define SYS_DIS_ALL_INT(x)          { }
#define SYS_RES_ALL_INT(x)          { }
#endif  // MS_NOSAPI

#define TAG_SYS "SYS"

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MSIF_Version                 _sysVersion =
                                    {
                                        .DDI = { SYS_DRV_VERSION, },
                                    };

static SYS_Status                   _sysStatus =
                                    {
                                        .WDT_Active = FALSE,
                                    };

const char* _Change = "$Change$";
const char* _Revision = "$Revision$";
static MS_U8            u8IsSysInit = 0;

#ifdef CONFIG_MSTAR_DVFS_ENABLE
MS_U32          g_bDvfsInitFlag = 0;
#endif

SYS_IO_PROC     g_SysIoProc =
                {
                    .SysSwI2CReadBytes = 0,
                    .SysSwI2CWriteBytes = 0,
                };

static MS_U8    u8IsGlobalInit = 0;
static MS_S32   s32SysMutexId = -1;
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#define SYS_DBG_FUNC()             if (_gu8DbgLevel >= E_SYS_DBGLV_ALL) \
                                        {MS_DEBUG_MSG(ULOGD(TAG_SYS, "\t====   %s   ====\n", __FUNCTION__));}
#define SYS_DBG_INFO(x, args...)   if (_gu8DbgLevel >= E_SYS_DBGLV_INFO ) \
                                        {MS_DEBUG_MSG(ULOGI(TAG_SYS, x, ##args));}
#define SYS_DBG_ERR(x, args...)    if (_gu8DbgLevel >= E_SYS_DBGLV_ERR_ONLY) \
                                        {MS_DEBUG_MSG(ULOGE(TAG_SYS, x, ##args));}
#if defined (MSOS_TYPE_LINUX)

//prevetn MDrv_SYS_GlobalInit called twice
static pthread_mutex_t teec_mutex = PTHREAD_MUTEX_INITIALIZER;

#ifdef MS_DEBUG
#include "string.h"
#define PTH_RET_CHK(_pf_) \
    ({ \
        int r = _pf_; \
        if ((r != 0) && (r != ETIMEDOUT)) \
            fprintf(stderr, "[PTHREAD] %s: %d: %s: %s\n", __FILE__, __LINE__, #_pf_, strerror(r)); \
        r; \
    })
#else
#define PTH_RET_CHK(_pf_) _pf_
#endif
#endif

static MS_U8            _gu8DbgLevel = E_SYS_DBGLV_ERR_ONLY;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
E_SYS_ReturnValue  _MDrv_SYS_GetXcByPartConfg (MS_U32 u32SourceType, SYS_XC_BYPART_TIMING_t *stInfo, MS_BOOL *retEn);
E_SYS_ReturnValue  _MDrv_SYS_XcInputMapping (MS_U32 u32SourceType, E_SYS_SOURCE_TYPE *retSrcType);
E_SYS_ReturnValue  _MDrv_SYS_XcTimingMapping (MS_U32 u32Width, MS_U32 u32Height, MS_U32 u32FrameRate, E_SYS_INPUT_TIMING *retTimingType);

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void* pInstantSys = NULL;
void* pAttributeSys = NULL;

//-------------------------------------------------------------------------------------------------
/// System initialzation
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SYS_Init(void)
{
    MS_PHY u32size;
    MS_VIRT u32addr;

    if (!u8IsSysInit)
    {
        if(UtopiaOpen(MODULE_SYS, &pInstantSys, 0, pAttributeSys) !=  UTOPIA_STATUS_SUCCESS)
        {
            ULOGE(TAG_SYS, "Open SYS fail\n");
            return FALSE;
        }

        if(!MDrv_MMIO_GetBASE(&u32addr, &u32size, MS_MODULE_PM))
        {
            SYS_DBG_INFO("%s Get IOMAP Base faill!\n",__FUNCTION__);
            return 0;
        }
        else
        {
            SYS_Init(u32addr);
        }

        //
        // Init for efuse usage
        //
        if(!MDrv_MMIO_GetBASE(&u32addr, &u32size, MS_MODULE_EFUSE))
        {
            // efuse not implemented
        }
        else
        {
            HAL_SYS_SetEfuseIOMapBase(u32addr);
        }


        if(!MDrv_MMIO_GetBASE(&u32addr, &u32size, MS_MODULE_OTP2))
        {
            // efuse not implemented
        }
        else
        {
            HAL_SYS_SetOtpIOMapBase(u32addr);
        }
        u8IsSysInit = 1;
    }


    return TRUE;
}
#if defined(MSOS_TYPE_LINUX_KERNEL)
EXPORT_SYMBOL(MDrv_SYS_Init);
#endif


#ifdef MSOS_TYPE_LINUX_KERNEL
MS_BOOL MDrv_SYS_GlobalInit(void)
{
    MS_BOOL bRet = TRUE;
    /* Create mutex. */
    if ((s32SysMutexId == -1) &&
        ((s32SysMutexId = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex_drvSYS", MSOS_PROCESS_SHARED)) < 0))
    {
        return FALSE;
    }


    MsOS_ObtainMutex(s32SysMutexId, MSOS_WAIT_FOREVER);
    /* Check init. */
    if (u8IsGlobalInit)
    {
        goto MDrv_SYS_GlobalInit_End;
    }

    /* Init. */
    if (!MDrv_MMIO_Init()   || !MsOS_Init()     ||
        !MsOS_MPool_Init()  || !MDrv_SEM_Init() ||
        !MDrv_MIU_Init()    || !MDrv_SYS_Init())
    {
        bRet = FALSE;
        goto MDrv_SYS_GlobalInit_End;
    }

#if defined(ANDROID)
    MApi_MBX_Init(E_MBX_CPU_MIPS, E_MBX_ROLE_HK, 5000);
    MApi_MBX_Enable(TRUE);
#endif

    /* Init done. */
    u8IsGlobalInit = 1;

MDrv_SYS_GlobalInit_End:
    MsOS_ReleaseMutex(s32SysMutexId);
    return bRet;
}
#else

#if defined (MSOS_TYPE_LINUX)
static void teec_mutex_lock(pthread_mutex_t *mu)
{
	int e = pthread_mutex_lock(mu);

	if (e != 0)
		printf("pthread_mutex_lock failed: %d\n", e);
}

static void teec_mutex_unlock(pthread_mutex_t *mu)
{
	int e = pthread_mutex_unlock(mu);

	if (e != 0)
		printf("pthread_mutex_unlock failed: %d\n", e);
}

static void teec_resetTeeCmd(struct tee_cmd_io *cmd)
{
	cmd->fd_sess	= -1;
	cmd->cmd	= 0;
	cmd->uuid	= NULL;
	cmd->origin	= TEEC_ORIGIN_API;
	cmd->err	= TEEC_SUCCESS;
	cmd->data	= NULL;
	cmd->data_size	= 0;
	cmd->op		= NULL;
}

/*
 * This function initializes a new TEE Context, connecting this Client
 * application to the TEE identified by the name name.
 *
 * name == NULL will give the default TEE.
 */
MS_U32 MDrv_SYS_TEEC_InitializeContext(const char *name, TEEC_Context *context)
{
    int name_size = 0;
    const char* _name = name;

    ULOGW (TAG_SYS, "TEEC initial Context: %s\n", name);

    if (context == NULL) {
        ULOGE (TAG_SYS,"\033[1;31m[%s] NULL context\033[m\n", __FUNCTION__);
        return TEEC_ERROR_BAD_PARAMETERS;
    }

    /*
     * Specification says that when no name is provided it should fall back
     * on a predefined TEE.
     */
    if (name == NULL) {
        ULOGE (TAG_SYS,"\033[1;31m[%s] Name null\033[m\n", __FUNCTION__);
        return TEEC_ERROR_BAD_PARAMETERS; /* Device name truncated */
    }

    name_size = snprintf(context->devname, TEEC_MAX_DEVNAME_SIZE,
                 "/dev/%s", _name);

    if (name_size >= TEEC_MAX_DEVNAME_SIZE) {
        ULOGE (TAG_SYS,"\033[1;31m[%s] Name size over %u\033[m\n", __FUNCTION__, TEEC_MAX_DEVNAME_SIZE);
        return TEEC_ERROR_BAD_PARAMETERS; /* Device name truncated */
   }

    context->fd = open(context->devname, O_RDWR);
    if (context->fd == -1) {
        ULOGE (TAG_SYS, "\033[1;31m[%s]open %s fail \033[m\n", __FUNCTION__, context->devname);
        return TEEC_ERROR_ITEM_NOT_FOUND;
    }
    return TEEC_SUCCESS;
}

/*
 * This function opens a new Session between the Client application and the
 * specified TEE application.
 *
 * Only connection_method == TEEC_LOGIN_PUBLIC is supported connection_data and
 * operation shall be set to NULL.
 */
MS_U32 MDrv_SYS_TEEC_Open(TEEC_Context *context, TEEC_Session *session, const TEEC_UUID *destination, MS_U32 connection_method, const void *connection_data, TEEC_Operation *operation, MS_U32 *error_origin)
{
	TEEC_Operation dummy_op;
	uint32_t origin = TEEC_ORIGIN_API;
	TEEC_Result res = TEEC_SUCCESS;
	(void)connection_data;
	struct tee_cmd_io cmd;

	memset(&dummy_op, 0, sizeof(TEEC_Operation));
	memset(&cmd, 0, sizeof(struct tee_cmd_io));

	if (session != NULL)
		session->fd = -1;

	if ((context == NULL) || (session == NULL)) {
                ULOGE (TAG_SYS, "\033[1;31m[%s] Null context or session\033[m\n", __FUNCTION__);
		res = TEEC_ERROR_BAD_PARAMETERS;
		goto error;
	}

	if (connection_method != TEEC_LOGIN_PUBLIC) {
                ULOGE (TAG_SYS, "\033[1;31m[%s] connection_method not public\033[m\n", __FUNCTION__);
		res = TEEC_ERROR_NOT_SUPPORTED;
		goto error;
	}

	teec_resetTeeCmd(&cmd);
	cmd.uuid = (TEEC_UUID *)destination;

	if (operation == NULL) {
		/*
		 * The code here exist because Global Platform API states that
		 * it is allowed to give operation as a NULL pointer. In kernel
		 * and secure world we in most cases don't want this to be NULL,
		 * hence we use this dummy operation when a client doesn't
		 * provide any operation.
		 */
		memset(&dummy_op, 0, sizeof(TEEC_Operation));
		operation = &dummy_op;
	}

	cmd.op = operation;

	errno = 0;
	if (ioctl(context->fd, TEE_OPEN_SESSION_IOC, &cmd) != 0) {
		ULOGD(TAG_SYS, "Ioctl(TEE_OPEN_SESSION_IOC) failed! (%s) err %08x ori %08x\n",
		     strerror(errno), cmd.err, cmd.origin);
		if (cmd.origin)
			origin = cmd.origin;
		else
			origin = TEEC_ORIGIN_COMMS;
		if (cmd.err)
			res = cmd.err;
		else
			res = TEEC_ERROR_COMMUNICATION;
		goto error;
	}
	session->fd = cmd.fd_sess;

	if (cmd.err != 0) {
		ULOGD(TAG_SYS, "open session to TA UUID %x %x %x failed\n",
		     destination->timeLow,
		     destination->timeMid, destination->timeHiAndVersion);
	}
	origin = cmd.origin;
	res = cmd.err;

error:
	// printf("**** res=0x%08x, org=%d, seeid=%d ***\n", res, origin, cmd.fd_sess)

	/*
	 * We do this check at the end instead of checking on every place where
	 * we set the error origin.
	 */
	if (res != TEEC_SUCCESS) {
		if (session != NULL && session->fd != -1) {
			close(session->fd);
			session->fd = -1;
		}
	}

	if (error_origin != NULL)
		*error_origin = origin;

	return res;
}

/*
 * This function destroys an initialized TEE Context, closing the connection
 * between the Client and the TEE.
 * The function implementation MUST do nothing if context is NULL
 */
void MDrv_SYS_TEEC_FinalizeContext(TEEC_Context *context)
{
	if (context)
		close(context->fd);
}

/*
 * This function closes a session which has been opened with a TEE
 * application.
 */
void MDrv_SYS_TEEC_Close(TEEC_Session *session)
{
	if (session == NULL)
		return;

	close(session->fd);
}

/*
 * Invokes a TEE command (secure service, sub-PA or whatever).
 */
MS_U32 MDrv_SYS_TEEC_InvokeCmd(TEEC_Session *session, MS_U32 cmd_id, TEEC_Operation *operation, MS_U32 *error_origin)
{
	//INMSG("session: [%p], cmd_id: [%d]", session, cmd_id);
	struct tee_cmd_io cmd;
	TEEC_Operation dummy_op;
	TEEC_Result result = TEEC_SUCCESS;
	uint32_t origin = TEEC_ORIGIN_API;

	memset(&dummy_op, 0, sizeof(TEEC_Operation));
	memset(&cmd, 0, sizeof(struct tee_cmd_io));

	if (session == NULL) {
                ULOGE (TAG_SYS, "\033[1;31m[%s] Null session\033[m\n", __FUNCTION__);
		origin = TEEC_ORIGIN_API;
		result = TEEC_ERROR_BAD_PARAMETERS;
		goto error;
	}

	if (operation == NULL) {
		/*
		 * The code here exist because Global Platform API states that
		 * it is allowed to give operation as a NULL pointer. In kernel
		 * and secure world we in most cases don't want this to be NULL,
		 * hence we use this dummy operation when a client doesn't
		 * provide any operation.
		 */
		memset(&dummy_op, 0, sizeof(TEEC_Operation));
		operation = &dummy_op;
	}

	teec_mutex_lock(&teec_mutex);
	operation->session = session;
	teec_mutex_unlock(&teec_mutex);

	teec_resetTeeCmd(&cmd);

	cmd.cmd = cmd_id;
	cmd.op = operation;

	if (ioctl(session->fd, TEE_INVOKE_COMMAND_IOC, &cmd) != 0)
                ULOGD(TAG_SYS, "Ioctl(TEE_INVOKE_COMMAND_IOC) failed! (%s)\n",strerror(errno));

	if (operation != NULL) {
		teec_mutex_lock(&teec_mutex);

		operation->session = NULL;

		teec_mutex_unlock(&teec_mutex);
	}

	origin = cmd.origin;
	result = cmd.err;

error:

	if (error_origin != NULL)
		*error_origin = origin;

	ULOGD(TAG_SYS, "result:%d\n",result);
    return result;
}
#endif

MS_BOOL MDrv_SYS_GlobalInit(void)
{
    MS_BOOL bRet = TRUE;

#if defined (MSOS_TYPE_LINUX)
    PTH_RET_CHK(pthread_mutex_lock(&_SYS_Global_Init_Mutex));
    if(u8IsInit)
    {
        ULOGD(TAG_SYS, "%s successfully called one time,call %s again in the same process  will  cause error",__func__,__func__);
        PTH_RET_CHK(pthread_mutex_unlock(&_SYS_Global_Init_Mutex));
        return TRUE;
    }
#endif

#if defined (ANDROID)
    bRet = MDrv_MMIO_Init();
    if(bRet == FALSE)
		goto err_return;

	bRet = MsOS_Init();
    if(bRet == FALSE)
		goto err_return;

	MsOS_MPool_Init();

	bRet = MDrv_SEM_Init();
    if(bRet == FALSE)
		goto err_return;

	bRet = MDrv_MIU_Init();
    if(bRet == FALSE)
		goto err_return;

	bRet = MDrv_SYS_Init();
    if(bRet == FALSE)
		goto err_return;

    MApi_MBX_Init(E_MBX_CPU_MIPS, E_MBX_ROLE_HK, 5000);
    MApi_MBX_Enable(TRUE);

	goto pass;

err_return:
	PTH_RET_CHK(pthread_mutex_unlock(&_SYS_Global_Init_Mutex));
	return bRet;
#else
    if(MDrv_MMIO_Init() == FALSE)
	   bRet = FALSE;
    if(MsOS_Init() == FALSE)
	   bRet = FALSE;
    if(MsOS_MPool_Init() == FALSE)
	   bRet = FALSE;
    if(MDrv_SEM_Init() == FALSE)
	   bRet = FALSE;
    if(MDrv_MIU_Init() == FALSE)
	   bRet = FALSE;
    if(MDrv_SYS_Init() == FALSE)
	   bRet = FALSE;

	goto pass;
#endif

pass:
#if defined (MSOS_TYPE_LINUX)
    u8IsInit = 1;
    PTH_RET_CHK(pthread_mutex_unlock(&_SYS_Global_Init_Mutex));
#endif

    ULOGD(TAG_SYS, "%s finished", __func__);
    return bRet;
}
#endif   //#ifndef MSOS_TYPE_LINUX_KERNEL

#if defined(MSOS_TYPE_LINUX_KERNEL)
EXPORT_SYMBOL(MDrv_SYS_GlobalInit);
#endif

//-------------------------------------------------------------------------------------------------
/// Get system information
/// @return Chip Revision ID
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_SYS_GetChipRev(void)
{
    return SYS_GetChipRev();
}

//-------------------------------------------------------------------------------------------------
/// Get system information
/// @return Chip Device ID
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_SYS_GetChipID(void)
{
    return SYS_GetChipID();
}

//-------------------------------------------------------------------------------------------------
/// Get system information
/// @return @ref SYS_Info
//-------------------------------------------------------------------------------------------------
const SYS_Info* MDrv_SYS_GetInfo(void)
{
    return SYS_GetInfo();
}

const MSIF_Version* MDrv_SYS_GetLibVer(void)
{
    return (const MSIF_Version*)(&_sysVersion);
}

MS_BOOL MDrv_SYS_GetStatus(SYS_Status *pStatus)
{
    memcpy(pStatus, &_sysStatus, sizeof(SYS_Status));
    return TRUE;
}

void MDrv_SYS_GetSoftwareVersion(MS_SW_VERSION_INFO *pSoftwareVersionInfo)
{
    memset(pSoftwareVersionInfo, '\0', sizeof(MS_SW_VERSION_INFO));

    strncpy(
        pSoftwareVersionInfo->MajorVersion,
        (char *) _sysVersion.DDI.version,
        sizeof(_sysVersion.DDI.version)
        );

    strncpy(
        pSoftwareVersionInfo->MinorVersion,
        (char *) _sysVersion.DDI.build,
        sizeof(_sysVersion.DDI.build)
        );

#ifdef UTOPIA_BSP_VERSION
    snprintf(
        pSoftwareVersionInfo->UtopiaBspVersion,
        sizeof(pSoftwareVersionInfo->UtopiaBspVersion),
        "%d",
        UTOPIA_BSP_VERSION
        );
#endif

#ifdef SW_VERSION_API_SYS
    snprintf(
        pSoftwareVersionInfo->ChangeList_API,
        sizeof(pSoftwareVersionInfo->ChangeList_API),
        "%d",
        SW_VERSION_API_SYS
        );
#endif

#ifdef SW_VERSION_DRV_SYS
    snprintf(
        pSoftwareVersionInfo->ChangeList_DRV,
        sizeof(pSoftwareVersionInfo->ChangeList_DRV),
        "%d",
        SW_VERSION_DRV_SYS
        );
#endif

#ifdef SW_VERSION_HAL_SYS
    snprintf(
        pSoftwareVersionInfo->ChangeList_HAL,
        sizeof(pSoftwareVersionInfo->ChangeList_HAL),
        "%d",
        SW_VERSION_HAL_SYS
        );
#endif

    return;
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable watchdog timer
/// @param  bEnable                 \b IN:  Enable/disable
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_SYS_WDTEnable(MS_BOOL bEnable)
{
    if (bEnable)
    {
#if 0 //TODO: Utopia & HAL
        WDT_REG(REG_WDT_DISABLEWORD_L) = 0;
        WDT_REG(REG_WDT_DISABLEWORD_H) = 0;
#endif
        _sysStatus.WDT_Active = TRUE;
    }
    else
    {
#if 0 //TODO: Utopia & HAL
        WDT_REG(REG_WDT_DISABLEWORD_L) = 0xaa55;
        WDT_REG(REG_WDT_DISABLEWORD_H) = 0x55aa;
#endif
        _sysStatus.WDT_Active = FALSE;
    }
}


//-------------------------------------------------------------------------------------------------
/// Clear watchdog timer
/// @return None
/// @note The monitor task should call this API regularly
//-------------------------------------------------------------------------------------------------
void MDrv_SYS_WDTClear(void)
{
#if 0 //TODO: Utopia & HAL
    WDT_REG(REG_WDT_CLEAR_STATUS) = 0x0001;
#endif
}


//-------------------------------------------------------------------------------------------------
/// Query whether Watchdog timer was triggered last time and clear the last status
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SYS_WDTLastStatus(void)
{
    MS_BOOL bRet = FALSE;

#if 0 //TODO: Utopia & HAL
    if (WDT_REG(REG_WDT_CLEAR_STATUS) & 0x0004)
    {
        bRet = TRUE;
    }

    WDT_REG(REG_WDT_CLEAR_STATUS) = 0x0002;
#endif

    return bRet;
}


//-------------------------------------------------------------------------------------------------
/// Set tigger time of watchdog timer
/// @param  u32Ms                   \b IN:  msec (350~357564 ms at 12 MHz; 146~148958 ms at 28.8 MHz)
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_SYS_WDTSetTime(MS_U32 u32Ms)
{
#if 0 //TODO: Utopia & HAL
    WDT_REG(REG_WDT_SETTIME) = (u32Ms * (XTAL_CLOCK_FREQ / 1000)) >> 22; //1 ~ 0x3FF
#endif
}


//-------------------------------------------------------------------------------------------------
/// Reset whole chip
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_SYS_ResetChip(void)
{
#if 0 //TODO: Utopia & HAL
    WDT_REG(REG_WDT_PROGRAMKEY_L)   = 0x5678;
    WDT_REG(REG_WDT_PROGRAMKEY_H)   = 0x1234;
    WDT_REG(REG_WDT_ENABLEMCURESET) = 0;
    WDT_REG(REG_WDT_DISABLEWORD_L)  = 0;
    WDT_REG(REG_WDT_DISABLEWORD_H)  = 0;
    WDT_REG(REG_WDT_SETTIME)        = 0;
    while(1);
#endif
}


//-------------------------------------------------------------------------------------------------
/// Reset CPU only
/// @return None
//-------------------------------------------------------------------------------------------------
void MDrv_SYS_ResetCPU(void)
{
#if 0 //TODO: Utopia & HAL
    #ifndef MS_NOSAPI
    MS_U32 u32OldIntr;
    #endif

    SYS_DIS_ALL_INT(u32OldIntr);

    SYS_ResetCPU();

//    SYS_RES_ALL_INT(u32OldInts)
    while(1);
#endif
}


void TINY_SYS_Reset(void)
{
#if 0 //TODO: Utopia & HAL
    SYS_ResetCPU();
    while (1);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set RFAGC and TAGC PAD MUX
/// @return None
//-------------------------------------------------------------------------------------------------
void  MDrv_SYS_SetAGCPadMux(SYS_AGC_PAD_SET eAgcPadMux)
{
    HAL_SYS_SetAGCPadMux(eAgcPadMux);
}

MS_BOOL MDrv_SYS_SetPCMCardDetectMode(SYS_PCM_CD_MODE ePCMCDMode)
{
    MS_BOOL ret = FALSE;

    if(ePCMCDMode >= E_PCM_CD_MAX)
        return ret;
    ret = HAL_SYS_SetPCMCardDetectMode(ePCMCDMode);
    return ret;
}

MS_BOOL MDrv_SYS_DisableDebugPort(void)
{
    return HAL_SYS_DisableDebugPort();
}

MS_BOOL MDrv_SYS_EnableDebugPort(void)
{
    return HAL_SYS_EnableDebugPort();
}

MS_BOOL MDrv_SYS_SetPadMux(SYS_PAD_MUX_SET ePadMuxType,SYS_PAD_SEL ePadSel)
{
    MS_BOOL ret = FALSE;
    if((ePadMuxType >= E_PAD_SET_MAX) || (ePadSel >= E_PAD_SEL_MAX))
        return ret;
    ret = HAL_SYS_SetPadMux(ePadMuxType,ePadSel);
    return ret;
}

MS_BOOL MDrv_SYS_PackMode(void)
{
#ifdef CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER_STRUCT_PACKED
    return TRUE;
#else
    return FALSE;
#endif
}

MS_BOOL MDrv_SYS_SetTSOutClockPhase(MS_U16 u16Val)
{
    return HAL_SYS_SetTSOutClockPhase(u16Val);
}

MS_BOOL MDrv_SYS_SetTSClockPhase(SYS_PAD_MUX_SET ePadMuxType, MS_U16 u16Val)
{
    return HAL_SYS_SetTSClockPhase(ePadMuxType, u16Val);
}

MS_BOOL MDrv_SYS_PadMuxTableSuspend(void)
{
    return HAL_SYS_PadMuxTableSuspend();
}

MS_BOOL MDrv_SYS_PadMuxTableResume(void)
{
    return HAL_SYS_PadMuxTableResume();
}
//-------------------------------------------------------------------------------------------------
/// @brief \b Function  \b Name: MDrv_SYS_SetDbgLevel
/// @brief \b Function  \b Description: Set debug level for debug message
/// @param eLevel       \b IN: E_SYS_DBGLV_NONE/E_SYS_DBGLV_ERR_ONLY/E_SYS_DBGLV_INFO/E_SYS_DBGLV_ALL
/// @return             \b SYS_Result
//-------------------------------------------------------------------------------------------------
void MDrv_SYS_SetDbgLevel(SYS_DbgLv eLevel)
{
    _gu8DbgLevel = eLevel;
}

void MDrv_SYS_VIFWriteByteByVDMbox(MS_U32 u32Reg, MS_U8 u8Val)
{
    SYS_VIF_WriteByteByVDMbox(u32Reg,u8Val);
}

void MDrv_SYS_VIFWriteByteMaskByVDMbox(MS_U32 u32Reg, MS_U8 u8Val, MS_U8 u8Mask)
{
    SYS_VIF_WriteByteMaskByVDMbox(u32Reg,u8Val,u8Mask);
}
void MDrv_SYS_VIFWriteRegBitByVDMbox(MS_U32 u32Reg, MS_U8 bEnable, MS_U8 u8Mask)
{
    SYS_VIF_WriteRegBitByVDMbox(u32Reg,bEnable,u8Mask);
}
MS_U8 MDrv_SYS_VIFReadByteByVDMbox(MS_U32 u32Reg)
{
    return SYS_VIF_ReadByteByVDMbox(u32Reg);
}
MS_U16 MDrv_SYS_VIFRead2ByteByVDMbox(MS_U32 u32Reg)
{
    return SYS_VIF_Read2ByteByVDMbox(u32Reg);
}

MS_BOOL MDrv_SYS_Query(E_SYS_QUERY id)
{
    return HAL_SYS_Query(id);
}

#ifdef _SYS_DAC_GAIN_CTRL_
//-------------------------------------------------------------------------------------------------
/// Set DAC 1/2 enhance gain.
/// @param u8DAC_ID \b IN: 1: DAC1, 2: DAC2, Other: Invalid DAC ID.
/// @param bEnable \b IN: enable/disable enhance gain.
/// @return void
//-------------------------------------------------------------------------------------------------
void MDrv_SYS_EnhanceDACGain(MS_U8 u8DAC_ID, MS_BOOL bEnable)
{
    HAL_SYS_EnhanceDACGain(u8DAC_ID, bEnable);
}
#endif


//-------------------------------------------------------------------------------------------------
/// Quert Dolby Hash Info.
/// @param index \b IN: E_SYS_DOLBY_VERSION: Dolby version, E_SYS_DOLBY_CONTROL_BIT: Dolby control bit, E_SYS_DOLBY_REBERSE_BIT: Dolby reverse bit.
/// @return The info about Dolby hash info accroding to the input param index.
//------------------------------------------------------------------------------------------------
MS_U32 MDrv_SYS_QueryDolbyHashInfo(E_SYS_DOLBY_HASH_INFO index)
{
    return HAL_SYS_QueryDolbyHashInfo(index);
}

//-------------------------------------------------------------------------------------------------
/// @brief \b Function  \b Name: MDrv_SYS_GetChipType
/// @brief \b Function  \b Description: get the TV/STB chip type
/// @param eLevel       \b IN: E_SYS_DBGLV_NONE/E_SYS_DBGLV_ERR_ONLY/E_SYS_DBGLV_INFO/E_SYS_DBGLV_ALL
/// @return             \b E_SYS_CHIP_TYPE  E_SYS_CHIP_TV:TV chip set / E_SYS_CHIP_STB : STB chip set
//-------------------------------------------------------------------------------------------------
E_SYS_CHIP_TYPE MDrv_SYS_GetChipType(void)
{
    return HAL_SYS_GetChipType();
}

void MDrv_SYS_SetChipType(E_SYS_CHIP_TYPE Type)
{
}


static volatile MS_U8 u8gDolbyKeyCustomer[24] =
    {'H','K','C','S',
      0x44,0x10,0x91,0x68,0xff,0x0e,0x4c,0x28,0x77,0x0e,0xf5,0x57,0xe0,0x20,0x02,0xdf
    ,'H','K','C','E'};

//-------------------------------------------------------------------------------------------------
/// @brief \b Function  \b Name: MDrv_SYS_GetDolbyKeyCustomer
/// @brief \b Function  \b Description: get the Key Customer
/// @param eLevel       \b IN/OUT: The point of key customer
/// @return             \b void
//-------------------------------------------------------------------------------------------------
void MDrv_SYS_GetDolbyKeyCustomer(MS_U8 * u8pkey)
{
    MS_U8 u8index;

    for(u8index =4; u8index <(16+4); u8index ++)
    {

        u8pkey[u8index-4] = u8gDolbyKeyCustomer [u8index];

    }

}

MS_U16 MDrv_SYS_ReadBrickTerminatorStatus(void)
{
    return HAL_SYS_ReadBrickTerminatorStatus();
}

void MDrv_SYS_WriteBrickTerminatorStatus(MS_U16 u16Status)
{
    HAL_SYS_WriteBrickTerminatorStatus(u16Status);
}

MS_BOOL MDrv_SYS_GetEfuseDid(MS_U16 *u16efuse_did)
{
    if(NULL == u16efuse_did)
    {
        SYS_DBG_INFO("%s NULL pointer is invalid!\n",__FUNCTION__);
        return FALSE;
    }

    HAL_SYS_GetEfuseDid(u16efuse_did);

    return TRUE;
}

MS_BOOL MDrv_SYS_ReadEfuseHDCPKey(MS_U16 u16ReadAddr, MS_U32 *u32HDCPKey)
{
    return HAL_SYS_ReadEfuseHDCPKey(u16ReadAddr,u32HDCPKey);
}

MS_BOOL MDrv_SYS_EnableWOL(MS_BOOL bEnable)
{
    if(bEnable)
        HAL_SYS_EnableWkEventWOL();
    else
        HAL_SYS_DisableWkEventWOL();

    return TRUE;
}

MS_BOOL MDrv_SYS_GetStatusWOL(void)
{
    return HAL_SYS_GetStatusWOL();
}

void MDrv_SYS_ResetStatusWOL(void)
{
    HAL_SYS_ResetStatusWOL();
}


/////////////////////////////////////////////////////////////////
//////// UTOPIA2 CODE ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////
enum
{
    SYS_POOL_ID_SYS0=0
} eSYS_PoolID;

//--------------------------------------------------------------------------------------------------
// Utopia2.0 will call this function to register module
//--------------------------------------------------------------------------------------------------
void SYSRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. create a module(module_name, SHM_size), and register to utopia2.0
    void* pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_SYS, 0, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);

    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)SYSOpen, (FUtopiaClose)SYSClose, (FUtopiaIOctl)SYSIoctl);

    // 2. Resource register
    void* psResource = NULL;

    // start func to add resources of a certain Resource_Pool
    UtopiaModuleAddResourceStart(pUtopiaModule, SYS_POOL_ID_SYS0);

    // create a resouce and regiter it to a certain Resource_Pool, resource can alloc private for internal use
    UtopiaResourceCreate("sys0", sizeof(SYS_RESOURCE_PRIVATE), &psResource);
#ifdef CONFIG_MSTAR_DVFS_ENABLE
    ((SYS_RESOURCE_PRIVATE*) psResource)->bDvfsInitFlag = 0;
#endif
    UtopiaResourceRegister(pUtopiaModule, psResource, SYS_POOL_ID_SYS0);

    //UtopiaResourceCreate("sys1", sizeof(SYS_RESOURCE_PRIVATE), &psResource);
    //UtopiaResourceRegister(pUtopiaModule, psResource, SYS_POOL_ID_SYS0);

    // end func to add resources of a certain Resource_Pool(this will set the ResourceSemaphore of this ResourcePool)
    UtopiaModuleAddResourceEnd(pUtopiaModule, SYS_POOL_ID_SYS0);
}

//--------------------------------------------------------------------------------------------------
// Utopia2.0 will call this function to get a instance to use SYS
// @ \b in: 32ModuleVersion => this is for checking if API version is the same
//--------------------------------------------------------------------------------------------------
MS_U32 SYSOpen(void** ppInstance, void* pAttribute)
{
    // for multi-process safe, check if already other Instance exist
    // 1. use moduleID to get module, then try to get resource
    void *pModule = NULL;
    void *pResource = NULL;
    UtopiaModuleGetPtr(MODULE_SYS, &pModule);

    if(UtopiaResourceObtain(pModule, SYS_POOL_ID_SYS0, &pResource) != 0)
    {
        ULOGE(TAG_SYS, "UtopiaResourceObtainToInstant fail\n");
        return UTOPIA_STATUS_FAIL;
    }

    // if get a resource, check the module private SHM(default to be 0, only create at the first time) to decide whether this process can open instance
    void *pSYSResPri = NULL;
    UtopiaResourceGetPrivate(pResource, &pSYSResPri);

    ULOGD(TAG_SYS, "\033[35mFunction = %s, Line = %d, current resource pri_shm content is: %u\033[m\n", __PRETTY_FUNCTION__, __LINE__, *(unsigned int *)pSYSResPri);

    //if(*(MS_U32 *)pSYSResPri == 0)
    {
#ifdef CONFIG_MSTAR_DVFS_ENABLE
        if(((SYS_RESOURCE_PRIVATE*) pSYSResPri)->bDvfsInitFlag == 0)
        {
            ((SYS_RESOURCE_PRIVATE*) pSYSResPri)->bDvfsInitFlag = 0;
            g_bDvfsInitFlag = 0;
        }
        else
        {
            g_bDvfsInitFlag = 1;
        }
#endif
    }

    UtopiaResourceRelease(pResource);

    ULOGD(TAG_SYS, "\033[35mFunction = %s, Line = %d, [SYS INFO] OPEN INSTANCE...\033[m\n", __PRETTY_FUNCTION__, __LINE__);
    SYS_INSTANT_PRIVATE *pSYSPri = NULL;

    // instance is allocated here, also can allocate private for internal use, ex, SYS_INSTANT_PRIVATE, ppInstance will point to a pointer of the created UTOPIA_INSTANCE
    UtopiaInstanceCreate(sizeof(SYS_INSTANT_PRIVATE), ppInstance);
    // set the pSYSPri point to the private of UTOPIA_INSTANCE
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pSYSPri);

    // setup func in private and assign the calling func in func ptr in instance private
    pSYSPri->fpSYS_GetChipRev                       = (IOCTL_SYS_GETCHIPREV)MDrv_SYS_GetChipRev;
    pSYSPri->fpSYS_GetChipID                        = (IOCTL_SYS_GETCHIPID)MDrv_SYS_GetChipID;
    pSYSPri->fpSYS_GetInfo                          = (IOCTL_SYS_GETINFO)MDrv_SYS_GetInfo;
    pSYSPri->fpSYS_GetLibVer                        = (IOCTL_SYS_GETLIBVER)MDrv_SYS_GetLibVer;
    pSYSPri->fpSYS_GetStatus                        = (IOCTL_SYS_GETSTATUS)MDrv_SYS_GetStatus;
    pSYSPri->fpSYS_GetSoftwareVersion               = (IOCTL_SYS_GETSOFTWAREVERSION)MDrv_SYS_GetSoftwareVersion;
    pSYSPri->fpSYS_DisableDebugPort                 = (IOCTL_SYS_DISABLEDEBUGPORT)MDrv_SYS_DisableDebugPort;
    pSYSPri->fpSYS_EnableDebugPort                  = (IOCTL_SYS_ENABLEDEBUGPORT)MDrv_SYS_EnableDebugPort;
    pSYSPri->fpSYS_SetDbgLevel                      = (IOCTL_SYS_SETDBGLEVEL)MDrv_SYS_SetDbgLevel;
    pSYSPri->fpSYS_VIFWriteByteByVDMbox             = (IOCTL_SYS_VIFWRITEBYTEBYVDMBOX)MDrv_SYS_VIFWriteByteByVDMbox;
    pSYSPri->fpSYS_VIFWriteByteMaskByVDMbox         = (IOCTL_SYS_VIFWRITEBYTEMASKBYVDMBOX)MDrv_SYS_VIFWriteByteMaskByVDMbox;
    pSYSPri->fpSYS_VIFWriteRegBitByVDMbox           = (IOCTL_SYS_VIFWRITEREGBITBYVDMBOX)MDrv_SYS_VIFWriteRegBitByVDMbox;
    pSYSPri->fpSYS_VIFReadByteByVDMbox              = (IOCTL_SYS_VIFREADBYTEBYVDMBOX)MDrv_SYS_VIFReadByteByVDMbox;
    pSYSPri->fpSYS_VIFRead2ByteByVDMbox             = (IOCTL_SYS_VIFREAD2BYTEBYVDMBOX)MDrv_SYS_VIFRead2ByteByVDMbox;
    pSYSPri->fpSYS_WDTEnable                        = (IOCTL_SYS_WDTENABLE)MDrv_SYS_WDTEnable;
    pSYSPri->fpSYS_WDTClear                         = (IOCTL_SYS_WDTCLEAR)MDrv_SYS_WDTClear;
    pSYSPri->fpSYS_WDTLastStatus                    = (IOCTL_SYS_WDTLASTSTATUS)MDrv_SYS_WDTLastStatus;
    pSYSPri->fpSYS_WDTSetTime                       = (IOCTL_SYS_WDTSETTIME)MDrv_SYS_WDTSetTime;
    pSYSPri->fpSYS_ResetChip                        = (IOCTL_SYS_RESETCHIP)MDrv_SYS_ResetChip;
    pSYSPri->fpSYS_ResetCPU                         = (IOCTL_SYS_RESETCPU)MDrv_SYS_ResetCPU;
    pSYSPri->fpSYS_SetAGCPadMux                     = (IOCTL_SYS_SETAGCPADMUX)MDrv_SYS_SetAGCPadMux;
    pSYSPri->fpSYS_SetPCMCardDetectMode             = (IOCTL_SYS_SETPCMCARDDETECTMODE)MDrv_SYS_SetPCMCardDetectMode;
    pSYSPri->fpSYS_SetPadMux                        = (IOCTL_SYS_SETPADMUX)MDrv_SYS_SetPadMux;
    pSYSPri->fpSYS_SetTSOutClockPhase               = (IOCTL_SYS_SETTSOUTCLOCKPHASE)MDrv_SYS_SetTSOutClockPhase;
    pSYSPri->fpSYS_Query                            = (IOCTL_SYS_QUERY)MDrv_SYS_Query;
#ifdef _SYS_DAC_GAIN_CTRL
    pSYSPri->fpSYS_EnhanceDACGain                   = (IOCTL_SYS_ENHANCEDACGAIN)MDrv_SYS_EnhanceDACGain;
#endif
    pSYSPri->fpSYS_Init                             = (IOCTL_SYS_INIT)MDrv_SYS_Init;
    pSYSPri->fpSYS_GlobalInit                       = (IOCTL_SYS_GLOBALINIT)MDrv_SYS_GlobalInit;
#if defined (MSOS_TYPE_LINUX)
    pSYSPri->fpSYS_TeecInitContext                  = (IOCTL_SYS_TEEC_INIT_CONTEXT)MDrv_SYS_TEEC_InitializeContext;
    pSYSPri->fpSYS_TeecOpen                         = (IOCTL_SYS_TEEC_OPEN)MDrv_SYS_TEEC_Open;
    pSYSPri->fpSYS_TeecClose                        = (IOCTL_SYS_TEEC_CLOSE)MDrv_SYS_TEEC_Close;
    pSYSPri->fpSYS_TeecInvokeCmd                    = (IOCTL_SYS_TEEC_INVOKE_CMD)MDrv_SYS_TEEC_InvokeCmd;
    pSYSPri->fpSYS_TeecFinalizeContext              = (IOCTL_SYS_TEEC_FINAL_CONTEXT)MDrv_SYS_TEEC_FinalizeContext;
#endif

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 SYSIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);

    SYS_INSTANT_PRIVATE *psSYSInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psSYSInstPri);

    PSYS_GETSTATUS_PARAM pGetStatusParam = NULL;
    PSYS_GETSOFTWAREVERSION_PARAM pGetSoftwareVersionParam = NULL;
    PSYS_SETDBGLEVEL_PARAM pSetDbgLevelParam = NULL;
    PSYS_VIFWRITEBYTEBYVDMBOX_PARAM pVIFWriteByteByVDMboxParam = NULL;
    PSYS_VIFWRITEBYTEMASKBYVDMBOX_PARAM pVIFWriteByteMaskByVDMboxParam = NULL;
    PSYS_VIFWRITEREGBITBYVDMBOX_PARAM pVIFWriteRegBitByVDMboxParam = NULL;
    PSYS_VIFREADBYTEBYVDMBOX_PARAM pVIFReadByteByVDMboxParam = NULL;
    PSYS_VIFREAD2BYTEBYVDMBOX_PARAM pVIFRead2ByteByVDMboxParam = NULL;
    PSYS_WDTENABLE_PARAM pWDTEnableParam = NULL;
    PSYS_WDTSETTIME_PARAM pWDTSetTimeParam = NULL;
    PSYS_SETAGCPADMUX_PARAM pSetAGCPadMuxParam = NULL;
    PSYS_SETPCMCARDDETECT_PARAM pSetPCMCardDetectModeParam = NULL;
    PSYS_SETPADMUX_PARAM pSetPadMuxParam = NULL;
    PSYS_SETTSOUTCLOCKPHASE_PARAM pSetTSOutClockPhaseParam = NULL;
    PSYS_QUERY_PARAM pQueryParam = NULL;
#ifdef _SYS_DAC_GAIN_CTRL
    PSYS_ENHANCEDACGAIN_PARAM pEnhanceDACGainParam = NULL;
#endif
#if defined (MSOS_TYPE_LINUX)
    PSYS_SYS_TEECOPEN_PARAM pTeecOpenParam = NULL;
    PSYS_SYS_TEECCLOSE_PARAM pTeecCloseParam = NULL;
    PSYS_SYS_TEECINVOKECMD_PARAM pTeecInvokeCmdParam = NULL;
    PSYS_SYS_TEECINITCONTEXT_PARAM pTeecInitContextParam = NULL;
#endif

    MS_U32 u32Ret;

    switch(u32Cmd)
    {
        case MDrv_CMD_SYS_GetChipRev:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_GetChipRev\n");
            *((MS_U8 *)(pArgs)) = psSYSInstPri->fpSYS_GetChipRev();
            return TRUE;
        case MDrv_CMD_SYS_GetChipID:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_GetChipID\n");
            *((MS_U16 *)(pArgs)) = psSYSInstPri->fpSYS_GetChipID();
            return TRUE;
        case MDrv_CMD_SYS_GetInfo:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_GetInfo\n");
            memcpy(pArgs, psSYSInstPri->fpSYS_GetInfo(), sizeof(const SYS_Info));
            return TRUE;
        case MDrv_CMD_SYS_GetLibVer:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_GetLibVer\n");
            memcpy(pArgs, psSYSInstPri->fpSYS_GetLibVer(), sizeof(const MSIF_Version));
            return TRUE;
        case MDrv_CMD_SYS_GetStatus:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_GetStatus\n");
            pGetStatusParam = (PSYS_GETSTATUS_PARAM)pArgs;
            u32Ret = (MS_U32)psSYSInstPri->fpSYS_GetStatus(pGetStatusParam->pStatus);
            return u32Ret;
        case MDrv_CMD_SYS_GetSoftwareVersion:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_GetSoftwareVersion\n");
            pGetSoftwareVersionParam = (PSYS_GETSOFTWAREVERSION_PARAM)pArgs;
            psSYSInstPri->fpSYS_GetSoftwareVersion(pGetSoftwareVersionParam->pSoftwareVersionInfo);
            return TRUE;
        case MDrv_CMD_SYS_DisableDebugPort:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_DisableDebugPort\n");
            u32Ret = (MS_U32)psSYSInstPri->fpSYS_DisableDebugPort();
            return u32Ret;
        case MDrv_CMD_SYS_EnableDebugPort:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_EnableDebugPort\n");
            u32Ret = (MS_U32)psSYSInstPri->fpSYS_EnableDebugPort();
            return u32Ret;
        case MDrv_CMD_SYS_SetDbgLevel:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_SetDbgLevel\n");
            pSetDbgLevelParam = (PSYS_SETDBGLEVEL_PARAM)pArgs;
            psSYSInstPri->fpSYS_SetDbgLevel(pSetDbgLevelParam->eLevel);
            return TRUE;
        case MDrv_CMD_SYS_VIFWriteByteByVDMbox:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_VIFWriteByteByVDMbox\n");
            pVIFWriteByteByVDMboxParam = (PSYS_VIFWRITEBYTEBYVDMBOX_PARAM)pArgs;
            psSYSInstPri->fpSYS_VIFWriteByteByVDMbox(pVIFWriteByteByVDMboxParam->u32Reg, pVIFWriteByteByVDMboxParam->u8Val);
            return TRUE;
        case MDrv_CMD_SYS_VIFWriteByteMaskByVDMbox:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_VIFWriteByteMaskByVDMbox\n");
            pVIFWriteByteMaskByVDMboxParam = (PSYS_VIFWRITEBYTEMASKBYVDMBOX_PARAM)pArgs;
            psSYSInstPri->fpSYS_VIFWriteByteMaskByVDMbox(pVIFWriteByteMaskByVDMboxParam->u32Reg, pVIFWriteByteMaskByVDMboxParam->u8Val, pVIFWriteByteMaskByVDMboxParam->u8Mask);
            return TRUE;
        case MDrv_CMD_SYS_VIFWriteRegBitByVDMbox:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_VIFWriteRegBitByVDMbox\n");
            pVIFWriteRegBitByVDMboxParam = (PSYS_VIFWRITEREGBITBYVDMBOX_PARAM)pArgs;
            psSYSInstPri->fpSYS_VIFWriteRegBitByVDMbox(pVIFWriteRegBitByVDMboxParam->u32Reg, pVIFWriteRegBitByVDMboxParam->bEnable, pVIFWriteRegBitByVDMboxParam->u8Mask);
            return TRUE;
        case MDrv_CMD_SYS_VIFReadByteByVDMbox:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_VIFReadByteByVDMbox\n");
            pVIFReadByteByVDMboxParam = (PSYS_VIFREADBYTEBYVDMBOX_PARAM)pArgs;
            u32Ret = (MS_U32)psSYSInstPri->fpSYS_VIFReadByteByVDMbox(pVIFReadByteByVDMboxParam->u32Reg);
            return u32Ret;
        case MDrv_CMD_SYS_VIFRead2ByteByVDMbox:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_VIFRead2ByteByVDMbox\n");
            pVIFRead2ByteByVDMboxParam = (PSYS_VIFREAD2BYTEBYVDMBOX_PARAM)pArgs;
            u32Ret = (MS_U32)psSYSInstPri->fpSYS_VIFRead2ByteByVDMbox(pVIFRead2ByteByVDMboxParam->u32Reg);
            return u32Ret;
        case MDrv_CMD_SYS_WDTEnable:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_WDTEnable\n");
            pWDTEnableParam = (PSYS_WDTENABLE_PARAM)pArgs;
            psSYSInstPri->fpSYS_WDTEnable(pWDTEnableParam->bEnable);
            return TRUE;
        case MDrv_CMD_SYS_WDTClear:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_WDTClear\n");
            psSYSInstPri->fpSYS_WDTClear();
            return TRUE;
        case MDrv_CMD_SYS_WDTLastStatus:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_WDTLastStatus\n");
            u32Ret = (MS_U32)psSYSInstPri->fpSYS_WDTLastStatus();
            return u32Ret;
        case MDrv_CMD_SYS_WDTSetTime:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_WDTSetTime\n");
            pWDTSetTimeParam = (PSYS_WDTSETTIME_PARAM)pArgs;
            psSYSInstPri->fpSYS_WDTSetTime(pWDTSetTimeParam->u32Ms);
            return TRUE;
        case MDrv_CMD_SYS_ResetChip:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_ResetChip\n");
            psSYSInstPri->fpSYS_ResetChip();
            return TRUE;
        case MDrv_CMD_SYS_ResetCPU:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_ResetCPU\n");
            psSYSInstPri->fpSYS_ResetCPU();
            return TRUE;
        case MDrv_CMD_SYS_SetAGCPadMux:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_SetAGCPadMux\n");
            pSetAGCPadMuxParam = (PSYS_SETAGCPADMUX_PARAM)pArgs;
            psSYSInstPri->fpSYS_SetAGCPadMux(pSetAGCPadMuxParam->eAgcPadMux);
            return TRUE;
        case MDrv_CMD_SYS_SetPCMCardDetectMode:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_SetPCMCardDetectMode\n");
            pSetPCMCardDetectModeParam = (PSYS_SETPCMCARDDETECT_PARAM)pArgs;
            u32Ret = (MS_U32)psSYSInstPri->fpSYS_SetPCMCardDetectMode(pSetPCMCardDetectModeParam->ePCMCDMode);
            return u32Ret;
        case MDrv_CMD_SYS_SetPadMux:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_SetPadMux\n");
            pSetPadMuxParam = (PSYS_SETPADMUX_PARAM)pArgs;
            u32Ret = (MS_U32)psSYSInstPri->fpSYS_SetPadMux(pSetPadMuxParam->ePadMuxType, pSetPadMuxParam->ePadSel);
            return u32Ret;
        case MDrv_CMD_SYS_SetTSOutClockPhase:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_SetTSOutClockPhase\n");
            pSetTSOutClockPhaseParam = (PSYS_SETTSOUTCLOCKPHASE_PARAM)pArgs;
            u32Ret = (MS_U32)psSYSInstPri->fpSYS_SetTSOutClockPhase(pSetTSOutClockPhaseParam->u16Val);
            return u32Ret;
        case MDrv_CMD_SYS_Query:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_Query\n");
            pQueryParam = (PSYS_QUERY_PARAM)pArgs;
            u32Ret = (MS_U32)psSYSInstPri->fpSYS_Query(pQueryParam->id);
            return u32Ret;
#ifdef _SYS_DAC_GAIN_CTRL
        case MDrv_CMD_SYS_EnhanceDACGain:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_EnhanceDACGain\n");
            pEnhanceDACGainParam = (PSYS_ENHANCEDACGAIN_PARAM)pArgs;
            psSYSInstPri->fpSYS_EnhanceDACGain(pEnhanceDACGainParam->u8DAC_ID, pEnhanceDACGainParam->bEnable);
            return TRUE;
#endif
        case MDrv_CMD_SYS_Init:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_Init\n");
            u32Ret = (MS_U32)psSYSInstPri->fpSYS_Init();
            return u32Ret;
        case MDrv_CMD_SYS_GlobalInit:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_GlobalInit\n");
            psSYSInstPri->fpSYS_GlobalInit();
            return TRUE;
#if defined (MSOS_TYPE_LINUX)
        case MDrv_CMD_SYS_TeecInitContext:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_TeecInitContext\n");
            pTeecInitContextParam = (PSYS_SYS_TEECINITCONTEXT_PARAM)pArgs;
            u32Ret = psSYSInstPri->fpSYS_TeecInitContext(pTeecInitContextParam->name, pTeecInitContextParam->context);
            return u32Ret;
        case MDrv_CMD_SYS_TeecOpen:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_TeecOpen\n");
            pTeecOpenParam = (PSYS_SYS_TEECOPEN_PARAM)pArgs;
            u32Ret = psSYSInstPri->fpSYS_TeecOpen(pTeecOpenParam->context, pTeecOpenParam->session, pTeecOpenParam->destination, pTeecOpenParam->connection_method, pTeecOpenParam->connection_data, pTeecOpenParam->operation, pTeecOpenParam->error_origin);
            return u32Ret;
        case MDrv_CMD_SYS_TeecClose:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_TeecClose\n");
            pTeecCloseParam = (PSYS_SYS_TEECCLOSE_PARAM)pArgs;
            psSYSInstPri->fpSYS_TeecClose(pTeecCloseParam->session);
            return TRUE;
        case MDrv_CMD_SYS_TeecInvokeCmd:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_TeecInvokeCmd\n");
            pTeecInvokeCmdParam = (PSYS_SYS_TEECINVOKECMD_PARAM)pArgs;
            u32Ret = psSYSInstPri->fpSYS_TeecInvokeCmd(pTeecInvokeCmdParam->session, pTeecInvokeCmdParam->cmd_id, pTeecInvokeCmdParam->operation, pTeecInvokeCmdParam->error_origin);
            return u32Ret;
        case MDrv_CMD_SYS_TeecFinalContext:
            ULOGD(TAG_SYS, "SYSIoctl - MDrv_CMD_SYS_TeecFinalContext\n");
            pTeecInitContextParam = (PSYS_SYS_TEECINITCONTEXT_PARAM)pArgs;
            psSYSInstPri->fpSYS_TeecFinalizeContext(pTeecInitContextParam->context);
            return TRUE;
#endif
        default:
            break;
    };

	return 0; // FIXME: error code
}

MS_U32 SYSClose(void* pInstance)
{
    UtopiaInstanceDelete(pInstance);

    // Restore resource pri_shm content
    // 1. use moduleID to get module, then try to get resource
    void *pModule = NULL;
    void *pResource = NULL;
    UtopiaModuleGetPtr(MODULE_SYS, &pModule);

    if(UtopiaResourceObtain(pModule, SYS_POOL_ID_SYS0, &pResource) != 0)
    {
        ULOGE(TAG_SYS, "UtopiaResourceObtainToInstant fail\n");
        return FALSE;
    }

    // if get a resource, check the module private SHM(default to be 0, only create at the first time) to decide whether this process can open instance
    void *pSYSResPri = NULL;
    UtopiaResourceGetPrivate(pResource, &pSYSResPri);

    if(*(MS_U32 *)pSYSResPri == 0)
    {
        ULOGE(TAG_SYS, "\033[35mFunction = %s, Line = %d, [SYSClose] Strange resource pri_shm content!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        UtopiaResourceRelease(pResource);
        return FALSE;
    }
    else
    {
        ULOGD(TAG_SYS, "\033[35mFunction = %s, Line = %d, [Multi-process Safe] Release an Instance!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        *(MS_U32 *)pSYSResPri = 0;
        UtopiaResourceRelease(pResource);
        return TRUE;
    }
}


MS_U32 MDrv_SYS_SetPowerState(EN_POWER_MODE u16PowerState)
{
	static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
	MS_U16 u32Return = UTOPIA_STATUS_FAIL;

	if (u16PowerState == E_POWER_SUSPEND)
	{
		_prev_u16PowerState = u16PowerState;
		u32Return = UTOPIA_STATUS_SUCCESS; //SUSPEND_OK
	}
	else if (u16PowerState == E_POWER_RESUME)
	{
		if (_prev_u16PowerState == E_POWER_SUSPEND)
		{
            MDrv_SYS_Init();

			_prev_u16PowerState = u16PowerState;
			u32Return = UTOPIA_STATUS_SUCCESS; //RESUME_OK
		}
		else
		{
			ULOGE(TAG_SYS, "[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
			u32Return = UTOPIA_STATUS_FAIL; //SUSPEND_FAILED
		}
	}
	else
	{
		ULOGE(TAG_SYS, "[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
		u32Return = UTOPIA_STATUS_FAIL;
	}

	return u32Return;
}

//-------------------------------------------------------------------------------------------------
/// Get System Hardware List
//-------------------------------------------------------------------------------------------------

MS_U32 MDrv_SYS_GetIpList(E_SYS_IP_TYPE TYPE)
{
	MS_U32 ret = 0;
	switch(TYPE)
	{
		case E_SYS_IP_ENCODER:
			if(CHIP_IP_MFE)
				ret |= SYS_ENCODER_MFE;
			if(CHIP_IP_VE)
				ret |= SYS_ENCODER_VE;
			return ret;

		case E_SYS_IP_FRONTEND:
			if(CHIP_IP_AVD)
				ret |= SYS_FRONTEND_AVD;
			if(CHIP_IP_DEMOD)
				ret |= SYS_FRONTEND_DEMOD;
			if(CHIP_IP_VBI)
				ret |= SYS_FRONTEND_VBI;
			if(CHIP_IP_VIF)
				ret |= SYS_FRONTEND_VIF;
			return ret;

		case E_SYS_IP_DEMUX:
			if(CHIP_IP_DMX)
				ret |= SYS_DEMUX_DMX;
			return ret;

		case E_SYS_IP_SYSTEM:
			if(CHIP_IP_CEC)
				ret |= SYS_SYSTEM_CEC;
			if(CHIP_IP_MBX)
				ret |= SYS_SYSTEM_MBX;
			if(CHIP_IP_SWI2C)
				ret |= SYS_SYSTEM_SWI2C;
			if(CHIP_IP_BDMA)
				ret |= SYS_SYSTEM_BDMA;
			if(CHIP_IP_CPU)
				ret |= SYS_SYSTEM_CPU;
			if(CHIP_IP_GPIO)
				ret |= SYS_SYSTEM_GPIO;
			if(CHIP_IP_HWI2C)
				ret |= SYS_SYSTEM_HWI2C;
			if(CHIP_IP_IR)
				ret |= SYS_SYSTEM_IR;
			if(CHIP_IP_MIU)
				ret |= SYS_SYSTEM_MIU;
			if(CHIP_IP_MPIF)
				ret |= SYS_SYSTEM_MPIF;
			if(CHIP_IP_MSPI)
				ret |= SYS_SYSTEM_MSPI;
			if(CHIP_IP_PM)
				ret |= SYS_SYSTEM_PM;
			if(CHIP_IP_PWM)
				ret |= SYS_SYSTEM_PWM;
			if(CHIP_IP_PWS)
				ret |= SYS_SYSTEM_PWS;
			if(CHIP_IP_RTC)
				ret |= SYS_SYSTEM_RTC;
			if(CHIP_IP_SAR)
				ret |= SYS_SYSTEM_SAR;
			if(CHIP_IP_URDMA)
				ret |= SYS_SYSTEM_URDMA;
			if(CHIP_IP_WDT)
				ret |= SYS_SYSTEM_WDT;
			return ret;

		case E_SYS_IP_CA:
			if(CHIP_IP_AESDMA)
				ret |= SYS_CA_AESDMA;
			if(CHIP_IP_CA)
				ret |= SYS_CA_CA;
			if(CHIP_IP_PCMCIA)
				ret |= SYS_CA_PCMCIA;
			if(CHIP_IP_SC)
				ret |= SYS_CA_SC;
			return ret;

		case E_SYS_IP_CODEC:
			if(CHIP_IP_GPD)
				ret |= SYS_CODEC_GPD;
			if(CHIP_IP_JPEG)
				ret |= SYS_CODEC_JPEG;
			if(CHIP_IP_VDEC)
				ret |= SYS_CODEC_VDEC;
			return ret;

		case E_SYS_IP_DISPLAY:
			if(CHIP_IP_ACE)
				ret |= SYS_DISPLAY_ACE;
			if(CHIP_IP_DAC)
				ret |= SYS_DISPLAY_DAC;
			if(CHIP_IP_DDC2BI)
				ret |= SYS_DISPLAY_DDC2BI;
			if(CHIP_IP_DIP)
				ret |= SYS_DISPLAY_DIP;
			if(CHIP_IP_DLC)
				ret |= SYS_DISPLAY_DLC;
			if(CHIP_IP_GOP)
				ret |= SYS_DISPLAY_GOP;
			if(CHIP_IP_HDMITX)
				ret |= SYS_DISPLAY_HDMITX;
			if(CHIP_IP_MHL)
				ret |= SYS_DISPLAY_MHL;
			if(CHIP_IP_MVOP)
				ret |= SYS_DISPLAY_MVOP;
			if(CHIP_IP_PNL)
				ret |= SYS_DISPLAY_PNL;
			if(CHIP_IP_XC)
				ret |= SYS_DISPLAY_XC;
			return ret;

		case E_SYS_IP_GFX:
			if(CHIP_IP_GFX)
				ret |= SYS_GFX_GFX;
			return ret;

		case E_SYS_IP_AUDIO:
			if(CHIP_IP_AUDIO)
				ret |= SYS_AUDIO_AUDIO;
			return ret;

		case E_SYS_IP_STORAGE:
			if(CHIP_IP_SERFLASH)
				ret |= SYS_STORAGE_SERFLASH;
			return ret;

		default:
			SYS_DBG_INFO("Not Support IP type %d\n",TYPE);
			break;
	}

	return ret;
}

//FOR INTERNAL USE ONLY
MS_U32 MDrv_SYS_RegisterIoProc(E_SYS_IO_PROC dwIoType, void *pdwIoFunction)
{
    MS_U32  dwReturnStatus = TRUE;

    switch(dwIoType)
    {
        case E_SYS_IO_SWI2C_READ:
        {
            //Register the Function Pointer of SWI2C Read Procedure
            g_SysIoProc.SysSwI2CReadBytes = pdwIoFunction;
        }
        break;

        case E_SYS_IO_SWI2C_WRITE:
        {
            //Register the Function Pointer of SWI2C Write Procedure
            g_SysIoProc.SysSwI2CWriteBytes = pdwIoFunction;
        }
        break;

        default:
        {
            SYS_DBG_INFO("Unknown IO Type\n");
            dwReturnStatus = FALSE;
        }
        break;
    }

    return dwReturnStatus;
}

char* MDrv_SYS_GetUtopiaReleaseLabel(void)
{
    return UTOPIA_RELEASE_LABEL;
}

char utopia_mode[10] = {""};
char tee_mode[10] = {""};

#if defined (MSOS_TYPE_LINUX)
static void _MDrv_SYS_ParserCmdLine(char *name)
{
    char *value = strchr(name, '=');

    if (value == 0) return;
    *value++ = 0;
    if (*name == 0) return;

    if (!strcmp(name,"utopia_mode")) {
            strncpy(utopia_mode, value, sizeof(utopia_mode)-1);
            utopia_mode[sizeof(utopia_mode)-1] = '\n';
        } else if (!strcmp(name,"tee_mode")) {
            strncpy(tee_mode, value, sizeof(tee_mode)-1);
            tee_mode[sizeof(utopia_mode)-1] = '\n';
        }
    //printu("utopia_mode = %s \n",utopia_mode);
    //printu("tee_mode = %s \n",tee_mode);
}

void MDrv_SYS_ReadKernelCmdLine(void)
{
     char cmdline[1024];
     char *ptr;
     int fd;
     fd = open("/proc/cmdline", O_RDONLY);
     if (fd >= 0) {
         int n = read(fd, cmdline, 1023);
         if (n < 0) n = 0;

         /* get rid of trailing newline, it happens */
         if (n > 0 && cmdline[n-1] == '\n') n--;

         cmdline[n] = 0;
         close(fd);
     } else {
         SYS_DBG_INFO("MDrv_SYS_ReadKernelCmdLine can't open /proc/cmdline\n");
         cmdline[0] = 0;
     }

     ptr = cmdline;
     while (ptr && *ptr) {
         char *x = strchr(ptr, ' ');
         if (x != 0) *x++ = 0;
         _MDrv_SYS_ParserCmdLine(ptr);
         ptr = x;
     }
 }
#else
void MDrv_SYS_ReadKernelCmdLine(void)
{
    SYS_DBG_INFO("Not Support\n");
}
#endif

MS_U32 MDrv_SYS_SetTEEInfo(MS_U32 osType)
{
    if (osType == SYS_TEEINFO_OSTYPE_OPTEE) {
        strncpy(tee_mode, "optee", sizeof(tee_mode)-1);
        tee_mode[sizeof(tee_mode)-1] = '\n';
    } else if (osType == SYS_TEEINFO_OSTYPE_NUTTX) {
        strncpy(tee_mode, "nuttx", sizeof(tee_mode)-1);
        tee_mode[sizeof(tee_mode)-1] = '\n';
    } else {
        strncpy(tee_mode, "notee", sizeof(tee_mode)-1);
        tee_mode[sizeof(tee_mode)-1] = '\n';
    }

    return 0;
}

MS_U32 MDrv_SYS_GetTEEInfo(SYS_TEEINFO* TeeInfo)
{
    if (!strcmp(tee_mode,"optee")) {
        TeeInfo->OsType = SYS_TEEINFO_OSTYPE_OPTEE;
    } else if (!strcmp(tee_mode,"nuttx")) {
        TeeInfo->OsType = SYS_TEEINFO_OSTYPE_NUTTX;
    }else{
        TeeInfo->OsType = SYS_TEEINFO_OSTYPE_NOTEE;
    }

    return 0;
}

E_SYS_ReturnValue  _MDrv_SYS_XcInputMapping (MS_U32 u32SourceType, E_SYS_SOURCE_TYPE *retSrcType)
{
    //
    // Input Source Mapping,
    //
    switch (u32SourceType)
    {
    case E_SYS_INPUT_SOURCE_DTV:
    case E_SYS_INPUT_SOURCE_DTV2:
    case E_SYS_INPUT_SOURCE_DTV3:
        *retSrcType = E_SYS_SOURCE_DTV;
        break;

    case E_SYS_INPUT_SOURCE_HDMI:
    case E_SYS_INPUT_SOURCE_HDMI2:
    case E_SYS_INPUT_SOURCE_HDMI3:
    case E_SYS_INPUT_SOURCE_HDMI4:
        *retSrcType = E_SYS_SOURCE_HDMI;
        break;

    case E_SYS_INPUT_SOURCE_STORAGE:
        *retSrcType = E_SYS_SOURCE_MM;
        break;

    case E_SYS_INPUT_SOURCE_GOOGLE_CAST:
        *retSrcType = E_SYS_SOURCE_GOOGLE_CAST;
        break;

    default:
        SYS_DBG_ERR ("Not support! Source type: %d\n", u32SourceType);
        return E_SYS_ERROR_PARAMETER;
    }
    return E_SYS_OK;
}

E_SYS_ReturnValue  _MDrv_SYS_XcTimingMapping (MS_U32 u32Width, MS_U32 u32Height, MS_U32 u32FrameRate, E_SYS_INPUT_TIMING *retTimingType)
{
#define TIMING_TOLERANCE 20
#define TIMING_FHD_W     1920
#define TIMING_FHD_H     1080
#define TIMING_4K_W      3840
#define TIMING_4K_H      2160

    MS_U32 _u32Width = 0x0, _u32Height = 0x0;

    if ( u32Width <= (TIMING_FHD_W+TIMING_TOLERANCE) &&
         u32Width >= (TIMING_FHD_W-TIMING_TOLERANCE))
    {
        _u32Width = TIMING_FHD_W;
    }
    else if ( u32Width >= (TIMING_4K_W-TIMING_TOLERANCE))
    {
        //To set the width to 4K_W if the width is large than TIMING_4K_W
        _u32Width = TIMING_4K_W;
    }

    if ( u32Height <= (TIMING_FHD_H+TIMING_TOLERANCE) &&
         u32Height >= (TIMING_FHD_H-TIMING_TOLERANCE))
    {
        _u32Height = TIMING_FHD_H;
    }
    else if ( u32Height >= (TIMING_4K_H-TIMING_TOLERANCE))
    {
        //To set the height to 4K_W if the height is large than TIMING_4K_H
        _u32Height = TIMING_4K_H;
    }

    if (_u32Width == TIMING_FHD_W && _u32Height == TIMING_FHD_H)
    {
        switch (u32FrameRate)
        {
        case 24:
        case 25:
        case 30:
            *retTimingType = E_SYS_INPUT_FHD24_25_30;
            break;
        case 48:
            *retTimingType = E_SYS_INPUT_FHD48;
            break;
        case 50:
        case 59:
        case 60:
            *retTimingType = E_SYS_INPUT_FHD50_60;
            break;
        default:
            SYS_DBG_ERR ("FHD with unknown frame rate: %d\n", u32FrameRate);
            *retTimingType = E_SYS_INPUT_FHD_UNKNOWN;
            break;
        }
    }
    else if (_u32Width == TIMING_4K_W && _u32Height == TIMING_4K_H)
    {
        switch (u32FrameRate)
        {
        case 24:
        case 25:
        case 30:
            *retTimingType = E_SYS_INPUT_4K2K24_25_30;
            break;
        case 48:
            *retTimingType = E_SYS_INPUT_4K2K48;
            break;
        case 50:
        case 59:
        case 60:
            *retTimingType = E_SYS_INPUT_4K2K50_60;
            break;
        case 120:
            *retTimingType = E_SYS_INPUT_4K2K120;
            break;
        default:
            //Special Case
            if (u32FrameRate >=100 && u32FrameRate < 120)
            {
                *retTimingType = E_SYS_INPUT_4K2K120;
            }
            else
            {
                SYS_DBG_ERR ("4k2k with unknown frame rate: %d\n", u32FrameRate);
                *retTimingType = E_SYS_INPUT_4K2K_UNKNOWN;
            }
            SYS_DBG_ERR ("4k2k with unknown frame rate: %d\n", u32FrameRate);
            break;
        }
    }
    else
    {
        SYS_DBG_ERR ("Not Mapped timing! W: %d, H: %d, F: %d\n", u32Width, u32Height, u32FrameRate);
        return E_SYS_NOT_SUPPORT;
    }

    return E_SYS_OK;
}

E_SYS_ReturnValue  MDrv_SYS_GetMemcConfg(MS_U32 u32SourceType, SYS_XC_TIMING_t *stXCTiming, MS_BOOL *retEnMemc)
{
    E_SYS_SOURCE_TYPE eSource = E_SYS_SOURCE_DTV;
    E_SYS_INPUT_TIMING eTiming = E_SYS_INPUT_MAX;

    if (stXCTiming == NULL || retEnMemc == NULL)
    {
        SYS_DBG_ERR ("stXCTiming == NULL || retEnMemc == NULL\n");
        return E_SYS_ERROR_PARAMETER;
    }

    if (stXCTiming->u32TimingVersion <= MDRV_SYS_XC_TIMING_VERSION &&
        stXCTiming->u32StructLength <= sizeof (SYS_XC_TIMING_t))
    {

        SYS_DBG_INFO ("Source: %d, u32Width: %d, u32Height: %d, u16Framerate: %d\n",
                      u32SourceType, stXCTiming->u32Width, stXCTiming->u32Height, stXCTiming->u16Framerate);

        //
        // Input Source Type Mapping
        //
        if ( _MDrv_SYS_XcInputMapping (u32SourceType, &eSource) != E_SYS_OK)
        {
            SYS_DBG_ERR ("Unmapped source type: %d\n", u32SourceType);
            return E_SYS_ERROR_PARAMETER;
        }

        //
        // Input Timing Mapping
        //
        if ( _MDrv_SYS_XcTimingMapping (stXCTiming->u32Width, stXCTiming->u32Height, stXCTiming->u16Framerate, &eTiming) != E_SYS_OK)
        {
            //
            //Error Handling,
            //Unable to determeternate the timing,
            //Config is up to frame rate only.
            //
            SYS_DBG_ERR ("Not support! Timing, W:%d, H:%d, F:%d\n", stXCTiming->u32Width, stXCTiming->u32Height, stXCTiming->u16Framerate);
            if (stXCTiming->u16Framerate > 30 )
            {
                eTiming = E_SYS_INPUT_ALWAYS_OFF;
            }
            else
            {
                eTiming = E_SYS_INPUT_ALWAYS_ON;
            }
        }
    } //End of Version check,  #define MDRV_SYS_XC_TIMING_VERSION 0x00000001UL
    else
    {
        SYS_DBG_ERR ("Version Error! Please update drvSYS\n");
        return E_SYS_FAIL;
    }

    return HAL_SYS_GetMemcConfg (eSource, eTiming, retEnMemc);
}

E_SYS_ReturnValue  _MDrv_SYS_GetXcByPartConfg(MS_U32 u32SourceType, SYS_XC_BYPART_TIMING_t *stInfo, MS_BOOL *retEn)
{
    E_SYS_SOURCE_TYPE eSource = E_SYS_SOURCE_DTV;
    E_SYS_INPUT_TIMING eInputTiming = E_SYS_INPUT_MAX;
    E_SYS_INPUT_TIMING eOutputTiming = E_SYS_INPUT_MAX;

    if (stInfo == NULL || retEn == NULL)
    {
        SYS_DBG_ERR ("stInfo == NULL || retEnMemc == NULL\n");
        return E_SYS_ERROR_PARAMETER;
    }

    if (stInfo->u32TimingVersion <= MDRV_SYS_XC_BYPART_TIMING_VERSION &&
        stInfo->u32StructLength <= sizeof (SYS_XC_BYPART_TIMING_t))
    {
        SYS_DBG_INFO ("Source: %d, Input(W: %d, H: %d, F: %d), Output(W: %d, H: %d, F: %d)\n",
                      u32SourceType, stInfo->stInput.u32Width, stInfo->stInput.u32Height, stInfo->stInput.u16Framerate,
                      stInfo->stOutput.u32Width, stInfo->stOutput.u32Height, stInfo->stOutput.u16Framerate);

        //
        // Input Source Type Mapping
        //
        if ( _MDrv_SYS_XcInputMapping (u32SourceType, &eSource) != E_SYS_OK)
        {
            SYS_DBG_ERR ("Unmapped source type: %d\n", u32SourceType);
            return E_SYS_ERROR_PARAMETER;
        }

        //
        // Input Timing Mapping
        //
        _MDrv_SYS_XcTimingMapping (stInfo->stInput.u32Width, stInfo->stInput.u32Height, stInfo->stInput.u16Framerate, &eInputTiming);
        _MDrv_SYS_XcTimingMapping (stInfo->stOutput.u32Width, stInfo->stOutput.u32Height, stInfo->stOutput.u16Framerate, &eOutputTiming);

    }
    return HAL_SYS_GetXcByPartConfg (eSource, eInputTiming, eOutputTiming, retEn);
}

E_SYS_ReturnValue  MDrv_SYS_GetQoSConfig(E_SYS_QOS_QUERY_TYPE eQueryType, MS_U32 u32SourceType, void *stInfo, MS_BOOL *retEn)
{
    E_SYS_ReturnValue eRet = E_SYS_NOT_SUPPORT;

    if (stInfo == NULL || retEn == NULL)
    {
        SYS_DBG_ERR ("Error Parameters\n");
        return E_SYS_ERROR_PARAMETER;
    }

    switch (eQueryType)
    {
    case E_SYS_QOS_QUERY_XC_BYPART_EN:
        eRet = _MDrv_SYS_GetXcByPartConfg (u32SourceType, (SYS_XC_BYPART_TIMING_t *)stInfo, retEn);
        break;

    case E_SYS_QOS_QUERY_MEMC_EN:
        eRet = MDrv_SYS_GetMemcConfg (u32SourceType, (SYS_XC_TIMING_t *)stInfo, retEn);
        break;

    default:
        SYS_DBG_ERR ("Unknown Query Type: %d\n", eQueryType);
        eRet = E_SYS_ERROR_PARAMETER;
        break;
    };

    return eRet;
}
