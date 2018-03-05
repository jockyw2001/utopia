#ifndef __DSCMB_PRIVATE__
#define __DSCMB_PRIVATE__

#include "regDSCMB.h"


#ifdef __cplusplus
extern "C"
{
#endif

#define MAGIC_NUM 0x12345678

typedef enum {
    E_DSCMB_RESOURCE,
} eDscmbResourceId;

typedef struct
{
    MS_U32 u32SlotIdOdd;
    MS_U32 u32SlotIdEven;
    MS_U32 u32SlotIdClear;
    MS_U32 u32PidFltId[MAX_DSCMB_PIDFLT_NUM];
    MS_U32 u32PidFltIdNum;
    MS_U32 u32DscmbId;
    MS_BOOL bUsed;
    MS_BOOL bDscmb;
    DSCMB_SLOT_Type SlotType;
    DSCMB_Type DscmbType;
} stDscmbTspMap;

typedef struct _DSCMB_RESOURCE_PRIVATE
{
    stDscmbTspMap DscmbTspMap[ENG_NUM][MAX_NUM];
    MS_U32        u32FreeSlot[ENG_NUM];  
    MS_BOOL       bInited_Drv;   
    MS_U32        u32Magic;
} DSCMB_RESOURCE_PRIVATE;


typedef struct _DSCMB_INSTANT_PRIVATE
{
    MS_U8           pInitVector[DSCMB_IV_LENGTH];
    MS_U8           pCipherKey[DSCMB_KEY_LENGTH];
    DSCMB_Type      eType;
    DSCMB_Key_Type  eKeyType;
} DSCMB_INSTANT_PRIVATE;


// descrambler initial function
MS_BOOL _MDrv_DSCMB_Init(void);

// descrambler finalize function
MS_BOOL _MDrv_DSCMB_Exit(void);

// descrambler query function
MS_BOOL _MDrv_DSCMB2_GetCap(MS_U32 u32EngId, DSCMB_Query_Type eQueryType, void* pInput, void* pOutput);

// descrambler filter management
MS_U32 _MDrv_DSCMB2_FltAlloc_Ex(MS_U32 u32EngId, DSCMB_Flt_Type eFltType);
MS_BOOL _MDrv_DSCMB2_FltFree(MS_U32 u32EngId, MS_U32 u32DscmbId);

// descrambler filter maps to TSP pid
MS_BOOL _MDrv_DSCMB2_FltConnectPid(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32Pid);
MS_BOOL _MDrv_DSCMB2_FltDisconnectPid(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32Pid);
MS_BOOL _MDrv_DSCMB2_FltDisconnectPid_Ex(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32Pid);

// descrambler filter maps to TSP filter id
MS_BOOL _MDrv_DSCMB2_FltConnectFltId(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32FltId);
MS_BOOL _MDrv_DSCMB2_FltDisconnectFltId(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_U32 u32FltId);



// Set descrambler filter as scrambling/decrambling mode
// Default filter mode is descrambling if this function is not specified
// This function should be invoked before MDrv_DSCMB_FltTypeSet
MS_BOOL _MDrv_DSCMB2_FltDscmb(MS_U32 u32EngId, MS_U32 u32DscmbId, MS_BOOL bDscmb);

// descrambler algorithm
MS_BOOL _MDrv_DSCMB2_FltTypeSet(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Type);

// descrambler miscellaneous option config
MS_BOOL _MDrv_DSCMB2_OptConfig(MS_U32 u32EngId, DSCMB_Option eOpt, void* pInput, void* pOutput, MS_BOOL bSet);


#if (!( defined(TSP_VERSION) && (TSP_VERSION == TSP_VER_2_0)))
// descrambler altorithm related
MS_BOOL _MDrv_DSCMB2_Multi2_SetRound(MS_U32 u32EngId, MS_U32 u32Round);
MS_BOOL _MDrv_DSCMB2_Multi2_SetSystemKey(MS_U32 u32EngId, MS_U8* u8Syskey);
MS_BOOL _MDrv_DSCMB2_HDCP2_SetRIV(MS_U32 u32EngId, MS_U8* pu8RIV);
#endif
// descrambler key
MS_BOOL _MDrv_DSCMB2_FltKeySet(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, MS_U8* pu8Key);
MS_BOOL _MDrv_DSCMB2_FltKeyReset(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType);
MS_BOOL _MDrv_DSCMB2_FltIVSet(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType, MS_U8* pu8IV);
MS_BOOL _MDrv_DSCMB2_FltKeyReset(MS_U32 u32EngId, MS_U32 u32DscmbId, DSCMB_Key_Type eKeyType);

// scramble status
MS_BOOL _MDrv_DSCMB2_PidFlt_ScmbStatus(MS_U32 u32EngId, MS_U32 u32PidFltId, SCMB_Level* pScmbLevel);
MS_U32 _MDrv_DSCMB2_SetPowerState(EN_POWER_MODE u16PowerState);

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
MS_U32 DSCMBMdbIoctl(MS_U32 cmd, const void* const pArgs);
#endif

#ifdef __cplusplus
}
#endif


#endif // __DSCMB_PRIVATE__


