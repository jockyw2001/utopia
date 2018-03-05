#ifndef __CA_PRIVATE__
#define __CA_PRIVATE__

#include "halCA.h"

typedef enum {
    E_CA_RESOURCE,
} eCAResourceId;


typedef struct _CA_RESOURCE_PRIVATE
{

} CA_RESOURCE_PRIVATE;

typedef struct _CA_INSTANT_PRIVATE
{

} CA_INSTANT_PRIVATE;


MS_BOOL _MDrv_CA_Init(void);
MS_BOOL _MDrv_CA_OTP_EnableSecureBoot(void);
MS_BOOL _MDrv_CA_OTP_IsSecureBootEnabled(void);
MS_BOOL _MDrv_CA_OTP_SetBlockLock(MS_U32 u32Start, MS_U32 u32End, CA_LOCK_TYPE eLockType);
MS_BOOL _MDrv_CA_OTP_GetBlockLock(MS_U32 *pu32Start, MS_U32 *pu32End, CA_LOCK_TYPE *peLockType);
MS_BOOL _MDrv_CA_OTP_IsBlank(MS_U32 addr);
MS_U32 _MDrv_CA_OTP_Read(MS_U32 addr);
MS_BOOL _MDrv_CA_OTP_Write(MS_U32 addr, MS_U32 value);
MS_BOOL _MDrv_CA_Locked(CA_OTP_LOCK eLock);
MS_BOOL _MDrv_CA_OTP_Lock(CA_OTP_LOCK eLock);
MS_BOOL _MDrv_CA_OTP_SetRSAextID(MS_U32 u32Value);
MS_U32 _MDrv_CA_OTP_GetRSAextID(void);
MS_BOOL _MDrv_CA_OTP_SetHASH1_REF_VER(MS_U32 u32HASH1_REF_VER);
MS_BOOL _MDrv_CA_OTP_GetHASH1_REF_VER(MS_U32 *pu32HASH1_REF_VER);
MS_BOOL _MDrv_CA_OTP_SetHASH_REF_VER(MS_U32 u32HASH0_REF_VER, MS_U32 u32HASH2_REF_VER);
MS_BOOL _MDrv_CA_OTP_GetHASH_REF_VER(MS_U32 *pu32HASH0_REF_VER, MS_U32 *pu32HASH2_REF_VER);
MS_BOOL _MDrv_CA_OTP_SetDebugPortMode(CA_DEBUG_PORT eDebugPort, CA_DEBUG_PORT_MODE eMode);
MS_BOOL _MDrv_CA_OTP_GetDebugPortMode(CA_DEBUG_PORT eDebugPort, CA_DEBUG_PORT_MODE *eMode);
MS_BOOL _MDrv_CA_OTP_SetDeviceId(const MS_U8 *pu8Did, MS_U32 u32Size);
MS_BOOL _MDrv_CA_OTP_GetDeviceId(MS_U8 *pu8Did, MS_U32 u32Size);
MS_U32 _MDrv_CA_MaxDeviceIdSize(void);
MS_BOOL _MDrv_CA_OTP_EnableSecureCWMode(void);
MS_BOOL _MDrv_CA_OTP_IsSecureCWMode(void);
MS_U32 _MDrv_CA_Random(void);
MS_U32 _MDrv_CA_OTP_ReadCAVendor(void);

#endif // __CA_PRIVATE__
