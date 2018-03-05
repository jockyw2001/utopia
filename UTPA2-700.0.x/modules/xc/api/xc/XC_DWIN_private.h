#include "mvideo_context.h"

#include "MsTypes.h"
typedef struct _DIP_SHARE_RESOURCE_PRIVATE
{
	XC_DIP_InternalStatus  g_DIPSrcInfo[MAX_DIP_WINDOW];
    MS_BOOL bLock[MAX_DIP_WINDOW];
}DIP_SHARE_RESOURCE_PRIVATE;

typedef struct _DIP_RESOURCE_PRIVATE
{

}DIP_RESOURCE_PRIVATE;

typedef struct _DIP_INSTANCE_PRIVATE
{
    MS_BOOL bInit;
    MS_U8 u8DbgLevel;
    SCALER_DIP_WIN eWindow;
    void* pResource;
    void* pShareResource;
}DIP_INSTANCE_PRIVATE;

void DIPRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 DIPOpen(void** ppInstance, const void* const pAttribute);
MS_U32 DIPClose(void* pInstance);
MS_U32 DIPIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
MS_U32 DIPMdbIoctl(MS_U32 u32Cmd, const void* const pArgs);
#endif
