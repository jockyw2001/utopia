#ifndef __DRV_CA_V2_H__
#define __DRV_CA_V2_H__

#include "drvCA.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
    E_MDRV_CMD_CA_Init,
    E_MDRV_CMD_CA_OTP_EnableSecureBoot,
    E_MDRV_CMD_CA_OTP_IsSecureBootEnabled,
    E_MDRV_CMD_CA_OTP_SetBlockLock,
    E_MDRV_CMD_CA_OTP_GetBlockLock,
    E_MDRV_CMD_CA_OTP_IsBlank,
    E_MDRV_CMD_CA_OTP_Read,
    E_MDRV_CMD_CA_OTP_Write,
    E_MDRV_CMD_CA_Locked,
    E_MDRV_CMD_CA_OTP_Lock,
    E_MDRV_CMD_CA_OTP_SetRSAextID,
    E_MDRV_CMD_CA_OTP_GetRSAextID,
    E_MDRV_CMD_CA_OTP_SetHASH1_REF_VER,
    E_MDRV_CMD_CA_OTP_GetHASH1_REF_VER,
    E_MDRV_CMD_CA_OTP_SetHASH_REF_VER,
    E_MDRV_CMD_CA_OTP_GetHASH_REF_VER,
    E_MDRV_CMD_CA_OTP_SetDebugPortMode,
    E_MDRV_CMD_CA_OTP_GetDebugPortMode,
    E_MDRV_CMD_CA_OTP_SetDeviceId,
    E_MDRV_CMD_CA_OTP_GetDeviceId,
    E_MDRV_CMD_CA_MaxDeviceIdSize,
    E_MDRV_CMD_CA_OTP_EnableSecureCWMode,
    E_MDRV_CMD_CA_OTP_IsSecureCWMode,
    E_MDRV_CMD_CA_Random,
    E_MDRV_CMD_CA_OTP_ReadCAVendor,

} eCAIoctlOpt;


typedef struct _CA_OTP_SETBLOCKLOCK
{
    MS_U32 u32Start;
    MS_U32 u32End;
    CA_LOCK_TYPE eLockType;
} CA_OTP_SETBLOCKLOCK;

typedef struct _CA_OTP_GETBLOCKLOCK
{
    MS_U32 *pu32Start;
    MS_U32 *pu32End;
    CA_LOCK_TYPE *peLockType;
} CA_OTP_GETBLOCKLOCK;

typedef struct _CA_OTP_ISBLANK
{
    MS_U32 u32addr;
} CA_OTP_ISBLANK;

typedef struct _CA_OTP_READ
{
    MS_U32 u32Addr;
    MS_U32 u32OTPValue;
} CA_OTP_READ;

typedef struct _CA_OTP_WRITE
{
    MS_U32 u32Addr;
    MS_U32 u32value;
} CA_OTP_WRITE;

typedef struct _CA_OTP_LOCK20
{
    CA_OTP_LOCK eLock;
} CA_OTP_LOCK20;

typedef struct _CA_OTP_SETRSAEXTID
{
    MS_U32 u32value;
} CA_OTP_SETRSAEXTID;

typedef struct _CA_OTP_GETRSAEXTID
{
    MS_U32 u32value;
} CA_OTP_GETRSAEXTID;

typedef struct _CA_OTP_SETHASH1REFVER
{
    MS_U32 u32HASH1_REF_VER;
} CA_OTP_SETHASH1REFVER;

typedef struct _CA_OTP_GETHASH1REFVER
{
    MS_U32 *pu32HASH1_REF_VER;
} CA_OTP_GETHASH1REFVER;

typedef struct _CA_OTP_SETHASHREFVER
{
    MS_U32 u32HASH0_REF_VER;
    MS_U32 u32HASH2_REF_VER;
} CA_OTP_SETHASHREFVER;

typedef struct _CA_OTP_GETHASHREFVER
{
    MS_U32 *pu32HASH0_REF_VER;
    MS_U32 *pu32HASH2_REF_VER;
} CA_OTP_GETHASHREFVER;

typedef struct _CA_OTP_SETDEBUGPORTMODE
{
    CA_DEBUG_PORT eDebugPort;
    CA_DEBUG_PORT_MODE eMode;
} CA_OTP_SETDEBUGPORTMODE;

typedef struct _CA_OTP_GETDEBUGPORTMODE
{
    CA_DEBUG_PORT eDebugPort;
    CA_DEBUG_PORT_MODE *eMode;
} CA_OTP_GETDEBUGPORTMODE;

typedef struct _CA_OTP_SETDEVICEID
{
    const MS_U8 *pu8Did;
    MS_U32 u32Size;
} CA_OTP_SETDEVICEID;

typedef struct _CA_OTP_GETDEVICEID
{
    MS_U8 *pu8Did;
    MS_U32 u32Size;
} CA_OTP_GETDEVICEID;

typedef struct _CA_MAXDEVICEIDSIZE
{
    MS_U32 u32MaxDeviceIdSize;
} CA_MAXDEVICEIDSIZE;

typedef struct _CA_RANDOM
{
    MS_U32 u32Random;
} CA_RANDOM;

typedef struct _CA_OTP_READCAVENDOR
{
    MS_U32 u32CAVendor;
} CA_OTP_READCAVENDOR;

MS_U32 CAOpen(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 CAClose(void* pInstantTmp);
MS_U32 CAIoctl(void* pInstantTmp, MS_U32 u32Cmd, void* pu32Args);

#ifdef __cplusplus
}
#endif

#endif // __DRV_CA_V2_H__
