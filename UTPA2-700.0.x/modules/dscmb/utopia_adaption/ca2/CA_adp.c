#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>

#include "MsTypes.h"
#include "utopia.h"
#include "drvCA.h"
#include "drvCA_v2.h"
#include "CA_adp.h"

#include "utopia_adp.h"

#define SPT_TYPE_A_SIZE 2
#define SPT_TYPE_B_SIZE 3
#define SPT_TYPE_C_SIZE 4
#define SPT_TYPE_D_SIZE 5
#define SPT_TYPE_E_SIZE 6
#define SPT_TYPE_F_SIZE 7

#define UADP_SPT_NAME0NXT_SIZE(NAME,SIZE) \
            UADP_SPT_BGN(&NAME [0],SIZE); \
            UADP_SPT_FIN(&NAME [1]);

#define UADP_SPT_NAME0NXT_TYPE(NAME,TYPE) \
            UADP_SPT_BGN(&NAME [0],sizeof(TYPE)); \
            UADP_SPT_FIN(&NAME [1]);

#define UADP_SPT_NAME1NXT_TYPE(NAME,TYPE,MB1,MB1NAME) \
            UADP_SPT_BGN(&NAME [0],sizeof(TYPE)); \
            UADP_SPT_NXT(&NAME [1],TYPE,MB1,MB1NAME); \
            UADP_SPT_FIN(&NAME [2]);

#define UADP_SPT_NAME2NXT_TYPE(NAME,TYPE,MB1,MB1NAME,MB2,MB2NAME) \
            UADP_SPT_BGN(&NAME [0],sizeof(TYPE)); \
            UADP_SPT_NXT(&NAME [1],TYPE,MB1,MB1NAME); \
            UADP_SPT_NXT(&NAME [2],TYPE,MB2,MB2NAME); \
            UADP_SPT_FIN(&NAME [3]);

#define UADP_SPT_NAME3NXT_TYPE(NAME,TYPE,MB1,MB1NAME,MB2,MB2NAME,MB3,MB3NAME) \
            UADP_SPT_BGN(&NAME [0],sizeof(TYPE)); \
            UADP_SPT_NXT(&NAME [1],TYPE,MB1,MB1NAME); \
            UADP_SPT_NXT(&NAME [2],TYPE,MB2,MB2NAME); \
            UADP_SPT_NXT(&NAME [3],TYPE,MB3,MB3NAME); \
            UADP_SPT_FIN(&NAME [4]);

#define UADP_SPT_NAME5NXT_TYPE(NAME,TYPE,MB1,MB1NAME,MB2,MB2NAME,MB3,MB3NAME,MB4,MB4NAME,MB5,MB5NAME) \
            UADP_SPT_BGN(&NAME [0],sizeof(TYPE)); \
            UADP_SPT_NXT(&NAME [1],TYPE,MB1,MB1NAME); \
            UADP_SPT_NXT(&NAME [2],TYPE,MB2,MB2NAME); \
            UADP_SPT_NXT(&NAME [3],TYPE,MB3,MB3NAME); \
            UADP_SPT_NXT(&NAME [4],TYPE,MB4,MB4NAME); \
            UADP_SPT_NXT(&NAME [5],TYPE,MB5,MB5NAME); \
            UADP_SPT_FIN(&NAME [6]);

//TOP
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_SetBlockLock[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_GetBlockLock[SPT_TYPE_B_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_IsBlank[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_Read[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_Write[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_Locked[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_Lock[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_SetRSAextID[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_GetRSAextID[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_SetHASH1_REF_VER[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_GetHASH1_REF_VER[SPT_TYPE_B_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_SetHASH_REF_VER[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_GetHASH_REF_VER[SPT_TYPE_C_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_SetDebugPortMode[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_GetDebugPortMode[SPT_TYPE_B_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_SetDeviceId[SPT_TYPE_B_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_GetDeviceId[SPT_TYPE_B_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_MaxDeviceIdSize[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_Random[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_ReadCAVendor[SPT_TYPE_A_SIZE];

//BOTTOM
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_GetBlockLock_peLockType[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_GetHASH1_REF_VER_pu32HASH1_REF_VER[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_GetHASH_REF_VER_pu32HASH0_REF_VER[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_GetHASH_REF_VER_pu32HASH2_REF_VER[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_GetDebugPortMode_eMode[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_SetDeviceId_pu8Did[SPT_TYPE_A_SIZE];
UADP_STRUCT_POINTER_TABLE spt_CA_OTP_GetDeviceId_pu8Did[SPT_TYPE_A_SIZE];

MS_U32 DSCMB_adp_Init(FUtopiaIOctl* pIoctl)
{
    //TOP
    UADP_SPT_NAME0NXT_TYPE(spt_CA_OTP_SetBlockLock, CA_OTP_SETBLOCKLOCK);
    UADP_SPT_NAME1NXT_TYPE(spt_CA_OTP_GetBlockLock, CA_OTP_GETBLOCKLOCK, peLockType, spt_CA_OTP_GetBlockLock_peLockType);
    UADP_SPT_NAME0NXT_TYPE(spt_CA_OTP_IsBlank, CA_OTP_ISBLANK);
    UADP_SPT_NAME0NXT_TYPE(spt_CA_OTP_Read, CA_OTP_READ);
    UADP_SPT_NAME0NXT_TYPE(spt_CA_OTP_Write, CA_OTP_WRITE);
    UADP_SPT_NAME0NXT_TYPE(spt_CA_Locked, CA_OTP_LOCK20);
    UADP_SPT_NAME0NXT_TYPE(spt_CA_OTP_Lock, CA_OTP_LOCK20);
    UADP_SPT_NAME0NXT_TYPE(spt_CA_OTP_SetRSAextID, CA_OTP_SETRSAEXTID);
    UADP_SPT_NAME0NXT_TYPE(spt_CA_OTP_GetRSAextID, CA_OTP_GETRSAEXTID);
    UADP_SPT_NAME0NXT_TYPE(spt_CA_OTP_SetHASH1_REF_VER, CA_OTP_SETHASH1REFVER);
    UADP_SPT_NAME1NXT_TYPE(spt_CA_OTP_GetHASH1_REF_VER, CA_OTP_GETHASH1REFVER, pu32HASH1_REF_VER, spt_CA_OTP_GetHASH1_REF_VER_pu32HASH1_REF_VER);
    UADP_SPT_NAME0NXT_TYPE(spt_CA_OTP_SetHASH_REF_VER, CA_OTP_SETHASHREFVER);
    UADP_SPT_NAME2NXT_TYPE(spt_CA_OTP_GetHASH_REF_VER, CA_OTP_GETHASHREFVER, pu32HASH0_REF_VER, spt_CA_OTP_GetHASH_REF_VER_pu32HASH0_REF_VER, pu32HASH2_REF_VER, spt_CA_OTP_GetHASH_REF_VER_pu32HASH2_REF_VER);
    UADP_SPT_NAME0NXT_TYPE(spt_CA_OTP_SetDebugPortMode, CA_OTP_SETDEBUGPORTMODE);
    UADP_SPT_NAME1NXT_TYPE(spt_CA_OTP_GetDebugPortMode, CA_OTP_GETDEBUGPORTMODE, eMode, spt_CA_OTP_GetDebugPortMode_eMode);
    UADP_SPT_NAME1NXT_TYPE(spt_CA_OTP_SetDeviceId, CA_OTP_SETDEVICEID, pu8Did, spt_CA_OTP_SetDeviceId_pu8Did);
    UADP_SPT_NAME1NXT_TYPE(spt_CA_OTP_GetDeviceId, CA_OTP_SETDEVICEID, pu8Did, spt_CA_OTP_GetDeviceId_pu8Did);
    UADP_SPT_NAME0NXT_TYPE(spt_CA_MaxDeviceIdSize, CA_MAXDEVICEIDSIZE);
    UADP_SPT_NAME0NXT_TYPE(spt_CA_Random, CA_RANDOM);
    UADP_SPT_NAME0NXT_TYPE(spt_CA_OTP_ReadCAVendor, CA_OTP_READCAVENDOR);

    //BOTTOM
    UADP_SPT_NAME0NXT_TYPE(spt_CA_OTP_GetBlockLock_peLockType, CA_LOCK_TYPE);
    UADP_SPT_NAME0NXT_TYPE(spt_CA_OTP_GetHASH1_REF_VER_pu32HASH1_REF_VER, MS_U32);
    UADP_SPT_NAME0NXT_TYPE(spt_CA_OTP_GetHASH_REF_VER_pu32HASH0_REF_VER, MS_U32);
    UADP_SPT_NAME0NXT_TYPE(spt_CA_OTP_GetHASH_REF_VER_pu32HASH2_REF_VER, MS_U32);
    UADP_SPT_NAME0NXT_TYPE(spt_CA_OTP_GetDebugPortMode_eMode, CA_DEBUG_PORT_MODE);
    UADP_SPT_NAME0NXT_SIZE(spt_CA_OTP_SetDeviceId_pu8Did, sizeof(MS_U8)*16);
    UADP_SPT_NAME0NXT_SIZE(spt_CA_OTP_GetDeviceId_pu8Did, sizeof(MS_U8)*16);

    *pIoctl = (FUtopiaIOctl)CA_adp_Ioctl;

    return 0;

}

MS_U32 DSCMB_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret = 0;
    char buffer_arg[32];
    UADP_STRUCT_POINTER_TABLE* sptPtr;
    switch(u32Cmd)
    {
        case E_MDRV_CMD_CA_Init:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
            break;
        case E_MDRV_CMD_CA_OTP_EnableSecureBoot:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
            break;
        case E_MDRV_CMD_CA_OTP_IsSecureBootEnabled:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
            break;
        case E_MDRV_CMD_CA_OTP_SetBlockLock:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_CA_OTP_SetBlockLock, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_CA_OTP_GetBlockLock:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_CA_OTP_GetBlockLock, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_CA_OTP_IsBlank:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_CA_OTP_IsBlank, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_CA_OTP_Read:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_CA_OTP_Read, spt_CA_OTP_Read, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_CA_OTP_Write:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_CA_OTP_Write, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_CA_Locked:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_CA_Locked, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_CA_OTP_Lock:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_CA_OTP_Lock, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_CA_OTP_SetRSAextID:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_CA_OTP_SetRSAextID, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_CA_OTP_GetRSAextID:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_CA_OTP_GetRSAextID, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_CA_OTP_SetHASH1_REF_VER:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_CA_OTP_SetHASH1_REF_VER, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_CA_OTP_GetHASH1_REF_VER:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_CA_OTP_GetHASH1_REF_VER, spt_CA_OTP_GetHASH1_REF_VER, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_CA_OTP_SetHASH_REF_VER:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_CA_OTP_SetHASH_REF_VER, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_CA_OTP_SetDebugPortMode:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_CA_OTP_SetDebugPortMode, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_CA_OTP_GetDebugPortMode:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_CA_OTP_GetDebugPortMode, spt_CA_OTP_GetDebugPortMode, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_CA_OTP_SetDeviceId:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_CA_OTP_SetDeviceId, NULL, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_CA_MaxDeviceIdSize:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_CA_MaxDeviceIdSize, spt_CA_MaxDeviceIdSize, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_CA_OTP_EnableSecureCWMode:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
            break;
        case E_MDRV_CMD_CA_OTP_IsSecureCWMode:
            u32Ret = UtopiaIoctl(pInstanceTmp, u32Cmd, pArgs);
            break;
        case E_MDRV_CMD_CA_Random:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_CA_Random, spt_CA_Random, buffer_arg, sizeof(buffer_arg));
            break;
        case E_MDRV_CMD_CA_OTP_ReadCAVendor:
            u32Ret = UADPBypassIoctl(pInstanceTmp, u32Cmd, pArgs, spt_CA_OTP_ReadCAVendor, spt_CA_OTP_ReadCAVendor, buffer_arg, sizeof(buffer_arg));
            break;
        default:
            break;
    }

    return u32Ret;
}
